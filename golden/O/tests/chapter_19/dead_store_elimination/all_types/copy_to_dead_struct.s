SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__zpabi_f__arg_0	EQU	$80
__zpabi_f__arg_1	EQU	$81

; @zp-link-meta-begin
; def f params=__zpabi_f__arg_0,__zpabi_f__arg_1 locals=__local_f__0,__local_f__1,__local_f__2 indirect=false in_cycle=false changes=__zpabi_f__arg_0,__zpabi_f__arg_1
; def main params= locals=__local_main__0 indirect=false in_cycle=false
; def target params= locals=__local_target__0 indirect=false in_cycle=false
; call main -> target
; call target -> f
; item f section=.text local=false zero=false
; item target section=.text local=false zero=false
; item main section=.text local=false zero=false
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

f:
   SUBROUTINE

   LDX   __zpabi_f__arg_0+1
   LDA   __zpabi_f__arg_0
   RTS

target:
   SUBROUTINE

   LDA   #$04
   STA   __zpabi_f__arg_0
   LDA   #$00
   STA   __zpabi_f__arg_1
   JMP   f

main:
   SUBROUTINE

   JMP   target
