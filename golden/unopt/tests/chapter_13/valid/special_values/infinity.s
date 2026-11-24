SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24

main:
   SUBROUTINE

   ; prologue: 0 arg bytes, 76 local bytes
   SEC
   LDA   SSP
   SBC   #$4E
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$4D
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   LDA   inf+7
   CMP   #$7F
   BNE   .cmp_true@0
   LDA   inf+6
   CMP   #$F0
   BNE   .cmp_true@0
   LDA   inf+5
   BNE   .cmp_true@0
   LDA   inf+4
   BNE   .cmp_true@0
   LDA   inf+3
   BNE   .cmp_true@0
   LDA   inf+2
   BNE   .cmp_true@0
   LDA   inf+1
   BNE   .cmp_true@0
   LDA   inf
   BNE   .cmp_true@0
   LDA   #$00
   JMP   .cmp_end@1
.cmp_true@0:
   LDA   #$01
.cmp_end@1:
   LDY   #$01
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
   ADC   #$4E
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$4D
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
   LDA   inf
   STA   HARGS
   LDA   inf+1
   STA   HARGS+1
   LDA   inf+2
   STA   HARGS+2
   LDA   inf+3
   STA   HARGS+3
   LDA   inf+4
   STA   HARGS+4
   LDA   inf+5
   STA   HARGS+5
   LDA   inf+6
   STA   HARGS+6
   LDA   inf+7
   STA   HARGS+7
   LDA   very_large
   STA   HARGS+8
   LDA   very_large+1
   STA   HARGS+9
   LDA   very_large+2
   STA   HARGS+10
   LDA   very_large+3
   STA   HARGS+11
   LDA   very_large+4
   STA   HARGS+12
   LDA   very_large+5
   STA   HARGS+13
   LDA   very_large+6
   STA   HARGS+14
   LDA   very_large+7
   STA   HARGS+15
   JSR   dle
   LDA   HARGS+16
   LDY   #$03
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
   ADC   #$4E
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$4D
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
   LDA   very_large
   STA   HARGS
   LDA   very_large+1
   STA   HARGS+1
   LDA   very_large+2
   STA   HARGS+2
   LDA   very_large+3
   STA   HARGS+3
   LDA   very_large+4
   STA   HARGS+4
   LDA   very_large+5
   STA   HARGS+5
   LDA   very_large+6
   STA   HARGS+6
   LDA   very_large+7
   STA   HARGS+7
   LDA   #$00
   STA   HARGS+8
   LDY   #$00
   STY   HARGS+9
   STY   HARGS+10
   STY   HARGS+11
   STY   HARGS+12
   STA   HARGS+13
   LDA   #$24
   STA   HARGS+14
   LDA   #$40
   STA   HARGS+15
   JSR   dmul
   LDA   HARGS+16
   LDY   #$05
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
   CMP   inf+7
   BNE   .cmp_true@3
   DEY
   LDA   (FP),Y
   CMP   inf+6
   BNE   .cmp_true@3
   DEY
   LDA   (FP),Y
   CMP   inf+5
   BNE   .cmp_true@3
   DEY
   LDA   (FP),Y
   CMP   inf+4
   BNE   .cmp_true@3
   DEY
   LDA   (FP),Y
   CMP   inf+3
   BNE   .cmp_true@3
   DEY
   LDA   (FP),Y
   CMP   inf+2
   BNE   .cmp_true@3
   DEY
   LDA   (FP),Y
   CMP   inf+1
   BNE   .cmp_true@3
   DEY
   LDA   (FP),Y
   CMP   inf
   BNE   .cmp_true@3
   LDA   #$00
   JMP   .cmp_end@4
.cmp_true@3:
   LDA   #$01
.cmp_end@4:
   LDY   #$0D
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
   ADC   #$4E
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$4D
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
   LDA   zero
   STA   HARGS+8
   LDA   zero+1
   STA   HARGS+9
   LDA   zero+2
   STA   HARGS+10
   LDA   zero+3
   STA   HARGS+11
   LDA   zero+4
   STA   HARGS+12
   LDA   zero+5
   STA   HARGS+13
   LDA   zero+6
   STA   HARGS+14
   LDA   zero+7
   STA   HARGS+15
   JSR   ddiv
   LDA   HARGS+16
   LDY   #$0F
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
   CMP   inf+7
   BNE   .cmp_true@6
   DEY
   LDA   (FP),Y
   CMP   inf+6
   BNE   .cmp_true@6
   DEY
   LDA   (FP),Y
   CMP   inf+5
   BNE   .cmp_true@6
   DEY
   LDA   (FP),Y
   CMP   inf+4
   BNE   .cmp_true@6
   DEY
   LDA   (FP),Y
   CMP   inf+3
   BNE   .cmp_true@6
   DEY
   LDA   (FP),Y
   CMP   inf+2
   BNE   .cmp_true@6
   DEY
   LDA   (FP),Y
   CMP   inf+1
   BNE   .cmp_true@6
   DEY
   LDA   (FP),Y
   CMP   inf
   BNE   .cmp_true@6
   LDA   #$00
   JMP   .cmp_end@7
.cmp_true@6:
   LDA   #$01
.cmp_end@7:
   LDY   #$17
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
   ADC   #$4E
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$4D
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
   LDA   inf
   LDY   #$19
   STA   (FP),Y
   LDA   inf+1
   INY
   STA   (FP),Y
   LDA   inf+2
   INY
   STA   (FP),Y
   LDA   inf+3
   INY
   STA   (FP),Y
   LDA   inf+4
   INY
   STA   (FP),Y
   LDA   inf+5
   INY
   STA   (FP),Y
   LDA   inf+6
   INY
   STA   (FP),Y
   LDA   inf+7
   EOR   #$80
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
   LDY   #$1C
   LDA   (FP),Y
   LDY   #$24
   STA   (FP),Y
   LDY   #$1D
   LDA   (FP),Y
   LDY   #$25
   STA   (FP),Y
   LDY   #$1E
   LDA   (FP),Y
   LDY   #$26
   STA   (FP),Y
   LDY   #$1F
   LDA   (FP),Y
   LDY   #$27
   STA   (FP),Y
   LDY   #$20
   LDA   (FP),Y
   LDY   #$28
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
   LDA   #$3F
   EOR   #$80
   INY
   STA   (FP),Y
   LDY   #$29
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
   LDA   zero
   STA   HARGS+8
   LDA   zero+1
   STA   HARGS+9
   LDA   zero+2
   STA   HARGS+10
   LDA   zero+3
   STA   HARGS+11
   LDA   zero+4
   STA   HARGS+12
   LDA   zero+5
   STA   HARGS+13
   LDA   zero+6
   STA   HARGS+14
   LDA   zero+7
   STA   HARGS+15
   JSR   ddiv
   LDA   HARGS+16
   LDY   #$31
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
   LDY   #$31
   LDA   (FP),Y
   LDY   #$39
   STA   (FP),Y
   LDY   #$32
   LDA   (FP),Y
   LDY   #$3A
   STA   (FP),Y
   LDY   #$33
   LDA   (FP),Y
   LDY   #$3B
   STA   (FP),Y
   LDY   #$34
   LDA   (FP),Y
   LDY   #$3C
   STA   (FP),Y
   LDY   #$35
   LDA   (FP),Y
   LDY   #$3D
   STA   (FP),Y
   LDY   #$36
   LDA   (FP),Y
   LDY   #$3E
   STA   (FP),Y
   LDY   #$37
   LDA   (FP),Y
   LDY   #$3F
   STA   (FP),Y
   LDY   #$38
   LDA   (FP),Y
   LDY   #$40
   STA   (FP),Y
   LDA   very_large
   INY
   STA   (FP),Y
   LDA   very_large+1
   INY
   STA   (FP),Y
   LDA   very_large+2
   INY
   STA   (FP),Y
   LDA   very_large+3
   INY
   STA   (FP),Y
   LDA   very_large+4
   INY
   STA   (FP),Y
   LDA   very_large+5
   INY
   STA   (FP),Y
   LDA   very_large+6
   INY
   STA   (FP),Y
   LDA   very_large+7
   EOR   #$80
   INY
   STA   (FP),Y
   LDY   #$41
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
   LDY   #$21
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
   JSR   dle
   LDA   HARGS+16
   LDY   #$49
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
   ADC   #$4E
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$4D
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
   LDY   #$28
   LDA   (FP),Y
   LDY   #$40
   CMP   (FP),Y
   BNE   .cmp_true@9
   LDY   #$27
   LDA   (FP),Y
   LDY   #$3F
   CMP   (FP),Y
   BNE   .cmp_true@9
   LDY   #$26
   LDA   (FP),Y
   LDY   #$3E
   CMP   (FP),Y
   BNE   .cmp_true@9
   LDY   #$25
   LDA   (FP),Y
   LDY   #$3D
   CMP   (FP),Y
   BNE   .cmp_true@9
   LDY   #$24
   LDA   (FP),Y
   LDY   #$3C
   CMP   (FP),Y
   BNE   .cmp_true@9
   LDY   #$23
   LDA   (FP),Y
   LDY   #$3B
   CMP   (FP),Y
   BNE   .cmp_true@9
   LDY   #$22
   LDA   (FP),Y
   LDY   #$3A
   CMP   (FP),Y
   BNE   .cmp_true@9
   LDY   #$21
   LDA   (FP),Y
   LDY   #$39
   CMP   (FP),Y
   BNE   .cmp_true@9
   LDA   #$00
   JMP   .cmp_end@10
.cmp_true@9:
   LDA   #$01
.cmp_end@10:
   LDY   #$4B
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
   ADC   #$4E
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
.if_end@5:
   LDX   #$00
   TXA

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$4E
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$4D
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

inf:
   DC.L  $00000000
   DC.L  $7FF00000

very_large:
   DC.L  $58ADBB99
   DC.L  $7FEFDCF1

zero:
   DS.B  8
