SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__local_main__i	EQU	$80
__local_main__product	EQU	$81

; @zp-link-meta-begin
; def main params= locals=__local_main__i,__local_main__product indirect=false in_cycle=false
; item main section=.text local=false zero=false
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

main:
   SUBROUTINE

   LDA   #$01
   STA   __local_main__product
   LDX   #$00
   LDA   #$00
   STA   __local_main__i
   LDY   #$00
.loop@0_start:
   LDA   __local_main__i
   SEC
   SBC   #$0A
   TYA
   SBC   #$00
   BVC   .jcmp_novf@0
   EOR   #$80
.jcmp_novf@0:
   BPL   .loop@0_break
   LDA   __local_main__product
   CLC
   ADC   #$02
   STA   __local_main__product
   BCC   .carry_inc@0
   INX
.carry_inc@0:
   INC   __local_main__i
   BNE   .loop@0_start
   INY
   JMP   .loop@0_start
.loop@0_break:
   LDA   __local_main__product
   RTS
