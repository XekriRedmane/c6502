SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24

print_alphabet:
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

   LDA   @1.count
   CLC
   ADC   #$41
   LDY   #$01
   STA   (FP),Y
   LDA   @1.count+1
   ADC   #$00
   INY
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$02
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   DEY
   LDA   (FP),Y
   STA   (SSP),Y
   INY
   LDA   (FP),Y
   STA   (SSP),Y
   JSR   putchar
   LDY   #$03
   STA   (FP),Y
   TXA
   INY
   STA   (FP),Y
   LDA   @1.count
   CLC
   ADC   #$01
   INY
   STA   (FP),Y
   LDA   @1.count+1
   ADC   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   @1.count
   INY
   LDA   (FP),Y
   STA   @1.count+1
   LDA   @1.count
   SEC
   SBC   #$1A
   LDA   @1.count+1
   SBC   #$00
   BVC   .cmp_novf@0
   EOR   #$80
.cmp_novf@0:
   ASL
   LDA   #$00
   ROL
   LDY   #$07
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@0
   JSR   print_alphabet
   LDY   #$09
   STA   (FP),Y
   TXA
   INY
   STA   (FP),Y
.if_end@0:
   LDX   @1.count+1
   LDA   @1.count

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

main:
   SUBROUTINE

   ; prologue: 0 arg bytes, 2 local bytes
   SEC
   LDA   SSP
   SBC   #$04
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$03
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   JSR   print_alphabet
   LDY   #$01
   STA   (FP),Y
   TXA
   INY
   STA   (FP),Y
   LDX   #$00
   TXA

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$04
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

@1.count:
   DS.B  2
