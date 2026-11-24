SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
g2	EQU	$26
g1	EQU	$27
gp5	EQU	$29
__zpabi_f24__p25_0	EQU	$80
__zpabi_f24__p25_1	EQU	$81
__zpabi_f24__p25_2	EQU	$82
__zpabi_f24__p25_3	EQU	$83
__zpabi_f24__pp26_0	EQU	$84
__zpabi_f24__pp26_1	EQU	$85
__zpabi_f16__p17_0	EQU	$86
__zpabi_f16__p17_1	EQU	$87
__zpabi_f16__p17_2	EQU	$88
__zpabi_f16__p17_3	EQU	$89
__zpabi_f16__p17_4	EQU	$8A
__zpabi_f16__p17_5	EQU	$8B
__zpabi_f16__p18_0	EQU	$8C
__zpabi_f16__p18_1	EQU	$8D
__zpabi_f16__p18_2	EQU	$8E
__zpabi_f16__p18_3	EQU	$8F
__zpabi_f16__p19	EQU	$90
__zpabi_f16__pp20_0	EQU	$91
__zpabi_f16__pp20_1	EQU	$92
__zpabi_f7__p8_0	EQU	$93
__zpabi_f7__p8_1	EQU	$94
__zpabi_f7__p8_2	EQU	$95
__zpabi_f7__p8_3	EQU	$96
__zpabi_f7__p8_4	EQU	$97
__zpabi_f7__p8_5	EQU	$98
__zpabi_f7__p9_0	EQU	$99
__zpabi_f7__p9_1	EQU	$9A
__zpabi_f7__p9_2	EQU	$9B
__zpabi_f7__p9_3	EQU	$9C
__zpabi_f7__pp10_0	EQU	$9D
__zpabi_f7__pp10_1	EQU	$9E
__zpabi_mix__v_0	EQU	$9F
__zpabi_mix__v_1	EQU	$A0
__zpabi_mix__v_2	EQU	$A1
__zpabi_mix__v_3	EQU	$A2
__local_main__0	EQU	$A3
__local_main__1	EQU	$A4
__local_main__2	EQU	$A5
__local_main__3	EQU	$A6
__local_main__5	EQU	$A8
__local_main__8	EQU	$AB
__local_main__9	EQU	$AC
__local_main__l31	EQU	$AD
__local_main__l29	EQU	$B1
__local_main__l32	EQU	$B2
__local_f24__0	EQU	$B4
__local_f24__4	EQU	$B8
__local_f24__5	EQU	$B9
__local_f24__6	EQU	$BA
__local_f24__7	EQU	$BB
__local_f24__8	EQU	$BC
__local_f24__9	EQU	$BD
__local_f24__10	EQU	$BE
__local_f24__11	EQU	$BF
__local_f24__12	EQU	$C0
__local_f24__13	EQU	$C1
__local_f24__l27	EQU	$C2
__local_f16__0	EQU	$C3
__local_f16__1	EQU	$C4
__local_f16__2	EQU	$C5
__local_f16__3	EQU	$C6
__local_f16__4	EQU	$C7
__local_f16__5	EQU	$C8
__local_f16__6	EQU	$C9
__local_f16__7	EQU	$CA
__local_f16__l22	EQU	$CD
__local_f7__w13	EQU	$D3
__local_f7__p8_3	EQU	$D6
__local_f7__p8_2	EQU	$D7
__local_f7__p8_1	EQU	$D8
__local_f7__p8_0	EQU	$D9
__local_f7__i15	EQU	$DA
__local_f7__i14	EQU	$DB
__local_mix__0	EQU	$E1
__local_mix__1	EQU	$E2
__local_mix__2	EQU	$E3
__local_mix__3	EQU	$E4

; @zp-link-meta-begin
; def f16 params=__zpabi_f16__p17_0,__zpabi_f16__p17_1,__zpabi_f16__p17_2,__zpabi_f16__p17_3,__zpabi_f16__p17_4,__zpabi_f16__p17_5,__zpabi_f16__p18_0,__zpabi_f16__p18_1,__zpabi_f16__p18_2,__zpabi_f16__p18_3,__zpabi_f16__p19,__zpabi_f16__pp20_0,__zpabi_f16__pp20_1 locals=__local_f16__0,__local_f16__1,__local_f16__2,__local_f16__3,__local_f16__4,__local_f16__5,__local_f16__6,__local_f16__7,__local_f16__8,__local_f16__9,__local_f16__l22,__local_f16__11,__local_f16__12,__local_f16__13 indirect=false in_cycle=false
; def f24 params=__zpabi_f24__p25_0,__zpabi_f24__p25_1,__zpabi_f24__p25_2,__zpabi_f24__p25_3,__zpabi_f24__pp26_0,__zpabi_f24__pp26_1 locals=__local_f24__0,__local_f24__1,__local_f24__2,__local_f24__3,__local_f24__4,__local_f24__5,__local_f24__6,__local_f24__7,__local_f24__8,__local_f24__9,__local_f24__10,__local_f24__11,__local_f24__12,__local_f24__13,__local_f24__l27 indirect=false in_cycle=false
; def f7 params=__zpabi_f7__p8_0,__zpabi_f7__p8_1,__zpabi_f7__p8_2,__zpabi_f7__p8_3,__zpabi_f7__p8_4,__zpabi_f7__p8_5,__zpabi_f7__p9_0,__zpabi_f7__p9_1,__zpabi_f7__p9_2,__zpabi_f7__p9_3,__zpabi_f7__pp10_0,__zpabi_f7__pp10_1 locals=__local_f7__0,__local_f7__1,__local_f7__w13,__local_f7__p8_5,__local_f7__p8_4,__local_f7__p8_3,__local_f7__p8_2,__local_f7__p8_1,__local_f7__p8_0,__local_f7__i15,__local_f7__i14,__local_f7__2,__local_f7__3,__local_f7__4,__local_f7__5,__local_f7__6 indirect=false in_cycle=false
; def fin0 params= locals=__local_fin0__0,__local_fin0__1,__local_fin0__2,__local_fin0__3 indirect=false in_cycle=false
; def fin1 params= locals=__local_fin1__0,__local_fin1__1,__local_fin1__2,__local_fin1__3 indirect=false in_cycle=false
; def main params= locals=__local_main__0,__local_main__1,__local_main__2,__local_main__3,__local_main__4,__local_main__5,__local_main__6,__local_main__7,__local_main__8,__local_main__9,__local_main__l31,__local_main__11,__local_main__12,__local_main__13,__local_main__l29,__local_main__l32,__local_main__16 indirect=false in_cycle=false
; def mix params=__zpabi_mix__v_0,__zpabi_mix__v_1,__zpabi_mix__v_2,__zpabi_mix__v_3 locals=__local_mix__0,__local_mix__1,__local_mix__2,__local_mix__3 indirect=false in_cycle=false
; call f16 -> f7
; call f16 -> mix
; call f24 -> f16
; call f24 -> mix
; call f7 -> mix
; call fin0 -> mix
; call fin1 -> mix
; call main -> f16
; call main -> f24
; call main -> f7
; call main -> fin0
; call main -> fin1
; call main -> mix
; item mix section=.text local=false zero=false
; item fin0 section=.text local=false zero=false
; item fin0@run3 section=.text local=true zero=false
; item fin1 section=.text local=false zero=false
; item fin1@run8 section=.text local=true zero=false
; item fin1@run7 section=.text local=true zero=false
; item fin1@run2 section=.text local=true zero=false
; item f7 section=.text local=false zero=false
; item f16 section=.text local=false zero=false
; item f16@run5 section=.text local=true zero=false
; item f24 section=.text local=false zero=false
; item f24@run6 section=.text local=true zero=false
; item f24@run1 section=.text local=true zero=false
; item main section=.text local=false zero=false
; item main@run4 section=.text local=true zero=false
; item main@run0 section=.text local=true zero=false
; item acc section=.bss local=false zero=true
; item ga3 section=.data local=false zero=false
; item gs4 section=.data local=false zero=false
; item gp6 section=.data local=false zero=false
; item __zp_init section=.text local=false zero=false
; zp g2 size=1 at=auto def=true init=$FF weight=194
; zp g1 size=2 at=auto def=true init=$00,$00 weight=13
; zp gp5 size=2 at=auto def=true init=<g1,>g1 weight=10
; runtime hargs_size=32 hargs_need=22
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
   LDA   #$00
   STA   __zpabi_mix__v_2
   JSR   fin0@run3
   LDA   g2
   BMI   .sx_neg@0
   LDA   #$00
   BEQ   .sx_done@1
.sx_neg@0:
   LDA   #$FF
.sx_done@1:
   STA   __zpabi_mix__v_1
   STA   __zpabi_mix__v_2
   STA   __zpabi_mix__v_3
   LDA   g2
   STA   __zpabi_mix__v_0
   JSR   mix
   LDA   ga3
   STA   __zpabi_mix__v_0
   LDA   ga3+1
   STA   __zpabi_mix__v_1
   LDA   ga3+2
   STA   __zpabi_mix__v_2
   LDA   ga3+3
   JSR   fin0@run3
   LDA   ga3+4
   STA   __zpabi_mix__v_0
   LDA   ga3+5
   STA   __zpabi_mix__v_1
   LDA   ga3+6
   STA   __zpabi_mix__v_2
   LDA   ga3+7
   STA   __zpabi_mix__v_3
   JMP   mix

fin0@run3:
   SUBROUTINE

   STA   __zpabi_mix__v_3
   JMP   mix

fin1:
   SUBROUTINE

   LDA   ga3+8
   STA   __zpabi_mix__v_0
   LDA   ga3+9
   STA   __zpabi_mix__v_1
   LDA   ga3+10
   STA   __zpabi_mix__v_2
   LDA   ga3+11
   JSR   fin0@run3
   JSR   fin1@run7
   STA   __zpabi_mix__v_1
   JSR   fin1@run8
   JSR   fin0@run3
   LDA   gs4+1
   JSR   fin1@run2
   LDA   gs4+2
   STA   __zpabi_mix__v_0
   LDA   gs4+3
   STA   __zpabi_mix__v_1
   LDA   gs4+4
   STA   __zpabi_mix__v_2
   LDA   gs4+5
   STA   __zpabi_mix__v_3
   JMP   mix

fin1@run8:
   SUBROUTINE

   LDA   ga3+14
   STA   __zpabi_mix__v_2
   LDA   ga3+15
   RTS

fin1@run7:
   SUBROUTINE

   LDA   ga3+12
   STA   __zpabi_mix__v_0
   LDA   ga3+13
   RTS

fin1@run2:
   SUBROUTINE

   STA   __zpabi_mix__v_0
   LDA   #$00
   STA   __zpabi_mix__v_1
   STA   __zpabi_mix__v_2
   STA   __zpabi_mix__v_3
   JMP   mix

f7:
   SUBROUTINE

   JSR   fin1@run7
   STA   __local_f7__w13
   JSR   fin1@run8
   STA   __zpabi_mix__v_3
   LDA   __zpabi_mix__v_0
   SEC
   SBC   #$01
   STA   ga3+12
   LDA   __local_f7__w13
   SBC   #$00
   STA   ga3+13
   LDA   __zpabi_mix__v_2
   SBC   #$00
   STA   ga3+14
   LDA   __zpabi_mix__v_3
   SBC   #$00
   STA   ga3+15
   LDA   __local_f7__w13
   STA   __zpabi_mix__v_1
   JSR   mix
   LDA   g1
   AND   #$07
   CMP   #$04
   BEQ   .case@1
   CMP   #$07
   BEQ   .case@2
.case@1:
.case@2:
   LDA   #$00
   STA   __local_f7__w13
.loop@4_start:
   LDA   #$00
   LDY   #$FE
   STY   g1
   STA   g1+1
   LDA   gs4+1
   STA   __zpabi_mix__v_2
   ASL
   CLC
   ADC   __zpabi_mix__v_2
   STA   gs4+1
   LDY   #$00
   LDA   (gp5),Y
   STA   __zpabi_mix__v_0
   INY
   LDA   (gp5),Y
   STA   __zpabi_mix__v_1
   LDA   #$00
   STA   __zpabi_mix__v_2
   STA   __zpabi_mix__v_3
   JSR   mix
   INC   __local_f7__w13
   LDA   __local_f7__w13
   CMP   #$03
   BCC   .loop@4_start
   LDA   #$03
   STA   __local_f7__i14
.loop@5_start:
   LDA   __local_f7__i14
   BNE   .lb_skip@1
   JMP   .loop@5_break
.lb_skip@1:
   LDA   #$04
   STA   __local_f7__i15
.loop@6_start:
   LDA   __local_f7__i15
   BNE   .lb_skip@0
   JMP   .loop@6_break
.lb_skip@0:
   LDA   g2
   STA   __zpabi_mix__v_3
   DEC   g2
   ORA   #$00
   BMI   .sx_neg@2
   LDA   #$00
   BEQ   .sx_done@3
.sx_neg@2:
   LDA   #$FF
.sx_done@3:
   STA   __zpabi_mix__v_0
   STA   __local_f7__w13
   STA   __zpabi_mix__v_2
   LDA   __zpabi_mix__v_3
   CLC
   ADC   #$5A
   BCC   .carry_inc@0
   INC   __zpabi_mix__v_0
   BNE   .carry_inc@0
   INC   __local_f7__w13
   BNE   .carry_inc@0
   INC   __zpabi_mix__v_2
.carry_inc@0:
   AND   #$FF
   STA   __local_f7__p8_0
   LDA   __zpabi_mix__v_0
   STA   __local_f7__p8_1
   LDA   __local_f7__w13
   STA   __local_f7__p8_2
   LDA   __zpabi_mix__v_2
   STA   __local_f7__p8_3
   LDY   #$00
   LDA   (__zpabi_f7__pp10_0),Y
   STA   __zpabi_mix__v_0
   INY
   LDA   (__zpabi_f7__pp10_0),Y
   STA   __local_f7__w13
   INY
   LDA   (__zpabi_f7__pp10_0),Y
   STA   __zpabi_mix__v_2
   INY
   LDA   (__zpabi_f7__pp10_0),Y
   STA   __zpabi_mix__v_3
   LDA   __local_f7__p8_0
   AND   __zpabi_mix__v_0
   STA   __zpabi_mix__v_0
   LDA   __local_f7__p8_1
   AND   __local_f7__w13
   STA   __zpabi_mix__v_1
   LDA   __local_f7__p8_2
   AND   __zpabi_mix__v_2
   STA   __zpabi_mix__v_2
   LDA   __local_f7__p8_3
   AND   __zpabi_mix__v_3
   JSR   fin0@run3
   DEC   __local_f7__i15
   JMP   .loop@6_start
.loop@6_break:
   DEC   gs4+1
   DEC   __local_f7__i14
   JMP   .loop@5_start
.loop@5_break:
   LDA   #$FF
   STA   HARGS+8
   LDA   #$00
   STA   HARGS+9
   LDY   #$00
   STY   HARGS+10
   STA   HARGS+11
   RTS

f16:
   SUBROUTINE

   LDA   #$03
   STA   __local_f16__l22
   LDA   #$00
   STA   __local_f16__l22+1
   STA   __local_f16__l22+2
   STA   __local_f16__l22+3
   LDY   #$00
   LDA   (__zpabi_f16__pp20_0),Y
   CMP   #$80
   ROR
   STA   (__zpabi_f16__pp20_0),Y
   LDA   __zpabi_f16__p18_0
   STA   __zpabi_mix__v_0
   LDA   __zpabi_f16__p18_1
   STA   __zpabi_mix__v_1
   LDA   __zpabi_f16__p18_2
   STA   __zpabi_mix__v_2
   LDA   __zpabi_f16__p18_3
   JSR   fin0@run3
   LDA   g1
   EOR   #$FF
   STA   __zpabi_f7__p9_0
   LDA   g1+1
   EOR   #$FF
   STA   __zpabi_f7__p9_1
   LDA   #$FF
   STA   __zpabi_f7__p8_0
   LDA   #$7F
   STA   __zpabi_f7__p8_1
   LDA   #$00
   STA   __zpabi_f7__p8_2
   JSR   f16@run5
   STA   __zpabi_f7__p9_2
   STA   __zpabi_f7__p9_3
   LDA   #<__local_f16__l22
   STA   __zpabi_f7__pp10_0
   LDA   #>__local_f16__l22
   STA   __zpabi_f7__pp10_1
   JSR   f7
   LDA   HARGS+8
   STA   __local_f16__4
   LDY   #$00
   LDA   (__zpabi_f16__pp20_0),Y
   STA   __local_f16__2
   CLC
   ADC   #$03
   AND   #$07
   CMP   #$07
   BEQ   .lb_skip@2
   JMP   .default@9
.lb_skip@2:
   LDA   __zpabi_f16__p17_0
   STA   HARGS
   LDA   __zpabi_f16__p17_1
   STA   HARGS+1
   LDA   __zpabi_f16__p17_2
   STA   HARGS+2
   LDA   __zpabi_f16__p17_3
   STA   HARGS+3
   LDA   __zpabi_f16__p17_4
   STA   HARGS+4
   LDA   __zpabi_f16__p17_5
   STA   HARGS+5
   LDA   #$00
   STA   HARGS+6
   STA   HARGS+7
   LDA   #$04
   STA   HARGS+8
   JSR   asl64
   LDA   HARGS+14
   AND   #$0F
   LDX   HARGS+13
   STX   __local_f16__7
   EOR   #$08
   SEC
   SBC   #$08
   STA   __local_f16__6
   LDA   HARGS+9
   CLC
   ADC   ga3
   STA   __local_f16__3
   LDA   HARGS+10
   ADC   ga3+1
   STA   __local_f16__2
   LDA   HARGS+11
   ADC   ga3+2
   STA   __local_f16__1
   LDA   HARGS+12
   ADC   ga3+3
   STA   __local_f16__0
   BCC   .carry_inc@2
   INC   __local_f16__7
   BNE   .carry_inc@2
   INC   __local_f16__6
.carry_inc@2:
   LDA   __local_f16__3
   STA   __local_f16__5
   LDA   __local_f16__2
   STA   __local_f16__4
   LDA   __local_f16__1
   STA   __local_f16__3
   LDA   __local_f16__0
   STA   __local_f16__2
   LDA   __local_f16__7
   STA   __local_f16__1
   LDA   __local_f16__6
   AND   #$0F
   LDX   __local_f16__5
   STX   HARGS+16
   LDX   __local_f16__4
   STX   HARGS+17
   LDX   __local_f16__3
   STX   HARGS+18
   LDX   __local_f16__2
   STX   HARGS+19
   LDX   __local_f16__1
   STX   HARGS+20
   EOR   #$08
   SEC
   SBC   #$08
   STA   HARGS+21
   RTS
.default@9:
   LDA   __local_f16__2
   SEC
   SBC   #$01
   LDY   #$00
   STA   (__zpabi_f16__pp20_0),Y
   LDA   ga3+8
   SEC
   SBC   __local_f16__4
   LDA   ga3+9
   SBC   #$00
   LDA   ga3+10
   SBC   #$00
   LDA   ga3+11
   SBC   #$00
   LDA   #$00
   ROL
   STA   __local_f16__0
   LDA   #$00
   BPL   .sx_done@7
   LDA   #$FF
.sx_done@7:
   STA   HARGS+18
   STA   HARGS+19
   STA   HARGS+20
   STA   HARGS+21
   LDA   __local_f16__0
   STA   HARGS+16
   LDA   #$00
   STA   HARGS+17
   RTS

f16@run5:
   SUBROUTINE

   STA   __zpabi_f7__p8_3
   STA   __zpabi_f7__p8_4
   STA   __zpabi_f7__p8_5
   RTS

f24:
   SUBROUTINE

   LDA   #$80
   STA   __local_f24__l27
   LDA   g2
   STA   __local_f24__13
   BMI   .sx_neg@8
   LDA   #$00
   BEQ   .sx_done@9
.sx_neg@8:
   LDA   #$FF
.sx_done@9:
   STA   __local_f24__12
   BMI   .sx_neg@10
   LDA   #$00
   BEQ   .sx_done@11
.sx_neg@10:
   LDA   #$FF
.sx_done@11:
   STA   __local_f24__11
   STA   __local_f24__10
   STA   __local_f24__9
   STA   __local_f24__8
   LDA   #$80
   BMI   .sx_neg@12
   LDA   #$00
   BEQ   .sx_done@13
.sx_neg@12:
   LDA   #$FF
.sx_done@13:
   STA   __local_f24__0
   SEC
   LDA   #$00
   SBC   #$80
   STA   __local_f24__7
   LDA   #$00
   SBC   __local_f24__0
   STA   __local_f24__6
   BMI   .sx_neg@14
   LDA   #$00
   BEQ   .sx_done@15
.sx_neg@14:
   LDA   #$FF
.sx_done@15:
   STA   __local_f24__5
   STA   __local_f24__4
   LDA   gs4+2
   STA   HARGS
   LDA   gs4+3
   STA   HARGS+1
   LDA   gs4+4
   STA   HARGS+2
   LDA   gs4+5
   STA   HARGS+3
   LDA   #$05
   STA   HARGS+4
   LDA   #$00
   STA   HARGS+5
   LDY   #$00
   STY   HARGS+6
   STA   HARGS+7
   JSR   udivmod32
   LDA   HARGS+8
   STA   __zpabi_f16__p19
   LDA   __local_f24__13
   STA   __zpabi_f16__p17_0
   LDA   __local_f24__12
   STA   __zpabi_f16__p17_1
   LDA   __local_f24__11
   STA   __zpabi_f16__p17_2
   LDA   __local_f24__10
   STA   __zpabi_f16__p17_3
   LDA   __local_f24__9
   STA   __zpabi_f16__p17_4
   LDA   __local_f24__8
   STA   __zpabi_f16__p17_5
   LDA   __local_f24__7
   STA   __zpabi_f16__p18_0
   LDA   __local_f24__6
   STA   __zpabi_f16__p18_1
   LDA   __local_f24__5
   STA   __zpabi_f16__p18_2
   LDA   __local_f24__4
   STA   __zpabi_f16__p18_3
   LDA   #<__local_f24__l27
   STA   __zpabi_f16__pp20_0
   LDA   #>__local_f24__l27
   JSR   f24@run6
   LDA   gs4+1
   ORA   #$81
   STA   gs4+1
   JSR   fin1@run2
   LDA   #$00
   AND   ga3+8
   STA   __zpabi_f16__p17_0
   LDA   #$80
   AND   ga3+9
   STA   __zpabi_f16__p17_1
   LDA   #$00
   AND   ga3+10
   STA   __zpabi_f16__p17_2
   LDA   #$00
   AND   ga3+11
   STA   __zpabi_f16__p17_3
   LDY   #$00
   LDA   (__zpabi_f24__pp26_0),Y
   SEC
   SBC   #$03
   STA   __zpabi_f16__p18_0
   INY
   LDA   (__zpabi_f24__pp26_0),Y
   SBC   #$00
   STA   __zpabi_f16__p18_1
   LDA   #$00
   STA   __zpabi_f16__p17_4
   STA   __zpabi_f16__p17_5
   STA   __zpabi_f16__p18_2
   STA   __zpabi_f16__p18_3
   LDA   __local_f24__l27
   STA   __zpabi_f16__p19
   LDA   #<__local_f24__l27
   STA   __zpabi_f16__pp20_0
   LDA   #>__local_f24__l27
   JSR   f24@run1
   LDA   #$FE
   RTS

f24@run6:
   SUBROUTINE

   STA   __zpabi_f16__pp20_1
   JMP   f16

f24@run1:
   SUBROUTINE

   JSR   f24@run6
   LDA   HARGS+16
   STA   __zpabi_mix__v_0
   LDA   HARGS+17
   STA   __zpabi_mix__v_1
   LDA   HARGS+18
   STA   __zpabi_mix__v_2
   LDA   HARGS+19
   STA   __zpabi_mix__v_3
   JMP   mix

main:
   SUBROUTINE

   LDA   #$03
   STA   __local_main__l29
   STA   __local_main__l32
   LDA   #$FF
   STA   __local_main__l31
   STA   __local_main__l31+1
   STA   __local_main__l31+2
   LDA   #$7F
   STA   __local_main__l31+3
   LDA   #$00
   STA   __local_main__l32+1
   LDA   gs4+2
   STA   __local_main__3
   LDA   gs4+3
   STA   __local_main__2
   LDA   gs4+4
   STA   __local_main__1
   LDA   gs4+5
   BNE   .cmp_true@16
   LDA   __local_main__1
   BNE   .cmp_true@16
   LDA   __local_main__2
   BNE   .cmp_true@16
   LDA   __local_main__3
   CMP   #$FF
   BNE   .cmp_true@16
   LDA   #$00
   BEQ   .cmp_end@17
.cmp_true@16:
   LDA   #$01
.cmp_end@17:
   STA   __zpabi_f7__p8_0
   LDA   #$00
   BPL   .sx_done@20
   LDA   #$FF
.sx_done@20:
   STA   __zpabi_f7__p8_2
   JSR   f16@run5
   LDA   #$00
   STA   __zpabi_f7__p8_1
   STA   __zpabi_f7__p9_2
   STA   __zpabi_f7__p9_3
   LDA   g1
   STA   __zpabi_f7__p9_0
   LDA   g1+1
   STA   __zpabi_f7__p9_1
   JSR   main@run4
   LDA   HARGS+8
   STA   __local_main__l32
   LDA   HARGS+9
   STA   __local_main__l32+1
   LDA   g1
   STA   HARGS
   LDA   g1+1
   STA   HARGS+1
   LDA   #$05
   STA   HARGS+2
   LDA   #$00
   STA   HARGS+3
   JSR   udivmod16
   LDA   __local_main__l29
   STA   __local_main__2
   BMI   .sx_neg@21
   LDA   #$00
   BEQ   .sx_done@22
.sx_neg@21:
   LDA   #$FF
.sx_done@22:
   STA   __zpabi_f16__p18_1
   STA   __zpabi_f16__p18_2
   STA   __zpabi_f16__p18_3
   LDA   HARGS+4
   STA   __zpabi_f16__p17_0
   LDA   HARGS+5
   STA   __zpabi_f16__p17_1
   LDA   #$00
   STA   __zpabi_f16__p17_2
   STA   __zpabi_f16__p17_3
   STA   __zpabi_f16__p17_4
   STA   __zpabi_f16__p17_5
   LDA   __local_main__2
   STA   __zpabi_f16__p18_0
   LDA   #$FF
   STA   __zpabi_f16__p19
   LDA   #<__local_main__l29
   STA   __zpabi_f16__pp20_0
   LDA   #>__local_main__l29
   JSR   f24@run6
   LDA   HARGS+16
   STA   __local_main__9
   LDA   HARGS+17
   STA   __local_main__8
   JSR   main@run0
   BNE   .lb_skip@3
   JMP   .if_end@5
.lb_skip@3:
   LDA   __local_main__l32
   STA   __local_main__5
   LDA   __local_main__l32+1
   STA   __zpabi_f7__p8_0
   LDA   gs4+2
   STA   __local_main__3
   LDA   gs4+3
   STA   __local_main__2
   LDA   gs4+4
   STA   __local_main__1
   LDA   #$00
   CMP   gs4+5
   BNE   .cmp_true@23
   CMP   __local_main__1
   BNE   .cmp_true@23
   LDA   __zpabi_f7__p8_0
   CMP   __local_main__2
   BNE   .cmp_true@23
   LDA   __local_main__5
   CMP   __local_main__3
   BNE   .cmp_true@23
   LDA   #$00
   BEQ   .cmp_end@24
.cmp_true@23:
   LDA   #$01
.cmp_end@24:
   STA   __local_main__2
   LDA   #$00
   BPL   .sx_done@27
   LDA   #$FF
.sx_done@27:
   STA   __zpabi_f24__p25_2
   STA   __zpabi_f24__p25_3
   LDA   __local_main__2
   STA   __zpabi_f24__p25_0
   LDA   #$00
   STA   __zpabi_f24__p25_1
   LDA   #<__local_main__l32
   STA   __zpabi_f24__pp26_0
   LDA   #>__local_main__l32
   STA   __zpabi_f24__pp26_1
   JSR   f24
   LDA   __local_main__l29
   STA   __local_main__2
   JSR   main@run0
   STA   __zpabi_f16__p19
   LDA   #$00
   STA   __zpabi_f16__p17_0
   STA   __zpabi_f16__p17_1
   STA   __zpabi_f16__p17_2
   STA   __zpabi_f16__p17_3
   STA   __zpabi_f16__p17_4
   STA   __zpabi_f16__p17_5
   LDA   __local_main__2
   STA   __zpabi_f16__p18_0
   LDA   #$00
   STA   __zpabi_f16__p18_1
   STA   __zpabi_f16__p18_2
   STA   __zpabi_f16__p18_3
   LDA   #<__local_main__l29
   STA   __zpabi_f16__pp20_0
   LDA   #>__local_main__l29
   JSR   f24@run1
   LDA   gs4+1
   LDX   #$FF
   SEC
.div_loop@28:
   INX
   SBC   #$05
   BCS   .div_loop@28
   ADC   #$05
   JSR   fin1@run2
.if_end@5:
   LDA   gs4+1
   LSR
   LSR
   LSR
   LSR
   LSR
   LSR
   STA   gs4+1
   JSR   main@run0
   BEQ   .lnot_true@29
   LDA   #$00
   BEQ   .lnot_end@30
.lnot_true@29:
   LDA   #$01
.lnot_end@30:
   STA   __zpabi_f7__p8_0
   LDA   #$00
   BPL   .sx_done@32
   LDA   #$FF
.sx_done@32:
   STA   __zpabi_f7__p8_2
   LDY   #$00
   LDA   (DPTR),Y
   STA   __local_main__1
   BMI   .sx_neg@33
   LDA   #$00
   BEQ   .sx_done@34
.sx_neg@33:
   LDA   #$FF
.sx_done@34:
   STA   __local_main__0
   LDA   __local_main__8
   CMP   __local_main__0
   BNE   .thread@0
   LDA   __local_main__9
   CMP   __local_main__1
   BEQ   .cmp_true@35
.thread@0:
   LDA   #$00
   BEQ   .cmp_end@36
.cmp_true@35:
   LDA   #$01
.cmp_end@36:
   STA   __local_main__2
   LDA   #$00
   BPL   .sx_done@39
   LDA   #$FF
.sx_done@39:
   STA   __zpabi_f7__p9_2
   STA   __zpabi_f7__p9_3
   LDA   #$00
   STA   __zpabi_f7__p8_1
   LDA   __zpabi_f7__p8_2
   JSR   f16@run5
   LDA   __local_main__2
   STA   __zpabi_f7__p9_0
   LDA   #$00
   STA   __zpabi_f7__p9_1
   JSR   main@run4
   JSR   fin0
   JSR   fin1
   LDA   acc+1
   AND   #$7F
   TAX
   LDA   acc
   RTS

main@run4:
   SUBROUTINE

   LDA   #<__local_main__l31
   STA   __zpabi_f7__pp10_0
   LDA   #>__local_main__l31
   STA   __zpabi_f7__pp10_1
   JMP   f7

main@run0:
   SUBROUTINE

   LDA   gp6
   STA   DPTR
   LDA   gp6+1
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   RTS

acc:
   DS.B  2

ga3:
   DC.L  $7FFFFFFF
   DC.L  $00008000
   DC.L  $00000003
   DS.B  4

gs4:
   DS.B  2
   DC.L  $00007FFF

gp6:
   DC.W  g2

__zp_init:
   SUBROUTINE

   LDA   #$00
   STA   g1
   STA   g1+1
   LDA   #$FF
   STA   g2
   LDA   #<g1
   STA   gp5
   LDA   #>g1
   STA   gp5+1
   RTS
