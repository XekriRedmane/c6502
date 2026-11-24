SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24

main:
   SUBROUTINE

   ; prologue: 0 arg bytes, 182 local bytes
   SEC
   LDA   SSP
   SBC   #$B8
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$B7
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
   LDY   #$19
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
   LDA   #$01
   LDY   #$00
   STA   (DPTR),Y
   LDA   #$00
   INY
   STA   (DPTR),Y
   LDY   #$19
   LDA   (FP),Y
   CLC
   ADC   #$02
   LDY   #$1B
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$1C
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDA   #$02
   LDY   #$00
   STA   (DPTR),Y
   LDA   #$00
   INY
   STA   (DPTR),Y
   LDY   #$19
   LDA   (FP),Y
   CLC
   ADC   #$04
   LDY   #$1D
   STA   (FP),Y
   LDY   #$1A
   LDA   (FP),Y
   ADC   #$00
   LDY   #$1E
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDA   #$03
   LDY   #$00
   STA   (DPTR),Y
   LDA   #$00
   INY
   STA   (DPTR),Y
   LDY   #$19
   LDA   (FP),Y
   CLC
   ADC   #$06
   LDY   #$1F
   STA   (FP),Y
   LDY   #$1A
   LDA   (FP),Y
   ADC   #$00
   LDY   #$20
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDA   #$04
   LDY   #$00
   STA   (DPTR),Y
   LDA   #$00
   INY
   STA   (DPTR),Y
   LDY   #$19
   LDA   (FP),Y
   CLC
   ADC   #$08
   LDY   #$21
   STA   (FP),Y
   LDY   #$1A
   LDA   (FP),Y
   ADC   #$00
   LDY   #$22
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDA   #$05
   LDY   #$00
   STA   (DPTR),Y
   LDA   #$00
   INY
   STA   (DPTR),Y
   LDY   #$19
   LDA   (FP),Y
   CLC
   ADC   #$0A
   LDY   #$23
   STA   (FP),Y
   LDY   #$1A
   LDA   (FP),Y
   ADC   #$00
   LDY   #$24
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDA   #$06
   LDY   #$00
   STA   (DPTR),Y
   LDA   #$00
   INY
   STA   (DPTR),Y
   LDY   #$19
   LDA   (FP),Y
   CLC
   ADC   #$0C
   LDY   #$25
   STA   (FP),Y
   LDY   #$1A
   LDA   (FP),Y
   ADC   #$00
   LDY   #$26
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDA   #$07
   LDY   #$00
   STA   (DPTR),Y
   LDA   #$00
   INY
   STA   (DPTR),Y
   LDY   #$19
   LDA   (FP),Y
   CLC
   ADC   #$0E
   LDY   #$27
   STA   (FP),Y
   LDY   #$1A
   LDA   (FP),Y
   ADC   #$00
   LDY   #$28
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDA   #$08
   LDY   #$00
   STA   (DPTR),Y
   LDA   #$00
   INY
   STA   (DPTR),Y
   LDY   #$19
   LDA   (FP),Y
   CLC
   ADC   #$10
   LDY   #$29
   STA   (FP),Y
   LDY   #$1A
   LDA   (FP),Y
   ADC   #$00
   LDY   #$2A
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDA   #$09
   LDY   #$00
   STA   (DPTR),Y
   LDA   #$00
   INY
   STA   (DPTR),Y
   LDY   #$19
   LDA   (FP),Y
   CLC
   ADC   #$12
   LDY   #$2B
   STA   (FP),Y
   LDY   #$1A
   LDA   (FP),Y
   ADC   #$00
   LDY   #$2C
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDA   #$0A
   LDY   #$00
   STA   (DPTR),Y
   LDA   #$00
   INY
   STA   (DPTR),Y
   LDY   #$19
   LDA   (FP),Y
   CLC
   ADC   #$14
   LDY   #$2D
   STA   (FP),Y
   LDY   #$1A
   LDA   (FP),Y
   ADC   #$00
   LDY   #$2E
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDA   #$0B
   LDY   #$00
   STA   (DPTR),Y
   LDA   #$00
   INY
   STA   (DPTR),Y
   LDY   #$19
   LDA   (FP),Y
   CLC
   ADC   #$16
   LDY   #$2F
   STA   (FP),Y
   LDY   #$1A
   LDA   (FP),Y
   ADC   #$00
   LDY   #$30
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
   ADC   #$01
   LDY   #$35
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   LDA   i
   LDY   #$31
   STA   (FP),Y
   STA   HARGS
   LDA   #$08
   STA   HARGS+1
   JSR   mul8
   LDA   HARGS+2
   LDY   #$32
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   LDA   (FP),Y
   CLC
   LDY   #$33
   ADC   (FP),Y
   LDY   #$37
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$34
   ADC   (FP),Y
   LDY   #$38
   STA   (FP),Y
   LDA   j
   INY
   STA   (FP),Y
   STA   HARGS
   LDA   #$04
   STA   HARGS+1
   JSR   mul8
   LDA   HARGS+2
   LDY   #$3A
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$37
   LDA   (FP),Y
   CLC
   LDY   #$3B
   ADC   (FP),Y
   LDY   #$3D
   STA   (FP),Y
   LDY   #$38
   LDA   (FP),Y
   LDY   #$3C
   ADC   (FP),Y
   LDY   #$3E
   STA   (FP),Y
   LDA   k
   INY
   STA   (FP),Y
   STA   HARGS
   LDA   #$02
   STA   HARGS+1
   JSR   mul8
   LDA   HARGS+2
   LDY   #$40
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$3D
   LDA   (FP),Y
   CLC
   LDY   #$41
   ADC   (FP),Y
   LDY   #$43
   STA   (FP),Y
   LDY   #$3E
   LDA   (FP),Y
   LDY   #$42
   ADC   (FP),Y
   LDY   #$44
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$45
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$46
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$01
   LDY   #$47
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$48
   STA   (FP),Y
   LDY   #$43
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
   LDY   #$46
   LDA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@0
   DEY
   LDA   (FP),Y
   CMP   #$0B
   BNE   .cmp_true@0
   LDA   #$00
   JMP   .cmp_end@1
.cmp_true@0:
   LDA   #$01
.cmp_end@1:
   LDY   #$49
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
   ADC   #$B8
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$B7
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
   ADC   #$01
   LDY   #$4F
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   LDA   i
   LDY   #$4B
   STA   (FP),Y
   STA   HARGS
   LDA   #$08
   STA   HARGS+1
   JSR   mul8
   LDA   HARGS+2
   LDY   #$4C
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   LDA   (FP),Y
   CLC
   LDY   #$4D
   ADC   (FP),Y
   LDY   #$51
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$4E
   ADC   (FP),Y
   LDY   #$52
   STA   (FP),Y
   LDA   j
   INY
   STA   (FP),Y
   STA   HARGS
   LDA   #$04
   STA   HARGS+1
   JSR   mul8
   LDA   HARGS+2
   LDY   #$54
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$51
   LDA   (FP),Y
   CLC
   LDY   #$55
   ADC   (FP),Y
   LDY   #$57
   STA   (FP),Y
   LDY   #$52
   LDA   (FP),Y
   LDY   #$56
   ADC   (FP),Y
   LDY   #$58
   STA   (FP),Y
   LDA   k
   INY
   STA   (FP),Y
   STA   HARGS
   LDA   #$02
   STA   HARGS+1
   JSR   mul8
   LDA   HARGS+2
   LDY   #$5A
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$57
   LDA   (FP),Y
   CLC
   LDY   #$5B
   ADC   (FP),Y
   LDY   #$5D
   STA   (FP),Y
   LDY   #$58
   LDA   (FP),Y
   LDY   #$5C
   ADC   (FP),Y
   LDY   #$5E
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$5F
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$60
   STA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@3
   DEY
   LDA   (FP),Y
   CMP   #$0C
   BNE   .cmp_true@3
   LDA   #$00
   JMP   .cmp_end@4
.cmp_true@3:
   LDA   #$01
.cmp_end@4:
   LDY   #$61
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
   ADC   #$B8
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$B7
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
   ADC   #$01
   LDY   #$69
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   LDA   i
   SEC
   SBC   #$01
   LDY   #$63
   STA   (FP),Y
   LDA   i+1
   SBC   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   i
   INY
   LDA   (FP),Y
   STA   i+1
   DEY
   LDA   (FP),Y
   LDY   #$65
   STA   (FP),Y
   STA   HARGS
   LDA   #$08
   STA   HARGS+1
   JSR   mul8
   LDA   HARGS+2
   LDY   #$66
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   LDA   (FP),Y
   CLC
   LDY   #$67
   ADC   (FP),Y
   LDY   #$6B
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$68
   ADC   (FP),Y
   LDY   #$6C
   STA   (FP),Y
   LDA   j
   INY
   STA   (FP),Y
   LDA   j+1
   INY
   STA   (FP),Y
   LDA   j
   SEC
   SBC   #$01
   INY
   STA   (FP),Y
   LDA   j+1
   SBC   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   j
   INY
   LDA   (FP),Y
   STA   j+1
   LDY   #$6D
   LDA   (FP),Y
   LDY   #$71
   STA   (FP),Y
   STA   HARGS
   LDA   #$04
   STA   HARGS+1
   JSR   mul8
   LDA   HARGS+2
   LDY   #$72
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$6B
   LDA   (FP),Y
   CLC
   LDY   #$73
   ADC   (FP),Y
   LDY   #$75
   STA   (FP),Y
   LDY   #$6C
   LDA   (FP),Y
   LDY   #$74
   ADC   (FP),Y
   LDY   #$76
   STA   (FP),Y
   LDA   k
   CLC
   ADC   #$01
   INY
   STA   (FP),Y
   LDA   k+1
   ADC   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   k
   INY
   LDA   (FP),Y
   STA   k+1
   DEY
   LDA   (FP),Y
   LDY   #$79
   STA   (FP),Y
   STA   HARGS
   LDA   #$02
   STA   HARGS+1
   JSR   mul8
   LDA   HARGS+2
   LDY   #$7A
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$75
   LDA   (FP),Y
   CLC
   LDY   #$7B
   ADC   (FP),Y
   LDY   #$7D
   STA   (FP),Y
   LDY   #$76
   LDA   (FP),Y
   LDY   #$7C
   ADC   (FP),Y
   LDY   #$7E
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$7F
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$80
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$01
   LDY   #$81
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$82
   STA   (FP),Y
   LDY   #$7D
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$81
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$82
   LDA   (FP),Y
   LDY   #$01
   STA   (DPTR),Y
   LDY   #$82
   LDA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@6
   DEY
   LDA   (FP),Y
   CMP   #$09
   BNE   .cmp_true@6
   LDA   #$00
   JMP   .cmp_end@7
.cmp_true@6:
   LDA   #$01
.cmp_end@7:
   LDY   #$83
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
   ADC   #$B8
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$B7
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
   ADC   #$01
   LDY   #$89
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   LDA   i
   LDY   #$85
   STA   (FP),Y
   STA   HARGS
   LDA   #$08
   STA   HARGS+1
   JSR   mul8
   LDA   HARGS+2
   LDY   #$86
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   LDA   (FP),Y
   CLC
   LDY   #$87
   ADC   (FP),Y
   LDY   #$8B
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$88
   ADC   (FP),Y
   LDY   #$8C
   STA   (FP),Y
   LDA   j
   INY
   STA   (FP),Y
   STA   HARGS
   LDA   #$04
   STA   HARGS+1
   JSR   mul8
   LDA   HARGS+2
   LDY   #$8E
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$8B
   LDA   (FP),Y
   CLC
   LDY   #$8F
   ADC   (FP),Y
   LDY   #$91
   STA   (FP),Y
   LDY   #$8C
   LDA   (FP),Y
   LDY   #$90
   ADC   (FP),Y
   LDY   #$92
   STA   (FP),Y
   LDA   k
   INY
   STA   (FP),Y
   STA   HARGS
   LDA   #$02
   STA   HARGS+1
   JSR   mul8
   LDA   HARGS+2
   LDY   #$94
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$91
   LDA   (FP),Y
   CLC
   LDY   #$95
   ADC   (FP),Y
   LDY   #$97
   STA   (FP),Y
   LDY   #$92
   LDA   (FP),Y
   LDY   #$96
   ADC   (FP),Y
   LDY   #$98
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$99
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$9A
   STA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@9
   DEY
   LDA   (FP),Y
   CMP   #$06
   BNE   .cmp_true@9
   LDA   #$00
   JMP   .cmp_end@10
.cmp_true@9:
   LDA   #$01
.cmp_end@10:
   LDY   #$9B
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
   ADC   #$B8
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$B7
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
   ADC   #$01
   LDY   #$A1
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   LDA   i
   LDY   #$9D
   STA   (FP),Y
   STA   HARGS
   LDA   #$08
   STA   HARGS+1
   JSR   mul8
   LDA   HARGS+2
   LDY   #$9E
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   LDA   (FP),Y
   CLC
   LDY   #$9F
   ADC   (FP),Y
   LDY   #$A3
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$A0
   ADC   (FP),Y
   LDY   #$A4
   STA   (FP),Y
   LDA   j
   INY
   STA   (FP),Y
   STA   HARGS
   LDA   #$04
   STA   HARGS+1
   JSR   mul8
   LDA   HARGS+2
   LDY   #$A6
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$A3
   LDA   (FP),Y
   CLC
   LDY   #$A7
   ADC   (FP),Y
   LDY   #$A9
   STA   (FP),Y
   LDY   #$A4
   LDA   (FP),Y
   LDY   #$A8
   ADC   (FP),Y
   LDY   #$AA
   STA   (FP),Y
   LDA   k
   INY
   STA   (FP),Y
   STA   HARGS
   LDA   #$02
   STA   HARGS+1
   JSR   mul8
   LDA   HARGS+2
   LDY   #$AC
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$A9
   LDA   (FP),Y
   CLC
   LDY   #$AD
   ADC   (FP),Y
   LDY   #$AF
   STA   (FP),Y
   LDY   #$AA
   LDA   (FP),Y
   LDY   #$AE
   ADC   (FP),Y
   LDY   #$B0
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$B1
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$B2
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   SEC
   SBC   #$01
   LDY   #$B3
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   SBC   #$00
   LDY   #$B4
   STA   (FP),Y
   LDY   #$AF
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$B3
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$B4
   LDA   (FP),Y
   LDY   #$01
   STA   (DPTR),Y
   LDY   #$B4
   LDA   (FP),Y
   BNE   .cmp_true@12
   DEY
   LDA   (FP),Y
   CMP   #$05
   BNE   .cmp_true@12
   LDA   #$00
   JMP   .cmp_end@13
.cmp_true@12:
   LDA   #$01
.cmp_end@13:
   LDY   #$B5
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
   ADC   #$B8
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
   ADC   #$B8
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$B7
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

i:
   DC.W  $0002

j:
   DC.W  $0001

k:
   DS.B  2
