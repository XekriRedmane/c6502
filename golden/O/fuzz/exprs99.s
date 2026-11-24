SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__zpabi_check__v1	EQU	$80
__zpabi_check__v2	EQU	$81
__zpabi_check__v3_0	EQU	$82
__zpabi_check__v3_1	EQU	$83
__zpabi_check__v3_2	EQU	$84
__zpabi_check__v3_3	EQU	$85
__zpabi_check__v4	EQU	$86
__zpabi_check__v5	EQU	$87
__local_check__fail_1	EQU	$89
__local_check__fail_0	EQU	$8A
__local_check__0	EQU	$8B
__local_check__1	EQU	$8C
__local_check__3	EQU	$8E
__local_check__5	EQU	$90
__local_check__6	EQU	$91
__local_check__v0_1	EQU	$92
__local_check__v0_0	EQU	$93

; @zp-link-meta-begin
; def check params=__zpabi_check__v0_0,__zpabi_check__v0_1,__zpabi_check__v1,__zpabi_check__v2,__zpabi_check__v3_0,__zpabi_check__v3_1,__zpabi_check__v3_2,__zpabi_check__v3_3,__zpabi_check__v4,__zpabi_check__v5 locals=__local_check__fail_1,__local_check__fail_0,__local_check__0,__local_check__1,__local_check__2,__local_check__3,__local_check__4,__local_check__5,__local_check__6,__local_check__v0_1,__local_check__v0_0 indirect=false in_cycle=false param_regs=A,X,,,,,,,, changes=__zpabi_check__v3_1,__zpabi_check__v4
; def main params= locals=__local_main__0 indirect=false in_cycle=false
; call main -> check
; item check section=.text local=false zero=false
; item main section=.text local=false zero=false
; runtime hargs_size=32 hargs_need=11
; @zp-link-meta-end

check:
   SUBROUTINE

   STA   __local_check__v0_0
   STX   __local_check__v0_1
   LDA   __zpabi_check__v1
   BNE   .or_true@0
   TXA
   ORA   __local_check__v0_0
   BNE   .or_true@0
   LDA   #$00
   STA   __local_check__fail_0
   JMP   .or_end@1
.or_true@0:
   LDA   #$01
   STA   __local_check__fail_0
.or_end@1:
   LDA   __zpabi_check__v4
   BMI   .sx_neg@0
   LDA   #$00
   BEQ   .sx_done@1
.sx_neg@0:
   LDA   #$FF
.sx_done@1:
   STA   __local_check__3
   LDA   __local_check__fail_0
   STA   HARGS
   LDA   #$00
   STA   HARGS+1
   LDA   __zpabi_check__v4
   STA   HARGS+2
   LDA   __local_check__3
   STA   HARGS+3
   JSR   mul16
   LDA   HARGS+5
   CMP   #$FF
   BNE   .jcmp_skip@2
   LDA   HARGS+4
   CMP   #$FF
   BNE   .lb_skip@0
   JMP   .check@asm_ssa_split@0
.lb_skip@0:
.jcmp_skip@2:
   LDA   #$01
   STA   __local_check__fail_0
   LDA   #$00
   STA   __local_check__fail_1
.if_end@4:
   LDA   __zpabi_check__v1
   BEQ   .and_false@5
   LDA   #$01
   STA   __local_check__5
   LDA   #$00
   BEQ   .and_end@6
.and_false@5:
   LDA   #$00
   STA   __local_check__5
.and_end@6:
   ORA   #$00
   BPL   .sx_done@4
   LDA   #$FF
.sx_done@4:
   STA   __local_check__1
   STA   __local_check__0
   LDA   __local_check__5
   SEC
   SBC   __zpabi_check__v3_0
   LDA   #$00
   SBC   __zpabi_check__v3_1
   LDA   __local_check__1
   SBC   __zpabi_check__v3_2
   LDA   __local_check__0
   SBC   __zpabi_check__v3_3
   LDA   #$00
   ROL
   STA   HARGS
   LDA   #$00
   STA   HARGS+1
   LDA   __local_check__v0_0
   STA   HARGS+2
   LDA   __local_check__v0_1
   STA   HARGS+3
   JSR   udivmod16
   LDA   HARGS+6
   ORA   HARGS+7
   BEQ   .if_end@7
   LDA   __local_check__fail_0
   ORA   #$02
   STA   __local_check__fail_0
.if_end@7:
   LDA   #$00
   SEC
   SBC   __zpabi_check__v3_0
   LDA   #$00
   SBC   __zpabi_check__v3_1
   LDA   #$00
   SBC   __zpabi_check__v3_2
   LDA   #$00
   SBC   __zpabi_check__v3_3
   BVC   .cmp_novf@5
   EOR   #$80
.cmp_novf@5:
   ASL
   LDA   #$00
   ROL
   STA   __local_check__0
   LDA   __zpabi_check__v2
   BMI   .sx_neg@6
   LDA   #$00
   BEQ   .sx_done@7
.sx_neg@6:
   LDA   #$FF
.sx_done@7:
   STA   __local_check__6
   LDA   __local_check__0
   CLC
   ADC   __zpabi_check__v2
   STA   __local_check__1
   LDA   #$00
   ADC   __local_check__6
   STA   __local_check__0
   LDA   __zpabi_check__v1
   SEC
   SBC   __local_check__1
   STA   __local_check__1
   LDA   #$00
   SBC   __local_check__0
   BNE   .jcmp_skip@8
   LDA   __local_check__1
   CMP   #$03
   BEQ   .if_end@8
.jcmp_skip@8:
   LDA   __local_check__fail_0
   ORA   #$04
   STA   __local_check__fail_0
.if_end@8:
   LDA   __zpabi_check__v5
   CMP   #$01
   BEQ   .vthread@0
   LDA   __local_check__fail_0
   ORA   #$08
   STA   __local_check__fail_0
   LDA   __zpabi_check__v5
   CMP   #$FF
   BEQ   .cmp_true@9
.vthread@0:
   LDA   #$00
   BEQ   .cmp_end@10
.cmp_true@9:
   LDA   #$01
.cmp_end@10:
   STA   __local_check__0
   LDA   __zpabi_check__v1
   SEC
   SBC   __local_check__0
   LDA   #$00
   SBC   #$00
   BVC   .cmp_novf@11
   EOR   #$80
.cmp_novf@11:
   ASL
   LDA   #$00
   ROL
   ORA   __zpabi_check__v2
   STA   __local_check__1
   LDA   __local_check__6
   CMP   #$FF
   BNE   .jcmp_skip@12
   LDA   __local_check__1
   CMP   #$FF
   BEQ   .if_end@10
.jcmp_skip@12:
   LDA   __local_check__fail_0
   ORA   #$10
   STA   __local_check__fail_0
.if_end@10:
   CLC
   LDA   __zpabi_check__v3_2
   STA   __local_check__1
   LDA   __zpabi_check__v3_3
   ADC   #$00
   STA   __local_check__0
   LDA   __zpabi_check__v3_0
   ORA   __zpabi_check__v3_1
   ORA   __local_check__1
   ORA   __local_check__0
   BEQ   .and_false@11
   LDA   __local_check__v0_0
   ORA   __local_check__v0_1
   BEQ   .and_false@11
   LDA   #$01
   STA   __local_check__5
   JMP   .and_end@12
.and_false@11:
   LDA   #$00
   STA   __local_check__5
.and_end@12:
   LDX   __zpabi_check__v5
   BEQ   .shift_done@14
.shift_loop@13:
   LDA   __local_check__3
   ASL
   ROR   __local_check__3
   ROR   __zpabi_check__v4
   DEX
   BNE   .shift_loop@13
.shift_done@14:
   LDA   #$F8
   ORA   __zpabi_check__v4
   STA   __local_check__1
   LDA   #$FF
   ORA   __local_check__3
   STA   __local_check__0
   LDA   __local_check__5
   CLC
   ADC   __local_check__1
   STA   __local_check__1
   LDA   #$00
   ADC   __local_check__0
   ORA   __local_check__1
   BEQ   .if_end@13
   LDA   __local_check__fail_0
   ORA   #$20
   STA   __local_check__fail_0
.if_end@13:
   LDA   __local_check__v0_0
   SEC
   SBC   __zpabi_check__v2
   LDA   __local_check__v0_1
   SBC   __local_check__6
   LDA   #$00
   ROL
   STA   __zpabi_check__v4
   SEC
   LDA   #$00
   SBC   __zpabi_check__v1
   STA   __local_check__1
   LDA   #$00
   SBC   #$00
   STA   __local_check__0
   LDA   #$00
   CMP   __local_check__0
   BNE   .cmp_end@16
   LDA   __zpabi_check__v4
   CMP   __local_check__1
   BEQ   .cmp_true@15
   LDA   #$00
   BEQ   .cmp_end@16
.cmp_true@15:
   LDA   #$01
.cmp_end@16:
   SEC
   SBC   __local_check__v0_0
   LDA   #$00
   SBC   __local_check__v0_1
   BCC   .if_end@14
   LDA   __local_check__fail_0
   ORA   #$40
   STA   __local_check__fail_0
.if_end@14:
   LDA   __local_check__v0_0
   ORA   __local_check__v0_1
   BEQ   .and_false@15
   LDA   __zpabi_check__v3_0
   ORA   __zpabi_check__v3_1
   ORA   __zpabi_check__v3_2
   ORA   __zpabi_check__v3_3
   BEQ   .and_false@15
   LDA   #$01
   STA   __local_check__1
   JMP   .and_end@16
.and_false@15:
   LDA   #$00
   STA   __local_check__1
.and_end@16:
   LDA   __zpabi_check__v2
   STA   HARGS
   LDA   __local_check__6
   STA   HARGS+1
   LDA   __local_check__1
   STA   HARGS+2
   LDA   #$00
   STA   HARGS+3
   JSR   sdivmod16
   LDA   HARGS+5
   CMP   #$FF
   BNE   .jcmp_skip@18
   LDA   HARGS+4
   CMP   #$FF
   BEQ   .if_end@17
.jcmp_skip@18:
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
   JMP   .if_end@4

main:
   SUBROUTINE

   LDA   #$03
   STA   __zpabi_check__v1
   LDA   #$FF
   STA   __zpabi_check__v2
   STA   __zpabi_check__v4
   LDA   #$00
   STA   __zpabi_check__v3_1
   STA   __zpabi_check__v3_2
   STA   __zpabi_check__v3_3
   LDA   #$01
   STA   __zpabi_check__v3_0
   STA   __zpabi_check__v5
   LDX   #$00
   JMP   check
