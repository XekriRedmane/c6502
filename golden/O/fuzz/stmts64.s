SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
z4	EQU	$26
z5	EQU	$2E
g3	EQU	$36
g2	EQU	$39
gp8	EQU	$3B
g1	EQU	$3D
__zpabi_f10__p12_0	EQU	$80
__zpabi_f10__p12_1	EQU	$81
__zpabi_mix__v_0	EQU	$82
__zpabi_mix__v_1	EQU	$83
__zpabi_mix__v_2	EQU	$84
__zpabi_mix__v_3	EQU	$85
__local_main__w23	EQU	$86
__local_main__0	EQU	$87
__local_main__1	EQU	$88
__local_main__2	EQU	$89
__local_main__5	EQU	$8C
__local_main__l22_1	EQU	$8D
__local_main__l22_0	EQU	$8E
__local_main__l21_3	EQU	$8F
__local_main__l21_2	EQU	$90
__local_main__l21_1	EQU	$91
__local_main__l21_0	EQU	$92
__local_f10__0	EQU	$94
__local_f10__i18	EQU	$95
__local_f10__1	EQU	$96
__local_f10__i16	EQU	$98
__local_f10__2	EQU	$99
__local_f10__l15	EQU	$9A
__local_mix__0	EQU	$9B
__local_mix__1	EQU	$9C
__local_mix__2	EQU	$9D
__local_mix__3	EQU	$9E

; @zp-link-meta-begin
; def f10 params=__zpabi_f10__p11_0,__zpabi_f10__p11_1,__zpabi_f10__p12_0,__zpabi_f10__p12_1 locals=__local_f10__0,__local_f10__i18,__local_f10__1,__local_f10__i17,__local_f10__i16,__local_f10__2,__local_f10__l15 indirect=false in_cycle=false param_regs=A,X,,
; def fin0 params= locals=__local_fin0__0,__local_fin0__1,__local_fin0__2,__local_fin0__3 indirect=false in_cycle=false
; def fin1 params= locals=__local_fin1__0,__local_fin1__1,__local_fin1__2,__local_fin1__3 indirect=false in_cycle=false
; def fin2 params= locals=__local_fin2__0,__local_fin2__1 indirect=false in_cycle=false
; def main params= locals=__local_main__w23,__local_main__0,__local_main__1,__local_main__2,__local_main__3,__local_main__4,__local_main__5,__local_main__l22_1,__local_main__l22_0,__local_main__l21_3,__local_main__l21_2,__local_main__l21_1,__local_main__l21_0,__local_main__l20 indirect=false in_cycle=false
; def mix params=__zpabi_mix__v_0,__zpabi_mix__v_1,__zpabi_mix__v_2,__zpabi_mix__v_3 locals=__local_mix__0,__local_mix__1,__local_mix__2,__local_mix__3 indirect=false in_cycle=false
; call f10 -> mix
; call fin0 -> mix
; call fin1 -> mix
; call fin2 -> mix
; call main -> f10
; call main -> fin0
; call main -> fin1
; call main -> fin2
; call main -> mix
; item mix section=.text local=false zero=false
; item fin0 section=.text local=false zero=false
; item fin0@run3 section=.text local=true zero=false
; item fin0@run2 section=.text local=true zero=false
; item fin0@run1 section=.text local=true zero=false
; item fin0@tail0 section=.text local=true zero=false
; item fin1 section=.text local=false zero=false
; item fin1@run4 section=.text local=true zero=false
; item fin1@run0 section=.text local=true zero=false
; item fin2 section=.text local=false zero=false
; item f10 section=.text local=false zero=false
; item main section=.text local=false zero=false
; item acc section=.bss local=false zero=true
; item ga6 section=.data local=false zero=false
; item gs7 section=.data local=false zero=false
; item gp9 section=.data local=false zero=false
; item __zp_init section=.text local=false zero=false
; zp z4 size=8 at=auto def=true init=$00,$01,$00,$00,$00,$00,$00,$00
; zp z5 size=8 at=auto def=true init=$FF,$00,$00,$00,$00,$00,$00,$00
; zp g3 size=3 at=auto def=true init=$7F,$00,$00 weight=137
; zp g2 size=2 at=auto def=true init=$02,$00 weight=18
; zp gp8 size=2 at=auto def=true init=<g3,>g3 weight=16
; zp g1 size=4 at=auto def=true init=$80,$00,$00,$00 weight=18
; runtime hargs_size=32 hargs_need=0
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
   STA   __zpabi_mix__v_0
   LDA   g2+1
   JSR   fin0@run2
   LDA   g3
   STA   __zpabi_mix__v_0
   LDA   g3+1
   STA   __zpabi_mix__v_1
   LDA   g3+2
   JSR   fin0@run3
   LDA   z4
   STA   __zpabi_mix__v_0
   LDA   z4+1
   STA   __zpabi_mix__v_1
   LDA   z4+2
   STA   __zpabi_mix__v_2
   LDA   z4+3
   JMP   fin0@tail0

fin0@run3:
   SUBROUTINE

   STA   __zpabi_mix__v_2
   LDA   #$00
   STA   __zpabi_mix__v_3
   JMP   mix

fin0@run2:
   SUBROUTINE

   STA   __zpabi_mix__v_1
   LDA   #$00
   STA   __zpabi_mix__v_2
   JMP   fin0@run1

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

   LDA   z5
   STA   __zpabi_mix__v_0
   LDA   z5+1
   STA   __zpabi_mix__v_1
   LDA   z5+2
   STA   __zpabi_mix__v_2
   LDA   z5+3
   JSR   fin0@run1
   LDA   ga6
   JSR   fin1@run0
   LDA   ga6+1
   JSR   fin1@run0
   LDA   ga6+2
   JSR   fin1@run4
   JMP   fin0@tail0

fin1@run4:
   SUBROUTINE

   STA   __zpabi_mix__v_0
   LDA   #$00
   STA   __zpabi_mix__v_1
   STA   __zpabi_mix__v_2
   RTS

fin1@run0:
   SUBROUTINE

   JSR   fin1@run4
   STA   __zpabi_mix__v_3
   JMP   mix

fin2:
   SUBROUTINE

   LDA   ga6+3
   JSR   fin1@run0
   LDA   gs7+1
   STA   __zpabi_mix__v_0
   LDA   gs7+2
   JSR   fin0@run2
   LDA   gs7+3
   STA   __zpabi_mix__v_0
   LDA   gs7+4
   STA   __zpabi_mix__v_1
   LDA   #$00
   STA   __zpabi_mix__v_2
   JMP   fin0@tail0

f10:
   SUBROUTINE

   LDA   #$01
   STA   __local_f10__l15
   LDA   __zpabi_f10__p12_1
   BMI   .sx_neg@0
   LDA   #$00
   BEQ   .sx_done@1
.sx_neg@0:
   LDA   #$FF
.sx_done@1:
   STA   __local_f10__2
   LDA   #$00
   STA   __local_f10__i16
.loop@0_start:
   LDA   __local_f10__i16
   CMP   #$04
   BCS   .loop@0_break
   LDX   #$03
.f10@rot@0:
   LDA   g3
   AND   #$01
   STA   g3
   LDA   #$00
   STA   g3+1
   STA   g3+2
   LDA   __zpabi_f10__p12_0
   STA   __local_f10__1
   LDA   __zpabi_f10__p12_1
   STA   __local_f10__i18
   LDA   __local_f10__2
   AND   #$07
   STA   __local_f10__0
   LDA   g3
   AND   __local_f10__1
   STA   __local_f10__1
   LDA   #$00
   AND   __local_f10__i18
   STA   __local_f10__i18
   LDA   #$00
   AND   __local_f10__0
   STA   __local_f10__0
   LDA   __local_f10__1
   ORA   __local_f10__i18
   ORA   __local_f10__0
   BEQ   .if_end@0
   LDA   gs7+3
   AND   #$7F
   RTS
.if_end@0:
   DEX
   BNE   .f10@rot@0
   LDA   gs7+1
   STA   __local_f10__i18
   LDA   __local_f10__i16
   AND   __zpabi_f10__p12_0
   AND   __local_f10__i18
   JSR   fin1@run0
   INC   __local_f10__i16
   JMP   .loop@0_start
.loop@0_break:
   LDA   gs7+3
   ORA   #$03
   STA   gs7+3
   AND   #$07
   CMP   #$01
   BEQ   .case@3
   CMP   #$05
   BEQ   .case@4
   CMP   #$07
   BEQ   .case@7
   BNE   .switch@2_break
.case@3:
   LDA   #$A8
   STA   __zpabi_mix__v_0
   LDA   #$AB
   STA   __zpabi_mix__v_1
   LDA   #$FF
   STA   __zpabi_mix__v_2
   JSR   fin0@run1
   JMP   .switch@2_break
.case@4:
   LDA   #$01
   STA   __local_f10__i18
.f10@rot@1:
   LDA   __local_f10__l15
   SEC
   SBC   #$01
   STA   __local_f10__l15
   DEC   __local_f10__i18
   BNE   .f10@rot@1
.case@7:
.switch@2_break:
   LDA   #$7F
   RTS

main:
   SUBROUTINE

   LDA   #$20
   STA   g1+1
   LDA   #$01
   STA   __local_main__l21_1
   LDA   #$FF
   STA   __local_main__l22_0
   STA   __local_main__l22_1
   LDA   #$00
   STA   g1
   STA   g1+2
   STA   g1+3
   STA   __local_main__l21_0
   STA   __local_main__l21_2
   STA   __local_main__l21_3
   STA   __local_main__w23
.loop@8_continue:
   CLC
   ADC   #$01
   STA   __local_main__5
   LDA   __local_main__w23
   CMP   #$03
   BCC   .lb_skip@0
   JMP   .loop@8_break
.lb_skip@0:
   LDA   __local_main__l21_0
   SEC
   SBC   #$5A
   STA   __local_main__l21_0
   LDA   __local_main__l21_1
   SBC   #$00
   STA   __local_main__l21_1
   LDA   __local_main__l21_2
   SBC   #$00
   STA   __local_main__l21_2
   BCS   .carry_inc@0
   DEC   __local_main__l21_3
.carry_inc@0:
   LDA   #$00
   BPL   .sx_done@3
   LDA   #$FF
.sx_done@3:
   STA   __local_main__w23
   LDA   __local_main__l21_0
   STA   gs7+3
   LDA   __local_main__l21_1
   ORA   __local_main__w23
   STA   gs7+4
   LDY   #$02
   LDA   (gp8),Y
   LSR
   DEY
   LDA   (gp8),Y
   ROR
   DEY
   LDA   (gp8),Y
   ROR
   LDA   #$00
   ROR
   AND   #$07
   CMP   #$02
   LDA   __local_main__5
   BEQ   .if_end@5
   LDA   g2
   ASL
   STA   __local_main__1
   LDA   g2+1
   ROL
   ASL   __local_main__1
   ROL
   STA   __local_main__2
   LDA   __local_main__1
   CLC
   ADC   g2
   STA   __local_main__0
   LDA   __local_main__2
   ADC   g2+1
   STA   __local_main__w23
   STA   g2+1
   LDA   __local_main__0
   STA   g2
   EOR   g3
   STA   __zpabi_mix__v_0
   LDA   __local_main__w23
   EOR   g3+1
   STA   __zpabi_mix__v_1
   TYA
   EOR   g3+2
   JSR   fin0@run3
.if_end@5:
   LDA   __local_main__l21_0
   EOR   __local_main__5
   LDX   __local_main__l21_1
   STX   __local_main__1
   LDX   __local_main__l21_2
   STX   __local_main__0
   LDX   __local_main__l21_3
   STX   __local_main__w23
   ORA   __local_main__1
   ORA   __local_main__0
   ORA   __local_main__w23
   BEQ   .if_end@6
   INC   __local_main__l22_0
   BNE   .inc_done@0
   INC   __local_main__l22_1
.inc_done@0:
   LDA   __local_main__l22_0
   ORA   __local_main__l22_1
   BNE   .loop@8_break
   LDA   __local_main__l22_0
   STA   __zpabi_f10__p12_0
   LDA   __local_main__l22_1
   STA   __zpabi_f10__p12_1
   LDX   #$00
   LDA   #$40
   JSR   f10
.if_end@6:
   LDA   __local_main__5
   STA   __local_main__w23
   JMP   .loop@8_continue
.loop@8_break:
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

ga6:
   DC.B  $01, $00, $00, $00

gs7:
   DS.B  1
   DC.W  $8000
   DC.W  $7FFF

gp9:
   DC.W  g2

__zp_init:
   SUBROUTINE

   LDA   #$00
   STA   g1+1
   STA   g1+2
   STA   g1+3
   STA   g2+1
   STA   g3+1
   STA   g3+2
   STA   z4
   STA   z4+2
   STA   z4+3
   STA   z4+4
   STA   z4+5
   STA   z4+6
   STA   z4+7
   STA   z5+1
   STA   z5+2
   STA   z5+3
   STA   z5+4
   STA   z5+5
   STA   z5+6
   STA   z5+7
   LDA   #$80
   STA   g1
   LDA   #$02
   STA   g2
   LDA   #$7F
   STA   g3
   LDA   #$01
   STA   z4+1
   LDA   #$FF
   STA   z5
   LDA   #<g3
   STA   gp8
   LDA   #>g3
   STA   gp8+1
   RTS
