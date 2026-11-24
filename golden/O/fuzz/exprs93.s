SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__zpabi_check__v1_0	EQU	$80
__zpabi_check__v1_1	EQU	$81
__zpabi_check__v2_0	EQU	$82
__zpabi_check__v2_1	EQU	$83
__zpabi_check__v3	EQU	$84
__zpabi_check__v4	EQU	$85
__zpabi_check__v5_0	EQU	$86
__zpabi_check__v5_1	EQU	$87
__zpabi_check__v5_2	EQU	$88
__zpabi_check__v5_3	EQU	$89
__local_check__fail_1	EQU	$8B
__local_check__fail_0	EQU	$8C
__local_check__0	EQU	$8D
__local_check__v0_1	EQU	$8E
__local_check__v0_0	EQU	$8F
__local_check__1	EQU	$90
__local_check__2	EQU	$91
__local_check__3	EQU	$92

; @zp-link-meta-begin
; def check params=__zpabi_check__v0_0,__zpabi_check__v0_1,__zpabi_check__v1_0,__zpabi_check__v1_1,__zpabi_check__v2_0,__zpabi_check__v2_1,__zpabi_check__v3,__zpabi_check__v4,__zpabi_check__v5_0,__zpabi_check__v5_1,__zpabi_check__v5_2,__zpabi_check__v5_3 locals=__local_check__fail_1,__local_check__fail_0,__local_check__0,__local_check__v0_1,__local_check__v0_0,__local_check__1,__local_check__2,__local_check__3,__local_check__4,__local_check__5 indirect=false in_cycle=false param_regs=A,X,,,,,,,,,,
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
   LDA   #$01
   STA   HARGS
   LDA   #$00
   LDX   #$08
   LSR   HARGS
.mul_loop@0:
   BCC   .mul_skip@1
   CLC
.mul_skip@1:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@0
   STA   __local_check__0
   LDA   HARGS
   EOR   #$FF
   STA   __local_check__fail_0
   LDA   __local_check__0
   EOR   #$FF
   CMP   #$FF
   BNE   .jcmp_skip@2
   LDA   __local_check__fail_0
   CMP   #$FF
   BNE   .lb_skip@0
   JMP   .check@asm_ssa_split@0
.lb_skip@0:
.jcmp_skip@2:
   LDA   #$02
   STA   __local_check__fail_0
   LDA   #$00
   STA   __local_check__fail_1
.if_end@1:
   LDA   __local_check__v0_0
   ORA   __local_check__v0_1
   BNE   .if_end@6
   LDA   __zpabi_check__v4
   BEQ   .check@ssa_block@6
   LDA   __zpabi_check__v1_0
   ORA   __zpabi_check__v1_1
   BNE   .if_end@6
.check@ssa_block@6:
   LDA   __local_check__fail_0
   ORA   #$04
   STA   __local_check__fail_0
   LDA   #$00
   STA   __local_check__fail_1
.if_end@6:
   LDA   __zpabi_check__v2_0
   AND   __zpabi_check__v5_0
   STA   __local_check__v0_1
   LDA   __zpabi_check__v2_1
   AND   __zpabi_check__v5_1
   STA   __local_check__0
   LDA   __zpabi_check__v1_0
   SEC
   SBC   __local_check__v0_1
   LDA   __zpabi_check__v1_1
   SBC   __local_check__0
   BCS   .if_end@7
   LDA   __local_check__fail_0
   ORA   #$08
   STA   __local_check__fail_0
.if_end@7:
   LDA   __zpabi_check__v3
   BMI   .sx_neg@3
   LDA   #$00
   BEQ   .sx_done@4
.sx_neg@3:
   LDA   #$FF
.sx_done@4:
   STA   __local_check__3
   STA   HARGS+1
   LDA   __zpabi_check__v3
   STA   HARGS
   LDA   #$07
   STA   HARGS+2
   LDA   #$00
   STA   HARGS+3
   JSR   udivmod16
   LDA   HARGS+6
   EOR   #$FF
   STA   __local_check__v0_1
   LDA   HARGS+7
   EOR   #$FF
   CMP   #$FF
   BNE   .jcmp_skip@5
   LDA   __local_check__v0_1
   CMP   #$FE
   BEQ   .if_end@8
.jcmp_skip@5:
   LDA   __local_check__fail_0
   ORA   #$10
   STA   __local_check__fail_0
.if_end@8:
   SEC
   LDA   #$00
   SBC   __zpabi_check__v3
   STA   __local_check__1
   LDA   #$00
   SBC   __local_check__3
   STA   __local_check__v0_0
   LDA   #$00
   STA   HARGS
   STA   HARGS+1
   LDA   __zpabi_check__v2_0
   STA   HARGS+2
   LDA   __zpabi_check__v2_1
   STA   HARGS+3
   JSR   sdivmod16
   LDA   __local_check__1
   SEC
   SBC   HARGS+4
   STA   HARGS+2
   LDA   __local_check__v0_0
   SBC   HARGS+5
   STA   HARGS+3
   LDA   __zpabi_check__v3
   STA   HARGS
   LDA   __local_check__3
   STA   HARGS+1
   JSR   sdivmod16
   LDA   HARGS+5
   CMP   #$FF
   BNE   .jcmp_skip@6
   LDA   HARGS+4
   CMP   #$FF
   BEQ   .if_end@9
.jcmp_skip@6:
   LDA   __local_check__fail_0
   ORA   #$20
   STA   __local_check__fail_0
.if_end@9:
   LDA   __zpabi_check__v1_1
   BMI   .sx_neg@7
   LDA   #$00
   BEQ   .sx_done@8
.sx_neg@7:
   LDA   #$FF
.sx_done@8:
   STA   __local_check__2
   LDA   #$FB
   SEC
   SBC   __zpabi_check__v3
   LDA   #$FF
   SBC   __local_check__3
   BVC   .cmp_novf@9
   EOR   #$80
.cmp_novf@9:
   ASL
   LDA   #$00
   ROL
   STA   __local_check__v0_0
   LDA   #$00
   BPL   .sx_done@11
   LDA   #$FF
.sx_done@11:
   STA   __local_check__v0_1
   STA   __local_check__0
   LDA   #$00
   SEC
   SBC   __local_check__v0_0
   STA   HARGS+4
   LDA   #$00
   SBC   #$00
   STA   HARGS+5
   LDA   __local_check__v0_0
   SBC   __local_check__v0_1
   STA   HARGS+6
   LDA   #$00
   SBC   __local_check__0
   STA   HARGS+7
   LDA   __zpabi_check__v1_0
   STA   HARGS
   LDA   __zpabi_check__v1_1
   STA   HARGS+1
   LDA   __local_check__2
   STA   HARGS+2
   STA   HARGS+3
   JSR   udivmod32
   LDA   HARGS+15
   BNE   .jcmp_skip@12
   LDA   HARGS+14
   BNE   .jcmp_skip@12
   LDA   HARGS+13
   CMP   #$E2
   BNE   .jcmp_skip@12
   LDA   HARGS+12
   CMP   #$15
   BEQ   .if_end@10
.jcmp_skip@12:
   LDA   __local_check__fail_0
   ORA   #$40
   STA   __local_check__fail_0
.if_end@10:
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

   LDA   #$15
   STA   __zpabi_check__v1_0
   LDA   #$E2
   STA   __zpabi_check__v1_1
   LDA   #$FF
   STA   __zpabi_check__v2_0
   STA   __zpabi_check__v2_1
   LDA   #$01
   STA   __zpabi_check__v3
   LDA   #$9E
   STA   __zpabi_check__v4
   LDA   #$00
   STA   __zpabi_check__v5_0
   STA   __zpabi_check__v5_1
   STA   __zpabi_check__v5_2
   LDA   #$80
   STA   __zpabi_check__v5_3
   LDX   #$00
   TXA
   JMP   check
