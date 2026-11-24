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

   ; prologue: 10 arg bytes, 68 local bytes
   SEC
   LDA   SSP
   SBC   #$46
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$45
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
   LDY   #$48
   LDA   (FP),Y
   LDY   #$02
   STA   (FP),Y
   LDY   #$49
   LDA   (FP),Y
   LDY   #$03
   STA   (FP),Y
   LDY   #$4A
   LDA   (FP),Y
   LDY   #$04
   STA   (FP),Y
   LDY   #$01
   LDA   (FP),Y
   SEC
   SBC   g1
   INY
   LDA   (FP),Y
   SBC   g1+1
   INY
   LDA   (FP),Y
   SBC   g1+2
   INY
   LDA   (FP),Y
   SBC   g1+3
   LDA   #$00
   ROL
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDA   g0
   INY
   STA   (FP),Y
   LDA   g0
   BMI   .sx_neg@1
   LDA   #$00
   JMP   .sx_done@2
.sx_neg@1:
   LDA   #$FF
.sx_done@2:
   LDY   #$08
   STA   (FP),Y
   LDY   #$05
   LDA   (FP),Y
   SEC
   LDY   #$07
   SBC   (FP),Y
   LDY   #$09
   STA   (FP),Y
   LDY   #$06
   LDA   (FP),Y
   LDY   #$08
   SBC   (FP),Y
   LDY   #$0A
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$0B
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$0C
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$0D
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$0E
   STA   (FP),Y
   LDY   #$4D
   LDA   (FP),Y
   LDY   #$0F
   STA   (FP),Y
   LDY   #$4E
   LDA   (FP),Y
   LDY   #$10
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@3
   LDA   #$00
   JMP   .sx_done@4
.sx_neg@3:
   LDA   #$FF
.sx_done@4:
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
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$13
   LDA   (FP),Y
   LDY   #$19
   STA   (FP),Y
   LDY   #$14
   LDA   (FP),Y
   LDY   #$1A
   STA   (FP),Y
   LDY   #$15
   LDA   (FP),Y
   LDY   #$1B
   STA   (FP),Y
   LDY   #$16
   LDA   (FP),Y
   LDY   #$1C
   STA   (FP),Y
   LDY   #$17
   LDA   (FP),Y
   LDY   #$1D
   STA   (FP),Y
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
   LDY   #$4D
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$4E
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   JSR   rec
   LDY   #$1F
   STA   (FP),Y
   TXA
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
   BMI   .sx_neg@5
   LDA   #$00
   JMP   .sx_done@6
.sx_neg@5:
   LDA   #$FF
.sx_done@6:
   LDY   #$23
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$19
   LDA   (FP),Y
   CLC
   LDY   #$21
   ADC   (FP),Y
   LDY   #$27
   STA   (FP),Y
   LDY   #$1A
   LDA   (FP),Y
   LDY   #$22
   ADC   (FP),Y
   LDY   #$28
   STA   (FP),Y
   LDY   #$1B
   LDA   (FP),Y
   LDY   #$23
   ADC   (FP),Y
   LDY   #$29
   STA   (FP),Y
   LDY   #$1C
   LDA   (FP),Y
   LDY   #$24
   ADC   (FP),Y
   LDY   #$2A
   STA   (FP),Y
   LDY   #$1D
   LDA   (FP),Y
   LDY   #$25
   ADC   (FP),Y
   LDY   #$2B
   STA   (FP),Y
   LDY   #$1E
   LDA   (FP),Y
   LDY   #$26
   ADC   (FP),Y
   LDY   #$2C
   STA   (FP),Y
   LDY   #$27
   LDA   (FP),Y
   LDY   #$2D
   STA   (FP),Y
   LDY   #$28
   LDA   (FP),Y
   LDY   #$2E
   STA   (FP),Y
   LDY   #$29
   LDA   (FP),Y
   LDY   #$2F
   STA   (FP),Y
   LDY   #$2A
   LDA   (FP),Y
   LDY   #$30
   STA   (FP),Y
   LDY   #$2B
   LDA   (FP),Y
   LDY   #$31
   STA   (FP),Y
   LDY   #$2C
   LDA   (FP),Y
   AND   #$01
   LDY   #$32
   STA   (FP),Y
   LDY   #$2D
   LDA   (FP),Y
   LDY   #$33
   STA   (FP),Y
   LDY   #$2E
   LDA   (FP),Y
   LDY   #$34
   STA   (FP),Y
   LDY   #$2F
   LDA   (FP),Y
   LDY   #$35
   STA   (FP),Y
   LDY   #$30
   LDA   (FP),Y
   LDY   #$36
   STA   (FP),Y
   LDY   #$31
   LDA   (FP),Y
   LDY   #$37
   STA   (FP),Y
   LDY   #$32
   LDA   (FP),Y
   EOR   #$01
   LDY   #$38
   STA   (FP),Y
   LDY   #$33
   LDA   (FP),Y
   LDY   #$39
   STA   (FP),Y
   LDY   #$34
   LDA   (FP),Y
   LDY   #$3A
   STA   (FP),Y
   LDY   #$35
   LDA   (FP),Y
   LDY   #$3B
   STA   (FP),Y
   LDY   #$36
   LDA   (FP),Y
   LDY   #$3C
   STA   (FP),Y
   LDY   #$37
   LDA   (FP),Y
   LDY   #$3D
   STA   (FP),Y
   LDY   #$38
   LDA   (FP),Y
   SEC
   SBC   #$01
   LDY   #$3E
   STA   (FP),Y
   LDY   #$39
   LDA   (FP),Y
   LDY   #$19
   STA   (FP),Y
   LDY   #$3A
   LDA   (FP),Y
   LDY   #$1A
   STA   (FP),Y
   LDY   #$3B
   LDA   (FP),Y
   LDY   #$1B
   STA   (FP),Y
   LDY   #$3C
   LDA   (FP),Y
   LDY   #$1C
   STA   (FP),Y
   LDY   #$3D
   LDA   (FP),Y
   LDY   #$1D
   STA   (FP),Y
   LDY   #$3E
   LDA   (FP),Y
   LDY   #$1E
   STA   (FP),Y
   LDA   g0
   LDY   #$3F
   STA   (FP),Y
   LDA   g0
   BMI   .sx_neg@7
   LDA   #$00
   JMP   .sx_done@8
.sx_neg@7:
   LDA   #$FF
.sx_done@8:
   LDY   #$40
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$3F
   LDA   (FP),Y
   STA   HARGS+16
   INY
   LDA   (FP),Y
   STA   HARGS+17
   INY
   LDA   (FP),Y
   STA   HARGS+18
   INY
   LDA   (FP),Y
   STA   HARGS+19
   INY
   LDA   (FP),Y
   STA   HARGS+20
   INY
   LDA   (FP),Y
   STA   HARGS+21

   ; epilogue
   CLC
   LDA   FP
   ADC   #$50
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

   ; prologue: 21 arg bytes, 104 local bytes
   SEC
   LDA   SSP
   SBC   #$6A
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$69
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   LDY   #$6F
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   g1
   SEC
   SBC   g1
   INY
   STA   (FP),Y
   LDA   g1+1
   SBC   g1+1
   INY
   STA   (FP),Y
   LDA   g1+2
   SBC   g1+2
   INY
   STA   (FP),Y
   LDA   g1+3
   SBC   g1+3
   INY
   STA   (FP),Y
   LDY   #$03
   LDA   (FP),Y
   LDY   #$07
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@9
   LDA   #$00
   JMP   .sx_done@10
.sx_neg@9:
   LDA   #$FF
.sx_done@10:
   LDY   #$0A
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$0B
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$0C
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@11
   LDA   #$00
   JMP   .sx_done@12
.sx_neg@11:
   LDA   #$FF
.sx_done@12:
   LDY   #$0D
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
   INY
   LDA   (FP),Y
   STA   HARGS+14
   INY
   LDA   (FP),Y
   STA   HARGS+15
   JSR   mul64
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
   LDA   HARGS+22
   INY
   STA   (FP),Y
   LDA   HARGS+23
   INY
   STA   (FP),Y
   LDY   #$13
   LDA   (FP),Y
   LDY   #$1B
   STA   (FP),Y
   LDY   #$14
   LDA   (FP),Y
   LDY   #$1C
   STA   (FP),Y
   LDY   #$15
   LDA   (FP),Y
   LDY   #$1D
   STA   (FP),Y
   LDY   #$16
   LDA   (FP),Y
   LDY   #$1E
   STA   (FP),Y
   LDY   #$17
   LDA   (FP),Y
   LDY   #$1F
   STA   (FP),Y
   LDY   #$18
   LDA   (FP),Y
   LDY   #$20
   STA   (FP),Y
   LDY   #$19
   LDA   (FP),Y
   LDY   #$21
   STA   (FP),Y
   LDY   #$1A
   LDA   (FP),Y
   AND   #$3F
   LDY   #$22
   STA   (FP),Y
   LDY   #$1B
   LDA   (FP),Y
   LDY   #$23
   STA   (FP),Y
   LDY   #$1C
   LDA   (FP),Y
   LDY   #$24
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
   LDY   #$22
   LDA   (FP),Y
   EOR   #$20
   LDY   #$2A
   STA   (FP),Y
   LDY   #$23
   LDA   (FP),Y
   LDY   #$2B
   STA   (FP),Y
   LDY   #$24
   LDA   (FP),Y
   LDY   #$2C
   STA   (FP),Y
   LDY   #$25
   LDA   (FP),Y
   LDY   #$2D
   STA   (FP),Y
   LDY   #$26
   LDA   (FP),Y
   LDY   #$2E
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
   SEC
   SBC   #$20
   LDY   #$32
   STA   (FP),Y
   LDY   #$2B
   LDA   (FP),Y
   LDY   #$33
   STA   (FP),Y
   LDY   #$2C
   LDA   (FP),Y
   LDY   #$34
   STA   (FP),Y
   LDY   #$2D
   LDA   (FP),Y
   LDY   #$35
   STA   (FP),Y
   LDY   #$2E
   LDA   (FP),Y
   LDY   #$36
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   LDY   #$37
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@13
   LDA   #$00
   JMP   .sx_done@14
.sx_neg@13:
   LDA   #$FF
.sx_done@14:
   LDY   #$38
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
   BMI   .sx_neg@15
   LDA   #$00
   JMP   .sx_done@16
.sx_neg@15:
   LDA   #$FF
.sx_done@16:
   LDY   #$3B
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
   LDY   #$77
   LDA   (FP),Y
   LDY   #$39
   EOR   (FP),Y
   LDY   #$41
   STA   (FP),Y
   LDY   #$78
   LDA   (FP),Y
   LDY   #$3A
   EOR   (FP),Y
   LDY   #$42
   STA   (FP),Y
   LDY   #$79
   LDA   (FP),Y
   LDY   #$3B
   EOR   (FP),Y
   LDY   #$43
   STA   (FP),Y
   LDY   #$7A
   LDA   (FP),Y
   LDY   #$3C
   EOR   (FP),Y
   LDY   #$44
   STA   (FP),Y
   LDY   #$7B
   LDA   (FP),Y
   LDY   #$3D
   EOR   (FP),Y
   LDY   #$45
   STA   (FP),Y
   LDY   #$7C
   LDA   (FP),Y
   LDY   #$3E
   EOR   (FP),Y
   LDY   #$46
   STA   (FP),Y
   LDY   #$7D
   LDA   (FP),Y
   LDY   #$3F
   EOR   (FP),Y
   LDY   #$47
   STA   (FP),Y
   LDY   #$7E
   LDA   (FP),Y
   LDY   #$40
   EOR   (FP),Y
   LDY   #$48
   STA   (FP),Y
   LDY   #$41
   LDA   (FP),Y
   LDY   #$49
   STA   (FP),Y
   LDY   #$42
   LDA   (FP),Y
   LDY   #$4A
   STA   (FP),Y
   SEC
   LDA   #$00
   LDY   #$6B
   SBC   (FP),Y
   LDY   #$4B
   STA   (FP),Y
   LDA   #$00
   LDY   #$6C
   SBC   (FP),Y
   LDY   #$4C
   STA   (FP),Y
   LDA   #$00
   LDY   #$6D
   SBC   (FP),Y
   LDY   #$4D
   STA   (FP),Y
   LDA   #$00
   LDY   #$6E
   SBC   (FP),Y
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
   LDY   #$08
   LDA   (FP),Y
   LDY   #$51
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$0A
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDY   #$33
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$34
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$35
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$36
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   LDY   #$49
   LDA   (FP),Y
   LDY   #$05
   STA   (SSP),Y
   LDY   #$4A
   LDA   (FP),Y
   LDY   #$06
   STA   (SSP),Y
   LDY   #$4F
   LDA   (FP),Y
   LDY   #$07
   STA   (SSP),Y
   LDY   #$50
   LDA   (FP),Y
   LDY   #$08
   STA   (SSP),Y
   LDY   #$52
   LDA   (FP),Y
   LDY   #$09
   STA   (SSP),Y
   LDY   #$53
   LDA   (FP),Y
   LDY   #$0A
   STA   (SSP),Y
   JSR   f2
   LDA   HARGS+16
   LDY   #$54
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
   LDY   #$02
   LDA   (FP),Y
   LDY   #$5A
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@17
   LDA   #$00
   JMP   .sx_done@18
.sx_neg@17:
   LDA   #$FF
.sx_done@18:
   LDY   #$5B
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$5A
   LDA   (FP),Y
   LDY   #$54
   EOR   (FP),Y
   LDY   #$60
   STA   (FP),Y
   LDY   #$5B
   LDA   (FP),Y
   LDY   #$55
   EOR   (FP),Y
   LDY   #$61
   STA   (FP),Y
   LDY   #$5C
   LDA   (FP),Y
   LDY   #$56
   EOR   (FP),Y
   LDY   #$62
   STA   (FP),Y
   LDY   #$5D
   LDA   (FP),Y
   LDY   #$57
   EOR   (FP),Y
   LDY   #$63
   STA   (FP),Y
   LDY   #$5E
   LDA   (FP),Y
   LDY   #$58
   EOR   (FP),Y
   LDY   #$64
   STA   (FP),Y
   LDY   #$5F
   LDA   (FP),Y
   LDY   #$59
   EOR   (FP),Y
   LDY   #$65
   STA   (FP),Y
   LDY   #$60
   LDA   (FP),Y
   LDY   #$66
   STA   (FP),Y
   LDY   #$02
   STA   (FP),Y
   LDA   g1
   LDY   #$67
   STA   (FP),Y
   LDA   g1+1
   INY
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
   ADC   #$7F
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$69
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

   ; prologue: 0 arg bytes, 169 local bytes
   SEC
   LDA   SSP
   SBC   #$AB
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$AA
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   LDA   g0
   BEQ   .and_false@1
   LDA   #$EB
   BEQ   .and_false@1
   LDA   #$01
   LDY   #$01
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   JMP   .and_end@2
.and_false@1:
   LDA   #$00
   LDY   #$01
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@2:
   LDY   #$01
   LDA   (FP),Y
   LDY   #$03
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$04
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@19
   LDA   #$00
   JMP   .sx_done@20
.sx_neg@19:
   LDA   #$FF
.sx_done@20:
   LDY   #$05
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   g1
   SEC
   LDY   #$03
   SBC   (FP),Y
   LDA   g1+1
   INY
   SBC   (FP),Y
   LDA   g1+2
   INY
   SBC   (FP),Y
   LDA   g1+3
   INY
   SBC   (FP),Y
   LDA   #$00
   ROL
   INY
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
   BMI   .sx_neg@21
   LDA   #$00
   JMP   .sx_done@22
.sx_neg@21:
   LDA   #$FF
.sx_done@22:
   LDY   #$0B
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$09
   LDA   (FP),Y
   LDY   #$0F
   STA   (FP),Y
   LDY   #$0A
   LDA   (FP),Y
   LDY   #$10
   STA   (FP),Y
   LDY   #$0B
   LDA   (FP),Y
   LDY   #$11
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   g0
   INY
   STA   (FP),Y
   LDA   g0
   BMI   .sx_neg@23
   LDA   #$00
   JMP   .sx_done@24
.sx_neg@23:
   LDA   #$FF
.sx_done@24:
   LDY   #$16
   STA   (FP),Y
   LDA   g0
   INY
   STA   (FP),Y
   LDA   g0
   BMI   .sx_neg@25
   LDA   #$00
   JMP   .sx_done@26
.sx_neg@25:
   LDA   #$FF
.sx_done@26:
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
   BMI   .sx_neg@27
   LDA   #$00
   JMP   .sx_done@28
.sx_neg@27:
   LDA   #$FF
.sx_done@28:
   LDY   #$1B
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$14
   CMP   (FP),Y
   BNE   .thread@0
   DEY
   CMP   (FP),Y
   BNE   .thread@0
   DEY
   CMP   (FP),Y
   BNE   .thread@0
   DEY
   CMP   (FP),Y
   BNE   .thread@0
   LDY   #$1A
   LDA   (FP),Y
   LDY   #$10
   CMP   (FP),Y
   BNE   .thread@0
   LDY   #$19
   LDA   (FP),Y
   LDY   #$0F
   CMP   (FP),Y
   BEQ   .cmp_true@29
.thread@0:
   LDA   #$00
   JMP   .cmp_end@30
.cmp_true@29:
   LDA   #$01
.cmp_end@30:
   LDY   #$1F
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$15
   LDA   (FP),Y
   LDY   #$1F
   ORA   (FP),Y
   LDY   #$21
   STA   (FP),Y
   LDY   #$16
   LDA   (FP),Y
   LDY   #$20
   ORA   (FP),Y
   LDY   #$22
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$23
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$64
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   g1
   ORA   g1+1
   ORA   g1+2
   ORA   g1+3
   BNE   .or_true@3
   LDA   g0
   BNE   .or_true@3
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   JMP   .or_end@4
.or_true@3:
   LDA   #$01
   LDY   #$27
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@4:
   LDY   #$27
   LDA   (FP),Y
   LDY   #$29
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$2A
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@32
   LDA   #$00
   JMP   .sx_done@33
.sx_neg@32:
   LDA   #$FF
.sx_done@33:
   LDY   #$2B
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$26
   LDA   (FP),Y
   LDY   #$2D
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@34
   LDA   #$00
   JMP   .sx_done@35
.sx_neg@34:
   LDA   #$FF
.sx_done@35:
   LDY   #$2E
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$2F
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$30
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@36
   LDA   #$00
   JMP   .sx_done@37
.sx_neg@36:
   LDA   #$FF
.sx_done@37:
   LDY   #$31
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$14
   LDA   (FP),Y
   LDY   #$34
   CMP   (FP),Y
   BNE   .cmp_true@38
   DEY
   CMP   (FP),Y
   BNE   .cmp_true@38
   DEY
   CMP   (FP),Y
   BNE   .cmp_true@38
   DEY
   CMP   (FP),Y
   BNE   .cmp_true@38
   LDY   #$10
   LDA   (FP),Y
   LDY   #$30
   CMP   (FP),Y
   BNE   .cmp_true@38
   LDY   #$0F
   LDA   (FP),Y
   LDY   #$2F
   CMP   (FP),Y
   BNE   .cmp_true@38
   LDA   #$00
   JMP   .cmp_end@39
.cmp_true@38:
   LDA   #$01
.cmp_end@39:
   LDY   #$35
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDA   #$E8
   CLC
   ADC   #$02
   INY
   STA   (FP),Y
   LDA   #$03
   ADC   #$00
   INY
   STA   (FP),Y
   LDY   #$26
   LDA   (FP),Y
   LDY   #$39
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@41
   LDA   #$00
   JMP   .sx_done@42
.sx_neg@41:
   LDA   #$FF
.sx_done@42:
   LDY   #$3A
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$0A
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDY   #$29
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$2A
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$2B
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$2C
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   LDY   #$35
   LDA   (FP),Y
   LDY   #$05
   STA   (SSP),Y
   LDY   #$36
   LDA   (FP),Y
   LDY   #$06
   STA   (SSP),Y
   LDY   #$37
   LDA   (FP),Y
   LDY   #$07
   STA   (SSP),Y
   LDY   #$38
   LDA   (FP),Y
   LDY   #$08
   STA   (SSP),Y
   LDY   #$39
   LDA   (FP),Y
   LDY   #$09
   STA   (SSP),Y
   LDY   #$3A
   LDA   (FP),Y
   LDY   #$0A
   STA   (SSP),Y
   JSR   f2
   LDA   HARGS+16
   LDY   #$3B
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
   LDY   #$0F
   LDA   (FP),Y
   LDY   #$3B
   EOR   (FP),Y
   LDY   #$41
   STA   (FP),Y
   LDY   #$10
   LDA   (FP),Y
   LDY   #$3C
   EOR   (FP),Y
   LDY   #$42
   STA   (FP),Y
   LDY   #$11
   LDA   (FP),Y
   LDY   #$3D
   EOR   (FP),Y
   LDY   #$43
   STA   (FP),Y
   LDY   #$12
   LDA   (FP),Y
   LDY   #$3E
   EOR   (FP),Y
   LDY   #$44
   STA   (FP),Y
   LDY   #$13
   LDA   (FP),Y
   LDY   #$3F
   EOR   (FP),Y
   LDY   #$45
   STA   (FP),Y
   LDY   #$14
   LDA   (FP),Y
   LDY   #$40
   EOR   (FP),Y
   LDY   #$46
   STA   (FP),Y
   LDY   #$41
   LDA   (FP),Y
   LDY   #$0F
   STA   (FP),Y
   LDY   #$42
   LDA   (FP),Y
   LDY   #$10
   STA   (FP),Y
   LDY   #$43
   LDA   (FP),Y
   LDY   #$11
   STA   (FP),Y
   LDY   #$44
   LDA   (FP),Y
   LDY   #$12
   STA   (FP),Y
   LDY   #$45
   LDA   (FP),Y
   LDY   #$13
   STA   (FP),Y
   LDY   #$46
   LDA   (FP),Y
   LDY   #$14
   STA   (FP),Y
   LDA   #$00
   LDY   #$47
   STA   (FP),Y
   INY
   STA   (FP),Y
.loop@0_start:
   LDY   #$47
   LDA   (FP),Y
   SEC
   SBC   #$03
   INY
   LDA   (FP),Y
   SBC   #$00
   BVC   .cmp_novf@43
   EOR   #$80
.cmp_novf@43:
   ASL
   LDA   #$00
   ROL
   LDY   #$49
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@0
   JMP   .loop@0_break
.lb_skip@0:
   LDY   #$24
   LDA   (FP),Y
   LDY   #$4B
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$26
   LDA   (FP),Y
   LDY   #$4F
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@44
   LDA   #$00
   JMP   .sx_done@45
.sx_neg@44:
   LDA   #$FF
.sx_done@45:
   LDY   #$50
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ORA   #$03
   LDY   #$51
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$52
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$53
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$54
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@46
   LDA   #$00
   JMP   .sx_done@47
.sx_neg@46:
   LDA   #$FF
.sx_done@47:
   LDY   #$55
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
   LDY   #$0F
   LDA   (FP),Y
   EOR   #$FF
   LDY   #$5B
   STA   (FP),Y
   LDY   #$10
   LDA   (FP),Y
   EOR   #$FF
   LDY   #$5C
   STA   (FP),Y
   LDY   #$11
   LDA   (FP),Y
   EOR   #$FF
   LDY   #$5D
   STA   (FP),Y
   LDY   #$12
   LDA   (FP),Y
   EOR   #$FF
   LDY   #$5E
   STA   (FP),Y
   LDY   #$13
   LDA   (FP),Y
   EOR   #$FF
   LDY   #$5F
   STA   (FP),Y
   LDY   #$14
   LDA   (FP),Y
   EOR   #$FF
   LDY   #$60
   STA   (FP),Y
   LDY   #$5B
   LDA   (FP),Y
   LDY   #$61
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$15
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDY   #$4B
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$4C
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$4D
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$4E
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   LDA   #$64
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
   INY
   STA   (SSP),Y
   LDY   #$53
   LDA   (FP),Y
   LDY   #$0D
   STA   (SSP),Y
   LDY   #$54
   LDA   (FP),Y
   LDY   #$0E
   STA   (SSP),Y
   LDY   #$55
   LDA   (FP),Y
   LDY   #$0F
   STA   (SSP),Y
   LDY   #$56
   LDA   (FP),Y
   LDY   #$10
   STA   (SSP),Y
   LDY   #$57
   LDA   (FP),Y
   LDY   #$11
   STA   (SSP),Y
   LDY   #$58
   LDA   (FP),Y
   LDY   #$12
   STA   (SSP),Y
   LDY   #$59
   LDA   (FP),Y
   LDY   #$13
   STA   (SSP),Y
   LDY   #$5A
   LDA   (FP),Y
   LDY   #$14
   STA   (SSP),Y
   LDY   #$61
   LDA   (FP),Y
   LDY   #$15
   STA   (SSP),Y
   JSR   f1
   LDY   #$62
   STA   (FP),Y
   TXA
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   LDY   #$47
   ADC   (FP),Y
   LDY   #$64
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$48
   ADC   (FP),Y
   LDY   #$65
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$66
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$67
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@48
   LDA   #$00
   JMP   .sx_done@49
.sx_neg@48:
   LDA   #$FF
.sx_done@49:
   LDY   #$68
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$0F
   LDA   (FP),Y
   CLC
   LDY   #$66
   ADC   (FP),Y
   LDY   #$6C
   STA   (FP),Y
   LDY   #$10
   LDA   (FP),Y
   LDY   #$67
   ADC   (FP),Y
   LDY   #$6D
   STA   (FP),Y
   LDY   #$11
   LDA   (FP),Y
   LDY   #$68
   ADC   (FP),Y
   LDY   #$6E
   STA   (FP),Y
   LDY   #$12
   LDA   (FP),Y
   LDY   #$69
   ADC   (FP),Y
   LDY   #$6F
   STA   (FP),Y
   LDY   #$13
   LDA   (FP),Y
   LDY   #$6A
   ADC   (FP),Y
   LDY   #$70
   STA   (FP),Y
   LDY   #$14
   LDA   (FP),Y
   LDY   #$6B
   ADC   (FP),Y
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
   LDY   #$6E
   LDA   (FP),Y
   LDY   #$74
   STA   (FP),Y
   LDY   #$6F
   LDA   (FP),Y
   LDY   #$75
   STA   (FP),Y
   LDY   #$70
   LDA   (FP),Y
   LDY   #$76
   STA   (FP),Y
   LDY   #$71
   LDA   (FP),Y
   AND   #$01
   LDY   #$77
   STA   (FP),Y
   LDY   #$72
   LDA   (FP),Y
   LDY   #$78
   STA   (FP),Y
   LDY   #$73
   LDA   (FP),Y
   LDY   #$79
   STA   (FP),Y
   LDY   #$74
   LDA   (FP),Y
   LDY   #$7A
   STA   (FP),Y
   LDY   #$75
   LDA   (FP),Y
   LDY   #$7B
   STA   (FP),Y
   LDY   #$76
   LDA   (FP),Y
   LDY   #$7C
   STA   (FP),Y
   LDY   #$77
   LDA   (FP),Y
   EOR   #$01
   LDY   #$7D
   STA   (FP),Y
   LDY   #$78
   LDA   (FP),Y
   LDY   #$7E
   STA   (FP),Y
   LDY   #$79
   LDA   (FP),Y
   LDY   #$7F
   STA   (FP),Y
   LDY   #$7A
   LDA   (FP),Y
   LDY   #$80
   STA   (FP),Y
   LDY   #$7B
   LDA   (FP),Y
   LDY   #$81
   STA   (FP),Y
   LDY   #$7C
   LDA   (FP),Y
   LDY   #$82
   STA   (FP),Y
   LDY   #$7D
   LDA   (FP),Y
   SEC
   SBC   #$01
   LDY   #$83
   STA   (FP),Y
   LDY   #$7E
   LDA   (FP),Y
   LDY   #$0F
   STA   (FP),Y
   LDY   #$7F
   LDA   (FP),Y
   LDY   #$10
   STA   (FP),Y
   LDY   #$80
   LDA   (FP),Y
   LDY   #$11
   STA   (FP),Y
   LDY   #$81
   LDA   (FP),Y
   LDY   #$12
   STA   (FP),Y
   LDY   #$82
   LDA   (FP),Y
   LDY   #$13
   STA   (FP),Y
   LDY   #$83
   LDA   (FP),Y
   LDY   #$14
   STA   (FP),Y
   LDY   #$47
   LDA   (FP),Y
   LDY   #$84
   STA   (FP),Y
   LDY   #$48
   LDA   (FP),Y
   LDY   #$85
   STA   (FP),Y
   LDY   #$47
   LDA   (FP),Y
   CLC
   ADC   #$01
   LDY   #$86
   STA   (FP),Y
   LDY   #$48
   LDA   (FP),Y
   ADC   #$00
   LDY   #$87
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$47
   STA   (FP),Y
   LDY   #$87
   LDA   (FP),Y
   LDY   #$48
   STA   (FP),Y
   JMP   .loop@0_start
.loop@0_break:
   LDA   g1
   LDY   #$88
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
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$88
   LDA   (FP),Y
   CLC
   LDY   #$0F
   ADC   (FP),Y
   LDY   #$8E
   STA   (FP),Y
   LDY   #$89
   LDA   (FP),Y
   LDY   #$10
   ADC   (FP),Y
   LDY   #$8F
   STA   (FP),Y
   LDY   #$8A
   LDA   (FP),Y
   LDY   #$11
   ADC   (FP),Y
   LDY   #$90
   STA   (FP),Y
   LDY   #$8B
   LDA   (FP),Y
   LDY   #$12
   ADC   (FP),Y
   LDY   #$91
   STA   (FP),Y
   LDY   #$8C
   LDA   (FP),Y
   LDY   #$13
   ADC   (FP),Y
   LDY   #$92
   STA   (FP),Y
   LDY   #$8D
   LDA   (FP),Y
   LDY   #$14
   ADC   (FP),Y
   LDY   #$93
   STA   (FP),Y
   LDY   #$8E
   LDA   (FP),Y
   LDY   #$94
   STA   (FP),Y
   LDY   #$8F
   LDA   (FP),Y
   LDY   #$95
   STA   (FP),Y
   LDY   #$90
   LDA   (FP),Y
   LDY   #$96
   STA   (FP),Y
   LDY   #$91
   LDA   (FP),Y
   LDY   #$97
   STA   (FP),Y
   LDY   #$92
   LDA   (FP),Y
   LDY   #$98
   STA   (FP),Y
   LDY   #$93
   LDA   (FP),Y
   AND   #$01
   LDY   #$99
   STA   (FP),Y
   LDY   #$94
   LDA   (FP),Y
   LDY   #$9A
   STA   (FP),Y
   LDY   #$95
   LDA   (FP),Y
   LDY   #$9B
   STA   (FP),Y
   LDY   #$96
   LDA   (FP),Y
   LDY   #$9C
   STA   (FP),Y
   LDY   #$97
   LDA   (FP),Y
   LDY   #$9D
   STA   (FP),Y
   LDY   #$98
   LDA   (FP),Y
   LDY   #$9E
   STA   (FP),Y
   LDY   #$99
   LDA   (FP),Y
   EOR   #$01
   LDY   #$9F
   STA   (FP),Y
   LDY   #$9A
   LDA   (FP),Y
   LDY   #$A0
   STA   (FP),Y
   LDY   #$9B
   LDA   (FP),Y
   LDY   #$A1
   STA   (FP),Y
   LDY   #$9C
   LDA   (FP),Y
   LDY   #$A2
   STA   (FP),Y
   LDY   #$9D
   LDA   (FP),Y
   LDY   #$A3
   STA   (FP),Y
   LDY   #$9E
   LDA   (FP),Y
   LDY   #$A4
   STA   (FP),Y
   LDY   #$9F
   LDA   (FP),Y
   SEC
   SBC   #$01
   LDY   #$A5
   STA   (FP),Y
   LDY   #$A0
   LDA   (FP),Y
   LDY   #$A6
   STA   (FP),Y
   LDY   #$A1
   LDA   (FP),Y
   LDY   #$A7
   STA   (FP),Y
   LDY   #$A2
   LDA   (FP),Y
   LDY   #$A8
   STA   (FP),Y
   LDY   #$A3
   LDA   (FP),Y
   LDY   #$A9
   STA   (FP),Y
   LDY   #$A6
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
   STA   HARGS+16
   LDA   #$00
   STA   HARGS+17
   LDY   #$00
   STY   HARGS+18
   STY   HARGS+19
   STY   HARGS+20
   STY   HARGS+21
   STY   HARGS+22
   STA   HARGS+23

   ; epilogue
   CLC
   LDA   FP
   ADC   #$AB
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$AA
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   RTS

main:
   SUBROUTINE

   ; prologue: 0 arg bytes, 42 local bytes
   SEC
   LDA   SSP
   SBC   #$2C
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$2B
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   JSR   f0
   LDA   HARGS+16
   LDY   #$01
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
   LDA   HARGS+22
   INY
   STA   (FP),Y
   LDA   HARGS+23
   INY
   STA   (FP),Y
   LDY   #$01
   LDA   (FP),Y
   LDY   #$09
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   LDY   #$0A
   STA   (FP),Y
   LDY   #$03
   LDA   (FP),Y
   LDY   #$0B
   STA   (FP),Y
   LDY   #$04
   LDA   (FP),Y
   LDY   #$0C
   STA   (FP),Y
   LDY   #$09
   LDA   (FP),Y
   LDY   #$0D
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
   LDA   g0
   INY
   STA   (FP),Y
   LDA   g0
   BMI   .sx_neg@50
   LDA   #$00
   JMP   .sx_done@51
.sx_neg@50:
   LDA   #$FF
.sx_done@51:
   LDY   #$12
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
   LDY   #$11
   LDA   (FP),Y
   LDY   #$15
   EOR   (FP),Y
   LDY   #$19
   STA   (FP),Y
   LDY   #$12
   LDA   (FP),Y
   LDY   #$16
   EOR   (FP),Y
   LDY   #$1A
   STA   (FP),Y
   LDY   #$13
   LDA   (FP),Y
   LDY   #$17
   EOR   (FP),Y
   LDY   #$1B
   STA   (FP),Y
   LDY   #$14
   LDA   (FP),Y
   LDY   #$18
   EOR   (FP),Y
   LDY   #$1C
   STA   (FP),Y
   LDY   #$0D
   LDA   (FP),Y
   LDY   #$19
   EOR   (FP),Y
   LDY   #$1D
   STA   (FP),Y
   LDY   #$0E
   LDA   (FP),Y
   LDY   #$1A
   EOR   (FP),Y
   LDY   #$1E
   STA   (FP),Y
   LDY   #$0F
   LDA   (FP),Y
   LDY   #$1B
   EOR   (FP),Y
   LDY   #$1F
   STA   (FP),Y
   LDY   #$10
   LDA   (FP),Y
   LDY   #$1C
   EOR   (FP),Y
   LDY   #$20
   STA   (FP),Y
   LDY   #$1D
   LDA   (FP),Y
   LDY   #$0D
   STA   (FP),Y
   LDY   #$1E
   LDA   (FP),Y
   LDY   #$0E
   STA   (FP),Y
   LDY   #$1F
   LDA   (FP),Y
   LDY   #$0F
   STA   (FP),Y
   LDY   #$20
   LDA   (FP),Y
   LDY   #$10
   STA   (FP),Y
   LDY   #$0D
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
   LDY   #$21
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
   LDY   #$0D
   LDA   (FP),Y
   LDY   #$21
   EOR   (FP),Y
   LDY   #$25
   STA   (FP),Y
   LDY   #$0E
   LDA   (FP),Y
   LDY   #$22
   EOR   (FP),Y
   LDY   #$26
   STA   (FP),Y
   LDY   #$0F
   LDA   (FP),Y
   LDY   #$23
   EOR   (FP),Y
   LDY   #$27
   STA   (FP),Y
   LDY   #$10
   LDA   (FP),Y
   LDY   #$24
   EOR   (FP),Y
   LDY   #$28
   STA   (FP),Y
   LDY   #$25
   LDA   (FP),Y
   LDY   #$29
   STA   (FP),Y
   LDY   #$26
   LDA   (FP),Y
   LDY   #$2A
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
   ADC   #$2C
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$2B
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
   DC.B  $39

g1:
   DC.L  $0000007A
