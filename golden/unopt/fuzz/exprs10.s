SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24

check:
   SUBROUTINE

   ; prologue: 0 arg bytes, 231 local bytes
   SEC
   LDA   SSP
   SBC   #$E9
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$E8
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
   LDA   v3
   INY
   STA   (FP),Y
   LDA   v3
   BMI   .sx_neg@0
   LDA   #$00
   JMP   .sx_done@1
.sx_neg@0:
   LDA   #$FF
.sx_done@1:
   LDY   #$04
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
   BMI   .sx_neg@2
   LDA   #$00
   JMP   .sx_done@3
.sx_neg@2:
   LDA   #$FF
.sx_done@3:
   LDY   #$07
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   v1
   INY
   STA   (FP),Y
   LDA   v1
   BMI   .sx_neg@4
   LDA   #$00
   JMP   .sx_done@5
.sx_neg@4:
   LDA   #$FF
.sx_done@5:
   LDY   #$0A
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$0B
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$0C
   STA   (FP),Y
   CMP   #$00
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
   LDY   #$0F
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$10
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@9
   LDA   #$00
   JMP   .sx_done@10
.sx_neg@9:
   LDA   #$FF
.sx_done@10:
   LDY   #$11
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$05
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
   LDY   #$0F
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
   JSR   mul32
   LDA   HARGS+8
   LDY   #$13
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
   LDY   #$13
   LDA   (FP),Y
   LDY   #$17
   STA   (FP),Y
   LDY   #$14
   LDA   (FP),Y
   LDY   #$18
   STA   (FP),Y
   LDY   #$15
   LDA   (FP),Y
   LDY   #$19
   STA   (FP),Y
   LDY   #$16
   LDA   (FP),Y
   LDY   #$1A
   STA   (FP),Y
   CMP   #$FF
   BNE   .cmp_true@11
   DEY
   LDA   (FP),Y
   CMP   #$FF
   BNE   .cmp_true@11
   DEY
   LDA   (FP),Y
   CMP   #$FF
   BNE   .cmp_true@11
   DEY
   LDA   (FP),Y
   CMP   #$80
   BNE   .cmp_true@11
   LDA   #$00
   JMP   .cmp_end@12
.cmp_true@11:
   LDA   #$01
.cmp_end@12:
   LDY   #$1B
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
   LDY   #$1D
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   LDY   #$1E
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   LDY   #$1E
   LDA   (FP),Y
   LDY   #$02
   STA   (FP),Y
.if_end@0:
   LDA   v1
   LDY   #$1F
   STA   (FP),Y
   LDA   v1
   BMI   .sx_neg@14
   LDA   #$00
   JMP   .sx_done@15
.sx_neg@14:
   LDA   #$FF
.sx_done@15:
   LDY   #$20
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$21
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$22
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@16
   LDA   #$00
   JMP   .sx_done@17
.sx_neg@16:
   LDA   #$FF
.sx_done@17:
   LDY   #$23
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
   LDY   #$28
   STA   (FP),Y
   LDY   #$22
   LDA   (FP),Y
   LDY   #$29
   STA   (FP),Y
   LDY   #$23
   LDA   (FP),Y
   LDY   #$2A
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   AND   #$1F
   INY
   STA   (FP),Y
   LDY   #$28
   LDA   (FP),Y
   LDY   #$2F
   STA   (FP),Y
   LDY   #$29
   LDA   (FP),Y
   EOR   #$01
   LDY   #$30
   STA   (FP),Y
   LDY   #$2A
   LDA   (FP),Y
   LDY   #$31
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$2E
   LDA   (FP),Y
   LDY   #$35
   STA   (FP),Y
   LDY   #$2F
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   INY
   ORA   (FP),Y
   INY
   ORA   (FP),Y
   INY
   ORA   (FP),Y
   INY
   ORA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lnot_true@18
   LDA   #$00
   JMP   .lnot_end@19
.lnot_true@18:
   LDA   #$01
.lnot_end@19:
   LDY   #$36
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDA   v2
   STA   HARGS
   LDA   v2+1
   STA   HARGS+1
   LDA   v2+2
   STA   HARGS+2
   LDA   v2+3
   STA   HARGS+3
   DEY
   LDA   (FP),Y
   STA   HARGS+4
   JSR   asl32
   LDA   HARGS+5
   LDY   #$38
   STA   (FP),Y
   LDA   HARGS+6
   INY
   STA   (FP),Y
   LDA   HARGS+7
   INY
   STA   (FP),Y
   LDA   HARGS+8
   INY
   STA   (FP),Y
   LDY   #$38
   LDA   (FP),Y
   LDY   #$3C
   STA   (FP),Y
   LDY   #$39
   LDA   (FP),Y
   LDY   #$3D
   STA   (FP),Y
   LDY   #$3A
   LDA   (FP),Y
   LDY   #$3E
   STA   (FP),Y
   LDY   #$3B
   LDA   (FP),Y
   LDY   #$3F
   STA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@20
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@20
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@20
   DEY
   LDA   (FP),Y
   CMP   #$0C
   BNE   .cmp_true@20
   LDA   #$00
   JMP   .cmp_end@21
.cmp_true@20:
   LDA   #$01
.cmp_end@21:
   LDY   #$40
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
   LDY   #$42
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   LDY   #$43
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   LDY   #$43
   LDA   (FP),Y
   LDY   #$02
   STA   (FP),Y
.if_end@1:
   LDA   v0
   LDY   #$44
   STA   (FP),Y
   INY
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@23
   LDA   #$00
   JMP   .sx_done@24
.sx_neg@23:
   LDA   #$FF
.sx_done@24:
   LDY   #$46
   STA   (FP),Y
   LDA   #$01
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@25
   LDA   #$00
   JMP   .sx_done@26
.sx_neg@25:
   LDA   #$FF
.sx_done@26:
   LDY   #$49
   STA   (FP),Y
   LDY   #$46
   LDA   (FP),Y
   LDY   #$49
   CMP   (FP),Y
   BNE   .cmp_true@27
   LDY   #$45
   LDA   (FP),Y
   LDY   #$48
   CMP   (FP),Y
   BNE   .cmp_true@27
   LDA   #$00
   JMP   .cmp_end@28
.cmp_true@27:
   LDA   #$01
.cmp_end@28:
   LDY   #$4A
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
   LDY   #$4C
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   LDY   #$4D
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   LDY   #$4D
   LDA   (FP),Y
   LDY   #$02
   STA   (FP),Y
.if_end@2:
   LDA   v0
   LDY   #$4E
   STA   (FP),Y
   LDA   v0
   BMI   .sx_neg@30
   LDA   #$00
   JMP   .sx_done@31
.sx_neg@30:
   LDA   #$FF
.sx_done@31:
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
   BMI   .sx_neg@32
   LDA   #$00
   JMP   .sx_done@33
.sx_neg@32:
   LDA   #$FF
.sx_done@33:
   LDY   #$52
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$50
   LDA   (FP),Y
   EOR   v2
   LDY   #$54
   STA   (FP),Y
   LDY   #$51
   LDA   (FP),Y
   EOR   v2+1
   LDY   #$55
   STA   (FP),Y
   LDY   #$52
   LDA   (FP),Y
   EOR   v2+2
   LDY   #$56
   STA   (FP),Y
   LDY   #$53
   LDA   (FP),Y
   EOR   v2+3
   LDY   #$57
   STA   (FP),Y
   LDA   v2
   STA   HARGS
   LDA   v2+1
   STA   HARGS+1
   LDA   v2+2
   STA   HARGS+2
   LDA   v2+3
   STA   HARGS+3
   LDY   #$54
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
   JSR   mul32
   LDA   HARGS+8
   LDY   #$58
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
   LDA   v3
   INY
   STA   (FP),Y
   LDA   v3
   BMI   .sx_neg@34
   LDA   #$00
   JMP   .sx_done@35
.sx_neg@34:
   LDA   #$FF
.sx_done@35:
   LDY   #$5D
   STA   (FP),Y
   LDA   v0
   INY
   STA   (FP),Y
   LDA   v0
   BMI   .sx_neg@36
   LDA   #$00
   JMP   .sx_done@37
.sx_neg@36:
   LDA   #$FF
.sx_done@37:
   LDY   #$5F
   STA   (FP),Y
   LDY   #$5C
   LDA   (FP),Y
   LDY   #$5E
   AND   (FP),Y
   LDY   #$60
   STA   (FP),Y
   LDY   #$5D
   LDA   (FP),Y
   LDY   #$5F
   AND   (FP),Y
   LDY   #$61
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$62
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$63
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@38
   LDA   #$00
   JMP   .sx_done@39
.sx_neg@38:
   LDA   #$FF
.sx_done@39:
   LDY   #$64
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   CMP   (FP),Y
   BNE   .cmp_true@40
   DEY
   CMP   (FP),Y
   BNE   .cmp_true@40
   DEY
   CMP   (FP),Y
   BNE   .cmp_true@40
   DEY
   CMP   (FP),Y
   BNE   .cmp_true@40
   DEY
   CMP   (FP),Y
   BNE   .cmp_true@40
   DEY
   CMP   (FP),Y
   BNE   .cmp_true@40
   LDA   #$07
   DEY
   CMP   (FP),Y
   BNE   .cmp_true@40
   LDA   #$00
   JMP   .cmp_end@41
.cmp_true@40:
   LDA   #$01
.cmp_end@41:
   LDY   #$69
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$6B
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$6C
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@43
   LDA   #$00
   JMP   .sx_done@44
.sx_neg@43:
   LDA   #$FF
.sx_done@44:
   LDY   #$6D
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$5B
   LDA   (FP),Y
   LDY   #$6E
   CMP   (FP),Y
   BNE   .thread@0
   LDY   #$5A
   LDA   (FP),Y
   LDY   #$6D
   CMP   (FP),Y
   BNE   .thread@0
   LDY   #$59
   LDA   (FP),Y
   LDY   #$6C
   CMP   (FP),Y
   BNE   .thread@0
   LDY   #$58
   LDA   (FP),Y
   LDY   #$6B
   CMP   (FP),Y
   BEQ   .cmp_true@45
.thread@0:
   LDA   #$00
   JMP   .cmp_end@46
.cmp_true@45:
   LDA   #$01
.cmp_end@46:
   LDY   #$6F
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$71
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$72
   STA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@48
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@48
   LDA   #$00
   JMP   .cmp_end@49
.cmp_true@48:
   LDA   #$01
.cmp_end@49:
   LDY   #$73
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
.if_end@3:
   LDA   v0
   LDY   #$77
   STA   (FP),Y
   LDA   v0
   BMI   .sx_neg@51
   LDA   #$00
   JMP   .sx_done@52
.sx_neg@51:
   LDA   #$FF
.sx_done@52:
   LDY   #$78
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$79
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$7A
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@53
   LDA   #$00
   JMP   .sx_done@54
.sx_neg@53:
   LDA   #$FF
.sx_done@54:
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
   LDY   #$79
   LDA   (FP),Y
   LDY   #$80
   STA   (FP),Y
   LDY   #$7A
   LDA   (FP),Y
   LDY   #$81
   STA   (FP),Y
   LDY   #$7B
   LDA   (FP),Y
   LDY   #$82
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   AND   #$1F
   INY
   STA   (FP),Y
   LDY   #$80
   LDA   (FP),Y
   LDY   #$87
   STA   (FP),Y
   LDY   #$81
   LDA   (FP),Y
   LDY   #$88
   STA   (FP),Y
   LDY   #$82
   LDA   (FP),Y
   LDY   #$89
   STA   (FP),Y
   CLC
   ADC   #$80
   INY
   STA   (FP),Y
   LDY   #$84
   LDA   (FP),Y
   ADC   #$00
   LDY   #$8B
   STA   (FP),Y
   LDY   #$85
   LDA   (FP),Y
   ADC   #$00
   LDY   #$8C
   STA   (FP),Y
   LDY   #$86
   LDA   (FP),Y
   ADC   #$00
   LDY   #$8D
   STA   (FP),Y
   LDY   #$87
   LDA   (FP),Y
   LDY   #$8E
   STA   (FP),Y
   LDY   #$88
   LDA   (FP),Y
   LDY   #$8F
   STA   (FP),Y
   LDY   #$89
   LDA   (FP),Y
   LDY   #$90
   STA   (FP),Y
   LDY   #$8A
   LDA   (FP),Y
   LDY   #$91
   STA   (FP),Y
   LDY   #$8B
   LDA   (FP),Y
   LDY   #$92
   STA   (FP),Y
   LDY   #$8C
   LDA   (FP),Y
   LDY   #$93
   STA   (FP),Y
   LDY   #$8D
   LDA   (FP),Y
   AND   #$1F
   LDY   #$94
   STA   (FP),Y
   LDA   v1
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$98
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$99
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@55
   LDA   #$00
   JMP   .sx_done@56
.sx_neg@55:
   LDA   #$FF
.sx_done@56:
   LDY   #$9A
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$98
   LDA   (FP),Y
   LDY   #$9F
   STA   (FP),Y
   LDY   #$99
   LDA   (FP),Y
   LDY   #$A0
   STA   (FP),Y
   LDY   #$9A
   LDA   (FP),Y
   LDY   #$A1
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   AND   #$1F
   INY
   STA   (FP),Y
   LDY   #$94
   LDA   (FP),Y
   LDY   #$A5
   CMP   (FP),Y
   BNE   .thread@1
   LDY   #$93
   LDA   (FP),Y
   LDY   #$A4
   CMP   (FP),Y
   BNE   .thread@1
   LDY   #$92
   LDA   (FP),Y
   LDY   #$A3
   CMP   (FP),Y
   BNE   .thread@1
   LDY   #$91
   LDA   (FP),Y
   LDY   #$A2
   CMP   (FP),Y
   BNE   .thread@1
   LDY   #$90
   LDA   (FP),Y
   LDY   #$A1
   CMP   (FP),Y
   BNE   .thread@1
   LDY   #$8F
   LDA   (FP),Y
   LDY   #$A0
   CMP   (FP),Y
   BNE   .thread@1
   LDY   #$8E
   LDA   (FP),Y
   LDY   #$9F
   CMP   (FP),Y
   BEQ   .cmp_true@57
.thread@1:
   LDA   #$00
   JMP   .cmp_end@58
.cmp_true@57:
   LDA   #$01
.cmp_end@58:
   LDY   #$A6
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   EOR   #$FF
   LDY   #$A8
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   EOR   #$FF
   LDY   #$A9
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$AA
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$AB
   STA   (FP),Y
   LDA   #$FF
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$AB
   LDA   (FP),Y
   LDY   #$AD
   CMP   (FP),Y
   BNE   .cmp_true@60
   LDY   #$AA
   LDA   (FP),Y
   LDY   #$AC
   CMP   (FP),Y
   BNE   .cmp_true@60
   LDA   #$00
   JMP   .cmp_end@61
.cmp_true@60:
   LDA   #$01
.cmp_end@61:
   LDY   #$AE
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
   LDY   #$B0
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   LDY   #$B1
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   LDY   #$B1
   LDA   (FP),Y
   LDY   #$02
   STA   (FP),Y
.if_end@4:
   LDA   #$00
   LDY   #$B2
   STA   (FP),Y
   LDA   #$01
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$B3
   LDA   (FP),Y
   CMP   #$01
   BNE   .cmp_true@63
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@63
   LDA   #$00
   JMP   .cmp_end@64
.cmp_true@63:
   LDA   #$01
.cmp_end@64:
   LDY   #$B6
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
   LDY   #$B8
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   LDY   #$B9
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   LDY   #$B9
   LDA   (FP),Y
   LDY   #$02
   STA   (FP),Y
.if_end@5:
   LDA   v4
   LDY   #$BA
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$BC
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$BD
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@66
   LDA   #$00
   JMP   .sx_done@67
.sx_neg@66:
   LDA   #$FF
.sx_done@67:
   LDY   #$BE
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   v5
   SEC
   LDY   #$BC
   SBC   (FP),Y
   LDY   #$C0
   STA   (FP),Y
   LDA   v5+1
   LDY   #$BD
   SBC   (FP),Y
   LDY   #$C1
   STA   (FP),Y
   LDA   v5+2
   LDY   #$BE
   SBC   (FP),Y
   LDY   #$C2
   STA   (FP),Y
   LDA   v5+3
   LDY   #$BF
   SBC   (FP),Y
   LDY   #$C3
   STA   (FP),Y
   LDA   v0
   INY
   STA   (FP),Y
   LDA   v0
   BMI   .sx_neg@68
   LDA   #$00
   JMP   .sx_done@69
.sx_neg@68:
   LDA   #$FF
.sx_done@69:
   LDY   #$C5
   STA   (FP),Y
   LDA   v5
   STA   HARGS
   LDA   v5+1
   STA   HARGS+1
   LDA   v5+2
   STA   HARGS+2
   LDA   v5+3
   STA   HARGS+3
   DEY
   LDA   (FP),Y
   STA   HARGS+4
   JSR   lsr32
   LDA   HARGS+5
   LDY   #$C6
   STA   (FP),Y
   LDA   HARGS+6
   INY
   STA   (FP),Y
   LDA   HARGS+7
   INY
   STA   (FP),Y
   LDA   HARGS+8
   INY
   STA   (FP),Y
   LDY   #$C0
   LDA   (FP),Y
   SEC
   LDY   #$C6
   SBC   (FP),Y
   LDY   #$C1
   LDA   (FP),Y
   LDY   #$C7
   SBC   (FP),Y
   LDY   #$C2
   LDA   (FP),Y
   LDY   #$C8
   SBC   (FP),Y
   LDY   #$C3
   LDA   (FP),Y
   LDY   #$C9
   SBC   (FP),Y
   LDA   #$00
   ROL
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDA   v4
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDA   v1
   INY
   STA   (FP),Y
   LDA   v1
   BMI   .sx_neg@70
   LDA   #$00
   JMP   .sx_done@71
.sx_neg@70:
   LDA   #$FF
.sx_done@71:
   LDY   #$CF
   STA   (FP),Y
   LDA   v3
   INY
   STA   (FP),Y
   LDA   v3
   BMI   .sx_neg@72
   LDA   #$00
   JMP   .sx_done@73
.sx_neg@72:
   LDA   #$FF
.sx_done@73:
   LDY   #$D1
   STA   (FP),Y
   LDY   #$CE
   LDA   (FP),Y
   LDY   #$D0
   ORA   (FP),Y
   LDY   #$D2
   STA   (FP),Y
   LDY   #$CF
   LDA   (FP),Y
   LDY   #$D1
   ORA   (FP),Y
   LDY   #$D3
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   SEC
   LDY   #$CC
   SBC   (FP),Y
   LDY   #$D3
   LDA   (FP),Y
   LDY   #$CD
   SBC   (FP),Y
   BVC   .cmp_novf@74
   EOR   #$80
.cmp_novf@74:
   ASL
   LDA   #$00
   ROL
   LDY   #$D4
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$CA
   LDA   (FP),Y
   LDY   #$D4
   AND   (FP),Y
   LDY   #$D6
   STA   (FP),Y
   LDY   #$CB
   LDA   (FP),Y
   LDY   #$D5
   AND   (FP),Y
   LDY   #$D7
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$D8
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$D9
   STA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@75
   DEY
   LDA   (FP),Y
   CMP   #$01
   BNE   .cmp_true@75
   LDA   #$00
   JMP   .cmp_end@76
.cmp_true@75:
   LDA   #$01
.cmp_end@76:
   LDY   #$DA
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
   ORA   #$40
   LDY   #$DC
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   LDY   #$DD
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   LDY   #$DD
   LDA   (FP),Y
   LDY   #$02
   STA   (FP),Y
.if_end@6:
   LDA   v1
   LDY   #$DE
   STA   (FP),Y
   INY
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@78
   LDA   #$00
   JMP   .sx_done@79
.sx_neg@78:
   LDA   #$FF
.sx_done@79:
   LDY   #$E0
   STA   (FP),Y
   LDA   #$FF
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@80
   LDA   #$00
   JMP   .sx_done@81
.sx_neg@80:
   LDA   #$FF
.sx_done@81:
   LDY   #$E3
   STA   (FP),Y
   LDY   #$E0
   LDA   (FP),Y
   LDY   #$E3
   CMP   (FP),Y
   BNE   .cmp_true@82
   LDY   #$DF
   LDA   (FP),Y
   LDY   #$E2
   CMP   (FP),Y
   BNE   .cmp_true@82
   LDA   #$00
   JMP   .cmp_end@83
.cmp_true@82:
   LDA   #$01
.cmp_end@83:
   LDY   #$E4
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@7
   LDY   #$01
   LDA   (FP),Y
   ORA   #$80
   LDY   #$E6
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   LDY   #$E7
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   LDY   #$E7
   LDA   (FP),Y
   LDY   #$02
   STA   (FP),Y
.if_end@7:
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
   ADC   #$E9
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$E8
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
   DC.B  $01

v1:
   DC.B  $FF

v2:
   DC.L  $0000000C

v3:
   DC.B  $80

v4:
   DS.B  1

v5:
   DC.L  $00000006
