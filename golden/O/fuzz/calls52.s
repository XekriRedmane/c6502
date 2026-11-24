SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__local_f3__0	EQU	$80
__zpabi_f0__p1_0	EQU	$80
__zpabi_f1__p0_0	EQU	$80
__zpabi_f2__p1_0	EQU	$80
__local_f3__1	EQU	$81
__zpabi_f0__p1_1	EQU	$81
__zpabi_f1__p0_1	EQU	$81
__zpabi_f2__p1_1	EQU	$81
__local_f3__2	EQU	$82
__zpabi_f0__p1_2	EQU	$82
__zpabi_f1__p0_2	EQU	$82
__zpabi_f2__p1_2	EQU	$82
__local_f3__3	EQU	$83
__zpabi_f0__p1_3	EQU	$83
__zpabi_f1__p0_3	EQU	$83
__zpabi_f2__p1_3	EQU	$83
__local_f3__4	EQU	$84
__zpabi_f0__p2_0	EQU	$84
__zpabi_f1__p0_4	EQU	$84
__zpabi_f2__p2_0	EQU	$84
__local_f3__5	EQU	$85
__zpabi_f0__p2_1	EQU	$85
__zpabi_f1__p1_0	EQU	$85
__zpabi_f2__p2_1	EQU	$85
__local_f3__6	EQU	$86
__zpabi_f0__p2_2	EQU	$86
__zpabi_f1__p1_1	EQU	$86
__zpabi_f2__p2_2	EQU	$86
__zpabi_f0__p2_3	EQU	$87
__zpabi_f1__p1_2	EQU	$87
__zpabi_f2__p2_3	EQU	$87
__zpabi_f0__p2_4	EQU	$88
__zpabi_f1__p1_3	EQU	$88
__zpabi_f2__p2_4	EQU	$88
__zpabi_f0__p2_5	EQU	$89
__zpabi_f1__p1_4	EQU	$89
__zpabi_f2__p2_5	EQU	$89
__local_f2__p0_1	EQU	$8A
__local_main__0	EQU	$8A
__zpabi_f1__p1_5	EQU	$8A
__local_f2__p0_0	EQU	$8B
__zpabi_f1__p3_0	EQU	$8B
__local_f2__0	EQU	$8C
__local_main__2	EQU	$8C
__zpabi_f1__p3_1	EQU	$8C
__local_f2__1	EQU	$8D
__local_main__3	EQU	$8D
__zpabi_f1__pp_0	EQU	$8D
__local_f2__2	EQU	$8E
__local_main__4	EQU	$8E
__zpabi_f1__pp_1	EQU	$8E
__local_f1__0	EQU	$8F
__local_f2__3	EQU	$8F
__local_main__5	EQU	$8F
__local_f1__1	EQU	$90
__local_f2__4	EQU	$90
__local_f1__p2	EQU	$91
__local_f2__5	EQU	$91
__local_main__7	EQU	$91
__local_f2__6	EQU	$92
__local_f2__7	EQU	$93
__local_f0__0	EQU	$94
__local_f2__8	EQU	$94
__local_f0__1	EQU	$95
__local_f0__2	EQU	$96
__local_f4__1	EQU	$96
__local_f0__3	EQU	$97
__local_f0__p0_1	EQU	$98
__local_f0__p0_0	EQU	$99
__local_f4__4	EQU	$99
__local_f4__5	EQU	$9A
__local_f4__6	EQU	$9B

; @zp-link-meta-begin
; def f0 params=__zpabi_f0__p0_0,__zpabi_f0__p0_1,__zpabi_f0__p1_0,__zpabi_f0__p1_1,__zpabi_f0__p1_2,__zpabi_f0__p1_3,__zpabi_f0__p2_0,__zpabi_f0__p2_1,__zpabi_f0__p2_2,__zpabi_f0__p2_3,__zpabi_f0__p2_4,__zpabi_f0__p2_5 locals=__local_f0__0,__local_f0__1,__local_f0__2,__local_f0__3,__local_f0__p0_1,__local_f0__p0_0 indirect=false in_cycle=false param_regs=A,X,,,,,,,,,,
; def f1 params=__zpabi_f1__p0_0,__zpabi_f1__p0_1,__zpabi_f1__p0_2,__zpabi_f1__p0_3,__zpabi_f1__p0_4,__zpabi_f1__p1_0,__zpabi_f1__p1_1,__zpabi_f1__p1_2,__zpabi_f1__p1_3,__zpabi_f1__p1_4,__zpabi_f1__p1_5,__zpabi_f1__p2,__zpabi_f1__p3_0,__zpabi_f1__p3_1,__zpabi_f1__pp_0,__zpabi_f1__pp_1 locals=__local_f1__0,__local_f1__1,__local_f1__p2 indirect=false in_cycle=false param_regs=,,,,,,,,,,,A,,,, carry=true
; def f2 params=__zpabi_f2__p0_0,__zpabi_f2__p0_1,__zpabi_f2__p1_0,__zpabi_f2__p1_1,__zpabi_f2__p1_2,__zpabi_f2__p1_3,__zpabi_f2__p2_0,__zpabi_f2__p2_1,__zpabi_f2__p2_2,__zpabi_f2__p2_3,__zpabi_f2__p2_4,__zpabi_f2__p2_5 locals=__local_f2__p0_1,__local_f2__p0_0,__local_f2__0,__local_f2__1,__local_f2__2,__local_f2__3,__local_f2__4,__local_f2__5,__local_f2__6,__local_f2__7,__local_f2__8 indirect=false in_cycle=false param_regs=A,X,,,,,,,,,, carry=true
; def f3 params= locals=__local_f3__0,__local_f3__1,__local_f3__2,__local_f3__3,__local_f3__4,__local_f3__5,__local_f3__6 indirect=false in_cycle=false
; def f4 params= locals=__local_f4__0,__local_f4__1,__local_f4__2,__local_f4__3,__local_f4__4,__local_f4__5,__local_f4__6 indirect=false in_cycle=false carry=true
; def main params= locals=__local_main__0,__local_main__1,__local_main__2,__local_main__3,__local_main__4,__local_main__5,__local_main__6,__local_main__7,__local_main__8,__local_main__9 indirect=false in_cycle=false
; call f1 -> f4
; call f2 -> f4
; call main -> f0
; item f4 section=.text local=false zero=false
; item f3 section=.text local=false zero=false
; item f2 section=.text local=false zero=false
; item f1 section=.text local=false zero=false
; item f0 section=.text local=false zero=false
; item main section=.text local=false zero=false
; item g0 section=.data local=false zero=false
; runtime hargs_size=32 hargs_need=22
; @zp-link-meta-end

f4:
   SUBROUTINE

   LDA   g0
   AND   #$07
   STA   HARGS+8
   LDA   g0
   STA   HARGS
   LDA   g0+1
   STA   HARGS+1
   LDA   g0+2
   STA   HARGS+2
   LDA   g0+3
   STA   HARGS+3
   LDA   g0+4
   STA   HARGS+4
   LDA   g0+5
   STA   HARGS+5
   ASL
   LDA   #$00
   ADC   #$FF
   EOR   #$FF
   STA   HARGS+6
   STA   HARGS+7
   JSR   asr64
   LDA   g0
   ORA   HARGS+9
   STA   __local_f4__4
   LDA   g0+1
   ORA   HARGS+10
   STA   __local_f4__5
   LDA   __local_f4__4
   STA   __local_f4__6
   SEC
   LDA   #$00
   SBC   __local_f4__6
   STA   __local_f4__1
   LDA   #$00
   SBC   __local_f4__5
   ORA   __local_f4__1
   BEQ   .cmp_end@0
   SEC
   RTS
.cmp_end@0:
   CMP   #$01
   RTS

f3:
   SUBROUTINE

   LDA   g0
   EOR   g0
   STA   __local_f3__6
   LDA   g0+1
   EOR   g0+1
   STA   __local_f3__2
   LDA   g0+2
   EOR   g0+2
   STA   __local_f3__1
   LDA   g0+3
   EOR   g0+3
   STA   __local_f3__0
   LDA   __local_f3__6
   SEC
   SBC   g0
   LDA   __local_f3__2
   SBC   g0+1
   LDA   __local_f3__1
   SBC   g0+2
   LDA   __local_f3__0
   SBC   g0+3
   LDA   #$00
   SBC   g0+4
   LDA   #$00
   SBC   g0+5
   BVC   .cmp_novf@1
   EOR   #$80
.cmp_novf@1:
   ASL
   LDA   #$00
   ROL
   STA   __local_f3__5
   LDA   #$00
   BPL   .sx_done@3
   LDA   #$FF
.sx_done@3:
   STA   __local_f3__4
   STA   __local_f3__3
   LDA   __local_f3__0
   CMP   __local_f3__3
   BNE   .cmp_true@4
   LDA   __local_f3__1
   CMP   __local_f3__4
   BNE   .cmp_true@4
   LDA   __local_f3__2
   BNE   .cmp_true@4
   LDA   __local_f3__6
   CMP   __local_f3__5
   BNE   .cmp_true@4
   LDA   #$00
   BEQ   .cmp_end@5
.cmp_true@4:
   LDA   #$01
.cmp_end@5:
   STA   __local_f3__2
   LDA   #$00
   BPL   .sx_done@8
   LDA   #$FF
.sx_done@8:
   STA   __local_f3__1
   STA   __local_f3__0
   LDA   __local_f3__2
   SEC
   SBC   __local_f3__5
   STA   HARGS+16
   LDA   #$00
   SBC   #$00
   STA   HARGS+17
   LDA   __local_f3__1
   SBC   __local_f3__4
   STA   HARGS+18
   LDA   __local_f3__0
   SBC   __local_f3__3
   STA   HARGS+19
   LDA   #$00
   STA   HARGS+20
   RTS

f2:
   SUBROUTINE

   AND   #$FF
   STA   __local_f2__0
   TXA
   AND   #$7F
   STA   __local_f2__p0_0
   JSR   f4
   LDA   #$00
   ROL
   STA   __local_f2__p0_1
   LDA   __local_f2__0
   EOR   __local_f2__p0_1
   STA   __local_f2__3
   LDA   __local_f2__p0_0
   STA   __local_f2__2
   LDY   #$00
   LDA   g0
   CLC
   ADC   __local_f2__3
   STA   __local_f2__3
   LDA   g0+1
   ADC   __local_f2__2
   STA   __local_f2__2
   LDA   g0+2
   ADC   #$00
   STA   __local_f2__1
   LDA   g0+3
   ADC   #$00
   STA   __local_f2__0
   LDA   g0+4
   ADC   #$00
   STA   __local_f2__p0_0
   LDA   g0+5
   ADC   #$00
   AND   #$0F
   EOR   #$08
   LDX   __local_f2__3
   STX   g0
   LDX   __local_f2__2
   STX   g0+1
   LDX   __local_f2__1
   STX   g0+2
   LDX   __local_f2__0
   STX   g0+3
   LDX   __local_f2__p0_0
   STX   g0+4
   SEC
   SBC   #$08
   STA   g0+5
   LDA   __zpabi_f2__p1_0
   ORA   __zpabi_f2__p1_1
   ORA   __zpabi_f2__p1_2
   ORA   __zpabi_f2__p1_3
   BEQ   .lnot_true@9
   TYA
   JMP   .lnot_end@10
.lnot_true@9:
   LDA   #$01
.lnot_end@10:
   STA   __local_f2__8
   TYA
   BPL   .sx_done@12
   LDA   #$FF
.sx_done@12:
   STA   __local_f2__7
   STA   __local_f2__6
   STA   __local_f2__5
   STA   __local_f2__4
   LDA   __zpabi_f2__p2_0
   STA   HARGS
   LDA   __zpabi_f2__p2_1
   STA   HARGS+1
   LDA   __zpabi_f2__p2_2
   STA   HARGS+2
   LDA   __zpabi_f2__p2_3
   STA   HARGS+3
   LDA   __zpabi_f2__p2_4
   STA   HARGS+4
   LDA   __zpabi_f2__p2_5
   STA   HARGS+5
   LDA   g0
   STA   HARGS+8
   LDA   g0+1
   STA   HARGS+9
   LDA   g0+2
   STA   HARGS+10
   LDA   g0+3
   STA   HARGS+11
   LDA   g0+4
   STA   HARGS+12
   LDA   g0+5
   STA   HARGS+13
   TYA
   STA   HARGS+6
   STA   HARGS+7
   STA   HARGS+14
   STA   HARGS+15
   JSR   mul64
   LDA   HARGS+21
   AND   #$0F
   EOR   #$08
   SEC
   SBC   #$08
   STA   __local_f2__p0_1
   LDA   __local_f2__4
   CMP   __local_f2__p0_1
   BNE   .thread@0
   LDA   __local_f2__5
   CMP   HARGS+20
   BNE   .thread@0
   LDA   __local_f2__6
   CMP   HARGS+19
   BNE   .thread@0
   LDA   __local_f2__7
   CMP   HARGS+18
   BNE   .thread@0
   LDA   #$00
   CMP   HARGS+17
   BNE   .cmp_end@14
   LDA   __local_f2__8
   CMP   HARGS+16
   BEQ   .cmp_true@13
.thread@0:
   CLC
   RTS
.cmp_true@13:
   SEC
   RTS
.cmp_end@14:
   CMP   #$01
   RTS

f1:
   SUBROUTINE

   STA   __local_f1__p2
   LDY   #$00
   LDA   (__zpabi_f1__pp_0),Y
   CLC
   ADC   __zpabi_f1__p1_0
   STA   __local_f1__1
   INY
   LDA   (__zpabi_f1__pp_0),Y
   ADC   __zpabi_f1__p1_1
   STA   (__zpabi_f1__pp_0),Y
   LDA   __local_f1__1
   DEY
   STA   (__zpabi_f1__pp_0),Y
   JSR   f4
   LDY   #$00
   LDA   (__zpabi_f1__pp_0),Y
   STA   __local_f1__1
   INY
   LDA   (__zpabi_f1__pp_0),Y
   STA   __local_f1__0
   LDA   #$E8
   SEC
   SBC   __local_f1__1
   LDA   #$03
   SBC   __local_f1__0
   BVC   .jcmp_novf@16
   EOR   #$80
.jcmp_novf@16:
   BPL   .or_true@4
   LDA   __zpabi_f1__p0_0
   ORA   __zpabi_f1__p0_1
   ORA   __zpabi_f1__p0_2
   ORA   __zpabi_f1__p0_3
   ORA   __zpabi_f1__p0_4
   BEQ   .f1@ssa_block@5
   LDA   __local_f1__p2
   BNE   .or_true@4
.f1@ssa_block@5:
   CLC
   RTS
.or_true@4:
   SEC
   RTS

f0:
   SUBROUTINE

   STA   __local_f0__p0_0
   TXA
   BMI   .sx_neg@17
   LDA   #$00
   BEQ   .sx_done@18
.sx_neg@17:
   LDA   #$FF
.sx_done@18:
   STA   __local_f0__3
   STA   __local_f0__2
   STA   __local_f0__1
   STA   __local_f0__0
   LDA   __local_f0__p0_0
   ORA   g0
   STA   __local_f0__p0_0
   TXA
   ORA   g0+1
   STA   __local_f0__p0_1
   LDA   __local_f0__3
   ORA   g0+2
   STA   __local_f0__3
   LDA   __local_f0__2
   ORA   g0+3
   STA   __local_f0__2
   LDA   __local_f0__1
   ORA   g0+4
   STA   __local_f0__1
   LDA   __local_f0__0
   ORA   g0+5
   STA   __local_f0__0
   LDA   __local_f0__p0_0
   ORA   __local_f0__p0_1
   ORA   __local_f0__3
   ORA   __local_f0__2
   ORA   __local_f0__1
   ORA   __local_f0__0
   BEQ   .lnot_true@19
   LDA   #$00
   BEQ   .lnot_end@20
.lnot_true@19:
   LDA   #$01
.lnot_end@20:
   LDX   #$00
   RTS

main:
   SUBROUTINE

   LDA   #$A8
   STA   __zpabi_f0__p1_0
   LDA   #$69
   STA   __zpabi_f0__p2_0
   LDA   #$00
   STA   __zpabi_f0__p1_1
   STA   __zpabi_f0__p1_2
   STA   __zpabi_f0__p1_3
   STA   __zpabi_f0__p2_1
   STA   __zpabi_f0__p2_2
   STA   __zpabi_f0__p2_3
   STA   __zpabi_f0__p2_4
   STA   __zpabi_f0__p2_5
   LDX   #$00
   LDA   #$12
   JSR   f0
   STA   __local_main__7
   TXA
   BMI   .sx_neg@21
   LDA   #$00
   BEQ   .sx_done@22
.sx_neg@21:
   LDA   #$FF
.sx_done@22:
   STA   __local_main__5
   STA   __local_main__4
   LDA   __local_main__7
   EOR   g0
   STA   __local_main__3
   TXA
   EOR   g0+1
   STA   __local_main__2
   LDA   __local_main__5
   EOR   g0+2
   STA   HARGS+2
   LDA   __local_main__4
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
   DC.B  $5E
   DS.B  5
