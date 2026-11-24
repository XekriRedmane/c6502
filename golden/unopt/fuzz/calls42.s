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

   ; prologue: 6 arg bytes, 67 local bytes
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

   LDA   g0
   LDY   #$01
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
   LDA   g0+3
   BMI   .sx_neg@1
   LDA   #$00
   JMP   .sx_done@2
.sx_neg@1:
   LDA   #$FF
.sx_done@2:
   LDY   #$05
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
   AND   #$07
   INY
   STA   (FP),Y
   LDY   #$07
   LDA   (FP),Y
   SEC
   LDY   #$46
   SBC   (FP),Y
   LDY   #$0D
   STA   (FP),Y
   LDY   #$08
   LDA   (FP),Y
   LDY   #$47
   SBC   (FP),Y
   LDY   #$0E
   STA   (FP),Y
   LDY   #$09
   LDA   (FP),Y
   LDY   #$48
   SBC   (FP),Y
   LDY   #$0F
   STA   (FP),Y
   LDY   #$0A
   LDA   (FP),Y
   LDY   #$49
   SBC   (FP),Y
   LDY   #$10
   STA   (FP),Y
   LDY   #$0B
   LDA   (FP),Y
   LDY   #$4A
   SBC   (FP),Y
   LDY   #$11
   STA   (FP),Y
   LDY   #$0C
   LDA   (FP),Y
   LDY   #$4B
   SBC   (FP),Y
   LDY   #$12
   STA   (FP),Y
   LDY   #$0D
   LDA   (FP),Y
   LDY   #$13
   STA   (FP),Y
   LDY   #$0E
   LDA   (FP),Y
   LDY   #$14
   STA   (FP),Y
   LDY   #$0F
   LDA   (FP),Y
   LDY   #$15
   STA   (FP),Y
   LDY   #$10
   LDA   (FP),Y
   LDY   #$16
   STA   (FP),Y
   LDY   #$11
   LDA   (FP),Y
   LDY   #$17
   STA   (FP),Y
   LDY   #$12
   LDA   (FP),Y
   AND   #$07
   LDY   #$18
   STA   (FP),Y
   LDA   #$07
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDA   #$03
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$19
   LDA   (FP),Y
   TAX
   BEQ   .shift_done@4
.shift_loop@3:
   LDY   #$1B
   LDA   (FP),Y
   ASL
   STA   (FP),Y
   INY
   LDA   (FP),Y
   ROL
   STA   (FP),Y
   DEX
   BNE   .shift_loop@3
.shift_done@4:
   LDY   #$1B
   LDA   (FP),Y
   LDY   #$1D
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$1E
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@5
   LDA   #$00
   JMP   .sx_done@6
.sx_neg@5:
   LDA   #$FF
.sx_done@6:
   LDY   #$1F
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$1D
   LDA   (FP),Y
   LDY   #$23
   STA   (FP),Y
   LDY   #$1E
   LDA   (FP),Y
   LDY   #$24
   STA   (FP),Y
   LDY   #$1F
   LDA   (FP),Y
   LDY   #$25
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   AND   #$07
   INY
   STA   (FP),Y
   LDY   #$18
   LDA   (FP),Y
   LDY   #$28
   CMP   (FP),Y
   BNE   .thread@0
   LDY   #$17
   LDA   (FP),Y
   LDY   #$27
   CMP   (FP),Y
   BNE   .thread@0
   LDY   #$16
   LDA   (FP),Y
   LDY   #$26
   CMP   (FP),Y
   BNE   .thread@0
   LDY   #$15
   LDA   (FP),Y
   LDY   #$25
   CMP   (FP),Y
   BNE   .thread@0
   LDY   #$14
   LDA   (FP),Y
   LDY   #$24
   CMP   (FP),Y
   BNE   .thread@0
   LDY   #$13
   LDA   (FP),Y
   LDY   #$23
   CMP   (FP),Y
   BEQ   .cmp_true@7
.thread@0:
   LDA   #$00
   JMP   .cmp_end@8
.cmp_true@7:
   LDA   #$01
.cmp_end@8:
   LDY   #$29
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$2B
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@10
   LDA   #$00
   JMP   .sx_done@11
.sx_neg@10:
   LDA   #$FF
.sx_done@11:
   LDY   #$2E
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$2D
   LDA   (FP),Y
   LDY   #$31
   STA   (FP),Y
   LDY   #$2E
   LDA   (FP),Y
   LDY   #$32
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
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
   BVC   .cmp_novf@12
   EOR   #$80
.cmp_novf@12:
   ASL
   LDA   #$00
   ROL
   LDY   #$35
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$37
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$38
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@13
   LDA   #$00
   JMP   .sx_done@14
.sx_neg@13:
   LDA   #$FF
.sx_done@14:
   LDY   #$39
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$37
   LDA   (FP),Y
   LDY   #$3B
   STA   (FP),Y
   LDY   #$38
   LDA   (FP),Y
   LDY   #$3C
   STA   (FP),Y
   LDY   #$39
   LDA   (FP),Y
   LDY   #$3D
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$2C
   LDA   (FP),Y
   LDY   #$3F
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@15
   LDA   #$00
   JMP   .sx_done@16
.sx_neg@15:
   LDA   #$FF
.sx_done@16:
   LDY   #$40
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ORA   #$64
   LDY   #$41
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$42
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$43
   STA   (FP),Y

   ; epilogue
   PHA
   CLC
   LDA   FP
   ADC   #$4B
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

   ; prologue: 14 arg bytes, 158 local bytes
   SEC
   LDA   SSP
   SBC   #$A0
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$9F
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   LDA   g0
   ORA   g0+1
   ORA   g0+2
   ORA   g0+3
   BNE   .lb_skip@0
   JMP   .and_false@1
.lb_skip@0:
   LDA   g0
   LDY   #$01
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
   LDA   g0+3
   BMI   .sx_neg@17
   LDA   #$00
   JMP   .sx_done@18
.sx_neg@17:
   LDA   #$FF
.sx_done@18:
   LDY   #$05
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
   AND   #$07
   INY
   STA   (FP),Y
   LDY   #$A3
   LDA   (FP),Y
   ORA   #$01
   LDY   #$0D
   STA   (FP),Y
   LDY   #$A4
   LDA   (FP),Y
   LDY   #$0E
   STA   (FP),Y
   LDY   #$A5
   LDA   (FP),Y
   LDY   #$0F
   STA   (FP),Y
   LDY   #$A6
   LDA   (FP),Y
   LDY   #$10
   STA   (FP),Y
   LDY   #$A7
   LDA   (FP),Y
   LDY   #$11
   STA   (FP),Y
   LDY   #$A8
   LDA   (FP),Y
   LDY   #$12
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
   LDA   HARGS+24
   LDY   #$13
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
   LDY   #$19
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@2:
   LDY   #$19
   LDA   (FP),Y
   LDY   #$1B
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$1C
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@19
   LDA   #$00
   JMP   .sx_done@20
.sx_neg@19:
   LDA   #$FF
.sx_done@20:
   LDY   #$1D
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$1B
   LDA   (FP),Y
   LDY   #$21
   STA   (FP),Y
   LDY   #$1C
   LDA   (FP),Y
   LDY   #$22
   STA   (FP),Y
   LDY   #$1D
   LDA   (FP),Y
   LDY   #$23
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   AND   #$07
   INY
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
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$26
   LDA   (FP),Y
   LDY   #$2C
   STA   (FP),Y
   LDY   #$A1
   LDA   (FP),Y
   LDY   #$2D
   STA   (FP),Y
   LDY   #$A2
   LDA   (FP),Y
   LDY   #$2E
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$2D
   LDA   (FP),Y
   LDY   #$31
   STA   (FP),Y
   LDY   #$2E
   LDA   (FP),Y
   LDY   #$32
   STA   (FP),Y
   LDY   #$2F
   LDA   (FP),Y
   LDY   #$33
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$AB
   LDA   (FP),Y
   LDY   #$35
   STA   (FP),Y
   LDY   #$AC
   LDA   (FP),Y
   LDY   #$36
   STA   (FP),Y
   LDY   #$AD
   LDA   (FP),Y
   LDY   #$37
   STA   (FP),Y
   LDY   #$AE
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
   LDY   #$37
   LDA   (FP),Y
   LDY   #$3B
   STA   (FP),Y
   LDY   #$38
   LDA   (FP),Y
   LDY   #$3C
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@21
   LDA   #$00
   JMP   .sx_done@22
.sx_neg@21:
   LDA   #$FF
.sx_done@22:
   LDY   #$3D
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$39
   LDA   (FP),Y
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
   AND   #$07
   INY
   STA   (FP),Y
   LDA   #$07
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   ORA   #$00
   BPL   .sx_done@24
   LDA   #$FF
.sx_done@24:
   LDY   #$47
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   g0
   LDY   #$45
   AND   (FP),Y
   LDY   #$49
   STA   (FP),Y
   LDA   g0+1
   LDY   #$46
   AND   (FP),Y
   LDY   #$4A
   STA   (FP),Y
   LDA   g0+2
   LDY   #$47
   AND   (FP),Y
   LDY   #$4B
   STA   (FP),Y
   LDA   g0+3
   LDY   #$48
   AND   (FP),Y
   LDY   #$4C
   STA   (FP),Y
   LDY   #$27
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
   LDY   #$49
   LDA   (FP),Y
   STA   HARGS+8
   JSR   asl64
   LDA   HARGS+9
   LDY   #$4D
   STA   (FP),Y
   LDA   HARGS+10
   INY
   STA   (FP),Y
   LDA   HARGS+11
   INY
   STA   (FP),Y
   LDA   HARGS+12
   INY
   STA   (FP),Y
   LDA   HARGS+13
   INY
   STA   (FP),Y
   LDA   HARGS+14
   INY
   STA   (FP),Y
   LDY   #$4D
   LDA   (FP),Y
   LDY   #$53
   STA   (FP),Y
   LDY   #$4E
   LDA   (FP),Y
   LDY   #$54
   STA   (FP),Y
   LDY   #$4F
   LDA   (FP),Y
   LDY   #$55
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
   AND   #$07
   LDY   #$58
   STA   (FP),Y
   LDY   #$53
   LDA   (FP),Y
   CLC
   LDY   #$3F
   SBC   (FP),Y
   LDY   #$54
   LDA   (FP),Y
   LDY   #$40
   SBC   (FP),Y
   LDY   #$55
   LDA   (FP),Y
   LDY   #$41
   SBC   (FP),Y
   LDY   #$56
   LDA   (FP),Y
   LDY   #$42
   SBC   (FP),Y
   LDY   #$57
   LDA   (FP),Y
   LDY   #$43
   SBC   (FP),Y
   LDY   #$58
   LDA   (FP),Y
   LDY   #$44
   SBC   (FP),Y
   LDA   #$00
   ROL
   LDY   #$59
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$5B
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$5C
   STA   (FP),Y
   LDY   #$A1
   LDA   (FP),Y
   LDY   #$5D
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$5E
   LDA   (FP),Y
   LDY   #$61
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ORA   #$01
   LDY   #$63
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$64
   STA   (FP),Y
   LDY   #$5F
   LDA   (FP),Y
   STA   HARGS
   INY
   LDA   (FP),Y
   STA   HARGS+1
   LDY   #$63
   LDA   (FP),Y
   STA   HARGS+2
   INY
   LDA   (FP),Y
   STA   HARGS+3
   JSR   sdivmod16
   LDA   HARGS+4
   LDY   #$65
   STA   (FP),Y
   LDA   HARGS+5
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$67
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$68
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@25
   LDA   #$00
   JMP   .sx_done@26
.sx_neg@25:
   LDA   #$FF
.sx_done@26:
   LDY   #$69
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$67
   LDA   (FP),Y
   LDY   #$6D
   STA   (FP),Y
   LDY   #$68
   LDA   (FP),Y
   LDY   #$6E
   STA   (FP),Y
   LDY   #$69
   LDA   (FP),Y
   LDY   #$6F
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   AND   #$07
   INY
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$06
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDY   #$6D
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$6E
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$6F
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$70
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   LDY   #$71
   LDA   (FP),Y
   LDY   #$05
   STA   (SSP),Y
   LDY   #$72
   LDA   (FP),Y
   LDY   #$06
   STA   (SSP),Y
   JSR   f2
   LDY   #$73
   STA   (FP),Y
   INY
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@27
   LDA   #$00
   JMP   .sx_done@28
.sx_neg@27:
   LDA   #$FF
.sx_done@28:
   LDY   #$75
   STA   (FP),Y
   LDY   #$5B
   LDA   (FP),Y
   LDY   #$74
   EOR   (FP),Y
   LDY   #$76
   STA   (FP),Y
   LDY   #$5C
   LDA   (FP),Y
   LDY   #$75
   EOR   (FP),Y
   LDY   #$77
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$5B
   STA   (FP),Y
   LDY   #$77
   LDA   (FP),Y
   LDY   #$5C
   STA   (FP),Y
   LDY   #$5E
   LDA   (FP),Y
   LDY   #$78
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
   LDY   #$78
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$79
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   JSR   rec
   LDY   #$7A
   STA   (FP),Y
   TXA
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$7C
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$7D
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@29
   LDA   #$00
   JMP   .sx_done@30
.sx_neg@29:
   LDA   #$FF
.sx_done@30:
   LDY   #$7E
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$7C
   LDA   (FP),Y
   LDY   #$80
   STA   (FP),Y
   LDY   #$7D
   LDA   (FP),Y
   LDY   #$81
   STA   (FP),Y
   LDY   #$7E
   LDA   (FP),Y
   LDY   #$82
   STA   (FP),Y
   AND   #$01
   INY
   STA   (FP),Y
   LDY   #$31
   LDA   (FP),Y
   CLC
   LDY   #$80
   ADC   (FP),Y
   LDY   #$84
   STA   (FP),Y
   LDY   #$32
   LDA   (FP),Y
   LDY   #$81
   ADC   (FP),Y
   LDY   #$85
   STA   (FP),Y
   LDY   #$33
   LDA   (FP),Y
   LDY   #$82
   ADC   (FP),Y
   LDY   #$86
   STA   (FP),Y
   LDY   #$34
   LDA   (FP),Y
   LDY   #$83
   ADC   (FP),Y
   LDY   #$87
   STA   (FP),Y
   LDY   #$84
   LDA   (FP),Y
   LDY   #$88
   STA   (FP),Y
   LDY   #$85
   LDA   (FP),Y
   LDY   #$89
   STA   (FP),Y
   LDY   #$86
   LDA   (FP),Y
   LDY   #$8A
   STA   (FP),Y
   LDY   #$87
   LDA   (FP),Y
   AND   #$01
   LDY   #$8B
   STA   (FP),Y
   LDY   #$88
   LDA   (FP),Y
   LDY   #$31
   STA   (FP),Y
   LDY   #$89
   LDA   (FP),Y
   LDY   #$32
   STA   (FP),Y
   LDY   #$8A
   LDA   (FP),Y
   LDY   #$33
   STA   (FP),Y
   LDY   #$8B
   LDA   (FP),Y
   LDY   #$34
   STA   (FP),Y
   LDY   #$A3
   LDA   (FP),Y
   EOR   #$07
   LDY   #$8C
   STA   (FP),Y
   LDY   #$A4
   LDA   (FP),Y
   LDY   #$8D
   STA   (FP),Y
   LDY   #$A5
   LDA   (FP),Y
   LDY   #$8E
   STA   (FP),Y
   LDY   #$A6
   LDA   (FP),Y
   LDY   #$8F
   STA   (FP),Y
   LDY   #$A7
   LDA   (FP),Y
   LDY   #$90
   STA   (FP),Y
   LDY   #$A8
   LDA   (FP),Y
   LDY   #$91
   STA   (FP),Y
   LDY   #$8C
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
   LDY   #$01
   STY   HARGS+8
   DEY
   STY   HARGS+9
   STY   HARGS+10
   STY   HARGS+11
   STY   HARGS+12
   STY   HARGS+13
   STA   HARGS+14
   STA   HARGS+15
   JSR   mul64
   LDA   HARGS+16
   LDY   #$92
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
   LDY   #$92
   LDA   (FP),Y
   LDY   #$98
   STA   (FP),Y
   LDY   #$93
   LDA   (FP),Y
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
   AND   #$07
   LDY   #$9D
   STA   (FP),Y
   LDY   #$98
   LDA   (FP),Y
   LDY   #$9E
   STA   (FP),Y

   ; epilogue
   PHA
   CLC
   LDA   FP
   ADC   #$AE
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

   ; prologue: 11 arg bytes, 88 local bytes
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
   SEC
   SBC   g0
   LDA   g0+1
   SBC   g0+1
   LDA   g0+2
   SBC   g0+2
   LDA   g0+3
   SBC   g0+3
   BVC   .cmp_novf@31
   EOR   #$80
.cmp_novf@31:
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
   LDY   #$03
   STA   (FP),Y
   INY
   STA   (FP),Y
   ORA   #$00
   BEQ   .and_false@3
   LDY   #$63
   LDA   (FP),Y
   BEQ   .and_false@3
   LDA   #$01
   LDY   #$05
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   JMP   .and_end@4
.and_false@3:
   LDA   #$00
   LDY   #$05
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@4:
   LDY   #$05
   LDA   (FP),Y
   LDY   #$07
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$08
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@32
   LDA   #$00
   JMP   .sx_done@33
.sx_neg@32:
   LDA   #$FF
.sx_done@33:
   LDY   #$09
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$07
   LDA   (FP),Y
   LDY   #$0D
   STA   (FP),Y
   LDY   #$08
   LDA   (FP),Y
   LDY   #$0E
   STA   (FP),Y
   LDY   #$09
   LDA   (FP),Y
   LDY   #$0F
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   AND   #$07
   INY
   STA   (FP),Y
   LDY   #$61
   LDA   (FP),Y
   LDY   #$13
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@34
   LDA   #$00
   JMP   .sx_done@35
.sx_neg@34:
   LDA   #$FF
.sx_done@35:
   LDY   #$14
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$15
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$16
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@36
   LDA   #$00
   JMP   .sx_done@37
.sx_neg@36:
   LDA   #$FF
.sx_done@37:
   LDY   #$17
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
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
   INY
   STA   (FP),Y
   AND   #$07
   INY
   STA   (FP),Y
   LDY   #$5B
   LDA   (FP),Y
   LDY   #$1B
   EOR   (FP),Y
   LDY   #$21
   STA   (FP),Y
   LDY   #$5C
   LDA   (FP),Y
   LDY   #$1C
   EOR   (FP),Y
   LDY   #$22
   STA   (FP),Y
   LDY   #$5D
   LDA   (FP),Y
   LDY   #$1D
   EOR   (FP),Y
   LDY   #$23
   STA   (FP),Y
   LDY   #$5E
   LDA   (FP),Y
   LDY   #$1E
   EOR   (FP),Y
   LDY   #$24
   STA   (FP),Y
   LDY   #$5F
   LDA   (FP),Y
   LDY   #$1F
   EOR   (FP),Y
   LDY   #$25
   STA   (FP),Y
   LDY   #$60
   LDA   (FP),Y
   LDY   #$20
   EOR   (FP),Y
   LDY   #$26
   STA   (FP),Y
   LDY   #$0D
   LDA   (FP),Y
   CLC
   LDY   #$21
   ADC   (FP),Y
   LDY   #$27
   STA   (FP),Y
   LDY   #$0E
   LDA   (FP),Y
   LDY   #$22
   ADC   (FP),Y
   LDY   #$28
   STA   (FP),Y
   LDY   #$0F
   LDA   (FP),Y
   LDY   #$23
   ADC   (FP),Y
   LDY   #$29
   STA   (FP),Y
   LDY   #$10
   LDA   (FP),Y
   LDY   #$24
   ADC   (FP),Y
   LDY   #$2A
   STA   (FP),Y
   LDY   #$11
   LDA   (FP),Y
   LDY   #$25
   ADC   (FP),Y
   LDY   #$2B
   STA   (FP),Y
   LDY   #$12
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
   AND   #$07
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
   DEY
   LDA   (FP),Y
   LDY   #$35
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$36
   STA   (FP),Y
   LDY   #$62
   LDA   (FP),Y
   LDY   #$37
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@38
   LDA   #$00
   JMP   .sx_done@39
.sx_neg@38:
   LDA   #$FF
.sx_done@39:
   LDY   #$3A
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$3B
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$3C
   STA   (FP),Y
   LDY   #$64
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
   LDY   #$63
   LDA   (FP),Y
   LDY   #$3F
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$41
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$42
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$35
   ORA   (FP),Y
   LDY   #$43
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$36
   ORA   (FP),Y
   LDY   #$44
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$45
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$46
   STA   (FP),Y
   LDY   #$3D
   LDA   (FP),Y
   CLC
   LDY   #$45
   ADC   (FP),Y
   LDY   #$47
   STA   (FP),Y
   LDY   #$3E
   LDA   (FP),Y
   LDY   #$46
   ADC   (FP),Y
   LDY   #$48
   STA   (FP),Y
   LDY   #$64
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$47
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$48
   LDA   (FP),Y
   LDY   #$01
   STA   (DPTR),Y
   LDY   #$35
   LDA   (FP),Y
   LDY   #$49
   STA   (FP),Y
   LDY   #$36
   LDA   (FP),Y
   LDY   #$4A
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
   LDY   #$49
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$4A
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   JSR   rec
   LDY   #$4B
   STA   (FP),Y
   TXA
   INY
   STA   (FP),Y
   LDY   #$04
   LDA   (FP),Y
   LDY   #$4D
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   LDY   #$4B
   ADC   (FP),Y
   LDY   #$4F
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$4C
   ADC   (FP),Y
   LDY   #$50
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$51
   STA   (FP),Y
   LDY   #$04
   STA   (FP),Y
   LDY   #$61
   LDA   (FP),Y
   LDY   #$52
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@40
   LDA   #$00
   JMP   .sx_done@41
.sx_neg@40:
   LDA   #$FF
.sx_done@41:
   LDY   #$53
   STA   (FP),Y
   LDY   #$61
   LDA   (FP),Y
   LDY   #$54
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@42
   LDA   #$00
   JMP   .sx_done@43
.sx_neg@42:
   LDA   #$FF
.sx_done@43:
   LDY   #$55
   STA   (FP),Y
   LDY   #$52
   LDA   (FP),Y
   LDY   #$54
   EOR   (FP),Y
   LDY   #$56
   STA   (FP),Y
   LDY   #$53
   LDA   (FP),Y
   LDY   #$55
   EOR   (FP),Y
   LDY   #$57
   STA   (FP),Y
   DEY
   ORA   (FP),Y
   BEQ   .cmp_end@44
   LDA   #$01
.cmp_end@44:
   LDY   #$58
   STA   (FP),Y

   ; epilogue
   PHA
   CLC
   LDA   FP
   ADC   #$65
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

   ; prologue: 0 arg bytes, 30 local bytes
   SEC
   LDA   SSP
   SBC   #$20
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$1F
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   LDA   #$05
   LDY   #$01
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDA   #$61
   INY
   STA   (FP),Y
   LDA   #$12
   INY
   STA   (FP),Y
   LDA   #$E5
   INY
   STA   (FP),Y
   CLC
   LDA   FP
   ADC   #$01
   INY
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$0B
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   #$62
   LDY   #$01
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
   LDY   #$03
   LDA   (FP),Y
   LDY   #$07
   STA   (SSP),Y
   LDY   #$04
   LDA   (FP),Y
   LDY   #$08
   STA   (SSP),Y
   LDY   #$05
   LDA   (FP),Y
   LDY   #$09
   STA   (SSP),Y
   LDY   #$06
   LDA   (FP),Y
   LDY   #$0A
   STA   (SSP),Y
   LDY   #$07
   LDA   (FP),Y
   LDY   #$0B
   STA   (SSP),Y
   JSR   f0
   LDY   #$08
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
   LDY   #$09
   LDA   (FP),Y
   LDY   #$0D
   STA   (FP),Y
   LDY   #$0A
   LDA   (FP),Y
   LDY   #$0E
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$0D
   LDA   (FP),Y
   EOR   g0
   LDY   #$11
   STA   (FP),Y
   LDY   #$0E
   LDA   (FP),Y
   EOR   g0+1
   LDY   #$12
   STA   (FP),Y
   LDY   #$0F
   LDA   (FP),Y
   EOR   g0+2
   LDY   #$13
   STA   (FP),Y
   LDY   #$10
   LDA   (FP),Y
   EOR   g0+3
   LDY   #$14
   STA   (FP),Y
   LDY   #$11
   LDA   (FP),Y
   LDY   #$0D
   STA   (FP),Y
   LDY   #$12
   LDA   (FP),Y
   LDY   #$0E
   STA   (FP),Y
   LDY   #$13
   LDA   (FP),Y
   LDY   #$0F
   STA   (FP),Y
   LDY   #$14
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
   LDY   #$15
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
   LDY   #$15
   EOR   (FP),Y
   LDY   #$19
   STA   (FP),Y
   LDY   #$0E
   LDA   (FP),Y
   LDY   #$16
   EOR   (FP),Y
   LDY   #$1A
   STA   (FP),Y
   LDY   #$0F
   LDA   (FP),Y
   LDY   #$17
   EOR   (FP),Y
   LDY   #$1B
   STA   (FP),Y
   LDY   #$10
   LDA   (FP),Y
   LDY   #$18
   EOR   (FP),Y
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
   TAX
   DEY
   LDA   (FP),Y

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$20
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$1F
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
   DC.L  $0000003E
