SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24

target:
   SUBROUTINE

   ; prologue: 12 arg bytes, 36 local bytes
   SEC
   LDA   SSP
   SBC   #$26
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$25
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   LDY   #$2B
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   LDY   #$2C
   LDA   (FP),Y
   LDY   #$02
   STA   (FP),Y
   LDY   #$2D
   LDA   (FP),Y
   LDY   #$03
   STA   (FP),Y
   LDY   #$2E
   LDA   (FP),Y
   LDY   #$04
   STA   (FP),Y
   LDY   #$2F
   LDA   (FP),Y
   LDY   #$05
   STA   (FP),Y
   LDY   #$30
   LDA   (FP),Y
   LDY   #$06
   STA   (FP),Y
   LDY   #$31
   LDA   (FP),Y
   LDY   #$07
   STA   (FP),Y
   LDY   #$32
   LDA   (FP),Y
   LDY   #$08
   STA   (FP),Y
   LDY   #$27
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@0
   LDY   #$01
   LDA   (FP),Y
   LDY   #$2B
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   LDY   #$2C
   STA   (FP),Y
   LDY   #$03
   LDA   (FP),Y
   LDY   #$2D
   STA   (FP),Y
   LDY   #$04
   LDA   (FP),Y
   LDY   #$2E
   STA   (FP),Y
   LDY   #$05
   LDA   (FP),Y
   LDY   #$2F
   STA   (FP),Y
   LDY   #$06
   LDA   (FP),Y
   LDY   #$30
   STA   (FP),Y
   LDY   #$07
   LDA   (FP),Y
   LDY   #$31
   STA   (FP),Y
   LDY   #$08
   LDA   (FP),Y
   LDY   #$32
   STA   (FP),Y
.if_end@0:
   LDY   #$29
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@1
   INY
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   LDY   #$2C
   LDA   (FP),Y
   LDY   #$02
   STA   (FP),Y
   LDY   #$2D
   LDA   (FP),Y
   LDY   #$03
   STA   (FP),Y
   LDY   #$2E
   LDA   (FP),Y
   LDY   #$04
   STA   (FP),Y
   LDY   #$2F
   LDA   (FP),Y
   LDY   #$05
   STA   (FP),Y
   LDY   #$30
   LDA   (FP),Y
   LDY   #$06
   STA   (FP),Y
   LDY   #$31
   LDA   (FP),Y
   LDY   #$07
   STA   (FP),Y
   LDY   #$32
   LDA   (FP),Y
   LDY   #$08
   STA   (FP),Y
.if_end@1:
   CLC
   LDA   FP
   ADC   #$01
   LDY   #$09
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$0B
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$0C
   STA   (FP),Y
   LDY   #$02
   LDA   (DPTR),Y
   LDY   #$0D
   STA   (FP),Y
   LDY   #$03
   LDA   (DPTR),Y
   LDY   #$0E
   STA   (FP),Y
   LDY   #$04
   LDA   (DPTR),Y
   LDY   #$0F
   STA   (FP),Y
   LDY   #$05
   LDA   (DPTR),Y
   LDY   #$10
   STA   (FP),Y
   LDY   #$06
   LDA   (DPTR),Y
   LDY   #$11
   STA   (FP),Y
   LDY   #$07
   LDA   (DPTR),Y
   LDY   #$12
   STA   (FP),Y
   CLC
   LDA   FP
   ADC   #$2B
   INY
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$15
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$16
   STA   (FP),Y
   LDY   #$02
   LDA   (DPTR),Y
   LDY   #$17
   STA   (FP),Y
   LDY   #$03
   LDA   (DPTR),Y
   LDY   #$18
   STA   (FP),Y
   LDY   #$04
   LDA   (DPTR),Y
   LDY   #$19
   STA   (FP),Y
   LDY   #$05
   LDA   (DPTR),Y
   LDY   #$1A
   STA   (FP),Y
   LDY   #$06
   LDA   (DPTR),Y
   LDY   #$1B
   STA   (FP),Y
   LDY   #$07
   LDA   (DPTR),Y
   LDY   #$1C
   STA   (FP),Y
   LDY   #$0B
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
   LDY   #$15
   LDA   (FP),Y
   STA   HARGS+8
   INY
   LDA   (FP),Y
   STA   HARGS+9
   INY
   LDA   (FP),Y
   STA   HARGS+10
   INY
   LDA   (FP),Y
   STA   HARGS+11
   INY
   LDA   (FP),Y
   STA   HARGS+12
   INY
   LDA   (FP),Y
   STA   HARGS+13
   INY
   LDA   (FP),Y
   STA   HARGS+14
   INY
   LDA   (FP),Y
   STA   HARGS+15
   JSR   dadd
   LDA   HARGS+16
   LDY   #$1D
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
   LDY   #$1D
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
   ADC   #$32
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

   CLC
   LDA   FP
   ADC   #$01
   LDY   #$09
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDA   #$00
   LDY   #$00
   STA   (DPTR),Y
   INY
   STA   (DPTR),Y
   INY
   STA   (DPTR),Y
   INY
   STA   (DPTR),Y
   INY
   STA   (DPTR),Y
   INY
   STA   (DPTR),Y
   LDA   #$39
   INY
   STA   (DPTR),Y
   LDA   #$40
   INY
   STA   (DPTR),Y
   SEC
   LDA   SSP
   SBC   #$0C
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   #$00
   LDY   #$01
   STA   (SSP),Y
   INY
   STA   (SSP),Y
   LDA   #$01
   INY
   STA   (SSP),Y
   LDA   #$00
   INY
   STA   (SSP),Y
   LDY   #$01
   LDA   (FP),Y
   LDY   #$05
   STA   (SSP),Y
   LDY   #$02
   LDA   (FP),Y
   LDY   #$06
   STA   (SSP),Y
   LDY   #$03
   LDA   (FP),Y
   LDY   #$07
   STA   (SSP),Y
   LDY   #$04
   LDA   (FP),Y
   LDY   #$08
   STA   (SSP),Y
   LDY   #$05
   LDA   (FP),Y
   LDY   #$09
   STA   (SSP),Y
   LDY   #$06
   LDA   (FP),Y
   LDY   #$0A
   STA   (SSP),Y
   LDY   #$07
   LDA   (FP),Y
   LDY   #$0B
   STA   (SSP),Y
   LDY   #$08
   LDA   (FP),Y
   LDY   #$0C
   STA   (SSP),Y
   JSR   target
   LDA   HARGS+16
   LDY   #$0B
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
   CMP   #$40
   BNE   .thread@0
   DEY
   LDA   (FP),Y
   CMP   #$49
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
   BNE   .thread@0
   DEY
   LDA   (FP),Y
   BNE   .thread@0
   DEY
   LDA   (FP),Y
   BEQ   .cmp_true@0
.thread@0:
   LDA   #$00
   JMP   .cmp_end@1
.cmp_true@0:
   LDA   #$01
.cmp_end@1:
   LDY   #$13
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
   ADC   #$16
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$15
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
