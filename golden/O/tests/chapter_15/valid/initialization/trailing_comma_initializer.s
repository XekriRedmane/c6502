SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__zpabi_foo__b_0	EQU	$80
__zpabi_foo__b_1	EQU	$81
__zpabi_foo__c_0	EQU	$82
__zpabi_foo__c_1	EQU	$83

; @zp-link-meta-begin
; def main params= locals=__local_main__arr_4 indirect=false in_cycle=false
; ext foo params=__zpabi_foo__a_0,__zpabi_foo__a_1,__zpabi_foo__b_0,__zpabi_foo__b_1,__zpabi_foo__c_0,__zpabi_foo__c_1 param_regs=A,X,,,,
; item main section=.text local=false zero=false
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

main:
   SUBROUTINE

   LDA   #$03
   LDX   #$00
   RTS
