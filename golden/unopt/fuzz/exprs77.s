SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24

check:
   SUBROUTINE

   ; prologue: 0 arg bytes, 136 local bytes
   SEC
   LDA   SSP
   SBC   #$8A
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$89
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
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$80
   INY
   STA   (FP),Y
   CMP   #$80
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
   LDY   #$07
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
   LDY   #$09
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   LDY   #$0A
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   LDY   #$0A
   LDA   (FP),Y
   LDY   #$02
   STA   (FP),Y
.if_end@0:
   LDA   v1
   LDY   #$0B
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$0D
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$0E
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@3
   LDA   #$00
   JMP   .sx_done@4
.sx_neg@3:
   LDA   #$FF
.sx_done@4:
   LDY   #$0F
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   v5
   INY
   STA   (FP),Y
   LDA   v5+1
   INY
   STA   (FP),Y
   LDA   v5+1
   BMI   .sx_neg@5
   LDA   #$00
   JMP   .sx_done@6
.sx_neg@5:
   LDA   #$FF
.sx_done@6:
   LDY   #$13
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   v4
   LDY   #$11
   ORA   (FP),Y
   LDY   #$15
   STA   (FP),Y
   LDA   v4+1
   LDY   #$12
   ORA   (FP),Y
   LDY   #$16
   STA   (FP),Y
   LDA   v4+2
   LDY   #$13
   ORA   (FP),Y
   LDY   #$17
   STA   (FP),Y
   LDA   v4+3
   LDY   #$14
   ORA   (FP),Y
   LDY   #$18
   STA   (FP),Y
   LDY   #$15
   LDA   (FP),Y
   CLC
   LDY   #$0D
   SBC   (FP),Y
   LDY   #$16
   LDA   (FP),Y
   LDY   #$0E
   SBC   (FP),Y
   LDY   #$17
   LDA   (FP),Y
   LDY   #$0F
   SBC   (FP),Y
   LDY   #$18
   LDA   (FP),Y
   LDY   #$10
   SBC   (FP),Y
   LDA   #$00
   ROL
   LDY   #$19
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDA   v2
   STA   HARGS
   LDA   v2+1
   STA   HARGS+1
   DEY
   LDA   (FP),Y
   STA   HARGS+2
   INY
   LDA   (FP),Y
   STA   HARGS+3
   JSR   mul16
   LDA   HARGS+4
   LDY   #$1B
   STA   (FP),Y
   LDA   HARGS+5
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$1D
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$1E
   STA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@7
   DEY
   LDA   (FP),Y
   CMP   #$01
   BNE   .cmp_true@7
   LDA   #$00
   JMP   .cmp_end@8
.cmp_true@7:
   LDA   #$01
.cmp_end@8:
   LDY   #$1F
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
   LDY   #$21
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   LDY   #$22
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   LDY   #$22
   LDA   (FP),Y
   LDY   #$02
   STA   (FP),Y
.if_end@1:
   LDA   v0
   LDY   #$23
   STA   (FP),Y
   LDA   v0+1
   INY
   STA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@10
   DEY
   LDA   (FP),Y
   CMP   #$03
   BNE   .cmp_true@10
   LDA   #$00
   JMP   .cmp_end@11
.cmp_true@10:
   LDA   #$01
.cmp_end@11:
   LDY   #$25
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .vthread@0
   LDY   #$01
   LDA   (FP),Y
   ORA   #$04
   LDY   #$27
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   LDY   #$28
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   LDY   #$28
   LDA   (FP),Y
   LDY   #$02
   STA   (FP),Y
   LDA   #$FF
   BEQ   .and_false@3
.vthread@0:
   LDA   v3
   ORA   v3+1
   ORA   v3+2
   ORA   v3+3
   BEQ   .and_false@3
   LDA   #$01
   LDY   #$29
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   JMP   .and_end@4
.and_false@3:
   LDA   #$00
   LDY   #$29
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@4:
   LDY   #$29
   LDA   (FP),Y
   LDY   #$2B
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$2C
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@13
   LDA   #$00
   JMP   .sx_done@14
.sx_neg@13:
   LDA   #$FF
.sx_done@14:
   LDY   #$2D
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$2B
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
   LDA   v3
   STA   HARGS+4
   LDA   v3+1
   STA   HARGS+5
   LDA   v3+2
   STA   HARGS+6
   LDA   v3+3
   STA   HARGS+7
   JSR   sdivmod32
   LDA   HARGS+12
   LDY   #$2F
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
   LDY   #$2F
   LDA   (FP),Y
   LDY   #$33
   STA   (FP),Y
   LDY   #$30
   LDA   (FP),Y
   LDY   #$34
   STA   (FP),Y
   LDY   #$31
   LDA   (FP),Y
   LDY   #$35
   STA   (FP),Y
   LDY   #$32
   LDA   (FP),Y
   LDY   #$36
   STA   (FP),Y
   LDA   v0
   INY
   STA   (FP),Y
   LDA   v0+1
   INY
   STA   (FP),Y
   LDA   v0+1
   BMI   .sx_neg@15
   LDA   #$00
   JMP   .sx_done@16
.sx_neg@15:
   LDA   #$FF
.sx_done@16:
   LDY   #$39
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   v4+3
   BNE   .cmp_true@17
   LDA   v4+2
   BNE   .cmp_true@17
   LDA   v4+1
   BNE   .cmp_true@17
   LDA   v4
   BNE   .cmp_true@17
   LDA   #$00
   JMP   .cmp_end@18
.cmp_true@17:
   LDA   #$01
.cmp_end@18:
   LDY   #$3B
   STA   (FP),Y
   LDA   #$00
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
   BMI   .sx_neg@20
   LDA   #$00
   JMP   .sx_done@21
.sx_neg@20:
   LDA   #$FF
.sx_done@21:
   LDY   #$3F
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$37
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
   LDY   #$3D
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
   JSR   udivmod32
   LDA   HARGS+8
   LDY   #$41
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
   LDY   #$33
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
   LDY   #$41
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
   JSR   udivmod32
   LDA   HARGS+12
   LDY   #$45
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
   LDY   #$45
   LDA   (FP),Y
   LDY   #$49
   STA   (FP),Y
   LDY   #$46
   LDA   (FP),Y
   LDY   #$4A
   STA   (FP),Y
   LDY   #$47
   LDA   (FP),Y
   LDY   #$4B
   STA   (FP),Y
   LDY   #$48
   LDA   (FP),Y
   LDY   #$4C
   STA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@22
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@22
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@22
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@22
   LDA   #$00
   JMP   .cmp_end@23
.cmp_true@22:
   LDA   #$01
.cmp_end@23:
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
   LDY   #$01
   LDA   (FP),Y
   ORA   #$08
   LDY   #$4F
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   LDY   #$50
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   LDY   #$50
   LDA   (FP),Y
   LDY   #$02
   STA   (FP),Y
.if_end@5:
   LDA   v0
   LDY   #$51
   STA   (FP),Y
   LDA   v0+1
   INY
   STA   (FP),Y
   LDA   v0+1
   BMI   .sx_neg@25
   LDA   #$00
   JMP   .sx_done@26
.sx_neg@25:
   LDA   #$FF
.sx_done@26:
   LDY   #$53
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$FF
   SEC
   LDY   #$51
   SBC   (FP),Y
   LDA   #$FF
   INY
   SBC   (FP),Y
   LDA   #$FF
   INY
   SBC   (FP),Y
   LDA   #$7F
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
   INY
   ORA   (FP),Y
   BEQ   .lnot_true@27
   LDA   #$00
   JMP   .lnot_end@28
.lnot_true@27:
   LDA   #$01
.lnot_end@28:
   LDY   #$57
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$59
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$5A
   STA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@29
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@29
   LDA   #$00
   JMP   .cmp_end@30
.cmp_true@29:
   LDA   #$01
.cmp_end@30:
   LDY   #$5B
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@6
   LDY   #$01
   LDA   (FP),Y
   ORA   #$10
   LDY   #$5D
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   LDY   #$5E
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   LDY   #$5E
   LDA   (FP),Y
   LDY   #$02
   STA   (FP),Y
.if_end@6:
   LDA   v2
   LDY   #$5F
   STA   (FP),Y
   LDA   v2+1
   INY
   STA   (FP),Y
   LDA   v2+1
   BMI   .sx_neg@32
   LDA   #$00
   JMP   .sx_done@33
.sx_neg@32:
   LDA   #$FF
.sx_done@33:
   LDY   #$61
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$5F
   LDA   (FP),Y
   LDY   #$63
   STA   (FP),Y
   LDY   #$60
   LDA   (FP),Y
   LDY   #$64
   STA   (FP),Y
   LDY   #$61
   LDA   (FP),Y
   LDY   #$65
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   v0
   INY
   STA   (FP),Y
   LDA   v0+1
   INY
   STA   (FP),Y
   LDA   v0+1
   BMI   .sx_neg@34
   LDA   #$00
   JMP   .sx_done@35
.sx_neg@34:
   LDA   #$FF
.sx_done@35:
   LDY   #$69
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   v4
   CLC
   LDY   #$67
   ADC   (FP),Y
   LDY   #$6B
   STA   (FP),Y
   LDA   v4+1
   LDY   #$68
   ADC   (FP),Y
   LDY   #$6C
   STA   (FP),Y
   LDA   v4+2
   LDY   #$69
   ADC   (FP),Y
   LDY   #$6D
   STA   (FP),Y
   LDA   v4+3
   LDY   #$6A
   ADC   (FP),Y
   LDY   #$6E
   STA   (FP),Y
   LDY   #$63
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
   LDY   #$6B
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
   JSR   udivmod32
   LDA   HARGS+12
   LDY   #$6F
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
   LDY   #$6F
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   INY
   ORA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@7
   LDA   v0
   ORA   v0+1
   BEQ   .and_false@7
   LDA   #$01
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   JMP   .and_end@8
.and_false@7:
   LDA   #$00
   LDY   #$73
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@8:
   LDY   #$73
   LDA   (FP),Y
   LDY   #$75
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$76
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CMP   #$01
   BNE   .cmp_true@36
   LDA   #$00
   JMP   .cmp_end@37
.cmp_true@36:
   LDA   #$01
.cmp_end@37:
   LDY   #$77
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@9
   LDY   #$01
   LDA   (FP),Y
   ORA   #$20
   LDY   #$79
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   LDY   #$7A
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   LDY   #$7A
   LDA   (FP),Y
   LDY   #$02
   STA   (FP),Y
.if_end@9:
   LDA   v3
   LDY   #$7B
   STA   (FP),Y
   LDA   v3+1
   INY
   STA   (FP),Y
   LDA   v3+2
   INY
   STA   (FP),Y
   LDA   v3+3
   INY
   STA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@38
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@38
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@38
   DEY
   LDA   (FP),Y
   CMP   #$01
   BNE   .cmp_true@38
   LDA   #$00
   JMP   .cmp_end@39
.cmp_true@38:
   LDA   #$01
.cmp_end@39:
   LDY   #$7F
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@10
   LDY   #$01
   LDA   (FP),Y
   ORA   #$40
   LDY   #$81
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   LDY   #$82
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   LDY   #$82
   LDA   (FP),Y
   LDY   #$02
   STA   (FP),Y
.if_end@10:
   LDA   #$00
   LDY   #$83
   STA   (FP),Y
   LDA   #$01
   INY
   STA   (FP),Y
   CMP   #$01
   BNE   .cmp_true@41
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@41
   LDA   #$00
   JMP   .cmp_end@42
.cmp_true@41:
   LDA   #$01
.cmp_end@42:
   LDY   #$85
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
   LDY   #$87
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   LDY   #$88
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   LDY   #$88
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
   ADC   #$8A
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$89
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

   ; prologue: 0 arg bytes, 2 local bytes
   SEC
   LDA   SSP
   SBC   #$04
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$03
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   JSR   check
   LDY   #$01
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
   ADC   #$04
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$03
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

v0:
   DC.W  $0003

v1:
   DC.B  $D2

v2:
   DC.W  $0001

v3:
   DC.L  $00000001

v4:
   DC.L  $A93F32A9

v5:
   DC.W  $FFFF
