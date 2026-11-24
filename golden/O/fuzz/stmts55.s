SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
z4	EQU	$26
@7.s17	EQU	$2E
@1.s11	EQU	$2F
g2	EQU	$33
gp7	EQU	$34
__local_f8__l10	EQU	$80
__zpabi_mix__v_0	EQU	$80
__local_f8__0	EQU	$81
__zpabi_mix__v_1	EQU	$81
__local_f8__1	EQU	$82
__zpabi_mix__v_2	EQU	$82
__local_f8__2	EQU	$83
__zpabi_mix__v_3	EQU	$83
__local_main__1	EQU	$85
__local_main__2	EQU	$86
__local_main__3	EQU	$87
__local_main__4	EQU	$88
__local_main__5	EQU	$89
__local_main__6	EQU	$8A
__local_main__7	EQU	$8B
__local_main__8	EQU	$8C
__local_f14__1	EQU	$8E
__local_f14__2	EQU	$8F
__local_f14__3	EQU	$90
__local_f14__w18	EQU	$91
__local_mix__0	EQU	$92
__local_mix__1	EQU	$93
__local_mix__2	EQU	$94
__local_mix__3	EQU	$95

; @zp-link-meta-begin
; def f14 params=__zpabi_f14__p15_0,__zpabi_f14__p15_1 locals=__local_f14__0,__local_f14__1,__local_f14__2,__local_f14__3,__local_f14__w18 indirect=false in_cycle=false param_regs=A,X
; def f8 params= locals=__local_f8__l10,__local_f8__0,__local_f8__1,__local_f8__2 indirect=false in_cycle=false
; def fin0 params= locals=__local_fin0__0,__local_fin0__1,__local_fin0__2,__local_fin0__3 indirect=false in_cycle=false
; def fin1 params= locals=__local_fin1__0,__local_fin1__1,__local_fin1__2,__local_fin1__3 indirect=false in_cycle=false
; def fin2 params= locals=__local_fin2__0 indirect=false in_cycle=false
; def main params= locals=__local_main__0,__local_main__1,__local_main__2,__local_main__3,__local_main__4,__local_main__5,__local_main__6,__local_main__7,__local_main__8 indirect=false in_cycle=false
; def mix params=__zpabi_mix__v_0,__zpabi_mix__v_1,__zpabi_mix__v_2,__zpabi_mix__v_3 locals=__local_mix__0,__local_mix__1,__local_mix__2,__local_mix__3 indirect=false in_cycle=false
; call f14 -> mix
; call fin0 -> mix
; call fin1 -> mix
; call fin2 -> mix
; call main -> f14
; call main -> fin0
; call main -> fin1
; call main -> fin2
; item mix section=.text local=false zero=false
; item fin0 section=.text local=false zero=false
; item fin0@run2 section=.text local=true zero=false
; item fin0@run1 section=.text local=true zero=false
; item fin1 section=.text local=false zero=false
; item fin2 section=.text local=false zero=false
; item f8 section=.text local=false zero=false
; item f8@run0 section=.text local=true zero=false
; item f14 section=.text local=false zero=false
; item main section=.text local=false zero=false
; item acc section=.bss local=false zero=true
; item g1 section=.data local=false zero=false
; item g3 section=.data local=false zero=false
; item ga5 section=.data local=false zero=false
; item gs6 section=.bss local=false zero=true
; item __zp_init section=.text local=false zero=false
; zp z4 size=8 at=auto def=true init=$02,$00,$00,$00,$00,$00,$00,$00
; zp @7.s17 size=1 at=auto def=true init=$02 weight=128 local=true
; zp @1.s11 size=4 at=auto def=true init= weight=128
; zp g2 size=1 at=auto def=true init=$FF weight=19
; zp gp7 size=2 at=auto def=true init=<g3,>g3 weight=17
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
   JSR   fin0@run1
   LDA   g2
   BMI   .sx_neg@0
   LDA   #$00
   BEQ   .sx_done@1
.sx_neg@0:
   LDA   #$FF
.sx_done@1:
   JSR   fin0@run2
   LDA   g2
   STA   __zpabi_mix__v_0
   JSR   mix
   LDA   g3
   STA   __zpabi_mix__v_0
   LDA   g3+1
   STA   __zpabi_mix__v_1
   LDA   g3+2
   STA   __zpabi_mix__v_2
   LDA   g3+3
   JSR   fin0@run1
   LDA   z4
   STA   __zpabi_mix__v_0
   LDA   z4+1
   STA   __zpabi_mix__v_1
   LDA   z4+2
   STA   __zpabi_mix__v_2
   LDA   z4+3
   STA   __zpabi_mix__v_3
   JMP   mix

fin0@run2:
   SUBROUTINE

   STA   __zpabi_mix__v_1
   STA   __zpabi_mix__v_2
   STA   __zpabi_mix__v_3
   RTS

fin0@run1:
   SUBROUTINE

   STA   __zpabi_mix__v_3
   JMP   mix

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
   BMI   .sx_neg@2
   LDA   #$00
   BEQ   .sx_done@3
.sx_neg@2:
   LDA   #$FF
.sx_done@3:
   JSR   fin0@run2
   LDA   gs6+1
   STA   __zpabi_mix__v_0
   JSR   mix
   LDA   gs6+2
   BMI   .sx_neg@4
   LDA   #$00
   BEQ   .sx_done@5
.sx_neg@4:
   LDA   #$FF
.sx_done@5:
   JSR   fin0@run2
   LDA   gs6+2
   STA   __zpabi_mix__v_0
   JMP   mix

f8:
   SUBROUTINE

   LDA   #$FF
   STA   __local_f8__l10
   STA   __local_f8__0
   JSR   f8@run0
   STA   z4+7
   LDA   gs6+1
   BMI   .sx_neg@6
   LDA   #$00
   BEQ   .sx_done@7
.sx_neg@6:
   LDA   #$FF
.sx_done@7:
   STA   __local_f8__l10
   LDA   gs6+1
   ASL
   LDA   __local_f8__l10
   ROL
   STA   __local_f8__2
   LDA   #$00
   ADC   #$FF
   EOR   #$FF
   STA   __local_f8__1
   LDA   gs6+2
   STA   __local_f8__l10
   BMI   .sx_neg@8
   LDA   #$00
   BEQ   .sx_done@9
.sx_neg@8:
   LDA   #$FF
.sx_done@9:
   ASL   __local_f8__l10
   ROL
   ASL   __local_f8__l10
   ROL
   STA   __local_f8__0
   LDA   __local_f8__2
   SEC
   SBC   __local_f8__l10
   LDA   __local_f8__1
   SBC   __local_f8__0
   BVC   .cmp_novf@10
   EOR   #$80
.cmp_novf@10:
   ASL
   LDA   #$00
   ROL
   EOR   #$01
   STA   __local_f8__l10
   LDA   #$00
   BPL   .sx_done@12
   LDA   #$FF
.sx_done@12:
   STA   HARGS+18
   STA   HARGS+19
   STA   HARGS+20
   STA   HARGS+21
   STA   HARGS+22
   STA   HARGS+23
   LDA   __local_f8__l10
   STA   HARGS+16
   LDA   #$00
   STA   HARGS+17
   RTS

f8@run0:
   SUBROUTINE

   LDA   #$00
   STA   z4
   STA   z4+1
   STA   z4+2
   STA   z4+3
   STA   z4+4
   STA   z4+5
   STA   z4+6
   RTS

f14:
   SUBROUTINE

   LDA   ga5
   LDX   ga5+1
   STX   __local_f14__2
   LDX   ga5+2
   STX   __local_f14__1
   LDX   ga5+3
   STX   __zpabi_mix__v_0
   ASL
   LDA   __local_f14__2
   ROL
   STA   ga5
   LDA   __local_f14__1
   ROL
   STA   ga5+1
   LDA   __zpabi_mix__v_0
   ROL
   STA   ga5+2
   LDA   #$00
   ROL
   STA   ga5+3
   LDA   #$00
   STA   __local_f14__w18
.loop@7_start:
   LDY   #$00
   LDA   (gp7),Y
   STA   __zpabi_mix__v_0
   LDA   #$06
   CLC
   ADC   __zpabi_mix__v_0
   AND   #$07
   CMP   #$02
   BEQ   .case@9
   CMP   #$03
   BEQ   .switch@8_break
   CMP   #$07
   BEQ   .case@13
   BNE   .default@14
.case@9:
   LDA   __local_f14__w18
   AND   #$03
   ASL
   ASL
   TAX
   LDA   ga5,X
   STA   __local_f14__3
   LDA   ga5+1,X
   STA   __local_f14__2
   LDA   ga5+2,X
   STA   __local_f14__1
   LDA   ga5+3,X
   STA   __zpabi_mix__v_0
   LDA   __local_f14__3
   ORA   __local_f14__2
   ORA   __local_f14__1
   ORA   __zpabi_mix__v_0
   BEQ   .switch@8_break
   LDA   __local_f14__w18
   BEQ   .switch@8_break
   BNE   .switch@8_break
.case@13:
.default@14:
   SEC
   LDA   #$00
   ROL
   STA   __zpabi_mix__v_0
   LDA   #$00
   BPL   .sx_done@14
   LDA   #$FF
.sx_done@14:
   STA   __zpabi_mix__v_2
   STA   __zpabi_mix__v_3
   LDA   #$00
   STA   __zpabi_mix__v_1
   JSR   mix
   LDA   gs6+2
   AND   #$07
   CMP   #$04
.switch@8_break:
   INC   __local_f14__w18
   LDA   __local_f14__w18
   CMP   #$02
   BCC   .loop@7_start
   LDA   z4+7
   BNE   .cmp_true@15
   LDA   z4+6
   BNE   .cmp_true@15
   LDA   z4+5
   BNE   .cmp_true@15
   LDA   z4+4
   BNE   .cmp_true@15
   LDA   z4+3
   BNE   .cmp_true@15
   LDA   z4+2
   BNE   .cmp_true@15
   LDA   z4+1
   BNE   .cmp_true@15
   LDA   z4
   BNE   .cmp_true@15
   LDA   #$00
   BEQ   .cmp_end@16
.cmp_true@15:
   LDA   #$01
.cmp_end@16:
   LDX   #$00
   RTS

main:
   SUBROUTINE

   SEC
   JSR   f8@run0
   LDY   #$00
   STY   z4+7
   STY   __local_main__1
   BCS   .carry_inc@1
   DEC   __local_main__1
.carry_inc@1:
   AND   #$FF
   LDX   __local_main__1
   SEC
   BCS   .carry_inc@2
   DEX
.carry_inc@2:
   JSR   f14
   LDA   g3
   STA   __local_main__8
   CLC
   ADC   #$01
   STA   __local_main__7
   LDA   g3+1
   ADC   #$00
   STA   __local_main__6
   LDA   g3+2
   ADC   #$00
   STA   __local_main__5
   LDA   g3+3
   ADC   #$00
   STA   __local_main__4
   LDA   g3+4
   ADC   #$00
   STA   __local_main__3
   LDA   g3+5
   ADC   #$00
   STA   __local_main__2
   LDA   g3+6
   ADC   #$00
   STA   __local_main__1
   LDA   g3+7
   ADC   #$00
   AND   #$1F
   EOR   #$10
   LDX   __local_main__7
   STX   g3
   LDX   __local_main__6
   STX   g3+1
   LDX   __local_main__5
   STX   g3+2
   LDX   __local_main__4
   STX   g3+3
   LDX   __local_main__3
   STX   g3+4
   LDX   __local_main__2
   STX   g3+5
   LDX   __local_main__1
   STX   g3+6
   SEC
   SBC   #$10
   STA   g3+7
   LDA   __local_main__8
   SEC
   SBC   g1
   STA   gs6+1
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
   DC.L  $00000100

g3:
   DC.L  $8E29336A
   DC.L  $FBBC97CC

ga5:
   DC.L  $00007FFF
   DC.L  $00000080
   DC.L  $0000000B
   DC.L  $00000001

gs6:
   DS.B  3

__zp_init:
   SUBROUTINE

   LDA   #$00
   STA   z4+1
   STA   z4+2
   STA   z4+3
   STA   z4+4
   STA   z4+5
   STA   z4+6
   STA   z4+7
   LDA   #$FF
   STA   g2
   LDA   #$02
   STA   z4
   STA   @7.s17
   LDA   #<g3
   STA   gp7
   LDA   #>g3
   STA   gp7+1
   RTS
