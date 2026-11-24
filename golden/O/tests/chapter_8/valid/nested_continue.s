SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__local_main__i_1	EQU	$82
__local_main__i_0	EQU	$83
__local_main__acc_1	EQU	$84
__local_main__acc_0	EQU	$85
__local_main__x_1	EQU	$86
__local_main__x_0	EQU	$87

; @zp-link-meta-begin
; def main params= locals=__local_main__0,__local_main__1,__local_main__i_1,__local_main__i_0,__local_main__acc_1,__local_main__acc_0,__local_main__x_1,__local_main__x_0 indirect=false in_cycle=false
; item main section=.text local=false zero=false
; runtime hargs_size=32 hargs_need=11
; @zp-link-meta-end

main:
   SUBROUTINE

   LDA   #$05
   STA   __local_main__x_0
   LDA   #$00
   STA   __local_main__x_1
   STA   __local_main__acc_0
   LDY   #$00
   STY   __local_main__acc_1
   LDX   __local_main__acc_1
   JMP   .loop@0_continue
.main@rot@0:
   STA   __local_main__i_1
   LDA   __local_main__x_0
   STA   __local_main__i_0
.loop@1_continue:
   LDA   #$0A
   SEC
   SBC   __local_main__i_0
   LDA   #$00
   SBC   __local_main__i_1
   BVC   .jcmp_novf@0
   EOR   #$80
.jcmp_novf@0:
   BMI   .loop@1_break
   INC   __local_main__i_0
   BNE   .inc_done@0
   INC   __local_main__i_1
.inc_done@0:
   LDA   __local_main__i_0
   STA   HARGS
   LDA   __local_main__i_1
   STA   HARGS+1
   LDA   #$02
   STA   HARGS+2
   LDA   #$00
   STA   HARGS+3
   STX   __local_main__acc_1
   JSR   sdivmod16
   LDX   __local_main__acc_1
   LDA   HARGS+6
   ORA   HARGS+7
   BNE   .loop@1_continue
   INC   __local_main__acc_0
   BNE   .loop@1_continue
   INX
   JMP   .loop@1_continue
.loop@1_break:
   LDA   __local_main__x_0
   SEC
   SBC   #$01
   STA   __local_main__x_0
   BCS   .loop@0_continue
   DEC   __local_main__x_1
.loop@0_continue:
   LDA   __local_main__x_1
   BPL   .main@rot@0
   LDA   __local_main__acc_0
   RTS
