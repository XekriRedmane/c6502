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
__zpabi_check__v2_0	EQU	$86
__zpabi_check__v2_1	EQU	$87
__zpabi_check__v2_2	EQU	$88
__zpabi_check__v2_3	EQU	$89
__zpabi_check__v3_0	EQU	$8A
__zpabi_check__v3_1	EQU	$8B
__zpabi_check__v4_0	EQU	$8C
__zpabi_check__v4_1	EQU	$8D
__zpabi_check__v5_0	EQU	$8E
__zpabi_check__v5_1	EQU	$8F
__zpabi_check__v5_2	EQU	$90
__zpabi_check__v5_3	EQU	$91
__local_check__fail_1	EQU	$93
__local_check__fail_0	EQU	$94
__local_check__0	EQU	$95
__local_check__1	EQU	$96
__local_check__2	EQU	$97
__local_check__3	EQU	$98
__local_check__4	EQU	$99
__local_check__5	EQU	$9A
__local_check__6	EQU	$9B

; @zp-link-meta-begin
; def check params=__zpabi_check__v0_0,__zpabi_check__v0_1,__zpabi_check__v0_2,__zpabi_check__v0_3,__zpabi_check__v1_0,__zpabi_check__v1_1,__zpabi_check__v2_0,__zpabi_check__v2_1,__zpabi_check__v2_2,__zpabi_check__v2_3,__zpabi_check__v3_0,__zpabi_check__v3_1,__zpabi_check__v4_0,__zpabi_check__v4_1,__zpabi_check__v5_0,__zpabi_check__v5_1,__zpabi_check__v5_2,__zpabi_check__v5_3 locals=__local_check__fail_1,__local_check__fail_0,__local_check__0,__local_check__1,__local_check__2,__local_check__3,__local_check__4,__local_check__5,__local_check__6 indirect=false in_cycle=false
; def main params= locals=__local_main__0 indirect=false in_cycle=false
; call main -> check
; item check section=.text local=false zero=false
; item main section=.text local=false zero=false
; runtime hargs_size=32 hargs_need=20
; @zp-link-meta-end

check:
   SUBROUTINE

   LDA   __zpabi_check__v1_0
   ORA   __zpabi_check__v1_1
   BNE   .lb_skip@0
   JMP   .check@asm_ssa_split@0
.lb_skip@0:
   LDA   #$01
   STA   __local_check__fail_0
   LDA   #$00
   STA   __local_check__fail_1
.if_end@0:
   LDA   #$07
   CLC
   SBC   __zpabi_check__v2_0
   LDA   #$00
   SBC   __zpabi_check__v2_1
   LDA   #$00
   SBC   __zpabi_check__v2_2
   LDA   #$00
   SBC   __zpabi_check__v2_3
   LDA   #$00
   ROL
   STA   __local_check__0
   LDA   #$00
   BPL   .sx_done@1
   LDA   #$FF
.sx_done@1:
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
   LDA   __local_check__0
   STA   HARGS+4
   LDA   #$00
   STA   HARGS+5
   JSR   udivmod32
   LDA   HARGS+8
   STA   __local_check__0
   LDA   HARGS+9
   STA   __local_check__1
   LDA   HARGS+10
   STA   __local_check__2
   LDA   HARGS+11
   ASL   __local_check__0
   ROL   __local_check__1
   ROL   __local_check__2
   ROL
   BNE   .jcmp_skip@2
   LDA   __local_check__2
   BNE   .jcmp_skip@2
   LDA   __local_check__1
   BNE   .jcmp_skip@2
   LDA   __local_check__0
   CMP   #$02
   BEQ   .if_end@1
.jcmp_skip@2:
   LDA   __local_check__fail_0
   ORA   #$02
   STA   __local_check__fail_0
.if_end@1:
   LDA   __zpabi_check__v5_3
   BNE   .jcmp_skip@3
   LDA   __zpabi_check__v5_2
   BNE   .jcmp_skip@3
   LDA   __zpabi_check__v5_1
   BNE   .jcmp_skip@3
   LDA   __zpabi_check__v5_0
   CMP   #$01
   BEQ   .if_end@3
.jcmp_skip@3:
   LDA   __local_check__fail_0
   ORA   #$08
   STA   __local_check__fail_0
.if_end@3:
   LDA   __zpabi_check__v3_1
   BMI   .sx_neg@4
   LDA   #$00
   BEQ   .sx_done@5
.sx_neg@4:
   LDA   #$FF
.sx_done@5:
   CMP   __zpabi_check__v0_3
   BNE   .thread@0
   CMP   __zpabi_check__v0_2
   BNE   .thread@0
   LDA   __zpabi_check__v3_1
   CMP   __zpabi_check__v0_1
   BNE   .thread@0
   LDA   __zpabi_check__v3_0
   CMP   __zpabi_check__v0_0
   BEQ   .cmp_true@6
.thread@0:
   LDA   #$00
   BEQ   .cmp_end@7
.cmp_true@6:
   LDA   #$01
.cmp_end@7:
   STA   __local_check__0
   LDA   #$FF
   BNE   .thread@1
   CMP   __local_check__0
   BEQ   .cmp_true@9
.thread@1:
   LDA   #$00
   BEQ   .cmp_end@10
.cmp_true@9:
   LDA   #$01
.cmp_end@10:
   STA   __local_check__6
   LDA   #$00
   BPL   .sx_done@13
   LDA   #$FF
.sx_done@13:
   STA   __local_check__5
   STA   __local_check__4
   LDA   __zpabi_check__v3_1
   BMI   .sx_neg@14
   LDA   #$00
   BEQ   .sx_done@15
.sx_neg@14:
   LDA   #$FF
.sx_done@15:
   STA   __local_check__1
   STA   __local_check__0
   LDA   __zpabi_check__v5_3
   CMP   __local_check__0
   BNE   .cmp_true@16
   LDA   __zpabi_check__v5_2
   CMP   __local_check__1
   BNE   .cmp_true@16
   LDA   __zpabi_check__v5_1
   CMP   __zpabi_check__v3_1
   BNE   .cmp_true@16
   LDA   __zpabi_check__v5_0
   CMP   __zpabi_check__v3_0
   BNE   .cmp_true@16
   LDA   #$00
   BEQ   .cmp_end@17
.cmp_true@16:
   LDA   #$01
.cmp_end@17:
   STA   __local_check__2
   LDA   #$00
   BPL   .sx_done@20
   LDA   #$FF
.sx_done@20:
   STA   __local_check__1
   STA   __local_check__0
   LDA   __zpabi_check__v2_0
   AND   __local_check__2
   STA   __local_check__3
   LDA   __zpabi_check__v2_2
   AND   __local_check__1
   STA   __local_check__1
   LDA   __zpabi_check__v2_3
   AND   __local_check__0
   STA   __local_check__0
   LDA   __local_check__6
   CLC
   ADC   __local_check__3
   STA   __local_check__3
   LDA   #$00
   ADC   #$00
   STA   __local_check__2
   LDA   __local_check__5
   ADC   __local_check__1
   STA   __local_check__1
   LDA   __local_check__4
   ADC   __local_check__0
   STA   __local_check__0
   LDA   __local_check__3
   ORA   __local_check__2
   ORA   __local_check__1
   ORA   __local_check__0
   BEQ   .if_end@4
   LDA   __local_check__fail_0
   ORA   #$10
   STA   __local_check__fail_0
.if_end@4:
   LDA   __zpabi_check__v4_1
   BNE   .jcmp_skip@21
   LDA   __zpabi_check__v4_0
   CMP   #$0C
   BEQ   .if_end@5
.jcmp_skip@21:
   LDA   __local_check__fail_0
   ORA   #$20
   STA   __local_check__fail_0
.if_end@5:
   LDA   #$01
   CMP   __zpabi_check__v3_1
   BNE   .cmp_end@23
   LDA   #$00
   CMP   __zpabi_check__v3_0
   BEQ   .cmp_end@23
   LDA   #$01
.cmp_end@23:
   STA   __local_check__6
   LDA   #$00
   BPL   .sx_done@26
   LDA   #$FF
.sx_done@26:
   STA   __local_check__5
   STA   __local_check__4
   LDA   __zpabi_check__v5_0
   SEC
   SBC   __local_check__6
   LDA   __zpabi_check__v5_1
   SBC   #$00
   LDA   __zpabi_check__v5_2
   SBC   __local_check__5
   LDA   __zpabi_check__v5_3
   SBC   __local_check__4
   LDA   #$00
   ROL
   EOR   #$01
   BEQ   .if_end@6
   LDA   __local_check__fail_0
   ORA   #$40
   STA   __local_check__fail_0
.if_end@6:
   LDA   __zpabi_check__v0_0
   AND   __zpabi_check__v0_0
   STA   __local_check__3
   LDA   __zpabi_check__v0_1
   AND   __zpabi_check__v0_1
   STA   __local_check__2
   LDA   __zpabi_check__v0_2
   AND   __zpabi_check__v0_2
   STA   __local_check__1
   LDA   __zpabi_check__v0_3
   AND   __zpabi_check__v0_3
   STA   __local_check__0
   LDA   #$45
   SEC
   SBC   __local_check__3
   LDA   #$00
   SBC   __local_check__2
   LDA   #$00
   SBC   __local_check__1
   LDA   #$00
   SBC   __local_check__0
   BVC   .cmp_novf@27
   EOR   #$80
.cmp_novf@27:
   ASL
   LDA   #$00
   ROL
   STA   __local_check__1
   LDA   __zpabi_check__v2_3
   BNE   .cmp_true@28
   LDA   __zpabi_check__v2_2
   BNE   .cmp_true@28
   LDA   __zpabi_check__v2_1
   BNE   .cmp_true@28
   LDA   __zpabi_check__v2_0
   CMP   #$01
   BNE   .cmp_true@28
   LDA   #$00
   BEQ   .cmp_end@29
.cmp_true@28:
   LDA   #$01
.cmp_end@29:
   STA   __local_check__0
   LDA   __local_check__1
   EOR   __local_check__0
   STA   __local_check__1
   LDA   #$00
   BNE   .jcmp_skip@31
   LDA   __local_check__1
   CMP   #$01
   BEQ   .if_end@7
.jcmp_skip@31:
   LDA   __local_check__fail_0
   ORA   #$80
   STA   __local_check__fail_0
.if_end@7:
   LDX   __local_check__fail_1
   LDA   __local_check__fail_0
   RTS
.check@asm_ssa_split@0:
   LDA   #$00
   STA   __local_check__fail_0
   STA   __local_check__fail_1
   JMP   .if_end@0

main:
   SUBROUTINE

   LDA   #$FF
   STA   __zpabi_check__v0_0
   STA   __zpabi_check__v0_1
   STA   __zpabi_check__v0_2
   STA   __zpabi_check__v0_3
   LDA   #$C3
   STA   __zpabi_check__v3_0
   LDA   #$51
   STA   __zpabi_check__v3_1
   LDA   #$0C
   STA   __zpabi_check__v4_0
   LDA   #$01
   STA   __zpabi_check__v5_0
   LDA   #$00
   STA   __zpabi_check__v1_0
   STA   __zpabi_check__v1_1
   STA   __zpabi_check__v2_0
   STA   __zpabi_check__v2_1
   STA   __zpabi_check__v2_2
   STA   __zpabi_check__v2_3
   STA   __zpabi_check__v4_1
   STA   __zpabi_check__v5_1
   STA   __zpabi_check__v5_2
   STA   __zpabi_check__v5_3
   JMP   check
