SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
z5	EQU	$26
z6	EQU	$2A
z7	EQU	$2B
@20.s36	EQU	$2D
gp11	EQU	$2E
g1	EQU	$30
g4	EQU	$32
gp10	EQU	$34
__zpabi_f12__p14_0	EQU	$80
__zpabi_f29__p31_0	EQU	$80
__zpabi_f12__p14_1	EQU	$81
__zpabi_f29__p31_1	EQU	$81
__local_main__1	EQU	$82
__zpabi_f12__p15_0	EQU	$82
__zpabi_f29__p32	EQU	$82
__local_main__l39	EQU	$83
__zpabi_f12__p15_1	EQU	$83
__zpabi_f29__pp33_0	EQU	$83
__zpabi_f12__pp16_0	EQU	$84
__zpabi_f29__pp33_1	EQU	$84
__zpabi_f12__pp16_1	EQU	$85
__zpabi_mix__v_0	EQU	$86
__zpabi_mix__v_1	EQU	$87
__zpabi_mix__v_2	EQU	$88
__zpabi_mix__v_3	EQU	$89
__local_f12__0	EQU	$8A
__local_f21__w28	EQU	$8A
__local_f12__1	EQU	$8B
__local_f21__l23	EQU	$8B
__local_f12__w20	EQU	$8C
__local_f21__0	EQU	$8C
__local_f12__2	EQU	$8D
__local_f21__1	EQU	$8D
__local_mix__0	EQU	$92
__local_mix__1	EQU	$93
__local_mix__2	EQU	$94
__local_mix__3	EQU	$95

; @zp-link-meta-begin
; def f12 params=__zpabi_f12__p13_0,__zpabi_f12__p13_1,__zpabi_f12__p14_0,__zpabi_f12__p14_1,__zpabi_f12__p15_0,__zpabi_f12__p15_1,__zpabi_f12__pp16_0,__zpabi_f12__pp16_1 locals=__local_f12__0,__local_f12__1,__local_f12__w20,__local_f12__2 indirect=false in_cycle=false param_regs=A,X,,,,,,
; def f21 params=__zpabi_f21__p22_0,__zpabi_f21__p22_1 locals=__local_f21__w28,__local_f21__l23,__local_f21__0,__local_f21__1,__local_f21__p22_1,__local_f21__p22_0,__local_f21__2,__local_f21__3 indirect=false in_cycle=false param_regs=A,X
; def f29 params=__zpabi_f29__p30_0,__zpabi_f29__p30_1,__zpabi_f29__p31_0,__zpabi_f29__p31_1,__zpabi_f29__p32,__zpabi_f29__pp33_0,__zpabi_f29__pp33_1 locals=__local_f29__w37,__local_f29__0 indirect=false in_cycle=false param_regs=A,X,,,,,
; def fin0 params= locals=__local_fin0__0,__local_fin0__1 indirect=false in_cycle=false
; def fin1 params= locals=__local_fin1__0,__local_fin1__1,__local_fin1__2,__local_fin1__3 indirect=false in_cycle=false
; def fin2 params= locals=__local_fin2__0,__local_fin2__1,__local_fin2__2,__local_fin2__3 indirect=false in_cycle=false
; def fin3 params= locals=__local_fin3__0,__local_fin3__1 indirect=false in_cycle=false
; def main params= locals=__local_main__w42,__local_main__0,__local_main__1,__local_main__l39,__local_main__2,__local_main__3 indirect=false in_cycle=false
; def mix params=__zpabi_mix__v_0,__zpabi_mix__v_1,__zpabi_mix__v_2,__zpabi_mix__v_3 locals=__local_mix__0,__local_mix__1,__local_mix__2,__local_mix__3 indirect=false in_cycle=false
; call f12 -> mix
; call f21 -> mix
; call f29 -> mix
; call fin0 -> mix
; call fin1 -> mix
; call fin2 -> mix
; call fin3 -> mix
; call main -> fin0
; call main -> fin1
; call main -> fin2
; call main -> fin3
; call main -> mix
; item mix section=.text local=false zero=false
; item fin0 section=.text local=false zero=false
; item fin0@run5 section=.text local=true zero=false
; item fin0@run4 section=.text local=true zero=false
; item fin0@run3 section=.text local=true zero=false
; item fin0@run0 section=.text local=true zero=false
; item fin0@tail0 section=.text local=true zero=false
; item fin0@tail0@tail1 section=.text local=true zero=false
; item fin1 section=.text local=false zero=false
; item fin1@run2 section=.text local=true zero=false
; item fin2 section=.text local=false zero=false
; item fin3 section=.text local=false zero=false
; item f12 section=.text local=false zero=false
; item f21 section=.text local=false zero=false
; item f21@run1 section=.text local=true zero=false
; item f29 section=.text local=false zero=false
; item main section=.text local=false zero=false
; item acc section=.bss local=false zero=true
; item g2 section=.data local=false zero=false
; item g3 section=.data local=false zero=false
; item ga8 section=.data local=false zero=false
; item gs9 section=.data local=false zero=false
; item @10.s27 section=.data local=true zero=false
; item __zp_init section=.text local=false zero=false
; zp z5 size=4 at=auto def=true init=$01,$00,$00,$00
; zp z6 size=1 at=auto def=true init=$80
; zp z7 size=2 at=auto def=true init=$00,$00
; zp @20.s36 size=1 at=auto def=true init=$FF weight=65 local=true
; zp gp11 size=2 at=auto def=true init=<z5,>z5 weight=71
; zp g1 size=2 at=auto def=true init=$FF,$00 weight=19
; zp g4 size=2 at=auto def=true init=$E8,$BF weight=11
; zp gp10 size=2 at=auto def=true init=<z6,>z6 weight=11
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

   LDA   g1+1
   BMI   .sx_neg@0
   LDA   #$00
   BEQ   .sx_done@1
.sx_neg@0:
   LDA   #$FF
.sx_done@1:
   STA   __zpabi_mix__v_2
   JSR   fin0@run3
   LDA   g2
   STA   __zpabi_mix__v_0
   LDA   g2+1
   JSR   fin0@run4
   LDA   g3
   STA   __zpabi_mix__v_0
   LDA   g3+1
   STA   __zpabi_mix__v_1
   BMI   .sx_neg@2
   LDA   #$00
   BEQ   .sx_done@3
.sx_neg@2:
   LDA   #$FF
.sx_done@3:
   JSR   fin0@run5
   LDA   g4
   STA   __zpabi_mix__v_0
   LDA   g4+1
   JMP   fin0@tail0

fin0@run5:
   SUBROUTINE

   STA   __zpabi_mix__v_2
   JMP   fin0@run0

fin0@run4:
   SUBROUTINE

   STA   __zpabi_mix__v_1
   LDA   #$00
   STA   __zpabi_mix__v_2
   JMP   fin0@run0

fin0@run3:
   SUBROUTINE

   STA   __zpabi_mix__v_3
   LDA   g1
   STA   __zpabi_mix__v_0
   LDA   g1+1
   STA   __zpabi_mix__v_1
   JMP   mix

fin0@run0:
   SUBROUTINE

   STA   __zpabi_mix__v_3
   JMP   mix

fin0@tail0:
   SUBROUTINE

   STA   __zpabi_mix__v_1
   LDA   #$00
   STA   __zpabi_mix__v_2
   JMP   fin0@tail0@tail1

fin0@tail0@tail1:
   SUBROUTINE

   STA   __zpabi_mix__v_3
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
   JSR   fin0@run0
   LDA   z6
   STA   __zpabi_mix__v_0
   JSR   fin1@run2
   LDA   z7+1
   BMI   .sx_neg@4
   LDA   #$00
   BEQ   .sx_done@5
.sx_neg@4:
   LDA   #$FF
.sx_done@5:
   STA   __zpabi_mix__v_2
   STA   __zpabi_mix__v_3
   LDA   z7
   STA   __zpabi_mix__v_0
   LDA   z7+1
   STA   __zpabi_mix__v_1
   JSR   mix
   LDA   ga8
   STA   __zpabi_mix__v_0
   LDA   ga8+1
   STA   __zpabi_mix__v_1
   LDA   ga8+2
   STA   __zpabi_mix__v_2
   LDA   ga8+3
   JMP   fin0@tail0@tail1

fin1@run2:
   SUBROUTINE

   LDA   #$00
   STA   __zpabi_mix__v_1
   STA   __zpabi_mix__v_2
   JMP   fin0@run0

fin2:
   SUBROUTINE

   LDA   ga8+4
   STA   __zpabi_mix__v_0
   LDA   ga8+5
   STA   __zpabi_mix__v_1
   LDA   ga8+6
   STA   __zpabi_mix__v_2
   LDA   ga8+7
   JSR   fin0@run0
   LDA   ga8+8
   STA   __zpabi_mix__v_0
   LDA   ga8+9
   STA   __zpabi_mix__v_1
   LDA   ga8+10
   STA   __zpabi_mix__v_2
   LDA   ga8+11
   JSR   fin0@run0
   LDA   ga8+12
   STA   __zpabi_mix__v_0
   LDA   ga8+13
   STA   __zpabi_mix__v_1
   LDA   ga8+14
   STA   __zpabi_mix__v_2
   LDA   ga8+15
   JSR   fin0@run0
   LDA   gs9+1
   STA   __zpabi_mix__v_0
   LDA   #$00
   STA   __zpabi_mix__v_1
   STA   __zpabi_mix__v_2
   JMP   fin0@tail0@tail1

fin3:
   SUBROUTINE

   LDA   gs9+2
   STA   __zpabi_mix__v_0
   LDA   gs9+3
   JMP   fin0@tail0

f12:
   SUBROUTINE

   LDA   __zpabi_f12__p15_0
   CLC
   ADC   #$01
   AND   #$07
   CMP   #$02
   BEQ   .case@1
   CMP   #$04
   BEQ   .case@2
   CMP   #$06
   BNE   .lb_skip@1
   JMP   .switch@0_break
.lb_skip@1:
   JMP   .default@4
.case@1:
   LDA   g1+1
   BMI   .sx_neg@6
   LDA   #$00
   BEQ   .sx_done@7
.sx_neg@6:
   LDA   #$FF
.sx_done@7:
   STA   __local_f12__0
   CLC
   ADC   #$01
   STA   __zpabi_mix__v_2
   LDA   __local_f12__0
   ADC   #$00
   JSR   fin0@run3
   JMP   .switch@0_break
.case@2:
   LDA   #$80
   STA   __zpabi_mix__v_0
   JSR   fin1@run2
   JMP   .switch@0_break
.default@4:
   LDA   #$01
   STA   __zpabi_mix__v_0
   JSR   fin1@run2
   LDA   gs9+2
   STA   __local_f12__0
   LDA   gs9+3
   STA   __local_f12__1
   INC   __local_f12__0
   BNE   .inc_done@0
   INC   __local_f12__1
.inc_done@0:
   LDA   __local_f12__0
   STA   gs9+2
   LDA   __local_f12__1
   STA   gs9+3
   LDA   g1
   CLC
   ADC   __local_f12__0
   STA   __zpabi_mix__v_0
   LDA   g1+1
   ADC   __local_f12__1
   JSR   fin0@run4
.switch@0_break:
   LDA   z6
   CLC
   ADC   #$5A
   STA   z6
   BNE   .lb_skip@0
   JMP   .if_end@0
.lb_skip@0:
   LDY   #$00
   LDA   (gp11),Y
   STA   __local_f12__0
   INY
   LDA   (gp11),Y
   STA   __local_f12__2
   INY
   LDA   (gp11),Y
   STA   __local_f12__w20
   INY
   LDA   (gp11),Y
   STA   __local_f12__1
   INC   __local_f12__0
   BNE   .inc_done@1
   INC   __local_f12__2
   BNE   .inc_done@1
   INC   __local_f12__w20
   BNE   .inc_done@1
   INC   __local_f12__1
.inc_done@1:
   LDA   __local_f12__0
   LDY   #$00
   STA   (gp11),Y
   LDA   __local_f12__2
   INY
   STA   (gp11),Y
   LDA   __local_f12__w20
   INY
   STA   (gp11),Y
   LDA   __local_f12__1
   INY
   STA   (gp11),Y
   LDA   __local_f12__0
   AND   #$07
   BEQ   .switch@5_break
   CMP   #$02
   BEQ   .case@7
   CMP   #$04
   BEQ   .switch@5_break
   CMP   #$06
   BEQ   .case@10
   BNE   .default@11
.case@7:
   LDY   #$00
   LDA   (__zpabi_f12__pp16_0),Y
   CLC
   ADC   #$FF
   STA   __zpabi_mix__v_0
   INY
   LDA   (__zpabi_f12__pp16_0),Y
   ADC   #$FF
   STA   __zpabi_mix__v_1
   INY
   LDA   (__zpabi_f12__pp16_0),Y
   ADC   #$FF
   STA   __zpabi_mix__v_2
   INY
   LDA   (__zpabi_f12__pp16_0),Y
   ADC   #$FF
   JSR   fin0@run0
   LDA   #$00
   STA   __local_f12__w20
.loop@8_start:
   LDA   g4
   STA   __local_f12__1
   LDA   g4+1
   LDA   z5
   EOR   #$81
   STA   z5
   ORA   __local_f12__1
   LDY   #$00
   STA   (gp10),Y
   INC   __local_f12__w20
   LDA   __local_f12__w20
   CMP   #$03
   BCC   .loop@8_start
.case@10:
.default@11:
.switch@5_break:
.if_end@0:
   LDX   #$00
   LDA   #$FF
   RTS

f21:
   SUBROUTINE

   LDA   #$00
   STA   __local_f21__l23
   LDY   #$00
   LDA   (gp11),Y
   STA   __local_f21__w28
   LDA   z6
   STA   HARGS
   LDA   #$5A
   STA   HARGS+1
   JSR   mul8
   LDA   HARGS+2
   STA   z6
   AND   __local_f21__w28
   STA   @10.s27
   JSR   f21@run1
   LDA   #$04
   CLC
   ADC   __local_f21__l23
   STA   __local_f21__l23
   LDA   #$00
   ADC   #$00
   STA   __local_f21__w28
   BMI   .sx_neg@8
   LDA   #$00
   BEQ   .sx_done@9
.sx_neg@8:
   LDA   #$FF
.sx_done@9:
   STA   __zpabi_mix__v_2
   STA   __zpabi_mix__v_3
   LDA   __local_f21__l23
   STA   __zpabi_mix__v_0
   LDA   __local_f21__w28
   STA   __zpabi_mix__v_1
   JSR   mix
   LDA   #$00
   STA   __local_f21__w28
.loop@12_start:
   LDA   g1
   AND   #$03
   STA   g1
   LDA   #$00
   STA   g1+1
   INC   __local_f21__w28
   LDA   __local_f21__w28
   CMP   #$02
   BCC   .loop@12_start
   LDA   ga8+12
   LDX   ga8+13
   STX   __local_f21__1
   LDX   ga8+14
   STX   __local_f21__0
   LDX   ga8+15
   STX   __local_f21__l23
   SBC   #$01
   STA   __local_f21__w28
   LDA   __local_f21__1
   SBC   #$00
   STA   __local_f21__1
   LDA   __local_f21__0
   SBC   #$00
   STA   ga8+14
   LDA   __local_f21__l23
   SBC   #$00
   STA   ga8+15
   LDA   __local_f21__w28
   STA   ga8+12
   LDA   __local_f21__1
   STA   ga8+13
   LDA   __local_f21__w28
   AND   #$07
   CMP   #$01
   BEQ   .case@14
   CMP   #$05
   BEQ   .switch@13_break
   CMP   #$07
   BEQ   .case@16
   BNE   .default@17
.case@14:
   LDA   z5
   SEC
   SBC   #$FF
   STA   z5
   LDA   z5+1
   SBC   #$00
   STA   z5+1
   LDA   z5+2
   SBC   #$00
   STA   z5+2
   BCS   .switch@13_break
   DEC   z5+3
   JMP   .switch@13_break
.case@16:
.default@17:
   LDA   #$01
   STA   @10.s27
   JSR   f21@run1
.switch@13_break:
   LDY   #$00
   LDA   (gp11),Y
   CLC
   ADC   z5
   STA   __local_f21__1
   INY
   LDA   (gp11),Y
   ADC   z5+1
   STA   __local_f21__0
   INY
   LDA   (gp11),Y
   ADC   z5+2
   STA   __local_f21__l23
   INY
   LDA   (gp11),Y
   ADC   z5+3
   STA   __local_f21__w28
   BMI   .sx_neg@10
   LDA   #$00
   BEQ   .sx_done@11
.sx_neg@10:
   LDA   #$FF
.sx_done@11:
   STA   HARGS+20
   STA   HARGS+21
   STA   HARGS+22
   STA   HARGS+23
   LDA   __local_f21__1
   STA   HARGS+16
   LDA   __local_f21__0
   STA   HARGS+17
   LDA   __local_f21__l23
   STA   HARGS+18
   LDA   __local_f21__w28
   STA   HARGS+19
   RTS

f21@run1:
   SUBROUTINE

   LDA   #$00
   STA   @10.s27+1
   STA   @10.s27+2
   STA   @10.s27+3
   STA   @10.s27+4
   STA   @10.s27+5
   STA   @10.s27+6
   STA   @10.s27+7
   RTS

f29:
   SUBROUTINE

   LDY   #$00
   LDA   (gp11),Y
   STA   __zpabi_mix__v_0
   BMI   .sx_neg@12
   LDA   #$00
   BEQ   .sx_done@13
.sx_neg@12:
   LDA   #$FF
.sx_done@13:
   STA   __zpabi_mix__v_1
   JSR   fin0@run5
   LDA   #$00
   STA   z6
   LDY   #$00
   LDA   (gp10),Y
   SEC
   SBC   #$01
   STA   __zpabi_mix__v_0
   STA   (gp10),Y
   LDA   @20.s36
   AND   __zpabi_mix__v_0
   STA   g3
   LDA   #$00
   STA   g3+1
   STA   __zpabi_mix__v_0
.loop@18_start:
   INC   __zpabi_mix__v_0
   LDA   __zpabi_mix__v_0
   CMP   #$02
   BCC   .loop@18_start
   LDX   #$00
   LDA   #$FF
   RTS

main:
   SUBROUTINE

   LDA   g4
   STA   __zpabi_mix__v_0
   LDA   g4+1
   LDA   __zpabi_mix__v_0
   STA   g4
   LDY   #$00
   STY   g4+1
   AND   #$07
   CMP   #$03
   BEQ   .case@28
   CMP   #$04
   BNE   .lb_skip@3
   JMP   .case@38
.lb_skip@3:
   CMP   #$06
   BNE   .lb_skip@2
   JMP   .case@39
.lb_skip@2:
   JMP   .default@40
.case@28:
   LDA   gs9+2
   STA   __local_main__l39
   LDA   gs9+3
   STA   __local_main__1
   LSR
   LDA   __local_main__l39
   ROR
   STA   __zpabi_mix__v_1
   TYA
   ROR
   CLC
   ADC   __local_main__l39
   STA   __zpabi_mix__v_0
   LDA   __zpabi_mix__v_1
   ADC   __local_main__1
   STA   gs9+3
   LDA   __zpabi_mix__v_0
   STA   gs9+2
   LDY   #$00
   STA   (gp10),Y
   LDA   #$B5
   STA   __local_main__l39
   LDA   #$00
   STA   __zpabi_mix__v_0
   JMP   .loop@35_continue
.main@rot@0:
   LDA   z7
   AND   #$01
   EOR   #$81
   STA   z7
   LDA   #$00
   STA   z7+1
   LDA   __local_main__1
   EOR   z7
   BEQ   .if_end@5
   DEC   __local_main__l39
   BMI   .sx_neg@14
   LDA   #$00
   BEQ   .sx_done@15
.sx_neg@14:
   LDA   #$FF
.sx_done@15:
   STA   __zpabi_mix__v_1
   LDA   #$80
   ORA   __local_main__l39
   STA   __zpabi_mix__v_0
   LDA   __zpabi_mix__v_1
   BMI   .sx_neg@16
   LDA   #$00
   BEQ   .sx_done@17
.sx_neg@16:
   LDA   #$FF
.sx_done@17:
   JSR   fin0@run5
.if_end@5:
   DEC   gs9+1
   LDA   __local_main__1
   STA   __zpabi_mix__v_0
.loop@35_continue:
   CLC
   ADC   #$01
   STA   __local_main__1
   LDA   __zpabi_mix__v_0
   CMP   #$04
   BCC   .main@rot@0
.case@38:
.case@39:
.default@40:
   JSR   fin0
   JSR   fin1
   JSR   fin2
   JSR   fin3
   LDA   acc+1
   AND   #$7F
   TAX
   LDA   acc
   RTS

acc:
   DS.B  2

g2:
   DC.W  $00FF

g3:
   DC.W  $007F

ga8:
   DC.L  $00008000
   DC.L  $00000100
   DS.B  4
   DC.L  $00008000

gs9:
   DS.B  1
   DC.B  $01
   DC.W  $6915

@10.s27:
   DC.L  $0000007F
   DC.L  $00000000

__zp_init:
   SUBROUTINE

   LDA   #$00
   STA   g1+1
   STA   z5+1
   STA   z5+2
   STA   z5+3
   STA   z7
   STA   z7+1
   LDA   #$FF
   STA   g1
   STA   @20.s36
   LDA   #$E8
   STA   g4
   LDA   #$BF
   STA   g4+1
   LDA   #$01
   STA   z5
   LDA   #$80
   STA   z6
   LDA   #<z6
   STA   gp10
   LDA   #>z6
   STA   gp10+1
   LDA   #<z5
   STA   gp11
   LDA   #>z5
   STA   gp11+1
   RTS
