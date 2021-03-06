#   This file must be sourced inside (ba)sh using: .
#
#   GNUstep.sh.  Generated from GNUstep.sh.in by configure.
#
#   Shell initialization for the GNUstep environment.
#
#   Copyright (C) 1997-2008 Free Software Foundation, Inc.
#
#   Author:  Scott Christley <scottc@net-community.com>
#   Author:  Adam Fedor <fedor@gnu.org>
#   Author:  Richard Frith-Macdonald <rfm@gnu.org>
#   Author:  Nicola Pero <nicola.pero@meta-innovation.com>
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

# Warning - this shell script is delicate, because it is sourced by
# using . rather than simply executed.  It is sourced because that is
# the only way to change the shell variables in the calling
# environment.
#
# Sourcing makes the shell script more delicate for the following reasons:
#
#  * temporary variables are automatically set in the calling
#  environment!  WORKAROUND: we need to unset all them after using
#  them to avoid polluting the calling environment;
#
#  * not only the exit value of the script, but the exit value of each
#  command we execute, might be interpreted by the calling
#  environment.  Typically, the calling environment might be using the
#  shell errexit option ('set -e') in bash parlance, which causes the
#  shell to exit if any command returns an error value.  If this were
#  a normal script, this option would mean that the shell would exit
#  if the return value of the whole script were an error value; but
#  because we are sourced, it is as all the commands in this script
#  were executed directly in the calling environment, so *all* values
#  returned by *all* commands must be non-error.  [this all typically
#  happens in rpm builds, where scripts are run with the errexit
#  option so that errors in scripts abort the build, and now if a
#  script sources GNUstep.sh, then we are exactly in this situation -
#  if any command inside GNUstep.sh returns an error value (even if
#  GNUstep.sh as a whole would be happy and return succes), the whole
#  rpm build process aborts!]. WORKAROUND: we must make sure all
#  commands return success - last resorts hacks like 'command || :'
#  which always returns success whatever command returns.
#

# If we're running in zsh (auch!) make sure we're using -y
# (SH_WORD_SPLIT) else our path manipulations won't work.
if [ -n "$ZSH_VERSION" ]; then

  # If -y is not already set, set it and remember that we
  # need to set it back to what it was at the end.
  if ( setopt | grep shwordsplit > /dev/null ); then :; else
    set -y
    GS_ZSH_NEED_TO_RESTORE_SET=yes
  fi

fi

# When this is set to 'yes', strict gnustep-make v2 compatibility mode
# is turned on.
GNUSTEP_MAKE_STRICT_V2_MODE=no

#
# Read our configuration files
#

# Determine the location of the system configuration file
if [ -z "$GNUSTEP_CONFIG_FILE" ]; then
  GNUSTEP_CONFIG_FILE=/etc/GNUstep/GNUstep.conf
else
  # Remember that the variable was already set in the environment, and
  # preserve it at the end of the script.  Otherwise we'll unset it to
  # avoid polluting the environment with our own internal variables.
  GNUSTEP_KEEP_CONFIG_FILE=yes
fi

# Determine the location of the user configuration file
if [ -z "$GNUSTEP_USER_CONFIG_FILE" ]; then
  GNUSTEP_USER_CONFIG_FILE=.GNUstep.conf
else
  GNUSTEP_KEEP_USER_CONFIG_FILE=yes
fi

# Read the system configuration file
if [ -f "$GNUSTEP_CONFIG_FILE" ]; then
  . "$GNUSTEP_CONFIG_FILE"
fi

# FIXME: determining GNUSTEP_HOME
GNUSTEP_HOME=~

# Read the user configuration file ... unless it is disabled (ie, set
# to an empty string)
if [ -n "$GNUSTEP_USER_CONFIG_FILE" ]; then
  case "$GNUSTEP_USER_CONFIG_FILE" in 
    /*) # An absolute path
        if [ -f "$GNUSTEP_USER_CONFIG_FILE" ]; then
          . "$GNUSTEP_USER_CONFIG_FILE"
        fi;;
     *) # Something else
        if [ -f "$GNUSTEP_HOME/$GNUSTEP_USER_CONFIG_FILE" ]; then
          . "$GNUSTEP_HOME/$GNUSTEP_USER_CONFIG_FILE"
        fi;;
  esac
fi

# Now, set any essential variable (that is not already set) to the
# built-in values.

if [ "$GNUSTEP_MAKE_STRICT_V2_MODE" = "yes" ]; then
  # In strict v2 mode, clean these obsolete variables in case the
  # config file contains them.  They shouldn't exist so unsetting
  # them can't harm.
  unset GNUSTEP_SYSTEM_ROOT
  unset GNUSTEP_LOCAL_ROOT
  unset GNUSTEP_NETWORK_ROOT
else
  # This is deprecated and will be removed
  if [ -z "$GNUSTEP_SYSTEM_ROOT" ]; then
    GNUSTEP_SYSTEM_ROOT=/mingw/../devlibs/gnustep1201/System
  fi

  # This is deprecated and will be removed
  if [ -z "$GNUSTEP_LOCAL_ROOT" ]; then
    GNUSTEP_LOCAL_ROOT=/mingw/../devlibs/gnustep1201/Local
  fi

  # This is deprecated and will be removed
  if [ -z "$GNUSTEP_NETWORK_ROOT" ]; then
    GNUSTEP_NETWORK_ROOT=/mingw/../devlibs/gnustep1201/Network
  fi

  export GNUSTEP_SYSTEM_ROOT GNUSTEP_LOCAL_ROOT GNUSTEP_NETWORK_ROOT
fi

# GNUSTEP_FLATTENED is obsolete, please use GNUSTEP_IS_FLATTENED
# instead
if [ "$GNUSTEP_MAKE_STRICT_V2_MODE" = "yes" ]; then
  unset GNUSTEP_FLATTENED
else
  GNUSTEP_FLATTENED=yes
  export GNUSTEP_FLATTENED
fi
GNUSTEP_IS_FLATTENED=yes
if [ -z "$LIBRARY_COMBO" ]; then
  LIBRARY_COMBO=gnu-gnu-gnu
fi
if [ ! "$GNUSTEP_MAKE_STRICT_V2_MODE" = "yes" ]; then
  # Having these variables in the environment is deprecated and will
  # be removed.  But for now, if we are not in strict gnustep-make v2
  # mode, then we always export these variables for
  # backwards-compatibility.
  export GNUSTEP_IS_FLATTENED LIBRARY_COMBO

  # Else they are only exported if GNUSTEP_SH_EXPORT_ALL_VARIABLES
  # is defined - at the end of the script.
fi

if [ -z "$GNUSTEP_MAKEFILES" ]; then
  GNUSTEP_MAKEFILES=/mingw/../devlibs/gnustep1201/System/Library/Makefiles
fi
export GNUSTEP_MAKEFILES


if [ "$GNUSTEP_MAKE_STRICT_V2_MODE" = "yes" ]; then
  # Make sure this is never set in gnustep-make v2 strict mode; it
  # might have been set in the config file.
  unset GNUSTEP_USER_DIR
  unset GNUSTEP_USER_ROOT
else
  # GNUSTEP_USER_DIR is deprecated and will be removed
  if [ -z "$GNUSTEP_USER_DIR" ]; then
    GNUSTEP_USER_DIR=GNUstep
  fi

  #
  # Set GNUSTEP_USER_ROOT which is the variable used in practice.
  # GNUSTEP_USER_ROOT is deprecated and will be removed
  #
  case "$GNUSTEP_USER_DIR" in 
    /*) # An absolute path
        GNUSTEP_USER_ROOT="$GNUSTEP_USER_DIR";;
     *) # Something else
        GNUSTEP_USER_ROOT="$GNUSTEP_HOME/$GNUSTEP_USER_DIR";;
  esac

  # This variable was used to set up GNUSTEP_USER_ROOT which is the one
  # that is actually exported; we can now drop it from the environment.
  unset GNUSTEP_USER_DIR

  # This is deprecated and will be removed
  export GNUSTEP_USER_ROOT
fi

# If multi-platform support is disabled, just use the hardcoded cpu,
# vendor and os determined when gnustep-make was configured.  The
# reason using the hardcoded ones might be better is that config.guess
# and similar scripts might even require compiling test files to
# determine the platform - but then you can't source GNUstep.sh
# without having gcc, binutils, libc6-dev installed.  Which can be a
# problem for end-users who are not developers and have no development
# tools installed.  To prevent this problem, unless we were configured
# to determine the platform at run time, by default we use the
# hardcoded values of GNUSTEP_HOST*.
if [ -z "" ]; then
  GNUSTEP_HOST=i686-pc-mingw32
  GNUSTEP_HOST_CPU=ix86
  GNUSTEP_HOST_VENDOR=pc
  GNUSTEP_HOST_OS=mingw32
fi

#
# Determine the host information
#
if [ -z "$GNUSTEP_HOST" ]; then
  # Not all shells (e.g. /bin/sh on FreeBSD < 4.0 or ash) have pushd/popd
  tmpdir=`pwd`; cd /tmp
  GNUSTEP_HOST=`$GNUSTEP_MAKEFILES/config.guess`
  GNUSTEP_HOST=`$GNUSTEP_MAKEFILES/config.sub $GNUSTEP_HOST`
  cd "$tmpdir"
  unset tmpdir
fi

if [ -z "$GNUSTEP_HOST_CPU" ]; then
  GNUSTEP_HOST_CPU=`$GNUSTEP_MAKEFILES/cpu.sh $GNUSTEP_HOST`
  GNUSTEP_HOST_CPU=`$GNUSTEP_MAKEFILES/clean_cpu.sh $GNUSTEP_HOST_CPU`
fi

if [ -z "$GNUSTEP_HOST_VENDOR" ]; then
  GNUSTEP_HOST_VENDOR=`$GNUSTEP_MAKEFILES/vendor.sh $GNUSTEP_HOST`
  GNUSTEP_HOST_VENDOR=`$GNUSTEP_MAKEFILES/clean_vendor.sh $GNUSTEP_HOST_VENDOR`
fi

if [ -z "$GNUSTEP_HOST_OS" ]; then 
  GNUSTEP_HOST_OS=`$GNUSTEP_MAKEFILES/os.sh $GNUSTEP_HOST`
  GNUSTEP_HOST_OS=`$GNUSTEP_MAKEFILES/clean_os.sh $GNUSTEP_HOST_OS`
fi

if [ ! "$GNUSTEP_MAKE_STRICT_V2_MODE" = "yes" ]; then
  # These variables are deprecated for usage in shell scripts; you
  # need to use gnustep-config to get them in a shell script.
  export GNUSTEP_HOST GNUSTEP_HOST_CPU GNUSTEP_HOST_VENDOR GNUSTEP_HOST_OS

  # Even in strict gnustep-make v2 mode, we export them at the end
  # if explicitly requested to export all variables.
fi

# Now load in all the remaining paths
. $GNUSTEP_MAKEFILES/filesystem.sh

# No longer needed
unset GNUSTEP_HOME

# Determine if the paths look like Windows paths that need fixing
fixup_paths=no
# Here we want to use `...` but the only portable way to use it when
# there are "..." inside the expression (ie, it's actually
# `..."..."...`) seems to be to use it in variable assignments.  So we
# use a temporary variable and assign the result of `...` to it
path_check=`echo "$GNUSTEP_MAKEFILES" | sed 's|^[a-zA-Z]:/.*$||'`
if [ -z "$path_check" ]; then
  fixup_paths=yes
fi
unset path_check

if [ "$GNUSTEP_MAKE_STRICT_V2_MODE" = "yes" ]; then
  unset GNUSTEP_PATHLIST
else
  # GNUSTEP_PATHLIST is deprecated and will be removed.
  #
  # GNUSTEP_PATHLIST is like an abstract path-like shell variable, which
  # can be used to search the gnustep directories.
  #
  if [ -z "$GNUSTEP_PATHLIST" ]; then

   GNUSTEP_PATHLIST=`$GNUSTEP_MAKEFILES/print_unique_pathlist.sh "$GNUSTEP_USER_ROOT" "$GNUSTEP_LOCAL_ROOT" "$GNUSTEP_NETWORK_ROOT" "$GNUSTEP_SYSTEM_ROOT" $fixup_paths`
   export GNUSTEP_PATHLIST
  fi
fi

#
# Add path to Tools to PATH
#
# NB: functionally, print_unique_pathlist.sh is mostly used to do the
# fixup_paths thing, since duplicated paths will automatically be
# checked below when we add them to the PATH.  On the other hand,
# removing duplicates later can be expensive since we do a echo+grep
# for each duplicate.  When there are many duplicates it's faster to
# use print_unique_pathlist.sh first to remove them and skip the
# echos+greps later.
GNUSTEP_TOOLS_PATHLIST=`$GNUSTEP_MAKEFILES/print_unique_pathlist.sh "$GNUSTEP_USER_TOOLS" "$GNUSTEP_LOCAL_TOOLS" "$GNUSTEP_NETWORK_TOOLS" "$GNUSTEP_SYSTEM_TOOLS" $fixup_paths`

# Now, we check the paths in GNUSTEP_*_ADMIN_TOOLS.  These paths
# should only be used by Administrators -- normal users don't have
# enough powers to use those tools to do useful things.  Our test for
# being an 'Administrator' is that anyone who can write to an Admin
# directory can be considered powerful enough to use those tools.

# FIXME: Unfortunately, this doesn't work if the Admin directory
# is mounted read-only, so a better test is required!

# So we examine GNUSTEP_*_ADMIN_TOOLS; if we find any path in that
# list that exists and that we can write to, we add it to our PATH.
for dir in "$GNUSTEP_SYSTEM_ADMIN_TOOLS" "$GNUSTEP_NETWORK_ADMIN_TOOLS" "$GNUSTEP_LOCAL_ADMIN_TOOLS" "$GNUSTEP_USER_ADMIN_TOOLS"; do
  if [ -d "$dir"  -a  -w "$dir" ]; then
    # Only add the new dir if it's not already in GNUSTEP_TOOLS_PATHLIST
    if (echo ":${GNUSTEP_TOOLS_PATHLIST}:" \
      |grep -v ":${dir}:" >/dev/null); then
      GNUSTEP_TOOLS_PATHLIST="$dir:$GNUSTEP_TOOLS_PATHLIST"
    fi
  fi
done

#
# And now, we put the results into PATH
#
old_IFS="$IFS"
IFS=:
for dir in $GNUSTEP_TOOLS_PATHLIST; do

  # Prepare the path_fragment
  if [ "$GNUSTEP_IS_FLATTENED" = "no" ]; then
    path_fragment="$dir:$dir/${GNUSTEP_HOST_CPU}/${GNUSTEP_HOST_OS}/${LIBRARY_COMBO}:$dir/${GNUSTEP_HOST_CPU}/${GNUSTEP_HOST_OS}"
  else
    path_fragment="$dir"
  fi

  # Add it to PATH, but only if not already there (eg, typically /usr/bin is already there)
  if [ -z "$PATH" ]; then
    PATH="$path_fragment"
  else
    if ( echo ":${PATH}:"\
      |grep -v ":${path_fragment}:" >/dev/null ); then
      PATH="${path_fragment}:${PATH}"
    fi
  fi

done
IFS="$old_IFS"
unset old_IFS
unset dir
unset path_fragment
unset GNUSTEP_TOOLS_PATHLIST
export PATH

# Determine the library paths
GNUSTEP_LIBRARIES_PATHLIST=`$GNUSTEP_MAKEFILES/print_unique_pathlist.sh "$GNUSTEP_USER_LIBRARIES" "$GNUSTEP_LOCAL_LIBRARIES" "$GNUSTEP_NETWORK_LIBRARIES" "$GNUSTEP_SYSTEM_LIBRARIES" $fixup_paths`

old_IFS="$IFS"
IFS=:
  for dir in $GNUSTEP_LIBRARIES_PATHLIST; do

    # prepare the path_fragment for libraries and this dir
    if [ "$GNUSTEP_IS_FLATTENED" = "yes" ]; then
      path_fragment="$dir"
    else
      path_fragment="$dir/$GNUSTEP_HOST_CPU/$GNUSTEP_HOST_OS/$LIBRARY_COMBO:$dir/$GNUSTEP_HOST_CPU/$GNUSTEP_HOST_OS"
    fi

    # Append the path_fragment to lib_paths ... in a different way,
    # depending on the machine we're on.
    case "$GNUSTEP_HOST_OS" in

      *nextstep4* | *darwin*)
        if [ -z "$DYLD_LIBRARY_PATH" ]; then
          DYLD_LIBRARY_PATH="$path_fragment"
        else
          if ( echo ":${DYLD_LIBRARY_PATH}:"\
	    |grep -v ":${path_fragment}:" >/dev/null ); then
	    DYLD_LIBRARY_PATH="$path_fragment:$DYLD_LIBRARY_PATH"
          fi
        fi
        export DYLD_LIBRARY_PATH;;

      *hpux*)
        if [ -z "$SHLIB_PATH" ]; then
          SHLIB_PATH="$path_fragment"
        else
          if ( echo ":${SHLIB_PATH}:"\
	    |grep -v ":${path_fragment}:" >/dev/null ); then
	    SHLIB_PATH="$path_fragment:$SHLIB_PATH"
          fi
        fi
        export SHLIB_PATH;

        if [ -z "$LD_LIBRARY_PATH" ]; then
          LD_LIBRARY_PATH="$path_fragment"
        else
          if ( echo ":${LD_LIBRARY_PATH}:"\
	    |grep -v ":${path_fragment}:" >/dev/null ); then
	    LD_LIBRARY_PATH="$path_fragment:$LD_LIBRARY_PATH"
          fi
        fi
        export LD_LIBRARY_PATH;;

      *)
        if [ -z "$LD_LIBRARY_PATH" ]; then
          LD_LIBRARY_PATH="$path_fragment"
        else
          if ( echo ":${LD_LIBRARY_PATH}:"\
	    |grep -v ":${path_fragment}:" >/dev/null ); then
	    LD_LIBRARY_PATH="$path_fragment:$LD_LIBRARY_PATH"
          fi
        fi
        export LD_LIBRARY_PATH;;
    esac
  done
IFS="$old_IFS"
unset old_IFS
unset dir
unset path_fragment

unset GNUSTEP_LIBRARIES_PATHLIST


# Determine the framework paths - but only on Darwin since it's the
# only platform where this is of any use.
case "$GNUSTEP_HOST_OS" in

  *darwin*)
    GNUSTEP_FRAMEWORKS_PATHLIST=`$GNUSTEP_MAKEFILES/print_unique_pathlist.sh "$GNUSTEP_USER_LIBRARY/Frameworks" "$GNUSTEP_LOCAL_LIBRARY/Frameworks" "$GNUSTEP_NETWORK_LIBRARY/Frameworks" "$GNUSTEP_SYSTEM_LIBRARY/Frameworks" $fixup_paths`

    old_IFS="$IFS"
    IFS=:
    for dir in $GNUSTEP_FRAMEWORKS_PATHLIST; do

      # prepare the path_fragment for frameworks
      path_fragment="$dir"

# The code below has been temporarily removed, because...
# Frameworks in GNUstep-make are supported by creating a link like
# 
#   Libraries/libMyFramework.dylib ->
#      Frameworks/MyFramework.framework/Versions/Current/libMyFramework.dylib,
#
# to mitigate the fact that FSF GCC does not support a -framework flag.
#
# On Darwin, however, we partially emulate -framework by setting the
# "install_name" to the framework name during linking. The dynamic
# linker (dyld) is smart enough to find the framework under this name,
# but only if DYLD_FRAMEWORK_PATH is set (unless we set the
# "install_name" to an absolute path, which we don't). We'd really like
# to fully support -framework, though.
#
# Use otool -L MyApplication.app/MyApplication, for instance, to see
# how the shared libraries/frameworks are linked.
#
#    if [ "$LIBRARY_COMBO" = "apple-apple-apple" -o \
#         "$LIBRARY_COMBO" = "apple" ]; then
    
      if [ -z "$DYLD_FRAMEWORK_PATH" ]; then
        DYLD_FRAMEWORK_PATH="$path_fragment"
      else
        if ( echo ":${DYLD_FRAMEWORK_PATH}:"\
	  |grep -v ":${path_fragment}:" >/dev/null ); then
          DYLD_FRAMEWORK_PATH="$path_fragment:$DYLD_FRAMEWORK_PATH"
        fi
      fi
      export DYLD_FRAMEWORK_PATH
    done

    IFS="$old_IFS"
    unset old_IFS
    unset dir
    unset path_fragment

    unset GNUSTEP_FRAMEWORKS_PATHLIST
    ;;

  *)
    ;;
esac

#
# Setup Java CLASSPATH and Guile GUILE_LOAD_PATH
#
GNUSTEP_LIBRARY_PATHLIST=`$GNUSTEP_MAKEFILES/print_unique_pathlist.sh "$GNUSTEP_USER_LIBRARY" "$GNUSTEP_LOCAL_LIBRARY" "$GNUSTEP_NETWORK_LIBRARY" "$GNUSTEP_SYSTEM_LIBRARY" $fixup_paths`

old_IFS="$IFS"
IFS=:
for dir in $GNUSTEP_LIBRARY_PATHLIST; do

  path_fragment="$dir/Libraries/Java"
  if [ -z "$CLASSPATH" ]; then
    CLASSPATH="$path_fragment"
  else
    if ( echo ":${CLASSPATH}:"\
      |grep -v ":${path_fragment}:" >/dev/null ); then
      CLASSPATH="$CLASSPATH:$path_fragment"
    fi
  fi

  path_fragment="$dir/Libraries/Guile"
  if [ -z "$GUILE_LOAD_PATH" ]; then
    GUILE_LOAD_PATH="$path_fragment"
  else
    if ( echo ":${GUILE_LOAD_PATH}:"\
      |grep -v ":${path_fragment}:" >/dev/null ); then
      GUILE_LOAD_PATH="$path_fragment:$GUILE_LOAD_PATH"
    fi
  fi

done
IFS="$old_IFS"
unset old_IFS
unset dir
unset path_fragment
export CLASSPATH
export GUILE_LOAD_PATH

#
# Make sure info files, that we install by default into
# xxx/Library/Documentation/info, are found by the info browsing
# programs.  To get this effect, we add those paths to INFOPATH.
#
GNUSTEP_INFO_PATHLIST=`$GNUSTEP_MAKEFILES/print_unique_pathlist.sh "$GNUSTEP_USER_DOC_INFO" "$GNUSTEP_LOCAL_DOC_INFO" "$GNUSTEP_NETWORK_DOC_INFO" "$GNUSTEP_SYSTEM_DOC_INFO" $fixup_paths`
old_IFS="$IFS"
IFS=:
for dir in $GNUSTEP_INFO_PATHLIST; do

  if [ -z "$INFOPATH" ]; then
    # The ':' at the end means to use the built-in paths after searching
    # the INFOPATH we provide.
    INFOPATH="${dir}:"
  else
    if ( echo ":${INFOPATH}:"\
      |grep -v ":${dir}:" >/dev/null ); then
      INFOPATH="$INFOPATH:${dir}:"
    fi
  fi
done
IFS="$old_IFS"
unset old_IFS
unset dir
unset GNUSTEP_INFO_PATHLIST
export INFOPATH

#
# Clean up the environment by removing the filesystem variables.  Do
# it unless we were explicitly requested not to clean it up!  Mostly
# gnustep-config will request that the environment is not cleaned up,
# so it can print out all of the GNUstep variables.
#
if [ -n "$GNUSTEP_SH_EXPORT_ALL_VARIABLES" ]; then
  export GNUSTEP_MAKE_STRICT_V2_MODE

  unset GNUSTEP_KEEP_CONFIG_FILE GNUSTEP_KEEP_USER_CONFIG_FILE

  export GNUSTEP_CONFIG_FILE GNUSTEP_USER_CONFIG_FILE

  export GNUSTEP_USER_DEFAULTS_DIR

  # Always export these variables even if in strict gnustep-make v2
  # mode, so that for example gnustep-config can determine them.
  export GNUSTEP_IS_FLATTENED LIBRARY_COMBO
  export GNUSTEP_HOST GNUSTEP_HOST_CPU GNUSTEP_HOST_VENDOR GNUSTEP_HOST_OS

  export GNUSTEP_SYSTEM_APPS GNUSTEP_SYSTEM_ADMIN_APPS GNUSTEP_SYSTEM_WEB_APPS GNUSTEP_SYSTEM_TOOLS GNUSTEP_SYSTEM_ADMIN_TOOLS 
  export GNUSTEP_SYSTEM_LIBRARY GNUSTEP_SYSTEM_HEADERS GNUSTEP_SYSTEM_LIBRARIES
  export GNUSTEP_SYSTEM_DOC GNUSTEP_SYSTEM_DOC_MAN GNUSTEP_SYSTEM_DOC_INFO

  export GNUSTEP_NETWORK_APPS GNUSTEP_NETWORK_ADMIN_APPS GNUSTEP_NETWORK_WEB_APPS GNUSTEP_NETWORK_TOOLS GNUSTEP_NETWORK_ADMIN_TOOLS 
  export GNUSTEP_NETWORK_LIBRARY GNUSTEP_NETWORK_HEADERS GNUSTEP_NETWORK_LIBRARIES
  export GNUSTEP_NETWORK_DOC GNUSTEP_NETWORK_DOC_MAN GNUSTEP_NETWORK_DOC_INFO

  export GNUSTEP_LOCAL_APPS GNUSTEP_LOCAL_ADMIN_APPS GNUSTEP_LOCAL_WEB_APPS GNUSTEP_LOCAL_TOOLS GNUSTEP_LOCAL_ADMIN_TOOLS 
  export GNUSTEP_LOCAL_LIBRARY GNUSTEP_LOCAL_HEADERS GNUSTEP_LOCAL_LIBRARIES
  export GNUSTEP_LOCAL_DOC GNUSTEP_LOCAL_DOC_MAN GNUSTEP_LOCAL_DOC_INFO

  export GNUSTEP_USER_APPS GNUSTEP_USER_ADMIN_APPS GNUSTEP_USER_WEB_APPS GNUSTEP_USER_TOOLS GNUSTEP_USER_ADMIN_TOOLS 
  export GNUSTEP_USER_LIBRARY GNUSTEP_USER_HEADERS GNUSTEP_USER_LIBRARIES
  export GNUSTEP_USER_DOC GNUSTEP_USER_DOC_MAN GNUSTEP_USER_DOC_INFO

  export GNUSTEP_SYSTEM_USERS_DIR GNUSTEP_LOCAL_USERS_DIR GNUSTEP_NETWORK_USERS_DIR
else
  if [ "$GNUSTEP_MAKE_STRICT_V2_MODE" = "yes" ]; then
    unset GNUSTEP_IS_FLATTENED
    unset LIBRARY_COMBO
    unset GNUSTEP_HOST
    unset GNUSTEP_HOST_CPU
    unset GNUSTEP_HOST_VENDOR
    unset GNUSTEP_HOST_OS
  fi

  unset GNUSTEP_MAKE_STRICT_V2_MODE

  # Unset these variables but only if we set them internally; keep
  # them if they were already in the environment.
  if [ -z "$GNUSTEP_KEEP_CONFIG_FILE" ]; then
    unset GNUSTEP_CONFIG_FILE
  fi
  unset GNUSTEP_KEEP_CONFIG_FILE 

  if [ -z "$GNUSTEP_KEEP_USER_CONFIG_FILE" ]; then
    unset GNUSTEP_USER_CONFIG_FILE
  fi
  unset GNUSTEP_KEEP_USER_CONFIG_FILE

  # Always unset these variables
  unset GNUSTEP_USER_DEFAULTS_DIR

  unset GNUSTEP_SYSTEM_APPS 
  unset GNUSTEP_SYSTEM_ADMIN_APPS 
  unset GNUSTEP_SYSTEM_WEB_APPS 
  unset GNUSTEP_SYSTEM_TOOLS 
  unset GNUSTEP_SYSTEM_ADMIN_TOOLS 
  unset GNUSTEP_SYSTEM_LIBRARY
  unset GNUSTEP_SYSTEM_HEADERS 
  unset GNUSTEP_SYSTEM_LIBRARIES 
  unset GNUSTEP_SYSTEM_DOC 
  unset GNUSTEP_SYSTEM_DOC_MAN
  unset GNUSTEP_SYSTEM_DOC_INFO

  unset GNUSTEP_NETWORK_APPS 
  unset GNUSTEP_NETWORK_ADMIN_APPS 
  unset GNUSTEP_NETWORK_WEB_APPS 
  unset GNUSTEP_NETWORK_TOOLS 
  unset GNUSTEP_NETWORK_ADMIN_TOOLS 
  unset GNUSTEP_NETWORK_LIBRARY
  unset GNUSTEP_NETWORK_HEADERS 
  unset GNUSTEP_NETWORK_LIBRARIES 
  unset GNUSTEP_NETWORK_DOC 
  unset GNUSTEP_NETWORK_DOC_MAN
  unset GNUSTEP_NETWORK_DOC_INFO

  unset GNUSTEP_LOCAL_APPS 
  unset GNUSTEP_LOCAL_ADMIN_APPS 
  unset GNUSTEP_LOCAL_WEB_APPS 
  unset GNUSTEP_LOCAL_TOOLS 
  unset GNUSTEP_LOCAL_ADMIN_TOOLS 
  unset GNUSTEP_LOCAL_LIBRARY
  unset GNUSTEP_LOCAL_HEADERS 
  unset GNUSTEP_LOCAL_LIBRARIES 
  unset GNUSTEP_LOCAL_DOC 
  unset GNUSTEP_LOCAL_DOC_MAN
  unset GNUSTEP_LOCAL_DOC_INFO

  unset GNUSTEP_USER_APPS 
  unset GNUSTEP_USER_ADMIN_APPS 
  unset GNUSTEP_USER_WEB_APPS 
  unset GNUSTEP_USER_TOOLS 
  unset GNUSTEP_USER_ADMIN_TOOLS 
  unset GNUSTEP_USER_LIBRARY
  unset GNUSTEP_USER_HEADERS 
  unset GNUSTEP_USER_LIBRARIES 
  unset GNUSTEP_USER_DOC 
  unset GNUSTEP_USER_DOC_MAN
  unset GNUSTEP_USER_DOC_INFO

  unset GNUSTEP_SYSTEM_USERS_DIR
  unset GNUSTEP_LOCAL_USERS_DIR
  unset GNUSTEP_NETWORK_USERS_DIR
fi

if [ -n "$GS_ZSH_NEED_TO_RESTORE_SET" ]; then
  set +y
fi
# EOF
