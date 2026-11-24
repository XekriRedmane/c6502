SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__local_main__0	EQU	$80
__local_main__j_1	EQU	$82
__local_main__j_0	EQU	$83
__local_main__i_1	EQU	$84
__local_main__i_0	EQU	$85
__local_main__ans_1	EQU	$86
__local_main__ans_0	EQU	$87

; @zp-link-meta-begin
; def main params= locals=__local_main__0,__local_main__1,__local_main__j_1,__local_main__j_0,__local_main__i_1,__local_main__i_0,__local_main__ans_1,__local_main__ans_0 indirect=false in_cycle=false
; item main section=.text local=false zero=false
; runtime hargs_size=32 hargs_need=11
; @zp-link-meta-end

main:
   SUBROUTINE

   LDA   #$00
   STA   __local_main__ans_0
   STA   __local_main__ans_1
   STA   __local_main__i_0
   LDY   #$00
   STY   __local_main__i_1
.loop@0_start:
   LDA   __local_main__i_0
   SEC
   SBC   #$0A
   LDA   __local_main__i_1
   SBC   #$00
   BVC   .jcmp_novf@0
   EOR   #$80
.jcmp_novf@0:
   BPL   .loop@0_break
   LDA   #$00
   STA   __local_main__j_0
   LDY   #$00
   STY   __local_main__j_1
.loop@1_start:
   LDA   __local_main__j_0
   SEC
   SBC   #$0A
   LDA   __local_main__j_1
   SBC   #$00
   BVC   .jcmp_novf@1
   EOR   #$80
.jcmp_novf@1:
   BPL   .loop@1_break
   LDA   __local_main__i_0
   STA   HARGS
   LDA   __local_main__i_1
   STA   HARGS+1
   LDA   #$02
   STA   HARGS+2
   LDA   #$00
   STA   HARGS+3
   JSR   sdivmod16
   LDA   HARGS+4
   STA   __local_main__0
   LDA   HARGS+5
   ASL   __local_main__0
   ROL
   CMP   __local_main__i_1
   BNE   .if_else@1
   LDA   __local_main__0
   CMP   __local_main__i_0
   BEQ   .loop@1_break
.if_else@1:
   LDA   __local_main__ans_0
   CLC
   ADC   __local_main__i_0
   STA   __local_main__ans_0
   LDA   __local_main__ans_1
   ADC   __local_main__i_1
   STA   __local_main__ans_1
   INC   __local_main__j_0
   BNE   .loop@1_start
   INC   __local_main__j_1
   JMP   .loop@1_start
.loop@1_break:
   INC   __local_main__i_0
   BNE   .loop@0_start
   INC   __local_main__i_1
   JMP   .loop@0_start
.loop@0_break:
   LDX   __local_main__ans_1
   LDA   __local_main__ans_0
   RTS
