SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
z4	EQU	$26
g3	EQU	$28
@16.s29	EQU	$29
g2	EQU	$2A
gp7	EQU	$2B
__zpabi_f15__pp17_0	EQU	$80
__zpabi_f23__p25_0	EQU	$80
__zpabi_f15__pp17_1	EQU	$81
__zpabi_f23__p25_1	EQU	$81
__zpabi_f23__p25_2	EQU	$82
__zpabi_f8__p9_0	EQU	$82
__zpabi_f23__p25_3	EQU	$83
__zpabi_f8__p9_1	EQU	$83
__zpabi_f23__p25_4	EQU	$84
__zpabi_f8__p9_2	EQU	$84
__zpabi_f23__p25_5	EQU	$85
__zpabi_f8__p9_3	EQU	$85
__zpabi_f23__p25_6	EQU	$86
__zpabi_f8__pp10_0	EQU	$86
__zpabi_f23__p25_7	EQU	$87
__zpabi_f8__pp10_1	EQU	$87
__zpabi_mix__v_0	EQU	$88
__zpabi_mix__v_1	EQU	$89
__zpabi_mix__v_2	EQU	$8A
__zpabi_mix__v_3	EQU	$8B
__local_f23__0	EQU	$8C
__local_main__0	EQU	$8C
__local_f15__w22	EQU	$8D
__local_f23__1	EQU	$8D
__local_main__1	EQU	$8D
__local_f15__1	EQU	$8E
__local_f23__l26	EQU	$8E
__local_main__w35	EQU	$8E
__local_f15__2	EQU	$8F
__local_f23__p24_1	EQU	$8F
__local_main__l32_3	EQU	$8F
__local_f15__3	EQU	$90
__local_f23__p24_0	EQU	$90
__local_main__l32_0	EQU	$90
__local_f15__4	EQU	$91
__local_main__2	EQU	$91
__local_f15__5	EQU	$92
__local_main__3	EQU	$92
__local_f15__6	EQU	$93
__local_main__4	EQU	$93
__local_f15__7	EQU	$94
__local_main__5	EQU	$94
__local_f15__8	EQU	$95
__local_main__6	EQU	$95
__local_f15__9	EQU	$96
__local_main__7	EQU	$96
__local_f15__10	EQU	$97
__local_main__8	EQU	$97
__local_f15__11	EQU	$98
__local_main__9	EQU	$98
__local_f15__12	EQU	$99
__local_f15__13	EQU	$9A
__local_f15__14	EQU	$9B
__local_main__12	EQU	$9B
__local_f15__l18	EQU	$9C
__local_main__l33	EQU	$9C
__local_f15__15	EQU	$9D
__local_f15__16	EQU	$9E
__local_f15__17	EQU	$9F
__local_f15__l20	EQU	$A0
__local_f8__0	EQU	$A1
__local_f8__w13	EQU	$A2
__local_f8__l11_1	EQU	$A3
__local_f8__l11_0	EQU	$A4
__local_f8__1	EQU	$A5
__local_f8__2	EQU	$A6
__local_f8__3	EQU	$A7
__local_f8__4	EQU	$A8
__local_f8__5	EQU	$A9
__local_f8__6	EQU	$AA
__local_f8__7	EQU	$AB
__local_f8__8	EQU	$AC
__local_f8__9	EQU	$AD
__local_f8__10	EQU	$AE
__local_f8__11	EQU	$AF
__local_f8__12	EQU	$B0
__local_f8__13	EQU	$B1
__local_f8__14	EQU	$B2
__local_mix__0	EQU	$B4
__local_mix__1	EQU	$B5
__local_mix__2	EQU	$B6
__local_mix__3	EQU	$B7

; @zp-link-meta-begin
; def f15 params=__zpabi_f15__p16_0,__zpabi_f15__p16_1,__zpabi_f15__pp17_0,__zpabi_f15__pp17_1 locals=__local_f15__0,__local_f15__w22,__local_f15__1,__local_f15__2,__local_f15__3,__local_f15__4,__local_f15__5,__local_f15__6,__local_f15__7,__local_f15__8,__local_f15__9,__local_f15__10,__local_f15__11,__local_f15__12,__local_f15__13,__local_f15__14,__local_f15__l18,__local_f15__15,__local_f15__16,__local_f15__17,__local_f15__l20 indirect=false in_cycle=false param_regs=A,X,,
; def f23 params=__zpabi_f23__p24_0,__zpabi_f23__p24_1,__zpabi_f23__p25_0,__zpabi_f23__p25_1,__zpabi_f23__p25_2,__zpabi_f23__p25_3,__zpabi_f23__p25_4,__zpabi_f23__p25_5,__zpabi_f23__p25_6,__zpabi_f23__p25_7 locals=__local_f23__0,__local_f23__1,__local_f23__l26,__local_f23__p24_1,__local_f23__p24_0 indirect=false in_cycle=false param_regs=A,X,,,,,,,,
; def f8 params=__zpabi_f8__p9_0,__zpabi_f8__p9_1,__zpabi_f8__p9_2,__zpabi_f8__p9_3,__zpabi_f8__pp10_0,__zpabi_f8__pp10_1 locals=__local_f8__0,__local_f8__w13,__local_f8__l11_1,__local_f8__l11_0,__local_f8__1,__local_f8__2,__local_f8__3,__local_f8__4,__local_f8__5,__local_f8__6,__local_f8__7,__local_f8__8,__local_f8__9,__local_f8__10,__local_f8__11,__local_f8__12,__local_f8__13,__local_f8__14,__local_f8__15 indirect=false in_cycle=false
; def fin0 params= locals=__local_fin0__0,__local_fin0__1 indirect=false in_cycle=false
; def fin1 params= locals=__local_fin1__0,__local_fin1__1 indirect=false in_cycle=false
; def fin2 params= locals=__local_fin2__0,__local_fin2__1,__local_fin2__2,__local_fin2__3 indirect=false in_cycle=false
; def main params= locals=__local_main__0,__local_main__1,__local_main__w35,__local_main__l32_3,__local_main__l32_0,__local_main__2,__local_main__3,__local_main__4,__local_main__5,__local_main__6,__local_main__7,__local_main__8,__local_main__9,__local_main__10,__local_main__11,__local_main__12,__local_main__l33 indirect=false in_cycle=false
; def mix params=__zpabi_mix__v_0,__zpabi_mix__v_1,__zpabi_mix__v_2,__zpabi_mix__v_3 locals=__local_mix__0,__local_mix__1,__local_mix__2,__local_mix__3 indirect=false in_cycle=false
; call f15 -> f8
; call f15 -> mix
; call f23 -> mix
; call f8 -> mix
; call fin0 -> mix
; call fin1 -> mix
; call fin2 -> mix
; call main -> fin0
; call main -> fin1
; call main -> fin2
; call main -> mix
; item mix section=.text local=false zero=false
; item fin0 section=.text local=false zero=false
; item fin0@run4 section=.text local=true zero=false
; item fin0@run2 section=.text local=true zero=false
; item fin0@run0 section=.text local=true zero=false
; item fin1 section=.text local=false zero=false
; item fin1@run3 section=.text local=true zero=false
; item fin2 section=.text local=false zero=false
; item f8 section=.text local=false zero=false
; item f15 section=.text local=false zero=false
; item f23 section=.text local=false zero=false
; item main section=.text local=false zero=false
; item main@run1 section=.text local=true zero=false
; item acc section=.bss local=false zero=true
; item g1 section=.data local=false zero=false
; item ga5 section=.data local=false zero=false
; item gs6 section=.data local=false zero=false
; item @3.s12 section=.data local=true zero=false
; item __zp_init section=.text local=false zero=false
; zp z4 size=2 at=auto def=true init=$01,$00
; zp g3 size=1 at=auto def=true init=$FF weight=30
; zp @16.s29 size=1 at=auto def=true init= weight=17
; zp g2 size=1 at=auto def=true init=$FF weight=11
; zp gp7 size=2 at=auto def=true init=<g2,>g2 weight=17
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

   LDA   g1
   STA   __zpabi_mix__v_0
   LDA   g1+1
   STA   __zpabi_mix__v_1
   LDA   g1+2
   STA   __zpabi_mix__v_2
   LDA   g1+3
   STA   __zpabi_mix__v_3
   LDA   g1+4
   LDA   g1+5
   LDA   g1+6
   LDA   g1+7
   JSR   mix
   LDA   g2
   STA   __zpabi_mix__v_0
   LDA   #$00
   STA   __zpabi_mix__v_1
   JSR   fin0@run4
   LDA   g3
   BMI   .sx_neg@0
   LDA   #$00
   BEQ   .sx_done@1
.sx_neg@0:
   LDA   #$FF
.sx_done@1:
   STA   __zpabi_mix__v_1
   JSR   fin0@run2
   LDA   g3
   STA   __zpabi_mix__v_0
   JSR   mix
   LDA   z4
   STA   __zpabi_mix__v_0
   LDA   z4+1
   STA   __zpabi_mix__v_1
   LDA   #$00
   STA   __zpabi_mix__v_2
   STA   __zpabi_mix__v_3
   JMP   mix

fin0@run4:
   SUBROUTINE

   STA   __zpabi_mix__v_2
   JMP   fin0@run0

fin0@run2:
   SUBROUTINE

   STA   __zpabi_mix__v_2
   STA   __zpabi_mix__v_3
   RTS

fin0@run0:
   SUBROUTINE

   STA   __zpabi_mix__v_3
   JMP   mix

fin1:
   SUBROUTINE

   LDA   ga5+1
   BMI   .sx_neg@2
   LDA   #$00
   BEQ   .sx_done@3
.sx_neg@2:
   LDA   #$FF
.sx_done@3:
   JSR   fin0@run2
   LDA   ga5
   STA   __zpabi_mix__v_0
   LDA   ga5+1
   JSR   fin1@run3
   LDA   ga5+3
   BMI   .sx_neg@4
   LDA   #$00
   BEQ   .sx_done@5
.sx_neg@4:
   LDA   #$FF
.sx_done@5:
   JSR   fin0@run2
   LDA   ga5+2
   STA   __zpabi_mix__v_0
   LDA   ga5+3
   JSR   fin1@run3
   LDA   ga5+5
   BMI   .sx_neg@6
   LDA   #$00
   BEQ   .sx_done@7
.sx_neg@6:
   LDA   #$FF
.sx_done@7:
   JSR   fin0@run2
   LDA   ga5+4
   STA   __zpabi_mix__v_0
   LDA   ga5+5
   JSR   fin1@run3
   LDA   ga5+7
   BMI   .sx_neg@8
   LDA   #$00
   BEQ   .sx_done@9
.sx_neg@8:
   LDA   #$FF
.sx_done@9:
   JSR   fin0@run2
   LDA   ga5+6
   STA   __zpabi_mix__v_0
   LDA   ga5+7
   STA   __zpabi_mix__v_1
   JMP   mix

fin1@run3:
   SUBROUTINE

   STA   __zpabi_mix__v_1
   JMP   mix

fin2:
   SUBROUTINE

   LDA   gs6+1
   STA   __zpabi_mix__v_0
   LDA   gs6+2
   STA   __zpabi_mix__v_1
   LDA   gs6+3
   STA   __zpabi_mix__v_2
   LDA   gs6+4
   JSR   fin0@run0
   LDA   gs6+10
   BMI   .sx_neg@10
   LDA   #$00
   BEQ   .sx_done@11
.sx_neg@10:
   LDA   #$FF
.sx_done@11:
   JSR   fin0@run2
   LDA   gs6+9
   STA   __zpabi_mix__v_0
   LDA   gs6+10
   STA   __zpabi_mix__v_1
   JMP   mix

f8:
   SUBROUTINE

   LDA   z4
   ASL
   LDA   z4+1
   ROL
   STA   z4
   LDA   #$00
   ROL
   STA   z4+1
   LDA   z4
   AND   #$07
   BEQ   .case@1
   CMP   #$01
   BEQ   .case@4
   CMP   #$05
   BEQ   .case@5
.xjump@0:
   LDA   #$00
   STA   __local_f8__l11_0
   STA   __local_f8__l11_1
   JMP   .switch@0_break
.case@1:
   LDA   #$00
   STA   __local_f8__w13
.loop@2_start:
   LDA   g3
   BMI   .sx_neg@12
   LDA   #$00
   BEQ   .sx_done@13
.sx_neg@12:
   LDA   #$FF
.sx_done@13:
   STA   __zpabi_mix__v_1
   STA   __zpabi_mix__v_2
   STA   __zpabi_mix__v_3
   LDA   g3
   STA   __zpabi_mix__v_0
   JSR   mix
   INC   __local_f8__w13
   LDA   __local_f8__w13
   CMP   #$02
   BCC   .loop@2_start
   BCS   .xjump@0
.case@4:
.case@5:
   LDA   @3.s12
   BMI   .sx_neg@14
   LDA   #$00
   BEQ   .sx_done@15
.sx_neg@14:
   LDA   #$FF
.sx_done@15:
   STA   __local_f8__0
   LDA   @3.s12
   EOR   #$FF
   STA   gs6+9
   LDA   __local_f8__0
   EOR   #$FF
   STA   gs6+10
   LDA   #$FF
   STA   __local_f8__l11_0
   STA   __local_f8__l11_1
.switch@0_break:
   LDA   z4
   SEC
   SBC   #$03
   STA   __local_f8__w13
   LDA   z4+1
   SBC   #$00
   ORA   __local_f8__w13
   BNE   .lb_skip@0
   JMP   .if_end@1
.lb_skip@0:
   LDA   __local_f8__l11_0
   ORA   __local_f8__l11_0
   STA   __local_f8__14
   LDA   __local_f8__l11_1
   ORA   __local_f8__l11_1
   STA   __local_f8__13
   LDA   g1
   STA   __local_f8__12
   LDA   g1+1
   STA   __local_f8__11
   LDA   g1+2
   STA   __local_f8__10
   LDA   g1+3
   STA   __local_f8__9
   LDA   g1+4
   STA   __local_f8__8
   LDA   g1+5
   STA   __local_f8__7
   LDA   g1+6
   STA   __local_f8__6
   LDA   g1+7
   STA   __local_f8__5
   LDA   gs6+9
   STA   __local_f8__4
   LDA   gs6+10
   STA   __local_f8__3
   BMI   .sx_neg@16
   LDA   #$00
   BEQ   .sx_done@17
.sx_neg@16:
   LDA   #$FF
.sx_done@17:
   STA   __local_f8__2
   STA   __local_f8__1
   STA   __local_f8__l11_0
   STA   __local_f8__l11_1
   STA   __local_f8__w13
   STA   __local_f8__0
   LDA   __local_f8__12
   AND   __local_f8__4
   STA   __local_f8__4
   LDA   __local_f8__11
   AND   __local_f8__3
   STA   __local_f8__3
   LDA   __local_f8__10
   AND   __local_f8__2
   STA   __local_f8__2
   LDA   __local_f8__9
   AND   __local_f8__1
   STA   __local_f8__1
   LDA   __local_f8__8
   AND   __local_f8__l11_0
   STA   __local_f8__l11_0
   LDA   __local_f8__7
   AND   __local_f8__l11_1
   STA   __local_f8__l11_1
   LDA   __local_f8__6
   AND   __local_f8__w13
   STA   __local_f8__w13
   LDA   __local_f8__5
   AND   __local_f8__0
   STA   __local_f8__0
   LDA   __local_f8__14
   SEC
   SBC   __local_f8__4
   LDA   __local_f8__13
   SBC   __local_f8__3
   LDA   #$00
   SBC   __local_f8__2
   LDA   #$00
   SBC   __local_f8__1
   LDA   #$00
   SBC   __local_f8__l11_0
   LDA   #$00
   SBC   __local_f8__l11_1
   LDA   #$00
   SBC   __local_f8__w13
   LDA   #$00
   SBC   __local_f8__0
   BVC   .cmp_novf@18
   EOR   #$80
.cmp_novf@18:
   ASL
   LDA   #$00
   ROL
   STA   gs6+9
   LDA   #$00
   STA   gs6+10
.if_end@1:
   LDY   #$00
   LDA   (__zpabi_f8__pp10_0),Y
   STA   __local_f8__0
   LSR
   LSR
   LSR
   LSR
   TAX
   LDA   __local_f8__0
   ASL
   ASL
   ASL
   ASL
   RTS

f15:
   SUBROUTINE

   LDA   #$FF
   STA   __local_f15__l18
   STA   __local_f15__15
   STA   __local_f15__16
   LDA   #$7F
   STA   __local_f15__17
   LDA   #$00
   STA   __local_f15__l20
   LDA   g1
   STA   __local_f15__14
   LDA   g1+1
   STA   __local_f15__13
   LDA   g1+2
   STA   __local_f15__12
   LDA   g1+3
   STA   __local_f15__11
   LDA   g1+4
   STA   __local_f15__10
   LDA   g1+5
   STA   __local_f15__9
   LDA   g1+6
   STA   __local_f15__8
   LDA   g1+7
   STA   __local_f15__7
   LDA   __local_f15__14
   CLC
   ADC   #$01
   STA   __local_f15__6
   LDA   __local_f15__13
   ADC   #$00
   STA   __local_f15__5
   LDA   __local_f15__12
   ADC   #$00
   STA   __local_f15__4
   LDA   __local_f15__11
   ADC   #$00
   STA   __local_f15__3
   LDA   __local_f15__10
   ADC   #$00
   STA   __local_f15__2
   LDA   __local_f15__9
   ADC   #$00
   STA   __local_f15__1
   LDA   __local_f15__8
   ADC   #$00
   STA   __local_f15__w22
   LDA   __local_f15__7
   ADC   #$00
   LDX   __local_f15__6
   STX   g1
   LDX   __local_f15__5
   STX   g1+1
   LDX   __local_f15__4
   STX   g1+2
   LDX   __local_f15__3
   STX   g1+3
   LDX   __local_f15__2
   STX   g1+4
   LDX   __local_f15__1
   STX   g1+5
   LDX   __local_f15__w22
   STX   g1+6
   STA   g1+7
   LDA   __local_f15__14
   ORA   __local_f15__13
   ORA   __local_f15__12
   ORA   __local_f15__11
   ORA   __local_f15__10
   ORA   __local_f15__9
   ORA   __local_f15__8
   ORA   __local_f15__7
   BEQ   .if_end@2
   LDA   g3
   BMI   .sx_neg@19
   LDA   #$00
   BEQ   .sx_done@20
.sx_neg@19:
   LDA   #$FF
.sx_done@20:
   STA   __zpabi_f8__p9_1
   STA   __zpabi_f8__p9_2
   STA   __zpabi_f8__p9_3
   LDA   g3
   STA   __zpabi_f8__p9_0
   LDA   #<__local_f15__l20
   STA   __zpabi_f8__pp10_0
   LDA   #>__local_f15__l20
   STA   __zpabi_f8__pp10_1
   JSR   f8
   STA   __zpabi_mix__v_0
   STX   __zpabi_mix__v_1
   LDA   #$00
   STA   __zpabi_mix__v_2
   JSR   fin0@run0
   LDA   #$03
   STA   z4
   LDA   #$00
   STA   z4+1
   LDA   __local_f15__l18
   STA   HARGS
   LDA   __local_f15__15
   STA   HARGS+1
   LDA   __local_f15__16
   STA   HARGS+2
   LDA   __local_f15__17
   STA   HARGS+3
   LDA   #$06
   STA   HARGS+4
   JSR   lsr32
   LDA   HARGS+5
   STA   __local_f15__l18
   LDA   HARGS+6
   STA   __local_f15__15
   LDA   HARGS+7
   STA   __local_f15__16
   LDA   HARGS+8
   STA   __local_f15__17
.if_end@2:
   LDA   #$00
   STA   __local_f15__w22
.loop@12_start:
   LDA   #$FF
   STA   g3
   LDY   #$00
   STA   (__zpabi_f15__pp17_0),Y
   INC   __local_f15__w22
   LDA   __local_f15__w22
   CMP   #$03
   BCC   .loop@12_start
   LDA   gs6+1
   STA   HARGS+16
   LDA   gs6+2
   STA   HARGS+17
   LDA   gs6+3
   STA   HARGS+18
   LDA   gs6+4
   STA   HARGS+19
   LDA   gs6+5
   STA   HARGS+20
   LDA   gs6+6
   STA   HARGS+21
   LDA   gs6+7
   STA   HARGS+22
   LDA   gs6+8
   STA   HARGS+23
   RTS

f23:
   SUBROUTINE

   STA   __local_f23__p24_0
   STX   __local_f23__p24_1
   LDA   #$01
   STA   __local_f23__l26
   LDA   gs6+9
   ORA   gs6+10
   BEQ   .and_false@3
   LDA   __local_f23__l26
   BEQ   .and_false@3
   LDA   #$01
   LDX   #$00
   BEQ   .and_end@4
.and_false@3:
   LDA   #$00
   LDX   #$00
.and_end@4:
   CLC
   ADC   g2
   BCC   .carry_inc@0
   INX
.carry_inc@0:
   AND   #$07
   BEQ   .case@14
   CMP   #$01
   BEQ   .case@15
   CMP   #$06
   BEQ   .switch@13_break
   BNE   .default@17
.case@14:
   LDA   #$FF
   EOR   __local_f23__p24_0
   STA   __local_f23__0
   LDA   #$00
   EOR   __local_f23__p24_1
   TAX
   LDA   __local_f23__0
   RTS
.case@15:
   LDA   __local_f23__p24_0
   SEC
   SBC   z4
   LDA   __local_f23__p24_1
   SBC   z4+1
   LDA   #$00
   ROL
   STA   __local_f23__0
   LDA   #$00
   BPL   .sx_done@22
   LDA   #$FF
.sx_done@22:
   JSR   fin0@run2
   LDA   __local_f23__0
   STA   __zpabi_mix__v_0
   LDA   #$00
   STA   __zpabi_mix__v_1
   JSR   mix
   JMP   .switch@13_break
.default@17:
   LDY   #$00
   LDA   (gp7),Y
   ORA   #$05
   STA   (gp7),Y
   LDX   #$00
   RTS
.switch@13_break:
   LDA   z4
   ORA   z4+1
   BEQ   .cond_else@5
   LDA   #$FF
   STA   __local_f23__1
   LDA   #$7F
   STA   __local_f23__0
   JMP   .cond_end@6
.cond_else@5:
   LDA   z4
   STA   __local_f23__1
   LDA   z4+1
   STA   __local_f23__0
.cond_end@6:
   LDA   #$FF
   ORA   __local_f23__1
   STA   __zpabi_mix__v_0
   LDA   #$7F
   ORA   __local_f23__0
   STA   __zpabi_mix__v_1
   LDA   #$00
   JSR   fin0@run4
   LDX   __local_f23__p24_1
   LDA   __local_f23__p24_0
   RTS

main:
   SUBROUTINE

   LDA   #$AB
   STA   __local_main__l33
   LDA   g3
   EOR   #$03
   STA   __local_main__4
   STA   g3
   BMI   .sx_neg@23
   LDA   #$00
   BEQ   .sx_done@24
.sx_neg@23:
   LDA   #$FF
.sx_done@24:
   LDX   __local_main__4
   STX   g1
   STA   g1+1
   STA   g1+2
   STA   g1+3
   STA   g1+4
   STA   g1+5
   STA   g1+6
   STA   g1+7
   LDA   #$00
   STA   __zpabi_mix__v_0
   STA   __zpabi_mix__v_1
   JSR   fin0@run4
   LDA   __local_main__l33
   STA   __local_main__12
   BMI   .sx_neg@25
   LDA   #$00
   BEQ   .sx_done@26
.sx_neg@25:
   LDA   #$FF
.sx_done@26:
   STA   __local_main__5
   LDA   g1
   LDX   g1+1
   STX   __local_main__4
   LDX   g1+2
   STX   __local_main__3
   LDX   g1+3
   STX   __local_main__2
   LDX   g1+4
   STX   __local_main__l32_0
   LDX   g1+5
   STX   __local_main__l32_3
   LDX   g1+6
   STX   __local_main__w35
   LDX   g1+7
   STX   __local_main__1
   CLC
   ADC   #$80
   STA   __local_main__0
   BCC   .carry_inc@1
   INC   __local_main__4
   BNE   .carry_inc@1
   INC   __local_main__3
   BNE   .carry_inc@1
   INC   __local_main__2
   BNE   .carry_inc@1
   INC   __local_main__l32_0
   BNE   .carry_inc@1
   INC   __local_main__l32_3
   BNE   .carry_inc@1
   INC   __local_main__w35
   BNE   .carry_inc@1
   INC   __local_main__1
.carry_inc@1:
   LDA   __local_main__5
   CMP   __local_main__1
   BNE   .jcmp_skip@27
   CMP   __local_main__w35
   BNE   .jcmp_skip@27
   CMP   __local_main__l32_3
   BNE   .jcmp_skip@27
   CMP   __local_main__l32_0
   BNE   .jcmp_skip@27
   CMP   __local_main__2
   BNE   .jcmp_skip@27
   CMP   __local_main__3
   BNE   .jcmp_skip@27
   CMP   __local_main__4
   BNE   .jcmp_skip@27
   LDA   __local_main__12
   CMP   __local_main__0
   BNE   .lb_skip@4
   JMP   .if_end@7
.lb_skip@4:
.jcmp_skip@27:
   LDA   __local_main__l33
   STA   __local_main__3
   LDA   gs6+1
   STA   __local_main__9
   LDA   gs6+2
   STA   __local_main__8
   LDA   gs6+3
   STA   __local_main__2
   LDA   gs6+4
   STA   __local_main__l32_0
   LDA   gs6+5
   STA   __local_main__l32_3
   LDA   gs6+6
   STA   __local_main__7
   LDA   gs6+7
   STA   __local_main__6
   LDA   gs6+8
   STA   __local_main__5
   LDA   z4
   SEC
   SBC   __local_main__9
   CLC
   ADC   __local_main__3
   CLC
   ADC   #$D2
   AND   #$07
   CMP   #$01
   BEQ   .case@20
   CMP   #$03
   BNE   .lb_skip@3
   JMP   .switch@19_break
.lb_skip@3:
   JMP   .default@26
.case@20:
   LDA   __local_main__9
   CLC
   ADC   #$01
   STA   __local_main__1
   LDA   __local_main__8
   ADC   #$00
   BCC   .carry_inc@4
   INC   __local_main__2
   BNE   .carry_inc@4
   INC   __local_main__l32_0
   BNE   .carry_inc@4
   INC   __local_main__l32_3
   BNE   .carry_inc@4
   INC   __local_main__7
   BNE   .carry_inc@4
   INC   __local_main__6
   BNE   .carry_inc@4
   INC   __local_main__5
.carry_inc@4:
   STA   __local_main__3
   LDA   __local_main__1
   STA   __local_main__4
   LDA   __local_main__7
   STA   __local_main__w35
   LDA   __local_main__6
   STA   __local_main__1
   LDA   __local_main__5
   AND   #$01
   LDX   __local_main__4
   STX   gs6+1
   LDX   __local_main__3
   STX   gs6+2
   LDX   __local_main__2
   STX   gs6+3
   LDX   __local_main__l32_0
   STX   gs6+4
   LDX   __local_main__l32_3
   STX   gs6+5
   LDX   __local_main__w35
   STX   gs6+6
   LDX   __local_main__1
   JSR   main@run1
   LDA   __local_main__9
   AND   #$07
   CMP   #$01
   BNE   .lb_skip@2
   JMP   .switch@19_break
.lb_skip@2:
   CMP   #$06
   BEQ   .lb_skip@1
   JMP   .default@24
.lb_skip@1:
   LDA   gs6+1
   LDX   gs6+2
   STX   __local_main__l32_0
   LDX   gs6+3
   STX   __local_main__l32_3
   LDX   gs6+4
   STX   __local_main__w35
   LDX   gs6+5
   STX   __local_main__4
   LDX   gs6+6
   STX   __local_main__3
   LDX   gs6+8
   STX   __local_main__5
   SEC
   SBC   #$01
   STA   __local_main__0
   LDA   __local_main__l32_0
   SBC   #$00
   STA   __local_main__l32_0
   LDA   __local_main__l32_3
   SBC   #$00
   STA   __local_main__l32_3
   LDA   __local_main__w35
   SBC   #$00
   STA   __local_main__w35
   LDA   __local_main__4
   SBC   #$00
   STA   __local_main__4
   LDA   __local_main__3
   SBC   #$00
   STA   __local_main__3
   LDA   __local_main__1
   SBC   #$00
   STA   __local_main__1
   BCS   .carry_inc@5
   DEC   __local_main__5
.carry_inc@5:
   LDA   __local_main__0
   STA   __local_main__2
   LDA   __local_main__5
   AND   #$01
   LDX   __local_main__4
   STX   gs6+5
   LDX   __local_main__3
   STX   gs6+6
   LDX   __local_main__1
   JSR   main@run1
   LDA   __local_main__2
   STA   gs6+1
   LDA   __local_main__l32_0
   STA   gs6+2
   LDA   __local_main__l32_3
   STA   gs6+3
   LDA   __local_main__w35
   STA   gs6+4
   LDA   #$00
   BPL   .sx_done@31
   LDA   #$FF
.sx_done@31:
   STA   __local_main__1
   STA   __local_main__0
   LDA   g2
   EOR   __local_main__2
   STA   __zpabi_mix__v_0
   LDA   #$00
   EOR   __local_main__l32_0
   STA   __zpabi_mix__v_1
   LDA   __local_main__1
   EOR   __local_main__l32_3
   STA   __zpabi_mix__v_2
   LDA   __local_main__0
   EOR   __local_main__w35
   JSR   fin0@run0
.default@24:
   LDA   ga5+6
   SEC
   SBC   #$5A
   STA   ga5+6
   BCS   .switch@19_break
   DEC   ga5+7
   JMP   .switch@19_break
.default@26:
   LDY   #$00
   LDA   (gp7),Y
   ASL
   STA   __local_main__0
   LDA   #$00
   ROL
   ASL   __local_main__0
   ROL
   STA   __local_main__1
   BMI   .sx_neg@32
   LDA   #$00
   BEQ   .sx_done@33
.sx_neg@32:
   LDA   #$FF
.sx_done@33:
   JSR   fin0@run2
   LDA   __local_main__0
   STA   __zpabi_mix__v_0
   LDA   __local_main__1
   JSR   fin1@run3
.switch@19_break:
   LDA   #$00
   STA   __local_main__l32_0
   LDX   #$80
   LDY   #$00
   STA   __local_main__l32_3
   STA   __local_main__w35
.loop@27_start:
   SEC
   SBC   #$01
   STA   __local_main__0
   BCS   .carry_inc@9
   CPX   #$01
   DEX
   BCS   .carry_inc@9
   CPY   #$01
   DEY
   BCS   .carry_inc@9
   DEC   __local_main__l32_3
.carry_inc@9:
   LDA   __local_main__l32_0
   CLC
   ADC   z4
   STA   g2
   INC   __local_main__w35
   LDA   __local_main__w35
   CMP   #$04
   BCC   .main@asm_ssa_split@0
.if_end@7:
   JSR   fin0
   JSR   fin1
   JSR   fin2
   LDA   acc+1
   AND   #$7F
   TAX
   LDA   acc
   RTS
.main@asm_ssa_split@0:
   LDA   __local_main__0
   STA   __local_main__l32_0
   JMP   .loop@27_start

main@run1:
   SUBROUTINE

   STX   gs6+7
   EOR   #$01
   SEC
   SBC   #$01
   STA   gs6+8
   RTS

acc:
   DS.B  2

g1:
   DC.L  $00000003
   DC.L  $00000000

ga5:
   DC.W  $00FF
   DC.W  $7FFF
   DS.B  2
   DC.W  $0080

gs6:
   DS.B  1
   DC.L  $00008000
   DC.L  $00000000
   DS.B  2

@3.s12:
   DC.B  $01

__zp_init:
   SUBROUTINE

   LDA   #$00
   STA   z4+1
   LDA   #$FF
   STA   g2
   STA   g3
   LDA   #$01
   STA   z4
   LDA   #<g2
   STA   gp7
   LDA   #>g2
   STA   gp7+1
   RTS
