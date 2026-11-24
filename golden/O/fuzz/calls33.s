SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
g0	EQU	$26
g1	EQU	$27
__zpabi_f0__p0_0	EQU	$80
__zpabi_f2__p1	EQU	$80
__zpabi_f3__p1_0	EQU	$80
__zpabi_f0__p0_1	EQU	$81
__zpabi_f2__p2_0	EQU	$81
__zpabi_f3__p1_1	EQU	$81
__zpabi_f0__p1_0	EQU	$82
__zpabi_f2__p2_1	EQU	$82
__zpabi_f3__p2_0	EQU	$82
__zpabi_f0__p1_1	EQU	$83
__zpabi_f2__p2_2	EQU	$83
__zpabi_f3__p2_1	EQU	$83
__zpabi_f0__p1_2	EQU	$84
__zpabi_f2__p2_3	EQU	$84
__zpabi_f3__p2_2	EQU	$84
__zpabi_f0__p1_3	EQU	$85
__zpabi_f2__p3_0	EQU	$85
__zpabi_f3__p2_3	EQU	$85
__local_f3__0	EQU	$86
__zpabi_f0__p2_0	EQU	$86
__zpabi_f2__p3_1	EQU	$86
__local_f3__1	EQU	$87
__zpabi_f0__p2_1	EQU	$87
__zpabi_f2__p3_2	EQU	$87
__local_f3__2	EQU	$88
__zpabi_f0__p2_2	EQU	$88
__zpabi_f2__p3_3	EQU	$88
__local_f1__0	EQU	$89
__local_f3__3	EQU	$89
__zpabi_f0__p2_3	EQU	$89
__local_f1__1	EQU	$8A
__local_f3__p0_1	EQU	$8A
__local_main__0	EQU	$8A
__local_f3__p0_0	EQU	$8B
__local_main__1	EQU	$8B
__local_main__2	EQU	$8C
__local_f1__4	EQU	$8D
__local_main__3	EQU	$8D
__local_f1__5	EQU	$8E
__local_f1__6	EQU	$8F
__local_f1__7	EQU	$90
__local_f1__8	EQU	$91
__local_f1__9	EQU	$92
__local_f2__p0_0	EQU	$93
__local_f2__p0_1	EQU	$94
__local_f2__0	EQU	$95
__local_rec__0	EQU	$97
__local_rec__1	EQU	$98
__local_rec__2	EQU	$99
__local_rec__3	EQU	$9A
__local_rec__4	EQU	$9B

; @zp-link-meta-begin
; def f0 params=__zpabi_f0__p0_0,__zpabi_f0__p0_1,__zpabi_f0__p1_0,__zpabi_f0__p1_1,__zpabi_f0__p1_2,__zpabi_f0__p1_3,__zpabi_f0__p2_0,__zpabi_f0__p2_1,__zpabi_f0__p2_2,__zpabi_f0__p2_3,__zpabi_f0__p3_0,__zpabi_f0__p3_1 locals= indirect=false in_cycle=false param_regs=,,,,,,,,,,A,X
; def f1 params= locals=__local_f1__0,__local_f1__1,__local_f1__2,__local_f1__3,__local_f1__4,__local_f1__5,__local_f1__6,__local_f1__7,__local_f1__8,__local_f1__9 indirect=false in_cycle=false
; def f2 params=__zpabi_f2__p0_0,__zpabi_f2__p0_1,__zpabi_f2__p1,__zpabi_f2__p2_0,__zpabi_f2__p2_1,__zpabi_f2__p2_2,__zpabi_f2__p2_3,__zpabi_f2__p3_0,__zpabi_f2__p3_1,__zpabi_f2__p3_2,__zpabi_f2__p3_3 locals=__local_f2__p0_0,__local_f2__p0_1,__local_f2__0,__local_f2__1 indirect=false in_cycle=false param_regs=A,X,,,,,,,,, carry=true
; def f3 params=__zpabi_f3__p0_0,__zpabi_f3__p0_1,__zpabi_f3__p1_0,__zpabi_f3__p1_1,__zpabi_f3__p2_0,__zpabi_f3__p2_1,__zpabi_f3__p2_2,__zpabi_f3__p2_3 locals=__local_f3__0,__local_f3__1,__local_f3__2,__local_f3__3,__local_f3__p0_1,__local_f3__p0_0,__local_f3__4 indirect=false in_cycle=false param_regs=A,X,,,,,, carry=true
; def main params= locals=__local_main__0,__local_main__1,__local_main__2,__local_main__3,__local_main__4,__local_main__5,__local_main__6,__local_main__7,__local_main__8,__local_main__9 indirect=false in_cycle=false
; def rec params= locals=__local_rec__0,__local_rec__1,__local_rec__2,__local_rec__3,__local_rec__4 indirect=false in_cycle=true
; call f1 -> f2
; call f1 -> rec
; call f2 -> rec
; call f3 -> rec
; call main -> f0
; call rec -> rec saved
; item rec section=.text local=false zero=false
; item f3 section=.text local=false zero=false
; item f2 section=.text local=false zero=false
; item f1 section=.text local=false zero=false
; item f1@run0 section=.text local=true zero=false
; item f0 section=.text local=false zero=false
; item main section=.text local=false zero=false
; item g2 section=.data local=false zero=false
; item __zp_init section=.text local=false zero=false
; zp g0 size=1 at=auto def=true init=$46 weight=9
; zp g1 size=2 at=auto def=true init=$A6,$00 weight=6
; runtime hargs_size=32 hargs_need=12
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
   ORA   __local_f3__p0_1
   BNE   .or_true@1
   LDA   g2
   ORA   g2+1
   ORA   g2+2
   ORA   g2+3
   BNE   .or_true@1
   LDA   #$00
   STA   __local_f3__2
   JMP   .or_end@2
.or_true@1:
   LDA   #$01
   STA   __local_f3__2
.or_end@2:
   LDA   __zpabi_f3__p2_0
   BMI   .sx_neg@1
   LDA   #$00
   BEQ   .sx_done@2
.sx_neg@1:
   LDA   #$FF
.sx_done@2:
   STA   __local_f3__0
   LDA   __local_f3__2
   SEC
   SBC   __zpabi_f3__p2_0
   STA   __local_f3__3
   LDA   #$00
   SBC   __local_f3__0
   STA   __local_f3__2
   LDA   g2
   STA   __local_f3__1
   LDA   g2+1
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
   LDA   __local_f3__1
   INY
   STA   (SSP),Y
   LDA   __local_f3__0
   INY
   STA   (SSP),Y
   JSR   rec
   CLC
   ADC   __local_f3__3
   STA   __local_f3__1
   TXA
   ADC   __local_f3__2
   STA   __local_f3__0
   LDA   __local_f3__1
   CLC
   ADC   __local_f3__p0_0
   STA   __local_f3__1
   LDA   __local_f3__0
   ADC   __local_f3__p0_1
   ORA   __local_f3__1
   BNE   .or_true@3
   LDA   __local_f3__p0_0
   CLC
   ADC   __zpabi_f3__p1_0
   STA   __local_f3__1
   LDA   __local_f3__p0_1
   ADC   __zpabi_f3__p1_1
   ORA   __local_f3__1
   BNE   .or_true@3
   CLC
   RTS
.or_true@3:
   SEC
   RTS

f2:
   SUBROUTINE

   STA   __local_f2__p0_0
   STX   __local_f2__p0_1
   LDA   #$FF
   CLC
   SBC   g1
   LDA   #$00
   SBC   g1+1
   LDA   #$00
   ROL
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
   LDA   __local_f2__0
   INY
   STA   (SSP),Y
   LDA   #$00
   INY
   STA   (SSP),Y
   JSR   rec
   LDA   __zpabi_f2__p1
   AND   #$07
   TAX
   BEQ   .shift_done@4
.shift_loop@3:
   ASL   __local_f2__p0_0
   ROL   __local_f2__p0_1
   DEX
   BNE   .shift_loop@3
.shift_done@4:
   LDA   __local_f2__p0_0
   STA   HARGS
   LDA   __local_f2__p0_1
   STA   HARGS+1
   LDA   __zpabi_f2__p3_0
   STA   HARGS+2
   LDA   #$00
   STA   HARGS+3
   JSR   mul16
   LDA   HARGS+5
   ORA   HARGS+4
   BEQ   .cmp_end@5
   SEC
   RTS
.cmp_end@5:
   CMP   #$01
   RTS

f1:
   SUBROUTINE

   JSR   f1@run0
   LDA   HARGS+3
   STA   __local_f1__1
   JSR   f1@run0
   LDA   HARGS+2
   CLC
   SBC   __local_f1__1
   LDA   #$00
   ROL
   STA   __local_f1__6
   LDA   #$00
   BPL   .sx_done@7
   LDA   #$FF
.sx_done@7:
   STA   __local_f1__9
   STA   __local_f1__8
   CLC
   LDY   #$00
   LDA   g0
   SBC   g1
   TYA
   SBC   g1+1
   TYA
   ROL
   STA   __local_f1__4
   LDA   g0
   STA   __zpabi_f2__p2_0
   LDA   g1
   AND   #$07
   TAX
   LDA   #$00
   STA   __zpabi_f2__p2_1
   CPX   #$00
   BEQ   .shift_done@9
.shift_loop@8:
   ASL   __zpabi_f2__p2_0
   ROL   __zpabi_f2__p2_1
   DEX
   BNE   .shift_loop@8
.shift_done@9:
   LDA   __zpabi_f2__p2_1
   BMI   .sx_neg@10
   TYA
   JMP   .sx_done@11
.sx_neg@10:
   LDA   #$FF
.sx_done@11:
   STA   __zpabi_f2__p2_2
   STA   __zpabi_f2__p2_3
   LDA   g1
   STA   __local_f1__1
   STA   __zpabi_f2__p3_0
   LDA   __local_f1__4
   STA   __zpabi_f2__p1
   LDA   g1+1
   STA   __zpabi_f2__p3_1
   LDA   #$00
   STA   __zpabi_f2__p3_2
   STA   __zpabi_f2__p3_3
   TAX
   LDA   g0
   JSR   f2
   LDA   #$00
   ROL
   STA   __local_f1__0
   LDA   __local_f1__6
   EOR   __local_f1__0
   STA   __local_f1__7
   LDA   #$00
   STA   __local_f1__6
   LDA   __local_f1__9
   STA   __local_f1__5
   LDA   __local_f1__8
   STA   __local_f1__4
   LDA   g0
   AND   g2
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
   STA   __zpabi_f2__p2_1
   STX   __zpabi_f2__p2_0
   TXA
   BMI   .sx_neg@12
   LDA   #$00
   BEQ   .sx_done@13
.sx_neg@12:
   LDA   #$FF
.sx_done@13:
   STA   __local_f1__1
   STA   __local_f1__0
   LDA   __local_f1__7
   CLC
   ADC   __zpabi_f2__p2_1
   STA   HARGS
   LDA   __local_f1__6
   ADC   __zpabi_f2__p2_0
   STA   HARGS+1
   LDA   __local_f1__5
   ADC   __local_f1__1
   STA   HARGS+2
   LDA   __local_f1__4
   ADC   __local_f1__0
   STA   HARGS+3
   LDA   g2
   AND   #$07
   STA   HARGS+4
   JSR   asl32
   LDA   g1
   SEC
   SBC   HARGS+5
   LDA   g1+1
   SBC   HARGS+6
   LDA   #$00
   SBC   HARGS+7
   LDA   #$00
   SBC   HARGS+8
   LDA   #$00
   ROL
   STA   __local_f1__0
   LDA   #$00
   BPL   .sx_done@15
   LDA   #$FF
.sx_done@15:
   STA   HARGS+10
   STA   HARGS+11
   LDA   __local_f1__0
   STA   HARGS+8
   LDA   #$00
   STA   HARGS+9
   RTS

f1@run0:
   SUBROUTINE

   LDA   g0
   ORA   #$01
   STA   HARGS+1
   LDA   g0
   STA   HARGS
   JMP   udivmod8

f0:
   SUBROUTINE

   LDA   #$FF
   STA   HARGS+8
   LDA   #$00
   STA   HARGS+9
   LDY   #$00
   STY   HARGS+10
   STA   HARGS+11
   RTS

main:
   SUBROUTINE

   LDA   #$91
   STA   __zpabi_f0__p0_0
   LDA   #$90
   STA   __zpabi_f0__p1_0
   LDA   #$C4
   STA   __zpabi_f0__p2_0
   LDA   #$00
   STA   __zpabi_f0__p0_1
   STA   __zpabi_f0__p1_1
   STA   __zpabi_f0__p1_2
   STA   __zpabi_f0__p1_3
   STA   __zpabi_f0__p2_1
   STA   __zpabi_f0__p2_2
   STA   __zpabi_f0__p2_3
   LDX   #$00
   LDA   #$9F
   JSR   f0
   LDA   g0
   EOR   g1
   STA   __local_main__3
   LDA   #$00
   EOR   g1+1
   STA   __local_main__2
   LDA   __local_main__3
   EOR   g2
   STA   __local_main__3
   LDA   __local_main__2
   EOR   g2+1
   STA   __local_main__2
   LDA   #$00
   EOR   g2+2
   STA   __local_main__1
   LDA   #$00
   EOR   g2+3
   STA   __local_main__0
   LDA   HARGS+8
   EOR   __local_main__3
   STA   __local_main__3
   LDA   HARGS+9
   EOR   __local_main__2
   STA   __local_main__2
   LDA   HARGS+10
   EOR   __local_main__1
   STA   HARGS+2
   LDA   HARGS+11
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
   DC.L  $00000087

__zp_init:
   SUBROUTINE

   LDA   #$00
   STA   g1+1
   LDA   #$46
   STA   g0
   LDA   #$A6
   STA   g1
   RTS
