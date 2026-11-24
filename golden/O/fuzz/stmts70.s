SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
z3	EQU	$26
gp7	EQU	$27
@4.s15	EQU	$29
g1	EQU	$2A
__zpabi_f8__pp11_0	EQU	$80
__zpabi_f8__pp11_1	EQU	$81
__zpabi_mix__v_0	EQU	$82
__zpabi_mix__v_1	EQU	$83
__zpabi_mix__v_2	EQU	$84
__zpabi_mix__v_3	EQU	$85
__local_main__0	EQU	$86
__local_main__1	EQU	$87
__local_main__2	EQU	$88
__local_main__3	EQU	$89
__local_main__l16	EQU	$8A
__local_main__l17	EQU	$8B
__local_f8__0	EQU	$8F
__local_f8__1	EQU	$90
__local_f8__2	EQU	$91
__local_f8__3	EQU	$92
__local_mix__0	EQU	$93
__local_mix__1	EQU	$94
__local_mix__2	EQU	$95
__local_mix__3	EQU	$96

; @zp-link-meta-begin
; def f8 params=__zpabi_f8__p9,__zpabi_f8__p10,__zpabi_f8__pp11_0,__zpabi_f8__pp11_1 locals=__local_f8__0,__local_f8__1,__local_f8__2,__local_f8__3 indirect=false in_cycle=false param_regs=A,X,,
; def fin0 params= locals=__local_fin0__0,__local_fin0__1 indirect=false in_cycle=false
; def fin1 params= locals=__local_fin1__0,__local_fin1__1 indirect=false in_cycle=false
; def fin2 params= locals=__local_fin2__0,__local_fin2__1,__local_fin2__2,__local_fin2__3 indirect=false in_cycle=false
; def main params= locals=__local_main__0,__local_main__1,__local_main__2,__local_main__3,__local_main__l16,__local_main__l17,__local_main__5,__local_main__6,__local_main__7 indirect=false in_cycle=false
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
; item fin0@run1 section=.text local=true zero=false
; item fin1 section=.text local=false zero=false
; item fin1@run4 section=.text local=true zero=false
; item fin1@run0 section=.text local=true zero=false
; item fin2 section=.text local=false zero=false
; item f8 section=.text local=false zero=false
; item f8@run3 section=.text local=true zero=false
; item main section=.text local=false zero=false
; item main@run5 section=.text local=true zero=false
; item main@run2 section=.text local=true zero=false
; item acc section=.bss local=false zero=true
; item g2 section=.data local=false zero=false
; item ga4 section=.data local=false zero=false
; item gs5 section=.data local=false zero=false
; item gp6 section=.data local=false zero=false
; item __zp_init section=.text local=false zero=false
; zp z3 size=1 at=auto def=true init=$02
; zp gp7 size=2 at=auto def=true init=<g2,>g2 weight=7
; zp @4.s15 size=1 at=auto def=true init=$02 weight=3 local=true
; zp g1 size=1 at=auto def=true init=$00 weight=3
; runtime hargs_size=32 hargs_need=9
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
   LDA   g2
   STA   __zpabi_mix__v_0
   LDA   g2+1
   STA   __zpabi_mix__v_1
   LDA   g2+2
   STA   __zpabi_mix__v_2
   LDA   g2+3
   STA   __zpabi_mix__v_3
   JSR   mix
   LDA   z3
   BMI   .sx_neg@0
   LDA   #$00
   BEQ   .sx_done@1
.sx_neg@0:
   LDA   #$FF
.sx_done@1:
   STA   __zpabi_mix__v_1
   STA   __zpabi_mix__v_2
   STA   __zpabi_mix__v_3
   LDA   z3
   STA   __zpabi_mix__v_0
   JSR   mix
   LDA   ga4
   STA   __zpabi_mix__v_0
   LDA   ga4+1
   STA   __zpabi_mix__v_1
   LDA   #$00
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

fin1:
   SUBROUTINE

   LDA   ga4+2
   STA   __zpabi_mix__v_0
   LDA   ga4+3
   JSR   fin1@run4
   LDA   ga4+4
   STA   __zpabi_mix__v_0
   LDA   ga4+5
   JSR   fin1@run4
   LDA   ga4+6
   STA   __zpabi_mix__v_0
   LDA   ga4+7
   JSR   fin1@run4
   LDA   gs5+2
   BMI   .sx_neg@2
   LDA   #$00
   BEQ   .sx_done@3
.sx_neg@2:
   LDA   #$FF
.sx_done@3:
   STA   __zpabi_mix__v_2
   STA   __zpabi_mix__v_3
   LDA   gs5+1
   STA   __zpabi_mix__v_0
   LDA   gs5+2
   STA   __zpabi_mix__v_1
   JMP   mix

fin1@run4:
   SUBROUTINE

   STA   __zpabi_mix__v_1
   JMP   fin1@run0

fin1@run0:
   SUBROUTINE

   LDA   #$00
   STA   __zpabi_mix__v_2
   STA   __zpabi_mix__v_3
   JMP   mix

fin2:
   SUBROUTINE

   LDA   gs5+3
   STA   __zpabi_mix__v_0
   LDA   gs5+4
   STA   __zpabi_mix__v_1
   LDA   gs5+5
   STA   __zpabi_mix__v_2
   LDA   gs5+6
   STA   __zpabi_mix__v_3
   JMP   mix

f8:
   SUBROUTINE

   LDA   g1
   ASL
   LDA   #$00
   ROL
   STA   g1
   JSR   fin0@run1
   LDY   #$00
   LDA   (gp7),Y
   AND   #$07
   CMP   #$04
   BEQ   .case@1
   CMP   #$05
   BNE   .switch@0_break
   LDA   (gp7),Y
   STA   __local_f8__0
   INY
   LDA   (gp7),Y
   STA   __local_f8__3
   INY
   LDA   (gp7),Y
   STA   __local_f8__2
   INY
   LDA   (gp7),Y
   STA   __local_f8__1
   INC   __local_f8__0
   BNE   .inc_done@0
   INC   __local_f8__3
   BNE   .inc_done@0
   INC   __local_f8__2
   BNE   .inc_done@0
   INC   __local_f8__1
.inc_done@0:
   LDA   __local_f8__0
   LDY   #$00
   STA   (gp7),Y
   LDA   __local_f8__3
   INY
   STA   (gp7),Y
   LDA   __local_f8__2
   INY
   STA   (gp7),Y
   LDA   __local_f8__1
   INY
   STA   (gp7),Y
   LDA   __local_f8__0
   STA   gs5+3
   LDA   __local_f8__3
   STA   gs5+4
   LDA   __local_f8__2
   STA   gs5+5
   LDA   __local_f8__1
   STA   gs5+6
.case@1:
.switch@0_break:
   JSR   f8@run3
   JSR   f8@run3
   LDA   g2
   CLC
   ADC   #$15
   STA   @4.s15
   LDA   gp6
   STA   DPTR
   LDA   gp6+1
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   STA   __local_f8__1
   BMI   .sx_neg@4
   LDA   #$00
   BEQ   .sx_done@5
.sx_neg@4:
   LDA   #$FF
.sx_done@5:
   LSR
   LDA   __local_f8__1
   ROR
   TAX
   LDA   #$00
   ROR
   RTS

f8@run3:
   SUBROUTINE

   ASL   g2
   ROL   g2+1
   ROL   g2+2
   ROL   g2+3
   RTS

main:
   SUBROUTINE

   LDA   #$03
   STA   __local_main__l16
   LDA   #$00
   STA   __local_main__l17
   STA   __local_main__l17+1
   STA   __local_main__l17+2
   LDA   #$80
   STA   __local_main__l17+3
   LDA   ga4+4
   ORA   ga4+5
   BNE   .lb_skip@0
   JMP   .if_else@2
.lb_skip@0:
   LDA   gs5+1
   STA   __local_main__3
   LDA   gs5+2
   STA   __local_main__2
   BMI   .sx_neg@6
   LDA   #$00
   BEQ   .sx_done@7
.sx_neg@6:
   LDA   #$FF
.sx_done@7:
   STA   __local_main__1
   STA   __local_main__0
   LDA   __local_main__3
   ORA   __local_main__2
   ORA   __local_main__1
   ORA   __local_main__0
   BEQ   .if_end@3
   LDX   z3
   JSR   main@run2
   LDY   #$00
   LDA   (gp7),Y
   JSR   f8
   JSR   main@run5
   LDA   __local_main__l17
   STA   __local_main__2
   LDA   __local_main__l16
   STA   __local_main__0
   LDA   __local_main__l17+3
   BNE   .cmp_true@10
   LDA   __local_main__l17+2
   BNE   .cmp_true@10
   LDA   __local_main__l17+1
   BNE   .cmp_true@10
   LDA   __local_main__l17
   CMP   __local_main__0
   BNE   .cmp_true@10
   LDA   #$00
   BEQ   .cmp_end@11
.cmp_true@10:
   LDA   #$01
.cmp_end@11:
   TAX
   JSR   main@run2
   LDA   __local_main__2
   JSR   f8
   STA   __local_main__l16
.if_end@3:
   LDA   ga4+4
   LDX   ga4+5
   STX   __local_main__1
   SEC
   SBC   #$01
   STA   __local_main__0
   LDA   __local_main__1
   SBC   #$00
   STA   ga4+5
   LDA   __local_main__0
   STA   ga4+4
   STA   z3
   JMP   .if_end@1
.if_else@2:
   LDA   gs5+3
   STA   __local_main__2
   LDA   gs5+1
   EOR   #$80
   TAX
   JSR   main@run2
   LDA   __local_main__2
   JSR   f8
   JSR   main@run5
.if_end@1:
   LDA   gs5+1
   ORA   #$05
   STA   gs5+1
   ORA   z3
   AND   #$07
   CMP   #$02
   BEQ   .case@12
   CMP   #$06
   BEQ   .case@13
   BNE   .default@14
.case@12:
   LDY   #$00
   LDA   (gp7),Y
   STA   HARGS
   INY
   LDA   (gp7),Y
   STA   HARGS+1
   INY
   LDA   (gp7),Y
   STA   HARGS+2
   INY
   LDA   (gp7),Y
   STA   HARGS+3
   LDA   #$06
   STA   HARGS+4
   JSR   asl32
   LDA   HARGS+6
   STA   __local_main__2
   LDA   HARGS+7
   STA   __local_main__1
   LDA   HARGS+8
   STA   __local_main__0
   LDA   HARGS+5
   LDY   #$00
   STA   (gp7),Y
   LDA   __local_main__2
   INY
   STA   (gp7),Y
   LDA   __local_main__1
   INY
   STA   (gp7),Y
   LDA   __local_main__0
   INY
   STA   (gp7),Y
.case@13:
.default@14:
   JSR   fin0
   JSR   fin1
   JSR   fin2
   LDA   acc+1
   AND   #$7F
   TAX
   LDA   acc
   RTS

main@run5:
   SUBROUTINE

   STA   __zpabi_mix__v_0
   STX   __zpabi_mix__v_1
   JMP   fin1@run0

main@run2:
   SUBROUTINE

   LDA   #<__local_main__l17
   STA   __zpabi_f8__pp11_0
   LDA   #>__local_main__l17
   STA   __zpabi_f8__pp11_1
   RTS

acc:
   DS.B  2

g2:
   DC.L  $000000FF

ga4:
   DC.W  $FFFF
   DC.W  $0080
   DS.B  4

gs5:
   DS.B  3
   DC.L  $00000003

gp6:
   DC.W  z3

__zp_init:
   SUBROUTINE

   LDA   #$00
   STA   g1
   LDA   #$02
   STA   z3
   STA   @4.s15
   LDA   #<g2
   STA   gp7
   LDA   #>g2
   STA   gp7+1
   RTS
