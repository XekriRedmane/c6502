SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
z4	EQU	$26
z5	EQU	$27
z6	EQU	$29
@2.s13	EQU	$2A
g1	EQU	$2B
gp9	EQU	$2C
__zpabi_mix__v_0	EQU	$80
__zpabi_mix__v_1	EQU	$81
__zpabi_mix__v_2	EQU	$82
__zpabi_mix__v_3	EQU	$83
__local_f10__0	EQU	$84
__local_f10__w15	EQU	$85
__local_f10__1	EQU	$86
__local_f10__2	EQU	$87
__local_f10__3	EQU	$88
__local_fin1__0	EQU	$88
__local_f10__i14	EQU	$8A
__local_mix__0	EQU	$8C
__local_mix__1	EQU	$8D
__local_mix__2	EQU	$8E
__local_mix__3	EQU	$8F

; @zp-link-meta-begin
; def f10 params=__zpabi_f10__p11_0,__zpabi_f10__p11_1 locals=__local_f10__0,__local_f10__w15,__local_f10__1,__local_f10__2,__local_f10__3,__local_f10__4,__local_f10__i14 indirect=false in_cycle=false param_regs=A,X
; def fin0 params= locals=__local_fin0__0,__local_fin0__1,__local_fin0__2,__local_fin0__3 indirect=false in_cycle=false
; def fin1 params= locals=__local_fin1__0,__local_fin1__1 indirect=false in_cycle=false
; def fin2 params= locals=__local_fin2__0,__local_fin2__1 indirect=false in_cycle=false
; def main params= locals=__local_main__0,__local_main__1,__local_main__2,__local_main__3 indirect=false in_cycle=false
; def mix params=__zpabi_mix__v_0,__zpabi_mix__v_1,__zpabi_mix__v_2,__zpabi_mix__v_3 locals=__local_mix__0,__local_mix__1,__local_mix__2,__local_mix__3 indirect=false in_cycle=false
; call f10 -> mix
; call fin0 -> mix
; call fin1 -> mix
; call fin2 -> mix
; call main -> fin0
; call main -> fin1
; call main -> fin2
; call main -> mix
; item mix section=.text local=false zero=false
; item fin0 section=.text local=false zero=false
; item fin0@run3 section=.text local=true zero=false
; item fin0@run1 section=.text local=true zero=false
; item fin0@run0 section=.text local=true zero=false
; item fin0@tail0 section=.text local=true zero=false
; item fin1 section=.text local=false zero=false
; item fin1@run5 section=.text local=true zero=false
; item fin1@run4 section=.text local=true zero=false
; item fin1@run2 section=.text local=true zero=false
; item fin2 section=.text local=false zero=false
; item f10 section=.text local=false zero=false
; item main section=.text local=false zero=false
; item acc section=.bss local=false zero=true
; item g2 section=.data local=false zero=false
; item g3 section=.data local=false zero=false
; item ga7 section=.data local=false zero=false
; item gs8 section=.data local=false zero=false
; item __zp_init section=.text local=false zero=false
; zp z4 size=1 at=auto def=true init=$7F
; zp z5 size=2 at=auto def=true init=$0B,$00
; zp z6 size=1 at=auto def=true init=$00
; zp @2.s13 size=1 at=auto def=true init= weight=16
; zp g1 size=1 at=auto def=true init=$02 weight=5
; zp gp9 size=2 at=auto def=true init=<g2,>g2 weight=8
; runtime hargs_size=32 hargs_need=22
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
   JSR   fin0@run3
   LDA   z4
   JMP   fin0@tail0

fin0@run3:
   SUBROUTINE

   STA   __zpabi_mix__v_1
   LDA   #$00
   STA   __zpabi_mix__v_2
   JMP   fin0@run0

fin0@run1:
   SUBROUTINE

   STA   __zpabi_mix__v_0
   LDA   #$00
   STA   __zpabi_mix__v_1
   STA   __zpabi_mix__v_2
   RTS

fin0@run0:
   SUBROUTINE

   STA   __zpabi_mix__v_3
   JMP   mix

fin0@tail0:
   SUBROUTINE

   JSR   fin0@run1
   STA   __zpabi_mix__v_3
   JMP   mix

fin1:
   SUBROUTINE

   LDA   z5
   STA   __zpabi_mix__v_0
   LDA   z5+1
   STA   __local_fin1__0
   BMI   .sx_neg@0
   LDA   #$00
   BEQ   .sx_done@1
.sx_neg@0:
   LDA   #$FF
.sx_done@1:
   JSR   fin1@run4
   LDA   __local_fin1__0
   JSR   fin1@run2
   LDA   z6
   STA   __local_fin1__0
   BMI   .sx_neg@2
   LDA   #$00
   BEQ   .sx_done@3
.sx_neg@2:
   LDA   #$FF
.sx_done@3:
   STA   __zpabi_mix__v_1
   JSR   fin1@run4
   LDA   __local_fin1__0
   STA   __zpabi_mix__v_0
   JSR   mix
   LDA   ga7+1
   BMI   .sx_neg@4
   LDA   #$00
   BEQ   .sx_done@5
.sx_neg@4:
   LDA   #$FF
.sx_done@5:
   JSR   fin1@run4
   JSR   fin1@run5
   JSR   fin1@run2
   LDA   ga7+3
   BMI   .sx_neg@6
   LDA   #$00
   BEQ   .sx_done@7
.sx_neg@6:
   LDA   #$FF
.sx_done@7:
   JSR   fin1@run4
   LDA   ga7+2
   STA   __zpabi_mix__v_0
   LDA   ga7+3
   STA   __zpabi_mix__v_1
   JMP   mix

fin1@run5:
   SUBROUTINE

   LDA   ga7
   STA   __zpabi_mix__v_0
   LDA   ga7+1
   RTS

fin1@run4:
   SUBROUTINE

   STA   __zpabi_mix__v_2
   STA   __zpabi_mix__v_3
   RTS

fin1@run2:
   SUBROUTINE

   STA   __zpabi_mix__v_1
   JMP   mix

fin2:
   SUBROUTINE

   LDA   ga7+5
   BMI   .sx_neg@8
   LDA   #$00
   BEQ   .sx_done@9
.sx_neg@8:
   LDA   #$FF
.sx_done@9:
   JSR   fin1@run4
   LDA   ga7+4
   STA   __zpabi_mix__v_0
   LDA   ga7+5
   JSR   fin1@run2
   LDA   ga7+7
   BMI   .sx_neg@10
   LDA   #$00
   BEQ   .sx_done@11
.sx_neg@10:
   LDA   #$FF
.sx_done@11:
   JSR   fin1@run4
   LDA   ga7+6
   STA   __zpabi_mix__v_0
   LDA   ga7+7
   JSR   fin1@run2
   LDA   gs8+1
   STA   __zpabi_mix__v_0
   LDA   gs8+2
   JSR   fin0@run3
   LDA   gs8+3
   JMP   fin0@tail0

f10:
   SUBROUTINE

   LDA   #$00
   STA   __zpabi_mix__v_0
   STA   __zpabi_mix__v_1
   STA   __zpabi_mix__v_2
   JSR   fin0@run0
   LDA   #$00
   STA   __local_f10__i14
.loop@0_start:
   LDA   __local_f10__i14
   CMP   #$03
   BCC   .lb_skip@0
   JMP   .vthread@0
.lb_skip@0:
   LDY   #$00
   LDA   (gp9),Y
   STA   HARGS
   INY
   LDA   (gp9),Y
   STA   HARGS+1
   INY
   LDA   (gp9),Y
   STA   HARGS+2
   INY
   LDA   (gp9),Y
   STA   HARGS+3
   LDA   #$03
   STA   HARGS+4
   LDA   #$00
   STA   HARGS+5
   LDY   #$00
   STY   HARGS+6
   STA   HARGS+7
   JSR   sdivmod32
   LDA   __local_f10__i14
   BEQ   .cond_else@0
   LDA   gs8+1
   STA   __local_f10__w15
   LDA   gs8+2
   STA   __local_f10__0
   JMP   .cond_end@1
.cond_else@0:
   LDA   z5
   STA   __local_f10__w15
   LDA   z5+1
   STA   __local_f10__0
.cond_end@1:
   LDA   HARGS+12
   STA   HARGS
   LDA   HARGS+13
   STA   HARGS+1
   LDA   HARGS+14
   STA   HARGS+2
   LDA   HARGS+15
   STA   HARGS+3
   LDA   __local_f10__w15
   STA   HARGS+4
   LDA   __local_f10__0
   STA   HARGS+5
   LDA   #$00
   STA   HARGS+6
   STA   HARGS+7
   JSR   mul32
   LDA   HARGS+8
   STA   __zpabi_mix__v_0
   LDA   HARGS+9
   STA   __zpabi_mix__v_1
   LDA   HARGS+10
   STA   __zpabi_mix__v_2
   LDA   HARGS+11
   JSR   fin0@run0
   LDA   z5
   STA   __local_f10__2
   LDA   z5+1
   STA   __local_f10__1
   LDA   __local_f10__2
   SEC
   SBC   #$01
   STA   __local_f10__w15
   LDA   __local_f10__1
   SBC   #$00
   LDX   __local_f10__w15
   STX   z5
   STA   z5+1
   LDA   __local_f10__2
   CLC
   ADC   __local_f10__i14
   AND   #$07
   CMP   #$02
   BEQ   .case@2
   CMP   #$06
   BEQ   .case@3
   BNE   .switch@1_break
.case@2:
   LDA   z6
   STA   __local_f10__2
   BMI   .sx_neg@12
   LDA   #$00
   BEQ   .sx_done@13
.sx_neg@12:
   LDA   #$FF
.sx_done@13:
   STA   __local_f10__3
   LDX   #$06
.shift_loop@14:
   LDA   __local_f10__3
   ASL
   ROR   __local_f10__3
   ROR   __local_f10__2
   DEX
   BNE   .shift_loop@14
   LDA   __local_f10__i14
   AND   #$03
   ASL
   TAX
   LDA   ga7,X
   STA   __local_f10__0
   LDA   ga7+1,X
   STA   __local_f10__1
   LDA   __local_f10__0
   SEC
   SBC   #$01
   STA   __local_f10__0
   BCS   .carry_inc@1
   DEC   __local_f10__1
.carry_inc@1:
   STA   ga7,X
   LDA   __local_f10__1
   STA   ga7+1,X
   LDA   __local_f10__2
   EOR   __local_f10__0
   STA   __local_f10__w15
   LDA   __local_f10__3
   EOR   __local_f10__1
   ORA   __local_f10__w15
   BNE   .loop@0_continue
.case@3:
.switch@1_break:
   LDA   #$00
   STA   __local_f10__w15
   CLC
   JMP   .loop@4_continue
.f10@rot@0:
   INC   z4
   LDA   __local_f10__0
   STA   __local_f10__w15
.loop@4_continue:
   ADC   #$01
   STA   __local_f10__0
   LDA   __local_f10__w15
   CMP   #$02
   BCC   .f10@rot@0
.loop@0_continue:
   INC   __local_f10__i14
   JMP   .loop@0_start
.vthread@0:
   LDA   #$7F
   CLC
   ADC   g2
   STA   __local_f10__0
   LDA   #$00
   ADC   g2+1
   TAX
   LDA   __local_f10__0
   RTS

main:
   SUBROUTINE

   LDA   z6
   STA   __zpabi_mix__v_1
   BMI   .sx_neg@18
   LDA   #$00
   BEQ   .sx_done@19
.sx_neg@18:
   LDA   #$FF
.sx_done@19:
   STA   __zpabi_mix__v_0
   LDA   gs8+1
   CLC
   ADC   __zpabi_mix__v_1
   STA   __zpabi_mix__v_1
   LDA   gs8+2
   ADC   __zpabi_mix__v_0
   ORA   __zpabi_mix__v_1
   BNE   .or_true@4
   JSR   fin1@run5
   STA   __zpabi_mix__v_1
   LDX   #$06
.shift_loop@20:
   ASL   __zpabi_mix__v_0
   ROL   __zpabi_mix__v_1
   DEX
   BNE   .shift_loop@20
   LDA   __zpabi_mix__v_0
   ORA   __zpabi_mix__v_1
   BNE   .or_true@4
   LDA   #$00
   STA   __zpabi_mix__v_1
   JMP   .or_end@5
.or_true@4:
   LDA   #$01
   STA   __zpabi_mix__v_1
.or_end@5:
   ORA   #$00
   BEQ   .case@6
   LDA   __zpabi_mix__v_1
   CMP   #$02
   BEQ   .switch@5_break
   CMP   #$03
   BEQ   .case@11
   BNE   .default@13
.case@6:
   LDA   ga7+4
   AND   #$07
   CMP   #$03
   BEQ   .case@8
   CMP   #$05
   BEQ   .case@9
   BNE   .switch@7_break
.case@8:
   LDA   g1
   SEC
   SBC   #$5A
   STA   g1
   BNE   .switch@7_break
   LDA   gs8+3
   STA   ga7+4
   LDA   #$00
   STA   ga7+5
.case@9:
.switch@7_break:
   LDA   g1
   STA   __zpabi_mix__v_0
   LSR
   LSR
   STA   __zpabi_mix__v_1
   LDA   __zpabi_mix__v_0
   ROR
   ROR
   ROR
   AND   #$C0
   STA   __zpabi_mix__v_0
   LDA   __zpabi_mix__v_1
   BMI   .sx_neg@22
   LDA   #$00
   BEQ   .sx_done@23
.sx_neg@22:
   LDA   #$FF
.sx_done@23:
   STA   __zpabi_mix__v_2
   JSR   fin0@run0
   JMP   .switch@5_break
.case@11:
   LDA   g3+1
   AND   #$41
   ORA   g3
   BEQ   .if_end@8
   LDA   g3
   STA   g1
   LDA   gs8+3
   JSR   fin0@run1
   JSR   fin0@run0
.if_end@8:
.default@13:
.switch@5_break:
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
   DC.L  $80000000

g3:
   DC.W  $0003

ga7:
   DC.W  $C13C
   DC.W  $8000
   DC.W  $FFFF
   DC.W  $000B

gs8:
   DS.B  1
   DC.W  $0002
   DS.B  1

__zp_init:
   SUBROUTINE

   LDA   #$00
   STA   z5+1
   STA   z6
   LDA   #$02
   STA   g1
   LDA   #$7F
   STA   z4
   LDA   #$0B
   STA   z5
   LDA   #<g2
   STA   gp9
   LDA   #>g2
   STA   gp9+1
   RTS
