/* Area:	ffi_call, closure_call
   Purpose:	Check structure passing with different structure size.
		Contains structs as parameter of the struct itself.
		Sample taken from Alan Modras patch to src/prep_cif.c.
   Limitations:	none.
   PR:		none.
   Originator:	<andreast@gcc.gnu.org> 20030911	 */

/* { dg-do run } */
#include "ffitest.h"

typedef struct A {
  unsigned long a;
  unsigned char b;
} A;

typedef struct B {
  struct A x;
  unsigned char y;
} B;

B B_fn(struct A b0, struct B b1)
{
  struct B result;

  result.x.a = b0.a + b1.x.a;
  result.x.b = b0.b + b1.x.b + b1.y;
  result.y = b0.b + b1.x.b;

  printf("%lu %d %lu %d %d: %lu %d %d\n", b0.a, b0.b, b1.x.a, b1.x.b, b1.y,
	 result.x.a, result.x.b, result.y);

  return result;
}

static void
B_gn(ffi_cif* cif __UNUSED__, void* resp, void** args,
     void* userdata __UNUSED__)
{
  struct A b0;
  struct B b1;

  b0 = *(struct A*)(args[0]);
  b1 = *(struct B*)(args[1]);

  *(B*)resp = B_fn(b0, b1);
}

int main (void)
{
  ffi_cif cif;
#ifndef USING_MMAP
  static ffi_closure cl;
#endif
  ffi_closure *pcl;
  void* args_dbl[3];
  ffi_type* cls_struct_fields[3];
  ffi_type* cls_struct_fields1[3];
  ffi_type cls_struct_type, cls_struct_type1;
  ffi_type* dbl_arg_types[3];

#ifdef USING_MMAP
  pcl = allocate_mmap (sizeof(ffi_closure));
#else
  pcl = &cl;
#endif

  cls_struct_type.size = 0;
  cls_struct_type.alignment = 0;
  cls_struct_type.type = FFI_TYPE_STRUCT;
  cls_struct_type.elements = cls_struct_fields;

  cls_struct_type1.size = 0;
  cls_struct_type1.alignment = 0;
  cls_struct_type1.type = FFI_TYPE_STRUCT;
  cls_struct_type1.elements = cls_struct_fields1;

  struct A e_dbl = { 1, 7};
  struct B f_dbl = {{12 , 127}, 99};

  struct B res_dbl;

  cls_struct_fields[0] = &ffi_type_ulong;
  cls_struct_fields[1] = &ffi_type_uchar;
  cls_struct_fields[2] = NULL;

  cls_struct_fields1[0] = &cls_struct_type;
  cls_struct_fields1[1] = &ffi_type_uchar;
  cls_struct_fields1[2] = NULL;


  dbl_arg_types[0] = &cls_struct_type;
  dbl_arg_types[1] = &cls_struct_type1;
  dbl_arg_types[2] = NULL;

  CHECK(ffi_prep_cif(&cif, FFI_DEFAULT_ABI, 2, &cls_struct_type1,
		     dbl_arg_types) == FFI_OK);

  args_dbl[0] = &e_dbl;
  args_dbl[1] = &f_dbl;
  args_dbl[2] = NULL;

  ffi_call(&cif, FFI_FN(B_fn), &res_dbl, args_dbl);
  /* { dg-output "1 7 12 127 99: 13 233 134" } */
  CHECK( res_dbl.x.a == (e_dbl.a + f_dbl.x.a));
  CHECK( res_dbl.x.b == (e_dbl.b + f_dbl.x.b + f_dbl.y));
  CHECK( res_dbl.y == (e_dbl.b + f_dbl.x.b));

  CHECK(ffi_prep_closure(pcl, &cif, B_gn, NULL) == FFI_OK);

  res_dbl = ((B(*)(A, B))(pcl))(e_dbl, f_dbl);
  /* { dg-output "\n1 7 12 127 99: 13 233 134" } */
  CHECK( res_dbl.x.a == (e_dbl.a + f_dbl.x.a));
  CHECK( res_dbl.x.b == (e_dbl.b + f_dbl.x.b + f_dbl.y));
  CHECK( res_dbl.y == (e_dbl.b + f_dbl.x.b));

  exit(0);
}
