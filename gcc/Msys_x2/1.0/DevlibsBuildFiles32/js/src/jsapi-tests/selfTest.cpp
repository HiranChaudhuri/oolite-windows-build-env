/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 4 -*-
 * vim: set ts=8 sw=4 et tw=99:
 */

#include "tests.h"

BEGIN_TEST(selfTest_NaNsAreSame)
{
    jsvalRoot v1(cx), v2(cx);
    EVAL("0/0", v1.addr());  // NaN
    CHECK_SAME(v1, v1);

    EVAL("Math.sin('no')", v2.addr());  // also NaN
    CHECK_SAME(v1, v2);
    return true;
}
END_TEST(selfTest_NaNsAreSame)

BEGIN_TEST(selfTest_globalHasNoParent)
{
    CHECK(JS_GetParent(cx, global) == NULL);
    return true;
}
END_TEST(selfTest_globalHasNoParent)
