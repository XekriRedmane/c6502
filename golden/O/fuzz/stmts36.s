SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
z4	EQU	$26
z5	EQU	$28
gp8	EQU	$2C
g3	EQU	$2E
g1	EQU	$30
g2	EQU	$31
__zpabi_f17__pp20_0	EQU	$80
__zpabi_f9__p11_0	EQU	$80
__zpabi_f17__pp20_1	EQU	$81
__zpabi_f9__p11_1	EQU	$81
__zpabi_f9__p11_2	EQU	$82
__zpabi_f9__p11_3	EQU	$83
__zpabi_mix__v_0	EQU	$84
__zpabi_mix__v_1	EQU	$85
__zpabi_mix__v_2	EQU	$86
__zpabi_mix__v_3	EQU	$87
__local_f17__w23	EQU	$88
__local_f9__w16	EQU	$88
__local_f17__0	EQU	$89
__local_f9__l14	EQU	$89
__local_f17__p19	EQU	$8A
__local_f9__0	EQU	$8A
__local_f17__1	EQU	$8B
__local_f9__1	EQU	$8B
__local_f9__2	EQU	$8C
__local_main__i29	EQU	$8C
__local_f9__3	EQU	$8D
__local_main__4	EQU	$8D
__local_f9__4	EQU	$8E
__local_main__5	EQU	$8E
__local_f9__5	EQU	$8F
__local_main__6	EQU	$8F
__local_main__7	EQU	$90
__local_main__8	EQU	$91
__local_main__9	EQU	$92
__local_main__w28	EQU	$93
__local_main__l26	EQU	$94
__local_main__10	EQU	$95
__local_mix__0	EQU	$96
__local_mix__1	EQU	$97
__local_mix__2	EQU	$98
__local_mix__3	EQU	$99

; @zp-link-meta-begin
; def f17 params=__zpabi_f17__p18,__zpabi_f17__p19,__zpabi_f17__pp20_0,__zpabi_f17__pp20_1 locals=__local_f17__w23,__local_f17__0,__local_f17__p19,__local_f17__1,__local_f17__2 indirect=false in_cycle=false param_regs=A,X,,
; def f9 params=__zpabi_f9__p10,__zpabi_f9__p11_0,__zpabi_f9__p11_1,__zpabi_f9__p11_2,__zpabi_f9__p11_3 locals=__local_f9__w16,__local_f9__l14,__local_f9__0,__local_f9__1,__local_f9__2,__local_f9__3,__local_f9__4,__local_f9__5 indirect=false in_cycle=false param_regs=A,,,,
; def fin0 params= locals=__local_fin0__0,__local_fin0__1 indirect=false in_cycle=false
; def fin1 params= locals=__local_fin1__0,__local_fin1__1 indirect=false in_cycle=false
; def fin2 params= locals=__local_fin2__0,__local_fin2__1 indirect=false in_cycle=false
; def main params= locals=__local_main__0,__local_main__1,__local_main__2,__local_main__3,__local_main__i29,__local_main__4,__local_main__5,__local_main__6,__local_main__7,__local_main__8,__local_main__9,__local_main__w28,__local_main__l26,__local_main__10 indirect=false in_cycle=false
; def mix params=__zpabi_mix__v_0,__zpabi_mix__v_1,__zpabi_mix__v_2,__zpabi_mix__v_3 locals=__local_mix__0,__local_mix__1,__local_mix__2,__local_mix__3 indirect=false in_cycle=false
; call f17 -> mix
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
; item fin0@run6 section=.text local=true zero=false
; item fin0@run5 section=.text local=true zero=false
; item fin0@run0 section=.text local=true zero=false
; item fin1 section=.text local=false zero=false
; item fin1@run3 section=.text local=true zero=false
; item fin1@run1 section=.text local=true zero=false
; item fin1@run1@run7 section=.text local=true zero=false
; item fin1@tail0 section=.text local=true zero=false
; item fin2 section=.text local=false zero=false
; item f9 section=.text local=false zero=false
; item f17 section=.text local=false zero=false
; item main section=.text local=false zero=false
; item main@run8 section=.text local=true zero=false
; item main@run4 section=.text local=true zero=false
; item main@run2 section=.text local=true zero=false
; item acc section=.bss local=false zero=true
; item ga6 section=.data local=false zero=false
; item gs7 section=.data local=false zero=false
; item __zp_init section=.text local=false zero=false
; zp z4 size=2 at=auto def=true init=$7F,$00
; zp z5 size=4 at=auto def=true init=$80,$00,$00,$00
; zp gp8 size=2 at=auto def=true init=<g1,>g1 weight=104
; zp g3 size=2 at=auto def=true init=$02,$00 weight=68
; zp g1 size=1 at=auto def=true init=$FF weight=12
; zp g2 size=1 at=auto def=true init=$00 weight=3
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
   JSR   fin0@run5
   LDA   g2
   BMI   .sx_neg@0
   LDA   #$00
   BEQ   .sx_done@1
.sx_neg@0:
   LDA   #$FF
.sx_done@1:
   STA   __zpabi_mix__v_1
   JSR   fin0@run6
   LDA   g2
   STA   __zpabi_mix__v_0
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
   LDA   z4+1
   BMI   .sx_neg@4
   LDA   #$00
   BEQ   .sx_done@5
.sx_neg@4:
   LDA   #$FF
.sx_done@5:
   JSR   fin0@run6
   LDA   z4
   STA   __zpabi_mix__v_0
   LDA   z4+1
   STA   __zpabi_mix__v_1
   JMP   mix

fin0@run6:
   SUBROUTINE

   STA   __zpabi_mix__v_2
   STA   __zpabi_mix__v_3
   RTS

fin0@run5:
   SUBROUTINE

   STA   __zpabi_mix__v_0
   JMP   fin0@run0

fin0@run0:
   SUBROUTINE

   LDA   #$00
   STA   __zpabi_mix__v_1
   JSR   fin0@run6
   JMP   mix

fin1:
   SUBROUTINE

   JSR   fin1@run3
   JSR   mix
   LDA   ga6
   STA   __zpabi_mix__v_0
   LDA   ga6+1
   JSR   fin1@run1
   LDA   ga6+2
   STA   __zpabi_mix__v_0
   LDA   ga6+3
   JSR   fin1@run1
   LDA   ga6+4
   STA   __zpabi_mix__v_0
   LDA   ga6+5
   JMP   fin1@tail0

fin1@run3:
   SUBROUTINE

   LDA   z5
   STA   __zpabi_mix__v_0
   LDA   z5+1
   STA   __zpabi_mix__v_1
   LDA   z5+2
   STA   __zpabi_mix__v_2
   LDA   z5+3
   STA   __zpabi_mix__v_3
   RTS

fin1@run1:
   SUBROUTINE

   JSR   fin1@run1@run7
   JMP   mix

fin1@run1@run7:
   SUBROUTINE

   STA   __zpabi_mix__v_1
   LDA   #$00
   STA   __zpabi_mix__v_2
   STA   __zpabi_mix__v_3
   RTS

fin1@tail0:
   SUBROUTINE

   JSR   fin1@run1@run7
   JMP   mix

fin2:
   SUBROUTINE

   LDA   ga6+6
   STA   __zpabi_mix__v_0
   LDA   ga6+7
   JSR   fin1@run1
   LDA   gs7+1
   JSR   fin0@run5
   LDA   gs7+2
   STA   __zpabi_mix__v_0
   LDA   gs7+3
   JMP   fin1@tail0

f9:
   SUBROUTINE

   LDA   #$00
   STA   __local_f9__w16
   STA   __local_f9__l14
   LDA   #$01
   STA   __local_f9__0
   LDA   #$00
   STA   __local_f9__1
   STA   __local_f9__2
   STA   __local_f9__3
   STA   __local_f9__4
   STA   __local_f9__5
   LDA   #$80
   STA   __local_f9__w16
   LDA   #$FF
   STA   __local_f9__l14
   STA   __local_f9__0
   STA   __local_f9__1
   STA   __local_f9__2
   STA   __local_f9__3
   STA   __local_f9__4
   STA   __local_f9__5
   STA   __local_f9__l14
   LDX   #$00
   LDY   #$00
   STY   __local_f9__w16
.loop@0_start:
   LDA   __local_f9__l14
   SEC
   SBC   #$05
   STA   __local_f9__l14
   BCS   .carry_inc@0
   DEX
.carry_inc@0:
   INC   __local_f9__w16
   LDA   __local_f9__w16
   CMP   #$02
   BCC   .loop@0_start
   INC   g1
   LDA   g1
   JSR   fin0@run5
   LDA   g3
   STA   __local_f9__l14
   SEC
   SBC   #$01
   STA   g3
   BCS   .carry_inc@1
   DEC   g3+1
.carry_inc@1:
   LDA   __local_f9__l14
   ORA   g1
   AND   #$07
   BEQ   .case@7
   CMP   #$03
   BEQ   .case@8
   CMP   #$07
   BEQ   .case@9
   BNE   .switch@6_break
.case@7:
   LDA   z5
   LDX   z5+1
   STX   __local_f9__0
   LDX   z5+2
   STX   __local_f9__l14
   LDX   z5+3
   STX   __local_f9__w16
   EOR   #$01
   STA   z5
   LDX   __local_f9__0
   STX   z5+1
   LDX   __local_f9__l14
   STX   z5+2
   LDX   __local_f9__w16
   STX   z5+3
   STA   __zpabi_mix__v_0
   LDA   __local_f9__0
   STA   __zpabi_mix__v_1
   LDA   __local_f9__l14
   STA   __zpabi_mix__v_2
   LDA   __local_f9__w16
   STA   __zpabi_mix__v_3
   JSR   mix
.case@8:
.case@9:
.switch@6_break:
   LDA   gs7+1
   RTS

f17:
   SUBROUTINE

   STX   __local_f17__p19
   LDA   #$7F
   STA   __zpabi_mix__v_0
   JSR   fin0@run0
   DEC   g2
   BPL   .if_end@2
   LDA   #$00
   STA   __local_f17__w23
   CLC
   JMP   .loop@10_continue
.f17@rot@0:
   LDA   z5
   LDA   z5+1
   LDA   z5+2
   LDA   z5+3
   LDY   #$00
   LDA   (gp8),Y
   AND   #$5A
   STA   (gp8),Y
   EOR   __local_f17__0
   BEQ   .if_end@3
   LDA   z4
   STA   __local_f17__1
   LDA   z4+1
   STA   __local_f17__p19
   INC   z4
   BNE   .inc_done@0
   INC   z4+1
.inc_done@0:
   LDY   #$00
   LDA   (gp8),Y
   ADC   g1
   STA   __local_f17__w23
   LDA   #$00
   ADC   #$00
   STA   __local_f17__0
   LDA   __local_f17__1
   EOR   __local_f17__w23
   STA   __local_f17__w23
   LDA   __local_f17__p19
   EOR   __local_f17__0
   TAX
   LDA   __local_f17__w23
   RTS
.if_end@3:
   LDA   __local_f17__0
   STA   __local_f17__w23
.loop@10_continue:
   ADC   #$01
   STA   __local_f17__0
   LDA   __local_f17__w23
   CMP   #$04
   BCC   .f17@rot@0
.if_end@2:
   LDA   __local_f17__p19
   SEC
   SBC   ga6+6
   STA   __local_f17__w23
   LDA   #$00
   SBC   ga6+7
   TAX
   LDA   __local_f17__w23
   RTS

main:
   SUBROUTINE

   LDA   #$01
   STA   __local_main__l26
   LDA   #$00
   STA   __local_main__10
   JSR   main@run2
   CLC
   ADC   #$05
   STA   __zpabi_mix__v_3
   BCC   .carry_inc@2
   INC   __zpabi_mix__v_0
   BNE   .carry_inc@2
   INC   __zpabi_mix__v_1
   BNE   .carry_inc@2
   INC   __zpabi_mix__v_2
.carry_inc@2:
   JSR   main@run4
   LDA   #$FF
   SEC
   SBC   __zpabi_mix__v_3
   AND   #$07
   BEQ   .case@13
   CMP   #$02
   BEQ   .case@14
   BNE   .switch@12_break
.case@13:
   LDA   z5
   STA   HARGS
   LDA   z5+1
   STA   HARGS+1
   LDA   z5+2
   STA   HARGS+2
   LDA   z5+3
   STA   HARGS+3
   LDA   #$04
   STA   HARGS+4
   JSR   asl32
   LDA   HARGS+5
   STA   z5
   LDA   HARGS+6
   STA   z5+1
   LDA   HARGS+7
   STA   z5+2
   LDA   HARGS+8
   STA   z5+3
.case@14:
.switch@12_break:
   LDA   #$00
   STA   __local_main__w28
.loop@15_start:
   AND   #$03
   ASL
   CLC
   TAX
   LDA   ga6,X
   STA   __zpabi_mix__v_3
   LDA   ga6+1,X
   STA   __zpabi_mix__v_2
   LDA   __zpabi_mix__v_3
   ADC   #$FF
   STA   __zpabi_mix__v_3
   BCC   .carry_inc@3
   INC   __zpabi_mix__v_2
.carry_inc@3:
   STA   ga6,X
   LDA   __zpabi_mix__v_2
   STA   ga6+1,X
   LDA   __zpabi_mix__v_3
   ASL
   STA   __local_main__6
   LDA   __zpabi_mix__v_2
   ROL
   STA   __local_main__7
   LDA   #$00
   STA   __local_main__8
   ROL   __local_main__8
   ROL
   ASL   __local_main__6
   ROL   __local_main__7
   ROL   __local_main__8
   ROL
   STA   __local_main__9
   LDA   __local_main__6
   CLC
   ADC   __zpabi_mix__v_3
   STA   __local_main__5
   LDA   __local_main__7
   ADC   __zpabi_mix__v_2
   STA   __local_main__4
   BCC   .carry_inc@4
   INC   __local_main__8
   BNE   .carry_inc@4
   INC   __local_main__9
.carry_inc@4:
   STA   __zpabi_mix__v_1
   LDA   __local_main__5
   STA   __zpabi_mix__v_0
   LDA   __local_main__8
   STA   __zpabi_mix__v_2
   LDA   __local_main__9
   STA   __zpabi_mix__v_3
   JSR   mix
   LDA   __local_main__w28
   LSR
   BEQ   .cond_else@4
   JSR   fin1@run3
   LDA   #$FF
   SEC
   SBC   __zpabi_mix__v_0
   STA   __zpabi_mix__v_0
   LDA   #$FF
   SBC   __zpabi_mix__v_1
   STA   __zpabi_mix__v_1
   LDA   #$00
   SBC   __zpabi_mix__v_2
   STA   __zpabi_mix__v_2
   LDA   #$00
   SBC   __zpabi_mix__v_3
   STA   __zpabi_mix__v_3
   JMP   .cond_end@5
.cond_else@4:
   LDA   __local_main__l26
   STA   HARGS
   LDA   __local_main__10
   STA   HARGS+1
   LDY   #$00
   LDA   (gp8),Y
   STA   HARGS+2
   LDA   #$00
   STA   HARGS+3
   JSR   mul16
   JSR   main@run8
   BMI   .sx_neg@8
   LDA   #$00
   BEQ   .sx_done@9
.sx_neg@8:
   LDA   #$FF
.sx_done@9:
   JSR   fin0@run6
.cond_end@5:
   JSR   mix
   LDA   #$04
   STA   __local_main__i29
.loop@16_start:
   LDA   __local_main__i29
   BNE   .lb_skip@1
   JMP   .loop@16_break
.lb_skip@1:
   LDY   #$00
   LDA   (gp8),Y
   CMP   #$FF
   BNE   .cmp_true@10
   LDA   #$00
   BEQ   .cmp_end@11
.cmp_true@10:
   LDA   #$01
.cmp_end@11:
   STA   HARGS
   LDA   #$00
   STA   HARGS+1
   LDY   #$04
   STY   HARGS+2
   STA   HARGS+3
   JSR   sdivmod16
   LDA   HARGS+5
   BMI   .sx_neg@12
   LDA   #$00
   BEQ   .sx_done@13
.sx_neg@12:
   LDA   #$FF
.sx_done@13:
   JSR   fin0@run6
   JSR   main@run8
   JSR   mix
   JSR   main@run2
   SEC
   SBC   #$03
   STA   __zpabi_mix__v_3
   LDA   __zpabi_mix__v_0
   SBC   #$00
   STA   __zpabi_mix__v_0
   LDA   __zpabi_mix__v_1
   SBC   #$00
   STA   __zpabi_mix__v_1
   BCS   .carry_inc@5
   DEC   __zpabi_mix__v_2
.carry_inc@5:
   LDA   __zpabi_mix__v_3
   JSR   main@run4
   LDA   __local_main__w28
   CLC
   ADC   __zpabi_mix__v_3
   STA   g3
   LDA   #$00
   ADC   __zpabi_mix__v_0
   STA   g3+1
   LDA   gs7+2
   STA   __zpabi_mix__v_2
   LDA   gs7+3
   STA   __zpabi_mix__v_3
   LDA   __zpabi_mix__v_2
   SEC
   SBC   #$01
   STA   gs7+2
   LDA   __zpabi_mix__v_3
   SBC   #$00
   STA   gs7+3
   LDA   __local_main__w28
   AND   __zpabi_mix__v_2
   AND   #$07
   BEQ   .case@18
   CMP   #$03
   BEQ   .case@19
   CMP   #$06
   BEQ   .case@20
   BNE   .default@21
.case@18:
.case@19:
   LDA   __local_main__5
   STA   HARGS
   LDA   __local_main__4
   STA   HARGS+1
   LDA   __local_main__8
   STA   HARGS+2
   LDA   __local_main__9
   STA   HARGS+3
   LDA   #$03
   STA   HARGS+4
   LDA   #$00
   STA   HARGS+5
   LDY   #$00
   STY   HARGS+6
   STA   HARGS+7
   JSR   sdivmod32
   LDA   HARGS+8
   SEC
   SBC   #$05
   STA   z4
   LDA   HARGS+9
   SBC   #$00
   STA   z4+1
.case@20:
.default@21:
   DEC   __local_main__i29
   JMP   .loop@16_start
.loop@16_break:
   INC   __local_main__w28
   LDA   __local_main__w28
   CMP   #$03
   BCS   .lb_skip@0
   JMP   .loop@15_start
.lb_skip@0:
   JSR   fin0
   JSR   fin1
   JSR   fin2
   LDA   acc+1
   AND   #$7F
   TAX
   LDA   acc
   RTS

main@run8:
   SUBROUTINE

   LDA   HARGS+4
   STA   __zpabi_mix__v_0
   LDA   HARGS+5
   STA   __zpabi_mix__v_1
   RTS

main@run4:
   SUBROUTINE

   STA   z5
   LDA   __zpabi_mix__v_0
   STA   z5+1
   LDA   __zpabi_mix__v_1
   STA   z5+2
   LDA   __zpabi_mix__v_2
   STA   z5+3
   RTS

main@run2:
   SUBROUTINE

   LDA   z5
   STA   __zpabi_mix__v_3
   LDA   z5+1
   STA   __zpabi_mix__v_0
   LDA   z5+2
   STA   __zpabi_mix__v_1
   LDA   z5+3
   STA   __zpabi_mix__v_2
   LDA   __zpabi_mix__v_3
   RTS

acc:
   DS.B  2

ga6:
   DC.W  $FFFF
   DC.W  $00FF
   DC.W  $0002
   DC.W  $0080

gs7:
   DS.B  2
   DC.W  $000B

__zp_init:
   SUBROUTINE

   LDA   #$00
   STA   g2
   STA   g3+1
   STA   z4+1
   STA   z5+1
   STA   z5+2
   STA   z5+3
   LDA   #$FF
   STA   g1
   LDA   #$02
   STA   g3
   LDA   #$7F
   STA   z4
   LDA   #$80
   STA   z5
   LDA   #<g1
   STA   gp8
   LDA   #>g1
   STA   gp8+1
   RTS
