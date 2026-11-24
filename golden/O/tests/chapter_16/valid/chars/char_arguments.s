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
__local_check_args__b	EQU	$87
__local_check_args__a	EQU	$88

; @zp-link-meta-begin
; def check_args params=__zpabi_check_args__a,__zpabi_check_args__b,__zpabi_check_args__c,__zpabi_check_args__d,__zpabi_check_args__e,__zpabi_check_args__f,__zpabi_check_args__g,__zpabi_check_args__h locals=__local_check_args__b,__local_check_args__a indirect=false in_cycle=false param_regs=A,X,,,,,,
; def main params= locals=__local_main__0 indirect=false in_cycle=false
; call main -> check_args
; item check_args section=.text local=false zero=false
; item main section=.text local=false zero=false
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

check_args:
   SUBROUTINE

   STA   __local_check_args__a
   STX   __local_check_args__b
   LDA   #$05
   CMP   __local_check_args__a
   BEQ   .if_end@0
   LDX   #$00
   LDA   #$01
   RTS
.if_end@0:
   LDA   #$F4
   CMP   __local_check_args__b
   BEQ   .if_end@1
   LDX   #$00
   LDA   #$02
   RTS
.if_end@1:
   LDA   #$75
   CMP   __zpabi_check_args__c
   BEQ   .if_end@2
   LDX   #$00
   LDA   #$03
   RTS
.if_end@2:
   LDA   #$FE
   CMP   __zpabi_check_args__d
   BEQ   .if_end@3
   LDX   #$00
   LDA   #$04
   RTS
.if_end@3:
   LDA   #$01
   CMP   __zpabi_check_args__e
   BEQ   .if_end@4
   LDX   #$00
   LDA   #$05
   RTS
.if_end@4:
   LDA   #$EC
   CMP   __zpabi_check_args__f
   BEQ   .if_end@5
   LDX   #$00
   LDA   #$06
   RTS
.if_end@5:
   LDA   #$3C
   CMP   __zpabi_check_args__g
   BEQ   .if_end@6
   LDX   #$00
   LDA   #$07
   RTS
.if_end@6:
   LDA   #$64
   CMP   __zpabi_check_args__h
   BEQ   .if_end@7
   LDX   #$00
   LDA   #$08
   RTS
.if_end@7:
   LDX   #$00
   TXA
   RTS

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
