SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24

check:
   SUBROUTINE

   ; prologue: 0 arg bytes, 177 local bytes
   SEC
   LDA   SSP
   SBC   #$B3
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$B2
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
   LDY   #$05
   LDA   (FP),Y
   SEC
   SBC   v2
   INY
   LDA   (FP),Y
   SBC   v2+1
   INY
   LDA   (FP),Y
   SBC   v2+2
   INY
   LDA   (FP),Y
   SBC   v2+3
   BVC   .cmp_novf@4
   EOR   #$80
.cmp_novf@4:
   ASL
   LDA   #$00
   ROL
   LDY   #$09
   STA   (FP),Y
   LDA   #$00
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
   CMP   #$00
   BNE   .cmp_true@5
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@5
   LDA   #$00
   JMP   .cmp_end@6
.cmp_true@5:
   LDA   #$01
.cmp_end@6:
   LDY   #$0D
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
   LDY   #$0F
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   LDY   #$10
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   LDY   #$10
   LDA   (FP),Y
   LDY   #$02
   STA   (FP),Y
.if_end@0:
   LDA   v1
   LDY   #$11
   STA   (FP),Y
   LDA   v1+1
   INY
   STA   (FP),Y
   LDA   v1+2
   INY
   STA   (FP),Y
   LDA   v1+3
   INY
   STA   (FP),Y
   LDA   v5
   STA   HARGS
   LDA   v5+1
   STA   HARGS+1
   LDA   v5+2
   STA   HARGS+2
   LDA   v5+3
   STA   HARGS+3
   LDY   #$11
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
   LDY   #$15
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
   CMP   v5+3
   BNE   .thread@0
   DEY
   LDA   (FP),Y
   CMP   v5+2
   BNE   .thread@0
   DEY
   LDA   (FP),Y
   CMP   v5+1
   BNE   .thread@0
   DEY
   LDA   (FP),Y
   CMP   v5
   BEQ   .cmp_true@8
.thread@0:
   LDA   #$00
   JMP   .cmp_end@9
.cmp_true@8:
   LDA   #$01
.cmp_end@9:
   LDY   #$19
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$1B
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$1C
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@11
   LDA   #$00
   JMP   .sx_done@12
.sx_neg@11:
   LDA   #$FF
.sx_done@12:
   LDY   #$1D
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$1B
   LDA   (FP),Y
   ORA   v1
   LDY   #$1F
   STA   (FP),Y
   LDY   #$1C
   LDA   (FP),Y
   ORA   v1+1
   LDY   #$20
   STA   (FP),Y
   LDY   #$1D
   LDA   (FP),Y
   ORA   v1+2
   LDY   #$21
   STA   (FP),Y
   LDY   #$1E
   LDA   (FP),Y
   ORA   v1+3
   LDY   #$22
   STA   (FP),Y
   LDY   #$1F
   LDA   (FP),Y
   LDY   #$23
   STA   (FP),Y
   LDY   #$20
   LDA   (FP),Y
   LDY   #$24
   STA   (FP),Y
   LDY   #$21
   LDA   (FP),Y
   LDY   #$25
   STA   (FP),Y
   LDY   #$22
   LDA   (FP),Y
   LDY   #$26
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
   CMP   #$02
   BNE   .cmp_true@13
   LDA   #$00
   JMP   .cmp_end@14
.cmp_true@13:
   LDA   #$01
.cmp_end@14:
   LDY   #$27
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
   LDY   #$29
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   LDY   #$2A
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   LDY   #$2A
   LDA   (FP),Y
   LDY   #$02
   STA   (FP),Y
.if_end@1:
   LDA   v4
   LDY   #$2B
   STA   (FP),Y
   LDA   v4
   BMI   .sx_neg@16
   LDA   #$00
   JMP   .sx_done@17
.sx_neg@16:
   LDA   #$FF
.sx_done@17:
   LDY   #$2C
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$2D
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$2E
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   HARGS
   INY
   LDA   (FP),Y
   STA   HARGS+1
   LDA   #$7F
   STA   HARGS+2
   LDA   #$00
   STA   HARGS+3
   JSR   mul16
   LDA   HARGS+4
   LDY   #$2F
   STA   (FP),Y
   LDA   HARGS+5
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$31
   STA   (FP),Y
   INY
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@18
   LDA   #$00
   JMP   .sx_done@19
.sx_neg@18:
   LDA   #$FF
.sx_done@19:
   LDY   #$33
   STA   (FP),Y
   LDA   v4
   INY
   STA   (FP),Y
   LDA   v4
   BMI   .sx_neg@20
   LDA   #$00
   JMP   .sx_done@21
.sx_neg@20:
   LDA   #$FF
.sx_done@21:
   LDY   #$35
   STA   (FP),Y
   LDA   v4
   INY
   STA   (FP),Y
   LDA   v4
   BMI   .sx_neg@22
   LDA   #$00
   JMP   .sx_done@23
.sx_neg@22:
   LDA   #$FF
.sx_done@23:
   LDY   #$37
   STA   (FP),Y
   LDY   #$34
   LDA   (FP),Y
   SEC
   LDY   #$36
   SBC   (FP),Y
   LDY   #$38
   STA   (FP),Y
   LDY   #$35
   LDA   (FP),Y
   LDY   #$37
   SBC   (FP),Y
   LDY   #$39
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$3A
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$3B
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@24
   LDA   #$00
   JMP   .sx_done@25
.sx_neg@24:
   LDA   #$FF
.sx_done@25:
   LDY   #$3C
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   v2
   ORA   v2
   INY
   STA   (FP),Y
   LDA   v2+1
   ORA   v2+1
   INY
   STA   (FP),Y
   LDA   v2+2
   ORA   v2+2
   INY
   STA   (FP),Y
   LDA   v2+3
   ORA   v2+3
   INY
   STA   (FP),Y
   LDY   #$3A
   LDA   (FP),Y
   SEC
   LDY   #$3E
   SBC   (FP),Y
   LDY   #$3B
   LDA   (FP),Y
   LDY   #$3F
   SBC   (FP),Y
   LDY   #$3C
   LDA   (FP),Y
   LDY   #$40
   SBC   (FP),Y
   LDY   #$3D
   LDA   (FP),Y
   LDY   #$41
   SBC   (FP),Y
   BVC   .cmp_novf@26
   EOR   #$80
.cmp_novf@26:
   ASL
   LDA   #$00
   ROL
   LDY   #$42
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$33
   LDA   (FP),Y
   LDY   #$43
   CMP   (FP),Y
   BNE   .cmp_true@27
   LDY   #$32
   LDA   (FP),Y
   LDY   #$42
   CMP   (FP),Y
   BNE   .cmp_true@27
   LDA   #$00
   JMP   .cmp_end@28
.cmp_true@27:
   LDA   #$01
.cmp_end@28:
   LDY   #$44
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$46
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$47
   STA   (FP),Y
   CMP   #$00
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
   LDY   #$48
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .or_true@3
   LDY   #$01
   LDA   (FP),Y
   ORA   #$04
   LDY   #$4A
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   LDY   #$4B
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   LDY   #$4B
   LDA   (FP),Y
   LDY   #$02
   STA   (FP),Y
   LDA   #$7F
   BNE   .or_true@3
   LDA   v4
   BNE   .or_true@3
   LDA   #$00
   LDY   #$4C
   STA   (FP),Y
   INY
   STA   (FP),Y
   JMP   .or_end@4
.or_true@3:
   LDA   #$01
   LDY   #$4C
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@4:
   LDY   #$4C
   LDA   (FP),Y
   LDY   #$4E
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$4F
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@33
   LDA   #$00
   JMP   .sx_done@34
.sx_neg@33:
   LDA   #$FF
.sx_done@34:
   LDY   #$50
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   v5
   STA   HARGS
   STA   HARGS+4
   LDA   v5+1
   STA   HARGS+1
   STA   HARGS+5
   LDA   v5+2
   STA   HARGS+2
   STA   HARGS+6
   LDA   v5+3
   STA   HARGS+3
   STA   HARGS+7
   JSR   mul32
   LDA   HARGS+8
   LDY   #$52
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
   LDY   #$4E
   LDA   (FP),Y
   LDY   #$52
   AND   (FP),Y
   LDY   #$56
   STA   (FP),Y
   LDY   #$4F
   LDA   (FP),Y
   LDY   #$53
   AND   (FP),Y
   LDY   #$57
   STA   (FP),Y
   LDY   #$50
   LDA   (FP),Y
   LDY   #$54
   AND   (FP),Y
   LDY   #$58
   STA   (FP),Y
   LDY   #$51
   LDA   (FP),Y
   LDY   #$55
   AND   (FP),Y
   LDY   #$59
   STA   (FP),Y
   LDA   v1
   STA   HARGS
   LDA   v1+1
   STA   HARGS+1
   LDA   v1+2
   STA   HARGS+2
   LDA   v1+3
   STA   HARGS+3
   LDY   #$56
   LDA   (FP),Y
   STA   HARGS+4
   JSR   asl32
   LDA   HARGS+5
   LDY   #$5A
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
   LDY   #$5A
   LDA   (FP),Y
   LDY   #$5E
   STA   (FP),Y
   LDY   #$5B
   LDA   (FP),Y
   LDY   #$5F
   STA   (FP),Y
   LDY   #$5C
   LDA   (FP),Y
   LDY   #$60
   STA   (FP),Y
   LDY   #$5D
   LDA   (FP),Y
   LDY   #$61
   STA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@35
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@35
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@35
   DEY
   LDA   (FP),Y
   CMP   #$04
   BNE   .cmp_true@35
   LDA   #$00
   JMP   .cmp_end@36
.cmp_true@35:
   LDA   #$01
.cmp_end@36:
   LDY   #$62
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
   LDY   #$64
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   LDY   #$65
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   LDY   #$65
   LDA   (FP),Y
   LDY   #$02
   STA   (FP),Y
.if_end@5:
   SEC
   LDA   #$00
   SBC   v5
   LDY   #$66
   STA   (FP),Y
   LDA   #$00
   SBC   v5+1
   INY
   STA   (FP),Y
   LDA   #$00
   SBC   v5+2
   INY
   STA   (FP),Y
   LDA   #$00
   SBC   v5+3
   INY
   STA   (FP),Y
   LDY   #$66
   LDA   (FP),Y
   LDY   #$6A
   STA   (FP),Y
   LDY   #$67
   LDA   (FP),Y
   LDY   #$6B
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$6C
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$6D
   STA   (FP),Y
   CMP   #$00
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
   LDY   #$6E
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
   LDY   #$70
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   LDY   #$71
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   LDY   #$71
   LDA   (FP),Y
   LDY   #$02
   STA   (FP),Y
.if_end@6:
   LDA   v1
   LDY   #$72
   STA   (FP),Y
   LDA   v1+1
   INY
   STA   (FP),Y
   LDA   v1+2
   INY
   STA   (FP),Y
   LDA   v1+3
   INY
   STA   (FP),Y
   LDA   v3
   INY
   STA   (FP),Y
   LDA   v3
   BMI   .sx_neg@41
   LDA   #$00
   JMP   .sx_done@42
.sx_neg@41:
   LDA   #$FF
.sx_done@42:
   LDY   #$77
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$78
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$79
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@43
   LDA   #$00
   JMP   .sx_done@44
.sx_neg@43:
   LDA   #$FF
.sx_done@44:
   LDY   #$7A
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   v0
   SEC
   LDY   #$78
   SBC   (FP),Y
   LDY   #$7C
   STA   (FP),Y
   LDA   v0+1
   LDY   #$79
   SBC   (FP),Y
   LDY   #$7D
   STA   (FP),Y
   LDA   v0+2
   LDY   #$7A
   SBC   (FP),Y
   LDY   #$7E
   STA   (FP),Y
   LDA   v0+3
   LDY   #$7B
   SBC   (FP),Y
   LDY   #$7F
   STA   (FP),Y
   LDY   #$7C
   LDA   (FP),Y
   EOR   #$FF
   LDY   #$80
   STA   (FP),Y
   LDY   #$7D
   LDA   (FP),Y
   EOR   #$FF
   LDY   #$81
   STA   (FP),Y
   LDY   #$7E
   LDA   (FP),Y
   EOR   #$FF
   LDY   #$82
   STA   (FP),Y
   LDY   #$7F
   LDA   (FP),Y
   EOR   #$FF
   LDY   #$83
   STA   (FP),Y
   LDY   #$80
   LDA   (FP),Y
   SEC
   LDY   #$72
   SBC   (FP),Y
   LDY   #$81
   LDA   (FP),Y
   LDY   #$73
   SBC   (FP),Y
   LDY   #$82
   LDA   (FP),Y
   LDY   #$74
   SBC   (FP),Y
   LDY   #$83
   LDA   (FP),Y
   LDY   #$75
   SBC   (FP),Y
   LDA   #$00
   ROL
   LDY   #$84
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$86
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$87
   STA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@45
   DEY
   LDA   (FP),Y
   CMP   #$01
   BNE   .cmp_true@45
   LDA   #$00
   JMP   .cmp_end@46
.cmp_true@45:
   LDA   #$01
.cmp_end@46:
   LDY   #$88
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
   LDY   #$8A
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   LDY   #$8B
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   LDY   #$8B
   LDA   (FP),Y
   LDY   #$02
   STA   (FP),Y
.if_end@7:
   LDA   v1
   LDY   #$8C
   STA   (FP),Y
   LDA   v1+1
   INY
   STA   (FP),Y
   LDA   v1+2
   INY
   STA   (FP),Y
   LDA   v1+3
   INY
   STA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@48
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@48
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@48
   DEY
   LDA   (FP),Y
   CMP   #$02
   BNE   .cmp_true@48
   LDA   #$00
   JMP   .cmp_end@49
.cmp_true@48:
   LDA   #$01
.cmp_end@49:
   LDY   #$90
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
   LDY   #$92
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   LDY   #$93
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   LDY   #$93
   LDA   (FP),Y
   LDY   #$02
   STA   (FP),Y
.if_end@8:
   LDA   #$45
   EOR   #$FF
   LDY   #$94
   STA   (FP),Y
   LDA   #$23
   EOR   #$FF
   INY
   STA   (FP),Y
   LDA   #$01
   EOR   #$FF
   INY
   STA   (FP),Y
   LDA   #$00
   EOR   #$FF
   INY
   STA   (FP),Y
   LDY   #$94
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
   LDA   v2
   STA   HARGS+4
   LDA   v2+1
   STA   HARGS+5
   LDA   v2+2
   STA   HARGS+6
   LDA   v2+3
   STA   HARGS+7
   JSR   mul32
   LDA   HARGS+8
   LDY   #$98
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
   LDY   #$98
   LDA   (FP),Y
   LDY   #$9C
   STA   (FP),Y
   LDY   #$99
   LDA   (FP),Y
   LDY   #$9D
   STA   (FP),Y
   LDY   #$9A
   LDA   (FP),Y
   LDY   #$9E
   STA   (FP),Y
   LDY   #$9B
   LDA   (FP),Y
   LDY   #$9F
   STA   (FP),Y
   LDA   v1
   INY
   STA   (FP),Y
   LDA   v1+1
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$A2
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$A3
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   v5
   SEC
   LDY   #$A2
   SBC   (FP),Y
   LDY   #$A6
   STA   (FP),Y
   LDA   v5+1
   LDY   #$A3
   SBC   (FP),Y
   LDY   #$A7
   STA   (FP),Y
   LDA   v5+2
   LDY   #$A4
   SBC   (FP),Y
   LDY   #$A8
   STA   (FP),Y
   LDA   v5+3
   LDY   #$A5
   SBC   (FP),Y
   LDY   #$A9
   STA   (FP),Y
   LDY   #$9F
   LDA   (FP),Y
   LDY   #$A9
   CMP   (FP),Y
   BNE   .cmp_true@51
   LDY   #$9E
   LDA   (FP),Y
   LDY   #$A8
   CMP   (FP),Y
   BNE   .cmp_true@51
   LDY   #$9D
   LDA   (FP),Y
   LDY   #$A7
   CMP   (FP),Y
   BNE   .cmp_true@51
   LDY   #$9C
   LDA   (FP),Y
   LDY   #$A6
   CMP   (FP),Y
   BNE   .cmp_true@51
   LDA   #$00
   JMP   .cmp_end@52
.cmp_true@51:
   LDA   #$01
.cmp_end@52:
   LDY   #$AA
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$AC
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$AD
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
   LDY   #$AE
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
   ADC   #$B3
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$B2
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
   DC.L  $822A87DC

v1:
   DC.L  $00000002

v2:
   DC.L  $00000001

v3:
   DC.B  $01

v4:
   DC.B  $01

v5:
   DC.L  $FFFFFFFF
