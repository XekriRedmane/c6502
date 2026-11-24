SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
z4	EQU	$26
z5	EQU	$28
g1	EQU	$2A
g2	EQU	$2C
__zpabi_f14__p16	EQU	$80
__zpabi_f14__p17_0	EQU	$81
__zpabi_f14__p17_1	EQU	$82
__zpabi_f8__p10_0	EQU	$83
__zpabi_f8__p10_1	EQU	$84
__zpabi_mix__v_0	EQU	$85
__zpabi_mix__v_1	EQU	$86
__zpabi_mix__v_2	EQU	$87
__zpabi_mix__v_3	EQU	$88
__local_f14__l19	EQU	$89
__local_f14__0	EQU	$8A
__local_main__1	EQU	$8A
__local_f14__1	EQU	$8B
__local_main__2	EQU	$8B
__local_f14__2	EQU	$8C
__local_main__3	EQU	$8C
__local_f14__3	EQU	$8D
__local_main__4	EQU	$8D
__local_f14__4	EQU	$8E
__local_main__5	EQU	$8E
__local_f14__5	EQU	$8F
__local_main__6	EQU	$8F
__local_f14__6	EQU	$90
__local_main__7	EQU	$90
__local_f14__i21	EQU	$91
__local_main__8	EQU	$91
__local_main__w25	EQU	$93
__local_f8__0	EQU	$94
__local_main__l22	EQU	$94
__local_f8__p9_1	EQU	$96
__local_f8__p9_0	EQU	$97
__local_f8__4	EQU	$9A
__local_mix__0	EQU	$9B
__local_mix__1	EQU	$9C
__local_mix__2	EQU	$9D
__local_mix__3	EQU	$9E

; @zp-link-meta-begin
; def f14 params=__zpabi_f14__p15_0,__zpabi_f14__p15_1,__zpabi_f14__p16,__zpabi_f14__p17_0,__zpabi_f14__p17_1 locals=__local_f14__l19,__local_f14__0,__local_f14__1,__local_f14__2,__local_f14__3,__local_f14__4,__local_f14__5,__local_f14__6,__local_f14__i21,__local_f14__7,__local_f14__8 indirect=false in_cycle=false param_regs=A,X,,,
; def f8 params=__zpabi_f8__p9_0,__zpabi_f8__p9_1,__zpabi_f8__p10_0,__zpabi_f8__p10_1 locals=__local_f8__0,__local_f8__1,__local_f8__p9_1,__local_f8__p9_0,__local_f8__2,__local_f8__3,__local_f8__4 indirect=false in_cycle=false param_regs=A,X,,
; def fin0 params= locals=__local_fin0__0,__local_fin0__1 indirect=false in_cycle=false
; def fin1 params= locals=__local_fin1__0,__local_fin1__1,__local_fin1__2,__local_fin1__3 indirect=false in_cycle=false
; def fin2 params= locals=__local_fin2__0,__local_fin2__1,__local_fin2__2,__local_fin2__3 indirect=false in_cycle=false
; def main params= locals=__local_main__0,__local_main__1,__local_main__2,__local_main__3,__local_main__4,__local_main__5,__local_main__6,__local_main__7,__local_main__8,__local_main__9,__local_main__w25,__local_main__l22 indirect=false in_cycle=false
; def mix params=__zpabi_mix__v_0,__zpabi_mix__v_1,__zpabi_mix__v_2,__zpabi_mix__v_3 locals=__local_mix__0,__local_mix__1,__local_mix__2,__local_mix__3 indirect=false in_cycle=false
; call f14 -> f8
; call f8 -> mix
; call fin0 -> mix
; call fin1 -> mix
; call fin2 -> mix
; call main -> fin0
; call main -> fin1
; call main -> fin2
; call main -> mix
; item mix section=.text local=false zero=false
; item fin0 section=.text local=false zero=false
; item fin0@run1 section=.text local=true zero=false
; item fin0@run0 section=.text local=true zero=false
; item fin1 section=.text local=false zero=false
; item fin2 section=.text local=false zero=false
; item fin2@run3 section=.text local=true zero=false
; item f8 section=.text local=false zero=false
; item f8@run2 section=.text local=true zero=false
; item f14 section=.text local=false zero=false
; item main section=.text local=false zero=false
; item main@run5 section=.text local=true zero=false
; item main@run4 section=.text local=true zero=false
; item acc section=.bss local=false zero=true
; item g3 section=.data local=false zero=false
; item ga6 section=.data local=false zero=false
; item gs7 section=.data local=false zero=false
; item @9.s20 section=.data local=true zero=false
; item __zp_init section=.text local=false zero=false
; zp z4 size=2 at=auto def=true init=$00,$00
; zp z5 size=2 at=auto def=true init=$0A,$00
; zp g1 size=2 at=auto def=true init=$FA,$6F weight=28
; zp g2 size=4 at=auto def=true init=$80,$00,$00,$00 weight=12
; runtime hargs_size=32 hargs_need=17
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
   JSR   fin0@run1
   LDA   g3
   STA   __zpabi_mix__v_0
   LDA   g3+1
   STA   __zpabi_mix__v_1
   LDA   g3+2
   STA   __zpabi_mix__v_2
   LDA   g3+3
   JSR   fin0@run0
   LDA   z4+1
   BMI   .sx_neg@2
   LDA   #$00
   BEQ   .sx_done@3
.sx_neg@2:
   LDA   #$FF
.sx_done@3:
   STA   __zpabi_mix__v_2
   STA   __zpabi_mix__v_3
   LDA   z4
   STA   __zpabi_mix__v_0
   LDA   z4+1
   STA   __zpabi_mix__v_1
   JMP   mix

fin0@run1:
   SUBROUTINE

   LDA   g2
   STA   __zpabi_mix__v_0
   LDA   g2+1
   STA   __zpabi_mix__v_1
   LDA   g2+2
   STA   __zpabi_mix__v_2
   LDA   g2+3
   JMP   fin0@run0

fin0@run0:
   SUBROUTINE

   STA   __zpabi_mix__v_3
   JMP   mix

fin1:
   SUBROUTINE

   LDA   z5+1
   BMI   .sx_neg@4
   LDA   #$00
   BEQ   .sx_done@5
.sx_neg@4:
   LDA   #$FF
.sx_done@5:
   STA   __zpabi_mix__v_2
   STA   __zpabi_mix__v_3
   LDA   z5
   STA   __zpabi_mix__v_0
   LDA   z5+1
   STA   __zpabi_mix__v_1
   JSR   mix
   LDA   ga6
   STA   __zpabi_mix__v_0
   LDA   ga6+1
   STA   __zpabi_mix__v_1
   LDA   ga6+2
   STA   __zpabi_mix__v_2
   LDA   ga6+3
   JSR   fin0@run0
   LDA   ga6+8
   STA   __zpabi_mix__v_0
   LDA   ga6+9
   STA   __zpabi_mix__v_1
   LDA   ga6+10
   STA   __zpabi_mix__v_2
   LDA   ga6+11
   JSR   fin0@run0
   LDA   ga6+16
   STA   __zpabi_mix__v_0
   LDA   ga6+17
   STA   __zpabi_mix__v_1
   LDA   ga6+18
   STA   __zpabi_mix__v_2
   LDA   ga6+19
   STA   __zpabi_mix__v_3
   JMP   mix

fin2:
   SUBROUTINE

   LDA   ga6+24
   STA   __zpabi_mix__v_0
   LDA   ga6+25
   STA   __zpabi_mix__v_1
   LDA   ga6+26
   STA   __zpabi_mix__v_2
   LDA   ga6+27
   JSR   fin0@run0
   LDA   gs7+1
   STA   __zpabi_mix__v_0
   LDA   gs7+2
   STA   __zpabi_mix__v_1
   JSR   fin2@run3
   LDA   gs7+3
   BMI   .sx_neg@6
   LDA   #$00
   BEQ   .sx_done@7
.sx_neg@6:
   LDA   #$FF
.sx_done@7:
   STA   __zpabi_mix__v_1
   STA   __zpabi_mix__v_2
   STA   __zpabi_mix__v_3
   LDA   gs7+3
   STA   __zpabi_mix__v_0
   JMP   mix

fin2@run3:
   SUBROUTINE

   LDA   #$00
   STA   __zpabi_mix__v_2
   JMP   fin0@run0

f8:
   SUBROUTINE

   STA   __local_f8__p9_0
   STX   __local_f8__p9_1
   LDA   gs7+3
   STA   __local_f8__0
   BMI   .sx_neg@8
   LDA   #$00
   BEQ   .sx_done@9
.sx_neg@8:
   LDA   #$FF
.sx_done@9:
   ASL   __local_f8__0
   ROL
   CMP   __local_f8__p9_1
   BNE   .thread@0
   LDA   __local_f8__0
   CMP   __local_f8__p9_0
   BEQ   .cmp_true@10
.thread@0:
   LDA   #$00
   BEQ   .cmp_end@11
.cmp_true@10:
   LDA   #$01
.cmp_end@11:
   STA   __local_f8__4
   LDA   #$00
   BPL   .sx_done@14
   LDA   #$FF
.sx_done@14:
   LDX   __local_f8__4
   STX   ga6+24
   LDX   #$00
   STX   ga6+25
   STA   ga6+26
   STA   ga6+27
   STA   ga6+28
   STA   ga6+29
   STA   ga6+30
   AND   #$07
   STA   ga6+31
   LDA   #$01
   STA   __zpabi_mix__v_0
   JSR   f8@run2
   LDA   g1
   SEC
   SBC   #$01
   STA   g1
   BCS   .carry_inc@0
   DEC   g1+1
.carry_inc@0:
   LDA   gs7+3
   BMI   .sx_neg@15
   LDA   #$00
   BEQ   .sx_done@16
.sx_neg@15:
   LDA   #$FF
.sx_done@16:
   STA   __local_f8__0
   LDA   gs7+3
   CLC
   SBC   gs7+1
   LDA   __local_f8__0
   SBC   gs7+2
   LDA   #$00
   ROL
   LDX   #$00
   RTS

f8@run2:
   SUBROUTINE

   LDA   #$00
   STA   __zpabi_mix__v_1
   STA   __zpabi_mix__v_2
   JMP   fin0@run0

f14:
   SUBROUTINE

   LDA   #$7F
   STA   __local_f14__l19
   LDA   #$00
   STA   __local_f14__0
   LDA   #$03
   STA   __local_f14__i21
.loop@0_start:
   LDA   __local_f14__i21
   BNE   .lb_skip@0
   JMP   .loop@0_break
.lb_skip@0:
   LDA   g2
   LDX   g2+1
   STX   __local_f14__1
   LDX   g2+2
   STX   __local_f14__0
   LDX   g2+3
   STX   __local_f14__l19
   ORA   __local_f14__1
   ORA   __local_f14__0
   ORA   __local_f14__l19
   BEQ   .lnot_true@17
   LDA   #$00
   BEQ   .lnot_end@18
.lnot_true@17:
   LDA   #$01
.lnot_end@18:
   AND   #$07
   CMP   #$05
   BEQ   .case@2
   CMP   #$06
   BEQ   .case@3
   BNE   .switch@1_break
.case@2:
.case@3:
   LDA   g1
   SEC
   SBC   #$03
   STA   g1
   BCS   .carry_inc@1
   DEC   g1+1
.carry_inc@1:
.switch@1_break:
   LDA   ga6+8
   STA   HARGS
   LDA   ga6+9
   STA   HARGS+1
   LDA   ga6+10
   STA   HARGS+2
   LDA   ga6+11
   STA   HARGS+3
   LDA   ga6+12
   STA   HARGS+4
   LDA   ga6+13
   STA   HARGS+5
   LDA   ga6+14
   STA   HARGS+6
   LDA   ga6+15
   STA   HARGS+7
   LDA   #$04
   STA   HARGS+8
   JSR   asl64
   LDA   HARGS+9
   STA   ga6+8
   LDA   HARGS+10
   STA   ga6+9
   LDA   HARGS+11
   STA   ga6+10
   LDA   HARGS+12
   STA   ga6+11
   LDA   HARGS+13
   STA   ga6+12
   LDA   HARGS+14
   STA   ga6+13
   LDA   HARGS+15
   STA   ga6+14
   LDA   HARGS+16
   AND   #$07
   STA   ga6+15
   DEC   __local_f14__i21
   JMP   .loop@0_start
.loop@0_break:
   LDA   ga6+24
   LDX   ga6+25
   STX   __local_f14__i21
   LDX   ga6+26
   STX   __local_f14__6
   LDX   ga6+27
   STX   __local_f14__5
   LDX   ga6+28
   STX   __local_f14__4
   LDX   ga6+29
   STX   __local_f14__3
   LDX   ga6+30
   STX   __local_f14__2
   LDX   ga6+31
   STX   __local_f14__1
   CLC
   ADC   gs7+1
   STA   __local_f14__0
   LDA   __local_f14__i21
   ADC   gs7+2
   BCC   .carry_inc@2
   INC   __local_f14__6
   BNE   .carry_inc@2
   INC   __local_f14__5
   BNE   .carry_inc@2
   INC   __local_f14__4
   BNE   .carry_inc@2
   INC   __local_f14__3
   BNE   .carry_inc@2
   INC   __local_f14__2
   BNE   .carry_inc@2
   INC   __local_f14__1
.carry_inc@2:
   STA   ga6+9
   LDA   __local_f14__0
   STA   ga6+8
   LDA   __local_f14__6
   STA   ga6+10
   LDA   __local_f14__5
   STA   ga6+11
   LDA   __local_f14__4
   STA   ga6+12
   LDA   __local_f14__3
   STA   ga6+13
   LDA   __local_f14__2
   STA   ga6+14
   LDA   __local_f14__1
   AND   #$07
   STA   ga6+15
   LDA   #$01
   STA   __zpabi_f8__p10_0
   LDA   #$00
   STA   __zpabi_f8__p10_1
   LDX   #$00
   LDA   z5
   JSR   f8
   LDA   g2
   STA   __zpabi_f8__p10_0
   LDA   g2+1
   LDA   g2+2
   LDA   g2+3
   LDA   #$00
   STA   __zpabi_f8__p10_1
   LDX   #$00
   LDA   #$FF
   JSR   f8
   LDA   @9.s20
   STA   __local_f14__3
   LDA   g3
   LDX   g3+1
   STX   __local_f14__2
   LDX   g3+2
   STX   __local_f14__1
   LDX   g3+3
   STX   __local_f14__0
   CLC
   ADC   #$FF
   STA   __local_f14__l19
   BCC   .carry_inc@3
   INC   __local_f14__2
   BNE   .carry_inc@3
   INC   __local_f14__1
   BNE   .carry_inc@3
   INC   __local_f14__0
.carry_inc@3:
   STA   g3
   LDA   __local_f14__2
   STA   g3+1
   LDA   __local_f14__1
   STA   g3+2
   LDA   __local_f14__0
   STA   g3+3
   LDA   __local_f14__3
   CLC
   ADC   __local_f14__l19
   STA   @9.s20
   LDA   __zpabi_f14__p16
   RTS

main:
   SUBROUTINE

   LDA   #$FF
   STA   __local_main__l22
   LDA   g2
   LDX   g2+1
   STX   __local_main__2
   LDX   g2+2
   STX   __local_main__1
   LDX   g2+3
   STX   __zpabi_mix__v_1
   ORA   __local_main__2
   ORA   __local_main__1
   ORA   __zpabi_mix__v_1
   BNE   .lb_skip@3
   JMP   .if_end@4
.lb_skip@3:
   LDA   #$00
   STA   __local_main__w25
.loop@4_start:
   AND   #$03
   ASL
   ASL
   ASL
   TAX
   JSR   main@run4
   STA   __local_main__6
   LDA   ga6+2,X
   STA   __local_main__5
   LDA   ga6+3,X
   STA   __local_main__4
   LDA   ga6+4,X
   STA   __local_main__3
   LDA   ga6+5,X
   STA   __local_main__2
   LDA   ga6+6,X
   STA   __local_main__1
   LDA   ga6+7,X
   STA   __local_main__8
   INC   __zpabi_mix__v_1
   BNE   .inc_done@0
   INC   __local_main__6
   BNE   .inc_done@0
   INC   __local_main__5
   BNE   .inc_done@0
   INC   __local_main__4
   BNE   .inc_done@0
   INC   __local_main__3
   BNE   .inc_done@0
   INC   __local_main__2
   BNE   .inc_done@0
   INC   __local_main__1
   BNE   .inc_done@0
   INC   __local_main__8
.inc_done@0:
   LDA   __zpabi_mix__v_1
   STA   __local_main__7
   LDA   __local_main__8
   AND   #$07
   STA   __zpabi_mix__v_1
   LDA   __local_main__7
   STA   ga6,X
   LDA   __local_main__6
   STA   ga6+1,X
   LDA   __local_main__5
   STA   ga6+2,X
   LDA   __local_main__4
   STA   ga6+3,X
   LDA   __local_main__3
   STA   ga6+4,X
   LDA   __local_main__2
   STA   ga6+5,X
   LDA   __local_main__1
   JSR   main@run5
   LDA   #$FF
   EOR   __local_main__7
   STA   z4
   LDA   #$FF
   EOR   __local_main__6
   STA   z4+1
   LDA   g1
   ORA   #$01
   ORA   g1+1
   BNE   .main@jthread@0
   LDA   gs7+1
   ORA   gs7+2
   BNE   .lb_skip@2
   JMP   .if_end@7
.lb_skip@2:
.main@jthread@0:
   JSR   main@run4
   STA   __local_main__6
   LDA   ga6+2,X
   STA   __local_main__5
   LDA   ga6+3,X
   STA   __local_main__4
   LDA   ga6+4,X
   STA   __local_main__3
   LDA   ga6+5,X
   STA   __local_main__2
   LDA   ga6+6,X
   STA   __local_main__1
   LDA   ga6+7,X
   STA   __local_main__8
   INC   __zpabi_mix__v_1
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
   INC   __local_main__8
.inc_done@1:
   LDA   __zpabi_mix__v_1
   STA   __local_main__7
   LDA   __local_main__8
   AND   #$07
   STA   __zpabi_mix__v_1
   LDA   __local_main__7
   STA   ga6,X
   LDA   __local_main__6
   STA   ga6+1,X
   LDA   __local_main__5
   STA   ga6+2,X
   LDA   __local_main__4
   STA   ga6+3,X
   LDA   __local_main__3
   STA   ga6+4,X
   LDA   __local_main__2
   STA   ga6+5,X
   LDA   __local_main__1
   JSR   main@run5
   LDA   gs7+1
   STA   __local_main__1
   LDA   gs7+2
   STA   __zpabi_mix__v_1
   LDA   __local_main__1
   SEC
   SBC   #$01
   STA   gs7+1
   LDA   __zpabi_mix__v_1
   SBC   #$00
   STA   gs7+2
   LDA   __local_main__1
   EOR   #$FF
   STA   __zpabi_mix__v_0
   JSR   fin2@run3
   LDA   __local_main__w25
   STA   __zpabi_mix__v_0
   JSR   f8@run2
.if_end@7:
   INC   __local_main__w25
   LDA   __local_main__w25
   CMP   #$02
   BCS   .lb_skip@1
   JMP   .loop@4_start
.lb_skip@1:
.if_end@4:
   LDA   __local_main__l22
   EOR   #$03
   STA   __local_main__l22
   STA   g3
   LDA   #$00
   STA   g3+1
   STA   g3+2
   STA   g3+3
   JSR   fin0@run1
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

   STA   ga6+6,X
   LDA   __zpabi_mix__v_1
   STA   ga6+7,X
   RTS

main@run4:
   SUBROUTINE

   LDA   ga6,X
   STA   __zpabi_mix__v_1
   LDA   ga6+1,X
   RTS

acc:
   DS.B  2

g3:
   DC.L  $00000002

ga6:
   DC.L  $00000100
   DC.L  $00000000
   DC.L  $00000002
   DC.L  $00000000
   DC.L  $00000001
   DC.L  $00000000
   DC.L  $00008000
   DC.L  $00000000

gs7:
   DS.B  1
   DC.W  $7FFF
   DC.B  $02

@9.s20:
   DC.B  $FF

__zp_init:
   SUBROUTINE

   LDA   #$00
   STA   g2+1
   STA   g2+2
   STA   g2+3
   STA   z4
   STA   z4+1
   STA   z5+1
   LDA   #$FA
   STA   g1
   LDA   #$6F
   STA   g1+1
   LDA   #$80
   STA   g2
   LDA   #$0A
   STA   z5
   RTS
