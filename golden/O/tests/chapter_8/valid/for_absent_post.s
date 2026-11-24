SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__local_main__a_1	EQU	$82
__local_main__a_0	EQU	$83

; @zp-link-meta-begin
; def main params= locals=__local_main__0,__local_main__1,__local_main__a_1,__local_main__a_0 indirect=false in_cycle=false
; item main section=.text local=false zero=false
; runtime hargs_size=32 hargs_need=11
; @zp-link-meta-end

main:
   SUBROUTINE

   LDA   #$85
   STA   __local_main__a_0
   LDY   #$FF
   STY   __local_main__a_1
   JMP   .loop@0_start
.main@rot@0:
   INC   __local_main__a_0
   BNE   .loop@0_start
   INC   __local_main__a_1
.loop@0_start:
   LDA   __local_main__a_0
   STA   HARGS
   LDA   __local_main__a_1
   STA   HARGS+1
   LDA   #$05
   STA   HARGS+2
   LDA   #$00
   STA   HARGS+3
   JSR   sdivmod16
   LDA   HARGS+6
   ORA   HARGS+7
   BNE   .main@rot@0
   LDA   __local_main__a_1
   BMI   .jcmp_neg@0
   ORA   __local_main__a_0
   BNE   .or_true@0
.jcmp_neg@0:
   LDA   #$00
   LDX   #$00
   RTS
.or_true@0:
   LDA   #$01
   LDX   #$00
   RTS
