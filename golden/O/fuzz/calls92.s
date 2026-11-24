SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
g1	EQU	$26
g2	EQU	$27
__local_main__0	EQU	$80
__zpabi_f2__p0_0	EQU	$80
__zpabi_f3__p1_0	EQU	$80
__local_main__1	EQU	$81
__zpabi_f2__p0_1	EQU	$81
__zpabi_f3__p1_1	EQU	$81
__local_main__2	EQU	$82
__zpabi_f2__p0_2	EQU	$82
__zpabi_f3__p1_2	EQU	$82
__local_main__3	EQU	$83
__zpabi_f2__p0_3	EQU	$83
__zpabi_f3__p1_3	EQU	$83
__zpabi_f2__p0_4	EQU	$84
__zpabi_f3__p1_4	EQU	$84
__zpabi_f2__p2	EQU	$85
__zpabi_f3__p1_5	EQU	$85
__zpabi_f3__p1_6	EQU	$86
__zpabi_f4__pp_0	EQU	$86
__zpabi_f3__p1_7	EQU	$87
__zpabi_f4__pp_1	EQU	$87
__local_main__8	EQU	$88
__zpabi_f3__p2_0	EQU	$88
__local_main__mbox	EQU	$89
__zpabi_f3__p2_1	EQU	$89
__zpabi_f3__p2_2	EQU	$8A
__zpabi_f3__p2_3	EQU	$8B
__local_f1__0	EQU	$8C
__local_f0__0	EQU	$8D
__local_f0__1	EQU	$8E
__local_f0__2	EQU	$8F
__local_f1__3	EQU	$8F
__local_f0__3	EQU	$90
__local_f0__4	EQU	$91
__local_f1__5	EQU	$91
__local_f0__5	EQU	$92
__local_f1__6	EQU	$92
__local_f0__6	EQU	$93
__local_f1__7	EQU	$93
__local_f0__7	EQU	$94
__local_f1__8	EQU	$94
__local_f0__8	EQU	$95
__local_f0__9	EQU	$96
__local_f1__it_1	EQU	$96
__local_f0__10	EQU	$97
__local_f1__it_0	EQU	$97
__local_f0__11	EQU	$98
__local_f1__l1	EQU	$98
__local_f0__12	EQU	$99
__local_f1__10	EQU	$99
__local_f0__13	EQU	$9A
__local_f1__11	EQU	$9A
__local_f0__pp	EQU	$9B
__local_f2__0	EQU	$9B
__local_f0__14	EQU	$9C
__local_f2__1	EQU	$9C
__local_f2__2	EQU	$9D
__local_f2__it_1	EQU	$9E
__local_f2__it_0	EQU	$9F
__local_f2__l3_1	EQU	$A0
__local_f2__l3_0	EQU	$A1
__local_f2__3	EQU	$A2
__local_f2__4	EQU	$A3
__local_f2__5	EQU	$A4
__local_f2__6	EQU	$A5
__local_f2__box4	EQU	$A6
__local_f4__0	EQU	$A9
__local_f4__1	EQU	$AA
__local_f4__2	EQU	$AB
__local_f4__3	EQU	$AC
__local_f4__4	EQU	$AD
__local_f4__5	EQU	$AE
__local_f4__6	EQU	$AF
__local_f4__7	EQU	$B0
__local_f4__p0_1	EQU	$B1
__local_f4__8	EQU	$B2
__local_f4__9	EQU	$B3
__local_f4__10	EQU	$B4
__local_f4__11	EQU	$B5
__local_f4__12	EQU	$B6
__local_f4__p0_0	EQU	$B7

; @zp-link-meta-begin
; def f0 params=__zpabi_f0__pp_0,__zpabi_f0__pp_1 locals=__local_f0__0,__local_f0__1,__local_f0__2,__local_f0__3,__local_f0__4,__local_f0__5,__local_f0__6,__local_f0__7,__local_f0__8,__local_f0__9,__local_f0__10,__local_f0__11,__local_f0__12,__local_f0__13,__local_f0__pp,__local_f0__14 indirect=false in_cycle=false param_regs=A,X carry=true
; def f1 params=__zpabi_f1__p0_0,__zpabi_f1__p0_1 locals=__local_f1__0,__local_f1__1,__local_f1__2,__local_f1__3,__local_f1__4,__local_f1__5,__local_f1__6,__local_f1__7,__local_f1__8,__local_f1__9,__local_f1__it_1,__local_f1__it_0,__local_f1__l1,__local_f1__10,__local_f1__11 indirect=false in_cycle=false param_regs=A,X
; def f2 params=__zpabi_f2__p0_0,__zpabi_f2__p0_1,__zpabi_f2__p0_2,__zpabi_f2__p0_3,__zpabi_f2__p0_4,__zpabi_f2__p1_0,__zpabi_f2__p1_1,__zpabi_f2__p2 locals=__local_f2__0,__local_f2__1,__local_f2__2,__local_f2__it_1,__local_f2__it_0,__local_f2__l3_1,__local_f2__l3_0,__local_f2__3,__local_f2__4,__local_f2__5,__local_f2__6,__local_f2__box4,__local_f2__8,__local_f2__9 indirect=false in_cycle=false param_regs=,,,,,A,X,
; def f3 params=__zpabi_f3__p0,__zpabi_f3__p1_0,__zpabi_f3__p1_1,__zpabi_f3__p1_2,__zpabi_f3__p1_3,__zpabi_f3__p1_4,__zpabi_f3__p1_5,__zpabi_f3__p1_6,__zpabi_f3__p1_7,__zpabi_f3__p2_0,__zpabi_f3__p2_1,__zpabi_f3__p2_2,__zpabi_f3__p2_3 locals=__local_f3__0 indirect=false in_cycle=false param_regs=A,,,,,,,,,,,,
; def f4 params=__zpabi_f4__p0_0,__zpabi_f4__p0_1,__zpabi_f4__pp_0,__zpabi_f4__pp_1 locals=__local_f4__0,__local_f4__1,__local_f4__2,__local_f4__3,__local_f4__4,__local_f4__5,__local_f4__6,__local_f4__7,__local_f4__p0_1,__local_f4__8,__local_f4__9,__local_f4__10,__local_f4__11,__local_f4__12,__local_f4__p0_0,__local_f4__13 indirect=false in_cycle=false param_regs=A,X,,
; def f5 params=__zpabi_f5__p0_0,__zpabi_f5__p0_1 locals=__local_f5__p0 indirect=false in_cycle=false param_regs=A,X
; def main params= locals=__local_main__0,__local_main__1,__local_main__2,__local_main__3,__local_main__4,__local_main__5,__local_main__6,__local_main__7,__local_main__8,__local_main__mbox,__local_main__10,__local_main__11,__local_main__12 indirect=false in_cycle=false
; call f0 -> f5
; call f1 -> f2
; call f1 -> f3
; call f2 -> f4
; call f4 -> f5
; call main -> f0
; item f5 section=.text local=false zero=false
; item f4 section=.text local=false zero=false
; item f4@run4 section=.text local=true zero=false
; item f4@run2 section=.text local=true zero=false
; item f4@run1 section=.text local=true zero=false
; item f4@run0 section=.text local=true zero=false
; item f3 section=.text local=false zero=false
; item f2 section=.text local=false zero=false
; item f1 section=.text local=false zero=false
; item f1@run3 section=.text local=true zero=false
; item f0 section=.text local=false zero=false
; item main section=.text local=false zero=false
; item g0 section=.data local=false zero=false
; item __zp_init section=.text local=false zero=false
; zp g1 size=1 at=auto def=true init=$91 weight=28
; zp g2 size=5 at=auto def=true init=$28,$00,$00,$00,$00 weight=16
; runtime hargs_size=32 hargs_need=26
; @zp-link-meta-end

f5:
   SUBROUTINE

   RTS

f4:
   SUBROUTINE

   STA   __local_f4__p0_0
   STX   __local_f4__p0_1
   LDA   #$FF
   CLC
   ADC   __local_f4__p0_0
   STA   __local_f4__2
   TXA
   ADC   #$7F
   STA   __local_f4__1
   LDA   g0
   ORA   g0+1
   ORA   g0+2
   ORA   g0+3
   BEQ   .lnot_true@0
   LDA   #$00
   BEQ   .lnot_end@1
.lnot_true@0:
   LDA   #$01
.lnot_end@1:
   STA   __local_f4__0
   LDA   __local_f4__2
   EOR   __local_f4__0
   STA   __local_f4__12
   LDA   __local_f4__1
   STA   __local_f4__11
   BMI   .sx_neg@2
   LDA   #$00
   BEQ   .sx_done@3
.sx_neg@2:
   LDA   #$FF
.sx_done@3:
   STA   __local_f4__10
   STA   __local_f4__9
   STA   __local_f4__8
   LDA   #$01
   SEC
   SBC   __local_f4__12
   LDA   #$00
   SBC   __local_f4__11
   LDA   #$00
   SBC   __local_f4__10
   LDA   #$00
   SBC   __local_f4__9
   LDA   #$00
   SBC   __local_f4__8
   BVC   .cmp_novf@4
   EOR   #$80
.cmp_novf@4:
   ASL
   LDA   #$00
   ROL
   STA   __local_f4__7
   LDA   #$00
   BPL   .sx_done@6
   LDA   #$FF
.sx_done@6:
   STA   __local_f4__6
   STA   __local_f4__5
   TXA
   BMI   .sx_neg@7
   LDA   #$00
   BEQ   .sx_done@8
.sx_neg@7:
   LDA   #$FF
.sx_done@8:
   STA   __local_f4__4
   STA   __local_f4__3
   LDY   #$00
   LDA   (__zpabi_f4__pp_0),Y
   STA   __local_f4__2
   INY
   LDA   (__zpabi_f4__pp_0),Y
   STA   __local_f4__1
   LDA   g2
   SEC
   SBC   __local_f4__7
   LDA   g2+1
   SBC   #$00
   LDA   g2+2
   SBC   __local_f4__6
   LDA   g2+3
   SBC   __local_f4__5
   LDA   g2+4
   SBC   #$00
   BVC   .cmp_novf@9
   EOR   #$80
.cmp_novf@9:
   JSR   f4@run2
   CLC
   ADC   __local_f4__2
   BCC   .carry_inc@0
   INC   __local_f4__1
.carry_inc@0:
   LDY   #$00
   STA   (__zpabi_f4__pp_0),Y
   LDA   __local_f4__1
   INY
   STA   (__zpabi_f4__pp_0),Y
   LDA   __local_f4__7
   CLC
   SBC   __local_f4__p0_0
   LDA   #$00
   SBC   __local_f4__p0_1
   LDA   __local_f4__6
   SBC   __local_f4__4
   LDA   __local_f4__5
   SBC   __local_f4__3
   LDA   #$00
   ROL
   LDX   #$00
   JSR   f5
   STA   __local_f4__1
   LDA   __local_f4__7
   ORA   __local_f4__p0_0
   STA   __local_f4__0
   LDA   __local_f4__8
   BNE   .thread@0
   LDA   __local_f4__9
   CMP   __local_f4__5
   BNE   .thread@0
   LDA   __local_f4__10
   CMP   __local_f4__6
   BNE   .thread@0
   LDA   __local_f4__11
   BNE   .thread@0
   LDA   __local_f4__12
   CMP   __local_f4__7
   BEQ   .cmp_true@10
.thread@0:
   LDA   #$00
   BEQ   .cmp_end@11
.cmp_true@10:
   LDA   #$01
.cmp_end@11:
   STA   HARGS+1
   LDA   __local_f4__0
   STA   HARGS
   JSR   mul8
   LDA   HARGS+2
   EOR   __local_f4__1
   JSR   f4@run4
   LDA   __local_f4__7
   SEC
   SBC   #$03
   LDA   #$00
   SBC   #$00
   LDA   __local_f4__6
   SBC   #$00
   LDA   __local_f4__5
   SBC   #$00
   LDA   #$00
   ROL
   STA   __local_f4__p0_1
   LDA   #$00
   BPL   .sx_done@14
   LDA   #$FF
.sx_done@14:
   STA   __local_f4__7
   STA   __local_f4__6
   LDY   #$00
   LDA   (__zpabi_f4__pp_0),Y
   STA   __local_f4__3
   INY
   LDA   (__zpabi_f4__pp_0),Y
   STA   __local_f4__2
   BMI   .sx_neg@15
   LDA   #$00
   BEQ   .sx_done@16
.sx_neg@15:
   LDA   #$FF
.sx_done@16:
   STA   HARGS+2
   STA   HARGS+3
   STA   __local_f4__1
   LDA   __local_f4__12
   ORA   #$01
   STA   HARGS+8
   LDA   __local_f4__11
   STA   HARGS+9
   LDA   __local_f4__10
   STA   HARGS+10
   LDA   __local_f4__9
   STA   HARGS+11
   LDA   __local_f4__8
   STA   __local_f4__0
   LDA   __local_f4__3
   STA   HARGS
   LDA   __local_f4__2
   STA   HARGS+1
   LDA   __local_f4__1
   JSR   f4@run1
   LDA   __local_f4__0
   JSR   f4@run0
   LDA   __local_f4__p0_1
   CLC
   ADC   HARGS+16
   STA   HARGS+8
   LDA   #$00
   ADC   HARGS+17
   STA   HARGS+9
   LDA   __local_f4__7
   ADC   HARGS+18
   STA   HARGS+10
   LDA   __local_f4__6
   ADC   HARGS+19
   STA   HARGS+11
   RTS

f4@run4:
   SUBROUTINE

   CLC
   ADC   g1
   STA   g1
   RTS

f4@run2:
   SUBROUTINE

   ASL
   LDA   #$00
   ROL
   EOR   #$01
   RTS

f4@run1:
   SUBROUTINE

   STA   HARGS+4
   ASL
   LDA   #$00
   ADC   #$FF
   EOR   #$FF
   STA   HARGS+5
   STA   HARGS+6
   STA   HARGS+7
   RTS

f4@run0:
   SUBROUTINE

   STA   HARGS+12
   ASL
   LDA   #$00
   ADC   #$FF
   EOR   #$FF
   STA   HARGS+13
   STA   HARGS+14
   STA   HARGS+15
   JMP   sdivmod64

f3:
   SUBROUTINE

   LDA   g0
   LDX   g0+1
   RTS

f2:
   SUBROUTINE

   LDA   g1
   STA   __local_f2__6
   LDA   #$00
   BPL   .sx_done@18
   LDA   #$FF
.sx_done@18:
   CMP   __zpabi_f2__p0_4
   BNE   .thread@1
   CMP   __zpabi_f2__p0_3
   BNE   .thread@1
   CMP   __zpabi_f2__p0_2
   BNE   .thread@1
   LDA   #$00
   CMP   __zpabi_f2__p0_1
   BNE   .cmp_end@20
   LDA   g1
   CMP   __zpabi_f2__p0_0
   BEQ   .cmp_true@19
.thread@1:
   LDA   #$00
   BEQ   .cmp_end@20
.cmp_true@19:
   LDA   #$01
.cmp_end@20:
   ORA   g0
   BEQ   .and_false@4
   LDA   __zpabi_f2__p2
   BEQ   .and_false@4
   LDA   #$00
   STA   __local_f2__5
   STA   __local_f2__4
   JMP   .sx_done@23
.and_false@4:
   LDA   #$01
   STA   __local_f2__5
   LDA   #$00
   STA   __local_f2__4
   BPL   .sx_done@23
   LDA   #$FF
.sx_done@23:
   STA   __local_f2__2
   STA   __local_f2__1
   STA   __local_f2__0
   LDA   __zpabi_f2__p0_0
   SEC
   SBC   g1
   LDA   __zpabi_f2__p0_1
   SBC   #$00
   LDA   __zpabi_f2__p0_2
   SBC   __local_f2__2
   LDA   __zpabi_f2__p0_3
   SBC   __local_f2__1
   LDA   __zpabi_f2__p0_4
   SBC   __local_f2__0
   BVC   .cmp_novf@24
   EOR   #$80
.cmp_novf@24:
   JSR   f4@run2
   CLC
   ADC   #$01
   STA   __local_f2__l3_0
   LDA   #$00
   ADC   #$00
   STA   __local_f2__l3_1
   LDA   __local_f2__6
   BMI   .sx_neg@25
   LDA   #$00
   BEQ   .sx_done@26
.sx_neg@25:
   LDA   #$FF
.sx_done@26:
   STA   __local_f2__3
   STA   __local_f2__box4+1
   LDA   __local_f2__6
   STA   __local_f2__box4
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
   BVC   .jcmp_novf@27
   EOR   #$80
.jcmp_novf@27:
   BPL   .loop@0_break
   LDA   __local_f2__5
   SEC
   SBC   __local_f2__6
   LDA   __local_f2__4
   SBC   __local_f2__3
   LDA   #$00
   ROL
   STA   __local_f2__2
   LDA   #<__local_f2__box4
   STA   __zpabi_f4__pp_0
   LDA   #>__local_f2__box4
   STA   __zpabi_f4__pp_1
   LDX   #$00
   LDA   __local_f2__2
   JSR   f4
   LDA   HARGS+8
   CLC
   ADC   __local_f2__it_0
   STA   __local_f2__1
   LDA   HARGS+9
   ADC   __local_f2__it_1
   STA   __local_f2__0
   LDA   __local_f2__l3_0
   CLC
   ADC   __local_f2__1
   STA   __local_f2__l3_0
   LDA   __local_f2__l3_1
   ADC   __local_f2__0
   STA   __local_f2__l3_1
   INC   __local_f2__it_0
   BNE   .loop@0_start
   INC   __local_f2__it_1
   JMP   .loop@0_start
.loop@0_break:
   LDA   __local_f2__l3_0
   CLC
   ADC   __local_f2__box4
   JSR   f4@run4
   LDA   __local_f2__5
   CLC
   SBC   __zpabi_f2__p2
   LDA   __local_f2__4
   SBC   #$00
   LDA   #$00
   ROL
   LDX   #$00
   RTS

f1:
   SUBROUTINE

   LDA   g0+3
   BMI   .sx_neg@28
   LDA   #$00
   BEQ   .sx_done@29
.sx_neg@28:
   LDA   #$FF
.sx_done@29:
   STA   __local_f1__0
   LDA   g2+4
   CMP   __local_f1__0
   BNE   .thread@2
   LDA   g2+3
   CMP   g0+3
   BNE   .thread@2
   LDA   g2+2
   CMP   g0+2
   BNE   .thread@2
   LDA   g2+1
   CMP   g0+1
   BNE   .thread@2
   LDA   g2
   CMP   g0
   BEQ   .cmp_true@30
.thread@2:
   LDA   #$00
   BEQ   .cmp_end@31
.cmp_true@30:
   LDA   #$01
.cmp_end@31:
   STA   __local_f1__8
   LDA   #$00
   BPL   .sx_done@34
   LDA   #$FF
.sx_done@34:
   STA   __local_f1__7
   STA   __local_f1__6
   STA   __local_f1__5
   LDA   g1
   ORA   #$01
   STA   __zpabi_f3__p2_0
   LDA   #$00
   BPL   .sx_done@36
   LDA   #$FF
.sx_done@36:
   STA   HARGS+10
   STA   HARGS+11
   STA   __local_f1__0
   LDA   g2
   STA   HARGS
   LDA   g2+1
   STA   HARGS+1
   LDA   g2+2
   STA   HARGS+2
   LDA   g2+3
   STA   HARGS+3
   LDA   g2+4
   JSR   f4@run1
   LDA   __zpabi_f3__p2_0
   STA   HARGS+8
   LDA   #$00
   STA   HARGS+9
   LDA   __local_f1__0
   JSR   f4@run0
   LDA   __local_f1__8
   SEC
   SBC   HARGS+16
   LDA   #$00
   SBC   HARGS+17
   LDA   __local_f1__7
   SBC   HARGS+18
   LDA   __local_f1__6
   SBC   HARGS+19
   LDA   __local_f1__5
   SBC   HARGS+20
   BVC   .cmp_novf@37
   EOR   #$80
.cmp_novf@37:
   JSR   f4@run2
   STA   __local_f1__11
   LDA   g1
   EOR   g1
   STA   __local_f1__0
   LDA   g0
   SEC
   SBC   __local_f1__0
   LDA   g0+1
   SBC   #$00
   LDA   g0+2
   SBC   #$00
   LDA   g0+3
   SBC   #$00
   BVC   .cmp_novf@38
   EOR   #$80
.cmp_novf@38:
   JSR   f4@run2
   STA   __local_f1__l1
   LDA   g2
   STA   __local_f1__10
   LDA   #$00
   STA   __local_f1__it_0
   LDY   #$00
   STY   __local_f1__it_1
.loop@1_start:
   LDA   __local_f1__it_0
   SEC
   SBC   #$03
   LDA   __local_f1__it_1
   SBC   #$00
   BVC   .jcmp_novf@39
   EOR   #$80
.jcmp_novf@39:
   BMI   .lb_skip@1
   JMP   .loop@1_break
.lb_skip@1:
   SEC
   LDA   #$00
   SBC   g1
   STA   __zpabi_f3__p1_0
   LDA   #$00
   SBC   #$00
   STA   __local_f1__8
   BMI   .sx_neg@40
   LDA   #$00
   BEQ   .sx_done@41
.sx_neg@40:
   LDA   #$FF
.sx_done@41:
   STA   __local_f1__7
   LDA   __local_f1__10
   BEQ   .and_false@10
   JSR   f1@run3
   BEQ   .and_false@10
   LDA   #$01
   STA   __zpabi_f3__p2_0
   LDA   #$00
   BEQ   .and_end@11
.and_false@10:
   LDA   #$00
   STA   __zpabi_f3__p2_0
.and_end@11:
   ORA   #$00
   BPL   .sx_done@43
   LDA   #$FF
.sx_done@43:
   STA   __zpabi_f3__p2_2
   STA   __zpabi_f3__p2_3
   LDA   __local_f1__8
   STA   __zpabi_f3__p1_1
   LDA   __local_f1__7
   STA   __zpabi_f3__p1_2
   STA   __zpabi_f3__p1_3
   STA   __zpabi_f3__p1_4
   STA   __zpabi_f3__p1_5
   STA   __zpabi_f3__p1_6
   STA   __zpabi_f3__p1_7
   LDA   #$00
   STA   __zpabi_f3__p2_1
   LDA   __local_f1__l1
   JSR   f3
   CLC
   ADC   __local_f1__it_0
   CLC
   ADC   __local_f1__l1
   STA   __local_f1__l1
   INC   __local_f1__it_0
   BEQ   .lb_skip@0
   JMP   .loop@1_start
.lb_skip@0:
   INC   __local_f1__it_1
   JMP   .loop@1_start
.loop@1_break:
   LDA   g1
   EOR   #$FF
   STA   __local_f1__8
   LDA   #$00
   EOR   #$FF
   STA   __local_f1__7
   BMI   .sx_neg@44
   LDA   #$00
   BEQ   .sx_done@45
.sx_neg@44:
   LDA   #$FF
.sx_done@45:
   STA   __local_f1__6
   LDA   __local_f1__10
   BMI   .sx_neg@46
   LDA   #$00
   BEQ   .sx_done@47
.sx_neg@46:
   LDA   #$FF
.sx_done@47:
   STA   __local_f1__0
   LDA   #$02
   SEC
   SBC   __local_f1__10
   LDA   #$00
   SBC   __local_f1__0
   BVC   .cmp_novf@48
   EOR   #$80
.cmp_novf@48:
   ASL
   LDA   #$00
   ROL
   STA   __local_f1__3
   LDA   __local_f1__l1
   EOR   g2
   STA   __zpabi_f2__p2
   LDA   __local_f1__8
   STA   __zpabi_f2__p0_0
   LDA   __local_f1__7
   STA   __zpabi_f2__p0_1
   LDA   __local_f1__6
   STA   __zpabi_f2__p0_2
   STA   __zpabi_f2__p0_3
   STA   __zpabi_f2__p0_4
   LDX   #$00
   LDA   __local_f1__3
   JSR   f2
   STA   __local_f1__0
   LDA   __local_f1__10
   EOR   __local_f1__0
   JSR   f4@run4
   SEC
   LDA   #$00
   SBC   __local_f1__11
   RTS

f1@run3:
   SUBROUTINE

   LDA   g2
   ORA   g2+1
   ORA   g2+2
   ORA   g2+3
   ORA   g2+4
   RTS

f0:
   SUBROUTINE

   STA   __local_f0__pp
   STX   __local_f0__14
   LDA   #$00
   BPL   .sx_done@52
   LDA   #$FF
.sx_done@52:
   CMP   g0+3
   BNE   .cmp_true@53
   CMP   g0+2
   BNE   .cmp_true@53
   LDA   #$00
   CMP   g0+1
   BNE   .cmp_true@53
   LDA   g1
   CMP   g0
   BNE   .cmp_true@53
   LDA   #$00
   BEQ   .cmp_end@54
.cmp_true@53:
   LDA   #$01
.cmp_end@54:
   STA   __local_f0__13
   LDA   #$00
   BPL   .sx_done@57
   LDA   #$FF
.sx_done@57:
   STA   __local_f0__12
   STA   __local_f0__11
   LDA   g1
   STA   __local_f0__10
   JSR   f1@run3
   BNE   .or_true@12
   LDA   __local_f0__10
   SEC
   SBC   #$08
   LDA   #$00
   SBC   #$00
   BCS   .or_true@12
   LDA   #$00
   STA   __local_f0__7
   JMP   .or_end@13
.or_true@12:
   LDA   #$01
   STA   __local_f0__7
   LDA   #$00
.or_end@13:
   ORA   #$00
   BPL   .sx_done@59
   LDA   #$FF
.sx_done@59:
   STA   __local_f0__5
   STA   __local_f0__9
   STA   __local_f0__8
   LDA   g0+3
   BMI   .sx_neg@60
   LDA   #$00
   BEQ   .sx_done@61
.sx_neg@60:
   LDA   #$FF
.sx_done@61:
   STA   __local_f0__0
   LDA   __local_f0__7
   CLC
   ADC   g0
   STA   __local_f0__7
   LDA   #$00
   ADC   g0+1
   STA   __local_f0__6
   LDA   __local_f0__5
   ADC   g0+2
   STA   __local_f0__5
   LDA   __local_f0__9
   ADC   g0+3
   STA   __local_f0__4
   LDA   __local_f0__8
   ADC   __local_f0__0
   STA   __local_f0__3
   LDA   g0+3
   CMP   __local_f0__11
   BNE   .thread@3
   LDA   g0+2
   CMP   __local_f0__12
   BNE   .thread@3
   LDA   g0+1
   BNE   .thread@3
   LDA   g0
   CMP   __local_f0__13
   BEQ   .cmp_true@62
.thread@3:
   LDA   #$00
   BEQ   .cmp_end@63
.cmp_true@62:
   LDA   #$01
.cmp_end@63:
   ORA   #$01
   STA   __local_f0__2
   LDA   #$00
   BPL   .sx_done@66
   LDA   #$FF
.sx_done@66:
   STA   HARGS+10
   STA   HARGS+11
   STA   __local_f0__0
   LDA   __local_f0__7
   STA   HARGS
   LDA   __local_f0__6
   STA   HARGS+1
   LDA   __local_f0__5
   STA   HARGS+2
   LDA   __local_f0__4
   STA   HARGS+3
   LDA   __local_f0__3
   JSR   f4@run1
   LDA   __local_f0__2
   STA   HARGS+8
   LDA   #$00
   STA   HARGS+9
   LDA   __local_f0__0
   JSR   f4@run0
   LDY   #$00
   LDA   (__local_f0__pp),Y
   CLC
   ADC   HARGS+24
   STA   __local_f0__1
   INY
   LDA   (__local_f0__pp),Y
   ADC   HARGS+25
   STA   (__local_f0__pp),Y
   LDA   __local_f0__1
   DEY
   STA   (__local_f0__pp),Y
   LDA   __local_f0__10
   ORA   #$01
   STA   HARGS+1
   LDA   #$FF
   STA   HARGS
   JSR   udivmod8
   LDA   HARGS+3
   LDX   #$00
   JSR   f5
   STA   __local_f0__0
   LDA   __local_f0__13
   EOR   __local_f0__0
   JSR   f4@run4
   LDA   __local_f0__10
   BEQ   .cmp_end@67
   SEC
   RTS
.cmp_end@67:
   CMP   #$01
   RTS

main:
   SUBROUTINE

   LDA   #$05
   STA   __local_main__mbox
   LDA   #$00
   STA   __local_main__mbox+1
   LDX   #>__local_main__mbox
   LDA   #<__local_main__mbox
   JSR   f0
   LDA   #$00
   ROL
   STA   __local_main__8
   LDA   g0
   EOR   g1
   EOR   g2
   STA   __local_main__3
   LDA   g0+1
   EOR   g2+1
   STA   __local_main__2
   LDA   g0+2
   EOR   g2+2
   STA   __local_main__1
   LDA   g0+3
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

g0:
   DC.L  $000000AC

__zp_init:
   SUBROUTINE

   LDA   #$00
   STA   g2+1
   STA   g2+2
   STA   g2+3
   STA   g2+4
   LDA   #$91
   STA   g1
   LDA   #$28
   STA   g2
   RTS
