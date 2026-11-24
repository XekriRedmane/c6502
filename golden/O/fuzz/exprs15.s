SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__zpabi_check__v1	EQU	$80
__zpabi_check__v2	EQU	$81
__zpabi_check__v3_0	EQU	$82
__zpabi_check__v3_1	EQU	$83
__zpabi_check__v4_0	EQU	$84
__zpabi_check__v4_1	EQU	$85
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
__local_check__5	EQU	$92
__local_check__6	EQU	$93
__local_check__v0_1	EQU	$94
__local_check__v0_0	EQU	$95

; @zp-link-meta-begin
; def check params=__zpabi_check__v0_0,__zpabi_check__v0_1,__zpabi_check__v1,__zpabi_check__v2,__zpabi_check__v3_0,__zpabi_check__v3_1,__zpabi_check__v4_0,__zpabi_check__v4_1,__zpabi_check__v5_0,__zpabi_check__v5_1,__zpabi_check__v5_2,__zpabi_check__v5_3 locals=__local_check__fail_1,__local_check__fail_0,__local_check__0,__local_check__1,__local_check__2,__local_check__3,__local_check__4,__local_check__5,__local_check__6,__local_check__v0_1,__local_check__v0_0 indirect=false in_cycle=false param_regs=A,X,,,,,,,,,,
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
   LDA   __zpabi_check__v2
   BMI   .sx_neg@0
   LDA   #$00
   BEQ   .sx_done@1
.sx_neg@0:
   LDA   #$FF
.sx_done@1:
   STA   __local_check__6
   BPL   .lb_skip@2
   JMP   .check@asm_ssa_split@0
.lb_skip@2:
   BEQ   .lb_skip@1
   JMP   .check@asm_ssa_split@0
.lb_skip@1:
   LDA   __zpabi_check__v2
   CMP   #$07
   BEQ   .lb_skip@0
   JMP   .check@asm_ssa_split@0
.lb_skip@0:
   LDA   #$01
   STA   __local_check__fail_0
   LDA   #$00
   STA   __local_check__fail_1
.if_end@0:
   LDA   #$FF
   CLC
   SBC   __zpabi_check__v4_0
   LDA   #$00
   SBC   __zpabi_check__v4_1
   LDA   #$00
   SBC   #$00
   LDA   #$00
   ROL
   STA   __local_check__0
   SEC
   LDA   #$00
   SBC   __local_check__0
   STA   __local_check__1
   LDA   #$00
   SBC   #$00
   ORA   __local_check__1
   BEQ   .if_end@1
   LDA   __local_check__fail_0
   ORA   #$02
   STA   __local_check__fail_0
   LDA   #$00
   STA   __local_check__fail_1
.if_end@1:
   LDA   __zpabi_check__v5_3
   CMP   #$80
   BNE   .jcmp_skip@4
   LDA   __zpabi_check__v5_2
   BNE   .jcmp_skip@4
   LDA   __zpabi_check__v5_1
   BNE   .jcmp_skip@4
   LDA   __zpabi_check__v5_0
   BEQ   .if_end@2
.jcmp_skip@4:
   LDA   __local_check__fail_0
   ORA   #$04
   STA   __local_check__fail_0
.if_end@2:
   LDA   __zpabi_check__v3_1
   BMI   .sx_neg@5
   LDA   #$00
   BEQ   .sx_done@6
.sx_neg@5:
   LDA   #$FF
.sx_done@6:
   STA   __local_check__1
   STA   __local_check__0
   LDA   #$02
   CLC
   ADC   __zpabi_check__v3_0
   STA   __local_check__5
   LDA   #$00
   ADC   __zpabi_check__v3_1
   STA   __local_check__4
   LDA   #$00
   ADC   __local_check__1
   STA   __local_check__3
   LDA   #$00
   ADC   __local_check__0
   STA   __local_check__2
   CPX   #$01
   BNE   .cmp_true@7
   LDA   __local_check__v0_0
   BNE   .cmp_true@7
   LDA   #$00
   BEQ   .cmp_end@8
.cmp_true@7:
   LDA   #$01
.cmp_end@8:
   STA   __local_check__0
   LDA   __zpabi_check__v3_1
   CMP   #$FF
   BNE   .thread@0
   LDA   __zpabi_check__v3_0
   CMP   #$FF
   BEQ   .cmp_true@10
.thread@0:
   LDA   #$00
   BEQ   .cmp_end@11
.cmp_true@10:
   LDA   #$01
.cmp_end@11:
   TAX
   LDA   #$00
   STA   __local_check__1
   CPX   #$00
   BEQ   .shift_done@14
.shift_loop@13:
   ASL   __local_check__0
   ROL   __local_check__1
   DEX
   BNE   .shift_loop@13
.shift_done@14:
   LDA   __local_check__1
   BMI   .sx_neg@15
   LDA   #$00
   BEQ   .sx_done@16
.sx_neg@15:
   LDA   #$FF
.sx_done@16:
   STA   HARGS+6
   STA   HARGS+7
   LDA   __local_check__5
   STA   HARGS
   LDA   __local_check__4
   STA   HARGS+1
   LDA   __local_check__3
   STA   HARGS+2
   LDA   __local_check__2
   STA   HARGS+3
   LDA   __local_check__0
   STA   HARGS+4
   LDA   __local_check__1
   STA   HARGS+5
   JSR   mul32
   LDA   HARGS+11
   CMP   #$FF
   BNE   .jcmp_skip@17
   LDA   HARGS+10
   CMP   #$FF
   BNE   .jcmp_skip@17
   LDA   HARGS+9
   CMP   #$80
   BNE   .jcmp_skip@17
   LDA   HARGS+8
   CMP   #$02
   BEQ   .if_end@3
.jcmp_skip@17:
   LDA   __local_check__fail_0
   ORA   #$08
   STA   __local_check__fail_0
.if_end@3:
   LDA   __zpabi_check__v3_0
   BMI   .sx_neg@18
   LDA   #$00
   BEQ   .sx_done@19
.sx_neg@18:
   LDA   #$FF
.sx_done@19:
   STA   __local_check__1
   STA   __local_check__0
   SEC
   SBC   #$01
   LDA   __local_check__1
   SBC   #$00
   LDA   __local_check__0
   SBC   #$00
   BVC   .cmp_novf@20
   EOR   #$80
.cmp_novf@20:
   ASL
   LDA   #$00
   ROL
   AND   #$7F
   EOR   #$40
   SEC
   SBC   #$40
   CMP   #$01
   BEQ   .if_end@4
   LDA   __local_check__fail_0
   ORA   #$10
   STA   __local_check__fail_0
.if_end@4:
   LDA   __zpabi_check__v4_0
   CLC
   ADC   __zpabi_check__v2
   STA   __local_check__2
   LDA   __zpabi_check__v4_1
   ADC   __local_check__6
   STA   __local_check__1
   LDA   #$01
   SEC
   SBC   __local_check__v0_0
   LDA   #$00
   SBC   __local_check__v0_1
   LDA   #$00
   SBC   #$00
   LDA   #$00
   ROL
   STA   __local_check__0
   LDA   __local_check__2
   SEC
   SBC   __local_check__0
   LDA   __local_check__1
   SBC   #$00
   BCS   .if_end@5
   LDA   __local_check__fail_0
   ORA   #$20
   STA   __local_check__fail_0
.if_end@5:
   LDA   __zpabi_check__v1
   BMI   .sx_neg@21
   LDA   #$00
   BEQ   .sx_done@22
.sx_neg@21:
   LDA   #$FF
.sx_done@22:
   STA   __local_check__v0_0
   LDA   __zpabi_check__v4_1
   CMP   __local_check__v0_0
   BNE   .thread@1
   LDA   __zpabi_check__v4_0
   CMP   __zpabi_check__v1
   BEQ   .cmp_true@23
.thread@1:
   LDA   #$00
   BEQ   .cmp_end@24
.cmp_true@23:
   LDA   #$01
.cmp_end@24:
   STA   __local_check__1
   LDA   __zpabi_check__v4_0
   CLC
   SBC   __zpabi_check__v2
   LDA   __zpabi_check__v4_1
   SBC   __local_check__6
   LDA   #$00
   ROL
   SEC
   SBC   __local_check__1
   LDA   #$00
   SBC   #$00
   BVC   .cmp_novf@26
   EOR   #$80
.cmp_novf@26:
   ASL
   LDA   #$00
   ROL
   EOR   #$01
   STA   HARGS
   LDA   #$00
   STA   HARGS+1
   LDA   __zpabi_check__v4_0
   STA   HARGS+2
   LDA   __zpabi_check__v4_1
   STA   HARGS+3
   JSR   udivmod16
   LDA   HARGS+4
   ORA   HARGS+5
   BEQ   .if_end@6
   LDA   __local_check__fail_0
   ORA   #$40
   STA   __local_check__fail_0
.if_end@6:
   LDA   #$FF
   CLC
   SBC   __zpabi_check__v4_0
   LDA   #$FF
   SBC   __zpabi_check__v4_1
   LDA   #$00
   ROL
   SEC
   SBC   __zpabi_check__v1
   STA   __local_check__v0_1
   LDA   #$00
   SBC   __local_check__v0_0
   STA   __local_check__6
   LDA   __zpabi_check__v2
   BMI   .sx_neg@27
   LDA   #$00
   BEQ   .sx_done@28
.sx_neg@27:
   LDA   #$FF
.sx_done@28:
   STA   HARGS+1
   STA   HARGS+2
   STA   HARGS+3
   LDA   __zpabi_check__v2
   STA   HARGS
   LDA   __zpabi_check__v5_0
   STA   HARGS+4
   LDA   __zpabi_check__v5_1
   STA   HARGS+5
   LDA   __zpabi_check__v5_2
   STA   HARGS+6
   LDA   __zpabi_check__v5_3
   STA   HARGS+7
   JSR   mul32
   LDA   __zpabi_check__v1
   SEC
   SBC   __zpabi_check__v4_0
   STA   __local_check__1
   LDA   __local_check__v0_0
   SBC   __zpabi_check__v4_1
   STA   __local_check__0
   LDA   HARGS+8
   CLC
   SBC   __local_check__1
   LDA   HARGS+9
   SBC   __local_check__0
   LDA   HARGS+10
   SBC   #$00
   LDA   HARGS+11
   SBC   #$00
   LDA   #$00
   ROL
   STA   __local_check__0
   LDA   __local_check__v0_1
   SEC
   SBC   __local_check__0
   LDA   __local_check__6
   SBC   #$00
   BVC   .jcmp_novf@29
   EOR   #$80
.jcmp_novf@29:
   BMI   .if_end@7
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

   LDA   #$0E
   STA   __zpabi_check__v1
   LDA   #$00
   STA   __zpabi_check__v2
   STA   __zpabi_check__v3_0
   STA   __zpabi_check__v5_0
   STA   __zpabi_check__v5_1
   STA   __zpabi_check__v5_2
   LDA   #$FF
   STA   __zpabi_check__v4_0
   STA   __zpabi_check__v4_1
   LDA   #$80
   STA   __zpabi_check__v3_1
   STA   __zpabi_check__v5_3
   LDX   #$00
   TXA
   JMP   check
