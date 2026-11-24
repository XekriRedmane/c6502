SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__zpabi_check__v0_0	EQU	$80
__zpabi_check__v0_1	EQU	$81
__zpabi_check__v0_2	EQU	$82
__zpabi_check__v0_3	EQU	$83
__zpabi_check__v0_4	EQU	$84
__zpabi_check__v0_5	EQU	$85
__zpabi_check__v0_6	EQU	$86
__zpabi_check__v0_7	EQU	$87
__zpabi_check__v1_0	EQU	$88
__zpabi_check__v1_1	EQU	$89
__zpabi_check__v1_2	EQU	$8A
__zpabi_check__v1_3	EQU	$8B
__zpabi_check__v2_0	EQU	$8C
__zpabi_check__v2_1	EQU	$8D
__zpabi_check__v2_2	EQU	$8E
__zpabi_check__v2_3	EQU	$8F
__zpabi_check__v2_4	EQU	$90
__zpabi_check__v3_0	EQU	$91
__zpabi_check__v3_1	EQU	$92
__zpabi_check__v3_2	EQU	$93
__zpabi_check__v3_3	EQU	$94
__zpabi_check__v3_4	EQU	$95
__zpabi_check__v3_5	EQU	$96
__zpabi_check__v3_6	EQU	$97
__zpabi_check__v3_7	EQU	$98
__zpabi_check__v4_0	EQU	$99
__zpabi_check__v4_1	EQU	$9A
__zpabi_check__v4_2	EQU	$9B
__zpabi_check__v4_3	EQU	$9C
__zpabi_check__v5	EQU	$9D
__local_check__fail_1	EQU	$9F
__local_check__fail_0	EQU	$A0
__local_check__0	EQU	$A1
__local_check__1	EQU	$A2
__local_check__4	EQU	$A5
__local_check__5	EQU	$A6
__local_check__6	EQU	$A7
__local_check__7	EQU	$A8
__local_check__8	EQU	$A9
__local_check__9	EQU	$AA
__local_check__10	EQU	$AB

; @zp-link-meta-begin
; def check params=__zpabi_check__v0_0,__zpabi_check__v0_1,__zpabi_check__v0_2,__zpabi_check__v0_3,__zpabi_check__v0_4,__zpabi_check__v0_5,__zpabi_check__v0_6,__zpabi_check__v0_7,__zpabi_check__v1_0,__zpabi_check__v1_1,__zpabi_check__v1_2,__zpabi_check__v1_3,__zpabi_check__v2_0,__zpabi_check__v2_1,__zpabi_check__v2_2,__zpabi_check__v2_3,__zpabi_check__v2_4,__zpabi_check__v3_0,__zpabi_check__v3_1,__zpabi_check__v3_2,__zpabi_check__v3_3,__zpabi_check__v3_4,__zpabi_check__v3_5,__zpabi_check__v3_6,__zpabi_check__v3_7,__zpabi_check__v4_0,__zpabi_check__v4_1,__zpabi_check__v4_2,__zpabi_check__v4_3,__zpabi_check__v5 locals=__local_check__fail_1,__local_check__fail_0,__local_check__0,__local_check__1,__local_check__2,__local_check__3,__local_check__4,__local_check__5,__local_check__6,__local_check__7,__local_check__8,__local_check__9,__local_check__10,__local_check__11,__local_check__12,__local_check__13,__local_check__14,__local_check__15 indirect=false in_cycle=false changes=__zpabi_check__v3_0
; def main params= locals=__local_main__0 indirect=false in_cycle=false
; call main -> check
; item check section=.text local=false zero=false
; item main section=.text local=false zero=false
; runtime hargs_size=32 hargs_need=24
; @zp-link-meta-end

check:
   SUBROUTINE

   LDA   #$00
   CMP   __zpabi_check__v2_4
   BNE   .cmp_end@1
   CMP   __zpabi_check__v2_3
   BNE   .cmp_end@1
   CMP   __zpabi_check__v2_2
   BNE   .cmp_end@1
   LDA   #$80
   CMP   __zpabi_check__v2_1
   BNE   .thread@0
   LDA   #$00
   CMP   __zpabi_check__v2_0
   BEQ   .cmp_true@0
.thread@0:
   LDA   #$00
   BEQ   .cmp_end@1
.cmp_true@0:
   LDA   #$01
.cmp_end@1:
   EOR   #$01
   STA   __local_check__fail_0
   LDA   __zpabi_check__v3_0
   SEC
   SBC   __local_check__fail_0
   LDA   #$00
   SBC   #$00
   BVC   .jcmp_novf@3
   EOR   #$80
.jcmp_novf@3:
   BPL   .lb_skip@0
   JMP   .check@asm_ssa_split@0
.lb_skip@0:
   LDA   #$02
   STA   __local_check__fail_0
   LDA   #$00
   STA   __local_check__fail_1
.if_end@1:
   LDA   __zpabi_check__v4_0
   STA   HARGS
   LDA   #$FF
   STA   HARGS+1
   JSR   mul8
   SEC
   LDA   #$00
   SBC   HARGS+2
   STA   __local_check__1
   LDA   #$00
   SBC   #$00
   CMP   #$FF
   BNE   .jcmp_skip@4
   LDA   __local_check__1
   CMP   #$01
   BEQ   .if_end@2
.jcmp_skip@4:
   LDA   __local_check__fail_0
   ORA   #$04
   STA   __local_check__fail_0
.if_end@2:
   LDA   __zpabi_check__v3_1
   AND   #$7F
   STA   __local_check__10
   LDA   #$00
   STA   __local_check__9
   LDY   #$00
   STY   __local_check__8
   STY   __local_check__7
   STY   __local_check__6
   STA   __local_check__5
   LDA   __zpabi_check__v5
   STA   HARGS
   TYA
   STA   __local_check__4
   STA   HARGS+1
   STY   HARGS+2
   STA   HARGS+3
   LDA   __zpabi_check__v4_0
   STA   HARGS+4
   LDA   __zpabi_check__v4_1
   STA   HARGS+5
   LDA   __zpabi_check__v4_2
   STA   HARGS+6
   LDA   __zpabi_check__v4_3
   STA   HARGS+7
   JSR   mul32
   LDA   __zpabi_check__v3_0
   CLC
   SBC   HARGS+8
   LDA   __local_check__10
   SBC   HARGS+9
   LDA   __local_check__9
   SBC   HARGS+10
   LDA   __local_check__8
   SBC   HARGS+11
   LDA   __local_check__7
   SBC   #$00
   LDA   __local_check__6
   SBC   #$00
   LDA   __local_check__5
   SBC   #$00
   LDA   __local_check__4
   SBC   #$00
   LDA   #$00
   ROL
   EOR   #$01
   STA   __local_check__1
   LDA   #$00
   BNE   .jcmp_skip@5
   LDA   __local_check__1
   CMP   #$01
   BEQ   .if_end@4
.jcmp_skip@5:
   LDA   __local_check__fail_0
   ORA   #$10
   STA   __local_check__fail_0
.if_end@4:
   LDA   __zpabi_check__v3_0
   CLC
   ADC   #$FF
   STA   __local_check__1
   LDA   __zpabi_check__v3_1
   ADC   #$7F
   STA   HARGS+3
   LDA   __local_check__1
   STA   HARGS+2
   LDA   #$01
   STA   HARGS
   LDA   #$00
   STA   HARGS+1
   JSR   sdivmod16
   LDA   HARGS+4
   ORA   HARGS+5
   BEQ   .if_end@7
   LDA   __local_check__fail_0
   ORA   #$20
   STA   __local_check__fail_0
.if_end@7:
   LDA   __zpabi_check__v1_0
   STA   HARGS
   LDA   __zpabi_check__v1_1
   STA   HARGS+1
   LDA   __zpabi_check__v1_2
   STA   HARGS+2
   LDA   __zpabi_check__v1_3
   STA   HARGS+3
   LDA   #$00
   STA   HARGS+4
   LDY   #$00
   STY   HARGS+5
   STY   HARGS+6
   STA   HARGS+7
   LDA   __zpabi_check__v3_0
   STA   HARGS+8
   LDA   __zpabi_check__v3_1
   STA   HARGS+9
   LDA   __zpabi_check__v3_2
   STA   HARGS+10
   LDA   __zpabi_check__v3_3
   STA   HARGS+11
   LDA   __zpabi_check__v3_4
   STA   HARGS+12
   LDA   __zpabi_check__v3_5
   STA   HARGS+13
   LDA   __zpabi_check__v3_6
   STA   HARGS+14
   LDA   __zpabi_check__v3_7
   STA   HARGS+15
   JSR   udivmod64
   LDA   HARGS+16
   ORA   #$7F
   STA   __local_check__7
   LDA   #$FF
   CLC
   SBC   __local_check__7
   LDA   #$FF
   SBC   HARGS+17
   LDA   #$FF
   SBC   HARGS+18
   LDA   #$7F
   SBC   HARGS+19
   LDA   #$00
   SBC   HARGS+20
   LDA   #$00
   SBC   HARGS+21
   LDA   #$00
   SBC   HARGS+22
   LDA   #$00
   SBC   HARGS+23
   LDA   #$00
   ROL
   STA   __local_check__0
   LDA   #$00
   BNE   .jcmp_skip@6
   LDA   __local_check__0
   CMP   #$01
   BEQ   .if_end@8
.jcmp_skip@6:
   LDA   __local_check__fail_0
   ORA   #$40
   STA   __local_check__fail_0
.if_end@8:
   LDA   __zpabi_check__v2_0
   CMP   #$DD
   BEQ   .if_end@9
   LDA   __local_check__fail_0
   ORA   #$80
   STA   __local_check__fail_0
.if_end@9:
   LDX   __local_check__fail_1
   LDA   __local_check__fail_0
   RTS
.check@asm_ssa_split@0:
   LDA   #$00
   STA   __local_check__fail_0
   STA   __local_check__fail_1
   JMP   .if_end@1

main:
   SUBROUTINE

   LDA   #$00
   STA   __zpabi_check__v0_0
   STA   __zpabi_check__v0_1
   STA   __zpabi_check__v0_2
   STA   __zpabi_check__v0_3
   STA   __zpabi_check__v0_4
   STA   __zpabi_check__v0_5
   STA   __zpabi_check__v0_6
   STA   __zpabi_check__v0_7
   STA   __zpabi_check__v1_0
   STA   __zpabi_check__v1_1
   STA   __zpabi_check__v1_2
   STA   __zpabi_check__v2_4
   STA   __zpabi_check__v3_0
   STA   __zpabi_check__v3_1
   STA   __zpabi_check__v3_2
   STA   __zpabi_check__v3_3
   STA   __zpabi_check__v3_4
   STA   __zpabi_check__v3_5
   STA   __zpabi_check__v3_6
   STA   __zpabi_check__v4_1
   STA   __zpabi_check__v4_2
   STA   __zpabi_check__v4_3
   LDA   #$80
   STA   __zpabi_check__v1_3
   LDA   #$DD
   STA   __zpabi_check__v2_0
   LDA   #$C1
   STA   __zpabi_check__v2_1
   LDA   #$2F
   STA   __zpabi_check__v2_2
   LDA   #$F7
   STA   __zpabi_check__v2_3
   LDA   #$10
   STA   __zpabi_check__v3_7
   LDA   #$01
   STA   __zpabi_check__v4_0
   STA   __zpabi_check__v5
   JMP   check
