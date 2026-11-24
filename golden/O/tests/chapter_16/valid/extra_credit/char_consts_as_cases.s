SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24

; @zp-link-meta-begin
; def main params= locals= indirect=false in_cycle=false
; item main section=.text local=false zero=false
; item @0.i section=.data local=true zero=false
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

main:
   SUBROUTINE

   LDA   @0.i+1
   BNE   .vthread@0
   LDA   @0.i
   CMP   #$64
   BEQ   .case@1
   LDA   @0.i+1
   BNE   .vthread@0
   LDA   @0.i
   CMP   #$41
   BEQ   .case@2
   LDA   @0.i+1
   BNE   .vthread@0
   LDA   @0.i
   CMP   #$42
   BEQ   .case@3
   LDA   @0.i+1
.vthread@0:
   CMP   #$07
   BNE   .default@5
   LDA   @0.i
   CMP   #$D0
   BEQ   .case@4
   BNE   .default@5
.case@1:
   LDX   #$00
   LDA   #$01
   RTS
.case@2:
   LDX   #$00
   TXA
   RTS
.case@3:
   LDX   #$00
   LDA   #$02
   RTS
.case@4:
   LDX   #$00
   LDA   #$03
   RTS
.default@5:
   LDX   #$FF
   TXA
   RTS

@0.i:
   DC.W  $0041
