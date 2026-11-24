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
__zpabi_check__v3_2	EQU	$87
__zpabi_check__v3_3	EQU	$88
__zpabi_check__v4_0	EQU	$89
__zpabi_check__v4_1	EQU	$8A
__zpabi_check__v5	EQU	$8B
__local_check__fail_1	EQU	$8D
__local_check__fail_0	EQU	$8E
__local_check__0	EQU	$8F
__local_check__1	EQU	$90
__local_check__2	EQU	$91
__local_check__3	EQU	$92
__local_check__4	EQU	$93
__local_check__5	EQU	$94
__local_check__6	EQU	$95
__local_check__7	EQU	$96
__local_check__v0_1	EQU	$97
__local_check__v0_0	EQU	$98

; @zp-link-meta-begin
; def check params=__zpabi_check__v0_0,__zpabi_check__v0_1,__zpabi_check__v1_0,__zpabi_check__v1_1,__zpabi_check__v1_2,__zpabi_check__v1_3,__zpabi_check__v2,__zpabi_check__v3_0,__zpabi_check__v3_1,__zpabi_check__v3_2,__zpabi_check__v3_3,__zpabi_check__v4_0,__zpabi_check__v4_1,__zpabi_check__v5 locals=__local_check__fail_1,__local_check__fail_0,__local_check__0,__local_check__1,__local_check__2,__local_check__3,__local_check__4,__local_check__5,__local_check__6,__local_check__7,__local_check__v0_1,__local_check__v0_0,__local_check__8,__local_check__9,__local_check__10 indirect=false in_cycle=false param_regs=A,X,,,,,,,,,,,,
; def main params= locals=__local_main__0 indirect=false in_cycle=false
; call main -> check
; item check section=.text local=false zero=false
; item main section=.text local=false zero=false
; runtime hargs_size=32 hargs_need=22
; @zp-link-meta-end

check:
   SUBROUTINE

   STA   __local_check__v0_0
   STX   __local_check__v0_1
   ORA   __local_check__v0_1
   BEQ   .lb_skip@0
   JMP   .check@asm_ssa_split@0
.lb_skip@0:
   LDA   #$01
   STA   __local_check__fail_0
   LDA   #$00
   STA   __local_check__fail_1
.if_end@0:
   LDA   __zpabi_check__v1_1
   BNE   .jcmp_skip@0
   LDA   __zpabi_check__v1_0
   CMP   #$01
   BEQ   .vthread@0
.jcmp_skip@0:
   LDA   __local_check__fail_0
   ORA   #$02
   STA   __local_check__fail_0
   LDA   #$00
   STA   __local_check__fail_1
   LDA   __zpabi_check__v1_1
   BMI   .sx_neg@1
.vthread@0:
   LDA   #$00
   BEQ   .sx_done@2
.sx_neg@1:
   LDA   #$FF
.sx_done@2:
   STA   __local_check__1
   LDA   __zpabi_check__v1_0
   STA   __local_check__7
   LDA   __zpabi_check__v1_1
   ORA   #$80
   STA   __local_check__6
   LDA   __local_check__1
   STA   __local_check__5
   STA   __local_check__4
   TXA
   BMI   .sx_neg@3
   LDA   #$00
   BEQ   .sx_done@4
.sx_neg@3:
   LDA   #$FF
.sx_done@4:
   STA   HARGS+6
   STA   HARGS+7
   LDA   #$7F
   STA   HARGS
   LDA   #$00
   STA   HARGS+1
   LDY   #$00
   STY   HARGS+2
   STA   HARGS+3
   LDA   __local_check__v0_0
   STA   HARGS+4
   STX   HARGS+5
   JSR   udivmod32
   LDA   HARGS+8
   ORA   HARGS+9
   ORA   HARGS+10
   ORA   HARGS+11
   BEQ   .and_false@2
   LDA   #$01
   STA   __local_check__3
   LDA   #$00
   BEQ   .and_end@3
.and_false@2:
   LDA   #$00
   STA   __local_check__3
.and_end@3:
   ORA   #$00
   BPL   .sx_done@6
   LDA   #$FF
.sx_done@6:
   STA   __local_check__1
   STA   __local_check__0
   LDA   __local_check__7
   SEC
   SBC   __local_check__3
   LDA   __local_check__6
   SBC   #$00
   LDA   __local_check__5
   SBC   __local_check__1
   LDA   __local_check__4
   SBC   __local_check__0
   BVC   .cmp_novf@7
   EOR   #$80
.cmp_novf@7:
   ASL
   LDA   #$00
   ROL
   EOR   #$01
   STA   __local_check__0
   LDA   #$00
   BNE   .jcmp_skip@8
   LDA   __local_check__0
   CMP   #$01
   BEQ   .if_end@4
.jcmp_skip@8:
   LDA   __local_check__fail_0
   ORA   #$04
   STA   __local_check__fail_0
.if_end@4:
   LDA   __zpabi_check__v3_3
   CMP   #$36
   BNE   .jcmp_skip@9
   LDA   __zpabi_check__v3_2
   CMP   #$F6
   BNE   .jcmp_skip@9
   LDA   __zpabi_check__v3_1
   CMP   #$75
   BNE   .jcmp_skip@9
   LDA   __zpabi_check__v3_0
   CMP   #$CC
   BEQ   .if_end@5
.jcmp_skip@9:
   LDA   __local_check__fail_0
   ORA   #$08
   STA   __local_check__fail_0
.if_end@5:
   LDA   __zpabi_check__v2
   BMI   .sx_neg@10
   LDA   #$00
   BEQ   .sx_done@11
.sx_neg@10:
   LDA   #$FF
.sx_done@11:
   STA   __local_check__1
   LDA   __local_check__v0_0
   STA   HARGS
   LDA   __local_check__v0_1
   STA   HARGS+1
   LDA   #$FF
   STA   HARGS+2
   STA   HARGS+3
   JSR   mul16
   LDA   HARGS+5
   CMP   #$FF
   BNE   .thread@0
   LDA   HARGS+4
   CMP   #$FF
   BEQ   .cmp_true@12
.thread@0:
   LDA   #$00
   BEQ   .cmp_end@13
.cmp_true@12:
   LDA   #$01
.cmp_end@13:
   TAX
   LDA   __zpabi_check__v2
   STA   __local_check__0
   CPX   #$00
   BEQ   .shift_done@16
.shift_loop@15:
   LDA   __local_check__1
   ASL
   ROR   __local_check__1
   ROR   __local_check__0
   DEX
   BNE   .shift_loop@15
.shift_done@16:
   LDA   __local_check__1
   BNE   .jcmp_skip@17
   LDA   __local_check__0
   CMP   #$5D
   BEQ   .if_end@6
.jcmp_skip@17:
   LDA   __local_check__fail_0
   ORA   #$10
   STA   __local_check__fail_0
.if_end@6:
   LDA   #$07
   SEC
   SBC   __zpabi_check__v1_0
   LDA   #$00
   SBC   __zpabi_check__v1_1
   LDA   #$00
   SBC   __zpabi_check__v1_2
   LDA   #$00
   SBC   __zpabi_check__v1_3
   LDA   #$00
   ROL
   STA   __local_check__6
   LDA   #$00
   BPL   .sx_done@19
   LDA   #$FF
.sx_done@19:
   STA   __local_check__5
   STA   __local_check__4
   LDA   __zpabi_check__v2
   BMI   .sx_neg@20
   LDA   #$00
   BEQ   .sx_done@21
.sx_neg@20:
   LDA   #$FF
.sx_done@21:
   STA   HARGS+5
   STA   HARGS+6
   STA   HARGS+7
   LDA   __zpabi_check__v3_0
   STA   HARGS
   LDA   __zpabi_check__v3_1
   STA   HARGS+1
   LDA   __zpabi_check__v3_2
   STA   HARGS+2
   LDA   __zpabi_check__v3_3
   STA   HARGS+3
   LDA   __zpabi_check__v2
   STA   HARGS+4
   JSR   sdivmod32
   LDA   __local_check__6
   SEC
   SBC   HARGS+12
   LDA   #$00
   SBC   HARGS+13
   LDA   __local_check__5
   SBC   HARGS+14
   LDA   __local_check__4
   SBC   HARGS+15
   BVC   .cmp_novf@22
   EOR   #$80
.cmp_novf@22:
   ASL
   LDA   #$00
   ROL
   EOR   #$01
   STA   __local_check__4
   LDA   #$00
   BPL   .sx_done@24
   LDA   #$FF
.sx_done@24:
   STA   HARGS+2
   STA   HARGS+3
   LDA   #$01
   EOR   __zpabi_check__v1_0
   STA   __local_check__3
   LDA   #$00
   EOR   __zpabi_check__v1_1
   STA   __local_check__2
   LDA   #$00
   EOR   __zpabi_check__v1_2
   STA   __local_check__1
   LDA   #$00
   EOR   __zpabi_check__v1_3
   STA   __local_check__0
   LDA   __local_check__3
   CLC
   ADC   #$7F
   STA   HARGS+4
   LDA   __local_check__2
   ADC   #$00
   STA   HARGS+5
   LDA   __local_check__1
   ADC   #$00
   STA   HARGS+6
   LDA   __local_check__0
   ADC   #$00
   STA   HARGS+7
   LDA   __local_check__4
   STA   HARGS
   LDA   #$00
   STA   HARGS+1
   JSR   udivmod32
   LDA   HARGS+12
   ORA   HARGS+13
   ORA   HARGS+14
   ORA   HARGS+15
   BEQ   .if_end@7
   LDA   __local_check__fail_0
   ORA   #$20
   STA   __local_check__fail_0
.if_end@7:
   LDA   __zpabi_check__v5
   BNE   .or_true@8
   LDA   #$00
   STA   __local_check__7
   STA   __local_check__6
   JMP   .or_end@9
.or_true@8:
   LDA   #$01
   STA   __local_check__7
   LDA   #$00
   STA   __local_check__6
.or_end@9:
   ORA   #$00
   BPL   .sx_done@26
   LDA   #$FF
.sx_done@26:
   STA   __local_check__5
   STA   __local_check__4
   LDA   __zpabi_check__v5
   BMI   .sx_neg@27
   LDA   #$00
   BEQ   .sx_done@28
.sx_neg@27:
   LDA   #$FF
.sx_done@28:
   STA   HARGS+1
   STA   HARGS+2
   STA   HARGS+3
   LDA   __zpabi_check__v5
   STA   HARGS
   LDA   __zpabi_check__v1_0
   STA   HARGS+4
   LDA   __zpabi_check__v1_1
   STA   HARGS+5
   LDA   __zpabi_check__v1_2
   STA   HARGS+6
   LDA   __zpabi_check__v1_3
   STA   HARGS+7
   JSR   mul32
   LDA   __local_check__7
   ORA   HARGS+8
   STA   __local_check__3
   LDA   __local_check__6
   ORA   HARGS+9
   STA   __local_check__6
   LDA   __local_check__5
   ORA   HARGS+10
   STA   __local_check__5
   LDA   __local_check__4
   ORA   HARGS+11
   STA   __local_check__4
   LDA   __zpabi_check__v4_0
   BMI   .sx_neg@29
   LDA   #$00
   BEQ   .sx_done@30
.sx_neg@29:
   LDA   #$FF
.sx_done@30:
   STA   __local_check__2
   STA   __local_check__1
   STA   __local_check__0
   LDA   __local_check__3
   CLC
   ADC   __zpabi_check__v4_0
   STA   __local_check__3
   LDA   __local_check__6
   ADC   __local_check__2
   STA   __local_check__2
   LDA   __local_check__5
   ADC   __local_check__1
   STA   __local_check__1
   LDA   __local_check__4
   ADC   __local_check__0
   CMP   #$FF
   BNE   .jcmp_skip@31
   LDA   __local_check__1
   CMP   #$FF
   BNE   .jcmp_skip@31
   LDA   __local_check__2
   CMP   #$FF
   BNE   .jcmp_skip@31
   LDA   __local_check__3
   CMP   #$8E
   BEQ   .if_end@10
.jcmp_skip@31:
   LDA   __local_check__fail_0
   ORA   #$40
   STA   __local_check__fail_0
.if_end@10:
   LDA   __zpabi_check__v3_3
   CMP   #$36
   BNE   .jcmp_skip@32
   LDA   __zpabi_check__v3_2
   CMP   #$F6
   BNE   .jcmp_skip@32
   LDA   __zpabi_check__v3_1
   CMP   #$75
   BNE   .jcmp_skip@32
   LDA   __zpabi_check__v3_0
   CMP   #$CC
   BEQ   .if_end@11
.jcmp_skip@32:
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

   LDA   #$01
   STA   __zpabi_check__v1_0
   STA   __zpabi_check__v4_0
   LDA   #$00
   STA   __zpabi_check__v1_1
   STA   __zpabi_check__v1_2
   STA   __zpabi_check__v1_3
   STA   __zpabi_check__v4_1
   LDA   #$5D
   STA   __zpabi_check__v2
   LDA   #$CC
   STA   __zpabi_check__v3_0
   LDA   #$75
   STA   __zpabi_check__v3_1
   LDA   #$F6
   STA   __zpabi_check__v3_2
   LDA   #$36
   STA   __zpabi_check__v3_3
   LDA   #$8D
   STA   __zpabi_check__v5
   LDX   #$FF
   TXA
   JMP   check
