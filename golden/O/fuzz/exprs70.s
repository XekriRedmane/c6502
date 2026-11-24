SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__zpabi_check__v1_0	EQU	$80
__zpabi_check__v1_1	EQU	$81
__zpabi_check__v2	EQU	$82
__zpabi_check__v3_0	EQU	$83
__zpabi_check__v3_1	EQU	$84
__zpabi_check__v3_2	EQU	$85
__zpabi_check__v3_3	EQU	$86
__zpabi_check__v4_0	EQU	$87
__zpabi_check__v4_1	EQU	$88
__zpabi_check__v5_0	EQU	$89
__zpabi_check__v5_1	EQU	$8A
__local_check__fail_1	EQU	$8C
__local_check__fail_0	EQU	$8D
__local_check__0	EQU	$8E
__local_check__1	EQU	$8F
__local_check__2	EQU	$90
__local_check__3	EQU	$91
__local_check__7	EQU	$95
__local_check__v0	EQU	$96

; @zp-link-meta-begin
; def check params=__zpabi_check__v0,__zpabi_check__v1_0,__zpabi_check__v1_1,__zpabi_check__v2,__zpabi_check__v3_0,__zpabi_check__v3_1,__zpabi_check__v3_2,__zpabi_check__v3_3,__zpabi_check__v4_0,__zpabi_check__v4_1,__zpabi_check__v5_0,__zpabi_check__v5_1 locals=__local_check__fail_1,__local_check__fail_0,__local_check__0,__local_check__1,__local_check__2,__local_check__3,__local_check__4,__local_check__5,__local_check__6,__local_check__7,__local_check__v0 indirect=false in_cycle=false param_regs=A,,,,,,,,,,,
; def main params= locals=__local_main__0 indirect=false in_cycle=false
; call main -> check
; item check section=.text local=false zero=false
; item main section=.text local=false zero=false
; runtime hargs_size=32 hargs_need=20
; @zp-link-meta-end

check:
   SUBROUTINE

   STA   __local_check__v0
   ORA   #$00
   BMI   .sx_neg@0
   LDA   #$00
   BEQ   .sx_done@1
.sx_neg@0:
   LDA   #$FF
.sx_done@1:
   STA   __local_check__7
   LDA   __local_check__v0
   EOR   #$FF
   STA   HARGS+2
   LDA   __local_check__7
   EOR   #$FF
   STA   HARGS+3
   LDA   __zpabi_check__v2
   STA   HARGS
   LDA   #$00
   STA   HARGS+1
   JSR   mul16
   LDA   #$00
   SEC
   SBC   HARGS+4
   STA   __local_check__fail_0
   LDA   HARGS+4
   SBC   HARGS+5
   ORA   __local_check__fail_0
   BNE   .lb_skip@0
   JMP   .check@asm_ssa_split@0
.lb_skip@0:
   LDA   #$01
   STA   __local_check__fail_0
   LDA   #$00
   STA   __local_check__fail_1
.if_end@0:
   LDY   #$00
   STY   HARGS
   STY   HARGS+1
   STA   HARGS+2
   LDA   #$80
   STA   HARGS+3
   LDA   __zpabi_check__v1_0
   STA   HARGS+4
   LDA   __zpabi_check__v1_1
   STA   HARGS+5
   TYA
   STA   HARGS+6
   STA   HARGS+7
   JSR   udivmod32
   LDA   __local_check__v0
   BMI   .sx_neg@2
   LDA   #$00
   BEQ   .sx_done@3
.sx_neg@2:
   LDA   #$FF
.sx_done@3:
   STA   __local_check__3
   STA   __local_check__2
   STA   __local_check__1
   LDA   HARGS+8
   SEC
   SBC   __local_check__v0
   STA   __local_check__0
   LDA   HARGS+9
   SBC   __local_check__3
   STA   __local_check__3
   LDA   HARGS+10
   SBC   __local_check__2
   STA   __local_check__2
   LDA   HARGS+11
   SBC   __local_check__1
   STA   __local_check__1
   LDA   #$01
   ORA   __local_check__0
   STA   __local_check__0
   LDA   __local_check__1
   BNE   .jcmp_skip@4
   LDA   __local_check__2
   CMP   #$03
   BNE   .jcmp_skip@4
   LDA   __local_check__3
   CMP   #$AA
   BNE   .jcmp_skip@4
   LDA   __local_check__0
   CMP   #$BF
   BEQ   .if_end@5
.jcmp_skip@4:
   LDA   __local_check__fail_0
   ORA   #$02
   STA   __local_check__fail_0
.if_end@5:
   LDA   __local_check__v0
   ORA   __local_check__7
   BEQ   .jcmp_skip@5
   LDA   __zpabi_check__v5_0
   SEC
   SBC   #$80
   STA   __local_check__3
   LDA   __zpabi_check__v5_1
   SBC   #$00
   STA   __local_check__2
   LDA   #$00
   SBC   #$00
   STA   __local_check__1
   LDA   #$00
   SBC   #$00
   STA   __local_check__0
   LDA   __local_check__3
   ORA   __local_check__2
   ORA   __local_check__1
   ORA   __local_check__0
   BNE   .if_end@8
.jcmp_skip@5:
   LDA   __local_check__fail_0
   ORA   #$04
   STA   __local_check__fail_0
.if_end@8:
   LDA   __zpabi_check__v1_0
   CMP   #$E8
   BEQ   .if_end@9
   LDA   __local_check__fail_0
   ORA   #$08
   STA   __local_check__fail_0
.if_end@9:
   LDA   __zpabi_check__v3_3
   BPL   .if_end@12
   LDA   __local_check__fail_0
   ORA   #$10
   STA   __local_check__fail_0
.if_end@12:
   LDA   __zpabi_check__v1_1
   CMP   #$22
   BNE   .jcmp_skip@6
   LDA   __zpabi_check__v1_0
   CMP   #$E8
   BEQ   .if_end@13
.jcmp_skip@6:
   LDA   __local_check__fail_0
   ORA   #$20
   STA   __local_check__fail_0
.if_end@13:
   LDA   #$00
   CMP   __zpabi_check__v1_1
   BNE   .cmp_end@8
   LDA   __zpabi_check__v2
   CMP   __zpabi_check__v1_0
   BEQ   .cmp_true@7
   LDA   #$00
   BEQ   .cmp_end@8
.cmp_true@7:
   LDA   #$01
.cmp_end@8:
   ORA   #$00
   STA   __local_check__1
   LDA   #$00
   CMP   __zpabi_check__v4_1
   BNE   .cmp_true@10
   LDA   __local_check__1
   CMP   __zpabi_check__v4_0
   BNE   .cmp_true@10
   LDA   #$00
   BEQ   .cmp_end@11
.cmp_true@10:
   LDA   #$01
.cmp_end@11:
   STA   __local_check__0
   LDA   #$00
   BNE   .jcmp_skip@13
   LDA   __local_check__0
   CMP   #$01
   BEQ   .if_end@14
.jcmp_skip@13:
   LDA   __local_check__fail_0
   ORA   #$40
   STA   __local_check__fail_0
.if_end@14:
   SEC
   LDA   #$00
   LDA   #$00
   SBC   #$80
   BCS   .and_false@15
   LDA   __zpabi_check__v5_0
   SEC
   SBC   #$FF
   STA   __local_check__1
   LDA   __zpabi_check__v5_1
   SBC   #$00
   ORA   __local_check__1
   BNE   .and_end@16
.and_false@15:
.and_end@16:
   SEC
   LDA   #$00
   ROL
   STA   __local_check__0
   LDA   #$00
   BNE   .jcmp_skip@14
   LDA   __local_check__0
   CMP   #$01
   BEQ   .if_end@17
.jcmp_skip@14:
   LDA   __local_check__fail_0
   ORA   #$80
   STA   __local_check__fail_0
.if_end@17:
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

   LDA   #$E8
   STA   __zpabi_check__v1_0
   LDA   #$22
   STA   __zpabi_check__v1_1
   LDA   #$06
   STA   __zpabi_check__v4_0
   LDA   #$01
   STA   __zpabi_check__v5_0
   LDA   #$00
   STA   __zpabi_check__v2
   STA   __zpabi_check__v3_0
   STA   __zpabi_check__v3_1
   STA   __zpabi_check__v3_2
   STA   __zpabi_check__v3_3
   STA   __zpabi_check__v4_1
   STA   __zpabi_check__v5_1
   LDA   #$FF
   JMP   check
