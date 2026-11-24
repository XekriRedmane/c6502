SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__local_main__i	EQU	$80
__local_main__a	EQU	$81

; @zp-link-meta-begin
; def main params= locals=__local_main__i,__local_main__a indirect=false in_cycle=false
; item main section=.text local=false zero=false
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

main:
   SUBROUTINE

   LDA   #$00
   STA   __local_main__a
   LDX   #$00
   LDA   #$9C
   STA   __local_main__i
   LDY   #$FF
.loop@0_start:
   TYA
   BMI   .jcmp_neg@0
   ORA   __local_main__i
   BNE   .loop@0_break
.jcmp_neg@0:
   INC   __local_main__a
   BNE   .inc_done@0
   INX
.inc_done@0:
   INC   __local_main__i
   BNE   .loop@0_start
   INY
   JMP   .loop@0_start
.loop@0_break:
   LDA   __local_main__a
   RTS
