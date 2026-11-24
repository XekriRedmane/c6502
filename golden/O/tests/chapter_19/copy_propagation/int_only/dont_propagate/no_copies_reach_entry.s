SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__zpabi_target__flag_0	EQU	$80
__zpabi_target__flag_1	EQU	$81
__local_target__a	EQU	$83

; @zp-link-meta-begin
; def main params= locals=__local_main__0 indirect=false in_cycle=false
; def target params=__zpabi_target__a_0,__zpabi_target__a_1,__zpabi_target__flag_0,__zpabi_target__flag_1 locals=__local_target__a indirect=false in_cycle=false param_regs=A,X,,
; call main -> target
; item target section=.text local=false zero=false
; item main section=.text local=false zero=false
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

target:
   SUBROUTINE

   STA   __local_target__a
   LDA   __zpabi_target__flag_0
   ORA   __zpabi_target__flag_1
   BEQ   .if_end@0
   LDA   __local_target__a
   RTS
.if_end@0:
   LDX   #$00
   LDA   #$0A
   RTS

main:
   SUBROUTINE

   LDA   #$01
   STA   __zpabi_target__flag_0
   LDA   #$00
   STA   __zpabi_target__flag_1
   LDX   #$00
   LDA   #$04
   JMP   target
