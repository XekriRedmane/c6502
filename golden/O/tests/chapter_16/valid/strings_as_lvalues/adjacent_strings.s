SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24

; @zp-link-meta-begin
; def main params= locals= indirect=false in_cycle=false
; ext puts params=__zpabi_puts__s_0,__zpabi_puts__s_1 param_regs=A,X
; call main -> puts
; item main section=.text local=false zero=false
; item str@0 section=.rodata local=true zero=false
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

main:
   SUBROUTINE

   LDX   #>str@0
   LDA   #<str@0
   JSR   puts
   LDX   #$00
   TXA
   RTS

str@0:
   DC.B  $48, $65, $6C, $6C, $6F, $2C, $20, $57, $6F, $72, $6C, $64, $00
