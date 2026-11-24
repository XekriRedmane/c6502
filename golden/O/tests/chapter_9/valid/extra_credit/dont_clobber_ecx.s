SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__zpabi_x__b_0	EQU	$80
__zpabi_x__b_1	EQU	$81
__zpabi_x__c_0	EQU	$82
__zpabi_x__c_1	EQU	$83
__zpabi_x__d_0	EQU	$84
__zpabi_x__d_1	EQU	$85
__zpabi_x__e_0	EQU	$86
__zpabi_x__e_1	EQU	$87
__zpabi_x__f_0	EQU	$88
__zpabi_x__f_1	EQU	$89
__local_main__1	EQU	$8B
__local_x__a_0	EQU	$8D

; @zp-link-meta-begin
; def main params= locals=__local_main__0,__local_main__1 indirect=false in_cycle=false
; def x params=__zpabi_x__a_0,__zpabi_x__a_1,__zpabi_x__b_0,__zpabi_x__b_1,__zpabi_x__c_0,__zpabi_x__c_1,__zpabi_x__d_0,__zpabi_x__d_1,__zpabi_x__e_0,__zpabi_x__e_1,__zpabi_x__f_0,__zpabi_x__f_1 locals=__local_x__a_1,__local_x__a_0 indirect=false in_cycle=false param_regs=A,X,,,,,,,,,,
; call main -> x
; item x section=.text local=false zero=false
; item main section=.text local=false zero=false
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

x:
   SUBROUTINE

   STA   __local_x__a_0
   TXA
   BNE   .and_false@0
   LDA   __local_x__a_0
   CMP   #$01
   BNE   .and_false@0
   LDA   __zpabi_x__b_1
   BNE   .and_false@0
   LDA   __zpabi_x__b_0
   CMP   #$02
   BNE   .and_false@0
   LDA   __zpabi_x__c_1
   BNE   .and_false@0
   LDA   __zpabi_x__c_0
   CMP   #$03
   BNE   .and_false@0
   LDA   __zpabi_x__d_1
   BNE   .and_false@0
   LDA   __zpabi_x__d_0
   CMP   #$04
   BNE   .and_false@0
   LDA   __zpabi_x__e_1
   BNE   .and_false@0
   LDA   __zpabi_x__e_0
   CMP   #$05
   BNE   .and_false@0
   LDA   __zpabi_x__f_1
   BNE   .and_false@0
   LDA   __zpabi_x__f_0
   CMP   #$06
   BNE   .and_false@0
   LDA   #$01
   LDX   #$00
   RTS
.and_false@0:
   LDA   #$00
   LDX   #$00
   RTS

main:
   SUBROUTINE

   LDA   #$02
   STA   __zpabi_x__b_0
   LDA   #$03
   STA   __zpabi_x__c_0
   LDA   #$04
   STA   __zpabi_x__d_0
   LDA   #$05
   STA   __zpabi_x__e_0
   LDA   #$06
   STA   __zpabi_x__f_0
   LDA   #$00
   STA   __zpabi_x__b_1
   STA   __zpabi_x__c_1
   STA   __zpabi_x__d_1
   STA   __zpabi_x__e_1
   STA   __zpabi_x__f_1
   LDX   #$00
   LDA   #$01
   JSR   x
   STA   __local_main__1
   TXA
   BNE   .thread@0
   LDA   __local_main__1
   CMP   #$01
   BEQ   .cmp_true@0
.thread@0:
   LDA   #$00
   BEQ   .cmp_end@1
.cmp_true@0:
   LDA   #$01
.cmp_end@1:
   EOR   #$01
   LDX   #$00
   RTS
