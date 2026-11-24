SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
z4	EQU	$26
g1	EQU	$2C
g3	EQU	$2D
__zpabi_f11__p13_0	EQU	$80
__zpabi_f11__p13_1	EQU	$81
__zpabi_f11__p13_2	EQU	$82
__zpabi_f11__p13_3	EQU	$83
__local_main__i20	EQU	$84
__zpabi_f11__p13_4	EQU	$84
__local_main__l18	EQU	$85
__zpabi_f11__p13_5	EQU	$85
__zpabi_f7__p8_0	EQU	$86
__zpabi_mix__v_0	EQU	$86
__zpabi_f7__p8_1	EQU	$87
__zpabi_mix__v_1	EQU	$87
__zpabi_f7__p8_2	EQU	$88
__zpabi_mix__v_2	EQU	$88
__zpabi_f7__p8_3	EQU	$89
__zpabi_mix__v_3	EQU	$89
__zpabi_f7__p8_4	EQU	$8A
__local_f11__i17	EQU	$8D
__local_f7__0	EQU	$92
__local_mix__0	EQU	$92
__local_mix__1	EQU	$93
__local_f7__2	EQU	$94
__local_mix__2	EQU	$94
__local_mix__3	EQU	$95
__local_f7__6	EQU	$98

; @zp-link-meta-begin
; def f11 params=__zpabi_f11__p12_0,__zpabi_f11__p12_1,__zpabi_f11__p13_0,__zpabi_f11__p13_1,__zpabi_f11__p13_2,__zpabi_f11__p13_3,__zpabi_f11__p13_4,__zpabi_f11__p13_5 locals=__local_f11__0,__local_f11__1,__local_f11__i17,__local_f11__2,__local_f11__3,__local_f11__4,__local_f11__5 indirect=false in_cycle=false param_regs=A,X,,,,,,
; def f7 params=__zpabi_f7__p8_0,__zpabi_f7__p8_1,__zpabi_f7__p8_2,__zpabi_f7__p8_3,__zpabi_f7__p8_4 locals=__local_f7__0,__local_f7__1,__local_f7__2,__local_f7__3,__local_f7__4,__local_f7__5,__local_f7__6 indirect=false in_cycle=false
; def fin0 params= locals=__local_fin0__0,__local_fin0__1,__local_fin0__2,__local_fin0__3 indirect=false in_cycle=false
; def fin1 params= locals=__local_fin1__0 indirect=false in_cycle=false
; def fin2 params= locals=__local_fin2__0,__local_fin2__1 indirect=false in_cycle=false
; def main params= locals=__local_main__0,__local_main__1,__local_main__2,__local_main__3,__local_main__i20,__local_main__l18 indirect=false in_cycle=false
; def mix params=__zpabi_mix__v_0,__zpabi_mix__v_1,__zpabi_mix__v_2,__zpabi_mix__v_3 locals=__local_mix__0,__local_mix__1,__local_mix__2,__local_mix__3 indirect=false in_cycle=false
; call f11 -> f7
; call f11 -> mix
; call fin0 -> mix
; call fin1 -> mix
; call fin2 -> mix
; call main -> f7
; call main -> fin0
; call main -> fin1
; call main -> fin2
; item mix section=.text local=false zero=false
; item fin0 section=.text local=false zero=false
; item fin0@tail0 section=.text local=true zero=false
; item fin1 section=.text local=false zero=false
; item fin1@run2 section=.text local=true zero=false
; item fin1@run0 section=.text local=true zero=false
; item fin2 section=.text local=false zero=false
; item f7 section=.text local=false zero=false
; item f11 section=.text local=false zero=false
; item f11@run1 section=.text local=true zero=false
; item main section=.text local=false zero=false
; item acc section=.bss local=false zero=true
; item g2 section=.data local=false zero=false
; item ga5 section=.data local=false zero=false
; item gs6 section=.data local=false zero=false
; item @2.s10 section=.data local=true zero=false
; item @6.s15 section=.data local=true zero=false
; item __zp_init section=.text local=false zero=false
; zp z4 size=6 at=auto def=true init=$FF,$FF,$00,$00,$00,$00
; zp g1 size=1 at=auto def=true init=$00 weight=81
; zp g3 size=1 at=auto def=true init=$FF weight=3
; runtime hargs_size=32 hargs_need=22
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
   STA   __zpabi_mix__v_1
   STA   __zpabi_mix__v_2
   STA   __zpabi_mix__v_3
   JSR   mix
   LDA   g2
   STA   __zpabi_mix__v_0
   LDA   g2+1
   STA   __zpabi_mix__v_1
   LDA   #$00
   STA   __zpabi_mix__v_2
   STA   __zpabi_mix__v_3
   JSR   mix
   LDA   g3
   BMI   .sx_neg@2
   LDA   #$00
   BEQ   .sx_done@3
.sx_neg@2:
   LDA   #$FF
.sx_done@3:
   STA   __zpabi_mix__v_1
   STA   __zpabi_mix__v_2
   STA   __zpabi_mix__v_3
   LDA   g3
   STA   __zpabi_mix__v_0
   JSR   mix
   LDA   z4
   STA   __zpabi_mix__v_0
   LDA   z4+1
   STA   __zpabi_mix__v_1
   LDA   z4+2
   STA   __zpabi_mix__v_2
   LDA   z4+3
   JMP   fin0@tail0

fin0@tail0:
   SUBROUTINE

   STA   __zpabi_mix__v_3
   JMP   mix

fin1:
   SUBROUTINE

   LDA   ga5
   JSR   fin1@run0
   LDA   ga5+1
   JSR   fin1@run0
   LDA   ga5+2
   JSR   fin1@run0
   LDA   ga5+3
   JSR   fin1@run2
   JMP   fin0@tail0

fin1@run2:
   SUBROUTINE

   STA   __zpabi_mix__v_0
   LDA   #$00
   STA   __zpabi_mix__v_1
   STA   __zpabi_mix__v_2
   RTS

fin1@run0:
   SUBROUTINE

   JSR   fin1@run2
   STA   __zpabi_mix__v_3
   JMP   mix

fin2:
   SUBROUTINE

   LDA   gs6+2
   BMI   .sx_neg@4
   LDA   #$00
   BEQ   .sx_done@5
.sx_neg@4:
   LDA   #$FF
.sx_done@5:
   STA   __zpabi_mix__v_2
   STA   __zpabi_mix__v_3
   LDA   gs6+1
   STA   __zpabi_mix__v_0
   LDA   gs6+2
   STA   __zpabi_mix__v_1
   JSR   mix
   LDA   gs6+3
   STA   __zpabi_mix__v_0
   LDA   gs6+4
   STA   __zpabi_mix__v_1
   LDA   #$00
   STA   __zpabi_mix__v_2
   JMP   fin0@tail0

f7:
   SUBROUTINE

   LDA   __zpabi_f7__p8_0
   ORA   #$05
   STA   __local_f7__6
   LDA   ga5+3
   LSR
   LSR
   LSR
   LSR
   LSR
   LSR
   STA   ga5+3
   LDA   z4
   STA   HARGS
   LDA   z4+1
   STA   HARGS+1
   LDA   z4+2
   STA   HARGS+2
   LDA   z4+3
   STA   HARGS+3
   LDA   z4+4
   STA   HARGS+4
   LDA   z4+5
   STA   HARGS+5
   LDA   #$00
   STA   HARGS+6
   STA   HARGS+7
   LDY   #$03
   STY   HARGS+8
   LDY   #$00
   STY   HARGS+9
   STY   HARGS+10
   STY   HARGS+11
   STY   HARGS+12
   STY   HARGS+13
   STA   HARGS+14
   STA   HARGS+15
   JSR   mul64
   LDA   HARGS+21
   AND   #$0F
   EOR   #$08
   LDX   HARGS+16
   STX   z4
   LDX   HARGS+17
   STX   z4+1
   LDX   HARGS+18
   STX   z4+2
   LDX   HARGS+19
   STX   z4+3
   LDX   HARGS+20
   STX   z4+4
   SEC
   SBC   #$08
   STA   z4+5
   LDA   ga5
   LDX   g2
   STX   __local_f7__0
   LDX   g2+1
   AND   __local_f7__0
   STA   __local_f7__0
   LDA   z4
   SEC
   SBC   __local_f7__0
   STA   __local_f7__2
   LDA   z4+1
   SBC   #$00
   STA   @2.s10+1
   LDA   __local_f7__2
   STA   @2.s10
   LDA   __local_f7__6
   BMI   .sx_neg@6
   LDA   #$00
   BEQ   .sx_done@7
.sx_neg@6:
   LDA   #$FF
.sx_done@7:
   TAX
   LDA   __local_f7__6
   RTS

f11:
   SUBROUTINE

   LDA   #$FF
   SEC
   SBC   z4
   STA   __zpabi_f7__p8_0
   LDA   #$FF
   SBC   z4+1
   STA   __zpabi_f7__p8_1
   LDA   #$FF
   SBC   z4+2
   STA   __local_f11__i17
   LDA   #$FF
   SBC   z4+3
   STA   __zpabi_f7__p8_3
   LDA   #$FF
   SBC   z4+4
   SEC
   AND   #$01
   LDX   __local_f11__i17
   JSR   f11@run1
   DEC   ga5+1
   LDA   ga5+1
   JSR   fin1@run0
   LDA   #$00
   STA   __local_f11__i17
   JMP   .loop@1_start
.f11@rot@0:
   LDA   g1
   ADC   #$03
   STA   g1
   STA   z4
   BMI   .sx_neg@8
   LDA   #$00
   BEQ   .sx_done@9
.sx_neg@8:
   LDA   #$FF
.sx_done@9:
   STA   z4+1
   STA   z4+2
   STA   z4+3
   STA   z4+4
   STA   z4+5
   LDA   __local_f11__i17
   AND   #$03
   TAX
   LDA   ga5,X
   EOR   #$03
   STA   ga5,X
   LDA   #$02
   ORA   __local_f11__i17
   JSR   fin1@run0
   INC   __local_f11__i17
.loop@1_start:
   LDA   __local_f11__i17
   CMP   #$03
   BCC   .f11@rot@0
   LDA   #$5B
   SBC   __zpabi_f11__p13_0
   STA   __zpabi_f7__p8_0
   LDA   #$20
   SBC   __zpabi_f11__p13_1
   STA   __zpabi_f7__p8_1
   LDA   #$DC
   SBC   __zpabi_f11__p13_2
   STA   __local_f11__i17
   LDA   #$AC
   SBC   __zpabi_f11__p13_3
   STA   __zpabi_f7__p8_3
   LDA   #$00
   SBC   __zpabi_f11__p13_4
   SEC
   AND   #$01
   LDX   __local_f11__i17
   JSR   f11@run1
   LDA   @6.s15
   ORA   @6.s15+1
   ORA   @6.s15+2
   ORA   @6.s15+3
   BEQ   .lnot_true@10
   LDA   #$00
   BEQ   .lnot_end@11
.lnot_true@10:
   LDA   #$01
.lnot_end@11:
   LDX   #$00
   RTS

f11@run1:
   SUBROUTINE

   STX   __zpabi_f7__p8_2
   EOR   #$01
   SBC   #$01
   STA   __zpabi_f7__p8_4
   JMP   f7

main:
   SUBROUTINE

   LDA   #$00
   STA   __zpabi_f7__p8_0
   STA   __zpabi_f7__p8_1
   STA   __zpabi_f7__p8_2
   STA   __zpabi_f7__p8_3
   STA   __zpabi_f7__p8_4
   JSR   f7
   STA   __local_main__l18
   LDA   #$00
   STA   __local_main__i20
   JMP   .loop@2_start
.main@rot@1:
   AND   #$03
   TAX
   LDA   ga5,X
   STA   ga5,X
   LDA   __local_main__l18
   EOR   #$01
   STA   __local_main__l18
   EOR   z4
   STA   gs6+1
   LDA   #$00
   EOR   z4+1
   STA   gs6+2
   INC   __local_main__i20
.loop@2_start:
   LDA   __local_main__i20
   CMP   #$03
   BCC   .main@rot@1
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

g2:
   DC.W  $0001

ga5:
   DC.B  $00, $00, $FF, $FF

gs6:
   DS.B  1
   DC.W  $FFFF
   DS.B  2

@2.s10:
   DC.W  $FFFF

@6.s15:
   DC.L  $00000100

__zp_init:
   SUBROUTINE

   LDA   #$00
   STA   g1
   STA   z4+2
   STA   z4+3
   STA   z4+4
   STA   z4+5
   LDA   #$FF
   STA   g3
   STA   z4
   STA   z4+1
   RTS
