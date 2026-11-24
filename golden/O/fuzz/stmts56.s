SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
z3	EQU	$26
z4	EQU	$27
z5	EQU	$29
gp8	EQU	$2D
g2	EQU	$2F
__zpabi_f16__p18_0	EQU	$80
__zpabi_f26__pp28_0	EQU	$80
__zpabi_f16__p18_1	EQU	$81
__zpabi_f26__pp28_1	EQU	$81
__local_main__l35	EQU	$82
__zpabi_f16__p18_2	EQU	$82
__local_main__0	EQU	$83
__zpabi_f16__p18_3	EQU	$83
__zpabi_f16__p18_4	EQU	$84
__zpabi_f16__p18_5	EQU	$85
__local_main__l37	EQU	$86
__zpabi_f16__p19_0	EQU	$86
__zpabi_f16__p19_1	EQU	$87
__zpabi_f16__p19_2	EQU	$88
__zpabi_f16__p19_3	EQU	$89
__local_f26__l30	EQU	$8A
__zpabi_f16__pp20_0	EQU	$8A
__local_f26__w33	EQU	$8B
__zpabi_f16__pp20_1	EQU	$8B
__local_f26__0	EQU	$8C
__zpabi_f9__p10_0	EQU	$8C
__local_f26__1	EQU	$8D
__zpabi_f9__p10_1	EQU	$8D
__local_f26__p27	EQU	$8E
__zpabi_f9__p10_2	EQU	$8E
__zpabi_f9__p10_3	EQU	$8F
__zpabi_f9__p11_0	EQU	$90
__zpabi_f9__p11_1	EQU	$91
__zpabi_f9__p11_2	EQU	$92
__zpabi_f9__p11_3	EQU	$93
__zpabi_f9__p11_4	EQU	$94
__zpabi_f9__p11_5	EQU	$95
__zpabi_mix__v_0	EQU	$96
__zpabi_mix__v_1	EQU	$97
__zpabi_mix__v_2	EQU	$98
__zpabi_mix__v_3	EQU	$99
__local_f16__l21	EQU	$9A
__local_f16__0	EQU	$9B
__local_f16__p17_1	EQU	$9C
__local_f16__p17_0	EQU	$9D
__local_f9__i14	EQU	$A6
__local_mix__0	EQU	$A7
__local_mix__1	EQU	$A8
__local_mix__2	EQU	$A9
__local_mix__3	EQU	$AA

; @zp-link-meta-begin
; def f16 params=__zpabi_f16__p17_0,__zpabi_f16__p17_1,__zpabi_f16__p18_0,__zpabi_f16__p18_1,__zpabi_f16__p18_2,__zpabi_f16__p18_3,__zpabi_f16__p18_4,__zpabi_f16__p18_5,__zpabi_f16__p19_0,__zpabi_f16__p19_1,__zpabi_f16__p19_2,__zpabi_f16__p19_3,__zpabi_f16__pp20_0,__zpabi_f16__pp20_1 locals=__local_f16__l21,__local_f16__0,__local_f16__p17_1,__local_f16__p17_0,__local_f16__1,__local_f16__2 indirect=false in_cycle=false param_regs=A,X,,,,,,,,,,,,
; def f26 params=__zpabi_f26__p27,__zpabi_f26__pp28_0,__zpabi_f26__pp28_1 locals=__local_f26__l30,__local_f26__w33,__local_f26__0,__local_f26__1,__local_f26__p27 indirect=false in_cycle=false param_regs=A,,
; def f9 params=__zpabi_f9__p10_0,__zpabi_f9__p10_1,__zpabi_f9__p10_2,__zpabi_f9__p10_3,__zpabi_f9__p11_0,__zpabi_f9__p11_1,__zpabi_f9__p11_2,__zpabi_f9__p11_3,__zpabi_f9__p11_4,__zpabi_f9__p11_5 locals=__local_f9__p11_5,__local_f9__p11_4,__local_f9__w15,__local_f9__p11_2,__local_f9__p11_1,__local_f9__p11_0,__local_f9__i14 indirect=false in_cycle=false changes=__zpabi_f9__p11_0,__zpabi_f9__p11_1,__zpabi_f9__p11_2,__zpabi_f9__p11_3,__zpabi_f9__p11_4,__zpabi_f9__p11_5
; def fin0 params= locals=__local_fin0__0,__local_fin0__1 indirect=false in_cycle=false
; def fin1 params= locals=__local_fin1__0 indirect=false in_cycle=false
; def fin2 params= locals=__local_fin2__0,__local_fin2__1 indirect=false in_cycle=false
; def main params= locals=__local_main__l35,__local_main__0,__local_main__1,__local_main__2,__local_main__l37,__local_main__4,__local_main__5,__local_main__6 indirect=false in_cycle=false
; def mix params=__zpabi_mix__v_0,__zpabi_mix__v_1,__zpabi_mix__v_2,__zpabi_mix__v_3 locals=__local_mix__0,__local_mix__1,__local_mix__2,__local_mix__3 indirect=false in_cycle=false
; call f16 -> f9
; call f26 -> mix
; call f9 -> mix
; call fin0 -> mix
; call fin1 -> mix
; call fin2 -> mix
; call main -> f26
; call main -> fin0
; call main -> fin1
; call main -> fin2
; call main -> mix
; item mix section=.text local=false zero=false
; item fin0 section=.text local=false zero=false
; item fin0@run3 section=.text local=true zero=false
; item fin0@run2 section=.text local=true zero=false
; item fin0@run1 section=.text local=true zero=false
; item fin0@run0 section=.text local=true zero=false
; item fin0@tail0 section=.text local=true zero=false
; item fin1 section=.text local=false zero=false
; item fin2 section=.text local=false zero=false
; item f9 section=.text local=false zero=false
; item f16 section=.text local=false zero=false
; item f26 section=.text local=false zero=false
; item main section=.text local=false zero=false
; item acc section=.bss local=false zero=true
; item g1 section=.bss local=false zero=true
; item ga6 section=.data local=false zero=false
; item gs7 section=.data local=false zero=false
; item __zp_init section=.text local=false zero=false
; zp z3 size=1 at=auto def=true init=$80
; zp z4 size=2 at=auto def=true init=$00,$80
; zp z5 size=4 at=auto def=true init=$80,$00,$00,$00
; zp gp8 size=2 at=auto def=true init=<g2,>g2 weight=352
; zp g2 size=2 at=auto def=true init=$03,$00 weight=21
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
   BMI   .sx_neg@0
   LDA   #$00
   BEQ   .sx_done@1
.sx_neg@0:
   LDA   #$FF
.sx_done@1:
   STA   __zpabi_mix__v_2
   JSR   fin0@run3
   LDA   g2
   STA   __zpabi_mix__v_0
   LDA   g2+1
   JSR   fin0@run1
   JSR   mix
   LDA   z3
   BMI   .sx_neg@2
   LDA   #$00
   BEQ   .sx_done@3
.sx_neg@2:
   LDA   #$FF
.sx_done@3:
   JSR   fin0@run0
   LDA   z3
   JSR   fin0@run2
   LDA   z4
   STA   __zpabi_mix__v_0
   LDA   z4+1
   JMP   fin0@tail0

fin0@run3:
   SUBROUTINE

   STA   __zpabi_mix__v_3
   JMP   mix

fin0@run2:
   SUBROUTINE

   STA   __zpabi_mix__v_0
   JMP   mix

fin0@run1:
   SUBROUTINE

   STA   __zpabi_mix__v_1
   LDA   #$00
   STA   __zpabi_mix__v_2
   STA   __zpabi_mix__v_3
   RTS

fin0@run0:
   SUBROUTINE

   STA   __zpabi_mix__v_1
   STA   __zpabi_mix__v_2
   STA   __zpabi_mix__v_3
   RTS

fin0@tail0:
   SUBROUTINE

   JSR   fin0@run1
   JMP   mix

fin1:
   SUBROUTINE

   LDA   z5
   STA   __zpabi_mix__v_0
   LDA   z5+1
   STA   __zpabi_mix__v_1
   LDA   z5+2
   STA   __zpabi_mix__v_2
   LDA   z5+3
   JSR   fin0@run3
   LDA   ga6
   BMI   .sx_neg@4
   LDA   #$00
   BEQ   .sx_done@5
.sx_neg@4:
   LDA   #$FF
.sx_done@5:
   JSR   fin0@run0
   LDA   ga6
   JSR   fin0@run2
   LDA   ga6+1
   BMI   .sx_neg@6
   LDA   #$00
   BEQ   .sx_done@7
.sx_neg@6:
   LDA   #$FF
.sx_done@7:
   JSR   fin0@run0
   LDA   ga6+1
   JSR   fin0@run2
   LDA   ga6+2
   BMI   .sx_neg@8
   LDA   #$00
   BEQ   .sx_done@9
.sx_neg@8:
   LDA   #$FF
.sx_done@9:
   JSR   fin0@run0
   LDA   ga6+2
   STA   __zpabi_mix__v_0
   JMP   mix

fin2:
   SUBROUTINE

   LDA   ga6+3
   BMI   .sx_neg@10
   LDA   #$00
   BEQ   .sx_done@11
.sx_neg@10:
   LDA   #$FF
.sx_done@11:
   JSR   fin0@run0
   LDA   ga6+3
   JSR   fin0@run2
   LDA   gs7+2
   BMI   .sx_neg@12
   LDA   #$00
   BEQ   .sx_done@13
.sx_neg@12:
   LDA   #$FF
.sx_done@13:
   STA   __zpabi_mix__v_2
   STA   __zpabi_mix__v_3
   LDA   gs7+1
   STA   __zpabi_mix__v_0
   LDA   gs7+2
   STA   __zpabi_mix__v_1
   JSR   mix
   LDA   gs7+3
   STA   __zpabi_mix__v_0
   LDA   gs7+4
   JMP   fin0@tail0

f9:
   SUBROUTINE

   LDA   #$02
   STA   __local_f9__i14
.loop@0_start:
   LDA   __local_f9__i14
   BEQ   .loop@0_break
   LDA   __zpabi_f9__p11_0
   STA   HARGS
   LDA   __zpabi_f9__p11_1
   STA   HARGS+1
   LDA   __zpabi_f9__p11_2
   STA   HARGS+2
   LDA   __zpabi_f9__p11_3
   STA   HARGS+3
   LDA   __zpabi_f9__p11_4
   STA   HARGS+4
   LDA   __zpabi_f9__p11_5
   STA   HARGS+5
   LDA   #$00
   STA   HARGS+6
   STA   HARGS+7
   LDY   #$05
   STY   HARGS+8
   LDY   #$00
   STY   HARGS+9
   STY   HARGS+10
   STY   HARGS+11
   STY   HARGS+12
   STY   HARGS+13
   STA   HARGS+14
   STA   HARGS+15
   JSR   mul64
   LDA   HARGS+16
   STA   __zpabi_f9__p11_0
   LDA   HARGS+17
   STA   __zpabi_f9__p11_1
   LDA   HARGS+18
   STA   __zpabi_f9__p11_2
   LDA   HARGS+19
   STA   __zpabi_f9__p11_3
   LDA   HARGS+20
   STA   __zpabi_f9__p11_4
   LDA   HARGS+21
   STA   __zpabi_f9__p11_5
   DEC   z3
   LDA   #$00
   STA   z5
   STA   z5+1
   STA   z5+2
   LDA   #$20
   STA   z5+3
   DEC   __local_f9__i14
   JMP   .loop@0_start
.loop@0_break:
   INC   z5
   BNE   .inc_done@0
   INC   z5+1
   BNE   .inc_done@0
   INC   z5+2
   BNE   .inc_done@0
   INC   z5+3
.inc_done@0:
   LDA   g1
   STA   __zpabi_f9__p11_5
   LDA   g1+1
   LDA   __zpabi_f9__p11_5
   AND   #$07
   CMP   #$01
   BEQ   .case@2
   CMP   #$02
   BEQ   .case@3
   JMP   .default@9
.case@2:
   LDA   z4
   STA   __zpabi_f9__p11_4
   LDA   z4+1
   STA   __zpabi_f9__p11_2
   LSR
   LDA   __zpabi_f9__p11_4
   ROR
   STA   __zpabi_f9__p11_3
   LDA   #$00
   ROR
   CLC
   ADC   __zpabi_f9__p11_4
   STA   __zpabi_f9__p11_4
   LDA   __zpabi_f9__p11_3
   ADC   __zpabi_f9__p11_2
   STA   __zpabi_f9__p11_5
   LDA   __zpabi_f9__p11_4
   STA   z4
   LDA   __zpabi_f9__p11_5
   STA   z4+1
   STA   gs7+2
   LDA   __zpabi_f9__p11_4
   STA   gs7+1
   LDA   ga6+2
   BEQ   .cond_else@0
   LDA   g1
   STA   __zpabi_f9__p11_4
   LDA   g1+1
   STA   __zpabi_f9__p11_5
   JMP   .cond_end@1
.cond_else@0:
   LDA   #$7F
   STA   __zpabi_f9__p11_4
   LDA   #$00
   STA   __zpabi_f9__p11_5
.cond_end@1:
   ORA   #$00
   BMI   .sx_neg@14
   LDA   #$00
   BEQ   .sx_done@15
.sx_neg@14:
   LDA   #$FF
.sx_done@15:
   STA   __zpabi_mix__v_2
   STA   __zpabi_mix__v_3
   LDA   __zpabi_f9__p11_4
   STA   __zpabi_mix__v_0
   LDA   __zpabi_f9__p11_5
   STA   __zpabi_mix__v_1
   JSR   mix
.case@3:
   LDA   z3
   AND   #$07
   BEQ   .case@5
   CMP   #$05
   BEQ   .case@7
   CMP   #$06
   BEQ   .case@8
   BNE   .switch@4_break
.case@5:
   LDA   #$00
   STA   g2
   STA   g2+1
   STA   __zpabi_f9__p11_3
.loop@6_start:
   LDY   #$00
   LDA   (gp8),Y
   ORA   #$05
   STA   (gp8),Y
   INC   __zpabi_f9__p11_3
   LDA   __zpabi_f9__p11_3
   CMP   #$03
   BCC   .loop@6_start
.case@7:
.case@8:
.switch@4_break:
.default@9:
   LDA   z4
   STA   __zpabi_f9__p11_5
   LDA   z4+1
   LDA   __zpabi_f9__p11_5
   RTS

f16:
   SUBROUTINE

   STA   __local_f16__p17_0
   STX   __local_f16__p17_1
   LDA   #$00
   STA   __local_f16__l21
   LDA   #$01
   STA   __local_f16__0
   ASL   g2
   ROL   g2+1
   ASL   g2
   ROL   g2+1
   LDA   g2
   STA   z4
   LDA   g2+1
   STA   z4+1
   LDA   ga6
   BMI   .sx_neg@16
   LDA   #$00
   BEQ   .sx_done@17
.sx_neg@16:
   LDA   #$FF
.sx_done@17:
   STA   __local_f16__l21
   LDA   ga6
   SEC
   SBC   __local_f16__p17_0
   STA   __zpabi_f9__p10_0
   LDA   __local_f16__l21
   SBC   __local_f16__p17_1
   STA   __zpabi_f9__p10_1
   LDA   #$02
   STA   __zpabi_f9__p11_0
   LDA   #$00
   STA   __zpabi_f9__p10_2
   STA   __zpabi_f9__p10_3
   STA   __zpabi_f9__p11_1
   STA   __zpabi_f9__p11_2
   STA   __zpabi_f9__p11_3
   STA   __zpabi_f9__p11_4
   STA   __zpabi_f9__p11_5
   JSR   f9
   LDA   z3
   EOR   #$81
   STA   __local_f16__l21
   STA   z3
   LDA   __zpabi_f16__p18_0
   EOR   __local_f16__l21
   RTS

f26:
   SUBROUTINE

   STA   __local_f26__p27
   LDA   #$00
   STA   __local_f26__l30
   STA   __local_f26__w33
   STA   __local_f26__l30
   STA   __local_f26__w33
.loop@20_continue:
   CLC
   ADC   #$01
   STA   __local_f26__1
   LDA   __local_f26__w33
   CMP   #$04
   BCC   .lb_skip@0
   JMP   .loop@20_break
.lb_skip@0:
   INC   __local_f26__p27
   LDA   __local_f26__p27
   CMP   #$01
   BEQ   .if_else@3
   LDA   g2+1
   BNE   .cmp_true@22
   LDA   g2
   CMP   __local_f26__p27
   BNE   .cmp_true@22
   LDA   #$00
   BEQ   .cmp_end@23
.cmp_true@22:
   LDA   #$01
.cmp_end@23:
   STA   gs7+1
   LDA   #$00
   STA   gs7+2
   LDA   z4
   STA   __local_f26__l30
   LDA   z4+1
   LDA   __local_f26__1
   SEC
   SBC   __local_f26__l30
   RTS
.if_else@3:
   LDA   gs7+3
   STA   __local_f26__0
   LDA   gs7+4
   STA   __local_f26__w33
   LDA   __local_f26__0
   SEC
   SBC   #$01
   STA   gs7+3
   LDA   __local_f26__w33
   SBC   #$00
   STA   gs7+4
   LDA   __local_f26__0
   ORA   __local_f26__w33
   BEQ   .if_end@4
   LDA   __local_f26__l30
   BMI   .sx_neg@25
   LDA   #$00
   BEQ   .sx_done@26
.sx_neg@25:
   LDA   #$FF
.sx_done@26:
   JSR   fin0@run0
   LDA   __local_f26__l30
   JSR   fin0@run2
   LDA   z5
   SEC
   SBC   #$01
   STA   z5
   LDA   z5+1
   SBC   #$00
   STA   z5+1
   LDA   z5+2
   SBC   #$00
   STA   z5+2
   BCS   .carry_inc@0
   DEC   z5+3
.carry_inc@0:
   LDA   __local_f26__l30
   AND   #$07
   CMP   #$03
   BNE   .default@24
   LDA   __local_f26__l30
   ASL
   ASL
   CLC
   ADC   __local_f26__l30
   STA   __local_f26__l30
   STA   g2
   BMI   .sx_neg@27
   LDA   #$00
   BEQ   .sx_done@28
.sx_neg@27:
   LDA   #$FF
.sx_done@28:
   STA   g2+1
.default@24:
   LDA   __local_f26__1
   STA   __zpabi_mix__v_0
   LDA   #$00
   STA   __zpabi_mix__v_1
   STA   __zpabi_mix__v_2
   JSR   fin0@run3
.if_end@4:
   LDA   __local_f26__1
   STA   __local_f26__w33
   JMP   .loop@20_continue
.loop@20_break:
   LDA   gs7+3
   RTS

main:
   SUBROUTINE

   LDA   #$00
   STA   __local_main__l35
   STA   __local_main__0
   LDA   #$02
   STA   __local_main__l37
   LDA   #$00
   STA   __local_main__l37+1
   STA   __local_main__l37+2
   STA   __local_main__l37+3
   LDA   z4
   LDX   z4+1
   STX   __local_main__0
   ORA   #$01
   STA   __local_main__l35
   STA   z4
   LDA   __local_main__0
   STA   z4+1
   LDA   ga6+1
   EOR   __local_main__l35
   AND   #$07
   CMP   #$01
   BEQ   .switch@25_break
   CMP   #$04
   BEQ   .case@27
   CMP   #$06
   BEQ   .case@28
   BNE   .switch@25_break
.case@27:
   LDA   z5
   CLC
   ADC   #$FF
   LDX   #<__local_main__l37
   STX   __zpabi_f26__pp28_0
   LDX   #>__local_main__l37
   STX   __zpabi_f26__pp28_1
   JSR   f26
.case@28:
.switch@25_break:
   LDA   z3
   BMI   .sx_neg@31
   LDA   #$00
   BEQ   .sx_done@32
.sx_neg@31:
   LDA   #$FF
.sx_done@32:
   LDX   z3
   STX   z4
   STA   z4+1
   LDA   gs7+3
   LDX   gs7+4
   STX   __local_main__0
   CLC
   ADC   #$FF
   STA   __local_main__l35
   BCC   .carry_inc@1
   INC   __local_main__0
.carry_inc@1:
   STA   gs7+3
   LDA   __local_main__0
   STA   gs7+4
   LDA   g2
   AND   __local_main__l35
   STA   __zpabi_mix__v_0
   LDA   g2+1
   AND   __local_main__0
   JSR   fin0@run1
   JSR   mix
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

g1:
   DS.B  2

ga6:
   DC.B  $FF, $FF, $01, $00

gs7:
   DS.B  1
   DC.W  $FB70
   DC.W  $000C

__zp_init:
   SUBROUTINE

   LDA   #$00
   STA   g2+1
   STA   z4
   STA   z5+1
   STA   z5+2
   STA   z5+3
   LDA   #$03
   STA   g2
   LDA   #$80
   STA   z3
   STA   z4+1
   STA   z5
   LDA   #<g2
   STA   gp8
   LDA   #>g2
   STA   gp8+1
   RTS
