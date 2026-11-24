SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
@15.s24	EQU	$26
g2	EQU	$27
__zpabi_f18__p20_0	EQU	$80
__zpabi_f18__p20_1	EQU	$81
__local_main__2	EQU	$82
__zpabi_f18__p21_0	EQU	$82
__local_main__3	EQU	$83
__zpabi_f18__p21_1	EQU	$83
__local_main__l26	EQU	$84
__zpabi_f10__p12	EQU	$84
__local_main__l27	EQU	$85
__zpabi_f10__p13	EQU	$85
__zpabi_mix__v_0	EQU	$86
__zpabi_mix__v_1	EQU	$87
__zpabi_mix__v_2	EQU	$88
__zpabi_mix__v_3	EQU	$89
__local_f18__l22	EQU	$8A
__local_f18__0	EQU	$8B
__local_f18__p20_1	EQU	$8C
__local_f18__p20_0	EQU	$8D
__local_f18__i25	EQU	$8E
__local_f18__p19	EQU	$8F
__local_f10__0	EQU	$92
__local_f10__1	EQU	$93
__local_f10__2	EQU	$94
__local_f10__3	EQU	$95
__local_f10__p11	EQU	$96
__local_f10__l16	EQU	$97
__local_f5__l8	EQU	$99
__local_f5__pp6	EQU	$9A
__local_f5__1	EQU	$9B
__local_mix__0	EQU	$9C
__local_mix__1	EQU	$9D
__local_mix__2	EQU	$9E
__local_mix__3	EQU	$9F

; @zp-link-meta-begin
; def f10 params=__zpabi_f10__p11_0,__zpabi_f10__p11_1,__zpabi_f10__p12,__zpabi_f10__p13 locals=__local_f10__0,__local_f10__1,__local_f10__2,__local_f10__3,__local_f10__p11,__local_f10__l16 indirect=false in_cycle=false param_regs=A,X,,
; def f18 params=__zpabi_f18__p19,__zpabi_f18__p20_0,__zpabi_f18__p20_1,__zpabi_f18__p21_0,__zpabi_f18__p21_1 locals=__local_f18__l22,__local_f18__0,__local_f18__p20_1,__local_f18__p20_0,__local_f18__i25,__local_f18__p19,__local_f18__1,__local_f18__2 indirect=false in_cycle=false param_regs=A,,,,
; def f5 params=__zpabi_f5__pp6_0,__zpabi_f5__pp6_1 locals=__local_f5__0,__local_f5__l8,__local_f5__pp6,__local_f5__1 indirect=false in_cycle=false param_regs=A,X
; def fin0 params= locals=__local_fin0__0,__local_fin0__1 indirect=false in_cycle=false
; def fin1 params= locals=__local_fin1__0,__local_fin1__1 indirect=false in_cycle=false
; def main params= locals=__local_main__0,__local_main__1,__local_main__2,__local_main__3,__local_main__l26,__local_main__l27 indirect=false in_cycle=false
; def mix params=__zpabi_mix__v_0,__zpabi_mix__v_1,__zpabi_mix__v_2,__zpabi_mix__v_3 locals=__local_mix__0,__local_mix__1,__local_mix__2,__local_mix__3 indirect=false in_cycle=false
; call f10 -> f5
; call f10 -> mix
; call f18 -> f10
; call f18 -> mix
; call f5 -> mix
; call fin0 -> mix
; call fin1 -> mix
; call main -> f5
; call main -> fin0
; call main -> fin1
; call main -> mix
; item mix section=.text local=false zero=false
; item fin0 section=.text local=false zero=false
; item fin0@run3 section=.text local=true zero=false
; item fin0@run2 section=.text local=true zero=false
; item fin0@run1 section=.text local=true zero=false
; item fin0@run0 section=.text local=true zero=false
; item fin1 section=.text local=false zero=false
; item f5 section=.text local=false zero=false
; item f10 section=.text local=false zero=false
; item f10@run4 section=.text local=true zero=false
; item f18 section=.text local=false zero=false
; item main section=.text local=false zero=false
; item acc section=.bss local=false zero=true
; item g1 section=.data local=false zero=false
; item ga3 section=.data local=false zero=false
; item gs4 section=.data local=false zero=false
; item __zp_init section=.text local=false zero=false
; zp @15.s24 size=1 at=auto def=true init=$00 weight=11 local=true
; zp g2 size=1 at=auto def=true init=$00 weight=6
; runtime hargs_size=32 hargs_need=21
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
   JSR   fin0@run1
   LDA   g2
   JSR   fin0@run0
   LDA   ga3+1
   BMI   .sx_neg@0
   LDA   #$00
   BEQ   .sx_done@1
.sx_neg@0:
   LDA   #$FF
.sx_done@1:
   JSR   fin0@run2
   LDA   ga3
   STA   __zpabi_mix__v_0
   LDA   ga3+1
   JSR   fin0@run3
   LDA   ga3+3
   BMI   .sx_neg@2
   LDA   #$00
   BEQ   .sx_done@3
.sx_neg@2:
   LDA   #$FF
.sx_done@3:
   JSR   fin0@run2
   LDA   ga3+2
   STA   __zpabi_mix__v_0
   LDA   ga3+3
   STA   __zpabi_mix__v_1
   JMP   mix

fin0@run3:
   SUBROUTINE

   STA   __zpabi_mix__v_1
   JMP   mix

fin0@run2:
   SUBROUTINE

   STA   __zpabi_mix__v_2
   STA   __zpabi_mix__v_3
   RTS

fin0@run1:
   SUBROUTINE

   LDA   #$00
   STA   __zpabi_mix__v_2
   STA   __zpabi_mix__v_3
   JMP   mix

fin0@run0:
   SUBROUTINE

   STA   __zpabi_mix__v_0
   LDA   #$00
   STA   __zpabi_mix__v_1
   JSR   fin0@run2
   JMP   mix

fin1:
   SUBROUTINE

   LDA   ga3+5
   BMI   .sx_neg@4
   LDA   #$00
   BEQ   .sx_done@5
.sx_neg@4:
   LDA   #$FF
.sx_done@5:
   JSR   fin0@run2
   LDA   ga3+4
   STA   __zpabi_mix__v_0
   LDA   ga3+5
   JSR   fin0@run3
   LDA   ga3+7
   BMI   .sx_neg@6
   LDA   #$00
   BEQ   .sx_done@7
.sx_neg@6:
   LDA   #$FF
.sx_done@7:
   JSR   fin0@run2
   LDA   ga3+6
   STA   __zpabi_mix__v_0
   LDA   ga3+7
   JSR   fin0@run3
   LDA   gs4+1
   JSR   fin0@run0
   LDA   gs4+2
   STA   __zpabi_mix__v_0
   LDA   gs4+3
   STA   __zpabi_mix__v_1
   LDA   #$00
   STA   __zpabi_mix__v_2
   STA   __zpabi_mix__v_3
   JMP   mix

f5:
   SUBROUTINE

   STA   __local_f5__pp6
   STX   __local_f5__1
   LDA   #$00
   STA   __local_f5__l8
   LDA   g1
   SEC
   SBC   g1
   STA   __zpabi_mix__v_0
   LDA   g1+1
   SBC   g1+1
   STA   __zpabi_mix__v_1
   JSR   fin0@run1
   LDA   g1
   STA   gs4+2
   LDA   g1+1
   STA   gs4+3
   LDY   #$00
   LDA   (__local_f5__pp6),Y
   AND   #$02
   STA   gs4+2
   LDA   #$00
   STA   gs4+3
   LDA   (__local_f5__pp6),Y
   JSR   fin0@run0
   INC   gs4+2
   BNE   .inc_done@0
   INC   gs4+3
.inc_done@0:
   LDA   __local_f5__l8
   LDA   gs4+2
   STA   HARGS+16
   LDA   #$00
   STA   HARGS+17
   LDY   #$00
   STY   HARGS+18
   STY   HARGS+19
   STA   HARGS+20
   RTS

f10:
   SUBROUTINE

   STA   __local_f10__p11
   LDA   #$7F
   STA   __local_f10__l16
   LDX   #>__local_f10__l16
   LDA   #<__local_f10__l16
   JSR   f5
   LDA   HARGS+16
   STA   __zpabi_mix__v_0
   LDA   HARGS+17
   STA   __zpabi_mix__v_1
   LDA   HARGS+18
   STA   __zpabi_mix__v_2
   LDA   HARGS+19
   STA   __zpabi_mix__v_3
   JSR   mix
   LDA   gs4+1
   STA   __local_f10__3
   LDA   #$00
   SEC
   SBC   #$80
   STA   __local_f10__2
   LDA   #$00
   SBC   #$00
   STA   __local_f10__1
   LDA   #$00
   SBC   #$00
   STA   __local_f10__0
   LDA   __local_f10__3
   ORA   __local_f10__2
   ORA   __local_f10__1
   ORA   __local_f10__0
   BEQ   .if_else@2
   LDA   gs4+2
   STA   __local_f10__0
   LDA   gs4+3
   ASL   __local_f10__0
   ROL
   ASL   __local_f10__0
   ROL
   ASL   __local_f10__0
   ROL
   ASL   __local_f10__0
   ROL
   STA   gs4+3
   STA   g1+1
   LDA   __local_f10__0
   STA   gs4+2
   STA   g1
   LDA   #$00
   STA   __zpabi_mix__v_0
   JSR   f10@run4
   JSR   mix
   JMP   .if_end@1
.if_else@2:
   LDA   g2
   SEC
   SBC   #$03
   STA   g2
   LDY   #$80
   STY   __zpabi_mix__v_1
   STA   __zpabi_mix__v_0
   JSR   fin0@run1
   LDX   #>__local_f10__l16
   LDA   #<__local_f10__l16
   JSR   f5
   LDA   HARGS+16
   STA   __local_f10__l16
.if_end@1:
   LDX   #>__local_f10__l16
   LDA   #<__local_f10__l16
   JSR   f5
   LDA   __local_f10__p11
   SEC
   SBC   #$FF
   RTS

f10@run4:
   SUBROUTINE

   STA   __zpabi_mix__v_1
   JMP   fin0@run2

f18:
   SUBROUTINE

   STA   __local_f18__p19
   LDA   #$00
   STA   __local_f18__l22
   STA   __local_f18__0
   STA   __local_f18__p20_1
   STA   __local_f18__p20_0
   LDA   @15.s24
   AND   #$05
   STA   @15.s24
   LDA   ga3+2
   LDX   ga3+3
   STX   __local_f18__p20_1
   SEC
   SBC   #$05
   BCS   .carry_inc@0
   DEC   __local_f18__p20_1
.carry_inc@0:
   AND   #$FF
   STA   __local_f18__0
   LDA   __local_f18__p20_1
   AND   #$01
   LDX   __local_f18__0
   STX   __local_f18__p20_1
   EOR   #$01
   SEC
   SBC   #$01
   STA   __local_f18__0
   STA   ga3+3
   LDA   __local_f18__p20_1
   STA   ga3+2
   CLC
   ADC   g2
   STA   __local_f18__l22
   BCC   .carry_inc@1
   INC   __local_f18__0
.carry_inc@1:
   LDA   __local_f18__0
   BMI   .sx_neg@8
   LDA   #$00
   BEQ   .sx_done@9
.sx_neg@8:
   LDA   #$FF
.sx_done@9:
   JSR   fin0@run2
   LDA   __local_f18__l22
   STA   __zpabi_mix__v_0
   LDA   __local_f18__0
   JSR   fin0@run3
   LDA   @15.s24
   STA   __zpabi_f10__p12
   LDA   #$00
   STA   __zpabi_f10__p13
   LDX   #$00
   JSR   f10
   STA   __local_f18__l22
   ORA   #$00
   BMI   .sx_neg@10
   LDA   #$00
   BEQ   .sx_done@11
.sx_neg@10:
   LDA   #$FF
.sx_done@11:
   JSR   f10@run4
   LDA   __local_f18__l22
   STA   __zpabi_mix__v_0
   JSR   mix
   LDA   #$00
   STA   __local_f18__i25
   LDA   __zpabi_f18__p20_0
   STA   __local_f18__p20_0
   LDA   __zpabi_f18__p20_1
   STA   __local_f18__p20_1
.loop@3_start:
   LDA   __local_f18__i25
   CMP   #$04
   BCC   .lb_skip@0
   JMP   .loop@3_break
.lb_skip@0:
   EOR   #$FF
   STA   __local_f18__0
   LDA   #$00
   EOR   #$FF
   TAX
   LDA   @15.s24
   AND   __zpabi_f18__p21_0
   STA   __local_f18__l22
   LDA   #$00
   CMP   __local_f18__p20_1
   BNE   .cmp_true@12
   LDA   __local_f18__i25
   CMP   __local_f18__p20_0
   BNE   .cmp_true@12
   LDA   #$00
   BEQ   .cmp_end@13
.cmp_true@12:
   LDA   #$01
.cmp_end@13:
   STA   __zpabi_f10__p13
   LDA   __local_f18__l22
   STA   __zpabi_f10__p12
   LDA   __local_f18__0
   JSR   f10
   STA   __local_f18__l22
   ORA   #$00
   BMI   .sx_neg@15
   LDA   #$00
   BEQ   .sx_done@16
.sx_neg@15:
   LDA   #$FF
.sx_done@16:
   JSR   f10@run4
   LDA   __local_f18__l22
   STA   __zpabi_mix__v_0
   JSR   mix
   LDA   __local_f18__p20_0
   CLC
   ADC   #$FF
   STA   __local_f18__p20_0
   BCC   .carry_inc@2
   INC   __local_f18__p20_1
.carry_inc@2:
   LDA   #$01
   CLC
   ADC   __local_f18__i25
   STA   __local_f18__0
   LDA   #$00
   ADC   #$00
   STA   __local_f18__l22
   LDA   __local_f18__p20_0
   ORA   __local_f18__0
   STA   gs4+2
   LDA   __local_f18__p20_1
   ORA   __local_f18__l22
   STA   gs4+3
   INC   __local_f18__i25
   JMP   .loop@3_start
.loop@3_break:
   LDA   gs4+2
   STA   __local_f18__p20_1
   LDA   gs4+3
   STA   __local_f18__0
   LDA   gs4+1
   STA   __local_f18__l22
   LDA   __local_f18__0
   BNE   .cmp_true@17
   LDA   __local_f18__p20_1
   CMP   __local_f18__l22
   BNE   .cmp_true@17
   LDA   #$00
   BEQ   .cmp_end@18
.cmp_true@17:
   LDA   #$01
.cmp_end@18:
   STA   __local_f18__l22
   LDA   __local_f18__p19
   BEQ   .lnot_true@20
   LDA   #$00
   BEQ   .lnot_end@21
.lnot_true@20:
   LDA   #$01
.lnot_end@21:
   STA   __zpabi_f10__p13
   LDA   #$00
   STA   __zpabi_f10__p12
   LDX   #$00
   LDA   __local_f18__l22
   JSR   f10
   LDA   ga3+4
   BEQ   .if_end@5
   LDA   __local_f18__p19
   LSR
   LDA   #$00
   ROR
   CLC
   ADC   __local_f18__p19
   RTS
.if_end@5:
   LDA   #$01
   RTS

main:
   SUBROUTINE

   LDA   #$02
   STA   __local_main__l26
   LDA   #$00
   STA   __local_main__l27
   LDA   __local_main__l26
   STA   __local_main__2
   BMI   .sx_neg@22
   LDA   #$00
   BEQ   .sx_done@23
.sx_neg@22:
   LDA   #$FF
.sx_done@23:
   STA   __zpabi_mix__v_0
   LDA   __local_main__2
   STA   __local_main__3
   LDA   __zpabi_mix__v_0
   STA   __local_main__2
   LDA   __local_main__l26
   BEQ   .cond_else@8
   LDA   #$00
   STA   __zpabi_mix__v_0
   JMP   .cond_end@9
.cond_else@8:
   LDA   #$2E
   STA   __zpabi_mix__v_0
.cond_end@9:
   LDA   __local_main__3
   EOR   __zpabi_mix__v_0
   STA   __zpabi_mix__v_0
   LDA   __local_main__2
   STA   __zpabi_mix__v_1
   BMI   .sx_neg@24
   LDA   #$00
   BEQ   .sx_done@25
.sx_neg@24:
   LDA   #$FF
.sx_done@25:
   JSR   fin0@run2
   JSR   mix
   LDA   g2
   JSR   fin0@run0
   LDA   gs4+2
   STA   __zpabi_mix__v_1
   LDA   gs4+3
   STA   __zpabi_mix__v_0
   LDA   __zpabi_mix__v_1
   SEC
   SBC   #$5A
   STA   __zpabi_mix__v_1
   BCS   .carry_inc@3
   DEC   __zpabi_mix__v_0
.carry_inc@3:
   STA   gs4+2
   LDA   __zpabi_mix__v_0
   STA   gs4+3
   LDA   __zpabi_mix__v_1
   ORA   __zpabi_mix__v_0
   BEQ   .if_else@11
   LDX   #>__local_main__l27
   LDA   #<__local_main__l27
   JSR   f5
   JMP   .if_end@10
.if_else@11:
   LDX   #>__local_main__l27
   LDA   #<__local_main__l27
   JSR   f5
   LDA   gs4+1
   JSR   fin0@run0
.if_end@10:
   LDA   #$00
   STA   __local_main__l27
   STA   ga3+6
   STA   ga3+7
   JSR   fin0
   JSR   fin1
   LDA   acc+1
   AND   #$7F
   TAX
   LDA   acc
   RTS

acc:
   DS.B  2

g1:
   DC.W  $FFFF

ga3:
   DC.W  $0080
   DC.W  $0003
   DC.W  $FFFF
   DC.W  $007F

gs4:
   DS.B  1
   DC.B  $FF
   DC.W  $FFFF

__zp_init:
   SUBROUTINE

   LDA   #$00
   STA   g2
   STA   @15.s24
   RTS
