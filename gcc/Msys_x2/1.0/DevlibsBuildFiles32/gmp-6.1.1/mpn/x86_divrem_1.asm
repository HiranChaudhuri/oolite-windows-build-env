dnl  mpn_divrem_1 - from x86 directory for fat binary.
dnl  Generated by configure - DO NOT EDIT.

define(OPERATION_divrem_1)
define(__gmpn_divrem_1, __gmpn_divrem_1_x86)
define(__gmpn_divrem_1c,__gmpn_divrem_1c_x86)
define(__gmpn_preinv_divrem_1,__gmpn_preinv_divrem_1_x86)
define(__gmpn_divrem_1_cps,__gmpn_divrem_1_cps_x86)

dnl  For k6 and k7 gcd_1 calling their corresponding mpn_modexact_1_odd
ifdef(`__gmpn_modexact_1_odd',,
`define(__gmpn_modexact_1_odd,__gmpn_modexact_1_odd_x86)')


include(.././mpn/x86/divrem_1.asm)
