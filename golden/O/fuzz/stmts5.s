SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
z3	EQU	$26
z4	EQU	$28
z5	EQU	$30
g1	EQU	$32
__zpabi_f8__p11	EQU	$80
__zpabi_f8__pp12_0	EQU	$81
__zpabi_f8__pp12_1	EQU	$82
__zpabi_mix__v_0	EQU	$83
__zpabi_mix__v_1	EQU	$84
__zpabi_mix__v_2	EQU	$85
__zpabi_mix__v_3	EQU	$86
__local_main__0	EQU	$87
__local_main__1	EQU	$88
__local_main__2	EQU	$89
__local_main__l19	EQU	$8B
__local_f8__w16	EQU	$8F
__local_f8__0	EQU	$90
__local_f8__p10	EQU	$92
__local_mix__0	EQU	$94
__local_mix__1	EQU	$95
__local_mix__2	EQU	$96
__local_mix__3	EQU	$97

; @zp-link-meta-begin
; def f8 params=__zpabi_f8__p9,__zpabi_f8__p10,__zpabi_f8__p11,__zpabi_f8__pp12_0,__zpabi_f8__pp12_1 locals=__local_f8__w16,__local_f8__0,__local_f8__p11,__local_f8__p10,__local_f8__1 indirect=false in_cycle=false param_regs=A,X,,, changes=__zpabi_f8__p11
; def fin0 params= locals=__local_fin0__0,__local_fin0__1,__local_fin0__2,__local_fin0__3 indirect=false in_cycle=false
; def fin1 params= locals=__local_fin1__0,__local_fin1__1 indirect=false in_cycle=false
; def fin2 params= locals=__local_fin2__0,__local_fin2__1,__local_fin2__2,__local_fin2__3 indirect=false in_cycle=false
; def main params= locals=__local_main__0,__local_main__1,__local_main__2,__local_main__3,__local_main__l19,__local_main__5,__local_main__6,__local_main__7 indirect=false in_cycle=false
; def mix params=__zpabi_mix__v_0,__zpabi_mix__v_1,__zpabi_mix__v_2,__zpabi_mix__v_3 locals=__local_mix__0,__local_mix__1,__local_mix__2,__local_mix__3 indirect=false in_cycle=false
; call f8 -> mix
; call fin0 -> mix
; call fin1 -> mix
; call fin2 -> mix
; call main -> f8
; call main -> fin0
; call main -> fin1
; call main -> fin2
; call main -> mix
; item mix section=.text local=false zero=false
; item fin0 section=.text local=false zero=false
; item fin0@run2 section=.text local=true zero=false
; item fin0@run1 section=.text local=true zero=false
; item fin0@tail0 section=.text local=true zero=false
; item fin1 section=.text local=false zero=false
; item fin1@run0 section=.text local=true zero=false
; item fin2 section=.text local=false zero=false
; item f8 section=.text local=false zero=false
; item main section=.text local=false zero=false
; item acc section=.bss local=false zero=true
; item g2 section=.data local=false zero=false
; item ga6 section=.data local=false zero=false
; item gs7 section=.data local=false zero=false
; item __zp_init section=.text local=false zero=false
; zp z3 size=2 at=auto def=true init=$02,$00
; zp z4 size=8 at=auto def=true init=$00,$00,$00,$00,$00,$00,$00,$00
; zp z5 size=2 at=auto def=true init=$FF,$00
; zp g1 size=1 at=auto def=true init=$03 weight=4
; runtime hargs_size=32 hargs_need=12
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
   JSR   fin0@run1
   LDA   g2+1
   BMI   .sx_neg@0
   LDA   #$00
   BEQ   .sx_done@1
.sx_neg@0:
   LDA   #$FF
.sx_done@1:
   STA   __zpabi_mix__v_2
   STA   __zpabi_mix__v_3
   LDA   g2
   STA   __zpabi_mix__v_0
   LDA   g2+1
   STA   __zpabi_mix__v_1
   JSR   mix
   LDA   z3
   STA   __zpabi_mix__v_0
   LDA   z3+1
   STA   __zpabi_mix__v_1
   BMI   .sx_neg@2
   LDA   #$00
   BEQ   .sx_done@3
.sx_neg@2:
   LDA   #$FF
.sx_done@3:
   JSR   fin0@run2
   LDA   z4
   STA   __zpabi_mix__v_0
   LDA   z4+1
   STA   __zpabi_mix__v_1
   LDA   z4+2
   STA   __zpabi_mix__v_2
   LDA   z4+3
   JMP   fin0@tail0

fin0@run2:
   SUBROUTINE

   STA   __zpabi_mix__v_2
   STA   __zpabi_mix__v_3
   JMP   mix

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

   LDA   z5
   STA   __zpabi_mix__v_0
   LDA   z5+1
   JSR   fin1@run0
   LDA   ga6
   STA   __zpabi_mix__v_0
   LDA   ga6+1
   JSR   fin1@run0
   LDA   ga6+2
   STA   __zpabi_mix__v_0
   LDA   ga6+3
   JSR   fin1@run0
   LDA   ga6+4
   STA   __zpabi_mix__v_0
   LDA   ga6+5
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

   LDA   ga6+6
   STA   __zpabi_mix__v_0
   LDA   ga6+7
   JSR   fin1@run0
   LDA   gs7+1
   JSR   fin0@run1
   LDA   gs7+2
   STA   __zpabi_mix__v_0
   LDA   gs7+3
   STA   __zpabi_mix__v_1
   LDA   gs7+4
   STA   __zpabi_mix__v_2
   LDA   gs7+5
   JMP   fin0@tail0

f8:
   SUBROUTINE

   STX   __local_f8__p10
   LDA   ga6+4
   CLC
   ADC   #$FF
   STA   ga6+4
   BCC   .carry_inc@0
   INC   ga6+5
.carry_inc@0:
   STX   gs7+1
   LDA   #$00
   SEC
   SBC   g1
   STA   g1
   AND   #$07
   BEQ   .switch@0_break
   CMP   #$01
   BEQ   .switch@0_break
   CMP   #$05
   BNE   .default@6
   LDA   #$00
   STA   __local_f8__w16
   JMP   .loop@4_continue
.f8@rot@0:
   LDA   __zpabi_f8__p11
   STA   __zpabi_mix__v_0
   LDA   #$00
   STA   __zpabi_mix__v_1
   STA   __zpabi_mix__v_2
   STA   __zpabi_mix__v_3
   JSR   mix
   LDA   gs7+1
   AND   #$81
   STA   gs7+1
   AND   __local_f8__p10
   BNE   .loop@4_break
   INC   __zpabi_f8__p11
   LDA   __local_f8__0
   STA   __local_f8__w16
.loop@4_continue:
   CLC
   ADC   #$01
   STA   __local_f8__0
   LDA   __local_f8__w16
   CMP   #$02
   BCC   .f8@rot@0
.loop@4_break:
.default@6:
.switch@0_break:
   LDA   z3
   STA   __local_f8__0
   LDA   z3+1
   STA   __local_f8__w16
   BMI   .sx_neg@4
   LDA   #$00
   BEQ   .sx_done@5
.sx_neg@4:
   LDA   #$FF
.sx_done@5:
   STA   HARGS+10
   STA   HARGS+11
   LDA   __local_f8__0
   STA   HARGS+8
   LDA   __local_f8__w16
   STA   HARGS+9
   RTS

main:
   SUBROUTINE

   LDA   #$80
   STA   __local_main__l19
   LDA   #$00
   STA   __local_main__l19+1
   STA   __local_main__l19+2
   STA   __local_main__l19+3
   STA   z5
   STA   z5+1
   LDA   gs7+1
   STA   __local_main__1
   CLC
   ADC   #$01
   STA   gs7+1
   LDA   gs7+2
   SEC
   SBC   #$7F
   AND   __local_main__1
   JSR   fin0@run1
   LDA   g2+1
   BMI   .sx_neg@6
   LDA   #$00
   BEQ   .sx_done@7
.sx_neg@6:
   LDA   #$FF
.sx_done@7:
   STA   __local_main__1
   STA   __local_main__0
   LDA   g2
   SEC
   SBC   #$FF
   LDA   g2+1
   SBC   #$FF
   LDA   __local_main__1
   SBC   #$00
   LDA   __local_main__0
   SBC   #$00
   LDA   #$00
   ROL
   STA   __local_main__2
   LDA   g1
   LDX   gs7+1
   STX   __local_main__1
   SEC
   SBC   __local_main__1
   LDA   #$00
   ROL
   TAX
   LDA   gs7+2
   CLC
   ADC   __local_main__1
   STA   __zpabi_f8__p11
   LDA   #<__local_main__l19
   STA   __zpabi_f8__pp12_0
   LDA   #>__local_main__l19
   STA   __zpabi_f8__pp12_1
   LDA   __local_main__2
   JSR   f8
   LDA   HARGS+8
   STA   __local_main__l19
   LDA   HARGS+9
   STA   __local_main__l19+1
   LDA   HARGS+10
   STA   __local_main__l19+2
   LDA   HARGS+11
   STA   __local_main__l19+3
   LDA   ga6
   EOR   #$FF
   STA   __zpabi_mix__v_0
   LDA   ga6+1
   EOR   #$FF
   STA   __zpabi_mix__v_1
   LDA   #$00
   JSR   fin0@run2
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
   DC.W  $FFFF

ga6:
   DS.B  2
   DC.W  $00FF
   DC.W  $0002
   DC.W  $0001

gs7:
   DS.B  1
   DC.B  $FF
   DC.L  $00000100

__zp_init:
   SUBROUTINE

   LDA   #$00
   STA   z3+1
   STA   z4
   STA   z4+1
   STA   z4+2
   STA   z4+3
   STA   z4+4
   STA   z4+5
   STA   z4+6
   STA   z4+7
   STA   z5+1
   LDA   #$03
   STA   g1
   LDA   #$02
   STA   z3
   LDA   #$FF
   STA   z5
   RTS
