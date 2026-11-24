SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24

; @zp-link-meta-begin
; def return_char params= locals= indirect=false in_cycle=false
; def return_schar params= locals= indirect=false in_cycle=false
; def return_uchar params= locals= indirect=false in_cycle=false
; item return_char section=.text local=false zero=false
; item return_schar section=.text local=false zero=false
; item return_uchar section=.text local=false zero=false
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

return_char:
   SUBROUTINE

   LDA   #$F6
   RTS

return_schar:
   SUBROUTINE

   LDA   #$F6
   RTS

return_uchar:
   SUBROUTINE

   LDA   #$F6
   RTS
