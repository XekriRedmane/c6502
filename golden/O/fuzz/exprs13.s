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
__zpabi_check__v3_0	EQU	$88
__zpabi_check__v3_1	EQU	$89
__zpabi_check__v4_0	EQU	$8A
__zpabi_check__v4_1	EQU	$8B
__zpabi_check__v5_0	EQU	$8C
__zpabi_check__v5_1	EQU	$8D
__zpabi_check__v5_2	EQU	$8E
__zpabi_check__v5_3	EQU	$8F
__zpabi_check__v5_4	EQU	$90
__zpabi_check__v5_5	EQU	$91
__local_check__fail_1	EQU	$93
__local_check__fail_0	EQU	$94
__local_check__0	EQU	$95
__local_check__1	EQU	$96
__local_check__2	EQU	$97
__local_check__3	EQU	$98
__local_check__4	EQU	$99
__local_check__5	EQU	$9A
__local_check__8	EQU	$9D

; @zp-link-meta-begin
; def check params=__zpabi_check__v0_0,__zpabi_check__v0_1,__zpabi_check__v0_2,__zpabi_check__v0_3,__zpabi_check__v1_0,__zpabi_check__v1_1,__zpabi_check__v2_0,__zpabi_check__v2_1,__zpabi_check__v3_0,__zpabi_check__v3_1,__zpabi_check__v4_0,__zpabi_check__v4_1,__zpabi_check__v5_0,__zpabi_check__v5_1,__zpabi_check__v5_2,__zpabi_check__v5_3,__zpabi_check__v5_4,__zpabi_check__v5_5 locals=__local_check__fail_1,__local_check__fail_0,__local_check__0,__local_check__1,__local_check__2,__local_check__3,__local_check__4,__local_check__5,__local_check__6,__local_check__7,__local_check__8 indirect=false in_cycle=false
; def main params= locals=__local_main__0 indirect=false in_cycle=false
; call main -> check
; item check section=.text local=false zero=false
; item main section=.text local=false zero=false
; runtime hargs_size=32 hargs_need=30
; @zp-link-meta-end

check:
   SUBROUTINE

   LDA   __zpabi_check__v3_1
   BNE   .jcmp_skip@0
   LDA   __zpabi_check__v3_0
   CMP   #$01
   BNE   .lb_skip@0
   JMP   .check@asm_ssa_split@0
.lb_skip@0:
.jcmp_skip@0:
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
   LDA   __zpabi_check__v5_4
   STA   HARGS+4
   LDA   __zpabi_check__v5_5
   STA   HARGS+5
   LDA   #$45
   STA   HARGS+8
   LDA   #$23
   STA   HARGS+9
   LDA   #$01
   STA   HARGS+10
   LDA   #$00
   STA   HARGS+6
   STA   HARGS+7
   STA   HARGS+11
   LDY   #$00
   STY   HARGS+12
   STY   HARGS+13
   STA   HARGS+14
   STA   HARGS+15
   JSR   udivmod64
   LDA   HARGS+29
   STA   __local_check__2
   LDA   __zpabi_check__v3_0
   SEC
   SBC   __zpabi_check__v4_0
   STA   __local_check__1
   LDA   __zpabi_check__v3_1
   SBC   __zpabi_check__v4_1
   STA   __local_check__0
   LDA   HARGS+24
   SEC
   SBC   __local_check__1
   STA   __local_check__1
   LDA   HARGS+25
   SBC   __local_check__0
   STA   __local_check__0
   LDA   HARGS+26
   SBC   #$00
   STA   __local_check__5
   LDA   HARGS+27
   SBC   #$00
   STA   __local_check__4
   LDA   HARGS+28
   SBC   #$00
   STA   __local_check__3
   BCS   .carry_inc@0
   DEC   __local_check__2
.carry_inc@0:
   LDA   __local_check__1
   SEC
   SBC   #$7F
   LDA   __local_check__0
   SBC   #$00
   LDA   __local_check__5
   SBC   #$00
   LDA   __local_check__4
   SBC   #$00
   LDA   __local_check__3
   SBC   #$00
   LDA   __local_check__2
   SBC   #$00
   BCS   .if_end@1
   LDA   __local_check__fail_0
   ORA   #$02
   STA   __local_check__fail_0
.if_end@1:
   LDA   __zpabi_check__v2_0
   BNE   .if_end@4
   LDA   __zpabi_check__v3_0
   ORA   __zpabi_check__v3_1
   BNE   .if_end@4
   LDA   __local_check__fail_0
   ORA   #$04
   STA   __local_check__fail_0
.if_end@4:
   LDA   __zpabi_check__v1_0
   EOR   __zpabi_check__v4_0
   STA   __local_check__1
   LDA   __zpabi_check__v1_1
   EOR   __zpabi_check__v4_1
   STA   __local_check__0
   LDA   __zpabi_check__v1_0
   CLC
   SBC   __local_check__1
   LDA   __zpabi_check__v1_1
   SBC   __local_check__0
   LDA   #$00
   ROL
   STA   __local_check__8
   LDA   __zpabi_check__v2_1
   BMI   .sx_neg@1
   LDA   #$00
   BEQ   .sx_done@2
.sx_neg@1:
   LDA   #$FF
.sx_done@2:
   STA   HARGS+2
   STA   HARGS+3
   LDA   __zpabi_check__v2_0
   STA   HARGS
   LDA   __zpabi_check__v2_1
   STA   HARGS+1
   LDA   __zpabi_check__v0_0
   STA   HARGS+4
   LDA   __zpabi_check__v0_1
   STA   HARGS+5
   LDA   __zpabi_check__v0_2
   STA   HARGS+6
   LDA   __zpabi_check__v0_3
   STA   HARGS+7
   JSR   mul32
   LDA   __zpabi_check__v2_0
   ORA   __zpabi_check__v2_1
   BNE   .or_true@5
   LDA   __zpabi_check__v3_0
   ORA   __zpabi_check__v3_1
   BNE   .or_true@5
   LDA   #$00
   STA   __local_check__3
   JMP   .or_end@6
.or_true@5:
   LDA   #$01
   STA   __local_check__3
   LDA   #$00
.or_end@6:
   ORA   #$00
   BPL   .sx_done@4
   LDA   #$FF
.sx_done@4:
   STA   __local_check__1
   STA   __local_check__0
   LDA   __local_check__3
   SEC
   SBC   HARGS+8
   LDA   #$00
   SBC   HARGS+9
   LDA   __local_check__1
   SBC   HARGS+10
   LDA   __local_check__0
   SBC   HARGS+11
   BVC   .cmp_novf@5
   EOR   #$80
.cmp_novf@5:
   ASL
   LDA   #$00
   ROL
   EOR   #$01
   STA   __local_check__0
   LDA   __local_check__8
   SEC
   SBC   __local_check__0
   LDA   #$00
   SBC   #$00
   BVC   .cmp_novf@6
   EOR   #$80
.cmp_novf@6:
   ASL
   LDA   #$00
   ROL
   STA   __local_check__0
   LDA   #$00
   BNE   .jcmp_skip@7
   LDA   __local_check__0
   CMP   #$01
   BEQ   .if_end@7
.jcmp_skip@7:
   LDA   __local_check__fail_0
   ORA   #$08
   STA   __local_check__fail_0
.if_end@7:
   LDA   __zpabi_check__v1_0
   ORA   __zpabi_check__v1_1
   BEQ   .if_end@8
   LDA   __local_check__fail_0
   ORA   #$10
   STA   __local_check__fail_0
.if_end@8:
   LDA   __zpabi_check__v2_0
   ORA   __zpabi_check__v2_1
   BEQ   .if_end@9
   LDA   __local_check__fail_0
   ORA   #$20
   STA   __local_check__fail_0
.if_end@9:
   LDA   __zpabi_check__v2_0
   ORA   __zpabi_check__v2_1
   BEQ   .if_end@11
   LDA   __local_check__fail_0
   ORA   #$80
   STA   __local_check__fail_0
.if_end@11:
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

   LDA   #$01
   STA   __zpabi_check__v3_0
   STA   __zpabi_check__v5_0
   LDA   #$A4
   STA   __zpabi_check__v4_0
   LDA   #$BE
   STA   __zpabi_check__v4_1
   LDA   #$00
   STA   __zpabi_check__v0_0
   STA   __zpabi_check__v0_1
   STA   __zpabi_check__v0_2
   STA   __zpabi_check__v0_3
   STA   __zpabi_check__v1_0
   STA   __zpabi_check__v1_1
   STA   __zpabi_check__v2_0
   STA   __zpabi_check__v2_1
   STA   __zpabi_check__v3_1
   STA   __zpabi_check__v5_1
   STA   __zpabi_check__v5_2
   STA   __zpabi_check__v5_3
   STA   __zpabi_check__v5_4
   STA   __zpabi_check__v5_5
   JMP   check
