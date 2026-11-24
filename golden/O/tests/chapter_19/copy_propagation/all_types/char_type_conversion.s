SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__zpabi_print_some_chars__c	EQU	$80
__zpabi_print_some_chars__d	EQU	$81
__local_target__c	EQU	$84
__local_callee__0	EQU	$85
__local_print_some_chars__b	EQU	$85
__local_callee__s	EQU	$86
__local_callee__c	EQU	$87

; @zp-link-meta-begin
; def callee params=__zpabi_callee__c,__zpabi_callee__s locals=__local_callee__0,__local_callee__s,__local_callee__c indirect=false in_cycle=false param_regs=A,X
; def main params= locals=__local_main__0 indirect=false in_cycle=false
; def print_some_chars params=__zpabi_print_some_chars__a,__zpabi_print_some_chars__b,__zpabi_print_some_chars__c,__zpabi_print_some_chars__d locals=__local_print_some_chars__b,__local_print_some_chars__a indirect=false in_cycle=false param_regs=A,X,,
; def target params=__zpabi_target__c,__zpabi_target__s locals=__local_target__s,__local_target__c indirect=false in_cycle=false param_regs=A,X
; ext putchar params=__zpabi_putchar__c_0,__zpabi_putchar__c_1 param_regs=A,X
; call main -> target
; call print_some_chars -> putchar
; call target -> callee
; call target -> print_some_chars
; item print_some_chars section=.text local=false zero=false
; item callee section=.text local=false zero=false
; item target section=.text local=false zero=false
; item main section=.text local=false zero=false
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

print_some_chars:
   SUBROUTINE

   STX   __local_print_some_chars__b
   LDX   #$00
   JSR   putchar
   LDX   #$00
   LDA   __local_print_some_chars__b
   JSR   putchar
   LDX   #$00
   LDA   __zpabi_print_some_chars__c
   JSR   putchar
   LDX   #$00
   LDA   __zpabi_print_some_chars__d
   JMP   putchar

callee:
   SUBROUTINE

   STA   __local_callee__c
   STX   __local_callee__s
   TXA
   BMI   .sx_neg@0
   LDA   #$00
   BEQ   .sx_done@1
.sx_neg@0:
   LDA   #$FF
.sx_done@1:
   STA   __local_callee__0
   LDA   #$00
   CMP   __local_callee__0
   BNE   .cmp_end@3
   LDA   __local_callee__c
   CMP   __local_callee__s
   BEQ   .cmp_true@2
   LDA   #$00
   BEQ   .cmp_end@3
.cmp_true@2:
   LDA   #$01
.cmp_end@3:
   LDX   #$00
   RTS

target:
   SUBROUTINE

   STA   __local_target__c
   STA   __zpabi_print_some_chars__c
   STX   __zpabi_print_some_chars__d
   LDX   #$42
   LDA   #$43
   JSR   print_some_chars
   LDX   __local_target__c
   TXA
   JMP   callee

main:
   SUBROUTINE

   LDX   #$40
   LDA   #$41
   JMP   target
