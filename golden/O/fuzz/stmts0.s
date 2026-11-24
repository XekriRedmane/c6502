SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
z4	EQU	$26
gp7	EQU	$27
g2	EQU	$29
__zpabi_mix__v_0	EQU	$80
__zpabi_mix__v_1	EQU	$81
__zpabi_mix__v_2	EQU	$82
__zpabi_mix__v_3	EQU	$83
__local_main__0	EQU	$84
__local_main__1	EQU	$85
__local_main__2	EQU	$86
__local_main__3	EQU	$87
__local_main__l17	EQU	$88
__local_mix__0	EQU	$90
__local_mix__1	EQU	$91
__local_mix__2	EQU	$92
__local_mix__3	EQU	$93

; @zp-link-meta-begin
; def f8 params=__zpabi_f8__pp9_0,__zpabi_f8__pp9_1 locals=__local_f8__0 indirect=false in_cycle=false param_regs=A,X
; def fin0 params= locals=__local_fin0__0,__local_fin0__1,__local_fin0__2,__local_fin0__3 indirect=false in_cycle=false
; def fin1 params= locals=__local_fin1__0,__local_fin1__1,__local_fin1__2,__local_fin1__3 indirect=false in_cycle=false
; def fin2 params= locals=__local_fin2__0 indirect=false in_cycle=false
; def main params= locals=__local_main__0,__local_main__1,__local_main__2,__local_main__3,__local_main__l17,__local_main__5,__local_main__6,__local_main__7 indirect=false in_cycle=false
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
; item fin0@run0 section=.text local=true zero=false
; item fin0@tail0 section=.text local=true zero=false
; item fin0@tail0@run2 section=.text local=true zero=false
; item fin1 section=.text local=false zero=false
; item fin2 section=.text local=false zero=false
; item f8 section=.text local=false zero=false
; item f8@run1 section=.text local=true zero=false
; item main section=.text local=false zero=false
; item acc section=.bss local=false zero=true
; item g1 section=.data local=false zero=false
; item g3 section=.data local=false zero=false
; item ga5 section=.data local=false zero=false
; item gs6 section=.data local=false zero=false
; item @2.s13 section=.data local=true zero=false
; item __zp_init section=.text local=false zero=false
; zp z4 size=1 at=auto def=true init=$FF
; zp gp7 size=2 at=auto def=true init=<g3,>g3 weight=22
; zp g2 size=4 at=auto def=true init=$00,$80,$00,$00 weight=14
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
   STA   __zpabi_mix__v_0
   LDA   g1+1
   STA   __zpabi_mix__v_1
   LDA   g1+2
   STA   __zpabi_mix__v_2
   LDA   g1+3
   JSR   fin0@run0
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
   LDA   g3+2
   STA   __zpabi_mix__v_2
   LDA   g3+3
   JSR   fin0@run0
   LDA   z4
   JMP   fin0@tail0

fin0@run0:
   SUBROUTINE

   STA   __zpabi_mix__v_3
   JMP   mix

fin0@tail0:
   SUBROUTINE

   JSR   fin0@tail0@run2
   STA   __zpabi_mix__v_3
   JMP   mix

fin0@tail0@run2:
   SUBROUTINE

   STA   __zpabi_mix__v_0
   LDA   #$00
   STA   __zpabi_mix__v_1
   STA   __zpabi_mix__v_2
   RTS

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
   JSR   fin0@tail0@run2
   JSR   fin0@run0
   LDA   gs6+2
   JMP   fin0@tail0

f8:
   SUBROUTINE

   LDA   g2
   SEC
   SBC   #$20
   STA   __zpabi_mix__v_0
   LDA   g2+1
   SBC   #$00
   STA   __zpabi_mix__v_1
   LDA   g2+2
   SBC   #$00
   STA   __zpabi_mix__v_2
   LDA   g2+3
   SBC   #$00
   JSR   fin0@run0
   INC   @2.s13
   BNE   .inc_done@0
   INC   @2.s13+1
.inc_done@0:
   LDA   @2.s13
   ORA   @2.s13+1
   BEQ   .if_end@2
   LDA   #$FF
   STA   g2
   LDA   #$00
   STA   g2+1
   STA   g2+2
   STA   g2+3
   LDA   gs6+2
   STA   z4
   LDA   gs6+1
   CLC
   ADC   #$05
   STA   gs6+1
   JSR   f8@run1
   RTS
.if_end@2:
   LDA   #$02
   JSR   f8@run1
   RTS

f8@run1:
   SUBROUTINE

   STA   HARGS+8
   LDA   #$00
   STA   HARGS+9
   LDY   #$00
   STY   HARGS+10
   STA   HARGS+11
   RTS

main:
   SUBROUTINE

   LDA   #$FF
   STA   __local_main__l17
   STA   __local_main__l17+1
   STA   __local_main__l17+2
   LDA   #$7F
   STA   __local_main__l17+3
   LDA   g1
   STA   z4
   LDA   ga5+4
   LDX   ga5+5
   STX   __local_main__3
   LDX   ga5+6
   STX   __local_main__2
   LDX   ga5+7
   STX   __local_main__1
   SEC
   SBC   #$01
   STA   __local_main__0
   LDA   __local_main__3
   SBC   #$00
   STA   __local_main__3
   LDA   __local_main__2
   SBC   #$00
   STA   __local_main__2
   BCS   .carry_inc@0
   DEC   __local_main__1
.carry_inc@0:
   LDA   __local_main__0
   STA   ga5+4
   LDA   __local_main__3
   STA   ga5+5
   LDA   __local_main__2
   STA   ga5+6
   LDA   __local_main__1
   STA   ga5+7
   LDA   __local_main__0
   CLC
   ADC   #$C6
   STA   __zpabi_mix__v_0
   LDA   __local_main__3
   ADC   #$9F
   STA   __zpabi_mix__v_1
   LDA   __local_main__2
   ADC   #$00
   STA   __zpabi_mix__v_2
   LDA   __local_main__1
   ADC   #$00
   JSR   fin0@run0
   LDY   #$00
   LDA   (gp7),Y
   ORA   #$81
   STA   (gp7),Y
   LDA   g2
   CLC
   ADC   #$FF
   STA   g2
   BCC   .carry_inc@1
   INC   g2+1
   BNE   .carry_inc@1
   INC   g2+2
   BNE   .carry_inc@1
   INC   g2+3
.carry_inc@1:
   LDA   ga5
   STA   __local_main__3
   LDA   ga5+1
   STA   __local_main__2
   LDA   ga5+2
   STA   __local_main__1
   LDA   ga5+3
   STA   __local_main__0
   LDA   g2
   CLC
   ADC   __local_main__3
   STA   __local_main__3
   LDA   g2+1
   ADC   __local_main__2
   STA   __local_main__2
   LDA   g2+2
   ADC   __local_main__1
   STA   __local_main__1
   LDA   g2+3
   ADC   __local_main__0
   STA   __local_main__0
   LDA   __local_main__3
   ORA   __local_main__2
   ORA   __local_main__1
   ORA   __local_main__0
   BEQ   .if_else@5
   LDA   #$FF
   STA   __local_main__3
   LDY   #$00
   STA   (gp7),Y
   LDA   #$01
   SEC
   SBC   __local_main__3
   STA   z4
   LDA   #$00
   INY
   STA   (gp7),Y
   INY
   STA   (gp7),Y
   INY
   STA   (gp7),Y
   LDY   #$00
   LDA   (gp7),Y
   STA   g1
   LDA   #$00
   STA   g1+1
   STY   g1+2
   STA   g1+3
   JMP   .if_end@4
.if_else@5:
   LDA   __local_main__l17
   SEC
   SBC   gs6+2
   LDA   __local_main__l17+1
   SBC   #$00
   LDA   __local_main__l17+2
   SBC   #$00
   LDA   __local_main__l17+3
   SBC   #$00
   BCS   .if_end@6
   LDA   g2
   SEC
   SBC   #$01
   STA   g2
   LDA   g2+1
   SBC   #$00
   STA   g2+1
   LDA   g2+2
   SBC   #$00
   STA   g2+2
   BCS   .carry_inc@2
   DEC   g2+3
.carry_inc@2:
   LDA   g2
   CLC
   ADC   gs6+2
   AND   #$07
   BEQ   .switch@1_break
   CMP   #$03
   BEQ   .case@3
   CMP   #$04
   BEQ   .case@4
   BNE   .switch@1_break
.case@3:
   LDX   #>__local_main__l17
   LDA   #<__local_main__l17
   JSR   f8
.case@4:
.switch@1_break:
.if_end@6:
.if_end@4:
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
   DC.L  $00007FFF

g3:
   DC.L  $000000FF

ga5:
   DC.L  $0000007F
   DC.L  $00000002
   DC.L  $0000FFFF
   DC.L  $00000003

gs6:
   DS.B  1
   DC.B  $FF
   DC.B  $08

@2.s13:
   DC.W  $007F

__zp_init:
   SUBROUTINE

   LDA   #$00
   STA   g2
   STA   g2+2
   STA   g2+3
   LDA   #$80
   STA   g2+1
   LDA   #$FF
   STA   z4
   LDA   #<g3
   STA   gp7
   LDA   #>g3
   STA   gp7+1
   RTS
