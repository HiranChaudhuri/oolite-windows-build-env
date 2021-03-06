/** Implementation of NSCopyObject() for GNUStep
   Copyright (C) 1994, 1995 Free Software Foundation, Inc.

   Written by:  Andrew Kachites McCallum <mccallum@gnu.ai.mit.edu>
   Date: August 1994

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

   <title>NSCopyObject class reference</title>
   $Date: 2010-05-29 10:09:53 +0100 (Sat, 29 May 2010) $ $Revision: 30473 $
   */

#import "common.h"

NSObject *NSCopyObject(NSObject *anObject, NSUInteger extraBytes, NSZone *zone)
{
  id copy = NSAllocateObject(((id)anObject)->class_pointer, extraBytes, zone);
  memcpy(copy, anObject,
    class_getInstanceSize(object_getClass(anObject)) + extraBytes);
  return copy;
}
