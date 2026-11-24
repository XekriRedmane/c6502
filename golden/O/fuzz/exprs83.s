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
__zpabi_check__v2_0	EQU	$8A
__zpabi_check__v2_1	EQU	$8B
__zpabi_check__v3	EQU	$8C
__zpabi_check__v4	EQU	$8D
__zpabi_check__v5_0	EQU	$8E
__zpabi_check__v5_1	EQU	$8F
__zpabi_check__v5_2	EQU	$90
__zpabi_check__v5_3	EQU	$91
__zpabi_check__v5_4	EQU	$92
__zpabi_check__v5_5	EQU	$93
__zpabi_check__v5_6	EQU	$94
__local_check__fail_1	EQU	$96
__local_check__fail_0	EQU	$97
__local_check__0	EQU	$98
__local_check__1	EQU	$99
__local_check__2	EQU	$9A
__local_check__3	EQU	$9B
__local_check__6	EQU	$9E

; @zp-link-meta-begin
; def check params=__zpabi_check__v0_0,__zpabi_check__v0_1,__zpabi_check__v0_2,__zpabi_check__v0_3,__zpabi_check__v0_4,__zpabi_check__v0_5,__zpabi_check__v0_6,__zpabi_check__v0_7,__zpabi_check__v1_0,__zpabi_check__v1_1,__zpabi_check__v2_0,__zpabi_check__v2_1,__zpabi_check__v3,__zpabi_check__v4,__zpabi_check__v5_0,__zpabi_check__v5_1,__zpabi_check__v5_2,__zpabi_check__v5_3,__zpabi_check__v5_4,__zpabi_check__v5_5,__zpabi_check__v5_6 locals=__local_check__fail_1,__local_check__fail_0,__local_check__0,__local_check__1,__local_check__2,__local_check__3,__local_check__4,__local_check__5,__local_check__6 indirect=false in_cycle=false
; def main params= locals=__local_main__0 indirect=false in_cycle=false
; call main -> check
; item check section=.text local=false zero=false
; item main section=.text local=false zero=false
; runtime hargs_size=32 hargs_need=23
; @zp-link-meta-end

check:
   SUBROUTINE

   LDA   __zpabi_check__v0_7
   CMP   #$FF
   BNE   .jcmp_skip@0
   LDA   __zpabi_check__v0_6
   CMP   #$B6
   BNE   .jcmp_skip@0
   LDA   __zpabi_check__v0_5
   CMP   #$A9
   BNE   .jcmp_skip@0
   LDA   __zpabi_check__v0_4
   CMP   #$C6
   BNE   .jcmp_skip@0
   LDA   __zpabi_check__v0_3
   CMP   #$D8
   BNE   .jcmp_skip@0
   LDA   __zpabi_check__v0_2
   CMP   #$02
   BNE   .jcmp_skip@0
   LDA   __zpabi_check__v0_1
   CMP   #$DB
   BNE   .jcmp_skip@0
   LDA   __zpabi_check__v0_0
   CMP   #$89
   BNE   .lb_skip@0
   JMP   .check@asm_ssa_split@0
.lb_skip@0:
.jcmp_skip@0:
   LDA   #$02
   STA   __local_check__fail_0
   LDA   #$00
   STA   __local_check__fail_1
.if_end@3:
   LDA   __zpabi_check__v3
   CMP   #$D1
   BEQ   .thread@0
   LDA   __local_check__fail_0
   ORA   #$04
   STA   __local_check__fail_0
   LDA   #$00
   STA   __local_check__fail_1
   CMP   #$FF
   BNE   .cmp_end@2
   LDA   __zpabi_check__v4
   CMP   #$FF
   BEQ   .cmp_true@1
.thread@0:
   LDA   #$00
   BEQ   .cmp_end@2
.cmp_true@1:
   LDA   #$01
.cmp_end@2:
   STA   __local_check__1
   LDA   __zpabi_check__v0_7
   BNE   .thread@1
   LDA   __zpabi_check__v0_6
   BNE   .thread@1
   LDA   __zpabi_check__v0_5
   BNE   .thread@1
   LDA   __zpabi_check__v0_4
   BNE   .thread@1
   LDA   __zpabi_check__v0_3
   BNE   .thread@1
   LDA   __zpabi_check__v0_2
   BNE   .thread@1
   LDA   __zpabi_check__v0_1
   BNE   .thread@1
   LDA   __zpabi_check__v0_0
   CMP   #$45
   BEQ   .cmp_true@4
.thread@1:
   LDA   #$00
   BEQ   .cmp_end@5
.cmp_true@4:
   LDA   #$01
.cmp_end@5:
   AND   __local_check__1
   STA   __local_check__6
   LDA   #$00
   BPL   .sx_done@8
   LDA   #$FF
.sx_done@8:
   LDX   __local_check__6
   STX   HARGS
   LDX   #$00
   STX   HARGS+1
   STA   HARGS+2
   STA   HARGS+3
   STA   HARGS+4
   STA   HARGS+5
   AND   #$07
   STA   HARGS+6
   LDA   __zpabi_check__v5_0
   STA   HARGS+8
   LDA   __zpabi_check__v5_1
   STA   HARGS+9
   LDA   __zpabi_check__v5_2
   STA   HARGS+10
   LDA   __zpabi_check__v5_3
   STA   HARGS+11
   LDA   __zpabi_check__v5_4
   STA   HARGS+12
   LDA   __zpabi_check__v5_5
   STA   HARGS+13
   LDA   __zpabi_check__v5_6
   STA   HARGS+14
   LDA   #$00
   STA   HARGS+7
   STA   HARGS+15
   JSR   mul64
   LDA   HARGS+22
   AND   #$07
   STA   __local_check__0
   LDA   HARGS+16
   ORA   HARGS+17
   ORA   HARGS+18
   ORA   HARGS+19
   ORA   HARGS+20
   ORA   HARGS+21
   ORA   __local_check__0
   BEQ   .if_end@6
   LDA   __local_check__fail_0
   ORA   #$10
   STA   __local_check__fail_0
.if_end@6:
   LDA   __zpabi_check__v4
   SEC
   SBC   __zpabi_check__v4
   LDA   #$00
   ROL
   STA   __local_check__0
   LDA   __zpabi_check__v3
   SEC
   SBC   __local_check__0
   LDA   #$00
   SBC   #$00
   BVC   .cmp_novf@9
   EOR   #$80
.cmp_novf@9:
   ASL
   LDA   #$00
   ROL
   EOR   #$01
   STA   __local_check__2
   LDA   __zpabi_check__v1_1
   BMI   .sx_neg@10
   LDA   #$00
   BEQ   .sx_done@11
.sx_neg@10:
   LDA   #$FF
.sx_done@11:
   STA   __local_check__1
   STA   __local_check__0
   LDA   #$00
   CMP   __local_check__0
   BNE   .cmp_end@13
   CMP   __local_check__1
   BNE   .cmp_end@13
   CMP   __zpabi_check__v1_1
   BNE   .cmp_end@13
   LDA   #$07
   CMP   __zpabi_check__v1_0
   BEQ   .cmp_true@12
   LDA   #$00
   BEQ   .cmp_end@13
.cmp_true@12:
   LDA   #$01
.cmp_end@13:
   CLC
   ADC   #$01
   STA   __local_check__1
   LDA   #$00
   ADC   #$00
   STA   __local_check__0
   LDA   __local_check__2
   AND   __local_check__1
   STA   __local_check__1
   LDA   #$00
   AND   __local_check__0
   BNE   .jcmp_skip@15
   LDA   __local_check__1
   CMP   #$01
   BEQ   .if_end@7
.jcmp_skip@15:
   LDA   __local_check__fail_0
   ORA   #$20
   STA   __local_check__fail_0
.if_end@7:
   LDA   __zpabi_check__v1_1
   CMP   #$60
   BNE   .jcmp_skip@16
   LDA   __zpabi_check__v1_0
   CMP   #$E0
   BEQ   .if_end@8
.jcmp_skip@16:
   LDA   __local_check__fail_0
   ORA   #$40
   STA   __local_check__fail_0
.if_end@8:
   LDA   __zpabi_check__v5_0
   ORA   __zpabi_check__v5_0
   STA   __local_check__3
   LDA   __zpabi_check__v5_1
   ORA   __zpabi_check__v5_1
   STA   __local_check__2
   LDA   __zpabi_check__v5_2
   ORA   __zpabi_check__v5_2
   STA   __local_check__1
   LDA   __zpabi_check__v5_3
   ORA   __zpabi_check__v5_3
   STA   __local_check__0
   LDA   __zpabi_check__v2_0
   AND   __local_check__3
   STA   __local_check__3
   LDA   __zpabi_check__v2_1
   AND   __local_check__2
   STA   __local_check__2
   LDA   #$00
   AND   __local_check__1
   STA   __local_check__1
   LDA   #$00
   AND   __local_check__0
   STA   __local_check__0
   LDA   __local_check__3
   ORA   __local_check__2
   ORA   __local_check__1
   ORA   __local_check__0
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
   JMP   .if_end@3

main:
   SUBROUTINE

   LDA   #$89
   STA   __zpabi_check__v0_0
   LDA   #$DB
   STA   __zpabi_check__v0_1
   LDA   #$02
   STA   __zpabi_check__v0_2
   LDA   #$D8
   STA   __zpabi_check__v0_3
   LDA   #$C6
   STA   __zpabi_check__v0_4
   LDA   #$A9
   STA   __zpabi_check__v0_5
   LDA   #$B6
   STA   __zpabi_check__v0_6
   LDA   #$FF
   STA   __zpabi_check__v0_7
   LDA   #$E0
   STA   __zpabi_check__v1_0
   LDA   #$60
   STA   __zpabi_check__v1_1
   LDA   #$80
   STA   __zpabi_check__v2_1
   LDA   #$D1
   STA   __zpabi_check__v3
   LDA   #$40
   STA   __zpabi_check__v4
   LDA   #$00
   STA   __zpabi_check__v2_0
   STA   __zpabi_check__v5_0
   STA   __zpabi_check__v5_1
   STA   __zpabi_check__v5_2
   STA   __zpabi_check__v5_3
   STA   __zpabi_check__v5_4
   STA   __zpabi_check__v5_5
   STA   __zpabi_check__v5_6
   JMP   check
