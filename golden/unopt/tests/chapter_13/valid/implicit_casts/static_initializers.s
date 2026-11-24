SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24

main:
   SUBROUTINE

   ; prologue: 0 arg bytes, 22 local bytes
   SEC
   LDA   SSP
   SBC   #$18
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$17
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   LDA   d1+7
   CMP   #$40
   BNE   .cmp_true@0
   LDA   d1+6
   CMP   #$DF
   BNE   .cmp_true@0
   LDA   d1+5
   CMP   #$FF
   BNE   .cmp_true@0
   LDA   d1+4
   CMP   #$C0
   BNE   .cmp_true@0
   LDA   d1+3
   BNE   .cmp_true@0
   LDA   d1+2
   BNE   .cmp_true@0
   LDA   d1+1
   BNE   .cmp_true@0
   LDA   d1
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
   ADC   #$18
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$17
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
   LDA   d2+7
   CMP   #$40
   BNE   .cmp_true@3
   LDA   d2+6
   CMP   #$EF
   BNE   .cmp_true@3
   LDA   d2+5
   CMP   #$FF
   BNE   .cmp_true@3
   LDA   d2+4
   CMP   #$E0
   BNE   .cmp_true@3
   LDA   d2+3
   BNE   .cmp_true@3
   LDA   d2+2
   BNE   .cmp_true@3
   LDA   d2+1
   BNE   .cmp_true@3
   LDA   d2
   BNE   .cmp_true@3
   LDA   #$00
   JMP   .cmp_end@4
.cmp_true@3:
   LDA   #$01
.cmp_end@4:
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
   ADC   #$18
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$17
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
   LDA   d3+7
   CMP   #$41
   BNE   .cmp_true@6
   LDA   d3+6
   CMP   #$CD
   BNE   .cmp_true@6
   LDA   d3+5
   CMP   #$CD
   BNE   .cmp_true@6
   LDA   d3+4
   CMP   #$65
   BNE   .cmp_true@6
   LDA   d3+3
   BNE   .cmp_true@6
   LDA   d3+2
   BNE   .cmp_true@6
   LDA   d3+1
   BNE   .cmp_true@6
   LDA   d3
   BNE   .cmp_true@6
   LDA   #$00
   JMP   .cmp_end@7
.cmp_true@6:
   LDA   #$01
.cmp_end@7:
   LDY   #$05
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
   ADC   #$18
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$17
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
   LDA   d4+7
   CMP   d3+7
   BNE   .cmp_true@9
   LDA   d4+6
   CMP   d3+6
   BNE   .cmp_true@9
   LDA   d4+5
   CMP   d3+5
   BNE   .cmp_true@9
   LDA   d4+4
   CMP   d3+4
   BNE   .cmp_true@9
   LDA   d4+3
   CMP   d3+3
   BNE   .cmp_true@9
   LDA   d4+2
   CMP   d3+2
   BNE   .cmp_true@9
   LDA   d4+1
   CMP   d3+1
   BNE   .cmp_true@9
   LDA   d4
   CMP   d3
   BNE   .cmp_true@9
   LDA   #$00
   JMP   .cmp_end@10
.cmp_true@9:
   LDA   #$01
.cmp_end@10:
   LDY   #$07
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
   ADC   #$18
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$17
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
   LDA   d5+7
   CMP   #$41
   BNE   .cmp_true@12
   LDA   d5+6
   CMP   #$ED
   BNE   .cmp_true@12
   LDA   d5+5
   CMP   #$CD
   BNE   .cmp_true@12
   LDA   d5+4
   CMP   #$65
   BNE   .cmp_true@12
   LDA   d5+3
   BNE   .cmp_true@12
   LDA   d5+2
   BNE   .cmp_true@12
   LDA   d5+1
   BNE   .cmp_true@12
   LDA   d5
   BNE   .cmp_true@12
   LDA   #$00
   JMP   .cmp_end@13
.cmp_true@12:
   LDA   #$01
.cmp_end@13:
   LDY   #$09
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
   ADC   #$18
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$17
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
   LDA   d6+7
   CMP   d3+7
   BNE   .cmp_true@15
   LDA   d6+6
   CMP   d3+6
   BNE   .cmp_true@15
   LDA   d6+5
   CMP   d3+5
   BNE   .cmp_true@15
   LDA   d6+4
   CMP   d3+4
   BNE   .cmp_true@15
   LDA   d6+3
   CMP   d3+3
   BNE   .cmp_true@15
   LDA   d6+2
   CMP   d3+2
   BNE   .cmp_true@15
   LDA   d6+1
   CMP   d3+1
   BNE   .cmp_true@15
   LDA   d6
   CMP   d3
   BNE   .cmp_true@15
   LDA   #$00
   JMP   .cmp_end@16
.cmp_true@15:
   LDA   #$01
.cmp_end@16:
   LDY   #$0B
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
   ADC   #$18
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$17
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
   LDA   d7+7
   CMP   d5+7
   BNE   .cmp_true@18
   LDA   d7+6
   CMP   d5+6
   BNE   .cmp_true@18
   LDA   d7+5
   CMP   d5+5
   BNE   .cmp_true@18
   LDA   d7+4
   CMP   d5+4
   BNE   .cmp_true@18
   LDA   d7+3
   CMP   d5+3
   BNE   .cmp_true@18
   LDA   d7+2
   CMP   d5+2
   BNE   .cmp_true@18
   LDA   d7+1
   CMP   d5+1
   BNE   .cmp_true@18
   LDA   d7
   CMP   d5
   BNE   .cmp_true@18
   LDA   #$00
   JMP   .cmp_end@19
.cmp_true@18:
   LDA   #$01
.cmp_end@19:
   LDY   #$0D
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
   ADC   #$18
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$17
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
   LDA   uninitialized
   ORA   uninitialized+1
   ORA   uninitialized+2
   ORA   uninitialized+3
   ORA   uninitialized+4
   ORA   uninitialized+5
   ORA   uninitialized+6
   ORA   uninitialized+7
   BEQ   .if_end@7
   LDX   #$00
   LDA   #$08

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$18
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$17
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
   LDA   i+1
   BNE   .cmp_true@21
   LDA   i
   CMP   #$04
   BNE   .cmp_true@21
   LDA   #$00
   JMP   .cmp_end@22
.cmp_true@21:
   LDA   #$01
.cmp_end@22:
   LDY   #$0F
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
   ADC   #$18
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$17
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
   LDA   u+3
   BNE   .cmp_true@24
   LDA   u+2
   BNE   .cmp_true@24
   LDA   u+1
   CMP   #$5B
   BNE   .cmp_true@24
   LDA   u
   CMP   #$E0
   BNE   .cmp_true@24
   LDA   #$00
   JMP   .cmp_end@25
.cmp_true@24:
   LDA   #$01
.cmp_end@25:
   LDY   #$11
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
   ADC   #$18
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$17
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
   LDA   l+7
   BNE   .cmp_true@27
   LDA   l+6
   BNE   .cmp_true@27
   LDA   l+5
   BNE   .cmp_true@27
   LDA   l+4
   BNE   .cmp_true@27
   LDA   l+3
   CMP   #$3B
   BNE   .cmp_true@27
   LDA   l+2
   CMP   #$9A
   BNE   .cmp_true@27
   LDA   l+1
   CMP   #$CA
   BNE   .cmp_true@27
   LDA   l
   BNE   .cmp_true@27
   LDA   #$00
   JMP   .cmp_end@28
.cmp_true@27:
   LDA   #$01
.cmp_end@28:
   LDY   #$13
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@10
   LDX   #$00
   LDA   #$0B

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$18
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$17
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
.if_end@10:
   LDA   ul+7
   BNE   .cmp_true@30
   LDA   ul+6
   BNE   .cmp_true@30
   LDA   ul+5
   BNE   .cmp_true@30
   LDA   ul+4
   BNE   .cmp_true@30
   LDA   ul+3
   CMP   #$EE
   BNE   .cmp_true@30
   LDA   ul+2
   CMP   #$6B
   BNE   .cmp_true@30
   LDA   ul+1
   CMP   #$28
   BNE   .cmp_true@30
   LDA   ul
   BNE   .cmp_true@30
   LDA   #$00
   JMP   .cmp_end@31
.cmp_true@30:
   LDA   #$01
.cmp_end@31:
   LDY   #$15
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@11
   LDX   #$00
   LDA   #$0C

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$18
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
.if_end@11:
   LDX   #$00
   TXA

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$18
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$17
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

d1:
   DC.L  $00000000
   DC.L  $40DFFFC0

d2:
   DC.L  $00000000
   DC.L  $40EFFFE0

d3:
   DC.L  $00000000
   DC.L  $41CDCD65

d4:
   DC.L  $00000000
   DC.L  $41CDCD65

d5:
   DC.L  $00000000
   DC.L  $41EDCD65

d6:
   DC.L  $00000000
   DC.L  $41CDCD65

d7:
   DC.L  $00000000
   DC.L  $41EDCD65

uninitialized:
   DS.B  8

i:
   DC.W  $0004

u:
   DC.L  $028F5BE0

l:
   DC.L  $3B9ACA00
   DC.L  $00000000

ul:
   DC.L  $EE6B2800
   DC.L  $00000000
