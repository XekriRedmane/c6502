SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
g0	EQU	$26
__zpabi_f0__p2_0	EQU	$80
__zpabi_f2__p1	EQU	$80
__zpabi_f0__p2_1	EQU	$81
__zpabi_f2__p2	EQU	$81
__zpabi_f0__p2_2	EQU	$82
__zpabi_f2__pp_0	EQU	$82
__zpabi_f0__p2_3	EQU	$83
__zpabi_f2__pp_1	EQU	$83
__local_f2__0	EQU	$84
__zpabi_f0__p3_0	EQU	$84
__local_f2__1	EQU	$85
__zpabi_f0__p3_1	EQU	$85
__local_f2__2	EQU	$86
__zpabi_f1__p2_0	EQU	$86
__local_f2__3	EQU	$87
__zpabi_f1__p2_1	EQU	$87
__local_f2__4	EQU	$88
__zpabi_f1__p2_2	EQU	$88
__local_f2__5	EQU	$89
__zpabi_f1__p2_3	EQU	$89
__local_f2__6	EQU	$8A
__zpabi_f0__p1_0	EQU	$8A
__zpabi_f1__p0_0	EQU	$8A
__zpabi_f0__p1_1	EQU	$8B
__zpabi_f1__p0_1	EQU	$8B
__zpabi_f0__p1_2	EQU	$8C
__zpabi_f1__p0_2	EQU	$8C
__zpabi_f0__p1_3	EQU	$8D
__zpabi_f1__p0_3	EQU	$8D
__local_main__0	EQU	$8E
__local_main__2	EQU	$90
__local_main__3	EQU	$91
__local_f0__0	EQU	$9B
__local_f0__it_1	EQU	$9C
__local_f0__it_0	EQU	$9D
__local_f0__l1_1	EQU	$9E
__local_f0__l1_0	EQU	$9F
__local_f0__1	EQU	$A0
__local_f0__2	EQU	$A1
__local_f0__3	EQU	$A2
__local_f0__4	EQU	$A3
__local_f0__5	EQU	$A4
__local_rec__0	EQU	$A7
__local_rec__1	EQU	$A8
__local_rec__2	EQU	$A9
__local_rec__3	EQU	$AA
__local_rec__4	EQU	$AB

; @zp-link-meta-begin
; def f0 params=__zpabi_f0__p0,__zpabi_f0__p1_0,__zpabi_f0__p1_1,__zpabi_f0__p1_2,__zpabi_f0__p1_3,__zpabi_f0__p2_0,__zpabi_f0__p2_1,__zpabi_f0__p2_2,__zpabi_f0__p2_3,__zpabi_f0__p3_0,__zpabi_f0__p3_1 locals=__local_f0__p0,__local_f0__0,__local_f0__it_1,__local_f0__it_0,__local_f0__l1_1,__local_f0__l1_0,__local_f0__1,__local_f0__2,__local_f0__3,__local_f0__4,__local_f0__5,__local_f0__6,__local_f0__7 indirect=false in_cycle=false param_regs=A,,,,,,,,,,
; def f1 params=__zpabi_f1__p0_0,__zpabi_f1__p0_1,__zpabi_f1__p0_2,__zpabi_f1__p0_3,__zpabi_f1__p1,__zpabi_f1__p2_0,__zpabi_f1__p2_1,__zpabi_f1__p2_2,__zpabi_f1__p2_3 locals=__local_f1__0 indirect=false in_cycle=false param_regs=,,,,A,,,,
; def f2 params=__zpabi_f2__p0_0,__zpabi_f2__p0_1,__zpabi_f2__p1,__zpabi_f2__p2,__zpabi_f2__pp_0,__zpabi_f2__pp_1 locals=__local_f2__0,__local_f2__1,__local_f2__2,__local_f2__3,__local_f2__4,__local_f2__5,__local_f2__6 indirect=false in_cycle=false param_regs=A,X,,,,
; def main params= locals=__local_main__0,__local_main__1,__local_main__2,__local_main__3,__local_main__4,__local_main__5,__local_main__6,__local_main__7,__local_main__8,__local_main__9,__local_main__10,__local_main__11 indirect=false in_cycle=false
; def rec params= locals=__local_rec__0,__local_rec__1,__local_rec__2,__local_rec__3,__local_rec__4 indirect=false in_cycle=true
; call f0 -> f1
; call f0 -> rec
; call main -> f0
; call rec -> rec saved
; share member=f0:__zpabi_f0__p1_0,__zpabi_f0__p1_1,__zpabi_f0__p1_2,__zpabi_f0__p1_3 member=f1:__zpabi_f1__p0_0,__zpabi_f1__p0_1,__zpabi_f1__p0_2,__zpabi_f1__p0_3 live=f0:f1 forward=f0>f1
; item rec section=.text local=false zero=false
; item f2 section=.text local=false zero=false
; item f1 section=.text local=false zero=false
; item f0 section=.text local=false zero=false
; item main section=.text local=false zero=false
; item g1 section=.data local=false zero=false
; item __zp_init section=.text local=false zero=false
; zp g0 size=4 at=auto def=true init=$75,$00,$00,$00 weight=15
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

f2:
   SUBROUTINE

   LDA   g0
   SEC
   SBC   g0
   LDA   g0+1
   SBC   g0+1
   LDA   g0+2
   SBC   g0+2
   LDA   g0+3
   SBC   g0+3
   LDA   #$00
   ROL
   AND   #$07
   TAX
   LDA   g1
   STA   __local_f2__5
   LDA   g1+1
   STA   __local_f2__6
   CPX   #$00
   BEQ   .shift_done@2
.shift_loop@1:
   ASL   __local_f2__5
   ROL   __local_f2__6
   DEX
   BNE   .shift_loop@1
.shift_done@2:
   LDA   __zpabi_f2__p1
   BMI   .sx_neg@3
   LDA   #$00
   BEQ   .sx_done@4
.sx_neg@3:
   LDA   #$FF
.sx_done@4:
   STA   HARGS+3
   LDA   #$00
   STA   HARGS
   STA   HARGS+1
   LDA   __zpabi_f2__p1
   STA   HARGS+2
   JSR   mul16
   LDA   HARGS+4
   STA   __local_f2__4
   LDA   HARGS+5
   STA   __local_f2__3
   LDY   #$00
   LDA   (__zpabi_f2__pp_0),Y
   STA   __local_f2__1
   INY
   LDA   (__zpabi_f2__pp_0),Y
   STA   __local_f2__2
   LDA   __zpabi_f2__p2
   BMI   .sx_neg@5
   LDA   #$00
   BEQ   .sx_done@6
.sx_neg@5:
   LDA   #$FF
.sx_done@6:
   STA   __local_f2__0
   LDA   __local_f2__1
   CLC
   ADC   __zpabi_f2__p2
   STA   __local_f2__1
   LDA   __local_f2__2
   ADC   __local_f2__0
   LDY   #$01
   STA   (__zpabi_f2__pp_0),Y
   LDA   __local_f2__1
   DEY
   STA   (__zpabi_f2__pp_0),Y
   LDA   (__zpabi_f2__pp_0),Y
   STA   __local_f2__0
   INY
   LDA   (__zpabi_f2__pp_0),Y
   STA   __local_f2__1
   LDA   __local_f2__4
   SEC
   SBC   __local_f2__0
   STA   __local_f2__0
   LDA   __local_f2__3
   SBC   __local_f2__1
   STA   __local_f2__1
   LDA   __local_f2__0
   EOR   __local_f2__5
   STA   __local_f2__0
   LDA   __local_f2__1
   EOR   __local_f2__6
   TAX
   LDA   __local_f2__0
   RTS

f1:
   SUBROUTINE

   LDA   g0
   RTS

f0:
   SUBROUTINE

   STA   __zpabi_f1__p2_0
   LDA   g0
   STA   __local_f0__5
   LDA   g0+1
   STA   __local_f0__4
   LDA   __zpabi_f1__p0_0
   EOR   #$FF
   STA   __local_f0__2
   LDA   __zpabi_f1__p0_1
   EOR   #$FF
   STA   __local_f0__1
   LDA   __zpabi_f1__p0_2
   EOR   #$FF
   STA   __local_f0__l1_0
   LDA   __zpabi_f1__p0_3
   EOR   #$FF
   STA   __local_f0__l1_1
   LDA   __local_f0__5
   ORA   #$01
   STA   HARGS+2
   LDA   __local_f0__4
   STA   HARGS+3
   LDA   __zpabi_f1__p2_0
   STA   HARGS
   LDA   #$00
   STA   HARGS+1
   JSR   sdivmod16
   LDA   HARGS+7
   BMI   .sx_neg@7
   LDA   #$00
   BEQ   .sx_done@8
.sx_neg@7:
   LDA   #$FF
.sx_done@8:
   STA   __local_f0__0
   STA   __zpabi_f1__p2_0
   LDA   __local_f0__l1_1
   CMP   __zpabi_f1__p2_0
   BNE   .thread@0
   LDA   __local_f0__l1_0
   CMP   __local_f0__0
   BNE   .thread@0
   LDA   __local_f0__1
   CMP   HARGS+7
   BNE   .thread@0
   LDA   __local_f0__2
   CMP   HARGS+6
   BEQ   .cmp_true@9
.thread@0:
   LDA   #$00
   BEQ   .cmp_end@10
.cmp_true@9:
   LDA   #$01
.cmp_end@10:
   STA   __local_f0__l1_0
   LDA   __zpabi_f1__p0_0
   AND   #$07
   TAX
   LDA   __local_f0__5
   STA   __local_f0__0
   LDA   __local_f0__4
   STA   __local_f0__it_1
   CPX   #$00
   BEQ   .shift_done@13
.shift_loop@12:
   LDA   __local_f0__it_1
   ASL
   ROR   __local_f0__it_1
   ROR   __local_f0__0
   DEX
   BNE   .shift_loop@12
.shift_done@13:
   LDA   __local_f0__l1_0
   EOR   #$01
   STA   __zpabi_f1__p2_0
   LDA   __local_f0__0
   SEC
   SBC   __zpabi_f1__p2_0
   STA   __local_f0__3
   LDA   __local_f0__4
   BMI   .sx_neg@14
   LDA   #$00
   BEQ   .sx_done@15
.sx_neg@14:
   LDA   #$FF
.sx_done@15:
   STA   __local_f0__2
   STA   __local_f0__1
   LDA   #$00
   STA   __local_f0__l1_1
   STA   __local_f0__it_0
   LDY   #$00
   STY   __local_f0__it_1
.loop@0_start:
   LDA   __local_f0__it_0
   SEC
   SBC   #$03
   LDA   __local_f0__it_1
   SBC   #$00
   BVC   .jcmp_novf@16
   EOR   #$80
.jcmp_novf@16:
   BPL   .loop@0_break
   LDA   g0+3
   CMP   __local_f0__1
   BNE   .thread@1
   LDA   g0+2
   CMP   __local_f0__2
   BNE   .thread@1
   LDA   g0+1
   CMP   __local_f0__4
   BNE   .thread@1
   LDA   g0
   CMP   __local_f0__5
   BEQ   .cmp_true@17
.thread@1:
   LDA   #$00
   BEQ   .cmp_end@18
.cmp_true@17:
   LDA   #$01
.cmp_end@18:
   STA   __zpabi_f1__p2_0
   LDA   #$00
   BPL   .sx_done@21
   LDA   #$FF
.sx_done@21:
   STA   __zpabi_f1__p2_2
   STA   __zpabi_f1__p2_3
   LDA   #$00
   STA   __zpabi_f1__p2_1
   LDA   __local_f0__3
   JSR   f1
   CLC
   ADC   __local_f0__it_0
   STA   __local_f0__0
   LDA   #$00
   ADC   __local_f0__it_1
   STA   __zpabi_f1__p2_0
   LDA   __local_f0__l1_0
   CLC
   ADC   __local_f0__0
   STA   __local_f0__l1_0
   LDA   __local_f0__l1_1
   ADC   __zpabi_f1__p2_0
   STA   __local_f0__l1_1
   INC   __local_f0__it_0
   BNE   .loop@0_start
   INC   __local_f0__it_1
   JMP   .loop@0_start
.loop@0_break:
   LDA   __local_f0__l1_1
   BMI   .sx_neg@22
   LDA   #$00
   BEQ   .sx_done@23
.sx_neg@22:
   LDA   #$FF
.sx_done@23:
   STA   __local_f0__0
   STA   __zpabi_f1__p2_0
   LDA   g0
   CLC
   ADC   __local_f0__l1_0
   STA   g0
   LDA   g0+1
   ADC   __local_f0__l1_1
   STA   g0+1
   LDA   g0+2
   ADC   __local_f0__0
   STA   g0+2
   LDA   g0+3
   ADC   __zpabi_f1__p2_0
   STA   g0+3
   LDA   __zpabi_f1__p0_0
   STA   __local_f0__0
   LDA   __zpabi_f1__p0_1
   STA   __zpabi_f1__p2_0
   LDA   __zpabi_f0__p2_0
   EOR   __local_f0__0
   STA   __local_f0__0
   LDA   __zpabi_f0__p2_1
   EOR   __zpabi_f1__p2_0
   STA   __zpabi_f1__p2_0
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
   LDA   __local_f0__0
   INY
   STA   (SSP),Y
   LDA   __zpabi_f1__p2_0
   INY
   STA   (SSP),Y
   JSR   rec
   LDA   __zpabi_f0__p3_0
   STA   __local_f0__0
   LDA   #$7F
   AND   __zpabi_f0__p3_1
   STA   __zpabi_f1__p2_0
   LDA   #$01
   CLC
   SBC   __local_f0__0
   LDA   #$00
   SBC   __zpabi_f1__p2_0
   LDA   #$00
   ROL
   STA   __zpabi_f1__p2_0
   LDA   #$00
   BPL   .sx_done@25
   LDA   #$FF
.sx_done@25:
   STA   HARGS+10
   STA   HARGS+11
   LDA   __zpabi_f1__p2_0
   STA   HARGS+8
   LDA   #$00
   STA   HARGS+9
   RTS

main:
   SUBROUTINE

   LDA   #$61
   STA   __zpabi_f0__p1_0
   LDA   #$42
   STA   __zpabi_f0__p2_0
   LDA   #$A6
   STA   __zpabi_f0__p3_0
   LDA   #$00
   STA   __zpabi_f0__p1_1
   STA   __zpabi_f0__p1_2
   STA   __zpabi_f0__p1_3
   STA   __zpabi_f0__p2_1
   STA   __zpabi_f0__p2_2
   STA   __zpabi_f0__p2_3
   STA   __zpabi_f0__p3_1
   LDA   #$3C
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

g1:
   DC.W  $0022

__zp_init:
   SUBROUTINE

   LDA   #$00
   STA   g0+1
   STA   g0+2
   STA   g0+3
   LDA   #$75
   STA   g0
   RTS
