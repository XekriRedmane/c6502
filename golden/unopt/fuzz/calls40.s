SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24

f5:
   SUBROUTINE

   ; prologue: 6 arg bytes, 9 local bytes
   SEC
   LDA   SSP
   SBC   #$0B
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$0A
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
   BPL   .sx_done@1
   LDA   #$FF
.sx_done@1:
   LDY   #$03
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   g1+3
   CMP   (FP),Y
   BNE   .thread@0
   LDA   g1+2
   DEY
   CMP   (FP),Y
   BNE   .thread@0
   LDA   g1+1
   DEY
   CMP   (FP),Y
   BNE   .thread@0
   LDA   g1
   DEY
   CMP   (FP),Y
   BEQ   .cmp_true@2
.thread@0:
   LDA   #$00
   JMP   .cmp_end@3
.cmp_true@2:
   LDA   #$01
.cmp_end@3:
   LDY   #$05
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$07
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   g2
   INY
   STA   (FP),Y

   ; epilogue
   PHA
   CLC
   LDA   FP
   ADC   #$11
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

f4:
   SUBROUTINE

   ; prologue: 1 arg bytes, 72 local bytes
   SEC
   LDA   SSP
   SBC   #$4A
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$49
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   LDA   #$E8
   ORA   g2
   LDY   #$01
   STA   (FP),Y
   LDA   #$03
   ORA   g2+1
   INY
   STA   (FP),Y
   LDA   g0
   INY
   STA   (FP),Y
   LDA   g0
   BMI   .sx_neg@5
   LDA   #$00
   JMP   .sx_done@6
.sx_neg@5:
   LDA   #$FF
.sx_done@6:
   LDY   #$04
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ORA   #$E8
   LDY   #$05
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ORA   #$03
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
   LDY   #$07
   AND   (FP),Y
   LDY   #$09
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   LDY   #$08
   AND   (FP),Y
   LDY   #$0A
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$0B
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDA   #$02
   DEY
   ORA   (FP),Y
   LDY   #$0F
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$10
   STA   (FP),Y
   DEY
   LDA   (FP),Y
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
   DEY
   LDA   (FP),Y
   LDY   #$14
   STA   (FP),Y
   LDA   #<f5
   INY
   STA   (FP),Y
   LDA   #>f5
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$17
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$18
   STA   (FP),Y
   LDY   #$4B
   LDA   (FP),Y
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
   BMI   .sx_neg@7
   LDA   #$00
   JMP   .sx_done@8
.sx_neg@7:
   LDA   #$FF
.sx_done@8:
   LDY   #$1D
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$1B
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
   BVC   .cmp_novf@9
   EOR   #$80
.cmp_novf@9:
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
   BMI   .sx_neg@10
   LDA   #$00
   JMP   .sx_done@11
.sx_neg@10:
   LDA   #$FF
.sx_done@11:
   LDY   #$23
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
   LDY   #$26
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$25
   LDA   (FP),Y
   LDY   #$29
   STA   (FP),Y
   LDY   #$26
   LDA   (FP),Y
   LDY   #$2A
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$06
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDY   #$21
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$22
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$23
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$24
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   LDY   #$29
   LDA   (FP),Y
   LDY   #$05
   STA   (SSP),Y
   LDY   #$2A
   LDA   (FP),Y
   LDY   #$06
   STA   (SSP),Y
   LDY   #$17
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   JSR   icall
   LDY   #$2B
   STA   (FP),Y
   INY
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@14
   LDA   #$00
   JMP   .sx_done@15
.sx_neg@14:
   LDA   #$FF
.sx_done@15:
   LDY   #$2D
   STA   (FP),Y
   LDY   #$0C
   LDA   (FP),Y
   LDY   #$2E
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$2C
   EOR   (FP),Y
   LDY   #$30
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$2D
   EOR   (FP),Y
   LDY   #$31
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$32
   STA   (FP),Y
   LDY   #$0C
   STA   (FP),Y
   LDY   #$33
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDA   g2
   CLC
   DEY
   ADC   (FP),Y
   LDY   #$35
   STA   (FP),Y
   LDA   g2+1
   DEY
   ADC   (FP),Y
   LDY   #$36
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   g2
   INY
   LDA   (FP),Y
   STA   g2+1
   LDA   g2
   AND   #$07
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDA   #$FF
   INY
   STA   (FP),Y
   LDA   #$7F
   INY
   STA   (FP),Y
   LDY   #$37
   LDA   (FP),Y
   TAX
   BEQ   .shift_done@17
.shift_loop@16:
   LDY   #$39
   LDA   (FP),Y
   ASL
   STA   (FP),Y
   INY
   LDA   (FP),Y
   ROL
   STA   (FP),Y
   DEX
   BNE   .shift_loop@16
.shift_done@17:
   LDA   g0
   LDY   #$3B
   STA   (FP),Y
   LDA   g0
   BMI   .sx_neg@18
   LDA   #$00
   JMP   .sx_done@19
.sx_neg@18:
   LDA   #$FF
.sx_done@19:
   LDY   #$3C
   STA   (FP),Y
   LDY   #$4B
   LDA   (FP),Y
   LDY   #$3D
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   SEC
   LDY   #$3B
   SBC   (FP),Y
   LDY   #$3E
   LDA   (FP),Y
   LDY   #$3C
   SBC   (FP),Y
   BVC   .cmp_novf@20
   EOR   #$80
.cmp_novf@20:
   ASL
   LDA   #$00
   ROL
   LDY   #$3F
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   AND   #$07
   LDY   #$41
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDA   #$00
   LDY   #$42
   STA   (FP),Y
   LDY   #$39
   LDA   (FP),Y
   LDY   #$43
   STA   (FP),Y
   LDY   #$3A
   LDA   (FP),Y
   LDY   #$44
   STA   (FP),Y
   LDY   #$41
   LDA   (FP),Y
   TAX
   BEQ   .shift_done@22
.shift_loop@21:
   LDY   #$44
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
   BNE   .shift_loop@21
.shift_done@22:
   LDY   #$43
   LDA   (FP),Y
   LDY   #$45
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$46
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@23
   LDA   #$00
   JMP   .sx_done@24
.sx_neg@23:
   LDA   #$FF
.sx_done@24:
   LDY   #$47
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$45
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
   RTS

f3:
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

   LDA   g0
   LDY   #$01
   STA   (FP),Y
   LDA   g0
   BMI   .sx_neg@25
   LDA   #$00
   JMP   .sx_done@26
.sx_neg@25:
   LDA   #$FF
.sx_done@26:
   LDY   #$02
   STA   (FP),Y
   LDA   g0
   INY
   STA   (FP),Y
   LDA   g0
   BMI   .sx_neg@27
   LDA   #$00
   JMP   .sx_done@28
.sx_neg@27:
   LDA   #$FF
.sx_done@28:
   LDY   #$04
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$05
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$06
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   SBC   g2
   INY
   LDA   (FP),Y
   SBC   g2+1
   LDA   #$00
   ROL
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$01
   LDA   (FP),Y
   SEC
   LDY   #$07
   SBC   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   LDY   #$08
   SBC   (FP),Y
   BVC   .cmp_novf@29
   EOR   #$80
.cmp_novf@29:
   ASL
   LDA   #$00
   ROL
   EOR   #$01
   LDY   #$09
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$0B
   STA   (FP),Y
   INY
   STA   (FP),Y
   ORA   #$00
   BNE   .or_true@0
   INY
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@30
   LDA   #$00
   JMP   .sx_done@31
.sx_neg@30:
   LDA   #$FF
.sx_done@31:
   LDY   #$0E
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
   BMI   .sx_neg@32
   LDA   #$00
   JMP   .sx_done@33
.sx_neg@32:
   LDA   #$FF
.sx_done@33:
   LDY   #$11
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   g1
   SEC
   LDY   #$0F
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
   BVC   .cmp_novf@34
   EOR   #$80
.cmp_novf@34:
   ASL
   LDA   #$00
   ROL
   LDY   #$13
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@0
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   JMP   .or_end@1
.or_true@0:
   LDA   #$01
   LDY   #$15
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@1:
   LDY   #$15
   LDA   (FP),Y
   LDY   #$17
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$18
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@35
   LDA   #$00
   JMP   .sx_done@36
.sx_neg@35:
   LDA   #$FF
.sx_done@36:
   LDY   #$19
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$17
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
   ADC   #$1C
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

   ; prologue: 2 arg bytes, 60 local bytes
   SEC
   LDA   SSP
   SBC   #$3E
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$3D
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
   LDA   g1+1
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
   LDA   g0
   INY
   STA   (FP),Y
   LDA   g0
   BMI   .sx_neg@37
   LDA   #$00
   JMP   .sx_done@38
.sx_neg@37:
   LDA   #$FF
.sx_done@38:
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
   LDA   g2
   ORA   #$01
   INY
   STA   (FP),Y
   LDA   g2+1
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
   JSR   udivmod16
   LDA   HARGS+4
   LDY   #$0B
   STA   (FP),Y
   LDA   HARGS+5
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   SBC   #$FF
   INY
   LDA   (FP),Y
   SBC   #$00
   LDA   #$00
   ROL
   INY
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
   LDA   g0
   INY
   STA   (FP),Y
   LDY   #$0F
   LDA   (FP),Y
   LDY   #$12
   STA   (FP),Y
   LDY   #$10
   LDA   (FP),Y
   LDY   #$13
   STA   (FP),Y
   LDY   #$3F
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$14
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$15
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   LDY   #$03
   ADC   (FP),Y
   LDY   #$16
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$04
   ADC   (FP),Y
   LDY   #$17
   STA   (FP),Y
   LDY   #$3F
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$16
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$17
   LDA   (FP),Y
   LDY   #$01
   STA   (DPTR),Y
   LDA   #<f5
   LDY   #$18
   STA   (FP),Y
   LDA   #>f5
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
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
.loop@0_start:
   LDY   #$1C
   LDA   (FP),Y
   SEC
   SBC   #$03
   INY
   LDA   (FP),Y
   SBC   #$00
   BVC   .cmp_novf@39
   EOR   #$80
.cmp_novf@39:
   ASL
   LDA   #$00
   ROL
   LDY   #$1E
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
   LDY   #$0F
   LDA   (FP),Y
   LDY   #$20
   STA   (FP),Y
   INY
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@40
   LDA   #$00
   JMP   .sx_done@41
.sx_neg@40:
   LDA   #$FF
.sx_done@41:
   LDY   #$22
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$06
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDY   #$21
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$22
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$23
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$24
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   LDA   g2
   INY
   STA   (SSP),Y
   LDA   g2+1
   INY
   STA   (SSP),Y
   LDY   #$1A
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   JSR   icall
   LDY   #$25
   STA   (FP),Y
   INY
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@42
   LDA   #$00
   JMP   .sx_done@43
.sx_neg@42:
   LDA   #$FF
.sx_done@43:
   LDY   #$27
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   LDY   #$1C
   ADC   (FP),Y
   LDY   #$28
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$1D
   ADC   (FP),Y
   LDY   #$29
   STA   (FP),Y
   LDY   #$12
   LDA   (FP),Y
   CLC
   LDY   #$28
   ADC   (FP),Y
   LDY   #$2A
   STA   (FP),Y
   LDY   #$13
   LDA   (FP),Y
   LDY   #$29
   ADC   (FP),Y
   LDY   #$2B
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$12
   STA   (FP),Y
   LDY   #$2B
   LDA   (FP),Y
   LDY   #$13
   STA   (FP),Y
   LDY   #$1C
   LDA   (FP),Y
   LDY   #$2C
   STA   (FP),Y
   LDY   #$1D
   LDA   (FP),Y
   LDY   #$2D
   STA   (FP),Y
   LDY   #$1C
   LDA   (FP),Y
   CLC
   ADC   #$01
   LDY   #$2E
   STA   (FP),Y
   LDY   #$1D
   LDA   (FP),Y
   ADC   #$00
   LDY   #$2F
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$1C
   STA   (FP),Y
   LDY   #$2F
   LDA   (FP),Y
   LDY   #$1D
   STA   (FP),Y
   JMP   .loop@0_start
.loop@0_break:
   LDY   #$3F
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$30
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$31
   STA   (FP),Y
   LDA   #$07
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   ORA   #$00
   BPL   .sx_done@45
   LDA   #$FF
.sx_done@45:
   LDY   #$34
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   g1
   LDY   #$32
   AND   (FP),Y
   LDY   #$36
   STA   (FP),Y
   LDA   g1+1
   LDY   #$33
   AND   (FP),Y
   LDY   #$37
   STA   (FP),Y
   LDA   g1+2
   LDY   #$34
   AND   (FP),Y
   LDY   #$38
   STA   (FP),Y
   LDA   g1+3
   LDY   #$35
   AND   (FP),Y
   LDY   #$39
   STA   (FP),Y
   LDY   #$30
   LDA   (FP),Y
   LDY   #$3A
   STA   (FP),Y
   LDY   #$31
   LDA   (FP),Y
   LDY   #$3B
   STA   (FP),Y
   LDY   #$36
   LDA   (FP),Y
   TAX
   BEQ   .shift_done@47
.shift_loop@46:
   LDY   #$3B
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
   BNE   .shift_loop@46
.shift_done@47:
   LDY   #$3B
   LDA   (FP),Y
   DEY
   ORA   (FP),Y
   BEQ   .cmp_end@48
   LDA   #$01
.cmp_end@48:
   LDY   #$3C
   STA   (FP),Y

   ; epilogue
   PHA
   CLC
   LDA   FP
   ADC   #$40
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

   ; prologue: 0 arg bytes, 122 local bytes
   SEC
   LDA   SSP
   SBC   #$7C
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$7B
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
   LDA   g0
   BMI   .sx_neg@49
   LDA   #$00
   JMP   .sx_done@50
.sx_neg@49:
   LDA   #$FF
.sx_done@50:
   LDY   #$02
   STA   (FP),Y
   LDA   g0
   INY
   STA   (FP),Y
   LDA   g0
   BMI   .sx_neg@51
   LDA   #$00
   JMP   .sx_done@52
.sx_neg@51:
   LDA   #$FF
.sx_done@52:
   LDY   #$04
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
   JSR   mul16
   LDA   HARGS+4
   LDY   #$05
   STA   (FP),Y
   LDA   HARGS+5
   INY
   STA   (FP),Y
   LDA   g2
   ORA   g2+1
   BEQ   .lnot_true@53
   LDA   #$00
   JMP   .lnot_end@54
.lnot_true@53:
   LDA   #$01
.lnot_end@54:
   LDY   #$07
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$06
   LDA   (FP),Y
   LDY   #$08
   CMP   (FP),Y
   BNE   .cmp_true@55
   LDY   #$05
   LDA   (FP),Y
   LDY   #$07
   CMP   (FP),Y
   BNE   .cmp_true@55
   LDA   #$00
   JMP   .cmp_end@56
.cmp_true@55:
   LDA   #$01
.cmp_end@56:
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
   DEY
   LDA   (FP),Y
   LDY   #$0D
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$0E
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   HARGS
   INY
   LDA   (FP),Y
   STA   HARGS+1
   LDA   g2
   STA   HARGS+2
   LDA   g2+1
   STA   HARGS+3
   JSR   mul16
   LDA   HARGS+4
   LDY   #$0F
   STA   (FP),Y
   LDA   HARGS+5
   INY
   STA   (FP),Y
   LDA   #$FF
   INY
   STA   (FP),Y
   LDA   g2+1
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   LDY   #$0F
   SBC   (FP),Y
   LDY   #$12
   LDA   (FP),Y
   LDY   #$10
   SBC   (FP),Y
   LDA   #$00
   ROL
   LDY   #$13
   STA   (FP),Y
   LDA   #$00
   INY
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
   BMI   .sx_neg@58
   LDA   #$00
   JMP   .sx_done@59
.sx_neg@58:
   LDA   #$FF
.sx_done@59:
   LDY   #$17
   STA   (FP),Y
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
   INY
   STA   (FP),Y
   SEC
   LDA   #$00
   SBC   #$FF
   INY
   STA   (FP),Y
   LDA   #$00
   SBC   #$00
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
   LDA   g2
   ORA   #$01
   INY
   STA   (FP),Y
   LDA   g2+1
   INY
   STA   (FP),Y
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
   JSR   udivmod16
   LDA   HARGS+6
   LDY   #$23
   STA   (FP),Y
   LDA   HARGS+7
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$25
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$26
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$25
   LDA   (FP),Y
   LDY   #$29
   STA   (FP),Y
   LDY   #$26
   LDA   (FP),Y
   LDY   #$2A
   STA   (FP),Y
   LDY   #$27
   LDA   (FP),Y
   LDY   #$2B
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$0D
   LDA   (FP),Y
   LDY   #$2D
   STA   (FP),Y
   LDY   #$0E
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
   LDY   #$29
   ORA   (FP),Y
   LDY   #$31
   STA   (FP),Y
   LDY   #$2E
   LDA   (FP),Y
   LDY   #$2A
   ORA   (FP),Y
   LDY   #$32
   STA   (FP),Y
   LDY   #$2F
   LDA   (FP),Y
   LDY   #$2B
   ORA   (FP),Y
   LDY   #$33
   STA   (FP),Y
   LDY   #$30
   LDA   (FP),Y
   LDY   #$2C
   ORA   (FP),Y
   LDY   #$34
   STA   (FP),Y
   LDY   #$31
   LDA   (FP),Y
   EOR   #$FF
   LDY   #$35
   STA   (FP),Y
   LDY   #$32
   LDA   (FP),Y
   EOR   #$FF
   LDY   #$36
   STA   (FP),Y
   LDY   #$33
   LDA   (FP),Y
   EOR   #$FF
   LDY   #$37
   STA   (FP),Y
   LDY   #$34
   LDA   (FP),Y
   EOR   #$FF
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
   LDA   #<f5
   INY
   STA   (FP),Y
   LDA   #>f5
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$3F
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$40
   STA   (FP),Y
   LDA   g2
   AND   #$E8
   INY
   STA   (FP),Y
   LDA   g2+1
   AND   #$03
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$43
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$44
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$06
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDY   #$43
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$44
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$45
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$46
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   LDA   #$E8
   INY
   STA   (SSP),Y
   LDA   #$03
   INY
   STA   (SSP),Y
   LDY   #$3F
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   JSR   icall
   LDY   #$47
   STA   (FP),Y
   INY
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@60
   LDA   #$00
   JMP   .sx_done@61
.sx_neg@60:
   LDA   #$FF
.sx_done@61:
   LDY   #$49
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$19
   LDA   (FP),Y
   LDY   #$48
   EOR   (FP),Y
   LDY   #$4C
   STA   (FP),Y
   LDY   #$1A
   LDA   (FP),Y
   LDY   #$49
   EOR   (FP),Y
   LDY   #$4D
   STA   (FP),Y
   LDY   #$1B
   LDA   (FP),Y
   LDY   #$4A
   EOR   (FP),Y
   LDY   #$4E
   STA   (FP),Y
   LDY   #$1C
   LDA   (FP),Y
   LDY   #$4B
   EOR   (FP),Y
   LDY   #$4F
   STA   (FP),Y
   LDY   #$4C
   LDA   (FP),Y
   LDY   #$19
   STA   (FP),Y
   LDY   #$4D
   LDA   (FP),Y
   LDY   #$1A
   STA   (FP),Y
   LDY   #$4E
   LDA   (FP),Y
   LDY   #$1B
   STA   (FP),Y
   LDY   #$4F
   LDA   (FP),Y
   LDY   #$1C
   STA   (FP),Y
   LDA   g2
   LDY   #$50
   STA   (FP),Y
   LDA   g2+1
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$50
   LDA   (FP),Y
   CLC
   LDY   #$19
   ADC   (FP),Y
   LDY   #$54
   STA   (FP),Y
   LDY   #$51
   LDA   (FP),Y
   LDY   #$1A
   ADC   (FP),Y
   LDY   #$55
   STA   (FP),Y
   LDY   #$52
   LDA   (FP),Y
   LDY   #$1B
   ADC   (FP),Y
   LDY   #$56
   STA   (FP),Y
   LDY   #$53
   LDA   (FP),Y
   LDY   #$1C
   ADC   (FP),Y
   LDY   #$57
   STA   (FP),Y
   LDY   #$54
   LDA   (FP),Y
   LDY   #$58
   STA   (FP),Y
   LDY   #$55
   LDA   (FP),Y
   LDY   #$59
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   g2
   INY
   LDA   (FP),Y
   STA   g2+1
   LDY   #$0D
   LDA   (FP),Y
   LDY   #$5A
   STA   (FP),Y
   LDY   #$0E
   LDA   (FP),Y
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
   CLC
   LDA   FP
   ADC   #$5C
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
   LDY   #$5F
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   JSR   f2
   LDY   #$60
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
   LDY   #$39
   LDA   (FP),Y
   LDY   #$61
   EOR   (FP),Y
   LDY   #$65
   STA   (FP),Y
   LDY   #$3A
   LDA   (FP),Y
   LDY   #$62
   EOR   (FP),Y
   LDY   #$66
   STA   (FP),Y
   LDY   #$3B
   LDA   (FP),Y
   LDY   #$63
   EOR   (FP),Y
   LDY   #$67
   STA   (FP),Y
   LDY   #$3C
   LDA   (FP),Y
   LDY   #$64
   EOR   (FP),Y
   LDY   #$68
   STA   (FP),Y
   LDY   #$65
   LDA   (FP),Y
   LDY   #$39
   STA   (FP),Y
   LDY   #$66
   LDA   (FP),Y
   LDY   #$3A
   STA   (FP),Y
   LDY   #$67
   LDA   (FP),Y
   LDY   #$3B
   STA   (FP),Y
   LDY   #$68
   LDA   (FP),Y
   LDY   #$3C
   STA   (FP),Y
   LDY   #$5C
   LDA   (FP),Y
   LDY   #$69
   STA   (FP),Y
   LDY   #$5D
   LDA   (FP),Y
   LDY   #$6A
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@62
   LDA   #$00
   JMP   .sx_done@63
.sx_neg@62:
   LDA   #$FF
.sx_done@63:
   LDY   #$6B
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$39
   LDA   (FP),Y
   CLC
   LDY   #$69
   ADC   (FP),Y
   LDY   #$6D
   STA   (FP),Y
   LDY   #$3A
   LDA   (FP),Y
   LDY   #$6A
   ADC   (FP),Y
   LDY   #$6E
   STA   (FP),Y
   LDY   #$3B
   LDA   (FP),Y
   LDY   #$6B
   ADC   (FP),Y
   LDY   #$6F
   STA   (FP),Y
   LDY   #$3C
   LDA   (FP),Y
   LDY   #$6C
   ADC   (FP),Y
   LDY   #$70
   STA   (FP),Y
   LDY   #$6D
   LDA   (FP),Y
   LDY   #$39
   STA   (FP),Y
   LDY   #$6E
   LDA   (FP),Y
   LDY   #$3A
   STA   (FP),Y
   LDY   #$6F
   LDA   (FP),Y
   LDY   #$3B
   STA   (FP),Y
   LDY   #$70
   LDA   (FP),Y
   LDY   #$3C
   STA   (FP),Y
   LDA   #$07
   LDY   #$71
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   ORA   #$00
   BPL   .sx_done@65
   LDA   #$FF
.sx_done@65:
   LDY   #$73
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   g1
   LDY   #$71
   AND   (FP),Y
   LDY   #$75
   STA   (FP),Y
   LDA   g1+1
   LDY   #$72
   AND   (FP),Y
   LDY   #$76
   STA   (FP),Y
   LDA   g1+2
   LDY   #$73
   AND   (FP),Y
   LDY   #$77
   STA   (FP),Y
   LDA   g1+3
   LDY   #$74
   AND   (FP),Y
   LDY   #$78
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$75
   LDA   (FP),Y
   TAX
   BEQ   .shift_done@67
.shift_loop@66:
   LDY   #$79
   LDA   (FP),Y
   ASL
   STA   (FP),Y
   INY
   LDA   (FP),Y
   ROL
   STA   (FP),Y
   DEX
   BNE   .shift_loop@66
.shift_done@67:
   LDY   #$7A
   LDA   (FP),Y
   TAX
   DEY
   LDA   (FP),Y

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$7C
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$7B
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

   ; prologue: 0 arg bytes, 119 local bytes
   SEC
   LDA   SSP
   SBC   #$79
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$78
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   LDA   #$01
   LDY   #$01
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   ORA   #$00
   BPL   .sx_done@69
   LDA   #$FF
.sx_done@69:
   LDY   #$03
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   g1
   LDY   #$01
   ORA   (FP),Y
   LDY   #$05
   STA   (FP),Y
   LDA   g1+1
   LDY   #$02
   ORA   (FP),Y
   LDY   #$06
   STA   (FP),Y
   LDA   g1+2
   LDY   #$03
   ORA   (FP),Y
   LDY   #$07
   STA   (FP),Y
   LDA   g1+3
   LDY   #$04
   ORA   (FP),Y
   LDY   #$08
   STA   (FP),Y
   LDA   g1
   STA   HARGS
   LDA   g1+1
   STA   HARGS+1
   LDA   g1+2
   STA   HARGS+2
   LDA   g1+3
   STA   HARGS+3
   LDY   #$05
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
   LDY   #$09
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
   LDY   #$09
   LDA   (FP),Y
   LDY   #$0D
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
   LDY   #$0E
   LDA   (FP),Y
   LDY   #$12
   STA   (FP),Y
   LDY   #$0F
   LDA   (FP),Y
   LDY   #$13
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
.loop@1_start:
   LDY   #$16
   LDA   (FP),Y
   SEC
   SBC   #$03
   INY
   LDA   (FP),Y
   SBC   #$00
   BVC   .cmp_novf@70
   EOR   #$80
.cmp_novf@70:
   ASL
   LDA   #$00
   ROL
   LDY   #$18
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@2
   JMP   .loop@1_break
.lb_skip@2:
   JSR   f1
   LDY   #$1A
   STA   (FP),Y
   TXA
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   LDY   #$16
   ADC   (FP),Y
   LDY   #$1C
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$17
   ADC   (FP),Y
   LDY   #$1D
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
   BMI   .sx_neg@71
   LDA   #$00
   JMP   .sx_done@72
.sx_neg@71:
   LDA   #$FF
.sx_done@72:
   LDY   #$20
   STA   (FP),Y
   INY
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
   AND   #$03
   INY
   STA   (FP),Y
   LDY   #$12
   LDA   (FP),Y
   CLC
   LDY   #$22
   ADC   (FP),Y
   LDY   #$26
   STA   (FP),Y
   LDY   #$13
   LDA   (FP),Y
   LDY   #$23
   ADC   (FP),Y
   LDY   #$27
   STA   (FP),Y
   LDY   #$14
   LDA   (FP),Y
   LDY   #$24
   ADC   (FP),Y
   LDY   #$28
   STA   (FP),Y
   LDY   #$15
   LDA   (FP),Y
   LDY   #$25
   ADC   (FP),Y
   LDY   #$29
   STA   (FP),Y
   LDY   #$26
   LDA   (FP),Y
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
   AND   #$03
   LDY   #$2D
   STA   (FP),Y
   LDY   #$2A
   LDA   (FP),Y
   LDY   #$12
   STA   (FP),Y
   LDY   #$2B
   LDA   (FP),Y
   LDY   #$13
   STA   (FP),Y
   LDY   #$2C
   LDA   (FP),Y
   LDY   #$14
   STA   (FP),Y
   LDY   #$2D
   LDA   (FP),Y
   LDY   #$15
   STA   (FP),Y
   INY
   LDA   (FP),Y
   LDY   #$2E
   STA   (FP),Y
   LDY   #$17
   LDA   (FP),Y
   LDY   #$2F
   STA   (FP),Y
   LDY   #$16
   LDA   (FP),Y
   CLC
   ADC   #$01
   LDY   #$30
   STA   (FP),Y
   LDY   #$17
   LDA   (FP),Y
   ADC   #$00
   LDY   #$31
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$16
   STA   (FP),Y
   LDY   #$31
   LDA   (FP),Y
   LDY   #$17
   STA   (FP),Y
   JMP   .loop@1_start
.loop@1_break:
   LDY   #$12
   LDA   (FP),Y
   LDY   #$32
   STA   (FP),Y
   LDY   #$13
   LDA   (FP),Y
   LDY   #$33
   STA   (FP),Y
   LDY   #$14
   LDA   (FP),Y
   LDY   #$34
   STA   (FP),Y
   LDY   #$15
   LDA   (FP),Y
   LDY   #$35
   STA   (FP),Y
   LDA   g1
   CLC
   LDY   #$32
   ADC   (FP),Y
   LDY   #$36
   STA   (FP),Y
   LDA   g1+1
   LDY   #$33
   ADC   (FP),Y
   LDY   #$37
   STA   (FP),Y
   LDA   g1+2
   LDY   #$34
   ADC   (FP),Y
   LDY   #$38
   STA   (FP),Y
   LDA   g1+3
   LDY   #$35
   ADC   (FP),Y
   LDY   #$39
   STA   (FP),Y
   LDY   #$36
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
.loop@2_start:
   LDY   #$3A
   LDA   (FP),Y
   SEC
   SBC   #$03
   INY
   LDA   (FP),Y
   SBC   #$00
   BVC   .cmp_novf@73
   EOR   #$80
.cmp_novf@73:
   ASL
   LDA   #$00
   ROL
   LDY   #$3C
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@1
   JMP   .loop@2_break
.lb_skip@1:
   LDA   g1
   ORA   g1+1
   ORA   g1+2
   ORA   g1+3
   BNE   .or_true@2
   LDA   g1
   ORA   g1+1
   ORA   g1+2
   ORA   g1+3
   BNE   .or_true@2
   LDA   #$00
   LDY   #$3E
   STA   (FP),Y
   INY
   STA   (FP),Y
   JMP   .or_end@3
.or_true@2:
   LDA   #$01
   LDY   #$3E
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@3:
   LDY   #$3E
   LDA   (FP),Y
   LDY   #$40
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
   JSR   f4
   LDA   HARGS+8
   LDY   #$41
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
   LDY   #$3A
   LDA   (FP),Y
   LDY   #$45
   STA   (FP),Y
   LDY   #$3B
   LDA   (FP),Y
   LDY   #$46
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@74
   LDA   #$00
   JMP   .sx_done@75
.sx_neg@74:
   LDA   #$FF
.sx_done@75:
   LDY   #$47
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$41
   LDA   (FP),Y
   CLC
   LDY   #$45
   ADC   (FP),Y
   LDY   #$49
   STA   (FP),Y
   LDY   #$42
   LDA   (FP),Y
   LDY   #$46
   ADC   (FP),Y
   LDY   #$4A
   STA   (FP),Y
   LDY   #$43
   LDA   (FP),Y
   LDY   #$47
   ADC   (FP),Y
   LDY   #$4B
   STA   (FP),Y
   LDY   #$44
   LDA   (FP),Y
   LDY   #$48
   ADC   (FP),Y
   LDY   #$4C
   STA   (FP),Y
   LDY   #$12
   LDA   (FP),Y
   CLC
   LDY   #$49
   ADC   (FP),Y
   LDY   #$4D
   STA   (FP),Y
   LDY   #$13
   LDA   (FP),Y
   LDY   #$4A
   ADC   (FP),Y
   LDY   #$4E
   STA   (FP),Y
   LDY   #$14
   LDA   (FP),Y
   LDY   #$4B
   ADC   (FP),Y
   LDY   #$4F
   STA   (FP),Y
   LDY   #$15
   LDA   (FP),Y
   LDY   #$4C
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
   AND   #$03
   LDY   #$54
   STA   (FP),Y
   LDY   #$51
   LDA   (FP),Y
   LDY   #$12
   STA   (FP),Y
   LDY   #$52
   LDA   (FP),Y
   LDY   #$13
   STA   (FP),Y
   LDY   #$53
   LDA   (FP),Y
   LDY   #$14
   STA   (FP),Y
   LDY   #$54
   LDA   (FP),Y
   LDY   #$15
   STA   (FP),Y
   LDY   #$3A
   LDA   (FP),Y
   LDY   #$55
   STA   (FP),Y
   LDY   #$3B
   LDA   (FP),Y
   LDY   #$56
   STA   (FP),Y
   LDY   #$3A
   LDA   (FP),Y
   CLC
   ADC   #$01
   LDY   #$57
   STA   (FP),Y
   LDY   #$3B
   LDA   (FP),Y
   ADC   #$00
   LDY   #$58
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$3A
   STA   (FP),Y
   LDY   #$58
   LDA   (FP),Y
   LDY   #$3B
   STA   (FP),Y
   JMP   .loop@2_start
.loop@2_break:
   LDA   g2
   LDY   #$59
   STA   (FP),Y
   LDA   g2+1
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   g1
   LDY   #$59
   AND   (FP),Y
   LDY   #$5D
   STA   (FP),Y
   LDA   g1+1
   LDY   #$5A
   AND   (FP),Y
   LDY   #$5E
   STA   (FP),Y
   LDA   g1+2
   LDY   #$5B
   AND   (FP),Y
   LDY   #$5F
   STA   (FP),Y
   LDA   g1+3
   LDY   #$5C
   AND   (FP),Y
   LDY   #$60
   STA   (FP),Y
   LDA   g2
   INY
   STA   (FP),Y
   LDA   g2+1
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$61
   LDA   (FP),Y
   LDY   #$12
   EOR   (FP),Y
   LDY   #$65
   STA   (FP),Y
   LDY   #$62
   LDA   (FP),Y
   LDY   #$13
   EOR   (FP),Y
   LDY   #$66
   STA   (FP),Y
   LDY   #$63
   LDA   (FP),Y
   LDY   #$14
   EOR   (FP),Y
   LDY   #$67
   STA   (FP),Y
   LDY   #$64
   LDA   (FP),Y
   LDY   #$15
   EOR   (FP),Y
   LDY   #$68
   STA   (FP),Y
   LDY   #$65
   LDA   (FP),Y
   LDY   #$69
   STA   (FP),Y
   LDY   #$66
   LDA   (FP),Y
   LDY   #$6A
   STA   (FP),Y
   LDY   #$67
   LDA   (FP),Y
   LDY   #$6B
   STA   (FP),Y
   LDY   #$68
   LDA   (FP),Y
   LDY   #$6C
   STA   (FP),Y
   LDY   #$5D
   LDA   (FP),Y
   LDY   #$69
   ORA   (FP),Y
   LDY   #$6D
   STA   (FP),Y
   LDY   #$5E
   LDA   (FP),Y
   LDY   #$6A
   ORA   (FP),Y
   LDY   #$6E
   STA   (FP),Y
   LDY   #$5F
   LDA   (FP),Y
   LDY   #$6B
   ORA   (FP),Y
   LDY   #$6F
   STA   (FP),Y
   LDY   #$60
   LDA   (FP),Y
   LDY   #$6C
   ORA   (FP),Y
   LDY   #$70
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   ORA   #$00
   BPL   .sx_done@77
   LDA   #$FF
.sx_done@77:
   LDY   #$73
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$70
   LDA   (FP),Y
   LDY   #$74
   CMP   (FP),Y
   BNE   .cmp_true@78
   LDY   #$6F
   LDA   (FP),Y
   LDY   #$73
   CMP   (FP),Y
   BNE   .cmp_true@78
   LDY   #$6E
   LDA   (FP),Y
   LDY   #$72
   CMP   (FP),Y
   BNE   .cmp_true@78
   LDY   #$6D
   LDA   (FP),Y
   LDY   #$71
   CMP   (FP),Y
   BNE   .cmp_true@78
   LDA   #$00
   JMP   .cmp_end@79
.cmp_true@78:
   LDA   #$01
.cmp_end@79:
   LDY   #$75
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$77
   STA   (FP),Y
   CMP   #$01

   ; epilogue
   PHP
   CLC
   LDA   FP
   ADC   #$79
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
   LDA   g0
   BMI   .sx_neg@81
   LDA   #$00
   JMP   .sx_done@82
.sx_neg@81:
   LDA   #$FF
.sx_done@82:
   LDY   #$0B
   STA   (FP),Y
   INY
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
   LDA   g2+1
   INY
   STA   (FP),Y
   LDA   #$00
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
   DC.B  $3E

g1:
   DC.L  $000000AA

g2:
   DC.W  $0020
