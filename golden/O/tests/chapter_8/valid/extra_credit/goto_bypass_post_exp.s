SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__local_main__i_1	EQU	$80
__local_main__i_0	EQU	$81
__local_main__sum	EQU	$82

; @zp-link-meta-begin
; def main params= locals=__local_main__i_1,__local_main__i_0,__local_main__sum indirect=false in_cycle=false
; item main section=.text local=false zero=false
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

main:
   SUBROUTINE

   LDA   #$00
   STA   __local_main__sum
   LDX   #$00
   STA   __local_main__i_0
   STA   __local_main__i_1
.main@lbl:
   INC   __local_main__sum
   BNE   .inc_done@0
   INX
.inc_done@0:
   INC   __local_main__i_0
   BNE   .inc_done@1
   INC   __local_main__i_1
.inc_done@1:
   LDA   #$0A
   SEC
   SBC   __local_main__i_0
   LDA   #$00
   SBC   __local_main__i_1
   BVC   .jcmp_novf@0
   EOR   #$80
.jcmp_novf@0:
   BPL   .main@lbl
   LDA   __local_main__sum
   RTS
