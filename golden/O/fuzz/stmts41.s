SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
@6.s18	EQU	$26
g2	EQU	$27
__zpabi_f20__pp22_0	EQU	$80
__zpabi_f20__pp22_1	EQU	$81
__zpabi_mix__v_0	EQU	$82
__zpabi_mix__v_1	EQU	$83
__zpabi_mix__v_2	EQU	$84
__zpabi_mix__v_3	EQU	$85
__local_main__1	EQU	$87
__local_main__2	EQU	$88
__local_main__3	EQU	$89
__local_f20__4	EQU	$8A
__local_main__4	EQU	$8A
__local_f20__5	EQU	$8B
__local_main__l26	EQU	$8B
__local_f8__0	EQU	$8F
__local_f8__w13	EQU	$90
__local_f20__11	EQU	$91
__local_f8__1	EQU	$91
__local_f20__p21_1	EQU	$92
__local_f8__2	EQU	$92
__local_f20__p21_0	EQU	$93
__local_f8__3	EQU	$93
__local_f8__4	EQU	$94
__local_f8__5	EQU	$95
__local_f14__i19	EQU	$96
__local_f8__6	EQU	$96
__local_f14__2	EQU	$97
__local_f14__3	EQU	$98
__local_mix__0	EQU	$98
__local_f14__4	EQU	$99
__local_mix__1	EQU	$99
__local_mix__2	EQU	$9A
__local_mix__3	EQU	$9B

; @zp-link-meta-begin
; def f14 params= locals=__local_f14__0,__local_f14__1,__local_f14__i19,__local_f14__2,__local_f14__3,__local_f14__4 indirect=false in_cycle=false
; def f20 params=__zpabi_f20__p21_0,__zpabi_f20__p21_1,__zpabi_f20__pp22_0,__zpabi_f20__pp22_1 locals=__local_f20__0,__local_f20__1,__local_f20__2,__local_f20__3,__local_f20__4,__local_f20__5,__local_f20__6,__local_f20__7,__local_f20__8,__local_f20__9,__local_f20__10,__local_f20__11,__local_f20__p21_1,__local_f20__p21_0 indirect=false in_cycle=false param_regs=A,X,,
; def f8 params=__zpabi_f8__pp9_0,__zpabi_f8__pp9_1 locals=__local_f8__0,__local_f8__w13,__local_f8__1,__local_f8__2,__local_f8__3,__local_f8__4,__local_f8__5,__local_f8__6,__local_f8__7 indirect=false in_cycle=false param_regs=A,X
; def fin0 params= locals=__local_fin0__0,__local_fin0__1,__local_fin0__2,__local_fin0__3 indirect=false in_cycle=false
; def fin1 params= locals=__local_fin1__0,__local_fin1__1,__local_fin1__2,__local_fin1__3 indirect=false in_cycle=false
; def fin2 params= locals=__local_fin2__0,__local_fin2__1,__local_fin2__2,__local_fin2__3 indirect=false in_cycle=false
; def main params= locals=__local_main__0,__local_main__1,__local_main__2,__local_main__3,__local_main__4,__local_main__l26,__local_main__6,__local_main__7,__local_main__8 indirect=false in_cycle=false
; def mix params=__zpabi_mix__v_0,__zpabi_mix__v_1,__zpabi_mix__v_2,__zpabi_mix__v_3 locals=__local_mix__0,__local_mix__1,__local_mix__2,__local_mix__3 indirect=false in_cycle=false
; call f20 -> f14
; call f20 -> mix
; call f8 -> mix
; call fin0 -> mix
; call fin1 -> mix
; call fin2 -> mix
; call main -> f14
; call main -> f8
; call main -> fin0
; call main -> fin1
; call main -> fin2
; call main -> mix
; item mix section=.text local=false zero=false
; item fin0 section=.text local=false zero=false
; item fin0@run5 section=.text local=true zero=false
; item fin0@run3 section=.text local=true zero=false
; item fin0@tail0 section=.text local=true zero=false
; item fin1 section=.text local=false zero=false
; item fin1@run2 section=.text local=true zero=false
; item fin1@run1 section=.text local=true zero=false
; item fin2 section=.text local=false zero=false
; item fin2@run0 section=.text local=true zero=false
; item f8 section=.text local=false zero=false
; item f8@run4 section=.text local=true zero=false
; item f14 section=.text local=false zero=false
; item f20 section=.text local=false zero=false
; item main section=.text local=false zero=false
; item acc section=.bss local=false zero=true
; item g1 section=.data local=false zero=false
; item g3 section=.data local=false zero=false
; item ga4 section=.data local=false zero=false
; item gs5 section=.data local=false zero=false
; item gp6 section=.data local=false zero=false
; item gp7 section=.data local=false zero=false
; item @2.s12 section=.data local=true zero=false
; item __zp_init section=.text local=false zero=false
; zp @6.s18 size=1 at=auto def=true init=$80 weight=9 local=true
; zp g2 size=4 at=auto def=true init=$FF,$FF,$00,$00 weight=13
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

   LDA   g1
   STA   __zpabi_mix__v_0
   LDA   g1+1
   STA   __zpabi_mix__v_1
   LDA   g1+2
   STA   __zpabi_mix__v_2
   LDA   g1+3
   JSR   fin0@run3
   LDA   g2
   STA   __zpabi_mix__v_0
   LDA   g2+1
   STA   __zpabi_mix__v_1
   LDA   g2+2
   STA   __zpabi_mix__v_2
   LDA   g2+3
   JSR   fin0@run3
   LDA   g3
   STA   __zpabi_mix__v_0
   LDA   g3+1
   STA   __zpabi_mix__v_1
   LDA   g3+2
   STA   __zpabi_mix__v_2
   LDA   g3+3
   JSR   fin0@run3
   LDA   ga4
   JSR   fin0@run5
   JMP   fin0@tail0

fin0@run5:
   SUBROUTINE

   STA   __zpabi_mix__v_0
   LDA   #$00
   STA   __zpabi_mix__v_1
   STA   __zpabi_mix__v_2
   RTS

fin0@run3:
   SUBROUTINE

   STA   __zpabi_mix__v_3
   JMP   mix

fin0@tail0:
   SUBROUTINE

   STA   __zpabi_mix__v_3
   JMP   mix

fin1:
   SUBROUTINE

   LDA   ga4+1
   JSR   fin1@run1
   LDA   ga4+2
   JSR   fin1@run1
   LDA   ga4+3
   JSR   fin1@run1
   JSR   fin1@run2
   JMP   fin0@tail0

fin1@run2:
   SUBROUTINE

   LDA   gs5+1
   STA   __zpabi_mix__v_0
   LDA   gs5+2
   STA   __zpabi_mix__v_1
   LDA   gs5+3
   STA   __zpabi_mix__v_2
   LDA   gs5+4
   RTS

fin1@run1:
   SUBROUTINE

   JSR   fin0@run5
   STA   __zpabi_mix__v_3
   JMP   mix

fin2:
   SUBROUTINE

   JSR   fin2@run0
   JMP   fin0@tail0

fin2@run0:
   SUBROUTINE

   LDA   gs5+7
   STA   __zpabi_mix__v_0
   LDA   gs5+8
   STA   __zpabi_mix__v_1
   LDA   gs5+9
   STA   __zpabi_mix__v_2
   LDA   gs5+10
   RTS

f8:
   SUBROUTINE

   LDA   #$00
   STA   __local_f8__w13
   JMP   .loop@0_continue
.f8@rot@0:
   LDA   #$00
   STA   __zpabi_mix__v_0
   STA   __zpabi_mix__v_1
   STA   __zpabi_mix__v_2
   STA   __zpabi_mix__v_3
   JSR   mix
   LDA   __local_f8__0
   STA   __local_f8__w13
.loop@0_continue:
   CLC
   ADC   #$01
   STA   __local_f8__0
   LDA   __local_f8__w13
   CMP   #$02
   BCC   .f8@rot@0
   JSR   f8@run4
   STA   __local_f8__2
   LDY   #$01
   LDA   (DPTR),Y
   STA   __local_f8__1
   INY
   LDA   (DPTR),Y
   STA   __local_f8__w13
   INY
   LDA   (DPTR),Y
   STA   __local_f8__0
   LDA   #$01
   EOR   __local_f8__2
   STA   __local_f8__4
   LDA   #$00
   EOR   __local_f8__1
   STA   __local_f8__3
   LDA   #$00
   EOR   __local_f8__w13
   STA   __local_f8__2
   LDA   #$00
   EOR   __local_f8__0
   STA   __local_f8__1
   BMI   .sx_neg@0
   LDA   #$00
   BEQ   .sx_done@1
.sx_neg@0:
   LDA   #$FF
.sx_done@1:
   LDX   __local_f8__4
   STX   @2.s12
   LDX   __local_f8__3
   STX   @2.s12+1
   LDX   __local_f8__2
   STX   @2.s12+2
   LDX   __local_f8__1
   STX   @2.s12+3
   STA   @2.s12+4
   AND   #$03
   STA   @2.s12+5
   LDA   g2
   ASL
   STA   __local_f8__3
   LDA   g2+1
   ROL
   STA   __local_f8__4
   LDA   g2+2
   ROL
   STA   __local_f8__5
   LDA   g2+3
   ROL
   STA   __local_f8__6
   LDA   __local_f8__3
   CLC
   ADC   g2
   STA   __local_f8__2
   LDA   __local_f8__4
   ADC   g2+1
   STA   __local_f8__1
   LDA   __local_f8__5
   ADC   g2+2
   STA   __local_f8__w13
   LDA   __local_f8__6
   ADC   g2+3
   LDX   __local_f8__1
   STX   g2+1
   LDX   __local_f8__w13
   STX   g2+2
   STA   g2+3
   LDA   __local_f8__2
   STA   g2
   STA   ga4+2
   LDA   #$FF
   RTS

f8@run4:
   SUBROUTINE

   LDA   gp7
   STA   DPTR
   LDA   gp7+1
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   RTS

f14:
   SUBROUTINE

   LDA   ga4+2
   ASL
   ASL
   ASL
   ASL
   ASL
   ASL
   STA   ga4+2
   STA   @6.s18
   LDA   gs5+1
   STA   __local_f14__4
   LDA   gs5+2
   STA   __local_f14__3
   LDA   gs5+3
   STA   __local_f14__2
   LDA   gs5+4
   STA   __local_f14__i19
   LDA   gs5+5
   LSR   gs5+6
   ROR
   STA   gs5+5
   LDA   __local_f14__i19
   ROR
   STA   gs5+4
   LDA   __local_f14__2
   ROR
   STA   gs5+3
   LDA   __local_f14__3
   ROR
   STA   gs5+2
   LDA   __local_f14__4
   ROR
   STA   gs5+1
   LDA   #$00
   STA   __local_f14__i19
   JMP   .loop@1_start
.f14@rot@1:
   AND   #$03
   TAX
   INC   ga4,X
   LDA   ga4,X
   STA   @6.s18
   LDA   __local_f14__i19
   STA   g2
   LDA   #$00
   STA   g2+1
   STA   g2+2
   STA   g2+3
   INC   __local_f14__i19
.loop@1_start:
   LDA   __local_f14__i19
   CMP   #$04
   BCC   .f14@rot@1
   LDA   #$FF
   STA   HARGS+8
   LDY   #$FF
   STY   HARGS+9
   STY   HARGS+10
   STA   HARGS+11
   RTS

f20:
   SUBROUTINE

   STA   __local_f20__p21_0
   STX   __local_f20__p21_1
   LDY   #$00
   LDA   #$FF
   EOR   #$5A
   STA   gs5+1
   LDA   #$FF
   STA   gs5+2
   STY   gs5+3
   TYA
   STA   gs5+4
   STA   gs5+5
   STY   gs5+6
   LDA   gs5+1
   STA   __local_f20__11
   LDA   gs5+7
   EOR   #$03
   STA   __zpabi_mix__v_3
   STA   gs5+7
   LDA   __local_f20__11
   EOR   __zpabi_mix__v_3
   AND   #$07
   CMP   #$06
   BEQ   .case@3
   CMP   #$07
   BNE   .lb_skip@0
   JMP   .switch@2_break
.lb_skip@0:
   JMP   .default@5
.case@3:
   LDA   __local_f20__p21_0
   CLC
   ADC   #$01
   STA   gs5+7
   BCC   .carry_inc@0
   INC   __local_f20__p21_1
.carry_inc@0:
   LDA   __local_f20__p21_1
   STA   gs5+8
   BMI   .sx_neg@4
   TYA
   JMP   .sx_done@5
.sx_neg@4:
   LDA   #$FF
.sx_done@5:
   STA   gs5+9
   STA   gs5+10
   LDA   #$3F
   STA   g2
   LDA   #$00
   STA   g2+1
   STA   g2+2
   STA   g2+3
   JMP   .switch@2_break
.default@5:
   TXA
   BMI   .sx_neg@6
   TYA
   JMP   .sx_done@7
.sx_neg@6:
   LDA   #$FF
.sx_done@7:
   STA   __local_f20__5
   STA   __local_f20__4
   JSR   fin2@run0
   STA   __zpabi_mix__v_3
   LDA   __local_f20__p21_0
   CLC
   ADC   __zpabi_mix__v_0
   STA   __zpabi_mix__v_0
   TXA
   ADC   __zpabi_mix__v_1
   STA   __zpabi_mix__v_1
   LDA   __local_f20__5
   ADC   __zpabi_mix__v_2
   STA   __zpabi_mix__v_2
   LDA   __local_f20__4
   ADC   __zpabi_mix__v_3
   AND   #$03
   EOR   #$02
   SEC
   SBC   #$02
   JSR   fin0@run3
   LDA   g3
   SEC
   SBC   #$01
   STA   g3
   LDA   g3+1
   SBC   #$00
   STA   g3+1
   LDA   g3+2
   SBC   #$00
   STA   g3+2
   BCS   .carry_inc@1
   DEC   g3+3
.carry_inc@1:
   LDA   g3
   ORA   g3+1
   ORA   g3+2
   ORA   g3+3
   BEQ   .if_end@0
   LDA   g3
   AND   g3
   STA   g2
   LDA   g3+1
   AND   g3+1
   STA   g2+1
   LDA   g3+2
   AND   g3+2
   STA   g2+2
   LDA   g3+3
   AND   g3+3
   STA   g2+3
.if_end@0:
.switch@2_break:
   JSR   fin1@run2
   JSR   fin0@run3
   JSR   f14
   LDA   g1
   EOR   #$05
   STA   g1
   STA   HARGS
   LDA   g1+1
   STA   HARGS+1
   LDA   #$04
   STA   HARGS+2
   LDA   #$00
   STA   HARGS+3
   JSR   sdivmod16
   LDA   HARGS+6
   LDX   HARGS+7
   RTS

main:
   SUBROUTINE

   LDA   #$80
   STA   __local_main__l26
   LDA   #$00
   STA   __local_main__l26+1
   STA   __local_main__l26+2
   STA   __local_main__l26+3
   JSR   f8@run4
   STA   __local_main__3
   LDY   #$01
   LDA   (DPTR),Y
   STA   __local_main__2
   INY
   LDA   (DPTR),Y
   STA   __local_main__1
   INY
   LDA   (DPTR),Y
   BNE   .cmp_true@8
   LDA   __local_main__1
   BNE   .cmp_true@8
   LDA   __local_main__2
   BNE   .cmp_true@8
   LDA   __local_main__3
   BNE   .cmp_true@8
   LDA   #$00
   BEQ   .cmp_end@9
.cmp_true@8:
   LDA   #$01
.cmp_end@9:
   STA   __local_main__4
   LDA   #$00
   BPL   .sx_done@12
   LDA   #$FF
.sx_done@12:
   LDX   __local_main__4
   STX   g1
   LDX   #$00
   STX   g1+1
   STA   g1+2
   STA   g1+3
   STA   g1+4
   AND   #$03
   STA   g1+5
   JSR   fin2@run0
   JSR   fin0@run3
   JSR   f14
   LDX   #>__local_main__l26
   LDA   #<__local_main__l26
   JSR   f8
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
   DS.B  3
   DC.B  $80
   DS.B  2

g3:
   DC.L  $00008000

ga4:
   DC.B  $00, $01, $FF, $00

gs5:
   DS.B  2
   DC.B  $01
   DS.B  4
   DC.L  $00000001

gp6:
   DC.W  g1

gp7:
   DC.W  g3

@2.s12:
   DC.B  $7D
   DC.B  $01
   DC.B  $58
   DC.B  $1E
   DC.B  $55
   DC.B  $01

__zp_init:
   SUBROUTINE

   LDA   #$00
   STA   g2+2
   STA   g2+3
   LDA   #$FF
   STA   g2
   STA   g2+1
   LDA   #$80
   STA   @6.s18
   RTS
