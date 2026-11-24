SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__local_target__y_1	EQU	$81
__local_target__y_0	EQU	$82
__local_target__x_1	EQU	$83
__local_target__x_0	EQU	$84

; @zp-link-meta-begin
; def main params= locals=__local_main__0 indirect=false in_cycle=false
; def target params= locals=__local_target__y_1,__local_target__y_0,__local_target__x_1,__local_target__x_0 indirect=false in_cycle=false
; ext putchar params=__zpabi_putchar__c_0,__zpabi_putchar__c_1 param_regs=A,X
; call main -> target
; call target -> putchar
; item target section=.text local=false zero=false
; item main section=.text local=false zero=false
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

target:
   SUBROUTINE

   LDA   #$41
   STA   __local_target__y_0
   LDY   #$00
   STY   __local_target__y_1
.loop@0_start:
   LDA   __local_target__y_0
   CLC
   ADC   #$02
   STA   __local_target__x_0
   LDA   __local_target__y_1
   ADC   #$00
   STA   __local_target__x_1
   LDA   #$46
   SEC
   SBC   __local_target__y_0
   LDA   #$00
   SBC   __local_target__y_1
   BVC   .jcmp_novf@0
   EOR   #$80
.jcmp_novf@0:
   BPL   .if_end@0
   LDA   __local_target__y_0
   CLC
   ADC   #$03
   STA   __local_target__x_0
   LDA   __local_target__y_1
   ADC   #$00
   STA   __local_target__x_1
.if_end@0:
   LDX   __local_target__x_1
   LDA   __local_target__x_0
   JSR   putchar
   STX   __local_target__y_1
   CLC
   ADC   #$03
   STA   __local_target__y_0
   BCC   .carry_inc@0
   INC   __local_target__y_1
.carry_inc@0:
   SEC
   SBC   #$5A
   LDA   __local_target__y_1
   SBC   #$00
   BVC   .jcmp_novf@1
   EOR   #$80
.jcmp_novf@1:
   BMI   .loop@0_start
   LDA   __local_target__x_1
   BNE   .jcmp_skip@2
   LDA   __local_target__x_0
   CMP   #$5A
   BEQ   .if_end@1
.jcmp_skip@2:
   LDX   #$00
   LDA   #$01
   RTS
.if_end@1:
   LDA   __local_target__y_1
   BNE   .jcmp_skip@3
   LDA   __local_target__y_0
   CMP   #$5D
   BEQ   .if_end@2
.jcmp_skip@3:
   LDX   #$00
   LDA   #$02
   RTS
.if_end@2:
   LDX   #$00
   TXA
   RTS

main:
   SUBROUTINE

   JMP   target
