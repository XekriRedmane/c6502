SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24

add_large:
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

   LDA   x
   CLC
   ADC   #$F8
   LDY   #$01
   STA   (FP),Y
   LDA   x+1
   ADC   #$FF
   INY
   STA   (FP),Y
   LDA   x+2
   ADC   #$FF
   INY
   STA   (FP),Y
   LDA   x+3
   ADC   #$7F
   INY
   STA   (FP),Y
   LDA   x+4
   ADC   #$00
   INY
   STA   (FP),Y
   LDA   x+5
   ADC   #$00
   INY
   STA   (FP),Y
   LDA   x+6
   ADC   #$00
   INY
   STA   (FP),Y
   LDA   x+7
   ADC   #$00
   INY
   STA   (FP),Y
   LDY   #$01
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
   BNE   .thread@0
   LDA   x+6
   BNE   .thread@0
   LDA   x+5
   BNE   .thread@0
   LDA   x+4
   BNE   .thread@0
   LDA   x+3
   CMP   #$7F
   BNE   .thread@0
   LDA   x+2
   CMP   #$FF
   BNE   .thread@0
   LDA   x+1
   CMP   #$FF
   BNE   .thread@0
   LDA   x
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

subtract_large:
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

   LDA   x
   SEC
   SBC   #$F8
   LDY   #$01
   STA   (FP),Y
   LDA   x+1
   SBC   #$FF
   INY
   STA   (FP),Y
   LDA   x+2
   SBC   #$FF
   INY
   STA   (FP),Y
   LDA   x+3
   SBC   #$7F
   INY
   STA   (FP),Y
   LDA   x+4
   SBC   #$00
   INY
   STA   (FP),Y
   LDA   x+5
   SBC   #$00
   INY
   STA   (FP),Y
   LDA   x+6
   SBC   #$00
   INY
   STA   (FP),Y
   LDA   x+7
   SBC   #$00
   INY
   STA   (FP),Y
   LDY   #$01
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
   BNE   .thread@1
   LDA   x+6
   BNE   .thread@1
   LDA   x+5
   BNE   .thread@1
   LDA   x+4
   BNE   .thread@1
   LDA   x+3
   BNE   .thread@1
   LDA   x+2
   BNE   .thread@1
   LDA   x+1
   BNE   .thread@1
   LDA   x
   CMP   #$05
   BEQ   .cmp_true@3
.thread@1:
   LDA   #$00
   JMP   .cmp_end@4
.cmp_true@3:
   LDA   #$01
.cmp_end@4:
   LDY   #$09
   STA   (FP),Y
   LDA   #$00
   INY
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

multiply_by_large:
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

   LDA   x
   STA   HARGS
   LDA   x+1
   STA   HARGS+1
   LDA   x+2
   STA   HARGS+2
   LDA   x+3
   STA   HARGS+3
   LDA   x+4
   STA   HARGS+4
   LDA   x+5
   STA   HARGS+5
   LDA   x+6
   STA   HARGS+6
   LDA   x+7
   STA   HARGS+7
   LDA   #$C2
   STA   HARGS+9
   LDA   #$EB
   STA   HARGS+10
   LDA   #$0B
   STA   HARGS+11
   LDA   #$00
   STA   HARGS+8
   STA   HARGS+12
   LDY   #$00
   STY   HARGS+13
   STY   HARGS+14
   STA   HARGS+15
   JSR   mul64
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
   BNE   .thread@2
   LDA   x+6
   BNE   .thread@2
   LDA   x+5
   BNE   .thread@2
   LDA   x+4
   BNE   .thread@2
   LDA   x+3
   CMP   #$3B
   BNE   .thread@2
   LDA   x+2
   CMP   #$9A
   BNE   .thread@2
   LDA   x+1
   CMP   #$CA
   BNE   .thread@2
   LDA   x
   BEQ   .cmp_true@6
.thread@2:
   LDA   #$00
   JMP   .cmp_end@7
.cmp_true@6:
   LDA   #$01
.cmp_end@7:
   LDY   #$09
   STA   (FP),Y
   LDA   #$00
   INY
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

   JSR   add_large
   LDY   #$01
   STA   (FP),Y
   TXA
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lnot_true@9
   LDA   #$00
   JMP   .lnot_end@10
.lnot_true@9:
   LDA   #$01
.lnot_end@10:
   LDY   #$03
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
.if_end@0:
   JSR   subtract_large
   LDY   #$05
   STA   (FP),Y
   TXA
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lnot_true@11
   LDA   #$00
   JMP   .lnot_end@12
.lnot_true@11:
   LDA   #$01
.lnot_end@12:
   LDY   #$07
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
.if_end@1:
   JSR   multiply_by_large
   LDY   #$09
   STA   (FP),Y
   TXA
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lnot_true@13
   LDA   #$00
   JMP   .lnot_end@14
.lnot_true@13:
   LDA   #$01
.lnot_end@14:
   LDY   #$0B
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
.if_end@2:
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

x:
   DC.L  $00000005
   DC.L  $00000000
