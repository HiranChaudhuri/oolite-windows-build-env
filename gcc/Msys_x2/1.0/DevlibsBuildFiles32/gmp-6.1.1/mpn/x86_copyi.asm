dnl  mpn_copyi - from x86 directory for fat binary.
dnl  Generated by configure - DO NOT EDIT.

define(OPERATION_copyi)
define(__gmpn_copyi, __gmpn_copyi_x86)
define(__gmpn_copyic,__gmpn_copyic_x86)
define(__gmpn_preinv_copyi,__gmpn_preinv_copyi_x86)
define(__gmpn_copyi_cps,__gmpn_copyi_cps_x86)

dnl  For k6 and k7 gcd_1 calling their corresponding mpn_modexact_1_odd
ifdef(`__gmpn_modexact_1_odd',,
`define(__gmpn_modexact_1_odd,__gmpn_modexact_1_odd_x86)')


include(.././mpn/x86/copyi.asm)
