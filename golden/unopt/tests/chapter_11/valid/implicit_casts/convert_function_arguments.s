SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24

foo:
   SUBROUTINE

   ; prologue: 34 arg bytes, 46 local bytes
   SEC
   LDA   SSP
   SBC   #$30
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$2F
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
   SBC   #$01
   LDY   #$01
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
   LDY   #$38
   LDA   (FP),Y
   LDY   #$08
   CMP   (FP),Y
   BNE   .cmp_true@0
   LDY   #$37
   LDA   (FP),Y
   LDY   #$07
   CMP   (FP),Y
   BNE   .cmp_true@0
   LDY   #$36
   LDA   (FP),Y
   LDY   #$06
   CMP   (FP),Y
   BNE   .cmp_true@0
   LDY   #$35
   LDA   (FP),Y
   LDY   #$05
   CMP   (FP),Y
   BNE   .cmp_true@0
   LDY   #$34
   LDA   (FP),Y
   LDY   #$04
   CMP   (FP),Y
   BNE   .cmp_true@0
   LDY   #$33
   LDA   (FP),Y
   LDY   #$03
   CMP   (FP),Y
   BNE   .cmp_true@0
   LDY   #$32
   LDA   (FP),Y
   LDY   #$02
   CMP   (FP),Y
   BNE   .cmp_true@0
   LDY   #$31
   LDA   (FP),Y
   LDY   #$01
   CMP   (FP),Y
   BNE   .cmp_true@0
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
   ADC   #$52
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$2F
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
   LDY   #$3A
   LDA   (FP),Y
   BNE   .cmp_true@3
   DEY
   LDA   (FP),Y
   CMP   #$02
   BNE   .cmp_true@3
   LDA   #$00
   JMP   .cmp_end@4
.cmp_true@3:
   LDA   #$01
.cmp_end@4:
   LDY   #$0B
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
   ADC   #$52
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$2F
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
   LDY   #$3C
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
   LDY   #$0D
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
   ADC   #$52
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$2F
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
   SEC
   LDA   #$00
   SBC   #$05
   LDY   #$0F
   STA   (FP),Y
   LDA   #$00
   SBC   #$00
   INY
   STA   (FP),Y
   LDY   #$3E
   LDA   (FP),Y
   LDY   #$10
   CMP   (FP),Y
   BNE   .cmp_true@9
   LDY   #$3D
   LDA   (FP),Y
   LDY   #$0F
   CMP   (FP),Y
   BNE   .cmp_true@9
   LDA   #$00
   JMP   .cmp_end@10
.cmp_true@9:
   LDA   #$01
.cmp_end@10:
   LDY   #$11
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
   ADC   #$52
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$2F
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
   SBC   #$65
   LDY   #$13
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
   LDY   #$46
   LDA   (FP),Y
   LDY   #$1A
   CMP   (FP),Y
   BNE   .cmp_true@12
   LDY   #$45
   LDA   (FP),Y
   LDY   #$19
   CMP   (FP),Y
   BNE   .cmp_true@12
   LDY   #$44
   LDA   (FP),Y
   LDY   #$18
   CMP   (FP),Y
   BNE   .cmp_true@12
   LDY   #$43
   LDA   (FP),Y
   LDY   #$17
   CMP   (FP),Y
   BNE   .cmp_true@12
   LDY   #$42
   LDA   (FP),Y
   LDY   #$16
   CMP   (FP),Y
   BNE   .cmp_true@12
   LDY   #$41
   LDA   (FP),Y
   LDY   #$15
   CMP   (FP),Y
   BNE   .cmp_true@12
   LDY   #$40
   LDA   (FP),Y
   LDY   #$14
   CMP   (FP),Y
   BNE   .cmp_true@12
   LDY   #$3F
   LDA   (FP),Y
   LDY   #$13
   CMP   (FP),Y
   BNE   .cmp_true@12
   LDA   #$00
   JMP   .cmp_end@13
.cmp_true@12:
   LDA   #$01
.cmp_end@13:
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
   ADC   #$52
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$2F
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
   SBC   #$7B
   LDY   #$1D
   STA   (FP),Y
   LDA   #$00
   SBC   #$00
   INY
   STA   (FP),Y
   LDY   #$48
   LDA   (FP),Y
   LDY   #$1E
   CMP   (FP),Y
   BNE   .cmp_true@15
   LDY   #$47
   LDA   (FP),Y
   LDY   #$1D
   CMP   (FP),Y
   BNE   .cmp_true@15
   LDA   #$00
   JMP   .cmp_end@16
.cmp_true@15:
   LDA   #$01
.cmp_end@16:
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
   ADC   #$52
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$2F
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
   SEC
   LDA   #$00
   SBC   #$0A
   LDY   #$21
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
   LDY   #$50
   LDA   (FP),Y
   LDY   #$28
   CMP   (FP),Y
   BNE   .cmp_true@18
   LDY   #$4F
   LDA   (FP),Y
   LDY   #$27
   CMP   (FP),Y
   BNE   .cmp_true@18
   LDY   #$4E
   LDA   (FP),Y
   LDY   #$26
   CMP   (FP),Y
   BNE   .cmp_true@18
   LDY   #$4D
   LDA   (FP),Y
   LDY   #$25
   CMP   (FP),Y
   BNE   .cmp_true@18
   LDY   #$4C
   LDA   (FP),Y
   LDY   #$24
   CMP   (FP),Y
   BNE   .cmp_true@18
   LDY   #$4B
   LDA   (FP),Y
   LDY   #$23
   CMP   (FP),Y
   BNE   .cmp_true@18
   LDY   #$4A
   LDA   (FP),Y
   LDY   #$22
   CMP   (FP),Y
   BNE   .cmp_true@18
   LDY   #$49
   LDA   (FP),Y
   LDY   #$21
   CMP   (FP),Y
   BNE   .cmp_true@18
   LDA   #$00
   JMP   .cmp_end@19
.cmp_true@18:
   LDA   #$01
.cmp_end@19:
   LDY   #$29
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
   ADC   #$52
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$2F
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
   SEC
   LDA   #$00
   SBC   #$2E
   LDY   #$2B
   STA   (FP),Y
   LDA   #$00
   SBC   #$00
   INY
   STA   (FP),Y
   LDY   #$52
   LDA   (FP),Y
   LDY   #$2C
   CMP   (FP),Y
   BNE   .cmp_true@21
   LDY   #$51
   LDA   (FP),Y
   LDY   #$2B
   CMP   (FP),Y
   BNE   .cmp_true@21
   LDA   #$00
   JMP   .cmp_end@22
.cmp_true@21:
   LDA   #$01
.cmp_end@22:
   LDY   #$2D
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
   ADC   #$52
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
   ADC   #$52
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$2F
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

   ; prologue: 0 arg bytes, 132 local bytes
   SEC
   LDA   SSP
   SBC   #$86
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$85
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
   SBC   #$01
   LDY   #$01
   STA   (FP),Y
   LDA   #$00
   SBC   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$03
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$04
   STA   (FP),Y
   LDA   #$02
   INY
   STA   (FP),Y
   LDA   #$01
   INY
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@24
   LDA   #$00
   JMP   .sx_done@25
.sx_neg@24:
   LDA   #$FF
.sx_done@25:
   LDY   #$07
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
   LDY   #$05
   LDA   (FP),Y
   LDY   #$0D
   STA   (FP),Y
   LDY   #$06
   LDA   (FP),Y
   LDY   #$0E
   STA   (FP),Y
   LDY   #$07
   LDA   (FP),Y
   LDY   #$0F
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
   SEC
   LDA   #$00
   SBC   #$00
   INY
   STA   (FP),Y
   LDA   #$00
   SBC   #$01
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$17
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$18
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@26
   LDA   #$00
   JMP   .sx_done@27
.sx_neg@26:
   LDA   #$FF
.sx_done@27:
   LDY   #$19
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
   LDY   #$17
   LDA   (FP),Y
   LDY   #$1F
   STA   (FP),Y
   LDY   #$18
   LDA   (FP),Y
   LDY   #$20
   STA   (FP),Y
   LDY   #$19
   LDA   (FP),Y
   LDY   #$21
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
   LDA   #$FB
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDA   #$01
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   ORA   #$00
   BPL   .sx_done@29
   LDA   #$FF
.sx_done@29:
   LDY   #$2B
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$27
   LDA   (FP),Y
   LDY   #$2F
   STA   (FP),Y
   LDY   #$28
   LDA   (FP),Y
   LDY   #$30
   STA   (FP),Y
   LDY   #$29
   LDA   (FP),Y
   LDY   #$31
   STA   (FP),Y
   LDY   #$2A
   LDA   (FP),Y
   LDY   #$32
   STA   (FP),Y
   LDY   #$2B
   LDA   (FP),Y
   LDY   #$33
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   SEC
   LDA   #$00
   SBC   #$65
   INY
   STA   (FP),Y
   LDA   #$00
   SBC   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$39
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$3A
   STA   (FP),Y
   SEC
   LDA   #$00
   SBC   #$7B
   INY
   STA   (FP),Y
   LDA   #$00
   SBC   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$3D
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$3E
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@30
   LDA   #$00
   JMP   .sx_done@31
.sx_neg@30:
   LDA   #$FF
.sx_done@31:
   LDY   #$3F
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
   LDY   #$3D
   LDA   (FP),Y
   LDY   #$45
   STA   (FP),Y
   LDY   #$3E
   LDA   (FP),Y
   LDY   #$46
   STA   (FP),Y
   LDY   #$3F
   LDA   (FP),Y
   LDY   #$47
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
   SEC
   LDA   #$00
   SBC   #$0A
   INY
   STA   (FP),Y
   LDA   #$00
   SBC   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$4F
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$50
   STA   (FP),Y
   LDA   #$D2
   INY
   STA   (FP),Y
   LDA   #$04
   INY
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@32
   LDA   #$00
   JMP   .sx_done@33
.sx_neg@32:
   LDA   #$FF
.sx_done@33:
   LDY   #$53
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
   LDY   #$51
   LDA   (FP),Y
   LDY   #$59
   STA   (FP),Y
   LDY   #$52
   LDA   (FP),Y
   LDY   #$5A
   STA   (FP),Y
   LDY   #$53
   LDA   (FP),Y
   LDY   #$5B
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
   LDY   #$03
   LDA   (FP),Y
   LDY   #$61
   STA   (FP),Y
   LDY   #$04
   LDA   (FP),Y
   LDY   #$62
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@34
   LDA   #$00
   JMP   .sx_done@35
.sx_neg@34:
   LDA   #$FF
.sx_done@35:
   LDY   #$63
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
   LDY   #$0D
   LDA   (FP),Y
   LDY   #$69
   STA   (FP),Y
   LDY   #$0E
   LDA   (FP),Y
   LDY   #$6A
   STA   (FP),Y
   LDY   #$1F
   LDA   (FP),Y
   LDY   #$6B
   STA   (FP),Y
   LDY   #$20
   LDA   (FP),Y
   LDY   #$6C
   STA   (FP),Y
   LDY   #$2F
   LDA   (FP),Y
   LDY   #$6D
   STA   (FP),Y
   LDY   #$30
   LDA   (FP),Y
   LDY   #$6E
   STA   (FP),Y
   LDY   #$39
   LDA   (FP),Y
   LDY   #$6F
   STA   (FP),Y
   LDY   #$3A
   LDA   (FP),Y
   LDY   #$70
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@36
   LDA   #$00
   JMP   .sx_done@37
.sx_neg@36:
   LDA   #$FF
.sx_done@37:
   LDY   #$71
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
   LDY   #$45
   LDA   (FP),Y
   LDY   #$77
   STA   (FP),Y
   LDY   #$46
   LDA   (FP),Y
   LDY   #$78
   STA   (FP),Y
   LDY   #$4F
   LDA   (FP),Y
   LDY   #$79
   STA   (FP),Y
   LDY   #$50
   LDA   (FP),Y
   LDY   #$7A
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@38
   LDA   #$00
   JMP   .sx_done@39
.sx_neg@38:
   LDA   #$FF
.sx_done@39:
   LDY   #$7B
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
   LDY   #$59
   LDA   (FP),Y
   LDY   #$81
   STA   (FP),Y
   LDY   #$5A
   LDA   (FP),Y
   LDY   #$82
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$22
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDY   #$61
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$62
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$63
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$64
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   LDY   #$65
   LDA   (FP),Y
   LDY   #$05
   STA   (SSP),Y
   LDY   #$66
   LDA   (FP),Y
   LDY   #$06
   STA   (SSP),Y
   LDY   #$67
   LDA   (FP),Y
   LDY   #$07
   STA   (SSP),Y
   LDY   #$68
   LDA   (FP),Y
   LDY   #$08
   STA   (SSP),Y
   LDY   #$69
   LDA   (FP),Y
   LDY   #$09
   STA   (SSP),Y
   LDY   #$6A
   LDA   (FP),Y
   LDY   #$0A
   STA   (SSP),Y
   LDY   #$6B
   LDA   (FP),Y
   LDY   #$0B
   STA   (SSP),Y
   LDY   #$6C
   LDA   (FP),Y
   LDY   #$0C
   STA   (SSP),Y
   LDY   #$6D
   LDA   (FP),Y
   LDY   #$0D
   STA   (SSP),Y
   LDY   #$6E
   LDA   (FP),Y
   LDY   #$0E
   STA   (SSP),Y
   LDY   #$6F
   LDA   (FP),Y
   LDY   #$0F
   STA   (SSP),Y
   LDY   #$70
   LDA   (FP),Y
   LDY   #$10
   STA   (SSP),Y
   LDY   #$71
   LDA   (FP),Y
   LDY   #$11
   STA   (SSP),Y
   LDY   #$72
   LDA   (FP),Y
   LDY   #$12
   STA   (SSP),Y
   LDY   #$73
   LDA   (FP),Y
   LDY   #$13
   STA   (SSP),Y
   LDY   #$74
   LDA   (FP),Y
   LDY   #$14
   STA   (SSP),Y
   LDY   #$75
   LDA   (FP),Y
   LDY   #$15
   STA   (SSP),Y
   LDY   #$76
   LDA   (FP),Y
   LDY   #$16
   STA   (SSP),Y
   LDY   #$77
   LDA   (FP),Y
   LDY   #$17
   STA   (SSP),Y
   LDY   #$78
   LDA   (FP),Y
   LDY   #$18
   STA   (SSP),Y
   LDY   #$79
   LDA   (FP),Y
   LDY   #$19
   STA   (SSP),Y
   LDY   #$7A
   LDA   (FP),Y
   LDY   #$1A
   STA   (SSP),Y
   LDY   #$7B
   LDA   (FP),Y
   LDY   #$1B
   STA   (SSP),Y
   LDY   #$7C
   LDA   (FP),Y
   LDY   #$1C
   STA   (SSP),Y
   LDY   #$7D
   LDA   (FP),Y
   LDY   #$1D
   STA   (SSP),Y
   LDY   #$7E
   LDA   (FP),Y
   LDY   #$1E
   STA   (SSP),Y
   LDY   #$7F
   LDA   (FP),Y
   LDY   #$1F
   STA   (SSP),Y
   LDY   #$80
   LDA   (FP),Y
   LDY   #$20
   STA   (SSP),Y
   LDY   #$81
   LDA   (FP),Y
   LDY   #$21
   STA   (SSP),Y
   LDY   #$82
   LDA   (FP),Y
   LDY   #$22
   STA   (SSP),Y
   JSR   foo
   LDY   #$83
   STA   (FP),Y
   TXA
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
   ADC   #$86
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$85
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
