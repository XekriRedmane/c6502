SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__local_main__a_1	EQU	$80
__local_main__a_0	EQU	$81

; @zp-link-meta-begin
; def main params= locals=__local_main__a_1,__local_main__a_0 indirect=false in_cycle=false
; item main section=.text local=false zero=false
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

main:
   SUBROUTINE

   LDA   #$00
   STA   __local_main__a_0
   STA   __local_main__a_1
.loop@0_start:
   INC   __local_main__a_0
   BNE   .inc_done@0
   INC   __local_main__a_1
.inc_done@0:
   LDA   #$03
   SEC
   SBC   __local_main__a_0
   LDA   #$00
   SBC   __local_main__a_1
   BVC   .jcmp_novf@0
   EOR   #$80
.jcmp_novf@0:
   BPL   .loop@0_start
   LDX   __local_main__a_1
   LDA   __local_main__a_0
   RTS
