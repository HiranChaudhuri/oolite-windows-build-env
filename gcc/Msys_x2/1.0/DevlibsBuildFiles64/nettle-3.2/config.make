# Makefile settings shared between Makefiles.

CC = gcc
CXX = g++
CFLAGS = -I/mingw/../devlibs/include -ggdb3 -Wno-pointer-sign -Wall -W -O2 -mtune=generic  -Wmissing-prototypes -Wmissing-declarations -Wstrict-prototypes   -Wpointer-arith -Wbad-function-cast -Wnested-externs
CXXFLAGS = -g -O2
CCPIC = 
CPPFLAGS = 
DEFS = -DHAVE_CONFIG_H
LDFLAGS = -L/mingw/../devlibs/lib
LIBS = -lgmp 
LIBOBJS = 
EMULATOR = 
NM = nm

OBJEXT = o
EXEEXT = .exe

CC_FOR_BUILD = gcc -O
EXEEXT_FOR_BUILD = .exe

DEP_FLAGS = -MT $@ -MD -MP -MF $@.d
DEP_PROCESS = true

PACKAGE_BUGREPORT = nettle-bugs@lists.lysator.liu.se
PACKAGE_NAME = nettle
PACKAGE_STRING = nettle 3.2
PACKAGE_TARNAME = nettle
PACKAGE_VERSION = 3.2

LIBNETTLE_MAJOR = 6
LIBNETTLE_MINOR = 2
LIBNETTLE_SONAME = 
LIBNETTLE_FILE = libnettle.dll.a
LIBNETTLE_FILE_SRC = $(LIBNETTLE_FILE)
LIBNETTLE_FORLINK = libnettle-$(LIBNETTLE_MAJOR)-$(LIBNETTLE_MINOR).dll
LIBNETTLE_LIBS = -Wl,--no-whole-archive $(LIBS)
LIBNETTLE_LINK = $(CC) $(CFLAGS) $(LDFLAGS) -shared -Wl,--out-implib=$(LIBNETTLE_FILE) -Wl,--export-all-symbols -Wl,--enable-auto-import -Wl,--whole-archive

LIBHOGWEED_MAJOR = 4
LIBHOGWEED_MINOR = 2
LIBHOGWEED_SONAME = 
LIBHOGWEED_FILE = libhogweed.dll.a
LIBHOGWEED_FILE_SRC = $(LIBHOGWEED_FILE)
LIBHOGWEED_FORLINK = libhogweed-$(LIBHOGWEED_MAJOR)-$(LIBHOGWEED_MINOR).dll
LIBHOGWEED_LIBS = -Wl,--no-whole-archive $(LIBS) libnettle.dll.a
LIBHOGWEED_LINK = $(CC) $(CFLAGS) $(LDFLAGS) -shared -Wl,--out-implib=$(LIBHOGWEED_FILE) -Wl,--export-all-symbols -Wl,--enable-auto-import -Wl,--whole-archive

GMP_NUMB_BITS = 64

AR = ar
ARFLAGS = cru
AUTOCONF = autoconf
AUTOHEADER = autoheader
M4 = /bin/m4
MAKEINFO = makeinfo
RANLIB = ranlib
LN_S = ln -s

prefix	=	/mingw/../devlibs
exec_prefix =	${prefix}
datarootdir =	${prefix}/share
bindir =	${exec_prefix}/bin
libdir =	${exec_prefix}/lib
includedir =	${prefix}/include
infodir =	${datarootdir}/info

# PRE_CPPFLAGS and PRE_LDFLAGS lets each Makefile.in prepend its own
# flags before CPPFLAGS and LDFLAGS. While EXTRA_CFLAGS are added at the end.

COMPILE = $(CC) $(PRE_CPPFLAGS) $(CPPFLAGS) $(DEFS) $(CFLAGS) $(EXTRA_CFLAGS) $(DEP_FLAGS)
COMPILE_CXX = $(CXX) $(PRE_CPPFLAGS) $(CPPFLAGS) $(DEFS) $(CXXFLAGS) $(DEP_FLAGS)
LINK = $(CC) $(CFLAGS) $(PRE_LDFLAGS) $(LDFLAGS)
LINK_CXX = $(CXX) $(CXXFLAGS) $(PRE_LDFLAGS) $(LDFLAGS)

# Default rule. Must be here, since config.make is included before the
# usual targets.
default: all

# For some reason the suffixes list must be set before the rules.
# Otherwise BSD make won't build binaries e.g. aesdata. On the other
# hand, AIX make has the opposite idiosyncrasies to BSD, and the AIX
# compile was broken when .SUFFIXES was moved here from Makefile.in.

.SUFFIXES:
.SUFFIXES: .asm .c .$(OBJEXT) .p$(OBJEXT) .html .dvi .info .exe .pdf .ps .texinfo

# Disable builtin rule
%$(EXEEXT) : %.c
.c:

# Keep object files
.PRECIOUS: %.o

.PHONY: all check install uninstall clean distclean mostlyclean maintainer-clean distdir \
	all-here check-here install-here clean-here distclean-here mostlyclean-here \
	maintainer-clean-here distdir-here \
	install-shared install-info install-headers \
	uninstall-shared uninstall-info uninstall-headers \
	dist distcleancheck
