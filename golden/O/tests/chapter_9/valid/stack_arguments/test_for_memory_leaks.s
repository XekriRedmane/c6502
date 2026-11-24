SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__zpabi_lots_of_args__b_0	EQU	$80
__zpabi_lots_of_args__b_1	EQU	$81
__zpabi_lots_of_args__c_0	EQU	$82
__zpabi_lots_of_args__c_1	EQU	$83
__zpabi_lots_of_args__d_0	EQU	$84
__zpabi_lots_of_args__d_1	EQU	$85
__zpabi_lots_of_args__e_0	EQU	$86
__zpabi_lots_of_args__e_1	EQU	$87
__zpabi_lots_of_args__f_0	EQU	$88
__zpabi_lots_of_args__f_1	EQU	$89
__zpabi_lots_of_args__g_0	EQU	$8A
__zpabi_lots_of_args__g_1	EQU	$8B
__zpabi_lots_of_args__h_0	EQU	$8C
__zpabi_lots_of_args__h_1	EQU	$8D
__zpabi_lots_of_args__i_0	EQU	$8E
__zpabi_lots_of_args__i_1	EQU	$8F
__zpabi_lots_of_args__j_0	EQU	$90
__zpabi_lots_of_args__j_1	EQU	$91
__zpabi_lots_of_args__k_0	EQU	$92
__zpabi_lots_of_args__k_1	EQU	$93
__zpabi_lots_of_args__l_0	EQU	$94
__zpabi_lots_of_args__l_1	EQU	$95
__zpabi_lots_of_args__m_0	EQU	$96
__zpabi_lots_of_args__m_1	EQU	$97
__zpabi_lots_of_args__n_0	EQU	$98
__zpabi_lots_of_args__n_1	EQU	$99
__zpabi_lots_of_args__o_0	EQU	$9A
__zpabi_lots_of_args__o_1	EQU	$9B
__local_main__i_1	EQU	$9C
__local_main__i_0	EQU	$9D
__local_lots_of_args__0	EQU	$A0

; @zp-link-meta-begin
; def lots_of_args params=__zpabi_lots_of_args__a_0,__zpabi_lots_of_args__a_1,__zpabi_lots_of_args__b_0,__zpabi_lots_of_args__b_1,__zpabi_lots_of_args__c_0,__zpabi_lots_of_args__c_1,__zpabi_lots_of_args__d_0,__zpabi_lots_of_args__d_1,__zpabi_lots_of_args__e_0,__zpabi_lots_of_args__e_1,__zpabi_lots_of_args__f_0,__zpabi_lots_of_args__f_1,__zpabi_lots_of_args__g_0,__zpabi_lots_of_args__g_1,__zpabi_lots_of_args__h_0,__zpabi_lots_of_args__h_1,__zpabi_lots_of_args__i_0,__zpabi_lots_of_args__i_1,__zpabi_lots_of_args__j_0,__zpabi_lots_of_args__j_1,__zpabi_lots_of_args__k_0,__zpabi_lots_of_args__k_1,__zpabi_lots_of_args__l_0,__zpabi_lots_of_args__l_1,__zpabi_lots_of_args__m_0,__zpabi_lots_of_args__m_1,__zpabi_lots_of_args__n_0,__zpabi_lots_of_args__n_1,__zpabi_lots_of_args__o_0,__zpabi_lots_of_args__o_1 locals=__local_lots_of_args__0 indirect=false in_cycle=false param_regs=A,X,,,,,,,,,,,,,,,,,,,,,,,,,,,,
; def main params= locals=__local_main__i_1,__local_main__i_0,__local_main__ret_1,__local_main__ret_0 indirect=false in_cycle=false
; call main -> lots_of_args
; item lots_of_args section=.text local=false zero=false
; item main section=.text local=false zero=false
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

lots_of_args:
   SUBROUTINE

   LDA   __zpabi_lots_of_args__l_0
   CLC
   ADC   __zpabi_lots_of_args__o_0
   STA   __local_lots_of_args__0
   LDA   __zpabi_lots_of_args__l_1
   ADC   __zpabi_lots_of_args__o_1
   TAX
   LDA   __local_lots_of_args__0
   RTS

main:
   SUBROUTINE

   LDA   #$00
   STA   __zpabi_lots_of_args__l_0
   STA   __zpabi_lots_of_args__l_1
   STA   __local_main__i_0
   LDY   #$00
   STY   __local_main__i_1
.loop@0_start:
   LDA   __local_main__i_0
   SEC
   SBC   #$08
   LDA   __local_main__i_1
   SBC   #$00
   BVC   .jcmp_novf@0
   EOR   #$80
.jcmp_novf@0:
   BMI   .lb_skip@1
   JMP   .loop@0_break
.lb_skip@1:
   LDA   #$02
   STA   __zpabi_lots_of_args__b_0
   LDA   #$03
   STA   __zpabi_lots_of_args__c_0
   LDA   #$04
   STA   __zpabi_lots_of_args__d_0
   LDA   #$05
   STA   __zpabi_lots_of_args__e_0
   LDA   #$06
   STA   __zpabi_lots_of_args__f_0
   LDA   #$07
   STA   __zpabi_lots_of_args__g_0
   LDA   #$08
   STA   __zpabi_lots_of_args__h_0
   LDA   #$09
   STA   __zpabi_lots_of_args__i_0
   LDA   #$0A
   STA   __zpabi_lots_of_args__j_0
   LDA   #$0B
   STA   __zpabi_lots_of_args__k_0
   LDA   #$0D
   STA   __zpabi_lots_of_args__m_0
   LDA   #$0E
   STA   __zpabi_lots_of_args__n_0
   LDA   #$0F
   STA   __zpabi_lots_of_args__o_0
   LDA   #$00
   STA   __zpabi_lots_of_args__b_1
   STA   __zpabi_lots_of_args__c_1
   STA   __zpabi_lots_of_args__d_1
   STA   __zpabi_lots_of_args__e_1
   STA   __zpabi_lots_of_args__f_1
   STA   __zpabi_lots_of_args__g_1
   STA   __zpabi_lots_of_args__h_1
   STA   __zpabi_lots_of_args__i_1
   STA   __zpabi_lots_of_args__j_1
   STA   __zpabi_lots_of_args__k_1
   STA   __zpabi_lots_of_args__m_1
   STA   __zpabi_lots_of_args__n_1
   STA   __zpabi_lots_of_args__o_1
   LDX   #$00
   LDA   #$01
   JSR   lots_of_args
   STA   __zpabi_lots_of_args__l_0
   STX   __zpabi_lots_of_args__l_1
   INC   __local_main__i_0
   BEQ   .lb_skip@0
   JMP   .loop@0_start
.lb_skip@0:
   INC   __local_main__i_1
   JMP   .loop@0_start
.loop@0_break:
   LDA   __zpabi_lots_of_args__l_1
   BNE   .thread@0
   LDA   __zpabi_lots_of_args__l_0
   CMP   #$78
   BEQ   .cmp_true@1
.thread@0:
   LDA   #$00
   BEQ   .cmp_end@2
.cmp_true@1:
   LDA   #$01
.cmp_end@2:
   EOR   #$01
   LDX   #$00
   RTS
