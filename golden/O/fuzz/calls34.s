SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
g0	EQU	$26
g1	EQU	$27
__zpabi_f0__p0_0	EQU	$80
__zpabi_f1__p2_0	EQU	$80
__zpabi_f3__p0_0	EQU	$80
__zpabi_f4__p1_0	EQU	$80
__zpabi_f0__p0_1	EQU	$81
__zpabi_f1__p2_1	EQU	$81
__zpabi_f3__p0_1	EQU	$81
__zpabi_f4__p1_1	EQU	$81
__zpabi_f0__p2_0	EQU	$82
__zpabi_f1__p2_2	EQU	$82
__zpabi_f3__p0_2	EQU	$82
__zpabi_f4__p2_0	EQU	$82
__local_f3__0	EQU	$83
__zpabi_f0__p2_1	EQU	$83
__zpabi_f1__p2_3	EQU	$83
__zpabi_f4__p2_1	EQU	$83
__local_f3__1	EQU	$84
__zpabi_f0__p2_2	EQU	$84
__zpabi_f1__p3_0	EQU	$84
__zpabi_f4__p3_0	EQU	$84
__local_f3__2	EQU	$85
__zpabi_f0__p2_3	EQU	$85
__zpabi_f1__p3_1	EQU	$85
__zpabi_f4__p3_1	EQU	$85
__local_f3__3	EQU	$86
__local_f4__0	EQU	$86
__zpabi_f0__p3_0	EQU	$86
__zpabi_f1__p3_2	EQU	$86
__local_f4__1	EQU	$87
__zpabi_f0__p3_1	EQU	$87
__zpabi_f1__p3_3	EQU	$87
__local_f1__0	EQU	$88
__local_f4__2	EQU	$88
__zpabi_f0__p3_2	EQU	$88
__local_f1__1	EQU	$89
__local_f4__3	EQU	$89
__zpabi_f0__p3_3	EQU	$89
__local_f1__2	EQU	$8A
__zpabi_f2__p1_0	EQU	$8A
__zpabi_f2__p1_1	EQU	$8B
__zpabi_f2__p1_2	EQU	$8C
__zpabi_f2__p1_3	EQU	$8D
__zpabi_f2__p2_0	EQU	$8E
__zpabi_f2__p2_1	EQU	$8F
__zpabi_f2__p2_2	EQU	$90
__zpabi_f2__p2_3	EQU	$91
__local_main__0	EQU	$92
__local_main__1	EQU	$93
__local_main__2	EQU	$94
__local_main__3	EQU	$95
__local_main__4	EQU	$96
__local_main__5	EQU	$97
__local_main__6	EQU	$98
__local_main__7	EQU	$99
__local_main__8	EQU	$9A
__local_f0__it_1	EQU	$A0
__local_f0__it_0	EQU	$A1
__local_f0__l1	EQU	$A2
__local_f0__p1	EQU	$A3

; @zp-link-meta-begin
; def f0 params=__zpabi_f0__p0_0,__zpabi_f0__p0_1,__zpabi_f0__p1,__zpabi_f0__p2_0,__zpabi_f0__p2_1,__zpabi_f0__p2_2,__zpabi_f0__p2_3,__zpabi_f0__p3_0,__zpabi_f0__p3_1,__zpabi_f0__p3_2,__zpabi_f0__p3_3 locals=__local_f0__0,__local_f0__1,__local_f0__2,__local_f0__it_1,__local_f0__it_0,__local_f0__l1,__local_f0__p1,__local_f0__3 indirect=false in_cycle=false param_regs=,,A,,,,,,,,
; def f1 params=__zpabi_f1__p0,__zpabi_f1__p1,__zpabi_f1__p2_0,__zpabi_f1__p2_1,__zpabi_f1__p2_2,__zpabi_f1__p2_3,__zpabi_f1__p3_0,__zpabi_f1__p3_1,__zpabi_f1__p3_2,__zpabi_f1__p3_3 locals=__local_f1__0,__local_f1__1,__local_f1__2,__local_f1__p1 indirect=false in_cycle=false param_regs=A,X,,,,,,,,
; def f2 params=__zpabi_f2__p0_0,__zpabi_f2__p0_1,__zpabi_f2__p1_0,__zpabi_f2__p1_1,__zpabi_f2__p1_2,__zpabi_f2__p1_3,__zpabi_f2__p2_0,__zpabi_f2__p2_1,__zpabi_f2__p2_2,__zpabi_f2__p2_3 locals= indirect=false in_cycle=false param_regs=A,X,,,,,,,,
; def f3 params=__zpabi_f3__p0_0,__zpabi_f3__p0_1,__zpabi_f3__p0_2 locals=__local_f3__0,__local_f3__1,__local_f3__2,__local_f3__3 indirect=false in_cycle=false
; def f4 params=__zpabi_f4__p0_0,__zpabi_f4__p0_1,__zpabi_f4__p1_0,__zpabi_f4__p1_1,__zpabi_f4__p2_0,__zpabi_f4__p2_1,__zpabi_f4__p3_0,__zpabi_f4__p3_1 locals=__local_f4__0,__local_f4__1,__local_f4__2,__local_f4__3 indirect=false in_cycle=false param_regs=A,X,,,,,,
; def main params= locals=__local_main__0,__local_main__1,__local_main__2,__local_main__3,__local_main__4,__local_main__5,__local_main__6,__local_main__7,__local_main__8,__local_main__9,__local_main__10 indirect=false in_cycle=false
; call f0 -> f2
; call main -> f0
; share member=f2:__zpabi_f2__p2_0,__zpabi_f2__p2_1,__zpabi_f2__p2_2,__zpabi_f2__p2_3 holder=f0 live=f0:f2 forward=f0>f2 writer=f0
; item f4 section=.text local=false zero=false
; item f3 section=.text local=false zero=false
; item f2 section=.text local=false zero=false
; item f1 section=.text local=false zero=false
; item f0 section=.text local=false zero=false
; item main section=.text local=false zero=false
; item g2 section=.data local=false zero=false
; item __zp_init section=.text local=false zero=false
; zp g0 size=1 at=auto def=true init=$3A weight=14
; zp g1 size=1 at=auto def=true init=$63 weight=9
; runtime hargs_size=32 hargs_need=22
; @zp-link-meta-end

f4:
   SUBROUTINE

   LDA   g1
   BMI   .sx_neg@0
   LDA   #$00
   BEQ   .sx_done@1
.sx_neg@0:
   LDA   #$FF
.sx_done@1:
   STA   __local_f4__0
   LDA   #$00
   SEC
   SBC   g1
   LDA   #$00
   SBC   __local_f4__0
   BVC   .cmp_novf@2
   EOR   #$80
.cmp_novf@2:
   ASL
   LDA   #$00
   ROL
   EOR   #$01
   STA   __local_f4__0
   LDA   g0
   STA   __local_f4__2
   BMI   .sx_neg@3
   LDA   #$00
   BEQ   .sx_done@4
.sx_neg@3:
   LDA   #$FF
.sx_done@4:
   STA   __local_f4__3
   LDA   __local_f4__0
   AND   #$07
   TAX
   BEQ   .shift_done@6
.shift_loop@5:
   ASL   __local_f4__2
   ROL   __local_f4__3
   DEX
   BNE   .shift_loop@5
.shift_done@6:
   LDA   __zpabi_f4__p1_0
   ORA   __zpabi_f4__p1_1
   BNE   .or_true@0
   LDA   #$00
   STA   __local_f4__1
   JMP   .or_end@1
.or_true@0:
   LDA   #$01
   STA   __local_f4__1
.or_end@1:
   LDA   __local_f4__3
   BNE   .thread@0
   LDA   __local_f4__2
   CMP   __local_f4__1
   BEQ   .cmp_true@7
.thread@0:
   LDA   #$00
   BEQ   .cmp_end@8
.cmp_true@7:
   LDA   #$01
.cmp_end@8:
   LDX   #$00
   RTS

f3:
   SUBROUTINE

   LDA   g0
   BMI   .sx_neg@10
   LDA   #$00
   BEQ   .sx_done@11
.sx_neg@10:
   LDA   #$FF
.sx_done@11:
   STA   __local_f3__2
   BMI   .sx_neg@12
   LDA   #$00
   BEQ   .sx_done@13
.sx_neg@12:
   LDA   #$FF
.sx_done@13:
   STA   __local_f3__1
   STA   __local_f3__0
   LDA   g2
   SEC
   SBC   g0
   LDA   g2+1
   SBC   __local_f3__2
   LDA   g2+2
   SBC   __local_f3__1
   LDA   g2+3
   SBC   __local_f3__0
   BCC   .and_false@2
   LDA   #$01
   STA   __local_f3__3
   LDA   #$00
   BEQ   .and_end@3
.and_false@2:
   LDA   #$00
   STA   __local_f3__3
.and_end@3:
   ORA   #$00
   BPL   .sx_done@15
   LDA   #$FF
.sx_done@15:
   STA   __local_f3__0
   ORA   __local_f3__3
   ORA   __local_f3__0
   BEQ   .lnot_true@16
   LDA   #$00
   BEQ   .lnot_end@17
.lnot_true@16:
   LDA   #$01
.lnot_end@17:
   AND   #$07
   TAX
   LDA   #$01
   STA   __local_f3__0
   CPX   #$00
   BEQ   .shift_done@19
.shift_loop@18:
   LSR   __local_f3__0
   DEX
   BNE   .shift_loop@18
.shift_done@19:
   LDX   #$00
   LDA   __local_f3__0
   RTS

f2:
   SUBROUTINE

   LDA   #$FF
   RTS

f1:
   SUBROUTINE

   TXA
   BMI   .sx_neg@20
   LDA   #$00
   BEQ   .sx_done@21
.sx_neg@20:
   LDA   #$FF
.sx_done@21:
   STA   __local_f1__2
   LDA   g0
   BMI   .sx_neg@22
   LDA   #$00
   BEQ   .sx_done@23
.sx_neg@22:
   LDA   #$FF
.sx_done@23:
   STA   __local_f1__1
   TXA
   EOR   g0
   STA   __local_f1__0
   LDA   __local_f1__2
   EOR   __local_f1__1
   STA   __local_f1__1
   SEC
   LDA   #$00
   SBC   __local_f1__0
   STA   __local_f1__0
   LDA   #$00
   SBC   __local_f1__1
   TAX
   LDA   __local_f1__0
   RTS

f0:
   SUBROUTINE

   STA   __local_f0__p1
   LDA   __zpabi_f0__p2_0
   STA   HARGS
   LDA   __zpabi_f0__p2_1
   STA   HARGS+1
   LDA   __zpabi_f0__p2_2
   STA   HARGS+2
   LDA   __zpabi_f0__p2_3
   STA   HARGS+3
   LDA   #$FF
   STA   HARGS+4
   LDA   #$00
   STA   HARGS+5
   LDY   #$00
   STY   HARGS+6
   STA   HARGS+7
   JSR   sdivmod32
   LDA   HARGS+8
   EOR   #$FF
   STA   __local_f0__l1
   LDA   __zpabi_f0__p0_0
   STA   __zpabi_f2__p2_0
   LDA   __zpabi_f0__p0_1
   STA   __zpabi_f2__p2_1
   BMI   .sx_neg@24
   LDA   #$00
   BEQ   .sx_done@25
.sx_neg@24:
   LDA   #$FF
.sx_done@25:
   STA   __zpabi_f2__p2_2
   STA   __zpabi_f2__p2_3
   LDA   #$00
   STA   __local_f0__it_0
   LDY   #$00
   STY   __local_f0__it_1
.loop@0_start:
   LDA   __local_f0__it_0
   SEC
   SBC   #$03
   LDA   __local_f0__it_1
   SBC   #$00
   BVC   .jcmp_novf@26
   EOR   #$80
.jcmp_novf@26:
   BPL   .loop@0_break
   LDA   __zpabi_f0__p3_0
   ORA   #$01
   STA   HARGS+4
   LDA   __zpabi_f0__p3_1
   STA   HARGS+5
   LDA   __zpabi_f0__p3_2
   STA   HARGS+6
   LDA   __zpabi_f0__p3_3
   STA   HARGS+7
   LDA   __local_f0__p1
   STA   HARGS
   LDA   #$00
   STA   HARGS+1
   LDY   #$00
   STY   HARGS+2
   STA   HARGS+3
   JSR   udivmod32
   LDX   HARGS+9
   LDA   g0
   BMI   .sx_neg@27
   LDA   #$00
   BEQ   .sx_done@28
.sx_neg@27:
   LDA   #$FF
.sx_done@28:
   STA   __zpabi_f2__p1_1
   LDA   #$01
   CLC
   ADC   g0
   STA   __zpabi_f2__p1_0
   LDA   #$00
   ADC   __zpabi_f2__p1_1
   STA   __zpabi_f2__p1_1
   BMI   .sx_neg@29
   LDA   #$00
   BEQ   .sx_done@30
.sx_neg@29:
   LDA   #$FF
.sx_done@30:
   STA   __zpabi_f2__p1_2
   STA   __zpabi_f2__p1_3
   LDA   HARGS+8
   JSR   f2
   CLC
   ADC   __local_f0__it_0
   CLC
   ADC   __local_f0__l1
   STA   __local_f0__l1
   INC   __local_f0__it_0
   BNE   .loop@0_start
   INC   __local_f0__it_1
   JMP   .loop@0_start
.loop@0_break:
   LDA   __zpabi_f0__p2_0
   RTS

main:
   SUBROUTINE

   LDA   #$11
   STA   __zpabi_f0__p0_0
   LDA   #$01
   STA   __zpabi_f0__p0_1
   STA   __zpabi_f0__p3_1
   LDA   #$4A
   STA   __zpabi_f0__p2_0
   LDA   #$08
   STA   __zpabi_f0__p3_0
   LDA   #$00
   STA   __zpabi_f0__p2_1
   STA   __zpabi_f0__p2_2
   STA   __zpabi_f0__p2_3
   STA   __zpabi_f0__p3_2
   STA   __zpabi_f0__p3_3
   LDA   #$68
   JSR   f0
   STA   __local_main__8
   LDA   g0
   BMI   .sx_neg@31
   LDA   #$00
   BEQ   .sx_done@32
.sx_neg@31:
   LDA   #$FF
.sx_done@32:
   STA   __local_main__6
   STA   __local_main__5
   STA   __local_main__4
   LDA   g1
   BMI   .sx_neg@33
   LDA   #$00
   BEQ   .sx_done@34
.sx_neg@33:
   LDA   #$FF
.sx_done@34:
   STA   __local_main__2
   STA   __local_main__1
   STA   __local_main__0
   LDA   g0
   EOR   g1
   STA   __local_main__7
   LDA   __local_main__6
   EOR   __local_main__2
   STA   __local_main__6
   LDA   __local_main__5
   EOR   __local_main__1
   STA   __local_main__5
   LDA   __local_main__4
   EOR   __local_main__0
   STA   __local_main__4
   LDA   __local_main__7
   EOR   g2
   STA   __local_main__3
   LDA   __local_main__6
   EOR   g2+1
   STA   __local_main__2
   LDA   __local_main__5
   EOR   g2+2
   STA   __local_main__1
   LDA   __local_main__4
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
   DC.L  $00000010

__zp_init:
   SUBROUTINE

   LDA   #$3A
   STA   g0
   LDA   #$63
   STA   g1
   RTS
