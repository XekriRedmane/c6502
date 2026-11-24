SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24

; @zp-link-meta-begin
; def main params= locals= indirect=false in_cycle=false
; item main section=.text local=false zero=false
; runtime hargs_size=32 hargs_need=24
; @zp-link-meta-end

main:
   SUBROUTINE

   LDA   #$00
   STA   HARGS+20
   LDY   #$00
   STY   HARGS+21
   STY   HARGS+22
   STA   HARGS+23
   LDY   #$C8
   STY   HARGS+16
   LDY   #$00
   STY   HARGS+17
   STY   HARGS+18
   STA   HARGS+19
   RTS
