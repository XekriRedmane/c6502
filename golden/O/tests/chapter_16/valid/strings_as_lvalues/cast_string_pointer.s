SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24

; @zp-link-meta-begin
; def main params= locals=__local_main__0 indirect=false in_cycle=false
; item main section=.text local=false zero=false
; item str@0 section=.rodata local=true zero=false
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

main:
   SUBROUTINE

   LDA   str@0+3
   CMP   #$73
   BEQ   .if_end@1
   LDX   #$00
   LDA   #$01
   RTS
.if_end@1:
   LDX   #$00
   TXA
   RTS

str@0:
   DC.B  $54, $68, $69, $73, $20, $69, $73, $20, $61, $20, $73, $74, $72, $69, $6E, $67
   DC.B  $21, $00
