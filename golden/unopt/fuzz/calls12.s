SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24

rec:
   SUBROUTINE

   ; prologue: 4 arg bytes, 54 local bytes
   SEC
   LDA   SSP
   SBC   #$38
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$37
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   LDY   #$3B
   LDA   (FP),Y
   STA   HARGS
   INY
   LDA   (FP),Y
   STA   HARGS+1
   LDA   #$07
   STA   HARGS+2
   LDA   #$00
   STA   HARGS+3
   JSR   mul16
   LDA   HARGS+4
   LDY   #$01
   STA   (FP),Y
   LDA   HARGS+5
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$03
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$3B
   LDA   (FP),Y
   LDY   #$05
   STA   (FP),Y
   LDY   #$3C
   LDA   (FP),Y
   LDY   #$06
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@0
   LDA   #$00
   JMP   .sx_done@1
.sx_neg@0:
   LDA   #$FF
.sx_done@1:
   LDY   #$07
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$03
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   ORA   #$00
   BPL   .sx_done@3
   LDA   #$FF
.sx_done@3:
   LDY   #$0B
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$05
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
   JSR   mul32
   LDA   HARGS+8
   LDY   #$0D
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
   LDY   #$10
   LDA   (FP),Y
   LDY   #$14
   STA   (FP),Y
   LDA   #$00
   SEC
   LDY   #$39
   SBC   (FP),Y
   LDA   #$00
   INY
   SBC   (FP),Y
   BVC   .cmp_novf@4
   EOR   #$80
.cmp_novf@4:
   ASL
   LDA   #$00
   ROL
   EOR   #$01
   LDY   #$15
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@0
   LDY   #$3B
   LDA   (FP),Y
   LDY   #$17
   STA   (FP),Y
   LDY   #$3C
   LDA   (FP),Y
   LDA   #$00
   LDY   #$18
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
   ADC   #$3C
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$37
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
   LDY   #$39
   LDA   (FP),Y
   SEC
   SBC   #$01
   LDY   #$19
   STA   (FP),Y
   LDY   #$3A
   LDA   (FP),Y
   SBC   #$00
   LDY   #$1A
   STA   (FP),Y
   LDY   #$3B
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
   LDY   #$1B
   STA   (FP),Y
   LDA   HARGS+5
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   LDY   #$39
   ADC   (FP),Y
   LDY   #$1D
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$3A
   ADC   (FP),Y
   LDY   #$1E
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$1F
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   AND   #$03
   LDY   #$20
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$04
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDY   #$19
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$1A
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$1F
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$20
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   JSR   rec
   LDY   #$21
   STA   (FP),Y
   TXA
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$23
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$24
   STA   (FP),Y
   LDY   #$04
   LDA   (FP),Y
   LDY   #$25
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$23
   LDA   (FP),Y
   CLC
   LDY   #$25
   ADC   (FP),Y
   LDY   #$27
   STA   (FP),Y
   LDY   #$24
   LDA   (FP),Y
   LDY   #$26
   ADC   (FP),Y
   LDY   #$28
   STA   (FP),Y
   LDA   #$0F
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   ORA   #$00
   BPL   .sx_done@6
   LDA   #$FF
.sx_done@6:
   LDY   #$2B
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$11
   LDA   (FP),Y
   LDY   #$29
   AND   (FP),Y
   LDY   #$2D
   STA   (FP),Y
   LDY   #$12
   LDA   (FP),Y
   LDY   #$2A
   AND   (FP),Y
   LDY   #$2E
   STA   (FP),Y
   LDY   #$13
   LDA   (FP),Y
   LDY   #$2B
   AND   (FP),Y
   LDY   #$2F
   STA   (FP),Y
   LDY   #$14
   LDA   (FP),Y
   LDY   #$2C
   AND   (FP),Y
   LDY   #$30
   STA   (FP),Y
   LDY   #$2D
   LDA   (FP),Y
   LDY   #$31
   STA   (FP),Y
   LDY   #$2E
   LDA   (FP),Y
   LDY   #$32
   STA   (FP),Y
   LDY   #$27
   LDA   (FP),Y
   CLC
   LDY   #$31
   ADC   (FP),Y
   LDY   #$33
   STA   (FP),Y
   LDY   #$28
   LDA   (FP),Y
   LDY   #$32
   ADC   (FP),Y
   LDY   #$34
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$35
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   AND   #$03
   LDY   #$36
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
   ADC   #$3C
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$37
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

   ; prologue: 3 arg bytes, 67 local bytes
   SEC
   LDA   SSP
   SBC   #$45
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$44
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   LDY   #$47
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@7
   LDA   #$00
   JMP   .sx_done@8
.sx_neg@7:
   LDA   #$FF
.sx_done@8:
   LDY   #$02
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$01
   LDA   (FP),Y
   LDY   #$05
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   LDY   #$06
   STA   (FP),Y
   INY
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
   BPL   .sx_done@10
   LDA   #$FF
.sx_done@10:
   LDY   #$0B
   STA   (FP),Y
   INY
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
   INY
   STA   (FP),Y
   LDY   #$46
   LDA   (FP),Y
   LDY   #$11
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@11
   LDA   #$00
   JMP   .sx_done@12
.sx_neg@11:
   LDA   #$FF
.sx_done@12:
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
   ORA   #$00
   BMI   .sx_neg@13
   LDA   #$00
   JMP   .sx_done@14
.sx_neg@13:
   LDA   #$FF
.sx_done@14:
   LDY   #$15
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$13
   LDA   (FP),Y
   SEC
   LDY   #$0D
   SBC   (FP),Y
   LDY   #$14
   LDA   (FP),Y
   LDY   #$0E
   SBC   (FP),Y
   LDY   #$15
   LDA   (FP),Y
   LDY   #$0F
   SBC   (FP),Y
   LDY   #$16
   LDA   (FP),Y
   LDY   #$10
   SBC   (FP),Y
   LDA   #$00
   ROL
   LDY   #$17
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$47
   LDA   (FP),Y
   LDY   #$19
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@15
   LDA   #$00
   JMP   .sx_done@16
.sx_neg@15:
   LDA   #$FF
.sx_done@16:
   LDY   #$1A
   STA   (FP),Y
   LDY   #$46
   LDA   (FP),Y
   LDY   #$1B
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@17
   LDA   #$00
   JMP   .sx_done@18
.sx_neg@17:
   LDA   #$FF
.sx_done@18:
   LDY   #$1C
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   SEC
   LDY   #$19
   SBC   (FP),Y
   LDY   #$1C
   LDA   (FP),Y
   LDY   #$1A
   SBC   (FP),Y
   BVC   .cmp_novf@19
   EOR   #$80
.cmp_novf@19:
   ASL
   LDA   #$00
   ROL
   EOR   #$01
   LDY   #$1D
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$17
   LDA   (FP),Y
   LDY   #$1D
   ORA   (FP),Y
   LDY   #$1F
   STA   (FP),Y
   LDY   #$18
   LDA   (FP),Y
   LDY   #$1E
   ORA   (FP),Y
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
   ORA   #$00
   BMI   .sx_neg@20
   LDA   #$00
   JMP   .sx_done@21
.sx_neg@20:
   LDA   #$FF
.sx_done@21:
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
   LDY   #$48
   LDA   (FP),Y
   LDY   #$29
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@22
   LDA   #$00
   JMP   .sx_done@23
.sx_neg@22:
   LDA   #$FF
.sx_done@23:
   LDY   #$2A
   STA   (FP),Y
   INY
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
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$05
   LDA   (FP),Y
   LDY   #$31
   STA   (FP),Y
   LDY   #$06
   LDA   (FP),Y
   LDY   #$32
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
   LDY   #$31
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$32
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   JSR   rec
   LDY   #$33
   STA   (FP),Y
   TXA
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$35
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$36
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@24
   LDA   #$00
   JMP   .sx_done@25
.sx_neg@24:
   LDA   #$FF
.sx_done@25:
   LDY   #$37
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$05
   LDA   (FP),Y
   CLC
   LDY   #$35
   ADC   (FP),Y
   LDY   #$39
   STA   (FP),Y
   LDY   #$06
   LDA   (FP),Y
   LDY   #$36
   ADC   (FP),Y
   LDY   #$3A
   STA   (FP),Y
   LDY   #$07
   LDA   (FP),Y
   LDY   #$37
   ADC   (FP),Y
   LDY   #$3B
   STA   (FP),Y
   LDY   #$08
   LDA   (FP),Y
   LDY   #$38
   ADC   (FP),Y
   LDY   #$3C
   STA   (FP),Y
   LDY   #$39
   LDA   (FP),Y
   LDY   #$05
   STA   (FP),Y
   LDY   #$3A
   LDA   (FP),Y
   LDY   #$06
   STA   (FP),Y
   LDY   #$3B
   LDA   (FP),Y
   LDY   #$07
   STA   (FP),Y
   LDY   #$3C
   LDA   (FP),Y
   LDY   #$08
   STA   (FP),Y
   LDY   #$25
   LDA   (FP),Y
   EOR   (FP),Y
   LDY   #$3D
   STA   (FP),Y
   LDY   #$26
   LDA   (FP),Y
   EOR   (FP),Y
   LDY   #$3E
   STA   (FP),Y
   LDY   #$27
   LDA   (FP),Y
   EOR   (FP),Y
   LDY   #$3F
   STA   (FP),Y
   LDY   #$28
   LDA   (FP),Y
   EOR   (FP),Y
   LDY   #$40
   STA   (FP),Y
   LDY   #$3D
   LDA   (FP),Y
   LDY   #$41
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
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
   ADC   #$48
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$44
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

   ; prologue: 0 arg bytes, 67 local bytes
   SEC
   LDA   SSP
   SBC   #$45
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$44
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   LDA   #$64
   LDY   #$01
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   ORA   #$00
   BPL   .sx_done@27
   LDA   #$FF
.sx_done@27:
   LDY   #$03
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   g1
   LDY   #$01
   AND   (FP),Y
   LDY   #$05
   STA   (FP),Y
   LDA   g1+1
   LDY   #$02
   AND   (FP),Y
   LDY   #$06
   STA   (FP),Y
   LDA   g1+2
   LDY   #$03
   AND   (FP),Y
   LDY   #$07
   STA   (FP),Y
   LDA   g1+3
   LDY   #$04
   AND   (FP),Y
   LDY   #$08
   STA   (FP),Y
   LDY   #$05
   LDA   (FP),Y
   EOR   g0
   LDY   #$09
   STA   (FP),Y
   LDY   #$06
   LDA   (FP),Y
   EOR   g0+1
   LDY   #$0A
   STA   (FP),Y
   LDY   #$07
   LDA   (FP),Y
   EOR   g0+2
   LDY   #$0B
   STA   (FP),Y
   LDY   #$08
   LDA   (FP),Y
   EOR   g0+3
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
   LDA   #$07
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   ORA   #$00
   BPL   .sx_done@29
   LDA   #$FF
.sx_done@29:
   LDY   #$1B
   STA   (FP),Y
   INY
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
   INY
   STA   (FP),Y
   LDY   #$1D
   LDA   (FP),Y
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
   LDA   #$07
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   ORA   #$00
   BPL   .sx_done@31
   LDA   #$FF
.sx_done@31:
   LDY   #$2B
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   g0
   LDY   #$29
   AND   (FP),Y
   LDY   #$2D
   STA   (FP),Y
   LDA   g0+1
   LDY   #$2A
   AND   (FP),Y
   LDY   #$2E
   STA   (FP),Y
   LDA   g0+2
   LDY   #$2B
   AND   (FP),Y
   LDY   #$2F
   STA   (FP),Y
   LDA   g0+3
   LDY   #$2C
   AND   (FP),Y
   LDY   #$30
   STA   (FP),Y
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
   LDY   #$2D
   LDA   (FP),Y
   STA   HARGS+4
   JSR   asl32
   LDA   HARGS+5
   LDY   #$31
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
   LDY   #$31
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   INY
   ORA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@0
   JMP   .and_false@1
.lb_skip@0:
   LDA   #$07
   LDY   #$35
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   ORA   #$00
   BPL   .sx_done@33
   LDA   #$FF
.sx_done@33:
   LDY   #$37
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   g1
   LDY   #$35
   AND   (FP),Y
   LDY   #$39
   STA   (FP),Y
   LDA   g1+1
   LDY   #$36
   AND   (FP),Y
   LDY   #$3A
   STA   (FP),Y
   LDA   g1+2
   LDY   #$37
   AND   (FP),Y
   LDY   #$3B
   STA   (FP),Y
   LDA   g1+3
   LDY   #$38
   AND   (FP),Y
   LDY   #$3C
   STA   (FP),Y
   LDY   #$15
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
   LDY   #$39
   LDA   (FP),Y
   STA   HARGS+4
   JSR   asr32
   LDA   HARGS+5
   LDY   #$3D
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
   LDY   #$3D
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   INY
   ORA   (FP),Y
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
   LDY   #$41
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@2:
   LDY   #$41
   LDA   (FP),Y
   LDY   #$43
   STA   (FP),Y

   ; epilogue
   PHA
   CLC
   LDA   FP
   ADC   #$45
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

f3:
   SUBROUTINE

   ; prologue: 1 arg bytes, 145 local bytes
   SEC
   LDA   SSP
   SBC   #$93
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$92
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
   LDA   #$00
   INY
   STA   (FP),Y
   ORA   #$00
   BPL   .sx_done@35
   LDA   #$FF
.sx_done@35:
   LDY   #$03
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$01
   LDA   (FP),Y
   SEC
   SBC   g0
   LDY   #$05
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   SBC   g0+1
   LDY   #$06
   STA   (FP),Y
   LDY   #$03
   LDA   (FP),Y
   SBC   g0+2
   LDY   #$07
   STA   (FP),Y
   LDY   #$04
   LDA   (FP),Y
   SBC   g0+3
   LDY   #$08
   STA   (FP),Y
   LDY   #$05
   LDA   (FP),Y
   LDY   #$09
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@36
   LDA   #$00
   JMP   .sx_done@37
.sx_neg@36:
   LDA   #$FF
.sx_done@37:
   LDY   #$0C
   STA   (FP),Y
   LDY   #$0A
   LDA   (FP),Y
   LDY   #$0D
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@38
   LDA   #$00
   JMP   .sx_done@39
.sx_neg@38:
   LDA   #$FF
.sx_done@39:
   LDY   #$0E
   STA   (FP),Y
   LDY   #$0B
   LDA   (FP),Y
   LDY   #$0D
   ORA   (FP),Y
   LDY   #$0F
   STA   (FP),Y
   LDY   #$0C
   LDA   (FP),Y
   LDY   #$0E
   ORA   (FP),Y
   LDY   #$10
   STA   (FP),Y
   LDY   #$0A
   LDA   (FP),Y
   LDY   #$11
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@40
   LDA   #$00
   JMP   .sx_done@41
.sx_neg@40:
   LDA   #$FF
.sx_done@41:
   LDY   #$12
   STA   (FP),Y
   LDY   #$94
   LDA   (FP),Y
   LDY   #$13
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@42
   LDA   #$00
   JMP   .sx_done@43
.sx_neg@42:
   LDA   #$FF
.sx_done@43:
   LDY   #$14
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ORA   #$01
   LDY   #$15
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$16
   STA   (FP),Y
   LDY   #$11
   LDA   (FP),Y
   STA   HARGS
   INY
   LDA   (FP),Y
   STA   HARGS+1
   LDY   #$15
   LDA   (FP),Y
   STA   HARGS+2
   INY
   LDA   (FP),Y
   STA   HARGS+3
   JSR   sdivmod16
   LDA   HARGS+6
   LDY   #$17
   STA   (FP),Y
   LDA   HARGS+7
   INY
   STA   (FP),Y
   LDY   #$0F
   LDA   (FP),Y
   CLC
   LDY   #$17
   ADC   (FP),Y
   LDY   #$19
   STA   (FP),Y
   LDY   #$10
   LDA   (FP),Y
   LDY   #$18
   ADC   (FP),Y
   LDY   #$1A
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$1B
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$1C
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@44
   LDA   #$00
   JMP   .sx_done@45
.sx_neg@44:
   LDA   #$FF
.sx_done@45:
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
   INY
   STA   (FP),Y
   LDY   #$94
   LDA   (FP),Y
   LDY   #$23
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@46
   LDA   #$00
   JMP   .sx_done@47
.sx_neg@46:
   LDA   #$FF
.sx_done@47:
   LDY   #$24
   STA   (FP),Y
   LDA   #$07
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   ORA   #$00
   BPL   .sx_done@49
   LDA   #$FF
.sx_done@49:
   LDY   #$27
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   g0
   LDY   #$25
   AND   (FP),Y
   LDY   #$29
   STA   (FP),Y
   LDA   g0+1
   LDY   #$26
   AND   (FP),Y
   LDY   #$2A
   STA   (FP),Y
   LDA   g0+2
   LDY   #$27
   AND   (FP),Y
   LDY   #$2B
   STA   (FP),Y
   LDA   g0+3
   LDY   #$28
   AND   (FP),Y
   LDY   #$2C
   STA   (FP),Y
   LDY   #$23
   LDA   (FP),Y
   LDY   #$2D
   STA   (FP),Y
   LDY   #$24
   LDA   (FP),Y
   LDY   #$2E
   STA   (FP),Y
   LDY   #$29
   LDA   (FP),Y
   TAX
   BEQ   .shift_done@51
.shift_loop@50:
   LDY   #$2E
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
   BNE   .shift_loop@50
.shift_done@51:
   LDY   #$2D
   LDA   (FP),Y
   LDY   #$2F
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$30
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@52
   LDA   #$00
   JMP   .sx_done@53
.sx_neg@52:
   LDA   #$FF
.sx_done@53:
   LDY   #$31
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$22
   CMP   (FP),Y
   BNE   .cmp_true@54
   DEY
   CMP   (FP),Y
   BNE   .cmp_true@54
   LDY   #$30
   LDA   (FP),Y
   LDY   #$20
   CMP   (FP),Y
   BNE   .cmp_true@54
   LDY   #$2F
   LDA   (FP),Y
   LDY   #$1F
   CMP   (FP),Y
   BNE   .cmp_true@54
   LDA   #$00
   JMP   .cmp_end@55
.cmp_true@54:
   LDA   #$01
.cmp_end@55:
   LDY   #$33
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
   LDA   #<f5
   INY
   STA   (FP),Y
   LDA   #>f5
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
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
.loop@0_start:
   LDY   #$3B
   LDA   (FP),Y
   SEC
   SBC   #$03
   INY
   LDA   (FP),Y
   SBC   #$00
   BVC   .cmp_novf@57
   EOR   #$80
.cmp_novf@57:
   ASL
   LDA   #$00
   ROL
   LDY   #$3D
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@2
   JMP   .loop@0_break
.lb_skip@2:
   LDY   #$36
   LDA   (FP),Y
   BEQ   .lnot_true@58
   LDA   #$00
   JMP   .lnot_end@59
.lnot_true@58:
   LDA   #$01
.lnot_end@59:
   LDY   #$3F
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$41
   STA   (FP),Y
   LDA   #$01
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   ORA   #$00
   BPL   .sx_done@61
   LDA   #$FF
.sx_done@61:
   LDY   #$44
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   g1
   LDY   #$42
   ORA   (FP),Y
   LDY   #$46
   STA   (FP),Y
   LDA   g1+1
   LDY   #$43
   ORA   (FP),Y
   LDY   #$47
   STA   (FP),Y
   LDA   g1+2
   LDY   #$44
   ORA   (FP),Y
   LDY   #$48
   STA   (FP),Y
   LDA   g1+3
   LDY   #$45
   ORA   (FP),Y
   LDY   #$49
   STA   (FP),Y
   LDA   g1
   STA   HARGS
   LDA   g1+1
   STA   HARGS+1
   LDA   g1+2
   STA   HARGS+2
   LDA   g1+3
   STA   HARGS+3
   LDY   #$46
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
   JSR   udivmod32
   LDA   HARGS+12
   LDY   #$4A
   STA   (FP),Y
   LDA   HARGS+13
   INY
   STA   (FP),Y
   LDA   HARGS+14
   INY
   STA   (FP),Y
   LDA   HARGS+15
   INY
   STA   (FP),Y
   LDY   #$4A
   LDA   (FP),Y
   LDY   #$4E
   STA   (FP),Y
   LDY   #$36
   LDA   (FP),Y
   LDY   #$4F
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@62
   LDA   #$00
   JMP   .sx_done@63
.sx_neg@62:
   LDA   #$FF
.sx_done@63:
   LDY   #$50
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$51
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$52
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@64
   LDA   #$00
   JMP   .sx_done@65
.sx_neg@64:
   LDA   #$FF
.sx_done@65:
   LDY   #$53
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   g0
   SEC
   LDY   #$51
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
   LDA   #$00
   ROL
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$57
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$03
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDY   #$41
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$4E
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$57
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$39
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   JSR   icall
   LDY   #$58
   STA   (FP),Y
   TXA
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   LDY   #$3B
   ADC   (FP),Y
   LDY   #$5A
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$3C
   ADC   (FP),Y
   LDY   #$5B
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$5C
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$5D
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@66
   LDA   #$00
   JMP   .sx_done@67
.sx_neg@66:
   LDA   #$FF
.sx_done@67:
   LDY   #$5E
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$1F
   LDA   (FP),Y
   CLC
   LDY   #$5C
   ADC   (FP),Y
   LDY   #$60
   STA   (FP),Y
   LDY   #$20
   LDA   (FP),Y
   LDY   #$5D
   ADC   (FP),Y
   LDY   #$61
   STA   (FP),Y
   LDY   #$21
   LDA   (FP),Y
   LDY   #$5E
   ADC   (FP),Y
   LDY   #$62
   STA   (FP),Y
   LDY   #$22
   LDA   (FP),Y
   LDY   #$5F
   ADC   (FP),Y
   LDY   #$63
   STA   (FP),Y
   LDY   #$60
   LDA   (FP),Y
   LDY   #$1F
   STA   (FP),Y
   LDY   #$61
   LDA   (FP),Y
   LDY   #$20
   STA   (FP),Y
   LDY   #$62
   LDA   (FP),Y
   LDY   #$21
   STA   (FP),Y
   LDY   #$63
   LDA   (FP),Y
   LDY   #$22
   STA   (FP),Y
   LDY   #$3B
   LDA   (FP),Y
   LDY   #$64
   STA   (FP),Y
   LDY   #$3C
   LDA   (FP),Y
   LDY   #$65
   STA   (FP),Y
   LDY   #$3B
   LDA   (FP),Y
   CLC
   ADC   #$01
   LDY   #$66
   STA   (FP),Y
   LDY   #$3C
   LDA   (FP),Y
   ADC   #$00
   LDY   #$67
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$3B
   STA   (FP),Y
   LDY   #$67
   LDA   (FP),Y
   LDY   #$3C
   STA   (FP),Y
   JMP   .loop@0_start
.loop@0_break:
   LDY   #$1F
   LDA   (FP),Y
   LDY   #$68
   STA   (FP),Y
   LDY   #$20
   LDA   (FP),Y
   LDY   #$69
   STA   (FP),Y
   LDY   #$21
   LDA   (FP),Y
   LDY   #$6A
   STA   (FP),Y
   LDY   #$22
   LDA   (FP),Y
   LDY   #$6B
   STA   (FP),Y
   LDA   g1
   CLC
   LDY   #$68
   ADC   (FP),Y
   LDY   #$6C
   STA   (FP),Y
   LDA   g1+1
   LDY   #$69
   ADC   (FP),Y
   LDY   #$6D
   STA   (FP),Y
   LDA   g1+2
   LDY   #$6A
   ADC   (FP),Y
   LDY   #$6E
   STA   (FP),Y
   LDA   g1+3
   LDY   #$6B
   ADC   (FP),Y
   LDY   #$6F
   STA   (FP),Y
   LDY   #$6C
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
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
.loop@1_start:
   LDY   #$70
   LDA   (FP),Y
   SEC
   SBC   #$03
   INY
   LDA   (FP),Y
   SBC   #$00
   BVC   .cmp_novf@68
   EOR   #$80
.cmp_novf@68:
   ASL
   LDA   #$00
   ROL
   LDY   #$72
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@1
   JMP   .loop@1_break
.lb_skip@1:
   JSR   f4
   LDY   #$74
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   LDY   #$70
   ADC   (FP),Y
   LDY   #$77
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$71
   ADC   (FP),Y
   LDY   #$78
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$79
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$7A
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@69
   LDA   #$00
   JMP   .sx_done@70
.sx_neg@69:
   LDA   #$FF
.sx_done@70:
   LDY   #$7B
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$1F
   LDA   (FP),Y
   CLC
   LDY   #$79
   ADC   (FP),Y
   LDY   #$7D
   STA   (FP),Y
   LDY   #$20
   LDA   (FP),Y
   LDY   #$7A
   ADC   (FP),Y
   LDY   #$7E
   STA   (FP),Y
   LDY   #$21
   LDA   (FP),Y
   LDY   #$7B
   ADC   (FP),Y
   LDY   #$7F
   STA   (FP),Y
   LDY   #$22
   LDA   (FP),Y
   LDY   #$7C
   ADC   (FP),Y
   LDY   #$80
   STA   (FP),Y
   LDY   #$7D
   LDA   (FP),Y
   LDY   #$1F
   STA   (FP),Y
   LDY   #$7E
   LDA   (FP),Y
   LDY   #$20
   STA   (FP),Y
   LDY   #$7F
   LDA   (FP),Y
   LDY   #$21
   STA   (FP),Y
   LDY   #$80
   LDA   (FP),Y
   LDY   #$22
   STA   (FP),Y
   LDY   #$70
   LDA   (FP),Y
   LDY   #$81
   STA   (FP),Y
   LDY   #$71
   LDA   (FP),Y
   LDY   #$82
   STA   (FP),Y
   LDY   #$70
   LDA   (FP),Y
   CLC
   ADC   #$01
   LDY   #$83
   STA   (FP),Y
   LDY   #$71
   LDA   (FP),Y
   ADC   #$00
   LDY   #$84
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$70
   STA   (FP),Y
   LDY   #$84
   LDA   (FP),Y
   LDY   #$71
   STA   (FP),Y
   JMP   .loop@1_start
.loop@1_break:
   LDA   g0
   CLC
   SBC   g1
   LDA   g0+1
   SBC   g1+1
   LDA   g0+2
   SBC   g1+2
   LDA   g0+3
   SBC   g1+3
   LDA   #$00
   ROL
   LDY   #$85
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
   LDY   #$85
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$86
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   JSR   rec
   LDY   #$87
   STA   (FP),Y
   TXA
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$89
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$8A
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@71
   LDA   #$00
   JMP   .sx_done@72
.sx_neg@71:
   LDA   #$FF
.sx_done@72:
   LDY   #$8B
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$1F
   LDA   (FP),Y
   CLC
   LDY   #$89
   ADC   (FP),Y
   LDY   #$8D
   STA   (FP),Y
   LDY   #$20
   LDA   (FP),Y
   LDY   #$8A
   ADC   (FP),Y
   LDY   #$8E
   STA   (FP),Y
   LDY   #$21
   LDA   (FP),Y
   LDY   #$8B
   ADC   (FP),Y
   LDY   #$8F
   STA   (FP),Y
   LDY   #$22
   LDA   (FP),Y
   LDY   #$8C
   ADC   (FP),Y
   LDY   #$90
   STA   (FP),Y
   LDY   #$8D
   LDA   (FP),Y
   LDY   #$1F
   STA   (FP),Y
   LDY   #$8E
   LDA   (FP),Y
   LDY   #$20
   STA   (FP),Y
   LDY   #$8F
   LDA   (FP),Y
   LDY   #$21
   STA   (FP),Y
   LDY   #$90
   LDA   (FP),Y
   LDY   #$22
   STA   (FP),Y
   LDY   #$0A
   LDA   (FP),Y
   BEQ   .cmp_end@73
   LDA   #$01
.cmp_end@73:
   LDY   #$91
   STA   (FP),Y

   ; epilogue
   PHA
   CLC
   LDA   FP
   ADC   #$94
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

f2:
   SUBROUTINE

   ; prologue: 2 arg bytes, 114 local bytes
   SEC
   LDA   SSP
   SBC   #$74
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$73
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   LDA   g0
   EOR   g1
   LDY   #$01
   STA   (FP),Y
   LDA   g0+1
   EOR   g1+1
   INY
   STA   (FP),Y
   LDA   g0+2
   EOR   g1+2
   INY
   STA   (FP),Y
   LDA   g0+3
   EOR   g1+3
   INY
   STA   (FP),Y
   LDY   #$01
   LDA   (FP),Y
   EOR   #$FF
   LDY   #$05
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   EOR   #$FF
   LDY   #$06
   STA   (FP),Y
   LDY   #$03
   LDA   (FP),Y
   EOR   #$FF
   LDY   #$07
   STA   (FP),Y
   LDY   #$04
   LDA   (FP),Y
   EOR   #$FF
   LDY   #$08
   STA   (FP),Y
   LDY   #$05
   LDA   (FP),Y
   LDY   #$09
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$0A
   LDA   (FP),Y
   LDY   #$0D
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$0B
   LDA   (FP),Y
   SEC
   LDY   #$0D
   SBC   (FP),Y
   LDY   #$0F
   STA   (FP),Y
   LDY   #$0C
   LDA   (FP),Y
   LDY   #$0E
   SBC   (FP),Y
   LDY   #$10
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$11
   STA   (FP),Y
   INY
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@74
   LDA   #$00
   JMP   .sx_done@75
.sx_neg@74:
   LDA   #$FF
.sx_done@75:
   LDY   #$13
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$14
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$15
   STA   (FP),Y
   SEC
   LDA   #$00
   DEY
   SBC   (FP),Y
   LDY   #$16
   STA   (FP),Y
   LDA   #$00
   DEY
   SBC   (FP),Y
   LDY   #$17
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   AND   #$07
   LDY   #$18
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDA   #$00
   LDY   #$19
   STA   (FP),Y
   LDA   g1
   STA   HARGS
   LDA   g1+1
   STA   HARGS+1
   LDA   g1+2
   STA   HARGS+2
   LDA   g1+3
   STA   HARGS+3
   DEY
   LDA   (FP),Y
   STA   HARGS+4
   JSR   asl32
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
   LDY   #$1D
   LDA   (FP),Y
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
   LDY   #$20
   LDA   (FP),Y
   LDY   #$24
   STA   (FP),Y
   LDY   #$21
   LDA   (FP),Y
   LDY   #$25
   STA   (FP),Y
   LDY   #$0A
   LDA   (FP),Y
   LDY   #$26
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDA   #$07
   SEC
   DEY
   SBC   (FP),Y
   LDA   #$00
   INY
   SBC   (FP),Y
   BVC   .cmp_novf@76
   EOR   #$80
.cmp_novf@76:
   ASL
   LDA   #$00
   ROL
   EOR   #$01
   LDY   #$28
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$2A
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
   LDY   #$2B
   STA   (FP),Y
   ORA   #$00
   BEQ   .if_else@4
   LDA   #$03
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   ORA   #$00
   BPL   .sx_done@78
   LDA   #$FF
.sx_done@78:
   LDY   #$2E
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$22
   LDA   (FP),Y
   CLC
   LDY   #$2C
   ADC   (FP),Y
   LDY   #$30
   STA   (FP),Y
   LDY   #$23
   LDA   (FP),Y
   LDY   #$2D
   ADC   (FP),Y
   LDY   #$31
   STA   (FP),Y
   LDY   #$24
   LDA   (FP),Y
   LDY   #$2E
   ADC   (FP),Y
   LDY   #$32
   STA   (FP),Y
   LDY   #$25
   LDA   (FP),Y
   LDY   #$2F
   ADC   (FP),Y
   LDY   #$33
   STA   (FP),Y
   LDY   #$30
   LDA   (FP),Y
   LDY   #$22
   STA   (FP),Y
   LDY   #$31
   LDA   (FP),Y
   LDY   #$23
   STA   (FP),Y
   LDY   #$32
   LDA   (FP),Y
   LDY   #$24
   STA   (FP),Y
   LDY   #$33
   LDA   (FP),Y
   LDY   #$25
   STA   (FP),Y
   JMP   .if_end@3
.if_else@4:
   LDA   #$05
   LDY   #$34
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   ORA   #$00
   BPL   .sx_done@80
   LDA   #$FF
.sx_done@80:
   LDY   #$36
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$22
   LDA   (FP),Y
   LDY   #$34
   EOR   (FP),Y
   LDY   #$38
   STA   (FP),Y
   LDY   #$23
   LDA   (FP),Y
   LDY   #$35
   EOR   (FP),Y
   LDY   #$39
   STA   (FP),Y
   LDY   #$24
   LDA   (FP),Y
   LDY   #$36
   EOR   (FP),Y
   LDY   #$3A
   STA   (FP),Y
   LDY   #$25
   LDA   (FP),Y
   LDY   #$37
   EOR   (FP),Y
   LDY   #$3B
   STA   (FP),Y
   LDY   #$38
   LDA   (FP),Y
   LDY   #$22
   STA   (FP),Y
   LDY   #$39
   LDA   (FP),Y
   LDY   #$23
   STA   (FP),Y
   LDY   #$3A
   LDA   (FP),Y
   LDY   #$24
   STA   (FP),Y
   LDY   #$3B
   LDA   (FP),Y
   LDY   #$25
   STA   (FP),Y
.if_end@3:
   LDY   #$22
   LDA   (FP),Y
   LDY   #$3C
   STA   (FP),Y
   LDY   #$23
   LDA   (FP),Y
   LDY   #$3D
   STA   (FP),Y
   LDY   #$24
   LDA   (FP),Y
   LDY   #$3E
   STA   (FP),Y
   LDY   #$25
   LDA   (FP),Y
   LDY   #$3F
   STA   (FP),Y
   LDA   g0
   CLC
   LDY   #$3C
   ADC   (FP),Y
   LDY   #$40
   STA   (FP),Y
   LDA   g0+1
   LDY   #$3D
   ADC   (FP),Y
   LDY   #$41
   STA   (FP),Y
   LDA   g0+2
   LDY   #$3E
   ADC   (FP),Y
   LDY   #$42
   STA   (FP),Y
   LDA   g0+3
   LDY   #$3F
   ADC   (FP),Y
   LDY   #$43
   STA   (FP),Y
   LDY   #$40
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
   JSR   f4
   LDY   #$44
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$14
   LDA   (FP),Y
   LDY   #$45
   EOR   (FP),Y
   LDY   #$47
   STA   (FP),Y
   LDY   #$15
   LDA   (FP),Y
   LDY   #$46
   EOR   (FP),Y
   LDY   #$48
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$14
   STA   (FP),Y
   LDY   #$48
   LDA   (FP),Y
   LDY   #$15
   STA   (FP),Y
   LDY   #$0A
   LDA   (FP),Y
   LDY   #$49
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$4B
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$4C
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@81
   LDA   #$00
   JMP   .sx_done@82
.sx_neg@81:
   LDA   #$FF
.sx_done@82:
   LDY   #$4D
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$4B
   LDA   (FP),Y
   SEC
   LDY   #$22
   SBC   (FP),Y
   LDY   #$4C
   LDA   (FP),Y
   LDY   #$23
   SBC   (FP),Y
   LDY   #$4D
   LDA   (FP),Y
   LDY   #$24
   SBC   (FP),Y
   LDY   #$4E
   LDA   (FP),Y
   LDY   #$25
   SBC   (FP),Y
   BVC   .cmp_novf@83
   EOR   #$80
.cmp_novf@83:
   ASL
   LDA   #$00
   ROL
   EOR   #$01
   LDY   #$4F
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
   LDY   #$4F
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$50
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   JSR   rec
   LDY   #$51
   STA   (FP),Y
   TXA
   INY
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
   BMI   .sx_neg@84
   LDA   #$00
   JMP   .sx_done@85
.sx_neg@84:
   LDA   #$FF
.sx_done@85:
   LDY   #$55
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$22
   LDA   (FP),Y
   CLC
   LDY   #$53
   ADC   (FP),Y
   LDY   #$57
   STA   (FP),Y
   LDY   #$23
   LDA   (FP),Y
   LDY   #$54
   ADC   (FP),Y
   LDY   #$58
   STA   (FP),Y
   LDY   #$24
   LDA   (FP),Y
   LDY   #$55
   ADC   (FP),Y
   LDY   #$59
   STA   (FP),Y
   LDY   #$25
   LDA   (FP),Y
   LDY   #$56
   ADC   (FP),Y
   LDY   #$5A
   STA   (FP),Y
   LDY   #$57
   LDA   (FP),Y
   LDY   #$22
   STA   (FP),Y
   LDY   #$58
   LDA   (FP),Y
   LDY   #$23
   STA   (FP),Y
   LDY   #$59
   LDA   (FP),Y
   LDY   #$24
   STA   (FP),Y
   LDY   #$5A
   LDA   (FP),Y
   LDY   #$25
   STA   (FP),Y
   LDA   g1
   ORA   g1+1
   ORA   g1+2
   ORA   g1+3
   BEQ   .and_false@5
   LDA   g1
   ORA   g1+1
   ORA   g1+2
   ORA   g1+3
   BEQ   .and_false@5
   LDA   #$01
   LDY   #$5B
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   JMP   .and_end@6
.and_false@5:
   LDA   #$00
   LDY   #$5B
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@6:
   LDY   #$5B
   LDA   (FP),Y
   LDY   #$5D
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$5E
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@86
   LDA   #$00
   JMP   .sx_done@87
.sx_neg@86:
   LDA   #$FF
.sx_done@87:
   LDY   #$5F
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$04
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$22
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
   LDY   #$61
   LDA   (FP),Y
   STA   HARGS+4
   JSR   asr32
   LDA   HARGS+5
   LDY   #$63
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
   LDA   #$01
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   ORA   #$00
   BPL   .sx_done@89
   LDA   #$FF
.sx_done@89:
   LDY   #$69
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$63
   LDA   (FP),Y
   LDY   #$67
   ORA   (FP),Y
   LDY   #$6B
   STA   (FP),Y
   LDY   #$64
   LDA   (FP),Y
   LDY   #$68
   ORA   (FP),Y
   LDY   #$6C
   STA   (FP),Y
   LDY   #$65
   LDA   (FP),Y
   LDY   #$69
   ORA   (FP),Y
   LDY   #$6D
   STA   (FP),Y
   LDY   #$66
   LDA   (FP),Y
   LDY   #$6A
   ORA   (FP),Y
   LDY   #$6E
   STA   (FP),Y
   LDY   #$5D
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
   LDY   #$6B
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
   JSR   sdivmod32
   LDA   HARGS+12
   LDY   #$6F
   STA   (FP),Y
   LDA   HARGS+13
   INY
   STA   (FP),Y
   LDA   HARGS+14
   INY
   STA   (FP),Y
   LDA   HARGS+15
   INY
   STA   (FP),Y
   LDY   #$6F
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

   ; epilogue
   CLC
   LDA   FP
   ADC   #$76
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

   ; prologue: 6 arg bytes, 66 local bytes
   SEC
   LDA   SSP
   SBC   #$44
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$43
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   LDA   #$03
   LDY   #$01
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   AND   #$03
   LDY   #$04
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDA   #$00
   LDY   #$05
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$06
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
   BPL   .sx_done@91
   LDA   #$FF
.sx_done@91:
   LDY   #$0A
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$45
   LDA   (FP),Y
   LDY   #$08
   ORA   (FP),Y
   LDY   #$0C
   STA   (FP),Y
   LDY   #$46
   LDA   (FP),Y
   LDY   #$09
   ORA   (FP),Y
   LDY   #$0D
   STA   (FP),Y
   LDY   #$47
   LDA   (FP),Y
   LDY   #$0A
   ORA   (FP),Y
   LDY   #$0E
   STA   (FP),Y
   LDY   #$48
   LDA   (FP),Y
   LDY   #$0B
   ORA   (FP),Y
   LDY   #$0F
   STA   (FP),Y
   LDY   #$0C
   LDA   (FP),Y
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
   LDY   #$0F
   LDA   (FP),Y
   LDY   #$13
   STA   (FP),Y
   LDA   g0
   STA   HARGS
   LDA   g0+1
   STA   HARGS+1
   LDA   g0+2
   STA   HARGS+2
   LDA   g0+3
   STA   HARGS+3
   LDY   #$10
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
   JSR   udivmod32
   LDA   HARGS+12
   LDY   #$14
   STA   (FP),Y
   LDA   HARGS+13
   INY
   STA   (FP),Y
   LDA   HARGS+14
   INY
   STA   (FP),Y
   LDA   HARGS+15
   INY
   STA   (FP),Y
   LDY   #$14
   LDA   (FP),Y
   LDY   #$18
   STA   (FP),Y
   LDY   #$15
   LDA   (FP),Y
   LDY   #$19
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$1A
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$1B
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$1C
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$1D
   STA   (FP),Y
   LDA   #$FF
   INY
   STA   (FP),Y
   LDA   #$7F
   INY
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@92
   LDA   #$00
   JMP   .sx_done@93
.sx_neg@92:
   LDA   #$FF
.sx_done@93:
   LDY   #$20
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$1E
   LDA   (FP),Y
   CLC
   ADC   g1
   LDY   #$22
   STA   (FP),Y
   LDY   #$1F
   LDA   (FP),Y
   ADC   g1+1
   LDY   #$23
   STA   (FP),Y
   LDY   #$20
   LDA   (FP),Y
   ADC   g1+2
   LDY   #$24
   STA   (FP),Y
   LDY   #$21
   LDA   (FP),Y
   ADC   g1+3
   LDY   #$25
   STA   (FP),Y
   LDA   #$01
   BNE   .or_true@7
   LDA   #$FF
   BNE   .or_true@7
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   JMP   .or_end@8
.or_true@7:
   LDA   #$01
   LDY   #$26
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@8:
   LDY   #$26
   LDA   (FP),Y
   LDY   #$28
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$29
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@94
   LDA   #$00
   JMP   .sx_done@95
.sx_neg@94:
   LDA   #$FF
.sx_done@95:
   LDY   #$2A
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$28
   LDA   (FP),Y
   SEC
   LDY   #$22
   SBC   (FP),Y
   LDY   #$29
   LDA   (FP),Y
   LDY   #$23
   SBC   (FP),Y
   LDY   #$2A
   LDA   (FP),Y
   LDY   #$24
   SBC   (FP),Y
   LDY   #$2B
   LDA   (FP),Y
   LDY   #$25
   SBC   (FP),Y
   LDA   #$00
   ROL
   LDY   #$2C
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$2E
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$2F
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@96
   LDA   #$00
   JMP   .sx_done@97
.sx_neg@96:
   LDA   #$FF
.sx_done@97:
   LDY   #$30
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$2E
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
   INY
   STA   (FP),Y
   LDY   #$49
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$36
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$37
   STA   (FP),Y
   LDA   #$64
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@98
   LDA   #$00
   JMP   .sx_done@99
.sx_neg@98:
   LDA   #$FF
.sx_done@99:
   LDY   #$3A
   STA   (FP),Y
   LDY   #$36
   LDA   (FP),Y
   CLC
   LDY   #$39
   ADC   (FP),Y
   LDY   #$3B
   STA   (FP),Y
   LDY   #$37
   LDA   (FP),Y
   LDY   #$3A
   ADC   (FP),Y
   LDY   #$3C
   STA   (FP),Y
   LDY   #$49
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$3B
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$3C
   LDA   (FP),Y
   LDY   #$01
   STA   (DPTR),Y
   LDY   #$49
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$3D
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$3E
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$3F
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$40
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@100
   LDA   #$00
   JMP   .sx_done@101
.sx_neg@100:
   LDA   #$FF
.sx_done@101:
   LDY   #$41
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$3F
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

   ; epilogue
   CLC
   LDA   FP
   ADC   #$4A
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

   ; prologue: 8 arg bytes, 50 local bytes
   SEC
   LDA   SSP
   SBC   #$34
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$33
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   LDA   #$00
   LDY   #$01
   STA   (FP),Y
   INY
   STA   (FP),Y
   ORA   #$00
   BPL   .sx_done@103
   LDA   #$FF
.sx_done@103:
   LDY   #$03
   STA   (FP),Y
   INY
   STA   (FP),Y
   CMP   g1+3
   BNE   .cmp_true@104
   CMP   g1+2
   BNE   .cmp_true@104
   LDY   #$02
   LDA   (FP),Y
   CMP   g1+1
   BNE   .cmp_true@104
   CMP   g1
   BNE   .cmp_true@104
   LDA   #$00
   JMP   .cmp_end@105
.cmp_true@104:
   LDA   #$01
.cmp_end@105:
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
   BMI   .sx_neg@107
   LDA   #$00
   JMP   .sx_done@108
.sx_neg@107:
   LDA   #$FF
.sx_done@108:
   LDY   #$09
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$07
   LDA   (FP),Y
   LDY   #$0B
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   g0
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
   LDY   #$0E
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$0C
   LDA   (FP),Y
   LDY   #$11
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@109
   LDA   #$00
   JMP   .sx_done@110
.sx_neg@109:
   LDA   #$FF
.sx_done@110:
   LDY   #$12
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$0F
   EOR   (FP),Y
   LDY   #$13
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$10
   EOR   (FP),Y
   LDY   #$14
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$15
   STA   (FP),Y
   LDY   #$0C
   STA   (FP),Y
   LDY   #$16
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@111
   LDA   #$00
   JMP   .sx_done@112
.sx_neg@111:
   LDA   #$FF
.sx_done@112:
   LDY   #$17
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   g1
   CLC
   LDY   #$16
   ADC   (FP),Y
   LDY   #$1A
   STA   (FP),Y
   LDA   g1+1
   LDY   #$17
   ADC   (FP),Y
   LDY   #$1B
   STA   (FP),Y
   LDA   g1+2
   LDY   #$18
   ADC   (FP),Y
   LDY   #$1C
   STA   (FP),Y
   LDA   g1+3
   LDY   #$19
   ADC   (FP),Y
   LDY   #$1D
   STA   (FP),Y
   LDY   #$1A
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
   LDA   #$00
   LDY   #$36
   CMP   (FP),Y
   BNE   .cmp_true@113
   LDA   #$64
   DEY
   CMP   (FP),Y
   BNE   .cmp_true@113
   LDA   #$00
   JMP   .cmp_end@114
.cmp_true@113:
   LDA   #$01
.cmp_end@114:
   LDY   #$1E
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
   LDY   #$1E
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$1F
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   JSR   rec
   LDY   #$20
   STA   (FP),Y
   TXA
   INY
   STA   (FP),Y
   LDY   #$0C
   LDA   (FP),Y
   LDY   #$22
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@116
   LDA   #$00
   JMP   .sx_done@117
.sx_neg@116:
   LDA   #$FF
.sx_done@117:
   LDY   #$23
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   LDY   #$20
   ADC   (FP),Y
   LDY   #$24
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$21
   ADC   (FP),Y
   LDY   #$25
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$26
   STA   (FP),Y
   LDY   #$0C
   STA   (FP),Y
   LDY   #$3B
   LDA   (FP),Y
   LDY   #$27
   STA   (FP),Y
   LDY   #$3C
   LDA   (FP),Y
   LDY   #$28
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   CMP   g0+3
   BNE   .cmp_true@118
   CMP   g0+2
   BNE   .cmp_true@118
   LDY   #$28
   LDA   (FP),Y
   CMP   g0+1
   BNE   .cmp_true@118
   DEY
   LDA   (FP),Y
   CMP   g0
   BNE   .cmp_true@118
   LDA   #$00
   JMP   .cmp_end@119
.cmp_true@118:
   LDA   #$01
.cmp_end@119:
   LDY   #$2B
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$2D
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$2E
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@121
   LDA   #$00
   JMP   .sx_done@122
.sx_neg@121:
   LDA   #$FF
.sx_done@122:
   LDY   #$2F
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$2D
   LDA   (FP),Y
   CLC
   SBC   g0
   INY
   LDA   (FP),Y
   SBC   g0+1
   INY
   LDA   (FP),Y
   SBC   g0+2
   INY
   LDA   (FP),Y
   SBC   g0+3
   LDA   #$00
   ROL
   INY
   STA   (FP),Y
   LDA   #$00
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
   ADC   #$3C
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$33
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

main:
   SUBROUTINE

   ; prologue: 0 arg bytes, 40 local bytes
   SEC
   LDA   SSP
   SBC   #$2A
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$29
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   LDA   #$12
   LDY   #$01
   STA   (FP),Y
   LDA   #$01
   INY
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@123
   LDA   #$00
   JMP   .sx_done@124
.sx_neg@123:
   LDA   #$FF
.sx_done@124:
   LDY   #$03
   STA   (FP),Y
   INY
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$08
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   #$63
   LDY   #$01
   STA   (SSP),Y
   LDA   #$00
   INY
   STA   (SSP),Y
   DEY
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   DEY
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   DEY
   LDA   (FP),Y
   LDY   #$05
   STA   (SSP),Y
   DEY
   LDA   (FP),Y
   LDY   #$06
   STA   (SSP),Y
   LDA   #$B9
   INY
   STA   (SSP),Y
   LDA   #$00
   INY
   STA   (SSP),Y
   JSR   f0
   LDY   #$05
   STA   (FP),Y
   TXA
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
   BMI   .sx_neg@125
   LDA   #$00
   JMP   .sx_done@126
.sx_neg@125:
   LDA   #$FF
.sx_done@126:
   LDY   #$09
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$07
   LDA   (FP),Y
   LDY   #$0B
   STA   (FP),Y
   LDY   #$08
   LDA   (FP),Y
   LDY   #$0C
   STA   (FP),Y
   LDY   #$09
   LDA   (FP),Y
   LDY   #$0D
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
   LDY   #$12
   LDA   (FP),Y
   LDY   #$16
   EOR   (FP),Y
   LDY   #$1A
   STA   (FP),Y
   LDY   #$0B
   LDA   (FP),Y
   LDY   #$17
   EOR   (FP),Y
   LDY   #$1B
   STA   (FP),Y
   LDY   #$0C
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
   LDY   #$1B
   LDA   (FP),Y
   LDY   #$0B
   STA   (FP),Y
   LDY   #$1C
   LDA   (FP),Y
   LDY   #$0C
   STA   (FP),Y
   LDY   #$1D
   LDA   (FP),Y
   LDY   #$0D
   STA   (FP),Y
   LDY   #$1E
   LDA   (FP),Y
   LDY   #$0E
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
   LDA   #$10
   STA   HARGS+4
   JSR   asr32
   LDA   HARGS+5
   LDY   #$1F
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
   LDY   #$0B
   LDA   (FP),Y
   LDY   #$1F
   EOR   (FP),Y
   LDY   #$23
   STA   (FP),Y
   LDY   #$0C
   LDA   (FP),Y
   LDY   #$20
   EOR   (FP),Y
   LDY   #$24
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
   LDY   #$23
   LDA   (FP),Y
   LDY   #$27
   STA   (FP),Y
   LDY   #$24
   LDA   (FP),Y
   LDY   #$28
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
   ADC   #$2A
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$29
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
   DC.L  $00000087

g1:
   DC.L  $00000059
