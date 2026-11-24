SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
letter	EQU	$26

; @zp-link-meta-begin
; def decrement_letter params= locals= indirect=false in_cycle=false
; def initialize_letter params= locals= indirect=false in_cycle=false
; def main params= locals= indirect=false in_cycle=false
; ext putchar params=__zpabi_putchar__c_0,__zpabi_putchar__c_1 param_regs=A,X
; call main -> decrement_letter
; call main -> initialize_letter
; call main -> putchar
; item initialize_letter section=.text local=false zero=false
; item decrement_letter section=.text local=false zero=false
; item main section=.text local=false zero=false
; item __zp_init section=.text local=false zero=false
; zp letter size=2 at=auto def=true init=$00,$00 weight=84
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

initialize_letter:
   SUBROUTINE

   LDA   #$5A
   STA   letter
   LDA   #$00
   STA   letter+1
   RTS

decrement_letter:
   SUBROUTINE

   LDA   letter
   BNE   .carry_inc@0
   DEC   letter+1
.carry_inc@0:
   DEC   letter
   RTS

main:
   SUBROUTINE

   JSR   initialize_letter
.loop@0_start:
   LDA   letter
   SEC
   SBC   #$41
   LDA   letter+1
   SBC   #$00
   BVC   .jcmp_novf@0
   EOR   #$80
.jcmp_novf@0:
   BMI   .loop@0_break
   LDX   letter+1
   LDA   letter
   JSR   putchar
   LDA   letter
   BNE   .carry_inc@1
   DEC   letter+1
.carry_inc@1:
   DEC   letter
   JMP   .loop@0_start
.loop@0_break:
   LDA   #$41
   STA   letter
   LDA   #$00
   STA   letter+1
.loop@1_start:
   LDA   #$5A
   SEC
   SBC   letter
   LDA   #$00
   SBC   letter+1
   BVC   .jcmp_novf@1
   EOR   #$80
.jcmp_novf@1:
   BMI   .loop@1_break
   LDX   letter+1
   LDA   letter
   JSR   putchar
   INC   letter
   BNE   .loop@1_start
   INC   letter+1
   JMP   .loop@1_start
.loop@1_break:
   JSR   initialize_letter
.loop@2_start:
   LDA   letter
   SEC
   SBC   #$41
   LDA   letter+1
   SBC   #$00
   BVC   .jcmp_novf@2
   EOR   #$80
.jcmp_novf@2:
   BMI   .loop@2_break
   LDX   letter+1
   LDA   letter
   JSR   putchar
   JSR   decrement_letter
   JMP   .loop@2_start
.loop@2_break:
   LDX   #$00
   TXA
   RTS

__zp_init:
   SUBROUTINE

   LDA   #$00
   STA   letter
   STA   letter+1
   RTS
