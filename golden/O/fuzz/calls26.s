SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
g0	EQU	$26
g2	EQU	$27
__zpabi_f0__p0_0	EQU	$80
__zpabi_f1__p0_0	EQU	$80
__zpabi_f0__p0_1	EQU	$81
__zpabi_f1__p0_1	EQU	$81
__zpabi_f0__p0_2	EQU	$82
__zpabi_f1__p0_2	EQU	$82
__zpabi_f0__p0_3	EQU	$83
__zpabi_f1__p0_3	EQU	$83
__zpabi_f0__p2_0	EQU	$84
__zpabi_f1__p2_0	EQU	$84
__zpabi_f0__p2_1	EQU	$85
__zpabi_f1__p2_1	EQU	$85
__zpabi_f0__p2_2	EQU	$86
__zpabi_f1__p2_2	EQU	$86
__zpabi_f0__p2_3	EQU	$87
__zpabi_f1__p2_3	EQU	$87
__zpabi_f0__p3_0	EQU	$88
__zpabi_f0__p3_1	EQU	$89
__zpabi_f2__pp_0	EQU	$8A
__zpabi_f2__pp_1	EQU	$8B
__zpabi_f3__p1_0	EQU	$8C
__zpabi_f3__p1_1	EQU	$8D
__zpabi_f3__p2	EQU	$8E
__local_f1__0	EQU	$8F
__local_main__0	EQU	$8F
__local_f1__1	EQU	$90
__local_main__1	EQU	$90
__local_f1__2	EQU	$91
__local_main__2	EQU	$91
__local_f1__p1	EQU	$92
__local_main__3	EQU	$92
__local_f1__3	EQU	$93
__local_main__4	EQU	$93
__local_f1__4	EQU	$94
__local_main__5	EQU	$94
__local_f1__5	EQU	$95
__local_main__6	EQU	$95
__local_f1__box2	EQU	$96
__local_main__7	EQU	$96
__local_main__8	EQU	$97
__local_main__9	EQU	$98
__local_main__10	EQU	$99
__local_main__11	EQU	$9A
__local_f0__p1	EQU	$9D
__local_f0__0	EQU	$9E
__local_f0__1	EQU	$9F
__local_f0__2	EQU	$A0
__local_f0__3	EQU	$A1
__local_f0__box2	EQU	$A2
__local_f2__0	EQU	$A5
__local_f2__1	EQU	$A6
__local_f2__2	EQU	$A7
__local_f2__3	EQU	$A8
__local_f2__p0	EQU	$A9
__local_f3__p0_1	EQU	$AB
__local_f3__p0_0	EQU	$AC
__local_f3__0	EQU	$AD

; @zp-link-meta-begin
; def f0 params=__zpabi_f0__p0_0,__zpabi_f0__p0_1,__zpabi_f0__p0_2,__zpabi_f0__p0_3,__zpabi_f0__p1,__zpabi_f0__p2_0,__zpabi_f0__p2_1,__zpabi_f0__p2_2,__zpabi_f0__p2_3,__zpabi_f0__p3_0,__zpabi_f0__p3_1 locals=__local_f0__p1,__local_f0__0,__local_f0__1,__local_f0__2,__local_f0__3,__local_f0__box2,__local_f0__5,__local_f0__6 indirect=false in_cycle=false param_regs=,,,,A,,,,,,
; def f1 params=__zpabi_f1__p0_0,__zpabi_f1__p0_1,__zpabi_f1__p0_2,__zpabi_f1__p0_3,__zpabi_f1__p1,__zpabi_f1__p2_0,__zpabi_f1__p2_1,__zpabi_f1__p2_2,__zpabi_f1__p2_3 locals=__local_f1__0,__local_f1__1,__local_f1__2,__local_f1__p1,__local_f1__3,__local_f1__4,__local_f1__5,__local_f1__box2,__local_f1__7,__local_f1__8,__local_f1__9 indirect=false in_cycle=false param_regs=,,,,A,,,,
; def f2 params=__zpabi_f2__p0,__zpabi_f2__pp_0,__zpabi_f2__pp_1 locals=__local_f2__0,__local_f2__1,__local_f2__2,__local_f2__3,__local_f2__p0,__local_f2__4 indirect=false in_cycle=false param_regs=A,,
; def f3 params=__zpabi_f3__p0_0,__zpabi_f3__p0_1,__zpabi_f3__p1_0,__zpabi_f3__p1_1,__zpabi_f3__p2 locals=__local_f3__p0_1,__local_f3__p0_0,__local_f3__0 indirect=false in_cycle=false param_regs=A,X,,,
; def main params= locals=__local_main__0,__local_main__1,__local_main__2,__local_main__3,__local_main__4,__local_main__5,__local_main__6,__local_main__7,__local_main__8,__local_main__9,__local_main__10,__local_main__11,__local_main__12,__local_main__13 indirect=false in_cycle=false
; call f0 -> f2
; call f0 -> f3
; call f1 -> f2
; call f1 -> f3
; call f2 -> f3
; call main -> f0
; item f3 section=.text local=false zero=false
; item f3@run0 section=.text local=true zero=false
; item f2 section=.text local=false zero=false
; item f1 section=.text local=false zero=false
; item f0 section=.text local=false zero=false
; item main section=.text local=false zero=false
; item g1 section=.data local=false zero=false
; item __zp_init section=.text local=false zero=false
; zp g0 size=1 at=auto def=true init=$6E weight=11
; zp g2 size=1 at=auto def=true init=$20 weight=6
; runtime hargs_size=32 hargs_need=12
; @zp-link-meta-end

f3:
   SUBROUTINE

   STA   __local_f3__p0_0
   STX   __local_f3__p0_1
   LDA   g1
   SEC
   SBC   #$07
   LDA   g1+1
   SBC   #$00
   LDA   g1+2
   SBC   #$00
   LDA   g1+3
   SBC   #$00
   BVC   .cmp_novf@0
   EOR   #$80
.cmp_novf@0:
   ASL
   LDA   #$00
   ROL
   EOR   #$01
   STA   __local_f3__0
   LDA   __local_f3__p0_0
   ORA   __local_f3__p0_1
   BNE   .or_true@0
   JSR   f3@run0
   BNE   .or_true@0
   LDA   #$00
   STA   __local_f3__p0_0
   JMP   .or_end@1
.or_true@0:
   LDA   #$01
   STA   __local_f3__p0_0
.or_end@1:
   LDA   __local_f3__0
   SEC
   SBC   __local_f3__p0_0
   STA   __local_f3__p0_0
   LDA   #$00
   SBC   #$00
   STA   __local_f3__p0_1
   BMI   .sx_neg@1
   LDA   #$00
   BEQ   .sx_done@2
.sx_neg@1:
   LDA   #$FF
.sx_done@2:
   STA   HARGS+10
   STA   HARGS+11
   LDA   __local_f3__p0_0
   STA   HARGS+8
   LDA   __local_f3__p0_1
   STA   HARGS+9
   RTS

f3@run0:
   SUBROUTINE

   LDA   g1
   ORA   g1+1
   ORA   g1+2
   ORA   g1+3
   RTS

f2:
   SUBROUTINE

   STA   __local_f2__p0
   LDA   g2
   BMI   .sx_neg@5
   LDA   #$00
   BEQ   .sx_done@6
.sx_neg@5:
   LDA   #$FF
.sx_done@6:
   STA   __local_f2__1
   LDA   g0
   SEC
   SBC   g2
   LDA   #$00
   SBC   __local_f2__1
   BVC   .cmp_novf@7
   EOR   #$80
.cmp_novf@7:
   ASL
   LDA   #$00
   ROL
   EOR   #$01
   STA   __local_f2__2
   LDA   #$00
   BPL   .sx_done@9
   LDA   #$FF
.sx_done@9:
   STA   __local_f2__1
   STA   __local_f2__0
   LDA   __local_f2__2
   SEC
   SBC   g1
   LDA   #$00
   SBC   g1+1
   LDA   __local_f2__1
   SBC   g1+2
   LDA   __local_f2__0
   SBC   g1+3
   BVC   .cmp_novf@10
   EOR   #$80
.cmp_novf@10:
   ASL
   LDA   #$00
   ROL
   STA   __local_f2__3
   LDY   #$01
   LDA   (__zpabi_f2__pp_0),Y
   STA   __local_f2__1
   DEY
   LDA   (__zpabi_f2__pp_0),Y
   CLC
   ADC   __local_f2__p0
   BCC   .carry_inc@0
   INC   __local_f2__1
.carry_inc@0:
   LDY   #$00
   STA   (__zpabi_f2__pp_0),Y
   LDA   __local_f2__1
   INY
   STA   (__zpabi_f2__pp_0),Y
   LDA   g0
   ORA   #$01
   STA   HARGS+1
   LDA   __local_f2__p0
   STA   HARGS
   JSR   udivmod8
   LDA   HARGS+3
   STA   __local_f2__2
   LDY   #$00
   LDA   (__zpabi_f2__pp_0),Y
   ORA   #$01
   STA   HARGS+2
   INY
   LDA   (__zpabi_f2__pp_0),Y
   STA   HARGS+3
   LDA   #$00
   STA   HARGS
   STA   HARGS+1
   JSR   sdivmod16
   LDA   HARGS+4
   STA   __zpabi_f3__p2
   LDA   #$01
   STA   __zpabi_f3__p1_0
   LDA   #$80
   STA   __zpabi_f3__p1_1
   LDX   #$00
   LDA   __local_f2__2
   JSR   f3
   LDA   __local_f2__3
   EOR   HARGS+8
   CLC
   ADC   g0
   STA   g0
   LDA   __local_f2__p0
   RTS

f1:
   SUBROUTINE

   STA   __local_f1__p1
   LDA   __zpabi_f1__p2_3
   BNE   .cmp_true@11
   LDA   __zpabi_f1__p2_2
   BNE   .cmp_true@11
   LDA   __zpabi_f1__p2_1
   BNE   .cmp_true@11
   LDA   __zpabi_f1__p2_0
   CMP   #$07
   BNE   .cmp_true@11
   LDA   #$00
   BEQ   .cmp_end@12
.cmp_true@11:
   LDA   #$01
.cmp_end@12:
   STA   __local_f1__2
   LDA   g1
   AND   #$07
   STA   HARGS+4
   LDA   __zpabi_f1__p0_0
   STA   HARGS
   LDA   __zpabi_f1__p0_1
   STA   HARGS+1
   LDA   __zpabi_f1__p0_2
   STA   HARGS+2
   LDA   __zpabi_f1__p0_3
   STA   HARGS+3
   JSR   lsr32
   LDA   __local_f1__2
   AND   HARGS+5
   STA   __local_f1__box2
   LDA   #$00
   AND   HARGS+6
   STA   __local_f1__box2+1
   LDA   g2
   ASL
   LDA   #$00
   ADC   #$FF
   EOR   #$FF
   LDX   #<__local_f1__box2
   STX   __zpabi_f2__pp_0
   LDX   #>__local_f1__box2
   STX   __zpabi_f2__pp_1
   JSR   f2
   LDA   #$01
   CLC
   SBC   __local_f1__p1
   LDA   #$00
   ROL
   STA   __local_f1__2
   LDA   __local_f1__p1
   ASL
   ASL
   ASL
   SEC
   SBC   __local_f1__p1
   STA   __zpabi_f3__p2
   LDA   g0
   STA   __zpabi_f3__p1_0
   LDA   #$00
   STA   __zpabi_f3__p1_1
   LDX   #$00
   LDA   __local_f1__2
   JSR   f3
   LDA   #$07
   EOR   HARGS+8
   STA   __local_f1__5
   LDA   #$00
   EOR   HARGS+9
   STA   __local_f1__4
   LDA   #$00
   EOR   HARGS+10
   STA   __local_f1__3
   LDA   #$00
   EOR   HARGS+11
   STA   __local_f1__p1
   LDA   g0
   CLC
   ADC   __local_f1__5
   STA   g0
   LDA   g2
   BMI   .sx_neg@14
   LDA   #$00
   BEQ   .sx_done@15
.sx_neg@14:
   LDA   #$FF
.sx_done@15:
   STA   __local_f1__0
   LDA   g2
   SEC
   SBC   #$E8
   STA   __local_f1__2
   LDA   __local_f1__0
   SBC   #$03
   STA   __local_f1__1
   LDA   __local_f1__p1
   CMP   __local_f1__p1
   BNE   .thread@0
   LDA   __local_f1__3
   CMP   __local_f1__3
   BNE   .thread@0
   LDA   __local_f1__4
   CMP   __local_f1__4
   BNE   .thread@0
   LDA   __local_f1__5
   CMP   __local_f1__5
   BEQ   .cmp_true@16
.thread@0:
   LDA   #$00
   BEQ   .cmp_end@17
.cmp_true@16:
   LDA   #$01
.cmp_end@17:
   STA   __local_f1__0
   LDA   __local_f1__1
   BNE   .cmp_true@19
   LDA   __local_f1__2
   CMP   __local_f1__0
   BNE   .cmp_true@19
   LDA   #$00
   BEQ   .cmp_end@20
.cmp_true@19:
   LDA   #$01
.cmp_end@20:
   STA   __local_f1__0
   LDA   #$00
   BPL   .sx_done@23
   LDA   #$FF
.sx_done@23:
   STA   HARGS+10
   STA   HARGS+11
   LDA   __local_f1__0
   STA   HARGS+8
   LDA   #$00
   STA   HARGS+9
   RTS

f0:
   SUBROUTINE

   STA   __local_f0__p1
   LDA   g1
   STA   __local_f0__3
   STA   __local_f0__box2
   LDA   g1+1
   STA   __local_f0__2
   STA   __local_f0__box2+1
   LDA   __zpabi_f0__p3_0
   AND   __local_f0__p1
   LDX   #<__local_f0__box2
   STX   __zpabi_f2__pp_0
   LDX   #>__local_f0__box2
   STX   __zpabi_f2__pp_1
   JSR   f2
   STA   __local_f0__0
   ORA   #$00
   BMI   .sx_neg@24
   LDA   #$00
   BEQ   .sx_done@25
.sx_neg@24:
   LDA   #$FF
.sx_done@25:
   STA   __local_f0__p1
   LDA   __local_f0__3
   EOR   __local_f0__0
   STA   __local_f0__0
   LDA   __local_f0__2
   EOR   __local_f0__p1
   STA   __local_f0__p1
   LDA   __local_f0__0
   CLC
   ADC   __local_f0__box2
   STA   __local_f0__2
   LDA   __local_f0__p1
   ADC   __local_f0__box2+1
   STA   __local_f0__1
   BMI   .sx_neg@26
   LDA   #$00
   BEQ   .sx_done@27
.sx_neg@26:
   LDA   #$FF
.sx_done@27:
   STA   __local_f0__0
   STA   __local_f0__p1
   LDA   g1
   CLC
   ADC   __local_f0__2
   STA   g1
   LDA   g1+1
   ADC   __local_f0__1
   STA   g1+1
   LDA   g1+2
   ADC   __local_f0__0
   STA   g1+2
   LDA   g1+3
   ADC   __local_f0__p1
   STA   g1+3
   LDA   #$00
   BPL   .sx_done@29
   LDA   #$FF
.sx_done@29:
   STA   __local_f0__0
   STA   __local_f0__p1
   LDA   __zpabi_f0__p2_3
   CMP   __local_f0__p1
   BNE   .cmp_true@30
   LDA   __zpabi_f0__p2_2
   CMP   __local_f0__0
   BNE   .cmp_true@30
   LDA   __zpabi_f0__p2_1
   BNE   .cmp_true@30
   LDA   __zpabi_f0__p2_0
   CMP   g0
   BNE   .cmp_true@30
   LDA   #$00
   BEQ   .cmp_end@31
.cmp_true@30:
   LDA   #$01
.cmp_end@31:
   STA   __local_f0__p1
   JSR   f3@run0
   BEQ   .lnot_true@33
   LDA   #$00
   BEQ   .lnot_end@34
.lnot_true@33:
   LDA   #$01
.lnot_end@34:
   STA   __zpabi_f3__p2
   LDA   #$FF
   STA   __zpabi_f3__p1_0
   STA   __zpabi_f3__p1_1
   LDX   #$00
   LDA   __local_f0__p1
   JSR   f3
   LDA   g2
   AND   #$64
   RTS

main:
   SUBROUTINE

   LDA   #$B2
   STA   __zpabi_f0__p0_0
   LDA   #$7B
   STA   __zpabi_f0__p2_0
   LDA   #$47
   STA   __zpabi_f0__p3_0
   LDA   #$00
   STA   __zpabi_f0__p0_1
   STA   __zpabi_f0__p0_2
   STA   __zpabi_f0__p0_3
   STA   __zpabi_f0__p2_1
   STA   __zpabi_f0__p2_2
   STA   __zpabi_f0__p2_3
   STA   __zpabi_f0__p3_1
   LDA   #$BB
   JSR   f0
   STA   __local_main__11
   ORA   #$00
   BMI   .sx_neg@35
   LDA   #$00
   BEQ   .sx_done@36
.sx_neg@35:
   LDA   #$FF
.sx_done@36:
   STA   __local_main__10
   STA   __local_main__9
   STA   __local_main__8
   LDA   g0
   EOR   g1
   STA   __local_main__7
   LDA   #$00
   EOR   g1+1
   STA   __local_main__6
   LDA   #$00
   EOR   g1+2
   STA   __local_main__5
   LDA   #$00
   EOR   g1+3
   STA   __local_main__4
   LDA   g2
   BMI   .sx_neg@37
   LDA   #$00
   BEQ   .sx_done@38
.sx_neg@37:
   LDA   #$FF
.sx_done@38:
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
   DC.L  $0000008B

__zp_init:
   SUBROUTINE

   LDA   #$6E
   STA   g0
   LDA   #$20
   STA   g2
   RTS
