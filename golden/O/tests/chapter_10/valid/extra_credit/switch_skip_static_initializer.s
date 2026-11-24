SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24

; @zp-link-meta-begin
; def main params= locals= indirect=false in_cycle=false
; item main section=.text local=false zero=false
; item a section=.data local=false zero=false
; item @0.x section=.data local=true zero=false
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

main:
   SUBROUTINE

   LDA   a+1
   BNE   .switch@0_break
   LDA   a
   CMP   #$01
   BEQ   .case@1
   LDA   a+1
   BNE   .switch@0_break
   LDA   a
   CMP   #$03
   BEQ   .case@2
   BNE   .switch@0_break
.case@1:
   LDA   #$00
   STA   @0.x
   STA   @0.x+1
.case@2:
   LDX   @0.x+1
   LDA   @0.x
   RTS
.switch@0_break:
   LDX   #$00
   TXA
   RTS

a:
   DC.W  $0003

@0.x:
   DC.W  $000A
