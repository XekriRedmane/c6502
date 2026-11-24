SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24

; @zp-link-meta-begin
; def main params= locals= indirect=false in_cycle=false
; item main section=.text local=false zero=false
; item @0.i section=.data local=true zero=false
; item @1.j section=.data local=true zero=false
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

main:
   SUBROUTINE

   LDA   @0.i
   SEC
   SBC   @1.j
   LDA   @0.i+1
   SBC   @1.j+1
   BVC   .jcmp_novf@0
   EOR   #$80
.jcmp_novf@0:
   BMI   .if_end@0
   LDX   #$00
   LDA   #$01
   RTS
.if_end@0:
   LDX   #$00
   TXA
   RTS

@0.i:
   DC.W  $0002

@1.j:
   DC.W  $0003
