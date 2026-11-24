SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24

rec:
   SUBROUTINE

   ; prologue: 4 arg bytes, 16 local bytes
   SEC
   LDA   SSP
   SBC   #$12
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$11
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   LDA   #$00
   SEC
   INY
   SBC   (FP),Y
   LDA   #$00
   INY
   SBC   (FP),Y
   BVC   .cmp_novf@0
   EOR   #$80
.cmp_novf@0:
   ASL
   LDA   #$00
   ROL
   EOR   #$01
   LDY   #$01
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@0
   LDY   #$15
   LDA   (FP),Y
   LDY   #$03
   STA   (FP),Y
   LDY   #$16
   LDA   (FP),Y
   LDA   #$00
   LDY   #$04
   STA   (FP),Y
   TAX
   DEY
   LDA   (FP),Y

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$16
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$11
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
   LDY   #$13
   LDA   (FP),Y
   SEC
   SBC   #$01
   LDY   #$05
   STA   (FP),Y
   LDY   #$14
   LDA   (FP),Y
   SBC   #$00
   LDY   #$06
   STA   (FP),Y
   LDY   #$15
   LDA   (FP),Y
   STA   HARGS
   INY
   LDA   (FP),Y
   STA   HARGS+1
   LDA   #$03
   STA   HARGS+2
   LDA   #$00
   STA   HARGS+3
   JSR   mul16
   LDA   HARGS+4
   LDY   #$07
   STA   (FP),Y
   LDA   HARGS+5
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   LDY   #$13
   ADC   (FP),Y
   LDY   #$09
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$14
   ADC   (FP),Y
   LDY   #$0A
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$0B
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   AND   #$03
   LDY   #$0C
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$04
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDY   #$05
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$06
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$0B
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$0C
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   JSR   rec
   LDY   #$0D
   STA   (FP),Y
   TXA
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$01
   LDY   #$0F
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$10
   STA   (FP),Y
   TAX
   DEY
   LDA   (FP),Y

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$16
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$11
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

   ; prologue: 2 arg bytes, 52 local bytes
   SEC
   LDA   SSP
   SBC   #$36
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$35
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   LDA   g0
   SEC
   SBC   g0
   LDA   g0+1
   SBC   g0+1
   LDA   g0+2
   SBC   g0+2
   LDA   g0+3
   SBC   g0+3
   BVC   .cmp_novf@1
   EOR   #$80
.cmp_novf@1:
   ASL
   LDA   #$00
   ROL
   LDY   #$01
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   SEC
   DEY
   SBC   (FP),Y
   LDY   #$03
   STA   (FP),Y
   LDA   #$00
   DEY
   SBC   (FP),Y
   LDY   #$04
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$05
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@2
   LDA   #$00
   JMP   .sx_done@3
.sx_neg@2:
   LDA   #$FF
.sx_done@3:
   LDY   #$08
   STA   (FP),Y
   LDA   #$03
   STA   HARGS
   LDA   #$00
   STA   HARGS+1
   DEY
   LDA   (FP),Y
   STA   HARGS+2
   INY
   LDA   (FP),Y
   STA   HARGS+3
   JSR   mul16
   LDA   HARGS+4
   LDY   #$09
   STA   (FP),Y
   LDA   HARGS+5
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@1
   LDY   #$06
   LDA   (FP),Y
   LDY   #$0B
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@4
   LDA   #$00
   JMP   .sx_done@5
.sx_neg@4:
   LDA   #$FF
.sx_done@5:
   LDY   #$0C
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   SEC
   SBC   #$01
   INY
   LDA   (FP),Y
   SBC   #$00
   BVC   .cmp_novf@6
   EOR   #$80
.cmp_novf@6:
   ASL
   LDA   #$00
   ROL
   LDY   #$0D
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@1
   LDA   #$01
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   JMP   .and_end@2
.and_false@1:
   LDA   #$00
   LDY   #$0F
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@2:
   LDY   #$0F
   LDA   (FP),Y
   LDY   #$11
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$12
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@7
   LDA   #$00
   JMP   .sx_done@8
.sx_neg@7:
   LDA   #$FF
.sx_done@8:
   LDY   #$13
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$11
   LDA   (FP),Y
   LDY   #$15
   STA   (FP),Y
   LDY   #$12
   LDA   (FP),Y
   LDY   #$16
   STA   (FP),Y
   LDY   #$13
   LDA   (FP),Y
   LDY   #$17
   STA   (FP),Y
   AND   #$03
   INY
   STA   (FP),Y
   LDY   #$15
   LDA   (FP),Y
   LDY   #$19
   STA   (FP),Y
   LDY   #$16
   LDA   (FP),Y
   LDY   #$1A
   STA   (FP),Y
   LDY   #$17
   LDA   (FP),Y
   LDY   #$1B
   STA   (FP),Y
   LDY   #$18
   LDA   (FP),Y
   LDY   #$1C
   STA   (FP),Y
   LDY   #$19
   LDA   (FP),Y
   LDY   #$1D
   STA   (FP),Y
   LDY   #$1A
   LDA   (FP),Y
   LDY   #$1E
   STA   (FP),Y
   LDY   #$1B
   LDA   (FP),Y
   LDY   #$1F
   STA   (FP),Y
   LDY   #$1C
   LDA   (FP),Y
   LDY   #$20
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$1D
   LDA   (FP),Y
   LDY   #$25
   STA   (FP),Y
   LDY   #$1E
   LDA   (FP),Y
   LDY   #$26
   STA   (FP),Y
   LDY   #$1F
   LDA   (FP),Y
   LDY   #$27
   STA   (FP),Y
   LDY   #$20
   LDA   (FP),Y
   LDY   #$28
   STA   (FP),Y
   LDY   #$21
   LDA   (FP),Y
   LDY   #$29
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$37
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$2D
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$2E
   STA   (FP),Y
   LDY   #$25
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
   BEQ   .and_false@3
   LDA   g1
   ORA   g1+1
   BEQ   .and_false@3
   LDA   #$01
   LDY   #$2F
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   JMP   .and_end@4
.and_false@3:
   LDA   #$00
   LDY   #$2F
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@4:
   LDY   #$2D
   LDA   (FP),Y
   CLC
   LDY   #$2F
   ADC   (FP),Y
   LDY   #$31
   STA   (FP),Y
   LDY   #$2E
   LDA   (FP),Y
   LDY   #$30
   ADC   (FP),Y
   LDY   #$32
   STA   (FP),Y
   LDY   #$37
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$31
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$32
   LDA   (FP),Y
   LDY   #$01
   STA   (DPTR),Y
   LDY   #$37
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$33
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$34
   STA   (FP),Y
   TAX
   DEY
   LDA   (FP),Y

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$38
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$35
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

   ; prologue: 9 arg bytes, 117 local bytes
   SEC
   LDA   SSP
   SBC   #$77
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$76
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   LDY   #$80
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$03
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$04
   STA   (FP),Y
   LDY   #$80
   LDA   (FP),Y
   LDY   #$05
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$80
   LDA   (FP),Y
   LDY   #$07
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$09
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$0A
   STA   (FP),Y
   LDY   #$03
   LDA   (FP),Y
   CLC
   LDY   #$09
   SBC   (FP),Y
   LDY   #$04
   LDA   (FP),Y
   LDY   #$0A
   SBC   (FP),Y
   LDA   #$00
   ROL
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$06
   LDA   (FP),Y
   LDY   #$0C
   CMP   (FP),Y
   BNE   .cmp_true@9
   LDY   #$05
   LDA   (FP),Y
   LDY   #$0B
   CMP   (FP),Y
   BNE   .cmp_true@9
   LDA   #$00
   JMP   .cmp_end@10
.cmp_true@9:
   LDA   #$01
.cmp_end@10:
   LDY   #$0D
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$0F
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$10
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@12
   LDA   #$00
   JMP   .sx_done@13
.sx_neg@12:
   LDA   #$FF
.sx_done@13:
   LDY   #$11
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$0F
   LDA   (FP),Y
   LDY   #$13
   STA   (FP),Y
   LDY   #$10
   LDA   (FP),Y
   LDY   #$14
   STA   (FP),Y
   LDY   #$11
   LDA   (FP),Y
   LDY   #$15
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   g1
   AND   #$07
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDA   #$E8
   INY
   STA   (FP),Y
   LDA   #$03
   INY
   STA   (FP),Y
   LDY   #$17
   LDA   (FP),Y
   TAX
   BEQ   .shift_done@15
.shift_loop@14:
   LDY   #$1A
   LDA   (FP),Y
   ASL
   LDA   (FP),Y
   ROR
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ROR
   STA   (FP),Y
   DEX
   BNE   .shift_loop@14
.shift_done@15:
   SEC
   LDA   #$00
   LDY   #$13
   SBC   (FP),Y
   LDY   #$1B
   STA   (FP),Y
   LDA   #$00
   LDY   #$14
   SBC   (FP),Y
   LDY   #$1C
   STA   (FP),Y
   LDA   #$00
   LDY   #$15
   SBC   (FP),Y
   LDY   #$1D
   STA   (FP),Y
   LDA   #$00
   LDY   #$16
   SBC   (FP),Y
   LDY   #$1E
   STA   (FP),Y
   LDA   #$07
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   ORA   #$00
   BPL   .sx_done@17
   LDA   #$FF
.sx_done@17:
   LDY   #$21
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$1B
   LDA   (FP),Y
   LDY   #$1F
   AND   (FP),Y
   LDY   #$23
   STA   (FP),Y
   LDY   #$1C
   LDA   (FP),Y
   LDY   #$20
   AND   (FP),Y
   LDY   #$24
   STA   (FP),Y
   LDY   #$1D
   LDA   (FP),Y
   LDY   #$21
   AND   (FP),Y
   LDY   #$25
   STA   (FP),Y
   LDY   #$1E
   LDA   (FP),Y
   LDY   #$22
   AND   (FP),Y
   LDY   #$26
   STA   (FP),Y
   LDY   #$19
   LDA   (FP),Y
   LDY   #$27
   STA   (FP),Y
   LDY   #$1A
   LDA   (FP),Y
   LDY   #$28
   STA   (FP),Y
   LDY   #$23
   LDA   (FP),Y
   TAX
   CPX   #$00
   BEQ   .shift_done@19
.shift_loop@18:
   LDY   #$27
   LDA   (FP),Y
   ASL
   STA   (FP),Y
   INY
   LDA   (FP),Y
   ROL
   STA   (FP),Y
   DEX
   BNE   .shift_loop@18
.shift_done@19:
   LDY   #$27
   LDA   (FP),Y
   LDY   #$29
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$2A
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@20
   LDA   #$00
   JMP   .sx_done@21
.sx_neg@20:
   LDA   #$FF
.sx_done@21:
   LDY   #$2B
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$29
   LDA   (FP),Y
   LDY   #$2D
   STA   (FP),Y
   LDY   #$2A
   LDA   (FP),Y
   LDY   #$2E
   STA   (FP),Y
   LDY   #$2B
   LDA   (FP),Y
   LDY   #$2F
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$03
   LDA   (FP),Y
   LDY   #$31
   STA   (FP),Y
   LDY   #$04
   LDA   (FP),Y
   LDY   #$32
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$33
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$34
   STA   (FP),Y
   CLC
   LDA   FP
   ADC   #$33
   INY
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$02
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   DEY
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$36
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   JSR   f3
   LDY   #$37
   STA   (FP),Y
   TXA
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$39
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$3A
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@22
   LDA   #$00
   JMP   .sx_done@23
.sx_neg@22:
   LDA   #$FF
.sx_done@23:
   LDY   #$3B
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$13
   LDA   (FP),Y
   LDY   #$39
   EOR   (FP),Y
   LDY   #$3D
   STA   (FP),Y
   LDY   #$14
   LDA   (FP),Y
   LDY   #$3A
   EOR   (FP),Y
   LDY   #$3E
   STA   (FP),Y
   LDY   #$15
   LDA   (FP),Y
   LDY   #$3B
   EOR   (FP),Y
   LDY   #$3F
   STA   (FP),Y
   LDY   #$16
   LDA   (FP),Y
   LDY   #$3C
   EOR   (FP),Y
   LDY   #$40
   STA   (FP),Y
   LDY   #$3D
   LDA   (FP),Y
   LDY   #$13
   STA   (FP),Y
   LDY   #$3E
   LDA   (FP),Y
   LDY   #$14
   STA   (FP),Y
   LDY   #$3F
   LDA   (FP),Y
   LDY   #$15
   STA   (FP),Y
   LDY   #$40
   LDA   (FP),Y
   LDY   #$16
   STA   (FP),Y
   LDY   #$33
   LDA   (FP),Y
   LDY   #$41
   STA   (FP),Y
   LDY   #$34
   LDA   (FP),Y
   LDY   #$42
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@24
   LDA   #$00
   JMP   .sx_done@25
.sx_neg@24:
   LDA   #$FF
.sx_done@25:
   LDY   #$43
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$13
   LDA   (FP),Y
   CLC
   LDY   #$41
   ADC   (FP),Y
   LDY   #$45
   STA   (FP),Y
   LDY   #$14
   LDA   (FP),Y
   LDY   #$42
   ADC   (FP),Y
   LDY   #$46
   STA   (FP),Y
   LDY   #$15
   LDA   (FP),Y
   LDY   #$43
   ADC   (FP),Y
   LDY   #$47
   STA   (FP),Y
   LDY   #$16
   LDA   (FP),Y
   LDY   #$44
   ADC   (FP),Y
   LDY   #$48
   STA   (FP),Y
   LDY   #$45
   LDA   (FP),Y
   LDY   #$13
   STA   (FP),Y
   LDY   #$46
   LDA   (FP),Y
   LDY   #$14
   STA   (FP),Y
   LDY   #$47
   LDA   (FP),Y
   LDY   #$15
   STA   (FP),Y
   LDY   #$48
   LDA   (FP),Y
   LDY   #$16
   STA   (FP),Y
   LDA   g0
   LDY   #$49
   STA   (FP),Y
   LDA   g0+1
   INY
   STA   (FP),Y
   LDA   g0+2
   INY
   STA   (FP),Y
   LDA   g0+3
   INY
   STA   (FP),Y
   LDY   #$49
   LDA   (FP),Y
   CLC
   LDY   #$13
   ADC   (FP),Y
   LDY   #$4D
   STA   (FP),Y
   LDY   #$4A
   LDA   (FP),Y
   LDY   #$14
   ADC   (FP),Y
   LDY   #$4E
   STA   (FP),Y
   LDY   #$4B
   LDA   (FP),Y
   LDY   #$15
   ADC   (FP),Y
   LDY   #$4F
   STA   (FP),Y
   LDY   #$4C
   LDA   (FP),Y
   LDY   #$16
   ADC   (FP),Y
   LDY   #$50
   STA   (FP),Y
   LDY   #$4D
   LDA   (FP),Y
   LDY   #$51
   STA   (FP),Y
   LDY   #$4E
   LDA   (FP),Y
   LDY   #$52
   STA   (FP),Y
   LDY   #$4F
   LDA   (FP),Y
   LDY   #$53
   STA   (FP),Y
   LDY   #$50
   LDA   (FP),Y
   LDY   #$54
   STA   (FP),Y
   LDY   #$51
   LDA   (FP),Y
   STA   g0
   INY
   LDA   (FP),Y
   STA   g0+1
   INY
   LDA   (FP),Y
   STA   g0+2
   INY
   LDA   (FP),Y
   STA   g0+3
   LDA   g0
   INY
   STA   (FP),Y
   LDA   g0+1
   INY
   STA   (FP),Y
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
   LDY   #$55
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$56
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   JSR   rec
   LDY   #$57
   STA   (FP),Y
   TXA
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$59
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$5A
   STA   (FP),Y
   LDY   #$03
   LDA   (FP),Y
   CLC
   LDY   #$59
   ADC   (FP),Y
   LDY   #$5B
   STA   (FP),Y
   LDY   #$04
   LDA   (FP),Y
   LDY   #$5A
   ADC   (FP),Y
   LDY   #$5C
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$03
   STA   (FP),Y
   LDY   #$5C
   LDA   (FP),Y
   LDY   #$04
   STA   (FP),Y
   LDY   #$80
   LDA   (FP),Y
   LDY   #$5D
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$5F
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$60
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@26
   LDA   #$00
   JMP   .sx_done@27
.sx_neg@26:
   LDA   #$FF
.sx_done@27:
   LDY   #$61
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$7F
   LDA   (FP),Y
   LDY   #$66
   CMP   (FP),Y
   BNE   .thread@0
   LDY   #$7E
   LDA   (FP),Y
   LDY   #$65
   CMP   (FP),Y
   BNE   .thread@0
   LDY   #$7D
   LDA   (FP),Y
   LDY   #$64
   CMP   (FP),Y
   BNE   .thread@0
   LDY   #$7C
   LDA   (FP),Y
   LDY   #$63
   CMP   (FP),Y
   BNE   .thread@0
   LDY   #$7B
   LDA   (FP),Y
   LDY   #$62
   CMP   (FP),Y
   BNE   .thread@0
   LDY   #$7A
   LDA   (FP),Y
   LDY   #$61
   CMP   (FP),Y
   BNE   .thread@0
   LDY   #$79
   LDA   (FP),Y
   LDY   #$60
   CMP   (FP),Y
   BNE   .thread@0
   LDY   #$78
   LDA   (FP),Y
   LDY   #$5F
   CMP   (FP),Y
   BEQ   .cmp_true@28
.thread@0:
   LDA   #$00
   JMP   .cmp_end@29
.cmp_true@28:
   LDA   #$01
.cmp_end@29:
   LDY   #$67
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$69
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$6A
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@31
   LDA   #$00
   JMP   .sx_done@32
.sx_neg@31:
   LDA   #$FF
.sx_done@32:
   LDY   #$6B
   STA   (FP),Y
   INY
   STA   (FP),Y
   SEC
   LDA   #$00
   LDY   #$13
   SBC   (FP),Y
   LDY   #$6D
   STA   (FP),Y
   LDA   #$00
   LDY   #$14
   SBC   (FP),Y
   LDY   #$6E
   STA   (FP),Y
   LDA   #$00
   LDY   #$15
   SBC   (FP),Y
   LDY   #$6F
   STA   (FP),Y
   LDA   #$00
   LDY   #$16
   SBC   (FP),Y
   LDY   #$70
   STA   (FP),Y
   LDY   #$69
   LDA   (FP),Y
   SEC
   LDY   #$6D
   SBC   (FP),Y
   LDY   #$71
   STA   (FP),Y
   LDY   #$6A
   LDA   (FP),Y
   LDY   #$6E
   SBC   (FP),Y
   LDY   #$72
   STA   (FP),Y
   LDY   #$6B
   LDA   (FP),Y
   LDY   #$6F
   SBC   (FP),Y
   LDY   #$73
   STA   (FP),Y
   LDY   #$6C
   LDA   (FP),Y
   LDY   #$70
   SBC   (FP),Y
   LDY   #$74
   STA   (FP),Y
   DEY
   ORA   (FP),Y
   DEY
   ORA   (FP),Y
   DEY
   ORA   (FP),Y
   BEQ   .cmp_end@33
   LDA   #$01
.cmp_end@33:
   LDY   #$75
   STA   (FP),Y

   ; epilogue
   PHA
   CLC
   LDA   FP
   ADC   #$80
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

f1:
   SUBROUTINE

   ; prologue: 0 arg bytes, 173 local bytes
   SEC
   LDA   SSP
   SBC   #$AF
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$AE
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   LDA   #$FF
   LDY   #$01
   STA   (FP),Y
   INY
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@34
   LDA   #$00
   JMP   .sx_done@35
.sx_neg@34:
   LDA   #$FF
.sx_done@35:
   LDY   #$03
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   LDY   #$06
   STA   (FP),Y
   LDY   #$03
   LDA   (FP),Y
   LDY   #$07
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   g0
   INY
   STA   (FP),Y
   LDA   g0+1
   INY
   STA   (FP),Y
   LDA   g0+2
   INY
   STA   (FP),Y
   LDA   g0+3
   INY
   STA   (FP),Y
   LDY   #$0A
   LDA   (FP),Y
   LDY   #$0E
   STA   (FP),Y
   LDY   #$0B
   LDA   (FP),Y
   LDY   #$0F
   STA   (FP),Y
   LDY   #$0C
   LDA   (FP),Y
   LDY   #$10
   STA   (FP),Y
   LDY   #$0D
   LDA   (FP),Y
   AND   #$03
   LDY   #$11
   STA   (FP),Y
   LDY   #$0E
   LDA   (FP),Y
   LDY   #$12
   STA   (FP),Y
   LDY   #$0F
   LDA   (FP),Y
   LDY   #$13
   STA   (FP),Y
   LDY   #$10
   LDA   (FP),Y
   LDY   #$14
   STA   (FP),Y
   LDY   #$11
   LDA   (FP),Y
   LDY   #$15
   STA   (FP),Y
   LDA   g0
   ORA   g0+1
   ORA   g0+2
   ORA   g0+3
   BEQ   .and_false@5
   LDY   #$06
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   INY
   ORA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@5
   LDA   #$01
   LDY   #$16
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   JMP   .and_end@6
.and_false@5:
   LDA   #$00
   LDY   #$16
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@6:
   LDY   #$16
   LDA   (FP),Y
   LDY   #$18
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$19
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@36
   LDA   #$00
   JMP   .sx_done@37
.sx_neg@36:
   LDA   #$FF
.sx_done@37:
   LDY   #$1A
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$18
   LDA   (FP),Y
   LDY   #$1C
   STA   (FP),Y
   LDY   #$19
   LDA   (FP),Y
   LDY   #$1D
   STA   (FP),Y
   LDY   #$1A
   LDA   (FP),Y
   LDY   #$1E
   STA   (FP),Y
   AND   #$03
   INY
   STA   (FP),Y
   LDA   #$03
   CLC
   LDY   #$12
   ADC   (FP),Y
   LDY   #$20
   STA   (FP),Y
   LDA   #$00
   LDY   #$13
   ADC   (FP),Y
   LDY   #$21
   STA   (FP),Y
   LDA   #$00
   LDY   #$14
   ADC   (FP),Y
   LDY   #$22
   STA   (FP),Y
   LDA   #$00
   LDY   #$15
   ADC   (FP),Y
   LDY   #$23
   STA   (FP),Y
   LDY   #$20
   LDA   (FP),Y
   LDY   #$24
   STA   (FP),Y
   LDY   #$21
   LDA   (FP),Y
   LDY   #$25
   STA   (FP),Y
   LDY   #$22
   LDA   (FP),Y
   LDY   #$26
   STA   (FP),Y
   LDY   #$23
   LDA   (FP),Y
   AND   #$03
   LDY   #$27
   STA   (FP),Y
   LDY   #$1F
   LDA   (FP),Y
   LDY   #$27
   CMP   (FP),Y
   BNE   .thread@1
   LDY   #$1E
   LDA   (FP),Y
   LDY   #$26
   CMP   (FP),Y
   BNE   .thread@1
   LDY   #$1D
   LDA   (FP),Y
   LDY   #$25
   CMP   (FP),Y
   BNE   .thread@1
   LDY   #$1C
   LDA   (FP),Y
   LDY   #$24
   CMP   (FP),Y
   BEQ   .cmp_true@38
.thread@1:
   LDA   #$00
   JMP   .cmp_end@39
.cmp_true@38:
   LDA   #$01
.cmp_end@39:
   LDY   #$28
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$2A
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$01
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   ORA   #$00
   BPL   .sx_done@42
   LDA   #$FF
.sx_done@42:
   LDY   #$2E
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$2C
   LDA   (FP),Y
   LDY   #$06
   ORA   (FP),Y
   LDY   #$30
   STA   (FP),Y
   LDY   #$2D
   LDA   (FP),Y
   LDY   #$07
   ORA   (FP),Y
   LDY   #$31
   STA   (FP),Y
   LDY   #$2E
   LDA   (FP),Y
   LDY   #$08
   ORA   (FP),Y
   LDY   #$32
   STA   (FP),Y
   LDY   #$2F
   LDA   (FP),Y
   LDY   #$09
   ORA   (FP),Y
   LDY   #$33
   STA   (FP),Y
   LDA   #$07
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   ORA   #$00
   BPL   .sx_done@44
   LDA   #$FF
.sx_done@44:
   LDY   #$36
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$34
   LDA   (FP),Y
   CLC
   LDY   #$06
   ADC   (FP),Y
   LDY   #$38
   STA   (FP),Y
   LDY   #$35
   LDA   (FP),Y
   LDY   #$07
   ADC   (FP),Y
   LDY   #$39
   STA   (FP),Y
   LDY   #$36
   LDA   (FP),Y
   LDY   #$08
   ADC   (FP),Y
   LDY   #$3A
   STA   (FP),Y
   LDY   #$37
   LDA   (FP),Y
   LDY   #$09
   ADC   (FP),Y
   LDY   #$3B
   STA   (FP),Y
   LDA   #$07
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   ORA   #$00
   BPL   .sx_done@46
   LDA   #$FF
.sx_done@46:
   LDY   #$3E
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$38
   LDA   (FP),Y
   LDY   #$3C
   AND   (FP),Y
   LDY   #$40
   STA   (FP),Y
   LDY   #$39
   LDA   (FP),Y
   LDY   #$3D
   AND   (FP),Y
   LDY   #$41
   STA   (FP),Y
   LDY   #$3A
   LDA   (FP),Y
   LDY   #$3E
   AND   (FP),Y
   LDY   #$42
   STA   (FP),Y
   LDY   #$3B
   LDA   (FP),Y
   LDY   #$3F
   AND   (FP),Y
   LDY   #$43
   STA   (FP),Y
   LDY   #$30
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
   LDY   #$40
   LDA   (FP),Y
   STA   HARGS+4
   JSR   asl32
   LDA   HARGS+5
   LDY   #$44
   STA   (FP),Y
   LDA   HARGS+6
   INY
   STA   (FP),Y
   LDA   HARGS+7
   INY
   STA   (FP),Y
   LDA   HARGS+8
   INY
   STA   (FP),Y
   LDY   #$44
   LDA   (FP),Y
   LDY   #$48
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   g0
   INY
   STA   (FP),Y
   LDA   g0+1
   INY
   STA   (FP),Y
   LDA   g0+2
   INY
   STA   (FP),Y
   LDA   g0+3
   INY
   STA   (FP),Y
   LDY   #$4A
   LDA   (FP),Y
   CLC
   LDY   #$06
   ADC   (FP),Y
   LDY   #$4E
   STA   (FP),Y
   LDY   #$4B
   LDA   (FP),Y
   LDY   #$07
   ADC   (FP),Y
   LDY   #$4F
   STA   (FP),Y
   LDY   #$4C
   LDA   (FP),Y
   LDY   #$08
   ADC   (FP),Y
   LDY   #$50
   STA   (FP),Y
   LDY   #$4D
   LDA   (FP),Y
   LDY   #$09
   ADC   (FP),Y
   LDY   #$51
   STA   (FP),Y
   LDY   #$4E
   LDA   (FP),Y
   LDY   #$52
   STA   (FP),Y
   LDY   #$4F
   LDA   (FP),Y
   LDY   #$53
   STA   (FP),Y
   LDY   #$50
   LDA   (FP),Y
   LDY   #$54
   STA   (FP),Y
   LDY   #$51
   LDA   (FP),Y
   LDY   #$55
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$07
   INY
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$09
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDY   #$52
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$53
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$54
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$55
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   LDY   #$56
   LDA   (FP),Y
   LDY   #$05
   STA   (SSP),Y
   LDY   #$57
   LDA   (FP),Y
   LDY   #$06
   STA   (SSP),Y
   LDY   #$58
   LDA   (FP),Y
   LDY   #$07
   STA   (SSP),Y
   LDY   #$59
   LDA   (FP),Y
   LDY   #$08
   STA   (SSP),Y
   LDY   #$5A
   LDA   (FP),Y
   LDY   #$09
   STA   (SSP),Y
   JSR   f2
   LDY   #$5B
   STA   (FP),Y
   ORA   #$00
   BEQ   .if_else@8
   LDA   #$03
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   ORA   #$00
   BPL   .sx_done@48
   LDA   #$FF
.sx_done@48:
   LDY   #$5E
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$06
   LDA   (FP),Y
   CLC
   LDY   #$5C
   ADC   (FP),Y
   LDY   #$60
   STA   (FP),Y
   LDY   #$07
   LDA   (FP),Y
   LDY   #$5D
   ADC   (FP),Y
   LDY   #$61
   STA   (FP),Y
   LDY   #$08
   LDA   (FP),Y
   LDY   #$5E
   ADC   (FP),Y
   LDY   #$62
   STA   (FP),Y
   LDY   #$09
   LDA   (FP),Y
   LDY   #$5F
   ADC   (FP),Y
   LDY   #$63
   STA   (FP),Y
   LDY   #$60
   LDA   (FP),Y
   LDY   #$06
   STA   (FP),Y
   LDY   #$61
   LDA   (FP),Y
   LDY   #$07
   STA   (FP),Y
   LDY   #$62
   LDA   (FP),Y
   LDY   #$08
   STA   (FP),Y
   LDY   #$63
   LDA   (FP),Y
   LDY   #$09
   STA   (FP),Y
   JMP   .if_end@7
.if_else@8:
   LDA   #$05
   LDY   #$64
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   ORA   #$00
   BPL   .sx_done@50
   LDA   #$FF
.sx_done@50:
   LDY   #$66
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$06
   LDA   (FP),Y
   LDY   #$64
   EOR   (FP),Y
   LDY   #$68
   STA   (FP),Y
   LDY   #$07
   LDA   (FP),Y
   LDY   #$65
   EOR   (FP),Y
   LDY   #$69
   STA   (FP),Y
   LDY   #$08
   LDA   (FP),Y
   LDY   #$66
   EOR   (FP),Y
   LDY   #$6A
   STA   (FP),Y
   LDY   #$09
   LDA   (FP),Y
   LDY   #$67
   EOR   (FP),Y
   LDY   #$6B
   STA   (FP),Y
   LDY   #$68
   LDA   (FP),Y
   LDY   #$06
   STA   (FP),Y
   LDY   #$69
   LDA   (FP),Y
   LDY   #$07
   STA   (FP),Y
   LDY   #$6A
   LDA   (FP),Y
   LDY   #$08
   STA   (FP),Y
   LDY   #$6B
   LDA   (FP),Y
   LDY   #$09
   STA   (FP),Y
.if_end@7:
   LDA   g0
   LDY   #$6C
   STA   (FP),Y
   LDA   g0+1
   INY
   STA   (FP),Y
   LDA   g0+2
   INY
   STA   (FP),Y
   LDA   g0+3
   INY
   STA   (FP),Y
   LDY   #$6C
   LDA   (FP),Y
   CLC
   LDY   #$06
   ADC   (FP),Y
   LDY   #$70
   STA   (FP),Y
   LDY   #$6D
   LDA   (FP),Y
   LDY   #$07
   ADC   (FP),Y
   LDY   #$71
   STA   (FP),Y
   LDY   #$6E
   LDA   (FP),Y
   LDY   #$08
   ADC   (FP),Y
   LDY   #$72
   STA   (FP),Y
   LDY   #$6F
   LDA   (FP),Y
   LDY   #$09
   ADC   (FP),Y
   LDY   #$73
   STA   (FP),Y
   LDY   #$70
   LDA   (FP),Y
   LDY   #$74
   STA   (FP),Y
   LDY   #$71
   LDA   (FP),Y
   LDY   #$75
   STA   (FP),Y
   LDY   #$72
   LDA   (FP),Y
   LDY   #$76
   STA   (FP),Y
   LDY   #$73
   LDA   (FP),Y
   LDY   #$77
   STA   (FP),Y
   LDY   #$74
   LDA   (FP),Y
   STA   g0
   INY
   LDA   (FP),Y
   STA   g0+1
   INY
   LDA   (FP),Y
   STA   g0+2
   INY
   LDA   (FP),Y
   STA   g0+3
   LDY   #$06
   LDA   (FP),Y
   LDY   #$78
   STA   (FP),Y
   LDY   #$07
   LDA   (FP),Y
   LDY   #$79
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$7A
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$7B
   STA   (FP),Y
   CLC
   LDA   FP
   ADC   #$7A
   INY
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$02
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   DEY
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$7D
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   JSR   f3
   LDY   #$7E
   STA   (FP),Y
   TXA
   INY
   STA   (FP),Y
   LDY   #$49
   LDA   (FP),Y
   LDY   #$80
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$7E
   EOR   (FP),Y
   LDY   #$82
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$7F
   EOR   (FP),Y
   LDY   #$83
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$84
   STA   (FP),Y
   LDY   #$49
   STA   (FP),Y
   LDY   #$85
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   LDY   #$7A
   ADC   (FP),Y
   LDY   #$87
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$7B
   ADC   (FP),Y
   LDY   #$88
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$89
   STA   (FP),Y
   LDY   #$49
   STA   (FP),Y
   LDA   g1
   LDY   #$8A
   STA   (FP),Y
   LDA   g1+1
   INY
   STA   (FP),Y
   LDA   g1+1
   BMI   .sx_neg@51
   LDA   #$00
   JMP   .sx_done@52
.sx_neg@51:
   LDA   #$FF
.sx_done@52:
   LDY   #$8C
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   g0
   SEC
   LDY   #$8A
   SBC   (FP),Y
   LDA   g0+1
   INY
   SBC   (FP),Y
   LDA   g0+2
   INY
   SBC   (FP),Y
   LDA   g0+3
   INY
   SBC   (FP),Y
   BVC   .cmp_novf@53
   EOR   #$80
.cmp_novf@53:
   ASL
   LDA   #$00
   ROL
   EOR   #$01
   LDY   #$8E
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
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
   LDY   #$8E
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$8F
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   JSR   rec
   LDY   #$90
   STA   (FP),Y
   TXA
   INY
   STA   (FP),Y
   LDY   #$49
   LDA   (FP),Y
   LDY   #$92
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   LDY   #$90
   ADC   (FP),Y
   LDY   #$94
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$91
   ADC   (FP),Y
   LDY   #$95
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$96
   STA   (FP),Y
   LDY   #$49
   STA   (FP),Y
   LDY   #$12
   LDA   (FP),Y
   LDY   #$97
   STA   (FP),Y
   LDY   #$13
   LDA   (FP),Y
   LDY   #$98
   STA   (FP),Y
   LDY   #$14
   LDA   (FP),Y
   LDY   #$99
   STA   (FP),Y
   LDY   #$15
   LDA   (FP),Y
   LDY   #$9A
   STA   (FP),Y
   LDY   #$97
   LDA   (FP),Y
   SEC
   SBC   g0
   LDY   #$9B
   STA   (FP),Y
   LDY   #$98
   LDA   (FP),Y
   SBC   g0+1
   LDY   #$9C
   STA   (FP),Y
   LDY   #$99
   LDA   (FP),Y
   SBC   g0+2
   LDY   #$9D
   STA   (FP),Y
   LDY   #$9A
   LDA   (FP),Y
   SBC   g0+3
   LDY   #$9E
   STA   (FP),Y
   LDA   #$07
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   ORA   #$00
   BPL   .sx_done@55
   LDA   #$FF
.sx_done@55:
   LDY   #$A1
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$09
   LDA   (FP),Y
   LDY   #$A2
   CMP   (FP),Y
   BNE   .thread@2
   LDY   #$08
   LDA   (FP),Y
   LDY   #$A1
   CMP   (FP),Y
   BNE   .thread@2
   LDY   #$07
   LDA   (FP),Y
   LDY   #$A0
   CMP   (FP),Y
   BNE   .thread@2
   LDY   #$06
   LDA   (FP),Y
   LDY   #$9F
   CMP   (FP),Y
   BEQ   .cmp_true@56
.thread@2:
   LDA   #$00
   JMP   .cmp_end@57
.cmp_true@56:
   LDA   #$01
.cmp_end@57:
   LDY   #$A3
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$A5
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$A6
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@59
   LDA   #$00
   JMP   .sx_done@60
.sx_neg@59:
   LDA   #$FF
.sx_done@60:
   LDY   #$A7
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$9B
   LDA   (FP),Y
   LDY   #$A5
   ORA   (FP),Y
   LDY   #$A9
   STA   (FP),Y
   LDY   #$9C
   LDA   (FP),Y
   LDY   #$A6
   ORA   (FP),Y
   LDY   #$AA
   STA   (FP),Y
   LDY   #$9D
   LDA   (FP),Y
   LDY   #$A7
   ORA   (FP),Y
   LDY   #$AB
   STA   (FP),Y
   LDY   #$9E
   LDA   (FP),Y
   LDY   #$A8
   ORA   (FP),Y
   LDY   #$AC
   STA   (FP),Y
   DEY
   ORA   (FP),Y
   DEY
   ORA   (FP),Y
   DEY
   ORA   (FP),Y
   BEQ   .cmp_end@61
   LDA   #$01
.cmp_end@61:
   LDY   #$AD
   STA   (FP),Y

   ; epilogue
   PHA
   CLC
   LDA   FP
   ADC   #$AF
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

   ; prologue: 11 arg bytes, 113 local bytes
   SEC
   LDA   SSP
   SBC   #$73
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$72
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   LDY   #$7C
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$03
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$04
   STA   (FP),Y
   LDY   #$7D
   LDA   (FP),Y
   CLC
   LDY   #$03
   SBC   (FP),Y
   LDY   #$7E
   LDA   (FP),Y
   LDY   #$04
   SBC   (FP),Y
   LDA   #$00
   ROL
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$7C
   LDA   (FP),Y
   LDY   #$07
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$09
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$0A
   STA   (FP),Y
   LDY   #$7E
   CMP   (FP),Y
   BNE   .cmp_true@62
   LDY   #$09
   LDA   (FP),Y
   LDY   #$7D
   CMP   (FP),Y
   BNE   .cmp_true@62
   LDA   #$00
   JMP   .cmp_end@63
.cmp_true@62:
   LDA   #$01
.cmp_end@63:
   LDY   #$0B
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$06
   LDA   (FP),Y
   LDY   #$0C
   CMP   (FP),Y
   BNE   .thread@3
   LDY   #$05
   LDA   (FP),Y
   LDY   #$0B
   CMP   (FP),Y
   BEQ   .cmp_true@65
.thread@3:
   LDA   #$00
   JMP   .cmp_end@66
.cmp_true@65:
   LDA   #$01
.cmp_end@66:
   LDY   #$0D
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$0F
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$10
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@68
   LDA   #$00
   JMP   .sx_done@69
.sx_neg@68:
   LDA   #$FF
.sx_done@69:
   LDY   #$11
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$0F
   LDA   (FP),Y
   LDY   #$13
   STA   (FP),Y
   LDY   #$10
   LDA   (FP),Y
   LDY   #$14
   STA   (FP),Y
   LDY   #$11
   LDA   (FP),Y
   LDY   #$15
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$7D
   LDA   (FP),Y
   LDY   #$17
   STA   (FP),Y
   LDY   #$7E
   LDA   (FP),Y
   LDY   #$18
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$74
   LDA   (FP),Y
   ORA   #$01
   LDY   #$1F
   STA   (FP),Y
   LDY   #$75
   LDA   (FP),Y
   LDY   #$20
   STA   (FP),Y
   LDY   #$76
   LDA   (FP),Y
   LDY   #$21
   STA   (FP),Y
   LDY   #$77
   LDA   (FP),Y
   LDY   #$22
   STA   (FP),Y
   LDY   #$78
   LDA   (FP),Y
   LDY   #$23
   STA   (FP),Y
   LDY   #$79
   LDA   (FP),Y
   LDY   #$24
   STA   (FP),Y
   LDY   #$7A
   LDA   (FP),Y
   LDY   #$25
   STA   (FP),Y
   LDY   #$7B
   LDA   (FP),Y
   LDY   #$26
   STA   (FP),Y
   LDY   #$17
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
   INY
   LDA   (FP),Y
   STA   HARGS+5
   INY
   LDA   (FP),Y
   STA   HARGS+6
   INY
   LDA   (FP),Y
   STA   HARGS+7
   INY
   LDA   (FP),Y
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
   STA   HARGS+12
   INY
   LDA   (FP),Y
   STA   HARGS+13
   INY
   LDA   (FP),Y
   STA   HARGS+14
   INY
   LDA   (FP),Y
   STA   HARGS+15
   JSR   sdivmod64
   LDA   HARGS+24
   LDY   #$27
   STA   (FP),Y
   LDA   HARGS+25
   INY
   STA   (FP),Y
   LDA   HARGS+26
   INY
   STA   (FP),Y
   LDA   HARGS+27
   INY
   STA   (FP),Y
   LDA   HARGS+28
   INY
   STA   (FP),Y
   LDA   HARGS+29
   INY
   STA   (FP),Y
   LDA   HARGS+30
   INY
   STA   (FP),Y
   LDA   HARGS+31
   INY
   STA   (FP),Y
   LDY   #$27
   LDA   (FP),Y
   LDY   #$2F
   STA   (FP),Y
   LDY   #$28
   LDA   (FP),Y
   LDY   #$30
   STA   (FP),Y
   LDY   #$29
   LDA   (FP),Y
   LDY   #$31
   STA   (FP),Y
   LDY   #$2A
   LDA   (FP),Y
   LDY   #$32
   STA   (FP),Y
   LDY   #$2F
   LDA   (FP),Y
   LDY   #$33
   STA   (FP),Y
   LDY   #$30
   LDA   (FP),Y
   LDY   #$34
   STA   (FP),Y
   LDY   #$31
   LDA   (FP),Y
   LDY   #$35
   STA   (FP),Y
   LDY   #$32
   LDA   (FP),Y
   AND   #$03
   LDY   #$36
   STA   (FP),Y
   LDY   #$33
   LDA   (FP),Y
   LDY   #$37
   STA   (FP),Y
   LDY   #$34
   LDA   (FP),Y
   LDY   #$38
   STA   (FP),Y
   LDY   #$35
   LDA   (FP),Y
   LDY   #$39
   STA   (FP),Y
   LDY   #$36
   LDA   (FP),Y
   LDY   #$3A
   STA   (FP),Y
   LDA   g1
   INY
   STA   (FP),Y
   LDA   g1+1
   INY
   STA   (FP),Y
   LDA   g1+1
   BMI   .sx_neg@70
   LDA   #$00
   JMP   .sx_done@71
.sx_neg@70:
   LDA   #$FF
.sx_done@71:
   LDY   #$3D
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$3B
   LDA   (FP),Y
   LDY   #$3F
   STA   (FP),Y
   LDY   #$3C
   LDA   (FP),Y
   LDY   #$40
   STA   (FP),Y
   LDY   #$3D
   LDA   (FP),Y
   LDY   #$41
   STA   (FP),Y
   AND   #$03
   INY
   STA   (FP),Y
   LDA   #$02
   STA   HARGS
   LDA   #$00
   STA   HARGS+1
   LDA   g1
   STA   HARGS+2
   LDA   g1+1
   STA   HARGS+3
   JSR   mul16
   LDA   HARGS+4
   LDY   #$43
   STA   (FP),Y
   LDA   HARGS+5
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$45
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$46
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@72
   LDA   #$00
   JMP   .sx_done@73
.sx_neg@72:
   LDA   #$FF
.sx_done@73:
   LDY   #$47
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$45
   LDA   (FP),Y
   LDY   #$49
   STA   (FP),Y
   LDY   #$46
   LDA   (FP),Y
   LDY   #$4A
   STA   (FP),Y
   LDY   #$47
   LDA   (FP),Y
   LDY   #$4B
   STA   (FP),Y
   AND   #$03
   INY
   STA   (FP),Y
   LDY   #$3F
   LDA   (FP),Y
   LDY   #$49
   EOR   (FP),Y
   LDY   #$4D
   STA   (FP),Y
   LDY   #$40
   LDA   (FP),Y
   LDY   #$4A
   EOR   (FP),Y
   LDY   #$4E
   STA   (FP),Y
   LDY   #$41
   LDA   (FP),Y
   LDY   #$4B
   EOR   (FP),Y
   LDY   #$4F
   STA   (FP),Y
   LDY   #$42
   LDA   (FP),Y
   LDY   #$4C
   EOR   (FP),Y
   LDY   #$50
   STA   (FP),Y
   LDY   #$4D
   LDA   (FP),Y
   LDY   #$51
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$FF
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   ORA   #$00
   BPL   .sx_done@75
   LDA   #$FF
.sx_done@75:
   LDY   #$55
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$37
   LDA   (FP),Y
   CLC
   LDY   #$53
   SBC   (FP),Y
   LDY   #$38
   LDA   (FP),Y
   LDY   #$54
   SBC   (FP),Y
   LDY   #$39
   LDA   (FP),Y
   LDY   #$55
   SBC   (FP),Y
   LDY   #$3A
   LDA   (FP),Y
   LDY   #$56
   SBC   (FP),Y
   LDA   #$00
   ROL
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDA   #$FF
   SEC
   LDY   #$7D
   SBC   (FP),Y
   LDA   #$00
   INY
   SBC   (FP),Y
   LDA   #$00
   ROL
   LDY   #$59
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$58
   LDA   (FP),Y
   LDY   #$5A
   CMP   (FP),Y
   BNE   .thread@4
   LDY   #$57
   LDA   (FP),Y
   LDY   #$59
   CMP   (FP),Y
   BEQ   .cmp_true@76
.thread@4:
   LDA   #$00
   JMP   .cmp_end@77
.cmp_true@76:
   LDA   #$01
.cmp_end@77:
   LDY   #$5B
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$5D
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$7C
   LDA   (FP),Y
   LDY   #$5F
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDA   #$02
   DEY
   ORA   (FP),Y
   LDY   #$61
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$62
   STA   (FP),Y
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
   LDY   #$61
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$62
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   JSR   rec
   LDY   #$63
   STA   (FP),Y
   TXA
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$65
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$66
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@79
   LDA   #$00
   JMP   .sx_done@80
.sx_neg@79:
   LDA   #$FF
.sx_done@80:
   LDY   #$67
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$13
   LDA   (FP),Y
   CLC
   LDY   #$65
   ADC   (FP),Y
   LDY   #$69
   STA   (FP),Y
   LDY   #$14
   LDA   (FP),Y
   LDY   #$66
   ADC   (FP),Y
   LDY   #$6A
   STA   (FP),Y
   LDY   #$15
   LDA   (FP),Y
   LDY   #$67
   ADC   (FP),Y
   LDY   #$6B
   STA   (FP),Y
   LDY   #$16
   LDA   (FP),Y
   LDY   #$68
   ADC   (FP),Y
   LDY   #$6C
   STA   (FP),Y
   LDY   #$69
   LDA   (FP),Y
   LDY   #$13
   STA   (FP),Y
   LDY   #$6A
   LDA   (FP),Y
   LDY   #$14
   STA   (FP),Y
   LDY   #$6B
   LDA   (FP),Y
   LDY   #$15
   STA   (FP),Y
   LDY   #$6C
   LDA   (FP),Y
   LDY   #$16
   STA   (FP),Y
   LDY   #$5E
   LDA   (FP),Y
   LDY   #$6D
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@81
   LDA   #$00
   JMP   .cmp_end@82
.cmp_true@81:
   LDA   #$01
.cmp_end@82:
   LDY   #$6F
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$71
   STA   (FP),Y
   CMP   #$01

   ; epilogue
   PHP
   CLC
   LDA   FP
   ADC   #$7E
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

main:
   SUBROUTINE

   ; prologue: 0 arg bytes, 32 local bytes
   SEC
   LDA   SSP
   SBC   #$22
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$21
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   LDA   #$57
   LDY   #$01
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$0B
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   #$2B
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
   INY
   STA   (SSP),Y
   INY
   STA   (SSP),Y
   INY
   STA   (SSP),Y
   LDY   #$01
   LDA   (FP),Y
   LDY   #$09
   STA   (SSP),Y
   LDA   #$13
   INY
   STA   (SSP),Y
   LDA   #$00
   INY
   STA   (SSP),Y
   JSR   f0
   LDA   #$00
   ROL
   LDY   #$02
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$03
   LDA   (FP),Y
   LDY   #$07
   STA   (FP),Y
   LDY   #$04
   LDA   (FP),Y
   LDY   #$08
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   g1
   INY
   STA   (FP),Y
   LDA   g1+1
   INY
   STA   (FP),Y
   LDA   g1+1
   BMI   .sx_neg@83
   LDA   #$00
   JMP   .sx_done@84
.sx_neg@83:
   LDA   #$FF
.sx_done@84:
   LDY   #$0D
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   g0
   LDY   #$0B
   EOR   (FP),Y
   LDY   #$0F
   STA   (FP),Y
   LDA   g0+1
   LDY   #$0C
   EOR   (FP),Y
   LDY   #$10
   STA   (FP),Y
   LDA   g0+2
   LDY   #$0D
   EOR   (FP),Y
   LDY   #$11
   STA   (FP),Y
   LDA   g0+3
   LDY   #$0E
   EOR   (FP),Y
   LDY   #$12
   STA   (FP),Y
   LDY   #$07
   LDA   (FP),Y
   LDY   #$0F
   EOR   (FP),Y
   LDY   #$13
   STA   (FP),Y
   LDY   #$08
   LDA   (FP),Y
   LDY   #$10
   EOR   (FP),Y
   LDY   #$14
   STA   (FP),Y
   LDY   #$09
   LDA   (FP),Y
   LDY   #$11
   EOR   (FP),Y
   LDY   #$15
   STA   (FP),Y
   LDY   #$0A
   LDA   (FP),Y
   LDY   #$12
   EOR   (FP),Y
   LDY   #$16
   STA   (FP),Y
   LDY   #$13
   LDA   (FP),Y
   LDY   #$07
   STA   (FP),Y
   LDY   #$14
   LDA   (FP),Y
   LDY   #$08
   STA   (FP),Y
   LDY   #$15
   LDA   (FP),Y
   LDY   #$09
   STA   (FP),Y
   LDY   #$16
   LDA   (FP),Y
   LDY   #$0A
   STA   (FP),Y
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
   LDA   #$10
   STA   HARGS+4
   JSR   asr32
   LDA   HARGS+5
   LDY   #$17
   STA   (FP),Y
   LDA   HARGS+6
   INY
   STA   (FP),Y
   LDA   HARGS+7
   INY
   STA   (FP),Y
   LDA   HARGS+8
   INY
   STA   (FP),Y
   LDY   #$07
   LDA   (FP),Y
   LDY   #$17
   EOR   (FP),Y
   LDY   #$1B
   STA   (FP),Y
   LDY   #$08
   LDA   (FP),Y
   LDY   #$18
   EOR   (FP),Y
   LDY   #$1C
   STA   (FP),Y
   LDY   #$09
   LDA   (FP),Y
   LDY   #$19
   EOR   (FP),Y
   LDY   #$1D
   STA   (FP),Y
   LDY   #$0A
   LDA   (FP),Y
   LDY   #$1A
   EOR   (FP),Y
   LDY   #$1E
   STA   (FP),Y
   LDY   #$1B
   LDA   (FP),Y
   LDY   #$1F
   STA   (FP),Y
   LDY   #$1C
   LDA   (FP),Y
   LDY   #$20
   STA   (FP),Y
   TAX
   DEY
   LDA   (FP),Y

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$22
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$21
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
   DC.L  $0000002D

g1:
   DC.W  $00AE
