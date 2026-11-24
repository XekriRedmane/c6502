SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__local_f__x	EQU	$80
__local_f__arg_1	EQU	$81
__local_f__arg_0	EQU	$82

; @zp-link-meta-begin
; def f params=__zpabi_f__arg_0,__zpabi_f__arg_1 locals=__local_f__x,__local_f__arg_1,__local_f__arg_0 indirect=false in_cycle=false param_regs=A,X
; def main params= locals= indirect=false in_cycle=false
; ext putchar params=__zpabi_putchar__c_0,__zpabi_putchar__c_1 param_regs=A,X
; call f -> putchar
; call main -> f
; item f section=.text local=false zero=false
; item main section=.text local=false zero=false
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

f:
   SUBROUTINE

   STA   __local_f__arg_0
   STX   __local_f__arg_1
   SEC
   SBC   #$0A
   TXA
   SBC   #$00
   BVC   .jcmp_novf@0
   EOR   #$80
.jcmp_novf@0:
   BPL   .f@asm_ssa_split@0
   LDA   #$4D
   STA   __local_f__x
   LDX   #$00
.if_end@0:
   LDA   __local_f__arg_0
   ORA   __local_f__arg_1
   BEQ   .if_end@1
   LDA   __local_f__x
   JSR   putchar
.if_end@1:
   LDX   #$00
   TXA
   RTS
.f@asm_ssa_split@0:
   LDA   #$4C
   STA   __local_f__x
   LDX   #$00
   BEQ   .if_end@0

main:
   SUBROUTINE

   LDX   #$00
   TXA
   JSR   f
   LDX   #$00
   LDA   #$01
   JSR   f
   LDX   #$00
   LDA   #$0B
   JSR   f
   LDX   #$00
   TXA
   RTS
