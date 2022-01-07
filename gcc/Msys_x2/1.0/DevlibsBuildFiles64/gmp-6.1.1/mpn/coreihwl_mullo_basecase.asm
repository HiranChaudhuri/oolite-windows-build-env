dnl  mpn_mullo_basecase - from x86_64/coreihwl directory for fat binary.
dnl  Generated by configure - DO NOT EDIT.

define(OPERATION_mullo_basecase)
define(__gmpn_mullo_basecase, __gmpn_mullo_basecase_coreihwl)
define(__gmpn_mullo_basecasec,__gmpn_mullo_basecasec_coreihwl)
define(__gmpn_preinv_mullo_basecase,__gmpn_preinv_mullo_basecase_coreihwl)
define(__gmpn_mullo_basecase_cps,__gmpn_mullo_basecase_cps_coreihwl)

dnl  For k6 and k7 gcd_1 calling their corresponding mpn_modexact_1_odd
ifdef(`__gmpn_modexact_1_odd',,
`define(__gmpn_modexact_1_odd,__gmpn_modexact_1_odd_coreihwl)')

define(MUL_TOOM22_THRESHOLD,22)
define(MUL_TOOM33_THRESHOLD,74)
define(SQR_TOOM2_THRESHOLD,34)
define(SQR_TOOM3_THRESHOLD,117)
define(BMOD_1_TO_MOD_1_THRESHOLD,25)

include(.././mpn/x86_64/coreihwl/mullo_basecase.asm)
