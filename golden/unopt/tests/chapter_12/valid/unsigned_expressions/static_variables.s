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

   LDA   x+7
   BNE   .cmp_true@0
   LDA   x+6
   BNE   .cmp_true@0
   LDA   x+5
   BNE   .cmp_true@0
   LDA   x+4
   BNE   .cmp_true@0
   LDA   x+3
   CMP   #$7F
   BNE   .cmp_true@0
   LDA   x+2
   CMP   #$FF
   BNE   .cmp_true@0
   LDA   x+1
   CMP   #$FF
   BNE   .cmp_true@0
   LDA   x
   CMP   #$FB
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
.if_end@0:
   LDA   #$0A
   LDY   #$03
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   ORA   #$00
   BPL   .sx_done@4
   LDA   #$FF
.sx_done@4:
   LDY   #$05
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
   LDA   x
   CLC
   LDY   #$03
   ADC   (FP),Y
   LDY   #$0B
   STA   (FP),Y
   LDA   x+1
   LDY   #$04
   ADC   (FP),Y
   LDY   #$0C
   STA   (FP),Y
   LDA   x+2
   LDY   #$05
   ADC   (FP),Y
   LDY   #$0D
   STA   (FP),Y
   LDA   x+3
   LDY   #$06
   ADC   (FP),Y
   LDY   #$0E
   STA   (FP),Y
   LDA   x+4
   LDY   #$07
   ADC   (FP),Y
   LDY   #$0F
   STA   (FP),Y
   LDA   x+5
   LDY   #$08
   ADC   (FP),Y
   LDY   #$10
   STA   (FP),Y
   LDA   x+6
   LDY   #$09
   ADC   (FP),Y
   LDY   #$11
   STA   (FP),Y
   LDA   x+7
   LDY   #$0A
   ADC   (FP),Y
   LDY   #$12
   STA   (FP),Y
   LDY   #$0B
   LDA   (FP),Y
   STA   x
   INY
   LDA   (FP),Y
   STA   x+1
   INY
   LDA   (FP),Y
   STA   x+2
   INY
   LDA   (FP),Y
   STA   x+3
   INY
   LDA   (FP),Y
   STA   x+4
   INY
   LDA   (FP),Y
   STA   x+5
   INY
   LDA   (FP),Y
   STA   x+6
   INY
   LDA   (FP),Y
   STA   x+7
   BNE   .cmp_true@5
   LDA   x+6
   BNE   .cmp_true@5
   LDA   x+5
   BNE   .cmp_true@5
   LDA   x+4
   BNE   .cmp_true@5
   LDA   x+3
   CMP   #$80
   BNE   .cmp_true@5
   LDA   x+2
   BNE   .cmp_true@5
   LDA   x+1
   BNE   .cmp_true@5
   LDA   x
   CMP   #$05
   BNE   .cmp_true@5
   LDA   #$00
   JMP   .cmp_end@6
.cmp_true@5:
   LDA   #$01
.cmp_end@6:
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
.if_end@1:
   LDA   zero_long
   ORA   zero_long+1
   ORA   zero_long+2
   ORA   zero_long+3
   ORA   zero_long+4
   ORA   zero_long+5
   ORA   zero_long+6
   ORA   zero_long+7
   BNE   .or_true@2
   LDA   zero_int
   ORA   zero_int+1
   BNE   .or_true@2
   LDA   #$00
   LDY   #$15
   STA   (FP),Y
   INY
   STA   (FP),Y
   JMP   .or_end@3
.or_true@2:
   LDA   #$01
   LDY   #$15
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@3:
   LDY   #$15
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@4
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
.if_end@4:
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

x:
   DC.L  $7FFFFFFB
   DC.L  $00000000

zero_long:
   DS.B  8

zero_int:
   DS.B  2
