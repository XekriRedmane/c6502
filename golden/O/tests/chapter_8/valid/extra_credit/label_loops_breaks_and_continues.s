SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__local_main__i	EQU	$80
__local_main__sum	EQU	$81

; @zp-link-meta-begin
; def main params= locals=__local_main__i,__local_main__sum indirect=false in_cycle=false
; item main section=.text local=false zero=false
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

main:
   SUBROUTINE

   LDA   #$02
   STA   __local_main__sum
   LDX   #$00
   LDA   #$00
   STA   __local_main__i
   LDY   #$00
.loop@2_start:
   LDA   __local_main__i
   SEC
   SBC   #$0A
   TYA
   SBC   #$00
   BVC   .jcmp_novf@0
   EOR   #$80
.jcmp_novf@0:
   BPL   .loop@2_break
   INC   __local_main__sum
   BNE   .inc_done@0
   INX
.inc_done@0:
   INC   __local_main__i
   BNE   .loop@2_start
   INY
   JMP   .loop@2_start
.loop@2_break:
   LDA   __local_main__sum
   RTS
