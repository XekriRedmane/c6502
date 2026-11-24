SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
g1	EQU	$26
__zpabi_f0__p1_0	EQU	$80
__zpabi_f1__p0_0	EQU	$80
__zpabi_f0__p1_1	EQU	$81
__zpabi_f1__p0_1	EQU	$81
__zpabi_f0__p1_2	EQU	$82
__zpabi_f1__p0_2	EQU	$82
__zpabi_f0__p1_3	EQU	$83
__zpabi_f1__p0_3	EQU	$83
__zpabi_f0__p2	EQU	$84
__zpabi_f1__p2_0	EQU	$84
__zpabi_f0__pp_0	EQU	$85
__zpabi_f1__p2_1	EQU	$85
__zpabi_f0__pp_1	EQU	$86
__zpabi_f1__p2_2	EQU	$86
__zpabi_f1__p2_3	EQU	$87
__zpabi_f2__p1_0	EQU	$87
__local_f1__l1	EQU	$88
__zpabi_f2__p1_1	EQU	$88
__local_f1__0	EQU	$89
__zpabi_f2__p1_2	EQU	$89
__local_f1__1	EQU	$8A
__zpabi_f2__p1_3	EQU	$8A
__local_rec__0	EQU	$8B
__zpabi_f2__p2	EQU	$8B
__local_rec__1	EQU	$8C
__zpabi_f2__pp_0	EQU	$8C
__local_rec__2	EQU	$8D
__zpabi_f2__pp_1	EQU	$8D
__local_rec__3	EQU	$8E
__zpabi_f3__p1_0	EQU	$8E
__local_rec__4	EQU	$8F
__zpabi_f3__p1_1	EQU	$8F
__local_main__0	EQU	$90
__local_main__2	EQU	$92
__local_main__3	EQU	$93
__local_main__mbox	EQU	$98
__local_f0__0	EQU	$9C
__local_f0__1	EQU	$9D
__local_f0__2	EQU	$9E
__local_f0__l2	EQU	$9F
__local_f0__4	EQU	$A1
__local_f0__5	EQU	$A2
__local_f0__p0	EQU	$A3
__local_f0__box2	EQU	$A4
__local_f2__0	EQU	$A7
__local_f2__1	EQU	$A8
__local_f2__2	EQU	$A9
__local_f2__3	EQU	$AA
__local_f3__0	EQU	$AB
__local_f3__1	EQU	$AC
__local_f3__2	EQU	$AD
__local_f3__3	EQU	$AE
__local_f3__4	EQU	$AF
__local_f3__p0_1	EQU	$B0
__local_f3__p0_0	EQU	$B1

; @zp-link-meta-begin
; def f0 params=__zpabi_f0__p0,__zpabi_f0__p1_0,__zpabi_f0__p1_1,__zpabi_f0__p1_2,__zpabi_f0__p1_3,__zpabi_f0__p2,__zpabi_f0__pp_0,__zpabi_f0__pp_1 locals=__local_f0__0,__local_f0__1,__local_f0__2,__local_f0__l2,__local_f0__3,__local_f0__4,__local_f0__5,__local_f0__p0,__local_f0__box2,__local_f0__7,__local_f0__8 indirect=false in_cycle=false param_regs=A,,,,,,,
; def f1 params=__zpabi_f1__p0_0,__zpabi_f1__p0_1,__zpabi_f1__p0_2,__zpabi_f1__p0_3,__zpabi_f1__p1,__zpabi_f1__p2_0,__zpabi_f1__p2_1,__zpabi_f1__p2_2,__zpabi_f1__p2_3 locals=__local_f1__l1,__local_f1__0,__local_f1__1 indirect=false in_cycle=false param_regs=,,,,A,,,, carry=true
; def f2 params=__zpabi_f2__p0_0,__zpabi_f2__p0_1,__zpabi_f2__p1_0,__zpabi_f2__p1_1,__zpabi_f2__p1_2,__zpabi_f2__p1_3,__zpabi_f2__p2,__zpabi_f2__pp_0,__zpabi_f2__pp_1 locals=__local_f2__0,__local_f2__1,__local_f2__2,__local_f2__3 indirect=false in_cycle=false param_regs=A,X,,,,,,,
; def f3 params=__zpabi_f3__p0_0,__zpabi_f3__p0_1,__zpabi_f3__p1_0,__zpabi_f3__p1_1 locals=__local_f3__0,__local_f3__1,__local_f3__2,__local_f3__3,__local_f3__4,__local_f3__p0_1,__local_f3__p0_0,__local_f3__5 indirect=false in_cycle=false param_regs=A,X,, carry=true
; def main params= locals=__local_main__0,__local_main__1,__local_main__2,__local_main__3,__local_main__4,__local_main__5,__local_main__6,__local_main__7,__local_main__mbox,__local_main__9,__local_main__10,__local_main__11 indirect=false in_cycle=false
; def rec params= locals=__local_rec__0,__local_rec__1,__local_rec__2,__local_rec__3,__local_rec__4 indirect=false in_cycle=true
; call f0 -> f2
; call f0 -> f3
; call f0 -> rec
; call f1 -> f3
; call f1 -> rec
; call f2 -> f3
; call main -> f0
; call rec -> rec saved
; item rec section=.text local=false zero=false
; item f3 section=.text local=false zero=false
; item f2 section=.text local=false zero=false
; item f1 section=.text local=false zero=false
; item f0 section=.text local=false zero=false
; item main section=.text local=false zero=false
; item g0 section=.data local=false zero=false
; item __zp_init section=.text local=false zero=false
; zp g1 size=2 at=auto def=true init=$7C,$00 weight=11
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

f3:
   SUBROUTINE

   STA   __local_f3__p0_0
   STX   __local_f3__p0_1
   LDA   __zpabi_f3__p1_0
   CLC
   SBC   __local_f3__p0_0
   LDA   __zpabi_f3__p1_1
   SBC   __local_f3__p0_1
   LDA   #$00
   ROL
   STA   __local_f3__3
   LDA   g0+3
   BNE   .thread@0
   LDA   g0+2
   BNE   .thread@0
   LDA   g0+1
   CMP   g1+1
   BNE   .thread@0
   LDA   g0
   CMP   g1
   BEQ   .cmp_true@1
.thread@0:
   LDA   #$00
   BEQ   .cmp_end@2
.cmp_true@1:
   LDA   #$01
.cmp_end@2:
   AND   #$07
   TAX
   LDA   #$00
   STA   __local_f3__4
   CPX   #$00
   BEQ   .shift_done@5
.shift_loop@4:
   LDA   __local_f3__4
   ASL
   ROR   __local_f3__4
   ROR   __local_f3__3
   DEX
   BNE   .shift_loop@4
.shift_done@5:
   LDA   __local_f3__3
   BMI   .sx_neg@6
   LDA   #$00
   BEQ   .sx_done@7
.sx_neg@6:
   LDA   #$FF
.sx_done@7:
   STA   __local_f3__2
   LDA   __local_f3__3
   AND   #$07
   TAX
   LDA   #$E8
   STA   __local_f3__0
   LDA   #$03
   STA   __local_f3__1
   CPX   #$00
   BEQ   .shift_done@9
.shift_loop@8:
   LDA   __local_f3__1
   ASL
   ROR   __local_f3__1
   ROR   __local_f3__0
   DEX
   BNE   .shift_loop@8
.shift_done@9:
   LDA   __local_f3__3
   SEC
   SBC   __local_f3__0
   LDA   __local_f3__2
   SBC   __local_f3__1
   BVC   .cmp_novf@10
   EOR   #$80
.cmp_novf@10:
   ASL
   LDA   #$00
   ROL
   STA   __local_f3__2
   LDA   g1
   ORA   #$01
   STA   HARGS+2
   LDA   g1+1
   STA   HARGS+3
   STA   HARGS+1
   LDA   g1
   STA   HARGS
   JSR   udivmod16
   LDA   HARGS+6
   ORA   HARGS+7
   BNE   .or_true@1
   LDA   __local_f3__p0_0
   ORA   #$01
   STA   HARGS+2
   LDA   __local_f3__p0_1
   STA   HARGS+3
   LDA   __local_f3__2
   STA   HARGS
   LDA   #$00
   STA   HARGS+1
   JSR   sdivmod16
   LDA   HARGS+6
   ORA   HARGS+7
   BNE   .or_true@1
   CLC
   RTS
.or_true@1:
   SEC
   RTS

f2:
   SUBROUTINE

   LDA   __zpabi_f2__p2
   AND   #$07
   TAX
   LDA   g1
   STA   __local_f2__0
   LDA   g1+1
   STA   __local_f2__1
   CPX   #$00
   BEQ   .shift_done@12
.shift_loop@11:
   LSR   __local_f2__1
   ROR   __local_f2__0
   DEX
   BNE   .shift_loop@11
.shift_done@12:
   SEC
   LDA   #$00
   SBC   __local_f2__0
   STA   __local_f2__3
   LDA   #$00
   SBC   __local_f2__1
   STA   __local_f2__2
   LDY   #$01
   LDA   (__zpabi_f2__pp_0),Y
   STA   __local_f2__1
   DEY
   LDA   (__zpabi_f2__pp_0),Y
   CLC
   ADC   __local_f2__3
   BCC   .carry_inc@2
   INC   __local_f2__1
   CLC
   BCC   .carry_inc@2
   INC   __local_f2__1
.carry_inc@2:
   LDY   #$00
   STA   (__zpabi_f2__pp_0),Y
   LDA   __local_f2__1
   INY
   STA   (__zpabi_f2__pp_0),Y
   LDX   g1+1
   LDA   __local_f2__3
   SEC
   SBC   #$03
   STA   __zpabi_f3__p1_0
   LDA   __local_f2__2
   SBC   #$00
   STA   __zpabi_f3__p1_1
   LDA   g1
   JSR   f3
   LDY   #$01
   LDA   (__zpabi_f2__pp_0),Y
   TAX
   DEY
   LDA   (__zpabi_f2__pp_0),Y
   RTS

f1:
   SUBROUTINE

   STA   __local_f1__l1
   LDA   __zpabi_f1__p0_0
   STA   HARGS
   LDA   __zpabi_f1__p0_1
   STA   HARGS+1
   LDA   __zpabi_f1__p0_2
   STA   HARGS+2
   LDA   __zpabi_f1__p0_3
   STA   HARGS+3
   LDA   #$07
   STA   HARGS+4
   LDA   #$00
   STA   HARGS+5
   LDY   #$00
   STY   HARGS+6
   STA   HARGS+7
   JSR   sdivmod32
   LDA   #$00
   SEC
   SBC   __local_f1__l1
   ORA   HARGS+8
   STA   __local_f1__1
   LDA   __zpabi_f1__p2_0
   ORA   __zpabi_f1__p2_1
   ORA   __zpabi_f1__p2_2
   ORA   __zpabi_f1__p2_3
   BNE   .or_true@5
   LDA   g1
   ORA   g1+1
   BNE   .or_true@5
   LDA   #$00
   BEQ   .or_end@6
.or_true@5:
   LDA   #$01
.or_end@6:
   STA   __zpabi_f3__p1_0
   LDA   #$00
   STA   __zpabi_f3__p1_1
   LDX   #$00
   LDA   #$01
   JSR   f3
   BCC   .if_else@8
   LDA   __local_f1__1
   CLC
   ADC   #$03
   STA   __local_f1__l1
   JMP   .if_end@7
.if_else@8:
   LDA   __local_f1__1
   EOR   #$05
   STA   __local_f1__l1
.if_end@7:
   LDA   g0
   CLC
   ADC   __local_f1__l1
   STA   g0
   LDA   g0+1
   ADC   #$00
   STA   g0+1
   LDA   g0+2
   ADC   #$00
   STA   g0+2
   LDA   g0+3
   ADC   #$00
   STA   g0+3
   LDA   g0
   STA   __local_f1__0
   LDA   g0+1
   STA   __local_f1__l1
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
   LDA   __local_f1__l1
   INY
   STA   (SSP),Y
   JSR   rec
   LDA   __zpabi_f1__p2_0
   SEC
   SBC   __zpabi_f1__p2_0
   LDA   __zpabi_f1__p2_1
   SBC   __zpabi_f1__p2_1
   LDA   __zpabi_f1__p2_2
   SBC   __zpabi_f1__p2_2
   LDA   __zpabi_f1__p2_3
   SBC   __zpabi_f1__p2_3
   BVC   .cmp_novf@15
   EOR   #$80
.cmp_novf@15:
   ASL
   LDA   #$00
   ROL
   AND   #$07
   TAX
   LDA   #$64
   STA   __local_f1__l1
   LDA   #$00
   STA   __local_f1__0
   CPX   #$00
   BEQ   .shift_done@17
.shift_loop@16:
   ASL   __local_f1__l1
   ROL   __local_f1__0
   DEX
   BNE   .shift_loop@16
.shift_done@17:
   LDA   __local_f1__0
   ORA   __local_f1__l1
   BEQ   .cmp_end@18
   SEC
   RTS
.cmp_end@18:
   CMP   #$01
   RTS

f0:
   SUBROUTINE

   STA   __local_f0__p0
   LDA   __zpabi_f0__p1_0
   ORA   #$01
   STA   HARGS+4
   LDA   __zpabi_f0__p1_1
   STA   HARGS+5
   LDA   __zpabi_f0__p1_2
   STA   HARGS+6
   LDA   __zpabi_f0__p1_3
   STA   HARGS+7
   LDA   __zpabi_f0__p2
   STA   HARGS
   LDA   #$00
   STA   HARGS+1
   LDY   #$00
   STY   HARGS+2
   STA   HARGS+3
   JSR   udivmod32
   LDA   __local_f0__p0
   BNE   .or_true@9
   LDA   #$00
   STA   __local_f0__1
   JMP   .or_end@10
.or_true@9:
   LDA   #$01
   STA   __local_f0__1
.or_end@10:
   LDA   HARGS+8
   AND   __local_f0__1
   STA   __local_f0__5
   LDA   #$00
   STA   __local_f0__4
   LDA   __zpabi_f0__p1_0
   ASL
   ASL
   ASL
   SEC
   SBC   __zpabi_f0__p1_0
   SEC
   SBC   g1
   STA   __local_f0__l2
   LDY   #$00
   LDA   (__zpabi_f0__pp_0),Y
   STA   __local_f0__2
   INY
   LDA   (__zpabi_f0__pp_0),Y
   STA   __local_f0__1
   LDA   __local_f0__p0
   AND   #$07
   TAX
   LDA   #$FF
   STA   __local_f0__0
   CPX   #$00
   BEQ   .shift_done@20
.shift_loop@19:
   LSR   __local_f0__0
   DEX
   BNE   .shift_loop@19
.shift_done@20:
   LDA   __local_f0__2
   CLC
   ADC   __local_f0__0
   BCC   .carry_inc@3
   INC   __local_f0__1
.carry_inc@3:
   LDY   #$00
   STA   (__zpabi_f0__pp_0),Y
   LDA   __local_f0__1
   INY
   STA   (__zpabi_f0__pp_0),Y
   LDX   g0+1
   LDA   __local_f0__5
   STA   __zpabi_f3__p1_0
   LDA   __local_f0__4
   STA   __zpabi_f3__p1_1
   LDA   g0
   JSR   f3
   BCC   .if_else@12
   LDA   __local_f0__l2
   CLC
   ADC   #$03
   STA   __local_f0__l2
   JMP   .if_end@11
.if_else@12:
   LDA   __local_f0__l2
   EOR   #$05
   STA   __local_f0__l2
.if_end@11:
   LDA   __local_f0__5
   STA   __local_f0__box2
   LDA   __local_f0__4
   STA   __local_f0__box2+1
   LDA   g1
   ORA   g1+1
   BEQ   .and_false@13
   LDA   #$01
   STA   __local_f0__2
   LDX   #$00
   BEQ   .and_end@14
.and_false@13:
   LDA   #$00
   STA   __local_f0__2
   LDX   #$00
.and_end@14:
   LDA   __zpabi_f0__p2
   STA   __zpabi_f2__p1_0
   LDA   #$00
   STA   __zpabi_f2__p1_1
   STA   __zpabi_f2__p1_2
   STA   __zpabi_f2__p1_3
   LDA   __local_f0__l2
   STA   __zpabi_f2__p2
   LDA   #<__local_f0__box2
   STA   __zpabi_f2__pp_0
   LDA   #>__local_f0__box2
   STA   __zpabi_f2__pp_1
   LDA   __local_f0__2
   JSR   f2
   STA   __local_f0__0
   LDA   __zpabi_f0__p1_0
   EOR   __local_f0__0
   STA   __local_f0__1
   CLC
   ADC   __local_f0__box2
   STA   __local_f0__0
   CLC
   ADC   g1
   STA   g1
   LDA   g1+1
   ADC   #$00
   STA   g1+1
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
   LDA   __zpabi_f0__p1_0
   INY
   STA   (SSP),Y
   LDA   __zpabi_f0__p1_1
   INY
   STA   (SSP),Y
   JSR   rec
   LDA   __zpabi_f0__p1_0
   STA   HARGS+8
   LDA   #$00
   STA   HARGS+9
   LDY   #$00
   STY   HARGS+10
   STA   HARGS+11
   RTS

main:
   SUBROUTINE

   LDA   #$05
   STA   __local_main__mbox
   LDA   #$00
   STA   __local_main__mbox+1
   LDA   #$95
   STA   __zpabi_f0__p1_0
   LDA   #$00
   STA   __zpabi_f0__p1_1
   STA   __zpabi_f0__p1_2
   STA   __zpabi_f0__p1_3
   LDA   #$AD
   STA   __zpabi_f0__p2
   LDA   #<__local_main__mbox
   STA   __zpabi_f0__pp_0
   LDA   #>__local_main__mbox
   STA   __zpabi_f0__pp_1
   LDA   #$F4
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
   EOR   g0+2
   STA   HARGS+2
   LDA   HARGS+11
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
   DC.L  $0000004C

__zp_init:
   SUBROUTINE

   LDA   #$00
   STA   g1+1
   LDA   #$7C
   STA   g1
   RTS
