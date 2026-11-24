SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__local_main__a	EQU	$80

; @zp-link-meta-begin
; def main params= locals=__local_main__a indirect=false in_cycle=false
; item main section=.text local=false zero=false
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

main:
   SUBROUTINE

   LDA   #$00
   STA   __local_main__a
   LDX   #$00
.loop@0_continue:
   SEC
   SBC   #$05
   TXA
   SBC   #$00
   BVC   .jcmp_novf@0
   EOR   #$80
.jcmp_novf@0:
   BPL   .loop@0_break
   LDA   __local_main__a
   CLC
   ADC   #$02
   STA   __local_main__a
   BCC   .loop@0_continue
   INX
   JMP   .loop@0_continue
.loop@0_break:
   LDA   __local_main__a
   RTS
