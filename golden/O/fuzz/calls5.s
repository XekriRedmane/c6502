SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
g0	EQU	$26
g1	EQU	$28
__local_main__0	EQU	$80
__zpabi_f1__p1	EQU	$80
__zpabi_f2__p0	EQU	$80
__local_main__1	EQU	$81
__zpabi_f2__p2_0	EQU	$81
__local_main__2	EQU	$82
__zpabi_f2__p2_1	EQU	$82
__local_main__3	EQU	$83
__zpabi_f2__p2_2	EQU	$83
__local_main__4	EQU	$84
__zpabi_f2__p2_3	EQU	$84
__local_main__5	EQU	$85
__zpabi_f2__p3	EQU	$85
__zpabi_f3__p1	EQU	$86
__zpabi_f4__pp_0	EQU	$87
__local_main__8	EQU	$88
__zpabi_f4__pp_1	EQU	$88
__local_f1__0	EQU	$89
__local_f2__0	EQU	$89
__local_f1__1	EQU	$8A
__local_f2__1	EQU	$8A
__local_f0__0	EQU	$8B
__local_f2__2	EQU	$8B
__local_f0__1	EQU	$8C
__local_f1__it_1	EQU	$8C
__local_f2__it_1	EQU	$8C
__local_f0__2	EQU	$8D
__local_f1__it_0	EQU	$8D
__local_f2__it_0	EQU	$8D
__local_f0__3	EQU	$8E
__local_f1__l0_3	EQU	$8E
__local_f2__l1_3	EQU	$8E
__local_f1__l0_2	EQU	$8F
__local_f2__l1_2	EQU	$8F
__local_f1__l0_1	EQU	$90
__local_f2__l1_1	EQU	$90
__local_f1__l0_0	EQU	$91
__local_f2__l1_0	EQU	$91
__local_f1__p0_1	EQU	$92
__local_f2__box4	EQU	$92
__local_f1__p0_0	EQU	$93
__local_f1__box4	EQU	$94
__local_f3__1	EQU	$9A
__local_f3__2	EQU	$9B
__local_f3__p2	EQU	$9C
__local_f3__3	EQU	$9D
__local_f3__p0	EQU	$9E
__local_f3__box4	EQU	$9F
__local_f4__1	EQU	$A3

; @zp-link-meta-begin
; def f0 params= locals=__local_f0__0,__local_f0__1,__local_f0__2,__local_f0__3 indirect=false in_cycle=false
; def f1 params=__zpabi_f1__p0_0,__zpabi_f1__p0_1,__zpabi_f1__p1 locals=__local_f1__0,__local_f1__1,__local_f1__2,__local_f1__it_1,__local_f1__it_0,__local_f1__l0_3,__local_f1__l0_2,__local_f1__l0_1,__local_f1__l0_0,__local_f1__p0_1,__local_f1__p0_0,__local_f1__box4,__local_f1__4,__local_f1__5,__local_f1__6,__local_f1__7,__local_f1__8,__local_f1__9 indirect=false in_cycle=false param_regs=A,X,
; def f2 params=__zpabi_f2__p0,__zpabi_f2__p1,__zpabi_f2__p2_0,__zpabi_f2__p2_1,__zpabi_f2__p2_2,__zpabi_f2__p2_3,__zpabi_f2__p3 locals=__local_f2__0,__local_f2__1,__local_f2__2,__local_f2__it_1,__local_f2__it_0,__local_f2__l1_3,__local_f2__l1_2,__local_f2__l1_1,__local_f2__l1_0,__local_f2__box4,__local_f2__4,__local_f2__5,__local_f2__6,__local_f2__7,__local_f2__8,__local_f2__9 indirect=false in_cycle=false param_regs=,A,,,,,
; def f3 params=__zpabi_f3__p0,__zpabi_f3__p1,__zpabi_f3__p2 locals=__local_f3__0,__local_f3__1,__local_f3__2,__local_f3__p2,__local_f3__3,__local_f3__p0,__local_f3__box4,__local_f3__5,__local_f3__6 indirect=false in_cycle=false param_regs=A,,X changes=__zpabi_f3__p1
; def f4 params=__zpabi_f4__p0,__zpabi_f4__pp_0,__zpabi_f4__pp_1 locals=__local_f4__0,__local_f4__1,__local_f4__2 indirect=false in_cycle=false param_regs=A,,
; def main params= locals=__local_main__0,__local_main__1,__local_main__2,__local_main__3,__local_main__4,__local_main__5,__local_main__6,__local_main__7,__local_main__8,__local_main__9,__local_main__10 indirect=false in_cycle=false
; call f1 -> f4
; call f2 -> f3
; call f2 -> f4
; call f3 -> f4
; call main -> f0
; item f4 section=.text local=false zero=false
; item f3 section=.text local=false zero=false
; item f3@run0 section=.text local=true zero=false
; item f2 section=.text local=false zero=false
; item f1 section=.text local=false zero=false
; item f0 section=.text local=false zero=false
; item main section=.text local=false zero=false
; item g2 section=.data local=false zero=false
; item __zp_init section=.text local=false zero=false
; zp g0 size=2 at=auto def=true init=$B0,$00 weight=17
; zp g1 size=4 at=auto def=true init=$A6,$00,$00,$00 weight=13
; runtime hargs_size=32 hargs_need=11
; @zp-link-meta-end

f4:
   SUBROUTINE

   LDY   #$01
   LDA   (__zpabi_f4__pp_0),Y
   STA   __local_f4__1
   LDA   #$E8
   CLC
   SBC   g1
   LDA   #$03
   SBC   g1+1
   LDA   #$00
   SBC   g1+2
   LDA   #$00
   SBC   g1+3
   LDA   #$00
   ROL
   CLC
   DEY
   ADC   (__zpabi_f4__pp_0),Y
   BCC   .carry_inc@0
   INC   __local_f4__1
.carry_inc@0:
   LDY   #$00
   STA   (__zpabi_f4__pp_0),Y
   LDA   __local_f4__1
   INY
   STA   (__zpabi_f4__pp_0),Y
   LDA   g0
   LDX   g0+1
   RTS

f3:
   SUBROUTINE

   STA   __local_f3__p0
   STX   __local_f3__p2
   LDA   __zpabi_f3__p1
   BMI   .sx_neg@0
   LDA   #$00
   BEQ   .sx_done@1
.sx_neg@0:
   LDA   #$FF
.sx_done@1:
   STA   __local_f3__1
   JSR   f3@run0
   TAX
   BEQ   .shift_done@3
.shift_loop@2:
   LSR   __local_f3__1
   ROR   __zpabi_f3__p1
   DEX
   BNE   .shift_loop@2
.shift_done@3:
   LDA   __zpabi_f3__p1
   ORA   #$01
   STA   __local_f3__3
   LDA   __local_f3__p2
   STA   __local_f3__box4
   LDA   #$00
   STA   __local_f3__box4+1
   JSR   f3@run0
   TAX
   LDA   __local_f3__3
   STA   __local_f3__2
   CPX   #$00
   BEQ   .shift_done@5
.shift_loop@4:
   LSR   __local_f3__2
   DEX
   BNE   .shift_loop@4
.shift_done@5:
   LDA   #<__local_f3__box4
   STA   __zpabi_f4__pp_0
   LDA   #>__local_f3__box4
   STA   __zpabi_f4__pp_1
   LDA   __local_f3__2
   JSR   f4
   STA   __local_f3__1
   STX   __zpabi_f3__p1
   LDA   __local_f3__p2
   EOR   __local_f3__1
   STA   __local_f3__2
   LDA   #$00
   EOR   __zpabi_f3__p1
   STA   __local_f3__p2
   LDA   __local_f3__box4
   STA   __local_f3__1
   LDA   __local_f3__box4+1
   STA   __zpabi_f3__p1
   LDA   __local_f3__2
   CLC
   ADC   __local_f3__1
   STA   __local_f3__2
   LDA   __local_f3__p2
   ADC   __zpabi_f3__p1
   STA   __zpabi_f3__p1
   LDA   __local_f3__3
   STA   __local_f3__1
   LDA   __zpabi_f3__p1
   BNE   .cmp_true@6
   LDA   __local_f3__2
   CMP   __local_f3__p0
   BNE   .cmp_true@6
   LDA   #$00
   BEQ   .cmp_end@7
.cmp_true@6:
   LDA   #$01
.cmp_end@7:
   STA   __zpabi_f3__p1
   LDA   __local_f3__1
   SEC
   SBC   __zpabi_f3__p1
   LDA   #$00
   SBC   #$00
   BVC   .cmp_novf@9
   EOR   #$80
.cmp_novf@9:
   ASL
   LDA   #$00
   ROL
   RTS

f3@run0:
   SUBROUTINE

   LDA   g2
   AND   #$07
   RTS

f2:
   SUBROUTINE

   LDA   g0+1
   CMP   #$7F
   BNE   .thread@0
   LDA   g0
   CMP   #$FF
   BEQ   .cmp_true@10
.thread@0:
   LDA   #$00
   BEQ   .cmp_end@11
.cmp_true@10:
   LDA   #$01
.cmp_end@11:
   STA   __local_f2__0
   LDA   #$00
   BNE   .cmp_true@13
   LDA   __zpabi_f2__p0
   CMP   __local_f2__0
   BNE   .cmp_true@13
   LDA   #$00
   BEQ   .cmp_end@14
.cmp_true@13:
   LDA   #$01
.cmp_end@14:
   STA   __local_f2__0
   LDA   __zpabi_f2__p2_0
   STA   __local_f2__l1_0
   LDA   __zpabi_f2__p2_1
   AND   #$7F
   STA   __local_f2__l1_1
   LDA   #$00
   STA   __local_f2__l1_2
   STA   __local_f2__l1_3
   STA   __local_f2__box4+1
   STA   __local_f2__it_0
   LDA   __local_f2__0
   STA   __local_f2__box4
   LDY   #$00
   STY   __local_f2__it_1
.loop@0_start:
   LDA   __local_f2__it_0
   SEC
   SBC   #$03
   LDA   __local_f2__it_1
   SBC   #$00
   BVC   .jcmp_novf@16
   EOR   #$80
.jcmp_novf@16:
   BPL   .loop@0_break
   LDA   g0
   ORA   g0+1
   BEQ   .and_false@0
   LDA   __local_f2__l1_0
   ORA   __local_f2__l1_1
   ORA   __local_f2__l1_2
   ORA   __local_f2__l1_3
   BEQ   .and_false@0
   LDA   #$01
   STA   __local_f2__2
   JMP   .and_end@1
.and_false@0:
   LDA   #$00
   STA   __local_f2__2
.and_end@1:
   LDA   #<__local_f2__box4
   STA   __zpabi_f4__pp_0
   LDA   #>__local_f2__box4
   STA   __zpabi_f4__pp_1
   LDA   __local_f2__2
   JSR   f4
   CLC
   ADC   __local_f2__it_0
   STA   __local_f2__1
   TXA
   ADC   __local_f2__it_1
   STA   __local_f2__0
   LDA   __local_f2__l1_0
   CLC
   ADC   __local_f2__1
   STA   __local_f2__l1_0
   LDA   __local_f2__l1_1
   ADC   __local_f2__0
   STA   __local_f2__l1_1
   BCC   .carry_inc@1
   INC   __local_f2__l1_2
   BNE   .carry_inc@1
   INC   __local_f2__l1_3
.carry_inc@1:
   INC   __local_f2__it_0
   BNE   .loop@0_start
   INC   __local_f2__it_1
   JMP   .loop@0_start
.loop@0_break:
   LDA   __local_f2__box4
   STA   __local_f2__it_1
   LDA   __local_f2__box4+1
   STA   __local_f2__2
   BMI   .sx_neg@17
   LDA   #$00
   BEQ   .sx_done@18
.sx_neg@17:
   LDA   #$FF
.sx_done@18:
   STA   __local_f2__1
   STA   __local_f2__0
   LDA   __local_f2__l1_0
   CLC
   ADC   __local_f2__it_1
   STA   __local_f2__it_0
   LDA   __local_f2__l1_1
   ADC   __local_f2__2
   STA   __local_f2__it_1
   LDA   __local_f2__l1_2
   ADC   __local_f2__1
   STA   __local_f2__2
   LDA   __local_f2__l1_3
   ADC   __local_f2__0
   STA   __local_f2__1
   LDA   g0+1
   CMP   g0+1
   BNE   .cmp_true@19
   LDA   g0
   CMP   g0
   BNE   .cmp_true@19
   LDA   #$00
   BEQ   .cmp_end@20
.cmp_true@19:
   LDA   #$01
.cmp_end@20:
   STA   __local_f2__0
   LDA   __zpabi_f2__p0
   SEC
   SBC   g0
   LDA   #$00
   SBC   g0+1
   BVC   .cmp_novf@22
   EOR   #$80
.cmp_novf@22:
   ASL
   LDA   #$00
   ROL
   TAX
   LDA   #$07
   STA   __zpabi_f3__p1
   LDA   __local_f2__0
   JSR   f3
   STA   __local_f2__0
   LDA   __local_f2__it_0
   EOR   __local_f2__0
   STA   __local_f2__l1_1
   LDA   __local_f2__it_1
   STA   __local_f2__l1_2
   LDA   __local_f2__2
   STA   __local_f2__l1_3
   LDA   __local_f2__1
   STA   __local_f2__it_0
   LDA   g2+3
   CMP   __zpabi_f2__p2_3
   BNE   .thread@1
   LDA   g2+2
   CMP   __zpabi_f2__p2_2
   BNE   .thread@1
   LDA   g2+1
   CMP   __zpabi_f2__p2_1
   BNE   .thread@1
   LDA   g2
   CMP   __zpabi_f2__p2_0
   BEQ   .cmp_true@23
.thread@1:
   LDA   #$00
   BEQ   .cmp_end@24
.cmp_true@23:
   LDA   #$01
.cmp_end@24:
   STA   __local_f2__1
   LDA   __local_f2__l1_1
   CLC
   SBC   #$03
   LDA   __local_f2__l1_2
   SBC   #$00
   LDA   __local_f2__l1_3
   SBC   #$00
   LDA   __local_f2__it_0
   SBC   #$00
   LDA   #$00
   ROL
   STA   __local_f2__0
   LDA   #$00
   BNE   .cmp_true@26
   LDA   __local_f2__1
   CMP   __local_f2__0
   BNE   .cmp_true@26
   LDA   #$00
   RTS
.cmp_true@26:
   LDA   #$01
   RTS

f1:
   SUBROUTINE

   STA   __local_f1__p0_0
   STX   __local_f1__p0_1
   JSR   f3@run0
   STA   HARGS+4
   LDA   g1
   STA   HARGS
   LDA   g1+1
   STA   HARGS+1
   LDA   g1+2
   STA   HARGS+2
   LDA   g1+3
   STA   HARGS+3
   JSR   asl32
   LDA   HARGS+5
   AND   g1
   STA   __local_f1__l0_0
   LDA   HARGS+6
   AND   g1+1
   STA   __local_f1__l0_1
   LDA   HARGS+7
   AND   g1+2
   STA   __local_f1__l0_2
   LDA   HARGS+8
   AND   g1+3
   STA   __local_f1__l0_3
   LDA   __local_f1__l0_0
   STA   __local_f1__box4
   LDA   __local_f1__l0_1
   STA   __local_f1__box4+1
   LDA   #$00
   STA   __local_f1__it_0
   LDY   #$00
   STY   __local_f1__it_1
.loop@1_start:
   LDA   __local_f1__it_0
   SEC
   SBC   #$03
   LDA   __local_f1__it_1
   SBC   #$00
   BVC   .jcmp_novf@29
   EOR   #$80
.jcmp_novf@29:
   BPL   .loop@1_break
   LDA   g1
   SEC
   SBC   #$02
   LDX   #<__local_f1__box4
   STX   __zpabi_f4__pp_0
   LDX   #>__local_f1__box4
   STX   __zpabi_f4__pp_1
   JSR   f4
   CLC
   ADC   __local_f1__it_0
   STA   __local_f1__1
   TXA
   ADC   __local_f1__it_1
   STA   __local_f1__0
   LDA   __local_f1__l0_0
   CLC
   ADC   __local_f1__1
   STA   __local_f1__l0_0
   LDA   __local_f1__l0_1
   ADC   __local_f1__0
   STA   __local_f1__l0_1
   BCC   .carry_inc@2
   INC   __local_f1__l0_2
   BNE   .carry_inc@2
   INC   __local_f1__l0_3
.carry_inc@2:
   INC   __local_f1__it_0
   BNE   .loop@1_start
   INC   __local_f1__it_1
   JMP   .loop@1_start
.loop@1_break:
   LDA   __local_f1__p0_0
   CLC
   SBC   __zpabi_f1__p1
   LDA   __local_f1__p0_1
   SBC   #$00
   LDA   #$00
   ROL
   SEC
   SBC   g0
   LDA   #$00
   SBC   g0+1
   BVC   .cmp_novf@30
   EOR   #$80
.cmp_novf@30:
   ASL
   LDA   #$00
   ROL
   EOR   #$01
   RTS

f0:
   SUBROUTINE

   JSR   f3@run0
   TAX
   LDA   g0
   STA   __local_f0__2
   LDA   g0+1
   STA   __local_f0__3
   CPX   #$00
   BEQ   .shift_done@32
.shift_loop@31:
   LDA   __local_f0__3
   ASL
   ROR   __local_f0__3
   ROR   __local_f0__2
   DEX
   BNE   .shift_loop@31
.shift_done@32:
   LDA   __local_f0__3
   BMI   .sx_neg@33
   LDA   #$00
   BEQ   .sx_done@34
.sx_neg@33:
   LDA   #$FF
.sx_done@34:
   STA   __local_f0__1
   STA   __local_f0__0
   LDA   g1+3
   CMP   __local_f0__0
   BNE   .cmp_true@35
   LDA   g1+2
   CMP   __local_f0__1
   BNE   .cmp_true@35
   LDA   g1+1
   CMP   __local_f0__3
   BNE   .cmp_true@35
   LDA   g1
   CMP   __local_f0__2
   BNE   .cmp_true@35
   LDA   #$00
   BEQ   .cmp_end@36
.cmp_true@35:
   LDA   #$01
.cmp_end@36:
   STA   __local_f0__1
   LDA   #$00
   SEC
   SBC   g2
   LDA   #$00
   SBC   g2+1
   LDA   #$00
   SBC   g2+2
   LDA   #$00
   SBC   g2+3
   BVC   .cmp_novf@38
   EOR   #$80
.cmp_novf@38:
   ASL
   LDA   #$00
   ROL
   EOR   #$01
   ORA   #$01
   STA   HARGS+2
   LDA   #$00
   STA   HARGS+3
   STA   HARGS+1
   LDA   __local_f0__1
   STA   HARGS
   JSR   sdivmod16
   LDA   HARGS+4
   RTS

main:
   SUBROUTINE

   JSR   f0
   STA   __local_main__8
   LDA   g0+1
   BMI   .sx_neg@39
   LDA   #$00
   BEQ   .sx_done@40
.sx_neg@39:
   LDA   #$FF
.sx_done@40:
   STA   __local_main__5
   STA   __local_main__4
   LDA   g0
   EOR   g1
   STA   __local_main__3
   LDA   g0+1
   EOR   g1+1
   STA   __local_main__2
   LDA   __local_main__5
   EOR   g1+2
   STA   __local_main__1
   LDA   __local_main__4
   EOR   g1+3
   STA   __local_main__0
   LDA   __local_main__3
   EOR   g2
   STA   __local_main__3
   LDA   __local_main__2
   EOR   g2+1
   STA   __local_main__2
   LDA   __local_main__1
   EOR   g2+2
   STA   __local_main__1
   LDA   __local_main__0
   EOR   g2+3
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

g2:
   DC.L  $00000007

__zp_init:
   SUBROUTINE

   LDA   #$00
   STA   g0+1
   STA   g1+1
   STA   g1+2
   STA   g1+3
   LDA   #$B0
   STA   g0
   LDA   #$A6
   STA   g1
   RTS
