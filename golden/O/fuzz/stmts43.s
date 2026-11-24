SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
z3	EQU	$26
gp7	EQU	$27
gp6	EQU	$29
g1	EQU	$2B
__zpabi_f18__pp20_0	EQU	$80
__zpabi_f8__p9_0	EQU	$80
__zpabi_f18__pp20_1	EQU	$81
__zpabi_f8__p9_1	EQU	$81
__local_f18__w24	EQU	$82
__zpabi_f8__p9_2	EQU	$82
__local_f18__0	EQU	$83
__zpabi_mix__v_0	EQU	$83
__local_f18__1	EQU	$84
__zpabi_mix__v_1	EQU	$84
__local_f18__2	EQU	$85
__zpabi_mix__v_2	EQU	$85
__local_f18__3	EQU	$86
__zpabi_mix__v_3	EQU	$86
__local_f18__4	EQU	$87
__local_main__0	EQU	$87
__local_f18__5	EQU	$88
__local_f18__6	EQU	$89
__local_f18__p19	EQU	$8A
__local_main__w29	EQU	$8A
__local_main__l27	EQU	$8B
__local_f12__pp13	EQU	$8C
__local_f12__0	EQU	$8D
__local_f12__1	EQU	$8E
__local_f12__2	EQU	$8F
__local_f12__3	EQU	$90
__local_f8__p9_2	EQU	$94
__local_f8__p9_1	EQU	$95
__local_f8__2	EQU	$99
__local_f8__3	EQU	$9A
__local_f8__i11	EQU	$9B
__local_mix__0	EQU	$9C
__local_mix__1	EQU	$9D
__local_mix__2	EQU	$9E
__local_mix__3	EQU	$9F

; @zp-link-meta-begin
; def f12 params=__zpabi_f12__pp13_0,__zpabi_f12__pp13_1 locals=__local_f12__pp13,__local_f12__0,__local_f12__1,__local_f12__2,__local_f12__3,__local_f12__4,__local_f12__5,__local_f12__6 indirect=false in_cycle=false param_regs=A,X
; def f18 params=__zpabi_f18__p19,__zpabi_f18__pp20_0,__zpabi_f18__pp20_1 locals=__local_f18__w24,__local_f18__0,__local_f18__1,__local_f18__2,__local_f18__3,__local_f18__4,__local_f18__5,__local_f18__6,__local_f18__p19 indirect=false in_cycle=false param_regs=A,,
; def f8 params=__zpabi_f8__p9_0,__zpabi_f8__p9_1,__zpabi_f8__p9_2 locals=__local_f8__p9_2,__local_f8__p9_1,__local_f8__p9_0,__local_f8__0,__local_f8__1,__local_f8__2,__local_f8__3,__local_f8__i11 indirect=false in_cycle=false changes=__zpabi_f8__p9_0
; def fin0 params= locals=__local_fin0__0,__local_fin0__1,__local_fin0__2,__local_fin0__3 indirect=false in_cycle=false
; def fin1 params= locals=__local_fin1__0 indirect=false in_cycle=false
; def fin2 params= locals=__local_fin2__0,__local_fin2__1 indirect=false in_cycle=false
; def main params= locals=__local_main__0,__local_main__w30,__local_main__1,__local_main__w29,__local_main__l27 indirect=false in_cycle=false
; def mix params=__zpabi_mix__v_0,__zpabi_mix__v_1,__zpabi_mix__v_2,__zpabi_mix__v_3 locals=__local_mix__0,__local_mix__1,__local_mix__2,__local_mix__3 indirect=false in_cycle=false
; call f12 -> f8
; call f12 -> mix
; call f8 -> mix
; call fin0 -> mix
; call fin1 -> mix
; call fin2 -> mix
; call main -> f12
; call main -> f8
; call main -> fin0
; call main -> fin1
; call main -> fin2
; call main -> mix
; item mix section=.text local=false zero=false
; item fin0 section=.text local=false zero=false
; item fin0@run2 section=.text local=true zero=false
; item fin0@run1 section=.text local=true zero=false
; item fin0@run0 section=.text local=true zero=false
; item fin1 section=.text local=false zero=false
; item fin2 section=.text local=false zero=false
; item f8 section=.text local=false zero=false
; item f12 section=.text local=false zero=false
; item f12@run3 section=.text local=true zero=false
; item f18 section=.text local=false zero=false
; item main section=.text local=false zero=false
; item acc section=.bss local=false zero=true
; item g2 section=.data local=false zero=false
; item ga4 section=.data local=false zero=false
; item gs5 section=.data local=false zero=false
; item @5.s17 section=.data local=true zero=false
; item __zp_init section=.text local=false zero=false
; zp z3 size=1 at=auto def=true init=$00
; zp gp7 size=2 at=auto def=true init=<g2,>g2 weight=26
; zp gp6 size=2 at=auto def=true init=<g1,>g1 weight=19
; zp g1 size=4 at=auto def=true init=$03,$00,$00,$00 weight=19
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

   LDA   g1
   STA   __zpabi_mix__v_0
   LDA   g1+1
   STA   __zpabi_mix__v_1
   LDA   g1+2
   STA   __zpabi_mix__v_2
   LDA   g1+3
   JSR   fin0@run1
   LDA   g2
   STA   __zpabi_mix__v_0
   LDA   g2+1
   STA   __zpabi_mix__v_1
   LDA   g2+2
   STA   __zpabi_mix__v_2
   LDA   g2+3
   JSR   fin0@run1
   LDA   z3
   JSR   fin0@run0
   LDA   ga4
   JSR   fin0@run2
   JMP   mix

fin0@run2:
   SUBROUTINE

   STA   __zpabi_mix__v_0
   LDA   #$00
   STA   __zpabi_mix__v_1
   STA   __zpabi_mix__v_2
   STA   __zpabi_mix__v_3
   RTS

fin0@run1:
   SUBROUTINE

   STA   __zpabi_mix__v_3
   JMP   mix

fin0@run0:
   SUBROUTINE

   JSR   fin0@run2
   JMP   mix

fin1:
   SUBROUTINE

   LDA   ga4+1
   JSR   fin0@run0
   LDA   ga4+2
   JSR   fin0@run0
   LDA   ga4+3
   JSR   fin0@run0
   LDA   gs5+1
   BMI   .sx_neg@0
   LDA   #$00
   BEQ   .sx_done@1
.sx_neg@0:
   LDA   #$FF
.sx_done@1:
   STA   __zpabi_mix__v_1
   STA   __zpabi_mix__v_2
   STA   __zpabi_mix__v_3
   LDA   gs5+1
   STA   __zpabi_mix__v_0
   JMP   mix

fin2:
   SUBROUTINE

   LDA   gs5+3
   BMI   .sx_neg@2
   LDA   #$00
   BEQ   .sx_done@3
.sx_neg@2:
   LDA   #$FF
.sx_done@3:
   STA   __zpabi_mix__v_2
   STA   __zpabi_mix__v_3
   LDA   gs5+2
   STA   __zpabi_mix__v_0
   LDA   gs5+3
   STA   __zpabi_mix__v_1
   JMP   mix

f8:
   SUBROUTINE

   INC   ga4+3
   LDA   ga4+3
   STA   __local_f8__p9_2
   LSR
   LSR
   LSR
   LSR
   LSR
   STA   __local_f8__p9_1
   LDA   __local_f8__p9_2
   ASL
   ASL
   ASL
   STA   __local_f8__p9_2
   LDA   __local_f8__p9_1
   BMI   .sx_neg@4
   LDA   #$00
   BEQ   .sx_done@5
.sx_neg@4:
   LDA   #$FF
.sx_done@5:
   STA   __zpabi_mix__v_2
   STA   __zpabi_mix__v_3
   LDA   __local_f8__p9_2
   STA   __zpabi_mix__v_0
   LDA   __local_f8__p9_1
   STA   __zpabi_mix__v_1
   JSR   mix
   LDA   gs5+1
   BEQ   .if_else@1
   LDA   z3
   SEC
   SBC   #$01
   STA   z3
   LDA   __zpabi_f8__p9_1
   STA   __local_f8__p9_1
   LDA   __zpabi_f8__p9_2
   STA   __local_f8__p9_2
   JMP   .if_end@0
.if_else@1:
   LDA   __zpabi_f8__p9_0
   CLC
   ADC   #$FF
   STA   __zpabi_f8__p9_0
   LDA   __zpabi_f8__p9_1
   ADC   #$00
   STA   __local_f8__p9_1
   LDA   __zpabi_f8__p9_2
   ADC   #$00
   AND   #$7F
   STA   __local_f8__p9_2
   LDA   #$00
   STA   __local_f8__i11
.loop@0_start:
   LDA   __local_f8__i11
   BEQ   .lb_skip@0
   JMP   .loop@0_break
.lb_skip@0:
   INC   g1
   BNE   .inc_done@0
   INC   g1+1
   BNE   .inc_done@0
   INC   g1+2
   BNE   .inc_done@0
   INC   g1+3
.inc_done@0:
   LDA   gs5+2
   STA   __local_f8__3
   LDA   gs5+3
   STA   __local_f8__2
   LDA   __local_f8__i11
   AND   #$03
   TAX
   LDA   ga4,X
   ORA   #$81
   STA   ga4,X
   ORA   __local_f8__3
   STA   __zpabi_mix__v_0
   LDA   __local_f8__2
   STA   __zpabi_mix__v_1
   BMI   .sx_neg@6
   LDA   #$00
   BEQ   .sx_done@7
.sx_neg@6:
   LDA   #$FF
.sx_done@7:
   STA   __zpabi_mix__v_2
   JSR   fin0@run1
   LDY   #$03
   LDA   (gp6),Y
   STA   __zpabi_mix__v_0
   LDY   #$00
   LDA   (gp6),Y
   SEC
   SBC   #$01
   STA   __zpabi_mix__v_1
   INY
   LDA   (gp6),Y
   SBC   #$00
   STA   __local_f8__3
   INY
   LDA   (gp6),Y
   SBC   #$00
   STA   __local_f8__2
   BCS   .carry_inc@0
   DEC   __zpabi_mix__v_0
.carry_inc@0:
   LDA   __zpabi_mix__v_1
   LDY   #$00
   STA   (gp6),Y
   LDA   __local_f8__3
   INY
   STA   (gp6),Y
   LDA   __local_f8__2
   INY
   STA   (gp6),Y
   LDA   __zpabi_mix__v_0
   INY
   STA   (gp6),Y
   INC   __local_f8__i11
   JMP   .loop@0_start
.loop@0_break:
.if_end@0:
   LDA   __zpabi_f8__p9_0
   STA   HARGS
   LDA   __local_f8__p9_1
   STA   HARGS+1
   LDA   __local_f8__p9_2
   STA   HARGS+2
   LDA   #$00
   STA   HARGS+3
   LDY   #$03
   STY   HARGS+4
   LDY   #$00
   STY   HARGS+5
   STY   HARGS+6
   STA   HARGS+7
   JSR   udivmod32
   LDA   #$00
   STA   HARGS+11
   RTS

f12:
   SUBROUTINE

   STA   __local_f12__pp13
   STX   __local_f12__0
   LDY   #$00
   LDA   (__local_f12__pp13),Y
   STA   __local_f12__1
   BMI   .sx_neg@8
   LDA   #$00
   BEQ   .sx_done@9
.sx_neg@8:
   LDA   #$FF
.sx_done@9:
   STA   __zpabi_f8__p9_1
   LDX   __local_f12__1
   STX   __zpabi_f8__p9_0
   JSR   f12@run3
   LDA   z3
   LDX   #$FF
   SEC
.div_loop@10:
   INX
   SBC   #$05
   BCS   .div_loop@10
   TXA
   BEQ   .if_else@4
   LDA   #$00
   BPL   .sx_done@12
   LDA   #$FF
.sx_done@12:
   STA   __local_f12__0
   LDA   @5.s17
   EOR   #$08
   STA   __zpabi_mix__v_0
   LDA   #$00
   STA   __zpabi_mix__v_1
   LDA   __local_f12__0
   STA   __zpabi_mix__v_2
   JSR   fin0@run1
   JMP   .if_end@3
.if_else@4:
   LDA   g1
   STA   HARGS
   LDA   g1+1
   STA   HARGS+1
   LDA   g1+2
   STA   HARGS+2
   LDA   g1+3
   STA   HARGS+3
   LDA   #$02
   STA   HARGS+4
   JSR   asr32
   LDA   HARGS+5
   STA   g1
   LDA   HARGS+6
   STA   g1+1
   LDA   HARGS+7
   STA   g1+2
   LDA   HARGS+8
   STA   g1+3
.if_end@3:
   LDY   #$07
   LDA   (gp7),Y
   STA   __local_f12__0
   LDY   #$00
   LDA   (gp7),Y
   SEC
   SBC   #$05
   STA   __local_f12__pp13
   INY
   LDA   (gp7),Y
   SBC   #$00
   STA   __zpabi_mix__v_1
   INY
   LDA   (gp7),Y
   SBC   #$00
   STA   __zpabi_mix__v_2
   INY
   LDA   (gp7),Y
   SBC   #$00
   STA   __zpabi_mix__v_3
   INY
   LDA   (gp7),Y
   SBC   #$00
   STA   __local_f12__3
   INY
   LDA   (gp7),Y
   SBC   #$00
   STA   __local_f12__2
   INY
   LDA   (gp7),Y
   SBC   #$00
   STA   __local_f12__1
   BCS   .carry_inc@1
   DEC   __local_f12__0
.carry_inc@1:
   LDA   __local_f12__pp13
   LDY   #$00
   STA   (gp7),Y
   LDA   __zpabi_mix__v_1
   INY
   STA   (gp7),Y
   LDA   __zpabi_mix__v_2
   INY
   STA   (gp7),Y
   LDA   __zpabi_mix__v_3
   INY
   STA   (gp7),Y
   LDA   __local_f12__3
   INY
   STA   (gp7),Y
   LDA   __local_f12__2
   INY
   STA   (gp7),Y
   LDA   __local_f12__1
   INY
   STA   (gp7),Y
   LDA   __local_f12__0
   INY
   STA   (gp7),Y
   LDA   __local_f12__pp13
   STA   __zpabi_mix__v_0
   JSR   mix
   LDX   #$00
   LDA   #$01
   RTS

f12@run3:
   SUBROUTINE

   AND   #$7F
   STA   __zpabi_f8__p9_2
   JMP   f8

f18:
   SUBROUTINE

   STA   __local_f18__p19
   LDA   g2
   STA   __local_f18__6
   LDA   g2+1
   STA   __local_f18__5
   LDA   g2+2
   STA   __local_f18__4
   LDA   g2+3
   STA   __local_f18__3
   LDA   g2+4
   STA   __local_f18__2
   LDA   g2+5
   STA   __local_f18__1
   LDA   g2+6
   STA   __local_f18__0
   LDA   g2+7
   STA   __local_f18__w24
   LDA   g2
   SEC
   SBC   #$01
   STA   g2
   LDA   g2+1
   SBC   #$00
   STA   g2+1
   LDA   g2+2
   SBC   #$00
   STA   g2+2
   LDA   g2+3
   SBC   #$00
   STA   g2+3
   LDA   g2+4
   SBC   #$00
   STA   g2+4
   LDA   g2+5
   SBC   #$00
   STA   g2+5
   LDA   g2+6
   SBC   #$00
   STA   g2+6
   BCS   .carry_inc@2
   DEC   g2+7
.carry_inc@2:
   LDA   __local_f18__6
   ORA   __local_f18__5
   ORA   __local_f18__4
   ORA   __local_f18__3
   ORA   __local_f18__2
   ORA   __local_f18__1
   ORA   __local_f18__0
   ORA   __local_f18__w24
   BNE   .lb_skip@1
   JMP   .if_end@5
.lb_skip@1:
   LDY   #$01
   LDA   (gp6),Y
   STA   __local_f18__2
   INY
   LDA   (gp6),Y
   STA   __local_f18__1
   INY
   LDA   (gp6),Y
   STA   __local_f18__0
   LDY   #$00
   LDA   (gp6),Y
   CLC
   ADC   __local_f18__p19
   BCC   .carry_inc@3
   INC   __local_f18__2
   BNE   .carry_inc@3
   INC   __local_f18__1
   BNE   .carry_inc@3
   INC   __local_f18__0
.carry_inc@3:
   ORA   __local_f18__2
   ORA   __local_f18__1
   ORA   __local_f18__0
   BEQ   .if_end@6
   LDA   #$03
   CLC
   ADC   gs5+2
   STA   __local_f18__0
   LDA   #$00
   ADC   gs5+3
   STA   __local_f18__w24
   BMI   .sx_neg@13
   LDA   #$00
   BEQ   .sx_done@14
.sx_neg@13:
   LDA   #$FF
.sx_done@14:
   STA   HARGS+18
   STA   HARGS+19
   STA   HARGS+20
   STA   HARGS+21
   STA   HARGS+22
   STA   HARGS+23
   LDA   __local_f18__0
   STA   HARGS+16
   LDA   __local_f18__w24
   STA   HARGS+17
   RTS
.if_end@6:
   LDA   #$00
   STA   __local_f18__w24
   JMP   .loop@2_continue
.f18@rot@0:
   LDY   #$00
   LDA   (gp7),Y
   STA   __local_f18__6
   LDA   __local_f18__p19
   EOR   __local_f18__6
   AND   #$07
   CMP   #$01
   BEQ   .switch@3_break
   CMP   #$03
   BEQ   .switch@3_break
   CMP   #$06
.switch@3_break:
   LDA   __local_f18__p19
   STA   __local_f18__w24
.loop@2_continue:
   CLC
   ADC   #$01
   STA   __local_f18__p19
   LDA   __local_f18__w24
   CMP   #$02
   BCC   .f18@rot@0
.if_end@5:
   LDA   ga4+3
   STA   HARGS+16
   LDA   #$00
   STA   HARGS+17
   LDY   #$00
   STY   HARGS+18
   STY   HARGS+19
   STY   HARGS+20
   STY   HARGS+21
   STY   HARGS+22
   STA   HARGS+23
   RTS

main:
   SUBROUTINE

   LDA   #$7F
   STA   __local_main__l27
   LDA   g2
   AND   #$05
   STA   g2
   LDA   #$00
   STA   g2+1
   LDY   #$00
   STY   g2+2
   STY   g2+3
   STY   g2+4
   STY   g2+5
   STY   g2+6
   STA   g2+7
   LDA   g2
   BEQ   .if_end@7
   LDX   #>__local_main__l27
   LDA   #<__local_main__l27
   JSR   f12
   LDA   gs5+1
   STA   __zpabi_f8__p9_1
   SEC
   SBC   #$01
   STA   gs5+1
   LDA   __zpabi_f8__p9_1
   BMI   .sx_neg@15
   LDA   #$00
   BEQ   .sx_done@16
.sx_neg@15:
   LDA   #$FF
.sx_done@16:
   STA   __local_main__0
   LDA   #$80
   CLC
   ADC   __zpabi_f8__p9_1
   STA   __zpabi_f8__p9_1
   LDA   #$00
   ADC   __local_main__0
   STA   g2+1
   LDA   __zpabi_f8__p9_1
   STA   g2
   LDA   #$00
   STA   g2+2
   STA   g2+3
   STA   g2+4
   STA   g2+5
   STA   g2+6
   STA   g2+7
.if_end@7:
   LDA   #$00
   STA   __local_main__w29
.loop@8_start:
   LDA   __local_main__l27
   AND   #$5A
   STA   __local_main__l27
   LDA   ga4+1
   CLC
   ADC   #$81
   STA   ga4+1
   LDA   g2
   SEC
   SBC   #$80
   STA   __zpabi_f8__p9_0
   LDA   g2+1
   SBC   #$00
   STA   __zpabi_f8__p9_1
   LDA   g2+2
   SBC   #$00
   JSR   f12@run3
   LDA   HARGS+8
   STA   __zpabi_mix__v_0
   LDA   HARGS+9
   STA   __zpabi_mix__v_1
   LDA   HARGS+10
   STA   __zpabi_mix__v_2
   LDA   HARGS+11
   JSR   fin0@run1
   LDY   #$00
   LDA   (gp7),Y
   AND   #$07
   CMP   #$02
   BEQ   .case@10
   CMP   #$04
   BEQ   .switch@9_break
   CMP   #$07
   BEQ   .case@13
   BNE   .default@14
.case@10:
   LDA   #$00
   STA   __zpabi_f8__p9_1
   CLC
   JMP   .loop@11_continue
.main@rot@1:
   DEC   __local_main__l27
   LDA   __local_main__0
   STA   __zpabi_f8__p9_1
.loop@11_continue:
   ADC   #$01
   STA   __local_main__0
   LDA   __zpabi_f8__p9_1
   CMP   #$03
   BCC   .main@rot@1
.case@13:
.default@14:
.switch@9_break:
   INC   __local_main__w29
   LDA   __local_main__w29
   CMP   #$04
   BCS   .lb_skip@2
   JMP   .loop@8_start
.lb_skip@2:
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
   DC.L  $000000FF
   DC.L  $00000000

ga4:
   DC.B  $80, $80, $FF, $03

gs5:
   DS.B  1
   DC.B  $FF
   DC.W  $007F

@5.s17:
   DC.B  $80

__zp_init:
   SUBROUTINE

   LDA   #$00
   STA   g1+1
   STA   g1+2
   STA   g1+3
   STA   z3
   LDA   #$03
   STA   g1
   LDA   #<g1
   STA   gp6
   LDA   #>g1
   STA   gp6+1
   LDA   #<g2
   STA   gp7
   LDA   #>g2
   STA   gp7+1
   RTS
