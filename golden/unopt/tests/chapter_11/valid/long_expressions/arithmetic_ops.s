SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24

addition:
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

   LDA   a
   CLC
   ADC   b
   LDY   #$01
   STA   (FP),Y
   LDA   a+1
   ADC   b+1
   INY
   STA   (FP),Y
   LDA   a+2
   ADC   b+2
   INY
   STA   (FP),Y
   LDA   a+3
   ADC   b+3
   INY
   STA   (FP),Y
   LDA   a+4
   ADC   b+4
   INY
   STA   (FP),Y
   LDA   a+5
   ADC   b+5
   INY
   STA   (FP),Y
   LDA   a+6
   ADC   b+6
   INY
   STA   (FP),Y
   LDA   a+7
   ADC   b+7
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

subtraction:
   SUBROUTINE

   ; prologue: 0 arg bytes, 18 local bytes
   SEC
   LDA   SSP
   SBC   #$14
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$13
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   LDA   a
   SEC
   SBC   b
   LDY   #$01
   STA   (FP),Y
   LDA   a+1
   SBC   b+1
   INY
   STA   (FP),Y
   LDA   a+2
   SBC   b+2
   INY
   STA   (FP),Y
   LDA   a+3
   SBC   b+3
   INY
   STA   (FP),Y
   LDA   a+4
   SBC   b+4
   INY
   STA   (FP),Y
   LDA   a+5
   SBC   b+5
   INY
   STA   (FP),Y
   LDA   a+6
   SBC   b+6
   INY
   STA   (FP),Y
   LDA   a+7
   SBC   b+7
   INY
   STA   (FP),Y
   SEC
   LDA   #$00
   SBC   #$5A
   INY
   STA   (FP),Y
   LDA   #$00
   SBC   #$94
   INY
   STA   (FP),Y
   LDA   #$00
   SBC   #$35
   INY
   STA   (FP),Y
   LDA   #$00
   SBC   #$77
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
   LDA   #$00
   SBC   #$00
   INY
   STA   (FP),Y
   LDA   #$00
   SBC   #$00
   INY
   STA   (FP),Y
   LDY   #$08
   LDA   (FP),Y
   LDY   #$10
   CMP   (FP),Y
   BNE   .thread@1
   LDY   #$07
   LDA   (FP),Y
   LDY   #$0F
   CMP   (FP),Y
   BNE   .thread@1
   LDY   #$06
   LDA   (FP),Y
   LDY   #$0E
   CMP   (FP),Y
   BNE   .thread@1
   LDY   #$05
   LDA   (FP),Y
   LDY   #$0D
   CMP   (FP),Y
   BNE   .thread@1
   LDY   #$04
   LDA   (FP),Y
   LDY   #$0C
   CMP   (FP),Y
   BNE   .thread@1
   LDY   #$03
   LDA   (FP),Y
   LDY   #$0B
   CMP   (FP),Y
   BNE   .thread@1
   LDY   #$02
   LDA   (FP),Y
   LDY   #$0A
   CMP   (FP),Y
   BNE   .thread@1
   LDY   #$01
   LDA   (FP),Y
   LDY   #$09
   CMP   (FP),Y
   BEQ   .cmp_true@3
.thread@1:
   LDA   #$00
   JMP   .cmp_end@4
.cmp_true@3:
   LDA   #$01
.cmp_end@4:
   LDY   #$11
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
   ADC   #$14
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$13
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

multiplication:
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

   LDA   a
   STA   HARGS
   LDA   a+1
   STA   HARGS+1
   LDA   a+2
   STA   HARGS+2
   LDA   a+3
   STA   HARGS+3
   LDA   a+4
   STA   HARGS+4
   LDA   a+5
   STA   HARGS+5
   LDA   a+6
   STA   HARGS+6
   LDA   a+7
   STA   HARGS+7
   LDA   #$04
   STA   HARGS+8
   LDA   #$00
   STA   HARGS+9
   LDY   #$00
   STY   HARGS+10
   STY   HARGS+11
   STY   HARGS+12
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
   CMP   #$00
   BNE   .thread@2
   DEY
   LDA   (FP),Y
   BNE   .thread@2
   DEY
   LDA   (FP),Y
   BNE   .thread@2
   DEY
   LDA   (FP),Y
   BNE   .thread@2
   DEY
   LDA   (FP),Y
   CMP   #$77
   BNE   .thread@2
   DEY
   LDA   (FP),Y
   CMP   #$35
   BNE   .thread@2
   DEY
   LDA   (FP),Y
   CMP   #$94
   BNE   .thread@2
   DEY
   LDA   (FP),Y
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

division:
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

   LDA   a
   STA   HARGS
   LDA   a+1
   STA   HARGS+1
   LDA   a+2
   STA   HARGS+2
   LDA   a+3
   STA   HARGS+3
   LDA   a+4
   STA   HARGS+4
   LDA   a+5
   STA   HARGS+5
   LDA   a+6
   STA   HARGS+6
   LDA   a+7
   STA   HARGS+7
   LDA   #$80
   STA   HARGS+8
   LDA   #$00
   STA   HARGS+9
   LDY   #$00
   STY   HARGS+10
   STY   HARGS+11
   STY   HARGS+12
   STY   HARGS+13
   STY   HARGS+14
   STA   HARGS+15
   JSR   sdivmod64
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
   STA   b
   INY
   LDA   (FP),Y
   STA   b+1
   INY
   LDA   (FP),Y
   STA   b+2
   INY
   LDA   (FP),Y
   STA   b+3
   INY
   LDA   (FP),Y
   STA   b+4
   INY
   LDA   (FP),Y
   STA   b+5
   INY
   LDA   (FP),Y
   STA   b+6
   INY
   LDA   (FP),Y
   STA   b+7
   BNE   .thread@3
   LDA   b+6
   BNE   .thread@3
   LDA   b+5
   BNE   .thread@3
   LDA   b+4
   BNE   .thread@3
   LDA   b+3
   BNE   .thread@3
   LDA   b+2
   CMP   #$0B
   BNE   .thread@3
   LDA   b+1
   CMP   #$EB
   BNE   .thread@3
   LDA   b
   CMP   #$C2
   BEQ   .cmp_true@9
.thread@3:
   LDA   #$00
   JMP   .cmp_end@10
.cmp_true@9:
   LDA   #$01
.cmp_end@10:
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

remaind:
   SUBROUTINE

   ; prologue: 0 arg bytes, 26 local bytes
   SEC
   LDA   SSP
   SBC   #$1C
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$1B
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
   SBC   a
   LDY   #$01
   STA   (FP),Y
   LDA   #$00
   SBC   a+1
   INY
   STA   (FP),Y
   LDA   #$00
   SBC   a+2
   INY
   STA   (FP),Y
   LDA   #$00
   SBC   a+3
   INY
   STA   (FP),Y
   LDA   #$00
   SBC   a+4
   INY
   STA   (FP),Y
   LDA   #$00
   SBC   a+5
   INY
   STA   (FP),Y
   LDA   #$00
   SBC   a+6
   INY
   STA   (FP),Y
   LDA   #$00
   SBC   a+7
   INY
   STA   (FP),Y
   LDY   #$01
   LDA   (FP),Y
   STA   HARGS
   INY
   LDA   (FP),Y
   STA   HARGS+1
   INY
   LDA   (FP),Y
   STA   HARGS+2
   INY
   LDA   (FP),Y
   STA   HARGS+3
   INY
   LDA   (FP),Y
   STA   HARGS+4
   INY
   LDA   (FP),Y
   STA   HARGS+5
   INY
   LDA   (FP),Y
   STA   HARGS+6
   INY
   LDA   (FP),Y
   STA   HARGS+7
   LDA   #$CA
   STA   HARGS+9
   LDA   #$9A
   STA   HARGS+10
   LDA   #$3B
   STA   HARGS+11
   LDA   #$00
   STA   HARGS+8
   STA   HARGS+12
   LDY   #$00
   STY   HARGS+13
   STY   HARGS+14
   STA   HARGS+15
   JSR   sdivmod64
   LDA   HARGS+24
   LDY   #$09
   STA   (FP),Y
   LDA   HARGS+25
   INY
   STA   (FP),Y
   LDA   HARGS+26
   INY
   STA   (FP),Y
   LDA   HARGS+27
   INY
   STA   (FP),Y
   LDA   HARGS+28
   INY
   STA   (FP),Y
   LDA   HARGS+29
   INY
   STA   (FP),Y
   LDA   HARGS+30
   INY
   STA   (FP),Y
   LDA   HARGS+31
   INY
   STA   (FP),Y
   LDY   #$09
   LDA   (FP),Y
   STA   b
   INY
   LDA   (FP),Y
   STA   b+1
   INY
   LDA   (FP),Y
   STA   b+2
   INY
   LDA   (FP),Y
   STA   b+3
   INY
   LDA   (FP),Y
   STA   b+4
   INY
   LDA   (FP),Y
   STA   b+5
   INY
   LDA   (FP),Y
   STA   b+6
   INY
   LDA   (FP),Y
   STA   b+7
   SEC
   LDA   #$00
   SBC   #$05
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
   LDA   #$00
   SBC   #$00
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
   LDA   #$00
   SBC   #$00
   INY
   STA   (FP),Y
   LDA   #$00
   SBC   #$00
   INY
   STA   (FP),Y
   LDA   b+7
   CMP   (FP),Y
   BNE   .thread@4
   LDA   b+6
   DEY
   CMP   (FP),Y
   BNE   .thread@4
   LDA   b+5
   DEY
   CMP   (FP),Y
   BNE   .thread@4
   LDA   b+4
   DEY
   CMP   (FP),Y
   BNE   .thread@4
   LDA   b+3
   DEY
   CMP   (FP),Y
   BNE   .thread@4
   LDA   b+2
   DEY
   CMP   (FP),Y
   BNE   .thread@4
   LDA   b+1
   DEY
   CMP   (FP),Y
   BNE   .thread@4
   LDA   b
   DEY
   CMP   (FP),Y
   BEQ   .cmp_true@12
.thread@4:
   LDA   #$00
   JMP   .cmp_end@13
.cmp_true@12:
   LDA   #$01
.cmp_end@13:
   LDY   #$19
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
   ADC   #$1C
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$1B
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

complement:
   SUBROUTINE

   ; prologue: 0 arg bytes, 18 local bytes
   SEC
   LDA   SSP
   SBC   #$14
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$13
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   LDA   a
   EOR   #$FF
   LDY   #$01
   STA   (FP),Y
   LDA   a+1
   EOR   #$FF
   INY
   STA   (FP),Y
   LDA   a+2
   EOR   #$FF
   INY
   STA   (FP),Y
   LDA   a+3
   EOR   #$FF
   INY
   STA   (FP),Y
   LDA   a+4
   EOR   #$FF
   INY
   STA   (FP),Y
   LDA   a+5
   EOR   #$FF
   INY
   STA   (FP),Y
   LDA   a+6
   EOR   #$FF
   INY
   STA   (FP),Y
   LDA   a+7
   EOR   #$FF
   INY
   STA   (FP),Y
   SEC
   LDA   #$00
   SBC   #$FF
   INY
   STA   (FP),Y
   LDA   #$00
   SBC   #$FF
   INY
   STA   (FP),Y
   LDA   #$00
   SBC   #$FF
   INY
   STA   (FP),Y
   LDA   #$00
   SBC   #$7F
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
   LDA   #$00
   SBC   #$00
   INY
   STA   (FP),Y
   LDA   #$00
   SBC   #$00
   INY
   STA   (FP),Y
   LDY   #$08
   LDA   (FP),Y
   LDY   #$10
   CMP   (FP),Y
   BNE   .thread@5
   LDY   #$07
   LDA   (FP),Y
   LDY   #$0F
   CMP   (FP),Y
   BNE   .thread@5
   LDY   #$06
   LDA   (FP),Y
   LDY   #$0E
   CMP   (FP),Y
   BNE   .thread@5
   LDY   #$05
   LDA   (FP),Y
   LDY   #$0D
   CMP   (FP),Y
   BNE   .thread@5
   LDY   #$04
   LDA   (FP),Y
   LDY   #$0C
   CMP   (FP),Y
   BNE   .thread@5
   LDY   #$03
   LDA   (FP),Y
   LDY   #$0B
   CMP   (FP),Y
   BNE   .thread@5
   LDY   #$02
   LDA   (FP),Y
   LDY   #$0A
   CMP   (FP),Y
   BNE   .thread@5
   LDY   #$01
   LDA   (FP),Y
   LDY   #$09
   CMP   (FP),Y
   BEQ   .cmp_true@15
.thread@5:
   LDA   #$00
   JMP   .cmp_end@16
.cmp_true@15:
   LDA   #$01
.cmp_end@16:
   LDY   #$11
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
   ADC   #$14
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$13
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

   ; prologue: 0 arg bytes, 32 local bytes
   SEC
   LDA   SSP
   SBC   #$22
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$21
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   LDA   #$F8
   STA   a
   LDA   #$FF
   STA   a+1
   STA   a+2
   LDA   #$7F
   STA   a+3
   LDA   #$05
   STA   b
   LDA   #$00
   STA   a+4
   STA   a+5
   STA   a+6
   STA   a+7
   STA   b+1
   STA   b+2
   STA   b+3
   STA   b+4
   STA   b+5
   STA   b+6
   STA   b+7
   JSR   addition
   LDY   #$01
   STA   (FP),Y
   TXA
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lnot_true@18
   LDA   #$00
   JMP   .lnot_end@19
.lnot_true@18:
   LDA   #$01
.lnot_end@19:
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
   ADC   #$22
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$21
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
   SBC   #$00
   LDY   #$05
   STA   (FP),Y
   LDA   #$00
   SBC   #$94
   INY
   STA   (FP),Y
   LDA   #$00
   SBC   #$35
   INY
   STA   (FP),Y
   LDA   #$00
   SBC   #$77
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
   LDA   #$00
   SBC   #$00
   INY
   STA   (FP),Y
   LDA   #$00
   SBC   #$00
   INY
   STA   (FP),Y
   LDY   #$05
   LDA   (FP),Y
   STA   a
   INY
   LDA   (FP),Y
   STA   a+1
   INY
   LDA   (FP),Y
   STA   a+2
   INY
   LDA   (FP),Y
   STA   a+3
   INY
   LDA   (FP),Y
   STA   a+4
   INY
   LDA   (FP),Y
   STA   a+5
   INY
   LDA   (FP),Y
   STA   a+6
   INY
   LDA   (FP),Y
   STA   a+7
   LDA   #$5A
   STA   b
   LDA   #$00
   STA   b+1
   STA   b+2
   STA   b+3
   STA   b+4
   STA   b+5
   STA   b+6
   STA   b+7
   JSR   subtraction
   LDY   #$0D
   STA   (FP),Y
   TXA
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lnot_true@20
   LDA   #$00
   JMP   .lnot_end@21
.lnot_true@20:
   LDA   #$01
.lnot_end@21:
   LDY   #$0F
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
   ADC   #$22
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$21
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
   LDA   #$65
   STA   a+1
   LDA   #$CD
   STA   a+2
   LDA   #$1D
   STA   a+3
   LDA   #$00
   STA   a
   STA   a+4
   STA   a+5
   STA   a+6
   STA   a+7
   JSR   multiplication
   LDY   #$11
   STA   (FP),Y
   TXA
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lnot_true@22
   LDA   #$00
   JMP   .lnot_end@23
.lnot_true@22:
   LDA   #$01
.lnot_end@23:
   LDY   #$13
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
   ADC   #$22
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$21
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
   LDA   #$E1
   STA   a+1
   LDA   #$F5
   STA   a+2
   LDA   #$05
   STA   a+3
   LDA   #$00
   STA   a
   STA   a+4
   STA   a+5
   STA   a+6
   STA   a+7
   JSR   division
   LDY   #$15
   STA   (FP),Y
   TXA
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lnot_true@24
   LDA   #$00
   JMP   .lnot_end@25
.lnot_true@24:
   LDA   #$01
.lnot_end@25:
   LDY   #$17
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
   ADC   #$22
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$21
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
   LDA   #$05
   STA   a
   LDA   #$CA
   STA   a+1
   LDA   #$9A
   STA   a+2
   LDA   #$3B
   STA   a+3
   LDA   #$00
   STA   a+4
   STA   a+5
   STA   a+6
   STA   a+7
   JSR   remaind
   LDY   #$19
   STA   (FP),Y
   TXA
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lnot_true@26
   LDA   #$00
   JMP   .lnot_end@27
.lnot_true@26:
   LDA   #$01
.lnot_end@27:
   LDY   #$1B
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
   ADC   #$22
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$21
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
   LDA   #$FE
   STA   a
   LDA   #$FF
   STA   a+1
   STA   a+2
   LDA   #$7F
   STA   a+3
   LDA   #$00
   STA   a+4
   STA   a+5
   STA   a+6
   STA   a+7
   JSR   complement
   LDY   #$1D
   STA   (FP),Y
   TXA
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lnot_true@28
   LDA   #$00
   JMP   .lnot_end@29
.lnot_true@28:
   LDA   #$01
.lnot_end@29:
   LDY   #$1F
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
   ADC   #$22
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
.if_end@5:
   LDX   #$00
   TXA

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$22
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$21
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

a:
   DS.B  8

b:
   DS.B  8
