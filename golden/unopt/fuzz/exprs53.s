SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24

check:
   SUBROUTINE

   ; prologue: 17 arg bytes, 206 local bytes
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
   LDY   #$D5
   LDA   (FP),Y
   LDY   #$03
   STA   (FP),Y
   LDY   #$D6
   LDA   (FP),Y
   LDY   #$04
   STA   (FP),Y
   CMP   #$FF
   BNE   .cmp_true@0
   DEY
   LDA   (FP),Y
   CMP   #$FF
   BNE   .cmp_true@0
   LDA   #$00
   JMP   .cmp_end@1
.cmp_true@0:
   LDA   #$01
.cmp_end@1:
   LDY   #$05
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
   LDY   #$07
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   LDY   #$08
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   LDY   #$08
   LDA   (FP),Y
   LDY   #$02
   STA   (FP),Y
.if_end@0:
   LDY   #$D5
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@1
   LDY   #$DE
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   INY
   ORA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@1
   LDA   #$00
   LDY   #$09
   STA   (FP),Y
   INY
   STA   (FP),Y
   JMP   .or_end@2
.or_true@1:
   LDA   #$01
   LDY   #$09
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@2:
   LDY   #$D6
   LDA   (FP),Y
   BNE   .cmp_true@3
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@3
   LDA   #$00
   JMP   .cmp_end@4
.cmp_true@3:
   LDA   #$01
.cmp_end@4:
   LDY   #$0B
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$09
   LDA   (FP),Y
   LDY   #$0B
   EOR   (FP),Y
   LDY   #$0D
   STA   (FP),Y
   LDY   #$0A
   LDA   (FP),Y
   LDY   #$0C
   EOR   (FP),Y
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
   BMI   .sx_neg@6
   LDA   #$00
   JMP   .sx_done@7
.sx_neg@6:
   LDA   #$FF
.sx_done@7:
   LDY   #$11
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$DE
   LDA   (FP),Y
   CLC
   LDY   #$0F
   SBC   (FP),Y
   LDY   #$DF
   LDA   (FP),Y
   LDY   #$10
   SBC   (FP),Y
   LDY   #$E0
   LDA   (FP),Y
   LDY   #$11
   SBC   (FP),Y
   LDY   #$E1
   LDA   (FP),Y
   LDY   #$12
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
   LDY   #$15
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$16
   STA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@8
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@8
   LDA   #$00
   JMP   .cmp_end@9
.cmp_true@8:
   LDA   #$01
.cmp_end@9:
   LDY   #$17
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
   LDY   #$19
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   LDY   #$1A
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   LDY   #$1A
   LDA   (FP),Y
   LDY   #$02
   STA   (FP),Y
.if_end@3:
   LDA   #$FF
   LDY   #$1B
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$1B
   LDA   (FP),Y
   LDY   #$1F
   STA   (FP),Y
   LDY   #$1C
   LDA   (FP),Y
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
   LDY   #$DD
   LDA   (FP),Y
   LDY   #$23
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$25
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$26
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@11
   LDA   #$00
   JMP   .sx_done@12
.sx_neg@11:
   LDA   #$FF
.sx_done@12:
   LDY   #$27
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$25
   LDA   (FP),Y
   LDY   #$D7
   ORA   (FP),Y
   LDY   #$29
   STA   (FP),Y
   LDY   #$26
   LDA   (FP),Y
   LDY   #$D8
   ORA   (FP),Y
   LDY   #$2A
   STA   (FP),Y
   LDY   #$27
   LDA   (FP),Y
   LDY   #$D9
   ORA   (FP),Y
   LDY   #$2B
   STA   (FP),Y
   LDY   #$28
   LDA   (FP),Y
   LDY   #$DA
   ORA   (FP),Y
   LDY   #$2C
   STA   (FP),Y
   LDY   #$29
   LDA   (FP),Y
   LDY   #$2D
   STA   (FP),Y
   LDY   #$2A
   LDA   (FP),Y
   LDY   #$2E
   STA   (FP),Y
   LDY   #$21
   LDA   (FP),Y
   LDY   #$2D
   EOR   (FP),Y
   LDY   #$2F
   STA   (FP),Y
   LDY   #$22
   LDA   (FP),Y
   LDY   #$2E
   EOR   (FP),Y
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
   CMP   #$00
   BNE   .cmp_true@13
   DEY
   LDA   (FP),Y
   CMP   #$7F
   BNE   .cmp_true@13
   LDA   #$00
   JMP   .cmp_end@14
.cmp_true@13:
   LDA   #$01
.cmp_end@14:
   LDY   #$33
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
   LDY   #$35
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   LDY   #$36
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   LDY   #$36
   LDA   (FP),Y
   LDY   #$02
   STA   (FP),Y
.if_end@4:
   LDY   #$D1
   LDA   (FP),Y
   LDY   #$37
   STA   (FP),Y
   LDY   #$D2
   LDA   (FP),Y
   LDY   #$38
   STA   (FP),Y
   LDY   #$D3
   LDA   (FP),Y
   LDY   #$39
   STA   (FP),Y
   LDY   #$D4
   LDA   (FP),Y
   LDY   #$3A
   STA   (FP),Y
   LDY   #$37
   LDA   (FP),Y
   SEC
   LDY   #$D7
   SBC   (FP),Y
   LDY   #$38
   LDA   (FP),Y
   LDY   #$D8
   SBC   (FP),Y
   LDY   #$39
   LDA   (FP),Y
   LDY   #$D9
   SBC   (FP),Y
   LDY   #$3A
   LDA   (FP),Y
   LDY   #$DA
   SBC   (FP),Y
   LDA   #$00
   ROL
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
   CMP   #$00
   BNE   .cmp_true@16
   DEY
   LDA   (FP),Y
   CMP   #$01
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
   BEQ   .if_end@5
   LDY   #$01
   LDA   (FP),Y
   ORA   #$08
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
.if_end@5:
   LDA   #$FF
   LDY   #$43
   STA   (FP),Y
   INY
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
   ORA   #$00
   BMI   .sx_neg@19
   LDA   #$00
   JMP   .sx_done@20
.sx_neg@19:
   LDA   #$FF
.sx_done@20:
   LDY   #$48
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$46
   LDA   (FP),Y
   SEC
   INY
   LDA   (FP),Y
   SBC   #$01
   INY
   LDA   (FP),Y
   SBC   #$00
   INY
   LDA   (FP),Y
   SBC   #$00
   BVC   .cmp_novf@21
   EOR   #$80
.cmp_novf@21:
   ASL
   LDA   #$00
   ROL
   EOR   #$01
   LDY   #$4A
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$4C
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$4D
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@22
   LDA   #$00
   JMP   .sx_done@23
.sx_neg@22:
   LDA   #$FF
.sx_done@23:
   LDY   #$4E
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$4C
   LDA   (FP),Y
   AND   #$02
   LDY   #$50
   STA   (FP),Y
   LDY   #$4D
   LDA   (FP),Y
   LDA   #$00
   LDY   #$51
   STA   (FP),Y
   LDY   #$4E
   LDA   (FP),Y
   LDA   #$00
   LDY   #$52
   STA   (FP),Y
   LDY   #$4F
   LDA   (FP),Y
   LDA   #$00
   LDY   #$53
   STA   (FP),Y
   LDA   #$3E
   INY
   STA   (FP),Y
   ORA   #$00
   BNE   .or_true@6
   LDY   #$DE
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   INY
   ORA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@6
   LDA   #$00
   LDY   #$55
   STA   (FP),Y
   INY
   STA   (FP),Y
   JMP   .or_end@7
.or_true@6:
   LDA   #$01
   LDY   #$55
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@7:
   LDY   #$DB
   LDA   (FP),Y
   LDY   #$57
   STA   (FP),Y
   LDY   #$DC
   LDA   (FP),Y
   LDY   #$58
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$57
   LDA   (FP),Y
   CLC
   LDY   #$D7
   SBC   (FP),Y
   LDY   #$58
   LDA   (FP),Y
   LDY   #$D8
   SBC   (FP),Y
   LDY   #$59
   LDA   (FP),Y
   LDY   #$D9
   SBC   (FP),Y
   LDY   #$5A
   LDA   (FP),Y
   LDY   #$DA
   SBC   (FP),Y
   LDA   #$00
   ROL
   LDY   #$5B
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$55
   LDA   (FP),Y
   LDY   #$5B
   AND   (FP),Y
   LDY   #$5D
   STA   (FP),Y
   LDY   #$56
   LDA   (FP),Y
   LDY   #$5C
   AND   (FP),Y
   LDY   #$5E
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
   BMI   .sx_neg@24
   LDA   #$00
   JMP   .sx_done@25
.sx_neg@24:
   LDA   #$FF
.sx_done@25:
   LDY   #$61
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$50
   LDA   (FP),Y
   LDY   #$5F
   AND   (FP),Y
   LDY   #$63
   STA   (FP),Y
   LDY   #$51
   LDA   (FP),Y
   LDY   #$60
   AND   (FP),Y
   LDY   #$64
   STA   (FP),Y
   LDY   #$52
   LDA   (FP),Y
   LDY   #$61
   AND   (FP),Y
   LDY   #$65
   STA   (FP),Y
   LDY   #$53
   LDA   (FP),Y
   LDY   #$62
   AND   (FP),Y
   LDY   #$66
   STA   (FP),Y
   LDY   #$63
   LDA   (FP),Y
   LDY   #$67
   STA   (FP),Y
   LDY   #$64
   LDA   (FP),Y
   LDY   #$68
   STA   (FP),Y
   LDY   #$65
   LDA   (FP),Y
   LDY   #$69
   STA   (FP),Y
   LDY   #$66
   LDA   (FP),Y
   LDY   #$6A
   STA   (FP),Y
   LDY   #$67
   LDA   (FP),Y
   BNE   .cmp_true@26
   LDA   #$00
   JMP   .cmp_end@27
.cmp_true@26:
   LDA   #$01
.cmp_end@27:
   LDY   #$6B
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
   ORA   #$10
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
.if_end@8:
   LDY   #$D5
   LDA   (FP),Y
   LDY   #$6F
   STA   (FP),Y
   LDY   #$D6
   LDA   (FP),Y
   LDY   #$70
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$6F
   LDA   (FP),Y
   SEC
   SBC   #$FF
   INY
   LDA   (FP),Y
   SBC   #$7F
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
   DEY
   LDA   (FP),Y
   LDY   #$75
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$76
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@28
   LDA   #$00
   JMP   .sx_done@29
.sx_neg@28:
   LDA   #$FF
.sx_done@29:
   LDY   #$77
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$D1
   LDA   (FP),Y
   LDY   #$79
   STA   (FP),Y
   LDY   #$D2
   LDA   (FP),Y
   LDY   #$7A
   STA   (FP),Y
   LDY   #$D3
   LDA   (FP),Y
   LDY   #$7B
   STA   (FP),Y
   LDY   #$D4
   LDA   (FP),Y
   LDY   #$7C
   STA   (FP),Y
   LDY   #$79
   LDA   (FP),Y
   CLC
   LDY   #$D7
   ADC   (FP),Y
   LDY   #$7D
   STA   (FP),Y
   LDY   #$7A
   LDA   (FP),Y
   LDY   #$D8
   ADC   (FP),Y
   LDY   #$7E
   STA   (FP),Y
   LDY   #$7B
   LDA   (FP),Y
   LDY   #$D9
   ADC   (FP),Y
   LDY   #$7F
   STA   (FP),Y
   LDY   #$7C
   LDA   (FP),Y
   LDY   #$DA
   ADC   (FP),Y
   LDY   #$80
   STA   (FP),Y
   LDY   #$DD
   LDA   (FP),Y
   LDY   #$81
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$81
   LDA   (FP),Y
   LDY   #$85
   STA   (FP),Y
   LDY   #$82
   LDA   (FP),Y
   LDY   #$86
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$7D
   LDA   (FP),Y
   SEC
   LDY   #$85
   SBC   (FP),Y
   LDY   #$89
   STA   (FP),Y
   LDY   #$7E
   LDA   (FP),Y
   LDY   #$86
   SBC   (FP),Y
   LDY   #$8A
   STA   (FP),Y
   LDY   #$7F
   LDA   (FP),Y
   LDY   #$87
   SBC   (FP),Y
   LDY   #$8B
   STA   (FP),Y
   LDY   #$80
   LDA   (FP),Y
   LDY   #$88
   SBC   (FP),Y
   LDY   #$8C
   STA   (FP),Y
   LDY   #$75
   LDA   (FP),Y
   SEC
   LDY   #$89
   SBC   (FP),Y
   LDY   #$8D
   STA   (FP),Y
   LDY   #$76
   LDA   (FP),Y
   LDY   #$8A
   SBC   (FP),Y
   LDY   #$8E
   STA   (FP),Y
   LDY   #$77
   LDA   (FP),Y
   LDY   #$8B
   SBC   (FP),Y
   LDY   #$8F
   STA   (FP),Y
   LDY   #$78
   LDA   (FP),Y
   LDY   #$8C
   SBC   (FP),Y
   LDY   #$90
   STA   (FP),Y
   LDY   #$8D
   LDA   (FP),Y
   LDY   #$91
   STA   (FP),Y
   LDY   #$8E
   LDA   (FP),Y
   LDY   #$92
   STA   (FP),Y
   LDY   #$8F
   LDA   (FP),Y
   LDY   #$93
   STA   (FP),Y
   LDY   #$90
   LDA   (FP),Y
   LDY   #$94
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
   CMP   #$73
   BNE   .cmp_true@30
   LDA   #$00
   JMP   .cmp_end@31
.cmp_true@30:
   LDA   #$01
.cmp_end@31:
   LDY   #$95
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
   LDY   #$97
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   LDY   #$98
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   LDY   #$98
   LDA   (FP),Y
   LDY   #$02
   STA   (FP),Y
.if_end@9:
   LDY   #$DD
   LDA   (FP),Y
   LDY   #$99
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$DD
   LDA   (FP),Y
   LDY   #$9B
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$99
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
   LDY   #$9D
   STA   (FP),Y
   LDA   HARGS+5
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@10
   LDY   #$DB
   LDA   (FP),Y
   LDY   #$9F
   STA   (FP),Y
   LDY   #$DC
   LDA   (FP),Y
   LDY   #$A0
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$9F
   ORA   (FP),Y
   LDY   #$A3
   STA   (FP),Y
   LDY   #$A0
   LDA   (FP),Y
   LDY   #$A4
   STA   (FP),Y
   LDY   #$A1
   LDA   (FP),Y
   LDY   #$A5
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$A3
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   INY
   ORA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@10
   LDA   #$01
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   JMP   .and_end@11
.and_false@10:
   LDA   #$00
   LDY   #$A7
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@11:
   LDY   #$DD
   LDA   (FP),Y
   LDY   #$A9
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$AB
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$AC
   STA   (FP),Y
   LDY   #$D6
   CMP   (FP),Y
   BNE   .cmp_true@33
   LDY   #$AB
   LDA   (FP),Y
   LDY   #$D5
   CMP   (FP),Y
   BNE   .cmp_true@33
   LDA   #$00
   JMP   .cmp_end@34
.cmp_true@33:
   LDA   #$01
.cmp_end@34:
   LDY   #$AD
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$AF
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$B0
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@36
   LDA   #$00
   JMP   .sx_done@37
.sx_neg@36:
   LDA   #$FF
.sx_done@37:
   LDY   #$B1
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$AF
   LDA   (FP),Y
   SEC
   LDY   #$DE
   SBC   (FP),Y
   LDY   #$B0
   LDA   (FP),Y
   LDY   #$DF
   SBC   (FP),Y
   LDY   #$B1
   LDA   (FP),Y
   LDY   #$E0
   SBC   (FP),Y
   LDY   #$B2
   LDA   (FP),Y
   LDY   #$E1
   SBC   (FP),Y
   LDA   #$00
   ROL
   LDY   #$B3
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$A7
   LDA   (FP),Y
   SEC
   LDY   #$B3
   SBC   (FP),Y
   LDY   #$B5
   STA   (FP),Y
   LDY   #$A8
   LDA   (FP),Y
   LDY   #$B4
   SBC   (FP),Y
   LDY   #$B6
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$B7
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$B8
   STA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@38
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@38
   LDA   #$00
   JMP   .cmp_end@39
.cmp_true@38:
   LDA   #$01
.cmp_end@39:
   LDY   #$B9
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
.if_end@12:
   LDA   #$FF
   LDY   #$BD
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$DE
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
   LDY   #$BD
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
   LDY   #$C1
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
   LDY   #$C1
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   INY
   ORA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lnot_true@41
   LDA   #$00
   JMP   .lnot_end@42
.lnot_true@41:
   LDA   #$01
.lnot_end@42:
   LDY   #$C5
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@13
   LDY   #$D5
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@13
   LDA   #$00
   LDY   #$C7
   STA   (FP),Y
   INY
   STA   (FP),Y
   JMP   .or_end@14
.or_true@13:
   LDA   #$01
   LDY   #$C7
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@14:
   LDY   #$C7
   LDA   (FP),Y
   LDY   #$C9
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$CA
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CMP   #$01
   BNE   .cmp_true@43
   LDA   #$00
   JMP   .cmp_end@44
.cmp_true@43:
   LDA   #$01
.cmp_end@44:
   LDY   #$CB
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@15
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
.if_end@15:
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
   ADC   #$E1
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

   ; prologue: 0 arg bytes, 3 local bytes
   SEC
   LDA   SSP
   SBC   #$05
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$04
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
   SEC
   LDA   SSP
   SBC   #$11
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   #$0E
   STA   (SSP),Y
   LDA   #$00
   INY
   STA   (SSP),Y
   INY
   STA   (SSP),Y
   INY
   STA   (SSP),Y
   LDA   #$FF
   INY
   STA   (SSP),Y
   INY
   STA   (SSP),Y
   LDA   #$00
   INY
   STA   (SSP),Y
   INY
   STA   (SSP),Y
   INY
   STA   (SSP),Y
   INY
   STA   (SSP),Y
   LDA   #$60
   INY
   STA   (SSP),Y
   LDA   #$BA
   INY
   STA   (SSP),Y
   LDY   #$01
   LDA   (FP),Y
   LDY   #$0D
   STA   (SSP),Y
   LDA   #$00
   INY
   STA   (SSP),Y
   INY
   STA   (SSP),Y
   INY
   STA   (SSP),Y
   INY
   STA   (SSP),Y
   JSR   check
   LDY   #$02
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
   ADC   #$05
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$04
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
