SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
z3	EQU	$26
z4	EQU	$27
g1	EQU	$2F
g2	EQU	$30
__zpabi_f22__p24_0	EQU	$80
__local_f14__1	EQU	$81
__zpabi_f22__p24_1	EQU	$81
__zpabi_f7__p9_0	EQU	$82
__zpabi_f7__p9_1	EQU	$83
__zpabi_f7__p10_0	EQU	$84
__zpabi_f7__p10_1	EQU	$85
__zpabi_f7__p10_2	EQU	$86
__zpabi_f7__p10_3	EQU	$87
__zpabi_f7__p10_4	EQU	$88
__zpabi_f7__p10_5	EQU	$89
__zpabi_f7__p10_6	EQU	$8A
__zpabi_f7__p10_7	EQU	$8B
__zpabi_f7__pp11_0	EQU	$8C
__zpabi_f7__pp11_1	EQU	$8D
__zpabi_mix__v_0	EQU	$8E
__zpabi_mix__v_1	EQU	$8F
__zpabi_mix__v_2	EQU	$90
__zpabi_mix__v_3	EQU	$91
__local_main__1	EQU	$93
__local_main__i31	EQU	$96
__local_f22__0	EQU	$97
__local_f22__1	EQU	$98
__local_f22__l25	EQU	$99
__local_f22__2	EQU	$9A
__local_f22__3	EQU	$9B
__local_f22__l26	EQU	$9C
__local_f7__0	EQU	$A4
__local_f7__1	EQU	$A5
__local_f7__2	EQU	$A6
__local_f7__3	EQU	$A7
__local_mix__0	EQU	$A8
__local_mix__1	EQU	$A9
__local_mix__2	EQU	$AA
__local_mix__3	EQU	$AB

; @zp-link-meta-begin
; def f14 params= locals=__local_f14__0,__local_f14__1 indirect=false in_cycle=false
; def f22 params=__zpabi_f22__p23,__zpabi_f22__p24_0,__zpabi_f22__p24_1 locals=__local_f22__0,__local_f22__1,__local_f22__l25,__local_f22__2,__local_f22__3,__local_f22__l26,__local_f22__5,__local_f22__6,__local_f22__7,__local_f22__8,__local_f22__9,__local_f22__10,__local_f22__11 indirect=false in_cycle=false param_regs=A,,
; def f7 params=__zpabi_f7__p8,__zpabi_f7__p9_0,__zpabi_f7__p9_1,__zpabi_f7__p10_0,__zpabi_f7__p10_1,__zpabi_f7__p10_2,__zpabi_f7__p10_3,__zpabi_f7__p10_4,__zpabi_f7__p10_5,__zpabi_f7__p10_6,__zpabi_f7__p10_7,__zpabi_f7__pp11_0,__zpabi_f7__pp11_1 locals=__local_f7__0,__local_f7__1,__local_f7__2,__local_f7__3 indirect=false in_cycle=false param_regs=A,,,,,,,,,,,,
; def fin0 params= locals=__local_fin0__0,__local_fin0__1 indirect=false in_cycle=false
; def fin1 params= locals=__local_fin1__0,__local_fin1__1,__local_fin1__2,__local_fin1__3 indirect=false in_cycle=false
; def fin2 params= locals=__local_fin2__0,__local_fin2__1 indirect=false in_cycle=false
; def main params= locals=__local_main__0,__local_main__1,__local_main__2,__local_main__3,__local_main__i31 indirect=false in_cycle=false
; def mix params=__zpabi_mix__v_0,__zpabi_mix__v_1,__zpabi_mix__v_2,__zpabi_mix__v_3 locals=__local_mix__0,__local_mix__1,__local_mix__2,__local_mix__3 indirect=false in_cycle=false
; call f22 -> f7
; call f7 -> mix
; call fin0 -> mix
; call fin1 -> mix
; call fin2 -> mix
; call main -> f14
; call main -> f22
; call main -> fin0
; call main -> fin1
; call main -> fin2
; call main -> mix
; item mix section=.text local=false zero=false
; item fin0 section=.text local=false zero=false
; item fin0@run4 section=.text local=true zero=false
; item fin0@run1 section=.text local=true zero=false
; item fin0@run0 section=.text local=true zero=false
; item fin1 section=.text local=false zero=false
; item fin2 section=.text local=false zero=false
; item fin2@run2 section=.text local=true zero=false
; item f7 section=.text local=false zero=false
; item f14 section=.text local=false zero=false
; item f14@run3 section=.text local=true zero=false
; item f22 section=.text local=false zero=false
; item main section=.text local=false zero=false
; item acc section=.bss local=false zero=true
; item ga5 section=.data local=false zero=false
; item gs6 section=.data local=false zero=false
; item @5.s13 section=.data local=true zero=false
; item __zp_init section=.text local=false zero=false
; zp z3 size=1 at=auto def=true init=$FF
; zp z4 size=8 at=auto def=true init=$FF,$7F,$00,$00,$00,$00,$00,$00
; zp g1 size=1 at=auto def=true init=$FF weight=67
; zp g2 size=2 at=auto def=true init=$80,$00 weight=79
; runtime hargs_size=32 hargs_need=25
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
   BMI   .sx_neg@0
   LDA   #$00
   BEQ   .sx_done@1
.sx_neg@0:
   LDA   #$FF
.sx_done@1:
   JSR   fin0@run4
   LDA   g1
   STA   __zpabi_mix__v_0
   JSR   mix
   LDA   g2
   STA   __zpabi_mix__v_0
   LDA   g2+1
   STA   __zpabi_mix__v_1
   BMI   .sx_neg@2
   LDA   #$00
   BEQ   .sx_done@3
.sx_neg@2:
   LDA   #$FF
.sx_done@3:
   STA   __zpabi_mix__v_2
   JSR   fin0@run1
   LDA   z3
   BMI   .sx_neg@4
   LDA   #$00
   BEQ   .sx_done@5
.sx_neg@4:
   LDA   #$FF
.sx_done@5:
   JSR   fin0@run4
   LDA   z3
   STA   __zpabi_mix__v_0
   JSR   mix
   JSR   fin0@run0
   JMP   mix

fin0@run4:
   SUBROUTINE

   STA   __zpabi_mix__v_1
   STA   __zpabi_mix__v_2
   STA   __zpabi_mix__v_3
   RTS

fin0@run1:
   SUBROUTINE

   STA   __zpabi_mix__v_3
   JMP   mix

fin0@run0:
   SUBROUTINE

   LDA   z4
   STA   __zpabi_mix__v_0
   LDA   z4+1
   STA   __zpabi_mix__v_1
   LDA   z4+2
   STA   __zpabi_mix__v_2
   LDA   z4+3
   STA   __zpabi_mix__v_3
   LDA   z4+4
   LDA   z4+5
   LDA   z4+6
   LDA   z4+7
   RTS

fin1:
   SUBROUTINE

   LDA   ga5
   STA   __zpabi_mix__v_0
   LDA   ga5+1
   STA   __zpabi_mix__v_1
   LDA   ga5+2
   STA   __zpabi_mix__v_2
   LDA   ga5+3
   JSR   fin0@run1
   LDA   ga5+4
   STA   __zpabi_mix__v_0
   LDA   ga5+5
   STA   __zpabi_mix__v_1
   LDA   ga5+6
   STA   __zpabi_mix__v_2
   LDA   ga5+7
   JSR   fin0@run1
   LDA   ga5+8
   STA   __zpabi_mix__v_0
   LDA   ga5+9
   STA   __zpabi_mix__v_1
   LDA   ga5+10
   STA   __zpabi_mix__v_2
   LDA   ga5+11
   JSR   fin0@run1
   LDA   ga5+12
   STA   __zpabi_mix__v_0
   LDA   ga5+13
   STA   __zpabi_mix__v_1
   LDA   ga5+14
   STA   __zpabi_mix__v_2
   LDA   ga5+15
   STA   __zpabi_mix__v_3
   JMP   mix

fin2:
   SUBROUTINE

   LDA   gs6+1
   STA   __zpabi_mix__v_0
   LDA   gs6+2
   JSR   fin2@run2
   LDA   gs6+3
   BMI   .sx_neg@6
   LDA   #$00
   BEQ   .sx_done@7
.sx_neg@6:
   LDA   #$FF
.sx_done@7:
   JSR   fin0@run4
   LDA   gs6+3
   STA   __zpabi_mix__v_0
   JMP   mix

fin2@run2:
   SUBROUTINE

   STA   __zpabi_mix__v_1
   LDA   #$00
   STA   __zpabi_mix__v_2
   JMP   fin0@run1

f7:
   SUBROUTINE

   LDA   z3
   BMI   .sx_neg@8
   LDA   #$00
   BEQ   .sx_done@9
.sx_neg@8:
   LDA   #$FF
.sx_done@9:
   STA   __local_f7__1
   LDA   gs6+3
   STA   __local_f7__0
   BMI   .sx_neg@10
   LDA   #$00
   BEQ   .sx_done@11
.sx_neg@10:
   LDA   #$FF
.sx_done@11:
   STA   HARGS+3
   LDA   z3
   STA   HARGS
   LDA   __local_f7__1
   STA   HARGS+1
   LDA   __local_f7__0
   STA   HARGS+2
   JSR   mul16
   LDA   HARGS+4
   STA   ga5
   LDA   HARGS+5
   STA   ga5+1
   BMI   .sx_neg@12
   LDA   #$00
   BEQ   .sx_done@13
.sx_neg@12:
   LDA   #$FF
.sx_done@13:
   STA   ga5+2
   STA   ga5+3
   LDA   z3
   ORA   #$03
   STA   z3
   DEC   gs6+3
   BEQ   .if_end@0
   LDA   g1
   BEQ   .cond_else@1
   BPL   .cond_end@2
   BMI   .cond_end@2
.cond_else@1:
   LDA   #$00
.cond_end@2:
   ASL   @5.s13
   ROL   @5.s13+1
   ASL   @5.s13
   ROL   @5.s13+1
   CLC
   ADC   @5.s13
   AND   #$07
   BEQ   .case@1
   CMP   #$02
   BEQ   .case@2
.case@1:
.case@2:
   LDA   @5.s13
   STA   HARGS+2
   LDA   #$00
   STA   HARGS
   STA   HARGS+1
   LDA   @5.s13+1
   STA   HARGS+3
   JSR   mul16
   LDA   HARGS+4
   STA   __zpabi_mix__v_0
   LDA   HARGS+5
   JSR   fin2@run2
   LDA   gs6+1
   AND   #$81
   STA   gs6+1
   STA   __zpabi_mix__v_0
   LDA   #$00
   STA   gs6+2
   STA   __zpabi_mix__v_1
   STA   __zpabi_mix__v_2
   JSR   fin0@run1
.if_end@0:
   LDA   g2
   STA   __local_f7__3
   LDA   g2+1
   STA   __local_f7__2
   LDA   __local_f7__3
   ASL
   STA   __local_f7__0
   LDA   __local_f7__2
   ROL
   ASL   __local_f7__0
   ROL
   STA   __local_f7__1
   LDA   __local_f7__0
   CLC
   ADC   __local_f7__3
   STA   __local_f7__3
   LDA   __local_f7__1
   ADC   __local_f7__2
   STA   __local_f7__2
   LDA   __local_f7__3
   STA   g2
   LDA   __local_f7__2
   STA   g2+1
   BMI   .sx_neg@18
   LDA   #$00
   BEQ   .sx_done@19
.sx_neg@18:
   LDA   #$FF
.sx_done@19:
   STA   __local_f7__1
   STA   __local_f7__0
   LDA   __local_f7__3
   EOR   __zpabi_f7__p10_0
   STA   __zpabi_mix__v_0
   LDA   __local_f7__2
   EOR   __zpabi_f7__p10_1
   STA   __zpabi_mix__v_1
   LDA   __local_f7__1
   EOR   __zpabi_f7__p10_2
   STA   __zpabi_mix__v_2
   LDA   __local_f7__0
   EOR   __zpabi_f7__p10_3
   JSR   fin0@run1
   LDA   @5.s13
   LDX   @5.s13+1
   RTS

f14:
   SUBROUTINE

   LDA   g2
   LDX   g2+1
   STX   __local_f14__1
   SEC
   SBC   #$05
   BCS   .carry_inc@0
   DEC   __local_f14__1
.carry_inc@0:
   STA   g2
   LDA   __local_f14__1
   STA   g2+1
   LDA   #$03
   STA   z3
   LDA   #$01
   JSR   f14@run3
   STA   HARGS+11
   RTS

f14@run3:
   SUBROUTINE

   STA   HARGS+8
   LDA   #$00
   STA   HARGS+9
   LDY   #$00
   STY   HARGS+10
   RTS

f22:
   SUBROUTINE

   LDA   #$7F
   STA   __local_f22__l25
   LDA   #$00
   STA   __local_f22__2
   LDA   #$06
   STA   __local_f22__l26
   LDA   #$00
   STA   __local_f22__l26+1
   STA   __local_f22__l26+2
   STA   __local_f22__l26+3
   STA   __local_f22__l26+4
   STA   __local_f22__l26+5
   STA   __local_f22__l26+6
   STA   __local_f22__l26+7
   LDA   __local_f22__l25
   STA   __local_f22__0
   LDA   __local_f22__2
   STA   __local_f22__1
   INC   __local_f22__0
   BNE   .inc_done@0
   INC   __local_f22__1
.inc_done@0:
   LDA   __local_f22__0
   STA   __local_f22__l25
   LDA   __local_f22__1
   STA   __local_f22__2
   LDA   z4
   STA   HARGS
   LDA   z4+1
   STA   HARGS+1
   LDA   z4+2
   STA   HARGS+2
   LDA   z4+3
   STA   HARGS+3
   LDA   z4+4
   STA   HARGS+4
   LDA   z4+5
   STA   HARGS+5
   LDA   z4+6
   STA   HARGS+6
   LDA   z4+7
   STA   HARGS+7
   LDA   #$07
   JSR   f14@run3
   STY   HARGS+11
   STY   HARGS+12
   STY   HARGS+13
   STY   HARGS+14
   STA   HARGS+15
   JSR   sdivmod64
   LDA   HARGS+24
   LSR
   LDA   #$00
   ROR
   STA   gs6+3
   STA   __local_f22__3
   LDA   __local_f22__l26
   ORA   __local_f22__l26+1
   ORA   __local_f22__l26+2
   ORA   __local_f22__l26+3
   ORA   __local_f22__l26+4
   ORA   __local_f22__l26+5
   ORA   __local_f22__l26+6
   ORA   __local_f22__l26+7
   BEQ   .lnot_true@20
   LDA   #$00
   BEQ   .lnot_end@21
.lnot_true@20:
   LDA   #$01
.lnot_end@21:
   STA   __local_f22__2
   LDA   z3
   BMI   .sx_neg@22
   LDA   #$00
   BEQ   .sx_done@23
.sx_neg@22:
   LDA   #$FF
.sx_done@23:
   STA   __zpabi_f7__p10_1
   STA   __zpabi_f7__p10_2
   STA   __zpabi_f7__p10_3
   STA   __zpabi_f7__p10_4
   STA   __zpabi_f7__p10_5
   STA   __zpabi_f7__p10_6
   STA   __zpabi_f7__p10_7
   LDA   __local_f22__2
   STA   __zpabi_f7__p9_0
   LDA   #$00
   STA   __zpabi_f7__p9_1
   LDA   z3
   STA   __zpabi_f7__p10_0
   LDA   #<__local_f22__l26
   STA   __zpabi_f7__pp11_0
   LDA   #>__local_f22__l26
   STA   __zpabi_f7__pp11_1
   LDA   __local_f22__3
   JSR   f7
   LDA   #$00
   STA   HARGS+8
   LDY   #$00
   STY   HARGS+9
   STY   HARGS+10
   STA   HARGS+11
   RTS

main:
   SUBROUTINE

   JSR   fin0@run0
   JSR   mix
   JSR   f14
   LDA   #$00
   STA   __local_main__i31
.main@rot@0:
   LDA   z3
   AND   #$81
   STA   z3
   LDX   g2
   STX   __local_main__1
   LDX   g2+1
   ORA   __local_main__1
   AND   #$07
   BEQ   .case@12
   CMP   #$03
   BEQ   .case@13
   CMP   #$04
   BEQ   .case@14
   CMP   #$07
   BEQ   .case@15
   BNE   .switch@11_break
.case@12:
   JSR   f14
.case@13:
.case@14:
   LDA   __local_main__i31
   CLC
   SBC   __local_main__i31
   LDA   #$00
   ROL
   STA   __local_main__1
   LDA   z3
   BMI   .sx_neg@26
   LDA   #$00
   BEQ   .sx_done@27
.sx_neg@26:
   LDA   #$FF
.sx_done@27:
   STA   __zpabi_f22__p24_1
   LDA   z3
   STA   __zpabi_f22__p24_0
   LDA   __local_main__1
   JSR   f22
.case@15:
.switch@11_break:
   INC   __local_main__i31
   LDA   __local_main__i31
   CMP   #$04
   BCC   .main@rot@0
   LDA   g2
   STA   HARGS
   LDA   g2+1
   STA   HARGS+1
   LDA   #$06
   STA   HARGS+2
   LDA   #$00
   STA   HARGS+3
   JSR   sdivmod16
   LDA   #$01
   STA   __zpabi_f22__p24_0
   LDA   #$00
   STA   __zpabi_f22__p24_1
   LDA   HARGS+6
   JSR   f22
   LDA   g2
   LDA   g2+1
   LDA   ga5
   AND   #$01
   STA   ga5
   LDA   #$00
   STA   ga5+1
   LDY   #$00
   STY   ga5+2
   STA   ga5+3
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

ga5:
   DC.L  $00010000
   DC.L  $000000FF
   DC.L  $0000FFFF
   DC.L  $0000000D

gs6:
   DS.B  1
   DC.W  $0100
   DC.B  $03

@5.s13:
   DC.W  $8000

__zp_init:
   SUBROUTINE

   LDA   #$00
   STA   g2+1
   STA   z4+2
   STA   z4+3
   STA   z4+4
   STA   z4+5
   STA   z4+6
   STA   z4+7
   LDA   #$FF
   STA   g1
   STA   z3
   STA   z4
   LDA   #$80
   STA   g2
   LDA   #$7F
   STA   z4+1
   RTS
