SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24

initialize_letter:
   SUBROUTINE

   LDA   #$5A
   STA   letter
   LDA   #$00
   STA   letter+1
   RTS

decrement_letter:
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

   LDA   letter
   SEC
   SBC   #$01
   LDY   #$01
   STA   (FP),Y
   LDA   letter+1
   SBC   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   letter
   INY
   LDA   (FP),Y
   STA   letter+1

   ; epilogue
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
   RTS

main:
   SUBROUTINE

   ; prologue: 0 arg bytes, 16 local bytes
   SEC
   LDA   SSP
   SBC   #$12
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$11
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   JSR   initialize_letter
.loop@0_start:
   LDA   letter
   SEC
   SBC   #$41
   LDA   letter+1
   SBC   #$00
   BVC   .cmp_novf@0
   EOR   #$80
.cmp_novf@0:
   ASL
   LDA   #$00
   ROL
   EOR   #$01
   LDY   #$01
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .loop@0_break
   SEC
   LDA   SSP
   SBC   #$02
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   letter
   DEY
   STA   (SSP),Y
   LDA   letter+1
   INY
   STA   (SSP),Y
   JSR   putchar
   LDY   #$03
   STA   (FP),Y
   TXA
   INY
   STA   (FP),Y
   LDA   letter
   SEC
   SBC   #$01
   INY
   STA   (FP),Y
   LDA   letter+1
   SBC   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   letter
   INY
   LDA   (FP),Y
   STA   letter+1
   JMP   .loop@0_start
.loop@0_break:
   LDA   #$41
   STA   letter
   LDA   #$00
   STA   letter+1
.loop@1_start:
   LDA   #$5A
   SEC
   SBC   letter
   LDA   #$00
   SBC   letter+1
   BVC   .cmp_novf@1
   EOR   #$80
.cmp_novf@1:
   ASL
   LDA   #$00
   ROL
   EOR   #$01
   LDY   #$07
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .loop@1_break
   SEC
   LDA   SSP
   SBC   #$02
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   letter
   LDY   #$01
   STA   (SSP),Y
   LDA   letter+1
   INY
   STA   (SSP),Y
   JSR   putchar
   LDY   #$09
   STA   (FP),Y
   TXA
   INY
   STA   (FP),Y
   LDA   letter
   CLC
   ADC   #$01
   INY
   STA   (FP),Y
   LDA   letter+1
   ADC   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   letter
   INY
   LDA   (FP),Y
   STA   letter+1
   JMP   .loop@1_start
.loop@1_break:
   JSR   initialize_letter
.loop@2_start:
   LDA   letter
   SEC
   SBC   #$41
   LDA   letter+1
   SBC   #$00
   BVC   .cmp_novf@2
   EOR   #$80
.cmp_novf@2:
   ASL
   LDA   #$00
   ROL
   EOR   #$01
   LDY   #$0D
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .loop@2_break
   SEC
   LDA   SSP
   SBC   #$02
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   letter
   LDY   #$01
   STA   (SSP),Y
   LDA   letter+1
   INY
   STA   (SSP),Y
   JSR   putchar
   LDY   #$0F
   STA   (FP),Y
   TXA
   INY
   STA   (FP),Y
   JSR   decrement_letter
   JMP   .loop@2_start
.loop@2_break:
   LDX   #$00
   TXA

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$12
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$11
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

letter:
   DS.B  2
