SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24

; @zp-link-meta-begin
; def foo params= locals= indirect=false in_cycle=false
; def main params= locals= indirect=false in_cycle=false
; ext exit params=__zpabi_exit__status_0,__zpabi_exit__status_1 param_regs=A,X
; call foo -> exit
; item foo section=.text local=false zero=false
; item main section=.text local=false zero=false
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

foo:
   SUBROUTINE

   LDX   #$00
   LDA   #$0A
   JSR   exit
   LDX   #$00
   TXA
   RTS

main:
   SUBROUTINE

   LDX   #$00
   LDA   #$02
   RTS
