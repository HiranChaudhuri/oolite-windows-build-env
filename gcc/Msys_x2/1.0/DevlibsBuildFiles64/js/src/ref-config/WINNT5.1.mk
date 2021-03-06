# -*- Mode: makefile -*-
#
# ***** BEGIN LICENSE BLOCK *****
# Version: MPL 1.1/GPL 2.0/LGPL 2.1
#
# The contents of this file are subject to the Mozilla Public License Version
# 1.1 (the "License"); you may not use this file except in compliance with
# the License. You may obtain a copy of the License at
# http://www.mozilla.org/MPL/
#
# Software distributed under the License is distributed on an "AS IS" basis,
# WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License
# for the specific language governing rights and limitations under the
# License.
#
# The Original Code is Mozilla Communicator client code, released
# March 31, 1998.
#
# The Initial Developer of the Original Code is
# Netscape Communications Corporation.
# Portions created by the Initial Developer are Copyright (C) 1998
# the Initial Developer. All Rights Reserved.
#
# Contributor(s):
#
# Alternatively, the contents of this file may be used under the terms of
# either the GNU General Public License Version 2 or later (the "GPL"), or
# the GNU Lesser General Public License Version 2.1 or later (the "LGPL"),
# in which case the provisions of the GPL or the LGPL are applicable instead
# of those above. If you wish to allow use of your version of this file only
# under the terms of either the GPL or the LGPL, and not to allow others to
# use your version of this file under the terms of the MPL, indicate your
# decision by deleting the provisions above and replace them with the notice
# and other provisions required by the GPL or the LGPL. If you do not delete
# the provisions above, a recipient may use your version of this file under
# the terms of any one of the MPL, the GPL or the LGPL.
#
# ***** END LICENSE BLOCK *****

#
# Config for Windows NT using MS Visual C++ (version?)
#

CC = gcc
CXX = g++

RANLIB = echo

PDBFILE = $(basename $(@F)).pdb

#.c.o:
#      $(CC) -c -MD $*.d $(CFLAGS) $<

CPU_ARCH = x64 # XXX fixme
GFX_ARCH = win64
NANOJIT_ARCH = X64

# MSVC compiler options for both debug/optimize
# -nologo  - suppress copyright message
# -W3      - Warning level 3
# -Gm      - enable minimal rebuild
# -Z7      - put debug info into the executable, not in .pdb file
# -Zi      - put debug info into .pdb file
# -YX      - automatic precompiled headers
# -GX      - enable C++ exception support
#WIN_CFLAGS = -nologo -W3
WIN_CFLAGS =

# MSVC compiler options for debug builds linked to MSVCRTD.DLL
# -MDd     - link with MSVCRTD.LIB (Dynamically-linked, multi-threaded, debug C-runtime)
# -Od      - minimal optimization
#WIN_IDG_CFLAGS = -MDd -Od -Z7
WIN_IDG_CFLAGS =

# MSVC compiler options for debug builds linked to MSVCRT.DLL
# -MD      - link with MSVCRT.LIB (Dynamically-linked, multi-threaded, debug C-runtime)
# -Od      - minimal optimization
#WIN_DEBUG_CFLAGS = -MD -Od -Zi -Fd$(OBJDIR)/$(PDBFILE)
WIN_DEBUG_CFLAGS =

# MSVC compiler options for release (optimized) builds
# -MD      - link with MSVCRT.LIB (Dynamically-linked, multi-threaded, C-runtime)
# -O2      - Optimize for speed
# -G5      - Optimize for Pentium
WIN_OPT_CFLAGS = -O2 -Wno-invalid-offsetof 

ifdef BUILD_OPT
OPTIMIZER = $(WIN_OPT_CFLAGS)
else
ifdef BUILD_IDG
OPTIMIZER = $(WIN_IDG_CFLAGS)
else
OPTIMIZER = $(WIN_DEBUG_CFLAGS)
endif
endif

OS_CFLAGS = -DXP_WIN -DEXPORT_JS_API -DJS_C_STRINGS_ARE_UTF8 -D__STDC_LIMIT_MACROS -D_WINDOWS -DWINVER=0x500 -D_WIN32_WINNT=0x500 $(WIN_CFLAGS)
JSDLL_CFLAGS = -DEXPORT_JS_API
OS_LIBS = -lm -lc

#PREBUILT_CPUCFG = 1
#USE_MSVC = 1

LIB_LINK_FLAGS= -lkernel32 -luser32 -lgdi32 -lwinmm -lwinspool -lcomdlg32\
 -ladvapi32 -lshell32 -lole32 -loleaut32 -luuid \
 -mwindows -shared

EXE_LINK_FLAGS= -lkernel32 -luser32 -lgdi32 -lwinspool -lcomdlg32\
 -ladvapi32 -lwinmm -lshell32 -lole32 -loleaut32 -luuid

# CAFEDIR = t:/cafe
# JCLASSPATH = $(CAFEDIR)/Java/Lib/classes.zip
# JAVAC = $(CAFEDIR)/Bin/sj.exe
# JAVAH = $(CAFEDIR)/Java/Bin/javah.exe
# JCFLAGS = -I$(CAFEDIR)/Java/Include -I$(CAFEDIR)/Java/Include/win32
