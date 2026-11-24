SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__zpabi_check__v2_0	EQU	$80
__zpabi_check__v2_1	EQU	$81
__zpabi_check__v2_2	EQU	$82
__zpabi_check__v2_3	EQU	$83
__zpabi_check__v3_0	EQU	$84
__zpabi_check__v3_1	EQU	$85
__zpabi_check__v4_0	EQU	$86
__zpabi_check__v4_1	EQU	$87
__zpabi_check__v4_2	EQU	$88
__zpabi_check__v4_3	EQU	$89
__zpabi_check__v5	EQU	$8A
__local_check__fail_1	EQU	$8C
__local_check__fail_0	EQU	$8D
__local_check__0	EQU	$8E
__local_check__1	EQU	$8F
__local_check__2	EQU	$90
__local_check__3	EQU	$91
__local_check__7	EQU	$95
__local_check__v1	EQU	$96
__local_check__v0	EQU	$97

; @zp-link-meta-begin
; def check params=__zpabi_check__v0,__zpabi_check__v1,__zpabi_check__v2_0,__zpabi_check__v2_1,__zpabi_check__v2_2,__zpabi_check__v2_3,__zpabi_check__v3_0,__zpabi_check__v3_1,__zpabi_check__v4_0,__zpabi_check__v4_1,__zpabi_check__v4_2,__zpabi_check__v4_3,__zpabi_check__v5 locals=__local_check__fail_1,__local_check__fail_0,__local_check__0,__local_check__1,__local_check__2,__local_check__3,__local_check__4,__local_check__5,__local_check__6,__local_check__7,__local_check__v1,__local_check__v0 indirect=false in_cycle=false param_regs=A,X,,,,,,,,,,, changes=__zpabi_check__v2_1,__zpabi_check__v2_2,__zpabi_check__v2_3
; def main params= locals=__local_main__0 indirect=false in_cycle=false
; call main -> check
; item check section=.text local=false zero=false
; item main section=.text local=false zero=false
; runtime hargs_size=32 hargs_need=20
; @zp-link-meta-end

check:
   SUBROUTINE

   STA   __local_check__v0
   STX   __local_check__v1
   TXA
   SEC
   SBC   __zpabi_check__v5
   LDA   #$00
   ROL
   TAX
   LDA   #$45
   STA   __local_check__1
   LDA   #$23
   STA   __local_check__2
   CPX   #$00
   BEQ   .shift_done@1
.shift_loop@0:
   ASL   __local_check__1
   ROL   __local_check__2
   DEX
   BNE   .shift_loop@0
.shift_done@1:
   LDA   __zpabi_check__v4_0
   CLC
   SBC   __zpabi_check__v4_0
   LDA   __zpabi_check__v4_1
   SBC   __zpabi_check__v4_1
   LDA   __zpabi_check__v4_2
   SBC   __zpabi_check__v4_2
   LDA   __zpabi_check__v4_3
   SBC   __zpabi_check__v4_3
   LDA   #$00
   ROL
   STA   __local_check__0
   LDA   __zpabi_check__v5
   EOR   #$FF
   STA   __local_check__fail_0
   LDA   #$00
   EOR   #$FF
   STA   __local_check__fail_1
   LDA   __local_check__0
   SEC
   SBC   __local_check__fail_0
   LDA   #$00
   SBC   __local_check__fail_1
   BVC   .cmp_novf@2
   EOR   #$80
.cmp_novf@2:
   ASL
   LDA   #$00
   ROL
   CLC
   ADC   __local_check__1
   STA   __local_check__fail_1
   BCC   .carry_inc@0
   INC   __local_check__2
.carry_inc@0:
   LDA   __local_check__2
   CMP   #$46
   BNE   .jcmp_skip@3
   LDA   __local_check__fail_1
   CMP   #$8A
   BNE   .lb_skip@0
   JMP   .check@asm_ssa_split@0
.lb_skip@0:
.jcmp_skip@3:
   LDA   #$01
   STA   __local_check__fail_0
   LDA   #$00
   STA   __local_check__fail_1
.if_end@0:
   LDA   __zpabi_check__v4_0
   ORA   __zpabi_check__v5
   STA   HARGS+4
   LDA   __zpabi_check__v4_1
   STA   HARGS+5
   LDA   __zpabi_check__v4_2
   STA   HARGS+6
   LDA   __zpabi_check__v4_3
   STA   HARGS+7
   LDA   __zpabi_check__v4_0
   STA   HARGS
   LDA   #$00
   STA   HARGS+1
   LDY   #$00
   STY   HARGS+2
   STA   HARGS+3
   JSR   udivmod32
   LDA   __zpabi_check__v5
   AND   __local_check__v1
   CLC
   ADC   __zpabi_check__v3_0
   STA   __local_check__1
   LDA   #$00
   ADC   __zpabi_check__v3_1
   STA   __local_check__0
   BMI   .sx_neg@4
   LDA   #$00
   BEQ   .sx_done@5
.sx_neg@4:
   LDA   #$FF
.sx_done@5:
   STA   HARGS+6
   STA   HARGS+7
   LDA   HARGS+8
   STA   HARGS
   LDA   HARGS+9
   STA   HARGS+1
   LDA   HARGS+10
   STA   HARGS+2
   LDA   HARGS+11
   STA   HARGS+3
   LDA   __local_check__1
   STA   HARGS+4
   LDA   __local_check__0
   STA   HARGS+5
   JSR   udivmod32
   LDA   HARGS+8
   ORA   HARGS+9
   ORA   HARGS+10
   ORA   HARGS+11
   BEQ   .if_end@4
   LDA   __local_check__fail_0
   ORA   #$04
   STA   __local_check__fail_0
.if_end@4:
   LDA   __zpabi_check__v3_0
   ORA   __zpabi_check__v3_1
   BEQ   .vthread@0
   LDA   __zpabi_check__v2_0
   EOR   #$02
   LDX   __zpabi_check__v2_1
   STX   __local_check__2
   LDX   __zpabi_check__v2_2
   STX   __local_check__1
   LDX   __zpabi_check__v2_3
   STX   __local_check__0
   ORA   __local_check__2
   ORA   __local_check__1
   ORA   __local_check__0
   BNE   .if_end@9
.vthread@0:
   LDA   __local_check__fail_0
   ORA   #$08
   STA   __local_check__fail_0
.if_end@9:
   LDA   __zpabi_check__v3_0
   LSR
   LDA   #$00
   ROR
   STA   __local_check__2
   LDA   #$00
   SEC
   SBC   __zpabi_check__v3_0
   STA   __local_check__0
   LDA   __local_check__2
   SBC   __zpabi_check__v3_1
   STA   __local_check__1
   LDA   __zpabi_check__v3_0
   ORA   __zpabi_check__v3_1
   BNE   .or_true@10
   LDX   #$00
   BEQ   .or_end@11
.or_true@10:
   LDX   #$01
.or_end@11:
   BEQ   .shift_done@7
.shift_loop@6:
   LSR   __local_check__1
   ROR   __local_check__0
   DEX
   BNE   .shift_loop@6
.shift_done@7:
   SEC
   LDA   #$00
   SBC   __local_check__v1
   TAX
   BEQ   .shift_done@9
.shift_loop@8:
   LSR   __local_check__1
   ROR   __local_check__0
   DEX
   BNE   .shift_loop@8
.shift_done@9:
   LDA   __local_check__1
   CMP   #$7C
   BNE   .jcmp_skip@10
   LDA   __local_check__0
   CMP   #$32
   BEQ   .if_end@12
.jcmp_skip@10:
   LDA   __local_check__fail_0
   ORA   #$10
   STA   __local_check__fail_0
.if_end@12:
   LDA   __zpabi_check__v2_0
   ORA   __local_check__v0
   STA   __local_check__7
   LDA   __zpabi_check__v2_0
   SEC
   SBC   __local_check__v0
   LDA   __zpabi_check__v2_1
   SBC   #$00
   LDA   __zpabi_check__v2_2
   SBC   #$00
   LDA   __zpabi_check__v2_3
   SBC   #$00
   LDA   #$00
   ROL
   STA   __local_check__0
   LDA   __zpabi_check__v5
   EOR   __local_check__0
   STA   __local_check__3
   LDA   #$00
   BPL   .sx_done@12
   LDA   #$FF
.sx_done@12:
   STA   __local_check__1
   STA   __local_check__0
   LDA   __local_check__3
   SEC
   SBC   __local_check__7
   LDA   #$00
   SBC   __zpabi_check__v2_1
   LDA   __local_check__1
   SBC   __zpabi_check__v2_2
   LDA   __local_check__0
   SBC   __zpabi_check__v2_3
   BCC   .if_end@13
   LDA   __local_check__fail_0
   ORA   #$20
   STA   __local_check__fail_0
.if_end@13:
   LDA   __local_check__v1
   BEQ   .if_end@14
   LDA   __local_check__fail_0
   ORA   #$40
   STA   __local_check__fail_0
.if_end@14:
   LDA   __local_check__v0
   CMP   __zpabi_check__v5
   BNE   .cmp_true@13
   LDA   #$00
   BEQ   .cmp_end@14
.cmp_true@13:
   LDA   #$01
.cmp_end@14:
   STA   __local_check__1
   LDA   #$07
   CLC
   SBC   __zpabi_check__v4_0
   LDA   #$00
   SBC   __zpabi_check__v4_1
   LDA   #$00
   SBC   __zpabi_check__v4_2
   LDA   #$00
   SBC   __zpabi_check__v4_3
   LDA   #$00
   ROL
   AND   __local_check__1
   STA   __local_check__2
   LDA   #$00
   STA   __local_check__1
   LDA   __local_check__v1
   CLC
   SBC   #$01
   LDA   #$00
   ROL
   CLC
   ADC   __local_check__2
   BCC   .carry_inc@1
   INC   __local_check__1
.carry_inc@1:
   ORA   __local_check__1
   BEQ   .if_end@17
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

   LDA   #$3F
   STA   __zpabi_check__v2_0
   LDA   #$7F
   STA   __zpabi_check__v2_1
   LDA   #$95
   STA   __zpabi_check__v2_2
   LDA   #$7E
   STA   __zpabi_check__v2_3
   LDA   #$9C
   STA   __zpabi_check__v3_0
   LDA   #$07
   STA   __zpabi_check__v3_1
   LDA   #$80
   STA   __zpabi_check__v4_3
   LDA   #$00
   STA   __zpabi_check__v4_0
   STA   __zpabi_check__v4_1
   STA   __zpabi_check__v4_2
   STA   __zpabi_check__v5
   LDX   #$00
   LDA   #$01
   JMP   check
