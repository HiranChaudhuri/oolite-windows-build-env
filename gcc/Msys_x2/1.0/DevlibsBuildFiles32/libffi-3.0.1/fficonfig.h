/* fficonfig.h.  Generated from fficonfig.h.in by configure.  */
/* fficonfig.h.in.  Generated from configure.ac by autoheader.  */

/* Define to one of `_getb67', `GETB67', `getb67' for Cray-2 and Cray-YMP
   systems. This function is required for `alloca.c' support on those systems.
   */
/* #undef CRAY_STACKSEG_END */

/* Define to 1 if using `alloca.c'. */
/* #undef C_ALLOCA */

/* Define to the flags needed for the .section .eh_frame directive. */
#define EH_FRAME_FLAGS "aw"

/* Define this if you want extra debugging. */
/* #undef FFI_DEBUG */

/* Define this is you do not want support for the raw API. */
/* #undef FFI_NO_RAW_API */

/* Define this is you do not want support for aggregate types. */
/* #undef FFI_NO_STRUCTS */

/* Define to 1 if you have `alloca', as a function or macro. */
#define HAVE_ALLOCA 1

/* Define to 1 if you have <alloca.h> and it should be used (not on Ultrix).
   */
/* #undef HAVE_ALLOCA_H */

/* Define if your assembler supports .cfi_* directives. */
#define HAVE_AS_CFI_PSEUDO_OP 1

/* Define if your assembler supports .register. */
/* #undef HAVE_AS_REGISTER_PSEUDO_OP */

/* Define if your assembler and linker support unaligned PC relative relocs.
   */
/* #undef HAVE_AS_SPARC_UA_PCREL */

/* Define to 1 if you have the <dlfcn.h> header file. */
/* #undef HAVE_DLFCN_H */

/* Define if __attribute__((visibility("hidden"))) is supported. */
/* #undef HAVE_HIDDEN_VISIBILITY_ATTRIBUTE */

/* Define to 1 if you have the <inttypes.h> header file. */
#define HAVE_INTTYPES_H 1

/* Define if you have the long double type and it is bigger than a double */
#define HAVE_LONG_DOUBLE 1

/* Define to 1 if you have the `memcpy' function. */
#define HAVE_MEMCPY 1

/* Define to 1 if you have the <memory.h> header file. */
#define HAVE_MEMORY_H 1

/* Define to 1 if you have the `mmap' function. */
/* #undef HAVE_MMAP */

/* Define if mmap with MAP_ANON(YMOUS) works. */
/* #undef HAVE_MMAP_ANON */

/* Define if mmap of /dev/zero works. */
/* #undef HAVE_MMAP_DEV_ZERO */

/* Define if read-only mmap of a plain file works. */
/* #undef HAVE_MMAP_FILE */

/* Define if .eh_frame sections should be read-only. */
/* #undef HAVE_RO_EH_FRAME */

/* Define to 1 if you have the <stdint.h> header file. */
#define HAVE_STDINT_H 1

/* Define to 1 if you have the <stdlib.h> header file. */
#define HAVE_STDLIB_H 1

/* Define to 1 if you have the <strings.h> header file. */
#define HAVE_STRINGS_H 1

/* Define to 1 if you have the <string.h> header file. */
#define HAVE_STRING_H 1

/* Define to 1 if you have the <sys/mman.h> header file. */
/* #undef HAVE_SYS_MMAN_H */

/* Define to 1 if you have the <sys/stat.h> header file. */
#define HAVE_SYS_STAT_H 1

/* Define to 1 if you have the <sys/types.h> header file. */
#define HAVE_SYS_TYPES_H 1

/* Define to 1 if you have the <unistd.h> header file. */
#define HAVE_UNISTD_H 1

/* Define to 1 if your C compiler doesn't accept -c and -o together. */
/* #undef NO_MINUS_C_MINUS_O */

/* Name of package */
#define PACKAGE "libffi"

/* Define to the address where bug reports for this package should be sent. */
#define PACKAGE_BUGREPORT "http://gcc.gnu.org/bugs.html"

/* Define to the full name of this package. */
#define PACKAGE_NAME "libffi"

/* Define to the full name and version of this package. */
#define PACKAGE_STRING "libffi 3.0.1"

/* Define to the one symbol short name of this package. */
#define PACKAGE_TARNAME "libffi"

/* Define to the version of this package. */
#define PACKAGE_VERSION "3.0.1"

/* The size of `double', as computed by sizeof. */
#define SIZEOF_DOUBLE 8

/* The size of `long double', as computed by sizeof. */
#define SIZEOF_LONG_DOUBLE 12

/* If using the C implementation of alloca, define if you know the
   direction of stack growth for your system; otherwise it will be
   automatically deduced at runtime.
	STACK_DIRECTION > 0 => grows toward higher addresses
	STACK_DIRECTION < 0 => grows toward lower addresses
	STACK_DIRECTION = 0 => direction of growth unknown */
/* #undef STACK_DIRECTION */

/* Define to 1 if you have the ANSI C header files. */
#define STDC_HEADERS 1

/* Define this if you are using Purify and want to suppress spurious messages.
   */
/* #undef USING_PURIFY */

/* Version number of package */
#define VERSION "3.0.1"

/* Define to 1 if your processor stores words with the most significant byte
   first (like Motorola and SPARC, unlike Intel and VAX). */
/* #undef WORDS_BIGENDIAN */


#ifdef HAVE_HIDDEN_VISIBILITY_ATTRIBUTE
#ifdef LIBFFI_ASM
#define FFI_HIDDEN(name) .hidden name
#else
#define FFI_HIDDEN __attribute__ ((visibility ("hidden")))
#endif
#else
#ifdef LIBFFI_ASM
#define FFI_HIDDEN(name)
#else
#define FFI_HIDDEN
#endif
#endif
