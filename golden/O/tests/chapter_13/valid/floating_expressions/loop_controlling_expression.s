SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__local_main__d_7	EQU	$80
__local_main__d_6	EQU	$81
__local_main__d_5	EQU	$82
__local_main__d_4	EQU	$83
__local_main__d_3	EQU	$84
__local_main__d_2	EQU	$85
__local_main__d_1	EQU	$86
__local_main__d_0	EQU	$87
__local_main__a_1	EQU	$88
__local_main__a_0	EQU	$89

; @zp-link-meta-begin
; def main params= locals=__local_main__d_7,__local_main__d_6,__local_main__d_5,__local_main__d_4,__local_main__d_3,__local_main__d_2,__local_main__d_1,__local_main__d_0,__local_main__a_1,__local_main__a_0 indirect=false in_cycle=false
; item main section=.text local=false zero=false
; item main@run0 section=.text local=true zero=false
; runtime hargs_size=32 hargs_need=24
; @zp-link-meta-end

main:
   SUBROUTINE

   LDA   #$00
   STA   __local_main__a_0
   STA   __local_main__a_1
   TAX
   STA   __local_main__d_0
   STA   __local_main__d_1
   STA   __local_main__d_2
   STA   __local_main__d_3
   STA   __local_main__d_4
   STA   __local_main__d_5
   LDA   #$59
   STA   __local_main__d_6
   LDA   #$40
   STA   __local_main__d_7
.loop@0_start:
   LDA   __local_main__d_0
   STA   HARGS
   LDA   __local_main__d_1
   STA   HARGS+1
   LDA   __local_main__d_2
   STA   HARGS+2
   LDA   __local_main__d_3
   STA   HARGS+3
   LDA   __local_main__d_4
   STA   HARGS+4
   LDA   __local_main__d_5
   STA   HARGS+5
   LDA   __local_main__d_6
   STA   HARGS+6
   LDA   __local_main__d_7
   JSR   main@run0
   STY   HARGS+13
   STY   HARGS+14
   STA   HARGS+15
   JSR   dle
   LDX   __local_main__a_1
   LDA   HARGS+16
   BNE   .loop@0_break
   INC   __local_main__a_0
   BNE   .inc_done@0
   INX
.inc_done@0:
   LDA   __local_main__d_0
   STA   HARGS
   LDA   __local_main__d_1
   STA   HARGS+1
   LDA   __local_main__d_2
   STA   HARGS+2
   LDA   __local_main__d_3
   STA   HARGS+3
   LDA   __local_main__d_4
   STA   HARGS+4
   LDA   __local_main__d_5
   STA   HARGS+5
   LDA   __local_main__d_6
   STA   HARGS+6
   LDA   __local_main__d_7
   JSR   main@run0
   STA   HARGS+13
   LDA   #$F0
   STA   HARGS+14
   LDA   #$3F
   STA   HARGS+15
   STX   __local_main__a_1
   JSR   dsub
   LDA   HARGS+16
   STA   __local_main__d_0
   LDA   HARGS+17
   STA   __local_main__d_1
   LDA   HARGS+18
   STA   __local_main__d_2
   LDA   HARGS+19
   STA   __local_main__d_3
   LDA   HARGS+20
   STA   __local_main__d_4
   LDA   HARGS+21
   STA   __local_main__d_5
   LDA   HARGS+22
   STA   __local_main__d_6
   LDA   HARGS+23
   STA   __local_main__d_7
   JMP   .loop@0_start
.loop@0_break:
   LDA   __local_main__a_0
   RTS

main@run0:
   SUBROUTINE

   STA   HARGS+7
   LDA   #$00
   STA   HARGS+8
   LDY   #$00
   STY   HARGS+9
   STY   HARGS+10
   STY   HARGS+11
   STY   HARGS+12
   RTS
