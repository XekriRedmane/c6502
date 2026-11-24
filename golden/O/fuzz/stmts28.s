SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
g2	EQU	$26
gp7	EQU	$27
g1	EQU	$29
g3	EQU	$2D
__zpabi_f15__p17_0	EQU	$80
__zpabi_f15__p17_1	EQU	$81
__zpabi_f15__pp18_0	EQU	$82
__zpabi_f15__pp18_1	EQU	$83
__zpabi_f8__p9_0	EQU	$84
__zpabi_f8__p9_1	EQU	$85
__zpabi_f8__p9_2	EQU	$86
__zpabi_f8__p9_3	EQU	$87
__zpabi_f8__p9_4	EQU	$88
__zpabi_f8__p9_5	EQU	$89
__zpabi_f8__p9_6	EQU	$8A
__zpabi_f8__p9_7	EQU	$8B
__zpabi_f8__pp10_0	EQU	$8C
__zpabi_f8__pp10_1	EQU	$8D
__zpabi_mix__v_0	EQU	$8E
__zpabi_mix__v_1	EQU	$8F
__zpabi_mix__v_2	EQU	$90
__zpabi_mix__v_3	EQU	$91
__local_f22__w28	EQU	$92
__local_main__0	EQU	$92
__local_f22__0	EQU	$93
__local_main__1	EQU	$93
__local_f22__1	EQU	$94
__local_main__2	EQU	$94
__local_f22__3	EQU	$96
__local_main__l32	EQU	$96
__local_f22__4	EQU	$97
__local_f22__5	EQU	$98
__local_f22__6	EQU	$99
__local_f22__7	EQU	$9A
__local_f22__8	EQU	$9B
__local_f22__9	EQU	$9C
__local_f22__10	EQU	$9D
__local_f22__11	EQU	$9E
__local_f22__i27	EQU	$9F
__local_f22__l24	EQU	$A0
__local_f22__l26	EQU	$A1
__local_f22__l25	EQU	$A5
__local_f15__l19	EQU	$A9
__local_mix__0	EQU	$AD
__local_mix__1	EQU	$AE
__local_mix__2	EQU	$AF
__local_mix__3	EQU	$B0

; @zp-link-meta-begin
; def f15 params=__zpabi_f15__p16,__zpabi_f15__p17_0,__zpabi_f15__p17_1,__zpabi_f15__pp18_0,__zpabi_f15__pp18_1 locals=__local_f15__p16,__local_f15__0,__local_f15__l19,__local_f15__2 indirect=false in_cycle=false param_regs=A,,,,
; def f22 params=__zpabi_f22__pp23_0,__zpabi_f22__pp23_1 locals=__local_f22__w28,__local_f22__0,__local_f22__1,__local_f22__2,__local_f22__3,__local_f22__4,__local_f22__5,__local_f22__6,__local_f22__7,__local_f22__8,__local_f22__9,__local_f22__10,__local_f22__11,__local_f22__i27,__local_f22__l24,__local_f22__l26,__local_f22__13,__local_f22__14,__local_f22__15,__local_f22__l25,__local_f22__17 indirect=false in_cycle=false param_regs=A,X
; def f8 params=__zpabi_f8__p9_0,__zpabi_f8__p9_1,__zpabi_f8__p9_2,__zpabi_f8__p9_3,__zpabi_f8__p9_4,__zpabi_f8__p9_5,__zpabi_f8__p9_6,__zpabi_f8__p9_7,__zpabi_f8__pp10_0,__zpabi_f8__pp10_1 locals=__local_f8__0,__local_f8__1 indirect=false in_cycle=false
; def fin0 params= locals=__local_fin0__0,__local_fin0__1 indirect=false in_cycle=false
; def fin1 params= locals=__local_fin1__0,__local_fin1__1 indirect=false in_cycle=false
; def fin2 params= locals=__local_fin2__0,__local_fin2__1,__local_fin2__2,__local_fin2__3 indirect=false in_cycle=false
; def main params= locals=__local_main__0,__local_main__1,__local_main__2,__local_main__3,__local_main__l32,__local_main__5,__local_main__6,__local_main__7 indirect=false in_cycle=false
; def mix params=__zpabi_mix__v_0,__zpabi_mix__v_1,__zpabi_mix__v_2,__zpabi_mix__v_3 locals=__local_mix__0,__local_mix__1,__local_mix__2,__local_mix__3 indirect=false in_cycle=false
; call f15 -> f8
; call f22 -> f15
; call f22 -> f8
; call f22 -> mix
; call f8 -> mix
; call fin0 -> mix
; call fin1 -> mix
; call fin2 -> mix
; call main -> f15
; call main -> fin0
; call main -> fin1
; call main -> fin2
; call main -> mix
; item mix section=.text local=false zero=false
; item fin0 section=.text local=false zero=false
; item fin0@run0 section=.text local=true zero=false
; item fin1 section=.text local=false zero=false
; item fin1@run4 section=.text local=true zero=false
; item fin1@run1 section=.text local=true zero=false
; item fin2 section=.text local=false zero=false
; item f8 section=.text local=false zero=false
; item f15 section=.text local=false zero=false
; item f15@run3 section=.text local=true zero=false
; item f22 section=.text local=false zero=false
; item f22@run2 section=.text local=true zero=false
; item main section=.text local=false zero=false
; item acc section=.bss local=false zero=true
; item g4 section=.data local=false zero=false
; item ga5 section=.data local=false zero=false
; item gs6 section=.data local=false zero=false
; item __zp_init section=.text local=false zero=false
; zp g2 size=1 at=auto def=true init=$80 weight=93
; zp gp7 size=2 at=auto def=true init=<g1,>g1 weight=69
; zp g1 size=4 at=auto def=true init=$03,$00,$00,$00 weight=132
; zp g3 size=1 at=auto def=true init=$00 weight=5
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
   STA   __zpabi_mix__v_0
   LDA   g1+1
   STA   __zpabi_mix__v_1
   LDA   g1+2
   STA   __zpabi_mix__v_2
   LDA   g1+3
   STA   __zpabi_mix__v_3
   JSR   mix
   LDA   g2
   STA   __zpabi_mix__v_0
   JSR   fin0@run0
   LDA   g3
   STA   __zpabi_mix__v_0
   JSR   fin0@run0
   LDA   g4+1
   BMI   .sx_neg@0
   LDA   #$00
   BEQ   .sx_done@1
.sx_neg@0:
   LDA   #$FF
.sx_done@1:
   STA   __zpabi_mix__v_2
   STA   __zpabi_mix__v_3
   LDA   g4
   STA   __zpabi_mix__v_0
   LDA   g4+1
   STA   __zpabi_mix__v_1
   JMP   mix

fin0@run0:
   SUBROUTINE

   LDA   #$00
   STA   __zpabi_mix__v_1
   STA   __zpabi_mix__v_2
   STA   __zpabi_mix__v_3
   JMP   mix

fin1:
   SUBROUTINE

   LDA   ga5
   STA   __zpabi_mix__v_0
   LDA   ga5+1
   JSR   fin1@run1
   LDA   ga5+2
   STA   __zpabi_mix__v_0
   LDA   ga5+3
   JSR   fin1@run1
   LDA   ga5+4
   STA   __zpabi_mix__v_0
   LDA   ga5+5
   JSR   fin1@run1
   LDA   ga5+6
   STA   __zpabi_mix__v_0
   LDA   ga5+7
   JSR   fin1@run4
   JMP   mix

fin1@run4:
   SUBROUTINE

   STA   __zpabi_mix__v_1
   LDA   #$00
   STA   __zpabi_mix__v_2
   STA   __zpabi_mix__v_3
   RTS

fin1@run1:
   SUBROUTINE

   JSR   fin1@run4
   JMP   mix

fin2:
   SUBROUTINE

   LDA   gs6+1
   STA   __zpabi_mix__v_0
   LDA   gs6+2
   STA   __zpabi_mix__v_1
   LDA   gs6+3
   STA   __zpabi_mix__v_2
   LDA   #$00
   STA   __zpabi_mix__v_3
   JSR   mix
   LDA   gs6+4
   STA   __zpabi_mix__v_0
   LDA   gs6+5
   STA   __zpabi_mix__v_1
   LDA   gs6+6
   STA   __zpabi_mix__v_2
   LDA   gs6+7
   STA   __zpabi_mix__v_3
   JMP   mix

f8:
   SUBROUTINE

   LDY   #$00
   LDA   (__zpabi_f8__pp10_0),Y
   STA   HARGS
   INY
   LDA   (__zpabi_f8__pp10_0),Y
   STA   HARGS+1
   LDA   #$02
   STA   HARGS+2
   LDA   #$00
   STA   HARGS+3
   JSR   sdivmod16
   LDA   HARGS+6
   STA   __zpabi_mix__v_1
   LDA   HARGS+7
   STA   __zpabi_mix__v_0
   LDX   #$06
.shift_loop@2:
   LDA   __zpabi_mix__v_0
   ASL
   ROR   __zpabi_mix__v_0
   ROR   __zpabi_mix__v_1
   DEX
   BNE   .shift_loop@2
   LDA   __zpabi_mix__v_1
   STA   gs6+4
   LDA   __zpabi_mix__v_0
   STA   gs6+5
   BMI   .sx_neg@4
   LDA   #$00
   BEQ   .sx_done@5
.sx_neg@4:
   LDA   #$FF
.sx_done@5:
   STA   gs6+6
   STA   gs6+7
   LDY   #$00
   LDA   (__zpabi_f8__pp10_0),Y
   AND   #$03
   STA   __zpabi_mix__v_0
   STY   __zpabi_mix__v_1
   STA   (__zpabi_f8__pp10_0),Y
   LDA   #$00
   INY
   STA   (__zpabi_f8__pp10_0),Y
   ORA   #$00
   BMI   .sx_neg@6
   LDA   #$00
   BEQ   .sx_done@7
.sx_neg@6:
   LDA   #$FF
.sx_done@7:
   STA   __zpabi_mix__v_2
   STA   __zpabi_mix__v_3
   JSR   mix
   LDY   #$01
   LDA   (__zpabi_f8__pp10_0),Y
   DEY
   ORA   (__zpabi_f8__pp10_0),Y
   BEQ   .if_end@0
   LDA   g1
   EOR   #$03
   STA   g1
   CLC
   ADC   __zpabi_f8__p9_0
   STA   gs6+4
   LDA   g1+1
   ADC   __zpabi_f8__p9_1
   STA   gs6+5
   LDA   g1+2
   ADC   __zpabi_f8__p9_2
   STA   gs6+6
   LDA   g1+3
   ADC   __zpabi_f8__p9_3
   STA   gs6+7
   LDA   ga5+4
   STA   __zpabi_mix__v_1
   LDA   ga5+5
   STA   __zpabi_mix__v_0
   LDA   __zpabi_mix__v_1
   CLC
   ADC   #$01
   STA   __zpabi_mix__v_1
   LDA   __zpabi_mix__v_0
   ADC   #$00
   STA   ga5+5
   LDA   __zpabi_mix__v_1
   STA   ga5+4
   RTS
.if_end@0:
   LDA   __zpabi_f8__p9_0
   RTS

f15:
   SUBROUTINE

   LDA   #$03
   STA   __local_f15__l19
   LDA   #$00
   STA   __local_f15__l19+1
   LDY   #$00
   LDA   g4
   EOR   g3
   STA   gs6+4
   LDA   g4+1
   STA   gs6+5
   BMI   .sx_neg@8
   TYA
   JMP   .sx_done@9
.sx_neg@8:
   LDA   #$FF
.sx_done@9:
   STA   gs6+6
   STA   gs6+7
   LDA   #$FD
   STA   __zpabi_f8__p9_0
   LDA   #$FF
   STA   __zpabi_f8__p9_1
   JSR   f15@run3
   LDA   #<__local_f15__l19
   STA   __zpabi_f8__pp10_0
   LDA   #>__local_f15__l19
   STA   __zpabi_f8__pp10_1
   JSR   f8
   LDA   g3
   LDX   #$00
   RTS

f15@run3:
   SUBROUTINE

   STA   __zpabi_f8__p9_2
   STA   __zpabi_f8__p9_3
   STA   __zpabi_f8__p9_4
   STA   __zpabi_f8__p9_5
   STA   __zpabi_f8__p9_6
   STA   __zpabi_f8__p9_7
   RTS

f22:
   SUBROUTINE

   LDA   #$00
   STA   __local_f22__l24
   STA   __local_f22__l25
   STA   __local_f22__l25+1
   LDA   #$EC
   STA   __local_f22__l26
   LDA   #$77
   STA   __local_f22__l26+1
   LDA   #$3C
   STA   __local_f22__l26+2
   LDA   #$16
   STA   __local_f22__l26+3
   LDA   gs6+4
   STA   __local_f22__1
   LDA   gs6+5
   STA   __local_f22__0
   LDA   gs6+6
   STA   __local_f22__w28
   LDA   gs6+7
   STA   __zpabi_f8__p9_0
   LDA   __local_f22__1
   SEC
   SBC   #$01
   STA   gs6+4
   LDA   __local_f22__0
   SBC   #$00
   STA   gs6+5
   LDA   __local_f22__w28
   SBC   #$00
   STA   gs6+6
   LDA   __zpabi_f8__p9_0
   SBC   #$00
   STA   gs6+7
   LDA   g1
   CLC
   ADC   __local_f22__1
   STA   __local_f22__1
   LDA   g1+1
   ADC   __local_f22__0
   STA   __local_f22__0
   LDA   g1+2
   ADC   __local_f22__w28
   LDX   __local_f22__1
   STX   gs6+1
   LDX   __local_f22__0
   STX   gs6+2
   AND   #$07
   STA   gs6+3
   LDA   gs6+4
   STA   __local_f22__1
   LDA   gs6+1
   STA   __zpabi_f15__p17_0
   LDA   gs6+2
   AND   #$7F
   STA   __zpabi_f15__p17_1
   LDA   #<__local_f22__l26
   STA   __zpabi_f15__pp18_0
   LDA   #>__local_f22__l26
   STA   __zpabi_f15__pp18_1
   LDA   __local_f22__1
   JSR   f15
   LDY   #$00
   LDA   (gp7),Y
   STA   __zpabi_f8__p9_0
   INY
   LDA   (gp7),Y
   STA   __local_f22__1
   INY
   LDA   (gp7),Y
   STA   __local_f22__0
   INY
   LDA   (gp7),Y
   STA   __local_f22__w28
   LDA   #$00
   CLC
   SBC   __zpabi_f8__p9_0
   LDA   #$00
   SBC   __local_f22__1
   LDA   #$00
   SBC   __local_f22__0
   LDA   #$00
   SBC   __local_f22__w28
   LDA   #$00
   ROL
   STA   __local_f22__w28
   LDA   #$00
   BPL   .sx_done@11
   LDA   #$FF
.sx_done@11:
   STA   __zpabi_mix__v_2
   STA   __zpabi_mix__v_3
   LDA   __local_f22__w28
   STA   __zpabi_mix__v_0
   LDA   #$00
   STA   __zpabi_mix__v_1
   JSR   mix
   LDA   #$0C
   STA   gs6+4
   LDA   #$00
   STA   gs6+5
   LDY   #$00
   STY   gs6+6
   STA   gs6+7
   INY
   LDA   (gp7),Y
   STA   __local_f22__1
   INY
   LDA   (gp7),Y
   STA   __local_f22__0
   INY
   LDA   (gp7),Y
   STA   __local_f22__w28
   LDY   #$00
   LDA   (gp7),Y
   ORA   __local_f22__1
   ORA   __local_f22__0
   ORA   __local_f22__w28
   BNE   .or_true@1
   LDA   __local_f22__l25
   ORA   __local_f22__l25+1
   BNE   .or_true@1
   LDA   #$00
   STA   __zpabi_f8__p9_0
   JMP   .or_end@2
.or_true@1:
   LDA   #$01
   STA   __zpabi_f8__p9_0
   LDA   #$00
.or_end@2:
   ORA   #$00
   BPL   .sx_done@13
   LDA   #$FF
.sx_done@13:
   JSR   f15@run3
   LDA   #$00
   STA   __zpabi_f8__p9_1
   LDA   #<__local_f22__l25
   STA   __zpabi_f8__pp10_0
   LDA   #>__local_f22__l25
   STA   __zpabi_f8__pp10_1
   JSR   f8
   STA   __local_f22__l25
   ORA   #$00
   BMI   .sx_neg@14
   LDA   #$00
   BEQ   .sx_done@15
.sx_neg@14:
   LDA   #$FF
.sx_done@15:
   STA   __local_f22__l25+1
   LDA   #$00
   STA   __local_f22__i27
.loop@5_start:
   LDA   __local_f22__i27
   CMP   #$03
   BCC   .lb_skip@1
   JMP   .loop@5_break
.lb_skip@1:
   LDA   #$00
   STA   __local_f22__w28
.loop@6_continue:
   CLC
   ADC   #$01
   STA   __local_f22__11
   LDA   __local_f22__w28
   CMP   #$02
   BCC   .lb_skip@0
   JMP   .loop@6_break
.lb_skip@0:
   LDA   __local_f22__l24
   ASL
   ASL
   ASL
   ASL
   ASL
   ASL
   STA   __local_f22__l24
   LDY   #$01
   LDA   (gp7),Y
   STA   __local_f22__10
   INY
   LDA   (gp7),Y
   STA   __local_f22__9
   INY
   LDA   (gp7),Y
   STA   __local_f22__8
   LDY   #$00
   LDA   (gp7),Y
   CLC
   ADC   #$03
   STA   __local_f22__7
   BCC   .carry_inc@0
   INC   __local_f22__10
   BNE   .carry_inc@0
   INC   __local_f22__9
   BNE   .carry_inc@0
   INC   __local_f22__8
.carry_inc@0:
   JSR   f22@run2
   STA   __local_f22__1
   LDA   gs6+6
   STA   __local_f22__6
   LDA   gs6+7
   STA   __local_f22__5
   LSR
   LDA   __local_f22__6
   ROR
   STA   __local_f22__4
   LDA   __local_f22__1
   ROR
   STA   __local_f22__3
   LDA   __zpabi_f8__p9_0
   ROR
   STA   __local_f22__0
   LDA   #$00
   ROR
   CLC
   ADC   __zpabi_f8__p9_0
   STA   __zpabi_f8__p9_0
   LDA   __local_f22__0
   ADC   __local_f22__1
   STA   __local_f22__1
   LDA   __local_f22__3
   ADC   __local_f22__6
   STA   __local_f22__0
   LDA   __local_f22__4
   ADC   __local_f22__5
   STA   __local_f22__w28
   LDA   __zpabi_f8__p9_0
   STA   gs6+4
   LDA   __local_f22__1
   STA   gs6+5
   LDA   __local_f22__0
   STA   gs6+6
   LDA   __local_f22__w28
   STA   gs6+7
   LDA   __local_f22__7
   AND   __zpabi_f8__p9_0
   STA   __zpabi_f8__p9_0
   LDA   __local_f22__10
   AND   __local_f22__1
   STA   __local_f22__1
   LDA   __local_f22__9
   AND   __local_f22__0
   STA   __local_f22__0
   LDA   __local_f22__8
   AND   __local_f22__w28
   STA   __local_f22__w28
   LDA   __zpabi_f8__p9_0
   ORA   __local_f22__1
   ORA   __local_f22__0
   ORA   __local_f22__w28
   BEQ   .if_else@4
   LDA   __local_f22__l24
   STA   __local_f22__0
   LDA   __local_f22__i27
   AND   #$03
   ASL
   TAX
   LDA   __local_f22__0
   STA   ga5,X
   LDA   #$00
   STA   ga5+1,X
   JSR   f22@run2
   STA   __local_f22__1
   LDA   gs6+6
   STA   __local_f22__0
   LDA   gs6+7
   STA   __local_f22__w28
   LDA   __zpabi_f8__p9_0
   SEC
   SBC   #$01
   STA   gs6+4
   LDA   __local_f22__1
   SBC   #$00
   STA   gs6+5
   LDA   __local_f22__0
   SBC   #$00
   STA   gs6+6
   LDA   __local_f22__w28
   SBC   #$00
   STA   gs6+7
   LDA   __zpabi_f8__p9_0
   STA   gs6+1
   LDA   __local_f22__1
   STA   gs6+2
   LDA   __local_f22__0
   AND   #$07
   STA   gs6+3
   JMP   .if_end@3
.if_else@4:
   LDA   g1
   CLC
   ADC   #$01
   STA   __zpabi_f8__p9_0
   LDA   g1+1
   ADC   #$00
   STA   __local_f22__1
   LDA   g1+2
   ADC   #$00
   STA   __local_f22__0
   LDA   g1+3
   ADC   #$00
   LDX   __zpabi_f8__p9_0
   STX   g1
   LDX   __local_f22__1
   STX   g1+1
   LDX   __local_f22__0
   STX   g1+2
   STA   g1+3
   LDA   __zpabi_f8__p9_0
   EOR   #$7F
   STA   g2
.if_end@3:
   LDA   __local_f22__11
   STA   __local_f22__w28
   JMP   .loop@6_continue
.loop@6_break:
   INC   __local_f22__i27
   JMP   .loop@5_start
.loop@5_break:
   JSR   f22@run2
   STA   __local_f22__1
   LDA   gs6+6
   STA   __local_f22__0
   LDA   gs6+7
   STA   __local_f22__w28
   LDA   __local_f22__l26
   AND   __zpabi_f8__p9_0
   STA   HARGS+8
   LDA   __local_f22__l26+1
   AND   __local_f22__1
   STA   HARGS+9
   LDA   __local_f22__l26+2
   AND   __local_f22__0
   STA   HARGS+10
   LDA   __local_f22__l26+3
   AND   __local_f22__w28
   STA   HARGS+11
   RTS

f22@run2:
   SUBROUTINE

   LDA   gs6+4
   STA   __zpabi_f8__p9_0
   LDA   gs6+5
   RTS

main:
   SUBROUTINE

   LDA   #$FF
   STA   __local_main__l32
   LDA   #$7F
   STA   __local_main__l32+1
   LDA   #$00
   STA   __local_main__l32+2
   STA   __local_main__l32+3
   LDA   gs6+1
   LDX   gs6+2
   STX   __local_main__2
   LDX   gs6+3
   STX   __local_main__1
   SEC
   SBC   #$01
   STA   __local_main__0
   LDA   __local_main__2
   SBC   #$00
   BCS   .carry_inc@1
   DEC   __local_main__1
.carry_inc@1:
   STA   gs6+2
   LDA   __local_main__0
   STA   gs6+1
   LDA   __local_main__1
   AND   #$07
   STA   gs6+3
   LDA   g4
   SEC
   SBC   #$01
   STA   g4
   BCS   .if_end@5
   DEC   g4+1
   LDA   #$00
   BEQ   .if_end@5
   LDA   #$01
   STA   __zpabi_mix__v_0
   JSR   fin0@run0
   LDY   #$00
   LDA   (gp7),Y
   STA   __local_main__0
   LDA   #$01
   CLC
   ADC   __local_main__0
   STA   __local_main__2
   LDA   g2
   BEQ   .lnot_true@16
   LDA   #$00
   BEQ   .lnot_end@17
.lnot_true@16:
   LDA   #$01
.lnot_end@17:
   STA   __zpabi_f15__p17_0
   LDA   #$00
   STA   __zpabi_f15__p17_1
   LDA   #<__local_main__l32
   STA   __zpabi_f15__pp18_0
   LDA   #>__local_main__l32
   STA   __zpabi_f15__pp18_1
   LDA   __local_main__2
   JSR   f15
.if_end@5:
   LDA   g3
   SEC
   SBC   #$5A
   STA   g3
   DEC   g2
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

g4:
   DC.W  $0001

ga5:
   DC.W  $0100
   DC.W  $C124
   DC.W  $0003
   DC.W  $0002

gs6:
   DS.B  1
   DC.B  $FF
   DC.B  $7F
   DS.B  1
   DC.L  $0000007F

__zp_init:
   SUBROUTINE

   LDA   #$00
   STA   g1+1
   STA   g1+2
   STA   g1+3
   STA   g3
   LDA   #$03
   STA   g1
   LDA   #$80
   STA   g2
   LDA   #<g1
   STA   gp7
   LDA   #>g1
   STA   gp7+1
   RTS
