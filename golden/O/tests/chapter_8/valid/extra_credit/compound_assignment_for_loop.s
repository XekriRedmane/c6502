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

   LDA   #$FF
   STA   __local_main__i
   LDX   #$FF
.loop@0_start:
   SEC
   SBC   #$9C
   TXA
   SBC   #$FF
   BVC   .jcmp_novf@0
   EOR   #$80
.jcmp_novf@0:
   BMI   .loop@0_break
   LDA   __local_main__i
   SEC
   SBC   #$03
   STA   __local_main__i
   BCS   .loop@0_start
   DEX
   JMP   .loop@0_start
.loop@0_break:
   CPX   #$FF
   BNE   .thread@0
   LDA   __local_main__i
   CMP   #$99
   BEQ   .cmp_true@1
.thread@0:
   LDA   #$00
   BEQ   .cmp_end@2
.cmp_true@1:
   LDA   #$01
.cmp_end@2:
   LDX   #$00
   RTS
