SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24

; @zp-link-meta-begin
; def main params= locals=__local_main__0,__local_main__1 indirect=false in_cycle=false
; item main section=.text local=false zero=false
; item glob section=.data local=false zero=false
; runtime hargs_size=32 hargs_need=10
; @zp-link-meta-end

main:
   SUBROUTINE

   LDA   glob
   STA   HARGS
   LDA   glob+1
   STA   HARGS+1
   LDA   glob+2
   STA   HARGS+2
   LDA   glob+3
   STA   HARGS+3
   LDA   glob+4
   STA   HARGS+4
   LDA   glob+5
   STA   HARGS+5
   LDA   glob+6
   STA   HARGS+6
   LDA   glob+7
   STA   HARGS+7
   JSR   d2i
   LDA   HARGS+9
   BNE   .jcmp_skip@0
   LDA   HARGS+8
   CMP   #$03
   BEQ   .if_end@2
.jcmp_skip@0:
   LDX   #$00
   LDA   #$03
   RTS
.if_end@2:
   LDX   #$00
   TXA
   RTS

glob:
   DC.L  $00000000
   DC.L  $40080000
