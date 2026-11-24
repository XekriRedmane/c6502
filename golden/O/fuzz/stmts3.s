SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
z5	EQU	$26
z6	EQU	$28
z7	EQU	$2C
g1	EQU	$2E
g3	EQU	$2F
g2	EQU	$30
gp10	EQU	$31
g4	EQU	$33
__local_f29__0	EQU	$80
__zpabi_f19__p20_0	EQU	$80
__zpabi_f19__p20_1	EQU	$81
__zpabi_f19__p20_2	EQU	$82
__zpabi_f19__p20_3	EQU	$83
__zpabi_f19__p21_0	EQU	$84
__zpabi_f19__p21_1	EQU	$85
__zpabi_f19__p22_0	EQU	$86
__zpabi_f19__p22_1	EQU	$87
__zpabi_f19__pp23_0	EQU	$88
__zpabi_f19__pp23_1	EQU	$89
__zpabi_f12__p13_0	EQU	$8A
__zpabi_f12__p13_1	EQU	$8B
__zpabi_f12__p13_2	EQU	$8C
__zpabi_f12__p13_3	EQU	$8D
__zpabi_f12__p14_0	EQU	$8E
__zpabi_f12__p14_1	EQU	$8F
__zpabi_f12__p14_2	EQU	$90
__zpabi_f12__p14_3	EQU	$91
__zpabi_f12__pp15_0	EQU	$92
__zpabi_f12__pp15_1	EQU	$93
__zpabi_mix__v_0	EQU	$94
__zpabi_mix__v_1	EQU	$95
__zpabi_mix__v_2	EQU	$96
__zpabi_mix__v_3	EQU	$97
__local_f19__p20_3	EQU	$98
__local_main__0	EQU	$98
__local_f19__p20_2	EQU	$99
__local_main__1	EQU	$99
__local_main__2	EQU	$9A
__local_f19__w28	EQU	$9C
__local_main__4	EQU	$9C
__local_f19__0	EQU	$9D
__local_main__5	EQU	$9D
__local_f19__1	EQU	$9E
__local_main__6	EQU	$9E
__local_f19__2	EQU	$9F
__local_f19__3	EQU	$A0
__local_main__i41	EQU	$A0
__local_f19__4	EQU	$A1
__local_f19__5	EQU	$A2
__local_main__l39	EQU	$A2
__local_f19__6	EQU	$A3
__local_f19__7	EQU	$A4
__local_f19__8	EQU	$A5
__local_f19__9	EQU	$A6
__local_f19__l26	EQU	$A7
__local_f12__l16	EQU	$A8
__local_f12__0	EQU	$A9
__local_f12__3	EQU	$AC
__local_f12__4	EQU	$AD
__local_f12__5	EQU	$AE
__local_f12__7	EQU	$B0
__local_f12__8	EQU	$B1
__local_f12__9	EQU	$B2
__local_f12__10	EQU	$B3
__local_f12__l17	EQU	$B4
__local_f12__11	EQU	$B5
__local_mix__0	EQU	$B6
__local_mix__1	EQU	$B7
__local_mix__2	EQU	$B8
__local_mix__3	EQU	$B9

; @zp-link-meta-begin
; def f12 params=__zpabi_f12__p13_0,__zpabi_f12__p13_1,__zpabi_f12__p13_2,__zpabi_f12__p13_3,__zpabi_f12__p14_0,__zpabi_f12__p14_1,__zpabi_f12__p14_2,__zpabi_f12__p14_3,__zpabi_f12__pp15_0,__zpabi_f12__pp15_1 locals=__local_f12__l16,__local_f12__0,__local_f12__1,__local_f12__2,__local_f12__3,__local_f12__4,__local_f12__5,__local_f12__6,__local_f12__7,__local_f12__8,__local_f12__9,__local_f12__10,__local_f12__l17,__local_f12__11 indirect=false in_cycle=false
; def f19 params=__zpabi_f19__p20_0,__zpabi_f19__p20_1,__zpabi_f19__p20_2,__zpabi_f19__p20_3,__zpabi_f19__p21_0,__zpabi_f19__p21_1,__zpabi_f19__p22_0,__zpabi_f19__p22_1,__zpabi_f19__pp23_0,__zpabi_f19__pp23_1 locals=__local_f19__p20_3,__local_f19__p20_2,__local_f19__p20_1,__local_f19__p20_0,__local_f19__w28,__local_f19__0,__local_f19__1,__local_f19__2,__local_f19__3,__local_f19__4,__local_f19__5,__local_f19__6,__local_f19__7,__local_f19__8,__local_f19__9,__local_f19__l26 indirect=false in_cycle=false changes=__zpabi_f19__p20_0,__zpabi_f19__p20_1
; def f29 params=__zpabi_f29__p30,__zpabi_f29__p31 locals=__local_f29__0,__local_f29__1,__local_f29__2,__local_f29__3 indirect=false in_cycle=false param_regs=A,X
; def fin0 params= locals=__local_fin0__0,__local_fin0__1,__local_fin0__2,__local_fin0__3 indirect=false in_cycle=false
; def fin1 params= locals=__local_fin1__0,__local_fin1__1,__local_fin1__2,__local_fin1__3 indirect=false in_cycle=false
; def fin2 params= locals=__local_fin2__0,__local_fin2__1,__local_fin2__2,__local_fin2__3 indirect=false in_cycle=false
; def fin3 params= locals=__local_fin3__0,__local_fin3__1,__local_fin3__2,__local_fin3__3 indirect=false in_cycle=false
; def main params= locals=__local_main__0,__local_main__1,__local_main__2,__local_main__3,__local_main__4,__local_main__5,__local_main__6,__local_main__7,__local_main__i41,__local_main__8,__local_main__l39,__local_main__10 indirect=false in_cycle=false
; def mix params=__zpabi_mix__v_0,__zpabi_mix__v_1,__zpabi_mix__v_2,__zpabi_mix__v_3 locals=__local_mix__0,__local_mix__1,__local_mix__2,__local_mix__3 indirect=false in_cycle=false
; call f12 -> mix
; call f19 -> f12
; call f19 -> mix
; call f29 -> mix
; call fin0 -> mix
; call fin1 -> mix
; call fin2 -> mix
; call fin3 -> mix
; call main -> f12
; call main -> fin0
; call main -> fin1
; call main -> fin2
; call main -> fin3
; call main -> mix
; item mix section=.text local=false zero=false
; item fin0 section=.text local=false zero=false
; item fin0@run7 section=.text local=true zero=false
; item fin0@run5 section=.text local=true zero=false
; item fin0@run1 section=.text local=true zero=false
; item fin0@tail0 section=.text local=true zero=false
; item fin1 section=.text local=false zero=false
; item fin1@run8 section=.text local=true zero=false
; item fin2 section=.text local=false zero=false
; item fin2@run9 section=.text local=true zero=false
; item fin3 section=.text local=false zero=false
; item f12 section=.text local=false zero=false
; item f12@run6 section=.text local=true zero=false
; item f12@run0 section=.text local=true zero=false
; item f19 section=.text local=false zero=false
; item f19@run4 section=.text local=true zero=false
; item f19@run3 section=.text local=true zero=false
; item f29 section=.text local=false zero=false
; item main section=.text local=false zero=false
; item main@run2 section=.text local=true zero=false
; item acc section=.bss local=false zero=true
; item ga8 section=.data local=false zero=false
; item gs9 section=.data local=false zero=false
; item gp11 section=.data local=false zero=false
; item __zp_init section=.text local=false zero=false
; zp z5 size=2 at=auto def=true init=$00,$00
; zp z6 size=4 at=auto def=true init=$FF,$FF,$FF,$FF
; zp z7 size=2 at=auto def=true init=$FF,$FF
; zp g1 size=1 at=auto def=true init=$00 weight=30
; zp g3 size=1 at=auto def=true init=$FF weight=18
; zp g2 size=1 at=auto def=true init=$AC weight=8
; zp gp10 size=2 at=auto def=true init=<z6,>z6 weight=10
; zp g4 size=4 at=auto def=true init=$FF,$FF,$00,$00 weight=12
; runtime hargs_size=32 hargs_need=21
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
   JSR   fin0@run5
   LDA   g1
   JSR   fin0@run7
   LDA   g2
   BMI   .sx_neg@2
   LDA   #$00
   BEQ   .sx_done@3
.sx_neg@2:
   LDA   #$FF
.sx_done@3:
   JSR   fin0@run5
   LDA   g2
   JSR   fin0@run7
   LDA   g3
   STA   __zpabi_mix__v_0
   LDA   #$00
   STA   __zpabi_mix__v_1
   STA   __zpabi_mix__v_2
   JSR   fin0@run1
   LDA   g4
   STA   __zpabi_mix__v_0
   LDA   g4+1
   STA   __zpabi_mix__v_1
   LDA   g4+2
   STA   __zpabi_mix__v_2
   LDA   g4+3
   JMP   fin0@tail0

fin0@run7:
   SUBROUTINE

   STA   __zpabi_mix__v_0
   JMP   mix

fin0@run5:
   SUBROUTINE

   STA   __zpabi_mix__v_1
   STA   __zpabi_mix__v_2
   STA   __zpabi_mix__v_3
   RTS

fin0@run1:
   SUBROUTINE

   STA   __zpabi_mix__v_3
   JMP   mix

fin0@tail0:
   SUBROUTINE

   STA   __zpabi_mix__v_3
   JMP   mix

fin1:
   SUBROUTINE

   LDA   z5+1
   BMI   .sx_neg@4
   LDA   #$00
   BEQ   .sx_done@5
.sx_neg@4:
   LDA   #$FF
.sx_done@5:
   STA   __zpabi_mix__v_2
   STA   __zpabi_mix__v_3
   LDA   z5
   STA   __zpabi_mix__v_0
   LDA   z5+1
   JSR   fin1@run8
   LDA   z6
   STA   __zpabi_mix__v_0
   LDA   z6+1
   STA   __zpabi_mix__v_1
   LDA   z6+2
   STA   __zpabi_mix__v_2
   LDA   z6+3
   JSR   fin0@run1
   LDA   z7+1
   BMI   .sx_neg@6
   LDA   #$00
   BEQ   .sx_done@7
.sx_neg@6:
   LDA   #$FF
.sx_done@7:
   STA   __zpabi_mix__v_2
   STA   __zpabi_mix__v_3
   LDA   z7
   STA   __zpabi_mix__v_0
   LDA   z7+1
   JSR   fin1@run8
   LDA   ga8
   STA   __zpabi_mix__v_0
   LDA   ga8+1
   STA   __zpabi_mix__v_1
   LDA   ga8+2
   STA   __zpabi_mix__v_2
   LDA   ga8+3
   JMP   fin0@tail0

fin1@run8:
   SUBROUTINE

   STA   __zpabi_mix__v_1
   JMP   mix

fin2:
   SUBROUTINE

   LDA   ga8+4
   STA   __zpabi_mix__v_0
   JSR   fin2@run9
   STA   __zpabi_mix__v_2
   LDA   ga8+7
   JSR   fin0@run1
   LDA   ga8+8
   STA   __zpabi_mix__v_0
   LDA   ga8+9
   STA   __zpabi_mix__v_1
   LDA   ga8+10
   STA   __zpabi_mix__v_2
   LDA   ga8+11
   JSR   fin0@run1
   LDA   ga8+12
   STA   __zpabi_mix__v_0
   LDA   ga8+13
   STA   __zpabi_mix__v_1
   LDA   ga8+14
   STA   __zpabi_mix__v_2
   LDA   ga8+15
   JSR   fin0@run1
   LDA   gs9+1
   STA   __zpabi_mix__v_0
   LDA   gs9+2
   STA   __zpabi_mix__v_1
   LDA   #$00
   STA   __zpabi_mix__v_2
   JMP   fin0@tail0

fin2@run9:
   SUBROUTINE

   LDA   ga8+5
   STA   __zpabi_mix__v_1
   LDA   ga8+6
   RTS

fin3:
   SUBROUTINE

   LDA   gs9+3
   STA   __zpabi_mix__v_0
   LDA   gs9+4
   STA   __zpabi_mix__v_1
   LDA   gs9+5
   STA   __zpabi_mix__v_2
   LDA   gs9+6
   JMP   fin0@tail0

f12:
   SUBROUTINE

   LDA   #$FF
   STA   __local_f12__l16
   STA   __local_f12__0
   LDA   #$7F
   STA   __local_f12__l17
   LDA   #$00
   STA   __local_f12__11
   LDA   __zpabi_f12__p14_0
   CLC
   ADC   #$01
   STA   __zpabi_mix__v_0
   LDA   __zpabi_f12__p14_1
   ADC   #$00
   STA   __zpabi_mix__v_1
   LDA   __zpabi_f12__p14_2
   ADC   #$00
   STA   __local_f12__0
   LDA   __zpabi_f12__p14_3
   ADC   #$00
   JSR   f12@run0
   STA   __local_f12__l16
   LDA   #$00
   SEC
   SBC   __zpabi_mix__v_0
   STA   __zpabi_mix__v_0
   LDA   #$00
   SBC   __zpabi_mix__v_1
   STA   __zpabi_mix__v_1
   LDA   #$00
   SBC   __local_f12__0
   STA   __local_f12__0
   LDA   #$00
   SBC   __local_f12__l16
   JSR   f12@run0
   STA   __local_f12__l16
   LDA   __zpabi_mix__v_0
   ORA   __zpabi_mix__v_1
   ORA   __local_f12__0
   ORA   __local_f12__l16
   BNE   .lb_skip@0
   JMP   .if_end@0
.lb_skip@0:
   LDA   ga8+4
   STA   __local_f12__l16
   JSR   fin2@run9
   STA   __local_f12__0
   LDA   ga8+7
   STA   __local_f12__3
   INC   __local_f12__l16
   BNE   .inc_done@0
   INC   __zpabi_mix__v_1
   BNE   .inc_done@0
   INC   __local_f12__0
   BNE   .inc_done@0
   INC   __local_f12__3
.inc_done@0:
   LDA   __local_f12__l16
   STA   ga8+4
   LDA   __zpabi_mix__v_1
   STA   ga8+5
   LDA   __local_f12__0
   STA   ga8+6
   LDA   __local_f12__3
   STA   ga8+7
   LDA   __zpabi_f12__p13_0
   ORA   __local_f12__l16
   STA   __zpabi_mix__v_0
   LDA   __zpabi_f12__p13_1
   ORA   __zpabi_mix__v_1
   STA   __zpabi_mix__v_1
   LDA   __zpabi_f12__p13_2
   ORA   __local_f12__0
   STA   __local_f12__0
   LDA   __zpabi_f12__p13_3
   ORA   __local_f12__3
   JSR   f12@run0
   STA   __local_f12__l16
   LDA   __zpabi_mix__v_0
   LDY   #$00
   STA   (gp10),Y
   LDA   __zpabi_mix__v_1
   INY
   STA   (gp10),Y
   LDA   __local_f12__0
   INY
   STA   (gp10),Y
   LDA   __local_f12__l16
   INY
   STA   (gp10),Y
.if_end@0:
   LDA   ga8+12
   STA   __local_f12__10
   BMI   .sx_neg@8
   LDA   #$00
   BEQ   .sx_done@9
.sx_neg@8:
   LDA   #$FF
.sx_done@9:
   STA   __local_f12__9
   STA   __local_f12__8
   STA   __local_f12__7
   LDA   __zpabi_f12__p13_0
   ASL
   STA   __local_f12__3
   LDA   __zpabi_f12__p13_1
   ROL
   STA   __local_f12__4
   LDA   __zpabi_f12__p13_2
   ROL
   STA   __local_f12__5
   LDA   __zpabi_f12__p13_3
   ROL
   LDX   __local_f12__3
   STX   __zpabi_mix__v_0
   LDX   __local_f12__4
   STX   __zpabi_mix__v_1
   LDX   __local_f12__5
   STX   __local_f12__0
   JSR   f12@run0
   STA   __local_f12__l16
   LDA   __local_f12__10
   CLC
   ADC   __zpabi_mix__v_0
   STA   __zpabi_mix__v_0
   LDA   __local_f12__9
   ADC   __zpabi_mix__v_1
   STA   __zpabi_mix__v_1
   LDA   __local_f12__8
   ADC   __local_f12__0
   STA   __local_f12__0
   LDA   __local_f12__7
   ADC   __local_f12__l16
   AND   #$1F
   LDX   __local_f12__0
   STX   __zpabi_mix__v_2
   JSR   f12@run6
   JSR   fin0@run1
   LDA   z5
   EOR   #$5A
   STA   z5
   LDY   #$00
   LDA   (__zpabi_f12__pp15_0),Y
   STA   __zpabi_mix__v_1
   LDA   g1
   BMI   .sx_neg@10
   LDA   #$00
   BEQ   .sx_done@11
.sx_neg@10:
   LDA   #$FF
.sx_done@11:
   STA   __local_f12__l16
   LDA   __zpabi_mix__v_1
   ORA   g1
   STA   z5
   LDA   __local_f12__l16
   STA   z5+1
   LDY   #$00
   LDA   (__zpabi_f12__pp15_0),Y
   STA   __local_f12__0
   CLC
   ADC   #$01
   STA   (__zpabi_f12__pp15_0),Y
   LDA   z7
   AND   __local_f12__0
   SEC
   SBC   #$5A
   STA   g3
   LDA   __local_f12__l17
   STA   __local_f12__0
   LDA   __local_f12__11
   STA   __local_f12__l16
   LDA   gs9+7
   BNE   .cmp_true@12
   LDA   gs9+6
   BNE   .cmp_true@12
   LDA   gs9+5
   BNE   .cmp_true@12
   LDA   gs9+4
   CMP   __local_f12__l16
   BNE   .cmp_true@12
   LDA   gs9+3
   CMP   __local_f12__0
   BNE   .cmp_true@12
   LDA   #$00
   BEQ   .cmp_end@13
.cmp_true@12:
   LDA   #$01
.cmp_end@13:
   LDX   #$00
   RTS

f12@run6:
   SUBROUTINE

   EOR   #$10
   SEC
   SBC   #$10
   RTS

f12@run0:
   SUBROUTINE

   AND   #$1F
   JSR   f12@run6
   RTS

f19:
   SUBROUTINE

   LDA   #$FF
   STA   __local_f19__p20_3
   LDA   #$7F
   STA   __local_f19__p20_2
   LDA   g1
   STA   __local_f19__l26
   LDA   #$00
   STA   __local_f19__w28
   LDA   __zpabi_f19__p20_2
   STA   __local_f19__p20_2
   LDA   __zpabi_f19__p20_3
   STA   __local_f19__p20_3
.loop@0_continue:
   LDA   __local_f19__w28
   CLC
   ADC   #$01
   STA   __local_f19__9
   LDA   __local_f19__w28
   BEQ   .lb_skip@2
   JMP   .loop@0_break
.lb_skip@2:
   LDA   gs9+3
   STA   __local_f19__8
   LDA   gs9+4
   STA   __local_f19__7
   LDA   gs9+5
   STA   __local_f19__6
   LDA   gs9+6
   STA   __local_f19__5
   LDA   gs9+7
   STA   __local_f19__4
   LDA   __local_f19__8
   CLC
   ADC   #$01
   STA   __local_f19__3
   LDA   __local_f19__7
   ADC   #$00
   STA   __local_f19__2
   LDA   __local_f19__6
   ADC   #$00
   STA   __local_f19__1
   LDA   __local_f19__5
   ADC   #$00
   STA   __local_f19__0
   LDA   __local_f19__4
   ADC   #$00
   AND   #$01
   LDX   __local_f19__3
   STX   gs9+3
   LDX   __local_f19__2
   STX   gs9+4
   LDX   __local_f19__1
   STX   gs9+5
   LDX   __local_f19__0
   STX   gs9+6
   EOR   #$01
   SEC
   SBC   #$01
   STA   gs9+7
   LDA   #$7F
   SEC
   SBC   __local_f19__8
   STA   __local_f19__3
   LDA   #$00
   SBC   __local_f19__7
   STA   __local_f19__2
   LDA   #$00
   SBC   __local_f19__6
   STA   __local_f19__1
   LDA   #$00
   SBC   __local_f19__5
   STA   __local_f19__0
   LDA   #$00
   SBC   __local_f19__4
   AND   #$01
   EOR   #$01
   STA   __local_f19__w28
   DEC   __local_f19__w28
   LDA   __local_f19__3
   ORA   __local_f19__2
   ORA   __local_f19__1
   ORA   __local_f19__0
   ORA   __local_f19__w28
   BNE   .lb_skip@1
   JMP   .if_end@1
.lb_skip@1:
   LDA   gs9+1
   STA   __local_f19__0
   LDA   gs9+2
   BNE   .cmp_true@15
   LDA   __local_f19__0
   CMP   #$02
   BNE   .cmp_true@15
   LDA   #$00
   BEQ   .cmp_end@16
.cmp_true@15:
   LDA   #$01
.cmp_end@16:
   STA   __local_f19__w28
   LDA   #$01
   SEC
   SBC   __local_f19__w28
   LDA   #$00
   SBC   #$00
   BVC   .cmp_novf@18
   EOR   #$80
.cmp_novf@18:
   ASL
   LDA   #$00
   ROL
   EOR   #$01
   AND   #$07
   CMP   #$02
   BEQ   .switch@1_break
   CMP   #$03
   BEQ   .switch@1_break
   CMP   #$04
   BNE   .default@5
   LDA   __zpabi_f19__p20_0
   STA   HARGS
   LDA   __zpabi_f19__p20_1
   STA   HARGS+1
   LDA   __local_f19__p20_2
   STA   HARGS+2
   LDA   __local_f19__p20_3
   STA   HARGS+3
   LDA   #$06
   STA   HARGS+4
   JSR   asl32
   LDA   HARGS+5
   STA   __zpabi_f19__p20_0
   LDA   HARGS+6
   STA   __zpabi_f19__p20_1
   STA   __local_f19__1
   LDA   HARGS+8
   STA   __local_f19__p20_3
   LDA   HARGS+7
   STA   __local_f19__p20_2
   STA   __local_f19__0
   LDA   __local_f19__p20_3
   JSR   f12@run0
   STA   __local_f19__w28
   LDA   HARGS+5
   LDY   #$00
   STA   (gp10),Y
   LDA   __local_f19__1
   INY
   STA   (gp10),Y
   LDA   __local_f19__0
   INY
   STA   (gp10),Y
   LDA   __local_f19__w28
   INY
   STA   (gp10),Y
   LDA   g1
   JSR   f19@run4
   STA   g1
.default@5:
.switch@1_break:
.if_end@1:
   LDA   __local_f19__9
   STA   __local_f19__w28
   JMP   .loop@0_continue
.loop@0_break:
   LDA   __zpabi_f19__p22_1
   BMI   .sx_neg@19
   LDA   #$00
   BEQ   .sx_done@20
.sx_neg@19:
   LDA   #$FF
.sx_done@20:
   STA   __zpabi_mix__v_2
   STA   __zpabi_mix__v_3
   LDA   __zpabi_f19__p22_0
   STA   __zpabi_mix__v_0
   LDA   __zpabi_f19__p22_1
   JSR   fin1@run8
   JSR   f19@run3
   LDA   #$03
   STA   HARGS+4
   JSR   asr32
   LDA   HARGS+8
   AND   #$1F
   LDX   HARGS+5
   STX   __zpabi_f12__p13_0
   LDX   HARGS+6
   STX   __zpabi_f12__p13_1
   LDX   HARGS+7
   STX   __zpabi_f12__p13_2
   JSR   f12@run6
   STA   __zpabi_f12__p13_3
   LDA   #$02
   STA   __zpabi_f12__p14_0
   LDA   #$00
   STA   __zpabi_f12__p14_1
   STA   __zpabi_f12__p14_2
   STA   __zpabi_f12__p14_3
   LDA   #<__local_f19__l26
   STA   __zpabi_f12__pp15_0
   LDA   #>__local_f19__l26
   STA   __zpabi_f12__pp15_1
   JSR   f12
   LDY   #$00
   LDA   (__zpabi_f19__pp23_0),Y
   STA   __zpabi_f19__p20_0
   INY
   LDA   (__zpabi_f19__pp23_0),Y
   STA   __zpabi_f19__p20_1
   INY
   LDA   (__zpabi_f19__pp23_0),Y
   STA   __local_f19__p20_2
   LDA   #$00
   INY
   CMP   (__zpabi_f19__pp23_0),Y
   BNE   .cmp_end@22
   CMP   __local_f19__p20_2
   BNE   .cmp_end@22
   CMP   __zpabi_f19__p20_1
   BNE   .cmp_end@22
   LDA   #$02
   CMP   __zpabi_f19__p20_0
   BEQ   .cmp_true@21
   LDA   #$00
   BEQ   .cmp_end@22
.cmp_true@21:
   LDA   #$01
.cmp_end@22:
   LDX   #$00
   RTS

f19@run4:
   SUBROUTINE

   STA   HARGS
   LDA   #$5A
   STA   HARGS+1
   JSR   mul8
   LDA   HARGS+2
   RTS

f19@run3:
   SUBROUTINE

   LDA   g4
   STA   HARGS
   LDA   g4+1
   STA   HARGS+1
   LDA   g4+2
   STA   HARGS+2
   LDA   g4+3
   STA   HARGS+3
   RTS

f29:
   SUBROUTINE

   LDA   g3
   JSR   f19@run4
   STA   g3
   LDA   z6
   SEC
   SBC   HARGS+2
   STA   __zpabi_mix__v_0
   LDA   z6+1
   SBC   #$00
   STA   __zpabi_mix__v_1
   LDA   z6+2
   SBC   #$00
   STA   __zpabi_mix__v_2
   LDA   z6+3
   SBC   #$00
   JSR   f12@run0
   JSR   fin0@run1
   LDA   g1
   ASL
   ASL
   ASL
   ASL
   ASL
   ASL
   STA   __local_f29__0
   STA   g1
   BMI   .sx_neg@24
   LDA   #$00
   BEQ   .sx_done@25
.sx_neg@24:
   LDA   #$FF
.sx_done@25:
   JSR   fin0@run5
   LDA   __local_f29__0
   JSR   fin0@run7
   LDA   g3
   AND   gs9+1
   STA   g2
   LDA   #$80
   SEC
   SBC   z6
   LDA   #$00
   SBC   z6+1
   LDA   #$00
   SBC   z6+2
   LDA   #$00
   SBC   z6+3
   BVC   .cmp_novf@26
   EOR   #$80
.cmp_novf@26:
   ASL
   LDA   #$00
   ROL
   EOR   #$01
   STA   __local_f29__0
   LDA   #$00
   BPL   .sx_done@28
   LDA   #$FF
.sx_done@28:
   STA   HARGS+18
   STA   HARGS+19
   STA   HARGS+20
   LDA   __local_f29__0
   STA   HARGS+16
   LDA   #$00
   STA   HARGS+17
   RTS

main:
   SUBROUTINE

   LDA   #$80
   STA   __local_main__l39
   LDA   gp11
   STA   DPTR
   LDA   gp11+1
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   STA   __zpabi_f12__p14_0
   BMI   .sx_neg@29
   LDA   #$00
   BEQ   .sx_done@30
.sx_neg@29:
   LDA   #$FF
.sx_done@30:
   STA   __local_main__2
   STA   __local_main__1
   STA   __local_main__0
   LDA   __zpabi_f12__p14_0
   AND   z6
   STA   __zpabi_f12__p13_0
   LDA   __local_main__2
   AND   z6+1
   STA   __local_main__6
   LDA   __local_main__1
   AND   z6+2
   STA   __local_main__5
   LDA   __local_main__0
   AND   z6+3
   STA   __local_main__4
   LDA   g2
   BMI   .sx_neg@31
   LDA   #$00
   BEQ   .sx_done@32
.sx_neg@31:
   LDA   #$FF
.sx_done@32:
   STA   __local_main__2
   BMI   .sx_neg@33
   LDA   #$00
   BEQ   .sx_done@34
.sx_neg@33:
   LDA   #$FF
.sx_done@34:
   STA   __local_main__1
   STA   __local_main__0
   LDA   __zpabi_f12__p13_0
   AND   g2
   STA   __zpabi_mix__v_0
   LDA   __local_main__6
   AND   __local_main__2
   STA   __zpabi_mix__v_1
   LDA   __local_main__5
   AND   __local_main__1
   STA   __zpabi_mix__v_2
   LDA   __local_main__4
   AND   __local_main__0
   JSR   fin0@run1
   JSR   f19@run3
   LDA   #$00
   STA   HARGS+4
   JSR   asr32
   LDA   HARGS+5
   STA   g4
   LDA   HARGS+6
   STA   g4+1
   LDA   HARGS+7
   STA   g4+2
   LDA   HARGS+8
   STA   g4+3
   LDA   g4
   ORA   g4+1
   ORA   g4+2
   ORA   g4+3
   BNE   .lb_skip@5
   JMP   .if_else@3
.lb_skip@5:
   LDA   __local_main__l39
   BEQ   .lnot_true@35
   LDA   #$00
   BEQ   .lnot_end@36
.lnot_true@35:
   LDA   #$01
.lnot_end@36:
   STA   __zpabi_f12__p13_0
   LDA   #$00
   BPL   .sx_done@38
   LDA   #$FF
.sx_done@38:
   STA   __local_main__i41
   LDA   __local_main__l39
   STA   __local_main__6
   LDA   #$00
   BPL   .sx_done@40
   LDA   #$FF
.sx_done@40:
   STA   __local_main__5
   STA   __local_main__4
   LDY   #$00
   LDA   (gp10),Y
   STA   __zpabi_f12__p14_0
   INY
   LDA   (gp10),Y
   STA   __local_main__2
   INY
   LDA   (gp10),Y
   STA   __local_main__1
   INY
   LDA   (gp10),Y
   STA   __local_main__0
   LDA   __local_main__6
   AND   __zpabi_f12__p14_0
   STA   __zpabi_f12__p14_0
   LDA   #$00
   AND   __local_main__2
   STA   __zpabi_f12__p14_1
   LDA   __local_main__5
   AND   __local_main__1
   STA   __zpabi_f12__p14_2
   LDA   __local_main__4
   AND   __local_main__0
   STA   __zpabi_f12__p14_3
   LDA   #$00
   STA   __zpabi_f12__p13_1
   LDA   __local_main__i41
   JSR   main@run2
   STA   __zpabi_mix__v_0
   STX   __zpabi_mix__v_1
   LDA   #$00
   STA   __zpabi_mix__v_2
   JSR   fin0@run1
   DEC   g2
   LDA   g2
   AND   #$01
   STA   g2
   JMP   .if_end@2
.if_else@3:
   LDA   gs9+1
   STA   __local_main__1
   LDA   gs9+2
   ASL   __local_main__1
   ROL
   ASL   __local_main__1
   ROL
   ASL   __local_main__1
   ROL
   ASL   __local_main__1
   ROL
   STA   gs9+2
   LDA   __local_main__1
   STA   gs9+1
   AND   #$07
   CMP   #$04
.if_end@2:
   LDA   #$01
   STA   g3
   LDA   g1
   BMI   .sx_neg@41
   LDA   #$00
   BEQ   .sx_done@42
.sx_neg@41:
   LDA   #$FF
.sx_done@42:
   STA   __local_main__0
   LDA   #$01
   SEC
   SBC   g1
   STA   __local_main__1
   LDA   #$00
   SBC   __local_main__0
   ORA   __local_main__1
   BNE   .lb_skip@4
   JMP   .if_end@4
.lb_skip@4:
   LDA   #$00
   STA   __local_main__i41
.loop@8_start:
   LDA   __local_main__i41
   CMP   #$03
   BCC   .lb_skip@3
   JMP   .loop@8_break
.lb_skip@3:
   LDA   z6
   STA   __local_main__5
   SEC
   SBC   #$01
   STA   __zpabi_f12__p14_0
   LDA   z6+1
   SBC   #$00
   STA   __local_main__2
   LDA   z6+2
   SBC   #$00
   STA   __local_main__1
   LDA   z6+3
   SBC   #$00
   AND   #$1F
   EOR   #$10
   LDX   __zpabi_f12__p14_0
   STX   z6
   LDX   __local_main__2
   STX   z6+1
   LDX   __local_main__1
   STX   z6+2
   SEC
   SBC   #$10
   STA   z6+3
   LDA   __local_main__5
   AND   #$07
   CMP   #$02
   BEQ   .case@10
   CMP   #$06
   BEQ   .case@11
   CMP   #$07
   BEQ   .case@12
   JMP   .default@13
.case@10:
   LDA   __local_main__i41
   BNE   .or_true@5
   LDA   g3
   BNE   .or_true@5
   LDA   #$00
   STA   __zpabi_f12__p13_0
   JMP   .or_end@6
.or_true@5:
   LDA   #$01
   STA   __zpabi_f12__p13_0
   LDA   #$00
.or_end@6:
   ORA   #$00
   BPL   .sx_done@44
   LDA   #$FF
.sx_done@44:
   STA   __local_main__5
   LDA   __local_main__l39
   BEQ   .cond_else@7
   LDA   g4
   STA   __zpabi_f12__p14_0
   LDA   g4+1
   STA   __local_main__2
   LDA   g4+2
   STA   __local_main__1
   LDA   g4+3
   JMP   .cond_end@8
.cond_else@7:
   LDA   z6+3
.cond_end@8:
   AND   #$1F
   LDX   __local_main__2
   STX   __zpabi_f12__p14_1
   LDX   __local_main__1
   STX   __zpabi_f12__p14_2
   JSR   f12@run6
   STA   __zpabi_f12__p14_3
   LDA   #$00
   STA   __zpabi_f12__p13_1
   LDA   __local_main__5
   JSR   main@run2
   JMP   .switch@9_break
.case@11:
.case@12:
   LDA   __local_main__i41
   BNE   .or_true@9
   LDA   gs9+1
   ORA   gs9+2
   BNE   .or_true@9
   LDA   #$00
   BEQ   .or_end@10
.or_true@9:
   LDA   #$01
.or_end@10:
   STA   g1
.default@13:
.switch@9_break:
   INC   __local_main__i41
   JMP   .loop@8_start
.loop@8_break:
.if_end@4:
   JSR   fin0
   JSR   fin1
   JSR   fin2
   JSR   fin3
   LDA   acc+1
   AND   #$7F
   TAX
   LDA   acc
   RTS

main@run2:
   SUBROUTINE

   STA   __zpabi_f12__p13_2
   STA   __zpabi_f12__p13_3
   LDA   #<__local_main__l39
   STA   __zpabi_f12__pp15_0
   LDA   #>__local_main__l39
   STA   __zpabi_f12__pp15_1
   JMP   f12

acc:
   DS.B  2

ga8:
   DC.L  $0000007F
   DC.L  $00000002
   DC.L  $80000000
   DC.L  $00000002

gs9:
   DS.B  1
   DC.W  $000A
   DC.B  $0C
   DS.B  4

gp11:
   DC.W  g2

__zp_init:
   SUBROUTINE

   LDA   #$00
   STA   g1
   STA   g4+2
   STA   g4+3
   STA   z5
   STA   z5+1
   LDA   #$AC
   STA   g2
   LDA   #$FF
   STA   g3
   STA   g4
   STA   g4+1
   STA   z6
   STA   z6+1
   STA   z6+2
   STA   z6+3
   STA   z7
   STA   z7+1
   LDA   #<z6
   STA   gp10
   LDA   #>z6
   STA   gp10+1
   RTS
