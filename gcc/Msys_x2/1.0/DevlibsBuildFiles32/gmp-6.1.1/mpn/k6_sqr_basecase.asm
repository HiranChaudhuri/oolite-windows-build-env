dnl  mpn_sqr_basecase - from x86/k6 directory for fat binary.
dnl  Generated by configure - DO NOT EDIT.

define(OPERATION_sqr_basecase)
define(__gmpn_sqr_basecase, __gmpn_sqr_basecase_k6)
define(__gmpn_sqr_basecasec,__gmpn_sqr_basecasec_k6)
define(__gmpn_preinv_sqr_basecase,__gmpn_preinv_sqr_basecase_k6)
define(__gmpn_sqr_basecase_cps,__gmpn_sqr_basecase_cps_k6)

dnl  For k6 and k7 gcd_1 calling their corresponding mpn_modexact_1_odd
ifdef(`__gmpn_modexact_1_odd',,
`define(__gmpn_modexact_1_odd,__gmpn_modexact_1_odd_k6)')

define(MUL_TOOM22_THRESHOLD,20)
define(MUL_TOOM33_THRESHOLD,69)
define(SQR_TOOM2_THRESHOLD,32)
define(SQR_TOOM3_THRESHOLD,97)

include(.././mpn/x86/k6/sqr_basecase.asm)

