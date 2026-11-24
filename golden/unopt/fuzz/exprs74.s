SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24

check:
   SUBROUTINE

   ; prologue: 0 arg bytes, 153 local bytes
   SEC
   LDA   SSP
   SBC   #$9B
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$9A
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
   LDA   v3+1
   INY
   STA   (FP),Y
   LDA   v3+1
   BMI   .sx_neg@0
   LDA   #$00
   JMP   .sx_done@1
.sx_neg@0:
   LDA   #$FF
.sx_done@1:
   LDY   #$05
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
   LDY   #$03
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
   LDY   #$07
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
   INY
   ORA   (FP),Y
   INY
   ORA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@0
   LDA   v0
   EOR   #$FF
   INY
   STA   (FP),Y
   LDA   v0+1
   EOR   #$FF
   INY
   STA   (FP),Y
   LDA   v0+2
   EOR   #$FF
   INY
   STA   (FP),Y
   LDA   v0+3
   EOR   #$FF
   INY
   STA   (FP),Y
   LDY   #$0B
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   INY
   ORA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@0
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   JMP   .or_end@1
.or_true@0:
   LDA   #$01
   LDY   #$0F
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@1:
   LDA   v4
   LDY   #$11
   STA   (FP),Y
   LDA   v4+1
   INY
   STA   (FP),Y
   LDA   v4+2
   INY
   STA   (FP),Y
   LDA   v4+3
   INY
   STA   (FP),Y
   LDY   #$11
   LDA   (FP),Y
   SEC
   SBC   #$80
   INY
   LDA   (FP),Y
   SBC   #$00
   INY
   LDA   (FP),Y
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
   LDA   v2
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
   LDY   #$17
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
   JSR   sdivmod16
   LDA   HARGS+4
   LDY   #$1B
   STA   (FP),Y
   LDA   HARGS+5
   INY
   STA   (FP),Y
   LDY   #$16
   LDA   (FP),Y
   LDY   #$1C
   CMP   (FP),Y
   BNE   .cmp_true@2
   LDY   #$15
   LDA   (FP),Y
   LDY   #$1B
   CMP   (FP),Y
   BNE   .cmp_true@2
   LDA   #$00
   JMP   .cmp_end@3
.cmp_true@2:
   LDA   #$01
.cmp_end@3:
   LDY   #$1D
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$0F
   LDA   (FP),Y
   LDY   #$1D
   ORA   (FP),Y
   LDY   #$1F
   STA   (FP),Y
   LDY   #$10
   LDA   (FP),Y
   LDY   #$1E
   ORA   (FP),Y
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
   CMP   #$00
   BNE   .cmp_true@5
   DEY
   LDA   (FP),Y
   CMP   #$01
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
   BEQ   .if_end@2
   LDY   #$01
   LDA   (FP),Y
   ORA   #$01
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
.if_end@2:
   LDA   #$02
   LDY   #$27
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CMP   #$02
   BNE   .cmp_true@8
   LDA   #$00
   JMP   .cmp_end@9
.cmp_true@8:
   LDA   #$01
.cmp_end@9:
   LDY   #$29
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
   LDY   #$2B
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   LDY   #$2C
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   LDY   #$2C
   LDA   (FP),Y
   LDY   #$02
   STA   (FP),Y
.if_end@3:
   LDA   #$00
   LDY   #$2D
   STA   (FP),Y
   LDA   #$01
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$2E
   LDA   (FP),Y
   CMP   #$01
   BNE   .cmp_true@10
   DEY
   LDA   (FP),Y
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
   BEQ   .if_end@4
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
.if_end@4:
   LDA   v4
   LDY   #$35
   STA   (FP),Y
   LDA   v4+1
   INY
   STA   (FP),Y
   LDA   v4+2
   INY
   STA   (FP),Y
   LDA   v4+3
   INY
   STA   (FP),Y
   CMP   #$FF
   BNE   .cmp_true@13
   DEY
   LDA   (FP),Y
   CMP   #$FF
   BNE   .cmp_true@13
   DEY
   LDA   (FP),Y
   CMP   #$FF
   BNE   .cmp_true@13
   DEY
   LDA   (FP),Y
   CMP   #$FF
   BNE   .cmp_true@13
   LDA   #$00
   JMP   .cmp_end@14
.cmp_true@13:
   LDA   #$01
.cmp_end@14:
   LDY   #$39
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
   LDY   #$3B
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   LDY   #$3C
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   LDY   #$3C
   LDA   (FP),Y
   LDY   #$02
   STA   (FP),Y
.if_end@5:
   LDA   v2
   LDY   #$3D
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$3F
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$40
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@16
   LDA   #$00
   JMP   .sx_done@17
.sx_neg@16:
   LDA   #$FF
.sx_done@17:
   LDY   #$41
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   v4
   LDY   #$3F
   ORA   (FP),Y
   LDY   #$43
   STA   (FP),Y
   LDA   v4+1
   LDY   #$40
   ORA   (FP),Y
   LDY   #$44
   STA   (FP),Y
   LDA   v4+2
   LDY   #$41
   ORA   (FP),Y
   LDY   #$45
   STA   (FP),Y
   LDA   v4+3
   LDY   #$42
   ORA   (FP),Y
   LDY   #$46
   STA   (FP),Y
   CMP   v4+3
   BNE   .cmp_true@18
   DEY
   LDA   (FP),Y
   CMP   v4+2
   BNE   .cmp_true@18
   DEY
   LDA   (FP),Y
   CMP   v4+1
   BNE   .cmp_true@18
   DEY
   LDA   (FP),Y
   CMP   v4
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
   LDY   #$49
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$4A
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@21
   LDA   #$00
   JMP   .sx_done@22
.sx_neg@21:
   LDA   #$FF
.sx_done@22:
   LDY   #$4B
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$49
   LDA   (FP),Y
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
   INY
   STA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@23
   LDY   #$4E
   LDA   (FP),Y
   BNE   .cmp_true@23
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@23
   LDA   #$00
   JMP   .cmp_end@24
.cmp_true@23:
   LDA   #$01
.cmp_end@24:
   LDY   #$51
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
   LDY   #$53
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   LDY   #$54
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   LDY   #$54
   LDA   (FP),Y
   LDY   #$02
   STA   (FP),Y
.if_end@6:
   LDA   v0
   LDY   #$55
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
   CMP   #$83
   BNE   .cmp_true@26
   DEY
   LDA   (FP),Y
   CMP   #$E9
   BNE   .cmp_true@26
   DEY
   LDA   (FP),Y
   CMP   #$60
   BNE   .cmp_true@26
   DEY
   LDA   (FP),Y
   CMP   #$E4
   BNE   .cmp_true@26
   LDA   #$00
   JMP   .cmp_end@27
.cmp_true@26:
   LDA   #$01
.cmp_end@27:
   LDY   #$59
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
   LDY   #$5B
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   LDY   #$5C
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   LDY   #$5C
   LDA   (FP),Y
   LDY   #$02
   STA   (FP),Y
.if_end@7:
   LDA   v2
   LDY   #$5D
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
   ORA   #$00
   BMI   .sx_neg@29
   LDA   #$00
   JMP   .sx_done@30
.sx_neg@29:
   LDA   #$FF
.sx_done@30:
   LDY   #$61
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   v0+3
   CMP   (FP),Y
   BNE   .cmp_true@31
   LDA   v0+2
   DEY
   CMP   (FP),Y
   BNE   .cmp_true@31
   LDA   v0+1
   DEY
   CMP   (FP),Y
   BNE   .cmp_true@31
   LDA   v0
   DEY
   CMP   (FP),Y
   BNE   .cmp_true@31
   LDA   #$00
   JMP   .cmp_end@32
.cmp_true@31:
   LDA   #$01
.cmp_end@32:
   LDY   #$63
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$65
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$66
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@34
   LDA   #$00
   JMP   .sx_done@35
.sx_neg@34:
   LDA   #$FF
.sx_done@35:
   LDY   #$67
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   v0
   INY
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
   LDY   #$69
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
   LDA   #$FF
   STA   HARGS+4
   LDA   #$00
   STA   HARGS+5
   LDY   #$00
   STY   HARGS+6
   STA   HARGS+7
   JSR   udivmod32
   LDA   HARGS+8
   LDY   #$6D
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
   LDY   #$65
   LDA   (FP),Y
   CLC
   LDY   #$6D
   ADC   (FP),Y
   LDY   #$71
   STA   (FP),Y
   LDY   #$66
   LDA   (FP),Y
   LDY   #$6E
   ADC   (FP),Y
   LDY   #$72
   STA   (FP),Y
   LDY   #$67
   LDA   (FP),Y
   LDY   #$6F
   ADC   (FP),Y
   LDY   #$73
   STA   (FP),Y
   LDY   #$68
   LDA   (FP),Y
   LDY   #$70
   ADC   (FP),Y
   LDY   #$74
   STA   (FP),Y
   LDA   v5
   INY
   STA   (FP),Y
   INY
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
   LDA   v1
   INY
   STA   (FP),Y
   LDA   v1+1
   INY
   STA   (FP),Y
   LDA   v1+1
   BMI   .sx_neg@38
   LDA   #$00
   JMP   .sx_done@39
.sx_neg@38:
   LDA   #$FF
.sx_done@39:
   LDY   #$7A
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   v0
   SEC
   LDY   #$78
   SBC   (FP),Y
   LDA   v0+1
   INY
   SBC   (FP),Y
   LDA   v0+2
   INY
   SBC   (FP),Y
   LDA   v0+3
   INY
   SBC   (FP),Y
   BVC   .cmp_novf@40
   EOR   #$80
.cmp_novf@40:
   ASL
   LDA   #$00
   ROL
   LDY   #$7C
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$76
   LDA   (FP),Y
   LDY   #$7C
   ORA   (FP),Y
   LDY   #$7E
   STA   (FP),Y
   LDY   #$77
   LDA   (FP),Y
   LDY   #$7D
   ORA   (FP),Y
   LDY   #$7F
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$80
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$81
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@41
   LDA   #$00
   JMP   .sx_done@42
.sx_neg@41:
   LDA   #$FF
.sx_done@42:
   LDY   #$82
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$71
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
   LDY   #$80
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
   LDY   #$84
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
   CMP   #$00
   BNE   .cmp_true@43
   DEY
   LDA   (FP),Y
   CMP   #$84
   BNE   .cmp_true@43
   DEY
   LDA   (FP),Y
   CMP   #$6D
   BNE   .cmp_true@43
   DEY
   LDA   (FP),Y
   CMP   #$CF
   BNE   .cmp_true@43
   LDA   #$00
   JMP   .cmp_end@44
.cmp_true@43:
   LDA   #$01
.cmp_end@44:
   LDY   #$8C
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
.if_end@8:
   LDA   v2
   LDY   #$90
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDA   #$0D
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$91
   LDA   (FP),Y
   LDY   #$94
   CMP   (FP),Y
   BNE   .cmp_true@46
   LDA   #$00
   JMP   .cmp_end@47
.cmp_true@46:
   LDA   #$01
.cmp_end@47:
   LDY   #$96
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
   LDY   #$98
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   LDY   #$99
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   LDY   #$99
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
   ADC   #$9B
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$9A
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
   DC.L  $83E960E4

v1:
   DC.W  $8000

v2:
   DC.B  $0D

v3:
   DC.W  $FFFF

v4:
   DC.L  $FFFFFFFF

v5:
   DC.B  $01
