SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__zpabi_target__flag_0	EQU	$80
__zpabi_target__flag_1	EQU	$81
__local_main__1	EQU	$83
__local_target__x	EQU	$84
__local_target__arg_1	EQU	$85
__local_target__arg_0	EQU	$86
__local_callee__arg_0	EQU	$87
__local_callee__arg_1	EQU	$88

; @zp-link-meta-begin
; def callee params=__zpabi_callee__arg_0,__zpabi_callee__arg_1 locals=__local_callee__arg_0,__local_callee__arg_1 indirect=false in_cycle=false param_regs=A,X
; def main params= locals=__local_main__0,__local_main__1 indirect=false in_cycle=false
; def target params=__zpabi_target__arg_0,__zpabi_target__arg_1,__zpabi_target__flag_0,__zpabi_target__flag_1 locals=__local_target__x,__local_target__arg_1,__local_target__arg_0 indirect=false in_cycle=false param_regs=A,X,,
; call main -> target
; call target -> callee
; item callee section=.text local=false zero=false
; item target section=.text local=false zero=false
; item main section=.text local=false zero=false
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

callee:
   SUBROUTINE

   STA   __local_callee__arg_0
   STX   __local_callee__arg_1
   ASL   __local_callee__arg_0
   ROL   __local_callee__arg_1
   LDX   __local_callee__arg_1
   LDA   __local_callee__arg_0
   RTS

target:
   SUBROUTINE

   STA   __local_target__arg_0
   STX   __local_target__arg_1
   CLC
   ADC   #$01
   STA   __local_target__x
   BCC   .carry_inc@0
   INX
.carry_inc@0:
   LDA   __zpabi_target__flag_0
   ORA   __zpabi_target__flag_1
   BEQ   .if_end@0
   LDA   __local_target__arg_0
   SEC
   SBC   #$01
   STA   __local_target__x
   LDA   __local_target__arg_1
   SBC   #$00
   TAX
.if_end@0:
   LDA   __local_target__x
   JMP   callee

main:
   SUBROUTINE

   LDA   #$00
   STA   __zpabi_target__flag_0
   STA   __zpabi_target__flag_1
   LDX   #$00
   LDA   #$04
   JSR   target
   STA   __local_main__1
   TXA
   BNE   .jcmp_skip@0
   LDA   __local_main__1
   CMP   #$0A
   BEQ   .if_end@1
.jcmp_skip@0:
   LDX   #$00
   LDA   #$01
   RTS
.if_end@1:
   LDA   #$01
   STA   __zpabi_target__flag_0
   LDA   #$00
   STA   __zpabi_target__flag_1
   LDX   #$00
   LDA   #$03
   JSR   target
   STA   __local_main__1
   TXA
   BNE   .jcmp_skip@1
   LDA   __local_main__1
   CMP   #$04
   BEQ   .if_end@2
.jcmp_skip@1:
   LDX   #$00
   LDA   #$02
   RTS
.if_end@2:
   LDX   #$00
   TXA
   RTS
