SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__zpabi_check__v1_0	EQU	$80
__zpabi_check__v1_1	EQU	$81
__zpabi_check__v1_2	EQU	$82
__zpabi_check__v1_3	EQU	$83
__zpabi_check__v2	EQU	$84
__zpabi_check__v3_0	EQU	$85
__zpabi_check__v3_1	EQU	$86
__zpabi_check__v4_0	EQU	$87
__zpabi_check__v4_1	EQU	$88
__zpabi_check__v4_2	EQU	$89
__zpabi_check__v4_3	EQU	$8A
__zpabi_check__v5_0	EQU	$8B
__zpabi_check__v5_1	EQU	$8C
__zpabi_check__v5_2	EQU	$8D
__zpabi_check__v5_3	EQU	$8E
__local_check__fail_1	EQU	$90
__local_check__fail_0	EQU	$91
__local_check__0	EQU	$92
__local_check__1	EQU	$93
__local_check__2	EQU	$94
__local_check__3	EQU	$95
__local_check__4	EQU	$96
__local_check__5	EQU	$97
__local_check__6	EQU	$98
__local_check__v0_1	EQU	$99
__local_check__v0_0	EQU	$9A

; @zp-link-meta-begin
; def check params=__zpabi_check__v0_0,__zpabi_check__v0_1,__zpabi_check__v1_0,__zpabi_check__v1_1,__zpabi_check__v1_2,__zpabi_check__v1_3,__zpabi_check__v2,__zpabi_check__v3_0,__zpabi_check__v3_1,__zpabi_check__v4_0,__zpabi_check__v4_1,__zpabi_check__v4_2,__zpabi_check__v4_3,__zpabi_check__v5_0,__zpabi_check__v5_1,__zpabi_check__v5_2,__zpabi_check__v5_3 locals=__local_check__fail_1,__local_check__fail_0,__local_check__0,__local_check__1,__local_check__2,__local_check__3,__local_check__4,__local_check__5,__local_check__6,__local_check__v0_1,__local_check__v0_0,__local_check__7 indirect=false in_cycle=false param_regs=A,X,,,,,,,,,,,,,,,
; def main params= locals=__local_main__0 indirect=false in_cycle=false
; call main -> check
; item check section=.text local=false zero=false
; item main section=.text local=false zero=false
; runtime hargs_size=32 hargs_need=20
; @zp-link-meta-end

check:
   SUBROUTINE

   STA   __local_check__v0_0
   STX   __local_check__v0_1
   LDA   __zpabi_check__v1_3
   CMP   #$FF
   BNE   .jcmp_skip@0
   LDA   __zpabi_check__v1_2
   CMP   #$FF
   BNE   .jcmp_skip@0
   LDA   __zpabi_check__v1_1
   CMP   #$FF
   BNE   .jcmp_skip@0
   LDA   __zpabi_check__v1_0
   CMP   #$FF
   BNE   .lb_skip@0
   JMP   .check@asm_ssa_split@0
.lb_skip@0:
.jcmp_skip@0:
   LDA   #$01
   STA   __local_check__fail_0
   LDA   #$00
   STA   __local_check__fail_1
.if_end@0:
   LDA   __local_check__v0_0
   ORA   __local_check__v0_1
   BEQ   .and_false@1
   LDA   #$FF
   STA   __local_check__0
   JMP   .and_end@2
.and_false@1:
   LDA   #$00
   LDY   #$00
   STY   __local_check__0
.and_end@2:
   SEC
   SBC   #$FF
   LDA   __local_check__0
   SBC   #$FF
   LDA   #$00
   ROL
   STA   __local_check__0
   LDA   #$00
   BNE   .jcmp_skip@1
   LDA   __local_check__0
   CMP   #$01
   BEQ   .if_end@3
.jcmp_skip@1:
   LDA   __local_check__fail_0
   ORA   #$02
   STA   __local_check__fail_0
   LDA   #$00
   STA   __local_check__fail_1
.if_end@3:
   LDA   __zpabi_check__v2
   EOR   #$FE
   AND   __zpabi_check__v4_0
   CMP   #$01
   BEQ   .if_end@4
   LDA   __local_check__fail_0
   ORA   #$04
   STA   __local_check__fail_0
.if_end@4:
   LDA   __zpabi_check__v3_1
   CMP   #$FF
   BNE   .jcmp_skip@2
   LDA   __zpabi_check__v3_0
   CMP   #$FF
   BEQ   .vthread@0
.jcmp_skip@2:
   LDA   __local_check__fail_0
   ORA   #$08
   STA   __local_check__fail_0
   LDA   __zpabi_check__v3_0
.vthread@0:
   EOR   __zpabi_check__v3_0
   STA   __local_check__1
   LDA   #$00
   AND   __zpabi_check__v2
   STA   __local_check__0
   LDA   __local_check__1
   EOR   __local_check__0
   BEQ   .if_end@6
   LDA   __local_check__fail_0
   ORA   #$10
   STA   __local_check__fail_0
.if_end@6:
   LDA   __zpabi_check__v2
   BMI   .sx_neg@3
   LDA   #$00
   BEQ   .sx_done@4
.sx_neg@3:
   LDA   #$FF
.sx_done@4:
   STA   __local_check__6
   STA   __local_check__5
   STA   __local_check__4
   LDA   __zpabi_check__v3_1
   BMI   .sx_neg@5
   LDA   #$00
   BEQ   .sx_done@6
.sx_neg@5:
   LDA   #$FF
.sx_done@6:
   STA   __local_check__1
   STA   __local_check__0
   LDA   __zpabi_check__v4_0
   CLC
   SBC   __zpabi_check__v3_0
   LDA   __zpabi_check__v4_1
   SBC   __zpabi_check__v3_1
   LDA   __zpabi_check__v4_2
   SBC   __local_check__1
   LDA   __zpabi_check__v4_3
   SBC   __local_check__0
   LDA   #$00
   ROL
   STA   HARGS+4
   LDA   __zpabi_check__v5_0
   STA   HARGS
   LDA   __zpabi_check__v5_1
   STA   HARGS+1
   LDA   __zpabi_check__v5_2
   STA   HARGS+2
   LDA   __zpabi_check__v5_3
   STA   HARGS+3
   JSR   asl32
   LDA   __local_check__4
   CMP   HARGS+8
   BNE   .if_end@7
   LDA   __local_check__5
   CMP   HARGS+7
   BNE   .if_end@7
   LDA   __local_check__6
   CMP   HARGS+6
   BNE   .if_end@7
   LDA   __zpabi_check__v2
   CMP   HARGS+5
   BNE   .if_end@7
   LDA   __local_check__fail_0
   ORA   #$20
   STA   __local_check__fail_0
.if_end@7:
   LDA   __zpabi_check__v4_1
   BNE   .jcmp_skip@7
   LDA   __zpabi_check__v4_0
   CMP   #$09
   BEQ   .if_end@8
.jcmp_skip@7:
   LDA   __local_check__fail_0
   ORA   #$40
   STA   __local_check__fail_0
.if_end@8:
   LDA   #$00
   STA   HARGS
   LDY   #$00
   STY   HARGS+1
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
   LDA   HARGS+9
   STA   __local_check__3
   LDA   HARGS+10
   STA   __local_check__2
   LDA   HARGS+11
   STA   __local_check__1
   LDA   HARGS+8
   CLC
   ADC   __zpabi_check__v2
   BCC   .carry_inc@0
   INC   __local_check__3
   BNE   .carry_inc@0
   INC   __local_check__2
   BNE   .carry_inc@0
   INC   __local_check__1
.carry_inc@0:
   ORA   __local_check__3
   ORA   __local_check__2
   ORA   __local_check__1
   BNE   .if_end@11
   LDA   __local_check__v0_0
   EOR   #$FF
   STA   __local_check__1
   LDA   __local_check__v0_1
   EOR   #$FF
   ORA   __local_check__1
   BNE   .if_end@11
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

   LDA   #$FF
   STA   __zpabi_check__v1_0
   STA   __zpabi_check__v1_1
   STA   __zpabi_check__v1_2
   STA   __zpabi_check__v1_3
   STA   __zpabi_check__v2
   STA   __zpabi_check__v3_0
   STA   __zpabi_check__v3_1
   LDA   #$09
   STA   __zpabi_check__v4_0
   LDA   #$01
   STA   __zpabi_check__v5_0
   LDA   #$00
   STA   __zpabi_check__v4_1
   STA   __zpabi_check__v4_2
   STA   __zpabi_check__v4_3
   STA   __zpabi_check__v5_1
   STA   __zpabi_check__v5_2
   STA   __zpabi_check__v5_3
   LDX   #$00
   LDA   #$01
   JMP   check
