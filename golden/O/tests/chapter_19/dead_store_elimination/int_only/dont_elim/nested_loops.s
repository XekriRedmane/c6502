SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__zpabi_target__b_0	EQU	$80
__zpabi_target__b_1	EQU	$81
__zpabi_target__c_0	EQU	$82
__zpabi_target__c_1	EQU	$83
__zpabi_target__d_0	EQU	$84
__zpabi_target__d_1	EQU	$85
__local_target__0	EQU	$87
__local_target__a_1	EQU	$8B
__local_target__a_0	EQU	$8C

; @zp-link-meta-begin
; def main params= locals=__local_main__0 indirect=false in_cycle=false
; def target params=__zpabi_target__a_0,__zpabi_target__a_1,__zpabi_target__b_0,__zpabi_target__b_1,__zpabi_target__c_0,__zpabi_target__c_1,__zpabi_target__d_0,__zpabi_target__d_1 locals=__local_target__0,__local_target__1,__local_target__c_1,__local_target__c_0,__local_target__a_1,__local_target__a_0,__local_target__b_1,__local_target__b_0 indirect=false in_cycle=false param_regs=A,X,,,,,, changes=__zpabi_target__b_0,__zpabi_target__b_1,__zpabi_target__c_0,__zpabi_target__c_1
; ext putchar params=__zpabi_putchar__c_0,__zpabi_putchar__c_1 param_regs=A,X
; call main -> target
; call target -> putchar
; item target section=.text local=false zero=false
; item main section=.text local=false zero=false
; runtime hargs_size=32 hargs_need=11
; @zp-link-meta-end

target:
   SUBROUTINE

   STA   __local_target__a_0
   STX   __local_target__a_1
.loop@0_continue:
   LDA   __local_target__a_1
   BPL   .lb_skip@2
   JMP   .loop@0_break
.lb_skip@2:
   ORA   __local_target__a_0
   BNE   .lb_skip@1
   JMP   .loop@0_break
.lb_skip@1:
   JMP   .loop@1_continue
.target@rot@0:
   ORA   __zpabi_target__c_0
   BEQ   .loop@2_continue
   LDA   __zpabi_target__c_0
   CLC
   ADC   __zpabi_target__d_0
   STA   __local_target__0
   LDA   __zpabi_target__c_1
   ADC   __zpabi_target__d_1
   TAX
   LDA   __local_target__0
   JSR   putchar
   LDA   __zpabi_target__c_0
   BNE   .carry_inc@0
   DEC   __zpabi_target__c_1
.carry_inc@0:
   DEC   __zpabi_target__c_0
   LDA   __zpabi_target__d_0
   STA   HARGS
   LDA   __zpabi_target__d_1
   STA   HARGS+1
   LDA   #$02
   STA   HARGS+2
   LDA   #$00
   STA   HARGS+3
   JSR   sdivmod16
   LDA   HARGS+6
   ORA   HARGS+7
   BEQ   .loop@1_continue
   LDA   __zpabi_target__c_0
   SEC
   SBC   #$02
   STA   __zpabi_target__c_0
   BCS   .loop@1_continue
   DEC   __zpabi_target__c_1
.loop@1_continue:
   LDA   __zpabi_target__c_1
   BPL   .target@rot@0
   BMI   .loop@2_continue
.target@rot@1:
   ORA   __zpabi_target__b_0
   BEQ   .loop@2_break
   LDA   __zpabi_target__b_0
   SEC
   SBC   #$01
   STA   __zpabi_target__b_0
   BCS   .carry_inc@2
   DEC   __zpabi_target__b_1
.carry_inc@2:
   LDA   #$0A
   STA   __zpabi_target__c_0
   LDA   #$00
   STA   __zpabi_target__c_1
.loop@2_continue:
   LDA   __zpabi_target__b_1
   BPL   .target@rot@1
.loop@2_break:
   LDA   __local_target__a_0
   SEC
   SBC   #$01
   STA   __local_target__a_0
   BCC   .lb_skip@0
   JMP   .loop@0_continue
.lb_skip@0:
   DEC   __local_target__a_1
   JMP   .loop@0_continue
.loop@0_break:
   LDX   #$00
   TXA
   RTS

main:
   SUBROUTINE

   LDA   #$04
   STA   __zpabi_target__b_0
   LDA   #$03
   STA   __zpabi_target__c_0
   LDA   #$41
   STA   __zpabi_target__d_0
   LDA   #$00
   STA   __zpabi_target__b_1
   STA   __zpabi_target__c_1
   STA   __zpabi_target__d_1
   LDX   #$00
   LDA   #$05
   JMP   target
