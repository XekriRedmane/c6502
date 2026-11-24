SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
z3	EQU	$26
gp6	EQU	$28
gp7	EQU	$2A
g2	EQU	$2C
g1	EQU	$2D
__zpabi_f23__p25_0	EQU	$80
__zpabi_f23__p25_1	EQU	$81
__zpabi_f15__p17_0	EQU	$82
__zpabi_f15__p17_1	EQU	$83
__zpabi_f15__p18	EQU	$84
__zpabi_mix__v_0	EQU	$85
__zpabi_mix__v_1	EQU	$86
__zpabi_mix__v_2	EQU	$87
__zpabi_mix__v_3	EQU	$88
__local_f23__0	EQU	$89
__local_main__w30	EQU	$89
__local_main__0	EQU	$8A
__local_f23__2	EQU	$8B
__local_main__1	EQU	$8B
__local_f23__3	EQU	$8C
__local_main__2	EQU	$8C
__local_f23__4	EQU	$8D
__local_main__l27_1	EQU	$8D
__local_f23__5	EQU	$8E
__local_main__l27_0	EQU	$8E
__local_f23__6	EQU	$8F
__local_f23__7	EQU	$90
__local_f23__p24_1	EQU	$91
__local_f23__p24_0	EQU	$92
__local_f15__1	EQU	$94
__local_f15__w22	EQU	$95
__local_f15__2	EQU	$96
__local_f15__3	EQU	$97
__local_f15__4	EQU	$98
__local_f15__5	EQU	$99
__local_f15__6	EQU	$9A
__local_f15__l20	EQU	$9B
__local_f8__0	EQU	$9D
__local_f8__pp9	EQU	$9E
__local_f8__1	EQU	$9F
__local_mix__0	EQU	$A0
__local_mix__1	EQU	$A1
__local_mix__2	EQU	$A2
__local_mix__3	EQU	$A3

; @zp-link-meta-begin
; def f15 params=__zpabi_f15__p16_0,__zpabi_f15__p16_1,__zpabi_f15__p17_0,__zpabi_f15__p17_1,__zpabi_f15__p18 locals=__local_f15__0,__local_f15__1,__local_f15__w22,__local_f15__2,__local_f15__3,__local_f15__4,__local_f15__5,__local_f15__6,__local_f15__l20,__local_f15__8 indirect=false in_cycle=false param_regs=A,X,,,
; def f23 params=__zpabi_f23__p24_0,__zpabi_f23__p24_1,__zpabi_f23__p25_0,__zpabi_f23__p25_1 locals=__local_f23__0,__local_f23__1,__local_f23__2,__local_f23__3,__local_f23__4,__local_f23__5,__local_f23__6,__local_f23__7,__local_f23__p24_1,__local_f23__p24_0 indirect=false in_cycle=false param_regs=A,X,,
; def f8 params=__zpabi_f8__pp9_0,__zpabi_f8__pp9_1 locals=__local_f8__0,__local_f8__pp9,__local_f8__1 indirect=false in_cycle=false param_regs=A,X
; def fin0 params= locals=__local_fin0__0,__local_fin0__1,__local_fin0__2,__local_fin0__3 indirect=false in_cycle=false
; def fin1 params= locals=__local_fin1__0,__local_fin1__1,__local_fin1__2,__local_fin1__3 indirect=false in_cycle=false
; def fin2 params= locals=__local_fin2__0,__local_fin2__1 indirect=false in_cycle=false
; def main params= locals=__local_main__w30,__local_main__0,__local_main__1,__local_main__2,__local_main__l27_1,__local_main__l27_0 indirect=false in_cycle=false
; def mix params=__zpabi_mix__v_0,__zpabi_mix__v_1,__zpabi_mix__v_2,__zpabi_mix__v_3 locals=__local_mix__0,__local_mix__1,__local_mix__2,__local_mix__3 indirect=false in_cycle=false
; call f15 -> f8
; call f15 -> mix
; call f23 -> f15
; call f23 -> mix
; call f8 -> mix
; call fin0 -> mix
; call fin1 -> mix
; call fin2 -> mix
; call main -> fin0
; call main -> fin1
; call main -> fin2
; item mix section=.text local=false zero=false
; item fin0 section=.text local=false zero=false
; item fin0@run1 section=.text local=true zero=false
; item fin0@run0 section=.text local=true zero=false
; item fin1 section=.text local=false zero=false
; item fin1@run3 section=.text local=true zero=false
; item fin1@run2 section=.text local=true zero=false
; item fin1@tail0 section=.text local=true zero=false
; item fin2 section=.text local=false zero=false
; item f8 section=.text local=false zero=false
; item f15 section=.text local=false zero=false
; item f23 section=.text local=false zero=false
; item main section=.text local=false zero=false
; item acc section=.bss local=false zero=true
; item ga4 section=.data local=false zero=false
; item gs5 section=.data local=false zero=false
; item @10.s21 section=.data local=true zero=false
; item __zp_init section=.text local=false zero=false
; zp z3 size=2 at=auto def=true init=$99,$44
; zp gp6 size=2 at=auto def=true init=<g1,>g1 weight=163
; zp gp7 size=2 at=auto def=true init=<g2,>g2 weight=69
; zp g2 size=1 at=auto def=true init=$FF weight=27
; zp g1 size=1 at=auto def=true init=$FF weight=5
; runtime hargs_size=32 hargs_need=11
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
   JSR   fin0@run1
   LDA   z3+1
   BMI   .sx_neg@0
   LDA   #$00
   BEQ   .sx_done@1
.sx_neg@0:
   LDA   #$FF
.sx_done@1:
   STA   __zpabi_mix__v_2
   STA   __zpabi_mix__v_3
   LDA   z3
   STA   __zpabi_mix__v_0
   LDA   z3+1
   STA   __zpabi_mix__v_1
   JSR   mix
   LDA   ga4
   STA   __zpabi_mix__v_0
   LDA   ga4+1
   STA   __zpabi_mix__v_1
   LDA   ga4+2
   STA   __zpabi_mix__v_2
   LDA   ga4+3
   STA   __zpabi_mix__v_3
   JMP   mix

fin0@run1:
   SUBROUTINE

   STA   __zpabi_mix__v_0
   JMP   fin0@run0

fin0@run0:
   SUBROUTINE

   LDA   #$00
   STA   __zpabi_mix__v_1
   STA   __zpabi_mix__v_2
   STA   __zpabi_mix__v_3
   JMP   mix

fin1:
   SUBROUTINE

   JSR   fin1@run3
   STA   __zpabi_mix__v_1
   LDA   ga4+10
   STA   __zpabi_mix__v_2
   LDA   ga4+11
   JSR   fin1@run2
   LDA   ga4+16
   STA   __zpabi_mix__v_0
   LDA   ga4+17
   STA   __zpabi_mix__v_1
   LDA   ga4+18
   STA   __zpabi_mix__v_2
   LDA   ga4+19
   JSR   fin1@run2
   LDA   ga4+24
   STA   __zpabi_mix__v_0
   LDA   ga4+25
   STA   __zpabi_mix__v_1
   LDA   ga4+26
   STA   __zpabi_mix__v_2
   LDA   ga4+27
   JSR   fin1@run2
   LDA   gs5+1
   STA   __zpabi_mix__v_0
   LDA   gs5+2
   JMP   fin1@tail0

fin1@run3:
   SUBROUTINE

   LDA   ga4+8
   STA   __zpabi_mix__v_0
   LDA   ga4+9
   RTS

fin1@run2:
   SUBROUTINE

   STA   __zpabi_mix__v_3
   JMP   mix

fin1@tail0:
   SUBROUTINE

   STA   __zpabi_mix__v_1
   LDA   #$00
   STA   __zpabi_mix__v_2
   STA   __zpabi_mix__v_3
   JMP   mix

fin2:
   SUBROUTINE

   LDA   gs5+3
   STA   __zpabi_mix__v_0
   LDA   gs5+4
   JMP   fin1@tail0

f8:
   SUBROUTINE

   STA   __local_f8__pp9
   STX   __local_f8__1
   LDY   #$00
   LDA   (__local_f8__pp9),Y
   JSR   fin0@run1
   LDA   gs5+1
   STA   __local_f8__pp9
   LDA   gs5+2
   STA   __local_f8__0
   LDA   __local_f8__pp9
   CLC
   ADC   #$01
   STA   gs5+1
   LDA   __local_f8__0
   ADC   #$00
   STA   gs5+2
   LDA   __local_f8__pp9
   ORA   __local_f8__0
   BEQ   .if_end@4
   INC   g1
   LDY   #$00
   LDA   (gp7),Y
   CLC
   ADC   g1
   RTS
.if_end@4:
   LDY   #$00
   LDA   (gp7),Y
   RTS

f15:
   SUBROUTINE

   LDA   #$7F
   STA   __local_f15__l20
   LDA   #$00
   STA   __local_f15__w22
.loop@3_start:
   LDY   #$00
   LDA   (gp6),Y
   CLC
   ADC   #$01
   STA   (gp6),Y
   LDA   gs5+3
   SEC
   SBC   #$5A
   STA   gs5+3
   BCS   .carry_inc@0
   DEC   gs5+4
.carry_inc@0:
   LDX   #>__local_f15__l20
   LDA   #<__local_f15__l20
   JSR   f8
   STA   __zpabi_mix__v_0
   ORA   #$00
   BMI   .sx_neg@2
   LDA   #$00
   BEQ   .sx_done@3
.sx_neg@2:
   LDA   #$FF
.sx_done@3:
   STA   __zpabi_mix__v_1
   STA   __zpabi_mix__v_2
   STA   __zpabi_mix__v_3
   JSR   mix
   INC   __local_f15__w22
   BEQ   .loop@3_start
   LDA   ga4+24
   STA   __local_f15__6
   LDA   ga4+25
   STA   __local_f15__5
   LDA   ga4+26
   STA   __local_f15__4
   LDA   ga4+27
   STA   __local_f15__3
   LDA   ga4+28
   STA   __local_f15__2
   LDA   ga4+29
   STA   __local_f15__w22
   LDA   ga4+30
   STA   __local_f15__1
   LDA   ga4+31
   STA   __zpabi_mix__v_0
   LDA   __local_f15__6
   CLC
   ADC   #$01
   STA   ga4+24
   LDA   __local_f15__5
   ADC   #$00
   STA   ga4+25
   LDA   __local_f15__4
   ADC   #$00
   STA   ga4+26
   LDA   __local_f15__3
   ADC   #$00
   STA   ga4+27
   LDA   __local_f15__2
   ADC   #$00
   STA   ga4+28
   LDA   __local_f15__w22
   ADC   #$00
   STA   ga4+29
   LDA   __local_f15__1
   ADC   #$00
   STA   ga4+30
   LDA   __zpabi_mix__v_0
   ADC   #$00
   STA   ga4+31
   LDA   __local_f15__6
   EOR   #$81
   STA   __zpabi_mix__v_0
   LDA   @10.s21
   SEC
   SBC   __zpabi_mix__v_0
   STA   __local_f15__2
   LDA   @10.s21+1
   SBC   #$00
   STA   __local_f15__w22
   LDA   @10.s21+2
   SBC   #$00
   STA   __local_f15__1
   LDA   @10.s21+3
   SBC   #$00
   STA   __zpabi_mix__v_0
   LDA   __local_f15__2
   ORA   __local_f15__w22
   ORA   __local_f15__1
   ORA   __zpabi_mix__v_0
   BEQ   .if_else@6
   LDX   #$FF
   TXA
   RTS
.if_else@6:
   JSR   fin1@run3
   STA   __local_f15__1
   LDA   #$80
   AND   __zpabi_mix__v_0
   STA   __zpabi_mix__v_0
   LDA   #$00
   AND   __local_f15__1
   TAX
   LDA   __zpabi_mix__v_0
   RTS

f23:
   SUBROUTINE

   STA   __local_f23__p24_0
   STX   __local_f23__p24_1
   LDA   #$0E
   AND   ga4
   STA   g1
   INC   g2
   LDA   g2
   AND   #$07
   BNE   .lb_skip@2
   JMP   .switch@4_break
.lb_skip@2:
   CMP   #$04
   BNE   .lb_skip@1
   JMP   .switch@4_break
.lb_skip@1:
   CMP   #$05
   BEQ   .case@7
   CMP   #$06
   BNE   .lb_skip@0
   JMP   .case@8
.lb_skip@0:
   JMP   .default@9
.case@7:
   LDY   #$00
   LDA   (gp7),Y
   STA   __zpabi_mix__v_0
   SEC
   SBC   #$01
   STA   (gp7),Y
   JSR   fin0@run0
   LDA   ga4
   LDX   ga4+1
   STX   __local_f23__7
   LDX   ga4+2
   STX   __local_f23__6
   LDX   ga4+3
   STX   __local_f23__5
   LDX   ga4+4
   STX   __local_f23__4
   LDX   ga4+5
   STX   __local_f23__3
   LDX   ga4+6
   STX   __local_f23__2
   LDX   ga4+7
   STX   __zpabi_mix__v_0
   SEC
   SBC   #$FF
   STA   __local_f23__0
   LDA   __local_f23__7
   SBC   #$00
   STA   ga4+1
   LDA   __local_f23__6
   SBC   #$00
   STA   ga4+2
   LDA   __local_f23__5
   SBC   #$00
   STA   ga4+3
   LDA   __local_f23__4
   SBC   #$00
   STA   ga4+4
   LDA   __local_f23__3
   SBC   #$00
   STA   ga4+5
   LDA   __local_f23__2
   SBC   #$00
   STA   ga4+6
   LDA   __zpabi_mix__v_0
   SBC   #$00
   STA   ga4+7
   LDA   __local_f23__0
   STA   ga4
   LDY   #$00
   STA   (gp6),Y
.case@8:
   LDA   #$03
   LDY   #$00
   STA   (gp7),Y
   JMP   .switch@4_break
.default@9:
   LDA   #$FF
   STA   z3
   STA   __local_f23__2
   STA   HARGS
   LDA   z3+1
   STA   HARGS+1
   LDA   #$03
   STA   HARGS+2
   LDA   #$00
   STA   HARGS+3
   JSR   sdivmod16
   LDA   __local_f23__2
   SEC
   SBC   HARGS+6
   LDA   #$00
   SBC   HARGS+7
   BVC   .cmp_novf@4
   EOR   #$80
.cmp_novf@4:
   ASL
   LDA   #$00
   ROL
   RTS
.switch@4_break:
   LDY   #$00
   LDA   (gp6),Y
   SEC
   SBC   #$01
   STA   (gp6),Y
   STA   ga4+16
   LDA   #$00
   STA   ga4+17
   STY   ga4+18
   STY   ga4+19
   STY   ga4+20
   STY   ga4+21
   STY   ga4+22
   STA   ga4+23
   LDA   __local_f23__p24_0
   CLC
   SBC   gs5+1
   LDA   __local_f23__p24_1
   SBC   gs5+2
   TYA
   ROL
   STA   __local_f23__3
   LDA   z3
   ORA   z3+1
   BEQ   .and_false@9
   LDA   g1
   BEQ   .and_false@9
   LDA   #$01
   STA   __zpabi_f15__p18
   JMP   .and_end@10
.and_false@9:
   LDA   #$00
   STA   __zpabi_f15__p18
.and_end@10:
   LDA   z3
   STA   __zpabi_f15__p17_0
   LDA   z3+1
   STA   __zpabi_f15__p17_1
   LDX   #$00
   LDA   __local_f23__3
   JSR   f15
   LDA   gs5+1
   RTS

main:
   SUBROUTINE

   LDA   gs5+3
   STA   gs5+1
   LDA   gs5+4
   STA   gs5+2
   LDA   ga4
   SEC
   SBC   #$FF
   STA   ga4
   LDA   ga4+1
   SBC   #$00
   STA   ga4+1
   LDA   ga4+2
   SBC   #$00
   STA   ga4+2
   LDA   ga4+3
   SBC   #$00
   STA   ga4+3
   LDA   ga4+4
   SBC   #$00
   STA   ga4+4
   LDA   ga4+5
   SBC   #$00
   STA   ga4+5
   LDA   ga4+6
   SBC   #$00
   STA   ga4+6
   BCS   .carry_inc@1
   DEC   ga4+7
.carry_inc@1:
   LDA   #$FF
   STA   __local_main__l27_0
   LDA   #$7F
   STA   __local_main__l27_1
   LDA   #$00
   STA   __local_main__w30
.loop@10_continue:
   CLC
   ADC   #$01
   STA   __local_main__2
   LDA   __local_main__w30
   CMP   #$02
   BCC   .lb_skip@3
   JMP   .loop@10_break
.lb_skip@3:
   LDA   #$00
   STA   __local_main__w30
.loop@11_continue:
   CLC
   ADC   #$01
   STA   __local_main__1
   LDA   __local_main__w30
   CMP   #$04
   BCS   .loop@11_break
   LDY   #$00
   LDA   (gp6),Y
   AND   #$5A
   STA   (gp6),Y
   AND   #$07
   CMP   #$02
   BEQ   .switch@12_break
   CMP   #$03
   BEQ   .case@14
   CMP   #$05
   BEQ   .case@15
   CMP   #$06
   BEQ   .case@16
   BNE   .default@17
.case@14:
   LDA   __local_main__l27_0
   ASL
   STA   __local_main__w30
   LDA   __local_main__l27_1
   ROL
   ASL   __local_main__w30
   ROL
   STA   __local_main__0
   LDA   __local_main__w30
   CLC
   ADC   __local_main__l27_0
   STA   __local_main__l27_0
   LDA   __local_main__0
   ADC   __local_main__l27_1
   STA   __local_main__l27_1
   JMP   .switch@12_break
.case@15:
   LDA   #$00
   AND   __local_main__2
   LDY   #$00
   STA   (gp7),Y
   JMP   .switch@12_break
.case@16:
.default@17:
   LDA   #$FF
   EOR   z3
   STA   __local_main__l27_0
   LDA   #$00
   EOR   z3+1
   STA   __local_main__l27_1
.switch@12_break:
   LDA   __local_main__1
   STA   __local_main__w30
   JMP   .loop@11_continue
.loop@11_break:
   LDA   __local_main__2
   STA   __local_main__w30
   JMP   .loop@10_continue
.loop@10_break:
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

ga4:
   DC.L  $00000002
   DC.L  $00000000
   DS.B  8
   DC.L  $00000001
   DC.L  $00000000
   DC.L  $000000FF
   DC.L  $00000000

gs5:
   DS.B  1
   DC.W  $FFFF
   DC.W  $0080

@10.s21:
   DC.L  $80000000

__zp_init:
   SUBROUTINE

   LDA   #$FF
   STA   g1
   STA   g2
   LDA   #$99
   STA   z3
   LDA   #$44
   STA   z3+1
   LDA   #<g1
   STA   gp6
   LDA   #>g1
   STA   gp6+1
   LDA   #<g2
   STA   gp7
   LDA   #>g2
   STA   gp7+1
   RTS
