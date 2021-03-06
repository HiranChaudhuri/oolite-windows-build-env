#   This file must be sourced inside csh using: source
#
#   GNUstep.csh.  Generated from GNUstep.csh.in by configure.
#
#   Shell initialization for the GNUstep environment.
#
#   Copyright (C) 1998-2008 Free Software Foundation, Inc.
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

# When this is set to 'yes', strict gnustep-make v2 compatibility mode
# is turned on.
set GNUSTEP_MAKE_STRICT_V2_MODE=no

#
# Read our configuration files
#

# Determine the location of the system configuration file
if ( ! ${?GNUSTEP_CONFIG_FILE} ) then
  setenv GNUSTEP_CONFIG_FILE "/etc/GNUstep/GNUstep.conf"
else
  set GNUSTEP_KEEP_CONFIG_FILE=yes
endif

# Determine the location of the user configuration file
if ( ! ${?GNUSTEP_USER_CONFIG_FILE} ) then
  setenv GNUSTEP_USER_CONFIG_FILE ".GNUstep.conf"
else
  set GNUSTEP_KEEP_USER_CONFIG_FILE=yes
endif

# Read the system configuration file
if ( -e "${GNUSTEP_CONFIG_FILE}" ) then
  #
  # Convert the config file from sh syntax to csh syntax, and execute it.
  #
  # We want to convert every line of the type ^xxx=yyy$ into setenv xxx yyy;
  # and ignore any other line.
  #
  # This sed expression will first delete all lines that don't match
  # the pattern ^[^#=][^#=]*=.*$ -- which means "start of line (^),
  # followed by a character that is not # and not = ([^#=]), followed
  # by 0 or more characters that are not # and not = ([^#=]*),
  # followed by a = (=), followed by some characters until end of the
  # line (.*$).  It will then replace each occurrence of the same
  # pattern (where the first and second relevant parts are now tagged
  # -- that's what the additional \(...\) do) with 'setenv \1 \2'.
  #
  # The result of all this is ... something that we want to execute!
  # We use eval to execute the results of `...`.
  #
  # Please note that ! must always be escaped in csh, which is why we
  # write \\!
  #
  # Also note that we add a ';' at the end of each setenv command so
  # that we can pipe all the commands through a single eval.
  #
  eval `sed -e '/^[^#=][^#=]*=.*$/\\!d' -e 's/^\([^#=][^#=]*\)=\(.*\)$/setenv \1 \2;/' "${GNUSTEP_CONFIG_FILE}"`
endif

# FIXME: determining GNUSTEP_HOME
set GNUSTEP_HOME = ~

# Read the user configuration file ... unless it is disabled (ie, set
# to an empty string)
if ( ${?GNUSTEP_USER_CONFIG_FILE} ) then
  switch ("${GNUSTEP_USER_CONFIG_FILE}")
   case /*: # An absolute path
     if ( -e "${GNUSTEP_USER_CONFIG_FILE}" ) then
      # See above for an explanation of the sed expression
      eval `sed -e '/^[^#=][^#=]*=.*$/\\!d' -e 's/^\([^#=][^#=]*\)=\(.*\)$/setenv \1 \2;/' "${GNUSTEP_USER_CONFIG_FILE}"`
     endif
     breaksw
   default: # Something else
     if ( -e "${GNUSTEP_HOME}/${GNUSTEP_USER_CONFIG_FILE}" ) then
       eval `sed -e '/^[^#=][^#=]*=.*$/\\!d' -e 's/^\([^#=][^#=]*\)=\(.*\)$/setenv \1 \2;/' "${GNUSTEP_HOME}/${GNUSTEP_USER_CONFIG_FILE}"`
     endif
     breaksw
   endsw
endif

# Now, set any essential variable (that is not already set) to the
# built-in values.
if ( "${GNUSTEP_MAKE_STRICT_V2_MODE}" == "yes" ) then
  unsetenv GNUSTEP_SYSTEM_ROOT
  unsetenv GNUSTEP_LOCAL_ROOT
  unsetenv GNUSTEP_NETWORK_ROOT
  unsetenv GNUSTEP_FLATTENED
else
  if ( ! ${?GNUSTEP_SYSTEM_ROOT} ) then
    setenv GNUSTEP_SYSTEM_ROOT "/mingw/../devlibs/gnustep1201/System"
  endif

  if ( ! ${?GNUSTEP_LOCAL_ROOT} ) then
    setenv GNUSTEP_LOCAL_ROOT "/mingw/../devlibs/gnustep1201/Local"
  endif

  if ( ! ${?GNUSTEP_NETWORK_ROOT} ) then
    setenv GNUSTEP_NETWORK_ROOT "/mingw/../devlibs/gnustep1201/Network"
  endif

  # GNUSTEP_FLATTENED is obsolete, please use GNUSTEP_IS_FLATTENED
  # instead
  setenv GNUSTEP_FLATTENED "yes"
endif

setenv GNUSTEP_IS_FLATTENED "yes"
if ( ! ${?LIBRARY_COMBO} ) then
  setenv LIBRARY_COMBO "gnu-gnu-gnu"
endif

if ( ! ${?GNUSTEP_MAKEFILES} ) then
  setenv GNUSTEP_MAKEFILES "/mingw/../devlibs/gnustep1201/System/Library/Makefiles"
endif

if ( "${GNUSTEP_MAKE_STRICT_V2_MODE}" == "yes" ) then
  unsetenv GNUSTEP_USER_DIR
  unsetenv GNUSTEP_USER_ROOT
else
  if ( ! ${?GNUSTEP_USER_DIR} ) then
    setenv GNUSTEP_USER_DIR "GNUstep"
  endif

  #
  # Set GNUSTEP_USER_ROOT which is the variable used in practice
  #
  switch ("${GNUSTEP_USER_DIR}")
   case /*: # An absolute path
     setenv GNUSTEP_USER_ROOT "${GNUSTEP_USER_DIR}"
     breaksw
   default: # Something else
     setenv GNUSTEP_USER_ROOT "${GNUSTEP_HOME}/${GNUSTEP_USER_DIR}"
     breaksw
  endsw

  unsetenv GNUSTEP_USER_DIR
endif

if ( "" == "" ) then
  setenv GNUSTEP_HOST "x86_64-w64-mingw32"
  setenv GNUSTEP_HOST_CPU "x86_64"
  setenv GNUSTEP_HOST_VENDOR "w64"
  setenv GNUSTEP_HOST_OS "mingw32"
endif

#
# Determine the host information
#
if ( ! ${?GNUSTEP_HOST} ) then
  pushd /tmp > /dev/null
  setenv GNUSTEP_HOST `${GNUSTEP_MAKEFILES}/config.guess`
  setenv GNUSTEP_HOST `${GNUSTEP_MAKEFILES}/config.sub ${GNUSTEP_HOST}`
  popd > /dev/null
endif

if ( ! ${?GNUSTEP_HOST_CPU} ) then
  setenv GNUSTEP_HOST_CPU `${GNUSTEP_MAKEFILES}/cpu.sh ${GNUSTEP_HOST}`
  setenv GNUSTEP_HOST_CPU `${GNUSTEP_MAKEFILES}/clean_cpu.sh ${GNUSTEP_HOST_CPU}`
endif

if ( ! ${?GNUSTEP_HOST_VENDOR} ) then
  setenv GNUSTEP_HOST_VENDOR `${GNUSTEP_MAKEFILES}/vendor.sh ${GNUSTEP_HOST}`
  setenv GNUSTEP_HOST_VENDOR `${GNUSTEP_MAKEFILES}/clean_vendor.sh ${GNUSTEP_HOST_VENDOR}`
endif

if ( ! ${?GNUSTEP_HOST_OS} ) then
  setenv GNUSTEP_HOST_OS `${GNUSTEP_MAKEFILES}/os.sh ${GNUSTEP_HOST}`
  setenv GNUSTEP_HOST_OS `${GNUSTEP_MAKEFILES}/clean_os.sh ${GNUSTEP_HOST_OS}`
endif

# Now load in all the remaining paths
source "${GNUSTEP_MAKEFILES}/filesystem.csh"

# No longer needed
unset GNUSTEP_HOME

# Determine if the paths look like Windows paths that need fixing
set fixup_paths=no
if ( `echo $GNUSTEP_MAKEFILES | sed 's|^[a-zA-Z]:/.*$||'` == "" ) then
  set fixup_paths=yes
endif

#
# Add the GNUstep tools directories to the path
#
if ( "${GNUSTEP_MAKE_STRICT_V2_MODE}" == "no" ) then
  if ( ! ${?GNUSTEP_PATHLIST} ) then
   setenv GNUSTEP_PATHLIST `$GNUSTEP_MAKEFILES/print_unique_pathlist.sh "$GNUSTEP_USER_ROOT" "$GNUSTEP_LOCAL_ROOT" "$GNUSTEP_NETWORK_ROOT" "$GNUSTEP_SYSTEM_ROOT" $fixup_paths`
  endif
endif

set GNUSTEP_TOOLS_PATHLIST=`$GNUSTEP_MAKEFILES/print_unique_pathlist.sh "$GNUSTEP_USER_TOOLS" "$GNUSTEP_LOCAL_TOOLS" "$GNUSTEP_NETWORK_TOOLS" "$GNUSTEP_SYSTEM_TOOLS" $fixup_paths`

foreach dir ( "${GNUSTEP_SYSTEM_ADMIN_TOOLS}" "${GNUSTEP_NETWORK_ADMIN_TOOLS}" "${GNUSTEP_LOCAL_ADMIN_TOOLS}" "${GNUSTEP_USER_ADMIN_TOOLS}" )
  if ( -d "${dir}"  &&  -w "${dir}" ) then
    if ( { (echo ":${GNUSTEP_TOOLS_PATHLIST}:"\
      |grep -v ":${dir}:" >/dev/null) } ) then
      setenv GNUSTEP_TOOLS_PATHLIST "${dir}:${GNUSTEP_TOOLS_PATHLIST}"
    endif
  endif
end

foreach dir ( `/bin/sh -c 'IFS=:; for i in '"${GNUSTEP_TOOLS_PATHLIST}"'; do echo $i; done'` )
  set path_fragment="${dir}"
  if ( "${GNUSTEP_IS_FLATTENED}" == "no" ) then
    set path_fragment="${dir}:${dir}/${GNUSTEP_HOST_CPU}/${GNUSTEP_HOST_OS}/${LIBRARY_COMBO}:${dir}/${GNUSTEP_HOST_CPU}/${GNUSTEP_HOST_OS}"
  endif

  if ( ! ${?PATH} ) then
    setenv PATH "${path_fragment}"
  else if ( { (echo ":${PATH}:"\
    |grep -v ":${path_fragment}:" >/dev/null) } ) then
    setenv PATH "${path_fragment}:${PATH}"
  endif
end
unset path_fragment dir
unset GNUSTEP_TOOLS_PATHLIST

set GNUSTEP_LIBRARIES_PATHLIST=`$GNUSTEP_MAKEFILES/print_unique_pathlist.sh "$GNUSTEP_USER_LIBRARIES" "$GNUSTEP_LOCAL_LIBRARIES" "$GNUSTEP_NETWORK_LIBRARIES" "$GNUSTEP_SYSTEM_LIBRARIES" $fixup_paths`
foreach dir ( `/bin/sh -c 'IFS=:; for i in '"${GNUSTEP_LIBRARIES_PATHLIST}"'; do echo $i; done'` )
  if ( "$GNUSTEP_IS_FLATTENED" == "yes" ) then
    set path_fragment="$dir"
  else
    set path_fragment="$dir/$GNUSTEP_HOST_CPU/$GNUSTEP_HOST_OS/$LIBRARY_COMBO:$dir/$GNUSTEP_HOST_CPU/$GNUSTEP_HOST_OS"
  endif

  switch ( "${GNUSTEP_HOST_OS}" )

    case *nextstep4* :
    case *darwin* :
      if ( $?DYLD_LIBRARY_PATH == 0 ) then
	  setenv DYLD_LIBRARY_PATH "${path_fragment}"
      else if ( { (echo ":${DYLD_LIBRARY_PATH}:"\
        |grep -v ":${path_fragment}:" >/dev/null) } ) then
	  setenv DYLD_LIBRARY_PATH "${path_fragment}:${DYLD_LIBRARY_PATH}"
      endif
      breaksw

    case *hpux* :
      if ( $?SHLIB_PATH == 0 ) then
	  setenv SHLIB_PATH "${path_fragment}"
      else if ( { (echo ":${SHLIB_PATH}:"\
	|grep -v ":${path_fragment}:" >/dev/null) } ) then
	  setenv SHLIB_PATH "${path_fragment}:${SHLIB_PATH}"
      endif

      if ( $?LD_LIBRARY_PATH == 0 ) then
	  setenv LD_LIBRARY_PATH "${path_fragment}"
      else if ( { (echo ":${LD_LIBRARY_PATH}:"\
	|grep -v ":${path_fragment}:" >/dev/null) } ) then
	  setenv LD_LIBRARY_PATH "${path_fragment}:${LD_LIBRARY_PATH}"
      endif

      breaksw

    case * :
      if ( $?LD_LIBRARY_PATH == 0 ) then
	  setenv LD_LIBRARY_PATH "${path_fragment}"
      else if ( { (echo ":${LD_LIBRARY_PATH}:"\
	|grep -v ":${path_fragment}:" >/dev/null) } ) then
	  setenv LD_LIBRARY_PATH "${path_fragment}:${LD_LIBRARY_PATH}"
      endif

      breaksw
  endsw
end
unset path_fragment dir
unset GNUSTEP_LIBRARIES_PATHLIST

switch ( "${GNUSTEP_HOST_OS}" )
  case *darwin* :
    set GNUSTEP_FRAMEWORKS_PATHLIST=`$GNUSTEP_MAKEFILES/print_unique_pathlist.sh "$GNUSTEP_USER_LIBRARY/Frameworks" "$GNUSTEP_LOCAL_LIBRARY/Frameworks" "$GNUSTEP_NETWORK_LIBRARY/Frameworks" "$GNUSTEP_SYSTEM_LIBRARY/Frameworks" $fixup_paths`
    foreach dir ( `/bin/sh -c 'IFS=:; for i in '"${GNUSTEP_FRAMEWORKS_PATHLIST}"'; do echo $i; done'` )
      set path_fragment="$dir"
      if ( $?DYLD_FRAMEWORK_PATH == 0 ) then
        setenv DYLD_FRAMEWORK_PATH "${path_fragment}"
      else if ( { (echo ":${DYLD_FRAMEWORK_PATH}:"\
	|grep -v ":${path_fragment}:" >/dev/null) } ) then
        setenv DYLD_FRAMEWORK_PATH "${path_fragment}:${DYLD_FRAMEWORK_PATH}"
      endif
    end
    unset dir path_fragment
    unset GNUSTEP_FRAMEWORKS_PATHLIST
    breaksw
  case * :
    breaksw
endsw

set GNUSTEP_LIBRARY_PATHLIST=`$GNUSTEP_MAKEFILES/print_unique_pathlist.sh "$GNUSTEP_USER_LIBRARY" "$GNUSTEP_LOCAL_LIBRARY" "$GNUSTEP_NETWORK_LIBRARY" "$GNUSTEP_SYSTEM_LIBRARY" $fixup_paths`

foreach dir ( `/bin/sh -c 'IFS=:; for i in '"${GNUSTEP_LIBRARY_PATHLIST}"'; do echo $i; done'` )

  set path_fragment="${dir}/Libraries/Java"
  if ( ! ${?CLASSPATH} ) then
    setenv CLASSPATH "${path_fragment}"
  else if ( { (echo ":${CLASSPATH}:"\
    |grep -v ":${path_fragment}:" >/dev/null) } ) then
    setenv CLASSPATH "${CLASSPATH}:${path_fragment}"
  endif

  set path_fragment="${dir}/Libraries/Guile"
  if ( ! ${?GUILE_LOAD_PATH} ) then
    setenv GUILE_LOAD_PATH "${path_fragment}"
  else if ( { (echo ":${GUILE_LOAD_PATH}:"\
    |grep -v ":${path_fragment}:" >/dev/null) } ) then
    setenv GUILE_LOAD_PATH "${path_fragment}:${GUILE_LOAD_PATH}"
  endif

end
unset path_fragment dir
unset GNUSTEP_TOOLS_PATHLIST

set GNUSTEP_INFO_PATHLIST=`$GNUSTEP_MAKEFILES/print_unique_pathlist.sh "$GNUSTEP_USER_DOC_INFO" "$GNUSTEP_LOCAL_DOC_INFO" "$GNUSTEP_NETWORK_DOC_INFO" "$GNUSTEP_SYSTEM_DOC_INFO" $fixup_paths`

foreach dir ( `/bin/sh -c 'IFS=:; for i in '"${GNUSTEP_INFO_PATHLIST}"'; do echo $i; done'` )

  if ( ! ${?INFOPATH} ) then
    setenv INFOPATH "${dir}:"
  else if ( { (echo ":${INFOPATH}:"\
    |grep -v ":${dir}:" >/dev/null) } ) then
    setenv INFOPATH "${INFOPATH}:${dir}:"
  endif

end

unset dir

if ( "${GNUSTEP_MAKE_STRICT_V2_MODE}" == "yes" ) then
  unsetenv GNUSTEP_IS_FLATTENED
  unsetenv LIBRARY_COMBO
  unsetenv GNUSTEP_HOST
  unsetenv GNUSTEP_HOST_CPU
  unsetenv GNUSTEP_HOST_VENDOR
  unsetenv GNUSTEP_HOST_OS
endif

unset GNUSTEP_MAKE_STRICT_V2_MODE

if ( ! {$?GNUSTEP_KEEP_CONFIG_FILE} ) then
  unsetenv GNUSTEP_CONFIG_FILE
endif
unset GNUSTEP_KEEP_CONFIG_FILE

if ( ! ${?GNUSTEP_KEEP_USER_CONFIG_FILE} ) then
  unsetenv GNUSTEP_USER_CONFIG_FILE
endif
unset GNUSTEP_KEEP_USER_CONFIG_FILE

unsetenv GNUSTEP_USER_DEFAULTS_DIR

unsetenv GNUSTEP_SYSTEM_APPS 
unsetenv GNUSTEP_SYSTEM_ADMIN_APPS 
unsetenv GNUSTEP_SYSTEM_WEB_APPS 
unsetenv GNUSTEP_SYSTEM_TOOLS 
unsetenv GNUSTEP_SYSTEM_ADMIN_TOOLS 
unsetenv GNUSTEP_SYSTEM_LIBRARY
unsetenv GNUSTEP_SYSTEM_HEADERS 
unsetenv GNUSTEP_SYSTEM_LIBRARIES 
unsetenv GNUSTEP_SYSTEM_DOC 
unsetenv GNUSTEP_SYSTEM_DOC_MAN
unsetenv GNUSTEP_SYSTEM_DOC_INFO

unsetenv GNUSTEP_NETWORK_APPS 
unsetenv GNUSTEP_NETWORK_ADMIN_APPS 
unsetenv GNUSTEP_NETWORK_WEB_APPS 
unsetenv GNUSTEP_NETWORK_TOOLS 
unsetenv GNUSTEP_NETWORK_ADMIN_TOOLS
unsetenv GNUSTEP_NETWORK_LIBRARY
unsetenv GNUSTEP_NETWORK_HEADERS 
unsetenv GNUSTEP_NETWORK_LIBRARIES 
unsetenv GNUSTEP_NETWORK_DOC 
unsetenv GNUSTEP_NETWORK_DOC_MAN
unsetenv GNUSTEP_NETWORK_DOC_INFO

unsetenv GNUSTEP_LOCAL_APPS 
unsetenv GNUSTEP_LOCAL_ADMIN_APPS 
unsetenv GNUSTEP_LOCAL_WEB_APPS 
unsetenv GNUSTEP_LOCAL_TOOLS 
unsetenv GNUSTEP_LOCAL_ADMIN_TOOLS
unsetenv GNUSTEP_LOCAL_LIBRARY
unsetenv GNUSTEP_LOCAL_HEADERS 
unsetenv GNUSTEP_LOCAL_LIBRARIES 
unsetenv GNUSTEP_LOCAL_DOC 
unsetenv GNUSTEP_LOCAL_DOC_MAN
unsetenv GNUSTEP_LOCAL_DOC_INFO

unsetenv GNUSTEP_USER_APPS 
unsetenv GNUSTEP_USER_ADMIN_APPS
unsetenv GNUSTEP_USER_WEB_APPS
unsetenv GNUSTEP_USER_TOOLS 
unsetenv GNUSTEP_USER_ADMIN_TOOLS 
unsetenv GNUSTEP_USER_LIBRARY
unsetenv GNUSTEP_USER_HEADERS 
unsetenv GNUSTEP_USER_LIBRARIES 
unsetenv GNUSTEP_USER_DOC 
unsetenv GNUSTEP_USER_DOC_MAN
unsetenv GNUSTEP_USER_DOC_INFO

unsetenv GNUSTEP_SYSTEM_USERS_DIR
unsetenv GNUSTEP_LOCAL_USERS_DIR
unsetenv GNUSTEP_NETWORK_USERS_DIR

