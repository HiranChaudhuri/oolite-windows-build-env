/* Area:		ffi_call, closure_call
   Purpose:		Test long doubles passed in variable argument lists.
   Limitations:	none.
   PR:			none.
   Originator:	Blake Chaffin 6/6/2007	 */

/* { dg-do run { xfail strongarm*-*-* xscale*-*-* } } */
/* { dg-output "" { xfail avr32*-*-* x86_64-*-mingw* } } */
/* { dg-skip-if "" arm*-*-* { "-mfloat-abi=hard" } { "" } } */

#include "ffitest.h"

static void
cls_longdouble_va_fn(ffi_cif* cif __UNUSED__, void* resp, 
		     void** args, void* userdata __UNUSED__)
{
	char*		format	= *(char**)args[0];
	long double	ldValue	= *(long double*)args[1];

	*(ffi_arg*)resp = printf(format, ldValue);
}

int main (void)
{
	ffi_cif cif;
        void *code;
	ffi_closure *pcl = ffi_closure_alloc(sizeof(ffi_closure), &code);
	void* args[3];
	ffi_type* arg_types[3];

	char*		format	= "%.1Lf\n";
	long double	ldArg	= 7;
	ffi_arg		res		= 0;

	arg_types[0] = &ffi_type_pointer;
	arg_types[1] = &ffi_type_longdouble;
	arg_types[2] = NULL;

	CHECK(ffi_prep_cif(&cif, FFI_DEFAULT_ABI, 2, &ffi_type_sint,
		arg_types) == FFI_OK);

	args[0] = &format;
	args[1] = &ldArg;
	args[2] = NULL;

	ffi_call(&cif, FFI_FN(printf), &res, args);
	// { dg-output "7.0" }
	printf("res: %d\n", (int) res);
	// { dg-output "\nres: 4" }

	CHECK(ffi_prep_closure_loc(pcl, &cif, cls_longdouble_va_fn, NULL, code) == FFI_OK);

	res	= ((int(*)(char*, long double))(code))(format, ldArg);
	// { dg-output "\n7.0" }
	printf("res: %d\n", (int) res);
	// { dg-output "\nres: 4" }

	exit(0);
}
