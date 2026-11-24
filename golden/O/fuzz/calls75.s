SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
g1	EQU	$26
g0	EQU	$27
__local_f3__0	EQU	$80
__zpabi_f0__p1_0	EQU	$80
__zpabi_f1__p0_0	EQU	$80
__zpabi_f2__p0_0	EQU	$80
__local_f3__1	EQU	$81
__zpabi_f0__p1_1	EQU	$81
__zpabi_f1__p0_1	EQU	$81
__zpabi_f2__p0_1	EQU	$81
__local_f3__2	EQU	$82
__zpabi_f0__p1_2	EQU	$82
__zpabi_f2__p0_2	EQU	$82
__zpabi_f0__p1_3	EQU	$83
__zpabi_f2__p0_3	EQU	$83
__local_f3__4	EQU	$84
__local_main__0	EQU	$84
__zpabi_f4__p0_0	EQU	$84
__local_f3__5	EQU	$85
__local_main__1	EQU	$85
__zpabi_f4__p0_1	EQU	$85
__local_f3__box5	EQU	$86
__local_main__2	EQU	$86
__zpabi_f4__p0_2	EQU	$86
__local_main__3	EQU	$87
__zpabi_f4__p0_3	EQU	$87
__zpabi_f4__p1_0	EQU	$88
__zpabi_f4__p1_1	EQU	$89
__zpabi_f4__p3	EQU	$8A
__zpabi_f4__pp_0	EQU	$8B
__zpabi_f4__pp_1	EQU	$8C
__zpabi_f2__p1_0	EQU	$8D
__zpabi_f4__p2_0	EQU	$8D
__zpabi_f2__p1_1	EQU	$8E
__zpabi_f4__p2_1	EQU	$8E
__zpabi_f2__p1_2	EQU	$8F
__zpabi_f4__p2_2	EQU	$8F
__local_f0__0	EQU	$90
__zpabi_f2__p1_3	EQU	$90
__zpabi_f4__p2_3	EQU	$90
__zpabi_f5__p1_0	EQU	$91
__local_f0__2	EQU	$92
__zpabi_f5__p1_1	EQU	$92
__zpabi_f5__p1_2	EQU	$93
__zpabi_f5__p1_3	EQU	$94
__zpabi_f5__pp_0	EQU	$95
__zpabi_f5__pp_1	EQU	$96
__local_f2__0	EQU	$97
__local_f2__1	EQU	$98
__local_f2__5	EQU	$9C
__local_f2__box4	EQU	$9D
__local_f4__1	EQU	$A0
__local_f4__2	EQU	$A1
__local_f4__4	EQU	$A3
__local_f4__it_1	EQU	$A4
__local_f4__it_0	EQU	$A5
__local_f4__l3	EQU	$A6
__local_f4__box5	EQU	$A7
__local_f5__p0_1	EQU	$A9
__local_f5__p0_0	EQU	$AA
__local_f5__0	EQU	$AB

; @zp-link-meta-begin
; def f0 params=__zpabi_f0__p0,__zpabi_f0__p1_0,__zpabi_f0__p1_1,__zpabi_f0__p1_2,__zpabi_f0__p1_3 locals=__local_f0__0,__local_f0__1,__local_f0__2,__local_f0__l2 indirect=false in_cycle=false param_regs=A,,,,
; def f1 params=__zpabi_f1__p0_0,__zpabi_f1__p0_1,__zpabi_f1__p1_0,__zpabi_f1__p1_1 locals=__local_f1__0,__local_f1__1 indirect=false in_cycle=false param_regs=,,A,X carry=true
; def f2 params=__zpabi_f2__p0_0,__zpabi_f2__p0_1,__zpabi_f2__p0_2,__zpabi_f2__p0_3,__zpabi_f2__p1_0,__zpabi_f2__p1_1,__zpabi_f2__p1_2,__zpabi_f2__p1_3 locals=__local_f2__0,__local_f2__1,__local_f2__2,__local_f2__3,__local_f2__4,__local_f2__5,__local_f2__box4,__local_f2__7 indirect=false in_cycle=false carry=true
; def f3 params= locals=__local_f3__0,__local_f3__1,__local_f3__2,__local_f3__3,__local_f3__4,__local_f3__5,__local_f3__box5,__local_f3__7 indirect=false in_cycle=false
; def f4 params=__zpabi_f4__p0_0,__zpabi_f4__p0_1,__zpabi_f4__p0_2,__zpabi_f4__p0_3,__zpabi_f4__p1_0,__zpabi_f4__p1_1,__zpabi_f4__p2_0,__zpabi_f4__p2_1,__zpabi_f4__p2_2,__zpabi_f4__p2_3,__zpabi_f4__p3,__zpabi_f4__pp_0,__zpabi_f4__pp_1 locals=__local_f4__0,__local_f4__1,__local_f4__2,__local_f4__3,__local_f4__4,__local_f4__it_1,__local_f4__it_0,__local_f4__l3,__local_f4__box5,__local_f4__6 indirect=false in_cycle=false
; def f5 params=__zpabi_f5__p0_0,__zpabi_f5__p0_1,__zpabi_f5__p1_0,__zpabi_f5__p1_1,__zpabi_f5__p1_2,__zpabi_f5__p1_3,__zpabi_f5__pp_0,__zpabi_f5__pp_1 locals=__local_f5__p0_1,__local_f5__p0_0,__local_f5__0 indirect=false in_cycle=false param_regs=A,X,,,,,,
; def main params= locals=__local_main__0,__local_main__1,__local_main__2,__local_main__3,__local_main__4,__local_main__5,__local_main__6,__local_main__7,__local_main__8,__local_main__9,__local_main__10,__local_main__11 indirect=false in_cycle=false
; call f2 -> f4
; call f3 -> f5
; call f4 -> f5
; call main -> f0
; share member=f2:__zpabi_f2__p1_0,__zpabi_f2__p1_1,__zpabi_f2__p1_2,__zpabi_f2__p1_3 member=f4:__zpabi_f4__p2_0,__zpabi_f4__p2_1,__zpabi_f4__p2_2,__zpabi_f4__p2_3 live=f2:f4 forward=f2>f4
; item f5 section=.text local=false zero=false
; item f4 section=.text local=false zero=false
; item f4@run0 section=.text local=true zero=false
; item f4@tail0 section=.text local=true zero=false
; item f3 section=.text local=false zero=false
; item f2 section=.text local=false zero=false
; item f2@tail1 section=.text local=true zero=false
; item f1 section=.text local=false zero=false
; item f0 section=.text local=false zero=false
; item main section=.text local=false zero=false
; item g2 section=.data local=false zero=false
; item __zp_init section=.text local=false zero=false
; zp g1 size=1 at=auto def=true init=$BA weight=18
; zp g0 size=2 at=auto def=true init=$74,$00 weight=10
; runtime hargs_size=32 hargs_need=22
; @zp-link-meta-end

f5:
   SUBROUTINE

   STA   __local_f5__p0_0
   STX   __local_f5__p0_1
   LDA   g1
   BNE   .or_true@0
   LDA   g0
   ORA   g0+1
   BNE   .f5@cthread@0
   BEQ   .f5@ssa_block@8
.or_true@0:
   LDA   g0
   ORA   g0+1
   BEQ   .f5@jthread@0
.f5@cthread@0:
   LDA   g2
   ORA   g2+1
   ORA   g2+2
   ORA   g2+3
   BNE   .or_true@2
.f5@jthread@0:
   LDA   g1
   BEQ   .f5@ssa_block@8
   LDA   __local_f5__p0_0
   ORA   __local_f5__p0_1
   BNE   .or_true@2
.f5@ssa_block@8:
   LDA   #$00
   STA   __local_f5__0
   JMP   .or_end@3
.or_true@2:
   LDA   #$01
   STA   __local_f5__0
.or_end@3:
   LDY   #$01
   LDA   (__zpabi_f5__pp_0),Y
   STA   __local_f5__p0_0
   DEY
   LDA   (__zpabi_f5__pp_0),Y
   CLC
   ADC   __local_f5__0
   BCC   .carry_inc@0
   INC   __local_f5__p0_0
.carry_inc@0:
   LDY   #$00
   STA   (__zpabi_f5__pp_0),Y
   LDA   __local_f5__p0_0
   INY
   STA   (__zpabi_f5__pp_0),Y
   LDA   #$01
   SEC
   SBC   __zpabi_f5__p1_0
   LDA   #$00
   SBC   __zpabi_f5__p1_1
   BVC   .cmp_novf@0
   EOR   #$80
.cmp_novf@0:
   ASL
   LDA   #$00
   ROL
   STA   __local_f5__0
   LDA   #$00
   BPL   .sx_done@2
   LDA   #$FF
.sx_done@2:
   CMP   g2+3
   BNE   .cmp_true@3
   CMP   g2+2
   BNE   .cmp_true@3
   LDA   #$00
   CMP   g2+1
   BNE   .cmp_true@3
   LDA   __local_f5__0
   CMP   g2
   BNE   .cmp_true@3
   LDA   #$00
   RTS
.cmp_true@3:
   LDA   #$01
   RTS

f4:
   SUBROUTINE

   LDA   #$64
   CLC
   SBC   __zpabi_f4__p0_0
   LDA   #$00
   SBC   __zpabi_f4__p0_1
   LDA   #$00
   SBC   __zpabi_f4__p0_2
   LDA   #$00
   SBC   __zpabi_f4__p0_3
   LDA   #$00
   ROL
   ORA   g1
   STA   __local_f4__l3
   LDY   #$00
   LDA   (__zpabi_f4__pp_0),Y
   STA   __local_f4__2
   INY
   LDA   (__zpabi_f4__pp_0),Y
   STA   __local_f4__1
   LDA   #$00
   CMP   g0+1
   BNE   .cmp_true@6
   LDA   #$FF
   CMP   g0
   BNE   .cmp_true@6
   LDA   #$00
   BEQ   .cmp_end@7
.cmp_true@6:
   LDA   #$01
.cmp_end@7:
   CLC
   ADC   __local_f4__2
   BCC   .carry_inc@1
   INC   __local_f4__1
.carry_inc@1:
   LDY   #$00
   STA   (__zpabi_f4__pp_0),Y
   LDA   __local_f4__1
   INY
   STA   (__zpabi_f4__pp_0),Y
   LDA   #$01
   STA   __local_f4__box5
   LDA   #$00
   STA   __local_f4__box5+1
   STA   __local_f4__it_0
   DEY
   STY   __local_f4__it_1
.loop@0_start:
   LDA   __local_f4__it_0
   SEC
   SBC   #$03
   LDA   __local_f4__it_1
   SBC   #$00
   BVC   .jcmp_novf@9
   EOR   #$80
.jcmp_novf@9:
   BPL   .loop@0_break
   LDA   #$01
   CLC
   SBC   __zpabi_f4__p2_0
   LDA   #$00
   SBC   __zpabi_f4__p2_1
   LDA   #$00
   SBC   __zpabi_f4__p2_2
   LDA   #$00
   SBC   __zpabi_f4__p2_3
   LDA   #$00
   ROL
   STA   __local_f4__4
   LDA   __local_f4__l3
   BEQ   .and_false@12
   LDA   #$01
   STA   __zpabi_f5__p1_0
   LDA   #$00
   BEQ   .and_end@13
.and_false@12:
   LDA   #$00
   STA   __zpabi_f5__p1_0
.and_end@13:
   ORA   #$00
   BPL   .sx_done@11
   LDA   #$FF
.sx_done@11:
   JSR   f4@run0
   LDA   #<__local_f4__box5
   STA   __zpabi_f5__pp_0
   LDA   #>__local_f4__box5
   STA   __zpabi_f5__pp_1
   LDX   #$00
   LDA   __local_f4__4
   JSR   f5
   CLC
   ADC   __local_f4__it_0
   CLC
   ADC   __local_f4__l3
   STA   __local_f4__l3
   INC   __local_f4__it_0
   BNE   .loop@0_start
   INC   __local_f4__it_1
   JMP   .loop@0_start
.loop@0_break:
   LDA   __zpabi_f4__p2_0
   ORA   #$01
   STA   HARGS+4
   LDA   __zpabi_f4__p2_1
   STA   HARGS+5
   LDA   __zpabi_f4__p2_2
   STA   HARGS+6
   LDA   __zpabi_f4__p2_3
   STA   HARGS+7
   LDA   #$64
   STA   HARGS
   LDA   #$00
   STA   HARGS+1
   LDY   #$00
   STY   HARGS+2
   STA   HARGS+3
   JSR   udivmod32
   JMP   f4@tail0

f4@run0:
   SUBROUTINE

   STA   __zpabi_f5__p1_2
   STA   __zpabi_f5__p1_3
   LDA   #$00
   STA   __zpabi_f5__p1_1
   RTS

f4@tail0:
   SUBROUTINE

   LDA   HARGS+12
   STA   HARGS+8
   LDA   HARGS+13
   STA   HARGS+9
   LDA   HARGS+14
   STA   HARGS+10
   LDA   HARGS+15
   STA   HARGS+11
   RTS

f3:
   SUBROUTINE

   LDA   g1
   BMI   .sx_neg@12
   LDA   #$00
   BEQ   .sx_done@13
.sx_neg@12:
   LDA   #$FF
.sx_done@13:
   STA   __local_f3__2
   BMI   .sx_neg@14
   LDA   #$00
   BEQ   .sx_done@15
.sx_neg@14:
   LDA   #$FF
.sx_done@15:
   STA   __local_f3__1
   STA   __local_f3__0
   LDA   g1
   SEC
   SBC   g2
   LDA   __local_f3__2
   SBC   g2+1
   LDA   __local_f3__1
   SBC   g2+2
   LDA   __local_f3__0
   SBC   g2+3
   BVC   .cmp_novf@16
   EOR   #$80
.cmp_novf@16:
   ASL
   LDA   #$00
   ROL
   EOR   #$01
   STA   __local_f3__box5
   LDA   #$00
   STA   __local_f3__box5+1
   LDA   g1
   BMI   .sx_neg@17
   LDA   #$00
   BEQ   .sx_done@18
.sx_neg@17:
   LDA   #$FF
.sx_done@18:
   TAX
   LDA   g0
   ORA   g0+1
   BNE   .or_true@14
   LDA   #$00
   STA   __zpabi_f5__p1_0
   JMP   .or_end@15
.or_true@14:
   LDA   #$01
   STA   __zpabi_f5__p1_0
   LDA   #$00
.or_end@15:
   ORA   #$00
   BPL   .sx_done@20
   LDA   #$FF
.sx_done@20:
   JSR   f4@run0
   LDA   #<__local_f3__box5
   STA   __zpabi_f5__pp_0
   LDA   #>__local_f3__box5
   STA   __zpabi_f5__pp_1
   LDA   g1
   JSR   f5
   LDA   g1
   BMI   .sx_neg@21
   LDA   #$00
   BEQ   .sx_done@22
.sx_neg@21:
   LDA   #$FF
.sx_done@22:
   STA   __local_f3__2
   BMI   .sx_neg@23
   LDA   #$00
   BEQ   .sx_done@24
.sx_neg@23:
   LDA   #$FF
.sx_done@24:
   STA   __local_f3__1
   STA   __local_f3__0
   LDA   g1
   AND   g2
   STA   __local_f3__5
   LDA   __local_f3__2
   AND   g2+1
   STA   __local_f3__4
   LDA   __local_f3__1
   AND   g2+2
   STA   __zpabi_f5__p1_0
   LDA   __local_f3__0
   AND   g2+3
   STA   __local_f3__2
   LDA   g1
   BMI   .sx_neg@25
   LDA   #$00
   BEQ   .sx_done@26
.sx_neg@25:
   LDA   #$FF
.sx_done@26:
   STA   __local_f3__0
   LDA   g1
   ORA   #$01
   STA   __local_f3__1
   LDA   __local_f3__0
   BMI   .sx_neg@27
   LDA   #$00
   BEQ   .sx_done@28
.sx_neg@27:
   LDA   #$FF
.sx_done@28:
   STA   HARGS+6
   STA   HARGS+7
   LDA   __local_f3__5
   STA   HARGS
   LDA   __local_f3__4
   STA   HARGS+1
   LDA   __zpabi_f5__p1_0
   STA   HARGS+2
   LDA   __local_f3__2
   STA   HARGS+3
   LDA   __local_f3__1
   STA   HARGS+4
   LDA   __local_f3__0
   STA   HARGS+5
   JSR   sdivmod32
   LDX   HARGS+9
   LDA   HARGS+8
   RTS

f2:
   SUBROUTINE

   LDA   g0
   SEC
   SBC   __zpabi_f2__p0_0
   LDA   g0+1
   SBC   __zpabi_f2__p0_1
   LDA   #$00
   SBC   __zpabi_f2__p0_2
   LDA   #$00
   SBC   __zpabi_f2__p0_3
   BVC   .cmp_novf@29
   EOR   #$80
.cmp_novf@29:
   ASL
   LDA   #$00
   ROL
   EOR   #$01
   STA   __local_f2__0
   LDA   #$07
   SEC
   SBC   __local_f2__0
   LDA   #$00
   SBC   #$00
   BVC   .cmp_novf@30
   EOR   #$80
.cmp_novf@30:
   ASL
   LDA   #$00
   ROL
   STA   __local_f2__5
   LDA   #$00
   BPL   .sx_done@32
   LDA   #$FF
.sx_done@32:
   STA   __local_f2__1
   STA   __local_f2__0
   LDA   __local_f2__5
   STA   __local_f2__box4
   LDY   #$00
   STY   __local_f2__box4+1
   ORA   #$00
   ORA   __local_f2__1
   ORA   __local_f2__0
   BEQ   .and_false@16
   LDA   #$01
   STA   __zpabi_f4__p1_0
   JMP   .and_end@17
.and_false@16:
   LDA   #$00
   STA   __zpabi_f4__p1_0
.and_end@17:
   LDA   #$02
   STA   __zpabi_f4__p0_0
   LDA   #$00
   STA   __zpabi_f4__p0_1
   STA   __zpabi_f4__p0_2
   STA   __zpabi_f4__p0_3
   STA   __zpabi_f4__p1_1
   LDA   g0
   STA   __zpabi_f4__p3
   LDA   #<__local_f2__box4
   STA   __zpabi_f4__pp_0
   LDA   #>__local_f2__box4
   STA   __zpabi_f4__pp_1
   JSR   f4
   LDA   __local_f2__box4
   STA   __local_f2__1
   LDA   __local_f2__5
   EOR   HARGS+8
   CLC
   ADC   __local_f2__1
   CLC
   ADC   g1
   STA   g1
   LDA   __zpabi_f4__p2_0
   AND   #$64
   JMP   f2@tail1

f2@tail1:
   SUBROUTINE

   BEQ   .cmp_end@35
   SEC
   RTS
.cmp_end@35:
   CMP   #$01
   RTS

f1:
   SUBROUTINE

   LDA   g1
   BMI   .sx_neg@36
   LDA   #$00
   BEQ   .sx_done@37
.sx_neg@36:
   LDA   #$FF
.sx_done@37:
   ORA   g1
   JMP   f2@tail1

f0:
   SUBROUTINE

   LDA   g1
   BMI   .sx_neg@39
   LDA   #$00
   BEQ   .sx_done@40
.sx_neg@39:
   LDA   #$FF
.sx_done@40:
   STA   __local_f0__2
   LDA   g1
   BMI   .sx_neg@41
   LDA   #$00
   BEQ   .sx_done@42
.sx_neg@41:
   LDA   #$FF
.sx_done@42:
   STA   __local_f0__0
   LDA   __local_f0__2
   CMP   __local_f0__0
   BNE   .cmp_true@43
   LDA   g1
   CMP   g1
   BNE   .cmp_true@43
   LDA   #$00
   BEQ   .cmp_end@44
.cmp_true@43:
   LDA   #$01
.cmp_end@44:
   STA   __local_f0__0
   LDA   #$00
   BPL   .sx_done@47
   LDA   #$FF
.sx_done@47:
   STA   HARGS+2
   STA   HARGS+3
   LDA   g2
   ORA   #$01
   STA   HARGS+4
   LDA   g2+1
   STA   HARGS+5
   LDA   g2+2
   STA   HARGS+6
   LDA   g2+3
   STA   HARGS+7
   LDA   __local_f0__0
   STA   HARGS
   LDA   #$00
   STA   HARGS+1
   JSR   sdivmod32
   JMP   f4@tail0

main:
   SUBROUTINE

   LDA   #$91
   STA   __zpabi_f0__p1_0
   LDA   #$00
   STA   __zpabi_f0__p1_1
   STA   __zpabi_f0__p1_2
   STA   __zpabi_f0__p1_3
   LDA   #$22
   JSR   f0
   LDA   g1
   BMI   .sx_neg@48
   LDA   #$00
   BEQ   .sx_done@49
.sx_neg@48:
   LDA   #$FF
.sx_done@49:
   STA   __local_main__2
   STA   __local_main__1
   STA   __local_main__0
   LDA   g0
   EOR   g1
   STA   __local_main__3
   LDA   g0+1
   EOR   __local_main__2
   STA   __local_main__2
   LDA   #$00
   EOR   __local_main__1
   STA   __local_main__1
   LDA   #$00
   EOR   __local_main__0
   STA   __local_main__0
   LDA   __local_main__3
   EOR   g2
   STA   __local_main__3
   LDA   __local_main__2
   EOR   g2+1
   STA   __local_main__2
   LDA   __local_main__1
   EOR   g2+2
   STA   __local_main__1
   LDA   __local_main__0
   EOR   g2+3
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

g2:
   DC.L  $000000AC

__zp_init:
   SUBROUTINE

   LDA   #$00
   STA   g0+1
   LDA   #$74
   STA   g0
   LDA   #$BA
   STA   g1
   RTS
