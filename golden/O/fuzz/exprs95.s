SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__zpabi_check__v0_0	EQU	$80
__zpabi_check__v0_1	EQU	$81
__zpabi_check__v0_2	EQU	$82
__zpabi_check__v0_3	EQU	$83
__zpabi_check__v0_4	EQU	$84
__zpabi_check__v0_5	EQU	$85
__zpabi_check__v0_6	EQU	$86
__zpabi_check__v1_0	EQU	$87
__zpabi_check__v1_1	EQU	$88
__zpabi_check__v1_2	EQU	$89
__zpabi_check__v1_3	EQU	$8A
__zpabi_check__v1_4	EQU	$8B
__zpabi_check__v1_5	EQU	$8C
__zpabi_check__v1_6	EQU	$8D
__zpabi_check__v2_0	EQU	$8E
__zpabi_check__v2_1	EQU	$8F
__zpabi_check__v3	EQU	$90
__zpabi_check__v4	EQU	$91
__zpabi_check__v5_0	EQU	$92
__zpabi_check__v5_1	EQU	$93
__local_check__fail_1	EQU	$95
__local_check__fail_0	EQU	$96
__local_check__0	EQU	$97
__local_check__1	EQU	$98
__local_check__2	EQU	$99
__local_check__3	EQU	$9A
__local_check__4	EQU	$9B
__local_check__5	EQU	$9C
__local_check__6	EQU	$9D

; @zp-link-meta-begin
; def check params=__zpabi_check__v0_0,__zpabi_check__v0_1,__zpabi_check__v0_2,__zpabi_check__v0_3,__zpabi_check__v0_4,__zpabi_check__v0_5,__zpabi_check__v0_6,__zpabi_check__v1_0,__zpabi_check__v1_1,__zpabi_check__v1_2,__zpabi_check__v1_3,__zpabi_check__v1_4,__zpabi_check__v1_5,__zpabi_check__v1_6,__zpabi_check__v2_0,__zpabi_check__v2_1,__zpabi_check__v3,__zpabi_check__v4,__zpabi_check__v5_0,__zpabi_check__v5_1 locals=__local_check__fail_1,__local_check__fail_0,__local_check__0,__local_check__1,__local_check__2,__local_check__3,__local_check__4,__local_check__5,__local_check__6 indirect=false in_cycle=false
; def main params= locals=__local_main__0 indirect=false in_cycle=false
; call main -> check
; item check section=.text local=false zero=false
; item main section=.text local=false zero=false
; runtime hargs_size=32 hargs_need=23
; @zp-link-meta-end

check:
   SUBROUTINE

   LDA   __zpabi_check__v4
   BMI   .sx_neg@0
   LDA   #$00
   BEQ   .sx_done@1
.sx_neg@0:
   LDA   #$FF
.sx_done@1:
   STA   __local_check__0
   LDA   __zpabi_check__v4
   SEC
   SBC   __zpabi_check__v2_0
   LDA   __local_check__0
   SBC   __zpabi_check__v2_1
   BVC   .cmp_novf@2
   EOR   #$80
.cmp_novf@2:
   ASL
   LDA   #$00
   ROL
   TAX
   LDA   __zpabi_check__v2_0
   STA   __local_check__fail_1
   LDA   __zpabi_check__v2_1
   STA   __local_check__fail_0
   CPX   #$00
   BEQ   .shift_done@4
.shift_loop@3:
   LDA   __local_check__fail_0
   ASL
   ROR   __local_check__fail_0
   ROR   __local_check__fail_1
   DEX
   BNE   .shift_loop@3
.shift_done@4:
   LDA   __local_check__fail_0
   BMI   .lb_skip@0
   JMP   .check@asm_ssa_split@0
.lb_skip@0:
   LDA   #$01
   STA   __local_check__fail_0
   LDA   #$00
   STA   __local_check__fail_1
.if_end@0:
   LDA   __zpabi_check__v3
   CMP   #$04
   BEQ   .if_end@1
   LDA   __local_check__fail_0
   ORA   #$02
   STA   __local_check__fail_0
   LDA   #$00
   STA   __local_check__fail_1
.if_end@1:
   LDA   __zpabi_check__v0_0
   ORA   __zpabi_check__v0_1
   ORA   __zpabi_check__v0_2
   ORA   __zpabi_check__v0_3
   ORA   __zpabi_check__v0_4
   ORA   __zpabi_check__v0_5
   ORA   __zpabi_check__v0_6
   BEQ   .if_end@2
   LDA   __local_check__fail_0
   ORA   #$04
   STA   __local_check__fail_0
.if_end@2:
   LDA   __local_check__0
   BNE   .jcmp_skip@5
   LDA   __zpabi_check__v4
   CMP   #$01
   BEQ   .if_end@3
.jcmp_skip@5:
   LDA   __local_check__fail_0
   ORA   #$08
   STA   __local_check__fail_0
.if_end@3:
   LDA   __zpabi_check__v0_0
   STA   HARGS
   LDA   __zpabi_check__v0_1
   STA   HARGS+1
   LDA   __zpabi_check__v0_2
   STA   HARGS+2
   LDA   __zpabi_check__v0_3
   STA   HARGS+3
   LDA   __zpabi_check__v0_4
   STA   HARGS+4
   LDA   __zpabi_check__v0_5
   STA   HARGS+5
   LDA   __zpabi_check__v0_6
   STA   HARGS+6
   LDA   #$00
   STA   HARGS+7
   LDY   #$FD
   STY   HARGS+8
   LDY   #$00
   STY   HARGS+9
   STY   HARGS+10
   STY   HARGS+11
   STY   HARGS+12
   STY   HARGS+13
   STY   HARGS+14
   STA   HARGS+15
   JSR   udivmod64
   LDA   __zpabi_check__v5_0
   AND   HARGS+16
   STA   __local_check__6
   LDA   __zpabi_check__v5_1
   AND   HARGS+17
   STA   __local_check__5
   LDA   #$00
   AND   HARGS+18
   STA   __local_check__4
   LDA   #$00
   AND   HARGS+19
   STA   __local_check__3
   LDA   #$00
   AND   HARGS+20
   STA   __local_check__2
   LDA   #$00
   AND   HARGS+21
   STA   __local_check__1
   LDA   #$00
   AND   HARGS+22
   STA   __local_check__0
   LDA   __local_check__6
   ORA   __local_check__5
   ORA   __local_check__4
   ORA   __local_check__3
   ORA   __local_check__2
   ORA   __local_check__1
   ORA   __local_check__0
   BEQ   .if_end@4
   LDA   __local_check__fail_0
   ORA   #$10
   STA   __local_check__fail_0
.if_end@4:
   LDA   __zpabi_check__v2_0
   ORA   __zpabi_check__v2_1
   BEQ   .and_false@5
   LDA   __zpabi_check__v3
   BEQ   .and_false@5
   LDX   #$01
   BNE   .and_end@6
.and_false@5:
   LDX   #$00
.and_end@6:
   LDA   __zpabi_check__v5_0
   STA   __local_check__2
   LDA   __zpabi_check__v5_1
   STA   __local_check__3
   CPX   #$00
   BEQ   .shift_done@7
.shift_loop@6:
   ASL   __local_check__2
   ROL   __local_check__3
   DEX
   BNE   .shift_loop@6
.shift_done@7:
   LDA   __zpabi_check__v0_0
   ORA   __local_check__2
   STA   __local_check__1
   LDA   __zpabi_check__v0_1
   ORA   __local_check__3
   CMP   #$42
   BNE   .jcmp_skip@8
   LDA   __local_check__1
   CMP   #$22
   BEQ   .if_end@7
.jcmp_skip@8:
   LDA   __local_check__fail_0
   ORA   #$20
   STA   __local_check__fail_0
.if_end@7:
   LDA   __zpabi_check__v0_0
   ORA   __zpabi_check__v0_0
   STA   __local_check__6
   LDA   __zpabi_check__v0_1
   ORA   __zpabi_check__v0_1
   STA   __local_check__5
   LDA   __zpabi_check__v0_2
   ORA   __zpabi_check__v0_2
   STA   __local_check__4
   LDA   __zpabi_check__v0_3
   ORA   __zpabi_check__v0_3
   STA   __local_check__3
   LDA   __zpabi_check__v0_4
   ORA   __zpabi_check__v0_4
   STA   __local_check__2
   LDA   __zpabi_check__v0_5
   ORA   __zpabi_check__v0_5
   STA   __local_check__1
   LDA   __zpabi_check__v0_6
   ORA   __zpabi_check__v0_6
   STA   __local_check__0
   LDA   __local_check__6
   ORA   __local_check__5
   ORA   __local_check__4
   ORA   __local_check__3
   ORA   __local_check__2
   ORA   __local_check__1
   ORA   __local_check__0
   BEQ   .if_end@9
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

   LDA   #$00
   STA   __zpabi_check__v0_0
   STA   __zpabi_check__v0_1
   STA   __zpabi_check__v0_2
   STA   __zpabi_check__v0_3
   STA   __zpabi_check__v0_4
   STA   __zpabi_check__v0_5
   STA   __zpabi_check__v0_6
   STA   __zpabi_check__v1_1
   STA   __zpabi_check__v1_2
   STA   __zpabi_check__v1_3
   STA   __zpabi_check__v1_4
   STA   __zpabi_check__v1_5
   STA   __zpabi_check__v1_6
   STA   __zpabi_check__v2_1
   LDA   #$0D
   STA   __zpabi_check__v1_0
   LDA   #$0E
   STA   __zpabi_check__v2_0
   LDA   #$04
   STA   __zpabi_check__v3
   LDA   #$01
   STA   __zpabi_check__v4
   LDA   #$11
   STA   __zpabi_check__v5_0
   LDA   #$A1
   STA   __zpabi_check__v5_1
   JMP   check
