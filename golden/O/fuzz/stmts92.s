SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
z3	EQU	$26
g1	EQU	$2E
g2	EQU	$2F
__zpabi_f11__p13_0	EQU	$80
__zpabi_f11__p13_1	EQU	$81
__zpabi_f11__p14	EQU	$82
__zpabi_mix__v_0	EQU	$83
__zpabi_mix__v_1	EQU	$84
__zpabi_mix__v_2	EQU	$85
__zpabi_mix__v_3	EQU	$86
__local_main__0	EQU	$87
__local_main__2	EQU	$89
__local_main__l21	EQU	$8B
__local_main__4	EQU	$8C
__local_f11__0	EQU	$8D
__local_f11__1	EQU	$8E
__local_f11__l17	EQU	$8F
__local_f11__l16	EQU	$90
__local_f11__2	EQU	$91
__local_f11__p12_1	EQU	$92
__local_f11__p12_0	EQU	$93
__local_f11__l18	EQU	$95
__local_f6__4	EQU	$9E
__local_f6__5	EQU	$9F
__local_f6__6	EQU	$A0
__local_f6__7	EQU	$A1
__local_f6__pp7	EQU	$A2
__local_f6__8	EQU	$A3
__local_mix__0	EQU	$A4
__local_mix__1	EQU	$A5
__local_mix__2	EQU	$A6
__local_mix__3	EQU	$A7

; @zp-link-meta-begin
; def f11 params=__zpabi_f11__p12_0,__zpabi_f11__p12_1,__zpabi_f11__p13_0,__zpabi_f11__p13_1,__zpabi_f11__p14 locals=__local_f11__0,__local_f11__1,__local_f11__l17,__local_f11__l16,__local_f11__2,__local_f11__p12_1,__local_f11__p12_0,__local_f11__3,__local_f11__l18,__local_f11__5,__local_f11__6,__local_f11__7,__local_f11__8 indirect=false in_cycle=false param_regs=A,X,,,
; def f6 params=__zpabi_f6__pp7_0,__zpabi_f6__pp7_1 locals=__local_f6__0,__local_f6__1,__local_f6__2,__local_f6__3,__local_f6__4,__local_f6__5,__local_f6__6,__local_f6__7,__local_f6__pp7,__local_f6__8 indirect=false in_cycle=false param_regs=A,X
; def fin0 params= locals=__local_fin0__0,__local_fin0__1,__local_fin0__2,__local_fin0__3 indirect=false in_cycle=false
; def fin1 params= locals=__local_fin1__0,__local_fin1__1,__local_fin1__2,__local_fin1__3 indirect=false in_cycle=false
; def fin2 params= locals=__local_fin2__0,__local_fin2__1,__local_fin2__2,__local_fin2__3 indirect=false in_cycle=false
; def main params= locals=__local_main__0,__local_main__1,__local_main__2,__local_main__3,__local_main__l21,__local_main__4 indirect=false in_cycle=false
; def mix params=__zpabi_mix__v_0,__zpabi_mix__v_1,__zpabi_mix__v_2,__zpabi_mix__v_3 locals=__local_mix__0,__local_mix__1,__local_mix__2,__local_mix__3 indirect=false in_cycle=false
; call f11 -> f6
; call f11 -> mix
; call f6 -> mix
; call fin0 -> mix
; call fin1 -> mix
; call fin2 -> mix
; call main -> f11
; call main -> fin0
; call main -> fin1
; call main -> fin2
; call main -> mix
; item mix section=.text local=false zero=false
; item fin0 section=.text local=false zero=false
; item fin0@run0 section=.text local=true zero=false
; item fin0@tail0 section=.text local=true zero=false
; item fin1 section=.text local=false zero=false
; item fin2 section=.text local=false zero=false
; item fin2@run2 section=.text local=true zero=false
; item f6 section=.text local=false zero=false
; item f6@run1 section=.text local=true zero=false
; item f11 section=.text local=false zero=false
; item main section=.text local=false zero=false
; item acc section=.bss local=false zero=true
; item ga4 section=.data local=false zero=false
; item gs5 section=.data local=false zero=false
; item __zp_init section=.text local=false zero=false
; zp z3 size=8 at=auto def=true init=$FF,$7F,$00,$00,$00,$00,$00,$00
; zp g1 size=1 at=auto def=true init=$07 weight=65
; zp g2 size=8 at=auto def=true init=$7F,$00,$00,$00,$00,$00,$00,$00 weight=131
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
   LDA   #$00
   STA   __zpabi_mix__v_1
   STA   __zpabi_mix__v_2
   JSR   fin0@run0
   LDA   g2
   STA   __zpabi_mix__v_0
   LDA   g2+1
   STA   __zpabi_mix__v_1
   LDA   g2+2
   STA   __zpabi_mix__v_2
   LDA   g2+3
   JSR   fin0@run0
   LDA   z3
   STA   __zpabi_mix__v_0
   LDA   z3+1
   STA   __zpabi_mix__v_1
   LDA   z3+2
   STA   __zpabi_mix__v_2
   LDA   z3+3
   JSR   fin0@run0
   LDA   ga4
   STA   __zpabi_mix__v_0
   LDA   ga4+1
   STA   __zpabi_mix__v_1
   LDA   ga4+2
   STA   __zpabi_mix__v_2
   LDA   ga4+3
   JMP   fin0@tail0

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

   LDA   ga4+8
   STA   __zpabi_mix__v_0
   LDA   ga4+9
   STA   __zpabi_mix__v_1
   LDA   ga4+10
   STA   __zpabi_mix__v_2
   LDA   ga4+11
   JSR   fin0@run0
   LDA   ga4+16
   STA   __zpabi_mix__v_0
   LDA   ga4+17
   STA   __zpabi_mix__v_1
   LDA   ga4+18
   STA   __zpabi_mix__v_2
   LDA   ga4+19
   JSR   fin0@run0
   LDA   ga4+24
   STA   __zpabi_mix__v_0
   LDA   ga4+25
   STA   __zpabi_mix__v_1
   LDA   ga4+26
   STA   __zpabi_mix__v_2
   LDA   ga4+27
   JSR   fin0@run0
   LDA   gs5+1
   STA   __zpabi_mix__v_0
   LDA   gs5+2
   STA   __zpabi_mix__v_1
   LDA   #$00
   STA   __zpabi_mix__v_2
   JMP   fin0@tail0

fin2:
   SUBROUTINE

   JSR   fin2@run2
   STA   __zpabi_mix__v_1
   LDA   gs5+5
   STA   __zpabi_mix__v_2
   LDA   gs5+6
   JMP   fin0@tail0

fin2@run2:
   SUBROUTINE

   LDA   gs5+3
   STA   __zpabi_mix__v_0
   LDA   gs5+4
   RTS

f6:
   SUBROUTINE

   STA   __local_f6__pp7
   STX   __local_f6__8
   LDA   g2
   CLC
   ADC   #$03
   STA   g2
   BCC   .carry_inc@0
   INC   g2+1
   BNE   .carry_inc@0
   INC   g2+2
   BNE   .carry_inc@0
   INC   g2+3
   BNE   .carry_inc@0
   INC   g2+4
   BNE   .carry_inc@0
   INC   g2+5
   BNE   .carry_inc@0
   INC   g2+6
   BNE   .carry_inc@0
   INC   g2+7
.carry_inc@0:
   LDY   #$00
   LDA   (__local_f6__pp7),Y
   STA   __local_f6__7
   INY
   LDA   (__local_f6__pp7),Y
   STA   __local_f6__6
   INY
   LDA   (__local_f6__pp7),Y
   STA   __local_f6__5
   INY
   LDA   (__local_f6__pp7),Y
   STA   __local_f6__4
   LDA   g2
   AND   __local_f6__7
   STA   __zpabi_mix__v_0
   LDA   g2+1
   AND   __local_f6__6
   STA   __zpabi_mix__v_1
   LDA   g2+2
   AND   __local_f6__5
   STA   __zpabi_mix__v_2
   LDA   g2+3
   AND   __local_f6__4
   JSR   fin0@run0
   JSR   fin2@run2
   JSR   f6@run1
   LDY   #$00
   LDA   (__local_f6__pp7),Y
   ORA   #$5A
   STA   (__local_f6__pp7),Y
   LDA   #$00
   AND   z3
   STA   __zpabi_mix__v_0
   LDA   #$00
   AND   z3+1
   STA   __zpabi_mix__v_1
   LDA   #$00
   AND   z3+2
   STA   __zpabi_mix__v_2
   LDA   #$00
   AND   z3+3
   JSR   fin0@run0
   LDA   g2
   STA   HARGS
   LDA   g2+1
   STA   HARGS+1
   LDA   g2+2
   STA   HARGS+2
   LDA   g2+3
   STA   HARGS+3
   LDA   g2+4
   STA   HARGS+4
   LDA   g2+5
   STA   HARGS+5
   LDA   g2+6
   STA   HARGS+6
   LDA   g2+7
   STA   HARGS+7
   LDA   #$06
   STA   HARGS+8
   JSR   lsr64
   LDA   HARGS+9
   STA   g2
   LDA   HARGS+10
   STA   g2+1
   LDA   HARGS+11
   STA   g2+2
   LDA   HARGS+12
   STA   g2+3
   LDA   HARGS+13
   STA   g2+4
   LDA   HARGS+14
   STA   g2+5
   LDA   HARGS+15
   STA   g2+6
   LDA   HARGS+16
   STA   g2+7
   LDA   HARGS+9
   RTS

f6@run1:
   SUBROUTINE

   STA   __zpabi_mix__v_1
   LDA   #$00
   STA   __zpabi_mix__v_2
   JMP   fin0@run0

f11:
   SUBROUTINE

   STA   __local_f11__p12_0
   STX   __local_f11__p12_1
   LDA   #$FF
   STA   __local_f11__l16
   STA   __local_f11__2
   LDA   #$04
   STA   __local_f11__l17
   LDA   #$03
   STA   __local_f11__l18
   LDA   #$00
   STA   __local_f11__l18+1
   STA   __local_f11__l18+2
   STA   __local_f11__l18+3
   STA   __local_f11__l18+4
   LDA   __local_f11__l16
   LDX   __local_f11__2
   STX   __local_f11__1
   CLC
   ADC   #$05
   STA   __local_f11__0
   BCC   .carry_inc@1
   INC   __local_f11__1
.carry_inc@1:
   STA   __local_f11__l16
   LDA   __local_f11__1
   STA   __local_f11__2
   LDA   __local_f11__0
   AND   #$07
   CMP   #$02
   BEQ   .switch@4_break
   LDA   __local_f11__l17
   ASL
   ASL
   ASL
   ASL
   ASL
   STA   __local_f11__1
   STA   __local_f11__l17
   BMI   .sx_neg@2
   LDA   #$00
   BEQ   .sx_done@3
.sx_neg@2:
   LDA   #$FF
.sx_done@3:
   STA   __local_f11__0
   LDA   __local_f11__1
   ORA   __local_f11__p12_0
   STA   __zpabi_mix__v_0
   LDA   __local_f11__0
   ORA   __local_f11__p12_1
   JSR   f6@run1
.switch@4_break:
   LDX   #>__local_f11__l18
   LDA   #<__local_f11__l18
   JSR   f6
   LDA   z3
   ORA   z3+1
   ORA   z3+2
   ORA   z3+3
   ORA   z3+4
   ORA   z3+5
   ORA   z3+6
   ORA   z3+7
   BEQ   .cond_else@5
   LDA   #$00
   STA   ga4+8
   STA   ga4+9
   STA   ga4+10
   STA   ga4+11
   STA   ga4+12
   STA   ga4+13
   STA   ga4+14
   JMP   .cond_end@6
.cond_else@5:
   LDA   z3
   STA   ga4+8
   LDA   z3+1
   STA   ga4+9
   LDA   z3+2
   STA   ga4+10
   LDA   z3+3
   STA   ga4+11
   LDA   z3+4
   STA   ga4+12
   LDA   z3+5
   STA   ga4+13
   LDA   z3+6
   STA   ga4+14
   LDA   z3+7
.cond_end@6:
   AND   #$03
   EOR   #$02
   SEC
   SBC   #$02
   STA   ga4+15
   LDA   #$00
   EOR   gs5+1
   STA   HARGS+16
   LDA   #$00
   EOR   gs5+2
   STA   HARGS+17
   LDA   #$01
   STA   HARGS+18
   LDA   #$00
   STA   HARGS+19
   STA   HARGS+20
   RTS

main:
   SUBROUTINE

   LDA   #$80
   STA   __local_main__l21
   LDA   #$00
   STA   __local_main__4
   STA   __zpabi_mix__v_0
   STA   __zpabi_mix__v_1
   LDA   z3+2
   AND   #$01
   STA   __zpabi_mix__v_2
   LDA   #$00
   JSR   fin0@run0
   LDA   __local_main__l21
   STA   __zpabi_f11__p14
   LDA   __local_main__4
   LDA   ga4+24
   STA   __zpabi_f11__p13_0
   LDA   ga4+25
   STA   __zpabi_f11__p13_1
   LDX   #$00
   TXA
   JSR   f11
   LDA   ga4+8
   CLC
   ADC   #$80
   STA   __zpabi_f11__p14
   LDA   #$00
   STA   __zpabi_f11__p13_0
   LDA   gs5+1
   STA   __zpabi_f11__p13_1
   LDX   #$00
   TXA
   JSR   f11
   LDA   __local_main__l21
   LDX   __local_main__4
   STX   __local_main__2
   SEC
   SBC   z3
   STA   __local_main__0
   LDA   __local_main__2
   SBC   z3+1
   TAX
   LDA   #$FF
   STA   __zpabi_f11__p13_0
   LDA   #$00
   STA   __zpabi_f11__p13_1
   STA   __zpabi_f11__p14
   LDA   __local_main__0
   JSR   f11
   LDA   HARGS+16
   STA   __local_main__l21
   LDA   HARGS+17
   STA   __local_main__4
   LDA   #$01
   STA   z3
   LDA   #$00
   STA   z3+1
   STA   z3+2
   STA   z3+3
   STA   z3+4
   STA   z3+5
   STA   z3+6
   STA   z3+7
   LDY   #$01
   STY   HARGS
   DEY
   STY   HARGS+1
   STY   HARGS+2
   STY   HARGS+3
   STY   HARGS+4
   STY   HARGS+5
   STY   HARGS+6
   STA   HARGS+7
   LDY   #$05
   STY   HARGS+8
   LDY   #$00
   STY   HARGS+9
   STY   HARGS+10
   STY   HARGS+11
   STY   HARGS+12
   STY   HARGS+13
   STY   HARGS+14
   STA   HARGS+15
   JSR   mul64
   LDA   HARGS+16
   STA   z3
   LDA   HARGS+17
   STA   z3+1
   LDA   HARGS+18
   STA   z3+2
   LDA   HARGS+19
   STA   z3+3
   LDA   HARGS+20
   STA   z3+4
   LDA   HARGS+21
   STA   z3+5
   LDA   HARGS+22
   STA   z3+6
   LDA   HARGS+23
   STA   z3+7
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

ga4:
   DC.L  $00007FFF
   DC.L  $00000000
   DC.L  $7FFFFFFF
   DC.L  $00000000
   DC.L  $00008000
   DC.L  $00000000
   DC.L  $00008000
   DC.L  $00000000

gs5:
   DS.B  1
   DC.W  $0002
   DC.L  $00000002

__zp_init:
   SUBROUTINE

   LDA   #$00
   STA   g2+1
   STA   g2+2
   STA   g2+3
   STA   g2+4
   STA   g2+5
   STA   g2+6
   STA   g2+7
   STA   z3+2
   STA   z3+3
   STA   z3+4
   STA   z3+5
   STA   z3+6
   STA   z3+7
   LDA   #$07
   STA   g1
   LDA   #$7F
   STA   g2
   STA   z3+1
   LDA   #$FF
   STA   z3
   RTS
