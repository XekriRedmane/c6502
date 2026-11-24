SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__zpabi_check__v0_0	EQU	$80
__zpabi_check__v0_1	EQU	$81
__zpabi_check__v0_2	EQU	$82
__zpabi_check__v0_3	EQU	$83
__zpabi_check__v1_0	EQU	$84
__zpabi_check__v1_1	EQU	$85
__zpabi_check__v1_2	EQU	$86
__zpabi_check__v1_3	EQU	$87
__zpabi_check__v1_4	EQU	$88
__zpabi_check__v2	EQU	$89
__zpabi_check__v3_0	EQU	$8A
__zpabi_check__v3_1	EQU	$8B
__zpabi_check__v4	EQU	$8C
__zpabi_check__v5_0	EQU	$8D
__zpabi_check__v5_1	EQU	$8E
__zpabi_check__v5_2	EQU	$8F
__zpabi_check__v5_3	EQU	$90
__local_check__fail_1	EQU	$92
__local_check__fail_0	EQU	$93
__local_check__0	EQU	$94
__local_check__1	EQU	$95
__local_check__2	EQU	$96
__local_check__3	EQU	$97
__local_check__4	EQU	$98
__local_check__5	EQU	$99
__local_check__6	EQU	$9A
__local_check__7	EQU	$9B
__local_check__8	EQU	$9C
__local_check__9	EQU	$9D

; @zp-link-meta-begin
; def check params=__zpabi_check__v0_0,__zpabi_check__v0_1,__zpabi_check__v0_2,__zpabi_check__v0_3,__zpabi_check__v1_0,__zpabi_check__v1_1,__zpabi_check__v1_2,__zpabi_check__v1_3,__zpabi_check__v1_4,__zpabi_check__v2,__zpabi_check__v3_0,__zpabi_check__v3_1,__zpabi_check__v4,__zpabi_check__v5_0,__zpabi_check__v5_1,__zpabi_check__v5_2,__zpabi_check__v5_3 locals=__local_check__fail_1,__local_check__fail_0,__local_check__0,__local_check__1,__local_check__2,__local_check__3,__local_check__4,__local_check__5,__local_check__6,__local_check__7,__local_check__8,__local_check__9,__local_check__10,__local_check__11,__local_check__12,__local_check__13,__local_check__14,__local_check__15,__local_check__16,__local_check__17 indirect=false in_cycle=false
; def main params= locals=__local_main__0 indirect=false in_cycle=false
; call main -> check
; item check section=.text local=false zero=false
; item check@run0 section=.text local=true zero=false
; item main section=.text local=false zero=false
; runtime hargs_size=32 hargs_need=29
; @zp-link-meta-end

check:
   SUBROUTINE

   LDA   __zpabi_check__v1_0
   SEC
   SBC   #$39
   LDA   __zpabi_check__v1_1
   SBC   #$66
   LDA   __zpabi_check__v1_2
   SBC   #$8B
   LDA   __zpabi_check__v1_3
   SBC   #$E8
   LDA   __zpabi_check__v1_4
   SBC   #$07
   LDA   #$00
   SBC   #$F8
   BVC   .cmp_novf@0
   EOR   #$80
.cmp_novf@0:
   ASL
   LDA   #$00
   ROL
   STA   __local_check__fail_1
   LDA   #$00
   BPL   .sx_done@2
   LDA   #$FF
.sx_done@2:
   STA   HARGS+2
   STA   HARGS+3
   LDA   __local_check__fail_1
   STA   HARGS
   LDA   #$00
   STA   HARGS+1
   LDA   __zpabi_check__v5_0
   STA   HARGS+4
   LDA   __zpabi_check__v5_1
   STA   HARGS+5
   LDA   __zpabi_check__v5_2
   STA   HARGS+6
   LDA   __zpabi_check__v5_3
   STA   HARGS+7
   JSR   udivmod32
   LDA   HARGS+8
   ORA   HARGS+9
   ORA   HARGS+10
   ORA   HARGS+11
   BNE   .lb_skip@1
   JMP   .check@asm_ssa_split@0
.lb_skip@1:
   LDA   #$01
   STA   __local_check__fail_0
   LDA   #$00
   STA   __local_check__fail_1
.if_end@0:
   LDA   __zpabi_check__v1_0
   STA   HARGS
   LDA   __zpabi_check__v1_1
   STA   HARGS+1
   LDA   __zpabi_check__v1_2
   STA   HARGS+2
   LDA   __zpabi_check__v1_3
   STA   HARGS+3
   LDA   __zpabi_check__v1_4
   JSR   check@run0
   STA   HARGS+8
   JSR   asl64
   LDA   HARGS+9
   STA   __local_check__6
   LDA   HARGS+10
   STA   __local_check__5
   LDA   HARGS+11
   STA   __local_check__4
   LDA   HARGS+12
   STA   __local_check__8
   LDA   HARGS+13
   AND   #$03
   STA   __local_check__7
   LDA   __zpabi_check__v3_1
   BMI   .sx_neg@3
   LDA   #$00
   BEQ   .sx_done@4
.sx_neg@3:
   LDA   #$FF
.sx_done@4:
   STA   HARGS+6
   STA   HARGS+7
   LDA   __zpabi_check__v5_0
   STA   HARGS
   LDA   __zpabi_check__v5_1
   STA   HARGS+1
   LDA   __zpabi_check__v5_2
   STA   HARGS+2
   LDA   __zpabi_check__v5_3
   STA   HARGS+3
   LDA   __zpabi_check__v3_0
   STA   HARGS+4
   LDA   __zpabi_check__v3_1
   STA   HARGS+5
   JSR   mul32
   LDA   __local_check__6
   ORA   HARGS+8
   STA   __local_check__6
   LDA   __local_check__5
   ORA   HARGS+9
   STA   __local_check__5
   LDA   __local_check__4
   ORA   HARGS+10
   STA   __local_check__4
   LDA   __local_check__8
   ORA   HARGS+11
   STA   __local_check__3
   LDA   __local_check__7
   STA   __local_check__2
   LDA   __zpabi_check__v2
   AND   __zpabi_check__v4
   STA   __local_check__1
   LDA   __zpabi_check__v0_0
   BMI   .sx_neg@5
   LDA   #$00
   BEQ   .sx_done@6
.sx_neg@5:
   LDA   #$FF
.sx_done@6:
   STA   __local_check__0
   LDA   #$00
   CMP   __local_check__0
   BNE   .cmp_end@8
   LDA   __local_check__1
   CMP   __zpabi_check__v0_0
   BEQ   .cmp_true@7
   LDA   #$00
   BEQ   .cmp_end@8
.cmp_true@7:
   LDA   #$01
.cmp_end@8:
   STA   HARGS+8
   LDA   __local_check__6
   STA   HARGS
   LDA   __local_check__5
   STA   HARGS+1
   LDA   __local_check__4
   STA   HARGS+2
   LDA   __local_check__3
   STA   HARGS+3
   LDA   __local_check__2
   JSR   check@run0
   JSR   asl64
   LDA   HARGS+13
   AND   #$03
   CMP   #$02
   BNE   .jcmp_skip@10
   LDA   HARGS+12
   CMP   #$FF
   BNE   .jcmp_skip@10
   LDA   HARGS+11
   CMP   #$FF
   BNE   .jcmp_skip@10
   LDA   HARGS+10
   CMP   #$80
   BNE   .jcmp_skip@10
   LDA   HARGS+9
   BEQ   .if_end@1
.jcmp_skip@10:
   LDA   __local_check__fail_0
   ORA   #$02
   STA   __local_check__fail_0
.if_end@1:
   LDA   __zpabi_check__v5_3
   BNE   .jcmp_skip@11
   LDA   __zpabi_check__v5_2
   BNE   .jcmp_skip@11
   LDA   __zpabi_check__v5_1
   BNE   .jcmp_skip@11
   LDA   __zpabi_check__v5_0
   CMP   #$01
   BEQ   .cmp_true@12
.jcmp_skip@11:
   LDA   __local_check__fail_0
   ORA   #$04
   STA   __local_check__fail_0
   LDA   #$23
   BNE   .cmp_true@12
   LDA   #$45
   CMP   __zpabi_check__v4
   BNE   .cmp_true@12
   LDA   #$00
   BEQ   .cmp_end@13
.cmp_true@12:
   LDA   #$01
.cmp_end@13:
   STA   __local_check__3
   LDA   #$00
   BPL   .sx_done@16
   LDA   #$FF
.sx_done@16:
   STA   __local_check__2
   STA   __local_check__1
   LDA   __local_check__3
   STA   __local_check__4
   LDA   __local_check__2
   AND   #$03
   STA   __local_check__0
   LDA   __zpabi_check__v1_0
   SEC
   SBC   __local_check__4
   LDA   __zpabi_check__v1_1
   SBC   #$00
   LDA   __zpabi_check__v1_2
   SBC   __local_check__2
   LDA   __zpabi_check__v1_3
   SBC   __local_check__1
   LDA   __zpabi_check__v1_4
   SBC   __local_check__0
   BCC   .check@ssa_block@6
   LDA   __zpabi_check__v0_0
   ORA   __zpabi_check__v0_1
   ORA   __zpabi_check__v0_2
   ORA   __zpabi_check__v0_3
   BEQ   .if_end@5
.check@ssa_block@6:
   LDA   __local_check__fail_0
   ORA   #$08
   STA   __local_check__fail_0
.if_end@5:
   LDA   __zpabi_check__v0_0
   STA   HARGS
   LDA   __zpabi_check__v0_1
   STA   HARGS+1
   LDA   __zpabi_check__v0_2
   STA   HARGS+2
   LDA   __zpabi_check__v0_3
   STA   HARGS+3
   LDA   __zpabi_check__v1_0
   STA   HARGS+8
   LDA   __zpabi_check__v1_1
   STA   HARGS+9
   LDA   __zpabi_check__v1_2
   STA   HARGS+10
   LDA   __zpabi_check__v1_3
   STA   HARGS+11
   LDA   __zpabi_check__v1_4
   STA   HARGS+12
   LDA   #$00
   STA   HARGS+4
   STA   HARGS+5
   STA   HARGS+6
   STA   HARGS+7
   STA   HARGS+13
   STA   HARGS+14
   STA   HARGS+15
   JSR   mul64
   LDA   HARGS+20
   AND   #$03
   STA   __local_check__8
   LDA   __zpabi_check__v4
   BEQ   .and_false@6
   LDA   __zpabi_check__v3_0
   ORA   __zpabi_check__v3_1
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
   BPL   .sx_done@18
   LDA   #$FF
.sx_done@18:
   STA   __local_check__5
   STA   __local_check__4
   LDA   __zpabi_check__v5_0
   EOR   #$26
   STA   __local_check__3
   LDA   __zpabi_check__v5_1
   EOR   #$7E
   STA   __local_check__2
   LDA   __zpabi_check__v5_2
   EOR   #$DC
   STA   __local_check__1
   LDA   __zpabi_check__v5_3
   EOR   #$E8
   STA   __local_check__0
   LDA   __local_check__7
   ORA   __local_check__3
   STA   HARGS+8
   LDA   __local_check__2
   STA   HARGS+9
   LDA   __local_check__5
   ORA   __local_check__1
   STA   HARGS+10
   LDA   __local_check__4
   ORA   __local_check__0
   STA   HARGS+11
   LDA   HARGS+16
   STA   HARGS
   LDA   HARGS+17
   STA   HARGS+1
   LDA   HARGS+18
   STA   HARGS+2
   LDA   HARGS+19
   STA   HARGS+3
   LDA   __local_check__8
   JSR   check@run0
   LDY   #$00
   STY   HARGS+12
   STA   HARGS+13
   STA   HARGS+14
   STA   HARGS+15
   JSR   udivmod64
   LDA   HARGS+24
   ORA   HARGS+25
   ORA   HARGS+26
   ORA   HARGS+27
   ORA   HARGS+28
   BEQ   .if_end@8
   LDA   __local_check__fail_0
   ORA   #$10
   STA   __local_check__fail_0
.if_end@8:
   LDA   __zpabi_check__v3_1
   CMP   #$80
   BNE   .jcmp_skip@19
   LDA   __zpabi_check__v3_0
   BEQ   .if_end@9
.jcmp_skip@19:
   LDA   __local_check__fail_0
   ORA   #$20
   STA   __local_check__fail_0
.if_end@9:
   LDA   __zpabi_check__v0_0
   SEC
   SBC   #$FF
   STA   __local_check__4
   LDA   __zpabi_check__v0_1
   SBC   #$00
   STA   __local_check__3
   LDA   __zpabi_check__v0_2
   SBC   #$00
   STA   __local_check__2
   LDA   __zpabi_check__v0_3
   SBC   #$00
   STA   __local_check__1
   LDA   #$00
   SBC   #$00
   AND   #$03
   STA   __local_check__0
   LDA   __local_check__4
   ORA   __local_check__3
   ORA   __local_check__2
   ORA   __local_check__1
   ORA   __local_check__0
   BEQ   .lb_skip@0
   JMP   .if_end@12
.lb_skip@0:
   LDA   #$00
   CMP   __zpabi_check__v1_4
   BNE   .cmp_end@21
   CMP   __zpabi_check__v1_3
   BNE   .cmp_end@21
   CMP   __zpabi_check__v1_2
   BNE   .cmp_end@21
   LDA   #$23
   CMP   __zpabi_check__v1_1
   BNE   .thread@1
   LDA   #$45
   CMP   __zpabi_check__v1_0
   BEQ   .cmp_true@20
.thread@1:
   LDA   #$00
   BEQ   .cmp_end@21
.cmp_true@20:
   LDA   #$01
.cmp_end@21:
   STA   __local_check__3
   LDA   #$00
   BPL   .sx_done@24
   LDA   #$FF
.sx_done@24:
   LDX   __local_check__3
   STX   __local_check__9
   STA   __local_check__7
   STA   __local_check__6
   AND   #$03
   STA   __local_check__5
   LDA   #$46
   ORA   __zpabi_check__v0_0
   STA   __local_check__4
   LDA   #$79
   ORA   __zpabi_check__v0_1
   STA   __local_check__3
   LDA   #$37
   ORA   __zpabi_check__v0_2
   STA   __local_check__2
   LDA   #$AC
   ORA   __zpabi_check__v0_3
   STA   __local_check__1
   LDA   __local_check__9
   EOR   __local_check__4
   STA   __local_check__4
   LDA   #$00
   EOR   __local_check__3
   STA   __local_check__3
   LDA   __local_check__7
   EOR   __local_check__2
   STA   __local_check__2
   LDA   __local_check__6
   EOR   __local_check__1
   STA   __local_check__1
   LDA   __local_check__5
   EOR   #$02
   STA   __local_check__0
   LDA   __local_check__4
   ORA   __local_check__3
   ORA   __local_check__2
   ORA   __local_check__1
   ORA   __local_check__0
   BNE   .if_end@12
   LDA   __local_check__fail_0
   ORA   #$40
   STA   __local_check__fail_0
.if_end@12:
   LDA   #$07
   ORA   __zpabi_check__v4
   STA   HARGS
   LDA   __zpabi_check__v4
   STA   HARGS+1
   JSR   udivmod8
   LDA   #$00
   BPL   .sx_done@26
   LDA   #$FF
.sx_done@26:
   STA   __local_check__2
   STA   __local_check__1
   AND   #$03
   STA   __local_check__0
   LDA   HARGS+2
   CLC
   ADC   __zpabi_check__v1_0
   STA   __local_check__4
   LDA   #$00
   ADC   __zpabi_check__v1_1
   STA   __local_check__3
   LDA   __local_check__2
   ADC   __zpabi_check__v1_2
   STA   __local_check__2
   LDA   __local_check__1
   ADC   __zpabi_check__v1_3
   STA   __local_check__1
   LDA   __local_check__0
   ADC   __zpabi_check__v1_4
   AND   #$03
   CMP   #$02
   BNE   .jcmp_skip@27
   LDA   __local_check__1
   BNE   .jcmp_skip@27
   LDA   __local_check__2
   BNE   .jcmp_skip@27
   LDA   __local_check__3
   BNE   .jcmp_skip@27
   LDA   __local_check__4
   CMP   #$01
   BEQ   .if_end@13
.jcmp_skip@27:
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
   JMP   .if_end@0

check@run0:
   SUBROUTINE

   STA   HARGS+4
   LDA   #$00
   STA   HARGS+5
   STA   HARGS+6
   STA   HARGS+7
   RTS

main:
   SUBROUTINE

   LDA   #$02
   STA   __zpabi_check__v1_4
   LDA   #$B2
   STA   __zpabi_check__v2
   LDA   #$80
   STA   __zpabi_check__v3_1
   STA   __zpabi_check__v4
   LDA   #$01
   STA   __zpabi_check__v5_0
   LDA   #$00
   STA   __zpabi_check__v0_0
   STA   __zpabi_check__v0_1
   STA   __zpabi_check__v0_2
   STA   __zpabi_check__v0_3
   STA   __zpabi_check__v1_0
   STA   __zpabi_check__v1_1
   STA   __zpabi_check__v1_2
   STA   __zpabi_check__v1_3
   STA   __zpabi_check__v3_0
   STA   __zpabi_check__v5_1
   STA   __zpabi_check__v5_2
   STA   __zpabi_check__v5_3
   JMP   check
