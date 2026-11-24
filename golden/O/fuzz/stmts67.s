SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
z4	EQU	$26
z5	EQU	$2E
gp8	EQU	$30
@4.s17	EQU	$32
gp9	EQU	$34
g3	EQU	$36
g2	EQU	$38
g1	EQU	$40
__local_main__0	EQU	$80
__zpabi_f20__p22	EQU	$80
__zpabi_f26__p27_0	EQU	$80
__local_main__1	EQU	$81
__zpabi_f20__p23_0	EQU	$81
__zpabi_f26__p27_1	EQU	$81
__zpabi_f20__p23_1	EQU	$82
__zpabi_f26__p27_2	EQU	$82
__zpabi_f26__p27_3	EQU	$83
__zpabi_f26__p28_0	EQU	$84
__zpabi_f26__p28_1	EQU	$85
__zpabi_f26__p29_0	EQU	$86
__zpabi_f26__p29_1	EQU	$87
__zpabi_f10__p11_0	EQU	$88
__zpabi_f10__p11_1	EQU	$89
__zpabi_f10__p11_2	EQU	$8A
__zpabi_f10__p11_3	EQU	$8B
__zpabi_f10__p12_0	EQU	$8C
__zpabi_f10__p12_1	EQU	$8D
__zpabi_f10__p13_0	EQU	$8E
__zpabi_f10__p13_1	EQU	$8F
__zpabi_mix__v_0	EQU	$90
__zpabi_mix__v_1	EQU	$91
__zpabi_mix__v_2	EQU	$92
__zpabi_mix__v_3	EQU	$93
__local_f20__0	EQU	$94
__local_f26__0	EQU	$94
__local_f20__1	EQU	$95
__local_f26__1	EQU	$95
__local_f20__p21	EQU	$96
__local_f26__2	EQU	$96
__local_f20__2	EQU	$97
__local_f26__3	EQU	$97
__local_f20__3	EQU	$98
__local_f26__w33	EQU	$98
__local_f20__4	EQU	$99
__local_f26__4	EQU	$99
__local_f26__5	EQU	$9A
__local_f26__6	EQU	$9B
__local_f10__0	EQU	$9E
__local_f10__1	EQU	$9F
__local_f10__2	EQU	$A0
__local_f10__3	EQU	$A1
__local_f10__4	EQU	$A2
__local_f10__5	EQU	$A3
__local_f10__9	EQU	$A7
__local_f10__w19	EQU	$A8
__local_f10__w18	EQU	$A9
__local_f10__l14	EQU	$AA
__local_f10__10	EQU	$AB
__local_mix__0	EQU	$AC
__local_mix__1	EQU	$AD
__local_mix__2	EQU	$AE
__local_mix__3	EQU	$AF

; @zp-link-meta-begin
; def f10 params=__zpabi_f10__p11_0,__zpabi_f10__p11_1,__zpabi_f10__p11_2,__zpabi_f10__p11_3,__zpabi_f10__p12_0,__zpabi_f10__p12_1,__zpabi_f10__p13_0,__zpabi_f10__p13_1 locals=__local_f10__0,__local_f10__1,__local_f10__2,__local_f10__3,__local_f10__4,__local_f10__5,__local_f10__6,__local_f10__7,__local_f10__8,__local_f10__9,__local_f10__w19,__local_f10__w18,__local_f10__l14,__local_f10__10 indirect=false in_cycle=false
; def f20 params=__zpabi_f20__p21_0,__zpabi_f20__p21_1,__zpabi_f20__p22,__zpabi_f20__p23_0,__zpabi_f20__p23_1 locals=__local_f20__0,__local_f20__1,__local_f20__p21,__local_f20__2,__local_f20__3,__local_f20__4,__local_f20__5,__local_f20__6,__local_f20__7,__local_f20__8 indirect=false in_cycle=false param_regs=A,X,,,
; def f26 params=__zpabi_f26__p27_0,__zpabi_f26__p27_1,__zpabi_f26__p27_2,__zpabi_f26__p27_3,__zpabi_f26__p28_0,__zpabi_f26__p28_1,__zpabi_f26__p29_0,__zpabi_f26__p29_1 locals=__local_f26__0,__local_f26__1,__local_f26__2,__local_f26__3,__local_f26__w33,__local_f26__4,__local_f26__5,__local_f26__6,__local_f26__7 indirect=false in_cycle=false
; def fin0 params= locals=__local_fin0__0,__local_fin0__1,__local_fin0__2,__local_fin0__3 indirect=false in_cycle=false
; def fin1 params= locals=__local_fin1__0,__local_fin1__1,__local_fin1__2,__local_fin1__3 indirect=false in_cycle=false
; def fin2 params= locals=__local_fin2__0,__local_fin2__1,__local_fin2__2,__local_fin2__3 indirect=false in_cycle=false
; def main params= locals=__local_main__0,__local_main__1,__local_main__2,__local_main__3,__local_main__4,__local_main__5 indirect=false in_cycle=false
; def mix params=__zpabi_mix__v_0,__zpabi_mix__v_1,__zpabi_mix__v_2,__zpabi_mix__v_3 locals=__local_mix__0,__local_mix__1,__local_mix__2,__local_mix__3 indirect=false in_cycle=false
; call f10 -> mix
; call f20 -> f10
; call f20 -> mix
; call f26 -> f10
; call f26 -> mix
; call fin0 -> mix
; call fin1 -> mix
; call fin2 -> mix
; call main -> f10
; call main -> fin0
; call main -> fin1
; call main -> fin2
; call main -> mix
; item mix section=.text local=false zero=false
; item fin0 section=.text local=false zero=false
; item fin0@run0 section=.text local=true zero=false
; item fin1 section=.text local=false zero=false
; item fin1@run2 section=.text local=true zero=false
; item fin2 section=.text local=false zero=false
; item f10 section=.text local=false zero=false
; item f10@run1 section=.text local=true zero=false
; item f20 section=.text local=false zero=false
; item f26 section=.text local=false zero=false
; item f26@run3 section=.text local=true zero=false
; item main section=.text local=false zero=false
; item acc section=.bss local=false zero=true
; item ga6 section=.data local=false zero=false
; item gs7 section=.data local=false zero=false
; item @13.s25 section=.data local=true zero=false
; item __zp_init section=.text local=false zero=false
; zp z4 size=8 at=auto def=true init=$7F,$00,$00,$00,$00,$00,$00,$00
; zp z5 size=2 at=auto def=true init=$00,$80
; zp gp8 size=2 at=auto def=true init=<z5,>z5 weight=273
; zp @4.s17 size=2 at=auto def=true init=$80,$00 weight=128 local=true
; zp gp9 size=2 at=auto def=true init=<g1,>g1 weight=89
; zp g3 size=2 at=auto def=true init=$12,$E1 weight=70
; zp g2 size=8 at=auto def=true init=$FF,$FF,$FF,$7F,$00,$00,$00,$00 weight=94
; zp g1 size=2 at=auto def=true init=$00,$00 weight=15
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
   LDA   g3+1
   BMI   .sx_neg@2
   LDA   #$00
   BEQ   .sx_done@3
.sx_neg@2:
   LDA   #$FF
.sx_done@3:
   STA   __zpabi_mix__v_2
   STA   __zpabi_mix__v_3
   LDA   g3
   STA   __zpabi_mix__v_0
   LDA   g3+1
   STA   __zpabi_mix__v_1
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

fin0@run0:
   SUBROUTINE

   STA   __zpabi_mix__v_3
   JMP   mix

fin1:
   SUBROUTINE

   LDA   z5
   STA   __zpabi_mix__v_0
   LDA   z5+1
   JSR   fin1@run2
   LDA   ga6
   STA   __zpabi_mix__v_0
   LDA   ga6+1
   STA   __zpabi_mix__v_1
   LDA   ga6+2
   STA   __zpabi_mix__v_2
   LDA   ga6+3
   JSR   fin0@run0
   LDA   ga6+4
   STA   __zpabi_mix__v_0
   LDA   ga6+5
   STA   __zpabi_mix__v_1
   LDA   ga6+6
   STA   __zpabi_mix__v_2
   LDA   ga6+7
   JSR   fin0@run0
   LDA   ga6+8
   STA   __zpabi_mix__v_0
   LDA   ga6+9
   STA   __zpabi_mix__v_1
   LDA   ga6+10
   STA   __zpabi_mix__v_2
   LDA   ga6+11
   STA   __zpabi_mix__v_3
   JMP   mix

fin1@run2:
   SUBROUTINE

   STA   __zpabi_mix__v_1
   LDA   #$00
   STA   __zpabi_mix__v_2
   JMP   fin0@run0

fin2:
   SUBROUTINE

   LDA   ga6+12
   STA   __zpabi_mix__v_0
   LDA   ga6+13
   STA   __zpabi_mix__v_1
   LDA   ga6+14
   STA   __zpabi_mix__v_2
   LDA   ga6+15
   JSR   fin0@run0
   LDA   gs7+1
   STA   __zpabi_mix__v_0
   LDA   gs7+2
   STA   __zpabi_mix__v_1
   LDA   gs7+3
   STA   __zpabi_mix__v_2
   LDA   gs7+4
   JSR   fin0@run0
   LDA   gs7+6
   BMI   .sx_neg@4
   LDA   #$00
   BEQ   .sx_done@5
.sx_neg@4:
   LDA   #$FF
.sx_done@5:
   STA   __zpabi_mix__v_2
   STA   __zpabi_mix__v_3
   LDA   gs7+5
   STA   __zpabi_mix__v_0
   LDA   gs7+6
   STA   __zpabi_mix__v_1
   JMP   mix

f10:
   SUBROUTINE

   LDA   #$80
   STA   __local_f10__l14
   LDA   #$00
   STA   __local_f10__10
   STA   __local_f10__w18
.loop@0_start:
   LDA   #$00
   STA   __local_f10__w19
.loop@1_start:
   LDA   #$02
   CLC
   SBC   __local_f10__w18
   LDA   #$00
   ROL
   STA   __local_f10__1
   LDY   #$00
   LDA   (gp8),Y
   CLC
   ADC   #$FF
   STA   __local_f10__0
   LDA   __local_f10__1
   SEC
   SBC   __local_f10__0
   AND   #$07
   BNE   .lb_skip@2
   JMP   .switch@2_break
.lb_skip@2:
   CMP   #$02
   BEQ   .case@4
   CMP   #$05
   BEQ   .case@5
   JMP   .default@6
.case@4:
   LDA   __zpabi_f10__p12_1
   BMI   .sx_neg@6
   LDA   #$00
   BEQ   .sx_done@7
.sx_neg@6:
   LDA   #$FF
.sx_done@7:
   STA   __local_f10__1
   STA   __local_f10__0
   LDA   g2
   CLC
   ADC   __zpabi_f10__p12_0
   STA   __local_f10__3
   LDA   g2+1
   ADC   __zpabi_f10__p12_1
   STA   __local_f10__2
   LDA   g2+2
   ADC   __local_f10__1
   STA   __local_f10__1
   LDA   g2+3
   ADC   __local_f10__0
   STA   __local_f10__0
   LDA   #$FE
   SEC
   SBC   __local_f10__3
   STA   __zpabi_mix__v_0
   LDA   #$00
   SBC   __local_f10__2
   STA   __zpabi_mix__v_1
   LDA   #$00
   SBC   __local_f10__1
   STA   __zpabi_mix__v_2
   LDA   #$00
   SBC   __local_f10__0
   JSR   fin0@run0
   LDA   #$FF
   STA   gs7+5
.case@5:
   LDA   __local_f10__w19
   STA   z4
   JSR   f10@run1
   LDY   #$00
   LDA   (gp9),Y
   CLC
   ADC   @4.s17
   RTS
.default@6:
   LDY   #$00
   LDA   (gp8),Y
   STA   HARGS
   INY
   LDA   (gp8),Y
   STA   HARGS+1
   LDA   #$5A
   STA   HARGS+2
   LDA   #$00
   STA   HARGS+3
   JSR   mul16
   LDA   HARGS+4
   STA   __local_f10__1
   LDY   #$00
   STA   (gp8),Y
   LDA   HARGS+5
   INY
   STA   (gp8),Y
   LDA   __zpabi_f10__p11_0
   ORA   __local_f10__1
   RTS
.switch@2_break:
   LDA   __local_f10__w18
   AND   #$03
   ASL
   ASL
   TAX
   LDA   __local_f10__w18
   STA   ga6,X
   LDA   #$00
   STA   ga6+1,X
   STA   ga6+2,X
   STA   ga6+3,X
   LDY   #$00
   LDA   (gp8),Y
   AND   #$01
   STA   __local_f10__9
   LDA   g3+1
   BMI   .sx_neg@8
   LDA   #$00
   BEQ   .sx_done@9
.sx_neg@8:
   LDA   #$FF
.sx_done@9:
   STA   __local_f10__5
   STA   __local_f10__4
   STA   __local_f10__3
   STA   __local_f10__2
   STA   __local_f10__1
   STA   __local_f10__0
   LDA   g3
   CLC
   SBC   z4
   LDA   g3+1
   SBC   z4+1
   LDA   __local_f10__5
   SBC   z4+2
   LDA   __local_f10__4
   SBC   z4+3
   LDA   __local_f10__3
   SBC   z4+4
   LDA   __local_f10__2
   SBC   z4+5
   LDA   __local_f10__1
   SBC   z4+6
   LDA   __local_f10__0
   SBC   z4+7
   LDA   #$00
   ROL
   STA   __local_f10__0
   LDA   #$00
   BNE   .cmp_true@10
   LDA   __local_f10__9
   CMP   __local_f10__0
   BNE   .cmp_true@10
   LDA   #$00
   BEQ   .cmp_end@11
.cmp_true@10:
   LDA   #$01
.cmp_end@11:
   STA   @4.s17
   LDA   #$00
   STA   @4.s17+1
   INC   __local_f10__w19
   LDA   __local_f10__w19
   CMP   #$02
   BCS   .lb_skip@1
   JMP   .loop@1_start
.lb_skip@1:
   LDA   __local_f10__l14
   STA   __local_f10__0
   LDA   __local_f10__10
   STA   __local_f10__1
   LDA   __local_f10__0
   BNE   .carry_inc@1
   DEC   __local_f10__1
.carry_inc@1:
   DEC   __local_f10__0
   LDA   __local_f10__0
   STA   __local_f10__l14
   LDA   __local_f10__1
   STA   __local_f10__10
   INC   __local_f10__w18
   LDA   __local_f10__w18
   CMP   #$03
   BCS   .lb_skip@0
   JMP   .loop@0_start
.lb_skip@0:
   LDA   __local_f10__l14
   STA   __local_f10__0
   LDA   __local_f10__10
   LSR   __local_f10__0
   LSR   __local_f10__0
   ASL
   ASL
   ASL
   ASL
   ASL
   ASL
   ORA   __local_f10__0
   RTS

f10@run1:
   SUBROUTINE

   LDA   #$00
   STA   z4+1
   STA   z4+2
   STA   z4+3
   STA   z4+4
   STA   z4+5
   STA   z4+6
   STA   z4+7
   RTS

f20:
   SUBROUTINE

   STA   __local_f20__p21
   LDA   g1
   STA   __local_f20__1
   SEC
   SBC   #$01
   STA   g1
   BCS   .carry_inc@2
   DEC   g1+1
.carry_inc@2:
   LDA   __local_f20__1
   AND   #$07
   BEQ   .case@8
   CMP   #$04
   BEQ   .case@9
   CMP   #$05
   BEQ   .case@10
   CMP   #$07
   BEQ   .case@11
   JMP   .default@12
.case@8:
   LDY   #$00
   LDA   (gp8),Y
   STA   __local_f20__1
   INY
   LDA   (gp8),Y
   LSR
   ROR   __local_f20__1
   LDA   #$00
   ROR
   DEY
   STA   (gp8),Y
   LDA   __local_f20__1
   INY
   STA   (gp8),Y
   LDA   #$FF
   STA   z4
   JSR   f10@run1
   JMP   .switch@7_break
.case@9:
.case@10:
   LDA   __local_f20__p21
   SEC
   SBC   gs7+5
   LDY   #$00
   STA   (gp8),Y
   LDA   #$00
   INY
   STA   (gp8),Y
   JMP   .switch@7_break
.case@11:
   LDA   #$FF
   STA   g1
   LDA   #$00
   STA   g1+1
   LDA   g3+1
   BMI   .sx_neg@13
   LDA   #$00
   BEQ   .sx_done@14
.sx_neg@13:
   LDA   #$FF
.sx_done@14:
   STA   __local_f20__4
   STA   __local_f20__3
   LDA   g3
   CLC
   ADC   ga6+8
   STA   __zpabi_f10__p11_0
   LDA   g3+1
   ADC   ga6+9
   STA   __zpabi_f10__p11_1
   LDA   __local_f20__4
   ADC   ga6+10
   STA   __zpabi_f10__p11_2
   LDA   __local_f20__3
   ADC   ga6+11
   STA   __zpabi_f10__p11_3
   LDA   gs7+5
   AND   ga6+12
   STA   __zpabi_f10__p13_0
   LDA   gs7+6
   AND   ga6+13
   STA   __zpabi_f10__p13_1
   LDA   z5
   STA   __zpabi_f10__p12_0
   LDA   z5+1
   STA   __zpabi_f10__p12_1
   JSR   f10
   JMP   .switch@7_break
.default@12:
   LDA   @13.s25
   CLC
   ADC   #$81
   STA   @13.s25
   BCC   .carry_inc@3
   INC   @13.s25+1
   BNE   .carry_inc@3
   INC   @13.s25+2
   BNE   .carry_inc@3
   INC   @13.s25+3
.carry_inc@3:
   LDX   @13.s25+1
   STX   __local_f20__p21
   LDX   @13.s25+2
   STX   __local_f20__1
   LDX   @13.s25+3
   STX   __local_f20__0
   STA   __zpabi_mix__v_0
   LDA   __local_f20__p21
   STA   __zpabi_mix__v_1
   LDA   __local_f20__1
   STA   __zpabi_mix__v_2
   LDA   __local_f20__0
   JSR   fin0@run0
   LDY   #$00
   LDA   (gp8),Y
   STA   __local_f20__0
   INY
   LDA   (gp8),Y
   STA   __local_f20__1
   LDA   __local_f20__0
   BNE   .carry_inc@4
   DEC   __local_f20__1
.carry_inc@4:
   DEC   __local_f20__0
   LDA   __local_f20__0
   LDY   #$00
   STA   (gp8),Y
   LDA   __local_f20__1
   INY
   STA   (gp8),Y
.switch@7_break:
   LDY   #$00
   LDA   (gp8),Y
   ORA   #$05
   STA   (gp8),Y
   LDA   g3
   ASL
   STA   __local_f20__p21
   LDA   g3+1
   ROL
   STA   __local_f20__2
   LDA   @13.s25
   STA   HARGS
   LDA   @13.s25+1
   STA   HARGS+1
   LDA   #$00
   STA   HARGS+2
   STA   HARGS+3
   JSR   mul16
   LDA   HARGS+4
   SEC
   SBC   __local_f20__p21
   STA   g3
   LDA   HARGS+5
   SBC   __local_f20__2
   STA   g3+1
   LDA   gs7+2
   LSR
   LDA   gs7+1
   ROR
   STA   __local_f20__2
   LDA   #$00
   ROR
   STA   __local_f20__p21
   LDA   __zpabi_f20__p22
   BMI   .sx_neg@15
   LDA   #$00
   BEQ   .sx_done@16
.sx_neg@15:
   LDA   #$FF
.sx_done@16:
   STA   __local_f20__1
   ASL
   ROR   __local_f20__1
   LDA   __zpabi_f20__p22
   ROR
   LDX   g2
   STX   __zpabi_f10__p11_0
   LDX   #$00
   STX   __zpabi_f10__p11_1
   STX   __zpabi_f10__p11_2
   STX   __zpabi_f10__p11_3
   LDX   __local_f20__p21
   STX   __zpabi_f10__p12_0
   LDX   __local_f20__2
   STX   __zpabi_f10__p12_1
   STA   __zpabi_f10__p13_0
   LDA   __local_f20__1
   STA   __zpabi_f10__p13_1
   JSR   f10
   LDY   #$00
   LDA   (gp8),Y
   STA   __local_f20__0
   INY
   LDA   (gp8),Y
   STA   __local_f20__1
   INC   __local_f20__0
   BNE   .inc_done@0
   INC   __local_f20__1
.inc_done@0:
   LDA   __local_f20__0
   LDY   #$00
   STA   (gp8),Y
   LDA   __local_f20__1
   INY
   STA   (gp8),Y
   LDA   ga6+12
   RTS

f26:
   SUBROUTINE

   LDA   __zpabi_f26__p29_0
   CLC
   ADC   #$FF
   AND   g1
   AND   #$07
   CMP   #$03
   BEQ   .case@14
   CMP   #$04
   BEQ   .case@15
   CMP   #$06
   BEQ   .case@16
   CMP   #$07
   BNE   .lb_skip@4
   JMP   .case@18
.lb_skip@4:
   JMP   .switch@13_break
.case@14:
.case@15:
   LDA   g2
   SEC
   SBC   #$01
   STA   __local_f26__6
   LDA   g2+1
   SBC   #$00
   STA   __local_f26__5
   LDA   g2+2
   SBC   #$00
   STA   __local_f26__4
   LDA   g2+3
   SBC   #$00
   STA   __local_f26__w33
   LDA   g2+4
   SBC   #$00
   STA   __local_f26__3
   LDA   g2+5
   SBC   #$00
   STA   __local_f26__2
   LDA   g2+6
   SBC   #$00
   STA   __local_f26__1
   LDA   g2+7
   SBC   #$00
   LDX   __local_f26__6
   STX   g2
   STX   g3
   LDX   __local_f26__4
   STX   g2+2
   LDX   __local_f26__w33
   STX   g2+3
   LDX   __local_f26__3
   STX   g2+4
   LDX   __local_f26__2
   STX   g2+5
   LDX   __local_f26__1
   STX   g2+6
   STA   g2+7
   LDA   __local_f26__5
   STA   g2+1
   STA   g3+1
.case@16:
   LDA   __zpabi_f26__p28_0
   AND   #$5A
   BNE   .if_end@0
   LDA   g2
   ORA   z4
   STA   __zpabi_f10__p11_0
   LDA   g2+1
   ORA   z4+1
   STA   __zpabi_f10__p11_1
   LDA   g2+2
   ORA   z4+2
   STA   __zpabi_f10__p11_2
   LDA   g2+3
   ORA   z4+3
   STA   __zpabi_f10__p11_3
   LDA   gs7+1
   SEC
   SBC   #$FF
   STA   __zpabi_f10__p13_0
   LDA   gs7+2
   SBC   #$FF
   STA   __zpabi_f10__p13_1
   LDA   #$01
   STA   __zpabi_f10__p12_0
   JSR   f26@run3
.if_end@0:
.case@18:
.switch@13_break:
   LDA   #$00
   STA   __local_f26__w33
.loop@19_start:
   LDA   g2
   CLC
   ADC   #$05
   STA   g2
   BCC   .carry_inc@5
   INC   g2+1
   BNE   .carry_inc@5
   INC   g2+2
   BNE   .carry_inc@5
   INC   g2+3
   BNE   .carry_inc@5
   INC   g2+4
   BNE   .carry_inc@5
   INC   g2+5
   BNE   .carry_inc@5
   INC   g2+6
   BNE   .carry_inc@5
   INC   g2+7
.carry_inc@5:
   LDY   #$01
   LDA   (gp9),Y
   STA   __local_f26__1
   DEY
   LDA   (gp9),Y
   SEC
   SBC   #$FF
   STA   __local_f26__0
   BCS   .carry_inc@6
   DEC   __local_f26__1
.carry_inc@6:
   LDY   #$00
   STA   (gp9),Y
   LDA   __local_f26__1
   INY
   STA   (gp9),Y
   STA   g2+1
   LDA   __local_f26__0
   STA   g2
   LDA   __local_f26__1
   BMI   .sx_neg@17
   LDA   #$00
   BEQ   .sx_done@18
.sx_neg@17:
   LDA   #$FF
.sx_done@18:
   STA   g2+2
   STA   g2+3
   STA   g2+4
   STA   g2+5
   STA   g2+6
   STA   g2+7
   INC   z4
   BNE   .inc_done@1
   INC   z4+1
   BNE   .inc_done@1
   INC   z4+2
   BNE   .inc_done@1
   INC   z4+3
   BNE   .inc_done@1
   INC   z4+4
   BNE   .inc_done@1
   INC   z4+5
   BNE   .inc_done@1
   INC   z4+6
   BNE   .inc_done@1
   INC   z4+7
.inc_done@1:
   LDY   #$00
   LDA   (gp8),Y
   AND   g1
   STA   __zpabi_mix__v_0
   INY
   LDA   (gp8),Y
   AND   g1+1
   JSR   fin1@run2
   INC   __local_f26__w33
   LDA   __local_f26__w33
   CMP   #$03
   BCS   .lb_skip@3
   JMP   .loop@19_start
.lb_skip@3:
   LDA   gs7+6
   CMP   g3+1
   BNE   .cmp_true@19
   LDA   gs7+5
   CMP   g3
   BNE   .cmp_true@19
   LDA   #$00
   BEQ   .cmp_end@20
.cmp_true@19:
   LDA   #$01
.cmp_end@20:
   STA   __local_f26__0
   LDA   #$00
   BPL   .sx_done@23
   LDA   #$FF
.sx_done@23:
   STA   HARGS+18
   STA   HARGS+19
   STA   HARGS+20
   STA   HARGS+21
   STA   HARGS+22
   STA   HARGS+23
   LDA   __local_f26__0
   STA   HARGS+16
   LDA   #$00
   STA   HARGS+17
   RTS

f26@run3:
   SUBROUTINE

   LDA   #$00
   STA   __zpabi_f10__p12_1
   JMP   f10

main:
   SUBROUTINE

   LDY   #$00
   LDA   (gp9),Y
   STA   __local_main__1
   INY
   LDA   (gp9),Y
   STA   __local_main__0
   LDA   #$00
   EOR   __local_main__1
   STA   __zpabi_f10__p13_0
   LDA   #$00
   EOR   __local_main__0
   STA   __zpabi_f10__p13_1
   LDA   g2
   STA   __zpabi_f10__p11_0
   LDA   g2+1
   STA   __zpabi_f10__p11_1
   LDA   g2+2
   STA   __zpabi_f10__p11_2
   LDA   g2+3
   STA   __zpabi_f10__p11_3
   LDA   #$02
   STA   __zpabi_f10__p12_0
   JSR   f26@run3
   STA   __local_main__0
   ORA   #$00
   BMI   .sx_neg@24
   LDA   #$00
   BEQ   .sx_done@25
.sx_neg@24:
   LDA   #$FF
.sx_done@25:
   STA   __zpabi_mix__v_1
   STA   __zpabi_mix__v_2
   STA   __zpabi_mix__v_3
   LDA   __local_main__0
   STA   __zpabi_mix__v_0
   JSR   mix
   LDA   #$FF
   STA   __zpabi_f10__p11_0
   STA   __zpabi_f10__p11_1
   STA   __zpabi_f10__p11_2
   STA   __zpabi_f10__p11_3
   LDA   g1
   STA   __zpabi_f10__p12_0
   LDA   g1+1
   STA   __zpabi_f10__p12_1
   LDA   #$00
   STA   __zpabi_f10__p13_0
   STA   __zpabi_f10__p13_1
   JSR   f10
   LDA   #$00
   STA   z5
   STA   z5+1
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
   DC.L  $80000000
   DC.L  $7FFFFFFF
   DS.B  4
   DC.L  $000000FF

gs7:
   DS.B  1
   DC.L  $0000007F
   DC.W  $B7B0

@13.s25:
   DC.L  $00008000

__zp_init:
   SUBROUTINE

   LDA   #$00
   STA   g1
   STA   g1+1
   STA   g2+4
   STA   g2+5
   STA   g2+6
   STA   g2+7
   STA   z4+1
   STA   z4+2
   STA   z4+3
   STA   z4+4
   STA   z4+5
   STA   z4+6
   STA   z4+7
   STA   z5
   STA   @4.s17+1
   LDA   #$FF
   STA   g2
   STA   g2+1
   STA   g2+2
   LDA   #$7F
   STA   g2+3
   STA   z4
   LDA   #$12
   STA   g3
   LDA   #$E1
   STA   g3+1
   LDA   #$80
   STA   z5+1
   STA   @4.s17
   LDA   #<z5
   STA   gp8
   LDA   #>z5
   STA   gp8+1
   LDA   #<g1
   STA   gp9
   LDA   #>g1
   STA   gp9+1
   RTS
