SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
z4	EQU	$26
z5	EQU	$2A
g1	EQU	$2C
g3	EQU	$2D
gp8	EQU	$2F
gp9	EQU	$31
g2	EQU	$33
__zpabi_f10__p11_0	EQU	$80
__zpabi_f10__p11_1	EQU	$81
__zpabi_f10__p11_2	EQU	$82
__zpabi_f10__p11_3	EQU	$83
__zpabi_f10__pp12_0	EQU	$84
__zpabi_f10__pp12_1	EQU	$85
__zpabi_mix__v_0	EQU	$86
__zpabi_mix__v_1	EQU	$87
__zpabi_mix__v_2	EQU	$88
__zpabi_mix__v_3	EQU	$89
__local_main__i27	EQU	$8A
__local_f20__1	EQU	$8B
__local_main__0	EQU	$8B
__local_f20__l22_0	EQU	$8C
__local_main__1	EQU	$8C
__local_f20__l22_1	EQU	$8D
__local_main__2	EQU	$8D
__local_main__3	EQU	$8E
__local_f20__3	EQU	$8F
__local_main__4	EQU	$8F
__local_f20__i24	EQU	$90
__local_main__5	EQU	$90
__local_f20__4	EQU	$91
__local_main__6	EQU	$91
__local_f20__5	EQU	$92
__local_main__i28	EQU	$92
__local_main__l26	EQU	$93
__local_f10__0	EQU	$95
__local_f10__1	EQU	$96
__local_f16__0	EQU	$96
__local_f16__1	EQU	$97
__local_f16__2	EQU	$98
__local_f16__3	EQU	$99
__local_f16__4	EQU	$9A
__local_f16__7	EQU	$9D
__local_f16__8	EQU	$9E
__local_f16__9	EQU	$9F
__local_f16__10	EQU	$A0
__local_f16__11	EQU	$A1
__local_f16__12	EQU	$A2
__local_f16__13	EQU	$A3
__local_f16__14	EQU	$A4
__local_mix__0	EQU	$A8
__local_mix__1	EQU	$A9
__local_mix__2	EQU	$AA
__local_mix__3	EQU	$AB

; @zp-link-meta-begin
; def f10 params=__zpabi_f10__p11_0,__zpabi_f10__p11_1,__zpabi_f10__p11_2,__zpabi_f10__p11_3,__zpabi_f10__pp12_0,__zpabi_f10__pp12_1 locals=__local_f10__0,__local_f10__1,__local_f10__2,__local_f10__3 indirect=false in_cycle=false
; def f16 params= locals=__local_f16__w19,__local_f16__0,__local_f16__1,__local_f16__2,__local_f16__3,__local_f16__4,__local_f16__5,__local_f16__6,__local_f16__7,__local_f16__8,__local_f16__9,__local_f16__10,__local_f16__11,__local_f16__12,__local_f16__13,__local_f16__14,__local_f16__15,__local_f16__16,__local_f16__17 indirect=false in_cycle=false
; def f20 params= locals=__local_f20__0,__local_f20__1,__local_f20__l22_0,__local_f20__l22_1,__local_f20__2,__local_f20__3,__local_f20__i24,__local_f20__4,__local_f20__5,__local_f20__6,__local_f20__7 indirect=false in_cycle=false
; def fin0 params= locals=__local_fin0__0,__local_fin0__1 indirect=false in_cycle=false
; def fin1 params= locals=__local_fin1__0,__local_fin1__1 indirect=false in_cycle=false
; def fin2 params= locals=__local_fin2__0,__local_fin2__1,__local_fin2__2,__local_fin2__3 indirect=false in_cycle=false
; def main params= locals=__local_main__i27,__local_main__0,__local_main__1,__local_main__2,__local_main__3,__local_main__4,__local_main__5,__local_main__6,__local_main__i28,__local_main__l26,__local_main__8 indirect=false in_cycle=false
; def mix params=__zpabi_mix__v_0,__zpabi_mix__v_1,__zpabi_mix__v_2,__zpabi_mix__v_3 locals=__local_mix__0,__local_mix__1,__local_mix__2,__local_mix__3 indirect=false in_cycle=false
; call f10 -> mix
; call f16 -> mix
; call f20 -> f16
; call f20 -> mix
; call fin0 -> mix
; call fin1 -> mix
; call fin2 -> mix
; call main -> f10
; call main -> f16
; call main -> fin0
; call main -> fin1
; call main -> fin2
; call main -> mix
; item mix section=.text local=false zero=false
; item fin0 section=.text local=false zero=false
; item fin0@run6 section=.text local=true zero=false
; item fin0@run3 section=.text local=true zero=false
; item fin0@run2 section=.text local=true zero=false
; item fin1 section=.text local=false zero=false
; item fin1@run0 section=.text local=true zero=false
; item fin2 section=.text local=false zero=false
; item f10 section=.text local=false zero=false
; item f10@run5 section=.text local=true zero=false
; item f10@run4 section=.text local=true zero=false
; item f10@run1 section=.text local=true zero=false
; item f16 section=.text local=false zero=false
; item f20 section=.text local=false zero=false
; item main section=.text local=false zero=false
; item acc section=.bss local=false zero=true
; item ga6 section=.data local=false zero=false
; item gs7 section=.data local=false zero=false
; item __zp_init section=.text local=false zero=false
; zp z4 size=4 at=auto def=true init=$0C,$00,$00,$00
; zp z5 size=2 at=auto def=true init=$00,$80
; zp g1 size=1 at=auto def=true init=$00 weight=23
; zp g3 size=2 at=auto def=true init=$02,$00 weight=32
; zp gp8 size=2 at=auto def=true init=<g3,>g3 weight=21
; zp gp9 size=2 at=auto def=true init=<z5,>z5 weight=11
; zp g2 size=8 at=auto def=true init=$00,$00,$00,$80,$00,$00,$00,$00 weight=35
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
   BMI   .sx_neg@0
   LDA   #$00
   BEQ   .sx_done@1
.sx_neg@0:
   LDA   #$FF
.sx_done@1:
   JSR   fin0@run2
   LDA   g1
   JSR   fin0@run3
   LDA   g2
   STA   __zpabi_mix__v_0
   LDA   g2+1
   STA   __zpabi_mix__v_1
   LDA   g2+2
   STA   __zpabi_mix__v_2
   LDA   g2+3
   STA   __zpabi_mix__v_3
   LDA   g2+4
   LDA   g2+5
   LDA   g2+6
   LDA   g2+7
   JSR   mix
   LDA   g3+1
   BMI   .sx_neg@2
   LDA   #$00
   BEQ   .sx_done@3
.sx_neg@2:
   LDA   #$FF
.sx_done@3:
   JSR   fin0@run6
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

fin0@run6:
   SUBROUTINE

   STA   __zpabi_mix__v_2
   STA   __zpabi_mix__v_3
   RTS

fin0@run3:
   SUBROUTINE

   STA   __zpabi_mix__v_0
   JMP   mix

fin0@run2:
   SUBROUTINE

   STA   __zpabi_mix__v_1
   JSR   fin0@run6
   RTS

fin1:
   SUBROUTINE

   LDA   z5
   STA   __zpabi_mix__v_0
   LDA   z5+1
   JSR   fin1@run0
   LDA   ga6
   BMI   .sx_neg@4
   LDA   #$00
   BEQ   .sx_done@5
.sx_neg@4:
   LDA   #$FF
.sx_done@5:
   JSR   fin0@run2
   LDA   ga6
   JSR   fin0@run3
   LDA   ga6+1
   BMI   .sx_neg@6
   LDA   #$00
   BEQ   .sx_done@7
.sx_neg@6:
   LDA   #$FF
.sx_done@7:
   JSR   fin0@run2
   LDA   ga6+1
   JSR   fin0@run3
   LDA   ga6+2
   BMI   .sx_neg@8
   LDA   #$00
   BEQ   .sx_done@9
.sx_neg@8:
   LDA   #$FF
.sx_done@9:
   JSR   fin0@run2
   LDA   ga6+2
   STA   __zpabi_mix__v_0
   JMP   mix

fin1@run0:
   SUBROUTINE

   STA   __zpabi_mix__v_1
   LDA   #$00
   STA   __zpabi_mix__v_2
   STA   __zpabi_mix__v_3
   JMP   mix

fin2:
   SUBROUTINE

   LDA   ga6+3
   BMI   .sx_neg@10
   LDA   #$00
   BEQ   .sx_done@11
.sx_neg@10:
   LDA   #$FF
.sx_done@11:
   JSR   fin0@run2
   LDA   ga6+3
   JSR   fin0@run3
   LDA   gs7+2
   BMI   .sx_neg@12
   LDA   #$00
   BEQ   .sx_done@13
.sx_neg@12:
   LDA   #$FF
.sx_done@13:
   JSR   fin0@run6
   LDA   gs7+1
   STA   __zpabi_mix__v_0
   LDA   gs7+2
   STA   __zpabi_mix__v_1
   JSR   mix
   LDA   gs7+3
   STA   __zpabi_mix__v_0
   LDA   gs7+4
   STA   __zpabi_mix__v_1
   LDA   gs7+5
   STA   __zpabi_mix__v_2
   LDA   gs7+6
   STA   __zpabi_mix__v_3
   JMP   mix

f10:
   SUBROUTINE

   JSR   f10@run1
   LDA   #$5A
   JSR   f10@run5
   JSR   mul32
   LDA   HARGS+8
   STA   gs7+3
   LDA   HARGS+10
   STA   gs7+5
   LDA   HARGS+11
   STA   gs7+6
   LDA   HARGS+9
   STA   gs7+4
   LDA   HARGS+8
   AND   #$07
   CMP   #$04
   BEQ   .case@1
   CMP   #$06
   BNE   .lb_skip@0
   JMP   .case@2
.lb_skip@0:
   JMP   .switch@0_break
.case@1:
   LDA   g3
   EOR   #$81
   STA   g3
   LDA   gs7+1
   STA   HARGS
   LDA   gs7+2
   STA   HARGS+1
   LDA   #$07
   STA   HARGS+2
   LDA   #$00
   STA   HARGS+3
   JSR   sdivmod16
   LDA   g3
   SEC
   SBC   HARGS+6
   STA   __local_f10__1
   LDA   g3+1
   SBC   HARGS+7
   ORA   __local_f10__1
   BEQ   .if_else@1
   LDY   #$00
   LDA   (gp8),Y
   STA   __local_f10__1
   INY
   LDA   (gp8),Y
   STA   __local_f10__0
   LDA   #$7F
   EOR   __local_f10__1
   STA   __zpabi_mix__v_0
   LDA   #$00
   EOR   __local_f10__0
   JSR   fin1@run0
   LDA   #$FE
   LDY   #$00
   STA   (__zpabi_f10__pp12_0),Y
   LDA   #$FF
   INY
   STA   (__zpabi_f10__pp12_0),Y
   JMP   .switch@0_break
.if_else@1:
   LDA   g3
   SEC
   SBC   #$05
   STA   g3
   BCS   .carry_inc@0
   DEC   g3+1
.carry_inc@0:
   LDA   g1
   ASL
   CLC
   ADC   g1
   STA   g1
   BEQ   .if_end@4
   LDA   #$7F
   STA   __zpabi_mix__v_0
   JSR   f10@run4
.if_end@4:
.case@2:
.switch@0_break:
   JSR   f10@run1
   LDA   #$04
   JSR   f10@run5
   JSR   sdivmod32
   LDX   HARGS+13
   LDA   HARGS+12
   RTS

f10@run5:
   SUBROUTINE

   STA   HARGS+4
   LDA   #$00
   STA   HARGS+5
   LDY   #$00
   STY   HARGS+6
   STA   HARGS+7
   RTS

f10@run4:
   SUBROUTINE

   LDA   #$00
   STA   __zpabi_mix__v_1
   JSR   fin0@run6
   JMP   mix

f10@run1:
   SUBROUTINE

   LDA   gs7+3
   STA   HARGS
   LDA   gs7+4
   STA   HARGS+1
   LDA   gs7+5
   STA   HARGS+2
   LDA   gs7+6
   STA   HARGS+3
   RTS

f16:
   SUBROUTINE

   LDA   #$00
   STA   __zpabi_mix__v_0
.loop@3_continue:
   CLC
   ADC   #$01
   STA   __local_f16__3
   LDA   __zpabi_mix__v_0
   BEQ   .lb_skip@2
   JMP   .loop@3_break
.lb_skip@2:
   INC   g3
   BNE   .inc_done@0
   INC   g3+1
.inc_done@0:
   LDA   __local_f16__3
   CLC
   SBC   __local_f16__3
   LDA   #$00
   ROL
   AND   #$07
   CMP   #$03
   BEQ   .switch@4_break
   CMP   #$04
   BEQ   .case@6
   CMP   #$05
   BEQ   .case@7
   BNE   .switch@4_break
.case@6:
   LDA   __local_f16__3
   AND   #$03
   CLC
   TAX
   LDA   ga6,X
   ADC   #$01
   STA   __local_f16__2
   STA   ga6,X
   BMI   .sx_neg@14
   LDA   #$00
   BEQ   .sx_done@15
.sx_neg@14:
   LDA   #$FF
.sx_done@15:
   STA   __local_f16__1
   LDY   #$00
   LDA   (gp9),Y
   STA   HARGS+2
   INY
   LDA   (gp9),Y
   STA   HARGS+3
   LDA   __local_f16__3
   STA   HARGS
   LDA   #$00
   STA   HARGS+1
   JSR   mul16
   LDA   __local_f16__2
   ORA   HARGS+4
   STA   __zpabi_mix__v_0
   LDA   __local_f16__1
   ORA   HARGS+5
   JSR   fin1@run0
.case@7:
.switch@4_break:
   LDA   __local_f16__3
   AND   #$03
   TAX
   LDA   ga6,X
   STA   __local_f16__0
   BMI   .sx_neg@16
   LDA   #$00
   BEQ   .sx_done@17
.sx_neg@16:
   LDA   #$FF
.sx_done@17:
   STA   __zpabi_mix__v_0
   LDA   __local_f16__0
   ORA   __local_f16__3
   STA   __local_f16__2
   LDA   __zpabi_mix__v_0
   STA   __local_f16__1
   LDA   #$7F
   AND   z5+1
   STA   __zpabi_mix__v_0
   LDA   __local_f16__2
   SEC
   SBC   z5
   STA   __local_f16__0
   LDA   __local_f16__1
   SBC   __zpabi_mix__v_0
   ORA   __local_f16__0
   BEQ   .if_end@8
   LDY   #$00
   LDA   (gp8),Y
   STA   __zpabi_mix__v_0
   INY
   LDA   (gp8),Y
   STA   __local_f16__0
   LDA   __zpabi_mix__v_0
   BNE   .carry_inc@1
   DEC   __local_f16__0
.carry_inc@1:
   DEC   __zpabi_mix__v_0
   LDA   __zpabi_mix__v_0
   LDY   #$00
   STA   (gp8),Y
   LDA   __local_f16__0
   INY
   STA   (gp8),Y
   LDA   __zpabi_mix__v_0
   RTS
.if_end@8:
   LDA   __local_f16__3
   STA   __zpabi_mix__v_0
   JMP   .loop@3_continue
.loop@3_break:
   LDA   #$FF
   SEC
   SBC   #$FF
   LDA   #$FF
   SBC   #$7F
   LDA   #$FF
   SBC   #$00
   LDA   #$7F
   SBC   #$00
   LDA   #$00
   ROL
   AND   #$07
   CMP   #$03
   BEQ   .case@9
   CMP   #$05
   BEQ   .case@10
   CMP   #$07
   BNE   .lb_skip@1
   JMP   .case@11
.lb_skip@1:
   JMP   .default@12
.case@9:
   LDA   gs7+1
   LDX   gs7+2
   STX   __zpabi_mix__v_0
   ASL
   LDA   __zpabi_mix__v_0
   ROL
   STA   __local_f16__0
   LDA   #$00
   ADC   #$FF
   EOR   #$FF
   STA   __zpabi_mix__v_0
   STA   gs7+2
   LDA   __local_f16__0
   STA   gs7+1
   LDY   #$00
   STA   (gp9),Y
   LDA   __zpabi_mix__v_0
   INY
   STA   (gp9),Y
   JMP   .switch@8_break
.case@10:
   LDA   g2
   STA   __local_f16__14
   LDA   g2+1
   STA   __local_f16__13
   LDA   g2+2
   STA   __local_f16__12
   LDA   g2+3
   STA   __local_f16__11
   LDA   g2+4
   STA   __local_f16__10
   LDA   g2+5
   STA   __local_f16__9
   LDA   g2+6
   STA   __local_f16__8
   LDA   g2+7
   STA   __local_f16__7
   LDA   g3+1
   BMI   .sx_neg@18
   LDA   #$00
   BEQ   .sx_done@19
.sx_neg@18:
   LDA   #$FF
.sx_done@19:
   STA   __local_f16__4
   STA   __local_f16__3
   STA   __local_f16__2
   STA   __local_f16__1
   STA   __local_f16__0
   STA   __zpabi_mix__v_0
   LDA   __local_f16__7
   CMP   __zpabi_mix__v_0
   BNE   .thread@0
   LDA   __local_f16__8
   CMP   __local_f16__0
   BNE   .thread@0
   LDA   __local_f16__9
   CMP   __local_f16__1
   BNE   .thread@0
   LDA   __local_f16__10
   CMP   __local_f16__2
   BNE   .thread@0
   LDA   __local_f16__11
   CMP   __local_f16__3
   BNE   .thread@0
   LDA   __local_f16__12
   CMP   __local_f16__4
   BNE   .thread@0
   LDA   __local_f16__13
   CMP   g3+1
   BNE   .thread@0
   LDA   __local_f16__14
   CMP   g3
   BEQ   .cmp_true@20
.thread@0:
   LDA   #$00
   BEQ   .cmp_end@21
.cmp_true@20:
   LDA   #$01
.cmp_end@21:
   STA   __zpabi_mix__v_0
   LDA   #$00
   BPL   .sx_done@24
   LDA   #$FF
.sx_done@24:
   JSR   fin0@run6
   LDA   #$00
   STA   __zpabi_mix__v_1
   JSR   mix
.case@11:
   LDA   g1
   SEC
   SBC   #$01
   STA   __local_f16__2
   STA   g1
   BMI   .sx_neg@25
   LDA   #$00
   BEQ   .sx_done@26
.sx_neg@25:
   LDA   #$FF
.sx_done@26:
   JSR   fin0@run2
   LDA   __local_f16__2
   JSR   fin0@run3
.default@12:
.switch@8_break:
   LDY   #$00
   LDA   (gp9),Y
   AND   z5
   RTS

f20:
   SUBROUTINE

   JSR   f16
   STA   __local_f20__l22_0
   LDA   g1
   BMI   .sx_neg@27
   LDA   #$00
   BEQ   .sx_done@28
.sx_neg@27:
   LDA   #$FF
.sx_done@28:
   STA   __local_f20__5
   BMI   .sx_neg@29
   LDA   #$00
   BEQ   .sx_done@30
.sx_neg@29:
   LDA   #$FF
.sx_done@30:
   STA   __local_f20__4
   STA   __local_f20__i24
   STA   __local_f20__3
   STA   __zpabi_mix__v_2
   STA   __local_f20__1
   STA   __zpabi_mix__v_1
   LDA   g1
   CLC
   SBC   #$7F
   LDA   __local_f20__5
   SBC   #$00
   LDA   __local_f20__4
   SBC   #$00
   LDA   __local_f20__i24
   SBC   #$00
   LDA   __local_f20__3
   SBC   #$00
   LDA   __zpabi_mix__v_2
   SBC   #$00
   LDA   __local_f20__1
   SBC   #$00
   LDA   __zpabi_mix__v_1
   SBC   #$00
   LDA   #$00
   ROL
   AND   #$07
   CMP   #$02
   BEQ   .case@14
   CMP   #$05
   BEQ   .case@15
   LDA   #$00
   STA   __local_f20__l22_1
   JMP   .default@17
.case@14:
   LDY   #$00
   LDA   (gp8),Y
   STA   __local_f20__l22_0
   INY
   LDA   (gp8),Y
   STA   __zpabi_mix__v_1
   LDA   #$00
   SEC
   SBC   __local_f20__l22_0
   STA   __local_f20__1
   LDA   __local_f20__l22_0
   SBC   __zpabi_mix__v_1
   STA   __zpabi_mix__v_1
   LDA   __local_f20__1
   DEY
   STA   (gp8),Y
   LDA   __zpabi_mix__v_1
   INY
   STA   (gp8),Y
   LDA   g3
   AND   #$05
   DEY
   STY   g3+1
   STA   g3
   RTS
.case@15:
   LDA   #$00
   STA   __local_f20__l22_1
   STA   __local_f20__i24
.loop@16_start:
   LDA   __local_f20__i24
   CMP   #$03
   BCC   .lb_skip@3
   JMP   .loop@16_break
.lb_skip@3:
   LSR
   LSR
   LSR
   STA   __zpabi_mix__v_1
   LDA   __local_f20__i24
   ASL
   ASL
   ASL
   ASL
   ASL
   STA   gs7+3
   LDA   __zpabi_mix__v_1
   STA   gs7+4
   BMI   .sx_neg@31
   LDA   #$00
   BEQ   .sx_done@32
.sx_neg@31:
   LDA   #$FF
.sx_done@32:
   STA   gs7+5
   STA   gs7+6
   LDA   __local_f20__l22_0
   SEC
   SBC   #$81
   STA   __local_f20__l22_0
   BCS   .carry_inc@2
   DEC   __local_f20__l22_1
.carry_inc@2:
   LDA   __local_f20__l22_1
   BMI   .sx_neg@33
   LDA   #$00
   BEQ   .sx_done@34
.sx_neg@33:
   LDA   #$FF
.sx_done@34:
   LDX   __local_f20__l22_0
   STX   z4
   LDX   __local_f20__l22_1
   STX   z4+1
   STA   z4+2
   STA   z4+3
   LDA   __local_f20__i24
   STA   z4
   LDA   #$00
   STA   z4+1
   STA   z4+2
   STA   z4+3
   LDA   z4
   STA   __zpabi_mix__v_1
   LDA   z4+1
   STA   __local_f20__3
   LDA   z4+2
   STA   __zpabi_mix__v_2
   LDA   z4+3
   STA   __local_f20__1
   LDA   __zpabi_mix__v_1
   SEC
   SBC   #$05
   STA   __zpabi_mix__v_1
   LDA   __local_f20__3
   SBC   #$00
   STA   __local_f20__3
   LDA   __zpabi_mix__v_2
   SBC   #$00
   STA   __zpabi_mix__v_2
   BCS   .carry_inc@3
   DEC   __local_f20__1
.carry_inc@3:
   LDA   __zpabi_mix__v_1
   STA   z4
   LDA   __local_f20__3
   STA   z4+1
   LDA   __zpabi_mix__v_2
   STA   z4+2
   LDA   __local_f20__1
   STA   z4+3
   LDA   __zpabi_mix__v_1
   STA   __zpabi_mix__v_0
   LDA   __local_f20__3
   STA   __zpabi_mix__v_1
   LDA   __local_f20__1
   STA   __zpabi_mix__v_3
   JSR   mix
   INC   __local_f20__i24
   JMP   .loop@16_start
.loop@16_break:
.default@17:
   LDA   gs7+3
   LDX   gs7+4
   STX   __zpabi_mix__v_2
   LDX   gs7+5
   STX   __local_f20__1
   LDX   gs7+6
   STX   __zpabi_mix__v_1
   SEC
   SBC   #$FF
   LDA   __zpabi_mix__v_2
   SBC   #$7F
   LDA   __local_f20__1
   SBC   #$00
   LDA   __zpabi_mix__v_1
   SBC   #$00
   BVC   .cmp_novf@35
   EOR   #$80
.cmp_novf@35:
   ASL
   LDA   #$00
   ROL
   EOR   #$01
   STA   __zpabi_mix__v_2
   LDA   #$00
   BPL   .sx_done@37
   LDA   #$FF
.sx_done@37:
   LDX   __zpabi_mix__v_2
   STX   z4
   LDX   #$00
   STX   z4+1
   STA   z4+2
   STA   z4+3
   LDA   __local_f20__l22_1
   ASL
   ROR   __local_f20__l22_1
   ROR   __local_f20__l22_0
   LDA   ga6+2
   CMP   #$80
   ROR
   CMP   #$80
   ROR
   STA   __local_f20__1
   STA   ga6+2
   BMI   .sx_neg@38
   LDA   #$00
   BEQ   .sx_done@39
.sx_neg@38:
   LDA   #$FF
.sx_done@39:
   STA   __zpabi_mix__v_1
   LDA   __local_f20__l22_0
   CLC
   ADC   __local_f20__1
   STA   __local_f20__1
   LDA   __local_f20__l22_1
   ADC   __zpabi_mix__v_1
   STA   __zpabi_mix__v_1
   BMI   .sx_neg@40
   LDA   #$00
   BEQ   .sx_done@41
.sx_neg@40:
   LDA   #$FF
.sx_done@41:
   JSR   fin0@run6
   LDA   __local_f20__1
   JSR   fin0@run3
   LDA   gs7+1
   STA   __zpabi_mix__v_1
   LDA   gs7+2
   STA   __local_f20__4
   LDA   __zpabi_mix__v_1
   SEC
   SBC   #$01
   STA   __local_f20__i24
   BCS   .carry_inc@4
   DEC   __local_f20__4
.carry_inc@4:
   STA   gs7+1
   LDA   __local_f20__4
   STA   gs7+2
   BMI   .sx_neg@42
   LDA   #$00
   BEQ   .sx_done@43
.sx_neg@42:
   LDA   #$FF
.sx_done@43:
   LDX   __local_f20__i24
   STX   g2
   LDX   __local_f20__4
   STX   g2+1
   STA   g2+2
   STA   g2+3
   STA   g2+4
   STA   g2+5
   STA   g2+6
   STA   g2+7
   LDA   z4
   STA   __zpabi_mix__v_1
   LDA   z4+1
   LDA   z4+2
   LDA   z4+3
   LDA   __zpabi_mix__v_1
   RTS

main:
   SUBROUTINE

   LDA   #$FF
   STA   __local_main__l26
   STA   __local_main__l26+1
   LDA   #$04
   STA   __local_main__i27
.main@rot@0:
   LDA   #$00
   STA   g3
   STA   g3+1
   DEC   __local_main__i27
   BNE   .main@rot@0
   LDA   #$01
   STA   __local_main__i28
.loop@19_start:
   LDA   __local_main__i28
   BNE   .lb_skip@5
   JMP   .loop@19_break
.lb_skip@5:
   LDA   g2
   STA   __local_main__i27
   LDA   g2+1
   STA   __local_main__6
   LDA   g2+2
   STA   __local_main__5
   LDA   g2+3
   STA   __local_main__4
   LDA   g2+4
   STA   __local_main__3
   LDA   g2+5
   STA   __local_main__2
   LDA   g2+6
   STA   __local_main__1
   LDA   g2+7
   STA   __local_main__0
   INC   __local_main__i27
   BNE   .inc_done@1
   INC   __local_main__6
   BNE   .inc_done@1
   INC   __local_main__5
   BNE   .inc_done@1
   INC   __local_main__4
   BNE   .inc_done@1
   INC   __local_main__3
   BNE   .inc_done@1
   INC   __local_main__2
   BNE   .inc_done@1
   INC   __local_main__1
   BNE   .inc_done@1
   INC   __local_main__0
.inc_done@1:
   LDA   __local_main__i27
   STA   g2
   LDA   __local_main__6
   STA   g2+1
   LDA   __local_main__5
   STA   g2+2
   LDA   __local_main__4
   STA   g2+3
   LDA   __local_main__3
   STA   g2+4
   LDA   __local_main__2
   STA   g2+5
   LDA   __local_main__1
   STA   g2+6
   LDA   __local_main__0
   STA   g2+7
   LDA   g1
   LSR
   LDA   #$00
   ROR
   CLC
   ADC   g1
   STA   __local_main__0
   STA   g1
   BMI   .sx_neg@44
   LDA   #$00
   BEQ   .sx_done@45
.sx_neg@44:
   LDA   #$FF
.sx_done@45:
   STA   __local_main__i27
   LDA   __local_main__0
   ORA   #$01
   ORA   __local_main__i27
   BNE   .lb_skip@4
   JMP   .if_end@11
.lb_skip@4:
   LDA   g2
   LDX   g2+1
   STX   __local_main__6
   LDX   g2+2
   STX   __local_main__5
   LDX   g2+3
   STX   __local_main__4
   LDX   g2+4
   STX   __local_main__3
   LDX   g2+5
   STX   __local_main__2
   LDX   g2+6
   STX   __local_main__1
   LDX   g2+7
   STX   __local_main__0
   SEC
   SBC   #$03
   STA   __local_main__i27
   LDA   __local_main__6
   SBC   #$00
   STA   __local_main__6
   LDA   __local_main__5
   SBC   #$00
   STA   __local_main__5
   LDA   __local_main__4
   SBC   #$00
   STA   __local_main__4
   LDA   __local_main__3
   SBC   #$00
   STA   __local_main__3
   LDA   __local_main__2
   SBC   #$00
   STA   __local_main__2
   LDA   __local_main__1
   SBC   #$00
   STA   __local_main__1
   BCS   .carry_inc@5
   DEC   __local_main__0
.carry_inc@5:
   LDA   __local_main__i27
   STA   g2
   LDA   __local_main__6
   STA   g2+1
   LDA   __local_main__5
   STA   g2+2
   LDA   __local_main__4
   STA   g2+3
   LDA   __local_main__3
   STA   g2+4
   LDA   __local_main__2
   STA   g2+5
   LDA   __local_main__1
   STA   g2+6
   LDA   __local_main__0
   STA   g2+7
   LDA   #$80
   AND   __local_main__i27
   AND   #$07
   CMP   #$06
   BNE   .default@22
   JSR   f16
   STA   __zpabi_mix__v_0
   JSR   f10@run4
.default@22:
.if_end@11:
   LDA   gs7+1
   BNE   .carry_inc@6
   DEC   gs7+2
.carry_inc@6:
   DEC   gs7+1
   DEC   __local_main__i28
   JMP   .loop@19_start
.loop@19_break:
   LDY   #$00
   LDA   (gp8),Y
   STA   __local_main__2
   INY
   LDA   (gp8),Y
   STA   __local_main__1
   LDA   __local_main__2
   SEC
   SBC   #$01
   STA   __local_main__0
   LDA   __local_main__1
   SBC   #$00
   STA   __local_main__i27
   LDA   __local_main__0
   DEY
   STA   (gp8),Y
   LDA   __local_main__i27
   INY
   STA   (gp8),Y
   LDA   __local_main__2
   ORA   __local_main__1
   BEQ   .if_end@12
   LDA   #$00
   STA   __zpabi_f10__p11_0
   STA   __zpabi_f10__p11_2
   STA   __zpabi_f10__p11_3
   LDA   #$80
   STA   __zpabi_f10__p11_1
   LDA   #<__local_main__l26
   STA   __zpabi_f10__pp12_0
   LDA   #>__local_main__l26
   STA   __zpabi_f10__pp12_1
   JSR   f10
   STA   __local_main__l26
   STX   __local_main__l26+1
.if_end@12:
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
   DC.B  $03, $FF, $03, $76

gs7:
   DS.B  1
   DC.W  $33C4
   DC.L  $7FFFFFFF

__zp_init:
   SUBROUTINE

   LDA   #$00
   STA   g1
   STA   g2
   STA   g2+1
   STA   g2+2
   STA   g2+4
   STA   g2+5
   STA   g2+6
   STA   g2+7
   STA   g3+1
   STA   z4+1
   STA   z4+2
   STA   z4+3
   STA   z5
   LDA   #$80
   STA   g2+3
   STA   z5+1
   LDA   #$02
   STA   g3
   LDA   #$0C
   STA   z4
   LDA   #<g3
   STA   gp8
   LDA   #>g3
   STA   gp8+1
   LDA   #<z5
   STA   gp9
   LDA   #>z5
   STA   gp9+1
   RTS
