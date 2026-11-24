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
__local_check__9	EQU	$96
__local_check__10	EQU	$97
__local_check__11	EQU	$98
__local_check__12	EQU	$99
__local_check__13	EQU	$9A
__local_check__14	EQU	$9B
__local_check__15	EQU	$9C
__local_check__16	EQU	$9D
__local_check__v0_1	EQU	$9E
__local_check__v0_0	EQU	$9F

; @zp-link-meta-begin
; def check params=__zpabi_check__v0_0,__zpabi_check__v0_1,__zpabi_check__v1_0,__zpabi_check__v1_1,__zpabi_check__v1_2,__zpabi_check__v1_3,__zpabi_check__v2_0,__zpabi_check__v2_1,__zpabi_check__v3,__zpabi_check__v4,__zpabi_check__v5_0,__zpabi_check__v5_1 locals=__local_check__fail_1,__local_check__fail_0,__local_check__0,__local_check__1,__local_check__2,__local_check__3,__local_check__4,__local_check__5,__local_check__6,__local_check__7,__local_check__8,__local_check__9,__local_check__10,__local_check__11,__local_check__12,__local_check__13,__local_check__14,__local_check__15,__local_check__16,__local_check__v0_1,__local_check__v0_0,__local_check__17,__local_check__18,__local_check__19,__local_check__20,__local_check__21,__local_check__22,__local_check__23 indirect=false in_cycle=false param_regs=A,X,,,,,,,,,,
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
   SEC
   SBC   __zpabi_check__v5_0
   LDA   #$00
   SBC   __zpabi_check__v5_1
   BVC   .cmp_novf@0
   EOR   #$80
.cmp_novf@0:
   ASL
   LDA   #$00
   ROL
   STA   __local_check__3
   LDA   #$00
   BPL   .sx_done@2
   LDA   #$FF
.sx_done@2:
   STA   __local_check__11
   STA   __local_check__10
   STA   __local_check__9
   STA   __local_check__8
   STA   __local_check__7
   LDA   __zpabi_check__v5_0
   STA   __local_check__5
   LDA   __zpabi_check__v5_1
   STA   __local_check__6
   LDX   __zpabi_check__v2_0
   BEQ   .shift_done@4
.shift_loop@3:
   LDA   __local_check__6
   ASL
   ROR   __local_check__6
   ROR   __local_check__5
   DEX
   BNE   .shift_loop@3
.shift_done@4:
   LDX   __zpabi_check__v1_0
   BEQ   .shift_done@6
.shift_loop@5:
   LDA   __local_check__6
   ASL
   ROR   __local_check__6
   ROR   __local_check__5
   DEX
   BNE   .shift_loop@5
.shift_done@6:
   LDA   __local_check__6
   BMI   .sx_neg@7
   LDA   #$00
   BEQ   .sx_done@8
.sx_neg@7:
   LDA   #$FF
.sx_done@8:
   STA   __local_check__2
   STA   __local_check__1
   STA   __local_check__0
   STA   __local_check__fail_0
   STA   __local_check__fail_1
   LDA   __local_check__3
   SEC
   SBC   __local_check__5
   STA   __local_check__4
   LDA   #$00
   SBC   __local_check__6
   STA   __local_check__3
   LDA   __local_check__11
   SBC   __local_check__2
   STA   __local_check__2
   LDA   __local_check__10
   SBC   __local_check__1
   STA   __local_check__1
   LDA   __local_check__9
   SBC   __local_check__0
   STA   __local_check__0
   LDA   __local_check__8
   SBC   __local_check__fail_0
   STA   __local_check__fail_0
   LDA   __local_check__7
   SBC   __local_check__fail_1
   JSR   check@run0
   BNE   .jcmp_skip@9
   LDA   __local_check__fail_0
   BNE   .jcmp_skip@9
   LDA   __local_check__0
   BNE   .jcmp_skip@9
   LDA   __local_check__1
   BNE   .jcmp_skip@9
   LDA   __local_check__2
   BNE   .jcmp_skip@9
   LDA   __local_check__3
   BNE   .jcmp_skip@9
   LDA   __local_check__4
   CMP   #$01
   BNE   .lb_skip@0
   JMP   .check@asm_ssa_split@0
.lb_skip@0:
.jcmp_skip@9:
   LDA   #$01
   STA   __local_check__fail_0
   LDA   #$00
   STA   __local_check__fail_1
.if_end@0:
   LDA   __local_check__v0_1
   BNE   .jcmp_skip@10
   LDA   __local_check__v0_0
   CMP   #$01
   BEQ   .if_end@1
.jcmp_skip@10:
   LDA   __local_check__fail_0
   ORA   #$02
   STA   __local_check__fail_0
   LDA   #$00
   STA   __local_check__fail_1
.if_end@1:
   LDA   __zpabi_check__v5_0
   ORA   __zpabi_check__v5_1
   BEQ   .and_false@2
   LDA   #$01
   STA   __local_check__16
   LDA   #$00
   STA   __local_check__14
   JMP   .and_end@3
.and_false@2:
   LDA   #$00
   STA   __local_check__16
   LDY   #$00
   STY   __local_check__14
.and_end@3:
   ORA   #$00
   BPL   .sx_done@12
   LDA   #$FF
.sx_done@12:
   STA   __local_check__13
   STA   __local_check__12
   STA   __local_check__11
   STA   __local_check__10
   STA   __local_check__9
   STA   __local_check__8
   LDA   __zpabi_check__v3
   BMI   .sx_neg@13
   LDA   #$00
   BEQ   .sx_done@14
.sx_neg@13:
   LDA   #$FF
.sx_done@14:
   STA   __local_check__15
   LDA   __zpabi_check__v3
   ORA   __zpabi_check__v2_0
   STA   HARGS
   LDA   __local_check__15
   ORA   __zpabi_check__v2_1
   STA   HARGS+1
   LDA   __zpabi_check__v4
   STA   HARGS+2
   LDA   #$00
   STA   HARGS+3
   JSR   sdivmod16
   LDA   HARGS+7
   BMI   .sx_neg@15
   LDA   #$00
   BEQ   .sx_done@16
.sx_neg@15:
   LDA   #$FF
.sx_done@16:
   STA   __local_check__5
   STA   __local_check__4
   STA   __local_check__3
   STA   __local_check__2
   STA   __local_check__1
   STA   __local_check__0
   LDA   HARGS+6
   SEC
   SBC   __local_check__16
   LDA   HARGS+7
   SBC   __local_check__14
   LDA   __local_check__5
   SBC   __local_check__13
   LDA   __local_check__4
   SBC   __local_check__12
   LDA   __local_check__3
   SBC   __local_check__11
   LDA   __local_check__2
   SBC   __local_check__10
   LDA   __local_check__1
   SBC   __local_check__9
   LDA   __local_check__0
   SBC   __local_check__8
   BVC   .cmp_novf@17
   EOR   #$80
.cmp_novf@17:
   ASL
   LDA   #$00
   ROL
   STA   __local_check__0
   LDA   #$00
   BNE   .jcmp_skip@18
   LDA   __local_check__0
   CMP   #$01
   BEQ   .if_end@4
.jcmp_skip@18:
   LDA   __local_check__fail_0
   ORA   #$04
   STA   __local_check__fail_0
.if_end@4:
   LDA   __local_check__v0_0
   ORA   __local_check__v0_1
   BEQ   .lnot_true@19
   LDA   #$00
   BEQ   .lnot_end@20
.lnot_true@19:
   LDA   #$01
.lnot_end@20:
   STA   __local_check__2
   LDA   __zpabi_check__v5_0
   AND   __zpabi_check__v2_0
   STA   __local_check__1
   LDA   __zpabi_check__v5_1
   AND   __zpabi_check__v2_1
   STA   __local_check__0
   LDA   #$00
   CMP   __local_check__0
   BNE   .cmp_end@22
   LDA   __local_check__2
   CMP   __local_check__1
   BEQ   .cmp_true@21
   LDA   #$00
   BEQ   .cmp_end@22
.cmp_true@21:
   LDA   #$01
.cmp_end@22:
   SEC
   SBC   __zpabi_check__v5_0
   LDA   #$00
   SBC   __zpabi_check__v5_1
   BVC   .jcmp_novf@24
   EOR   #$80
.jcmp_novf@24:
   BMI   .if_end@5
   LDA   __local_check__fail_0
   ORA   #$08
   STA   __local_check__fail_0
.if_end@5:
   LDA   __zpabi_check__v2_0
   SEC
   SBC   __zpabi_check__v4
   STA   __local_check__0
   LDA   __zpabi_check__v2_1
   SBC   #$00
   STA   __local_check__1
   LDA   __zpabi_check__v3
   BMI   .sx_neg@25
   LDA   #$00
   BEQ   .sx_done@26
.sx_neg@25:
   LDA   #$FF
.sx_done@26:
   STA   __local_check__4
   STA   __local_check__3
   STA   __local_check__2
   LDA   #$7F
   EOR   __zpabi_check__v3
   STA   HARGS
   LDA   #$00
   EOR   __local_check__4
   STA   HARGS+1
   LDA   #$00
   EOR   __local_check__3
   STA   HARGS+2
   LDA   #$00
   EOR   __local_check__2
   STA   HARGS+3
   LDA   #$02
   STA   HARGS+4
   LDA   #$00
   STA   HARGS+5
   LDY   #$00
   STY   HARGS+6
   STA   HARGS+7
   JSR   sdivmod32
   LDX   HARGS+12
   BEQ   .shift_done@28
.shift_loop@27:
   LDA   __local_check__1
   ASL
   ROR   __local_check__1
   ROR   __local_check__0
   DEX
   BNE   .shift_loop@27
.shift_done@28:
   LDA   __local_check__1
   BNE   .jcmp_skip@29
   LDA   __local_check__0
   CMP   #$03
   BEQ   .if_end@6
.jcmp_skip@29:
   LDA   __local_check__fail_0
   ORA   #$10
   STA   __local_check__fail_0
.if_end@6:
   LDA   __zpabi_check__v2_1
   BMI   .sx_neg@30
   LDA   #$00
   BEQ   .sx_done@31
.sx_neg@30:
   LDA   #$FF
.sx_done@31:
   STA   __local_check__5
   STA   __local_check__4
   STA   __local_check__3
   STA   __local_check__2
   STA   __local_check__1
   STA   __local_check__0
   LDA   #$01
   CLC
   ADC   __zpabi_check__v2_0
   STA   __local_check__7
   LDA   #$00
   ADC   __zpabi_check__v2_1
   STA   __local_check__6
   LDA   #$00
   ADC   __local_check__5
   STA   __local_check__5
   LDA   #$00
   ADC   __local_check__4
   STA   __local_check__4
   LDA   #$00
   ADC   __local_check__3
   STA   __local_check__3
   LDA   #$00
   ADC   __local_check__2
   STA   __local_check__2
   LDA   #$00
   ADC   __local_check__1
   STA   __local_check__1
   LDA   #$00
   ADC   __local_check__0
   JSR   check@run0
   STA   __local_check__0
   LDA   __local_check__7
   ORA   __local_check__6
   ORA   __local_check__5
   ORA   __local_check__4
   ORA   __local_check__3
   ORA   __local_check__2
   ORA   __local_check__1
   ORA   __local_check__0
   BEQ   .and_false@7
   LDA   __zpabi_check__v5_0
   SEC
   SBC   __zpabi_check__v2_0
   STA   __local_check__1
   LDA   __zpabi_check__v5_1
   SBC   __zpabi_check__v2_1
   ORA   __local_check__1
   BEQ   .and_false@7
   LDA   #$01
   STA   __local_check__14
   LDA   #$00
   BEQ   .and_end@8
.and_false@7:
   LDA   #$00
   STA   __local_check__14
.and_end@8:
   ORA   #$00
   BPL   .sx_done@33
   LDA   #$FF
.sx_done@33:
   STA   __local_check__12
   STA   __local_check__11
   STA   __local_check__10
   STA   __local_check__9
   STA   __local_check__8
   LDA   __zpabi_check__v3
   ORA   __local_check__v0_0
   STA   __local_check__5
   LDA   __local_check__15
   ORA   __local_check__v0_1
   STA   __local_check__4
   BMI   .sx_neg@34
   LDA   #$00
   BEQ   .sx_done@35
.sx_neg@34:
   LDA   #$FF
.sx_done@35:
   STA   __local_check__3
   STA   __local_check__2
   STA   __local_check__1
   STA   __local_check__7
   SEC
   SBC   #$80
   STA   __local_check__0
   LDA   __local_check__2
   SBC   #$00
   STA   __local_check__2
   LDA   __local_check__1
   SBC   #$00
   STA   __local_check__1
   BCS   .carry_inc@0
   DEC   __local_check__7
.carry_inc@0:
   LDA   __local_check__5
   STA   __local_check__6
   LDA   __local_check__4
   STA   __local_check__5
   LDA   __local_check__3
   STA   __local_check__4
   LDA   __local_check__0
   STA   __local_check__3
   LDA   __local_check__7
   JSR   check@run0
   STA   __local_check__0
   LDA   __local_check__14
   ORA   __local_check__6
   STA   __local_check__6
   LDA   __local_check__12
   ORA   __local_check__4
   STA   __local_check__4
   LDA   __local_check__11
   ORA   __local_check__3
   STA   __local_check__3
   LDA   __local_check__10
   ORA   __local_check__2
   STA   __local_check__2
   LDA   __local_check__9
   ORA   __local_check__1
   STA   __local_check__1
   LDA   __local_check__8
   ORA   __local_check__0
   CMP   #$FF
   BNE   .jcmp_skip@36
   LDA   __local_check__1
   CMP   #$FF
   BNE   .jcmp_skip@36
   LDA   __local_check__2
   CMP   #$FF
   BNE   .jcmp_skip@36
   LDA   __local_check__3
   CMP   #$80
   BNE   .jcmp_skip@36
   LDA   __local_check__4
   BNE   .jcmp_skip@36
   LDA   __local_check__5
   BNE   .jcmp_skip@36
   LDA   __local_check__6
   CMP   #$01
   BEQ   .if_end@9
.jcmp_skip@36:
   LDA   __local_check__fail_0
   ORA   #$20
   STA   __local_check__fail_0
.if_end@9:
   LDA   __local_check__v0_1
   BNE   .jcmp_skip@37
   LDA   __local_check__v0_0
   CMP   #$01
   BEQ   .if_end@10
.jcmp_skip@37:
   LDA   __local_check__fail_0
   ORA   #$40
   STA   __local_check__fail_0
.if_end@10:
   LDA   __zpabi_check__v3
   CMP   #$01
   BEQ   .if_end@11
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

check@run0:
   SUBROUTINE

   AND   #$0F
   EOR   #$08
   SEC
   SBC   #$08
   RTS

main:
   SUBROUTINE

   LDA   #$0D
   STA   __zpabi_check__v1_0
   LDA   #$00
   STA   __zpabi_check__v1_1
   STA   __zpabi_check__v1_2
   STA   __zpabi_check__v1_3
   STA   __zpabi_check__v2_1
   LDA   #$04
   STA   __zpabi_check__v2_0
   LDA   #$01
   STA   __zpabi_check__v3
   STA   __zpabi_check__v4
   LDA   #$D0
   STA   __zpabi_check__v5_0
   LDA   #$1F
   STA   __zpabi_check__v5_1
   LDX   #$00
   LDA   #$01
   JMP   check
