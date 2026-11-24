SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__zpabi_check_args__c	EQU	$80
__zpabi_check_args__d	EQU	$81
__zpabi_check_args__e	EQU	$82
__zpabi_check_args__f	EQU	$83
__zpabi_check_args__g	EQU	$84
__zpabi_check_args__h	EQU	$85

; @zp-link-meta-begin
; def main params= locals=__local_main__0 indirect=false in_cycle=false
; ext check_args params=__zpabi_check_args__a,__zpabi_check_args__b,__zpabi_check_args__c,__zpabi_check_args__d,__zpabi_check_args__e,__zpabi_check_args__f,__zpabi_check_args__g,__zpabi_check_args__h param_regs=A,X,,,,,,
; call main -> check_args
; item main section=.text local=false zero=false
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

main:
   SUBROUTINE

   LDA   #$75
   STA   __zpabi_check_args__c
   LDA   #$FE
   STA   __zpabi_check_args__d
   LDA   #$01
   STA   __zpabi_check_args__e
   LDA   #$EC
   STA   __zpabi_check_args__f
   LDA   #$3C
   STA   __zpabi_check_args__g
   LDA   #$64
   STA   __zpabi_check_args__h
   LDX   #$F4
   LDA   #$05
   JMP   check_args
