SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
z5	EQU	$26
g4	EQU	$2A
g3	EQU	$2B
gp9	EQU	$2F
g1	EQU	$31
g2	EQU	$33
__zpabi_mix__v_0	EQU	$80
__zpabi_mix__v_1	EQU	$81
__zpabi_mix__v_2	EQU	$82
__zpabi_mix__v_3	EQU	$83
__local_main__0	EQU	$84
__local_main__2	EQU	$86
__local_main__3	EQU	$87
__local_main__4	EQU	$88
__local_main__l22	EQU	$89
__local_f10__l11_1	EQU	$8E
__local_f10__l11_0	EQU	$8F
__local_f10__4	EQU	$90
__local_f10__5	EQU	$91
__local_mix__0	EQU	$94
__local_mix__1	EQU	$95
__local_mix__2	EQU	$96
__local_mix__3	EQU	$97

; @zp-link-meta-begin
; def f10 params= locals=__local_f10__0,__local_f10__1,__local_f10__2,__local_f10__3,__local_f10__l11_1,__local_f10__l11_0,__local_f10__4,__local_f10__5,__local_f10__6,__local_f10__7 indirect=false in_cycle=false
; def f14 params=__zpabi_f14__p15_0,__zpabi_f14__p15_1 locals=__local_f14__0 indirect=false in_cycle=false param_regs=A,X
; def fin0 params= locals=__local_fin0__0,__local_fin0__1 indirect=false in_cycle=false
; def fin1 params= locals=__local_fin1__0,__local_fin1__1,__local_fin1__2,__local_fin1__3 indirect=false in_cycle=false
; def fin2 params= locals=__local_fin2__0,__local_fin2__1,__local_fin2__2,__local_fin2__3 indirect=false in_cycle=false
; def main params= locals=__local_main__0,__local_main__1,__local_main__2,__local_main__3,__local_main__4,__local_main__l22 indirect=false in_cycle=false
; def mix params=__zpabi_mix__v_0,__zpabi_mix__v_1,__zpabi_mix__v_2,__zpabi_mix__v_3 locals=__local_mix__0,__local_mix__1,__local_mix__2,__local_mix__3 indirect=false in_cycle=false
; call f10 -> mix
; call fin0 -> mix
; call fin1 -> mix
; call fin2 -> mix
; call main -> f10
; call main -> f14
; call main -> fin0
; call main -> fin1
; call main -> fin2
; call main -> mix
; item mix section=.text local=false zero=false
; item fin0 section=.text local=false zero=false
; item fin0@run2 section=.text local=true zero=false
; item fin0@tail0 section=.text local=true zero=false
; item fin1 section=.text local=false zero=false
; item fin2 section=.text local=false zero=false
; item fin2@run4 section=.text local=true zero=false
; item f10 section=.text local=false zero=false
; item f10@run3 section=.text local=true zero=false
; item f10@run1 section=.text local=true zero=false
; item f10@run0 section=.text local=true zero=false
; item f14 section=.text local=false zero=false
; item main section=.text local=false zero=false
; item acc section=.bss local=false zero=true
; item ga6 section=.data local=false zero=false
; item gs7 section=.data local=false zero=false
; item gp8 section=.data local=false zero=false
; item __zp_init section=.text local=false zero=false
; zp z5 size=4 at=auto def=true init=$0C,$00,$00,$00
; zp g4 size=1 at=auto def=true init=$FF weight=12
; zp g3 size=4 at=auto def=true init=$FF,$FF,$FF,$7F weight=14
; zp gp9 size=2 at=auto def=true init=<g3,>g3 weight=7
; zp g1 size=2 at=auto def=true init=$FF,$7F weight=6
; zp g2 size=1 at=auto def=true init=$02 weight=3
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
   BMI   .sx_neg@2
   LDA   #$00
   BEQ   .sx_done@3
.sx_neg@2:
   LDA   #$FF
.sx_done@3:
   STA   __zpabi_mix__v_1
   STA   __zpabi_mix__v_2
   STA   __zpabi_mix__v_3
   LDA   g2
   STA   __zpabi_mix__v_0
   JSR   mix
   LDA   g3
   STA   __zpabi_mix__v_0
   LDA   g3+1
   STA   __zpabi_mix__v_1
   LDA   g3+2
   STA   __zpabi_mix__v_2
   LDA   g3+3
   JSR   fin0@run2
   LDA   g4
   STA   __zpabi_mix__v_0
   BMI   .sx_neg@4
   LDA   #$00
   BEQ   .sx_done@5
.sx_neg@4:
   LDA   #$FF
.sx_done@5:
   STA   __zpabi_mix__v_1
   STA   __zpabi_mix__v_2
   JMP   fin0@tail0

fin0@run2:
   SUBROUTINE

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
   STA   __zpabi_mix__v_1
   LDA   z5+2
   STA   __zpabi_mix__v_2
   LDA   z5+3
   JSR   fin0@run2
   LDA   ga6
   STA   __zpabi_mix__v_0
   LDA   ga6+1
   STA   __zpabi_mix__v_1
   LDA   ga6+2
   STA   __zpabi_mix__v_2
   LDA   ga6+3
   JSR   fin0@run2
   LDA   ga6+4
   STA   __zpabi_mix__v_0
   LDA   ga6+5
   STA   __zpabi_mix__v_1
   LDA   ga6+6
   STA   __zpabi_mix__v_2
   LDA   ga6+7
   JSR   fin0@run2
   LDA   ga6+8
   STA   __zpabi_mix__v_0
   LDA   ga6+9
   STA   __zpabi_mix__v_1
   LDA   ga6+10
   STA   __zpabi_mix__v_2
   LDA   ga6+11
   JMP   fin0@tail0

fin2:
   SUBROUTINE

   LDA   ga6+12
   STA   __zpabi_mix__v_0
   LDA   ga6+13
   STA   __zpabi_mix__v_1
   LDA   ga6+14
   STA   __zpabi_mix__v_2
   LDA   ga6+15
   JSR   fin0@run2
   JSR   fin2@run4
   STA   __zpabi_mix__v_1
   LDA   gs7+3
   STA   __zpabi_mix__v_2
   LDA   gs7+4
   JSR   fin0@run2
   LDA   gs7+5
   STA   __zpabi_mix__v_0
   LDA   #$00
   STA   __zpabi_mix__v_1
   STA   __zpabi_mix__v_2
   JMP   fin0@tail0

fin2@run4:
   SUBROUTINE

   LDA   gs7+1
   STA   __zpabi_mix__v_0
   LDA   gs7+2
   RTS

f10:
   SUBROUTINE

   LDY   #$00
   LDA   (gp9),Y
   STA   __local_f10__5
   INY
   LDA   (gp9),Y
   STA   __local_f10__4
   INY
   LDA   (gp9),Y
   STA   __local_f10__l11_0
   INY
   LDA   (gp9),Y
   STA   __local_f10__l11_1
   LDA   __local_f10__5
   SEC
   SBC   #$01
   STA   __zpabi_mix__v_0
   LDA   __local_f10__4
   SBC   #$00
   STA   __zpabi_mix__v_1
   LDA   __local_f10__l11_0
   SBC   #$00
   STA   __zpabi_mix__v_2
   LDA   __local_f10__l11_1
   SBC   #$00
   STA   __zpabi_mix__v_3
   LDA   __zpabi_mix__v_0
   LDY   #$00
   STA   (gp9),Y
   LDA   __zpabi_mix__v_1
   INY
   STA   (gp9),Y
   LDA   __zpabi_mix__v_2
   INY
   STA   (gp9),Y
   LDA   __zpabi_mix__v_3
   INY
   STA   (gp9),Y
   LDA   #$00
   EOR   __local_f10__5
   STA   __zpabi_mix__v_0
   LDA   #$01
   EOR   __local_f10__4
   STA   __zpabi_mix__v_1
   LDA   #$00
   EOR   __local_f10__l11_0
   STA   __zpabi_mix__v_2
   LDA   #$00
   EOR   __local_f10__l11_1
   JSR   f10@run3
   BEQ   .if_else@1
   LDY   #$03
   LDA   (gp9),Y
   STA   __zpabi_mix__v_2
   LDY   #$00
   LDA   (gp9),Y
   SEC
   SBC   #$01
   STA   __zpabi_mix__v_3
   INY
   LDA   (gp9),Y
   SBC   #$00
   STA   __zpabi_mix__v_0
   INY
   LDA   (gp9),Y
   SBC   #$00
   STA   __zpabi_mix__v_1
   BCS   .carry_inc@0
   DEC   __zpabi_mix__v_2
.carry_inc@0:
   LDA   __zpabi_mix__v_3
   LDY   #$00
   STA   (gp9),Y
   LDA   __zpabi_mix__v_0
   INY
   STA   (gp9),Y
   LDA   __zpabi_mix__v_1
   INY
   STA   (gp9),Y
   LDA   __zpabi_mix__v_2
   INY
   STA   (gp9),Y
   LDA   z5
   STA   gs7+1
   LDA   z5+1
   STA   gs7+2
   LDA   z5+2
   STA   gs7+3
   LDA   z5+3
   STA   gs7+4
   LDY   #$00
   LDA   (gp9),Y
   ORA   #$05
   STA   (gp9),Y
   STA   g4
   JMP   .if_end@0
.if_else@1:
   LDA   gs7+1
   SEC
   SBC   #$01
   STA   gs7+1
   LDA   gs7+2
   SBC   #$00
   STA   gs7+2
   LDA   gs7+3
   SBC   #$00
   STA   gs7+3
   BCS   .carry_inc@1
   DEC   gs7+4
.carry_inc@1:
.if_end@0:
   LDA   ga6+4
   STA   __local_f10__l11_1
   LDA   ga6+5
   STA   __local_f10__l11_0
   LDA   ga6+6
   STA   __local_f10__4
   LDA   ga6+7
   LSR
   ROR   __local_f10__4
   ROR   __local_f10__l11_0
   ROR   __local_f10__l11_1
   LSR
   STA   __local_f10__5
   ROR   __local_f10__4
   ROR   __local_f10__l11_0
   ROR   __local_f10__l11_1
   LDA   __local_f10__l11_1
   STA   ga6+4
   LDA   __local_f10__l11_0
   STA   ga6+5
   LDA   __local_f10__4
   STA   ga6+6
   LDA   __local_f10__5
   STA   ga6+7
   LDA   __local_f10__l11_1
   ORA   #$01
   STA   __zpabi_mix__v_0
   LDA   __local_f10__l11_0
   STA   __zpabi_mix__v_1
   LDA   __local_f10__4
   STA   __zpabi_mix__v_2
   LDA   __local_f10__5
   JSR   f10@run3
   BNE   .lb_skip@0
   JMP   .f10@asm_ssa_split@0
.lb_skip@0:
   INC   g3
   BNE   .inc_done@0
   INC   g3+1
   BNE   .inc_done@0
   INC   g3+2
   BNE   .inc_done@0
   INC   g3+3
.inc_done@0:
   LDA   z5
   CLC
   ADC   g3
   STA   __zpabi_mix__v_0
   LDA   z5+1
   ADC   g3+1
   STA   __zpabi_mix__v_1
   LDA   z5+2
   ADC   g3+2
   STA   __zpabi_mix__v_2
   LDA   z5+3
   ADC   g3+3
   JSR   fin0@run2
   LDA   #$00
   STA   __zpabi_mix__v_0
   STA   __zpabi_mix__v_1
   STA   __zpabi_mix__v_2
   JSR   fin0@run2
   LDA   #$00
   STA   __local_f10__l11_0
   STA   __local_f10__l11_1
.if_end@2:
   JSR   f10@run0
   LDY   #$00
   LDA   (DPTR),Y
   STA   HARGS
   INY
   LDA   (DPTR),Y
   STA   HARGS+1
   INY
   LDA   (DPTR),Y
   STA   HARGS+2
   INY
   LDA   (DPTR),Y
   STA   HARGS+3
   LDA   #$05
   STA   HARGS+4
   JSR   asl32
   LDA   HARGS+5
   STA   __zpabi_mix__v_0
   LDA   HARGS+6
   STA   __zpabi_mix__v_1
   LDA   HARGS+7
   STA   __zpabi_mix__v_2
   LDA   HARGS+8
   STA   __zpabi_mix__v_3
   JSR   f10@run0
   LDA   __zpabi_mix__v_0
   JSR   f10@run1
   JSR   mix
   JSR   f10@run0
   LDY   #$00
   LDA   (DPTR),Y
   STA   __zpabi_mix__v_0
   INY
   LDA   (DPTR),Y
   STA   __zpabi_mix__v_1
   INY
   LDA   (DPTR),Y
   STA   __zpabi_mix__v_2
   INY
   LDA   (DPTR),Y
   STA   __zpabi_mix__v_3
   LDA   __zpabi_mix__v_0
   EOR   #$5A
   STA   __zpabi_mix__v_0
   JSR   f10@run1
   LDA   __zpabi_mix__v_0
   STA   g1
   LDA   __zpabi_mix__v_1
   STA   g1+1
   LDX   __local_f10__l11_1
   LDA   __local_f10__l11_0
   RTS
.f10@asm_ssa_split@0:
   LDA   #$FF
   STA   __local_f10__l11_0
   STA   __local_f10__l11_1
   JMP   .if_end@2

f10@run3:
   SUBROUTINE

   STA   __zpabi_mix__v_3
   LDA   __zpabi_mix__v_0
   ORA   __zpabi_mix__v_1
   ORA   __zpabi_mix__v_2
   ORA   __zpabi_mix__v_3
   RTS

f10@run1:
   SUBROUTINE

   LDY   #$00
   STA   (DPTR),Y
   JSR   f10@run0
   LDA   __zpabi_mix__v_1
   LDY   #$01
   STA   (DPTR),Y
   JSR   f10@run0
   LDA   __zpabi_mix__v_2
   LDY   #$02
   STA   (DPTR),Y
   JSR   f10@run0
   LDA   __zpabi_mix__v_3
   LDY   #$03
   STA   (DPTR),Y
   RTS

f10@run0:
   SUBROUTINE

   LDA   gp8
   STA   DPTR
   LDA   gp8+1
   STA   DPTR+1
   RTS

f14:
   SUBROUTINE

   LDA   g2
   BEQ   .cond_else@3
   LDA   #$7F
   BNE   .cond_end@4
.cond_else@3:
   LDA   #$01
.cond_end@4:
   STA   gs7+5
   LDA   g3
   BMI   .sx_neg@6
   LDA   #$00
   BEQ   .sx_done@7
.sx_neg@6:
   LDA   #$FF
.sx_done@7:
   TAX
   LDA   g3
   RTS

main:
   SUBROUTINE

   LDA   #$00
   STA   __local_main__l22
   LDA   g4
   BEQ   .lnot_true@8
   LDA   #$00
   BEQ   .lnot_end@9
.lnot_true@8:
   LDA   #$01
.lnot_end@9:
   LDX   #$00
   JSR   f14
   STA   __zpabi_mix__v_0
   TXA
   BMI   .sx_neg@10
   LDA   #$00
   BEQ   .sx_done@11
.sx_neg@10:
   LDA   #$FF
.sx_done@11:
   STA   __zpabi_mix__v_2
   STA   __zpabi_mix__v_3
   STX   __zpabi_mix__v_1
   JSR   mix
   JSR   f10
   JSR   fin2@run4
   STA   __local_main__4
   LDA   gs7+3
   STA   __local_main__3
   LDA   gs7+4
   STA   __local_main__2
   LDA   #$00
   SEC
   SBC   __zpabi_mix__v_0
   STA   __local_main__0
   LDA   __zpabi_mix__v_0
   SBC   __local_main__4
   STA   __zpabi_mix__v_0
   LDA   __local_main__4
   SBC   __local_main__3
   STA   gs7+3
   LDA   __local_main__3
   SBC   __local_main__2
   STA   gs7+4
   LDA   __local_main__0
   STA   gs7+1
   LDA   __zpabi_mix__v_0
   STA   gs7+2
   LDA   __local_main__0
   AND   #$07
   CMP   #$01
   BNE   .lb_skip@2
   JMP   .switch@12_break
.lb_skip@2:
   CMP   #$03
   BEQ   .case@14
   CMP   #$04
   BEQ   .case@15
   CMP   #$07
   BNE   .lb_skip@1
   JMP   .case@16
.lb_skip@1:
   JMP   .switch@12_break
.case@14:
.case@15:
   LDA   gs7+1
   STA   __local_main__3
   LDA   gs7+2
   STA   __local_main__2
   LDA   gs7+3
   STA   __zpabi_mix__v_0
   LDA   gs7+4
   STA   __local_main__0
   LDA   g3
   AND   __local_main__3
   STA   __local_main__3
   LDA   g3+1
   AND   __local_main__2
   STA   __local_main__2
   LDA   g3+2
   AND   __zpabi_mix__v_0
   STA   __zpabi_mix__v_0
   LDA   g3+3
   AND   __local_main__0
   STA   __local_main__0
   LDA   #$00
   EOR   __local_main__3
   STA   __local_main__3
   LDA   #$00
   EOR   __local_main__2
   STA   __local_main__2
   LDA   #$7F
   EOR   __zpabi_mix__v_0
   STA   __zpabi_mix__v_0
   LDA   #$00
   EOR   __local_main__0
   STA   __local_main__0
   LDA   __local_main__3
   ORA   __local_main__2
   ORA   __zpabi_mix__v_0
   ORA   __local_main__0
   BEQ   .if_else@6
   LDA   ga6
   AND   ga6
   STA   g4
   LDA   g3
   ORA   g3+1
   ORA   g3+2
   ORA   g3+3
   BEQ   .if_else@8
   JSR   f10
   JMP   .if_end@7
.if_else@8:
   LDA   __local_main__l22
   STA   gs7+1
   BMI   .sx_neg@12
   LDA   #$00
   BEQ   .sx_done@13
.sx_neg@12:
   LDA   #$FF
.sx_done@13:
   STA   gs7+2
   STA   gs7+3
   STA   gs7+4
.if_end@7:
   LDA   g1
   BNE   .carry_inc@2
   DEC   g1+1
.carry_inc@2:
   DEC   g1
   LDA   g1
   CLC
   ADC   #$7F
   STA   __zpabi_mix__v_0
   LDA   g1+1
   ADC   #$00
   ORA   __zpabi_mix__v_0
   BEQ   .if_end@9
   JSR   f10
   STA   __local_main__l22
.if_end@9:
.if_else@6:
.case@16:
.switch@12_break:
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

ga6:
   DC.L  $00000002
   DS.B  4
   DC.L  $00007FFF
   DS.B  4

gs7:
   DS.B  1
   DC.L  $58752AD9
   DS.B  1

gp8:
   DC.W  z5

__zp_init:
   SUBROUTINE

   LDA   #$00
   STA   z5+1
   STA   z5+2
   STA   z5+3
   LDA   #$FF
   STA   g1
   STA   g3
   STA   g3+1
   STA   g3+2
   STA   g4
   LDA   #$7F
   STA   g1+1
   STA   g3+3
   LDA   #$02
   STA   g2
   LDA   #$0C
   STA   z5
   LDA   #<g3
   STA   gp9
   LDA   #>g3
   STA   gp9+1
   RTS
