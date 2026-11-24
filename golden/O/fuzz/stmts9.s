SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
gp7	EQU	$26
gp6	EQU	$28
g1	EQU	$2A
g2	EQU	$2B
__local_f21__0	EQU	$80
__zpabi_f13__p15_0	EQU	$80
__local_f21__1	EQU	$81
__zpabi_f13__p15_1	EQU	$81
__local_f21__2	EQU	$82
__zpabi_f13__p16	EQU	$82
__zpabi_f13__pp17_0	EQU	$83
__zpabi_f13__pp17_1	EQU	$84
__zpabi_mix__v_0	EQU	$85
__zpabi_mix__v_1	EQU	$86
__zpabi_mix__v_2	EQU	$87
__zpabi_mix__v_3	EQU	$88
__local_main__0	EQU	$89
__local_main__1	EQU	$8A
__local_main__3	EQU	$8C
__local_main__4	EQU	$8D
__local_main__5	EQU	$8E
__local_main__7	EQU	$90
__local_main__l27	EQU	$91
__local_main__l29	EQU	$92
__local_f13__0	EQU	$94
__local_f13__1	EQU	$95
__local_f13__2	EQU	$96
__local_f13__3	EQU	$97
__local_f13__4	EQU	$98
__local_f13__5	EQU	$99
__local_f13__6	EQU	$9A
__local_f13__7	EQU	$9B
__local_f13__l19	EQU	$9C
__local_f13__8	EQU	$9D
__local_f13__l20	EQU	$9E
__local_f8__l11	EQU	$A1
__local_f8__l10	EQU	$A2
__local_f8__pp9	EQU	$A3
__local_f8__1	EQU	$A4
__local_mix__0	EQU	$A5
__local_mix__1	EQU	$A6
__local_mix__2	EQU	$A7
__local_mix__3	EQU	$A8

; @zp-link-meta-begin
; def f13 params=__zpabi_f13__p14_0,__zpabi_f13__p14_1,__zpabi_f13__p15_0,__zpabi_f13__p15_1,__zpabi_f13__p16,__zpabi_f13__pp17_0,__zpabi_f13__pp17_1 locals=__local_f13__0,__local_f13__1,__local_f13__2,__local_f13__3,__local_f13__4,__local_f13__5,__local_f13__6,__local_f13__7,__local_f13__l19,__local_f13__8,__local_f13__l20,__local_f13__10 indirect=false in_cycle=false param_regs=A,X,,,,,
; def f21 params=__zpabi_f21__pp22_0,__zpabi_f21__pp22_1 locals=__local_f21__0,__local_f21__1,__local_f21__2,__local_f21__3 indirect=false in_cycle=false param_regs=A,X
; def f8 params=__zpabi_f8__pp9_0,__zpabi_f8__pp9_1 locals=__local_f8__0,__local_f8__l11,__local_f8__l10,__local_f8__pp9,__local_f8__1 indirect=false in_cycle=false param_regs=A,X
; def fin0 params= locals=__local_fin0__0,__local_fin0__1 indirect=false in_cycle=false
; def fin1 params= locals=__local_fin1__0,__local_fin1__1,__local_fin1__2,__local_fin1__3 indirect=false in_cycle=false
; def fin2 params= locals=__local_fin2__0,__local_fin2__1,__local_fin2__2,__local_fin2__3 indirect=false in_cycle=false
; def main params= locals=__local_main__0,__local_main__1,__local_main__2,__local_main__3,__local_main__4,__local_main__5,__local_main__6,__local_main__7,__local_main__l27,__local_main__l29,__local_main__10 indirect=false in_cycle=false
; def mix params=__zpabi_mix__v_0,__zpabi_mix__v_1,__zpabi_mix__v_2,__zpabi_mix__v_3 locals=__local_mix__0,__local_mix__1,__local_mix__2,__local_mix__3 indirect=false in_cycle=false
; call f13 -> f8
; call f13 -> mix
; call f8 -> mix
; call fin0 -> mix
; call fin1 -> mix
; call fin2 -> mix
; call main -> f13
; call main -> f21
; call main -> fin0
; call main -> fin1
; call main -> fin2
; call main -> mix
; item mix section=.text local=false zero=false
; item fin0 section=.text local=false zero=false
; item fin0@run2 section=.text local=true zero=false
; item fin0@run1 section=.text local=true zero=false
; item fin0@run0 section=.text local=true zero=false
; item fin0@tail0 section=.text local=true zero=false
; item fin1 section=.text local=false zero=false
; item fin2 section=.text local=false zero=false
; item f8 section=.text local=false zero=false
; item f13 section=.text local=false zero=false
; item f21 section=.text local=false zero=false
; item main section=.text local=false zero=false
; item acc section=.bss local=false zero=true
; item g3 section=.data local=false zero=false
; item ga4 section=.data local=false zero=false
; item gs5 section=.data local=false zero=false
; item __zp_init section=.text local=false zero=false
; zp gp7 size=2 at=auto def=true init=<g1,>g1 weight=30
; zp gp6 size=2 at=auto def=true init=<g3,>g3 weight=27
; zp g1 size=1 at=auto def=true init=$7F weight=4
; zp g2 size=4 at=auto def=true init=$80,$00,$00,$00 weight=13
; runtime hargs_size=32 hargs_need=18
; @zp-link-meta-end

mix:
   SUBROUTINE

   LDA   acc
   ASL
   STA   __local_mix__2
   LDA   acc+1
   ROL
   STA   __local_mix__3
   LDA   #$00
   ROL
   ORA   __local_mix__2
   LDX   __local_mix__3
   STX   __local_mix__0
   EOR   __zpabi_mix__v_0
   STA   __local_mix__1
   LDA   __local_mix__0
   EOR   __zpabi_mix__v_1
   STA   __local_mix__0
   LDA   __local_mix__1
   EOR   __zpabi_mix__v_2
   STA   acc
   LDA   __local_mix__0
   EOR   __zpabi_mix__v_3
   STA   acc+1
   RTS

fin0:
   SUBROUTINE

   LDA   g1
   JSR   fin0@run0
   LDA   g2
   STA   __zpabi_mix__v_0
   LDA   g2+1
   STA   __zpabi_mix__v_1
   LDA   g2+2
   STA   __zpabi_mix__v_2
   LDA   g2+3
   JSR   fin0@run1
   LDA   g3+1
   BMI   .sx_neg@0
   LDA   #$00
   BEQ   .sx_done@1
.sx_neg@0:
   LDA   #$FF
.sx_done@1:
   STA   __zpabi_mix__v_2
   STA   __zpabi_mix__v_3
   LDA   g3
   STA   __zpabi_mix__v_0
   LDA   g3+1
   STA   __zpabi_mix__v_1
   JSR   mix
   LDA   ga4
   JSR   fin0@run2
   JMP   fin0@tail0

fin0@run2:
   SUBROUTINE

   STA   __zpabi_mix__v_0
   LDA   #$00
   STA   __zpabi_mix__v_1
   STA   __zpabi_mix__v_2
   RTS

fin0@run1:
   SUBROUTINE

   STA   __zpabi_mix__v_3
   JMP   mix

fin0@run0:
   SUBROUTINE

   JSR   fin0@run2
   STA   __zpabi_mix__v_3
   JMP   mix

fin0@tail0:
   SUBROUTINE

   STA   __zpabi_mix__v_3
   JMP   mix

fin1:
   SUBROUTINE

   LDA   ga4+1
   JSR   fin0@run0
   LDA   ga4+2
   JSR   fin0@run0
   LDA   ga4+3
   JSR   fin0@run0
   LDA   gs5+1
   STA   __zpabi_mix__v_0
   LDA   gs5+2
   STA   __zpabi_mix__v_1
   LDA   gs5+3
   STA   __zpabi_mix__v_2
   LDA   gs5+4
   JMP   fin0@tail0

fin2:
   SUBROUTINE

   LDA   gs5+5
   STA   __zpabi_mix__v_0
   LDA   gs5+6
   STA   __zpabi_mix__v_1
   LDA   gs5+7
   STA   __zpabi_mix__v_2
   LDA   gs5+8
   JMP   fin0@tail0

f8:
   SUBROUTINE

   STA   __local_f8__pp9
   STX   __local_f8__1
   LDA   #$FF
   STA   __local_f8__l10
   LDA   #$01
   STA   __local_f8__l11
   LDA   __local_f8__l11
   STA   __zpabi_mix__v_1
   LDX   #$04
.shift_loop@2:
   LDA   __zpabi_mix__v_1
   ASL
   ROR   __zpabi_mix__v_1
   DEX
   BNE   .shift_loop@2
   LDA   __zpabi_mix__v_1
   STA   __local_f8__l11
   BEQ   .if_else@1
   LDA   gs5+5
   AND   #$07
   BEQ   .switch@0_break
   CMP   #$02
   BEQ   .case@2
   CMP   #$04
   BEQ   .case@3
   BNE   .default@4
.case@2:
   LDA   __local_f8__l10
   LDY   #$00
   STA   (gp6),Y
   LDA   #$00
   INY
   STA   (gp6),Y
.case@3:
   LDA   gs5+5
   EOR   #$FF
   STA   __zpabi_mix__v_0
   LDA   gs5+6
   EOR   #$FF
   STA   __zpabi_mix__v_1
   LDA   gs5+7
   EOR   #$FF
   STA   __zpabi_mix__v_2
   LDA   gs5+8
   EOR   #$FF
   JSR   fin0@run1
   LDY   #$00
   LDA   (gp7),Y
   CLC
   ADC   #$01
   STA   (gp7),Y
   JMP   .switch@0_break
.default@4:
   LDA   ga4+3
   LDY   #$00
   AND   (__local_f8__pp9),Y
   LDX   #$00
   RTS
.switch@0_break:
   LDA   g2
   LDX   g2+1
   RTS
.if_else@1:
   LDA   __local_f8__l11
   STA   __local_f8__l11
   BEQ   .if_end@2
   LDA   g3
   EOR   #$FF
   STA   __local_f8__l11
   STA   g3
   LDA   g3+1
   STA   __zpabi_mix__v_1
   BMI   .sx_neg@4
   LDA   #$00
   BEQ   .sx_done@5
.sx_neg@4:
   LDA   #$FF
.sx_done@5:
   STA   __zpabi_mix__v_2
   STA   __zpabi_mix__v_3
   LDA   __local_f8__l11
   STA   __zpabi_mix__v_0
   JSR   mix
   LDY   #$00
   LDA   (gp7),Y
   AND   #$05
   STA   (gp7),Y
.if_end@2:
   INC   g2
   BNE   .inc_done@0
   INC   g2+1
   BNE   .inc_done@0
   INC   g2+2
   BNE   .inc_done@0
   INC   g2+3
.inc_done@0:
   LDA   g2
   SEC
   SBC   #$FF
   AND   #$07
   CMP   #$03
   BEQ   .case@6
   CMP   #$05
   BEQ   .case@7
.case@6:
.case@7:
   LDX   #$00
   LDA   #$FF
   RTS

f13:
   SUBROUTINE

   LDA   #$7F
   STA   __local_f13__l19
   LDA   #$00
   STA   __local_f13__8
   LDA   #$FF
   STA   __local_f13__l20
   STA   __local_f13__l20+1
   LDA   gs5+1
   STA   __local_f13__7
   LDA   gs5+2
   STA   __local_f13__6
   LDA   gs5+3
   STA   __local_f13__5
   LDA   gs5+4
   STA   __local_f13__4
   LDA   __local_f13__7
   CLC
   ADC   #$01
   STA   gs5+1
   LDA   __local_f13__6
   ADC   #$00
   STA   gs5+2
   LDA   __local_f13__5
   ADC   #$00
   STA   gs5+3
   LDA   __local_f13__4
   ADC   #$00
   STA   gs5+4
   LDA   gs5+5
   STA   __local_f13__3
   LDA   gs5+6
   STA   __local_f13__2
   LDA   gs5+7
   STA   __local_f13__1
   LDA   gs5+8
   STA   __local_f13__0
   LDA   __local_f13__7
   AND   __local_f13__3
   STA   __zpabi_mix__v_0
   LDA   __local_f13__6
   AND   __local_f13__2
   STA   __zpabi_mix__v_1
   LDA   __local_f13__5
   AND   __local_f13__1
   STA   __zpabi_mix__v_2
   LDA   __local_f13__4
   AND   __local_f13__0
   JSR   fin0@run1
   LDX   #>__local_f13__l20
   LDA   #<__local_f13__l20
   JSR   f8
   LDA   __local_f13__l19
   LDX   __local_f13__8
   STX   __local_f13__1
   SEC
   SBC   #$81
   BCS   .carry_inc@1
   DEC   __local_f13__1
.carry_inc@1:
   STA   __local_f13__l19
   LDA   __local_f13__1
   STA   __local_f13__8
   LDA   gs5+1
   LDX   gs5+2
   RTS

f21:
   SUBROUTINE

   LDY   #$00
   LDA   (gp6),Y
   STA   __local_f21__1
   INY
   LDA   (gp6),Y
   STA   __local_f21__0
   LDA   #$00
   EOR   __local_f21__1
   STA   __local_f21__1
   LDA   #$00
   EOR   __local_f21__0
   ORA   __local_f21__1
   BEQ   .if_end@3
   DEY
   LDA   (gp7),Y
   STA   __local_f21__1
   CLC
   ADC   #$01
   STA   (gp7),Y
   LDA   __local_f21__1
   STA   gs5+1
   LDA   #$00
   STA   gs5+2
   STY   gs5+3
   STA   gs5+4
.if_end@3:
   LDA   gs5+1
   LDX   gs5+2
   STX   __local_f21__2
   LDX   gs5+3
   STX   __local_f21__1
   LDX   gs5+4
   STX   __local_f21__0
   ORA   __local_f21__2
   ORA   __local_f21__1
   ORA   __local_f21__0
   BEQ   .if_end@4
   LDY   #$00
   LDA   (gp6),Y
   AND   #$07
   CMP   #$04
   BEQ   .case@15
   CMP   #$05
   BEQ   .case@16
   CMP   #$06
   BEQ   .case@17
   CMP   #$07
   BEQ   .case@18
   BNE   .default@19
.case@15:
.case@16:
   LDA   gs5+1
   CLC
   ADC   #$81
   STA   gs5+1
   BCC   .carry_inc@2
   INC   gs5+2
   BNE   .carry_inc@2
   INC   gs5+3
   BNE   .carry_inc@2
   INC   gs5+4
.carry_inc@2:
.case@17:
   LDA   #$10
   SEC
   SBC   g3
   RTS
.case@18:
.default@19:
.if_end@4:
   LDA   #$FF
   RTS

main:
   SUBROUTINE

   LDA   #$7F
   STA   __local_main__l27
   LDA   #$FF
   STA   __local_main__l29
   STA   __local_main__l29+1
   LDY   #$00
   LDA   (gp7),Y
   BEQ   .cond_else@7
   LDA   #$01
   STA   __local_main__5
   LDA   #$00
   STA   __local_main__7
   STA   __local_main__4
   JMP   .cond_end@8
.cond_else@7:
   LDA   g1
   STA   __local_main__7
   LDA   #$00
   BPL   .sx_done@7
   LDA   #$FF
.sx_done@7:
   STA   __local_main__5
   STA   __local_main__4
   LDA   #$00
.cond_end@8:
   STA   HARGS+1
   LDA   __local_main__7
   STA   HARGS
   LDA   __local_main__5
   STA   HARGS+2
   LDA   __local_main__4
   STA   HARGS+3
   STA   HARGS+4
   STA   HARGS+5
   STA   HARGS+6
   STA   HARGS+7
   LDA   #$06
   STA   HARGS+8
   LDA   #$00
   STA   HARGS+9
   LDY   #$00
   STY   HARGS+10
   STY   HARGS+11
   STY   HARGS+12
   STY   HARGS+13
   STY   HARGS+14
   STA   HARGS+15
   JSR   sdivmod64
   LDA   HARGS+16
   STA   __local_main__l29
   LDA   HARGS+17
   STA   __local_main__l29+1
   LDX   #>__local_main__l27
   LDA   #<__local_main__l27
   JSR   f21
   STA   __local_main__l29
   ORA   #$00
   BMI   .sx_neg@8
   LDA   #$00
   BEQ   .sx_done@9
.sx_neg@8:
   LDA   #$FF
.sx_done@9:
   STA   __local_main__l29+1
   LDA   __local_main__l29
   ORA   g2
   LDY   #$00
   STA   (gp7),Y
   LDA   __local_main__l27
   STA   __local_main__1
   BMI   .sx_neg@10
   LDA   #$00
   BEQ   .sx_done@11
.sx_neg@10:
   LDA   #$FF
.sx_done@11:
   STA   __local_main__0
   LDA   g3
   ORA   __local_main__1
   STA   __local_main__3
   LDA   g3+1
   ORA   __local_main__0
   STA   __zpabi_f13__p15_0
   LDA   __local_main__l27
   BMI   .sx_neg@12
   LDA   #$00
   BEQ   .sx_done@13
.sx_neg@12:
   LDA   #$FF
.sx_done@13:
   STA   __local_main__0
   LDA   __local_main__1
   SEC
   SBC   __local_main__3
   LDA   __local_main__0
   SBC   __zpabi_f13__p15_0
   BVC   .cmp_novf@14
   EOR   #$80
.cmp_novf@14:
   ASL
   LDA   #$00
   ROL
   EOR   #$01
   AND   #$07
   LDY   #$00
   ORA   #$00
   BEQ   .case@21
   CMP   #$01
   BNE   .lb_skip@3
   JMP   .case@26
.lb_skip@3:
   CMP   #$04
   BNE   .lb_skip@2
   JMP   .case@27
.lb_skip@2:
   CMP   #$06
   BNE   .lb_skip@1
   JMP   .case@28
.lb_skip@1:
   JMP   .switch@20_break
.case@21:
   LDA   gs5+5
   STA   __local_main__3
   LDA   gs5+6
   STA   __zpabi_f13__p15_0
   LDA   gs5+7
   STA   __local_main__5
   LDA   gs5+8
   STA   __local_main__4
   LDA   __local_main__3
   SEC
   SBC   #$01
   STA   gs5+5
   LDA   __zpabi_f13__p15_0
   SBC   #$00
   STA   gs5+6
   LDA   __local_main__5
   SBC   #$00
   STA   gs5+7
   LDA   __local_main__4
   SBC   #$00
   STA   gs5+8
   TYA
   SEC
   SBC   ga4+1
   STA   __local_main__1
   LDA   #$01
   SBC   #$00
   STA   __local_main__0
   LDA   __local_main__3
   EOR   __local_main__1
   STA   __local_main__3
   LDA   __zpabi_f13__p15_0
   EOR   __local_main__0
   STA   __zpabi_f13__p15_0
   LDA   __local_main__5
   STA   __local_main__1
   LDA   __local_main__4
   STA   __local_main__0
   LDA   __local_main__3
   ORA   __zpabi_f13__p15_0
   ORA   __local_main__1
   ORA   __local_main__0
   BNE   .lb_skip@0
   JMP   .switch@20_break
.lb_skip@0:
   LDY   #$00
   LDA   (gp6),Y
   STA   __local_main__0
   INY
   LDA   (gp6),Y
   STA   __local_main__1
   INC   __local_main__0
   BNE   .inc_done@1
   INC   __local_main__1
.inc_done@1:
   LDA   __local_main__0
   LDY   #$00
   STA   (gp6),Y
   LDA   __local_main__1
   INY
   STA   (gp6),Y
   STA   g2+1
   LDA   __local_main__0
   STA   g2
   LDA   __local_main__1
   BMI   .sx_neg@15
   LDA   #$00
   BEQ   .sx_done@16
.sx_neg@15:
   LDA   #$FF
.sx_done@16:
   STA   g2+2
   STA   g2+3
   LDA   ga4+1
   CLC
   ADC   #$81
   STA   ga4+1
   LDY   #$00
   LDA   (gp6),Y
   STA   HARGS
   INY
   LDA   (gp6),Y
   STA   HARGS+1
   LDA   g1
   STA   HARGS+2
   LDA   #$00
   STA   HARGS+3
   JSR   mul16
   LDX   HARGS+5
   LDA   ga4+3
   STA   __zpabi_f13__p15_0
   LDA   #$7F
   AND   gs5+1
   STA   __zpabi_f13__p16
   LDA   #$00
   STA   __zpabi_f13__p15_1
   LDA   #<__local_main__l29
   STA   __zpabi_f13__pp17_0
   LDA   #>__local_main__l29
   STA   __zpabi_f13__pp17_1
   LDA   HARGS+4
   JSR   f13
   STA   __local_main__1
   TXA
   BMI   .sx_neg@17
   LDA   #$00
   BEQ   .sx_done@18
.sx_neg@17:
   LDA   #$FF
.sx_done@18:
   STA   __zpabi_mix__v_2
   STA   __zpabi_mix__v_3
   LDA   __local_main__1
   STA   __zpabi_mix__v_0
   STX   __zpabi_mix__v_1
   JSR   mix
   LDA   ga4+1
   ASL
   ASL
   ASL
   STA   __local_main__0
   STA   ga4+1
   LDA   #$FF
   CLC
   ADC   __local_main__0
   AND   #$07
   BNE   .switch@20_break
.case@26:
.case@27:
.case@28:
.switch@20_break:
   JSR   fin0
   JSR   fin1
   JSR   fin2
   LDA   acc+1
   AND   #$7F
   TAX
   LDA   acc
   RTS

acc:
   DS.B  2

g3:
   DC.W  $0002

ga4:
   DC.B  $FF, $FF, $8D, $00

gs5:
   DS.B  1
   DC.L  $00010000
   DC.L  $23E6F01C

__zp_init:
   SUBROUTINE

   LDA   #$00
   STA   g2+1
   STA   g2+2
   STA   g2+3
   LDA   #$7F
   STA   g1
   LDA   #$80
   STA   g2
   LDA   #<g3
   STA   gp6
   LDA   #>g3
   STA   gp6+1
   LDA   #<g1
   STA   gp7
   LDA   #>g1
   STA   gp7+1
   RTS
