SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
z4	EQU	$26
g3	EQU	$28
g2	EQU	$29
__local_main__0	EQU	$80
__zpabi_f14__p16_0	EQU	$80
__zpabi_f14__p16_1	EQU	$81
__local_main__2	EQU	$82
__zpabi_f14__p16_2	EQU	$82
__local_main__3	EQU	$83
__zpabi_f14__p16_3	EQU	$83
__local_main__l22	EQU	$84
__zpabi_f14__p17	EQU	$84
__local_main__4	EQU	$85
__zpabi_f14__pp18_0	EQU	$85
__zpabi_f14__pp18_1	EQU	$86
__zpabi_f8__p10_0	EQU	$87
__zpabi_f8__p10_1	EQU	$88
__zpabi_f8__p10_2	EQU	$89
__zpabi_f8__p10_3	EQU	$8A
__zpabi_mix__v_0	EQU	$8B
__zpabi_mix__v_1	EQU	$8C
__zpabi_mix__v_2	EQU	$8D
__zpabi_mix__v_3	EQU	$8E
__local_f14__i20	EQU	$8F
__local_f14__0	EQU	$90
__local_f14__1	EQU	$91
__local_f14__2	EQU	$92
__local_f14__3	EQU	$93
__local_f14__4	EQU	$94
__local_f14__5	EQU	$95
__local_f14__6	EQU	$96
__local_f14__l19	EQU	$97
__local_f8__0	EQU	$99
__local_f8__1	EQU	$9A
__local_f8__2	EQU	$9B
__local_f8__3	EQU	$9C
__local_f8__4	EQU	$9D
__local_f8__5	EQU	$9E
__local_f8__7	EQU	$A0
__local_f8__i13	EQU	$A1
__local_f8__l12	EQU	$A2
__local_f8__8	EQU	$A3
__local_f8__9	EQU	$A4
__local_f8__10	EQU	$A5
__local_f8__11	EQU	$A6
__local_f8__12	EQU	$A7
__local_f8__13	EQU	$A8
__local_mix__0	EQU	$A9
__local_mix__1	EQU	$AA
__local_mix__2	EQU	$AB
__local_mix__3	EQU	$AC

; @zp-link-meta-begin
; def f14 params=__zpabi_f14__p15,__zpabi_f14__p16_0,__zpabi_f14__p16_1,__zpabi_f14__p16_2,__zpabi_f14__p16_3,__zpabi_f14__p17,__zpabi_f14__pp18_0,__zpabi_f14__pp18_1 locals=__local_f14__i20,__local_f14__0,__local_f14__1,__local_f14__2,__local_f14__3,__local_f14__4,__local_f14__5,__local_f14__6,__local_f14__l19,__local_f14__7 indirect=false in_cycle=false param_regs=A,,,,,,,
; def f8 params=__zpabi_f8__p9_0,__zpabi_f8__p9_1,__zpabi_f8__p10_0,__zpabi_f8__p10_1,__zpabi_f8__p10_2,__zpabi_f8__p10_3 locals=__local_f8__0,__local_f8__1,__local_f8__2,__local_f8__3,__local_f8__4,__local_f8__5,__local_f8__6,__local_f8__7,__local_f8__i13,__local_f8__l12,__local_f8__8,__local_f8__9,__local_f8__10,__local_f8__11,__local_f8__12,__local_f8__13 indirect=false in_cycle=false param_regs=A,X,,,,
; def fin0 params= locals=__local_fin0__0,__local_fin0__1,__local_fin0__2,__local_fin0__3 indirect=false in_cycle=false
; def fin1 params= locals=__local_fin1__0 indirect=false in_cycle=false
; def fin2 params= locals=__local_fin2__0,__local_fin2__1,__local_fin2__2,__local_fin2__3 indirect=false in_cycle=false
; def main params= locals=__local_main__0,__local_main__1,__local_main__2,__local_main__3,__local_main__l22,__local_main__4 indirect=false in_cycle=false
; def mix params=__zpabi_mix__v_0,__zpabi_mix__v_1,__zpabi_mix__v_2,__zpabi_mix__v_3 locals=__local_mix__0,__local_mix__1,__local_mix__2,__local_mix__3 indirect=false in_cycle=false
; call f14 -> f8
; call f14 -> mix
; call f8 -> mix
; call fin0 -> mix
; call fin1 -> mix
; call fin2 -> mix
; call main -> f8
; call main -> fin0
; call main -> fin1
; call main -> fin2
; call main -> mix
; item mix section=.text local=false zero=false
; item fin0 section=.text local=false zero=false
; item fin0@run4 section=.text local=true zero=false
; item fin0@run0 section=.text local=true zero=false
; item fin1 section=.text local=false zero=false
; item fin1@run2 section=.text local=true zero=false
; item fin1@run1 section=.text local=true zero=false
; item fin2 section=.text local=false zero=false
; item fin2@run5 section=.text local=true zero=false
; item f8 section=.text local=false zero=false
; item f14 section=.text local=false zero=false
; item main section=.text local=false zero=false
; item main@run3 section=.text local=true zero=false
; item acc section=.bss local=false zero=true
; item g1 section=.data local=false zero=false
; item ga5 section=.data local=false zero=false
; item gs6 section=.data local=false zero=false
; item gp7 section=.data local=false zero=false
; item __zp_init section=.text local=false zero=false
; zp z4 size=2 at=auto def=true init=$03,$00
; zp g3 size=1 at=auto def=true init=$80 weight=6
; zp g2 size=1 at=auto def=true init=$00 weight=4
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

   LDA   g1
   STA   __zpabi_mix__v_0
   LDA   g1+1
   STA   __zpabi_mix__v_1
   LDA   g1+2
   STA   __zpabi_mix__v_2
   LDA   g1+3
   JSR   fin0@run4
   LDA   g2
   JSR   fin0@run0
   LDA   g3
   JSR   fin0@run0
   LDA   z4+1
   BMI   .sx_neg@0
   LDA   #$00
   BEQ   .sx_done@1
.sx_neg@0:
   LDA   #$FF
.sx_done@1:
   STA   __zpabi_mix__v_2
   STA   __zpabi_mix__v_3
   LDA   z4
   STA   __zpabi_mix__v_0
   LDA   z4+1
   STA   __zpabi_mix__v_1
   JMP   mix

fin0@run4:
   SUBROUTINE

   STA   __zpabi_mix__v_3
   JMP   mix

fin0@run0:
   SUBROUTINE

   STA   __zpabi_mix__v_0
   LDA   #$00
   STA   __zpabi_mix__v_1
   STA   __zpabi_mix__v_2
   STA   __zpabi_mix__v_3
   JMP   mix

fin1:
   SUBROUTINE

   LDA   ga5
   BMI   .sx_neg@2
   LDA   #$00
   BEQ   .sx_done@3
.sx_neg@2:
   LDA   #$FF
.sx_done@3:
   JSR   fin1@run1
   LDA   ga5
   JSR   fin1@run2
   LDA   ga5+1
   BMI   .sx_neg@4
   LDA   #$00
   BEQ   .sx_done@5
.sx_neg@4:
   LDA   #$FF
.sx_done@5:
   JSR   fin1@run1
   LDA   ga5+1
   JSR   fin1@run2
   LDA   ga5+2
   BMI   .sx_neg@6
   LDA   #$00
   BEQ   .sx_done@7
.sx_neg@6:
   LDA   #$FF
.sx_done@7:
   JSR   fin1@run1
   LDA   ga5+2
   JSR   fin1@run2
   LDA   ga5+3
   BMI   .sx_neg@8
   LDA   #$00
   BEQ   .sx_done@9
.sx_neg@8:
   LDA   #$FF
.sx_done@9:
   JSR   fin1@run1
   LDA   ga5+3
   STA   __zpabi_mix__v_0
   JMP   mix

fin1@run2:
   SUBROUTINE

   STA   __zpabi_mix__v_0
   JMP   mix

fin1@run1:
   SUBROUTINE

   STA   __zpabi_mix__v_1
   STA   __zpabi_mix__v_2
   STA   __zpabi_mix__v_3
   RTS

fin2:
   SUBROUTINE

   LDA   gs6+1
   JSR   fin0@run0
   LDA   gs6+2
   STA   __zpabi_mix__v_0
   JSR   fin2@run5
   STA   __zpabi_mix__v_2
   LDA   gs6+5
   STA   __zpabi_mix__v_3
   JMP   mix

fin2@run5:
   SUBROUTINE

   LDA   gs6+3
   STA   __zpabi_mix__v_1
   LDA   gs6+4
   RTS

f8:
   SUBROUTINE

   LDA   __zpabi_f8__p10_0
   AND   #$07
   CMP   #$01
   BEQ   .case@1
   CMP   #$03
   BNE   .lb_skip@2
   JMP   .switch@0_break
.lb_skip@2:
   CMP   #$07
   BNE   .lb_skip@1
   JMP   .case@3
.lb_skip@1:
   JMP   .default@8
.case@1:
   LDA   g3
   JSR   fin0@run0
   LDA   gs6+2
   STA   __local_f8__13
   LDA   gs6+3
   STA   __local_f8__12
   LDA   gs6+4
   STA   __local_f8__11
   LDA   gs6+5
   STA   __local_f8__10
   LDA   gs6+6
   STA   __local_f8__9
   LDA   gs6+7
   STA   __local_f8__8
   LDA   gs6+8
   STA   __local_f8__l12
   LDA   gs6+9
   STA   __local_f8__i13
   LDA   __local_f8__13
   ASL
   STA   __local_f8__0
   LDA   __local_f8__12
   ROL
   STA   __local_f8__1
   LDA   __local_f8__11
   ROL
   STA   __local_f8__2
   LDA   __local_f8__10
   ROL
   STA   __local_f8__3
   LDA   __local_f8__9
   ROL
   STA   __local_f8__4
   LDA   __local_f8__8
   ROL
   STA   __local_f8__5
   LDA   __local_f8__l12
   ROL
   STA   __zpabi_mix__v_1
   LDA   __local_f8__i13
   ROL
   STA   __local_f8__7
   LDA   __local_f8__0
   CLC
   ADC   __local_f8__13
   STA   gs6+2
   LDA   __local_f8__1
   ADC   __local_f8__12
   STA   gs6+3
   LDA   __local_f8__2
   ADC   __local_f8__11
   STA   gs6+4
   LDA   __local_f8__3
   ADC   __local_f8__10
   STA   gs6+5
   LDA   __local_f8__4
   ADC   __local_f8__9
   STA   gs6+6
   LDA   __local_f8__5
   ADC   __local_f8__8
   STA   gs6+7
   LDA   __zpabi_mix__v_1
   ADC   __local_f8__l12
   STA   gs6+8
   LDA   __local_f8__7
   ADC   __local_f8__i13
   STA   gs6+9
   JMP   .switch@0_break
.case@3:
   LDA   #$00
   STA   __local_f8__l12
   STA   __local_f8__i13
.loop@4_start:
   LDA   __local_f8__i13
   CMP   #$02
   BCC   .lb_skip@0
   JMP   .loop@4_break
.lb_skip@0:
   ASL   __local_f8__l12
   LDA   __local_f8__l12
   CLC
   ADC   g1
   STA   __local_f8__7
   LDA   #$00
   ADC   g1+1
   STA   __zpabi_mix__v_1
   LDA   #$00
   ADC   g1+2
   STA   __local_f8__5
   LDA   #$00
   ADC   g1+3
   STA   __local_f8__4
   LDA   #$00
   ADC   g1+4
   STA   __local_f8__3
   LDA   #$00
   ADC   g1+5
   STA   __local_f8__2
   LDA   #$00
   ADC   g1+6
   STA   __local_f8__1
   LDA   #$00
   ADC   g1+7
   STA   __local_f8__0
   LDA   __local_f8__7
   ORA   __zpabi_mix__v_1
   ORA   __local_f8__5
   ORA   __local_f8__4
   ORA   __local_f8__3
   ORA   __local_f8__2
   ORA   __local_f8__1
   ORA   __local_f8__0
   BEQ   .if_end@0
   LDA   gs6+1
   SEC
   SBC   #$81
   STA   gs6+1
   RTS
.if_end@0:
   LDA   #$FF
   SEC
   SBC   __local_f8__i13
   AND   #$07
   BEQ   .switch@5_break
   LDA   __local_f8__l12
   ASL
   LDA   #$00
   ROL
   STA   __local_f8__l12
.switch@5_break:
   INC   __local_f8__i13
   JMP   .loop@4_start
.loop@4_break:
   LDA   gs6+1
   STA   __local_f8__0
   SBC   #$01
   STA   gs6+1
   LDA   __local_f8__0
   STA   g1
   LDA   #$00
   STA   g1+1
   STA   g1+2
   STA   g1+3
   STA   g1+4
   STA   g1+5
   STA   g1+6
   STA   g1+7
.default@8:
   LDA   gs6+2
   STA   __local_f8__7
   JSR   fin2@run5
   STA   __local_f8__5
   LDA   gs6+5
   STA   __local_f8__4
   LDA   gs6+6
   STA   __local_f8__3
   LDA   gs6+7
   STA   __local_f8__2
   LDA   gs6+8
   STA   __local_f8__1
   LDA   gs6+9
   STA   __local_f8__0
   LDA   __local_f8__7
   SEC
   SBC   #$01
   STA   gs6+2
   LDA   __zpabi_mix__v_1
   SBC   #$00
   STA   gs6+3
   LDA   __local_f8__5
   SBC   #$00
   STA   gs6+4
   LDA   __local_f8__4
   SBC   #$00
   STA   gs6+5
   LDA   __local_f8__3
   SBC   #$00
   STA   gs6+6
   LDA   __local_f8__2
   SBC   #$00
   STA   gs6+7
   LDA   __local_f8__1
   SBC   #$00
   STA   gs6+8
   LDA   __local_f8__0
   SBC   #$00
   STA   gs6+9
   LDA   #$00
   BPL   .sx_done@11
   LDA   #$FF
.sx_done@11:
   STA   __local_f8__1
   STA   __local_f8__0
   LDA   g3
   ORA   __local_f8__7
   STA   __zpabi_mix__v_0
   LDA   __local_f8__1
   ORA   __local_f8__5
   STA   __zpabi_mix__v_2
   LDA   __local_f8__0
   ORA   __local_f8__4
   JSR   fin0@run4
.switch@0_break:
   LDA   __zpabi_f8__p10_0
   RTS

f14:
   SUBROUTINE

   LDA   #$00
   STA   __local_f14__l19
   LDA   #$FF
   STA   gs6+1
   STA   g3
   LDY   #$00
   LDA   (__zpabi_f14__pp18_0),Y
   ORA   #$03
   STA   (__zpabi_f14__pp18_0),Y
   LDA   #$03
   STA   __local_f14__i20
.f14@rot@0:
   LDA   #$FF
   STA   __zpabi_mix__v_0
   LDA   #$7F
   STA   __zpabi_mix__v_1
   LDA   #$00
   STA   __zpabi_mix__v_2
   STA   __zpabi_mix__v_3
   JSR   mix
   DEC   __local_f14__i20
   BNE   .f14@rot@0
   LDA   gs6+2
   LDX   gs6+3
   STX   __local_f14__6
   LDX   gs6+4
   STX   __local_f14__5
   LDX   gs6+5
   STX   __local_f14__4
   LDX   gs6+6
   STX   __local_f14__3
   LDX   gs6+7
   STX   __local_f14__2
   LDX   gs6+8
   STX   __local_f14__1
   LDX   gs6+9
   STX   __local_f14__0
   CLC
   ADC   #$03
   STA   __local_f14__i20
   BCC   .carry_inc@0
   INC   __local_f14__6
   BNE   .carry_inc@0
   INC   __local_f14__5
   BNE   .carry_inc@0
   INC   __local_f14__4
   BNE   .carry_inc@0
   INC   __local_f14__3
   BNE   .carry_inc@0
   INC   __local_f14__2
   BNE   .carry_inc@0
   INC   __local_f14__1
   BNE   .carry_inc@0
   INC   __local_f14__0
.carry_inc@0:
   STA   gs6+2
   LDA   __local_f14__6
   STA   gs6+3
   STA   g1+1
   LDA   __local_f14__5
   STA   gs6+4
   STA   g1+2
   LDA   __local_f14__4
   STA   gs6+5
   STA   g1+3
   LDA   __local_f14__3
   STA   gs6+6
   STA   g1+4
   LDA   __local_f14__2
   STA   gs6+7
   STA   g1+5
   LDA   __local_f14__1
   STA   gs6+8
   STA   g1+6
   LDA   __local_f14__i20
   STA   g1
   LDA   __local_f14__0
   STA   gs6+9
   STA   g1+7
   LDA   __local_f14__l19
   STA   __local_f14__i20
   BMI   .sx_neg@12
   LDA   #$00
   BEQ   .sx_done@13
.sx_neg@12:
   LDA   #$FF
.sx_done@13:
   STA   __zpabi_f8__p10_1
   STA   __zpabi_f8__p10_2
   STA   __zpabi_f8__p10_3
   LDA   __local_f14__i20
   STA   __zpabi_f8__p10_0
   LDX   __zpabi_f14__p16_1
   LDA   __zpabi_f14__p16_0
   JSR   f8
   STA   __local_f14__i20
   ORA   #$00
   BMI   .sx_neg@14
   LDA   #$00
   BEQ   .sx_done@15
.sx_neg@14:
   LDA   #$FF
.sx_done@15:
   JSR   fin1@run1
   LDA   __local_f14__i20
   JSR   fin1@run2
   LDX   #$80
   LDA   #$00
   RTS

main:
   SUBROUTINE

   LDA   #$00
   STA   __local_main__l22
   STA   __local_main__4
   JSR   main@run3
   STA   __local_main__3
   LDY   #$01
   LDA   (DPTR),Y
   STA   __local_main__2
   LSR
   LDA   __local_main__3
   ROR
   STA   __zpabi_mix__v_1
   LDA   #$00
   ROR
   SEC
   SBC   __local_main__3
   STA   __local_main__0
   LDA   __zpabi_mix__v_1
   SBC   __local_main__2
   TAX
   LDA   #$7F
   STA   __zpabi_f8__p10_0
   LDA   #$00
   STA   __zpabi_f8__p10_1
   STA   __zpabi_f8__p10_2
   STA   __zpabi_f8__p10_3
   LDA   __local_main__0
   JSR   f8
   LDA   g2
   AND   #$03
   STA   __local_main__0
   STA   g2
   LDA   #$FF
   ORA   __local_main__0
   ORA   #$7F
   BNE   .lb_skip@3
   JMP   .if_else@2
.lb_skip@3:
   LDA   ga5
   AND   #$81
   STA   ga5
   STA   gs6+2
   BMI   .sx_neg@16
   LDA   #$00
   BEQ   .sx_done@17
.sx_neg@16:
   LDA   #$FF
.sx_done@17:
   STA   gs6+3
   STA   gs6+4
   STA   gs6+5
   STA   gs6+6
   STA   gs6+7
   STA   gs6+8
   STA   gs6+9
   LDA   __local_main__l22
   STA   __local_main__0
   LDA   __local_main__4
   STA   __zpabi_mix__v_1
   LDX   #$03
.shift_loop@18:
   LDA   __zpabi_mix__v_1
   ASL
   ROR   __zpabi_mix__v_1
   ROR   __local_main__0
   DEX
   BNE   .shift_loop@18
   LDA   __zpabi_mix__v_1
   BMI   .sx_neg@20
   LDA   #$00
   BEQ   .sx_done@21
.sx_neg@20:
   LDA   #$FF
.sx_done@21:
   STA   __zpabi_mix__v_2
   STA   __zpabi_mix__v_3
   LDA   __local_main__0
   JSR   fin1@run2
   LDA   ga5
   STA   __local_main__0
   SEC
   SBC   #$01
   STA   ga5
   LDA   __local_main__0
   BMI   .sx_neg@22
   LDA   #$00
   BEQ   .sx_done@23
.sx_neg@22:
   LDA   #$FF
.sx_done@23:
   JSR   fin1@run1
   LDA   __local_main__0
   JSR   fin1@run2
   JMP   .if_end@1
.if_else@2:
   LDA   #$FF
   STA   ga5
   STA   z4
   BMI   .sx_neg@24
   LDA   #$00
.sx_neg@24:
   STA   z4+1
.if_end@1:
   JSR   main@run3
   STA   __zpabi_mix__v_1
   LDY   #$01
   LDA   (DPTR),Y
   STA   __local_main__0
   LDA   __zpabi_mix__v_1
   CLC
   ADC   #$FC
   STA   __zpabi_mix__v_0
   LDA   __local_main__0
   ADC   #$FF
   STA   __zpabi_mix__v_1
   LDA   #$00
   STA   __zpabi_mix__v_2
   JSR   fin0@run4
   LDA   g3
   CLC
   ADC   #$5A
   STA   g3
   STA   g2
   JSR   fin0
   JSR   fin1
   JSR   fin2
   LDA   acc+1
   AND   #$7F
   TAX
   LDA   acc
   RTS

main@run3:
   SUBROUTINE

   LDA   gp7
   STA   DPTR
   LDA   gp7+1
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   RTS

acc:
   DS.B  2

g1:
   DC.L  $00008000
   DC.L  $00000000

ga5:
   DC.B  $00, $0F, $00, $02

gs6:
   DS.B  1
   DC.B  $FF
   DC.L  $80000000
   DC.L  $00000000

gp7:
   DC.W  z4

__zp_init:
   SUBROUTINE

   LDA   #$00
   STA   g2
   STA   z4+1
   LDA   #$80
   STA   g3
   LDA   #$03
   STA   z4
   RTS
