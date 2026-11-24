SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
z5	EQU	$26
z6	EQU	$2C
z7	EQU	$30
g4	EQU	$31
@3.s15	EQU	$32
g1	EQU	$33
gp10	EQU	$35
__zpabi_f11__p13	EQU	$80
__zpabi_mix__v_0	EQU	$81
__zpabi_mix__v_1	EQU	$82
__zpabi_mix__v_2	EQU	$83
__zpabi_mix__v_3	EQU	$84
__local_main__l17	EQU	$85
__local_f11__p13	EQU	$86
__local_main__i20	EQU	$86
__local_main__0	EQU	$87
__local_main__1	EQU	$88
__local_main__2	EQU	$89
__local_f11__2	EQU	$8A
__local_main__3	EQU	$8A
__local_main__4	EQU	$8B
__local_main__5	EQU	$8C
__local_main__8	EQU	$8F
__local_main__9	EQU	$90
__local_main__10	EQU	$91
__local_main__11	EQU	$92
__local_mix__0	EQU	$98
__local_mix__1	EQU	$99
__local_mix__2	EQU	$9A
__local_mix__3	EQU	$9B

; @zp-link-meta-begin
; def f11 params=__zpabi_f11__p12_0,__zpabi_f11__p12_1,__zpabi_f11__p13 locals=__local_f11__p12_1,__local_f11__p13,__local_f11__w16,__local_f11__0,__local_f11__1,__local_f11__2 indirect=false in_cycle=false param_regs=A,X,
; def fin0 params= locals=__local_fin0__0,__local_fin0__1,__local_fin0__2,__local_fin0__3 indirect=false in_cycle=false
; def fin1 params= locals=__local_fin1__0,__local_fin1__1,__local_fin1__2,__local_fin1__3 indirect=false in_cycle=false
; def fin2 params= locals=__local_fin2__0,__local_fin2__1,__local_fin2__2,__local_fin2__3 indirect=false in_cycle=false
; def fin3 params= locals=__local_fin3__0,__local_fin3__1 indirect=false in_cycle=false
; def main params= locals=__local_main__l17,__local_main__i20,__local_main__0,__local_main__1,__local_main__2,__local_main__3,__local_main__4,__local_main__5,__local_main__6,__local_main__7,__local_main__8,__local_main__9,__local_main__10,__local_main__11,__local_main__12 indirect=false in_cycle=false
; def mix params=__zpabi_mix__v_0,__zpabi_mix__v_1,__zpabi_mix__v_2,__zpabi_mix__v_3 locals=__local_mix__0,__local_mix__1,__local_mix__2,__local_mix__3 indirect=false in_cycle=false
; call f11 -> mix
; call fin0 -> mix
; call fin1 -> mix
; call fin2 -> mix
; call fin3 -> mix
; call main -> fin0
; call main -> fin1
; call main -> fin2
; call main -> fin3
; call main -> mix
; item mix section=.text local=false zero=false
; item fin0 section=.text local=false zero=false
; item fin0@run2 section=.text local=true zero=false
; item fin0@run0 section=.text local=true zero=false
; item fin0@tail0 section=.text local=true zero=false
; item fin1 section=.text local=false zero=false
; item fin2 section=.text local=false zero=false
; item fin2@run1 section=.text local=true zero=false
; item fin3 section=.text local=false zero=false
; item f11 section=.text local=false zero=false
; item main section=.text local=false zero=false
; item acc section=.bss local=false zero=true
; item g2 section=.data local=false zero=false
; item g3 section=.data local=false zero=false
; item ga8 section=.data local=false zero=false
; item gs9 section=.data local=false zero=false
; item __zp_init section=.text local=false zero=false
; zp z5 size=6 at=auto def=true init=$01,$00,$00,$00,$00,$00
; zp z6 size=4 at=auto def=true init=$02,$00,$00,$00
; zp z7 size=1 at=auto def=true init=$80
; zp g4 size=1 at=auto def=true init=$01 weight=18
; zp @3.s15 size=1 at=auto def=true init=$03 weight=16 local=true
; zp g1 size=2 at=auto def=true init=$7F,$00 weight=27
; zp gp10 size=2 at=auto def=true init=<g2,>g2 weight=8
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
   STA   __zpabi_mix__v_2
   STA   __zpabi_mix__v_3
   LDA   g1
   STA   __zpabi_mix__v_0
   LDA   g1+1
   STA   __zpabi_mix__v_1
   JSR   mix
   LDA   g2
   STA   __zpabi_mix__v_0
   LDA   g2+1
   STA   __zpabi_mix__v_1
   LDA   g2+2
   STA   __zpabi_mix__v_2
   LDA   g2+3
   JSR   fin0@run0
   LDA   g3
   STA   __zpabi_mix__v_0
   LDA   g3+1
   STA   __zpabi_mix__v_1
   LDA   #$00
   STA   __zpabi_mix__v_2
   JSR   fin0@run0
   LDA   g4
   JSR   fin0@run2
   JMP   fin0@tail0

fin0@run2:
   SUBROUTINE

   STA   __zpabi_mix__v_0
   LDA   #$00
   STA   __zpabi_mix__v_1
   STA   __zpabi_mix__v_2
   RTS

fin0@run0:
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
   JSR   fin0@run0
   LDA   z6
   STA   __zpabi_mix__v_0
   LDA   z6+1
   STA   __zpabi_mix__v_1
   LDA   z6+2
   STA   __zpabi_mix__v_2
   LDA   z6+3
   JSR   fin0@run0
   LDA   z7
   JSR   fin0@run2
   JSR   fin0@run0
   LDA   ga8
   STA   __zpabi_mix__v_0
   LDA   ga8+1
   STA   __zpabi_mix__v_1
   LDA   ga8+2
   STA   __zpabi_mix__v_2
   LDA   ga8+3
   JMP   fin0@tail0

fin2:
   SUBROUTINE

   LDA   ga8+6
   STA   __zpabi_mix__v_0
   LDA   ga8+7
   STA   __zpabi_mix__v_1
   LDA   ga8+8
   STA   __zpabi_mix__v_2
   LDA   ga8+9
   JSR   fin0@run0
   LDA   ga8+12
   STA   __zpabi_mix__v_0
   LDA   ga8+13
   STA   __zpabi_mix__v_1
   LDA   ga8+14
   STA   __zpabi_mix__v_2
   LDA   ga8+15
   JSR   fin0@run0
   LDA   ga8+18
   STA   __zpabi_mix__v_0
   LDA   ga8+19
   STA   __zpabi_mix__v_1
   LDA   ga8+20
   STA   __zpabi_mix__v_2
   LDA   ga8+21
   JSR   fin0@run0
   JSR   fin2@run1
   JMP   fin0@tail0

fin2@run1:
   SUBROUTINE

   LDA   gs9+1
   STA   __zpabi_mix__v_0
   LDA   gs9+2
   STA   __zpabi_mix__v_1
   LDA   gs9+3
   STA   __zpabi_mix__v_2
   LDA   gs9+4
   RTS

fin3:
   SUBROUTINE

   LDA   gs9+5
   STA   __zpabi_mix__v_0
   LDA   gs9+6
   STA   __zpabi_mix__v_1
   LDA   #$00
   STA   __zpabi_mix__v_2
   JMP   fin0@tail0

f11:
   SUBROUTINE

   STA   __local_f11__p13
   TXA
   BMI   .sx_neg@2
   LDA   #$00
   BEQ   .sx_done@3
.sx_neg@2:
   LDA   #$FF
.sx_done@3:
   STA   __zpabi_mix__v_2
   STA   __zpabi_mix__v_3
   LDA   __local_f11__p13
   STA   __zpabi_mix__v_0
   STX   __zpabi_mix__v_1
   JSR   mix
   LDA   z6
   STA   gs9+5
   LDA   z6+1
   STA   gs9+6
   LDA   #$00
   STA   __zpabi_mix__v_2
   LDA   __zpabi_f11__p13
   STA   __local_f11__p13
.loop@0_start:
   LDA   @3.s15
   CLC
   ADC   #$03
   STA   @3.s15
   LDA   __local_f11__p13
   CLC
   ADC   #$01
   BEQ   .if_end@0
   INC   g1
   BNE   .inc_done@0
   INC   g1+1
.inc_done@0:
   LDA   g2
   LDY   #$00
   STA   (gp10),Y
   LDA   g2+1
   INY
   STA   (gp10),Y
   LDA   g2+2
   INY
   STA   (gp10),Y
   LDA   g2+3
   INY
   STA   (gp10),Y
   LDA   g2+4
   INY
   STA   (gp10),Y
   LDA   g2+5
   INY
   STA   (gp10),Y
.if_end@0:
   LDA   gs9+5
   LDX   gs9+6
   STX   __zpabi_mix__v_0
   SEC
   SBC   #$FF
   STA   __local_f11__p13
   LDA   __zpabi_mix__v_0
   SBC   #$00
   STA   gs9+6
   LDA   __local_f11__p13
   STA   gs9+5
   DEC   g4
   INC   __zpabi_mix__v_2
   BEQ   .loop@0_start
   JSR   fin2@run1
   STA   __local_f11__p13
   INC   __zpabi_mix__v_0
   BNE   .inc_done@1
   INC   __zpabi_mix__v_1
   BNE   .inc_done@1
   INC   __zpabi_mix__v_2
   BNE   .inc_done@1
   INC   __local_f11__p13
.inc_done@1:
   LDA   __zpabi_mix__v_0
   STA   gs9+1
   LDA   __zpabi_mix__v_1
   STA   gs9+2
   LDA   __zpabi_mix__v_2
   STA   gs9+3
   LDA   __local_f11__p13
   STA   gs9+4
   JSR   fin0@run0
   LDA   gs9+1
   STA   __local_f11__p13
   LDA   gs9+2
   STA   __local_f11__2
   LDA   g2
   SEC
   SBC   __local_f11__p13
   STA   __local_f11__p13
   LDA   g2+1
   SBC   __local_f11__2
   STA   __local_f11__2
   LDA   __local_f11__p13
   LDX   __local_f11__2
   RTS

main:
   SUBROUTINE

   LDA   #$7F
   STA   __local_main__l17
   LDA   #$00
   STA   __local_main__i20
   LDA   #$01
   STA   __local_main__i20
.main@rot@0:
   LDA   z6
   BMI   .sx_neg@6
   LDA   #$00
   BEQ   .sx_done@7
.sx_neg@6:
   LDA   #$FF
.sx_done@7:
   STA   __zpabi_mix__v_1
   STA   __zpabi_mix__v_2
   STA   __zpabi_mix__v_3
   LDA   z6
   STA   __zpabi_mix__v_0
   JSR   mix
   DEC   __local_main__i20
   BNE   .main@rot@0
   LDA   gs9+5
   ORA   gs9+6
   BEQ   .cond_else@1
   LDA   g3
   STA   __local_main__11
   LDA   g3+1
   STA   __local_main__10
   LDA   #$00
   STA   __local_main__9
   STA   __local_main__8
   JMP   .sx_done@9
.cond_else@1:
   LDA   g2
   STA   __local_main__11
   LDA   g2+1
   STA   __local_main__10
   LDA   g2+2
   STA   __local_main__9
   LDA   g2+3
   STA   __local_main__8
   LDA   #$00
   BPL   .sx_done@9
   LDA   #$FF
.sx_done@9:
   STA   __local_main__5
   STA   __local_main__4
   LDA   g4
   EOR   gs9+1
   STA   __local_main__3
   LDA   #$00
   EOR   gs9+2
   STA   __local_main__2
   LDA   __local_main__5
   EOR   gs9+3
   STA   __local_main__1
   LDA   __local_main__4
   EOR   gs9+4
   STA   __local_main__0
   LDA   __local_main__11
   AND   __local_main__3
   STA   __zpabi_mix__v_0
   LDA   __local_main__10
   AND   __local_main__2
   STA   __zpabi_mix__v_1
   LDA   __local_main__9
   AND   __local_main__1
   STA   __zpabi_mix__v_2
   LDA   __local_main__8
   AND   __local_main__0
   JSR   fin0@run0
   LDA   g1
   AND   #$05
   STA   g1
   STA   g3
   LDA   #$00
   STA   g1+1
   STA   g3+1
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

g2:
   DC.B  $05
   DC.B  $1B
   DC.B  $EE
   DC.B  $3A
   DC.B  $5F
   DC.B  $E7

g3:
   DC.W  $7048

ga8:
   DC.B  $01
   DS.B  5
   DC.B  $03
   DS.B  5
   DC.B  $FF
   DC.B  $FF
   DC.B  $FF
   DC.B  $7F
   DS.B  2
   DC.B  $18
   DC.B  $FF
   DC.B  $66
   DC.B  $9E
   DC.B  $26
   DC.B  $1A

gs9:
   DS.B  1
   DC.L  $00000100
   DC.W  $0001

__zp_init:
   SUBROUTINE

   LDA   #$00
   STA   g1+1
   STA   z5+1
   STA   z5+2
   STA   z5+3
   STA   z5+4
   STA   z5+5
   STA   z6+1
   STA   z6+2
   STA   z6+3
   LDA   #$7F
   STA   g1
   LDA   #$01
   STA   g4
   STA   z5
   LDA   #$02
   STA   z6
   LDA   #$80
   STA   z7
   LDA   #<g2
   STA   gp10
   LDA   #>g2
   STA   gp10+1
   LDA   #$03
   STA   @3.s15
   RTS
