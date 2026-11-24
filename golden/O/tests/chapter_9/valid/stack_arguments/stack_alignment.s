SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__zpabi_even_arguments__b_0	EQU	$80
__zpabi_odd_arguments__b_0	EQU	$80
__zpabi_even_arguments__b_1	EQU	$81
__zpabi_odd_arguments__b_1	EQU	$81
__zpabi_even_arguments__c_0	EQU	$82
__zpabi_odd_arguments__c_0	EQU	$82
__zpabi_even_arguments__c_1	EQU	$83
__zpabi_odd_arguments__c_1	EQU	$83
__zpabi_even_arguments__d_0	EQU	$84
__zpabi_odd_arguments__d_0	EQU	$84
__zpabi_even_arguments__d_1	EQU	$85
__zpabi_odd_arguments__d_1	EQU	$85
__zpabi_even_arguments__e_0	EQU	$86
__zpabi_odd_arguments__e_0	EQU	$86
__zpabi_even_arguments__e_1	EQU	$87
__zpabi_odd_arguments__e_1	EQU	$87
__zpabi_even_arguments__f_0	EQU	$88
__zpabi_odd_arguments__f_0	EQU	$88
__zpabi_even_arguments__f_1	EQU	$89
__zpabi_odd_arguments__f_1	EQU	$89
__zpabi_even_arguments__g_0	EQU	$8A
__zpabi_odd_arguments__g_0	EQU	$8A
__zpabi_even_arguments__g_1	EQU	$8B
__zpabi_odd_arguments__g_1	EQU	$8B
__zpabi_even_arguments__h_0	EQU	$8C
__zpabi_odd_arguments__h_0	EQU	$8C
__zpabi_even_arguments__h_1	EQU	$8D
__zpabi_odd_arguments__h_1	EQU	$8D
__zpabi_odd_arguments__i_0	EQU	$8E
__zpabi_odd_arguments__i_1	EQU	$8F

; @zp-link-meta-begin
; def main params= locals= indirect=false in_cycle=false
; ext even_arguments params=__zpabi_even_arguments__a_0,__zpabi_even_arguments__a_1,__zpabi_even_arguments__b_0,__zpabi_even_arguments__b_1,__zpabi_even_arguments__c_0,__zpabi_even_arguments__c_1,__zpabi_even_arguments__d_0,__zpabi_even_arguments__d_1,__zpabi_even_arguments__e_0,__zpabi_even_arguments__e_1,__zpabi_even_arguments__f_0,__zpabi_even_arguments__f_1,__zpabi_even_arguments__g_0,__zpabi_even_arguments__g_1,__zpabi_even_arguments__h_0,__zpabi_even_arguments__h_1 param_regs=A,X,,,,,,,,,,,,,,
; ext odd_arguments params=__zpabi_odd_arguments__a_0,__zpabi_odd_arguments__a_1,__zpabi_odd_arguments__b_0,__zpabi_odd_arguments__b_1,__zpabi_odd_arguments__c_0,__zpabi_odd_arguments__c_1,__zpabi_odd_arguments__d_0,__zpabi_odd_arguments__d_1,__zpabi_odd_arguments__e_0,__zpabi_odd_arguments__e_1,__zpabi_odd_arguments__f_0,__zpabi_odd_arguments__f_1,__zpabi_odd_arguments__g_0,__zpabi_odd_arguments__g_1,__zpabi_odd_arguments__h_0,__zpabi_odd_arguments__h_1,__zpabi_odd_arguments__i_0,__zpabi_odd_arguments__i_1 param_regs=A,X,,,,,,,,,,,,,,,,
; call main -> even_arguments
; call main -> odd_arguments
; item main section=.text local=false zero=false
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

main:
   SUBROUTINE

   LDA   #$02
   STA   __zpabi_even_arguments__b_0
   LDA   #$03
   STA   __zpabi_even_arguments__c_0
   LDA   #$04
   STA   __zpabi_even_arguments__d_0
   LDA   #$05
   STA   __zpabi_even_arguments__e_0
   LDA   #$06
   STA   __zpabi_even_arguments__f_0
   LDA   #$07
   STA   __zpabi_even_arguments__g_0
   LDA   #$08
   STA   __zpabi_even_arguments__h_0
   LDA   #$00
   STA   __zpabi_even_arguments__b_1
   STA   __zpabi_even_arguments__c_1
   STA   __zpabi_even_arguments__d_1
   STA   __zpabi_even_arguments__e_1
   STA   __zpabi_even_arguments__f_1
   STA   __zpabi_even_arguments__g_1
   STA   __zpabi_even_arguments__h_1
   LDX   #$00
   LDA   #$01
   JSR   even_arguments
   LDA   #$02
   STA   __zpabi_odd_arguments__b_0
   LDA   #$03
   STA   __zpabi_odd_arguments__c_0
   LDA   #$04
   STA   __zpabi_odd_arguments__d_0
   LDA   #$05
   STA   __zpabi_odd_arguments__e_0
   LDA   #$06
   STA   __zpabi_odd_arguments__f_0
   LDA   #$07
   STA   __zpabi_odd_arguments__g_0
   LDA   #$08
   STA   __zpabi_odd_arguments__h_0
   LDA   #$09
   STA   __zpabi_odd_arguments__i_0
   LDA   #$00
   STA   __zpabi_odd_arguments__b_1
   STA   __zpabi_odd_arguments__c_1
   STA   __zpabi_odd_arguments__d_1
   STA   __zpabi_odd_arguments__e_1
   STA   __zpabi_odd_arguments__f_1
   STA   __zpabi_odd_arguments__g_1
   STA   __zpabi_odd_arguments__h_1
   STA   __zpabi_odd_arguments__i_1
   LDX   #$00
   LDA   #$01
   JSR   odd_arguments
   LDX   #$00
   LDA   #$03
   RTS
