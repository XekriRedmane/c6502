SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24

target:
   SUBROUTINE

   ; prologue: 0 arg bytes, 20 local bytes
   SEC
   LDA   SSP
   SBC   #$16
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$15
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
   SBC   #$FF
   LDY   #$01
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
   LDA   #$01
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   ORA   #$00
   BPL   .sx_done@1
   LDA   #$FF
.sx_done@1:
   LDY   #$07
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$01
   LDA   (FP),Y
   SEC
   LDY   #$05
   SBC   (FP),Y
   LDY   #$09
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   LDY   #$06
   SBC   (FP),Y
   LDY   #$0A
   STA   (FP),Y
   LDY   #$03
   LDA   (FP),Y
   LDY   #$07
   SBC   (FP),Y
   LDY   #$0B
   STA   (FP),Y
   LDY   #$04
   LDA   (FP),Y
   LDY   #$08
   SBC   (FP),Y
   LDY   #$0C
   STA   (FP),Y
   LDY   #$09
   LDA   (FP),Y
   LDY   #$0D
   STA   (FP),Y
   LDY   #$0A
   LDA   (FP),Y
   LDY   #$0E
   STA   (FP),Y
   LDY   #$0B
   LDA   (FP),Y
   LDY   #$0F
   STA   (FP),Y
   LDY   #$0C
   LDA   (FP),Y
   LDY   #$10
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@2
   LDA   #$00
   JMP   .sx_done@3
.sx_neg@2:
   LDA   #$FF
.sx_done@3:
   LDY   #$11
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$0D
   LDA   (FP),Y
   STA   HARGS+16
   INY
   LDA   (FP),Y
   STA   HARGS+17
   INY
   LDA   (FP),Y
   STA   HARGS+18
   INY
   LDA   (FP),Y
   STA   HARGS+19
   INY
   LDA   (FP),Y
   STA   HARGS+20
   INY
   LDA   (FP),Y
   STA   HARGS+21
   INY
   LDA   (FP),Y
   STA   HARGS+22
   INY
   LDA   (FP),Y
   STA   HARGS+23

   ; epilogue
   CLC
   LDA   FP
   ADC   #$16
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

   JSR   target
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
   EOR   #$FF
   LDY   #$09
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   EOR   #$FF
   LDY   #$0A
   STA   (FP),Y
   LDY   #$03
   LDA   (FP),Y
   EOR   #$FF
   LDY   #$0B
   STA   (FP),Y
   LDY   #$04
   LDA   (FP),Y
   EOR   #$FF
   LDY   #$0C
   STA   (FP),Y
   LDY   #$05
   LDA   (FP),Y
   EOR   #$FF
   LDY   #$0D
   STA   (FP),Y
   LDY   #$06
   LDA   (FP),Y
   EOR   #$FF
   LDY   #$0E
   STA   (FP),Y
   LDY   #$07
   LDA   (FP),Y
   EOR   #$FF
   LDY   #$0F
   STA   (FP),Y
   LDY   #$08
   LDA   (FP),Y
   EOR   #$FF
   LDY   #$10
   STA   (FP),Y
   LDA   #$FF
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$7F
   INY
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@4
   LDA   #$00
   JMP   .sx_done@5
.sx_neg@4:
   LDA   #$FF
.sx_done@5:
   LDY   #$15
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$10
   LDA   (FP),Y
   LDY   #$18
   CMP   (FP),Y
   BNE   .cmp_true@6
   LDY   #$0F
   LDA   (FP),Y
   LDY   #$17
   CMP   (FP),Y
   BNE   .cmp_true@6
   LDY   #$0E
   LDA   (FP),Y
   LDY   #$16
   CMP   (FP),Y
   BNE   .cmp_true@6
   LDY   #$0D
   LDA   (FP),Y
   LDY   #$15
   CMP   (FP),Y
   BNE   .cmp_true@6
   LDY   #$0C
   LDA   (FP),Y
   LDY   #$14
   CMP   (FP),Y
   BNE   .cmp_true@6
   LDY   #$0B
   LDA   (FP),Y
   LDY   #$13
   CMP   (FP),Y
   BNE   .cmp_true@6
   LDY   #$0A
   LDA   (FP),Y
   LDY   #$12
   CMP   (FP),Y
   BNE   .cmp_true@6
   LDY   #$09
   LDA   (FP),Y
   LDY   #$11
   CMP   (FP),Y
   BNE   .cmp_true@6
   LDA   #$00
   JMP   .cmp_end@7
.cmp_true@6:
   LDA   #$01
.cmp_end@7:
   LDY   #$19
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@0
   LDA   #$01
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   ORA   #$00
   BPL   .sx_done@10
   LDA   #$FF
.sx_done@10:
   LDY   #$1D
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
   LDY   #$1B
   LDA   (FP),Y
   STA   HARGS+16
   INY
   LDA   (FP),Y
   STA   HARGS+17
   INY
   LDA   (FP),Y
   STA   HARGS+18
   INY
   LDA   (FP),Y
   STA   HARGS+19
   INY
   LDA   (FP),Y
   STA   HARGS+20
   INY
   LDA   (FP),Y
   STA   HARGS+21
   INY
   LDA   (FP),Y
   STA   HARGS+22
   INY
   LDA   (FP),Y
   STA   HARGS+23

   ; epilogue
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
   RTS
.if_end@0:
   LDA   #$00
   LDY   #$23
   STA   (FP),Y
   INY
   STA   (FP),Y
   ORA   #$00
   BPL   .sx_done@12
   LDA   #$FF
.sx_done@12:
   LDY   #$25
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
   LDY   #$23
   LDA   (FP),Y
   STA   HARGS+16
   INY
   LDA   (FP),Y
   STA   HARGS+17
   INY
   LDA   (FP),Y
   STA   HARGS+18
   INY
   LDA   (FP),Y
   STA   HARGS+19
   INY
   LDA   (FP),Y
   STA   HARGS+20
   INY
   LDA   (FP),Y
   STA   HARGS+21
   INY
   LDA   (FP),Y
   STA   HARGS+22
   INY
   LDA   (FP),Y
   STA   HARGS+23

   ; epilogue
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
   RTS
