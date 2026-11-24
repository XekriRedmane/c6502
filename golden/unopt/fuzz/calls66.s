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

f2:
   SUBROUTINE

   ; prologue: 10 arg bytes, 43 local bytes
   SEC
   LDA   SSP
   SBC   #$2D
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$2C
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   LDY   #$36
   LDA   (FP),Y
   AND   #$07
   LDY   #$01
   STA   (FP),Y
   LDY   #$37
   LDA   (FP),Y
   LDA   #$00
   LDY   #$02
   STA   (FP),Y
   LDA   #$01
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$01
   LDA   (FP),Y
   TAX
   BEQ   .shift_done@2
.shift_loop@1:
   LDY   #$03
   LDA   (FP),Y
   ASL
   STA   (FP),Y
   INY
   LDA   (FP),Y
   ROL
   STA   (FP),Y
   DEX
   BNE   .shift_loop@1
.shift_done@2:
   LDY   #$03
   LDA   (FP),Y
   SEC
   LDY   #$36
   SBC   (FP),Y
   LDY   #$04
   LDA   (FP),Y
   LDY   #$37
   SBC   (FP),Y
   BVC   .cmp_novf@3
   EOR   #$80
.cmp_novf@3:
   ASL
   LDA   #$00
   ROL
   EOR   #$01
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
   DEY
   LDA   (FP),Y
   LDY   #$09
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$0A
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$0B
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$36
   LDA   (FP),Y
   LDY   #$0D
   STA   (FP),Y
   LDY   #$37
   LDA   (FP),Y
   LDY   #$0E
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@4
   LDA   #$00
   JMP   .sx_done@5
.sx_neg@4:
   LDA   #$FF
.sx_done@5:
   LDY   #$0F
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$0D
   LDA   (FP),Y
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
   AND   #$07
   INY
   STA   (FP),Y
   LDA   g1
   LDY   #$11
   AND   (FP),Y
   LDY   #$15
   STA   (FP),Y
   LDA   g1+1
   LDY   #$12
   AND   (FP),Y
   LDY   #$16
   STA   (FP),Y
   LDA   g1+2
   LDY   #$13
   AND   (FP),Y
   LDY   #$17
   STA   (FP),Y
   LDA   g1+3
   LDY   #$14
   AND   (FP),Y
   LDY   #$18
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
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$19
   LDA   (FP),Y
   CLC
   ADC   g0
   LDY   #$1F
   STA   (FP),Y
   LDY   #$1A
   LDA   (FP),Y
   ADC   g0+1
   LDY   #$20
   STA   (FP),Y
   LDY   #$1B
   LDA   (FP),Y
   ADC   g0+2
   LDY   #$21
   STA   (FP),Y
   LDY   #$1C
   LDA   (FP),Y
   ADC   g0+3
   LDY   #$22
   STA   (FP),Y
   LDY   #$1D
   LDA   (FP),Y
   ADC   g0+4
   LDY   #$23
   STA   (FP),Y
   LDY   #$1E
   LDA   (FP),Y
   ADC   g0+5
   LDY   #$24
   STA   (FP),Y
   LDY   #$1F
   LDA   (FP),Y
   LDY   #$25
   STA   (FP),Y
   LDY   #$20
   LDA   (FP),Y
   LDY   #$26
   STA   (FP),Y
   LDY   #$21
   LDA   (FP),Y
   LDY   #$27
   STA   (FP),Y
   LDY   #$22
   LDA   (FP),Y
   LDY   #$28
   STA   (FP),Y
   LDY   #$23
   LDA   (FP),Y
   LDY   #$29
   STA   (FP),Y
   LDY   #$24
   LDA   (FP),Y
   AND   #$0F
   LDY   #$2A
   STA   (FP),Y
   DEY
   ORA   (FP),Y
   DEY
   ORA   (FP),Y
   DEY
   ORA   (FP),Y
   DEY
   ORA   (FP),Y
   DEY
   ORA   (FP),Y
   BEQ   .cmp_end@6
   LDA   #$01
.cmp_end@6:
   LDY   #$2B
   STA   (FP),Y

   ; epilogue
   PHA
   CLC
   LDA   FP
   ADC   #$37
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

   ; prologue: 8 arg bytes, 55 local bytes
   SEC
   LDA   SSP
   SBC   #$39
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$38
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
   LDY   #$3B
   LDA   (FP),Y
   LDY   #$02
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@7
   LDA   #$00
   JMP   .sx_done@8
.sx_neg@7:
   LDA   #$FF
.sx_done@8:
   LDY   #$03
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$01
   LDA   (FP),Y
   LDY   #$07
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   LDY   #$08
   STA   (FP),Y
   LDY   #$03
   LDA   (FP),Y
   LDY   #$09
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   AND   #$0F
   INY
   STA   (FP),Y
   LDA   g0
   ORA   #$01
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
   LDA   g0+4
   INY
   STA   (FP),Y
   LDA   g0+5
   INY
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
   INY
   LDA   (FP),Y
   STA   HARGS+4
   INY
   LDA   (FP),Y
   STA   HARGS+5
   LDA   #$00
   STA   HARGS+6
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
   LDA   #$00
   STA   HARGS+14
   STA   HARGS+15
   JSR   udivmod64
   LDA   HARGS+16
   LDY   #$13
   STA   (FP),Y
   LDA   HARGS+17
   INY
   STA   (FP),Y
   LDA   HARGS+18
   INY
   STA   (FP),Y
   LDA   HARGS+19
   INY
   STA   (FP),Y
   LDA   HARGS+20
   INY
   STA   (FP),Y
   LDA   HARGS+21
   INY
   STA   (FP),Y
   LDY   #$13
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
   BNE   .or_true@1
   LDY   #$3C
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
   BNE   .or_true@1
   LDA   #$00
   LDY   #$19
   STA   (FP),Y
   INY
   STA   (FP),Y
   JMP   .or_end@2
.or_true@1:
   LDA   #$01
   LDY   #$19
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@2:
   LDY   #$19
   LDA   (FP),Y
   LDY   #$1B
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$1C
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@9
   LDA   #$00
   JMP   .sx_done@10
.sx_neg@9:
   LDA   #$FF
.sx_done@10:
   LDY   #$1D
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$1B
   LDA   (FP),Y
   LDY   #$1F
   STA   (FP),Y
   LDY   #$1C
   LDA   (FP),Y
   LDY   #$20
   STA   (FP),Y
   LDY   #$1D
   LDA   (FP),Y
   LDY   #$21
   STA   (FP),Y
   AND   #$07
   INY
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
   LDY   #$26
   STA   (FP),Y
   LDA   g1
   STA   HARGS
   LDA   g1+1
   STA   HARGS+1
   LDA   g1+2
   STA   HARGS+2
   LDA   g1+3
   STA   HARGS+3
   LDA   #$03
   STA   HARGS+4
   LDA   #$00
   STA   HARGS+5
   LDY   #$00
   STY   HARGS+6
   STA   HARGS+7
   JSR   mul32
   LDA   HARGS+8
   LDY   #$27
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
   AND   #$07
   LDY   #$2E
   STA   (FP),Y
   LDY   #$2B
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   INY
   ORA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lnot_true@11
   LDA   #$00
   JMP   .lnot_end@12
.lnot_true@11:
   LDA   #$01
.lnot_end@12:
   LDY   #$2F
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$31
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$32
   STA   (FP),Y
   LDA   g0
   SEC
   SBC   g0
   LDA   g0+1
   SBC   g0+1
   LDA   g0+2
   SBC   g0+2
   LDA   g0+3
   SBC   g0+3
   LDA   g0+4
   SBC   g0+4
   LDA   g0+5
   SBC   g0+5
   LDA   #$00
   ROL
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$35
   STA   (FP),Y
   INY
   STA   (FP),Y
   ORA   #$00
   BEQ   .cmp_end@13
   LDA   #$01
.cmp_end@13:
   LDY   #$37
   STA   (FP),Y

   ; epilogue
   PHA
   CLC
   LDA   FP
   ADC   #$41
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

   ; prologue: 8 arg bytes, 137 local bytes
   SEC
   LDA   SSP
   SBC   #$8B
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$8A
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   LDY   #$8E
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   LDY   #$8F
   LDA   (FP),Y
   LDY   #$02
   STA   (FP),Y
   LDY   #$90
   LDA   (FP),Y
   LDY   #$03
   STA   (FP),Y
   LDY   #$91
   LDA   (FP),Y
   LDY   #$04
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$01
   LDA   (FP),Y
   LDY   #$07
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   LDY   #$08
   STA   (FP),Y
   LDY   #$03
   LDA   (FP),Y
   LDY   #$09
   STA   (FP),Y
   LDY   #$04
   LDA   (FP),Y
   LDY   #$0A
   STA   (FP),Y
   LDY   #$05
   LDA   (FP),Y
   LDY   #$0B
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$8E
   LDA   (FP),Y
   AND   #$07
   LDY   #$0D
   STA   (FP),Y
   LDY   #$8F
   LDA   (FP),Y
   LDA   #$00
   LDY   #$0E
   STA   (FP),Y
   LDY   #$90
   LDA   (FP),Y
   LDA   #$00
   LDY   #$0F
   STA   (FP),Y
   LDY   #$91
   LDA   (FP),Y
   LDA   #$00
   LDY   #$10
   STA   (FP),Y
   LDY   #$8C
   LDA   (FP),Y
   LDY   #$11
   STA   (FP),Y
   LDY   #$8D
   LDA   (FP),Y
   LDY   #$12
   STA   (FP),Y
   LDY   #$0D
   LDA   (FP),Y
   TAX
   BEQ   .shift_done@15
.shift_loop@14:
   LDY   #$11
   LDA   (FP),Y
   ASL
   STA   (FP),Y
   INY
   LDA   (FP),Y
   ROL
   STA   (FP),Y
   DEX
   BNE   .shift_loop@14
.shift_done@15:
   LDY   #$8C
   LDA   (FP),Y
   LDY   #$13
   STA   (FP),Y
   LDY   #$8D
   LDA   (FP),Y
   LDY   #$14
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@16
   LDA   #$00
   JMP   .sx_done@17
.sx_neg@16:
   LDA   #$FF
.sx_done@17:
   LDY   #$15
   STA   (FP),Y
   INY
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
   AND   #$07
   INY
   STA   (FP),Y
   LDY   #$17
   LDA   (FP),Y
   CLC
   LDY   #$8E
   SBC   (FP),Y
   LDY   #$18
   LDA   (FP),Y
   LDY   #$8F
   SBC   (FP),Y
   LDY   #$19
   LDA   (FP),Y
   LDY   #$90
   SBC   (FP),Y
   LDY   #$1A
   LDA   (FP),Y
   LDY   #$91
   SBC   (FP),Y
   LDA   #$00
   ROL
   LDY   #$1B
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$11
   LDA   (FP),Y
   SEC
   LDY   #$1B
   SBC   (FP),Y
   LDY   #$12
   LDA   (FP),Y
   LDY   #$1C
   SBC   (FP),Y
   BVC   .cmp_novf@18
   EOR   #$80
.cmp_novf@18:
   ASL
   LDA   #$00
   ROL
   LDY   #$1D
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$1F
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$20
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$21
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$22
   STA   (FP),Y
   LDA   #$01
   BEQ   .and_false@3
   LDA   g0
   ORA   g0+1
   ORA   g0+2
   ORA   g0+3
   ORA   g0+4
   ORA   g0+5
   BEQ   .and_false@3
   LDA   #$01
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   JMP   .and_end@4
.and_false@3:
   LDA   #$00
   LDY   #$23
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@4:
   LDY   #$23
   LDA   (FP),Y
   LDY   #$25
   STA   (FP),Y
   LDA   #$01
   SEC
   LDY   #$8E
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
   LDY   #$26
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$28
   STA   (FP),Y
   LDA   g1
   SEC
   SBC   g1
   LDA   g1+1
   SBC   g1+1
   LDA   g1+2
   SBC   g1+2
   LDA   g1+3
   SBC   g1+3
   LDA   #$00
   ROL
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$2B
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$2C
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@19
   LDA   #$00
   JMP   .sx_done@20
.sx_neg@19:
   LDA   #$FF
.sx_done@20:
   LDY   #$2D
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$2B
   LDA   (FP),Y
   LDY   #$31
   STA   (FP),Y
   LDY   #$2C
   LDA   (FP),Y
   LDY   #$32
   STA   (FP),Y
   LDY   #$2D
   LDA   (FP),Y
   LDY   #$33
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   AND   #$0F
   INY
   STA   (FP),Y
   LDA   g0
   INY
   STA   (FP),Y
   LDA   g0+1
   INY
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$0A
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDY   #$25
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$28
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$31
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$32
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   LDY   #$33
   LDA   (FP),Y
   LDY   #$05
   STA   (SSP),Y
   LDY   #$34
   LDA   (FP),Y
   LDY   #$06
   STA   (SSP),Y
   LDY   #$35
   LDA   (FP),Y
   LDY   #$07
   STA   (SSP),Y
   LDY   #$36
   LDA   (FP),Y
   LDY   #$08
   STA   (SSP),Y
   LDY   #$37
   LDA   (FP),Y
   LDY   #$09
   STA   (SSP),Y
   LDY   #$38
   LDA   (FP),Y
   LDY   #$0A
   STA   (SSP),Y
   JSR   f2
   LDY   #$39
   STA   (FP),Y
   ORA   #$00
   BNE   .lb_skip@0
   JMP   .if_else@6
.lb_skip@0:
   LDY   #$07
   LDA   (FP),Y
   CLC
   ADC   #$03
   LDY   #$3A
   STA   (FP),Y
   LDY   #$08
   LDA   (FP),Y
   ADC   #$00
   LDY   #$3B
   STA   (FP),Y
   LDY   #$09
   LDA   (FP),Y
   ADC   #$00
   LDY   #$3C
   STA   (FP),Y
   LDY   #$0A
   LDA   (FP),Y
   ADC   #$00
   LDY   #$3D
   STA   (FP),Y
   LDY   #$0B
   LDA   (FP),Y
   ADC   #$00
   LDY   #$3E
   STA   (FP),Y
   LDY   #$0C
   LDA   (FP),Y
   ADC   #$00
   LDY   #$3F
   STA   (FP),Y
   LDY   #$3A
   LDA   (FP),Y
   LDY   #$40
   STA   (FP),Y
   LDY   #$3B
   LDA   (FP),Y
   LDY   #$41
   STA   (FP),Y
   LDY   #$3C
   LDA   (FP),Y
   LDY   #$42
   STA   (FP),Y
   LDY   #$3D
   LDA   (FP),Y
   LDY   #$43
   STA   (FP),Y
   LDY   #$3E
   LDA   (FP),Y
   LDY   #$44
   STA   (FP),Y
   LDY   #$3F
   LDA   (FP),Y
   AND   #$0F
   LDY   #$45
   STA   (FP),Y
   LDY   #$40
   LDA   (FP),Y
   LDY   #$07
   STA   (FP),Y
   LDY   #$41
   LDA   (FP),Y
   LDY   #$08
   STA   (FP),Y
   LDY   #$42
   LDA   (FP),Y
   LDY   #$09
   STA   (FP),Y
   LDY   #$43
   LDA   (FP),Y
   LDY   #$0A
   STA   (FP),Y
   LDY   #$44
   LDA   (FP),Y
   LDY   #$0B
   STA   (FP),Y
   LDY   #$45
   LDA   (FP),Y
   LDY   #$0C
   STA   (FP),Y
   JMP   .if_end@5
.if_else@6:
   LDY   #$07
   LDA   (FP),Y
   EOR   #$05
   LDY   #$46
   STA   (FP),Y
   LDY   #$08
   LDA   (FP),Y
   LDY   #$47
   STA   (FP),Y
   LDY   #$09
   LDA   (FP),Y
   LDY   #$48
   STA   (FP),Y
   LDY   #$0A
   LDA   (FP),Y
   LDY   #$49
   STA   (FP),Y
   LDY   #$0B
   LDA   (FP),Y
   LDY   #$4A
   STA   (FP),Y
   LDY   #$0C
   LDA   (FP),Y
   LDY   #$4B
   STA   (FP),Y
   LDY   #$46
   LDA   (FP),Y
   LDY   #$07
   STA   (FP),Y
   LDY   #$47
   LDA   (FP),Y
   LDY   #$08
   STA   (FP),Y
   LDY   #$48
   LDA   (FP),Y
   LDY   #$09
   STA   (FP),Y
   LDY   #$49
   LDA   (FP),Y
   LDY   #$0A
   STA   (FP),Y
   LDY   #$4A
   LDA   (FP),Y
   LDY   #$0B
   STA   (FP),Y
   LDY   #$4B
   LDA   (FP),Y
   LDY   #$0C
   STA   (FP),Y
.if_end@5:
   LDY   #$8E
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   INY
   ORA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@7
   LDY   #$21
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@7
   LDA   #$00
   LDY   #$4C
   STA   (FP),Y
   INY
   STA   (FP),Y
   JMP   .or_end@8
.or_true@7:
   LDA   #$01
   LDY   #$4C
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@8:
   LDY   #$92
   LDA   (FP),Y
   STA   HARGS
   INY
   LDA   (FP),Y
   STA   HARGS+1
   DEY
   LDA   (FP),Y
   STA   HARGS+2
   INY
   LDA   (FP),Y
   STA   HARGS+3
   JSR   mul16
   LDA   HARGS+4
   LDY   #$4E
   STA   (FP),Y
   LDA   HARGS+5
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$50
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$51
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@21
   LDA   #$00
   JMP   .sx_done@22
.sx_neg@21:
   LDA   #$FF
.sx_done@22:
   LDY   #$52
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$50
   LDA   (FP),Y
   LDY   #$56
   STA   (FP),Y
   LDY   #$51
   LDA   (FP),Y
   LDY   #$57
   STA   (FP),Y
   LDY   #$52
   LDA   (FP),Y
   LDY   #$58
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   AND   #$0F
   INY
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$08
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDY   #$4C
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$4D
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$56
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$57
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   LDY   #$58
   LDA   (FP),Y
   LDY   #$05
   STA   (SSP),Y
   LDY   #$59
   LDA   (FP),Y
   LDY   #$06
   STA   (SSP),Y
   LDY   #$5A
   LDA   (FP),Y
   LDY   #$07
   STA   (SSP),Y
   LDY   #$5B
   LDA   (FP),Y
   LDY   #$08
   STA   (SSP),Y
   JSR   f1
   LDY   #$5C
   STA   (FP),Y
   ORA   #$00
   BEQ   .if_else@10
   LDY   #$21
   LDA   (FP),Y
   CLC
   ADC   #$03
   LDY   #$5D
   STA   (FP),Y
   LDY   #$22
   LDA   (FP),Y
   ADC   #$00
   LDY   #$5E
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$21
   STA   (FP),Y
   LDY   #$5E
   LDA   (FP),Y
   LDY   #$22
   STA   (FP),Y
   JMP   .if_end@9
.if_else@10:
   LDY   #$21
   LDA   (FP),Y
   EOR   #$05
   LDY   #$5F
   STA   (FP),Y
   LDY   #$22
   LDA   (FP),Y
   LDY   #$60
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$21
   STA   (FP),Y
   LDY   #$60
   LDA   (FP),Y
   LDY   #$22
   STA   (FP),Y
.if_end@9:
   LDA   #$00
   CMP   #$7F
   BNE   .cmp_end@24
   LDA   #$FF
   CMP   #$FF
   BEQ   .cmp_true@23
   LDA   #$00
   JMP   .cmp_end@24
.cmp_true@23:
   LDA   #$01
.cmp_end@24:
   LDY   #$61
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
   LDY   #$21
   LDA   (FP),Y
   CLC
   LDY   #$65
   ADC   (FP),Y
   LDY   #$67
   STA   (FP),Y
   LDY   #$22
   LDA   (FP),Y
   LDY   #$66
   ADC   (FP),Y
   LDY   #$68
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$21
   STA   (FP),Y
   LDY   #$68
   LDA   (FP),Y
   LDY   #$22
   STA   (FP),Y
   LDY   #$8C
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@11
   LDA   #$FF
   BEQ   .and_false@11
   LDA   #$01
   LDY   #$69
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   JMP   .and_end@12
.and_false@11:
   LDA   #$00
   LDY   #$69
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@12:
   LDY   #$69
   LDA   (FP),Y
   LDY   #$6B
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$6C
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@26
   LDA   #$00
   JMP   .sx_done@27
.sx_neg@26:
   LDA   #$FF
.sx_done@27:
   LDY   #$6D
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$6B
   LDA   (FP),Y
   LDY   #$71
   STA   (FP),Y
   LDY   #$6C
   LDA   (FP),Y
   LDY   #$72
   STA   (FP),Y
   LDY   #$6D
   LDA   (FP),Y
   LDY   #$73
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   AND   #$0F
   INY
   STA   (FP),Y
   LDY   #$07
   LDA   (FP),Y
   ORA   #$01
   LDY   #$77
   STA   (FP),Y
   LDY   #$08
   LDA   (FP),Y
   LDY   #$78
   STA   (FP),Y
   LDY   #$09
   LDA   (FP),Y
   LDY   #$79
   STA   (FP),Y
   LDY   #$0A
   LDA   (FP),Y
   LDY   #$7A
   STA   (FP),Y
   LDY   #$0B
   LDA   (FP),Y
   LDY   #$7B
   STA   (FP),Y
   LDY   #$0C
   LDA   (FP),Y
   LDY   #$7C
   STA   (FP),Y
   LDA   #$00
   STA   HARGS
   LDY   #$00
   STY   HARGS+1
   STY   HARGS+2
   STY   HARGS+3
   STY   HARGS+4
   STY   HARGS+5
   STA   HARGS+6
   STA   HARGS+7
   LDY   #$77
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
   LDA   #$00
   STA   HARGS+14
   STA   HARGS+15
   JSR   udivmod64
   LDA   HARGS+24
   LDY   #$7D
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
   LDY   #$71
   LDA   (FP),Y
   LDY   #$7D
   AND   (FP),Y
   LDY   #$83
   STA   (FP),Y
   LDY   #$72
   LDA   (FP),Y
   LDY   #$7E
   AND   (FP),Y
   LDY   #$84
   STA   (FP),Y
   LDY   #$73
   LDA   (FP),Y
   LDY   #$7F
   AND   (FP),Y
   LDY   #$85
   STA   (FP),Y
   LDY   #$74
   LDA   (FP),Y
   LDY   #$80
   AND   (FP),Y
   LDY   #$86
   STA   (FP),Y
   LDY   #$75
   LDA   (FP),Y
   LDY   #$81
   AND   (FP),Y
   LDY   #$87
   STA   (FP),Y
   LDY   #$76
   LDA   (FP),Y
   LDY   #$82
   AND   (FP),Y
   LDY   #$88
   STA   (FP),Y
   LDY   #$83
   LDA   (FP),Y
   LDY   #$89
   STA   (FP),Y

   ; epilogue
   PHA
   CLC
   LDA   FP
   ADC   #$93
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

   ; prologue: 0 arg bytes, 35 local bytes
   SEC
   LDA   SSP
   SBC   #$25
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$24
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
   SBC   #$08
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   #$CD
   LDY   #$01
   STA   (SSP),Y
   LDA   #$00
   INY
   STA   (SSP),Y
   LDA   #$A2
   INY
   STA   (SSP),Y
   LDA   #$00
   INY
   STA   (SSP),Y
   INY
   STA   (SSP),Y
   INY
   STA   (SSP),Y
   LDA   #$8A
   INY
   STA   (SSP),Y
   LDA   #$00
   INY
   STA   (SSP),Y
   JSR   f0
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
   LDA   g0+2
   INY
   STA   (FP),Y
   LDA   g0+3
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
   LDY   #$0A
   LDA   (FP),Y
   LDY   #$0E
   EOR   (FP),Y
   LDY   #$12
   STA   (FP),Y
   LDY   #$0B
   LDA   (FP),Y
   LDY   #$0F
   EOR   (FP),Y
   LDY   #$13
   STA   (FP),Y
   LDY   #$0C
   LDA   (FP),Y
   LDY   #$10
   EOR   (FP),Y
   LDY   #$14
   STA   (FP),Y
   LDY   #$0D
   LDA   (FP),Y
   LDY   #$11
   EOR   (FP),Y
   LDY   #$15
   STA   (FP),Y
   LDY   #$06
   LDA   (FP),Y
   LDY   #$12
   EOR   (FP),Y
   LDY   #$16
   STA   (FP),Y
   LDY   #$07
   LDA   (FP),Y
   LDY   #$13
   EOR   (FP),Y
   LDY   #$17
   STA   (FP),Y
   LDY   #$08
   LDA   (FP),Y
   LDY   #$14
   EOR   (FP),Y
   LDY   #$18
   STA   (FP),Y
   LDY   #$09
   LDA   (FP),Y
   LDY   #$15
   EOR   (FP),Y
   LDY   #$19
   STA   (FP),Y
   LDY   #$16
   LDA   (FP),Y
   LDY   #$06
   STA   (FP),Y
   LDY   #$17
   LDA   (FP),Y
   LDY   #$07
   STA   (FP),Y
   LDY   #$18
   LDA   (FP),Y
   LDY   #$08
   STA   (FP),Y
   LDY   #$19
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
   LDY   #$1A
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
   LDY   #$1A
   EOR   (FP),Y
   LDY   #$1E
   STA   (FP),Y
   LDY   #$07
   LDA   (FP),Y
   LDY   #$1B
   EOR   (FP),Y
   LDY   #$1F
   STA   (FP),Y
   LDY   #$08
   LDA   (FP),Y
   LDY   #$1C
   EOR   (FP),Y
   LDY   #$20
   STA   (FP),Y
   LDY   #$09
   LDA   (FP),Y
   LDY   #$1D
   EOR   (FP),Y
   LDY   #$21
   STA   (FP),Y
   LDY   #$1E
   LDA   (FP),Y
   LDY   #$22
   STA   (FP),Y
   LDY   #$1F
   LDA   (FP),Y
   LDY   #$23
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
   ADC   #$25
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$24
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
   DC.B  $3F
   DS.B  5

g1:
   DC.L  $0000004B
