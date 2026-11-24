SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
g0	EQU	$26
__local_f3__0	EQU	$80
__local_f3__1	EQU	$81
__local_f3__2	EQU	$82
__local_f3__3	EQU	$83

; @zp-link-meta-begin
; def f0 params= locals= indirect=true in_cycle=false
; def f1 params= locals= indirect=true in_cycle=false
; def f2 params= locals= indirect=true in_cycle=false
; def f3 params= locals=__local_f3__0,__local_f3__1,__local_f3__2,__local_f3__3 indirect=false in_cycle=false
; def main params= locals= indirect=false in_cycle=false
; call f0 -> f1
; call main -> f0
; item f3 section=.text local=false zero=false
; item f2 section=.text local=false zero=false
; item f1 section=.text local=false zero=false
; item f0 section=.text local=false zero=false
; item main section=.text local=false zero=false
; item __zp_init section=.text local=false zero=false
; zp g0 size=1 at=auto def=true init=$9C weight=6
; runtime hargs_size=32 hargs_need=22
; @zp-link-meta-end

f3:
   SUBROUTINE

   ; prologue: 15 arg bytes, 0 local bytes
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
   BMI   .sx_neg@0
   LDA   #$00
   BEQ   .sx_done@1
.sx_neg@0:
   LDA   #$FF
.sx_done@1:
   STA   __local_f3__2
   STA   __local_f3__1
   STA   __local_f3__0
   LDY   #$03
   LDA   (FP),Y
   STA   __local_f3__3
   LDA   __local_f3__2
   AND   #$3F
   STA   __local_f3__0
   LDA   g0
   BEQ   .and_false@0
   LDA   __local_f3__3
   ORA   __local_f3__2
   ORA   __local_f3__1
   ORA   __local_f3__0
   BNE   .and_end@1
.and_false@0:
.and_end@1:
   LDY   #$10
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   STA   __local_f3__0
   LDY   #$10
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$01
   LDA   (DPTR),Y
   STA   __local_f3__1
   LDA   __local_f3__0
   CLC
   ADC   #$64
   STA   __local_f3__0
   BCC   .carry_inc@0
   INC   __local_f3__1
.carry_inc@0:
   LDY   #$10
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDA   __local_f3__0
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$10
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDA   __local_f3__1
   LDY   #$01
   STA   (DPTR),Y
   LDY   #$03
   LDA   (FP),Y

   ; epilogue
   PHA
   CLC
   LDA   FP
   ADC   #$11
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
   RTS

f2:
   SUBROUTINE

   ; prologue: 15 arg bytes, 7 local bytes, 5 callee-saved bytes
   SEC
   LDA   SSP
   SBC   #$09
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$08
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

   LDY   #$0F
   LDA   (FP),Y
   ORA   (FP),Y
   STA   $C4
   LDY   #$17
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   STA   $C1
   LDY   #$17
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$01
   LDA   (DPTR),Y
   STA   $C0
   LDA   $C4
   BMI   .sx_neg@2
   LDA   #$00
   BEQ   .sx_done@3
.sx_neg@2:
   LDA   #$FF
.sx_done@3:
   STA   $80
   LDA   $C4
   ORA   #$01
   STA   $81
   LDA   $80
   BMI   .sx_neg@4
   LDA   #$00
   BEQ   .sx_done@5
.sx_neg@4:
   LDA   #$FF
.sx_done@5:
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
   STA   $81
   LDA   HARGS+13
   STA   $80
   LDA   $C1
   CLC
   ADC   $81
   STA   $81
   LDA   $C0
   ADC   $80
   STA   $80
   LDY   #$17
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDA   $81
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$17
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDA   $80
   LDY   #$01
   STA   (DPTR),Y
   LDA   $C4
   LDY   #$06
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDA   #<f3
   STA   $C2
   LDA   #>f3
   STA   $C3
   LDY   #$0F
   LDA   (FP),Y
   AND   #$07
   TAX
   INY
   LDA   (FP),Y
   INY
   LDA   (FP),Y
   INY
   LDA   (FP),Y
   LDA   #$E8
   STA   $C0
   LDA   #$03
   STA   $C1
   CPX   #$00
   BEQ   .shift_done@7
.shift_loop@6:
   LDA   $C1
   ASL
   ROR   $C1
   ROR   $C0
   DEX
   BNE   .shift_loop@6
.shift_done@7:
   LDA   $C4
   ORA   #$01
   STA   HARGS+4
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
   LDA   #$00
   STA   HARGS+5
   LDY   #$00
   STY   HARGS+6
   STA   HARGS+7
   JSR   sdivmod32
   LDA   HARGS+8
   STA   $8B
   LDA   HARGS+9
   STA   $8A
   LDA   HARGS+10
   STA   $89
   LDA   HARGS+11
   STA   $88
   LDA   $C4
   STA   HARGS
   LDA   #$00
   LDX   #$08
   LSR   HARGS
.mul_loop@8:
   BCC   .mul_skip@9
   CLC
.mul_skip@9:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@8
   STA   $87
   LDA   HARGS
   STA   $86
   LDA   $C4
   BMI   .sx_neg@10
   LDA   #$00
   BEQ   .sx_done@11
.sx_neg@10:
   LDA   #$FF
.sx_done@11:
   STA   $82
   STA   $81
   STA   $80
   LDA   $C4
   SEC
   LDY   #$0F
   SBC   (FP),Y
   STA   $85
   LDA   $82
   INY
   SBC   (FP),Y
   STA   $84
   LDA   $81
   INY
   SBC   (FP),Y
   STA   $83
   LDA   $80
   INY
   SBC   (FP),Y
   STA   $82
   CLC
   LDA   FP
   ADC   #$06
   STA   $80
   LDA   FP+1
   ADC   #$00
   STA   $81
   SEC
   LDA   SSP
   SBC   #$0F
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   $C0
   LDY   #$01
   STA   (SSP),Y
   LDA   $8B
   INY
   STA   (SSP),Y
   LDA   $8A
   INY
   STA   (SSP),Y
   LDA   $89
   INY
   STA   (SSP),Y
   LDA   $88
   INY
   STA   (SSP),Y
   LDA   $86
   INY
   STA   (SSP),Y
   LDA   $87
   INY
   STA   (SSP),Y
   LDA   #$00
   INY
   STA   (SSP),Y
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
   STA   $80
   LDA   $C4
   EOR   $80
   STA   $81
   LDY   #$06
   LDA   (FP),Y
   CLC
   ADC   $81
   CLC
   ADC   g0
   STA   g0
   LDY   #$0F
   LDA   (FP),Y
   SEC
   SBC   $C4
   STA   $80
   LDY   #$0B
   LDA   (FP),Y
   ORA   $80

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
   ADC   #$18
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$08
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   PLA
   RTS

f1:
   SUBROUTINE

   ; prologue: 6 arg bytes, 9 local bytes, 7 callee-saved bytes
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

   LDY   #$0C
   LDA   (FP),Y
   AND   (FP),Y
   STA   $80
   LDA   g0
   STA   $C6
   BMI   .sx_neg@12
   LDA   #$00
   BEQ   .sx_done@13
.sx_neg@12:
   LDA   #$FF
.sx_done@13:
   STA   $C5
   STA   $C4
   STA   $C3
   LDA   $80
   LDY   #$08
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDA   #<f3
   STA   $C1
   LDA   #>f3
   STA   $C2
   LDA   $80
   ORA   #$01
   STA   HARGS+1
   LDA   #$03
   STA   HARGS
   JSR   udivmod8
   LDA   HARGS+3
   STA   $C0
   LDA   $C6
   ORA   #$01
   STA   HARGS+4
   LDA   $C5
   STA   HARGS+5
   LDA   $C4
   STA   HARGS+6
   LDA   $C3
   STA   HARGS+7
   LDA   #$00
   STA   HARGS
   LDY   #$00
   STY   HARGS+1
   STY   HARGS+2
   STA   HARGS+3
   JSR   udivmod32
   LDA   HARGS+8
   STA   $85
   LDA   HARGS+9
   STA   $84
   LDA   HARGS+10
   STA   $83
   LDA   HARGS+11
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
   SBC   #$0F
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDY   #$0C
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$0E
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$0F
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$10
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   LDY   #$11
   LDA   (FP),Y
   LDY   #$05
   STA   (SSP),Y
   LDA   $C0
   INY
   STA   (SSP),Y
   LDA   #$00
   INY
   STA   (SSP),Y
   INY
   STA   (SSP),Y
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
   LDA   $80
   INY
   STA   (SSP),Y
   LDA   $81
   INY
   STA   (SSP),Y
   LDA   $C1
   STA   DPTR
   LDA   $C2
   STA   DPTR+1
   JSR   icall
   LDA   #$07
   CLC
   LDY   #$0E
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
   CLC
   LDA   FP
   ADC   #$11
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

   ; prologue: 3 arg bytes, 6 local bytes, 4 callee-saved bytes
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

   LDY   #$0A
   LDA   (FP),Y
   CLC
   DEY
   SBC   (FP),Y
   LDY   #$0B
   LDA   (FP),Y
   SBC   #$00
   LDA   #$00
   ROL
   STA   $80
   LDY   #$09
   LDA   (FP),Y
   EOR   $80
   STA   $C3
   LDA   g0
   STA   $C2
   BMI   .sx_neg@14
   LDA   #$00
   BEQ   .sx_done@15
.sx_neg@14:
   LDA   #$FF
.sx_done@15:
   STA   $C1
   ORA   $C2
   BEQ   .and_false@2
   LDY   #$0A
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@2
   LDA   #$01
   STA   $81
   JMP   .and_end@3
.and_false@2:
   LDA   #$00
   STA   $81
.and_end@3:
   LDY   #$09
   LDA   (FP),Y
   CLC
   INY
   SBC   (FP),Y
   LDA   #$00
   INY
   SBC   (FP),Y
   LDA   #$00
   ROL
   AND   #$07
   TAX
   CPX   #$00
   BEQ   .shift_done@17
.shift_loop@16:
   LSR   $81
   DEX
   BNE   .shift_loop@16
.shift_done@17:
   LDA   $81
   STA   $C0
   LDA   $C3
   AND   #$02
   STA   $84
   LDA   $C0
   BNE   .or_true@4
   LDY   #$0A
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@4
   LDA   #$00
   STA   $83
   JMP   .or_end@5
.or_true@4:
   LDA   #$01
   STA   $83
   LDA   #$00
.or_end@5:
   ORA   #$00
   BPL   .sx_done@19
   LDA   #$FF
.sx_done@19:
   STA   $81
   LDY   #$00
   STY   $82
   AND   #$3F
   STA   $80
   SEC
   LDA   SSP
   SBC   #$06
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   $84
   INY
   STA   (SSP),Y
   LDA   #$00
   INY
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
   JSR   f1
   LDA   $C3
   LDY   #$05
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDA   #<f3
   STA   $85
   LDA   #>f3
   STA   $86
   LDY   #$0A
   LDA   (FP),Y
   SEC
   DEY
   SBC   (FP),Y
   LDY   #$0B
   LDA   (FP),Y
   SBC   #$00
   LDA   #$00
   ROL
   STA   $84
   LDA   #$00
   BPL   .sx_done@21
   LDA   #$FF
.sx_done@21:
   STA   $83
   STA   $82
   CLC
   LDA   FP
   ADC   #$05
   STA   $80
   LDA   FP+1
   ADC   #$00
   STA   $81
   SEC
   LDA   SSP
   SBC   #$0F
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   #$FF
   LDY   #$01
   STA   (SSP),Y
   LDA   #$07
   INY
   STA   (SSP),Y
   LDA   #$00
   INY
   STA   (SSP),Y
   INY
   STA   (SSP),Y
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
   INY
   STA   (SSP),Y
   LDA   $84
   INY
   STA   (SSP),Y
   LDA   #$00
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
   LDA   $85
   STA   DPTR
   LDA   $86
   STA   DPTR+1
   JSR   icall
   LDY   #$09
   LDA   (FP),Y
   ORA   #$01
   STA   HARGS+2
   LDA   $C2
   STA   HARGS
   LDA   $C1
   STA   HARGS+1
   LDA   #$00
   STA   HARGS+3
   JSR   sdivmod16
   LDA   HARGS+4
   STA   $81
   LDA   HARGS+5
   STA   $80
   LDA   $81
   ORA   #$01
   STA   $83
   LDA   $80
   STA   $82
   BMI   .sx_neg@22
   LDA   #$00
   BEQ   .sx_done@23
.sx_neg@22:
   LDA   #$FF
.sx_done@23:
   STA   $81
   STA   $80
   LDA   $83
   STA   HARGS+4
   LDA   $81
   STA   HARGS+6
   AND   #$3F
   STA   HARGS+7
   LDA   $C0
   STA   HARGS
   LDA   #$00
   STA   HARGS+1
   LDY   #$00
   STY   HARGS+2
   STA   HARGS+3
   JSR   udivmod32
   LDA   HARGS+12

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
   CLC
   LDA   FP
   ADC   #$0B
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$07
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
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
   SBC   #$03
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   #$E8
   DEY
   STA   (SSP),Y
   LDA   #$28
   INY
   STA   (SSP),Y
   LDA   #$00
   INY
   STA   (SSP),Y
   JSR   f0
   STA   $84
   LDA   g0
   STA   $83
   BMI   .sx_neg@24
   LDA   #$00
   BEQ   .sx_done@25
.sx_neg@24:
   LDA   #$FF
.sx_done@25:
   STA   $82
   STA   $81
   STA   $80
   LDA   $84
   EOR   $83
   STA   $C1
   LDA   #$00
   EOR   $82
   STA   $C0
   LDA   #$00
   EOR   $81
   STA   HARGS+2
   LDA   #$00
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

   LDA   #$9C
   STA   g0
   RTS
