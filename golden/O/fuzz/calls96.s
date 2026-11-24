SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
g1	EQU	$26
__zpabi_f0__p0_0	EQU	$80
__zpabi_f1__p0_0	EQU	$80
__zpabi_f3__p0_0	EQU	$80
__zpabi_f0__p0_1	EQU	$81
__zpabi_f1__p0_1	EQU	$81
__zpabi_f3__p0_1	EQU	$81
__zpabi_f0__p0_2	EQU	$82
__zpabi_f1__p0_2	EQU	$82
__zpabi_f3__p0_2	EQU	$82
__zpabi_f0__p0_3	EQU	$83
__zpabi_f1__p0_3	EQU	$83
__zpabi_f3__p0_3	EQU	$83
__zpabi_f0__p0_4	EQU	$84
__zpabi_f1__p1_0	EQU	$84
__zpabi_f3__p2_0	EQU	$84
__zpabi_f1__p1_1	EQU	$85
__zpabi_f3__p2_1	EQU	$85
__zpabi_f1__p1_2	EQU	$86
__zpabi_f3__pp_0	EQU	$86
__zpabi_f1__p1_3	EQU	$87
__zpabi_f3__pp_1	EQU	$87
__zpabi_f2__p0_0	EQU	$88
__local_f3__1	EQU	$89
__zpabi_f2__p0_1	EQU	$89
__zpabi_f2__p1_0	EQU	$8A
__zpabi_f2__p1_1	EQU	$8B
__zpabi_f2__p3_0	EQU	$8C
__zpabi_f2__p3_1	EQU	$8D
__zpabi_f2__p3_2	EQU	$8E
__zpabi_f2__p3_3	EQU	$8F
__zpabi_f2__pp_0	EQU	$90
__zpabi_f2__pp_1	EQU	$91
__zpabi_f4__p1_0	EQU	$92
__zpabi_f4__p1_1	EQU	$93
__local_f1__0	EQU	$94
__local_main__0	EQU	$94
__local_f1__1	EQU	$95
__local_main__2	EQU	$96
__local_main__3	EQU	$97
__local_f1__box2	EQU	$98
__local_main__5	EQU	$99
__local_main__6	EQU	$9A
__local_main__7	EQU	$9B
__local_main__8	EQU	$9C
__local_f0__0	EQU	$9F
__local_f0__1	EQU	$A0
__local_f0__2	EQU	$A1
__local_f0__3	EQU	$A2
__local_f0__4	EQU	$A3
__local_f0__9	EQU	$A8
__local_f0__10	EQU	$A9
__local_f0__11	EQU	$AA
__local_f0__box2	EQU	$AB
__local_f2__p2_1	EQU	$AF
__local_f2__p2_0	EQU	$B0
__local_f2__0	EQU	$B1
__local_f2__1	EQU	$B2
__local_f2__2	EQU	$B3
__local_f2__3	EQU	$B4
__local_f2__4	EQU	$B5
__local_f2__5	EQU	$B6

; @zp-link-meta-begin
; def f0 params=__zpabi_f0__p0_0,__zpabi_f0__p0_1,__zpabi_f0__p0_2,__zpabi_f0__p0_3,__zpabi_f0__p0_4 locals=__local_f0__0,__local_f0__1,__local_f0__2,__local_f0__3,__local_f0__4,__local_f0__5,__local_f0__6,__local_f0__7,__local_f0__8,__local_f0__9,__local_f0__10,__local_f0__11,__local_f0__box2,__local_f0__13,__local_f0__14,__local_f0__15 indirect=false in_cycle=false
; def f1 params=__zpabi_f1__p0_0,__zpabi_f1__p0_1,__zpabi_f1__p0_2,__zpabi_f1__p0_3,__zpabi_f1__p1_0,__zpabi_f1__p1_1,__zpabi_f1__p1_2,__zpabi_f1__p1_3 locals=__local_f1__0,__local_f1__1,__local_f1__2,__local_f1__3,__local_f1__box2,__local_f1__5 indirect=false in_cycle=false carry=true
; def f2 params=__zpabi_f2__p0_0,__zpabi_f2__p0_1,__zpabi_f2__p1_0,__zpabi_f2__p1_1,__zpabi_f2__p2_0,__zpabi_f2__p2_1,__zpabi_f2__p3_0,__zpabi_f2__p3_1,__zpabi_f2__p3_2,__zpabi_f2__p3_3,__zpabi_f2__pp_0,__zpabi_f2__pp_1 locals=__local_f2__p2_1,__local_f2__p2_0,__local_f2__0,__local_f2__1,__local_f2__2,__local_f2__3,__local_f2__4,__local_f2__5 indirect=false in_cycle=false param_regs=,,,,A,X,,,,,, carry=true
; def f3 params=__zpabi_f3__p0_0,__zpabi_f3__p0_1,__zpabi_f3__p0_2,__zpabi_f3__p0_3,__zpabi_f3__p1,__zpabi_f3__p2_0,__zpabi_f3__p2_1,__zpabi_f3__p3,__zpabi_f3__pp_0,__zpabi_f3__pp_1 locals=__local_f3__0,__local_f3__1 indirect=false in_cycle=false param_regs=,,,,A,,,X,, carry=true
; def f4 params=__zpabi_f4__p0,__zpabi_f4__p1_0,__zpabi_f4__p1_1 locals= indirect=false in_cycle=false param_regs=A,,
; def main params= locals=__local_main__0,__local_main__1,__local_main__2,__local_main__3,__local_main__4,__local_main__5,__local_main__6,__local_main__7,__local_main__8,__local_main__9,__local_main__10 indirect=false in_cycle=false
; call f0 -> f2
; call f0 -> f4
; call f1 -> f2
; call f2 -> f4
; call f3 -> f4
; call main -> f0
; item f4 section=.text local=false zero=false
; item f3 section=.text local=false zero=false
; item f2 section=.text local=false zero=false
; item f1 section=.text local=false zero=false
; item f0 section=.text local=false zero=false
; item f0@run1 section=.text local=true zero=false
; item f0@run0 section=.text local=true zero=false
; item main section=.text local=false zero=false
; item g0 section=.data local=false zero=false
; item __zp_init section=.text local=false zero=false
; zp g1 size=1 at=auto def=true init=$57 weight=5
; runtime hargs_size=32 hargs_need=21
; @zp-link-meta-end

f4:
   SUBROUTINE

   LDX   __zpabi_f4__p1_1
   LDA   __zpabi_f4__p1_0
   RTS

f3:
   SUBROUTINE

   LDY   #$01
   LDA   (__zpabi_f3__pp_0),Y
   STA   __local_f3__1
   DEY
   LDA   (__zpabi_f3__pp_0),Y
   CLC
   ADC   #$02
   BCC   .carry_inc@0
   INC   __local_f3__1
.carry_inc@0:
   LDY   #$00
   STA   (__zpabi_f3__pp_0),Y
   LDA   __local_f3__1
   INY
   STA   (__zpabi_f3__pp_0),Y
   LDA   #$01
   STA   __zpabi_f4__p1_0
   DEY
   STY   __zpabi_f4__p1_1
   JSR   f4
   SEC
   RTS

f2:
   SUBROUTINE

   STA   __local_f2__p2_0
   LDY   #$00
   LDA   (__zpabi_f2__pp_0),Y
   STA   __local_f2__1
   INY
   LDA   (__zpabi_f2__pp_0),Y
   STA   __local_f2__0
   LDA   g0
   ORA   g0+1
   ORA   g0+2
   ORA   g0+3
   ORA   g0+4
   BNE   .or_true@0
   TXA
   ORA   __local_f2__p2_0
   BNE   .or_true@0
   LDA   #$00
   STA   __local_f2__p2_0
   JMP   .or_end@1
.or_true@0:
   LDA   #$01
   STA   __local_f2__p2_0
.or_end@1:
   LDA   __local_f2__1
   CLC
   ADC   __local_f2__p2_0
   STA   __local_f2__p2_0
   LDA   __local_f2__0
   ADC   #$00
   LDY   #$01
   STA   (__zpabi_f2__pp_0),Y
   LDA   __local_f2__p2_0
   DEY
   STA   (__zpabi_f2__pp_0),Y
   SEC
   LDA   #$00
   SBC   __zpabi_f2__p1_0
   STA   __local_f2__p2_1
   LDA   (__zpabi_f2__pp_0),Y
   STA   HARGS
   INY
   LDA   (__zpabi_f2__pp_0),Y
   STA   HARGS+1
   LDA   #$FF
   STA   HARGS+2
   LDA   #$00
   STA   HARGS+3
   JSR   sdivmod16
   LDA   HARGS+6
   STA   __zpabi_f4__p1_0
   LDA   HARGS+7
   STA   __zpabi_f4__p1_1
   LDA   __local_f2__p2_1
   JSR   f4
   LDA   g0
   STA   __local_f2__5
   LDA   g0+1
   STA   __local_f2__4
   LDA   g0+2
   STA   __local_f2__3
   LDA   g0+3
   STA   __local_f2__2
   LDA   __zpabi_f2__p3_0
   AND   #$07
   STA   HARGS+4
   LDA   __zpabi_f2__p3_0
   STA   HARGS
   LDA   __zpabi_f2__p3_1
   STA   HARGS+1
   LDA   __zpabi_f2__p3_2
   STA   HARGS+2
   LDA   __zpabi_f2__p3_3
   STA   HARGS+3
   JSR   asl32
   LDA   __local_f2__5
   SEC
   SBC   HARGS+5
   LDA   __local_f2__4
   SBC   HARGS+6
   LDA   __local_f2__3
   SBC   HARGS+7
   LDA   __local_f2__2
   SBC   HARGS+8
   RTS

f1:
   SUBROUTINE

   LDA   __zpabi_f1__p0_0
   EOR   #$FF
   STA   __zpabi_f2__p0_0
   LDA   __zpabi_f1__p0_1
   EOR   #$FF
   STA   __zpabi_f2__p3_0
   LDA   __zpabi_f1__p0_2
   EOR   #$FF
   STA   __local_f1__1
   LDA   __zpabi_f1__p0_3
   EOR   #$FF
   STA   __local_f1__0
   LDA   __zpabi_f2__p0_0
   CLC
   SBC   #$03
   LDA   __zpabi_f2__p3_0
   SBC   #$00
   LDA   __local_f1__1
   SBC   #$00
   LDA   __local_f1__0
   SBC   #$00
   LDA   #$00
   ROL
   STA   __local_f1__box2
   LDA   #$00
   STA   __local_f1__box2+1
   LDA   __zpabi_f1__p1_1
   AND   #$01
   LDX   __zpabi_f1__p1_0
   STX   __zpabi_f2__p0_0
   EOR   #$01
   TAX
   DEX
   LDA   #$E8
   SEC
   SBC   __local_f1__box2
   LDA   #$03
   SBC   #$00
   BVC   .cmp_novf@0
   EOR   #$80
.cmp_novf@0:
   ASL
   LDA   #$00
   ROL
   STA   __zpabi_f2__p3_0
   LDA   #$00
   BPL   .sx_done@2
   LDA   #$FF
.sx_done@2:
   STA   __zpabi_f2__p3_2
   STA   __zpabi_f2__p3_3
   STX   __zpabi_f2__p0_1
   LDA   #$01
   STA   __zpabi_f2__p1_0
   LDA   #$00
   STA   __zpabi_f2__p1_1
   STA   __zpabi_f2__p3_1
   LDA   #<__local_f1__box2
   STA   __zpabi_f2__pp_0
   LDA   #>__local_f1__box2
   STA   __zpabi_f2__pp_1
   LDA   __zpabi_f2__p0_0
   JSR   f2
   LDA   #$01
   CLC
   SBC   g0
   LDA   #$00
   SBC   g0+1
   LDA   #$00
   SBC   g0+2
   LDA   #$00
   SBC   g0+3
   LDA   #$00
   SBC   g0+4
   RTS

f0:
   SUBROUTINE

   LDA   g0
   AND   #$07
   STA   HARGS+8
   JSR   f0@run0
   JSR   lsr64
   LDA   #$00
   BPL   .sx_done@4
   LDA   #$FF
.sx_done@4:
   STA   __local_f0__2
   STA   __local_f0__1
   AND   #$03
   STA   __local_f0__0
   LDA   g1
   SEC
   SBC   HARGS+9
   LDA   #$00
   SBC   HARGS+10
   LDA   __local_f0__2
   SBC   HARGS+11
   LDA   __local_f0__1
   SBC   HARGS+12
   LDA   __local_f0__0
   SBC   HARGS+13
   LDA   #$00
   ROL
   STA   __local_f0__2
   LDA   g0
   ORA   __zpabi_f0__p0_0
   LDX   __zpabi_f0__p0_0
   STX   __zpabi_f4__p1_0
   LDX   __zpabi_f0__p0_1
   STX   __zpabi_f4__p1_1
   JSR   f4
   STA   __local_f0__1
   STX   __local_f0__0
   LDA   __local_f0__2
   EOR   __local_f0__1
   STA   __local_f0__10
   LDA   #$00
   EOR   __local_f0__0
   STA   __local_f0__9
   BMI   .sx_neg@5
   LDA   #$00
   BEQ   .sx_done@6
.sx_neg@5:
   LDA   #$FF
.sx_done@6:
   STA   __local_f0__2
   STA   __local_f0__1
   LDA   __local_f0__10
   STA   __local_f0__4
   LDA   __local_f0__9
   STA   __local_f0__3
   LDA   __local_f0__2
   AND   #$03
   STA   __local_f0__0
   LDA   g0
   CLC
   ADC   __local_f0__4
   STA   __local_f0__4
   LDA   g0+1
   ADC   __local_f0__3
   STA   __local_f0__3
   LDA   g0+2
   ADC   __local_f0__2
   STA   __local_f0__2
   LDA   g0+3
   ADC   __local_f0__1
   STA   __local_f0__1
   LDA   g0+4
   ADC   __local_f0__0
   LDX   __local_f0__4
   STX   g0
   LDX   __local_f0__3
   STX   g0+1
   LDX   __local_f0__2
   STX   g0+2
   LDX   __local_f0__1
   JSR   f0@run1
   LDA   __local_f0__10
   STA   __local_f0__box2
   LDA   __local_f0__9
   STA   __local_f0__box2+1
   LDA   g0
   STA   __local_f0__1
   LDA   g0+1
   AND   #$01
   LDX   __local_f0__1
   STX   __zpabi_f2__p0_0
   EOR   #$01
   SEC
   SBC   #$01
   STA   __zpabi_f2__p0_1
   LDA   g1
   SEC
   SBC   __zpabi_f0__p0_0
   STA   __local_f0__1
   LDA   #$00
   SBC   __zpabi_f0__p0_1
   AND   #$01
   LDX   __local_f0__1
   STX   __zpabi_f2__p1_0
   EOR   #$01
   SEC
   SBC   #$01
   STA   __zpabi_f2__p1_1
   LDA   #$00
   BPL   .sx_done@10
   LDA   #$FF
.sx_done@10:
   STA   __local_f0__2
   STA   __local_f0__1
   LDA   g0
   AND   g1
   STA   __zpabi_f2__p3_0
   LDA   #$00
   STA   __zpabi_f2__p3_1
   LDA   g0+2
   AND   __local_f0__2
   STA   __zpabi_f2__p3_2
   LDA   g0+3
   AND   __local_f0__1
   STA   __zpabi_f2__p3_3
   LDA   #<__local_f0__box2
   STA   __zpabi_f2__pp_0
   LDA   #>__local_f0__box2
   STA   __zpabi_f2__pp_1
   LDX   #$00
   LDA   #$01
   JSR   f2
   LDA   #$00
   ROL
   STA   __local_f0__0
   LDA   __local_f0__10
   EOR   __local_f0__0
   LDX   __local_f0__9
   STX   __local_f0__0
   CLC
   ADC   __local_f0__box2
   STA   __local_f0__11
   LDA   __local_f0__0
   ADC   __local_f0__box2+1
   STA   __local_f0__10
   BMI   .sx_neg@11
   LDA   #$00
   BEQ   .sx_done@12
.sx_neg@11:
   LDA   #$FF
.sx_done@12:
   LDX   __local_f0__11
   STX   __local_f0__9
   LDX   __local_f0__10
   STX   __zpabi_f2__p0_0
   STA   __zpabi_f2__p0_1
   STA   __zpabi_f2__p1_0
   AND   #$03
   STA   __zpabi_f2__p1_1
   LDA   g0
   CLC
   ADC   __local_f0__9
   STA   __local_f0__4
   LDA   g0+1
   ADC   __zpabi_f2__p0_0
   STA   __local_f0__3
   LDA   g0+2
   ADC   __zpabi_f2__p0_1
   STA   __local_f0__2
   LDA   g0+3
   ADC   __zpabi_f2__p1_0
   STA   __local_f0__1
   LDA   g0+4
   ADC   __zpabi_f2__p1_1
   LDX   __local_f0__4
   STX   g0
   LDX   __local_f0__3
   STX   g0+1
   LDX   __local_f0__2
   STX   g0+2
   LDX   __local_f0__1
   JSR   f0@run1
   LDA   __zpabi_f2__p1_1
   CMP   g0+4
   BNE   .or_true@8
   LDA   __zpabi_f2__p1_0
   CMP   g0+3
   BNE   .or_true@8
   CMP   g0+2
   BNE   .or_true@8
   LDA   __zpabi_f2__p0_0
   CMP   g0+1
   BNE   .or_true@8
   LDA   __local_f0__9
   CMP   g0
   BNE   .or_true@8
   ORA   #$01
   STA   __local_f0__4
   LDA   __local_f0__10
   STA   __local_f0__3
   BMI   .sx_neg@13
   LDA   #$00
   BEQ   .sx_done@14
.sx_neg@13:
   LDA   #$FF
.sx_done@14:
   LDX   __local_f0__4
   STX   HARGS+8
   LDX   __local_f0__3
   STX   HARGS+9
   STA   HARGS+10
   STA   HARGS+11
   AND   #$03
   STA   HARGS+12
   JSR   f0@run0
   STA   HARGS+13
   STA   HARGS+14
   STA   HARGS+15
   JSR   udivmod64
   LDA   HARGS+16
   ORA   HARGS+17
   ORA   HARGS+18
   ORA   HARGS+19
   ORA   HARGS+20
   BNE   .or_true@8
   LDA   #$00
   RTS
.or_true@8:
   LDA   #$01
   RTS

f0@run1:
   SUBROUTINE

   STX   g0+3
   AND   #$03
   STA   g0+4
   RTS

f0@run0:
   SUBROUTINE

   LDA   __zpabi_f0__p0_0
   STA   HARGS
   LDA   __zpabi_f0__p0_1
   STA   HARGS+1
   LDA   __zpabi_f0__p0_2
   STA   HARGS+2
   LDA   __zpabi_f0__p0_3
   STA   HARGS+3
   LDA   __zpabi_f0__p0_4
   STA   HARGS+4
   LDA   #$00
   STA   HARGS+5
   STA   HARGS+6
   STA   HARGS+7
   RTS

main:
   SUBROUTINE

   LDA   #$BF
   STA   __zpabi_f0__p0_0
   LDA   #$00
   STA   __zpabi_f0__p0_1
   STA   __zpabi_f0__p0_2
   STA   __zpabi_f0__p0_3
   STA   __zpabi_f0__p0_4
   JSR   f0
   STA   __local_main__8
   ORA   #$00
   BMI   .sx_neg@15
   LDA   #$00
   BEQ   .sx_done@16
.sx_neg@15:
   LDA   #$FF
.sx_done@16:
   STA   __local_main__7
   STA   __local_main__6
   STA   __local_main__5
   LDA   g0
   EOR   g1
   STA   __local_main__3
   LDA   __local_main__8
   EOR   __local_main__3
   STA   __local_main__3
   LDA   __local_main__7
   EOR   g0+1
   STA   __local_main__2
   LDA   __local_main__6
   EOR   g0+2
   STA   HARGS+2
   LDA   __local_main__5
   EOR   g0+3
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

g0:
   DC.B  $34
   DS.B  4

__zp_init:
   SUBROUTINE

   LDA   #$57
   STA   g1
   RTS
