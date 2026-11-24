SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__zpabi_check__v2_0	EQU	$80
__zpabi_check__v2_1	EQU	$81
__zpabi_check__v2_2	EQU	$82
__zpabi_check__v2_3	EQU	$83
__zpabi_check__v2_4	EQU	$84
__zpabi_check__v2_5	EQU	$85
__zpabi_check__v3_0	EQU	$86
__zpabi_check__v3_1	EQU	$87
__zpabi_check__v3_2	EQU	$88
__zpabi_check__v3_3	EQU	$89
__zpabi_check__v3_4	EQU	$8A
__zpabi_check__v3_5	EQU	$8B
__zpabi_check__v3_6	EQU	$8C
__zpabi_check__v3_7	EQU	$8D
__zpabi_check__v4_0	EQU	$8E
__zpabi_check__v4_1	EQU	$8F
__zpabi_check__v4_2	EQU	$90
__zpabi_check__v5_0	EQU	$91
__zpabi_check__v5_1	EQU	$92
__zpabi_check__v5_2	EQU	$93
__zpabi_check__v5_3	EQU	$94
__zpabi_check__v5_4	EQU	$95
__zpabi_check__v5_5	EQU	$96
__zpabi_check__v5_6	EQU	$97
__zpabi_check__v5_7	EQU	$98
__local_check__fail_1	EQU	$9A
__local_check__fail_0	EQU	$9B
__local_check__0	EQU	$9C
__local_check__1	EQU	$9D
__local_check__2	EQU	$9E
__local_check__3	EQU	$9F
__local_check__4	EQU	$A0
__local_check__5	EQU	$A1
__local_check__6	EQU	$A2
__local_check__7	EQU	$A3
__local_check__8	EQU	$A4
__local_check__9	EQU	$A5
__local_check__10	EQU	$A6
__local_check__11	EQU	$A7
__local_check__12	EQU	$A8
__local_check__13	EQU	$A9
__local_check__14	EQU	$AA
__local_check__15	EQU	$AB
__local_check__16	EQU	$AC
__local_check__v1	EQU	$AD
__local_check__v0	EQU	$AE

; @zp-link-meta-begin
; def check params=__zpabi_check__v0,__zpabi_check__v1,__zpabi_check__v2_0,__zpabi_check__v2_1,__zpabi_check__v2_2,__zpabi_check__v2_3,__zpabi_check__v2_4,__zpabi_check__v2_5,__zpabi_check__v3_0,__zpabi_check__v3_1,__zpabi_check__v3_2,__zpabi_check__v3_3,__zpabi_check__v3_4,__zpabi_check__v3_5,__zpabi_check__v3_6,__zpabi_check__v3_7,__zpabi_check__v4_0,__zpabi_check__v4_1,__zpabi_check__v4_2,__zpabi_check__v5_0,__zpabi_check__v5_1,__zpabi_check__v5_2,__zpabi_check__v5_3,__zpabi_check__v5_4,__zpabi_check__v5_5,__zpabi_check__v5_6,__zpabi_check__v5_7 locals=__local_check__fail_1,__local_check__fail_0,__local_check__0,__local_check__1,__local_check__2,__local_check__3,__local_check__4,__local_check__5,__local_check__6,__local_check__7,__local_check__8,__local_check__9,__local_check__10,__local_check__11,__local_check__12,__local_check__13,__local_check__14,__local_check__15,__local_check__16,__local_check__v1,__local_check__v0,__local_check__17,__local_check__18,__local_check__19,__local_check__20,__local_check__21 indirect=false in_cycle=false param_regs=A,X,,,,,,,,,,,,,,,,,,,,,,,,,
; def main params= locals=__local_main__0 indirect=false in_cycle=false
; call main -> check
; item check section=.text local=false zero=false
; item main section=.text local=false zero=false
; runtime hargs_size=32 hargs_need=32
; @zp-link-meta-end

check:
   SUBROUTINE

   STA   __local_check__v0
   STX   __local_check__v1
   LDA   __zpabi_check__v3_0
   ORA   __zpabi_check__v3_1
   ORA   __zpabi_check__v3_2
   ORA   __zpabi_check__v3_3
   ORA   __zpabi_check__v3_4
   ORA   __zpabi_check__v3_5
   ORA   __zpabi_check__v3_6
   ORA   __zpabi_check__v3_7
   BEQ   .lb_skip@1
   JMP   .check@asm_ssa_split@0
.lb_skip@1:
   LDA   __zpabi_check__v2_0
   ORA   __zpabi_check__v2_1
   ORA   __zpabi_check__v2_2
   ORA   __zpabi_check__v2_3
   ORA   __zpabi_check__v2_4
   ORA   __zpabi_check__v2_5
   BNE   .lb_skip@0
   JMP   .check@asm_ssa_split@0
.lb_skip@0:
   JMP   .check@asm_ssa_split@0
.if_end@4:
   LDA   __local_check__v0
   BMI   .sx_neg@3
   LDA   #$00
   BEQ   .sx_done@4
.sx_neg@3:
   LDA   #$FF
.sx_done@4:
   STA   __local_check__16
   LDA   __local_check__v0
   BMI   .jcmp_skip@7
   CMP   #$01
   BEQ   .if_end@5
.jcmp_skip@7:
   LDA   #$02
   STA   __local_check__fail_0
   LDA   #$00
   STA   __local_check__fail_1
.if_end@5:
   LDA   __zpabi_check__v5_0
   EOR   #$FF
   STA   HARGS
   LDA   __zpabi_check__v5_1
   EOR   #$FF
   STA   HARGS+1
   LDA   __zpabi_check__v5_2
   EOR   #$FF
   STA   HARGS+2
   LDA   __zpabi_check__v5_3
   EOR   #$FF
   STA   HARGS+3
   LDA   __zpabi_check__v5_4
   EOR   #$FF
   STA   HARGS+4
   LDA   __zpabi_check__v5_5
   EOR   #$FF
   STA   HARGS+5
   LDA   __zpabi_check__v5_6
   EOR   #$FF
   STA   HARGS+6
   LDA   __zpabi_check__v5_7
   EOR   #$FF
   STA   HARGS+7
   SEC
   LDA   #$00
   SBC   __zpabi_check__v3_0
   STA   __local_check__7
   LDA   #$00
   SBC   __zpabi_check__v3_1
   STA   __local_check__6
   LDA   #$00
   SBC   __zpabi_check__v3_2
   STA   __local_check__5
   LDA   #$00
   SBC   __zpabi_check__v3_3
   STA   __local_check__4
   LDA   #$00
   SBC   __zpabi_check__v3_4
   STA   __local_check__3
   LDA   #$00
   SBC   __zpabi_check__v3_5
   STA   __local_check__2
   LDA   #$00
   SBC   __zpabi_check__v3_6
   STA   __local_check__1
   LDA   #$00
   SBC   __zpabi_check__v3_7
   AND   #$3F
   LDX   __local_check__7
   STX   __local_check__15
   LDX   __local_check__6
   STX   __local_check__14
   LDX   __local_check__5
   STX   __local_check__13
   LDX   __local_check__4
   STX   __local_check__12
   LDX   __local_check__3
   STX   __local_check__11
   LDX   __local_check__2
   STX   __local_check__10
   LDX   __local_check__1
   STX   __local_check__9
   EOR   #$20
   SEC
   SBC   #$20
   STA   __local_check__8
   LDA   __local_check__15
   STA   HARGS+8
   LDA   __local_check__14
   STA   HARGS+9
   LDA   __local_check__13
   STA   HARGS+10
   LDA   __local_check__12
   STA   HARGS+11
   LDA   __local_check__11
   STA   HARGS+12
   LDA   __local_check__10
   STA   HARGS+13
   LDA   __local_check__9
   STA   HARGS+14
   LDA   __local_check__8
   STA   HARGS+15
   JSR   sdivmod64
   LDA   HARGS+24
   EOR   #$07
   STA   __local_check__7
   LDA   HARGS+31
   CMP   #$FF
   BNE   .jcmp_skip@8
   LDA   HARGS+30
   CMP   #$FF
   BNE   .jcmp_skip@8
   LDA   HARGS+29
   CMP   #$FF
   BNE   .jcmp_skip@8
   LDA   HARGS+28
   CMP   #$FF
   BNE   .jcmp_skip@8
   LDA   HARGS+27
   CMP   #$FF
   BNE   .jcmp_skip@8
   LDA   HARGS+26
   CMP   #$FF
   BNE   .jcmp_skip@8
   LDA   HARGS+25
   CMP   #$FF
   BNE   .jcmp_skip@8
   LDA   __local_check__7
   CMP   #$F8
   BEQ   .if_end@6
.jcmp_skip@8:
   LDA   __local_check__fail_0
   ORA   #$04
   STA   __local_check__fail_0
.if_end@6:
   SEC
   LDA   #$00
   SBC   __local_check__v0
   STA   __local_check__1
   LDA   #$00
   SBC   __local_check__16
   STA   __local_check__0
   BMI   .sx_neg@9
   LDA   #$00
   BEQ   .sx_done@10
.sx_neg@9:
   LDA   #$FF
.sx_done@10:
   STA   HARGS+10
   STA   HARGS+11
   STA   HARGS+12
   STA   HARGS+13
   STA   HARGS+14
   STA   HARGS+15
   LDA   __local_check__15
   STA   HARGS
   LDA   __local_check__14
   STA   HARGS+1
   LDA   __local_check__13
   STA   HARGS+2
   LDA   __local_check__12
   STA   HARGS+3
   LDA   __local_check__11
   STA   HARGS+4
   LDA   __local_check__10
   STA   HARGS+5
   LDA   __local_check__9
   STA   HARGS+6
   LDA   __local_check__8
   STA   HARGS+7
   LDA   __local_check__1
   STA   HARGS+8
   LDA   __local_check__0
   STA   HARGS+9
   JSR   sdivmod64
   LDA   #$FE
   CLC
   ADC   HARGS+24
   STA   __local_check__7
   LDA   #$FF
   ADC   HARGS+25
   STA   __local_check__6
   LDA   #$FF
   ADC   HARGS+26
   STA   __local_check__5
   LDA   #$FF
   ADC   HARGS+27
   STA   __local_check__4
   LDA   #$FF
   ADC   HARGS+28
   STA   __local_check__3
   LDA   #$FF
   ADC   HARGS+29
   STA   __local_check__2
   LDA   #$FF
   ADC   HARGS+30
   STA   __local_check__1
   LDA   #$FF
   ADC   HARGS+31
   AND   #$3F
   EOR   #$20
   SEC
   SBC   #$20
   CMP   #$FF
   BNE   .jcmp_skip@11
   LDA   __local_check__1
   CMP   #$FF
   BNE   .jcmp_skip@11
   LDA   __local_check__2
   CMP   #$FF
   BNE   .jcmp_skip@11
   LDA   __local_check__3
   CMP   #$FF
   BNE   .jcmp_skip@11
   LDA   __local_check__4
   CMP   #$FF
   BNE   .jcmp_skip@11
   LDA   __local_check__5
   CMP   #$FF
   BNE   .jcmp_skip@11
   LDA   __local_check__6
   CMP   #$FF
   BNE   .jcmp_skip@11
   LDA   __local_check__7
   CMP   #$FE
   BEQ   .if_end@7
.jcmp_skip@11:
   LDA   __local_check__fail_0
   ORA   #$08
   STA   __local_check__fail_0
.if_end@7:
   LDA   __zpabi_check__v5_0
   ORA   __zpabi_check__v5_1
   ORA   __zpabi_check__v5_2
   ORA   __zpabi_check__v5_3
   ORA   __zpabi_check__v5_4
   ORA   __zpabi_check__v5_5
   ORA   __zpabi_check__v5_6
   ORA   __zpabi_check__v5_7
   BNE   .or_true@8
   LDA   __zpabi_check__v3_0
   SEC
   SBC   __zpabi_check__v5_0
   LDA   __zpabi_check__v3_1
   SBC   __zpabi_check__v5_1
   LDA   __zpabi_check__v3_2
   SBC   __zpabi_check__v5_2
   LDA   __zpabi_check__v3_3
   SBC   __zpabi_check__v5_3
   LDA   __zpabi_check__v3_4
   SBC   __zpabi_check__v5_4
   LDA   __zpabi_check__v3_5
   SBC   __zpabi_check__v5_5
   LDA   __zpabi_check__v3_6
   SBC   __zpabi_check__v5_6
   LDA   __zpabi_check__v3_7
   SBC   __zpabi_check__v5_7
   BVC   .jcmp_novf@12
   EOR   #$80
.jcmp_novf@12:
   BPL   .or_true@8
   LDA   #$00
   STA   __local_check__0
   STA   __local_check__1
   JMP   .or_end@9
.or_true@8:
   LDA   #$01
   STA   __local_check__0
   LDA   #$00
   STA   __local_check__1
.or_end@9:
   LDA   __zpabi_check__v5_7
   CMP   __zpabi_check__v5_7
   BNE   .cmp_true@13
   LDA   __zpabi_check__v5_6
   CMP   __zpabi_check__v5_6
   BNE   .cmp_true@13
   LDA   __zpabi_check__v5_5
   CMP   __zpabi_check__v5_5
   BNE   .cmp_true@13
   LDA   __zpabi_check__v5_4
   CMP   __zpabi_check__v5_4
   BNE   .cmp_true@13
   LDA   __zpabi_check__v5_3
   CMP   __zpabi_check__v5_3
   BNE   .cmp_true@13
   LDA   __zpabi_check__v5_2
   CMP   __zpabi_check__v5_2
   BNE   .cmp_true@13
   LDA   __zpabi_check__v5_1
   CMP   __zpabi_check__v5_1
   BNE   .cmp_true@13
   LDA   __zpabi_check__v5_0
   CMP   __zpabi_check__v5_0
   BNE   .cmp_true@13
   LDA   #$00
   BEQ   .cmp_end@14
.cmp_true@13:
   LDA   #$01
.cmp_end@14:
   STA   __local_check__2
   LDA   #$00
   BPL   .sx_done@17
   LDA   #$FF
.sx_done@17:
   STA   HARGS+2
   STA   HARGS+3
   STA   HARGS+4
   STA   HARGS+5
   STA   HARGS+6
   STA   HARGS+7
   LDA   __local_check__2
   STA   HARGS
   LDA   #$00
   STA   HARGS+1
   LDA   __local_check__15
   STA   HARGS+8
   LDA   __local_check__14
   STA   HARGS+9
   LDA   __local_check__13
   STA   HARGS+10
   LDA   __local_check__12
   STA   HARGS+11
   LDA   __local_check__11
   STA   HARGS+12
   LDA   __local_check__10
   STA   HARGS+13
   LDA   __local_check__9
   STA   HARGS+14
   LDA   __local_check__8
   STA   HARGS+15
   JSR   mul64
   LDX   HARGS+16
   BEQ   .shift_done@19
.shift_loop@18:
   ASL   __local_check__0
   ROL   __local_check__1
   DEX
   BNE   .shift_loop@18
.shift_done@19:
   LDA   __local_check__1
   BNE   .jcmp_skip@20
   LDA   __local_check__0
   CMP   #$01
   BEQ   .if_end@10
.jcmp_skip@20:
   LDA   __local_check__fail_0
   ORA   #$10
   STA   __local_check__fail_0
.if_end@10:
   LDA   __local_check__v1
   BMI   .sx_neg@21
   LDA   #$00
   BEQ   .sx_done@22
.sx_neg@21:
   LDA   #$FF
.sx_done@22:
   STA   __local_check__6
   STA   __local_check__5
   STA   __local_check__4
   LDA   __local_check__v0
   BMI   .sx_neg@23
   LDA   #$00
   BEQ   .sx_done@24
.sx_neg@23:
   LDA   #$FF
.sx_done@24:
   STA   __local_check__2
   STA   __local_check__1
   LDA   __local_check__v0
   STA   __local_check__3
   LDA   __local_check__2
   AND   #$7F
   STA   __local_check__0
   LDA   __local_check__v1
   EOR   __local_check__3
   STA   __local_check__3
   LDA   __local_check__6
   EOR   __local_check__2
   STA   __local_check__2
   LDA   __local_check__5
   EOR   __local_check__1
   STA   __local_check__1
   LDA   __local_check__4
   EOR   __local_check__0
   CMP   #$FF
   BNE   .jcmp_skip@25
   LDA   __local_check__1
   CMP   #$FF
   BNE   .jcmp_skip@25
   LDA   __local_check__2
   CMP   #$FF
   BNE   .jcmp_skip@25
   LDA   __local_check__3
   CMP   #$81
   BEQ   .if_end@11
.jcmp_skip@25:
   LDA   __local_check__fail_0
   ORA   #$20
   STA   __local_check__fail_0
.if_end@11:
   LDA   __local_check__v0
   CMP   #$01
   BEQ   .if_end@12
   LDA   __local_check__fail_0
   ORA   #$40
   STA   __local_check__fail_0
.if_end@12:
   LDA   #$81
   STA   HARGS
   LDA   #$00
   STA   HARGS+1
   LDY   #$00
   STY   HARGS+2
   STY   HARGS+3
   STY   HARGS+4
   STA   HARGS+5
   ASL
   TYA
   ADC   #$FF
   EOR   #$FF
   STA   HARGS+6
   STA   HARGS+7
   LDA   __zpabi_check__v4_0
   STA   HARGS+8
   LDA   __zpabi_check__v4_1
   STA   HARGS+9
   LDA   __zpabi_check__v4_2
   STA   HARGS+10
   TYA
   STA   HARGS+11
   STY   HARGS+12
   STA   HARGS+13
   ASL
   TYA
   ADC   #$FF
   EOR   #$FF
   STA   HARGS+14
   STA   HARGS+15
   JSR   sdivmod64
   LDA   HARGS+21
   AND   #$01
   EOR   #$01
   SEC
   SBC   #$01
   STA   __local_check__6
   LDA   __zpabi_check__v2_5
   BMI   .sx_neg@26
   LDA   #$00
   BEQ   .sx_done@27
.sx_neg@26:
   LDA   #$FF
.sx_done@27:
   STA   __local_check__1
   STA   __local_check__0
   LDA   __zpabi_check__v2_0
   SEC
   SBC   __zpabi_check__v3_0
   LDA   __zpabi_check__v2_1
   SBC   __zpabi_check__v3_1
   LDA   __zpabi_check__v2_2
   SBC   __zpabi_check__v3_2
   LDA   __zpabi_check__v2_3
   SBC   __zpabi_check__v3_3
   LDA   __zpabi_check__v2_4
   SBC   __zpabi_check__v3_4
   LDA   __zpabi_check__v2_5
   SBC   __zpabi_check__v3_5
   LDA   __local_check__1
   SBC   __zpabi_check__v3_6
   LDA   __local_check__0
   SBC   __zpabi_check__v3_7
   BVC   .cmp_novf@28
   EOR   #$80
.cmp_novf@28:
   ASL
   LDA   #$00
   ROL
   STA   __local_check__1
   LDA   #$00
   BPL   .sx_done@30
   LDA   #$FF
.sx_done@30:
   LDX   __local_check__1
   STX   __local_check__5
   AND   #$1F
   STA   __local_check__3
   SEC
   LDA   #$00
   SBC   __zpabi_check__v4_0
   STA   __local_check__2
   LDA   #$00
   SBC   __zpabi_check__v4_1
   STA   __local_check__1
   LDA   #$00
   SBC   __zpabi_check__v4_2
   AND   #$1F
   STA   __local_check__0
   LDA   __local_check__5
   EOR   __local_check__2
   STA   __local_check__2
   LDA   #$00
   EOR   __local_check__1
   STA   __local_check__1
   LDA   __local_check__3
   EOR   __local_check__0
   STA   __local_check__0
   LDA   HARGS+16
   ORA   __local_check__2
   STA   __local_check__5
   LDA   HARGS+17
   ORA   __local_check__1
   STA   __local_check__4
   LDA   HARGS+18
   ORA   __local_check__0
   STA   __local_check__3
   LDA   __local_check__6
   BNE   .jcmp_skip@31
   LDA   HARGS+20
   BNE   .jcmp_skip@31
   LDA   HARGS+19
   BNE   .jcmp_skip@31
   LDA   __local_check__3
   CMP   #$10
   BNE   .jcmp_skip@31
   LDA   __local_check__4
   BNE   .jcmp_skip@31
   LDA   __local_check__5
   CMP   #$01
   BEQ   .if_end@13
.jcmp_skip@31:
   LDA   __local_check__fail_0
   ORA   #$80
   STA   __local_check__fail_0
.if_end@13:
   LDX   __local_check__fail_1
   LDA   __local_check__fail_0
   RTS
.check@asm_ssa_split@0:
   LDA   #$00
   STA   __local_check__fail_0
   STA   __local_check__fail_1
   JMP   .if_end@4

main:
   SUBROUTINE

   LDA   #$FF
   STA   __zpabi_check__v2_0
   STA   __zpabi_check__v2_1
   STA   __zpabi_check__v2_2
   STA   __zpabi_check__v2_3
   STA   __zpabi_check__v2_4
   STA   __zpabi_check__v2_5
   LDA   #$0B
   STA   __zpabi_check__v3_0
   LDA   #$10
   STA   __zpabi_check__v4_2
   LDA   #$00
   STA   __zpabi_check__v3_1
   STA   __zpabi_check__v3_2
   STA   __zpabi_check__v3_3
   STA   __zpabi_check__v3_4
   STA   __zpabi_check__v3_5
   STA   __zpabi_check__v3_6
   STA   __zpabi_check__v3_7
   STA   __zpabi_check__v4_0
   STA   __zpabi_check__v4_1
   STA   __zpabi_check__v5_0
   STA   __zpabi_check__v5_1
   STA   __zpabi_check__v5_2
   STA   __zpabi_check__v5_3
   STA   __zpabi_check__v5_4
   STA   __zpabi_check__v5_5
   STA   __zpabi_check__v5_6
   STA   __zpabi_check__v5_7
   LDX   #$80
   LDA   #$01
   JMP   check
