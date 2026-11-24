SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24

check:
   SUBROUTINE

   ; prologue: 0 arg bytes, 141 local bytes
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
   INY
   STA   (FP),Y
   LDA   #$01
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$04
   LDA   (FP),Y
   CMP   #$01
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
   LDA   v5
   LDY   #$0B
   STA   (FP),Y
   LDA   v5+1
   INY
   STA   (FP),Y
   LDA   #$02
   BNE   .or_true@1
   LDA   v2
   ORA   v2+1
   ORA   v2+2
   ORA   v2+3
   BNE   .or_true@1
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   JMP   .or_end@2
.or_true@1:
   LDA   #$01
   LDY   #$0D
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@2:
   LDY   #$0B
   LDA   (FP),Y
   LDY   #$0F
   STA   (FP),Y
   LDY   #$0C
   LDA   (FP),Y
   LDY   #$10
   STA   (FP),Y
   LDY   #$0D
   LDA   (FP),Y
   TAX
   BEQ   .shift_done@4
.shift_loop@3:
   LDY   #$10
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
   BNE   .shift_loop@3
.shift_done@4:
   SEC
   LDA   #$00
   LDY   #$0F
   SBC   (FP),Y
   LDY   #$11
   STA   (FP),Y
   LDA   #$00
   DEY
   SBC   (FP),Y
   LDY   #$12
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$13
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$14
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
   LDY   #$15
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
   LDY   #$17
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   LDY   #$18
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   LDY   #$18
   LDA   (FP),Y
   LDY   #$02
   STA   (FP),Y
.if_end@3:
   LDA   v3
   LDY   #$19
   STA   (FP),Y
   LDA   v3
   BMI   .sx_neg@8
   LDA   #$00
   JMP   .sx_done@9
.sx_neg@8:
   LDA   #$FF
.sx_done@9:
   LDY   #$1A
   STA   (FP),Y
   LDA   v2
   CLC
   SBC   v2
   LDA   v2+1
   SBC   v2+1
   LDA   v2+2
   SBC   v2+2
   LDA   v2+3
   SBC   v2+3
   LDA   #$00
   ROL
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDA   v1
   INY
   STA   (FP),Y
   LDA   v1
   BMI   .sx_neg@10
   LDA   #$00
   JMP   .sx_done@11
.sx_neg@10:
   LDA   #$FF
.sx_done@11:
   LDY   #$1E
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$1D
   LDA   (FP),Y
   SEC
   SBC   v0
   INY
   LDA   (FP),Y
   SBC   v0+1
   INY
   LDA   (FP),Y
   SBC   v0+2
   INY
   LDA   (FP),Y
   SBC   v0+3
   BVC   .cmp_novf@12
   EOR   #$80
.cmp_novf@12:
   ASL
   LDA   #$00
   ROL
   EOR   #$01
   LDY   #$21
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$1B
   LDA   (FP),Y
   LDY   #$23
   STA   (FP),Y
   LDY   #$1C
   LDA   (FP),Y
   LDY   #$24
   STA   (FP),Y
   LDY   #$21
   LDA   (FP),Y
   TAX
   BEQ   .shift_done@14
.shift_loop@13:
   LDY   #$23
   LDA   (FP),Y
   ASL
   STA   (FP),Y
   INY
   LDA   (FP),Y
   ROL
   STA   (FP),Y
   DEX
   BNE   .shift_loop@13
.shift_done@14:
   LDY   #$19
   LDA   (FP),Y
   SEC
   LDY   #$23
   SBC   (FP),Y
   LDY   #$1A
   LDA   (FP),Y
   LDY   #$24
   SBC   (FP),Y
   BVC   .cmp_novf@15
   EOR   #$80
.cmp_novf@15:
   ASL
   LDA   #$00
   ROL
   LDY   #$25
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$27
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$28
   STA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@16
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@16
   LDA   #$00
   JMP   .cmp_end@17
.cmp_true@16:
   LDA   #$01
.cmp_end@17:
   LDY   #$29
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
.if_end@4:
   LDA   v0
   LDY   #$2D
   STA   (FP),Y
   LDA   v0+1
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$2F
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   EOR   #$01
   LDY   #$30
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$31
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$32
   STA   (FP),Y
   LDA   #$1D
   INY
   STA   (FP),Y
   LDA   #$F5
   INY
   STA   (FP),Y
   LDY   #$32
   LDA   (FP),Y
   LDY   #$34
   CMP   (FP),Y
   BNE   .cmp_true@19
   LDY   #$31
   LDA   (FP),Y
   LDY   #$33
   CMP   (FP),Y
   BNE   .cmp_true@19
   LDA   #$00
   JMP   .cmp_end@20
.cmp_true@19:
   LDA   #$01
.cmp_end@20:
   LDY   #$35
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
   LDY   #$37
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   LDY   #$38
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   LDY   #$38
   LDA   (FP),Y
   LDY   #$02
   STA   (FP),Y
.if_end@5:
   LDA   #$02
   CLC
   SBC   v4
   LDA   #$00
   SBC   v4+1
   LDA   #$00
   SBC   v4+2
   LDA   #$00
   SBC   v4+3
   LDA   #$00
   ROL
   LDY   #$39
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$3B
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$3C
   STA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@22
   DEY
   LDA   (FP),Y
   CMP   #$01
   BNE   .cmp_true@22
   LDA   #$00
   JMP   .cmp_end@23
.cmp_true@22:
   LDA   #$01
.cmp_end@23:
   LDY   #$3D
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
   LDY   #$3F
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   LDY   #$40
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   LDY   #$40
   LDA   (FP),Y
   LDY   #$02
   STA   (FP),Y
.if_end@6:
   LDA   v1
   LDY   #$41
   STA   (FP),Y
   LDA   v1
   BMI   .sx_neg@25
   LDA   #$00
   JMP   .sx_done@26
.sx_neg@25:
   LDA   #$FF
.sx_done@26:
   LDY   #$42
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   CMP   v0+3
   BNE   .thread@0
   CMP   v0+2
   BNE   .thread@0
   CMP   v0+1
   BNE   .thread@0
   LDY   #$41
   LDA   (FP),Y
   CMP   v0
   BEQ   .cmp_true@27
.thread@0:
   LDA   #$00
   JMP   .cmp_end@28
.cmp_true@27:
   LDA   #$01
.cmp_end@28:
   LDY   #$45
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDA   v5
   INY
   STA   (FP),Y
   LDA   v5+1
   INY
   STA   (FP),Y
   LDA   v5+2
   INY
   STA   (FP),Y
   LDA   v5+3
   INY
   STA   (FP),Y
   LDA   #$00
   CLC
   LDY   #$47
   SBC   (FP),Y
   LDA   #$00
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
   SEC
   LDY   #$45
   SBC   (FP),Y
   LDY   #$4C
   LDA   (FP),Y
   LDY   #$46
   SBC   (FP),Y
   BVC   .cmp_novf@30
   EOR   #$80
.cmp_novf@30:
   ASL
   LDA   #$00
   ROL
   LDY   #$4D
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$4F
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$50
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@31
   LDA   #$00
   JMP   .sx_done@32
.sx_neg@31:
   LDA   #$FF
.sx_done@32:
   LDY   #$51
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$80
   INY
   STA   (FP),Y
   ORA   #$00
   BEQ   .and_false@7
   LDA   v2
   ORA   v2+1
   ORA   v2+2
   ORA   v2+3
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
   LDY   #$54
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@8:
   LDY   #$54
   LDA   (FP),Y
   LDY   #$56
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$57
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
   INY
   STA   (FP),Y
   LDA   v2
   INY
   STA   (FP),Y
   LDA   v2+1
   INY
   STA   (FP),Y
   LDA   v2+2
   INY
   STA   (FP),Y
   LDA   v2+3
   INY
   STA   (FP),Y
   LDY   #$56
   LDA   (FP),Y
   LDY   #$5A
   EOR   (FP),Y
   LDY   #$5E
   STA   (FP),Y
   LDY   #$57
   LDA   (FP),Y
   LDY   #$5B
   EOR   (FP),Y
   LDY   #$5F
   STA   (FP),Y
   LDY   #$58
   LDA   (FP),Y
   LDY   #$5C
   EOR   (FP),Y
   LDY   #$60
   STA   (FP),Y
   LDY   #$59
   LDA   (FP),Y
   LDY   #$5D
   EOR   (FP),Y
   LDY   #$61
   STA   (FP),Y
   LDY   #$52
   LDA   (FP),Y
   LDY   #$61
   CMP   (FP),Y
   BNE   .cmp_true@35
   LDY   #$51
   LDA   (FP),Y
   LDY   #$60
   CMP   (FP),Y
   BNE   .cmp_true@35
   LDY   #$50
   LDA   (FP),Y
   LDY   #$5F
   CMP   (FP),Y
   BNE   .cmp_true@35
   LDY   #$4F
   LDA   (FP),Y
   LDY   #$5E
   CMP   (FP),Y
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
   LDY   #$64
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$65
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
   LDY   #$66
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
   LDY   #$68
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   LDY   #$69
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   LDY   #$69
   LDA   (FP),Y
   LDY   #$02
   STA   (FP),Y
.if_end@9:
   LDA   v3
   LDY   #$6A
   STA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@41
   LDA   #$00
   JMP   .cmp_end@42
.cmp_true@41:
   LDA   #$01
.cmp_end@42:
   LDY   #$6B
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
   LDY   #$6D
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   LDY   #$6E
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   LDY   #$6E
   LDA   (FP),Y
   LDY   #$02
   STA   (FP),Y
.if_end@10:
   LDA   #$01
   SEC
   SBC   v2
   LDY   #$6F
   STA   (FP),Y
   LDA   #$00
   SBC   v2+1
   INY
   STA   (FP),Y
   LDA   #$00
   SBC   v2+2
   INY
   STA   (FP),Y
   LDA   #$00
   SBC   v2+3
   INY
   STA   (FP),Y
   LDA   v0
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$76
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$77
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@43
   LDA   #$00
   JMP   .sx_done@44
.sx_neg@43:
   LDA   #$FF
.sx_done@44:
   LDY   #$78
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$76
   LDA   (FP),Y
   SEC
   LDY   #$6F
   SBC   (FP),Y
   LDY   #$77
   LDA   (FP),Y
   LDY   #$70
   SBC   (FP),Y
   LDY   #$78
   LDA   (FP),Y
   LDY   #$71
   SBC   (FP),Y
   LDY   #$79
   LDA   (FP),Y
   LDY   #$72
   SBC   (FP),Y
   LDA   #$00
   ROL
   LDY   #$7A
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
   BMI   .sx_neg@45
   LDA   #$00
   JMP   .sx_done@46
.sx_neg@45:
   LDA   #$FF
.sx_done@46:
   LDY   #$7E
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   v4
   CLC
   SBC   #$07
   LDA   v4+1
   SBC   #$00
   LDA   v4+2
   SBC   #$00
   LDA   v4+3
   SBC   #$00
   LDA   #$00
   ROL
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDA   v4
   STA   HARGS
   LDA   v4+1
   STA   HARGS+1
   LDA   v4+2
   STA   HARGS+2
   LDA   v4+3
   STA   HARGS+3
   DEY
   LDA   (FP),Y
   STA   HARGS+4
   JSR   lsr32
   LDA   HARGS+5
   LDY   #$82
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
   LDY   #$7C
   LDA   (FP),Y
   CLC
   LDY   #$82
   SBC   (FP),Y
   LDY   #$7D
   LDA   (FP),Y
   LDY   #$83
   SBC   (FP),Y
   LDY   #$7E
   LDA   (FP),Y
   LDY   #$84
   SBC   (FP),Y
   LDY   #$7F
   LDA   (FP),Y
   LDY   #$85
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
   LDY   #$88
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$89
   STA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@47
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@47
   LDA   #$00
   JMP   .cmp_end@48
.cmp_true@47:
   LDA   #$01
.cmp_end@48:
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
   ADC   #$8F
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
   DC.L  $E9FFF41D

v1:
   DS.B  1

v2:
   DC.L  $FFFFFFFF

v3:
   DS.B  1

v4:
   DC.L  $00000001

v5:
   DC.L  $FFFFFFFF
