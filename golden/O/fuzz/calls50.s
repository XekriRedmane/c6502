SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
g1	EQU	$26
g0	EQU	$27
__zpabi_f0__p0_0	EQU	$80
__zpabi_f1__p1_0	EQU	$80
__zpabi_f0__p0_1	EQU	$81
__zpabi_f1__p1_1	EQU	$81
__zpabi_f0__p2_0	EQU	$82
__zpabi_f3__p0_0	EQU	$82
__zpabi_f0__p2_1	EQU	$83
__zpabi_f3__p0_1	EQU	$83
__zpabi_f0__p2_2	EQU	$84
__zpabi_f3__p0_2	EQU	$84
__zpabi_f0__p2_3	EQU	$85
__zpabi_f3__p0_3	EQU	$85
__zpabi_f0__p3_0	EQU	$86
__zpabi_f3__p1_0	EQU	$86
__zpabi_f0__p3_1	EQU	$87
__zpabi_f3__p1_1	EQU	$87
__zpabi_f0__p3_2	EQU	$88
__zpabi_f3__p2_0	EQU	$88
__zpabi_f0__p3_3	EQU	$89
__zpabi_f3__p2_1	EQU	$89
__zpabi_f0__pp_0	EQU	$8A
__zpabi_f3__p2_2	EQU	$8A
__zpabi_f0__pp_1	EQU	$8B
__zpabi_f3__p2_3	EQU	$8B
__local_main__0	EQU	$8C
__zpabi_f3__p3	EQU	$8C
__local_main__1	EQU	$8D
__zpabi_f4__p1_0	EQU	$8D
__local_main__2	EQU	$8E
__zpabi_f4__p1_1	EQU	$8E
__local_main__3	EQU	$8F
__zpabi_f4__p1_2	EQU	$8F
__zpabi_f4__p1_3	EQU	$90
__local_main__5	EQU	$91
__zpabi_f4__p2_0	EQU	$91
__local_main__6	EQU	$92
__zpabi_f4__p2_1	EQU	$92
__local_main__7	EQU	$93
__zpabi_f4__p2_2	EQU	$93
__local_main__8	EQU	$94
__zpabi_f4__p2_3	EQU	$94
__local_main__mbox	EQU	$95
__zpabi_f4__pp_0	EQU	$95
__zpabi_f4__pp_1	EQU	$96
__zpabi_f4__p3_0	EQU	$97
__zpabi_f4__p3_1	EQU	$98
__local_f0__0	EQU	$99
__zpabi_f5__p1_0	EQU	$99
__local_f0__1	EQU	$9A
__zpabi_f5__p1_1	EQU	$9A
__local_f0__2	EQU	$9B
__zpabi_f5__p1_2	EQU	$9B
__local_f0__3	EQU	$9C
__zpabi_f5__p1_3	EQU	$9C
__local_f0__4	EQU	$9D
__local_f1__4	EQU	$A1
__local_f1__p0	EQU	$A2
__local_f2__0	EQU	$A3
__local_f2__1	EQU	$A4
__local_f2__3	EQU	$A6
__local_f3__3	EQU	$A6
__local_f3__4	EQU	$A7
__local_f3__box4	EQU	$A8
__local_f2__7	EQU	$AA
__local_f2__it_1	EQU	$AB
__local_f2__it_0	EQU	$AC
__local_f2__8	EQU	$AD
__local_f2__9	EQU	$AE
__local_f2__10	EQU	$AF
__local_f2__11	EQU	$B0
__local_f2__12	EQU	$B1
__local_f2__box4	EQU	$B2
__local_f4__0	EQU	$B6
__local_f5__0	EQU	$B7
__local_f5__1	EQU	$B8
__local_f5__2	EQU	$B9

; @zp-link-meta-begin
; def f0 params=__zpabi_f0__p0_0,__zpabi_f0__p0_1,__zpabi_f0__p1,__zpabi_f0__p2_0,__zpabi_f0__p2_1,__zpabi_f0__p2_2,__zpabi_f0__p2_3,__zpabi_f0__p3_0,__zpabi_f0__p3_1,__zpabi_f0__p3_2,__zpabi_f0__p3_3,__zpabi_f0__pp_0,__zpabi_f0__pp_1 locals=__local_f0__0,__local_f0__1,__local_f0__2,__local_f0__3,__local_f0__4,__local_f0__5,__local_f0__6,__local_f0__7 indirect=false in_cycle=false param_regs=,,A,,,,,,,,,,
; def f1 params=__zpabi_f1__p0,__zpabi_f1__p1_0,__zpabi_f1__p1_1 locals=__local_f1__0,__local_f1__1,__local_f1__2,__local_f1__3,__local_f1__4,__local_f1__p0 indirect=false in_cycle=false param_regs=A,,
; def f2 params= locals=__local_f2__0,__local_f2__1,__local_f2__2,__local_f2__3,__local_f2__4,__local_f2__5,__local_f2__6,__local_f2__7,__local_f2__it_1,__local_f2__it_0,__local_f2__8,__local_f2__9,__local_f2__10,__local_f2__11,__local_f2__12,__local_f2__box4,__local_f2__14 indirect=false in_cycle=false
; def f3 params=__zpabi_f3__p0_0,__zpabi_f3__p0_1,__zpabi_f3__p0_2,__zpabi_f3__p0_3,__zpabi_f3__p1_0,__zpabi_f3__p1_1,__zpabi_f3__p2_0,__zpabi_f3__p2_1,__zpabi_f3__p2_2,__zpabi_f3__p2_3,__zpabi_f3__p3 locals=__local_f3__0,__local_f3__1,__local_f3__2,__local_f3__3,__local_f3__4,__local_f3__box4,__local_f3__6,__local_f3__7 indirect=false in_cycle=false
; def f4 params=__zpabi_f4__p0_0,__zpabi_f4__p0_1,__zpabi_f4__p1_0,__zpabi_f4__p1_1,__zpabi_f4__p1_2,__zpabi_f4__p1_3,__zpabi_f4__p2_0,__zpabi_f4__p2_1,__zpabi_f4__p2_2,__zpabi_f4__p2_3,__zpabi_f4__p3_0,__zpabi_f4__p3_1,__zpabi_f4__pp_0,__zpabi_f4__pp_1 locals=__local_f4__p0_1,__local_f4__p0_0,__local_f4__0 indirect=false in_cycle=false param_regs=A,X,,,,,,,,,,,,
; def f5 params=__zpabi_f5__p0_0,__zpabi_f5__p0_1,__zpabi_f5__p1_0,__zpabi_f5__p1_1,__zpabi_f5__p1_2,__zpabi_f5__p1_3 locals=__local_f5__0,__local_f5__1,__local_f5__2,__local_f5__3 indirect=false in_cycle=false param_regs=A,X,,,, carry=true
; def main params= locals=__local_main__0,__local_main__1,__local_main__2,__local_main__3,__local_main__4,__local_main__5,__local_main__6,__local_main__7,__local_main__8,__local_main__mbox,__local_main__10,__local_main__11,__local_main__12 indirect=false in_cycle=false
; call f1 -> f2
; call f1 -> f3
; call f2 -> f4
; call f3 -> f4
; call f3 -> f5
; call f4 -> f5
; call main -> f0
; share member=f4:__zpabi_f4__p3_0,__zpabi_f4__p3_1 holder=f2 live=f2:f4 forward=f2>f4 writer=f2
; item f5 section=.text local=false zero=false
; item f4 section=.text local=false zero=false
; item f3 section=.text local=false zero=false
; item f2 section=.text local=false zero=false
; item f2@run0 section=.text local=true zero=false
; item f1 section=.text local=false zero=false
; item f0 section=.text local=false zero=false
; item main section=.text local=false zero=false
; item __zp_init section=.text local=false zero=false
; zp g1 size=1 at=auto def=true init=$B1 weight=17
; zp g0 size=2 at=auto def=true init=$A3,$00 weight=17
; runtime hargs_size=32 hargs_need=22
; @zp-link-meta-end

f5:
   SUBROUTINE

   LDA   #$00
   BPL   .sx_done@1
   LDA   #$FF
.sx_done@1:
   STA   __local_f5__1
   STA   __local_f5__0
   LDA   __zpabi_f5__p1_3
   CMP   __local_f5__0
   BNE   .cmp_true@2
   LDA   __zpabi_f5__p1_2
   CMP   __local_f5__1
   BNE   .cmp_true@2
   LDA   __zpabi_f5__p1_1
   BNE   .cmp_true@2
   LDA   __zpabi_f5__p1_0
   CMP   g1
   BNE   .cmp_true@2
   LDA   #$00
   BEQ   .cmp_end@3
.cmp_true@2:
   LDA   #$01
.cmp_end@3:
   STA   __local_f5__1
   LDA   g1
   ORA   #$01
   STA   HARGS+1
   LDA   g1
   STA   HARGS
   JSR   udivmod8
   LDA   HARGS+2
   STA   __local_f5__2
   LDA   g1
   CLC
   ADC   __local_f5__1
   AND   #$07
   TAX
   BEQ   .shift_done@6
.shift_loop@5:
   LSR   __local_f5__2
   DEX
   BNE   .shift_loop@5
.shift_done@6:
   LDA   __local_f5__2
   BEQ   .cmp_end@7
   SEC
   RTS
.cmp_end@7:
   CMP   #$01
   RTS

f4:
   SUBROUTINE

   STA   __zpabi_f5__p1_0
   AND   __zpabi_f5__p1_0
   STA   __local_f4__0
   LDY   #$00
   LDA   (__zpabi_f4__pp_0),Y
   CLC
   ADC   __zpabi_f4__p1_0
   STA   __zpabi_f5__p1_0
   INY
   LDA   (__zpabi_f4__pp_0),Y
   ADC   __zpabi_f4__p1_1
   STA   (__zpabi_f4__pp_0),Y
   LDA   __zpabi_f5__p1_0
   DEY
   STA   (__zpabi_f4__pp_0),Y
   LDA   (__zpabi_f4__pp_0),Y
   STA   __zpabi_f5__p1_0
   INY
   LDA   (__zpabi_f4__pp_0),Y
   STA   __zpabi_f5__p1_1
   LDA   g0
   EOR   __zpabi_f5__p1_0
   STA   __zpabi_f5__p1_0
   LDA   g0+1
   EOR   __zpabi_f5__p1_1
   STA   __zpabi_f5__p1_1
   BMI   .sx_neg@8
   LDA   #$00
   BEQ   .sx_done@9
.sx_neg@8:
   LDA   #$FF
.sx_done@9:
   STA   __zpabi_f5__p1_2
   STA   __zpabi_f5__p1_3
   LDX   __zpabi_f4__p1_1
   LDA   __zpabi_f4__p1_0
   JSR   f5
   LDA   __zpabi_f4__p2_0
   CLC
   ADC   __local_f4__0
   RTS

f3:
   SUBROUTINE

   LDA   __zpabi_f3__p0_0
   ORA   g0
   STA   __local_f3__4
   LDA   __zpabi_f3__p0_0
   SEC
   SBC   #$01
   LDA   __zpabi_f3__p0_1
   SBC   #$00
   LDA   __zpabi_f3__p0_2
   SBC   #$00
   LDA   __zpabi_f3__p0_3
   SBC   #$00
   BVC   .cmp_novf@12
   EOR   #$80
.cmp_novf@12:
   ASL
   LDA   #$00
   ROL
   STA   __zpabi_f5__p1_0
   LDA   #$00
   BPL   .sx_done@14
   LDA   #$FF
.sx_done@14:
   STA   __zpabi_f5__p1_2
   STA   __zpabi_f5__p1_3
   LDA   #$00
   STA   __zpabi_f5__p1_1
   LDX   #$00
   LDA   __local_f3__4
   JSR   f5
   LDA   #$00
   ROL
   STA   __zpabi_f5__p1_0
   LDA   __local_f3__4
   EOR   __zpabi_f5__p1_0
   STA   __local_f3__3
   STA   __local_f3__box4
   LDA   #$00
   STA   __local_f3__box4+1
   LDA   __zpabi_f3__p3
   BMI   .sx_neg@15
   LDA   #$00
   BEQ   .sx_done@16
.sx_neg@15:
   LDA   #$FF
.sx_done@16:
   STA   __zpabi_f4__p1_1
   STA   __zpabi_f4__p1_2
   STA   __zpabi_f4__p1_3
   LDA   __zpabi_f3__p3
   STA   __zpabi_f4__p1_0
   LDA   #$00
   STA   __zpabi_f4__p2_0
   STA   __zpabi_f4__p2_1
   STA   __zpabi_f4__p2_2
   STA   __zpabi_f4__p2_3
   STA   __zpabi_f4__p3_1
   LDA   g1
   STA   __zpabi_f4__p3_0
   LDA   #<__local_f3__box4
   STA   __zpabi_f4__pp_0
   LDA   #>__local_f3__box4
   STA   __zpabi_f4__pp_1
   LDX   #$00
   LDA   __local_f3__4
   JSR   f4
   STA   __zpabi_f5__p1_0
   LDA   __local_f3__3
   EOR   __zpabi_f5__p1_0
   CLC
   ADC   __local_f3__box4
   CLC
   ADC   g1
   STA   g1
   LDA   __zpabi_f3__p1_0
   ORA   #$01
   STA   HARGS+2
   LDA   __zpabi_f3__p1_1
   STA   HARGS+3
   LDA   __local_f3__4
   STA   HARGS
   LDA   #$00
   STA   HARGS+1
   JSR   udivmod16
   LDA   __zpabi_f3__p0_0
   ORA   __zpabi_f3__p0_1
   ORA   __zpabi_f3__p0_2
   ORA   __zpabi_f3__p0_3
   BEQ   .and_false@0
   LDA   g0
   ORA   g0+1
   BEQ   .and_false@0
   LDA   #$01
   BNE   .and_end@1
.and_false@0:
   LDA   #$00
.and_end@1:
   SEC
   SBC   HARGS+4
   LDA   #$00
   SBC   HARGS+5
   LDA   #$00
   ROL
   LDX   #$00
   RTS

f2:
   SUBROUTINE

   LDA   g0
   STA   __local_f2__12
   LDA   g0+1
   STA   __local_f2__11
   BMI   .sx_neg@17
   LDA   #$00
   BEQ   .sx_done@18
.sx_neg@17:
   LDA   #$FF
.sx_done@18:
   STA   __local_f2__10
   STA   __local_f2__9
   LDA   #$64
   SEC
   SBC   __local_f2__12
   LDA   #$00
   SBC   __local_f2__11
   LDA   #$00
   SBC   __local_f2__10
   LDA   #$00
   SBC   __local_f2__9
   BVC   .cmp_novf@19
   EOR   #$80
.cmp_novf@19:
   JSR   f2@run0
   STA   __local_f2__0
   LDA   g1
   SEC
   SBC   __local_f2__0
   LDA   #$00
   SBC   #$00
   BVC   .cmp_novf@20
   EOR   #$80
.cmp_novf@20:
   JSR   f2@run0
   STA   __zpabi_f4__p3_0
   LDA   #$00
   STA   __zpabi_f4__p3_1
   LDA   __local_f2__12
   ORA   __local_f2__11
   ORA   __local_f2__10
   ORA   __local_f2__9
   BEQ   .and_false@2
   LDA   g1
   BEQ   .and_false@2
   LDA   #$01
   STA   __local_f2__3
   LDA   #$00
   BEQ   .and_end@3
.and_false@2:
   LDA   #$00
   STA   __local_f2__3
.and_end@3:
   ORA   #$00
   BPL   .sx_done@22
   LDA   #$FF
.sx_done@22:
   STA   __local_f2__1
   STA   __local_f2__0
   LDA   __local_f2__3
   SEC
   SBC   __local_f2__12
   LDA   #$00
   SBC   __local_f2__11
   LDA   __local_f2__1
   SBC   __local_f2__10
   LDA   __local_f2__0
   SBC   __local_f2__9
   BVC   .cmp_novf@23
   EOR   #$80
.cmp_novf@23:
   ASL
   LDA   #$00
   ROL
   STA   __local_f2__8
   LDA   __local_f2__12
   STA   __local_f2__box4
   LDA   __local_f2__11
   STA   __local_f2__box4+1
   LDA   #$00
   STA   __local_f2__it_0
   LDY   #$00
   STY   __local_f2__it_1
.loop@0_start:
   LDA   __local_f2__it_0
   SEC
   SBC   #$03
   LDA   __local_f2__it_1
   SBC   #$00
   BVC   .jcmp_novf@24
   EOR   #$80
.jcmp_novf@24:
   BMI   .lb_skip@1
   JMP   .loop@0_break
.lb_skip@1:
   LDA   __zpabi_f4__p3_1
   BNE   .thread@0
   LDA   __zpabi_f4__p3_0
   CMP   #$FF
   BEQ   .cmp_true@25
.thread@0:
   LDA   #$00
   BEQ   .cmp_end@26
.cmp_true@25:
   LDA   #$01
.cmp_end@26:
   STA   __local_f2__7
   LDA   __local_f2__12
   SEC
   SBC   __local_f2__8
   LDA   __local_f2__11
   SBC   #$00
   LDA   __local_f2__10
   SBC   #$00
   LDA   __local_f2__9
   SBC   #$00
   BVC   .cmp_novf@28
   EOR   #$80
.cmp_novf@28:
   JSR   f2@run0
   STA   __zpabi_f4__p1_0
   LDA   #$00
   BPL   .sx_done@30
   LDA   #$FF
.sx_done@30:
   STA   __zpabi_f4__p1_2
   LDA   g0+1
   BMI   .sx_neg@31
   LDA   #$00
   BEQ   .sx_done@32
.sx_neg@31:
   LDA   #$FF
.sx_done@32:
   CMP   __local_f2__9
   BNE   .thread@1
   CMP   __local_f2__10
   BNE   .thread@1
   LDA   g0+1
   CMP   __local_f2__11
   BNE   .thread@1
   LDA   g0
   CMP   __local_f2__12
   BEQ   .cmp_true@33
.thread@1:
   LDA   #$00
   BEQ   .cmp_end@34
.cmp_true@33:
   LDA   #$01
.cmp_end@34:
   STA   __zpabi_f4__p2_0
   LDA   #$00
   BPL   .sx_done@37
   LDA   #$FF
.sx_done@37:
   STA   __zpabi_f4__p2_2
   STA   __zpabi_f4__p2_3
   LDA   #$00
   STA   __zpabi_f4__p1_1
   STA   __zpabi_f4__p2_1
   LDA   __zpabi_f4__p1_2
   STA   __zpabi_f4__p1_3
   LDA   #<__local_f2__box4
   STA   __zpabi_f4__pp_0
   LDA   #>__local_f2__box4
   STA   __zpabi_f4__pp_1
   LDX   #$00
   LDA   __local_f2__7
   JSR   f4
   STA   __local_f2__1
   ORA   #$00
   BMI   .sx_neg@38
   LDA   #$00
   BEQ   .sx_done@39
.sx_neg@38:
   LDA   #$FF
.sx_done@39:
   STA   __local_f2__0
   LDA   __local_f2__1
   CLC
   ADC   __local_f2__it_0
   STA   __local_f2__1
   LDA   __local_f2__0
   ADC   __local_f2__it_1
   STA   __local_f2__0
   LDA   __zpabi_f4__p3_0
   CLC
   ADC   __local_f2__1
   STA   __zpabi_f4__p3_0
   LDA   __zpabi_f4__p3_1
   ADC   __local_f2__0
   STA   __zpabi_f4__p3_1
   INC   __local_f2__it_0
   BEQ   .lb_skip@0
   JMP   .loop@0_start
.lb_skip@0:
   INC   __local_f2__it_1
   JMP   .loop@0_start
.loop@0_break:
   LDA   __zpabi_f4__p3_0
   CLC
   ADC   __local_f2__box4
   CLC
   ADC   g1
   STA   g1
   LDA   g0
   SEC
   SBC   __local_f2__12
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

   STA   __local_f1__p0
   ORA   #$00
   BMI   .sx_neg@42
   LDA   #$00
   BEQ   .sx_done@43
.sx_neg@42:
   LDA   #$FF
.sx_done@43:
   STA   __local_f1__4
   LDA   g1
   AND   __local_f1__p0
   STA   __zpabi_f3__p1_1
   LDA   __zpabi_f1__p1_0
   ORA   __zpabi_f1__p1_1
   BEQ   .and_false@4
   LDA   __zpabi_f3__p1_1
   SEC
   SBC   #$07
   BVC   .jcmp_novf@44
   EOR   #$80
.jcmp_novf@44:
   BPL   .and_end@5
.and_false@4:
.and_end@5:
   LDA   __zpabi_f1__p1_1
   BMI   .sx_neg@45
   LDA   #$00
   BEQ   .sx_done@46
.sx_neg@45:
   LDA   #$FF
.sx_done@46:
   STA   __zpabi_f3__p0_2
   SEC
   LDA   #$00
   SBC   __local_f1__p0
   STA   __zpabi_f3__p1_0
   LDA   #$00
   SBC   __local_f1__4
   STA   __zpabi_f3__p1_1
   LDA   __zpabi_f1__p1_1
   BMI   .sx_neg@47
   LDA   #$00
   BEQ   .sx_done@48
.sx_neg@47:
   LDA   #$FF
.sx_done@48:
   STA   __zpabi_f3__p2_2
   STA   __zpabi_f3__p2_3
   LDA   __zpabi_f1__p1_0
   STA   __zpabi_f3__p0_0
   LDA   __zpabi_f1__p1_1
   STA   __zpabi_f3__p0_1
   LDA   __zpabi_f3__p0_2
   STA   __zpabi_f3__p0_3
   LDA   __zpabi_f1__p1_0
   STA   __zpabi_f3__p2_0
   LDA   __zpabi_f1__p1_1
   STA   __zpabi_f3__p2_1
   LDA   #$01
   STA   __zpabi_f3__p3
   JSR   f3
   JSR   f2
   LDA   __local_f1__p0
   SEC
   SBC   #$FF
   LDA   __local_f1__4
   SBC   #$FF
   BVC   .cmp_novf@49
   EOR   #$80
.cmp_novf@49:
   JSR   f2@run0
   RTS

f0:
   SUBROUTINE

   LDA   g0+1
   BMI   .sx_neg@50
   LDA   #$00
   BEQ   .sx_done@51
.sx_neg@50:
   LDA   #$FF
.sx_done@51:
   STA   HARGS+2
   STA   HARGS+3
   LDA   __zpabi_f0__p3_0
   ORA   #$01
   STA   HARGS+4
   LDA   __zpabi_f0__p3_1
   STA   HARGS+5
   LDA   __zpabi_f0__p3_2
   STA   HARGS+6
   LDA   __zpabi_f0__p3_3
   STA   HARGS+7
   LDA   g0
   STA   HARGS
   LDA   g0+1
   STA   HARGS+1
   JSR   sdivmod32
   LDA   __zpabi_f0__p3_0
   SEC
   SBC   #$E8
   STA   __local_f0__3
   LDA   __zpabi_f0__p3_1
   SBC   #$03
   STA   __local_f0__2
   LDA   __zpabi_f0__p3_2
   SBC   #$00
   STA   __local_f0__1
   LDA   __zpabi_f0__p3_3
   SBC   #$00
   STA   __local_f0__0
   LDA   __local_f0__3
   SEC
   SBC   HARGS+8
   LDA   __local_f0__2
   SBC   HARGS+9
   LDA   __local_f0__1
   SBC   HARGS+10
   LDA   __local_f0__0
   SBC   HARGS+11
   BVC   .cmp_novf@52
   EOR   #$80
.cmp_novf@52:
   ASL
   LDA   #$00
   ROL
   STA   __local_f0__0
   LSR
   BCS   .or_true@10
   LDA   __local_f0__0
   BEQ   .or_end@11
.or_true@10:
.or_end@11:
   LDY   #$00
   LDA   (__zpabi_f0__pp_0),Y
   STA   __local_f0__4
   INY
   LDA   (__zpabi_f0__pp_0),Y
   STA   __local_f0__3
   LDA   #$00
   BPL   .sx_done@54
   LDA   #$FF
.sx_done@54:
   STA   __local_f0__1
   STA   __local_f0__0
   LDA   __zpabi_f0__p2_3
   CMP   __local_f0__0
   BNE   .thread@2
   LDA   __zpabi_f0__p2_2
   CMP   __local_f0__1
   BNE   .thread@2
   LDA   __zpabi_f0__p2_1
   BNE   .thread@2
   LDA   __zpabi_f0__p2_0
   CMP   g1
   BEQ   .cmp_true@55
.thread@2:
   LDA   #$00
   BEQ   .cmp_end@56
.cmp_true@55:
   LDA   #$01
.cmp_end@56:
   CLC
   ADC   __local_f0__4
   BCC   .carry_inc@0
   INC   __local_f0__3
.carry_inc@0:
   LDY   #$00
   STA   (__zpabi_f0__pp_0),Y
   LDA   __local_f0__3
   INY
   STA   (__zpabi_f0__pp_0),Y
   LDA   g0+1
   BMI   .sx_neg@58
   LDA   #$00
   BEQ   .sx_done@59
.sx_neg@58:
   LDA   #$FF
.sx_done@59:
   STA   __local_f0__1
   STA   __local_f0__0
   LDA   __zpabi_f0__p2_0
   CLC
   SBC   g0
   LDA   __zpabi_f0__p2_1
   SBC   g0+1
   LDA   __zpabi_f0__p2_2
   SBC   __local_f0__1
   LDA   __zpabi_f0__p2_3
   SBC   __local_f0__0
   LDA   #$00
   ROL
   RTS

main:
   SUBROUTINE

   LDA   #$05
   STA   __local_main__mbox
   LDA   #$00
   STA   __local_main__mbox+1
   LDA   #$26
   STA   __zpabi_f0__p0_0
   LDA   #$01
   STA   __zpabi_f0__p0_1
   LDA   #$47
   STA   __zpabi_f0__p2_0
   LDA   #$00
   STA   __zpabi_f0__p2_1
   STA   __zpabi_f0__p2_2
   STA   __zpabi_f0__p2_3
   STA   __zpabi_f0__p3_1
   STA   __zpabi_f0__p3_2
   STA   __zpabi_f0__p3_3
   LDA   #$9D
   STA   __zpabi_f0__p3_0
   LDA   #<__local_main__mbox
   STA   __zpabi_f0__pp_0
   LDA   #>__local_main__mbox
   STA   __zpabi_f0__pp_1
   LDA   #$E4
   JSR   f0
   STA   __local_main__8
   ORA   #$00
   BMI   .sx_neg@60
   LDA   #$00
   BEQ   .sx_done@61
.sx_neg@60:
   LDA   #$FF
.sx_done@61:
   STA   __local_main__7
   STA   __local_main__6
   STA   __local_main__5
   LDA   g0+1
   BMI   .sx_neg@62
   LDA   #$00
   BEQ   .sx_done@63
.sx_neg@62:
   LDA   #$FF
.sx_done@63:
   STA   __local_main__1
   LDA   g0
   EOR   g1
   STA   __local_main__3
   LDA   __local_main__1
   STA   __local_main__0
   LDA   __local_main__8
   EOR   __local_main__3
   STA   __local_main__3
   LDA   __local_main__7
   EOR   g0+1
   STA   __local_main__2
   LDA   __local_main__6
   EOR   __local_main__1
   STA   HARGS+2
   LDA   __local_main__5
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
   LDA   #$A3
   STA   g0
   LDA   #$B1
   STA   g1
   RTS
