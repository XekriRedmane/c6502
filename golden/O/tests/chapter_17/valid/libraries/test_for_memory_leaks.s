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
__local_lots_of_args__a_1	EQU	$9C
__local_lots_of_args__a_0	EQU	$9D

; @zp-link-meta-begin
; def lots_of_args params=__zpabi_lots_of_args__a_0,__zpabi_lots_of_args__a_1,__zpabi_lots_of_args__b_0,__zpabi_lots_of_args__b_1,__zpabi_lots_of_args__c_0,__zpabi_lots_of_args__c_1,__zpabi_lots_of_args__d_0,__zpabi_lots_of_args__d_1,__zpabi_lots_of_args__e_0,__zpabi_lots_of_args__e_1,__zpabi_lots_of_args__f_0,__zpabi_lots_of_args__f_1,__zpabi_lots_of_args__g_0,__zpabi_lots_of_args__g_1,__zpabi_lots_of_args__h_0,__zpabi_lots_of_args__h_1,__zpabi_lots_of_args__i_0,__zpabi_lots_of_args__i_1,__zpabi_lots_of_args__j_0,__zpabi_lots_of_args__j_1,__zpabi_lots_of_args__k_0,__zpabi_lots_of_args__k_1,__zpabi_lots_of_args__l_0,__zpabi_lots_of_args__l_1,__zpabi_lots_of_args__m_0,__zpabi_lots_of_args__m_1,__zpabi_lots_of_args__n_0,__zpabi_lots_of_args__n_1,__zpabi_lots_of_args__o_0,__zpabi_lots_of_args__o_1 locals=__local_lots_of_args__a_1,__local_lots_of_args__a_0 indirect=false in_cycle=false param_regs=A,X,,,,,,,,,,,,,,,,,,,,,,,,,,,,
; ext exit params=__zpabi_exit__status_0,__zpabi_exit__status_1 param_regs=A,X
; call lots_of_args -> exit
; item lots_of_args section=.text local=false zero=false
; item sum section=.bss local=false zero=true
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

lots_of_args:
   SUBROUTINE

   STA   __local_lots_of_args__a_0
   TXA
   BNE   .jcmp_skip@0
   LDA   __local_lots_of_args__a_0
   CMP   #$01
   BEQ   .if_end@0
.jcmp_skip@0:
   LDX   #$00
   LDA   #$01
   JSR   exit
.if_end@0:
   LDA   __zpabi_lots_of_args__b_1
   BNE   .jcmp_skip@1
   LDA   __zpabi_lots_of_args__b_0
   CMP   #$02
   BEQ   .if_end@1
.jcmp_skip@1:
   LDX   #$00
   LDA   #$02
   JSR   exit
.if_end@1:
   LDA   __zpabi_lots_of_args__c_1
   BNE   .jcmp_skip@2
   LDA   __zpabi_lots_of_args__c_0
   CMP   #$03
   BEQ   .if_end@2
.jcmp_skip@2:
   LDX   #$00
   LDA   #$03
   JSR   exit
.if_end@2:
   LDA   __zpabi_lots_of_args__d_1
   BNE   .jcmp_skip@3
   LDA   __zpabi_lots_of_args__d_0
   CMP   #$04
   BEQ   .if_end@3
.jcmp_skip@3:
   LDX   #$00
   LDA   #$04
   JSR   exit
.if_end@3:
   LDA   __zpabi_lots_of_args__e_1
   BNE   .jcmp_skip@4
   LDA   __zpabi_lots_of_args__e_0
   CMP   #$05
   BEQ   .if_end@4
.jcmp_skip@4:
   LDX   #$00
   LDA   #$05
   JSR   exit
.if_end@4:
   LDA   __zpabi_lots_of_args__f_1
   BNE   .jcmp_skip@5
   LDA   __zpabi_lots_of_args__f_0
   CMP   #$06
   BEQ   .if_end@5
.jcmp_skip@5:
   LDX   #$00
   LDA   #$06
   JSR   exit
.if_end@5:
   LDA   __zpabi_lots_of_args__g_1
   BNE   .jcmp_skip@6
   LDA   __zpabi_lots_of_args__g_0
   CMP   #$07
   BEQ   .if_end@6
.jcmp_skip@6:
   LDX   #$00
   LDA   #$07
   JSR   exit
.if_end@6:
   LDA   __zpabi_lots_of_args__h_1
   BNE   .jcmp_skip@7
   LDA   __zpabi_lots_of_args__h_0
   CMP   #$08
   BEQ   .if_end@7
.jcmp_skip@7:
   LDX   #$00
   LDA   #$08
   JSR   exit
.if_end@7:
   LDA   __zpabi_lots_of_args__i_1
   BNE   .jcmp_skip@8
   LDA   __zpabi_lots_of_args__i_0
   CMP   #$09
   BEQ   .if_end@8
.jcmp_skip@8:
   LDX   #$00
   LDA   #$09
   JSR   exit
.if_end@8:
   LDA   __zpabi_lots_of_args__j_1
   BNE   .jcmp_skip@9
   LDA   __zpabi_lots_of_args__j_0
   CMP   #$0A
   BEQ   .if_end@9
.jcmp_skip@9:
   LDX   #$00
   LDA   #$0A
   JSR   exit
.if_end@9:
   LDA   __zpabi_lots_of_args__k_1
   BNE   .jcmp_skip@10
   LDA   __zpabi_lots_of_args__k_0
   CMP   #$0B
   BEQ   .if_end@10
.jcmp_skip@10:
   LDX   #$00
   LDA   #$0B
   JSR   exit
.if_end@10:
   LDA   __zpabi_lots_of_args__l_1
   BNE   .jcmp_skip@11
   LDA   __zpabi_lots_of_args__l_0
   CMP   #$0C
   BEQ   .if_end@11
.jcmp_skip@11:
   LDX   #$00
   LDA   #$0C
   JSR   exit
.if_end@11:
   LDA   __zpabi_lots_of_args__m_1
   BNE   .jcmp_skip@12
   LDA   __zpabi_lots_of_args__m_0
   CMP   #$0D
   BEQ   .if_end@12
.jcmp_skip@12:
   LDX   #$00
   LDA   #$0D
   JSR   exit
.if_end@12:
   LDA   __zpabi_lots_of_args__n_1
   BNE   .jcmp_skip@13
   LDA   __zpabi_lots_of_args__n_0
   CMP   #$0E
   BEQ   .if_end@13
.jcmp_skip@13:
   LDX   #$00
   LDA   #$0E
   JSR   exit
.if_end@13:
   LDA   __zpabi_lots_of_args__o_1
   BMI   .sx_neg@14
   LDA   #$00
   BEQ   .sx_done@15
.sx_neg@14:
   LDA   #$FF
.sx_done@15:
   STA   __local_lots_of_args__a_0
   STA   __local_lots_of_args__a_1
   LDA   sum
   CLC
   ADC   __zpabi_lots_of_args__o_0
   STA   sum
   LDA   sum+1
   ADC   __zpabi_lots_of_args__o_1
   STA   sum+1
   LDA   sum+2
   ADC   __local_lots_of_args__a_0
   STA   sum+2
   LDA   sum+3
   ADC   __local_lots_of_args__a_1
   STA   sum+3
   RTS

sum:
   DS.B  4
