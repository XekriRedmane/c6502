SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
g3	EQU	$26
g2	EQU	$28
g1	EQU	$29
__local_main__l16	EQU	$80
__zpabi_f8__p9_0	EQU	$80
__local_main__0	EQU	$81
__zpabi_f8__p9_1	EQU	$81
__local_main__1	EQU	$82
__zpabi_f8__p9_2	EQU	$82
__local_main__2	EQU	$83
__zpabi_f8__p9_3	EQU	$83
__local_main__i18	EQU	$84
__zpabi_f8__p9_4	EQU	$84
__zpabi_f8__p9_5	EQU	$85
__zpabi_f8__p9_6	EQU	$86
__zpabi_f8__p9_7	EQU	$87
__zpabi_f8__p10_0	EQU	$88
__zpabi_f8__p10_1	EQU	$89
__zpabi_f8__p10_2	EQU	$8A
__zpabi_f8__p10_3	EQU	$8B
__zpabi_f8__p11_0	EQU	$8C
__zpabi_f8__p11_1	EQU	$8D
__zpabi_f8__pp12_0	EQU	$8E
__zpabi_f8__pp12_1	EQU	$8F
__zpabi_mix__v_0	EQU	$90
__zpabi_mix__v_1	EQU	$91
__zpabi_mix__v_2	EQU	$92
__zpabi_mix__v_3	EQU	$93
__local_mix__0	EQU	$96
__local_mix__1	EQU	$97
__local_mix__2	EQU	$98
__local_mix__3	EQU	$99

; @zp-link-meta-begin
; def f8 params=__zpabi_f8__p9_0,__zpabi_f8__p9_1,__zpabi_f8__p9_2,__zpabi_f8__p9_3,__zpabi_f8__p9_4,__zpabi_f8__p9_5,__zpabi_f8__p9_6,__zpabi_f8__p9_7,__zpabi_f8__p10_0,__zpabi_f8__p10_1,__zpabi_f8__p10_2,__zpabi_f8__p10_3,__zpabi_f8__p11_0,__zpabi_f8__p11_1,__zpabi_f8__pp12_0,__zpabi_f8__pp12_1 locals=__local_f8__0,__local_f8__1 indirect=false in_cycle=false
; def fin0 params= locals=__local_fin0__0,__local_fin0__1,__local_fin0__2,__local_fin0__3 indirect=false in_cycle=false
; def fin1 params= locals=__local_fin1__0,__local_fin1__1,__local_fin1__2,__local_fin1__3 indirect=false in_cycle=false
; def fin2 params= locals=__local_fin2__0 indirect=false in_cycle=false
; def main params= locals=__local_main__l16,__local_main__0,__local_main__1,__local_main__2,__local_main__i18,__local_main__3,__local_main__4 indirect=false in_cycle=false
; def mix params=__zpabi_mix__v_0,__zpabi_mix__v_1,__zpabi_mix__v_2,__zpabi_mix__v_3 locals=__local_mix__0,__local_mix__1,__local_mix__2,__local_mix__3 indirect=false in_cycle=false
; call f8 -> mix
; call fin0 -> mix
; call fin1 -> mix
; call fin2 -> mix
; call main -> fin0
; call main -> fin1
; call main -> fin2
; item mix section=.text local=false zero=false
; item fin0 section=.text local=false zero=false
; item fin0@run1 section=.text local=true zero=false
; item fin0@tail0 section=.text local=true zero=false
; item fin1 section=.text local=false zero=false
; item fin2 section=.text local=false zero=false
; item f8 section=.text local=false zero=false
; item f8@run2 section=.text local=true zero=false
; item f8@run0 section=.text local=true zero=false
; item main section=.text local=false zero=false
; item main@run3 section=.text local=true zero=false
; item acc section=.bss local=false zero=true
; item ga4 section=.data local=false zero=false
; item gs5 section=.data local=false zero=false
; item gp6 section=.data local=false zero=false
; item gp7 section=.data local=false zero=false
; item @5.s15 section=.data local=true zero=false
; item __zp_init section=.text local=false zero=false
; zp g3 size=2 at=auto def=true init=$02,$00 weight=37
; zp g2 size=1 at=auto def=true init=$FF weight=10
; zp g1 size=4 at=auto def=true init=$00,$00,$00,$80 weight=17
; runtime hargs_size=32 hargs_need=9
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
   JSR   fin0@run1
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
   LDA   g3+1
   BMI   .sx_neg@2
   LDA   #$00
   BEQ   .sx_done@3
.sx_neg@2:
   LDA   #$FF
.sx_done@3:
   STA   __zpabi_mix__v_2
   STA   __zpabi_mix__v_3
   LDA   g3
   STA   __zpabi_mix__v_0
   LDA   g3+1
   STA   __zpabi_mix__v_1
   JSR   mix
   LDA   ga4
   STA   __zpabi_mix__v_0
   LDA   ga4+1
   STA   __zpabi_mix__v_1
   LDA   ga4+2
   STA   __zpabi_mix__v_2
   LDA   ga4+3
   JMP   fin0@tail0

fin0@run1:
   SUBROUTINE

   STA   __zpabi_mix__v_3
   JMP   mix

fin0@tail0:
   SUBROUTINE

   STA   __zpabi_mix__v_3
   JMP   mix

fin1:
   SUBROUTINE

   LDA   ga4+8
   STA   __zpabi_mix__v_0
   LDA   ga4+9
   STA   __zpabi_mix__v_1
   LDA   ga4+10
   STA   __zpabi_mix__v_2
   LDA   ga4+11
   JSR   fin0@run1
   LDA   ga4+16
   STA   __zpabi_mix__v_0
   LDA   ga4+17
   STA   __zpabi_mix__v_1
   LDA   ga4+18
   STA   __zpabi_mix__v_2
   LDA   ga4+19
   JSR   fin0@run1
   LDA   ga4+24
   STA   __zpabi_mix__v_0
   LDA   ga4+25
   STA   __zpabi_mix__v_1
   LDA   ga4+26
   STA   __zpabi_mix__v_2
   LDA   ga4+27
   JSR   fin0@run1
   LDA   gs5+1
   STA   __zpabi_mix__v_0
   LDA   gs5+2
   STA   __zpabi_mix__v_1
   LDA   #$00
   STA   __zpabi_mix__v_2
   JMP   fin0@tail0

fin2:
   SUBROUTINE

   LDA   gs5+3
   STA   __zpabi_mix__v_0
   LDA   #$00
   STA   __zpabi_mix__v_1
   STA   __zpabi_mix__v_2
   JMP   fin0@tail0

f8:
   SUBROUTINE

   JSR   f8@run2
   LDA   #$7F
   LDY   #$00
   STA   (DPTR),Y
   JSR   f8@run2
   LDA   #$00
   LDY   #$01
   STA   (DPTR),Y
   LDA   g3+1
   STA   __zpabi_mix__v_1
   ASL
   ROR   __zpabi_mix__v_1
   LDA   g3
   ROR
   STA   __zpabi_mix__v_0
   LDA   __zpabi_mix__v_1
   ASL
   ROR   __zpabi_mix__v_1
   ROR   __zpabi_mix__v_0
   LDA   __zpabi_mix__v_1
   BMI   .sx_neg@4
   LDA   #$00
   BEQ   .sx_done@5
.sx_neg@4:
   LDA   #$FF
.sx_done@5:
   STA   __zpabi_mix__v_2
   JSR   fin0@run1
   LDY   #$00
   LDA   (__zpabi_f8__pp12_0),Y
   STA   __zpabi_mix__v_0
   INY
   LDA   (__zpabi_f8__pp12_0),Y
   STA   __zpabi_mix__v_1
   LDA   __zpabi_mix__v_0
   BNE   .carry_inc@0
   DEC   __zpabi_mix__v_1
.carry_inc@0:
   DEC   __zpabi_mix__v_0
   LDA   __zpabi_mix__v_0
   LDY   #$00
   STA   (__zpabi_f8__pp12_0),Y
   LDA   __zpabi_mix__v_1
   INY
   STA   (__zpabi_f8__pp12_0),Y
   LDA   __zpabi_mix__v_0
   STA   gs5+3
   LDA   #$03
   AND   ga4+24
   AND   #$07
   CMP   #$02
   BNE   .default@6
   LDA   g3
   INC   g3
   BNE   .inc_done@0
   INC   g3+1
.inc_done@0:
   AND   #$07
   BEQ   .case@3
   CMP   #$05
   BEQ   .case@4
   BNE   .default@5
.case@3:
   LDA   @5.s15
   STA   __zpabi_mix__v_1
   LDA   @5.s15+1
   LDA   __zpabi_mix__v_1
   AND   #$01
   STA   @5.s15
   LDA   #$00
   STA   @5.s15+1
.case@4:
.default@5:
   DEC   gs5+3
.default@6:
   JSR   f8@run0
   STA   __zpabi_mix__v_0
   LDA   g2
   SEC
   SBC   __zpabi_mix__v_0
   RTS

f8@run2:
   SUBROUTINE

   LDA   gp6
   STA   DPTR
   LDA   gp6+1
   STA   DPTR+1
   RTS

f8@run0:
   SUBROUTINE

   LDA   gp7
   STA   DPTR
   LDA   gp7+1
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   RTS

main:
   SUBROUTINE

   LDA   #$FF
   STA   __local_main__l16
   LDA   #$00
   STA   __local_main__0
   STA   __local_main__1
   STA   __local_main__2
   STA   __local_main__i18
.loop@7_start:
   LDA   __local_main__i18
   CMP   #$02
   BCC   .lb_skip@0
   JMP   .loop@7_break
.lb_skip@0:
   LDA   g3
   STA   __local_main__2
   LDA   g3+1
   STA   __local_main__1
   LDA   gs5+1
   STA   HARGS
   LDA   gs5+2
   STA   HARGS+1
   LDA   #$07
   STA   HARGS+2
   LDA   #$00
   STA   HARGS+3
   JSR   udivmod16
   LDA   __local_main__2
   ORA   HARGS+4
   STA   gs5+1
   LDA   __local_main__1
   ORA   HARGS+5
   STA   gs5+2
   LDA   gs5+1
   STA   gs5+3
   LDA   g1
   ORA   g1+1
   ORA   g1+2
   ORA   g1+3
   BNE   .if_else@2
   INC   g3
   BNE   .inc_done@1
   INC   g3+1
.inc_done@1:
   LDA   __local_main__i18
   JSR   main@run3
   STA   __local_main__0
   LDA   gs5+3
   STA   __local_main__l16
   STA   HARGS
   LDA   #$00
   LDX   #$08
   LSR   HARGS
.mul_loop@6:
   BCC   .mul_skip@7
   CLC
   ADC   __local_main__l16
.mul_skip@7:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@6
   STA   __local_main__1
   LDA   __local_main__0
   SEC
   SBC   HARGS
   STA   __local_main__0
   LDA   #$00
   SBC   __local_main__1
   STA   __local_main__l16
   STA   g1+1
   LDA   __local_main__0
   STA   g1
   LDA   __local_main__l16
   BMI   .sx_neg@8
   LDA   #$00
   BEQ   .sx_done@9
.sx_neg@8:
   LDA   #$FF
.sx_done@9:
   STA   g1+2
   STA   g1+3
   JMP   .loop@7_continue
.if_else@2:
   LDA   __local_main__i18
   JSR   main@run3
   STA   g2
.loop@7_continue:
   INC   __local_main__i18
   JMP   .loop@7_start
.loop@7_break:
   LDA   #$04
   STA   __local_main__l16
   LDA   #$00
   STA   __local_main__0
   STA   __local_main__1
   STA   __local_main__2
   JSR   f8@run0
   LDY   #$00
   STA   (DPTR),Y
   JSR   fin0
   JSR   fin1
   JSR   fin2
   LDA   acc+1
   AND   #$7F
   TAX
   LDA   acc
   RTS

main@run3:
   SUBROUTINE

   AND   #$03
   ASL
   ASL
   ASL
   TAX
   LDA   ga4,X
   RTS

acc:
   DS.B  2

ga4:
   DC.L  $00007FFF
   DC.L  $00000000
   DC.L  $F2BDE933
   DC.L  $2515B67D
   DC.L  $00000002
   DC.L  $00000000
   DC.L  $EAAE183B
   DC.L  $34E02BCC

gs5:
   DS.B  1
   DC.W  $0003
   DC.B  $FF

gp6:
   DC.W  g3

gp7:
   DC.W  g2

@5.s15:
   DC.W  $0001

__zp_init:
   SUBROUTINE

   LDA   #$00
   STA   g1
   STA   g1+1
   STA   g1+2
   STA   g3+1
   LDA   #$80
   STA   g1+3
   LDA   #$FF
   STA   g2
   LDA   #$02
   STA   g3
   RTS
