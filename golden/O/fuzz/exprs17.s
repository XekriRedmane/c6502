SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__zpabi_check__v1_0	EQU	$80
__zpabi_check__v1_1	EQU	$81
__zpabi_check__v1_2	EQU	$82
__zpabi_check__v1_3	EQU	$83
__zpabi_check__v2_0	EQU	$84
__zpabi_check__v2_1	EQU	$85
__zpabi_check__v3	EQU	$86
__zpabi_check__v4_0	EQU	$87
__zpabi_check__v4_1	EQU	$88
__zpabi_check__v4_2	EQU	$89
__zpabi_check__v4_3	EQU	$8A
__zpabi_check__v4_4	EQU	$8B
__zpabi_check__v4_5	EQU	$8C
__zpabi_check__v4_6	EQU	$8D
__zpabi_check__v4_7	EQU	$8E
__zpabi_check__v5_0	EQU	$8F
__zpabi_check__v5_1	EQU	$90
__zpabi_check__v5_2	EQU	$91
__zpabi_check__v5_3	EQU	$92
__zpabi_check__v5_4	EQU	$93
__zpabi_check__v5_5	EQU	$94
__zpabi_check__v5_6	EQU	$95
__zpabi_check__v5_7	EQU	$96
__local_check__fail_1	EQU	$98
__local_check__fail_0	EQU	$99
__local_check__0	EQU	$9A
__local_check__1	EQU	$9B
__local_check__2	EQU	$9C
__local_check__3	EQU	$9D
__local_check__4	EQU	$9E
__local_check__5	EQU	$9F
__local_check__6	EQU	$A0
__local_check__7	EQU	$A1
__local_check__8	EQU	$A2
__local_check__9	EQU	$A3
__local_check__10	EQU	$A4
__local_check__11	EQU	$A5
__local_check__12	EQU	$A6
__local_check__13	EQU	$A7
__local_check__14	EQU	$A8
__local_check__15	EQU	$A9
__local_check__16	EQU	$AA
__local_check__v0_1	EQU	$AB
__local_check__v0_0	EQU	$AC

; @zp-link-meta-begin
; def check params=__zpabi_check__v0_0,__zpabi_check__v0_1,__zpabi_check__v1_0,__zpabi_check__v1_1,__zpabi_check__v1_2,__zpabi_check__v1_3,__zpabi_check__v2_0,__zpabi_check__v2_1,__zpabi_check__v3,__zpabi_check__v4_0,__zpabi_check__v4_1,__zpabi_check__v4_2,__zpabi_check__v4_3,__zpabi_check__v4_4,__zpabi_check__v4_5,__zpabi_check__v4_6,__zpabi_check__v4_7,__zpabi_check__v5_0,__zpabi_check__v5_1,__zpabi_check__v5_2,__zpabi_check__v5_3,__zpabi_check__v5_4,__zpabi_check__v5_5,__zpabi_check__v5_6,__zpabi_check__v5_7 locals=__local_check__fail_1,__local_check__fail_0,__local_check__0,__local_check__1,__local_check__2,__local_check__3,__local_check__4,__local_check__5,__local_check__6,__local_check__7,__local_check__8,__local_check__9,__local_check__10,__local_check__11,__local_check__12,__local_check__13,__local_check__14,__local_check__15,__local_check__16,__local_check__v0_1,__local_check__v0_0,__local_check__17,__local_check__18,__local_check__19,__local_check__20,__local_check__21,__local_check__22,__local_check__23,__local_check__24 indirect=false in_cycle=false param_regs=A,X,,,,,,,,,,,,,,,,,,,,,,,
; def main params= locals=__local_main__0 indirect=false in_cycle=false
; call main -> check
; item check section=.text local=false zero=false
; item check@run0 section=.text local=true zero=false
; item main section=.text local=false zero=false
; runtime hargs_size=32 hargs_need=24
; @zp-link-meta-end

check:
   SUBROUTINE

   STA   __local_check__v0_0
   STX   __local_check__v0_1
   LDA   #$00
   CMP   __zpabi_check__v4_7
   BNE   .cmp_true@0
   CMP   __zpabi_check__v4_6
   BNE   .cmp_true@0
   CMP   __zpabi_check__v4_5
   BNE   .cmp_true@0
   CMP   __zpabi_check__v4_4
   BNE   .cmp_true@0
   CMP   __zpabi_check__v4_3
   BNE   .cmp_true@0
   CMP   __zpabi_check__v4_2
   BNE   .cmp_true@0
   LDA   __zpabi_check__v2_1
   CMP   __zpabi_check__v4_1
   BNE   .cmp_true@0
   LDA   __zpabi_check__v2_0
   CMP   __zpabi_check__v4_0
   BNE   .cmp_true@0
   LDA   #$00
   BEQ   .cmp_end@1
.cmp_true@0:
   LDA   #$01
.cmp_end@1:
   STA   __local_check__fail_0
   LDA   __zpabi_check__v3
   BMI   .sx_neg@3
   LDA   #$00
   BEQ   .sx_done@4
.sx_neg@3:
   LDA   #$FF
.sx_done@4:
   STA   __local_check__16
   LDA   __zpabi_check__v3
   BMI   .sx_neg@5
   LDA   #$00
   BEQ   .sx_done@6
.sx_neg@5:
   LDA   #$FF
.sx_done@6:
   STA   __local_check__10
   STA   __local_check__9
   STA   __local_check__8
   LDA   __zpabi_check__v3
   CLC
   SBC   #$FF
   LDA   __local_check__10
   SBC   #$00
   LDA   __local_check__9
   SBC   #$00
   LDA   __local_check__8
   SBC   #$00
   LDA   #$00
   ROL
   STA   __local_check__fail_1
   LDA   __local_check__fail_0
   SEC
   SBC   __local_check__fail_1
   STA   __local_check__7
   LDA   #$00
   SBC   #$00
   STA   __local_check__6
   LDA   __zpabi_check__v4_0
   ORA   __zpabi_check__v4_1
   ORA   __zpabi_check__v4_2
   ORA   __zpabi_check__v4_3
   ORA   __zpabi_check__v4_4
   ORA   __zpabi_check__v4_5
   ORA   __zpabi_check__v4_6
   ORA   __zpabi_check__v4_7
   BEQ   .and_false@1
   JSR   check@run0
   BEQ   .and_false@1
   LDA   #$01
   STA   __local_check__5
   LDA   #$00
   BEQ   .and_end@2
.and_false@1:
   LDA   #$00
   STA   __local_check__5
.and_end@2:
   ORA   #$00
   BPL   .sx_done@8
   LDA   #$FF
.sx_done@8:
   STA   __local_check__3
   STA   __local_check__2
   STA   __local_check__1
   STA   __local_check__0
   STA   __local_check__fail_0
   AND   #$07
   STA   __local_check__fail_1
   LDA   __zpabi_check__v4_0
   CLC
   SBC   __local_check__5
   LDA   __zpabi_check__v4_1
   SBC   #$00
   LDA   __zpabi_check__v4_2
   SBC   __local_check__3
   LDA   __zpabi_check__v4_3
   SBC   __local_check__2
   LDA   __zpabi_check__v4_4
   SBC   __local_check__1
   LDA   __zpabi_check__v4_5
   SBC   __local_check__0
   LDA   __zpabi_check__v4_6
   SBC   __local_check__fail_0
   LDA   __zpabi_check__v4_7
   SBC   __local_check__fail_1
   LDA   #$00
   ROL
   STA   __local_check__fail_1
   LDA   __local_check__7
   SEC
   SBC   __local_check__fail_1
   BCS   .carry_inc@0
   DEC   __local_check__6
.carry_inc@0:
   ORA   __local_check__6
   BNE   .lb_skip@0
   JMP   .check@asm_ssa_split@0
.lb_skip@0:
   LDA   #$02
   STA   __local_check__fail_0
   LDA   #$00
   STA   __local_check__fail_1
.if_end@3:
   LDA   __zpabi_check__v1_3
   BNE   .cmp_true@9
   LDA   __zpabi_check__v1_2
   BNE   .cmp_true@9
   LDA   __zpabi_check__v1_1
   CMP   __local_check__v0_1
   BNE   .cmp_true@9
   LDA   __zpabi_check__v1_0
   CMP   __local_check__v0_0
   BNE   .cmp_true@9
   LDA   #$00
   BEQ   .cmp_end@10
.cmp_true@9:
   LDA   #$01
.cmp_end@10:
   STA   __local_check__1
   BMI   .sx_neg@12
   LDA   #$00
   BEQ   .sx_done@13
.sx_neg@12:
   LDA   #$FF
.sx_done@13:
   AND   #$3F
   BNE   .jcmp_skip@14
   LDA   __local_check__1
   CMP   #$01
   BEQ   .if_end@4
.jcmp_skip@14:
   LDA   __local_check__fail_0
   ORA   #$04
   STA   __local_check__fail_0
   LDA   #$00
   STA   __local_check__fail_1
.if_end@4:
   LDA   __zpabi_check__v2_0
   SEC
   SBC   #$07
   STA   __local_check__2
   LDA   __zpabi_check__v2_1
   SBC   #$00
   STA   __local_check__3
   LDA   __local_check__8
   LSR
   LDA   __local_check__9
   ROR
   LDA   __local_check__10
   ROR
   LDA   __zpabi_check__v3
   ROR
   LDA   #$00
   ROR
   TAX
   BEQ   .shift_done@16
.shift_loop@15:
   LDA   __local_check__3
   ASL
   ROR   __local_check__3
   ROR   __local_check__2
   DEX
   BNE   .shift_loop@15
.shift_done@16:
   LDA   __zpabi_check__v5_0
   SEC
   SBC   #$7F
   STA   __local_check__1
   LDA   __zpabi_check__v5_1
   SBC   #$00
   STA   __local_check__0
   LDA   __local_check__3
   CMP   __local_check__0
   BNE   .cmp_true@17
   LDA   __local_check__2
   CMP   __local_check__1
   BNE   .cmp_true@17
   LDA   #$00
   BEQ   .cmp_end@18
.cmp_true@17:
   LDA   #$01
.cmp_end@18:
   STA   __local_check__0
   LDA   #$00
   BNE   .jcmp_skip@20
   LDA   __local_check__0
   CMP   #$01
   BEQ   .if_end@5
.jcmp_skip@20:
   LDA   __local_check__fail_0
   ORA   #$08
   STA   __local_check__fail_0
.if_end@5:
   JSR   check@run0
   BEQ   .and_false@6
   LDA   #$01
   STA   __local_check__7
   LDA   #$00
   BEQ   .and_end@7
.and_false@6:
   LDA   #$00
   STA   __local_check__7
.and_end@7:
   ORA   #$00
   BPL   .sx_done@22
   LDA   #$FF
.sx_done@22:
   LDX   __local_check__7
   STX   __local_check__15
   STA   __local_check__13
   STA   __local_check__12
   STA   __local_check__11
   STA   __local_check__10
   STA   __local_check__9
   AND   #$07
   STA   __local_check__8
   LDA   __zpabi_check__v4_0
   EOR   __zpabi_check__v4_0
   STA   __local_check__7
   LDA   __zpabi_check__v4_1
   EOR   __zpabi_check__v4_1
   STA   __local_check__6
   LDA   __zpabi_check__v4_2
   EOR   __zpabi_check__v4_2
   STA   __local_check__5
   LDA   __zpabi_check__v4_3
   EOR   __zpabi_check__v4_3
   STA   __local_check__4
   LDA   __zpabi_check__v4_4
   EOR   __zpabi_check__v4_4
   STA   __local_check__3
   LDA   __zpabi_check__v4_5
   EOR   __zpabi_check__v4_5
   STA   __local_check__2
   LDA   __zpabi_check__v4_6
   EOR   __zpabi_check__v4_6
   STA   __local_check__1
   LDA   __zpabi_check__v4_7
   EOR   __zpabi_check__v4_7
   STA   __local_check__0
   LDA   __local_check__7
   CLC
   SBC   __local_check__15
   LDA   __local_check__6
   SBC   #$00
   LDA   __local_check__5
   SBC   __local_check__13
   LDA   __local_check__4
   SBC   __local_check__12
   LDA   __local_check__3
   SBC   __local_check__11
   LDA   __local_check__2
   SBC   __local_check__10
   LDA   __local_check__1
   SBC   __local_check__9
   LDA   __local_check__0
   SBC   __local_check__8
   LDA   #$00
   ROL
   STA   __local_check__6
   LDA   #$00
   BPL   .sx_done@24
   LDA   #$FF
.sx_done@24:
   STA   __local_check__5
   STA   __local_check__4
   STA   __local_check__3
   STA   __local_check__2
   STA   __local_check__1
   LDA   __local_check__6
   STA   __local_check__7
   LDA   __local_check__5
   AND   #$07
   STA   __local_check__0
   LDA   __zpabi_check__v5_0
   CLC
   SBC   __local_check__7
   LDA   __zpabi_check__v5_1
   SBC   #$00
   LDA   __zpabi_check__v5_2
   SBC   __local_check__5
   LDA   __zpabi_check__v5_3
   SBC   __local_check__4
   LDA   __zpabi_check__v5_4
   SBC   __local_check__3
   LDA   __zpabi_check__v5_5
   SBC   __local_check__2
   LDA   __zpabi_check__v5_6
   SBC   __local_check__1
   LDA   __zpabi_check__v5_7
   SBC   __local_check__0
   LDA   #$00
   ROL
   STA   __local_check__0
   LDA   #$00
   BNE   .jcmp_skip@25
   LDA   __local_check__0
   CMP   #$01
   BEQ   .if_end@8
.jcmp_skip@25:
   LDA   __local_check__fail_0
   ORA   #$10
   STA   __local_check__fail_0
.if_end@8:
   LDA   __zpabi_check__v5_0
   ORA   __zpabi_check__v5_1
   ORA   __zpabi_check__v5_2
   ORA   __zpabi_check__v5_3
   ORA   __zpabi_check__v5_4
   ORA   __zpabi_check__v5_5
   ORA   __zpabi_check__v5_6
   ORA   __zpabi_check__v5_7
   BNE   .or_true@10
   SEC
   LDA   #$00
   SBC   __zpabi_check__v4_0
   STA   __local_check__7
   LDA   #$00
   SBC   __zpabi_check__v4_1
   STA   __local_check__6
   LDA   #$00
   SBC   __zpabi_check__v4_2
   STA   __local_check__5
   LDA   #$00
   SBC   __zpabi_check__v4_3
   STA   __local_check__4
   LDA   #$00
   SBC   __zpabi_check__v4_4
   STA   __local_check__3
   LDA   #$00
   SBC   __zpabi_check__v4_5
   STA   __local_check__2
   LDA   #$00
   SBC   __zpabi_check__v4_6
   STA   __local_check__1
   LDA   #$00
   SBC   __zpabi_check__v4_7
   AND   #$07
   STA   __local_check__0
   LDA   __local_check__7
   ORA   __local_check__6
   ORA   __local_check__5
   ORA   __local_check__4
   ORA   __local_check__3
   ORA   __local_check__2
   ORA   __local_check__1
   ORA   __local_check__0
   BNE   .or_true@10
   LDA   #$00
   STA   __local_check__7
   JMP   .or_end@11
.or_true@10:
   LDA   #$01
   STA   __local_check__7
   LDA   #$00
.or_end@11:
   ORA   #$00
   BPL   .sx_done@27
   LDA   #$FF
.sx_done@27:
   LDX   __local_check__7
   STX   __local_check__15
   LDX   #$00
   STX   __local_check__14
   STA   __local_check__13
   STA   __local_check__12
   STA   __local_check__11
   STA   __local_check__10
   STA   __local_check__9
   AND   #$07
   STA   __local_check__8
   LDA   __zpabi_check__v3
   EOR   #$FF
   STA   __local_check__7
   LDA   __local_check__16
   EOR   #$FF
   STA   __local_check__6
   BMI   .sx_neg@28
   LDA   #$00
   BEQ   .sx_done@29
.sx_neg@28:
   LDA   #$FF
.sx_done@29:
   LDX   __local_check__7
   STX   HARGS
   LDX   __local_check__6
   STX   HARGS+1
   STA   HARGS+2
   STA   HARGS+3
   STA   HARGS+4
   STA   HARGS+5
   STA   HARGS+6
   AND   #$07
   STA   HARGS+7
   LDA   __zpabi_check__v4_0
   CLC
   ADC   __local_check__v0_0
   STA   __local_check__7
   LDA   __zpabi_check__v4_1
   ADC   __local_check__v0_1
   STA   __local_check__6
   LDA   __zpabi_check__v4_2
   ADC   #$00
   STA   __local_check__5
   LDA   __zpabi_check__v4_3
   ADC   #$00
   STA   __local_check__4
   LDA   __zpabi_check__v4_4
   ADC   #$00
   STA   __local_check__3
   LDA   __zpabi_check__v4_5
   ADC   #$00
   STA   __local_check__2
   LDA   __zpabi_check__v4_6
   ADC   #$00
   STA   __local_check__1
   LDA   __zpabi_check__v4_7
   ADC   #$00
   LDX   __local_check__7
   STX   HARGS+8
   LDX   __local_check__6
   STX   HARGS+9
   LDX   __local_check__5
   STX   HARGS+10
   LDX   __local_check__4
   STX   HARGS+11
   LDX   __local_check__3
   STX   HARGS+12
   LDX   __local_check__2
   STX   HARGS+13
   LDX   __local_check__1
   STX   HARGS+14
   AND   #$07
   STA   HARGS+15
   JSR   udivmod64
   LDA   __local_check__8
   CMP   HARGS+23
   BNE   .jcmp_skip@30
   LDA   __local_check__9
   CMP   HARGS+22
   BNE   .jcmp_skip@30
   LDA   __local_check__10
   CMP   HARGS+21
   BNE   .jcmp_skip@30
   LDA   __local_check__11
   CMP   HARGS+20
   BNE   .jcmp_skip@30
   LDA   __local_check__12
   CMP   HARGS+19
   BNE   .jcmp_skip@30
   LDA   __local_check__13
   CMP   HARGS+18
   BNE   .jcmp_skip@30
   LDA   __local_check__14
   CMP   HARGS+17
   BNE   .jcmp_skip@30
   LDA   __local_check__15
   CMP   HARGS+16
   BEQ   .if_end@12
.jcmp_skip@30:
   LDA   __local_check__fail_0
   ORA   #$40
   STA   __local_check__fail_0
.if_end@12:
   LDA   __local_check__v0_0
   ORA   __local_check__v0_1
   BNE   .or_true@13
   LDA   __zpabi_check__v2_0
   SEC
   SBC   __zpabi_check__v4_0
   LDA   __zpabi_check__v2_1
   SBC   __zpabi_check__v4_1
   LDA   #$00
   SBC   __zpabi_check__v4_2
   LDA   #$00
   SBC   __zpabi_check__v4_3
   LDA   #$00
   SBC   __zpabi_check__v4_4
   LDA   #$00
   SBC   __zpabi_check__v4_5
   LDA   #$00
   SBC   __zpabi_check__v4_6
   LDA   #$00
   SBC   __zpabi_check__v4_7
   BCC   .or_true@13
   LDA   #$00
   STA   __local_check__1
   JMP   .or_end@14
.or_true@13:
   LDA   #$01
   STA   __local_check__1
.or_end@14:
   LDA   #$FF
   SEC
   SBC   __local_check__1
   LDA   #$FF
   SBC   #$00
   BVC   .jcmp_novf@31
   EOR   #$80
.jcmp_novf@31:
   BMI   .if_end@15
   LDA   __local_check__fail_0
   ORA   #$80
   STA   __local_check__fail_0
.if_end@15:
   LDX   __local_check__fail_1
   LDA   __local_check__fail_0
   RTS
.check@asm_ssa_split@0:
   LDA   #$00
   STA   __local_check__fail_0
   STA   __local_check__fail_1
   JMP   .if_end@3

check@run0:
   SUBROUTINE

   LDA   __zpabi_check__v1_0
   ORA   __zpabi_check__v1_1
   ORA   __zpabi_check__v1_2
   ORA   __zpabi_check__v1_3
   RTS

main:
   SUBROUTINE

   LDA   #$00
   STA   __zpabi_check__v1_0
   STA   __zpabi_check__v1_1
   STA   __zpabi_check__v1_2
   STA   __zpabi_check__v1_3
   STA   __zpabi_check__v3
   STA   __zpabi_check__v4_0
   STA   __zpabi_check__v4_1
   STA   __zpabi_check__v4_2
   STA   __zpabi_check__v4_3
   STA   __zpabi_check__v4_4
   STA   __zpabi_check__v4_5
   STA   __zpabi_check__v4_6
   LDA   #$54
   STA   __zpabi_check__v2_0
   LDA   #$2A
   STA   __zpabi_check__v2_1
   LDA   #$04
   STA   __zpabi_check__v4_7
   LDA   #$E2
   STA   __zpabi_check__v5_0
   LDA   #$E9
   STA   __zpabi_check__v5_1
   LDA   #$35
   STA   __zpabi_check__v5_2
   LDA   #$23
   STA   __zpabi_check__v5_3
   LDA   #$C8
   STA   __zpabi_check__v5_4
   LDA   #$D1
   STA   __zpabi_check__v5_5
   LDA   #$FC
   STA   __zpabi_check__v5_6
   LDA   #$07
   STA   __zpabi_check__v5_7
   LDX   #$20
   LDA   #$00
   JMP   check
