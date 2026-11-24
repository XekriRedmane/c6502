SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__zpabi_double_isnan__d_0	EQU	$80
__local_target__flag_0	EQU	$81
__zpabi_double_isnan__d_1	EQU	$81
__zpabi_double_isnan__d_2	EQU	$82
__zpabi_double_isnan__d_3	EQU	$83
__zpabi_double_isnan__d_4	EQU	$84
__zpabi_double_isnan__d_5	EQU	$85
__zpabi_double_isnan__d_6	EQU	$86
__zpabi_double_isnan__d_7	EQU	$87
__local_main__0	EQU	$88

; @zp-link-meta-begin
; def main params= locals=__local_main__0,__local_main__1 indirect=false in_cycle=false
; def target params=__zpabi_target__flag_0,__zpabi_target__flag_1 locals=__local_target__flag_1,__local_target__flag_0 indirect=false in_cycle=false param_regs=A,X
; ext double_isnan params=__zpabi_double_isnan__d_0,__zpabi_double_isnan__d_1,__zpabi_double_isnan__d_2,__zpabi_double_isnan__d_3,__zpabi_double_isnan__d_4,__zpabi_double_isnan__d_5,__zpabi_double_isnan__d_6,__zpabi_double_isnan__d_7
; call main -> double_isnan
; call main -> target
; item target section=.text local=false zero=false
; item target@run0 section=.text local=true zero=false
; item main section=.text local=false zero=false
; item na section=.bss local=false zero=true
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

target:
   SUBROUTINE

   STA   __local_target__flag_0
   JSR   target@run0
   TXA
   ORA   __local_target__flag_0
   BEQ   .if_end@0
   JSR   target@run0
.if_end@0:
   LDX   #$00
   TXA
   RTS

target@run0:
   SUBROUTINE

   LDA   #$00
   STA   na
   STA   na+1
   STA   na+2
   STA   na+3
   STA   na+4
   STA   na+5
   LDA   #$F8
   STA   na+6
   LDA   #$FF
   STA   na+7
   RTS

main:
   SUBROUTINE

   LDX   #$00
   LDA   #$01
   JSR   target
   LDA   na
   STA   __zpabi_double_isnan__d_0
   LDA   na+1
   STA   __zpabi_double_isnan__d_1
   LDA   na+2
   STA   __zpabi_double_isnan__d_2
   LDA   na+3
   STA   __zpabi_double_isnan__d_3
   LDA   na+4
   STA   __zpabi_double_isnan__d_4
   LDA   na+5
   STA   __zpabi_double_isnan__d_5
   LDA   na+6
   STA   __zpabi_double_isnan__d_6
   LDA   na+7
   STA   __zpabi_double_isnan__d_7
   JSR   double_isnan
   STX   __local_main__0
   ORA   __local_main__0
   BNE   .if_end@1
   LDX   #$00
   LDA   #$01
   RTS
.if_end@1:
   LDX   #$00
   TXA
   RTS

na:
   DS.B  8
