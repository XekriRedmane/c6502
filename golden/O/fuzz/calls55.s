SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
g0	EQU	$26

; @zp-link-meta-begin
; def f0 params= locals= indirect=true in_cycle=false
; def f1 params= locals= indirect=true in_cycle=false
; def f2 params= locals= indirect=false in_cycle=false
; def main params= locals= indirect=false in_cycle=false
; def rec params= locals= indirect=false in_cycle=true
; call f0 -> f1
; call f0 -> rec
; call f1 -> rec
; call main -> f0
; call rec -> rec
; item rec section=.text local=false zero=false
; item f2 section=.text local=false zero=false
; item f1 section=.text local=false zero=false
; item f0 section=.text local=false zero=false
; item main section=.text local=false zero=false
; item __zp_init section=.text local=false zero=false
; zp g0 size=2 at=auto def=true init=$BD,$00 weight=12
; runtime hargs_size=32 hargs_need=24
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

f2:
   SUBROUTINE

   ; prologue: 8 arg bytes, 0 local bytes
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
   STA   HARGS+16
   INY
   LDA   (FP),Y
   STA   HARGS+17
   LDA   #$00
   STA   HARGS+18
   LDY   #$00
   STY   HARGS+19
   STY   HARGS+20
   STY   HARGS+21
   STY   HARGS+22
   STA   HARGS+23

   ; epilogue
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

f1:
   SUBROUTINE

   ; prologue: 28 arg bytes, 4 local bytes, 4 callee-saved bytes
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

   LDA   #<f2
   STA   $89
   LDA   #>f2
   STA   $8A
   LDY   #$22
   LDA   (FP),Y
   BMI   .sx_neg@1
   LDA   #$00
   BEQ   .sx_done@2
.sx_neg@1:
   LDA   #$FF
.sx_done@2:
   STA   $83
   STA   $82
   STA   $81
   STA   $80
   LDY   #$1F
   LDA   (FP),Y
   SEC
   LDY   #$0F
   SBC   (FP),Y
   LDY   #$20
   LDA   (FP),Y
   LDY   #$10
   SBC   (FP),Y
   LDY   #$21
   LDA   (FP),Y
   LDY   #$11
   SBC   (FP),Y
   LDY   #$22
   LDA   (FP),Y
   LDY   #$12
   SBC   (FP),Y
   LDA   $83
   INY
   SBC   (FP),Y
   LDA   $82
   INY
   SBC   (FP),Y
   LDA   $81
   INY
   SBC   (FP),Y
   LDA   $80
   INY
   SBC   (FP),Y
   BVC   .cmp_novf@3
   EOR   #$80
.cmp_novf@3:
   ASL
   LDA   #$00
   ROL
   STA   $88
   LDY   #$1F
   LDA   (FP),Y
   LDY   #$17
   ORA   (FP),Y
   STA   $87
   LDY   #$20
   LDA   (FP),Y
   LDY   #$18
   ORA   (FP),Y
   STA   $86
   LDY   #$21
   LDA   (FP),Y
   LDY   #$19
   ORA   (FP),Y
   STA   $85
   LDY   #$22
   LDA   (FP),Y
   LDY   #$1A
   ORA   (FP),Y
   STA   $84
   LDA   g0
   STA   $83
   LDA   g0+1
   STA   $82
   BMI   .sx_neg@4
   LDA   #$00
   BEQ   .sx_done@5
.sx_neg@4:
   LDA   #$FF
.sx_done@5:
   STA   $81
   LDY   #$22
   CMP   (FP),Y
   BNE   .cmp_true@6
   DEY
   CMP   (FP),Y
   BNE   .cmp_true@6
   LDA   $82
   DEY
   CMP   (FP),Y
   BNE   .cmp_true@6
   LDA   $83
   DEY
   CMP   (FP),Y
   BNE   .cmp_true@6
   LDA   #$00
   BEQ   .cmp_end@7
.cmp_true@6:
   LDA   #$01
.cmp_end@7:
   STA   $80
   SEC
   LDA   SSP
   SBC   #$08
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   $88
   LDY   #$01
   STA   (SSP),Y
   LDA   $87
   INY
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
   LDA   $80
   INY
   STA   (SSP),Y
   LDA   #$03
   INY
   STA   (SSP),Y
   LDA   #$00
   INY
   STA   (SSP),Y
   LDA   $89
   STA   DPTR
   LDA   $8A
   STA   DPTR+1
   JSR   icall
   LDA   HARGS+16
   STA   $83
   LDA   HARGS+17
   STA   $82
   LDA   HARGS+18
   STA   $81
   LDA   HARGS+19
   STA   $80
   LDY   #$1F
   LDA   (FP),Y
   EOR   $83
   STA   $C3
   INY
   LDA   (FP),Y
   EOR   $82
   STA   $C2
   INY
   LDA   (FP),Y
   EOR   $81
   STA   $C1
   INY
   LDA   (FP),Y
   EOR   $80
   STA   $C0
   LDY   #$07
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   INY
   ORA   (FP),Y
   INY
   ORA   (FP),Y
   INY
   ORA   (FP),Y
   INY
   ORA   (FP),Y
   INY
   ORA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@1
   LDA   g0
   ORA   g0+1
   BEQ   .and_false@1
   LDA   #$01
   STA   $81
   LDA   #$00
   STA   $80
   JMP   .and_end@2
.and_false@1:
   LDA   #$00
   STA   $81
   STA   $80
.and_end@2:
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
   STA   $82
   STX   $81
   TXA
   BMI   .sx_neg@9
   LDA   #$00
   BEQ   .sx_done@10
.sx_neg@9:
   LDA   #$FF
.sx_done@10:
   STA   $80
   LDA   $C3
   CLC
   ADC   $82
   STA   $82
   LDA   $C2
   ADC   $81
   STA   $81
   LDA   $C1
   ADC   $80
   STA   $80
   LDA   $82
   STA   HARGS+8
   LDA   $81
   STA   HARGS+9
   LDA   $80
   AND   #$1F
   STA   HARGS+10

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
   CLC
   LDA   FP
   ADC   #$22
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

f0:
   SUBROUTINE

   ; prologue: 5 arg bytes, 13 local bytes, 13 callee-saved bytes
   SEC
   LDA   SSP
   SBC   #$0F
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$0E
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

   LDA   g0+1
   CMP   g0+1
   BNE   .cmp_true@11
   LDA   g0
   CMP   g0
   BNE   .cmp_true@11
   LDA   #$00
   BEQ   .cmp_end@12
.cmp_true@11:
   LDA   #$01
.cmp_end@12:
   STA   $C2
   LDA   #$00
   BPL   .sx_done@15
   LDA   #$FF
.sx_done@15:
   STA   $C1
   STA   $C0
   LDY   #$10
   LDA   (FP),Y
   STA   HARGS
   LDA   #$00
   STA   HARGS+1
   LDY   #$00
   STY   HARGS+2
   STA   HARGS+3
   LDY   #$11
   LDA   (FP),Y
   STA   HARGS+4
   INY
   LDA   (FP),Y
   STA   HARGS+5
   INY
   LDA   (FP),Y
   STA   HARGS+6
   INY
   LDA   (FP),Y
   STA   HARGS+7
   JSR   mul32
   LDA   HARGS+8
   STA   $83
   LDA   HARGS+9
   STA   $82
   LDA   HARGS+10
   STA   $81
   LDA   HARGS+11
   STA   $80
   LDA   $C2
   SEC
   SBC   $83
   LDA   #$00
   SBC   $82
   LDA   $C1
   SBC   $81
   LDA   $C0
   SBC   $80
   LDA   #$00
   ROL
   STA   $CC
   LDA   $83
   ORA   $82
   ORA   $81
   ORA   $80
   BNE   .or_true@3
   LDA   g0
   SEC
   SBC   g0
   STA   $81
   LDA   g0+1
   SBC   g0+1
   STA   $80
   ORA   $81
   BNE   .or_true@3
   LDA   #$00
   STA   $CB
   STA   $CA
   JMP   .or_end@4
.or_true@3:
   LDA   #$01
   STA   $CB
   LDA   #$00
   STA   $CA
.or_end@4:
   ORA   #$00
   BPL   .sx_done@17
   LDA   #$FF
.sx_done@17:
   STA   $C9
   STA   $C8
   LDA   g0
   SEC
   LDY   #$10
   SBC   (FP),Y
   LDA   g0+1
   SBC   #$00
   BVC   .jcmp_novf@18
   EOR   #$80
.jcmp_novf@18:
   BPL   .or_true@5
   LDA   $CB
   AND   #$07
   STA   HARGS+4
   LDY   #$11
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
   JSR   lsr32
   LDA   HARGS+5
   STA   $83
   LDA   HARGS+6
   STA   $82
   LDA   HARGS+7
   STA   $81
   LDA   HARGS+8
   STA   $80
   LDA   $83
   ORA   $82
   ORA   $81
   ORA   $80
   BEQ   .or_end@6
.or_true@5:
.or_end@6:
   LDA   #<f2
   STA   $C6
   LDA   #>f2
   STA   $C7
   LDY   #$14
   LDA   (FP),Y
   BNE   .cmp_true@19
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@19
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@19
   DEY
   LDA   (FP),Y
   CMP   #$01
   BNE   .cmp_true@19
   LDA   #$00
   BEQ   .cmp_end@20
.cmp_true@19:
   LDA   #$01
.cmp_end@20:
   STA   $C5
   LDA   #$00
   SEC
   LDY   #$10
   SBC   (FP),Y
   STA   $C4
   LDA   (FP),Y
   SBC   #$00
   STA   $C3
   BMI   .sx_neg@22
   LDA   #$00
   BEQ   .sx_done@23
.sx_neg@22:
   LDA   #$FF
.sx_done@23:
   STA   $C2
   STA   $C1
   LDY   #$10
   LDA   (FP),Y
   EOR   #$FF
   STA   $C0
   LDA   (FP),Y
   STA   HARGS
   LDA   #$00
   STA   HARGS+1
   LDY   #$00
   STY   HARGS+2
   STA   HARGS+3
   LDA   #$FF
   STA   HARGS+4
   DEY
   STY   HARGS+5
   STY   HARGS+6
   STA   HARGS+7
   JSR   sdivmod32
   LDA   HARGS+8
   STA   $81
   LDA   HARGS+9
   STA   $80
   SEC
   LDA   SSP
   SBC   #$08
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   $C5
   LDY   #$01
   STA   (SSP),Y
   LDA   $C4
   INY
   STA   (SSP),Y
   LDA   $C3
   INY
   STA   (SSP),Y
   LDA   $C2
   INY
   STA   (SSP),Y
   LDA   $C1
   INY
   STA   (SSP),Y
   LDA   $C0
   INY
   STA   (SSP),Y
   LDA   $81
   INY
   STA   (SSP),Y
   LDA   $80
   INY
   STA   (SSP),Y
   LDA   $C6
   STA   DPTR
   LDA   $C7
   STA   DPTR+1
   JSR   icall
   LDA   HARGS+16
   STA   $83
   LDA   HARGS+17
   STA   $82
   LDA   HARGS+18
   STA   $81
   LDA   HARGS+19
   STA   $80
   LDA   #$FF
   EOR   $83
   STA   $C3
   LDA   #$FF
   EOR   $82
   STA   $C2
   LDA   #$FF
   EOR   $81
   STA   $C1
   LDA   #$FF
   EOR   $80
   STA   $C0
   BMI   .sx_neg@24
   LDA   #$00
   BEQ   .sx_done@25
.sx_neg@24:
   LDA   #$FF
.sx_done@25:
   STA   $95
   STA   $94
   STA   $93
   STA   $92
   LDA   g0
   STA   $83
   LDA   g0+1
   STA   $82
   BMI   .sx_neg@26
   LDA   #$00
   BEQ   .sx_done@27
.sx_neg@26:
   LDA   #$FF
.sx_done@27:
   STA   $81
   STA   $80
   LDY   #$14
   LDA   (FP),Y
   CMP   $80
   BNE   .cmp_true@28
   DEY
   LDA   (FP),Y
   CMP   $81
   BNE   .cmp_true@28
   DEY
   LDA   (FP),Y
   CMP   $82
   BNE   .cmp_true@28
   DEY
   LDA   (FP),Y
   CMP   $83
   BNE   .cmp_true@28
   LDA   #$00
   BEQ   .cmp_end@29
.cmp_true@28:
   LDA   #$01
.cmp_end@29:
   STA   $91
   LDA   #$00
   BPL   .sx_done@32
   LDA   #$FF
.sx_done@32:
   STA   $90
   STA   $8F
   STA   $8E
   STA   $8D
   STA   $8C
   STA   $8B
   LDA   g0
   ORA   g0+1
   BEQ   .and_false@7
   LDA   $CB
   ORA   $CA
   ORA   $C9
   ORA   $C8
   BEQ   .and_false@7
   LDA   #$01
   STA   $8A
   LDA   #$00
   STA   $89
   JMP   .and_end@8
.and_false@7:
   LDA   #$00
   STA   $8A
   LDY   #$00
   STY   $89
.and_end@8:
   ORA   #$00
   BPL   .sx_done@34
   LDA   #$FF
.sx_done@34:
   STA   $88
   STA   $87
   STA   $86
   STA   $85
   STA   $84
   STA   $83
   LDA   $CB
   CLC
   LDY   #$10
   SBC   (FP),Y
   LDA   $CA
   SBC   #$00
   LDA   $C9
   SBC   #$00
   LDA   $C8
   SBC   #$00
   LDA   #$00
   ROL
   STA   $82
   LDA   #$00
   BPL   .sx_done@36
   LDA   #$FF
.sx_done@36:
   STA   $81
   STA   $80
   SEC
   LDA   SSP
   SBC   #$1C
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   $C3
   LDY   #$01
   STA   (SSP),Y
   LDA   $C2
   INY
   STA   (SSP),Y
   LDA   $C1
   INY
   STA   (SSP),Y
   LDA   $C0
   INY
   STA   (SSP),Y
   LDA   $95
   INY
   STA   (SSP),Y
   LDA   $94
   INY
   STA   (SSP),Y
   LDA   $93
   INY
   STA   (SSP),Y
   LDA   $92
   INY
   STA   (SSP),Y
   LDA   $91
   INY
   STA   (SSP),Y
   LDA   #$00
   INY
   STA   (SSP),Y
   LDA   $90
   INY
   STA   (SSP),Y
   LDA   $8F
   INY
   STA   (SSP),Y
   LDA   $8E
   INY
   STA   (SSP),Y
   LDA   $8D
   INY
   STA   (SSP),Y
   LDA   $8C
   INY
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
   LDA   $87
   INY
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
   LDA   $81
   INY
   STA   (SSP),Y
   LDA   $80
   INY
   STA   (SSP),Y
   JSR   f1
   LDA   HARGS+8
   STA   $82
   LDA   HARGS+9
   STA   $81
   LDA   HARGS+10
   STA   $80
   LDA   $C3
   EOR   $82
   STA   HARGS
   LDA   $C2
   EOR   $81
   STA   HARGS+1
   LDA   $C1
   EOR   $80
   STA   HARGS+2
   LDA   $C0
   STA   HARGS+3
   LDY   #$10
   LDA   (FP),Y
   ORA   #$01
   STA   HARGS+4
   LDA   #$00
   STA   HARGS+5
   LDY   #$00
   STY   HARGS+6
   STA   HARGS+7
   JSR   sdivmod32
   LDA   HARGS+8
   STA   $81
   LDA   HARGS+9
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
   LDA   $CC
   BEQ   .cmp_end@37
   LDA   #$01
.cmp_end@37:

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
   CLC
   LDA   FP
   ADC   #$14
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
   SBC   #$05
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   #$0B
   DEY
   STA   (SSP),Y
   LDA   #$1C
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
   JSR   f0
   STA   $84
   LDA   g0
   STA   $83
   LDA   g0+1
   STA   $82
   BMI   .sx_neg@38
   LDA   #$00
   BEQ   .sx_done@39
.sx_neg@38:
   LDA   #$FF
.sx_done@39:
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

   LDA   #$00
   STA   g0+1
   LDA   #$BD
   STA   g0
   RTS
