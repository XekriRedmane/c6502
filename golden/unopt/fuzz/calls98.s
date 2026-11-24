SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24

f4:
   SUBROUTINE

   ; prologue: 4 arg bytes, 31 local bytes
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

   LDA   g2
   ORA   g2+1
   ORA   g2+2
   ORA   g2+3
   ORA   g2+4
   ORA   g2+5
   ORA   g2+6
   BNE   .or_true@0
   INY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@0
   LDA   #$00
   LDY   #$01
   STA   (FP),Y
   INY
   STA   (FP),Y
   JMP   .or_end@1
.or_true@0:
   LDA   #$01
   LDY   #$01
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@1:
   LDY   #$24
   LDA   (FP),Y
   AND   #$07
   LDY   #$03
   STA   (FP),Y
   LDY   #$25
   LDA   (FP),Y
   LDA   #$00
   LDY   #$04
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
   TAX
   BEQ   .shift_done@1
.shift_loop@0:
   LDY   #$05
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
   LDY   #$05
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
   INY
   STA   (FP),Y
   LDA   g0
   INY
   STA   (FP),Y
   LDA   g0
   BMI   .sx_neg@2
   LDA   #$00
   JMP   .sx_done@3
.sx_neg@2:
   LDA   #$FF
.sx_done@3:
   LDY   #$0C
   STA   (FP),Y
   LDA   g1
   AND   #$07
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
   INY
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
   TAX
   BEQ   .shift_done@5
.shift_loop@4:
   LDY   #$13
   LDA   (FP),Y
   ASL
   STA   (FP),Y
   INY
   LDA   (FP),Y
   ROL
   STA   (FP),Y
   DEX
   BNE   .shift_loop@4
.shift_done@5:
   LDY   #$13
   LDA   (FP),Y
   LDY   #$15
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$16
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@6
   LDA   #$00
   JMP   .sx_done@7
.sx_neg@6:
   LDA   #$FF
.sx_done@7:
   LDY   #$17
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   ORA   #$00
   BPL   .sx_done@9
   LDA   #$FF
.sx_done@9:
   LDY   #$1B
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$18
   LDA   (FP),Y
   LDY   #$1C
   CMP   (FP),Y
   BNE   .cmp_true@10
   LDY   #$17
   LDA   (FP),Y
   LDY   #$1B
   CMP   (FP),Y
   BNE   .cmp_true@10
   LDY   #$16
   LDA   (FP),Y
   LDY   #$1A
   CMP   (FP),Y
   BNE   .cmp_true@10
   LDY   #$15
   LDA   (FP),Y
   LDY   #$19
   CMP   (FP),Y
   BNE   .cmp_true@10
   LDA   #$00
   JMP   .cmp_end@11
.cmp_true@10:
   LDA   #$01
.cmp_end@11:
   LDY   #$1D
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$1F
   STA   (FP),Y
   CMP   #$01

   ; epilogue
   PHP
   CLC
   LDA   FP
   ADC   #$25
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

f3:
   SUBROUTINE

   ; prologue: 2 arg bytes, 31 local bytes
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

   LDA   #$FF
   SEC
   SBC   #$03
   LDA   #$00
   SBC   #$00
   BVC   .cmp_novf@13
   EOR   #$80
.cmp_novf@13:
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
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$04
   LDA   (FP),Y
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
   LDA   g0
   INY
   STA   (FP),Y
   LDA   g0
   BMI   .sx_neg@14
   LDA   #$00
   JMP   .sx_done@15
.sx_neg@14:
   LDA   #$FF
.sx_done@15:
   LDY   #$0D
   STA   (FP),Y
   CMP   #$03
   BNE   .thread@0
   DEY
   LDA   (FP),Y
   CMP   #$E8
   BEQ   .cmp_true@16
.thread@0:
   LDA   #$00
   JMP   .cmp_end@17
.cmp_true@16:
   LDA   #$01
.cmp_end@17:
   LDY   #$0E
   STA   (FP),Y
   LDA   #$00
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
   BMI   .sx_neg@19
   LDA   #$00
   JMP   .sx_done@20
.sx_neg@19:
   LDA   #$FF
.sx_done@20:
   LDY   #$12
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$10
   LDA   (FP),Y
   SEC
   SBC   g2
   INY
   LDA   (FP),Y
   SBC   g2+1
   INY
   LDA   (FP),Y
   SBC   g2+2
   INY
   LDA   (FP),Y
   SBC   g2+3
   INY
   LDA   (FP),Y
   SBC   g2+4
   INY
   LDA   (FP),Y
   SBC   g2+5
   INY
   LDA   (FP),Y
   SBC   g2+6
   LDA   #$00
   ROL
   INY
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
   BMI   .sx_neg@21
   LDA   #$00
   JMP   .sx_done@22
.sx_neg@21:
   LDA   #$FF
.sx_done@22:
   LDY   #$1B
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$19
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
   RTS

f2:
   SUBROUTINE

   ; prologue: 9 arg bytes, 121 local bytes
   SEC
   LDA   SSP
   SBC   #$7B
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$7A
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
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   g0
   BNE   .lb_skip@0
   JMP   .and_false@2
.lb_skip@0:
   LDY   #$84
   LDA   (FP),Y
   LDY   #$07
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@23
   LDA   #$00
   JMP   .sx_done@24
.sx_neg@23:
   LDA   #$FF
.sx_done@24:
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
   ORA   #$00
   BMI   .sx_neg@25
   LDA   #$00
   JMP   .sx_done@26
.sx_neg@25:
   LDA   #$FF
.sx_done@26:
   LDY   #$0B
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   g1+5
   CMP   (FP),Y
   BNE   .cmp_true@27
   LDA   g1+4
   DEY
   CMP   (FP),Y
   BNE   .cmp_true@27
   LDA   g1+3
   DEY
   CMP   (FP),Y
   BNE   .cmp_true@27
   LDA   g1+2
   DEY
   CMP   (FP),Y
   BNE   .cmp_true@27
   LDA   g1+1
   DEY
   CMP   (FP),Y
   BNE   .cmp_true@27
   LDA   g1
   DEY
   CMP   (FP),Y
   BNE   .cmp_true@27
   LDA   #$00
   JMP   .cmp_end@28
.cmp_true@27:
   LDA   #$01
.cmp_end@28:
   LDY   #$0F
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
   LDY   #$11
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@3:
   LDY   #$11
   LDA   (FP),Y
   LDY   #$13
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$14
   STA   (FP),Y
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
   BMI   .sx_neg@32
   LDA   #$00
   JMP   .sx_done@33
.sx_neg@32:
   LDA   #$FF
.sx_done@33:
   LDY   #$19
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$17
   LDA   (FP),Y
   SEC
   LDY   #$7C
   SBC   (FP),Y
   LDY   #$18
   LDA   (FP),Y
   LDY   #$7D
   SBC   (FP),Y
   LDY   #$19
   LDA   (FP),Y
   LDY   #$7E
   SBC   (FP),Y
   LDY   #$1A
   LDA   (FP),Y
   LDY   #$7F
   SBC   (FP),Y
   BVC   .cmp_novf@34
   EOR   #$80
.cmp_novf@34:
   ASL
   LDA   #$00
   ROL
   LDY   #$1B
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$80
   LDA   (FP),Y
   LDY   #$1D
   STA   (FP),Y
   INY
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@35
   LDA   #$00
   JMP   .sx_done@36
.sx_neg@35:
   LDA   #$FF
.sx_done@36:
   LDY   #$1F
   STA   (FP),Y
   LDY   #$1B
   LDA   (FP),Y
   LDY   #$1E
   AND   (FP),Y
   LDY   #$20
   STA   (FP),Y
   LDY   #$1C
   LDA   (FP),Y
   LDY   #$1F
   AND   (FP),Y
   LDY   #$21
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
   BMI   .sx_neg@37
   LDA   #$00
   JMP   .sx_done@38
.sx_neg@37:
   LDA   #$FF
.sx_done@38:
   LDY   #$24
   STA   (FP),Y
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
   INY
   STA   (FP),Y
   LDA   g0
   INY
   STA   (FP),Y
   LDA   g0
   BMI   .sx_neg@39
   LDA   #$00
   JMP   .sx_done@40
.sx_neg@39:
   LDA   #$FF
.sx_done@40:
   LDY   #$2B
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$2C
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$2D
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@41
   LDA   #$00
   JMP   .sx_done@42
.sx_neg@41:
   LDA   #$FF
.sx_done@42:
   LDY   #$2E
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$7F
   LDA   (FP),Y
   LDY   #$2F
   CMP   (FP),Y
   BNE   .thread@1
   LDY   #$7E
   LDA   (FP),Y
   LDY   #$2E
   CMP   (FP),Y
   BNE   .thread@1
   LDY   #$7D
   LDA   (FP),Y
   LDY   #$2D
   CMP   (FP),Y
   BNE   .thread@1
   LDY   #$7C
   LDA   (FP),Y
   LDY   #$2C
   CMP   (FP),Y
   BEQ   .cmp_true@43
.thread@1:
   LDA   #$00
   JMP   .cmp_end@44
.cmp_true@43:
   LDA   #$01
.cmp_end@44:
   LDY   #$30
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$32
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$33
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@46
   LDA   #$00
   JMP   .sx_done@47
.sx_neg@46:
   LDA   #$FF
.sx_done@47:
   LDY   #$34
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$32
   LDA   (FP),Y
   LDY   #$36
   STA   (FP),Y
   LDY   #$33
   LDA   (FP),Y
   LDY   #$37
   STA   (FP),Y
   LDY   #$34
   LDA   (FP),Y
   LDY   #$38
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$84
   LDA   (FP),Y
   LDY   #$3A
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@48
   LDA   #$00
   JMP   .sx_done@49
.sx_neg@48:
   LDA   #$FF
.sx_done@49:
   LDY   #$3B
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ORA   #$01
   LDY   #$3C
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$3D
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
   BMI   .sx_neg@50
   LDA   #$00
   JMP   .sx_done@51
.sx_neg@50:
   LDA   #$FF
.sx_done@51:
   LDY   #$40
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$7C
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
   LDY   #$3E
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
   LDY   #$42
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
   LDY   #$42
   LDA   (FP),Y
   LDY   #$46
   STA   (FP),Y
   LDY   #$43
   LDA   (FP),Y
   LDY   #$47
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
   LDY   #$47
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   JSR   f3
   LDA   HARGS+16
   LDY   #$48
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
   LDY   #$36
   LDA   (FP),Y
   LDY   #$4F
   STA   (FP),Y
   LDY   #$37
   LDA   (FP),Y
   LDY   #$50
   STA   (FP),Y
   LDY   #$38
   LDA   (FP),Y
   LDY   #$51
   STA   (FP),Y
   LDY   #$39
   LDA   (FP),Y
   LDY   #$52
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$4F
   LDA   (FP),Y
   LDY   #$48
   EOR   (FP),Y
   LDY   #$56
   STA   (FP),Y
   LDY   #$50
   LDA   (FP),Y
   LDY   #$49
   EOR   (FP),Y
   LDY   #$57
   STA   (FP),Y
   LDY   #$51
   LDA   (FP),Y
   LDY   #$4A
   EOR   (FP),Y
   LDY   #$58
   STA   (FP),Y
   LDY   #$52
   LDA   (FP),Y
   LDY   #$4B
   EOR   (FP),Y
   LDY   #$59
   STA   (FP),Y
   LDY   #$53
   LDA   (FP),Y
   LDY   #$4C
   EOR   (FP),Y
   LDY   #$5A
   STA   (FP),Y
   LDY   #$54
   LDA   (FP),Y
   LDY   #$4D
   EOR   (FP),Y
   LDY   #$5B
   STA   (FP),Y
   LDY   #$55
   LDA   (FP),Y
   LDY   #$4E
   EOR   (FP),Y
   LDY   #$5C
   STA   (FP),Y
   LDY   #$56
   LDA   (FP),Y
   LDY   #$5D
   STA   (FP),Y
   LDY   #$57
   LDA   (FP),Y
   LDY   #$5E
   STA   (FP),Y
   LDY   #$58
   LDA   (FP),Y
   LDY   #$5F
   STA   (FP),Y
   LDY   #$59
   LDA   (FP),Y
   LDY   #$60
   STA   (FP),Y
   LDY   #$5D
   LDA   (FP),Y
   LDY   #$36
   STA   (FP),Y
   LDY   #$5E
   LDA   (FP),Y
   LDY   #$37
   STA   (FP),Y
   LDY   #$5F
   LDA   (FP),Y
   LDY   #$38
   STA   (FP),Y
   LDY   #$60
   LDA   (FP),Y
   LDY   #$39
   STA   (FP),Y
   LDY   #$13
   LDA   (FP),Y
   LDY   #$61
   STA   (FP),Y
   LDY   #$14
   LDA   (FP),Y
   LDY   #$62
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@52
   LDA   #$00
   JMP   .sx_done@53
.sx_neg@52:
   LDA   #$FF
.sx_done@53:
   LDY   #$63
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
   BPL   .sx_done@55
   LDA   #$FF
.sx_done@55:
   LDY   #$67
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$7C
   LDA   (FP),Y
   LDY   #$65
   ORA   (FP),Y
   LDY   #$69
   STA   (FP),Y
   LDY   #$7D
   LDA   (FP),Y
   LDY   #$66
   ORA   (FP),Y
   LDY   #$6A
   STA   (FP),Y
   LDY   #$7E
   LDA   (FP),Y
   LDY   #$67
   ORA   (FP),Y
   LDY   #$6B
   STA   (FP),Y
   LDY   #$7F
   LDA   (FP),Y
   LDY   #$68
   ORA   (FP),Y
   LDY   #$6C
   STA   (FP),Y
   LDY   #$61
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
   LDY   #$69
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
   LDY   #$6D
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
   LDY   #$6D
   LDA   (FP),Y
   LDY   #$71
   STA   (FP),Y
   LDY   #$6E
   LDA   (FP),Y
   LDY   #$72
   STA   (FP),Y
   LDY   #$6F
   LDA   (FP),Y
   LDY   #$73
   STA   (FP),Y
   LDY   #$70
   LDA   (FP),Y
   LDY   #$74
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@56
   LDA   #$00
   JMP   .sx_done@57
.sx_neg@56:
   LDA   #$FF
.sx_done@57:
   LDY   #$75
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$01
   LDA   (FP),Y
   SEC
   LDY   #$71
   SBC   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   LDY   #$72
   SBC   (FP),Y
   LDY   #$03
   LDA   (FP),Y
   LDY   #$73
   SBC   (FP),Y
   LDY   #$04
   LDA   (FP),Y
   LDY   #$74
   SBC   (FP),Y
   LDY   #$05
   LDA   (FP),Y
   LDY   #$75
   SBC   (FP),Y
   LDY   #$06
   LDA   (FP),Y
   LDY   #$76
   SBC   (FP),Y
   BVC   .cmp_novf@58
   EOR   #$80
.cmp_novf@58:
   ASL
   LDA   #$00
   ROL
   LDY   #$77
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$79
   STA   (FP),Y
   CMP   #$01

   ; epilogue
   PHP
   CLC
   LDA   FP
   ADC   #$84
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

f1:
   SUBROUTINE

   ; prologue: 0 arg bytes, 107 local bytes
   SEC
   LDA   SSP
   SBC   #$6D
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$6C
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
   BPL   .sx_done@60
   LDA   #$FF
.sx_done@60:
   LDY   #$03
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$01
   LDA   (FP),Y
   LDY   #$05
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$03
   LDA   (FP),Y
   LDY   #$07
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$05
   LDA   (FP),Y
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
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   g2
   ORA   #$01
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
   LDA   g2+4
   INY
   STA   (FP),Y
   LDA   g2+5
   INY
   STA   (FP),Y
   LDA   g2+6
   INY
   STA   (FP),Y
   LDY   #$09
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
   LDA   #$00
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
   INY
   LDA   (FP),Y
   STA   HARGS+14
   LDA   #$00
   STA   HARGS+15
   JSR   udivmod64
   LDA   HARGS+16
   LDY   #$17
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
   LDA   g0
   BEQ   .lnot_true@61
   LDA   #$00
   JMP   .lnot_end@62
.lnot_true@61:
   LDA   #$01
.lnot_end@62:
   LDY   #$1E
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ORA   #$01
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
   DEY
   LDA   (FP),Y
   LDY   #$23
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@63
   LDA   #$00
   JMP   .sx_done@64
.sx_neg@63:
   LDA   #$FF
.sx_done@64:
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
   LDY   #$17
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
   LDA   #$00
   STA   HARGS+7
   LDY   #$22
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
   LDA   #$00
   STA   HARGS+15
   JSR   udivmod64
   LDA   HARGS+24
   LDY   #$29
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
   LDA   HARGS+30
   INY
   STA   (FP),Y
   LDY   #$29
   LDA   (FP),Y
   LDY   #$30
   STA   (FP),Y
   LDY   #$2A
   LDA   (FP),Y
   LDY   #$31
   STA   (FP),Y
   LDY   #$2B
   LDA   (FP),Y
   LDY   #$32
   STA   (FP),Y
   LDY   #$2C
   LDA   (FP),Y
   LDY   #$33
   STA   (FP),Y
   LDY   #$2D
   LDA   (FP),Y
   LDY   #$34
   STA   (FP),Y
   LDY   #$2E
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
   AND   #$0F
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
   LDY   #$3E
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
   EOR   #$08
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
   LDY   #$45
   STA   (FP),Y
   LDY   #$40
   LDA   (FP),Y
   LDY   #$46
   STA   (FP),Y
   LDY   #$41
   LDA   (FP),Y
   SEC
   SBC   #$08
   LDY   #$47
   STA   (FP),Y
   LDY   #$42
   LDA   (FP),Y
   LDY   #$48
   STA   (FP),Y
   LDY   #$43
   LDA   (FP),Y
   LDY   #$49
   STA   (FP),Y
   LDY   #$44
   LDA   (FP),Y
   LDY   #$4A
   STA   (FP),Y
   LDY   #$45
   LDA   (FP),Y
   LDY   #$4B
   STA   (FP),Y
   LDY   #$46
   LDA   (FP),Y
   LDY   #$4C
   STA   (FP),Y
   LDY   #$47
   LDA   (FP),Y
   LDY   #$4D
   STA   (FP),Y
   LDY   #$48
   LDA   (FP),Y
   LDY   #$4E
   STA   (FP),Y
   LDY   #$49
   LDA   (FP),Y
   LDY   #$4F
   STA   (FP),Y
   LDY   #$4A
   LDA   (FP),Y
   LDY   #$50
   STA   (FP),Y
   LDY   #$4B
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
   LDY   #$54
   STA   (FP),Y
   LDY   #$51
   LDA   (FP),Y
   LDY   #$55
   STA   (FP),Y
   LDY   #$05
   LDA   (FP),Y
   LDY   #$56
   STA   (FP),Y
   LDY   #$06
   LDA   (FP),Y
   LDY   #$57
   STA   (FP),Y
   LDY   #$07
   LDA   (FP),Y
   LDY   #$58
   STA   (FP),Y
   LDY   #$08
   LDA   (FP),Y
   LDY   #$59
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$56
   LDA   (FP),Y
   SEC
   LDY   #$48
   SBC   (FP),Y
   LDY   #$57
   LDA   (FP),Y
   LDY   #$49
   SBC   (FP),Y
   LDY   #$58
   LDA   (FP),Y
   LDY   #$4A
   SBC   (FP),Y
   LDY   #$59
   LDA   (FP),Y
   LDY   #$4B
   SBC   (FP),Y
   LDY   #$5A
   LDA   (FP),Y
   LDY   #$4C
   SBC   (FP),Y
   LDY   #$5B
   LDA   (FP),Y
   LDY   #$4D
   SBC   (FP),Y
   BVC   .cmp_novf@65
   EOR   #$80
.cmp_novf@65:
   ASL
   LDA   #$00
   ROL
   EOR   #$01
   LDY   #$5C
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
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
   LDA   g0
   INY
   STA   (FP),Y
   LDA   g0
   BMI   .sx_neg@66
   LDA   #$00
   JMP   .sx_done@67
.sx_neg@66:
   LDA   #$FF
.sx_done@67:
   LDY   #$63
   STA   (FP),Y
   LDA   g0
   INY
   STA   (FP),Y
   LDA   g0
   BMI   .sx_neg@68
   LDA   #$00
   JMP   .sx_done@69
.sx_neg@68:
   LDA   #$FF
.sx_done@69:
   LDY   #$65
   STA   (FP),Y
   LDY   #$62
   LDA   (FP),Y
   SEC
   LDY   #$64
   SBC   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$65
   SBC   (FP),Y
   BVC   .cmp_novf@70
   EOR   #$80
.cmp_novf@70:
   ASL
   LDA   #$00
   ROL
   EOR   #$01
   LDY   #$66
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$68
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$69
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@71
   LDA   #$00
   JMP   .sx_done@72
.sx_neg@71:
   LDA   #$FF
.sx_done@72:
   LDY   #$6A
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$68
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
   ADC   #$6D
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

   ; prologue: 9 arg bytes, 151 local bytes
   SEC
   LDA   SSP
   SBC   #$99
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$98
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
   BMI   .sx_neg@73
   LDA   #$00
   JMP   .sx_done@74
.sx_neg@73:
   LDA   #$FF
.sx_done@74:
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
   LDA   g1
   INY
   STA   (FP),Y
   LDA   g1+1
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
   LDY   #$9A
   LDA   (FP),Y
   LDY   #$0D
   STA   (FP),Y
   LDY   #$9B
   LDA   (FP),Y
   LDY   #$0E
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
   LDY   #$0D
   LDA   (FP),Y
   SEC
   SBC   g2
   LDY   #$14
   STA   (FP),Y
   LDY   #$0E
   LDA   (FP),Y
   SBC   g2+1
   LDY   #$15
   STA   (FP),Y
   LDY   #$0F
   LDA   (FP),Y
   SBC   g2+2
   LDY   #$16
   STA   (FP),Y
   LDY   #$10
   LDA   (FP),Y
   SBC   g2+3
   LDY   #$17
   STA   (FP),Y
   LDY   #$11
   LDA   (FP),Y
   SBC   g2+4
   LDY   #$18
   STA   (FP),Y
   LDY   #$12
   LDA   (FP),Y
   SBC   g2+5
   LDY   #$19
   STA   (FP),Y
   LDY   #$13
   LDA   (FP),Y
   SBC   g2+6
   LDY   #$1A
   STA   (FP),Y
   LDY   #$0B
   LDA   (FP),Y
   LDY   #$1B
   STA   (FP),Y
   LDY   #$0C
   LDA   (FP),Y
   LDY   #$1C
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
   LDY   #$14
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
   LDA   #$00
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
   INY
   LDA   (FP),Y
   STA   HARGS+14
   LDA   #$00
   STA   HARGS+15
   JSR   mul64
   LDA   HARGS+16
   LDY   #$22
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
   LDY   #$22
   LDA   (FP),Y
   LDY   #$29
   STA   (FP),Y
   LDY   #$23
   LDA   (FP),Y
   LDY   #$2A
   STA   (FP),Y
   LDY   #$24
   LDA   (FP),Y
   LDY   #$2B
   STA   (FP),Y
   LDY   #$25
   LDA   (FP),Y
   LDY   #$2C
   STA   (FP),Y
   LDY   #$29
   LDA   (FP),Y
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
   LDY   #$2C
   LDA   (FP),Y
   LDY   #$30
   STA   (FP),Y
   LDA   g0
   INY
   STA   (FP),Y
   LDA   g0
   BMI   .sx_neg@75
   LDA   #$00
   JMP   .sx_done@76
.sx_neg@75:
   LDA   #$FF
.sx_done@76:
   LDY   #$32
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
   BMI   .sx_neg@77
   LDA   #$00
   JMP   .sx_done@78
.sx_neg@77:
   LDA   #$FF
.sx_done@78:
   LDY   #$35
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$33
   LDA   (FP),Y
   CLC
   LDY   #$2D
   SBC   (FP),Y
   LDY   #$34
   LDA   (FP),Y
   LDY   #$2E
   SBC   (FP),Y
   LDY   #$35
   LDA   (FP),Y
   LDY   #$2F
   SBC   (FP),Y
   LDY   #$36
   LDA   (FP),Y
   LDY   #$30
   SBC   (FP),Y
   LDA   #$00
   ROL
   LDY   #$37
   STA   (FP),Y
   LDA   #$00
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
   ORA   #$00
   BMI   .sx_neg@79
   LDA   #$00
   JMP   .sx_done@80
.sx_neg@79:
   LDA   #$FF
.sx_done@80:
   LDY   #$3B
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$05
   LDA   (FP),Y
   LDY   #$3D
   STA   (FP),Y
   LDY   #$06
   LDA   (FP),Y
   LDY   #$3E
   STA   (FP),Y
   LDY   #$07
   LDA   (FP),Y
   LDY   #$3F
   STA   (FP),Y
   LDY   #$08
   LDA   (FP),Y
   LDY   #$40
   STA   (FP),Y
   LDY   #$3D
   LDA   (FP),Y
   SEC
   LDY   #$39
   SBC   (FP),Y
   LDY   #$3E
   LDA   (FP),Y
   LDY   #$3A
   SBC   (FP),Y
   LDY   #$3F
   LDA   (FP),Y
   LDY   #$3B
   SBC   (FP),Y
   LDY   #$40
   LDA   (FP),Y
   LDY   #$3C
   SBC   (FP),Y
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
   DEY
   LDA   (FP),Y
   LDY   #$44
   STA   (FP),Y
   LDA   #$03
   SEC
   LDY   #$0B
   SBC   (FP),Y
   LDY   #$45
   STA   (FP),Y
   LDA   #$00
   LDY   #$0C
   SBC   (FP),Y
   LDY   #$46
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$47
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$48
   STA   (FP),Y
   LDA   #$00
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
   LDY   #$4C
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$4D
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$4E
   STA   (FP),Y
   LDY   #$9A
   LDA   (FP),Y
   ORA   #$01
   LDY   #$4F
   STA   (FP),Y
   LDY   #$9B
   LDA   (FP),Y
   LDY   #$50
   STA   (FP),Y
   LDY   #$4D
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
   LDY   #$51
   STA   (FP),Y
   LDA   HARGS+7
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
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$03
   LDY   #$43
   ORA   (FP),Y
   LDY   #$57
   STA   (FP),Y
   LDY   #$44
   LDA   (FP),Y
   LDY   #$58
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$59
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$09
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDY   #$47
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$48
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$49
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$4A
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   LDY   #$53
   LDA   (FP),Y
   LDY   #$05
   STA   (SSP),Y
   LDY   #$54
   LDA   (FP),Y
   LDY   #$06
   STA   (SSP),Y
   LDY   #$55
   LDA   (FP),Y
   LDY   #$07
   STA   (SSP),Y
   LDY   #$56
   LDA   (FP),Y
   LDY   #$08
   STA   (SSP),Y
   LDY   #$59
   LDA   (FP),Y
   LDY   #$09
   STA   (SSP),Y
   JSR   f2
   LDA   #$00
   ROL
   LDY   #$5A
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$43
   LDA   (FP),Y
   LDY   #$5B
   EOR   (FP),Y
   LDY   #$5D
   STA   (FP),Y
   LDY   #$44
   LDA   (FP),Y
   LDY   #$5C
   EOR   (FP),Y
   LDY   #$5E
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$43
   STA   (FP),Y
   LDY   #$5E
   LDA   (FP),Y
   LDY   #$44
   STA   (FP),Y
   JSR   f1
   LDA   HARGS+8
   LDY   #$5F
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
   LDY   #$05
   LDA   (FP),Y
   LDY   #$63
   STA   (FP),Y
   LDY   #$06
   LDA   (FP),Y
   LDY   #$64
   STA   (FP),Y
   LDY   #$07
   LDA   (FP),Y
   LDY   #$65
   STA   (FP),Y
   LDY   #$08
   LDA   (FP),Y
   LDY   #$66
   STA   (FP),Y
   LDY   #$63
   LDA   (FP),Y
   LDY   #$5F
   EOR   (FP),Y
   LDY   #$67
   STA   (FP),Y
   LDY   #$64
   LDA   (FP),Y
   LDY   #$60
   EOR   (FP),Y
   LDY   #$68
   STA   (FP),Y
   LDY   #$65
   LDA   (FP),Y
   LDY   #$61
   EOR   (FP),Y
   LDY   #$69
   STA   (FP),Y
   LDY   #$66
   LDA   (FP),Y
   LDY   #$62
   EOR   (FP),Y
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
   LDY   #$69
   LDA   (FP),Y
   LDY   #$6D
   STA   (FP),Y
   LDY   #$6A
   LDA   (FP),Y
   LDY   #$6E
   STA   (FP),Y
   LDY   #$6B
   LDA   (FP),Y
   LDY   #$05
   STA   (FP),Y
   LDY   #$6C
   LDA   (FP),Y
   LDY   #$06
   STA   (FP),Y
   LDY   #$6D
   LDA   (FP),Y
   LDY   #$07
   STA   (FP),Y
   LDY   #$6E
   LDA   (FP),Y
   LDY   #$08
   STA   (FP),Y
   LDY   #$05
   LDA   (FP),Y
   LDY   #$6F
   STA   (FP),Y
   LDY   #$06
   LDA   (FP),Y
   LDY   #$70
   STA   (FP),Y
   LDY   #$07
   LDA   (FP),Y
   LDY   #$71
   STA   (FP),Y
   LDY   #$08
   LDA   (FP),Y
   LDY   #$72
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@83
   LDA   #$00
   JMP   .sx_done@84
.sx_neg@83:
   LDA   #$FF
.sx_done@84:
   LDY   #$73
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   g1
   CLC
   LDY   #$6F
   ADC   (FP),Y
   LDY   #$75
   STA   (FP),Y
   LDA   g1+1
   LDY   #$70
   ADC   (FP),Y
   LDY   #$76
   STA   (FP),Y
   LDA   g1+2
   LDY   #$71
   ADC   (FP),Y
   LDY   #$77
   STA   (FP),Y
   LDA   g1+3
   LDY   #$72
   ADC   (FP),Y
   LDY   #$78
   STA   (FP),Y
   LDA   g1+4
   LDY   #$73
   ADC   (FP),Y
   LDY   #$79
   STA   (FP),Y
   LDA   g1+5
   LDY   #$74
   ADC   (FP),Y
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
   AND   #$0F
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
   LDY   #$83
   STA   (FP),Y
   LDY   #$7E
   LDA   (FP),Y
   LDY   #$84
   STA   (FP),Y
   LDY   #$7F
   LDA   (FP),Y
   LDY   #$85
   STA   (FP),Y
   LDY   #$80
   LDA   (FP),Y
   EOR   #$08
   LDY   #$86
   STA   (FP),Y
   LDY   #$81
   LDA   (FP),Y
   LDY   #$87
   STA   (FP),Y
   LDY   #$82
   LDA   (FP),Y
   LDY   #$88
   STA   (FP),Y
   LDY   #$83
   LDA   (FP),Y
   LDY   #$89
   STA   (FP),Y
   LDY   #$84
   LDA   (FP),Y
   LDY   #$8A
   STA   (FP),Y
   LDY   #$85
   LDA   (FP),Y
   LDY   #$8B
   STA   (FP),Y
   LDY   #$86
   LDA   (FP),Y
   SEC
   SBC   #$08
   LDY   #$8C
   STA   (FP),Y
   LDY   #$87
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
   INY
   LDA   (FP),Y
   STA   g1+4
   INY
   LDA   (FP),Y
   STA   g1+5
   LDY   #$A2
   LDA   (FP),Y
   LDY   #$8D
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@85
   LDA   #$00
   JMP   .sx_done@86
.sx_neg@85:
   LDA   #$FF
.sx_done@86:
   LDY   #$8E
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   AND   #$07
   LDY   #$8F
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDA   #$00
   LDY   #$90
   STA   (FP),Y
   LDA   #$03
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$8F
   LDA   (FP),Y
   TAX
   BEQ   .shift_done@88
.shift_loop@87:
   LDY   #$91
   LDA   (FP),Y
   ASL
   STA   (FP),Y
   INY
   LDA   (FP),Y
   ROL
   STA   (FP),Y
   DEX
   BNE   .shift_loop@87
.shift_done@88:
   LDA   g1
   ORA   g1+1
   ORA   g1+2
   ORA   g1+3
   ORA   g1+4
   ORA   g1+5
   BEQ   .and_false@4
   LDA   #$03
   BEQ   .and_false@4
   LDA   #$01
   LDY   #$93
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   JMP   .and_end@5
.and_false@4:
   LDA   #$00
   LDY   #$93
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@5:
   LDY   #$92
   LDA   (FP),Y
   LDY   #$94
   CMP   (FP),Y
   BNE   .thread@2
   LDY   #$91
   LDA   (FP),Y
   LDY   #$93
   CMP   (FP),Y
   BEQ   .cmp_true@89
.thread@2:
   LDA   #$00
   JMP   .cmp_end@90
.cmp_true@89:
   LDA   #$01
.cmp_end@90:
   LDY   #$95
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$97
   STA   (FP),Y

   ; epilogue
   PHA
   CLC
   LDA   FP
   ADC   #$A2
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

   ; prologue: 0 arg bytes, 44 local bytes
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

   LDA   #$18
   LDY   #$01
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$09
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   #$08
   STA   (SSP),Y
   LDA   #$01
   INY
   STA   (SSP),Y
   LDA   #$20
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
   LDY   #$01
   LDA   (FP),Y
   LDY   #$09
   STA   (SSP),Y
   JSR   f0
   LDY   #$02
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
   LDY   #$03
   LDA   (FP),Y
   LDY   #$07
   STA   (FP),Y
   LDY   #$04
   LDA   (FP),Y
   LDY   #$08
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   g0
   INY
   STA   (FP),Y
   LDA   g0
   BMI   .sx_neg@92
   LDA   #$00
   JMP   .sx_done@93
.sx_neg@92:
   LDA   #$FF
.sx_done@93:
   LDY   #$0C
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
   LDY   #$0E
   LDA   (FP),Y
   LDY   #$12
   EOR   (FP),Y
   LDY   #$16
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
   LDY   #$0A
   LDA   (FP),Y
   LDY   #$1E
   EOR   (FP),Y
   LDY   #$22
   STA   (FP),Y
   LDY   #$1F
   LDA   (FP),Y
   LDY   #$07
   STA   (FP),Y
   LDY   #$20
   LDA   (FP),Y
   LDY   #$08
   STA   (FP),Y
   LDY   #$21
   LDA   (FP),Y
   LDY   #$09
   STA   (FP),Y
   LDY   #$22
   LDA   (FP),Y
   LDY   #$0A
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
   LDA   #$10
   STA   HARGS+4
   JSR   asr32
   LDA   HARGS+5
   LDY   #$23
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
   LDY   #$07
   LDA   (FP),Y
   LDY   #$23
   EOR   (FP),Y
   LDY   #$27
   STA   (FP),Y
   LDY   #$08
   LDA   (FP),Y
   LDY   #$24
   EOR   (FP),Y
   LDY   #$28
   STA   (FP),Y
   LDY   #$09
   LDA   (FP),Y
   LDY   #$25
   EOR   (FP),Y
   LDY   #$29
   STA   (FP),Y
   LDY   #$0A
   LDA   (FP),Y
   LDY   #$26
   EOR   (FP),Y
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
   TAX
   DEY
   LDA   (FP),Y

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$2E
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$2D
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
   DC.B  $5A

g1:
   DC.B  $0F
   DS.B  5

g2:
   DC.B  $48
   DS.B  6
