SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__zpabi_check_arguments__a_0	EQU	$80
__zpabi_check_arguments__a_1	EQU	$81
__zpabi_check_arguments__a_2	EQU	$82
__zpabi_check_arguments__a_3	EQU	$83
__zpabi_check_arguments__a_4	EQU	$84
__zpabi_check_arguments__a_5	EQU	$85
__zpabi_check_arguments__a_6	EQU	$86
__zpabi_check_arguments__a_7	EQU	$87
__zpabi_check_arguments__b_0	EQU	$88
__zpabi_check_arguments__b_1	EQU	$89
__zpabi_check_arguments__b_2	EQU	$8A
__zpabi_check_arguments__b_3	EQU	$8B
__zpabi_check_arguments__b_4	EQU	$8C
__zpabi_check_arguments__b_5	EQU	$8D
__zpabi_check_arguments__b_6	EQU	$8E
__zpabi_check_arguments__b_7	EQU	$8F
__zpabi_check_arguments__c_0	EQU	$90
__zpabi_check_arguments__c_1	EQU	$91
__zpabi_check_arguments__c_2	EQU	$92
__zpabi_check_arguments__c_3	EQU	$93
__zpabi_check_arguments__c_4	EQU	$94
__zpabi_check_arguments__c_5	EQU	$95
__zpabi_check_arguments__c_6	EQU	$96
__zpabi_check_arguments__c_7	EQU	$97
__zpabi_check_arguments__d_0	EQU	$98
__zpabi_check_arguments__d_1	EQU	$99
__zpabi_check_arguments__d_2	EQU	$9A
__zpabi_check_arguments__d_3	EQU	$9B
__zpabi_check_arguments__d_4	EQU	$9C
__zpabi_check_arguments__d_5	EQU	$9D
__zpabi_check_arguments__d_6	EQU	$9E
__zpabi_check_arguments__d_7	EQU	$9F
__zpabi_check_arguments__e_0	EQU	$A0
__zpabi_check_arguments__e_1	EQU	$A1
__zpabi_check_arguments__e_2	EQU	$A2
__zpabi_check_arguments__e_3	EQU	$A3
__zpabi_check_arguments__e_4	EQU	$A4
__zpabi_check_arguments__e_5	EQU	$A5
__zpabi_check_arguments__e_6	EQU	$A6
__zpabi_check_arguments__e_7	EQU	$A7
__zpabi_check_arguments__f_0	EQU	$A8
__zpabi_check_arguments__f_1	EQU	$A9
__zpabi_check_arguments__f_2	EQU	$AA
__zpabi_check_arguments__f_3	EQU	$AB
__zpabi_check_arguments__f_4	EQU	$AC
__zpabi_check_arguments__f_5	EQU	$AD
__zpabi_check_arguments__f_6	EQU	$AE
__zpabi_check_arguments__f_7	EQU	$AF
__zpabi_check_arguments__g_0	EQU	$B0
__zpabi_check_arguments__g_1	EQU	$B1
__zpabi_check_arguments__g_2	EQU	$B2
__zpabi_check_arguments__g_3	EQU	$B3
__zpabi_check_arguments__g_4	EQU	$B4
__zpabi_check_arguments__g_5	EQU	$B5
__zpabi_check_arguments__g_6	EQU	$B6
__zpabi_check_arguments__g_7	EQU	$B7
__zpabi_check_arguments__h_0	EQU	$B8
__zpabi_check_arguments__h_1	EQU	$B9
__zpabi_check_arguments__h_2	EQU	$BA
__zpabi_check_arguments__h_3	EQU	$BB
__zpabi_check_arguments__h_4	EQU	$BC
__zpabi_check_arguments__h_5	EQU	$BD
__zpabi_check_arguments__h_6	EQU	$BE
__zpabi_check_arguments__h_7	EQU	$BF

; @zp-link-meta-begin
; def check_arguments params=__zpabi_check_arguments__a_0,__zpabi_check_arguments__a_1,__zpabi_check_arguments__a_2,__zpabi_check_arguments__a_3,__zpabi_check_arguments__a_4,__zpabi_check_arguments__a_5,__zpabi_check_arguments__a_6,__zpabi_check_arguments__a_7,__zpabi_check_arguments__b_0,__zpabi_check_arguments__b_1,__zpabi_check_arguments__b_2,__zpabi_check_arguments__b_3,__zpabi_check_arguments__b_4,__zpabi_check_arguments__b_5,__zpabi_check_arguments__b_6,__zpabi_check_arguments__b_7,__zpabi_check_arguments__c_0,__zpabi_check_arguments__c_1,__zpabi_check_arguments__c_2,__zpabi_check_arguments__c_3,__zpabi_check_arguments__c_4,__zpabi_check_arguments__c_5,__zpabi_check_arguments__c_6,__zpabi_check_arguments__c_7,__zpabi_check_arguments__d_0,__zpabi_check_arguments__d_1,__zpabi_check_arguments__d_2,__zpabi_check_arguments__d_3,__zpabi_check_arguments__d_4,__zpabi_check_arguments__d_5,__zpabi_check_arguments__d_6,__zpabi_check_arguments__d_7,__zpabi_check_arguments__e_0,__zpabi_check_arguments__e_1,__zpabi_check_arguments__e_2,__zpabi_check_arguments__e_3,__zpabi_check_arguments__e_4,__zpabi_check_arguments__e_5,__zpabi_check_arguments__e_6,__zpabi_check_arguments__e_7,__zpabi_check_arguments__f_0,__zpabi_check_arguments__f_1,__zpabi_check_arguments__f_2,__zpabi_check_arguments__f_3,__zpabi_check_arguments__f_4,__zpabi_check_arguments__f_5,__zpabi_check_arguments__f_6,__zpabi_check_arguments__f_7,__zpabi_check_arguments__g_0,__zpabi_check_arguments__g_1,__zpabi_check_arguments__g_2,__zpabi_check_arguments__g_3,__zpabi_check_arguments__g_4,__zpabi_check_arguments__g_5,__zpabi_check_arguments__g_6,__zpabi_check_arguments__g_7,__zpabi_check_arguments__h_0,__zpabi_check_arguments__h_1,__zpabi_check_arguments__h_2,__zpabi_check_arguments__h_3,__zpabi_check_arguments__h_4,__zpabi_check_arguments__h_5,__zpabi_check_arguments__h_6,__zpabi_check_arguments__h_7 locals= indirect=false in_cycle=false
; def main params= locals=__local_main__0 indirect=false in_cycle=false
; call main -> check_arguments
; item main section=.text local=false zero=false
; item check_arguments section=.text local=false zero=false
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

main:
   SUBROUTINE

   LDA   #$00
   STA   __zpabi_check_arguments__a_0
   STA   __zpabi_check_arguments__a_1
   STA   __zpabi_check_arguments__a_2
   STA   __zpabi_check_arguments__a_3
   STA   __zpabi_check_arguments__a_4
   STA   __zpabi_check_arguments__a_5
   STA   __zpabi_check_arguments__b_0
   STA   __zpabi_check_arguments__b_1
   STA   __zpabi_check_arguments__b_2
   STA   __zpabi_check_arguments__b_3
   STA   __zpabi_check_arguments__b_4
   STA   __zpabi_check_arguments__b_5
   STA   __zpabi_check_arguments__b_6
   STA   __zpabi_check_arguments__c_0
   STA   __zpabi_check_arguments__c_1
   STA   __zpabi_check_arguments__c_2
   STA   __zpabi_check_arguments__c_3
   STA   __zpabi_check_arguments__c_4
   STA   __zpabi_check_arguments__c_5
   STA   __zpabi_check_arguments__d_0
   STA   __zpabi_check_arguments__d_1
   STA   __zpabi_check_arguments__d_2
   STA   __zpabi_check_arguments__d_3
   STA   __zpabi_check_arguments__d_4
   STA   __zpabi_check_arguments__d_5
   STA   __zpabi_check_arguments__e_0
   STA   __zpabi_check_arguments__e_1
   STA   __zpabi_check_arguments__e_2
   STA   __zpabi_check_arguments__e_3
   STA   __zpabi_check_arguments__e_4
   STA   __zpabi_check_arguments__e_5
   STA   __zpabi_check_arguments__f_0
   STA   __zpabi_check_arguments__f_1
   STA   __zpabi_check_arguments__f_2
   STA   __zpabi_check_arguments__f_3
   STA   __zpabi_check_arguments__f_4
   STA   __zpabi_check_arguments__f_5
   STA   __zpabi_check_arguments__f_6
   STA   __zpabi_check_arguments__g_0
   STA   __zpabi_check_arguments__g_1
   STA   __zpabi_check_arguments__g_2
   STA   __zpabi_check_arguments__g_3
   STA   __zpabi_check_arguments__g_4
   STA   __zpabi_check_arguments__g_5
   STA   __zpabi_check_arguments__h_0
   STA   __zpabi_check_arguments__h_1
   STA   __zpabi_check_arguments__h_2
   STA   __zpabi_check_arguments__h_3
   STA   __zpabi_check_arguments__h_4
   STA   __zpabi_check_arguments__h_5
   LDA   #$F0
   STA   __zpabi_check_arguments__a_6
   STA   __zpabi_check_arguments__e_6
   LDA   #$3F
   STA   __zpabi_check_arguments__a_7
   LDA   #$40
   STA   __zpabi_check_arguments__b_7
   STA   __zpabi_check_arguments__c_7
   STA   __zpabi_check_arguments__d_7
   LDA   #$08
   STA   __zpabi_check_arguments__c_6
   STA   __zpabi_check_arguments__g_6
   LDA   #$10
   STA   __zpabi_check_arguments__d_6
   STA   __zpabi_check_arguments__h_6
   LDA   #$BF
   STA   __zpabi_check_arguments__e_7
   LDA   #$C0
   STA   __zpabi_check_arguments__f_7
   STA   __zpabi_check_arguments__g_7
   STA   __zpabi_check_arguments__h_7
   JMP   check_arguments

check_arguments:
   SUBROUTINE

   LDA   __zpabi_check_arguments__a_7
   CMP   #$3F
   BNE   .jcmp_skip@0
   LDA   __zpabi_check_arguments__a_6
   CMP   #$F0
   BNE   .jcmp_skip@0
   LDA   __zpabi_check_arguments__a_5
   BNE   .jcmp_skip@0
   LDA   __zpabi_check_arguments__a_4
   BNE   .jcmp_skip@0
   LDA   __zpabi_check_arguments__a_3
   BNE   .jcmp_skip@0
   LDA   __zpabi_check_arguments__a_2
   BNE   .jcmp_skip@0
   LDA   __zpabi_check_arguments__a_1
   BNE   .jcmp_skip@0
   LDA   __zpabi_check_arguments__a_0
   BEQ   .if_end@0
.jcmp_skip@0:
   LDX   #$00
   LDA   #$01
   RTS
.if_end@0:
   LDA   __zpabi_check_arguments__b_7
   CMP   #$40
   BNE   .jcmp_skip@1
   LDA   __zpabi_check_arguments__b_6
   BNE   .jcmp_skip@1
   LDA   __zpabi_check_arguments__b_5
   BNE   .jcmp_skip@1
   LDA   __zpabi_check_arguments__b_4
   BNE   .jcmp_skip@1
   LDA   __zpabi_check_arguments__b_3
   BNE   .jcmp_skip@1
   LDA   __zpabi_check_arguments__b_2
   BNE   .jcmp_skip@1
   LDA   __zpabi_check_arguments__b_1
   BNE   .jcmp_skip@1
   LDA   __zpabi_check_arguments__b_0
   BEQ   .if_end@1
.jcmp_skip@1:
   LDX   #$00
   LDA   #$02
   RTS
.if_end@1:
   LDA   __zpabi_check_arguments__c_7
   CMP   #$40
   BNE   .jcmp_skip@2
   LDA   __zpabi_check_arguments__c_6
   CMP   #$08
   BNE   .jcmp_skip@2
   LDA   __zpabi_check_arguments__c_5
   BNE   .jcmp_skip@2
   LDA   __zpabi_check_arguments__c_4
   BNE   .jcmp_skip@2
   LDA   __zpabi_check_arguments__c_3
   BNE   .jcmp_skip@2
   LDA   __zpabi_check_arguments__c_2
   BNE   .jcmp_skip@2
   LDA   __zpabi_check_arguments__c_1
   BNE   .jcmp_skip@2
   LDA   __zpabi_check_arguments__c_0
   BEQ   .if_end@2
.jcmp_skip@2:
   LDX   #$00
   LDA   #$03
   RTS
.if_end@2:
   LDA   __zpabi_check_arguments__d_7
   CMP   #$40
   BNE   .jcmp_skip@3
   LDA   __zpabi_check_arguments__d_6
   CMP   #$10
   BNE   .jcmp_skip@3
   LDA   __zpabi_check_arguments__d_5
   BNE   .jcmp_skip@3
   LDA   __zpabi_check_arguments__d_4
   BNE   .jcmp_skip@3
   LDA   __zpabi_check_arguments__d_3
   BNE   .jcmp_skip@3
   LDA   __zpabi_check_arguments__d_2
   BNE   .jcmp_skip@3
   LDA   __zpabi_check_arguments__d_1
   BNE   .jcmp_skip@3
   LDA   __zpabi_check_arguments__d_0
   BEQ   .if_end@3
.jcmp_skip@3:
   LDX   #$00
   LDA   #$04
   RTS
.if_end@3:
   LDA   __zpabi_check_arguments__e_7
   CMP   #$BF
   BNE   .jcmp_skip@4
   LDA   __zpabi_check_arguments__e_6
   CMP   #$F0
   BNE   .jcmp_skip@4
   LDA   __zpabi_check_arguments__e_5
   BNE   .jcmp_skip@4
   LDA   __zpabi_check_arguments__e_4
   BNE   .jcmp_skip@4
   LDA   __zpabi_check_arguments__e_3
   BNE   .jcmp_skip@4
   LDA   __zpabi_check_arguments__e_2
   BNE   .jcmp_skip@4
   LDA   __zpabi_check_arguments__e_1
   BNE   .jcmp_skip@4
   LDA   __zpabi_check_arguments__e_0
   BEQ   .if_end@4
.jcmp_skip@4:
   LDX   #$00
   LDA   #$05
   RTS
.if_end@4:
   LDA   __zpabi_check_arguments__f_7
   CMP   #$C0
   BNE   .jcmp_skip@5
   LDA   __zpabi_check_arguments__f_6
   BNE   .jcmp_skip@5
   LDA   __zpabi_check_arguments__f_5
   BNE   .jcmp_skip@5
   LDA   __zpabi_check_arguments__f_4
   BNE   .jcmp_skip@5
   LDA   __zpabi_check_arguments__f_3
   BNE   .jcmp_skip@5
   LDA   __zpabi_check_arguments__f_2
   BNE   .jcmp_skip@5
   LDA   __zpabi_check_arguments__f_1
   BNE   .jcmp_skip@5
   LDA   __zpabi_check_arguments__f_0
   BEQ   .if_end@5
.jcmp_skip@5:
   LDX   #$00
   LDA   #$06
   RTS
.if_end@5:
   LDA   __zpabi_check_arguments__g_7
   CMP   #$C0
   BNE   .jcmp_skip@6
   LDA   __zpabi_check_arguments__g_6
   CMP   #$08
   BNE   .jcmp_skip@6
   LDA   __zpabi_check_arguments__g_5
   BNE   .jcmp_skip@6
   LDA   __zpabi_check_arguments__g_4
   BNE   .jcmp_skip@6
   LDA   __zpabi_check_arguments__g_3
   BNE   .jcmp_skip@6
   LDA   __zpabi_check_arguments__g_2
   BNE   .jcmp_skip@6
   LDA   __zpabi_check_arguments__g_1
   BNE   .jcmp_skip@6
   LDA   __zpabi_check_arguments__g_0
   BEQ   .if_end@6
.jcmp_skip@6:
   LDX   #$00
   LDA   #$07
   RTS
.if_end@6:
   LDA   __zpabi_check_arguments__h_7
   CMP   #$C0
   BNE   .jcmp_skip@7
   LDA   __zpabi_check_arguments__h_6
   CMP   #$10
   BNE   .jcmp_skip@7
   LDA   __zpabi_check_arguments__h_5
   BNE   .jcmp_skip@7
   LDA   __zpabi_check_arguments__h_4
   BNE   .jcmp_skip@7
   LDA   __zpabi_check_arguments__h_3
   BNE   .jcmp_skip@7
   LDA   __zpabi_check_arguments__h_2
   BNE   .jcmp_skip@7
   LDA   __zpabi_check_arguments__h_1
   BNE   .jcmp_skip@7
   LDA   __zpabi_check_arguments__h_0
   BEQ   .if_end@7
.jcmp_skip@7:
   LDX   #$00
   LDA   #$08
   RTS
.if_end@7:
   LDX   #$00
   TXA
   RTS
