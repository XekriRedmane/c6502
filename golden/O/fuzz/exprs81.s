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
__zpabi_check__v2	EQU	$88
__zpabi_check__v3_0	EQU	$89
__zpabi_check__v3_1	EQU	$8A
__zpabi_check__v3_2	EQU	$8B
__zpabi_check__v3_3	EQU	$8C
__zpabi_check__v4	EQU	$8D
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
; def check params=__zpabi_check__v0_0,__zpabi_check__v0_1,__zpabi_check__v0_2,__zpabi_check__v0_3,__zpabi_check__v1_0,__zpabi_check__v1_1,__zpabi_check__v1_2,__zpabi_check__v1_3,__zpabi_check__v2,__zpabi_check__v3_0,__zpabi_check__v3_1,__zpabi_check__v3_2,__zpabi_check__v3_3,__zpabi_check__v4,__zpabi_check__v5_0,__zpabi_check__v5_1,__zpabi_check__v5_2,__zpabi_check__v5_3 locals=__local_check__fail_1,__local_check__fail_0,__local_check__0,__local_check__1,__local_check__2,__local_check__3,__local_check__4,__local_check__5,__local_check__6,__local_check__7,__local_check__8,__local_check__9 indirect=false in_cycle=false
; def main params= locals=__local_main__0 indirect=false in_cycle=false
; call main -> check
; item check section=.text local=false zero=false
; item main section=.text local=false zero=false
; runtime hargs_size=32 hargs_need=20
; @zp-link-meta-end

check:
   SUBROUTINE

   LDA   #$00
   CMP   __zpabi_check__v5_3
   BNE   .cmp_end@1
   CMP   __zpabi_check__v5_2
   BNE   .cmp_end@1
   CMP   __zpabi_check__v5_1
   BNE   .cmp_end@1
   CMP   __zpabi_check__v5_0
   BNE   .cmp_end@1
   LDA   #$01
.cmp_end@1:
   STA   __local_check__0
   LDA   #$00
   BPL   .sx_done@4
   LDA   #$FF
.sx_done@4:
   STA   __local_check__fail_0
   STA   __local_check__fail_1
   LDA   __local_check__0
   SEC
   SBC   #$6B
   LDA   #$00
   SBC   #$D6
   LDA   __local_check__fail_0
   SBC   #$46
   LDA   __local_check__fail_1
   SBC   #$BB
   LDA   #$00
   ROL
   STA   __local_check__0
   LDA   #$00
   BPL   .sx_done@6
   LDA   #$FF
.sx_done@6:
   STA   __local_check__fail_0
   STA   __local_check__fail_1
   LDA   __local_check__0
   SEC
   SBC   #$02
   LDA   #$00
   SBC   #$00
   LDA   __local_check__fail_0
   SBC   #$00
   LDA   __local_check__fail_1
   SBC   #$00
   BVC   .cmp_novf@7
   EOR   #$80
.cmp_novf@7:
   ASL
   LDA   #$00
   ROL
   STA   __local_check__fail_1
   LDA   #$00
   BNE   .jcmp_skip@8
   LDA   __local_check__fail_1
   CMP   #$01
   BNE   .lb_skip@0
   JMP   .check@asm_ssa_split@0
.lb_skip@0:
.jcmp_skip@8:
   LDA   #$01
   STA   __local_check__fail_0
   LDA   #$00
   STA   __local_check__fail_1
.if_end@0:
   LDA   __zpabi_check__v5_0
   STA   HARGS
   LDA   __zpabi_check__v5_1
   STA   HARGS+1
   LDA   __zpabi_check__v5_2
   STA   HARGS+2
   LDA   __zpabi_check__v5_3
   STA   HARGS+3
   LDA   #$07
   STA   HARGS+4
   LDA   #$00
   STA   HARGS+5
   LDY   #$00
   STY   HARGS+6
   STA   HARGS+7
   JSR   udivmod32
   LDA   HARGS+8
   ORA   #$45
   STA   __local_check__3
   LDA   HARGS+9
   ORA   #$23
   STA   __local_check__2
   LDA   HARGS+10
   ORA   #$01
   STA   __local_check__1
   LDA   __local_check__3
   ORA   __local_check__2
   ORA   __local_check__1
   ORA   HARGS+11
   BEQ   .vthread@0
   LDA   __zpabi_check__v4
   BMI   .sx_neg@9
   LDA   #$00
   BEQ   .sx_done@10
.sx_neg@9:
   LDA   #$FF
.sx_done@10:
   STA   __local_check__6
   LDA   __zpabi_check__v4
   BMI   .sx_neg@11
   LDA   #$00
   BEQ   .sx_done@12
.sx_neg@11:
   LDA   #$FF
.sx_done@12:
   STA   __local_check__2
   LDA   __zpabi_check__v4
   ORA   #$80
   STA   __local_check__5
   LDA   __local_check__2
   STA   __local_check__4
   STA   __local_check__3
   LDA   #$7F
   STA   HARGS
   LDA   #$00
   STA   HARGS+1
   LDA   __zpabi_check__v4
   STA   HARGS+2
   LDA   __local_check__6
   STA   HARGS+3
   JSR   udivmod16
   LDA   __local_check__5
   SEC
   SBC   HARGS+6
   STA   __local_check__1
   LDA   __local_check__4
   SBC   HARGS+7
   STA   __local_check__0
   LDA   __local_check__3
   SBC   #$00
   STA   __local_check__3
   BCS   .carry_inc@0
   DEC   __local_check__2
.carry_inc@0:
   LDA   __local_check__1
   ORA   __local_check__0
   ORA   __local_check__3
   ORA   __local_check__2
   BNE   .if_end@5
.vthread@0:
   LDA   __local_check__fail_0
   ORA   #$08
   STA   __local_check__fail_0
.if_end@5:
   LDA   __zpabi_check__v3_0
   SEC
   SBC   #$01
   LDA   __zpabi_check__v3_1
   SBC   #$00
   LDA   __zpabi_check__v3_2
   SBC   #$00
   LDA   __zpabi_check__v3_3
   SBC   #$00
   LDA   #$00
   ROL
   STA   __local_check__0
   LDA   __zpabi_check__v2
   BMI   .sx_neg@13
   LDA   #$00
   BEQ   .sx_done@14
.sx_neg@13:
   LDA   #$FF
.sx_done@14:
   STA   __local_check__5
   LDA   __local_check__0
   ORA   __zpabi_check__v2
   STA   __local_check__4
   LDA   __zpabi_check__v3_0
   AND   __zpabi_check__v5_0
   STA   __local_check__3
   LDA   __zpabi_check__v3_1
   AND   __zpabi_check__v5_1
   STA   __local_check__2
   LDA   __zpabi_check__v3_2
   AND   __zpabi_check__v5_2
   STA   __local_check__1
   LDA   __zpabi_check__v3_3
   AND   __zpabi_check__v5_3
   STA   __local_check__0
   LDA   #$00
   CMP   __local_check__0
   BNE   .cmp_true@15
   CMP   __local_check__1
   BNE   .cmp_true@15
   CMP   __local_check__2
   BNE   .cmp_true@15
   CMP   __local_check__3
   BEQ   .cmp_end@16
.cmp_true@15:
   LDA   #$01
.cmp_end@16:
   STA   __local_check__0
   LDA   __local_check__4
   SEC
   SBC   __local_check__0
   STA   __local_check__0
   BCS   .carry_inc@1
   DEC   __local_check__5
.carry_inc@1:
   LDA   __local_check__5
   CMP   #$FF
   BNE   .jcmp_skip@18
   LDA   __local_check__0
   CMP   #$FF
   BEQ   .if_end@9
.jcmp_skip@18:
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
   JMP   .if_end@0

main:
   SUBROUTINE

   LDA   #$00
   STA   __zpabi_check__v0_0
   STA   __zpabi_check__v0_1
   STA   __zpabi_check__v0_2
   STA   __zpabi_check__v3_1
   STA   __zpabi_check__v3_2
   STA   __zpabi_check__v3_3
   STA   __zpabi_check__v5_0
   STA   __zpabi_check__v5_1
   STA   __zpabi_check__v5_2
   LDA   #$FF
   STA   __zpabi_check__v1_0
   STA   __zpabi_check__v1_1
   STA   __zpabi_check__v1_2
   STA   __zpabi_check__v1_3
   STA   __zpabi_check__v2
   LDA   #$01
   STA   __zpabi_check__v3_0
   LDA   #$80
   STA   __zpabi_check__v0_3
   STA   __zpabi_check__v4
   STA   __zpabi_check__v5_3
   JMP   check
