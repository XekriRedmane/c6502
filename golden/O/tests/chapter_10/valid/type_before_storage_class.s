SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__local_main__0	EQU	$80

; @zp-link-meta-begin
; def main params= locals=__local_main__0 indirect=false in_cycle=false
; ext foo params=
; item main section=.text local=false zero=false
; item bar section=.data local=true zero=false
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

main:
   SUBROUTINE

   LDA   #$03
   CLC
   ADC   bar
   STA   __local_main__0
   LDA   #$00
   ADC   bar+1
   TAX
   LDA   __local_main__0
   RTS

bar:
   DC.W  $0004
