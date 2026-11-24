SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24

; @zp-link-meta-begin
; def _main params= locals= indirect=false in_cycle=false
; def main params= locals= indirect=false in_cycle=false
; def main_ params= locals= indirect=false in_cycle=false
; item main section=.text local=false zero=false
; item main_ section=.text local=false zero=false
; item _main section=.text local=false zero=false
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

main:
   SUBROUTINE

   LDX   #$00
   TXA
   RTS

main_:
   SUBROUTINE

   LDX   #$00
   TXA
   RTS

_main:
   SUBROUTINE

   LDX   #$00
   TXA
   RTS
