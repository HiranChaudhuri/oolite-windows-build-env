/** Implementation of host class
   Copyright (C) 1996, 1997,1999 Free Software Foundation, Inc.

   Written by: Luke Howard <lukeh@xedoc.com.au>
   Date: 1996
   Rewrite by: Richard Frith-Macdonald <richard@brainstorm.co.uk>
   Date: 1999

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

   <title>NSHost class reference</title>
   $Date: 2010-05-29 10:09:53 +0100 (Sat, 29 May 2010) $ $Revision: 30473 $
  */

#import "common.h"
#define	EXPOSE_NSHost_IVARS	1
#import "Foundation/NSLock.h"
#import "Foundation/NSHost.h"
#import "Foundation/NSArray.h"
#import "Foundation/NSDictionary.h"
#import "Foundation/NSEnumerator.h"
#import "Foundation/NSSet.h"
#import "Foundation/NSCoder.h"

#if defined(__MINGW__)
#include <winsock2.h>
#else
#include <netdb.h>
#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif
#include <sys/param.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#endif /* !__MINGW__*/

#ifndef	INADDR_NONE
#define	INADDR_NONE	-1
#endif

static NSString			*localHostName = @"GNUstep local host";
static Class			hostClass;
static NSLock			*_hostCacheLock = nil;
static BOOL			_hostCacheEnabled = YES;
static NSMutableDictionary	*_hostCache = nil;


@interface NSHost (Private)
- (void) _addName: (NSString*)name;
+ (struct hostent*) _entryForAddress: (NSString*)address;
- (id) _initWithHostEntry: (struct hostent*)entry key: (NSString*)key;
+ (NSMutableSet*) _localAddresses;
@end

@implementation NSHost (Private)

- (void) _addName: (NSString*)name
{
  NSMutableSet	*s = [_names mutableCopy];

  name = [name copy];
  [s addObject: name];
  ASSIGNCOPY(_names, s);
  RELEASE(s);
  if (_hostCacheEnabled == YES)
    {
      [_hostCache setObject: self forKey: name];
    }
  RELEASE(name);
}

+ (struct hostent*) _entryForAddress: (NSString*)address
{
  struct hostent	*h = 0;
  struct in_addr	hostaddr;

#ifndef	HAVE_INET_ATON
  hostaddr.s_addr = inet_addr([address UTF8String]);
  if (hostaddr.s_addr == INADDR_NONE)
    {
      NSLog(@"Attempt to lookup host entry for bad IP address (%@)", address);
    }
#else
  if (inet_aton([address UTF8String], (struct in_addr*)&hostaddr.s_addr) == 0)
    {
      NSLog(@"Attempt to lookup host entry for bad IP address (%@)", address);
    }
#endif
  else
    {
      h = gethostbyaddr((char*)&hostaddr, sizeof(hostaddr), AF_INET);
      if (h == 0)
	{
	  NSDebugLog(@"Host '%@' not found using 'gethostbyaddr()' - perhaps "
	    @"the address is wrong, networking is not set up on your machine,"
	    @" or the requested address lacks a reverse-dns entry.", address);
	}
    }
  return h;
}

- (id) _initWithAddress: (NSString*)name
{
  if ((self = [super init]) == nil)
    {
      return nil;
    }
  name = [name copy];
  _names = [[NSSet alloc] initWithObjects: &name count: 1];
  _addresses = RETAIN(_names);
  if (_hostCacheEnabled == YES)
    {
      [_hostCache setObject: self forKey: name];
    }
  RELEASE(name);
  return self;
}

- (id) _initWithHostEntry: (struct hostent*)entry key: (NSString*)name
{
  int			i;
  char			*ptr;
  struct in_addr	in;
  NSString		*h_name;
  NSMutableSet		*names;
  NSMutableSet		*addresses;
  NSMutableSet		*extra;

  if ((self = [super init]) == nil)
    {
      return nil;
    }
  if ([name isEqualToString: localHostName] == NO
    && entry == (struct hostent*)NULL)
    {
      NSLog(@"Host '%@' init failed - perhaps the name/address is wrong or "
	@"networking is not set up on your machine", name);
      DESTROY(self);
      return nil;
    }
  else if (name == nil && entry != (struct hostent*)NULL)
    {
      NSLog(@"Nil hostname supplied but network database entry is not empty");
      DESTROY(self);
      return nil;
    }

  names = [NSMutableSet new];
  addresses = [NSMutableSet new];

  if ([name isEqualToString: localHostName] == YES)
    {
      extra = [hostClass _localAddresses];
    }
  else
    {
      extra = nil;
    }

  for (;;)
    {
      /*
       * We remove all the IP addresses that we have added to the host so
       * far from the set of extra addresses available on the current host.
       * Then we try to find a new network database entry for one of the
       * remaining extra addresses, and loop round to add all the names
       * and addresses for that entry.
       */
      [extra minusSet: addresses];
      while (entry == 0 && [extra count] > 0)
	{
	  NSString	*a = [extra anyObject];

	  entry = [hostClass _entryForAddress: a];
	  if (entry == 0)
	    {
	      /*
	       * Can't find a database entry for this IP address, but since
	       * we know the address is valid, we add it to the list of
	       * addresses for this host anyway.
	       */
	      [addresses addObject: a];
	      [extra removeObject: a];
	    }
	}
      if (entry == 0)
	{
	  break;
	}

      h_name = [NSString stringWithUTF8String: entry->h_name];
      [names addObject: h_name];

      if (entry->h_aliases != 0)
	{
	  i = 0;
	  while ((ptr = entry->h_aliases[i++]) != 0)
	    {
	      [names addObject: [NSString stringWithUTF8String: ptr]];
	    }
	}
      if (entry->h_addr_list != 0)
	{
	  i = 0;
	  while ((ptr = entry->h_addr_list[i++]) != 0)
	    {
	      NSString	*addr;

	      memcpy((void*)&in.s_addr, (const void*)ptr, entry->h_length);
	      addr = [NSString stringWithUTF8String: (char*)inet_ntoa(in)];
	      [addresses addObject: addr];
	    }
	}
      entry = 0;
    }

  _names = [names copy];
  RELEASE(names);
  _addresses = [addresses copy];
  RELEASE(addresses);

  if (_hostCacheEnabled == YES)
    {
      [_hostCache setObject: self forKey: name];
    }

  return self;
}

+ (NSMutableSet*) _localAddresses
{
  NSMutableSet	*set;

  set = [[self currentHost]->_addresses mutableCopy];
  [set addObject: @"127.0.0.1"];
  return AUTORELEASE(set);
}
@end

@implementation NSHost

/*
 *	Max hostname length in line with RFC  1123
 */
#define	GSMAXHOSTNAMELEN	255

/**
 * Return the current host name ... may change if we are using dhcp etc
 */
static NSString*
myHostName()
{
  static NSString	*name = nil;
  static char		old[GSMAXHOSTNAMELEN+1];
  char			buf[GSMAXHOSTNAMELEN+1];
  int			res;

  [_hostCacheLock lock];
  res = gethostname(buf, GSMAXHOSTNAMELEN);
  if (res < 0 || *buf == '\0')
    {
      NSLog(@"Unable to get name of current host - using 'localhost'");
      ASSIGN(name, @"localhost");
    }
  else if (name == nil || strcmp(old, buf) != 0)
    {
      strcpy(old, buf);
      RELEASE(name);
      name = [[NSString alloc] initWithCString: buf];
    }
  [_hostCacheLock unlock];
  return name;
}

+ (void) initialize
{
  if (self == [NSHost class])
    {
      hostClass = self;
      _hostCacheLock = [[NSRecursiveLock alloc] init];
      _hostCache = [NSMutableDictionary new];
    }
}

+ (NSHost*) currentHost
{
  return [self hostWithName: myHostName()];
}

+ (NSHost*) hostWithName: (NSString*)name
{
  BOOL		tryByAddress = NO;
  NSHost	*host = nil;

  if (name == nil)
    {
      NSLog(@"Nil host name sent to [NSHost +hostWithName:]");
      return nil;
    }
  if ([name isEqual: @""] == YES)
    {
      NSLog(@"Empty host name sent to [NSHost +hostWithName:]");
      return nil;
    }

  [_hostCacheLock lock];
  if (_hostCacheEnabled == YES)
    {
      host = [_hostCache objectForKey: name];
    }
  if (host == nil)
    {
      if ([name isEqualToString: localHostName] == YES)
	{
	  /*
	   * Special GNUstep extension host - we try to have a host entry
	   * with ALL the IP addresses of any interfaces on the local machine
	   */
	  host = [[self alloc] _initWithHostEntry: 0 key: localHostName];
	  IF_NO_GC([host autorelease];)
	}
      else
	{
	  struct hostent	*h = 0;
	  const char		*n = [name UTF8String];

	  h = gethostbyname((char*)n);
	  if (h == 0 && sscanf(n, "%*d.%*d.%*d.%*d") == 4)
	    {
	      tryByAddress = YES;
	    }
	  else if (h == 0)
	    {
	      if ([name isEqualToString: myHostName()] == YES)
		{
		  NSLog(@"No network address appears to be available "
		    @"for this machine (%@) - using loopback address "
		    @"(127.0.0.1)", name);
		  NSLog(@"You probably need a line like '"
		    @"127.0.0.1 %@ localhost' in your /etc/hosts file", name);
		  host = [self hostWithAddress: @"127.0.0.1"];
		  [host _addName: name];
		}
	      else
		{
		  NSLog(@"Host '%@' not found using 'gethostbyname()' - "
		    @"perhaps the hostname is wrong or networking is not "
		    @"set up on your machine", name);
		}
	    }
	  else
	    {
	      host = [[self alloc] _initWithHostEntry: h key: name];
	      IF_NO_GC([host autorelease];)
	    }
	}
    }
  else
    {
      IF_NO_GC([[host retain] autorelease];)
    }
  [_hostCacheLock unlock];
  if (tryByAddress == YES)
    {
      return [self hostWithAddress: name];
    }
  return host;
}

+ (NSHost*) hostWithAddress: (NSString*)address
{
  NSHost	*host = nil;

  if (address == nil)
    {
      NSLog(@"Nil host address sent to [NSHost +hostWithAddress:]");
      return nil;
    }
  if ([address isEqual: @""] == YES)
    {
      NSLog(@"Empty host address sent to [NSHost +hostWithAddress:]");
      return nil;
    }

  [_hostCacheLock lock];
  if (_hostCacheEnabled == YES)
    {
      host = [_hostCache objectForKey: address];
    }
  if (host == nil)
    {
      struct hostent	*h;

      h = [self _entryForAddress: address];
      if (h == 0)
	{
	  struct in_addr	hostaddr;
	  BOOL			badAddr = NO;

#ifndef	HAVE_INET_ATON
	  hostaddr.s_addr = inet_addr([address UTF8String]);
	  if (hostaddr.s_addr == INADDR_NONE)
	    {
	      badAddr = YES;
	    }
#else
	  if (inet_aton([address UTF8String],
	    (struct in_addr*)&hostaddr.s_addr) == 0)
	    {
	      badAddr = YES;
	    }
#endif
	  if (badAddr == NO)
	    {
	      host = [[self alloc] _initWithAddress: address];
	      IF_NO_GC([host autorelease];)
	    }
	}
      else
	{
	  host = [[self alloc] _initWithHostEntry: h key: address];
	  IF_NO_GC([host autorelease];)
	}
    }
  else
    {
      IF_NO_GC([[host retain] autorelease];)
    }
  [_hostCacheLock unlock];
  return host;
}

+ (void) setHostCacheEnabled: (BOOL)flag
{
  [_hostCacheLock lock];
  _hostCacheEnabled = flag;
  [_hostCacheLock unlock];
}

+ (BOOL) isHostCacheEnabled
{
  return _hostCacheEnabled;
}

+ (void) flushHostCache
{
  [_hostCacheLock lock];
  [_hostCache removeAllObjects];
  [_hostCacheLock unlock];
}

/* Methods for encoding/decoding*/

- (id) replacementObjectForPortCoder: (NSPortCoder*)aCoder
{
  return self;
}

- (void) encodeWithCoder: (NSCoder*)aCoder
{
  NSString	*address = [self address];

  if ([address isEqual: @"127.0.0.1"] == YES)
    {
      NSEnumerator	*e = [_addresses objectEnumerator];

      while ((address = [e nextObject]) != nil)
	{
	  if ([address isEqual: @"127.0.0.1"] == NO)
	    {
	      break;
	    }
	}
    }
  [aCoder encodeObject: address];
}

- (id) initWithCoder: (NSCoder*)aCoder
{
  NSString	*address;
  NSHost	*host;

  address = [aCoder decodeObject];
  if (address != nil)
    {
      host = [NSHost hostWithAddress: address];
    }
  else
    {
      host = [NSHost currentHost];
    }
  IF_NO_GC([host retain];)
  DESTROY(self);
  return host;
}

- (void) dealloc
{
  RELEASE(_names);
  RELEASE(_addresses);
  [super dealloc];
}

- (id) init
{
  [self dealloc];
  return nil;
}

/*
 *	The OpenStep spec says that [-hash] must be the same for any two
 *	objects that [-isEqual: ] returns YES for.  We have a problem in
 *	that [-isEqualToHost: ] is specified to return YES if any name or
 *	address part of two hosts is the same.  That means we can't
 *	reasonably calculate a hash since two hosts with radically
 *	different ivar contents may be 'equal'.  The best I can think of
 *	is for all hosts to hash to the same value - which makes it very
 *	inefficient to store them in a set, dictionary, map or hash table.
 */
- (unsigned) hash
{
  return 1;
}

- (BOOL) isEqual: (id)other
{
  if (other == self)
    {
      return YES;
    }
  if ([other isKindOfClass: [NSHost class]])
    {
      return [self isEqualToHost: (NSHost*)other];
    }
  return NO;
}

- (BOOL) isEqualToHost: (NSHost*)aHost
{
  NSEnumerator	*e;
  NSString	*a;

  if (aHost == self)
    {
      return YES;
    }
  e = [aHost->_addresses objectEnumerator];
  while ((a = [e nextObject]) != nil)
    {
      if ([_addresses member: a] != nil)
	{
	  return YES;
	}
    }
  return NO;
}

- (NSString*) name
{
  return [_names anyObject];
}

- (NSArray*) names
{
  return [_names allObjects];
}

- (NSString*) address
{
  return [_addresses anyObject];
}

- (NSArray*) addresses
{
  return [_addresses allObjects];
}

- (NSString*) description
{
  return [NSString stringWithFormat: @"Host %@ (%@ %@)",
    [self name], _names, _addresses];
}

@end

@implementation	NSHost (GNUstep)
+ (NSHost*) localHost
{
  return [self hostWithName: localHostName];
}
@end

