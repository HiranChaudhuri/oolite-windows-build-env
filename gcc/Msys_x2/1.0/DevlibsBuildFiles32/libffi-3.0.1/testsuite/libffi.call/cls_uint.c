/* Area:	closure_call
   Purpose:	Check return value uint.
   Limitations:	none.
   PR:		none.
   Originator:	<andreast@gcc.gnu.org> 20030828	 */

/* { dg-do run } */
#include "ffitest.h"

static void cls_ret_uint_fn(ffi_cif* cif __UNUSED__, void* resp, void** args,
			    void* userdata __UNUSED__)
{
  *(ffi_arg *)resp = *(unsigned int *)args[0];

  printf("%d: %d\n",*(unsigned int *)args[0],
	 (int)*(ffi_arg *)(resp));
}
typedef unsigned int (*cls_ret_uint)(unsigned int);

int main (void)
{
  ffi_cif cif;
#ifndef USING_MMAP
  static ffi_closure cl;
#endif
  ffi_closure *pcl;
  ffi_type * cl_arg_types[2];
  unsigned int res;

#ifdef USING_MMAP
  pcl = allocate_mmap (sizeof(ffi_closure));
#else
  pcl = &cl;
#endif

  cl_arg_types[0] = &ffi_type_uint;
  cl_arg_types[1] = NULL;

  /* Initialize the cif */
  CHECK(ffi_prep_cif(&cif, FFI_DEFAULT_ABI, 1,
		     &ffi_type_uint, cl_arg_types) == FFI_OK);

  CHECK(ffi_prep_closure(pcl, &cif, cls_ret_uint_fn, NULL)  == FFI_OK);

  res = (*((cls_ret_uint)pcl))(2147483647);
  /* { dg-output "2147483647: 2147483647" } */
  printf("res: %d\n",res);
  /* { dg-output "\nres: 2147483647" } */

  exit(0);
}
