SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__local_main__i_1	EQU	$80
__local_main__i_0	EQU	$81
__local_main__a	EQU	$82

; @zp-link-meta-begin
; def main params= locals=__local_main__i_1,__local_main__i_0,__local_main__a indirect=false in_cycle=false
; item main section=.text local=false zero=false
; runtime hargs_size=32 hargs_need=11
; @zp-link-meta-end

main:
   SUBROUTINE

   LDA   #$39
   STA   __local_main__a
   LDX   #$30
   LDA   #$05
   STA   __local_main__i_0
   LDA   #$00
   STA   __local_main__i_1
.loop@0_start:
   LDA   __local_main__a
   STA   HARGS
   STX   HARGS+1
   LDA   #$03
   STA   HARGS+2
   LDA   #$00
   STA   HARGS+3
   JSR   sdivmod16
   LDA   HARGS+4
   STA   __local_main__a
   LDX   HARGS+5
   LDA   __local_main__i_0
   SEC
   SBC   #$01
   STA   __local_main__i_0
   BCS   .carry_inc@0
   DEC   __local_main__i_1
.carry_inc@0:
   LDA   __local_main__i_1
   BPL   .loop@0_start
   LDA   __local_main__a
   RTS
