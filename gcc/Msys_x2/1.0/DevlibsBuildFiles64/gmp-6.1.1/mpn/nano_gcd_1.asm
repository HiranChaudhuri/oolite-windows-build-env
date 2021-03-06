dnl  mpn_gcd_1 - from x86_64/nano directory for fat binary.
dnl  Generated by configure - DO NOT EDIT.

define(OPERATION_gcd_1)
define(__gmpn_gcd_1, __gmpn_gcd_1_nano)
define(__gmpn_gcd_1c,__gmpn_gcd_1c_nano)
define(__gmpn_preinv_gcd_1,__gmpn_preinv_gcd_1_nano)
define(__gmpn_gcd_1_cps,__gmpn_gcd_1_cps_nano)

dnl  For k6 and k7 gcd_1 calling their corresponding mpn_modexact_1_odd
ifdef(`__gmpn_modexact_1_odd',,
`define(__gmpn_modexact_1_odd,__gmpn_modexact_1_odd_nano)')

define(MUL_TOOM22_THRESHOLD,27)
define(MUL_TOOM33_THRESHOLD,38)
define(SQR_TOOM2_THRESHOLD,52)
define(SQR_TOOM3_THRESHOLD,73)
define(BMOD_1_TO_MOD_1_THRESHOLD,22)

include(.././mpn/x86_64/nano/gcd_1.asm)

