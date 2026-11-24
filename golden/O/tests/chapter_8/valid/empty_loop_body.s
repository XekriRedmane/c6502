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

   LDA   #$64
   STA   __local_main__i
   LDX   #$00
.loop@0_start:
   LDA   __local_main__i
   SEC
   SBC   #$05
   STA   __local_main__i
   BCS   .carry_inc@0
   DEX
   SEC
.carry_inc@0:
   SBC   #$32
   TXA
   SBC   #$00
   BVC   .jcmp_novf@0
   EOR   #$80
.jcmp_novf@0:
   BPL   .loop@0_start
   LDA   __local_main__i
   RTS
