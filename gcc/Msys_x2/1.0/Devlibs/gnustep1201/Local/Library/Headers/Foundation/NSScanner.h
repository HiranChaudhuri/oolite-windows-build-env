/* Definitions for NSScanner class
   Copyright (C) 1996,1999 Free Software Foundation, Inc.

   Author:  Eric Norum <eric@skatter.usask.ca>
   Created: 1996
   
   This file is part of the GNUstep Objective-C Library.

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

#ifndef __NSScanner_h_GNUSTEP_BASE_INCLUDE
#define __NSScanner_h_GNUSTEP_BASE_INCLUDE
#import	<GNUstepBase/GSVersionMacros.h>

#import	<Foundation/NSDecimal.h>
#import	<Foundation/NSDictionary.h>
#import	<Foundation/NSCharacterSet.h>

#if	defined(__cplusplus)
extern "C" {
#endif

/*
 * NSScanner class
 */
@interface NSScanner : NSObject <NSCopying>
{
#if	GS_EXPOSE(NSScanner)
@private
  NSString		*_string;
  NSCharacterSet	*_charactersToBeSkipped;
  BOOL			(*_skipImp)(NSCharacterSet*, SEL, unichar);
  NSDictionary		*_locale;
  NSUInteger		_scanLocation;
  unichar		_decimal;
  BOOL			_caseSensitive;
  BOOL			_isUnicode;
#endif
#if	!GS_NONFRAGILE
  void			*_unused;
#endif
}

/*
 * Creating an NSScanner
 */
+ (id) localizedScannerWithString: (NSString*)aString;
+ (id) scannerWithString: (NSString*)aString;
- (id) initWithString: (NSString*)aString;

/*
 * Getting an NSScanner's string
 */
- (NSString*) string;

/*
 * Configuring an NSScanner
 */
- (NSUInteger) scanLocation;
- (void) setScanLocation: (NSUInteger)anIndex;

- (BOOL) caseSensitive;
- (void) setCaseSensitive: (BOOL)flag;

- (NSCharacterSet*) charactersToBeSkipped;
- (void) setCharactersToBeSkipped: (NSCharacterSet *)aSet;

- (NSDictionary*)locale;
- (void)setLocale:(NSDictionary*)localeDictionary;

/*
 * Scanning a string
 */
- (BOOL) scanInt: (int*)value;
- (BOOL) scanHexInt: (unsigned int*)value;
- (BOOL) scanLongLong: (long long*)value;
- (BOOL) scanFloat: (float*)value;
- (BOOL) scanDouble: (double*)value;
- (BOOL) scanString: (NSString*)string intoString: (NSString**)value;
- (BOOL) scanCharactersFromSet: (NSCharacterSet*)aSet
		    intoString: (NSString**)value;
- (BOOL) scanUpToString: (NSString*)string intoString: (NSString**)value;
- (BOOL) scanUpToCharactersFromSet: (NSCharacterSet*)aSet 
			intoString: (NSString**)value;
- (BOOL) isAtEnd;

#if OS_API_VERSION(GS_API_NONE, GS_API_NONE)
- (BOOL) scanRadixUnsignedInt: (unsigned int*)value;
#endif
#if OS_API_VERSION(GS_API_MACOSX, GS_API_LATEST)
- (BOOL) scanDecimal: (NSDecimal*)value;
#endif
#if OS_API_VERSION(100500, GS_API_LATEST)
/** Not implemented */
- (BOOL) scanHexDouble: (double *)result;
/** Not implemented */
- (BOOL) scanHexFloat: (float *)result;
/** Not implemented */
- (BOOL) scanHexLongLong: (unsigned long long *)result;
/** Not implemented */
- (BOOL) scanInteger: (NSInteger *)value;
#endif
@end

#if	defined(__cplusplus)
}
#endif

#endif /* __NSScanner_h_GNUSTEP_BASE_INCLUDE */
