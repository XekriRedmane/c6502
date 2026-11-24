SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24

; @zp-link-meta-begin
; def f params=__zpabi_f__a_0,__zpabi_f__a_1 locals= indirect=false in_cycle=false param_regs=A,X
; def main params= locals=__local_main__0 indirect=false in_cycle=false
; call main -> f
; item f section=.text local=false zero=false
; item main section=.text local=false zero=false
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

f:
   SUBROUTINE

   LDX   #$00
   LDA   #$11
   RTS

main:
   SUBROUTINE

   LDX   #$00
   LDA   #$0A
   JMP   f
