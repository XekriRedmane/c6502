SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
g3	EQU	$26
g2	EQU	$27
@11.s24	EQU	$29
g1	EQU	$2A
__zpabi_f27__pp29_0	EQU	$80
__zpabi_f27__pp29_1	EQU	$81
__zpabi_f17__p19_0	EQU	$82
__zpabi_f17__p19_1	EQU	$83
__zpabi_f17__p19_2	EQU	$84
__zpabi_f17__p19_3	EQU	$85
__zpabi_f17__p19_4	EQU	$86
__zpabi_f17__p19_5	EQU	$87
__zpabi_f17__p19_6	EQU	$88
__zpabi_f17__p19_7	EQU	$89
__zpabi_f17__pp20_0	EQU	$8A
__zpabi_f17__pp20_1	EQU	$8B
__zpabi_f9__p10_0	EQU	$8C
__zpabi_f9__p10_1	EQU	$8D
__zpabi_f9__p10_2	EQU	$8E
__zpabi_f9__p10_3	EQU	$8F
__zpabi_f9__p11	EQU	$90
__zpabi_mix__v_0	EQU	$91
__zpabi_mix__v_1	EQU	$92
__zpabi_mix__v_2	EQU	$93
__zpabi_mix__v_3	EQU	$94
__local_f27__0	EQU	$95
__local_main__0	EQU	$95
__local_main__1	EQU	$96
__local_f27__2	EQU	$97
__local_main__4	EQU	$99
__local_f27__l30	EQU	$9A
__local_main__l34	EQU	$9A
__local_f17__0	EQU	$9B
__local_f17__2	EQU	$9D
__local_f17__w25	EQU	$9E
__local_f9__w16	EQU	$9F
__local_f9__0	EQU	$A0
__local_f9__i14	EQU	$A1
__local_f9__1	EQU	$A2
__local_f9__2	EQU	$A3
__local_f9__3	EQU	$A4
__local_f9__4	EQU	$A5
__local_mix__0	EQU	$A7
__local_mix__1	EQU	$A8
__local_mix__2	EQU	$A9
__local_mix__3	EQU	$AA

; @zp-link-meta-begin
; def f17 params=__zpabi_f17__p18,__zpabi_f17__p19_0,__zpabi_f17__p19_1,__zpabi_f17__p19_2,__zpabi_f17__p19_3,__zpabi_f17__p19_4,__zpabi_f17__p19_5,__zpabi_f17__p19_6,__zpabi_f17__p19_7,__zpabi_f17__pp20_0,__zpabi_f17__pp20_1 locals=__local_f17__0,__local_f17__1,__local_f17__2,__local_f17__w25 indirect=false in_cycle=false param_regs=A,,,,,,,,,,
; def f27 params=__zpabi_f27__p28_0,__zpabi_f27__p28_1,__zpabi_f27__pp29_0,__zpabi_f27__pp29_1 locals=__local_f27__0,__local_f27__1,__local_f27__2,__local_f27__p28,__local_f27__3,__local_f27__l30 indirect=false in_cycle=false param_regs=A,X,,
; def f9 params=__zpabi_f9__p10_0,__zpabi_f9__p10_1,__zpabi_f9__p10_2,__zpabi_f9__p10_3,__zpabi_f9__p11 locals=__local_f9__w16,__local_f9__0,__local_f9__i14,__local_f9__1,__local_f9__2,__local_f9__3,__local_f9__4,__local_f9__5 indirect=false in_cycle=false
; def fin0 params= locals=__local_fin0__0,__local_fin0__1 indirect=false in_cycle=false
; def fin1 params= locals=__local_fin1__0 indirect=false in_cycle=false
; def fin2 params= locals=__local_fin2__0,__local_fin2__1,__local_fin2__2,__local_fin2__3 indirect=false in_cycle=false
; def main params= locals=__local_main__0,__local_main__1,__local_main__2,__local_main__3,__local_main__4,__local_main__l34 indirect=false in_cycle=false
; def mix params=__zpabi_mix__v_0,__zpabi_mix__v_1,__zpabi_mix__v_2,__zpabi_mix__v_3 locals=__local_mix__0,__local_mix__1,__local_mix__2,__local_mix__3 indirect=false in_cycle=false
; call f17 -> f9
; call f27 -> f17
; call f9 -> mix
; call fin0 -> mix
; call fin1 -> mix
; call fin2 -> mix
; call main -> f17
; call main -> fin0
; call main -> fin1
; call main -> fin2
; call main -> mix
; item mix section=.text local=false zero=false
; item fin0 section=.text local=false zero=false
; item fin0@run7 section=.text local=true zero=false
; item fin0@run6 section=.text local=true zero=false
; item fin0@run2 section=.text local=true zero=false
; item fin1 section=.text local=false zero=false
; item fin1@run9 section=.text local=true zero=false
; item fin1@run3 section=.text local=true zero=false
; item fin2 section=.text local=false zero=false
; item f9 section=.text local=false zero=false
; item f9@run4 section=.text local=true zero=false
; item f17 section=.text local=false zero=false
; item f17@run8 section=.text local=true zero=false
; item f17@run1 section=.text local=true zero=false
; item f27 section=.text local=false zero=false
; item f27@run5 section=.text local=true zero=false
; item f27@run0 section=.text local=true zero=false
; item main section=.text local=false zero=false
; item acc section=.bss local=false zero=true
; item g4 section=.data local=false zero=false
; item ga5 section=.data local=false zero=false
; item gs6 section=.data local=false zero=false
; item gp7 section=.data local=false zero=false
; item gp8 section=.data local=false zero=false
; item __zp_init section=.text local=false zero=false
; zp g3 size=1 at=auto def=true init=$FF weight=70
; zp g2 size=2 at=auto def=true init=$00,$00 weight=46
; zp @11.s24 size=1 at=auto def=true init=$00 weight=18 local=true
; zp g1 size=2 at=auto def=true init=$02,$00 weight=12
; runtime hargs_size=32 hargs_need=12
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

   JSR   fin0@run6
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
   STA   __zpabi_mix__v_1
   JSR   mix
   LDA   g3
   JSR   fin0@run7
   JSR   mix
   LDA   g4
   JSR   fin0@run7
   JMP   mix

fin0@run7:
   SUBROUTINE

   STA   __zpabi_mix__v_0
   LDA   #$00
   STA   __zpabi_mix__v_1
   STA   __zpabi_mix__v_2
   STA   __zpabi_mix__v_3
   RTS

fin0@run6:
   SUBROUTINE

   LDA   g1
   STA   __zpabi_mix__v_0
   LDA   g1+1
   JMP   fin0@run2

fin0@run2:
   SUBROUTINE

   STA   __zpabi_mix__v_1
   LDA   #$00
   STA   __zpabi_mix__v_2
   STA   __zpabi_mix__v_3
   JMP   mix

fin1:
   SUBROUTINE

   LDA   ga5
   BMI   .sx_neg@2
   LDA   #$00
   BEQ   .sx_done@3
.sx_neg@2:
   LDA   #$FF
.sx_done@3:
   JSR   fin1@run3
   LDA   ga5
   JSR   fin1@run9
   LDA   ga5+1
   BMI   .sx_neg@4
   LDA   #$00
   BEQ   .sx_done@5
.sx_neg@4:
   LDA   #$FF
.sx_done@5:
   JSR   fin1@run3
   LDA   ga5+1
   JSR   fin1@run9
   LDA   ga5+2
   BMI   .sx_neg@6
   LDA   #$00
   BEQ   .sx_done@7
.sx_neg@6:
   LDA   #$FF
.sx_done@7:
   JSR   fin1@run3
   LDA   ga5+2
   JSR   fin1@run9
   LDA   ga5+3
   BMI   .sx_neg@8
   LDA   #$00
   BEQ   .sx_done@9
.sx_neg@8:
   LDA   #$FF
.sx_done@9:
   JSR   fin1@run3
   LDA   ga5+3
   STA   __zpabi_mix__v_0
   JMP   mix

fin1@run9:
   SUBROUTINE

   STA   __zpabi_mix__v_0
   JMP   mix

fin1@run3:
   SUBROUTINE

   STA   __zpabi_mix__v_1
   STA   __zpabi_mix__v_2
   STA   __zpabi_mix__v_3
   RTS

fin2:
   SUBROUTINE

   LDA   gs6+1
   STA   __zpabi_mix__v_0
   LDA   gs6+2
   STA   __zpabi_mix__v_1
   LDA   gs6+3
   STA   __zpabi_mix__v_2
   LDA   gs6+4
   STA   __zpabi_mix__v_3
   JSR   mix
   LDA   gs6+5
   BMI   .sx_neg@10
   LDA   #$00
   BEQ   .sx_done@11
.sx_neg@10:
   LDA   #$FF
.sx_done@11:
   JSR   fin1@run3
   LDA   gs6+5
   STA   __zpabi_mix__v_0
   JMP   mix

f9:
   SUBROUTINE

   LDA   #$02
   STA   __local_f9__w16
   JSR   f9@run4
   LSR
   LSR
   LSR
   LSR
   LDY   #$00
   STA   (DPTR),Y
   LDA   #$00
   STA   __local_f9__i14
.f9@rot@0:
   LDA   g2
   EOR   #$03
   STA   g2
   AND   __local_f9__i14
   BNE   .loop@0_break
   LDA   g2
   STA   __local_f9__0
   LDA   g2+1
   STA   __local_f9__w16
   INC   g2
   BNE   .inc_done@0
   INC   g2+1
.inc_done@0:
   LDA   #$00
   AND   __local_f9__0
   STA   __zpabi_mix__v_0
   LDA   #$00
   AND   __local_f9__w16
   JSR   fin0@run2
   LDA   __local_f9__i14
   AND   #$03
   TAX
   LDA   ga5,X
   BMI   .sx_neg@12
   LDA   #$00
   BEQ   .sx_done@13
.sx_neg@12:
   LDA   #$FF
.sx_done@13:
   STA   __local_f9__w16
   LDA   ga5,X
   STA   g1
   LDA   __local_f9__w16
   STA   g1+1
   INC   __local_f9__i14
   LDA   __local_f9__i14
   CMP   #$03
   BCC   .f9@rot@0
.loop@0_break:
   LDA   #$00
   STA   __local_f9__w16
.loop@1_continue:
   CLC
   ADC   #$01
   STA   __local_f9__3
   LDA   __local_f9__w16
   BEQ   .lb_skip@0
   JMP   .loop@1_break
.lb_skip@0:
   LDA   #$00
   STA   __local_f9__w16
.loop@2_continue:
   CLC
   ADC   #$01
   STA   __local_f9__2
   LDA   __local_f9__w16
   BNE   .loop@2_break
   LDA   gs6+1
   EOR   #$01
   STA   gs6+1
   AND   #$07
   BEQ   .case@4
   CMP   #$02
   BEQ   .case@5
   CMP   #$05
   BEQ   .case@6
   BNE   .switch@3_break
.case@4:
.case@5:
   LDA   __zpabi_f9__p11
   BMI   .sx_neg@14
   LDA   #$00
   BEQ   .sx_done@15
.sx_neg@14:
   LDA   #$FF
.sx_done@15:
   STA   __local_f9__4
   STA   __local_f9__3
   STA   __local_f9__2
   LDA   gs6+1
   STA   __local_f9__1
   LDA   gs6+2
   STA   __local_f9__i14
   LDA   gs6+3
   STA   __local_f9__0
   LDA   gs6+4
   STA   __local_f9__w16
   LDA   __zpabi_f9__p11
   AND   __local_f9__1
   STA   HARGS+8
   LDA   __local_f9__4
   AND   __local_f9__i14
   STA   HARGS+9
   LDA   __local_f9__3
   AND   __local_f9__0
   STA   HARGS+10
   LDA   __local_f9__2
   AND   __local_f9__w16
   STA   HARGS+11
   RTS
.case@6:
.switch@3_break:
   LDA   __local_f9__2
   STA   __local_f9__w16
   JMP   .loop@2_continue
.loop@2_break:
   LDA   __local_f9__3
   STA   __local_f9__w16
   JMP   .loop@1_continue
.loop@1_break:
   LDA   g3
   LSR
   LSR
   LSR
   LSR
   LSR
   LSR
   STA   HARGS+8
   LDA   #$00
   STA   HARGS+9
   LDY   #$00
   STY   HARGS+10
   STA   HARGS+11
   RTS

f9@run4:
   SUBROUTINE

   LDA   gp7
   STA   DPTR
   LDA   gp7+1
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   RTS

f17:
   SUBROUTINE

   LDA   __zpabi_f17__p19_0
   ORA   __zpabi_f17__p19_1
   ORA   __zpabi_f17__p19_2
   ORA   __zpabi_f17__p19_3
   ORA   __zpabi_f17__p19_4
   ORA   __zpabi_f17__p19_5
   ORA   __zpabi_f17__p19_6
   ORA   __zpabi_f17__p19_7
   BEQ   .and_false@2
   LDA   g3
   BEQ   .and_false@2
   LDA   #$01
   STA   __zpabi_f9__p10_0
   LDA   #$00
   BEQ   .and_end@3
.and_false@2:
   LDA   #$00
   STA   __zpabi_f9__p10_0
.and_end@3:
   ORA   #$00
   BPL   .sx_done@17
   LDA   #$FF
.sx_done@17:
   STA   __zpabi_f9__p10_2
   STA   __zpabi_f9__p10_3
   LDA   #$00
   STA   __zpabi_f9__p10_1
   LDA   #$FF
   STA   __zpabi_f9__p11
   JSR   f9
   LDA   #$00
   STA   __local_f17__w25
.loop@7_start:
   LDA   #$FF
   STA   __zpabi_f9__p10_0
   LDA   #$00
   STA   __zpabi_f9__p10_1
   STA   __zpabi_f9__p10_2
   STA   __zpabi_f9__p10_3
   STA   __zpabi_f9__p11
   JSR   f9
   LDA   __local_f17__w25
   AND   #$03
   TAX
   LDA   ga5,X
   STA   __zpabi_f9__p10_0
   BMI   .sx_neg@18
   LDA   #$00
   BEQ   .sx_done@19
.sx_neg@18:
   LDA   #$FF
.sx_done@19:
   STA   __local_f17__0
   LDA   __zpabi_f9__p10_0
   EOR   #$FF
   STA   __zpabi_f9__p10_0
   LDA   __local_f17__0
   EOR   #$FF
   STA   __local_f17__0
   LDA   g3
   SEC
   SBC   __zpabi_f9__p10_0
   STA   gs6+1
   LDA   #$00
   SBC   __local_f17__0
   STA   gs6+2
   BMI   .sx_neg@20
   LDA   #$00
   BEQ   .sx_done@21
.sx_neg@20:
   LDA   #$FF
.sx_done@21:
   STA   gs6+3
   STA   gs6+4
   LDA   @11.s24
   CLC
   ADC   #$01
   STA   @11.s24
   INC   __local_f17__w25
   LDA   __local_f17__w25
   CMP   #$03
   BCC   .loop@7_start
   LDA   #$01
   CLC
   SBC   #$00
   LDA   #$00
   SBC   #$00
   LDA   #$00
   ROL
   STA   ga5+1
   LDA   @11.s24
   CLC
   ADC   #$81
   STA   @11.s24
   LDY   #$00
   LDA   (__zpabi_f17__pp20_0),Y
   STA   __local_f17__2
   LDA   gs6+5
   STA   __zpabi_f9__p10_0
   BMI   .sx_neg@22
   LDA   #$00
   BEQ   .sx_done@23
.sx_neg@22:
   LDA   #$FF
.sx_done@23:
   STA   __local_f17__0
   LDA   __local_f17__2
   CLC
   ADC   __zpabi_f9__p10_0
   STA   __zpabi_f9__p10_0
   LDA   #$00
   ADC   __local_f17__0
   LDX   __zpabi_f9__p10_0
   STX   g1
   STA   g1+1
   JSR   f17@run1
   STA   __local_f17__0
   LDY   #$01
   LDA   (DPTR),Y
   STA   __zpabi_f9__p10_0
   LDX   #$06
.shift_loop@24:
   LDA   __zpabi_f9__p10_0
   ASL
   ROR   __zpabi_f9__p10_0
   ROR   __local_f17__0
   DEX
   BNE   .shift_loop@24
   LDA   __local_f17__0
   LDY   #$00
   STA   (DPTR),Y
   JSR   f17@run8
   LDA   __zpabi_f9__p10_0
   LDY   #$01
   STA   (DPTR),Y
   LDX   #$00
   TXA
   RTS

f17@run8:
   SUBROUTINE

   LDA   gp8
   STA   DPTR
   LDA   gp8+1
   STA   DPTR+1
   RTS

f17@run1:
   SUBROUTINE

   JSR   f17@run8
   LDY   #$00
   LDA   (DPTR),Y
   RTS

f27:
   SUBROUTINE

   STA   __zpabi_f17__p19_0
   LDA   #$00
   STA   __local_f27__l30
   LDY   #$00
   LDA   (__zpabi_f27__pp29_0),Y
   STA   __local_f27__0
   SEC
   LDA   #$00
   SBC   __local_f27__0
   AND   #$07
   CMP   #$04
   LDA   g1
   STA   __local_f27__2
   LDA   g1+1
   LDA   __zpabi_f17__p19_0
   BMI   .sx_neg@28
   LDA   #$00
   BEQ   .sx_done@29
.sx_neg@28:
   LDA   #$FF
.sx_done@29:
   STA   __zpabi_f17__p19_1
   JSR   f27@run0
   JSR   f27@run5
   LDA   __local_f27__2
   JSR   f17
   LDA   g3
   ORA   g2
   STA   __zpabi_f17__p19_0
   LDA   g2+1
   BMI   .sx_neg@30
   LDA   #$00
   BEQ   .sx_done@31
.sx_neg@30:
   LDA   #$FF
.sx_done@31:
   JSR   f27@run0
   LDA   g2+1
   STA   __zpabi_f17__p19_1
   JSR   f27@run5
   LDA   gs6+5
   JSR   f17
   LDA   #$01
   STA   g3
   LDY   #$00
   LDA   (__zpabi_f27__pp29_0),Y
   SEC
   SBC   #$01
   STA   (__zpabi_f27__pp29_0),Y
   LDA   gs6+1
   ORA   gs6+2
   ORA   gs6+3
   ORA   gs6+4
   BEQ   .lnot_true@32
   LDA   #$00
   BEQ   .lnot_end@33
.lnot_true@32:
   LDA   #$01
.lnot_end@33:
   STA   __local_f27__2
   LDA   #$00
   BPL   .sx_done@35
   LDA   #$FF
.sx_done@35:
   JSR   f27@run0
   LDA   __local_f27__2
   STA   __zpabi_f17__p19_0
   LDA   #$00
   STA   __zpabi_f17__p19_1
   JSR   f27@run5
   LDA   #$FF
   JSR   f17
   JSR   f9@run4
   LDX   #$00
   RTS

f27@run5:
   SUBROUTINE

   LDA   #<__local_f27__l30
   STA   __zpabi_f17__pp20_0
   LDA   #>__local_f27__l30
   STA   __zpabi_f17__pp20_1
   RTS

f27@run0:
   SUBROUTINE

   STA   __zpabi_f17__p19_2
   STA   __zpabi_f17__p19_3
   STA   __zpabi_f17__p19_4
   STA   __zpabi_f17__p19_5
   STA   __zpabi_f17__p19_6
   STA   __zpabi_f17__p19_7
   RTS

main:
   SUBROUTINE

   LDA   #$D4
   STA   __local_main__l34
   LDA   g2
   EOR   #$05
   STA   g2
   JSR   f17@run1
   STA   __local_main__1
   LDY   #$01
   LDA   (DPTR),Y
   STA   __zpabi_f17__p19_0
   LDA   __local_main__1
   ORA   g3
   STA   __local_main__1
   LDA   __zpabi_f17__p19_0
   STA   __local_main__0
   BMI   .sx_neg@36
   LDA   #$00
   BEQ   .sx_done@37
.sx_neg@36:
   LDA   #$FF
.sx_done@37:
   STA   __zpabi_mix__v_2
   STA   __zpabi_mix__v_3
   LDA   __local_main__1
   STA   __zpabi_mix__v_0
   LDA   __local_main__0
   STA   __zpabi_mix__v_1
   JSR   mix
   LDA   #$D4
   DEC   __local_main__l34
   ORA   #$01
   STA   g2
   LDA   #$00
   STA   g2+1
   LDA   gs6+1
   LDX   gs6+2
   STX   __zpabi_f17__p19_1
   LDX   gs6+3
   STX   __zpabi_f17__p19_0
   LDX   gs6+4
   STX   __local_main__1
   SEC
   SBC   #$05
   STA   __local_main__0
   LDA   __zpabi_f17__p19_1
   SBC   #$00
   STA   __zpabi_f17__p19_1
   LDA   __zpabi_f17__p19_0
   SBC   #$00
   STA   __zpabi_f17__p19_0
   BCS   .carry_inc@0
   DEC   __local_main__1
.carry_inc@0:
   LDA   __local_main__0
   STA   gs6+1
   LDA   __zpabi_f17__p19_1
   STA   gs6+2
   LDA   __zpabi_f17__p19_0
   STA   gs6+3
   LDA   __local_main__1
   STA   gs6+4
   LDA   #$01
   ORA   __local_main__0
   ORA   __zpabi_f17__p19_1
   ORA   __zpabi_f17__p19_0
   ORA   __local_main__1
   BEQ   .if_end@8
   LDA   ga5+1
   STA   __local_main__1
   LDA   gs6+5
   CLC
   ADC   #$81
   STA   __local_main__0
   STA   gs6+5
   LDA   __local_main__1
   EOR   __local_main__0
   STA   ga5
   LDA   #$FF
   STA   __zpabi_mix__v_0
   JSR   fin1@run3
   JSR   mix
.if_end@8:
   JSR   fin0@run6
   LDA   g4
   LSR
   LDA   #$00
   ROR
   STA   __local_main__0
   LDA   #$FF
   SEC
   SBC   __local_main__0
   STA   __local_main__0
   LDA   #$FF
   ORA   __local_main__0
   BNE   .lb_skip@1
   JMP   .if_end@9
.lb_skip@1:
   LDA   #$FF
   SEC
   SBC   g2
   LDA   #$FF
   SBC   #$00
   BVC   .cmp_novf@40
   EOR   #$80
.cmp_novf@40:
   ASL
   LDA   #$00
   ROL
   EOR   #$01
   STA   __local_main__4
   JSR   f17@run1
   STA   __zpabi_f17__p19_0
   LDY   #$01
   LDA   (DPTR),Y
   STA   __zpabi_f17__p19_1
   LDX   #$06
.shift_loop@41:
   LDA   __zpabi_f17__p19_1
   ASL
   ROR   __zpabi_f17__p19_1
   ROR   __zpabi_f17__p19_0
   DEX
   BNE   .shift_loop@41
   LDA   __zpabi_f17__p19_1
   BMI   .sx_neg@43
   LDA   #$00
   BEQ   .sx_done@44
.sx_neg@43:
   LDA   #$FF
.sx_done@44:
   JSR   f27@run0
   LDA   #<__local_main__l34
   STA   __zpabi_f17__pp20_0
   LDA   #>__local_main__l34
   STA   __zpabi_f17__pp20_1
   LDA   __local_main__4
   JSR   f17
   STA   __local_main__1
   TXA
   BMI   .sx_neg@45
   LDA   #$00
   BEQ   .sx_done@46
.sx_neg@45:
   LDA   #$FF
.sx_done@46:
   STA   __zpabi_mix__v_2
   STA   __zpabi_mix__v_3
   LDA   __local_main__1
   STA   __zpabi_mix__v_0
   STX   __zpabi_mix__v_1
   JSR   mix
.if_end@9:
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

g4:
   DC.B  $02

ga5:
   DC.B  $0B, $03, $88, $80

gs6:
   DS.B  1
   DC.L  $00007FFF
   DS.B  1

gp7:
   DC.W  g4

gp8:
   DC.W  g2

__zp_init:
   SUBROUTINE

   LDA   #$00
   STA   g1+1
   STA   g2
   STA   g2+1
   STA   @11.s24
   LDA   #$02
   STA   g1
   LDA   #$FF
   STA   g3
   RTS
