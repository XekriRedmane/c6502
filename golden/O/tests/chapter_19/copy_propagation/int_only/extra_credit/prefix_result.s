SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__zpabi_callee__b_0	EQU	$80
__zpabi_callee__b_1	EQU	$81
__local_main__1	EQU	$83
__local_target__0	EQU	$84
__local_target__2	EQU	$86
__local_target__3	EQU	$87
__local_callee__a_0	EQU	$88

; @zp-link-meta-begin
; def callee params=__zpabi_callee__a_0,__zpabi_callee__a_1,__zpabi_callee__b_0,__zpabi_callee__b_1 locals=__local_callee__a_0,__local_callee__a_1 indirect=false in_cycle=false param_regs=A,X,,
; def f params= locals= indirect=false in_cycle=false
; def main params= locals=__local_main__0,__local_main__1 indirect=false in_cycle=false
; def set_globvar params= locals= indirect=false in_cycle=false
; def target params= locals=__local_target__0,__local_target__1,__local_target__2,__local_target__3 indirect=false in_cycle=false
; call main -> target
; call target -> callee
; call target -> f
; call target -> set_globvar
; share member=callee:__zpabi_callee__b_0,__zpabi_callee__b_1 holder=target live=target:callee,f,set_globvar forward=target>callee writer=target
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

   CLC
   ADC   __zpabi_callee__b_0
   STA   __local_callee__a_0
   TXA
   ADC   __zpabi_callee__b_1
   TAX
   LDA   __local_callee__a_0
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

   JSR   f
   STA   __zpabi_callee__b_0
   STX   __zpabi_callee__b_1
   INC   __zpabi_callee__b_0
   BNE   .inc_done@0
   INC   __zpabi_callee__b_1
.inc_done@0:
   JSR   set_globvar
   LDX   __zpabi_callee__b_1
   LDA   __zpabi_callee__b_0
   JSR   callee
   STA   __local_target__3
   STX   __local_target__2
   JSR   f
   STA   HARGS+2
   LDA   __zpabi_callee__b_0
   STA   HARGS
   LDA   __zpabi_callee__b_1
   STA   HARGS+1
   STX   HARGS+3
   JSR   mul16
   LDA   __local_target__3
   CLC
   ADC   HARGS+4
   STA   __local_target__0
   LDA   __local_target__2
   ADC   HARGS+5
   TAX
   LDA   __local_target__0
   RTS

main:
   SUBROUTINE

   JSR   target
   STA   __local_main__1
   TXA
   BNE   .jcmp_skip@0
   LDA   __local_main__1
   CMP   #$14
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
