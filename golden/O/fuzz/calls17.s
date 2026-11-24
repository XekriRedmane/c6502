SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
g1	EQU	$26
g0	EQU	$28
__local_main__0	EQU	$80
__zpabi_f1__p0_0	EQU	$80
__zpabi_f2__pp_0	EQU	$80
__local_main__1	EQU	$81
__zpabi_f1__p0_1	EQU	$81
__zpabi_f2__pp_1	EQU	$81
__local_f2__0	EQU	$82
__local_main__2	EQU	$82
__zpabi_f1__p0_2	EQU	$82
__local_f2__1	EQU	$83
__local_main__3	EQU	$83
__zpabi_f1__p0_3	EQU	$83
__zpabi_f1__p0_4	EQU	$84
__local_f2__3	EQU	$85
__zpabi_f1__p0_5	EQU	$85
__local_f2__4	EQU	$86
__zpabi_f1__p0_6	EQU	$86
__zpabi_f1__p0_7	EQU	$87
__local_f2__6	EQU	$88
__zpabi_f1__p1_0	EQU	$88
__zpabi_f1__p1_1	EQU	$89
__zpabi_f1__p1_2	EQU	$8A
__zpabi_f1__p1_3	EQU	$8B
__local_f2__p0	EQU	$8C
__zpabi_f1__p2_0	EQU	$8C
__local_f2__box5	EQU	$8D
__zpabi_f1__p2_1	EQU	$8D
__local_f0__0	EQU	$8E
__zpabi_f1__p2_2	EQU	$8E
__local_f0__1	EQU	$8F
__zpabi_f1__p2_3	EQU	$8F
__local_f0__2	EQU	$90
__zpabi_f5__p0_0	EQU	$90
__local_f0__3	EQU	$91
__zpabi_f5__p0_1	EQU	$91
__zpabi_f5__p0_2	EQU	$92
__zpabi_f5__p0_3	EQU	$93
__zpabi_f5__p2_0	EQU	$94
__zpabi_f5__p2_1	EQU	$95
__zpabi_f5__p2_2	EQU	$96
__zpabi_f5__p2_3	EQU	$97
__zpabi_f5__p2_4	EQU	$98
__zpabi_f5__p2_5	EQU	$99
__zpabi_f5__p2_6	EQU	$9A
__zpabi_f5__p2_7	EQU	$9B
__zpabi_f5__p3_0	EQU	$9C
__zpabi_f5__p3_1	EQU	$9D
__zpabi_f5__p3_2	EQU	$9E
__zpabi_f5__p3_3	EQU	$9F
__zpabi_f5__pp_0	EQU	$A0
__zpabi_f5__pp_1	EQU	$A1
__local_f4__0	EQU	$A3
__local_f4__1	EQU	$A4
__local_f4__4	EQU	$A7
__local_f4__5	EQU	$A8
__local_f4__6	EQU	$A9
__local_f4__7	EQU	$AA
__local_f4__12	EQU	$AF
__local_f4__13	EQU	$B0
__local_f4__18	EQU	$B5
__local_f4__19	EQU	$B6
__local_f4__20	EQU	$B7
__local_f4__21	EQU	$B8
__local_f4__22	EQU	$B9
__local_f4__box5	EQU	$BA
__local_f5__1	EQU	$BD
__local_f5__2	EQU	$BE
__local_f5__3	EQU	$BF

; @zp-link-meta-begin
; def f0 params= locals=__local_f0__0,__local_f0__1,__local_f0__2,__local_f0__3 indirect=false in_cycle=false
; def f1 params=__zpabi_f1__p0_0,__zpabi_f1__p0_1,__zpabi_f1__p0_2,__zpabi_f1__p0_3,__zpabi_f1__p0_4,__zpabi_f1__p0_5,__zpabi_f1__p0_6,__zpabi_f1__p0_7,__zpabi_f1__p1_0,__zpabi_f1__p1_1,__zpabi_f1__p1_2,__zpabi_f1__p1_3,__zpabi_f1__p2_0,__zpabi_f1__p2_1,__zpabi_f1__p2_2,__zpabi_f1__p2_3 locals=__local_f1__0 indirect=false in_cycle=false carry=true
; def f2 params=__zpabi_f2__p0,__zpabi_f2__pp_0,__zpabi_f2__pp_1 locals=__local_f2__0,__local_f2__1,__local_f2__2,__local_f2__3,__local_f2__4,__local_f2__5,__local_f2__6,__local_f2__7,__local_f2__8,__local_f2__9,__local_f2__p0,__local_f2__box5,__local_f2__11 indirect=false in_cycle=false param_regs=A,,
; def f3 params=__zpabi_f3__p0_0,__zpabi_f3__p0_1 locals=__local_f3__p0 indirect=false in_cycle=false param_regs=A,X
; def f4 params= locals=__local_f4__0,__local_f4__1,__local_f4__2,__local_f4__3,__local_f4__4,__local_f4__5,__local_f4__6,__local_f4__7,__local_f4__8,__local_f4__9,__local_f4__10,__local_f4__11,__local_f4__12,__local_f4__13,__local_f4__14,__local_f4__15,__local_f4__16,__local_f4__17,__local_f4__18,__local_f4__19,__local_f4__20,__local_f4__21,__local_f4__22,__local_f4__box5,__local_f4__24 indirect=false in_cycle=false carry=true
; def f5 params=__zpabi_f5__p0_0,__zpabi_f5__p0_1,__zpabi_f5__p0_2,__zpabi_f5__p0_3,__zpabi_f5__p1_0,__zpabi_f5__p1_1,__zpabi_f5__p2_0,__zpabi_f5__p2_1,__zpabi_f5__p2_2,__zpabi_f5__p2_3,__zpabi_f5__p2_4,__zpabi_f5__p2_5,__zpabi_f5__p2_6,__zpabi_f5__p2_7,__zpabi_f5__p3_0,__zpabi_f5__p3_1,__zpabi_f5__p3_2,__zpabi_f5__p3_3,__zpabi_f5__pp_0,__zpabi_f5__pp_1 locals=__local_f5__0,__local_f5__1,__local_f5__2,__local_f5__3 indirect=false in_cycle=false param_regs=,,,,A,X,,,,,,,,,,,,,,
; def main params= locals=__local_main__0,__local_main__1,__local_main__2,__local_main__3,__local_main__4,__local_main__5,__local_main__6,__local_main__7,__local_main__8,__local_main__9,__local_main__10,__local_main__11,__local_main__12,__local_main__13 indirect=false in_cycle=false
; call f1 -> f4
; call f2 -> f4
; call f2 -> f5
; call f4 -> f5
; call main -> f0
; item f5 section=.text local=false zero=false
; item f4 section=.text local=false zero=false
; item f4@run0 section=.text local=true zero=false
; item f3 section=.text local=false zero=false
; item f2 section=.text local=false zero=false
; item f1 section=.text local=false zero=false
; item f0 section=.text local=false zero=false
; item main section=.text local=false zero=false
; item __zp_init section=.text local=false zero=false
; zp g1 size=2 at=auto def=true init=$C4,$00 weight=14
; zp g0 size=4 at=auto def=true init=$4A,$00,$00,$00 weight=16
; runtime hargs_size=32 hargs_need=24
; @zp-link-meta-end

f5:
   SUBROUTINE

   LDY   #$00
   LDA   (__zpabi_f5__pp_0),Y
   STA   __local_f5__3
   INY
   LDA   (__zpabi_f5__pp_0),Y
   STA   __local_f5__2
   LDA   __zpabi_f5__p2_0
   ORA   #$01
   STA   HARGS+8
   LDA   __zpabi_f5__p2_1
   STA   HARGS+9
   STA   HARGS+1
   LDA   __zpabi_f5__p2_2
   STA   HARGS+10
   STA   HARGS+2
   LDA   __zpabi_f5__p2_3
   STA   HARGS+11
   STA   HARGS+3
   LDA   __zpabi_f5__p2_4
   STA   HARGS+12
   STA   HARGS+4
   LDA   __zpabi_f5__p2_5
   STA   HARGS+13
   STA   HARGS+5
   LDA   __zpabi_f5__p2_6
   STA   HARGS+14
   STA   HARGS+6
   LDA   __zpabi_f5__p2_0
   STA   HARGS
   LDA   __zpabi_f5__p2_7
   STA   HARGS+15
   STA   HARGS+7
   JSR   udivmod64
   LDA   __local_f5__3
   CLC
   ADC   HARGS+16
   STA   __local_f5__1
   LDA   __local_f5__2
   ADC   HARGS+17
   LDY   #$01
   STA   (__zpabi_f5__pp_0),Y
   LDA   __local_f5__1
   DEY
   STA   (__zpabi_f5__pp_0),Y
   LDA   (__zpabi_f5__pp_0),Y
   STA   __local_f5__1
   LDA   g1
   SEC
   SBC   __local_f5__1
   RTS

f4:
   SUBROUTINE

   LDA   g1
   STA   __local_f4__22
   LDA   g1+1
   STA   __local_f4__21
   BMI   .sx_neg@0
   LDA   #$00
   BEQ   .sx_done@1
.sx_neg@0:
   LDA   #$FF
.sx_done@1:
   STA   __local_f4__20
   STA   __local_f4__19
   LDA   __local_f4__22
   ORA   __local_f4__21
   ORA   __local_f4__20
   ORA   __local_f4__19
   BNE   .or_true@0
   LDA   #$02
   SEC
   SBC   g0
   STA   __zpabi_f5__p3_0
   LDA   #$00
   SBC   g0+1
   STA   __zpabi_f5__p3_1
   LDA   #$00
   SBC   g0+2
   STA   __local_f4__1
   LDA   #$00
   SBC   g0+3
   STA   __local_f4__0
   LDA   __zpabi_f5__p3_0
   ORA   __zpabi_f5__p3_1
   ORA   __local_f4__1
   ORA   __local_f4__0
   BNE   .or_true@0
   LDA   #$00
   STA   __local_f4__18
   JMP   .or_end@1
.or_true@0:
   LDA   #$01
   STA   __local_f4__18
.or_end@1:
   LDA   __local_f4__22
   STA   __local_f4__box5
   LDA   __local_f4__21
   STA   __local_f4__box5+1
   STA   __zpabi_f5__p3_0
   ASL
   ROR   __zpabi_f5__p3_0
   LDA   g1
   ROR
   STA   __zpabi_f5__p3_1
   LDA   __zpabi_f5__p3_0
   BMI   .sx_neg@2
   LDA   #$00
   BEQ   .sx_done@3
.sx_neg@2:
   LDA   #$FF
.sx_done@3:
   LDX   __zpabi_f5__p3_1
   STX   __zpabi_f5__p0_0
   LDX   __zpabi_f5__p3_0
   STX   __zpabi_f5__p0_1
   STA   __zpabi_f5__p0_2
   AND   #$01
   STA   __zpabi_f5__p0_3
   LDA   g0
   EOR   g1
   STA   __local_f4__13
   LDA   g0+1
   EOR   g1+1
   TAX
   LDA   __local_f4__18
   BMI   .sx_neg@4
   LDA   #$00
   BEQ   .sx_done@5
.sx_neg@4:
   LDA   #$FF
.sx_done@5:
   STA   __local_f4__12
   LDA   __local_f4__18
   BMI   .sx_neg@6
   LDA   #$00
   BEQ   .sx_done@7
.sx_neg@6:
   LDA   #$FF
.sx_done@7:
   STA   __zpabi_f5__p3_1
   STA   __local_f4__1
   STA   __local_f4__0
   LDA   __local_f4__18
   SEC
   SBC   __local_f4__22
   LDA   __zpabi_f5__p3_1
   SBC   __local_f4__21
   LDA   __local_f4__1
   SBC   __local_f4__20
   LDA   __local_f4__0
   SBC   __local_f4__19
   BVC   .cmp_novf@8
   EOR   #$80
.cmp_novf@8:
   ASL
   LDA   #$00
   ROL
   EOR   #$01
   STA   __local_f4__6
   LDA   #$00
   BPL   .sx_done@10
   LDA   #$FF
.sx_done@10:
   LDY   __local_f4__6
   STY   __zpabi_f5__p2_0
   STA   __zpabi_f5__p2_2
   AND   #$07
   STA   __local_f4__4
   LDA   __local_f4__18
   ORA   __local_f4__18
   STA   __zpabi_f5__p3_0
   LDA   __local_f4__12
   ORA   __local_f4__12
   STA   __zpabi_f5__p3_1
   BMI   .sx_neg@11
   LDA   #$00
   BEQ   .sx_done@12
.sx_neg@11:
   LDA   #$FF
.sx_done@12:
   STA   __zpabi_f5__p3_2
   STA   __zpabi_f5__p3_3
   LDA   #$00
   JSR   f4@run0
   LDA   __local_f4__4
   STA   __zpabi_f5__p2_7
   LDA   #<__local_f4__box5
   STA   __zpabi_f5__pp_0
   LDA   #>__local_f4__box5
   STA   __zpabi_f5__pp_1
   LDA   __local_f4__13
   JSR   f5
   STA   __local_f4__0
   LDA   __local_f4__22
   EOR   __local_f4__0
   STA   __local_f4__7
   LDA   __local_f4__21
   STA   __local_f4__6
   LDA   __local_f4__20
   STA   __local_f4__5
   LDA   __local_f4__19
   STA   __local_f4__4
   LDA   __local_f4__box5
   STA   __zpabi_f5__p3_0
   LDA   __local_f4__box5+1
   STA   __zpabi_f5__p3_1
   BMI   .sx_neg@13
   LDA   #$00
   BEQ   .sx_done@14
.sx_neg@13:
   LDA   #$FF
.sx_done@14:
   STA   __local_f4__1
   STA   __local_f4__0
   LDA   __local_f4__7
   CLC
   ADC   __zpabi_f5__p3_0
   STA   HARGS
   LDA   __local_f4__6
   ADC   __zpabi_f5__p3_1
   STA   HARGS+1
   LDA   __local_f4__5
   ADC   __local_f4__1
   STA   HARGS+2
   LDA   __local_f4__4
   ADC   __local_f4__0
   STA   HARGS+3
   LDA   g0
   AND   #$64
   STA   __local_f4__7
   LDA   #$00
   STA   __local_f4__6
   LDY   #$00
   STY   __local_f4__5
   STA   __local_f4__4
   LDA   g1
   AND   #$07
   STA   HARGS+4
   JSR   asr32
   LDA   __local_f4__7
   SEC
   SBC   HARGS+5
   LDA   __local_f4__6
   SBC   HARGS+6
   LDA   __local_f4__5
   SBC   HARGS+7
   LDA   __local_f4__4
   SBC   HARGS+8
   RTS

f4@run0:
   SUBROUTINE

   STA   __zpabi_f5__p2_1
   LDA   __zpabi_f5__p2_2
   STA   __zpabi_f5__p2_3
   STA   __zpabi_f5__p2_4
   STA   __zpabi_f5__p2_5
   STA   __zpabi_f5__p2_6
   RTS

f3:
   SUBROUTINE

   RTS

f2:
   SUBROUTINE

   STA   __local_f2__p0
   LDA   g0
   AND   g0
   STA   __local_f2__1
   LDA   g0+1
   AND   g0+1
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
   JSR   f4
   LDA   #$00
   STA   __local_f2__box5
   STA   __local_f2__box5+1
   LDA   g1+1
   BMI   .cmp_true@17
   BNE   .cmp_true@17
   LDA   g1
   BNE   .cmp_true@17
   LDA   #$00
   BEQ   .cmp_end@18
.cmp_true@17:
   LDA   #$01
.cmp_end@18:
   STA   __local_f2__6
   LDA   #$00
   BPL   .sx_done@21
   LDA   #$FF
.sx_done@21:
   LDX   __local_f2__6
   STX   __zpabi_f5__p2_0
   STA   __zpabi_f5__p2_2
   AND   #$07
   STA   __zpabi_f5__p2_7
   LDA   __local_f2__p0
   BMI   .sx_neg@22
   LDA   #$00
   BEQ   .sx_done@23
.sx_neg@22:
   LDA   #$FF
.sx_done@23:
   STA   __zpabi_f5__p3_1
   STA   __zpabi_f5__p3_2
   STA   __zpabi_f5__p3_3
   LDA   #$01
   STA   __zpabi_f5__p0_0
   LDA   #$00
   STA   __zpabi_f5__p0_1
   STA   __zpabi_f5__p0_2
   STA   __zpabi_f5__p0_3
   JSR   f4@run0
   LDA   __local_f2__p0
   STA   __zpabi_f5__p3_0
   LDA   #<__local_f2__box5
   STA   __zpabi_f5__pp_0
   LDA   #>__local_f2__box5
   STA   __zpabi_f5__pp_1
   LDX   #$FF
   TXA
   JSR   f5
   STA   __local_f2__0
   LDA   #$00
   EOR   __local_f2__0
   STA   __local_f2__4
   LDA   __local_f2__box5
   STA   __local_f2__3
   LDA   __local_f2__box5+1
   STA   __zpabi_f5__p2_7
   BMI   .sx_neg@24
   LDA   #$00
   BEQ   .sx_done@25
.sx_neg@24:
   LDA   #$FF
.sx_done@25:
   STA   __local_f2__1
   STA   __local_f2__0
   LDA   __local_f2__4
   CLC
   ADC   __local_f2__3
   STA   __local_f2__3
   LDA   #$00
   ADC   __zpabi_f5__p2_7
   STA   __zpabi_f5__p2_7
   LDA   #$00
   ADC   __local_f2__1
   STA   __local_f2__1
   LDA   #$00
   ADC   __local_f2__0
   STA   __local_f2__0
   LDA   g0
   CLC
   ADC   __local_f2__3
   STA   g0
   LDA   g0+1
   ADC   __zpabi_f5__p2_7
   STA   g0+1
   LDA   g0+2
   ADC   __local_f2__1
   STA   g0+2
   LDA   g0+3
   ADC   __local_f2__0
   STA   g0+3
   LDA   #$01
   STA   HARGS+16
   LDA   #$00
   STA   HARGS+17
   LDY   #$00
   STY   HARGS+18
   STY   HARGS+19
   STY   HARGS+20
   STY   HARGS+21
   STY   HARGS+22
   STA   HARGS+23
   RTS

f1:
   SUBROUTINE

   JSR   f4
   CLC
   RTS

f0:
   SUBROUTINE

   LDA   g0
   ORA   g0
   STA   __local_f0__3
   LDA   g0+1
   ORA   g0+1
   STA   __local_f0__2
   LDA   g0+2
   ORA   g0+2
   STA   __local_f0__1
   LDA   g0+3
   ORA   g0+3
   STA   __local_f0__0
   LDA   __local_f0__3
   EOR   g0
   STA   __local_f0__3
   LDA   __local_f0__2
   EOR   g0+1
   STA   __local_f0__2
   LDA   __local_f0__1
   EOR   g0+2
   STA   __local_f0__1
   LDA   __local_f0__0
   EOR   g0+3
   AND   #$01
   STA   __local_f0__0
   LDA   #$00
   CMP   __local_f0__0
   BNE   .cmp_true@26
   CMP   __local_f0__1
   BNE   .cmp_true@26
   CMP   __local_f0__2
   BNE   .cmp_true@26
   LDA   #$02
   CMP   __local_f0__3
   BNE   .cmp_true@26
   LDA   #$00
   BEQ   .cmp_end@27
.cmp_true@26:
   LDA   #$01
.cmp_end@27:
   SEC
   SBC   g1
   STA   __local_f0__1
   LDA   #$00
   SBC   g1+1
   STA   __local_f0__0
   BMI   .sx_neg@29
   LDA   #$00
   BEQ   .sx_done@30
.sx_neg@29:
   LDA   #$FF
.sx_done@30:
   STA   HARGS+10
   STA   HARGS+11
   LDA   __local_f0__1
   STA   HARGS+8
   LDA   __local_f0__0
   STA   HARGS+9
   RTS

main:
   SUBROUTINE

   JSR   f0
   LDA   g1+1
   BMI   .sx_neg@31
   LDA   #$00
   BEQ   .sx_done@32
.sx_neg@31:
   LDA   #$FF
.sx_done@32:
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
   STA   g0+2
   STA   g0+3
   STA   g1+1
   LDA   #$4A
   STA   g0
   LDA   #$C4
   STA   g1
   RTS
