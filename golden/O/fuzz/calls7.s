SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
g0	EQU	$26
__zpabi_f0__p0_0	EQU	$80
__zpabi_f3__p1_0	EQU	$80
__zpabi_f0__p0_1	EQU	$81
__zpabi_f3__p1_1	EQU	$81
__zpabi_f0__p0_2	EQU	$82
__zpabi_f3__p1_2	EQU	$82
__zpabi_f0__p0_3	EQU	$83
__zpabi_f3__p1_3	EQU	$83
__zpabi_f3__p2	EQU	$84
__zpabi_f4__p0	EQU	$85
__zpabi_f4__p1_0	EQU	$86
__zpabi_f4__p1_1	EQU	$87
__zpabi_f4__p1_2	EQU	$88
__zpabi_f4__p1_3	EQU	$89
__zpabi_f4__p2_0	EQU	$8A
__zpabi_f4__p2_1	EQU	$8B
__zpabi_f4__p2_2	EQU	$8C
__zpabi_f4__p2_3	EQU	$8D
__zpabi_f4__pp_0	EQU	$8E
__zpabi_f4__pp_1	EQU	$8F
__local_f1__0	EQU	$90
__local_f1__l2_0	EQU	$91
__local_f1__1	EQU	$92
__local_main__2	EQU	$92
__local_f1__2	EQU	$93
__local_main__3	EQU	$93
__local_f0__0	EQU	$94
__local_f1__l2_3	EQU	$94
__local_f0__p1	EQU	$95
__local_f1__l2_2	EQU	$95
__local_f1__3	EQU	$96
__local_f1__l2_1	EQU	$97
__local_f1__4	EQU	$98
__local_f1__5	EQU	$99
__local_f0__5	EQU	$9A
__local_f1__6	EQU	$9A
__local_f0__6	EQU	$9B
__local_f1__7	EQU	$9B
__local_f0__box4	EQU	$9C
__local_f1__8	EQU	$9C
__local_f1__box4	EQU	$9D
__local_f1__box2	EQU	$9F
__local_f2__0	EQU	$A4
__local_f2__1	EQU	$A5
__local_f2__3	EQU	$A7
__local_f2__4	EQU	$A8
__local_f2__5	EQU	$A9
__local_f2__6	EQU	$AA
__local_f2__7	EQU	$AB
__local_f2__8	EQU	$AC
__local_f2__9	EQU	$AD
__local_f2__pp	EQU	$AE
__local_f2__10	EQU	$AF
__local_f2__box4	EQU	$B0
__local_f3__0	EQU	$B2
__local_f3__1	EQU	$B3
__local_f3__3	EQU	$B5
__local_f3__p0	EQU	$B6
__local_f3__4	EQU	$B7
__local_f3__box4	EQU	$B8
__local_f4__0	EQU	$BB
__local_f4__1	EQU	$BC
__local_f4__2	EQU	$BD
__local_f4__3	EQU	$BE
__local_f4__p3_1	EQU	$BF
__local_f4__p3_0	EQU	$C0
__local_rec__0	EQU	$C3
__local_rec__1	EQU	$C4
__local_rec__2	EQU	$C5
__local_rec__3	EQU	$C6
__local_rec__4	EQU	$C7

; @zp-link-meta-begin
; def f0 params=__zpabi_f0__p0_0,__zpabi_f0__p0_1,__zpabi_f0__p0_2,__zpabi_f0__p0_3,__zpabi_f0__p1 locals=__local_f0__0,__local_f0__p1,__local_f0__1,__local_f0__2,__local_f0__3,__local_f0__4,__local_f0__5,__local_f0__6,__local_f0__box4,__local_f0__8 indirect=false in_cycle=false param_regs=,,,,A carry=true
; def f1 params=__zpabi_f1__pp_0,__zpabi_f1__pp_1 locals=__local_f1__0,__local_f1__l2_0,__local_f1__1,__local_f1__2,__local_f1__l2_3,__local_f1__l2_2,__local_f1__3,__local_f1__l2_1,__local_f1__4,__local_f1__5,__local_f1__6,__local_f1__7,__local_f1__8,__local_f1__box4,__local_f1__10,__local_f1__box2,__local_f1__12,__local_f1__13,__local_f1__14,__local_f1__15 indirect=false in_cycle=false param_regs=A,X
; def f2 params=__zpabi_f2__pp_0,__zpabi_f2__pp_1 locals=__local_f2__0,__local_f2__1,__local_f2__2,__local_f2__3,__local_f2__4,__local_f2__5,__local_f2__6,__local_f2__7,__local_f2__8,__local_f2__9,__local_f2__pp,__local_f2__10,__local_f2__box4,__local_f2__12 indirect=false in_cycle=false param_regs=A,X carry=true
; def f3 params=__zpabi_f3__p0,__zpabi_f3__p1_0,__zpabi_f3__p1_1,__zpabi_f3__p1_2,__zpabi_f3__p1_3,__zpabi_f3__p2 locals=__local_f3__0,__local_f3__1,__local_f3__2,__local_f3__3,__local_f3__p0,__local_f3__4,__local_f3__box4,__local_f3__6,__local_f3__7 indirect=false in_cycle=false param_regs=A,,,,,
; def f4 params=__zpabi_f4__p0,__zpabi_f4__p1_0,__zpabi_f4__p1_1,__zpabi_f4__p1_2,__zpabi_f4__p1_3,__zpabi_f4__p2_0,__zpabi_f4__p2_1,__zpabi_f4__p2_2,__zpabi_f4__p2_3,__zpabi_f4__p3_0,__zpabi_f4__p3_1,__zpabi_f4__pp_0,__zpabi_f4__pp_1 locals=__local_f4__0,__local_f4__1,__local_f4__2,__local_f4__3,__local_f4__p3_1,__local_f4__p3_0,__local_f4__4,__local_f4__5 indirect=false in_cycle=false param_regs=,,,,,,,,,A,X,, carry=true
; def main params= locals=__local_main__0,__local_main__1,__local_main__2,__local_main__3 indirect=false in_cycle=false
; def rec params= locals=__local_rec__0,__local_rec__1,__local_rec__2,__local_rec__3,__local_rec__4 indirect=false in_cycle=true
; call f0 -> f4
; call f1 -> f2
; call f1 -> f4
; call f1 -> rec
; call f2 -> f3
; call f2 -> f4
; call f3 -> f4
; call f4 -> rec
; call main -> f0
; call rec -> rec saved
; item rec section=.text local=false zero=false
; item f4 section=.text local=false zero=false
; item f3 section=.text local=false zero=false
; item f2 section=.text local=false zero=false
; item f1 section=.text local=false zero=false
; item f0 section=.text local=false zero=false
; item main section=.text local=false zero=false
; item __zp_init section=.text local=false zero=false
; zp g0 size=2 at=auto def=true init=$A6,$00 weight=27
; runtime hargs_size=32 hargs_need=22
; @zp-link-meta-end

rec:
   SUBROUTINE

   ; prologue: 4 arg bytes, 0 local bytes
   SEC
   LDA   SSP
   SBC   #$02
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$01
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   LDY   #$05
   LDA   (FP),Y
   ASL
   ASL
   ASL
   STA   __local_rec__0
   SEC
   SBC   (FP),Y
   STA   __local_rec__4
   DEY
   LDA   (FP),Y
   BMI   .jcmp_neg@0
   DEY
   ORA   (FP),Y
   BNE   .if_end@0
.jcmp_neg@0:
   LDY   #$05
   LDA   (FP),Y
   STA   __local_rec__0
   INY
   LDA   (FP),Y
   LDX   #$00
   LDA   __local_rec__0

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$06
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$01
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   PLA
   TAX
   PLA
   RTS
.if_end@0:
   LDY   #$05
   LDA   (FP),Y
   ASL
   STA   __local_rec__2
   INY
   LDA   (FP),Y
   ROL
   STA   __local_rec__3
   LDA   __local_rec__2
   CLC
   DEY
   ADC   (FP),Y
   STA   __local_rec__1
   LDA   __local_rec__3
   INY
   ADC   (FP),Y
   STA   __local_rec__0
   LDA   __local_rec__1
   PHA
   LDA   __local_rec__0
   PHA
   LDA   __local_rec__1
   CLC
   LDY   #$03
   ADC   (FP),Y
   STA   __local_rec__1
   LDA   __local_rec__0
   INY
   ADC   (FP),Y
   STA   __local_rec__0
   DEY
   LDA   (FP),Y
   SEC
   SBC   #$01
   STA   __local_rec__3
   INY
   LDA   (FP),Y
   SBC   #$00
   STA   __local_rec__2
   LDA   __local_rec__0
   AND   #$03
   STA   __local_rec__0
   LDA   __local_rec__4
   PHA
   SEC
   LDA   SSP
   SBC   #$04
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   __local_rec__3
   LDY   #$01
   STA   (SSP),Y
   LDA   __local_rec__2
   INY
   STA   (SSP),Y
   LDA   __local_rec__1
   INY
   STA   (SSP),Y
   LDA   __local_rec__0
   INY
   STA   (SSP),Y
   JSR   rec
   STA   __local_rec__4
   STX   __local_rec__3
   PLA
   STA   __local_rec__2
   PLA
   PLA
   STA   __local_rec__0
   LDA   __local_rec__4
   CLC
   ADC   __local_rec__2
   STA   __local_rec__2
   BCC   .carry_inc@0
   INC   __local_rec__3
.carry_inc@0:
   LDA   __local_rec__0
   AND   #$0F
   CLC
   ADC   __local_rec__2
   STA   __local_rec__0
   LDA   __local_rec__3
   ADC   #$00
   AND   #$03
   TAX
   LDA   __local_rec__0

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$06
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$01
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   PLA
   TAX
   PLA
   RTS

f4:
   SUBROUTINE

   STA   __local_f4__p3_0
   STX   __local_f4__p3_1
   LDY   #$00
   LDA   (__zpabi_f4__pp_0),Y
   STA   __local_f4__3
   INY
   LDA   (__zpabi_f4__pp_0),Y
   STA   __local_f4__2
   LDA   __zpabi_f4__p1_0
   EOR   __local_f4__p3_0
   STA   __local_f4__1
   LDA   __zpabi_f4__p1_1
   EOR   __local_f4__p3_1
   STA   __local_f4__0
   LDA   __local_f4__3
   CLC
   ADC   __local_f4__1
   STA   __local_f4__1
   LDA   __local_f4__2
   ADC   __local_f4__0
   STA   (__zpabi_f4__pp_0),Y
   LDA   __local_f4__1
   DEY
   STA   (__zpabi_f4__pp_0),Y
   LDA   __zpabi_f4__p0
   BMI   .sx_neg@1
   LDA   #$00
   BEQ   .sx_done@2
.sx_neg@1:
   LDA   #$FF
.sx_done@2:
   STA   __local_f4__0
   LDA   __zpabi_f4__p0
   ORA   #$01
   STA   HARGS+2
   LDA   __local_f4__0
   STA   HARGS+3
   LDA   #$E8
   STA   HARGS
   LDA   #$03
   STA   HARGS+1
   JSR   sdivmod16
   LDA   HARGS+4
   STA   __local_f4__1
   LDA   HARGS+5
   STA   __local_f4__0
   SEC
   LDA   SSP
   SBC   #$04
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   #$03
   LDY   #$01
   STA   (SSP),Y
   LDA   #$00
   INY
   STA   (SSP),Y
   LDA   __local_f4__1
   INY
   STA   (SSP),Y
   LDA   __local_f4__0
   INY
   STA   (SSP),Y
   JSR   rec
   STA   __local_f4__3
   STX   __local_f4__2
   TXA
   BMI   .sx_neg@3
   LDA   #$00
   BEQ   .sx_done@4
.sx_neg@3:
   LDA   #$FF
.sx_done@4:
   STA   __local_f4__1
   STA   __local_f4__0
   LDA   __local_f4__p3_0
   CLC
   ADC   __local_f4__3
   STA   __local_f4__p3_0
   LDA   __local_f4__p3_1
   ADC   __local_f4__2
   STA   __local_f4__p3_1
   LDA   #$00
   ADC   __local_f4__1
   STA   __local_f4__3
   LDA   #$00
   ADC   __local_f4__0
   STA   __local_f4__2
   LDA   __local_f4__p3_0
   SEC
   SBC   g0
   LDA   __local_f4__p3_1
   SBC   g0+1
   LDA   __local_f4__3
   SBC   #$00
   LDA   __local_f4__2
   SBC   #$00
   BVC   .cmp_novf@5
   EOR   #$80
.cmp_novf@5:
   ASL
   LDA   #$00
   ROL
   EOR   #$FF
   STA   __local_f4__1
   LDA   #$00
   EOR   #$FF
   ORA   __local_f4__1
   BEQ   .cmp_end@6
   SEC
   RTS
.cmp_end@6:
   CMP   #$01
   RTS

f3:
   SUBROUTINE

   STA   __local_f3__p0
   LDA   __zpabi_f3__p2
   BMI   .sx_neg@7
   LDA   #$00
   BEQ   .sx_done@8
.sx_neg@7:
   LDA   #$FF
.sx_done@8:
   STA   __local_f3__3
   LDA   g0
   AND   #$07
   TAX
   LDA   __zpabi_f3__p2
   STA   __local_f3__1
   LDA   __local_f3__3
   STA   __zpabi_f4__p1_0
   CPX   #$00
   BEQ   .shift_done@10
.shift_loop@9:
   LDA   __zpabi_f4__p1_0
   ASL
   ROR   __zpabi_f4__p1_0
   ROR   __local_f3__1
   DEX
   BNE   .shift_loop@9
.shift_done@10:
   LDA   __local_f3__p0
   BMI   .sx_neg@11
   LDA   #$00
   BEQ   .sx_done@12
.sx_neg@11:
   LDA   #$FF
.sx_done@12:
   STA   __local_f3__0
   LDA   g0+1
   CMP   __local_f3__0
   BNE   .cmp_true@13
   LDA   g0
   CMP   __local_f3__p0
   BNE   .cmp_true@13
   LDA   #$00
   BEQ   .cmp_end@14
.cmp_true@13:
   LDA   #$01
.cmp_end@14:
   STA   __local_f3__0
   LDA   __local_f3__1
   SEC
   SBC   __local_f3__0
   STA   __local_f3__4
   LDA   __zpabi_f3__p1_0
   ORA   __zpabi_f3__p1_1
   ORA   __zpabi_f3__p1_2
   ORA   __zpabi_f3__p1_3
   BEQ   .lnot_true@16
   LDA   #$00
   BEQ   .lnot_end@17
.lnot_true@16:
   LDA   #$01
.lnot_end@17:
   STA   __local_f3__1
   LDA   __zpabi_f3__p2
   SEC
   SBC   #$07
   LDA   __local_f3__3
   SBC   #$00
   BVC   .cmp_novf@18
   EOR   #$80
.cmp_novf@18:
   ASL
   LDA   #$00
   ROL
   EOR   #$01
   EOR   __local_f3__1
   STA   __local_f3__p0
   LDA   __local_f3__4
   STA   __local_f3__box4
   BMI   .sx_neg@19
   LDA   #$00
   BEQ   .sx_done@20
.sx_neg@19:
   LDA   #$FF
.sx_done@20:
   STA   __local_f3__box4+1
   LDA   __local_f3__p0
   ORA   #$01
   STA   HARGS+1
   LDA   __local_f3__p0
   STA   HARGS
   JSR   udivmod8
   LDA   g0
   SEC
   SBC   __zpabi_f3__p1_0
   LDA   g0+1
   SBC   __zpabi_f3__p1_1
   LDA   #$00
   SBC   __zpabi_f3__p1_2
   LDA   #$00
   SBC   __zpabi_f3__p1_3
   BVC   .cmp_novf@21
   EOR   #$80
.cmp_novf@21:
   ASL
   LDA   #$00
   ROL
   STA   __zpabi_f4__p1_0
   LDA   #$00
   BPL   .sx_done@23
   LDA   #$FF
.sx_done@23:
   STA   __zpabi_f4__p1_2
   STA   __zpabi_f4__p1_3
   LDA   __zpabi_f3__p1_0
   EOR   #$FF
   STA   __zpabi_f4__p2_0
   LDA   __zpabi_f3__p1_1
   EOR   #$FF
   STA   __zpabi_f4__p2_1
   LDA   __zpabi_f3__p1_2
   EOR   #$FF
   STA   __zpabi_f4__p2_2
   LDA   __zpabi_f3__p1_3
   EOR   #$FF
   STA   __zpabi_f4__p2_3
   LDA   HARGS+2
   STA   __zpabi_f4__p0
   LDA   #$00
   STA   __zpabi_f4__p1_1
   LDA   #<__local_f3__box4
   STA   __zpabi_f4__pp_0
   LDA   #>__local_f3__box4
   STA   __zpabi_f4__pp_1
   LDX   #$00
   LDA   __local_f3__p0
   JSR   f4
   LDA   __local_f3__4
   BMI   .sx_neg@24
   LDA   #$00
   BEQ   .sx_done@25
.sx_neg@24:
   LDA   #$FF
.sx_done@25:
   STA   __zpabi_f4__p1_0
   STA   __local_f3__1
   STA   __local_f3__0
   LDA   __local_f3__4
   SEC
   SBC   __zpabi_f3__p1_0
   LDA   __zpabi_f4__p1_0
   SBC   __zpabi_f3__p1_1
   LDA   __local_f3__1
   SBC   __zpabi_f3__p1_2
   LDA   __local_f3__0
   SBC   __zpabi_f3__p1_3
   BVC   .cmp_novf@26
   EOR   #$80
.cmp_novf@26:
   ASL
   LDA   #$00
   ROL
   EOR   #$01
   STA   __local_f3__1
   LDA   #$00
   BPL   .sx_done@28
   LDA   #$FF
.sx_done@28:
   STA   HARGS+10
   STA   HARGS+11
   LDA   __local_f3__1
   STA   HARGS+8
   LDA   #$00
   STA   HARGS+9
   RTS

f2:
   SUBROUTINE

   STA   __local_f2__pp
   STX   __local_f2__10
   LDA   g0
   ORA   g0+1
   BEQ   .and_false@1
   LDA   #$01
   STA   __local_f2__9
   JMP   .and_end@2
.and_false@1:
   LDA   #$00
   STA   __local_f2__9
.and_end@2:
   LDA   #$00
   CLC
   SBC   __local_f2__9
   LDA   #$80
   SBC   #$00
   LDA   #$00
   ROL
   STA   __local_f2__0
   LDA   #$00
   BNE   .cmp_end@30
   CMP   __local_f2__0
   BNE   .cmp_end@30
   LDA   #$01
.cmp_end@30:
   STA   __local_f2__8
   LDA   #$00
   BPL   .sx_done@33
   LDA   #$FF
.sx_done@33:
   STA   __local_f2__7
   STA   __local_f2__6
   LDY   #$00
   LDA   (__local_f2__pp),Y
   STA   __local_f2__0
   INY
   LDA   (__local_f2__pp),Y
   STA   __local_f2__1
   INC   __local_f2__0
   BNE   .inc_done@0
   INC   __local_f2__1
.inc_done@0:
   LDA   __local_f2__0
   LDY   #$00
   STA   (__local_f2__pp),Y
   LDA   __local_f2__1
   INY
   STA   (__local_f2__pp),Y
   LDA   #$00
   STA   __local_f2__box4
   LDA   #$80
   STA   __local_f2__box4+1
   DEY
   LDA   (__local_f2__pp),Y
   STA   __local_f2__5
   LDA   g0
   ORA   #$01
   STA   HARGS+2
   LDA   g0+1
   STA   HARGS+3
   LDA   __local_f2__9
   STA   HARGS
   LDA   #$00
   STA   HARGS+1
   JSR   udivmod16
   LDA   __local_f2__8
   SEC
   SBC   #$FF
   LDA   #$00
   SBC   #$00
   LDA   __local_f2__7
   SBC   #$00
   LDA   __local_f2__6
   SBC   #$00
   BVC   .cmp_novf@34
   EOR   #$80
.cmp_novf@34:
   ASL
   LDA   #$00
   ROL
   STA   __zpabi_f4__p2_0
   LDA   #$00
   BPL   .sx_done@36
   LDA   #$FF
.sx_done@36:
   STA   __zpabi_f4__p2_2
   STA   __zpabi_f4__p2_3
   LDA   __local_f2__5
   STA   __zpabi_f4__p0
   LDA   HARGS+4
   STA   __zpabi_f4__p1_0
   LDA   HARGS+5
   STA   __zpabi_f4__p1_1
   LDA   #$00
   STA   __zpabi_f4__p1_2
   STA   __zpabi_f4__p1_3
   STA   __zpabi_f4__p2_1
   LDA   #<__local_f2__box4
   STA   __zpabi_f4__pp_0
   LDA   #>__local_f2__box4
   STA   __zpabi_f4__pp_1
   LDX   #$00
   LDA   #$FF
   JSR   f4
   LDA   #$00
   ROL
   STA   __local_f2__0
   LDA   __local_f2__9
   EOR   __local_f2__0
   CLC
   ADC   __local_f2__box4
   STA   __local_f2__4
   LDA   __local_f2__8
   ORA   #$01
   STA   HARGS+4
   LDA   __local_f2__7
   STA   HARGS+6
   LDA   __local_f2__6
   STA   HARGS+7
   LDA   #$00
   STA   HARGS+5
   STA   HARGS
   LDY   #$80
   STY   HARGS+1
   LDY   #$00
   STY   HARGS+2
   STA   HARGS+3
   JSR   sdivmod32
   LDA   HARGS+12
   STA   __local_f2__3
   LDY   #$00
   LDA   (__local_f2__pp),Y
   ORA   #$01
   STA   HARGS+2
   INY
   LDA   (__local_f2__pp),Y
   STA   HARGS+3
   LDA   __local_f2__4
   STA   HARGS
   LDA   #$00
   STA   HARGS+1
   JSR   sdivmod16
   LDA   HARGS+7
   BMI   .sx_neg@37
   LDA   #$00
   BEQ   .sx_done@38
.sx_neg@37:
   LDA   #$FF
.sx_done@38:
   STA   __zpabi_f3__p1_2
   STA   __zpabi_f3__p1_3
   LDA   HARGS+6
   STA   __zpabi_f3__p1_0
   LDA   HARGS+7
   STA   __zpabi_f3__p1_1
   LDA   g0
   STA   __zpabi_f3__p2
   LDA   __local_f2__3
   JSR   f3
   LDA   __local_f2__8
   EOR   HARGS+8
   STA   __local_f2__1
   LDA   #$00
   EOR   HARGS+9
   STA   __local_f2__0
   LDA   g0
   CLC
   ADC   __local_f2__1
   STA   g0
   LDA   g0+1
   ADC   __local_f2__0
   STA   g0+1
   LDA   #$80
   BNE   .and_false@3
   LDA   #$00
   CMP   __local_f2__4
   BNE   .and_end@4
   SEC
   RTS
.and_false@3:
   CLC
   RTS
.and_end@4:
   CMP   #$01
   RTS

f1:
   SUBROUTINE

   STA   __local_f1__l2_3
   STX   __local_f1__l2_2
   LDA   g0
   STA   __local_f1__8
   LDA   g0+1
   STA   __local_f1__7
   LDA   g0
   ORA   g0+1
   BEQ   .lnot_true@39
   LDA   #$00
   BEQ   .lnot_end@40
.lnot_true@39:
   LDA   #$01
.lnot_end@40:
   STA   __local_f1__0
   LDA   #$00
   BPL   .sx_done@42
   LDA   #$FF
.sx_done@42:
   STA   HARGS+2
   STA   HARGS+3
   LDA   __local_f1__8
   ORA   #$01
   STA   HARGS+4
   LDA   __local_f1__7
   STA   HARGS+5
   LDA   #$00
   STA   HARGS+6
   LDY   #$00
   STY   HARGS+7
   STA   HARGS+1
   LDA   __local_f1__0
   STA   HARGS
   JSR   sdivmod32
   LDA   HARGS+8
   STA   __local_f1__6
   LDA   HARGS+9
   STA   __local_f1__2
   LDA   g0
   STA   __local_f1__1
   LDA   g0+1
   STA   __local_f1__l2_1
   LDA   __local_f1__8
   ORA   __local_f1__7
   BEQ   .and_false@5
   LDA   __local_f1__1
   ORA   __local_f1__l2_1
   BNE   .and_end@6
.and_false@5:
.and_end@6:
   LDY   #$00
   LDA   (__local_f1__l2_3),Y
   CLC
   ADC   __local_f1__1
   STA   __local_f1__l2_0
   INY
   LDA   (__local_f1__l2_3),Y
   ADC   __local_f1__l2_1
   STA   (__local_f1__l2_3),Y
   LDA   __local_f1__l2_0
   DEY
   STA   (__local_f1__l2_3),Y
   LDA   __local_f1__8
   STA   __local_f1__box4
   LDA   __local_f1__7
   STA   __local_f1__box4+1
   LDA   g0
   ORA   #$01
   STA   HARGS+4
   LDA   g0+1
   STA   HARGS+5
   LDA   __local_f1__8
   STA   HARGS
   LDA   __local_f1__7
   STA   HARGS+1
   LDA   #$00
   STA   HARGS+2
   STY   HARGS+3
   STY   HARGS+6
   STA   HARGS+7
   JSR   sdivmod32
   LDA   HARGS+12
   STA   __zpabi_f4__p0
   LDA   __local_f1__6
   EOR   #$E8
   STA   __zpabi_f4__p1_0
   LDA   __local_f1__2
   EOR   #$03
   STA   __zpabi_f4__p1_1
   LDA   #$00
   STA   __zpabi_f4__p1_2
   STA   __zpabi_f4__p1_3
   STA   __zpabi_f4__p2_1
   STA   __zpabi_f4__p2_2
   STA   __zpabi_f4__p2_3
   LDA   #$02
   STA   __zpabi_f4__p2_0
   LDA   #<__local_f1__box4
   STA   __zpabi_f4__pp_0
   LDA   #>__local_f1__box4
   STA   __zpabi_f4__pp_1
   LDX   #$FF
   LDA   #$FE
   JSR   f4
   BCC   .if_else@10
   LDA   __local_f1__1
   CLC
   ADC   #$03
   STA   __local_f1__3
   LDA   __local_f1__l2_1
   ADC   #$00
   STA   __local_f1__l2_1
   LDA   #$00
   ADC   #$00
   STA   __local_f1__l2_2
   LDA   #$00
   ADC   #$00
   STA   __local_f1__l2_3
   JMP   .if_end@9
.if_else@10:
   LDA   __local_f1__1
   EOR   #$05
   STA   __local_f1__3
   LDA   #$00
   STA   __local_f1__l2_2
   STA   __local_f1__l2_3
.if_end@9:
   LDA   __local_f1__box4
   STA   __local_f1__1
   LDA   __local_f1__box4+1
   STA   __local_f1__2
   BMI   .sx_neg@43
   LDA   #$00
   BEQ   .sx_done@44
.sx_neg@43:
   LDA   #$FF
.sx_done@44:
   STA   __local_f1__l2_0
   STA   __local_f1__0
   LDA   __local_f1__3
   CLC
   ADC   __local_f1__1
   STA   __local_f1__1
   LDA   __local_f1__l2_1
   ADC   __local_f1__2
   STA   __local_f1__5
   LDA   __local_f1__l2_2
   ADC   __local_f1__l2_0
   STA   __local_f1__4
   LDA   __local_f1__l2_3
   ADC   __local_f1__0
   STA   __local_f1__l2_1
   LDA   g0
   CLC
   ADC   __local_f1__1
   STA   g0
   LDA   g0+1
   ADC   __local_f1__5
   STA   g0+1
   LDA   __local_f1__8
   STA   __local_f1__box2
   LDA   __local_f1__7
   STA   __local_f1__box2+1
   LDX   #>__local_f1__box2
   LDA   #<__local_f1__box2
   JSR   f2
   BCC   .if_else@12
   LDA   __local_f1__1
   CLC
   ADC   #$03
   STA   __local_f1__l2_0
   BCC   .if_end@11
   INC   __local_f1__5
   BNE   .if_end@11
   INC   __local_f1__4
   BNE   .if_end@11
   INC   __local_f1__l2_1
   JMP   .if_end@11
.if_else@12:
   LDA   __local_f1__1
   EOR   #$05
   STA   __local_f1__l2_0
.if_end@11:
   LDA   __local_f1__box2
   STA   __local_f1__0
   LDA   __local_f1__box2+1
   STA   __local_f1__3
   BMI   .sx_neg@45
   LDA   #$00
   BEQ   .sx_done@46
.sx_neg@45:
   LDA   #$FF
.sx_done@46:
   STA   __local_f1__l2_2
   STA   __local_f1__l2_3
   LDA   __local_f1__l2_0
   CLC
   ADC   __local_f1__0
   STA   __local_f1__0
   LDA   __local_f1__5
   ADC   __local_f1__3
   STA   __local_f1__l2_0
   LDA   __local_f1__4
   ADC   __local_f1__l2_2
   STA   __local_f1__1
   LDA   __local_f1__l2_1
   ADC   __local_f1__l2_3
   STA   __local_f1__2
   LDA   g0
   CLC
   ADC   __local_f1__0
   STA   g0
   LDA   g0+1
   ADC   __local_f1__l2_0
   STA   g0+1
   LDA   __local_f1__2
   ASL
   ROR   __local_f1__2
   ROR   __local_f1__1
   ROR   __local_f1__l2_0
   ROR   __local_f1__0
   SEC
   LDA   SSP
   SBC   #$04
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   #$03
   LDY   #$01
   STA   (SSP),Y
   LDA   #$00
   INY
   STA   (SSP),Y
   LDA   __local_f1__0
   INY
   STA   (SSP),Y
   LDA   __local_f1__l2_0
   INY
   STA   (SSP),Y
   JSR   rec
   LDA   #$01
   SEC
   SBC   __local_f1__6
   RTS

f0:
   SUBROUTINE

   STA   __local_f0__p1
   LDA   g0
   CLC
   SBC   g0
   LDA   g0+1
   SBC   g0+1
   LDA   #$00
   ROL
   AND   g0
   STA   __local_f0__6
   LDA   #$00
   AND   g0+1
   STA   __local_f0__5
   STA   __local_f0__box4+1
   LDA   __local_f0__6
   STA   __local_f0__box4
   LDA   g0
   ORA   g0+1
   BEQ   .and_false@13
   LDA   __zpabi_f0__p0_0
   ORA   __zpabi_f0__p0_1
   ORA   __zpabi_f0__p0_2
   ORA   __zpabi_f0__p0_3
   BEQ   .and_false@13
   LDA   #$01
   STA   __zpabi_f4__p0
   JMP   .and_end@14
.and_false@13:
   LDA   #$00
   STA   __zpabi_f4__p0
.and_end@14:
   LDA   __local_f0__p1
   AND   #$07
   TAY
   LDA   #$07
   STA   __zpabi_f4__p1_0
   TYA
   TAX
   BEQ   .shift_done@48
.shift_loop@47:
   LSR   __zpabi_f4__p1_0
   DEX
   BNE   .shift_loop@47
.shift_done@48:
   LDA   __local_f0__6
   STA   __zpabi_f4__p2_0
   LDA   __local_f0__5
   STA   __zpabi_f4__p2_1
   TYA
   TAX
   BEQ   .shift_done@50
.shift_loop@49:
   ASL   __zpabi_f4__p2_0
   ROL   __zpabi_f4__p2_1
   DEX
   BNE   .shift_loop@49
.shift_done@50:
   LDA   __zpabi_f0__p0_0
   BMI   .sx_neg@51
   LDA   #$00
   BEQ   .sx_done@52
.sx_neg@51:
   LDA   #$FF
.sx_done@52:
   TAX
   LDA   #$00
   STA   __zpabi_f4__p1_1
   STA   __zpabi_f4__p1_2
   STA   __zpabi_f4__p1_3
   STA   __zpabi_f4__p2_2
   STA   __zpabi_f4__p2_3
   LDA   #<__local_f0__box4
   STA   __zpabi_f4__pp_0
   LDA   #>__local_f0__box4
   STA   __zpabi_f4__pp_1
   LDA   __zpabi_f0__p0_0
   JSR   f4
   LDA   #$00
   ROL
   STA   __local_f0__0
   LDA   __local_f0__6
   EOR   __local_f0__0
   STA   __zpabi_f4__p2_1
   LDA   __local_f0__5
   STA   __zpabi_f4__p2_0
   LDA   __local_f0__box4
   STA   __local_f0__p1
   LDA   __local_f0__box4+1
   STA   __local_f0__0
   LDA   __zpabi_f4__p2_1
   CLC
   ADC   __local_f0__p1
   STA   __local_f0__p1
   LDA   __zpabi_f4__p2_0
   ADC   __local_f0__0
   STA   __local_f0__0
   LDA   g0
   CLC
   ADC   __local_f0__p1
   STA   g0
   LDA   g0+1
   ADC   __local_f0__0
   STA   g0+1
   SEC
   RTS

main:
   SUBROUTINE

   LDA   #$FD
   STA   __zpabi_f0__p0_0
   LDA   #$00
   STA   __zpabi_f0__p0_1
   STA   __zpabi_f0__p0_2
   STA   __zpabi_f0__p0_3
   LDA   #$17
   JSR   f0
   LDA   #$00
   ROL
   EOR   g0
   STA   __local_main__3
   LDA   #$00
   EOR   g0+1
   STA   __local_main__2
   LDA   __local_main__3
   LDX   __local_main__2
   RTS

__zp_init:
   SUBROUTINE

   LDA   #$00
   STA   g0+1
   LDA   #$A6
   STA   g0
   RTS
