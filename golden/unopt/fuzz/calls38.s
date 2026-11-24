SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24

f5:
   SUBROUTINE

   ; prologue: 7 arg bytes, 65 local bytes
   SEC
   LDA   SSP
   SBC   #$43
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$42
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
   ORA   #$00
   BMI   .sx_neg@0
   LDA   #$00
   JMP   .sx_done@1
.sx_neg@0:
   LDA   #$FF
.sx_done@1:
   LDY   #$02
   STA   (FP),Y
   LDA   g1
   INY
   STA   (FP),Y
   LDA   g1
   BMI   .sx_neg@2
   LDA   #$00
   JMP   .sx_done@3
.sx_neg@2:
   LDA   #$FF
.sx_done@3:
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
   DEY
   LDA   (FP),Y
   LDY   #$07
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$08
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@4
   LDA   #$00
   JMP   .sx_done@5
.sx_neg@4:
   LDA   #$FF
.sx_done@5:
   LDY   #$09
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   ORA   #$00
   BPL   .sx_done@7
   LDA   #$FF
.sx_done@7:
   LDY   #$0D
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   g0
   LDY   #$0B
   EOR   (FP),Y
   LDY   #$0F
   STA   (FP),Y
   LDA   g0+1
   LDY   #$0C
   EOR   (FP),Y
   LDY   #$10
   STA   (FP),Y
   LDA   g0+2
   LDY   #$0D
   EOR   (FP),Y
   LDY   #$11
   STA   (FP),Y
   LDA   g0+3
   LDY   #$0E
   EOR   (FP),Y
   LDY   #$12
   STA   (FP),Y
   LDY   #$0F
   LDA   (FP),Y
   SEC
   LDY   #$07
   SBC   (FP),Y
   LDY   #$10
   LDA   (FP),Y
   LDY   #$08
   SBC   (FP),Y
   LDY   #$11
   LDA   (FP),Y
   LDY   #$09
   SBC   (FP),Y
   LDY   #$12
   LDA   (FP),Y
   LDY   #$0A
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
   BMI   .sx_neg@8
   LDA   #$00
   JMP   .sx_done@9
.sx_neg@8:
   LDA   #$FF
.sx_done@9:
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
   LDY   #$44
   LDA   (FP),Y
   LDY   #$1D
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@10
   LDA   #$00
   JMP   .sx_done@11
.sx_neg@10:
   LDA   #$FF
.sx_done@11:
   LDY   #$1E
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$1F
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$20
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@12
   LDA   #$00
   JMP   .sx_done@13
.sx_neg@12:
   LDA   #$FF
.sx_done@13:
   LDY   #$21
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
   BPL   .sx_done@15
   LDA   #$FF
.sx_done@15:
   LDY   #$25
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$45
   LDA   (FP),Y
   LDY   #$23
   ORA   (FP),Y
   LDY   #$27
   STA   (FP),Y
   LDY   #$46
   LDA   (FP),Y
   LDY   #$24
   ORA   (FP),Y
   LDY   #$28
   STA   (FP),Y
   LDY   #$47
   LDA   (FP),Y
   LDY   #$25
   ORA   (FP),Y
   LDY   #$29
   STA   (FP),Y
   LDY   #$48
   LDA   (FP),Y
   LDY   #$26
   ORA   (FP),Y
   LDY   #$2A
   STA   (FP),Y
   LDY   #$1F
   LDA   (FP),Y
   SEC
   LDY   #$27
   SBC   (FP),Y
   LDY   #$2B
   STA   (FP),Y
   LDY   #$20
   LDA   (FP),Y
   LDY   #$28
   SBC   (FP),Y
   LDY   #$2C
   STA   (FP),Y
   LDY   #$21
   LDA   (FP),Y
   LDY   #$29
   SBC   (FP),Y
   LDY   #$2D
   STA   (FP),Y
   LDY   #$22
   LDA   (FP),Y
   LDY   #$2A
   SBC   (FP),Y
   LDY   #$2E
   STA   (FP),Y
   LDY   #$2B
   LDA   (FP),Y
   LDY   #$2F
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
   LDY   #$31
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$32
   STA   (FP),Y
   LDY   #$44
   LDA   (FP),Y
   LDY   #$33
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@16
   LDA   #$00
   JMP   .sx_done@17
.sx_neg@16:
   LDA   #$FF
.sx_done@17:
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
   ORA   #$00
   BMI   .sx_neg@18
   LDA   #$00
   JMP   .sx_done@19
.sx_neg@18:
   LDA   #$FF
.sx_done@19:
   LDY   #$37
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$35
   LDA   (FP),Y
   LDY   #$19
   ORA   (FP),Y
   LDY   #$39
   STA   (FP),Y
   LDY   #$36
   LDA   (FP),Y
   LDY   #$1A
   ORA   (FP),Y
   LDY   #$3A
   STA   (FP),Y
   LDY   #$37
   LDA   (FP),Y
   LDY   #$1B
   ORA   (FP),Y
   LDY   #$3B
   STA   (FP),Y
   LDY   #$38
   LDA   (FP),Y
   LDY   #$1C
   ORA   (FP),Y
   LDY   #$3C
   STA   (FP),Y
   LDY   #$39
   LDA   (FP),Y
   LDY   #$3D
   STA   (FP),Y
   LDY   #$3A
   LDA   (FP),Y
   LDY   #$3E
   STA   (FP),Y
   LDY   #$31
   LDA   (FP),Y
   CLC
   LDY   #$3D
   ADC   (FP),Y
   LDY   #$3F
   STA   (FP),Y
   LDY   #$32
   LDA   (FP),Y
   LDY   #$3E
   ADC   (FP),Y
   LDY   #$40
   STA   (FP),Y
   LDY   #$49
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$3F
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$40
   LDA   (FP),Y
   LDY   #$01
   STA   (DPTR),Y
   LDY   #$45
   LDA   (FP),Y
   LDY   #$41
   STA   (FP),Y

   ; epilogue
   PHA
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
   PLA
   RTS

f4:
   SUBROUTINE

   ; prologue: 15 arg bytes, 104 local bytes
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

   LDA   g1
   LDY   #$01
   STA   (FP),Y
   LDA   g1
   BMI   .sx_neg@20
   LDA   #$00
   JMP   .sx_done@21
.sx_neg@20:
   LDA   #$FF
.sx_done@21:
   LDY   #$02
   STA   (FP),Y
   LDY   #$77
   LDA   (FP),Y
   LDY   #$03
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@22
   LDA   #$00
   JMP   .sx_done@23
.sx_neg@22:
   LDA   #$FF
.sx_done@23:
   LDY   #$04
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   AND   #$07
   LDY   #$05
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDA   #$00
   LDY   #$06
   STA   (FP),Y
   LDY   #$01
   LDA   (FP),Y
   LDY   #$07
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   LDY   #$08
   STA   (FP),Y
   LDY   #$05
   LDA   (FP),Y
   TAX
   BEQ   .shift_done@25
.shift_loop@24:
   LDY   #$07
   LDA   (FP),Y
   ASL
   STA   (FP),Y
   INY
   LDA   (FP),Y
   ROL
   STA   (FP),Y
   DEX
   BNE   .shift_loop@24
.shift_done@25:
   LDY   #$07
   LDA   (FP),Y
   LDY   #$09
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$0A
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@26
   LDA   #$00
   JMP   .sx_done@27
.sx_neg@26:
   LDA   #$FF
.sx_done@27:
   LDY   #$0B
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$6B
   LDA   (FP),Y
   SEC
   LDY   #$09
   SBC   (FP),Y
   LDY   #$6C
   LDA   (FP),Y
   LDY   #$0A
   SBC   (FP),Y
   LDY   #$6D
   LDA   (FP),Y
   LDY   #$0B
   SBC   (FP),Y
   LDY   #$6E
   LDA   (FP),Y
   LDY   #$0C
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
   LDY   #$0F
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$10
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@28
   LDA   #$00
   JMP   .sx_done@29
.sx_neg@28:
   LDA   #$FF
.sx_done@29:
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
   LDY   #$78
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$17
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$18
   STA   (FP),Y
   LDA   g1
   INY
   STA   (FP),Y
   LDA   g1
   BMI   .sx_neg@30
   LDA   #$00
   JMP   .sx_done@31
.sx_neg@30:
   LDA   #$FF
.sx_done@31:
   LDY   #$1A
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ORA   #$01
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
   DEY
   LDA   (FP),Y
   LDY   #$1E
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@32
   LDA   #$00
   JMP   .sx_done@33
.sx_neg@32:
   LDA   #$FF
.sx_done@33:
   LDY   #$1F
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$6F
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
   LDY   #$1D
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
   LDY   #$21
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
   LDY   #$21
   LDA   (FP),Y
   LDY   #$25
   STA   (FP),Y
   LDY   #$22
   LDA   (FP),Y
   LDY   #$26
   STA   (FP),Y
   LDY   #$17
   LDA   (FP),Y
   CLC
   LDY   #$25
   ADC   (FP),Y
   LDY   #$27
   STA   (FP),Y
   LDY   #$18
   LDA   (FP),Y
   LDY   #$26
   ADC   (FP),Y
   LDY   #$28
   STA   (FP),Y
   LDY   #$78
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$27
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$28
   LDA   (FP),Y
   LDY   #$01
   STA   (DPTR),Y
   LDY   #$13
   LDA   (FP),Y
   LDY   #$29
   STA   (FP),Y
   LDY   #$14
   LDA   (FP),Y
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
   LDA   #<f5
   INY
   STA   (FP),Y
   LDA   #>f5
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$2F
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$30
   STA   (FP),Y
   LDY   #$77
   LDA   (FP),Y
   LDY   #$31
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@34
   LDA   #$00
   JMP   .sx_done@35
.sx_neg@34:
   LDA   #$FF
.sx_done@35:
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
   BMI   .sx_neg@36
   LDA   #$00
   JMP   .sx_done@37
.sx_neg@36:
   LDA   #$FF
.sx_done@37:
   LDY   #$35
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$73
   LDA   (FP),Y
   LDY   #$33
   ORA   (FP),Y
   LDY   #$37
   STA   (FP),Y
   LDY   #$74
   LDA   (FP),Y
   LDY   #$34
   ORA   (FP),Y
   LDY   #$38
   STA   (FP),Y
   LDY   #$75
   LDA   (FP),Y
   LDY   #$35
   ORA   (FP),Y
   LDY   #$39
   STA   (FP),Y
   LDY   #$76
   LDA   (FP),Y
   LDY   #$36
   ORA   (FP),Y
   LDY   #$3A
   STA   (FP),Y
   LDY   #$37
   LDA   (FP),Y
   LDY   #$3B
   STA   (FP),Y
   LDY   #$6F
   LDA   (FP),Y
   LDY   #$3C
   STA   (FP),Y
   LDY   #$70
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
   BMI   .sx_neg@38
   LDA   #$00
   JMP   .sx_done@39
.sx_neg@38:
   LDA   #$FF
.sx_done@39:
   LDY   #$40
   STA   (FP),Y
   INY
   STA   (FP),Y
   CLC
   LDA   FP
   ADC   #$2B
   INY
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$07
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDY   #$3B
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
   LDY   #$41
   LDA   (FP),Y
   LDY   #$05
   STA   (SSP),Y
   LDY   #$42
   LDA   (FP),Y
   LDY   #$06
   STA   (SSP),Y
   LDY   #$43
   LDA   (FP),Y
   LDY   #$07
   STA   (SSP),Y
   LDY   #$2F
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   JSR   icall
   LDY   #$44
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
   LDY   #$46
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$13
   LDA   (FP),Y
   LDY   #$45
   EOR   (FP),Y
   LDY   #$49
   STA   (FP),Y
   LDY   #$14
   LDA   (FP),Y
   LDY   #$46
   EOR   (FP),Y
   LDY   #$4A
   STA   (FP),Y
   LDY   #$15
   LDA   (FP),Y
   LDY   #$47
   EOR   (FP),Y
   LDY   #$4B
   STA   (FP),Y
   LDY   #$16
   LDA   (FP),Y
   LDY   #$48
   EOR   (FP),Y
   LDY   #$4C
   STA   (FP),Y
   LDY   #$49
   LDA   (FP),Y
   LDY   #$13
   STA   (FP),Y
   LDY   #$4A
   LDA   (FP),Y
   LDY   #$14
   STA   (FP),Y
   LDY   #$4B
   LDA   (FP),Y
   LDY   #$15
   STA   (FP),Y
   LDY   #$4C
   LDA   (FP),Y
   LDY   #$16
   STA   (FP),Y
   LDY   #$2B
   LDA   (FP),Y
   LDY   #$4D
   STA   (FP),Y
   LDY   #$2C
   LDA   (FP),Y
   LDY   #$4E
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@42
   LDA   #$00
   JMP   .sx_done@43
.sx_neg@42:
   LDA   #$FF
.sx_done@43:
   LDY   #$4F
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$13
   LDA   (FP),Y
   CLC
   LDY   #$4D
   ADC   (FP),Y
   LDY   #$51
   STA   (FP),Y
   LDY   #$14
   LDA   (FP),Y
   LDY   #$4E
   ADC   (FP),Y
   LDY   #$52
   STA   (FP),Y
   LDY   #$15
   LDA   (FP),Y
   LDY   #$4F
   ADC   (FP),Y
   LDY   #$53
   STA   (FP),Y
   LDY   #$16
   LDA   (FP),Y
   LDY   #$50
   ADC   (FP),Y
   LDY   #$54
   STA   (FP),Y
   LDY   #$51
   LDA   (FP),Y
   LDY   #$13
   STA   (FP),Y
   LDY   #$52
   LDA   (FP),Y
   LDY   #$14
   STA   (FP),Y
   LDY   #$53
   LDA   (FP),Y
   LDY   #$15
   STA   (FP),Y
   LDY   #$54
   LDA   (FP),Y
   LDY   #$16
   STA   (FP),Y
   LDA   g1
   LDY   #$55
   STA   (FP),Y
   LDA   g1
   BMI   .sx_neg@44
   LDA   #$00
   JMP   .sx_done@45
.sx_neg@44:
   LDA   #$FF
.sx_done@45:
   LDY   #$56
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$55
   LDA   (FP),Y
   CLC
   LDY   #$13
   ADC   (FP),Y
   LDY   #$59
   STA   (FP),Y
   LDY   #$56
   LDA   (FP),Y
   LDY   #$14
   ADC   (FP),Y
   LDY   #$5A
   STA   (FP),Y
   LDY   #$57
   LDA   (FP),Y
   LDY   #$15
   ADC   (FP),Y
   LDY   #$5B
   STA   (FP),Y
   LDY   #$58
   LDA   (FP),Y
   LDY   #$16
   ADC   (FP),Y
   LDY   #$5C
   STA   (FP),Y
   LDY   #$59
   LDA   (FP),Y
   LDY   #$5D
   STA   (FP),Y
   STA   g1
   LDY   #$6F
   LDA   (FP),Y
   LDY   #$5E
   STA   (FP),Y
   LDY   #$70
   LDA   (FP),Y
   LDY   #$5F
   STA   (FP),Y
   LDY   #$71
   LDA   (FP),Y
   LDY   #$60
   STA   (FP),Y
   LDY   #$72
   LDA   (FP),Y
   LDY   #$61
   STA   (FP),Y
   LDA   g0
   CLC
   LDY   #$5E
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
   INY
   ORA   (FP),Y
   BEQ   .and_false@0
   LDA   #$00
   BNE   .lnot_end@47
   LDA   #$01
.lnot_end@47:
   LDY   #$64
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
   LDY   #$66
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@1:
   LDY   #$66
   LDA   (FP),Y
   LDY   #$68
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

f3:
   SUBROUTINE

   ; prologue: 6 arg bytes, 30 local bytes
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

   LDA   g1
   LDY   #$01
   STA   (FP),Y
   LDA   g1
   BMI   .sx_neg@48
   LDA   #$00
   JMP   .sx_done@49
.sx_neg@48:
   LDA   #$FF
.sx_done@49:
   LDY   #$02
   STA   (FP),Y
   LDY   #$25
   LDA   (FP),Y
   LDY   #$03
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   SEC
   LDY   #$01
   SBC   (FP),Y
   LDY   #$04
   LDA   (FP),Y
   LDY   #$02
   SBC   (FP),Y
   BVC   .cmp_novf@50
   EOR   #$80
.cmp_novf@50:
   ASL
   LDA   #$00
   ROL
   LDY   #$05
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDA   #$07
   EOR   #$FF
   INY
   STA   (FP),Y
   LDA   #$00
   EOR   #$FF
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   AND   #$07
   LDY   #$09
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDA   #$00
   LDY   #$0A
   STA   (FP),Y
   LDY   #$05
   LDA   (FP),Y
   LDY   #$0B
   STA   (FP),Y
   LDY   #$06
   LDA   (FP),Y
   LDY   #$0C
   STA   (FP),Y
   LDY   #$09
   LDA   (FP),Y
   TAX
   BEQ   .shift_done@52
.shift_loop@51:
   LDY   #$0B
   LDA   (FP),Y
   ASL
   STA   (FP),Y
   INY
   LDA   (FP),Y
   ROL
   STA   (FP),Y
   DEX
   BNE   .shift_loop@51
.shift_done@52:
   LDY   #$0B
   LDA   (FP),Y
   LDY   #$0D
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$0E
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@53
   LDA   #$00
   JMP   .sx_done@54
.sx_neg@53:
   LDA   #$FF
.sx_done@54:
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
   INY
   STA   (FP),Y
   LDA   #$07
   BEQ   .and_false@4
   LDA   #$02
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
   INY
   ORA   (FP),Y
   BEQ   .and_false@2
   LDY   #$25
   LDA   (FP),Y
   BEQ   .and_false@2
   LDA   #$01
   LDY   #$17
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
   INY
   STA   (FP),Y
   LDA   #$01
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$11
   LDA   (FP),Y
   LDY   #$1D
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00

   ; epilogue
   PHA
   CLC
   LDA   FP
   ADC   #$26
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

   ; prologue: 6 arg bytes, 110 local bytes
   SEC
   LDA   SSP
   SBC   #$70
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$6F
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
   BPL   .sx_done@56
   LDA   #$FF
.sx_done@56:
   LDY   #$03
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$01
   LDA   (FP),Y
   LDY   #$72
   AND   (FP),Y
   LDY   #$05
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   LDY   #$73
   AND   (FP),Y
   LDY   #$06
   STA   (FP),Y
   LDY   #$03
   LDA   (FP),Y
   LDY   #$74
   AND   (FP),Y
   LDY   #$07
   STA   (FP),Y
   LDY   #$04
   LDA   (FP),Y
   LDY   #$75
   AND   (FP),Y
   LDY   #$08
   STA   (FP),Y
   LDY   #$71
   LDA   (FP),Y
   LDY   #$09
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$0B
   STA   (FP),Y
   LDA   #$00
   DEY
   AND   (FP),Y
   LDY   #$0C
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   AND   #$07
   LDY   #$0D
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDA   #$00
   LDY   #$0E
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
   LDY   #$0D
   LDA   (FP),Y
   STA   HARGS+4
   JSR   asr32
   LDA   HARGS+5
   LDY   #$0F
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
   LDA   #$7F
   CMP   #$7F
   BNE   .thread@0
   LDA   #$FF
   CMP   #$FF
   BEQ   .cmp_true@57
.thread@0:
   LDA   #$00
   JMP   .cmp_end@58
.cmp_true@57:
   LDA   #$01
.cmp_end@58:
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
   BMI   .sx_neg@60
   LDA   #$00
   JMP   .sx_done@61
.sx_neg@60:
   LDA   #$FF
.sx_done@61:
   LDY   #$1B
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$13
   LDA   (FP),Y
   LDY   #$1D
   STA   (FP),Y
   LDY   #$14
   LDA   (FP),Y
   LDY   #$1E
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$1F
   STA   (FP),Y
   LDA   g0
   INY
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$06
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
   LDY   #$1B
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$1C
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   LDY   #$1F
   LDA   (FP),Y
   LDY   #$05
   STA   (SSP),Y
   LDY   #$20
   LDA   (FP),Y
   LDY   #$06
   STA   (SSP),Y
   JSR   f3
   LDY   #$21
   STA   (FP),Y
   ORA   #$00
   BEQ   .if_else@7
   LDA   #$03
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   ORA   #$00
   BPL   .sx_done@63
   LDA   #$FF
.sx_done@63:
   LDY   #$24
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$13
   LDA   (FP),Y
   CLC
   LDY   #$22
   ADC   (FP),Y
   LDY   #$26
   STA   (FP),Y
   LDY   #$14
   LDA   (FP),Y
   LDY   #$23
   ADC   (FP),Y
   LDY   #$27
   STA   (FP),Y
   LDY   #$15
   LDA   (FP),Y
   LDY   #$24
   ADC   (FP),Y
   LDY   #$28
   STA   (FP),Y
   LDY   #$16
   LDA   (FP),Y
   LDY   #$25
   ADC   (FP),Y
   LDY   #$29
   STA   (FP),Y
   LDY   #$26
   LDA   (FP),Y
   LDY   #$13
   STA   (FP),Y
   LDY   #$27
   LDA   (FP),Y
   LDY   #$14
   STA   (FP),Y
   LDY   #$28
   LDA   (FP),Y
   LDY   #$15
   STA   (FP),Y
   LDY   #$29
   LDA   (FP),Y
   LDY   #$16
   STA   (FP),Y
   JMP   .if_end@6
.if_else@7:
   LDA   #$05
   LDY   #$2A
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   ORA   #$00
   BPL   .sx_done@65
   LDA   #$FF
.sx_done@65:
   LDY   #$2C
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$13
   LDA   (FP),Y
   LDY   #$2A
   EOR   (FP),Y
   LDY   #$2E
   STA   (FP),Y
   LDY   #$14
   LDA   (FP),Y
   LDY   #$2B
   EOR   (FP),Y
   LDY   #$2F
   STA   (FP),Y
   LDY   #$15
   LDA   (FP),Y
   LDY   #$2C
   EOR   (FP),Y
   LDY   #$30
   STA   (FP),Y
   LDY   #$16
   LDA   (FP),Y
   LDY   #$2D
   EOR   (FP),Y
   LDY   #$31
   STA   (FP),Y
   LDY   #$2E
   LDA   (FP),Y
   LDY   #$13
   STA   (FP),Y
   LDY   #$2F
   LDA   (FP),Y
   LDY   #$14
   STA   (FP),Y
   LDY   #$30
   LDA   (FP),Y
   LDY   #$15
   STA   (FP),Y
   LDY   #$31
   LDA   (FP),Y
   LDY   #$16
   STA   (FP),Y
.if_end@6:
   LDY   #$13
   LDA   (FP),Y
   LDY   #$32
   STA   (FP),Y
   LDY   #$14
   LDA   (FP),Y
   LDY   #$33
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$34
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$35
   STA   (FP),Y
   LDA   #<f5
   INY
   STA   (FP),Y
   LDA   #>f5
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$38
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$39
   STA   (FP),Y
   LDY   #$76
   LDA   (FP),Y
   LDY   #$3A
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@66
   LDA   #$00
   JMP   .sx_done@67
.sx_neg@66:
   LDA   #$FF
.sx_done@67:
   LDY   #$3B
   STA   (FP),Y
   LDA   g1
   INY
   STA   (FP),Y
   LDA   g1
   BMI   .sx_neg@68
   LDA   #$00
   JMP   .sx_done@69
.sx_neg@68:
   LDA   #$FF
.sx_done@69:
   LDY   #$3D
   STA   (FP),Y
   LDY   #$3A
   LDA   (FP),Y
   SEC
   LDY   #$3C
   SBC   (FP),Y
   LDY   #$3E
   STA   (FP),Y
   LDY   #$3B
   LDA   (FP),Y
   LDY   #$3D
   SBC   (FP),Y
   LDY   #$3F
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$40
   STA   (FP),Y
   LDY   #$71
   LDA   (FP),Y
   LDY   #$41
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   CLC
   LDA   FP
   ADC   #$34
   INY
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$07
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDY   #$40
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$41
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$42
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$43
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   LDY   #$44
   LDA   (FP),Y
   LDY   #$05
   STA   (SSP),Y
   LDY   #$45
   LDA   (FP),Y
   LDY   #$06
   STA   (SSP),Y
   LDY   #$46
   LDA   (FP),Y
   LDY   #$07
   STA   (SSP),Y
   LDY   #$38
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
   BMI   .sx_neg@70
   LDA   #$00
   JMP   .sx_done@71
.sx_neg@70:
   LDA   #$FF
.sx_done@71:
   LDY   #$49
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$13
   LDA   (FP),Y
   LDY   #$48
   EOR   (FP),Y
   LDY   #$4C
   STA   (FP),Y
   LDY   #$14
   LDA   (FP),Y
   LDY   #$49
   EOR   (FP),Y
   LDY   #$4D
   STA   (FP),Y
   LDY   #$15
   LDA   (FP),Y
   LDY   #$4A
   EOR   (FP),Y
   LDY   #$4E
   STA   (FP),Y
   LDY   #$16
   LDA   (FP),Y
   LDY   #$4B
   EOR   (FP),Y
   LDY   #$4F
   STA   (FP),Y
   LDY   #$4C
   LDA   (FP),Y
   LDY   #$13
   STA   (FP),Y
   LDY   #$4D
   LDA   (FP),Y
   LDY   #$14
   STA   (FP),Y
   LDY   #$4E
   LDA   (FP),Y
   LDY   #$15
   STA   (FP),Y
   LDY   #$4F
   LDA   (FP),Y
   LDY   #$16
   STA   (FP),Y
   LDY   #$34
   LDA   (FP),Y
   LDY   #$50
   STA   (FP),Y
   LDY   #$35
   LDA   (FP),Y
   LDY   #$51
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@72
   LDA   #$00
   JMP   .sx_done@73
.sx_neg@72:
   LDA   #$FF
.sx_done@73:
   LDY   #$52
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$13
   LDA   (FP),Y
   CLC
   LDY   #$50
   ADC   (FP),Y
   LDY   #$54
   STA   (FP),Y
   LDY   #$14
   LDA   (FP),Y
   LDY   #$51
   ADC   (FP),Y
   LDY   #$55
   STA   (FP),Y
   LDY   #$15
   LDA   (FP),Y
   LDY   #$52
   ADC   (FP),Y
   LDY   #$56
   STA   (FP),Y
   LDY   #$16
   LDA   (FP),Y
   LDY   #$53
   ADC   (FP),Y
   LDY   #$57
   STA   (FP),Y
   LDY   #$54
   LDA   (FP),Y
   LDY   #$13
   STA   (FP),Y
   LDY   #$55
   LDA   (FP),Y
   LDY   #$14
   STA   (FP),Y
   LDY   #$56
   LDA   (FP),Y
   LDY   #$15
   STA   (FP),Y
   LDY   #$57
   LDA   (FP),Y
   LDY   #$16
   STA   (FP),Y
   LDY   #$72
   LDA   (FP),Y
   LDY   #$58
   STA   (FP),Y
   LDY   #$73
   LDA   (FP),Y
   LDY   #$59
   STA   (FP),Y
   LDY   #$74
   LDA   (FP),Y
   LDY   #$5A
   STA   (FP),Y
   LDY   #$75
   LDA   (FP),Y
   LDY   #$5B
   STA   (FP),Y
   LDY   #$13
   LDA   (FP),Y
   LDY   #$5C
   STA   (FP),Y
   LDY   #$14
   LDA   (FP),Y
   LDY   #$5D
   STA   (FP),Y
   LDY   #$15
   LDA   (FP),Y
   LDY   #$5E
   STA   (FP),Y
   LDY   #$16
   LDA   (FP),Y
   LDY   #$5F
   STA   (FP),Y
   LDA   g0
   STA   HARGS
   LDA   g0+1
   STA   HARGS+1
   LDA   g0+2
   STA   HARGS+2
   LDA   g0+3
   STA   HARGS+3
   LDY   #$5C
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
   LDY   #$60
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
   LDY   #$58
   LDA   (FP),Y
   CLC
   LDY   #$60
   ADC   (FP),Y
   LDY   #$64
   STA   (FP),Y
   LDY   #$59
   LDA   (FP),Y
   LDY   #$61
   ADC   (FP),Y
   LDY   #$65
   STA   (FP),Y
   LDY   #$5A
   LDA   (FP),Y
   LDY   #$62
   ADC   (FP),Y
   LDY   #$66
   STA   (FP),Y
   LDY   #$5B
   LDA   (FP),Y
   LDY   #$63
   ADC   (FP),Y
   LDY   #$67
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   ORA   #$00
   BPL   .sx_done@75
   LDA   #$FF
.sx_done@75:
   LDY   #$6A
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$67
   LDA   (FP),Y
   LDY   #$6B
   CMP   (FP),Y
   BNE   .cmp_true@76
   LDY   #$66
   LDA   (FP),Y
   LDY   #$6A
   CMP   (FP),Y
   BNE   .cmp_true@76
   LDY   #$65
   LDA   (FP),Y
   LDY   #$69
   CMP   (FP),Y
   BNE   .cmp_true@76
   LDY   #$64
   LDA   (FP),Y
   LDY   #$68
   CMP   (FP),Y
   BNE   .cmp_true@76
   LDA   #$00
   JMP   .cmp_end@77
.cmp_true@76:
   LDA   #$01
.cmp_end@77:
   LDY   #$6C
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$6E
   STA   (FP),Y
   CMP   #$01

   ; epilogue
   PHP
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
   PLP
   RTS

f1:
   SUBROUTINE

   ; prologue: 8 arg bytes, 49 local bytes
   SEC
   LDA   SSP
   SBC   #$33
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$32
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   LDA   g0
   EOR   #$FF
   LDY   #$01
   STA   (FP),Y
   LDA   g0+1
   EOR   #$FF
   INY
   STA   (FP),Y
   LDA   g0+2
   EOR   #$FF
   INY
   STA   (FP),Y
   LDA   g0+3
   EOR   #$FF
   INY
   STA   (FP),Y
   LDA   g0
   EOR   g0
   INY
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
   LDA   #$01
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   ORA   #$00
   BPL   .sx_done@80
   LDA   #$FF
.sx_done@80:
   LDY   #$0B
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$05
   LDA   (FP),Y
   LDY   #$09
   ORA   (FP),Y
   LDY   #$0D
   STA   (FP),Y
   LDY   #$06
   LDA   (FP),Y
   LDY   #$0A
   ORA   (FP),Y
   LDY   #$0E
   STA   (FP),Y
   LDY   #$07
   LDA   (FP),Y
   LDY   #$0B
   ORA   (FP),Y
   LDY   #$0F
   STA   (FP),Y
   LDY   #$08
   LDA   (FP),Y
   LDY   #$0C
   ORA   (FP),Y
   LDY   #$10
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
   LDY   #$0D
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
   LDY   #$11
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
   LDY   #$11
   LDA   (FP),Y
   LDY   #$15
   STA   (FP),Y
   LDY   #$12
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
   LDY   #$38
   LDA   (FP),Y
   LDY   #$19
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
   LDY   #$38
   LDA   (FP),Y
   LDY   #$1E
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$06
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
   LDY   #$1B
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$1C
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   LDY   #$1D
   LDA   (FP),Y
   LDY   #$05
   STA   (SSP),Y
   LDY   #$1E
   LDA   (FP),Y
   LDY   #$06
   STA   (SSP),Y
   JSR   f2
   LDA   #$00
   ROL
   LDY   #$1F
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$17
   LDA   (FP),Y
   LDY   #$20
   EOR   (FP),Y
   LDY   #$22
   STA   (FP),Y
   LDY   #$18
   LDA   (FP),Y
   LDY   #$21
   EOR   (FP),Y
   LDY   #$23
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$17
   STA   (FP),Y
   LDY   #$23
   LDA   (FP),Y
   LDY   #$18
   STA   (FP),Y
   LDY   #$34
   LDA   (FP),Y
   LDY   #$24
   STA   (FP),Y
   LDY   #$35
   LDA   (FP),Y
   LDY   #$25
   STA   (FP),Y
   LDY   #$36
   LDA   (FP),Y
   LDY   #$26
   STA   (FP),Y
   LDY   #$37
   LDA   (FP),Y
   LDY   #$27
   STA   (FP),Y
   LDY   #$24
   LDA   (FP),Y
   SEC
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
   LDA   #$03
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   ORA   #$00
   BPL   .sx_done@82
   LDA   #$FF
.sx_done@82:
   LDY   #$2C
   STA   (FP),Y
   INY
   STA   (FP),Y
   CMP   g0+3
   BNE   .thread@1
   CMP   g0+2
   BNE   .thread@1
   LDY   #$2B
   LDA   (FP),Y
   CMP   g0+1
   BNE   .thread@1
   DEY
   LDA   (FP),Y
   CMP   g0
   BEQ   .cmp_true@83
.thread@1:
   LDA   #$00
   JMP   .cmp_end@84
.cmp_true@83:
   LDA   #$01
.cmp_end@84:
   LDY   #$2E
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$28
   LDA   (FP),Y
   LDY   #$2E
   AND   (FP),Y
   LDY   #$30
   STA   (FP),Y
   LDY   #$29
   LDA   (FP),Y
   LDY   #$2F
   AND   (FP),Y
   LDY   #$31
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
   ADC   #$3B
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$32
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

   ; prologue: 4 arg bytes, 67 local bytes
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

   LDA   g1
   LDY   #$01
   STA   (FP),Y
   LDA   g1
   BMI   .sx_neg@86
   LDA   #$00
   JMP   .sx_done@87
.sx_neg@86:
   LDA   #$FF
.sx_done@87:
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
   BMI   .sx_neg@88
   LDA   #$00
   JMP   .sx_done@89
.sx_neg@88:
   LDA   #$FF
.sx_done@89:
   LDY   #$05
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$46
   LDA   (FP),Y
   CLC
   LDY   #$03
   ADC   (FP),Y
   LDY   #$07
   STA   (FP),Y
   LDY   #$47
   LDA   (FP),Y
   LDY   #$04
   ADC   (FP),Y
   LDY   #$08
   STA   (FP),Y
   LDY   #$48
   LDA   (FP),Y
   LDY   #$05
   ADC   (FP),Y
   LDY   #$09
   STA   (FP),Y
   LDY   #$49
   LDA   (FP),Y
   LDY   #$06
   ADC   (FP),Y
   LDY   #$0A
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
   LDY   #$0A
   LDA   (FP),Y
   LDY   #$0E
   STA   (FP),Y
   LDA   g0
   LDY   #$0B
   AND   (FP),Y
   LDY   #$0F
   STA   (FP),Y
   LDA   g0+1
   LDY   #$0C
   AND   (FP),Y
   LDY   #$10
   STA   (FP),Y
   LDA   g0+2
   LDY   #$0D
   AND   (FP),Y
   LDY   #$11
   STA   (FP),Y
   LDA   g0+3
   LDY   #$0E
   AND   (FP),Y
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
   DEY
   LDA   (FP),Y
   LDY   #$15
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$16
   STA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@90
   DEY
   LDA   (FP),Y
   CMP   #$01
   BNE   .cmp_true@90
   LDA   #$00
   JMP   .cmp_end@91
.cmp_true@90:
   LDA   #$01
.cmp_end@91:
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
   BMI   .sx_neg@93
   LDA   #$00
   JMP   .sx_done@94
.sx_neg@93:
   LDA   #$FF
.sx_done@94:
   LDY   #$1B
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$46
   LDA   (FP),Y
   LDY   #$1D
   STA   (FP),Y
   LDY   #$47
   LDA   (FP),Y
   LDY   #$1E
   STA   (FP),Y
   LDY   #$48
   LDA   (FP),Y
   LDY   #$1F
   STA   (FP),Y
   LDY   #$49
   LDA   (FP),Y
   LDY   #$20
   STA   (FP),Y
   LDA   #$01
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   ORA   #$00
   BPL   .sx_done@96
   LDA   #$FF
.sx_done@96:
   LDY   #$23
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   g0
   LDY   #$21
   ORA   (FP),Y
   LDY   #$25
   STA   (FP),Y
   LDA   g0+1
   LDY   #$22
   ORA   (FP),Y
   LDY   #$26
   STA   (FP),Y
   LDA   g0+2
   LDY   #$23
   ORA   (FP),Y
   LDY   #$27
   STA   (FP),Y
   LDA   g0+3
   LDY   #$24
   ORA   (FP),Y
   LDY   #$28
   STA   (FP),Y
   LDY   #$1D
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
   LDY   #$25
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
   LDY   #$29
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
   LDY   #$29
   LDA   (FP),Y
   LDY   #$2D
   STA   (FP),Y
   LDA   #$FF
   SEC
   LDY   #$15
   SBC   (FP),Y
   LDY   #$2E
   STA   (FP),Y
   LDA   #$7F
   LDY   #$16
   SBC   (FP),Y
   LDY   #$2F
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$30
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$06
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
   LDY   #$1B
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$1C
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   LDY   #$2D
   LDA   (FP),Y
   LDY   #$05
   STA   (SSP),Y
   LDY   #$30
   LDA   (FP),Y
   LDY   #$06
   STA   (SSP),Y
   JSR   f3
   LDY   #$31
   STA   (FP),Y
   ORA   #$00
   BEQ   .if_else@9
   LDY   #$15
   LDA   (FP),Y
   CLC
   ADC   #$03
   LDY   #$32
   STA   (FP),Y
   LDY   #$16
   LDA   (FP),Y
   ADC   #$00
   LDY   #$33
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$15
   STA   (FP),Y
   LDY   #$33
   LDA   (FP),Y
   LDY   #$16
   STA   (FP),Y
   JMP   .if_end@8
.if_else@9:
   LDY   #$15
   LDA   (FP),Y
   EOR   #$05
   LDY   #$34
   STA   (FP),Y
   LDY   #$16
   LDA   (FP),Y
   LDY   #$35
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$15
   STA   (FP),Y
   LDY   #$35
   LDA   (FP),Y
   LDY   #$16
   STA   (FP),Y
.if_end@8:
   LDA   g1
   LDY   #$36
   STA   (FP),Y
   LDA   g1
   BMI   .sx_neg@97
   LDA   #$00
   JMP   .sx_done@98
.sx_neg@97:
   LDA   #$FF
.sx_done@98:
   LDY   #$37
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   LDY   #$15
   ADC   (FP),Y
   LDY   #$38
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$16
   ADC   (FP),Y
   LDY   #$39
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$3A
   STA   (FP),Y
   STA   g1
   INY
   STA   (FP),Y
   LDA   g1
   BMI   .sx_neg@99
   LDA   #$00
   JMP   .sx_done@100
.sx_neg@99:
   LDA   #$FF
.sx_done@100:
   LDY   #$3C
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   EOR   #$FF
   LDY   #$3D
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   EOR   #$FF
   LDY   #$3E
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$3F
   STA   (FP),Y
   INY
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@101
   LDA   #$00
   JMP   .sx_done@102
.sx_neg@101:
   LDA   #$FF
.sx_done@102:
   LDY   #$41
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$40
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
   ADC   #$49
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

   ; prologue: 0 arg bytes, 38 local bytes
   SEC
   LDA   SSP
   SBC   #$28
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$27
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   LDA   #$20
   LDY   #$01
   STA   (FP),Y
   LDA   #$01
   INY
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@103
   LDA   #$00
   JMP   .sx_done@104
.sx_neg@103:
   LDA   #$FF
.sx_done@104:
   LDY   #$03
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
   LDY   #$01
   LDA   (FP),Y
   STA   (SSP),Y
   INY
   LDA   (FP),Y
   STA   (SSP),Y
   INY
   LDA   (FP),Y
   STA   (SSP),Y
   INY
   LDA   (FP),Y
   STA   (SSP),Y
   JSR   f0
   LDA   HARGS+8
   LDY   #$05
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
   LDY   #$09
   STA   (FP),Y
   LDY   #$06
   LDA   (FP),Y
   LDY   #$0A
   STA   (FP),Y
   LDY   #$07
   LDA   (FP),Y
   LDY   #$0B
   STA   (FP),Y
   LDY   #$08
   LDA   (FP),Y
   LDY   #$0C
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
   LDA   g1
   BMI   .sx_neg@105
   LDA   #$00
   JMP   .sx_done@106
.sx_neg@105:
   LDA   #$FF
.sx_done@106:
   LDY   #$12
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
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
   LDY   #$09
   LDA   (FP),Y
   LDY   #$15
   EOR   (FP),Y
   LDY   #$19
   STA   (FP),Y
   LDY   #$0A
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
   LDY   #$19
   LDA   (FP),Y
   LDY   #$09
   STA   (FP),Y
   LDY   #$1A
   LDA   (FP),Y
   LDY   #$0A
   STA   (FP),Y
   LDY   #$1B
   LDA   (FP),Y
   LDY   #$0B
   STA   (FP),Y
   LDY   #$1C
   LDA   (FP),Y
   LDY   #$0C
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
   LDA   #$10
   STA   HARGS+4
   JSR   asr32
   LDA   HARGS+5
   LDY   #$1D
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
   LDY   #$21
   LDA   (FP),Y
   LDY   #$25
   STA   (FP),Y
   LDY   #$22
   LDA   (FP),Y
   LDY   #$26
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
   ADC   #$28
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$27
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
   DC.L  $0000001A

g1:
   DC.B  $5D
