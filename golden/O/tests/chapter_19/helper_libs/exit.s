SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24

; @zp-link-meta-begin
; def exit_wrapper params=__zpabi_exit_wrapper__status_0,__zpabi_exit_wrapper__status_1 locals=__local_exit_wrapper__status indirect=false in_cycle=false param_regs=A,X
; ext exit params=__zpabi_exit__status_0,__zpabi_exit__status_1 param_regs=A,X
; call exit_wrapper -> exit
; item exit_wrapper section=.text local=false zero=false
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

exit_wrapper:
   SUBROUTINE

   JSR   exit
   LDX   #$00
   TXA
   RTS
