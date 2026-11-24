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
__local_check__1	EQU	$8E
__local_check__2	EQU	$8F
__local_check__3	EQU	$90
__local_check__4	EQU	$91
__local_check__v0_1	EQU	$92
__local_check__v0_0	EQU	$93

; @zp-link-meta-begin
; def check params=__zpabi_check__v0_0,__zpabi_check__v0_1,__zpabi_check__v1_0,__zpabi_check__v1_1,__zpabi_check__v2_0,__zpabi_check__v2_1,__zpabi_check__v3,__zpabi_check__v4,__zpabi_check__v5_0,__zpabi_check__v5_1,__zpabi_check__v5_2,__zpabi_check__v5_3 locals=__local_check__fail_1,__local_check__fail_0,__local_check__0,__local_check__1,__local_check__2,__local_check__3,__local_check__4,__local_check__v0_1,__local_check__v0_0 indirect=false in_cycle=false param_regs=A,X,,,,,,,,,,
; def main params= locals=__local_main__0 indirect=false in_cycle=false
; call main -> check
; item check section=.text local=false zero=false
; item main section=.text local=false zero=false
; runtime hargs_size=32 hargs_need=9
; @zp-link-meta-end

check:
   SUBROUTINE

   STA   __local_check__v0_0
   STX   __local_check__v0_1
   LDA   __zpabi_check__v5_0
   SEC
   SBC   __zpabi_check__v4
   STA   __local_check__1
   LDA   __zpabi_check__v5_1
   SBC   #$00
   STA   __local_check__0
   LDA   __zpabi_check__v5_2
   SBC   #$00
   STA   __local_check__fail_0
   LDA   __zpabi_check__v5_3
   SBC   #$00
   CMP   #$CB
   BNE   .jcmp_skip@0
   LDA   __local_check__fail_0
   CMP   #$0A
   BNE   .jcmp_skip@0
   LDA   __local_check__0
   CMP   #$7C
   BNE   .jcmp_skip@0
   LDA   __local_check__1
   CMP   #$F9
   BNE   .lb_skip@0
   JMP   .check@asm_ssa_split@0
.lb_skip@0:
.jcmp_skip@0:
   LDA   #$01
   STA   __local_check__fail_0
   LDA   #$00
   STA   __local_check__fail_1
.if_end@0:
   LDA   __zpabi_check__v2_0
   STA   HARGS
   LDA   __zpabi_check__v2_1
   STA   HARGS+1
   LDA   __zpabi_check__v1_0
   STA   HARGS+2
   LDA   __zpabi_check__v1_1
   STA   HARGS+3
   JSR   udivmod16
   LDA   HARGS+7
   CMP   #$E2
   BNE   .jcmp_skip@1
   LDA   HARGS+6
   CMP   #$A0
   BEQ   .if_end@1
.jcmp_skip@1:
   LDA   __local_check__fail_0
   ORA   #$02
   STA   __local_check__fail_0
.if_end@1:
   LDA   __zpabi_check__v3
   BMI   .sx_neg@2
   LDA   #$00
   BEQ   .sx_done@3
.sx_neg@2:
   LDA   #$FF
.sx_done@3:
   STA   __local_check__4
   LDA   __zpabi_check__v3
   EOR   #$FF
   STA   __local_check__1
   LDA   __local_check__4
   EOR   #$FF
   STA   __local_check__0
   LDA   __local_check__1
   EOR   #$FF
   STA   __local_check__1
   LDA   __local_check__0
   EOR   #$FF
   STA   __local_check__0
   LDA   __local_check__1
   SEC
   SBC   __local_check__v0_0
   STA   __local_check__1
   LDA   __local_check__0
   SBC   __local_check__v0_1
   BNE   .jcmp_skip@4
   LDA   __local_check__1
   CMP   #$04
   BEQ   .if_end@2
.jcmp_skip@4:
   LDA   __local_check__fail_0
   ORA   #$04
   STA   __local_check__fail_0
.if_end@2:
   LDA   __zpabi_check__v5_0
   AND   __local_check__v0_0
   STA   __local_check__3
   LDA   __zpabi_check__v5_1
   AND   __local_check__v0_1
   BMI   .jcmp_skip@7
   BNE   .jcmp_skip@7
   LDA   __local_check__3
   CMP   #$09
   BEQ   .if_end@5
.jcmp_skip@7:
   LDA   __local_check__fail_0
   ORA   #$20
   STA   __local_check__fail_0
.if_end@5:
   LDA   __zpabi_check__v5_0
   LSR
   STA   __local_check__1
   LDA   #$00
   ROR
   ORA   __local_check__1
   BEQ   .vthread@1
   LDA   __local_check__v0_1
   CMP   __local_check__4
   BNE   .cmp_true@8
   LDA   __local_check__v0_0
   CMP   __zpabi_check__v3
   BNE   .cmp_true@8
   LDA   #$00
   BEQ   .cmp_end@9
.cmp_true@8:
   LDA   #$01
.cmp_end@9:
   STA   __local_check__2
   LDA   #$00
   BPL   .sx_done@12
   LDA   #$FF
.sx_done@12:
   STA   __local_check__1
   STA   __local_check__0
   LDA   __zpabi_check__v5_0
   ORA   __local_check__2
   STA   __local_check__3
   LDA   __zpabi_check__v5_1
   STA   __local_check__2
   LDA   __zpabi_check__v5_2
   ORA   __local_check__1
   STA   __local_check__1
   LDA   __zpabi_check__v5_3
   ORA   __local_check__0
   STA   __local_check__0
   LDA   __local_check__3
   ORA   __local_check__2
   ORA   __local_check__1
   ORA   __local_check__0
   BNE   .if_end@8
.vthread@1:
   LDA   __local_check__fail_0
   ORA   #$40
   STA   __local_check__fail_0
.if_end@8:
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
   LDA   #$A0
   STA   __zpabi_check__v2_0
   LDA   #$E2
   STA   __zpabi_check__v2_1
   LDA   #$0D
   STA   __zpabi_check__v3
   LDA   #$00
   STA   __zpabi_check__v4
   LDA   #$F9
   STA   __zpabi_check__v5_0
   LDA   #$7C
   STA   __zpabi_check__v5_1
   LDA   #$0A
   STA   __zpabi_check__v5_2
   LDA   #$CB
   STA   __zpabi_check__v5_3
   LDX   #$00
   LDA   #$09
   JMP   check
