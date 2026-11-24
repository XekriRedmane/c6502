SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24

main:
   SUBROUTINE

   ; prologue: 0 arg bytes, 36 local bytes
   SEC
   LDA   SSP
   SBC   #$26
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$25
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   SEC
   LDA   SSP
   SBC   #$18
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
   LDA   #$14
   INY
   STA   (SSP),Y
   LDA   #$40
   INY
   STA   (SSP),Y
   LDA   #$92
   INY
   STA   (SSP),Y
   LDA   #$D5
   INY
   STA   (SSP),Y
   LDA   #$4D
   INY
   STA   (SSP),Y
   LDA   #$06
   INY
   STA   (SSP),Y
   LDA   #$CF
   INY
   STA   (SSP),Y
   LDA   #$F0
   INY
   STA   (SSP),Y
   LDA   #$80
   INY
   STA   (SSP),Y
   LDA   #$44
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
   LDA   #$80
   INY
   STA   (SSP),Y
   LDA   #$84
   INY
   STA   (SSP),Y
   LDA   #$4E
   INY
   STA   (SSP),Y
   LDA   #$41
   INY
   STA   (SSP),Y
   JSR   fma
   LDA   HARGS+16
   LDY   #$01
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
   SEC
   LDA   SSP
   SBC   #$0A
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   #$C8
   LDY   #$01
   STA   (SSP),Y
   LDA   #$CA
   INY
   STA   (SSP),Y
   LDA   #$B0
   INY
   STA   (SSP),Y
   LDA   #$C4
   INY
   STA   (SSP),Y
   LDA   #$9C
   INY
   STA   (SSP),Y
   LDA   #$45
   INY
   STA   (SSP),Y
   LDA   #$80
   INY
   STA   (SSP),Y
   LDA   #$4F
   INY
   STA   (SSP),Y
   LDA   #$05
   INY
   STA   (SSP),Y
   LDA   #$00
   INY
   STA   (SSP),Y
   JSR   ldexp
   LDA   HARGS+16
   LDY   #$11
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
   LDY   #$14
   LDA   (FP),Y
   LDY   #$1C
   STA   (FP),Y
   LDY   #$15
   LDA   (FP),Y
   LDY   #$1D
   STA   (FP),Y
   LDY   #$16
   LDA   (FP),Y
   LDY   #$1E
   STA   (FP),Y
   LDY   #$17
   LDA   (FP),Y
   LDY   #$1F
   STA   (FP),Y
   LDY   #$18
   LDA   (FP),Y
   LDY   #$20
   STA   (FP),Y
   LDY   #$10
   LDA   (FP),Y
   CMP   #$44
   BNE   .cmp_true@0
   DEY
   LDA   (FP),Y
   CMP   #$A5
   BNE   .cmp_true@0
   DEY
   LDA   (FP),Y
   CMP   #$2D
   BNE   .cmp_true@0
   DEY
   LDA   (FP),Y
   CMP   #$02
   BNE   .cmp_true@0
   DEY
   LDA   (FP),Y
   CMP   #$C7
   BNE   .cmp_true@0
   DEY
   LDA   (FP),Y
   CMP   #$E1
   BNE   .cmp_true@0
   DEY
   LDA   (FP),Y
   CMP   #$4A
   BNE   .cmp_true@0
   DEY
   LDA   (FP),Y
   CMP   #$F7
   BNE   .cmp_true@0
   LDA   #$00
   JMP   .cmp_end@1
.cmp_true@0:
   LDA   #$01
.cmp_end@1:
   LDY   #$21
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
   ADC   #$26
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$25
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
   LDY   #$20
   LDA   (FP),Y
   CMP   #$4F
   BNE   .cmp_true@3
   DEY
   LDA   (FP),Y
   CMP   #$D0
   BNE   .cmp_true@3
   DEY
   LDA   (FP),Y
   CMP   #$45
   BNE   .cmp_true@3
   DEY
   LDA   (FP),Y
   CMP   #$9C
   BNE   .cmp_true@3
   DEY
   LDA   (FP),Y
   CMP   #$C4
   BNE   .cmp_true@3
   DEY
   LDA   (FP),Y
   CMP   #$B0
   BNE   .cmp_true@3
   DEY
   LDA   (FP),Y
   CMP   #$CA
   BNE   .cmp_true@3
   DEY
   LDA   (FP),Y
   CMP   #$C8
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
   TAX
   PLA
   RTS
.if_end@1:
   LDX   #$00
   TXA

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$26
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$25
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
