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

f2:
   SUBROUTINE

   ; prologue: 6 arg bytes, 97 local bytes
   SEC
   LDA   SSP
   SBC   #$63
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$62
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
   AND   #$07
   LDY   #$01
   STA   (FP),Y
   LDY   #$65
   LDA   (FP),Y
   LDA   #$00
   LDY   #$02
   STA   (FP),Y
   LDY   #$64
   LDA   (FP),Y
   LDY   #$03
   STA   (FP),Y
   LDY   #$65
   LDA   (FP),Y
   LDY   #$04
   STA   (FP),Y
   LDY   #$01
   LDA   (FP),Y
   TAX
   BEQ   .shift_done@8
.shift_loop@7:
   LDY   #$04
   LDA   (FP),Y
   LSR
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ROR
   STA   (FP),Y
   DEX
   BNE   .shift_loop@7
.shift_done@8:
   SEC
   LDA   #$00
   LDY   #$03
   SBC   (FP),Y
   LDY   #$05
   STA   (FP),Y
   LDA   #$00
   DEY
   SBC   (FP),Y
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
   LDA   #$00
   INY
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
   SEC
   SBC   g0
   LDA   g0+1
   SBC   g0+1
   LDA   g0+2
   SBC   g0+2
   LDA   g0+3
   SBC   g0+3
   LDA   #$00
   ROL
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   AND   #$07
   LDY   #$11
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDA   #$00
   LDY   #$12
   STA   (FP),Y
   LDA   g1
   INY
   STA   (FP),Y
   LDA   g1+1
   INY
   STA   (FP),Y
   LDY   #$11
   LDA   (FP),Y
   TAX
   BEQ   .shift_done@10
.shift_loop@9:
   LDY   #$13
   LDA   (FP),Y
   ASL
   STA   (FP),Y
   INY
   LDA   (FP),Y
   ROL
   STA   (FP),Y
   DEX
   BNE   .shift_loop@9
.shift_done@10:
   LDY   #$13
   LDA   (FP),Y
   LDY   #$15
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$16
   STA   (FP),Y
   LDA   #$00
   INY
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
   LDY   #$64
   LDA   (FP),Y
   LDY   #$1D
   STA   (FP),Y
   LDY   #$65
   LDA   (FP),Y
   LDY   #$1E
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$0B
   LDA   (FP),Y
   LDY   #$1D
   AND   (FP),Y
   LDY   #$21
   STA   (FP),Y
   LDY   #$0C
   LDA   (FP),Y
   LDY   #$1E
   AND   (FP),Y
   LDY   #$22
   STA   (FP),Y
   LDY   #$0D
   LDA   (FP),Y
   LDY   #$1F
   AND   (FP),Y
   LDY   #$23
   STA   (FP),Y
   LDY   #$0E
   LDA   (FP),Y
   LDY   #$20
   AND   (FP),Y
   LDY   #$24
   STA   (FP),Y
   LDY   #$19
   LDA   (FP),Y
   LDY   #$25
   STA   (FP),Y
   INY
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@11
   LDA   #$00
   JMP   .sx_done@12
.sx_neg@11:
   LDA   #$FF
.sx_done@12:
   LDY   #$27
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$28
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$29
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@13
   LDA   #$00
   JMP   .sx_done@14
.sx_neg@13:
   LDA   #$FF
.sx_done@14:
   LDY   #$2A
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$21
   LDA   (FP),Y
   SEC
   LDY   #$28
   SBC   (FP),Y
   LDY   #$2C
   STA   (FP),Y
   LDY   #$22
   LDA   (FP),Y
   LDY   #$29
   SBC   (FP),Y
   LDY   #$2D
   STA   (FP),Y
   LDY   #$23
   LDA   (FP),Y
   LDY   #$2A
   SBC   (FP),Y
   LDY   #$2E
   STA   (FP),Y
   LDY   #$24
   LDA   (FP),Y
   LDY   #$2B
   SBC   (FP),Y
   LDY   #$2F
   STA   (FP),Y
   LDY   #$2C
   LDA   (FP),Y
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
   LDY   #$2F
   LDA   (FP),Y
   LDY   #$33
   STA   (FP),Y
   LDY   #$30
   LDA   (FP),Y
   LDY   #$34
   STA   (FP),Y
   LDY   #$31
   LDA   (FP),Y
   LDY   #$35
   STA   (FP),Y
   LDY   #$32
   LDA   (FP),Y
   LDY   #$36
   STA   (FP),Y
   LDY   #$33
   LDA   (FP),Y
   LDY   #$37
   STA   (FP),Y
   LDY   #$66
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
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$38
   LDA   (FP),Y
   LDY   #$3C
   STA   (FP),Y
   LDY   #$39
   LDA   (FP),Y
   LDY   #$3D
   STA   (FP),Y
   LDY   #$3A
   LDA   (FP),Y
   TAX
   BEQ   .shift_done@18
.shift_loop@17:
   LDY   #$3D
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
   BNE   .shift_loop@17
.shift_done@18:
   LDA   #$00
   STA   HARGS
   STA   HARGS+1
   LDY   #$3C
   LDA   (FP),Y
   STA   HARGS+2
   INY
   LDA   (FP),Y
   STA   HARGS+3
   JSR   mul16
   LDA   HARGS+4
   LDY   #$3E
   STA   (FP),Y
   LDA   HARGS+5
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$40
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$41
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@19
   LDA   #$00
   JMP   .sx_done@20
.sx_neg@19:
   LDA   #$FF
.sx_done@20:
   LDY   #$42
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$40
   LDA   (FP),Y
   LDY   #$44
   STA   (FP),Y
   LDY   #$41
   LDA   (FP),Y
   LDY   #$45
   STA   (FP),Y
   LDY   #$42
   LDA   (FP),Y
   LDY   #$46
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$68
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$48
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$49
   STA   (FP),Y
   LDY   #$67
   LDA   (FP),Y
   LDY   #$4A
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@21
   LDA   #$00
   JMP   .sx_done@22
.sx_neg@21:
   LDA   #$FF
.sx_done@22:
   LDY   #$4B
   STA   (FP),Y
   LDY   #$48
   LDA   (FP),Y
   CLC
   LDY   #$4A
   ADC   (FP),Y
   LDY   #$4C
   STA   (FP),Y
   LDY   #$49
   LDA   (FP),Y
   LDY   #$4B
   ADC   (FP),Y
   LDY   #$4D
   STA   (FP),Y
   LDY   #$68
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$4C
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$4D
   LDA   (FP),Y
   LDY   #$01
   STA   (DPTR),Y
   LDY   #$68
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$4E
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$4F
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$50
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$51
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@23
   LDA   #$00
   JMP   .sx_done@24
.sx_neg@23:
   LDA   #$FF
.sx_done@24:
   LDY   #$52
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$44
   LDA   (FP),Y
   SEC
   LDY   #$50
   SBC   (FP),Y
   LDY   #$54
   STA   (FP),Y
   LDY   #$45
   LDA   (FP),Y
   LDY   #$51
   SBC   (FP),Y
   LDY   #$55
   STA   (FP),Y
   LDY   #$46
   LDA   (FP),Y
   LDY   #$52
   SBC   (FP),Y
   LDY   #$56
   STA   (FP),Y
   LDY   #$47
   LDA   (FP),Y
   LDY   #$53
   SBC   (FP),Y
   LDY   #$57
   STA   (FP),Y
   LDY   #$19
   LDA   (FP),Y
   LDY   #$58
   STA   (FP),Y
   LDY   #$1A
   LDA   (FP),Y
   LDY   #$59
   STA   (FP),Y
   LDY   #$1B
   LDA   (FP),Y
   LDY   #$5A
   STA   (FP),Y
   LDY   #$1C
   LDA   (FP),Y
   LDY   #$5B
   STA   (FP),Y
   LDY   #$54
   LDA   (FP),Y
   LDY   #$58
   EOR   (FP),Y
   LDY   #$5C
   STA   (FP),Y
   LDY   #$55
   LDA   (FP),Y
   LDY   #$59
   EOR   (FP),Y
   LDY   #$5D
   STA   (FP),Y
   LDY   #$56
   LDA   (FP),Y
   LDY   #$5A
   EOR   (FP),Y
   LDY   #$5E
   STA   (FP),Y
   LDY   #$57
   LDA   (FP),Y
   LDY   #$5B
   EOR   (FP),Y
   LDY   #$5F
   STA   (FP),Y
   LDY   #$5C
   LDA   (FP),Y
   LDY   #$60
   STA   (FP),Y
   LDY   #$5D
   LDA   (FP),Y
   LDY   #$61
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
   ADC   #$69
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$62
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

f1:
   SUBROUTINE

   ; prologue: 9 arg bytes, 23 local bytes
   SEC
   LDA   SSP
   SBC   #$19
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$18
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
   LDA   g1
   INY
   STA   (FP),Y
   LDA   g1+1
   INY
   STA   (FP),Y
   LDA   #$00
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
   LDY   #$05
   LDA   (FP),Y
   LDY   #$09
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$1D
   CMP   (FP),Y
   BNE   .cmp_true@25
   DEY
   CMP   (FP),Y
   BNE   .cmp_true@25
   LDY   #$08
   LDA   (FP),Y
   LDY   #$1B
   CMP   (FP),Y
   BNE   .cmp_true@25
   LDY   #$07
   LDA   (FP),Y
   LDY   #$1A
   CMP   (FP),Y
   BNE   .cmp_true@25
   LDA   #$00
   JMP   .cmp_end@26
.cmp_true@25:
   LDA   #$01
.cmp_end@26:
   LDY   #$0B
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$0D
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$0E
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@28
   LDA   #$00
   JMP   .sx_done@29
.sx_neg@28:
   LDA   #$FF
.sx_done@29:
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
   LDA   g0
   INY
   STA   (FP),Y
   LDA   g0+1
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$17
   STA   (FP),Y

   ; epilogue
   PHA
   CLC
   LDA   FP
   ADC   #$22
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

   ; prologue: 11 arg bytes, 108 local bytes
   SEC
   LDA   SSP
   SBC   #$6E
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$6D
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
   DEY
   LDA   (FP),Y
   LDY   #$03
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$04
   STA   (FP),Y
   LDY   #$70
   LDA   (FP),Y
   EOR   #$FF
   LDY   #$05
   STA   (FP),Y
   LDY   #$71
   LDA   (FP),Y
   EOR   #$FF
   LDY   #$06
   STA   (FP),Y
   LDY   #$72
   LDA   (FP),Y
   EOR   #$FF
   LDY   #$07
   STA   (FP),Y
   LDY   #$73
   LDA   (FP),Y
   EOR   #$FF
   LDY   #$08
   STA   (FP),Y
   LDY   #$6F
   LDA   (FP),Y
   LDY   #$09
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$03
   LDA   (FP),Y
   ORA   #$01
   LDY   #$0B
   STA   (FP),Y
   LDY   #$04
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
   JSR   sdivmod16
   LDA   HARGS+6
   LDY   #$0D
   STA   (FP),Y
   LDA   HARGS+7
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
   BMI   .sx_neg@30
   LDA   #$00
   JMP   .sx_done@31
.sx_neg@30:
   LDA   #$FF
.sx_done@31:
   LDY   #$11
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$08
   LDA   (FP),Y
   LDY   #$12
   CMP   (FP),Y
   BNE   .thread@0
   LDY   #$07
   LDA   (FP),Y
   LDY   #$11
   CMP   (FP),Y
   BNE   .thread@0
   LDY   #$06
   LDA   (FP),Y
   LDY   #$10
   CMP   (FP),Y
   BNE   .thread@0
   LDY   #$05
   LDA   (FP),Y
   LDY   #$0F
   CMP   (FP),Y
   BEQ   .cmp_true@32
.thread@0:
   LDA   #$00
   JMP   .cmp_end@33
.cmp_true@32:
   LDA   #$01
.cmp_end@33:
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
   LDA   #$07
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   ORA   #$00
   BPL   .sx_done@36
   LDA   #$FF
.sx_done@36:
   LDY   #$19
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$70
   LDA   (FP),Y
   LDY   #$17
   AND   (FP),Y
   LDY   #$1B
   STA   (FP),Y
   LDY   #$71
   LDA   (FP),Y
   LDY   #$18
   AND   (FP),Y
   LDY   #$1C
   STA   (FP),Y
   LDY   #$72
   LDA   (FP),Y
   LDY   #$19
   AND   (FP),Y
   LDY   #$1D
   STA   (FP),Y
   LDY   #$73
   LDA   (FP),Y
   LDY   #$1A
   AND   (FP),Y
   LDY   #$1E
   STA   (FP),Y
   LDY   #$03
   LDA   (FP),Y
   LDY   #$1F
   STA   (FP),Y
   LDY   #$04
   LDA   (FP),Y
   LDY   #$20
   STA   (FP),Y
   LDY   #$1B
   LDA   (FP),Y
   TAX
   BEQ   .shift_done@38
.shift_loop@37:
   LDY   #$20
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
   BNE   .shift_loop@37
.shift_done@38:
   LDY   #$15
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lnot_true@39
   LDA   #$00
   JMP   .lnot_end@40
.lnot_true@39:
   LDA   #$01
.lnot_end@40:
   LDY   #$21
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$1F
   LDA   (FP),Y
   SEC
   LDY   #$21
   SBC   (FP),Y
   LDY   #$23
   STA   (FP),Y
   LDY   #$20
   LDA   (FP),Y
   LDY   #$22
   SBC   (FP),Y
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
   ORA   #$00
   BMI   .sx_neg@41
   LDA   #$00
   JMP   .sx_done@42
.sx_neg@41:
   LDA   #$FF
.sx_done@42:
   LDY   #$27
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
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
.loop@0_start:
   LDY   #$2D
   LDA   (FP),Y
   SEC
   SBC   #$03
   INY
   LDA   (FP),Y
   SBC   #$00
   BVC   .cmp_novf@43
   EOR   #$80
.cmp_novf@43:
   ASL
   LDA   #$00
   ROL
   LDY   #$2F
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
   LDY   #$29
   LDA   (FP),Y
   LDY   #$31
   STA   (FP),Y
   LDY   #$2A
   LDA   (FP),Y
   LDY   #$32
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$33
   STA   (FP),Y
   LDY   #$03
   LDA   (FP),Y
   LDY   #$34
   STA   (FP),Y
   LDY   #$04
   LDA   (FP),Y
   LDY   #$35
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@44
   LDA   #$00
   JMP   .sx_done@45
.sx_neg@44:
   LDA   #$FF
.sx_done@45:
   LDY   #$36
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   g0+3
   CMP   (FP),Y
   BNE   .thread@1
   LDA   g0+2
   DEY
   CMP   (FP),Y
   BNE   .thread@1
   LDA   g0+1
   DEY
   CMP   (FP),Y
   BNE   .thread@1
   LDA   g0
   DEY
   CMP   (FP),Y
   BEQ   .cmp_true@46
.thread@1:
   LDA   #$00
   JMP   .cmp_end@47
.cmp_true@46:
   LDA   #$01
.cmp_end@47:
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
   ORA   #$00
   BMI   .sx_neg@49
   LDA   #$00
   JMP   .sx_done@50
.sx_neg@49:
   LDA   #$FF
.sx_done@50:
   LDY   #$3C
   STA   (FP),Y
   INY
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$09
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDY   #$70
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$71
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$72
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$73
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   LDY   #$33
   LDA   (FP),Y
   LDY   #$05
   STA   (SSP),Y
   LDY   #$3A
   LDA   (FP),Y
   LDY   #$06
   STA   (SSP),Y
   LDY   #$3B
   LDA   (FP),Y
   LDY   #$07
   STA   (SSP),Y
   LDY   #$3C
   LDA   (FP),Y
   LDY   #$08
   STA   (SSP),Y
   LDY   #$3D
   LDA   (FP),Y
   LDY   #$09
   STA   (SSP),Y
   JSR   f1
   LDY   #$3E
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   LDY   #$2D
   ADC   (FP),Y
   LDY   #$41
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$2E
   ADC   (FP),Y
   LDY   #$42
   STA   (FP),Y
   LDY   #$15
   LDA   (FP),Y
   CLC
   LDY   #$41
   ADC   (FP),Y
   LDY   #$43
   STA   (FP),Y
   LDY   #$16
   LDA   (FP),Y
   LDY   #$42
   ADC   (FP),Y
   LDY   #$44
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$15
   STA   (FP),Y
   LDY   #$44
   LDA   (FP),Y
   LDY   #$16
   STA   (FP),Y
   LDY   #$2D
   LDA   (FP),Y
   LDY   #$45
   STA   (FP),Y
   LDY   #$2E
   LDA   (FP),Y
   LDY   #$46
   STA   (FP),Y
   LDY   #$2D
   LDA   (FP),Y
   CLC
   ADC   #$01
   LDY   #$47
   STA   (FP),Y
   LDY   #$2E
   LDA   (FP),Y
   ADC   #$00
   LDY   #$48
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$2D
   STA   (FP),Y
   LDY   #$48
   LDA   (FP),Y
   LDY   #$2E
   STA   (FP),Y
   JMP   .loop@0_start
.loop@0_break:
   LDY   #$15
   LDA   (FP),Y
   LDY   #$49
   STA   (FP),Y
   LDY   #$16
   LDA   (FP),Y
   LDY   #$4A
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@51
   LDA   #$00
   JMP   .sx_done@52
.sx_neg@51:
   LDA   #$FF
.sx_done@52:
   LDY   #$4B
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   g0
   CLC
   LDY   #$49
   ADC   (FP),Y
   LDY   #$4D
   STA   (FP),Y
   LDA   g0+1
   LDY   #$4A
   ADC   (FP),Y
   LDY   #$4E
   STA   (FP),Y
   LDA   g0+2
   LDY   #$4B
   ADC   (FP),Y
   LDY   #$4F
   STA   (FP),Y
   LDA   g0+3
   LDY   #$4C
   ADC   (FP),Y
   LDY   #$50
   STA   (FP),Y
   LDY   #$4D
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
   LDY   #$74
   LDA   (FP),Y
   LDY   #$70
   EOR   (FP),Y
   LDY   #$51
   STA   (FP),Y
   LDY   #$75
   LDA   (FP),Y
   LDY   #$71
   EOR   (FP),Y
   LDY   #$52
   STA   (FP),Y
   LDY   #$76
   LDA   (FP),Y
   LDY   #$72
   EOR   (FP),Y
   LDY   #$53
   STA   (FP),Y
   LDY   #$77
   LDA   (FP),Y
   LDY   #$73
   EOR   (FP),Y
   LDY   #$54
   STA   (FP),Y
   LDY   #$51
   LDA   (FP),Y
   LDY   #$55
   STA   (FP),Y
   LDY   #$52
   LDA   (FP),Y
   LDY   #$56
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
   LDY   #$55
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$56
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   JSR   rec
   LDY   #$57
   STA   (FP),Y
   TXA
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$59
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$5A
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@53
   LDA   #$00
   JMP   .sx_done@54
.sx_neg@53:
   LDA   #$FF
.sx_done@54:
   LDY   #$5B
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$29
   LDA   (FP),Y
   CLC
   LDY   #$59
   ADC   (FP),Y
   LDY   #$5D
   STA   (FP),Y
   LDY   #$2A
   LDA   (FP),Y
   LDY   #$5A
   ADC   (FP),Y
   LDY   #$5E
   STA   (FP),Y
   LDY   #$2B
   LDA   (FP),Y
   LDY   #$5B
   ADC   (FP),Y
   LDY   #$5F
   STA   (FP),Y
   LDY   #$2C
   LDA   (FP),Y
   LDY   #$5C
   ADC   (FP),Y
   LDY   #$60
   STA   (FP),Y
   LDY   #$5D
   LDA   (FP),Y
   LDY   #$29
   STA   (FP),Y
   LDY   #$5E
   LDA   (FP),Y
   LDY   #$2A
   STA   (FP),Y
   LDY   #$5F
   LDA   (FP),Y
   LDY   #$2B
   STA   (FP),Y
   LDY   #$60
   LDA   (FP),Y
   LDY   #$2C
   STA   (FP),Y
   LDY   #$78
   LDA   (FP),Y
   LDY   #$61
   STA   (FP),Y
   LDA   #$7F
   LDY   #$79
   AND   (FP),Y
   LDY   #$62
   STA   (FP),Y
   LDA   #$FF
   CMP   #$FF
   BEQ   .cmp_true@55
   LDA   #$00
   JMP   .cmp_end@56
.cmp_true@55:
   LDA   #$01
.cmp_end@56:
   LDY   #$63
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$65
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$66
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   LDY   #$61
   SBC   (FP),Y
   LDY   #$66
   LDA   (FP),Y
   LDY   #$62
   SBC   (FP),Y
   LDA   #$00
   ROL
   LDY   #$67
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$69
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$6A
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@57
   LDA   #$00
   JMP   .sx_done@58
.sx_neg@57:
   LDA   #$FF
.sx_done@58:
   LDY   #$6B
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$69
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

   LDA   #$3C
   LDY   #$01
   STA   (FP),Y
   LDA   #$61
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   ORA   #$00
   BPL   .sx_done@60
   LDA   #$FF
.sx_done@60:
   LDY   #$04
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$42
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   ORA   #$00
   BPL   .sx_done@62
   LDA   #$FF
.sx_done@62:
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
   INY
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
   INY
   LDA   (FP),Y
   STA   (SSP),Y
   LDA   #$A6
   INY
   STA   (SSP),Y
   LDA   #$00
   INY
   STA   (SSP),Y
   JSR   f0
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
   LDA   #$00
   INY
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
   LDY   #$12
   LDA   (FP),Y
   LDY   #$1E
   EOR   (FP),Y
   LDY   #$22
   STA   (FP),Y
   LDY   #$13
   LDA   (FP),Y
   LDY   #$1F
   EOR   (FP),Y
   LDY   #$23
   STA   (FP),Y
   LDY   #$14
   LDA   (FP),Y
   LDY   #$20
   EOR   (FP),Y
   LDY   #$24
   STA   (FP),Y
   LDY   #$15
   LDA   (FP),Y
   LDY   #$21
   EOR   (FP),Y
   LDY   #$25
   STA   (FP),Y
   LDY   #$22
   LDA   (FP),Y
   LDY   #$12
   STA   (FP),Y
   LDY   #$23
   LDA   (FP),Y
   LDY   #$13
   STA   (FP),Y
   LDY   #$24
   LDA   (FP),Y
   LDY   #$14
   STA   (FP),Y
   LDY   #$25
   LDA   (FP),Y
   LDY   #$15
   STA   (FP),Y
   LDY   #$12
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
   LDY   #$12
   LDA   (FP),Y
   LDY   #$26
   EOR   (FP),Y
   LDY   #$2A
   STA   (FP),Y
   LDY   #$13
   LDA   (FP),Y
   LDY   #$27
   EOR   (FP),Y
   LDY   #$2B
   STA   (FP),Y
   LDY   #$14
   LDA   (FP),Y
   LDY   #$28
   EOR   (FP),Y
   LDY   #$2C
   STA   (FP),Y
   LDY   #$15
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
   DC.L  $00000075

g1:
   DC.W  $0022
