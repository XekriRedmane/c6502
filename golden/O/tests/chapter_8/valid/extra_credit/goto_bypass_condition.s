SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__local_main__i	EQU	$80

; @zp-link-meta-begin
; def main params= locals=__local_main__i indirect=false in_cycle=false
; item main section=.text local=false zero=false
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

main:
   SUBROUTINE

   LDA   #$01
   STA   __local_main__i
   LDX   #$00
.main@while_start:
   INC   __local_main__i
   BNE   .inc_done@0
   INX
.inc_done@0:
   LDA   __local_main__i
   SEC
   SBC   #$0A
   TXA
   SBC   #$00
   BVC   .jcmp_novf@0
   EOR   #$80
.jcmp_novf@0:
   BMI   .main@while_start
   LDA   __local_main__i
   RTS
