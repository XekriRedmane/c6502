SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24

check:
   SUBROUTINE

   ; prologue: 12 arg bytes, 141 local bytes
   SEC
   LDA   SSP
   SBC   #$8F
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$8E
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
   INY
   STA   (FP),Y
   LDY   #$97
   LDA   (FP),Y
   LDY   #$03
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$05
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$06
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@0
   LDA   #$00
   JMP   .sx_done@1
.sx_neg@0:
   LDA   #$FF
.sx_done@1:
   LDY   #$07
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$98
   LDA   (FP),Y
   SEC
   LDY   #$05
   SBC   (FP),Y
   LDY   #$09
   STA   (FP),Y
   LDY   #$99
   LDA   (FP),Y
   LDY   #$06
   SBC   (FP),Y
   LDY   #$0A
   STA   (FP),Y
   LDY   #$9A
   LDA   (FP),Y
   LDY   #$07
   SBC   (FP),Y
   LDY   #$0B
   STA   (FP),Y
   LDY   #$9B
   LDA   (FP),Y
   LDY   #$08
   SBC   (FP),Y
   LDY   #$0C
   STA   (FP),Y
   LDY   #$09
   LDA   (FP),Y
   LDY   #$0D
   STA   (FP),Y
   LDY   #$0A
   LDA   (FP),Y
   LDY   #$0E
   STA   (FP),Y
   LDY   #$0B
   LDA   (FP),Y
   LDY   #$0F
   STA   (FP),Y
   LDY   #$0C
   LDA   (FP),Y
   LDY   #$10
   STA   (FP),Y
   CMP   #$CB
   BNE   .cmp_true@2
   DEY
   LDA   (FP),Y
   CMP   #$0A
   BNE   .cmp_true@2
   DEY
   LDA   (FP),Y
   CMP   #$7C
   BNE   .cmp_true@2
   DEY
   LDA   (FP),Y
   CMP   #$F9
   BNE   .cmp_true@2
   LDA   #$00
   JMP   .cmp_end@3
.cmp_true@2:
   LDA   #$01
.cmp_end@3:
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
   LDY   #$01
   LDA   (FP),Y
   ORA   #$01
   LDY   #$13
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   LDY   #$14
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   LDY   #$14
   LDA   (FP),Y
   LDY   #$02
   STA   (FP),Y
.if_end@0:
   LDY   #$92
   LDA   (FP),Y
   LDY   #$15
   STA   (FP),Y
   LDY   #$93
   LDA   (FP),Y
   LDY   #$16
   STA   (FP),Y
   LDY   #$94
   LDA   (FP),Y
   STA   HARGS
   INY
   LDA   (FP),Y
   STA   HARGS+1
   LDY   #$15
   LDA   (FP),Y
   STA   HARGS+2
   INY
   LDA   (FP),Y
   STA   HARGS+3
   JSR   udivmod16
   LDA   HARGS+6
   LDY   #$17
   STA   (FP),Y
   LDA   HARGS+7
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$19
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$1A
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$19
   LDA   (FP),Y
   LDY   #$1D
   STA   (FP),Y
   LDY   #$1A
   LDA   (FP),Y
   LDY   #$1E
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$1F
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$20
   STA   (FP),Y
   LDA   #$A0
   INY
   STA   (FP),Y
   LDA   #$E2
   INY
   STA   (FP),Y
   LDY   #$20
   LDA   (FP),Y
   LDY   #$22
   CMP   (FP),Y
   BNE   .cmp_true@5
   LDY   #$1F
   LDA   (FP),Y
   LDY   #$21
   CMP   (FP),Y
   BNE   .cmp_true@5
   LDA   #$00
   JMP   .cmp_end@6
.cmp_true@5:
   LDA   #$01
.cmp_end@6:
   LDY   #$23
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@1
   LDY   #$01
   LDA   (FP),Y
   ORA   #$02
   LDY   #$25
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   LDY   #$26
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   LDY   #$26
   LDA   (FP),Y
   LDY   #$02
   STA   (FP),Y
.if_end@1:
   LDY   #$96
   LDA   (FP),Y
   LDY   #$27
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@8
   LDA   #$00
   JMP   .sx_done@9
.sx_neg@8:
   LDA   #$FF
.sx_done@9:
   LDY   #$28
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   EOR   #$FF
   LDY   #$29
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   EOR   #$FF
   LDY   #$2A
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   EOR   #$FF
   LDY   #$2B
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   EOR   #$FF
   LDY   #$2C
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   SEC
   LDY   #$90
   SBC   (FP),Y
   LDY   #$2D
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$91
   SBC   (FP),Y
   LDY   #$2E
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$2F
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$30
   STA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@10
   DEY
   LDA   (FP),Y
   CMP   #$04
   BNE   .cmp_true@10
   LDA   #$00
   JMP   .cmp_end@11
.cmp_true@10:
   LDA   #$01
.cmp_end@11:
   LDY   #$31
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@2
   LDY   #$01
   LDA   (FP),Y
   ORA   #$04
   LDY   #$33
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   LDY   #$34
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   LDY   #$34
   LDA   (FP),Y
   LDY   #$02
   STA   (FP),Y
.if_end@2:
   LDA   #$00
   LDY   #$35
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@13
   LDA   #$00
   JMP   .sx_done@14
.sx_neg@13:
   LDA   #$FF
.sx_done@14:
   LDY   #$38
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@15
   LDA   #$00
   JMP   .sx_done@16
.sx_neg@15:
   LDA   #$FF
.sx_done@16:
   LDY   #$3B
   STA   (FP),Y
   LDY   #$38
   LDA   (FP),Y
   LDY   #$3B
   CMP   (FP),Y
   BNE   .cmp_true@17
   LDY   #$37
   LDA   (FP),Y
   LDY   #$3A
   CMP   (FP),Y
   BNE   .cmp_true@17
   LDA   #$00
   JMP   .cmp_end@18
.cmp_true@17:
   LDA   #$01
.cmp_end@18:
   LDY   #$3C
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@3
   LDY   #$01
   LDA   (FP),Y
   ORA   #$08
   LDY   #$3E
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   LDY   #$3F
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   LDY   #$3F
   LDA   (FP),Y
   LDY   #$02
   STA   (FP),Y
.if_end@3:
   LDA   #$01
   LDY   #$40
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CMP   #$01
   BNE   .cmp_true@20
   LDA   #$00
   JMP   .cmp_end@21
.cmp_true@20:
   LDA   #$01
.cmp_end@21:
   LDY   #$42
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@4
   LDY   #$01
   LDA   (FP),Y
   ORA   #$10
   LDY   #$44
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   LDY   #$45
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   LDY   #$45
   LDA   (FP),Y
   LDY   #$02
   STA   (FP),Y
.if_end@4:
   LDY   #$90
   LDA   (FP),Y
   LDY   #$46
   STA   (FP),Y
   LDY   #$91
   LDA   (FP),Y
   LDY   #$47
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@22
   LDA   #$00
   JMP   .sx_done@23
.sx_neg@22:
   LDA   #$FF
.sx_done@23:
   LDY   #$48
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$98
   LDA   (FP),Y
   LDY   #$46
   AND   (FP),Y
   LDY   #$4A
   STA   (FP),Y
   LDY   #$99
   LDA   (FP),Y
   LDY   #$47
   AND   (FP),Y
   LDY   #$4B
   STA   (FP),Y
   LDY   #$9A
   LDA   (FP),Y
   LDY   #$48
   AND   (FP),Y
   LDY   #$4C
   STA   (FP),Y
   LDY   #$9B
   LDA   (FP),Y
   LDY   #$49
   AND   (FP),Y
   LDY   #$4D
   STA   (FP),Y
   LDY   #$4A
   LDA   (FP),Y
   LDY   #$4E
   STA   (FP),Y
   LDY   #$4B
   LDA   (FP),Y
   LDY   #$4F
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
   BMI   .sx_neg@24
   LDA   #$00
   JMP   .sx_done@25
.sx_neg@24:
   LDA   #$FF
.sx_done@25:
   LDY   #$52
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$50
   LDA   (FP),Y
   LDY   #$54
   STA   (FP),Y
   LDY   #$51
   LDA   (FP),Y
   LDY   #$55
   STA   (FP),Y
   LDY   #$52
   LDA   (FP),Y
   LDY   #$56
   STA   (FP),Y
   INY
   STA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@26
   LDY   #$55
   LDA   (FP),Y
   BNE   .cmp_true@26
   DEY
   LDA   (FP),Y
   CMP   #$09
   BNE   .cmp_true@26
   LDA   #$00
   JMP   .cmp_end@27
.cmp_true@26:
   LDA   #$01
.cmp_end@27:
   LDY   #$58
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@5
   LDY   #$01
   LDA   (FP),Y
   ORA   #$20
   LDY   #$5A
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   LDY   #$5B
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   LDY   #$5B
   LDA   (FP),Y
   LDY   #$02
   STA   (FP),Y
.if_end@5:
   LDY   #$98
   LDA   (FP),Y
   LDY   #$5C
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
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
   DEY
   LDA   (FP),Y
   STA   HARGS
   LDA   #$00
   LDX   #$08
   LSR   HARGS
.mul_loop@29:
   BCC   .mul_skip@30
   CLC
   ADC   #$80
.mul_skip@30:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@29
   LDY   #$62
   STA   (FP),Y
   LDA   HARGS
   DEY
   STA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@1
   JMP   .and_false@6
.lb_skip@1:
   LDY   #$96
   LDA   (FP),Y
   LDY   #$63
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@31
   LDA   #$00
   JMP   .sx_done@32
.sx_neg@31:
   LDA   #$FF
.sx_done@32:
   LDY   #$64
   STA   (FP),Y
   LDY   #$91
   LDA   (FP),Y
   LDY   #$64
   CMP   (FP),Y
   BNE   .cmp_true@33
   LDY   #$90
   LDA   (FP),Y
   LDY   #$63
   CMP   (FP),Y
   BNE   .cmp_true@33
   LDA   #$00
   JMP   .cmp_end@34
.cmp_true@33:
   LDA   #$01
.cmp_end@34:
   LDY   #$65
   STA   (FP),Y
   LDA   #$00
   INY
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
   BMI   .sx_neg@36
   LDA   #$00
   JMP   .sx_done@37
.sx_neg@36:
   LDA   #$FF
.sx_done@37:
   LDY   #$69
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$98
   LDA   (FP),Y
   LDY   #$67
   ORA   (FP),Y
   LDY   #$6B
   STA   (FP),Y
   LDY   #$99
   LDA   (FP),Y
   LDY   #$68
   ORA   (FP),Y
   LDY   #$6C
   STA   (FP),Y
   LDY   #$9A
   LDA   (FP),Y
   LDY   #$69
   ORA   (FP),Y
   LDY   #$6D
   STA   (FP),Y
   LDY   #$9B
   LDA   (FP),Y
   LDY   #$6A
   ORA   (FP),Y
   LDY   #$6E
   STA   (FP),Y
   LDY   #$6B
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   INY
   ORA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@6
   LDA   #$01
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   JMP   .and_end@7
.and_false@6:
   LDA   #$00
   LDY   #$6F
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@7:
   LDY   #$6F
   LDA   (FP),Y
   LDY   #$71
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$72
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CMP   #$01
   BNE   .cmp_true@38
   LDA   #$00
   JMP   .cmp_end@39
.cmp_true@38:
   LDA   #$01
.cmp_end@39:
   LDY   #$73
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@8
   LDY   #$01
   LDA   (FP),Y
   ORA   #$40
   LDY   #$75
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   LDY   #$76
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   LDY   #$76
   LDA   (FP),Y
   LDY   #$02
   STA   (FP),Y
.if_end@8:
   LDY   #$97
   LDA   (FP),Y
   LDY   #$77
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@0
   JMP   .or_true@9
.lb_skip@0:
   LDA   #$07
   LDY   #$79
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$7C
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$7D
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@40
   LDA   #$00
   JMP   .sx_done@41
.sx_neg@40:
   LDA   #$FF
.sx_done@41:
   LDY   #$7E
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$FF
   SEC
   LDY   #$7C
   SBC   (FP),Y
   LDA   #$7F
   INY
   SBC   (FP),Y
   LDA   #$00
   INY
   SBC   (FP),Y
   LDA   #$00
   INY
   SBC   (FP),Y
   LDA   #$00
   ROL
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$82
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$83
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@42
   LDA   #$00
   JMP   .sx_done@43
.sx_neg@42:
   LDA   #$FF
.sx_done@43:
   LDY   #$84
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$82
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   INY
   ORA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@9
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   JMP   .or_end@10
.or_true@9:
   LDA   #$01
   LDY   #$86
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@10:
   LDY   #$86
   LDA   (FP),Y
   LDY   #$88
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$89
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CMP   #$01
   BNE   .cmp_true@44
   LDA   #$00
   JMP   .cmp_end@45
.cmp_true@44:
   LDA   #$01
.cmp_end@45:
   LDY   #$8A
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@11
   LDY   #$01
   LDA   (FP),Y
   ORA   #$80
   LDY   #$8C
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   LDY   #$8D
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   LDY   #$8D
   LDA   (FP),Y
   LDY   #$02
   STA   (FP),Y
.if_end@11:
   LDY   #$02
   LDA   (FP),Y
   TAX
   DEY
   LDA   (FP),Y

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$9B
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$8E
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

   ; prologue: 0 arg bytes, 6 local bytes
   SEC
   LDA   SSP
   SBC   #$08
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$07
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   LDA   #$FF
   LDY   #$01
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$0D
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$0C
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   #$09
   LDY   #$01
   STA   (SSP),Y
   LDA   #$00
   INY
   STA   (SSP),Y
   DEY
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   DEY
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   LDA   #$A0
   INY
   STA   (SSP),Y
   LDA   #$E2
   INY
   STA   (SSP),Y
   LDY   #$03
   LDA   (FP),Y
   LDY   #$07
   STA   (SSP),Y
   LDY   #$04
   LDA   (FP),Y
   LDY   #$08
   STA   (SSP),Y
   LDA   #$F9
   INY
   STA   (SSP),Y
   LDA   #$7C
   INY
   STA   (SSP),Y
   LDA   #$0A
   INY
   STA   (SSP),Y
   LDA   #$CB
   INY
   STA   (SSP),Y
   JSR   check
   LDY   #$05
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
   ADC   #$08
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$07
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
