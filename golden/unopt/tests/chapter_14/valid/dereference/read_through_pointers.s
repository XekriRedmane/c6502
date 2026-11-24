SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24

main:
   SUBROUTINE

   ; prologue: 0 arg bytes, 178 local bytes
   SEC
   LDA   SSP
   SBC   #$B4
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$B3
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
   SBC   #$64
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
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$C0
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
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$0C
   INY
   STA   (FP),Y
   LDA   #$40
   INY
   STA   (FP),Y
   CLC
   LDA   FP
   ADC   #$03
   INY
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
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
   CLC
   LDA   FP
   ADC   #$05
   INY
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$1B
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$1C
   STA   (FP),Y
   CLC
   LDA   FP
   ADC   #$0D
   INY
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$1F
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$20
   STA   (FP),Y
   LDY   #$17
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$21
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$22
   STA   (FP),Y
   SEC
   LDA   #$00
   SBC   #$64
   INY
   STA   (FP),Y
   LDA   #$00
   SBC   #$00
   INY
   STA   (FP),Y
   LDY   #$22
   LDA   (FP),Y
   LDY   #$24
   CMP   (FP),Y
   BNE   .cmp_true@0
   LDY   #$21
   LDA   (FP),Y
   LDY   #$23
   CMP   (FP),Y
   BNE   .cmp_true@0
   LDA   #$00
   JMP   .cmp_end@1
.cmp_true@0:
   LDA   #$01
.cmp_end@1:
   LDY   #$25
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
   ADC   #$B4
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$B3
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
   LDY   #$1B
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$27
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$28
   STA   (FP),Y
   LDY   #$02
   LDA   (DPTR),Y
   LDY   #$29
   STA   (FP),Y
   LDY   #$03
   LDA   (DPTR),Y
   LDY   #$2A
   STA   (FP),Y
   LDY   #$04
   LDA   (DPTR),Y
   LDY   #$2B
   STA   (FP),Y
   LDY   #$05
   LDA   (DPTR),Y
   LDY   #$2C
   STA   (FP),Y
   LDY   #$06
   LDA   (DPTR),Y
   LDY   #$2D
   STA   (FP),Y
   LDY   #$07
   LDA   (DPTR),Y
   LDY   #$2E
   STA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@3
   DEY
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
   CMP   #$C0
   BNE   .cmp_true@3
   DEY
   LDA   (FP),Y
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
   LDY   #$2F
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
   ADC   #$B4
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$B3
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
   LDY   #$1F
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$31
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$32
   STA   (FP),Y
   LDY   #$02
   LDA   (DPTR),Y
   LDY   #$33
   STA   (FP),Y
   LDY   #$03
   LDA   (DPTR),Y
   LDY   #$34
   STA   (FP),Y
   LDY   #$04
   LDA   (DPTR),Y
   LDY   #$35
   STA   (FP),Y
   LDY   #$05
   LDA   (DPTR),Y
   LDY   #$36
   STA   (FP),Y
   LDY   #$06
   LDA   (DPTR),Y
   LDY   #$37
   STA   (FP),Y
   LDY   #$07
   LDA   (DPTR),Y
   LDY   #$38
   STA   (FP),Y
   CMP   #$40
   BNE   .cmp_true@6
   DEY
   LDA   (FP),Y
   CMP   #$0C
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
   LDY   #$39
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
   ADC   #$B4
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$B3
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
   LDA   #$0C
   LDY   #$03
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDA   #$E8
   LDY   #$3B
   STA   (FP),Y
   LDA   #$03
   INY
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@9
   LDA   #$00
   JMP   .sx_done@10
.sx_neg@9:
   LDA   #$FF
.sx_done@10:
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
   LDY   #$3B
   LDA   (FP),Y
   LDY   #$05
   STA   (FP),Y
   LDY   #$3C
   LDA   (FP),Y
   LDY   #$06
   STA   (FP),Y
   LDY   #$3D
   LDA   (FP),Y
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
   LDA   #$FC
   LDY   #$43
   STA   (FP),Y
   LDA   #$A9
   INY
   STA   (FP),Y
   LDA   #$F1
   INY
   STA   (FP),Y
   LDA   #$D2
   INY
   STA   (FP),Y
   LDA   #$4D
   INY
   STA   (FP),Y
   LDA   #$62
   INY
   STA   (FP),Y
   LDA   #$50
   INY
   STA   (FP),Y
   LDA   #$3F
   EOR   #$80
   INY
   STA   (FP),Y
   LDY   #$43
   LDA   (FP),Y
   LDY   #$0D
   STA   (FP),Y
   LDY   #$44
   LDA   (FP),Y
   LDY   #$0E
   STA   (FP),Y
   LDY   #$45
   LDA   (FP),Y
   LDY   #$0F
   STA   (FP),Y
   LDY   #$46
   LDA   (FP),Y
   LDY   #$10
   STA   (FP),Y
   LDY   #$47
   LDA   (FP),Y
   LDY   #$11
   STA   (FP),Y
   LDY   #$48
   LDA   (FP),Y
   LDY   #$12
   STA   (FP),Y
   LDY   #$49
   LDA   (FP),Y
   LDY   #$13
   STA   (FP),Y
   LDY   #$4A
   LDA   (FP),Y
   LDY   #$14
   STA   (FP),Y
   LDY   #$17
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$4B
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$4C
   STA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@11
   DEY
   LDA   (FP),Y
   CMP   #$0C
   BNE   .cmp_true@11
   LDA   #$00
   JMP   .cmp_end@12
.cmp_true@11:
   LDA   #$01
.cmp_end@12:
   LDY   #$4D
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
   ADC   #$B4
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$B3
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
   LDY   #$1B
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$4F
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$50
   STA   (FP),Y
   LDY   #$02
   LDA   (DPTR),Y
   LDY   #$51
   STA   (FP),Y
   LDY   #$03
   LDA   (DPTR),Y
   LDY   #$52
   STA   (FP),Y
   LDY   #$04
   LDA   (DPTR),Y
   LDY   #$53
   STA   (FP),Y
   LDY   #$05
   LDA   (DPTR),Y
   LDY   #$54
   STA   (FP),Y
   LDY   #$06
   LDA   (DPTR),Y
   LDY   #$55
   STA   (FP),Y
   LDY   #$07
   LDA   (DPTR),Y
   LDY   #$56
   STA   (FP),Y
   LDA   #$E8
   INY
   STA   (FP),Y
   LDA   #$03
   INY
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@14
   LDA   #$00
   JMP   .sx_done@15
.sx_neg@14:
   LDA   #$FF
.sx_done@15:
   LDY   #$59
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
   LDY   #$56
   LDA   (FP),Y
   LDY   #$5E
   CMP   (FP),Y
   BNE   .cmp_true@16
   LDY   #$55
   LDA   (FP),Y
   LDY   #$5D
   CMP   (FP),Y
   BNE   .cmp_true@16
   LDY   #$54
   LDA   (FP),Y
   LDY   #$5C
   CMP   (FP),Y
   BNE   .cmp_true@16
   LDY   #$53
   LDA   (FP),Y
   LDY   #$5B
   CMP   (FP),Y
   BNE   .cmp_true@16
   LDY   #$52
   LDA   (FP),Y
   LDY   #$5A
   CMP   (FP),Y
   BNE   .cmp_true@16
   LDY   #$51
   LDA   (FP),Y
   LDY   #$59
   CMP   (FP),Y
   BNE   .cmp_true@16
   LDY   #$50
   LDA   (FP),Y
   LDY   #$58
   CMP   (FP),Y
   BNE   .cmp_true@16
   LDY   #$4F
   LDA   (FP),Y
   LDY   #$57
   CMP   (FP),Y
   BNE   .cmp_true@16
   LDA   #$00
   JMP   .cmp_end@17
.cmp_true@16:
   LDA   #$01
.cmp_end@17:
   LDY   #$5F
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
   ADC   #$B4
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$B3
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
   LDY   #$1F
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$61
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$62
   STA   (FP),Y
   LDY   #$02
   LDA   (DPTR),Y
   LDY   #$63
   STA   (FP),Y
   LDY   #$03
   LDA   (DPTR),Y
   LDY   #$64
   STA   (FP),Y
   LDY   #$04
   LDA   (DPTR),Y
   LDY   #$65
   STA   (FP),Y
   LDY   #$05
   LDA   (DPTR),Y
   LDY   #$66
   STA   (FP),Y
   LDY   #$06
   LDA   (DPTR),Y
   LDY   #$67
   STA   (FP),Y
   LDY   #$07
   LDA   (DPTR),Y
   LDY   #$68
   STA   (FP),Y
   LDA   #$FC
   INY
   STA   (FP),Y
   LDA   #$A9
   INY
   STA   (FP),Y
   LDA   #$F1
   INY
   STA   (FP),Y
   LDA   #$D2
   INY
   STA   (FP),Y
   LDA   #$4D
   INY
   STA   (FP),Y
   LDA   #$62
   INY
   STA   (FP),Y
   LDA   #$50
   INY
   STA   (FP),Y
   LDA   #$3F
   EOR   #$80
   INY
   STA   (FP),Y
   LDY   #$68
   LDA   (FP),Y
   LDY   #$70
   CMP   (FP),Y
   BNE   .cmp_true@19
   LDY   #$67
   LDA   (FP),Y
   LDY   #$6F
   CMP   (FP),Y
   BNE   .cmp_true@19
   LDY   #$66
   LDA   (FP),Y
   LDY   #$6E
   CMP   (FP),Y
   BNE   .cmp_true@19
   LDY   #$65
   LDA   (FP),Y
   LDY   #$6D
   CMP   (FP),Y
   BNE   .cmp_true@19
   LDY   #$64
   LDA   (FP),Y
   LDY   #$6C
   CMP   (FP),Y
   BNE   .cmp_true@19
   LDY   #$63
   LDA   (FP),Y
   LDY   #$6B
   CMP   (FP),Y
   BNE   .cmp_true@19
   LDY   #$62
   LDA   (FP),Y
   LDY   #$6A
   CMP   (FP),Y
   BNE   .cmp_true@19
   LDY   #$61
   LDA   (FP),Y
   LDY   #$69
   CMP   (FP),Y
   BNE   .cmp_true@19
   LDA   #$00
   JMP   .cmp_end@20
.cmp_true@19:
   LDA   #$01
.cmp_end@20:
   LDY   #$71
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
   ADC   #$B4
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$B3
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
   LDA   #$01
   LDY   #$73
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
   LDA   #$02
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
   LDA   #$66
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
   LDA   #$A6
   INY
   STA   (FP),Y
   LDA   #$40
   INY
   STA   (FP),Y
   EOR   #$80
   INY
   STA   (FP),Y
   LDY   #$7D
   LDA   (FP),Y
   LDY   #$85
   STA   (FP),Y
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
   STA   (FP),Y
   LDY   #$83
   LDA   (FP),Y
   LDY   #$8B
   STA   (FP),Y
   LDY   #$84
   LDA   (FP),Y
   LDY   #$8C
   STA   (FP),Y
   CLC
   LDA   FP
   ADC   #$73
   INY
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$17
   STA   (FP),Y
   LDY   #$8E
   LDA   (FP),Y
   LDY   #$18
   STA   (FP),Y
   CLC
   LDA   FP
   ADC   #$75
   LDY   #$8F
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$1B
   STA   (FP),Y
   LDY   #$90
   LDA   (FP),Y
   LDY   #$1C
   STA   (FP),Y
   CLC
   LDA   FP
   ADC   #$85
   LDY   #$91
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$1F
   STA   (FP),Y
   LDY   #$92
   LDA   (FP),Y
   LDY   #$20
   STA   (FP),Y
   LDY   #$17
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$93
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$94
   STA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@22
   DEY
   LDA   (FP),Y
   CMP   #$01
   BNE   .cmp_true@22
   LDA   #$00
   JMP   .cmp_end@23
.cmp_true@22:
   LDA   #$01
.cmp_end@23:
   LDY   #$95
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
   ADC   #$B4
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$B3
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
   LDY   #$1B
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$97
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$98
   STA   (FP),Y
   LDY   #$02
   LDA   (DPTR),Y
   LDY   #$99
   STA   (FP),Y
   LDY   #$03
   LDA   (DPTR),Y
   LDY   #$9A
   STA   (FP),Y
   LDY   #$04
   LDA   (DPTR),Y
   LDY   #$9B
   STA   (FP),Y
   LDY   #$05
   LDA   (DPTR),Y
   LDY   #$9C
   STA   (FP),Y
   LDY   #$06
   LDA   (DPTR),Y
   LDY   #$9D
   STA   (FP),Y
   LDY   #$07
   LDA   (DPTR),Y
   LDY   #$9E
   STA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@25
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@25
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@25
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@25
   DEY
   LDA   (FP),Y
   CMP   #$02
   BNE   .cmp_true@25
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@25
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@25
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@25
   LDA   #$00
   JMP   .cmp_end@26
.cmp_true@25:
   LDA   #$01
.cmp_end@26:
   LDY   #$9F
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
   ADC   #$B4
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$B3
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
   LDY   #$1F
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$A1
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$A2
   STA   (FP),Y
   LDY   #$02
   LDA   (DPTR),Y
   LDY   #$A3
   STA   (FP),Y
   LDY   #$03
   LDA   (DPTR),Y
   LDY   #$A4
   STA   (FP),Y
   LDY   #$04
   LDA   (DPTR),Y
   LDY   #$A5
   STA   (FP),Y
   LDY   #$05
   LDA   (DPTR),Y
   LDY   #$A6
   STA   (FP),Y
   LDY   #$06
   LDA   (DPTR),Y
   LDY   #$A7
   STA   (FP),Y
   LDY   #$07
   LDA   (DPTR),Y
   LDY   #$A8
   STA   (FP),Y
   LDA   #$66
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
   LDA   #$A6
   INY
   STA   (FP),Y
   LDA   #$40
   INY
   STA   (FP),Y
   EOR   #$80
   INY
   STA   (FP),Y
   LDY   #$A8
   LDA   (FP),Y
   LDY   #$B0
   CMP   (FP),Y
   BNE   .cmp_true@28
   LDY   #$A7
   LDA   (FP),Y
   LDY   #$AF
   CMP   (FP),Y
   BNE   .cmp_true@28
   LDY   #$A6
   LDA   (FP),Y
   LDY   #$AE
   CMP   (FP),Y
   BNE   .cmp_true@28
   LDY   #$A5
   LDA   (FP),Y
   LDY   #$AD
   CMP   (FP),Y
   BNE   .cmp_true@28
   LDY   #$A4
   LDA   (FP),Y
   LDY   #$AC
   CMP   (FP),Y
   BNE   .cmp_true@28
   LDY   #$A3
   LDA   (FP),Y
   LDY   #$AB
   CMP   (FP),Y
   BNE   .cmp_true@28
   LDY   #$A2
   LDA   (FP),Y
   LDY   #$AA
   CMP   (FP),Y
   BNE   .cmp_true@28
   LDY   #$A1
   LDA   (FP),Y
   LDY   #$A9
   CMP   (FP),Y
   BNE   .cmp_true@28
   LDA   #$00
   JMP   .cmp_end@29
.cmp_true@28:
   LDA   #$01
.cmp_end@29:
   LDY   #$B1
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@8
   LDX   #$00
   LDA   #$09

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$B4
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
.if_end@8:
   LDX   #$00
   TXA

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$B4
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$B3
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
