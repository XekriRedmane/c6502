SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
g1	EQU	$26
g2	EQU	$27
__local_f5__0	EQU	$80
__local_main__0	EQU	$80
__local_rec__0	EQU	$80
__zpabi_f1__p0_0	EQU	$80
__local_f5__1	EQU	$81
__local_main__1	EQU	$81
__local_rec__1	EQU	$81
__zpabi_f1__p0_1	EQU	$81
__local_f5__2	EQU	$82
__local_main__2	EQU	$82
__local_rec__2	EQU	$82
__zpabi_f1__p0_2	EQU	$82
__local_f5__3	EQU	$83
__local_main__3	EQU	$83
__local_rec__3	EQU	$83
__zpabi_f1__p0_3	EQU	$83
__local_main__4	EQU	$84
__local_rec__4	EQU	$84
__zpabi_f1__p1_0	EQU	$84
__local_main__5	EQU	$85
__zpabi_f1__p1_1	EQU	$85
__local_main__6	EQU	$86
__zpabi_f1__p1_2	EQU	$86
__local_main__mbox	EQU	$87
__zpabi_f1__p1_3	EQU	$87
__local_f1__0	EQU	$88
__local_f0__0	EQU	$8B
__local_f0__1	EQU	$8C
__local_f0__2	EQU	$8D
__local_f0__3	EQU	$8E
__local_f0__4	EQU	$8F
__local_f0__pp	EQU	$90
__local_f0__5	EQU	$91

; @zp-link-meta-begin
; def f0 params=__zpabi_f0__pp_0,__zpabi_f0__pp_1 locals=__local_f0__0,__local_f0__1,__local_f0__2,__local_f0__3,__local_f0__4,__local_f0__pp,__local_f0__5 indirect=false in_cycle=false param_regs=A,X carry=true
; def f1 params=__zpabi_f1__p0_0,__zpabi_f1__p0_1,__zpabi_f1__p0_2,__zpabi_f1__p0_3,__zpabi_f1__p1_0,__zpabi_f1__p1_1,__zpabi_f1__p1_2,__zpabi_f1__p1_3 locals=__local_f1__0,__local_f1__1 indirect=false in_cycle=false
; def f2 params= locals= indirect=true in_cycle=false
; def f3 params= locals= indirect=true in_cycle=false carry=true
; def f4 params= locals= indirect=true in_cycle=false
; def f5 params= locals=__local_f5__0,__local_f5__1,__local_f5__2,__local_f5__3 indirect=false in_cycle=false
; def main params= locals=__local_main__0,__local_main__1,__local_main__2,__local_main__3,__local_main__4,__local_main__5,__local_main__6,__local_main__mbox,__local_main__8,__local_main__9,__local_main__10 indirect=false in_cycle=false
; def rec params= locals=__local_rec__0,__local_rec__1,__local_rec__2,__local_rec__3,__local_rec__4 indirect=false in_cycle=true
; call f2 -> f4
; call f2 -> rec
; call f3 -> f4
; call main -> f0
; call rec -> rec saved
; item rec section=.text local=false zero=false
; item f5 section=.text local=false zero=false
; item f4 section=.text local=false zero=false
; item f3 section=.text local=false zero=false
; item f2 section=.text local=false zero=false
; item f1 section=.text local=false zero=false
; item f0 section=.text local=false zero=false
; item main section=.text local=false zero=false
; item g0 section=.data local=false zero=false
; item __zp_init section=.text local=false zero=false
; zp g1 size=1 at=auto def=true init=$2D weight=18
; zp g2 size=2 at=auto def=true init=$39,$00 weight=19
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

   LDA   g2
   CLC
   ADC   g0
   STA   __local_f5__3
   LDA   g2+1
   ADC   g0+1
   STA   __local_f5__2
   LDA   #$00
   ADC   g0+2
   STA   __local_f5__1
   LDA   #$00
   ADC   g0+3
   STA   __local_f5__0
   LDA   __local_f5__3
   ORA   __local_f5__2
   ORA   __local_f5__1
   ORA   __local_f5__0
   BNE   .or_true@1
   LDA   g2
   ORA   g2+1
   BNE   .or_true@1
   LDA   #$00
   BEQ   .vthread@1
.or_true@1:
   LDA   #$01
.vthread@1:
   CLC
   ADC   g1
   AND   #$07
   STA   HARGS+4
   LDA   g0
   STA   HARGS
   LDA   g0+1
   STA   HARGS+1
   LDA   g0+2
   STA   HARGS+2
   LDA   g0+3
   STA   HARGS+3
   JSR   asr32
   LDX   HARGS+6
   LDA   HARGS+5
   RTS

f4:
   SUBROUTINE

   ; prologue: 5 arg bytes, 0 local bytes
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

   LDA   #<f5
   STA   $80
   LDA   #>f5
   STA   $81
   LDA   #<f5
   STA   DPTR
   LDA   #>f5
   STA   DPTR+1
   JSR   icall
   LDA   g0
   LDX   g0+1

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$07
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

   ; prologue: 7 arg bytes, 6 local bytes, 6 callee-saved bytes
   SEC
   LDA   SSP
   SBC   #$08
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$07
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

   LDY   #$0F
   LDA   (FP),Y
   BMI   .sx_neg@3
   LDA   #$00
   BEQ   .sx_done@4
.sx_neg@3:
   LDA   #$FF
.sx_done@4:
   STA   $C3
   LDY   #$0F
   LDA   (FP),Y
   AND   #$07
   TAX
   LDA   (FP),Y
   STA   $C2
   CPX   #$00
   BEQ   .shift_done@6
.shift_loop@5:
   LDA   $C3
   ASL
   ROR   $C3
   ROR   $C2
   DEX
   BNE   .shift_loop@5
.shift_done@6:
   LDA   $C3
   BMI   .sx_neg@7
   LDA   #$00
   BEQ   .sx_done@8
.sx_neg@7:
   LDA   #$FF
.sx_done@8:
   STA   $C1
   STA   $C0
   LDY   #$0A
   LDA   (FP),Y
   BMI   .sx_neg@9
   LDA   #$00
   BEQ   .sx_done@10
.sx_neg@9:
   LDA   #$FF
.sx_done@10:
   STA   $80
   LDY   #$0A
   LDA   (FP),Y
   ORA   #$01
   STA   $81
   LDA   $80
   BMI   .sx_neg@11
   LDA   #$00
   BEQ   .sx_done@12
.sx_neg@11:
   LDA   #$FF
.sx_done@12:
   STA   HARGS+6
   STA   HARGS+7
   LDY   #$0B
   LDA   (FP),Y
   STA   HARGS
   INY
   LDA   (FP),Y
   STA   HARGS+1
   INY
   LDA   (FP),Y
   STA   HARGS+2
   INY
   LDA   (FP),Y
   STA   HARGS+3
   LDA   $81
   STA   HARGS+4
   LDA   $80
   STA   HARGS+5
   JSR   sdivmod32
   LDA   HARGS+12
   STA   HARGS+4
   LDA   HARGS+13
   STA   HARGS+5
   LDA   HARGS+14
   STA   HARGS+6
   LDA   HARGS+15
   STA   HARGS+7
   LDA   $C2
   STA   HARGS
   LDA   $C3
   STA   HARGS+1
   LDA   $C1
   STA   HARGS+2
   LDA   $C0
   STA   HARGS+3
   JSR   mul32
   LDA   HARGS+8
   STA   $C3
   LDA   HARGS+9
   STA   $C2
   LDA   HARGS+10
   STA   $C1
   LDA   HARGS+11
   STA   $C0
   LDA   #<f5
   STA   $80
   LDA   #>f5
   STA   $81
   LDA   #<f5
   STA   DPTR
   LDA   #>f5
   STA   DPTR+1
   JSR   icall
   STA   $83
   STX   $82
   TXA
   BMI   .sx_neg@13
   LDA   #$00
   BEQ   .sx_done@14
.sx_neg@13:
   LDA   #$FF
.sx_done@14:
   STA   $81
   STA   $80
   LDA   $C3
   EOR   $83
   STA   $C5
   LDA   $C2
   EOR   $82
   STA   $C4
   LDA   $C1
   EOR   $81
   STA   $C3
   LDA   $C0
   EOR   $80
   STA   $C2
   LDA   g1
   CLC
   ADC   $C5
   STA   g1
   LDA   #$00
   STA   $C1
   LDY   #$00
   STY   $C0
.loop@0_start:
   LDA   $C1
   SEC
   SBC   #$03
   LDA   $C0
   SBC   #$00
   BVC   .jcmp_novf@15
   EOR   #$80
.jcmp_novf@15:
   BPL   .loop@0_break
   LDA   g1
   STA   $83
   BMI   .sx_neg@16
   LDA   #$00
   BEQ   .sx_done@17
.sx_neg@16:
   LDA   #$FF
.sx_done@17:
   STA   $82
   STA   $81
   STA   $80
   SEC
   LDA   SSP
   SBC   #$05
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   #$02
   LDY   #$01
   STA   (SSP),Y
   LDA   $83
   INY
   STA   (SSP),Y
   LDA   $82
   INY
   STA   (SSP),Y
   LDA   $81
   INY
   STA   (SSP),Y
   LDA   $80
   INY
   STA   (SSP),Y
   JSR   f4
   CLC
   ADC   $C1
   STA   $83
   TXA
   ADC   $C0
   STA   $82
   BMI   .sx_neg@18
   LDA   #$00
   BEQ   .sx_done@19
.sx_neg@18:
   LDA   #$FF
.sx_done@19:
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
   BEQ   .lb_skip@0
   JMP   .loop@0_start
.lb_skip@0:
   INC   $C0
   JMP   .loop@0_start
.loop@0_break:
   LDA   g1
   CLC
   ADC   $C5
   STA   g1
   LDA   g2+1
   ORA   g2
   BEQ   .cmp_end@20
   LDA   #$01
.cmp_end@20:
   CMP   #$01

   ; epilogue
   PHP
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
   CLC
   LDA   FP
   ADC   #$0F
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
   PLP
   RTS

f2:
   SUBROUTINE

   ; prologue: 3 arg bytes, 9 local bytes, 9 callee-saved bytes
   SEC
   LDA   SSP
   SBC   #$0B
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$0A
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

   LDA   g2
   STA   $C1
   LDA   g2+1
   STA   $C0
   LDY   #$0E
   LDA   (FP),Y
   BMI   .sx_neg@21
   LDA   #$00
   BEQ   .sx_done@22
.sx_neg@21:
   LDA   #$FF
.sx_done@22:
   STA   $C8
   LDA   $C1
   LDY   #$0E
   EOR   (FP),Y
   STA   $81
   LDA   $C0
   EOR   $C8
   STA   $80
   LDA   #$01
   AND   $81
   STA   $C7
   LDA   #$00
   AND   $80
   STA   $C6
   LDA   #<f5
   STA   $80
   LDA   #>f5
   STA   $81
   LDA   #<f5
   STA   DPTR
   LDA   #>f5
   STA   DPTR+1
   JSR   icall
   STA   $81
   STX   $80
   LDA   $C1
   EOR   $81
   STA   $C3
   LDA   $C0
   EOR   $80
   STA   $C2
   LDA   g1
   CLC
   ADC   $C3
   STA   g1
   LDY   #$0D
   LDA   (FP),Y
   BMI   .sx_neg@23
   LDA   #$00
   BEQ   .sx_done@24
.sx_neg@23:
   LDA   #$FF
.sx_done@24:
   STA   $C5
   STA   $C4
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
   BVC   .jcmp_novf@25
   EOR   #$80
.jcmp_novf@25:
   BPL   .loop@1_break
   LDA   g2
   STA   $80
   SEC
   LDA   SSP
   SBC   #$05
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   $80
   LDY   #$01
   STA   (SSP),Y
   LDY   #$0C
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$0D
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDA   $C5
   INY
   STA   (SSP),Y
   LDA   $C4
   INY
   STA   (SSP),Y
   JSR   f4
   CLC
   ADC   $C1
   STA   $81
   TXA
   ADC   $C0
   STA   $80
   LDA   $C3
   CLC
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
   LDA   g2
   CLC
   ADC   $C3
   STA   g2
   LDA   g2+1
   ADC   $C2
   STA   g2+1
   LDA   $C7
   STA   HARGS
   LDA   $C6
   STA   HARGS+1
   LDA   $C3
   STA   HARGS+2
   LDA   $C2
   STA   HARGS+3
   JSR   mul16
   LDA   HARGS+4
   STA   $81
   LDA   HARGS+5
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
   LDA   $81
   INY
   STA   (SSP),Y
   LDA   $80
   INY
   STA   (SSP),Y
   JSR   rec
   LDY   #$0E
   LDA   (FP),Y
   ORA   #$01
   STA   HARGS+2
   LDA   $C8
   STA   HARGS+3
   LDA   #$00
   STA   HARGS
   STA   HARGS+1
   JSR   sdivmod16
   LDA   HARGS+4
   ORA   HARGS+5
   BNE   .or_true@3
   LDY   #$0E
   LDA   (FP),Y
   BNE   .or_true@3
   LDA   #$00
   LDX   #$00
   BEQ   .or_end@4
.or_true@3:
   LDA   #$01
   LDX   #$00
.or_end@4:

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
   CLC
   LDA   FP
   ADC   #$0E
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

f1:
   SUBROUTINE

   LDA   #$00
   CMP   g2+1
   BNE   .vthread@0
   LDA   #$01
   CMP   g2
   BEQ   .cmp_true@31
   LDA   #$00
.vthread@0:
.cmp_true@31:
   STA   __local_f1__0
   LDA   #$00
   BPL   .sx_done@35
   LDA   #$FF
.sx_done@35:
   STA   HARGS+10
   STA   HARGS+11
   LDA   __local_f1__0
   STA   HARGS+8
   LDA   #$00
   STA   HARGS+9
   RTS

f0:
   SUBROUTINE

   STA   __local_f0__pp
   STX   __local_f0__5
   LDA   g0
   AND   g2
   STA   __local_f0__2
   LDA   g0+1
   AND   g2+1
   STA   __local_f0__4
   LDA   __local_f0__2
   EOR   #$FF
   STA   __local_f0__2
   LDA   __local_f0__4
   EOR   #$FF
   STA   __local_f0__3
   ORA   __local_f0__2
   BNE   .or_true@5
   LDA   g1
   BEQ   .or_end@6
.or_true@5:
.or_end@6:
   LDY   #$00
   LDA   (__local_f0__pp),Y
   STA   __local_f0__1
   INY
   LDA   (__local_f0__pp),Y
   STA   __local_f0__0
   LDA   __local_f0__2
   AND   #$07
   TAX
   BEQ   .shift_done@37
.shift_loop@36:
   ASL   __local_f0__2
   ROL   __local_f0__3
   DEX
   BNE   .shift_loop@36
.shift_done@37:
   LDA   __local_f0__1
   CLC
   ADC   __local_f0__2
   STA   __local_f0__1
   LDA   __local_f0__0
   ADC   __local_f0__3
   LDY   #$01
   STA   (__local_f0__pp),Y
   LDA   __local_f0__1
   DEY
   STA   (__local_f0__pp),Y
   LDA   g0+3
   ORA   g0+2
   ORA   g0+1
   ORA   g0
   BEQ   .cmp_end@38
   SEC
   RTS
.cmp_end@38:
   CMP   #$01
   RTS

main:
   SUBROUTINE

   LDA   #$05
   STA   __local_main__mbox
   LDA   #$00
   STA   __local_main__mbox+1
   LDX   #>__local_main__mbox
   LDA   #<__local_main__mbox
   JSR   f0
   LDA   #$00
   ROL
   STA   __local_main__6
   LDA   g1
   BMI   .sx_neg@39
   LDA   #$00
   BEQ   .sx_done@40
.sx_neg@39:
   LDA   #$FF
.sx_done@40:
   STA   __local_main__2
   STA   __local_main__1
   STA   __local_main__0
   LDA   g0
   EOR   g1
   STA   __local_main__3
   LDA   g0+1
   EOR   __local_main__2
   STA   __local_main__2
   LDA   g0+2
   EOR   __local_main__1
   STA   __local_main__5
   LDA   g0+3
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
   LDA   __local_main__6
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

g0:
   DC.L  $000000B3

__zp_init:
   SUBROUTINE

   LDA   #$00
   STA   g2+1
   LDA   #$2D
   STA   g1
   LDA   #$39
   STA   g2
   RTS
