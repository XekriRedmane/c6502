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
__zpabi_check__v3	EQU	$86
__zpabi_check__v4_0	EQU	$87
__zpabi_check__v4_1	EQU	$88
__zpabi_check__v4_2	EQU	$89
__zpabi_check__v4_3	EQU	$8A
__zpabi_check__v5_0	EQU	$8B
__zpabi_check__v5_1	EQU	$8C
__local_check__fail_1	EQU	$8E
__local_check__fail_0	EQU	$8F
__local_check__0	EQU	$90
__local_check__1	EQU	$91
__local_check__2	EQU	$92
__local_check__4	EQU	$94
__local_check__5	EQU	$95
__local_check__6	EQU	$96
__local_check__7	EQU	$97
__local_check__8	EQU	$98
__local_check__v0_1	EQU	$99
__local_check__v0_0	EQU	$9A

; @zp-link-meta-begin
; def check params=__zpabi_check__v0_0,__zpabi_check__v0_1,__zpabi_check__v1_0,__zpabi_check__v1_1,__zpabi_check__v1_2,__zpabi_check__v1_3,__zpabi_check__v2_0,__zpabi_check__v2_1,__zpabi_check__v3,__zpabi_check__v4_0,__zpabi_check__v4_1,__zpabi_check__v4_2,__zpabi_check__v4_3,__zpabi_check__v5_0,__zpabi_check__v5_1 locals=__local_check__fail_1,__local_check__fail_0,__local_check__0,__local_check__1,__local_check__2,__local_check__3,__local_check__4,__local_check__5,__local_check__6,__local_check__7,__local_check__8,__local_check__v0_1,__local_check__v0_0,__local_check__9,__local_check__10,__local_check__11 indirect=false in_cycle=false param_regs=A,X,,,,,,,,,,,,,
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
   SEC
   SBC   __zpabi_check__v2_0
   TXA
   SBC   __zpabi_check__v2_1
   LDA   #$00
   ROL
   STA   __local_check__fail_1
   LDA   __zpabi_check__v3
   BMI   .sx_neg@0
   LDA   #$00
   BEQ   .sx_done@1
.sx_neg@0:
   LDA   #$FF
.sx_done@1:
   STA   __local_check__8
   LDA   __local_check__fail_1
   STA   HARGS
   LDA   #$00
   STA   HARGS+1
   LDA   __zpabi_check__v3
   STA   HARGS+2
   LDA   __local_check__8
   STA   HARGS+3
   JSR   sdivmod16
   SEC
   LDA   #$00
   SBC   HARGS+4
   STA   __local_check__fail_0
   LDA   #$00
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
   LDA   __zpabi_check__v3
   STA   HARGS
   LDA   __local_check__8
   STA   HARGS+1
   LDA   __zpabi_check__v5_0
   STA   HARGS+2
   LDA   __zpabi_check__v5_1
   STA   HARGS+3
   JSR   sdivmod16
   LDA   HARGS+4
   STA   __local_check__7
   LDA   HARGS+5
   STA   __local_check__6
   BMI   .sx_neg@2
   LDA   #$00
   BEQ   .sx_done@3
.sx_neg@2:
   LDA   #$FF
.sx_done@3:
   STA   __local_check__5
   STA   __local_check__4
   LDA   #$4A
   STA   HARGS
   LDA   #$6D
   STA   HARGS+1
   LDA   #$00
   STA   HARGS+2
   STA   HARGS+3
   LDA   __zpabi_check__v4_0
   STA   HARGS+4
   LDA   __zpabi_check__v4_1
   STA   HARGS+5
   LDA   __zpabi_check__v4_2
   STA   HARGS+6
   LDA   __zpabi_check__v4_3
   STA   HARGS+7
   JSR   udivmod32
   LDA   __local_check__4
   CMP   HARGS+15
   BNE   .cmp_true@4
   LDA   __local_check__5
   CMP   HARGS+14
   BNE   .cmp_true@4
   LDA   __local_check__6
   CMP   HARGS+13
   BNE   .cmp_true@4
   LDA   __local_check__7
   CMP   HARGS+12
   BNE   .cmp_true@4
   LDA   #$00
   BEQ   .cmp_end@5
.cmp_true@4:
   LDA   #$01
.cmp_end@5:
   STA   __local_check__1
   LDA   __zpabi_check__v4_0
   SEC
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
   LDA   #$00
   BNE   .cmp_true@7
   LDA   __local_check__1
   CMP   __local_check__0
   BNE   .cmp_true@7
   LDA   #$00
   BEQ   .cmp_end@8
.cmp_true@7:
   LDA   #$01
.cmp_end@8:
   STA   __local_check__0
   LDA   #$00
   BNE   .jcmp_skip@10
   LDA   __local_check__0
   CMP   #$01
   BEQ   .if_end@1
.jcmp_skip@10:
   LDA   __local_check__fail_0
   ORA   #$02
   STA   __local_check__fail_0
.if_end@1:
   LDA   __local_check__v0_0
   AND   __local_check__v0_0
   STA   __local_check__1
   LDA   __local_check__v0_1
   AND   __local_check__v0_1
   STA   __local_check__0
   LDA   __local_check__1
   CLC
   SBC   #$FF
   LDA   __local_check__0
   SBC   #$FF
   LDA   #$00
   ROL
   STA   __local_check__0
   LDA   #$00
   STA   __local_check__1
   LDX   __zpabi_check__v1_0
   BEQ   .shift_done@12
.shift_loop@11:
   LDA   __local_check__1
   ASL
   ROR   __local_check__1
   ROR   __local_check__0
   DEX
   BNE   .shift_loop@11
.shift_done@12:
   LDA   __local_check__0
   ORA   __local_check__1
   BEQ   .if_end@2
   LDA   __local_check__fail_0
   ORA   #$04
   STA   __local_check__fail_0
.if_end@2:
   LDA   __zpabi_check__v5_1
   BMI   .sx_neg@13
   LDA   #$00
   BEQ   .sx_done@14
.sx_neg@13:
   LDA   #$FF
.sx_done@14:
   STA   __local_check__1
   STA   __local_check__0
   LDA   __zpabi_check__v5_0
   CLC
   SBC   __zpabi_check__v4_0
   LDA   __zpabi_check__v5_1
   SBC   __zpabi_check__v4_1
   LDA   __local_check__1
   SBC   __zpabi_check__v4_2
   LDA   __local_check__0
   SBC   __zpabi_check__v4_3
   LDA   #$00
   ROL
   STA   __local_check__1
   LDA   __local_check__v0_0
   CLC
   SBC   __zpabi_check__v3
   LDA   __local_check__v0_1
   SBC   __local_check__8
   LDA   #$00
   ROL
   SEC
   SBC   __local_check__1
   LDA   #$00
   SBC   #$00
   BVC   .cmp_novf@15
   EOR   #$80
.cmp_novf@15:
   ASL
   LDA   #$00
   ROL
   EOR   #$01
   STA   __local_check__2
   LDA   #$00
   BPL   .vthread@0
   LDA   #$FF
   BNE   .jcmp_skip@18
   LDA   #$00
   BNE   .jcmp_skip@18
.vthread@0:
   LDA   __local_check__2
   CMP   #$01
   BEQ   .if_end@3
.jcmp_skip@18:
   LDA   __local_check__fail_0
   ORA   #$08
   STA   __local_check__fail_0
.if_end@3:
   LDA   #$07
   EOR   __zpabi_check__v1_0
   STA   __local_check__6
   LDA   __zpabi_check__v2_0
   SEC
   SBC   #$07
   LDA   __zpabi_check__v2_1
   SBC   #$00
   LDA   #$00
   ROL
   CLC
   ADC   __local_check__6
   TAX
   LDA   __zpabi_check__v5_0
   STA   __local_check__0
   LDA   __zpabi_check__v5_1
   STA   __local_check__1
   CPX   #$00
   BEQ   .shift_done@22
.shift_loop@21:
   LSR   __local_check__1
   ROR   __local_check__0
   DEX
   BNE   .shift_loop@21
.shift_done@22:
   LDA   __local_check__1
   BNE   .jcmp_skip@23
   LDA   __local_check__0
   CMP   #$80
   BEQ   .if_end@4
.jcmp_skip@23:
   LDA   __local_check__fail_0
   ORA   #$10
   STA   __local_check__fail_0
.if_end@4:
   LDA   __zpabi_check__v1_0
   ORA   __zpabi_check__v1_1
   BEQ   .if_end@5
   LDA   __local_check__fail_0
   ORA   #$20
   STA   __local_check__fail_0
.if_end@5:
   LDA   __local_check__v0_0
   CLC
   ADC   #$07
   STA   __local_check__0
   BCC   .carry_inc@0
   INC   __local_check__v0_1
.carry_inc@0:
   LDA   __local_check__v0_1
   CMP   #$80
   BNE   .jcmp_skip@24
   LDA   __local_check__0
   CMP   #$07
   BEQ   .if_end@6
.jcmp_skip@24:
   LDA   __local_check__fail_0
   ORA   #$40
   STA   __local_check__fail_0
.if_end@6:
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
   STA   __zpabi_check__v1_1
   STA   __zpabi_check__v1_2
   STA   __zpabi_check__v1_3
   STA   __zpabi_check__v2_0
   STA   __zpabi_check__v4_1
   STA   __zpabi_check__v4_2
   STA   __zpabi_check__v4_3
   STA   __zpabi_check__v5_0
   LDA   #$05
   STA   __zpabi_check__v3
   LDA   #$01
   STA   __zpabi_check__v4_0
   LDA   #$80
   STA   __zpabi_check__v2_1
   STA   __zpabi_check__v5_1
   LDX   #$80
   LDA   #$00
   JMP   check
