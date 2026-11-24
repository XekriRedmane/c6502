SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24

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

   LDA   #<ul
   LDY   #$01
   STA   (FP),Y
   LDA   #>ul
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
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$05
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$06
   STA   (FP),Y
   LDY   #$02
   LDA   (DPTR),Y
   LDY   #$07
   STA   (FP),Y
   LDY   #$03
   LDA   (DPTR),Y
   LDY   #$08
   STA   (FP),Y
   LDY   #$04
   LDA   (DPTR),Y
   LDY   #$09
   STA   (FP),Y
   LDY   #$05
   LDA   (DPTR),Y
   LDY   #$0A
   STA   (FP),Y
   LDY   #$06
   LDA   (DPTR),Y
   LDY   #$0B
   STA   (FP),Y
   LDY   #$07
   LDA   (DPTR),Y
   LDY   #$0C
   STA   (FP),Y
   SEC
   LDA   #$00
   SBC   #$E8
   INY
   STA   (FP),Y
   LDA   #$00
   SBC   #$03
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$0F
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$10
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@0
   LDA   #$00
   JMP   .sx_done@1
.sx_neg@0:
   LDA   #$FF
.sx_done@1:
   LDY   #$11
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
   LDY   #$0F
   AND   (FP),Y
   LDY   #$17
   STA   (FP),Y
   LDY   #$06
   LDA   (FP),Y
   LDY   #$10
   AND   (FP),Y
   LDY   #$18
   STA   (FP),Y
   LDY   #$07
   LDA   (FP),Y
   LDY   #$11
   AND   (FP),Y
   LDY   #$19
   STA   (FP),Y
   LDY   #$08
   LDA   (FP),Y
   LDY   #$12
   AND   (FP),Y
   LDY   #$1A
   STA   (FP),Y
   LDY   #$09
   LDA   (FP),Y
   LDY   #$13
   AND   (FP),Y
   LDY   #$1B
   STA   (FP),Y
   LDY   #$0A
   LDA   (FP),Y
   LDY   #$14
   AND   (FP),Y
   LDY   #$1C
   STA   (FP),Y
   LDY   #$0B
   LDA   (FP),Y
   LDY   #$15
   AND   (FP),Y
   LDY   #$1D
   STA   (FP),Y
   LDY   #$0C
   LDA   (FP),Y
   LDY   #$16
   AND   (FP),Y
   LDY   #$1E
   STA   (FP),Y
   LDY   #$03
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
   LDA   ul+7
   BNE   .cmp_true@2
   LDA   ul+6
   BNE   .cmp_true@2
   LDA   ul+5
   BNE   .cmp_true@2
   LDA   ul+4
   BNE   .cmp_true@2
   LDA   ul+3
   CMP   #$FB
   BNE   .cmp_true@2
   LDA   ul+2
   CMP   #$FA
   BNE   .cmp_true@2
   LDA   ul+1
   CMP   #$F8
   BNE   .cmp_true@2
   LDA   ul
   CMP   #$18
   BNE   .cmp_true@2
   LDA   #$00
   JMP   .cmp_end@3
.cmp_true@2:
   LDA   #$01
.cmp_end@3:
   LDY   #$1F
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
.if_end@0:
   LDY   #$03
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
   LDY   #$02
   LDA   (DPTR),Y
   LDY   #$23
   STA   (FP),Y
   LDY   #$03
   LDA   (DPTR),Y
   LDY   #$24
   STA   (FP),Y
   LDY   #$04
   LDA   (DPTR),Y
   LDY   #$25
   STA   (FP),Y
   LDY   #$05
   LDA   (DPTR),Y
   LDY   #$26
   STA   (FP),Y
   LDY   #$06
   LDA   (DPTR),Y
   LDY   #$27
   STA   (FP),Y
   LDY   #$07
   LDA   (DPTR),Y
   LDY   #$28
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDA   #$FF
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
   LDY   #$21
   LDA   (FP),Y
   LDY   #$29
   ORA   (FP),Y
   LDY   #$31
   STA   (FP),Y
   LDY   #$22
   LDA   (FP),Y
   LDY   #$2A
   ORA   (FP),Y
   LDY   #$32
   STA   (FP),Y
   LDY   #$23
   LDA   (FP),Y
   LDY   #$2B
   ORA   (FP),Y
   LDY   #$33
   STA   (FP),Y
   LDY   #$24
   LDA   (FP),Y
   LDY   #$2C
   ORA   (FP),Y
   LDY   #$34
   STA   (FP),Y
   LDY   #$25
   LDA   (FP),Y
   LDY   #$2D
   ORA   (FP),Y
   LDY   #$35
   STA   (FP),Y
   LDY   #$26
   LDA   (FP),Y
   LDY   #$2E
   ORA   (FP),Y
   LDY   #$36
   STA   (FP),Y
   LDY   #$27
   LDA   (FP),Y
   LDY   #$2F
   ORA   (FP),Y
   LDY   #$37
   STA   (FP),Y
   LDY   #$28
   LDA   (FP),Y
   LDY   #$30
   ORA   (FP),Y
   LDY   #$38
   STA   (FP),Y
   LDY   #$03
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$31
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$32
   LDA   (FP),Y
   LDY   #$01
   STA   (DPTR),Y
   LDY   #$33
   LDA   (FP),Y
   LDY   #$02
   STA   (DPTR),Y
   LDY   #$34
   LDA   (FP),Y
   LDY   #$03
   STA   (DPTR),Y
   LDY   #$35
   LDA   (FP),Y
   LDY   #$04
   STA   (DPTR),Y
   LDY   #$36
   LDA   (FP),Y
   LDY   #$05
   STA   (DPTR),Y
   LDY   #$37
   LDA   (FP),Y
   LDY   #$06
   STA   (DPTR),Y
   LDY   #$38
   LDA   (FP),Y
   LDY   #$07
   STA   (DPTR),Y
   LDA   ul+7
   BNE   .cmp_true@5
   LDA   ul+6
   BNE   .cmp_true@5
   LDA   ul+5
   BNE   .cmp_true@5
   LDA   ul+4
   BNE   .cmp_true@5
   LDA   ul+3
   CMP   #$FB
   BNE   .cmp_true@5
   LDA   ul+2
   CMP   #$FA
   BNE   .cmp_true@5
   LDA   ul+1
   CMP   #$FF
   BNE   .cmp_true@5
   LDA   ul
   CMP   #$18
   BNE   .cmp_true@5
   LDA   #$00
   JMP   .cmp_end@6
.cmp_true@5:
   LDA   #$01
.cmp_end@6:
   LDY   #$39
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
.if_end@1:
   LDA   #$64
   LDY   #$3B
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDA   #$F0
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   SEC
   SBC   #$10
   INY
   STA   (FP),Y
   LDA   #$00
   SBC   #$0F
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
   LDY   #$41
   LDA   (FP),Y
   LDY   #$49
   STA   (FP),Y
   LDY   #$42
   LDA   (FP),Y
   LDY   #$4A
   STA   (FP),Y
   LDY   #$43
   LDA   (FP),Y
   LDY   #$4B
   STA   (FP),Y
   LDY   #$44
   LDA   (FP),Y
   LDY   #$4C
   STA   (FP),Y
   LDY   #$45
   LDA   (FP),Y
   LDY   #$4D
   STA   (FP),Y
   LDY   #$46
   LDA   (FP),Y
   LDY   #$4E
   STA   (FP),Y
   LDY   #$47
   LDA   (FP),Y
   LDY   #$4F
   STA   (FP),Y
   LDY   #$48
   LDA   (FP),Y
   LDY   #$50
   STA   (FP),Y
   CLC
   LDA   FP
   ADC   #$3D
   INY
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$53
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$54
   STA   (FP),Y
   CLC
   LDA   FP
   ADC   #$49
   INY
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$57
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$58
   STA   (FP),Y
   LDY   #$53
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$59
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$5A
   STA   (FP),Y
   LDY   #$02
   LDA   (DPTR),Y
   LDY   #$5B
   STA   (FP),Y
   LDY   #$03
   LDA   (DPTR),Y
   LDY   #$5C
   STA   (FP),Y
   LDY   #$57
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$5D
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$5E
   STA   (FP),Y
   LDY   #$02
   LDA   (DPTR),Y
   LDY   #$5F
   STA   (FP),Y
   LDY   #$03
   LDA   (DPTR),Y
   LDY   #$60
   STA   (FP),Y
   LDY   #$04
   LDA   (DPTR),Y
   LDY   #$61
   STA   (FP),Y
   LDY   #$05
   LDA   (DPTR),Y
   LDY   #$62
   STA   (FP),Y
   LDY   #$06
   LDA   (DPTR),Y
   LDY   #$63
   STA   (FP),Y
   LDY   #$07
   LDA   (DPTR),Y
   LDY   #$64
   STA   (FP),Y
   LDY   #$59
   LDA   (FP),Y
   LDY   #$65
   STA   (FP),Y
   LDY   #$5A
   LDA   (FP),Y
   LDY   #$66
   STA   (FP),Y
   LDY   #$5B
   LDA   (FP),Y
   LDY   #$67
   STA   (FP),Y
   LDY   #$5C
   LDA   (FP),Y
   LDY   #$68
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
   LDY   #$65
   LDA   (FP),Y
   LDY   #$5D
   EOR   (FP),Y
   LDY   #$6D
   STA   (FP),Y
   LDY   #$66
   LDA   (FP),Y
   LDY   #$5E
   EOR   (FP),Y
   LDY   #$6E
   STA   (FP),Y
   LDY   #$67
   LDA   (FP),Y
   LDY   #$5F
   EOR   (FP),Y
   LDY   #$6F
   STA   (FP),Y
   LDY   #$68
   LDA   (FP),Y
   LDY   #$60
   EOR   (FP),Y
   LDY   #$70
   STA   (FP),Y
   LDY   #$69
   LDA   (FP),Y
   LDY   #$61
   EOR   (FP),Y
   LDY   #$71
   STA   (FP),Y
   LDY   #$6A
   LDA   (FP),Y
   LDY   #$62
   EOR   (FP),Y
   LDY   #$72
   STA   (FP),Y
   LDY   #$6B
   LDA   (FP),Y
   LDY   #$63
   EOR   (FP),Y
   LDY   #$73
   STA   (FP),Y
   LDY   #$6C
   LDA   (FP),Y
   LDY   #$64
   EOR   (FP),Y
   LDY   #$74
   STA   (FP),Y
   LDY   #$6D
   LDA   (FP),Y
   LDY   #$75
   STA   (FP),Y
   LDY   #$6E
   LDA   (FP),Y
   LDY   #$76
   STA   (FP),Y
   LDY   #$6F
   LDA   (FP),Y
   LDY   #$77
   STA   (FP),Y
   LDY   #$70
   LDA   (FP),Y
   LDY   #$78
   STA   (FP),Y
   LDY   #$53
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$75
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$76
   LDA   (FP),Y
   LDY   #$01
   STA   (DPTR),Y
   LDY   #$77
   LDA   (FP),Y
   LDY   #$02
   STA   (DPTR),Y
   LDY   #$78
   LDA   (FP),Y
   LDY   #$03
   STA   (DPTR),Y
   LDY   #$75
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   INY
   ORA   (FP),Y
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
.if_end@2:
   LDY   #$3D
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   INY
   ORA   (FP),Y
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
.if_end@3:
   LDY   #$3B
   LDA   (FP),Y
   CMP   #$64
   BNE   .cmp_true@8
   LDA   #$00
   JMP   .cmp_end@9
.cmp_true@8:
   LDA   #$01
.cmp_end@9:
   LDY   #$79
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
.if_end@4:
   SEC
   LDA   #$00
   SBC   #$10
   LDY   #$7B
   STA   (FP),Y
   LDA   #$00
   SBC   #$0F
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
   LDY   #$82
   CMP   (FP),Y
   BNE   .cmp_true@10
   LDY   #$4F
   LDA   (FP),Y
   LDY   #$81
   CMP   (FP),Y
   BNE   .cmp_true@10
   LDY   #$4E
   LDA   (FP),Y
   LDY   #$80
   CMP   (FP),Y
   BNE   .cmp_true@10
   LDY   #$4D
   LDA   (FP),Y
   LDY   #$7F
   CMP   (FP),Y
   BNE   .cmp_true@10
   LDY   #$4C
   LDA   (FP),Y
   LDY   #$7E
   CMP   (FP),Y
   BNE   .cmp_true@10
   LDY   #$4B
   LDA   (FP),Y
   LDY   #$7D
   CMP   (FP),Y
   BNE   .cmp_true@10
   LDY   #$4A
   LDA   (FP),Y
   LDY   #$7C
   CMP   (FP),Y
   BNE   .cmp_true@10
   LDY   #$49
   LDA   (FP),Y
   LDY   #$7B
   CMP   (FP),Y
   BNE   .cmp_true@10
   LDA   #$00
   JMP   .cmp_end@11
.cmp_true@10:
   LDA   #$01
.cmp_end@11:
   LDY   #$83
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
   ADC   #$86
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
.if_end@5:
   LDX   #$00
   TXA

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

ul:
   DC.L  $FBFAF9F8
   DC.L  $00000000
