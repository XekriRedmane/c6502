SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__zpabi_target__flag2_0	EQU	$80
__zpabi_target__flag2_1	EQU	$81
__zpabi_target__y_0	EQU	$82
__zpabi_target__y_1	EQU	$83
__local_target__0	EQU	$85

; @zp-link-meta-begin
; def main params= locals=__local_main__0 indirect=false in_cycle=false
; def target params=__zpabi_target__flag_0,__zpabi_target__flag_1,__zpabi_target__flag2_0,__zpabi_target__flag2_1,__zpabi_target__y_0,__zpabi_target__y_1 locals=__local_target__0 indirect=false in_cycle=false param_regs=A,X,,,,
; call main -> target
; item target section=.text local=false zero=false
; item main section=.text local=false zero=false
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

target:
   SUBROUTINE

   LDA   __zpabi_target__y_0
   CLC
   ADC   __zpabi_target__y_0
   STA   __local_target__0
   LDA   __zpabi_target__y_1
   ADC   __zpabi_target__y_1
   TAX
   LDA   __local_target__0
   RTS

main:
   SUBROUTINE

   LDA   #$01
   STA   __zpabi_target__flag2_0
   LDA   #$0A
   STA   __zpabi_target__y_0
   LDA   #$00
   STA   __zpabi_target__flag2_1
   STA   __zpabi_target__y_1
   LDX   #$00
   JMP   target
