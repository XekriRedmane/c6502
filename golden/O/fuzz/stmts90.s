SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
z4	EQU	$26
z5	EQU	$28
g2	EQU	$2A
g3	EQU	$2C
g1	EQU	$2E
__zpabi_f10__p13_0	EQU	$80
__zpabi_f20__p22_0	EQU	$80
__zpabi_f10__p13_1	EQU	$81
__zpabi_f20__p22_1	EQU	$81
__zpabi_f10__pp14_0	EQU	$82
__zpabi_f20__pp23_0	EQU	$82
__zpabi_f10__pp14_1	EQU	$83
__zpabi_f20__pp23_1	EQU	$83
__local_f10__w19	EQU	$84
__zpabi_mix__v_0	EQU	$84
__local_f10__0	EQU	$85
__zpabi_mix__v_1	EQU	$85
__local_f10__1	EQU	$86
__zpabi_mix__v_2	EQU	$86
__local_f10__2	EQU	$87
__zpabi_mix__v_3	EQU	$87
__local_main__0	EQU	$88
__local_main__1	EQU	$89
__local_f10__l15	EQU	$8A
__local_main__2	EQU	$8A
__local_f10__5	EQU	$8B
__local_main__l30	EQU	$8B
__local_main__3	EQU	$8C
__local_main__l32	EQU	$8D
__local_f20__p21_0	EQU	$91
__local_f20__p21_1	EQU	$92
__local_f20__0	EQU	$93
__local_f20__1	EQU	$94
__local_f20__i27	EQU	$95
__local_f20__2	EQU	$96
__local_mix__0	EQU	$98
__local_mix__1	EQU	$99
__local_mix__2	EQU	$9A
__local_mix__3	EQU	$9B

; @zp-link-meta-begin
; def f10 params=__zpabi_f10__p11,__zpabi_f10__p12,__zpabi_f10__p13_0,__zpabi_f10__p13_1,__zpabi_f10__pp14_0,__zpabi_f10__pp14_1 locals=__local_f10__w19,__local_f10__0,__local_f10__1,__local_f10__2,__local_f10__3,__local_f10__4,__local_f10__l15,__local_f10__5 indirect=false in_cycle=false param_regs=A,X,,,,
; def f20 params=__zpabi_f20__p21_0,__zpabi_f20__p21_1,__zpabi_f20__p22_0,__zpabi_f20__p22_1,__zpabi_f20__pp23_0,__zpabi_f20__pp23_1 locals=__local_f20__p21_0,__local_f20__p21_1,__local_f20__0,__local_f20__1,__local_f20__i27,__local_f20__2,__local_f20__3 indirect=false in_cycle=false param_regs=A,X,,,,
; def fin0 params= locals=__local_fin0__0,__local_fin0__1 indirect=false in_cycle=false
; def fin1 params= locals=__local_fin1__0,__local_fin1__1 indirect=false in_cycle=false
; def fin2 params= locals=__local_fin2__0,__local_fin2__1,__local_fin2__2 indirect=false in_cycle=false
; def main params= locals=__local_main__0,__local_main__1,__local_main__2,__local_main__l30,__local_main__3,__local_main__l32,__local_main__5,__local_main__6,__local_main__7 indirect=false in_cycle=false
; def mix params=__zpabi_mix__v_0,__zpabi_mix__v_1,__zpabi_mix__v_2,__zpabi_mix__v_3 locals=__local_mix__0,__local_mix__1,__local_mix__2,__local_mix__3 indirect=false in_cycle=false
; call f20 -> mix
; call fin0 -> mix
; call fin1 -> mix
; call fin2 -> mix
; call main -> f20
; call main -> fin0
; call main -> fin1
; call main -> fin2
; item mix section=.text local=false zero=false
; item fin0 section=.text local=false zero=false
; item fin0@run0 section=.text local=true zero=false
; item fin0@run0@run3 section=.text local=true zero=false
; item fin0@tail0 section=.text local=true zero=false
; item fin1 section=.text local=false zero=false
; item fin2 section=.text local=false zero=false
; item f10 section=.text local=false zero=false
; item f20 section=.text local=false zero=false
; item f20@run2 section=.text local=true zero=false
; item f20@run1 section=.text local=true zero=false
; item main section=.text local=false zero=false
; item acc section=.bss local=false zero=true
; item ga6 section=.data local=false zero=false
; item gs7 section=.data local=false zero=false
; item gp8 section=.data local=false zero=false
; item gp9 section=.data local=false zero=false
; item @5.s17 section=.data local=true zero=false
; item @13.s26 section=.data local=true zero=false
; item __zp_init section=.text local=false zero=false
; zp z4 size=2 at=auto def=true init=$09,$00
; zp z5 size=2 at=auto def=true init=$00,$80
; zp g2 size=2 at=auto def=true init=$00,$80 weight=31
; zp g3 size=2 at=auto def=true init=$0B,$00 weight=26
; zp g1 size=2 at=auto def=true init=$00,$00 weight=18
; runtime hargs_size=32 hargs_need=20
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
   JSR   fin0@run0
   LDA   g2
   STA   __zpabi_mix__v_0
   LDA   g2+1
   JSR   fin0@run0
   LDA   g3+1
   BMI   .sx_neg@0
   LDA   #$00
   BEQ   .sx_done@1
.sx_neg@0:
   LDA   #$FF
.sx_done@1:
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
   JMP   fin0@tail0

fin0@run0:
   SUBROUTINE

   JSR   fin0@run0@run3
   JMP   mix

fin0@run0@run3:
   SUBROUTINE

   STA   __zpabi_mix__v_1
   LDA   #$00
   STA   __zpabi_mix__v_2
   STA   __zpabi_mix__v_3
   RTS

fin0@tail0:
   SUBROUTINE

   JSR   fin0@run0@run3
   JMP   mix

fin1:
   SUBROUTINE

   LDA   z5
   STA   __zpabi_mix__v_0
   LDA   z5+1
   STA   __zpabi_mix__v_1
   BMI   .sx_neg@2
   LDA   #$00
   BEQ   .sx_done@3
.sx_neg@2:
   LDA   #$FF
.sx_done@3:
   STA   __zpabi_mix__v_2
   STA   __zpabi_mix__v_3
   JSR   mix
   LDA   ga6
   STA   __zpabi_mix__v_0
   LDA   ga6+1
   JSR   fin0@run0
   LDA   ga6+2
   STA   __zpabi_mix__v_0
   LDA   ga6+3
   JSR   fin0@run0
   LDA   ga6+4
   STA   __zpabi_mix__v_0
   LDA   ga6+5
   JMP   fin0@tail0

fin2:
   SUBROUTINE

   LDA   ga6+6
   STA   __zpabi_mix__v_0
   LDA   ga6+7
   JSR   fin0@run0
   LDA   gs7+2
   BMI   .sx_neg@4
   LDA   #$00
   BEQ   .sx_done@5
.sx_neg@4:
   LDA   #$FF
.sx_done@5:
   STA   __zpabi_mix__v_2
   STA   __zpabi_mix__v_3
   LDA   gs7+1
   STA   __zpabi_mix__v_0
   LDA   gs7+2
   STA   __zpabi_mix__v_1
   JSR   mix
   LDA   gs7+5
   BMI   .sx_neg@6
   LDA   #$00
   BEQ   .sx_done@7
.sx_neg@6:
   LDA   #$FF
.sx_done@7:
   STA   __zpabi_mix__v_3
   LDA   gs7+3
   STA   __zpabi_mix__v_0
   LDA   gs7+4
   STA   __zpabi_mix__v_1
   LDA   gs7+5
   STA   __zpabi_mix__v_2
   JMP   mix

f10:
   SUBROUTINE

   LDA   #$00
   STA   __local_f10__l15
   LDA   #$80
   STA   __local_f10__5
   LDA   #$00
   STA   __local_f10__w19
   LDA   #$80
   STA   __local_f10__0
   LDA   #$00
   STA   __local_f10__1
   STA   __local_f10__2
   LDA   __local_f10__l15
   LDX   __local_f10__5
   STX   __local_f10__w19
   ASL
   LDA   __local_f10__w19
   ROL
   STA   __local_f10__0
   LDA   #$00
   ADC   #$FF
   EOR   #$FF
   LDX   __local_f10__0
   STX   __local_f10__l15
   STA   __local_f10__5
   LDA   __local_f10__0
   EOR   @5.s17
   AND   #$07
   BEQ   .case@1
   CMP   #$02
   BNE   .lb_skip@2
   JMP   .case@13
.lb_skip@2:
   CMP   #$04
   BNE   .lb_skip@1
   JMP   .case@14
.lb_skip@1:
   CMP   #$05
   BNE   .lb_skip@0
   JMP   .case@15
.lb_skip@0:
   JMP   .switch@0_break
.case@1:
   LDA   z5
   STA   __local_f10__w19
   LDA   z5+1
   LDA   gs7+3
   EOR   __local_f10__w19
   AND   #$07
   CMP   #$02
   BEQ   .case@3
   CMP   #$06
   BEQ   .case@4
   BNE   .default@10
.case@3:
   LDA   ga6+2
   STA   __local_f10__2
   LDA   ga6+3
   STA   __local_f10__1
   LDA   __local_f10__2
   ASL
   STA   __local_f10__w19
   LDA   __local_f10__1
   ROL
   STA   __local_f10__0
   LDA   __local_f10__w19
   CLC
   ADC   __local_f10__2
   STA   ga6+2
   LDA   __local_f10__0
   ADC   __local_f10__1
   STA   ga6+3
   JMP   .switch@0_break
.case@4:
   LDA   g2
   INC   g2
   BNE   .inc_done@0
   INC   g2+1
.inc_done@0:
   SEC
   SBC   #$02
   AND   #$07
   BEQ   .switch@5_break
   CMP   #$02
   BEQ   .switch@5_break
   CMP   #$04
   BEQ   .switch@5_break
   CMP   #$05
   BNE   .switch@5_break
   LDA   z4
   EOR   #$03
   STA   z4
   STA   g3
   LDA   z4+1
   STA   g3+1
.switch@5_break:
   LDA   __local_f10__l15
   LDA   __local_f10__5
   JMP   .switch@0_break
.default@10:
   LDA   #$00
   STA   __local_f10__w19
   CLC
   JMP   .loop@12_continue
.f10@rot@0:
   LDA   g3
   STA   __local_f10__w19
   BNE   .carry_inc@0
   DEC   g3+1
.carry_inc@0:
   DEC   g3
   LDA   __local_f10__0
   AND   __local_f10__w19
   STA   __local_f10__l15
   LDA   #$00
   STA   __local_f10__5
   LDA   __local_f10__0
   STA   __local_f10__w19
.loop@12_continue:
   ADC   #$01
   STA   __local_f10__0
   LDA   __local_f10__w19
   CMP   #$02
   BCC   .f10@rot@0
.case@13:
.case@14:
.case@15:
.switch@0_break:
   SEC
   LDA   #$00
   SBC   #$01
   LDA   #$00
   SBC   #$00
   LDA   #$00
   ROL
   RTS

f20:
   SUBROUTINE

   STA   __local_f20__p21_0
   STX   __local_f20__p21_1
   LDA   z5
   STA   __local_f20__0
   LDA   z5+1
   STA   __local_f20__1
   INC   __local_f20__0
   BNE   .inc_done@1
   INC   __local_f20__1
.inc_done@1:
   LDA   __local_f20__0
   STA   z5
   LDA   __local_f20__1
   STA   z5+1
   LDA   gs7+1
   STA   __local_f20__1
   LDA   gs7+2
   CLC
   ADC   #$80
   STA   __local_f20__0
   LDA   __local_f20__1
   CLC
   SBC   #$18
   LDA   __local_f20__0
   SBC   #$00
   LDA   #$00
   ROL
   STA   __local_f20__0
   LDA   #$00
   BPL   .sx_done@11
   LDA   #$FF
.sx_done@11:
   STA   __zpabi_mix__v_2
   STA   __zpabi_mix__v_3
   LDA   __local_f20__0
   STA   __zpabi_mix__v_0
   LDA   #$00
   STA   __zpabi_mix__v_1
   JSR   mix
   JSR   f20@run1
   LDY   #$00
   LDA   (DPTR),Y
   STA   __local_f20__0
   INY
   LDA   (DPTR),Y
   STA   __local_f20__i27
   INC   __local_f20__0
   BNE   .inc_done@2
   INC   __local_f20__i27
.inc_done@2:
   LDA   __local_f20__0
   LDY   #$00
   STA   (DPTR),Y
   JSR   f20@run1
   LDA   __local_f20__i27
   LDY   #$01
   STA   (DPTR),Y
   LDA   __local_f20__0
   EOR   #$7F
   LDX   __local_f20__i27
   STX   __local_f20__0
   LDX   gp9
   STX   DPTR
   LDX   gp9+1
   STX   DPTR+1
   DEY
   STA   (DPTR),Y
   JSR   f20@run2
   LDA   __local_f20__0
   LDY   #$01
   STA   (DPTR),Y
   LDA   gs7+1
   STA   __local_f20__0
   LDA   gs7+2
   STA   __local_f20__1
   ASL
   ROR   __local_f20__1
   ROR   __local_f20__0
   LDA   __local_f20__1
   ASL
   ROR   __local_f20__1
   ROR   __local_f20__0
   LDA   __local_f20__0
   STA   gs7+1
   LDA   __local_f20__1
   STA   gs7+2
   LDA   __local_f20__0
   ORA   __local_f20__1
   BNE   .lb_skip@3
   JMP   .if_end@0
.lb_skip@3:
   LDY   #$00
   LDA   (__zpabi_f20__pp23_0),Y
   STA   HARGS
   INY
   LDA   (__zpabi_f20__pp23_0),Y
   STA   HARGS+1
   INY
   LDA   (__zpabi_f20__pp23_0),Y
   STA   HARGS+2
   INY
   LDA   (__zpabi_f20__pp23_0),Y
   STA   HARGS+3
   LDA   #$5A
   STA   HARGS+4
   LDA   #$00
   STA   HARGS+5
   LDY   #$00
   STY   HARGS+6
   STA   HARGS+7
   JSR   mul32
   LDA   HARGS+8
   STA   __local_f20__2
   LDA   HARGS+9
   STA   __local_f20__i27
   LDA   HARGS+10
   STA   __local_f20__1
   LDA   HARGS+11
   AND   #$01
   EOR   #$01
   STA   __local_f20__0
   DEC   __local_f20__0
   LDA   __local_f20__2
   LDY   #$00
   STA   (__zpabi_f20__pp23_0),Y
   LDA   __local_f20__i27
   INY
   STA   (__zpabi_f20__pp23_0),Y
   LDA   __local_f20__1
   INY
   STA   (__zpabi_f20__pp23_0),Y
   LDA   __local_f20__0
   INY
   STA   (__zpabi_f20__pp23_0),Y
   LDA   #$7F
   ORA   __local_f20__2
   STA   z4
   LDA   __local_f20__i27
   STA   z4+1
   LDA   #$01
   STA   __local_f20__i27
.f20@rot@1:
   LDA   g1
   SEC
   SBC   #$01
   STA   __local_f20__1
   LDA   g1+1
   SBC   #$00
   STA   __local_f20__0
   STA   g1+1
   LDA   __local_f20__1
   STA   g1
   LDA   @13.s26
   CLC
   ADC   __local_f20__1
   STA   __zpabi_mix__v_0
   LDA   @13.s26+1
   ADC   __local_f20__0
   STA   __zpabi_mix__v_1
   LDA   @13.s26+2
   ADC   #$00
   STA   __zpabi_mix__v_2
   LDA   @13.s26+3
   ADC   #$00
   STA   __zpabi_mix__v_3
   JSR   mix
   DEC   __local_f20__i27
   BNE   .f20@rot@1
.if_end@0:
   LDA   ga6+2
   ORA   ga6+3
   BEQ   .if_else@2
   LDA   @13.s26
   STA   HARGS
   LDA   @13.s26+1
   STA   HARGS+1
   LDA   @13.s26+2
   STA   HARGS+2
   LDA   @13.s26+3
   STA   HARGS+3
   LDA   #$06
   STA   HARGS+4
   JSR   asl32
   LDA   #$00
   SEC
   SBC   HARGS+5
   STA   __local_f20__0
   LDA   #$00
   SBC   HARGS+6
   STA   __local_f20__p21_1
   LDA   #$00
   SBC   HARGS+7
   AND   #$03
   LDX   __local_f20__0
   STX   HARGS+8
   LDX   __local_f20__p21_1
   STX   HARGS+9
   EOR   #$02
   SEC
   SBC   #$02
   STA   HARGS+10
   RTS
.if_else@2:
   JSR   f20@run2
   LDY   #$00
   LDA   (DPTR),Y
   STA   __local_f20__0
   INY
   LDA   (DPTR),Y
   LSR
   STA   __local_f20__1
   ROR   __local_f20__0
   LSR   __local_f20__p21_1
   ROR   __local_f20__p21_0
   LSR   __local_f20__p21_1
   ROR   __local_f20__p21_0
   LSR   __local_f20__p21_1
   ROR   __local_f20__p21_0
   LSR   __local_f20__p21_1
   ROR   __local_f20__p21_0
   LDA   __local_f20__0
   AND   __local_f20__p21_0
   STA   gs7+3
   LDA   __local_f20__1
   AND   __local_f20__p21_1
   STA   gs7+4
   LDA   #$00
   STA   gs7+5
   LDA   z5
   ORA   z5+1
   BEQ   .and_false@3
   DEY
   LDA   (DPTR),Y
   STA   __local_f20__p21_1
   INY
   LDA   (DPTR),Y
   ORA   __local_f20__p21_1
   BEQ   .and_false@3
   LDA   #$01
   STA   __local_f20__p21_1
   LDA   #$00
   BEQ   .and_end@4
.and_false@3:
   LDA   #$00
   STA   __local_f20__p21_1
.and_end@4:
   ORA   #$00
   BPL   .sx_done@13
   LDA   #$FF
.sx_done@13:
   STA   HARGS+10
   LDA   __local_f20__p21_1
   STA   HARGS+8
   LDA   #$00
   STA   HARGS+9
   RTS

f20@run2:
   SUBROUTINE

   LDA   gp9
   STA   DPTR
   LDA   gp9+1
   STA   DPTR+1
   RTS

f20@run1:
   SUBROUTINE

   LDA   gp8
   STA   DPTR
   LDA   gp8+1
   STA   DPTR+1
   RTS

main:
   SUBROUTINE

   LDA   #$03
   STA   __local_main__l30
   LDA   #$00
   STA   __local_main__3
   STA   __local_main__l32
   LDA   #$01
   STA   __local_main__l32+1
   LDA   #$00
   STA   __local_main__l32+2
   STA   __local_main__l32+3
   LDA   __local_main__l30
   STA   __local_main__2
   LDA   __local_main__3
   STA   __local_main__1
   LDA   #$00
   SEC
   SBC   __local_main__2
   STA   __local_main__0
   LDA   __local_main__2
   SBC   __local_main__1
   LDX   __local_main__0
   STX   __local_main__l30
   STA   __local_main__3
   LDA   __local_main__0
   AND   #$07
   CMP   #$03
   BEQ   .switch@19_break
   LDA   g1
   SEC
   SBC   #$FF
   STA   __local_main__2
   LDA   g1+1
   SBC   #$00
   TAX
   LDA   #$FF
   ORA   g2
   STA   __zpabi_f20__p22_0
   LDA   g2+1
   STA   __zpabi_f20__p22_1
   LDA   #<__local_main__l32
   STA   __zpabi_f20__pp23_0
   LDA   #>__local_main__l32
   STA   __zpabi_f20__pp23_1
   LDA   __local_main__2
   JSR   f20
   LDA   HARGS+8
   STA   __local_main__l30
   LDA   HARGS+9
   STA   __local_main__3
.switch@19_break:
   JSR   f20@run1
   LDY   #$00
   LDA   (DPTR),Y
   STA   __local_main__1
   INY
   LDA   (DPTR),Y
   ORA   __local_main__1
   BEQ   .cond_else@11
   LDA   z4
   STA   __local_main__l30
   LDA   z4+1
   STA   __local_main__2
   LDA   #$00
   STA   __local_main__1
   STA   __local_main__0
   JMP   .cond_end@12
.cond_else@11:
   LDA   __local_main__l32
   STA   __local_main__l30
   LDA   __local_main__l32+1
   STA   __local_main__2
   LDA   __local_main__l32+2
   STA   __local_main__1
   LDA   __local_main__l32+3
   STA   __local_main__0
.cond_end@12:
   LDA   __local_main__l30
   ORA   __local_main__2
   ORA   __local_main__1
   ORA   __local_main__0
   BNE   .vthread@0
   LDA   z5
   LDA   z5+1
.vthread@0:
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
   DC.W  $0007
   DC.W  $007F
   DC.W  $0001
   DC.W  $7FFF

gs7:
   DS.B  1
   DC.W  $D768
   DC.B  $FF
   DS.B  2

gp8:
   DC.W  g3

gp9:
   DC.W  z4

@5.s17:
   DC.W  $FFFF

@13.s26:
   DC.L  $00000002

__zp_init:
   SUBROUTINE

   LDA   #$00
   STA   g1
   STA   g1+1
   STA   g2
   STA   g3+1
   STA   z4+1
   STA   z5
   LDA   #$80
   STA   g2+1
   STA   z5+1
   LDA   #$0B
   STA   g3
   LDA   #$09
   STA   z4
   RTS
