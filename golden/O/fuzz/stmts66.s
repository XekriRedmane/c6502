SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
z3	EQU	$26
z4	EQU	$28
z5	EQU	$30
g1	EQU	$31
__zpabi_mix__v_0	EQU	$80
__zpabi_mix__v_1	EQU	$81
__zpabi_mix__v_2	EQU	$82
__zpabi_mix__v_3	EQU	$83
__local_main__2	EQU	$86
__local_main__3	EQU	$87
__local_main__4	EQU	$88
__local_main__5	EQU	$89
__local_main__6	EQU	$8A
__local_main__7	EQU	$8B
__local_main__w17	EQU	$8C
__local_f10__l14	EQU	$8D
__local_f10__0	EQU	$8E
__local_f10__1	EQU	$8F
__local_f10__2	EQU	$90
__local_f10__3	EQU	$91
__local_f10__4	EQU	$92
__local_f10__5	EQU	$93
__local_f10__p11_1	EQU	$94
__local_f10__p11_0	EQU	$95
__local_f10__8	EQU	$98
__local_f10__l13	EQU	$99
__local_f10__9	EQU	$9A
__local_f10__10	EQU	$9B
__local_f10__11	EQU	$9C
__local_f10__12	EQU	$9D
__local_f10__13	EQU	$9E
__local_mix__0	EQU	$9F
__local_mix__1	EQU	$A0
__local_mix__2	EQU	$A1
__local_mix__3	EQU	$A2

; @zp-link-meta-begin
; def f10 params=__zpabi_f10__p11_0,__zpabi_f10__p11_1 locals=__local_f10__l14,__local_f10__0,__local_f10__1,__local_f10__2,__local_f10__3,__local_f10__4,__local_f10__5,__local_f10__p11_1,__local_f10__p11_0,__local_f10__6,__local_f10__7,__local_f10__8,__local_f10__l13,__local_f10__9,__local_f10__10,__local_f10__11,__local_f10__12,__local_f10__13 indirect=false in_cycle=false param_regs=A,X
; def fin0 params= locals=__local_fin0__0,__local_fin0__1 indirect=false in_cycle=false
; def fin1 params= locals=__local_fin1__0,__local_fin1__1,__local_fin1__2,__local_fin1__3 indirect=false in_cycle=false
; def fin2 params= locals=__local_fin2__0,__local_fin2__1,__local_fin2__2,__local_fin2__3 indirect=false in_cycle=false
; def main params= locals=__local_main__0,__local_main__1,__local_main__2,__local_main__3,__local_main__4,__local_main__5,__local_main__6,__local_main__7,__local_main__w17 indirect=false in_cycle=false
; def mix params=__zpabi_mix__v_0,__zpabi_mix__v_1,__zpabi_mix__v_2,__zpabi_mix__v_3 locals=__local_mix__0,__local_mix__1,__local_mix__2,__local_mix__3 indirect=false in_cycle=false
; call f10 -> mix
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
; item fin0@run4 section=.text local=true zero=false
; item fin0@run2 section=.text local=true zero=false
; item fin0@run1 section=.text local=true zero=false
; item fin1 section=.text local=false zero=false
; item fin2 section=.text local=false zero=false
; item f10 section=.text local=false zero=false
; item f10@run3 section=.text local=true zero=false
; item f10@run0 section=.text local=true zero=false
; item main section=.text local=false zero=false
; item acc section=.bss local=false zero=true
; item g2 section=.data local=false zero=false
; item ga6 section=.data local=false zero=false
; item gs7 section=.data local=false zero=false
; item gp8 section=.data local=false zero=false
; item gp9 section=.data local=false zero=false
; item __zp_init section=.text local=false zero=false
; zp z3 size=2 at=auto def=true init=$FF,$FF
; zp z4 size=8 at=auto def=true init=$03,$00,$00,$00,$00,$00,$00,$00
; zp z5 size=1 at=auto def=true init=$01
; zp g1 size=4 at=auto def=true init=$00,$00,$00,$80 weight=13
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

   JSR   fin0@run2
   LDA   g2+1
   BMI   .sx_neg@0
   LDA   #$00
   BEQ   .sx_done@1
.sx_neg@0:
   LDA   #$FF
.sx_done@1:
   STA   __zpabi_mix__v_2
   STA   __zpabi_mix__v_3
   LDA   g2
   STA   __zpabi_mix__v_0
   LDA   g2+1
   STA   __zpabi_mix__v_1
   JSR   mix
   LDA   z3
   STA   __zpabi_mix__v_0
   LDA   z3+1
   STA   __zpabi_mix__v_1
   LDA   #$00
   STA   __zpabi_mix__v_2
   JSR   fin0@run1
   LDA   z4
   STA   __zpabi_mix__v_0
   LDA   z4+1
   STA   __zpabi_mix__v_1
   LDA   z4+2
   STA   __zpabi_mix__v_2
   LDA   z4+3
   STA   __zpabi_mix__v_3
   JSR   fin0@run4
   JMP   mix

fin0@run4:
   SUBROUTINE

   LDA   z4+4
   LDA   z4+5
   LDA   z4+6
   LDA   z4+7
   RTS

fin0@run2:
   SUBROUTINE

   LDA   g1
   STA   __zpabi_mix__v_0
   LDA   g1+1
   STA   __zpabi_mix__v_1
   LDA   g1+2
   STA   __zpabi_mix__v_2
   LDA   g1+3
   JMP   fin0@run1

fin0@run1:
   SUBROUTINE

   STA   __zpabi_mix__v_3
   JMP   mix

fin1:
   SUBROUTINE

   LDA   z5
   BMI   .sx_neg@2
   LDA   #$00
   BEQ   .sx_done@3
.sx_neg@2:
   LDA   #$FF
.sx_done@3:
   STA   __zpabi_mix__v_1
   STA   __zpabi_mix__v_2
   STA   __zpabi_mix__v_3
   LDA   z5
   STA   __zpabi_mix__v_0
   JSR   mix
   LDA   ga6
   STA   __zpabi_mix__v_0
   LDA   ga6+1
   STA   __zpabi_mix__v_1
   LDA   ga6+2
   STA   __zpabi_mix__v_2
   LDA   ga6+3
   JSR   fin0@run1
   LDA   ga6+4
   STA   __zpabi_mix__v_0
   LDA   ga6+5
   STA   __zpabi_mix__v_1
   LDA   ga6+6
   STA   __zpabi_mix__v_2
   LDA   ga6+7
   JSR   fin0@run1
   LDA   ga6+8
   STA   __zpabi_mix__v_0
   LDA   ga6+9
   STA   __zpabi_mix__v_1
   LDA   ga6+10
   STA   __zpabi_mix__v_2
   LDA   ga6+11
   STA   __zpabi_mix__v_3
   JMP   mix

fin2:
   SUBROUTINE

   LDA   ga6+12
   STA   __zpabi_mix__v_0
   LDA   ga6+13
   STA   __zpabi_mix__v_1
   LDA   ga6+14
   STA   __zpabi_mix__v_2
   LDA   ga6+15
   JSR   fin0@run1
   LDA   gs7+1
   STA   __zpabi_mix__v_0
   LDA   gs7+2
   STA   __zpabi_mix__v_1
   LDA   gs7+3
   STA   __zpabi_mix__v_2
   LDA   gs7+4
   JSR   fin0@run1
   LDA   gs7+5
   STA   __zpabi_mix__v_0
   LDA   gs7+6
   STA   __zpabi_mix__v_1
   LDA   gs7+7
   STA   __zpabi_mix__v_2
   LDA   gs7+8
   STA   __zpabi_mix__v_3
   JMP   mix

f10:
   SUBROUTINE

   STA   __local_f10__p11_0
   STX   __local_f10__p11_1
   LDA   #$03
   STA   __local_f10__l14
   LDA   #$00
   STA   __local_f10__0
   STA   __local_f10__1
   STA   __local_f10__2
   LDA   #$02
   STA   __local_f10__l13
   LDA   #$00
   STA   __local_f10__9
   STA   __local_f10__10
   STA   __local_f10__11
   STA   __local_f10__12
   STA   __local_f10__13
   LDA   __local_f10__l13
   STA   __local_f10__l14
   LDA   __local_f10__9
   STA   __local_f10__3
   LDA   __local_f10__10
   STA   __local_f10__2
   LDA   __local_f10__11
   STA   __local_f10__1
   LDA   __local_f10__12
   STA   __local_f10__0
   LDA   __local_f10__13
   STA   __local_f10__5
   INC   __local_f10__l14
   BNE   .inc_done@0
   INC   __local_f10__3
   BNE   .inc_done@0
   INC   __local_f10__2
   BNE   .inc_done@0
   INC   __local_f10__1
   BNE   .inc_done@0
   INC   __local_f10__0
   BNE   .inc_done@0
   INC   __local_f10__5
.inc_done@0:
   LDA   __local_f10__l14
   STA   __local_f10__4
   LDA   __local_f10__5
   AND   #$0F
   LDY   __local_f10__4
   STY   __local_f10__l13
   LDY   __local_f10__3
   STY   __local_f10__9
   LDY   __local_f10__2
   STY   __local_f10__10
   LDY   __local_f10__1
   STY   __local_f10__11
   LDY   __local_f10__0
   STY   __local_f10__12
   STA   __local_f10__13
   LDA   gp8
   STA   DPTR
   LDA   gp8+1
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   STA   __local_f10__0
   INY
   LDA   (DPTR),Y
   STA   __local_f10__l14
   LDA   __local_f10__0
   AND   __local_f10__p11_0
   STA   __local_f10__0
   LDA   __local_f10__l14
   AND   __local_f10__p11_1
   STA   __local_f10__l14
   JSR   f10@run0
   LDA   __local_f10__0
   JSR   f10@run3
   LDA   __local_f10__l14
   LDY   #$01
   STA   (DPTR),Y
   LDA   #$00
   CMP   #$77
   BNE   .if_else@1
   CMP   #$C1
   BNE   .if_else@1
   LDA   z3+1
   CMP   #$2C
   BNE   .if_else@1
   LDA   z3
   CMP   #$ED
   BNE   .if_else@1
   INC   g1
   BNE   .inc_done@1
   INC   g1+1
   BNE   .inc_done@1
   INC   g1+2
   BNE   .inc_done@1
   INC   g1+3
.inc_done@1:
   JSR   fin0@run2
   JSR   f10@run0
   LDY   #$00
   LDA   (DPTR),Y
   STA   __local_f10__l14
   INY
   LDA   (DPTR),Y
   STA   __local_f10__0
   LDA   __local_f10__l14
   BNE   .carry_inc@0
   DEC   __local_f10__0
.carry_inc@0:
   DEC   __local_f10__l14
   LDA   __local_f10__l14
   JSR   f10@run3
   LDA   __local_f10__0
   LDY   #$01
   STA   (DPTR),Y
   LDA   #$ED
   LDX   #$2C
   RTS
.if_else@1:
   LDA   ga6+12
   EOR   #$5A
   STA   ga6+12
   AND   #$07
   CMP   #$01
   BEQ   .case@1
   CMP   #$02
   BEQ   .case@2
   CMP   #$03
   BNE   .lb_skip@1
   JMP   .case@7
.lb_skip@1:
   CMP   #$04
   BNE   .lb_skip@0
   JMP   .case@8
.lb_skip@0:
   LDA   #$ED
   LDX   #$2C
   RTS
.case@1:
   TXA
   BMI   .sx_neg@4
   LDA   #$00
   BEQ   .sx_done@5
.sx_neg@4:
   LDA   #$FF
.sx_done@5:
   STA   __local_f10__4
   STA   __local_f10__3
   LDA   gs7+5
   STA   __local_f10__2
   LDA   gs7+6
   STA   __local_f10__1
   LDA   gs7+7
   STA   __local_f10__0
   LDA   gs7+8
   STA   __local_f10__l14
   LDA   __local_f10__p11_0
   AND   __local_f10__2
   STA   __zpabi_mix__v_0
   TXA
   AND   __local_f10__1
   STA   __zpabi_mix__v_1
   LDA   __local_f10__4
   AND   __local_f10__0
   STA   __zpabi_mix__v_2
   LDA   __local_f10__3
   AND   __local_f10__l14
   JSR   fin0@run1
   LDA   #$EE
   LDX   #$2C
   RTS
.case@2:
   LDA   z4
   STA   __local_f10__4
   LDA   z4+1
   LDA   z4+2
   LDA   z4+3
   JSR   fin0@run4
   LDA   gs7+1
   LDX   gs7+2
   STX   __local_f10__0
   LDX   gs7+3
   STX   __local_f10__l14
   LDX   gs7+4
   STX   __local_f10__1
   ASL
   ROL   __local_f10__0
   ROL   __local_f10__l14
   LDA   __local_f10__1
   ROL
   STA   gs7+3
   LDA   #$00
   ROL
   STA   gs7+4
   LDA   __local_f10__0
   STA   gs7+1
   LDA   __local_f10__l14
   STA   gs7+2
   LDA   __local_f10__4
   CLC
   ADC   __local_f10__0
   AND   #$07
   BEQ   .switch@3_break
   CMP   #$03
   BEQ   .case@5
   CMP   #$06
   BEQ   .case@6
   JMP   .case@8
.case@5:
   LDA   g1
   ASL
   STA   __local_f10__l14
   LDA   g1+1
   ROL
   STA   __local_f10__0
   LDA   g1+2
   ROL
   STA   __local_f10__1
   LDA   g1+3
   ROL
   STA   __local_f10__2
   LDA   __local_f10__l14
   CLC
   ADC   g1
   STA   g1
   LDA   __local_f10__0
   ADC   g1+1
   STA   g1+1
   LDA   __local_f10__1
   ADC   g1+2
   STA   g1+2
   LDA   __local_f10__2
   ADC   g1+3
   STA   g1+3
.case@6:
.switch@3_break:
   LDA   #$ED
   LDX   #$2C
   RTS
.case@7:
   LDA   __local_f10__p11_0
   STA   __local_f10__8
   LDA   __local_f10__l13
   LDY   __local_f10__9
   STY   __local_f10__4
   LDY   __local_f10__10
   STY   __local_f10__3
   LDY   __local_f10__11
   STY   __local_f10__2
   LDY   __local_f10__12
   STY   __local_f10__1
   LDY   __local_f10__13
   STY   __local_f10__0
   SEC
   SBC   #$FF
   STA   __local_f10__l14
   LDA   __local_f10__4
   SBC   #$00
   STA   __local_f10__4
   LDA   __local_f10__3
   SBC   #$00
   STA   __local_f10__3
   LDA   __local_f10__2
   SBC   #$00
   STA   __local_f10__2
   LDA   __local_f10__1
   SBC   #$00
   STA   __local_f10__1
   BCS   .carry_inc@1
   DEC   __local_f10__0
.carry_inc@1:
   LDA   __local_f10__0
   AND   #$0F
   LDY   __local_f10__l14
   STY   __local_f10__l13
   LDY   __local_f10__4
   STY   __local_f10__9
   LDY   __local_f10__3
   STY   __local_f10__10
   LDY   __local_f10__2
   STY   __local_f10__11
   LDY   __local_f10__1
   STY   __local_f10__12
   STA   __local_f10__13
   LDA   __local_f10__8
   AND   __local_f10__l14
   STA   __local_f10__l14
   TXA
   AND   __local_f10__4
   TAX
   LDA   __local_f10__l14
   RTS
.case@8:
   LDA   #$ED
   LDX   #$2C
   RTS

f10@run3:
   SUBROUTINE

   LDY   #$00
   STA   (DPTR),Y
   JMP   f10@run0

f10@run0:
   SUBROUTINE

   LDA   gp9
   STA   DPTR
   LDA   gp9+1
   STA   DPTR+1
   RTS

main:
   SUBROUTINE

   LDA   z5
   BMI   .sx_neg@8
   LDA   #$00
   BEQ   .sx_done@9
.sx_neg@8:
   LDA   #$FF
.sx_done@9:
   ASL
   LDA   #$00
   ROL
   EOR   #$01
   LDX   #$00
   JSR   f10
   LDA   #$00
   STA   __local_main__w17
.loop@9_start:
   AND   #$03
   ASL
   ASL
   TAX
   LDA   ga6,X
   EOR   #$FF
   STA   __local_main__3
   LDA   ga6+1,X
   EOR   #$FF
   STA   __local_main__2
   LDA   ga6+2,X
   EOR   #$FF
   STA   __zpabi_mix__v_1
   LDA   ga6+3,X
   EOR   #$FF
   CMP   g1+3
   BNE   .cmp_true@10
   LDA   __zpabi_mix__v_1
   CMP   g1+2
   BNE   .cmp_true@10
   LDA   __local_main__2
   CMP   g1+1
   BNE   .cmp_true@10
   LDA   __local_main__3
   CMP   g1
   BNE   .cmp_true@10
   LDA   #$00
   BEQ   .cmp_end@11
.cmp_true@10:
   LDA   #$01
.cmp_end@11:
   STA   z5
   LDA   z4
   STA   __zpabi_mix__v_0
   LDA   z4+1
   STA   __local_main__7
   LDA   z4+2
   STA   __local_main__6
   LDA   z4+3
   STA   __local_main__5
   LDA   z4+4
   STA   __local_main__4
   LDA   z4+5
   STA   __local_main__3
   LDA   z4+6
   STA   __local_main__2
   LDA   z4+7
   STA   __zpabi_mix__v_1
   INC   __zpabi_mix__v_0
   BNE   .inc_done@2
   INC   __local_main__7
   BNE   .inc_done@2
   INC   __local_main__6
   BNE   .inc_done@2
   INC   __local_main__5
   BNE   .inc_done@2
   INC   __local_main__4
   BNE   .inc_done@2
   INC   __local_main__3
   BNE   .inc_done@2
   INC   __local_main__2
   BNE   .inc_done@2
   INC   __zpabi_mix__v_1
.inc_done@2:
   LDA   __zpabi_mix__v_0
   STA   z4
   LDA   __local_main__7
   STA   z4+1
   LDA   __local_main__6
   STA   z4+2
   LDA   __local_main__5
   STA   z4+3
   LDA   __local_main__4
   STA   z4+4
   LDA   __local_main__3
   STA   z4+5
   LDA   __local_main__2
   STA   z4+6
   LDA   __zpabi_mix__v_1
   STA   z4+7
   INC   __local_main__w17
   LDA   __local_main__w17
   CMP   #$04
   BCS   .lb_skip@2
   JMP   .loop@9_start
.lb_skip@2:
   LDA   g2+1
   STA   __zpabi_mix__v_1
   LDA   g2
   BNE   .carry_inc@2
   DEC   g2+1
.carry_inc@2:
   DEC   g2
   CLC
   ADC   #$80
   STA   __zpabi_mix__v_0
   BCC   .carry_inc@3
   INC   __zpabi_mix__v_1
.carry_inc@3:
   LDA   __zpabi_mix__v_1
   BMI   .sx_neg@13
   LDA   #$00
   BEQ   .sx_done@14
.sx_neg@13:
   LDA   #$FF
.sx_done@14:
   STA   __zpabi_mix__v_2
   JSR   fin0@run1
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
   DC.W  $FFFF

ga6:
   DS.B  4
   DC.L  $80000000
   DC.L  $968F0CBF
   DC.L  $0000FFFF

gs7:
   DS.B  1
   DC.L  $00007FFF
   DC.L  $00000002

gp8:
   DC.W  g2

gp9:
   DC.W  z3

__zp_init:
   SUBROUTINE

   LDA   #$00
   STA   g1
   STA   g1+1
   STA   g1+2
   STA   z4+1
   STA   z4+2
   STA   z4+3
   STA   z4+4
   STA   z4+5
   STA   z4+6
   STA   z4+7
   LDA   #$80
   STA   g1+3
   LDA   #$FF
   STA   z3
   STA   z3+1
   LDA   #$03
   STA   z4
   LDA   #$01
   STA   z5
   RTS
