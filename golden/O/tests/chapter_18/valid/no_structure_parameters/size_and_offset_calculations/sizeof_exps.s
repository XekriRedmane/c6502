SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24

; @zp-link-meta-begin
; def get_twentybyte_ptr params= locals= indirect=false in_cycle=false
; def main params= locals= indirect=false in_cycle=false
; item get_twentybyte_ptr section=.text local=false zero=false
; item main section=.text local=false zero=false
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

get_twentybyte_ptr:
   SUBROUTINE

   LDX   #$00
   TXA
   RTS

main:
   SUBROUTINE

   LDX   #$00
   LDA   #$01
   RTS
