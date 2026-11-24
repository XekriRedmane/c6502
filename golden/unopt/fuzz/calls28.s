SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24

f2:
   SUBROUTINE

   ; prologue: 8 arg bytes, 25 local bytes
   SEC
   LDA   SSP
   SBC   #$1B
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$1A
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   LDY   #$22
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   LDY   #$23
   LDA   (FP),Y
   LDY   #$02
   STA   (FP),Y
   ORA   #$00
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
   LDY   #$01
   LDA   (FP),Y
   LDY   #$05
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
   LDA   g0
   ORA   g0+1
   BEQ   .and_false@0
   LDA   g0
   INY
   STA   (FP),Y
   LDA   g0+1
   INY
   STA   (FP),Y
   LDA   g0+1
   BMI   .sx_neg@2
   LDA   #$00
   JMP   .sx_done@3
.sx_neg@2:
   LDA   #$FF
.sx_done@3:
   LDY   #$0B
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$05
   LDA   (FP),Y
   SEC
   LDY   #$09
   SBC   (FP),Y
   LDY   #$06
   LDA   (FP),Y
   LDY   #$0A
   SBC   (FP),Y
   LDY   #$07
   LDA   (FP),Y
   LDY   #$0B
   SBC   (FP),Y
   LDY   #$08
   LDA   (FP),Y
   LDY   #$0C
   SBC   (FP),Y
   BVC   .cmp_novf@4
   EOR   #$80
.cmp_novf@4:
   ASL
   LDA   #$00
   ROL
   EOR   #$01
   LDY   #$0D
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@0
   LDA   #$01
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   JMP   .and_end@1
.and_false@0:
   LDA   #$00
   LDY   #$0F
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@1:
   LDY   #$0F
   LDA   (FP),Y
   LDY   #$11
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   g0
   INY
   STA   (FP),Y
   LDA   g0+1
   INY
   STA   (FP),Y
   LDA   g2
   ORA   g2+1
   BEQ   .and_false@2
   LDA   #$00
   LDY   #$23
   CMP   (FP),Y
   BNE   .cmp_true@5
   LDA   #$01
   DEY
   CMP   (FP),Y
   BNE   .cmp_end@6
   LDA   #$00
   JMP   .cmp_end@6
.cmp_true@5:
   LDA   #$01
.cmp_end@6:
   LDY   #$15
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
   LDY   #$17
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@3:
   LDY   #$17
   LDA   (FP),Y
   LDY   #$19
   STA   (FP),Y

   ; epilogue
   PHA
   CLC
   LDA   FP
   ADC   #$23
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

   LDY   #$2F
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   LDY   #$30
   LDA   (FP),Y
   LDY   #$02
   STA   (FP),Y
   LDY   #$31
   LDA   (FP),Y
   LDY   #$03
   STA   (FP),Y
   LDY   #$2A
   LDA   (FP),Y
   LDY   #$04
   STA   (FP),Y
   LDY   #$2B
   LDA   (FP),Y
   LDY   #$05
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$04
   LDA   (FP),Y
   CLC
   LDY   #$01
   SBC   (FP),Y
   LDY   #$05
   LDA   (FP),Y
   LDY   #$02
   SBC   (FP),Y
   LDY   #$06
   LDA   (FP),Y
   LDY   #$03
   SBC   (FP),Y
   LDA   #$00
   ROL
   LDY   #$07
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   SEC
   SBC   g0
   INY
   LDA   (FP),Y
   SBC   g0+1
   BVC   .cmp_novf@8
   EOR   #$80
.cmp_novf@8:
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
   LDY   #$0B
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$0C
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@9
   LDA   #$00
   JMP   .sx_done@10
.sx_neg@9:
   LDA   #$FF
.sx_done@10:
   LDY   #$0D
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
   LDY   #$0D
   LDA   (FP),Y
   LDY   #$11
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$2C
   LDA   (FP),Y
   LDY   #$13
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$0F
   LDA   (FP),Y
   LDY   #$15
   STA   (FP),Y
   LDY   #$10
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
   LDY   #$32
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$19
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$1A
   STA   (FP),Y
   LDY   #$2E
   LDA   (FP),Y
   BEQ   .lnot_true@11
   LDA   #$00
   JMP   .lnot_end@12
.lnot_true@11:
   LDA   #$01
.lnot_end@12:
   LDY   #$1B
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$19
   LDA   (FP),Y
   CLC
   LDY   #$1B
   ADC   (FP),Y
   LDY   #$1D
   STA   (FP),Y
   LDY   #$1A
   LDA   (FP),Y
   LDY   #$1C
   ADC   (FP),Y
   LDY   #$1E
   STA   (FP),Y
   LDY   #$32
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$1D
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$1E
   LDA   (FP),Y
   LDY   #$01
   STA   (DPTR),Y
   LDA   #$FF
   STA   HARGS
   LDA   #$00
   STA   HARGS+1
   LDA   g2
   STA   HARGS+2
   LDA   g2+1
   STA   HARGS+3
   JSR   mul16
   LDA   HARGS+4
   LDY   #$1F
   STA   (FP),Y
   LDA   HARGS+5
   INY
   STA   (FP),Y
   LDY   #$14
   LDA   (FP),Y
   BEQ   .and_false@4
   LDA   #$64
   BEQ   .and_false@4
   LDA   #$01
   LDY   #$21
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   JMP   .and_end@5
.and_false@4:
   LDA   #$00
   LDY   #$21
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@5:
   LDY   #$1F
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
   JSR   mul16
   LDA   HARGS+4
   LDY   #$23
   STA   (FP),Y
   LDA   HARGS+5
   INY
   STA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@13
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@13
   LDA   #$00
   JMP   .cmp_end@14
.cmp_true@13:
   LDA   #$01
.cmp_end@14:
   LDY   #$25
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$27
   STA   (FP),Y
   CMP   #$01

   ; epilogue
   PHP
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
   PLP
   RTS

f0:
   SUBROUTINE

   ; prologue: 5 arg bytes, 137 local bytes
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

   INY
   LDA   (FP),Y
   ORA   #$01
   LDY   #$01
   STA   (FP),Y
   LDY   #$8D
   LDA   (FP),Y
   LDY   #$02
   STA   (FP),Y
   LDY   #$8E
   LDA   (FP),Y
   LDY   #$03
   STA   (FP),Y
   LDA   #$01
   STA   HARGS
   LDA   #$00
   STA   HARGS+1
   LDY   #$00
   STY   HARGS+2
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
   LDA   #$00
   STA   HARGS+7
   JSR   udivmod32
   LDA   HARGS+12
   LDY   #$04
   STA   (FP),Y
   LDA   HARGS+13
   INY
   STA   (FP),Y
   LDA   HARGS+14
   INY
   STA   (FP),Y
   LDA   g0
   INY
   STA   (FP),Y
   LDA   g0+1
   INY
   STA   (FP),Y
   LDA   g0+1
   BMI   .sx_neg@16
   LDA   #$00
   JMP   .sx_done@17
.sx_neg@16:
   LDA   #$FF
.sx_done@17:
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
   AND   #$07
   LDY   #$0C
   STA   (FP),Y
   LDY   #$8E
   LDA   (FP),Y
   LDY   #$0C
   CMP   (FP),Y
   BNE   .cmp_true@18
   LDY   #$8D
   LDA   (FP),Y
   LDY   #$0B
   CMP   (FP),Y
   BNE   .cmp_true@18
   LDY   #$8C
   LDA   (FP),Y
   LDY   #$0A
   CMP   (FP),Y
   BNE   .cmp_true@18
   LDA   #$00
   JMP   .cmp_end@19
.cmp_true@18:
   LDA   #$01
.cmp_end@19:
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
   BMI   .sx_neg@21
   LDA   #$00
   JMP   .sx_done@22
.sx_neg@21:
   LDA   #$FF
.sx_done@22:
   LDY   #$11
   STA   (FP),Y
   LDY   #$0F
   LDA   (FP),Y
   LDY   #$12
   STA   (FP),Y
   LDY   #$10
   LDA   (FP),Y
   LDY   #$13
   STA   (FP),Y
   LDY   #$11
   LDA   (FP),Y
   AND   #$07
   LDY   #$14
   STA   (FP),Y
   LDY   #$04
   LDA   (FP),Y
   CLC
   LDY   #$12
   ADC   (FP),Y
   LDY   #$15
   STA   (FP),Y
   LDY   #$05
   LDA   (FP),Y
   LDY   #$13
   ADC   (FP),Y
   LDY   #$16
   STA   (FP),Y
   LDY   #$06
   LDA   (FP),Y
   LDY   #$14
   ADC   (FP),Y
   LDY   #$17
   STA   (FP),Y
   LDY   #$15
   LDA   (FP),Y
   LDY   #$18
   STA   (FP),Y
   LDY   #$16
   LDA   (FP),Y
   LDY   #$19
   STA   (FP),Y
   LDY   #$17
   LDA   (FP),Y
   AND   #$07
   LDY   #$1A
   STA   (FP),Y
   LDY   #$18
   LDA   (FP),Y
   LDY   #$1B
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   g2
   INY
   STA   (FP),Y
   LDA   g2+1
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$1F
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   AND   #$7F
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
   LDA   #$E8
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$8F
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$25
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$26
   STA   (FP),Y
   LDY   #$24
   LDA   (FP),Y
   LDY   #$27
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@23
   LDA   #$00
   JMP   .sx_done@24
.sx_neg@23:
   LDA   #$FF
.sx_done@24:
   LDY   #$28
   STA   (FP),Y
   LDY   #$24
   LDA   (FP),Y
   LDY   #$29
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@25
   LDA   #$00
   JMP   .sx_done@26
.sx_neg@25:
   LDA   #$FF
.sx_done@26:
   LDY   #$2A
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   SEC
   LDY   #$27
   SBC   (FP),Y
   LDY   #$2A
   LDA   (FP),Y
   LDY   #$28
   SBC   (FP),Y
   BVC   .cmp_novf@27
   EOR   #$80
.cmp_novf@27:
   ASL
   LDA   #$00
   ROL
   LDY   #$2B
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$25
   LDA   (FP),Y
   CLC
   LDY   #$2B
   ADC   (FP),Y
   LDY   #$2D
   STA   (FP),Y
   LDY   #$26
   LDA   (FP),Y
   LDY   #$2C
   ADC   (FP),Y
   LDY   #$2E
   STA   (FP),Y
   LDY   #$8F
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$2D
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$2E
   LDA   (FP),Y
   LDY   #$01
   STA   (DPTR),Y
   LDY   #$1C
   LDA   (FP),Y
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
   LDY   #$21
   LDA   (FP),Y
   LDY   #$33
   STA   (FP),Y
   LDY   #$22
   LDA   (FP),Y
   LDY   #$34
   STA   (FP),Y
   LDA   #$01
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$33
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
   LDY   #$37
   STA   (FP),Y
   LDA   HARGS+5
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
   LDA   g1
   INY
   STA   (FP),Y
   LDA   g1+1
   INY
   STA   (FP),Y
   LDA   g1+1
   BMI   .sx_neg@28
   LDA   #$00
   JMP   .sx_done@29
.sx_neg@28:
   LDA   #$FF
.sx_done@29:
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
   LDA   #$03
   SEC
   SBC   #$FF
   LDA   #$00
   SBC   #$7F
   BVC   .cmp_novf@30
   EOR   #$80
.cmp_novf@30:
   ASL
   LDA   #$00
   ROL
   LDY   #$41
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$43
   STA   (FP),Y
   LDY   #$1C
   LDA   (FP),Y
   LDY   #$44
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   EOR   #$FF
   LDY   #$46
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   EOR   #$7F
   LDY   #$47
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$48
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$49
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@31
   LDA   #$00
   JMP   .sx_done@32
.sx_neg@31:
   LDA   #$FF
.sx_done@32:
   LDY   #$4A
   STA   (FP),Y
   LDY   #$48
   LDA   (FP),Y
   LDY   #$4B
   STA   (FP),Y
   LDY   #$49
   LDA   (FP),Y
   LDY   #$4C
   STA   (FP),Y
   LDY   #$4A
   LDA   (FP),Y
   AND   #$07
   LDY   #$4D
   STA   (FP),Y
   CLC
   LDA   FP
   ADC   #$31
   INY
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$0A
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDY   #$39
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$3A
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
   LDY   #$43
   LDA   (FP),Y
   LDY   #$05
   STA   (SSP),Y
   LDY   #$4B
   LDA   (FP),Y
   LDY   #$06
   STA   (SSP),Y
   LDY   #$4C
   LDA   (FP),Y
   LDY   #$07
   STA   (SSP),Y
   LDY   #$4D
   LDA   (FP),Y
   LDY   #$08
   STA   (SSP),Y
   LDY   #$4E
   LDA   (FP),Y
   LDY   #$09
   STA   (SSP),Y
   LDY   #$4F
   LDA   (FP),Y
   LDY   #$0A
   STA   (SSP),Y
   JSR   f1
   LDA   #$00
   ROL
   LDY   #$50
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$21
   LDA   (FP),Y
   LDY   #$53
   STA   (FP),Y
   LDY   #$22
   LDA   (FP),Y
   LDY   #$54
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$51
   EOR   (FP),Y
   LDY   #$55
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$52
   EOR   (FP),Y
   LDY   #$56
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$57
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$58
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$59
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   AND   #$7F
   LDY   #$5A
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$21
   STA   (FP),Y
   LDY   #$5A
   LDA   (FP),Y
   LDY   #$22
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$5B
   STA   (FP),Y
   LDY   #$22
   LDA   (FP),Y
   LDY   #$5C
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   LDY   #$31
   ADC   (FP),Y
   LDY   #$5D
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$32
   ADC   (FP),Y
   LDY   #$5E
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$5F
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$60
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$61
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   AND   #$7F
   LDY   #$62
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$21
   STA   (FP),Y
   LDY   #$62
   LDA   (FP),Y
   LDY   #$22
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$63
   STA   (FP),Y
   LDY   #$22
   LDA   (FP),Y
   LDY   #$64
   STA   (FP),Y
   LDA   g2
   CLC
   DEY
   ADC   (FP),Y
   LDY   #$65
   STA   (FP),Y
   LDA   g2+1
   DEY
   ADC   (FP),Y
   LDY   #$66
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   g2
   INY
   LDA   (FP),Y
   STA   g2+1
   SEC
   LDA   #$00
   LDY   #$21
   SBC   (FP),Y
   LDY   #$67
   STA   (FP),Y
   LDA   #$00
   LDY   #$22
   SBC   (FP),Y
   LDY   #$68
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$69
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   AND   #$7F
   LDY   #$6A
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$6B
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$6C
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$21
   LDA   (FP),Y
   LDY   #$6F
   STA   (FP),Y
   LDY   #$22
   LDA   (FP),Y
   LDY   #$70
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   SEC
   SBC   g1
   INY
   LDA   (FP),Y
   SBC   g1+1
   BVC   .cmp_novf@33
   EOR   #$80
.cmp_novf@33:
   ASL
   LDA   #$00
   ROL
   LDY   #$71
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   SEC
   SBC   #$E8
   INY
   STA   (FP),Y
   LDA   #$00
   SBC   #$03
   INY
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$08
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDY   #$6B
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$6C
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$6D
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$6E
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
   LDY   #$73
   LDA   (FP),Y
   LDY   #$07
   STA   (SSP),Y
   LDY   #$74
   LDA   (FP),Y
   LDY   #$08
   STA   (SSP),Y
   JSR   f2
   LDY   #$75
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$24
   LDA   (FP),Y
   LDY   #$78
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@34
   LDA   #$00
   JMP   .sx_done@35
.sx_neg@34:
   LDA   #$FF
.sx_done@35:
   LDY   #$79
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$76
   EOR   (FP),Y
   LDY   #$7A
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$77
   EOR   (FP),Y
   LDY   #$7B
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$7C
   STA   (FP),Y
   LDY   #$24
   STA   (FP),Y
   LDY   #$7D
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@36
   LDA   #$00
   JMP   .sx_done@37
.sx_neg@36:
   LDA   #$FF
.sx_done@37:
   LDY   #$7E
   STA   (FP),Y
   LDA   g2
   CLC
   DEY
   ADC   (FP),Y
   LDY   #$7F
   STA   (FP),Y
   LDA   g2+1
   DEY
   ADC   (FP),Y
   LDY   #$80
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   g2
   INY
   LDA   (FP),Y
   STA   g2+1
   LDY   #$1C
   LDA   (FP),Y
   LDY   #$81
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDA   #$07
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$81
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
   LDY   #$85
   STA   (FP),Y
   LDA   HARGS+5
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   EOR   #$FF
   LDY   #$87
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   EOR   #$FF
   LDY   #$88
   STA   (FP),Y
   DEY
   ORA   (FP),Y
   BEQ   .cmp_end@38
   LDA   #$01
.cmp_end@38:
   LDY   #$89
   STA   (FP),Y

   ; epilogue
   PHA
   CLC
   LDA   FP
   ADC   #$90
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

   ; prologue: 0 arg bytes, 47 local bytes
   SEC
   LDA   SSP
   SBC   #$31
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$30
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
   SBC   #$05
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   #$2A
   LDY   #$01
   STA   (SSP),Y
   LDA   #$00
   INY
   STA   (SSP),Y
   INY
   STA   (SSP),Y
   LDA   (FP),Y
   INY
   STA   (SSP),Y
   LDA   (FP),Y
   INY
   STA   (SSP),Y
   JSR   f0
   LDY   #$05
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
   LDY   #$06
   LDA   (FP),Y
   LDY   #$0A
   STA   (FP),Y
   LDY   #$07
   LDA   (FP),Y
   LDY   #$0B
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
   BMI   .sx_neg@39
   LDA   #$00
   JMP   .sx_done@40
.sx_neg@39:
   LDA   #$FF
.sx_done@40:
   LDY   #$10
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
   BMI   .sx_neg@41
   LDA   #$00
   JMP   .sx_done@42
.sx_neg@41:
   LDA   #$FF
.sx_done@42:
   LDY   #$14
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
   LDA   g2
   INY
   STA   (FP),Y
   LDA   g2+1
   INY
   STA   (FP),Y
   LDA   g2+1
   BMI   .sx_neg@43
   LDA   #$00
   JMP   .sx_done@44
.sx_neg@43:
   LDA   #$FF
.sx_done@44:
   LDY   #$1C
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$16
   LDA   (FP),Y
   LDY   #$1A
   EOR   (FP),Y
   LDY   #$1E
   STA   (FP),Y
   LDY   #$17
   LDA   (FP),Y
   LDY   #$1B
   EOR   (FP),Y
   LDY   #$1F
   STA   (FP),Y
   LDY   #$18
   LDA   (FP),Y
   LDY   #$1C
   EOR   (FP),Y
   LDY   #$20
   STA   (FP),Y
   LDY   #$19
   LDA   (FP),Y
   LDY   #$1D
   EOR   (FP),Y
   LDY   #$21
   STA   (FP),Y
   LDY   #$0A
   LDA   (FP),Y
   LDY   #$1E
   EOR   (FP),Y
   LDY   #$22
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
   LDY   #$22
   LDA   (FP),Y
   LDY   #$0A
   STA   (FP),Y
   LDY   #$23
   LDA   (FP),Y
   LDY   #$0B
   STA   (FP),Y
   LDY   #$24
   LDA   (FP),Y
   LDY   #$0C
   STA   (FP),Y
   LDY   #$25
   LDA   (FP),Y
   LDY   #$0D
   STA   (FP),Y
   LDY   #$0A
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
   LDY   #$26
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
   LDY   #$0A
   LDA   (FP),Y
   LDY   #$26
   EOR   (FP),Y
   LDY   #$2A
   STA   (FP),Y
   LDY   #$0B
   LDA   (FP),Y
   LDY   #$27
   EOR   (FP),Y
   LDY   #$2B
   STA   (FP),Y
   LDY   #$0C
   LDA   (FP),Y
   LDY   #$28
   EOR   (FP),Y
   LDY   #$2C
   STA   (FP),Y
   LDY   #$0D
   LDA   (FP),Y
   LDY   #$29
   EOR   (FP),Y
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
   LDY   #$30
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
   DC.W  $008B

g1:
   DC.W  $0039

g2:
   DC.W  $00A3
