SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24

; @zp-link-meta-begin
; def get_union_ptr params= locals= indirect=false in_cycle=false
; def main params= locals= indirect=false in_cycle=false
; item main section=.text local=false zero=false
; item get_union_ptr section=.text local=false zero=false
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

main:
   SUBROUTINE

   LDX   #$00
   LDA   #$02
   RTS

get_union_ptr:
   SUBROUTINE

   LDX   #$00
   TXA
   RTS
