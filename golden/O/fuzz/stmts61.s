SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
z4	EQU	$26
z5	EQU	$27
g3	EQU	$2B
gp8	EQU	$2C
__zpabi_mix__v_0	EQU	$80
__zpabi_mix__v_1	EQU	$81
__zpabi_mix__v_2	EQU	$82
__zpabi_mix__v_3	EQU	$83
__local_main__l14	EQU	$84
__local_main__0	EQU	$85
__local_main__1	EQU	$86
__local_main__2	EQU	$87
__local_main__l16	EQU	$88
__local_f9__l11	EQU	$89
__local_f9__0	EQU	$8A
__local_mix__0	EQU	$8D
__local_mix__1	EQU	$8E
__local_mix__2	EQU	$8F
__local_mix__3	EQU	$90

; @zp-link-meta-begin
; def f9 params=__zpabi_f9__pp10_0,__zpabi_f9__pp10_1 locals=__local_f9__l11,__local_f9__0 indirect=false in_cycle=false param_regs=A,X
; def fin0 params= locals=__local_fin0__0,__local_fin0__1 indirect=false in_cycle=false
; def fin1 params= locals=__local_fin1__0,__local_fin1__1,__local_fin1__2,__local_fin1__3 indirect=false in_cycle=false
; def fin2 params= locals=__local_fin2__0,__local_fin2__1 indirect=false in_cycle=false
; def main params= locals=__local_main__l14,__local_main__0,__local_main__1,__local_main__2,__local_main__l16 indirect=false in_cycle=false
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
; item fin0@run3 section=.text local=true zero=false
; item fin0@run2 section=.text local=true zero=false
; item fin0@run1 section=.text local=true zero=false
; item fin0@run0 section=.text local=true zero=false
; item fin1 section=.text local=false zero=false
; item fin2 section=.text local=false zero=false
; item f9 section=.text local=false zero=false
; item main section=.text local=false zero=false
; item acc section=.bss local=false zero=true
; item g1 section=.data local=false zero=false
; item g2 section=.data local=false zero=false
; item ga6 section=.data local=false zero=false
; item gs7 section=.data local=false zero=false
; item __zp_init section=.text local=false zero=false
; zp z4 size=1 at=auto def=true init=$FF
; zp z5 size=4 at=auto def=true init=$00,$80,$00,$00
; zp g3 size=1 at=auto def=true init=$00 weight=6
; zp gp8 size=2 at=auto def=true init=<g1,>g1 weight=6
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
   JSR   fin0@run0
   LDA   g2+1
   BMI   .sx_neg@0
   LDA   #$00
   BEQ   .sx_done@1
.sx_neg@0:
   LDA   #$FF
.sx_done@1:
   JSR   fin0@run1
   LDA   g2
   STA   __zpabi_mix__v_0
   LDA   g2+1
   JSR   fin0@run2
   LDA   g3
   BMI   .sx_neg@2
   LDA   #$00
   BEQ   .sx_done@3
.sx_neg@2:
   LDA   #$FF
.sx_done@3:
   JSR   fin0@run3
   LDA   g3
   STA   __zpabi_mix__v_0
   JSR   mix
   LDA   z4
   STA   __zpabi_mix__v_0
   BMI   .sx_neg@4
   LDA   #$00
   BEQ   .sx_done@5
.sx_neg@4:
   LDA   #$FF
.sx_done@5:
   JSR   fin0@run3
   JMP   mix

fin0@run3:
   SUBROUTINE

   STA   __zpabi_mix__v_1
   JMP   fin0@run1

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

   STA   __zpabi_mix__v_0
   LDA   #$00
   STA   __zpabi_mix__v_1
   JSR   fin0@run1
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
   STA   __zpabi_mix__v_3
   JSR   mix
   LDA   ga6+1
   BMI   .sx_neg@6
   LDA   #$00
   BEQ   .sx_done@7
.sx_neg@6:
   LDA   #$FF
.sx_done@7:
   JSR   fin0@run1
   LDA   ga6
   STA   __zpabi_mix__v_0
   LDA   ga6+1
   JSR   fin0@run2
   LDA   ga6+3
   BMI   .sx_neg@8
   LDA   #$00
   BEQ   .sx_done@9
.sx_neg@8:
   LDA   #$FF
.sx_done@9:
   JSR   fin0@run1
   LDA   ga6+2
   STA   __zpabi_mix__v_0
   LDA   ga6+3
   JSR   fin0@run2
   LDA   ga6+5
   BMI   .sx_neg@10
   LDA   #$00
   BEQ   .sx_done@11
.sx_neg@10:
   LDA   #$FF
.sx_done@11:
   JSR   fin0@run1
   LDA   ga6+4
   STA   __zpabi_mix__v_0
   LDA   ga6+5
   STA   __zpabi_mix__v_1
   JMP   mix

fin2:
   SUBROUTINE

   LDA   ga6+7
   BMI   .sx_neg@12
   LDA   #$00
   BEQ   .sx_done@13
.sx_neg@12:
   LDA   #$FF
.sx_done@13:
   JSR   fin0@run1
   LDA   ga6+6
   STA   __zpabi_mix__v_0
   LDA   ga6+7
   JSR   fin0@run2
   LDA   gs7+1
   BMI   .sx_neg@14
   LDA   #$00
   BEQ   .sx_done@15
.sx_neg@14:
   LDA   #$FF
.sx_done@15:
   JSR   fin0@run3
   LDA   gs7+1
   STA   __zpabi_mix__v_0
   JSR   mix
   LDA   gs7+3
   BMI   .sx_neg@16
   LDA   #$00
   BEQ   .sx_done@17
.sx_neg@16:
   LDA   #$FF
.sx_done@17:
   JSR   fin0@run1
   LDA   gs7+2
   STA   __zpabi_mix__v_0
   LDA   gs7+3
   STA   __zpabi_mix__v_1
   JMP   mix

f9:
   SUBROUTINE

   LDA   #$FF
   STA   __local_f9__l11
   LDY   #$00
   LDA   (gp8),Y
   CLC
   ADC   #$03
   STA   (gp8),Y
   JSR   fin0@run0
   LDA   #$02
   STA   g3
   BEQ   .cond_else@0
   LDA   g1
   STA   __local_f9__l11
   LDA   #$00
   BEQ   .vthread@0
.cond_else@0:
   LDA   z5
   STA   __local_f9__l11
   LDA   #$00
.vthread@0:
   SEC
   SBC   __local_f9__l11
   AND   #$07
   CMP   #$02
   BNE   .switch@0_break
   LDY   #$00
   LDA   (gp8),Y
   STA   __local_f9__0
   SEC
   SBC   #$01
   STA   (gp8),Y
   LDA   __local_f9__0
   EOR   #$FF
   STA   __zpabi_mix__v_0
   LDA   #$00
   EOR   #$7F
   STA   __zpabi_mix__v_1
   LDA   #$00
   STA   __zpabi_mix__v_2
   STA   __zpabi_mix__v_3
   JSR   mix
   LDA   #$FF
   STA   g2
.switch@0_break:
   LDA   g3
   EOR   #$5A
   STA   g3
   LDX   #$00
   LDA   #$01
   RTS

main:
   SUBROUTINE

   LDA   #$00
   STA   __local_main__l14
   LDA   #$FF
   STA   __local_main__l16
   INC   gs7+1
   BEQ   .if_end@4
   LDA   z4
   STA   __local_main__2
   BMI   .sx_neg@20
   LDA   #$00
   BEQ   .sx_done@21
.sx_neg@20:
   LDA   #$FF
.sx_done@21:
   STA   __local_main__1
   LDA   __local_main__2
   SEC
   SBC   ga6+2
   STA   __local_main__2
   LDA   __local_main__1
   SBC   ga6+3
   STA   __local_main__1
   LDA   gs7+2
   ORA   gs7+3
   BEQ   .and_false@5
   LDA   g2
   ORA   g2+1
   BEQ   .and_false@5
   LDA   #$01
   STA   __local_main__0
   JMP   .and_end@6
.and_false@5:
   LDA   #$00
   STA   __local_main__0
.and_end@6:
   LDA   __local_main__2
   SEC
   SBC   __local_main__0
   LDA   __local_main__1
   SBC   #$00
   BVC   .cmp_novf@22
   EOR   #$80
.cmp_novf@22:
   ASL
   LDA   #$00
   ROL
   EOR   #$01
   STA   gs7+2
   LDA   #$00
   STA   gs7+3
.if_end@4:
   LDX   #>__local_main__l16
   LDA   #<__local_main__l16
   JSR   f9
   STA   __local_main__l16
   ASL
   ASL
   ASL
   AND   #$03
   EOR   #$02
   SEC
   SBC   #$02
   STA   z4
   LDY   #$00
   LDA   (gp8),Y
   CLC
   ADC   #$01
   STA   __local_main__l14
   STA   (gp8),Y
   LDA   #$FF
   EOR   __local_main__l14
   STA   g2
   LDA   #$FF
   STA   g2+1
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
   DC.B  $FF

g2:
   DC.W  $0003

ga6:
   DC.W  $0100
   DC.W  $0080
   DC.W  $0100
   DC.W  $7FFF

gs7:
   DS.B  2
   DC.W  $00FF

__zp_init:
   SUBROUTINE

   LDA   #$00
   STA   g3
   STA   z5
   STA   z5+2
   STA   z5+3
   LDA   #$FF
   STA   z4
   LDA   #$80
   STA   z5+1
   LDA   #<g1
   STA   gp8
   LDA   #>g1
   STA   gp8+1
   RTS
