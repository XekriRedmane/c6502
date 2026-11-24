SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__local_main__0	EQU	$80

; @zp-link-meta-begin
; def main params= locals=__local_main__0 indirect=false in_cycle=false
; item main section=.text local=false zero=false
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

main:
   SUBROUTINE

   LDA   #$03
   CLC
   SBC   #$01
   LDA   #$00
   SBC   #$00
   LDA   #$00
   ROL
   STA   __local_main__0
   LDA   #$00
   BNE   .and_false@0
   LDA   #$03
   CMP   __local_main__0
   BNE   .and_false@0
   LDA   #$02
   LDX   #$00
   RTS
.and_false@0:
   LDA   #$01
   LDX   #$00
   RTS
