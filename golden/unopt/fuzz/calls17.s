SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24

f5:
   SUBROUTINE

   ; prologue: 20 arg bytes, 35 local bytes
   SEC
   LDA   SSP
   SBC   #$25
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$24
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
   LDY   #$04
   LDA   (FP),Y
   LDY   #$08
   STA   (FP),Y
   LDY   #$38
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$09
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$0A
   STA   (FP),Y
   LDY   #$2C
   LDA   (FP),Y
   ORA   #$01
   LDY   #$0B
   STA   (FP),Y
   LDY   #$2D
   LDA   (FP),Y
   LDY   #$0C
   STA   (FP),Y
   LDY   #$2E
   LDA   (FP),Y
   LDY   #$0D
   STA   (FP),Y
   LDY   #$2F
   LDA   (FP),Y
   LDY   #$0E
   STA   (FP),Y
   LDY   #$30
   LDA   (FP),Y
   LDY   #$0F
   STA   (FP),Y
   LDY   #$31
   LDA   (FP),Y
   LDY   #$10
   STA   (FP),Y
   LDY   #$32
   LDA   (FP),Y
   LDY   #$11
   STA   (FP),Y
   LDY   #$33
   LDA   (FP),Y
   LDY   #$12
   STA   (FP),Y
   LDY   #$2C
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
   LDY   #$0B
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
   JSR   udivmod64
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
   LDY   #$09
   LDA   (FP),Y
   CLC
   LDY   #$1B
   ADC   (FP),Y
   LDY   #$1D
   STA   (FP),Y
   LDY   #$0A
   LDA   (FP),Y
   LDY   #$1C
   ADC   (FP),Y
   LDY   #$1E
   STA   (FP),Y
   LDY   #$38
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
   LDY   #$38
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$1F
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$20
   STA   (FP),Y
   LDA   g1
   SEC
   DEY
   SBC   (FP),Y
   LDY   #$21
   STA   (FP),Y
   LDA   g1+1
   DEY
   SBC   (FP),Y
   LDY   #$22
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$23
   STA   (FP),Y

   ; epilogue
   PHA
   CLC
   LDA   FP
   ADC   #$39
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

   ; prologue: 0 arg bytes, 120 local bytes
   SEC
   LDA   SSP
   SBC   #$7A
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$79
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
   LDA   g1+1
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
   LDY   #$05
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   INY
   ORA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@0
   LDA   #$02
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
   LDY   #$09
   LDA   (FP),Y
   SEC
   SBC   g0
   LDY   #$0D
   STA   (FP),Y
   LDY   #$0A
   LDA   (FP),Y
   SBC   g0+1
   LDY   #$0E
   STA   (FP),Y
   LDY   #$0B
   LDA   (FP),Y
   SBC   g0+2
   LDY   #$0F
   STA   (FP),Y
   LDY   #$0C
   LDA   (FP),Y
   SBC   g0+3
   LDY   #$10
   STA   (FP),Y
   LDY   #$0D
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   INY
   ORA   (FP),Y
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
   LDY   #$11
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@1:
   LDY   #$11
   LDA   (FP),Y
   LDY   #$13
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$05
   LDA   (FP),Y
   LDY   #$15
   STA   (FP),Y
   LDY   #$06
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
   LDA   #$01
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDA   g1
   INY
   STA   (FP),Y
   LDA   g1+1
   INY
   STA   (FP),Y
   LDY   #$19
   LDA   (FP),Y
   TAX
   BEQ   .shift_done@5
.shift_loop@4:
   LDY   #$1C
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
   BNE   .shift_loop@4
.shift_done@5:
   LDY   #$1B
   LDA   (FP),Y
   LDY   #$1D
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$1E
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@6
   LDA   #$00
   JMP   .sx_done@7
.sx_neg@6:
   LDA   #$FF
.sx_done@7:
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
   AND   #$01
   INY
   STA   (FP),Y
   LDA   g1
   INY
   STA   (FP),Y
   LDA   g1+1
   INY
   STA   (FP),Y
   LDA   g1+1
   BMI   .sx_neg@8
   LDA   #$00
   JMP   .sx_done@9
.sx_neg@8:
   LDA   #$FF
.sx_done@9:
   LDY   #$27
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   g0
   LDY   #$25
   EOR   (FP),Y
   LDY   #$29
   STA   (FP),Y
   LDA   g0+1
   LDY   #$26
   EOR   (FP),Y
   LDY   #$2A
   STA   (FP),Y
   LDA   g0+2
   LDY   #$27
   EOR   (FP),Y
   LDY   #$2B
   STA   (FP),Y
   LDA   g0+3
   LDY   #$28
   EOR   (FP),Y
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
   LDY   #$14
   LDA   (FP),Y
   LDY   #$2F
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@10
   LDA   #$00
   JMP   .sx_done@11
.sx_neg@10:
   LDA   #$FF
.sx_done@11:
   LDY   #$30
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$31
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$32
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@12
   LDA   #$00
   JMP   .sx_done@13
.sx_neg@12:
   LDA   #$FF
.sx_done@13:
   LDY   #$33
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$31
   LDA   (FP),Y
   SEC
   LDY   #$05
   SBC   (FP),Y
   LDY   #$32
   LDA   (FP),Y
   LDY   #$06
   SBC   (FP),Y
   LDY   #$33
   LDA   (FP),Y
   LDY   #$07
   SBC   (FP),Y
   LDY   #$34
   LDA   (FP),Y
   LDY   #$08
   SBC   (FP),Y
   BVC   .cmp_novf@14
   EOR   #$80
.cmp_novf@14:
   ASL
   LDA   #$00
   ROL
   EOR   #$01
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
   BMI   .sx_neg@15
   LDA   #$00
   JMP   .sx_done@16
.sx_neg@15:
   LDA   #$FF
.sx_done@16:
   LDY   #$39
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
   LDY   #$37
   LDA   (FP),Y
   LDY   #$3F
   STA   (FP),Y
   LDY   #$38
   LDA   (FP),Y
   LDY   #$40
   STA   (FP),Y
   LDY   #$39
   LDA   (FP),Y
   LDY   #$41
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   AND   #$07
   INY
   STA   (FP),Y
   LDY   #$14
   LDA   (FP),Y
   LDY   #$47
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@17
   LDA   #$00
   JMP   .sx_done@18
.sx_neg@17:
   LDA   #$FF
.sx_done@18:
   LDY   #$48
   STA   (FP),Y
   LDY   #$14
   LDA   (FP),Y
   LDY   #$49
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@19
   LDA   #$00
   JMP   .sx_done@20
.sx_neg@19:
   LDA   #$FF
.sx_done@20:
   LDY   #$4A
   STA   (FP),Y
   LDY   #$47
   LDA   (FP),Y
   LDY   #$49
   ORA   (FP),Y
   LDY   #$4B
   STA   (FP),Y
   LDY   #$48
   LDA   (FP),Y
   LDY   #$4A
   ORA   (FP),Y
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
   ORA   #$00
   BMI   .sx_neg@21
   LDA   #$00
   JMP   .sx_done@22
.sx_neg@21:
   LDA   #$FF
.sx_done@22:
   LDY   #$4F
   STA   (FP),Y
   INY
   STA   (FP),Y
   CLC
   LDA   FP
   ADC   #$17
   INY
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$14
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
   LDY   #$2D
   LDA   (FP),Y
   LDY   #$05
   STA   (SSP),Y
   LDY   #$2E
   LDA   (FP),Y
   LDY   #$06
   STA   (SSP),Y
   LDY   #$3F
   LDA   (FP),Y
   LDY   #$07
   STA   (SSP),Y
   LDY   #$40
   LDA   (FP),Y
   LDY   #$08
   STA   (SSP),Y
   LDY   #$41
   LDA   (FP),Y
   LDY   #$09
   STA   (SSP),Y
   LDY   #$42
   LDA   (FP),Y
   LDY   #$0A
   STA   (SSP),Y
   LDY   #$43
   LDA   (FP),Y
   LDY   #$0B
   STA   (SSP),Y
   LDY   #$44
   LDA   (FP),Y
   LDY   #$0C
   STA   (SSP),Y
   LDY   #$45
   LDA   (FP),Y
   LDY   #$0D
   STA   (SSP),Y
   LDY   #$46
   LDA   (FP),Y
   LDY   #$0E
   STA   (SSP),Y
   LDY   #$4D
   LDA   (FP),Y
   LDY   #$0F
   STA   (SSP),Y
   LDY   #$4E
   LDA   (FP),Y
   LDY   #$10
   STA   (SSP),Y
   LDY   #$4F
   LDA   (FP),Y
   LDY   #$11
   STA   (SSP),Y
   LDY   #$50
   LDA   (FP),Y
   LDY   #$12
   STA   (SSP),Y
   LDY   #$51
   LDA   (FP),Y
   LDY   #$13
   STA   (SSP),Y
   LDY   #$52
   LDA   (FP),Y
   LDY   #$14
   STA   (SSP),Y
   JSR   f5
   LDY   #$53
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
   LDY   #$05
   LDA   (FP),Y
   LDY   #$54
   EOR   (FP),Y
   LDY   #$58
   STA   (FP),Y
   LDY   #$06
   LDA   (FP),Y
   LDY   #$55
   EOR   (FP),Y
   LDY   #$59
   STA   (FP),Y
   LDY   #$07
   LDA   (FP),Y
   LDY   #$56
   EOR   (FP),Y
   LDY   #$5A
   STA   (FP),Y
   LDY   #$08
   LDA   (FP),Y
   LDY   #$57
   EOR   (FP),Y
   LDY   #$5B
   STA   (FP),Y
   LDY   #$58
   LDA   (FP),Y
   LDY   #$05
   STA   (FP),Y
   LDY   #$59
   LDA   (FP),Y
   LDY   #$06
   STA   (FP),Y
   LDY   #$5A
   LDA   (FP),Y
   LDY   #$07
   STA   (FP),Y
   LDY   #$5B
   LDA   (FP),Y
   LDY   #$08
   STA   (FP),Y
   LDY   #$17
   LDA   (FP),Y
   LDY   #$5C
   STA   (FP),Y
   LDY   #$18
   LDA   (FP),Y
   LDY   #$5D
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@23
   LDA   #$00
   JMP   .sx_done@24
.sx_neg@23:
   LDA   #$FF
.sx_done@24:
   LDY   #$5E
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$05
   LDA   (FP),Y
   CLC
   LDY   #$5C
   ADC   (FP),Y
   LDY   #$60
   STA   (FP),Y
   LDY   #$06
   LDA   (FP),Y
   LDY   #$5D
   ADC   (FP),Y
   LDY   #$61
   STA   (FP),Y
   LDY   #$07
   LDA   (FP),Y
   LDY   #$5E
   ADC   (FP),Y
   LDY   #$62
   STA   (FP),Y
   LDY   #$08
   LDA   (FP),Y
   LDY   #$5F
   ADC   (FP),Y
   LDY   #$63
   STA   (FP),Y
   LDY   #$60
   LDA   (FP),Y
   LDY   #$05
   STA   (FP),Y
   LDY   #$61
   LDA   (FP),Y
   LDY   #$06
   STA   (FP),Y
   LDY   #$62
   LDA   (FP),Y
   LDY   #$07
   STA   (FP),Y
   LDY   #$63
   LDA   (FP),Y
   LDY   #$08
   STA   (FP),Y
   LDA   #$64
   LDY   #$64
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   ORA   #$00
   BPL   .sx_done@26
   LDA   #$FF
.sx_done@26:
   LDY   #$66
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   g0
   LDY   #$64
   AND   (FP),Y
   LDY   #$68
   STA   (FP),Y
   LDA   g0+1
   LDY   #$65
   AND   (FP),Y
   LDY   #$69
   STA   (FP),Y
   LDA   g0+2
   LDY   #$66
   AND   (FP),Y
   LDY   #$6A
   STA   (FP),Y
   LDA   g0+3
   LDY   #$67
   AND   (FP),Y
   LDY   #$6B
   STA   (FP),Y
   LDA   g1
   AND   #$07
   INY
   STA   (FP),Y
   LDA   #$00
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
   LDY   #$6C
   LDA   (FP),Y
   STA   HARGS+4
   JSR   asr32
   LDA   HARGS+5
   LDY   #$6E
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
   LDY   #$71
   LDA   (FP),Y
   LDY   #$75
   STA   (FP),Y
   LDY   #$68
   LDA   (FP),Y
   SEC
   LDY   #$72
   SBC   (FP),Y
   LDY   #$69
   LDA   (FP),Y
   LDY   #$73
   SBC   (FP),Y
   LDY   #$6A
   LDA   (FP),Y
   LDY   #$74
   SBC   (FP),Y
   LDY   #$6B
   LDA   (FP),Y
   LDY   #$75
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
   LDY   #$78
   STA   (FP),Y
   CMP   #$01

   ; epilogue
   PHP
   CLC
   LDA   FP
   ADC   #$7A
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

   ; prologue: 2 arg bytes, 24 local bytes
   SEC
   LDA   SSP
   SBC   #$1A
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$19
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   LDA   g1
   ORA   #$01
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
   ORA   #$00
   BMI   .sx_neg@27
   LDA   #$00
   JMP   .sx_done@28
.sx_neg@27:
   LDA   #$FF
.sx_done@28:
   LDY   #$05
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   g0
   STA   HARGS
   LDA   g0+1
   STA   HARGS+1
   LDA   g0+2
   STA   HARGS+2
   LDA   g0+3
   STA   HARGS+3
   LDY   #$03
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
   LDY   #$07
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
   LDY   #$0B
   STA   (FP),Y
   LDY   #$1C
   LDA   (FP),Y
   LDY   #$0C
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   g0+3
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
   BEQ   .cmp_true@29
.thread@0:
   LDA   #$00
   JMP   .cmp_end@30
.cmp_true@29:
   LDA   #$01
.cmp_end@30:
   LDY   #$0F
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$11
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$12
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@32
   LDA   #$00
   JMP   .sx_done@33
.sx_neg@32:
   LDA   #$FF
.sx_done@33:
   LDY   #$13
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$11
   LDA   (FP),Y
   CLC
   LDY   #$07
   SBC   (FP),Y
   LDY   #$12
   LDA   (FP),Y
   LDY   #$08
   SBC   (FP),Y
   LDY   #$13
   LDA   (FP),Y
   LDY   #$09
   SBC   (FP),Y
   LDY   #$14
   LDA   (FP),Y
   LDY   #$0A
   SBC   (FP),Y
   LDA   #$00
   ROL
   LDY   #$15
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$17
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$1C
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
   ADC   #$1C
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$19
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

   ; prologue: 3 arg bytes, 150 local bytes
   SEC
   LDA   SSP
   SBC   #$98
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$97
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
   SBC   #$01
   LDA   #$00
   SBC   #$00
   BVC   .cmp_novf@34
   EOR   #$80
.cmp_novf@34:
   ASL
   LDA   #$00
   ROL
   EOR   #$01
   LDY   #$01
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   SEC
   DEY
   SBC   (FP),Y
   LDY   #$03
   STA   (FP),Y
   LDA   #$00
   DEY
   SBC   (FP),Y
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
   ORA   #$00
   BMI   .sx_neg@35
   LDA   #$00
   JMP   .sx_done@36
.sx_neg@35:
   LDA   #$FF
.sx_done@36:
   LDY   #$07
   STA   (FP),Y
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
   INY
   STA   (FP),Y
   LDA   g1
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$99
   LDA   (FP),Y
   LDY   #$0F
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@37
   LDA   #$00
   JMP   .sx_done@38
.sx_neg@37:
   LDA   #$FF
.sx_done@38:
   LDY   #$10
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   HARGS
   INY
   LDA   (FP),Y
   STA   HARGS+1
   LDA   g1
   STA   HARGS+2
   LDA   g1+1
   STA   HARGS+3
   JSR   mul16
   LDA   HARGS+4
   LDY   #$11
   STA   (FP),Y
   LDA   HARGS+5
   INY
   STA   (FP),Y
   LDY   #$99
   LDA   (FP),Y
   LDY   #$13
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@39
   LDA   #$00
   JMP   .sx_done@40
.sx_neg@39:
   LDA   #$FF
.sx_done@40:
   LDY   #$14
   STA   (FP),Y
   LDA   g1
   SEC
   DEY
   SBC   (FP),Y
   LDA   g1+1
   INY
   SBC   (FP),Y
   BVC   .cmp_novf@41
   EOR   #$80
.cmp_novf@41:
   ASL
   LDA   #$00
   ROL
   LDY   #$15
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ORA   #$01
   LDY   #$17
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$18
   STA   (FP),Y
   LDY   #$11
   LDA   (FP),Y
   STA   HARGS
   INY
   LDA   (FP),Y
   STA   HARGS+1
   LDY   #$17
   LDA   (FP),Y
   STA   HARGS+2
   INY
   LDA   (FP),Y
   STA   HARGS+3
   JSR   sdivmod16
   LDA   HARGS+6
   LDY   #$19
   STA   (FP),Y
   LDA   HARGS+7
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
   BMI   .sx_neg@42
   LDA   #$00
   JMP   .sx_done@43
.sx_neg@42:
   LDA   #$FF
.sx_done@43:
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
   LDY   #$09
   LDA   (FP),Y
   LDY   #$23
   STA   (FP),Y
   LDY   #$0A
   LDA   (FP),Y
   LDY   #$24
   STA   (FP),Y
   LDY   #$0B
   LDA   (FP),Y
   LDY   #$25
   STA   (FP),Y
   LDY   #$0C
   LDA   (FP),Y
   LDY   #$26
   STA   (FP),Y
   CMP   g0+3
   BNE   .cmp_true@44
   DEY
   LDA   (FP),Y
   CMP   g0+2
   BNE   .cmp_true@44
   DEY
   LDA   (FP),Y
   CMP   g0+1
   BNE   .cmp_true@44
   DEY
   LDA   (FP),Y
   CMP   g0
   BNE   .cmp_true@44
   LDA   #$00
   JMP   .cmp_end@45
.cmp_true@44:
   LDA   #$01
.cmp_end@45:
   LDY   #$27
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$99
   LDA   (FP),Y
   LDY   #$29
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@47
   LDA   #$00
   JMP   .sx_done@48
.sx_neg@47:
   LDA   #$FF
.sx_done@48:
   LDY   #$2A
   STA   (FP),Y
   LDA   #$02
   SEC
   DEY
   SBC   (FP),Y
   LDA   #$00
   INY
   SBC   (FP),Y
   BVC   .cmp_novf@49
   EOR   #$80
.cmp_novf@49:
   ASL
   LDA   #$00
   ROL
   EOR   #$01
   LDY   #$2B
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$28
   LDA   (FP),Y
   LDY   #$2C
   CMP   (FP),Y
   BNE   .thread@1
   LDY   #$27
   LDA   (FP),Y
   LDY   #$2B
   CMP   (FP),Y
   BEQ   .cmp_true@50
.thread@1:
   LDA   #$00
   JMP   .cmp_end@51
.cmp_true@50:
   LDA   #$01
.cmp_end@51:
   LDY   #$2D
   STA   (FP),Y
   LDA   #$00
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
   DEY
   LDA   (FP),Y
   LDY   #$31
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$32
   STA   (FP),Y
   LDY   #$9A
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$33
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$34
   STA   (FP),Y
   LDA   g0
   AND   g0
   INY
   STA   (FP),Y
   LDA   g0+1
   AND   g0+1
   INY
   STA   (FP),Y
   LDA   g0+2
   AND   g0+2
   INY
   STA   (FP),Y
   LDA   g0+3
   AND   g0+3
   INY
   STA   (FP),Y
   LDY   #$35
   LDA   (FP),Y
   LDY   #$39
   STA   (FP),Y
   LDY   #$36
   LDA   (FP),Y
   LDY   #$3A
   STA   (FP),Y
   LDY   #$33
   LDA   (FP),Y
   CLC
   LDY   #$39
   ADC   (FP),Y
   LDY   #$3B
   STA   (FP),Y
   LDY   #$34
   LDA   (FP),Y
   LDY   #$3A
   ADC   (FP),Y
   LDY   #$3C
   STA   (FP),Y
   LDY   #$9A
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
   JSR   f4
   LDA   #$00
   ROL
   LDY   #$3D
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
   LDY   #$1F
   LDA   (FP),Y
   LDY   #$3E
   EOR   (FP),Y
   LDY   #$42
   STA   (FP),Y
   LDY   #$20
   LDA   (FP),Y
   LDY   #$3F
   EOR   (FP),Y
   LDY   #$43
   STA   (FP),Y
   LDY   #$21
   LDA   (FP),Y
   LDY   #$40
   EOR   (FP),Y
   LDY   #$44
   STA   (FP),Y
   LDY   #$22
   LDA   (FP),Y
   LDY   #$41
   EOR   (FP),Y
   LDY   #$45
   STA   (FP),Y
   LDY   #$42
   LDA   (FP),Y
   LDY   #$1F
   STA   (FP),Y
   LDY   #$43
   LDA   (FP),Y
   LDY   #$20
   STA   (FP),Y
   LDY   #$44
   LDA   (FP),Y
   LDY   #$21
   STA   (FP),Y
   LDY   #$45
   LDA   (FP),Y
   LDY   #$22
   STA   (FP),Y
   LDY   #$09
   LDA   (FP),Y
   LDY   #$46
   STA   (FP),Y
   LDY   #$0A
   LDA   (FP),Y
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
   LDY   #$09
   LDA   (FP),Y
   EOR   #$FF
   LDY   #$4A
   STA   (FP),Y
   LDY   #$0A
   LDA   (FP),Y
   EOR   #$FF
   LDY   #$4B
   STA   (FP),Y
   LDY   #$0B
   LDA   (FP),Y
   EOR   #$FF
   LDY   #$4C
   STA   (FP),Y
   LDY   #$0C
   LDA   (FP),Y
   EOR   #$FF
   LDY   #$4D
   STA   (FP),Y
   LDY   #$4A
   LDA   (FP),Y
   LDY   #$4E
   STA   (FP),Y
   LDY   #$4B
   LDA   (FP),Y
   LDY   #$4F
   STA   (FP),Y
   LDA   g1
   INY
   STA   (FP),Y
   LDA   g1+1
   INY
   STA   (FP),Y
   LDA   g1+1
   BMI   .sx_neg@53
   LDA   #$00
   JMP   .sx_done@54
.sx_neg@53:
   LDA   #$FF
.sx_done@54:
   LDY   #$52
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$0C
   CMP   (FP),Y
   BNE   .cmp_true@55
   DEY
   CMP   (FP),Y
   BNE   .cmp_true@55
   LDY   #$51
   LDA   (FP),Y
   LDY   #$0A
   CMP   (FP),Y
   BNE   .cmp_true@55
   LDY   #$50
   LDA   (FP),Y
   LDY   #$09
   CMP   (FP),Y
   BNE   .cmp_true@55
   LDA   #$00
   JMP   .cmp_end@56
.cmp_true@55:
   LDA   #$01
.cmp_end@56:
   LDY   #$54
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$56
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$57
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@58
   LDA   #$00
   JMP   .sx_done@59
.sx_neg@58:
   LDA   #$FF
.sx_done@59:
   LDY   #$58
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
   LDY   #$56
   LDA   (FP),Y
   LDY   #$5E
   STA   (FP),Y
   LDY   #$57
   LDA   (FP),Y
   LDY   #$5F
   STA   (FP),Y
   LDY   #$58
   LDA   (FP),Y
   LDY   #$60
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   AND   #$07
   INY
   STA   (FP),Y
   LDY   #$99
   LDA   (FP),Y
   LDY   #$66
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@60
   LDA   #$00
   JMP   .sx_done@61
.sx_neg@60:
   LDA   #$FF
.sx_done@61:
   LDY   #$67
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   CLC
   LDA   FP
   ADC   #$48
   INY
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$14
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   #$01
   LDY   #$01
   STA   (SSP),Y
   LDA   #$00
   INY
   STA   (SSP),Y
   INY
   STA   (SSP),Y
   INY
   STA   (SSP),Y
   LDY   #$4E
   LDA   (FP),Y
   LDY   #$05
   STA   (SSP),Y
   LDY   #$4F
   LDA   (FP),Y
   LDY   #$06
   STA   (SSP),Y
   LDY   #$5E
   LDA   (FP),Y
   LDY   #$07
   STA   (SSP),Y
   LDY   #$5F
   LDA   (FP),Y
   LDY   #$08
   STA   (SSP),Y
   LDY   #$60
   LDA   (FP),Y
   LDY   #$09
   STA   (SSP),Y
   LDY   #$61
   LDA   (FP),Y
   LDY   #$0A
   STA   (SSP),Y
   LDY   #$62
   LDA   (FP),Y
   LDY   #$0B
   STA   (SSP),Y
   LDY   #$63
   LDA   (FP),Y
   LDY   #$0C
   STA   (SSP),Y
   LDY   #$64
   LDA   (FP),Y
   LDY   #$0D
   STA   (SSP),Y
   LDY   #$65
   LDA   (FP),Y
   LDY   #$0E
   STA   (SSP),Y
   LDY   #$66
   LDA   (FP),Y
   LDY   #$0F
   STA   (SSP),Y
   LDY   #$67
   LDA   (FP),Y
   LDY   #$10
   STA   (SSP),Y
   LDY   #$68
   LDA   (FP),Y
   LDY   #$11
   STA   (SSP),Y
   LDY   #$69
   LDA   (FP),Y
   LDY   #$12
   STA   (SSP),Y
   LDY   #$6A
   LDA   (FP),Y
   LDY   #$13
   STA   (SSP),Y
   LDY   #$6B
   LDA   (FP),Y
   LDY   #$14
   STA   (SSP),Y
   JSR   f5
   LDY   #$6C
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
   LDY   #$6D
   EOR   (FP),Y
   LDY   #$71
   STA   (FP),Y
   LDY   #$0A
   LDA   (FP),Y
   LDY   #$6E
   EOR   (FP),Y
   LDY   #$72
   STA   (FP),Y
   LDY   #$0B
   LDA   (FP),Y
   LDY   #$6F
   EOR   (FP),Y
   LDY   #$73
   STA   (FP),Y
   LDY   #$0C
   LDA   (FP),Y
   LDY   #$70
   EOR   (FP),Y
   LDY   #$74
   STA   (FP),Y
   LDY   #$71
   LDA   (FP),Y
   LDY   #$09
   STA   (FP),Y
   LDY   #$72
   LDA   (FP),Y
   LDY   #$0A
   STA   (FP),Y
   LDY   #$73
   LDA   (FP),Y
   LDY   #$0B
   STA   (FP),Y
   LDY   #$74
   LDA   (FP),Y
   LDY   #$0C
   STA   (FP),Y
   LDY   #$48
   LDA   (FP),Y
   LDY   #$75
   STA   (FP),Y
   LDY   #$49
   LDA   (FP),Y
   LDY   #$76
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@62
   LDA   #$00
   JMP   .sx_done@63
.sx_neg@62:
   LDA   #$FF
.sx_done@63:
   LDY   #$77
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$09
   LDA   (FP),Y
   CLC
   LDY   #$75
   ADC   (FP),Y
   LDY   #$79
   STA   (FP),Y
   LDY   #$0A
   LDA   (FP),Y
   LDY   #$76
   ADC   (FP),Y
   LDY   #$7A
   STA   (FP),Y
   LDY   #$0B
   LDA   (FP),Y
   LDY   #$77
   ADC   (FP),Y
   LDY   #$7B
   STA   (FP),Y
   LDY   #$0C
   LDA   (FP),Y
   LDY   #$78
   ADC   (FP),Y
   LDY   #$7C
   STA   (FP),Y
   LDY   #$79
   LDA   (FP),Y
   LDY   #$09
   STA   (FP),Y
   LDY   #$7A
   LDA   (FP),Y
   LDY   #$0A
   STA   (FP),Y
   LDY   #$7B
   LDA   (FP),Y
   LDY   #$0B
   STA   (FP),Y
   LDY   #$7C
   LDA   (FP),Y
   LDY   #$0C
   STA   (FP),Y
   LDY   #$09
   LDA   (FP),Y
   LDY   #$7D
   STA   (FP),Y
   LDY   #$0A
   LDA   (FP),Y
   LDY   #$7E
   STA   (FP),Y
   LDY   #$0B
   LDA   (FP),Y
   LDY   #$7F
   STA   (FP),Y
   LDY   #$0C
   LDA   (FP),Y
   LDY   #$80
   STA   (FP),Y
   LDA   g0
   CLC
   LDY   #$7D
   ADC   (FP),Y
   LDY   #$81
   STA   (FP),Y
   LDA   g0+1
   LDY   #$7E
   ADC   (FP),Y
   LDY   #$82
   STA   (FP),Y
   LDA   g0+2
   LDY   #$7F
   ADC   (FP),Y
   LDY   #$83
   STA   (FP),Y
   LDA   g0+3
   LDY   #$80
   ADC   (FP),Y
   LDY   #$84
   STA   (FP),Y
   LDY   #$81
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
   LDA   #$01
   SEC
   SBC   #$E8
   LDA   #$00
   SBC   #$03
   BVC   .cmp_novf@64
   EOR   #$80
.cmp_novf@64:
   ASL
   LDA   #$00
   ROL
   LDY   #$85
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$87
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$88
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@65
   LDA   #$00
   JMP   .sx_done@66
.sx_neg@65:
   LDA   #$FF
.sx_done@66:
   LDY   #$89
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
   LDY   #$87
   LDA   (FP),Y
   LDY   #$8F
   STA   (FP),Y
   LDY   #$88
   LDA   (FP),Y
   LDY   #$90
   STA   (FP),Y
   LDY   #$89
   LDA   (FP),Y
   LDY   #$91
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   AND   #$07
   INY
   STA   (FP),Y
   LDY   #$8F
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
   INY
   LDA   (FP),Y
   STA   HARGS+23

   ; epilogue
   CLC
   LDA   FP
   ADC   #$9B
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

   ; prologue: 16 arg bytes, 33 local bytes
   SEC
   LDA   SSP
   SBC   #$23
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$22
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   LDY   #$2C
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   LDY   #$2D
   LDA   (FP),Y
   LDY   #$02
   STA   (FP),Y
   LDY   #$2E
   LDA   (FP),Y
   LDY   #$03
   STA   (FP),Y
   LDY   #$2F
   LDA   (FP),Y
   LDY   #$04
   STA   (FP),Y
   LDY   #$01
   LDA   (FP),Y
   LDY   #$30
   EOR   (FP),Y
   LDY   #$05
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   LDY   #$31
   EOR   (FP),Y
   LDY   #$06
   STA   (FP),Y
   LDY   #$03
   LDA   (FP),Y
   LDY   #$32
   EOR   (FP),Y
   LDY   #$07
   STA   (FP),Y
   LDY   #$04
   LDA   (FP),Y
   LDY   #$33
   EOR   (FP),Y
   LDY   #$08
   STA   (FP),Y
   LDY   #$05
   LDA   (FP),Y
   SEC
   LDY   #$30
   SBC   (FP),Y
   LDY   #$06
   LDA   (FP),Y
   LDY   #$31
   SBC   (FP),Y
   LDY   #$07
   LDA   (FP),Y
   LDY   #$32
   SBC   (FP),Y
   LDY   #$08
   LDA   (FP),Y
   LDY   #$33
   SBC   (FP),Y
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
   LDA   #$00
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
   LDY   #$11
   LDA   (FP),Y
   LDY   #$19
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   JSR   f4
   LDA   #$00
   ROL
   LDY   #$1D
   STA   (FP),Y
   ORA   #$00
   BEQ   .if_else@3
   LDY   #$0B
   LDA   (FP),Y
   CLC
   ADC   #$03
   LDY   #$1E
   STA   (FP),Y
   LDY   #$0C
   LDA   (FP),Y
   ADC   #$00
   LDY   #$1F
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$0B
   STA   (FP),Y
   LDY   #$1F
   LDA   (FP),Y
   LDY   #$0C
   STA   (FP),Y
   JMP   .if_end@2
.if_else@3:
   LDY   #$0B
   LDA   (FP),Y
   EOR   #$05
   LDY   #$20
   STA   (FP),Y
   LDY   #$0C
   LDA   (FP),Y
   LDY   #$21
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$0B
   STA   (FP),Y
   LDY   #$21
   LDA   (FP),Y
   LDY   #$0C
   STA   (FP),Y
.if_end@2:
   LDA   #$00

   ; epilogue
   PHA
   CLC
   LDA   FP
   ADC   #$33
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$22
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

   ; prologue: 0 arg bytes, 102 local bytes
   SEC
   LDA   SSP
   SBC   #$68
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$67
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   LDA   g0
   ORA   g0
   LDY   #$01
   STA   (FP),Y
   LDA   g0+1
   ORA   g0+1
   INY
   STA   (FP),Y
   LDA   g0+2
   ORA   g0+2
   INY
   STA   (FP),Y
   LDA   g0+3
   ORA   g0+3
   INY
   STA   (FP),Y
   LDY   #$01
   LDA   (FP),Y
   EOR   g0
   LDY   #$05
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   EOR   g0+1
   LDY   #$06
   STA   (FP),Y
   LDY   #$03
   LDA   (FP),Y
   EOR   g0+2
   LDY   #$07
   STA   (FP),Y
   LDY   #$04
   LDA   (FP),Y
   EOR   g0+3
   LDY   #$08
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
   AND   #$01
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
   LDA   g1
   INY
   STA   (FP),Y
   LDA   g1+1
   INY
   STA   (FP),Y
   LDA   g1+1
   BMI   .sx_neg@67
   LDA   #$00
   JMP   .sx_done@68
.sx_neg@67:
   LDA   #$FF
.sx_done@68:
   LDY   #$13
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
   LDY   #$11
   LDA   (FP),Y
   LDY   #$19
   STA   (FP),Y
   LDY   #$12
   LDA   (FP),Y
   LDY   #$1A
   STA   (FP),Y
   LDY   #$13
   LDA   (FP),Y
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
   AND   #$07
   INY
   STA   (FP),Y
   LDY   #$19
   LDA   (FP),Y
   LDY   #$21
   STA   (FP),Y
   LDY   #$1A
   LDA   (FP),Y
   LDY   #$22
   STA   (FP),Y
   LDY   #$1B
   LDA   (FP),Y
   LDY   #$23
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$20
   LDA   (FP),Y
   LDY   #$28
   STA   (FP),Y
   LDA   g1
   INY
   STA   (FP),Y
   LDA   g1+1
   INY
   STA   (FP),Y
   LDA   g1+1
   BMI   .sx_neg@69
   LDA   #$00
   JMP   .sx_done@70
.sx_neg@69:
   LDA   #$FF
.sx_done@70:
   LDY   #$2B
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
   LDY   #$29
   LDA   (FP),Y
   LDY   #$31
   STA   (FP),Y
   LDY   #$2A
   LDA   (FP),Y
   LDY   #$32
   STA   (FP),Y
   LDY   #$2B
   LDA   (FP),Y
   LDY   #$33
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   AND   #$07
   INY
   STA   (FP),Y
   LDY   #$31
   LDA   (FP),Y
   SEC
   LDY   #$21
   SBC   (FP),Y
   LDY   #$32
   LDA   (FP),Y
   LDY   #$22
   SBC   (FP),Y
   LDY   #$33
   LDA   (FP),Y
   LDY   #$23
   SBC   (FP),Y
   LDY   #$34
   LDA   (FP),Y
   LDY   #$24
   SBC   (FP),Y
   LDY   #$35
   LDA   (FP),Y
   LDY   #$25
   SBC   (FP),Y
   LDY   #$36
   LDA   (FP),Y
   LDY   #$26
   SBC   (FP),Y
   LDY   #$37
   LDA   (FP),Y
   LDY   #$27
   SBC   (FP),Y
   LDY   #$38
   LDA   (FP),Y
   LDY   #$28
   SBC   (FP),Y
   LDA   #$00
   ROL
   LDY   #$39
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$3B
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$3C
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@71
   LDA   #$00
   JMP   .sx_done@72
.sx_neg@71:
   LDA   #$FF
.sx_done@72:
   LDY   #$3D
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
   LDY   #$3B
   LDA   (FP),Y
   LDY   #$43
   STA   (FP),Y
   LDY   #$3C
   LDA   (FP),Y
   LDY   #$44
   STA   (FP),Y
   LDY   #$3D
   LDA   (FP),Y
   LDY   #$45
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   AND   #$07
   INY
   STA   (FP),Y
   LDA   #$03
   LDY   #$21
   ORA   (FP),Y
   LDY   #$4B
   STA   (FP),Y
   LDY   #$22
   LDA   (FP),Y
   LDY   #$4C
   STA   (FP),Y
   LDY   #$23
   LDA   (FP),Y
   LDY   #$4D
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$28
   LDA   (FP),Y
   LDY   #$52
   STA   (FP),Y
   LDY   #$43
   LDA   (FP),Y
   LDY   #$4B
   AND   (FP),Y
   LDY   #$53
   STA   (FP),Y
   LDY   #$44
   LDA   (FP),Y
   LDY   #$4C
   AND   (FP),Y
   LDY   #$54
   STA   (FP),Y
   LDY   #$45
   LDA   (FP),Y
   LDY   #$4D
   AND   (FP),Y
   LDY   #$55
   STA   (FP),Y
   LDY   #$46
   LDA   (FP),Y
   LDY   #$4E
   AND   (FP),Y
   LDY   #$56
   STA   (FP),Y
   LDY   #$47
   LDA   (FP),Y
   LDY   #$4F
   AND   (FP),Y
   LDY   #$57
   STA   (FP),Y
   LDY   #$48
   LDA   (FP),Y
   LDY   #$50
   AND   (FP),Y
   LDY   #$58
   STA   (FP),Y
   LDY   #$49
   LDA   (FP),Y
   LDY   #$51
   AND   (FP),Y
   LDY   #$59
   STA   (FP),Y
   LDY   #$4A
   LDA   (FP),Y
   LDY   #$52
   AND   (FP),Y
   LDY   #$5A
   STA   (FP),Y
   LDY   #$53
   LDA   (FP),Y
   LDY   #$5B
   STA   (FP),Y
   LDY   #$54
   LDA   (FP),Y
   LDY   #$5C
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$5D
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$5E
   STA   (FP),Y
   LDA   #$00
   LDY   #$10
   CMP   (FP),Y
   BNE   .cmp_true@73
   DEY
   CMP   (FP),Y
   BNE   .cmp_true@73
   DEY
   CMP   (FP),Y
   BNE   .cmp_true@73
   LDA   #$02
   DEY
   CMP   (FP),Y
   BNE   .cmp_true@73
   LDA   #$00
   JMP   .cmp_end@74
.cmp_true@73:
   LDA   #$01
.cmp_end@74:
   LDY   #$5F
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   SEC
   SBC   g1
   LDY   #$61
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   SBC   g1+1
   LDY   #$62
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$63
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$64
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@76
   LDA   #$00
   JMP   .sx_done@77
.sx_neg@76:
   LDA   #$FF
.sx_done@77:
   LDY   #$65
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$63
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
   ADC   #$68
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

   JSR   f0
   LDA   HARGS+8
   LDY   #$01
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
   LDY   #$04
   LDA   (FP),Y
   LDY   #$08
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
   LDA   g1+1
   BMI   .sx_neg@78
   LDA   #$00
   JMP   .sx_done@79
.sx_neg@78:
   LDA   #$FF
.sx_done@79:
   LDY   #$0F
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$09
   LDA   (FP),Y
   LDY   #$0D
   EOR   (FP),Y
   LDY   #$11
   STA   (FP),Y
   LDY   #$0A
   LDA   (FP),Y
   LDY   #$0E
   EOR   (FP),Y
   LDY   #$12
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
   LDY   #$05
   LDA   (FP),Y
   LDY   #$11
   EOR   (FP),Y
   LDY   #$15
   STA   (FP),Y
   LDY   #$06
   LDA   (FP),Y
   LDY   #$12
   EOR   (FP),Y
   LDY   #$16
   STA   (FP),Y
   LDY   #$07
   LDA   (FP),Y
   LDY   #$13
   EOR   (FP),Y
   LDY   #$17
   STA   (FP),Y
   LDY   #$08
   LDA   (FP),Y
   LDY   #$14
   EOR   (FP),Y
   LDY   #$18
   STA   (FP),Y
   LDY   #$15
   LDA   (FP),Y
   LDY   #$05
   STA   (FP),Y
   LDY   #$16
   LDA   (FP),Y
   LDY   #$06
   STA   (FP),Y
   LDY   #$17
   LDA   (FP),Y
   LDY   #$07
   STA   (FP),Y
   LDY   #$18
   LDA   (FP),Y
   LDY   #$08
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
   LDA   #$10
   STA   HARGS+4
   JSR   asr32
   LDA   HARGS+5
   LDY   #$19
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
   LDY   #$05
   LDA   (FP),Y
   LDY   #$19
   EOR   (FP),Y
   LDY   #$1D
   STA   (FP),Y
   LDY   #$06
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
   LDY   #$1D
   LDA   (FP),Y
   LDY   #$21
   STA   (FP),Y
   LDY   #$1E
   LDA   (FP),Y
   LDY   #$22
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

g0:
   DC.L  $0000004A

g1:
   DC.W  $00C4
