SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__zpabi_target__b_0	EQU	$80
__zpabi_target__b_1	EQU	$81
__zpabi_callee__b_0	EQU	$82
__zpabi_callee__b_1	EQU	$83
__local_main__1	EQU	$85
__local_target__0	EQU	$86
__local_target__1	EQU	$87
__local_target__2	EQU	$88
__local_target__3	EQU	$89
__local_target__a_1	EQU	$8A
__local_target__a_0	EQU	$8B

; @zp-link-meta-begin
; def callee params=__zpabi_callee__a_0,__zpabi_callee__a_1,__zpabi_callee__b_0,__zpabi_callee__b_1 locals=__local_callee__a_1 indirect=false in_cycle=false param_regs=A,X,,
; def f params= locals= indirect=false in_cycle=false
; def main params= locals=__local_main__0,__local_main__1 indirect=false in_cycle=false
; def set_globvar params= locals= indirect=false in_cycle=false
; def target params=__zpabi_target__a_0,__zpabi_target__a_1,__zpabi_target__b_0,__zpabi_target__b_1 locals=__local_target__0,__local_target__1,__local_target__2,__local_target__3,__local_target__a_1,__local_target__a_0,__local_target__4,__local_target__5 indirect=false in_cycle=false param_regs=A,X,,
; call main -> target
; call target -> callee
; call target -> f
; call target -> set_globvar
; item callee section=.text local=false zero=false
; item f section=.text local=false zero=false
; item set_globvar section=.text local=false zero=false
; item target section=.text local=false zero=false
; item main section=.text local=false zero=false
; item globl section=.bss local=false zero=true
; runtime hargs_size=32 hargs_need=10
; @zp-link-meta-end

callee:
   SUBROUTINE

   STA   HARGS
   STX   HARGS+1
   LDA   __zpabi_callee__b_0
   STA   HARGS+2
   LDA   __zpabi_callee__b_1
   STA   HARGS+3
   JSR   mul16
   LDA   HARGS+4
   LDX   HARGS+5
   RTS

f:
   SUBROUTINE

   LDX   #$00
   LDA   #$03
   RTS

set_globvar:
   SUBROUTINE

   LDA   #$04
   STA   globl
   LDA   #$00
   STA   globl+1
   LDX   #$00
   RTS

target:
   SUBROUTINE

   STA   __local_target__a_0
   STX   __local_target__a_1
   JSR   set_globvar
   LDA   __local_target__a_0
   STA   __zpabi_callee__b_0
   LDA   __local_target__a_1
   STA   __zpabi_callee__b_1
   LDX   __local_target__a_1
   LDA   __local_target__a_0
   JSR   callee
   STA   __local_target__0
   STX   __local_target__1
   JSR   f
   STA   __local_target__3
   STX   __local_target__2
   LDA   __local_target__0
   CLC
   ADC   __local_target__a_0
   STA   __local_target__0
   LDA   __local_target__1
   ADC   __local_target__a_1
   STA   __local_target__1
   LDA   __local_target__0
   SEC
   SBC   __local_target__3
   STA   __local_target__0
   LDA   __local_target__1
   SBC   __local_target__2
   TAX
   LDA   __local_target__0
   RTS

main:
   SUBROUTINE

   LDA   #$06
   STA   __zpabi_target__b_0
   LDA   #$00
   STA   __zpabi_target__b_1
   LDX   #$00
   LDA   #$05
   JSR   target
   STA   __local_main__1
   TXA
   BNE   .jcmp_skip@0
   LDA   __local_main__1
   CMP   #$1B
   BEQ   .if_end@0
.jcmp_skip@0:
   LDX   #$00
   LDA   #$01
   RTS
.if_end@0:
   LDA   globl+1
   BNE   .jcmp_skip@1
   LDA   globl
   CMP   #$04
   BEQ   .if_end@1
.jcmp_skip@1:
   LDX   #$00
   LDA   #$02
   RTS
.if_end@1:
   LDX   #$00
   TXA
   RTS

globl:
   DS.B  2
