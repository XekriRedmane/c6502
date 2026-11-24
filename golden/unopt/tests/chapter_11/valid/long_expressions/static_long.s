SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24

main:
   SUBROUTINE

   ; prologue: 0 arg bytes, 12 local bytes
   SEC
   LDA   SSP
   SBC   #$0E
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$0D
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   LDA   foo
   CLC
   ADC   #$05
   LDY   #$01
   STA   (FP),Y
   LDA   foo+1
   ADC   #$00
   INY
   STA   (FP),Y
   LDA   foo+2
   ADC   #$00
   INY
   STA   (FP),Y
   LDA   foo+3
   ADC   #$00
   INY
   STA   (FP),Y
   LDA   foo+4
   ADC   #$00
   INY
   STA   (FP),Y
   LDA   foo+5
   ADC   #$00
   INY
   STA   (FP),Y
   LDA   foo+6
   ADC   #$00
   INY
   STA   (FP),Y
   LDA   foo+7
   ADC   #$00
   INY
   STA   (FP),Y
   CMP   #$00
   BNE   .thread@0
   DEY
   LDA   (FP),Y
   BNE   .thread@0
   DEY
   LDA   (FP),Y
   BNE   .thread@0
   DEY
   LDA   (FP),Y
   BNE   .thread@0
   DEY
   LDA   (FP),Y
   CMP   #$7F
   BNE   .thread@0
   DEY
   LDA   (FP),Y
   CMP   #$FF
   BNE   .thread@0
   DEY
   LDA   (FP),Y
   CMP   #$FF
   BNE   .thread@0
   DEY
   LDA   (FP),Y
   CMP   #$FD
   BEQ   .cmp_true@0
.thread@0:
   LDA   #$00
   JMP   .cmp_end@1
.cmp_true@0:
   LDA   #$01
.cmp_end@1:
   LDY   #$09
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@0
   LDA   #$40
   STA   foo+3
   LDA   #$00
   STA   foo
   STA   foo+1
   STA   foo+2
   STA   foo+4
   STA   foo+5
   STA   foo+6
   STA   foo+7
   BNE   .thread@1
   LDA   foo+3
   CMP   #$40
   BNE   .thread@1
   LDA   foo+2
   BNE   .thread@1
   BEQ   .cmp_true@3
.thread@1:
   LDA   #$00
   JMP   .cmp_end@4
.cmp_true@3:
   LDA   #$01
.cmp_end@4:
   LDY   #$0B
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
   LDA   #$01

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$0E
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
.if_end@0:
   LDX   #$00
   TXA

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$0E
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$0D
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

foo:
   DC.L  $7FFFFFF8
   DC.L  $00000000
