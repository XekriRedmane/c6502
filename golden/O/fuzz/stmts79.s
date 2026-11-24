SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
g1	EQU	$26
gp6	EQU	$27
g2	EQU	$29
gp5	EQU	$2B
@4.s13	EQU	$2D
__zpabi_f7__p9_0	EQU	$80
__zpabi_f7__p9_1	EQU	$81
__zpabi_f7__p10_0	EQU	$82
__zpabi_f7__p10_1	EQU	$83
__zpabi_mix__v_0	EQU	$84
__zpabi_mix__v_1	EQU	$85
__zpabi_mix__v_2	EQU	$86
__zpabi_mix__v_3	EQU	$87
__local_f14__0	EQU	$88
__local_main__l20	EQU	$88
__local_f14__1	EQU	$89
__local_f14__w19	EQU	$8A
__local_f7__0	EQU	$8B
__local_main__2	EQU	$8B
__local_f7__1	EQU	$8C
__local_main__3	EQU	$8C
__local_f7__2	EQU	$8D
__local_main__i22	EQU	$8D
__local_f7__3	EQU	$8E
__local_f7__4	EQU	$8F
__local_f7__5	EQU	$90
__local_f7__6	EQU	$91
__local_f7__7	EQU	$92
__local_mix__0	EQU	$93
__local_mix__1	EQU	$94
__local_mix__2	EQU	$95
__local_mix__3	EQU	$96

; @zp-link-meta-begin
; def f14 params=__zpabi_f14__p15 locals=__local_f14__0,__local_f14__1,__local_f14__w19 indirect=false in_cycle=false param_regs=A
; def f7 params=__zpabi_f7__p8_0,__zpabi_f7__p8_1,__zpabi_f7__p9_0,__zpabi_f7__p9_1,__zpabi_f7__p10_0,__zpabi_f7__p10_1 locals=__local_f7__0,__local_f7__1,__local_f7__2,__local_f7__3,__local_f7__4,__local_f7__5,__local_f7__6,__local_f7__7 indirect=false in_cycle=false param_regs=A,X,,,,
; def fin0 params= locals=__local_fin0__0,__local_fin0__1 indirect=false in_cycle=false
; def fin1 params= locals=__local_fin1__0,__local_fin1__1,__local_fin1__2,__local_fin1__3 indirect=false in_cycle=false
; def main params= locals=__local_main__l20,__local_main__0,__local_main__1,__local_main__2,__local_main__3,__local_main__i22 indirect=false in_cycle=false
; def mix params=__zpabi_mix__v_0,__zpabi_mix__v_1,__zpabi_mix__v_2,__zpabi_mix__v_3 locals=__local_mix__0,__local_mix__1,__local_mix__2,__local_mix__3 indirect=false in_cycle=false
; call f14 -> f7
; call f14 -> mix
; call f7 -> mix
; call fin0 -> mix
; call fin1 -> mix
; call main -> fin0
; call main -> fin1
; call main -> mix
; item mix section=.text local=false zero=false
; item fin0 section=.text local=false zero=false
; item fin0@run2 section=.text local=true zero=false
; item fin0@run1 section=.text local=true zero=false
; item fin0@run0 section=.text local=true zero=false
; item fin1 section=.text local=false zero=false
; item f7 section=.text local=false zero=false
; item f14 section=.text local=false zero=false
; item main section=.text local=false zero=false
; item acc section=.bss local=false zero=true
; item ga3 section=.data local=false zero=false
; item gs4 section=.data local=false zero=false
; item __zp_init section=.text local=false zero=false
; zp g1 size=1 at=auto def=true init=$01 weight=42
; zp gp6 size=2 at=auto def=true init=<g2,>g2 weight=33
; zp g2 size=2 at=auto def=true init=$7F,$00 weight=25
; zp gp5 size=2 at=auto def=true init=<g1,>g1 weight=20
; zp @4.s13 size=1 at=auto def=true init=$7F weight=3 local=true
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
   BMI   .sx_neg@0
   LDA   #$00
   BEQ   .sx_done@1
.sx_neg@0:
   LDA   #$FF
.sx_done@1:
   STA   __zpabi_mix__v_1
   JSR   fin0@run2
   LDA   g1
   STA   __zpabi_mix__v_0
   JSR   mix
   LDA   g2+1
   BMI   .sx_neg@2
   LDA   #$00
   BEQ   .sx_done@3
.sx_neg@2:
   LDA   #$FF
.sx_done@3:
   JSR   fin0@run2
   LDA   g2
   STA   __zpabi_mix__v_0
   LDA   g2+1
   STA   __zpabi_mix__v_1
   JSR   mix
   LDA   ga3
   JSR   fin0@run1
   LDA   ga3+1
   JSR   fin0@run0
   JMP   mix

fin0@run2:
   SUBROUTINE

   STA   __zpabi_mix__v_2
   STA   __zpabi_mix__v_3
   RTS

fin0@run1:
   SUBROUTINE

   JSR   fin0@run0
   JMP   mix

fin0@run0:
   SUBROUTINE

   STA   __zpabi_mix__v_0
   LDA   #$00
   STA   __zpabi_mix__v_1
   JSR   fin0@run2
   RTS

fin1:
   SUBROUTINE

   LDA   ga3+2
   JSR   fin0@run1
   LDA   ga3+3
   JSR   fin0@run1
   LDA   gs4+1
   STA   __zpabi_mix__v_0
   LDA   gs4+2
   STA   __zpabi_mix__v_1
   LDA   gs4+3
   STA   __zpabi_mix__v_2
   LDA   gs4+4
   STA   __zpabi_mix__v_3
   JSR   mix
   LDA   gs4+5
   STA   __zpabi_mix__v_0
   LDA   gs4+6
   STA   __zpabi_mix__v_1
   LDA   gs4+7
   STA   __zpabi_mix__v_2
   LDA   gs4+8
   STA   __zpabi_mix__v_3
   JMP   mix

f7:
   SUBROUTINE

   LDA   ga3+3
   SEC
   SBC   gs4+5
   LDY   #$00
   STA   (gp5),Y
   LDA   gs4+5
   STA   HARGS
   LDA   gs4+6
   STA   HARGS+1
   LDA   gs4+7
   STA   HARGS+2
   LDA   gs4+8
   STA   HARGS+3
   LDA   #$04
   STA   HARGS+4
   JSR   asl32
   LDA   HARGS+6
   STA   gs4+6
   LDA   HARGS+7
   STA   gs4+7
   LDA   HARGS+8
   STA   gs4+8
   LDA   HARGS+5
   STA   gs4+5
   STA   @4.s13
   LDA   gs4+1
   STA   __local_f7__7
   LDA   gs4+2
   STA   __local_f7__6
   LDA   gs4+3
   STA   __local_f7__5
   LDA   gs4+4
   STA   __local_f7__4
   LDY   #$00
   LDA   (gp6),Y
   STA   __local_f7__3
   INY
   LDA   (gp6),Y
   STA   __local_f7__2
   BMI   .sx_neg@4
   LDA   #$00
   BEQ   .sx_done@5
.sx_neg@4:
   LDA   #$FF
.sx_done@5:
   STA   __local_f7__1
   STA   __local_f7__0
   LDA   __local_f7__7
   SEC
   SBC   __local_f7__3
   STA   __local_f7__3
   LDA   __local_f7__6
   SBC   __local_f7__2
   STA   __local_f7__2
   LDA   __local_f7__5
   SBC   __local_f7__1
   STA   __local_f7__1
   LDA   __local_f7__4
   SBC   __local_f7__0
   STA   __local_f7__0
   LDA   __local_f7__3
   ORA   __local_f7__2
   ORA   __local_f7__1
   ORA   __local_f7__0
   BEQ   .if_end@0
   LDA   #$03
   STA   g1
   LDY   #$00
   LDA   (gp5),Y
   STA   __local_f7__1
   ASL
   ASL
   CLC
   ADC   __local_f7__1
   STA   __local_f7__1
   STA   (gp5),Y
   ORA   #$00
   BMI   .sx_neg@6
   LDA   #$00
   BEQ   .sx_done@7
.sx_neg@6:
   LDA   #$FF
.sx_done@7:
   STA   __local_f7__0
   LDA   __local_f7__1
   EOR   __zpabi_f7__p10_0
   STA   __zpabi_mix__v_0
   LDA   __local_f7__0
   EOR   __zpabi_f7__p10_1
   STA   __zpabi_mix__v_1
   LDA   #$00
   STA   __zpabi_mix__v_2
   STA   __zpabi_mix__v_3
   JSR   mix
   LDA   @4.s13
   CLC
   ADC   #$FF
   STA   @4.s13
   STA   HARGS+8
   LDA   #$00
   STA   HARGS+9
   LDY   #$00
   STY   HARGS+10
   STA   HARGS+11
   RTS
.if_end@0:
   LDA   #$00
   STA   HARGS+8
   LDY   #$00
   STY   HARGS+9
   STY   HARGS+10
   STA   HARGS+11
   RTS

f14:
   SUBROUTINE

   LDA   g1
   BMI   .sx_neg@8
   LDA   #$00
   BEQ   .sx_done@9
.sx_neg@8:
   LDA   #$FF
.sx_done@9:
   STA   __local_f14__0
   LDA   #$01
   SEC
   SBC   g1
   STA   __local_f14__1
   LDA   #$00
   SBC   __local_f14__0
   TAX
   LDA   g1
   BMI   .sx_neg@10
   LDA   #$00
   BEQ   .sx_done@11
.sx_neg@10:
   LDA   #$FF
.sx_done@11:
   STA   __zpabi_f7__p10_1
   LDA   #$02
   STA   __zpabi_f7__p9_0
   LDA   #$00
   STA   __zpabi_f7__p9_1
   LDA   g1
   STA   __zpabi_f7__p10_0
   LDA   __local_f14__1
   JSR   f7
   LDA   g1
   CMP   #$80
   ROR
   SEC
   SBC   #$01
   STA   g1
   BEQ   .if_end@1
   CLC
   ADC   #$01
   STA   __local_f14__0
   STA   g1
   BMI   .sx_neg@12
   LDA   #$00
   BEQ   .sx_done@13
.sx_neg@12:
   LDA   #$FF
.sx_done@13:
   STA   __zpabi_mix__v_1
   JSR   fin0@run2
   LDA   __local_f14__0
   STA   __zpabi_mix__v_0
   JSR   mix
.if_end@1:
   LDA   #$00
   STA   __local_f14__w19
.loop@0_start:
   LDA   g2
   AND   #$05
   STA   g2
   LDY   #$00
   STY   g2+1
   CLC
   ADC   ga3+2
   STA   __local_f14__1
   TYA
   ADC   #$00
   STA   __local_f14__0
   BMI   .sx_neg@14
   TYA
   JMP   .sx_done@15
.sx_neg@14:
   LDA   #$FF
.sx_done@15:
   JSR   fin0@run2
   LDA   __local_f14__1
   STA   __zpabi_mix__v_0
   LDA   __local_f14__0
   STA   __zpabi_mix__v_1
   JSR   mix
   INC   __local_f14__w19
   LDA   __local_f14__w19
   CMP   #$02
   BCC   .loop@0_start
   LDY   #$00
   LDA   (gp5),Y
   RTS

main:
   SUBROUTINE

   LDA   #$08
   STA   __local_main__l20
   LDA   #$00
   STA   __local_main__i22
.loop@1_start:
   LDA   __local_main__i22
   CMP   #$02
   BCC   .lb_skip@0
   JMP   .loop@1_break
.lb_skip@0:
   LDY   #$00
   LDA   (gp5),Y
   STA   __local_main__3
   ADC   #$01
   STA   (gp5),Y
   LDA   __local_main__3
   BMI   .sx_neg@16
   LDA   #$00
   BEQ   .sx_done@17
.sx_neg@16:
   LDA   #$FF
.sx_done@17:
   STA   __local_main__2
   LDY   #$00
   LDA   (gp6),Y
   STA   __zpabi_mix__v_0
   INY
   LDA   (gp6),Y
   STA   __zpabi_mix__v_1
   LDA   __local_main__3
   CLC
   ADC   __zpabi_mix__v_0
   STA   __zpabi_mix__v_0
   LDA   __local_main__2
   ADC   __zpabi_mix__v_1
   STA   __zpabi_mix__v_1
   BMI   .sx_neg@18
   LDA   #$00
   BEQ   .sx_done@19
.sx_neg@18:
   LDA   #$FF
.sx_done@19:
   JSR   fin0@run2
   JSR   mix
   LDA   g1
   CLC
   ADC   #$05
   STA   g1
   AND   #$07
   CMP   #$02
   BNE   .loop@1_continue
   LDA   __local_main__i22
   AND   #$07
   CMP   #$01
   BEQ   .case@6
   CMP   #$07
   BEQ   .case@7
   BNE   .default@8
.case@6:
   LDA   __local_main__l20
   AND   #$81
   STA   __local_main__l20
   STA   ga3+3
   LDY   #$00
   LDA   (gp6),Y
   STA   HARGS
   INY
   LDA   (gp6),Y
   STA   HARGS+1
   LDA   #$06
   STA   HARGS+2
   LDA   #$00
   STA   HARGS+3
   JSR   sdivmod16
   LDA   HARGS+4
   STA   g2
   LDA   HARGS+5
   STA   g2+1
.case@7:
   INC   gs4+5
   BNE   .inc_done@0
   INC   gs4+6
   BNE   .inc_done@0
   INC   gs4+7
   BNE   .inc_done@0
   INC   gs4+8
.inc_done@0:
   LDA   g1
   STA   __zpabi_mix__v_0
   LDX   #$04
.shift_loop@20:
   LDA   __zpabi_mix__v_0
   ASL
   ROR   __zpabi_mix__v_0
   DEX
   BNE   .shift_loop@20
   LDA   __zpabi_mix__v_0
   STA   g1
   STA   __zpabi_mix__v_1
   BMI   .sx_neg@22
   LDA   #$00
   BEQ   .sx_done@23
.sx_neg@22:
   LDA   #$FF
.sx_done@23:
   STA   __zpabi_mix__v_0
   LDA   #$08
   ORA   __zpabi_mix__v_1
   ORA   __zpabi_mix__v_0
   BNE   .switch@5_break
.default@8:
.switch@5_break:
.loop@1_continue:
   INC   __local_main__i22
   JMP   .loop@1_start
.loop@1_break:
   JSR   fin0
   JSR   fin1
   LDA   acc+1
   AND   #$7F
   TAX
   LDA   acc
   RTS

acc:
   DS.B  2

ga3:
   DC.B  $7F, $00, $FF, $0A

gs4:
   DS.B  1
   DC.L  $00007FFF
   DC.L  $00000004

__zp_init:
   SUBROUTINE

   LDA   #$00
   STA   g2+1
   LDA   #$01
   STA   g1
   LDA   #$7F
   STA   g2
   STA   @4.s13
   LDA   #<g1
   STA   gp5
   LDA   #>g1
   STA   gp5+1
   LDA   #<g2
   STA   gp6
   LDA   #>g2
   STA   gp6+1
   RTS
