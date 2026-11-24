SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24

; @zp-link-meta-begin
; def main params= locals= indirect=false in_cycle=false
; item main section=.text local=false zero=false
; item i section=.data local=false zero=false
; item j section=.data local=false zero=false
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

main:
   SUBROUTINE

   LDA   i
   ORA   i+1
   BEQ   .if_end@0
   LDX   #$00
   LDA   #$01
   RTS
.if_end@0:
   LDA   j+7
   BNE   .jcmp_skip@0
   LDA   j+6
   BNE   .jcmp_skip@0
   LDA   j+5
   BNE   .jcmp_skip@0
   LDA   j+4
   BNE   .jcmp_skip@0
   LDA   j+3
   BNE   .jcmp_skip@0
   LDA   j+2
   CMP   #$01
   BNE   .jcmp_skip@0
   LDA   j+1
   CMP   #$E2
   BNE   .jcmp_skip@0
   LDA   j
   CMP   #$40
   BEQ   .if_end@1
.jcmp_skip@0:
   LDX   #$00
   LDA   #$02
   RTS
.if_end@1:
   LDX   #$00
   TXA
   RTS

i:
   DC.W  $0100

j:
   DC.L  $0001E240
   DC.L  $00000000
