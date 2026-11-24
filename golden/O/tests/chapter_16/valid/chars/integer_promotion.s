SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__local_complement__uc	EQU	$80
__local_negate__uc	EQU	$80
__zpabi_add_chars__c3	EQU	$80
__zpabi_add_then_div__c	EQU	$80
__local_main__1	EQU	$82
__local_add_chars__c2	EQU	$83
__local_add_then_div__0	EQU	$83
__local_add_then_div__b	EQU	$84
__local_mixed_multiply__s	EQU	$84
__local_add_then_div__1	EQU	$85
__local_add_then_div__a	EQU	$86

; @zp-link-meta-begin
; def add_chars params=__zpabi_add_chars__c1,__zpabi_add_chars__c2,__zpabi_add_chars__c3 locals=__local_add_chars__c2,__local_add_chars__c1 indirect=false in_cycle=false param_regs=A,X,
; def add_then_div params=__zpabi_add_then_div__a,__zpabi_add_then_div__b,__zpabi_add_then_div__c locals=__local_add_then_div__0,__local_add_then_div__b,__local_add_then_div__1,__local_add_then_div__a indirect=false in_cycle=false param_regs=A,X,
; def complement params=__zpabi_complement__uc locals=__local_complement__uc indirect=false in_cycle=false param_regs=A
; def decrement params=__zpabi_decrement__s locals=__local_decrement__s indirect=false in_cycle=false param_regs=A
; def main params= locals=__local_main__0,__local_main__1 indirect=false in_cycle=false
; def mixed_multiply params=__zpabi_mixed_multiply__s,__zpabi_mixed_multiply__u locals=__local_mixed_multiply__u,__local_mixed_multiply__s indirect=false in_cycle=false param_regs=A,X
; def negate params=__zpabi_negate__uc locals=__local_negate__uc indirect=false in_cycle=false param_regs=A
; call main -> add_chars
; call main -> add_then_div
; call main -> complement
; call main -> decrement
; call main -> mixed_multiply
; call main -> negate
; item add_chars section=.text local=false zero=false
; item negate section=.text local=false zero=false
; item complement section=.text local=false zero=false
; item add_then_div section=.text local=false zero=false
; item mixed_multiply section=.text local=false zero=false
; item decrement section=.text local=false zero=false
; item main section=.text local=false zero=false
; runtime hargs_size=32 hargs_need=11
; @zp-link-meta-end

add_chars:
   SUBROUTINE

   STX   __local_add_chars__c2
   CLC
   ADC   __local_add_chars__c2
   STA   __local_add_chars__c2
   LDA   #$00
   ADC   #$00
   TAX
   LDA   __local_add_chars__c2
   CLC
   ADC   __zpabi_add_chars__c3
   BCC   .carry_inc@0
   INX
.carry_inc@0:
   RTS

negate:
   SUBROUTINE

   STA   __local_negate__uc
   SEC
   LDA   #$00
   SBC   __local_negate__uc
   STA   __local_negate__uc
   LDA   #$00
   SBC   #$00
   TAX
   LDA   __local_negate__uc
   RTS

complement:
   SUBROUTINE

   EOR   #$FF
   STA   __local_complement__uc
   LDA   #$00
   EOR   #$FF
   TAX
   LDA   __local_complement__uc
   RTS

add_then_div:
   SUBROUTINE

   STA   __local_add_then_div__a
   STX   __local_add_then_div__b
   ORA   #$00
   BMI   .sx_neg@0
   LDA   #$00
   BEQ   .sx_done@1
.sx_neg@0:
   LDA   #$FF
.sx_done@1:
   STA   __local_add_then_div__1
   TXA
   BMI   .sx_neg@2
   LDA   #$00
   BEQ   .sx_done@3
.sx_neg@2:
   LDA   #$FF
.sx_done@3:
   STA   __local_add_then_div__0
   LDA   __local_add_then_div__a
   CLC
   ADC   __local_add_then_div__b
   STA   __local_add_then_div__b
   LDA   __local_add_then_div__1
   ADC   __local_add_then_div__0
   STA   __local_add_then_div__0
   LDA   __zpabi_add_then_div__c
   BMI   .sx_neg@4
   LDA   #$00
   BEQ   .sx_done@5
.sx_neg@4:
   LDA   #$FF
.sx_done@5:
   STA   HARGS+3
   LDA   __local_add_then_div__b
   STA   HARGS
   LDA   __local_add_then_div__0
   STA   HARGS+1
   LDA   __zpabi_add_then_div__c
   STA   HARGS+2
   JSR   sdivmod16
   LDA   HARGS+4
   LDX   HARGS+5
   RTS

mixed_multiply:
   SUBROUTINE

   STA   __local_mixed_multiply__s
   ORA   #$00
   BMI   .sx_neg@6
   LDA   #$00
   BEQ   .sx_done@7
.sx_neg@6:
   LDA   #$FF
.sx_done@7:
   STA   HARGS+1
   LDA   __local_mixed_multiply__s
   STA   HARGS
   STX   HARGS+2
   LDA   #$00
   STA   HARGS+3
   JSR   mul16
   LDA   HARGS+4
   LDX   HARGS+5
   RTS

decrement:
   SUBROUTINE

   SEC
   SBC   #$01
   RTS

main:
   SUBROUTINE

   LDA   #$6D
   STA   __zpabi_add_chars__c3
   LDX   #$64
   TXA
   JSR   add_chars
   CPX   #$01
   BNE   .jcmp_skip@8
   CMP   #$35
   BEQ   .if_end@0
.jcmp_skip@8:
   LDX   #$00
   LDA   #$01
   RTS
.if_end@0:
   LDA   #$01
   JSR   negate
   CPX   #$FF
   BNE   .jcmp_skip@9
   CMP   #$FF
   BEQ   .if_end@1
.jcmp_skip@9:
   LDX   #$00
   LDA   #$02
   RTS
.if_end@1:
   LDA   #$01
   JSR   complement
   CPX   #$FF
   BNE   .jcmp_skip@10
   CMP   #$FE
   BEQ   .if_end@2
.jcmp_skip@10:
   LDX   #$00
   LDA   #$03
   RTS
.if_end@2:
   LDA   #$02
   STA   __zpabi_add_then_div__c
   LDX   #$03
   LDA   #$7F
   JSR   add_then_div
   STA   __local_main__1
   TXA
   BNE   .jcmp_skip@11
   LDA   __local_main__1
   CMP   #$41
   BEQ   .if_end@3
.jcmp_skip@11:
   LDX   #$00
   LDA   #$04
   RTS
.if_end@3:
   LDX   #$FA
   LDA   #$FD
   JSR   mixed_multiply
   CPX   #$FD
   BNE   .jcmp_skip@12
   CMP   #$12
   BEQ   .if_end@4
.jcmp_skip@12:
   LDX   #$00
   LDA   #$05
   RTS
.if_end@4:
   LDA   #$80
   JSR   decrement
   CMP   #$7F
   BEQ   .if_end@6
   LDX   #$00
   LDA   #$07
   RTS
.if_end@6:
   LDX   #$00
   TXA
   RTS
