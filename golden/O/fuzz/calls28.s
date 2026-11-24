SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
g2	EQU	$26
g0	EQU	$28
__zpabi_f0__p0_0	EQU	$80
__zpabi_f0__p0_1	EQU	$81
__zpabi_f0__p0_2	EQU	$82
__zpabi_f0__pp_0	EQU	$83
__zpabi_f0__pp_1	EQU	$84
__zpabi_f1__p1_0	EQU	$85
__zpabi_f2__p0_0	EQU	$85
__zpabi_f1__p1_1	EQU	$86
__zpabi_f2__p0_1	EQU	$86
__zpabi_f1__p2	EQU	$87
__zpabi_f2__p0_2	EQU	$87
__zpabi_f1__p3_0	EQU	$88
__zpabi_f2__p0_3	EQU	$88
__zpabi_f1__p3_1	EQU	$89
__zpabi_f2__p1_0	EQU	$89
__zpabi_f1__p3_2	EQU	$8A
__zpabi_f2__p1_1	EQU	$8A
__zpabi_f1__pp_0	EQU	$8B
__zpabi_f1__pp_1	EQU	$8C
__local_main__0	EQU	$8D
__local_main__1	EQU	$8E
__local_main__2	EQU	$8F
__local_main__3	EQU	$90
__local_main__4	EQU	$91
__local_main__5	EQU	$92
__local_main__6	EQU	$93
__local_main__7	EQU	$94
__local_main__8	EQU	$95
__local_main__mbox	EQU	$96
__local_f0__0	EQU	$9A
__local_f0__1	EQU	$9B
__local_f0__3	EQU	$9D
__local_f0__5	EQU	$9F
__local_f0__6	EQU	$A0
__local_f0__7	EQU	$A1
__local_f0__box1	EQU	$A2
__local_f2__0	EQU	$A5
__local_f1__1	EQU	$A6
__local_f2__1	EQU	$A6
__local_f1__2	EQU	$A7
__local_f1__3	EQU	$A8
__local_f2__4	EQU	$A9
__local_f2__5	EQU	$AA
__local_f2__p2_1	EQU	$AB
__local_f2__p2_0	EQU	$AC

; @zp-link-meta-begin
; def f0 params=__zpabi_f0__p0_0,__zpabi_f0__p0_1,__zpabi_f0__p0_2,__zpabi_f0__pp_0,__zpabi_f0__pp_1 locals=__local_f0__0,__local_f0__1,__local_f0__2,__local_f0__3,__local_f0__4,__local_f0__5,__local_f0__6,__local_f0__7,__local_f0__box1,__local_f0__9,__local_f0__10 indirect=false in_cycle=false carry=true
; def f1 params=__zpabi_f1__p0_0,__zpabi_f1__p0_1,__zpabi_f1__p1_0,__zpabi_f1__p1_1,__zpabi_f1__p2,__zpabi_f1__p3_0,__zpabi_f1__p3_1,__zpabi_f1__p3_2,__zpabi_f1__pp_0,__zpabi_f1__pp_1 locals=__local_f1__0,__local_f1__1,__local_f1__2,__local_f1__3 indirect=false in_cycle=false param_regs=A,X,,,,,,,, carry=true
; def f2 params=__zpabi_f2__p0_0,__zpabi_f2__p0_1,__zpabi_f2__p0_2,__zpabi_f2__p0_3,__zpabi_f2__p1_0,__zpabi_f2__p1_1,__zpabi_f2__p2_0,__zpabi_f2__p2_1 locals=__local_f2__0,__local_f2__1,__local_f2__2,__local_f2__3,__local_f2__4,__local_f2__5,__local_f2__p2_1,__local_f2__p2_0 indirect=false in_cycle=false param_regs=,,,,,,A,X
; def main params= locals=__local_main__0,__local_main__1,__local_main__2,__local_main__3,__local_main__4,__local_main__5,__local_main__6,__local_main__7,__local_main__8,__local_main__mbox,__local_main__10,__local_main__11,__local_main__12 indirect=false in_cycle=false
; call f0 -> f1
; call f0 -> f2
; call main -> f0
; item f2 section=.text local=false zero=false
; item f1 section=.text local=false zero=false
; item f1@tail0 section=.text local=true zero=false
; item f0 section=.text local=false zero=false
; item main section=.text local=false zero=false
; item g1 section=.data local=false zero=false
; item __zp_init section=.text local=false zero=false
; zp g2 size=2 at=auto def=true init=$A3,$00 weight=8
; zp g0 size=2 at=auto def=true init=$8B,$00 weight=6
; runtime hargs_size=32 hargs_need=20
; @zp-link-meta-end

f2:
   SUBROUTINE

   STA   __local_f2__p2_0
   STX   __local_f2__p2_1
   TXA
   BMI   .sx_neg@0
   LDA   #$00
   BEQ   .sx_done@1
.sx_neg@0:
   LDA   #$FF
.sx_done@1:
   STA   __local_f2__5
   STA   __local_f2__4
   LDA   g0
   ORA   g0+1
   BEQ   .and_false@0
   LDA   g0+1
   BMI   .sx_neg@2
   LDA   #$00
   BEQ   .sx_done@3
.sx_neg@2:
   LDA   #$FF
.sx_done@3:
   STA   __local_f2__1
   STA   __local_f2__0
   LDA   __local_f2__p2_0
   SEC
   SBC   g0
   TXA
   SBC   g0+1
   LDA   __local_f2__5
   SBC   __local_f2__1
   LDA   __local_f2__4
   SBC   __local_f2__0
   BVC   .jcmp_novf@4
   EOR   #$80
.jcmp_novf@4:
   BPL   .and_end@1
.and_false@0:
.and_end@1:
   LDA   g2
   ORA   g2+1
   BEQ   .and_false@2
   LDA   #$00
   CMP   __local_f2__p2_1
   BNE   .jcmp_skip@5
   LDA   #$01
   CMP   __local_f2__p2_0
   BEQ   .and_false@2
.jcmp_skip@5:
   LDA   #$01
   RTS
.and_false@2:
   LDA   #$00
   RTS

f1:
   SUBROUTINE

   LDY   #$00
   LDA   (__zpabi_f1__pp_0),Y
   STA   __local_f1__2
   INY
   LDA   (__zpabi_f1__pp_0),Y
   STA   __local_f1__1
   LDA   __zpabi_f1__p2
   BEQ   .lnot_true@6
   LDA   #$00
   BEQ   .lnot_end@7
.lnot_true@6:
   LDA   #$01
.lnot_end@7:
   CLC
   ADC   __local_f1__2
   BCC   .carry_inc@0
   INC   __local_f1__1
.carry_inc@0:
   LDY   #$00
   STA   (__zpabi_f1__pp_0),Y
   LDA   __local_f1__1
   INY
   STA   (__zpabi_f1__pp_0),Y
   LDA   #$00
   SEC
   SBC   g2
   STA   __local_f1__3
   LDA   g2
   SBC   g2+1
   STA   __local_f1__2
   LDA   __zpabi_f1__p1_0
   BEQ   .and_false@4
   LDA   #$01
   STA   __local_f1__1
   JMP   .and_end@5
.and_false@4:
   LDA   #$00
   STA   __local_f1__1
.and_end@5:
   LDA   __local_f1__3
   STA   HARGS
   LDA   __local_f1__2
   STA   HARGS+1
   LDA   __local_f1__1
   STA   HARGS+2
   LDA   #$00
   STA   HARGS+3
   JSR   mul16
   LDA   HARGS+5
   ORA   HARGS+4
   JMP   f1@tail0

f1@tail0:
   SUBROUTINE

   BEQ   .cmp_end@8
   SEC
   RTS
.cmp_end@8:
   CMP   #$01
   RTS

f0:
   SUBROUTINE

   LDA   __zpabi_f0__p0_0
   ORA   #$01
   STA   HARGS+4
   LDA   __zpabi_f0__p0_1
   STA   HARGS+5
   LDA   __zpabi_f0__p0_2
   STA   HARGS+6
   LDA   #$01
   STA   HARGS
   LDA   #$00
   STA   HARGS+1
   LDY   #$00
   STY   HARGS+2
   STY   HARGS+3
   STA   HARGS+7
   JSR   udivmod32
   LDA   g0+1
   BMI   .sx_neg@9
   LDA   #$00
   BEQ   .sx_done@10
.sx_neg@9:
   LDA   #$FF
.sx_done@10:
   AND   #$07
   STA   __local_f0__0
   LDA   __zpabi_f0__p0_2
   CMP   __local_f0__0
   BNE   .cmp_true@11
   LDA   __zpabi_f0__p0_1
   CMP   g0+1
   BNE   .cmp_true@11
   LDA   __zpabi_f0__p0_0
   CMP   g0
   BNE   .cmp_true@11
   LDA   #$00
   BEQ   .cmp_end@12
.cmp_true@11:
   LDA   #$01
.cmp_end@12:
   CLC
   ADC   HARGS+12
   STA   __local_f0__7
   LDA   g2
   STA   __local_f0__6
   LDA   g2+1
   AND   #$7F
   STA   __local_f0__5
   LDY   #$01
   LDA   (__zpabi_f0__pp_0),Y
   STA   __local_f0__1
   DEY
   LDA   (__zpabi_f0__pp_0),Y
   CLC
   BCC   .carry_inc@1
   INC   __local_f0__1
.carry_inc@1:
   LDY   #$00
   STA   (__zpabi_f0__pp_0),Y
   LDA   __local_f0__1
   INY
   STA   (__zpabi_f0__pp_0),Y
   LDA   __local_f0__7
   STA   __local_f0__box1
   DEY
   STY   __local_f0__box1+1
   EOR   #$FF
   STA   __zpabi_f1__p3_0
   TYA
   EOR   #$7F
   STA   __local_f0__1
   BMI   .sx_neg@18
   TYA
   JMP   .sx_done@19
.sx_neg@18:
   LDA   #$FF
.sx_done@19:
   LDX   __local_f0__1
   STX   __zpabi_f1__p3_1
   AND   #$07
   STA   __zpabi_f1__p3_2
   LDA   g1
   STA   __zpabi_f1__p1_0
   LDA   g1+1
   STA   __zpabi_f1__p1_1
   LDA   #$01
   STA   __zpabi_f1__p2
   LDA   #<__local_f0__box1
   STA   __zpabi_f1__pp_0
   LDA   #>__local_f0__box1
   STA   __zpabi_f1__pp_1
   LDX   __local_f0__5
   LDA   __local_f0__6
   JSR   f1
   LDA   #$00
   ROL
   STA   __local_f0__0
   LDA   __local_f0__6
   EOR   __local_f0__0
   STA   __local_f0__1
   LDA   __local_f0__5
   AND   #$7F
   STA   __local_f0__0
   LDA   __local_f0__1
   CLC
   ADC   __local_f0__box1
   STA   __local_f0__1
   LDA   __local_f0__0
   ADC   __local_f0__box1+1
   LDX   __local_f0__1
   STX   __local_f0__3
   AND   #$7F
   STA   __zpabi_f1__p3_0
   LDA   g2
   CLC
   ADC   __local_f0__3
   STA   g2
   LDA   g2+1
   ADC   __zpabi_f1__p3_0
   STA   g2+1
   SEC
   LDA   #$00
   SBC   __local_f0__3
   STA   __local_f0__1
   LDA   #$00
   SBC   __zpabi_f1__p3_0
   AND   #$7F
   STA   __local_f0__0
   LDA   __local_f0__3
   SEC
   SBC   g1
   LDA   __zpabi_f1__p3_0
   SBC   g1+1
   BVC   .cmp_novf@20
   EOR   #$80
.cmp_novf@20:
   ASL
   LDA   #$00
   ROL
   STA   __zpabi_f2__p1_0
   LDA   __local_f0__1
   STA   __zpabi_f2__p0_0
   LDA   __local_f0__0
   STA   __zpabi_f2__p0_1
   LDA   #$00
   STA   __zpabi_f2__p0_2
   STA   __zpabi_f2__p0_3
   STA   __zpabi_f2__p1_1
   LDX   #$FC
   LDA   #$18
   JSR   f2
   STA   __local_f0__0
   LDA   #$E8
   EOR   __local_f0__0
   STA   __local_f0__1
   BMI   .sx_neg@21
   LDA   #$00
   BEQ   .sx_done@22
.sx_neg@21:
   LDA   #$FF
.sx_done@22:
   STA   __local_f0__0
   LDA   g2
   CLC
   ADC   __local_f0__1
   STA   g2
   LDA   g2+1
   ADC   __local_f0__0
   STA   g2+1
   LDA   __local_f0__7
   LDX   #$FF
   SEC
.div_loop@23:
   INX
   SBC   #$07
   BCS   .div_loop@23
   TXA
   EOR   #$FF
   STA   __local_f0__1
   LDA   #$00
   EOR   #$FF
   ORA   __local_f0__1
   JMP   f1@tail0

main:
   SUBROUTINE

   LDA   #$05
   STA   __local_main__mbox
   LDA   #$00
   STA   __local_main__mbox+1
   LDA   #$2A
   STA   __zpabi_f0__p0_0
   LDA   #$00
   STA   __zpabi_f0__p0_1
   STA   __zpabi_f0__p0_2
   LDA   #<__local_main__mbox
   STA   __zpabi_f0__pp_0
   LDA   #>__local_main__mbox
   STA   __zpabi_f0__pp_1
   JSR   f0
   LDA   #$00
   ROL
   STA   __local_main__8
   LDA   g0+1
   BMI   .sx_neg@25
   LDA   #$00
   BEQ   .sx_done@26
.sx_neg@25:
   LDA   #$FF
.sx_done@26:
   STA   __local_main__5
   STA   __local_main__4
   LDA   g1+1
   BMI   .sx_neg@27
   LDA   #$00
   BEQ   .sx_done@28
.sx_neg@27:
   LDA   #$FF
.sx_done@28:
   STA   __local_main__1
   STA   __local_main__0
   LDA   g0
   EOR   g1
   STA   __local_main__7
   LDA   g0+1
   EOR   g1+1
   STA   __local_main__6
   LDA   __local_main__5
   EOR   __local_main__1
   STA   __local_main__5
   LDA   __local_main__4
   EOR   __local_main__0
   STA   __local_main__4
   LDA   g2+1
   BMI   .sx_neg@29
   LDA   #$00
   BEQ   .sx_done@30
.sx_neg@29:
   LDA   #$FF
.sx_done@30:
   STA   __local_main__1
   STA   __local_main__0
   LDA   __local_main__7
   EOR   g2
   STA   __local_main__3
   LDA   __local_main__6
   EOR   g2+1
   STA   __local_main__2
   LDA   __local_main__5
   EOR   __local_main__1
   STA   __local_main__1
   LDA   __local_main__4
   EOR   __local_main__0
   STA   __local_main__0
   LDA   __local_main__8
   EOR   __local_main__3
   STA   __local_main__3
   LDA   #$00
   EOR   __local_main__2
   STA   __local_main__2
   LDA   #$00
   EOR   __local_main__1
   STA   HARGS+2
   LDA   #$00
   EOR   __local_main__0
   STA   HARGS+3
   LDA   __local_main__3
   STA   HARGS
   LDA   __local_main__2
   STA   HARGS+1
   LDA   #$10
   STA   HARGS+4
   JSR   asr32
   LDA   __local_main__3
   EOR   HARGS+5
   STA   __local_main__0
   LDA   __local_main__2
   EOR   HARGS+6
   TAX
   LDA   __local_main__0
   RTS

g1:
   DC.W  $0039

__zp_init:
   SUBROUTINE

   LDA   #$00
   STA   g0+1
   STA   g2+1
   LDA   #$8B
   STA   g0
   LDA   #$A3
   STA   g2
   RTS
