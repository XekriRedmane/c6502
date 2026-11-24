SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24

; @zp-link-meta-begin
; def main params= locals=__local_main__0 indirect=false in_cycle=false
; item main section=.text local=false zero=false
; item x section=.data local=false zero=false
; runtime hargs_size=32 hargs_need=17
; @zp-link-meta-end

main:
   SUBROUTINE

   LDA   x
   STA   HARGS+8
   LDA   x+1
   STA   HARGS+9
   LDA   x+2
   STA   HARGS+10
   LDA   x+3
   STA   HARGS+11
   LDA   x+4
   STA   HARGS+12
   LDA   x+5
   STA   HARGS+13
   LDA   x+6
   STA   HARGS+14
   LDA   x+7
   EOR   #$80
   STA   HARGS+15
   LDA   #$00
   STA   HARGS
   LDY   #$00
   STY   HARGS+1
   STY   HARGS+2
   STY   HARGS+3
   STY   HARGS+4
   STY   HARGS+5
   STA   HARGS+6
   LDA   #$80
   STA   HARGS+7
   JSR   dlt
   LDA   HARGS+16
   LDX   #$00
   RTS

x:
   DC.L  $00000000
   DC.L  $40140000
