SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24

main:
   SUBROUTINE

   ; prologue: 0 arg bytes, 246 local bytes
   SEC
   LDA   SSP
   SBC   #$F8
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$F7
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   LDA   #$61
   LDY   #$01
   STA   (FP),Y
   CLC
   LDA   FP
   ADC   #$02
   LDY   #$0B
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
   LDY   #$01
   LDA   (FP),Y
   DEY
   STA   (DPTR),Y
   LDA   #$A0
   LDY   #$0D
   STA   (FP),Y
   LDA   #$86
   INY
   STA   (FP),Y
   LDA   #$01
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
   LDY   #$0B
   LDA   (FP),Y
   CLC
   ADC   #$01
   LDY   #$15
   STA   (FP),Y
   LDY   #$0C
   LDA   (FP),Y
   ADC   #$00
   LDY   #$16
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$0D
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$0E
   LDA   (FP),Y
   LDY   #$01
   STA   (DPTR),Y
   LDY   #$0F
   LDA   (FP),Y
   LDY   #$02
   STA   (DPTR),Y
   LDY   #$10
   LDA   (FP),Y
   LDY   #$03
   STA   (DPTR),Y
   LDY   #$11
   LDA   (FP),Y
   LDY   #$04
   STA   (DPTR),Y
   LDY   #$12
   LDA   (FP),Y
   LDY   #$05
   STA   (DPTR),Y
   LDY   #$13
   LDA   (FP),Y
   LDY   #$06
   STA   (DPTR),Y
   LDY   #$14
   LDA   (FP),Y
   LDY   #$07
   STA   (DPTR),Y
   LDA   #$02
   LDY   #$17
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$80
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
   CLC
   LDA   FP
   ADC   #$1F
   LDY   #$34
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
   LDY   #$17
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$18
   LDA   (FP),Y
   LDY   #$01
   STA   (DPTR),Y
   LDY   #$19
   LDA   (FP),Y
   LDY   #$02
   STA   (DPTR),Y
   LDY   #$1A
   LDA   (FP),Y
   LDY   #$03
   STA   (DPTR),Y
   LDY   #$1B
   LDA   (FP),Y
   LDY   #$04
   STA   (DPTR),Y
   LDY   #$1C
   LDA   (FP),Y
   LDY   #$05
   STA   (DPTR),Y
   LDY   #$1D
   LDA   (FP),Y
   LDY   #$06
   STA   (DPTR),Y
   LDY   #$1E
   LDA   (FP),Y
   LDY   #$07
   STA   (DPTR),Y
   CLC
   LDA   FP
   ADC   #$02
   LDY   #$38
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   LDY   #$34
   LDA   (FP),Y
   CLC
   ADC   #$08
   LDY   #$36
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$37
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   INY
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$39
   LDA   (FP),Y
   LDY   #$01
   STA   (DPTR),Y
   LDY   #$34
   LDA   (FP),Y
   CLC
   ADC   #$0A
   LDY   #$3A
   STA   (FP),Y
   LDY   #$35
   LDA   (FP),Y
   ADC   #$00
   LDY   #$3B
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDA   #$64
   LDY   #$00
   STA   (DPTR),Y
   LDA   #$00
   INY
   STA   (DPTR),Y
   SEC
   SBC   #$50
   LDY   #$3C
   STA   (FP),Y
   LDA   #$00
   SBC   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$3E
   STA   (FP),Y
   LDY   #$34
   LDA   (FP),Y
   CLC
   ADC   #$0C
   LDY   #$3F
   STA   (FP),Y
   LDY   #$35
   LDA   (FP),Y
   ADC   #$00
   LDY   #$40
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$3E
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDA   #$FF
   LDY   #$41
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
   LDY   #$34
   LDA   (FP),Y
   CLC
   ADC   #$0D
   LDY   #$49
   STA   (FP),Y
   LDY   #$35
   LDA   (FP),Y
   ADC   #$00
   LDY   #$4A
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$41
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$42
   LDA   (FP),Y
   LDY   #$01
   STA   (DPTR),Y
   LDY   #$43
   LDA   (FP),Y
   LDY   #$02
   STA   (DPTR),Y
   LDY   #$44
   LDA   (FP),Y
   LDY   #$03
   STA   (DPTR),Y
   LDY   #$45
   LDA   (FP),Y
   LDY   #$04
   STA   (DPTR),Y
   LDY   #$46
   LDA   (FP),Y
   LDY   #$05
   STA   (DPTR),Y
   LDY   #$47
   LDA   (FP),Y
   LDY   #$06
   STA   (DPTR),Y
   LDY   #$48
   LDA   (FP),Y
   LDY   #$07
   STA   (DPTR),Y
   CLC
   LDA   FP
   ADC   #$02
   LDY   #$4B
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
   LDY   #$4D
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$50
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$51
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@0
   LDA   #$00
   JMP   .sx_done@1
.sx_neg@0:
   LDA   #$FF
.sx_done@1:
   LDY   #$52
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
   ADC   #$1F
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
   LDY   #$5A
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$5B
   STA   (FP),Y
   LDY   #$02
   LDA   (DPTR),Y
   LDY   #$5C
   STA   (FP),Y
   LDY   #$03
   LDA   (DPTR),Y
   LDY   #$5D
   STA   (FP),Y
   LDY   #$04
   LDA   (DPTR),Y
   LDY   #$5E
   STA   (FP),Y
   LDY   #$05
   LDA   (DPTR),Y
   LDY   #$5F
   STA   (FP),Y
   LDY   #$06
   LDA   (DPTR),Y
   LDY   #$60
   STA   (FP),Y
   LDY   #$07
   LDA   (DPTR),Y
   LDY   #$61
   STA   (FP),Y
   LDY   #$50
   LDA   (FP),Y
   LDY   #$5A
   ORA   (FP),Y
   LDY   #$62
   STA   (FP),Y
   LDY   #$51
   LDA   (FP),Y
   LDY   #$5B
   ORA   (FP),Y
   LDY   #$63
   STA   (FP),Y
   LDY   #$52
   LDA   (FP),Y
   LDY   #$5C
   ORA   (FP),Y
   LDY   #$64
   STA   (FP),Y
   LDY   #$53
   LDA   (FP),Y
   LDY   #$5D
   ORA   (FP),Y
   LDY   #$65
   STA   (FP),Y
   LDY   #$54
   LDA   (FP),Y
   LDY   #$5E
   ORA   (FP),Y
   LDY   #$66
   STA   (FP),Y
   LDY   #$55
   LDA   (FP),Y
   LDY   #$5F
   ORA   (FP),Y
   LDY   #$67
   STA   (FP),Y
   LDY   #$56
   LDA   (FP),Y
   LDY   #$60
   ORA   (FP),Y
   LDY   #$68
   STA   (FP),Y
   LDY   #$57
   LDA   (FP),Y
   LDY   #$61
   ORA   (FP),Y
   LDY   #$69
   STA   (FP),Y
   LDA   #$63
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$80
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
   LDY   #$69
   LDA   (FP),Y
   LDY   #$71
   CMP   (FP),Y
   BNE   .cmp_true@2
   LDY   #$68
   LDA   (FP),Y
   LDY   #$70
   CMP   (FP),Y
   BNE   .cmp_true@2
   LDY   #$67
   LDA   (FP),Y
   LDY   #$6F
   CMP   (FP),Y
   BNE   .cmp_true@2
   LDY   #$66
   LDA   (FP),Y
   LDY   #$6E
   CMP   (FP),Y
   BNE   .cmp_true@2
   LDY   #$65
   LDA   (FP),Y
   LDY   #$6D
   CMP   (FP),Y
   BNE   .cmp_true@2
   LDY   #$64
   LDA   (FP),Y
   LDY   #$6C
   CMP   (FP),Y
   BNE   .cmp_true@2
   LDY   #$63
   LDA   (FP),Y
   LDY   #$6B
   CMP   (FP),Y
   BNE   .cmp_true@2
   LDY   #$62
   LDA   (FP),Y
   LDY   #$6A
   CMP   (FP),Y
   BNE   .cmp_true@2
   LDA   #$00
   JMP   .cmp_end@3
.cmp_true@2:
   LDA   #$01
.cmp_end@3:
   LDY   #$72
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
   ADC   #$F8
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$F7
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
   ADC   #$1F
   LDY   #$74
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$0A
   LDY   #$76
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$77
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$78
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$79
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$7A
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$7B
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@5
   LDA   #$00
   JMP   .sx_done@6
.sx_neg@5:
   LDA   #$FF
.sx_done@6:
   LDY   #$7C
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
   ADC   #$02
   INY
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$01
   LDY   #$84
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$85
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$86
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$87
   STA   (FP),Y
   LDY   #$02
   LDA   (DPTR),Y
   LDY   #$88
   STA   (FP),Y
   LDY   #$03
   LDA   (DPTR),Y
   LDY   #$89
   STA   (FP),Y
   LDY   #$04
   LDA   (DPTR),Y
   LDY   #$8A
   STA   (FP),Y
   LDY   #$05
   LDA   (DPTR),Y
   LDY   #$8B
   STA   (FP),Y
   LDY   #$06
   LDA   (DPTR),Y
   LDY   #$8C
   STA   (FP),Y
   LDY   #$07
   LDA   (DPTR),Y
   LDY   #$8D
   STA   (FP),Y
   LDY   #$7A
   LDA   (FP),Y
   LDY   #$86
   EOR   (FP),Y
   LDY   #$8E
   STA   (FP),Y
   LDY   #$7B
   LDA   (FP),Y
   LDY   #$87
   EOR   (FP),Y
   LDY   #$8F
   STA   (FP),Y
   LDY   #$7C
   LDA   (FP),Y
   LDY   #$88
   EOR   (FP),Y
   LDY   #$90
   STA   (FP),Y
   LDY   #$7D
   LDA   (FP),Y
   LDY   #$89
   EOR   (FP),Y
   LDY   #$91
   STA   (FP),Y
   LDY   #$7E
   LDA   (FP),Y
   LDY   #$8A
   EOR   (FP),Y
   LDY   #$92
   STA   (FP),Y
   LDY   #$7F
   LDA   (FP),Y
   LDY   #$8B
   EOR   (FP),Y
   LDY   #$93
   STA   (FP),Y
   LDY   #$80
   LDA   (FP),Y
   LDY   #$8C
   EOR   (FP),Y
   LDY   #$94
   STA   (FP),Y
   LDY   #$81
   LDA   (FP),Y
   LDY   #$8D
   EOR   (FP),Y
   LDY   #$95
   STA   (FP),Y
   LDA   #$C4
   INY
   STA   (FP),Y
   LDA   #$86
   INY
   STA   (FP),Y
   LDA   #$01
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
   LDY   #$95
   LDA   (FP),Y
   LDY   #$9D
   CMP   (FP),Y
   BNE   .cmp_true@7
   LDY   #$94
   LDA   (FP),Y
   LDY   #$9C
   CMP   (FP),Y
   BNE   .cmp_true@7
   LDY   #$93
   LDA   (FP),Y
   LDY   #$9B
   CMP   (FP),Y
   BNE   .cmp_true@7
   LDY   #$92
   LDA   (FP),Y
   LDY   #$9A
   CMP   (FP),Y
   BNE   .cmp_true@7
   LDY   #$91
   LDA   (FP),Y
   LDY   #$99
   CMP   (FP),Y
   BNE   .cmp_true@7
   LDY   #$90
   LDA   (FP),Y
   LDY   #$98
   CMP   (FP),Y
   BNE   .cmp_true@7
   LDY   #$8F
   LDA   (FP),Y
   LDY   #$97
   CMP   (FP),Y
   BNE   .cmp_true@7
   LDY   #$8E
   LDA   (FP),Y
   LDY   #$96
   CMP   (FP),Y
   BNE   .cmp_true@7
   LDA   #$00
   JMP   .cmp_end@8
.cmp_true@7:
   LDA   #$01
.cmp_end@8:
   LDY   #$9E
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
   ADC   #$F8
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$F7
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
   ADC   #$1F
   LDY   #$A0
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$08
   LDY   #$A2
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$A3
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$A4
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$A5
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$A6
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   CLC
   LDA   FP
   ADC   #$1F
   INY
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$0C
   LDY   #$AB
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$AC
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$AD
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$A7
   LDA   (FP),Y
   LDY   #$AE
   AND   (FP),Y
   LDY   #$B0
   STA   (FP),Y
   LDY   #$A8
   LDA   (FP),Y
   LDY   #$AF
   AND   (FP),Y
   LDY   #$B1
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CMP   #$20
   BNE   .cmp_true@10
   LDA   #$00
   JMP   .cmp_end@11
.cmp_true@10:
   LDA   #$01
.cmp_end@11:
   LDY   #$B2
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
   ADC   #$F8
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$F7
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
   CLC
   LDA   FP
   ADC   #$1F
   LDY   #$B4
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
   LDY   #$B6
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$B7
   STA   (FP),Y
   LDY   #$02
   LDA   (DPTR),Y
   LDY   #$B8
   STA   (FP),Y
   LDY   #$03
   LDA   (DPTR),Y
   LDY   #$B9
   STA   (FP),Y
   LDY   #$04
   LDA   (DPTR),Y
   LDY   #$BA
   STA   (FP),Y
   LDY   #$05
   LDA   (DPTR),Y
   LDY   #$BB
   STA   (FP),Y
   LDY   #$06
   LDA   (DPTR),Y
   LDY   #$BC
   STA   (FP),Y
   LDY   #$07
   LDA   (DPTR),Y
   LDY   #$BD
   STA   (FP),Y
   LDY   #$B6
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
   LDA   #$1A
   STA   HARGS+8
   JSR   lsr64
   LDA   HARGS+9
   LDY   #$BE
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
   LDA   #$20
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
   LDY   #$C2
   LDA   (FP),Y
   LDY   #$CA
   CMP   (FP),Y
   BNE   .cmp_true@12
   LDY   #$C1
   LDA   (FP),Y
   LDY   #$C9
   CMP   (FP),Y
   BNE   .cmp_true@12
   LDY   #$C0
   LDA   (FP),Y
   LDY   #$C8
   CMP   (FP),Y
   BNE   .cmp_true@12
   LDY   #$BF
   LDA   (FP),Y
   LDY   #$C7
   CMP   (FP),Y
   BNE   .cmp_true@12
   LDY   #$BE
   LDA   (FP),Y
   LDY   #$C6
   CMP   (FP),Y
   BNE   .cmp_true@12
   LDA   #$00
   JMP   .cmp_end@13
.cmp_true@12:
   LDA   #$01
.cmp_end@13:
   LDY   #$CE
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
   ADC   #$F8
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$F7
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
   CLC
   LDA   FP
   ADC   #$1F
   LDY   #$D0
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$0A
   LDY   #$D2
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$D3
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDA   #$0C
   LDY   #$00
   STA   (DPTR),Y
   LDA   #$00
   INY
   STA   (DPTR),Y
   CLC
   LDA   FP
   ADC   #$02
   LDY   #$D4
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
   LDY   #$D6
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
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   CLC
   LDA   FP
   ADC   #$1F
   INY
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$0A
   LDY   #$E1
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$E2
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$E3
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$E4
   STA   (FP),Y
   LDY   #$D7
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
   LDY   #$E3
   LDA   (FP),Y
   STA   HARGS+8
   JSR   asl64
   LDA   HARGS+9
   LDY   #$E5
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
   LDA   #$00
   INY
   STA   (FP),Y
   LDA   #$10
   INY
   STA   (FP),Y
   LDA   #$06
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   ORA   #$00
   BPL   .sx_done@16
   LDA   #$FF
.sx_done@16:
   LDY   #$F1
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$EC
   LDA   (FP),Y
   LDY   #$F4
   CMP   (FP),Y
   BNE   .cmp_true@17
   LDY   #$EB
   LDA   (FP),Y
   LDY   #$F3
   CMP   (FP),Y
   BNE   .cmp_true@17
   LDY   #$EA
   LDA   (FP),Y
   LDY   #$F2
   CMP   (FP),Y
   BNE   .cmp_true@17
   LDY   #$E9
   LDA   (FP),Y
   LDY   #$F1
   CMP   (FP),Y
   BNE   .cmp_true@17
   LDY   #$E8
   LDA   (FP),Y
   LDY   #$F0
   CMP   (FP),Y
   BNE   .cmp_true@17
   LDY   #$E7
   LDA   (FP),Y
   LDY   #$EF
   CMP   (FP),Y
   BNE   .cmp_true@17
   LDY   #$E6
   LDA   (FP),Y
   LDY   #$EE
   CMP   (FP),Y
   BNE   .cmp_true@17
   LDY   #$E5
   LDA   (FP),Y
   LDY   #$ED
   CMP   (FP),Y
   BNE   .cmp_true@17
   LDA   #$00
   JMP   .cmp_end@18
.cmp_true@17:
   LDA   #$01
.cmp_end@18:
   LDY   #$F5
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
   ADC   #$F8
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
   ADC   #$F8
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$F7
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
