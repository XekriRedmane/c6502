SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24

addition:
   SUBROUTINE

   ; prologue: 0 arg bytes, 26 local bytes
   SEC
   LDA   SSP
   SBC   #$1C
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$1B
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   LDA   i
   LDY   #$01
   STA   (FP),Y
   LDA   i+1
   INY
   STA   (FP),Y
   LDA   i+1
   BMI   .sx_neg@0
   LDA   #$00
   JMP   .sx_done@1
.sx_neg@0:
   LDA   #$FF
.sx_done@1:
   LDY   #$03
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
   LDY   #$01
   LDA   (FP),Y
   CLC
   ADC   l
   LDY   #$09
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   ADC   l+1
   LDY   #$0A
   STA   (FP),Y
   LDY   #$03
   LDA   (FP),Y
   ADC   l+2
   LDY   #$0B
   STA   (FP),Y
   LDY   #$04
   LDA   (FP),Y
   ADC   l+3
   LDY   #$0C
   STA   (FP),Y
   LDY   #$05
   LDA   (FP),Y
   ADC   l+4
   LDY   #$0D
   STA   (FP),Y
   LDY   #$06
   LDA   (FP),Y
   ADC   l+5
   LDY   #$0E
   STA   (FP),Y
   LDY   #$07
   LDA   (FP),Y
   ADC   l+6
   LDY   #$0F
   STA   (FP),Y
   LDY   #$08
   LDA   (FP),Y
   ADC   l+7
   LDY   #$10
   STA   (FP),Y
   LDY   #$09
   LDA   (FP),Y
   LDY   #$11
   STA   (FP),Y
   LDY   #$0A
   LDA   (FP),Y
   LDY   #$12
   STA   (FP),Y
   LDY   #$0B
   LDA   (FP),Y
   LDY   #$13
   STA   (FP),Y
   LDY   #$0C
   LDA   (FP),Y
   LDY   #$14
   STA   (FP),Y
   LDY   #$0D
   LDA   (FP),Y
   LDY   #$15
   STA   (FP),Y
   LDY   #$0E
   LDA   (FP),Y
   LDY   #$16
   STA   (FP),Y
   LDY   #$0F
   LDA   (FP),Y
   LDY   #$17
   STA   (FP),Y
   LDY   #$10
   LDA   (FP),Y
   LDY   #$18
   STA   (FP),Y
   CMP   #$00
   BNE   .thread@0
   DEY
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
   BNE   .thread@0
   DEY
   LDA   (FP),Y
   BNE   .thread@0
   DEY
   LDA   (FP),Y
   CMP   #$80
   BNE   .thread@0
   DEY
   LDA   (FP),Y
   CMP   #$8C
   BEQ   .cmp_true@2
.thread@0:
   LDA   #$00
   JMP   .cmp_end@3
.cmp_true@2:
   LDA   #$01
.cmp_end@3:
   LDY   #$19
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
   ADC   #$1C
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$1B
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

division:
   SUBROUTINE

   ; prologue: 0 arg bytes, 26 local bytes
   SEC
   LDA   SSP
   SBC   #$1C
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$1B
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   LDA   i
   LDY   #$01
   STA   (FP),Y
   LDA   i+1
   INY
   STA   (FP),Y
   LDA   i+1
   BMI   .sx_neg@5
   LDA   #$00
   JMP   .sx_done@6
.sx_neg@5:
   LDA   #$FF
.sx_done@6:
   LDY   #$03
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
   LDA   l
   STA   HARGS
   LDA   l+1
   STA   HARGS+1
   LDA   l+2
   STA   HARGS+2
   LDA   l+3
   STA   HARGS+3
   LDA   l+4
   STA   HARGS+4
   LDA   l+5
   STA   HARGS+5
   LDA   l+6
   STA   HARGS+6
   LDA   l+7
   STA   HARGS+7
   LDY   #$01
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
   LDA   HARGS+16
   LDY   #$09
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
   LDY   #$09
   LDA   (FP),Y
   LDY   #$11
   STA   (FP),Y
   LDY   #$0A
   LDA   (FP),Y
   LDY   #$12
   STA   (FP),Y
   LDY   #$0B
   LDA   (FP),Y
   LDY   #$13
   STA   (FP),Y
   LDY   #$0C
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
   CMP   #$00
   BNE   .thread@1
   DEY
   LDA   (FP),Y
   BNE   .thread@1
   DEY
   LDA   (FP),Y
   CMP   #$27
   BNE   .thread@1
   DEY
   LDA   (FP),Y
   CMP   #$10
   BEQ   .cmp_true@7
.thread@1:
   LDA   #$00
   JMP   .cmp_end@8
.cmp_true@7:
   LDA   #$01
.cmp_end@8:
   LDY   #$19
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
   ADC   #$1C
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$1B
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

comparison:
   SUBROUTINE

   ; prologue: 0 arg bytes, 10 local bytes
   SEC
   LDA   SSP
   SBC   #$0C
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$0B
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   LDA   i
   LDY   #$01
   STA   (FP),Y
   LDA   i+1
   INY
   STA   (FP),Y
   LDA   i+1
   BMI   .sx_neg@10
   LDA   #$00
   JMP   .sx_done@11
.sx_neg@10:
   LDA   #$FF
.sx_done@11:
   LDY   #$03
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
   LDA   l
   SEC
   LDY   #$01
   SBC   (FP),Y
   LDA   l+1
   INY
   SBC   (FP),Y
   LDA   l+2
   INY
   SBC   (FP),Y
   LDA   l+3
   INY
   SBC   (FP),Y
   LDA   l+4
   INY
   SBC   (FP),Y
   LDA   l+5
   INY
   SBC   (FP),Y
   LDA   l+6
   INY
   SBC   (FP),Y
   LDA   l+7
   INY
   SBC   (FP),Y
   BVC   .cmp_novf@12
   EOR   #$80
.cmp_novf@12:
   ASL
   LDA   #$00
   ROL
   EOR   #$01
   LDY   #$09
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
   ADC   #$0C
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$0B
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

conditional:
   SUBROUTINE

   ; prologue: 0 arg bytes, 26 local bytes
   SEC
   LDA   SSP
   SBC   #$1C
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$1B
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   LDA   #$01
   BEQ   .cond_else@0
   LDA   l
   LDY   #$01
   STA   (FP),Y
   LDA   l+1
   INY
   STA   (FP),Y
   LDA   l+2
   INY
   STA   (FP),Y
   LDA   l+3
   INY
   STA   (FP),Y
   LDA   l+4
   INY
   STA   (FP),Y
   LDA   l+5
   INY
   STA   (FP),Y
   LDA   l+6
   INY
   STA   (FP),Y
   LDA   l+7
   INY
   STA   (FP),Y
   JMP   .cond_end@1
.cond_else@0:
   LDA   i
   LDY   #$09
   STA   (FP),Y
   LDA   i+1
   INY
   STA   (FP),Y
   LDA   i+1
   BMI   .sx_neg@13
   LDA   #$00
   JMP   .sx_done@14
.sx_neg@13:
   LDA   #$FF
.sx_done@14:
   LDY   #$0B
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
   LDY   #$09
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   LDY   #$0A
   LDA   (FP),Y
   LDY   #$02
   STA   (FP),Y
   LDY   #$0B
   LDA   (FP),Y
   LDY   #$03
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
.cond_end@1:
   LDY   #$01
   LDA   (FP),Y
   LDY   #$11
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   LDY   #$12
   STA   (FP),Y
   LDY   #$03
   LDA   (FP),Y
   LDY   #$13
   STA   (FP),Y
   LDY   #$04
   LDA   (FP),Y
   LDY   #$14
   STA   (FP),Y
   LDY   #$05
   LDA   (FP),Y
   LDY   #$15
   STA   (FP),Y
   LDY   #$06
   LDA   (FP),Y
   LDY   #$16
   STA   (FP),Y
   LDY   #$07
   LDA   (FP),Y
   LDY   #$17
   STA   (FP),Y
   LDY   #$08
   LDA   (FP),Y
   LDY   #$18
   STA   (FP),Y
   CMP   #$00
   BNE   .thread@2
   DEY
   LDA   (FP),Y
   BNE   .thread@2
   DEY
   LDA   (FP),Y
   BNE   .thread@2
   DEY
   LDA   (FP),Y
   BNE   .thread@2
   DEY
   LDA   (FP),Y
   CMP   #$40
   BNE   .thread@2
   DEY
   LDA   (FP),Y
   BNE   .thread@2
   DEY
   LDA   (FP),Y
   BNE   .thread@2
   DEY
   LDA   (FP),Y
   BEQ   .cmp_true@15
.thread@2:
   LDA   #$00
   JMP   .cmp_end@16
.cmp_true@15:
   LDA   #$01
.cmp_end@16:
   LDY   #$19
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
   ADC   #$1C
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$1B
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

   ; prologue: 0 arg bytes, 34 local bytes
   SEC
   LDA   SSP
   SBC   #$24
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$23
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   LDA   #$82
   LDY   #$01
   STA   (FP),Y
   LDA   #$80
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   ORA   #$00
   BPL   .sx_done@19
   LDA   #$FF
.sx_done@19:
   LDY   #$05
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$01
   LDA   (FP),Y
   STA   l
   INY
   LDA   (FP),Y
   STA   l+1
   INY
   LDA   (FP),Y
   STA   l+2
   INY
   LDA   (FP),Y
   STA   l+3
   INY
   LDA   (FP),Y
   STA   l+4
   INY
   LDA   (FP),Y
   STA   l+5
   INY
   LDA   (FP),Y
   STA   l+6
   INY
   LDA   (FP),Y
   STA   l+7
   LDA   #$0A
   STA   i
   LDA   #$00
   STA   i+1
   JSR   addition
   LDY   #$09
   STA   (FP),Y
   TXA
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lnot_true@20
   LDA   #$00
   JMP   .lnot_end@21
.lnot_true@20:
   LDA   #$01
.lnot_end@21:
   LDY   #$0B
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@2
   LDX   #$00
   LDA   #$01

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$24
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$23
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
.if_end@2:
   LDA   #$A0
   STA   l
   LDA   #$86
   STA   l+1
   LDA   #$01
   STA   l+2
   LDA   #$00
   STA   l+3
   STA   l+4
   STA   l+5
   STA   l+6
   STA   l+7
   JSR   division
   LDY   #$0D
   STA   (FP),Y
   TXA
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lnot_true@22
   LDA   #$00
   JMP   .lnot_end@23
.lnot_true@22:
   LDA   #$01
.lnot_end@23:
   LDY   #$0F
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@3
   LDX   #$00
   LDA   #$02

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$24
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$23
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
.if_end@3:
   SEC
   LDA   #$00
   SBC   #$64
   LDY   #$11
   STA   (FP),Y
   LDA   #$00
   SBC   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   i
   INY
   LDA   (FP),Y
   STA   i+1
   LDA   #$82
   INY
   STA   (FP),Y
   LDA   #$80
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   ORA   #$00
   BPL   .sx_done@25
   LDA   #$FF
.sx_done@25:
   LDY   #$17
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$13
   LDA   (FP),Y
   STA   l
   INY
   LDA   (FP),Y
   STA   l+1
   INY
   LDA   (FP),Y
   STA   l+2
   INY
   LDA   (FP),Y
   STA   l+3
   INY
   LDA   (FP),Y
   STA   l+4
   INY
   LDA   (FP),Y
   STA   l+5
   INY
   LDA   (FP),Y
   STA   l+6
   INY
   LDA   (FP),Y
   STA   l+7
   JSR   comparison
   LDY   #$1B
   STA   (FP),Y
   TXA
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lnot_true@26
   LDA   #$00
   JMP   .lnot_end@27
.lnot_true@26:
   LDA   #$01
.lnot_end@27:
   LDY   #$1D
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@4
   LDX   #$00
   LDA   #$03

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$24
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$23
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
.if_end@4:
   LDA   #$40
   STA   l+3
   LDA   #$0A
   STA   i
   LDA   #$00
   STA   l
   STA   l+1
   STA   l+2
   STA   l+4
   STA   l+5
   STA   l+6
   STA   l+7
   STA   i+1
   JSR   conditional
   LDY   #$1F
   STA   (FP),Y
   TXA
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lnot_true@28
   LDA   #$00
   JMP   .lnot_end@29
.lnot_true@28:
   LDA   #$01
.lnot_end@29:
   LDY   #$21
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@5
   LDX   #$00
   LDA   #$04

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$24
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
.if_end@5:
   LDX   #$00
   TXA

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$24
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$23
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

l:
   DS.B  8

i:
   DS.B  2
