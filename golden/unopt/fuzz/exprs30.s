SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24

check:
   SUBROUTINE

   ; prologue: 0 arg bytes, 179 local bytes
   SEC
   LDA   SSP
   SBC   #$B5
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$B4
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
   LDA   #$07
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$03
   LDA   (FP),Y
   SEC
   SBC   #$FF
   INY
   LDA   (FP),Y
   SBC   #$FF
   INY
   LDA   (FP),Y
   SBC   #$FF
   INY
   LDA   (FP),Y
   SBC   #$7F
   LDA   #$00
   ROL
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDA   v2
   INY
   STA   (FP),Y
   LDA   v2+1
   INY
   STA   (FP),Y
   LDA   v2+1
   BMI   .sx_neg@0
   LDA   #$00
   JMP   .sx_done@1
.sx_neg@0:
   LDA   #$FF
.sx_done@1:
   LDY   #$0B
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   v0
   STA   HARGS
   LDA   v0+1
   STA   HARGS+1
   LDA   v0+2
   STA   HARGS+2
   LDA   v0+3
   STA   HARGS+3
   LDY   #$09
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
   JSR   sdivmod32
   LDA   HARGS+12
   LDY   #$0D
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
   LDY   #$07
   LDA   (FP),Y
   LDY   #$11
   STA   (FP),Y
   LDY   #$08
   LDA   (FP),Y
   LDY   #$12
   STA   (FP),Y
   LDY   #$0D
   LDA   (FP),Y
   TAX
   BEQ   .shift_done@3
.shift_loop@2:
   LDY   #$12
   LDA   (FP),Y
   ASL
   LDA   (FP),Y
   ROR
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ROR
   STA   (FP),Y
   DEX
   BNE   .shift_loop@2
.shift_done@3:
   LDA   v2
   ORA   v2+1
   BEQ   .and_false@0
   LDA   v5
   BEQ   .and_false@0
   LDA   #$01
   LDY   #$13
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   JMP   .and_end@1
.and_false@0:
   LDA   #$00
   LDY   #$13
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@1:
   LDY   #$11
   LDA   (FP),Y
   LDY   #$15
   STA   (FP),Y
   LDY   #$12
   LDA   (FP),Y
   LDY   #$16
   STA   (FP),Y
   LDY   #$13
   LDA   (FP),Y
   TAX
   BEQ   .shift_done@5
.shift_loop@4:
   LDY   #$16
   LDA   (FP),Y
   ASL
   LDA   (FP),Y
   ROR
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ROR
   STA   (FP),Y
   DEX
   BNE   .shift_loop@4
.shift_done@5:
   LDY   #$15
   LDA   (FP),Y
   LDY   #$17
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$18
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
   LDY   #$19
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
   ORA   #$01
   LDY   #$1B
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   LDY   #$1C
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   LDY   #$1C
   LDA   (FP),Y
   LDY   #$02
   STA   (FP),Y
.if_end@2:
   LDA   v3
   LDY   #$1D
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
   CMP   #$22
   BNE   .cmp_true@9
   DEY
   LDA   (FP),Y
   CMP   #$52
   BNE   .cmp_true@9
   DEY
   LDA   (FP),Y
   CMP   #$7D
   BNE   .cmp_true@9
   DEY
   LDA   (FP),Y
   CMP   #$BD
   BNE   .cmp_true@9
   LDA   #$00
   JMP   .cmp_end@10
.cmp_true@9:
   LDA   #$01
.cmp_end@10:
   LDY   #$21
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
   ORA   #$02
   LDY   #$23
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   LDY   #$24
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   LDY   #$24
   LDA   (FP),Y
   LDY   #$02
   STA   (FP),Y
.if_end@3:
   LDA   #$79
   LDY   #$25
   STA   (FP),Y
   INY
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@12
   LDA   #$00
   JMP   .sx_done@13
.sx_neg@12:
   LDA   #$FF
.sx_done@13:
   LDY   #$27
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@6
   LDA   v5
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDA   #$02
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$28
   LDA   (FP),Y
   LDY   #$2B
   AND   (FP),Y
   LDY   #$2D
   STA   (FP),Y
   LDY   #$29
   LDA   (FP),Y
   LDY   #$2C
   AND   (FP),Y
   LDY   #$2E
   STA   (FP),Y
   DEY
   LDA   (FP),Y
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
   LDY   #$2F
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@7:
   LDY   #$2F
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@0
   JMP   .and_false@4
.lb_skip@0:
   SEC
   LDA   v3+1
   SBC   #$80
   LDA   v3+2
   SBC   #$00
   LDA   v3+3
   SBC   #$00
   LDA   #$00
   ROL
   LDY   #$31
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDA   v5
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$35
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$36
   STA   (FP),Y
   LDA   #$7F
   DEY
   AND   (FP),Y
   LDY   #$37
   STA   (FP),Y
   LDA   #$00
   DEY
   AND   (FP),Y
   LDY   #$38
   STA   (FP),Y
   LDY   #$31
   LDA   (FP),Y
   LDY   #$39
   STA   (FP),Y
   LDY   #$32
   LDA   (FP),Y
   LDY   #$3A
   STA   (FP),Y
   LDY   #$37
   LDA   (FP),Y
   TAX
   BEQ   .shift_done@15
.shift_loop@14:
   LDY   #$3A
   LDA   (FP),Y
   ASL
   LDA   (FP),Y
   ROR
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ROR
   STA   (FP),Y
   DEX
   BNE   .shift_loop@14
.shift_done@15:
   LDY   #$39
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@4
   LDA   #$01
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   JMP   .and_end@5
.and_false@4:
   LDA   #$00
   LDY   #$3B
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@5:
   LDY   #$3B
   LDA   (FP),Y
   LDY   #$3D
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$3E
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@16
   LDA   #$00
   JMP   .cmp_end@17
.cmp_true@16:
   LDA   #$01
.cmp_end@17:
   LDY   #$3F
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
   ORA   #$04
   LDY   #$41
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   LDY   #$42
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   LDY   #$42
   LDA   (FP),Y
   LDY   #$02
   STA   (FP),Y
.if_end@8:
   LDA   #$00
   LDY   #$43
   STA   (FP),Y
   LDA   #$80
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$44
   LDA   (FP),Y
   CMP   #$80
   BNE   .cmp_true@18
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@18
   LDA   #$00
   JMP   .cmp_end@19
.cmp_true@18:
   LDA   #$01
.cmp_end@19:
   LDY   #$47
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
   ORA   #$08
   LDY   #$49
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   LDY   #$4A
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   LDY   #$4A
   LDA   (FP),Y
   LDY   #$02
   STA   (FP),Y
.if_end@9:
   LDA   v1
   LDY   #$4B
   STA   (FP),Y
   LDA   v1
   BMI   .sx_neg@21
   LDA   #$00
   JMP   .sx_done@22
.sx_neg@21:
   LDA   #$FF
.sx_done@22:
   LDY   #$4C
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$4D
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$4E
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   SBC   #$00
   INY
   LDA   (FP),Y
   SBC   #$00
   LDA   #$00
   ROL
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDA   v5
   INY
   STA   (FP),Y
   LDA   #$00
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
   ORA   #$00
   BMI   .sx_neg@23
   LDA   #$00
   JMP   .sx_done@24
.sx_neg@23:
   LDA   #$FF
.sx_done@24:
   LDY   #$55
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$53
   LDA   (FP),Y
   CLC
   SBC   v3
   INY
   LDA   (FP),Y
   SBC   v3+1
   INY
   LDA   (FP),Y
   SBC   v3+2
   INY
   LDA   (FP),Y
   SBC   v3+3
   LDA   #$00
   ROL
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$4F
   LDA   (FP),Y
   CLC
   LDY   #$57
   ADC   (FP),Y
   LDY   #$59
   STA   (FP),Y
   LDY   #$50
   LDA   (FP),Y
   LDY   #$58
   ADC   (FP),Y
   LDY   #$5A
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$5B
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$5C
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@25
   LDA   #$00
   JMP   .sx_done@26
.sx_neg@25:
   LDA   #$FF
.sx_done@26:
   LDY   #$5D
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$5B
   LDA   (FP),Y
   CLC
   ADC   v3
   LDY   #$5F
   STA   (FP),Y
   LDY   #$5C
   LDA   (FP),Y
   ADC   v3+1
   LDY   #$60
   STA   (FP),Y
   LDY   #$5D
   LDA   (FP),Y
   ADC   v3+2
   LDY   #$61
   STA   (FP),Y
   LDY   #$5E
   LDA   (FP),Y
   ADC   v3+3
   LDY   #$62
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
   LDY   #$62
   LDA   (FP),Y
   LDY   #$66
   STA   (FP),Y
   CMP   #$22
   BNE   .cmp_true@27
   DEY
   LDA   (FP),Y
   CMP   #$52
   BNE   .cmp_true@27
   DEY
   LDA   (FP),Y
   CMP   #$7D
   BNE   .cmp_true@27
   DEY
   LDA   (FP),Y
   CMP   #$BE
   BNE   .cmp_true@27
   LDA   #$00
   JMP   .cmp_end@28
.cmp_true@27:
   LDA   #$01
.cmp_end@28:
   LDY   #$67
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
   ORA   #$10
   LDY   #$69
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   LDY   #$6A
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   LDY   #$6A
   LDA   (FP),Y
   LDY   #$02
   STA   (FP),Y
.if_end@10:
   LDA   v0
   LDY   #$6B
   STA   (FP),Y
   LDA   v0+1
   INY
   STA   (FP),Y
   LDA   v0+2
   INY
   STA   (FP),Y
   LDA   v0+3
   INY
   STA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@30
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@30
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@30
   DEY
   LDA   (FP),Y
   CMP   #$01
   BNE   .cmp_true@30
   LDA   #$00
   JMP   .cmp_end@31
.cmp_true@30:
   LDA   #$01
.cmp_end@31:
   LDY   #$6F
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
   ORA   #$20
   LDY   #$71
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   LDY   #$72
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   LDY   #$72
   LDA   (FP),Y
   LDY   #$02
   STA   (FP),Y
.if_end@11:
   LDA   v3+3
   BNE   .cmp_true@33
   LDA   v3+2
   BNE   .cmp_true@33
   LDA   v3+1
   BNE   .cmp_true@33
   LDA   v3
   CMP   #$80
   BNE   .cmp_true@33
   LDA   #$00
   JMP   .cmp_end@34
.cmp_true@33:
   LDA   #$01
.cmp_end@34:
   LDY   #$73
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$75
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$76
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@36
   LDA   #$00
   JMP   .sx_done@37
.sx_neg@36:
   LDA   #$FF
.sx_done@37:
   LDY   #$77
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$80
   INY
   STA   (FP),Y
   INY
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
   LDY   #$7C
   LDA   (FP),Y
   ORA   v0
   LDY   #$80
   STA   (FP),Y
   LDY   #$7D
   LDA   (FP),Y
   ORA   v0+1
   LDY   #$81
   STA   (FP),Y
   LDY   #$7E
   LDA   (FP),Y
   ORA   v0+2
   LDY   #$82
   STA   (FP),Y
   LDY   #$7F
   LDA   (FP),Y
   ORA   v0+3
   LDY   #$83
   STA   (FP),Y
   LDY   #$75
   LDA   (FP),Y
   LDY   #$80
   ORA   (FP),Y
   LDY   #$84
   STA   (FP),Y
   LDY   #$76
   LDA   (FP),Y
   LDY   #$81
   ORA   (FP),Y
   LDY   #$85
   STA   (FP),Y
   LDY   #$77
   LDA   (FP),Y
   LDY   #$82
   ORA   (FP),Y
   LDY   #$86
   STA   (FP),Y
   LDY   #$78
   LDA   (FP),Y
   LDY   #$83
   ORA   (FP),Y
   LDY   #$87
   STA   (FP),Y
   LDY   #$84
   LDA   (FP),Y
   LDY   #$88
   STA   (FP),Y
   LDY   #$85
   LDA   (FP),Y
   LDY   #$89
   STA   (FP),Y
   LDY   #$86
   LDA   (FP),Y
   LDY   #$8A
   STA   (FP),Y
   LDY   #$87
   LDA   (FP),Y
   LDY   #$8B
   STA   (FP),Y
   LDA   #$75
   BEQ   .lnot_true@42
   LDA   #$00
   JMP   .lnot_end@43
.lnot_true@42:
   LDA   #$01
.lnot_end@43:
   LDY   #$8C
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$8E
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$8F
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@44
   LDA   #$00
   JMP   .sx_done@45
.sx_neg@44:
   LDA   #$FF
.sx_done@45:
   LDY   #$90
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$88
   LDA   (FP),Y
   LDY   #$8E
   AND   (FP),Y
   LDY   #$92
   STA   (FP),Y
   LDY   #$89
   LDA   (FP),Y
   LDY   #$8F
   AND   (FP),Y
   LDY   #$93
   STA   (FP),Y
   LDY   #$8A
   LDA   (FP),Y
   LDY   #$90
   AND   (FP),Y
   LDY   #$94
   STA   (FP),Y
   LDY   #$8B
   LDA   (FP),Y
   LDY   #$91
   AND   (FP),Y
   LDY   #$95
   STA   (FP),Y
   LDY   #$92
   LDA   (FP),Y
   LDY   #$96
   STA   (FP),Y
   LDY   #$93
   LDA   (FP),Y
   LDY   #$97
   STA   (FP),Y
   LDY   #$94
   LDA   (FP),Y
   LDY   #$98
   STA   (FP),Y
   LDY   #$95
   LDA   (FP),Y
   LDY   #$99
   STA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@46
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@46
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@46
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@46
   LDA   #$00
   JMP   .cmp_end@47
.cmp_true@46:
   LDA   #$01
.cmp_end@47:
   LDY   #$9A
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@12
   LDY   #$01
   LDA   (FP),Y
   ORA   #$40
   LDY   #$9C
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   LDY   #$9D
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   LDY   #$9D
   LDA   (FP),Y
   LDY   #$02
   STA   (FP),Y
.if_end@12:
   LDA   v1
   LDY   #$9E
   STA   (FP),Y
   LDA   v1
   BMI   .sx_neg@49
   LDA   #$00
   JMP   .sx_done@50
.sx_neg@49:
   LDA   #$FF
.sx_done@50:
   LDY   #$9F
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$A0
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$A1
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   HARGS
   INY
   LDA   (FP),Y
   STA   HARGS+1
   LDA   v4
   STA   HARGS+2
   LDA   v4+1
   STA   HARGS+3
   JSR   udivmod16
   LDA   HARGS+6
   LDY   #$A2
   STA   (FP),Y
   LDA   HARGS+7
   INY
   STA   (FP),Y
   LDA   v1
   INY
   STA   (FP),Y
   LDA   v1
   BMI   .sx_neg@51
   LDA   #$00
   JMP   .sx_done@52
.sx_neg@51:
   LDA   #$FF
.sx_done@52:
   LDY   #$A5
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$A6
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$A7
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   LDY   #$A2
   SBC   (FP),Y
   LDY   #$A7
   LDA   (FP),Y
   LDY   #$A3
   SBC   (FP),Y
   LDA   #$00
   ROL
   LDY   #$A8
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   SEC
   SBC   v2
   INY
   STA   (FP),Y
   LDA   #$00
   SBC   v2+1
   INY
   STA   (FP),Y
   LDY   #$A8
   LDA   (FP),Y
   SEC
   LDY   #$AA
   SBC   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$AB
   SBC   (FP),Y
   BVC   .cmp_novf@53
   EOR   #$80
.cmp_novf@53:
   ASL
   LDA   #$00
   ROL
   EOR   #$01
   LDY   #$AC
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$AE
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$AF
   STA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@54
   DEY
   LDA   (FP),Y
   CMP   #$01
   BNE   .cmp_true@54
   LDA   #$00
   JMP   .cmp_end@55
.cmp_true@54:
   LDA   #$01
.cmp_end@55:
   LDY   #$B0
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@13
   LDY   #$01
   LDA   (FP),Y
   ORA   #$80
   LDY   #$B2
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   LDY   #$B3
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   LDY   #$B3
   LDA   (FP),Y
   LDY   #$02
   STA   (FP),Y
.if_end@13:
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
   ADC   #$B5
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$B4
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
   DC.L  $00000001

v1:
   DC.B  $0A

v2:
   DC.W  $FFFF

v3:
   DC.L  $22527DBD

v4:
   DC.W  $0001

v5:
   DC.B  $01
