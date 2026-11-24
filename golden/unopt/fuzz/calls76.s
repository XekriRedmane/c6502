SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24

f4:
   SUBROUTINE

   ; prologue: 0 arg bytes, 16 local bytes
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

   LDA   #$E8
   LDY   #$01
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
   LDA   g1+3
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
   INY
   STA   (FP),Y
   CMP   g0+6
   BNE   .cmp_true@2
   CMP   g0+5
   BNE   .cmp_true@2
   CMP   g0+4
   BNE   .cmp_true@2
   LDY   #$06
   LDA   (FP),Y
   CMP   g0+3
   BNE   .cmp_true@2
   DEY
   LDA   (FP),Y
   CMP   g0+2
   BNE   .cmp_true@2
   DEY
   LDA   (FP),Y
   CMP   g0+1
   BNE   .cmp_true@2
   DEY
   LDA   (FP),Y
   CMP   g0
   BNE   .cmp_true@2
   LDA   #$00
   JMP   .cmp_end@3
.cmp_true@2:
   LDA   #$01
.cmp_end@3:
   LDY   #$0A
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   BNE   .or_true@0
   LDA   g0
   ORA   g0+1
   ORA   g0+2
   ORA   g0+3
   ORA   g0+4
   ORA   g0+5
   ORA   g0+6
   BNE   .or_true@0
   LDA   #$00
   LDY   #$0C
   STA   (FP),Y
   INY
   STA   (FP),Y
   JMP   .or_end@1
.or_true@0:
   LDA   #$01
   LDY   #$0C
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@1:
   LDY   #$0A
   LDA   (FP),Y
   LDY   #$0C
   AND   (FP),Y
   LDY   #$0E
   STA   (FP),Y
   LDY   #$0B
   LDA   (FP),Y
   LDY   #$0D
   AND   (FP),Y
   LDY   #$0F
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$10
   STA   (FP),Y

   ; epilogue
   PHA
   CLC
   LDA   FP
   ADC   #$12
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

   ; prologue: 4 arg bytes, 44 local bytes
   SEC
   LDA   SSP
   SBC   #$2E
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$2D
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   LDA   g1
   SEC
   SBC   g1
   LDY   #$01
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
   LDA   g1+3
   CMP   (FP),Y
   BNE   .cmp_true@5
   LDA   g1+2
   DEY
   CMP   (FP),Y
   BNE   .cmp_true@5
   LDA   g1+1
   DEY
   CMP   (FP),Y
   BNE   .cmp_true@5
   LDA   g1
   DEY
   CMP   (FP),Y
   BNE   .cmp_true@5
   LDA   #$00
   JMP   .cmp_end@6
.cmp_true@5:
   LDA   #$01
.cmp_end@6:
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
   LDA   #<f4
   INY
   STA   (FP),Y
   LDA   #>f4
   INY
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
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   JSR   icall
   LDY   #$0D
   STA   (FP),Y
   INY
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@8
   LDA   #$00
   JMP   .sx_done@9
.sx_neg@8:
   LDA   #$FF
.sx_done@9:
   LDY   #$0F
   STA   (FP),Y
   LDY   #$07
   LDA   (FP),Y
   LDY   #$0E
   EOR   (FP),Y
   LDY   #$10
   STA   (FP),Y
   LDY   #$08
   LDA   (FP),Y
   LDY   #$0F
   EOR   (FP),Y
   LDY   #$11
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$07
   STA   (FP),Y
   LDY   #$11
   LDA   (FP),Y
   LDY   #$08
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$12
   STA   (FP),Y
   LDY   #$08
   LDA   (FP),Y
   LDY   #$13
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@10
   LDA   #$00
   JMP   .sx_done@11
.sx_neg@10:
   LDA   #$FF
.sx_done@11:
   LDY   #$14
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   g1
   CLC
   LDY   #$12
   ADC   (FP),Y
   LDY   #$16
   STA   (FP),Y
   LDA   g1+1
   LDY   #$13
   ADC   (FP),Y
   LDY   #$17
   STA   (FP),Y
   LDA   g1+2
   LDY   #$14
   ADC   (FP),Y
   LDY   #$18
   STA   (FP),Y
   LDA   g1+3
   LDY   #$15
   ADC   (FP),Y
   LDY   #$19
   STA   (FP),Y
   LDY   #$16
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
   LDA   #$65
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   STA   HARGS+1
   LDA   #$64
   STA   HARGS
   DEY
   LDA   (FP),Y
   STA   HARGS+2
   INY
   LDA   (FP),Y
   STA   HARGS+3
   JSR   sdivmod16
   LDA   HARGS+4
   LDY   #$1C
   STA   (FP),Y
   LDA   HARGS+5
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$1E
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$1F
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@12
   LDA   #$00
   JMP   .sx_done@13
.sx_neg@12:
   LDA   #$FF
.sx_done@13:
   LDY   #$20
   STA   (FP),Y
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
   LDY   #$2F
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
   LDY   #$22
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
   LDY   #$22
   LDA   (FP),Y
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
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$26
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
   INY
   LDA   (FP),Y
   STA   HARGS+22

   ; epilogue
   CLC
   LDA   FP
   ADC   #$32
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

f2:
   SUBROUTINE

   ; prologue: 0 arg bytes, 113 local bytes
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

   LDA   g0
   EOR   g0
   LDY   #$01
   STA   (FP),Y
   LDA   g0+1
   EOR   g0+1
   INY
   STA   (FP),Y
   LDA   g0+2
   EOR   g0+2
   INY
   STA   (FP),Y
   LDA   g0+3
   EOR   g0+3
   INY
   STA   (FP),Y
   LDA   g0+4
   EOR   g0+4
   INY
   STA   (FP),Y
   LDA   g0+5
   EOR   g0+5
   INY
   STA   (FP),Y
   LDA   g0+6
   EOR   g0+6
   INY
   STA   (FP),Y
   LDY   #$01
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
   BEQ   .and_false@2
   LDA   g1
   AND   g1
   INY
   STA   (FP),Y
   LDA   g1+1
   AND   g1+1
   INY
   STA   (FP),Y
   LDA   g1+2
   AND   g1+2
   INY
   STA   (FP),Y
   LDA   g1+3
   AND   g1+3
   INY
   STA   (FP),Y
   LDY   #$08
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   INY
   ORA   (FP),Y
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
   LDY   #$0C
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@3:
   LDY   #$0C
   LDA   (FP),Y
   LDY   #$0E
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$0F
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@14
   LDA   #$00
   JMP   .sx_done@15
.sx_neg@14:
   LDA   #$FF
.sx_done@15:
   LDY   #$10
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$0E
   LDA   (FP),Y
   LDY   #$15
   STA   (FP),Y
   LDY   #$0F
   LDA   (FP),Y
   LDY   #$16
   STA   (FP),Y
   LDY   #$10
   LDA   (FP),Y
   LDY   #$17
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$15
   LDA   (FP),Y
   LDY   #$1C
   STA   (FP),Y
   LDY   #$16
   LDA   (FP),Y
   LDY   #$1D
   STA   (FP),Y
   LDY   #$17
   LDA   (FP),Y
   LDY   #$1E
   STA   (FP),Y
   LDY   #$1C
   LDA   (FP),Y
   LDY   #$1F
   STA   (FP),Y
   LDY   #$1D
   LDA   (FP),Y
   LDY   #$20
   STA   (FP),Y
   LDY   #$1E
   LDA   (FP),Y
   AND   #$03
   LDY   #$21
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
   LDA   g1+3
   BMI   .sx_neg@16
   LDA   #$00
   JMP   .sx_done@17
.sx_neg@16:
   LDA   #$FF
.sx_done@17:
   LDY   #$26
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   g0+6
   CMP   (FP),Y
   BNE   .thread@0
   LDA   g0+5
   DEY
   CMP   (FP),Y
   BNE   .thread@0
   LDA   g0+4
   DEY
   CMP   (FP),Y
   BNE   .thread@0
   LDA   g0+3
   DEY
   CMP   (FP),Y
   BNE   .thread@0
   LDA   g0+2
   DEY
   CMP   (FP),Y
   BNE   .thread@0
   LDA   g0+1
   DEY
   CMP   (FP),Y
   BNE   .thread@0
   LDA   g0
   DEY
   CMP   (FP),Y
   BEQ   .cmp_true@18
.thread@0:
   LDA   #$00
   JMP   .cmp_end@19
.cmp_true@18:
   LDA   #$01
.cmp_end@19:
   LDY   #$29
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
   BMI   .sx_neg@21
   LDA   #$00
   JMP   .sx_done@22
.sx_neg@21:
   LDA   #$FF
.sx_done@22:
   LDY   #$2D
   STA   (FP),Y
   LDY   #$2B
   LDA   (FP),Y
   LDY   #$2E
   STA   (FP),Y
   LDY   #$2C
   LDA   (FP),Y
   LDY   #$2F
   STA   (FP),Y
   LDY   #$2D
   LDA   (FP),Y
   AND   #$03
   LDY   #$30
   STA   (FP),Y
   LDY   #$21
   LDA   (FP),Y
   LDY   #$30
   CMP   (FP),Y
   BNE   .cmp_true@23
   LDY   #$20
   LDA   (FP),Y
   LDY   #$2F
   CMP   (FP),Y
   BNE   .cmp_true@23
   LDY   #$1F
   LDA   (FP),Y
   LDY   #$2E
   CMP   (FP),Y
   BNE   .cmp_true@23
   LDA   #$00
   JMP   .cmp_end@24
.cmp_true@23:
   LDA   #$01
.cmp_end@24:
   LDY   #$31
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$33
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$34
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@26
   LDA   #$00
   JMP   .sx_done@27
.sx_neg@26:
   LDA   #$FF
.sx_done@27:
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
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$FF
   INY
   STA   (FP),Y
   LDA   #$7F
   INY
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@28
   LDA   #$00
   JMP   .sx_done@29
.sx_neg@28:
   LDA   #$FF
.sx_done@29:
   LDY   #$3F
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
   LDY   #$3D
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$3E
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$3F
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$40
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   JSR   f3
   LDA   HARGS+16
   LDY   #$41
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
   LDY   #$3C
   LDA   (FP),Y
   LDY   #$48
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
   LDY   #$48
   LDA   (FP),Y
   LDY   #$41
   EOR   (FP),Y
   LDY   #$4F
   STA   (FP),Y
   LDY   #$49
   LDA   (FP),Y
   LDY   #$42
   EOR   (FP),Y
   LDY   #$50
   STA   (FP),Y
   LDY   #$4A
   LDA   (FP),Y
   LDY   #$43
   EOR   (FP),Y
   LDY   #$51
   STA   (FP),Y
   LDY   #$4B
   LDA   (FP),Y
   LDY   #$44
   EOR   (FP),Y
   LDY   #$52
   STA   (FP),Y
   LDY   #$4C
   LDA   (FP),Y
   LDY   #$45
   EOR   (FP),Y
   LDY   #$53
   STA   (FP),Y
   LDY   #$4D
   LDA   (FP),Y
   LDY   #$46
   EOR   (FP),Y
   LDY   #$54
   STA   (FP),Y
   LDY   #$4E
   LDA   (FP),Y
   LDY   #$47
   EOR   (FP),Y
   LDY   #$55
   STA   (FP),Y
   LDY   #$4F
   LDA   (FP),Y
   LDY   #$56
   STA   (FP),Y
   LDY   #$3C
   STA   (FP),Y
   LDY   #$57
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   g1
   CLC
   LDY   #$57
   ADC   (FP),Y
   LDY   #$5B
   STA   (FP),Y
   LDA   g1+1
   LDY   #$58
   ADC   (FP),Y
   LDY   #$5C
   STA   (FP),Y
   LDA   g1+2
   LDY   #$59
   ADC   (FP),Y
   LDY   #$5D
   STA   (FP),Y
   LDA   g1+3
   LDY   #$5A
   ADC   (FP),Y
   LDY   #$5E
   STA   (FP),Y
   LDY   #$5B
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
   LDA   #<f4
   INY
   STA   (FP),Y
   LDA   #>f4
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$61
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$62
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   JSR   icall
   LDY   #$63
   STA   (FP),Y
   INY
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@30
   LDA   #$00
   JMP   .sx_done@31
.sx_neg@30:
   LDA   #$FF
.sx_done@31:
   LDY   #$65
   STA   (FP),Y
   LDY   #$3C
   LDA   (FP),Y
   LDY   #$66
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$64
   EOR   (FP),Y
   LDY   #$68
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$65
   EOR   (FP),Y
   LDY   #$69
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$6A
   STA   (FP),Y
   LDY   #$3C
   STA   (FP),Y
   LDA   #$64
   BEQ   .lnot_true@32
   LDA   #$00
   JMP   .lnot_end@33
.lnot_true@32:
   LDA   #$01
.lnot_end@33:
   LDY   #$6B
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   ORA   #$01
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$6B
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
   LDY   #$6F
   STA   (FP),Y
   LDA   HARGS+5
   INY
   STA   (FP),Y
   DEY
   ORA   (FP),Y
   BEQ   .cmp_end@34
   LDA   #$01
.cmp_end@34:
   LDY   #$71
   STA   (FP),Y

   ; epilogue
   PHA
   CLC
   LDA   FP
   ADC   #$73
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

   ; prologue: 4 arg bytes, 233 local bytes
   SEC
   LDA   SSP
   SBC   #$EB
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$EA
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
   LDY   #$EC
   LDA   (FP),Y
   CLC
   SBC   (FP),Y
   INY
   LDA   (FP),Y
   SBC   (FP),Y
   INY
   LDA   (FP),Y
   SBC   (FP),Y
   INY
   LDA   (FP),Y
   SBC   (FP),Y
   LDA   #$00
   ROL
   LDY   #$03
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ORA   #$01
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
   LDY   #$01
   LDA   (FP),Y
   STA   HARGS
   INY
   LDA   (FP),Y
   STA   HARGS+1
   LDY   #$07
   LDA   (FP),Y
   STA   HARGS+2
   INY
   LDA   (FP),Y
   STA   HARGS+3
   JSR   udivmod16
   LDA   HARGS+6
   LDY   #$09
   STA   (FP),Y
   LDA   HARGS+7
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$0B
   STA   (FP),Y
   INY
   STA   (FP),Y
   ORA   #$00
   BNE   .or_true@6
   LDA   g0
   ORA   g0+1
   ORA   g0+2
   ORA   g0+3
   ORA   g0+4
   ORA   g0+5
   ORA   g0+6
   BNE   .or_true@6
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   JMP   .or_end@7
.or_true@6:
   LDA   #$01
   LDY   #$0D
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@7:
   LDY   #$0D
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@4
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
   LDY   #$EF
   CMP   (FP),Y
   BNE   .thread@1
   LDY   #$11
   LDA   (FP),Y
   LDY   #$EE
   CMP   (FP),Y
   BNE   .thread@1
   LDY   #$10
   LDA   (FP),Y
   LDY   #$ED
   CMP   (FP),Y
   BNE   .thread@1
   LDY   #$0F
   LDA   (FP),Y
   LDY   #$EC
   CMP   (FP),Y
   BEQ   .cmp_true@35
.thread@1:
   LDA   #$00
   JMP   .cmp_end@36
.cmp_true@35:
   LDA   #$01
.cmp_end@36:
   LDY   #$13
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@4
   LDA   #$01
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   JMP   .and_end@5
.and_false@4:
   LDA   #$00
   LDY   #$15
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@5:
   LDY   #$15
   LDA   (FP),Y
   LDY   #$17
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$18
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@38
   LDA   #$00
   JMP   .sx_done@39
.sx_neg@38:
   LDA   #$FF
.sx_done@39:
   LDY   #$19
   STA   (FP),Y
   LDY   #$17
   LDA   (FP),Y
   LDY   #$1A
   STA   (FP),Y
   LDY   #$18
   LDA   (FP),Y
   LDY   #$1B
   STA   (FP),Y
   LDY   #$19
   LDA   (FP),Y
   AND   #$03
   LDY   #$1C
   STA   (FP),Y
   LDY   #$1A
   LDA   (FP),Y
   LDY   #$1D
   STA   (FP),Y
   LDY   #$1B
   LDA   (FP),Y
   LDY   #$1E
   STA   (FP),Y
   LDY   #$1C
   LDA   (FP),Y
   LDY   #$1F
   STA   (FP),Y
   LDA   g0
   INY
   STA   (FP),Y
   LDA   g0+1
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$22
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$23
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@40
   LDA   #$00
   JMP   .sx_done@41
.sx_neg@40:
   LDA   #$FF
.sx_done@41:
   LDY   #$24
   STA   (FP),Y
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
   LDA   #$00
   INY
   STA   (FP),Y
   ORA   #$00
   BPL   .sx_done@43
   LDA   #$FF
.sx_done@43:
   LDY   #$2B
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   g1
   LDY   #$29
   AND   (FP),Y
   LDY   #$2D
   STA   (FP),Y
   LDA   g1+1
   LDY   #$2A
   AND   (FP),Y
   LDY   #$2E
   STA   (FP),Y
   LDA   g1+2
   LDY   #$2B
   AND   (FP),Y
   LDY   #$2F
   STA   (FP),Y
   LDA   g1+3
   LDY   #$2C
   AND   (FP),Y
   LDY   #$30
   STA   (FP),Y
   LDA   g0
   STA   HARGS
   LDA   g0+1
   STA   HARGS+1
   LDA   g0+2
   STA   HARGS+2
   LDA   g0+3
   STA   HARGS+3
   LDA   g0+4
   STA   HARGS+4
   LDA   g0+5
   STA   HARGS+5
   LDA   g0+6
   STA   HARGS+6
   ASL
   LDA   #$00
   ADC   #$FF
   EOR   #$FF
   STA   HARGS+7
   LDY   #$2D
   LDA   (FP),Y
   STA   HARGS+8
   JSR   asr64
   LDA   HARGS+9
   LDY   #$31
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
   LDA   HARGS+15
   INY
   STA   (FP),Y
   LDY   #$28
   LDA   (FP),Y
   LDY   #$37
   CMP   (FP),Y
   BNE   .thread@2
   LDY   #$27
   LDA   (FP),Y
   LDY   #$36
   CMP   (FP),Y
   BNE   .thread@2
   LDY   #$26
   LDA   (FP),Y
   LDY   #$35
   CMP   (FP),Y
   BNE   .thread@2
   LDY   #$25
   LDA   (FP),Y
   LDY   #$34
   CMP   (FP),Y
   BNE   .thread@2
   LDY   #$24
   LDA   (FP),Y
   LDY   #$33
   CMP   (FP),Y
   BNE   .thread@2
   LDY   #$23
   LDA   (FP),Y
   LDY   #$32
   CMP   (FP),Y
   BNE   .thread@2
   LDY   #$22
   LDA   (FP),Y
   LDY   #$31
   CMP   (FP),Y
   BEQ   .cmp_true@44
.thread@2:
   LDA   #$00
   JMP   .cmp_end@45
.cmp_true@44:
   LDA   #$01
.cmp_end@45:
   LDY   #$38
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$3A
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$3B
   STA   (FP),Y
   LDA   #$07
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
   LDY   #$3C
   STA   (FP),Y
   LDA   HARGS+5
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$3E
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$3F
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@47
   LDA   #$00
   JMP   .sx_done@48
.sx_neg@47:
   LDA   #$FF
.sx_done@48:
   LDY   #$40
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
   LDY   #$42
   LDA   (FP),Y
   SEC
   LDY   #$EC
   SBC   (FP),Y
   LDY   #$46
   STA   (FP),Y
   LDY   #$43
   LDA   (FP),Y
   LDY   #$ED
   SBC   (FP),Y
   LDY   #$47
   STA   (FP),Y
   LDY   #$44
   LDA   (FP),Y
   LDY   #$EE
   SBC   (FP),Y
   LDY   #$48
   STA   (FP),Y
   LDY   #$45
   LDA   (FP),Y
   LDY   #$EF
   SBC   (FP),Y
   LDY   #$49
   STA   (FP),Y
   LDY   #$3E
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
   JSR   mul32
   LDA   HARGS+8
   LDY   #$4A
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
   LDY   #$4A
   LDA   (FP),Y
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
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$4E
   LDA   (FP),Y
   LDY   #$55
   STA   (FP),Y
   LDY   #$4F
   LDA   (FP),Y
   LDY   #$56
   STA   (FP),Y
   LDY   #$50
   LDA   (FP),Y
   LDY   #$57
   STA   (FP),Y
   LDY   #$51
   LDA   (FP),Y
   LDY   #$58
   STA   (FP),Y
   LDY   #$52
   LDA   (FP),Y
   LDY   #$59
   STA   (FP),Y
   INY
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
   BPL   .sx_done@50
   LDA   #$FF
.sx_done@50:
   LDY   #$5E
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
   LDY   #$5C
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$5D
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$5E
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$5F
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   JSR   f3
   LDA   HARGS+16
   LDY   #$60
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
   LDY   #$0C
   LDA   (FP),Y
   LDY   #$67
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
   LDY   #$67
   LDA   (FP),Y
   LDY   #$60
   EOR   (FP),Y
   LDY   #$6E
   STA   (FP),Y
   LDY   #$68
   LDA   (FP),Y
   LDY   #$61
   EOR   (FP),Y
   LDY   #$6F
   STA   (FP),Y
   LDY   #$69
   LDA   (FP),Y
   LDY   #$62
   EOR   (FP),Y
   LDY   #$70
   STA   (FP),Y
   LDY   #$6A
   LDA   (FP),Y
   LDY   #$63
   EOR   (FP),Y
   LDY   #$71
   STA   (FP),Y
   LDY   #$6B
   LDA   (FP),Y
   LDY   #$64
   EOR   (FP),Y
   LDY   #$72
   STA   (FP),Y
   LDY   #$6C
   LDA   (FP),Y
   LDY   #$65
   EOR   (FP),Y
   LDY   #$73
   STA   (FP),Y
   LDY   #$6D
   LDA   (FP),Y
   LDY   #$66
   EOR   (FP),Y
   LDY   #$74
   STA   (FP),Y
   LDY   #$6E
   LDA   (FP),Y
   LDY   #$75
   STA   (FP),Y
   LDY   #$0C
   STA   (FP),Y
   LDY   #$76
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
   LDA   g0
   CLC
   LDY   #$76
   ADC   (FP),Y
   LDY   #$7D
   STA   (FP),Y
   LDA   g0+1
   LDY   #$77
   ADC   (FP),Y
   LDY   #$7E
   STA   (FP),Y
   LDA   g0+2
   LDY   #$78
   ADC   (FP),Y
   LDY   #$7F
   STA   (FP),Y
   LDA   g0+3
   LDY   #$79
   ADC   (FP),Y
   LDY   #$80
   STA   (FP),Y
   LDA   g0+4
   LDY   #$7A
   ADC   (FP),Y
   LDY   #$81
   STA   (FP),Y
   LDA   g0+5
   LDY   #$7B
   ADC   (FP),Y
   LDY   #$82
   STA   (FP),Y
   LDA   g0+6
   LDY   #$7C
   ADC   (FP),Y
   LDY   #$83
   STA   (FP),Y
   LDY   #$7D
   LDA   (FP),Y
   LDY   #$84
   STA   (FP),Y
   LDY   #$7E
   LDA   (FP),Y
   LDY   #$85
   STA   (FP),Y
   LDY   #$7F
   LDA   (FP),Y
   LDY   #$86
   STA   (FP),Y
   LDY   #$80
   LDA   (FP),Y
   LDY   #$87
   STA   (FP),Y
   LDY   #$81
   LDA   (FP),Y
   LDY   #$88
   STA   (FP),Y
   LDY   #$82
   LDA   (FP),Y
   LDY   #$89
   STA   (FP),Y
   LDY   #$83
   LDA   (FP),Y
   AND   #$01
   LDY   #$8A
   STA   (FP),Y
   LDY   #$84
   LDA   (FP),Y
   LDY   #$8B
   STA   (FP),Y
   LDY   #$85
   LDA   (FP),Y
   LDY   #$8C
   STA   (FP),Y
   LDY   #$86
   LDA   (FP),Y
   LDY   #$8D
   STA   (FP),Y
   LDY   #$87
   LDA   (FP),Y
   LDY   #$8E
   STA   (FP),Y
   LDY   #$88
   LDA   (FP),Y
   LDY   #$8F
   STA   (FP),Y
   LDY   #$89
   LDA   (FP),Y
   LDY   #$90
   STA   (FP),Y
   LDY   #$8A
   LDA   (FP),Y
   EOR   #$01
   LDY   #$91
   STA   (FP),Y
   LDY   #$8B
   LDA   (FP),Y
   LDY   #$92
   STA   (FP),Y
   LDY   #$8C
   LDA   (FP),Y
   LDY   #$93
   STA   (FP),Y
   LDY   #$8D
   LDA   (FP),Y
   LDY   #$94
   STA   (FP),Y
   LDY   #$8E
   LDA   (FP),Y
   LDY   #$95
   STA   (FP),Y
   LDY   #$8F
   LDA   (FP),Y
   LDY   #$96
   STA   (FP),Y
   LDY   #$90
   LDA   (FP),Y
   LDY   #$97
   STA   (FP),Y
   LDY   #$91
   LDA   (FP),Y
   SEC
   SBC   #$01
   LDY   #$98
   STA   (FP),Y
   LDY   #$92
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
   INY
   LDA   (FP),Y
   STA   g0+4
   INY
   LDA   (FP),Y
   STA   g0+5
   INY
   LDA   (FP),Y
   STA   g0+6
   JSR   f2
   LDY   #$99
   STA   (FP),Y
   ORA   #$00
   BNE   .lb_skip@1
   JMP   .if_else@9
.lb_skip@1:
   LDY   #$55
   LDA   (FP),Y
   CLC
   ADC   #$03
   LDY   #$9A
   STA   (FP),Y
   LDY   #$56
   LDA   (FP),Y
   ADC   #$00
   LDY   #$9B
   STA   (FP),Y
   LDY   #$57
   LDA   (FP),Y
   ADC   #$00
   LDY   #$9C
   STA   (FP),Y
   LDY   #$58
   LDA   (FP),Y
   ADC   #$00
   LDY   #$9D
   STA   (FP),Y
   LDY   #$59
   LDA   (FP),Y
   ADC   #$00
   LDY   #$9E
   STA   (FP),Y
   LDY   #$5A
   LDA   (FP),Y
   ADC   #$00
   LDY   #$9F
   STA   (FP),Y
   LDY   #$5B
   LDA   (FP),Y
   ADC   #$00
   LDY   #$A0
   STA   (FP),Y
   LDY   #$9A
   LDA   (FP),Y
   LDY   #$A1
   STA   (FP),Y
   LDY   #$9B
   LDA   (FP),Y
   LDY   #$A2
   STA   (FP),Y
   LDY   #$9C
   LDA   (FP),Y
   LDY   #$A3
   STA   (FP),Y
   LDY   #$9D
   LDA   (FP),Y
   LDY   #$A4
   STA   (FP),Y
   LDY   #$9E
   LDA   (FP),Y
   LDY   #$A5
   STA   (FP),Y
   LDY   #$9F
   LDA   (FP),Y
   LDY   #$A6
   STA   (FP),Y
   LDY   #$A0
   LDA   (FP),Y
   AND   #$01
   LDY   #$A7
   STA   (FP),Y
   LDY   #$A1
   LDA   (FP),Y
   LDY   #$A8
   STA   (FP),Y
   LDY   #$A2
   LDA   (FP),Y
   LDY   #$A9
   STA   (FP),Y
   LDY   #$A3
   LDA   (FP),Y
   LDY   #$AA
   STA   (FP),Y
   LDY   #$A4
   LDA   (FP),Y
   LDY   #$AB
   STA   (FP),Y
   LDY   #$A5
   LDA   (FP),Y
   LDY   #$AC
   STA   (FP),Y
   LDY   #$A6
   LDA   (FP),Y
   LDY   #$AD
   STA   (FP),Y
   LDY   #$A7
   LDA   (FP),Y
   EOR   #$01
   LDY   #$AE
   STA   (FP),Y
   LDY   #$A8
   LDA   (FP),Y
   LDY   #$AF
   STA   (FP),Y
   LDY   #$A9
   LDA   (FP),Y
   LDY   #$B0
   STA   (FP),Y
   LDY   #$AA
   LDA   (FP),Y
   LDY   #$B1
   STA   (FP),Y
   LDY   #$AB
   LDA   (FP),Y
   LDY   #$B2
   STA   (FP),Y
   LDY   #$AC
   LDA   (FP),Y
   LDY   #$B3
   STA   (FP),Y
   LDY   #$AD
   LDA   (FP),Y
   LDY   #$B4
   STA   (FP),Y
   LDY   #$AE
   LDA   (FP),Y
   SEC
   SBC   #$01
   LDY   #$B5
   STA   (FP),Y
   LDY   #$AF
   LDA   (FP),Y
   LDY   #$55
   STA   (FP),Y
   LDY   #$B0
   LDA   (FP),Y
   LDY   #$56
   STA   (FP),Y
   LDY   #$B1
   LDA   (FP),Y
   LDY   #$57
   STA   (FP),Y
   LDY   #$B2
   LDA   (FP),Y
   LDY   #$58
   STA   (FP),Y
   LDY   #$B3
   LDA   (FP),Y
   LDY   #$59
   STA   (FP),Y
   LDY   #$B4
   LDA   (FP),Y
   LDY   #$5A
   STA   (FP),Y
   LDY   #$B5
   LDA   (FP),Y
   LDY   #$5B
   STA   (FP),Y
   JMP   .if_end@8
.if_else@9:
   LDY   #$55
   LDA   (FP),Y
   EOR   #$05
   LDY   #$B6
   STA   (FP),Y
   LDY   #$56
   LDA   (FP),Y
   LDY   #$B7
   STA   (FP),Y
   LDY   #$57
   LDA   (FP),Y
   LDY   #$B8
   STA   (FP),Y
   LDY   #$58
   LDA   (FP),Y
   LDY   #$B9
   STA   (FP),Y
   LDY   #$59
   LDA   (FP),Y
   LDY   #$BA
   STA   (FP),Y
   LDY   #$5A
   LDA   (FP),Y
   LDY   #$BB
   STA   (FP),Y
   LDY   #$5B
   LDA   (FP),Y
   LDY   #$BC
   STA   (FP),Y
   LDY   #$B6
   LDA   (FP),Y
   LDY   #$55
   STA   (FP),Y
   LDY   #$B7
   LDA   (FP),Y
   LDY   #$56
   STA   (FP),Y
   LDY   #$B8
   LDA   (FP),Y
   LDY   #$57
   STA   (FP),Y
   LDY   #$B9
   LDA   (FP),Y
   LDY   #$58
   STA   (FP),Y
   LDY   #$BA
   LDA   (FP),Y
   LDY   #$59
   STA   (FP),Y
   LDY   #$BB
   LDA   (FP),Y
   LDY   #$5A
   STA   (FP),Y
   LDY   #$BC
   LDA   (FP),Y
   LDY   #$5B
   STA   (FP),Y
.if_end@8:
   LDA   g0
   CLC
   LDY   #$55
   ADC   (FP),Y
   LDY   #$BD
   STA   (FP),Y
   LDA   g0+1
   LDY   #$56
   ADC   (FP),Y
   LDY   #$BE
   STA   (FP),Y
   LDA   g0+2
   LDY   #$57
   ADC   (FP),Y
   LDY   #$BF
   STA   (FP),Y
   LDA   g0+3
   LDY   #$58
   ADC   (FP),Y
   LDY   #$C0
   STA   (FP),Y
   LDA   g0+4
   LDY   #$59
   ADC   (FP),Y
   LDY   #$C1
   STA   (FP),Y
   LDA   g0+5
   LDY   #$5A
   ADC   (FP),Y
   LDY   #$C2
   STA   (FP),Y
   LDA   g0+6
   LDY   #$5B
   ADC   (FP),Y
   LDY   #$C3
   STA   (FP),Y
   LDY   #$BD
   LDA   (FP),Y
   LDY   #$C4
   STA   (FP),Y
   LDY   #$BE
   LDA   (FP),Y
   LDY   #$C5
   STA   (FP),Y
   LDY   #$BF
   LDA   (FP),Y
   LDY   #$C6
   STA   (FP),Y
   LDY   #$C0
   LDA   (FP),Y
   LDY   #$C7
   STA   (FP),Y
   LDY   #$C1
   LDA   (FP),Y
   LDY   #$C8
   STA   (FP),Y
   LDY   #$C2
   LDA   (FP),Y
   LDY   #$C9
   STA   (FP),Y
   LDY   #$C3
   LDA   (FP),Y
   AND   #$01
   LDY   #$CA
   STA   (FP),Y
   LDY   #$C4
   LDA   (FP),Y
   LDY   #$CB
   STA   (FP),Y
   LDY   #$C5
   LDA   (FP),Y
   LDY   #$CC
   STA   (FP),Y
   LDY   #$C6
   LDA   (FP),Y
   LDY   #$CD
   STA   (FP),Y
   LDY   #$C7
   LDA   (FP),Y
   LDY   #$CE
   STA   (FP),Y
   LDY   #$C8
   LDA   (FP),Y
   LDY   #$CF
   STA   (FP),Y
   LDY   #$C9
   LDA   (FP),Y
   LDY   #$D0
   STA   (FP),Y
   LDY   #$CA
   LDA   (FP),Y
   EOR   #$01
   LDY   #$D1
   STA   (FP),Y
   LDY   #$CB
   LDA   (FP),Y
   LDY   #$D2
   STA   (FP),Y
   LDY   #$CC
   LDA   (FP),Y
   LDY   #$D3
   STA   (FP),Y
   LDY   #$CD
   LDA   (FP),Y
   LDY   #$D4
   STA   (FP),Y
   LDY   #$CE
   LDA   (FP),Y
   LDY   #$D5
   STA   (FP),Y
   LDY   #$CF
   LDA   (FP),Y
   LDY   #$D6
   STA   (FP),Y
   LDY   #$D0
   LDA   (FP),Y
   LDY   #$D7
   STA   (FP),Y
   LDY   #$D1
   LDA   (FP),Y
   SEC
   SBC   #$01
   LDY   #$D8
   STA   (FP),Y
   LDY   #$D2
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
   INY
   LDA   (FP),Y
   STA   g0+4
   INY
   LDA   (FP),Y
   STA   g0+5
   INY
   LDA   (FP),Y
   STA   g0+6
   LDA   #$EB
   BEQ   .lnot_true@51
   LDA   #$00
   JMP   .lnot_end@52
.lnot_true@51:
   LDA   #$01
.lnot_end@52:
   LDY   #$D9
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@0
   JMP   .or_true@10
.lb_skip@0:
   LDY   #$1D
   LDA   (FP),Y
   LDY   #$DB
   STA   (FP),Y
   LDY   #$1E
   LDA   (FP),Y
   LDY   #$DC
   STA   (FP),Y
   LDY   #$1F
   LDA   (FP),Y
   LDY   #$DD
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
   LDY   #$55
   LDA   (FP),Y
   SEC
   LDY   #$DB
   SBC   (FP),Y
   LDY   #$56
   LDA   (FP),Y
   LDY   #$DC
   SBC   (FP),Y
   LDY   #$57
   LDA   (FP),Y
   LDY   #$DD
   SBC   (FP),Y
   LDY   #$58
   LDA   (FP),Y
   LDY   #$DE
   SBC   (FP),Y
   LDY   #$59
   LDA   (FP),Y
   LDY   #$DF
   SBC   (FP),Y
   LDY   #$5A
   LDA   (FP),Y
   LDY   #$E0
   SBC   (FP),Y
   LDY   #$5B
   LDA   (FP),Y
   LDY   #$E1
   SBC   (FP),Y
   BVC   .cmp_novf@53
   EOR   #$80
.cmp_novf@53:
   ASL
   LDA   #$00
   ROL
   LDY   #$E2
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@10
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   JMP   .or_end@11
.or_true@10:
   LDA   #$01
   LDY   #$E4
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@11:
   LDY   #$E4
   LDA   (FP),Y
   LDY   #$E6
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$E7
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@54
   LDA   #$00
   JMP   .sx_done@55
.sx_neg@54:
   LDA   #$FF
.sx_done@55:
   LDY   #$E8
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$E6
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
   ADC   #$EF
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

   ; prologue: 6 arg bytes, 64 local bytes
   SEC
   LDA   SSP
   SBC   #$42
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$41
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
   ORA   #$00
   BPL   .sx_done@57
   LDA   #$FF
.sx_done@57:
   LDY   #$03
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
   BPL   .sx_done@59
   LDA   #$FF
.sx_done@59:
   LDY   #$07
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   g1
   LDY   #$05
   ORA   (FP),Y
   LDY   #$09
   STA   (FP),Y
   LDA   g1+1
   LDY   #$06
   ORA   (FP),Y
   LDY   #$0A
   STA   (FP),Y
   LDA   g1+2
   LDY   #$07
   ORA   (FP),Y
   LDY   #$0B
   STA   (FP),Y
   LDA   g1+3
   LDY   #$08
   ORA   (FP),Y
   LDY   #$0C
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
   LDY   #$09
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
   LDY   #$0D
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
   LDY   #$46
   LDA   (FP),Y
   AND   (FP),Y
   LDY   #$11
   STA   (FP),Y
   LDY   #$47
   LDA   (FP),Y
   AND   (FP),Y
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
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$0D
   LDA   (FP),Y
   SEC
   LDY   #$13
   SBC   (FP),Y
   LDY   #$0E
   LDA   (FP),Y
   LDY   #$14
   SBC   (FP),Y
   LDY   #$0F
   LDA   (FP),Y
   LDY   #$15
   SBC   (FP),Y
   LDY   #$10
   LDA   (FP),Y
   LDY   #$16
   SBC   (FP),Y
   BVC   .cmp_novf@60
   EOR   #$80
.cmp_novf@60:
   ASL
   LDA   #$00
   ROL
   LDY   #$17
   STA   (FP),Y
   LDA   #$00
   INY
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
   BMI   .sx_neg@61
   LDA   #$00
   JMP   .sx_done@62
.sx_neg@61:
   LDA   #$FF
.sx_done@62:
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
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   g0
   LDY   #$21
   EOR   (FP),Y
   LDY   #$28
   STA   (FP),Y
   LDA   g0+1
   LDY   #$22
   EOR   (FP),Y
   LDY   #$29
   STA   (FP),Y
   LDA   g0+2
   LDY   #$23
   EOR   (FP),Y
   LDY   #$2A
   STA   (FP),Y
   LDA   g0+3
   LDY   #$24
   EOR   (FP),Y
   LDY   #$2B
   STA   (FP),Y
   LDA   g0+4
   LDY   #$25
   EOR   (FP),Y
   LDY   #$2C
   STA   (FP),Y
   LDA   g0+5
   LDY   #$26
   EOR   (FP),Y
   LDY   #$2D
   STA   (FP),Y
   LDA   g0+6
   LDY   #$27
   EOR   (FP),Y
   LDY   #$2E
   STA   (FP),Y
   LDY   #$28
   LDA   (FP),Y
   LDY   #$2F
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$48
   LDA   (FP),Y
   LDY   #$31
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$30
   LDA   (FP),Y
   LDY   #$33
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$31
   LDA   (FP),Y
   LDY   #$33
   CMP   (FP),Y
   BEQ   .cmp_true@63
   LDA   #$00
   JMP   .cmp_end@64
.cmp_true@63:
   LDA   #$01
.cmp_end@64:
   LDY   #$35
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@12
   LDA   #$FF
   EOR   #$FF
   INY
   STA   (FP),Y
   LDA   #$00
   EOR   #$FF
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@12
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   JMP   .or_end@13
.or_true@12:
   LDA   #$01
   LDY   #$39
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@13:
   LDY   #$39
   LDA   (FP),Y
   LDY   #$3B
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$43
   LDA   (FP),Y
   LDY   #$3D
   STA   (FP),Y
   LDY   #$44
   LDA   (FP),Y
   LDY   #$3E
   STA   (FP),Y
   LDY   #$45
   LDA   (FP),Y
   LDY   #$3F
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$3D
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
   ADC   #$48
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

main:
   SUBROUTINE

   ; prologue: 0 arg bytes, 31 local bytes
   SEC
   LDA   SSP
   SBC   #$21
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$20
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   LDA   #$58
   LDY   #$01
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$06
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   #$28
   STA   (SSP),Y
   LDA   #$01
   INY
   STA   (SSP),Y
   LDA   #$00
   INY
   STA   (SSP),Y
   LDA   #$0C
   INY
   STA   (SSP),Y
   LDA   #$01
   INY
   STA   (SSP),Y
   LDY   #$01
   LDA   (FP),Y
   LDY   #$06
   STA   (SSP),Y
   JSR   f0
   LDA   HARGS+8
   LDY   #$02
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
   LDY   #$02
   LDA   (FP),Y
   LDY   #$06
   STA   (FP),Y
   LDY   #$03
   LDA   (FP),Y
   LDY   #$07
   STA   (FP),Y
   LDY   #$04
   LDA   (FP),Y
   LDY   #$08
   STA   (FP),Y
   LDY   #$05
   LDA   (FP),Y
   LDY   #$09
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
   LDY   #$06
   LDA   (FP),Y
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
   LDY   #$12
   LDA   (FP),Y
   LDY   #$06
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
   LDY   #$16
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
   LDY   #$1E
   STA   (FP),Y
   LDY   #$1B
   LDA   (FP),Y
   LDY   #$1F
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
   ADC   #$21
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$20
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
   DC.B  $33
   DS.B  6

g1:
   DC.L  $0000000D
