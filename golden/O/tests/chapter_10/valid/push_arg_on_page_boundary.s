SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__zpabi_foo__b_0	EQU	$80
__zpabi_foo__b_1	EQU	$81
__zpabi_foo__c_0	EQU	$82
__zpabi_foo__c_1	EQU	$83
__zpabi_foo__d_0	EQU	$84
__zpabi_foo__d_1	EQU	$85
__zpabi_foo__e_0	EQU	$86
__zpabi_foo__e_1	EQU	$87
__zpabi_foo__f_0	EQU	$88
__zpabi_foo__f_1	EQU	$89
__zpabi_foo__g_0	EQU	$8A
__zpabi_foo__g_1	EQU	$8B
__local_foo__0	EQU	$8D

; @zp-link-meta-begin
; def foo params=__zpabi_foo__a_0,__zpabi_foo__a_1,__zpabi_foo__b_0,__zpabi_foo__b_1,__zpabi_foo__c_0,__zpabi_foo__c_1,__zpabi_foo__d_0,__zpabi_foo__d_1,__zpabi_foo__e_0,__zpabi_foo__e_1,__zpabi_foo__f_0,__zpabi_foo__f_1,__zpabi_foo__g_0,__zpabi_foo__g_1 locals=__local_foo__0 indirect=false in_cycle=false param_regs=A,X,,,,,,,,,,,,
; def main params= locals=__local_main__0 indirect=false in_cycle=false
; call main -> foo
; item foo section=.text local=false zero=false
; item main section=.text local=false zero=false
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

foo:
   SUBROUTINE

   LDA   __zpabi_foo__g_0
   CLC
   ADC   #$01
   STA   __local_foo__0
   LDA   __zpabi_foo__g_1
   ADC   #$00
   TAX
   LDA   __local_foo__0
   RTS

main:
   SUBROUTINE

   LDA   #$00
   STA   __zpabi_foo__b_0
   STA   __zpabi_foo__b_1
   STA   __zpabi_foo__c_0
   STA   __zpabi_foo__c_1
   STA   __zpabi_foo__d_0
   STA   __zpabi_foo__d_1
   STA   __zpabi_foo__e_0
   STA   __zpabi_foo__e_1
   STA   __zpabi_foo__f_0
   STA   __zpabi_foo__f_1
   LDA   zed
   STA   __zpabi_foo__g_0
   LDA   zed+1
   STA   __zpabi_foo__g_1
   LDX   #$00
   TXA
   JMP   foo
