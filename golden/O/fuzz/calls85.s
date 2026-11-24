SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
g2	EQU	$26
g1	EQU	$27
g0	EQU	$29
__zpabi_f0__p1_0	EQU	$80
__zpabi_f0__p1_1	EQU	$81
__zpabi_f0__p1_2	EQU	$82
__zpabi_f0__p1_3	EQU	$83
__zpabi_f0__p2_0	EQU	$84
__zpabi_f0__p2_1	EQU	$85
__zpabi_f0__p3_0	EQU	$86
__zpabi_f0__p3_1	EQU	$87
__zpabi_f0__p3_2	EQU	$88
__zpabi_f0__p3_3	EQU	$89
__zpabi_f1__p0_0	EQU	$8A
__zpabi_f1__p0_1	EQU	$8B
__zpabi_f1__p0_2	EQU	$8C
__zpabi_f1__p0_3	EQU	$8D
__zpabi_f2__p0_0	EQU	$8E
__zpabi_f2__p0_1	EQU	$8F
__zpabi_f2__p0_2	EQU	$90
__zpabi_f2__p0_3	EQU	$91
__zpabi_f2__p2	EQU	$92
__zpabi_f2__pp_0	EQU	$93
__zpabi_f2__pp_1	EQU	$94
__local_main__0	EQU	$95
__local_main__1	EQU	$96
__local_main__2	EQU	$97
__local_main__3	EQU	$98
__local_main__4	EQU	$99
__local_main__5	EQU	$9A
__local_f1__0	EQU	$9D
__local_f1__it_1	EQU	$9E
__local_f1__it_0	EQU	$9F
__local_f1__l0	EQU	$A2
__local_f1__box2	EQU	$A3
__local_f2__0	EQU	$A7
__local_f3__0	EQU	$A7
__local_f2__1	EQU	$A8
__local_f3__1	EQU	$A8
__local_f2__2	EQU	$A9
__local_f3__2	EQU	$A9
__local_f2__3	EQU	$AA
__local_f3__3	EQU	$AA
__local_f3__4	EQU	$AB
__local_f3__5	EQU	$AC
__local_f3__6	EQU	$AD
__local_f2__p1_1	EQU	$AE
__local_f3__7	EQU	$AE
__local_f2__p1_0	EQU	$AF
__local_f3__9	EQU	$B0

; @zp-link-meta-begin
; def f0 params=__zpabi_f0__p0,__zpabi_f0__p1_0,__zpabi_f0__p1_1,__zpabi_f0__p1_2,__zpabi_f0__p1_3,__zpabi_f0__p2_0,__zpabi_f0__p2_1,__zpabi_f0__p3_0,__zpabi_f0__p3_1,__zpabi_f0__p3_2,__zpabi_f0__p3_3 locals= indirect=false in_cycle=false param_regs=A,,,,,,,,,, carry=true
; def f1 params=__zpabi_f1__p0_0,__zpabi_f1__p0_1,__zpabi_f1__p0_2,__zpabi_f1__p0_3 locals=__local_f1__0,__local_f1__it_1,__local_f1__it_0,__local_f1__1,__local_f1__2,__local_f1__l0,__local_f1__box2,__local_f1__4,__local_f1__5,__local_f1__6 indirect=false in_cycle=false
; def f2 params=__zpabi_f2__p0_0,__zpabi_f2__p0_1,__zpabi_f2__p0_2,__zpabi_f2__p0_3,__zpabi_f2__p1_0,__zpabi_f2__p1_1,__zpabi_f2__p2,__zpabi_f2__pp_0,__zpabi_f2__pp_1 locals=__local_f2__0,__local_f2__1,__local_f2__2,__local_f2__3,__local_f2__4,__local_f2__5,__local_f2__6,__local_f2__p1_1,__local_f2__p1_0 indirect=false in_cycle=false param_regs=,,,,A,X,,,
; def f3 params= locals=__local_f3__0,__local_f3__1,__local_f3__2,__local_f3__3,__local_f3__4,__local_f3__5,__local_f3__6,__local_f3__7,__local_f3__8,__local_f3__9 indirect=false in_cycle=false
; def main params= locals=__local_main__0,__local_main__1,__local_main__2,__local_main__3,__local_main__4,__local_main__5,__local_main__6,__local_main__7 indirect=false in_cycle=false
; call f0 -> f1
; call f1 -> f2
; call f1 -> f3
; call main -> f0
; item f3 section=.text local=false zero=false
; item f2 section=.text local=false zero=false
; item f1 section=.text local=false zero=false
; item f1@run0 section=.text local=true zero=false
; item f0 section=.text local=false zero=false
; item main section=.text local=false zero=false
; item __zp_init section=.text local=false zero=false
; zp g2 size=1 at=auto def=true init=$76 weight=6
; zp g1 size=2 at=auto def=true init=$31,$00 weight=11
; zp g0 size=4 at=auto def=true init=$1B,$00,$00,$00 weight=12
; runtime hargs_size=32 hargs_need=22
; @zp-link-meta-end

f3:
   SUBROUTINE

   LDA   g0
   SEC
   SBC   #$E8
   LDA   g0+1
   SBC   #$03
   LDA   g0+2
   SBC   #$00
   LDA   g0+3
   SBC   #$00
   BVC   .cmp_novf@0
   EOR   #$80
.cmp_novf@0:
   ASL
   LDA   #$00
   ROL
   STA   __local_f3__2
   LDA   #$00
   BPL   .sx_done@2
   LDA   #$FF
.sx_done@2:
   STA   __local_f3__1
   STA   __local_f3__0
   LDA   g0
   AND   __local_f3__2
   STA   __local_f3__9
   LDA   g0+2
   AND   __local_f3__1
   STA   __local_f3__5
   LDA   g0+3
   AND   __local_f3__0
   STA   __local_f3__4
   LDA   g1+1
   CMP   g1+1
   BNE   .cmp_true@3
   LDA   g1
   CMP   g1
   BNE   .cmp_true@3
   LDA   #$00
   BEQ   .cmp_end@4
.cmp_true@3:
   LDA   #$01
.cmp_end@4:
   STA   __local_f3__2
   SEC
   LDA   #$00
   SBC   g2
   STA   __local_f3__1
   LDA   #$00
   SBC   #$00
   STA   __local_f3__0
   LDA   __local_f3__2
   AND   __local_f3__1
   STA   __local_f3__7
   LDA   #$00
   AND   __local_f3__0
   STA   __local_f3__6
   LDA   g1
   AND   #$07
   TAX
   LDA   g1
   STA   __local_f3__0
   LDA   g1+1
   STA   __local_f3__1
   CPX   #$00
   BEQ   .shift_done@7
.shift_loop@6:
   LDA   __local_f3__1
   ASL
   ROR   __local_f3__1
   ROR   __local_f3__0
   DEX
   BNE   .shift_loop@6
.shift_done@7:
   LDA   __local_f3__0
   ORA   __local_f3__1
   BEQ   .and_false@0
   LDA   __local_f3__6
   BMI   .sx_neg@8
   LDA   #$00
   BEQ   .sx_done@9
.sx_neg@8:
   LDA   #$FF
.sx_done@9:
   STA   HARGS+6
   STA   HARGS+7
   LDA   __local_f3__9
   STA   HARGS
   LDA   #$00
   STA   HARGS+1
   LDA   __local_f3__5
   STA   HARGS+2
   LDA   __local_f3__4
   STA   HARGS+3
   LDA   __local_f3__7
   STA   HARGS+4
   LDA   __local_f3__6
   STA   HARGS+5
   JSR   mul32
   LDA   HARGS+8
   ORA   HARGS+9
   ORA   HARGS+10
   ORA   HARGS+11
   BEQ   .and_false@0
   LDA   #$01
   STA   __local_f3__5
   LDA   #$00
   BEQ   .and_end@1
.and_false@0:
   LDA   #$00
   STA   __local_f3__5
.and_end@1:
   ORA   #$00
   BPL   .sx_done@11
   LDA   #$FF
.sx_done@11:
   STA   __local_f3__3
   STA   __local_f3__2
   LDA   __local_f3__6
   BMI   .sx_neg@12
   LDA   #$00
   BEQ   .sx_done@13
.sx_neg@12:
   LDA   #$FF
.sx_done@13:
   STA   __local_f3__1
   STA   __local_f3__0
   LDA   __local_f3__5
   EOR   __local_f3__7
   STA   HARGS
   LDA   #$00
   EOR   __local_f3__6
   STA   HARGS+1
   LDA   __local_f3__3
   EOR   __local_f3__1
   STA   HARGS+2
   LDA   __local_f3__2
   EOR   __local_f3__0
   STA   HARGS+3
   LDA   __local_f3__9
   AND   #$07
   STA   HARGS+4
   JSR   lsr32
   LDA   HARGS+6
   STA   HARGS+9
   LDA   HARGS+7
   STA   HARGS+10
   LDA   HARGS+8
   STA   HARGS+11
   LDA   HARGS+5
   STA   HARGS+8
   RTS

f2:
   SUBROUTINE

   STA   __local_f2__p1_0
   STX   __local_f2__p1_1
   LDY   #$00
   LDA   (__zpabi_f2__pp_0),Y
   STA   __local_f2__3
   INY
   LDA   (__zpabi_f2__pp_0),Y
   STA   __local_f2__2
   LDA   g0
   BMI   .sx_neg@14
   LDA   #$00
   BEQ   .sx_done@15
.sx_neg@14:
   LDA   #$FF
.sx_done@15:
   STA   __local_f2__0
   LDA   __local_f2__3
   CLC
   ADC   g0
   STA   __local_f2__1
   LDA   __local_f2__2
   ADC   __local_f2__0
   LDY   #$01
   STA   (__zpabi_f2__pp_0),Y
   LDA   __local_f2__1
   DEY
   STA   (__zpabi_f2__pp_0),Y
   LDA   g1
   ORA   #$01
   STA   __local_f2__1
   LDA   g1+1
   BMI   .sx_neg@16
   LDA   #$00
   BEQ   .sx_done@17
.sx_neg@16:
   LDA   #$FF
.sx_done@17:
   STA   HARGS+6
   STA   HARGS+7
   LDA   __zpabi_f2__p0_0
   STA   HARGS
   LDA   __zpabi_f2__p0_1
   STA   HARGS+1
   LDA   __zpabi_f2__p0_2
   STA   HARGS+2
   LDA   __zpabi_f2__p0_3
   STA   HARGS+3
   LDA   __local_f2__1
   STA   HARGS+4
   LDA   g1+1
   STA   HARGS+5
   JSR   sdivmod32
   LDA   __local_f2__p1_1
   CMP   g1+1
   BNE   .cmp_true@18
   LDA   __local_f2__p1_0
   CMP   g1
   BNE   .cmp_true@18
   LDA   #$00
   BEQ   .cmp_end@19
.cmp_true@18:
   LDA   #$01
.cmp_end@19:
   STA   __local_f2__2
   LDA   #$00
   BPL   .sx_done@22
   LDA   #$FF
.sx_done@22:
   STA   __local_f2__1
   STA   __local_f2__0
   LDA   HARGS+8
   AND   __local_f2__2
   STA   HARGS+8
   LDA   #$00
   STA   HARGS+9
   LDA   HARGS+10
   AND   __local_f2__1
   STA   HARGS+10
   LDA   HARGS+11
   AND   __local_f2__0
   STA   HARGS+11
   RTS

f1:
   SUBROUTINE

   LDA   #$00
   BPL   .sx_done@24
   LDA   #$FF
.sx_done@24:
   STA   __local_f1__it_1
   STA   __local_f1__0
   JSR   f1@run0
   LDA   __local_f1__it_1
   SBC   g0+2
   LDA   __local_f1__0
   SBC   g0+3
   BVC   .cmp_novf@25
   EOR   #$80
.cmp_novf@25:
   ASL
   LDA   #$00
   ROL
   EOR   #$01
   STA   __zpabi_f2__p0_0
   LDA   #$00
   BPL   .sx_done@27
   LDA   #$FF
.sx_done@27:
   STA   __local_f1__it_1
   STA   __local_f1__0
   JSR   f1@run0
   LDA   __local_f1__it_1
   SBC   g0+2
   LDA   __local_f1__0
   SBC   g0+3
   BVC   .cmp_novf@28
   EOR   #$80
.cmp_novf@28:
   ASL
   LDA   #$00
   ROL
   EOR   #$01
   STA   __local_f1__0
   LDA   __zpabi_f2__p0_0
   SEC
   SBC   __local_f1__0
   LDA   #$00
   SBC   #$00
   BVC   .cmp_novf@29
   EOR   #$80
.cmp_novf@29:
   ASL
   LDA   #$00
   ROL
   STA   __local_f1__l0
   LDA   #$00
   STA   __local_f1__it_0
   LDY   #$00
   STY   __local_f1__it_1
.loop@0_start:
   LDA   __local_f1__it_0
   SEC
   SBC   #$03
   LDA   __local_f1__it_1
   SBC   #$00
   BVC   .jcmp_novf@30
   EOR   #$80
.jcmp_novf@30:
   BPL   .loop@0_break
   JSR   f3
   LDA   HARGS+8
   CLC
   ADC   __local_f1__it_0
   CLC
   ADC   __local_f1__l0
   STA   __local_f1__l0
   INC   __local_f1__it_0
   BNE   .loop@0_start
   INC   __local_f1__it_1
   JMP   .loop@0_start
.loop@0_break:
   LDA   __local_f1__l0
   BMI   .sx_neg@31
   LDA   #$00
   BEQ   .sx_done@32
.sx_neg@31:
   LDA   #$FF
.sx_done@32:
   STA   __local_f1__0
   LDA   g1
   CLC
   ADC   __local_f1__l0
   STA   g1
   LDA   g1+1
   ADC   __local_f1__0
   STA   g1+1
   LDA   __local_f1__l0
   STA   __local_f1__box2
   LDA   __local_f1__0
   STA   __local_f1__box2+1
   LDA   g2
   STA   __zpabi_f2__p0_0
   LDA   g0
   AND   #$07
   TAX
   LDA   #$00
   STA   __zpabi_f2__p0_1
   CPX   #$00
   BEQ   .shift_done@34
.shift_loop@33:
   ASL   __zpabi_f2__p0_0
   ROL   __zpabi_f2__p0_1
   DEX
   BNE   .shift_loop@33
.shift_done@34:
   LDA   __zpabi_f2__p0_1
   BMI   .sx_neg@35
   LDA   #$00
   BEQ   .sx_done@36
.sx_neg@35:
   LDA   #$FF
.sx_done@36:
   STA   __zpabi_f2__p0_2
   STA   __zpabi_f2__p0_3
   LDA   g2
   STA   __zpabi_f2__p2
   LDA   #<__local_f1__box2
   STA   __zpabi_f2__pp_0
   LDA   #>__local_f1__box2
   STA   __zpabi_f2__pp_1
   LDX   #$7F
   LDA   #$FE
   JSR   f2
   LDA   __local_f1__l0
   EOR   HARGS+8
   CLC
   ADC   __local_f1__box2
   STA   __zpabi_f2__p0_0
   BMI   .sx_neg@37
   LDA   #$00
   BEQ   .sx_done@38
.sx_neg@37:
   LDA   #$FF
.sx_done@38:
   STA   __local_f1__it_0
   STA   __local_f1__it_1
   STA   __local_f1__0
   LDA   g0
   CLC
   ADC   __zpabi_f2__p0_0
   STA   g0
   LDA   g0+1
   ADC   __local_f1__it_0
   STA   g0+1
   LDA   g0+2
   ADC   __local_f1__it_1
   STA   g0+2
   LDA   g0+3
   ADC   __local_f1__0
   STA   g0+3
   LDA   __zpabi_f1__p0_0
   SEC
   STA   HARGS+8
   LDA   __zpabi_f1__p0_1
   STA   HARGS+9
   LDA   __zpabi_f1__p0_2
   STA   HARGS+10
   LDA   __zpabi_f1__p0_3
   SBC   #$00
   STA   HARGS+11
   RTS

f1@run0:
   SUBROUTINE

   LDA   g2
   SEC
   SBC   g0
   LDA   #$00
   SBC   g0+1
   RTS

f0:
   SUBROUTINE

   LDA   #$00
   STA   __zpabi_f1__p0_0
   STA   __zpabi_f1__p0_1
   STA   __zpabi_f1__p0_2
   STA   __zpabi_f1__p0_3
   JSR   f1
   SEC
   RTS

main:
   SUBROUTINE

   LDA   #$16
   STA   __zpabi_f0__p1_0
   LDA   #$01
   STA   __zpabi_f0__p1_1
   STA   __zpabi_f0__p3_0
   STA   __zpabi_f0__p3_1
   LDA   #$17
   STA   __zpabi_f0__p2_0
   LDA   #$00
   STA   __zpabi_f0__p1_2
   STA   __zpabi_f0__p1_3
   STA   __zpabi_f0__p2_1
   STA   __zpabi_f0__p3_2
   STA   __zpabi_f0__p3_3
   LDA   #$71
   JSR   f0
   LDA   #$00
   ROL
   STA   __local_main__5
   LDA   g1+1
   BMI   .sx_neg@39
   LDA   #$00
   BEQ   .sx_done@40
.sx_neg@39:
   LDA   #$FF
.sx_done@40:
   STA   __local_main__1
   STA   __local_main__0
   LDA   g0
   EOR   g1
   STA   __local_main__3
   LDA   g0+1
   EOR   g1+1
   STA   __local_main__2
   LDA   g0+2
   EOR   __local_main__1
   STA   __local_main__1
   LDA   g0+3
   EOR   __local_main__0
   STA   __local_main__4
   LDA   __local_main__3
   EOR   g2
   STA   __local_main__3
   LDA   __local_main__4
   STA   __local_main__0
   LDA   __local_main__5
   EOR   __local_main__3
   STA   __local_main__3
   LDA   #$00
   EOR   __local_main__2
   STA   __local_main__2
   LDA   #$00
   EOR   __local_main__1
   STA   HARGS+2
   LDA   #$00
   EOR   __local_main__0
   STA   HARGS+3
   LDA   __local_main__3
   STA   HARGS
   LDA   __local_main__2
   STA   HARGS+1
   LDA   #$10
   STA   HARGS+4
   JSR   asr32
   LDA   __local_main__3
   EOR   HARGS+5
   STA   __local_main__0
   LDA   __local_main__2
   EOR   HARGS+6
   TAX
   LDA   __local_main__0
   RTS

__zp_init:
   SUBROUTINE

   LDA   #$00
   STA   g0+1
   STA   g0+2
   STA   g0+3
   STA   g1+1
   LDA   #$1B
   STA   g0
   LDA   #$31
   STA   g1
   LDA   #$76
   STA   g2
   RTS
