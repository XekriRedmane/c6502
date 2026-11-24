SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
g0	EQU	$26
__local_f3__0	EQU	$80
__zpabi_f0__p0_0	EQU	$80
__zpabi_f1__p0_0	EQU	$80
__zpabi_f2__p0_0	EQU	$80
__local_f3__it_1	EQU	$81
__zpabi_f0__p0_1	EQU	$81
__zpabi_f1__p0_1	EQU	$81
__zpabi_f2__p0_1	EQU	$81
__local_f3__it_0	EQU	$82
__zpabi_f0__p0_2	EQU	$82
__zpabi_f1__p0_2	EQU	$82
__zpabi_f2__p0_2	EQU	$82
__local_f3__l2	EQU	$83
__zpabi_f0__p0_3	EQU	$83
__zpabi_f1__p0_3	EQU	$83
__zpabi_f2__p0_3	EQU	$83
__local_f1__0	EQU	$84
__local_f3__p0_0	EQU	$84
__zpabi_f0__p1_0	EQU	$84
__zpabi_f2__pp_0	EQU	$84
__local_f1__1	EQU	$85
__zpabi_f0__p1_1	EQU	$85
__zpabi_f2__pp_1	EQU	$85
__local_f1__2	EQU	$86
__local_f2__0	EQU	$86
__zpabi_f0__p1_2	EQU	$86
__local_f1__3	EQU	$87
__local_f2__1	EQU	$87
__zpabi_f0__p1_3	EQU	$87
__zpabi_f4__p1_0	EQU	$88
__zpabi_f4__p1_1	EQU	$89
__zpabi_f4__p1_2	EQU	$8A
__zpabi_f4__p1_3	EQU	$8B
__zpabi_f5__p0_0	EQU	$8C
__zpabi_f5__p0_1	EQU	$8D
__zpabi_f5__p0_2	EQU	$8E
__zpabi_f5__p0_3	EQU	$8F
__zpabi_f5__p1_0	EQU	$90
__zpabi_f5__p1_1	EQU	$91
__zpabi_f5__p3_0	EQU	$92
__zpabi_f5__p3_1	EQU	$93
__zpabi_f5__pp_0	EQU	$94
__zpabi_f5__pp_1	EQU	$95
__local_main__2	EQU	$98
__local_main__3	EQU	$99
__local_f0__l1_1	EQU	$9A
__local_f0__l1_0	EQU	$9B
__local_f0__0	EQU	$9C
__local_f0__1	EQU	$9D
__local_f0__2	EQU	$9E
__local_f0__box5	EQU	$9F
__local_f4__0	EQU	$A1
__local_f4__10	EQU	$AB
__local_f4__p0	EQU	$AC
__local_f4__box5	EQU	$AD
__local_f5__0	EQU	$B0
__local_f5__1	EQU	$B1
__local_f5__2	EQU	$B2
__local_f5__3	EQU	$B3
__local_rec__0	EQU	$B4
__local_rec__1	EQU	$B5
__local_rec__2	EQU	$B6
__local_rec__3	EQU	$B7
__local_rec__4	EQU	$B8

; @zp-link-meta-begin
; def f0 params=__zpabi_f0__p0_0,__zpabi_f0__p0_1,__zpabi_f0__p0_2,__zpabi_f0__p0_3,__zpabi_f0__p1_0,__zpabi_f0__p1_1,__zpabi_f0__p1_2,__zpabi_f0__p1_3,__zpabi_f0__p2 locals=__local_f0__l1_1,__local_f0__l1_0,__local_f0__0,__local_f0__1,__local_f0__2,__local_f0__box5,__local_f0__4 indirect=false in_cycle=false param_regs=,,,,,,,,A carry=true
; def f1 params=__zpabi_f1__p0_0,__zpabi_f1__p0_1,__zpabi_f1__p0_2,__zpabi_f1__p0_3 locals=__local_f1__0,__local_f1__1,__local_f1__2,__local_f1__3 indirect=false in_cycle=false carry=true
; def f2 params=__zpabi_f2__p0_0,__zpabi_f2__p0_1,__zpabi_f2__p0_2,__zpabi_f2__p0_3,__zpabi_f2__pp_0,__zpabi_f2__pp_1 locals=__local_f2__0,__local_f2__1,__local_f2__2,__local_f2__3 indirect=false in_cycle=false changes=__zpabi_f2__p0_0,__zpabi_f2__p0_1 carry=true
; def f3 params=__zpabi_f3__p0_0,__zpabi_f3__p0_1 locals=__local_f3__0,__local_f3__it_1,__local_f3__it_0,__local_f3__l2,__local_f3__p0_0,__local_f3__1,__local_f3__2 indirect=false in_cycle=false param_regs=A,X
; def f4 params=__zpabi_f4__p0,__zpabi_f4__p1_0,__zpabi_f4__p1_1,__zpabi_f4__p1_2,__zpabi_f4__p1_3 locals=__local_f4__0,__local_f4__1,__local_f4__2,__local_f4__3,__local_f4__4,__local_f4__5,__local_f4__6,__local_f4__7,__local_f4__8,__local_f4__9,__local_f4__10,__local_f4__p0,__local_f4__box5,__local_f4__12,__local_f4__13 indirect=false in_cycle=false param_regs=A,,,, carry=true
; def f5 params=__zpabi_f5__p0_0,__zpabi_f5__p0_1,__zpabi_f5__p0_2,__zpabi_f5__p0_3,__zpabi_f5__p1_0,__zpabi_f5__p1_1,__zpabi_f5__p2_0,__zpabi_f5__p2_1,__zpabi_f5__p3_0,__zpabi_f5__p3_1,__zpabi_f5__pp_0,__zpabi_f5__pp_1 locals=__local_f5__0,__local_f5__1,__local_f5__2,__local_f5__3 indirect=false in_cycle=false param_regs=,,,,,,A,X,,,, carry=true
; def main params= locals=__local_main__0,__local_main__1,__local_main__2,__local_main__3 indirect=false in_cycle=false
; def rec params= locals=__local_rec__0,__local_rec__1,__local_rec__2,__local_rec__3,__local_rec__4 indirect=false in_cycle=true
; call f0 -> f4
; call f0 -> f5
; call f0 -> rec
; call f1 -> rec
; call f2 -> rec
; call f3 -> f4
; call f3 -> rec
; call f4 -> f5
; call f5 -> rec
; call main -> f0
; call rec -> rec saved
; item rec section=.text local=false zero=false
; item f5 section=.text local=false zero=false
; item f5@tail0 section=.text local=true zero=false
; item f4 section=.text local=false zero=false
; item f3 section=.text local=false zero=false
; item f3@run0 section=.text local=true zero=false
; item f2 section=.text local=false zero=false
; item f1 section=.text local=false zero=false
; item f0 section=.text local=false zero=false
; item main section=.text local=false zero=false
; item __zp_init section=.text local=false zero=false
; zp g0 size=2 at=auto def=true init=$52,$00 weight=13
; runtime hargs_size=32 hargs_need=11
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

f5:
   SUBROUTINE

   LDA   __zpabi_f5__p3_0
   LSR
   LDA   #$00
   ROR
   STA   __local_f5__0
   LDA   #$00
   SEC
   SBC   __zpabi_f5__p3_0
   STA   __local_f5__1
   LDA   __local_f5__0
   SBC   __zpabi_f5__p3_1
   STA   __local_f5__0
   LDA   __local_f5__1
   ORA   #$01
   STA   HARGS+2
   LDA   __local_f5__0
   STA   HARGS+3
   LDA   #$64
   STA   HARGS
   LDA   #$00
   STA   HARGS+1
   JSR   udivmod16
   LDA   __zpabi_f5__pp_0
   STA   DPTR
   LDA   __zpabi_f5__pp_1
   STA   DPTR+1
   LDY   #$00
   LDA   (__zpabi_f5__pp_0),Y
   STA   __local_f5__3
   INY
   LDA   (__zpabi_f5__pp_0),Y
   STA   __local_f5__2
   LDA   HARGS+4
   ORA   HARGS+4
   STA   __local_f5__1
   LDA   HARGS+5
   ORA   HARGS+5
   STA   __local_f5__0
   LDA   __local_f5__3
   CLC
   ADC   __local_f5__1
   STA   __local_f5__1
   LDA   __local_f5__2
   ADC   __local_f5__0
   STA   (__zpabi_f5__pp_0),Y
   LDA   __local_f5__1
   DEY
   STA   (__zpabi_f5__pp_0),Y
   LDA   (__zpabi_f5__pp_0),Y
   STA   __local_f5__1
   INY
   LDA   (__zpabi_f5__pp_0),Y
   STA   __local_f5__0
   LDA   __zpabi_f5__p3_0
   EOR   __local_f5__1
   STA   __local_f5__1
   LDA   __zpabi_f5__p3_1
   EOR   __local_f5__0
   STA   __local_f5__0
   SEC
   LDA   SSP
   SBC   #$04
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   #$03
   STA   (SSP),Y
   LDA   #$00
   INY
   STA   (SSP),Y
   LDA   __local_f5__1
   INY
   STA   (SSP),Y
   LDA   __local_f5__0
   INY
   STA   (SSP),Y
   JSR   rec
   LDA   __zpabi_f5__p3_1
   ORA   __zpabi_f5__p3_0
   JMP   f5@tail0

f5@tail0:
   SUBROUTINE

   BEQ   .cmp_end@1
   SEC
   RTS
.cmp_end@1:
   CMP   #$01
   RTS

f4:
   SUBROUTINE

   STA   __local_f4__p0
   ORA   #$00
   BMI   .sx_neg@2
   LDA   #$00
   BEQ   .sx_done@3
.sx_neg@2:
   LDA   #$FF
.sx_done@3:
   STA   __local_f4__0
   LDA   __local_f4__p0
   BMI   .sx_neg@4
   LDA   #$00
   BEQ   .sx_done@5
.sx_neg@4:
   LDA   #$FF
.sx_done@5:
   STA   __local_f4__10
   STA   __local_f4__box5+1
   LDA   __local_f4__p0
   STA   __local_f4__box5
   ORA   #$01
   STA   HARGS+2
   LDA   __local_f4__10
   STA   HARGS+3
   LDA   #$FF
   STA   HARGS
   LDA   #$7F
   STA   HARGS+1
   JSR   sdivmod16
   LDA   HARGS+7
   BMI   .sx_neg@6
   LDA   #$00
   BEQ   .sx_done@7
.sx_neg@6:
   LDA   #$FF
.sx_done@7:
   STA   __zpabi_f5__p0_2
   LDA   __local_f4__p0
   SEC
   SBC   __zpabi_f4__p1_0
   STA   __zpabi_f5__p1_0
   LDA   __local_f4__0
   SBC   __zpabi_f4__p1_1
   STA   __zpabi_f5__p1_1
   LDA   __local_f4__p0
   AND   #$07
   TAX
   LDA   #$FF
   STA   __zpabi_f5__p3_0
   LDA   #$7F
   STA   __zpabi_f5__p3_1
   CPX   #$00
   BEQ   .shift_done@9
.shift_loop@8:
   LDA   __zpabi_f5__p3_1
   ASL
   ROR   __zpabi_f5__p3_1
   ROR   __zpabi_f5__p3_0
   DEX
   BNE   .shift_loop@8
.shift_done@9:
   LDA   HARGS+6
   STA   __zpabi_f5__p0_0
   LDA   HARGS+7
   STA   __zpabi_f5__p0_1
   LDA   __zpabi_f5__p0_2
   STA   __zpabi_f5__p0_3
   LDA   #<__local_f4__box5
   STA   __zpabi_f5__pp_0
   LDA   #>__local_f4__box5
   STA   __zpabi_f5__pp_1
   LDX   __local_f4__10
   LDA   __local_f4__p0
   JSR   f5
   LDA   __zpabi_f4__p1_3
   ORA   __zpabi_f4__p1_2
   ORA   __zpabi_f4__p1_1
   ORA   __zpabi_f4__p1_0
   JMP   f5@tail0

f3:
   SUBROUTINE

   STA   __local_f3__p0_0
   STX   __local_f3__it_0
   LDA   g0
   JSR   f3@run0
   STA   __local_f3__it_1
   BMI   .sx_neg@11
   LDA   #$00
   BEQ   .sx_done@12
.sx_neg@11:
   LDA   #$FF
.sx_done@12:
   CMP   __local_f3__it_0
   BNE   .thread@0
   LDA   __local_f3__it_1
   CMP   __local_f3__p0_0
   BEQ   .cmp_true@13
.thread@0:
   LDA   #$00
   BEQ   .cmp_end@14
.cmp_true@13:
   LDA   #$01
.cmp_end@14:
   STA   __local_f3__l2
   LDA   #$00
   STA   __local_f3__it_0
   LDY   #$00
   STY   __local_f3__it_1
.loop@0_start:
   LDA   __local_f3__it_0
   SEC
   SBC   #$03
   LDA   __local_f3__it_1
   SBC   #$00
   BVC   .jcmp_novf@16
   EOR   #$80
.jcmp_novf@16:
   BPL   .loop@0_break
   LDA   __local_f3__p0_0
   JSR   f3@run0
   LDX   __local_f3__l2
   STX   __zpabi_f4__p1_0
   LDX   #$00
   STX   __zpabi_f4__p1_1
   STX   __zpabi_f4__p1_2
   STX   __zpabi_f4__p1_3
   JSR   f4
   LDA   #$00
   ROL
   CLC
   ADC   __local_f3__it_0
   CLC
   ADC   __local_f3__l2
   STA   __local_f3__l2
   INC   __local_f3__it_0
   BNE   .loop@0_start
   INC   __local_f3__it_1
   JMP   .loop@0_start
.loop@0_break:
   LDA   __local_f3__l2
   ORA   #$E0
   STA   __local_f3__it_1
   LDA   #$1F
   STA   __local_f3__0
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
   LDA   __local_f3__it_1
   INY
   STA   (SSP),Y
   LDA   __local_f3__0
   INY
   STA   (SSP),Y
   JSR   rec
   LDA   g0
   CLC
   SBC   __local_f3__l2
   LDA   g0+1
   SBC   #$00
   LDA   #$00
   ROL
   AND   g0
   LDX   #$00
   RTS

f3@run0:
   SUBROUTINE

   AND   #$03
   EOR   #$02
   SEC
   SBC   #$02
   RTS

f2:
   SUBROUTINE

   LDA   __zpabi_f2__pp_0
   STA   DPTR
   LDA   __zpabi_f2__pp_1
   STA   DPTR+1
   LDY   #$00
   LDA   (__zpabi_f2__pp_0),Y
   CLC
   ADC   #$06
   STA   __local_f2__1
   INY
   LDA   (__zpabi_f2__pp_0),Y
   ADC   #$01
   STA   (__zpabi_f2__pp_0),Y
   LDA   __local_f2__1
   DEY
   STA   (__zpabi_f2__pp_0),Y
   LDA   __zpabi_f2__p0_0
   CLC
   ADC   #$64
   STA   __local_f2__1
   LDA   __zpabi_f2__p0_1
   ADC   #$00
   STA   __local_f2__0
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
   LDA   __local_f2__1
   INY
   STA   (SSP),Y
   LDA   __local_f2__0
   INY
   STA   (SSP),Y
   JSR   rec
   LDA   __zpabi_f2__p0_3
   ORA   __zpabi_f2__p0_2
   ORA   __zpabi_f2__p0_1
   ORA   __zpabi_f2__p0_0
   JMP   f5@tail0

f1:
   SUBROUTINE

   LDA   __zpabi_f1__p0_0
   BMI   .sx_neg@18
   LDA   #$00
   BEQ   .sx_done@19
.sx_neg@18:
   LDA   #$FF
.sx_done@19:
   STA   __local_f1__0
   LDA   __zpabi_f1__p0_0
   CLC
   ADC   __zpabi_f1__p0_0
   STA   __local_f1__3
   LDA   __zpabi_f1__p0_1
   ADC   __local_f1__0
   STA   __local_f1__2
   STA   HARGS+1
   LDA   __local_f1__3
   STA   HARGS
   LDA   g0
   STA   HARGS+2
   LDA   g0+1
   STA   HARGS+3
   JSR   mul16
   LDA   HARGS+4
   STA   __local_f1__1
   LDA   HARGS+5
   STA   __local_f1__0
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
   LDA   __local_f1__1
   INY
   STA   (SSP),Y
   LDA   __local_f1__0
   INY
   STA   (SSP),Y
   JSR   rec
   LDA   __local_f1__3
   STA   HARGS
   LDA   __local_f1__2
   STA   HARGS+1
   LDA   #$03
   STA   HARGS+2
   LDA   #$00
   STA   HARGS+3
   JSR   sdivmod16
   LDA   HARGS+4
   EOR   __local_f1__3
   STA   __local_f1__1
   LDA   HARGS+5
   EOR   __local_f1__2
   ORA   __local_f1__1
   JMP   f5@tail0

f0:
   SUBROUTINE

   CLC
   SBC   #$64
   LDA   #$00
   ROL
   JSR   f3@run0
   STA   __local_f0__2
   LDA   g0
   LSR
   LDA   #$00
   ROR
   JSR   f3@run0
   STA   __local_f0__1
   LDA   __local_f0__2
   BMI   .sx_neg@21
   LDA   #$00
   BEQ   .sx_done@22
.sx_neg@21:
   LDA   #$FF
.sx_done@22:
   STA   __local_f0__0
   STA   __local_f0__l1_0
   STA   __local_f0__l1_1
   LDA   __zpabi_f0__p1_0
   AND   __local_f0__2
   STA   __zpabi_f4__p1_0
   LDA   __zpabi_f0__p1_1
   AND   __local_f0__0
   STA   __zpabi_f4__p1_1
   LDA   __zpabi_f0__p1_2
   AND   __local_f0__l1_0
   STA   __zpabi_f4__p1_2
   LDA   __zpabi_f0__p1_3
   AND   __local_f0__l1_1
   STA   __zpabi_f4__p1_3
   LDA   __local_f0__1
   JSR   f4
   BCC   .if_else@4
   LDA   __zpabi_f0__p1_0
   CLC
   ADC   #$03
   STA   __local_f0__l1_0
   LDA   __zpabi_f0__p1_1
   ADC   #$00
   STA   __local_f0__l1_1
   JMP   .if_end@3
.if_else@4:
   LDA   __zpabi_f0__p1_0
   EOR   #$05
   STA   __local_f0__l1_0
   LDA   __zpabi_f0__p1_1
   STA   __local_f0__l1_1
.if_end@3:
   LDA   g0
   CLC
   ADC   __local_f0__l1_0
   STA   g0
   LDA   g0+1
   ADC   __local_f0__l1_1
   STA   g0+1
   LDA   __local_f0__2
   STA   __local_f0__box5
   BMI   .sx_neg@23
   LDA   #$00
   BEQ   .sx_done@24
.sx_neg@23:
   LDA   #$FF
.sx_done@24:
   STA   __local_f0__box5+1
   LDA   #$00
   SEC
   SBC   __zpabi_f0__p1_0
   STA   __zpabi_f5__p0_0
   LDA   __zpabi_f0__p1_0
   SBC   __zpabi_f0__p1_1
   STA   __zpabi_f5__p0_1
   LDA   __zpabi_f0__p1_1
   SBC   __zpabi_f0__p1_2
   STA   __zpabi_f5__p0_2
   LDA   __zpabi_f0__p1_2
   SBC   __zpabi_f0__p1_3
   STA   __zpabi_f5__p0_3
   LDA   __zpabi_f0__p0_0
   SEC
   SBC   __zpabi_f0__p0_0
   LDA   __zpabi_f0__p0_1
   SBC   __zpabi_f0__p0_1
   LDA   __zpabi_f0__p0_2
   SBC   __zpabi_f0__p0_2
   LDA   __zpabi_f0__p0_3
   SBC   __zpabi_f0__p0_3
   LDA   #$00
   ROL
   STA   __local_f0__0
   LDA   __zpabi_f0__p0_0
   ORA   __zpabi_f0__p0_0
   STA   __zpabi_f5__p3_0
   LDA   __zpabi_f0__p0_1
   ORA   __zpabi_f0__p0_1
   STA   __zpabi_f5__p3_1
   LDA   #$00
   STA   __zpabi_f5__p1_0
   STA   __zpabi_f5__p1_1
   LDA   #<__local_f0__box5
   STA   __zpabi_f5__pp_0
   LDA   #>__local_f0__box5
   STA   __zpabi_f5__pp_1
   LDX   #$00
   LDA   __local_f0__0
   JSR   f5
   LDA   #$00
   ROL
   STA   __local_f0__l1_1
   LDA   __local_f0__2
   EOR   __local_f0__l1_1
   JSR   f3@run0
   CLC
   ADC   __local_f0__box5
   JSR   f3@run0
   STA   __local_f0__l1_0
   BMI   .sx_neg@25
   LDA   #$00
   BEQ   .sx_done@26
.sx_neg@25:
   LDA   #$FF
.sx_done@26:
   STA   __local_f0__l1_1
   LDA   g0
   CLC
   ADC   __local_f0__l1_0
   STA   g0
   LDA   g0+1
   ADC   __local_f0__l1_1
   STA   g0+1
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
   LDA   #$FF
   INY
   STA   (SSP),Y
   LDA   #$00
   INY
   STA   (SSP),Y
   JSR   rec
   LDA   __zpabi_f0__p1_3
   ORA   __zpabi_f0__p1_2
   ORA   __zpabi_f0__p1_1
   ORA   __zpabi_f0__p1_0
   JMP   f5@tail0

main:
   SUBROUTINE

   LDA   #$9D
   STA   __zpabi_f0__p0_0
   LDA   #$80
   STA   __zpabi_f0__p1_0
   LDA   #$00
   STA   __zpabi_f0__p0_1
   STA   __zpabi_f0__p0_2
   STA   __zpabi_f0__p0_3
   STA   __zpabi_f0__p1_1
   STA   __zpabi_f0__p1_2
   STA   __zpabi_f0__p1_3
   LDA   #$D9
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
   LDA   #$52
   STA   g0
   RTS
