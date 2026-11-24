SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
z4	EQU	$26
z5	EQU	$27
z6	EQU	$29
__zpabi_mix__v_0	EQU	$80
__zpabi_mix__v_1	EQU	$81
__zpabi_mix__v_2	EQU	$82
__zpabi_mix__v_3	EQU	$83
__local_f9__l11	EQU	$84
__local_f9__1	EQU	$86
__local_f9__2	EQU	$87
__local_f9__3	EQU	$88
__local_f9__4	EQU	$89
__local_f9__5	EQU	$8A
__local_mix__0	EQU	$8C
__local_mix__1	EQU	$8D
__local_mix__2	EQU	$8E
__local_mix__3	EQU	$8F

; @zp-link-meta-begin
; def f9 params= locals=__local_f9__l11,__local_f9__0,__local_f9__1,__local_f9__2,__local_f9__3,__local_f9__4,__local_f9__5,__local_f9__6 indirect=false in_cycle=false
; def fin0 params= locals=__local_fin0__0,__local_fin0__1,__local_fin0__2,__local_fin0__3 indirect=false in_cycle=false
; def fin1 params= locals=__local_fin1__0,__local_fin1__1,__local_fin1__2,__local_fin1__3 indirect=false in_cycle=false
; def fin2 params= locals=__local_fin2__0,__local_fin2__1,__local_fin2__2,__local_fin2__3 indirect=false in_cycle=false
; def main params= locals=__local_main__0,__local_main__1,__local_main__2,__local_main__3 indirect=false in_cycle=false
; def mix params=__zpabi_mix__v_0,__zpabi_mix__v_1,__zpabi_mix__v_2,__zpabi_mix__v_3 locals=__local_mix__0,__local_mix__1,__local_mix__2,__local_mix__3 indirect=false in_cycle=false
; call f9 -> mix
; call fin0 -> mix
; call fin1 -> mix
; call fin2 -> mix
; call main -> fin0
; call main -> fin1
; call main -> fin2
; call main -> mix
; item mix section=.text local=false zero=false
; item fin0 section=.text local=false zero=false
; item fin0@run2 section=.text local=true zero=false
; item fin0@run0 section=.text local=true zero=false
; item fin0@tail0 section=.text local=true zero=false
; item fin1 section=.text local=false zero=false
; item fin2 section=.text local=false zero=false
; item fin2@run1 section=.text local=true zero=false
; item f9 section=.text local=false zero=false
; item main section=.text local=false zero=false
; item acc section=.bss local=false zero=true
; item g1 section=.data local=false zero=false
; item g2 section=.data local=false zero=false
; item g3 section=.data local=false zero=false
; item ga7 section=.data local=false zero=false
; item gs8 section=.data local=false zero=false
; item @1.s12 section=.data local=true zero=false
; item __zp_init section=.text local=false zero=false
; zp z4 size=1 at=auto def=true init=$00
; zp z5 size=2 at=auto def=true init=$FF,$00
; zp z6 size=4 at=auto def=true init=$00,$00,$01,$00
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
   JSR   fin0@run0
   LDA   g2
   STA   __zpabi_mix__v_0
   LDA   g2+1
   STA   __zpabi_mix__v_1
   LDA   g2+2
   STA   __zpabi_mix__v_2
   LDA   g2+3
   JSR   fin0@run2
   LDA   g3
   STA   __zpabi_mix__v_0
   LDA   g3+1
   STA   __zpabi_mix__v_1
   LDA   g3+2
   STA   __zpabi_mix__v_2
   LDA   g3+3
   JSR   fin0@run2
   LDA   z4
   STA   __zpabi_mix__v_0
   BMI   .sx_neg@0
   LDA   #$00
   BEQ   .sx_done@1
.sx_neg@0:
   LDA   #$FF
.sx_done@1:
   STA   __zpabi_mix__v_1
   STA   __zpabi_mix__v_2
   JMP   fin0@tail0

fin0@run2:
   SUBROUTINE

   STA   __zpabi_mix__v_3
   JMP   mix

fin0@run0:
   SUBROUTINE

   STA   __zpabi_mix__v_1
   LDA   #$00
   STA   __zpabi_mix__v_2
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
   JSR   fin0@run0
   LDA   z6
   STA   __zpabi_mix__v_0
   LDA   z6+1
   STA   __zpabi_mix__v_1
   LDA   z6+2
   STA   __zpabi_mix__v_2
   LDA   z6+3
   JSR   fin0@run2
   LDA   ga7
   STA   __zpabi_mix__v_0
   LDA   ga7+1
   JSR   fin0@run0
   LDA   ga7+2
   STA   __zpabi_mix__v_0
   LDA   ga7+3
   STA   __zpabi_mix__v_1
   LDA   #$00
   STA   __zpabi_mix__v_2
   JMP   fin0@tail0

fin2:
   SUBROUTINE

   LDA   ga7+4
   STA   __zpabi_mix__v_0
   LDA   ga7+5
   JSR   fin0@run0
   LDA   ga7+6
   STA   __zpabi_mix__v_0
   LDA   ga7+7
   JSR   fin0@run0
   LDA   gs8+1
   STA   __zpabi_mix__v_0
   LDA   gs8+2
   STA   __zpabi_mix__v_1
   LDA   gs8+3
   STA   __zpabi_mix__v_2
   LDA   gs8+4
   JSR   fin0@run2
   JSR   fin2@run1
   JMP   fin0@tail0

fin2@run1:
   SUBROUTINE

   LDA   gs8+9
   STA   __zpabi_mix__v_0
   LDA   gs8+10
   STA   __zpabi_mix__v_1
   LDA   gs8+11
   STA   __zpabi_mix__v_2
   LDA   gs8+12
   RTS

f9:
   SUBROUTINE

   LDA   #$FF
   STA   __local_f9__l11
   LDA   #$00
   STA   __zpabi_mix__v_0
   STA   __zpabi_mix__v_1
   STA   __zpabi_mix__v_2
   JSR   fin0@run2
   LDA   @1.s12
   STA   __zpabi_mix__v_0
   LDA   #$00
   AND   @1.s12+1
   STA   __zpabi_mix__v_1
   LDA   #$00
   STA   __zpabi_mix__v_2
   JSR   fin0@run2
   LDA   gs8+1
   LDX   gs8+2
   STX   __local_f9__5
   LDX   gs8+3
   STX   __local_f9__4
   LDX   gs8+4
   STX   __local_f9__3
   LDX   gs8+9
   STX   __local_f9__2
   LDX   gs8+10
   STX   __local_f9__1
   LDX   gs8+11
   STX   __zpabi_mix__v_0
   LDX   gs8+12
   STX   __local_f9__l11
   CLC
   ADC   __local_f9__2
   STA   gs8+9
   LDA   __local_f9__5
   ADC   __local_f9__1
   STA   gs8+10
   LDA   __local_f9__4
   ADC   __zpabi_mix__v_0
   STA   gs8+11
   LDA   __local_f9__3
   ADC   __local_f9__l11
   STA   gs8+12
   LDX   #$00
   LDA   #$FF
   RTS

main:
   SUBROUTINE

   LDA   g2
   ORA   #$03
   STA   g2
   SEC
   SBC   g3
   STA   z4
   JSR   fin2@run1
   STA   __zpabi_mix__v_3
   LDA   __zpabi_mix__v_0
   CLC
   ADC   #$03
   STA   __zpabi_mix__v_0
   BCC   .carry_inc@0
   INC   __zpabi_mix__v_1
   BNE   .carry_inc@0
   INC   __zpabi_mix__v_2
   BNE   .carry_inc@0
   INC   __zpabi_mix__v_3
.carry_inc@0:
   STA   gs8+9
   LDA   __zpabi_mix__v_1
   STA   gs8+10
   LDA   __zpabi_mix__v_2
   STA   gs8+11
   LDA   __zpabi_mix__v_3
   STA   gs8+12
   JSR   mix
   LDA   g2
   STA   z6
   LDA   g2+1
   STA   z6+1
   LDA   g2+2
   STA   z6+2
   LDA   g2+3
   STA   z6+3
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
   DC.W  $0002

g2:
   DC.L  $80000000

g3:
   DC.L  $00000001

ga7:
   DC.W  $FFFF
   DC.W  $0002
   DC.W  $0001
   DC.W  $DE6D

gs8:
   DS.B  1
   DC.L  $00008000
   DC.L  $00000000
   DC.L  $00000002

@1.s12:
   DC.W  $0002

__zp_init:
   SUBROUTINE

   LDA   #$00
   STA   z4
   STA   z5+1
   STA   z6
   STA   z6+1
   STA   z6+3
   LDA   #$FF
   STA   z5
   LDA   #$01
   STA   z6+2
   RTS
