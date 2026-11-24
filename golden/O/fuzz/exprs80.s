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
__zpabi_check__v4	EQU	$86
__zpabi_check__v5_0	EQU	$87
__zpabi_check__v5_1	EQU	$88
__zpabi_check__v5_2	EQU	$89
__zpabi_check__v5_3	EQU	$8A
__local_check__fail_1	EQU	$8C
__local_check__fail_0	EQU	$8D
__local_check__0	EQU	$8E
__local_check__1	EQU	$8F
__local_check__2	EQU	$90
__local_check__3	EQU	$91
__local_check__v0_1	EQU	$92
__local_check__v0_0	EQU	$93

; @zp-link-meta-begin
; def check params=__zpabi_check__v0_0,__zpabi_check__v0_1,__zpabi_check__v1_0,__zpabi_check__v1_1,__zpabi_check__v2_0,__zpabi_check__v2_1,__zpabi_check__v3_0,__zpabi_check__v3_1,__zpabi_check__v4,__zpabi_check__v5_0,__zpabi_check__v5_1,__zpabi_check__v5_2,__zpabi_check__v5_3 locals=__local_check__fail_1,__local_check__fail_0,__local_check__0,__local_check__1,__local_check__2,__local_check__3,__local_check__v0_1,__local_check__v0_0,__local_check__4,__local_check__5 indirect=false in_cycle=false param_regs=A,X,,,,,,,,,,,
; def main params= locals=__local_main__0 indirect=false in_cycle=false
; call main -> check
; item check section=.text local=false zero=false
; item check@run0 section=.text local=true zero=false
; item main section=.text local=false zero=false
; runtime hargs_size=32 hargs_need=22
; @zp-link-meta-end

check:
   SUBROUTINE

   STA   __local_check__v0_0
   STX   __local_check__v0_1
   LDA   __zpabi_check__v4
   BMI   .sx_neg@0
   LDA   #$00
   BEQ   .sx_done@1
.sx_neg@0:
   LDA   #$FF
.sx_done@1:
   STA   __local_check__0
   SEC
   LDA   #$00
   SBC   __zpabi_check__v4
   STA   __local_check__fail_0
   LDA   #$00
   SBC   __local_check__0
   CMP   #$FF
   BNE   .jcmp_skip@2
   LDA   __local_check__fail_0
   CMP   #$FD
   BNE   .lb_skip@0
   JMP   .check@asm_ssa_split@0
.lb_skip@0:
.jcmp_skip@2:
   LDA   #$01
   STA   __local_check__fail_0
   LDA   #$00
   STA   __local_check__fail_1
.if_end@0:
   LDA   #$7F
   CLC
   ADC   __zpabi_check__v2_0
   STA   __local_check__3
   LDA   #$00
   ADC   __zpabi_check__v2_1
   STA   __local_check__2
   LDA   __zpabi_check__v3_0
   SEC
   SBC   __zpabi_check__v4
   STA   __local_check__1
   LDA   __zpabi_check__v3_1
   SBC   __local_check__0
   STA   __local_check__0
   LDA   __local_check__2
   CMP   __local_check__0
   BNE   .thread@0
   LDA   __local_check__3
   CMP   __local_check__1
   BEQ   .cmp_true@3
.thread@0:
   LDA   #$00
   BEQ   .cmp_end@4
.cmp_true@3:
   LDA   #$01
.cmp_end@4:
   STA   __local_check__0
   LDA   __zpabi_check__v3_0
   SEC
   SBC   __local_check__0
   LDA   __zpabi_check__v3_1
   SBC   #$00
   BCS   .if_end@1
   LDA   __local_check__fail_0
   ORA   #$02
   STA   __local_check__fail_0
   LDA   #$00
   STA   __local_check__fail_1
.if_end@1:
   LDA   __zpabi_check__v1_0
   CLC
   ADC   __zpabi_check__v3_0
   STA   __local_check__1
   LDA   __zpabi_check__v1_1
   ADC   __zpabi_check__v3_1
   BNE   .jcmp_skip@6
   LDA   __local_check__1
   CMP   #$02
   BEQ   .if_end@2
.jcmp_skip@6:
   LDA   __local_check__fail_0
   ORA   #$04
   STA   __local_check__fail_0
.if_end@2:
   JSR   check@run0
   LDA   __zpabi_check__v1_0
   STA   HARGS+2
   LDA   __zpabi_check__v1_1
   STA   HARGS+3
   JSR   mul16
   LDA   #$01
   CLC
   SBC   HARGS+4
   LDA   #$00
   SBC   HARGS+5
   LDA   #$00
   ROL
   STA   __local_check__0
   LDA   #$00
   STA   __local_check__1
   LDX   __zpabi_check__v2_0
   BEQ   .shift_done@8
.shift_loop@7:
   LDA   __local_check__1
   ASL
   ROR   __local_check__1
   ROR   __local_check__0
   DEX
   BNE   .shift_loop@7
.shift_done@8:
   LDA   __local_check__0
   ORA   __local_check__1
   BEQ   .if_end@5
   LDA   __local_check__fail_0
   ORA   #$08
   STA   __local_check__fail_0
.if_end@5:
   LDA   #$00
   CMP   __zpabi_check__v5_3
   BNE   .cmp_true@9
   CMP   __zpabi_check__v5_2
   BNE   .cmp_true@9
   CMP   __zpabi_check__v5_1
   BNE   .cmp_true@9
   CMP   __zpabi_check__v5_0
   BEQ   .cmp_end@10
.cmp_true@9:
   LDA   #$01
.cmp_end@10:
   AND   __zpabi_check__v1_0
   STA   __local_check__0
   LDA   #$00
   STA   __local_check__1
   SEC
   SBC   __zpabi_check__v5_0
   TAX
   BEQ   .shift_done@13
.shift_loop@12:
   ASL   __local_check__0
   ROL   __local_check__1
   DEX
   BNE   .shift_loop@12
.shift_done@13:
   LDA   __local_check__0
   ORA   __local_check__1
   BEQ   .if_end@6
   LDA   __local_check__fail_0
   ORA   #$10
   STA   __local_check__fail_0
.if_end@6:
   LDA   #$FF
   ORA   __zpabi_check__v3_0
   STA   HARGS+2
   LDA   #$FF
   ORA   __zpabi_check__v3_1
   STA   HARGS+3
   JSR   check@run0
   JSR   udivmod16
   LDX   HARGS+6
   LDA   #$80
   STA   __local_check__0
   CPX   #$00
   BEQ   .shift_done@15
.shift_loop@14:
   LSR   __local_check__0
   DEX
   BNE   .shift_loop@14
.shift_done@15:
   LDA   __local_check__0
   CMP   #$40
   BEQ   .if_end@7
   LDA   __local_check__fail_0
   ORA   #$20
   STA   __local_check__fail_0
.if_end@7:
   LDA   __zpabi_check__v5_0
   STA   HARGS
   LDA   __zpabi_check__v5_1
   STA   HARGS+1
   LDA   __zpabi_check__v5_2
   STA   HARGS+2
   LDA   __zpabi_check__v5_3
   STA   HARGS+3
   LDA   #$02
   STA   HARGS+4
   LDA   #$00
   STA   HARGS+5
   LDY   #$00
   STY   HARGS+6
   STA   HARGS+7
   JSR   sdivmod32
   LDA   __zpabi_check__v2_0
   SEC
   SBC   HARGS+8
   LDA   #$00
   SBC   HARGS+9
   LDA   #$00
   SBC   HARGS+10
   LDA   #$00
   SBC   HARGS+11
   BVC   .cmp_novf@16
   EOR   #$80
.cmp_novf@16:
   ASL
   LDA   #$00
   ROL
   EOR   #$01
   STA   __local_check__2
   LDA   __zpabi_check__v2_0
   STA   HARGS
   STA   HARGS+2
   LDA   __zpabi_check__v2_1
   STA   HARGS+1
   STA   HARGS+3
   JSR   udivmod16
   LDA   __local_check__v0_1
   CMP   HARGS+5
   BNE   .cmp_true@17
   LDA   __local_check__v0_0
   CMP   HARGS+4
   BNE   .cmp_true@17
   LDA   #$00
   BEQ   .cmp_end@18
.cmp_true@17:
   LDA   #$01
.cmp_end@18:
   STA   __local_check__0
   LDA   #$00
   BNE   .jcmp_skip@20
   LDA   __local_check__2
   CMP   __local_check__0
   BEQ   .if_end@8
.jcmp_skip@20:
   LDA   __local_check__fail_0
   ORA   #$40
   STA   __local_check__fail_0
.if_end@8:
   LDA   __zpabi_check__v5_0
   ORA   __zpabi_check__v5_1
   ORA   __zpabi_check__v5_2
   ORA   __zpabi_check__v5_3
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

check@run0:
   SUBROUTINE

   LDA   __zpabi_check__v3_0
   STA   HARGS
   LDA   __zpabi_check__v3_1
   STA   HARGS+1
   RTS

main:
   SUBROUTINE

   LDA   #$01
   STA   __zpabi_check__v1_0
   STA   __zpabi_check__v2_0
   STA   __zpabi_check__v3_0
   LDA   #$03
   STA   __zpabi_check__v4
   LDA   #$00
   STA   __zpabi_check__v1_1
   STA   __zpabi_check__v2_1
   STA   __zpabi_check__v3_1
   STA   __zpabi_check__v5_0
   STA   __zpabi_check__v5_1
   STA   __zpabi_check__v5_2
   STA   __zpabi_check__v5_3
   LDX   #$00
   JMP   check
