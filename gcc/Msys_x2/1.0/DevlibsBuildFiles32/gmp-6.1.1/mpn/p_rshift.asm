dnl  mpn_rshift - from x86/pentium directory for fat binary.
dnl  Generated by configure - DO NOT EDIT.

define(OPERATION_rshift)
define(__gmpn_rshift, __gmpn_rshift_pentium)
define(__gmpn_rshiftc,__gmpn_rshiftc_pentium)
define(__gmpn_preinv_rshift,__gmpn_preinv_rshift_pentium)
define(__gmpn_rshift_cps,__gmpn_rshift_cps_pentium)

dnl  For k6 and k7 gcd_1 calling their corresponding mpn_modexact_1_odd
ifdef(`__gmpn_modexact_1_odd',,
`define(__gmpn_modexact_1_odd,__gmpn_modexact_1_odd_pentium)')

define(MUL_TOOM22_THRESHOLD,16)
define(MUL_TOOM33_THRESHOLD,90)
define(SQR_TOOM2_THRESHOLD,22)
define(SQR_TOOM3_THRESHOLD,122)

include(.././mpn/x86/pentium/rshift.asm)
