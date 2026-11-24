SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24

; @zp-link-meta-begin
; def main params= locals=__local_main__my_union indirect=false in_cycle=false
; item main section=.text local=false zero=false
; item glob section=.data local=false zero=false
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

main:
   SUBROUTINE

   LDA   glob
   LDX   glob+1
   RTS

glob:
   DC.W  $0001
   DC.W  $0002
