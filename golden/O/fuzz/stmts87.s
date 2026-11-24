SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
g1	EQU	$26
gp6	EQU	$27
g2	EQU	$29
gp5	EQU	$2B
__zpabi_f11__p13_0	EQU	$80
__zpabi_f11__p13_1	EQU	$81
__zpabi_f11__p14_0	EQU	$82
__zpabi_f11__p14_1	EQU	$83
__zpabi_f11__pp15_0	EQU	$84
__zpabi_f11__pp15_1	EQU	$85
__zpabi_mix__v_0	EQU	$86
__zpabi_mix__v_1	EQU	$87
__zpabi_mix__v_2	EQU	$88
__zpabi_mix__v_3	EQU	$89
__local_main__0	EQU	$8A
__local_main__2	EQU	$8C
__local_main__l25	EQU	$8E
__local_f11__0	EQU	$90
__local_f11__1	EQU	$91
__local_f11__2	EQU	$92
__local_f11__3	EQU	$93
__local_f11__l18	EQU	$96
__local_f11__4	EQU	$97
__local_f11__l17	EQU	$98
__local_f11__5	EQU	$99
__local_f11__6	EQU	$9A
__local_f11__7	EQU	$9B
__local_f11__8	EQU	$9C
__local_f11__9	EQU	$9D
__local_f11__10	EQU	$9E
__local_f11__11	EQU	$9F
__local_f7__0	EQU	$A1
__local_f7__1	EQU	$A2
__local_f7__l9	EQU	$A3
__local_mix__0	EQU	$A4
__local_mix__1	EQU	$A5
__local_mix__2	EQU	$A6
__local_mix__3	EQU	$A7

; @zp-link-meta-begin
; def f11 params=__zpabi_f11__p12_0,__zpabi_f11__p12_1,__zpabi_f11__p13_0,__zpabi_f11__p13_1,__zpabi_f11__p14_0,__zpabi_f11__p14_1,__zpabi_f11__pp15_0,__zpabi_f11__pp15_1 locals=__local_f11__w21,__local_f11__0,__local_f11__1,__local_f11__2,__local_f11__3,__local_f11__p14_1,__local_f11__p14_0,__local_f11__l18,__local_f11__4,__local_f11__l17,__local_f11__5,__local_f11__6,__local_f11__7,__local_f11__8,__local_f11__9,__local_f11__10,__local_f11__11,__local_f11__12 indirect=false in_cycle=false param_regs=A,X,,,,,, changes=__zpabi_f11__p14_0,__zpabi_f11__p14_1
; def f7 params= locals=__local_f7__0,__local_f7__1,__local_f7__l9 indirect=false in_cycle=false
; def fin0 params= locals=__local_fin0__0,__local_fin0__1 indirect=false in_cycle=false
; def fin1 params= locals=__local_fin1__0,__local_fin1__1 indirect=false in_cycle=false
; def main params= locals=__local_main__0,__local_main__1,__local_main__2,__local_main__3,__local_main__l25 indirect=false in_cycle=false
; def mix params=__zpabi_mix__v_0,__zpabi_mix__v_1,__zpabi_mix__v_2,__zpabi_mix__v_3 locals=__local_mix__0,__local_mix__1,__local_mix__2,__local_mix__3 indirect=false in_cycle=false
; call f11 -> f7
; call f11 -> mix
; call f7 -> mix
; call fin0 -> mix
; call fin1 -> mix
; call main -> f11
; call main -> f7
; call main -> fin0
; call main -> fin1
; call main -> mix
; item mix section=.text local=false zero=false
; item fin0 section=.text local=false zero=false
; item fin0@run2 section=.text local=true zero=false
; item fin0@run1 section=.text local=true zero=false
; item fin0@run0 section=.text local=true zero=false
; item fin1 section=.text local=false zero=false
; item f7 section=.text local=false zero=false
; item f11 section=.text local=false zero=false
; item main section=.text local=false zero=false
; item acc section=.bss local=false zero=true
; item ga3 section=.data local=false zero=false
; item gs4 section=.data local=false zero=false
; item __zp_init section=.text local=false zero=false
; zp g1 size=1 at=auto def=true init=$7F weight=9
; zp gp6 size=2 at=auto def=true init=<g2,>g2 weight=9
; zp g2 size=2 at=auto def=true init=$7F,$00 weight=8
; zp gp5 size=2 at=auto def=true init=<g1,>g1 weight=7
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
   JSR   fin0@run0
   JSR   mix
   LDA   g2+1
   BMI   .sx_neg@0
   LDA   #$00
   BEQ   .sx_done@1
.sx_neg@0:
   LDA   #$FF
.sx_done@1:
   JSR   fin0@run2
   LDA   g2
   STA   __zpabi_mix__v_0
   LDA   g2+1
   JSR   fin0@run1
   LDA   ga3+1
   BMI   .sx_neg@2
   LDA   #$00
   BEQ   .sx_done@3
.sx_neg@2:
   LDA   #$FF
.sx_done@3:
   JSR   fin0@run2
   LDA   ga3
   STA   __zpabi_mix__v_0
   LDA   ga3+1
   JSR   fin0@run1
   LDA   ga3+3
   BMI   .sx_neg@4
   LDA   #$00
   BEQ   .sx_done@5
.sx_neg@4:
   LDA   #$FF
.sx_done@5:
   JSR   fin0@run2
   LDA   ga3+2
   STA   __zpabi_mix__v_0
   LDA   ga3+3
   STA   __zpabi_mix__v_1
   JMP   mix

fin0@run2:
   SUBROUTINE

   STA   __zpabi_mix__v_2
   STA   __zpabi_mix__v_3
   RTS

fin0@run1:
   SUBROUTINE

   STA   __zpabi_mix__v_1
   JMP   mix

fin0@run0:
   SUBROUTINE

   STA   __zpabi_mix__v_0
   LDA   #$00
   STA   __zpabi_mix__v_1
   JSR   fin0@run2
   RTS

fin1:
   SUBROUTINE

   LDA   ga3+5
   BMI   .sx_neg@6
   LDA   #$00
   BEQ   .sx_done@7
.sx_neg@6:
   LDA   #$FF
.sx_done@7:
   JSR   fin0@run2
   LDA   ga3+4
   STA   __zpabi_mix__v_0
   LDA   ga3+5
   JSR   fin0@run1
   LDA   ga3+7
   BMI   .sx_neg@8
   LDA   #$00
   BEQ   .sx_done@9
.sx_neg@8:
   LDA   #$FF
.sx_done@9:
   JSR   fin0@run2
   LDA   ga3+6
   STA   __zpabi_mix__v_0
   LDA   ga3+7
   JSR   fin0@run1
   LDA   gs4+2
   BMI   .sx_neg@10
   LDA   #$00
   BEQ   .sx_done@11
.sx_neg@10:
   LDA   #$FF
.sx_done@11:
   JSR   fin0@run2
   LDA   gs4+1
   STA   __zpabi_mix__v_0
   LDA   gs4+2
   JSR   fin0@run1
   LDA   gs4+3
   JSR   fin0@run0
   JMP   mix

f7:
   SUBROUTINE

   LDA   #$FF
   STA   __local_f7__l9
   LDA   gs4+1
   ORA   #$81
   STA   gs4+1
   LDY   #$00
   LDA   (gp5),Y
   JSR   fin0@run0
   JSR   mix
   LDA   gs4+3
   ORA   #$5A
   STA   gs4+3
   LDY   #$00
   STA   (gp5),Y
   LDA   __local_f7__l9
   STA   __local_f7__0
   BMI   .sx_neg@12
   LDA   #$00
   BEQ   .sx_done@13
.sx_neg@12:
   LDA   #$FF
.sx_done@13:
   STA   __local_f7__1
   LDA   __local_f7__0
   AND   g2
   STA   __local_f7__0
   LDA   __local_f7__1
   AND   g2+1
   TAX
   LDA   __local_f7__0
   RTS

f11:
   SUBROUTINE

   LDA   #$01
   STA   __local_f11__l17
   LDA   #$00
   STA   __local_f11__5
   STA   __local_f11__6
   STA   __local_f11__7
   STA   __local_f11__8
   STA   __local_f11__9
   STA   __local_f11__10
   STA   __local_f11__11
   LDA   #$01
   STA   __local_f11__l18
   LDA   #$00
   STA   __local_f11__4
   LDA   __zpabi_f11__p14_0
   ORA   __zpabi_f11__p14_1
   BEQ   .lnot_true@14
   LDA   #$00
   BEQ   .lnot_end@15
.lnot_true@14:
   LDA   #$01
.lnot_end@15:
   STA   __zpabi_mix__v_1
   LDA   __zpabi_f11__p14_0
   ASL
   STA   __local_f11__1
   LDA   __zpabi_f11__p14_1
   ROL
   STA   __local_f11__2
   LDA   __zpabi_mix__v_1
   CLC
   ADC   __local_f11__1
   STA   __local_f11__0
   LDA   #$00
   ADC   __local_f11__2
   STA   __zpabi_mix__v_1
   BMI   .sx_neg@16
   LDA   #$00
   BEQ   .sx_done@17
.sx_neg@16:
   LDA   #$FF
.sx_done@17:
   JSR   fin0@run2
   LDA   __local_f11__0
   STA   __zpabi_mix__v_0
   JSR   mix
   LDY   #$00
   LDA   (__zpabi_f11__pp15_0),Y
   SEC
   SBC   gs4+1
   LDA   #$00
   SBC   gs4+2
   BVC   .cmp_novf@18
   EOR   #$80
.cmp_novf@18:
   ASL
   LDA   #$00
   ROL
   STA   __zpabi_mix__v_1
   LDA   #$00
   BPL   .sx_done@20
   LDA   #$FF
.sx_done@20:
   JSR   fin0@run2
   LDA   __zpabi_mix__v_1
   STA   __zpabi_mix__v_0
   LDA   #$00
   STA   __zpabi_mix__v_1
   JSR   mix
   LDA   #$00
   STA   __zpabi_mix__v_1
.loop@0_continue:
   CLC
   ADC   #$01
   STA   __local_f11__3
   LDA   __zpabi_mix__v_1
   BEQ   .lb_skip@0
   JMP   .loop@0_break
.lb_skip@0:
   LDA   __zpabi_f11__p14_1
   LSR
   LDA   __zpabi_f11__p14_0
   ROR
   STA   __zpabi_f11__p14_1
   LDA   #$00
   ROR
   STA   __zpabi_f11__p14_0
   LDA   #$00
   STA   __zpabi_mix__v_1
   JMP   .loop@1_continue
.f11@rot@0:
   JSR   f7
   STA   __zpabi_mix__v_0
   STX   __zpabi_mix__v_1
   LDA   #$00
   STA   __zpabi_mix__v_2
   STA   __zpabi_mix__v_3
   JSR   mix
   LDA   __local_f11__0
   STA   __zpabi_mix__v_1
.loop@1_continue:
   CLC
   ADC   #$01
   STA   __local_f11__0
   LDA   __zpabi_mix__v_1
   CMP   #$04
   BCC   .f11@rot@0
   LDA   __zpabi_f11__p14_0
   STA   HARGS
   LDA   __zpabi_f11__p14_1
   STA   HARGS+1
   LDA   #$02
   STA   HARGS+2
   LDA   #$00
   STA   HARGS+3
   JSR   sdivmod16
   LDA   HARGS+4
   SEC
   SBC   #$02
   AND   #$07
   CMP   #$01
   BEQ   .case@3
   CMP   #$03
   BEQ   .case@4
   CMP   #$04
   BEQ   .case@5
   JMP   .switch@2_break
.case@3:
.case@4:
   LDA   __local_f11__l17
   STA   __zpabi_mix__v_0
   LDA   __local_f11__5
   STA   __zpabi_mix__v_1
   LDA   __local_f11__6
   STA   __zpabi_mix__v_2
   LDA   __local_f11__7
   STA   __zpabi_mix__v_3
   LDA   __local_f11__8
   LDA   __local_f11__9
   LDA   __local_f11__10
   LDA   __local_f11__11
   JSR   mix
   LDA   __local_f11__3
   ASL
   ASL
   STA   g1
.case@5:
   LDA   __local_f11__l18
   STA   __local_f11__1
   LDA   __local_f11__4
   ASL   __local_f11__1
   ROL
   ASL   __local_f11__1
   ROL
   ASL   __local_f11__1
   ROL
   ASL   __local_f11__1
   ROL
   LDX   __local_f11__1
   STX   __local_f11__l18
   STA   __local_f11__4
   LDA   __local_f11__l17
   STA   __local_f11__0
   LDA   __local_f11__5
   LDA   __local_f11__6
   LDA   __local_f11__7
   LDA   __local_f11__8
   LDA   __local_f11__9
   LDA   __local_f11__10
   LDA   __local_f11__11
   LDA   __local_f11__1
   CLC
   ADC   __local_f11__0
   AND   #$07
   BEQ   .case@7
   CMP   #$03
   BEQ   .switch@6_break
   CMP   #$05
   BEQ   .case@9
   CMP   #$06
   BEQ   .case@10
   BNE   .default@11
.case@7:
   LDY   #$01
   LDA   (gp6),Y
   DEY
   ORA   (gp6),Y
   BEQ   .and_false@0
   LDA   __local_f11__3
   BEQ   .and_false@0
   LDA   #$01
   STA   __local_f11__0
   LDA   #$00
   BEQ   .and_end@1
.and_false@0:
   LDA   #$00
   STA   __local_f11__0
.and_end@1:
   ORA   #$00
   BPL   .sx_done@22
   LDA   #$FF
.sx_done@22:
   JSR   fin0@run2
   LDA   __local_f11__0
   STA   __zpabi_mix__v_0
   LDA   #$00
   JSR   fin0@run1
   JMP   .switch@6_break
.case@9:
   LDA   ga3+4
   CLC
   ADC   #$05
   STA   ga3+4
   BCC   .carry_inc@1
   INC   ga3+5
.carry_inc@1:
.case@10:
.default@11:
.switch@6_break:
.switch@2_break:
   LDA   __local_f11__3
   STA   __zpabi_mix__v_1
   JMP   .loop@0_continue
.loop@0_break:
   LDA   #$00
   RTS

main:
   SUBROUTINE

   LDA   #$00
   STA   __local_main__l25
   INC   g2
   BNE   .inc_done@0
   INC   g2+1
.inc_done@0:
   LDA   gs4+1
   AND   #$07
   CMP   #$01
   BEQ   .case@13
   CMP   #$03
   BEQ   .case@14
   CMP   #$07
   BEQ   .case@15
   BNE   .default@16
.case@13:
   LDY   #$00
   LDA   (gp5),Y
   CLC
   ADC   #$01
   STA   __local_main__0
   STA   (gp5),Y
   LDA   g2
   ORA   __local_main__0
   ORA   g2+1
   BEQ   .if_end@2
   LDA   (gp6),Y
   EOR   gs4+1
   STA   __local_main__0
   INY
   LDA   (gp6),Y
   EOR   gs4+2
   LSR   __local_main__0
   LSR   __local_main__0
   ASL
   ASL
   ASL
   ASL
   ASL
   ASL
   ORA   __local_main__0
   DEY
   STA   (gp5),Y
.if_end@2:
   LDA   #$5A
   AND   ga3
   STA   g2
   LDA   #$00
   AND   ga3+1
   STA   g2+1
.case@14:
   INC   g2
   BNE   .inc_done@1
   INC   g2+1
.inc_done@1:
.case@15:
.default@16:
   LDA   __local_main__l25
   STA   __local_main__2
   LDY   #$00
   LDA   (gp5),Y
   STA   __zpabi_f11__p13_0
   LDA   #$00
   STA   __zpabi_f11__p13_1
   STA   __zpabi_f11__p14_0
   STA   __zpabi_f11__p14_1
   LDA   #<__local_main__l25
   STA   __zpabi_f11__pp15_0
   LDA   #>__local_main__l25
   STA   __zpabi_f11__pp15_1
   LDX   #$00
   LDA   __local_main__2
   JSR   f11
   JSR   f7
   STA   __zpabi_mix__v_0
   STX   __zpabi_mix__v_1
   LDA   #$00
   STA   __zpabi_mix__v_2
   STA   __zpabi_mix__v_3
   JSR   mix
   JSR   fin0
   JSR   fin1
   LDA   acc+1
   AND   #$7F
   TAX
   LDA   acc
   RTS

acc:
   DS.B  2

ga3:
   DC.W  $007F
   DC.W  $FFFF
   DC.W  $0002
   DC.W  $0080

gs4:
   DS.B  1
   DC.W  $FFFF
   DC.B  $FF

__zp_init:
   SUBROUTINE

   LDA   #$00
   STA   g2+1
   LDA   #$7F
   STA   g1
   STA   g2
   LDA   #<g1
   STA   gp5
   LDA   #>g1
   STA   gp5+1
   LDA   #<g2
   STA   gp6
   LDA   #>g2
   STA   gp6+1
   RTS
