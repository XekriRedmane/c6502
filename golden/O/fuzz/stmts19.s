SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
z3	EQU	$26
z4	EQU	$2A
z5	EQU	$2C
gp8	EQU	$30
@4.s15	EQU	$32
g2	EQU	$34
__zpabi_f22__p24_0	EQU	$80
__zpabi_f22__p24_1	EQU	$81
__zpabi_f22__p24_2	EQU	$82
__zpabi_f22__p25_0	EQU	$83
__local_main__4	EQU	$84
__zpabi_f22__p25_1	EQU	$84
__local_main__5	EQU	$85
__zpabi_f22__p25_2	EQU	$85
__local_main__6	EQU	$86
__zpabi_f22__p25_3	EQU	$86
__zpabi_f22__p25_4	EQU	$87
__zpabi_f22__p25_5	EQU	$88
__zpabi_f22__p25_6	EQU	$89
__zpabi_f22__pp26_0	EQU	$8A
__zpabi_f22__pp26_1	EQU	$8B
__zpabi_f9__p11	EQU	$8C
__zpabi_f9__pp12_0	EQU	$8D
__zpabi_f9__pp12_1	EQU	$8E
__zpabi_mix__v_0	EQU	$8F
__zpabi_mix__v_1	EQU	$90
__zpabi_mix__v_2	EQU	$91
__zpabi_mix__v_3	EQU	$92
__local_f22__l27	EQU	$93
__local_f22__0	EQU	$94
__local_f22__1	EQU	$95
__local_f22__2	EQU	$96
__local_f22__3	EQU	$97
__local_f22__4	EQU	$98
__local_f22__5	EQU	$99
__local_f22__6	EQU	$9A
__local_f22__l30	EQU	$9B
__local_f17__0	EQU	$9F
__local_f17__1	EQU	$A0
__local_f17__2	EQU	$A1
__local_f17__3	EQU	$A2
__local_f17__i21	EQU	$A3
__local_f17__w20	EQU	$A4
__local_f17__l19	EQU	$A5
__local_f9__l14	EQU	$AD
__local_f9__0	EQU	$AE
__local_f9__1	EQU	$AF
__local_f9__2	EQU	$B0
__local_f9__3	EQU	$B1
__local_f9__4	EQU	$B2
__local_f9__5	EQU	$B3
__local_mix__0	EQU	$BB
__local_mix__1	EQU	$BC
__local_mix__2	EQU	$BD
__local_mix__3	EQU	$BE

; @zp-link-meta-begin
; def f17 params= locals=__local_f17__0,__local_f17__1,__local_f17__2,__local_f17__3,__local_f17__i21,__local_f17__w20,__local_f17__l19,__local_f17__5,__local_f17__6,__local_f17__7,__local_f17__8,__local_f17__9,__local_f17__10,__local_f17__11 indirect=false in_cycle=false
; def f22 params=__zpabi_f22__p23,__zpabi_f22__p24_0,__zpabi_f22__p24_1,__zpabi_f22__p24_2,__zpabi_f22__p25_0,__zpabi_f22__p25_1,__zpabi_f22__p25_2,__zpabi_f22__p25_3,__zpabi_f22__p25_4,__zpabi_f22__p25_5,__zpabi_f22__p25_6,__zpabi_f22__pp26_0,__zpabi_f22__pp26_1 locals=__local_f22__l27,__local_f22__0,__local_f22__1,__local_f22__2,__local_f22__3,__local_f22__4,__local_f22__5,__local_f22__6,__local_f22__l30,__local_f22__8,__local_f22__9,__local_f22__10 indirect=false in_cycle=false param_regs=A,,,,,,,,,,,,
; def f9 params=__zpabi_f9__p10_0,__zpabi_f9__p10_1,__zpabi_f9__p11,__zpabi_f9__pp12_0,__zpabi_f9__pp12_1 locals=__local_f9__l14,__local_f9__0,__local_f9__1,__local_f9__2,__local_f9__3,__local_f9__4,__local_f9__5,__local_f9__6,__local_f9__7,__local_f9__8,__local_f9__9,__local_f9__10,__local_f9__11,__local_f9__12 indirect=false in_cycle=false param_regs=A,X,,,
; def fin0 params= locals=__local_fin0__0,__local_fin0__1 indirect=false in_cycle=false
; def fin1 params= locals=__local_fin1__0,__local_fin1__1 indirect=false in_cycle=false
; def fin2 params= locals=__local_fin2__0,__local_fin2__1,__local_fin2__2,__local_fin2__3 indirect=false in_cycle=false
; def main params= locals=__local_main__0,__local_main__1,__local_main__2,__local_main__3,__local_main__4,__local_main__5,__local_main__6 indirect=false in_cycle=false
; def mix params=__zpabi_mix__v_0,__zpabi_mix__v_1,__zpabi_mix__v_2,__zpabi_mix__v_3 locals=__local_mix__0,__local_mix__1,__local_mix__2,__local_mix__3 indirect=false in_cycle=false
; call f17 -> f9
; call f17 -> mix
; call f22 -> f17
; call f22 -> f9
; call f22 -> mix
; call f9 -> mix
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
; item fin0@run0 section=.text local=true zero=false
; item fin1 section=.text local=false zero=false
; item fin1@run8 section=.text local=true zero=false
; item fin1@run4 section=.text local=true zero=false
; item fin2 section=.text local=false zero=false
; item f9 section=.text local=false zero=false
; item f9@run6 section=.text local=true zero=false
; item f17 section=.text local=false zero=false
; item f17@run7 section=.text local=true zero=false
; item f17@run1 section=.text local=true zero=false
; item f22 section=.text local=false zero=false
; item f22@run5 section=.text local=true zero=false
; item f22@run2 section=.text local=true zero=false
; item main section=.text local=false zero=false
; item acc section=.bss local=false zero=true
; item g1 section=.bss local=false zero=true
; item ga6 section=.data local=false zero=false
; item gs7 section=.data local=false zero=false
; item __zp_init section=.text local=false zero=false
; zp z3 size=4 at=auto def=true init=$FF,$7F,$00,$00
; zp z4 size=2 at=auto def=true init=$C2,$C6
; zp z5 size=4 at=auto def=true init=$01,$00,$00,$00
; zp gp8 size=2 at=auto def=true init=<g1,>g1 weight=67
; zp @4.s15 size=2 at=auto def=true init=$03,$00 weight=16 local=true
; zp g2 size=1 at=auto def=true init=$0F weight=6
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
   BMI   .sx_neg@0
   LDA   #$00
   BEQ   .sx_done@1
.sx_neg@0:
   LDA   #$FF
.sx_done@1:
   STA   __zpabi_mix__v_1
   JSR   fin0@run3
   LDA   g1
   STA   __zpabi_mix__v_0
   JSR   mix
   LDA   g2
   BMI   .sx_neg@2
   LDA   #$00
   BEQ   .sx_done@3
.sx_neg@2:
   LDA   #$FF
.sx_done@3:
   STA   __zpabi_mix__v_1
   JSR   fin0@run3
   LDA   g2
   STA   __zpabi_mix__v_0
   JSR   mix
   LDA   z3
   STA   __zpabi_mix__v_0
   LDA   z3+1
   STA   __zpabi_mix__v_1
   LDA   z3+2
   STA   __zpabi_mix__v_2
   LDA   z3+3
   JSR   fin0@run0
   LDA   z4+1
   BMI   .sx_neg@4
   LDA   #$00
   BEQ   .sx_done@5
.sx_neg@4:
   LDA   #$FF
.sx_done@5:
   JSR   fin0@run3
   LDA   z4
   STA   __zpabi_mix__v_0
   LDA   z4+1
   STA   __zpabi_mix__v_1
   JMP   mix

fin0@run3:
   SUBROUTINE

   STA   __zpabi_mix__v_2
   STA   __zpabi_mix__v_3
   RTS

fin0@run0:
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
   JSR   fin0@run0
   LDA   ga6+1
   BMI   .sx_neg@6
   LDA   #$00
   BEQ   .sx_done@7
.sx_neg@6:
   LDA   #$FF
.sx_done@7:
   JSR   fin0@run3
   LDA   ga6
   STA   __zpabi_mix__v_0
   LDA   ga6+1
   JSR   fin1@run4
   LDA   ga6+3
   BMI   .sx_neg@8
   LDA   #$00
   BEQ   .sx_done@9
.sx_neg@8:
   LDA   #$FF
.sx_done@9:
   JSR   fin0@run3
   JSR   fin1@run8
   JSR   fin1@run4
   LDA   ga6+5
   BMI   .sx_neg@10
   LDA   #$00
   BEQ   .sx_done@11
.sx_neg@10:
   LDA   #$FF
.sx_done@11:
   JSR   fin0@run3
   LDA   ga6+4
   STA   __zpabi_mix__v_0
   LDA   ga6+5
   STA   __zpabi_mix__v_1
   JMP   mix

fin1@run8:
   SUBROUTINE

   LDA   ga6+2
   STA   __zpabi_mix__v_0
   LDA   ga6+3
   RTS

fin1@run4:
   SUBROUTINE

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
   JSR   fin0@run3
   LDA   ga6+6
   STA   __zpabi_mix__v_0
   LDA   ga6+7
   JSR   fin1@run4
   LDA   gs7+1
   STA   __zpabi_mix__v_0
   LDA   gs7+2
   STA   __zpabi_mix__v_1
   LDA   gs7+3
   STA   __zpabi_mix__v_2
   LDA   gs7+4
   JSR   fin0@run0
   LDA   gs7+8
   STA   __zpabi_mix__v_0
   LDA   gs7+9
   STA   __zpabi_mix__v_1
   LDA   gs7+10
   STA   __zpabi_mix__v_2
   LDA   gs7+11
   STA   __zpabi_mix__v_3
   JMP   mix

f9:
   SUBROUTINE

   LDA   #$80
   STA   __local_f9__l14
   LDA   #$00
   STA   __local_f9__0
   LDA   g2
   BMI   .sx_neg@14
   LDA   #$00
   BEQ   .sx_done@15
.sx_neg@14:
   LDA   #$FF
.sx_done@15:
   STA   HARGS+1
   LDA   g2
   STA   HARGS
   LDA   #$06
   JSR   f9@run6
   LDA   HARGS+6
   ORA   HARGS+7
   BNE   .lb_skip@0
   JMP   .if_else@1
.lb_skip@0:
   LDA   ga6+5
   BMI   .sx_neg@16
   LDA   #$00
   BEQ   .sx_done@17
.sx_neg@16:
   LDA   #$FF
.sx_done@17:
   LDX   gs7+1
   STX   __local_f9__5
   LDX   gs7+2
   STX   __local_f9__4
   LDX   gs7+3
   STX   __local_f9__3
   LDX   gs7+4
   STX   __local_f9__2
   LDX   gs7+5
   STX   __local_f9__1
   LDX   gs7+6
   STX   __local_f9__0
   CMP   gs7+7
   BNE   .thread@0
   CMP   __local_f9__0
   BNE   .thread@0
   CMP   __local_f9__1
   BNE   .thread@0
   CMP   __local_f9__2
   BNE   .thread@0
   CMP   __local_f9__3
   BNE   .thread@0
   LDA   ga6+5
   CMP   __local_f9__4
   BNE   .thread@0
   LDA   ga6+4
   CMP   __local_f9__5
   BEQ   .cmp_true@18
.thread@0:
   LDA   #$00
   BEQ   .cmp_end@19
.cmp_true@18:
   LDA   #$01
.cmp_end@19:
   STA   __local_f9__0
   LDA   __zpabi_f9__p11
   AND   #$03
   STA   __local_f9__l14
   LDA   __local_f9__0
   SEC
   SBC   __local_f9__l14
   STA   __local_f9__0
   LDA   #$00
   SBC   #$00
   STA   __local_f9__l14
   BMI   .sx_neg@21
   LDA   #$00
   BEQ   .sx_done@22
.sx_neg@21:
   LDA   #$FF
.sx_done@22:
   JSR   fin0@run3
   LDA   __local_f9__0
   STA   __zpabi_mix__v_0
   LDA   __local_f9__l14
   JSR   fin1@run4
   LDA   @4.s15
   CLC
   ADC   #$FF
   STA   __local_f9__0
   LDA   @4.s15+1
   ADC   #$00
   STA   __local_f9__l14
   STA   @4.s15+1
   LDA   __local_f9__0
   STA   @4.s15
   STA   gs7+8
   LDA   __local_f9__l14
   STA   gs7+9
   BMI   .sx_neg@23
   LDA   #$00
   BEQ   .sx_done@24
.sx_neg@23:
   LDA   #$FF
.sx_done@24:
   STA   gs7+10
   STA   gs7+11
   LDA   #$00
   STA   HARGS+8
   LDY   #$00
   STY   HARGS+9
   STY   HARGS+10
   STA   HARGS+11
   RTS
.if_else@1:
   LDY   #$00
   LDA   (__zpabi_f9__pp12_0),Y
   STA   __zpabi_mix__v_0
   INY
   LDA   (__zpabi_f9__pp12_0),Y
   SEC
   SBC   #$80
   STA   __zpabi_mix__v_1
   INY
   LDA   (__zpabi_f9__pp12_0),Y
   SBC   #$00
   STA   __zpabi_mix__v_2
   INY
   LDA   (__zpabi_f9__pp12_0),Y
   SBC   #$00
   JSR   fin0@run0
   LDA   ga6+3
   BMI   .sx_neg@25
   LDA   #$00
   BEQ   .sx_done@26
.sx_neg@25:
   LDA   #$FF
.sx_done@26:
   STA   __local_f9__4
   STA   __local_f9__3
   LDA   gs7+8
   STA   __local_f9__2
   LDA   gs7+9
   STA   __local_f9__1
   LDA   gs7+10
   STA   __local_f9__0
   LDA   gs7+11
   STA   __local_f9__l14
   LDA   ga6+2
   AND   __local_f9__2
   STA   HARGS+8
   LDA   ga6+3
   AND   __local_f9__1
   STA   HARGS+9
   LDA   __local_f9__4
   AND   __local_f9__0
   STA   HARGS+10
   LDA   __local_f9__3
   AND   __local_f9__l14
   STA   HARGS+11
   RTS

f9@run6:
   SUBROUTINE

   STA   HARGS+2
   LDA   #$00
   STA   HARGS+3
   JMP   sdivmod16

f17:
   SUBROUTINE

   LDA   #$FF
   STA   __local_f17__l19
   STA   __local_f17__l19+1
   LDA   #$00
   STA   __local_f17__l19+2
   STA   __local_f17__l19+3
   STA   __local_f17__w20
.loop@4_start:
   LDA   #$00
   STA   __local_f17__i21
.loop@5_start:
   LDA   __local_f17__i21
   CMP   #$02
   BCC   .lb_skip@2
   JMP   .loop@5_break
.lb_skip@2:
   LDA   z5
   ORA   z5+1
   ORA   z5+2
   ORA   z5+3
   BEQ   .cond_else@2
   LDA   #$7F
   STA   __local_f17__3
   LDA   #$00
   STA   __local_f17__2
   JMP   .cond_end@3
.cond_else@2:
   LDY   #$00
   LDA   (gp8),Y
   STA   __local_f17__3
   BMI   .sx_neg@27
   LDA   #$00
   BEQ   .sx_done@28
.sx_neg@27:
   LDA   #$FF
.sx_done@28:
   STA   __local_f17__2
.cond_end@3:
   LDA   __local_f17__w20
   AND   #$03
   ASL
   TAX
   LDA   ga6,X
   STA   HARGS
   LDA   ga6+1,X
   STA   HARGS+1
   LDA   #$07
   JSR   f9@run6
   LDA   HARGS+4
   JSR   f17@run7
   LDX   __local_f17__2
   LDA   __local_f17__3
   JSR   f9
   JSR   f17@run1
   LDA   z5
   STA   HARGS
   LDA   z5+1
   STA   HARGS+1
   LDA   z5+2
   STA   HARGS+2
   LDA   z5+3
   STA   HARGS+3
   LDA   #$05
   STA   HARGS+4
   JSR   asl32
   LDA   HARGS+5
   STA   z5
   LDA   HARGS+6
   STA   __local_f17__2
   STA   z5+1
   LDA   HARGS+7
   STA   z5+2
   LDA   HARGS+8
   STA   z5+3
   LDA   __local_f17__i21
   AND   #$03
   ASL
   TAX
   LDA   HARGS+5
   STA   ga6,X
   LDA   __local_f17__2
   STA   ga6+1,X
   LDA   #$02
   SEC
   SBC   z5
   STA   gs7+1
   LDA   #$00
   SBC   z5+1
   STA   gs7+2
   LDA   #$00
   SBC   z5+2
   STA   gs7+3
   LDA   #$00
   SBC   z5+3
   STA   gs7+4
   LDA   #$00
   LDY   #$00
   STA   gs7+7
   LDA   gs7+1
   SEC
   SBC   #$05
   STA   gs7+1
   LDA   gs7+2
   SBC   #$00
   STA   gs7+2
   LDA   gs7+3
   SBC   #$00
   STA   gs7+3
   LDA   gs7+4
   SBC   #$00
   STA   gs7+4
   TYA
   SBC   #$00
   STA   gs7+5
   TYA
   SBC   #$00
   STA   gs7+6
   BCS   .carry_inc@0
   DEC   gs7+7
.carry_inc@0:
   INC   __local_f17__i21
   JMP   .loop@5_start
.loop@5_break:
   INC   __local_f17__w20
   BNE   .lb_skip@1
   JMP   .loop@4_start
.lb_skip@1:
   LDA   z3+3
   LSR
   STA   __local_f17__w20
   LDA   z3+2
   ROR
   STA   __local_f17__i21
   LDA   z3+1
   ROR
   STA   __local_f17__3
   LDA   z3
   ROR
   LSR   __local_f17__w20
   ROR   __local_f17__i21
   ROR   __local_f17__3
   ROR
   STA   __local_f17__2
   LDA   g2
   EOR   #$FF
   JSR   f17@run7
   LDX   __local_f17__3
   LDA   __local_f17__2
   JSR   f9
   LDA   gs7+8
   EOR   #$03
   STA   gs7+8
   AND   #$07
   CMP   #$02
   LDA   gs7+8
   LDX   gs7+9
   STX   __local_f17__2
   LDX   gs7+10
   STX   __local_f17__1
   LDX   gs7+11
   STX   __local_f17__0
   ORA   __local_f17__2
   ORA   __local_f17__1
   ORA   __local_f17__0
   BEQ   .cond_else@4
   LDA   g1
   BPL   .cond_end@5
   RTS
.cond_else@4:
   LDA   ga6+6
.cond_end@5:
   RTS

f17@run7:
   SUBROUTINE

   STA   __zpabi_f9__p11
   LDA   #<__local_f17__l19
   STA   __zpabi_f9__pp12_0
   LDA   #>__local_f17__l19
   STA   __zpabi_f9__pp12_1
   RTS

f17@run1:
   SUBROUTINE

   LDA   HARGS+8
   STA   __zpabi_mix__v_0
   LDA   HARGS+9
   STA   __zpabi_mix__v_1
   LDA   HARGS+10
   STA   __zpabi_mix__v_2
   LDA   HARGS+11
   JMP   fin0@run0

f22:
   SUBROUTINE

   LDA   #$80
   STA   __local_f22__l27
   LDA   #$00
   STA   __local_f22__0
   STA   __local_f22__1
   STA   __local_f22__2
   STA   __local_f22__3
   STA   __local_f22__4
   STA   __local_f22__5
   STA   __local_f22__6
   STA   __local_f22__l30
   LDA   #$80
   STA   __local_f22__l30+1
   LDA   #$00
   STA   __local_f22__l30+2
   STA   __local_f22__l30+3
   LDA   gs7+1
   LDX   gs7+2
   STX   __local_f22__l27
   ASL
   LDA   __local_f22__l27
   ROL
   JSR   f22@run2
   LDX   #$00
   LDA   #$03
   JSR   f9
   JSR   f17@run1
   LDY   #$00
   LDA   (__zpabi_f22__pp26_0),Y
   JSR   f22@run2
   LDX   #$00
   TXA
   JSR   f9
   JSR   f17
   STA   __zpabi_mix__v_0
   JSR   f22@run5
   LDA   z4
   JSR   f22@run2
   LDX   #$01
   LDA   #$00
   JSR   f9
   INC   gs7+1
   BNE   .inc_done@1
   INC   gs7+2
   BNE   .inc_done@1
   INC   gs7+3
   BNE   .inc_done@1
   INC   gs7+4
   BNE   .inc_done@1
   INC   gs7+5
   BNE   .inc_done@1
   INC   gs7+6
   BNE   .inc_done@1
   INC   gs7+7
.inc_done@1:
   LDX   #$00
   LDA   #$FF
   RTS

f22@run5:
   SUBROUTINE

   LDA   #$00
   STA   __zpabi_mix__v_1
   STA   __zpabi_mix__v_2
   JMP   fin0@run0

f22@run2:
   SUBROUTINE

   STA   __zpabi_f9__p11
   LDA   #<__local_f22__l30
   STA   __zpabi_f9__pp12_0
   LDA   #>__local_f22__l30
   STA   __zpabi_f9__pp12_1
   RTS

main:
   SUBROUTINE

   LDA   #$F1
   STA   gs7+8
   LDA   #$03
   STA   gs7+9
   LDA   #$00
   STA   gs7+10
   STA   gs7+11
   JSR   fin1@run8
   STA   __zpabi_mix__v_1
   LDA   __zpabi_mix__v_0
   CLC
   ADC   #$FF
   STA   __zpabi_mix__v_0
   BCC   .carry_inc@1
   INC   __zpabi_mix__v_1
.carry_inc@1:
   STA   ga6+2
   LDA   __zpabi_mix__v_1
   STA   ga6+3
   BMI   .sx_neg@33
   LDA   #$00
   BEQ   .sx_done@34
.sx_neg@33:
   LDA   #$FF
.sx_done@34:
   STA   __zpabi_mix__v_2
   JSR   fin0@run0
   LDA   #$0D
   STA   __zpabi_mix__v_0
   JSR   f22@run5
   LDA   #$E0
   STA   __zpabi_mix__v_0
   LDA   #$0F
   STA   __zpabi_mix__v_1
   LDA   #$00
   STA   __zpabi_mix__v_2
   JSR   fin0@run0
   LDA   gs7+1
   STA   __local_main__6
   LDA   gs7+2
   STA   __local_main__5
   LDA   gs7+3
   STA   __local_main__4
   LDA   gs7+4
   STA   __zpabi_mix__v_3
   LDA   gs7+5
   STA   __zpabi_mix__v_2
   LDA   gs7+6
   STA   __zpabi_mix__v_1
   LDA   gs7+7
   LSR
   LDA   __zpabi_mix__v_1
   ROR
   STA   gs7+7
   LDA   __zpabi_mix__v_2
   ROR
   STA   gs7+6
   LDA   __zpabi_mix__v_3
   ROR
   STA   gs7+5
   LDA   __local_main__4
   ROR
   STA   __zpabi_mix__v_3
   LDA   __local_main__5
   ROR
   STA   __zpabi_mix__v_2
   LDA   __local_main__6
   ROR
   STA   __zpabi_mix__v_1
   LDA   #$00
   ROR
   STA   __zpabi_mix__v_0
   STA   gs7+1
   LDA   __zpabi_mix__v_1
   STA   gs7+2
   LDA   __zpabi_mix__v_2
   STA   gs7+3
   LDA   __zpabi_mix__v_3
   STA   gs7+4
   JSR   mix
   LDY   #$00
   LDA   (gp8),Y
   CLC
   ADC   #$81
   STA   (gp8),Y
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
   DS.B  1

ga6:
   DS.B  2
   DC.W  $0080
   DC.W  $00FF
   DC.W  $0002

gs7:
   DS.B  1
   DC.B  $FF
   DS.B  6
   DC.L  $00000001

__zp_init:
   SUBROUTINE

   LDA   #$00
   STA   z3+2
   STA   z3+3
   STA   z5+1
   STA   z5+2
   STA   z5+3
   STA   @4.s15+1
   LDA   #$0F
   STA   g2
   LDA   #$FF
   STA   z3
   LDA   #$7F
   STA   z3+1
   LDA   #$C2
   STA   z4
   LDA   #$C6
   STA   z4+1
   LDA   #$01
   STA   z5
   LDA   #<g1
   STA   gp8
   LDA   #>g1
   STA   gp8+1
   LDA   #$03
   STA   @4.s15
   RTS
