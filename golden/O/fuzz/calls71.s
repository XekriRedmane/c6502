SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
g0	EQU	$26
g1	EQU	$27
g2	EQU	$29
__local_f1__0	EQU	$80
__zpabi_f0__p0	EQU	$80
__zpabi_f2__p0_0	EQU	$80
__zpabi_f3__p2_0	EQU	$80
__zpabi_f4__p0_0	EQU	$80
__local_f1__1	EQU	$81
__zpabi_f0__p1_0	EQU	$81
__zpabi_f2__p0_1	EQU	$81
__zpabi_f3__p2_1	EQU	$81
__zpabi_f4__p0_1	EQU	$81
__local_f1__2	EQU	$82
__zpabi_f0__p1_1	EQU	$82
__zpabi_f2__p2_0	EQU	$82
__zpabi_f4__p0_2	EQU	$82
__zpabi_f0__p1_2	EQU	$83
__zpabi_f2__p2_1	EQU	$83
__zpabi_f4__p0_3	EQU	$83
__local_f1__4	EQU	$84
__local_f4__0	EQU	$84
__zpabi_f0__p1_3	EQU	$84
__zpabi_f2__p2_2	EQU	$84
__local_f1__5	EQU	$85
__local_f4__1	EQU	$85
__zpabi_f0__p2_0	EQU	$85
__zpabi_f2__p2_3	EQU	$85
__zpabi_f0__p2_1	EQU	$86
__zpabi_f0__p2_2	EQU	$87
__zpabi_f0__p2_3	EQU	$88
__zpabi_f0__p2_4	EQU	$89
__zpabi_f0__p2_5	EQU	$8A
__zpabi_f0__p2_6	EQU	$8B
__zpabi_f0__p2_7	EQU	$8C
__local_main__0	EQU	$8D
__local_main__1	EQU	$8E
__local_main__2	EQU	$8F
__local_main__3	EQU	$90
__local_main__4	EQU	$91
__local_main__5	EQU	$92
__local_main__6	EQU	$93
__local_main__8	EQU	$95
__local_f0__0	EQU	$98
__local_f0__1	EQU	$99
__local_f0__2	EQU	$9A
__local_f0__3	EQU	$9B
__local_rec__0	EQU	$9C
__local_rec__1	EQU	$9D
__local_rec__2	EQU	$9E
__local_rec__3	EQU	$9F
__local_rec__4	EQU	$A0

; @zp-link-meta-begin
; def f0 params=__zpabi_f0__p0,__zpabi_f0__p1_0,__zpabi_f0__p1_1,__zpabi_f0__p1_2,__zpabi_f0__p1_3,__zpabi_f0__p2_0,__zpabi_f0__p2_1,__zpabi_f0__p2_2,__zpabi_f0__p2_3,__zpabi_f0__p2_4,__zpabi_f0__p2_5,__zpabi_f0__p2_6,__zpabi_f0__p2_7,__zpabi_f0__p3_0,__zpabi_f0__p3_1 locals=__local_f0__0,__local_f0__1,__local_f0__2,__local_f0__3 indirect=false in_cycle=false param_regs=,,,,,,,,,,,,,A,X carry=true
; def f1 params= locals=__local_f1__0,__local_f1__1,__local_f1__2,__local_f1__3,__local_f1__4,__local_f1__5 indirect=false in_cycle=false
; def f2 params=__zpabi_f2__p0_0,__zpabi_f2__p0_1,__zpabi_f2__p1_0,__zpabi_f2__p1_1,__zpabi_f2__p2_0,__zpabi_f2__p2_1,__zpabi_f2__p2_2,__zpabi_f2__p2_3 locals= indirect=false in_cycle=false param_regs=,,A,X,,,, carry=true
; def f3 params=__zpabi_f3__p0,__zpabi_f3__p1,__zpabi_f3__p2_0,__zpabi_f3__p2_1 locals= indirect=false in_cycle=false param_regs=A,X,,
; def f4 params=__zpabi_f4__p0_0,__zpabi_f4__p0_1,__zpabi_f4__p0_2,__zpabi_f4__p0_3 locals=__local_f4__0,__local_f4__1 indirect=false in_cycle=false
; def main params= locals=__local_main__0,__local_main__1,__local_main__2,__local_main__3,__local_main__4,__local_main__5,__local_main__6,__local_main__7,__local_main__8,__local_main__9,__local_main__10 indirect=false in_cycle=false
; def rec params= locals=__local_rec__0,__local_rec__1,__local_rec__2,__local_rec__3,__local_rec__4 indirect=false in_cycle=true
; call f0 -> rec
; call f1 -> rec
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
; zp g0 size=1 at=auto def=true init=$43 weight=7
; zp g1 size=2 at=auto def=true init=$C1,$00 weight=8
; zp g2 size=2 at=auto def=true init=$43,$00 weight=6
; runtime hargs_size=32 hargs_need=21
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

   LDA   __zpabi_f4__p0_0
   EOR   g2
   STA   __local_f4__1
   LDA   __zpabi_f4__p0_1
   EOR   g2+1
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
   LDA   g0
   BMI   .sx_neg@1
   LDA   #$00
   BEQ   .sx_done@2
.sx_neg@1:
   LDA   #$FF
.sx_done@2:
   TAX
   LDA   g0
   RTS

f3:
   SUBROUTINE

   LDA   __zpabi_f3__p2_0
   ORA   #$01
   STA   HARGS+2
   LDA   __zpabi_f3__p2_1
   STA   HARGS+3
   LDA   #$FF
   STA   HARGS
   STA   HARGS+1
   JSR   udivmod16
   LDA   HARGS+6
   STA   HARGS+16
   LDA   HARGS+7
   STA   HARGS+17
   LDA   #$00
   STA   HARGS+18
   LDY   #$00
   STY   HARGS+19
   STA   HARGS+20
   RTS

f2:
   SUBROUTINE

   SEC
   RTS

f1:
   SUBROUTINE

   LDA   g0
   STA   __local_f1__2
   BMI   .sx_neg@3
   LDA   #$00
   BEQ   .sx_done@4
.sx_neg@3:
   LDA   #$FF
.sx_done@4:
   STA   __local_f1__1
   LDA   g1+1
   CMP   #$7F
   BNE   .thread@0
   LDA   g1
   CMP   #$FF
   BEQ   .cmp_true@5
.thread@0:
   LDA   #$00
   BEQ   .cmp_end@6
.cmp_true@5:
   LDA   #$01
.cmp_end@6:
   ORA   g0
   STA   __local_f1__5
   LDA   __local_f1__1
   STA   __local_f1__4
   BMI   .sx_neg@8
   LDA   #$00
   BEQ   .sx_done@9
.sx_neg@8:
   LDA   #$FF
.sx_done@9:
   STA   __local_f1__1
   STA   __local_f1__0
   LDA   __local_f1__5
   ORA   __local_f1__4
   ORA   __local_f1__1
   ORA   __local_f1__0
   BEQ   .lnot_true@10
   LDA   #$00
   BEQ   .lnot_end@11
.lnot_true@10:
   LDA   #$01
.lnot_end@11:
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
   LDA   __local_f1__0
   INY
   STA   (SSP),Y
   LDA   #$00
   INY
   STA   (SSP),Y
   JSR   rec
   STA   __local_f1__1
   STX   __local_f1__0
   LDA   g0
   BMI   .sx_neg@12
   LDA   #$00
   BEQ   .sx_done@13
.sx_neg@12:
   LDA   #$FF
.sx_done@13:
   STA   __local_f1__2
   LDA   __local_f1__5
   CLC
   ADC   __local_f1__1
   STA   __local_f1__1
   LDA   __local_f1__4
   ADC   __local_f1__0
   LSR
   ROR   __local_f1__1
   LDA   #$00
   ROR
   ORA   g0
   STA   __local_f1__0
   LDA   __local_f1__1
   ORA   __local_f1__2
   TAX
   LDA   __local_f1__0
   RTS

f0:
   SUBROUTINE

   LDA   __zpabi_f0__p1_0
   ORA   __zpabi_f0__p1_1
   ORA   __zpabi_f0__p1_2
   ORA   __zpabi_f0__p1_3
   BEQ   .lnot_true@14
   LDA   #$00
   BEQ   .lnot_end@15
.lnot_true@14:
   LDA   #$01
.lnot_end@15:
   STA   __local_f0__0
   LDA   g0
   STA   __local_f0__3
   BMI   .sx_neg@16
   LDA   #$00
   BEQ   .sx_done@17
.sx_neg@16:
   LDA   #$FF
.sx_done@17:
   STA   __local_f0__2
   STA   HARGS+1
   LDA   __local_f0__3
   STA   HARGS
   LDA   __local_f0__0
   STA   HARGS+2
   LDA   #$00
   STA   HARGS+3
   JSR   mul16
   LDA   HARGS+4
   STA   __local_f0__1
   LDA   HARGS+5
   STA   __local_f0__0
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
   LDA   __local_f0__1
   INY
   STA   (SSP),Y
   LDA   __local_f0__0
   INY
   STA   (SSP),Y
   JSR   rec
   LDA   __local_f0__3
   ASL
   LDA   __local_f0__2
   ROL
   STA   __local_f0__1
   LDA   #$00
   ADC   #$FF
   EOR   #$FF
   STA   __local_f0__0
   LDA   g1
   SEC
   SBC   __local_f0__1
   STA   __local_f0__1
   LDA   g1+1
   SBC   __local_f0__0
   ORA   __local_f0__1
   BEQ   .cmp_end@18
   SEC
   RTS
.cmp_end@18:
   CMP   #$01
   RTS

main:
   SUBROUTINE

   LDA   #$82
   STA   __zpabi_f0__p0
   LDA   #$03
   STA   __zpabi_f0__p1_0
   LDA   #$8C
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
   STA   __zpabi_f0__p2_6
   STA   __zpabi_f0__p2_7
   LDX   #$00
   LDA   #$8D
   JSR   f0
   LDA   #$00
   ROL
   STA   __local_main__8
   LDA   g0
   BMI   .sx_neg@19
   LDA   #$00
   BEQ   .sx_done@20
.sx_neg@19:
   LDA   #$FF
.sx_done@20:
   STA   __local_main__6
   STA   __local_main__5
   STA   __local_main__4
   LDA   g1+1
   BMI   .sx_neg@21
   LDA   #$00
   BEQ   .sx_done@22
.sx_neg@21:
   LDA   #$FF
.sx_done@22:
   STA   __local_main__1
   STA   __local_main__0
   LDA   g0
   EOR   g1
   STA   __local_main__3
   LDA   __local_main__6
   EOR   g1+1
   STA   __local_main__2
   LDA   __local_main__5
   EOR   __local_main__1
   STA   __local_main__5
   LDA   __local_main__4
   EOR   __local_main__0
   STA   __local_main__4
   LDA   __local_main__3
   EOR   g2
   STA   __local_main__3
   LDA   __local_main__2
   EOR   g2+1
   STA   __local_main__2
   LDA   __local_main__5
   STA   __local_main__1
   LDA   __local_main__4
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

__zp_init:
   SUBROUTINE

   LDA   #$00
   STA   g1+1
   STA   g2+1
   LDA   #$43
   STA   g0
   STA   g2
   LDA   #$C1
   STA   g1
   RTS
