SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24

check:
   SUBROUTINE

   ; prologue: 20 arg bytes, 131 local bytes
   SEC
   LDA   SSP
   SBC   #$85
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$84
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
   LDA   #$FF
   BEQ   .lnot_true@0
   LDA   #$00
   JMP   .lnot_end@1
.lnot_true@0:
   LDA   #$01
.lnot_end@1:
   LDY   #$03
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$97
   LDA   (FP),Y
   LDY   #$05
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@2
   LDA   #$00
   JMP   .sx_done@3
.sx_neg@2:
   LDA   #$FF
.sx_done@3:
   LDY   #$06
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   SEC
   LDY   #$94
   SBC   (FP),Y
   LDY   #$06
   LDA   (FP),Y
   LDY   #$95
   SBC   (FP),Y
   BVC   .cmp_novf@4
   EOR   #$80
.cmp_novf@4:
   ASL
   LDA   #$00
   ROL
   LDY   #$07
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$94
   LDA   (FP),Y
   LDY   #$09
   STA   (FP),Y
   LDY   #$95
   LDA   (FP),Y
   LDY   #$0A
   STA   (FP),Y
   LDY   #$07
   LDA   (FP),Y
   TAX
   BEQ   .shift_done@6
.shift_loop@5:
   LDY   #$0A
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
   BNE   .shift_loop@5
.shift_done@6:
   LDY   #$09
   LDA   (FP),Y
   SEC
   LDY   #$03
   SBC   (FP),Y
   LDY   #$0A
   LDA   (FP),Y
   LDY   #$04
   SBC   (FP),Y
   BVC   .cmp_novf@7
   EOR   #$80
.cmp_novf@7:
   ASL
   LDA   #$00
   ROL
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
   CMP   #$00
   BNE   .cmp_true@8
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@8
   LDA   #$00
   JMP   .cmp_end@9
.cmp_true@8:
   LDA   #$01
.cmp_end@9:
   LDY   #$0F
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@0
   LDY   #$01
   LDA   (FP),Y
   ORA   #$01
   LDY   #$11
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   LDY   #$12
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   LDY   #$12
   LDA   (FP),Y
   LDY   #$02
   STA   (FP),Y
.if_end@0:
   LDY   #$96
   LDA   (FP),Y
   LDY   #$13
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDA   #$04
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$14
   LDA   (FP),Y
   LDY   #$17
   CMP   (FP),Y
   BNE   .cmp_true@11
   LDA   #$00
   JMP   .cmp_end@12
.cmp_true@11:
   LDA   #$01
.cmp_end@12:
   LDY   #$19
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@1
   LDY   #$01
   LDA   (FP),Y
   ORA   #$02
   LDY   #$1B
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   LDY   #$1C
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   LDY   #$1C
   LDA   (FP),Y
   LDY   #$02
   STA   (FP),Y
.if_end@1:
   LDY   #$86
   LDA   (FP),Y
   LDY   #$1D
   STA   (FP),Y
   LDY   #$87
   LDA   (FP),Y
   LDY   #$1E
   STA   (FP),Y
   LDY   #$88
   LDA   (FP),Y
   LDY   #$1F
   STA   (FP),Y
   LDY   #$89
   LDA   (FP),Y
   LDY   #$20
   STA   (FP),Y
   LDY   #$8A
   LDA   (FP),Y
   LDY   #$21
   STA   (FP),Y
   LDY   #$8B
   LDA   (FP),Y
   LDY   #$22
   STA   (FP),Y
   LDY   #$8C
   LDA   (FP),Y
   LDY   #$23
   STA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@13
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@13
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@13
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@13
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@13
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@13
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@13
   LDA   #$00
   JMP   .cmp_end@14
.cmp_true@13:
   LDA   #$01
.cmp_end@14:
   LDY   #$24
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@2
   LDY   #$01
   LDA   (FP),Y
   ORA   #$04
   LDY   #$26
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   LDY   #$27
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   LDY   #$27
   LDA   (FP),Y
   LDY   #$02
   STA   (FP),Y
.if_end@2:
   LDY   #$97
   LDA   (FP),Y
   LDY   #$28
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@16
   LDA   #$00
   JMP   .sx_done@17
.sx_neg@16:
   LDA   #$FF
.sx_done@17:
   LDY   #$29
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$2A
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$2B
   STA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@18
   DEY
   LDA   (FP),Y
   CMP   #$01
   BNE   .cmp_true@18
   LDA   #$00
   JMP   .cmp_end@19
.cmp_true@18:
   LDA   #$01
.cmp_end@19:
   LDY   #$2C
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@3
   LDY   #$01
   LDA   (FP),Y
   ORA   #$08
   LDY   #$2E
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   LDY   #$2F
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   LDY   #$2F
   LDA   (FP),Y
   LDY   #$02
   STA   (FP),Y
.if_end@3:
   LDY   #$98
   LDA   (FP),Y
   LDY   #$30
   STA   (FP),Y
   LDY   #$99
   LDA   (FP),Y
   LDY   #$31
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
   LDA   #$FF
   EOR   #$02
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
   INY
   STA   (FP),Y
   LDY   #$86
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
   LDY   #$37
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
   LDY   #$3E
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
   LDY   #$30
   LDA   (FP),Y
   LDY   #$3E
   AND   (FP),Y
   LDY   #$45
   STA   (FP),Y
   LDY   #$31
   LDA   (FP),Y
   LDY   #$3F
   AND   (FP),Y
   LDY   #$46
   STA   (FP),Y
   LDY   #$32
   LDA   (FP),Y
   LDY   #$40
   AND   (FP),Y
   LDY   #$47
   STA   (FP),Y
   LDY   #$33
   LDA   (FP),Y
   LDY   #$41
   AND   (FP),Y
   LDY   #$48
   STA   (FP),Y
   LDY   #$34
   LDA   (FP),Y
   LDY   #$42
   AND   (FP),Y
   LDY   #$49
   STA   (FP),Y
   LDY   #$35
   LDA   (FP),Y
   LDY   #$43
   AND   (FP),Y
   LDY   #$4A
   STA   (FP),Y
   LDY   #$36
   LDA   (FP),Y
   LDY   #$44
   AND   (FP),Y
   LDY   #$4B
   STA   (FP),Y
   LDY   #$45
   LDA   (FP),Y
   LDY   #$4C
   STA   (FP),Y
   LDY   #$46
   LDA   (FP),Y
   LDY   #$4D
   STA   (FP),Y
   LDY   #$47
   LDA   (FP),Y
   LDY   #$4E
   STA   (FP),Y
   LDY   #$48
   LDA   (FP),Y
   LDY   #$4F
   STA   (FP),Y
   LDY   #$49
   LDA   (FP),Y
   LDY   #$50
   STA   (FP),Y
   LDY   #$4A
   LDA   (FP),Y
   LDY   #$51
   STA   (FP),Y
   LDY   #$4B
   LDA   (FP),Y
   LDY   #$52
   STA   (FP),Y
   LDY   #$4D
   LDA   (FP),Y
   BNE   .cmp_true@21
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@21
   LDA   #$00
   JMP   .cmp_end@22
.cmp_true@21:
   LDA   #$01
.cmp_end@22:
   LDY   #$53
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@4
   LDY   #$01
   LDA   (FP),Y
   ORA   #$10
   LDY   #$55
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   LDY   #$56
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   LDY   #$56
   LDA   (FP),Y
   LDY   #$02
   STA   (FP),Y
.if_end@4:
   LDY   #$86
   LDA   (FP),Y
   LDY   #$57
   STA   (FP),Y
   LDY   #$87
   LDA   (FP),Y
   LDY   #$58
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$59
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$5A
   STA   (FP),Y
   LDY   #$94
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@5
   INY
   LDA   (FP),Y
   BEQ   .and_false@5
   LDA   #$01
   LDY   #$5B
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   JMP   .and_end@6
.and_false@5:
   LDA   #$00
   LDY   #$5B
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@6:
   LDY   #$98
   LDA   (FP),Y
   LDY   #$5D
   STA   (FP),Y
   LDY   #$99
   LDA   (FP),Y
   LDY   #$5E
   STA   (FP),Y
   LDY   #$5B
   LDA   (FP),Y
   TAX
   BEQ   .shift_done@25
.shift_loop@24:
   LDY   #$5D
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
   LDY   #$59
   LDA   (FP),Y
   LDY   #$5D
   ORA   (FP),Y
   LDY   #$5F
   STA   (FP),Y
   LDY   #$5A
   LDA   (FP),Y
   LDY   #$5E
   ORA   (FP),Y
   LDY   #$60
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$61
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$62
   STA   (FP),Y
   CMP   #$42
   BNE   .cmp_true@26
   DEY
   LDA   (FP),Y
   CMP   #$22
   BNE   .cmp_true@26
   LDA   #$00
   JMP   .cmp_end@27
.cmp_true@26:
   LDA   #$01
.cmp_end@27:
   LDY   #$63
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@7
   LDY   #$01
   LDA   (FP),Y
   ORA   #$20
   LDY   #$65
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   LDY   #$66
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   LDY   #$66
   LDA   (FP),Y
   LDY   #$02
   STA   (FP),Y
.if_end@7:
   LDA   #$01
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
   CMP   #$01
   BNE   .cmp_true@29
   LDA   #$00
   JMP   .cmp_end@30
.cmp_true@29:
   LDA   #$01
.cmp_end@30:
   LDY   #$6E
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@8
   LDY   #$01
   LDA   (FP),Y
   ORA   #$40
   LDY   #$70
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   LDY   #$71
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   LDY   #$71
   LDA   (FP),Y
   LDY   #$02
   STA   (FP),Y
.if_end@8:
   LDY   #$86
   LDA   (FP),Y
   ORA   (FP),Y
   LDY   #$72
   STA   (FP),Y
   LDY   #$87
   LDA   (FP),Y
   ORA   (FP),Y
   LDY   #$73
   STA   (FP),Y
   LDY   #$88
   LDA   (FP),Y
   ORA   (FP),Y
   LDY   #$74
   STA   (FP),Y
   LDY   #$89
   LDA   (FP),Y
   ORA   (FP),Y
   LDY   #$75
   STA   (FP),Y
   LDY   #$8A
   LDA   (FP),Y
   ORA   (FP),Y
   LDY   #$76
   STA   (FP),Y
   LDY   #$8B
   LDA   (FP),Y
   ORA   (FP),Y
   LDY   #$77
   STA   (FP),Y
   LDY   #$8C
   LDA   (FP),Y
   ORA   (FP),Y
   LDY   #$78
   STA   (FP),Y
   LDY   #$72
   LDA   (FP),Y
   LDY   #$79
   STA   (FP),Y
   LDY   #$73
   LDA   (FP),Y
   LDY   #$7A
   STA   (FP),Y
   LDY   #$74
   LDA   (FP),Y
   LDY   #$7B
   STA   (FP),Y
   LDY   #$75
   LDA   (FP),Y
   LDY   #$7C
   STA   (FP),Y
   LDY   #$76
   LDA   (FP),Y
   LDY   #$7D
   STA   (FP),Y
   LDY   #$77
   LDA   (FP),Y
   LDY   #$7E
   STA   (FP),Y
   LDY   #$78
   LDA   (FP),Y
   LDY   #$7F
   STA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@31
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@31
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@31
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@31
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@31
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@31
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@31
   LDA   #$00
   JMP   .cmp_end@32
.cmp_true@31:
   LDA   #$01
.cmp_end@32:
   LDY   #$80
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@9
   LDY   #$01
   LDA   (FP),Y
   ORA   #$80
   LDY   #$82
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   LDY   #$83
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   LDY   #$83
   LDA   (FP),Y
   LDY   #$02
   STA   (FP),Y
.if_end@9:
   LDY   #$02
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
   ADC   #$99
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$84
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

   ; prologue: 0 arg bytes, 4 local bytes
   SEC
   LDA   SSP
   SBC   #$06
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$05
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   LDA   #$04
   LDY   #$01
   STA   (FP),Y
   LDA   #$01
   INY
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$14
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   #$00
   DEY
   STA   (SSP),Y
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
   INY
   STA   (SSP),Y
   LDA   #$0D
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
   INY
   STA   (SSP),Y
   LDA   #$0E
   INY
   STA   (SSP),Y
   LDA   #$00
   INY
   STA   (SSP),Y
   LDY   #$01
   LDA   (FP),Y
   LDY   #$11
   STA   (SSP),Y
   LDY   #$02
   LDA   (FP),Y
   LDY   #$12
   STA   (SSP),Y
   LDA   #$11
   INY
   STA   (SSP),Y
   LDA   #$A1
   INY
   STA   (SSP),Y
   JSR   check
   LDY   #$03
   STA   (FP),Y
   TXA
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
   ADC   #$06
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$05
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
