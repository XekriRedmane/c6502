SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24

main:
   SUBROUTINE

   ; prologue: 0 arg bytes, 130 local bytes
   SEC
   LDA   SSP
   SBC   #$84
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$83
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   LDA   zero
   LDY   #$01
   STA   (FP),Y
   LDA   zero+1
   INY
   STA   (FP),Y
   LDA   zero+2
   INY
   STA   (FP),Y
   LDA   zero+3
   INY
   STA   (FP),Y
   LDA   zero+4
   INY
   STA   (FP),Y
   LDA   zero+5
   INY
   STA   (FP),Y
   LDA   zero+6
   INY
   STA   (FP),Y
   LDA   zero+7
   EOR   #$80
   INY
   STA   (FP),Y
   LDY   #$01
   LDA   (FP),Y
   LDY   #$09
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   LDY   #$0A
   STA   (FP),Y
   LDY   #$03
   LDA   (FP),Y
   LDY   #$0B
   STA   (FP),Y
   LDY   #$04
   LDA   (FP),Y
   LDY   #$0C
   STA   (FP),Y
   LDY   #$05
   LDA   (FP),Y
   LDY   #$0D
   STA   (FP),Y
   LDY   #$06
   LDA   (FP),Y
   LDY   #$0E
   STA   (FP),Y
   LDY   #$07
   LDA   (FP),Y
   LDY   #$0F
   STA   (FP),Y
   LDY   #$08
   LDA   (FP),Y
   LDY   #$10
   STA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@0
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@0
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@0
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@0
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@0
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@0
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@0
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@0
   LDA   #$00
   JMP   .cmp_end@1
.cmp_true@0:
   LDA   #$01
.cmp_end@1:
   LDY   #$11
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@0
   LDX   #$00
   LDA   #$01

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$84
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$83
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
   LDA   #$00
   STA   HARGS
   LDY   #$00
   STY   HARGS+1
   STY   HARGS+2
   STY   HARGS+3
   STY   HARGS+4
   STA   HARGS+5
   LDA   #$F0
   STA   HARGS+6
   LDA   #$3F
   STA   HARGS+7
   LDY   #$09
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
   JSR   ddiv
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
   LDA   #$F0
   INY
   STA   (FP),Y
   LDA   #$7F
   EOR   #$80
   INY
   STA   (FP),Y
   LDY   #$1A
   LDA   (FP),Y
   LDY   #$22
   CMP   (FP),Y
   BNE   .cmp_true@3
   LDY   #$19
   LDA   (FP),Y
   LDY   #$21
   CMP   (FP),Y
   BNE   .cmp_true@3
   LDY   #$18
   LDA   (FP),Y
   LDY   #$20
   CMP   (FP),Y
   BNE   .cmp_true@3
   LDY   #$17
   LDA   (FP),Y
   LDY   #$1F
   CMP   (FP),Y
   BNE   .cmp_true@3
   LDY   #$16
   LDA   (FP),Y
   LDY   #$1E
   CMP   (FP),Y
   BNE   .cmp_true@3
   LDY   #$15
   LDA   (FP),Y
   LDY   #$1D
   CMP   (FP),Y
   BNE   .cmp_true@3
   LDY   #$14
   LDA   (FP),Y
   LDY   #$1C
   CMP   (FP),Y
   BNE   .cmp_true@3
   LDY   #$13
   LDA   (FP),Y
   LDY   #$1B
   CMP   (FP),Y
   BNE   .cmp_true@3
   LDA   #$00
   JMP   .cmp_end@4
.cmp_true@3:
   LDA   #$01
.cmp_end@4:
   LDY   #$23
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@1
   LDX   #$00
   LDA   #$02

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$84
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$83
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
.if_end@1:
   SEC
   LDA   #$00
   SBC   #$0A
   LDY   #$25
   STA   (FP),Y
   LDA   #$00
   SBC   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   HARGS
   INY
   LDA   (FP),Y
   STA   HARGS+1
   JSR   i2d
   LDA   HARGS+2
   LDY   #$27
   STA   (FP),Y
   LDA   HARGS+3
   INY
   STA   (FP),Y
   LDA   HARGS+4
   INY
   STA   (FP),Y
   LDA   HARGS+5
   INY
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
   LDA   HARGS+9
   INY
   STA   (FP),Y
   LDY   #$27
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
   LDY   #$09
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
   JSR   ddiv
   LDA   HARGS+16
   LDY   #$2F
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
   CMP   #$7F
   BNE   .cmp_true@6
   DEY
   LDA   (FP),Y
   CMP   #$F0
   BNE   .cmp_true@6
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@6
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@6
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@6
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@6
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@6
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@6
   LDA   #$00
   JMP   .cmp_end@7
.cmp_true@6:
   LDA   #$01
.cmp_end@7:
   LDY   #$37
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
   LDA   #$03

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$84
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$83
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
   LDA   #$00
   LDY   #$39
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$09
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
   INY
   ORA   (FP),Y
   BEQ   .and_false@3
   LDA   #$01
   LDY   #$39
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@3
   LDA   #$01
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   JMP   .and_end@4
.and_false@3:
   LDA   #$00
   LDY   #$3B
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@4:
   LDY   #$39
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
   ADC   #$84
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$83
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
   LDY   #$09
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
   INY
   ORA   (FP),Y
   BEQ   .if_end@6
   LDX   #$00
   LDA   #$05

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$84
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$83
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
.if_end@6:
   LDA   #$00
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
   INY
   STA   (FP),Y
   EOR   #$80
   INY
   STA   (FP),Y
   LDA   zero+7
   CMP   (FP),Y
   BNE   .cmp_true@9
   LDA   zero+6
   DEY
   CMP   (FP),Y
   BNE   .cmp_true@9
   LDA   zero+5
   DEY
   CMP   (FP),Y
   BNE   .cmp_true@9
   LDA   zero+4
   DEY
   CMP   (FP),Y
   BNE   .cmp_true@9
   LDA   zero+3
   DEY
   CMP   (FP),Y
   BNE   .cmp_true@9
   LDA   zero+2
   DEY
   CMP   (FP),Y
   BNE   .cmp_true@9
   LDA   zero+1
   DEY
   CMP   (FP),Y
   BNE   .cmp_true@9
   LDA   zero
   DEY
   CMP   (FP),Y
   BNE   .cmp_true@9
   LDA   #$00
   JMP   .cmp_end@10
.cmp_true@9:
   LDA   #$01
.cmp_end@10:
   LDY   #$45
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@7
   LDX   #$00
   LDA   #$06

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$84
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$83
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
.if_end@7:
   LDA   #$00
   LDY   #$47
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
   INY
   STA   (FP),Y
   EOR   #$80
   INY
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$10
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   #$00
   LDY   #$01
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
   LDA   #$10
   INY
   STA   (SSP),Y
   LDA   #$40
   INY
   STA   (SSP),Y
   LDY   #$47
   LDA   (FP),Y
   LDY   #$09
   STA   (SSP),Y
   LDY   #$48
   LDA   (FP),Y
   LDY   #$0A
   STA   (SSP),Y
   LDY   #$49
   LDA   (FP),Y
   LDY   #$0B
   STA   (SSP),Y
   LDY   #$4A
   LDA   (FP),Y
   LDY   #$0C
   STA   (SSP),Y
   LDY   #$4B
   LDA   (FP),Y
   LDY   #$0D
   STA   (SSP),Y
   LDY   #$4C
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
   JSR   copysign
   LDA   HARGS+16
   LDY   #$4F
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
   LDY   #$4F
   LDA   (FP),Y
   LDY   #$57
   STA   (FP),Y
   LDY   #$50
   LDA   (FP),Y
   LDY   #$58
   STA   (FP),Y
   LDY   #$51
   LDA   (FP),Y
   LDY   #$59
   STA   (FP),Y
   LDY   #$52
   LDA   (FP),Y
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
   LDY   #$55
   LDA   (FP),Y
   LDY   #$5D
   STA   (FP),Y
   LDY   #$56
   LDA   (FP),Y
   LDY   #$5E
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
   LDA   #$14
   INY
   STA   (FP),Y
   LDA   #$40
   EOR   #$80
   INY
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$10
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDY   #$5F
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$60
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$61
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$62
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   LDY   #$63
   LDA   (FP),Y
   LDY   #$05
   STA   (SSP),Y
   LDY   #$64
   LDA   (FP),Y
   LDY   #$06
   STA   (SSP),Y
   LDY   #$65
   LDA   (FP),Y
   LDY   #$07
   STA   (SSP),Y
   LDY   #$66
   LDA   (FP),Y
   LDY   #$08
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
   INY
   STA   (SSP),Y
   INY
   STA   (SSP),Y
   JSR   copysign
   LDA   HARGS+16
   LDY   #$67
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
   LDY   #$67
   LDA   (FP),Y
   LDY   #$6F
   STA   (FP),Y
   LDY   #$68
   LDA   (FP),Y
   LDY   #$70
   STA   (FP),Y
   LDY   #$69
   LDA   (FP),Y
   LDY   #$71
   STA   (FP),Y
   LDY   #$6A
   LDA   (FP),Y
   LDY   #$72
   STA   (FP),Y
   LDY   #$6B
   LDA   (FP),Y
   LDY   #$73
   STA   (FP),Y
   LDY   #$6C
   LDA   (FP),Y
   LDY   #$74
   STA   (FP),Y
   LDY   #$6D
   LDA   (FP),Y
   LDY   #$75
   STA   (FP),Y
   LDY   #$6E
   LDA   (FP),Y
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
   LDA   #$10
   INY
   STA   (FP),Y
   LDA   #$40
   EOR   #$80
   INY
   STA   (FP),Y
   LDY   #$5E
   LDA   (FP),Y
   LDY   #$7E
   CMP   (FP),Y
   BNE   .cmp_true@12
   LDY   #$5D
   LDA   (FP),Y
   LDY   #$7D
   CMP   (FP),Y
   BNE   .cmp_true@12
   LDY   #$5C
   LDA   (FP),Y
   LDY   #$7C
   CMP   (FP),Y
   BNE   .cmp_true@12
   LDY   #$5B
   LDA   (FP),Y
   LDY   #$7B
   CMP   (FP),Y
   BNE   .cmp_true@12
   LDY   #$5A
   LDA   (FP),Y
   LDY   #$7A
   CMP   (FP),Y
   BNE   .cmp_true@12
   LDY   #$59
   LDA   (FP),Y
   LDY   #$79
   CMP   (FP),Y
   BNE   .cmp_true@12
   LDY   #$58
   LDA   (FP),Y
   LDY   #$78
   CMP   (FP),Y
   BNE   .cmp_true@12
   LDY   #$57
   LDA   (FP),Y
   LDY   #$77
   CMP   (FP),Y
   BNE   .cmp_true@12
   LDA   #$00
   JMP   .cmp_end@13
.cmp_true@12:
   LDA   #$01
.cmp_end@13:
   LDY   #$7F
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@8
   LDX   #$00
   LDA   #$07

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$84
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$83
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
.if_end@8:
   LDY   #$76
   LDA   (FP),Y
   CMP   #$40
   BNE   .cmp_true@15
   DEY
   LDA   (FP),Y
   CMP   #$14
   BNE   .cmp_true@15
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@15
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@15
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@15
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@15
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@15
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@15
   LDA   #$00
   JMP   .cmp_end@16
.cmp_true@15:
   LDA   #$01
.cmp_end@16:
   LDY   #$81
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@9
   LDX   #$00
   LDA   #$08

   ; epilogue
   PHA
   TXA
   PHA
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
   PLA
   TAX
   PLA
   RTS
.if_end@9:
   LDX   #$00
   TXA

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$84
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$83
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

zero:
   DS.B  8
