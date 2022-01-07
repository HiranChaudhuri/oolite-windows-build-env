dnl  mpn_mod_1_1p - from x86/p6/sse2 directory for fat binary.
dnl  Generated by configure - DO NOT EDIT.

define(OPERATION_mod_1_1)
define(__gmpn_mod_1_1p, __gmpn_mod_1_1p_p6_sse2)
define(__gmpn_mod_1c_1p,__gmpn_mod_1c_1p_p6_sse2)
define(__gmpn_preinv_mod_1_1p,__gmpn_preinv_mod_1_1p_p6_sse2)
define(__gmpn_mod_1_1p_cps,__gmpn_mod_1_1p_cps_p6_sse2)

dnl  For k6 and k7 gcd_1 calling their corresponding mpn_modexact_1_odd
ifdef(`__gmpn_modexact_1_odd',,
`define(__gmpn_modexact_1_odd,__gmpn_modexact_1_odd_p6_sse2)')

define(MUL_TOOM22_THRESHOLD,20)
define(MUL_TOOM33_THRESHOLD,77)
define(SQR_TOOM2_THRESHOLD,30)
define(SQR_TOOM3_THRESHOLD,101)
define(BMOD_1_TO_MOD_1_THRESHOLD,21)

include(.././mpn/x86/p6/sse2/mod_1_1.asm)
