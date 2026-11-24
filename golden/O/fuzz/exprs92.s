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
__zpabi_check__v2	EQU	$8C
__zpabi_check__v3_0	EQU	$8D
__zpabi_check__v3_1	EQU	$8E
__zpabi_check__v4_0	EQU	$8F
__zpabi_check__v4_1	EQU	$90
__zpabi_check__v4_2	EQU	$91
__zpabi_check__v4_3	EQU	$92
__zpabi_check__v5_0	EQU	$93
__zpabi_check__v5_1	EQU	$94
__local_check__fail_1	EQU	$96
__local_check__fail_0	EQU	$97
__local_check__0	EQU	$98
__local_check__1	EQU	$99
__local_check__2	EQU	$9A
__local_check__3	EQU	$9B
__local_check__4	EQU	$9C
__local_check__5	EQU	$9D
__local_check__6	EQU	$9E
__local_check__7	EQU	$9F
__local_check__8	EQU	$A0

; @zp-link-meta-begin
; def check params=__zpabi_check__v0_0,__zpabi_check__v0_1,__zpabi_check__v0_2,__zpabi_check__v0_3,__zpabi_check__v0_4,__zpabi_check__v0_5,__zpabi_check__v0_6,__zpabi_check__v0_7,__zpabi_check__v1_0,__zpabi_check__v1_1,__zpabi_check__v1_2,__zpabi_check__v1_3,__zpabi_check__v2,__zpabi_check__v3_0,__zpabi_check__v3_1,__zpabi_check__v4_0,__zpabi_check__v4_1,__zpabi_check__v4_2,__zpabi_check__v4_3,__zpabi_check__v5_0,__zpabi_check__v5_1 locals=__local_check__fail_1,__local_check__fail_0,__local_check__0,__local_check__1,__local_check__2,__local_check__3,__local_check__4,__local_check__5,__local_check__6,__local_check__7,__local_check__8,__local_check__9,__local_check__10,__local_check__11,__local_check__12,__local_check__13,__local_check__14 indirect=false in_cycle=false
; def main params= locals=__local_main__0 indirect=false in_cycle=false
; call main -> check
; item check section=.text local=false zero=false
; item main section=.text local=false zero=false
; runtime hargs_size=32 hargs_need=32
; @zp-link-meta-end

check:
   SUBROUTINE

   LDA   #$00
   BNE   .lb_skip@1
   JMP   .check@asm_ssa_split@0
.lb_skip@1:
   LDA   #$01
   STA   __local_check__fail_0
   LDA   #$00
   STA   __local_check__fail_1
.if_end@0:
   LDA   __zpabi_check__v0_0
   STA   HARGS
   LDA   __zpabi_check__v0_1
   STA   HARGS+1
   LDA   __zpabi_check__v0_2
   STA   HARGS+2
   LDA   __zpabi_check__v0_3
   STA   HARGS+3
   LDA   __zpabi_check__v0_4
   STA   HARGS+4
   LDA   __zpabi_check__v0_5
   STA   HARGS+5
   LDA   __zpabi_check__v0_6
   STA   HARGS+6
   LDA   __zpabi_check__v0_7
   STA   HARGS+7
   LDA   #$FF
   STA   HARGS+8
   LDA   #$00
   STA   HARGS+9
   LDY   #$00
   STY   HARGS+10
   STY   HARGS+11
   STY   HARGS+12
   STY   HARGS+13
   STY   HARGS+14
   STA   HARGS+15
   JSR   sdivmod64
   LDA   __zpabi_check__v5_1
   BMI   .sx_neg@1
   LDA   #$00
   BEQ   .sx_done@2
.sx_neg@1:
   LDA   #$FF
.sx_done@2:
   STA   __local_check__5
   STA   __local_check__4
   STA   __local_check__3
   STA   __local_check__2
   STA   __local_check__1
   STA   __local_check__0
   LDA   __zpabi_check__v0_7
   CMP   __local_check__0
   BNE   .cmp_true@3
   LDA   __zpabi_check__v0_6
   CMP   __local_check__1
   BNE   .cmp_true@3
   LDA   __zpabi_check__v0_5
   CMP   __local_check__2
   BNE   .cmp_true@3
   LDA   __zpabi_check__v0_4
   CMP   __local_check__3
   BNE   .cmp_true@3
   LDA   __zpabi_check__v0_3
   CMP   __local_check__4
   BNE   .cmp_true@3
   LDA   __zpabi_check__v0_2
   CMP   __local_check__5
   BNE   .cmp_true@3
   LDA   __zpabi_check__v0_1
   CMP   __zpabi_check__v5_1
   BNE   .cmp_true@3
   LDA   __zpabi_check__v0_0
   CMP   __zpabi_check__v5_0
   BNE   .cmp_true@3
   LDA   #$00
   BEQ   .cmp_end@4
.cmp_true@3:
   LDA   #$01
.cmp_end@4:
   STA   __local_check__6
   LDA   #$00
   BPL   .sx_done@7
   LDA   #$FF
.sx_done@7:
   STA   __local_check__5
   STA   __local_check__4
   STA   __local_check__3
   STA   __local_check__2
   STA   __local_check__1
   STA   __local_check__0
   LDA   HARGS+31
   CMP   __local_check__0
   BNE   .cmp_true@8
   LDA   HARGS+30
   CMP   __local_check__1
   BNE   .cmp_true@8
   LDA   HARGS+29
   CMP   __local_check__2
   BNE   .cmp_true@8
   LDA   HARGS+28
   CMP   __local_check__3
   BNE   .cmp_true@8
   LDA   HARGS+27
   CMP   __local_check__4
   BNE   .cmp_true@8
   LDA   HARGS+26
   CMP   __local_check__5
   BNE   .cmp_true@8
   LDA   HARGS+25
   BNE   .cmp_true@8
   LDA   HARGS+24
   CMP   __local_check__6
   BNE   .cmp_true@8
   LDA   #$00
   BEQ   .cmp_end@9
.cmp_true@8:
   LDA   #$01
.cmp_end@9:
   STA   __local_check__0
   SEC
   LDA   #$00
   SBC   __local_check__0
   STA   __local_check__1
   LDA   #$00
   SBC   #$00
   ORA   __local_check__1
   BEQ   .if_end@1
   LDA   __local_check__fail_0
   ORA   #$02
   STA   __local_check__fail_0
.if_end@1:
   LDA   __zpabi_check__v4_0
   ORA   __zpabi_check__v4_1
   ORA   __zpabi_check__v4_2
   ORA   __zpabi_check__v4_3
   BNE   .lb_skip@0
   JMP   .if_end@4
.lb_skip@0:
   LDA   __zpabi_check__v2
   STA   HARGS
   LDA   #$00
   STA   HARGS+1
   LDY   #$00
   STY   HARGS+2
   STY   HARGS+3
   STY   HARGS+4
   STY   HARGS+5
   STY   HARGS+6
   STA   HARGS+7
   LDA   __zpabi_check__v0_0
   STA   HARGS+8
   LDA   __zpabi_check__v0_1
   STA   HARGS+9
   LDA   __zpabi_check__v0_2
   STA   HARGS+10
   LDA   __zpabi_check__v0_3
   STA   HARGS+11
   LDA   __zpabi_check__v0_4
   STA   HARGS+12
   LDA   __zpabi_check__v0_5
   STA   HARGS+13
   LDA   __zpabi_check__v0_6
   STA   HARGS+14
   LDA   __zpabi_check__v0_7
   STA   HARGS+15
   JSR   sdivmod64
   LDA   HARGS+25
   STA   __local_check__6
   LDA   HARGS+26
   STA   __local_check__5
   LDA   HARGS+27
   STA   __local_check__4
   LDA   HARGS+28
   STA   __local_check__3
   LDA   HARGS+29
   STA   __local_check__2
   LDA   HARGS+30
   STA   __local_check__1
   LDA   HARGS+31
   STA   __local_check__8
   LDA   HARGS+24
   CLC
   BCC   .carry_inc@0
   INC   __local_check__6
   BNE   .carry_inc@0
   INC   __local_check__5
   BNE   .carry_inc@0
   INC   __local_check__4
   BNE   .carry_inc@0
   INC   __local_check__3
   BNE   .carry_inc@0
   INC   __local_check__2
   BNE   .carry_inc@0
   INC   __local_check__1
   BNE   .carry_inc@0
   INC   __local_check__8
.carry_inc@0:
   AND   #$FF
   STA   __local_check__7
   LDA   __local_check__8
   AND   #$03
   EOR   #$02
   SEC
   SBC   #$02
   STA   __local_check__0
   LDA   __local_check__7
   ORA   __local_check__6
   ORA   __local_check__5
   ORA   __local_check__4
   ORA   __local_check__3
   ORA   __local_check__2
   ORA   __local_check__1
   ORA   __local_check__0
   BEQ   .if_end@4
   LDA   __local_check__fail_0
   ORA   #$04
   STA   __local_check__fail_0
.if_end@4:
   LDA   __zpabi_check__v4_3
   CMP   #$0F
   BNE   .jcmp_skip@11
   LDA   __zpabi_check__v4_2
   CMP   #$E9
   BNE   .jcmp_skip@11
   LDA   __zpabi_check__v4_1
   CMP   #$38
   BNE   .jcmp_skip@11
   LDA   __zpabi_check__v4_0
   CMP   #$EF
   BEQ   .vthread@0
.jcmp_skip@11:
   LDA   __local_check__fail_0
   ORA   #$08
   STA   __local_check__fail_0
   LDA   __zpabi_check__v4_0
.vthread@0:
   CLC
   SBC   #$00
   LDA   __zpabi_check__v4_1
   SBC   #$01
   LDA   __zpabi_check__v4_2
   SBC   #$00
   LDA   __zpabi_check__v4_3
   SBC   #$00
   LDA   #$00
   ROL
   STA   __local_check__2
   LDA   #$00
   BPL   .sx_done@13
   LDA   #$FF
.sx_done@13:
   STA   __local_check__1
   STA   __local_check__0
   LDA   __zpabi_check__v4_0
   SEC
   SBC   __local_check__2
   LDA   __zpabi_check__v4_1
   SBC   #$00
   LDA   __zpabi_check__v4_2
   SBC   __local_check__1
   LDA   __zpabi_check__v4_3
   SBC   __local_check__0
   LDA   #$00
   ROL
   STA   __local_check__0
   LDA   __zpabi_check__v5_0
   SEC
   SBC   __local_check__0
   STA   __local_check__1
   LDA   __zpabi_check__v5_1
   SBC   #$00
   BNE   .jcmp_skip@14
   LDA   __local_check__1
   CMP   #$08
   BEQ   .if_end@7
.jcmp_skip@14:
   LDA   __local_check__fail_0
   ORA   #$20
   STA   __local_check__fail_0
.if_end@7:
   LDA   __zpabi_check__v1_0
   EOR   #$FF
   STA   __local_check__3
   LDA   __zpabi_check__v1_1
   EOR   #$FF
   STA   __local_check__2
   LDA   __zpabi_check__v1_2
   EOR   #$FF
   STA   __local_check__1
   LDA   __zpabi_check__v1_3
   EOR   #$FF
   STA   __local_check__0
   LDA   __local_check__3
   ORA   __local_check__2
   ORA   __local_check__1
   ORA   __local_check__0
   BEQ   .lnot_true@15
   LDA   #$00
   BEQ   .lnot_end@16
.lnot_true@15:
   LDA   #$01
.lnot_end@16:
   STA   __local_check__4
   LDA   #$00
   BPL   .sx_done@18
   LDA   #$FF
.sx_done@18:
   STA   __local_check__3
   STA   __local_check__2
   STA   __local_check__1
   STA   __local_check__0
   LDA   #$01
   SEC
   SBC   __local_check__4
   LDA   #$00
   SBC   #$00
   LDA   #$00
   SBC   __local_check__3
   LDA   #$00
   SBC   __local_check__2
   LDA   #$00
   SBC   __local_check__1
   LDA   #$00
   SBC   __local_check__0
   BVC   .jcmp_novf@19
   EOR   #$80
.jcmp_novf@19:
   BPL   .if_end@8
   LDA   __local_check__fail_0
   ORA   #$40
   STA   __local_check__fail_0
.if_end@8:
   LDA   #$45
   ORA   __zpabi_check__v2
   STA   __local_check__6
   LDA   __zpabi_check__v4_3
   BNE   .thread@0
   LDA   __zpabi_check__v4_2
   BNE   .thread@0
   LDA   __zpabi_check__v4_1
   BNE   .thread@0
   LDA   __zpabi_check__v4_0
   CMP   #$FF
   BEQ   .cmp_true@20
.thread@0:
   LDA   #$00
   BEQ   .cmp_end@21
.cmp_true@20:
   LDA   #$01
.cmp_end@21:
   STA   __local_check__2
   LDA   #$00
   BPL   .sx_done@24
   LDA   #$FF
.sx_done@24:
   STA   __local_check__1
   STA   __local_check__0
   LDA   #$00
   CMP   __local_check__0
   BNE   .cmp_end@26
   LDA   #$01
   CMP   __local_check__1
   BNE   .thread@1
   LDA   #$23
   BNE   .thread@1
   LDA   __local_check__6
   CMP   __local_check__2
   BEQ   .cmp_true@25
.thread@1:
   LDA   #$00
   BEQ   .cmp_end@26
.cmp_true@25:
   LDA   #$01
.cmp_end@26:
   ORA   #$00
   BEQ   .if_end@9
   LDA   __local_check__fail_0
   ORA   #$80
   STA   __local_check__fail_0
.if_end@9:
   LDX   __local_check__fail_1
   LDA   __local_check__fail_0
   RTS
.check@asm_ssa_split@0:
   STA   __local_check__fail_0
   STA   __local_check__fail_1
   JMP   .if_end@0

main:
   SUBROUTINE

   LDA   #$01
   STA   __zpabi_check__v0_0
   LDA   #$FF
   STA   __zpabi_check__v2
   LDA   #$9C
   STA   __zpabi_check__v3_0
   LDA   #$D6
   STA   __zpabi_check__v3_1
   LDA   #$EF
   STA   __zpabi_check__v4_0
   LDA   #$38
   STA   __zpabi_check__v4_1
   LDA   #$E9
   STA   __zpabi_check__v4_2
   LDA   #$0F
   STA   __zpabi_check__v4_3
   LDA   #$09
   STA   __zpabi_check__v5_0
   LDA   #$00
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
   STA   __zpabi_check__v1_3
   STA   __zpabi_check__v5_1
   JMP   check
