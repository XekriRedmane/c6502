SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
@1.x	EQU	$26

; @zp-link-meta-begin
; def main params= locals=__local_main__0 indirect=false in_cycle=false
; def target params=__zpabi_target__flag_0,__zpabi_target__flag_1 locals= indirect=false in_cycle=false param_regs=A,X
; call main -> target
; item target section=.text local=false zero=false
; item main section=.text local=false zero=false
; zp @1.x size=2 at=auto def=true init= weight=49
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

target:
   SUBROUTINE

   LDX   #$00
   TXA
   RTS

main:
   SUBROUTINE

   LDX   #$00
   LDA   #$14
   JMP   target
