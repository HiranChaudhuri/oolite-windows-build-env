dnl  mpn_preinv_mod_1 - from x86/k6 directory for fat binary.
dnl  Generated by configure - DO NOT EDIT.

define(OPERATION_pre_mod_1)
define(__gmpn_preinv_mod_1, __gmpn_preinv_mod_1_k6)
define(__gmpn_preinv_mod_1c,__gmpn_preinv_mod_1c_k6)
define(__gmpn_preinv_preinv_mod_1,__gmpn_preinv_preinv_mod_1_k6)
define(__gmpn_preinv_mod_1_cps,__gmpn_preinv_mod_1_cps_k6)

dnl  For k6 and k7 gcd_1 calling their corresponding mpn_modexact_1_odd
ifdef(`__gmpn_modexact_1_odd',,
`define(__gmpn_modexact_1_odd,__gmpn_modexact_1_odd_k6)')

define(MUL_TOOM22_THRESHOLD,20)
define(MUL_TOOM33_THRESHOLD,69)
define(SQR_TOOM2_THRESHOLD,32)
define(SQR_TOOM3_THRESHOLD,97)

include(.././mpn/x86/k6/pre_mod_1.asm)

