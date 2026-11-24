SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__zpabi_check__v2	EQU	$80
__zpabi_check__v3	EQU	$81
__zpabi_check__v4_0	EQU	$82
__zpabi_check__v4_1	EQU	$83
__zpabi_check__v4_2	EQU	$84
__zpabi_check__v4_3	EQU	$85
__zpabi_check__v5_0	EQU	$86
__zpabi_check__v5_1	EQU	$87
__zpabi_check__v5_2	EQU	$88
__local_check__fail_1	EQU	$8A
__local_check__fail_0	EQU	$8B
__local_check__0	EQU	$8C
__local_check__1	EQU	$8D
__local_check__2	EQU	$8E
__local_check__3	EQU	$8F
__local_check__4	EQU	$90
__local_check__5	EQU	$91
__local_check__6	EQU	$92
__local_check__7	EQU	$93
__local_check__8	EQU	$94
__local_check__9	EQU	$95
__local_check__v1	EQU	$96
__local_check__v0	EQU	$97

; @zp-link-meta-begin
; def check params=__zpabi_check__v0,__zpabi_check__v1,__zpabi_check__v2,__zpabi_check__v3,__zpabi_check__v4_0,__zpabi_check__v4_1,__zpabi_check__v4_2,__zpabi_check__v4_3,__zpabi_check__v5_0,__zpabi_check__v5_1,__zpabi_check__v5_2 locals=__local_check__fail_1,__local_check__fail_0,__local_check__0,__local_check__1,__local_check__2,__local_check__3,__local_check__4,__local_check__5,__local_check__6,__local_check__7,__local_check__8,__local_check__9,__local_check__v1,__local_check__v0,__local_check__10,__local_check__11,__local_check__12,__local_check__13,__local_check__14 indirect=false in_cycle=false param_regs=A,X,,,,,,,,,
; def main params= locals=__local_main__0 indirect=false in_cycle=false
; call main -> check
; item check section=.text local=false zero=false
; item check@run1 section=.text local=true zero=false
; item check@run0 section=.text local=true zero=false
; item main section=.text local=false zero=false
; runtime hargs_size=32 hargs_need=22
; @zp-link-meta-end

check:
   SUBROUTINE

   STA   __local_check__v0
   STX   __local_check__v1
   ORA   #$00
   BMI   .sx_neg@0
   LDA   #$00
   BEQ   .sx_done@1
.sx_neg@0:
   LDA   #$FF
.sx_done@1:
   STA   __local_check__9
   LDA   #$02
   SEC
   SBC   __local_check__v0
   STA   __local_check__3
   LDA   #$00
   SBC   __local_check__9
   STA   __local_check__2
   LDA   __local_check__3
   STA   __local_check__7
   LDA   __local_check__2
   STA   __local_check__3
   BMI   .sx_neg@4
   LDA   #$00
   BEQ   .sx_done@5
.sx_neg@4:
   LDA   #$FF
.sx_done@5:
   STA   __local_check__2
   STA   __local_check__1
   LDA   __local_check__9
   BNE   .thread@0
   LDA   __local_check__v0
   BEQ   .cmp_true@6
.thread@0:
   LDA   #$00
   BEQ   .cmp_end@7
.cmp_true@6:
   LDA   #$01
.cmp_end@7:
   STA   __local_check__0
   LDA   #$00
   BPL   .sx_done@10
   LDA   #$FF
.sx_done@10:
   STA   __local_check__fail_0
   LDA   __zpabi_check__v3
   BMI   .sx_neg@11
   LDA   #$00
   BEQ   .sx_done@12
.sx_neg@11:
   LDA   #$FF
.sx_done@12:
   STA   __local_check__8
   LDA   __zpabi_check__v3
   BMI   .sx_neg@13
   LDA   #$00
   BEQ   .sx_done@14
.sx_neg@13:
   LDA   #$FF
.sx_done@14:
   STA   __local_check__6
   STA   __local_check__5
   STA   __local_check__4
   LDA   __zpabi_check__v4_0
   AND   __zpabi_check__v3
   STA   HARGS+4
   LDA   __zpabi_check__v4_1
   AND   __local_check__6
   STA   HARGS+5
   LDA   __zpabi_check__v4_2
   AND   __local_check__5
   STA   HARGS+6
   LDA   __zpabi_check__v4_3
   AND   __local_check__4
   STA   HARGS+7
   LDA   __local_check__0
   STA   HARGS
   LDA   #$00
   STA   HARGS+1
   LDA   __local_check__fail_0
   STA   HARGS+2
   STA   HARGS+3
   JSR   udivmod32
   LDA   HARGS+12
   STA   HARGS+4
   LDA   HARGS+13
   STA   HARGS+5
   LDA   HARGS+14
   STA   HARGS+6
   LDA   HARGS+15
   STA   HARGS+7
   LDA   __local_check__7
   STA   HARGS
   LDA   __local_check__3
   STA   HARGS+1
   LDA   __local_check__2
   STA   HARGS+2
   LDA   __local_check__1
   STA   HARGS+3
   JSR   mul32
   JSR   check@run1
   ORA   HARGS+11
   BNE   .lb_skip@0
   JMP   .check@asm_ssa_split@0
.lb_skip@0:
   LDA   #$01
   STA   __local_check__fail_0
   LDA   #$00
   STA   __local_check__fail_1
.if_end@0:
   SEC
   SBC   __zpabi_check__v3
   STA   __local_check__3
   LDA   __zpabi_check__v3
   SBC   __local_check__6
   STA   __local_check__2
   LDA   __local_check__6
   SBC   __local_check__5
   STA   __local_check__1
   LDA   __local_check__5
   SBC   __local_check__4
   STA   __local_check__0
   LDA   __local_check__3
   SEC
   SBC   __zpabi_check__v4_0
   LDA   __local_check__2
   SBC   __zpabi_check__v4_1
   LDA   __local_check__1
   SBC   __zpabi_check__v4_2
   LDA   __local_check__0
   SBC   __zpabi_check__v4_3
   BCS   .if_end@1
   LDA   __local_check__fail_0
   ORA   #$02
   STA   __local_check__fail_0
   LDA   #$00
   STA   __local_check__fail_1
.if_end@1:
   LDA   __local_check__v0
   BMI   .sx_neg@15
   LDA   #$00
   BEQ   .sx_done@16
.sx_neg@15:
   LDA   #$FF
.sx_done@16:
   STA   HARGS+5
   STA   HARGS+6
   STA   HARGS+7
   JSR   check@run0
   LDA   __local_check__v0
   STA   HARGS+4
   JSR   udivmod32
   JSR   check@run1
   ORA   HARGS+11
   BNE   .or_true@2
   LDA   #$00
   BPL   .sx_done@18
   LDA   #$FF
.sx_done@18:
   LDX   __zpabi_check__v2
   STX   HARGS+4
   LDX   #$00
   STX   HARGS+5
   AND   #$7F
   STA   HARGS+6
   LDA   __zpabi_check__v5_0
   STA   HARGS
   LDA   __zpabi_check__v5_1
   STA   HARGS+1
   LDA   __zpabi_check__v5_2
   STA   HARGS+2
   LDA   #$00
   STA   HARGS+3
   STA   HARGS+7
   JSR   udivmod32
   JSR   check@run1
   BNE   .or_true@2
   LDA   #$00
   STA   __local_check__7
   JMP   .or_end@3
.or_true@2:
   LDA   #$01
   STA   __local_check__7
.or_end@3:
   LDA   #$45
   ORA   __zpabi_check__v5_0
   STA   __local_check__5
   LDA   #$23
   ORA   __zpabi_check__v5_1
   STA   __local_check__4
   LDA   #$01
   ORA   __zpabi_check__v5_2
   STA   __local_check__3
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
   STA   __local_check__1
   LDA   #$00
   BPL   .sx_done@20
   LDA   #$FF
.sx_done@20:
   LDX   __local_check__1
   STX   __local_check__2
   AND   #$7F
   STA   __local_check__0
   LDA   __local_check__2
   CLC
   SBC   __local_check__5
   LDA   #$00
   SBC   __local_check__4
   LDA   __local_check__0
   SBC   __local_check__3
   LDA   #$00
   ROL
   SEC
   SBC   __local_check__7
   LDA   #$00
   SBC   #$00
   BVC   .jcmp_novf@21
   EOR   #$80
.jcmp_novf@21:
   BMI   .if_end@4
   LDA   __local_check__fail_0
   ORA   #$04
   STA   __local_check__fail_0
.if_end@4:
   LDA   __local_check__v1
   BEQ   .if_end@6
   LDA   __local_check__fail_0
   ORA   #$10
   STA   __local_check__fail_0
.if_end@6:
   LDA   __zpabi_check__v3
   STA   HARGS
   LDA   __local_check__8
   STA   HARGS+1
   LDA   __local_check__v0
   STA   HARGS+2
   LDA   __local_check__9
   STA   HARGS+3
   JSR   sdivmod16
   LDA   __local_check__v1
   EOR   #$FF
   STA   __local_check__1
   BMI   .sx_neg@22
   LDA   #$00
   BEQ   .sx_done@23
.sx_neg@22:
   LDA   #$FF
.sx_done@23:
   STA   __local_check__0
   LDA   HARGS+4
   SEC
   SBC   __local_check__1
   LDA   HARGS+5
   SBC   __local_check__0
   BVC   .cmp_novf@24
   EOR   #$80
.cmp_novf@24:
   ASL
   LDA   #$00
   ROL
   EOR   #$01
   STA   __local_check__1
   LDA   #$00
   BPL   .sx_done@26
   LDA   #$FF
.sx_done@26:
   STA   HARGS+10
   STA   HARGS+11
   STA   HARGS+12
   STA   __local_check__0
   JSR   check@run0
   LDA   #$00
   STA   HARGS+4
   STA   HARGS+5
   ASL
   LDA   #$00
   ADC   #$FF
   EOR   #$FF
   STA   HARGS+6
   STA   HARGS+7
   LDA   __local_check__1
   STA   HARGS+8
   LDA   #$00
   STA   HARGS+9
   LDA   __local_check__0
   STA   HARGS+13
   ASL
   LDA   #$00
   ADC   #$FF
   EOR   #$FF
   STA   HARGS+14
   STA   HARGS+15
   JSR   sdivmod64
   LDA   HARGS+21
   AND   #$03
   EOR   #$02
   SEC
   SBC   #$02
   BNE   .jcmp_skip@27
   LDA   HARGS+20
   BNE   .jcmp_skip@27
   LDA   HARGS+19
   BNE   .jcmp_skip@27
   LDA   HARGS+18
   BNE   .jcmp_skip@27
   LDA   HARGS+17
   BNE   .jcmp_skip@27
   LDA   HARGS+16
   CMP   #$01
   BEQ   .if_end@7
.jcmp_skip@27:
   LDA   __local_check__fail_0
   ORA   #$20
   STA   __local_check__fail_0
.if_end@7:
   LDA   __zpabi_check__v2
   CMP   #$FF
   BEQ   .if_end@8
   LDA   __local_check__fail_0
   ORA   #$40
   STA   __local_check__fail_0
.if_end@8:
   LDA   __local_check__v0
   EOR   #$FF
   STA   __local_check__4
   LDA   __local_check__9
   EOR   #$FF
   STA   __local_check__3
   LDA   __local_check__v1
   BMI   .sx_neg@28
   LDA   #$00
   BEQ   .sx_done@29
.sx_neg@28:
   LDA   #$FF
.sx_done@29:
   STA   __local_check__0
   LDA   __zpabi_check__v2
   CLC
   ADC   __local_check__v1
   STA   __local_check__2
   LDA   #$00
   ADC   __local_check__0
   STA   __local_check__1
   LDA   __zpabi_check__v2
   STA   __local_check__0
   LDX   __zpabi_check__v3
   BEQ   .shift_done@31
.shift_loop@30:
   LSR   __local_check__0
   DEX
   BNE   .shift_loop@30
.shift_done@31:
   LDA   __local_check__1
   BNE   .cmp_true@32
   LDA   __local_check__2
   CMP   __local_check__0
   BNE   .cmp_true@32
   LDA   #$00
   BEQ   .cmp_end@33
.cmp_true@32:
   LDA   #$01
.cmp_end@33:
   STA   __local_check__0
   LDA   __local_check__3
   BNE   .if_end@9
   LDA   __local_check__4
   CMP   __local_check__0
   BNE   .if_end@9
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

check@run1:
   SUBROUTINE

   LDA   HARGS+8
   ORA   HARGS+9
   ORA   HARGS+10
   RTS

check@run0:
   SUBROUTINE

   LDA   __zpabi_check__v4_0
   STA   HARGS
   LDA   __zpabi_check__v4_1
   STA   HARGS+1
   LDA   __zpabi_check__v4_2
   STA   HARGS+2
   LDA   __zpabi_check__v4_3
   STA   HARGS+3
   RTS

main:
   SUBROUTINE

   LDA   #$FF
   STA   __zpabi_check__v2
   LDA   #$01
   STA   __zpabi_check__v3
   STA   __zpabi_check__v4_0
   LDA   #$00
   STA   __zpabi_check__v4_1
   STA   __zpabi_check__v4_2
   STA   __zpabi_check__v4_3
   LDA   #$79
   STA   __zpabi_check__v5_0
   LDA   #$6C
   STA   __zpabi_check__v5_1
   LDA   #$32
   STA   __zpabi_check__v5_2
   LDX   #$00
   LDA   #$0B
   JMP   check
