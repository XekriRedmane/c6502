SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24

check:
   SUBROUTINE

   ; prologue: 16 arg bytes, 206 local bytes
   SEC
   LDA   SSP
   SBC   #$D0
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$CF
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
   LDY   #$D9
   LDA   (FP),Y
   LDY   #$03
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDA   #$80
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$04
   LDA   (FP),Y
   LDY   #$07
   CMP   (FP),Y
   BNE   .cmp_true@0
   LDA   #$00
   JMP   .cmp_end@1
.cmp_true@0:
   LDA   #$01
.cmp_end@1:
   LDY   #$09
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
   LDY   #$0B
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   LDY   #$0C
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   LDY   #$0C
   LDA   (FP),Y
   LDY   #$02
   STA   (FP),Y
.if_end@0:
   SEC
   LDA   #$00
   SBC   #$00
   LDY   #$0D
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
   SBC   #$80
   INY
   STA   (FP),Y
   LDY   #$D8
   LDA   (FP),Y
   LDY   #$11
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@2
   LDA   #$00
   JMP   .sx_done@3
.sx_neg@2:
   LDA   #$FF
.sx_done@3:
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
   ORA   #$00
   BMI   .sx_neg@4
   LDA   #$00
   JMP   .sx_done@5
.sx_neg@4:
   LDA   #$FF
.sx_done@5:
   LDY   #$15
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$DA
   LDA   (FP),Y
   LDY   #$13
   AND   (FP),Y
   LDY   #$17
   STA   (FP),Y
   LDY   #$DB
   LDA   (FP),Y
   LDY   #$14
   AND   (FP),Y
   LDY   #$18
   STA   (FP),Y
   LDY   #$DC
   LDA   (FP),Y
   LDY   #$15
   AND   (FP),Y
   LDY   #$19
   STA   (FP),Y
   LDY   #$DD
   LDA   (FP),Y
   LDY   #$16
   AND   (FP),Y
   LDY   #$1A
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
   LDY   #$D1
   LDA   (FP),Y
   STA   HARGS+4
   JSR   lsr32
   LDA   HARGS+5
   LDY   #$1B
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
   LDY   #$0D
   LDA   (FP),Y
   SEC
   LDY   #$1B
   SBC   (FP),Y
   LDY   #$0E
   LDA   (FP),Y
   LDY   #$1C
   SBC   (FP),Y
   LDY   #$0F
   LDA   (FP),Y
   LDY   #$1D
   SBC   (FP),Y
   LDY   #$10
   LDA   (FP),Y
   LDY   #$1E
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
   LDY   #$21
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$22
   STA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@6
   DEY
   LDA   (FP),Y
   CMP   #$01
   BNE   .cmp_true@6
   LDA   #$00
   JMP   .cmp_end@7
.cmp_true@6:
   LDA   #$01
.cmp_end@7:
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
   LDY   #$E0
   LDA   (FP),Y
   LDY   #$27
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@9
   LDA   #$00
   JMP   .sx_done@10
.sx_neg@9:
   LDA   #$FF
.sx_done@10:
   LDY   #$28
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$29
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$2A
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@11
   LDA   #$00
   JMP   .sx_done@12
.sx_neg@11:
   LDA   #$FF
.sx_done@12:
   LDY   #$2B
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$29
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
   LDY   #$DA
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
   LDY   #$2D
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
   LDY   #$DA
   LDA   (FP),Y
   CLC
   LDY   #$2D
   SBC   (FP),Y
   LDY   #$DB
   LDA   (FP),Y
   LDY   #$2E
   SBC   (FP),Y
   LDY   #$DC
   LDA   (FP),Y
   LDY   #$2F
   SBC   (FP),Y
   LDY   #$DD
   LDA   (FP),Y
   LDY   #$30
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
   LDY   #$33
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$34
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@13
   LDA   #$00
   JMP   .sx_done@14
.sx_neg@13:
   LDA   #$FF
.sx_done@14:
   LDY   #$35
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$DA
   LDA   (FP),Y
   CLC
   LDY   #$33
   SBC   (FP),Y
   LDY   #$DB
   LDA   (FP),Y
   LDY   #$34
   SBC   (FP),Y
   LDY   #$DC
   LDA   (FP),Y
   LDY   #$35
   SBC   (FP),Y
   LDY   #$DD
   LDA   (FP),Y
   LDY   #$36
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
   LDY   #$39
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$3A
   STA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@15
   DEY
   LDA   (FP),Y
   CMP   #$01
   BNE   .cmp_true@15
   LDA   #$00
   JMP   .cmp_end@16
.cmp_true@15:
   LDA   #$01
.cmp_end@16:
   LDY   #$3B
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
   LDY   #$3D
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   LDY   #$3E
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   LDY   #$3E
   LDA   (FP),Y
   LDY   #$02
   STA   (FP),Y
   LDA   #$FF
   BNE   .or_true@3
   LDY   #$D1
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
   LDY   #$3F
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@3
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   JMP   .or_end@4
.or_true@3:
   LDA   #$01
   LDY   #$41
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@4:
   LDY   #$41
   LDA   (FP),Y
   LDY   #$43
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$44
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
   LDY   #$45
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
   LDY   #$47
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   LDY   #$48
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   LDY   #$48
   LDA   (FP),Y
   LDY   #$02
   STA   (FP),Y
.if_end@5:
   LDY   #$E0
   LDA   (FP),Y
   LDY   #$49
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@22
   LDA   #$00
   JMP   .sx_done@23
.sx_neg@22:
   LDA   #$FF
.sx_done@23:
   LDY   #$4A
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$4B
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$4C
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@24
   LDA   #$00
   JMP   .sx_done@25
.sx_neg@24:
   LDA   #$FF
.sx_done@25:
   LDY   #$4D
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$4B
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
   LDY   #$DA
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
   LDY   #$4F
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
   LDY   #$D8
   LDA   (FP),Y
   LDY   #$53
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@26
   LDA   #$00
   JMP   .sx_done@27
.sx_neg@26:
   LDA   #$FF
.sx_done@27:
   LDY   #$54
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$55
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$56
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@28
   LDA   #$00
   JMP   .sx_done@29
.sx_neg@28:
   LDA   #$FF
.sx_done@29:
   LDY   #$57
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$4F
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
   LDY   #$55
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
   LDY   #$59
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
   LDY   #$59
   LDA   (FP),Y
   EOR   #$FF
   LDY   #$5D
   STA   (FP),Y
   LDY   #$5A
   LDA   (FP),Y
   EOR   #$FF
   LDY   #$5E
   STA   (FP),Y
   LDY   #$5B
   LDA   (FP),Y
   EOR   #$FF
   LDY   #$5F
   STA   (FP),Y
   LDY   #$5C
   LDA   (FP),Y
   EOR   #$FF
   LDY   #$60
   STA   (FP),Y
   LDY   #$5D
   LDA   (FP),Y
   LDY   #$61
   STA   (FP),Y
   LDY   #$5E
   LDA   (FP),Y
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
   CMP   #$FF
   BNE   .cmp_true@30
   DEY
   LDA   (FP),Y
   CMP   #$FF
   BNE   .cmp_true@30
   DEY
   LDA   (FP),Y
   CMP   #$FF
   BNE   .cmp_true@30
   DEY
   LDA   (FP),Y
   CMP   #$FF
   BNE   .cmp_true@30
   LDA   #$00
   JMP   .cmp_end@31
.cmp_true@30:
   LDA   #$01
.cmp_end@31:
   LDY   #$65
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
   LDY   #$67
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   LDY   #$68
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   LDY   #$68
   LDA   (FP),Y
   LDY   #$02
   STA   (FP),Y
.if_end@6:
   LDY   #$E0
   LDA   (FP),Y
   LDY   #$69
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@33
   LDA   #$00
   JMP   .sx_done@34
.sx_neg@33:
   LDA   #$FF
.sx_done@34:
   LDY   #$6A
   STA   (FP),Y
   LDA   #$01
   CLC
   SBC   #$FF
   LDA   #$00
   SBC   #$7F
   LDA   #$00
   SBC   #$00
   LDA   #$00
   SBC   #$00
   LDA   #$00
   ROL
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$6D
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$6E
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@35
   LDA   #$00
   JMP   .sx_done@36
.sx_neg@35:
   LDA   #$FF
.sx_done@36:
   LDY   #$6F
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$D1
   LDA   (FP),Y
   SEC
   LDY   #$6D
   SBC   (FP),Y
   LDY   #$D2
   LDA   (FP),Y
   LDY   #$6E
   SBC   (FP),Y
   LDY   #$D3
   LDA   (FP),Y
   LDY   #$6F
   SBC   (FP),Y
   LDY   #$D4
   LDA   (FP),Y
   LDY   #$70
   SBC   (FP),Y
   LDY   #$D5
   LDA   (FP),Y
   LDY   #$71
   SBC   (FP),Y
   LDY   #$D6
   LDA   (FP),Y
   LDY   #$72
   SBC   (FP),Y
   LDY   #$D7
   LDA   (FP),Y
   LDY   #$73
   SBC   (FP),Y
   BVC   .cmp_novf@37
   EOR   #$80
.cmp_novf@37:
   ASL
   LDA   #$00
   ROL
   LDY   #$74
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$69
   LDA   (FP),Y
   LDY   #$76
   STA   (FP),Y
   LDY   #$6A
   LDA   (FP),Y
   LDY   #$77
   STA   (FP),Y
   LDY   #$74
   LDA   (FP),Y
   TAX
   BEQ   .shift_done@39
.shift_loop@38:
   LDY   #$77
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
   BNE   .shift_loop@38
.shift_done@39:
   LDY   #$76
   LDA   (FP),Y
   LDY   #$78
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$79
   STA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@40
   DEY
   LDA   (FP),Y
   CMP   #$01
   BNE   .cmp_true@40
   LDA   #$00
   JMP   .cmp_end@41
.cmp_true@40:
   LDA   #$01
.cmp_end@41:
   LDY   #$7A
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
.if_end@7:
   LDA   #$FF
   LDY   #$7E
   STA   (FP),Y
   INY
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@43
   LDA   #$00
   JMP   .sx_done@44
.sx_neg@43:
   LDA   #$FF
.sx_done@44:
   LDY   #$80
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$81
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$82
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@45
   LDA   #$00
   JMP   .sx_done@46
.sx_neg@45:
   LDA   #$FF
.sx_done@46:
   LDY   #$83
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$D7
   CMP   (FP),Y
   BNE   .thread@0
   DEY
   CMP   (FP),Y
   BNE   .thread@0
   DEY
   CMP   (FP),Y
   BNE   .thread@0
   DEY
   CMP   (FP),Y
   BNE   .thread@0
   DEY
   CMP   (FP),Y
   BNE   .thread@0
   LDY   #$82
   LDA   (FP),Y
   LDY   #$D2
   CMP   (FP),Y
   BNE   .thread@0
   LDY   #$81
   LDA   (FP),Y
   LDY   #$D1
   CMP   (FP),Y
   BEQ   .cmp_true@47
.thread@0:
   LDA   #$00
   JMP   .cmp_end@48
.cmp_true@47:
   LDA   #$01
.cmp_end@48:
   LDY   #$88
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$8A
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$8B
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@50
   LDA   #$00
   JMP   .sx_done@51
.sx_neg@50:
   LDA   #$FF
.sx_done@51:
   LDY   #$8C
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$D8
   LDA   (FP),Y
   LDY   #$8E
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@52
   LDA   #$00
   JMP   .sx_done@53
.sx_neg@52:
   LDA   #$FF
.sx_done@53:
   LDY   #$8F
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$90
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$91
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@54
   LDA   #$00
   JMP   .sx_done@55
.sx_neg@54:
   LDA   #$FF
.sx_done@55:
   LDY   #$92
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$90
   LDA   (FP),Y
   CLC
   LDY   #$DA
   SBC   (FP),Y
   LDY   #$91
   LDA   (FP),Y
   LDY   #$DB
   SBC   (FP),Y
   LDY   #$92
   LDA   (FP),Y
   LDY   #$DC
   SBC   (FP),Y
   LDY   #$93
   LDA   (FP),Y
   LDY   #$DD
   SBC   (FP),Y
   LDA   #$00
   ROL
   LDY   #$94
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$96
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$99
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$9A
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@56
   LDA   #$00
   JMP   .sx_done@57
.sx_neg@56:
   LDA   #$FF
.sx_done@57:
   LDY   #$9B
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$8D
   LDA   (FP),Y
   LDY   #$9C
   CMP   (FP),Y
   BNE   .thread@1
   DEY
   CMP   (FP),Y
   BNE   .thread@1
   LDY   #$8B
   LDA   (FP),Y
   LDY   #$9A
   CMP   (FP),Y
   BNE   .thread@1
   LDY   #$8A
   LDA   (FP),Y
   LDY   #$99
   CMP   (FP),Y
   BEQ   .cmp_true@58
.thread@1:
   LDA   #$00
   JMP   .cmp_end@59
.cmp_true@58:
   LDA   #$01
.cmp_end@59:
   LDY   #$9D
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$9F
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$A0
   STA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@61
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@61
   LDA   #$00
   JMP   .cmp_end@62
.cmp_true@61:
   LDA   #$01
.cmp_end@62:
   LDY   #$A1
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
   LDY   #$A3
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   LDY   #$A4
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   LDY   #$A4
   LDA   (FP),Y
   LDY   #$02
   STA   (FP),Y
.if_end@8:
   LDY   #$D9
   LDA   (FP),Y
   LDY   #$A5
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$A7
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$A8
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@64
   LDA   #$00
   JMP   .sx_done@65
.sx_neg@64:
   LDA   #$FF
.sx_done@65:
   LDY   #$A9
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$D1
   LDA   (FP),Y
   LDY   #$A7
   EOR   (FP),Y
   LDY   #$AE
   STA   (FP),Y
   LDY   #$D2
   LDA   (FP),Y
   LDY   #$A8
   EOR   (FP),Y
   LDY   #$AF
   STA   (FP),Y
   LDY   #$D3
   LDA   (FP),Y
   LDY   #$A9
   EOR   (FP),Y
   LDY   #$B0
   STA   (FP),Y
   LDY   #$D4
   LDA   (FP),Y
   LDY   #$AA
   EOR   (FP),Y
   LDY   #$B1
   STA   (FP),Y
   LDY   #$D5
   LDA   (FP),Y
   LDY   #$AB
   EOR   (FP),Y
   LDY   #$B2
   STA   (FP),Y
   LDY   #$D6
   LDA   (FP),Y
   LDY   #$AC
   EOR   (FP),Y
   LDY   #$B3
   STA   (FP),Y
   LDY   #$D7
   LDA   (FP),Y
   LDY   #$AD
   EOR   (FP),Y
   LDY   #$B4
   STA   (FP),Y
   LDY   #$AE
   LDA   (FP),Y
   LDY   #$B5
   STA   (FP),Y
   LDY   #$AF
   LDA   (FP),Y
   LDY   #$B6
   STA   (FP),Y
   LDY   #$B0
   LDA   (FP),Y
   LDY   #$B7
   STA   (FP),Y
   LDY   #$B1
   LDA   (FP),Y
   LDY   #$B8
   STA   (FP),Y
   LDY   #$D9
   LDA   (FP),Y
   BEQ   .and_false@9
   LDA   #$01
   LDY   #$B9
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   JMP   .and_end@10
.and_false@9:
   LDA   #$00
   LDY   #$B9
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@10:
   LDY   #$D8
   LDA   (FP),Y
   LDY   #$BB
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@66
   LDA   #$00
   JMP   .sx_done@67
.sx_neg@66:
   LDA   #$FF
.sx_done@67:
   LDY   #$BC
   STA   (FP),Y
   LDY   #$E0
   LDA   (FP),Y
   LDY   #$BD
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@68
   LDA   #$00
   JMP   .sx_done@69
.sx_neg@68:
   LDA   #$FF
.sx_done@69:
   LDY   #$BE
   STA   (FP),Y
   LDY   #$BB
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
   LDA   HARGS+6
   LDY   #$BF
   STA   (FP),Y
   LDA   HARGS+7
   INY
   STA   (FP),Y
   LDY   #$B9
   LDA   (FP),Y
   LDY   #$BF
   EOR   (FP),Y
   LDY   #$C1
   STA   (FP),Y
   LDY   #$BA
   LDA   (FP),Y
   LDY   #$C0
   EOR   (FP),Y
   LDY   #$C2
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$C3
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$C4
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@70
   LDA   #$00
   JMP   .sx_done@71
.sx_neg@70:
   LDA   #$FF
.sx_done@71:
   LDY   #$C5
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$B5
   LDA   (FP),Y
   CLC
   LDY   #$C3
   SBC   (FP),Y
   LDY   #$B6
   LDA   (FP),Y
   LDY   #$C4
   SBC   (FP),Y
   LDY   #$B7
   LDA   (FP),Y
   LDY   #$C5
   SBC   (FP),Y
   LDY   #$B8
   LDA   (FP),Y
   LDY   #$C6
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
   LDY   #$C9
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$CA
   STA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@72
   DEY
   LDA   (FP),Y
   CMP   #$01
   BNE   .cmp_true@72
   LDA   #$00
   JMP   .cmp_end@73
.cmp_true@72:
   LDA   #$01
.cmp_end@73:
   LDY   #$CB
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
   LDY   #$CD
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   LDY   #$CE
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   LDY   #$CE
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
   ADC   #$E0
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$CF
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

   ; prologue: 0 arg bytes, 7 local bytes
   SEC
   LDA   SSP
   SBC   #$09
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$08
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   LDA   #$80
   LDY   #$01
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDA   #$80
   INY
   STA   (FP),Y
   LDA   #$01
   INY
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$10
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   #$00
   LDY   #$01
   STA   (SSP),Y
   INY
   STA   (SSP),Y
   INY
   STA   (SSP),Y
   INY
   STA   (SSP),Y
   INY
   STA   (SSP),Y
   INY
   STA   (SSP),Y
   INY
   STA   (SSP),Y
   LDY   #$01
   LDA   (FP),Y
   LDY   #$08
   STA   (SSP),Y
   LDY   #$02
   LDA   (FP),Y
   LDY   #$09
   STA   (SSP),Y
   LDA   #$01
   INY
   STA   (SSP),Y
   LDA   #$00
   INY
   STA   (SSP),Y
   INY
   STA   (SSP),Y
   INY
   STA   (SSP),Y
   LDY   #$03
   LDA   (FP),Y
   LDY   #$0E
   STA   (SSP),Y
   LDY   #$04
   LDA   (FP),Y
   LDY   #$0F
   STA   (SSP),Y
   LDY   #$05
   LDA   (FP),Y
   LDY   #$10
   STA   (SSP),Y
   JSR   check
   LDY   #$06
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
   ADC   #$09
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$08
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
