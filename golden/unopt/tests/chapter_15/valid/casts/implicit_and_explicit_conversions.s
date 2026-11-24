SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24

main:
   SUBROUTINE

   ; prologue: 0 arg bytes, 140 local bytes
   SEC
   LDA   SSP
   SBC   #$8E
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$8D
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
   ORA   #$00
   BMI   .sx_neg@0
   LDA   #$00
   JMP   .sx_done@1
.sx_neg@0:
   LDA   #$FF
.sx_done@1:
   LDY   #$05
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
   CLC
   LDA   FP
   ADC   #$0B
   LDY   #$2B
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
   LDY   #$03
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$04
   LDA   (FP),Y
   LDY   #$01
   STA   (DPTR),Y
   LDY   #$05
   LDA   (FP),Y
   LDY   #$02
   STA   (DPTR),Y
   LDY   #$06
   LDA   (FP),Y
   LDY   #$03
   STA   (DPTR),Y
   LDY   #$07
   LDA   (FP),Y
   LDY   #$04
   STA   (DPTR),Y
   LDY   #$08
   LDA   (FP),Y
   LDY   #$05
   STA   (DPTR),Y
   LDY   #$09
   LDA   (FP),Y
   LDY   #$06
   STA   (DPTR),Y
   LDY   #$0A
   LDA   (FP),Y
   LDY   #$07
   STA   (DPTR),Y
   SEC
   LDA   #$00
   SBC   #$02
   LDY   #$2D
   STA   (FP),Y
   LDA   #$00
   SBC   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$2F
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$30
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@2
   LDA   #$00
   JMP   .sx_done@3
.sx_neg@2:
   LDA   #$FF
.sx_done@3:
   LDY   #$31
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
   LDY   #$2B
   LDA   (FP),Y
   CLC
   ADC   #$08
   LDY   #$37
   STA   (FP),Y
   LDY   #$2C
   LDA   (FP),Y
   ADC   #$00
   LDY   #$38
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$2F
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$30
   LDA   (FP),Y
   LDY   #$01
   STA   (DPTR),Y
   LDY   #$31
   LDA   (FP),Y
   LDY   #$02
   STA   (DPTR),Y
   LDY   #$32
   LDA   (FP),Y
   LDY   #$03
   STA   (DPTR),Y
   LDY   #$33
   LDA   (FP),Y
   LDY   #$04
   STA   (DPTR),Y
   LDY   #$34
   LDA   (FP),Y
   LDY   #$05
   STA   (DPTR),Y
   LDY   #$35
   LDA   (FP),Y
   LDY   #$06
   STA   (DPTR),Y
   LDY   #$36
   LDA   (FP),Y
   LDY   #$07
   STA   (DPTR),Y
   SEC
   LDA   #$00
   SBC   #$03
   LDY   #$39
   STA   (FP),Y
   LDA   #$00
   SBC   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$3B
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$3C
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@4
   LDA   #$00
   JMP   .sx_done@5
.sx_neg@4:
   LDA   #$FF
.sx_done@5:
   LDY   #$3D
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
   LDY   #$2B
   LDA   (FP),Y
   CLC
   ADC   #$10
   LDY   #$43
   STA   (FP),Y
   LDY   #$2C
   LDA   (FP),Y
   ADC   #$00
   LDY   #$44
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$3B
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$3C
   LDA   (FP),Y
   LDY   #$01
   STA   (DPTR),Y
   LDY   #$3D
   LDA   (FP),Y
   LDY   #$02
   STA   (DPTR),Y
   LDY   #$3E
   LDA   (FP),Y
   LDY   #$03
   STA   (DPTR),Y
   LDY   #$3F
   LDA   (FP),Y
   LDY   #$04
   STA   (DPTR),Y
   LDY   #$40
   LDA   (FP),Y
   LDY   #$05
   STA   (DPTR),Y
   LDY   #$41
   LDA   (FP),Y
   LDY   #$06
   STA   (DPTR),Y
   LDY   #$42
   LDA   (FP),Y
   LDY   #$07
   STA   (DPTR),Y
   SEC
   LDA   #$00
   SBC   #$04
   LDY   #$45
   STA   (FP),Y
   LDA   #$00
   SBC   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$47
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$48
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@6
   LDA   #$00
   JMP   .sx_done@7
.sx_neg@6:
   LDA   #$FF
.sx_done@7:
   LDY   #$49
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
   LDY   #$2B
   LDA   (FP),Y
   CLC
   ADC   #$18
   LDY   #$4F
   STA   (FP),Y
   LDY   #$2C
   LDA   (FP),Y
   ADC   #$00
   LDY   #$50
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$47
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$48
   LDA   (FP),Y
   LDY   #$01
   STA   (DPTR),Y
   LDY   #$49
   LDA   (FP),Y
   LDY   #$02
   STA   (DPTR),Y
   LDY   #$4A
   LDA   (FP),Y
   LDY   #$03
   STA   (DPTR),Y
   LDY   #$4B
   LDA   (FP),Y
   LDY   #$04
   STA   (DPTR),Y
   LDY   #$4C
   LDA   (FP),Y
   LDY   #$05
   STA   (DPTR),Y
   LDY   #$4D
   LDA   (FP),Y
   LDY   #$06
   STA   (DPTR),Y
   LDY   #$4E
   LDA   (FP),Y
   LDY   #$07
   STA   (DPTR),Y
   CLC
   LDA   FP
   ADC   #$0B
   LDY   #$51
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   CLC
   LDA   FP
   ADC   #$0B
   INY
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   LDY   #$52
   LDA   (FP),Y
   LDY   #$54
   CMP   (FP),Y
   BNE   .cmp_true@8
   LDY   #$51
   LDA   (FP),Y
   LDY   #$53
   CMP   (FP),Y
   BNE   .cmp_true@8
   LDA   #$00
   JMP   .cmp_end@9
.cmp_true@8:
   LDA   #$01
.cmp_end@9:
   LDY   #$55
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
   ADC   #$8E
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$8D
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
   CLC
   LDA   FP
   ADC   #$0B
   LDY   #$57
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   CLC
   LDA   FP
   ADC   #$0B
   INY
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   LDY   #$58
   LDA   (FP),Y
   LDY   #$5A
   CMP   (FP),Y
   BNE   .cmp_true@11
   LDY   #$57
   LDA   (FP),Y
   LDY   #$59
   CMP   (FP),Y
   BNE   .cmp_true@11
   LDA   #$00
   JMP   .cmp_end@12
.cmp_true@11:
   LDA   #$01
.cmp_end@12:
   LDY   #$5B
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
   ADC   #$8E
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$8D
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
   CLC
   LDA   FP
   ADC   #$0B
   LDY   #$5D
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$5F
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$60
   STA   (FP),Y
   LDA   #$00
   STA   HARGS
   LDX   #$08
   LSR   HARGS
.mul_loop@14:
   BCC   .mul_skip@15
   CLC
   ADC   #$08
.mul_skip@15:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@14
   LDY   #$62
   STA   (FP),Y
   LDA   HARGS
   DEY
   STA   (FP),Y
   LDY   #$5F
   LDA   (FP),Y
   CLC
   LDY   #$61
   ADC   (FP),Y
   LDY   #$63
   STA   (FP),Y
   LDY   #$60
   LDA   (FP),Y
   LDY   #$62
   ADC   (FP),Y
   LDY   #$64
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$65
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$66
   STA   (FP),Y
   LDY   #$02
   LDA   (DPTR),Y
   LDY   #$67
   STA   (FP),Y
   LDY   #$03
   LDA   (DPTR),Y
   LDY   #$68
   STA   (FP),Y
   LDY   #$04
   LDA   (DPTR),Y
   LDY   #$69
   STA   (FP),Y
   LDY   #$05
   LDA   (DPTR),Y
   LDY   #$6A
   STA   (FP),Y
   LDY   #$06
   LDA   (DPTR),Y
   LDY   #$6B
   STA   (FP),Y
   LDY   #$07
   LDA   (DPTR),Y
   LDY   #$6C
   STA   (FP),Y
   LDA   #$FF
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
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
   LDY   #$6C
   LDA   (FP),Y
   LDY   #$74
   CMP   (FP),Y
   BNE   .cmp_true@16
   LDY   #$6B
   LDA   (FP),Y
   LDY   #$73
   CMP   (FP),Y
   BNE   .cmp_true@16
   LDY   #$6A
   LDA   (FP),Y
   LDY   #$72
   CMP   (FP),Y
   BNE   .cmp_true@16
   LDY   #$69
   LDA   (FP),Y
   LDY   #$71
   CMP   (FP),Y
   BNE   .cmp_true@16
   LDY   #$68
   LDA   (FP),Y
   LDY   #$70
   CMP   (FP),Y
   BNE   .cmp_true@16
   LDY   #$67
   LDA   (FP),Y
   LDY   #$6F
   CMP   (FP),Y
   BNE   .cmp_true@16
   LDY   #$66
   LDA   (FP),Y
   LDY   #$6E
   CMP   (FP),Y
   BNE   .cmp_true@16
   LDY   #$65
   LDA   (FP),Y
   LDY   #$6D
   CMP   (FP),Y
   BNE   .cmp_true@16
   LDA   #$00
   JMP   .cmp_end@17
.cmp_true@16:
   LDA   #$01
.cmp_end@17:
   LDY   #$75
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
   ADC   #$8E
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$8D
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
   LDA   #$03
   STA   HARGS
   LDA   #$00
   LDX   #$08
   LSR   HARGS
.mul_loop@19:
   BCC   .mul_skip@20
   CLC
   ADC   #$08
.mul_skip@20:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@19
   LDY   #$78
   STA   (FP),Y
   LDA   HARGS
   DEY
   STA   (FP),Y
   LDY   #$5F
   LDA   (FP),Y
   CLC
   LDY   #$77
   ADC   (FP),Y
   LDY   #$79
   STA   (FP),Y
   LDY   #$60
   LDA   (FP),Y
   LDY   #$78
   ADC   (FP),Y
   LDY   #$7A
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$7B
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$7C
   STA   (FP),Y
   LDY   #$02
   LDA   (DPTR),Y
   LDY   #$7D
   STA   (FP),Y
   LDY   #$03
   LDA   (DPTR),Y
   LDY   #$7E
   STA   (FP),Y
   LDY   #$04
   LDA   (DPTR),Y
   LDY   #$7F
   STA   (FP),Y
   LDY   #$05
   LDA   (DPTR),Y
   LDY   #$80
   STA   (FP),Y
   LDY   #$06
   LDA   (DPTR),Y
   LDY   #$81
   STA   (FP),Y
   LDY   #$07
   LDA   (DPTR),Y
   LDY   #$82
   STA   (FP),Y
   LDA   #$FC
   INY
   STA   (FP),Y
   LDA   #$FF
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
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
   LDY   #$82
   LDA   (FP),Y
   LDY   #$8A
   CMP   (FP),Y
   BNE   .cmp_true@21
   LDY   #$81
   LDA   (FP),Y
   LDY   #$89
   CMP   (FP),Y
   BNE   .cmp_true@21
   LDY   #$80
   LDA   (FP),Y
   LDY   #$88
   CMP   (FP),Y
   BNE   .cmp_true@21
   LDY   #$7F
   LDA   (FP),Y
   LDY   #$87
   CMP   (FP),Y
   BNE   .cmp_true@21
   LDY   #$7E
   LDA   (FP),Y
   LDY   #$86
   CMP   (FP),Y
   BNE   .cmp_true@21
   LDY   #$7D
   LDA   (FP),Y
   LDY   #$85
   CMP   (FP),Y
   BNE   .cmp_true@21
   LDY   #$7C
   LDA   (FP),Y
   LDY   #$84
   CMP   (FP),Y
   BNE   .cmp_true@21
   LDY   #$7B
   LDA   (FP),Y
   LDY   #$83
   CMP   (FP),Y
   BNE   .cmp_true@21
   LDA   #$00
   JMP   .cmp_end@22
.cmp_true@21:
   LDA   #$01
.cmp_end@22:
   LDY   #$8B
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
   ADC   #$8E
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
.if_end@3:
   LDX   #$00
   TXA

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$8E
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$8D
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
