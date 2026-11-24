SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__zpabi_check__v1_0	EQU	$80
__zpabi_check__v1_1	EQU	$81
__zpabi_check__v2_0	EQU	$82
__zpabi_check__v2_1	EQU	$83
__zpabi_check__v3_0	EQU	$84
__zpabi_check__v3_1	EQU	$85
__zpabi_check__v3_2	EQU	$86
__zpabi_check__v3_3	EQU	$87
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
__local_check__v0	EQU	$97

; @zp-link-meta-begin
; def check params=__zpabi_check__v0,__zpabi_check__v1_0,__zpabi_check__v1_1,__zpabi_check__v2_0,__zpabi_check__v2_1,__zpabi_check__v3_0,__zpabi_check__v3_1,__zpabi_check__v3_2,__zpabi_check__v3_3,__zpabi_check__v4_0,__zpabi_check__v4_1,__zpabi_check__v4_2,__zpabi_check__v4_3,__zpabi_check__v5_0,__zpabi_check__v5_1,__zpabi_check__v5_2,__zpabi_check__v5_3 locals=__local_check__fail_1,__local_check__fail_0,__local_check__0,__local_check__1,__local_check__2,__local_check__3,__local_check__v0,__local_check__4,__local_check__5,__local_check__6,__local_check__7 indirect=false in_cycle=false param_regs=A,,,,,,,,,,,,,,,,
; def main params= locals=__local_main__0 indirect=false in_cycle=false
; call main -> check
; item check section=.text local=false zero=false
; item main section=.text local=false zero=false
; runtime hargs_size=32 hargs_need=9
; @zp-link-meta-end

check:
   SUBROUTINE

   STA   __local_check__v0
   LDA   __zpabi_check__v5_0
   CLC
   ADC   __zpabi_check__v2_0
   STA   __local_check__1
   LDA   __zpabi_check__v5_1
   ADC   __zpabi_check__v2_1
   STA   __local_check__0
   LDA   __zpabi_check__v5_2
   ADC   #$00
   STA   __local_check__fail_0
   LDA   __zpabi_check__v5_3
   ADC   #$00
   STA   __local_check__fail_1
   LDA   #$7F
   EOR   __local_check__1
   STA   __local_check__1
   LDA   #$00
   EOR   __local_check__0
   STA   __local_check__0
   LDA   #$00
   EOR   __local_check__fail_0
   STA   __local_check__3
   LDA   #$00
   EOR   __local_check__fail_1
   STA   __local_check__2
   LDA   #$00
   LDY   #$00
   EOR   #$FF
   STA   __local_check__fail_0
   TYA
   EOR   #$FF
   STA   __local_check__fail_1
   LDA   __local_check__1
   EOR   __local_check__fail_0
   STA   __local_check__1
   LDA   __local_check__0
   EOR   __local_check__fail_1
   STA   __local_check__0
   LDA   __local_check__3
   STA   __local_check__fail_0
   LDA   __local_check__2
   CMP   #$73
   BNE   .jcmp_skip@0
   LDA   __local_check__fail_0
   CMP   #$EE
   BNE   .jcmp_skip@0
   LDA   __local_check__0
   CMP   #$DB
   BNE   .jcmp_skip@0
   LDA   __local_check__1
   CMP   #$D8
   BNE   .lb_skip@0
   JMP   .check@asm_ssa_split@0
.lb_skip@0:
.jcmp_skip@0:
   LDA   #$01
   STA   __local_check__fail_0
   LDA   #$00
   STA   __local_check__fail_1
.if_end@0:
   LDA   __zpabi_check__v5_3
   CMP   #$73
   BNE   .jcmp_skip@1
   LDA   __zpabi_check__v5_2
   CMP   #$ED
   BNE   .jcmp_skip@1
   LDA   __zpabi_check__v5_1
   CMP   #$A4
   BNE   .jcmp_skip@1
   LDA   __zpabi_check__v5_0
   CMP   #$58
   BEQ   .if_end@1
.jcmp_skip@1:
   LDA   __local_check__fail_0
   ORA   #$02
   STA   __local_check__fail_0
   STY   __local_check__fail_1
.if_end@1:
   LDA   __local_check__v0
   BNE   .and_false@2
   TYA
   EOR   __zpabi_check__v3_0
   STA   __local_check__3
   TYA
   EOR   __zpabi_check__v3_1
   STA   __local_check__2
   TYA
   EOR   __zpabi_check__v3_2
   STA   __local_check__1
   TYA
   EOR   __zpabi_check__v3_3
   STA   __local_check__0
   LDA   __local_check__3
   ORA   __local_check__2
   ORA   __local_check__1
   ORA   __local_check__0
   BEQ   .and_false@2
   LDA   #$01
   STA   __local_check__3
   LDA   #$00
   STA   __local_check__2
   JMP   .and_end@3
.and_false@2:
   LDA   #$00
   STA   __local_check__3
   STA   __local_check__2
.and_end@3:
   LDA   __zpabi_check__v2_0
   STA   HARGS
   LDA   __zpabi_check__v2_1
   STA   HARGS+1
   LDA   __local_check__v0
   STA   HARGS+2
   STY   HARGS+3
   JSR   udivmod16
   LDA   HARGS+4
   ORA   HARGS+5
   BEQ   .lnot_true@2
   LDA   #$00
   BEQ   .lnot_end@3
.lnot_true@2:
   LDA   #$01
.lnot_end@3:
   STA   __local_check__0
   LDA   __local_check__3
   SEC
   SBC   __local_check__0
   LDA   __local_check__2
   SBC   #$00
   BVC   .jcmp_novf@4
   EOR   #$80
.jcmp_novf@4:
   BPL   .if_end@4
   LDA   __local_check__fail_0
   ORA   #$04
   STA   __local_check__fail_0
.if_end@4:
   LDA   __zpabi_check__v2_0
   EOR   #$FF
   STA   __local_check__1
   LDA   __zpabi_check__v2_1
   EOR   #$FF
   ORA   __local_check__1
   BEQ   .check@ssa_block@9
   SEC
   LDA   #$00
   SBC   __zpabi_check__v4_0
   STA   __local_check__3
   LDA   #$00
   SBC   __zpabi_check__v4_1
   STA   __local_check__2
   LDA   #$00
   SBC   __zpabi_check__v4_2
   STA   __local_check__1
   LDA   #$00
   SBC   __zpabi_check__v4_3
   STA   __local_check__0
   LDA   __local_check__3
   ORA   __local_check__2
   ORA   __local_check__1
   ORA   __local_check__0
   BNE   .if_end@8
.check@ssa_block@9:
   LDA   __local_check__fail_0
   ORA   #$10
   STA   __local_check__fail_0
.if_end@8:
   LDA   __zpabi_check__v1_1
   CMP   #$FF
   BNE   .jcmp_skip@5
   LDA   __zpabi_check__v1_0
   CMP   #$FF
   BEQ   .if_end@9
.jcmp_skip@5:
   LDA   __local_check__fail_0
   ORA   #$20
   STA   __local_check__fail_0
.if_end@9:
   LDA   __zpabi_check__v4_3
   CMP   #$FF
   BNE   .jcmp_skip@6
   LDA   __zpabi_check__v4_2
   CMP   #$FF
   BNE   .jcmp_skip@6
   LDA   __zpabi_check__v4_1
   CMP   #$FF
   BNE   .jcmp_skip@6
   LDA   __zpabi_check__v4_0
   CMP   #$FF
   BEQ   .if_end@10
.jcmp_skip@6:
   LDA   __local_check__fail_0
   ORA   #$40
   STA   __local_check__fail_0
.if_end@10:
   LDA   __zpabi_check__v3_3
   BNE   .jcmp_skip@7
   LDA   __zpabi_check__v3_2
   BNE   .jcmp_skip@7
   LDA   __zpabi_check__v3_1
   BNE   .jcmp_skip@7
   LDA   __zpabi_check__v3_0
   CMP   #$06
   BEQ   .if_end@11
.jcmp_skip@7:
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
   STA   __zpabi_check__v4_0
   STA   __zpabi_check__v4_1
   STA   __zpabi_check__v4_2
   STA   __zpabi_check__v4_3
   LDA   #$00
   STA   __zpabi_check__v2_0
   STA   __zpabi_check__v3_1
   STA   __zpabi_check__v3_2
   STA   __zpabi_check__v3_3
   LDA   #$80
   STA   __zpabi_check__v2_1
   LDA   #$06
   STA   __zpabi_check__v3_0
   LDA   #$58
   STA   __zpabi_check__v5_0
   LDA   #$A4
   STA   __zpabi_check__v5_1
   LDA   #$ED
   STA   __zpabi_check__v5_2
   LDA   #$73
   STA   __zpabi_check__v5_3
   LDA   #$80
   JMP   check
