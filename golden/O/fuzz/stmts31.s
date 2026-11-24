SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
z4	EQU	$26
@2.s11	EQU	$28
g1	EQU	$2A
g2	EQU	$2C
__zpabi_mix__v_0	EQU	$80
__zpabi_mix__v_1	EQU	$81
__zpabi_mix__v_2	EQU	$82
__zpabi_mix__v_3	EQU	$83
__local_main__w16	EQU	$86
__local_main__l13	EQU	$88
__local_f7__0	EQU	$8A
__local_f7__1	EQU	$8B
__local_f7__3	EQU	$8D
__local_f7__l9	EQU	$8E
__local_mix__0	EQU	$8E
__local_f7__4	EQU	$8F
__local_mix__1	EQU	$8F
__local_mix__2	EQU	$90
__local_mix__3	EQU	$91

; @zp-link-meta-begin
; def f7 params=__zpabi_f7__pp8_0,__zpabi_f7__pp8_1 locals=__local_f7__0,__local_f7__1,__local_f7__2,__local_f7__3,__local_f7__l9,__local_f7__4 indirect=false in_cycle=false param_regs=A,X
; def fin0 params= locals=__local_fin0__0,__local_fin0__1,__local_fin0__2,__local_fin0__3 indirect=false in_cycle=false
; def fin1 params= locals=__local_fin1__0,__local_fin1__1,__local_fin1__2,__local_fin1__3 indirect=false in_cycle=false
; def fin2 params= locals=__local_fin2__0,__local_fin2__1 indirect=false in_cycle=false
; def main params= locals=__local_main__0,__local_main__1,__local_main__w16,__local_main__2,__local_main__l13,__local_main__4 indirect=false in_cycle=false
; def mix params=__zpabi_mix__v_0,__zpabi_mix__v_1,__zpabi_mix__v_2,__zpabi_mix__v_3 locals=__local_mix__0,__local_mix__1,__local_mix__2,__local_mix__3 indirect=false in_cycle=false
; call fin0 -> mix
; call fin1 -> mix
; call fin2 -> mix
; call main -> f7
; call main -> fin0
; call main -> fin1
; call main -> fin2
; call main -> mix
; item mix section=.text local=false zero=false
; item fin0 section=.text local=false zero=false
; item fin0@run0 section=.text local=true zero=false
; item fin1 section=.text local=false zero=false
; item fin2 section=.text local=false zero=false
; item f7 section=.text local=false zero=false
; item main section=.text local=false zero=false
; item main@run1 section=.text local=true zero=false
; item acc section=.bss local=false zero=true
; item g3 section=.data local=false zero=false
; item ga5 section=.data local=false zero=false
; item gs6 section=.data local=false zero=false
; item __zp_init section=.text local=false zero=false
; zp z4 size=2 at=auto def=true init=$FF,$FF
; zp @2.s11 size=2 at=auto def=true init=$03,$00 weight=25 local=true
; zp g1 size=2 at=auto def=true init=$FF,$7F weight=25
; zp g2 size=8 at=auto def=true init=$00,$00,$00,$00,$00,$00,$00,$00 weight=26
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
   LDA   z4+1
   BMI   .sx_neg@2
   LDA   #$00
   BEQ   .sx_done@3
.sx_neg@2:
   LDA   #$FF
.sx_done@3:
   STA   __zpabi_mix__v_2
   STA   __zpabi_mix__v_3
   LDA   z4
   STA   __zpabi_mix__v_0
   LDA   z4+1
   STA   __zpabi_mix__v_1
   JMP   mix

fin0@run0:
   SUBROUTINE

   STA   __zpabi_mix__v_3
   JMP   mix

fin1:
   SUBROUTINE

   LDA   ga5
   STA   __zpabi_mix__v_0
   LDA   ga5+1
   STA   __zpabi_mix__v_1
   LDA   ga5+2
   STA   __zpabi_mix__v_2
   LDA   ga5+3
   JSR   fin0@run0
   LDA   ga5+4
   STA   __zpabi_mix__v_0
   LDA   ga5+5
   STA   __zpabi_mix__v_1
   LDA   ga5+6
   STA   __zpabi_mix__v_2
   LDA   ga5+7
   JSR   fin0@run0
   LDA   ga5+8
   STA   __zpabi_mix__v_0
   LDA   ga5+9
   STA   __zpabi_mix__v_1
   LDA   ga5+10
   STA   __zpabi_mix__v_2
   LDA   ga5+11
   JSR   fin0@run0
   LDA   ga5+12
   STA   __zpabi_mix__v_0
   LDA   ga5+13
   STA   __zpabi_mix__v_1
   LDA   ga5+14
   STA   __zpabi_mix__v_2
   LDA   ga5+15
   STA   __zpabi_mix__v_3
   JMP   mix

fin2:
   SUBROUTINE

   LDA   gs6+1
   BMI   .sx_neg@4
   LDA   #$00
   BEQ   .sx_done@5
.sx_neg@4:
   LDA   #$FF
.sx_done@5:
   STA   __zpabi_mix__v_1
   STA   __zpabi_mix__v_2
   STA   __zpabi_mix__v_3
   LDA   gs6+1
   STA   __zpabi_mix__v_0
   JSR   mix
   LDA   gs6+3
   BMI   .sx_neg@6
   LDA   #$00
   BEQ   .sx_done@7
.sx_neg@6:
   LDA   #$FF
.sx_done@7:
   STA   __zpabi_mix__v_2
   STA   __zpabi_mix__v_3
   LDA   gs6+2
   STA   __zpabi_mix__v_0
   LDA   gs6+3
   STA   __zpabi_mix__v_1
   JMP   mix

f7:
   SUBROUTINE

   LDA   #$03
   STA   __local_f7__l9
   LDA   #$00
   STA   __local_f7__4
   LDA   g2
   ORA   g2+1
   ORA   g2+2
   ORA   g2+3
   ORA   g2+4
   ORA   g2+5
   ORA   g2+6
   ORA   g2+7
   BNE   .lb_skip@0
   JMP   .if_end@0
.lb_skip@0:
   LDA   ga5
   AND   #$03
   STA   __local_f7__3
   LDA   #$00
   LDY   #$00
   STY   ga5+1
   STA   ga5+2
   LDA   __local_f7__3
   STA   ga5
   STY   ga5+3
   LDA   @2.s11
   STA   HARGS
   LDA   @2.s11+1
   STA   HARGS+1
   LDA   #$06
   STA   HARGS+2
   STY   HARGS+3
   JSR   sdivmod16
   LDA   HARGS+6
   AND   #$07
   CMP   #$03
   BEQ   .case@2
   CMP   #$05
   BEQ   .case@3
   BNE   .default@4
.case@2:
   LDA   g3+1
   LSR
   STA   __local_f7__1
   LDA   g3
   ROR
   LSR   __local_f7__1
   ROR
   STA   g3
   STA   ga5+8
   LDA   __local_f7__1
   STA   g3+1
   STA   ga5+9
   LDA   #$00
   STA   ga5+10
   STA   ga5+11
   LDA   __local_f7__l9
   STA   __local_f7__0
   LDA   __local_f7__4
   STA   __local_f7__1
   INC   __local_f7__0
   BNE   .inc_done@0
   INC   __local_f7__1
.inc_done@0:
   LDA   __local_f7__0
   STA   __local_f7__l9
   LDA   __local_f7__1
   STA   __local_f7__4
.case@3:
.default@4:
.if_end@0:
   LDA   gs6+2
   EOR   g3
   RTS

main:
   SUBROUTINE

   LDA   #$02
   STA   __local_main__l13
   LDX   #>__local_main__l13
   LDA   #<__local_main__l13
   JSR   f7
   LDA   gs6+1
   BMI   .sx_neg@8
   LDA   #$00
   BEQ   .sx_done@9
.sx_neg@8:
   LDA   #$FF
.sx_done@9:
   STA   __zpabi_mix__v_1
   LDA   gs6+2
   CLC
   ADC   gs6+1
   STA   __zpabi_mix__v_0
   LDA   gs6+3
   ADC   __zpabi_mix__v_1
   STA   __zpabi_mix__v_1
   BMI   .sx_neg@10
   LDA   #$00
   BEQ   .sx_done@11
.sx_neg@10:
   LDA   #$FF
.sx_done@11:
   JSR   main@run1
   JSR   f7
   STA   __zpabi_mix__v_0
   LDA   #$00
   STA   __zpabi_mix__v_1
   JSR   main@run1
   JSR   f7
   LDA   #$00
   STA   __local_main__w16
.loop@5_start:
   LDX   #>__local_main__l13
   LDA   #<__local_main__l13
   JSR   f7
   STA   __zpabi_mix__v_0
   LDA   #$00
   STA   __zpabi_mix__v_1
   STA   __zpabi_mix__v_2
   STA   __zpabi_mix__v_3
   JSR   mix
   LDA   g1
   BNE   .carry_inc@0
   DEC   g1+1
.carry_inc@0:
   DEC   g1
   INC   __local_main__w16
   LDA   __local_main__w16
   CMP   #$03
   BCC   .loop@5_start
   LDX   #>__local_main__l13
   LDA   #<__local_main__l13
   JSR   f7
   JSR   fin0
   JSR   fin1
   JSR   fin2
   LDA   acc+1
   AND   #$7F
   TAX
   LDA   acc
   RTS

main@run1:
   SUBROUTINE

   STA   __zpabi_mix__v_2
   JSR   fin0@run0
   LDX   #>__local_main__l13
   LDA   #<__local_main__l13
   RTS

acc:
   DS.B  2

g3:
   DC.W  $7FFF

ga5:
   DS.B  4
   DC.L  $AEE0DF31
   DC.L  $00000100
   DC.L  $0000007F

gs6:
   DS.B  2
   DC.W  $00FF

__zp_init:
   SUBROUTINE

   LDA   #$00
   STA   g2
   STA   g2+1
   STA   g2+2
   STA   g2+3
   STA   g2+4
   STA   g2+5
   STA   g2+6
   STA   g2+7
   STA   @2.s11+1
   LDA   #$FF
   STA   g1
   STA   z4
   STA   z4+1
   LDA   #$7F
   STA   g1+1
   LDA   #$03
   STA   @2.s11
   RTS
