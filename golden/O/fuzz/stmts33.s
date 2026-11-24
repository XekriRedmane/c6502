SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
z3	EQU	$26
z4	EQU	$28
z5	EQU	$29
g2	EQU	$2A
gp8	EQU	$2B
g1	EQU	$2D
__zpabi_f14__p16_0	EQU	$80
__zpabi_mix__v_0	EQU	$80
__zpabi_f14__p16_1	EQU	$81
__zpabi_mix__v_1	EQU	$81
__zpabi_mix__v_2	EQU	$82
__zpabi_mix__v_3	EQU	$83
__local_main__w23	EQU	$84
__local_main__0	EQU	$85
__local_main__1	EQU	$86
__local_main__6	EQU	$8B
__local_f14__0	EQU	$8D
__local_f9__0	EQU	$8D
__local_f14__1	EQU	$8E
__local_f9__1	EQU	$8E
__local_f14__2	EQU	$8F
__local_f9__2	EQU	$8F
__local_f14__3	EQU	$90
__local_f9__3	EQU	$90
__local_f14__4	EQU	$91
__local_f9__4	EQU	$91
__local_f14__5	EQU	$92
__local_f9__5	EQU	$92
__local_f14__6	EQU	$93
__local_f9__6	EQU	$93
__local_f9__7	EQU	$94
__local_mix__0	EQU	$95
__local_mix__1	EQU	$96
__local_mix__2	EQU	$97
__local_mix__3	EQU	$98

; @zp-link-meta-begin
; def f14 params=__zpabi_f14__p15_0,__zpabi_f14__p15_1,__zpabi_f14__p16_0,__zpabi_f14__p16_1 locals=__local_f14__0,__local_f14__1,__local_f14__2,__local_f14__3,__local_f14__4,__local_f14__5,__local_f14__6 indirect=false in_cycle=false param_regs=A,X,,
; def f9 params=__zpabi_f9__p10,__zpabi_f9__p11 locals=__local_f9__0,__local_f9__1,__local_f9__2,__local_f9__3,__local_f9__4,__local_f9__5,__local_f9__6,__local_f9__7 indirect=false in_cycle=false param_regs=A,X
; def fin0 params= locals=__local_fin0__0,__local_fin0__1 indirect=false in_cycle=false
; def fin1 params= locals=__local_fin1__0 indirect=false in_cycle=false
; def fin2 params= locals=__local_fin2__0,__local_fin2__1,__local_fin2__2,__local_fin2__3 indirect=false in_cycle=false
; def main params= locals=__local_main__w23,__local_main__0,__local_main__1,__local_main__2,__local_main__3,__local_main__4,__local_main__5,__local_main__6,__local_main__7 indirect=false in_cycle=false
; def mix params=__zpabi_mix__v_0,__zpabi_mix__v_1,__zpabi_mix__v_2,__zpabi_mix__v_3 locals=__local_mix__0,__local_mix__1,__local_mix__2,__local_mix__3 indirect=false in_cycle=false
; call f9 -> mix
; call fin0 -> mix
; call fin1 -> mix
; call fin2 -> mix
; call main -> f14
; call main -> f9
; call main -> fin0
; call main -> fin1
; call main -> fin2
; call main -> mix
; item mix section=.text local=false zero=false
; item fin0 section=.text local=false zero=false
; item fin0@run4 section=.text local=true zero=false
; item fin0@run3 section=.text local=true zero=false
; item fin0@run1 section=.text local=true zero=false
; item fin1 section=.text local=false zero=false
; item fin1@run2 section=.text local=true zero=false
; item fin1@run0 section=.text local=true zero=false
; item fin2 section=.text local=false zero=false
; item f9 section=.text local=false zero=false
; item f14 section=.text local=false zero=false
; item main section=.text local=false zero=false
; item main@run5 section=.text local=true zero=false
; item acc section=.bss local=false zero=true
; item ga6 section=.data local=false zero=false
; item gs7 section=.data local=false zero=false
; item @3.s13 section=.data local=true zero=false
; item __zp_init section=.text local=false zero=false
; zp z3 size=2 at=auto def=true init=$91,$92
; zp z4 size=1 at=auto def=true init=$80
; zp z5 size=1 at=auto def=true init=$13
; zp g2 size=1 at=auto def=true init=$FF weight=10
; zp gp8 size=2 at=auto def=true init=<z4,>z4 weight=12
; zp g1 size=2 at=auto def=true init=$00,$00 weight=10
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
   JSR   fin0@run3
   LDA   g2
   JSR   fin0@run4
   JSR   fin0@run1
   LDA   z3+1
   BMI   .sx_neg@2
   LDA   #$00
   BEQ   .sx_done@3
.sx_neg@2:
   LDA   #$FF
.sx_done@3:
   STA   __zpabi_mix__v_2
   STA   __zpabi_mix__v_3
   LDA   z3
   STA   __zpabi_mix__v_0
   LDA   z3+1
   JSR   fin0@run3
   LDA   z4
   JSR   fin0@run4
   STA   __zpabi_mix__v_3
   JMP   mix

fin0@run4:
   SUBROUTINE

   STA   __zpabi_mix__v_0
   LDA   #$00
   STA   __zpabi_mix__v_1
   STA   __zpabi_mix__v_2
   RTS

fin0@run3:
   SUBROUTINE

   STA   __zpabi_mix__v_1
   JMP   mix

fin0@run1:
   SUBROUTINE

   STA   __zpabi_mix__v_3
   JMP   mix

fin1:
   SUBROUTINE

   LDA   z5
   BMI   .sx_neg@4
   LDA   #$00
   BEQ   .sx_done@5
.sx_neg@4:
   LDA   #$FF
.sx_done@5:
   JSR   fin1@run0
   LDA   z5
   JSR   fin1@run2
   LDA   ga6
   BMI   .sx_neg@6
   LDA   #$00
   BEQ   .sx_done@7
.sx_neg@6:
   LDA   #$FF
.sx_done@7:
   JSR   fin1@run0
   LDA   ga6
   JSR   fin1@run2
   LDA   ga6+1
   BMI   .sx_neg@8
   LDA   #$00
   BEQ   .sx_done@9
.sx_neg@8:
   LDA   #$FF
.sx_done@9:
   JSR   fin1@run0
   LDA   ga6+1
   JSR   fin1@run2
   LDA   ga6+2
   BMI   .sx_neg@10
   LDA   #$00
   BEQ   .sx_done@11
.sx_neg@10:
   LDA   #$FF
.sx_done@11:
   JSR   fin1@run0
   LDA   ga6+2
   STA   __zpabi_mix__v_0
   JMP   mix

fin1@run2:
   SUBROUTINE

   STA   __zpabi_mix__v_0
   JMP   mix

fin1@run0:
   SUBROUTINE

   STA   __zpabi_mix__v_1
   STA   __zpabi_mix__v_2
   STA   __zpabi_mix__v_3
   RTS

fin2:
   SUBROUTINE

   LDA   ga6+3
   BMI   .sx_neg@12
   LDA   #$00
   BEQ   .sx_done@13
.sx_neg@12:
   LDA   #$FF
.sx_done@13:
   JSR   fin1@run0
   LDA   ga6+3
   JSR   fin1@run2
   LDA   gs7+2
   BMI   .sx_neg@14
   LDA   #$00
   BEQ   .sx_done@15
.sx_neg@14:
   LDA   #$FF
.sx_done@15:
   STA   __zpabi_mix__v_2
   STA   __zpabi_mix__v_3
   LDA   gs7+1
   STA   __zpabi_mix__v_0
   LDA   gs7+2
   JSR   fin0@run3
   LDA   gs7+3
   STA   __zpabi_mix__v_0
   LDA   gs7+4
   STA   __zpabi_mix__v_1
   LDA   gs7+5
   STA   __zpabi_mix__v_2
   LDA   gs7+6
   STA   __zpabi_mix__v_3
   JMP   mix

f9:
   SUBROUTINE

   LDA   gs7+3
   STA   __local_f9__7
   LDA   gs7+4
   STA   __local_f9__6
   LDA   gs7+5
   STA   __local_f9__5
   LDA   gs7+6
   STA   __local_f9__4
   LDA   gs7+1
   STA   __local_f9__3
   LDA   gs7+2
   STA   __local_f9__2
   BMI   .sx_neg@16
   LDA   #$00
   BEQ   .sx_done@17
.sx_neg@16:
   LDA   #$FF
.sx_done@17:
   STA   __local_f9__1
   STA   __local_f9__0
   LDA   __local_f9__7
   SEC
   SBC   __local_f9__3
   STA   __local_f9__3
   LDA   __local_f9__6
   SBC   __local_f9__2
   STA   __local_f9__2
   LDA   __local_f9__5
   SBC   __local_f9__1
   STA   __local_f9__1
   LDA   __local_f9__4
   SBC   __local_f9__0
   STA   __local_f9__0
   LDA   #$00
   AND   __local_f9__3
   STA   __zpabi_mix__v_0
   LDA   #$00
   AND   __local_f9__2
   STA   __zpabi_mix__v_1
   LDA   #$00
   AND   __local_f9__1
   STA   __zpabi_mix__v_2
   LDA   #$00
   AND   __local_f9__0
   JSR   fin0@run1
   LDA   @3.s13
   SEC
   SBC   #$03
   STA   @3.s13
   STA   gs7+1
   LDA   #$00
   STA   gs7+2
   LDA   gs7+3
   LDY   #$00
   AND   (gp8),Y
   STA   HARGS+16
   LDA   #$00
   STA   HARGS+17
   STY   HARGS+18
   STY   HARGS+19
   STY   HARGS+20
   STY   HARGS+21
   STY   HARGS+22
   STA   HARGS+23
   RTS

f14:
   SUBROUTINE

   LDA   __zpabi_f14__p16_0
   SEC
   SBC   #$81
   STA   gs7+3
   LDA   __zpabi_f14__p16_1
   SBC   #$00
   STA   gs7+4
   LDA   #$00
   STA   gs7+5
   STA   gs7+6
   LDA   #$FE
   SEC
   SBC   z4
   LDA   #$00
   ROL
   STA   __local_f14__6
   LDA   #$00
   BPL   .sx_done@19
   LDA   #$FF
.sx_done@19:
   STA   __local_f14__5
   STA   __local_f14__4
   STA   __local_f14__3
   STA   __local_f14__2
   STA   __local_f14__1
   STA   __local_f14__0
   LDA   #$00
   CMP   __local_f14__0
   BNE   .cmp_end@21
   CMP   __local_f14__1
   BNE   .cmp_end@21
   CMP   __local_f14__2
   BNE   .cmp_end@21
   CMP   __local_f14__3
   BNE   .cmp_end@21
   CMP   __local_f14__4
   BNE   .cmp_end@21
   CMP   __local_f14__5
   BNE   .cmp_end@21
   CMP   #$00
   BNE   .thread@0
   LDA   #$80
   CMP   __local_f14__6
   BEQ   .cmp_true@20
.thread@0:
   LDA   #$00
   BEQ   .cmp_end@21
.cmp_true@20:
   LDA   #$01
.cmp_end@21:
   AND   #$07
   CMP   #$01
   BEQ   .switch@7_break
   CMP   #$02
   BEQ   .switch@7_break
   CMP   #$06
.switch@7_break:
   LDA   #$01
   RTS

main:
   SUBROUTINE

   LDA   gs7+3
   STA   __local_main__w23
   LDA   gs7+4
   STA   __zpabi_f14__p16_0
   LDA   gs7+5
   STA   __local_main__1
   LDA   gs7+6
   STA   __local_main__0
   INC   __local_main__w23
   BNE   .inc_done@0
   INC   __zpabi_f14__p16_0
   BNE   .inc_done@0
   INC   __local_main__1
   BNE   .inc_done@0
   INC   __local_main__0
.inc_done@0:
   LDA   __local_main__w23
   STA   gs7+3
   ORA   __zpabi_f14__p16_0
   ORA   __local_main__1
   ORA   __local_main__0
   BNE   .lb_skip@1
   JMP   .if_else@3
.lb_skip@1:
   LDA   gs7+3
   STA   __local_main__6
   LDA   #$00
   LDY   #$00
   STY   gs7+4
   STY   gs7+5
   STA   gs7+6
   LDA   gs7+1
   STA   __zpabi_f14__p16_0
   LDA   gs7+2
   STA   __local_main__1
   LDA   z5
   BMI   .sx_neg@23
   TYA
   JMP   .sx_done@24
.sx_neg@23:
   LDA   #$FF
.sx_done@24:
   STA   __local_main__w23
   LDA   __zpabi_f14__p16_0
   SEC
   SBC   z5
   STA   __zpabi_f14__p16_0
   LDA   __local_main__1
   SBC   __local_main__w23
   STA   __local_main__1
   BMI   .sx_neg@25
   TYA
   JMP   .sx_done@26
.sx_neg@25:
   LDA   #$FF
.sx_done@26:
   STA   __local_main__0
   STA   __local_main__w23
   LDA   __local_main__6
   CLC
   ADC   __zpabi_f14__p16_0
   STA   __zpabi_f14__p16_0
   TYA
   ADC   __local_main__1
   STA   __local_main__1
   TYA
   ADC   __local_main__0
   STA   __local_main__0
   TYA
   ADC   __local_main__w23
   STA   __local_main__w23
   LDA   __zpabi_f14__p16_0
   ORA   __local_main__1
   ORA   __local_main__0
   ORA   __local_main__w23
   BEQ   .if_else@5
   LDA   g1
   LDY   #$FF
   STY   HARGS
   STA   HARGS+1
   JSR   mul8
   LDA   HARGS+2
   LDX   #$FF
   JSR   f9
   LDA   #$00
   STA   __local_main__w23
.loop@12_continue:
   CLC
   ADC   #$01
   STA   __local_main__0
   LDA   __local_main__w23
   CMP   #$03
   BCC   .lb_skip@0
   JMP   .if_end@2
.lb_skip@0:
   LDA   z3
   ORA   z3+1
   BNE   .or_true@6
   LDA   z5
   BNE   .or_true@6
   LDA   #$00
   BEQ   .or_end@7
.or_true@6:
   LDA   #$01
.or_end@7:
   LDX   #$FF
   JSR   f9
   LDA   HARGS+16
   STA   __zpabi_mix__v_0
   LDA   HARGS+17
   STA   __zpabi_mix__v_1
   LDA   HARGS+18
   STA   __zpabi_mix__v_2
   LDA   HARGS+19
   JSR   fin0@run1
   LDA   __local_main__0
   STA   __local_main__w23
   JMP   .loop@12_continue
.if_else@5:
   LDY   #$00
   LDA   (gp8),Y
   SEC
   SBC   #$80
   LDA   #$00
   ROL
   TAX
   LDA   #$00
   JSR   f9
   LDA   z4
   BEQ   .and_false@8
   LDA   g2
   BEQ   .and_false@8
   LDA   #$01
   STA   __local_main__w23
   JMP   .and_end@9
.and_false@8:
   LDA   #$00
   STA   __local_main__w23
.and_end@9:
   LDY   #$00
   LDA   (gp8),Y
   TAX
   LDA   __local_main__w23
   JSR   f9
   JMP   .if_end@2
.if_else@3:
   LDA   __local_main__w23
   SEC
   SBC   #$81
   STA   __local_main__w23
   LDA   __zpabi_f14__p16_0
   SBC   #$00
   STA   __zpabi_f14__p16_0
   LDA   __local_main__1
   SBC   #$00
   STA   __local_main__1
   BCS   .carry_inc@0
   DEC   __local_main__0
.carry_inc@0:
   LDA   __local_main__w23
   STA   gs7+3
   LDA   __zpabi_f14__p16_0
   STA   gs7+4
   LDA   __local_main__1
   STA   gs7+5
   LDA   __local_main__0
   STA   gs7+6
   LDA   __local_main__w23
   ORA   __zpabi_f14__p16_0
   ORA   __local_main__1
   ORA   __local_main__0
   BEQ   .if_end@10
   LDY   #$00
   LDA   (gp8),Y
   BEQ   .if_else@12
   JSR   main@run5
   LDX   gs7+5
   STX   __local_main__0
   LDX   gs7+6
   STX   __local_main__w23
   LDX   __zpabi_f14__p16_0
   STX   __zpabi_mix__v_0
   STA   __zpabi_mix__v_1
   LDA   __local_main__0
   STA   __zpabi_mix__v_2
   LDA   __local_main__w23
   JSR   fin0@run1
   JMP   .if_end@11
.if_else@12:
   JSR   main@run5
   STA   __local_main__1
   LDA   gs7+5
   STA   __local_main__0
   LDA   gs7+6
   STA   __local_main__w23
   LDA   __zpabi_f14__p16_0
   SEC
   SBC   #$FF
   LDA   __local_main__1
   SBC   #$00
   LDA   __local_main__0
   SBC   #$00
   LDA   __local_main__w23
   SBC   #$00
   LDA   #$00
   ROL
   STA   __local_main__w23
   LDA   __local_main__1
   CLC
   ADC   #$01
   STA   __zpabi_f14__p16_1
   LDX   #$00
   LDA   __local_main__w23
   JSR   f14
.if_end@11:
.if_end@10:
.if_end@2:
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

   LDA   gs7+3
   STA   __zpabi_f14__p16_0
   LDA   gs7+4
   RTS

acc:
   DS.B  2

ga6:
   DC.B  $FF, $03, $FF, $01

gs7:
   DS.B  1
   DC.W  $0080
   DC.L  $00000003

@3.s13:
   DC.B  $CB

__zp_init:
   SUBROUTINE

   LDA   #$00
   STA   g1
   STA   g1+1
   LDA   #$FF
   STA   g2
   LDA   #$91
   STA   z3
   LDA   #$92
   STA   z3+1
   LDA   #$80
   STA   z4
   LDA   #$13
   STA   z5
   LDA   #<z4
   STA   gp8
   LDA   #>z4
   STA   gp8+1
   RTS
