SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
g2	EQU	$26
g1	EQU	$28
gp5	EQU	$29
@15.s26	EQU	$2B
__zpabi_f21__p22_0	EQU	$80
__zpabi_f21__p22_1	EQU	$81
__zpabi_f21__p22_2	EQU	$82
__zpabi_f21__p22_3	EQU	$83
__zpabi_f6__p7_0	EQU	$84
__zpabi_f6__p7_1	EQU	$85
__zpabi_f6__p7_2	EQU	$86
__zpabi_f6__p7_3	EQU	$87
__zpabi_f6__p9_0	EQU	$88
__zpabi_f6__p9_1	EQU	$89
__zpabi_f6__p9_2	EQU	$8A
__zpabi_f6__p9_3	EQU	$8B
__zpabi_f6__pp10_0	EQU	$8C
__zpabi_f6__pp10_1	EQU	$8D
__zpabi_f6__p8	EQU	$8E
__zpabi_mix__v_0	EQU	$8F
__zpabi_mix__v_1	EQU	$90
__zpabi_mix__v_2	EQU	$91
__zpabi_mix__v_3	EQU	$92
__local_f14__w19	EQU	$93
__local_f21__0	EQU	$93
__local_main__w30	EQU	$93
__local_f14__0	EQU	$94
__local_f21__1	EQU	$94
__local_main__0	EQU	$94
__local_f14__1	EQU	$95
__local_f21__2	EQU	$95
__local_f14__2	EQU	$96
__local_f14__3	EQU	$97
__local_f21__4	EQU	$97
__local_main__3	EQU	$97
__local_f14__4	EQU	$98
__local_f21__5	EQU	$98
__local_main__4	EQU	$98
__local_f14__l15	EQU	$99
__local_f21__6	EQU	$99
__local_main__5	EQU	$99
__local_f21__w27	EQU	$9A
__local_main__w32	EQU	$9A
__local_f21__l24	EQU	$9B
__local_main__6	EQU	$9B
__local_main__w31	EQU	$9C
__local_main__7	EQU	$9D
__local_main__l29	EQU	$9E
__local_f6__0	EQU	$A2
__local_f6__1	EQU	$A3
__local_f6__2	EQU	$A4
__local_f6__l11	EQU	$A7
__local_mix__0	EQU	$A8
__local_mix__1	EQU	$A9
__local_mix__2	EQU	$AA
__local_mix__3	EQU	$AB

; @zp-link-meta-begin
; def f14 params= locals=__local_f14__w19,__local_f14__0,__local_f14__1,__local_f14__2,__local_f14__3,__local_f14__4,__local_f14__l15,__local_f14__6 indirect=false in_cycle=false
; def f21 params=__zpabi_f21__p22_0,__zpabi_f21__p22_1,__zpabi_f21__p22_2,__zpabi_f21__p22_3 locals=__local_f21__0,__local_f21__1,__local_f21__2,__local_f21__3,__local_f21__4,__local_f21__5,__local_f21__6,__local_f21__w27,__local_f21__l24 indirect=false in_cycle=false
; def f6 params=__zpabi_f6__p7_0,__zpabi_f6__p7_1,__zpabi_f6__p7_2,__zpabi_f6__p7_3,__zpabi_f6__p8,__zpabi_f6__p9_0,__zpabi_f6__p9_1,__zpabi_f6__p9_2,__zpabi_f6__p9_3,__zpabi_f6__pp10_0,__zpabi_f6__pp10_1 locals=__local_f6__0,__local_f6__1,__local_f6__2,__local_f6__3,__local_f6__i13,__local_f6__l11 indirect=false in_cycle=false
; def fin0 params= locals=__local_fin0__0,__local_fin0__1 indirect=false in_cycle=false
; def fin1 params= locals=__local_fin1__0,__local_fin1__1,__local_fin1__2,__local_fin1__3 indirect=false in_cycle=false
; def main params= locals=__local_main__w30,__local_main__0,__local_main__1,__local_main__2,__local_main__3,__local_main__4,__local_main__5,__local_main__w32,__local_main__6,__local_main__w31,__local_main__7,__local_main__l29,__local_main__9,__local_main__10,__local_main__11 indirect=false in_cycle=false
; def mix params=__zpabi_mix__v_0,__zpabi_mix__v_1,__zpabi_mix__v_2,__zpabi_mix__v_3 locals=__local_mix__0,__local_mix__1,__local_mix__2,__local_mix__3 indirect=false in_cycle=false
; call f14 -> f6
; call f14 -> mix
; call f21 -> f6
; call f6 -> mix
; call fin0 -> mix
; call fin1 -> mix
; call main -> f6
; call main -> fin0
; call main -> fin1
; call main -> mix
; share member=f6:__zpabi_f6__p8 holder=f21 live=f21:f6 forward=f21>f6 writer=f21
; item mix section=.text local=false zero=false
; item fin0 section=.text local=false zero=false
; item fin0@run5 section=.text local=true zero=false
; item fin0@run2 section=.text local=true zero=false
; item fin0@run1 section=.text local=true zero=false
; item fin0@run0 section=.text local=true zero=false
; item fin1 section=.text local=false zero=false
; item fin1@run3 section=.text local=true zero=false
; item f6 section=.text local=false zero=false
; item f14 section=.text local=false zero=false
; item f14@run4 section=.text local=true zero=false
; item f21 section=.text local=false zero=false
; item main section=.text local=false zero=false
; item acc section=.bss local=false zero=true
; item ga3 section=.data local=false zero=false
; item gs4 section=.data local=false zero=false
; item __zp_init section=.text local=false zero=false
; zp g2 size=2 at=auto def=true init=$FF,$FF weight=297
; zp g1 size=1 at=auto def=true init=$00 weight=107
; zp gp5 size=2 at=auto def=true init=<g2,>g2 weight=40
; zp @15.s26 size=2 at=auto def=true init=$0D,$00 weight=16 local=true
; runtime hargs_size=32 hargs_need=10
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
   JSR   fin0@run1
   LDA   g2+1
   BMI   .sx_neg@0
   LDA   #$00
   BEQ   .sx_done@1
.sx_neg@0:
   LDA   #$FF
.sx_done@1:
   STA   __zpabi_mix__v_2
   STA   __zpabi_mix__v_3
   LDA   g2
   STA   __zpabi_mix__v_0
   LDA   g2+1
   JSR   fin0@run2
   LDA   ga3
   JSR   fin0@run1
   LDA   ga3+1
   STA   __zpabi_mix__v_0
   JSR   fin0@run5
   JMP   mix

fin0@run5:
   SUBROUTINE

   LDA   #$00
   STA   __zpabi_mix__v_1
   STA   __zpabi_mix__v_2
   STA   __zpabi_mix__v_3
   RTS

fin0@run2:
   SUBROUTINE

   STA   __zpabi_mix__v_1
   JMP   mix

fin0@run1:
   SUBROUTINE

   STA   __zpabi_mix__v_0
   JMP   fin0@run0

fin0@run0:
   SUBROUTINE

   JSR   fin0@run5
   JMP   mix

fin1:
   SUBROUTINE

   LDA   ga3+2
   JSR   fin0@run1
   LDA   ga3+3
   JSR   fin0@run1
   LDA   gs4+1
   STA   __zpabi_mix__v_0
   LDA   gs4+2
   STA   __zpabi_mix__v_1
   LDA   gs4+3
   STA   __zpabi_mix__v_2
   LDA   gs4+4
   JSR   fin1@run3
   LDA   gs4+9
   STA   __zpabi_mix__v_0
   LDA   gs4+10
   STA   __zpabi_mix__v_1
   LDA   gs4+11
   STA   __zpabi_mix__v_2
   LDA   gs4+12
   STA   __zpabi_mix__v_3
   JMP   mix

fin1@run3:
   SUBROUTINE

   STA   __zpabi_mix__v_3
   JMP   mix

f6:
   SUBROUTINE

   LDA   #$00
   STA   __local_f6__l11
   LDY   #$00
   LDA   (__zpabi_f6__pp10_0),Y
   JSR   fin0@run1
   LDA   __local_f6__l11
   SEC
   SBC   #$01
   STA   __local_f6__l11
   LDA   __local_f6__l11
   STA   __local_f6__1
   SEC
   SBC   #$01
   STA   __local_f6__l11
   LDA   __local_f6__1
   AND   #$07
   BEQ   .case@1
   CMP   #$03
   BEQ   .case@2
   BNE   .default@3
.case@1:
.case@2:
   LDA   #$80
   STA   gs4+1
   LDA   #$00
   STA   gs4+2
   LDY   #$00
   STY   gs4+3
   STY   gs4+4
   STY   gs4+5
   STY   gs4+6
   STY   gs4+7
   STA   gs4+8
   JMP   .switch@0_break
.default@3:
   LDX   #$03
.f6@rot@0:
   LDA   gs4+9
   LDY   gs4+10
   STY   __local_f6__2
   LDY   gs4+11
   STY   __local_f6__1
   LDY   gs4+12
   STY   __local_f6__0
   ASL
   STA   gs4+9
   LDA   __local_f6__2
   ROL
   STA   gs4+10
   LDA   __local_f6__1
   ROL
   STA   gs4+11
   LDA   __local_f6__0
   ROL
   ASL   gs4+9
   ROL   gs4+10
   ROL   gs4+11
   ROL
   STA   gs4+12
   DEX
   BNE   .f6@rot@0
.switch@0_break:
   LDA   __local_f6__l11
   STA   __local_f6__1
   ASL
   ASL
   CLC
   ADC   __local_f6__1
   STA   __local_f6__l11
   STA   gs4+9
   LDA   #$00
   STA   gs4+10
   LDY   #$00
   STY   gs4+11
   STA   gs4+12
   LDA   gs4+1
   LDX   gs4+2
   RTS

f14:
   SUBROUTINE

   LDA   #$FF
   STA   __local_f14__l15
   LDA   ga3+2
   STA   __local_f14__w19
   LSR
   LSR
   LSR
   LSR
   LSR
   STA   __local_f14__2
   LDA   __local_f14__w19
   ASL
   ASL
   ASL
   STA   __local_f14__1
   LDA   __local_f14__2
   BMI   .sx_neg@2
   LDA   #$00
   BEQ   .sx_done@3
.sx_neg@2:
   LDA   #$FF
.sx_done@3:
   STA   __zpabi_f6__p7_2
   STA   __zpabi_f6__p7_3
   LDA   __local_f14__1
   STA   __zpabi_f6__p7_0
   LDA   __local_f14__2
   STA   __zpabi_f6__p7_1
   LDA   #$02
   STA   __zpabi_f6__p8
   LDA   #$80
   STA   __zpabi_f6__p9_0
   LDA   #$00
   STA   __zpabi_f6__p9_1
   STA   __zpabi_f6__p9_2
   STA   __zpabi_f6__p9_3
   LDA   #<__local_f14__l15
   STA   __zpabi_f6__pp10_0
   LDA   #>__local_f14__l15
   JSR   f14@run4
   LDA   #$00
   STA   __local_f14__w19
.loop@5_continue:
   CLC
   ADC   #$01
   STA   __local_f14__4
   LDA   __local_f14__w19
   BEQ   .lb_skip@1
   JMP   .loop@5_break
.lb_skip@1:
   LDA   __local_f14__l15
   EOR   #$01
   STA   __local_f14__l15
   BNE   .lb_skip@0
   JMP   .if_else@1
.lb_skip@0:
   LDA   gs4+9
   STA   __local_f14__3
   LDA   gs4+10
   STA   __local_f14__2
   LDA   gs4+11
   STA   __local_f14__1
   LDA   gs4+12
   STA   __local_f14__0
   LDA   __local_f14__3
   CLC
   ADC   #$01
   STA   gs4+9
   LDA   __local_f14__2
   ADC   #$00
   STA   gs4+10
   LDA   __local_f14__1
   ADC   #$00
   STA   gs4+11
   LDA   __local_f14__0
   ADC   #$00
   STA   gs4+12
   LDA   #$01
   AND   __local_f14__4
   STA   __local_f14__w19
   LDA   __local_f14__3
   SEC
   SBC   __local_f14__w19
   STA   __zpabi_mix__v_0
   LDA   __local_f14__2
   SBC   #$00
   STA   __zpabi_mix__v_1
   LDA   __local_f14__1
   SBC   #$00
   STA   __zpabi_mix__v_2
   LDA   __local_f14__0
   SBC   #$00
   JSR   fin1@run3
   LDY   #$00
   LDA   (gp5),Y
   STA   HARGS
   INY
   LDA   (gp5),Y
   STA   HARGS+1
   LDA   #$00
   STA   HARGS+2
   STA   HARGS+3
   JSR   mul16
   LDA   HARGS+4
   STA   g2
   LDA   HARGS+5
   STA   g2+1
   JMP   .if_end@0
.if_else@1:
   LDA   #$01
   STA   __zpabi_mix__v_0
   JSR   fin0@run0
.if_end@0:
   LDA   __local_f14__l15
   STA   __local_f14__0
   LDA   __local_f14__4
   AND   #$03
   STA   __local_f14__1
   LDX   __local_f14__1
   LDA   ga3,X
   STA   HARGS
   LDA   #$00
   LDX   #$08
   LSR   HARGS
.mul_loop@4:
   BCC   .mul_skip@5
   CLC
   ADC   __local_f14__0
.mul_skip@5:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@4
   STA   __local_f14__0
   ORA   #$00
   BMI   .sx_neg@6
   LDA   #$00
   BEQ   .sx_done@7
.sx_neg@6:
   LDA   #$FF
.sx_done@7:
   STA   __zpabi_mix__v_2
   STA   __zpabi_mix__v_3
   LDA   HARGS
   STA   __zpabi_mix__v_0
   LDA   __local_f14__0
   JSR   fin0@run2
   LDX   __local_f14__1
   LDA   g1
   STA   ga3,X
   LDA   __local_f14__l15
   ORA   #$81
   STA   __local_f14__l15
   LDA   __local_f14__4
   STA   __local_f14__w19
   JMP   .loop@5_continue
.loop@5_break:
   LDA   g1
   RTS

f14@run4:
   SUBROUTINE

   STA   __zpabi_f6__pp10_1
   JMP   f6

f21:
   SUBROUTINE

   LDA   #$01
   STA   __local_f21__l24
   LDA   __zpabi_f21__p22_0
   STA   __zpabi_f6__p8
   LDA   #$00
   STA   __local_f21__w27
.loop@7_start:
   LDA   #$00
   BPL   .sx_done@9
   LDA   #$FF
.sx_done@9:
   STA   __local_f21__6
   STA   __local_f21__5
   LDA   gs4+9
   LDX   gs4+10
   STX   __local_f21__4
   LDX   gs4+11
   STX   __local_f21__1
   LDX   gs4+12
   STX   __zpabi_f6__p7_0
   CLC
   ADC   #$03
   STA   __local_f21__0
   BCC   .carry_inc@0
   INC   __local_f21__4
   BNE   .carry_inc@0
   INC   __local_f21__1
   BNE   .carry_inc@0
   INC   __zpabi_f6__p7_0
.carry_inc@0:
   STA   gs4+9
   LDA   __local_f21__4
   STA   gs4+10
   LDA   __local_f21__1
   STA   gs4+11
   LDA   __zpabi_f6__p7_0
   STA   gs4+12
   LDA   g1
   ORA   __local_f21__0
   STA   __local_f21__2
   LDA   __local_f21__6
   ORA   __local_f21__1
   STA   __local_f21__1
   LDA   __local_f21__5
   ORA   __zpabi_f6__p7_0
   STA   __local_f21__0
   LDA   __local_f21__2
   ORA   __local_f21__4
   ORA   __local_f21__1
   ORA   __local_f21__0
   BEQ   .if_end@2
   LDA   __local_f21__w27
   BEQ   .and_false@3
   LDA   #$01
   STA   __zpabi_f6__p7_0
   LDA   #$00
   BEQ   .and_end@4
.and_false@3:
   LDA   #$00
   STA   __zpabi_f6__p7_0
.and_end@4:
   ORA   #$00
   BPL   .sx_done@11
   LDA   #$FF
.sx_done@11:
   STA   __zpabi_f6__p7_2
   STA   __zpabi_f6__p7_3
   LDA   #$00
   STA   __zpabi_f6__p7_1
   STA   __zpabi_f6__p9_0
   STA   __zpabi_f6__p9_1
   STA   __zpabi_f6__p9_2
   STA   __zpabi_f6__p9_3
   LDA   #<__local_f21__l24
   STA   __zpabi_f6__pp10_0
   LDA   #>__local_f21__l24
   JSR   f14@run4
   STA   __local_f21__l24
.if_end@2:
   LDA   gs4+9
   STA   __local_f21__1
   LDA   gs4+10
   STA   __local_f21__0
   LDA   @15.s26
   SEC
   SBC   #$01
   STA   @15.s26
   BCS   .carry_inc@1
   DEC   @15.s26+1
.carry_inc@1:
   LDA   __local_f21__1
   EOR   @15.s26
   STA   g2
   LDA   __local_f21__0
   EOR   @15.s26+1
   STA   g2+1
   LDA   __zpabi_f21__p22_0
   AND   #$07
   BEQ   .case@9
   CMP   #$02
   BNE   .lb_skip@3
   JMP   .switch@8_break
.lb_skip@3:
   CMP   #$06
   BEQ   .case@11
   CMP   #$07
   BEQ   .case@12
   JMP   .default@13
.case@9:
   INC   gs4+9
   BNE   .inc_done@0
   INC   gs4+10
   BNE   .inc_done@0
   INC   gs4+11
   BNE   .inc_done@0
   INC   gs4+12
.inc_done@0:
   LDY   #$00
   LDA   (gp5),Y
   EOR   #$81
   STA   (gp5),Y
   JMP   .switch@8_break
.case@11:
.case@12:
   LDA   gs4+9
   STA   __zpabi_f6__p7_0
   LDA   gs4+10
   STA   __local_f21__2
   LDA   gs4+11
   STA   __local_f21__1
   LDA   gs4+12
   STA   __local_f21__0
   LDA   __zpabi_f6__p7_0
   EOR   #$81
   STA   __zpabi_f6__p7_0
   STA   gs4+9
   LDA   __local_f21__w27
   EOR   __zpabi_f6__p7_0
   STA   __zpabi_f6__p7_0
   LDA   #$00
   EOR   __local_f21__2
   STA   __local_f21__2
   LDA   #$00
   EOR   __local_f21__1
   STA   __local_f21__1
   LDA   #$00
   EOR   __local_f21__0
   STA   __local_f21__0
   LDA   __zpabi_f6__p7_0
   ORA   __local_f21__2
   ORA   __local_f21__1
   ORA   __local_f21__0
   BNE   .switch@8_break
   LDY   #$00
   LDA   (gp5),Y
   STA   __local_f21__0
   INY
   LDA   (gp5),Y
   STA   __local_f21__1
   LDX   #$06
.shift_loop@12:
   ASL   __local_f21__0
   ROL   __local_f21__1
   DEX
   BNE   .shift_loop@12
   LDA   __local_f21__0
   LDY   #$00
   STA   (gp5),Y
   LDA   __local_f21__1
   INY
   STA   (gp5),Y
   LDA   __local_f21__w27
   AND   #$03
   TAX
   LDA   ga3,X
   STA   ga3,X
.default@13:
.switch@8_break:
   INC   __local_f21__w27
   BNE   .lb_skip@2
   JMP   .loop@7_start
.lb_skip@2:
   LDX   __zpabi_f21__p22_1
   LDA   __zpabi_f21__p22_0
   RTS

main:
   SUBROUTINE

   LDA   #$FF
   STA   __local_main__l29
   LDA   #$04
   STA   g1
   LDA   #$00
   STA   __local_main__w30
.loop@14_continue:
   CLC
   ADC   #$01
   STA   __local_main__7
   LDA   __local_main__w30
   CMP   #$02
   BCC   .lb_skip@5
   JMP   .loop@14_break
.lb_skip@5:
   LDA   #$00
   STA   __local_main__w31
.loop@15_start:
   LDA   #$00
   STA   gs4+9
   STA   gs4+10
   BPL   .sx_done@15
   LDA   #$FF
.sx_done@15:
   STA   gs4+11
   STA   gs4+12
   LDA   g1
   ADC   __local_main__w31
   STA   g2
   LDA   #$00
   ADC   #$00
   STA   g2+1
   LDA   gs4+1
   STA   __local_main__6
   LDA   gs4+2
   STA   __local_main__w32
   LDA   gs4+3
   STA   __local_main__5
   LDA   gs4+4
   STA   __local_main__4
   LDA   g2
   STA   HARGS
   STA   HARGS+1
   JSR   mul8
   LDA   #$04
   CLC
   ADC   __local_main__w31
   STA   __zpabi_f6__p9_0
   LDA   #$00
   ADC   #$00
   STA   __zpabi_f6__p9_1
   BMI   .sx_neg@16
   LDA   #$00
   BEQ   .sx_done@17
.sx_neg@16:
   LDA   #$FF
.sx_done@17:
   STA   __zpabi_f6__p9_2
   STA   __zpabi_f6__p9_3
   LDA   __local_main__6
   STA   __zpabi_f6__p7_0
   LDA   __local_main__w32
   STA   __zpabi_f6__p7_1
   LDA   __local_main__5
   STA   __zpabi_f6__p7_2
   LDA   __local_main__4
   STA   __zpabi_f6__p7_3
   LDA   HARGS+2
   STA   __zpabi_f6__p8
   LDA   #<__local_main__l29
   STA   __zpabi_f6__pp10_0
   LDA   #>__local_main__l29
   JSR   f14@run4
   STA   __zpabi_mix__v_0
   STX   __zpabi_mix__v_1
   LDA   #$00
   STA   __zpabi_mix__v_2
   JSR   fin1@run3
   INC   __local_main__w31
   LDA   __local_main__w31
   CMP   #$03
   BCS   .lb_skip@4
   JMP   .loop@15_start
.lb_skip@4:
   LDA   #$00
   SBC   g2
   STA   __local_main__0
   LDA   #$00
   SBC   g2+1
   STA   __local_main__w30
   BMI   .sx_neg@18
   LDA   #$00
   BEQ   .sx_done@19
.sx_neg@18:
   LDA   #$FF
.sx_done@19:
   STA   __zpabi_mix__v_2
   STA   __zpabi_mix__v_3
   LDA   __local_main__0
   STA   __zpabi_mix__v_0
   LDA   __local_main__w30
   JSR   fin0@run2
   LDA   __local_main__l29
   SEC
   SBC   #$FF
   STA   __local_main__l29
   LDA   __local_main__7
   STA   __local_main__w30
   JMP   .loop@14_continue
.loop@14_break:
   LDA   #$00
   STA   __local_main__w32
.loop@16_start:
   LDA   __local_main__l29
   STA   __local_main__5
   LDA   #$00
   BPL   .sx_done@21
   LDA   #$FF
.sx_done@21:
   STA   __local_main__4
   STA   __local_main__3
   LDA   gs4+9
   STA   __zpabi_f6__p9_0
   LDA   gs4+10
   STA   __zpabi_f6__p9_1
   LDA   gs4+11
   STA   __local_main__0
   LDA   gs4+12
   STA   __local_main__w30
   LDA   __zpabi_f6__p9_0
   SEC
   SBC   __local_main__5
   LDA   __zpabi_f6__p9_1
   SBC   #$00
   LDA   __local_main__0
   SBC   __local_main__4
   LDA   __local_main__w30
   SBC   __local_main__3
   BVC   .cmp_novf@22
   EOR   #$80
.cmp_novf@22:
   ASL
   LDA   #$00
   ROL
   SEC
   SBC   g1
   STA   gs4+1
   LDA   #$00
   SBC   #$00
   STA   gs4+2
   BMI   .sx_neg@23
   LDA   #$00
   BEQ   .sx_done@24
.sx_neg@23:
   LDA   #$FF
.sx_done@24:
   STA   gs4+3
   STA   gs4+4
   STA   gs4+5
   STA   gs4+6
   STA   gs4+7
   STA   gs4+8
   INC   __local_main__w32
   BEQ   .loop@16_start
   JSR   fin0
   JSR   fin1
   LDA   acc+1
   AND   #$7F
   TAX
   LDA   acc
   RTS

acc:
   DS.B  2

ga3:
   DC.B  $FF, $00, $7F, $80

gs4:
   DS.B  1
   DC.L  $0000007F
   DC.L  $00000000
   DC.L  $80000000

__zp_init:
   SUBROUTINE

   LDA   #$00
   STA   g1
   STA   @15.s26+1
   LDA   #$FF
   STA   g2
   STA   g2+1
   LDA   #<g2
   STA   gp5
   LDA   #>g2
   STA   gp5+1
   LDA   #$0D
   STA   @15.s26
   RTS
