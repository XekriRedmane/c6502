SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
z4	EQU	$26
z5	EQU	$27
g1	EQU	$2B
@11.s24	EQU	$2C
g2	EQU	$2E
gp8	EQU	$2F
gp9	EQU	$31
__zpabi_f18__p20	EQU	$80
__zpabi_f27__p28_0	EQU	$80
__zpabi_f18__pp21_0	EQU	$81
__zpabi_f27__p28_1	EQU	$81
__zpabi_f18__pp21_1	EQU	$82
__zpabi_f27__p28_2	EQU	$82
__zpabi_f10__p12_0	EQU	$83
__zpabi_f27__p28_3	EQU	$83
__zpabi_f10__p12_1	EQU	$84
__zpabi_f27__p29_0	EQU	$84
__zpabi_f10__p12_2	EQU	$85
__zpabi_f27__p29_1	EQU	$85
__zpabi_f10__p12_3	EQU	$86
__zpabi_f27__p30_0	EQU	$86
__zpabi_f10__p12_4	EQU	$87
__zpabi_f27__p30_1	EQU	$87
__zpabi_f10__p12_5	EQU	$88
__zpabi_f27__p30_2	EQU	$88
__zpabi_f10__p12_6	EQU	$89
__zpabi_f27__p30_3	EQU	$89
__zpabi_f10__p12_7	EQU	$8A
__zpabi_f27__p30_4	EQU	$8A
__zpabi_f10__pp13_0	EQU	$8B
__zpabi_f27__p30_5	EQU	$8B
__zpabi_f10__pp13_1	EQU	$8C
__zpabi_f27__p30_6	EQU	$8C
__local_f10__l15	EQU	$8D
__zpabi_f27__p30_7	EQU	$8D
__local_f10__0	EQU	$8E
__zpabi_f27__pp31_0	EQU	$8E
__local_f10__l16	EQU	$8F
__zpabi_f27__pp31_1	EQU	$8F
__local_f10__1	EQU	$90
__zpabi_mix__v_0	EQU	$90
__zpabi_mix__v_1	EQU	$91
__zpabi_mix__v_2	EQU	$92
__zpabi_mix__v_3	EQU	$93
__local_f18__0	EQU	$94
__local_f27__w36	EQU	$94
__local_main__l38	EQU	$94
__local_main__0	EQU	$95
__local_f18__w26	EQU	$96
__local_f27__1	EQU	$96
__local_main__1	EQU	$96
__local_f27__2	EQU	$97
__local_main__2	EQU	$97
__local_f18__i25	EQU	$98
__local_f27__3	EQU	$98
__local_main__l39	EQU	$98
__local_f18__l22	EQU	$99
__local_f27__4	EQU	$99
__local_f18__3	EQU	$9A
__local_f27__5	EQU	$9A
__local_f18__l23	EQU	$9B
__local_mix__0	EQU	$9D
__local_mix__1	EQU	$9E
__local_mix__2	EQU	$9F
__local_mix__3	EQU	$A0

; @zp-link-meta-begin
; def f10 params=__zpabi_f10__p11,__zpabi_f10__p12_0,__zpabi_f10__p12_1,__zpabi_f10__p12_2,__zpabi_f10__p12_3,__zpabi_f10__p12_4,__zpabi_f10__p12_5,__zpabi_f10__p12_6,__zpabi_f10__p12_7,__zpabi_f10__pp13_0,__zpabi_f10__pp13_1 locals=__local_f10__l15,__local_f10__0,__local_f10__l16,__local_f10__1,__local_f10__2 indirect=false in_cycle=false param_regs=A,,,,,,,,,,
; def f18 params=__zpabi_f18__p19_0,__zpabi_f18__p19_1,__zpabi_f18__p20,__zpabi_f18__pp21_0,__zpabi_f18__pp21_1 locals=__local_f18__0,__local_f18__1,__local_f18__w26,__local_f18__2,__local_f18__i25,__local_f18__l22,__local_f18__3,__local_f18__l23,__local_f18__5 indirect=false in_cycle=false param_regs=A,X,,,
; def f27 params=__zpabi_f27__p28_0,__zpabi_f27__p28_1,__zpabi_f27__p28_2,__zpabi_f27__p28_3,__zpabi_f27__p29_0,__zpabi_f27__p29_1,__zpabi_f27__p30_0,__zpabi_f27__p30_1,__zpabi_f27__p30_2,__zpabi_f27__p30_3,__zpabi_f27__p30_4,__zpabi_f27__p30_5,__zpabi_f27__p30_6,__zpabi_f27__p30_7,__zpabi_f27__pp31_0,__zpabi_f27__pp31_1 locals=__local_f27__w36,__local_f27__0,__local_f27__1,__local_f27__2,__local_f27__3,__local_f27__4,__local_f27__5 indirect=false in_cycle=false
; def fin0 params= locals=__local_fin0__0,__local_fin0__1,__local_fin0__2,__local_fin0__3 indirect=false in_cycle=false
; def fin1 params= locals=__local_fin1__0 indirect=false in_cycle=false
; def fin2 params= locals=__local_fin2__0,__local_fin2__1,__local_fin2__2 indirect=false in_cycle=false
; def main params= locals=__local_main__l38,__local_main__0,__local_main__1,__local_main__2,__local_main__l39,__local_main__4,__local_main__5,__local_main__6 indirect=false in_cycle=false
; def mix params=__zpabi_mix__v_0,__zpabi_mix__v_1,__zpabi_mix__v_2,__zpabi_mix__v_3 locals=__local_mix__0,__local_mix__1,__local_mix__2,__local_mix__3 indirect=false in_cycle=false
; call f18 -> f10
; call f18 -> mix
; call f27 -> mix
; call fin0 -> mix
; call fin1 -> mix
; call fin2 -> mix
; call main -> f10
; call main -> fin0
; call main -> fin1
; call main -> fin2
; item mix section=.text local=false zero=false
; item fin0 section=.text local=false zero=false
; item fin0@run2 section=.text local=true zero=false
; item fin0@run1 section=.text local=true zero=false
; item fin0@run0 section=.text local=true zero=false
; item fin0@run0@run4 section=.text local=true zero=false
; item fin0@tail0 section=.text local=true zero=false
; item fin1 section=.text local=false zero=false
; item fin2 section=.text local=false zero=false
; item f10 section=.text local=false zero=false
; item f18 section=.text local=false zero=false
; item f18@run3 section=.text local=true zero=false
; item f27 section=.text local=false zero=false
; item main section=.text local=false zero=false
; item acc section=.bss local=false zero=true
; item g3 section=.data local=false zero=false
; item ga6 section=.data local=false zero=false
; item gs7 section=.data local=false zero=false
; item __zp_init section=.text local=false zero=false
; zp z4 size=1 at=auto def=true init=$00
; zp z5 size=4 at=auto def=true init=$03,$00,$00,$00
; zp g1 size=1 at=auto def=true init=$80 weight=134
; zp @11.s24 size=2 at=auto def=true init=$00,$00 weight=128 local=true
; zp g2 size=1 at=auto def=true init=$00 weight=25
; zp gp8 size=2 at=auto def=true init=<g2,>g2 weight=28
; zp gp9 size=2 at=auto def=true init=<g3,>g3 weight=9
; runtime hargs_size=32 hargs_need=24
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
   BMI   .sx_neg@0
   LDA   #$00
   BEQ   .sx_done@1
.sx_neg@0:
   LDA   #$FF
.sx_done@1:
   STA   __zpabi_mix__v_1
   STA   __zpabi_mix__v_2
   STA   __zpabi_mix__v_3
   LDA   g1
   STA   __zpabi_mix__v_0
   JSR   mix
   LDA   g2
   JSR   fin0@run1
   LDA   g3
   STA   __zpabi_mix__v_0
   LDA   g3+1
   STA   __zpabi_mix__v_1
   LDA   g3+2
   STA   __zpabi_mix__v_2
   LDA   g3+3
   JSR   fin0@run2
   LDA   z4
   JMP   fin0@tail0

fin0@run2:
   SUBROUTINE

   STA   __zpabi_mix__v_3
   JMP   mix

fin0@run1:
   SUBROUTINE

   STA   __zpabi_mix__v_0
   JMP   fin0@run0

fin0@run0:
   SUBROUTINE

   JSR   fin0@run0@run4
   JMP   mix

fin0@run0@run4:
   SUBROUTINE

   LDA   #$00
   STA   __zpabi_mix__v_1
   STA   __zpabi_mix__v_2
   STA   __zpabi_mix__v_3
   RTS

fin0@tail0:
   SUBROUTINE

   STA   __zpabi_mix__v_0
   JSR   fin0@run0@run4
   JMP   mix

fin1:
   SUBROUTINE

   LDA   z5
   STA   __zpabi_mix__v_0
   LDA   z5+1
   STA   __zpabi_mix__v_1
   LDA   z5+2
   STA   __zpabi_mix__v_2
   LDA   z5+3
   JSR   fin0@run2
   LDA   ga6
   JSR   fin0@run1
   LDA   ga6+1
   JSR   fin0@run1
   LDA   ga6+2
   JMP   fin0@tail0

fin2:
   SUBROUTINE

   LDA   ga6+3
   JSR   fin0@run1
   LDA   gs7+1
   BMI   .sx_neg@2
   LDA   #$00
   BEQ   .sx_done@3
.sx_neg@2:
   LDA   #$FF
.sx_done@3:
   STA   __zpabi_mix__v_1
   STA   __zpabi_mix__v_2
   STA   __zpabi_mix__v_3
   LDA   gs7+1
   STA   __zpabi_mix__v_0
   JSR   mix
   LDA   gs7+2
   STA   __zpabi_mix__v_0
   LDA   gs7+3
   STA   __zpabi_mix__v_1
   LDA   gs7+4
   STA   __zpabi_mix__v_2
   LDA   #$00
   STA   __zpabi_mix__v_3
   JMP   mix

f10:
   SUBROUTINE

   LDA   #$FF
   STA   __local_f10__l15
   LDA   #$09
   STA   __local_f10__l16
   LDA   #$00
   STA   __local_f10__1
   LDY   #$00
   LDA   (gp8),Y
   SEC
   SBC   #$01
   STA   (gp8),Y
   LDA   __local_f10__l16
   LDX   __local_f10__1
   STX   __local_f10__0
   CLC
   ADC   #$03
   BCC   .carry_inc@0
   INC   __local_f10__0
.carry_inc@0:
   STA   __local_f10__l16
   LDA   __local_f10__0
   STA   __local_f10__1
   LDA   gs7+1
   BMI   .sx_neg@4
   LDA   #$00
   BEQ   .sx_done@5
.sx_neg@4:
   LDA   #$FF
.sx_done@5:
   STA   HARGS+17
   LDX   gs7+1
   STX   HARGS+16
   AND   #$01
   STA   HARGS+18
   LDA   #$00
   STA   HARGS+19
   LDY   #$00
   STY   HARGS+20
   STY   HARGS+21
   STY   HARGS+22
   STA   HARGS+23
   RTS

f18:
   SUBROUTINE

   LDA   #$FF
   STA   __local_f18__l22
   LDA   #$00
   STA   __local_f18__3
   LDA   #$FF
   STA   __local_f18__l23
   LDA   #$00
   STA   __local_f18__i25
.loop@1_start:
   LDA   __local_f18__i25
   BEQ   .lb_skip@1
   JMP   .loop@1_break
.lb_skip@1:
   LDY   #$00
   LDA   (gp8),Y
   ASL
   ASL
   ASL
   ASL
   ASL
   STA   (gp8),Y
   LDA   #$00
   STA   __local_f18__w26
.loop@2_start:
   LDA   #$00
   STA   __zpabi_mix__v_0
   STA   __zpabi_mix__v_1
   STA   __zpabi_mix__v_2
   JSR   fin0@run2
   LDA   z4
   STA   __local_f18__0
   LDA   g1
   LSR
   LDA   #$00
   ROR
   STA   g1
   AND   __local_f18__0
   STA   __local_f18__l23
   BEQ   .cond_else@1
   LDA   gs7+2
   JMP   .cond_end@2
.cond_else@1:
   LDA   __local_f18__i25
.cond_end@2:
   AND   #$07
   CMP   #$02
   BEQ   .case@4
   CMP   #$07
   BNE   .switch@3_break
   LDA   @11.s24
   STA   ga6
   CLC
   ADC   __local_f18__w26
   STA   __zpabi_mix__v_0
   LDA   @11.s24+1
   ADC   #$00
   STA   __local_f18__0
   LDA   #$00
   CMP   __local_f18__0
   BNE   .cmp_end@9
   LDA   __local_f18__i25
   CMP   __zpabi_mix__v_0
   BEQ   .cmp_true@8
   LDA   #$00
   BEQ   .cmp_end@9
.cmp_true@8:
   LDA   #$01
.cmp_end@9:
   STA   __zpabi_mix__v_0
   LDA   #$00
   BPL   .sx_done@12
   LDA   #$FF
.sx_done@12:
   LDX   __zpabi_mix__v_0
   STX   gs7+2
   LDX   #$00
   STX   gs7+3
   AND   #$01
   STA   gs7+4
.case@4:
.switch@3_break:
   INC   __local_f18__w26
   LDA   __local_f18__w26
   CMP   #$03
   BCS   .lb_skip@0
   JMP   .loop@2_start
.lb_skip@0:
   LDA   z4
   JSR   fin0@run1
   INC   __local_f18__i25
   JMP   .loop@1_start
.loop@1_break:
   LDA   z4
   STA   __zpabi_mix__v_0
   CLC
   ADC   #$01
   STA   z4
   JSR   fin0@run0
   LDA   __local_f18__l22
   LDX   __local_f18__3
   LSR
   LDA   #$00
   ROR
   STA   __local_f18__w26
   LDY   #$00
   LDA   (gp8),Y
   STA   __zpabi_f10__p12_0
   LDA   #$00
   STA   __zpabi_f10__p12_1
   STA   __zpabi_f10__p12_2
   JSR   f18@run3
   LDA   #<__local_f18__l23
   STA   __zpabi_f10__pp13_0
   LDA   #>__local_f18__l23
   STA   __zpabi_f10__pp13_1
   LDA   __local_f18__w26
   JSR   f10
   LDA   ga6+1
   LDX   #$FF
   SEC
.div_loop@13:
   INX
   SBC   #$05
   BCS   .div_loop@13
   TXA
   RTS

f18@run3:
   SUBROUTINE

   STA   __zpabi_f10__p12_3
   STA   __zpabi_f10__p12_4
   STA   __zpabi_f10__p12_5
   STA   __zpabi_f10__p12_6
   STA   __zpabi_f10__p12_7
   RTS

f27:
   SUBROUTINE

   LDY   #$00
   LDA   (gp9),Y
   EOR   #$FF
   STA   __zpabi_mix__v_0
   INY
   LDA   (gp9),Y
   EOR   #$FF
   STA   __zpabi_mix__v_1
   INY
   LDA   (gp9),Y
   EOR   #$FF
   STA   __zpabi_mix__v_2
   INY
   LDA   (gp9),Y
   EOR   #$FF
   JSR   fin0@run2
   LDA   gs7+2
   STA   __local_f27__5
   LDA   gs7+3
   STA   __local_f27__4
   LDA   gs7+4
   STA   __local_f27__3
   LDA   g1
   BMI   .sx_neg@14
   LDA   #$00
   BEQ   .sx_done@15
.sx_neg@14:
   LDA   #$FF
.sx_done@15:
   STA   __local_f27__2
   BMI   .sx_neg@16
   LDA   #$00
   BEQ   .sx_done@17
.sx_neg@16:
   LDA   #$FF
.sx_done@17:
   STA   __local_f27__1
   STA   __zpabi_mix__v_0
   LDA   g1
   SEC
   SBC   #$FF
   STA   __local_f27__w36
   LDA   __local_f27__2
   SBC   #$00
   STA   __local_f27__2
   LDA   __local_f27__1
   SBC   #$00
   STA   __local_f27__1
   BCS   .carry_inc@1
   DEC   __zpabi_mix__v_0
.carry_inc@1:
   LDA   __local_f27__w36
   SEC
   SBC   __local_f27__5
   LDA   __local_f27__2
   SBC   __local_f27__4
   LDA   __local_f27__1
   SBC   __local_f27__3
   LDA   __zpabi_mix__v_0
   SBC   #$00
   BVC   .jcmp_novf@18
   EOR   #$80
.jcmp_novf@18:
   BMI   .lb_skip@2
   JMP   .vthread@1
.lb_skip@2:
   LDA   g1
   ASL
   ASL
   STA   __local_f27__1
   STA   g1
   BMI   .sx_neg@19
   LDA   #$00
   BEQ   .sx_done@20
.sx_neg@19:
   LDA   #$FF
.sx_done@20:
   STA   gs7+3
   LDX   __local_f27__1
   STX   gs7+2
   AND   #$01
   STA   gs7+4
   LDA   z5
   ORA   z5+1
   ORA   z5+2
   ORA   z5+3
   BEQ   .and_false@4
   LDA   #$01
   STA   __zpabi_mix__v_0
   LDA   #$00
   BEQ   .and_end@5
.and_false@4:
   LDA   #$00
   STA   __zpabi_mix__v_0
.and_end@5:
   ORA   #$00
   BPL   .sx_done@22
   LDA   #$FF
.sx_done@22:
   STA   __zpabi_mix__v_2
   STA   __zpabi_mix__v_3
   LDA   #$00
   STA   __zpabi_mix__v_1
   JSR   mix
   LDA   #$00
   STA   __local_f27__w36
.loop@6_continue:
   CLC
   ADC   #$01
   STA   __local_f27__1
   LDA   __local_f27__w36
   CMP   #$04
   BCS   .vthread@1
   LDA   g2
   SEC
   SBC   #$FF
   STA   __local_f27__2
   LDA   __local_f27__1
   BEQ   .if_else@7
   LDY   #$00
   LDA   (gp8),Y
   STA   __local_f27__w36
   LDA   #$FF
   SEC
   SBC   __local_f27__w36
   LDA   #$FF
   SBC   #$00
   BVC   .cmp_novf@23
   EOR   #$80
.cmp_novf@23:
   ASL
   LDA   #$00
   ROL
   STA   __local_f27__1
   LDA   __local_f27__2
   AND   #$05
   ORA   __local_f27__1
   LDX   #$00
   RTS
.if_else@7:
   LDA   ga6+2
   EOR   #$03
   STA   ga6+2
   SEC
   SBC   __local_f27__1
   STA   __zpabi_mix__v_0
   LDA   #$00
   SBC   #$00
   STA   __local_f27__w36
   STA   g3+1
   LDA   __zpabi_mix__v_0
   STA   g3
   LDA   __local_f27__w36
   BMI   .sx_neg@24
   LDA   #$00
   BEQ   .sx_done@25
.sx_neg@24:
   LDA   #$FF
.sx_done@25:
   STA   g3+2
   STA   g3+3
   LDA   __local_f27__1
   STA   __local_f27__w36
   JMP   .loop@6_continue
.vthread@1:
   LDA   #$00
   LDX   #$00
   RTS

main:
   SUBROUTINE

   LDA   #$03
   STA   __local_main__l38
   LDA   #$00
   STA   __local_main__0
   STA   __local_main__1
   STA   __local_main__2
   LDA   #$FF
   STA   __local_main__l39
   LDA   g1
   BNE   .or_true@8
   LDA   #$00
   BEQ   .or_end@9
.or_true@8:
   LDA   #$01
.or_end@9:
   AND   #$81
   STA   __local_main__l38
   STA   ga6
   LDA   g1
   EOR   __local_main__l38
   STA   gs7+1
   LDA   #$FF
   STA   __zpabi_f10__p12_0
   STA   __zpabi_f10__p12_1
   LDA   #$00
   STA   __zpabi_f10__p12_2
   JSR   f18@run3
   LDA   #<__local_main__l39
   STA   __zpabi_f10__pp13_0
   LDA   #>__local_main__l39
   STA   __zpabi_f10__pp13_1
   LDY   #$00
   LDA   (gp8),Y
   JSR   f10
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
   DC.L  $80000000

ga6:
   DC.B  $03, $FF, $FF, $7A

gs7:
   DS.B  2
   DC.B  $FF
   DS.B  2

__zp_init:
   SUBROUTINE

   LDA   #$00
   STA   g2
   STA   z4
   STA   z5+1
   STA   z5+2
   STA   z5+3
   STA   @11.s24
   STA   @11.s24+1
   LDA   #$80
   STA   g1
   LDA   #$03
   STA   z5
   LDA   #<g2
   STA   gp8
   LDA   #>g2
   STA   gp8+1
   LDA   #<g3
   STA   gp9
   LDA   #>g3
   STA   gp9+1
   RTS
