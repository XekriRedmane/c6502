SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24

main:
   SUBROUTINE

   ; prologue: 0 arg bytes, 74 local bytes
   SEC
   LDA   SSP
   SBC   #$4C
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$4B
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   LDA   #$64
   LDY   #$01
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
   INY
   STA   (FP),Y
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
   LDA   #$16
   STA   HARGS+8
   JSR   asl64
   LDA   HARGS+9
   LDY   #$09
   STA   (FP),Y
   LDA   HARGS+10
   INY
   STA   (FP),Y
   LDA   HARGS+11
   INY
   STA   (FP),Y
   LDA   HARGS+12
   INY
   STA   (FP),Y
   LDA   HARGS+13
   INY
   STA   (FP),Y
   LDA   HARGS+14
   INY
   STA   (FP),Y
   LDA   HARGS+15
   INY
   STA   (FP),Y
   LDA   HARGS+16
   INY
   STA   (FP),Y
   LDY   #$09
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   LDY   #$0A
   LDA   (FP),Y
   LDY   #$02
   STA   (FP),Y
   LDY   #$0B
   LDA   (FP),Y
   LDY   #$03
   STA   (FP),Y
   LDY   #$0C
   LDA   (FP),Y
   LDY   #$04
   STA   (FP),Y
   LDY   #$0D
   LDA   (FP),Y
   LDY   #$05
   STA   (FP),Y
   LDY   #$0E
   LDA   (FP),Y
   LDY   #$06
   STA   (FP),Y
   LDY   #$0F
   LDA   (FP),Y
   LDY   #$07
   STA   (FP),Y
   LDY   #$10
   LDA   (FP),Y
   LDY   #$08
   STA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@0
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@0
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@0
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@0
   DEY
   LDA   (FP),Y
   CMP   #$19
   BNE   .cmp_true@0
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@0
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@0
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@0
   LDA   #$00
   JMP   .cmp_end@1
.cmp_true@0:
   LDA   #$01
.cmp_end@1:
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
   ADC   #$4C
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$4B
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
   LDA   #$04
   STA   HARGS+8
   JSR   asr64
   LDA   HARGS+9
   LDY   #$13
   STA   (FP),Y
   LDA   HARGS+10
   INY
   STA   (FP),Y
   LDA   HARGS+11
   INY
   STA   (FP),Y
   LDA   HARGS+12
   INY
   STA   (FP),Y
   LDA   HARGS+13
   INY
   STA   (FP),Y
   LDA   HARGS+14
   INY
   STA   (FP),Y
   LDA   HARGS+15
   INY
   STA   (FP),Y
   LDA   HARGS+16
   INY
   STA   (FP),Y
   LDY   #$13
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   LDY   #$14
   LDA   (FP),Y
   LDY   #$02
   STA   (FP),Y
   LDY   #$15
   LDA   (FP),Y
   LDY   #$03
   STA   (FP),Y
   LDY   #$16
   LDA   (FP),Y
   LDY   #$04
   STA   (FP),Y
   LDY   #$17
   LDA   (FP),Y
   LDY   #$05
   STA   (FP),Y
   LDY   #$18
   LDA   (FP),Y
   LDY   #$06
   STA   (FP),Y
   LDY   #$19
   LDA   (FP),Y
   LDY   #$07
   STA   (FP),Y
   LDY   #$1A
   LDA   (FP),Y
   LDY   #$08
   STA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@3
   LDY   #$19
   LDA   (FP),Y
   BNE   .cmp_true@3
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@3
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@3
   DEY
   LDA   (FP),Y
   CMP   #$01
   BNE   .cmp_true@3
   DEY
   LDA   (FP),Y
   CMP   #$90
   BNE   .cmp_true@3
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@3
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@3
   LDA   #$00
   JMP   .cmp_end@4
.cmp_true@3:
   LDA   #$01
.cmp_end@4:
   LDY   #$1B
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
   ADC   #$4C
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$4B
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
   LDY   #$08
   LDA   (FP),Y
   BNE   .cmp_true@6
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@6
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@6
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@6
   DEY
   LDA   (FP),Y
   CMP   #$01
   BNE   .cmp_true@6
   DEY
   LDA   (FP),Y
   CMP   #$90
   BNE   .cmp_true@6
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@6
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@6
   LDA   #$00
   JMP   .cmp_end@7
.cmp_true@6:
   LDA   #$01
.cmp_end@7:
   LDY   #$1D
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
   ADC   #$4C
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$4B
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
   LDA   #$39
   LDY   #$1F
   STA   (FP),Y
   LDA   #$30
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
   INY
   STA   (FP),Y
   LDY   #$2E
   STA   (FP),Y
   DEY
   STA   (FP),Y
   DEY
   STA   (FP),Y
   DEY
   STA   (FP),Y
   LDY   #$20
   LDA   (FP),Y
   LDY   #$2A
   STA   (FP),Y
   LDY   #$1F
   LDA   (FP),Y
   LDY   #$29
   STA   (FP),Y
   LDA   #$00
   DEY
   STA   (FP),Y
   DEY
   STA   (FP),Y
   LDY   #$29
   LDA   (FP),Y
   ASL
   STA   (FP),Y
   INY
   LDA   (FP),Y
   ROL
   STA   (FP),Y
   INY
   LDA   (FP),Y
   ROL
   STA   (FP),Y
   INY
   LDA   (FP),Y
   ROL
   STA   (FP),Y
   INY
   LDA   (FP),Y
   ROL
   STA   (FP),Y
   INY
   LDA   (FP),Y
   ROL
   STA   (FP),Y
   LDY   #$27
   LDA   (FP),Y
   LDY   #$1F
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$29
   LDA   (FP),Y
   LDY   #$21
   STA   (FP),Y
   LDY   #$2A
   LDA   (FP),Y
   LDY   #$22
   STA   (FP),Y
   LDY   #$2B
   LDA   (FP),Y
   LDY   #$23
   STA   (FP),Y
   LDY   #$2C
   LDA   (FP),Y
   LDY   #$24
   STA   (FP),Y
   LDY   #$2D
   LDA   (FP),Y
   LDY   #$25
   STA   (FP),Y
   LDY   #$2E
   LDA   (FP),Y
   LDY   #$26
   STA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@9
   LDY   #$2D
   LDA   (FP),Y
   BNE   .cmp_true@9
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@9
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@9
   DEY
   LDA   (FP),Y
   CMP   #$60
   BNE   .cmp_true@9
   DEY
   LDA   (FP),Y
   CMP   #$72
   BNE   .cmp_true@9
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@9
   LDA   #$00
   JMP   .cmp_end@10
.cmp_true@9:
   LDA   #$01
.cmp_end@10:
   LDY   #$2F
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
   ADC   #$4C
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$4B
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
   LDY   #$1F
   SBC   (FP),Y
   LDY   #$31
   STA   (FP),Y
   LDA   #$00
   LDY   #$20
   SBC   (FP),Y
   LDY   #$32
   STA   (FP),Y
   LDA   #$00
   LDY   #$21
   SBC   (FP),Y
   LDY   #$33
   STA   (FP),Y
   LDA   #$00
   LDY   #$22
   SBC   (FP),Y
   LDY   #$34
   STA   (FP),Y
   LDA   #$00
   LDY   #$23
   SBC   (FP),Y
   LDY   #$35
   STA   (FP),Y
   LDA   #$00
   LDY   #$24
   SBC   (FP),Y
   LDY   #$36
   STA   (FP),Y
   LDA   #$00
   LDY   #$25
   SBC   (FP),Y
   LDY   #$37
   STA   (FP),Y
   LDA   #$00
   LDY   #$26
   SBC   (FP),Y
   LDY   #$38
   STA   (FP),Y
   LDY   #$31
   LDA   (FP),Y
   LDY   #$1F
   STA   (FP),Y
   LDY   #$32
   LDA   (FP),Y
   LDY   #$20
   STA   (FP),Y
   LDY   #$33
   LDA   (FP),Y
   LDY   #$21
   STA   (FP),Y
   LDY   #$34
   LDA   (FP),Y
   LDY   #$22
   STA   (FP),Y
   LDY   #$35
   LDA   (FP),Y
   LDY   #$23
   STA   (FP),Y
   LDY   #$36
   LDA   (FP),Y
   LDY   #$24
   STA   (FP),Y
   LDY   #$37
   LDA   (FP),Y
   LDY   #$25
   STA   (FP),Y
   LDY   #$38
   LDA   (FP),Y
   LDY   #$26
   STA   (FP),Y
   LDY   #$1F
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
   LDA   #$0A
   STA   HARGS+8
   JSR   asr64
   LDA   HARGS+9
   LDY   #$39
   STA   (FP),Y
   LDA   HARGS+10
   INY
   STA   (FP),Y
   LDA   HARGS+11
   INY
   STA   (FP),Y
   LDA   HARGS+12
   INY
   STA   (FP),Y
   LDA   HARGS+13
   INY
   STA   (FP),Y
   LDA   HARGS+14
   INY
   STA   (FP),Y
   LDA   HARGS+15
   INY
   STA   (FP),Y
   LDA   HARGS+16
   INY
   STA   (FP),Y
   LDY   #$39
   LDA   (FP),Y
   LDY   #$1F
   STA   (FP),Y
   LDY   #$3A
   LDA   (FP),Y
   LDY   #$20
   STA   (FP),Y
   LDY   #$3B
   LDA   (FP),Y
   LDY   #$21
   STA   (FP),Y
   LDY   #$3C
   LDA   (FP),Y
   LDY   #$22
   STA   (FP),Y
   LDY   #$3D
   LDA   (FP),Y
   LDY   #$23
   STA   (FP),Y
   LDY   #$3E
   LDA   (FP),Y
   LDY   #$24
   STA   (FP),Y
   LDY   #$3F
   LDA   (FP),Y
   LDY   #$25
   STA   (FP),Y
   LDY   #$40
   LDA   (FP),Y
   LDY   #$26
   STA   (FP),Y
   SEC
   LDA   #$00
   SBC   #$80
   LDY   #$41
   STA   (FP),Y
   LDA   #$00
   SBC   #$1C
   INY
   STA   (FP),Y
   LDA   #$00
   SBC   #$18
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
   LDY   #$40
   LDA   (FP),Y
   LDY   #$48
   CMP   (FP),Y
   BNE   .cmp_true@12
   LDY   #$3F
   LDA   (FP),Y
   LDY   #$47
   CMP   (FP),Y
   BNE   .cmp_true@12
   LDY   #$3E
   LDA   (FP),Y
   LDY   #$46
   CMP   (FP),Y
   BNE   .cmp_true@12
   LDY   #$3D
   LDA   (FP),Y
   LDY   #$45
   CMP   (FP),Y
   BNE   .cmp_true@12
   LDY   #$3C
   LDA   (FP),Y
   LDY   #$44
   CMP   (FP),Y
   BNE   .cmp_true@12
   LDY   #$3B
   LDA   (FP),Y
   LDY   #$43
   CMP   (FP),Y
   BNE   .cmp_true@12
   LDY   #$3A
   LDA   (FP),Y
   LDY   #$42
   CMP   (FP),Y
   BNE   .cmp_true@12
   LDY   #$39
   LDA   (FP),Y
   LDY   #$41
   CMP   (FP),Y
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
   ADC   #$4C
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
.if_end@4:
   LDX   #$00
   TXA

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$4C
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$4B
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
