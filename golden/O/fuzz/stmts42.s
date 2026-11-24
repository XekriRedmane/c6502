SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
z5	EQU	$26
z6	EQU	$28
z7	EQU	$29
g3	EQU	$2B
g1	EQU	$2D
gp10	EQU	$2E
g4	EQU	$30
g2	EQU	$34
__zpabi_f11__p13_0	EQU	$80
__zpabi_f11__p13_1	EQU	$81
__zpabi_f11__p14	EQU	$82
__zpabi_f11__pp15_0	EQU	$83
__zpabi_f11__pp15_1	EQU	$84
__zpabi_mix__v_0	EQU	$85
__zpabi_mix__v_1	EQU	$86
__zpabi_mix__v_2	EQU	$87
__zpabi_mix__v_3	EQU	$88
__local_main__0	EQU	$89
__local_main__1	EQU	$8A
__local_main__2	EQU	$8B
__local_main__4	EQU	$8D
__local_main__l21	EQU	$8E
__local_f11__w20	EQU	$8F
__local_f11__p14	EQU	$90
__local_f11__0	EQU	$91
__local_f11__1	EQU	$92
__local_f11__2	EQU	$93
__local_f11__p12	EQU	$94
__local_f11__l16	EQU	$95
__local_f11__w19	EQU	$96
__local_mix__0	EQU	$98
__local_mix__1	EQU	$99
__local_mix__2	EQU	$9A
__local_mix__3	EQU	$9B

; @zp-link-meta-begin
; def f11 params=__zpabi_f11__p12,__zpabi_f11__p13_0,__zpabi_f11__p13_1,__zpabi_f11__p14,__zpabi_f11__pp15_0,__zpabi_f11__pp15_1 locals=__local_f11__w20,__local_f11__p14,__local_f11__0,__local_f11__1,__local_f11__2,__local_f11__p12,__local_f11__l16,__local_f11__w19,__local_f11__3 indirect=false in_cycle=false param_regs=A,,,,,
; def fin0 params= locals=__local_fin0__0,__local_fin0__1 indirect=false in_cycle=false
; def fin1 params= locals=__local_fin1__0,__local_fin1__1 indirect=false in_cycle=false
; def fin2 params= locals=__local_fin2__0,__local_fin2__1,__local_fin2__2,__local_fin2__3 indirect=false in_cycle=false
; def fin3 params= locals=__local_fin3__0,__local_fin3__1,__local_fin3__2,__local_fin3__3 indirect=false in_cycle=false
; def main params= locals=__local_main__0,__local_main__1,__local_main__2,__local_main__3,__local_main__4,__local_main__l21 indirect=false in_cycle=false
; def mix params=__zpabi_mix__v_0,__zpabi_mix__v_1,__zpabi_mix__v_2,__zpabi_mix__v_3 locals=__local_mix__0,__local_mix__1,__local_mix__2,__local_mix__3 indirect=false in_cycle=false
; call f11 -> mix
; call fin0 -> mix
; call fin1 -> mix
; call fin2 -> mix
; call fin3 -> mix
; call main -> f11
; call main -> fin0
; call main -> fin1
; call main -> fin2
; call main -> fin3
; call main -> mix
; item mix section=.text local=false zero=false
; item fin0 section=.text local=false zero=false
; item fin0@run1 section=.text local=true zero=false
; item fin0@tail0 section=.text local=true zero=false
; item fin1 section=.text local=false zero=false
; item fin1@run0 section=.text local=true zero=false
; item fin2 section=.text local=false zero=false
; item fin3 section=.text local=false zero=false
; item f11 section=.text local=false zero=false
; item main section=.text local=false zero=false
; item acc section=.bss local=false zero=true
; item ga8 section=.data local=false zero=false
; item gs9 section=.data local=false zero=false
; item __zp_init section=.text local=false zero=false
; zp z5 size=2 at=auto def=true init=$7F,$00
; zp z6 size=1 at=auto def=true init=$03
; zp z7 size=2 at=auto def=true init=$02,$00
; zp g3 size=2 at=auto def=true init=$00,$00 weight=26
; zp g1 size=1 at=auto def=true init=$00 weight=12
; zp gp10 size=2 at=auto def=true init=<z5,>z5 weight=13
; zp g4 size=4 at=auto def=true init=$02,$00,$00,$00 weight=18
; zp g2 size=1 at=auto def=true init=$00 weight=3
; runtime hargs_size=32 hargs_need=11
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
   BMI   .sx_neg@0
   LDA   #$00
   BEQ   .sx_done@1
.sx_neg@0:
   LDA   #$FF
.sx_done@1:
   STA   __zpabi_mix__v_1
   STA   __zpabi_mix__v_2
   STA   __zpabi_mix__v_3
   JSR   mix
   LDA   g2
   JSR   fin0@run1
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
   LDA   g4
   STA   __zpabi_mix__v_0
   LDA   g4+1
   STA   __zpabi_mix__v_1
   LDA   g4+2
   STA   __zpabi_mix__v_2
   LDA   g4+3
   JMP   fin0@tail0

fin0@run1:
   SUBROUTINE

   STA   __zpabi_mix__v_0
   LDA   #$00
   STA   __zpabi_mix__v_1
   STA   __zpabi_mix__v_2
   STA   __zpabi_mix__v_3
   JMP   mix

fin0@tail0:
   SUBROUTINE

   STA   __zpabi_mix__v_3
   JMP   mix

fin1:
   SUBROUTINE

   LDA   z5+1
   BMI   .sx_neg@4
   LDA   #$00
   BEQ   .sx_done@5
.sx_neg@4:
   LDA   #$FF
.sx_done@5:
   STA   __zpabi_mix__v_2
   STA   __zpabi_mix__v_3
   LDA   z5
   STA   __zpabi_mix__v_0
   LDA   z5+1
   STA   __zpabi_mix__v_1
   JSR   mix
   LDA   z6
   JSR   fin0@run1
   LDA   z7
   STA   __zpabi_mix__v_0
   LDA   z7+1
   JSR   fin1@run0
   LDA   ga8
   STA   __zpabi_mix__v_0
   LDA   ga8+1
   STA   __zpabi_mix__v_1
   LDA   #$00
   STA   __zpabi_mix__v_2
   JMP   fin0@tail0

fin1@run0:
   SUBROUTINE

   STA   __zpabi_mix__v_1
   LDA   #$00
   STA   __zpabi_mix__v_2
   STA   __zpabi_mix__v_3
   JMP   mix

fin2:
   SUBROUTINE

   LDA   ga8+2
   STA   __zpabi_mix__v_0
   LDA   ga8+3
   JSR   fin1@run0
   LDA   ga8+4
   STA   __zpabi_mix__v_0
   LDA   ga8+5
   JSR   fin1@run0
   LDA   ga8+6
   STA   __zpabi_mix__v_0
   LDA   ga8+7
   JSR   fin1@run0
   LDA   gs9+1
   STA   __zpabi_mix__v_0
   LDA   gs9+2
   STA   __zpabi_mix__v_1
   LDA   gs9+3
   STA   __zpabi_mix__v_2
   LDA   gs9+4
   JMP   fin0@tail0

fin3:
   SUBROUTINE

   LDA   gs9+9
   STA   __zpabi_mix__v_0
   LDA   gs9+10
   STA   __zpabi_mix__v_1
   LDA   gs9+11
   STA   __zpabi_mix__v_2
   LDA   gs9+12
   JMP   fin0@tail0

f11:
   SUBROUTINE

   STA   __local_f11__p12
   LDA   __zpabi_f11__p13_0
   LSR
   LDA   #$00
   ROR
   EOR   #$02
   STA   g1
   LDA   __zpabi_f11__p14
   ORA   #$5A
   STA   __local_f11__p14
   AND   #$01
   JSR   fin0@run1
   LDA   #$03
   STA   __local_f11__l16
   LDY   #$00
   STY   __local_f11__w19
.loop@0_start:
   LDA   __local_f11__l16
   BMI   .sx_neg@8
   LDA   #$00
   BEQ   .sx_done@9
.sx_neg@8:
   LDA   #$FF
.sx_done@9:
   STA   HARGS+1
   LDA   __local_f11__l16
   STA   HARGS
   LDA   #$05
   STA   HARGS+2
   LDA   #$00
   STA   HARGS+3
   JSR   sdivmod16
   LDA   HARGS+4
   ORA   HARGS+5
   BEQ   .if_end@0
   LDA   #$01
   ORA   __local_f11__p14
   AND   #$07
   BEQ   .switch@1_break
   CMP   #$04
   BEQ   .switch@1_break
   LDA   g3
   STA   __local_f11__l16
   BNE   .carry_inc@0
   DEC   g3+1
.carry_inc@0:
   DEC   g3
   INC   __local_f11__p12
.switch@1_break:
   LDA   g4
   CLC
   ADC   #$01
   STA   __local_f11__2
   LDA   g4+1
   ADC   #$00
   STA   __local_f11__1
   LDA   g4+2
   ADC   #$00
   STA   __local_f11__0
   LDA   g4+3
   ADC   #$00
   LDX   __local_f11__2
   STX   g4
   LDX   __local_f11__1
   STX   g4+1
   LDX   __local_f11__0
   STX   g4+2
   STA   g4+3
   LDA   #$7F
   CLC
   ADC   __local_f11__2
   STA   __local_f11__0
   LDA   #$00
   ADC   __local_f11__1
   STA   __local_f11__w20
   LDA   __local_f11__0
   LDY   #$00
   STA   (gp10),Y
   LDA   __local_f11__w20
   INY
   STA   (gp10),Y
.if_end@0:
   INC   __local_f11__w19
   LDA   __local_f11__w19
   CMP   #$02
   BCS   .lb_skip@0
   JMP   .loop@0_start
.lb_skip@0:
   LDA   g2
   ASL
   ASL
   STA   g2
   STA   z5
   LDA   #$00
   STA   z5+1
   STA   __local_f11__w20
.loop@5_start:
   LDA   __local_f11__p14
   EOR   #$03
   STA   __local_f11__p14
   STA   g1
   INC   __local_f11__w20
   LDA   __local_f11__w20
   CMP   #$03
   BCC   .loop@5_start
   LDA   __local_f11__l16
   BMI   .sx_neg@10
   LDA   #$00
   BEQ   .sx_done@11
.sx_neg@10:
   LDA   #$FF
.sx_done@11:
   TAX
   LDA   #$01
   ORA   __local_f11__l16
   RTS

main:
   SUBROUTINE

   LDA   #$FF
   STA   __local_main__l21
   LDY   #$00
   LDA   (gp10),Y
   ORA   z7
   STA   __local_main__1
   INY
   LDA   (gp10),Y
   ORA   z7+1
   STA   g4+1
   LDA   __local_main__1
   STA   g4
   LDA   z7
   STA   gs9+1
   LDA   z7+1
   STA   gs9+2
   LDA   #$00
   STA   g4+2
   STA   g4+3
   STA   gs9+3
   DEY
   STY   gs9+4
   STY   gs9+5
   STY   gs9+6
   STY   gs9+7
   STA   gs9+8
   LDA   g3
   EOR   #$FF
   STA   __local_main__4
   LDA   g1
   STA   __zpabi_f11__p13_0
   BMI   .sx_neg@12
   TYA
   JMP   .sx_done@13
.sx_neg@12:
   LDA   #$FF
.sx_done@13:
   STA   __zpabi_f11__p13_1
   LDA   #$FF
   STA   __zpabi_f11__p14
   LDA   #<__local_main__l21
   STA   __zpabi_f11__pp15_0
   LDA   #>__local_main__l21
   STA   __zpabi_f11__pp15_1
   LDA   __local_main__4
   JSR   f11
   INC   ga8
   BNE   .inc_done@0
   INC   ga8+1
.inc_done@0:
   LDA   g1
   STA   __zpabi_f11__p13_0
   BMI   .sx_neg@14
   LDA   #$00
   BEQ   .sx_done@15
.sx_neg@14:
   LDA   #$FF
.sx_done@15:
   STA   __local_main__2
   STA   __local_main__1
   STA   __local_main__0
   LDA   #$00
   EOR   __zpabi_f11__p13_0
   STA   __zpabi_mix__v_0
   LDA   #$00
   EOR   __local_main__2
   STA   __zpabi_mix__v_1
   LDA   #$01
   EOR   __local_main__1
   STA   __zpabi_mix__v_2
   LDA   #$00
   EOR   __local_main__0
   STA   __zpabi_mix__v_3
   JSR   mix
   LDA   z7
   AND   #$81
   STA   z7
   LDA   #$00
   STA   z7+1
   ORA   z7
   BEQ   .if_end@1
   LDY   #$00
   LDA   (gp10),Y
   STA   __local_main__0
   INY
   LDA   (gp10),Y
   STA   __local_main__1
   LDX   #$04
.shift_loop@16:
   LDA   __local_main__1
   ASL
   ROR   __local_main__1
   ROR   __local_main__0
   DEX
   BNE   .shift_loop@16
   LDA   __local_main__0
   LDY   #$00
   STA   (gp10),Y
   LDA   __local_main__1
   INY
   STA   (gp10),Y
   LDA   __local_main__0
   STA   __local_main__l21
.if_end@1:
   JSR   fin0
   JSR   fin1
   JSR   fin2
   JSR   fin3
   LDA   acc+1
   AND   #$7F
   TAX
   LDA   acc
   RTS

acc:
   DS.B  2

ga8:
   DC.W  $007F
   DS.B  2
   DC.W  $FFFF
   DC.W  $7FFF

gs9:
   DS.B  1
   DC.L  $0000000C
   DC.L  $00000000
   DC.B  $C9
   DC.B  $FC
   DC.B  $EF
   DC.B  $11
   DC.B  $9C
   DC.B  $03

__zp_init:
   SUBROUTINE

   LDA   #$00
   STA   g1
   STA   g2
   STA   g3
   STA   g3+1
   STA   g4+1
   STA   g4+2
   STA   g4+3
   STA   z5+1
   STA   z7+1
   LDA   #$02
   STA   g4
   STA   z7
   LDA   #$7F
   STA   z5
   LDA   #$03
   STA   z6
   LDA   #<z5
   STA   gp10
   LDA   #>z5
   STA   gp10+1
   RTS
