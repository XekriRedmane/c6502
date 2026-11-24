SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24

main:
   SUBROUTINE

   ; prologue: 0 arg bytes, 156 local bytes
   SEC
   LDA   SSP
   SBC   #$9E
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$9D
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   LDA   @0.d
   LDY   #$01
   STA   (FP),Y
   LDA   @0.d+1
   INY
   STA   (FP),Y
   LDA   @0.d+2
   INY
   STA   (FP),Y
   LDA   @0.d+3
   INY
   STA   (FP),Y
   LDA   @0.d+4
   INY
   STA   (FP),Y
   LDA   @0.d+5
   INY
   STA   (FP),Y
   LDA   @0.d+6
   INY
   STA   (FP),Y
   LDA   @0.d+7
   INY
   STA   (FP),Y
   LDA   @0.d
   STA   HARGS
   LDA   @0.d+1
   STA   HARGS+1
   LDA   @0.d+2
   STA   HARGS+2
   LDA   @0.d+3
   STA   HARGS+3
   LDA   @0.d+4
   STA   HARGS+4
   LDA   @0.d+5
   STA   HARGS+5
   LDA   @0.d+6
   STA   HARGS+6
   LDA   @0.d+7
   STA   HARGS+7
   LDA   #$00
   STA   HARGS+8
   LDY   #$00
   STY   HARGS+9
   STY   HARGS+10
   STY   HARGS+11
   STY   HARGS+12
   STA   HARGS+13
   LDA   #$F0
   STA   HARGS+14
   LDA   #$3F
   STA   HARGS+15
   JSR   dadd
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
   STA   @0.d
   INY
   LDA   (FP),Y
   STA   @0.d+1
   INY
   LDA   (FP),Y
   STA   @0.d+2
   INY
   LDA   (FP),Y
   STA   @0.d+3
   INY
   LDA   (FP),Y
   STA   @0.d+4
   INY
   LDA   (FP),Y
   STA   @0.d+5
   INY
   LDA   (FP),Y
   STA   @0.d+6
   INY
   LDA   (FP),Y
   STA   @0.d+7
   LDY   #$08
   LDA   (FP),Y
   CMP   #$3F
   BNE   .cmp_true@0
   DEY
   LDA   (FP),Y
   CMP   #$E8
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
   ADC   #$9E
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$9D
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
   LDA   @0.d+7
   CMP   #$3F
   BNE   .cmp_true@3
   LDA   @0.d+6
   CMP   #$FC
   BNE   .cmp_true@3
   LDA   @0.d+5
   BNE   .cmp_true@3
   LDA   @0.d+4
   BNE   .cmp_true@3
   LDA   @0.d+3
   BNE   .cmp_true@3
   LDA   @0.d+2
   BNE   .cmp_true@3
   LDA   @0.d+1
   BNE   .cmp_true@3
   LDA   @0.d
   BNE   .cmp_true@3
   LDA   #$00
   JMP   .cmp_end@4
.cmp_true@3:
   LDA   #$01
.cmp_end@4:
   LDY   #$13
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
   ADC   #$9E
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$9D
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
   LDA   #$CD
   LDY   #$15
   STA   (FP),Y
   LDA   #$CC
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$0C
   INY
   STA   (FP),Y
   LDA   #$59
   INY
   STA   (FP),Y
   LDA   #$40
   EOR   #$80
   INY
   STA   (FP),Y
   LDY   #$15
   LDA   (FP),Y
   STA   @0.d
   INY
   LDA   (FP),Y
   STA   @0.d+1
   INY
   LDA   (FP),Y
   STA   @0.d+2
   INY
   LDA   (FP),Y
   STA   @0.d+3
   INY
   LDA   (FP),Y
   STA   @0.d+4
   INY
   LDA   (FP),Y
   STA   @0.d+5
   INY
   LDA   (FP),Y
   STA   @0.d+6
   INY
   LDA   (FP),Y
   STA   @0.d+7
   LDA   @0.d
   STA   HARGS
   LDA   @0.d+1
   STA   HARGS+1
   LDA   @0.d+2
   STA   HARGS+2
   LDA   @0.d+3
   STA   HARGS+3
   LDA   @0.d+4
   STA   HARGS+4
   LDA   @0.d+5
   STA   HARGS+5
   LDA   @0.d+6
   STA   HARGS+6
   LDA   @0.d+7
   STA   HARGS+7
   LDA   #$00
   STA   HARGS+8
   LDY   #$00
   STY   HARGS+9
   STY   HARGS+10
   STY   HARGS+11
   STY   HARGS+12
   STA   HARGS+13
   LDA   #$F0
   STA   HARGS+14
   LDA   #$3F
   STA   HARGS+15
   JSR   dadd
   LDA   HARGS+16
   LDY   #$1D
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
   LDY   #$1D
   LDA   (FP),Y
   STA   @0.d
   INY
   LDA   (FP),Y
   STA   @0.d+1
   INY
   LDA   (FP),Y
   STA   @0.d+2
   INY
   LDA   (FP),Y
   STA   @0.d+3
   INY
   LDA   (FP),Y
   STA   @0.d+4
   INY
   LDA   (FP),Y
   STA   @0.d+5
   INY
   LDA   (FP),Y
   STA   @0.d+6
   INY
   LDA   (FP),Y
   STA   @0.d+7
   LDA   #$CD
   INY
   STA   (FP),Y
   LDA   #$CC
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
   LDA   #$58
   INY
   STA   (FP),Y
   LDA   #$40
   EOR   #$80
   INY
   STA   (FP),Y
   LDY   #$24
   LDA   (FP),Y
   LDY   #$2C
   CMP   (FP),Y
   BNE   .cmp_true@6
   LDY   #$23
   LDA   (FP),Y
   LDY   #$2B
   CMP   (FP),Y
   BNE   .cmp_true@6
   LDY   #$22
   LDA   (FP),Y
   LDY   #$2A
   CMP   (FP),Y
   BNE   .cmp_true@6
   LDY   #$21
   LDA   (FP),Y
   LDY   #$29
   CMP   (FP),Y
   BNE   .cmp_true@6
   LDY   #$20
   LDA   (FP),Y
   LDY   #$28
   CMP   (FP),Y
   BNE   .cmp_true@6
   LDY   #$1F
   LDA   (FP),Y
   LDY   #$27
   CMP   (FP),Y
   BNE   .cmp_true@6
   LDY   #$1E
   LDA   (FP),Y
   LDY   #$26
   CMP   (FP),Y
   BNE   .cmp_true@6
   LDY   #$1D
   LDA   (FP),Y
   LDY   #$25
   CMP   (FP),Y
   BNE   .cmp_true@6
   LDA   #$00
   JMP   .cmp_end@7
.cmp_true@6:
   LDA   #$01
.cmp_end@7:
   LDY   #$2D
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
   ADC   #$9E
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$9D
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
   LDA   #$CD
   LDY   #$2F
   STA   (FP),Y
   LDA   #$CC
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
   LDA   #$58
   INY
   STA   (FP),Y
   LDA   #$40
   EOR   #$80
   INY
   STA   (FP),Y
   LDA   @0.d+7
   CMP   (FP),Y
   BNE   .cmp_true@9
   LDA   @0.d+6
   DEY
   CMP   (FP),Y
   BNE   .cmp_true@9
   LDA   @0.d+5
   DEY
   CMP   (FP),Y
   BNE   .cmp_true@9
   LDA   @0.d+4
   DEY
   CMP   (FP),Y
   BNE   .cmp_true@9
   LDA   @0.d+3
   DEY
   CMP   (FP),Y
   BNE   .cmp_true@9
   LDA   @0.d+2
   DEY
   CMP   (FP),Y
   BNE   .cmp_true@9
   LDA   @0.d+1
   DEY
   CMP   (FP),Y
   BNE   .cmp_true@9
   LDA   @0.d
   DEY
   CMP   (FP),Y
   BNE   .cmp_true@9
   LDA   #$00
   JMP   .cmp_end@10
.cmp_true@9:
   LDA   #$01
.cmp_end@10:
   LDY   #$37
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
   LDA   #$04

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$9E
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$9D
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
   LDA   @0.d
   LDY   #$39
   STA   (FP),Y
   LDA   @0.d+1
   INY
   STA   (FP),Y
   LDA   @0.d+2
   INY
   STA   (FP),Y
   LDA   @0.d+3
   INY
   STA   (FP),Y
   LDA   @0.d+4
   INY
   STA   (FP),Y
   LDA   @0.d+5
   INY
   STA   (FP),Y
   LDA   @0.d+6
   INY
   STA   (FP),Y
   LDA   @0.d+7
   INY
   STA   (FP),Y
   LDA   @0.d
   STA   HARGS
   LDA   @0.d+1
   STA   HARGS+1
   LDA   @0.d+2
   STA   HARGS+2
   LDA   @0.d+3
   STA   HARGS+3
   LDA   @0.d+4
   STA   HARGS+4
   LDA   @0.d+5
   STA   HARGS+5
   LDA   @0.d+6
   STA   HARGS+6
   LDA   @0.d+7
   STA   HARGS+7
   LDA   #$00
   STA   HARGS+8
   LDY   #$00
   STY   HARGS+9
   STY   HARGS+10
   STY   HARGS+11
   STY   HARGS+12
   STA   HARGS+13
   LDA   #$F0
   STA   HARGS+14
   LDA   #$3F
   STA   HARGS+15
   JSR   dsub
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
   LDA   HARGS+23
   INY
   STA   (FP),Y
   LDY   #$41
   LDA   (FP),Y
   STA   @0.d
   INY
   LDA   (FP),Y
   STA   @0.d+1
   INY
   LDA   (FP),Y
   STA   @0.d+2
   INY
   LDA   (FP),Y
   STA   @0.d+3
   INY
   LDA   (FP),Y
   STA   @0.d+4
   INY
   LDA   (FP),Y
   STA   @0.d+5
   INY
   LDA   (FP),Y
   STA   @0.d+6
   INY
   LDA   (FP),Y
   STA   @0.d+7
   LDA   #$CD
   INY
   STA   (FP),Y
   LDA   #$CC
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
   LDA   #$58
   INY
   STA   (FP),Y
   LDA   #$40
   EOR   #$80
   INY
   STA   (FP),Y
   LDY   #$40
   LDA   (FP),Y
   LDY   #$50
   CMP   (FP),Y
   BNE   .cmp_true@12
   LDY   #$3F
   LDA   (FP),Y
   LDY   #$4F
   CMP   (FP),Y
   BNE   .cmp_true@12
   LDY   #$3E
   LDA   (FP),Y
   LDY   #$4E
   CMP   (FP),Y
   BNE   .cmp_true@12
   LDY   #$3D
   LDA   (FP),Y
   LDY   #$4D
   CMP   (FP),Y
   BNE   .cmp_true@12
   LDY   #$3C
   LDA   (FP),Y
   LDY   #$4C
   CMP   (FP),Y
   BNE   .cmp_true@12
   LDY   #$3B
   LDA   (FP),Y
   LDY   #$4B
   CMP   (FP),Y
   BNE   .cmp_true@12
   LDY   #$3A
   LDA   (FP),Y
   LDY   #$4A
   CMP   (FP),Y
   BNE   .cmp_true@12
   LDY   #$39
   LDA   (FP),Y
   LDY   #$49
   CMP   (FP),Y
   BNE   .cmp_true@12
   LDA   #$00
   JMP   .cmp_end@13
.cmp_true@12:
   LDA   #$01
.cmp_end@13:
   LDY   #$51
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
   LDA   #$05

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$9E
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$9D
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
   LDA   #$CD
   LDY   #$53
   STA   (FP),Y
   LDA   #$CC
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$0C
   INY
   STA   (FP),Y
   LDA   #$59
   INY
   STA   (FP),Y
   LDA   #$40
   EOR   #$80
   INY
   STA   (FP),Y
   LDA   @0.d+7
   CMP   (FP),Y
   BNE   .cmp_true@15
   LDA   @0.d+6
   DEY
   CMP   (FP),Y
   BNE   .cmp_true@15
   LDA   @0.d+5
   DEY
   CMP   (FP),Y
   BNE   .cmp_true@15
   LDA   @0.d+4
   DEY
   CMP   (FP),Y
   BNE   .cmp_true@15
   LDA   @0.d+3
   DEY
   CMP   (FP),Y
   BNE   .cmp_true@15
   LDA   @0.d+2
   DEY
   CMP   (FP),Y
   BNE   .cmp_true@15
   LDA   @0.d+1
   DEY
   CMP   (FP),Y
   BNE   .cmp_true@15
   LDA   @0.d
   DEY
   CMP   (FP),Y
   BNE   .cmp_true@15
   LDA   #$00
   JMP   .cmp_end@16
.cmp_true@15:
   LDA   #$01
.cmp_end@16:
   LDY   #$5B
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
   LDA   #$06

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$9E
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$9D
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
   LDA   @0.d
   STA   HARGS
   LDA   @0.d+1
   STA   HARGS+1
   LDA   @0.d+2
   STA   HARGS+2
   LDA   @0.d+3
   STA   HARGS+3
   LDA   @0.d+4
   STA   HARGS+4
   LDA   @0.d+5
   STA   HARGS+5
   LDA   @0.d+6
   STA   HARGS+6
   LDA   @0.d+7
   STA   HARGS+7
   LDA   #$00
   STA   HARGS+8
   LDY   #$00
   STY   HARGS+9
   STY   HARGS+10
   STY   HARGS+11
   STY   HARGS+12
   STA   HARGS+13
   LDA   #$F0
   STA   HARGS+14
   LDA   #$3F
   STA   HARGS+15
   JSR   dsub
   LDA   HARGS+16
   LDY   #$5D
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
   LDY   #$5D
   LDA   (FP),Y
   STA   @0.d
   INY
   LDA   (FP),Y
   STA   @0.d+1
   INY
   LDA   (FP),Y
   STA   @0.d+2
   INY
   LDA   (FP),Y
   STA   @0.d+3
   INY
   LDA   (FP),Y
   STA   @0.d+4
   INY
   LDA   (FP),Y
   STA   @0.d+5
   INY
   LDA   (FP),Y
   STA   @0.d+6
   INY
   LDA   (FP),Y
   STA   @0.d+7
   LDA   #$CD
   INY
   STA   (FP),Y
   LDA   #$CC
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$4C
   INY
   STA   (FP),Y
   LDA   #$59
   INY
   STA   (FP),Y
   LDA   #$40
   EOR   #$80
   INY
   STA   (FP),Y
   LDY   #$64
   LDA   (FP),Y
   LDY   #$6C
   CMP   (FP),Y
   BNE   .cmp_true@18
   LDY   #$63
   LDA   (FP),Y
   LDY   #$6B
   CMP   (FP),Y
   BNE   .cmp_true@18
   LDY   #$62
   LDA   (FP),Y
   LDY   #$6A
   CMP   (FP),Y
   BNE   .cmp_true@18
   LDY   #$61
   LDA   (FP),Y
   LDY   #$69
   CMP   (FP),Y
   BNE   .cmp_true@18
   LDY   #$60
   LDA   (FP),Y
   LDY   #$68
   CMP   (FP),Y
   BNE   .cmp_true@18
   LDY   #$5F
   LDA   (FP),Y
   LDY   #$67
   CMP   (FP),Y
   BNE   .cmp_true@18
   LDY   #$5E
   LDA   (FP),Y
   LDY   #$66
   CMP   (FP),Y
   BNE   .cmp_true@18
   LDY   #$5D
   LDA   (FP),Y
   LDY   #$65
   CMP   (FP),Y
   BNE   .cmp_true@18
   LDA   #$00
   JMP   .cmp_end@19
.cmp_true@18:
   LDA   #$01
.cmp_end@19:
   LDY   #$6D
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@6
   LDX   #$00
   LDA   #$07

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$9E
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$9D
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
   LDA   #$CD
   LDY   #$6F
   STA   (FP),Y
   LDA   #$CC
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$4C
   INY
   STA   (FP),Y
   LDA   #$59
   INY
   STA   (FP),Y
   LDA   #$40
   EOR   #$80
   INY
   STA   (FP),Y
   LDA   @0.d+7
   CMP   (FP),Y
   BNE   .cmp_true@21
   LDA   @0.d+6
   DEY
   CMP   (FP),Y
   BNE   .cmp_true@21
   LDA   @0.d+5
   DEY
   CMP   (FP),Y
   BNE   .cmp_true@21
   LDA   @0.d+4
   DEY
   CMP   (FP),Y
   BNE   .cmp_true@21
   LDA   @0.d+3
   DEY
   CMP   (FP),Y
   BNE   .cmp_true@21
   LDA   @0.d+2
   DEY
   CMP   (FP),Y
   BNE   .cmp_true@21
   LDA   @0.d+1
   DEY
   CMP   (FP),Y
   BNE   .cmp_true@21
   LDA   @0.d
   DEY
   CMP   (FP),Y
   BNE   .cmp_true@21
   LDA   #$00
   JMP   .cmp_end@22
.cmp_true@21:
   LDA   #$01
.cmp_end@22:
   LDY   #$77
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
   LDA   #$08

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$9E
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$9D
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
   LDA   #$4F
   STA   @0.d
   LDA   #$9B
   STA   @0.d+1
   LDA   #$0E
   STA   @0.d+2
   LDA   #$0A
   STA   @0.d+3
   LDA   #$B4
   STA   @0.d+4
   LDA   #$E3
   STA   @0.d+5
   LDA   #$92
   STA   @0.d+6
   LDA   #$3B
   STA   @0.d+7
   LDA   #$4F
   LDY   #$79
   STA   (FP),Y
   LDA   @0.d+1
   INY
   STA   (FP),Y
   LDA   @0.d+2
   INY
   STA   (FP),Y
   LDA   @0.d+3
   INY
   STA   (FP),Y
   LDA   @0.d+4
   INY
   STA   (FP),Y
   LDA   @0.d+5
   INY
   STA   (FP),Y
   LDA   @0.d+6
   INY
   STA   (FP),Y
   LDA   @0.d+7
   INY
   STA   (FP),Y
   LDA   @0.d
   STA   HARGS
   LDA   @0.d+1
   STA   HARGS+1
   LDA   @0.d+2
   STA   HARGS+2
   LDA   @0.d+3
   STA   HARGS+3
   LDA   @0.d+4
   STA   HARGS+4
   LDA   @0.d+5
   STA   HARGS+5
   LDA   @0.d+6
   STA   HARGS+6
   LDA   @0.d+7
   STA   HARGS+7
   LDA   #$00
   STA   HARGS+8
   LDY   #$00
   STY   HARGS+9
   STY   HARGS+10
   STY   HARGS+11
   STY   HARGS+12
   STA   HARGS+13
   LDA   #$F0
   STA   HARGS+14
   LDA   #$3F
   STA   HARGS+15
   JSR   dadd
   LDA   HARGS+16
   LDY   #$81
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
   LDY   #$81
   LDA   (FP),Y
   STA   @0.d
   INY
   LDA   (FP),Y
   STA   @0.d+1
   INY
   LDA   (FP),Y
   STA   @0.d+2
   INY
   LDA   (FP),Y
   STA   @0.d+3
   INY
   LDA   (FP),Y
   STA   @0.d+4
   INY
   LDA   (FP),Y
   STA   @0.d+5
   INY
   LDA   (FP),Y
   STA   @0.d+6
   INY
   LDA   (FP),Y
   STA   @0.d+7
   CMP   #$3F
   BNE   .cmp_true@24
   LDA   @0.d+6
   CMP   #$F0
   BNE   .cmp_true@24
   LDA   @0.d+5
   BNE   .cmp_true@24
   LDA   @0.d+4
   BNE   .cmp_true@24
   LDA   @0.d+3
   BNE   .cmp_true@24
   LDA   @0.d+2
   BNE   .cmp_true@24
   LDA   @0.d+1
   BNE   .cmp_true@24
   LDA   @0.d
   BNE   .cmp_true@24
   LDA   #$00
   JMP   .cmp_end@25
.cmp_true@24:
   LDA   #$01
.cmp_end@25:
   LDY   #$89
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
   LDA   #$09

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$9E
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$9D
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
   LDA   #$50
   STA   @0.d
   LDA   #$EF
   STA   @0.d+1
   LDA   #$E2
   STA   @0.d+2
   LDA   #$D6
   STA   @0.d+3
   LDA   #$E4
   STA   @0.d+4
   LDA   #$1A
   STA   @0.d+5
   LDA   #$4B
   STA   @0.d+6
   LDA   #$44
   STA   @0.d+7
   LDA   #$50
   LDY   #$8B
   STA   (FP),Y
   LDA   @0.d+1
   INY
   STA   (FP),Y
   LDA   @0.d+2
   INY
   STA   (FP),Y
   LDA   @0.d+3
   INY
   STA   (FP),Y
   LDA   @0.d+4
   INY
   STA   (FP),Y
   LDA   @0.d+5
   INY
   STA   (FP),Y
   LDA   @0.d+6
   INY
   STA   (FP),Y
   LDA   @0.d+7
   INY
   STA   (FP),Y
   LDA   @0.d
   STA   HARGS
   LDA   @0.d+1
   STA   HARGS+1
   LDA   @0.d+2
   STA   HARGS+2
   LDA   @0.d+3
   STA   HARGS+3
   LDA   @0.d+4
   STA   HARGS+4
   LDA   @0.d+5
   STA   HARGS+5
   LDA   @0.d+6
   STA   HARGS+6
   LDA   @0.d+7
   STA   HARGS+7
   LDA   #$00
   STA   HARGS+8
   LDY   #$00
   STY   HARGS+9
   STY   HARGS+10
   STY   HARGS+11
   STY   HARGS+12
   STA   HARGS+13
   LDA   #$F0
   STA   HARGS+14
   LDA   #$3F
   STA   HARGS+15
   JSR   dsub
   LDA   HARGS+16
   LDY   #$93
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
   LDY   #$93
   LDA   (FP),Y
   STA   @0.d
   INY
   LDA   (FP),Y
   STA   @0.d+1
   INY
   LDA   (FP),Y
   STA   @0.d+2
   INY
   LDA   (FP),Y
   STA   @0.d+3
   INY
   LDA   (FP),Y
   STA   @0.d+4
   INY
   LDA   (FP),Y
   STA   @0.d+5
   INY
   LDA   (FP),Y
   STA   @0.d+6
   INY
   LDA   (FP),Y
   STA   @0.d+7
   CMP   #$44
   BNE   .cmp_true@27
   LDA   @0.d+6
   CMP   #$4B
   BNE   .cmp_true@27
   LDA   @0.d+5
   CMP   #$1A
   BNE   .cmp_true@27
   LDA   @0.d+4
   CMP   #$E4
   BNE   .cmp_true@27
   LDA   @0.d+3
   CMP   #$D6
   BNE   .cmp_true@27
   LDA   @0.d+2
   CMP   #$E2
   BNE   .cmp_true@27
   LDA   @0.d+1
   CMP   #$EF
   BNE   .cmp_true@27
   LDA   @0.d
   CMP   #$50
   BNE   .cmp_true@27
   LDA   #$00
   JMP   .cmp_end@28
.cmp_true@27:
   LDA   #$01
.cmp_end@28:
   LDY   #$9B
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
   LDA   #$0A

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$9E
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
   ADC   #$9E
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$9D
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

@0.d:
   DC.L  $00000000
   DC.L  $3FE80000
