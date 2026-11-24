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
__zpabi_check__v2_0	EQU	$88
__zpabi_check__v2_1	EQU	$89
__zpabi_check__v3	EQU	$8A
__zpabi_check__v4_0	EQU	$8B
__zpabi_check__v4_1	EQU	$8C
__zpabi_check__v4_2	EQU	$8D
__zpabi_check__v4_3	EQU	$8E
__zpabi_check__v5	EQU	$8F
__local_check__fail_1	EQU	$91
__local_check__fail_0	EQU	$92
__local_check__0	EQU	$93
__local_check__1	EQU	$94
__local_check__2	EQU	$95
__local_check__3	EQU	$96
__local_check__4	EQU	$97
__local_check__5	EQU	$98

; @zp-link-meta-begin
; def check params=__zpabi_check__v0_0,__zpabi_check__v0_1,__zpabi_check__v0_2,__zpabi_check__v0_3,__zpabi_check__v1_0,__zpabi_check__v1_1,__zpabi_check__v1_2,__zpabi_check__v1_3,__zpabi_check__v2_0,__zpabi_check__v2_1,__zpabi_check__v3,__zpabi_check__v4_0,__zpabi_check__v4_1,__zpabi_check__v4_2,__zpabi_check__v4_3,__zpabi_check__v5 locals=__local_check__fail_1,__local_check__fail_0,__local_check__0,__local_check__1,__local_check__2,__local_check__3,__local_check__4,__local_check__5,__local_check__6,__local_check__7 indirect=false in_cycle=false
; def main params= locals=__local_main__0 indirect=false in_cycle=false
; call main -> check
; item check section=.text local=false zero=false
; item main section=.text local=false zero=false
; runtime hargs_size=32 hargs_need=22
; @zp-link-meta-end

check:
   SUBROUTINE

   LDA   __zpabi_check__v0_3
   BNE   .jcmp_skip@0
   LDA   __zpabi_check__v0_2
   BNE   .jcmp_skip@0
   LDA   __zpabi_check__v0_1
   BNE   .jcmp_skip@0
   LDA   __zpabi_check__v0_0
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
   LDA   __zpabi_check__v5
   BMI   .sx_neg@1
   LDA   #$00
   BEQ   .sx_done@2
.sx_neg@1:
   LDA   #$FF
.sx_done@2:
   STA   __local_check__5
   LDA   __zpabi_check__v5
   BMI   .sx_neg@3
   LDA   #$00
   BEQ   .sx_done@4
.sx_neg@3:
   LDA   #$FF
.sx_done@4:
   LDY   #$FF
   STY   HARGS
   STY   HARGS+1
   STA   HARGS+2
   STA   HARGS+3
   STY   HARGS+4
   LDA   #$00
   STA   HARGS+5
   INY
   STY   HARGS+6
   STA   HARGS+7
   JSR   udivmod32
   LDA   HARGS+12
   EOR   #$FF
   STA   __local_check__3
   LDA   HARGS+13
   EOR   #$FF
   STA   __local_check__2
   LDA   HARGS+14
   EOR   #$FF
   STA   __local_check__1
   LDA   HARGS+15
   EOR   #$FF
   CMP   #$FF
   BNE   .jcmp_skip@5
   LDA   __local_check__1
   CMP   #$FF
   BNE   .jcmp_skip@5
   LDA   __local_check__2
   CMP   #$FF
   BNE   .jcmp_skip@5
   LDA   __local_check__3
   CMP   #$FF
   BEQ   .if_end@1
.jcmp_skip@5:
   LDA   __local_check__fail_0
   ORA   #$02
   STA   __local_check__fail_0
.if_end@1:
   LDA   __zpabi_check__v3
   BMI   .sx_neg@6
   LDA   #$00
   BEQ   .sx_done@7
.sx_neg@6:
   LDA   #$FF
.sx_done@7:
   STA   __local_check__4
   LDA   __zpabi_check__v3
   STA   __local_check__1
   LDA   __local_check__4
   STA   __local_check__0
   BMI   .sx_neg@8
   LDA   #$00
   BEQ   .sx_done@9
.sx_neg@8:
   LDA   #$FF
.sx_done@9:
   STA   HARGS+6
   STA   HARGS+7
   LDA   __zpabi_check__v0_0
   STA   HARGS
   LDA   __zpabi_check__v0_1
   STA   HARGS+1
   LDA   __zpabi_check__v0_2
   STA   HARGS+2
   LDA   __zpabi_check__v0_3
   STA   HARGS+3
   LDA   __local_check__1
   STA   HARGS+4
   LDA   __local_check__0
   STA   HARGS+5
   JSR   sdivmod32
   LDA   HARGS+11
   CMP   #$FF
   BNE   .jcmp_skip@10
   LDA   HARGS+10
   CMP   #$FF
   BNE   .jcmp_skip@10
   LDA   HARGS+9
   CMP   #$FF
   BNE   .jcmp_skip@10
   LDA   HARGS+8
   CMP   #$FF
   BEQ   .if_end@3
.jcmp_skip@10:
   LDA   __local_check__fail_0
   ORA   #$08
   STA   __local_check__fail_0
.if_end@3:
   LDA   __zpabi_check__v3
   CMP   #$FF
   BEQ   .vthread@0
   LDA   __local_check__fail_0
   ORA   #$10
   STA   __local_check__fail_0
   LDA   __zpabi_check__v3
.vthread@0:
   SEC
   SBC   __zpabi_check__v5
   LDA   __local_check__4
   SBC   __local_check__5
   BVC   .cmp_novf@11
   EOR   #$80
.cmp_novf@11:
   ASL
   LDA   #$00
   ROL
   AND   __zpabi_check__v3
   BEQ   .if_end@5
   LDA   __local_check__fail_0
   ORA   #$20
   STA   __local_check__fail_0
.if_end@5:
   LDA   __zpabi_check__v2_1
   BMI   .sx_neg@12
   LDA   #$00
   BEQ   .sx_done@13
.sx_neg@12:
   LDA   #$FF
.sx_done@13:
   STA   __local_check__1
   STA   __local_check__0
   LDA   __zpabi_check__v2_0
   AND   __zpabi_check__v4_0
   STA   __local_check__5
   LDA   __zpabi_check__v2_1
   AND   __zpabi_check__v4_1
   STA   __local_check__4
   LDA   __local_check__1
   AND   __zpabi_check__v4_2
   STA   __local_check__3
   LDA   __local_check__0
   AND   __zpabi_check__v4_3
   STA   __local_check__2
   LDA   __zpabi_check__v2_1
   BMI   .sx_neg@14
   LDA   #$00
   BEQ   .sx_done@15
.sx_neg@14:
   LDA   #$FF
.sx_done@15:
   STA   __local_check__1
   STA   __local_check__0
   LDA   __zpabi_check__v2_0
   SEC
   SBC   __zpabi_check__v1_0
   LDA   __zpabi_check__v2_1
   SBC   __zpabi_check__v1_1
   LDA   __local_check__1
   SBC   __zpabi_check__v1_2
   LDA   __local_check__0
   SBC   __zpabi_check__v1_3
   BVC   .cmp_novf@16
   EOR   #$80
.cmp_novf@16:
   ASL
   LDA   #$00
   ROL
   EOR   #$01
   STA   HARGS+4
   LDA   __local_check__5
   STA   HARGS
   LDA   __local_check__4
   STA   HARGS+1
   LDA   __local_check__3
   STA   HARGS+2
   LDA   __local_check__2
   STA   HARGS+3
   JSR   asl32
   LDA   HARGS+5
   ORA   HARGS+6
   ORA   HARGS+7
   ORA   HARGS+8
   BEQ   .if_end@7
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

   LDA   #$01
   STA   __zpabi_check__v0_0
   STA   __zpabi_check__v1_0
   LDA   #$00
   STA   __zpabi_check__v0_1
   STA   __zpabi_check__v0_2
   STA   __zpabi_check__v0_3
   STA   __zpabi_check__v1_1
   STA   __zpabi_check__v1_2
   STA   __zpabi_check__v1_3
   STA   __zpabi_check__v4_0
   STA   __zpabi_check__v4_1
   STA   __zpabi_check__v4_2
   LDA   #$96
   STA   __zpabi_check__v2_0
   LDA   #$64
   STA   __zpabi_check__v2_1
   LDA   #$FF
   STA   __zpabi_check__v3
   LDA   #$80
   STA   __zpabi_check__v4_3
   STA   __zpabi_check__v5
   JMP   check
