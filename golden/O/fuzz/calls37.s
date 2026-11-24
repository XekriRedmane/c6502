SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
g0	EQU	$26
g2	EQU	$2A
__zpabi_f0__p0_0	EQU	$80
__zpabi_f0__p0_1	EQU	$81
__zpabi_f0__p0_2	EQU	$82
__zpabi_f0__p0_3	EQU	$83
__zpabi_f0__p1_0	EQU	$84
__zpabi_f0__p1_1	EQU	$85
__zpabi_f1__p2	EQU	$86
__zpabi_f2__p1_0	EQU	$87
__zpabi_f2__p1_1	EQU	$88
__zpabi_f2__p1_2	EQU	$89
__zpabi_f2__p1_3	EQU	$8A
__local_main__0	EQU	$8B
__local_main__1	EQU	$8C
__local_main__2	EQU	$8D
__local_main__3	EQU	$8E
__local_main__4	EQU	$8F
__local_main__5	EQU	$90
__local_main__6	EQU	$91
__local_main__7	EQU	$92
__local_main__8	EQU	$93
__local_main__9	EQU	$94
__local_main__10	EQU	$95
__local_main__11	EQU	$96
__local_f0__it_1	EQU	$9B
__local_f0__2	EQU	$9C
__local_f0__it_0	EQU	$9D
__local_f0__l1	EQU	$9E
__local_f0__l2_0	EQU	$9F
__local_f0__3	EQU	$A0
__local_f0__4	EQU	$A1
__local_f0__5	EQU	$A2
__local_f0__6	EQU	$A3
__local_f0__7	EQU	$A4
__local_f0__p2_1	EQU	$A5
__local_f0__p2_0	EQU	$A6
__local_f1__l1	EQU	$AC
__local_f1__0	EQU	$AD
__local_f1__1	EQU	$AE
__local_f1__2	EQU	$AF
__local_f1__3	EQU	$B0
__local_f1__4	EQU	$B1
__local_f2__p0_1	EQU	$B2
__local_f2__p0_0	EQU	$B3
__local_f2__0	EQU	$B4
__local_f2__1	EQU	$B5

; @zp-link-meta-begin
; def f0 params=__zpabi_f0__p0_0,__zpabi_f0__p0_1,__zpabi_f0__p0_2,__zpabi_f0__p0_3,__zpabi_f0__p1_0,__zpabi_f0__p1_1,__zpabi_f0__p2_0,__zpabi_f0__p2_1 locals=__local_f0__0,__local_f0__1,__local_f0__it_1,__local_f0__2,__local_f0__it_0,__local_f0__l1,__local_f0__l2_0,__local_f0__3,__local_f0__4,__local_f0__5,__local_f0__6,__local_f0__7,__local_f0__p2_1,__local_f0__p2_0,__local_f0__8,__local_f0__9,__local_f0__10,__local_f0__11,__local_f0__12 indirect=false in_cycle=false param_regs=,,,,,,A,X
; def f1 params=__zpabi_f1__p0,__zpabi_f1__p1,__zpabi_f1__p2 locals=__local_f1__l1,__local_f1__0,__local_f1__1,__local_f1__2,__local_f1__3,__local_f1__4 indirect=false in_cycle=false param_regs=A,X,
; def f2 params=__zpabi_f2__p0_0,__zpabi_f2__p0_1,__zpabi_f2__p1_0,__zpabi_f2__p1_1,__zpabi_f2__p1_2,__zpabi_f2__p1_3 locals=__local_f2__p0_1,__local_f2__p0_0,__local_f2__0,__local_f2__1 indirect=false in_cycle=false param_regs=A,X,,,, carry=true
; def main params= locals=__local_main__0,__local_main__1,__local_main__2,__local_main__3,__local_main__4,__local_main__5,__local_main__6,__local_main__7,__local_main__8,__local_main__9,__local_main__10,__local_main__11,__local_main__12,__local_main__13 indirect=false in_cycle=false
; call f0 -> f1
; call f0 -> f2
; call f1 -> f2
; call main -> f0
; item f2 section=.text local=false zero=false
; item f1 section=.text local=false zero=false
; item f0 section=.text local=false zero=false
; item main section=.text local=false zero=false
; item g1 section=.data local=false zero=false
; item __zp_init section=.text local=false zero=false
; zp g0 size=4 at=auto def=true init=$BF,$00,$00,$00 weight=16
; zp g2 size=1 at=auto def=true init=$A1 weight=3
; runtime hargs_size=32 hargs_need=20
; @zp-link-meta-end

f2:
   SUBROUTINE

   STX   __local_f2__p0_1
   ORA   __local_f2__p0_1
   BNE   .or_true@0
   LDA   __zpabi_f2__p1_0
   ORA   __zpabi_f2__p1_1
   ORA   __zpabi_f2__p1_2
   ORA   __zpabi_f2__p1_3
   BNE   .or_true@0
   LDA   #$00
   STA   __local_f2__1
   JMP   .or_end@1
.or_true@0:
   LDA   #$01
   STA   __local_f2__1
   LDA   #$00
.or_end@1:
   ORA   #$00
   BPL   .sx_done@1
   LDA   #$FF
.sx_done@1:
   STA   __local_f2__p0_0
   STA   __local_f2__p0_1
   LDA   g1
   CLC
   ADC   __local_f2__1
   STA   __local_f2__1
   LDA   g1+1
   ADC   #$00
   STA   __local_f2__0
   LDA   g1+2
   ADC   __local_f2__p0_0
   STA   __local_f2__p0_0
   LDA   g1+3
   ADC   __local_f2__p0_1
   ORA   __local_f2__p0_0
   ORA   __local_f2__0
   ORA   __local_f2__1
   BEQ   .cmp_end@2
   SEC
   RTS
.cmp_end@2:
   CMP   #$01
   RTS

f1:
   SUBROUTINE

   STX   __local_f1__l1
   LDA   g1
   STA   __local_f1__4
   LDA   #$00
   SEC
   SBC   __local_f1__l1
   STA   __local_f1__l1
   LDA   __zpabi_f1__p2
   EOR   __local_f1__l1
   STA   __local_f1__3
   LDA   g1+3
   CMP   g0+3
   BNE   .thread@0
   LDA   g1+2
   CMP   g0+2
   BNE   .thread@0
   LDA   g1+1
   CMP   g0+1
   BNE   .thread@0
   LDA   g1
   CMP   g0
   BEQ   .cmp_true@3
.thread@0:
   LDA   #$00
   BEQ   .cmp_end@4
.cmp_true@3:
   LDA   #$01
.cmp_end@4:
   STA   __local_f1__2
   LDA   __local_f1__3
   BMI   .sx_neg@6
   LDA   #$00
   BEQ   .sx_done@7
.sx_neg@6:
   LDA   #$FF
.sx_done@7:
   STA   __local_f1__1
   STA   __local_f1__0
   STA   __local_f1__l1
   LDA   __local_f1__4
   SEC
   SBC   __local_f1__3
   STA   __zpabi_f2__p1_0
   LDA   #$00
   SBC   __local_f1__1
   STA   __zpabi_f2__p1_1
   LDA   #$00
   SBC   __local_f1__0
   STA   __zpabi_f2__p1_2
   LDA   #$00
   SBC   __local_f1__l1
   STA   __zpabi_f2__p1_3
   LDX   #$00
   LDA   __local_f1__2
   JSR   f2
   BCC   .if_else@3
   LDA   __local_f1__4
   CLC
   ADC   #$03
   STA   __local_f1__l1
   JMP   .if_end@2
.if_else@3:
   LDA   __local_f1__4
   EOR   #$05
   STA   __local_f1__l1
.if_end@2:
   LDA   #$07
   EOR   __local_f1__l1
   STA   __local_f1__3
   LDA   g0
   STA   HARGS
   LDA   g0+1
   STA   HARGS+1
   LDA   g0+2
   STA   HARGS+2
   LDA   g0+3
   STA   HARGS+3
   LDA   #$65
   STA   HARGS+4
   LDA   #$00
   STA   HARGS+5
   LDY   #$00
   STY   HARGS+6
   STA   HARGS+7
   JSR   udivmod32
   LDA   __local_f1__3
   CLC
   SBC   HARGS+8
   LDA   #$00
   SBC   HARGS+9
   LDA   #$00
   SBC   HARGS+10
   LDA   #$00
   SBC   HARGS+11
   LDA   #$00
   ROL
   LDX   #$00
   RTS

f0:
   SUBROUTINE

   STA   __local_f0__p2_0
   STX   __local_f0__p2_1
   LDA   g2
   BMI   .sx_neg@8
   LDA   #$00
   BEQ   .sx_done@9
.sx_neg@8:
   LDA   #$FF
.sx_done@9:
   STA   __zpabi_f2__p1_1
   BMI   .sx_neg@10
   LDA   #$00
   BEQ   .sx_done@11
.sx_neg@10:
   LDA   #$FF
.sx_done@11:
   STA   HARGS+2
   STA   HARGS+3
   LDA   g0
   ORA   #$01
   STA   HARGS+4
   LDA   g0+1
   STA   HARGS+5
   LDA   g0+2
   STA   HARGS+6
   LDA   g0+3
   STA   HARGS+7
   LDA   g2
   STA   HARGS
   LDA   __zpabi_f2__p1_1
   STA   HARGS+1
   JSR   udivmod32
   LDA   g0+3
   CMP   __zpabi_f0__p0_3
   BNE   .cmp_true@12
   LDA   g0+2
   CMP   __zpabi_f0__p0_2
   BNE   .cmp_true@12
   LDA   g0+1
   CMP   __zpabi_f0__p0_1
   BNE   .cmp_true@12
   LDA   g0
   CMP   __zpabi_f0__p0_0
   BNE   .cmp_true@12
   LDA   #$00
   BEQ   .cmp_end@13
.cmp_true@12:
   LDA   #$01
.cmp_end@13:
   STA   __local_f0__it_1
   LDA   #$00
   BPL   .sx_done@16
   LDA   #$FF
.sx_done@16:
   STA   __zpabi_f2__p1_0
   STA   __zpabi_f2__p1_1
   LDA   HARGS+12
   AND   __local_f0__it_1
   STA   __local_f0__7
   LDA   #$00
   STA   __local_f0__6
   LDA   HARGS+14
   AND   __zpabi_f2__p1_0
   STA   __local_f0__5
   LDA   HARGS+15
   AND   __zpabi_f2__p1_1
   STA   __local_f0__4
   SEC
   LDA   #$00
   SBC   __local_f0__p2_0
   ASL
   STA   __local_f0__l1
   LDA   g1
   SEC
   SBC   #$E8
   LDA   #$00
   SBC   #$03
   LDA   #$00
   ROL
   STA   __local_f0__3
   LDA   __zpabi_f0__p1_0
   STA   __local_f0__l2_0
   LDA   __zpabi_f0__p1_1
   STA   __local_f0__it_0
   LDA   #$00
   STA   __local_f0__2
   LDY   #$00
   STY   __local_f0__it_1
.loop@0_start:
   LDA   __local_f0__2
   SEC
   SBC   #$03
   LDA   __local_f0__it_1
   SBC   #$00
   BVC   .jcmp_novf@17
   EOR   #$80
.jcmp_novf@17:
   BPL   .loop@0_break
   LDA   g0
   STA   HARGS+1
   LDA   __local_f0__l2_0
   STA   HARGS
   JSR   mul8
   LDX   HARGS+2
   LDA   __local_f0__l1
   STA   __zpabi_f1__p2
   LDA   #$07
   JSR   f1
   CLC
   ADC   __local_f0__2
   STA   __zpabi_f2__p1_0
   TXA
   ADC   __local_f0__it_1
   STA   __zpabi_f2__p1_1
   LDA   __local_f0__l2_0
   CLC
   ADC   __zpabi_f2__p1_0
   STA   __local_f0__l2_0
   LDA   __local_f0__it_0
   ADC   __zpabi_f2__p1_1
   STA   __local_f0__it_0
   INC   __local_f0__2
   BNE   .loop@0_start
   INC   __local_f0__it_1
   JMP   .loop@0_start
.loop@0_break:
   LDA   g0
   CLC
   ADC   __local_f0__l2_0
   STA   g0
   LDA   g0+1
   ADC   __local_f0__it_0
   STA   g0+1
   BCC   .carry_inc@0
   INC   g0+2
   BNE   .carry_inc@0
   INC   g0+3
.carry_inc@0:
   LDA   #$00
   STA   __local_f0__it_0
   LDY   #$00
   STY   __local_f0__2
.loop@1_start:
   LDA   __local_f0__it_0
   SEC
   SBC   #$03
   LDA   __local_f0__2
   SBC   #$00
   BVC   .jcmp_novf@18
   EOR   #$80
.jcmp_novf@18:
   BPL   .loop@1_break
   LDA   __local_f0__p2_0
   ORA   #$01
   STA   HARGS+2
   LDA   __local_f0__p2_1
   STA   HARGS+3
   LDA   __local_f0__l1
   STA   HARGS
   LDA   #$00
   STA   HARGS+1
   JSR   udivmod16
   LDX   HARGS+7
   LDA   #$E8
   CLC
   ADC   __zpabi_f0__p1_0
   STA   __zpabi_f2__p1_0
   LDA   #$03
   ADC   __zpabi_f0__p1_1
   STA   __zpabi_f2__p1_1
   BMI   .sx_neg@19
   LDA   #$00
   BEQ   .sx_done@20
.sx_neg@19:
   LDA   #$FF
.sx_done@20:
   STA   __zpabi_f2__p1_2
   STA   __zpabi_f2__p1_3
   LDA   HARGS+6
   JSR   f2
   LDA   #$00
   ROL
   CLC
   ADC   __local_f0__it_0
   CLC
   ADC   __local_f0__l1
   STA   __local_f0__l1
   INC   __local_f0__it_0
   BNE   .loop@1_start
   INC   __local_f0__2
   JMP   .loop@1_start
.loop@1_break:
   LDA   #$00
   BPL   .sx_done@22
   LDA   #$FF
.sx_done@22:
   STA   __zpabi_f2__p1_0
   STA   __zpabi_f2__p1_1
   LDA   __local_f0__4
   CMP   __zpabi_f2__p1_1
   BNE   .cmp_true@23
   LDA   __local_f0__5
   CMP   __zpabi_f2__p1_0
   BNE   .cmp_true@23
   LDA   __local_f0__6
   BNE   .cmp_true@23
   LDA   __local_f0__7
   CMP   __local_f0__3
   BNE   .cmp_true@23
   LDA   #$00
   BEQ   .cmp_end@24
.cmp_true@23:
   LDA   #$01
.cmp_end@24:
   SEC
   SBC   #$03
   LDA   #$00
   SBC   #$00
   BVC   .cmp_novf@26
   EOR   #$80
.cmp_novf@26:
   ASL
   LDA   #$00
   ROL
   EOR   #$01
   RTS

main:
   SUBROUTINE

   LDA   #$05
   STA   __zpabi_f0__p0_0
   LDA   #$00
   STA   __zpabi_f0__p0_2
   STA   __zpabi_f0__p0_3
   LDA   #$2C
   STA   __zpabi_f0__p1_0
   LDA   #$01
   STA   __zpabi_f0__p0_1
   STA   __zpabi_f0__p1_1
   LDX   #$01
   LDA   #$05
   JSR   f0
   STA   __local_main__11
   ORA   #$00
   BMI   .sx_neg@27
   LDA   #$00
   BEQ   .sx_done@28
.sx_neg@27:
   LDA   #$FF
.sx_done@28:
   STA   __local_main__10
   STA   __local_main__9
   STA   __local_main__8
   LDA   g0
   EOR   g1
   STA   __local_main__7
   LDA   g0+1
   EOR   g1+1
   STA   __local_main__6
   LDA   g0+2
   EOR   g1+2
   STA   __local_main__5
   LDA   g0+3
   EOR   g1+3
   STA   __local_main__4
   LDA   g2
   BMI   .sx_neg@29
   LDA   #$00
   BEQ   .sx_done@30
.sx_neg@29:
   LDA   #$FF
.sx_done@30:
   STA   __local_main__2
   STA   __local_main__1
   STA   __local_main__0
   LDA   __local_main__7
   EOR   g2
   STA   __local_main__3
   LDA   __local_main__6
   EOR   __local_main__2
   STA   __local_main__2
   LDA   __local_main__5
   EOR   __local_main__1
   STA   __local_main__1
   LDA   __local_main__4
   EOR   __local_main__0
   STA   __local_main__0
   LDA   __local_main__11
   EOR   __local_main__3
   STA   __local_main__3
   LDA   __local_main__10
   EOR   __local_main__2
   STA   __local_main__2
   LDA   __local_main__9
   EOR   __local_main__1
   STA   HARGS+2
   LDA   __local_main__8
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
   DC.L  $00000083

__zp_init:
   SUBROUTINE

   LDA   #$00
   STA   g0+1
   STA   g0+2
   STA   g0+3
   LDA   #$BF
   STA   g0
   LDA   #$A1
   STA   g2
   RTS
