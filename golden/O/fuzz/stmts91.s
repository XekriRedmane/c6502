SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
g2	EQU	$26
gp5	EQU	$27
@3.s12	EQU	$29
__zpabi_f24__p26_0	EQU	$80
__zpabi_f24__p26_1	EQU	$81
__zpabi_f15__p18_0	EQU	$82
__zpabi_f15__p18_1	EQU	$83
__zpabi_f15__pp19_0	EQU	$84
__zpabi_f15__pp19_1	EQU	$85
__zpabi_f6__p7_0	EQU	$86
__zpabi_f6__p7_1	EQU	$87
__zpabi_f6__p7_2	EQU	$88
__zpabi_f6__p7_3	EQU	$89
__zpabi_f6__p7_4	EQU	$8A
__zpabi_f6__p7_5	EQU	$8B
__zpabi_f6__p7_6	EQU	$8C
__zpabi_f6__p7_7	EQU	$8D
__zpabi_f6__pp8_0	EQU	$8E
__zpabi_f6__pp8_1	EQU	$8F
__zpabi_mix__v_0	EQU	$90
__zpabi_mix__v_1	EQU	$91
__zpabi_mix__v_2	EQU	$92
__zpabi_mix__v_3	EQU	$93
__local_f24__l27	EQU	$96
__local_f24__2	EQU	$97
__local_f24__w30	EQU	$98
__local_f24__l28	EQU	$99
__local_f24__l29	EQU	$9B
__local_f15__l20	EQU	$A3
__local_f15__0	EQU	$A4
__local_f15__1	EQU	$A5
__local_f15__2	EQU	$A6
__local_f15__3	EQU	$A7
__local_f15__4	EQU	$A8
__local_f15__5	EQU	$A9
__local_f15__6	EQU	$AA
__local_f15__l22	EQU	$AB
__local_f6__1	EQU	$B0
__local_f6__i14	EQU	$B1
__local_f6__2	EQU	$B2
__local_f6__w13	EQU	$BB
__local_f6__l11	EQU	$BC
__local_f6__3	EQU	$BD
__local_f6__l9	EQU	$BE
__local_f6__4	EQU	$BF
__local_mix__0	EQU	$C1
__local_mix__1	EQU	$C2
__local_mix__2	EQU	$C3
__local_mix__3	EQU	$C4

; @zp-link-meta-begin
; def f15 params=__zpabi_f15__p16,__zpabi_f15__p17,__zpabi_f15__p18_0,__zpabi_f15__p18_1,__zpabi_f15__pp19_0,__zpabi_f15__pp19_1 locals=__local_f15__l20,__local_f15__0,__local_f15__1,__local_f15__2,__local_f15__3,__local_f15__4,__local_f15__5,__local_f15__6,__local_f15__l22,__local_f15__8,__local_f15__9,__local_f15__10 indirect=false in_cycle=false param_regs=A,X,,,,
; def f24 params=__zpabi_f24__p25,__zpabi_f24__p26_0,__zpabi_f24__p26_1 locals=__local_f24__0,__local_f24__1,__local_f24__l27,__local_f24__2,__local_f24__w30,__local_f24__l28,__local_f24__4,__local_f24__l29,__local_f24__6,__local_f24__7,__local_f24__8,__local_f24__9,__local_f24__10,__local_f24__11,__local_f24__12 indirect=false in_cycle=false param_regs=A,,
; def f6 params=__zpabi_f6__p7_0,__zpabi_f6__p7_1,__zpabi_f6__p7_2,__zpabi_f6__p7_3,__zpabi_f6__p7_4,__zpabi_f6__p7_5,__zpabi_f6__p7_6,__zpabi_f6__p7_7,__zpabi_f6__pp8_0,__zpabi_f6__pp8_1 locals=__local_f6__0,__local_f6__1,__local_f6__i14,__local_f6__2,__local_f6__p7_7,__local_f6__p7_6,__local_f6__p7_5,__local_f6__p7_4,__local_f6__p7_3,__local_f6__p7_2,__local_f6__p7_1,__local_f6__p7_0,__local_f6__w13,__local_f6__l11,__local_f6__3,__local_f6__l9,__local_f6__4,__local_f6__5 indirect=false in_cycle=false changes=__zpabi_f6__p7_0,__zpabi_f6__p7_1,__zpabi_f6__p7_2,__zpabi_f6__p7_3,__zpabi_f6__p7_4,__zpabi_f6__p7_5,__zpabi_f6__p7_6,__zpabi_f6__p7_7
; def fin0 params= locals=__local_fin0__0 indirect=false in_cycle=false
; def fin1 params= locals=__local_fin1__0 indirect=false in_cycle=false
; def main params= locals=__local_main__0 indirect=false in_cycle=false
; def mix params=__zpabi_mix__v_0,__zpabi_mix__v_1,__zpabi_mix__v_2,__zpabi_mix__v_3 locals=__local_mix__0,__local_mix__1,__local_mix__2,__local_mix__3 indirect=false in_cycle=false
; call f15 -> f6
; call f15 -> mix
; call f24 -> f15
; call f24 -> f6
; call f24 -> mix
; call f6 -> mix
; call fin0 -> mix
; call fin1 -> mix
; call main -> fin0
; call main -> fin1
; call main -> mix
; item mix section=.text local=false zero=false
; item fin0 section=.text local=false zero=false
; item fin0@run5 section=.text local=true zero=false
; item fin0@run4 section=.text local=true zero=false
; item fin0@run0 section=.text local=true zero=false
; item fin0@run0@run6 section=.text local=true zero=false
; item fin0@tail0 section=.text local=true zero=false
; item fin1 section=.text local=false zero=false
; item f6 section=.text local=false zero=false
; item f6@run3 section=.text local=true zero=false
; item f15 section=.text local=false zero=false
; item f15@run2 section=.text local=true zero=false
; item f15@run1 section=.text local=true zero=false
; item f24 section=.text local=false zero=false
; item main section=.text local=false zero=false
; item acc section=.bss local=false zero=true
; item g1 section=.data local=false zero=false
; item ga3 section=.data local=false zero=false
; item gs4 section=.data local=false zero=false
; item @13.s23 section=.data local=true zero=false
; item __zp_init section=.text local=false zero=false
; zp g2 size=1 at=auto def=true init=$7F weight=84
; zp gp5 size=2 at=auto def=true init=<g2,>g2 weight=17
; zp @3.s12 size=2 at=auto def=true init=$7F,$00 weight=8 local=true
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
   BMI   .sx_neg@0
   LDA   #$00
   BEQ   .sx_done@1
.sx_neg@0:
   LDA   #$FF
.sx_done@1:
   STA   __zpabi_mix__v_1
   JSR   fin0@run5
   LDA   g1
   STA   __zpabi_mix__v_0
   JSR   mix
   LDA   g2
   JSR   fin0@run4
   LDA   ga3
   JSR   fin0@run4
   LDA   ga3+1
   JMP   fin0@tail0

fin0@run5:
   SUBROUTINE

   STA   __zpabi_mix__v_2
   STA   __zpabi_mix__v_3
   RTS

fin0@run4:
   SUBROUTINE

   STA   __zpabi_mix__v_0
   JMP   fin0@run0

fin0@run0:
   SUBROUTINE

   JSR   fin0@run0@run6
   JMP   mix

fin0@run0@run6:
   SUBROUTINE

   LDA   #$00
   STA   __zpabi_mix__v_1
   JMP   fin0@run5

fin0@tail0:
   SUBROUTINE

   STA   __zpabi_mix__v_0
   JSR   fin0@run0@run6
   JMP   mix

fin1:
   SUBROUTINE

   LDA   ga3+2
   JSR   fin0@run4
   LDA   ga3+3
   JSR   fin0@run4
   LDA   gs4+1
   BMI   .sx_neg@2
   LDA   #$00
   BEQ   .sx_done@3
.sx_neg@2:
   LDA   #$FF
.sx_done@3:
   STA   __zpabi_mix__v_1
   JSR   fin0@run5
   LDA   gs4+1
   STA   __zpabi_mix__v_0
   JSR   mix
   LDA   gs4+2
   JMP   fin0@tail0

f6:
   SUBROUTINE

   LDA   #$FF
   STA   __local_f6__l9
   STA   __local_f6__4
   LDA   #$00
   STA   __local_f6__l11
   STA   __local_f6__3
   STA   __local_f6__w13
.loop@0_start:
   LDY   #$00
   LDA   (gp5),Y
   STA   ga3+3
   LDA   (gp5),Y
   ORA   #$01
   AND   #$07
   CMP   #$01
   BEQ   .case@2
   CMP   #$05
   BNE   .lb_skip@2
   JMP   .case@4
.lb_skip@2:
   JMP   .switch@1_break
.case@2:
   LDA   #$01
   STA   __local_f6__i14
.f6@rot@0:
   INC   __zpabi_f6__p7_0
   BNE   .inc_done@0
   INC   __zpabi_f6__p7_1
   BNE   .inc_done@0
   INC   __zpabi_f6__p7_2
   BNE   .inc_done@0
   INC   __zpabi_f6__p7_3
   BNE   .inc_done@0
   INC   __zpabi_f6__p7_4
   BNE   .inc_done@0
   INC   __zpabi_f6__p7_5
   BNE   .inc_done@0
   INC   __zpabi_f6__p7_6
   BNE   .inc_done@0
   INC   __zpabi_f6__p7_7
.inc_done@0:
   LDA   __zpabi_f6__p7_0
   STA   g2
   LDA   gs4+2
   STA   __local_f6__1
   CLC
   ADC   #$01
   STA   gs4+2
   LDY   #$00
   LDA   (__zpabi_f6__pp8_0),Y
   STA   __zpabi_mix__v_0
   INY
   LDA   (__zpabi_f6__pp8_0),Y
   STA   __zpabi_mix__v_1
   LDA   __local_f6__1
   ORA   __zpabi_mix__v_0
   STA   __zpabi_mix__v_0
   JSR   f6@run3
   DEC   __local_f6__i14
   BNE   .f6@rot@0
   LDY   #$00
   LDA   (__zpabi_f6__pp8_0),Y
   STA   __local_f6__2
   INY
   LDA   (__zpabi_f6__pp8_0),Y
   STA   __local_f6__i14
   LDA   __local_f6__2
   CLC
   ADC   #$01
   STA   __local_f6__1
   LDA   __local_f6__i14
   ADC   #$00
   STA   (__zpabi_f6__pp8_0),Y
   LDA   __local_f6__1
   DEY
   STA   (__zpabi_f6__pp8_0),Y
   LDA   __local_f6__2
   CLC
   ADC   __local_f6__w13
   STA   __zpabi_mix__v_0
   LDA   __local_f6__i14
   ADC   #$00
   STA   __zpabi_mix__v_1
   JSR   f6@run3
   JMP   .switch@1_break
.case@4:
   LDA   __local_f6__l11
   STA   __local_f6__i14
   LDA   __local_f6__3
   ASL   __local_f6__i14
   ROL
   LDX   __local_f6__i14
   STX   __local_f6__l11
   STA   __local_f6__3
   LDA   __local_f6__i14
   SEC
   SBC   #$01
   AND   #$07
   BEQ   .switch@5_break
   CMP   #$02
   BEQ   .switch@5_break
   CMP   #$04
   BNE   .lb_skip@1
   JMP   .loop@0_continue
.lb_skip@1:
   CMP   #$06
.switch@5_break:
   LDA   gs4+1
   STA   __local_f6__1
   BMI   .sx_neg@4
   LDA   #$00
   BEQ   .sx_done@5
.sx_neg@4:
   LDA   #$FF
.sx_done@5:
   STA   __zpabi_mix__v_0
   LDA   #$7F
   SEC
   SBC   __local_f6__1
   LDA   #$00
   SBC   __zpabi_mix__v_0
   BVC   .cmp_novf@6
   EOR   #$80
.cmp_novf@6:
   ASL
   LDA   #$00
   ROL
   STA   __zpabi_mix__v_0
   LDA   #$65
   BNE   .thread@0
   LDA   #$9C
   CMP   __zpabi_mix__v_0
   BEQ   .cmp_true@7
.thread@0:
   LDA   #$00
   BEQ   .cmp_end@8
.cmp_true@7:
   LDA   #$01
.cmp_end@8:
   STA   __zpabi_mix__v_0
   LDA   #$00
   BPL   .sx_done@11
   LDA   #$FF
.sx_done@11:
   JSR   fin0@run5
   LDA   #$00
   STA   __zpabi_mix__v_1
   JSR   mix
.switch@1_break:
   LDA   __local_f6__l9
   STA   __zpabi_mix__v_0
   LDA   __local_f6__4
   LSR
   STA   __local_f6__1
   ROR   __zpabi_mix__v_0
   LDA   __zpabi_mix__v_0
   ORA   __local_f6__1
   BEQ   .if_end@0
   LDA   __local_f6__l9
   STA   __zpabi_mix__v_0
   LDA   __local_f6__4
   STA   __local_f6__2
   LDA   __zpabi_mix__v_0
   CLC
   ADC   #$01
   STA   __local_f6__i14
   BCC   .carry_inc@1
   INC   __local_f6__2
.carry_inc@1:
   STA   __local_f6__l9
   LDA   __local_f6__2
   STA   __local_f6__4
   LDA   __local_f6__i14
   EOR   @3.s12
   STA   __local_f6__1
   LDA   __local_f6__2
   EOR   @3.s12+1
   STA   __zpabi_mix__v_0
   LDA   __local_f6__1
   STA   __local_f6__l11
   LDA   __zpabi_mix__v_0
   STA   __local_f6__3
.if_end@0:
.loop@0_continue:
   INC   __local_f6__w13
   BNE   .lb_skip@0
   JMP   .loop@0_start
.lb_skip@0:
   LDA   gs4+1
   STA   __zpabi_mix__v_0
   BMI   .sx_neg@12
   LDA   #$00
   BEQ   .sx_done@13
.sx_neg@12:
   LDA   #$FF
.sx_done@13:
   TAX
   LDA   __zpabi_mix__v_0
   RTS

f6@run3:
   SUBROUTINE

   LDA   #$00
   STA   __zpabi_mix__v_2
   STA   __zpabi_mix__v_3
   JMP   mix

f15:
   SUBROUTINE

   LDA   #$00
   STA   __local_f15__l20
   STA   __local_f15__0
   LDA   #$01
   STA   __local_f15__1
   LDA   #$00
   STA   __local_f15__2
   STA   __local_f15__3
   STA   __local_f15__4
   STA   __local_f15__5
   STA   __local_f15__6
   LDA   #$80
   STA   __local_f15__l22
   LDA   #$00
   STA   __local_f15__l22+1
   LDA   @13.s23
   ORA   #$01
   STA   __local_f15__l20
   STA   @13.s23
   BMI   .sx_neg@14
   LDA   #$00
   BEQ   .sx_done@15
.sx_neg@14:
   LDA   #$FF
.sx_done@15:
   STA   __local_f15__0
   LDA   ga3+2
   ORA   __local_f15__l20
   STA   __local_f15__l20
   LDA   __local_f15__0
   BMI   .sx_neg@16
   LDA   #$00
   BEQ   .sx_done@17
.sx_neg@16:
   LDA   #$FF
.sx_done@17:
   JSR   fin0@run5
   LDA   __local_f15__l20
   STA   __zpabi_mix__v_0
   LDA   __local_f15__0
   STA   __zpabi_mix__v_1
   JSR   mix
   LDA   gs4+1
   BMI   .sx_neg@18
   LDA   #$00
   BEQ   .sx_done@19
.sx_neg@18:
   LDA   #$FF
.sx_done@19:
   STA   __zpabi_f6__p7_1
   JSR   f15@run1
   LDA   gs4+1
   STA   __zpabi_f6__p7_0
   JSR   f15@run2
   LDA   #$00
   STA   __zpabi_f6__p7_0
   STA   __zpabi_f6__p7_1
   JSR   f15@run1
   JSR   f15@run2
   LDA   gs4+1
   BMI   .sx_neg@20
   LDA   #$00
   BEQ   .sx_done@21
.sx_neg@20:
   LDA   #$FF
.sx_done@21:
   STA   HARGS+3
   LDA   #$00
   STA   HARGS
   STA   HARGS+1
   LDA   gs4+1
   STA   HARGS+2
   JSR   mul16
   LDA   HARGS+5
   BMI   .sx_neg@22
   LDA   #$00
   BEQ   .sx_done@23
.sx_neg@22:
   LDA   #$FF
.sx_done@23:
   JSR   f15@run1
   LDA   HARGS+4
   STA   __zpabi_f6__p7_0
   LDA   HARGS+5
   STA   __zpabi_f6__p7_1
   JSR   f15@run2
   STA   __zpabi_mix__v_0
   STX   __zpabi_mix__v_1
   JSR   f6@run3
   LDA   __local_f15__l22
   STA   __local_f15__l20
   LDA   __local_f15__l22+1
   STA   __local_f15__0
   LDX   #$06
.shift_loop@24:
   LSR   __local_f15__0
   ROR   __local_f15__l20
   DEX
   BNE   .shift_loop@24
   LDA   __local_f15__l20
   STA   HARGS+8
   LDA   __local_f15__0
   STA   HARGS+9
   LDA   #$00
   STA   HARGS+10
   RTS

f15@run2:
   SUBROUTINE

   LDA   #<__local_f15__l22
   STA   __zpabi_f6__pp8_0
   LDA   #>__local_f15__l22
   STA   __zpabi_f6__pp8_1
   JMP   f6

f15@run1:
   SUBROUTINE

   STA   __zpabi_f6__p7_2
   STA   __zpabi_f6__p7_3
   STA   __zpabi_f6__p7_4
   STA   __zpabi_f6__p7_5
   STA   __zpabi_f6__p7_6
   STA   __zpabi_f6__p7_7
   RTS

f24:
   SUBROUTINE

   LDA   #$00
   STA   __local_f24__l27
   STA   __local_f24__2
   LDA   #$FF
   STA   __local_f24__l28
   LDA   #$7F
   STA   __local_f24__l28+1
   LDA   #$00
   STA   __local_f24__l29
   STA   __local_f24__l29+1
   STA   __local_f24__l29+2
   STA   __local_f24__l29+3
   STA   __local_f24__l29+4
   STA   __local_f24__l29+5
   STA   __local_f24__l29+6
   STA   __local_f24__l29+7
   LDA   #$FF
   STA   gs4+2
   STA   g2
   STA   __zpabi_f6__p7_0
   LDA   #$00
   STA   __zpabi_f6__p7_1
   JSR   f15@run1
   LDA   #<__local_f24__l28
   STA   __zpabi_f6__pp8_0
   LDA   #>__local_f24__l28
   STA   __zpabi_f6__pp8_1
   JSR   f6
   STA   __local_f24__l27
   TXA
   STA   __local_f24__2
   LDA   g2
   BEQ   .lnot_true@26
   LDA   #$00
   BEQ   .lnot_end@27
.lnot_true@26:
   LDA   #$01
.lnot_end@27:
   TAX
   LDA   ga3
   STA   __zpabi_mix__v_2
   LDA   __local_f24__l27
   STA   __zpabi_mix__v_3
   LDA   __local_f24__2
   LDA   __zpabi_mix__v_2
   AND   __zpabi_mix__v_3
   STA   __zpabi_f15__p18_0
   LDA   #$00
   STA   __zpabi_f15__p18_1
   LDA   #<__local_f24__l29
   STA   __zpabi_f15__pp19_0
   LDA   #>__local_f24__l29
   STA   __zpabi_f15__pp19_1
   LDA   #$44
   JSR   f15
   LDA   #$00
   STA   __local_f24__w30
.loop@10_start:
   LDX   #$FF
   SEC
.div_loop@28:
   INX
   SBC   #$05
   BCS   .div_loop@28
   TXA
   BEQ   .if_else@2
   LDA   ga3+2
   ORA   #$03
   STA   ga3+2
   BEQ   .if_end@3
   LDA   g2
   ORA   #$03
   STA   g2
.if_end@3:
   LDA   __local_f24__l29
   LDX   __local_f24__l29+1
   STX   __local_f24__l27
   LDX   __local_f24__l29+2
   STX   __zpabi_mix__v_2
   LDX   __local_f24__l29+3
   STX   __zpabi_mix__v_3
   STA   __zpabi_mix__v_0
   LDA   __local_f24__l27
   STA   __zpabi_mix__v_1
   JSR   mix
   JMP   .loop@10_continue
.if_else@2:
   LDA   #$00
   STA   __local_f24__l28+1
   LDA   __local_f24__l28
   AND   #$07
   CMP   #$03
   BEQ   .case@12
   CMP   #$07
   BEQ   .case@13
   BNE   .switch@11_break
.case@12:
   INC   gs4+2
.case@13:
.switch@11_break:
.loop@10_continue:
   INC   __local_f24__w30
   LDA   __local_f24__w30
   CMP   #$02
   BCC   .loop@10_start
   LDA   #$00
   STA   HARGS+8
   LDY   #$00
   STY   HARGS+9
   STA   HARGS+10
   RTS

main:
   SUBROUTINE

   LDA   ga3+1
   AND   #$03
   STA   g2
   LDA   #$03
   STA   __zpabi_mix__v_0
   JSR   fin0@run0
   JSR   fin0
   JSR   fin1
   LDA   acc+1
   AND   #$7F
   TAX
   LDA   acc
   RTS

acc:
   DS.B  2

g1:
   DC.B  $03

ga3:
   DC.B  $7F, $00, $02, $80

gs4:
   DS.B  1
   DC.B  $FF
   DC.B  $FF

@13.s23:
   DC.B  $03

__zp_init:
   SUBROUTINE

   LDA   #$00
   STA   @3.s12+1
   LDA   #$7F
   STA   g2
   STA   @3.s12
   LDA   #<g2
   STA   gp5
   LDA   #>g2
   STA   gp5+1
   RTS
