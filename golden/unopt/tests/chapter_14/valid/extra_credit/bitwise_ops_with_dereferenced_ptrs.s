SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24

main:
   SUBROUTINE

   ; prologue: 0 arg bytes, 164 local bytes
   SEC
   LDA   SSP
   SBC   #$A6
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$A5
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
   LDY   #$01
   LDA   (FP),Y
   LDY   #$05
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   LDY   #$06
   STA   (FP),Y
   LDY   #$03
   LDA   (FP),Y
   LDY   #$07
   STA   (FP),Y
   LDY   #$04
   LDA   (FP),Y
   LDY   #$08
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
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
   ADC   #$05
   INY
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$13
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$14
   STA   (FP),Y
   CLC
   LDA   FP
   ADC   #$09
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
   LDY   #$13
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$19
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$1A
   STA   (FP),Y
   LDY   #$02
   LDA   (DPTR),Y
   LDY   #$1B
   STA   (FP),Y
   LDY   #$03
   LDA   (DPTR),Y
   LDY   #$1C
   STA   (FP),Y
   LDY   #$19
   LDA   (FP),Y
   LDY   #$1D
   STA   (FP),Y
   LDY   #$1A
   LDA   (FP),Y
   LDY   #$1E
   STA   (FP),Y
   LDY   #$1B
   LDA   (FP),Y
   LDY   #$1F
   STA   (FP),Y
   LDY   #$1C
   LDA   (FP),Y
   LDY   #$20
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
   LDY   #$17
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$25
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$26
   STA   (FP),Y
   LDY   #$02
   LDA   (DPTR),Y
   LDY   #$27
   STA   (FP),Y
   LDY   #$03
   LDA   (DPTR),Y
   LDY   #$28
   STA   (FP),Y
   LDY   #$04
   LDA   (DPTR),Y
   LDY   #$29
   STA   (FP),Y
   LDY   #$05
   LDA   (DPTR),Y
   LDY   #$2A
   STA   (FP),Y
   LDY   #$06
   LDA   (DPTR),Y
   LDY   #$2B
   STA   (FP),Y
   LDY   #$07
   LDA   (DPTR),Y
   LDY   #$2C
   STA   (FP),Y
   LDY   #$1D
   LDA   (FP),Y
   LDY   #$25
   AND   (FP),Y
   LDY   #$2D
   STA   (FP),Y
   LDY   #$1E
   LDA   (FP),Y
   LDY   #$26
   AND   (FP),Y
   LDY   #$2E
   STA   (FP),Y
   LDY   #$1F
   LDA   (FP),Y
   LDY   #$27
   AND   (FP),Y
   LDY   #$2F
   STA   (FP),Y
   LDY   #$20
   LDA   (FP),Y
   LDY   #$28
   AND   (FP),Y
   LDY   #$30
   STA   (FP),Y
   LDY   #$21
   LDA   (FP),Y
   LDY   #$29
   AND   (FP),Y
   LDY   #$31
   STA   (FP),Y
   LDY   #$22
   LDA   (FP),Y
   LDY   #$2A
   AND   (FP),Y
   LDY   #$32
   STA   (FP),Y
   LDY   #$23
   LDA   (FP),Y
   LDY   #$2B
   AND   (FP),Y
   LDY   #$33
   STA   (FP),Y
   LDY   #$24
   LDA   (FP),Y
   LDY   #$2C
   AND   (FP),Y
   LDY   #$34
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   ORA   #$00
   BPL   .sx_done@1
   LDA   #$FF
.sx_done@1:
   LDY   #$37
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
   LDY   #$34
   LDA   (FP),Y
   LDY   #$3C
   CMP   (FP),Y
   BNE   .cmp_true@2
   LDY   #$33
   LDA   (FP),Y
   LDY   #$3B
   CMP   (FP),Y
   BNE   .cmp_true@2
   LDY   #$32
   LDA   (FP),Y
   LDY   #$3A
   CMP   (FP),Y
   BNE   .cmp_true@2
   LDY   #$31
   LDA   (FP),Y
   LDY   #$39
   CMP   (FP),Y
   BNE   .cmp_true@2
   LDY   #$30
   LDA   (FP),Y
   LDY   #$38
   CMP   (FP),Y
   BNE   .cmp_true@2
   LDY   #$2F
   LDA   (FP),Y
   LDY   #$37
   CMP   (FP),Y
   BNE   .cmp_true@2
   LDY   #$2E
   LDA   (FP),Y
   LDY   #$36
   CMP   (FP),Y
   BNE   .cmp_true@2
   LDY   #$2D
   LDA   (FP),Y
   LDY   #$35
   CMP   (FP),Y
   BNE   .cmp_true@2
   LDA   #$00
   JMP   .cmp_end@3
.cmp_true@2:
   LDA   #$01
.cmp_end@3:
   LDY   #$3D
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
   ADC   #$A6
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$A5
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
   LDY   #$13
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$3F
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$40
   STA   (FP),Y
   LDY   #$02
   LDA   (DPTR),Y
   LDY   #$41
   STA   (FP),Y
   LDY   #$03
   LDA   (DPTR),Y
   LDY   #$42
   STA   (FP),Y
   LDY   #$3F
   LDA   (FP),Y
   LDY   #$43
   STA   (FP),Y
   LDY   #$40
   LDA   (FP),Y
   LDY   #$44
   STA   (FP),Y
   LDY   #$41
   LDA   (FP),Y
   LDY   #$45
   STA   (FP),Y
   LDY   #$42
   LDA   (FP),Y
   LDY   #$46
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
   LDY   #$02
   LDA   (DPTR),Y
   LDY   #$4D
   STA   (FP),Y
   LDY   #$03
   LDA   (DPTR),Y
   LDY   #$4E
   STA   (FP),Y
   LDY   #$04
   LDA   (DPTR),Y
   LDY   #$4F
   STA   (FP),Y
   LDY   #$05
   LDA   (DPTR),Y
   LDY   #$50
   STA   (FP),Y
   LDY   #$06
   LDA   (DPTR),Y
   LDY   #$51
   STA   (FP),Y
   LDY   #$07
   LDA   (DPTR),Y
   LDY   #$52
   STA   (FP),Y
   LDY   #$43
   LDA   (FP),Y
   LDY   #$4B
   ORA   (FP),Y
   LDY   #$53
   STA   (FP),Y
   LDY   #$44
   LDA   (FP),Y
   LDY   #$4C
   ORA   (FP),Y
   LDY   #$54
   STA   (FP),Y
   LDY   #$45
   LDA   (FP),Y
   LDY   #$4D
   ORA   (FP),Y
   LDY   #$55
   STA   (FP),Y
   LDY   #$46
   LDA   (FP),Y
   LDY   #$4E
   ORA   (FP),Y
   LDY   #$56
   STA   (FP),Y
   LDY   #$47
   LDA   (FP),Y
   LDY   #$4F
   ORA   (FP),Y
   LDY   #$57
   STA   (FP),Y
   LDY   #$48
   LDA   (FP),Y
   LDY   #$50
   ORA   (FP),Y
   LDY   #$58
   STA   (FP),Y
   LDY   #$49
   LDA   (FP),Y
   LDY   #$51
   ORA   (FP),Y
   LDY   #$59
   STA   (FP),Y
   LDY   #$4A
   LDA   (FP),Y
   LDY   #$52
   ORA   (FP),Y
   LDY   #$5A
   STA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@5
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@5
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@5
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@5
   DEY
   LDA   (FP),Y
   CMP   #$80
   BNE   .cmp_true@5
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@5
   DEY
   LDA   (FP),Y
   CMP   #$FF
   BNE   .cmp_true@5
   DEY
   LDA   (FP),Y
   CMP   #$FF
   BNE   .cmp_true@5
   LDA   #$00
   JMP   .cmp_end@6
.cmp_true@5:
   LDA   #$01
.cmp_end@6:
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
   ADC   #$A6
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$A5
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
   SBC   #$01
   LDY   #$5D
   STA   (FP),Y
   LDA   #$00
   SBC   #$00
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
   CLC
   LDA   FP
   ADC   #$5F
   INY
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$63
   STA   (FP),Y
   DEY
   LDA   (FP),Y
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
   DEY
   LDA   (FP),Y
   LDY   #$67
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$68
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@8
   LDA   #$00
   JMP   .sx_done@9
.sx_neg@8:
   LDA   #$FF
.sx_done@9:
   LDY   #$69
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
   LDY   #$67
   LDA   (FP),Y
   LDY   #$09
   AND   (FP),Y
   LDY   #$6F
   STA   (FP),Y
   LDY   #$68
   LDA   (FP),Y
   LDY   #$0A
   AND   (FP),Y
   LDY   #$70
   STA   (FP),Y
   LDY   #$69
   LDA   (FP),Y
   LDY   #$0B
   AND   (FP),Y
   LDY   #$71
   STA   (FP),Y
   LDY   #$6A
   LDA   (FP),Y
   LDY   #$0C
   AND   (FP),Y
   LDY   #$72
   STA   (FP),Y
   LDY   #$6B
   LDA   (FP),Y
   LDY   #$0D
   AND   (FP),Y
   LDY   #$73
   STA   (FP),Y
   LDY   #$6C
   LDA   (FP),Y
   LDY   #$0E
   AND   (FP),Y
   LDY   #$74
   STA   (FP),Y
   LDY   #$6D
   LDA   (FP),Y
   LDY   #$0F
   AND   (FP),Y
   LDY   #$75
   STA   (FP),Y
   LDY   #$6E
   LDA   (FP),Y
   LDY   #$10
   AND   (FP),Y
   LDY   #$76
   STA   (FP),Y
   LDY   #$17
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$77
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$78
   STA   (FP),Y
   LDY   #$02
   LDA   (DPTR),Y
   LDY   #$79
   STA   (FP),Y
   LDY   #$03
   LDA   (DPTR),Y
   LDY   #$7A
   STA   (FP),Y
   LDY   #$04
   LDA   (DPTR),Y
   LDY   #$7B
   STA   (FP),Y
   LDY   #$05
   LDA   (DPTR),Y
   LDY   #$7C
   STA   (FP),Y
   LDY   #$06
   LDA   (DPTR),Y
   LDY   #$7D
   STA   (FP),Y
   LDY   #$07
   LDA   (DPTR),Y
   LDY   #$7E
   STA   (FP),Y
   LDY   #$76
   LDA   (FP),Y
   LDY   #$7E
   CMP   (FP),Y
   BNE   .cmp_true@10
   LDY   #$75
   LDA   (FP),Y
   LDY   #$7D
   CMP   (FP),Y
   BNE   .cmp_true@10
   LDY   #$74
   LDA   (FP),Y
   LDY   #$7C
   CMP   (FP),Y
   BNE   .cmp_true@10
   LDY   #$73
   LDA   (FP),Y
   LDY   #$7B
   CMP   (FP),Y
   BNE   .cmp_true@10
   LDY   #$72
   LDA   (FP),Y
   LDY   #$7A
   CMP   (FP),Y
   BNE   .cmp_true@10
   LDY   #$71
   LDA   (FP),Y
   LDY   #$79
   CMP   (FP),Y
   BNE   .cmp_true@10
   LDY   #$70
   LDA   (FP),Y
   LDY   #$78
   CMP   (FP),Y
   BNE   .cmp_true@10
   LDY   #$6F
   LDA   (FP),Y
   LDY   #$77
   CMP   (FP),Y
   BNE   .cmp_true@10
   LDA   #$00
   JMP   .cmp_end@11
.cmp_true@10:
   LDA   #$01
.cmp_end@11:
   LDY   #$7F
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
   ADC   #$A6
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$A5
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
   LDY   #$63
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$81
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$82
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$83
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$84
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@13
   LDA   #$00
   JMP   .sx_done@14
.sx_neg@13:
   LDA   #$FF
.sx_done@14:
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
   INY
   STA   (FP),Y
   LDY   #$17
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$8B
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$8C
   STA   (FP),Y
   LDY   #$02
   LDA   (DPTR),Y
   LDY   #$8D
   STA   (FP),Y
   LDY   #$03
   LDA   (DPTR),Y
   LDY   #$8E
   STA   (FP),Y
   LDY   #$04
   LDA   (DPTR),Y
   LDY   #$8F
   STA   (FP),Y
   LDY   #$05
   LDA   (DPTR),Y
   LDY   #$90
   STA   (FP),Y
   LDY   #$06
   LDA   (DPTR),Y
   LDY   #$91
   STA   (FP),Y
   LDY   #$07
   LDA   (DPTR),Y
   LDY   #$92
   STA   (FP),Y
   LDY   #$83
   LDA   (FP),Y
   LDY   #$8B
   ORA   (FP),Y
   LDY   #$93
   STA   (FP),Y
   LDY   #$84
   LDA   (FP),Y
   LDY   #$8C
   ORA   (FP),Y
   LDY   #$94
   STA   (FP),Y
   LDY   #$85
   LDA   (FP),Y
   LDY   #$8D
   ORA   (FP),Y
   LDY   #$95
   STA   (FP),Y
   LDY   #$86
   LDA   (FP),Y
   LDY   #$8E
   ORA   (FP),Y
   LDY   #$96
   STA   (FP),Y
   LDY   #$87
   LDA   (FP),Y
   LDY   #$8F
   ORA   (FP),Y
   LDY   #$97
   STA   (FP),Y
   LDY   #$88
   LDA   (FP),Y
   LDY   #$90
   ORA   (FP),Y
   LDY   #$98
   STA   (FP),Y
   LDY   #$89
   LDA   (FP),Y
   LDY   #$91
   ORA   (FP),Y
   LDY   #$99
   STA   (FP),Y
   LDY   #$8A
   LDA   (FP),Y
   LDY   #$92
   ORA   (FP),Y
   LDY   #$9A
   STA   (FP),Y
   LDY   #$5F
   LDA   (FP),Y
   LDY   #$9B
   STA   (FP),Y
   LDY   #$60
   LDA   (FP),Y
   LDY   #$9C
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@15
   LDA   #$00
   JMP   .sx_done@16
.sx_neg@15:
   LDA   #$FF
.sx_done@16:
   LDY   #$9D
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
   LDY   #$9A
   LDA   (FP),Y
   LDY   #$A2
   CMP   (FP),Y
   BNE   .cmp_true@17
   LDY   #$99
   LDA   (FP),Y
   LDY   #$A1
   CMP   (FP),Y
   BNE   .cmp_true@17
   LDY   #$98
   LDA   (FP),Y
   LDY   #$A0
   CMP   (FP),Y
   BNE   .cmp_true@17
   LDY   #$97
   LDA   (FP),Y
   LDY   #$9F
   CMP   (FP),Y
   BNE   .cmp_true@17
   LDY   #$96
   LDA   (FP),Y
   LDY   #$9E
   CMP   (FP),Y
   BNE   .cmp_true@17
   LDY   #$95
   LDA   (FP),Y
   LDY   #$9D
   CMP   (FP),Y
   BNE   .cmp_true@17
   LDY   #$94
   LDA   (FP),Y
   LDY   #$9C
   CMP   (FP),Y
   BNE   .cmp_true@17
   LDY   #$93
   LDA   (FP),Y
   LDY   #$9B
   CMP   (FP),Y
   BNE   .cmp_true@17
   LDA   #$00
   JMP   .cmp_end@18
.cmp_true@17:
   LDA   #$01
.cmp_end@18:
   LDY   #$A3
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
   ADC   #$A6
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
   ADC   #$A6
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$A5
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
