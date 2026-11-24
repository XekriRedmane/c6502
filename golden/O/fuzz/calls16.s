SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
g1	EQU	$26
g0	EQU	$28
__zpabi_f0__p1_0	EQU	$80
__zpabi_f1__p1_0	EQU	$80
__zpabi_f0__p1_1	EQU	$81
__zpabi_f1__p1_1	EQU	$81
__zpabi_f0__p1_2	EQU	$82
__zpabi_f1__p1_2	EQU	$82
__zpabi_f0__p1_3	EQU	$83
__zpabi_f1__p1_3	EQU	$83
__zpabi_f0__pp_0	EQU	$84
__zpabi_f1__pp_0	EQU	$84
__zpabi_f0__pp_1	EQU	$85
__zpabi_f1__pp_1	EQU	$85
__zpabi_f2__p1	EQU	$86
__zpabi_f2__p2	EQU	$87
__zpabi_f2__p3_0	EQU	$88
__zpabi_f2__p3_1	EQU	$89
__zpabi_f2__p3_2	EQU	$8A
__zpabi_f2__p3_3	EQU	$8B
__zpabi_f3__p0_0	EQU	$8C
__zpabi_f4__p1_0	EQU	$8C
__zpabi_f3__p0_1	EQU	$8D
__zpabi_f4__p1_1	EQU	$8D
__zpabi_f3__p0_2	EQU	$8E
__zpabi_f3__p0_3	EQU	$8F
__zpabi_f3__p1_0	EQU	$90
__zpabi_f3__p1_1	EQU	$91
__zpabi_f3__p1_2	EQU	$92
__zpabi_f3__p1_3	EQU	$93
__zpabi_f3__p2	EQU	$94
__zpabi_f3__p3_0	EQU	$95
__zpabi_f3__p3_1	EQU	$96
__local_f1__0	EQU	$97
__local_main__0	EQU	$97
__local_f1__1	EQU	$98
__local_f1__2	EQU	$99
__local_main__2	EQU	$99
__local_f1__3	EQU	$9A
__local_main__3	EQU	$9A
__local_f1__4	EQU	$9B
__local_f1__5	EQU	$9C
__local_f1__6	EQU	$9D
__local_f1__7	EQU	$9E
__local_main__mbox	EQU	$9F
__local_f1__11	EQU	$A2
__local_f1__12	EQU	$A3
__local_f1__p0_1	EQU	$A4
__local_f0__2	EQU	$A5
__local_f1__p0_0	EQU	$A5
__local_f0__3	EQU	$A6
__local_f0__4	EQU	$A7
__local_f0__5	EQU	$A8
__local_f2__l2_1	EQU	$A8
__local_f0__6	EQU	$A9
__local_f2__l2_0	EQU	$A9
__local_f0__7	EQU	$AA
__local_f2__0	EQU	$AA
__local_f0__8	EQU	$AB
__local_f2__1	EQU	$AB
__local_f0__9	EQU	$AC
__local_f2__p0_1	EQU	$AC
__local_f0__10	EQU	$AD
__local_f2__p0_0	EQU	$AD
__local_f0__p0	EQU	$AE
__local_f3__0	EQU	$B2
__local_f3__1	EQU	$B3
__local_f3__2	EQU	$B4
__local_f3__3	EQU	$B5
__local_f3__4	EQU	$B6
__local_f3__5	EQU	$B7

; @zp-link-meta-begin
; def f0 params=__zpabi_f0__p0,__zpabi_f0__p1_0,__zpabi_f0__p1_1,__zpabi_f0__p1_2,__zpabi_f0__p1_3,__zpabi_f0__pp_0,__zpabi_f0__pp_1 locals=__local_f0__0,__local_f0__1,__local_f0__2,__local_f0__3,__local_f0__4,__local_f0__5,__local_f0__6,__local_f0__7,__local_f0__8,__local_f0__9,__local_f0__10,__local_f0__p0,__local_f0__11 indirect=false in_cycle=false param_regs=A,,,,,,
; def f1 params=__zpabi_f1__p0_0,__zpabi_f1__p0_1,__zpabi_f1__p1_0,__zpabi_f1__p1_1,__zpabi_f1__p1_2,__zpabi_f1__p1_3,__zpabi_f1__pp_0,__zpabi_f1__pp_1 locals=__local_f1__0,__local_f1__1,__local_f1__2,__local_f1__3,__local_f1__4,__local_f1__5,__local_f1__6,__local_f1__7,__local_f1__8,__local_f1__9,__local_f1__10,__local_f1__11,__local_f1__12,__local_f1__p0_1,__local_f1__p0_0,__local_f1__13,__local_f1__14 indirect=false in_cycle=false param_regs=A,X,,,,,, changes=__zpabi_f1__p1_1,__zpabi_f1__p1_2,__zpabi_f1__p1_3
; def f2 params=__zpabi_f2__p0_0,__zpabi_f2__p0_1,__zpabi_f2__p1,__zpabi_f2__p2,__zpabi_f2__p3_0,__zpabi_f2__p3_1,__zpabi_f2__p3_2,__zpabi_f2__p3_3 locals=__local_f2__l2_1,__local_f2__l2_0,__local_f2__0,__local_f2__1,__local_f2__p0_1,__local_f2__p0_0,__local_f2__2,__local_f2__3,__local_f2__4,__local_f2__5 indirect=false in_cycle=false param_regs=A,X,,,,,,
; def f3 params=__zpabi_f3__p0_0,__zpabi_f3__p0_1,__zpabi_f3__p0_2,__zpabi_f3__p0_3,__zpabi_f3__p1_0,__zpabi_f3__p1_1,__zpabi_f3__p1_2,__zpabi_f3__p1_3,__zpabi_f3__p2,__zpabi_f3__p3_0,__zpabi_f3__p3_1 locals=__local_f3__0,__local_f3__1,__local_f3__2,__local_f3__3,__local_f3__4,__local_f3__5 indirect=false in_cycle=false carry=true
; def f4 params=__zpabi_f4__p0,__zpabi_f4__p1_0,__zpabi_f4__p1_1 locals=__local_f4__0 indirect=false in_cycle=false param_regs=A,, carry=true
; def main params= locals=__local_main__0,__local_main__1,__local_main__2,__local_main__3,__local_main__4,__local_main__5,__local_main__6,__local_main__7,__local_main__mbox,__local_main__9,__local_main__10,__local_main__11 indirect=false in_cycle=false
; call f0 -> f3
; call f1 -> f2
; call f1 -> f4
; call f2 -> f3
; call f2 -> f4
; call main -> f0
; item f4 section=.text local=false zero=false
; item f3 section=.text local=false zero=false
; item f2 section=.text local=false zero=false
; item f2@run1 section=.text local=true zero=false
; item f2@run0 section=.text local=true zero=false
; item f1 section=.text local=false zero=false
; item f0 section=.text local=false zero=false
; item main section=.text local=false zero=false
; item __zp_init section=.text local=false zero=false
; zp g1 size=2 at=auto def=true init=$3A,$00 weight=11
; zp g0 size=2 at=auto def=true init=$48,$00 weight=7
; runtime hargs_size=32 hargs_need=22
; @zp-link-meta-end

f4:
   SUBROUTINE

   LDA   __zpabi_f4__p1_1
   ORA   __zpabi_f4__p1_0
   BEQ   .cmp_end@0
   SEC
   RTS
.cmp_end@0:
   CMP   #$01
   RTS

f3:
   SUBROUTINE

   LDA   g1
   ASL
   STA   __local_f3__0
   LDA   g1+1
   ROL
   STA   __local_f3__1
   LDA   __local_f3__0
   CLC
   ADC   g1
   STA   __local_f3__5
   LDA   __local_f3__1
   ADC   g1+1
   STA   __local_f3__4
   LDA   __zpabi_f3__p0_0
   SEC
   SBC   #$01
   STA   __local_f3__3
   LDA   __zpabi_f3__p0_1
   SBC   #$00
   STA   __local_f3__2
   LDA   __zpabi_f3__p0_2
   SBC   #$00
   STA   __local_f3__1
   LDA   __zpabi_f3__p0_3
   SBC   #$00
   STA   __local_f3__0
   LDA   __local_f3__3
   SEC
   SBC   __local_f3__5
   LDA   __local_f3__2
   SBC   __local_f3__4
   LDA   __local_f3__1
   SBC   #$00
   LDA   __local_f3__0
   SBC   #$00
   RTS

f2:
   SUBROUTINE

   STA   __local_f2__p0_0
   STX   __local_f2__p0_1
   LDA   __zpabi_f2__p3_0
   ORA   #$01
   STA   HARGS+4
   LDA   __zpabi_f2__p3_1
   STA   HARGS+5
   LDA   __zpabi_f2__p3_2
   STA   HARGS+6
   LDA   __zpabi_f2__p3_3
   STA   HARGS+7
   LDA   #$FF
   JSR   f2@run0
   JSR   sdivmod32
   LDA   __zpabi_f2__p3_0
   SEC
   SBC   HARGS+8
   LDA   __zpabi_f2__p3_1
   SBC   HARGS+9
   LDA   __zpabi_f2__p3_2
   SBC   HARGS+10
   LDA   __zpabi_f2__p3_3
   SBC   HARGS+11
   BVC   .cmp_novf@1
   EOR   #$80
.cmp_novf@1:
   JSR   f2@run1
   STA   __local_f2__1
   LDA   __zpabi_f2__p3_0
   SEC
   SBC   #$FF
   LDA   __zpabi_f2__p3_1
   SBC   #$00
   LDA   __zpabi_f2__p3_2
   SBC   #$00
   LDA   __zpabi_f2__p3_3
   SBC   #$00
   BVC   .cmp_novf@2
   EOR   #$80
.cmp_novf@2:
   JSR   f2@run1
   STA   __local_f2__0
   LDA   #$00
   BPL   .sx_done@4
   LDA   #$FF
.sx_done@4:
   STA   __local_f2__l2_0
   STA   __local_f2__l2_1
   LDA   g0
   ORA   #$01
   STA   HARGS+2
   LDA   g0+1
   STA   HARGS+3
   LDA   __local_f2__p0_0
   STA   HARGS
   LDA   __local_f2__p0_1
   STA   HARGS+1
   JSR   udivmod16
   LDA   HARGS+4
   STA   __zpabi_f3__p3_0
   LDA   HARGS+5
   STA   __zpabi_f3__p3_1
   LDA   __zpabi_f2__p2
   STA   __zpabi_f3__p0_0
   LDA   #$00
   STA   __zpabi_f3__p0_1
   STA   __zpabi_f3__p0_2
   STA   __zpabi_f3__p0_3
   LDA   __local_f2__0
   STA   __zpabi_f3__p1_0
   LDA   #$00
   STA   __zpabi_f3__p1_1
   LDA   __local_f2__l2_0
   STA   __zpabi_f3__p1_2
   LDA   __local_f2__l2_1
   STA   __zpabi_f3__p1_3
   LDA   __zpabi_f2__p2
   STA   __zpabi_f3__p2
   JSR   f3
   LDA   __local_f2__1
   ORA   #$01
   STA   HARGS+4
   LDA   __zpabi_f2__p3_0
   STA   HARGS
   LDA   __zpabi_f2__p3_1
   STA   HARGS+1
   LDA   __zpabi_f2__p3_2
   STA   HARGS+2
   LDA   __zpabi_f2__p3_3
   STA   HARGS+3
   LDA   #$00
   STA   HARGS+5
   STA   HARGS+6
   STA   HARGS+7
   JSR   sdivmod32
   LDA   __zpabi_f2__p1
   BMI   .sx_neg@5
   LDA   #$00
   BEQ   .sx_done@6
.sx_neg@5:
   LDA   #$FF
.sx_done@6:
   STA   __zpabi_f4__p1_1
   LDA   __zpabi_f2__p1
   STA   __zpabi_f4__p1_0
   LDA   HARGS+12
   JSR   f4
   BCC   .if_else@1
   LDA   __local_f2__1
   CLC
   ADC   #$03
   STA   __local_f2__l2_0
   LDA   #$00
   ADC   #$00
   STA   __local_f2__l2_1
   JMP   .if_end@0
.if_else@1:
   LDA   __local_f2__1
   EOR   #$05
   STA   __local_f2__l2_0
   LDA   #$00
   STA   __local_f2__l2_1
.if_end@0:
   LDA   g1
   CLC
   ADC   __local_f2__l2_0
   STA   g1
   LDA   g1+1
   ADC   __local_f2__l2_1
   STA   g1+1
   LDA   __local_f2__p0_1
   BMI   .sx_neg@7
   LDA   #$00
   BEQ   .sx_done@8
.sx_neg@7:
   LDA   #$FF
.sx_done@8:
   STA   HARGS+10
   STA   HARGS+11
   LDA   __local_f2__p0_0
   STA   HARGS+8
   LDA   __local_f2__p0_1
   STA   HARGS+9
   RTS

f2@run1:
   SUBROUTINE

   ASL
   LDA   #$00
   ROL
   EOR   #$01
   RTS

f2@run0:
   SUBROUTINE

   STA   HARGS
   LDA   #$00
   STA   HARGS+1
   LDY   #$00
   STY   HARGS+2
   STA   HARGS+3
   RTS

f1:
   SUBROUTINE

   STA   __local_f1__p0_0
   STX   __local_f1__p0_1
   TXA
   BMI   .sx_neg@9
   LDA   #$00
   BEQ   .sx_done@10
.sx_neg@9:
   LDA   #$FF
.sx_done@10:
   STA   __local_f1__1
   STA   __local_f1__0
   LDA   __local_f1__p0_0
   SEC
   SBC   __zpabi_f1__p1_0
   TXA
   SBC   __zpabi_f1__p1_1
   LDA   __local_f1__1
   SBC   __zpabi_f1__p1_2
   LDA   __local_f1__0
   SBC   __zpabi_f1__p1_3
   BCC   .or_true@2
   LDA   __local_f1__p0_0
   ORA   #$01
   STA   HARGS+2
   STX   HARGS+3
   LDA   __local_f1__p0_0
   STA   HARGS
   STX   HARGS+1
   JSR   sdivmod16
   LDA   HARGS+6
   ORA   HARGS+7
   BNE   .or_true@2
   LDA   #$00
   STA   __local_f1__12
   JMP   .or_end@3
.or_true@2:
   LDA   #$01
   STA   __local_f1__12
.or_end@3:
   LDA   g0
   ORA   g0+1
   BEQ   .and_false@4
   LDA   __local_f1__12
   BEQ   .and_false@4
   LDA   #$01
   STA   __local_f1__7
   LDA   #$00
   STA   __local_f1__6
   JMP   .and_end@5
.and_false@4:
   LDA   #$00
   STA   __local_f1__7
   LDY   #$00
   STY   __local_f1__6
.and_end@5:
   ORA   #$00
   BPL   .sx_done@12
   LDA   #$FF
.sx_done@12:
   STA   __local_f1__5
   STA   __local_f1__4
   LDA   __zpabi_f1__p1_0
   ORA   #$01
   STA   __local_f1__11
   LDA   __zpabi_f1__p1_1
   STA   HARGS+1
   STA   HARGS+5
   LDA   __zpabi_f1__p1_2
   STA   HARGS+2
   STA   HARGS+6
   LDA   __zpabi_f1__p1_0
   STA   HARGS
   LDA   __local_f1__11
   STA   HARGS+4
   LDA   __zpabi_f1__p1_3
   STA   HARGS+3
   STA   HARGS+7
   JSR   udivmod32
   LDA   __local_f1__7
   ORA   HARGS+8
   STA   __local_f1__7
   LDA   __local_f1__6
   ORA   HARGS+9
   STA   __local_f1__6
   LDA   __local_f1__5
   ORA   HARGS+10
   STA   __local_f1__5
   LDA   __local_f1__4
   ORA   HARGS+11
   STA   __local_f1__4
   LDY   #$00
   LDA   (__zpabi_f1__pp_0),Y
   STA   __local_f1__3
   INY
   LDA   (__zpabi_f1__pp_0),Y
   STA   __local_f1__2
   LDA   #$01
   JSR   f2@run0
   LDA   __local_f1__11
   STA   HARGS+4
   LDA   __zpabi_f1__p1_1
   STA   HARGS+5
   LDA   __zpabi_f1__p1_2
   STA   HARGS+6
   LDA   __zpabi_f1__p1_3
   STA   HARGS+7
   JSR   udivmod32
   LDA   __local_f1__3
   CLC
   ADC   HARGS+8
   STA   __local_f1__1
   LDA   __local_f1__2
   ADC   HARGS+9
   LDY   #$01
   STA   (__zpabi_f1__pp_0),Y
   LDA   __local_f1__1
   DEY
   STA   (__zpabi_f1__pp_0),Y
   LDA   #$00
   CMP   __zpabi_f1__p1_3
   BNE   .cmp_end@14
   CMP   __zpabi_f1__p1_2
   BNE   .cmp_end@14
   CMP   __zpabi_f1__p1_1
   BNE   .cmp_end@14
   LDA   __local_f1__12
   CMP   __zpabi_f1__p1_0
   BEQ   .cmp_true@13
   LDA   #$00
   BEQ   .cmp_end@14
.cmp_true@13:
   LDA   #$01
.cmp_end@14:
   LDX   g0
   STX   __zpabi_f4__p1_0
   LDX   g0+1
   STX   __zpabi_f4__p1_1
   JSR   f4
   LDA   __zpabi_f1__p1_0
   SEC
   SBC   __local_f1__12
   LDA   __zpabi_f1__p1_1
   SBC   #$00
   LDA   __zpabi_f1__p1_2
   SBC   #$00
   LDA   __zpabi_f1__p1_3
   SBC   #$00
   LDA   #$00
   ROL
   STA   __local_f1__3
   LDA   __local_f1__7
   ORA   __zpabi_f1__p1_0
   STA   __zpabi_f2__p2
   LDA   g1
   SEC
   SBC   __local_f1__7
   STA   __zpabi_f2__p3_0
   LDA   g1+1
   SBC   __local_f1__6
   STA   __zpabi_f2__p3_1
   LDA   #$00
   SBC   __local_f1__5
   STA   __zpabi_f2__p3_2
   LDA   #$00
   SBC   __local_f1__4
   STA   __zpabi_f2__p3_3
   LDA   g1
   STA   __zpabi_f2__p1
   LDX   #$00
   LDA   __local_f1__3
   JSR   f2
   LDA   __local_f1__p0_0
   AND   __local_f1__p0_0
   STA   __local_f1__0
   LDA   __local_f1__p0_1
   AND   __local_f1__p0_1
   STA   __local_f1__1
   LDA   __local_f1__p0_0
   ORA   __local_f1__0
   STA   __local_f1__0
   LDA   __local_f1__p0_1
   ORA   __local_f1__1
   TAX
   LDA   __local_f1__0
   RTS

f0:
   SUBROUTINE

   STA   __local_f0__p0
   LDA   g1
   AND   #$07
   TAX
   LDA   g0
   STA   __zpabi_f3__p3_0
   LDA   g0+1
   STA   __zpabi_f3__p3_1
   CPX   #$00
   BEQ   .shift_done@17
.shift_loop@16:
   LSR   __zpabi_f3__p3_1
   ROR   __zpabi_f3__p3_0
   DEX
   BNE   .shift_loop@16
.shift_done@17:
   LDA   __local_f0__p0
   SEC
   SBC   __zpabi_f3__p3_0
   LDA   #$00
   SBC   __zpabi_f3__p3_1
   LDA   #$00
   ROL
   STA   __local_f0__10
   LDY   #$00
   LDA   (__zpabi_f0__pp_0),Y
   STA   __local_f0__2
   INY
   LDA   (__zpabi_f0__pp_0),Y
   STA   __zpabi_f3__p3_1
   LDA   #$07
   SEC
   SBC   __zpabi_f0__p1_0
   LDA   #$00
   SBC   __zpabi_f0__p1_1
   LDA   #$00
   SBC   __zpabi_f0__p1_2
   LDA   #$00
   SBC   __zpabi_f0__p1_3
   BVC   .cmp_novf@18
   EOR   #$80
.cmp_novf@18:
   JSR   f2@run1
   CLC
   ADC   __local_f0__2
   BCC   .carry_inc@0
   INC   __zpabi_f3__p3_1
.carry_inc@0:
   LDY   #$00
   STA   (__zpabi_f0__pp_0),Y
   LDA   __zpabi_f3__p3_1
   INY
   STA   (__zpabi_f0__pp_0),Y
   LDA   __zpabi_f0__p1_0
   CLC
   SBC   #$E8
   LDA   __zpabi_f0__p1_1
   SBC   #$03
   LDA   __zpabi_f0__p1_2
   SBC   #$00
   LDA   __zpabi_f0__p1_3
   SBC   #$00
   LDA   #$00
   ROL
   STA   __local_f0__9
   LDA   #$00
   BPL   .sx_done@20
   LDA   #$FF
.sx_done@20:
   STA   __local_f0__8
   STA   __local_f0__7
   LDA   g1
   AND   #$07
   TAX
   LDA   #$E8
   STA   __local_f0__5
   LDA   #$03
   STA   __local_f0__6
   CPX   #$00
   BEQ   .shift_done@22
.shift_loop@21:
   ASL   __local_f0__5
   ROL   __local_f0__6
   DEX
   BNE   .shift_loop@21
.shift_done@22:
   LDA   __local_f0__6
   BMI   .sx_neg@23
   LDA   #$00
   BEQ   .sx_done@24
.sx_neg@23:
   LDA   #$FF
.sx_done@24:
   STA   __local_f0__4
   STA   __local_f0__3
   LDA   g1
   STA   HARGS
   LDA   g1+1
   STA   HARGS+1
   LDA   #$FF
   STA   HARGS+2
   LDA   #$00
   STA   HARGS+3
   JSR   udivmod16
   LDA   __zpabi_f0__p1_0
   AND   #$07
   TAX
   LDA   #$FF
   STA   __zpabi_f3__p3_0
   LDA   #$00
   STA   __zpabi_f3__p3_1
   CPX   #$00
   BEQ   .shift_done@26
.shift_loop@25:
   ASL   __zpabi_f3__p3_0
   ROL   __zpabi_f3__p3_1
   DEX
   BNE   .shift_loop@25
.shift_done@26:
   LDA   __local_f0__9
   STA   __zpabi_f3__p0_0
   LDA   #$00
   STA   __zpabi_f3__p0_1
   LDA   __local_f0__8
   STA   __zpabi_f3__p0_2
   LDA   __local_f0__7
   STA   __zpabi_f3__p0_3
   LDA   __local_f0__5
   STA   __zpabi_f3__p1_0
   LDA   __local_f0__6
   STA   __zpabi_f3__p1_1
   LDA   __local_f0__4
   STA   __zpabi_f3__p1_2
   LDA   __local_f0__3
   STA   __zpabi_f3__p1_3
   LDA   HARGS+6
   STA   __zpabi_f3__p2
   JSR   f3
   LDA   #$00
   ROL
   STA   __zpabi_f3__p3_0
   LDA   __local_f0__10
   EOR   __zpabi_f3__p3_0
   CLC
   ADC   g1
   STA   g1
   BCC   .carry_inc@1
   INC   g1+1
.carry_inc@1:
   LDA   __zpabi_f0__p1_0
   ORA   #$07
   ORA   #$01
   STA   HARGS+4
   LDA   __zpabi_f0__p1_1
   STA   HARGS+5
   LDA   __zpabi_f0__p1_2
   STA   HARGS+6
   LDA   __zpabi_f0__p1_3
   STA   HARGS+7
   LDA   __local_f0__p0
   JSR   f2@run0
   JMP   udivmod32

main:
   SUBROUTINE

   LDA   #$05
   STA   __local_main__mbox
   LDA   #$00
   STA   __local_main__mbox+1
   LDA   #$FE
   STA   __zpabi_f0__p1_0
   LDA   #$00
   STA   __zpabi_f0__p1_1
   STA   __zpabi_f0__p1_2
   STA   __zpabi_f0__p1_3
   LDA   #<__local_main__mbox
   STA   __zpabi_f0__pp_0
   LDA   #>__local_main__mbox
   STA   __zpabi_f0__pp_1
   LDA   #$3D
   JSR   f0
   LDA   g0
   EOR   g1
   STA   __local_main__3
   LDA   g0+1
   EOR   g1+1
   STA   __local_main__2
   LDA   HARGS+8
   EOR   __local_main__3
   STA   __local_main__3
   LDA   HARGS+9
   EOR   __local_main__2
   STA   __local_main__2
   LDA   HARGS+10
   STA   HARGS+2
   LDA   HARGS+11
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

__zp_init:
   SUBROUTINE

   LDA   #$00
   STA   g0+1
   STA   g1+1
   LDA   #$48
   STA   g0
   LDA   #$3A
   STA   g1
   RTS
