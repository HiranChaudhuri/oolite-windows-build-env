/** Interface to ObjC runtime for GNUStep
   Copyright (C) 1995, 1997, 2000 Free Software Foundation, Inc.

   Written by:  Andrew Kachites McCallum <mccallum@gnu.ai.mit.edu>
   Date: 1995
   
   This file is part of the GNUstep Base Library.

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.
   
   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.
   
   You should have received a copy of the GNU Lesser General Public
   License along with this library; if not, write to the Free
   Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02111 USA.

    AutogsdocSource: NSObjCRuntime.m
    AutogsdocSource: NSLog.m

   */ 

#ifndef __NSObjCRuntime_h_GNUSTEP_BASE_INCLUDE
#define __NSObjCRuntime_h_GNUSTEP_BASE_INCLUDE

#include <stdarg.h>
#include <limits.h>

#import	<GNUstepBase/GSVersionMacros.h>
#import	<GNUstepBase/GSConfig.h>

/* These typedefs must be in place before GSObjCRuntime.h is imported.
 */

#if     !defined(NSINTEGER_DEFINED)
typedef	intptr_t	NSInteger;
typedef	uintptr_t	NSUInteger;
#endif /* !defined(NSINTEGER_DEFINED) */

#if     !defined(CGFLOAT_DEFINED)
#if     GS_SIZEOF_VOIDP == 8
#define CGFLOAT_IS_DBL  1
typedef double          CGFloat;
#else
typedef float           CGFloat;
#endif
#endif /* !defined(CGFLOAT_DEFINED) */

#define NSINTEGER_DEFINED 1
#define CGFLOAT_DEFINED 1

#if	defined(__cplusplus)
extern "C" {
#endif

enum 
{
  /**
   * Specifies that the enumeration is concurrency-safe.  Note that this does
   * not mean that it will be carried out in a concurrent manner, only that
   * it can be.
   */
  NSEnumerationConcurrent = (1UL << 0),
  /**
   * Specifies that the enumeration should happen in the opposite of the
   * natural order of the collection.
   */
  NSEnumerationReverse = (1UL << 1)
};

/** Bitfield used to specify options to control enumeration over collections.
 */
typedef NSUInteger NSEnumerationOptions;

#import <GNUstepBase/GSObjCRuntime.h>

#if OS_API_VERSION(100500,GS_API_LATEST) 
GS_EXPORT NSString	*NSStringFromProtocol(Protocol *aProtocol);
GS_EXPORT Protocol	*NSProtocolFromString(NSString *aProtocolName);
#endif
GS_EXPORT SEL		NSSelectorFromString(NSString *aSelectorName);
GS_EXPORT NSString	*NSStringFromSelector(SEL aSelector);
GS_EXPORT SEL		NSSelectorFromString(NSString *aSelectorName);
GS_EXPORT Class		NSClassFromString(NSString *aClassName);
GS_EXPORT NSString	*NSStringFromClass(Class aClass);
GS_EXPORT const char	*NSGetSizeAndAlignment(const char *typePtr,
  NSUInteger *sizep, NSUInteger *alignp);

#if OS_API_VERSION(GS_API_NONE, GS_API_NONE)
/* Logging */
/**
 *  OpenStep spec states that log messages go to stderr, but just in case
 *  someone wants them to go somewhere else, they can implement a function
 *  like this and assign a pointer to it to _NSLog_printf_handler.
 */
typedef void NSLog_printf_handler (NSString* message);
GS_EXPORT NSLog_printf_handler	*_NSLog_printf_handler;
GS_EXPORT int	_NSLogDescriptor;
@class NSRecursiveLock;
GS_EXPORT NSRecursiveLock	*GSLogLock(void);
#endif

GS_EXPORT void			NSLog (NSString *format, ...);
GS_EXPORT void			NSLogv (NSString *format, va_list args);

#ifndef YES
#define YES		1
#endif
#ifndef NO
#define NO		0
#endif
#ifndef nil
#define nil		0
#endif

#if	defined(__cplusplus)
}
#endif

#endif /* __NSObjCRuntime_h_GNUSTEP_BASE_INCLUDE */
