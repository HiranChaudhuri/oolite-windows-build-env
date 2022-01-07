dnl  mpn_divrem_1 - from x86/pentium4/sse2 directory for fat binary.
dnl  Generated by configure - DO NOT EDIT.

define(OPERATION_divrem_1)
define(__gmpn_divrem_1, __gmpn_divrem_1_pentium4_sse2)
define(__gmpn_divrem_1c,__gmpn_divrem_1c_pentium4_sse2)
define(__gmpn_preinv_divrem_1,__gmpn_preinv_divrem_1_pentium4_sse2)
define(__gmpn_divrem_1_cps,__gmpn_divrem_1_cps_pentium4_sse2)

dnl  For k6 and k7 gcd_1 calling their corresponding mpn_modexact_1_odd
ifdef(`__gmpn_modexact_1_odd',,
`define(__gmpn_modexact_1_odd,__gmpn_modexact_1_odd_pentium4_sse2)')

define(MUL_TOOM22_THRESHOLD,29)
define(MUL_TOOM33_THRESHOLD,113)
define(SQR_TOOM2_THRESHOLD,44)
define(SQR_TOOM3_THRESHOLD,173)
define(BMOD_1_TO_MOD_1_THRESHOLD,20)

include(.././mpn/x86/pentium4/sse2/divrem_1.asm)
