SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__zpabi_f__flag_0	EQU	$80
__zpabi_f__flag_1	EQU	$81
__local_main__1	EQU	$83
__local_f__src_1	EQU	$85
__local_f__0	EQU	$86

; @zp-link-meta-begin
; def f params=__zpabi_f__src_0,__zpabi_f__src_1,__zpabi_f__flag_0,__zpabi_f__flag_1 locals=__local_f__src_0,__local_f__src_1,__local_f__0 indirect=false in_cycle=false param_regs=A,X,,
; def main params= locals=__local_main__0,__local_main__1 indirect=false in_cycle=false
; ext putchar params=__zpabi_putchar__c_0,__zpabi_putchar__c_1 param_regs=A,X
; call f -> putchar
; call main -> f
; item f section=.text local=false zero=false
; item main section=.text local=false zero=false
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

f:
   SUBROUTINE

   STA   __local_f__0
   STX   __local_f__src_1
   LDA   __zpabi_f__flag_0
   ORA   __zpabi_f__flag_1
   BEQ   .f@asm_ssa_split@0
   LDA   #$41
   LDX   #$00
.if_end@0:
   JSR   putchar
   LDX   __local_f__src_1
   LDA   __local_f__0
   RTS
.f@asm_ssa_split@0:
   LDA   __local_f__0
   JMP   .if_end@0

main:
   SUBROUTINE

   LDA   #$00
   STA   __zpabi_f__flag_0
   STA   __zpabi_f__flag_1
   LDX   #$00
   LDA   #$44
   JSR   f
   STA   __local_main__1
   TXA
   BNE   .jcmp_skip@0
   LDA   __local_main__1
   CMP   #$44
   BEQ   .if_end@1
.jcmp_skip@0:
   LDX   #$00
   LDA   #$01
   RTS
.if_end@1:
   LDA   #$01
   STA   __zpabi_f__flag_0
   LDA   #$00
   STA   __zpabi_f__flag_1
   LDX   #$00
   LDA   #$46
   JSR   f
   STA   __local_main__1
   TXA
   BNE   .jcmp_skip@1
   LDA   __local_main__1
   CMP   #$46
   BEQ   .if_end@2
.jcmp_skip@1:
   LDX   #$00
   LDA   #$02
   RTS
.if_end@2:
   LDX   #$00
   TXA
   RTS
