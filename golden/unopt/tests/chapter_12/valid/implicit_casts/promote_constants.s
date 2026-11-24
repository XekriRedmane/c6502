SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24

main:
   SUBROUTINE

   ; prologue: 0 arg bytes, 42 local bytes
   SEC
   LDA   SSP
   SBC   #$2C
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$2B
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   SEC
   LDA   #$00
   SBC   negative_one
   LDY   #$01
   STA   (FP),Y
   LDA   #$00
   SBC   negative_one+1
   INY
   STA   (FP),Y
   LDA   #$00
   SBC   negative_one+2
   INY
   STA   (FP),Y
   LDA   #$00
   SBC   negative_one+3
   INY
   STA   (FP),Y
   LDA   #$00
   SBC   negative_one+4
   INY
   STA   (FP),Y
   LDA   #$00
   SBC   negative_one+5
   INY
   STA   (FP),Y
   LDA   #$00
   SBC   negative_one+6
   INY
   STA   (FP),Y
   LDA   #$00
   SBC   negative_one+7
   INY
   STA   (FP),Y
   LDY   #$01
   LDA   (FP),Y
   STA   negative_one
   INY
   LDA   (FP),Y
   STA   negative_one+1
   INY
   LDA   (FP),Y
   STA   negative_one+2
   INY
   LDA   (FP),Y
   STA   negative_one+3
   INY
   LDA   (FP),Y
   STA   negative_one+4
   INY
   LDA   (FP),Y
   STA   negative_one+5
   INY
   LDA   (FP),Y
   STA   negative_one+6
   INY
   LDA   (FP),Y
   STA   negative_one+7
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$04
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
   LDY   #$09
   LDA   (FP),Y
   SEC
   SBC   negative_one
   INY
   LDA   (FP),Y
   SBC   negative_one+1
   INY
   LDA   (FP),Y
   SBC   negative_one+2
   INY
   LDA   (FP),Y
   SBC   negative_one+3
   INY
   LDA   (FP),Y
   SBC   negative_one+4
   INY
   LDA   (FP),Y
   SBC   negative_one+5
   INY
   LDA   (FP),Y
   SBC   negative_one+6
   INY
   LDA   (FP),Y
   SBC   negative_one+7
   BVC   .cmp_novf@0
   EOR   #$80
.cmp_novf@0:
   ASL
   LDA   #$00
   ROL
   EOR   #$01
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
   ADC   #$2C
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$2B
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
   SEC
   LDA   #$00
   SBC   #$0A
   LDY   #$13
   STA   (FP),Y
   LDA   #$00
   SBC   #$80
   INY
   STA   (FP),Y
   LDA   #$00
   SBC   #$00
   INY
   STA   (FP),Y
   LDA   #$00
   SBC   #$00
   INY
   STA   (FP),Y
   LDY   #$13
   LDA   (FP),Y
   LDY   #$17
   STA   (FP),Y
   LDY   #$14
   LDA   (FP),Y
   LDY   #$18
   STA   (FP),Y
   LDY   #$15
   LDA   (FP),Y
   LDY   #$19
   STA   (FP),Y
   LDY   #$16
   LDA   (FP),Y
   LDY   #$1A
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@1
   LDA   #$00
   JMP   .sx_done@2
.sx_neg@1:
   LDA   #$FF
.sx_done@2:
   LDY   #$1B
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$17
   LDA   (FP),Y
   SEC
   SBC   zero
   INY
   LDA   (FP),Y
   SBC   zero+1
   INY
   LDA   (FP),Y
   SBC   zero+2
   INY
   LDA   (FP),Y
   SBC   zero+3
   INY
   LDA   (FP),Y
   SBC   zero+4
   INY
   LDA   (FP),Y
   SBC   zero+5
   INY
   LDA   (FP),Y
   SBC   zero+6
   INY
   LDA   (FP),Y
   SBC   zero+7
   BVC   .cmp_novf@3
   EOR   #$80
.cmp_novf@3:
   ASL
   LDA   #$00
   ROL
   EOR   #$01
   LDY   #$1F
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
   ADC   #$2C
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$2B
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
   LDA   #$03
   CLC
   ADC   #$FD
   LDY   #$21
   STA   (FP),Y
   LDA   #$00
   ADC   #$FF
   INY
   STA   (FP),Y
   LDA   #$00
   ADC   #$00
   INY
   STA   (FP),Y
   LDA   #$00
   ADC   #$00
   INY
   STA   (FP),Y
   LDA   #$00
   ADC   #$00
   INY
   STA   (FP),Y
   LDA   #$00
   ADC   #$00
   INY
   STA   (FP),Y
   LDA   #$00
   ADC   #$00
   INY
   STA   (FP),Y
   LDA   #$00
   ADC   #$00
   INY
   STA   (FP),Y
   LDY   #$21
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
   BEQ   .lnot_true@4
   LDA   #$00
   JMP   .lnot_end@5
.lnot_true@4:
   LDA   #$01
.lnot_end@5:
   LDY   #$29
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
   ADC   #$2C
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
.if_end@2:
   LDX   #$00
   TXA

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$2C
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$2B
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

negative_one:
   DC.L  $00000001
   DC.L  $00000000

zero:
   DS.B  8
