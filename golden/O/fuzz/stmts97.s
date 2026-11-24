SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
z5	EQU	$26
@1.s11	EQU	$27
g1	EQU	$29
g2	EQU	$2B
gp8	EQU	$2C
__zpabi_mix__v_0	EQU	$80
__zpabi_mix__v_1	EQU	$81
__zpabi_mix__v_2	EQU	$82
__zpabi_mix__v_3	EQU	$83
__local_f20__l22	EQU	$86
__local_f9__0	EQU	$86
__local_f20__0	EQU	$87
__local_f9__i13	EQU	$87
__local_f20__1	EQU	$88
__local_f20__2	EQU	$89
__local_f20__3	EQU	$8A
__local_f9__i12	EQU	$8A
__local_mix__0	EQU	$8D
__local_mix__1	EQU	$8E
__local_mix__2	EQU	$8F
__local_mix__3	EQU	$90

; @zp-link-meta-begin
; def f14 params=__zpabi_f14__p15 locals=__local_f14__0,__local_f14__1 indirect=false in_cycle=false param_regs=A
; def f20 params= locals=__local_f20__l22,__local_f20__0,__local_f20__1,__local_f20__2,__local_f20__3 indirect=false in_cycle=false
; def f9 params= locals=__local_f9__0,__local_f9__i13,__local_f9__1,__local_f9__2,__local_f9__i12 indirect=false in_cycle=false
; def fin0 params= locals=__local_fin0__0,__local_fin0__1,__local_fin0__2,__local_fin0__3 indirect=false in_cycle=false
; def fin1 params= locals=__local_fin1__0,__local_fin1__1 indirect=false in_cycle=false
; def fin2 params= locals=__local_fin2__0,__local_fin2__1 indirect=false in_cycle=false
; def main params= locals=__local_main__0,__local_main__1 indirect=false in_cycle=false
; def mix params=__zpabi_mix__v_0,__zpabi_mix__v_1,__zpabi_mix__v_2,__zpabi_mix__v_3 locals=__local_mix__0,__local_mix__1,__local_mix__2,__local_mix__3 indirect=false in_cycle=false
; call f14 -> mix
; call f20 -> f14
; call f20 -> mix
; call f9 -> mix
; call fin0 -> mix
; call fin1 -> mix
; call fin2 -> mix
; call main -> f14
; call main -> f20
; call main -> f9
; call main -> fin0
; call main -> fin1
; call main -> fin2
; call main -> mix
; item mix section=.text local=false zero=false
; item fin0 section=.text local=false zero=false
; item fin0@run4 section=.text local=true zero=false
; item fin0@run2 section=.text local=true zero=false
; item fin0@run1 section=.text local=true zero=false
; item fin0@run0 section=.text local=true zero=false
; item fin1 section=.text local=false zero=false
; item fin1@run3 section=.text local=true zero=false
; item fin2 section=.text local=false zero=false
; item f9 section=.text local=false zero=false
; item f14 section=.text local=false zero=false
; item f20 section=.text local=false zero=false
; item main section=.text local=false zero=false
; item acc section=.bss local=false zero=true
; item g3 section=.data local=false zero=false
; item g4 section=.data local=false zero=false
; item ga6 section=.data local=false zero=false
; item gs7 section=.data local=false zero=false
; item __zp_init section=.text local=false zero=false
; zp z5 size=1 at=auto def=true init=$00
; zp @1.s11 size=2 at=auto def=true init=$00,$00 weight=130 local=true
; zp g1 size=2 at=auto def=true init=$00,$00 weight=10
; zp g2 size=1 at=auto def=true init=$C5 weight=5
; zp gp8 size=2 at=auto def=true init=<g3,>g3 weight=7
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

   LDA   g1+1
   BMI   .sx_neg@0
   LDA   #$00
   BEQ   .sx_done@1
.sx_neg@0:
   LDA   #$FF
.sx_done@1:
   JSR   fin0@run1
   LDA   g1
   STA   __zpabi_mix__v_0
   LDA   g1+1
   JSR   fin0@run2
   LDA   g2
   BMI   .sx_neg@2
   LDA   #$00
   BEQ   .sx_done@3
.sx_neg@2:
   LDA   #$FF
.sx_done@3:
   STA   __zpabi_mix__v_1
   JSR   fin0@run1
   LDA   g2
   STA   __zpabi_mix__v_0
   JSR   mix
   LDA   g3
   STA   __zpabi_mix__v_0
   LDA   g3+1
   JSR   fin0@run4
   LDA   g4
   STA   __zpabi_mix__v_0
   LDA   g4+1
   STA   __zpabi_mix__v_1
   LDA   g4+2
   STA   __zpabi_mix__v_2
   LDA   g4+3
   STA   __zpabi_mix__v_3
   JMP   mix

fin0@run4:
   SUBROUTINE

   STA   __zpabi_mix__v_1
   JMP   fin0@run0

fin0@run2:
   SUBROUTINE

   STA   __zpabi_mix__v_1
   JMP   mix

fin0@run1:
   SUBROUTINE

   STA   __zpabi_mix__v_2
   STA   __zpabi_mix__v_3
   RTS

fin0@run0:
   SUBROUTINE

   LDA   #$00
   STA   __zpabi_mix__v_2
   STA   __zpabi_mix__v_3
   JMP   mix

fin1:
   SUBROUTINE

   LDA   z5
   JSR   fin1@run3
   JSR   mix
   LDA   ga6+1
   BMI   .sx_neg@4
   LDA   #$00
   BEQ   .sx_done@5
.sx_neg@4:
   LDA   #$FF
.sx_done@5:
   JSR   fin0@run1
   LDA   ga6
   STA   __zpabi_mix__v_0
   LDA   ga6+1
   JSR   fin0@run2
   LDA   ga6+3
   BMI   .sx_neg@6
   LDA   #$00
   BEQ   .sx_done@7
.sx_neg@6:
   LDA   #$FF
.sx_done@7:
   JSR   fin0@run1
   LDA   ga6+2
   STA   __zpabi_mix__v_0
   LDA   ga6+3
   JSR   fin0@run2
   LDA   ga6+5
   BMI   .sx_neg@8
   LDA   #$00
   BEQ   .sx_done@9
.sx_neg@8:
   LDA   #$FF
.sx_done@9:
   JSR   fin0@run1
   LDA   ga6+4
   STA   __zpabi_mix__v_0
   LDA   ga6+5
   STA   __zpabi_mix__v_1
   JMP   mix

fin1@run3:
   SUBROUTINE

   STA   __zpabi_mix__v_0
   LDA   #$00
   STA   __zpabi_mix__v_1
   JMP   fin0@run1

fin2:
   SUBROUTINE

   LDA   ga6+7
   BMI   .sx_neg@10
   LDA   #$00
   BEQ   .sx_done@11
.sx_neg@10:
   LDA   #$FF
.sx_done@11:
   JSR   fin0@run1
   LDA   ga6+6
   STA   __zpabi_mix__v_0
   LDA   ga6+7
   JSR   fin0@run2
   LDA   gs7+1
   STA   __zpabi_mix__v_0
   LDA   gs7+2
   JSR   fin0@run4
   LDA   gs7+3
   JSR   fin1@run3
   JMP   mix

f9:
   SUBROUTINE

   LDA   #$00
   STA   __local_f9__i12
.f9@rot@1:
   LDA   g1+1
   BMI   .sx_neg@12
   LDA   #$00
   BEQ   .sx_done@13
.sx_neg@12:
   LDA   #$FF
.sx_done@13:
   STA   __local_f9__i13
   STA   __local_f9__0
   LDA   g1
   CLC
   ADC   #$3F
   STA   __zpabi_mix__v_0
   LDA   g1+1
   ADC   #$00
   STA   __zpabi_mix__v_1
   LDA   __local_f9__i13
   ADC   #$00
   STA   __zpabi_mix__v_2
   LDA   __local_f9__0
   ADC   #$00
   STA   __zpabi_mix__v_3
   JSR   mix
   LDA   #$02
   STA   __local_f9__i13
.f9@rot@0:
   LDA   __local_f9__i12
   AND   __local_f9__i12
   LDX   #$FF
   SEC
.div_loop@14:
   INX
   SBC   #$05
   BCS   .div_loop@14
   ADC   #$05
   BEQ   .if_end@0
   LDA   @1.s11
   SEC
   SBC   #$5A
   STA   @1.s11
   BCS   .carry_inc@0
   DEC   @1.s11+1
.carry_inc@0:
.if_end@0:
   DEC   __local_f9__i13
   BNE   .f9@rot@0
   INC   __local_f9__i12
   BEQ   .f9@rot@1
   LDY   #$00
   LDA   (gp8),Y
   STA   __zpabi_mix__v_0
   INY
   LDA   (gp8),Y
   JSR   fin0@run4
   LDY   #$00
   LDA   (gp8),Y
   STA   __local_f9__0
   INY
   LDA   (gp8),Y
   STA   __local_f9__i13
   INC   __local_f9__0
   BNE   .inc_done@0
   INC   __local_f9__i13
.inc_done@0:
   LDA   __local_f9__0
   LDY   #$00
   STA   (gp8),Y
   LDA   __local_f9__i13
   INY
   STA   (gp8),Y
   LDA   __local_f9__0
   ORA   __local_f9__i13
   BEQ   .if_end@1
   LDA   @1.s11
   SEC
   SBC   #$03
   STA   @1.s11
   BCS   .carry_inc@1
   DEC   @1.s11+1
.carry_inc@1:
   AND   gs7+1
   STA   __local_f9__0
   LDA   @1.s11+1
   AND   gs7+2
   TAX
   LDA   __local_f9__0
   RTS
.if_end@1:
   LDY   #$00
   LDA   (gp8),Y
   EOR   z5
   STA   __local_f9__0
   INY
   LDA   (gp8),Y
   TAX
   LDA   __local_f9__0
   RTS

f14:
   SUBROUTINE

   LDA   gs7+3
   STA   __zpabi_mix__v_0
   LDA   #$00
   CMP   #$01
   BNE   .cmp_true@15
   LDA   __zpabi_mix__v_0
   BNE   .cmp_true@15
   LDA   #$00
   BEQ   .cmp_end@16
.cmp_true@15:
   LDA   #$01
.cmp_end@16:
   STA   __zpabi_mix__v_0
   LDA   #$00
   BPL   .sx_done@19
   LDA   #$FF
.sx_done@19:
   JSR   fin0@run1
   LDA   #$00
   STA   __zpabi_mix__v_1
   JSR   mix
   LDA   gs7+1
   AND   #$81
   STA   gs7+1
   LDA   #$00
   STA   gs7+2
   LDA   #$FF
   RTS

f20:
   SUBROUTINE

   LDA   #$00
   STA   __local_f20__l22
   STA   __local_f20__0
   STA   __local_f20__1
   LDA   #$80
   STA   __local_f20__2
   LDA   g3
   ORA   #$81
   STA   g3
   EOR   g1
   STA   z5
   LDA   gs7+3
   AND   g4
   STA   __local_f20__l22
   LDA   #$00
   STA   __local_f20__0
   STA   __local_f20__1
   STA   __local_f20__2
   LDA   g4
   AND   #$07
   CMP   #$04
   BEQ   .case@3
   CMP   #$05
   BEQ   .case@4
   JMP   .default@5
.case@3:
   LDA   g4
   JSR   f14
   STA   __local_f20__l22
   ORA   #$00
   BMI   .sx_neg@20
   LDA   #$00
   BEQ   .sx_done@21
.sx_neg@20:
   LDA   #$FF
.sx_done@21:
   STA   __zpabi_mix__v_1
   JSR   fin0@run1
   LDA   __local_f20__l22
   STA   __zpabi_mix__v_0
   JSR   mix
   LDA   g2
   SEC
   SBC   #$01
   STA   __local_f20__0
   STA   g2
   BMI   .sx_neg@22
   LDA   #$00
   BEQ   .sx_done@23
.sx_neg@22:
   LDA   #$FF
.sx_done@23:
   STA   __local_f20__l22
   LDA   __local_f20__0
   ORA   #$08
   STA   __local_f20__0
   LDA   __local_f20__l22
   BMI   .sx_neg@24
   LDA   #$00
   BEQ   .sx_done@25
.sx_neg@24:
   LDA   #$FF
.sx_done@25:
   JSR   fin0@run1
   LDA   __local_f20__0
   STA   __zpabi_mix__v_0
   LDA   __local_f20__l22
   JSR   fin0@run2
   JMP   .switch@2_break
.case@4:
   LDA   gs7+1
   STA   __local_f20__l22
   LDA   gs7+2
   ASL   __local_f20__l22
   ROL
   ASL   __local_f20__l22
   ROL
   STA   gs7+2
   LDA   __local_f20__l22
   STA   gs7+1
   STA   gs7+3
   LDA   #$00
   AND   g4
   STA   __local_f20__2
   LDA   #$00
   AND   g4+1
   STA   __local_f20__1
   LDA   #$00
   AND   g4+2
   STA   __local_f20__0
   LDA   #$00
   AND   g4+3
   STA   __local_f20__3
   DEC   gs7+3
   LDA   __local_f20__2
   ORA   __local_f20__l22
   LDX   __local_f20__3
   STX   __local_f20__l22
   ORA   __local_f20__1
   ORA   __local_f20__0
   ORA   __local_f20__l22
   BEQ   .if_end@3
   LDA   z5
   STA   __local_f20__l22
   INC   z5
   LDA   #$00
   EOR   __local_f20__l22
   LDY   #$01
   STY   __local_f20__l22
   DEY
   STA   (gp8),Y
   LDA   __local_f20__l22
   INY
   STA   (gp8),Y
   LDA   #$00
   STA   g2
.if_end@3:
.default@5:
.switch@2_break:
   LDX   #$00
   LDA   #$02
   RTS

main:
   SUBROUTINE

   LDA   gs7+3
   AND   z5
   JSR   f14
   JSR   f9
   STA   __zpabi_mix__v_0
   STX   __zpabi_mix__v_1
   JSR   fin0@run0
   JSR   f20
   STA   __zpabi_mix__v_0
   TXA
   BMI   .sx_neg@26
   LDA   #$00
   BEQ   .sx_done@27
.sx_neg@26:
   LDA   #$FF
.sx_done@27:
   JSR   fin0@run1
   STX   __zpabi_mix__v_1
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

g3:
   DC.W  $007F

g4:
   DC.L  $00000080

ga6:
   DC.W  $8000
   DC.W  $FFFF
   DC.W  $0003
   DC.W  $0002

gs7:
   DS.B  1
   DC.W  $8000
   DC.B  $08

__zp_init:
   SUBROUTINE

   LDA   #$00
   STA   g1
   STA   g1+1
   STA   z5
   STA   @1.s11
   STA   @1.s11+1
   LDA   #$C5
   STA   g2
   LDA   #<g3
   STA   gp8
   LDA   #>g3
   STA   gp8+1
   RTS
