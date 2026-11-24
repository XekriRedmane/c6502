SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__zpabi_f__flag_0	EQU	$80
__zpabi_f__flag_1	EQU	$81
__local_main__0	EQU	$82
__local_main__1	EQU	$83
__local_f__arg_0	EQU	$84
__local_f__arg_1	EQU	$85

; @zp-link-meta-begin
; def f params=__zpabi_f__arg_0,__zpabi_f__arg_1,__zpabi_f__flag_0,__zpabi_f__flag_1 locals=__local_f__arg_0,__local_f__arg_1 indirect=false in_cycle=false param_regs=A,X,,
; def main params= locals=__local_main__0,__local_main__1 indirect=false in_cycle=false
; call main -> f
; item f section=.text local=false zero=false
; item main section=.text local=false zero=false
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

f:
   SUBROUTINE

   STA   __local_f__arg_0
   STX   __local_f__arg_1
   ASL   __local_f__arg_0
   ROL   __local_f__arg_1
   LDA   __zpabi_f__flag_0
   ORA   __zpabi_f__flag_1
   BEQ   .if_end@0
   LDX   __local_f__arg_1
   LDA   __local_f__arg_0
   RTS
.if_end@0:
   LDX   #$00
   TXA
   RTS

main:
   SUBROUTINE

   LDA   #$01
   STA   __zpabi_f__flag_0
   LDA   #$00
   STA   __zpabi_f__flag_1
   LDX   #$00
   LDA   #$14
   JSR   f
   STA   __local_main__1
   TXA
   BNE   .jcmp_skip@0
   LDA   __local_main__1
   CMP   #$28
   BEQ   .if_end@1
.jcmp_skip@0:
   LDX   #$00
   LDA   #$01
   RTS
.if_end@1:
   LDA   #$00
   STA   __zpabi_f__flag_0
   STA   __zpabi_f__flag_1
   LDX   #$00
   LDA   #$03
   JSR   f
   STX   __local_main__0
   ORA   __local_main__0
   BEQ   .if_end@2
   LDX   #$00
   LDA   #$02
   RTS
.if_end@2:
   LDX   #$00
   TXA
   RTS
