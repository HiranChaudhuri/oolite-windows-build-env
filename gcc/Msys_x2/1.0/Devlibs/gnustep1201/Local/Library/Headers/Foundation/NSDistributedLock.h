/* Interface for NSDistributedLock for GNUStep
   Copyright (C) 1997 Free Software Foundation, Inc.

   Written by:  Richard Frith-Macdonald <richard@brainstorm.co.uk>
   Date: 1997

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
   */

#ifndef __NSDistributedLock_h_GNUSTEP_BASE_INCLUDE
#define __NSDistributedLock_h_GNUSTEP_BASE_INCLUDE
#import	<GNUstepBase/GSVersionMacros.h>

#import	<Foundation/NSObject.h>
#import	<Foundation/NSString.h>
#import	<Foundation/NSDate.h>

#if	defined(__cplusplus)
extern "C" {
#endif

@interface NSDistributedLock : NSObject
{
#if	GS_EXPOSE(NSDistributedLock)
  NSString	*_lockPath;
  NSDate	*_lockTime;
#endif
#if	!GS_NONFRAGILE
  void		*_unused;
#endif
}

+ (NSDistributedLock*) lockWithPath: (NSString*)aPath;
- (id) initWithPath: (NSString*)aPath;

- (void) breakLock;
- (NSDate*) lockDate;
- (BOOL) tryLock;
- (void) unlock;

@end

#if	defined(__cplusplus)
}
#endif

#endif /* __NSDistributedLock_h_GNUSTEP_BASE_INCLUDE */
