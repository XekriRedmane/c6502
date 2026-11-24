SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24

; @zp-link-meta-begin
; def main params= locals= indirect=false in_cycle=false
; item main section=.text local=false zero=false
; item x section=.data local=true zero=false
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

main:
   SUBROUTINE

   INC   x
   BNE   .inc_done@0
   INC   x+1
.inc_done@0:
   LDX   x+1
   LDA   x
   RTS

x:
   DC.W  $000A
