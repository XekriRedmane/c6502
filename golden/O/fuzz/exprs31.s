SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__zpabi_check__v2	EQU	$80
__zpabi_check__v3	EQU	$81
__zpabi_check__v4	EQU	$82
__zpabi_check__v5	EQU	$83
__local_check__fail_1	EQU	$85
__local_check__fail_0	EQU	$86
__local_check__0	EQU	$87
__local_check__1	EQU	$88
__local_check__2	EQU	$89
__local_check__3	EQU	$8A
__local_check__4	EQU	$8B
__local_check__5	EQU	$8C
__local_check__6	EQU	$8D
__local_check__v1	EQU	$8E
__local_check__v0	EQU	$8F

; @zp-link-meta-begin
; def check params=__zpabi_check__v0,__zpabi_check__v1,__zpabi_check__v2,__zpabi_check__v3,__zpabi_check__v4,__zpabi_check__v5 locals=__local_check__fail_1,__local_check__fail_0,__local_check__0,__local_check__1,__local_check__2,__local_check__3,__local_check__4,__local_check__5,__local_check__6,__local_check__v1,__local_check__v0,__local_check__7 indirect=false in_cycle=false param_regs=A,X,,,,
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
   LDA   __zpabi_check__v2
   BMI   .sx_neg@0
   LDA   #$00
   BEQ   .sx_done@1
.sx_neg@0:
   LDA   #$FF
.sx_done@1:
   STA   __local_check__5
   TXA
   EOR   __zpabi_check__v2
   STA   HARGS
   TXA
   CLC
   ADC   __zpabi_check__v3
   STA   HARGS+2
   LDA   #$00
   ADC   #$00
   STA   HARGS+3
   LDA   #$00
   STA   HARGS+1
   JSR   mul16
   LDA   HARGS+4
   ORA   HARGS+5
   BNE   .lb_skip@0
   JMP   .check@asm_ssa_split@0
.lb_skip@0:
   LDA   #$01
   STA   __local_check__fail_0
   LDA   #$00
   STA   __local_check__fail_1
.if_end@0:
   LDA   __zpabi_check__v5
   ASL
   LDA   #$00
   ROL
   STA   __local_check__1
   LDA   __zpabi_check__v4
   BMI   .sx_neg@2
   LDA   #$00
   BEQ   .sx_done@3
.sx_neg@2:
   LDA   #$FF
.sx_done@3:
   STA   __local_check__6
   LDA   __zpabi_check__v2
   AND   __zpabi_check__v4
   STA   __local_check__0
   LDA   __local_check__5
   AND   __local_check__6
   STA   __local_check__2
   LDA   __local_check__1
   SEC
   SBC   __local_check__0
   STA   __local_check__0
   LDA   #$00
   SBC   __local_check__2
   STA   __local_check__1
   SEC
   LDA   #$00
   SBC   __zpabi_check__v2
   TAX
   BEQ   .shift_done@5
.shift_loop@4:
   LDA   __local_check__1
   ASL
   ROR   __local_check__1
   ROR   __local_check__0
   DEX
   BNE   .shift_loop@4
.shift_done@5:
   LDA   __local_check__1
   BNE   .jcmp_skip@6
   LDA   __local_check__0
   CMP   #$01
   BEQ   .if_end@6
.jcmp_skip@6:
   LDA   __local_check__fail_0
   ORA   #$04
   STA   __local_check__fail_0
   LDA   #$00
   STA   __local_check__fail_1
.if_end@6:
   LDA   __zpabi_check__v2
   ORA   #$07
   STA   __local_check__4
   LDA   __local_check__5
   STA   __local_check__3
   LDA   __zpabi_check__v5
   SEC
   SBC   __local_check__v1
   LDA   #$00
   ROL
   STA   __local_check__2
   LDA   #$00
   BPL   .sx_done@8
   LDA   #$FF
.sx_done@8:
   STA   __local_check__1
   STA   __local_check__0
   LDA   __local_check__v1
   SEC
   SBC   __local_check__2
   STA   HARGS+4
   LDA   #$00
   SBC   #$00
   STA   HARGS+5
   LDA   #$00
   SBC   __local_check__1
   STA   HARGS+6
   LDA   #$00
   SBC   __local_check__0
   STA   HARGS+7
   LDA   __local_check__4
   STA   HARGS
   LDA   __local_check__3
   STA   HARGS+1
   LDA   #$00
   STA   HARGS+2
   STA   HARGS+3
   JSR   mul32
   LDA   HARGS+11
   CMP   #$FF
   BNE   .jcmp_skip@9
   LDA   HARGS+10
   CMP   #$FF
   BNE   .jcmp_skip@9
   LDA   HARGS+9
   CMP   #$FF
   BNE   .jcmp_skip@9
   LDA   HARGS+8
   CMP   #$F9
   BEQ   .if_end@7
.jcmp_skip@9:
   LDA   __local_check__fail_0
   ORA   #$08
   STA   __local_check__fail_0
.if_end@7:
   LDA   __zpabi_check__v2
   ORA   __local_check__5
   BEQ   .if_end@10
   LDA   __local_check__fail_0
   ORA   #$10
   STA   __local_check__fail_0
.if_end@10:
   LDA   __zpabi_check__v4
   SEC
   SBC   #$FF
   LDA   __local_check__6
   SBC   #$FF
   BVC   .cmp_novf@10
   EOR   #$80
.cmp_novf@10:
   ASL
   LDA   #$00
   ROL
   STA   __local_check__2
   LDA   #$FF
   SEC
   SBC   __zpabi_check__v2
   STA   __local_check__1
   LDA   #$FF
   SBC   __local_check__5
   STA   __local_check__0
   LDA   __local_check__2
   EOR   __local_check__1
   STA   __local_check__1
   LDA   #$00
   EOR   __local_check__0
   CMP   #$FF
   BNE   .jcmp_skip@11
   LDA   __local_check__1
   CMP   #$FF
   BEQ   .if_end@11
.jcmp_skip@11:
   LDA   __local_check__fail_0
   ORA   #$20
   STA   __local_check__fail_0
.if_end@11:
   LDA   __local_check__v0
   BMI   .sx_neg@12
   LDA   #$00
   BEQ   .sx_done@13
.sx_neg@12:
   LDA   #$FF
.sx_done@13:
   STA   __local_check__2
   STA   __local_check__1
   STA   __local_check__0
   LDA   #$00
   CMP   __local_check__0
   BNE   .cmp_end@15
   CMP   __local_check__1
   BNE   .cmp_end@15
   CMP   __local_check__2
   BNE   .cmp_end@15
   LDA   #$80
   CMP   __local_check__v0
   BEQ   .cmp_true@14
   LDA   #$00
   BEQ   .cmp_end@15
.cmp_true@14:
   LDA   #$01
.cmp_end@15:
   STA   __local_check__5
   LDA   #$00
   BPL   .sx_done@18
   LDA   #$FF
.sx_done@18:
   STA   __local_check__4
   STA   __local_check__3
   LDA   __zpabi_check__v2
   BMI   .sx_neg@19
   LDA   #$00
   BEQ   .sx_done@20
.sx_neg@19:
   LDA   #$FF
.sx_done@20:
   STA   __local_check__2
   STA   __local_check__1
   STA   __local_check__0
   LDA   __zpabi_check__v2
   SEC
   SBC   __local_check__5
   LDA   __local_check__2
   SBC   #$00
   LDA   __local_check__1
   SBC   __local_check__4
   LDA   __local_check__0
   SBC   __local_check__3
   BVC   .cmp_novf@21
   EOR   #$80
.cmp_novf@21:
   ASL
   LDA   #$00
   ROL
   EOR   #$01
   ORA   __zpabi_check__v3
   STA   __local_check__1
   LDA   #$00
   BNE   .jcmp_skip@22
   LDA   __local_check__1
   CMP   #$01
   BEQ   .if_end@12
.jcmp_skip@22:
   LDA   __local_check__fail_0
   ORA   #$40
   STA   __local_check__fail_0
.if_end@12:
   LDA   __zpabi_check__v4
   ORA   __zpabi_check__v5
   STA   __local_check__0
   LDA   __local_check__6
   STA   __local_check__1
   LDX   __local_check__v1
   BEQ   .shift_done@24
.shift_loop@23:
   ASL   __local_check__0
   ROL   __local_check__1
   DEX
   BNE   .shift_loop@23
.shift_done@24:
   LDA   #$01
   SEC
   SBC   __local_check__0
   LDA   #$00
   SBC   __local_check__1
   BVC   .cmp_novf@25
   EOR   #$80
.cmp_novf@25:
   ASL
   LDA   #$00
   ROL
   STA   __local_check__0
   LDA   #$00
   BNE   .jcmp_skip@26
   LDA   __local_check__0
   CMP   #$01
   BEQ   .if_end@13
.jcmp_skip@26:
   LDA   __local_check__fail_0
   ORA   #$80
   STA   __local_check__fail_0
.if_end@13:
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
   STA   __zpabi_check__v2
   STA   __zpabi_check__v3
   STA   __zpabi_check__v4
   LDA   #$FF
   STA   __zpabi_check__v5
   LDX   #$00
   JMP   check
