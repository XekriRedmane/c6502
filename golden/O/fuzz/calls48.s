SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
g0	EQU	$26
__local_f2__0	EQU	$80
__zpabi_f0__p0_0	EQU	$80
__zpabi_f1__p0_0	EQU	$80
__local_f2__1	EQU	$81
__zpabi_f0__p0_1	EQU	$81
__zpabi_f1__p0_1	EQU	$81
__local_f2__2	EQU	$82
__zpabi_f0__p0_2	EQU	$82
__zpabi_f1__p0_2	EQU	$82
__zpabi_f0__p0_3	EQU	$83
__zpabi_f1__p0_3	EQU	$83
__local_f2__p0	EQU	$84
__zpabi_f1__pp_0	EQU	$84
__zpabi_f3__p1_0	EQU	$84
__zpabi_f1__pp_1	EQU	$85
__zpabi_f3__p1_1	EQU	$85
__local_f1__0	EQU	$86
__zpabi_f3__p3	EQU	$86
__local_f1__1	EQU	$87
__zpabi_f3__pp_0	EQU	$87
__local_f1__2	EQU	$88
__zpabi_f3__pp_1	EQU	$88
__local_f1__3	EQU	$89
__local_main__0	EQU	$89
__local_f1__4	EQU	$8A
__local_f1__5	EQU	$8B
__local_main__2	EQU	$8B
__local_f1__6	EQU	$8C
__local_main__3	EQU	$8C
__local_f1__7	EQU	$8D
__local_f1__8	EQU	$8E
__local_f0__0	EQU	$8F
__local_f1__9	EQU	$8F
__local_f0__1	EQU	$90
__local_f0__2	EQU	$91
__local_f0__3	EQU	$92
__local_f0__4	EQU	$93
__local_f0__5	EQU	$94
__local_f0__6	EQU	$95
__local_f0__7	EQU	$96
__local_f0__box3	EQU	$97
__local_f3__0	EQU	$9C
__local_f3__1	EQU	$9D
__local_f3__2	EQU	$9E
__local_f3__3	EQU	$9F
__local_f3__it_1	EQU	$A0
__local_f3__it_0	EQU	$A1
__local_f3__l2_3	EQU	$A2
__local_f3__l2_2	EQU	$A3
__local_f3__l2_1	EQU	$A4
__local_f4__0	EQU	$A7
__local_f4__2	EQU	$A9
__local_f4__4	EQU	$AB
__local_f4__p0_1	EQU	$AC
__local_f4__p0_0	EQU	$AD

; @zp-link-meta-begin
; def f0 params=__zpabi_f0__p0_0,__zpabi_f0__p0_1,__zpabi_f0__p0_2,__zpabi_f0__p0_3 locals=__local_f0__0,__local_f0__1,__local_f0__2,__local_f0__3,__local_f0__4,__local_f0__5,__local_f0__6,__local_f0__7,__local_f0__box3,__local_f0__9,__local_f0__10,__local_f0__11,__local_f0__12 indirect=false in_cycle=false
; def f1 params=__zpabi_f1__p0_0,__zpabi_f1__p0_1,__zpabi_f1__p0_2,__zpabi_f1__p0_3,__zpabi_f1__pp_0,__zpabi_f1__pp_1 locals=__local_f1__0,__local_f1__1,__local_f1__2,__local_f1__3,__local_f1__4,__local_f1__5,__local_f1__6,__local_f1__7,__local_f1__8,__local_f1__9 indirect=false in_cycle=false
; def f2 params=__zpabi_f2__p0 locals=__local_f2__0,__local_f2__1,__local_f2__2,__local_f2__3,__local_f2__p0,__local_f2__4 indirect=false in_cycle=false param_regs=A
; def f3 params=__zpabi_f3__p0,__zpabi_f3__p1_0,__zpabi_f3__p1_1,__zpabi_f3__p2,__zpabi_f3__p3,__zpabi_f3__pp_0,__zpabi_f3__pp_1 locals=__local_f3__0,__local_f3__1,__local_f3__2,__local_f3__3,__local_f3__it_1,__local_f3__it_0,__local_f3__l2_3,__local_f3__l2_2,__local_f3__l2_1,__local_f3__l2_0,__local_f3__4 indirect=false in_cycle=false param_regs=A,,,X,,, changes=__zpabi_f3__p1_0
; def f4 params=__zpabi_f4__p0_0,__zpabi_f4__p0_1 locals=__local_f4__0,__local_f4__1,__local_f4__2,__local_f4__3,__local_f4__4,__local_f4__p0_1,__local_f4__p0_0,__local_f4__5,__local_f4__6,__local_f4__7 indirect=false in_cycle=false param_regs=A,X
; def main params= locals=__local_main__0,__local_main__1,__local_main__2,__local_main__3,__local_main__4,__local_main__5 indirect=false in_cycle=false
; call f0 -> f3
; call f2 -> f4
; call f3 -> f4
; call main -> f0
; item f4 section=.text local=false zero=false
; item f4@run1 section=.text local=true zero=false
; item f4@run0 section=.text local=true zero=false
; item f3 section=.text local=false zero=false
; item f2 section=.text local=false zero=false
; item f1 section=.text local=false zero=false
; item f0 section=.text local=false zero=false
; item main section=.text local=false zero=false
; item __zp_init section=.text local=false zero=false
; zp g0 size=4 at=auto def=true init=$B6,$00,$00,$00 weight=18
; runtime hargs_size=32 hargs_need=22
; @zp-link-meta-end

f4:
   SUBROUTINE

   STA   __local_f4__p0_0
   STX   __local_f4__p0_1
   LDA   g0+3
   BNE   .cmp_true@0
   LDA   g0+2
   BNE   .cmp_true@0
   LDA   g0+1
   CMP   __local_f4__p0_1
   BNE   .cmp_true@0
   LDA   g0
   CMP   __local_f4__p0_0
   BNE   .cmp_true@0
   LDA   #$00
   BEQ   .cmp_end@1
.cmp_true@0:
   LDA   #$01
.cmp_end@1:
   STA   __local_f4__4
   LDA   __local_f4__p0_0
   CLC
   SBC   g0
   TXA
   JSR   f4@run0
   LDA   #$00
   ROL
   STA   __local_f4__0
   LDA   __local_f4__4
   SEC
   SBC   __local_f4__0
   LDA   #$00
   SBC   #$00
   BVC   .cmp_novf@3
   EOR   #$80
.cmp_novf@3:
   ASL
   LDA   #$00
   ROL
   EOR   #$01
   STA   __local_f4__4
   LDA   __local_f4__p0_0
   ORA   #$01
   STA   HARGS+4
   STX   HARGS+5
   LDA   #$00
   STA   HARGS+6
   LDY   #$00
   STY   HARGS+7
   STY   HARGS+1
   STY   HARGS+2
   STA   HARGS+3
   LDA   __local_f4__4
   STA   HARGS
   JSR   udivmod32
   LDA   HARGS+12
   STA   HARGS
   LDA   HARGS+13
   STA   HARGS+1
   LDA   HARGS+14
   STA   HARGS+2
   LDA   HARGS+15
   STA   HARGS+3
   LDA   g0
   EOR   #$02
   JSR   f4@run1
   LDA   g0+2
   STA   HARGS+6
   LDA   g0+3
   STA   HARGS+7
   JSR   udivmod32
   LDA   HARGS+12
   STA   __local_f4__2
   BMI   .sx_neg@4
   LDA   #$00
   BEQ   .sx_done@5
.sx_neg@4:
   LDA   #$FF
.sx_done@5:
   STA   __local_f4__0
   LDA   __local_f4__p0_0
   ORA   #$01
   STA   HARGS+2
   LDA   __local_f4__p0_1
   STA   HARGS+3
   LDA   __local_f4__4
   STA   HARGS
   LDA   #$00
   STA   HARGS+1
   JSR   udivmod16
   LDA   __local_f4__2
   SEC
   SBC   #$FF
   LDA   __local_f4__0
   SBC   #$00
   BVC   .cmp_novf@6
   EOR   #$80
.cmp_novf@6:
   ASL
   LDA   #$00
   ROL
   EOR   #$01
   CLC
   ADC   HARGS+4
   RTS

f4@run1:
   SUBROUTINE

   ORA   #$01
   STA   HARGS+4
   LDA   g0+1
   STA   HARGS+5
   RTS

f4@run0:
   SUBROUTINE

   SBC   g0+1
   LDA   #$00
   SBC   g0+2
   LDA   #$00
   SBC   g0+3
   RTS

f3:
   SUBROUTINE

   LDA   __zpabi_f3__p3
   BMI   .sx_neg@7
   LDA   #$00
   BEQ   .sx_done@8
.sx_neg@7:
   LDA   #$FF
.sx_done@8:
   STA   HARGS+3
   LDA   #$E8
   STA   HARGS
   LDA   #$03
   STA   HARGS+1
   LDA   __zpabi_f3__p3
   STA   HARGS+2
   JSR   mul16
   LDA   HARGS+5
   STA   __local_f3__it_1
   LDA   __zpabi_f3__p1_0
   SEC
   SBC   g0
   LDA   __zpabi_f3__p1_1
   JSR   f4@run0
   BVC   .cmp_novf@9
   EOR   #$80
.cmp_novf@9:
   ASL
   LDA   #$00
   ROL
   STA   __local_f3__0
   LDA   HARGS+4
   SEC
   SBC   __local_f3__0
   STA   __local_f3__0
   BCS   .carry_inc@0
   DEC   __local_f3__it_1
   SEC
.carry_inc@0:
   LDA   #$00
   SBC   __local_f3__0
   STA   __local_f3__1
   LDA   #$00
   SBC   __local_f3__it_1
   STA   __local_f3__0
   LDY   #$00
   LDA   (__zpabi_f3__pp_0),Y
   CLC
   ADC   __local_f3__1
   STA   __local_f3__1
   INY
   LDA   (__zpabi_f3__pp_0),Y
   ADC   __local_f3__0
   STA   (__zpabi_f3__pp_0),Y
   LDA   __local_f3__1
   DEY
   STA   (__zpabi_f3__pp_0),Y
   LDA   __zpabi_f3__p1_1
   STA   __local_f3__l2_1
   LDA   #$00
   STA   __local_f3__l2_2
   STA   __local_f3__l2_3
   STA   __local_f3__it_0
   STY   __local_f3__it_1
.loop@0_start:
   LDA   __local_f3__it_0
   SEC
   SBC   #$03
   LDA   __local_f3__it_1
   SBC   #$00
   BVC   .jcmp_novf@10
   EOR   #$80
.jcmp_novf@10:
   BPL   .loop@0_break
   SEC
   LDA   #$00
   ROL
   LDX   #$00
   JSR   f4
   STA   __local_f3__1
   ORA   #$00
   BMI   .sx_neg@11
   LDA   #$00
   BEQ   .sx_done@12
.sx_neg@11:
   LDA   #$FF
.sx_done@12:
   STA   __local_f3__0
   LDA   __local_f3__1
   CLC
   ADC   __local_f3__it_0
   STA   __local_f3__3
   LDA   __local_f3__0
   ADC   __local_f3__it_1
   STA   __local_f3__2
   BMI   .sx_neg@13
   LDA   #$00
   BEQ   .sx_done@14
.sx_neg@13:
   LDA   #$FF
.sx_done@14:
   STA   __local_f3__1
   STA   __local_f3__0
   LDA   __zpabi_f3__p1_0
   CLC
   ADC   __local_f3__3
   STA   __zpabi_f3__p1_0
   LDA   __local_f3__l2_1
   ADC   __local_f3__2
   STA   __local_f3__l2_1
   LDA   __local_f3__l2_2
   ADC   __local_f3__1
   STA   __local_f3__l2_2
   LDA   __local_f3__l2_3
   ADC   __local_f3__0
   STA   __local_f3__l2_3
   INC   __local_f3__it_0
   BNE   .loop@0_start
   INC   __local_f3__it_1
   JMP   .loop@0_start
.loop@0_break:
   LDA   g0
   CLC
   ADC   __zpabi_f3__p1_0
   STA   g0
   LDA   g0+1
   ADC   __local_f3__l2_1
   STA   g0+1
   LDA   g0+2
   ADC   __local_f3__l2_2
   STA   g0+2
   LDA   g0+3
   ADC   __local_f3__l2_3
   STA   g0+3
   LDY   #$01
   LDA   (__zpabi_f3__pp_0),Y
   DEY
   ORA   (__zpabi_f3__pp_0),Y
   BEQ   .lnot_true@15
   LDA   #$00
   BEQ   .lnot_end@16
.lnot_true@15:
   LDA   #$01
.lnot_end@16:
   STA   __local_f3__0
   LDA   #$00
   BPL   .sx_done@18
   LDA   #$FF
.sx_done@18:
   STA   HARGS+10
   STA   HARGS+11
   LDA   __local_f3__0
   STA   HARGS+8
   LDA   #$00
   STA   HARGS+9
   RTS

f2:
   SUBROUTINE

   STA   __local_f2__p0
   LDA   g0
   JSR   f4@run1
   STA   HARGS+1
   LDA   g0+2
   STA   HARGS+6
   STA   HARGS+2
   LDA   g0
   STA   HARGS
   LDA   g0+3
   STA   HARGS+7
   STA   HARGS+3
   JSR   sdivmod32
   LDA   HARGS+8
   SEC
   SBC   __local_f2__p0
   LDA   HARGS+9
   SBC   #$00
   LDA   HARGS+10
   SBC   #$00
   LDA   HARGS+11
   SBC   #$00
   BVC   .cmp_novf@19
   EOR   #$80
.cmp_novf@19:
   ASL
   LDA   #$00
   ROL
   STA   __local_f2__2
   LDA   __local_f2__p0
   AND   #$07
   TAX
   LDA   __local_f2__2
   STA   __local_f2__0
   CPX   #$00
   BEQ   .shift_done@21
.shift_loop@20:
   LSR   __local_f2__0
   DEX
   BNE   .shift_loop@20
.shift_done@21:
   LDX   #$00
   LDA   __local_f2__0
   JSR   f4
   STA   __local_f2__1
   ORA   #$00
   BMI   .sx_neg@22
   LDA   #$00
   BEQ   .sx_done@23
.sx_neg@22:
   LDA   #$FF
.sx_done@23:
   STA   __local_f2__0
   LDA   __local_f2__2
   EOR   __local_f2__1
   STA   HARGS+8
   LDA   #$00
   EOR   __local_f2__0
   STA   HARGS+9
   LDA   #$00
   STA   HARGS+10
   STA   HARGS+11
   RTS

f1:
   SUBROUTINE

   LDA   g0
   ORA   g0
   STA   __local_f1__1
   LDA   g0+1
   ORA   g0+1
   STA   __local_f1__9
   LDA   g0+2
   ORA   g0+2
   STA   __local_f1__8
   LDA   g0+3
   ORA   g0+3
   STA   __local_f1__7
   SEC
   LDA   #$00
   SBC   __zpabi_f1__p0_0
   STA   __local_f1__0
   LDA   #$00
   SBC   __zpabi_f1__p0_1
   STA   __local_f1__6
   LDA   #$00
   SBC   __zpabi_f1__p0_2
   STA   __local_f1__5
   LDA   #$00
   SBC   __zpabi_f1__p0_3
   STA   __local_f1__4
   LDA   __local_f1__1
   AND   __local_f1__0
   STA   __local_f1__0
   LDA   __local_f1__9
   AND   __local_f1__6
   STA   __local_f1__1
   LDA   __local_f1__8
   AND   __local_f1__5
   STA   __local_f1__2
   LDA   __local_f1__7
   AND   __local_f1__4
   STA   __local_f1__3
   LDY   #$00
   LDA   (__zpabi_f1__pp_0),Y
   STA   __local_f1__9
   INY
   LDA   (__zpabi_f1__pp_0),Y
   STA   __local_f1__8
   LDA   __zpabi_f1__p0_3
   CMP   g0+3
   BNE   .cmp_true@24
   LDA   __zpabi_f1__p0_2
   CMP   g0+2
   BNE   .cmp_true@24
   LDA   __zpabi_f1__p0_1
   CMP   g0+1
   BNE   .cmp_true@24
   LDA   __zpabi_f1__p0_0
   CMP   g0
   BNE   .cmp_true@24
   LDA   #$00
   BEQ   .cmp_end@25
.cmp_true@24:
   LDA   #$01
.cmp_end@25:
   CLC
   ADC   __local_f1__9
   BCC   .carry_inc@1
   INC   __local_f1__8
.carry_inc@1:
   LDY   #$00
   STA   (__zpabi_f1__pp_0),Y
   LDA   __local_f1__8
   INY
   STA   (__zpabi_f1__pp_0),Y
   LDA   __zpabi_f1__p0_3
   LSR
   STA   __local_f1__7
   LDA   __zpabi_f1__p0_2
   ROR
   STA   __local_f1__6
   LDA   __zpabi_f1__p0_1
   ROR
   STA   __local_f1__5
   LDA   __zpabi_f1__p0_0
   ROR
   STA   __local_f1__4
   ASL   __local_f1__0
   ROL   __local_f1__1
   ROL   __local_f1__2
   ROL   __local_f1__3
   ASL   __local_f1__0
   ROL   __local_f1__1
   ROL   __local_f1__2
   ROL   __local_f1__3
   LDA   __local_f1__0
   CLC
   SBC   __local_f1__4
   LDA   __local_f1__1
   SBC   __local_f1__5
   LDA   __local_f1__2
   SBC   __local_f1__6
   LDA   __local_f1__3
   SBC   __local_f1__7
   LDA   #$00
   ROL
   RTS

f0:
   SUBROUTINE

   LDA   __zpabi_f0__p0_0
   SEC
   SBC   #$64
   LDA   __zpabi_f0__p0_1
   SBC   #$00
   LDA   __zpabi_f0__p0_2
   SBC   #$00
   LDA   __zpabi_f0__p0_3
   SBC   #$00
   LDA   #$00
   ROL
   STA   __local_f0__6
   LDA   #$00
   BPL   .sx_done@28
   LDA   #$FF
.sx_done@28:
   STA   __local_f0__5
   STA   __local_f0__4
   LDA   __local_f0__6
   STA   __local_f0__box3
   LDY   #$00
   STY   __local_f0__box3+1
   SEC
   SBC   __local_f0__6
   TYA
   SBC   #$00
   LDA   __local_f0__5
   SBC   __local_f0__5
   LDA   __local_f0__4
   SBC   __local_f0__4
   BVC   .cmp_novf@29
   EOR   #$80
.cmp_novf@29:
   ASL
   TYA
   ROL
   TAX
   LDA   g0
   STA   __zpabi_f3__p1_0
   LDA   g0+1
   STA   __zpabi_f3__p1_1
   LDA   #$07
   STA   __zpabi_f3__p3
   LDA   #<__local_f0__box3
   STA   __zpabi_f3__pp_0
   LDA   #>__local_f0__box3
   STA   __zpabi_f3__pp_1
   LDA   __local_f0__6
   JSR   f3
   LDA   __local_f0__6
   EOR   HARGS+8
   STA   __local_f0__7
   LDA   #$00
   EOR   HARGS+9
   STA   __local_f0__6
   LDA   __local_f0__5
   EOR   HARGS+10
   STA   __local_f0__5
   LDA   __local_f0__4
   EOR   HARGS+11
   STA   __local_f0__4
   LDA   __local_f0__box3
   STA   __local_f0__3
   LDA   __local_f0__box3+1
   STA   __local_f0__2
   BMI   .sx_neg@30
   LDA   #$00
   BEQ   .sx_done@31
.sx_neg@30:
   LDA   #$FF
.sx_done@31:
   STA   __local_f0__1
   STA   __local_f0__0
   LDA   __local_f0__7
   CLC
   ADC   __local_f0__3
   STA   __local_f0__3
   LDA   __local_f0__6
   ADC   __local_f0__2
   STA   __local_f0__2
   LDA   __local_f0__5
   ADC   __local_f0__1
   STA   __local_f0__1
   LDA   __local_f0__4
   ADC   __local_f0__0
   STA   __local_f0__0
   LDA   g0
   CLC
   ADC   __local_f0__3
   STA   g0
   LDA   g0+1
   ADC   __local_f0__2
   STA   g0+1
   LDA   g0+2
   ADC   __local_f0__1
   STA   g0+2
   LDA   g0+3
   ADC   __local_f0__0
   STA   g0+3
   LDA   #$0E
   STA   HARGS+8
   LDA   #$00
   STA   HARGS+9
   LDY   #$00
   STY   HARGS+10
   STA   HARGS+11
   RTS

main:
   SUBROUTINE

   LDA   #$BD
   STA   __zpabi_f0__p0_0
   LDA   #$00
   STA   __zpabi_f0__p0_1
   STA   __zpabi_f0__p0_2
   STA   __zpabi_f0__p0_3
   JSR   f0
   LDA   HARGS+8
   EOR   g0
   STA   __local_main__3
   LDA   HARGS+9
   EOR   g0+1
   STA   __local_main__2
   LDA   HARGS+10
   EOR   g0+2
   STA   HARGS+2
   LDA   HARGS+11
   EOR   g0+3
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
   LDA   #$B6
   STA   g0
   RTS
