SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__zpabi_check__v1_0	EQU	$80
__zpabi_check__v1_1	EQU	$81
__zpabi_check__v2_0	EQU	$82
__zpabi_check__v2_1	EQU	$83
__zpabi_check__v2_2	EQU	$84
__zpabi_check__v2_3	EQU	$85
__zpabi_check__v3	EQU	$86
__zpabi_check__v4	EQU	$87
__zpabi_check__v5_0	EQU	$88
__zpabi_check__v5_1	EQU	$89
__local_check__fail_1	EQU	$8B
__local_check__fail_0	EQU	$8C
__local_check__0	EQU	$8D
__local_check__1	EQU	$8E
__local_check__2	EQU	$8F
__local_check__4	EQU	$91
__local_check__v0_1	EQU	$92
__local_check__v0_0	EQU	$93

; @zp-link-meta-begin
; def check params=__zpabi_check__v0_0,__zpabi_check__v0_1,__zpabi_check__v1_0,__zpabi_check__v1_1,__zpabi_check__v2_0,__zpabi_check__v2_1,__zpabi_check__v2_2,__zpabi_check__v2_3,__zpabi_check__v3,__zpabi_check__v4,__zpabi_check__v5_0,__zpabi_check__v5_1 locals=__local_check__fail_1,__local_check__fail_0,__local_check__0,__local_check__1,__local_check__2,__local_check__3,__local_check__4,__local_check__v0_1,__local_check__v0_0 indirect=false in_cycle=false param_regs=A,X,,,,,,,,,,
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
   LDA   __zpabi_check__v3
   BMI   .sx_neg@0
   LDA   #$00
   BEQ   .sx_done@1
.sx_neg@0:
   LDA   #$FF
.sx_done@1:
   STA   __local_check__fail_1
   LDA   #$80
   SEC
   SBC   __zpabi_check__v3
   LDA   #$FF
   SBC   __local_check__fail_1
   BVC   .cmp_novf@2
   EOR   #$80
.cmp_novf@2:
   ASL
   LDA   #$00
   ROL
   STA   __local_check__fail_1
   LDA   #$00
   SEC
   SBC   __local_check__fail_1
   LDA   #$00
   SBC   #$00
   BVC   .cmp_novf@3
   EOR   #$80
.cmp_novf@3:
   ASL
   LDA   #$00
   ROL
   STA   __local_check__0
   LDA   #$00
   BPL   .vthread@0
   LDA   #$FF
   BNE   .jcmp_skip@6
   LDA   #$00
   BNE   .jcmp_skip@6
.vthread@0:
   LDA   __local_check__0
   CMP   #$01
   BNE   .lb_skip@0
   JMP   .check@asm_ssa_split@0
.lb_skip@0:
.jcmp_skip@6:
   LDA   #$01
   STA   __local_check__fail_0
   LDA   #$00
   STA   __local_check__fail_1
.if_end@0:
   LDA   __zpabi_check__v1_0
   ORA   __zpabi_check__v1_1
   BNE   .or_true@1
   LDA   __zpabi_check__v2_0
   ORA   __zpabi_check__v2_1
   ORA   __zpabi_check__v2_2
   ORA   __zpabi_check__v2_3
   BNE   .or_true@1
   LDA   #$00
   STA   __local_check__4
   JMP   .or_end@2
.or_true@1:
   LDA   #$01
   STA   __local_check__4
.or_end@2:
   LDA   __zpabi_check__v4
   BMI   .sx_neg@7
   LDA   #$00
   BEQ   .sx_done@8
.sx_neg@7:
   LDA   #$FF
.sx_done@8:
   STA   __local_check__2
   LDA   __local_check__v0_0
   ORA   __local_check__v0_1
   BEQ   .and_false@5
   LDA   __zpabi_check__v5_0
   ORA   __zpabi_check__v5_1
   BEQ   .and_false@5
   LDA   #$01
   STA   __local_check__1
   JMP   .and_end@6
.and_false@5:
   LDA   #$00
   STA   __local_check__1
.and_end@6:
   LDA   __local_check__2
   BNE   .cmp_true@9
   LDA   __zpabi_check__v4
   CMP   __local_check__1
   BNE   .cmp_true@9
   LDA   #$00
   BEQ   .cmp_end@10
.cmp_true@9:
   LDA   #$01
.cmp_end@10:
   STA   __local_check__0
   LDA   __local_check__4
   SEC
   SBC   __local_check__0
   LDA   #$00
   SBC   #$00
   BVC   .jcmp_novf@12
   EOR   #$80
.jcmp_novf@12:
   BPL   .if_end@7
   LDA   __local_check__fail_0
   ORA   #$02
   STA   __local_check__fail_0
   LDA   #$00
   STA   __local_check__fail_1
.if_end@7:
   LDA   __zpabi_check__v5_0
   STA   HARGS
   LDA   __zpabi_check__v5_1
   STA   HARGS+1
   LDA   __zpabi_check__v1_0
   STA   HARGS+2
   LDA   __zpabi_check__v1_1
   STA   HARGS+3
   JSR   udivmod16
   LDA   HARGS+7
   CMP   #$7F
   BNE   .jcmp_skip@13
   LDA   HARGS+6
   CMP   #$FF
   BEQ   .if_end@8
.jcmp_skip@13:
   LDA   __local_check__fail_0
   ORA   #$04
   STA   __local_check__fail_0
.if_end@8:
   LDA   __local_check__v0_0
   ORA   __local_check__v0_1
   BNE   .if_end@12
   LDA   __zpabi_check__v4
   AND   __local_check__v0_0
   STA   HARGS+2
   LDA   __local_check__2
   AND   __local_check__v0_1
   STA   HARGS+3
   LDA   #$FF
   STA   HARGS
   STA   HARGS+1
   JSR   udivmod16
   LDA   HARGS+6
   ORA   HARGS+7
   BNE   .if_end@12
   LDA   __local_check__fail_0
   ORA   #$10
   STA   __local_check__fail_0
.if_end@12:
   LDA   __zpabi_check__v3
   SEC
   SBC   __zpabi_check__v4
   BVC   .jcmp_novf@14
   EOR   #$80
.jcmp_novf@14:
   BPL   .if_end@13
   LDA   __local_check__fail_0
   ORA   #$20
   STA   __local_check__fail_0
.if_end@13:
   LDA   __zpabi_check__v3
   BEQ   .if_end@14
   LDA   __local_check__fail_0
   ORA   #$40
   STA   __local_check__fail_0
.if_end@14:
   LDA   __zpabi_check__v1_0
   STA   __local_check__0
   LDA   __zpabi_check__v1_1
   STA   __local_check__1
   LDX   __zpabi_check__v3
   BEQ   .shift_done@16
.shift_loop@15:
   LSR   __local_check__1
   ROR   __local_check__0
   DEX
   BNE   .shift_loop@15
.shift_done@16:
   LDA   __local_check__v0_1
   CMP   __local_check__1
   BNE   .if_end@15
   LDA   __local_check__v0_0
   CMP   __local_check__0
   BNE   .if_end@15
   LDA   __local_check__fail_0
   ORA   #$80
   STA   __local_check__fail_0
.if_end@15:
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
   STA   __zpabi_check__v1_0
   STA   __zpabi_check__v2_1
   STA   __zpabi_check__v2_2
   STA   __zpabi_check__v2_3
   STA   __zpabi_check__v3
   LDA   #$80
   STA   __zpabi_check__v1_1
   LDA   #$01
   STA   __zpabi_check__v2_0
   LDA   #$F5
   STA   __zpabi_check__v4
   LDA   #$FF
   STA   __zpabi_check__v5_0
   STA   __zpabi_check__v5_1
   LDX   #$00
   LDA   #$01
   JMP   check
