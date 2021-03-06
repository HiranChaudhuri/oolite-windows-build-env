#
#   config-noarch.make.in
#
#   The settings required by the makefile package that are determined
#   by configure but that are independent of the platform that we
#   are working on (keep in mind we can support multiple platforms
#   being used at the same time!).
#
#   In practice, this file should containg global gnustep-make options
#   (like GNUSTEP_IS_FLATTENED or GNUSTEP_SYSTEM_ROOT), as opposed to
#   config info needed to build or compile or do things on a certain
#   platform (like CC or OPTFLAG), which should go into config.make
#
#   Copyright (C) 1997-2006 Free Software Foundation, Inc.
#
#   Author:  Scott Christley <scottc@net-community.com>
#   Author:  Ovidiu Predescu <ovidiu@net-community.com>
#   Author:  Nicola Pero <n.pero@mi.flashnet.it>
#
#   This file is part of the GNUstep Makefile Package.
#
#   This library is free software; you can redistribute it and/or
#   modify it under the terms of the GNU General Public License
#   as published by the Free Software Foundation; either version 3
#   of the License, or (at your option) any later version.
#   
#   You should have received a copy of the GNU General Public
#   License along with this library; see the file COPYING.
#   If not, write to the Free Software Foundation,
#   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.

#
# The GNUstep Make Package Version
#
GNUSTEP_MAKE_MAJOR_VERSION=2
GNUSTEP_MAKE_MINOR_VERSION=4
GNUSTEP_MAKE_SUBMINOR_VERSION=0
GNUSTEP_MAKE_VERSION=2.4.0

# Enable or disable strict gnustep-make v2 mode.  In strict
# gnustep-make v2 mode, we actively try to be backwards-incompatible
# with gnustep-make v1.  This dangerous option is useful in
# test/development builds when you want to make sure your software has
# been properly updated to gnustep-make v2.
#
# If this option is set to 'yes', we enable strict gnustep-make v2
# mode.  Else, it's ignored.
#
GNUSTEP_MAKE_STRICT_V2_MODE=no

# The default library combination
default_library_combo = gnu-gnu-gnu

#
# Location of GNUstep's config file for this installation
#
# Warning - the base library's configure.in will extract the GNUstep
# config file location from the following line using grep/sed - so if
# you change the following lines you *need* to update the base library
# configure.in too.
#
# PS: At run-time, this can be overridden on the command-line, or
# via an environment variable.
ifeq ($(GNUSTEP_CONFIG_FILE),)
GNUSTEP_CONFIG_FILE = /etc/GNUstep/GNUstep.conf
endif

#
# Now we set up the environment and everything by reading the GNUstep
# configuration file(s).
#

# These are the defaults value ... they will be used only if they are
# not set in the config files (or on the command-line or in
# environment).
ifeq ($(GNUSTEP_MAKE_STRICT_V2_MODE),yes)
# FIXME: These should abort if the variables are ever evaluated, but
# it needs testing.
#  GNUSTEP_SYSTEM_ROOT = $(error GNUSTEP_SYSTEM_ROOT is obsolete)
#  GNUSTEP_LOCAL_ROOT = $(error GNUSTEP_LOCAL_ROOT is obsolete)
#  GNUSTEP_NETWORK_ROOT = $(error GNUSTEP_NETWORK_ROOT is obsolete)
#  GNUSTEP_USER_DIR = $(error GNUSTEP_USER_DIR is obsolete)
else
  GNUSTEP_SYSTEM_ROOT = /mingw/../devlibs/gnustep1201/System
  GNUSTEP_LOCAL_ROOT = /mingw/../devlibs/gnustep1201/Local
  GNUSTEP_NETWORK_ROOT = /mingw/../devlibs/gnustep1201/Network
  GNUSTEP_USER_DIR = GNUstep
endif

# This includes the GNUstep configuration file, but only if it exists
-include $(GNUSTEP_CONFIG_FILE)

# FIXME: determining GNUSTEP_HOME
GNUSTEP_HOME = $(HOME)

# Read the user configuration file ... unless it is disabled (ie, set
# to an empty string)
ifneq ($(GNUSTEP_USER_CONFIG_FILE),)

 # FIXME - Checking for relative vs. absolute paths!
 ifneq ($(filter /%, $(GNUSTEP_USER_CONFIG_FILE)),)
  # Path starts with '/', consider it absolute
  -include $(GNUSTEP_USER_CONFIG_FILE)
 else
  # Path does no start with '/', try it as relative
  -include $(GNUSTEP_HOME)/$(GNUSTEP_USER_CONFIG_FILE)
 endif 

endif

# GNUSTEP_FLATTENED is obsolete, please use GNUSTEP_IS_FLATTENED
# instead
ifeq ($(GNUSTEP_MAKE_STRICT_V2_MODE),yes)
# FIXME: Test that this is OK
#  GNUSTEP_FLATTENED = $(error GNUSTEP_FLATTENED is obsolete)
else
  GNUSTEP_FLATTENED = yes
endif
GNUSTEP_IS_FLATTENED = yes

#
# Set GNUSTEP_USER_ROOT from GNUSTEP_USER_DIR; GNUSTEP_USER_ROOT is
# the variable used in practice
#
ifneq ($(GNUSTEP_MAKE_STRICT_V2_MODE),yes)
  ifneq ($(filter /%, $(GNUSTEP_USER_DIR)),)
   # Path starts with '/', consider it absolute
   GNUSTEP_USER_ROOT = $(GNUSTEP_USER_DIR)
  else
   # Path does no start with '/', try it as relative
   GNUSTEP_USER_ROOT = $(GNUSTEP_HOME)/$(GNUSTEP_USER_DIR)
  endif 
else
# FIXME: Test that this is OK.  When I use it, it seems to
# abort even if GNUSTEP_USER_ROOT is never referenced!
#  GNUSTEP_USER_ROOT = $(error GNUSTEP_USER_ROOT is obsolete)
endif

# If multi-platform support is disabled, just use the hardcoded cpu,
# vendor and os determined when gnustep-make was configured.  The
# reason using the hardcoded ones might be better is that config.guess
# and similar scripts might even require compiling test files to
# determine the platform - which is horribly slow (that is done in
# names.make if GNUSTEP_HOST is not yet set at that stage).  To
# prevent this problem, unless we were configured to determine the
# platform at run time, by default we use the hardcoded values of
# GNUSTEP_HOST*.

ifeq ("","")
  GNUSTEP_HOST = i686-pc-mingw32
  GNUSTEP_HOST_CPU = ix86
  GNUSTEP_HOST_VENDOR = pc
  GNUSTEP_HOST_OS = mingw32
endif

# Enables of disables parallel building support.  When parallel
# building support is disabled, the target .NOTPARALLEL: is used in
# all make invocations of gnustep-make to make sure a traditional non
# parallel build is always performed.  When parallel building support
# is enabled, all make invocations are still .NOTPARALLEL except for
# an additional make invocation which is performed when compiling an
# executable or library; that invocation does not use .NOTPARALLEL and
# will compile all the files of the executable or library in parallel.
GNUSTEP_MAKE_PARALLEL_BUILDING = yes