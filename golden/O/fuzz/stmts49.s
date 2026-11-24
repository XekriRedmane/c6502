SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
z3	EQU	$26
z4	EQU	$27
g1	EQU	$29
gp7	EQU	$2A
@13.s25	EQU	$2C
@9.s19	EQU	$2D
g2	EQU	$2E
__zpabi_f14__p16_0	EQU	$80
__zpabi_f8__p10_0	EQU	$80
__zpabi_f14__p16_1	EQU	$81
__zpabi_f8__p10_1	EQU	$81
__zpabi_f14__p17_0	EQU	$82
__zpabi_f8__p10_2	EQU	$82
__zpabi_f14__p17_1	EQU	$83
__zpabi_f8__p10_3	EQU	$83
__zpabi_mix__v_0	EQU	$84
__zpabi_mix__v_1	EQU	$85
__zpabi_mix__v_2	EQU	$86
__zpabi_mix__v_3	EQU	$87
__local_f21__w26	EQU	$88
__local_f21__0	EQU	$89
__local_f21__1	EQU	$8A
__local_f21__3	EQU	$8C
__local_f21__l23	EQU	$8D
__local_f21__pp22	EQU	$8E
__local_f21__4	EQU	$8F
__local_f14__w20	EQU	$90
__local_f8__l11	EQU	$90
__local_f14__0	EQU	$91
__local_f8__0	EQU	$91
__local_f14__p16_1	EQU	$92
__local_f14__p16_0	EQU	$93
__local_f14__1	EQU	$94
__local_f14__p15_1	EQU	$95
__local_f14__p15_0	EQU	$96
__local_f14__2	EQU	$97
__local_f14__3	EQU	$98
__local_f14__4	EQU	$99
__local_f14__5	EQU	$9A
__local_mix__0	EQU	$9D
__local_mix__1	EQU	$9E
__local_mix__2	EQU	$9F
__local_mix__3	EQU	$A0

; @zp-link-meta-begin
; def f14 params=__zpabi_f14__p15_0,__zpabi_f14__p15_1,__zpabi_f14__p16_0,__zpabi_f14__p16_1,__zpabi_f14__p17_0,__zpabi_f14__p17_1 locals=__local_f14__w20,__local_f14__0,__local_f14__p16_1,__local_f14__p16_0,__local_f14__1,__local_f14__p15_1,__local_f14__p15_0,__local_f14__2,__local_f14__3,__local_f14__4,__local_f14__5,__local_f14__6,__local_f14__7 indirect=false in_cycle=false param_regs=A,X,,,,
; def f21 params=__zpabi_f21__pp22_0,__zpabi_f21__pp22_1 locals=__local_f21__w26,__local_f21__0,__local_f21__1,__local_f21__2,__local_f21__3,__local_f21__l23,__local_f21__pp22,__local_f21__4 indirect=false in_cycle=false param_regs=A,X
; def f8 params=__zpabi_f8__p9,__zpabi_f8__p10_0,__zpabi_f8__p10_1,__zpabi_f8__p10_2,__zpabi_f8__p10_3 locals=__local_f8__l11,__local_f8__0 indirect=false in_cycle=false param_regs=A,,,,
; def fin0 params= locals=__local_fin0__0,__local_fin0__1 indirect=false in_cycle=false
; def fin1 params= locals=__local_fin1__0,__local_fin1__1 indirect=false in_cycle=false
; def fin2 params= locals=__local_fin2__0,__local_fin2__1,__local_fin2__2,__local_fin2__3 indirect=false in_cycle=false
; def main params= locals=__local_main__0 indirect=false in_cycle=false
; def mix params=__zpabi_mix__v_0,__zpabi_mix__v_1,__zpabi_mix__v_2,__zpabi_mix__v_3 locals=__local_mix__0,__local_mix__1,__local_mix__2,__local_mix__3 indirect=false in_cycle=false
; call f14 -> mix
; call f21 -> f14
; call f21 -> f8
; call f21 -> mix
; call f8 -> mix
; call fin0 -> mix
; call fin1 -> mix
; call fin2 -> mix
; call main -> fin0
; call main -> fin1
; call main -> fin2
; call main -> mix
; item mix section=.text local=false zero=false
; item fin0 section=.text local=false zero=false
; item fin0@run3 section=.text local=true zero=false
; item fin0@run2 section=.text local=true zero=false
; item fin0@run1 section=.text local=true zero=false
; item fin0@tail0 section=.text local=true zero=false
; item fin0@tail0@run4 section=.text local=true zero=false
; item fin1 section=.text local=false zero=false
; item fin1@run0 section=.text local=true zero=false
; item fin2 section=.text local=false zero=false
; item f8 section=.text local=false zero=false
; item f14 section=.text local=false zero=false
; item f21 section=.text local=false zero=false
; item main section=.text local=false zero=false
; item acc section=.bss local=false zero=true
; item ga5 section=.data local=false zero=false
; item gs6 section=.data local=false zero=false
; item __zp_init section=.text local=false zero=false
; zp z3 size=1 at=auto def=true init=$FF
; zp z4 size=2 at=auto def=true init=$FF,$00
; zp g1 size=1 at=auto def=true init=$91 weight=23
; zp gp7 size=2 at=auto def=true init=<z4,>z4 weight=22
; zp @13.s25 size=1 at=auto def=true init=$01 weight=9 local=true
; zp @9.s19 size=1 at=auto def=true init=$00 weight=8 local=true
; zp g2 size=1 at=auto def=true init=$80 weight=5
; runtime hargs_size=32 hargs_need=11
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
   STA   __zpabi_mix__v_0
   BMI   .sx_neg@0
   LDA   #$00
   BEQ   .sx_done@1
.sx_neg@0:
   LDA   #$FF
.sx_done@1:
   JSR   fin0@run3
   LDA   g2
   STA   __zpabi_mix__v_0
   BMI   .sx_neg@2
   LDA   #$00
   BEQ   .sx_done@3
.sx_neg@2:
   LDA   #$FF
.sx_done@3:
   JSR   fin0@run3
   LDA   z3
   BMI   .sx_neg@4
   LDA   #$00
   BEQ   .sx_done@5
.sx_neg@4:
   LDA   #$FF
.sx_done@5:
   JSR   fin0@run2
   LDA   z4
   STA   __zpabi_mix__v_0
   LDA   z4+1
   JMP   fin0@tail0

fin0@run3:
   SUBROUTINE

   JSR   fin0@run1
   JMP   mix

fin0@run2:
   SUBROUTINE

   JSR   fin0@run1
   LDA   z3
   STA   __zpabi_mix__v_0
   JMP   mix

fin0@run1:
   SUBROUTINE

   STA   __zpabi_mix__v_1
   STA   __zpabi_mix__v_2
   STA   __zpabi_mix__v_3
   RTS

fin0@tail0:
   SUBROUTINE

   JSR   fin0@tail0@run4
   JMP   mix

fin0@tail0@run4:
   SUBROUTINE

   STA   __zpabi_mix__v_1
   LDA   #$00
   STA   __zpabi_mix__v_2
   STA   __zpabi_mix__v_3
   RTS

fin1:
   SUBROUTINE

   LDA   ga5
   STA   __zpabi_mix__v_0
   LDA   ga5+1
   JSR   fin1@run0
   LDA   ga5+2
   STA   __zpabi_mix__v_0
   LDA   ga5+3
   JSR   fin1@run0
   LDA   ga5+4
   STA   __zpabi_mix__v_0
   LDA   ga5+5
   JSR   fin1@run0
   LDA   ga5+6
   STA   __zpabi_mix__v_0
   LDA   ga5+7
   JMP   fin0@tail0

fin1@run0:
   SUBROUTINE

   JSR   fin0@tail0@run4
   JMP   mix

fin2:
   SUBROUTINE

   LDA   gs6+1
   STA   __zpabi_mix__v_0
   LDA   gs6+2
   STA   __zpabi_mix__v_1
   LDA   gs6+3
   STA   __zpabi_mix__v_2
   LDA   gs6+4
   STA   __zpabi_mix__v_3
   JSR   mix
   LDA   gs6+10
   BMI   .sx_neg@6
   LDA   #$00
   BEQ   .sx_done@7
.sx_neg@6:
   LDA   #$FF
.sx_done@7:
   STA   __zpabi_mix__v_2
   STA   __zpabi_mix__v_3
   LDA   gs6+9
   STA   __zpabi_mix__v_0
   LDA   gs6+10
   STA   __zpabi_mix__v_1
   JMP   mix

f8:
   SUBROUTINE

   LDA   #$FF
   STA   __local_f8__l11
   LDA   gs6+9
   STA   __local_f8__0
   LDA   gs6+10
   STA   __local_f8__l11
   LDA   __local_f8__0
   SEC
   SBC   #$01
   STA   gs6+9
   LDA   __local_f8__l11
   SBC   #$00
   STA   gs6+10
   LDA   #$7F
   AND   __local_f8__0
   STA   __local_f8__0
   LDA   #$00
   AND   __local_f8__l11
   STA   __local_f8__l11
   LDA   __local_f8__0
   LDY   #$00
   STA   (gp7),Y
   LDA   __local_f8__l11
   INY
   STA   (gp7),Y
   LDA   z3
   ORA   #$FE
   STA   __zpabi_mix__v_0
   LDA   #$FF
   JSR   fin1@run0
   LDA   #$00
   AND   ga5+4
   STA   __zpabi_mix__v_0
   LDA   #$00
   AND   ga5+5
   JSR   fin1@run0
   LDA   g2
   STA   __local_f8__l11
   BMI   .sx_neg@10
   LDA   #$00
   BEQ   .sx_done@11
.sx_neg@10:
   LDA   #$FF
.sx_done@11:
   TAX
   LDA   __local_f8__l11
   RTS

f14:
   SUBROUTINE

   STA   __local_f14__p16_0
   STX   __local_f14__p16_1
   LDA   ga5+2
   STA   __local_f14__0
   LDA   ga5+3
   STA   __local_f14__w20
   LDA   #$7F
   ORA   __local_f14__0
   STA   ga5+4
   LDA   __local_f14__w20
   STA   ga5+5
   LDA   g1
   CLC
   ADC   #$81
   STA   __local_f14__w20
   STA   g1
   BMI   .sx_neg@12
   LDA   #$00
   BEQ   .sx_done@13
.sx_neg@12:
   LDA   #$FF
.sx_done@13:
   JSR   fin0@run1
   LDA   __local_f14__w20
   STA   __zpabi_mix__v_0
   JSR   mix
   LDA   __local_f14__p16_0
   ASL
   STA   __local_f14__w20
   LDA   __local_f14__p16_1
   ROL
   STA   __local_f14__0
   LDA   __local_f14__w20
   CLC
   ADC   __local_f14__p16_0
   STA   __local_f14__p15_0
   LDA   __local_f14__0
   ADC   __local_f14__p16_1
   STA   __local_f14__p15_1
   ORA   __local_f14__p15_0
   BNE   .lb_skip@1
   JMP   .if_else@1
.lb_skip@1:
   LDA   z3
   CLC
   ADC   #$05
   STA   z3
   AND   gs6+1
   STA   g1
   LDA   #$00
   STA   __local_f14__w20
   LDA   __zpabi_f14__p16_0
   STA   __local_f14__p16_0
   LDA   __zpabi_f14__p16_1
   STA   __local_f14__p16_1
.loop@0_continue:
   LDA   __local_f14__w20
   CLC
   ADC   #$01
   STA   __local_f14__1
   LDA   __local_f14__w20
   CMP   #$03
   BCC   .lb_skip@0
   JMP   .loop@0_break
.lb_skip@0:
   INC   __local_f14__p16_0
   BNE   .inc_done@0
   INC   __local_f14__p16_1
.inc_done@0:
   LDA   @9.s19
   LDY   #$00
   LDA   (gp7),Y
   STA   __local_f14__w20
   INY
   LDA   (gp7),Y
   STA   __local_f14__0
   INC   __local_f14__w20
   BNE   .inc_done@1
   INC   __local_f14__0
.inc_done@1:
   LDA   __local_f14__w20
   LDY   #$00
   STA   (gp7),Y
   LDA   __local_f14__0
   INY
   STA   (gp7),Y
   LDA   g1
   SEC
   SBC   #$01
   STA   g1
   STA   gs6+9
   BMI   .sx_neg@14
   LDA   #$00
   BEQ   .sx_done@15
.sx_neg@14:
   LDA   #$FF
.sx_done@15:
   STA   gs6+10
   LDA   __local_f14__p15_0
   BNE   .carry_inc@0
   DEC   __local_f14__p15_1
.carry_inc@0:
   DEC   __local_f14__p15_0
   LDA   gs6+9
   STA   __local_f14__0
   LDA   gs6+10
   STA   __local_f14__w20
   LDA   __local_f14__p15_0
   CLC
   ADC   __local_f14__0
   STA   __zpabi_mix__v_0
   LDA   __local_f14__p15_1
   ADC   __local_f14__w20
   JSR   fin1@run0
   LDA   __local_f14__1
   STA   __local_f14__w20
   JMP   .loop@0_continue
.loop@0_break:
   LDA   gs6+1
   STA   __local_f14__1
   LDA   gs6+2
   STA   __local_f14__p15_1
   LDA   gs6+3
   STA   __local_f14__p15_0
   LDA   gs6+4
   STA   __local_f14__2
   LDA   gs6+5
   STA   __local_f14__3
   LDA   gs6+6
   STA   __local_f14__4
   LDA   gs6+7
   STA   __local_f14__5
   LDA   gs6+8
   ASL   __local_f14__1
   ROL   __local_f14__p15_1
   ROL   __local_f14__p15_0
   ROL   __local_f14__2
   ROL   __local_f14__3
   ROL   __local_f14__4
   ROL   __local_f14__5
   ROL
   LDX   __local_f14__1
   STX   gs6+1
   LDX   __local_f14__p15_1
   STX   gs6+2
   LDX   __local_f14__p15_0
   STX   gs6+3
   LDX   __local_f14__2
   STX   gs6+4
   LDX   __local_f14__3
   STX   gs6+5
   LDX   __local_f14__4
   STX   gs6+6
   LDX   __local_f14__5
   STX   gs6+7
   STA   gs6+8
   LDA   __local_f14__1
   LDY   #$00
   STA   (gp7),Y
   LDA   __local_f14__p15_1
   INY
   STA   (gp7),Y
   JMP   .if_end@0
.if_else@1:
   LDA   __zpabi_f14__p16_1
   BMI   .sx_neg@16
   LDA   #$00
   BEQ   .sx_done@17
.sx_neg@16:
   LDA   #$FF
.sx_done@17:
   STA   __zpabi_mix__v_2
   STA   __zpabi_mix__v_3
   LDA   __zpabi_f14__p16_0
   CLC
   ADC   #$01
   STA   __local_f14__p16_0
   LDA   __zpabi_f14__p16_1
   ADC   #$00
   STA   __local_f14__p16_1
   LDA   __zpabi_f14__p16_0
   STA   __zpabi_mix__v_0
   LDA   __zpabi_f14__p16_1
   STA   __zpabi_mix__v_1
   JSR   mix
.if_end@0:
   LDA   g2
   STA   __local_f14__0
   BMI   .sx_neg@18
   LDA   #$00
   BEQ   .sx_done@19
.sx_neg@18:
   LDA   #$FF
.sx_done@19:
   STA   __local_f14__w20
   LDA   __local_f14__0
   SEC
   SBC   __local_f14__p16_0
   LDA   __local_f14__w20
   SBC   __local_f14__p16_1
   BVC   .cmp_novf@20
   EOR   #$80
.cmp_novf@20:
   ASL
   LDA   #$00
   ROL
   EOR   #$01
   RTS

f21:
   SUBROUTINE

   STA   __local_f21__pp22
   STX   __local_f21__4
   LDA   #$00
   STA   __local_f21__l23
   LDA   gs6+9
   STA   HARGS
   LDA   gs6+10
   STA   HARGS+1
   LDA   #$04
   STA   HARGS+2
   LDA   #$00
   STA   HARGS+3
   JSR   sdivmod16
   LDA   HARGS+4
   STA   @13.s25
   LDA   #$00
   STA   __local_f21__w26
.loop@1_continue:
   CLC
   ADC   #$01
   STA   __local_f21__3
   LDA   __local_f21__w26
   BNE   .loop@1_break
   LDA   @13.s25
   BEQ   .and_false@3
   LDY   #$01
   LDA   (__local_f21__pp22),Y
   STA   __local_f21__1
   INY
   LDA   (__local_f21__pp22),Y
   STA   __local_f21__0
   INY
   LDA   (__local_f21__pp22),Y
   STA   __local_f21__w26
   LDY   #$00
   LDA   (__local_f21__pp22),Y
   ORA   __local_f21__1
   ORA   __local_f21__0
   ORA   __local_f21__w26
   BEQ   .and_false@3
   LDA   #$01
   STA   __local_f21__w26
   LDX   #$00
   BEQ   .and_end@4
.and_false@3:
   LDA   #$00
   STA   __local_f21__w26
   LDX   #$00
.and_end@4:
   LDA   __local_f21__l23
   STA   __zpabi_f14__p16_0
   LDA   #$00
   STA   __zpabi_f14__p16_1
   LDA   z4
   STA   __zpabi_f14__p17_0
   LDA   z4+1
   STA   __zpabi_f14__p17_1
   LDA   __local_f21__w26
   JSR   f14
   LDA   z3
   BMI   .sx_neg@21
   LDA   #$00
   BEQ   .sx_done@22
.sx_neg@21:
   LDA   #$FF
.sx_done@22:
   JSR   fin0@run2
   LDA   __local_f21__3
   STA   __local_f21__w26
   JMP   .loop@1_continue
.loop@1_break:
   LDY   #$00
   LDA   (gp7),Y
   STA   __local_f21__0
   LDA   g2
   BEQ   .lnot_true@23
   LDA   #$00
   BEQ   .lnot_end@24
.lnot_true@23:
   LDA   #$01
.lnot_end@24:
   STA   __local_f21__w26
   LDA   #$00
   BPL   .sx_done@26
   LDA   #$FF
.sx_done@26:
   STA   __zpabi_f8__p10_2
   STA   __zpabi_f8__p10_3
   LDA   __local_f21__w26
   STA   __zpabi_f8__p10_0
   LDA   #$00
   STA   __zpabi_f8__p10_1
   LDA   __local_f21__0
   JSR   f8
   LDA   z4
   STA   HARGS
   LDA   z4+1
   STA   HARGS+1
   LDA   #$05
   STA   HARGS+2
   LDA   #$00
   STA   HARGS+3
   JSR   udivmod16
   LDA   HARGS+4
   RTS

main:
   SUBROUTINE

   LDA   z4
   AND   #$02
   STA   __zpabi_mix__v_0
   LDA   #$00
   JSR   fin0@run3
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

ga5:
   DC.W  $8000
   DC.W  $0100
   DC.W  $0003
   DC.W  $007F

gs6:
   DS.B  1
   DC.L  $00000003
   DC.L  $00000000
   DS.B  2

__zp_init:
   SUBROUTINE

   LDA   #$00
   STA   z4+1
   STA   @9.s19
   LDA   #$91
   STA   g1
   LDA   #$80
   STA   g2
   LDA   #$FF
   STA   z3
   STA   z4
   LDA   #<z4
   STA   gp7
   LDA   #>z4
   STA   gp7+1
   LDA   #$01
   STA   @13.s25
   RTS
