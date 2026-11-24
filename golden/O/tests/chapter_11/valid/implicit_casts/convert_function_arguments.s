SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__zpabi_foo__a_0	EQU	$80
__zpabi_foo__a_1	EQU	$81
__zpabi_foo__a_2	EQU	$82
__zpabi_foo__a_3	EQU	$83
__zpabi_foo__a_4	EQU	$84
__zpabi_foo__a_5	EQU	$85
__zpabi_foo__a_6	EQU	$86
__zpabi_foo__a_7	EQU	$87
__zpabi_foo__b_0	EQU	$88
__zpabi_foo__b_1	EQU	$89
__zpabi_foo__c_0	EQU	$8A
__zpabi_foo__c_1	EQU	$8B
__zpabi_foo__d_0	EQU	$8C
__zpabi_foo__d_1	EQU	$8D
__zpabi_foo__e_0	EQU	$8E
__zpabi_foo__e_1	EQU	$8F
__zpabi_foo__e_2	EQU	$90
__zpabi_foo__e_3	EQU	$91
__zpabi_foo__e_4	EQU	$92
__zpabi_foo__e_5	EQU	$93
__zpabi_foo__e_6	EQU	$94
__zpabi_foo__e_7	EQU	$95
__zpabi_foo__f_0	EQU	$96
__zpabi_foo__f_1	EQU	$97
__zpabi_foo__g_0	EQU	$98
__zpabi_foo__g_1	EQU	$99
__zpabi_foo__g_2	EQU	$9A
__zpabi_foo__g_3	EQU	$9B
__zpabi_foo__g_4	EQU	$9C
__zpabi_foo__g_5	EQU	$9D
__zpabi_foo__g_6	EQU	$9E
__zpabi_foo__g_7	EQU	$9F
__zpabi_foo__h_0	EQU	$A0
__zpabi_foo__h_1	EQU	$A1

; @zp-link-meta-begin
; def foo params=__zpabi_foo__a_0,__zpabi_foo__a_1,__zpabi_foo__a_2,__zpabi_foo__a_3,__zpabi_foo__a_4,__zpabi_foo__a_5,__zpabi_foo__a_6,__zpabi_foo__a_7,__zpabi_foo__b_0,__zpabi_foo__b_1,__zpabi_foo__c_0,__zpabi_foo__c_1,__zpabi_foo__d_0,__zpabi_foo__d_1,__zpabi_foo__e_0,__zpabi_foo__e_1,__zpabi_foo__e_2,__zpabi_foo__e_3,__zpabi_foo__e_4,__zpabi_foo__e_5,__zpabi_foo__e_6,__zpabi_foo__e_7,__zpabi_foo__f_0,__zpabi_foo__f_1,__zpabi_foo__g_0,__zpabi_foo__g_1,__zpabi_foo__g_2,__zpabi_foo__g_3,__zpabi_foo__g_4,__zpabi_foo__g_5,__zpabi_foo__g_6,__zpabi_foo__g_7,__zpabi_foo__h_0,__zpabi_foo__h_1 locals= indirect=false in_cycle=false
; def main params= locals=__local_main__0 indirect=false in_cycle=false
; call main -> foo
; item foo section=.text local=false zero=false
; item main section=.text local=false zero=false
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

foo:
   SUBROUTINE

   LDA   __zpabi_foo__a_7
   CMP   #$FF
   BNE   .jcmp_skip@0
   LDA   __zpabi_foo__a_6
   CMP   #$FF
   BNE   .jcmp_skip@0
   LDA   __zpabi_foo__a_5
   CMP   #$FF
   BNE   .jcmp_skip@0
   LDA   __zpabi_foo__a_4
   CMP   #$FF
   BNE   .jcmp_skip@0
   LDA   __zpabi_foo__a_3
   CMP   #$FF
   BNE   .jcmp_skip@0
   LDA   __zpabi_foo__a_2
   CMP   #$FF
   BNE   .jcmp_skip@0
   LDA   __zpabi_foo__a_1
   CMP   #$FF
   BNE   .jcmp_skip@0
   LDA   __zpabi_foo__a_0
   CMP   #$FF
   BEQ   .if_end@0
.jcmp_skip@0:
   LDX   #$00
   LDA   #$01
   RTS
.if_end@0:
   LDA   __zpabi_foo__b_1
   BNE   .jcmp_skip@1
   LDA   __zpabi_foo__b_0
   CMP   #$02
   BEQ   .if_end@1
.jcmp_skip@1:
   LDX   #$00
   LDA   #$02
   RTS
.if_end@1:
   LDA   __zpabi_foo__c_0
   ORA   __zpabi_foo__c_1
   BEQ   .if_end@2
   LDX   #$00
   LDA   #$03
   RTS
.if_end@2:
   LDA   __zpabi_foo__d_1
   CMP   #$FF
   BNE   .jcmp_skip@2
   LDA   __zpabi_foo__d_0
   CMP   #$FB
   BEQ   .if_end@3
.jcmp_skip@2:
   LDX   #$00
   LDA   #$04
   RTS
.if_end@3:
   LDA   __zpabi_foo__e_7
   CMP   #$FF
   BNE   .jcmp_skip@3
   LDA   __zpabi_foo__e_6
   CMP   #$FF
   BNE   .jcmp_skip@3
   LDA   __zpabi_foo__e_5
   CMP   #$FF
   BNE   .jcmp_skip@3
   LDA   __zpabi_foo__e_4
   CMP   #$FF
   BNE   .jcmp_skip@3
   LDA   __zpabi_foo__e_3
   CMP   #$FF
   BNE   .jcmp_skip@3
   LDA   __zpabi_foo__e_2
   CMP   #$FF
   BNE   .jcmp_skip@3
   LDA   __zpabi_foo__e_1
   CMP   #$FF
   BNE   .jcmp_skip@3
   LDA   __zpabi_foo__e_0
   CMP   #$9B
   BEQ   .if_end@4
.jcmp_skip@3:
   LDX   #$00
   LDA   #$05
   RTS
.if_end@4:
   LDA   __zpabi_foo__f_1
   CMP   #$FF
   BNE   .jcmp_skip@4
   LDA   __zpabi_foo__f_0
   CMP   #$85
   BEQ   .if_end@5
.jcmp_skip@4:
   LDX   #$00
   LDA   #$06
   RTS
.if_end@5:
   LDA   __zpabi_foo__g_7
   CMP   #$FF
   BNE   .jcmp_skip@5
   LDA   __zpabi_foo__g_6
   CMP   #$FF
   BNE   .jcmp_skip@5
   LDA   __zpabi_foo__g_5
   CMP   #$FF
   BNE   .jcmp_skip@5
   LDA   __zpabi_foo__g_4
   CMP   #$FF
   BNE   .jcmp_skip@5
   LDA   __zpabi_foo__g_3
   CMP   #$FF
   BNE   .jcmp_skip@5
   LDA   __zpabi_foo__g_2
   CMP   #$FF
   BNE   .jcmp_skip@5
   LDA   __zpabi_foo__g_1
   CMP   #$FF
   BNE   .jcmp_skip@5
   LDA   __zpabi_foo__g_0
   CMP   #$F6
   BEQ   .if_end@6
.jcmp_skip@5:
   LDX   #$00
   LDA   #$07
   RTS
.if_end@6:
   LDA   __zpabi_foo__h_1
   CMP   #$FF
   BNE   .jcmp_skip@6
   LDA   __zpabi_foo__h_0
   CMP   #$D2
   BEQ   .if_end@7
.jcmp_skip@6:
   LDX   #$00
   LDA   #$08
   RTS
.if_end@7:
   LDX   #$00
   TXA
   RTS

main:
   SUBROUTINE

   LDA   #$FF
   STA   __zpabi_foo__a_0
   STA   __zpabi_foo__a_1
   STA   __zpabi_foo__a_2
   STA   __zpabi_foo__a_3
   STA   __zpabi_foo__a_4
   STA   __zpabi_foo__a_5
   STA   __zpabi_foo__a_6
   STA   __zpabi_foo__a_7
   STA   __zpabi_foo__c_1
   STA   __zpabi_foo__e_1
   STA   __zpabi_foo__e_2
   STA   __zpabi_foo__e_3
   STA   __zpabi_foo__e_4
   STA   __zpabi_foo__e_5
   STA   __zpabi_foo__e_6
   STA   __zpabi_foo__e_7
   STA   __zpabi_foo__f_1
   STA   __zpabi_foo__g_1
   STA   __zpabi_foo__g_2
   STA   __zpabi_foo__g_3
   STA   __zpabi_foo__g_4
   STA   __zpabi_foo__g_5
   STA   __zpabi_foo__g_6
   STA   __zpabi_foo__g_7
   LDA   #$02
   STA   __zpabi_foo__b_0
   LDA   #$01
   STA   __zpabi_foo__b_1
   LDA   #$00
   STA   __zpabi_foo__c_0
   STA   __zpabi_foo__d_1
   LDA   #$FB
   STA   __zpabi_foo__d_0
   LDA   #$9B
   STA   __zpabi_foo__e_0
   LDA   #$85
   STA   __zpabi_foo__f_0
   LDA   #$F6
   STA   __zpabi_foo__g_0
   LDA   #$D2
   STA   __zpabi_foo__h_0
   LDA   #$04
   STA   __zpabi_foo__h_1
   JMP   foo
