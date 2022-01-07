dnl  mpn_copyi - from x86/pentium directory for fat binary.
dnl  Generated by configure - DO NOT EDIT.

define(OPERATION_copyi)
define(__gmpn_copyi, __gmpn_copyi_pentium)
define(__gmpn_copyic,__gmpn_copyic_pentium)
define(__gmpn_preinv_copyi,__gmpn_preinv_copyi_pentium)
define(__gmpn_copyi_cps,__gmpn_copyi_cps_pentium)

dnl  For k6 and k7 gcd_1 calling their corresponding mpn_modexact_1_odd
ifdef(`__gmpn_modexact_1_odd',,
`define(__gmpn_modexact_1_odd,__gmpn_modexact_1_odd_pentium)')

define(MUL_TOOM22_THRESHOLD,16)
define(MUL_TOOM33_THRESHOLD,90)
define(SQR_TOOM2_THRESHOLD,22)
define(SQR_TOOM3_THRESHOLD,122)

include(.././mpn/x86/pentium/copyi.asm)
