SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24

f4:
   SUBROUTINE

   ; prologue: 8 arg bytes, 39 local bytes
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

   LDY   #$30
   LDA   (FP),Y
   AND   #$07
   LDY   #$01
   STA   (FP),Y
   LDY   #$31
   LDA   (FP),Y
   LDA   #$00
   LDY   #$02
   STA   (FP),Y
   LDY   #$30
   LDA   (FP),Y
   LDY   #$03
   STA   (FP),Y
   LDY   #$31
   LDA   (FP),Y
   LDY   #$04
   STA   (FP),Y
   LDY   #$01
   LDA   (FP),Y
   TAX
   BEQ   .shift_done@1
.shift_loop@0:
   LDY   #$03
   LDA   (FP),Y
   ASL
   STA   (FP),Y
   INY
   LDA   (FP),Y
   ROL
   STA   (FP),Y
   DEX
   BNE   .shift_loop@0
.shift_done@1:
   LDY   #$03
   LDA   (FP),Y
   LDY   #$05
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$06
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
   BMI   .sx_neg@2
   LDA   #$00
   JMP   .sx_done@3
.sx_neg@2:
   LDA   #$FF
.sx_done@3:
   LDY   #$09
   STA   (FP),Y
   LDY   #$07
   LDA   (FP),Y
   LDY   #$0A
   STA   (FP),Y
   LDY   #$08
   LDA   (FP),Y
   LDY   #$0B
   STA   (FP),Y
   LDY   #$09
   LDA   (FP),Y
   AND   #$03
   LDY   #$0C
   STA   (FP),Y
   LDY   #$0A
   LDA   (FP),Y
   LDY   #$0D
   STA   (FP),Y
   LDY   #$0B
   LDA   (FP),Y
   LDY   #$0E
   STA   (FP),Y
   LDY   #$0C
   LDA   (FP),Y
   LDY   #$0F
   STA   (FP),Y
   LDA   g0
   INY
   STA   (FP),Y
   LDA   g0
   BMI   .sx_neg@4
   LDA   #$00
   JMP   .sx_done@5
.sx_neg@4:
   LDA   #$FF
.sx_done@5:
   LDY   #$11
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$12
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$13
   STA   (FP),Y
   LDA   #$7F
   BNE   .thread@0
   LDA   #$FF
   CMP   #$64
   BEQ   .cmp_true@6
.thread@0:
   LDA   #$00
   JMP   .cmp_end@7
.cmp_true@6:
   LDA   #$01
.cmp_end@7:
   LDY   #$14
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDA   g1
   INY
   STA   (FP),Y
   LDA   g1
   BMI   .sx_neg@9
   LDA   #$00
   JMP   .sx_done@10
.sx_neg@9:
   LDA   #$FF
.sx_done@10:
   LDY   #$17
   STA   (FP),Y
   LDY   #$14
   LDA   (FP),Y
   SEC
   LDY   #$16
   SBC   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$17
   SBC   (FP),Y
   BVC   .cmp_novf@11
   EOR   #$80
.cmp_novf@11:
   ASL
   LDA   #$00
   ROL
   EOR   #$01
   LDY   #$18
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$1A
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   g0
   INY
   STA   (FP),Y
   LDA   g0
   BMI   .sx_neg@12
   LDA   #$00
   JMP   .sx_done@13
.sx_neg@12:
   LDA   #$FF
.sx_done@13:
   LDY   #$1D
   STA   (FP),Y
   LDY   #$1B
   LDA   (FP),Y
   LDY   #$1E
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   AND   #$07
   LDY   #$20
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDA   #$00
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
   LDY   #$20
   LDA   (FP),Y
   TAX
   BEQ   .shift_done@15
.shift_loop@14:
   LDY   #$22
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
   LDY   #$2C
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@0
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@0
   LDA   #$00
   LDY   #$24
   STA   (FP),Y
   INY
   STA   (FP),Y
   JMP   .or_end@1
.or_true@0:
   LDA   #$01
   LDY   #$24
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@1:
   LDY   #$23
   LDA   (FP),Y
   LDY   #$25
   CMP   (FP),Y
   BNE   .thread@1
   LDY   #$22
   LDA   (FP),Y
   LDY   #$24
   CMP   (FP),Y
   BEQ   .cmp_true@16
.thread@1:
   LDA   #$00
   JMP   .cmp_end@17
.cmp_true@16:
   LDA   #$01
.cmp_end@17:
   LDY   #$26
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
   ADC   #$31
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

f3:
   SUBROUTINE

   ; prologue: 3 arg bytes, 68 local bytes
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

   LDA   g1
   LDY   #$01
   STA   (FP),Y
   LDA   g1
   BMI   .sx_neg@19
   LDA   #$00
   JMP   .sx_done@20
.sx_neg@19:
   LDA   #$FF
.sx_done@20:
   LDY   #$02
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$03
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$04
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@21
   LDA   #$00
   JMP   .sx_done@22
.sx_neg@21:
   LDA   #$FF
.sx_done@22:
   LDY   #$05
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
   BPL   .sx_done@24
   LDA   #$FF
.sx_done@24:
   LDY   #$09
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   g2
   LDY   #$07
   ORA   (FP),Y
   LDY   #$0B
   STA   (FP),Y
   LDA   g2+1
   LDY   #$08
   ORA   (FP),Y
   LDY   #$0C
   STA   (FP),Y
   LDA   g2+2
   LDY   #$09
   ORA   (FP),Y
   LDY   #$0D
   STA   (FP),Y
   LDA   g2+3
   LDY   #$0A
   ORA   (FP),Y
   LDY   #$0E
   STA   (FP),Y
   LDY   #$03
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
   LDY   #$0B
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
   LDY   #$0F
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
   DEY
   LDA   (FP),Y
   LDY   #$17
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$18
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@27
   LDA   #$00
   JMP   .sx_done@28
.sx_neg@27:
   LDA   #$FF
.sx_done@28:
   LDY   #$19
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$0F
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
   LDY   #$17
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
   LDY   #$1B
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
   LDY   #$22
   LDA   (FP),Y
   LDY   #$26
   STA   (FP),Y
   SEC
   LDA   #$7F
   SBC   #$00
   BVC   .cmp_novf@29
   EOR   #$80
.cmp_novf@29:
   ASL
   LDA   #$00
   ROL
   EOR   #$01
   LDY   #$27
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@2
   LDA   g0
   INY
   STA   (FP),Y
   LDA   g0
   BMI   .sx_neg@30
   LDA   #$00
   JMP   .sx_done@31
.sx_neg@30:
   LDA   #$FF
.sx_done@31:
   LDY   #$2A
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
   BMI   .sx_neg@32
   LDA   #$00
   JMP   .sx_done@33
.sx_neg@32:
   LDA   #$FF
.sx_done@33:
   LDY   #$2D
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   g2
   SEC
   LDY   #$2B
   SBC   (FP),Y
   LDA   g2+1
   INY
   SBC   (FP),Y
   LDA   g2+2
   INY
   SBC   (FP),Y
   LDA   g2+3
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
   INY
   ORA   (FP),Y
   BEQ   .and_false@2
   LDA   #$01
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   JMP   .and_end@3
.and_false@2:
   LDA   #$00
   LDY   #$31
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@3:
   LDY   #$31
   LDA   (FP),Y
   LDY   #$33
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$34
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@34
   LDA   #$00
   JMP   .sx_done@35
.sx_neg@34:
   LDA   #$FF
.sx_done@35:
   LDY   #$35
   STA   (FP),Y
   INY
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
   INY
   STA   (FP),Y
   LDA   #$07
   BNE   .or_true@4
   LDA   g1
   BNE   .or_true@4
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   JMP   .or_end@5
.or_true@4:
   LDA   #$01
   LDY   #$3B
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@5:
   LDY   #$37
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   INY
   ORA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lnot_true@36
   LDA   #$00
   JMP   .lnot_end@37
.lnot_true@36:
   LDA   #$01
.lnot_end@37:
   LDY   #$3D
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   AND   #$07
   LDY   #$3F
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDA   #$00
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
   LDY   #$3F
   LDA   (FP),Y
   TAX
   BEQ   .shift_done@39
.shift_loop@38:
   LDY   #$42
   LDA   (FP),Y
   LSR
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ROR
   STA   (FP),Y
   DEX
   BNE   .shift_loop@38
.shift_done@39:
   LDY   #$41
   LDA   (FP),Y
   LDY   #$43
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$44
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
   ADC   #$49
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$45
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

   ; prologue: 10 arg bytes, 39 local bytes
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

   LDA   g1
   LDY   #$01
   STA   (FP),Y
   LDA   g1
   BMI   .sx_neg@40
   LDA   #$00
   JMP   .sx_done@41
.sx_neg@40:
   LDA   #$FF
.sx_done@41:
   LDY   #$02
   STA   (FP),Y
   LDA   #$65
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$01
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
   LDA   HARGS+4
   LDY   #$05
   STA   (FP),Y
   LDA   HARGS+5
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
   BMI   .sx_neg@42
   LDA   #$00
   JMP   .sx_done@43
.sx_neg@42:
   LDA   #$FF
.sx_done@43:
   LDY   #$09
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$30
   LDA   (FP),Y
   SEC
   SBC   (FP),Y
   LDY   #$0B
   STA   (FP),Y
   LDY   #$31
   LDA   (FP),Y
   SBC   (FP),Y
   LDY   #$0C
   STA   (FP),Y
   LDY   #$32
   LDA   (FP),Y
   SBC   (FP),Y
   LDY   #$0D
   STA   (FP),Y
   LDY   #$33
   LDA   (FP),Y
   SBC   (FP),Y
   LDY   #$0E
   STA   (FP),Y
   LDY   #$07
   LDA   (FP),Y
   SEC
   LDY   #$0B
   SBC   (FP),Y
   LDY   #$08
   LDA   (FP),Y
   LDY   #$0C
   SBC   (FP),Y
   LDY   #$09
   LDA   (FP),Y
   LDY   #$0D
   SBC   (FP),Y
   LDY   #$0A
   LDA   (FP),Y
   LDY   #$0E
   SBC   (FP),Y
   BVC   .cmp_novf@44
   EOR   #$80
.cmp_novf@44:
   ASL
   LDA   #$00
   ROL
   LDY   #$0F
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$11
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   g1
   INY
   STA   (FP),Y
   LDA   g1
   BMI   .sx_neg@45
   LDA   #$00
   JMP   .sx_done@46
.sx_neg@45:
   LDA   #$FF
.sx_done@46:
   LDY   #$14
   STA   (FP),Y
   LDY   #$12
   LDA   (FP),Y
   LDY   #$15
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@47
   LDA   #$00
   JMP   .sx_done@48
.sx_neg@47:
   LDA   #$FF
.sx_done@48:
   LDY   #$16
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDA   #$00
   LDY   #$17
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDA   #$00
   LDY   #$18
   STA   (FP),Y
   LDY   #$14
   LDA   (FP),Y
   LDY   #$18
   CMP   (FP),Y
   BNE   .thread@2
   LDY   #$13
   LDA   (FP),Y
   LDY   #$17
   CMP   (FP),Y
   BEQ   .cmp_true@49
.thread@2:
   LDA   #$00
   JMP   .cmp_end@50
.cmp_true@49:
   LDA   #$01
.cmp_end@50:
   LDY   #$19
   STA   (FP),Y
   LDA   #$00
   INY
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
   BMI   .sx_neg@52
   LDA   #$00
   JMP   .sx_done@53
.sx_neg@52:
   LDA   #$FF
.sx_done@53:
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
   LDA   g1
   INY
   STA   (FP),Y
   LDA   g1
   BMI   .sx_neg@54
   LDA   #$00
   JMP   .sx_done@55
.sx_neg@54:
   LDA   #$FF
.sx_done@55:
   LDY   #$24
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$25
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$26
   STA   (FP),Y
   LDA   #$FF
   INY
   STA   (FP),Y

   ; epilogue
   PHA
   CLC
   LDA   FP
   ADC   #$33
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

   ; prologue: 10 arg bytes, 39 local bytes
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

   LDY   #$2B
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@56
   LDA   #$00
   JMP   .sx_done@57
.sx_neg@56:
   LDA   #$FF
.sx_done@57:
   LDY   #$02
   STA   (FP),Y
   LDA   g0
   INY
   STA   (FP),Y
   LDA   g0
   BMI   .sx_neg@58
   LDA   #$00
   JMP   .sx_done@59
.sx_neg@58:
   LDA   #$FF
.sx_done@59:
   LDY   #$04
   STA   (FP),Y
   LDY   #$01
   LDA   (FP),Y
   LDY   #$03
   EOR   (FP),Y
   LDY   #$05
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   LDY   #$04
   EOR   (FP),Y
   LDY   #$06
   STA   (FP),Y
   SEC
   LDA   #$00
   DEY
   SBC   (FP),Y
   LDY   #$07
   STA   (FP),Y
   LDA   #$00
   DEY
   SBC   (FP),Y
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
   DEY
   LDA   (FP),Y
   LDY   #$0C
   STA   (FP),Y
   LDA   g1
   INY
   STA   (FP),Y
   LDA   #$E8
   EOR   #$FF
   INY
   STA   (FP),Y
   LDA   #$03
   EOR   #$FF
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$10
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$11
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@60
   LDA   #$00
   JMP   .sx_done@61
.sx_neg@60:
   LDA   #$FF
.sx_done@61:
   LDY   #$12
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   g1
   INY
   STA   (FP),Y
   LDA   g1
   BMI   .sx_neg@62
   LDA   #$00
   JMP   .sx_done@63
.sx_neg@62:
   LDA   #$FF
.sx_done@63:
   LDY   #$15
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$16
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$17
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@64
   LDA   #$00
   JMP   .sx_done@65
.sx_neg@64:
   LDA   #$FF
.sx_done@65:
   LDY   #$18
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$16
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
   LDY   #$30
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
   LDY   #$1A
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
   LDY   #$10
   LDA   (FP),Y
   SEC
   LDY   #$1A
   SBC   (FP),Y
   LDY   #$1E
   STA   (FP),Y
   LDY   #$11
   LDA   (FP),Y
   LDY   #$1B
   SBC   (FP),Y
   LDY   #$1F
   STA   (FP),Y
   LDY   #$12
   LDA   (FP),Y
   LDY   #$1C
   SBC   (FP),Y
   LDY   #$20
   STA   (FP),Y
   LDY   #$13
   LDA   (FP),Y
   LDY   #$1D
   SBC   (FP),Y
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
   LDY   #$0B
   LDA   (FP),Y
   LDY   #$26
   STA   (FP),Y
   LDY   #$0C
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
   ADC   #$33
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

f0:
   SUBROUTINE

   ; prologue: 11 arg bytes, 70 local bytes
   SEC
   LDA   SSP
   SBC   #$48
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$47
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   LDA   #$02
   LDY   #$01
   STA   (FP),Y
   LDA   #$00
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
   DEY
   LDA   (FP),Y
   LDY   #$06
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$07
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@66
   LDA   #$00
   JMP   .sx_done@67
.sx_neg@66:
   LDA   #$FF
.sx_done@67:
   LDY   #$08
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$4C
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
   LDY   #$06
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
   LDA   HARGS+8
   LDY   #$0A
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
   LDY   #$0A
   LDA   (FP),Y
   EOR   #$FF
   LDY   #$0E
   STA   (FP),Y
   LDY   #$0B
   LDA   (FP),Y
   EOR   #$FF
   LDY   #$0F
   STA   (FP),Y
   LDY   #$0C
   LDA   (FP),Y
   EOR   #$FF
   LDY   #$10
   STA   (FP),Y
   LDY   #$0D
   LDA   (FP),Y
   EOR   #$FF
   LDY   #$11
   STA   (FP),Y
   LDY   #$0E
   LDA   (FP),Y
   LDY   #$12
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
.loop@0_start:
   LDY   #$14
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
   LDY   #$16
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
   LDY   #$4B
   LDA   (FP),Y
   LDY   #$18
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$1A
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$1B
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@69
   LDA   #$00
   JMP   .sx_done@70
.sx_neg@69:
   LDA   #$FF
.sx_done@70:
   LDY   #$1C
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
   BPL   .sx_done@72
   LDA   #$FF
.sx_done@72:
   LDY   #$20
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$50
   LDA   (FP),Y
   LDY   #$1E
   ORA   (FP),Y
   LDY   #$22
   STA   (FP),Y
   LDY   #$51
   LDA   (FP),Y
   LDY   #$1F
   ORA   (FP),Y
   LDY   #$23
   STA   (FP),Y
   LDY   #$52
   LDA   (FP),Y
   LDY   #$20
   ORA   (FP),Y
   LDY   #$24
   STA   (FP),Y
   LDY   #$53
   LDA   (FP),Y
   LDY   #$21
   ORA   (FP),Y
   LDY   #$25
   STA   (FP),Y
   LDY   #$1A
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
   LDY   #$22
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
   LDA   HARGS+8
   LDY   #$26
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
   LDY   #$26
   LDA   (FP),Y
   LDY   #$2A
   STA   (FP),Y
   LDY   #$27
   LDA   (FP),Y
   LDY   #$2B
   STA   (FP),Y
   LDA   g0
   INY
   STA   (FP),Y
   LDA   g0
   BMI   .sx_neg@73
   LDA   #$00
   JMP   .sx_done@74
.sx_neg@73:
   LDA   #$FF
.sx_done@74:
   LDY   #$2D
   STA   (FP),Y
   LDA   #$01
   CLC
   DEY
   ADC   (FP),Y
   LDY   #$2E
   STA   (FP),Y
   LDA   #$00
   DEY
   ADC   (FP),Y
   LDY   #$2F
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$30
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$31
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@75
   LDA   #$00
   JMP   .sx_done@76
.sx_neg@75:
   LDA   #$FF
.sx_done@76:
   LDY   #$32
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$49
   LDA   (FP),Y
   LDY   #$34
   STA   (FP),Y
   LDY   #$4A
   LDA   (FP),Y
   LDY   #$35
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@77
   LDA   #$00
   JMP   .sx_done@78
.sx_neg@77:
   LDA   #$FF
.sx_done@78:
   LDY   #$36
   STA   (FP),Y
   INY
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$0A
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDY   #$2A
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$2B
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$30
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$31
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   LDY   #$32
   LDA   (FP),Y
   LDY   #$05
   STA   (SSP),Y
   LDY   #$33
   LDA   (FP),Y
   LDY   #$06
   STA   (SSP),Y
   LDY   #$34
   LDA   (FP),Y
   LDY   #$07
   STA   (SSP),Y
   LDY   #$35
   LDA   (FP),Y
   LDY   #$08
   STA   (SSP),Y
   LDY   #$36
   LDA   (FP),Y
   LDY   #$09
   STA   (SSP),Y
   LDY   #$37
   LDA   (FP),Y
   LDY   #$0A
   STA   (SSP),Y
   JSR   f2
   LDY   #$38
   STA   (FP),Y
   INY
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@79
   LDA   #$00
   JMP   .sx_done@80
.sx_neg@79:
   LDA   #$FF
.sx_done@80:
   LDY   #$3A
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   LDY   #$14
   ADC   (FP),Y
   LDY   #$3B
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$15
   ADC   (FP),Y
   LDY   #$3C
   STA   (FP),Y
   LDY   #$13
   LDA   (FP),Y
   LDY   #$3D
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   LDY   #$3B
   ADC   (FP),Y
   LDY   #$3F
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$3C
   ADC   (FP),Y
   LDY   #$40
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$41
   STA   (FP),Y
   LDY   #$13
   STA   (FP),Y
   INY
   LDA   (FP),Y
   LDY   #$42
   STA   (FP),Y
   LDY   #$15
   LDA   (FP),Y
   LDY   #$43
   STA   (FP),Y
   LDY   #$14
   LDA   (FP),Y
   CLC
   ADC   #$01
   LDY   #$44
   STA   (FP),Y
   LDY   #$15
   LDA   (FP),Y
   ADC   #$00
   LDY   #$45
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$14
   STA   (FP),Y
   LDY   #$45
   LDA   (FP),Y
   LDY   #$15
   STA   (FP),Y
   JMP   .loop@0_start
.loop@0_break:
   LDY   #$4C
   LDA   (FP),Y
   LDY   #$46
   STA   (FP),Y

   ; epilogue
   PHA
   CLC
   LDA   FP
   ADC   #$53
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

   ; prologue: 0 arg bytes, 52 local bytes
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

   LDA   #$68
   LDY   #$01
   STA   (FP),Y
   LDA   #$4A
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   ORA   #$00
   BPL   .sx_done@82
   LDA   #$FF
.sx_done@82:
   LDY   #$04
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$08
   INY
   STA   (FP),Y
   LDA   #$01
   INY
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@83
   LDA   #$00
   JMP   .sx_done@84
.sx_neg@83:
   LDA   #$FF
.sx_done@84:
   LDY   #$08
   STA   (FP),Y
   INY
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$0B
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   #$11
   LDY   #$01
   STA   (SSP),Y
   LDA   #$01
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
   DEY
   LDA   (FP),Y
   LDY   #$07
   STA   (SSP),Y
   DEY
   LDA   (FP),Y
   LDY   #$08
   STA   (SSP),Y
   DEY
   LDA   (FP),Y
   LDY   #$09
   STA   (SSP),Y
   DEY
   LDA   (FP),Y
   LDY   #$0A
   STA   (SSP),Y
   DEY
   LDA   (FP),Y
   LDY   #$0B
   STA   (SSP),Y
   JSR   f0
   LDY   #$0A
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
   LDY   #$0B
   LDA   (FP),Y
   LDY   #$0F
   STA   (FP),Y
   LDY   #$0C
   LDA   (FP),Y
   LDY   #$10
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   g0
   INY
   STA   (FP),Y
   LDA   g0
   BMI   .sx_neg@85
   LDA   #$00
   JMP   .sx_done@86
.sx_neg@85:
   LDA   #$FF
.sx_done@86:
   LDY   #$14
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   g1
   INY
   STA   (FP),Y
   LDA   g1
   BMI   .sx_neg@87
   LDA   #$00
   JMP   .sx_done@88
.sx_neg@87:
   LDA   #$FF
.sx_done@88:
   LDY   #$18
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
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
   LDY   #$15
   LDA   (FP),Y
   LDY   #$19
   EOR   (FP),Y
   LDY   #$1D
   STA   (FP),Y
   LDY   #$16
   LDA   (FP),Y
   LDY   #$1A
   EOR   (FP),Y
   LDY   #$1E
   STA   (FP),Y
   LDA   g2
   INY
   STA   (FP),Y
   LDA   g2+1
   INY
   STA   (FP),Y
   LDA   g2+2
   INY
   STA   (FP),Y
   LDA   g2+3
   INY
   STA   (FP),Y
   LDY   #$1B
   LDA   (FP),Y
   LDY   #$1F
   EOR   (FP),Y
   LDY   #$23
   STA   (FP),Y
   LDY   #$1C
   LDA   (FP),Y
   LDY   #$20
   EOR   (FP),Y
   LDY   #$24
   STA   (FP),Y
   LDY   #$1D
   LDA   (FP),Y
   LDY   #$21
   EOR   (FP),Y
   LDY   #$25
   STA   (FP),Y
   LDY   #$1E
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
   LDY   #$11
   LDA   (FP),Y
   LDY   #$25
   EOR   (FP),Y
   LDY   #$29
   STA   (FP),Y
   LDY   #$12
   LDA   (FP),Y
   LDY   #$26
   EOR   (FP),Y
   LDY   #$2A
   STA   (FP),Y
   LDY   #$27
   LDA   (FP),Y
   LDY   #$0F
   STA   (FP),Y
   LDY   #$28
   LDA   (FP),Y
   LDY   #$10
   STA   (FP),Y
   LDY   #$29
   LDA   (FP),Y
   LDY   #$11
   STA   (FP),Y
   LDY   #$2A
   LDA   (FP),Y
   LDY   #$12
   STA   (FP),Y
   LDY   #$0F
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
   LDY   #$2B
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
   LDY   #$0F
   LDA   (FP),Y
   LDY   #$2B
   EOR   (FP),Y
   LDY   #$2F
   STA   (FP),Y
   LDY   #$10
   LDA   (FP),Y
   LDY   #$2C
   EOR   (FP),Y
   LDY   #$30
   STA   (FP),Y
   LDY   #$11
   LDA   (FP),Y
   LDY   #$2D
   EOR   (FP),Y
   LDY   #$31
   STA   (FP),Y
   LDY   #$12
   LDA   (FP),Y
   LDY   #$2E
   EOR   (FP),Y
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
   TAX
   DEY
   LDA   (FP),Y

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$36
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

g0:
   DC.B  $3A

g1:
   DC.B  $63

g2:
   DC.L  $00000010
