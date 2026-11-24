SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24

f:
   SUBROUTINE

   LDA   #$64
   STA   global
   LDA   #$00
   STA   global+1
   LDX   #$00
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

   LDA   #$00
   STA   global
   STA   global+1
   LDA   flag
   ORA   flag+1
   BEQ   .if_end@0
   JSR   f
   LDY   #$01
   STA   (FP),Y
   TXA
   INY
   STA   (FP),Y
.if_end@0:
   LDX   global+1
   LDA   global

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
   LDY   #$03
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

global:
   DS.B  2

flag:
   DC.W  $0001
