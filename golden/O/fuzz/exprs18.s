SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__zpabi_check__v1_0	EQU	$80
__zpabi_check__v1_1	EQU	$81
__zpabi_check__v1_2	EQU	$82
__zpabi_check__v1_3	EQU	$83
__zpabi_check__v2_0	EQU	$84
__zpabi_check__v2_1	EQU	$85
__zpabi_check__v3_0	EQU	$86
__zpabi_check__v3_1	EQU	$87
__zpabi_check__v4_0	EQU	$88
__zpabi_check__v4_1	EQU	$89
__zpabi_check__v4_2	EQU	$8A
__zpabi_check__v4_3	EQU	$8B
__zpabi_check__v5_0	EQU	$8C
__zpabi_check__v5_1	EQU	$8D
__zpabi_check__v5_2	EQU	$8E
__zpabi_check__v5_3	EQU	$8F
__local_check__fail_1	EQU	$91
__local_check__fail_0	EQU	$92
__local_check__0	EQU	$93
__local_check__1	EQU	$94
__local_check__2	EQU	$95
__local_check__3	EQU	$96
__local_check__4	EQU	$97
__local_check__5	EQU	$98
__local_check__v0_1	EQU	$99
__local_check__v0_0	EQU	$9A

; @zp-link-meta-begin
; def check params=__zpabi_check__v0_0,__zpabi_check__v0_1,__zpabi_check__v1_0,__zpabi_check__v1_1,__zpabi_check__v1_2,__zpabi_check__v1_3,__zpabi_check__v2_0,__zpabi_check__v2_1,__zpabi_check__v3_0,__zpabi_check__v3_1,__zpabi_check__v4_0,__zpabi_check__v4_1,__zpabi_check__v4_2,__zpabi_check__v4_3,__zpabi_check__v5_0,__zpabi_check__v5_1,__zpabi_check__v5_2,__zpabi_check__v5_3 locals=__local_check__fail_1,__local_check__fail_0,__local_check__0,__local_check__1,__local_check__2,__local_check__3,__local_check__4,__local_check__5,__local_check__v0_1,__local_check__v0_0 indirect=false in_cycle=false param_regs=A,X,,,,,,,,,,,,,,,,
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
   LDA   __zpabi_check__v4_0
   AND   __zpabi_check__v5_0
   STA   __local_check__1
   LDA   __zpabi_check__v4_1
   AND   __zpabi_check__v5_1
   STA   __local_check__0
   LDA   __zpabi_check__v4_2
   AND   __zpabi_check__v5_2
   STA   __local_check__fail_0
   LDA   __zpabi_check__v4_3
   AND   __zpabi_check__v5_3
   BNE   .jcmp_skip@0
   LDA   __local_check__fail_0
   BNE   .jcmp_skip@0
   LDA   __local_check__0
   BNE   .jcmp_skip@0
   LDA   __local_check__1
   CMP   #$08
   BNE   .lb_skip@0
   JMP   .check@asm_ssa_split@0
.lb_skip@0:
.jcmp_skip@0:
   LDA   #$01
   STA   __local_check__fail_0
   LDA   #$00
   STA   __local_check__fail_1
.if_end@0:
   LDA   __zpabi_check__v3_1
   BNE   .jcmp_skip@1
   LDA   __zpabi_check__v3_0
   CMP   #$04
   BEQ   .if_end@1
.jcmp_skip@1:
   LDA   __local_check__fail_0
   ORA   #$02
   STA   __local_check__fail_0
   LDA   #$00
   STA   __local_check__fail_1
.if_end@1:
   LDA   __zpabi_check__v4_3
   CMP   #$E6
   BNE   .jcmp_skip@2
   LDA   __zpabi_check__v4_2
   CMP   #$34
   BNE   .jcmp_skip@2
   LDA   __zpabi_check__v4_1
   CMP   #$2C
   BNE   .jcmp_skip@2
   LDA   __zpabi_check__v4_0
   CMP   #$1C
   BEQ   .if_end@2
.jcmp_skip@2:
   LDA   __local_check__fail_0
   ORA   #$04
   STA   __local_check__fail_0
.if_end@2:
   LDA   __zpabi_check__v1_0
   ORA   __zpabi_check__v1_1
   ORA   __zpabi_check__v1_2
   ORA   __zpabi_check__v1_3
   BNE   .or_true@4
   LDA   __zpabi_check__v2_1
   BMI   .sx_neg@3
   LDA   #$00
   BEQ   .sx_done@4
.sx_neg@3:
   LDA   #$FF
.sx_done@4:
   STA   __local_check__1
   STA   __local_check__0
   LDA   __zpabi_check__v5_0
   SEC
   SBC   __zpabi_check__v2_0
   LDA   __zpabi_check__v5_1
   SBC   __zpabi_check__v2_1
   LDA   __zpabi_check__v5_2
   SBC   __local_check__1
   LDA   __zpabi_check__v5_3
   SBC   __local_check__0
   BCS   .or_true@4
   LDA   #$00
   STA   __local_check__5
   STA   __local_check__4
   JMP   .or_end@5
.or_true@4:
   LDA   #$01
   STA   __local_check__5
   LDA   #$00
   STA   __local_check__4
.or_end@5:
   LDA   __local_check__v0_0
   EOR   #$FF
   STA   __local_check__3
   TXA
   EOR   #$FF
   STA   __local_check__2
   LDA   __zpabi_check__v3_1
   BMI   .sx_neg@5
   LDA   #$00
   BEQ   .sx_done@6
.sx_neg@5:
   LDA   #$FF
.sx_done@6:
   STA   __local_check__1
   STA   __local_check__0
   LDA   __zpabi_check__v1_0
   CLC
   SBC   __zpabi_check__v3_0
   LDA   __zpabi_check__v1_1
   SBC   __zpabi_check__v3_1
   LDA   __zpabi_check__v1_2
   SBC   __local_check__1
   LDA   __zpabi_check__v1_3
   SBC   __local_check__0
   LDA   #$00
   ROL
   STA   HARGS+2
   LDA   __local_check__3
   STA   HARGS
   LDA   __local_check__2
   STA   HARGS+1
   LDA   #$00
   STA   HARGS+3
   JSR   sdivmod16
   LDA   __local_check__5
   SEC
   SBC   HARGS+6
   LDA   __local_check__4
   SBC   HARGS+7
   BVC   .jcmp_novf@7
   EOR   #$80
.jcmp_novf@7:
   BPL   .if_end@6
   LDA   __local_check__fail_0
   ORA   #$10
   STA   __local_check__fail_0
.if_end@6:
   LDA   __local_check__v0_1
   BMI   .sx_neg@8
   LDA   #$00
   BEQ   .sx_done@9
.sx_neg@8:
   LDA   #$FF
.sx_done@9:
   STA   __local_check__1
   STA   __local_check__0
   LDA   __local_check__v0_0
   EOR   __zpabi_check__v4_0
   STA   __local_check__3
   LDA   __local_check__v0_1
   EOR   __zpabi_check__v4_1
   STA   __local_check__2
   LDA   __local_check__1
   EOR   __zpabi_check__v4_2
   STA   __local_check__1
   LDA   __local_check__0
   EOR   __zpabi_check__v4_3
   STA   __local_check__0
   LDA   __zpabi_check__v4_0
   SEC
   SBC   __local_check__3
   LDA   __zpabi_check__v4_1
   SBC   __local_check__2
   LDA   __zpabi_check__v4_2
   SBC   __local_check__1
   LDA   __zpabi_check__v4_3
   SBC   __local_check__0
   BVC   .cmp_novf@10
   EOR   #$80
.cmp_novf@10:
   ASL
   LDA   #$00
   ROL
   STA   __local_check__0
   LDA   #$00
   BPL   .sx_done@12
   LDA   #$FF
.sx_done@12:
   STA   HARGS+6
   STA   HARGS+7
   LDA   __zpabi_check__v4_0
   STA   HARGS
   LDA   __zpabi_check__v4_1
   STA   HARGS+1
   LDA   __zpabi_check__v4_2
   STA   HARGS+2
   LDA   __zpabi_check__v4_3
   STA   HARGS+3
   LDA   __local_check__0
   STA   HARGS+4
   LDA   #$00
   STA   HARGS+5
   JSR   mul32
   LDA   HARGS+11
   CMP   #$E6
   BNE   .jcmp_skip@13
   LDA   HARGS+10
   CMP   #$34
   BNE   .jcmp_skip@13
   LDA   HARGS+9
   CMP   #$2C
   BNE   .jcmp_skip@13
   LDA   HARGS+8
   CMP   #$1C
   BEQ   .if_end@7
.jcmp_skip@13:
   LDA   __local_check__fail_0
   ORA   #$20
   STA   __local_check__fail_0
.if_end@7:
   LDA   __zpabi_check__v2_1
   CMP   #$E0
   BNE   .jcmp_skip@14
   LDA   __zpabi_check__v2_0
   BEQ   .if_end@8
.jcmp_skip@14:
   LDA   __local_check__fail_0
   ORA   #$40
   STA   __local_check__fail_0
.if_end@8:
   LDA   __zpabi_check__v3_1
   BNE   .jcmp_skip@15
   LDA   __zpabi_check__v3_0
   CMP   #$04
   BEQ   .if_end@9
.jcmp_skip@15:
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

   LDA   #$FF
   STA   __zpabi_check__v1_0
   STA   __zpabi_check__v1_1
   STA   __zpabi_check__v1_2
   STA   __zpabi_check__v1_3
   LDA   #$E0
   STA   __zpabi_check__v2_1
   LDA   #$04
   STA   __zpabi_check__v3_0
   LDA   #$1C
   STA   __zpabi_check__v4_0
   LDA   #$2C
   STA   __zpabi_check__v4_1
   LDA   #$34
   STA   __zpabi_check__v4_2
   LDA   #$E6
   STA   __zpabi_check__v4_3
   LDA   #$0A
   STA   __zpabi_check__v5_0
   LDA   #$00
   STA   __zpabi_check__v2_0
   STA   __zpabi_check__v3_1
   STA   __zpabi_check__v5_1
   STA   __zpabi_check__v5_2
   STA   __zpabi_check__v5_3
   LDX   #$FF
   TXA
   JMP   check
