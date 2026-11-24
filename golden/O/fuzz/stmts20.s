SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
z4	EQU	$26
z5	EQU	$28
z6	EQU	$2A
g3	EQU	$2C
g1	EQU	$2E
__zpabi_f10__p11_0	EQU	$80
__zpabi_mix__v_0	EQU	$80
__zpabi_f10__p11_1	EQU	$81
__zpabi_mix__v_1	EQU	$81
__zpabi_f10__p11_2	EQU	$82
__zpabi_mix__v_2	EQU	$82
__zpabi_f10__p11_3	EQU	$83
__zpabi_mix__v_3	EQU	$83
__zpabi_f10__p12	EQU	$84
__local_f22__0	EQU	$85
__local_main__0	EQU	$85
__local_main__1	EQU	$86
__local_f22__pp23	EQU	$87
__local_f22__2	EQU	$88
__local_f22__l26	EQU	$89
__local_main__l29	EQU	$89
__local_main__4	EQU	$8A
__local_main__l28	EQU	$8B
__local_f17__1	EQU	$8D
__local_f17__4	EQU	$90
__local_f17__5	EQU	$91
__local_f17__6	EQU	$92
__local_f17__7	EQU	$93
__local_f10__l13	EQU	$94
__local_mix__0	EQU	$94
__local_f10__0	EQU	$95
__local_mix__1	EQU	$95
__local_mix__2	EQU	$96
__local_mix__3	EQU	$97

; @zp-link-meta-begin
; def f10 params=__zpabi_f10__p11_0,__zpabi_f10__p11_1,__zpabi_f10__p11_2,__zpabi_f10__p11_3,__zpabi_f10__p12 locals=__local_f10__l13,__local_f10__0 indirect=false in_cycle=false
; def f17 params=__zpabi_f17__pp18_0,__zpabi_f17__pp18_1 locals=__local_f17__0,__local_f17__1,__local_f17__2,__local_f17__3,__local_f17__4,__local_f17__5,__local_f17__6,__local_f17__7 indirect=false in_cycle=false param_regs=A,X
; def f22 params=__zpabi_f22__pp23_0,__zpabi_f22__pp23_1 locals=__local_f22__0,__local_f22__1,__local_f22__pp23,__local_f22__2,__local_f22__l26 indirect=false in_cycle=false param_regs=A,X
; def fin0 params= locals=__local_fin0__0,__local_fin0__1 indirect=false in_cycle=false
; def fin1 params= locals=__local_fin1__0,__local_fin1__1 indirect=false in_cycle=false
; def fin2 params= locals=__local_fin2__0,__local_fin2__1 indirect=false in_cycle=false
; def main params= locals=__local_main__0,__local_main__1,__local_main__2,__local_main__3,__local_main__l29,__local_main__4,__local_main__l28 indirect=false in_cycle=false
; def mix params=__zpabi_mix__v_0,__zpabi_mix__v_1,__zpabi_mix__v_2,__zpabi_mix__v_3 locals=__local_mix__0,__local_mix__1,__local_mix__2,__local_mix__3 indirect=false in_cycle=false
; call f17 -> f10
; call f17 -> mix
; call f22 -> f10
; call f22 -> f17
; call f22 -> mix
; call fin0 -> mix
; call fin1 -> mix
; call fin2 -> mix
; call main -> f17
; call main -> fin0
; call main -> fin1
; call main -> fin2
; call main -> mix
; item mix section=.text local=false zero=false
; item fin0 section=.text local=false zero=false
; item fin0@run4 section=.text local=true zero=false
; item fin0@run1 section=.text local=true zero=false
; item fin0@run0 section=.text local=true zero=false
; item fin0@run0@run3 section=.text local=true zero=false
; item fin1 section=.text local=false zero=false
; item fin1@tail0 section=.text local=true zero=false
; item fin2 section=.text local=false zero=false
; item f10 section=.text local=false zero=false
; item f17 section=.text local=false zero=false
; item f17@run2 section=.text local=true zero=false
; item f22 section=.text local=false zero=false
; item main section=.text local=false zero=false
; item acc section=.bss local=false zero=true
; item g2 section=.data local=false zero=false
; item ga7 section=.data local=false zero=false
; item gs8 section=.data local=false zero=false
; item gp9 section=.data local=false zero=false
; item @12.s27 section=.data local=true zero=false
; item __zp_init section=.text local=false zero=false
; zp z4 size=2 at=auto def=true init=$00,$00
; zp z5 size=2 at=auto def=true init=$00,$80
; zp z6 size=2 at=auto def=true init=$80,$00
; zp g3 size=2 at=auto def=true init=$EC,$EA weight=8
; zp g1 size=2 at=auto def=true init=$FF,$FF weight=7
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
   BMI   .sx_neg@0
   LDA   #$00
   BEQ   .sx_done@1
.sx_neg@0:
   LDA   #$FF
.sx_done@1:
   JSR   fin0@run4
   JSR   mix
   LDA   g2
   JSR   fin0@run0
   LDA   g3
   STA   __zpabi_mix__v_0
   LDA   g3+1
   JSR   fin0@run1
   LDA   z4+1
   BMI   .sx_neg@2
   LDA   #$00
   BEQ   .sx_done@3
.sx_neg@2:
   LDA   #$FF
.sx_done@3:
   JSR   fin0@run4
   LDA   z4
   STA   __zpabi_mix__v_0
   LDA   z4+1
   STA   __zpabi_mix__v_1
   JMP   mix

fin0@run4:
   SUBROUTINE

   STA   __zpabi_mix__v_2
   STA   __zpabi_mix__v_3
   RTS

fin0@run1:
   SUBROUTINE

   STA   __zpabi_mix__v_1
   LDA   #$00
   STA   __zpabi_mix__v_2
   STA   __zpabi_mix__v_3
   JMP   mix

fin0@run0:
   SUBROUTINE

   JSR   fin0@run0@run3
   JMP   mix

fin0@run0@run3:
   SUBROUTINE

   STA   __zpabi_mix__v_0
   LDA   #$00
   STA   __zpabi_mix__v_1
   JSR   fin0@run4
   RTS

fin1:
   SUBROUTINE

   LDA   z5
   STA   __zpabi_mix__v_0
   LDA   z5+1
   JSR   fin0@run1
   LDA   z6
   STA   __zpabi_mix__v_0
   LDA   z6+1
   JSR   fin0@run1
   LDA   ga7
   JSR   fin0@run0
   LDA   ga7+1
   JMP   fin1@tail0

fin1@tail0:
   SUBROUTINE

   JSR   fin0@run0@run3
   JMP   mix

fin2:
   SUBROUTINE

   LDA   ga7+2
   JSR   fin0@run0
   LDA   ga7+3
   JSR   fin0@run0
   LDA   gs8+2
   BMI   .sx_neg@4
   LDA   #$00
   BEQ   .sx_done@5
.sx_neg@4:
   LDA   #$FF
.sx_done@5:
   JSR   fin0@run4
   LDA   gs8+1
   STA   __zpabi_mix__v_0
   LDA   gs8+2
   STA   __zpabi_mix__v_1
   JSR   mix
   LDA   gs8+3
   JMP   fin1@tail0

f10:
   SUBROUTINE

   LDA   #$80
   STA   __local_f10__l13
   LDA   #$00
   STA   __local_f10__0
   LDA   g3
   EOR   #$01
   STA   g3
   STA   g1
   LDA   g3+1
   STA   g1+1
   LDA   z6
   LDX   z6+1
   ASL
   ASL
   ASL
   ASL
   ASL
   ASL
   STA   gs8+3
   LDA   #$FF
   RTS

f17:
   SUBROUTINE

   LDA   g1
   STA   __local_f17__1
   LDA   g1+1
   LDA   g3
   AND   #$01
   STA   g3
   LDA   #$00
   STA   g3+1
   LDA   __local_f17__1
   AND   g3
   JSR   f17@run2
   LDA   #$00
   STA   __zpabi_f10__p11_0
   STA   __zpabi_f10__p11_2
   STA   __zpabi_f10__p11_3
   LDA   #$80
   STA   __zpabi_f10__p11_1
   LDA   #$FF
   STA   __zpabi_f10__p12
   JSR   f10
   LDA   g3
   JSR   f17@run2
   LDA   ga7
   BEQ   .cond_else@0
   LDA   gp9
   STA   DPTR
   LDA   gp9+1
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   STA   __local_f17__7
   INY
   LDA   (DPTR),Y
   STA   __local_f17__6
   LDA   #$00
   STA   __local_f17__5
   STA   __local_f17__4
   JMP   .cond_end@1
.cond_else@0:
   LDA   #$FF
   STA   __local_f17__7
   STA   __local_f17__6
   STA   __local_f17__5
   LDY   #$7F
   STY   __local_f17__4
.cond_end@1:
   LDA   __local_f17__7
   STA   HARGS+16
   LDA   __local_f17__6
   STA   HARGS+17
   LDA   __local_f17__5
   STA   HARGS+18
   LDA   __local_f17__4
   STA   HARGS+19
   LDA   #$00
   STA   HARGS+20
   LDY   #$00
   STY   HARGS+21
   STY   HARGS+22
   STA   HARGS+23
   RTS

f17@run2:
   SUBROUTINE

   STA   __zpabi_mix__v_0
   LDA   #$00
   STA   __zpabi_mix__v_1
   JSR   fin0@run4
   JMP   mix

f22:
   SUBROUTINE

   STA   __local_f22__pp23
   STX   __local_f22__2
   LDY   #$FF
   STY   __local_f22__l26
   INY
   LDA   (__local_f22__pp23),Y
   STA   __local_f22__0
   INY
   LDA   (__local_f22__pp23),Y
   STA   __zpabi_mix__v_0
   LDA   __local_f22__0
   BNE   .carry_inc@0
   DEC   __zpabi_mix__v_0
.carry_inc@0:
   DEC   __local_f22__0
   LDA   __local_f22__0
   LDY   #$00
   STA   (__local_f22__pp23),Y
   LDA   __zpabi_mix__v_0
   INY
   STA   (__local_f22__pp23),Y
   LDA   @12.s27
   SEC
   SBC   z4
   LDA   #$00
   SBC   z4+1
   BVC   .jcmp_novf@6
   EOR   #$80
.jcmp_novf@6:
   BPL   .and_false@2
   LDA   __local_f22__l26
   STA   __zpabi_mix__v_0
   BMI   .sx_neg@7
   LDA   #$00
   BEQ   .sx_done@8
.sx_neg@7:
   LDA   #$FF
.sx_done@8:
   STA   __local_f22__0
   LDA   __zpabi_mix__v_0
   CLC
   ADC   g3
   STA   __zpabi_mix__v_0
   LDA   __local_f22__0
   ADC   g3+1
   ORA   __zpabi_mix__v_0
   BEQ   .and_false@2
   LDA   #$01
   STA   __zpabi_mix__v_0
   LDA   #$00
   BEQ   .and_end@3
.and_false@2:
   LDA   #$00
   STA   __zpabi_mix__v_0
.and_end@3:
   ORA   #$00
   BPL   .sx_done@10
   LDA   #$FF
.sx_done@10:
   JSR   fin0@run4
   LDA   #$00
   STA   __zpabi_mix__v_1
   JSR   mix
   LDX   #>__local_f22__l26
   LDA   #<__local_f22__l26
   JSR   f17
   LDA   #$00
   STA   __zpabi_f10__p11_0
   STA   __zpabi_f10__p11_1
   STA   __zpabi_f10__p11_2
   STA   __zpabi_f10__p11_3
   LDA   z4
   STA   __zpabi_f10__p12
   JSR   f10
   LDX   #>__local_f22__l26
   LDA   #<__local_f22__l26
   JSR   f17
   LDY   #$00
   LDA   (__local_f22__pp23),Y
   STA   __zpabi_mix__v_0
   INY
   LDA   (__local_f22__pp23),Y
   STA   __local_f22__0
   LDA   g3
   CLC
   SBC   __zpabi_mix__v_0
   LDA   g3+1
   SBC   __local_f22__0
   LDA   #$00
   ROL
   STA   __local_f22__0
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
   LDA   __local_f22__0
   STA   HARGS+16
   LDA   #$00
   STA   HARGS+17
   RTS

main:
   SUBROUTINE

   LDA   #$7F
   STA   __local_main__l28
   LDA   #$00
   STA   __local_main__l29
   STA   __local_main__4
   LDA   g1
   STA   HARGS
   LDA   g1+1
   STA   HARGS+1
   LDA   #$5A
   STA   HARGS+2
   LDA   #$00
   STA   HARGS+3
   JSR   mul16
   LDA   HARGS+4
   STA   g1
   LDA   HARGS+5
   STA   g1+1
   LDA   __local_main__l29
   STA   __local_main__1
   LDA   __local_main__4
   STA   __local_main__0
   LDA   HARGS+4
   SEC
   SBC   __local_main__1
   STA   __zpabi_mix__v_0
   LDA   HARGS+5
   SBC   __local_main__0
   JSR   fin0@run1
   LDX   #>__local_main__l28
   LDA   #<__local_main__l28
   JSR   f17
   LDA   HARGS+16
   STA   __zpabi_mix__v_0
   LDA   HARGS+17
   STA   __zpabi_mix__v_1
   LDA   HARGS+18
   STA   __zpabi_mix__v_2
   LDA   HARGS+19
   STA   __zpabi_mix__v_3
   JSR   mix
   LDA   g1
   STA   __local_main__0
   LDA   g1+1
   STA   __local_main__1
   LDA   __local_main__0
   BNE   .carry_inc@1
   DEC   __local_main__1
.carry_inc@1:
   DEC   __local_main__0
   LDA   __local_main__0
   STA   g1
   LDA   __local_main__1
   STA   g1+1
   LDA   __local_main__0
   STA   gs8+3
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

g2:
   DC.B  $03

ga7:
   DC.B  $3F
   DC.B  $21
   DC.B  $3F
   DC.B  $02

gs8:
   DS.B  1
   DC.W  $000B
   DC.B  $7F

gp9:
   DC.W  g3

@12.s27:
   DC.B  $FF

__zp_init:
   SUBROUTINE

   LDA   #$00
   STA   z4
   STA   z4+1
   STA   z5
   STA   z6+1
   LDA   #$FF
   STA   g1
   STA   g1+1
   LDA   #$EC
   STA   g3
   LDA   #$EA
   STA   g3+1
   LDA   #$80
   STA   z5+1
   STA   z6
   RTS
