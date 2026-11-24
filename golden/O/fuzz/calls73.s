SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24

; @zp-link-meta-begin
; def f0 params= locals= indirect=false in_cycle=false
; def f1 params= locals= indirect=false in_cycle=false
; def f2 params= locals= indirect=false in_cycle=false
; def f3 params= locals= indirect=false in_cycle=false
; def f4 params= locals= indirect=false in_cycle=false
; def main params= locals= indirect=false in_cycle=false
; def rec params= locals= indirect=false in_cycle=true
; call f0 -> rec
; call f1 -> f2
; call f1 -> f3
; call f2 -> rec
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
; item g0 section=.data local=false zero=false
; runtime hargs_size=32 hargs_need=29
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

   ; prologue: 6 arg bytes, 4 local bytes, 4 callee-saved bytes
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

   LDY   #$08
   LDA   (FP),Y
   BMI   .sx_neg@1
   LDA   #$00
   BEQ   .sx_done@2
.sx_neg@1:
   LDA   #$FF
.sx_done@2:
   STA   $81
   STA   $80
   LDY   #$09
   LDA   (FP),Y
   SEC
   LDY   #$07
   SBC   (FP),Y
   LDY   #$0A
   LDA   (FP),Y
   LDY   #$08
   SBC   (FP),Y
   LDA   #$00
   SBC   $81
   LDA   #$00
   SBC   $80
   LDA   #$00
   ROL
   STA   $83
   LDA   (FP),Y
   BMI   .sx_neg@3
   LDA   #$00
   BEQ   .sx_done@4
.sx_neg@3:
   LDA   #$FF
.sx_done@4:
   STA   $82
   STA   $81
   LDY   #$07
   LDA   (FP),Y
   ORA   g0
   STA   $C3
   INY
   LDA   (FP),Y
   ORA   g0+1
   STA   $C2
   LDA   $82
   ORA   g0+2
   STA   $C1
   LDA   $81
   ORA   g0+3
   STA   $C0
   LDY   #$0B
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   STA   $82
   LDY   #$0B
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$01
   LDA   (DPTR),Y
   STA   $81
   LDY   #$09
   LDA   (FP),Y
   CLC
   SBC   $83
   INY
   LDA   (FP),Y
   SBC   #$00
   LDA   #$00
   ROL
   CLC
   ADC   $82
   STA   $80
   LDA   $81
   ADC   #$00
   STA   $81
   INY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDA   $80
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$0B
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDA   $81
   LDY   #$01
   STA   (DPTR),Y
   LDY   #$0B
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   STA   $80
   LDY   #$0B
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$01
   LDA   (DPTR),Y
   STA   $81
   LDY   #$09
   LDA   (FP),Y
   AND   #$07
   TAX
   INY
   LDA   (FP),Y
   CPX   #$00
   BEQ   .shift_done@6
.shift_loop@5:
   ASL   $80
   ROL   $81
   DEX
   BNE   .shift_loop@5
.shift_done@6:
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
   LDA   $81
   INY
   STA   (SSP),Y
   JSR   rec
   LDA   $C3
   ORA   $C2
   ORA   $C1
   ORA   $C0
   BEQ   .and_false@1
   LDA   #$01
   BNE   .and_end@2
.and_false@1:
   LDA   #$00
.and_end@2:
   STA   HARGS+8
   LDA   #$00
   STA   HARGS+9
   LDY   #$00
   STY   HARGS+10
   STA   HARGS+11

   ; epilogue
   INY
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
   ADC   #$0C
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

f3:
   SUBROUTINE

   LDA   g0
   STA   $88
   BMI   .sx_neg@7
   LDA   #$00
   BEQ   .sx_done@8
.sx_neg@7:
   LDA   #$FF
.sx_done@8:
   STA   $87
   STA   $84
   STA   $86
   STA   $85
   LDA   g0
   STA   $82
   LDA   g0+1
   STA   $81
   LDA   g0+2
   STA   $83
   LDA   g0+3
   STA   $80
   LDA   $82
   CLC
   ADC   $88
   STA   $82
   LDA   $81
   ADC   $87
   STA   $81
   LDA   $83
   ADC   $84
   STA   $84
   LDA   $80
   ADC   $86
   STA   $83
   LDA   #$00
   ADC   $85
   AND   #$1F
   EOR   #$10
   STA   $80
   SEC
   SBC   #$10
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
   LDA   $82
   INY
   STA   (SSP),Y
   LDA   $81
   INY
   STA   (SSP),Y
   JSR   rec
   LDA   g0+4
   CMP   g0+4
   BNE   .cmp_true@9
   LDA   g0+3
   CMP   g0+3
   BNE   .cmp_true@9
   LDA   g0+2
   CMP   g0+2
   BNE   .cmp_true@9
   LDA   g0+1
   CMP   g0+1
   BNE   .cmp_true@9
   LDA   g0
   CMP   g0
   BNE   .cmp_true@9
   LDA   #$00
   BEQ   .cmp_end@10
.cmp_true@9:
   LDA   #$01
.cmp_end@10:
   LDX   #$00
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

   SEC
   LDA   SSP
   SBC   #$04
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   #$03
   DEY
   STA   (SSP),Y
   LDA   #$00
   INY
   STA   (SSP),Y
   LDA   #$02
   INY
   STA   (SSP),Y
   LDA   #$00
   INY
   STA   (SSP),Y
   JSR   rec
   LDY   #$0A
   LDA   (FP),Y
   BNE   .thread@0
   DEY
   LDA   (FP),Y
   BNE   .thread@0
   DEY
   LDA   (FP),Y
   BNE   .thread@0
   DEY
   LDA   (FP),Y
   CMP   #$03
   BEQ   .cmp_true@12
.thread@0:
   LDA   #$00
   BEQ   .cmp_end@13
.cmp_true@12:
   LDA   #$01
.cmp_end@13:
   STA   $80
   LDA   #$03
   AND   $80

   ; epilogue
   PHA
   CLC
   LDA   FP
   ADC   #$0A
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

f1:
   SUBROUTINE

   ; prologue: 6 arg bytes, 2 local bytes, 2 callee-saved bytes
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

   LDY   #$05
   LDA   (FP),Y
   BMI   .sx_neg@15
   LDA   #$00
   BEQ   .sx_done@16
.sx_neg@15:
   LDA   #$FF
.sx_done@16:
   STA   $C1
   LDY   #$05
   LDA   (FP),Y
   AND   #$07
   TAX
   INY
   LDA   (FP),Y
   CPX   #$00
   BEQ   .shift_done@18
.shift_loop@17:
   ASL
   DEX
   BNE   .shift_loop@17
.shift_done@18:
   STA   $80
   AND   #$7F
   STA   $C0
   LDY   #$07
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
   LDA   #$04
   STA   HARGS+4
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
   CLC
   SBC   $C0
   LDA   $82
   SBC   #$00
   LDA   $81
   SBC   #$00
   LDA   $80
   SBC   #$00
   LDA   #$00
   ROL
   STA   $C0
   JSR   f3
   STA   $80
   LDA   $C0
   EOR   $80
   STA   $C0
   CLC
   ADC   g0
   STA   $84
   LDA   g0+1
   ADC   #$00
   STA   $83
   LDA   g0+2
   ADC   #$00
   STA   $82
   LDA   g0+3
   ADC   #$00
   STA   $81
   LDA   g0+4
   ADC   #$00
   AND   #$1F
   EOR   #$10
   STA   $80
   LDA   $84
   STA   g0
   LDA   $83
   STA   g0+1
   LDA   $82
   STA   g0+2
   LDA   $81
   STA   g0+3
   LDA   $80
   SEC
   SBC   #$10
   STA   g0+4
   LDY   #$05
   LDA   (FP),Y
   SEC
   SBC   #$FF
   LDA   $C1
   SBC   #$00
   BVC   .cmp_novf@19
   EOR   #$80
.cmp_novf@19:
   ASL
   LDA   #$00
   ROL
   EOR   #$01
   AND   #$7F
   STA   $87
   LDA   $C0
   LSR
   LSR
   LSR
   LSR
   STA   $86
   LDA   $C0
   ASL
   ASL
   ASL
   ASL
   STA   $85
   LDA   $C0
   LDY   #$07
   EOR   (FP),Y
   STA   $84
   LDY   #$05
   LDA   (FP),Y
   BEQ   .and_false@3
   LDY   #$07
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   INY
   ORA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@3
   LDA   #$01
   STA   $83
   LDA   #$00
   STA   $82
   JMP   .and_end@4
.and_false@3:
   LDA   #$00
   STA   $83
   LDY   #$00
   STY   $82
.and_end@4:
   ORA   #$00
   BPL   .sx_done@21
   LDA   #$FF
.sx_done@21:
   STA   $81
   STA   $80
   SEC
   LDA   SSP
   SBC   #$08
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   $87
   LDY   #$01
   STA   (SSP),Y
   LDA   $85
   INY
   STA   (SSP),Y
   LDA   $86
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
   LDA   $81
   INY
   STA   (SSP),Y
   LDA   $80
   INY
   STA   (SSP),Y
   JSR   f2
   ORA   #$00
   BEQ   .if_else@6
   LDA   $C0
   CLC
   ADC   #$03
   STA   $85
   JMP   .if_end@5
.if_else@6:
   LDA   $C0
   EOR   #$05
   STA   $85
.if_end@5:
   LDA   g0
   CLC
   ADC   $85
   STA   $84
   LDA   g0+1
   ADC   #$00
   STA   $83
   LDA   g0+2
   ADC   #$00
   STA   $82
   LDA   g0+3
   ADC   #$00
   STA   $81
   LDA   g0+4
   ADC   #$00
   AND   #$1F
   EOR   #$10
   STA   $80
   LDA   $84
   STA   g0
   LDA   $83
   STA   g0+1
   LDA   $82
   STA   g0+2
   LDA   $81
   STA   g0+3
   LDA   $80
   SEC
   SBC   #$10
   STA   g0+4
   LDA   $85
   STA   HARGS
   LDA   #$00
   STA   HARGS+1
   LDY   #$00
   STY   HARGS+2
   STA   HARGS+3
   LDA   #$E9
   STA   HARGS+4
   LDA   #$03
   STA   HARGS+5
   TYA
   STA   HARGS+6
   STA   HARGS+7
   JSR   sdivmod32
   LDA   HARGS+8
   STA   $83
   LDA   HARGS+9
   STA   $82
   LDA   HARGS+11
   ORA   HARGS+10
   ORA   $82
   ORA   $83
   BEQ   .cmp_end@22
   LDA   #$01
.cmp_end@22:

   ; epilogue
   PHA
   LDY   #$01
   LDA   (FP),Y
   STA   $C0
   INY
   LDA   (FP),Y
   STA   $C1
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
   PLA
   RTS

f0:
   SUBROUTINE

   ; prologue: 10 arg bytes, 0 local bytes
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

   LDY   #$06
   LDA   (FP),Y
   ORA   #$01
   STA   HARGS+8
   INY
   LDA   (FP),Y
   STA   HARGS+9
   INY
   LDA   (FP),Y
   STA   HARGS+10
   INY
   LDA   (FP),Y
   STA   HARGS+11
   INY
   LDA   (FP),Y
   STA   $80
   LDY   #$06
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
   INY
   LDA   (FP),Y
   STA   HARGS+4
   ASL
   LDA   #$00
   ADC   #$FF
   EOR   #$FF
   STA   HARGS+5
   STA   HARGS+6
   STA   HARGS+7
   LDA   $80
   STA   HARGS+12
   ASL
   LDA   #$00
   ADC   #$FF
   EOR   #$FF
   STA   HARGS+13
   STA   HARGS+14
   STA   HARGS+15
   JSR   sdivmod64
   LDA   HARGS+24
   STA   $84
   LDA   HARGS+25
   STA   $83
   LDA   HARGS+26
   STA   $82
   LDA   HARGS+27
   STA   $81
   LDA   HARGS+28
   STA   $80
   LDA   $84
   ORA   $83
   ORA   $82
   ORA   $81
   ORA   $80
   BEQ   .and_false@7
   LDY   #$04
   LDA   (FP),Y
   SEC
   LDY   #$06
   SBC   (FP),Y
   STA   $84
   DEY
   LDA   (FP),Y
   LDY   #$07
   SBC   (FP),Y
   STA   $83
   LDA   #$00
   INY
   SBC   (FP),Y
   STA   $82
   LDA   #$00
   INY
   SBC   (FP),Y
   STA   $81
   LDA   #$00
   INY
   SBC   (FP),Y
   AND   #$1F
   EOR   #$10
   SEC
   SBC   #$10
   STA   $80
   LDA   $84
   ORA   $83
   ORA   $82
   ORA   $81
   ORA   $80
   BEQ   .and_false@7
   LDA   #$01
   BNE   .and_end@8
.and_false@7:
   LDA   #$00
.and_end@8:
   SEC
   LDY   #$06
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
   INY
   SBC   (FP),Y
   BVC   .jcmp_novf@23
   EOR   #$80
.jcmp_novf@23:
   BMI   .and_false@9
   LDY   #$04
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .and_end@10
.and_false@9:
.and_end@10:
   LDY   #$0B
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   STA   $81
   LDY   #$0B
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
   LDY   #$06
   ADC   (FP),Y
   STA   $81
   LDA   $80
   INY
   ADC   (FP),Y
   STA   $80
   LDY   #$0B
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDA   $81
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$0B
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDA   $80
   LDY   #$01
   STA   (DPTR),Y
   LDA   #$00
   LDY   #$05
   CMP   (FP),Y
   BNE   .cmp_true@24
   LDY   #$03
   LDA   (FP),Y
   INY
   CMP   (FP),Y
   BNE   .cmp_true@24
   LDA   #$00
   BEQ   .cmp_end@25
.cmp_true@24:
   LDA   #$01
.cmp_end@25:
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
   LDY   #$06
   LDA   (FP),Y

   ; epilogue
   PHA
   CLC
   LDA   FP
   ADC   #$0C
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
   SBC   #$0A
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   #$49
   LDY   #$01
   STA   (SSP),Y
   LDA   #$F8
   INY
   STA   (SSP),Y
   LDA   #$00
   INY
   STA   (SSP),Y
   LDA   #$28
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
   LDA   g0+1
   STA   $82
   LDA   g0+2
   STA   $81
   LDA   g0+3
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

g0:
   DC.B  $90
   DS.B  4
