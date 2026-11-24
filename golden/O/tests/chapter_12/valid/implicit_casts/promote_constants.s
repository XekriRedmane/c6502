SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24

; @zp-link-meta-begin
; def main params= locals= indirect=false in_cycle=false
; item main section=.text local=false zero=false
; item negative_one section=.data local=false zero=false
; item zero section=.bss local=false zero=true
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

main:
   SUBROUTINE

   SEC
   LDA   #$00
   SBC   negative_one
   STA   negative_one
   LDA   #$00
   SBC   negative_one+1
   STA   negative_one+1
   LDA   #$00
   SBC   negative_one+2
   STA   negative_one+2
   LDA   #$00
   SBC   negative_one+3
   STA   negative_one+3
   LDA   #$00
   SBC   negative_one+4
   STA   negative_one+4
   LDA   #$00
   SBC   negative_one+5
   STA   negative_one+5
   LDA   #$00
   SBC   negative_one+6
   STA   negative_one+6
   LDA   #$00
   SBC   negative_one+7
   STA   negative_one+7
   LDA   #$00
   SEC
   SBC   negative_one
   LDA   #$00
   SBC   negative_one+1
   LDA   #$04
   SBC   negative_one+2
   LDA   #$00
   SBC   negative_one+3
   LDA   #$00
   SBC   negative_one+4
   LDA   #$00
   SBC   negative_one+5
   LDA   #$00
   SBC   negative_one+6
   LDA   #$00
   SBC   negative_one+7
   BVC   .jcmp_novf@0
   EOR   #$80
.jcmp_novf@0:
   BMI   .if_end@0
   LDX   #$00
   LDA   #$01
   RTS
.if_end@0:
   LDA   #$F6
   SEC
   SBC   zero
   LDA   #$7F
   SBC   zero+1
   LDA   #$FF
   SBC   zero+2
   LDA   #$FF
   SBC   zero+3
   LDA   #$FF
   SBC   zero+4
   LDA   #$FF
   SBC   zero+5
   LDA   #$FF
   SBC   zero+6
   LDA   #$FF
   SBC   zero+7
   BVC   .jcmp_novf@1
   EOR   #$80
.jcmp_novf@1:
   BMI   .if_end@1
   LDX   #$00
   LDA   #$02
   RTS
.if_end@1:
   LDX   #$00
   TXA
   RTS

negative_one:
   DC.L  $00000001
   DC.L  $00000000

zero:
   DS.B  8
