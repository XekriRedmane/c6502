SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
g0	EQU	$26
__local_f1__0	EQU	$80
__local_f2__0	EQU	$80
__zpabi_f0__p0_0	EQU	$80
__local_f1__1	EQU	$81
__local_f2__1	EQU	$81
__zpabi_f0__p0_1	EQU	$81
__local_f1__it_1	EQU	$82
__local_f2__2	EQU	$82
__zpabi_f0__p0_2	EQU	$82
__local_f1__it_0	EQU	$83
__local_f2__3	EQU	$83
__zpabi_f0__p0_3	EQU	$83
__local_f1__l0	EQU	$84
__local_f2__4	EQU	$84
__zpabi_f0__p1_0	EQU	$84
__local_f1__box4	EQU	$85
__local_f2__5	EQU	$85
__zpabi_f0__p1_1	EQU	$85
__local_f2__6	EQU	$86
__zpabi_f0__p1_2	EQU	$86
__local_f2__7	EQU	$87
__zpabi_f0__p1_3	EQU	$87
__local_f2__8	EQU	$88
__zpabi_f0__p2	EQU	$88
__local_f2__9	EQU	$89
__zpabi_f0__p3_0	EQU	$89
__local_f2__10	EQU	$8A
__zpabi_f0__p3_1	EQU	$8A
__local_f2__11	EQU	$8B
__zpabi_f0__p3_2	EQU	$8B
__local_f2__12	EQU	$8C
__zpabi_f0__p3_3	EQU	$8C
__local_f2__13	EQU	$8D
__zpabi_f0__pp_0	EQU	$8D
__local_f2__14	EQU	$8E
__zpabi_f0__pp_1	EQU	$8E
__local_f2__box4	EQU	$8F
__local_main__0	EQU	$8F
__local_f2__box3	EQU	$91
__local_main__2	EQU	$91
__local_main__3	EQU	$92
__local_main__mbox	EQU	$94
__local_f3__0	EQU	$96
__local_f3__1	EQU	$97
__local_f0__0	EQU	$98
__local_f3__2	EQU	$98
__local_f0__1	EQU	$99
__local_f3__3	EQU	$99
__local_f0__2	EQU	$9A
__local_f3__pp	EQU	$9A
__local_f0__3	EQU	$9B
__local_f3__4	EQU	$9B
__local_f0__it_1	EQU	$9E
__local_f0__it_0	EQU	$9F
__local_f0__l0_3	EQU	$A0
__local_f0__l0_2	EQU	$A1
__local_f0__l0_1	EQU	$A2
__local_f0__l0_0	EQU	$A3
__local_f0__box4	EQU	$A4
__local_f4__0	EQU	$A6
__local_f4__1	EQU	$A7
__local_f4__2	EQU	$A8
__local_f4__3	EQU	$A9
__local_f4__4	EQU	$AA
__local_f4__pp	EQU	$AE
__local_f4__8	EQU	$AF
__local_rec__0	EQU	$B0
__local_rec__1	EQU	$B1
__local_rec__2	EQU	$B2
__local_rec__3	EQU	$B3
__local_rec__4	EQU	$B4

; @zp-link-meta-begin
; def f0 params=__zpabi_f0__p0_0,__zpabi_f0__p0_1,__zpabi_f0__p0_2,__zpabi_f0__p0_3,__zpabi_f0__p1_0,__zpabi_f0__p1_1,__zpabi_f0__p1_2,__zpabi_f0__p1_3,__zpabi_f0__p2,__zpabi_f0__p3_0,__zpabi_f0__p3_1,__zpabi_f0__p3_2,__zpabi_f0__p3_3,__zpabi_f0__pp_0,__zpabi_f0__pp_1 locals=__local_f0__0,__local_f0__1,__local_f0__2,__local_f0__3,__local_f0__4,__local_f0__5,__local_f0__it_1,__local_f0__it_0,__local_f0__l0_3,__local_f0__l0_2,__local_f0__l0_1,__local_f0__l0_0,__local_f0__box4,__local_f0__7 indirect=false in_cycle=false carry=true
; def f1 params= locals=__local_f1__0,__local_f1__1,__local_f1__it_1,__local_f1__it_0,__local_f1__l0,__local_f1__box4,__local_f1__3,__local_f1__4,__local_f1__5 indirect=false in_cycle=false carry=true
; def f2 params= locals=__local_f2__0,__local_f2__1,__local_f2__2,__local_f2__3,__local_f2__4,__local_f2__5,__local_f2__6,__local_f2__7,__local_f2__8,__local_f2__9,__local_f2__10,__local_f2__11,__local_f2__12,__local_f2__13,__local_f2__14,__local_f2__box4,__local_f2__16,__local_f2__box3,__local_f2__18,__local_f2__19,__local_f2__20,__local_f2__21 indirect=false in_cycle=false
; def f3 params=__zpabi_f3__pp_0,__zpabi_f3__pp_1 locals=__local_f3__0,__local_f3__1,__local_f3__2,__local_f3__3,__local_f3__pp,__local_f3__4 indirect=false in_cycle=false param_regs=A,X
; def f4 params=__zpabi_f4__pp_0,__zpabi_f4__pp_1 locals=__local_f4__0,__local_f4__1,__local_f4__2,__local_f4__3,__local_f4__4,__local_f4__5,__local_f4__6,__local_f4__7,__local_f4__pp,__local_f4__8 indirect=false in_cycle=false param_regs=A,X
; def main params= locals=__local_main__0,__local_main__1,__local_main__2,__local_main__3,__local_main__4,__local_main__mbox,__local_main__6,__local_main__7,__local_main__8 indirect=false in_cycle=false
; def rec params= locals=__local_rec__0,__local_rec__1,__local_rec__2,__local_rec__3,__local_rec__4 indirect=false in_cycle=true
; call f0 -> f4
; call f1 -> f4
; call f2 -> f3
; call f2 -> f4
; call f4 -> rec
; call main -> f0
; call rec -> rec saved
; item rec section=.text local=false zero=false
; item f4 section=.text local=false zero=false
; item f4@tail0 section=.text local=true zero=false
; item f3 section=.text local=false zero=false
; item f3@run0 section=.text local=true zero=false
; item f2 section=.text local=false zero=false
; item f1 section=.text local=false zero=false
; item f1@tail1 section=.text local=true zero=false
; item f0 section=.text local=false zero=false
; item main section=.text local=false zero=false
; item __zp_init section=.text local=false zero=false
; zp g0 size=4 at=auto def=true init=$27,$00,$00,$00 weight=22
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

   STA   __local_f4__pp
   STX   __local_f4__8
   LDA   g0+3
   CMP   g0+3
   BNE   .thread@0
   LDA   g0+2
   CMP   g0+2
   BNE   .thread@0
   LDA   g0+1
   CMP   g0+1
   BNE   .thread@0
   LDA   g0
   CMP   g0
   BEQ   .cmp_true@1
.thread@0:
   LDA   #$00
   BEQ   .cmp_end@2
.cmp_true@1:
   LDA   #$01
.cmp_end@2:
   AND   #$03
   STA   __local_f4__4
   BMI   .sx_neg@4
   LDA   #$00
   BEQ   .sx_done@5
.sx_neg@4:
   LDA   #$FF
.sx_done@5:
   STA   __local_f4__3
   LDA   __local_f4__pp
   STA   DPTR
   LDY   #$00
   LDA   (__local_f4__pp),Y
   STA   __local_f4__2
   INY
   LDA   (__local_f4__pp),Y
   STA   __local_f4__1
   LDA   __local_f4__3
   CMP   __local_f4__3
   BNE   .cmp_true@6
   LDA   __local_f4__4
   CMP   __local_f4__4
   BNE   .cmp_true@6
   LDA   #$00
   BEQ   .cmp_end@7
.cmp_true@6:
   LDA   #$01
.cmp_end@7:
   CLC
   ADC   __local_f4__2
   BCC   .carry_inc@1
   INC   __local_f4__1
.carry_inc@1:
   LDY   #$00
   STA   (__local_f4__pp),Y
   LDA   __local_f4__1
   INY
   STA   (__local_f4__pp),Y
   DEY
   LDA   (__local_f4__pp),Y
   STA   __local_f4__1
   STX   DPTR+1
   SEC
   SBC   __local_f4__4
   INY
   LDA   (__local_f4__pp),Y
   SBC   __local_f4__3
   BVC   .cmp_novf@9
   EOR   #$80
.cmp_novf@9:
   ASL
   LDA   #$00
   ROL
   EOR   #$01
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
   LDA   __local_f4__0
   INY
   STA   (SSP),Y
   LDA   #$00
   INY
   STA   (SSP),Y
   JSR   rec
   LDA   #$E8
   STA   HARGS+8
   LDA   #$03
   JMP   f4@tail0

f4@tail0:
   SUBROUTINE

   STA   HARGS+9
   LDA   #$00
   STA   HARGS+10
   STA   HARGS+11
   RTS

f3:
   SUBROUTINE

   STA   __local_f3__pp
   STX   __local_f3__4
   LDY   #$00
   LDA   (__local_f3__pp),Y
   STA   __local_f3__3
   INY
   LDA   (__local_f3__pp),Y
   STA   __local_f3__2
   JSR   f3@run0
   BEQ   .and_false@1
   LDA   #$01
   STA   __local_f3__1
   JMP   .and_end@2
.and_false@1:
   LDA   #$00
   STA   __local_f3__1
.and_end@2:
   LDA   __local_f3__3
   CLC
   ADC   __local_f3__1
   STA   __local_f3__1
   LDA   __local_f3__2
   ADC   #$00
   LDY   #$01
   STA   (__local_f3__pp),Y
   LDA   __local_f3__1
   DEY
   STA   (__local_f3__pp),Y
   LDA   (__local_f3__pp),Y
   STA   __local_f3__1
   INY
   LDA   (__local_f3__pp),Y
   STA   __local_f3__0
   BMI   .sx_neg@10
   LDA   #$00
   BEQ   .sx_done@11
.sx_neg@10:
   LDA   #$FF
.sx_done@11:
   STA   HARGS+18
   STA   HARGS+19
   STA   HARGS+20
   STA   HARGS+21
   LDA   __local_f3__1
   STA   HARGS+16
   LDA   __local_f3__0
   STA   HARGS+17
   RTS

f3@run0:
   SUBROUTINE

   LDA   g0
   ORA   g0+1
   ORA   g0+2
   ORA   g0+3
   RTS

f2:
   SUBROUTINE

   LDA   #$01
   CLC
   ADC   g0
   STA   __local_f2__3
   LDA   #$00
   ADC   g0+1
   STA   __local_f2__2
   LDA   #$00
   ADC   g0+2
   STA   __local_f2__1
   LDA   #$00
   ADC   g0+3
   STA   __local_f2__0
   LDA   g0
   CLC
   SBC   __local_f2__3
   LDA   g0+1
   SBC   __local_f2__2
   LDA   g0+2
   SBC   __local_f2__1
   LDA   g0+3
   SBC   __local_f2__0
   LDA   #$00
   ROL
   STA   __local_f2__14
   LDA   #$00
   CMP   g0+3
   BNE   .cmp_end@13
   CMP   g0+2
   BNE   .cmp_end@13
   CMP   g0+1
   BNE   .cmp_end@13
   CMP   g0
   BNE   .cmp_end@13
   LDA   #$01
.cmp_end@13:
   STA   __local_f2__2
   LDA   __local_f2__14
   SEC
   SBC   __local_f2__14
   STA   __local_f2__1
   LDA   #$00
   SBC   #$00
   STA   __local_f2__0
   LDA   __local_f2__2
   SEC
   SBC   __local_f2__1
   STA   __local_f2__13
   LDA   #$00
   SBC   __local_f2__0
   STA   __local_f2__12
   JSR   f3@run0
   BNE   .or_true@3
   LDA   __local_f2__14
   CLC
   ADC   g0
   STA   __local_f2__3
   LDA   #$00
   ADC   g0+1
   STA   __local_f2__2
   LDA   #$00
   ADC   g0+2
   STA   __local_f2__1
   LDA   #$00
   ADC   g0+3
   STA   __local_f2__0
   LDA   __local_f2__3
   ORA   __local_f2__2
   ORA   __local_f2__1
   ORA   __local_f2__0
   BNE   .or_true@3
   LDA   #$00
   STA   __local_f2__8
   STA   __local_f2__7
   JMP   .or_end@4
.or_true@3:
   LDA   #$01
   STA   __local_f2__8
   LDA   #$00
   STA   __local_f2__7
.or_end@4:
   ORA   #$00
   BPL   .sx_done@16
   LDA   #$FF
.sx_done@16:
   STA   __local_f2__6
   STA   __local_f2__5
   LDA   __local_f2__14
   STA   __local_f2__box4
   LDA   #$00
   STA   __local_f2__box4+1
   LDX   #>__local_f2__box4
   LDA   #<__local_f2__box4
   JSR   f4
   LDA   __local_f2__8
   EOR   HARGS+8
   STA   __local_f2__9
   LDA   __local_f2__7
   EOR   HARGS+9
   STA   __local_f2__8
   LDA   __local_f2__6
   EOR   HARGS+10
   STA   __local_f2__7
   LDA   __local_f2__5
   EOR   HARGS+11
   STA   __local_f2__6
   LDA   __local_f2__box4
   STA   __local_f2__4
   LDA   __local_f2__box4+1
   STA   __local_f2__3
   BMI   .sx_neg@17
   LDA   #$00
   BEQ   .sx_done@18
.sx_neg@17:
   LDA   #$FF
.sx_done@18:
   STA   __local_f2__2
   STA   __local_f2__1
   LDA   __local_f2__9
   CLC
   ADC   __local_f2__4
   STA   __local_f2__4
   LDA   __local_f2__8
   ADC   __local_f2__3
   STA   __local_f2__3
   LDA   __local_f2__7
   ADC   __local_f2__2
   STA   __local_f2__2
   LDA   __local_f2__6
   ADC   __local_f2__1
   LDX   __local_f2__4
   STX   __local_f2__11
   LDX   __local_f2__3
   STX   __local_f2__10
   LDX   __local_f2__2
   STX   __local_f2__9
   STA   __local_f2__8
   LDA   __local_f2__14
   STA   __local_f2__box3
   LDA   #$00
   STA   __local_f2__box3+1
   LDX   #>__local_f2__box3
   LDA   #<__local_f2__box3
   JSR   f3
   LDA   __local_f2__11
   EOR   HARGS+16
   STA   __local_f2__4
   LDA   __local_f2__10
   EOR   HARGS+17
   STA   __local_f2__3
   LDA   __local_f2__9
   EOR   HARGS+18
   STA   __local_f2__2
   LDA   __local_f2__8
   EOR   HARGS+19
   LDX   __local_f2__4
   STX   __local_f2__9
   LDX   __local_f2__3
   STX   __local_f2__8
   LDX   __local_f2__2
   STX   __local_f2__7
   STA   __local_f2__6
   LDA   __local_f2__box3
   STA   __local_f2__4
   LDA   __local_f2__box3+1
   STA   __local_f2__3
   BMI   .sx_neg@21
   LDA   #$00
   BEQ   .sx_done@22
.sx_neg@21:
   LDA   #$FF
.sx_done@22:
   STA   __local_f2__2
   STA   __local_f2__1
   LDA   __local_f2__9
   CLC
   ADC   __local_f2__4
   STA   __local_f2__4
   LDA   __local_f2__8
   ADC   __local_f2__3
   STA   __local_f2__3
   LDA   __local_f2__7
   ADC   __local_f2__2
   STA   __local_f2__2
   LDA   __local_f2__6
   ADC   __local_f2__1
   LDX   __local_f2__3
   STX   __local_f2__8
   LDX   __local_f2__2
   STX   __local_f2__7
   STA   __local_f2__6
   LDA   g0
   CLC
   ADC   __local_f2__4
   STA   __local_f2__4
   LDA   g0+1
   ADC   __local_f2__8
   STA   __local_f2__3
   LDA   g0+2
   ADC   __local_f2__7
   STA   __local_f2__2
   LDA   g0+3
   ADC   __local_f2__6
   LDX   __local_f2__4
   STX   g0
   LDX   __local_f2__3
   STX   g0+1
   LDX   __local_f2__2
   STX   g0+2
   STA   g0+3
   LDA   __local_f2__13
   STA   HARGS+8
   LDA   __local_f2__12
   JMP   f4@tail0

f1:
   SUBROUTINE

   LDA   g0
   AND   #$07
   TAX
   LDA   #$07
   STA   __local_f1__l0
   CPX   #$00
   BEQ   .shift_done@24
.shift_loop@23:
   ASL   __local_f1__l0
   DEX
   BNE   .shift_loop@23
.shift_done@24:
   LDA   __local_f1__l0
   STA   __local_f1__box4
   BMI   .sx_neg@25
   LDA   #$00
   BEQ   .sx_done@26
.sx_neg@25:
   LDA   #$FF
.sx_done@26:
   STA   __local_f1__box4+1
   LDA   #$00
   STA   __local_f1__it_0
   LDY   #$00
   STY   __local_f1__it_1
.loop@0_start:
   LDA   __local_f1__it_0
   SEC
   SBC   #$03
   LDA   __local_f1__it_1
   SBC   #$00
   BVC   .jcmp_novf@27
   EOR   #$80
.jcmp_novf@27:
   BPL   .loop@0_break
   LDX   #>__local_f1__box4
   LDA   #<__local_f1__box4
   JSR   f4
   LDA   HARGS+8
   CLC
   ADC   __local_f1__it_0
   CLC
   ADC   __local_f1__l0
   STA   __local_f1__l0
   INC   __local_f1__it_0
   BNE   .loop@0_start
   INC   __local_f1__it_1
   JMP   .loop@0_start
.loop@0_break:
   LDA   __local_f1__l0
   CLC
   ADC   __local_f1__box4
   STA   __local_f1__it_0
   BMI   .sx_neg@28
   LDA   #$00
   BEQ   .sx_done@29
.sx_neg@28:
   LDA   #$FF
.sx_done@29:
   STA   __local_f1__it_1
   STA   __local_f1__1
   STA   __local_f1__0
   LDA   g0
   CLC
   ADC   __local_f1__it_0
   STA   g0
   LDA   g0+1
   ADC   __local_f1__it_1
   STA   g0+1
   LDA   g0+2
   ADC   __local_f1__1
   STA   g0+2
   LDA   g0+3
   ADC   __local_f1__0
   STA   g0+3
   ORA   g0+2
   ORA   g0+1
   ORA   g0
   JMP   f1@tail1

f1@tail1:
   SUBROUTINE

   BEQ   .cmp_end@30
   SEC
   RTS
.cmp_end@30:
   CMP   #$01
   RTS

f0:
   SUBROUTINE

   LDA   __zpabi_f0__p1_0
   CLC
   ADC   __zpabi_f0__p1_0
   STA   HARGS
   LDA   __zpabi_f0__p1_1
   ADC   __zpabi_f0__p1_1
   STA   HARGS+1
   LDA   __zpabi_f0__p1_2
   ADC   __zpabi_f0__p1_2
   STA   HARGS+2
   LDA   __zpabi_f0__p1_3
   ADC   __zpabi_f0__p1_3
   STA   HARGS+3
   LDA   #$00
   STA   HARGS+4
   JSR   lsr32
   LDA   HARGS+5
   STA   __local_f0__l0_0
   LDA   HARGS+6
   STA   __local_f0__l0_1
   LDA   HARGS+7
   STA   __local_f0__l0_2
   LDA   HARGS+8
   STA   __local_f0__l0_3
   LDY   #$01
   LDA   (__zpabi_f0__pp_0),Y
   STA   __local_f0__1
   DEY
   LDA   (__zpabi_f0__pp_0),Y
   CLC
   ADC   #$03
   BCC   .carry_inc@2
   INC   __local_f0__1
.carry_inc@2:
   LDY   #$00
   STA   (__zpabi_f0__pp_0),Y
   LDA   __local_f0__1
   INY
   STA   (__zpabi_f0__pp_0),Y
   LDA   __local_f0__l0_0
   STA   __local_f0__box4
   LDA   __local_f0__l0_1
   STA   __local_f0__box4+1
   LDA   #$00
   STA   __local_f0__it_0
   DEY
   STY   __local_f0__it_1
.loop@1_start:
   LDA   __local_f0__it_0
   SEC
   SBC   #$03
   LDA   __local_f0__it_1
   SBC   #$00
   BVC   .jcmp_novf@31
   EOR   #$80
.jcmp_novf@31:
   BPL   .loop@1_break
   LDX   #>__local_f0__box4
   LDA   #<__local_f0__box4
   JSR   f4
   LDA   __local_f0__it_1
   BMI   .sx_neg@32
   LDA   #$00
   BEQ   .sx_done@33
.sx_neg@32:
   LDA   #$FF
.sx_done@33:
   STA   __local_f0__1
   STA   __local_f0__0
   LDA   HARGS+8
   CLC
   ADC   __local_f0__it_0
   STA   __local_f0__3
   LDA   HARGS+9
   ADC   __local_f0__it_1
   STA   __local_f0__2
   LDA   HARGS+10
   ADC   __local_f0__1
   STA   __local_f0__1
   LDA   HARGS+11
   ADC   __local_f0__0
   STA   __local_f0__0
   LDA   __local_f0__l0_0
   CLC
   ADC   __local_f0__3
   STA   __local_f0__l0_0
   LDA   __local_f0__l0_1
   ADC   __local_f0__2
   STA   __local_f0__l0_1
   LDA   __local_f0__l0_2
   ADC   __local_f0__1
   STA   __local_f0__l0_2
   LDA   __local_f0__l0_3
   ADC   __local_f0__0
   STA   __local_f0__l0_3
   INC   __local_f0__it_0
   BNE   .loop@1_start
   INC   __local_f0__it_1
   JMP   .loop@1_start
.loop@1_break:
   LDA   __local_f0__box4
   STA   __local_f0__3
   LDA   __local_f0__box4+1
   STA   __local_f0__2
   BMI   .sx_neg@34
   LDA   #$00
   BEQ   .sx_done@35
.sx_neg@34:
   LDA   #$FF
.sx_done@35:
   STA   __local_f0__1
   STA   __local_f0__0
   LDA   __local_f0__l0_0
   CLC
   ADC   __local_f0__3
   STA   __local_f0__3
   LDA   __local_f0__l0_1
   ADC   __local_f0__2
   STA   __local_f0__2
   LDA   __local_f0__l0_2
   ADC   __local_f0__1
   STA   __local_f0__1
   LDA   __local_f0__l0_3
   ADC   __local_f0__0
   STA   __local_f0__0
   LDA   g0
   CLC
   ADC   __local_f0__3
   STA   g0
   LDA   g0+1
   ADC   __local_f0__2
   STA   g0+1
   LDA   g0+2
   ADC   __local_f0__1
   STA   g0+2
   LDA   g0+3
   ADC   __local_f0__0
   STA   g0+3
   LDA   __zpabi_f0__p2
   JMP   f1@tail1

main:
   SUBROUTINE

   LDA   #$05
   STA   __local_main__mbox
   LDA   #$00
   STA   __local_main__mbox+1
   LDY   #$AF
   STY   __zpabi_f0__p0_0
   STA   __zpabi_f0__p0_1
   STA   __zpabi_f0__p0_2
   STA   __zpabi_f0__p0_3
   STA   __zpabi_f0__p1_1
   STA   __zpabi_f0__p1_2
   STA   __zpabi_f0__p1_3
   STA   __zpabi_f0__p3_1
   STA   __zpabi_f0__p3_2
   STA   __zpabi_f0__p3_3
   LDA   #$67
   STA   __zpabi_f0__p1_0
   LDA   #$2C
   STA   __zpabi_f0__p2
   LDA   #$05
   STA   __zpabi_f0__p3_0
   LDA   #<__local_main__mbox
   STA   __zpabi_f0__pp_0
   LDA   #>__local_main__mbox
   STA   __zpabi_f0__pp_1
   JSR   f0
   LDA   #$00
   ROL
   EOR   g0
   STA   __local_main__3
   LDA   #$00
   EOR   g0+1
   STA   __local_main__2
   LDA   #$00
   EOR   g0+2
   STA   HARGS+2
   LDA   #$00
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

__zp_init:
   SUBROUTINE

   LDA   #$00
   STA   g0+1
   STA   g0+2
   STA   g0+3
   LDA   #$27
   STA   g0
   RTS
