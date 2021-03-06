/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*-
 * vim: set ts=4 sw=4 et tw=99:
 *
 * ***** BEGIN LICENSE BLOCK *****
 * Version: MPL 1.1/GPL 2.0/LGPL 2.1
 *
 * The contents of this file are subject to the Mozilla Public License Version
 * 1.1 (the "License"); you may not use this file except in compliance with
 * the License. You may obtain a copy of the License at
 * http://www.mozilla.org/MPL/
 *
 * Software distributed under the License is distributed on an "AS IS" basis,
 * WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License
 * for the specific language governing rights and limitations under the
 * License.
 *
 * The Original Code is Mozilla SpiderMonkey JavaScript 1.9 code, released
 * May 28, 2008.
 *
 * The Initial Developer of the Original Code is
 *   Brendan Eich <brendan@mozilla.org>
 *
 * Contributor(s):
 *   David Anderson <danderson@mozilla.com>
 *   David Mandelin <dmandelin@mozilla.com>
 *
 * Alternatively, the contents of this file may be used under the terms of
 * either of the GNU General Public License Version 2 or later (the "GPL"),
 * or the GNU Lesser General Public License Version 2.1 or later (the "LGPL"),
 * in which case the provisions of the GPL or the LGPL are applicable instead
 * of those above. If you wish to allow use of your version of this file only
 * under the terms of either the GPL or the LGPL, and not to allow others to
 * use your version of this file under the terms of the MPL, indicate your
 * decision by deleting the provisions above and replace them with the notice
 * and other provisions required by the GPL or the LGPL. If you do not delete
 * the provisions above, a recipient may use your version of this file under
 * the terms of any one of the MPL, the GPL or the LGPL.
 *
 * ***** END LICENSE BLOCK ***** */

#include "jscntxt.h"
#include "jsscope.h"
#include "jsobj.h"
#include "jslibmath.h"
#include "jsiter.h"
#include "jsnum.h"
#include "jsxml.h"
#include "jsstaticcheck.h"
#include "jsbool.h"
#include "assembler/assembler/MacroAssemblerCodeRef.h"
#include "assembler/assembler/CodeLocation.h"
#include "jsiter.h"
#include "jstypes.h"
#include "methodjit/StubCalls.h"
#include "jstracer.h"
#include "jspropertycache.h"
#include "methodjit/MonoIC.h"
#include "jsanalyze.h"
#include "methodjit/BaseCompiler.h"
#include "methodjit/ICRepatcher.h"

#include "jsinterpinlines.h"
#include "jspropertycacheinlines.h"
#include "jsscopeinlines.h"
#include "jsscriptinlines.h"
#include "jsstrinlines.h"
#include "jsobjinlines.h"
#include "jscntxtinlines.h"
#include "jsatominlines.h"
#include "StubCalls-inl.h"
#include "MethodJIT-inl.h"

#include "jsautooplen.h"

using namespace js;
using namespace js::mjit;
using namespace JSC;

using ic::Repatcher;

static jsbytecode *
FindExceptionHandler(JSContext *cx)
{
    JSStackFrame *fp = cx->fp();
    JSScript *script = fp->script();

top:
    if (cx->isExceptionPending() && JSScript::isValidOffset(script->trynotesOffset)) {
        // The PC is updated before every stub call, so we can use it here.
        unsigned offset = cx->regs->pc - script->main;

        JSTryNoteArray *tnarray = script->trynotes();
        for (unsigned i = 0; i < tnarray->length; ++i) {
            JSTryNote *tn = &tnarray->vector[i];

            // The following if condition actually tests two separate conditions:
            //   (1) offset - tn->start >= tn->length
            //       means the PC is not in the range of this try note, so we
            //       should continue searching, after considering:
            //   (2) offset - tn->start == tn->length
            //       means the PC is at the first op of the exception handler
            //       for this try note. This happens when an exception is thrown
            //       during recording: the interpreter sets the PC to the handler
            //       and then exits. In this case, we are in fact at the right
            //       exception handler. 
            //      
            //       Hypothetically, the op we are at might have thrown an
            //       exception, in which case this would not be the right handler.
            //       But the first ops of exception handlers generated by our
            //       bytecode compiler cannot throw, so this is not possible.
            if (offset - tn->start > tn->length)
                continue;
            if (tn->stackDepth > cx->regs->sp - fp->base())
                continue;

            jsbytecode *pc = script->main + tn->start + tn->length;
            JSBool ok = js_UnwindScope(cx, tn->stackDepth, JS_TRUE);
            JS_ASSERT(cx->regs->sp == fp->base() + tn->stackDepth);

            switch (tn->kind) {
                case JSTRY_CATCH:
                  JS_ASSERT(js_GetOpcode(cx, fp->script(), pc) == JSOP_ENTERBLOCK);

#if JS_HAS_GENERATORS
                  /* Catch cannot intercept the closing of a generator. */
                  if (JS_UNLIKELY(cx->getPendingException().isMagic(JS_GENERATOR_CLOSING)))
                      break;
#endif

                  /*
                   * Don't clear cx->throwing to save cx->exception from GC
                   * until it is pushed to the stack via [exception] in the
                   * catch block.
                   */
                  return pc;

                case JSTRY_FINALLY:
                  /*
                   * Push (true, exception) pair for finally to indicate that
                   * [retsub] should rethrow the exception.
                   */
                  cx->regs->sp[0].setBoolean(true);
                  cx->regs->sp[1] = cx->getPendingException();
                  cx->regs->sp += 2;
                  cx->clearPendingException();
                  return pc;

                case JSTRY_ITER:
                {
                  /*
                   * This is similar to JSOP_ENDITER in the interpreter loop,
                   * except the code now uses the stack slot normally used by
                   * JSOP_NEXTITER, namely regs.sp[-1] before the regs.sp -= 2
                   * adjustment and regs.sp[1] after, to save and restore the
                   * pending exception.
                   */
                  Value v = cx->getPendingException();
                  JS_ASSERT(js_GetOpcode(cx, fp->script(), pc) == JSOP_ENDITER);
                  cx->clearPendingException();
                  ok = !!js_CloseIterator(cx, &cx->regs->sp[-1].toObject());
                  cx->regs->sp -= 1;
                  if (!ok)
                      goto top;
                  cx->setPendingException(v);
                }
            }
        }
    }

    return NULL;
}

/*
 * Clean up a frame and return.
 */
static void
InlineReturn(VMFrame &f)
{
    JSContext *cx = f.cx;
    JSStackFrame *fp = f.regs.fp;

    JS_ASSERT(f.fp() != f.entryfp);

    JS_ASSERT(!js_IsActiveWithOrBlock(cx, &fp->scopeChain(), 0));

    Value *newsp = fp->actualArgs() - 1;
    newsp[-1] = fp->returnValue();
    cx->stack().popInlineFrame(cx, fp->prev(), newsp);
}

void JS_FASTCALL
stubs::SlowCall(VMFrame &f, uint32 argc)
{
    Value *vp = f.regs.sp - (argc + 2);

    if (!Invoke(f.cx, InvokeArgsAlreadyOnTheStack(vp, argc), 0))
        THROW();
}

void JS_FASTCALL
stubs::SlowNew(VMFrame &f, uint32 argc)
{
    JSContext *cx = f.cx;
    Value *vp = f.regs.sp - (argc + 2);

    if (!InvokeConstructor(cx, InvokeArgsAlreadyOnTheStack(vp, argc)))
        THROW();
}

/*
 * This function must only be called after the early prologue, since it depends
 * on fp->exec.fun.
 */
static inline void
RemovePartialFrame(JSContext *cx, JSStackFrame *fp)
{
    JSStackFrame *prev = fp->prev();
    Value *newsp = (Value *)fp;
    cx->stack().popInlineFrame(cx, prev, newsp);
}

/*
 * HitStackQuota is called after the early prologue pushing the new frame would
 * overflow f.stackLimit.
 */
void JS_FASTCALL
stubs::HitStackQuota(VMFrame &f)
{
    /* Include space to push another frame. */
    uintN nvals = f.fp()->script()->nslots + VALUES_PER_STACK_FRAME;
    JS_ASSERT(f.regs.sp == f.fp()->base());
    if (f.cx->stack().bumpCommitAndLimit(f.entryfp, f.regs.sp, nvals, &f.stackLimit))
        return;

    /* Remove the current partially-constructed frame before throwing. */
    RemovePartialFrame(f.cx, f.fp());
    js_ReportOverRecursed(f.cx);
    THROW();
}

/*
 * This function must only be called after the early prologue, since it depends
 * on fp->exec.fun.
 */
void * JS_FASTCALL
stubs::FixupArity(VMFrame &f, uint32 nactual)
{
    JSContext *cx = f.cx;
    JSStackFrame *oldfp = f.fp();

    JS_ASSERT(nactual != oldfp->numFormalArgs());

    /*
     * Grossssss! *move* the stack frame. If this ends up being perf-critical,
     * we can figure out how to spot-optimize it. Be careful to touch only the
     * members that have been initialized by initCallFrameCallerHalf and the
     * early prologue.
     */
    uint32 flags         = oldfp->isConstructingFlag();
    JSFunction *fun      = oldfp->fun();
    void *ncode          = oldfp->nativeReturnAddress();

    /* Pop the inline frame. */
    f.fp() = oldfp->prev();
    f.regs.sp = (Value*) oldfp;

    /* Reserve enough space for a callee frame. */
    JSStackFrame *newfp = cx->stack().getInlineFrameWithinLimit(cx, (Value*) oldfp, nactual,
                                                                fun, fun->script(), &flags,
                                                                f.entryfp, &f.stackLimit);
    if (!newfp) {
        /*
         * The PC is not coherent with the current frame, so fix it up for
         * exception handling.
         */
        f.regs.pc = f.jit()->nativeToPC(ncode);
        THROWV(NULL);
    }

    /* Reset the part of the stack frame set by the caller. */
    newfp->initCallFrameCallerHalf(cx, flags, ncode);

    /* Reset the part of the stack frame set by the prologue up to now. */
    newfp->initCallFrameEarlyPrologue(fun, nactual);

    /* The caller takes care of assigning fp to regs. */
    return newfp;
}

void * JS_FASTCALL
stubs::CompileFunction(VMFrame &f, uint32 nactual)
{
    /*
     * We have a partially constructed frame. That's not really good enough to
     * compile though because we could throw, so get a full, adjusted frame.
     */
    JSContext *cx = f.cx;
    JSStackFrame *fp = f.fp();

    /*
     * Since we can only use members set by initCallFrameCallerHalf,
     * we must carefully extract the callee from the nactual.
     */
    JSObject &callee = fp->formalArgsEnd()[-(int(nactual) + 2)].toObject();
    JSFunction *fun = callee.getFunctionPrivate();
    JSScript *script = fun->script();

    /*
     * FixupArity/RemovePartialFrame expect to be called after the early
     * prologue.
     */
    fp->initCallFrameEarlyPrologue(fun, nactual);

    if (nactual != fp->numFormalArgs()) {
        fp = (JSStackFrame *)FixupArity(f, nactual);
        if (!fp)
            return NULL;
    }

    /* Finish frame initialization. */
    fp->initCallFrameLatePrologue();

    /* These would have been initialized by the prologue. */
    f.regs.fp = fp;
    f.regs.sp = fp->base();
    f.regs.pc = script->code;

    if (fun->isHeavyweight() && !js_GetCallObject(cx, fp))
        THROWV(NULL);

    CompileStatus status = CanMethodJIT(cx, script, fp, CompileRequest_JIT);
    if (status == Compile_Okay)
        return script->getJIT(fp->isConstructing())->invokeEntry;

    /* Function did not compile... interpret it. */
    JSBool ok = Interpret(cx, fp);
    InlineReturn(f);

    if (!ok)
        THROWV(NULL);

    return NULL;
}

static inline bool
UncachedInlineCall(VMFrame &f, uint32 flags, void **pret, bool *unjittable, uint32 argc)
{
    JSContext *cx = f.cx;
    Value *vp = f.regs.sp - (argc + 2);
    JSObject &callee = vp->toObject();
    JSFunction *newfun = callee.getFunctionPrivate();
    JSScript *newscript = newfun->script();

    /* Get pointer to new frame/slots, prepare arguments. */
    StackSpace &stack = cx->stack();
    JSStackFrame *newfp = stack.getInlineFrameWithinLimit(cx, f.regs.sp, argc,
                                                          newfun, newscript, &flags,
                                                          f.entryfp, &f.stackLimit);
    if (JS_UNLIKELY(!newfp))
        return false;

    /* Initialize frame, locals. */
    newfp->initCallFrame(cx, callee, newfun, argc, flags);
    SetValueRangeToUndefined(newfp->slots(), newscript->nfixed);

    /* Officially push the frame. */
    stack.pushInlineFrame(cx, newscript, newfp, &f.regs);
    JS_ASSERT(newfp == f.regs.fp);

    /* Try to compile if not already compiled. */
    if (newscript->getJITStatus(newfp->isConstructing()) == JITScript_None) {
        CompileStatus status = CanMethodJIT(cx, newscript, newfp, CompileRequest_Interpreter);
        if (status == Compile_Error) {
            /* A runtime exception was thrown, get out. */
            InlineReturn(f);
            return false;
        }
        if (status == Compile_Abort)
            *unjittable = true;
    }

    /* Create call object now that we can't fail entering callee. */
    if (newfun->isHeavyweight() && !js_GetCallObject(cx, newfp))
        return false;

    /* If newscript was successfully compiled, run it. */
    if (JITScript *jit = newscript->getJIT(newfp->isConstructing())) {
        *pret = jit->invokeEntry;
        return true;
    }

    /* Otherwise, run newscript in the interpreter. */
    bool ok = !!Interpret(cx, cx->fp());
    InlineReturn(f);

    *pret = NULL;
    return ok;
}

void * JS_FASTCALL
stubs::UncachedNew(VMFrame &f, uint32 argc)
{
    UncachedCallResult ucr;
    UncachedNewHelper(f, argc, &ucr);
    return ucr.codeAddr;
}

void
stubs::UncachedNewHelper(VMFrame &f, uint32 argc, UncachedCallResult *ucr)
{
    ucr->init();

    JSContext *cx = f.cx;
    Value *vp = f.regs.sp - (argc + 2);

    /* Try to do a fast inline call before the general Invoke path. */
    if (IsFunctionObject(*vp, &ucr->fun) && ucr->fun->isInterpreted()) {
        ucr->callee = &vp->toObject();
        if (!UncachedInlineCall(f, JSFRAME_CONSTRUCTING, &ucr->codeAddr, &ucr->unjittable, argc))
            THROW();
    } else {
        if (!InvokeConstructor(cx, InvokeArgsAlreadyOnTheStack(vp, argc)))
            THROW();
    }
}

void * JS_FASTCALL
stubs::UncachedCall(VMFrame &f, uint32 argc)
{
    UncachedCallResult ucr;
    UncachedCallHelper(f, argc, &ucr);
    return ucr.codeAddr;
}

void JS_FASTCALL
stubs::Eval(VMFrame &f, uint32 argc)
{
    Value *vp = f.regs.sp - (argc + 2);

    JSObject *callee;
    JSFunction *fun;

    if (!IsFunctionObject(*vp, &callee) ||
        !IsBuiltinEvalFunction((fun = callee->getFunctionPrivate())))
    {
        if (!Invoke(f.cx, InvokeArgsAlreadyOnTheStack(vp, argc), 0))
            THROW();
        return;
    }

    JS_ASSERT(f.regs.fp == f.cx->fp());
    if (!DirectEval(f.cx, fun, argc, vp))
        THROW();
}

void
stubs::UncachedCallHelper(VMFrame &f, uint32 argc, UncachedCallResult *ucr)
{
    ucr->init();

    JSContext *cx = f.cx;
    Value *vp = f.regs.sp - (argc + 2);

    if (IsFunctionObject(*vp, &ucr->callee)) {
        ucr->callee = &vp->toObject();
        ucr->fun = GET_FUNCTION_PRIVATE(cx, ucr->callee);

        if (ucr->fun->isInterpreted()) {
            if (!UncachedInlineCall(f, 0, &ucr->codeAddr, &ucr->unjittable, argc))
                THROW();
            return;
        }

        if (ucr->fun->isNative()) {
            if (!CallJSNative(cx, ucr->fun->u.n.native, argc, vp))
                THROW();
            return;
        }
    }

    if (!Invoke(f.cx, InvokeArgsAlreadyOnTheStack(vp, argc), 0))
        THROW();

    return;
}

void JS_FASTCALL
stubs::PutStrictEvalCallObject(VMFrame &f)
{
    JS_ASSERT(f.fp()->isEvalFrame());
    JS_ASSERT(f.fp()->script()->strictModeCode);
    JS_ASSERT(f.fp()->hasCallObj());
    js_PutCallObject(f.cx, f.fp());
}

void JS_FASTCALL
stubs::PutActivationObjects(VMFrame &f)
{
    JS_ASSERT(f.fp()->hasCallObj() || f.fp()->hasArgsObj());
    js::PutActivationObjects(f.cx, f.fp());
}

extern "C" void *
js_InternalThrow(VMFrame &f)
{
    JSContext *cx = f.cx;

    // It's possible that from within RunTracer(), Interpret() returned with
    // an error and finished the frame (i.e., called ScriptEpilogue), but has
    // not yet performed an inline return.
    //
    // In this case, RunTracer() has no choice but to propagate the error
    // up to the method JIT, and thus to this function. But ScriptEpilogue()
    // has already been called. Detect this, and avoid double-finishing the
    // frame. See HandleErrorInExcessFrame() and bug 624100.
    if (f.fp()->finishedInInterpreter()) {
        // If it's the last frame, just propagate the failure up again.
        if (f.fp() == f.entryfp)
            return NULL;

        InlineReturn(f);
    }

    // Make sure sp is up to date.
    JS_ASSERT(cx->regs == &f.regs);

    // Call the throw hook if necessary
    JSThrowHook handler = f.cx->debugHooks->throwHook;
    if (handler) {
        Value rval;
        switch (handler(cx, cx->fp()->script(), cx->regs->pc, Jsvalify(&rval),
                        cx->debugHooks->throwHookData)) {
          case JSTRAP_ERROR:
            cx->clearPendingException();
            return NULL;

          case JSTRAP_RETURN:
            cx->clearPendingException();
            cx->fp()->setReturnValue(rval);
            return JS_FUNC_TO_DATA_PTR(void *,
                   cx->jaegerCompartment()->forceReturnTrampoline());

          case JSTRAP_THROW:
            cx->setPendingException(rval);
            break;

          default:
            break;
        }
    }

    jsbytecode *pc = NULL;
    for (;;) {
        pc = FindExceptionHandler(cx);
        if (pc)
            break;

        // The JIT guarantees that ScriptEpilogue() has always been run
        // upon exiting to its caller. This is important for consistency,
        // where execution modes make similar guarantees about prologues
        // and epilogues. RunTracer(), Interpret(), and Invoke() all
        // rely on this property.
        JS_ASSERT(!f.fp()->finishedInInterpreter());
        js_UnwindScope(cx, 0, cx->isExceptionPending());
        ScriptEpilogue(f.cx, f.fp(), false);

        // Don't remove the last frame, this is the responsibility of
        // JaegerShot()'s caller. We only guarantee that ScriptEpilogue()
        // has been run.
        if (f.entryfp == f.fp())
            break;

        JS_ASSERT(f.regs.sp == cx->regs->sp);
        InlineReturn(f);
    }

    JS_ASSERT(f.regs.sp == cx->regs->sp);

    if (!pc)
        return NULL;

    JSStackFrame *fp = cx->fp();
    JSScript *script = fp->script();
    return script->nativeCodeForPC(fp->isConstructing(), pc);
}

void JS_FASTCALL
stubs::GetCallObject(VMFrame &f)
{
    JS_ASSERT(f.fp()->fun()->isHeavyweight());
    if (!js_GetCallObject(f.cx, f.fp()))
        THROW();
}

void JS_FASTCALL
stubs::CreateThis(VMFrame &f, JSObject *proto)
{
    JSContext *cx = f.cx;
    JSStackFrame *fp = f.fp();
    JSObject *callee = &fp->callee();
    JSObject *obj = js_CreateThisForFunctionWithProto(cx, callee, proto);
    if (!obj)
        THROW();
    fp->formalArgs()[-1].setObject(*obj);
}

void JS_FASTCALL
stubs::EnterScript(VMFrame &f)
{
    JSStackFrame *fp = f.fp();
    JSContext *cx = f.cx;

    if (fp->script()->debugMode) {
        if (fp->isExecuteFrame()) {
            JSInterpreterHook hook = cx->debugHooks->executeHook;
            if (JS_UNLIKELY(hook != NULL))
                fp->setHookData(hook(cx, fp, JS_TRUE, 0, cx->debugHooks->executeHookData));
        } else {
            JSInterpreterHook hook = cx->debugHooks->callHook;
            if (JS_UNLIKELY(hook != NULL))
                fp->setHookData(hook(cx, fp, JS_TRUE, 0, cx->debugHooks->callHookData));
        }
    }

    Probes::enterJSFun(cx, fp->maybeFun(), fp->script());
}

void JS_FASTCALL
stubs::LeaveScript(VMFrame &f)
{
    JSStackFrame *fp = f.fp();
    JSContext *cx = f.cx;
    Probes::exitJSFun(cx, fp->maybeFun(), fp->maybeScript());

    if (fp->script()->debugMode) {
        void *hookData;
        JSInterpreterHook hook =
            fp->isExecuteFrame() ? cx->debugHooks->executeHook : cx->debugHooks->callHook;

        if (JS_UNLIKELY(hook != NULL) && (hookData = fp->maybeHookData())) {
            JSBool ok = JS_TRUE;
            hook(cx, fp, JS_FALSE, &ok, hookData);
            if (!ok)
                THROW();
        }
    }
}

#ifdef JS_TRACER

/*
 * Called when an error is in progress and the topmost frame could not handle
 * it. This will unwind to a given frame, or find and align to an exception
 * handler in the process.
 */
static inline bool
HandleErrorInExcessFrame(VMFrame &f, JSStackFrame *stopFp, bool searchedTopmostFrame = true)
{
    JSContext *cx = f.cx;

    /*
     * Callers of this called either Interpret() or JaegerShot(), which would
     * have searched for exception handlers already. If we see stopFp, just
     * return false. Otherwise, pop the frame, since it's guaranteed useless.
     *
     * Note that this also guarantees ScriptEpilogue() has been called.
     */
    JSStackFrame *fp = cx->fp();
    if (searchedTopmostFrame) {
        /*
         * This is a special case meaning that fp->finishedInInterpreter() is
         * true. If so, and fp == stopFp, our only choice is to propagate this
         * error up, back to the method JIT, and then to js_InternalThrow,
         * where this becomes a special case. See the comment there and bug
         * 624100.
         */
        if (fp == stopFp)
            return false;

        /*
         * Otherwise, the protocol here (like Invoke) is to assume that the
         * execution mode finished the frame, and to just pop it.
         */
        InlineReturn(f);
    }

    /* Remove the bottom frame. */
    bool returnOK = false;
    for (;;) {
        fp = cx->fp();

        /* Clear imacros. */
        if (fp->hasImacropc()) {
            cx->regs->pc = fp->imacropc();
            fp->clearImacropc();
        }
        JS_ASSERT(!fp->hasImacropc());

        /* If there's an exception and a handler, set the pc and leave. */
        if (cx->isExceptionPending()) {
            jsbytecode *pc = FindExceptionHandler(cx);
            if (pc) {
                cx->regs->pc = pc;
                returnOK = true;
                break;
            }
        }

        /* Don't unwind if this was the entry frame. */
        if (fp == stopFp)
            break;

        /* Unwind and return. */
        returnOK &= bool(js_UnwindScope(cx, 0, returnOK || cx->isExceptionPending()));
        returnOK = ScriptEpilogue(cx, fp, returnOK);
        InlineReturn(f);
    }

    JS_ASSERT(&f.regs == cx->regs);
    JS_ASSERT_IF(!returnOK, cx->fp() == stopFp);

    return returnOK;
}

/* Returns whether the current PC has method JIT'd code. */
static inline void *
AtSafePoint(JSContext *cx)
{
    JSStackFrame *fp = cx->fp();
    if (fp->hasImacropc())
        return false;

    JSScript *script = fp->script();
    return script->maybeNativeCodeForPC(fp->isConstructing(), cx->regs->pc);
}

/*
 * Interprets until either a safe point is reached that has method JIT'd
 * code, or the current frame tries to return.
 */
static inline JSBool
PartialInterpret(VMFrame &f)
{
    JSContext *cx = f.cx;
    JSStackFrame *fp = cx->fp();

#ifdef DEBUG
    JSScript *script = fp->script();
    JS_ASSERT(!fp->finishedInInterpreter());
    JS_ASSERT(fp->hasImacropc() ||
              !script->maybeNativeCodeForPC(fp->isConstructing(), cx->regs->pc));
#endif

    JSBool ok = JS_TRUE;
    ok = Interpret(cx, fp, 0, JSINTERP_SAFEPOINT);

    return ok;
}

JS_STATIC_ASSERT(JSOP_NOP == 0);

/*
 * Returns whether the current PC would return, or if the frame has already
 * been completed. This distinction avoids re-entering the interpreter or JIT
 * to complete a JSOP_RETURN. Instead, that edge case is handled in
 * HandleFinishedFrame. We could consider reducing complexity, and making this
 * function return only "finishedInInterpreter", and always using the full VM
 * machinery to fully finish frames.
 */
static inline bool
FrameIsFinished(JSContext *cx)
{
    JSOp op = JSOp(*cx->regs->pc);
    return (op == JSOP_RETURN ||
            op == JSOP_RETRVAL ||
            op == JSOP_STOP)
        ? true
        : cx->fp()->finishedInInterpreter();
}


/* Simulate an inline_return by advancing the pc. */
static inline void
AdvanceReturnPC(JSContext *cx)
{
    JS_ASSERT(*cx->regs->pc == JSOP_CALL ||
              *cx->regs->pc == JSOP_NEW ||
              *cx->regs->pc == JSOP_EVAL ||
              *cx->regs->pc == JSOP_FUNCALL ||
              *cx->regs->pc == JSOP_FUNAPPLY);
    cx->regs->pc += JSOP_CALL_LENGTH;
}


/*
 * Given a frame that is about to return, make sure its return value and
 * activation objects are fixed up. Then, pop the frame and advance the
 * current PC. Note that while we could enter the JIT at this point, the
 * logic would still be necessary for the interpreter, so it's easier
 * (and faster) to finish frames in C++ even if at a safe point here.
 */
static bool
HandleFinishedFrame(VMFrame &f, JSStackFrame *entryFrame)
{
    JSContext *cx = f.cx;

    JS_ASSERT(FrameIsFinished(cx));

    /*
     * This is the most difficult and complicated piece of the tracer
     * integration, and historically has been very buggy. The problem is that
     * although this frame has to be popped (see RemoveExcessFrames), it may
     * be at a JSOP_RETURN opcode, and it might not have ever been executed.
     * That is, fp->rval may not be set to the top of the stack, and if it
     * has, the stack has already been decremented. Note that fp->rval is not
     * the only problem: the epilogue may never have been executed.
     *
     * Here are the edge cases and whether the frame has been exited cleanly:
     *  1. No: A trace exited directly before a RETURN op, and the
     *         interpreter never ran.
     *  2. Yes: The interpreter exited cleanly.
     *  3. No: The interpreter exited on a safe point. LEAVE_ON_SAFE_POINT
     *         is not used in between JSOP_RETURN and advancing the PC,
     *         therefore, it cannot have been run if at a safe point.
     *  4. No: Somewhere in the RunTracer call tree, we removed a frame,
     *         and we returned to a JSOP_RETURN opcode. Note carefully
     *         that in this situation, FrameIsFinished() returns true!
     *  5. Yes: The function exited in the method JIT, during
     *         FinishExcessFrames() However, in this case, we'll never enter
     *         HandleFinishedFrame(): we always immediately pop JIT'd frames.
     *
     * Since the only scenario where this fixup is NOT needed is a normal exit
     * from the interpreter, we can cleanly check for this scenario by checking
     * a bit it sets in the frame.
     */
    bool returnOK = true;
    if (!cx->fp()->finishedInInterpreter()) {
        if (JSOp(*cx->regs->pc) == JSOP_RETURN)
            cx->fp()->setReturnValue(f.regs.sp[-1]);

        returnOK = ScriptEpilogue(cx, cx->fp(), true);
    }

    JS_ASSERT_IF(cx->fp()->isFunctionFrame() &&
                 !cx->fp()->isEvalFrame(),
                 !cx->fp()->hasCallObj());

    if (cx->fp() != entryFrame) {
        InlineReturn(f);
        AdvanceReturnPC(cx);
    }

    return returnOK;
}

/*
 * Given a frame newer than the entry frame, try to finish it. If it's at a
 * return position, pop the frame. If it's at a safe point, execute it in
 * Jaeger code. Otherwise, try to interpret until a safe point.
 *
 * While this function is guaranteed to make progress, it may not actually
 * finish or pop the current frame. It can either:
 *   1) Finalize a finished frame, or
 *   2) Finish and finalize the frame in the Method JIT, or
 *   3) Interpret, which can:
 *     a) Propagate an error, or
 *     b) Finish the frame, but not finalize it, or
 *     c) Abruptly leave at any point in the frame, or in a newer frame
 *        pushed by a call, that has method JIT'd code.
 */
static bool
EvaluateExcessFrame(VMFrame &f, JSStackFrame *entryFrame)
{
    JSContext *cx = f.cx;
    JSStackFrame *fp = cx->fp();

    /*
     * A "finished" frame is when the interpreter rested on a STOP,
     * RETURN, RETRVAL, etc. We check for finished frames BEFORE looking
     * for a safe point. If the frame was finished, we could have already
     * called ScriptEpilogue(), and entering the JIT could call it twice.
     */
    if (!fp->hasImacropc() && FrameIsFinished(cx))
        return HandleFinishedFrame(f, entryFrame);

    if (void *ncode = AtSafePoint(cx)) {
        if (!JaegerShotAtSafePoint(cx, ncode))
            return false;
        InlineReturn(f);
        AdvanceReturnPC(cx);
        return true;
    }

    return PartialInterpret(f);
}

/*
 * Evaluate frames newer than the entry frame until all are gone. This will
 * always leave f.regs.fp == entryFrame.
 */
static bool
FinishExcessFrames(VMFrame &f, JSStackFrame *entryFrame)
{
    JSContext *cx = f.cx;

    while (cx->fp() != entryFrame || entryFrame->hasImacropc()) {
        if (!EvaluateExcessFrame(f, entryFrame)) {
            if (!HandleErrorInExcessFrame(f, entryFrame))
                return false;
        }
    }

    return true;
}

#if defined JS_MONOIC
static void
UpdateTraceHintSingle(Repatcher &repatcher, JSC::CodeLocationJump jump, JSC::CodeLocationLabel target)
{
    /*
     * Hack: The value that will be patched is before the executable address,
     * so to get protection right, just unprotect the general region around
     * the jump.
     */
    repatcher.relink(jump, target);

    JaegerSpew(JSpew_PICs, "relinking trace hint %p to %p\n",
               jump.executableAddress(), target.executableAddress());
}

static void
DisableTraceHint(JITScript *jit, ic::TraceICInfo &ic)
{
    Repatcher repatcher(jit);
    UpdateTraceHintSingle(repatcher, ic.traceHint, ic.jumpTarget);

    if (ic.hasSlowTraceHint)
        UpdateTraceHintSingle(repatcher, ic.slowTraceHint, ic.jumpTarget);
}

static void
ResetTraceHintAt(JSScript *script, js::mjit::JITScript *jit,
                 jsbytecode *pc, uint16_t index, bool full)
{
    if (index >= jit->nTraceICs)
        return;
    ic::TraceICInfo &ic = jit->traceICs()[index];
    if (!ic.initialized)
        return;
    
    JS_ASSERT(ic.jumpTargetPC == pc);

    JaegerSpew(JSpew_PICs, "Enabling trace IC %u in script %p\n", index, script);

    Repatcher repatcher(jit);

    UpdateTraceHintSingle(repatcher, ic.traceHint, ic.stubEntry);

    if (ic.hasSlowTraceHint)
        UpdateTraceHintSingle(repatcher, ic.slowTraceHint, ic.stubEntry);

    if (full) {
        ic.traceData = NULL;
        ic.loopCounterStart = 1;
        ic.loopCounter = ic.loopCounterStart;
    }
}
#endif

void
js::mjit::ResetTraceHint(JSScript *script, jsbytecode *pc, uint16_t index, bool full)
{
#if JS_MONOIC
    if (script->jitNormal)
        ResetTraceHintAt(script, script->jitNormal, pc, index, full);

    if (script->jitCtor)
        ResetTraceHintAt(script, script->jitCtor, pc, index, full);
#endif
}

#if JS_MONOIC
void *
RunTracer(VMFrame &f, ic::TraceICInfo &ic)
#else
void *
RunTracer(VMFrame &f)
#endif
{
    JSContext *cx = f.cx;
    JSStackFrame *entryFrame = f.fp();
    TracePointAction tpa;

    /* :TODO: nuke PIC? */
    if (!cx->traceJitEnabled)
        return NULL;

    /*
     * Force initialization of the entry frame's scope chain and return value,
     * if necessary.  The tracer can query the scope chain without needing to
     * check the HAS_SCOPECHAIN flag, and the frame is guaranteed to have the
     * correct return value stored if we trace/interpret through to the end
     * of the frame.
     */
    entryFrame->scopeChain();
    entryFrame->returnValue();

    bool blacklist;
    uintN inlineCallCount = 0;
    void **traceData;
    uintN *traceEpoch;
    uint32 *loopCounter;
    uint32 hits;
#if JS_MONOIC
    traceData = &ic.traceData;
    traceEpoch = &ic.traceEpoch;
    loopCounter = &ic.loopCounter;
    *loopCounter = 1;
    hits = ic.loopCounterStart;
#else
    traceData = NULL;
    traceEpoch = NULL;
    loopCounter = NULL;
    hits = 1;
#endif
    tpa = MonitorTracePoint(f.cx, inlineCallCount, &blacklist, traceData, traceEpoch,
                            loopCounter, hits);
    JS_ASSERT(!TRACE_RECORDER(cx));

#if JS_MONOIC
    ic.loopCounterStart = *loopCounter;
    if (blacklist)
        DisableTraceHint(entryFrame->jit(), ic);
#endif

    // Even though ExecuteTree() bypasses the interpreter, it should propagate
    // error failures correctly.
    JS_ASSERT_IF(cx->isExceptionPending(), tpa == TPA_Error);

	f.fp() = cx->fp();
    JS_ASSERT(f.fp() == cx->fp());
    switch (tpa) {
      case TPA_Nothing:
        return NULL;

      case TPA_Error:
        if (!HandleErrorInExcessFrame(f, entryFrame, f.fp()->finishedInInterpreter()))
            THROWV(NULL);
        JS_ASSERT(!cx->fp()->hasImacropc());
        break;

      case TPA_RanStuff:
      case TPA_Recorded:
        break;
    }

    /*
     * The tracer could have dropped us off on any frame at any position.
     * Well, it could not have removed frames (recursion is disabled).
     *
     * Frames after the entryFrame cannot be entered via JaegerShotAtSafePoint()
     * unless each is at a safe point. We can JaegerShotAtSafePoint these
     * frames individually, but we must unwind to the entryFrame.
     *
     * Note carefully that JaegerShotAtSafePoint can resume methods at
     * arbitrary safe points whereas JaegerShot cannot.
     *
     * If we land on entryFrame without a safe point in sight, we'll end up
     * at the RETURN op. This is an edge case with two paths:
     *
     * 1) The entryFrame is the last inline frame. If it fell on a RETURN,
     *    move the return value down.
     * 2) The entryFrame is NOT the last inline frame. Pop the frame.
     *
     * In both cases, we hijack the stub to return to InjectJaegerReturn. This
     * moves |oldFp->rval| into the scripted return registers.
     */

  restart:
    /* Step 1. Finish frames created after the entry frame. */
    if (!FinishExcessFrames(f, entryFrame))
        THROWV(NULL);

    /* IMacros are guaranteed to have been removed by now. */
    JS_ASSERT(f.fp() == entryFrame);
    JS_ASSERT(!entryFrame->hasImacropc());

    /* Step 2. If entryFrame is done, use a special path to return to EnterMethodJIT(). */
    if (FrameIsFinished(cx)) {
        if (!HandleFinishedFrame(f, entryFrame))
            THROWV(NULL);

        void *retPtr = JS_FUNC_TO_DATA_PTR(void *, InjectJaegerReturn);
        *f.returnAddressLocation() = retPtr;
        return NULL;
    }

    /* Step 3. If entryFrame is at a safe point, just leave. */
    if (void *ncode = AtSafePoint(cx))
        return ncode;

    /* Step 4. Do a partial interp, then restart the whole process. */
    if (!PartialInterpret(f)) {
        if (!HandleErrorInExcessFrame(f, entryFrame))
            THROWV(NULL);
    }

    goto restart;
}

#endif /* JS_TRACER */

#if defined JS_TRACER
# if defined JS_MONOIC
void *JS_FASTCALL
stubs::InvokeTracer(VMFrame &f, ic::TraceICInfo *ic)
{
    return RunTracer(f, *ic);
}

# else

void *JS_FASTCALL
stubs::InvokeTracer(VMFrame &f)
{
    return RunTracer(f);
}
# endif /* JS_MONOIC */
#endif /* JS_TRACER */

