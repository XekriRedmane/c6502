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

   ; prologue: 1 arg bytes, 46 local bytes
   SEC
   LDA   SSP
   SBC   #$30
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$2F
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   LDA   g1
   LDY   #$01
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   LDY   #$05
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$03
   LDA   (FP),Y
   LDY   #$05
   EOR   (FP),Y
   LDY   #$07
   STA   (FP),Y
   LDY   #$04
   LDA   (FP),Y
   LDY   #$06
   EOR   (FP),Y
   LDY   #$08
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@1
   LDA   #$07
   SEC
   SBC   #$02
   LDA   #$00
   SBC   #$00
   BVC   .cmp_novf@1
   EOR   #$80
.cmp_novf@1:
   ASL
   LDA   #$00
   ROL
   LDY   #$09
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@1
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   JMP   .or_end@2
.or_true@1:
   LDA   #$01
   LDY   #$0B
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@2:
   LDY   #$0B
   LDA   (FP),Y
   LDY   #$0D
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$0E
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@2
   LDA   #$00
   JMP   .sx_done@3
.sx_neg@2:
   LDA   #$FF
.sx_done@3:
   LDY   #$0F
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$0D
   LDA   (FP),Y
   LDY   #$12
   STA   (FP),Y
   LDY   #$0E
   LDA   (FP),Y
   LDY   #$13
   STA   (FP),Y
   LDY   #$0F
   LDA   (FP),Y
   LDY   #$14
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$31
   LDA   (FP),Y
   LDY   #$17
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@4
   LDA   #$00
   JMP   .sx_done@5
.sx_neg@4:
   LDA   #$FF
.sx_done@5:
   LDY   #$18
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$19
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$1A
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@6
   LDA   #$00
   JMP   .sx_done@7
.sx_neg@6:
   LDA   #$FF
.sx_done@7:
   LDY   #$1B
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$16
   CMP   (FP),Y
   BNE   .cmp_true@8
   DEY
   CMP   (FP),Y
   BNE   .cmp_true@8
   DEY
   CMP   (FP),Y
   BNE   .cmp_true@8
   LDY   #$1A
   LDA   (FP),Y
   LDY   #$13
   CMP   (FP),Y
   BNE   .cmp_true@8
   LDY   #$19
   LDA   (FP),Y
   LDY   #$12
   CMP   (FP),Y
   BNE   .cmp_true@8
   LDA   #$00
   JMP   .cmp_end@9
.cmp_true@8:
   LDA   #$01
.cmp_end@9:
   LDY   #$1E
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDA   #$FF
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$1E
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
   JSR   sdivmod16
   LDA   HARGS+6
   LDY   #$22
   STA   (FP),Y
   LDA   HARGS+7
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$24
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
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
   LDY   #$26
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$27
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   JSR   rec
   LDY   #$28
   STA   (FP),Y
   TXA
   INY
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   LDY   #$2A
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   LDY   #$28
   ADC   (FP),Y
   LDY   #$2C
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$29
   ADC   (FP),Y
   LDY   #$2D
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$2E
   STA   (FP),Y
   LDY   #$02
   STA   (FP),Y
   LDY   #$25
   LDA   (FP),Y

   ; epilogue
   PHA
   CLC
   LDA   FP
   ADC   #$31
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$2F
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

   ; prologue: 0 arg bytes, 88 local bytes
   SEC
   LDA   SSP
   SBC   #$5A
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$59
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   LDA   g0
   ORA   #$01
   LDY   #$01
   STA   (FP),Y
   LDA   g0+1
   INY
   STA   (FP),Y
   LDA   g0
   STA   HARGS
   LDA   g0+1
   STA   HARGS+1
   DEY
   LDA   (FP),Y
   STA   HARGS+2
   INY
   LDA   (FP),Y
   STA   HARGS+3
   JSR   sdivmod16
   LDA   HARGS+4
   LDY   #$03
   STA   (FP),Y
   LDA   HARGS+5
   INY
   STA   (FP),Y
   LDA   #$07
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   ORA   #$00
   BPL   .sx_done@12
   LDA   #$FF
.sx_done@12:
   LDY   #$07
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   g1
   LDY   #$05
   AND   (FP),Y
   LDY   #$09
   STA   (FP),Y
   LDA   g1+1
   LDY   #$06
   AND   (FP),Y
   LDY   #$0A
   STA   (FP),Y
   LDA   g1+2
   LDY   #$07
   AND   (FP),Y
   LDY   #$0B
   STA   (FP),Y
   LDA   g1+3
   LDY   #$08
   AND   (FP),Y
   LDY   #$0C
   STA   (FP),Y
   LDY   #$03
   LDA   (FP),Y
   LDY   #$0D
   STA   (FP),Y
   LDY   #$04
   LDA   (FP),Y
   LDY   #$0E
   STA   (FP),Y
   LDY   #$09
   LDA   (FP),Y
   TAX
   CPX   #$00
   BEQ   .shift_done@14
.shift_loop@13:
   LDY   #$0D
   LDA   (FP),Y
   ASL
   STA   (FP),Y
   INY
   LDA   (FP),Y
   ROL
   STA   (FP),Y
   DEX
   BNE   .shift_loop@13
.shift_done@14:
   LDY   #$0D
   LDA   (FP),Y
   LDY   #$0F
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$10
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@15
   LDA   #$00
   JMP   .sx_done@16
.sx_neg@15:
   LDA   #$FF
.sx_done@16:
   LDY   #$11
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$0F
   LDA   (FP),Y
   LDY   #$14
   STA   (FP),Y
   LDY   #$10
   LDA   (FP),Y
   LDY   #$15
   STA   (FP),Y
   LDY   #$11
   LDA   (FP),Y
   LDY   #$16
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   g2
   INY
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$01
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   JSR   f3
   LDY   #$1A
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
   INY
   STA   (FP),Y
   LDY   #$14
   LDA   (FP),Y
   LDY   #$1B
   EOR   (FP),Y
   LDY   #$20
   STA   (FP),Y
   LDY   #$15
   LDA   (FP),Y
   LDY   #$1C
   EOR   (FP),Y
   LDY   #$21
   STA   (FP),Y
   LDY   #$16
   LDA   (FP),Y
   LDY   #$1D
   EOR   (FP),Y
   LDY   #$22
   STA   (FP),Y
   LDY   #$17
   LDA   (FP),Y
   LDY   #$1E
   EOR   (FP),Y
   LDY   #$23
   STA   (FP),Y
   LDY   #$18
   LDA   (FP),Y
   LDY   #$1F
   EOR   (FP),Y
   LDY   #$24
   STA   (FP),Y
   LDY   #$20
   LDA   (FP),Y
   LDY   #$14
   STA   (FP),Y
   LDY   #$21
   LDA   (FP),Y
   LDY   #$15
   STA   (FP),Y
   LDY   #$22
   LDA   (FP),Y
   LDY   #$16
   STA   (FP),Y
   LDY   #$23
   LDA   (FP),Y
   LDY   #$17
   STA   (FP),Y
   LDY   #$24
   LDA   (FP),Y
   LDY   #$18
   STA   (FP),Y
   LDA   g1
   LDY   #$25
   STA   (FP),Y
   LDA   g1+1
   INY
   STA   (FP),Y
   LDA   g1+2
   INY
   STA   (FP),Y
   LDA   g1+3
   INY
   STA   (FP),Y
   LDA   g1+3
   BMI   .sx_neg@17
   LDA   #$00
   JMP   .sx_done@18
.sx_neg@17:
   LDA   #$FF
.sx_done@18:
   LDY   #$29
   STA   (FP),Y
   LDY   #$25
   LDA   (FP),Y
   CLC
   LDY   #$14
   ADC   (FP),Y
   LDY   #$2A
   STA   (FP),Y
   LDY   #$26
   LDA   (FP),Y
   LDY   #$15
   ADC   (FP),Y
   LDY   #$2B
   STA   (FP),Y
   LDY   #$27
   LDA   (FP),Y
   LDY   #$16
   ADC   (FP),Y
   LDY   #$2C
   STA   (FP),Y
   LDY   #$28
   LDA   (FP),Y
   LDY   #$17
   ADC   (FP),Y
   LDY   #$2D
   STA   (FP),Y
   LDY   #$29
   LDA   (FP),Y
   LDY   #$18
   ADC   (FP),Y
   LDY   #$2E
   STA   (FP),Y
   LDY   #$2A
   LDA   (FP),Y
   LDY   #$2F
   STA   (FP),Y
   LDY   #$2B
   LDA   (FP),Y
   LDY   #$30
   STA   (FP),Y
   LDY   #$2C
   LDA   (FP),Y
   LDY   #$31
   STA   (FP),Y
   LDY   #$2D
   LDA   (FP),Y
   LDY   #$32
   STA   (FP),Y
   LDY   #$2E
   LDA   (FP),Y
   AND   #$01
   LDY   #$33
   STA   (FP),Y
   LDY   #$2F
   LDA   (FP),Y
   LDY   #$34
   STA   (FP),Y
   LDY   #$30
   LDA   (FP),Y
   LDY   #$35
   STA   (FP),Y
   LDY   #$31
   LDA   (FP),Y
   LDY   #$36
   STA   (FP),Y
   LDY   #$32
   LDA   (FP),Y
   LDY   #$37
   STA   (FP),Y
   LDY   #$33
   LDA   (FP),Y
   EOR   #$01
   LDY   #$38
   STA   (FP),Y
   LDY   #$34
   LDA   (FP),Y
   LDY   #$39
   STA   (FP),Y
   LDY   #$35
   LDA   (FP),Y
   LDY   #$3A
   STA   (FP),Y
   LDY   #$36
   LDA   (FP),Y
   LDY   #$3B
   STA   (FP),Y
   LDY   #$37
   LDA   (FP),Y
   LDY   #$3C
   STA   (FP),Y
   LDY   #$38
   LDA   (FP),Y
   SEC
   SBC   #$01
   LDY   #$3D
   STA   (FP),Y
   LDY   #$39
   LDA   (FP),Y
   LDY   #$3E
   STA   (FP),Y
   LDY   #$3A
   LDA   (FP),Y
   LDY   #$3F
   STA   (FP),Y
   LDY   #$3B
   LDA   (FP),Y
   LDY   #$40
   STA   (FP),Y
   LDY   #$3C
   LDA   (FP),Y
   LDY   #$41
   STA   (FP),Y
   LDY   #$3E
   LDA   (FP),Y
   LDY   #$42
   STA   (FP),Y
   LDY   #$3F
   LDA   (FP),Y
   LDY   #$43
   STA   (FP),Y
   LDY   #$40
   LDA   (FP),Y
   LDY   #$44
   STA   (FP),Y
   LDY   #$41
   LDA   (FP),Y
   AND   #$1F
   LDY   #$45
   STA   (FP),Y
   LDY   #$42
   LDA   (FP),Y
   LDY   #$46
   STA   (FP),Y
   LDY   #$43
   LDA   (FP),Y
   LDY   #$47
   STA   (FP),Y
   LDY   #$44
   LDA   (FP),Y
   LDY   #$48
   STA   (FP),Y
   LDY   #$45
   LDA   (FP),Y
   EOR   #$10
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
   LDY   #$48
   LDA   (FP),Y
   LDY   #$4C
   STA   (FP),Y
   LDY   #$49
   LDA   (FP),Y
   SEC
   SBC   #$10
   LDY   #$4D
   STA   (FP),Y
   LDY   #$4A
   LDA   (FP),Y
   STA   g1
   INY
   LDA   (FP),Y
   STA   g1+1
   INY
   LDA   (FP),Y
   STA   g1+2
   INY
   LDA   (FP),Y
   STA   g1+3
   LDA   g2
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDA   g0
   SEC
   DEY
   SBC   (FP),Y
   LDA   g0+1
   INY
   SBC   (FP),Y
   BVC   .cmp_novf@19
   EOR   #$80
.cmp_novf@19:
   ASL
   LDA   #$00
   ROL
   EOR   #$01
   LDY   #$50
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@3
   LDA   #$07
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDA   #$FF
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$52
   LDA   (FP),Y
   TAX
   BEQ   .shift_done@21
.shift_loop@20:
   LDY   #$55
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
   BNE   .shift_loop@20
.shift_done@21:
   LDY   #$54
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@3
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   JMP   .or_end@4
.or_true@3:
   LDA   #$01
   LDY   #$56
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@4:
   LDY   #$56
   LDA   (FP),Y
   LDY   #$58
   STA   (FP),Y

   ; epilogue
   PHA
   CLC
   LDA   FP
   ADC   #$5A
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

   ; prologue: 2 arg bytes, 116 local bytes
   SEC
   LDA   SSP
   SBC   #$76
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$75
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   LDA   g0
   LDY   #$01
   STA   (FP),Y
   LDA   g0+1
   INY
   STA   (FP),Y
   LDA   g0+1
   BMI   .sx_neg@22
   LDA   #$00
   JMP   .sx_done@23
.sx_neg@22:
   LDA   #$FF
.sx_done@23:
   LDY   #$03
   STA   (FP),Y
   INY
   STA   (FP),Y
   CMP   g1+3
   BNE   .thread@0
   CMP   g1+2
   BNE   .thread@0
   LDY   #$02
   LDA   (FP),Y
   CMP   g1+1
   BNE   .thread@0
   DEY
   LDA   (FP),Y
   CMP   g1
   BEQ   .cmp_true@24
.thread@0:
   LDA   #$00
   JMP   .cmp_end@25
.cmp_true@24:
   LDA   #$01
.cmp_end@25:
   LDY   #$05
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$07
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$08
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@27
   LDA   #$00
   JMP   .sx_done@28
.sx_neg@27:
   LDA   #$FF
.sx_done@28:
   LDY   #$09
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$77
   LDA   (FP),Y
   LDY   #$0B
   STA   (FP),Y
   LDY   #$78
   LDA   (FP),Y
   LDY   #$0C
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@29
   LDA   #$00
   JMP   .sx_done@30
.sx_neg@29:
   LDA   #$FF
.sx_done@30:
   LDY   #$0D
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   g1
   SEC
   LDY   #$0B
   SBC   (FP),Y
   LDY   #$0F
   STA   (FP),Y
   LDA   g1+1
   LDY   #$0C
   SBC   (FP),Y
   LDY   #$10
   STA   (FP),Y
   LDA   g1+2
   LDY   #$0D
   SBC   (FP),Y
   LDY   #$11
   STA   (FP),Y
   LDA   g1+3
   LDY   #$0E
   SBC   (FP),Y
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
   LDY   #$12
   LDA   (FP),Y
   AND   #$1F
   LDY   #$16
   STA   (FP),Y
   LDY   #$13
   LDA   (FP),Y
   LDY   #$17
   STA   (FP),Y
   LDY   #$14
   LDA   (FP),Y
   LDY   #$18
   STA   (FP),Y
   LDY   #$15
   LDA   (FP),Y
   LDY   #$19
   STA   (FP),Y
   LDY   #$16
   LDA   (FP),Y
   EOR   #$10
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
   SEC
   SBC   #$10
   LDY   #$1E
   STA   (FP),Y
   LDY   #$07
   LDA   (FP),Y
   CLC
   LDY   #$1B
   ADC   (FP),Y
   LDY   #$1F
   STA   (FP),Y
   LDY   #$08
   LDA   (FP),Y
   LDY   #$1C
   ADC   (FP),Y
   LDY   #$20
   STA   (FP),Y
   LDY   #$09
   LDA   (FP),Y
   LDY   #$1D
   ADC   (FP),Y
   LDY   #$21
   STA   (FP),Y
   LDY   #$0A
   LDA   (FP),Y
   LDY   #$1E
   ADC   (FP),Y
   LDY   #$22
   STA   (FP),Y
   LDY   #$1F
   LDA   (FP),Y
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
   AND   #$1F
   LDY   #$26
   STA   (FP),Y
   LDY   #$23
   LDA   (FP),Y
   LDY   #$27
   STA   (FP),Y
   LDY   #$24
   LDA   (FP),Y
   LDY   #$28
   STA   (FP),Y
   LDY   #$25
   LDA   (FP),Y
   LDY   #$29
   STA   (FP),Y
   LDY   #$26
   LDA   (FP),Y
   EOR   #$10
   LDY   #$2A
   STA   (FP),Y
   LDY   #$27
   LDA   (FP),Y
   LDY   #$2B
   STA   (FP),Y
   LDY   #$28
   LDA   (FP),Y
   LDY   #$2C
   STA   (FP),Y
   LDY   #$29
   LDA   (FP),Y
   LDY   #$2D
   STA   (FP),Y
   LDY   #$2A
   LDA   (FP),Y
   SEC
   SBC   #$10
   LDY   #$2E
   STA   (FP),Y
   LDY   #$2B
   LDA   (FP),Y
   LDY   #$2F
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@31
   LDA   #$00
   JMP   .sx_done@32
.sx_neg@31:
   LDA   #$FF
.sx_done@32:
   LDY   #$32
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$31
   LDA   (FP),Y
   LDY   #$35
   STA   (FP),Y
   LDY   #$32
   LDA   (FP),Y
   LDY   #$36
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
   LDA   g1+2
   INY
   STA   (FP),Y
   LDA   g1+3
   INY
   STA   (FP),Y
   LDY   #$39
   LDA   (FP),Y
   LDY   #$3D
   STA   (FP),Y
   LDY   #$3A
   LDA   (FP),Y
   LDY   #$3E
   STA   (FP),Y
   LDY   #$3B
   LDA   (FP),Y
   LDY   #$3F
   STA   (FP),Y
   LDY   #$3C
   LDA   (FP),Y
   LDY   #$40
   STA   (FP),Y
   JSR   f2
   LDY   #$41
   STA   (FP),Y
   ORA   #$00
   BEQ   .if_else@6
   LDA   #$03
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   ORA   #$00
   BPL   .sx_done@34
   LDA   #$FF
.sx_done@34:
   LDY   #$44
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$3D
   LDA   (FP),Y
   CLC
   LDY   #$42
   ADC   (FP),Y
   LDY   #$46
   STA   (FP),Y
   LDY   #$3E
   LDA   (FP),Y
   LDY   #$43
   ADC   (FP),Y
   LDY   #$47
   STA   (FP),Y
   LDY   #$3F
   LDA   (FP),Y
   LDY   #$44
   ADC   (FP),Y
   LDY   #$48
   STA   (FP),Y
   LDY   #$40
   LDA   (FP),Y
   LDY   #$45
   ADC   (FP),Y
   LDY   #$49
   STA   (FP),Y
   LDY   #$46
   LDA   (FP),Y
   LDY   #$3D
   STA   (FP),Y
   LDY   #$47
   LDA   (FP),Y
   LDY   #$3E
   STA   (FP),Y
   LDY   #$48
   LDA   (FP),Y
   LDY   #$3F
   STA   (FP),Y
   LDY   #$49
   LDA   (FP),Y
   LDY   #$40
   STA   (FP),Y
   JMP   .if_end@5
.if_else@6:
   LDA   #$05
   LDY   #$4A
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   ORA   #$00
   BPL   .sx_done@36
   LDA   #$FF
.sx_done@36:
   LDY   #$4C
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$3D
   LDA   (FP),Y
   LDY   #$4A
   EOR   (FP),Y
   LDY   #$4E
   STA   (FP),Y
   LDY   #$3E
   LDA   (FP),Y
   LDY   #$4B
   EOR   (FP),Y
   LDY   #$4F
   STA   (FP),Y
   LDY   #$3F
   LDA   (FP),Y
   LDY   #$4C
   EOR   (FP),Y
   LDY   #$50
   STA   (FP),Y
   LDY   #$40
   LDA   (FP),Y
   LDY   #$4D
   EOR   (FP),Y
   LDY   #$51
   STA   (FP),Y
   LDY   #$4E
   LDA   (FP),Y
   LDY   #$3D
   STA   (FP),Y
   LDY   #$4F
   LDA   (FP),Y
   LDY   #$3E
   STA   (FP),Y
   LDY   #$50
   LDA   (FP),Y
   LDY   #$3F
   STA   (FP),Y
   LDY   #$51
   LDA   (FP),Y
   LDY   #$40
   STA   (FP),Y
.if_end@5:
   LDA   g1
   LDY   #$52
   STA   (FP),Y
   LDA   g1+1
   INY
   STA   (FP),Y
   LDA   g1+2
   INY
   STA   (FP),Y
   LDA   g1+3
   INY
   STA   (FP),Y
   LDY   #$52
   LDA   (FP),Y
   CLC
   LDY   #$3D
   ADC   (FP),Y
   LDY   #$56
   STA   (FP),Y
   LDY   #$53
   LDA   (FP),Y
   LDY   #$3E
   ADC   (FP),Y
   LDY   #$57
   STA   (FP),Y
   LDY   #$54
   LDA   (FP),Y
   LDY   #$3F
   ADC   (FP),Y
   LDY   #$58
   STA   (FP),Y
   LDY   #$55
   LDA   (FP),Y
   LDY   #$40
   ADC   (FP),Y
   LDY   #$59
   STA   (FP),Y
   LDY   #$56
   LDA   (FP),Y
   LDY   #$5A
   STA   (FP),Y
   LDY   #$57
   LDA   (FP),Y
   LDY   #$5B
   STA   (FP),Y
   LDY   #$58
   LDA   (FP),Y
   LDY   #$5C
   STA   (FP),Y
   LDY   #$59
   LDA   (FP),Y
   LDY   #$5D
   STA   (FP),Y
   LDY   #$5A
   LDA   (FP),Y
   LDY   #$5E
   STA   (FP),Y
   LDY   #$5B
   LDA   (FP),Y
   LDY   #$5F
   STA   (FP),Y
   LDY   #$5C
   LDA   (FP),Y
   LDY   #$60
   STA   (FP),Y
   LDY   #$5D
   LDA   (FP),Y
   AND   #$1F
   LDY   #$61
   STA   (FP),Y
   LDY   #$5E
   LDA   (FP),Y
   LDY   #$62
   STA   (FP),Y
   LDY   #$5F
   LDA   (FP),Y
   LDY   #$63
   STA   (FP),Y
   LDY   #$60
   LDA   (FP),Y
   LDY   #$64
   STA   (FP),Y
   LDY   #$61
   LDA   (FP),Y
   EOR   #$10
   LDY   #$65
   STA   (FP),Y
   LDY   #$62
   LDA   (FP),Y
   LDY   #$66
   STA   (FP),Y
   LDY   #$63
   LDA   (FP),Y
   LDY   #$67
   STA   (FP),Y
   LDY   #$64
   LDA   (FP),Y
   LDY   #$68
   STA   (FP),Y
   LDY   #$65
   LDA   (FP),Y
   SEC
   SBC   #$10
   LDY   #$69
   STA   (FP),Y
   LDY   #$66
   LDA   (FP),Y
   STA   g1
   INY
   LDA   (FP),Y
   STA   g1+1
   INY
   LDA   (FP),Y
   STA   g1+2
   INY
   LDA   (FP),Y
   STA   g1+3
   LDA   #$02
   INY
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$01
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   JSR   f3
   LDY   #$6B
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
   LDY   #$35
   LDA   (FP),Y
   LDY   #$6C
   EOR   (FP),Y
   LDY   #$70
   STA   (FP),Y
   LDY   #$36
   LDA   (FP),Y
   LDY   #$6D
   EOR   (FP),Y
   LDY   #$71
   STA   (FP),Y
   LDY   #$37
   LDA   (FP),Y
   LDY   #$6E
   EOR   (FP),Y
   LDY   #$72
   STA   (FP),Y
   LDY   #$38
   LDA   (FP),Y
   LDY   #$6F
   EOR   (FP),Y
   LDY   #$73
   STA   (FP),Y
   LDY   #$70
   LDA   (FP),Y
   LDY   #$35
   STA   (FP),Y
   LDY   #$71
   LDA   (FP),Y
   LDY   #$36
   STA   (FP),Y
   LDY   #$72
   LDA   (FP),Y
   LDY   #$37
   STA   (FP),Y
   LDY   #$73
   LDA   (FP),Y
   LDY   #$38
   STA   (FP),Y
   DEY
   ORA   (FP),Y
   DEY
   ORA   (FP),Y
   DEY
   ORA   (FP),Y
   BEQ   .cmp_end@37
   LDA   #$01
.cmp_end@37:
   LDY   #$74
   STA   (FP),Y

   ; epilogue
   PHA
   CLC
   LDA   FP
   ADC   #$78
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

   ; prologue: 12 arg bytes, 89 local bytes
   SEC
   LDA   SSP
   SBC   #$5B
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$5A
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   LDY   #$60
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$5E
   LDA   (FP),Y
   LDY   #$03
   STA   (FP),Y
   LDY   #$5F
   LDA   (FP),Y
   LDY   #$04
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$03
   LDA   (FP),Y
   SEC
   LDY   #$64
   SBC   (FP),Y
   LDY   #$04
   LDA   (FP),Y
   LDY   #$65
   SBC   (FP),Y
   LDY   #$05
   LDA   (FP),Y
   LDY   #$66
   SBC   (FP),Y
   LDY   #$06
   LDA   (FP),Y
   LDY   #$67
   SBC   (FP),Y
   BVC   .cmp_novf@38
   EOR   #$80
.cmp_novf@38:
   ASL
   LDA   #$00
   ROL
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
   ORA   #$00
   BMI   .sx_neg@39
   LDA   #$00
   JMP   .sx_done@40
.sx_neg@39:
   LDA   #$FF
.sx_done@40:
   LDY   #$0B
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$64
   LDA   (FP),Y
   LDY   #$09
   AND   (FP),Y
   LDY   #$0D
   STA   (FP),Y
   LDY   #$65
   LDA   (FP),Y
   LDY   #$0A
   AND   (FP),Y
   LDY   #$0E
   STA   (FP),Y
   LDY   #$66
   LDA   (FP),Y
   LDY   #$0B
   AND   (FP),Y
   LDY   #$0F
   STA   (FP),Y
   LDY   #$67
   LDA   (FP),Y
   LDY   #$0C
   AND   (FP),Y
   LDY   #$10
   STA   (FP),Y
   LDY   #$0D
   LDA   (FP),Y
   LDY   #$11
   STA   (FP),Y
   LDY   #$0E
   LDA   (FP),Y
   LDY   #$12
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$13
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$14
   STA   (FP),Y
   LDA   g2
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   AND   #$07
   LDY   #$17
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDA   #$00
   LDY   #$18
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
   BEQ   .shift_done@42
.shift_loop@41:
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
   BNE   .shift_loop@41
.shift_done@42:
   LDY   #$19
   LDA   (FP),Y
   LDY   #$1B
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$1C
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@43
   LDA   #$00
   JMP   .sx_done@44
.sx_neg@43:
   LDA   #$FF
.sx_done@44:
   LDY   #$1D
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$60
   LDA   (FP),Y
   SEC
   LDY   #$1B
   SBC   (FP),Y
   LDY   #$61
   LDA   (FP),Y
   LDY   #$1C
   SBC   (FP),Y
   LDY   #$62
   LDA   (FP),Y
   LDY   #$1D
   SBC   (FP),Y
   LDY   #$63
   LDA   (FP),Y
   LDY   #$1E
   SBC   (FP),Y
   BVC   .cmp_novf@45
   EOR   #$80
.cmp_novf@45:
   ASL
   LDA   #$00
   ROL
   EOR   #$01
   LDY   #$1F
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$21
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$22
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@46
   LDA   #$00
   JMP   .sx_done@47
.sx_neg@46:
   LDA   #$FF
.sx_done@47:
   LDY   #$23
   STA   (FP),Y
   INY
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
   LDY   #$27
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$64
   LDA   (FP),Y
   LDY   #$29
   STA   (FP),Y
   LDY   #$65
   LDA   (FP),Y
   LDY   #$2A
   STA   (FP),Y
   LDY   #$66
   LDA   (FP),Y
   LDY   #$2B
   STA   (FP),Y
   LDY   #$67
   LDA   (FP),Y
   LDY   #$2C
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@48
   LDA   #$00
   JMP   .sx_done@49
.sx_neg@48:
   LDA   #$FF
.sx_done@49:
   LDY   #$2D
   STA   (FP),Y
   LDY   #$29
   LDA   (FP),Y
   LDY   #$2E
   STA   (FP),Y
   LDY   #$2A
   LDA   (FP),Y
   LDY   #$2F
   STA   (FP),Y
   LDY   #$2B
   LDA   (FP),Y
   LDY   #$30
   STA   (FP),Y
   LDY   #$2C
   LDA   (FP),Y
   LDY   #$31
   STA   (FP),Y
   LDY   #$2D
   LDA   (FP),Y
   LDY   #$32
   STA   (FP),Y
   LDA   #$FF
   INY
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$01
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   JSR   f3
   LDY   #$34
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
   INY
   STA   (FP),Y
   LDY   #$2E
   LDA   (FP),Y
   LDY   #$35
   EOR   (FP),Y
   LDY   #$3A
   STA   (FP),Y
   LDY   #$2F
   LDA   (FP),Y
   LDY   #$36
   EOR   (FP),Y
   LDY   #$3B
   STA   (FP),Y
   LDY   #$30
   LDA   (FP),Y
   LDY   #$37
   EOR   (FP),Y
   LDY   #$3C
   STA   (FP),Y
   LDY   #$31
   LDA   (FP),Y
   LDY   #$38
   EOR   (FP),Y
   LDY   #$3D
   STA   (FP),Y
   LDY   #$32
   LDA   (FP),Y
   LDY   #$39
   EOR   (FP),Y
   LDY   #$3E
   STA   (FP),Y
   LDY   #$3A
   LDA   (FP),Y
   LDY   #$2E
   STA   (FP),Y
   LDY   #$3B
   LDA   (FP),Y
   LDY   #$2F
   STA   (FP),Y
   LDY   #$3C
   LDA   (FP),Y
   LDY   #$30
   STA   (FP),Y
   LDY   #$3D
   LDA   (FP),Y
   LDY   #$31
   STA   (FP),Y
   LDY   #$3E
   LDA   (FP),Y
   LDY   #$32
   STA   (FP),Y
   LDA   g0
   LDY   #$3F
   STA   (FP),Y
   LDA   g0+1
   INY
   STA   (FP),Y
   LDA   g0+1
   BMI   .sx_neg@50
   LDA   #$00
   JMP   .sx_done@51
.sx_neg@50:
   LDA   #$FF
.sx_done@51:
   LDY   #$41
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$3F
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
   LDY   #$64
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
   LDY   #$43
   STA   (FP),Y
   LDA   HARGS+9
   INY
   STA   (FP),Y
   LDA   HARGS+10
   INY
   STA   (FP),Y
   LDA   HARGS+11
   INY
   STA   (FP),Y
   LDY   #$43
   LDA   (FP),Y
   LDY   #$47
   STA   (FP),Y
   LDY   #$44
   LDA   (FP),Y
   LDY   #$48
   STA   (FP),Y
   LDY   #$45
   LDA   (FP),Y
   LDY   #$49
   STA   (FP),Y
   LDY   #$46
   LDA   (FP),Y
   AND   #$1F
   LDY   #$4A
   STA   (FP),Y
   LDY   #$47
   LDA   (FP),Y
   LDY   #$4B
   STA   (FP),Y
   LDY   #$48
   LDA   (FP),Y
   LDY   #$4C
   STA   (FP),Y
   LDY   #$49
   LDA   (FP),Y
   LDY   #$4D
   STA   (FP),Y
   LDY   #$4A
   LDA   (FP),Y
   EOR   #$10
   LDY   #$4E
   STA   (FP),Y
   LDY   #$4B
   LDA   (FP),Y
   LDY   #$4F
   STA   (FP),Y
   LDY   #$4C
   LDA   (FP),Y
   LDY   #$50
   STA   (FP),Y
   LDY   #$4D
   LDA   (FP),Y
   LDY   #$51
   STA   (FP),Y
   LDY   #$4E
   LDA   (FP),Y
   SEC
   SBC   #$10
   LDY   #$52
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   ORA   #$00
   BPL   .sx_done@53
   LDA   #$FF
.sx_done@53:
   LDY   #$55
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$52
   LDA   (FP),Y
   LDY   #$56
   CMP   (FP),Y
   BNE   .cmp_true@54
   LDY   #$51
   LDA   (FP),Y
   LDY   #$55
   CMP   (FP),Y
   BNE   .cmp_true@54
   LDY   #$50
   LDA   (FP),Y
   LDY   #$54
   CMP   (FP),Y
   BNE   .cmp_true@54
   LDY   #$4F
   LDA   (FP),Y
   LDY   #$53
   CMP   (FP),Y
   BNE   .cmp_true@54
   LDA   #$00
   JMP   .cmp_end@55
.cmp_true@54:
   LDA   #$01
.cmp_end@55:
   LDY   #$57
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$59
   STA   (FP),Y
   CMP   #$01

   ; epilogue
   PHP
   CLC
   LDA   FP
   ADC   #$67
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

   ; prologue: 0 arg bytes, 39 local bytes
   SEC
   LDA   SSP
   SBC   #$29
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$28
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
   SBC   #$0C
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   #$3D
   LDY   #$01
   STA   (SSP),Y
   LDA   #$00
   INY
   STA   (SSP),Y
   LDA   #$7D
   INY
   STA   (SSP),Y
   LDA   #$00
   INY
   STA   (SSP),Y
   LDA   #$17
   INY
   STA   (SSP),Y
   LDA   #$00
   INY
   STA   (SSP),Y
   INY
   STA   (SSP),Y
   INY
   STA   (SSP),Y
   LDA   #$11
   INY
   STA   (SSP),Y
   LDA   #$00
   INY
   STA   (SSP),Y
   INY
   STA   (SSP),Y
   INY
   STA   (SSP),Y
   JSR   f0
   LDA   #$00
   ROL
   LDY   #$01
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
   LDA   g0+1
   BMI   .sx_neg@57
   LDA   #$00
   JMP   .sx_done@58
.sx_neg@57:
   LDA   #$FF
.sx_done@58:
   LDY   #$0C
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$0A
   LDA   (FP),Y
   EOR   g1
   LDY   #$0E
   STA   (FP),Y
   LDY   #$0B
   LDA   (FP),Y
   EOR   g1+1
   LDY   #$0F
   STA   (FP),Y
   LDY   #$0C
   LDA   (FP),Y
   EOR   g1+2
   LDY   #$10
   STA   (FP),Y
   LDY   #$0D
   LDA   (FP),Y
   EOR   g1+3
   LDY   #$11
   STA   (FP),Y
   LDA   g2
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$0E
   LDA   (FP),Y
   LDY   #$12
   EOR   (FP),Y
   LDY   #$16
   STA   (FP),Y
   LDY   #$0F
   LDA   (FP),Y
   LDY   #$13
   EOR   (FP),Y
   LDY   #$17
   STA   (FP),Y
   LDY   #$10
   LDA   (FP),Y
   LDY   #$14
   EOR   (FP),Y
   LDY   #$18
   STA   (FP),Y
   LDY   #$11
   LDA   (FP),Y
   LDY   #$15
   EOR   (FP),Y
   LDY   #$19
   STA   (FP),Y
   LDY   #$06
   LDA   (FP),Y
   LDY   #$16
   EOR   (FP),Y
   LDY   #$1A
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
   LDY   #$1A
   LDA   (FP),Y
   LDY   #$06
   STA   (FP),Y
   LDY   #$1B
   LDA   (FP),Y
   LDY   #$07
   STA   (FP),Y
   LDY   #$1C
   LDA   (FP),Y
   LDY   #$08
   STA   (FP),Y
   LDY   #$1D
   LDA   (FP),Y
   LDY   #$09
   STA   (FP),Y
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
   LDA   #$10
   STA   HARGS+4
   JSR   asr32
   LDA   HARGS+5
   LDY   #$1E
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
   LDY   #$06
   LDA   (FP),Y
   LDY   #$1E
   EOR   (FP),Y
   LDY   #$22
   STA   (FP),Y
   LDY   #$07
   LDA   (FP),Y
   LDY   #$1F
   EOR   (FP),Y
   LDY   #$23
   STA   (FP),Y
   LDY   #$08
   LDA   (FP),Y
   LDY   #$20
   EOR   (FP),Y
   LDY   #$24
   STA   (FP),Y
   LDY   #$09
   LDA   (FP),Y
   LDY   #$21
   EOR   (FP),Y
   LDY   #$25
   STA   (FP),Y
   LDY   #$22
   LDA   (FP),Y
   LDY   #$26
   STA   (FP),Y
   LDY   #$23
   LDA   (FP),Y
   LDY   #$27
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
   ADC   #$29
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$28
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
   DC.W  $005E

g1:
   DC.L  $000000A0

g2:
   DC.B  $9B
