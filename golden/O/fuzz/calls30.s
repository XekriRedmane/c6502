SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
g0	EQU	$26
__zpabi_f0__p1_0	EQU	$80
__zpabi_f1__p0_0	EQU	$80
__zpabi_f2__p2_0	EQU	$80
__zpabi_f3__p1_0	EQU	$80
__zpabi_f0__p1_1	EQU	$81
__zpabi_f1__p0_1	EQU	$81
__zpabi_f2__p2_1	EQU	$81
__zpabi_f3__p1_1	EQU	$81
__zpabi_f0__p1_2	EQU	$82
__zpabi_f1__p0_2	EQU	$82
__zpabi_f2__p2_2	EQU	$82
__zpabi_f3__p1_2	EQU	$82
__zpabi_f0__p1_3	EQU	$83
__zpabi_f1__p0_3	EQU	$83
__zpabi_f2__p2_3	EQU	$83
__zpabi_f3__p1_3	EQU	$83
__zpabi_f0__pp_0	EQU	$84
__zpabi_f1__p1_0	EQU	$84
__zpabi_f3__p2_0	EQU	$84
__zpabi_f0__pp_1	EQU	$85
__zpabi_f1__p1_1	EQU	$85
__zpabi_f3__p2_1	EQU	$85
__local_main__0	EQU	$86
__zpabi_f1__p2_0	EQU	$86
__zpabi_f3__p3_0	EQU	$86
__local_main__1	EQU	$87
__zpabi_f1__p2_1	EQU	$87
__zpabi_f3__p3_1	EQU	$87
__local_main__2	EQU	$88
__zpabi_f1__p2_2	EQU	$88
__zpabi_f3__p3_2	EQU	$88
__local_main__3	EQU	$89
__zpabi_f1__p2_3	EQU	$89
__zpabi_f3__p3_3	EQU	$89
__local_main__4	EQU	$8A
__zpabi_f1__p3_0	EQU	$8A
__zpabi_f4__p0_0	EQU	$8A
__local_main__5	EQU	$8B
__zpabi_f1__p3_1	EQU	$8B
__zpabi_f4__p0_1	EQU	$8B
__local_f1__0	EQU	$8C
__local_main__6	EQU	$8C
__zpabi_f4__p1_0	EQU	$8C
__local_f1__1	EQU	$8D
__local_main__7	EQU	$8D
__zpabi_f4__p1_1	EQU	$8D
__local_f1__2	EQU	$8E
__local_main__8	EQU	$8E
__zpabi_f4__p2	EQU	$8E
__local_f1__3	EQU	$8F
__local_f2__0	EQU	$8F
__local_f3__p0	EQU	$8F
__local_main__9	EQU	$8F
__local_f2__1	EQU	$90
__local_f3__0	EQU	$90
__local_main__10	EQU	$90
__local_f2__2	EQU	$91
__local_f3__1	EQU	$91
__local_main__11	EQU	$91
__local_f2__3	EQU	$92
__local_f3__2	EQU	$92
__local_main__mbox	EQU	$92
__local_f2__4	EQU	$93
__local_f2__p0	EQU	$94
__local_f0__0	EQU	$96
__local_f0__1	EQU	$97
__local_f0__2	EQU	$98
__local_f0__3	EQU	$99
__local_f0__4	EQU	$9A
__local_f4__p3_1	EQU	$9A
__local_f0__p0	EQU	$9B
__local_f4__p3_0	EQU	$9B
__local_rec__0	EQU	$9C
__local_rec__1	EQU	$9D
__local_rec__2	EQU	$9E
__local_rec__3	EQU	$9F
__local_rec__4	EQU	$A0

; @zp-link-meta-begin
; def f0 params=__zpabi_f0__p0,__zpabi_f0__p1_0,__zpabi_f0__p1_1,__zpabi_f0__p1_2,__zpabi_f0__p1_3,__zpabi_f0__p2,__zpabi_f0__pp_0,__zpabi_f0__pp_1 locals=__local_f0__0,__local_f0__1,__local_f0__2,__local_f0__3,__local_f0__4,__local_f0__p0 indirect=false in_cycle=false param_regs=A,,,,,X,,
; def f1 params=__zpabi_f1__p0_0,__zpabi_f1__p0_1,__zpabi_f1__p0_2,__zpabi_f1__p0_3,__zpabi_f1__p1_0,__zpabi_f1__p1_1,__zpabi_f1__p2_0,__zpabi_f1__p2_1,__zpabi_f1__p2_2,__zpabi_f1__p2_3,__zpabi_f1__p3_0,__zpabi_f1__p3_1 locals=__local_f1__0,__local_f1__1,__local_f1__2,__local_f1__3 indirect=false in_cycle=false
; def f2 params=__zpabi_f2__p0,__zpabi_f2__p1,__zpabi_f2__p2_0,__zpabi_f2__p2_1,__zpabi_f2__p2_2,__zpabi_f2__p2_3 locals=__local_f2__0,__local_f2__1,__local_f2__2,__local_f2__3,__local_f2__4,__local_f2__p0,__local_f2__5,__local_f2__6,__local_f2__7 indirect=false in_cycle=false param_regs=A,X,,,,
; def f3 params=__zpabi_f3__p0,__zpabi_f3__p1_0,__zpabi_f3__p1_1,__zpabi_f3__p1_2,__zpabi_f3__p1_3,__zpabi_f3__p2_0,__zpabi_f3__p2_1,__zpabi_f3__p3_0,__zpabi_f3__p3_1,__zpabi_f3__p3_2,__zpabi_f3__p3_3 locals=__local_f3__p0,__local_f3__0,__local_f3__1,__local_f3__2,__local_f3__3,__local_f3__4,__local_f3__5 indirect=false in_cycle=false param_regs=A,,,,,,,,,, carry=true
; def f4 params=__zpabi_f4__p0_0,__zpabi_f4__p0_1,__zpabi_f4__p1_0,__zpabi_f4__p1_1,__zpabi_f4__p2,__zpabi_f4__p3_0,__zpabi_f4__p3_1 locals=__local_f4__0,__local_f4__1,__local_f4__p3_1,__local_f4__p3_0 indirect=false in_cycle=false param_regs=,,,,,A,X changes=__zpabi_f4__p1_0,__zpabi_f4__p1_1
; def main params= locals=__local_main__0,__local_main__1,__local_main__2,__local_main__3,__local_main__4,__local_main__5,__local_main__6,__local_main__7,__local_main__8,__local_main__9,__local_main__10,__local_main__11,__local_main__mbox,__local_main__13,__local_main__14,__local_main__15 indirect=false in_cycle=false
; def rec params= locals=__local_rec__0,__local_rec__1,__local_rec__2,__local_rec__3,__local_rec__4 indirect=false in_cycle=true
; call f0 -> rec
; call f2 -> f4
; call f2 -> rec
; call f3 -> f4
; call main -> f0
; call rec -> rec saved
; item rec section=.text local=false zero=false
; item f4 section=.text local=false zero=false
; item f3 section=.text local=false zero=false
; item f2 section=.text local=false zero=false
; item f2@tail0 section=.text local=true zero=false
; item f1 section=.text local=false zero=false
; item f0 section=.text local=false zero=false
; item main section=.text local=false zero=false
; item g1 section=.data local=false zero=false
; item g2 section=.data local=false zero=false
; item __zp_init section=.text local=false zero=false
; zp g0 size=1 at=auto def=true init=$9F weight=5
; runtime hargs_size=32 hargs_need=20
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
   LDA   g2
   AND   #$02
   BEQ   .and_false@1
   LDA   __local_f4__p3_0
   AND   #$07
   TAX
   BEQ   .shift_done@2
.shift_loop@1:
   LDA   __zpabi_f4__p1_1
   ASL
   ROR   __zpabi_f4__p1_1
   ROR   __zpabi_f4__p1_0
   DEX
   BNE   .shift_loop@1
.shift_done@2:
   LDA   __zpabi_f4__p1_0
   ORA   __zpabi_f4__p1_1
   BNE   .and_end@2
.and_false@1:
.and_end@2:
   LDA   #$E8
   AND   __zpabi_f4__p2
   STA   __zpabi_f4__p1_1
   LDA   __local_f4__p3_0
   ORA   __local_f4__p3_1
   BNE   .or_true@3
   LDA   g1
   ORA   g1+1
   ORA   g1+2
   ORA   g1+3
   BNE   .or_true@3
   LDA   #$00
   STA   __zpabi_f4__p1_0
   JMP   .or_end@4
.or_true@3:
   LDA   #$01
   STA   __zpabi_f4__p1_0
.or_end@4:
   LDA   __zpabi_f4__p1_1
   SEC
   SBC   __zpabi_f4__p1_0
   RTS

f3:
   SUBROUTINE

   STA   __local_f3__p0
   ORA   #$00
   BMI   .sx_neg@3
   LDA   #$00
   BEQ   .sx_done@4
.sx_neg@3:
   LDA   #$FF
.sx_done@4:
   STA   __local_f3__2
   LDA   __local_f3__p0
   CLC
   ADC   #$64
   STA   __local_f3__1
   BCC   .carry_inc@1
   INC   __local_f3__2
.carry_inc@1:
   LDA   __local_f3__2
   BMI   .sx_neg@5
   LDA   #$00
   BEQ   .sx_done@6
.sx_neg@5:
   LDA   #$FF
.sx_done@6:
   STA   __local_f3__0
   STA   __local_f3__p0
   LDA   __zpabi_f3__p1_3
   CMP   __local_f3__p0
   BNE   .cmp_true@7
   LDA   __zpabi_f3__p1_2
   CMP   __local_f3__0
   BNE   .cmp_true@7
   LDA   __zpabi_f3__p1_1
   CMP   __local_f3__2
   BNE   .cmp_true@7
   LDA   __zpabi_f3__p1_0
   CMP   __local_f3__1
   BNE   .cmp_true@7
   LDA   #$00
   BEQ   .cmp_end@8
.cmp_true@7:
   LDA   #$01
.cmp_end@8:
   STA   __local_f3__2
   LDA   g1
   STA   __local_f3__1
   LDA   g1+1
   STA   __local_f3__0
   LDA   __zpabi_f3__p2_1
   BMI   .sx_neg@10
   LDA   #$00
   BEQ   .sx_done@11
.sx_neg@10:
   LDA   #$FF
.sx_done@11:
   STA   HARGS+2
   STA   HARGS+3
   LDA   __zpabi_f3__p3_0
   ORA   #$01
   STA   HARGS+4
   LDA   __zpabi_f3__p3_1
   STA   HARGS+5
   LDA   __zpabi_f3__p3_2
   STA   HARGS+6
   LDA   __zpabi_f3__p3_3
   STA   HARGS+7
   LDA   __zpabi_f3__p2_0
   STA   HARGS
   LDA   __zpabi_f3__p2_1
   STA   HARGS+1
   JSR   udivmod32
   LDX   HARGS+13
   LDA   __local_f3__1
   STA   __zpabi_f4__p0_0
   LDA   __local_f3__0
   STA   __zpabi_f4__p0_1
   LDA   __local_f3__2
   STA   __zpabi_f4__p1_0
   LDA   #$00
   STA   __zpabi_f4__p1_1
   LDA   #$64
   STA   __zpabi_f4__p2
   LDA   HARGS+12
   JSR   f4
   LDA   __zpabi_f3__p2_1
   ORA   __zpabi_f3__p2_0
   BEQ   .cmp_end@12
   SEC
   RTS
.cmp_end@12:
   CMP   #$01
   RTS

f2:
   SUBROUTINE

   STA   __local_f2__p0
   ORA   #$00
   BMI   .sx_neg@13
   LDA   #$00
   BEQ   .sx_done@14
.sx_neg@13:
   LDA   #$FF
.sx_done@14:
   STA   __local_f2__4
   LDA   __local_f2__p0
   EOR   #$FF
   STA   __local_f2__3
   LDA   __local_f2__p0
   BMI   .sx_neg@15
   LDA   #$00
   BEQ   .sx_done@16
.sx_neg@15:
   LDA   #$FF
.sx_done@16:
   CMP   g1+3
   BNE   .thread@0
   CMP   g1+2
   BNE   .thread@0
   CMP   g1+1
   BNE   .thread@0
   LDA   __local_f2__p0
   CMP   g1
   BEQ   .cmp_true@17
.thread@0:
   LDA   #$00
   BEQ   .cmp_end@18
.cmp_true@17:
   LDA   #$01
.cmp_end@18:
   ORA   __local_f2__3
   STA   __local_f2__3
   LDA   g1
   STA   __local_f2__2
   LDA   g1+1
   STA   __local_f2__1
   STX   HARGS
   LDA   #$00
   STA   HARGS+1
   LDA   __local_f2__p0
   STA   HARGS+2
   LDA   __local_f2__4
   STA   HARGS+3
   JSR   mul16
   LDX   HARGS+5
   LDA   __local_f2__2
   STA   __zpabi_f4__p0_0
   LDA   __local_f2__1
   STA   __zpabi_f4__p0_1
   LDA   g2
   STA   __zpabi_f4__p1_0
   LDA   g2+1
   STA   __zpabi_f4__p1_1
   LDA   __local_f2__3
   STA   __zpabi_f4__p2
   LDA   HARGS+4
   JSR   f4
   STA   __local_f2__1
   ORA   #$00
   BMI   .sx_neg@20
   LDA   #$00
   BEQ   .sx_done@21
.sx_neg@20:
   LDA   #$FF
.sx_done@21:
   STA   __local_f2__0
   LDA   #$07
   EOR   __local_f2__1
   STA   __local_f2__3
   LDA   #$00
   EOR   __local_f2__0
   STA   __local_f2__2
   LDA   #$07
   EOR   __local_f2__1
   CLC
   ADC   g0
   STA   g0
   LDA   __local_f2__p0
   ORA   #$01
   STA   HARGS+2
   LDA   __local_f2__4
   STA   HARGS+3
   LDA   __local_f2__3
   STA   HARGS
   LDA   __local_f2__2
   STA   HARGS+1
   JSR   sdivmod16
   LDA   HARGS+4
   STA   __local_f2__1
   LDA   HARGS+5
   STA   __local_f2__0
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
   LDA   __local_f2__1
   INY
   STA   (SSP),Y
   LDA   __local_f2__0
   INY
   STA   (SSP),Y
   JSR   rec
   LDA   #$FF
   SEC
   SBC   __local_f2__3
   LDA   #$00
   SBC   __local_f2__2
   JMP   f2@tail0

f2@tail0:
   SUBROUTINE

   BVC   .cmp_novf@22
   EOR   #$80
.cmp_novf@22:
   ASL
   LDA   #$00
   ROL
   EOR   #$01
   RTS

f1:
   SUBROUTINE

   LDA   __zpabi_f1__p1_0
   EOR   __zpabi_f1__p0_0
   STA   __local_f1__3
   LDA   __zpabi_f1__p1_1
   EOR   __zpabi_f1__p0_1
   STA   __local_f1__2
   LDA   #$00
   EOR   __zpabi_f1__p0_2
   STA   __local_f1__1
   LDA   #$00
   EOR   __zpabi_f1__p0_3
   STA   __local_f1__0
   LDA   __local_f1__3
   SEC
   SBC   __zpabi_f1__p2_0
   LDA   __local_f1__2
   SBC   __zpabi_f1__p2_1
   LDA   __local_f1__1
   SBC   __zpabi_f1__p2_2
   LDA   __local_f1__0
   SBC   __zpabi_f1__p2_3
   LDA   #$00
   ROL
   RTS

f0:
   SUBROUTINE

   STA   __local_f0__p0
   ORA   #$00
   BMI   .sx_neg@23
   LDA   #$00
   BEQ   .sx_done@24
.sx_neg@23:
   LDA   #$FF
.sx_done@24:
   STA   __local_f0__4
   LDA   __zpabi_f0__pp_0
   STA   DPTR
   LDA   __zpabi_f0__pp_1
   STA   DPTR+1
   LDY   #$00
   LDA   (__zpabi_f0__pp_0),Y
   STA   __local_f0__3
   INY
   LDA   (__zpabi_f0__pp_0),Y
   STA   __local_f0__2
   SEC
   LDA   #$00
   SBC   g1
   STA   __local_f0__1
   LDA   #$00
   SBC   g1+1
   STA   __local_f0__0
   LDA   __local_f0__3
   CLC
   ADC   __local_f0__1
   STA   __local_f0__1
   LDA   __local_f0__2
   ADC   __local_f0__0
   STA   (__zpabi_f0__pp_0),Y
   LDA   __local_f0__1
   DEY
   STA   (__zpabi_f0__pp_0),Y
   TXA
   SEC
   SBC   g1
   LDA   #$00
   SBC   g1+1
   LDA   #$00
   SBC   g1+2
   LDA   #$00
   SBC   g1+3
   LDA   #$00
   ROL
   STA   __local_f0__0
   SEC
   LDA   SSP
   SBC   #$04
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   #$03
   INY
   STA   (SSP),Y
   LDA   #$00
   INY
   STA   (SSP),Y
   LDA   __local_f0__0
   INY
   STA   (SSP),Y
   LDA   #$00
   INY
   STA   (SSP),Y
   JSR   rec
   LDA   g2
   AND   g2
   STA   __local_f0__1
   LDA   g2+1
   AND   g2+1
   STA   __local_f0__0
   LDA   __local_f0__p0
   SEC
   SBC   __local_f0__1
   LDA   __local_f0__4
   SBC   __local_f0__0
   JMP   f2@tail0

main:
   SUBROUTINE

   LDA   #$05
   STA   __local_main__mbox
   LDA   #$00
   STA   __local_main__mbox+1
   LDA   #$51
   STA   __zpabi_f0__p1_0
   LDA   #$00
   STA   __zpabi_f0__p1_1
   STA   __zpabi_f0__p1_2
   STA   __zpabi_f0__p1_3
   LDA   #<__local_main__mbox
   STA   __zpabi_f0__pp_0
   LDA   #>__local_main__mbox
   STA   __zpabi_f0__pp_1
   LDX   #$DD
   LDA   #$29
   JSR   f0
   STA   __local_main__11
   ORA   #$00
   BMI   .sx_neg@26
   LDA   #$00
   BEQ   .sx_done@27
.sx_neg@26:
   LDA   #$FF
.sx_done@27:
   STA   __local_main__10
   STA   __local_main__9
   STA   __local_main__8
   LDA   g0
   BMI   .sx_neg@28
   LDA   #$00
   BEQ   .sx_done@29
.sx_neg@28:
   LDA   #$FF
.sx_done@29:
   STA   __local_main__6
   STA   __local_main__5
   STA   __local_main__4
   LDA   g0
   EOR   g1
   STA   __local_main__7
   LDA   __local_main__6
   EOR   g1+1
   STA   __local_main__6
   LDA   __local_main__5
   EOR   g1+2
   STA   __local_main__5
   LDA   __local_main__4
   EOR   g1+3
   STA   __local_main__4
   LDA   g2+1
   BMI   .sx_neg@30
   LDA   #$00
   BEQ   .sx_done@31
.sx_neg@30:
   LDA   #$FF
.sx_done@31:
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
   DC.L  $00000035

g2:
   DC.W  $000C

__zp_init:
   SUBROUTINE

   LDA   #$9F
   STA   g0
   RTS
