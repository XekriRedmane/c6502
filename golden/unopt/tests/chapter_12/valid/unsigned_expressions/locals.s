SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24

main:
   SUBROUTINE

   ; prologue: 0 arg bytes, 90 local bytes
   SEC
   LDA   SSP
   SBC   #$5C
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$5B
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   LDA   #$00
   LDY   #$01
   STA   (FP),Y
   LDA   #$CA
   INY
   STA   (FP),Y
   LDA   #$9A
   INY
   STA   (FP),Y
   LDA   #$3B
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
   SEC
   SBC   #$01
   INY
   STA   (FP),Y
   LDA   #$00
   SBC   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$0B
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$0C
   STA   (FP),Y
   SEC
   LDA   #$00
   SBC   #$00
   INY
   STA   (FP),Y
   LDA   #$00
   SBC   #$CA
   INY
   STA   (FP),Y
   LDA   #$00
   SBC   #$9A
   INY
   STA   (FP),Y
   LDA   #$00
   SBC   #$3B
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
   LDY   #$0D
   LDA   (FP),Y
   LDY   #$15
   STA   (FP),Y
   LDY   #$0E
   LDA   (FP),Y
   LDY   #$16
   STA   (FP),Y
   LDY   #$0F
   LDA   (FP),Y
   LDY   #$17
   STA   (FP),Y
   LDY   #$10
   LDA   (FP),Y
   LDY   #$18
   STA   (FP),Y
   LDY   #$11
   LDA   (FP),Y
   LDY   #$19
   STA   (FP),Y
   LDY   #$12
   LDA   (FP),Y
   LDY   #$1A
   STA   (FP),Y
   LDY   #$13
   LDA   (FP),Y
   LDY   #$1B
   STA   (FP),Y
   LDY   #$14
   LDA   (FP),Y
   LDY   #$1C
   STA   (FP),Y
   LDA   #$0A
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$08
   LDA   (FP),Y
   BNE   .cmp_true@0
   LDY   #$04
   LDA   (FP),Y
   CMP   #$3B
   BNE   .cmp_true@0
   DEY
   LDA   (FP),Y
   CMP   #$9A
   BNE   .cmp_true@0
   DEY
   LDA   (FP),Y
   CMP   #$CA
   BNE   .cmp_true@0
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@0
   LDA   #$00
   JMP   .cmp_end@1
.cmp_true@0:
   LDA   #$01
.cmp_end@1:
   LDY   #$21
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
   ADC   #$5C
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$5B
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
   SBC   #$01
   LDY   #$23
   STA   (FP),Y
   LDA   #$00
   SBC   #$00
   INY
   STA   (FP),Y
   LDY   #$0C
   LDA   (FP),Y
   LDY   #$24
   CMP   (FP),Y
   BNE   .cmp_true@3
   LDY   #$0B
   LDA   (FP),Y
   LDY   #$23
   CMP   (FP),Y
   BNE   .cmp_true@3
   LDA   #$00
   JMP   .cmp_end@4
.cmp_true@3:
   LDA   #$01
.cmp_end@4:
   LDY   #$25
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
   ADC   #$5C
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$5B
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
   SEC
   LDA   #$00
   SBC   #$00
   LDY   #$27
   STA   (FP),Y
   LDA   #$00
   SBC   #$CA
   INY
   STA   (FP),Y
   LDA   #$00
   SBC   #$9A
   INY
   STA   (FP),Y
   LDA   #$00
   SBC   #$3B
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
   LDY   #$1C
   LDA   (FP),Y
   LDY   #$2E
   CMP   (FP),Y
   BNE   .cmp_true@6
   LDY   #$1B
   LDA   (FP),Y
   LDY   #$2D
   CMP   (FP),Y
   BNE   .cmp_true@6
   LDY   #$1A
   LDA   (FP),Y
   LDY   #$2C
   CMP   (FP),Y
   BNE   .cmp_true@6
   LDY   #$19
   LDA   (FP),Y
   LDY   #$2B
   CMP   (FP),Y
   BNE   .cmp_true@6
   LDY   #$18
   LDA   (FP),Y
   LDY   #$2A
   CMP   (FP),Y
   BNE   .cmp_true@6
   LDY   #$17
   LDA   (FP),Y
   LDY   #$29
   CMP   (FP),Y
   BNE   .cmp_true@6
   LDY   #$16
   LDA   (FP),Y
   LDY   #$28
   CMP   (FP),Y
   BNE   .cmp_true@6
   LDY   #$15
   LDA   (FP),Y
   LDY   #$27
   CMP   (FP),Y
   BNE   .cmp_true@6
   LDA   #$00
   JMP   .cmp_end@7
.cmp_true@6:
   LDA   #$01
.cmp_end@7:
   LDY   #$2F
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
   ADC   #$5C
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$5B
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
   LDY   #$20
   LDA   (FP),Y
   BNE   .cmp_true@9
   LDY   #$1D
   LDA   (FP),Y
   CMP   #$0A
   BNE   .cmp_true@9
   LDA   #$00
   JMP   .cmp_end@10
.cmp_true@9:
   LDA   #$01
.cmp_end@10:
   LDY   #$31
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
   ADC   #$5C
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$5B
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
   SEC
   LDA   #$00
   LDY   #$01
   SBC   (FP),Y
   LDY   #$33
   STA   (FP),Y
   LDA   #$00
   LDY   #$02
   SBC   (FP),Y
   LDY   #$34
   STA   (FP),Y
   LDA   #$00
   LDY   #$03
   SBC   (FP),Y
   LDY   #$35
   STA   (FP),Y
   LDA   #$00
   LDY   #$04
   SBC   (FP),Y
   LDY   #$36
   STA   (FP),Y
   LDA   #$00
   LDY   #$05
   SBC   (FP),Y
   LDY   #$37
   STA   (FP),Y
   LDA   #$00
   LDY   #$06
   SBC   (FP),Y
   LDY   #$38
   STA   (FP),Y
   LDA   #$00
   LDY   #$07
   SBC   (FP),Y
   LDY   #$39
   STA   (FP),Y
   LDA   #$00
   LDY   #$08
   SBC   (FP),Y
   LDY   #$3A
   STA   (FP),Y
   LDY   #$33
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   LDY   #$34
   LDA   (FP),Y
   LDY   #$02
   STA   (FP),Y
   LDY   #$35
   LDA   (FP),Y
   LDY   #$03
   STA   (FP),Y
   LDY   #$36
   LDA   (FP),Y
   LDY   #$04
   STA   (FP),Y
   LDY   #$37
   LDA   (FP),Y
   LDY   #$05
   STA   (FP),Y
   LDY   #$38
   LDA   (FP),Y
   LDY   #$06
   STA   (FP),Y
   LDY   #$39
   LDA   (FP),Y
   LDY   #$07
   STA   (FP),Y
   LDY   #$3A
   LDA   (FP),Y
   LDY   #$08
   STA   (FP),Y
   LDY   #$0B
   LDA   (FP),Y
   SEC
   SBC   #$01
   LDY   #$3B
   STA   (FP),Y
   LDY   #$0C
   LDA   (FP),Y
   SBC   #$00
   LDY   #$3C
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$0B
   STA   (FP),Y
   LDY   #$3C
   LDA   (FP),Y
   LDY   #$0C
   STA   (FP),Y
   LDY   #$15
   LDA   (FP),Y
   CLC
   ADC   #$02
   LDY   #$3D
   STA   (FP),Y
   LDY   #$16
   LDA   (FP),Y
   ADC   #$CA
   LDY   #$3E
   STA   (FP),Y
   LDY   #$17
   LDA   (FP),Y
   ADC   #$9A
   LDY   #$3F
   STA   (FP),Y
   LDY   #$18
   LDA   (FP),Y
   ADC   #$3B
   LDY   #$40
   STA   (FP),Y
   LDY   #$19
   LDA   (FP),Y
   ADC   #$00
   LDY   #$41
   STA   (FP),Y
   LDY   #$1A
   LDA   (FP),Y
   ADC   #$00
   LDY   #$42
   STA   (FP),Y
   LDY   #$1B
   LDA   (FP),Y
   ADC   #$00
   LDY   #$43
   STA   (FP),Y
   LDY   #$1C
   LDA   (FP),Y
   ADC   #$00
   LDY   #$44
   STA   (FP),Y
   LDY   #$3D
   LDA   (FP),Y
   LDY   #$15
   STA   (FP),Y
   LDY   #$3E
   LDA   (FP),Y
   LDY   #$16
   STA   (FP),Y
   LDY   #$3F
   LDA   (FP),Y
   LDY   #$17
   STA   (FP),Y
   LDY   #$40
   LDA   (FP),Y
   LDY   #$18
   STA   (FP),Y
   LDY   #$41
   LDA   (FP),Y
   LDY   #$19
   STA   (FP),Y
   LDY   #$42
   LDA   (FP),Y
   LDY   #$1A
   STA   (FP),Y
   LDY   #$43
   LDA   (FP),Y
   LDY   #$1B
   STA   (FP),Y
   LDY   #$44
   LDA   (FP),Y
   LDY   #$1C
   STA   (FP),Y
   INY
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
   LDA   #$66
   STA   HARGS+4
   STA   HARGS+5
   LDA   #$00
   STA   HARGS+6
   STA   HARGS+7
   JSR   mul32
   LDA   HARGS+8
   LDY   #$45
   STA   (FP),Y
   LDA   HARGS+9
   INY
   STA   (FP),Y
   LDA   HARGS+10
   INY
   STA   (FP),Y
   LDA   HARGS+11
   INY
   STA   (FP),Y
   LDY   #$45
   LDA   (FP),Y
   LDY   #$1D
   STA   (FP),Y
   LDY   #$46
   LDA   (FP),Y
   LDY   #$1E
   STA   (FP),Y
   LDY   #$47
   LDA   (FP),Y
   LDY   #$1F
   STA   (FP),Y
   LDY   #$48
   LDA   (FP),Y
   LDY   #$20
   STA   (FP),Y
   LDY   #$08
   LDA   (FP),Y
   BNE   .cmp_true@12
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@12
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@12
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@12
   DEY
   LDA   (FP),Y
   CMP   #$C4
   BNE   .cmp_true@12
   DEY
   LDA   (FP),Y
   CMP   #$65
   BNE   .cmp_true@12
   DEY
   LDA   (FP),Y
   CMP   #$36
   BNE   .cmp_true@12
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@12
   LDA   #$00
   JMP   .cmp_end@13
.cmp_true@12:
   LDA   #$01
.cmp_end@13:
   LDY   #$49
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
   ADC   #$5C
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$5B
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
   SEC
   LDA   #$00
   SBC   #$02
   LDY   #$4B
   STA   (FP),Y
   LDA   #$00
   SBC   #$00
   INY
   STA   (FP),Y
   LDY   #$0C
   LDA   (FP),Y
   LDY   #$4C
   CMP   (FP),Y
   BNE   .cmp_true@15
   LDY   #$0B
   LDA   (FP),Y
   LDY   #$4B
   CMP   (FP),Y
   BNE   .cmp_true@15
   LDA   #$00
   JMP   .cmp_end@16
.cmp_true@15:
   LDA   #$01
.cmp_end@16:
   LDY   #$4D
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
   ADC   #$5C
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$5B
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
   LDA   #$02
   LDY   #$4F
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   ORA   #$00
   BPL   .sx_done@19
   LDA   #$FF
.sx_done@19:
   LDY   #$51
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
   LDY   #$1C
   LDA   (FP),Y
   LDY   #$56
   CMP   (FP),Y
   BNE   .cmp_true@20
   LDY   #$1B
   LDA   (FP),Y
   LDY   #$55
   CMP   (FP),Y
   BNE   .cmp_true@20
   LDY   #$1A
   LDA   (FP),Y
   LDY   #$54
   CMP   (FP),Y
   BNE   .cmp_true@20
   LDY   #$19
   LDA   (FP),Y
   LDY   #$53
   CMP   (FP),Y
   BNE   .cmp_true@20
   LDY   #$18
   LDA   (FP),Y
   LDY   #$52
   CMP   (FP),Y
   BNE   .cmp_true@20
   LDY   #$17
   LDA   (FP),Y
   LDY   #$51
   CMP   (FP),Y
   BNE   .cmp_true@20
   LDY   #$16
   LDA   (FP),Y
   LDY   #$50
   CMP   (FP),Y
   BNE   .cmp_true@20
   LDY   #$15
   LDA   (FP),Y
   LDY   #$4F
   CMP   (FP),Y
   BNE   .cmp_true@20
   LDA   #$00
   JMP   .cmp_end@21
.cmp_true@20:
   LDA   #$01
.cmp_end@21:
   LDY   #$57
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@6
   LDX   #$00
   LDA   #$07

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$5C
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$5B
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
.if_end@6:
   LDY   #$20
   LDA   (FP),Y
   BNE   .cmp_true@23
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@23
   DEY
   LDA   (FP),Y
   CMP   #$FF
   BNE   .cmp_true@23
   DEY
   LDA   (FP),Y
   CMP   #$FC
   BNE   .cmp_true@23
   LDA   #$00
   JMP   .cmp_end@24
.cmp_true@23:
   LDA   #$01
.cmp_end@24:
   LDY   #$59
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@7
   LDX   #$00
   LDA   #$08

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$5C
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
.if_end@7:
   LDX   #$00
   TXA

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$5C
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$5B
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
