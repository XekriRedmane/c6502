SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__zpabi_check__v0_0	EQU	$80
__zpabi_check__v0_1	EQU	$81
__zpabi_check__v0_2	EQU	$82
__zpabi_check__v0_3	EQU	$83
__zpabi_check__v1	EQU	$84
__zpabi_check__v2_0	EQU	$85
__zpabi_check__v2_1	EQU	$86
__zpabi_check__v2_2	EQU	$87
__zpabi_check__v2_3	EQU	$88
__zpabi_check__v3_0	EQU	$89
__zpabi_check__v3_1	EQU	$8A
__zpabi_check__v3_2	EQU	$8B
__zpabi_check__v3_3	EQU	$8C
__zpabi_check__v4_0	EQU	$8D
__zpabi_check__v4_1	EQU	$8E
__zpabi_check__v4_2	EQU	$8F
__zpabi_check__v4_3	EQU	$90
__zpabi_check__v4_4	EQU	$91
__zpabi_check__v5_0	EQU	$92
__zpabi_check__v5_1	EQU	$93
__zpabi_check__v5_2	EQU	$94
__zpabi_check__v5_3	EQU	$95
__local_check__fail_1	EQU	$97
__local_check__fail_0	EQU	$98
__local_check__0	EQU	$99
__local_check__1	EQU	$9A
__local_check__2	EQU	$9B
__local_check__3	EQU	$9C
__local_check__4	EQU	$9D
__local_check__5	EQU	$9E
__local_check__6	EQU	$9F

; @zp-link-meta-begin
; def check params=__zpabi_check__v0_0,__zpabi_check__v0_1,__zpabi_check__v0_2,__zpabi_check__v0_3,__zpabi_check__v1,__zpabi_check__v2_0,__zpabi_check__v2_1,__zpabi_check__v2_2,__zpabi_check__v2_3,__zpabi_check__v3_0,__zpabi_check__v3_1,__zpabi_check__v3_2,__zpabi_check__v3_3,__zpabi_check__v4_0,__zpabi_check__v4_1,__zpabi_check__v4_2,__zpabi_check__v4_3,__zpabi_check__v4_4,__zpabi_check__v5_0,__zpabi_check__v5_1,__zpabi_check__v5_2,__zpabi_check__v5_3 locals=__local_check__fail_1,__local_check__fail_0,__local_check__0,__local_check__1,__local_check__2,__local_check__3,__local_check__4,__local_check__5,__local_check__6,__local_check__7 indirect=false in_cycle=false
; def main params= locals=__local_main__0 indirect=false in_cycle=false
; call main -> check
; item check section=.text local=false zero=false
; item main section=.text local=false zero=false
; runtime hargs_size=32 hargs_need=21
; @zp-link-meta-end

check:
   SUBROUTINE

   LDA   __zpabi_check__v5_0
   ORA   __zpabi_check__v0_0
   STA   __local_check__1
   LDA   __zpabi_check__v5_1
   ORA   __zpabi_check__v0_1
   STA   __local_check__0
   LDA   __zpabi_check__v5_2
   ORA   __zpabi_check__v0_2
   STA   __local_check__fail_0
   LDA   __zpabi_check__v5_3
   ORA   __zpabi_check__v0_3
   STA   __local_check__fail_1
   LDA   __local_check__1
   ORA   __local_check__0
   ORA   __local_check__fail_0
   ORA   __local_check__fail_1
   BEQ   .and_false@1
   SEC
   LDA   __zpabi_check__v0_1
   SBC   #$80
   LDA   __zpabi_check__v0_2
   SBC   #$00
   LDA   __zpabi_check__v0_3
   SBC   #$00
   BCC   .and_false@1
   LDA   #$01
   STA   __local_check__6
   LDA   #$00
   STA   __local_check__4
   JMP   .and_end@2
.and_false@1:
   LDA   #$00
   STA   __local_check__6
   LDY   #$00
   STY   __local_check__4
.and_end@2:
   ORA   #$00
   BPL   .sx_done@1
   LDA   #$FF
.sx_done@1:
   STA   __local_check__3
   STA   __local_check__2
   LDA   __zpabi_check__v1
   BMI   .sx_neg@2
   LDA   #$00
   BEQ   .sx_done@3
.sx_neg@2:
   LDA   #$FF
.sx_done@3:
   STA   __local_check__5
   LDA   __zpabi_check__v3_0
   STA   HARGS
   LDA   __zpabi_check__v3_1
   STA   HARGS+1
   LDA   __zpabi_check__v3_2
   STA   HARGS+2
   LDA   __zpabi_check__v3_3
   STA   HARGS+3
   LDA   __zpabi_check__v1
   STA   HARGS+4
   JSR   lsr32
   LDA   #$01
   AND   HARGS+5
   STA   __local_check__1
   LDA   #$00
   AND   HARGS+6
   STA   __local_check__0
   LDA   #$00
   AND   HARGS+7
   STA   __local_check__fail_0
   LDA   #$00
   AND   HARGS+8
   STA   __local_check__fail_1
   LDA   __local_check__6
   ORA   __local_check__1
   STA   __local_check__1
   LDA   __local_check__4
   ORA   __local_check__0
   STA   __local_check__0
   LDA   __local_check__3
   ORA   __local_check__fail_0
   STA   __local_check__fail_0
   LDA   __local_check__2
   ORA   __local_check__fail_1
   BNE   .jcmp_skip@4
   LDA   __local_check__fail_0
   BNE   .jcmp_skip@4
   LDA   __local_check__0
   BNE   .jcmp_skip@4
   LDA   __local_check__1
   CMP   #$01
   BNE   .lb_skip@0
   JMP   .check@asm_ssa_split@0
.lb_skip@0:
.jcmp_skip@4:
   LDA   #$02
   STA   __local_check__fail_0
   LDA   #$00
   STA   __local_check__fail_1
.vthread@0:
   LDY   #$FF
   CPY   #$FF
   BNE   .jcmp_skip@7
   BEQ   .if_end@6
.jcmp_skip@7:
   LDA   __local_check__fail_0
   ORA   #$04
   STA   __local_check__fail_0
   LDA   #$00
   STA   __local_check__fail_1
.if_end@6:
   LDA   __zpabi_check__v4_0
   CLC
   SBC   __zpabi_check__v4_0
   LDA   __zpabi_check__v4_1
   SBC   __zpabi_check__v4_1
   LDA   __zpabi_check__v4_2
   SBC   __zpabi_check__v4_2
   LDA   __zpabi_check__v4_3
   SBC   __zpabi_check__v4_3
   LDA   __zpabi_check__v4_4
   SBC   __zpabi_check__v4_4
   LDA   #$00
   ROL
   STA   __local_check__1
   LDA   #$07
   SEC
   SBC   __zpabi_check__v1
   LDA   #$00
   SBC   __local_check__5
   BVC   .cmp_novf@8
   EOR   #$80
.cmp_novf@8:
   ASL
   LDA   #$00
   ROL
   STA   __local_check__0
   LDA   __local_check__1
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
   STA   __local_check__3
   LDA   #$00
   BPL   .sx_done@11
   TYA
.sx_done@11:
   LDX   __local_check__3
   STX   HARGS
   LDX   #$00
   STX   HARGS+1
   STA   HARGS+2
   STA   HARGS+3
   AND   #$0F
   STA   HARGS+4
   LDA   __zpabi_check__v0_0
   STA   HARGS+8
   LDA   __zpabi_check__v0_1
   STA   HARGS+9
   LDA   __zpabi_check__v0_2
   STA   HARGS+10
   LDA   __zpabi_check__v0_3
   STA   HARGS+11
   LDA   #$00
   STA   HARGS+5
   STA   HARGS+6
   STA   HARGS+7
   STA   HARGS+12
   STA   HARGS+13
   STA   HARGS+14
   STA   HARGS+15
   JSR   mul64
   LDA   HARGS+20
   AND   #$0F
   BNE   .jcmp_skip@12
   LDA   HARGS+19
   CMP   #$FF
   BNE   .jcmp_skip@12
   LDA   HARGS+18
   CMP   #$FF
   BNE   .jcmp_skip@12
   LDA   HARGS+17
   CMP   #$FF
   BNE   .jcmp_skip@12
   LDA   HARGS+16
   CMP   #$FF
   BEQ   .if_end@7
.jcmp_skip@12:
   LDA   __local_check__fail_0
   ORA   #$08
   STA   __local_check__fail_0
.if_end@7:
   LDA   __zpabi_check__v4_4
   CMP   #$04
   BNE   .jcmp_skip@13
   LDA   __zpabi_check__v4_3
   CMP   #$23
   BNE   .jcmp_skip@13
   LDA   __zpabi_check__v4_2
   CMP   #$A7
   BNE   .jcmp_skip@13
   LDA   __zpabi_check__v4_1
   CMP   #$71
   BNE   .jcmp_skip@13
   LDA   __zpabi_check__v4_0
   CMP   #$1A
   BEQ   .if_end@8
.jcmp_skip@13:
   LDA   __local_check__fail_0
   ORA   #$10
   STA   __local_check__fail_0
.if_end@8:
   LDA   __zpabi_check__v0_0
   SEC
   SBC   __zpabi_check__v2_0
   LDA   __zpabi_check__v0_1
   SBC   __zpabi_check__v2_1
   LDA   __zpabi_check__v0_2
   SBC   __zpabi_check__v2_2
   LDA   __zpabi_check__v0_3
   SBC   __zpabi_check__v2_3
   LDA   #$00
   ROL
   STA   __local_check__0
   LDA   #$00
   BPL   .sx_done@15
   LDA   #$FF
.sx_done@15:
   STA   HARGS+2
   STA   HARGS+3
   LDA   __local_check__0
   STA   HARGS
   LDA   #$00
   STA   HARGS+1
   LDA   __zpabi_check__v0_0
   STA   HARGS+4
   LDA   __zpabi_check__v0_1
   STA   HARGS+5
   LDA   __zpabi_check__v0_2
   STA   HARGS+6
   LDA   __zpabi_check__v0_3
   STA   HARGS+7
   JSR   udivmod32
   LDA   HARGS+8
   ORA   HARGS+9
   ORA   HARGS+10
   ORA   HARGS+11
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
   JMP   .vthread@0

main:
   SUBROUTINE

   LDA   #$FF
   STA   __zpabi_check__v0_0
   STA   __zpabi_check__v0_1
   STA   __zpabi_check__v0_2
   STA   __zpabi_check__v0_3
   LDA   #$01
   STA   __zpabi_check__v1
   STA   __zpabi_check__v5_0
   LDA   #$80
   STA   __zpabi_check__v2_3
   LDA   #$09
   STA   __zpabi_check__v3_0
   LDA   #$1A
   STA   __zpabi_check__v4_0
   LDA   #$71
   STA   __zpabi_check__v4_1
   LDA   #$A7
   STA   __zpabi_check__v4_2
   LDA   #$23
   STA   __zpabi_check__v4_3
   LDA   #$04
   STA   __zpabi_check__v4_4
   LDA   #$00
   STA   __zpabi_check__v2_0
   STA   __zpabi_check__v2_1
   STA   __zpabi_check__v2_2
   STA   __zpabi_check__v3_1
   STA   __zpabi_check__v3_2
   STA   __zpabi_check__v3_3
   STA   __zpabi_check__v5_1
   STA   __zpabi_check__v5_2
   STA   __zpabi_check__v5_3
   JMP   check
