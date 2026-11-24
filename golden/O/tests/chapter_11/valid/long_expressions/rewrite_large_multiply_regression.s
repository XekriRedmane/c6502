SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__zpabi_check_12_ints__b_0	EQU	$80
__zpabi_check_12_ints__b_1	EQU	$81
__zpabi_check_12_ints__c_0	EQU	$82
__zpabi_check_12_ints__c_1	EQU	$83
__zpabi_check_12_ints__d_0	EQU	$84
__zpabi_check_12_ints__d_1	EQU	$85
__zpabi_check_12_ints__e_0	EQU	$86
__zpabi_check_12_ints__e_1	EQU	$87
__zpabi_check_12_ints__f_0	EQU	$88
__zpabi_check_12_ints__f_1	EQU	$89
__zpabi_check_12_ints__g_0	EQU	$8A
__zpabi_check_12_ints__g_1	EQU	$8B
__zpabi_check_12_ints__h_0	EQU	$8C
__zpabi_check_12_ints__h_1	EQU	$8D
__zpabi_check_12_ints__i_0	EQU	$8E
__zpabi_check_12_ints__i_1	EQU	$8F
__zpabi_check_12_ints__j_0	EQU	$90
__zpabi_check_12_ints__j_1	EQU	$91
__zpabi_check_12_ints__k_0	EQU	$92
__zpabi_check_12_ints__k_1	EQU	$93
__zpabi_check_12_ints__l_0	EQU	$94
__zpabi_check_12_ints__l_1	EQU	$95
__zpabi_check_12_ints__start_0	EQU	$96
__zpabi_check_12_ints__start_1	EQU	$97
__local_main__0	EQU	$98
__local_main__1	EQU	$99
__local_main__2	EQU	$9A
__local_main__3	EQU	$9B
__local_main__4	EQU	$9C
__local_main__5	EQU	$9D
__local_main__6	EQU	$9E
__local_main__7	EQU	$9F
__local_main__8	EQU	$A0
__local_main__9	EQU	$A1
__local_main__10	EQU	$A2
__local_main__11	EQU	$A3
__local_main__12	EQU	$A4
__local_main__13	EQU	$A5
__local_main__14	EQU	$A6
__local_main__15	EQU	$A7
__local_main__16	EQU	$A8
__local_main__17	EQU	$A9
__local_main__18	EQU	$AA
__local_main__19	EQU	$AB
__local_main__20	EQU	$AC
__local_main__21	EQU	$AD
__local_main__22	EQU	$AE
__local_main__23	EQU	$AF
__local_check_12_ints__0	EQU	$BC
__local_check_12_ints__1	EQU	$BD
__local_check_12_ints__a_0	EQU	$BF

; @zp-link-meta-begin
; def check_12_ints params=__zpabi_check_12_ints__a_0,__zpabi_check_12_ints__a_1,__zpabi_check_12_ints__b_0,__zpabi_check_12_ints__b_1,__zpabi_check_12_ints__c_0,__zpabi_check_12_ints__c_1,__zpabi_check_12_ints__d_0,__zpabi_check_12_ints__d_1,__zpabi_check_12_ints__e_0,__zpabi_check_12_ints__e_1,__zpabi_check_12_ints__f_0,__zpabi_check_12_ints__f_1,__zpabi_check_12_ints__g_0,__zpabi_check_12_ints__g_1,__zpabi_check_12_ints__h_0,__zpabi_check_12_ints__h_1,__zpabi_check_12_ints__i_0,__zpabi_check_12_ints__i_1,__zpabi_check_12_ints__j_0,__zpabi_check_12_ints__j_1,__zpabi_check_12_ints__k_0,__zpabi_check_12_ints__k_1,__zpabi_check_12_ints__l_0,__zpabi_check_12_ints__l_1,__zpabi_check_12_ints__start_0,__zpabi_check_12_ints__start_1 locals=__local_check_12_ints__0,__local_check_12_ints__1,__local_check_12_ints__a_1,__local_check_12_ints__a_0 indirect=false in_cycle=false param_regs=A,X,,,,,,,,,,,,,,,,,,,,,,,,
; def main params= locals=__local_main__0,__local_main__1,__local_main__2,__local_main__3,__local_main__4,__local_main__5,__local_main__6,__local_main__7,__local_main__8,__local_main__9,__local_main__10,__local_main__11,__local_main__12,__local_main__13,__local_main__14,__local_main__15,__local_main__16,__local_main__17,__local_main__18,__local_main__19,__local_main__20,__local_main__21,__local_main__22,__local_main__23,__local_main__24,__local_main__25,__local_main__26,__local_main__27,__local_main__28,__local_main__29,__local_main__30,__local_main__31,__local_main__32,__local_main__33,__local_main__34,__local_main__35 indirect=false in_cycle=false
; call main -> check_12_ints
; item main section=.text local=false zero=false
; item main@run1 section=.text local=true zero=false
; item check_12_ints section=.text local=false zero=false
; item check_12_ints@run0 section=.text local=true zero=false
; item glob section=.data local=false zero=false
; runtime hargs_size=32 hargs_need=24
; @zp-link-meta-end

main:
   SUBROUTINE

   LDA   glob
   STA   HARGS
   LDA   glob+1
   STA   HARGS+1
   LDA   glob+2
   STA   HARGS+2
   LDA   glob+3
   STA   HARGS+3
   LDA   glob+4
   STA   HARGS+4
   LDA   glob+5
   STA   HARGS+5
   LDA   glob+6
   STA   HARGS+6
   LDA   glob+7
   STA   HARGS+7
   LDA   #$C2
   STA   HARGS+9
   LDA   #$EB
   STA   HARGS+10
   LDA   #$0B
   STA   HARGS+11
   LDA   #$00
   STA   HARGS+8
   STA   HARGS+12
   LDY   #$00
   STY   HARGS+13
   STY   HARGS+14
   STA   HARGS+15
   JSR   mul64
   LDA   HARGS+16
   STA   __local_main__23
   LDA   HARGS+17
   STA   __local_main__22
   LDA   HARGS+18
   STA   __local_main__21
   LDA   HARGS+19
   STA   __local_main__20
   LDA   HARGS+20
   STA   __local_main__19
   LDA   HARGS+21
   STA   __local_main__18
   LDA   HARGS+22
   STA   __local_main__17
   LDA   HARGS+23
   STA   __local_main__16
   LDA   glob+1
   STA   __local_main__15
   LDA   glob
   SEC
   SBC   #$04
   STA   __local_main__14
   BCS   .carry_inc@0
   DEC   __local_main__15
.carry_inc@0:
   CLC
   ADC   __local_main__14
   STA   __local_main__13
   LDA   __local_main__15
   ADC   __local_main__15
   STA   __local_main__12
   LDA   #$02
   CLC
   ADC   __local_main__14
   STA   __local_main__11
   LDA   #$00
   ADC   __local_main__15
   STA   __local_main__10
   LDA   __local_main__13
   STA   HARGS
   STA   HARGS+2
   LDA   __local_main__12
   STA   HARGS+1
   JSR   main@run1
   STA   __local_main__9
   LDA   HARGS+5
   STA   __local_main__8
   LDA   #$06
   SEC
   SBC   __local_main__14
   STA   __local_main__7
   LDA   #$00
   SBC   __local_main__15
   STA   __local_main__6
   LDA   __local_main__13
   STA   HARGS
   LDA   __local_main__12
   STA   HARGS+1
   LDA   __local_main__11
   STA   HARGS+2
   LDA   __local_main__10
   JSR   main@run1
   STA   __local_main__5
   LDA   HARGS+5
   STA   __local_main__4
   LDA   __local_main__14
   CLC
   ADC   #$06
   STA   __local_main__3
   LDA   __local_main__15
   ADC   #$00
   STA   __local_main__2
   LDA   __local_main__13
   ASL
   STA   __local_main__0
   LDA   __local_main__12
   ROL
   ASL   __local_main__0
   ROL
   STA   __local_main__1
   LDA   __local_main__11
   STA   HARGS
   STA   HARGS+2
   LDA   __local_main__10
   STA   HARGS+1
   JSR   main@run1
   STA   __zpabi_check_12_ints__i_0
   LDA   HARGS+5
   STA   __zpabi_check_12_ints__i_1
   LDA   __local_main__9
   CLC
   ADC   __local_main__5
   STA   __zpabi_check_12_ints__j_0
   LDA   __local_main__8
   ADC   __local_main__4
   STA   __zpabi_check_12_ints__j_1
   LDA   #$10
   SEC
   SBC   __local_main__7
   STA   __zpabi_check_12_ints__k_0
   LDA   #$00
   SBC   __local_main__6
   STA   __zpabi_check_12_ints__k_1
   LDA   __local_main__5
   CLC
   ADC   __local_main__5
   STA   __zpabi_check_12_ints__l_0
   LDA   __local_main__4
   ADC   __local_main__4
   STA   __zpabi_check_12_ints__l_1
   LDA   __local_main__13
   STA   __zpabi_check_12_ints__b_0
   LDA   __local_main__12
   STA   __zpabi_check_12_ints__b_1
   LDA   __local_main__11
   STA   __zpabi_check_12_ints__c_0
   LDA   __local_main__10
   STA   __zpabi_check_12_ints__c_1
   LDA   __local_main__9
   STA   __zpabi_check_12_ints__d_0
   LDA   __local_main__8
   STA   __zpabi_check_12_ints__d_1
   LDA   __local_main__7
   STA   __zpabi_check_12_ints__e_0
   LDA   __local_main__6
   STA   __zpabi_check_12_ints__e_1
   LDA   __local_main__5
   STA   __zpabi_check_12_ints__f_0
   LDA   __local_main__4
   STA   __zpabi_check_12_ints__f_1
   LDA   __local_main__3
   STA   __zpabi_check_12_ints__g_0
   LDA   __local_main__2
   STA   __zpabi_check_12_ints__g_1
   LDA   __local_main__0
   STA   __zpabi_check_12_ints__h_0
   LDA   __local_main__1
   STA   __zpabi_check_12_ints__h_1
   LDA   #$01
   STA   __zpabi_check_12_ints__start_0
   LDA   #$00
   STA   __zpabi_check_12_ints__start_1
   LDX   __local_main__15
   LDA   __local_main__14
   JSR   check_12_ints
   LDA   glob+1
   STA   __local_main__11
   LDA   glob
   CLC
   ADC   #$08
   STA   __local_main__10
   BCC   .carry_inc@1
   INC   __local_main__11
.carry_inc@1:
   LDA   #$1C
   SEC
   SBC   __local_main__10
   STA   __local_main__9
   LDA   #$00
   SBC   __local_main__11
   STA   __local_main__8
   LDA   #$04
   CLC
   ADC   __local_main__10
   STA   __local_main__7
   LDA   #$00
   ADC   __local_main__11
   STA   __local_main__6
   LDA   __local_main__10
   CLC
   ADC   #$01
   STA   __local_main__5
   LDA   __local_main__11
   ADC   #$00
   STA   __local_main__4
   LDA   #$20
   SEC
   SBC   __local_main__5
   STA   __zpabi_check_12_ints__f_0
   LDA   #$00
   SBC   __local_main__4
   STA   __zpabi_check_12_ints__f_1
   LDA   __local_main__5
   CLC
   ADC   #$02
   STA   __local_main__3
   LDA   __local_main__4
   ADC   #$00
   STA   __local_main__2
   LDA   #$23
   SEC
   SBC   __local_main__3
   STA   __zpabi_check_12_ints__g_0
   LDA   #$00
   SBC   __local_main__2
   STA   __zpabi_check_12_ints__g_1
   LDA   __local_main__9
   CLC
   ADC   #$05
   STA   __zpabi_check_12_ints__h_0
   LDA   __local_main__8
   ADC   #$00
   STA   __zpabi_check_12_ints__h_1
   LDA   __local_main__10
   ASL
   STA   __local_main__0
   LDA   __local_main__11
   ROL
   STA   __local_main__1
   LDA   __local_main__0
   SEC
   SBC   #$05
   STA   __zpabi_check_12_ints__i_0
   LDA   __local_main__1
   SBC   #$00
   STA   __zpabi_check_12_ints__i_1
   LDA   __local_main__9
   CLC
   ADC   #$07
   STA   __zpabi_check_12_ints__j_0
   LDA   __local_main__8
   ADC   #$00
   STA   __zpabi_check_12_ints__j_1
   LDA   #$06
   CLC
   ADC   __local_main__7
   STA   __zpabi_check_12_ints__k_0
   LDA   #$00
   ADC   __local_main__6
   STA   __zpabi_check_12_ints__k_1
   LDA   __local_main__10
   CLC
   ADC   #$0B
   STA   __zpabi_check_12_ints__l_0
   LDA   __local_main__11
   ADC   #$00
   STA   __zpabi_check_12_ints__l_1
   LDA   __local_main__5
   STA   __zpabi_check_12_ints__b_0
   LDA   __local_main__4
   STA   __zpabi_check_12_ints__b_1
   LDA   __local_main__9
   STA   __zpabi_check_12_ints__c_0
   LDA   __local_main__8
   STA   __zpabi_check_12_ints__c_1
   LDA   __local_main__3
   STA   __zpabi_check_12_ints__d_0
   LDA   __local_main__2
   STA   __zpabi_check_12_ints__d_1
   LDA   __local_main__7
   STA   __zpabi_check_12_ints__e_0
   LDA   __local_main__6
   STA   __zpabi_check_12_ints__e_1
   LDA   #$0D
   STA   __zpabi_check_12_ints__start_0
   LDA   #$00
   STA   __zpabi_check_12_ints__start_1
   LDX   __local_main__11
   LDA   __local_main__10
   JSR   check_12_ints
   LDA   __local_main__16
   BNE   .jcmp_skip@0
   LDA   __local_main__17
   BNE   .jcmp_skip@0
   LDA   __local_main__18
   BNE   .jcmp_skip@0
   LDA   __local_main__19
   BNE   .jcmp_skip@0
   LDA   __local_main__20
   CMP   #$3B
   BNE   .jcmp_skip@0
   LDA   __local_main__21
   CMP   #$9A
   BNE   .jcmp_skip@0
   LDA   __local_main__22
   CMP   #$CA
   BNE   .jcmp_skip@0
   LDA   __local_main__23
   BEQ   .if_end@0
.jcmp_skip@0:
   LDX   #$FF
   TXA
   RTS
.if_end@0:
   LDX   #$00
   TXA
   RTS

main@run1:
   SUBROUTINE

   STA   HARGS+3
   JSR   mul16
   LDA   HARGS+4
   RTS

check_12_ints:
   SUBROUTINE

   STA   __local_check_12_ints__a_0
   LDA   __zpabi_check_12_ints__start_0
   STA   __local_check_12_ints__1
   LDA   __zpabi_check_12_ints__start_1
   STA   __local_check_12_ints__0
   CPX   __local_check_12_ints__0
   BNE   .jcmp_skip@1
   LDA   __local_check_12_ints__a_0
   CMP   __local_check_12_ints__1
   BEQ   .if_end@1
.jcmp_skip@1:
   LDX   __local_check_12_ints__0
   LDA   __local_check_12_ints__1
   RTS
.if_end@1:
   LDA   __zpabi_check_12_ints__start_0
   CLC
   ADC   #$01
   STA   __local_check_12_ints__1
   JSR   check_12_ints@run0
   STA   __local_check_12_ints__0
   LDA   __zpabi_check_12_ints__b_1
   CMP   __local_check_12_ints__0
   BNE   .jcmp_skip@2
   LDA   __zpabi_check_12_ints__b_0
   CMP   __local_check_12_ints__1
   BEQ   .if_end@2
.jcmp_skip@2:
   LDX   __local_check_12_ints__0
   LDA   __local_check_12_ints__1
   RTS
.if_end@2:
   LDA   __zpabi_check_12_ints__start_0
   CLC
   ADC   #$02
   STA   __local_check_12_ints__1
   JSR   check_12_ints@run0
   STA   __local_check_12_ints__0
   LDA   __zpabi_check_12_ints__c_1
   CMP   __local_check_12_ints__0
   BNE   .jcmp_skip@3
   LDA   __zpabi_check_12_ints__c_0
   CMP   __local_check_12_ints__1
   BEQ   .if_end@3
.jcmp_skip@3:
   LDX   __local_check_12_ints__0
   LDA   __local_check_12_ints__1
   RTS
.if_end@3:
   LDA   __zpabi_check_12_ints__start_0
   CLC
   ADC   #$03
   STA   __local_check_12_ints__1
   JSR   check_12_ints@run0
   STA   __local_check_12_ints__0
   LDA   __zpabi_check_12_ints__d_1
   CMP   __local_check_12_ints__0
   BNE   .jcmp_skip@4
   LDA   __zpabi_check_12_ints__d_0
   CMP   __local_check_12_ints__1
   BEQ   .if_end@4
.jcmp_skip@4:
   LDX   __local_check_12_ints__0
   LDA   __local_check_12_ints__1
   RTS
.if_end@4:
   LDA   __zpabi_check_12_ints__start_0
   CLC
   ADC   #$04
   STA   __local_check_12_ints__1
   JSR   check_12_ints@run0
   STA   __local_check_12_ints__0
   LDA   __zpabi_check_12_ints__e_1
   CMP   __local_check_12_ints__0
   BNE   .jcmp_skip@5
   LDA   __zpabi_check_12_ints__e_0
   CMP   __local_check_12_ints__1
   BEQ   .if_end@5
.jcmp_skip@5:
   LDX   __local_check_12_ints__0
   LDA   __local_check_12_ints__1
   RTS
.if_end@5:
   LDA   __zpabi_check_12_ints__start_0
   CLC
   ADC   #$05
   STA   __local_check_12_ints__1
   JSR   check_12_ints@run0
   STA   __local_check_12_ints__0
   LDA   __zpabi_check_12_ints__f_1
   CMP   __local_check_12_ints__0
   BNE   .jcmp_skip@6
   LDA   __zpabi_check_12_ints__f_0
   CMP   __local_check_12_ints__1
   BEQ   .if_end@6
.jcmp_skip@6:
   LDX   __local_check_12_ints__0
   LDA   __local_check_12_ints__1
   RTS
.if_end@6:
   LDA   __zpabi_check_12_ints__start_0
   CLC
   ADC   #$06
   STA   __local_check_12_ints__1
   JSR   check_12_ints@run0
   STA   __local_check_12_ints__0
   LDA   __zpabi_check_12_ints__g_1
   CMP   __local_check_12_ints__0
   BNE   .jcmp_skip@7
   LDA   __zpabi_check_12_ints__g_0
   CMP   __local_check_12_ints__1
   BEQ   .if_end@7
.jcmp_skip@7:
   LDX   __local_check_12_ints__0
   LDA   __local_check_12_ints__1
   RTS
.if_end@7:
   LDA   __zpabi_check_12_ints__start_0
   CLC
   ADC   #$07
   STA   __local_check_12_ints__1
   JSR   check_12_ints@run0
   STA   __local_check_12_ints__0
   LDA   __zpabi_check_12_ints__h_1
   CMP   __local_check_12_ints__0
   BNE   .jcmp_skip@8
   LDA   __zpabi_check_12_ints__h_0
   CMP   __local_check_12_ints__1
   BEQ   .if_end@8
.jcmp_skip@8:
   LDX   __local_check_12_ints__0
   LDA   __local_check_12_ints__1
   RTS
.if_end@8:
   LDA   __zpabi_check_12_ints__start_0
   CLC
   ADC   #$08
   STA   __local_check_12_ints__1
   JSR   check_12_ints@run0
   STA   __local_check_12_ints__0
   LDA   __zpabi_check_12_ints__i_1
   CMP   __local_check_12_ints__0
   BNE   .jcmp_skip@12
   LDA   __zpabi_check_12_ints__i_0
   CMP   __local_check_12_ints__1
   BNE   .jcmp_skip@12
   LDA   __zpabi_check_12_ints__start_0
   CLC
   ADC   #$09
   STA   __local_check_12_ints__1
   JSR   check_12_ints@run0
   STA   __local_check_12_ints__0
   LDA   __zpabi_check_12_ints__j_1
   CMP   __local_check_12_ints__0
   BNE   .jcmp_skip@12
   LDA   __zpabi_check_12_ints__j_0
   CMP   __local_check_12_ints__1
   BNE   .jcmp_skip@12
   LDA   __zpabi_check_12_ints__start_0
   CLC
   ADC   #$0A
   STA   __local_check_12_ints__1
   JSR   check_12_ints@run0
   STA   __local_check_12_ints__0
   LDA   __zpabi_check_12_ints__k_1
   CMP   __local_check_12_ints__0
   BNE   .jcmp_skip@12
   LDA   __zpabi_check_12_ints__k_0
   CMP   __local_check_12_ints__1
   BNE   .jcmp_skip@12
   LDA   __zpabi_check_12_ints__start_0
   CLC
   ADC   #$0B
   STA   __local_check_12_ints__1
   JSR   check_12_ints@run0
   STA   __local_check_12_ints__0
   LDA   __zpabi_check_12_ints__l_1
   CMP   __local_check_12_ints__0
   BNE   .jcmp_skip@12
   LDA   __zpabi_check_12_ints__l_0
   CMP   __local_check_12_ints__1
   BEQ   .if_end@12
.jcmp_skip@12:
   LDX   __local_check_12_ints__0
   LDA   __local_check_12_ints__1
   RTS
.if_end@12:
   LDX   #$00
   TXA
   RTS

check_12_ints@run0:
   SUBROUTINE

   LDA   __zpabi_check_12_ints__start_1
   ADC   #$00
   RTS

glob:
   DC.L  $00000005
   DC.L  $00000000
