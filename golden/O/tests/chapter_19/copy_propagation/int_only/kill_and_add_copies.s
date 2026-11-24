SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__zpabi_callee__b_0	EQU	$80
__zpabi_callee__b_1	EQU	$81
__local_main__1	EQU	$83
__local_target__param_1	EQU	$84
__local_target__param_0	EQU	$85
__local_callee__a_0	EQU	$87

; @zp-link-meta-begin
; def callee params=__zpabi_callee__a_0,__zpabi_callee__a_1,__zpabi_callee__b_0,__zpabi_callee__b_1 locals=__local_callee__a_0,__local_callee__a_1 indirect=false in_cycle=false param_regs=A,X,,
; def main params= locals=__local_main__0,__local_main__1 indirect=false in_cycle=false
; def set_globvar params=__zpabi_set_globvar__i_0,__zpabi_set_globvar__i_1 locals=__local_set_globvar__i_1,__local_set_globvar__i_0 indirect=false in_cycle=false param_regs=A,X
; def target params=__zpabi_target__param_0,__zpabi_target__param_1 locals=__local_target__param_1,__local_target__param_0,__local_target__0 indirect=false in_cycle=false param_regs=A,X
; call main -> target
; call target -> callee
; call target -> set_globvar
; item set_globvar section=.text local=false zero=false
; item callee section=.text local=false zero=false
; item target section=.text local=false zero=false
; item main section=.text local=false zero=false
; item globvar section=.bss local=true zero=true
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

set_globvar:
   SUBROUTINE

   STA   globvar
   STX   globvar+1
   LDX   #$00
   TXA
   RTS

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

target:
   SUBROUTINE

   STA   __local_target__param_0
   STX   __local_target__param_1
   JSR   set_globvar
   LDA   __local_target__param_0
   STA   __zpabi_callee__b_0
   LDA   __local_target__param_1
   STA   __zpabi_callee__b_1
   LDX   #$00
   LDA   #$0A
   JMP   callee

main:
   SUBROUTINE

   LDX   #$00
   LDA   #$04
   JSR   target
   STA   __local_main__1
   TXA
   BNE   .jcmp_skip@0
   LDA   __local_main__1
   CMP   #$0E
   BEQ   .if_end@0
.jcmp_skip@0:
   LDX   #$00
   LDA   #$01
   RTS
.if_end@0:
   LDA   globvar+1
   BNE   .jcmp_skip@1
   LDA   globvar
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

globvar:
   DS.B  2
