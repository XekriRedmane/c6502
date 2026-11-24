SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
g2	EQU	$26
g0	EQU	$27
g1	EQU	$29
__local_f1__0	EQU	$80
__local_main__0	EQU	$80
__zpabi_f3__p0_0	EQU	$80
__local_f1__1	EQU	$81
__local_main__1	EQU	$81
__zpabi_f3__p0_1	EQU	$81
__local_f1__it_1	EQU	$82
__local_main__2	EQU	$82
__zpabi_f3__p0_2	EQU	$82
__local_f1__it_0	EQU	$83
__local_main__3	EQU	$83
__zpabi_f3__p0_3	EQU	$83
__local_f1__l0	EQU	$84
__local_main__4	EQU	$84
__zpabi_f3__p1_0	EQU	$84
__local_f1__2	EQU	$85
__zpabi_f3__p1_1	EQU	$85
__local_f1__3	EQU	$86
__zpabi_f3__p1_2	EQU	$86
__local_f1__box5	EQU	$87
__zpabi_f3__p1_3	EQU	$87
__zpabi_f3__p2_0	EQU	$88
__zpabi_f3__p2_1	EQU	$89
__zpabi_f3__p2_2	EQU	$8A
__local_f0__0	EQU	$8B
__local_f2__0	EQU	$8B
__zpabi_f3__p2_3	EQU	$8B
__local_f0__1	EQU	$8C
__local_f2__1	EQU	$8C
__local_f3__0	EQU	$8C
__local_f0__it_1	EQU	$8D
__local_f2__2	EQU	$8D
__local_f3__1	EQU	$8D
__local_f0__it_0	EQU	$8E
__local_f2__3	EQU	$8E
__local_f3__2	EQU	$8E
__local_f0__l0_1	EQU	$8F
__local_f3__3	EQU	$8F
__local_f0__l0_0	EQU	$90
__local_f2__5	EQU	$90
__local_f3__4	EQU	$90
__local_f0__p0	EQU	$91
__local_f2__6	EQU	$91
__local_f3__5	EQU	$91
__local_f0__2	EQU	$92
__local_f2__7	EQU	$92
__local_f3__6	EQU	$92
__local_f3__7	EQU	$93
__local_f3__10	EQU	$96
__local_f3__11	EQU	$97
__local_f3__box5	EQU	$98
__local_f4__box5	EQU	$98
__local_f5__0	EQU	$9E
__local_f5__1	EQU	$9F
__local_f5__2	EQU	$A0
__local_f5__3	EQU	$A1
__local_f5__4	EQU	$A2
__local_f5__pp	EQU	$A4
__local_f5__6	EQU	$A5

; @zp-link-meta-begin
; def f0 params=__zpabi_f0__p0 locals=__local_f0__0,__local_f0__1,__local_f0__it_1,__local_f0__it_0,__local_f0__l0_1,__local_f0__l0_0,__local_f0__p0,__local_f0__2,__local_f0__3,__local_f0__4,__local_f0__5 indirect=false in_cycle=false param_regs=A
; def f1 params= locals=__local_f1__0,__local_f1__1,__local_f1__it_1,__local_f1__it_0,__local_f1__l0,__local_f1__2,__local_f1__3,__local_f1__box5,__local_f1__5,__local_f1__6,__local_f1__7 indirect=false in_cycle=false carry=true
; def f2 params= locals=__local_f2__0,__local_f2__1,__local_f2__2,__local_f2__3,__local_f2__4,__local_f2__5,__local_f2__6,__local_f2__7 indirect=false in_cycle=false
; def f3 params=__zpabi_f3__p0_0,__zpabi_f3__p0_1,__zpabi_f3__p0_2,__zpabi_f3__p0_3,__zpabi_f3__p1_0,__zpabi_f3__p1_1,__zpabi_f3__p1_2,__zpabi_f3__p1_3,__zpabi_f3__p2_0,__zpabi_f3__p2_1,__zpabi_f3__p2_2,__zpabi_f3__p2_3 locals=__local_f3__0,__local_f3__1,__local_f3__2,__local_f3__3,__local_f3__4,__local_f3__5,__local_f3__6,__local_f3__7,__local_f3__8,__local_f3__9,__local_f3__10,__local_f3__11,__local_f3__box5,__local_f3__13,__local_f3__14,__local_f3__15,__local_f3__16,__local_f3__17 indirect=false in_cycle=false
; def f4 params= locals=__local_f4__0,__local_f4__1,__local_f4__box5,__local_f4__3 indirect=false in_cycle=false
; def f5 params=__zpabi_f5__pp_0,__zpabi_f5__pp_1 locals=__local_f5__0,__local_f5__1,__local_f5__2,__local_f5__3,__local_f5__4,__local_f5__5,__local_f5__pp,__local_f5__6,__local_f5__7,__local_f5__8,__local_f5__9 indirect=false in_cycle=false param_regs=A,X
; def main params= locals=__local_main__0,__local_main__1,__local_main__2,__local_main__3,__local_main__4,__local_main__5,__local_main__6,__local_main__7,__local_main__8,__local_main__9,__local_main__10 indirect=false in_cycle=false
; call f0 -> f4
; call f1 -> f2
; call f1 -> f5
; call f3 -> f5
; call f4 -> f5
; call main -> f0
; item f5 section=.text local=false zero=false
; item f5@run0 section=.text local=true zero=false
; item f4 section=.text local=false zero=false
; item f3 section=.text local=false zero=false
; item f2 section=.text local=false zero=false
; item f1 section=.text local=false zero=false
; item f0 section=.text local=false zero=false
; item main section=.text local=false zero=false
; item __zp_init section=.text local=false zero=false
; zp g2 size=1 at=auto def=true init=$2F weight=14
; zp g0 size=2 at=auto def=true init=$73,$00 weight=18
; zp g1 size=4 at=auto def=true init=$96,$00,$00,$00 weight=13
; runtime hargs_size=32 hargs_need=22
; @zp-link-meta-end

f5:
   SUBROUTINE

   STA   __local_f5__pp
   STX   __local_f5__6
   LDA   g2
   ORA   #$01
   STA   HARGS+4
   JSR   f5@run0
   STA   HARGS+5
   LDY   #$00
   STY   HARGS+6
   STA   HARGS+7
   JSR   sdivmod32
   LDA   HARGS+8
   ORA   g0
   STA   __local_f5__4
   LDA   HARGS+9
   ORA   g0+1
   STA   __local_f5__3
   LDA   __local_f5__4
   SEC
   SBC   g0
   LDA   __local_f5__3
   SBC   #$00
   LDA   HARGS+10
   SBC   #$00
   LDA   HARGS+11
   SBC   #$00
   BVC   .cmp_novf@0
   EOR   #$80
.cmp_novf@0:
   ASL
   LDA   #$00
   ROL
   EOR   #$01
   CLC
   ADC   g1
   STA   __local_f5__1
   LDA   #$00
   ADC   g1+1
   STA   __local_f5__0
   LDY   #$00
   LDA   (__local_f5__pp),Y
   STA   __local_f5__3
   INY
   LDA   (__local_f5__pp),Y
   STA   __local_f5__2
   LDA   __local_f5__1
   ORA   #$01
   STA   HARGS+2
   LDA   __local_f5__0
   STA   HARGS+3
   LDA   g0
   STA   HARGS
   LDA   g0+1
   STA   HARGS+1
   JSR   udivmod16
   LDA   __local_f5__3
   CLC
   ADC   HARGS+4
   STA   __local_f5__1
   LDA   __local_f5__2
   ADC   HARGS+5
   LDY   #$01
   STA   (__local_f5__pp),Y
   LDA   __local_f5__1
   DEY
   STA   (__local_f5__pp),Y
   LDA   __local_f5__4
   RTS

f5@run0:
   SUBROUTINE

   LDA   g1
   STA   HARGS
   LDA   g1+1
   STA   HARGS+1
   LDA   g1+2
   STA   HARGS+2
   LDA   g1+3
   STA   HARGS+3
   LDA   #$00
   RTS

f4:
   SUBROUTINE

   LDA   g0
   CLC
   SBC   g0
   LDA   g0+1
   SBC   g0+1
   LDA   #$00
   ROL
   STA   __local_f4__box5
   BMI   .sx_neg@1
   LDA   #$00
   BEQ   .sx_done@2
.sx_neg@1:
   LDA   #$FF
.sx_done@2:
   STA   __local_f4__box5+1
   LDX   #>__local_f4__box5
   LDA   #<__local_f4__box5
   JSR   f5
   LDA   #$00
   STA   HARGS+8
   LDY   #$00
   STY   HARGS+9
   STY   HARGS+10
   STA   HARGS+11
   RTS

f3:
   SUBROUTINE

   LDA   __zpabi_f3__p1_0
   CLC
   ADC   #$03
   STA   __local_f3__11
   LDA   __zpabi_f3__p1_1
   ADC   #$00
   STA   __local_f3__10
   LDA   g1
   ORA   g1+1
   ORA   g1+2
   ORA   g1+3
   BEQ   .and_false@2
   LDA   g0
   EOR   #$FF
   STA   __local_f3__1
   LDA   g0+1
   EOR   #$FF
   ORA   __local_f3__1
   BEQ   .and_false@2
   LDA   #$01
   STA   __local_f3__2
   LDA   #$00
   STA   __local_f3__3
   JMP   .and_end@3
.and_false@2:
   LDA   #$00
   STA   __local_f3__2
   STA   __local_f3__3
.and_end@3:
   JSR   f5@run0
   STA   HARGS+4
   LDY   #$00
   STY   HARGS+5
   STY   HARGS+6
   STA   HARGS+7
   JSR   mul32
   LDA   #$00
   BPL   .sx_done@4
   LDA   #$FF
.sx_done@4:
   STA   __local_f3__1
   STA   __local_f3__0
   LDA   __zpabi_f3__p1_0
   SEC
   SBC   g2
   STA   __local_f3__5
   LDA   __zpabi_f3__p1_1
   SBC   #$00
   STA   __local_f3__4
   LDA   __zpabi_f3__p1_2
   SBC   __local_f3__1
   STA   __local_f3__1
   LDA   __zpabi_f3__p1_3
   SBC   __local_f3__0
   STA   __local_f3__0
   LDA   __local_f3__5
   ORA   #$01
   STA   HARGS+4
   LDA   __local_f3__4
   STA   HARGS+5
   LDA   __local_f3__1
   STA   HARGS+6
   LDA   __local_f3__0
   STA   HARGS+7
   LDA   HARGS+8
   STA   HARGS
   LDA   HARGS+9
   STA   HARGS+1
   LDA   HARGS+10
   STA   HARGS+2
   LDA   HARGS+11
   STA   HARGS+3
   JSR   udivmod32
   LDA   HARGS+12
   STA   __local_f3__7
   LDA   HARGS+13
   STA   __local_f3__6
   LDA   #$00
   SEC
   SBC   __zpabi_f3__p0_0
   LDA   #$00
   SBC   __zpabi_f3__p0_1
   LDA   #$00
   SBC   __zpabi_f3__p0_2
   LDA   #$00
   SBC   __zpabi_f3__p0_3
   LDA   #$00
   ROL
   STA   __local_f3__0
   LDA   #$00
   BPL   .sx_done@6
   LDA   #$FF
.sx_done@6:
   STA   HARGS+2
   STA   HARGS+3
   LDA   __zpabi_f3__p1_0
   ORA   #$01
   STA   HARGS+4
   LDA   __zpabi_f3__p1_1
   STA   HARGS+5
   LDA   __zpabi_f3__p1_2
   STA   HARGS+6
   LDA   __zpabi_f3__p1_3
   STA   HARGS+7
   LDA   __local_f3__0
   STA   HARGS
   LDA   #$00
   STA   HARGS+1
   JSR   udivmod32
   LDA   HARGS+8
   STA   __local_f3__5
   LDA   HARGS+9
   STA   __local_f3__4
   LDA   __local_f3__11
   STA   __local_f3__box5
   LDA   __local_f3__10
   STA   __local_f3__box5+1
   LDX   #>__local_f3__box5
   LDA   #<__local_f3__box5
   JSR   f5
   STA   __local_f3__1
   ORA   #$00
   BMI   .sx_neg@7
   LDA   #$00
   BEQ   .sx_done@8
.sx_neg@7:
   LDA   #$FF
.sx_done@8:
   STA   __local_f3__0
   LDA   __local_f3__5
   EOR   __local_f3__1
   STA   __local_f3__1
   LDA   __local_f3__4
   EOR   __local_f3__0
   STA   __local_f3__0
   LDA   __local_f3__1
   CLC
   ADC   __local_f3__box5
   STA   __local_f3__1
   LDA   __local_f3__0
   ADC   __local_f3__box5+1
   STA   __local_f3__0
   LDA   g2
   CLC
   ADC   __local_f3__1
   STA   g2
   LDA   __local_f3__2
   AND   #$07
   TAX
   BEQ   .shift_done@10
.shift_loop@9:
   ASL   __local_f3__2
   ROL   __local_f3__3
   DEX
   BNE   .shift_loop@9
.shift_done@10:
   LDA   __local_f3__7
   STA   HARGS
   LDA   __local_f3__6
   STA   HARGS+1
   LDA   __local_f3__1
   STA   HARGS+2
   LDA   __local_f3__0
   STA   HARGS+3
   JSR   mul16
   LDA   HARGS+4
   CLC
   ADC   __local_f3__2
   STA   __local_f3__0
   LDA   HARGS+5
   ADC   __local_f3__3
   TAX
   LDA   __local_f3__0
   RTS

f2:
   SUBROUTINE

   LDA   #$00
   BPL   .sx_done@12
   LDA   #$FF
.sx_done@12:
   STA   __local_f2__1
   STA   __local_f2__0
   LDA   g2
   SEC
   SBC   g1
   LDA   #$00
   SBC   g1+1
   LDA   __local_f2__1
   SBC   g1+2
   LDA   __local_f2__0
   SBC   g1+3
   BVC   .cmp_novf@13
   EOR   #$80
.cmp_novf@13:
   ASL
   LDA   #$00
   ROL
   EOR   #$01
   STA   __local_f2__6
   LDA   #$00
   BPL   .sx_done@15
   LDA   #$FF
.sx_done@15:
   STA   __local_f2__5
   LDA   #$00
   BPL   .sx_done@17
   LDA   #$FF
.sx_done@17:
   STA   __local_f2__1
   STA   __local_f2__0
   LDA   g1
   ORA   g2
   STA   __local_f2__3
   LDA   g1+2
   ORA   __local_f2__1
   STA   __local_f2__1
   LDA   g1+3
   ORA   __local_f2__0
   STA   __local_f2__0
   LDA   __local_f2__3
   ORA   #$01
   STA   HARGS+4
   LDA   g1+1
   STA   HARGS+5
   LDA   __local_f2__1
   STA   HARGS+6
   LDA   __local_f2__0
   STA   HARGS+7
   LDA   __local_f2__6
   STA   HARGS
   LDA   #$00
   STA   HARGS+1
   LDA   __local_f2__5
   STA   HARGS+2
   STA   HARGS+3
   JSR   sdivmod32
   LDA   g2
   CLC
   ADC   HARGS+12
   STA   __local_f2__7
   LDA   #$00
   ADC   #$00
   STA   __local_f2__6
   BMI   .sx_neg@18
   LDA   #$00
   BEQ   .sx_done@19
.sx_neg@18:
   LDA   #$FF
.sx_done@19:
   STA   __local_f2__5
   LDA   #$00
   BPL   .sx_done@21
   LDA   #$FF
.sx_done@21:
   STA   __local_f2__1
   STA   __local_f2__0
   LDA   g2
   SEC
   SBC   g1
   STA   __local_f2__3
   LDA   #$00
   SBC   g1+1
   STA   __local_f2__2
   LDA   __local_f2__1
   SBC   g1+2
   STA   __local_f2__1
   LDA   __local_f2__0
   SBC   g1+3
   STA   __local_f2__0
   LDA   __local_f2__3
   ORA   #$01
   STA   HARGS+4
   LDA   __local_f2__2
   STA   HARGS+5
   LDA   __local_f2__1
   STA   HARGS+6
   LDA   __local_f2__0
   STA   HARGS+7
   LDA   __local_f2__7
   STA   HARGS
   LDA   __local_f2__6
   STA   HARGS+1
   LDA   __local_f2__5
   STA   HARGS+2
   STA   HARGS+3
   JMP   sdivmod32

f1:
   SUBROUTINE

   LDA   g0
   AND   #$07
   BNE   .or_true@4
   LDA   g1
   SEC
   SBC   g0
   LDA   g1+1
   SBC   g0+1
   LDA   g1+2
   SBC   #$00
   LDA   g1+3
   SBC   #$00
   BVC   .jcmp_novf@22
   EOR   #$80
.jcmp_novf@22:
   BMI   .or_true@4
   LDA   #$00
   STA   __local_f1__l0
   JMP   .or_end@5
.or_true@4:
   LDA   #$01
   STA   __local_f1__l0
.or_end@5:
   LDA   g0
   ASL
   STA   __local_f1__2
   LDA   g0+1
   ROL
   STA   __local_f1__3
   LDA   __local_f1__l0
   STA   __local_f1__box5
   BMI   .sx_neg@23
   LDA   #$00
   BEQ   .sx_done@24
.sx_neg@23:
   LDA   #$FF
.sx_done@24:
   STA   __local_f1__box5+1
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
   BVC   .jcmp_novf@25
   EOR   #$80
.jcmp_novf@25:
   BPL   .loop@0_break
   LDX   #>__local_f1__box5
   LDA   #<__local_f1__box5
   JSR   f5
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
   CLC
   ADC   __local_f1__box5
   STA   __local_f1__l0
   JSR   f2
   LDA   __local_f1__2
   EOR   HARGS+8
   STA   __local_f1__it_0
   LDA   __local_f1__3
   EOR   HARGS+9
   STA   __local_f1__it_1
   LDA   #$00
   EOR   HARGS+10
   STA   __local_f1__1
   LDA   #$00
   EOR   HARGS+11
   STA   __local_f1__0
   LDA   g1
   CLC
   ADC   __local_f1__it_0
   STA   g1
   LDA   g1+1
   ADC   __local_f1__it_1
   STA   g1+1
   LDA   g1+2
   ADC   __local_f1__1
   STA   g1+2
   LDA   g1+3
   ADC   __local_f1__0
   STA   g1+3
   LDA   __local_f1__l0
   BNE   .or_true@8
   LDA   g2
   BNE   .or_true@8
   CLC
   RTS
.or_true@8:
   SEC
   RTS

f0:
   SUBROUTINE

   STA   __local_f0__p0
   LDA   g0+1
   LSR
   LDA   g0
   ROR
   STA   __local_f0__it_0
   LDA   #$00
   ROR
   STA   __local_f0__it_1
   LDA   __local_f0__p0
   BNE   .or_true@10
   LDA   #$00
   STA   __local_f0__1
   JMP   .or_end@11
.or_true@10:
   LDA   #$01
   STA   __local_f0__1
.or_end@11:
   LDA   __local_f0__it_1
   SEC
   SBC   __local_f0__1
   STA   __local_f0__l0_0
   LDA   __local_f0__it_0
   SBC   #$00
   STA   __local_f0__l0_1
   LDA   __local_f0__p0
   BMI   .sx_neg@26
   LDA   #$00
   BEQ   .sx_done@27
.sx_neg@26:
   LDA   #$FF
.sx_done@27:
   STA   __local_f0__it_1
   LDA   __local_f0__p0
   AND   #$07
   TAX
   LDA   g0
   STA   __local_f0__0
   LDA   g0+1
   STA   __local_f0__1
   CPX   #$00
   BEQ   .shift_done@29
.shift_loop@28:
   ASL   __local_f0__0
   ROL   __local_f0__1
   DEX
   BNE   .shift_loop@28
.shift_done@29:
   LDA   __local_f0__0
   SEC
   SBC   __local_f0__p0
   STA   __local_f0__2
   LDA   __local_f0__1
   SBC   __local_f0__it_1
   STA   __local_f0__p0
   LDA   #$00
   STA   __local_f0__it_0
   LDY   #$00
   STY   __local_f0__it_1
.loop@1_start:
   LDA   __local_f0__it_0
   SEC
   SBC   #$03
   LDA   __local_f0__it_1
   SBC   #$00
   BVC   .jcmp_novf@30
   EOR   #$80
.jcmp_novf@30:
   BPL   .loop@1_break
   JSR   f4
   LDA   HARGS+8
   CLC
   ADC   __local_f0__it_0
   STA   __local_f0__1
   LDA   HARGS+9
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
   BNE   .loop@1_start
   INC   __local_f0__it_1
   JMP   .loop@1_start
.loop@1_break:
   LDA   g2
   CLC
   ADC   __local_f0__l0_0
   STA   g2
   LDA   __local_f0__2
   ORA   __local_f0__p0
   BEQ   .and_false@12
   LDA   #$01
   STA   __local_f0__1
   JMP   .and_end@13
.and_false@12:
   LDA   #$00
   STA   __local_f0__1
.and_end@13:
   LDA   g0
   AND   __local_f0__1
   STA   HARGS+8
   LDA   #$00
   STA   HARGS+9
   STA   HARGS+10
   STA   HARGS+11
   RTS

main:
   SUBROUTINE

   LDA   #$13
   JSR   f0
   LDA   g0
   EOR   g1
   STA   __local_main__3
   LDA   g0+1
   EOR   g1+1
   STA   __local_main__2
   LDA   #$00
   EOR   g1+2
   STA   __local_main__1
   LDA   #$00
   EOR   g1+3
   STA   __local_main__4
   LDA   __local_main__3
   EOR   g2
   STA   __local_main__3
   LDA   __local_main__4
   STA   __local_main__0
   LDA   HARGS+8
   EOR   __local_main__3
   STA   __local_main__3
   LDA   HARGS+9
   EOR   __local_main__2
   STA   __local_main__2
   LDA   HARGS+10
   EOR   __local_main__1
   STA   HARGS+2
   LDA   HARGS+11
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
   STA   g1+1
   STA   g1+2
   STA   g1+3
   LDA   #$73
   STA   g0
   LDA   #$96
   STA   g1
   LDA   #$2F
   STA   g2
   RTS
