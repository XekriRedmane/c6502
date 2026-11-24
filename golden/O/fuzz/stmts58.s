SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
z3	EQU	$26
z4	EQU	$28
g2	EQU	$2A
__zpabi_f7__p9_0	EQU	$80
__zpabi_f7__p9_1	EQU	$81
__zpabi_f7__pp10_0	EQU	$82
__zpabi_f7__pp10_1	EQU	$83
__zpabi_mix__v_0	EQU	$84
__zpabi_mix__v_1	EQU	$85
__zpabi_mix__v_2	EQU	$86
__zpabi_mix__v_3	EQU	$87
__local_f7__0	EQU	$88
__local_main__w18	EQU	$88
__local_f7__1	EQU	$89
__local_main__0	EQU	$89
__local_f7__2	EQU	$8A
__local_main__i19	EQU	$8A
__local_f7__3	EQU	$8B
__local_main__1	EQU	$8B
__local_f7__4	EQU	$8C
__local_f7__5	EQU	$8D
__local_f7__6	EQU	$8E
__local_f7__7	EQU	$8F
__local_f7__l11	EQU	$94
__local_mix__0	EQU	$95
__local_mix__1	EQU	$96
__local_mix__2	EQU	$97
__local_mix__3	EQU	$98

; @zp-link-meta-begin
; def f7 params=__zpabi_f7__p8,__zpabi_f7__p9_0,__zpabi_f7__p9_1,__zpabi_f7__pp10_0,__zpabi_f7__pp10_1 locals=__local_f7__0,__local_f7__1,__local_f7__2,__local_f7__3,__local_f7__4,__local_f7__5,__local_f7__6,__local_f7__7,__local_f7__8,__local_f7__9,__local_f7__10,__local_f7__11,__local_f7__l11 indirect=false in_cycle=false param_regs=A,,,,
; def fin0 params= locals=__local_fin0__0,__local_fin0__1 indirect=false in_cycle=false
; def fin1 params= locals=__local_fin1__0,__local_fin1__1 indirect=false in_cycle=false
; def fin2 params= locals=__local_fin2__0 indirect=false in_cycle=false
; def main params= locals=__local_main__w18,__local_main__0,__local_main__i19,__local_main__1,__local_main__2,__local_main__3,__local_main__4,__local_main__5,__local_main__6 indirect=false in_cycle=false
; def mix params=__zpabi_mix__v_0,__zpabi_mix__v_1,__zpabi_mix__v_2,__zpabi_mix__v_3 locals=__local_mix__0,__local_mix__1,__local_mix__2,__local_mix__3 indirect=false in_cycle=false
; call f7 -> mix
; call fin0 -> mix
; call fin1 -> mix
; call fin2 -> mix
; call main -> fin0
; call main -> fin1
; call main -> fin2
; call main -> mix
; item mix section=.text local=false zero=false
; item fin0 section=.text local=false zero=false
; item fin0@run3 section=.text local=true zero=false
; item fin0@run1 section=.text local=true zero=false
; item fin0@run0 section=.text local=true zero=false
; item fin0@run0@run4 section=.text local=true zero=false
; item fin1 section=.text local=false zero=false
; item fin2 section=.text local=false zero=false
; item f7 section=.text local=false zero=false
; item f7@run2 section=.text local=true zero=false
; item main section=.text local=false zero=false
; item acc section=.bss local=false zero=true
; item g1 section=.data local=false zero=false
; item ga5 section=.data local=false zero=false
; item gs6 section=.data local=false zero=false
; item @4.s14 section=.data local=true zero=false
; item __zp_init section=.text local=false zero=false
; zp z3 size=2 at=auto def=true init=$00,$80
; zp z4 size=2 at=auto def=true init=$2F,$91
; zp g2 size=2 at=auto def=true init=$0D,$00 weight=20
; runtime hargs_size=32 hargs_need=10
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
   JSR   fin0@run3
   LDA   z4
   STA   __zpabi_mix__v_0
   LDA   z4+1
   STA   __zpabi_mix__v_1
   BMI   .sx_neg@2
   LDA   #$00
   BEQ   .sx_done@3
.sx_neg@2:
   LDA   #$FF
.sx_done@3:
   STA   __zpabi_mix__v_2
   STA   __zpabi_mix__v_3
   JMP   mix

fin0@run3:
   SUBROUTINE

   LDA   z3
   STA   __zpabi_mix__v_0
   LDA   z3+1
   JMP   fin0@run0

fin0@run1:
   SUBROUTINE

   STA   __zpabi_mix__v_0
   LDA   #$00
   STA   __zpabi_mix__v_1
   STA   __zpabi_mix__v_2
   STA   __zpabi_mix__v_3
   JMP   mix

fin0@run0:
   SUBROUTINE

   JSR   fin0@run0@run4
   JMP   mix

fin0@run0@run4:
   SUBROUTINE

   STA   __zpabi_mix__v_1
   LDA   #$00
   STA   __zpabi_mix__v_2
   STA   __zpabi_mix__v_3
   RTS

fin1:
   SUBROUTINE

   LDA   ga5
   STA   __zpabi_mix__v_0
   LDA   ga5+1
   JSR   fin0@run0
   LDA   ga5+2
   STA   __zpabi_mix__v_0
   LDA   ga5+3
   JSR   fin0@run0
   LDA   ga5+4
   STA   __zpabi_mix__v_0
   LDA   ga5+5
   JSR   fin0@run0
   LDA   ga5+6
   STA   __zpabi_mix__v_0
   LDA   ga5+7
   JSR   fin0@run0@run4
   JMP   mix

fin2:
   SUBROUTINE

   LDA   gs6+1
   JSR   fin0@run1
   LDA   gs6+2
   BMI   .sx_neg@4
   LDA   #$00
   BEQ   .sx_done@5
.sx_neg@4:
   LDA   #$FF
.sx_done@5:
   STA   __zpabi_mix__v_1
   STA   __zpabi_mix__v_2
   STA   __zpabi_mix__v_3
   LDA   gs6+2
   STA   __zpabi_mix__v_0
   JMP   mix

f7:
   SUBROUTINE

   LDA   #$FF
   STA   __local_f7__l11
   LDY   #$00
   LDA   (__zpabi_f7__pp10_0),Y
   STA   __zpabi_mix__v_0
   INY
   LDA   (__zpabi_f7__pp10_0),Y
   STA   __zpabi_mix__v_1
   INY
   LDA   (__zpabi_f7__pp10_0),Y
   STA   __zpabi_mix__v_2
   INY
   LDA   (__zpabi_f7__pp10_0),Y
   STA   __zpabi_mix__v_3
   INY
   LDA   (__zpabi_f7__pp10_0),Y
   STA   __local_f7__7
   INY
   LDA   (__zpabi_f7__pp10_0),Y
   STA   __local_f7__6
   INY
   LDA   (__zpabi_f7__pp10_0),Y
   STA   __local_f7__5
   INY
   LDA   (__zpabi_f7__pp10_0),Y
   STA   __local_f7__4
   LDA   __zpabi_mix__v_0
   CLC
   ADC   #$01
   STA   __local_f7__3
   LDA   __zpabi_mix__v_1
   ADC   #$00
   STA   __local_f7__2
   LDA   __zpabi_mix__v_2
   ADC   #$00
   STA   __local_f7__1
   LDA   __zpabi_mix__v_3
   ADC   #$00
   STA   __local_f7__0
   BCC   .carry_inc@0
   INC   __local_f7__7
   BNE   .carry_inc@0
   INC   __local_f7__6
   BNE   .carry_inc@0
   INC   __local_f7__5
   BNE   .carry_inc@0
   INC   __local_f7__4
.carry_inc@0:
   LDA   __local_f7__3
   LDY   #$00
   STA   (__zpabi_f7__pp10_0),Y
   LDA   __local_f7__2
   INY
   STA   (__zpabi_f7__pp10_0),Y
   LDA   __local_f7__1
   INY
   STA   (__zpabi_f7__pp10_0),Y
   LDA   __local_f7__0
   INY
   STA   (__zpabi_f7__pp10_0),Y
   LDA   __local_f7__7
   INY
   STA   (__zpabi_f7__pp10_0),Y
   LDA   __local_f7__6
   INY
   STA   (__zpabi_f7__pp10_0),Y
   LDA   __local_f7__5
   INY
   STA   (__zpabi_f7__pp10_0),Y
   LDA   __local_f7__4
   INY
   STA   (__zpabi_f7__pp10_0),Y
   JSR   mix
   INC   gs6+2
   LDA   z4
   LDX   z4+1
   STX   __local_f7__2
   SEC
   SBC   #$03
   STA   __local_f7__0
   BCS   .carry_inc@1
   DEC   __local_f7__2
.carry_inc@1:
   STA   z4
   LDA   __local_f7__2
   STA   z4+1
   LDA   g2
   CLC
   ADC   __local_f7__0
   STA   __local_f7__1
   LDA   g2+1
   ADC   __local_f7__2
   ORA   __local_f7__1
   BEQ   .if_else@1
   LDA   gs6+1
   JSR   fin0@run1
   INC   gs6+2
   LDA   gs6+2
   STA   @4.s14
   BMI   .sx_neg@6
   LDA   #$00
   BEQ   .sx_done@7
.sx_neg@6:
   LDA   #$FF
.sx_done@7:
   STA   @4.s14+1
   JMP   .if_end@0
.if_else@1:
   LDA   __local_f7__l11
   STA   __local_f7__7
   LDY   #$00
   LDA   (__zpabi_f7__pp10_0),Y
   STA   __local_f7__3
   INY
   LDA   (__zpabi_f7__pp10_0),Y
   STA   __local_f7__6
   INY
   LDA   (__zpabi_f7__pp10_0),Y
   STA   __local_f7__5
   INY
   LDA   (__zpabi_f7__pp10_0),Y
   STA   __local_f7__4
   LDA   __local_f7__7
   BMI   .sx_neg@8
   LDA   #$00
   BEQ   .sx_done@9
.sx_neg@8:
   LDA   #$FF
.sx_done@9:
   STA   __local_f7__2
   STA   __local_f7__1
   STA   __local_f7__0
   LDA   __local_f7__7
   ORA   __local_f7__3
   STA   __local_f7__3
   LDA   __local_f7__2
   ORA   __local_f7__6
   STA   __local_f7__2
   LDA   __local_f7__1
   ORA   __local_f7__5
   STA   __local_f7__1
   LDA   __local_f7__0
   ORA   __local_f7__4
   STA   __local_f7__0
   LDA   #$FF
   CLC
   ADC   __local_f7__3
   STA   __zpabi_mix__v_0
   LDA   #$7F
   ADC   __local_f7__2
   STA   __zpabi_mix__v_1
   LDA   #$00
   ADC   __local_f7__1
   STA   __zpabi_mix__v_2
   LDA   #$00
   ADC   __local_f7__0
   STA   __zpabi_mix__v_3
   JSR   mix
   LDA   gs6+1
   SEC
   SBC   #$5A
   STA   gs6+1
.if_end@0:
   LDA   #$00
   STA   __zpabi_mix__v_0
   JSR   f7@run2
   LDX   #$FF
   TXA
   RTS

f7@run2:
   SUBROUTINE

   LDA   #$FF
   STA   __zpabi_mix__v_1
   STA   __zpabi_mix__v_2
   STA   __zpabi_mix__v_3
   JMP   mix

main:
   SUBROUTINE

   LDA   gs6+1
   AND   #$07
   CMP   #$01
   BNE   .lb_skip@2
   JMP   .switch@0_break
.lb_skip@2:
   CMP   #$03
   BEQ   .case@2
   CMP   #$05
   BNE   .lb_skip@1
   JMP   .case@5
.lb_skip@1:
   CMP   #$07
   BNE   .lb_skip@0
   JMP   .case@12
.lb_skip@0:
   JMP   .switch@0_break
.case@2:
   LDA   #$00
   STA   __local_main__w18
   JMP   .loop@3_continue
.main@rot@1:
   DEC   gs6+1
   LDA   __local_main__1
   AND   #$03
   ASL
   CLC
   TAX
   LDA   ga5,X
   AND   #$03
   STA   __local_main__0
   LDA   #$00
   STA   ga5+1,X
   LDA   __local_main__0
   STA   ga5,X
   ADC   #$80
   STA   __zpabi_mix__v_0
   LDA   #$00
   ADC   #$FF
   JSR   fin0@run0
   LDA   g2
   SEC
   SBC   #$03
   STA   g2
   BCS   .carry_inc@2
   DEC   g2+1
.carry_inc@2:
   LDA   #$01
   STA   __local_main__i19
.main@rot@0:
   LDA   ga5+2
   ORA   #$5A
   STA   ga5+2
   DEC   __local_main__i19
   BNE   .main@rot@0
   LDA   __local_main__1
   STA   __local_main__w18
.loop@3_continue:
   CLC
   ADC   #$01
   STA   __local_main__1
   LDA   __local_main__w18
   CMP   #$04
   BCC   .main@rot@1
   LDA   #$FE
   STA   __zpabi_mix__v_0
   JSR   f7@run2
.case@5:
   LDA   g2
   STA   HARGS
   LDA   g2+1
   STA   HARGS+1
   LDA   #$5A
   STA   HARGS+2
   LDA   #$00
   STA   HARGS+3
   JSR   mul16
   LDA   HARGS+4
   STA   g2
   LDA   HARGS+5
   STA   g2+1
   ORA   g2
   BEQ   .if_end@2
   LDA   gs6+2
   STA   __local_main__1
   BMI   .sx_neg@10
   LDA   #$00
   BEQ   .sx_done@11
.sx_neg@10:
   LDA   #$FF
.sx_done@11:
   AND   #$7F
   LDX   ga5+4
   STX   __local_main__0
   CMP   ga5+5
   BNE   .thread@0
   LDA   __local_main__1
   CMP   __local_main__0
   BEQ   .cmp_true@12
.thread@0:
   LDA   #$00
   BEQ   .cmp_end@13
.cmp_true@12:
   LDA   #$01
.cmp_end@13:
   AND   #$07
   CMP   #$03
   BEQ   .switch@6_break
   CMP   #$04
   BEQ   .case@8
   CMP   #$05
   BEQ   .case@9
   CMP   #$07
   BEQ   .case@10
   BNE   .default@11
.case@8:
   LDA   #$FF
   SEC
   SBC   ga5+6
   AND   #$02
   STA   z4
   LDA   #$00
   STA   z4+1
   JMP   .switch@6_break
.case@9:
   JSR   fin0@run3
.case@10:
.default@11:
.switch@6_break:
.if_end@2:
.case@12:
.switch@0_break:
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

g1:
   DC.B  $01

ga5:
   DC.W  $FFFF
   DC.W  $0100
   DC.W  $0080
   DC.W  $FFFF

gs6:
   DS.B  1
   DC.B  $02
   DC.B  $FF

@4.s14:
   DC.W  $0002

__zp_init:
   SUBROUTINE

   LDA   #$00
   STA   g2+1
   STA   z3
   LDA   #$0D
   STA   g2
   LDA   #$80
   STA   z3+1
   LDA   #$2F
   STA   z4
   LDA   #$91
   STA   z4+1
   RTS
