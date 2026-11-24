SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
z3	EQU	$26
@3.s12	EQU	$27
__zpabi_f6__p8_0	EQU	$80
__zpabi_f6__p8_1	EQU	$81
__zpabi_mix__v_0	EQU	$82
__zpabi_mix__v_1	EQU	$83
__zpabi_mix__v_2	EQU	$84
__zpabi_mix__v_3	EQU	$85
__local_main__1	EQU	$87
__local_main__i15	EQU	$88
__local_main__2	EQU	$89
__local_f6__0	EQU	$8A
__local_f6__1	EQU	$8B
__local_f6__2	EQU	$8C
__local_f6__3	EQU	$8D
__local_f6__4	EQU	$8E
__local_f6__5	EQU	$8F
__local_f6__6	EQU	$90
__local_f6__7	EQU	$91
__local_mix__0	EQU	$92
__local_mix__1	EQU	$93
__local_mix__2	EQU	$94
__local_mix__3	EQU	$95

; @zp-link-meta-begin
; def f6 params=__zpabi_f6__p7,__zpabi_f6__p8_0,__zpabi_f6__p8_1 locals=__local_f6__0,__local_f6__1,__local_f6__2,__local_f6__3,__local_f6__4,__local_f6__5,__local_f6__6,__local_f6__7 indirect=false in_cycle=false param_regs=A,,
; def fin0 params= locals=__local_fin0__0 indirect=false in_cycle=false
; def fin1 params= locals=__local_fin1__0,__local_fin1__1 indirect=false in_cycle=false
; def fin2 params= locals=__local_fin2__0 indirect=false in_cycle=false
; def main params= locals=__local_main__0,__local_main__1,__local_main__i15,__local_main__2 indirect=false in_cycle=false
; def mix params=__zpabi_mix__v_0,__zpabi_mix__v_1,__zpabi_mix__v_2,__zpabi_mix__v_3 locals=__local_mix__0,__local_mix__1,__local_mix__2,__local_mix__3 indirect=false in_cycle=false
; call f6 -> mix
; call fin0 -> mix
; call fin1 -> mix
; call fin2 -> mix
; call main -> f6
; call main -> fin0
; call main -> fin1
; call main -> fin2
; call main -> mix
; item mix section=.text local=false zero=false
; item fin0 section=.text local=false zero=false
; item fin0@run3 section=.text local=true zero=false
; item fin0@run1 section=.text local=true zero=false
; item fin0@run0 section=.text local=true zero=false
; item fin1 section=.text local=false zero=false
; item fin2 section=.text local=false zero=false
; item f6 section=.text local=false zero=false
; item main section=.text local=false zero=false
; item main@run2 section=.text local=true zero=false
; item acc section=.bss local=false zero=true
; item g1 section=.data local=false zero=false
; item g2 section=.data local=false zero=false
; item ga4 section=.data local=false zero=false
; item gs5 section=.data local=false zero=false
; item __zp_init section=.text local=false zero=false
; zp z3 size=1 at=auto def=true init=$00
; zp @3.s12 size=1 at=auto def=true init=$FF weight=5 local=true
; runtime hargs_size=32 hargs_need=20
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
   STA   __zpabi_mix__v_1
   LDA   g2+2
   STA   __zpabi_mix__v_2
   LDA   g2+3
   JSR   fin0@run1
   LDA   z3
   BMI   .sx_neg@0
   LDA   #$00
   BEQ   .sx_done@1
.sx_neg@0:
   LDA   #$FF
.sx_done@1:
   JSR   fin0@run0
   LDA   z3
   JSR   fin0@run3
   LDA   ga4
   BMI   .sx_neg@2
   LDA   #$00
   BEQ   .sx_done@3
.sx_neg@2:
   LDA   #$FF
.sx_done@3:
   JSR   fin0@run0
   LDA   ga4
   STA   __zpabi_mix__v_0
   JMP   mix

fin0@run3:
   SUBROUTINE

   STA   __zpabi_mix__v_0
   JMP   mix

fin0@run1:
   SUBROUTINE

   STA   __zpabi_mix__v_3
   JMP   mix

fin0@run0:
   SUBROUTINE

   STA   __zpabi_mix__v_1
   STA   __zpabi_mix__v_2
   STA   __zpabi_mix__v_3
   RTS

fin1:
   SUBROUTINE

   LDA   ga4+1
   BMI   .sx_neg@4
   LDA   #$00
   BEQ   .sx_done@5
.sx_neg@4:
   LDA   #$FF
.sx_done@5:
   JSR   fin0@run0
   LDA   ga4+1
   JSR   fin0@run3
   LDA   ga4+2
   BMI   .sx_neg@6
   LDA   #$00
   BEQ   .sx_done@7
.sx_neg@6:
   LDA   #$FF
.sx_done@7:
   JSR   fin0@run0
   LDA   ga4+2
   JSR   fin0@run3
   LDA   ga4+3
   BMI   .sx_neg@8
   LDA   #$00
   BEQ   .sx_done@9
.sx_neg@8:
   LDA   #$FF
.sx_done@9:
   JSR   fin0@run0
   LDA   ga4+3
   JSR   fin0@run3
   LDA   gs5+1
   STA   __zpabi_mix__v_0
   LDA   gs5+2
   STA   __zpabi_mix__v_1
   LDA   #$00
   STA   __zpabi_mix__v_2
   STA   __zpabi_mix__v_3
   JMP   mix

fin2:
   SUBROUTINE

   LDA   gs5+3
   STA   __zpabi_mix__v_0
   LDA   #$00
   STA   __zpabi_mix__v_1
   STA   __zpabi_mix__v_2
   STA   __zpabi_mix__v_3
   JMP   mix

f6:
   SUBROUTINE

   LDA   gs5+1
   AND   #$5A
   STA   __local_f6__5
   LDY   #$00
   STY   gs5+2
   STA   gs5+1
   LDA   g1
   STA   __local_f6__3
   LDA   g1+1
   STA   __local_f6__2
   LDA   g1+2
   STA   __local_f6__1
   LDA   g1+3
   STA   __local_f6__0
   LDA   __local_f6__5
   CLC
   ADC   __local_f6__3
   STA   __zpabi_mix__v_0
   TYA
   ADC   __local_f6__2
   STA   __zpabi_mix__v_1
   TYA
   ADC   __local_f6__1
   STA   __zpabi_mix__v_2
   TYA
   ADC   __local_f6__0
   JSR   fin0@run1
   LDA   ga4
   BMI   .sx_neg@10
   LDA   #$00
   BEQ   .sx_done@11
.sx_neg@10:
   LDA   #$FF
.sx_done@11:
   STA   __local_f6__2
   STA   __local_f6__1
   STA   __local_f6__0
   LDA   ga4
   ORA   #$05
   STA   HARGS
   LDA   __local_f6__2
   ORA   #$66
   STA   HARGS+1
   LDA   __local_f6__1
   ORA   #$81
   STA   HARGS+2
   LDA   __local_f6__0
   ORA   #$BC
   STA   HARGS+3
   LDA   #$06
   STA   HARGS+4
   LDA   #$00
   STA   HARGS+5
   LDY   #$00
   STY   HARGS+6
   STA   HARGS+7
   JSR   udivmod32
   LDA   HARGS+8
   STA   __zpabi_mix__v_0
   LDA   HARGS+9
   STA   __zpabi_mix__v_1
   LDA   HARGS+10
   STA   __zpabi_mix__v_2
   LDA   HARGS+11
   JSR   fin0@run1
   LDA   z3
   STA   __local_f6__0
   LDX   #$05
.shift_loop@12:
   LDA   __local_f6__0
   ASL
   ROR   __local_f6__0
   DEX
   BNE   .shift_loop@12
   LDA   __local_f6__0
   STA   z3
   STA   __local_f6__7
   BMI   .sx_neg@14
   LDA   #$00
   BEQ   .sx_done@15
.sx_neg@14:
   LDA   #$FF
.sx_done@15:
   STA   __local_f6__6
   STA   __local_f6__5
   STA   __local_f6__4
   LDA   g1
   STA   __local_f6__3
   LDA   g1+1
   STA   __local_f6__2
   LDA   g1+2
   STA   __local_f6__1
   LDA   g1+3
   STA   __local_f6__0
   LDA   __local_f6__7
   EOR   __local_f6__3
   STA   __zpabi_mix__v_0
   LDA   __local_f6__6
   EOR   __local_f6__2
   STA   __zpabi_mix__v_1
   LDA   __local_f6__5
   EOR   __local_f6__1
   STA   __zpabi_mix__v_2
   LDA   __local_f6__4
   EOR   __local_f6__0
   JSR   fin0@run1
   LDA   @3.s12
   SEC
   SBC   #$01
   STA   __local_f6__2
   STA   @3.s12
   BMI   .sx_neg@16
   LDA   #$00
   BEQ   .sx_done@17
.sx_neg@16:
   LDA   #$FF
.sx_done@17:
   STA   __local_f6__1
   LDA   __local_f6__2
   SEC
   SBC   gs5+3
   STA   __local_f6__0
   BCS   .carry_inc@0
   DEC   __local_f6__1
.carry_inc@0:
   LDA   __local_f6__1
   BMI   .sx_neg@18
   LDA   #$00
   BEQ   .sx_done@19
.sx_neg@18:
   LDA   #$FF
.sx_done@19:
   STA   __zpabi_mix__v_2
   STA   __zpabi_mix__v_3
   LDA   __local_f6__0
   STA   __zpabi_mix__v_0
   LDA   __local_f6__1
   STA   __zpabi_mix__v_1
   JSR   mix
   LDA   @3.s12
   INC   @3.s12
   CLC
   ADC   gs5+3
   STA   gs5+3
   LDX   #$00
   TXA
   RTS

main:
   SUBROUTINE

   LDA   #$01
   STA   __local_main__i15
   LDA   g1
   STA   __zpabi_f6__p8_0
   LDA   g1+1
   STA   __zpabi_f6__p8_1
   LDA   g1+2
   LDA   g1+3
   LDA   #$FA
   JSR   f6
   STA   __local_main__2
   ORA   #$00
   BMI   .sx_neg@20
   LDA   #$00
   BEQ   .sx_done@21
.sx_neg@20:
   LDA   #$FF
.sx_done@21:
   STA   __zpabi_f6__p8_1
   LDA   __local_main__2
   STA   __zpabi_f6__p8_0
   LDA   #$33
   JSR   f6
   STA   __zpabi_mix__v_0
   STX   __zpabi_mix__v_1
   LDA   #$00
   STA   __zpabi_mix__v_2
   JSR   fin0@run1
   LDA   #$00
   STA   __local_main__i15
   JMP   .loop@0_start
.main@rot@0:
   LDA   ga4+1
   EOR   #$05
   STA   ga4+1
   STA   gs5+1
   BMI   .sx_neg@22
   LDA   #$00
   BEQ   .sx_done@23
.sx_neg@22:
   LDA   #$FF
.sx_done@23:
   STA   gs5+2
   LDA   __local_main__i15
   AND   #$03
   TAX
   LDA   ga4,X
   ADC   #$05
   STA   __zpabi_mix__v_0
   STA   ga4,X
   BMI   .sx_neg@24
   LDA   #$00
   BEQ   .sx_done@25
.sx_neg@24:
   LDA   #$FF
.sx_done@25:
   JSR   fin0@run0
   JSR   mix
   INC   __local_main__i15
.loop@0_start:
   LDA   __local_main__i15
   CMP   #$04
   BCC   .main@rot@0
   LDA   gs5+3
   STA   __local_main__i15
   LDA   __local_main__2
   BMI   .sx_neg@26
   LDA   #$00
   BEQ   .sx_done@27
.sx_neg@26:
   LDA   #$FF
.sx_done@27:
   STA   HARGS+1
   LDA   __local_main__2
   JSR   main@run2
   LDA   HARGS+4
   AND   #$07
   CMP   #$02
   BEQ   .case@2
   CMP   #$03
   BEQ   .case@3
   CMP   #$05
   BEQ   .case@9
   BNE   .switch@1_break
.case@2:
   LDA   gs5+3
   BEQ   .cond_else@0
   LDA   #$07
   STA   __local_main__1
   LDA   #$00
   BEQ   .cond_end@1
.cond_else@0:
   LDA   __local_main__i15
   STA   __local_main__1
   BMI   .sx_neg@28
   LDA   #$00
   BEQ   .sx_done@29
.sx_neg@28:
   LDA   #$FF
.sx_done@29:
.cond_end@1:
   STA   HARGS+1
   LDA   __local_main__1
   JSR   main@run2
   LDA   HARGS+6
   STA   ga4
   JMP   .switch@1_break
.case@3:
   LDA   g1
   LDX   g1+1
   LDX   g1+2
   LDX   g1+3
   AND   #$07
   BEQ   .case@5
   CMP   #$05
   BEQ   .case@6
.case@5:
.case@6:
.case@9:
.switch@1_break:
   JSR   fin0
   JSR   fin1
   JSR   fin2
   LDA   acc+1
   AND   #$7F
   TAX
   LDA   acc
   RTS

main@run2:
   SUBROUTINE

   STA   HARGS
   LDA   #$04
   STA   HARGS+2
   LDA   #$00
   STA   HARGS+3
   JMP   sdivmod16

acc:
   DS.B  2

g1:
   DC.L  $00000009

g2:
   DC.L  $00008000

ga4:
   DC.B  $FF, $AB, $7F, $FF

gs5:
   DS.B  1
   DC.W  $0001
   DC.B  $FF

__zp_init:
   SUBROUTINE

   LDA   #$00
   STA   z3
   LDA   #$FF
   STA   @3.s12
   RTS
