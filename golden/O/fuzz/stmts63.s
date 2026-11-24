SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
z3	EQU	$26
g1	EQU	$27
g2	EQU	$29
gp6	EQU	$2A
__zpabi_f14__p16	EQU	$80
__zpabi_f22__p24_0	EQU	$80
__zpabi_f14__p17_0	EQU	$81
__zpabi_f22__p24_1	EQU	$81
__zpabi_f14__p17_1	EQU	$82
__zpabi_f22__p24_2	EQU	$82
__zpabi_f22__p24_3	EQU	$83
__zpabi_f22__p25	EQU	$84
__zpabi_f7__pp9_0	EQU	$85
__zpabi_f7__pp9_1	EQU	$86
__zpabi_mix__v_0	EQU	$87
__zpabi_mix__v_1	EQU	$88
__zpabi_mix__v_2	EQU	$89
__zpabi_mix__v_3	EQU	$8A
__local_main__0	EQU	$8B
__local_main__1	EQU	$8C
__local_main__2	EQU	$8D
__local_main__3	EQU	$8E
__local_main__4	EQU	$8F
__local_main__6	EQU	$91
__local_main__7	EQU	$92
__local_main__8	EQU	$93
__local_main__9	EQU	$94
__local_main__i32	EQU	$95
__local_main__l31	EQU	$96
__local_f22__1	EQU	$99
__local_f14__2	EQU	$9A
__local_f22__p23	EQU	$9A
__local_f22__2	EQU	$9B
__local_f22__3	EQU	$9C
__local_f14__5	EQU	$9D
__local_f22__4	EQU	$9D
__local_f22__5	EQU	$9E
__local_f22__l26	EQU	$9F
__local_f22__6	EQU	$A0
__local_f22__7	EQU	$A1
__local_f22__8	EQU	$A2
__local_f22__9	EQU	$A3
__local_f22__10	EQU	$A4
__local_f22__11	EQU	$A5
__local_f14__14	EQU	$A6
__local_f22__l27	EQU	$A6
__local_f14__l18	EQU	$A7
__local_f7__2	EQU	$AC
__local_f7__3	EQU	$AD
__local_f7__w13	EQU	$AE
__local_mix__0	EQU	$AF
__local_mix__1	EQU	$B0
__local_mix__2	EQU	$B1
__local_mix__3	EQU	$B2

; @zp-link-meta-begin
; def f14 params=__zpabi_f14__p15_0,__zpabi_f14__p15_1,__zpabi_f14__p16,__zpabi_f14__p17_0,__zpabi_f14__p17_1 locals=__local_f14__0,__local_f14__1,__local_f14__2,__local_f14__3,__local_f14__4,__local_f14__5,__local_f14__6,__local_f14__7,__local_f14__8,__local_f14__9,__local_f14__10,__local_f14__11,__local_f14__12,__local_f14__13,__local_f14__14,__local_f14__l18,__local_f14__16 indirect=false in_cycle=false param_regs=A,X,,,
; def f22 params=__zpabi_f22__p23,__zpabi_f22__p24_0,__zpabi_f22__p24_1,__zpabi_f22__p24_2,__zpabi_f22__p24_3,__zpabi_f22__p25 locals=__local_f22__0,__local_f22__1,__local_f22__p23,__local_f22__2,__local_f22__3,__local_f22__4,__local_f22__5,__local_f22__l26,__local_f22__6,__local_f22__7,__local_f22__8,__local_f22__9,__local_f22__10,__local_f22__11,__local_f22__l27,__local_f22__13,__local_f22__14,__local_f22__15 indirect=false in_cycle=false param_regs=A,,,,,
; def f7 params=__zpabi_f7__p8_0,__zpabi_f7__p8_1,__zpabi_f7__pp9_0,__zpabi_f7__pp9_1 locals=__local_f7__0,__local_f7__1,__local_f7__2,__local_f7__3,__local_f7__w13 indirect=false in_cycle=false param_regs=A,X,,
; def fin0 params= locals=__local_fin0__0,__local_fin0__1,__local_fin0__2,__local_fin0__3 indirect=false in_cycle=false
; def fin1 params= locals=__local_fin1__0,__local_fin1__1,__local_fin1__2,__local_fin1__3 indirect=false in_cycle=false
; def fin2 params= locals=__local_fin2__0,__local_fin2__1 indirect=false in_cycle=false
; def main params= locals=__local_main__0,__local_main__1,__local_main__2,__local_main__3,__local_main__4,__local_main__5,__local_main__6,__local_main__7,__local_main__8,__local_main__9,__local_main__i32,__local_main__l31,__local_main__11 indirect=false in_cycle=false
; def mix params=__zpabi_mix__v_0,__zpabi_mix__v_1,__zpabi_mix__v_2,__zpabi_mix__v_3 locals=__local_mix__0,__local_mix__1,__local_mix__2,__local_mix__3 indirect=false in_cycle=false
; call f14 -> f7
; call f14 -> mix
; call f22 -> f7
; call f22 -> mix
; call f7 -> mix
; call fin0 -> mix
; call fin1 -> mix
; call fin2 -> mix
; call main -> f14
; call main -> f22
; call main -> f7
; call main -> fin0
; call main -> fin1
; call main -> fin2
; call main -> mix
; item mix section=.text local=false zero=false
; item fin0 section=.text local=false zero=false
; item fin0@run4 section=.text local=true zero=false
; item fin0@run0 section=.text local=true zero=false
; item fin0@tail0 section=.text local=true zero=false
; item fin1 section=.text local=false zero=false
; item fin2 section=.text local=false zero=false
; item f7 section=.text local=false zero=false
; item f7@run3 section=.text local=true zero=false
; item f14 section=.text local=false zero=false
; item f14@run2 section=.text local=true zero=false
; item f14@run1 section=.text local=true zero=false
; item f22 section=.text local=false zero=false
; item f22@run5 section=.text local=true zero=false
; item main section=.text local=false zero=false
; item acc section=.bss local=false zero=true
; item ga4 section=.data local=false zero=false
; item gs5 section=.data local=false zero=false
; item @3.s12 section=.data local=true zero=false
; item @17.s28 section=.bss local=true zero=true
; item __zp_init section=.text local=false zero=false
; zp z3 size=1 at=auto def=true init=$FF
; zp g1 size=2 at=auto def=true init=$00,$80 weight=43
; zp g2 size=1 at=auto def=true init=$01 weight=14
; zp gp6 size=2 at=auto def=true init=<g2,>g2 weight=11
; runtime hargs_size=32 hargs_need=23
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

   JSR   fin0@run4
   LDA   #$00
   STA   __zpabi_mix__v_2
   JSR   fin0@run0
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
   LDA   z3
   BMI   .sx_neg@2
   LDA   #$00
   BEQ   .sx_done@3
.sx_neg@2:
   LDA   #$FF
.sx_done@3:
   STA   __zpabi_mix__v_1
   STA   __zpabi_mix__v_2
   STA   __zpabi_mix__v_3
   LDA   z3
   STA   __zpabi_mix__v_0
   JSR   mix
   LDA   ga4
   STA   __zpabi_mix__v_0
   LDA   ga4+1
   STA   __zpabi_mix__v_1
   LDA   ga4+2
   STA   __zpabi_mix__v_2
   LDA   ga4+3
   JMP   fin0@tail0

fin0@run4:
   SUBROUTINE

   LDA   g1
   STA   __zpabi_mix__v_0
   LDA   g1+1
   STA   __zpabi_mix__v_1
   RTS

fin0@run0:
   SUBROUTINE

   STA   __zpabi_mix__v_3
   JMP   mix

fin0@tail0:
   SUBROUTINE

   STA   __zpabi_mix__v_3
   JMP   mix

fin1:
   SUBROUTINE

   LDA   ga4+7
   STA   __zpabi_mix__v_0
   LDA   ga4+8
   STA   __zpabi_mix__v_1
   LDA   ga4+9
   STA   __zpabi_mix__v_2
   LDA   ga4+10
   JSR   fin0@run0
   LDA   ga4+14
   STA   __zpabi_mix__v_0
   LDA   ga4+15
   STA   __zpabi_mix__v_1
   LDA   ga4+16
   STA   __zpabi_mix__v_2
   LDA   ga4+17
   JSR   fin0@run0
   LDA   ga4+21
   STA   __zpabi_mix__v_0
   LDA   ga4+22
   STA   __zpabi_mix__v_1
   LDA   ga4+23
   STA   __zpabi_mix__v_2
   LDA   ga4+24
   JSR   fin0@run0
   LDA   gs5+1
   STA   __zpabi_mix__v_0
   LDA   gs5+2
   STA   __zpabi_mix__v_1
   LDA   gs5+3
   STA   __zpabi_mix__v_2
   LDA   gs5+4
   JMP   fin0@tail0

fin2:
   SUBROUTINE

   LDA   gs5+5
   STA   __zpabi_mix__v_0
   LDA   gs5+6
   STA   __zpabi_mix__v_1
   LDA   #$00
   STA   __zpabi_mix__v_2
   JMP   fin0@tail0

f7:
   SUBROUTINE

   LDA   #$00
   STA   __local_f7__w13
.loop@0_start:
   JSR   fin0@run4
   LDA   __zpabi_mix__v_0
   ASL
   STA   __local_f7__2
   LDA   __zpabi_mix__v_1
   ROL
   STA   __local_f7__3
   LDA   __local_f7__2
   CLC
   ADC   __zpabi_mix__v_0
   STA   __zpabi_mix__v_0
   LDA   __local_f7__3
   ADC   __zpabi_mix__v_1
   STA   __zpabi_mix__v_1
   LDA   __zpabi_mix__v_0
   STA   g1
   LDA   __zpabi_mix__v_1
   STA   g1+1
   LDA   __zpabi_mix__v_0
   ORA   __zpabi_mix__v_1
   BEQ   .if_else@3
   LDY   #$01
   LDA   (__zpabi_f7__pp9_0),Y
   STA   __zpabi_mix__v_1
   DEY
   LDA   (__zpabi_f7__pp9_0),Y
   ORA   #$5A
   STA   __zpabi_mix__v_0
   STA   (__zpabi_f7__pp9_0),Y
   LDA   __zpabi_mix__v_1
   BMI   .sx_neg@4
   LDA   #$00
   BEQ   .sx_done@5
.sx_neg@4:
   LDA   #$FF
.sx_done@5:
   JSR   f7@run3
.if_else@3:
   INC   __local_f7__w13
   LDA   __local_f7__w13
   CMP   #$03
   BCC   .loop@0_start
   INC   @3.s12
   BNE   .inc_done@0
   INC   @3.s12+1
.inc_done@0:
   LDA   @3.s12
   AND   #$07
   CMP   #$01
   BNE   .switch@1_break
   LDA   g2
   BMI   .sx_neg@6
   LDA   #$00
   BEQ   .sx_done@7
.sx_neg@6:
   LDA   #$FF
.sx_done@7:
   STA   __zpabi_mix__v_1
   LDA   g2
   STA   g1
   LDA   __zpabi_mix__v_1
   STA   g1+1
.switch@1_break:
   LDA   ga4
   RTS

f7@run3:
   SUBROUTINE

   STA   __zpabi_mix__v_2
   JMP   fin0@run0

f14:
   SUBROUTINE

   LDA   #$00
   STA   __local_f14__l18
   STA   __local_f14__l18+1
   LDY   #$00
   LDA   (gp6),Y
   STA   __local_f14__2
   BMI   .sx_neg@8
   LDA   #$00
   BEQ   .sx_done@9
.sx_neg@8:
   LDA   #$FF
.sx_done@9:
   JSR   f14@run2
   LDA   __local_f14__2
   JSR   f7
   STA   __local_f14__14
   LDA   g2
   BMI   .sx_neg@10
   LDA   #$00
   BEQ   .sx_done@11
.sx_neg@10:
   LDA   #$FF
.sx_done@11:
   STA   __local_f14__5
   BMI   .sx_neg@12
   LDA   #$00
   BEQ   .sx_done@13
.sx_neg@12:
   LDA   #$FF
.sx_done@13:
   LDX   g2
   STX   HARGS+8
   LDX   __local_f14__5
   STX   HARGS+9
   STA   HARGS+10
   STA   HARGS+11
   STA   HARGS+12
   STA   HARGS+13
   AND   #$7F
   STA   HARGS+14
   LDA   ga4
   STA   HARGS
   LDA   ga4+1
   STA   HARGS+1
   LDA   ga4+2
   STA   HARGS+2
   LDA   ga4+3
   STA   HARGS+3
   LDA   ga4+4
   STA   HARGS+4
   LDA   ga4+5
   STA   HARGS+5
   LDA   ga4+6
   STA   HARGS+6
   LDA   #$00
   STA   HARGS+7
   STA   HARGS+15
   JSR   mul64
   JSR   f14@run1
   LDA   __local_f14__14
   CLC
   ADC   #$01
   STA   __local_f14__2
   LDA   #$00
   ADC   #$00
   JSR   f14@run2
   LDA   __local_f14__2
   JSR   f7
   LDA   __zpabi_f14__p17_0
   AND   __local_f14__l18
   RTS

f14@run2:
   SUBROUTINE

   TAX
   LDA   #<__local_f14__l18
   STA   __zpabi_f7__pp9_0
   LDA   #>__local_f14__l18
   STA   __zpabi_f7__pp9_1
   RTS

f14@run1:
   SUBROUTINE

   LDA   HARGS+16
   STA   __zpabi_mix__v_0
   LDA   HARGS+17
   STA   __zpabi_mix__v_1
   LDA   HARGS+18
   STA   __zpabi_mix__v_2
   LDA   HARGS+19
   JMP   fin0@run0

f22:
   SUBROUTINE

   STA   __local_f22__p23
   LDA   #$2B
   STA   __local_f22__l26
   LDA   #$27
   STA   __local_f22__6
   LDA   #$77
   STA   __local_f22__7
   LDA   #$6B
   STA   __local_f22__8
   LDA   #$11
   STA   __local_f22__9
   LDA   #$30
   STA   __local_f22__10
   LDA   #$13
   STA   __local_f22__11
   LDA   #$02
   STA   __local_f22__l27
   LDA   #$00
   STA   __local_f22__l27+1
   LDA   #<__local_f22__l27
   STA   __zpabi_f7__pp9_0
   LDA   #>__local_f22__l27
   STA   __zpabi_f7__pp9_1
   LDX   __zpabi_f22__p24_1
   LDA   __zpabi_f22__p24_0
   JSR   f7
   STA   __zpabi_mix__v_0
   LDA   #$00
   STA   __zpabi_mix__v_1
   JSR   f7@run3
   LDA   __local_f22__p23
   LSR
   LDA   #$00
   ROR
   STA   __zpabi_mix__v_0
   BMI   .sx_neg@14
   LDA   #$00
   BEQ   .sx_done@15
.sx_neg@14:
   LDA   #$FF
.sx_done@15:
   STA   __zpabi_mix__v_1
   JSR   f7@run3
   LDA   g2
   BMI   .sx_neg@16
   LDA   #$00
   BEQ   .sx_done@17
.sx_neg@16:
   LDA   #$FF
.sx_done@17:
   JSR   f22@run5
   STA   __local_f22__1
   LDA   @17.s28+1
   SBC   __zpabi_mix__v_0
   ORA   __local_f22__1
   BNE   .lb_skip@2
   JMP   .if_end@6
.lb_skip@2:
   LDA   g2
   BMI   .sx_neg@18
   LDA   #$00
   BEQ   .sx_done@19
.sx_neg@18:
   LDA   #$FF
.sx_done@19:
   JSR   f22@run5
   LDA   @17.s28+1
   SBC   __zpabi_mix__v_0
   BCC   .lb_skip@1
   JMP   .if_else@8
.lb_skip@1:
   LDA   @17.s28
   ORA   @17.s28+1
   BNE   .lb_skip@0
   JMP   .if_end@7
.lb_skip@0:
   LDA   __local_f22__l26
   STA   __zpabi_mix__v_0
   LDA   __local_f22__6
   STA   __local_f22__4
   LDA   __local_f22__7
   STA   __local_f22__3
   LDA   __local_f22__8
   STA   __local_f22__2
   LDA   __local_f22__9
   STA   __local_f22__p23
   LDA   __local_f22__10
   STA   __local_f22__1
   LDA   __local_f22__11
   STA   __local_f22__l26
   LDA   __zpabi_mix__v_0
   SEC
   SBC   #$5A
   STA   __zpabi_mix__v_0
   LDA   __local_f22__4
   SBC   #$00
   STA   __local_f22__4
   LDA   __local_f22__3
   SBC   #$00
   STA   __local_f22__3
   LDA   __local_f22__2
   SBC   #$00
   STA   __local_f22__2
   LDA   __local_f22__p23
   SBC   #$00
   STA   __local_f22__p23
   LDA   __local_f22__1
   SBC   #$00
   STA   __local_f22__1
   BCS   .carry_inc@0
   DEC   __local_f22__l26
.carry_inc@0:
   LDA   __zpabi_mix__v_0
   STA   __local_f22__5
   LDA   __local_f22__l26
   AND   #$7F
   STA   __zpabi_mix__v_0
   LDA   __local_f22__5
   STA   __local_f22__l26
   LDA   __local_f22__4
   STA   __local_f22__6
   LDA   __local_f22__3
   STA   __local_f22__7
   LDA   __local_f22__2
   STA   __local_f22__8
   LDA   __local_f22__p23
   STA   __local_f22__9
   LDA   __local_f22__1
   STA   __local_f22__10
   LDA   __zpabi_mix__v_0
   STA   __local_f22__11
   LDA   __local_f22__5
   STA   HARGS+16
   LDA   __local_f22__4
   STA   HARGS+17
   LDA   __local_f22__3
   STA   HARGS+18
   LDA   __local_f22__2
   STA   HARGS+19
   LDA   __local_f22__p23
   STA   HARGS+20
   LDA   __local_f22__1
   STA   HARGS+21
   LDA   __zpabi_mix__v_0
   AND   #$07
   STA   HARGS+22
   RTS
.if_else@8:
   LDY   #$00
   LDA   (gp6),Y
   AND   #$07
   BEQ   .if_end@6
   CMP   #$04
   BEQ   .case@7
   CMP   #$06
   BEQ   .case@8
.case@7:
.case@8:
.if_end@7:
.if_end@6:
   LDA   #$00
   CMP   ga4+6
   BNE   .vthread@0
   CMP   ga4+5
   BNE   .vthread@0
   CMP   ga4+4
   BNE   .vthread@0
   CMP   ga4+3
   BNE   .vthread@0
   CMP   ga4+2
   BNE   .vthread@0
   CMP   ga4+1
   BNE   .vthread@0
   LDA   #$01
   CMP   ga4
   BEQ   .cmp_true@20
   LDA   #$00
.vthread@0:
.cmp_true@20:
   STA   __local_f22__4
   LDA   #$00
   BPL   .sx_done@24
   LDA   #$FF
.sx_done@24:
   LDX   __local_f22__4
   STX   HARGS+16
   LDX   #$00
   STX   HARGS+17
   STA   HARGS+18
   STA   HARGS+19
   STA   HARGS+20
   STA   HARGS+21
   AND   #$07
   STA   HARGS+22
   RTS

f22@run5:
   SUBROUTINE

   STA   __zpabi_mix__v_0
   LDA   @17.s28
   SEC
   SBC   g2
   RTS

main:
   SUBROUTINE

   LDA   #$02
   STA   __local_main__l31
   LDA   #$00
   STA   __local_main__l31+1
   LDY   #$00
   LDA   (gp6),Y
   STA   __local_main__7
   BMI   .sx_neg@25
   LDA   #$00
   BEQ   .sx_done@26
.sx_neg@25:
   LDA   #$FF
.sx_done@26:
   LDX   gs5+1
   STX   __local_main__3
   LDX   gs5+2
   STX   __local_main__2
   LDX   gs5+3
   STX   __local_main__1
   CMP   gs5+4
   BNE   .thread@1
   CMP   __local_main__1
   BNE   .thread@1
   CMP   __local_main__2
   BNE   .thread@1
   LDA   __local_main__7
   CMP   __local_main__3
   BEQ   .cmp_true@27
.thread@1:
   LDA   #$00
   BEQ   .cmp_end@28
.cmp_true@27:
   LDA   #$01
.cmp_end@28:
   STA   __local_main__0
   LDA   #$00
   BPL   .sx_done@31
   LDA   #$FF
.sx_done@31:
   STA   __zpabi_mix__v_2
   STA   __zpabi_mix__v_3
   LDA   __local_main__0
   STA   __zpabi_mix__v_0
   LDA   #$00
   STA   __zpabi_mix__v_1
   JSR   mix
   LDA   z3
   STA   gs5+1
   BMI   .sx_neg@32
   LDA   #$00
   BEQ   .sx_done@33
.sx_neg@32:
   LDA   #$FF
.sx_done@33:
   STA   gs5+2
   STA   gs5+3
   STA   gs5+4
   LDA   g2
   SEC
   SBC   gs5+5
   STA   __local_main__6
   LDA   g1
   STA   __zpabi_f22__p24_0
   LDA   g1+1
   STA   __local_main__4
   LDA   gs5+5
   STA   __local_main__3
   LDA   gs5+6
   STA   __local_main__2
   LDA   z3
   BMI   .sx_neg@34
   LDA   #$00
   BEQ   .sx_done@35
.sx_neg@34:
   LDA   #$FF
.sx_done@35:
   STA   __local_main__0
   LDA   __local_main__2
   CMP   __local_main__0
   BNE   .thread@2
   LDA   __local_main__3
   CMP   z3
   BEQ   .cmp_true@36
.thread@2:
   LDA   #$00
   BEQ   .cmp_end@37
.cmp_true@36:
   LDA   #$01
.cmp_end@37:
   STA   __zpabi_f22__p25
   LDA   __local_main__4
   STA   __zpabi_f22__p24_1
   LDA   #$00
   STA   __zpabi_f22__p24_2
   STA   __zpabi_f22__p24_3
   LDA   __local_main__6
   JSR   f22
   JSR   f14@run1
   LDA   #$00
   STA   __local_main__i32
.loop@10_start:
   LDA   __local_main__i32
   CMP   #$04
   BCC   .lb_skip@4
   JMP   .loop@10_break
.lb_skip@4:
   LDA   gs5+5
   BNE   .carry_inc@2
   DEC   gs5+6
.carry_inc@2:
   DEC   gs5+5
   LDA   __local_main__i32
   AND   #$03
   STA   __local_main__1
   ASL
   ASL
   ASL
   SEC
   SBC   __local_main__1
   STA   __local_main__9
   LDX   __local_main__9
   LDA   ga4,X
   STA   HARGS
   LDA   ga4+1,X
   STA   HARGS+1
   LDA   ga4+2,X
   STA   HARGS+2
   LDA   ga4+3,X
   STA   HARGS+3
   LDA   ga4+4,X
   STA   HARGS+4
   LDA   ga4+5,X
   STA   HARGS+5
   LDA   ga4+6,X
   STA   HARGS+6
   LDA   #$00
   STA   HARGS+7
   LDA   #$04
   STA   HARGS+8
   JSR   asl64
   LDA   HARGS+9
   STA   __local_main__1
   LDA   HARGS+10
   STA   __local_main__0
   LDA   HARGS+11
   STA   __local_main__4
   LDA   HARGS+12
   STA   __local_main__3
   LDA   HARGS+13
   STA   __local_main__2
   LDA   HARGS+14
   STA   __local_main__8
   LDA   HARGS+15
   AND   #$7F
   STA   __local_main__7
   LDX   __local_main__9
   LDA   __local_main__1
   STA   ga4,X
   LDA   __local_main__0
   STA   ga4+1,X
   LDA   __local_main__4
   STA   ga4+2,X
   LDA   __local_main__3
   STA   ga4+3,X
   LDA   __local_main__2
   STA   ga4+4,X
   LDA   __local_main__8
   STA   ga4+5,X
   LDA   __local_main__7
   STA   ga4+6,X
   LDA   __local_main__1
   SEC
   SBC   #$FF
   STA   __local_main__1
   LDA   __local_main__0
   SBC   #$FF
   STA   __local_main__0
   LDA   __local_main__4
   SBC   #$00
   STA   __local_main__4
   LDA   __local_main__3
   SBC   #$00
   STA   __local_main__3
   LDA   __local_main__2
   SBC   #$00
   STA   __local_main__2
   LDA   __local_main__8
   SBC   #$00
   STA   __local_main__8
   BCS   .carry_inc@3
   DEC   __local_main__7
.carry_inc@3:
   LDA   __local_main__1
   STA   __local_main__6
   LDA   __local_main__0
   STA   __zpabi_f22__p24_0
   LDA   __local_main__8
   STA   __local_main__1
   LDA   __local_main__7
   AND   #$7F
   STA   __local_main__0
   LDA   __local_main__6
   ORA   __zpabi_f22__p24_0
   ORA   __local_main__4
   ORA   __local_main__3
   ORA   __local_main__2
   ORA   __local_main__1
   ORA   __local_main__0
   BNE   .lb_skip@3
   JMP   .if_end@10
.lb_skip@3:
   LDA   g1
   LDX   g1+1
   STX   __local_main__0
   CLC
   SBC   #$00
   LDA   __local_main__0
   SBC   #$00
   LDA   #$00
   ROL
   STA   __local_main__2
   LDA   #<__local_main__l31
   STA   __zpabi_f7__pp9_0
   LDA   #>__local_main__l31
   STA   __zpabi_f7__pp9_1
   LDX   #$00
   LDA   __local_main__2
   JSR   f7
   LDA   z3
   SEC
   SBC   #$FF
   STA   __local_main__1
   STA   z3
   BMI   .sx_neg@39
   LDA   #$00
   BEQ   .sx_done@40
.sx_neg@39:
   LDA   #$FF
.sx_done@40:
   ORA   __local_main__1
   BEQ   .if_end@11
   LDA   __local_main__i32
   EOR   #$FF
   STA   __local_main__4
   LDY   #$00
   LDA   (gp6),Y
   STA   __local_main__3
   BMI   .sx_neg@41
   LDA   #$00
   BEQ   .sx_done@42
.sx_neg@41:
   LDA   #$FF
.sx_done@42:
   STA   __local_main__2
   LDA   g2
   BMI   .sx_neg@43
   LDA   #$00
   BEQ   .sx_done@44
.sx_neg@43:
   LDA   #$FF
.sx_done@44:
   STA   __local_main__0
   LDA   __local_main__3
   AND   g2
   STA   __zpabi_f14__p17_0
   LDA   __local_main__2
   AND   __local_main__0
   STA   __zpabi_f14__p17_1
   LDA   __local_main__4
   STA   __zpabi_f14__p16
   LDX   #$00
   LDA   #$01
   JSR   f14
   JMP   .loop@10_break
.if_end@11:
.if_end@10:
   INC   __local_main__i32
   JMP   .loop@10_start
.loop@10_break:
   JSR   fin0
   JSR   fin1
   JSR   fin2
   LDA   acc+1
   AND   #$7F
   TAX
   LDA   acc
   RTS

acc:
   DS.B  2

ga4:
   DC.B  $7F
   DS.B  6
   DC.B  $02
   DS.B  7
   DC.B  $01
   DS.B  5
   DC.B  $03
   DS.B  6

gs5:
   DS.B  1
   DC.L  $00000001
   DC.W  $FFFF

@3.s12:
   DC.W  $7FFF

@17.s28:
   DS.B  2

__zp_init:
   SUBROUTINE

   LDA   #$00
   STA   g1
   LDA   #$80
   STA   g1+1
   LDA   #$01
   STA   g2
   LDA   #$FF
   STA   z3
   LDA   #<g2
   STA   gp6
   LDA   #>g2
   STA   gp6+1
   RTS
