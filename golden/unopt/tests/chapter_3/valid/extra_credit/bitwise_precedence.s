SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24

main:
   SUBROUTINE

   ; prologue: 0 arg bytes, 10 local bytes
   SEC
   LDA   SSP
   SBC   #$0C
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$0B
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   LDA   #$00
   LDY   #$02
   STA   (FP),Y
   ASL
   LDA   (FP),Y
   ROR
   STA   (FP),Y
   LDA   #$50
   DEY
   STA   (FP),Y
   ROR
   STA   (FP),Y
   INY
   LDA   (FP),Y
   ASL
   LDA   (FP),Y
   ROR
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ROR
   STA   (FP),Y
   LDA   #$07
   LDY   #$03
   STA   (FP),Y
   ASL
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   ROL
   STA   (FP),Y
   LDA   #$05
   DEY
   AND   (FP),Y
   LDY   #$05
   STA   (FP),Y
   LDA   #$00
   DEY
   AND   (FP),Y
   LDY   #$06
   STA   (FP),Y
   LDA   #$01
   DEY
   EOR   (FP),Y
   LDY   #$07
   STA   (FP),Y
   LDA   #$00
   DEY
   EOR   (FP),Y
   LDY   #$08
   STA   (FP),Y
   LDY   #$01
   LDA   (FP),Y
   LDY   #$07
   ORA   (FP),Y
   LDY   #$09
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   LDY   #$08
   ORA   (FP),Y
   LDY   #$0A
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
   ADC   #$0C
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$0B
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
