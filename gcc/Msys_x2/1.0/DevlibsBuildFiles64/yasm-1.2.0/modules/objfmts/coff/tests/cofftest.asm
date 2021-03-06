; test source file for assembling to COFF
; build with (under DJGPP, for example):
;    yasm -f coff cofftest.asm
;    gcc -o cofftest cofftest.c cofftest.o

; This file should test the following:
; [1] Define and export a global text-section symbol
; [2] Define and export a global data-section symbol
; [3] Define and export a global BSS-section symbol
; [4] Define a non-global text-section symbol
; [5] Define a non-global data-section symbol
; [6] Define a non-global BSS-section symbol
; [7] Define a COMMON symbol
; [8] Define a NASM local label
; [9] Reference a NASM local label
; [10] Import an external symbol
; [11] Make a PC-relative call to an external symbol
; [12] Reference a text-section symbol in the text section
; [13] Reference a data-section symbol in the text section
; [14] Reference a BSS-section symbol in the text section
; [15] Reference a text-section symbol in the data section
; [16] Reference a data-section symbol in the data section
; [17] Reference a BSS-section symbol in the data section

[BITS 32]
[GLOBAL _lrotate]	; [1]
[GLOBAL _greet]		; [1]
[GLOBAL _asmstr]	; [2]
[GLOBAL _textptr]	; [2]
[GLOBAL _selfptr]	; [2]
[GLOBAL _integer]	; [3]
[EXTERN _printf]	; [10]
[COMMON _commvar 4]	; [7]

[SECTION .text]

; prototype: long lrotate(long x, int num);
_lrotate:			; [1]
	  push ebp
	  mov ebp,esp
	  mov eax,[ebp+8]
	  mov ecx,[ebp+12]
.label	  rol eax,1		; [4] [8]
	  loop .label		; [9] [12]
	  mov esp,ebp
	  pop ebp
	  ret

; prototype: void greet(void);
_greet	  mov eax,[_integer]	; [14]
	  inc eax
	  mov [localint],eax	; [14]
	  push dword [_commvar]
	  mov eax,[localptr]	; [13]
	  push dword [eax]
	  push dword [_integer]	; [1] [14]
	  push dword _printfstr	; [13]
	  call _printf		; [11]
	  add esp,16
	  ret

[SECTION .data]

; a string
_asmstr	  db 'hello, world', 0	; [2]

; a string for Printf
_printfstr db "integer==%d, localint==%d, commvar=%d"
	  db 10, 0

; some pointers
localptr  dd localint		; [5] [17]
_textptr  dd _greet		; [15]
_selfptr  dd _selfptr		; [16]

[SECTION .bss]

; an integer
_integer  resd 1		; [3]

; a local integer
localint  resd 1		; [6]
