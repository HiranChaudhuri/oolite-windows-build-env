dnl  mpn_copyd - from x86/p6 directory for fat binary.
dnl  Generated by configure - DO NOT EDIT.

define(OPERATION_copyd)
define(__gmpn_copyd, __gmpn_copyd_p6)
define(__gmpn_copydc,__gmpn_copydc_p6)
define(__gmpn_preinv_copyd,__gmpn_preinv_copyd_p6)
define(__gmpn_copyd_cps,__gmpn_copyd_cps_p6)

dnl  For k6 and k7 gcd_1 calling their corresponding mpn_modexact_1_odd
ifdef(`__gmpn_modexact_1_odd',,
`define(__gmpn_modexact_1_odd,__gmpn_modexact_1_odd_p6)')

define(MUL_TOOM22_THRESHOLD,20)
define(MUL_TOOM33_THRESHOLD,74)
define(SQR_TOOM2_THRESHOLD,30)
define(SQR_TOOM3_THRESHOLD,101)
define(BMOD_1_TO_MOD_1_THRESHOLD,21)

include(.././mpn/x86/p6/copyd.asm)

