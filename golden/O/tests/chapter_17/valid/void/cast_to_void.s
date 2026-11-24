SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24

; @zp-link-meta-begin
; def do_nothing params= locals= indirect=false in_cycle=false
; def main params= locals= indirect=false in_cycle=false
; def set_x params=__zpabi_set_x__i_0,__zpabi_set_x__i_1 locals=__local_set_x__i_1,__local_set_x__i_0 indirect=false in_cycle=false param_regs=A,X
; call main -> do_nothing
; call main -> set_x
; item set_x section=.text local=false zero=false
; item do_nothing section=.text local=false zero=false
; item main section=.text local=false zero=false
; item x section=.bss local=false zero=true
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

set_x:
   SUBROUTINE

   STA   x
   STX   x+1
   LDX   #$00
   TXA
   RTS

do_nothing:
   SUBROUTINE

   RTS

main:
   SUBROUTINE

   LDX   #$00
   LDA   #$0C
   JSR   set_x
   JSR   do_nothing
   LDX   x+1
   LDA   x
   RTS

x:
   DS.B  2
