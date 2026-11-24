SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
g0	EQU	$26

; @zp-link-meta-begin
; def f0 params= locals= indirect=false in_cycle=false
; def f1 params= locals= indirect=true in_cycle=false
; def f2 params= locals= indirect=true in_cycle=false
; def f3 params= locals= indirect=true in_cycle=false
; def f4 params= locals= indirect=false in_cycle=false
; def main params= locals= indirect=false in_cycle=false
; def rec params= locals= indirect=false in_cycle=true
; call f0 -> f1
; call f0 -> f2
; call f1 -> f3
; call f2 -> f3
; call f3 -> rec
; call f4 -> rec
; call main -> f0
; call rec -> rec
; item rec section=.text local=false zero=false
; item f4 section=.text local=false zero=false
; item f3 section=.text local=false zero=false
; item f2 section=.text local=false zero=false
; item f1 section=.text local=false zero=false
; item f0 section=.text local=false zero=false
; item main section=.text local=false zero=false
; item __zp_init section=.text local=false zero=false
; zp g0 size=1 at=auto def=true init=$97 weight=42
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

   LDY   #$04
   LDA   (FP),Y
   BMI   .jcmp_neg@0
   DEY
   ORA   (FP),Y
   BNE   .if_end@0
.jcmp_neg@0:
   LDY   #$05
   LDA   (FP),Y
   STA   $80
   INY
   LDA   (FP),Y
   LDX   #$00
   LDA   $80

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
   STA   $82
   INY
   LDA   (FP),Y
   ROL
   STA   $83
   LDA   $82
   CLC
   DEY
   ADC   (FP),Y
   STA   $81
   LDA   $83
   INY
   ADC   (FP),Y
   STA   $80
   LDA   $81
   CLC
   LDY   #$03
   ADC   (FP),Y
   STA   $81
   LDA   $80
   INY
   ADC   (FP),Y
   STA   $80
   DEY
   LDA   (FP),Y
   SEC
   SBC   #$01
   STA   $83
   INY
   LDA   (FP),Y
   SBC   #$00
   STA   $82
   LDA   $80
   AND   #$03
   STA   $80
   SEC
   LDA   SSP
   SBC   #$04
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   $83
   LDY   #$01
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
   JSR   rec
   STA   $80
   INC   $80
   BNE   .inc_done@0
   INX
.inc_done@0:
   LDA   $80

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

   ; prologue: 2 arg bytes, 4 local bytes, 4 callee-saved bytes
   SEC
   LDA   SSP
   SBC   #$06
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$05
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

   LDA   g0
   STA   $82
   BMI   .sx_neg@1
   LDA   #$00
   BEQ   .sx_done@2
.sx_neg@1:
   LDA   #$FF
.sx_done@2:
   STA   $81
   LDY   #$07
   LDA   (FP),Y
   BMI   .sx_neg@3
   LDA   #$00
   BEQ   .sx_done@4
.sx_neg@3:
   LDA   #$FF
.sx_done@4:
   STA   $80
   LDA   $82
   SEC
   LDY   #$07
   SBC   (FP),Y
   LDA   $81
   SBC   $80
   BVC   .cmp_novf@5
   EOR   #$80
.cmp_novf@5:
   ASL
   LDA   #$00
   ROL
   EOR   #$01
   STA   $C3
   LDA   #$00
   BPL   .sx_done@7
   LDA   #$FF
.sx_done@7:
   STA   $83
   STA   $82
   LDY   #$07
   LDA   (FP),Y
   BMI   .sx_neg@8
   LDA   #$00
   BEQ   .sx_done@9
.sx_neg@8:
   LDA   #$FF
.sx_done@9:
   STA   $C2
   STA   $C1
   STA   $C0
   LDA   g0
   STA   $81
   BMI   .sx_neg@10
   LDA   #$00
   BEQ   .sx_done@11
.sx_neg@10:
   LDA   #$FF
.sx_done@11:
   STA   $80
   LDA   $81
   ORA   #$01
   STA   $81
   LDA   $80
   BMI   .sx_neg@12
   LDA   #$00
   BEQ   .sx_done@13
.sx_neg@12:
   LDA   #$FF
.sx_done@13:
   STA   HARGS+6
   STA   HARGS+7
   LDA   $C3
   STA   HARGS
   LDA   #$00
   STA   HARGS+1
   LDA   $83
   STA   HARGS+2
   STA   HARGS+3
   LDA   $81
   STA   HARGS+4
   LDA   $80
   STA   HARGS+5
   JSR   udivmod32
   LDA   HARGS+12
   STA   $83
   LDA   HARGS+13
   STA   $82
   LDA   HARGS+14
   STA   $81
   LDA   HARGS+15
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
   LDY   #$07
   LDA   (FP),Y
   STA   HARGS
   LDA   $C2
   STA   HARGS+1
   LDA   $C1
   STA   HARGS+2
   LDA   $C0
   STA   HARGS+3
   JSR   udivmod32
   LDA   HARGS+8
   STA   $C1
   BMI   .sx_neg@14
   LDA   #$00
   BEQ   .sx_done@15
.sx_neg@14:
   LDA   #$FF
.sx_done@15:
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
   LDA   $C1
   INY
   STA   (SSP),Y
   LDA   $80
   INY
   STA   (SSP),Y
   JSR   rec
   LDA   #$E8
   CLC
   LDY   #$08
   SBC   (FP),Y
   LDA   #$03
   SBC   #$00
   LDA   #$00
   ROL
   STA   $C0
   LDA   $C1
   BMI   .sx_neg@16
   LDA   #$00
   BEQ   .sx_done@17
.sx_neg@16:
   LDA   #$FF
.sx_done@17:
   STA   HARGS+1
   LDA   $C1
   STA   HARGS
   LDA   $C3
   STA   HARGS+2
   LDA   #$00
   STA   HARGS+3
   JSR   mul16
   LDA   HARGS+4
   STA   $80
   LDA   HARGS+5
   STA   $81
   LDA   $C0
   EOR   $80
   STA   $80
   LDA   #$00
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
   INY
   LDA   (FP),Y
   STA   $C2
   INY
   LDA   (FP),Y
   STA   $C3
   CLC
   LDA   FP
   ADC   #$08
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

f3:
   SUBROUTINE

   ; prologue: 7 arg bytes, 1 local bytes, 1 callee-saved bytes
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

   LDY   #$06
   LDA   (FP),Y
   SEC
   SBC   #$02
   BVC   .jcmp_novf@18
   EOR   #$80
.jcmp_novf@18:
   BPL   .or_true@1
   LDY   #$06
   LDA   (FP),Y
   SEC
   SBC   #$01
   BVC   .jcmp_novf@19
   EOR   #$80
.jcmp_novf@19:
   BMI   .or_true@1
   LDA   #$00
   STA   $81
   JMP   .or_end@2
.or_true@1:
   LDA   #$01
   STA   $81
.or_end@2:
   ORA   #$00
   BNE   .or_true@3
   LDA   #$00
   STA   $80
   JMP   .or_end@4
.or_true@3:
   LDA   #$01
   STA   $80
.or_end@4:
   STA   HARGS
   LDY   #$07
   LDA   (FP),Y
   STA   HARGS+1
   JSR   mul8
   LDA   HARGS+2
   STA   $C0
   LDA   #<f4
   STA   $81
   LDA   #>f4
   STA   $82
   LDA   g0
   STA   $80
   SEC
   LDA   SSP
   SBC   #$02
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   g0
   LDY   #$01
   STA   (SSP),Y
   LDA   $80
   INY
   STA   (SSP),Y
   LDA   $81
   STA   DPTR
   LDA   $82
   STA   DPTR+1
   JSR   icall
   STA   $80
   LDA   $C0
   EOR   $80
   STA   $80
   CLC
   ADC   g0
   STA   g0
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
   LDA   #$01
   INY
   STA   (SSP),Y
   LDA   #$00
   INY
   STA   (SSP),Y
   JSR   rec
   LDA   #$00
   LDY   #$05
   CMP   (FP),Y
   BNE   .cmp_end@23
   LDA   #$02
   DEY
   CMP   (FP),Y
   BEQ   .cmp_true@22
   LDA   #$00
   BEQ   .cmp_end@23
.cmp_true@22:
   LDA   #$01
.cmp_end@23:
   STA   $C0
   LDY   #$04
   LDA   (FP),Y
   STA   HARGS
   INY
   LDA   (FP),Y
   STA   HARGS+1
   LDA   #$07
   STA   HARGS+2
   LDA   #$00
   STA   HARGS+3
   JSR   udivmod16
   LDA   HARGS+4
   STA   $81
   LDA   HARGS+5
   STA   $80
   LDA   $C0
   EOR   $81
   STA   HARGS+8
   LDA   #$00
   EOR   $80
   STA   HARGS+9
   LDA   #$00
   STA   HARGS+10
   STA   HARGS+11

   ; epilogue
   LDY   #$01
   LDA   (FP),Y
   STA   $C0
   CLC
   LDA   FP
   ADC   #$0A
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

f2:
   SUBROUTINE

   ; prologue: 2 arg bytes, 12 local bytes, 12 callee-saved bytes
   SEC
   LDA   SSP
   SBC   #$0E
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$0D
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

   LDA   g0
   STA   $CB
   STA   $83
   BMI   .sx_neg@25
   LDA   #$00
   BEQ   .sx_done@26
.sx_neg@25:
   LDA   #$FF
.sx_done@26:
   STA   $82
   LDA   g0
   STA   $81
   BMI   .sx_neg@27
   LDA   #$00
   BEQ   .sx_done@28
.sx_neg@27:
   LDA   #$FF
.sx_done@28:
   STA   $80
   LDA   $83
   AND   $81
   STA   $C9
   LDA   $82
   AND   $80
   STA   $CA
   LDA   $CB
   EOR   #$FF
   AND   #$07
   TAX
   BEQ   .shift_done@32
.shift_loop@31:
   LDA   $CA
   ASL
   ROR   $CA
   ROR   $C9
   DEX
   BNE   .shift_loop@31
.shift_done@32:
   LDA   $CA
   BMI   .sx_neg@33
   LDA   #$00
   BEQ   .sx_done@34
.sx_neg@33:
   LDA   #$FF
.sx_done@34:
   STA   $C3
   STA   $C2
   LDA   g0
   STA   $C8
   BMI   .sx_neg@35
   LDA   #$00
   BEQ   .sx_done@36
.sx_neg@35:
   LDA   #$FF
.sx_done@36:
   STA   $C7
   BMI   .sx_neg@37
   LDA   #$00
   BEQ   .sx_done@38
.sx_neg@37:
   LDA   #$FF
.sx_done@38:
   STA   $C6
   STA   $C5
   LDY   #$0F
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   STA   $81
   LDY   #$0F
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$01
   LDA   (DPTR),Y
   STA   $80
   LDA   $81
   CLC
   ADC   $C9
   STA   $81
   LDA   $80
   ADC   $CA
   STA   $80
   LDY   #$0F
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDA   $81
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$0F
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDA   $80
   LDY   #$01
   STA   (DPTR),Y
   LDA   $CB
   BMI   .sx_neg@39
   LDA   #$00
   BEQ   .sx_done@40
.sx_neg@39:
   LDA   #$FF
.sx_done@40:
   STA   $C4
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
   BVC   .jcmp_novf@41
   EOR   #$80
.jcmp_novf@41:
   BMI   .lb_skip@1
   JMP   .loop@0_break
.lb_skip@1:
   LDA   $C8
   EOR   $CB
   STA   $84
   LDA   $C7
   EOR   $C4
   STA   $83
   LDY   #$0F
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   STA   $81
   LDY   #$0F
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$01
   LDA   (DPTR),Y
   STA   $80
   LDA   #$02
   SEC
   SBC   $81
   LDA   #$00
   SBC   $80
   BVC   .cmp_novf@42
   EOR   #$80
.cmp_novf@42:
   ASL
   LDA   #$00
   ROL
   STA   $82
   LDA   $C3
   STA   $81
   LDA   $C2
   STA   $80
   SEC
   LDA   SSP
   SBC   #$07
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   $84
   LDY   #$01
   STA   (SSP),Y
   LDA   $83
   INY
   STA   (SSP),Y
   LDA   $82
   INY
   STA   (SSP),Y
   LDA   $C9
   INY
   STA   (SSP),Y
   LDA   $CA
   INY
   STA   (SSP),Y
   LDA   $81
   INY
   STA   (SSP),Y
   LDA   $80
   INY
   STA   (SSP),Y
   JSR   f3
   LDA   HARGS+8
   STA   $83
   LDA   HARGS+9
   STA   $82
   LDA   HARGS+10
   STA   $85
   LDA   HARGS+11
   STA   $84
   LDA   $C0
   BMI   .sx_neg@43
   LDA   #$00
   BEQ   .sx_done@44
.sx_neg@43:
   LDA   #$FF
.sx_done@44:
   STA   $81
   STA   $80
   LDA   $83
   CLC
   ADC   $C1
   STA   $83
   LDA   $82
   ADC   $C0
   STA   $82
   LDA   $85
   ADC   $81
   STA   $81
   LDA   $84
   ADC   $80
   STA   $80
   LDA   $C9
   CLC
   ADC   $83
   STA   $C9
   LDA   $CA
   ADC   $82
   STA   $CA
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
   LDA   g0
   CLC
   ADC   $C9
   STA   g0
   LDA   #<f4
   STA   $80
   LDA   #>f4
   STA   $81
   SEC
   LDA   SSP
   SBC   #$02
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   $C8
   LDY   #$01
   STA   (SSP),Y
   LDA   $CB
   INY
   STA   (SSP),Y
   LDA   $80
   STA   DPTR
   LDA   $81
   STA   DPTR+1
   JSR   icall
   STA   $80
   LDA   $CB
   EOR   $80
   STA   $80
   CLC
   ADC   g0
   STA   g0
   STA   $82
   BMI   .sx_neg@45
   LDA   #$00
   BEQ   .sx_done@46
.sx_neg@45:
   LDA   #$FF
.sx_done@46:
   STA   $83
   LDA   $C8
   AND   #$07
   TAX
   BEQ   .shift_done@48
.shift_loop@47:
   ASL   $82
   ROL   $83
   DEX
   BNE   .shift_loop@47
.shift_done@48:
   LDA   $80
   BMI   .sx_neg@49
   LDA   #$00
   BEQ   .sx_done@50
.sx_neg@49:
   LDA   #$FF
.sx_done@50:
   STA   $81
   LDA   $80
   ORA   $C8
   STA   $80
   LDA   $81
   ORA   $C7
   STA   $81
   LDA   $80
   SEC
   SBC   $82
   STA   $80
   LDA   $81
   SBC   $83
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
   CLC
   LDA   FP
   ADC   #$10
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
   STA   $82
   BMI   .sx_neg@51
   LDA   #$00
   BEQ   .sx_done@52
.sx_neg@51:
   LDA   #$FF
.sx_done@52:
   STA   $81
   LDA   g0
   STA   $80
   BMI   .sx_neg@53
   LDA   #$00
   BEQ   .sx_done@54
.sx_neg@53:
   LDA   #$FF
.sx_done@54:
   STA   HARGS+3
   LDA   $82
   STA   HARGS
   STA   HARGS+2
   LDA   $81
   STA   HARGS+1
   JSR   mul16
   LDA   HARGS+4
   STA   $83
   LDA   HARGS+5
   STA   $82
   LDA   g0
   STA   $81
   BMI   .cmp_true@57
   CMP   #$FF
   BNE   .cmp_true@57
   LDA   #$00
   BEQ   .cmp_end@58
.cmp_true@57:
   LDA   #$01
.cmp_end@58:
   STA   $80
   ORA   #$01
   STA   HARGS+2
   LDA   #$00
   STA   HARGS+3
   LDA   $83
   STA   HARGS
   LDA   $82
   STA   HARGS+1
   JSR   sdivmod16
   LDA   HARGS+6
   STA   $C1
   LDA   HARGS+7
   STA   $C0
   LDY   #$08
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   STA   $83
   LDY   #$08
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$01
   LDA   (DPTR),Y
   STA   $82
   LDA   g0
   BNE   .or_true@7
   LDA   #$00
   STA   $81
   JMP   .or_end@8
.or_true@7:
   LDA   #$01
   STA   $81
.or_end@8:
   LDA   $83
   CLC
   ADC   $81
   STA   $81
   LDA   $82
   ADC   #$00
   STA   $80
   LDY   #$08
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDA   $81
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$08
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDA   $80
   LDY   #$01
   STA   (DPTR),Y
   LDA   #<f4
   STA   $82
   LDA   #>f4
   STA   $83
   LDA   $C1
   SEC
   SBC   $C1
   LDA   $C0
   SBC   $C0
   LDA   #$00
   ROL
   STA   $81
   LDY   #$08
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   STA   $80
   LDY   #$08
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$01
   LDA   (DPTR),Y
   LDA   $C1
   CLC
   ADC   $80
   STA   $80
   SEC
   LDA   SSP
   SBC   #$02
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   $81
   STA   (SSP),Y
   LDA   $80
   INY
   STA   (SSP),Y
   LDA   $82
   STA   DPTR
   LDA   $83
   STA   DPTR+1
   JSR   icall
   STA   $81
   STX   $80
   LDA   $C1
   EOR   $81
   STA   $C4
   LDA   $C0
   EOR   $80
   STA   $C3
   LDA   #$00
   STA   $C2
   LDY   #$00
   STY   $C1
.loop@1_start:
   LDA   $C2
   SEC
   SBC   #$03
   LDA   $C1
   SBC   #$00
   BVC   .jcmp_novf@60
   EOR   #$80
.jcmp_novf@60:
   BMI   .lb_skip@3
   JMP   .loop@1_break
.lb_skip@3:
   LDA   g0
   BEQ   .lnot_true@61
   LDA   #$00
   BEQ   .lnot_end@62
.lnot_true@61:
   LDA   #$01
.lnot_end@62:
   STA   $C0
   LDY   #$08
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   STA   HARGS+1
   LDY   #$08
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$01
   LDA   (DPTR),Y
   LDA   $C4
   STA   HARGS
   JSR   mul8
   LDA   HARGS+2
   STA   $80
   SEC
   LDA   SSP
   SBC   #$07
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   $C0
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
   INY
   STA   (SSP),Y
   INY
   STA   (SSP),Y
   INY
   STA   (SSP),Y
   JSR   f3
   LDA   HARGS+8
   STA   $81
   LDA   HARGS+9
   STA   $80
   LDA   $81
   CLC
   ADC   $C2
   STA   $81
   LDA   $80
   ADC   $C1
   STA   $80
   LDA   $C4
   CLC
   ADC   $81
   STA   $C4
   LDA   $C3
   ADC   $80
   STA   $C3
   INC   $C2
   BEQ   .lb_skip@2
   JMP   .loop@1_start
.lb_skip@2:
   INC   $C1
   JMP   .loop@1_start
.loop@1_break:
   LDY   #$08
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   STA   $81
   LDY   #$08
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$01
   LDA   (DPTR),Y
   STA   $80
   LDA   $81
   ORA   #$01
   STA   HARGS+2
   LDA   $80
   STA   HARGS+3
   STA   HARGS+1
   LDA   $81
   STA   HARGS
   JSR   sdivmod16
   LDA   HARGS+6
   STA   $83
   LDA   HARGS+7
   STA   $82
   LDA   g0
   STA   $81
   BMI   .sx_neg@63
   LDA   #$00
   BEQ   .sx_done@64
.sx_neg@63:
   LDA   #$FF
.sx_done@64:
   STA   $80
   LDA   $81
   ORA   #$01
   STA   HARGS+2
   LDA   $80
   STA   HARGS+3
   LDA   $83
   STA   HARGS
   LDA   $82
   STA   HARGS+1
   JSR   sdivmod16
   LDA   HARGS+6
   LDX   HARGS+7

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
   PLA
   TAX
   PLA
   RTS

f0:
   SUBROUTINE

   ; prologue: 6 arg bytes, 7 local bytes, 3 callee-saved bytes
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

   LDA   g0
   STA   $87
   BMI   .sx_neg@65
   LDA   #$00
   BEQ   .sx_done@66
.sx_neg@65:
   LDA   #$FF
.sx_done@66:
   STA   $86
   BMI   .sx_neg@67
   LDA   #$00
   BEQ   .sx_done@68
.sx_neg@67:
   LDA   #$FF
.sx_done@68:
   STA   $85
   STA   $84
   LDA   g0
   STA   $83
   BMI   .sx_neg@69
   LDA   #$00
   BEQ   .sx_done@70
.sx_neg@69:
   LDA   #$FF
.sx_done@70:
   STA   $82
   BMI   .sx_neg@71
   LDA   #$00
   BEQ   .sx_done@72
.sx_neg@71:
   LDA   #$FF
.sx_done@72:
   STA   $81
   STA   $80
   LDY   #$0A
   LDA   (FP),Y
   CLC
   ADC   $83
   STA   $83
   INY
   LDA   (FP),Y
   ADC   $82
   STA   $82
   INY
   LDA   (FP),Y
   ADC   $81
   STA   $81
   INY
   LDA   (FP),Y
   ADC   $80
   STA   $80
   LDA   $84
   CMP   $80
   BNE   .cmp_true@73
   CMP   $81
   BNE   .cmp_true@73
   LDA   $86
   CMP   $82
   BNE   .cmp_true@73
   LDA   $87
   CMP   $83
   BNE   .cmp_true@73
   LDA   #$00
   BEQ   .cmp_end@74
.cmp_true@73:
   LDA   #$01
.cmp_end@74:
   STA   $C1
   LDA   g0
   STA   $81
   BMI   .sx_neg@76
   LDA   #$00
   BEQ   .sx_done@77
.sx_neg@76:
   LDA   #$FF
.sx_done@77:
   STA   $80
   LDA   $C1
   BMI   .sx_neg@78
   LDA   #$00
   BEQ   .sx_done@79
.sx_neg@78:
   LDA   #$FF
.sx_done@79:
   STA   $C0
   LDA   $C1
   ORA   #$01
   STA   HARGS+2
   LDA   $C0
   STA   HARGS+3
   LDA   $81
   STA   HARGS
   LDA   $80
   STA   HARGS+1
   JSR   sdivmod16
   LDA   HARGS+4
   STA   $C2
   LDY   #$0E
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   STA   $81
   LDY   #$0E
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$01
   LDA   (DPTR),Y
   STA   $80
   LDA   $81
   CLC
   ADC   $C1
   STA   $81
   LDA   $80
   ADC   $C0
   STA   $80
   LDY   #$0E
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDA   $81
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$0E
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDA   $80
   LDY   #$01
   STA   (DPTR),Y
   LDA   $C1
   LDY   #$04
   STA   (FP),Y
   LDA   $C0
   INY
   STA   (FP),Y
   CLC
   LDA   FP
   ADC   #$04
   STA   $80
   LDA   FP+1
   ADC   #$00
   STA   $81
   SEC
   LDA   SSP
   SBC   #$02
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   $80
   LDY   #$01
   STA   (SSP),Y
   LDA   $81
   INY
   STA   (SSP),Y
   JSR   f2
   STA   $80
   LDA   $C1
   EOR   $80
   STA   $81
   LDY   #$04
   LDA   (FP),Y
   CLC
   ADC   $81
   STA   $81
   BMI   .sx_neg@80
   LDA   #$00
   BEQ   .sx_done@81
.sx_neg@80:
   LDA   #$FF
.sx_done@81:
   STA   $80
   LDA   g0
   CLC
   ADC   $81
   STA   g0
   LDA   $81
   LDY   #$06
   STA   (FP),Y
   LDA   $80
   INY
   STA   (FP),Y
   LDA   #$00
   STA   $C1
   LDY   #$00
   STY   $C0
.loop@2_start:
   LDA   $C1
   SEC
   SBC   #$03
   LDA   $C0
   SBC   #$00
   BVC   .jcmp_novf@82
   EOR   #$80
.jcmp_novf@82:
   BPL   .loop@2_break
   CLC
   LDA   FP
   ADC   #$06
   STA   $80
   LDA   FP+1
   ADC   #$00
   STA   $81
   SEC
   LDA   SSP
   SBC   #$02
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   $80
   LDY   #$01
   STA   (SSP),Y
   LDA   $81
   INY
   STA   (SSP),Y
   JSR   f1
   CLC
   ADC   $C1
   CLC
   ADC   $C2
   STA   $C2
   INC   $C1
   BNE   .loop@2_start
   INC   $C0
   JMP   .loop@2_start
.loop@2_break:
   LDY   #$06
   LDA   (FP),Y
   CLC
   ADC   $C2
   STA   $C0
   LDA   g0
   STA   $82
   BMI   .sx_neg@83
   LDA   #$00
   BEQ   .sx_done@84
.sx_neg@83:
   LDA   #$FF
.sx_done@84:
   STA   $81
   LDA   g0
   STA   $80
   BMI   .sx_neg@85
   LDA   #$00
   BEQ   .sx_done@86
.sx_neg@85:
   LDA   #$FF
.sx_done@86:
   STA   HARGS+3
   LDA   $82
   STA   HARGS
   STA   HARGS+2
   LDA   $81
   STA   HARGS+1
   JSR   mul16
   LDA   HARGS+4
   STA   $81
   LDA   HARGS+5
   STA   $80
   LDA   $C0
   BMI   .sx_neg@87
   LDA   #$00
   BEQ   .sx_done@88
.sx_neg@87:
   LDA   #$FF
.sx_done@88:
   ASL   $C0
   ROL
   ASL   $C0
   ROL
   STA   $C1
   LDA   $C0
   SEC
   SBC   $81
   LDA   $C1
   SBC   $80
   BVC   .cmp_novf@89
   EOR   #$80
.cmp_novf@89:
   ASL
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
   CLC
   LDA   FP
   ADC   #$0F
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

main:
   SUBROUTINE

   ; prologue: 0 arg bytes, 4 local bytes, 2 callee-saved bytes
   SEC
   LDA   SSP
   SBC   #$06
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$05
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

   LDA   #$05
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   CLC
   LDA   FP
   ADC   #$03
   STA   $80
   LDA   FP+1
   ADC   #$00
   STA   $81
   SEC
   LDA   SSP
   SBC   #$06
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   #$69
   LDY   #$01
   STA   (SSP),Y
   LDA   #$00
   INY
   STA   (SSP),Y
   INY
   STA   (SSP),Y
   INY
   STA   (SSP),Y
   LDA   $80
   INY
   STA   (SSP),Y
   LDA   $81
   INY
   STA   (SSP),Y
   JSR   f0
   STA   $84
   LDA   g0
   STA   $83
   BMI   .sx_neg@90
   LDA   #$00
   BEQ   .sx_done@91
.sx_neg@90:
   LDA   #$FF
.sx_done@91:
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
   ADC   #$06
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$05
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

   LDA   #$97
   STA   g0
   RTS
