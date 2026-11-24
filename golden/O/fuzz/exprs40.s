SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__zpabi_check__v1	EQU	$80
__zpabi_check__v2_0	EQU	$81
__zpabi_check__v2_1	EQU	$82
__zpabi_check__v3_0	EQU	$83
__zpabi_check__v3_1	EQU	$84
__zpabi_check__v3_2	EQU	$85
__zpabi_check__v3_3	EQU	$86
__zpabi_check__v4	EQU	$87
__zpabi_check__v5_0	EQU	$88
__zpabi_check__v5_1	EQU	$89
__local_check__fail_1	EQU	$8B
__local_check__fail_0	EQU	$8C
__local_check__0	EQU	$8D
__local_check__1	EQU	$8E
__local_check__2	EQU	$8F
__local_check__3	EQU	$90
__local_check__4	EQU	$91
__local_check__5	EQU	$92
__local_check__6	EQU	$93
__local_check__7	EQU	$94
__local_check__8	EQU	$95
__local_check__12	EQU	$99
__local_check__13	EQU	$9A
__local_check__v0_1	EQU	$9B
__local_check__v0_0	EQU	$9C

; @zp-link-meta-begin
; def check params=__zpabi_check__v0_0,__zpabi_check__v0_1,__zpabi_check__v1,__zpabi_check__v2_0,__zpabi_check__v2_1,__zpabi_check__v3_0,__zpabi_check__v3_1,__zpabi_check__v3_2,__zpabi_check__v3_3,__zpabi_check__v4,__zpabi_check__v5_0,__zpabi_check__v5_1 locals=__local_check__fail_1,__local_check__fail_0,__local_check__0,__local_check__1,__local_check__2,__local_check__3,__local_check__4,__local_check__5,__local_check__6,__local_check__7,__local_check__8,__local_check__9,__local_check__10,__local_check__11,__local_check__12,__local_check__13,__local_check__v0_1,__local_check__v0_0 indirect=false in_cycle=false param_regs=A,X,,,,,,,,,,
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
   LDA   __zpabi_check__v5_0
   ORA   __zpabi_check__v5_1
   BEQ   .and_false@0
   LDA   __zpabi_check__v1
   BEQ   .and_false@0
   LDA   #$01
   STA   __local_check__fail_0
   JMP   .vthread@0
.and_false@0:
   LDA   #$00
   STA   __local_check__fail_0
   LDA   #$01
.vthread@0:
   STA   HARGS
   LDA   #$80
   STA   HARGS+1
   LDA   #$FF
   STA   HARGS+2
   LDY   #$FF
   STY   HARGS+3
   STA   HARGS+4
   ASL
   LDA   #$00
   ADC   #$FF
   EOR   #$FF
   STA   HARGS+5
   STA   HARGS+6
   STA   HARGS+7
   LDA   __local_check__fail_0
   STA   HARGS+8
   JSR   asr64
   LDA   HARGS+13
   CMP   #$FF
   BNE   .jcmp_skip@2
   LDA   HARGS+12
   CMP   #$FF
   BNE   .jcmp_skip@2
   LDA   HARGS+11
   CMP   #$FF
   BNE   .jcmp_skip@2
   LDA   HARGS+10
   CMP   #$80
   BNE   .jcmp_skip@2
   LDA   HARGS+9
   CMP   #$01
   BNE   .lb_skip@0
   JMP   .check@asm_ssa_split@0
.lb_skip@0:
.jcmp_skip@2:
   LDA   #$01
   STA   __local_check__fail_0
   LDA   #$00
   STA   __local_check__fail_1
.if_end@2:
   LDA   __zpabi_check__v1
   BMI   .sx_neg@3
   LDA   #$00
   BEQ   .sx_done@4
.sx_neg@3:
   LDA   #$FF
.sx_done@4:
   STA   __local_check__4
   LDA   __zpabi_check__v5_0
   STA   __local_check__2
   LDA   __zpabi_check__v5_1
   STA   __local_check__3
   LDX   __zpabi_check__v1
   BEQ   .shift_done@6
.shift_loop@5:
   LDA   __local_check__3
   ASL
   ROR   __local_check__3
   ROR   __local_check__2
   DEX
   BNE   .shift_loop@5
.shift_done@6:
   LDA   __zpabi_check__v2_0
   SEC
   SBC   __zpabi_check__v1
   STA   __local_check__1
   LDA   __zpabi_check__v2_1
   SBC   __local_check__4
   STA   __local_check__0
   LDA   __local_check__2
   EOR   __local_check__1
   STA   __local_check__1
   LDA   __local_check__3
   EOR   __local_check__0
   STA   __local_check__0
   LDA   __local_check__1
   SEC
   SBC   #$FF
   LDA   __local_check__0
   SBC   #$00
   LDA   #$00
   ROL
   STA   __local_check__0
   LDA   #$00
   BNE   .jcmp_skip@7
   LDA   __local_check__0
   CMP   #$01
   BEQ   .if_end@3
.jcmp_skip@7:
   LDA   __local_check__fail_0
   ORA   #$02
   STA   __local_check__fail_0
   LDA   #$00
   STA   __local_check__fail_1
.if_end@3:
   LDA   __zpabi_check__v5_0
   SEC
   SBC   __zpabi_check__v1
   LDA   __zpabi_check__v5_1
   SBC   __local_check__4
   BVC   .cmp_novf@8
   EOR   #$80
.cmp_novf@8:
   ASL
   LDA   #$00
   ROL
   STA   __local_check__0
   LDA   __local_check__v0_0
   SEC
   SBC   __local_check__0
   LDA   __local_check__v0_1
   SBC   #$00
   BVC   .cmp_novf@9
   EOR   #$80
.cmp_novf@9:
   ASL
   LDA   #$00
   ROL
   EOR   #$01
   STA   __local_check__0
   LDA   #$00
   BNE   .jcmp_skip@10
   LDA   __local_check__0
   CMP   #$01
   BEQ   .if_end@4
.jcmp_skip@10:
   LDA   __local_check__fail_0
   ORA   #$04
   STA   __local_check__fail_0
.if_end@4:
   LDA   __local_check__v0_1
   BMI   .sx_neg@11
   LDA   #$00
   BEQ   .sx_done@12
.sx_neg@11:
   LDA   #$FF
.sx_done@12:
   LDX   __local_check__v0_0
   STX   __local_check__8
   LDX   __local_check__v0_1
   STX   __local_check__7
   STA   __local_check__5
   LDA   __zpabi_check__v4
   BMI   .sx_neg@13
   LDA   #$00
   BEQ   .sx_done@14
.sx_neg@13:
   LDA   #$FF
.sx_done@14:
   STA   __local_check__2
   STA   __local_check__1
   AND   #$03
   STA   __local_check__4
   LDA   __zpabi_check__v4
   SEC
   SBC   #$01
   STA   __local_check__0
   LDA   __local_check__2
   SBC   #$00
   STA   __local_check__2
   LDA   __local_check__1
   SBC   #$00
   STA   __local_check__1
   BCS   .carry_inc@0
   DEC   __local_check__4
.carry_inc@0:
   LDA   __local_check__0
   STA   __local_check__3
   LDA   __local_check__4
   AND   #$03
   STA   __local_check__0
   LDA   __local_check__5
   CMP   __local_check__0
   BNE   .thread@0
   CMP   __local_check__1
   BNE   .thread@0
   LDA   __local_check__7
   CMP   __local_check__2
   BNE   .thread@0
   LDA   __local_check__8
   CMP   __local_check__3
   BEQ   .cmp_true@15
.thread@0:
   LDA   #$00
   BEQ   .cmp_end@16
.cmp_true@15:
   LDA   #$01
.cmp_end@16:
   EOR   #$01
   STA   __local_check__1
   LDA   #$00
   BNE   .jcmp_skip@18
   LDA   __local_check__1
   CMP   #$01
   BEQ   .if_end@7
.jcmp_skip@18:
   LDA   __local_check__fail_0
   ORA   #$20
   STA   __local_check__fail_0
.if_end@7:
   LDA   __local_check__v0_1
   BMI   .sx_neg@19
   LDA   #$00
   BEQ   .sx_done@20
.sx_neg@19:
   LDA   #$FF
.sx_done@20:
   STA   __local_check__13
   STA   __local_check__12
   LDA   __local_check__v0_0
   STA   HARGS+4
   LDA   __local_check__v0_1
   STA   HARGS+5
   LDA   __local_check__13
   STA   HARGS+6
   AND   #$03
   STA   HARGS+7
   LDA   #$07
   STA   HARGS
   LDA   #$00
   STA   HARGS+1
   LDY   #$00
   STY   HARGS+2
   STA   HARGS+3
   JSR   mul32
   LDA   HARGS+8
   STA   HARGS
   LDA   HARGS+9
   STA   HARGS+1
   LDA   HARGS+10
   STA   HARGS+2
   LDA   HARGS+11
   AND   #$03
   STA   HARGS+3
   LDA   __zpabi_check__v5_0
   STA   HARGS+4
   JSR   lsr32
   LDA   __zpabi_check__v3_0
   EOR   #$FF
   STA   __local_check__7
   LDA   __zpabi_check__v3_1
   EOR   #$FF
   STA   __local_check__6
   LDA   __zpabi_check__v3_2
   EOR   #$FF
   STA   __local_check__5
   LDA   __zpabi_check__v3_3
   EOR   #$FF
   STA   __local_check__4
   LDA   __local_check__v0_0
   STA   __local_check__3
   LDA   __local_check__v0_1
   STA   __local_check__2
   LDA   __local_check__13
   STA   __local_check__1
   LDA   __local_check__12
   AND   #$03
   STA   __local_check__0
   LDA   __local_check__3
   SEC
   SBC   __local_check__7
   LDA   __local_check__2
   SBC   __local_check__6
   LDA   __local_check__1
   SBC   __local_check__5
   LDA   __local_check__0
   SBC   __local_check__4
   BVC   .cmp_novf@21
   EOR   #$80
.cmp_novf@21:
   ASL
   LDA   #$00
   ROL
   STA   __local_check__2
   LDA   #$00
   BPL   .sx_done@23
   LDA   #$FF
.sx_done@23:
   STA   __local_check__1
   LDA   __local_check__2
   STA   __local_check__3
   LDA   __local_check__1
   AND   #$03
   STA   __local_check__0
   LDA   HARGS+8
   CMP   __local_check__0
   BNE   .jcmp_skip@24
   LDA   HARGS+7
   CMP   __local_check__1
   BNE   .jcmp_skip@24
   LDA   HARGS+6
   BNE   .jcmp_skip@24
   LDA   HARGS+5
   CMP   __local_check__3
   BEQ   .if_end@9
.jcmp_skip@24:
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
   JMP   .if_end@2

main:
   SUBROUTINE

   LDA   #$80
   STA   __zpabi_check__v2_1
   LDA   #$FF
   STA   __zpabi_check__v3_0
   STA   __zpabi_check__v3_1
   STA   __zpabi_check__v3_2
   STA   __zpabi_check__v3_3
   LDA   #$0A
   STA   __zpabi_check__v5_0
   LDA   #$00
   STA   __zpabi_check__v1
   STA   __zpabi_check__v2_0
   STA   __zpabi_check__v4
   STA   __zpabi_check__v5_1
   LDX   #$00
   LDA   #$01
   JMP   check
