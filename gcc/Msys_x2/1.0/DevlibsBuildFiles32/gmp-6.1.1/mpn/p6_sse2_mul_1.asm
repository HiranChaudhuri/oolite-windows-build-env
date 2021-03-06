dnl  mpn_mul_1 - from x86/p6/sse2 directory for fat binary.
dnl  Generated by configure - DO NOT EDIT.

define(OPERATION_mul_1)
define(__gmpn_mul_1, __gmpn_mul_1_p6_sse2)
define(__gmpn_mul_1c,__gmpn_mul_1c_p6_sse2)
define(__gmpn_preinv_mul_1,__gmpn_preinv_mul_1_p6_sse2)
define(__gmpn_mul_1_cps,__gmpn_mul_1_cps_p6_sse2)

dnl  For k6 and k7 gcd_1 calling their corresponding mpn_modexact_1_odd
ifdef(`__gmpn_modexact_1_odd',,
`define(__gmpn_modexact_1_odd,__gmpn_modexact_1_odd_p6_sse2)')

define(MUL_TOOM22_THRESHOLD,20)
define(MUL_TOOM33_THRESHOLD,77)
define(SQR_TOOM2_THRESHOLD,30)
define(SQR_TOOM3_THRESHOLD,101)
define(BMOD_1_TO_MOD_1_THRESHOLD,21)

include(.././mpn/x86/p6/sse2/mul_1.asm)

