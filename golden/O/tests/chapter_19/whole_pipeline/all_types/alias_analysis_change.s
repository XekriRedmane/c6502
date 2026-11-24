SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__local_target__y	EQU	$83
__local_foo__ptr	EQU	$86
__local_foo__1	EQU	$87

; @zp-link-meta-begin
; def foo params=__zpabi_foo__ptr_0,__zpabi_foo__ptr_1 locals=__local_foo__0,__local_foo__ptr,__local_foo__1 indirect=false in_cycle=false param_regs=A,X
; def main params= locals=__local_main__0 indirect=false in_cycle=false
; def target params= locals=__local_target__0,__local_target__1,__local_target__y,__local_target__3 indirect=false in_cycle=false
; ext putchar params=__zpabi_putchar__c_0,__zpabi_putchar__c_1 param_regs=A,X
; call foo -> putchar
; call main -> target
; call target -> foo
; item foo section=.text local=false zero=false
; item target section=.text local=false zero=false
; item main section=.text local=false zero=false
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

foo:
   SUBROUTINE

   STA   __local_foo__ptr
   STX   __local_foo__1
   LDY   #$01
   LDA   (__local_foo__ptr),Y
   TAX
   DEY
   LDA   (__local_foo__ptr),Y
   JSR   putchar
   LDX   #$00
   TXA
   RTS

target:
   SUBROUTINE

   LDA   #$41
   STA   __local_target__y
   LDA   #$00
   STA   __local_target__y+1
   LDX   #>__local_target__y
   LDA   #<__local_target__y
   JSR   foo
   LDX   #$00
   TXA
   RTS

main:
   SUBROUTINE

   JMP   target
