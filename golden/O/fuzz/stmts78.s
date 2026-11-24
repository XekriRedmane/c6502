SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
z4	EQU	$26
g3	EQU	$2A
gp8	EQU	$2B
g1	EQU	$2D
g2	EQU	$2E
gp7	EQU	$30
__zpabi_mix__v_0	EQU	$80
__zpabi_mix__v_1	EQU	$81
__zpabi_mix__v_2	EQU	$82
__zpabi_mix__v_3	EQU	$83
__local_main__0	EQU	$84
__local_main__l14	EQU	$85
__local_main__1	EQU	$86
__local_main__l13	EQU	$87
__local_main__2	EQU	$88
__local_f9__i12	EQU	$8C
__local_mix__0	EQU	$8F
__local_mix__1	EQU	$90
__local_mix__2	EQU	$91
__local_mix__3	EQU	$92

; @zp-link-meta-begin
; def f9 params=__zpabi_f9__p10_0,__zpabi_f9__p10_1 locals=__local_f9__0,__local_f9__1,__local_f9__i12,__local_f9__p10_1,__local_f9__2 indirect=false in_cycle=false param_regs=A,X
; def fin0 params= locals=__local_fin0__0,__local_fin0__1 indirect=false in_cycle=false
; def fin1 params= locals=__local_fin1__0,__local_fin1__1 indirect=false in_cycle=false
; def fin2 params= locals=__local_fin2__0,__local_fin2__1 indirect=false in_cycle=false
; def main params= locals=__local_main__0,__local_main__l14,__local_main__1,__local_main__l13,__local_main__2,__local_main__3 indirect=false in_cycle=false
; def mix params=__zpabi_mix__v_0,__zpabi_mix__v_1,__zpabi_mix__v_2,__zpabi_mix__v_3 locals=__local_mix__0,__local_mix__1,__local_mix__2,__local_mix__3 indirect=false in_cycle=false
; call f9 -> mix
; call fin0 -> mix
; call fin1 -> mix
; call fin2 -> mix
; call main -> f9
; call main -> fin0
; call main -> fin1
; call main -> fin2
; item mix section=.text local=false zero=false
; item fin0 section=.text local=false zero=false
; item fin0@run4 section=.text local=true zero=false
; item fin0@run1 section=.text local=true zero=false
; item fin1 section=.text local=false zero=false
; item fin1@run0 section=.text local=true zero=false
; item fin1@run0@run3 section=.text local=true zero=false
; item fin1@tail0 section=.text local=true zero=false
; item fin2 section=.text local=false zero=false
; item f9 section=.text local=false zero=false
; item f9@run2 section=.text local=true zero=false
; item main section=.text local=false zero=false
; item acc section=.bss local=false zero=true
; item ga5 section=.data local=false zero=false
; item gs6 section=.data local=false zero=false
; item __zp_init section=.text local=false zero=false
; zp z4 size=4 at=auto def=true init=$00,$00,$01,$00
; zp g3 size=1 at=auto def=true init=$7F weight=13
; zp gp8 size=2 at=auto def=true init=<g1,>g1 weight=24
; zp g1 size=1 at=auto def=true init=$6E weight=11
; zp g2 size=2 at=auto def=true init=$02,$00 weight=10
; zp gp7 size=2 at=auto def=true init=<g3,>g3 weight=10
; runtime hargs_size=32 hargs_need=10
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
   JSR   fin0@run1
   JSR   fin0@run4
   BMI   .sx_neg@0
   LDA   #$00
   BEQ   .sx_done@1
.sx_neg@0:
   LDA   #$FF
.sx_done@1:
   JSR   fin0@run1
   LDA   g3
   BMI   .sx_neg@2
   LDA   #$00
   BEQ   .sx_done@3
.sx_neg@2:
   LDA   #$FF
.sx_done@3:
   STA   __zpabi_mix__v_1
   STA   __zpabi_mix__v_2
   STA   __zpabi_mix__v_3
   LDA   g3
   STA   __zpabi_mix__v_0
   JSR   mix
   LDA   z4
   STA   __zpabi_mix__v_0
   LDA   z4+1
   STA   __zpabi_mix__v_1
   LDA   z4+2
   STA   __zpabi_mix__v_2
   LDA   z4+3
   STA   __zpabi_mix__v_3
   JMP   mix

fin0@run4:
   SUBROUTINE

   LDA   g2
   STA   __zpabi_mix__v_0
   LDA   g2+1
   STA   __zpabi_mix__v_1
   RTS

fin0@run1:
   SUBROUTINE

   STA   __zpabi_mix__v_2
   STA   __zpabi_mix__v_3
   JMP   mix

fin1:
   SUBROUTINE

   LDA   ga5
   STA   __zpabi_mix__v_0
   LDA   ga5+1
   JSR   fin1@run0
   LDA   ga5+2
   STA   __zpabi_mix__v_0
   LDA   ga5+3
   JSR   fin1@run0
   LDA   ga5+4
   STA   __zpabi_mix__v_0
   LDA   ga5+5
   JSR   fin1@run0
   LDA   ga5+6
   STA   __zpabi_mix__v_0
   LDA   ga5+7
   JMP   fin1@tail0

fin1@run0:
   SUBROUTINE

   JSR   fin1@run0@run3
   JMP   mix

fin1@run0@run3:
   SUBROUTINE

   STA   __zpabi_mix__v_1
   LDA   #$00
   STA   __zpabi_mix__v_2
   STA   __zpabi_mix__v_3
   RTS

fin1@tail0:
   SUBROUTINE

   JSR   fin1@run0@run3
   JMP   mix

fin2:
   SUBROUTINE

   LDA   gs6+2
   BMI   .sx_neg@4
   LDA   #$00
   BEQ   .sx_done@5
.sx_neg@4:
   LDA   #$FF
.sx_done@5:
   STA   __zpabi_mix__v_2
   STA   __zpabi_mix__v_3
   LDA   gs6+1
   STA   __zpabi_mix__v_0
   LDA   gs6+2
   STA   __zpabi_mix__v_1
   JSR   mix
   LDA   gs6+3
   STA   __zpabi_mix__v_0
   LDA   gs6+4
   JMP   fin1@tail0

f9:
   SUBROUTINE

   STA   HARGS
   LDA   gs6+1
   CLC
   ADC   #$81
   STA   gs6+1
   BCC   .carry_inc@0
   INC   gs6+2
.carry_inc@0:
   LDY   #$00
   LDA   (gp8),Y
   STA   __local_f9__i12
   LDA   gs6+3
   ORA   #$81
   STA   gs6+3
   ORA   __local_f9__i12
   STA   g3
   STX   HARGS+1
   LDA   #$06
   JSR   f9@run2
   LDA   HARGS+4
   AND   #$07
   CMP   #$03
   BEQ   .case@1
   CMP   #$04
   BNE   .lb_skip@1
   JMP   .switch@0_break
.lb_skip@1:
   CMP   #$05
   BEQ   .case@3
   JMP   .default@8
.case@1:
   LDA   ga5
   ORA   ga5+1
   BEQ   .cond_else@0
   LDA   #$01
   STA   __zpabi_mix__v_0
   LDA   #$00
   BEQ   .cond_end@1
.cond_else@0:
   LDA   #$03
   STA   __zpabi_mix__v_0
   LDA   #$00
.cond_end@1:
   LDY   #$00
   STY   __zpabi_mix__v_1
   STY   __zpabi_mix__v_2
   STA   __zpabi_mix__v_3
   JSR   mix
   JMP   .switch@0_break
.case@3:
   LDA   #$00
   STA   __local_f9__i12
.loop@4_start:
   LDA   __local_f9__i12
   BEQ   .lb_skip@0
   JMP   .loop@4_break
.lb_skip@0:
   BEQ   .cond_else@2
   LDA   gs6+1
   STA   HARGS
   LDA   gs6+2
   STA   HARGS+1
   LDA   #$02
   JSR   f9@run2
   LDA   HARGS+6
   STA   __zpabi_mix__v_0
   LDA   HARGS+7
   STA   __zpabi_mix__v_1
   JMP   .cond_end@3
.cond_else@2:
   LDY   #$00
   LDA   (gp7),Y
   STA   __zpabi_mix__v_0
   BMI   .sx_neg@6
   LDA   #$00
   BEQ   .sx_done@7
.sx_neg@6:
   LDA   #$FF
.sx_done@7:
   STA   __zpabi_mix__v_1
   ASL
   ROR   __zpabi_mix__v_1
   ROR   __zpabi_mix__v_0
.cond_end@3:
   LDA   __zpabi_mix__v_1
   BMI   .sx_neg@8
   LDA   #$00
   BEQ   .sx_done@9
.sx_neg@8:
   LDA   #$FF
.sx_done@9:
   JSR   fin0@run1
   LDA   __local_f9__i12
   AND   #$03
   ASL
   TAX
   SEC
   LDA   __local_f9__i12
   LDX   #$FF
.div_loop@10:
   INX
   SBC   #$05
   BCS   .div_loop@10
   ADC   #$05
   BEQ   .if_end@4
   LDA   g3
   AND   g1
   STA   g2
   LDA   #$00
   STA   g2+1
   JMP   .loop@4_break
.if_end@4:
   INC   __local_f9__i12
   JMP   .loop@4_start
.loop@4_break:
.default@8:
.switch@0_break:
   JSR   fin0@run4
   LDA   #$7F
   SEC
   SBC   __zpabi_mix__v_0
   STA   __zpabi_mix__v_0
   LDA   #$00
   SBC   __zpabi_mix__v_1
   TAX
   LDA   __zpabi_mix__v_0
   RTS

f9@run2:
   SUBROUTINE

   STA   HARGS+2
   LDA   #$00
   STA   HARGS+3
   JMP   sdivmod16

main:
   SUBROUTINE

   LDA   #$00
   STA   __local_main__l13
   STA   __local_main__2
   STA   __local_main__l14
   INC   g3
   LDY   #$00
   LDA   (gp8),Y
   LDX   #$00
   JSR   f9
   LDA   __local_main__l14
   SEC
   SBC   #$FF
   STA   __local_main__l14
   LDY   #$00
   LDA   (gp8),Y
   LSR
   LSR
   STA   (gp8),Y
   LDA   (gp8),Y
   CLC
   ADC   #$01
   STA   (gp8),Y
   AND   #$07
   BEQ   .case@11
   CMP   #$03
   BEQ   .case@12
   CMP   #$04
   BEQ   .case@13
   JMP   .default@20
.case@11:
   LDX   #$00
   LDA   #$02
   JSR   f9
   STA   __local_main__l13
   TXA
   STA   __local_main__2
   LDA   gs6+3
   STA   __local_main__l14
   LDA   gs6+4
   STA   __local_main__0
   LDA   g1
   EOR   #$02
   STA   HARGS
   LDA   #$00
   STA   HARGS+1
   LDA   __local_main__l14
   STA   HARGS+2
   LDA   __local_main__0
   STA   HARGS+3
   JSR   mul16
   LDA   HARGS+4
   STA   gs6+3
   LDA   HARGS+5
   STA   gs6+4
   JMP   .switch@10_break
.case@12:
   LDY   #$00
   LDA   (gp7),Y
   SEC
   SBC   #$01
   STA   (gp7),Y
   JMP   .switch@10_break
.case@13:
   LDY   #$00
   LDA   (gp8),Y
   SEC
   SBC   #$FF
   STA   (gp8),Y
   AND   #$07
   CMP   #$01
   BEQ   .case@15
   CMP   #$05
   BEQ   .switch@14_break
   CMP   #$06
   BEQ   .case@17
   CMP   #$07
   BEQ   .case@18
   BNE   .default@19
.case@15:
   LDA   __local_main__l13
   STA   __local_main__1
   LDA   __local_main__2
   STA   __local_main__l14
   LDY   #$00
   LDA   (gp8),Y
   CLC
   ADC   #$01
   STA   __local_main__0
   STA   (gp8),Y
   LDA   __local_main__1
   ORA   __local_main__0
   STA   ga5+6
   LDA   __local_main__l14
   STA   ga5+7
.case@17:
.case@18:
.default@19:
.switch@14_break:
.default@20:
.switch@10_break:
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

ga5:
   DC.W  $0002
   DC.W  $00FF
   DS.B  2
   DC.W  $7FFF

gs6:
   DS.B  1
   DC.W  $0080
   DC.W  $0080

__zp_init:
   SUBROUTINE

   LDA   #$00
   STA   g2+1
   STA   z4
   STA   z4+1
   STA   z4+3
   LDA   #$6E
   STA   g1
   LDA   #$02
   STA   g2
   LDA   #$7F
   STA   g3
   LDA   #$01
   STA   z4+2
   LDA   #<g3
   STA   gp7
   LDA   #>g3
   STA   gp7+1
   LDA   #<g1
   STA   gp8
   LDA   #>g1
   STA   gp8+1
   RTS
