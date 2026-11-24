SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
g0	EQU	$26
g2	EQU	$27
__local_f1__0	EQU	$80
__zpabi_f0__p0_0	EQU	$80
__zpabi_f0__p0_1	EQU	$81
__zpabi_f0__p0_2	EQU	$82
__zpabi_f0__p1_0	EQU	$83
__zpabi_f0__p1_1	EQU	$84
__zpabi_f0__p1_2	EQU	$85
__zpabi_f0__p1_3	EQU	$86
__local_f3__0	EQU	$87
__zpabi_f2__p0_0	EQU	$87
__local_f3__1	EQU	$88
__zpabi_f2__p0_1	EQU	$88
__local_f3__2	EQU	$89
__zpabi_f2__p0_2	EQU	$89
__zpabi_f2__pp_0	EQU	$8A
__local_f3__4	EQU	$8B
__zpabi_f2__pp_1	EQU	$8B
__local_main__0	EQU	$8C
__local_main__1	EQU	$8D
__local_main__2	EQU	$8E
__local_main__3	EQU	$8F
__local_main__4	EQU	$90
__local_main__5	EQU	$91
__local_main__6	EQU	$92
__local_main__7	EQU	$93
__local_main__8	EQU	$94
__local_f0__0	EQU	$97
__local_f0__1	EQU	$98
__local_f0__it_1	EQU	$99
__local_f0__it_0	EQU	$9A
__local_f0__l0_1	EQU	$9B
__local_f0__l0_0	EQU	$9C
__local_f0__box2	EQU	$9D
__local_f2__0	EQU	$A2
__local_f2__1	EQU	$A3
__local_f2__2	EQU	$A4
__local_f2__3	EQU	$A5
__local_f2__4	EQU	$A6
__local_f2__5	EQU	$A7
__local_f2__p1	EQU	$A8

; @zp-link-meta-begin
; def f0 params=__zpabi_f0__p0_0,__zpabi_f0__p0_1,__zpabi_f0__p0_2,__zpabi_f0__p1_0,__zpabi_f0__p1_1,__zpabi_f0__p1_2,__zpabi_f0__p1_3 locals=__local_f0__0,__local_f0__1,__local_f0__it_1,__local_f0__it_0,__local_f0__l0_1,__local_f0__l0_0,__local_f0__box2,__local_f0__3,__local_f0__4,__local_f0__5,__local_f0__6 indirect=false in_cycle=false carry=true
; def f1 params= locals=__local_f1__0,__local_f1__1,__local_f1__2,__local_f1__3,__local_f1__4 indirect=false in_cycle=false carry=true
; def f2 params=__zpabi_f2__p0_0,__zpabi_f2__p0_1,__zpabi_f2__p0_2,__zpabi_f2__p1,__zpabi_f2__pp_0,__zpabi_f2__pp_1 locals=__local_f2__0,__local_f2__1,__local_f2__2,__local_f2__3,__local_f2__4,__local_f2__5,__local_f2__p1 indirect=false in_cycle=false param_regs=,,,A,,
; def f3 params= locals=__local_f3__0,__local_f3__1,__local_f3__2,__local_f3__3,__local_f3__4 indirect=false in_cycle=false
; def main params= locals=__local_main__0,__local_main__1,__local_main__2,__local_main__3,__local_main__4,__local_main__5,__local_main__6,__local_main__7,__local_main__8,__local_main__9,__local_main__10 indirect=false in_cycle=false
; call f0 -> f2
; call f0 -> f3
; call main -> f0
; item f3 section=.text local=false zero=false
; item f2 section=.text local=false zero=false
; item f2@run0 section=.text local=true zero=false
; item f1 section=.text local=false zero=false
; item f0 section=.text local=false zero=false
; item main section=.text local=false zero=false
; item g1 section=.data local=false zero=false
; item __zp_init section=.text local=false zero=false
; zp g0 size=1 at=auto def=true init=$7B weight=9
; zp g2 size=4 at=auto def=true init=$73,$00,$00,$00 weight=12
; runtime hargs_size=32 hargs_need=20
; @zp-link-meta-end

f3:
   SUBROUTINE

   LDA   g0
   STA   __local_f3__0
   LDA   g2
   AND   #$07
   TAX
   LDA   #$00
   STA   __local_f3__1
   CPX   #$00
   BEQ   .shift_done@1
.shift_loop@0:
   ASL   __local_f3__0
   ROL   __local_f3__1
   DEX
   BNE   .shift_loop@0
.shift_done@1:
   LDA   __local_f3__0
   ORA   __local_f3__1
   BNE   .or_true@0
   LDA   g2+3
   BNE   .or_true@0
   LDA   g2+2
   BNE   .or_true@0
   LDA   g2+1
   BNE   .or_true@0
   LDA   g2
   CMP   g2
   BNE   .or_true@0
   LDA   #$00
   STA   __local_f3__4
   JMP   .or_end@1
.or_true@0:
   LDA   #$01
   STA   __local_f3__4
.or_end@1:
   LDA   g0
   BNE   .or_true@2
   LDA   __local_f3__4
   BEQ   .or_end@3
.or_true@2:
.or_end@3:
   LDA   #$00
   BPL   .sx_done@3
   LDA   #$FF
.sx_done@3:
   STA   __local_f3__1
   STA   __local_f3__0
   LDA   g2
   CLC
   SBC   g0
   LDA   g2+1
   SBC   #$00
   LDA   g2+2
   SBC   __local_f3__1
   LDA   g2+3
   SBC   __local_f3__0
   BCC   .lnot_true@4
   LDA   #$00
   BEQ   .lnot_end@5
.lnot_true@4:
   LDA   #$01
.lnot_end@5:
   STA   __local_f3__2
   LDA   #$00
   BPL   .sx_done@7
   LDA   #$FF
.sx_done@7:
   STA   __local_f3__1
   STA   __local_f3__0
   LDA   g2
   SEC
   SBC   __local_f3__4
   LDA   g2+1
   SBC   #$00
   LDA   g2+2
   SBC   __local_f3__1
   LDA   g2+3
   SBC   __local_f3__0
   LDA   #$00
   ROL
   CLC
   ADC   __local_f3__2
   STA   __local_f3__0
   LDA   #$00
   ADC   #$00
   TAX
   LDA   __local_f3__0
   RTS

f2:
   SUBROUTINE

   STA   __local_f2__p1
   LDA   __zpabi_f2__p0_2
   BMI   .sx_neg@8
   LDA   #$00
   BEQ   .sx_done@9
.sx_neg@8:
   LDA   #$FF
.sx_done@9:
   STA   __local_f2__0
   LDA   __zpabi_f2__p0_0
   SEC
   SBC   g1
   LDA   __zpabi_f2__p0_1
   SBC   g1+1
   LDA   __zpabi_f2__p0_2
   SBC   g1+2
   LDA   __local_f2__0
   SBC   g1+3
   BVC   .cmp_novf@10
   EOR   #$80
.cmp_novf@10:
   JSR   f2@run0
   AND   g0
   EOR   #$FF
   STA   __local_f2__1
   LDA   #$00
   EOR   #$FF
   STA   __local_f2__0
   LDY   #$00
   LDA   (__zpabi_f2__pp_0),Y
   CLC
   ADC   __local_f2__1
   STA   __local_f2__1
   INY
   LDA   (__zpabi_f2__pp_0),Y
   ADC   __local_f2__0
   STA   (__zpabi_f2__pp_0),Y
   LDA   __local_f2__1
   DEY
   STA   (__zpabi_f2__pp_0),Y
   SEC
   LDA   #$00
   SBC   __zpabi_f2__p0_0
   STA   __local_f2__2
   LDA   #$00
   SBC   __zpabi_f2__p0_1
   STA   __local_f2__1
   LDA   #$00
   SBC   __zpabi_f2__p0_2
   AND   #$03
   LDX   __local_f2__2
   STX   __local_f2__5
   LDX   __local_f2__1
   STX   __local_f2__4
   EOR   #$02
   SEC
   SBC   #$02
   STA   __local_f2__3
   LDA   #$64
   SEC
   SBC   __local_f2__p1
   STA   __local_f2__2
   LDA   #$00
   SBC   #$00
   STA   __local_f2__1
   BMI   .sx_neg@11
   LDA   #$00
   BEQ   .sx_done@12
.sx_neg@11:
   LDA   #$FF
.sx_done@12:
   STA   __local_f2__0
   LDA   __local_f2__5
   SEC
   SBC   __local_f2__2
   LDA   __local_f2__4
   SBC   __local_f2__1
   LDA   __local_f2__3
   SBC   __local_f2__0
   BVC   .cmp_novf@13
   EOR   #$80
.cmp_novf@13:
   JSR   f2@run0
   STA   __local_f2__0
   LDA   #$00
   BPL   .sx_done@15
   LDA   #$FF
.sx_done@15:
   STA   HARGS+10
   STA   HARGS+11
   LDA   __local_f2__0
   STA   HARGS+8
   LDA   #$00
   STA   HARGS+9
   RTS

f2@run0:
   SUBROUTINE

   ASL
   LDA   #$00
   ROL
   EOR   #$01
   RTS

f1:
   SUBROUTINE

   LDA   g2
   ORA   #$01
   STA   HARGS+4
   LDA   g2+1
   STA   HARGS+5
   LDA   g2+2
   STA   HARGS+6
   LDA   g2+3
   STA   HARGS+7
   LDA   g1
   STA   HARGS
   LDA   g1+1
   STA   HARGS+1
   LDA   g1+2
   STA   HARGS+2
   LDA   g1+3
   STA   HARGS+3
   JSR   udivmod32
   LDA   HARGS+15
   BNE   .cmp_true@16
   LDA   HARGS+14
   BNE   .cmp_true@16
   LDA   HARGS+13
   BNE   .cmp_true@16
   LDA   HARGS+12
   CMP   g0
   BNE   .cmp_true@16
   LDA   #$00
   BEQ   .cmp_end@17
.cmp_true@16:
   LDA   #$01
.cmp_end@17:
   STA   __local_f1__0
   LDA   #$07
   CLC
   SBC   __local_f1__0
   LDA   #$00
   SBC   #$00
   RTS

f0:
   SUBROUTINE

   LDA   #$00
   SEC
   SBC   __zpabi_f0__p0_0
   LDA   #$00
   SBC   __zpabi_f0__p0_1
   LDA   #$00
   SBC   __zpabi_f0__p0_2
   BVC   .cmp_novf@19
   EOR   #$80
.cmp_novf@19:
   JSR   f2@run0
   STA   __local_f0__l0_1
   LDA   #$00
   BPL   .sx_done@21
   LDA   #$FF
.sx_done@21:
   STA   HARGS+2
   STA   HARGS+3
   LDA   g1
   LDX   g1+1
   STX   __local_f0__it_1
   LDX   g1+2
   STX   __local_f0__1
   LDX   g1+3
   STX   __local_f0__0
   CLC
   ADC   g2
   STA   HARGS+4
   LDA   __local_f0__it_1
   ADC   g2+1
   STA   HARGS+5
   LDA   __local_f0__1
   ADC   g2+2
   STA   HARGS+6
   LDA   __local_f0__0
   ADC   g2+3
   STA   HARGS+7
   LDA   __local_f0__l0_1
   STA   HARGS
   LDA   #$00
   STA   HARGS+1
   JSR   mul32
   LDA   HARGS+8
   STA   __local_f0__l0_0
   STA   __local_f0__box2
   LDA   HARGS+9
   STA   __local_f0__l0_1
   STA   __local_f0__box2+1
   LDA   __zpabi_f0__p1_0
   AND   #$07
   TAX
   LDA   __local_f0__l0_0
   STA   __local_f0__it_1
   LDA   #$00
   STA   __local_f0__it_0
   CPX   #$00
   BEQ   .shift_done@23
.shift_loop@22:
   ASL   __local_f0__it_1
   ROL   __local_f0__it_0
   DEX
   BNE   .shift_loop@22
.shift_done@23:
   LDA   __local_f0__it_0
   BMI   .sx_neg@24
   LDA   #$00
   BEQ   .sx_done@25
.sx_neg@24:
   LDA   #$FF
.sx_done@25:
   STA   __zpabi_f2__p0_2
   LDA   __local_f0__it_1
   STA   __zpabi_f2__p0_0
   LDA   __local_f0__it_0
   STA   __zpabi_f2__p0_1
   LDA   #<__local_f0__box2
   STA   __zpabi_f2__pp_0
   LDA   #>__local_f0__box2
   STA   __zpabi_f2__pp_1
   LDA   g0
   JSR   f2
   LDA   __local_f0__l0_0
   EOR   HARGS+8
   STA   __local_f0__1
   LDA   __local_f0__l0_1
   EOR   HARGS+9
   STA   __local_f0__0
   LDA   __local_f0__1
   CLC
   ADC   __local_f0__box2
   STA   __local_f0__l0_0
   LDA   __local_f0__0
   ADC   __local_f0__box2+1
   STA   __local_f0__l0_1
   BMI   .sx_neg@26
   LDA   #$00
   BEQ   .sx_done@27
.sx_neg@26:
   LDA   #$FF
.sx_done@27:
   STA   __local_f0__1
   STA   __local_f0__0
   LDA   g1
   CLC
   ADC   __local_f0__l0_0
   STA   g1
   LDA   g1+1
   ADC   __local_f0__l0_1
   STA   g1+1
   LDA   g1+2
   ADC   __local_f0__1
   STA   g1+2
   LDA   g1+3
   ADC   __local_f0__0
   STA   g1+3
   LDA   #$00
   STA   __local_f0__it_0
   LDY   #$00
   STY   __local_f0__it_1
.loop@0_start:
   LDA   __local_f0__it_0
   SEC
   SBC   #$03
   LDA   __local_f0__it_1
   SBC   #$00
   BVC   .jcmp_novf@28
   EOR   #$80
.jcmp_novf@28:
   BPL   .loop@0_break
   JSR   f3
   CLC
   ADC   __local_f0__it_0
   STA   __local_f0__1
   TXA
   ADC   __local_f0__it_1
   STA   __local_f0__0
   LDA   __local_f0__l0_0
   CLC
   ADC   __local_f0__1
   STA   __local_f0__l0_0
   LDA   __local_f0__l0_1
   ADC   __local_f0__0
   STA   __local_f0__l0_1
   INC   __local_f0__it_0
   BNE   .loop@0_start
   INC   __local_f0__it_1
   JMP   .loop@0_start
.loop@0_break:
   LDA   __local_f0__l0_1
   BMI   .sx_neg@29
   LDA   #$00
   BEQ   .sx_done@30
.sx_neg@29:
   LDA   #$FF
.sx_done@30:
   STA   __local_f0__1
   STA   __local_f0__0
   LDA   g2
   CLC
   ADC   __local_f0__l0_0
   STA   g2
   LDA   g2+1
   ADC   __local_f0__l0_1
   STA   g2+1
   LDA   g2+2
   ADC   __local_f0__1
   STA   g2+2
   LDA   g2+3
   ADC   __local_f0__0
   STA   g2+3
   LDA   #$02
   EOR   __local_f0__l0_0
   BEQ   .cmp_end@31
   SEC
   RTS
.cmp_end@31:
   CMP   #$01
   RTS

main:
   SUBROUTINE

   LDA   #$D2
   STA   __zpabi_f0__p0_0
   LDA   #$1D
   STA   __zpabi_f0__p1_0
   LDA   #$00
   STA   __zpabi_f0__p0_1
   STA   __zpabi_f0__p0_2
   STA   __zpabi_f0__p1_1
   STA   __zpabi_f0__p1_2
   STA   __zpabi_f0__p1_3
   JSR   f0
   LDA   #$00
   ROL
   STA   __local_main__8
   LDA   g0
   EOR   g1
   STA   __local_main__7
   LDA   #$00
   EOR   g1+1
   STA   __local_main__6
   LDA   #$00
   EOR   g1+2
   STA   __local_main__5
   LDA   #$00
   EOR   g1+3
   STA   __local_main__4
   LDA   __local_main__7
   EOR   g2
   STA   __local_main__3
   LDA   __local_main__6
   EOR   g2+1
   STA   __local_main__2
   LDA   __local_main__5
   EOR   g2+2
   STA   __local_main__1
   LDA   __local_main__4
   EOR   g2+3
   STA   __local_main__0
   LDA   __local_main__8
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

g1:
   DC.L  $000000A2

__zp_init:
   SUBROUTINE

   LDA   #$00
   STA   g2+1
   STA   g2+2
   STA   g2+3
   LDA   #$7B
   STA   g0
   LDA   #$73
   STA   g2
   RTS
