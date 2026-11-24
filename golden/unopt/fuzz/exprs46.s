SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24

check:
   SUBROUTINE

   ; prologue: 18 arg bytes, 188 local bytes
   SEC
   LDA   SSP
   SBC   #$BE
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$BD
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
   LDA   #$45
   INY
   STA   (FP),Y
   LDA   #$23
   INY
   STA   (FP),Y
   LDA   #$01
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CMP   #$01
   BNE   .cmp_true@0
   DEY
   LDA   (FP),Y
   CMP   #$23
   BNE   .cmp_true@0
   DEY
   LDA   (FP),Y
   CMP   #$45
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
   LDY   #$BF
   LDA   (FP),Y
   LDY   #$0B
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@3
   LDA   #$00
   JMP   .sx_done@4
.sx_neg@3:
   LDA   #$FF
.sx_done@4:
   LDY   #$0C
   STA   (FP),Y
   LDY   #$CC
   LDA   (FP),Y
   LDY   #$0D
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@5
   LDA   #$00
   JMP   .sx_done@6
.sx_neg@5:
   LDA   #$FF
.sx_done@6:
   LDY   #$0E
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
   BMI   .sx_neg@7
   LDA   #$00
   JMP   .sx_done@8
.sx_neg@7:
   LDA   #$FF
.sx_done@8:
   LDY   #$11
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$CB
   LDA   (FP),Y
   LDY   #$12
   CMP   (FP),Y
   BNE   .thread@0
   LDY   #$CA
   LDA   (FP),Y
   LDY   #$11
   CMP   (FP),Y
   BNE   .thread@0
   LDY   #$C9
   LDA   (FP),Y
   LDY   #$10
   CMP   (FP),Y
   BNE   .thread@0
   LDY   #$C8
   LDA   (FP),Y
   LDY   #$0F
   CMP   (FP),Y
   BEQ   .cmp_true@9
.thread@0:
   LDA   #$00
   JMP   .cmp_end@10
.cmp_true@9:
   LDA   #$01
.cmp_end@10:
   LDY   #$13
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$0B
   LDA   (FP),Y
   LDY   #$15
   STA   (FP),Y
   LDY   #$0C
   LDA   (FP),Y
   LDY   #$16
   STA   (FP),Y
   LDY   #$13
   LDA   (FP),Y
   TAX
   BEQ   .shift_done@13
.shift_loop@12:
   LDY   #$15
   LDA   (FP),Y
   ASL
   STA   (FP),Y
   INY
   LDA   (FP),Y
   ROL
   STA   (FP),Y
   DEX
   BNE   .shift_loop@12
.shift_done@13:
   LDY   #$15
   LDA   (FP),Y
   LDY   #$17
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
   LDY   #$19
   LDA   (FP),Y
   LDY   #$1C
   CMP   (FP),Y
   BNE   .cmp_true@14
   LDA   #$00
   JMP   .cmp_end@15
.cmp_true@14:
   LDA   #$01
.cmp_end@15:
   LDY   #$1E
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
   LDY   #$20
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   LDY   #$21
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   LDY   #$21
   LDA   (FP),Y
   LDY   #$02
   STA   (FP),Y
.if_end@1:
   LDY   #$BF
   LDA   (FP),Y
   LDY   #$22
   STA   (FP),Y
   INY
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@16
   LDA   #$00
   JMP   .sx_done@17
.sx_neg@16:
   LDA   #$FF
.sx_done@17:
   LDY   #$24
   STA   (FP),Y
   LDA   #$01
   INY
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
   LDY   #$27
   STA   (FP),Y
   LDY   #$24
   LDA   (FP),Y
   LDY   #$27
   CMP   (FP),Y
   BNE   .cmp_true@20
   LDY   #$23
   LDA   (FP),Y
   LDY   #$26
   CMP   (FP),Y
   BNE   .cmp_true@20
   LDA   #$00
   JMP   .cmp_end@21
.cmp_true@20:
   LDA   #$01
.cmp_end@21:
   LDY   #$28
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
   LDY   #$2A
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   LDY   #$2B
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   LDY   #$2B
   LDA   (FP),Y
   LDY   #$02
   STA   (FP),Y
.if_end@2:
   LDA   #$00
   STA   HARGS
   LDY   #$80
   STY   HARGS+1
   LDY   #$00
   STY   HARGS+2
   STA   HARGS+3
   LDY   #$80
   STY   HARGS+4
   LDY   #$00
   STY   HARGS+5
   STY   HARGS+6
   STA   HARGS+7
   JSR   mul32
   LDA   HARGS+8
   LDY   #$2C
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
   LDY   #$2C
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   INY
   ORA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@3
   LDY   #$C0
   LDA   (FP),Y
   LDY   #$30
   STA   (FP),Y
   LDY   #$C1
   LDA   (FP),Y
   LDY   #$31
   STA   (FP),Y
   LDY   #$C2
   LDA   (FP),Y
   LDY   #$32
   STA   (FP),Y
   LDY   #$C3
   LDA   (FP),Y
   LDY   #$33
   STA   (FP),Y
   LDY   #$30
   LDA   (FP),Y
   LDY   #$CD
   AND   (FP),Y
   LDY   #$34
   STA   (FP),Y
   LDY   #$31
   LDA   (FP),Y
   LDY   #$CE
   AND   (FP),Y
   LDY   #$35
   STA   (FP),Y
   LDY   #$32
   LDA   (FP),Y
   LDY   #$CF
   AND   (FP),Y
   LDY   #$36
   STA   (FP),Y
   LDY   #$33
   LDA   (FP),Y
   LDY   #$D0
   AND   (FP),Y
   LDY   #$37
   STA   (FP),Y
   LDY   #$34
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   INY
   ORA   (FP),Y
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
   LDY   #$38
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@4:
   LDY   #$38
   LDA   (FP),Y
   LDY   #$3A
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$3B
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@23
   LDA   #$00
   JMP   .sx_done@24
.sx_neg@23:
   LDA   #$FF
.sx_done@24:
   LDY   #$3C
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$CD
   LDA   (FP),Y
   CLC
   LDY   #$3A
   SBC   (FP),Y
   LDY   #$CE
   LDA   (FP),Y
   LDY   #$3B
   SBC   (FP),Y
   LDY   #$CF
   LDA   (FP),Y
   LDY   #$3C
   SBC   (FP),Y
   LDY   #$D0
   LDA   (FP),Y
   LDY   #$3D
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
   LDY   #$40
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$41
   STA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@25
   DEY
   LDA   (FP),Y
   CMP   #$01
   BNE   .cmp_true@25
   LDA   #$00
   JMP   .cmp_end@26
.cmp_true@25:
   LDA   #$01
.cmp_end@26:
   LDY   #$42
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
.if_end@5:
   LDY   #$CD
   LDA   (FP),Y
   LDY   #$46
   STA   (FP),Y
   LDY   #$CE
   LDA   (FP),Y
   LDY   #$47
   STA   (FP),Y
   LDY   #$CF
   LDA   (FP),Y
   LDY   #$48
   STA   (FP),Y
   LDY   #$D0
   LDA   (FP),Y
   LDY   #$49
   STA   (FP),Y
   CMP   #$D3
   BNE   .cmp_true@28
   DEY
   LDA   (FP),Y
   CMP   #$39
   BNE   .cmp_true@28
   DEY
   LDA   (FP),Y
   CMP   #$3B
   BNE   .cmp_true@28
   DEY
   LDA   (FP),Y
   CMP   #$0F
   BNE   .cmp_true@28
   LDA   #$00
   JMP   .cmp_end@29
.cmp_true@28:
   LDA   #$01
.cmp_end@29:
   LDY   #$4A
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
.if_end@6:
   LDA   #$07
   LDY   #$4E
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$BF
   LDA   (FP),Y
   LDY   #$51
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@31
   LDA   #$00
   JMP   .sx_done@32
.sx_neg@31:
   LDA   #$FF
.sx_done@32:
   LDY   #$52
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
   JSR   mul16
   LDA   HARGS+4
   LDY   #$53
   STA   (FP),Y
   LDA   HARGS+5
   INY
   STA   (FP),Y
   LDY   #$C8
   LDA   (FP),Y
   LDY   #$55
   STA   (FP),Y
   LDY   #$C9
   LDA   (FP),Y
   LDY   #$56
   STA   (FP),Y
   LDY   #$CA
   LDA   (FP),Y
   LDY   #$57
   STA   (FP),Y
   LDY   #$CB
   LDA   (FP),Y
   LDY   #$58
   STA   (FP),Y
   LDY   #$C7
   CMP   (FP),Y
   BNE   .cmp_true@33
   LDY   #$57
   LDA   (FP),Y
   LDY   #$C6
   CMP   (FP),Y
   BNE   .cmp_true@33
   LDY   #$56
   LDA   (FP),Y
   LDY   #$C5
   CMP   (FP),Y
   BNE   .cmp_true@33
   LDY   #$55
   LDA   (FP),Y
   LDY   #$C4
   CMP   (FP),Y
   BNE   .cmp_true@33
   LDA   #$00
   JMP   .cmp_end@34
.cmp_true@33:
   LDA   #$01
.cmp_end@34:
   LDY   #$59
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   SEC
   LDY   #$53
   SBC   (FP),Y
   LDY   #$5A
   LDA   (FP),Y
   LDY   #$54
   SBC   (FP),Y
   BVC   .cmp_novf@36
   EOR   #$80
.cmp_novf@36:
   ASL
   LDA   #$00
   ROL
   EOR   #$01
   LDY   #$5B
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$5D
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$5E
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@37
   LDA   #$00
   JMP   .sx_done@38
.sx_neg@37:
   LDA   #$FF
.sx_done@38:
   LDY   #$5F
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$BF
   LDA   (FP),Y
   LDY   #$61
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@39
   LDA   #$00
   JMP   .sx_done@40
.sx_neg@39:
   LDA   #$FF
.sx_done@40:
   LDY   #$62
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$63
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$64
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@41
   LDA   #$00
   JMP   .sx_done@42
.sx_neg@41:
   LDA   #$FF
.sx_done@42:
   LDY   #$65
   STA   (FP),Y
   INY
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
   LDY   #$C0
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
   LDA   HARGS+8
   LDY   #$67
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
   LDY   #$67
   LDA   (FP),Y
   EOR   #$FF
   LDY   #$6B
   STA   (FP),Y
   LDY   #$68
   LDA   (FP),Y
   EOR   #$FF
   LDY   #$6C
   STA   (FP),Y
   LDY   #$69
   LDA   (FP),Y
   EOR   #$FF
   LDY   #$6D
   STA   (FP),Y
   LDY   #$6A
   LDA   (FP),Y
   EOR   #$FF
   LDY   #$6E
   STA   (FP),Y
   LDY   #$5D
   LDA   (FP),Y
   SEC
   LDY   #$6B
   SBC   (FP),Y
   LDY   #$5E
   LDA   (FP),Y
   LDY   #$6C
   SBC   (FP),Y
   LDY   #$5F
   LDA   (FP),Y
   LDY   #$6D
   SBC   (FP),Y
   LDY   #$60
   LDA   (FP),Y
   LDY   #$6E
   SBC   (FP),Y
   BVC   .cmp_novf@43
   EOR   #$80
.cmp_novf@43:
   ASL
   LDA   #$00
   ROL
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
   BNE   .cmp_true@44
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@44
   LDA   #$00
   JMP   .cmp_end@45
.cmp_true@44:
   LDA   #$01
.cmp_end@45:
   LDY   #$73
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
.if_end@7:
   LDY   #$CC
   LDA   (FP),Y
   LDY   #$77
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@47
   LDA   #$00
   JMP   .sx_done@48
.sx_neg@47:
   LDA   #$FF
.sx_done@48:
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
   BMI   .sx_neg@49
   LDA   #$00
   JMP   .sx_done@50
.sx_neg@49:
   LDA   #$FF
.sx_done@50:
   LDY   #$7B
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$C8
   LDA   (FP),Y
   SEC
   LDY   #$79
   SBC   (FP),Y
   LDY   #$7D
   STA   (FP),Y
   LDY   #$C9
   LDA   (FP),Y
   LDY   #$7A
   SBC   (FP),Y
   LDY   #$7E
   STA   (FP),Y
   LDY   #$CA
   LDA   (FP),Y
   LDY   #$7B
   SBC   (FP),Y
   LDY   #$7F
   STA   (FP),Y
   LDY   #$CB
   LDA   (FP),Y
   LDY   #$7C
   SBC   (FP),Y
   LDY   #$80
   STA   (FP),Y
   LDA   #$96
   INY
   STA   (FP),Y
   LDA   #$E9
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$83
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$84
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@51
   LDA   #$00
   JMP   .sx_done@52
.sx_neg@51:
   LDA   #$FF
.sx_done@52:
   LDY   #$85
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$83
   LDA   (FP),Y
   SEC
   LDY   #$7D
   SBC   (FP),Y
   LDY   #$84
   LDA   (FP),Y
   LDY   #$7E
   SBC   (FP),Y
   LDY   #$85
   LDA   (FP),Y
   LDY   #$7F
   SBC   (FP),Y
   LDY   #$86
   LDA   (FP),Y
   LDY   #$80
   SBC   (FP),Y
   BVC   .cmp_novf@53
   EOR   #$80
.cmp_novf@53:
   ASL
   LDA   #$00
   ROL
   EOR   #$01
   LDY   #$87
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$89
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$8A
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@54
   LDA   #$00
   JMP   .sx_done@55
.sx_neg@54:
   LDA   #$FF
.sx_done@55:
   LDY   #$8B
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$BF
   LDA   (FP),Y
   LDY   #$8D
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@56
   LDA   #$00
   JMP   .sx_done@57
.sx_neg@56:
   LDA   #$FF
.sx_done@57:
   LDY   #$8E
   STA   (FP),Y
   LDA   #$FF
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$8D
   LDA   (FP),Y
   SEC
   LDY   #$8F
   SBC   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$90
   SBC   (FP),Y
   BVC   .cmp_novf@58
   EOR   #$80
.cmp_novf@58:
   ASL
   LDA   #$00
   ROL
   EOR   #$01
   LDY   #$91
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$93
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$94
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@59
   LDA   #$00
   JMP   .sx_done@60
.sx_neg@59:
   LDA   #$FF
.sx_done@60:
   LDY   #$95
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$89
   LDA   (FP),Y
   CLC
   LDY   #$93
   ADC   (FP),Y
   LDY   #$97
   STA   (FP),Y
   LDY   #$8A
   LDA   (FP),Y
   LDY   #$94
   ADC   (FP),Y
   LDY   #$98
   STA   (FP),Y
   LDY   #$8B
   LDA   (FP),Y
   LDY   #$95
   ADC   (FP),Y
   LDY   #$99
   STA   (FP),Y
   LDY   #$8C
   LDA   (FP),Y
   LDY   #$96
   ADC   (FP),Y
   LDY   #$9A
   STA   (FP),Y
   LDY   #$97
   LDA   (FP),Y
   LDY   #$9B
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
   CMP   #$00
   BNE   .cmp_true@61
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@61
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@61
   DEY
   LDA   (FP),Y
   CMP   #$02
   BNE   .cmp_true@61
   LDA   #$00
   JMP   .cmp_end@62
.cmp_true@61:
   LDA   #$01
.cmp_end@62:
   LDY   #$9F
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
   LDY   #$A1
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   LDY   #$A2
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   LDY   #$A2
   LDA   (FP),Y
   LDY   #$02
   STA   (FP),Y
.if_end@8:
   LDA   #$00
   LDY   #$A3
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@64
   LDA   #$00
   JMP   .sx_done@65
.sx_neg@64:
   LDA   #$FF
.sx_done@65:
   LDY   #$A6
   STA   (FP),Y
   LDA   #$07
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@66
   LDA   #$00
   JMP   .sx_done@67
.sx_neg@66:
   LDA   #$FF
.sx_done@67:
   LDY   #$A9
   STA   (FP),Y
   LDA   #$80
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@68
   LDA   #$00
   JMP   .sx_done@69
.sx_neg@68:
   LDA   #$FF
.sx_done@69:
   LDY   #$AC
   STA   (FP),Y
   LDY   #$A8
   LDA   (FP),Y
   CLC
   LDY   #$AB
   ADC   (FP),Y
   LDY   #$AD
   STA   (FP),Y
   LDY   #$A9
   LDA   (FP),Y
   LDY   #$AC
   ADC   (FP),Y
   LDY   #$AE
   STA   (FP),Y
   LDY   #$A5
   LDA   (FP),Y
   SEC
   LDY   #$AD
   SBC   (FP),Y
   LDY   #$A6
   LDA   (FP),Y
   LDY   #$AE
   SBC   (FP),Y
   BVC   .cmp_novf@70
   EOR   #$80
.cmp_novf@70:
   ASL
   LDA   #$00
   ROL
   EOR   #$01
   LDY   #$AF
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$B1
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$B2
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@71
   LDA   #$00
   JMP   .sx_done@72
.sx_neg@71:
   LDA   #$FF
.sx_done@72:
   LDY   #$B3
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$B1
   LDA   (FP),Y
   LDY   #$B5
   STA   (FP),Y
   LDY   #$B2
   LDA   (FP),Y
   LDY   #$B6
   STA   (FP),Y
   LDY   #$B3
   LDA   (FP),Y
   LDY   #$B7
   STA   (FP),Y
   INY
   STA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@73
   LDY   #$B6
   LDA   (FP),Y
   BNE   .cmp_true@73
   DEY
   LDA   (FP),Y
   CMP   #$01
   BNE   .cmp_true@73
   LDA   #$00
   JMP   .cmp_end@74
.cmp_true@73:
   LDA   #$01
.cmp_end@74:
   LDY   #$B9
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
   LDY   #$BB
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   LDY   #$BC
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   LDY   #$BC
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
   ADC   #$D0
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$BD
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

   ; prologue: 0 arg bytes, 4 local bytes
   SEC
   LDA   SSP
   SBC   #$06
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$05
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   LDA   #$01
   LDY   #$01
   STA   (FP),Y
   LDA   #$80
   INY
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$12
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   DEY
   LDA   (FP),Y
   STA   (SSP),Y
   LDA   #$0E
   INY
   STA   (SSP),Y
   LDA   #$00
   INY
   STA   (SSP),Y
   INY
   STA   (SSP),Y
   INY
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
   LDA   #$F9
   INY
   STA   (SSP),Y
   LDA   #$AA
   INY
   STA   (SSP),Y
   LDA   #$7B
   INY
   STA   (SSP),Y
   LDA   #$F2
   INY
   STA   (SSP),Y
   LDY   #$02
   LDA   (FP),Y
   LDY   #$0E
   STA   (SSP),Y
   LDA   #$0F
   INY
   STA   (SSP),Y
   LDA   #$3B
   INY
   STA   (SSP),Y
   LDA   #$39
   INY
   STA   (SSP),Y
   LDA   #$D3
   INY
   STA   (SSP),Y
   JSR   check
   LDY   #$03
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
   ADC   #$06
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$05
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
