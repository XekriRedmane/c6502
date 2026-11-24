SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24

check:
   SUBROUTINE

   ; prologue: 0 arg bytes, 213 local bytes
   SEC
   LDA   SSP
   SBC   #$D7
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$D6
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
   LDA   v4
   INY
   STA   (FP),Y
   LDA   v4
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
   LDA   v2
   DEY
   ORA   (FP),Y
   LDY   #$07
   STA   (FP),Y
   LDA   v2+1
   DEY
   ORA   (FP),Y
   LDY   #$08
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDA   #$80
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$0B
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$0C
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   HARGS
   INY
   LDA   (FP),Y
   STA   HARGS+1
   LDA   v2
   STA   HARGS+2
   LDA   v2+1
   STA   HARGS+3
   JSR   mul16
   LDA   HARGS+4
   LDY   #$0D
   STA   (FP),Y
   LDA   HARGS+5
   INY
   STA   (FP),Y
   LDY   #$08
   LDA   (FP),Y
   LDY   #$0E
   CMP   (FP),Y
   BNE   .cmp_true@2
   LDY   #$07
   LDA   (FP),Y
   LDY   #$0D
   CMP   (FP),Y
   BNE   .cmp_true@2
   LDA   #$00
   JMP   .cmp_end@3
.cmp_true@2:
   LDA   #$01
.cmp_end@3:
   LDY   #$0F
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$11
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDA   #$01
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$13
   LDA   (FP),Y
   LDY   #$16
   CMP   (FP),Y
   BNE   .cmp_true@5
   LDA   #$00
   JMP   .cmp_end@6
.cmp_true@5:
   LDA   #$01
.cmp_end@6:
   LDY   #$18
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
   LDY   #$1A
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   LDY   #$1B
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   LDY   #$1B
   LDA   (FP),Y
   LDY   #$02
   STA   (FP),Y
.if_end@0:
   LDA   v0
   LDY   #$1C
   STA   (FP),Y
   LDA   v0+1
   INY
   STA   (FP),Y
   LDA   #$07
   STA   HARGS
   LDA   #$00
   STA   HARGS+1
   DEY
   LDA   (FP),Y
   STA   HARGS+2
   INY
   LDA   (FP),Y
   STA   HARGS+3
   JSR   udivmod16
   LDA   HARGS+4
   LDY   #$1E
   STA   (FP),Y
   LDA   HARGS+5
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$20
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$21
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$FF
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@7
   LDA   #$00
   JMP   .sx_done@8
.sx_neg@7:
   LDA   #$FF
.sx_done@8:
   LDY   #$26
   STA   (FP),Y
   SEC
   LDA   #$00
   DEY
   SBC   (FP),Y
   LDY   #$27
   STA   (FP),Y
   LDA   #$00
   DEY
   SBC   (FP),Y
   LDY   #$28
   STA   (FP),Y
   LDA   v3
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@9
   LDA   #$00
   JMP   .sx_done@10
.sx_neg@9:
   LDA   #$FF
.sx_done@10:
   LDY   #$2B
   STA   (FP),Y
   LDY   #$27
   LDA   (FP),Y
   LDY   #$2A
   AND   (FP),Y
   LDY   #$2C
   STA   (FP),Y
   LDY   #$28
   LDA   (FP),Y
   LDY   #$2B
   AND   (FP),Y
   LDY   #$2D
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$2E
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$2F
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@11
   LDA   #$00
   JMP   .sx_done@12
.sx_neg@11:
   LDA   #$FF
.sx_done@12:
   LDY   #$30
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$20
   LDA   (FP),Y
   LDY   #$2E
   EOR   (FP),Y
   LDY   #$32
   STA   (FP),Y
   LDY   #$21
   LDA   (FP),Y
   LDY   #$2F
   EOR   (FP),Y
   LDY   #$33
   STA   (FP),Y
   LDY   #$22
   LDA   (FP),Y
   LDY   #$30
   EOR   (FP),Y
   LDY   #$34
   STA   (FP),Y
   LDY   #$23
   LDA   (FP),Y
   LDY   #$31
   EOR   (FP),Y
   LDY   #$35
   STA   (FP),Y
   LDY   #$32
   LDA   (FP),Y
   LDY   #$36
   STA   (FP),Y
   LDY   #$33
   LDA   (FP),Y
   LDY   #$37
   STA   (FP),Y
   LDY   #$34
   LDA   (FP),Y
   LDY   #$38
   STA   (FP),Y
   LDY   #$35
   LDA   (FP),Y
   LDY   #$39
   STA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@13
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@13
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@13
   DEY
   LDA   (FP),Y
   CMP   #$01
   BNE   .cmp_true@13
   LDA   #$00
   JMP   .cmp_end@14
.cmp_true@13:
   LDA   #$01
.cmp_end@14:
   LDY   #$3A
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
   LDY   #$3C
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   LDY   #$3D
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   LDY   #$3D
   LDA   (FP),Y
   LDY   #$02
   STA   (FP),Y
.if_end@1:
   LDA   #$FF
   LDY   #$3E
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   v1
   INY
   STA   (FP),Y
   LDA   v1
   BMI   .sx_neg@16
   LDA   #$00
   JMP   .sx_done@17
.sx_neg@16:
   LDA   #$FF
.sx_done@17:
   LDY   #$41
   STA   (FP),Y
   LDY   #$3E
   LDA   (FP),Y
   SEC
   LDY   #$40
   SBC   (FP),Y
   LDY   #$42
   STA   (FP),Y
   LDY   #$3F
   LDA   (FP),Y
   LDY   #$41
   SBC   (FP),Y
   LDY   #$43
   STA   (FP),Y
   LDA   v1
   INY
   STA   (FP),Y
   LDA   v1
   BMI   .sx_neg@18
   LDA   #$00
   JMP   .sx_done@19
.sx_neg@18:
   LDA   #$FF
.sx_done@19:
   LDY   #$45
   STA   (FP),Y
   LDY   #$43
   LDA   (FP),Y
   LDY   #$45
   CMP   (FP),Y
   BNE   .thread@0
   LDY   #$42
   LDA   (FP),Y
   LDY   #$44
   CMP   (FP),Y
   BEQ   .cmp_true@20
.thread@0:
   LDA   #$00
   JMP   .cmp_end@21
.cmp_true@20:
   LDA   #$01
.cmp_end@21:
   LDY   #$46
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDA   v1
   INY
   STA   (FP),Y
   LDA   v1
   BMI   .sx_neg@23
   LDA   #$00
   JMP   .sx_done@24
.sx_neg@23:
   LDA   #$FF
.sx_done@24:
   LDY   #$49
   STA   (FP),Y
   LDA   v0+1
   CMP   v0+1
   BNE   .thread@1
   LDA   v0
   CMP   v0
   BEQ   .cmp_true@25
.thread@1:
   LDA   #$00
   JMP   .cmp_end@26
.cmp_true@25:
   LDA   #$01
.cmp_end@26:
   LDY   #$4A
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$48
   LDA   (FP),Y
   SEC
   LDY   #$4A
   SBC   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$4B
   SBC   (FP),Y
   BVC   .cmp_novf@28
   EOR   #$80
.cmp_novf@28:
   ASL
   LDA   #$00
   ROL
   LDY   #$4C
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$46
   LDA   (FP),Y
   SEC
   LDY   #$4C
   SBC   (FP),Y
   LDY   #$47
   LDA   (FP),Y
   LDY   #$4D
   SBC   (FP),Y
   BVC   .cmp_novf@29
   EOR   #$80
.cmp_novf@29:
   ASL
   LDA   #$00
   ROL
   EOR   #$01
   LDY   #$4E
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
   CMP   #$00
   BNE   .cmp_true@30
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@30
   LDA   #$00
   JMP   .cmp_end@31
.cmp_true@30:
   LDA   #$01
.cmp_end@31:
   LDY   #$52
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
   LDY   #$54
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   LDY   #$55
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   LDY   #$55
   LDA   (FP),Y
   LDY   #$02
   STA   (FP),Y
.if_end@2:
   LDA   v1
   LDY   #$56
   STA   (FP),Y
   INY
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@33
   LDA   #$00
   JMP   .sx_done@34
.sx_neg@33:
   LDA   #$FF
.sx_done@34:
   LDY   #$58
   STA   (FP),Y
   LDA   #$FF
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@35
   LDA   #$00
   JMP   .sx_done@36
.sx_neg@35:
   LDA   #$FF
.sx_done@36:
   LDY   #$5B
   STA   (FP),Y
   LDY   #$58
   LDA   (FP),Y
   LDY   #$5B
   CMP   (FP),Y
   BNE   .cmp_true@37
   LDY   #$57
   LDA   (FP),Y
   LDY   #$5A
   CMP   (FP),Y
   BNE   .cmp_true@37
   LDA   #$00
   JMP   .cmp_end@38
.cmp_true@37:
   LDA   #$01
.cmp_end@38:
   LDY   #$5C
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .or_true@4
   LDY   #$01
   LDA   (FP),Y
   ORA   #$08
   LDY   #$5E
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   LDY   #$5F
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   LDY   #$5F
   LDA   (FP),Y
   LDY   #$02
   STA   (FP),Y
   LDA   #$7F
   BNE   .or_true@4
   LDA   v5
   BNE   .or_true@4
   LDA   #$00
   LDY   #$60
   STA   (FP),Y
   INY
   STA   (FP),Y
   JMP   .or_end@5
.or_true@4:
   LDA   #$01
   LDY   #$60
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@5:
   LDY   #$60
   LDA   (FP),Y
   LDY   #$62
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$63
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@40
   LDA   #$00
   JMP   .sx_done@41
.sx_neg@40:
   LDA   #$FF
.sx_done@41:
   LDY   #$64
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   v0
   INY
   STA   (FP),Y
   LDA   v0+1
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$FF
   LDY   #$68
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$FF
   LDY   #$69
   STA   (FP),Y
   LDA   v2
   INY
   STA   (FP),Y
   LDA   v2+1
   INY
   STA   (FP),Y
   LDX   v2
   BEQ   .shift_done@43
.shift_loop@42:
   LDY   #$6A
   LDA   (FP),Y
   ASL
   STA   (FP),Y
   INY
   LDA   (FP),Y
   ROL
   STA   (FP),Y
   DEX
   BNE   .shift_loop@42
.shift_done@43:
   LDY   #$6A
   LDA   (FP),Y
   CLC
   LDY   #$68
   SBC   (FP),Y
   LDY   #$6B
   LDA   (FP),Y
   LDY   #$69
   SBC   (FP),Y
   LDA   #$00
   ROL
   LDY   #$6C
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$6E
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$6F
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@44
   LDA   #$00
   JMP   .sx_done@45
.sx_neg@44:
   LDA   #$FF
.sx_done@45:
   LDY   #$70
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$62
   LDA   (FP),Y
   LDY   #$6E
   AND   (FP),Y
   LDY   #$72
   STA   (FP),Y
   LDY   #$63
   LDA   (FP),Y
   LDY   #$6F
   AND   (FP),Y
   LDY   #$73
   STA   (FP),Y
   LDY   #$64
   LDA   (FP),Y
   LDY   #$70
   AND   (FP),Y
   LDY   #$74
   STA   (FP),Y
   LDY   #$65
   LDA   (FP),Y
   LDY   #$71
   AND   (FP),Y
   LDY   #$75
   STA   (FP),Y
   LDY   #$72
   LDA   (FP),Y
   LDY   #$76
   STA   (FP),Y
   LDY   #$73
   LDA   (FP),Y
   LDY   #$77
   STA   (FP),Y
   LDY   #$74
   LDA   (FP),Y
   LDY   #$78
   STA   (FP),Y
   LDY   #$75
   LDA   (FP),Y
   LDY   #$79
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
   LDY   #$7A
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
   LDY   #$7C
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   LDY   #$7D
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   LDY   #$7D
   LDA   (FP),Y
   LDY   #$02
   STA   (FP),Y
.if_end@6:
   LDA   v0
   LDY   #$7E
   STA   (FP),Y
   LDA   v0+1
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   HARGS
   INY
   LDA   (FP),Y
   STA   HARGS+1
   LDA   v2
   STA   HARGS+2
   LDA   v2+1
   STA   HARGS+3
   JSR   udivmod16
   LDA   HARGS+6
   LDY   #$80
   STA   (FP),Y
   LDA   HARGS+7
   INY
   STA   (FP),Y
   LDA   v0
   INY
   STA   (FP),Y
   LDA   v0+1
   INY
   STA   (FP),Y
   LDY   #$81
   LDA   (FP),Y
   LDY   #$83
   CMP   (FP),Y
   BNE   .thread@2
   LDY   #$80
   LDA   (FP),Y
   LDY   #$82
   CMP   (FP),Y
   BEQ   .cmp_true@49
.thread@2:
   LDA   #$00
   JMP   .cmp_end@50
.cmp_true@49:
   LDA   #$01
.cmp_end@50:
   LDY   #$84
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   EOR   v0
   LDY   #$86
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   EOR   v0+1
   LDY   #$87
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$88
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$89
   STA   (FP),Y
   LDA   #$76
   INY
   STA   (FP),Y
   LDA   #$E4
   INY
   STA   (FP),Y
   LDY   #$89
   LDA   (FP),Y
   LDY   #$8B
   CMP   (FP),Y
   BNE   .cmp_true@52
   LDY   #$88
   LDA   (FP),Y
   LDY   #$8A
   CMP   (FP),Y
   BNE   .cmp_true@52
   LDA   #$00
   JMP   .cmp_end@53
.cmp_true@52:
   LDA   #$01
.cmp_end@53:
   LDY   #$8C
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
   ORA   #$20
   LDY   #$8E
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   LDY   #$8F
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   LDY   #$8F
   LDA   (FP),Y
   LDY   #$02
   STA   (FP),Y
.if_end@7:
   LDA   v4
   LDY   #$90
   STA   (FP),Y
   LDA   v4
   BMI   .sx_neg@55
   LDA   #$00
   JMP   .sx_done@56
.sx_neg@55:
   LDA   #$FF
.sx_done@56:
   LDY   #$91
   STA   (FP),Y
   LDA   v1
   INY
   STA   (FP),Y
   LDA   v1
   BMI   .sx_neg@57
   LDA   #$00
   JMP   .sx_done@58
.sx_neg@57:
   LDA   #$FF
.sx_done@58:
   LDY   #$93
   STA   (FP),Y
   SEC
   LDA   #$00
   DEY
   SBC   (FP),Y
   LDY   #$94
   STA   (FP),Y
   LDA   #$00
   DEY
   SBC   (FP),Y
   LDY   #$95
   STA   (FP),Y
   LDY   #$90
   LDA   (FP),Y
   SEC
   LDY   #$94
   SBC   (FP),Y
   LDY   #$91
   LDA   (FP),Y
   LDY   #$95
   SBC   (FP),Y
   BVC   .cmp_novf@59
   EOR   #$80
.cmp_novf@59:
   ASL
   LDA   #$00
   ROL
   EOR   #$01
   LDY   #$96
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDA   v4
   INY
   STA   (FP),Y
   LDA   v4
   BMI   .sx_neg@60
   LDA   #$00
   JMP   .sx_done@61
.sx_neg@60:
   LDA   #$FF
.sx_done@61:
   LDY   #$99
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   HARGS
   INY
   LDA   (FP),Y
   STA   HARGS+1
   LDA   #$DC
   STA   HARGS+2
   LDA   #$74
   STA   HARGS+3
   JSR   sdivmod16
   LDA   HARGS+6
   LDY   #$9A
   STA   (FP),Y
   LDA   HARGS+7
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$9C
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$9D
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@62
   LDA   #$00
   JMP   .sx_done@63
.sx_neg@62:
   LDA   #$FF
.sx_done@63:
   LDY   #$9E
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   v3
   CLC
   LDY   #$9C
   SBC   (FP),Y
   LDA   v3+1
   INY
   SBC   (FP),Y
   LDA   v3+2
   INY
   SBC   (FP),Y
   LDA   v3+3
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
   SEC
   LDY   #$96
   SBC   (FP),Y
   LDY   #$A1
   LDA   (FP),Y
   LDY   #$97
   SBC   (FP),Y
   BVC   .cmp_novf@64
   EOR   #$80
.cmp_novf@64:
   ASL
   LDA   #$00
   ROL
   EOR   #$01
   LDY   #$A2
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$A4
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$A5
   STA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@65
   DEY
   LDA   (FP),Y
   CMP   #$01
   BNE   .cmp_true@65
   LDA   #$00
   JMP   .cmp_end@66
.cmp_true@65:
   LDA   #$01
.cmp_end@66:
   LDY   #$A6
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
   LDY   #$A8
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   LDY   #$A9
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   LDY   #$A9
   LDA   (FP),Y
   LDY   #$02
   STA   (FP),Y
.if_end@8:
   LDA   v1
   LDY   #$AA
   STA   (FP),Y
   LDA   v1
   BMI   .sx_neg@68
   LDA   #$00
   JMP   .sx_done@69
.sx_neg@68:
   LDA   #$FF
.sx_done@69:
   LDY   #$AB
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$AC
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$AD
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@70
   LDA   #$00
   JMP   .sx_done@71
.sx_neg@70:
   LDA   #$FF
.sx_done@71:
   LDY   #$AE
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$AC
   LDA   (FP),Y
   SEC
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
   DEY
   LDA   (FP),Y
   LDY   #$B2
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$B3
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@72
   LDA   #$00
   JMP   .sx_done@73
.sx_neg@72:
   LDA   #$FF
.sx_done@73:
   LDY   #$B4
   STA   (FP),Y
   INY
   STA   (FP),Y
   CMP   v3+3
   BNE   .cmp_true@74
   CMP   v3+2
   BNE   .cmp_true@74
   LDY   #$B3
   LDA   (FP),Y
   CMP   v3+1
   BNE   .cmp_true@74
   DEY
   LDA   (FP),Y
   CMP   v3
   BNE   .cmp_true@74
   LDA   #$00
   JMP   .cmp_end@75
.cmp_true@74:
   LDA   #$01
.cmp_end@75:
   LDY   #$B6
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDA   v5
   INY
   STA   (FP),Y
   LDA   v5
   BMI   .sx_neg@77
   LDA   #$00
   JMP   .sx_done@78
.sx_neg@77:
   LDA   #$FF
.sx_done@78:
   LDY   #$B9
   STA   (FP),Y
   LDA   v1
   INY
   STA   (FP),Y
   LDA   v1
   BMI   .sx_neg@79
   LDA   #$00
   JMP   .sx_done@80
.sx_neg@79:
   LDA   #$FF
.sx_done@80:
   LDY   #$BB
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   SEC
   LDY   #$B8
   SBC   (FP),Y
   LDY   #$BB
   LDA   (FP),Y
   LDY   #$B9
   SBC   (FP),Y
   BVC   .cmp_novf@81
   EOR   #$80
.cmp_novf@81:
   ASL
   LDA   #$00
   ROL
   EOR   #$01
   LDY   #$BC
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$BE
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$BF
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@82
   LDA   #$00
   JMP   .sx_done@83
.sx_neg@82:
   LDA   #$FF
.sx_done@83:
   LDY   #$C0
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   v4
   INY
   STA   (FP),Y
   LDA   v4
   BMI   .sx_neg@84
   LDA   #$00
   JMP   .sx_done@85
.sx_neg@84:
   LDA   #$FF
.sx_done@85:
   LDY   #$C3
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$C4
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$C5
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@86
   LDA   #$00
   JMP   .sx_done@87
.sx_neg@86:
   LDA   #$FF
.sx_done@87:
   LDY   #$C6
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   LDY   #$C4
   AND   (FP),Y
   LDY   #$C8
   STA   (FP),Y
   LDA   #$80
   LDY   #$C5
   AND   (FP),Y
   LDY   #$C9
   STA   (FP),Y
   LDA   #$00
   LDY   #$C6
   AND   (FP),Y
   LDY   #$CA
   STA   (FP),Y
   LDA   #$00
   LDY   #$C7
   AND   (FP),Y
   LDY   #$CB
   STA   (FP),Y
   LDY   #$C1
   LDA   (FP),Y
   LDY   #$CB
   CMP   (FP),Y
   BNE   .thread@3
   DEY
   CMP   (FP),Y
   BNE   .thread@3
   LDY   #$BF
   LDA   (FP),Y
   LDY   #$C9
   CMP   (FP),Y
   BNE   .thread@3
   LDY   #$BE
   LDA   (FP),Y
   LDY   #$C8
   CMP   (FP),Y
   BEQ   .cmp_true@88
.thread@3:
   LDA   #$00
   JMP   .cmp_end@89
.cmp_true@88:
   LDA   #$01
.cmp_end@89:
   LDY   #$CC
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$B6
   LDA   (FP),Y
   LDY   #$CC
   ORA   (FP),Y
   LDY   #$CE
   STA   (FP),Y
   LDY   #$B7
   LDA   (FP),Y
   LDY   #$CD
   ORA   (FP),Y
   LDY   #$CF
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$D0
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$D1
   STA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@91
   DEY
   LDA   (FP),Y
   CMP   #$01
   BNE   .cmp_true@91
   LDA   #$00
   JMP   .cmp_end@92
.cmp_true@91:
   LDA   #$01
.cmp_end@92:
   LDY   #$D2
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
   ORA   #$80
   LDY   #$D4
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   LDY   #$D5
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   LDY   #$D5
   LDA   (FP),Y
   LDY   #$02
   STA   (FP),Y
.if_end@9:
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
   ADC   #$D7
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$D6
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
   DC.W  $E476

v1:
   DC.B  $FF

v2:
   DC.W  $0005

v3:
   DC.L  $FFFFFFFF

v4:
   DC.B  $FF

v5:
   DS.B  1
