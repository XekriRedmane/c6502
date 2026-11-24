SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
g_ptr	EQU	$26
@10.struct_ptr	EQU	$28
__zpabi_test_static_local__b_0	EQU	$80
__zpabi_test_static_local_pointer__b_0	EQU	$80
__zpabi_test_static_local__b_1	EQU	$81
__zpabi_test_static_local_pointer__b_1	EQU	$81
__zpabi_malloc__size_0	EQU	$82
__zpabi_malloc__size_1	EQU	$83
__local_test_static_local__a_1	EQU	$84
__zpabi_malloc__size_2	EQU	$84
__local_test_static_local__a_0	EQU	$85
__zpabi_malloc__size_3	EQU	$85
__local_test_static_local_pointer__a_1	EQU	$87
__local_test_static_local_pointer__a_0	EQU	$88

; @zp-link-meta-begin
; def f1 params= locals= indirect=false in_cycle=false
; def f2 params= locals=__local_f2__0 indirect=false in_cycle=false
; def f3 params= locals=__local_f3__0 indirect=false in_cycle=false
; def f4 params= locals=__local_f4__0 indirect=false in_cycle=false
; def main params= locals= indirect=false in_cycle=false
; def test_global_struct params= locals= indirect=false in_cycle=false
; def test_global_struct_pointer params= locals= indirect=false in_cycle=false
; def test_static_local params=__zpabi_test_static_local__a_0,__zpabi_test_static_local__a_1,__zpabi_test_static_local__b_0,__zpabi_test_static_local__b_1 locals=__local_test_static_local__0,__local_test_static_local__1,__local_test_static_local__a_1,__local_test_static_local__a_0 indirect=false in_cycle=false param_regs=A,X,,
; def test_static_local_pointer params=__zpabi_test_static_local_pointer__a_0,__zpabi_test_static_local_pointer__a_1,__zpabi_test_static_local_pointer__b_0,__zpabi_test_static_local_pointer__b_1 locals=__local_test_static_local_pointer__0,__local_test_static_local_pointer__a_1,__local_test_static_local_pointer__a_0 indirect=false in_cycle=false param_regs=A,X,,
; ext malloc params=__zpabi_malloc__size_0,__zpabi_malloc__size_1,__zpabi_malloc__size_2,__zpabi_malloc__size_3
; ext putchar params=__zpabi_putchar__ch_0,__zpabi_putchar__ch_1 param_regs=A,X
; ext puts params=__zpabi_puts__s_0,__zpabi_puts__s_1 param_regs=A,X
; call f2 -> putchar
; call f4 -> putchar
; call main -> test_global_struct
; call main -> test_global_struct_pointer
; call main -> test_static_local
; call main -> test_static_local_pointer
; call test_global_struct -> f1
; call test_global_struct -> f2
; call test_global_struct_pointer -> f3
; call test_global_struct_pointer -> f4
; call test_global_struct_pointer -> malloc
; call test_static_local -> putchar
; call test_static_local -> puts
; call test_static_local_pointer -> malloc
; call test_static_local_pointer -> putchar
; item test_static_local section=.text local=false zero=false
; item test_static_local_pointer section=.text local=false zero=false
; item test_static_local_pointer@run0 section=.text local=true zero=false
; item f1 section=.text local=false zero=false
; item f2 section=.text local=false zero=false
; item f2@tail0 section=.text local=true zero=false
; item test_global_struct section=.text local=false zero=false
; item f3 section=.text local=false zero=false
; item f4 section=.text local=false zero=false
; item test_global_struct_pointer section=.text local=false zero=false
; item test_global_struct_pointer@run1 section=.text local=true zero=false
; item main section=.text local=false zero=false
; item main@run3 section=.text local=true zero=false
; item main@run2 section=.text local=true zero=false
; item str@0 section=.rodata local=true zero=false
; item @6.static_struct section=.bss local=true zero=true
; item g section=.bss local=false zero=true
; item __zp_init section=.text local=false zero=false
; zp g_ptr size=2 at=auto def=true init=$00,$00 weight=14
; zp @10.struct_ptr size=2 at=auto def=true init=$00,$00 weight=6 local=true
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

test_static_local:
   SUBROUTINE

   STA   __local_test_static_local__a_0
   STX   __local_test_static_local__a_1
   LDA   @6.static_struct
   ORA   @6.static_struct+1
   BNE   .if_else@3
   LDA   @6.static_struct+2
   ORA   @6.static_struct+3
   BNE   .if_else@3
   LDX   #>str@0
   LDA   #<str@0
   JSR   puts
   JMP   .if_end@2
.if_else@3:
   LDA   @6.static_struct
   LDX   @6.static_struct+1
   JSR   putchar
   LDA   @6.static_struct+2
   LDX   @6.static_struct+3
   JSR   putchar
   LDX   #$00
   LDA   #$0A
   JSR   putchar
.if_end@2:
   LDA   __local_test_static_local__a_0
   STA   @6.static_struct
   LDA   __local_test_static_local__a_1
   STA   @6.static_struct+1
   LDA   __zpabi_test_static_local__b_0
   STA   @6.static_struct+2
   LDA   __zpabi_test_static_local__b_1
   STA   @6.static_struct+3
   RTS

test_static_local_pointer:
   SUBROUTINE

   STA   __local_test_static_local_pointer__a_0
   STX   __local_test_static_local_pointer__a_1
   LDA   @10.struct_ptr
   ORA   @10.struct_ptr+1
   BNE   .if_else@5
   LDA   #$04
   STA   __zpabi_malloc__size_0
   JSR   test_static_local_pointer@run0
   STA   @10.struct_ptr
   STX   @10.struct_ptr+1
   JMP   .if_end@4
.if_else@5:
   LDY   #$01
   LDA   (@10.struct_ptr),Y
   TAX
   DEY
   LDA   (@10.struct_ptr),Y
   JSR   putchar
   LDY   #$03
   LDA   (@10.struct_ptr),Y
   TAX
   DEY
   LDA   (@10.struct_ptr),Y
   JSR   putchar
   LDX   #$00
   LDA   #$0A
   JSR   putchar
.if_end@4:
   LDA   __local_test_static_local_pointer__a_0
   LDY   #$00
   STA   (@10.struct_ptr),Y
   LDA   __local_test_static_local_pointer__a_1
   INY
   STA   (@10.struct_ptr),Y
   LDA   __zpabi_test_static_local_pointer__b_0
   INY
   STA   (@10.struct_ptr),Y
   LDA   __zpabi_test_static_local_pointer__b_1
   INY
   STA   (@10.struct_ptr),Y
   RTS

test_static_local_pointer@run0:
   SUBROUTINE

   LDA   #$00
   STA   __zpabi_malloc__size_1
   STA   __zpabi_malloc__size_2
   STA   __zpabi_malloc__size_3
   JMP   malloc

f1:
   SUBROUTINE

   INC   g
   INC   g+1
   INC   g+2
   RTS

f2:
   SUBROUTINE

   LDA   g
   LDX   #$00
   JSR   putchar
   LDA   g+1
   LDX   #$00
   JSR   putchar
   LDA   g+2
   JMP   f2@tail0

f2@tail0:
   SUBROUTINE

   LDX   #$00
   JSR   putchar
   LDX   #$00
   LDA   #$0A
   JMP   putchar

test_global_struct:
   SUBROUTINE

   LDA   #$41
   STA   g
   LDA   #$42
   STA   g+1
   LDA   #$43
   STA   g+2
   JSR   f1
   JSR   f2
   JSR   f1
   JMP   f2

f3:
   SUBROUTINE

   LDY   #$00
   LDA   (g_ptr),Y
   CLC
   ADC   #$01
   STA   (g_ptr),Y
   INY
   LDA   (g_ptr),Y
   CLC
   ADC   #$01
   STA   (g_ptr),Y
   INY
   LDA   (g_ptr),Y
   CLC
   ADC   #$01
   STA   (g_ptr),Y
   RTS

f4:
   SUBROUTINE

   LDY   #$00
   LDA   (g_ptr),Y
   LDX   #$00
   JSR   putchar
   LDY   #$01
   LDA   (g_ptr),Y
   LDX   #$00
   JSR   putchar
   LDY   #$02
   LDA   (g_ptr),Y
   JMP   f2@tail0

test_global_struct_pointer:
   SUBROUTINE

   LDA   #<g
   STA   g_ptr
   LDA   #>g
   STA   g_ptr+1
   JSR   test_global_struct_pointer@run1
   JSR   test_global_struct_pointer@run1
   LDA   #$03
   STA   __zpabi_malloc__size_0
   JSR   test_static_local_pointer@run0
   STA   g_ptr
   STX   g_ptr+1
   LDA   #$61
   LDY   #$00
   STA   (g_ptr),Y
   LDA   #$62
   INY
   STA   (g_ptr),Y
   LDA   #$63
   INY
   STA   (g_ptr),Y
   JSR   test_global_struct_pointer@run1
   JSR   f3
   JMP   f4

test_global_struct_pointer@run1:
   SUBROUTINE

   JSR   f3
   JMP   f4

main:
   SUBROUTINE

   LDA   #$6E
   STA   __zpabi_test_static_local__b_0
   JSR   main@run2
   LDA   #$6D
   JSR   test_static_local
   LDA   #$70
   STA   __zpabi_test_static_local__b_0
   JSR   main@run2
   LDA   #$6F
   JSR   test_static_local
   LDA   #$21
   STA   __zpabi_test_static_local__b_0
   JSR   main@run2
   LDA   #$21
   JSR   test_static_local
   LDA   #$78
   STA   __zpabi_test_static_local_pointer__b_0
   JSR   main@run3
   LDA   #$77
   JSR   test_static_local_pointer
   LDA   #$7A
   STA   __zpabi_test_static_local_pointer__b_0
   JSR   main@run3
   LDA   #$79
   JSR   test_static_local_pointer
   LDA   #$21
   STA   __zpabi_test_static_local_pointer__b_0
   JSR   main@run3
   LDA   #$21
   JSR   test_static_local_pointer
   JSR   test_global_struct
   JSR   test_global_struct_pointer
   LDX   #$00
   TXA
   RTS

main@run3:
   SUBROUTINE

   LDA   #$00
   STA   __zpabi_test_static_local_pointer__b_1
   LDX   #$00
   RTS

main@run2:
   SUBROUTINE

   LDA   #$00
   STA   __zpabi_test_static_local__b_1
   LDX   #$00
   RTS

str@0:
   DC.B  $7A, $65, $72, $6F, $00

@6.static_struct:
   DS.B  4

g:
   DS.B  3

__zp_init:
   SUBROUTINE

   LDA   #$00
   STA   @10.struct_ptr
   STA   @10.struct_ptr+1
   STA   g_ptr
   STA   g_ptr+1
   RTS
