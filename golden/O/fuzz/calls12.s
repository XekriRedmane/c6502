SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
g1	EQU	$26
g0	EQU	$2A
__local_f5__0	EQU	$80
__zpabi_f1__p0_0	EQU	$80
__local_f5__1	EQU	$81
__zpabi_f1__p0_1	EQU	$81
__local_f5__2	EQU	$82
__zpabi_f1__p0_2	EQU	$82
__local_f5__3	EQU	$83
__zpabi_f1__p0_3	EQU	$83
__local_f4__4	EQU	$84
__local_f5__4	EQU	$84
__zpabi_f1__pp_0	EQU	$84
__local_f4__5	EQU	$85
__zpabi_f1__pp_1	EQU	$85
__local_f1__0	EQU	$86
__local_f4__6	EQU	$86
__local_f1__1	EQU	$87
__local_f4__7	EQU	$87
__local_rec__0	EQU	$87
__local_rec__1	EQU	$88
__local_rec__2	EQU	$89
__local_rec__3	EQU	$8A
__local_rec__4	EQU	$8B

; @zp-link-meta-begin
; def f0 params= locals= indirect=false in_cycle=false
; def f1 params=__zpabi_f1__p0_0,__zpabi_f1__p0_1,__zpabi_f1__p0_2,__zpabi_f1__p0_3,__zpabi_f1__pp_0,__zpabi_f1__pp_1 locals=__local_f1__0,__local_f1__1 indirect=false in_cycle=false
; def f2 params= locals= indirect=false in_cycle=false
; def f3 params= locals= indirect=true in_cycle=false
; def f4 params= locals=__local_f4__0,__local_f4__1,__local_f4__2,__local_f4__3,__local_f4__4,__local_f4__5,__local_f4__6,__local_f4__7 indirect=false in_cycle=false
; def f5 params= locals=__local_f5__0,__local_f5__1,__local_f5__2,__local_f5__3,__local_f5__4,__local_f5__5,__local_f5__6 indirect=false in_cycle=false
; def main params= locals= indirect=false in_cycle=false
; def rec params= locals=__local_rec__0,__local_rec__1,__local_rec__2,__local_rec__3,__local_rec__4 indirect=false in_cycle=true
; call f0 -> f3
; call f0 -> rec
; call f2 -> f3
; call f2 -> f4
; call f2 -> rec
; call f3 -> f4
; call f3 -> rec
; call f5 -> rec
; call main -> f0
; call rec -> rec saved
; item rec section=.text local=false zero=false
; item f5 section=.text local=false zero=false
; item f4 section=.text local=false zero=false
; item f4@run0 section=.text local=true zero=false
; item f3 section=.text local=false zero=false
; item f2 section=.text local=false zero=false
; item f1 section=.text local=false zero=false
; item f0 section=.text local=false zero=false
; item main section=.text local=false zero=false
; item __zp_init section=.text local=false zero=false
; zp g1 size=4 at=auto def=true init=$59,$00,$00,$00 weight=30
; zp g0 size=4 at=auto def=true init=$87,$00,$00,$00 weight=21
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

f5:
   SUBROUTINE

   ; prologue: 3 arg bytes, 0 local bytes
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

   INY
   LDA   (FP),Y
   BMI   .sx_neg@1
   LDA   #$00
   BEQ   .sx_done@2
.sx_neg@1:
   LDA   #$FF
.sx_done@2:
   STA   __local_f5__4
   LDY   #$03
   LDA   (FP),Y
   BMI   .sx_neg@3
   LDA   #$00
   BEQ   .sx_done@4
.sx_neg@3:
   LDA   #$FF
.sx_done@4:
   STA   __local_f5__3
   STA   __local_f5__2
   STA   __local_f5__1
   LDY   #$04
   LDA   (FP),Y
   BMI   .sx_neg@5
   LDA   #$00
   BEQ   .sx_done@6
.sx_neg@5:
   LDA   #$FF
.sx_done@6:
   STA   __local_f5__0
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
   LDY   #$04
   LDA   (FP),Y
   DEY
   STA   (SSP),Y
   LDA   __local_f5__0
   INY
   STA   (SSP),Y
   JSR   rec
   LDY   #$03
   LDA   (FP),Y
   SEC
   SBC   #$FF
   LDA   __local_f5__3
   SBC   #$00
   LDA   __local_f5__2
   SBC   #$00
   LDA   __local_f5__1
   SBC   #$00
   LDA   #$00
   ROL
   STA   __local_f5__1
   LDA   (FP),Y
   SEC
   INY
   SBC   (FP),Y
   LDA   __local_f5__4
   SBC   __local_f5__0
   BVC   .cmp_novf@7
   EOR   #$80
.cmp_novf@7:
   ASL
   LDA   #$00
   ROL
   EOR   #$01
   ORA   __local_f5__1
   STA   __local_f5__0
   EOR   __local_f5__0
   LDX   #$00

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$05
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

   LDA   g1
   AND   #$64
   EOR   g0
   STA   __local_f4__7
   LDA   #$00
   EOR   g0+1
   STA   __local_f4__6
   LDA   #$00
   EOR   g0+2
   STA   __local_f4__5
   LDA   #$00
   EOR   g0+3
   STA   __local_f4__4
   LDA   g0
   AND   #$07
   STA   HARGS+4
   LDA   __local_f4__7
   STA   HARGS
   LDA   __local_f4__6
   STA   HARGS+1
   LDA   __local_f4__5
   STA   HARGS+2
   LDA   __local_f4__4
   STA   HARGS+3
   JSR   asl32
   JSR   f4@run0
   BEQ   .and_false@1
   LDA   g1
   AND   #$07
   STA   HARGS+4
   LDA   __local_f4__7
   STA   HARGS
   LDA   __local_f4__6
   STA   HARGS+1
   LDA   __local_f4__5
   STA   HARGS+2
   LDA   __local_f4__4
   STA   HARGS+3
   JSR   asr32
   JSR   f4@run0
   BEQ   .and_false@1
   LDA   #$01
   RTS
.and_false@1:
   LDA   #$00
   RTS

f4@run0:
   SUBROUTINE

   LDA   HARGS+5
   ORA   HARGS+6
   ORA   HARGS+7
   ORA   HARGS+8
   RTS

f3:
   SUBROUTINE

   ; prologue: 1 arg bytes, 14 local bytes, 14 callee-saved bytes
   SEC
   LDA   SSP
   SBC   #$10
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$0F
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1
   LDA   $C0
   LDY   #$01
   STA   (FP),Y
   LDA   $C1
   INY
   STA   (FP),Y
   LDA   $C2
   INY
   STA   (FP),Y
   LDA   $C3
   INY
   STA   (FP),Y
   LDA   $C4
   INY
   STA   (FP),Y
   LDA   $C5
   INY
   STA   (FP),Y
   LDA   $C6
   INY
   STA   (FP),Y
   LDA   $C7
   INY
   STA   (FP),Y
   LDA   $C8
   INY
   STA   (FP),Y
   LDA   $C9
   INY
   STA   (FP),Y
   LDA   $CA
   INY
   STA   (FP),Y
   LDA   $CB
   INY
   STA   (FP),Y
   LDA   $CC
   INY
   STA   (FP),Y
   LDA   $CD
   INY
   STA   (FP),Y

   LDA   #$FF
   SEC
   SBC   g0
   STA   $CD
   BMI   .sx_neg@8
   LDA   #$00
   BEQ   .sx_done@9
.sx_neg@8:
   LDA   #$FF
.sx_done@9:
   STA   $80
   LDA   $CD
   ORA   $CD
   STA   $C3
   LDA   $80
   ORA   $80
   STA   $C2
   LDY   #$11
   LDA   (FP),Y
   BMI   .sx_neg@10
   LDA   #$00
   BEQ   .sx_done@11
.sx_neg@10:
   LDA   #$FF
.sx_done@11:
   STA   $C1
   LDY   #$11
   LDA   (FP),Y
   ORA   #$01
   STA   HARGS+2
   LDA   $C1
   STA   HARGS+3
   LDA   $CD
   STA   HARGS
   LDA   $80
   STA   HARGS+1
   JSR   sdivmod16
   LDA   HARGS+6
   STA   $81
   LDA   HARGS+7
   STA   $80
   LDA   $C3
   CLC
   ADC   $81
   STA   $C5
   LDA   $C2
   ADC   $80
   STA   $C4
   BMI   .sx_neg@12
   LDA   #$00
   BEQ   .sx_done@13
.sx_neg@12:
   LDA   #$FF
.sx_done@13:
   STA   $C3
   STA   $C2
   LDA   g0
   AND   #$07
   TAX
   LDY   #$11
   LDA   (FP),Y
   STA   $C0
   CPX   #$00
   BEQ   .shift_done@15
.shift_loop@14:
   LDA   $C1
   ASL
   ROR   $C1
   ROR   $C0
   DEX
   BNE   .shift_loop@14
.shift_done@15:
   LDA   $C1
   BMI   .sx_neg@16
   LDA   #$00
   BEQ   .sx_done@17
.sx_neg@16:
   LDA   #$FF
.sx_done@17:
   STA   $81
   STA   $80
   CMP   $C2
   BNE   .cmp_true@18
   CMP   $C3
   BNE   .cmp_true@18
   LDA   $C1
   CMP   $C4
   BNE   .cmp_true@18
   LDA   $C0
   CMP   $C5
   BNE   .cmp_true@18
   LDA   #$00
   BEQ   .cmp_end@19
.cmp_true@18:
   LDA   #$01
.cmp_end@19:
   STA   $CC
   LDA   #<f5
   STA   $CA
   LDA   #>f5
   STA   $CB
   LDA   $CC
   BMI   .sx_neg@21
   LDA   #$00
   BEQ   .sx_done@22
.sx_neg@21:
   LDA   #$FF
.sx_done@22:
   STA   $C9
   STA   $C8
   STA   $C7
   LDA   #$00
   STA   $C6
   LDY   #$00
   STY   $C1
.loop@0_start:
   LDA   $C6
   SEC
   SBC   #$03
   LDA   $C1
   SBC   #$00
   BVC   .jcmp_novf@23
   EOR   #$80
.jcmp_novf@23:
   BMI   .lb_skip@1
   JMP   .loop@0_break
.lb_skip@1:
   LDA   $CC
   EOR   #$01
   STA   $C0
   LDA   g1
   ORA   #$01
   STA   HARGS+4
   LDA   g1+1
   STA   HARGS+5
   STA   HARGS+1
   LDA   g1+2
   STA   HARGS+6
   STA   HARGS+2
   LDA   g1
   STA   HARGS
   LDA   g1+3
   STA   HARGS+7
   STA   HARGS+3
   JSR   udivmod32
   LDA   HARGS+12
   STA   $81
   LDA   g0
   SEC
   SBC   $CC
   LDA   g0+1
   SBC   $C9
   LDA   g0+2
   SBC   $C8
   LDA   g0+3
   SBC   $C7
   LDA   #$00
   ROL
   STA   $80
   SEC
   LDA   SSP
   SBC   #$03
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   $C0
   LDY   #$01
   STA   (SSP),Y
   LDA   $81
   INY
   STA   (SSP),Y
   LDA   $80
   INY
   STA   (SSP),Y
   LDA   $CA
   STA   DPTR
   LDA   $CB
   STA   DPTR+1
   JSR   icall
   CLC
   ADC   $C6
   STA   $83
   TXA
   ADC   $C1
   STA   $82
   BMI   .sx_neg@24
   LDA   #$00
   BEQ   .sx_done@25
.sx_neg@24:
   LDA   #$FF
.sx_done@25:
   STA   $81
   STA   $80
   LDA   $C5
   CLC
   ADC   $83
   STA   $C5
   LDA   $C4
   ADC   $82
   STA   $C4
   LDA   $C3
   ADC   $81
   STA   $C3
   LDA   $C2
   ADC   $80
   STA   $C2
   INC   $C6
   BEQ   .lb_skip@0
   JMP   .loop@0_start
.lb_skip@0:
   INC   $C1
   JMP   .loop@0_start
.loop@0_break:
   LDA   g1
   CLC
   ADC   $C5
   STA   g1
   LDA   g1+1
   ADC   $C4
   STA   g1+1
   LDA   g1+2
   ADC   $C3
   STA   g1+2
   LDA   g1+3
   ADC   $C2
   STA   g1+3
   LDA   #$00
   STA   $C1
   LDY   #$00
   STY   $C0
.loop@1_start:
   LDA   $C1
   SEC
   SBC   #$03
   LDA   $C0
   SBC   #$00
   BVC   .jcmp_novf@26
   EOR   #$80
.jcmp_novf@26:
   BPL   .loop@1_break
   JSR   f4
   CLC
   ADC   $C1
   STA   $83
   LDA   #$00
   ADC   $C0
   STA   $82
   BMI   .sx_neg@27
   LDA   #$00
   BEQ   .sx_done@28
.sx_neg@27:
   LDA   #$FF
.sx_done@28:
   STA   $81
   STA   $80
   LDA   $C5
   CLC
   ADC   $83
   STA   $C5
   LDA   $C4
   ADC   $82
   STA   $C4
   LDA   $C3
   ADC   $81
   STA   $C3
   LDA   $C2
   ADC   $80
   STA   $C2
   INC   $C1
   BNE   .loop@1_start
   INC   $C0
   JMP   .loop@1_start
.loop@1_break:
   LDA   g0
   CLC
   SBC   g1
   LDA   g0+1
   SBC   g1+1
   LDA   g0+2
   SBC   g1+2
   LDA   g0+3
   SBC   g1+3
   LDA   #$00
   ROL
   STA   $80
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
   LDA   $80
   INY
   STA   (SSP),Y
   LDA   #$00
   INY
   STA   (SSP),Y
   JSR   rec
   LDA   $CD
   BEQ   .cmp_end@29
   LDA   #$01
.cmp_end@29:

   ; epilogue
   PHA
   LDY   #$01
   LDA   (FP),Y
   STA   $C0
   INY
   LDA   (FP),Y
   STA   $C1
   INY
   LDA   (FP),Y
   STA   $C2
   INY
   LDA   (FP),Y
   STA   $C3
   INY
   LDA   (FP),Y
   STA   $C4
   INY
   LDA   (FP),Y
   STA   $C5
   INY
   LDA   (FP),Y
   STA   $C6
   INY
   LDA   (FP),Y
   STA   $C7
   INY
   LDA   (FP),Y
   STA   $C8
   INY
   LDA   (FP),Y
   STA   $C9
   INY
   LDA   (FP),Y
   STA   $CA
   INY
   LDA   (FP),Y
   STA   $CB
   INY
   LDA   (FP),Y
   STA   $CC
   INY
   LDA   (FP),Y
   STA   $CD
   CLC
   LDA   FP
   ADC   #$11
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   INY
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   PLA
   RTS

f2:
   SUBROUTINE

   ; prologue: 2 arg bytes, 5 local bytes, 5 callee-saved bytes
   SEC
   LDA   SSP
   SBC   #$07
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$06
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1
   LDA   $C0
   LDY   #$01
   STA   (FP),Y
   LDA   $C1
   INY
   STA   (FP),Y
   LDA   $C2
   INY
   STA   (FP),Y
   LDA   $C3
   INY
   STA   (FP),Y
   LDA   $C4
   INY
   STA   (FP),Y

   LDA   g0
   EOR   g1
   STA   $83
   LDA   g0+1
   EOR   g1+1
   STA   $82
   LDA   $83
   EOR   #$FF
   STA   $C4
   SEC
   SBC   $C4
   STA   $81
   BMI   .sx_neg@30
   LDA   #$00
   BEQ   .sx_done@31
.sx_neg@30:
   LDA   #$FF
.sx_done@31:
   STA   $80
   SEC
   LDA   #$00
   SBC   $81
   STA   $81
   LDA   #$00
   SBC   $80
   STA   $80
   LDA   $81
   AND   #$07
   STA   HARGS+4
   LDA   g1
   STA   HARGS
   LDA   g1+1
   STA   HARGS+1
   LDA   g1+2
   STA   HARGS+2
   LDA   g1+3
   STA   HARGS+3
   JSR   asl32
   LDA   HARGS+5
   STA   $C0
   LDA   HARGS+6
   STA   $C3
   LDA   HARGS+7
   STA   $C2
   LDA   HARGS+8
   STA   $C1
   LDA   #$07
   SEC
   SBC   $C4
   LDA   #$00
   ROL
   STA   $80
   SEC
   LDA   SSP
   SBC   #$01
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   $80
   LDY   #$01
   STA   (SSP),Y
   JSR   f3
   STA   $80
   ORA   #$00
   BEQ   .if_else@4
   LDA   $C0
   CLC
   ADC   #$03
   STA   $C0
   BCC   .if_end@3
   INC   $C3
   BNE   .if_end@3
   INC   $C2
   BNE   .if_end@3
   INC   $C1
   JMP   .if_end@3
.if_else@4:
   LDA   $C0
   EOR   #$05
   STA   $C0
.if_end@3:
   LDA   g0
   CLC
   ADC   $C0
   STA   g0
   LDA   g0+1
   ADC   $C3
   STA   g0+1
   LDA   g0+2
   ADC   $C2
   STA   g0+2
   LDA   g0+3
   ADC   $C1
   STA   g0+3
   JSR   f4
   LDA   $C4
   SEC
   SBC   $C0
   LDA   #$00
   SBC   $C3
   LDA   #$00
   SBC   $C2
   LDA   #$00
   SBC   $C1
   BVC   .cmp_novf@32
   EOR   #$80
.cmp_novf@32:
   ASL
   LDA   #$00
   ROL
   EOR   #$01
   STA   $80
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
   LDA   $80
   INY
   STA   (SSP),Y
   LDA   #$00
   INY
   STA   (SSP),Y
   JSR   rec
   STA   $83
   STX   $82
   TXA
   BMI   .sx_neg@33
   LDA   #$00
   BEQ   .sx_done@34
.sx_neg@33:
   LDA   #$FF
.sx_done@34:
   STA   $81
   STA   $80
   LDA   $C0
   CLC
   ADC   $83
   STA   $83
   LDA   $C3
   ADC   $82
   STA   $82
   LDA   $C2
   ADC   $81
   STA   $81
   LDA   $C1
   ADC   $80
   STA   $80
   LDA   g1
   ORA   g1+1
   ORA   g1+2
   ORA   g1+3
   BEQ   .and_false@5
   LDA   #$01
   STA   $C3
   LDA   #$00
   STA   $C2
   JMP   .and_end@6
.and_false@5:
   LDA   #$00
   STA   $C3
   LDY   #$00
   STY   $C2
.and_end@6:
   ORA   #$00
   BPL   .sx_done@36
   LDA   #$FF
.sx_done@36:
   STA   $C1
   STA   $C0
   LDA   $83
   STA   HARGS
   LDA   $82
   STA   HARGS+1
   LDA   $81
   STA   HARGS+2
   LDA   $80
   STA   HARGS+3
   LDA   #$04
   STA   HARGS+4
   JSR   asr32
   LDA   HARGS+5
   STA   $83
   LDA   HARGS+6
   STA   $82
   LDA   HARGS+7
   STA   $81
   LDA   HARGS+8
   STA   $80
   LDA   $83
   ORA   #$01
   STA   HARGS+4
   LDA   $82
   STA   HARGS+5
   LDA   $81
   STA   HARGS+6
   LDA   $80
   STA   HARGS+7
   LDA   $C3
   STA   HARGS
   LDA   $C2
   STA   HARGS+1
   LDA   $C1
   STA   HARGS+2
   LDA   $C0
   STA   HARGS+3
   JSR   sdivmod32
   LDA   HARGS+12
   STA   HARGS+8
   LDA   HARGS+13
   STA   HARGS+9
   LDA   HARGS+14
   STA   HARGS+10
   LDA   HARGS+15
   STA   HARGS+11

   ; epilogue
   LDY   #$01
   LDA   (FP),Y
   STA   $C0
   INY
   LDA   (FP),Y
   STA   $C1
   INY
   LDA   (FP),Y
   STA   $C2
   INY
   LDA   (FP),Y
   STA   $C3
   INY
   LDA   (FP),Y
   STA   $C4
   CLC
   LDA   FP
   ADC   #$09
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   INY
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   RTS

f1:
   SUBROUTINE

   LDY   #$01
   LDA   (__zpabi_f1__pp_0),Y
   STA   __local_f1__1
   DEY
   LDA   (__zpabi_f1__pp_0),Y
   CLC
   ADC   #$64
   BCC   .carry_inc@2
   INC   __local_f1__1
.carry_inc@2:
   LDY   #$00
   STA   (__zpabi_f1__pp_0),Y
   LDA   __local_f1__1
   INY
   STA   (__zpabi_f1__pp_0),Y
   DEY
   LDA   (__zpabi_f1__pp_0),Y
   STA   __local_f1__1
   INY
   LDA   (__zpabi_f1__pp_0),Y
   STA   __local_f1__0
   BMI   .sx_neg@37
   LDA   #$00
   BEQ   .sx_done@38
.sx_neg@37:
   LDA   #$FF
.sx_done@38:
   STA   HARGS+10
   STA   HARGS+11
   LDA   __local_f1__1
   STA   HARGS+8
   LDA   __local_f1__0
   STA   HARGS+9
   RTS

f0:
   SUBROUTINE

   ; prologue: 8 arg bytes, 1 local bytes, 1 callee-saved bytes
   SEC
   LDA   SSP
   SBC   #$03
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$02
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1
   LDA   $C0
   LDY   #$01
   STA   (FP),Y

   LDA   #$00
   CMP   g1+3
   BNE   .cmp_true@39
   CMP   g1+2
   BNE   .cmp_true@39
   CMP   g1+1
   BNE   .cmp_true@39
   CMP   g1
   BEQ   .cmp_end@40
.cmp_true@39:
   LDA   #$01
.cmp_end@40:
   STA   $C0
   LDA   g0
   STA   $80
   SEC
   LDA   SSP
   SBC   #$01
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   $80
   LDY   #$01
   STA   (SSP),Y
   JSR   f3
   STA   $80
   LDA   $C0
   EOR   $80
   STA   $83
   BMI   .sx_neg@42
   LDA   #$00
   BEQ   .sx_done@43
.sx_neg@42:
   LDA   #$FF
.sx_done@43:
   STA   $82
   STA   $81
   STA   $80
   LDA   g1
   CLC
   ADC   $83
   STA   g1
   LDA   g1+1
   ADC   $82
   STA   g1+1
   LDA   g1+2
   ADC   $81
   STA   g1+2
   LDA   g1+3
   ADC   $80
   STA   g1+3
   LDA   #$00
   LDY   #$05
   CMP   (FP),Y
   BNE   .cmp_true@44
   LDA   #$64
   DEY
   CMP   (FP),Y
   BNE   .cmp_true@44
   LDA   #$00
   BEQ   .cmp_end@45
.cmp_true@44:
   LDA   #$01
.cmp_end@45:
   STA   $80
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
   LDA   $80
   INY
   STA   (SSP),Y
   LDA   #$00
   INY
   STA   (SSP),Y
   JSR   rec
   LDA   #$00
   CMP   g0+3
   BNE   .cmp_true@47
   CMP   g0+2
   BNE   .cmp_true@47
   LDY   #$0B
   LDA   (FP),Y
   CMP   g0+1
   BNE   .cmp_true@47
   DEY
   LDA   (FP),Y
   CMP   g0
   BNE   .cmp_true@47
   LDA   #$00
   BEQ   .cmp_end@48
.cmp_true@47:
   LDA   #$01
.cmp_end@48:
   STA   $82
   LDA   #$00
   BPL   .sx_done@51
   LDA   #$FF
.sx_done@51:
   STA   $81
   STA   $80
   LDA   $82
   CLC
   SBC   g0
   LDA   #$00
   SBC   g0+1
   LDA   $81
   SBC   g0+2
   LDA   $80
   SBC   g0+3
   LDA   #$00
   ROL
   LDX   #$00

   ; epilogue
   PHA
   TXA
   PHA
   LDY   #$01
   LDA   (FP),Y
   STA   $C0
   CLC
   LDA   FP
   ADC   #$0B
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   INY
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

main:
   SUBROUTINE

   ; prologue: 0 arg bytes, 2 local bytes, 2 callee-saved bytes
   SEC
   LDA   SSP
   SBC   #$04
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$03
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1
   LDA   $C0
   LDY   #$01
   STA   (FP),Y
   LDA   $C1
   INY
   STA   (FP),Y

   SEC
   LDA   SSP
   SBC   #$08
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   #$63
   DEY
   STA   (SSP),Y
   LDA   #$00
   INY
   STA   (SSP),Y
   LDA   #$12
   INY
   STA   (SSP),Y
   LDA   #$01
   INY
   STA   (SSP),Y
   LDA   #$00
   INY
   STA   (SSP),Y
   INY
   STA   (SSP),Y
   LDA   #$B9
   INY
   STA   (SSP),Y
   LDA   #$00
   INY
   STA   (SSP),Y
   JSR   f0
   STA   $8B
   STX   $8A
   TXA
   BMI   .sx_neg@52
   LDA   #$00
   BEQ   .sx_done@53
.sx_neg@52:
   LDA   #$FF
.sx_done@53:
   STA   $89
   STA   $88
   LDA   g0
   STA   $87
   LDA   g0+1
   STA   $86
   LDA   g0+2
   STA   $85
   LDA   g0+3
   STA   $84
   LDA   g1
   STA   $83
   LDA   g1+1
   STA   $82
   LDA   g1+2
   STA   $81
   LDA   g1+3
   STA   $80
   LDA   $87
   EOR   $83
   STA   $83
   LDA   $86
   EOR   $82
   STA   $82
   LDA   $85
   EOR   $81
   STA   $81
   LDA   $84
   EOR   $80
   STA   $80
   LDA   $8B
   EOR   $83
   STA   $C1
   TXA
   EOR   $82
   STA   $C0
   LDA   $89
   EOR   $81
   STA   HARGS+2
   LDA   $88
   EOR   $80
   STA   HARGS+3
   LDA   $C1
   STA   HARGS
   LDA   $C0
   STA   HARGS+1
   LDA   #$10
   STA   HARGS+4
   JSR   asr32
   LDA   HARGS+5
   STA   $80
   LDA   HARGS+6
   STA   $81
   LDA   $C1
   EOR   $80
   STA   $80
   LDA   $C0
   EOR   $81
   TAX
   LDA   $80

   ; epilogue
   PHA
   TXA
   PHA
   LDY   #$01
   LDA   (FP),Y
   STA   $C0
   INY
   LDA   (FP),Y
   STA   $C1
   CLC
   LDA   FP
   ADC   #$04
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   INY
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

__zp_init:
   SUBROUTINE

   LDA   #$00
   STA   g0+1
   STA   g0+2
   STA   g0+3
   STA   g1+1
   STA   g1+2
   STA   g1+3
   LDA   #$87
   STA   g0
   LDA   #$59
   STA   g1
   RTS
