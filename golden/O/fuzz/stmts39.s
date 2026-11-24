SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
z3	EQU	$26
z4	EQU	$2A
gp8	EQU	$2B
g2	EQU	$2D
gp7	EQU	$2E
g1	EQU	$30
__zpabi_f9__p12	EQU	$80
__zpabi_mix__v_0	EQU	$80
__zpabi_f9__pp13_0	EQU	$81
__zpabi_mix__v_1	EQU	$81
__zpabi_f9__pp13_1	EQU	$82
__zpabi_mix__v_2	EQU	$82
__zpabi_mix__v_3	EQU	$83
__local_main__0	EQU	$84
__local_main__1	EQU	$85
__local_main__2	EQU	$86
__local_main__3	EQU	$87
__local_main__l21	EQU	$88
__local_main__4	EQU	$89
__local_main__l23	EQU	$8A
__local_f16__w20	EQU	$8B
__local_f16__0	EQU	$8C
__local_f16__1	EQU	$8D
__local_f16__2	EQU	$8E
__local_f16__3	EQU	$8F
__local_f16__l18	EQU	$90
__local_f16__l19	EQU	$91
__local_mix__0	EQU	$94
__local_mix__1	EQU	$95
__local_mix__2	EQU	$96
__local_mix__3	EQU	$97

; @zp-link-meta-begin
; def f16 params=__zpabi_f16__p17_0,__zpabi_f16__p17_1 locals=__local_f16__w20,__local_f16__0,__local_f16__1,__local_f16__2,__local_f16__3,__local_f16__l18,__local_f16__l19,__local_f16__5,__local_f16__6 indirect=false in_cycle=false param_regs=A,X
; def f9 params=__zpabi_f9__p10,__zpabi_f9__p11,__zpabi_f9__p12,__zpabi_f9__pp13_0,__zpabi_f9__pp13_1 locals=__local_f9__0,__local_f9__1,__local_f9__2 indirect=false in_cycle=false param_regs=A,X,,,
; def fin0 params= locals=__local_fin0__0,__local_fin0__1,__local_fin0__2,__local_fin0__3 indirect=false in_cycle=false
; def fin1 params= locals=__local_fin1__0 indirect=false in_cycle=false
; def fin2 params= locals=__local_fin2__0,__local_fin2__1 indirect=false in_cycle=false
; def main params= locals=__local_main__0,__local_main__1,__local_main__2,__local_main__3,__local_main__l21,__local_main__4,__local_main__l23 indirect=false in_cycle=false
; def mix params=__zpabi_mix__v_0,__zpabi_mix__v_1,__zpabi_mix__v_2,__zpabi_mix__v_3 locals=__local_mix__0,__local_mix__1,__local_mix__2,__local_mix__3 indirect=false in_cycle=false
; call f16 -> f9
; call f16 -> mix
; call fin0 -> mix
; call fin1 -> mix
; call fin2 -> mix
; call main -> f16
; call main -> f9
; call main -> fin0
; call main -> fin1
; call main -> fin2
; call main -> mix
; item mix section=.text local=false zero=false
; item fin0 section=.text local=false zero=false
; item fin0@run0 section=.text local=true zero=false
; item fin0@run0@run2 section=.text local=true zero=false
; item fin0@tail0 section=.text local=true zero=false
; item fin1 section=.text local=false zero=false
; item fin2 section=.text local=false zero=false
; item f9 section=.text local=false zero=false
; item f16 section=.text local=false zero=false
; item f16@run4 section=.text local=true zero=false
; item f16@run1 section=.text local=true zero=false
; item main section=.text local=false zero=false
; item main@run3 section=.text local=true zero=false
; item acc section=.bss local=false zero=true
; item ga5 section=.data local=false zero=false
; item gs6 section=.data local=false zero=false
; item __zp_init section=.text local=false zero=false
; zp z3 size=4 at=auto def=true init=$01,$00,$00,$00
; zp z4 size=1 at=auto def=true init=$07
; zp gp8 size=2 at=auto def=true init=<z3,>z3 weight=31
; zp g2 size=1 at=auto def=true init=$0A weight=13
; zp gp7 size=2 at=auto def=true init=<g2,>g2 weight=24
; zp g1 size=7 at=auto def=true init=$00,$80,$00,$00,$00,$00,$00 weight=26
; runtime hargs_size=32 hargs_need=9
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
   JSR   fin0@run0
   LDA   z3
   STA   __zpabi_mix__v_0
   LDA   z3+1
   STA   __zpabi_mix__v_1
   LDA   z3+2
   STA   __zpabi_mix__v_2
   LDA   z3+3
   STA   __zpabi_mix__v_3
   JSR   mix
   LDA   z4
   JMP   fin0@tail0

fin0@run0:
   SUBROUTINE

   JSR   fin0@run0@run2
   JMP   mix

fin0@run0@run2:
   SUBROUTINE

   STA   __zpabi_mix__v_0
   LDA   #$00
   STA   __zpabi_mix__v_1
   STA   __zpabi_mix__v_2
   STA   __zpabi_mix__v_3
   RTS

fin0@tail0:
   SUBROUTINE

   JSR   fin0@run0@run2
   JMP   mix

fin1:
   SUBROUTINE

   LDA   ga5
   JSR   fin0@run0
   LDA   ga5+1
   JSR   fin0@run0
   LDA   ga5+2
   JSR   fin0@run0
   LDA   ga5+3
   JMP   fin0@tail0

fin2:
   SUBROUTINE

   LDA   gs6+2
   BMI   .sx_neg@0
   LDA   #$00
   BEQ   .sx_done@1
.sx_neg@0:
   LDA   #$FF
.sx_done@1:
   STA   __zpabi_mix__v_2
   STA   __zpabi_mix__v_3
   LDA   gs6+1
   STA   __zpabi_mix__v_0
   LDA   gs6+2
   STA   __zpabi_mix__v_1
   JSR   mix
   LDA   gs6+3
   STA   __zpabi_mix__v_0
   LDA   gs6+4
   STA   __zpabi_mix__v_1
   LDA   #$00
   STA   __zpabi_mix__v_2
   STA   __zpabi_mix__v_3
   JMP   mix

f9:
   SUBROUTINE

   LDA   gs6+3
   ORA   #$03
   STA   gs6+3
   ORA   __zpabi_f9__p12
   AND   #$07
   CMP   #$02
   BEQ   .case@1
   CMP   #$03
   BEQ   .case@3
   CMP   #$04
   BEQ   .case@4
   CMP   #$05
   BEQ   .case@5
   BNE   .switch@0_break
.case@1:
   LDA   #$01
   CMP   g2
   BEQ   .cmp_true@4
   LDA   #$00
.cmp_true@4:
   STA   gs6+1
   LDA   #$00
   STA   gs6+2
   LDA   gs6+3
   LDX   gs6+4
   RTS
.case@3:
.case@4:
.case@5:
.switch@0_break:
   LDY   #$01
   LDA   (gp8),Y
   TAX
   DEY
   LDA   (gp8),Y
   RTS

f16:
   SUBROUTINE

   LDA   #$03
   STA   __local_f16__l19
   LDA   z4
   STA   HARGS
   LDA   #$5A
   STA   HARGS+1
   JSR   mul8
   LDA   HARGS+2
   AND   #$3F
   STA   z4
   LDA   #$0C
   ORA   z4
   JSR   fin0@run0
   LDA   #$03
   STA   __local_f16__l18
   LDA   #$00
   STA   __local_f16__w20
.loop@6_continue:
   CLC
   ADC   #$01
   STA   __local_f16__3
   LDA   __local_f16__w20
   CMP   #$02
   BCC   .lb_skip@1
   JMP   .loop@6_break
.lb_skip@1:
   LDA   gs6+3
   STA   __local_f16__0
   ORA   gs6+4
   BNE   .lb_skip@0
   JMP   .if_else@6
.lb_skip@0:
   LDA   g1
   STA   __local_f16__w20
   SEC
   SBC   #$01
   STA   g1
   LDA   g1+1
   SBC   #$00
   STA   g1+1
   LDA   g1+2
   SBC   #$00
   STA   g1+2
   LDA   g1+3
   SBC   #$00
   STA   g1+3
   LDA   g1+4
   SBC   #$00
   STA   g1+4
   LDA   g1+5
   SBC   #$00
   STA   g1+5
   BCS   .carry_inc@0
   DEC   g1+6
.carry_inc@0:
   LDA   __local_f16__w20
   LDY   #$00
   STA   (gp7),Y
   LDA   #$00
   STA   __zpabi_f9__p12
   JSR   f16@run4
   LDX   #$2A
   LDA   #$FE
   JSR   f9
   JSR   f16@run1
   LDA   __local_f16__l18
   CLC
   ADC   #$01
   AND   #$3F
   STA   __local_f16__w20
   LDA   __local_f16__l18
   JSR   fin0@run0
   LDA   __local_f16__w20
   STA   __local_f16__l18
   JMP   .if_end@5
.if_else@6:
   LDA   __local_f16__0
   ADC   gs6+1
   STA   __local_f16__2
   LDA   __local_f16__3
   CLC
   ADC   __local_f16__3
   STA   __local_f16__1
   LDY   #$00
   LDA   (gp8),Y
   STA   HARGS
   LDA   __local_f16__3
   STA   HARGS+1
   JSR   mul8
   LDA   HARGS+2
   STA   __zpabi_f9__p12
   JSR   f16@run4
   LDX   __local_f16__1
   LDA   __local_f16__2
   JSR   f9
   JSR   f16@run1
.if_end@5:
   LDY   #$00
   LDA   (gp8),Y
   STA   HARGS
   INY
   LDA   (gp8),Y
   STA   HARGS+1
   INY
   LDA   (gp8),Y
   STA   HARGS+2
   INY
   LDA   (gp8),Y
   STA   HARGS+3
   LDA   #$06
   STA   HARGS+4
   JSR   lsr32
   LDA   HARGS+5
   STA   __local_f16__2
   LDA   HARGS+6
   STA   __local_f16__1
   LDA   HARGS+7
   STA   __local_f16__0
   LDA   HARGS+8
   STA   __local_f16__w20
   LDA   __local_f16__2
   LDY   #$00
   STA   (gp8),Y
   LDA   __local_f16__1
   INY
   STA   (gp8),Y
   LDA   __local_f16__0
   INY
   STA   (gp8),Y
   LDA   __local_f16__w20
   INY
   STA   (gp8),Y
   LDA   __local_f16__2
   LDY   #$00
   STA   (gp7),Y
   LDA   gs6+1
   CLC
   ADC   #$03
   STA   __local_f16__l19
   CLC
   ADC   __local_f16__3
   STA   gs6+1
   LDA   #$00
   ADC   #$00
   STA   gs6+2
   LDA   __local_f16__3
   STA   __local_f16__w20
   JMP   .loop@6_continue
.loop@6_break:
   LDA   z3
   INC   z3
   BNE   .inc_done@0
   INC   z3+1
   BNE   .inc_done@0
   INC   z3+2
   BNE   .inc_done@0
   INC   z3+3
.inc_done@0:
   AND   #$02
   AND   #$07
   CMP   #$05
   LDX   #$00
   LDA   #$02
   RTS

f16@run4:
   SUBROUTINE

   LDA   #<__local_f16__l19
   STA   __zpabi_f9__pp13_0
   LDA   #>__local_f16__l19
   STA   __zpabi_f9__pp13_1
   RTS

f16@run1:
   SUBROUTINE

   STA   __zpabi_mix__v_0
   STX   __zpabi_mix__v_1
   LDA   #$00
   STA   __zpabi_mix__v_2
   STA   __zpabi_mix__v_3
   JMP   mix

main:
   SUBROUTINE

   LDA   #$00
   STA   __local_main__l21
   STA   __local_main__4
   LDA   #$FF
   STA   __local_main__l23
   LDA   gs6+1
   STA   __local_main__3
   LDA   z3
   ORA   z3+1
   ORA   z3+2
   ORA   z3+3
   BNE   .or_true@7
   LDA   #$FF
   BNE   .or_true@7
   LDX   #$00
   BEQ   .or_end@8
.or_true@7:
   LDX   #$01
.or_end@8:
   LDA   gs6+3
   STA   __local_main__2
   ORA   gs6+4
   BNE   .cond_end@10
   LDA   z3
   STA   __local_main__2
.cond_end@10:
   LDA   __local_main__2
   JSR   main@run3
   LDA   __local_main__3
   JSR   f9
   STA   __local_main__l23
   LDA   z4
   CLC
   ADC   #$05
   AND   #$3F
   STA   z4
   BNE   .lb_skip@3
   JMP   .if_else@12
.lb_skip@3:
   LDY   #$00
   LDA   (gp8),Y
   STA   __local_main__0
   INY
   LDA   (gp8),Y
   STA   __local_main__3
   INY
   LDA   (gp8),Y
   STA   __local_main__2
   INY
   LDA   (gp8),Y
   STA   __local_main__1
   INC   __local_main__0
   BNE   .inc_done@1
   INC   __local_main__3
   BNE   .inc_done@1
   INC   __local_main__2
   BNE   .inc_done@1
   INC   __local_main__1
.inc_done@1:
   LDA   __local_main__0
   LDY   #$00
   STA   (gp8),Y
   LDA   __local_main__3
   INY
   STA   (gp8),Y
   LDA   __local_main__2
   INY
   STA   (gp8),Y
   LDA   __local_main__1
   INY
   STA   (gp8),Y
   LDA   __local_main__0
   AND   #$02
   BNE   .lb_skip@2
   JMP   .if_end@13
.lb_skip@2:
   LDA   __local_main__l23
   CLC
   ADC   #$05
   STA   __local_main__l23
   SEC
   SBC   ga5+2
   STA   gs6+3
   LDA   #$00
   SBC   #$00
   STA   gs6+4
   LDY   #$00
   LDA   (gp8),Y
   STA   __local_main__0
   INY
   LDA   (gp8),Y
   STA   __local_main__1
   DEY
   LDA   (gp8),Y
   AND   __local_main__0
   STA   __local_main__0
   INY
   LDA   (gp8),Y
   AND   __local_main__1
   TAX
   LDA   __local_main__0
   JSR   f16
   JSR   f16@run1
   LDA   z4
   AND   #$3F
   STA   __local_main__2
   LDX   __local_main__l21
   LDA   __local_main__4
   LDA   ga5+1
   JSR   main@run3
   LDA   __local_main__2
   JSR   f9
   STA   __local_main__l21
   TXA
   STA   __local_main__4
   LDA   z4
   ASL
   ASL
   CLC
   ADC   z4
   AND   #$3F
   STA   z4
.if_end@13:
   LDY   #$00
   LDA   (gp8),Y
   AND   #$81
   STA   (gp8),Y
   LDA   #$00
   INY
   STA   (gp8),Y
   INY
   STA   (gp8),Y
   INY
   STA   (gp8),Y
   JMP   .if_end@11
.if_else@12:
   LDA   gs6+1
   STA   __local_main__0
   LDA   gs6+2
   STA   __local_main__1
   INC   __local_main__0
   BNE   .inc_done@2
   INC   __local_main__1
.inc_done@2:
   LDA   __local_main__0
   STA   gs6+1
   STA   gs6+3
   LDA   __local_main__1
   STA   gs6+2
   STA   gs6+4
.if_end@11:
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

   STA   __zpabi_f9__p12
   LDA   #<__local_main__l23
   STA   __zpabi_f9__pp13_0
   LDA   #>__local_main__l23
   STA   __zpabi_f9__pp13_1
   RTS

acc:
   DS.B  2

ga5:
   DC.B  $00, $00, $2B, $FF

gs6:
   DS.B  1
   DC.W  $0002
   DC.W  $0080

__zp_init:
   SUBROUTINE

   LDA   #$00
   STA   g1
   STA   g1+2
   STA   g1+3
   STA   g1+4
   STA   g1+5
   STA   g1+6
   STA   z3+1
   STA   z3+2
   STA   z3+3
   LDA   #$80
   STA   g1+1
   LDA   #$0A
   STA   g2
   LDA   #$01
   STA   z3
   LDA   #$07
   STA   z4
   LDA   #<g2
   STA   gp7
   LDA   #>g2
   STA   gp7+1
   LDA   #<z3
   STA   gp8
   LDA   #>z3
   STA   gp8+1
   RTS
