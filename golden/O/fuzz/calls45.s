SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
g1	EQU	$26
g0	EQU	$27
__local_f4__0	EQU	$80
__local_main__0	EQU	$80
__local_f4__1	EQU	$81
__local_main__1	EQU	$81
__local_f4__2	EQU	$82
__local_main__2	EQU	$82
__local_f4__3	EQU	$83
__local_main__3	EQU	$83
__local_f4__4	EQU	$84
__local_f4__5	EQU	$85
__local_main__6	EQU	$86
__local_main__7	EQU	$87
__local_main__8	EQU	$88
__local_main__9	EQU	$89
__local_f0__0	EQU	$8C
__local_f0__1	EQU	$8D
__local_f0__2	EQU	$8E
__local_f0__3	EQU	$8F
__local_f0__box2	EQU	$90
__local_f2__0	EQU	$92
__local_rec__0	EQU	$92
__local_f2__1	EQU	$93
__local_rec__1	EQU	$93
__local_f2__2	EQU	$94
__local_rec__2	EQU	$94
__local_f2__3	EQU	$95
__local_rec__3	EQU	$95
__local_f2__pp	EQU	$96
__local_rec__4	EQU	$96
__local_f2__4	EQU	$97

; @zp-link-meta-begin
; def f0 params= locals=__local_f0__0,__local_f0__1,__local_f0__2,__local_f0__3,__local_f0__box2,__local_f0__5 indirect=false in_cycle=false
; def f1 params= locals= indirect=true in_cycle=false
; def f2 params=__zpabi_f2__pp_0,__zpabi_f2__pp_1 locals=__local_f2__0,__local_f2__1,__local_f2__2,__local_f2__3,__local_f2__pp,__local_f2__4 indirect=false in_cycle=false param_regs=A,X
; def f3 params= locals= indirect=true in_cycle=false
; def f4 params= locals=__local_f4__0,__local_f4__1,__local_f4__2,__local_f4__3,__local_f4__4,__local_f4__5 indirect=false in_cycle=false
; def main params= locals=__local_main__0,__local_main__1,__local_main__2,__local_main__3,__local_main__4,__local_main__5,__local_main__6,__local_main__7,__local_main__8,__local_main__9,__local_main__10,__local_main__11 indirect=false in_cycle=false
; def rec params= locals=__local_rec__0,__local_rec__1,__local_rec__2,__local_rec__3,__local_rec__4 indirect=false in_cycle=true
; call f0 -> f2
; call f0 -> rec
; call f1 -> f2
; call f1 -> rec
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
; zp g1 size=1 at=auto def=true init=$4D weight=13
; zp g0 size=2 at=auto def=true init=$41,$00 weight=13
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

   ; prologue: 9 arg bytes, 0 local bytes
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
   BEQ   .and_false@1
   LDA   g1
   BNE   .and_end@2
.and_false@1:
.and_end@2:
   LDY   #$08
   LDA   (FP),Y
   SEC
   LDY   #$04
   SBC   (FP),Y
   LDY   #$09
   LDA   (FP),Y
   LDY   #$05
   SBC   (FP),Y
   LDA   #$00
   INY
   SBC   (FP),Y
   LDA   #$00
   INY
   SBC   (FP),Y
   BVC   .cmp_novf@1
   EOR   #$80
.cmp_novf@1:
   ASL
   LDA   #$00
   ROL
   STA   __local_f4__0
   AND   #$07
   TAX
   LDA   #$01
   STA   __local_f4__4
   LDA   #$00
   STA   __local_f4__5
   CPX   #$00
   BEQ   .shift_done@3
.shift_loop@2:
   ASL   __local_f4__4
   ROL   __local_f4__5
   DEX
   BNE   .shift_loop@2
.shift_done@3:
   LDY   #$0A
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   STA   __local_f4__3
   LDY   #$0A
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$01
   LDA   (DPTR),Y
   STA   __local_f4__2
   LDA   g0
   CLC
   LDY   #$08
   ADC   (FP),Y
   STA   __local_f4__1
   LDA   g0+1
   INY
   ADC   (FP),Y
   STA   __local_f4__0
   LDA   __local_f4__3
   CLC
   ADC   __local_f4__1
   STA   __local_f4__1
   LDA   __local_f4__2
   ADC   __local_f4__0
   STA   __local_f4__0
   INY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDA   __local_f4__1
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$0A
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDA   __local_f4__0
   LDY   #$01
   STA   (DPTR),Y
   LDA   __local_f4__5
   LSR
   LDA   __local_f4__4
   ROR
   BMI   .sx_neg@4
   LDA   #$00
   BEQ   .sx_done@5
.sx_neg@4:
   LDA   #$FF
.sx_done@5:
   ASL
   LDA   #$00
   ROL
   EOR   #$01
   STA   __local_f4__0
   LDA   #$00
   BPL   .sx_done@7
   LDA   #$FF
.sx_done@7:
   STA   HARGS+10
   STA   HARGS+11
   LDA   __local_f4__0
   STA   HARGS+8
   LDA   #$00
   STA   HARGS+9

   ; epilogue
   CLC
   LDA   FP
   ADC   #$0B
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
   RTS

f3:
   SUBROUTINE

   ; prologue: 1 arg bytes, 2 local bytes
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

   INY
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDA   #<f4
   STA   $84
   LDA   #>f4
   STA   $85
   LDA   g0
   STA   $83
   LDA   g0+1
   STA   $82
   CLC
   LDA   FP
   ADC   #$01
   STA   $80
   LDA   FP+1
   ADC   #$00
   STA   $81
   SEC
   LDA   SSP
   SBC   #$09
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDY   #$05
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   INY
   STA   (SSP),Y
   LDA   #$00
   INY
   STA   (SSP),Y
   INY
   STA   (SSP),Y
   INY
   STA   (SSP),Y
   LDA   $83
   INY
   STA   (SSP),Y
   LDA   $82
   INY
   STA   (SSP),Y
   LDA   $80
   INY
   STA   (SSP),Y
   LDA   $81
   INY
   STA   (SSP),Y
   LDA   $84
   STA   DPTR
   LDA   $85
   STA   DPTR+1
   JSR   icall
   LDA   g0
   STA   $81
   LDA   g0+1
   STA   $80
   LDY   #$05
   LDA   (FP),Y
   STA   HARGS
   LDA   #$00
   STA   HARGS+1
   LDY   #$00
   STY   HARGS+2
   STA   HARGS+3
   LDA   $81
   STA   HARGS+4
   LDA   $80
   STA   HARGS+5
   TYA
   STA   HARGS+6
   STA   HARGS+7
   JSR   mul32

   ; epilogue
   CLC
   LDA   FP
   ADC   #$05
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$03
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   RTS

f2:
   SUBROUTINE

   STA   __local_f2__pp
   STX   __local_f2__4
   LDY   #$00
   LDA   (__local_f2__pp),Y
   STA   __local_f2__3
   INY
   LDA   (__local_f2__pp),Y
   STA   __local_f2__2
   LDA   g1
   BMI   .sx_neg@8
   LDA   #$00
   BEQ   .sx_done@9
.sx_neg@8:
   LDA   #$FF
.sx_done@9:
   STA   __local_f2__0
   LDA   __local_f2__3
   CLC
   ADC   g1
   STA   __local_f2__1
   LDA   __local_f2__2
   ADC   __local_f2__0
   LDY   #$01
   STA   (__local_f2__pp),Y
   LDA   __local_f2__1
   DEY
   STA   (__local_f2__pp),Y
   LDA   g0
   STA   HARGS
   LDA   g0+1
   STA   HARGS+1
   LDA   #$65
   STA   HARGS+2
   LDA   #$00
   STA   HARGS+3
   JSR   udivmod16
   LDA   g1
   BEQ   .and_false@5
   LDY   #$01
   LDA   (__local_f2__pp),Y
   DEY
   ORA   (__local_f2__pp),Y
   BEQ   .and_false@5
   LDA   #$01
   STA   __local_f2__1
   JMP   .and_end@6
.and_false@5:
   LDA   #$00
   STA   __local_f2__1
.and_end@6:
   LDA   HARGS+7
   BNE   .thread@0
   LDA   HARGS+6
   CMP   __local_f2__1
   BEQ   .cmp_true@10
.thread@0:
   LDA   #$00
   BEQ   .cmp_end@11
.cmp_true@10:
   LDA   #$01
.cmp_end@11:
   STA   __local_f2__0
   LDA   #$00
   BPL   .sx_done@14
   LDA   #$FF
.sx_done@14:
   STA   HARGS+10
   STA   HARGS+11
   LDA   __local_f2__0
   STA   HARGS+8
   LDA   #$00
   STA   HARGS+9
   RTS

f1:
   SUBROUTINE

   ; prologue: 10 arg bytes, 9 local bytes, 5 callee-saved bytes
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

   LDA   g1
   STA   $81
   BMI   .sx_neg@15
   LDA   #$00
   BEQ   .sx_done@16
.sx_neg@15:
   LDA   #$FF
.sx_done@16:
   STA   $80
   LDA   $81
   ORA   #$01
   STA   $81
   LDA   $80
   BMI   .sx_neg@17
   LDA   #$00
   BEQ   .sx_done@18
.sx_neg@17:
   LDA   #$FF
.sx_done@18:
   STA   HARGS+6
   STA   HARGS+7
   LDY   #$10
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
   JSR   udivmod32
   LDA   HARGS+12
   STA   $82
   LDA   g0
   STA   $81
   LDA   g0+1
   STA   $80
   LDA   $81
   CLC
   LDY   #$0C
   SBC   (FP),Y
   LDA   $80
   INY
   SBC   (FP),Y
   LDA   #$00
   INY
   SBC   (FP),Y
   LDA   #$00
   INY
   SBC   (FP),Y
   LDA   #$00
   ROL
   STA   $80
   LDA   $82
   SEC
   SBC   $80
   STA   $C0
   LDY   #$14
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   STA   $83
   LDY   #$14
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$01
   LDA   (DPTR),Y
   STA   $82
   LDA   g1
   STA   $81
   BMI   .sx_neg@19
   LDA   #$00
   BEQ   .sx_done@20
.sx_neg@19:
   LDA   #$FF
.sx_done@20:
   STA   $80
   LDA   $83
   CLC
   ADC   $81
   STA   $81
   LDA   $82
   ADC   $80
   STA   $80
   LDY   #$14
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDA   $81
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$14
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDA   $80
   LDY   #$01
   STA   (DPTR),Y
   LDA   $C0
   LDY   #$06
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@21
   LDA   #$00
   BEQ   .sx_done@22
.sx_neg@21:
   LDA   #$FF
.sx_done@22:
   LDY   #$07
   STA   (FP),Y
   CLC
   LDA   FP
   ADC   #$06
   STA   $80
   LDA   FP+1
   ADC   #$00
   STA   $81
   LDX   $81
   LDA   $80
   JSR   f2
   LDA   $C0
   EOR   HARGS+8
   STA   $81
   LDY   #$06
   LDA   (FP),Y
   STA   $80
   CLC
   ADC   $81
   STA   $C4
   BMI   .sx_neg@23
   LDA   #$00
   BEQ   .sx_done@24
.sx_neg@23:
   LDA   #$FF
.sx_done@24:
   STA   $C0
   LDA   g1
   CLC
   ADC   $C4
   STA   g1
   LDA   $C4
   LDY   #$08
   STA   (FP),Y
   LDA   $C0
   INY
   STA   (FP),Y
   LDA   #<f4
   STA   $C2
   LDA   #>f4
   STA   $C3
   LDY   #$0C
   LDA   (FP),Y
   AND   #$07
   STA   $C1
   INY
   LDA   (FP),Y
   INY
   LDA   (FP),Y
   INY
   LDA   (FP),Y
   INY
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
   LDA   $C1
   STA   HARGS+4
   JSR   lsr32
   LDA   HARGS+5
   STA   $87
   LDA   $C4
   AND   $C4
   STA   $86
   LDA   $C0
   AND   $C0
   STA   $85
   BMI   .sx_neg@25
   LDA   #$00
   BEQ   .sx_done@26
.sx_neg@25:
   LDA   #$FF
.sx_done@26:
   STA   $84
   STA   $83
   LDA   #$01
   SEC
   LDY   #$10
   SBC   (FP),Y
   LDA   #$00
   INY
   SBC   (FP),Y
   LDA   #$00
   INY
   SBC   (FP),Y
   LDA   #$00
   INY
   SBC   (FP),Y
   LDA   #$00
   ROL
   STA   $82
   CLC
   LDA   FP
   ADC   #$08
   STA   $80
   LDA   FP+1
   ADC   #$00
   STA   $81
   SEC
   LDA   SSP
   SBC   #$09
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   $87
   LDY   #$01
   STA   (SSP),Y
   LDA   $86
   INY
   STA   (SSP),Y
   LDA   $85
   INY
   STA   (SSP),Y
   LDA   $84
   INY
   STA   (SSP),Y
   LDA   $83
   INY
   STA   (SSP),Y
   LDA   $82
   INY
   STA   (SSP),Y
   LDA   #$00
   INY
   STA   (SSP),Y
   LDA   $80
   INY
   STA   (SSP),Y
   LDA   $81
   INY
   STA   (SSP),Y
   LDA   $C2
   STA   DPTR
   LDA   $C3
   STA   DPTR+1
   JSR   icall
   LDA   $C4
   EOR   HARGS+8
   STA   $81
   LDY   #$08
   LDA   (FP),Y
   CLC
   ADC   $81
   STA   $C0
   LDA   g1
   STA   $81
   BMI   .sx_neg@27
   LDA   #$00
   BEQ   .sx_done@28
.sx_neg@27:
   LDA   #$FF
.sx_done@28:
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
   CLC
   ADC   $C0
   STA   $83
   BMI   .sx_neg@29
   LDA   #$00
   BEQ   .sx_done@30
.sx_neg@29:
   LDA   #$FF
.sx_done@30:
   STA   $82
   LDY   #$14
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   STA   $81
   LDY   #$14
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$01
   LDA   (DPTR),Y
   STA   $80
   LDA   $81
   SEC
   SBC   $83
   LDA   $80
   SBC   $82
   BVC   .cmp_novf@31
   EOR   #$80
.cmp_novf@31:
   ASL
   LDA   #$00
   ROL
   EOR   #$01
   LDX   $C1
   BEQ   .shift_done@33
.shift_loop@32:
   ASL
   DEX
   BNE   .shift_loop@32
.shift_done@33:

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
   CLC
   LDA   FP
   ADC   #$15
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$0A
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   PLA
   RTS

f0:
   SUBROUTINE

   LDA   g0
   STA   __local_f0__3
   STA   __local_f0__box2
   LDA   g0+1
   STA   __local_f0__2
   STA   __local_f0__box2+1
   LDX   #>__local_f0__box2
   LDA   #<__local_f0__box2
   JSR   f2
   LDA   __local_f0__3
   EOR   HARGS+8
   STA   __local_f0__1
   LDA   __local_f0__2
   EOR   HARGS+9
   STA   __local_f0__0
   LDA   __local_f0__1
   CLC
   ADC   __local_f0__box2
   STA   __local_f0__1
   LDA   __local_f0__0
   ADC   __local_f0__box2+1
   STA   __local_f0__0
   LDA   g0
   CLC
   ADC   __local_f0__1
   STA   g0
   LDA   g0+1
   ADC   __local_f0__0
   STA   g0+1
   LDA   g1
   STA   __local_f0__1
   BMI   .sx_neg@34
   LDA   #$00
   BEQ   .sx_done@35
.sx_neg@34:
   LDA   #$FF
.sx_done@35:
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
   LDA   g1
   RTS

main:
   SUBROUTINE

   JSR   f0
   STA   __local_main__9
   ORA   #$00
   BMI   .sx_neg@36
   LDA   #$00
   BEQ   .sx_done@37
.sx_neg@36:
   LDA   #$FF
.sx_done@37:
   STA   __local_main__8
   STA   __local_main__7
   STA   __local_main__6
   LDA   g1
   BMI   .sx_neg@38
   LDA   #$00
   BEQ   .sx_done@39
.sx_neg@38:
   LDA   #$FF
.sx_done@39:
   STA   __local_main__2
   STA   __local_main__1
   STA   __local_main__0
   LDA   g0
   EOR   g1
   STA   __local_main__3
   LDA   g0+1
   EOR   __local_main__2
   STA   __local_main__2
   LDA   #$00
   EOR   __local_main__1
   STA   __local_main__1
   LDA   #$00
   EOR   __local_main__0
   STA   __local_main__0
   LDA   __local_main__9
   EOR   __local_main__3
   STA   __local_main__3
   LDA   __local_main__8
   EOR   __local_main__2
   STA   __local_main__2
   LDA   __local_main__7
   EOR   __local_main__1
   STA   HARGS+2
   LDA   __local_main__6
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
   STA   g0+1
   LDA   #$41
   STA   g0
   LDA   #$4D
   STA   g1
   RTS
