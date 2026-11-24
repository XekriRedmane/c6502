SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24

check:
   SUBROUTINE

   ; prologue: 0 arg bytes, 174 local bytes
   SEC
   LDA   SSP
   SBC   #$B0
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$AF
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
   LDA   v0
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
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
   LDA   v0
   LDY   #$0D
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDA   v1
   ORA   v1+1
   ORA   v1+2
   ORA   v1+3
   BEQ   .lnot_true@2
   LDA   #$00
   JMP   .lnot_end@3
.lnot_true@2:
   LDA   #$01
.lnot_end@3:
   LDY   #$0F
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   v2
   LDY   #$11
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   v2+1
   LDY   #$12
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   SEC
   LDY   #$0D
   SBC   (FP),Y
   LDY   #$12
   LDA   (FP),Y
   LDY   #$0E
   SBC   (FP),Y
   BVC   .cmp_novf@4
   EOR   #$80
.cmp_novf@4:
   ASL
   LDA   #$00
   ROL
   LDY   #$13
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
   LDY   #$17
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
.if_end@1:
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
   INY
   STA   (FP),Y
   LDY   #$1B
   LDA   (FP),Y
   CMP   #$FF
   BNE   .cmp_true@8
   LDA   #$00
   JMP   .cmp_end@9
.cmp_true@8:
   LDA   #$01
.cmp_end@9:
   LDY   #$20
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
   LDY   #$22
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   LDY   #$23
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   LDY   #$23
   LDA   (FP),Y
   LDY   #$02
   STA   (FP),Y
.if_end@2:
   LDA   v2
   EOR   #$FF
   LDY   #$24
   STA   (FP),Y
   LDA   v2+1
   EOR   #$FF
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$26
   STA   (FP),Y
   INY
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@10
   LDA   #$00
   JMP   .sx_done@11
.sx_neg@10:
   LDA   #$FF
.sx_done@11:
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
   BMI   .sx_neg@12
   LDA   #$00
   JMP   .sx_done@13
.sx_neg@12:
   LDA   #$FF
.sx_done@13:
   LDY   #$2B
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   v3
   INY
   STA   (FP),Y
   LDA   v3+1
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$29
   LDA   (FP),Y
   SEC
   LDY   #$2D
   SBC   (FP),Y
   LDY   #$31
   STA   (FP),Y
   LDY   #$2A
   LDA   (FP),Y
   LDY   #$2E
   SBC   (FP),Y
   LDY   #$32
   STA   (FP),Y
   LDY   #$2B
   LDA   (FP),Y
   LDY   #$2F
   SBC   (FP),Y
   LDY   #$33
   STA   (FP),Y
   LDY   #$2C
   LDA   (FP),Y
   LDY   #$30
   SBC   (FP),Y
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
   LDY   #$33
   LDA   (FP),Y
   LDY   #$37
   STA   (FP),Y
   LDY   #$34
   LDA   (FP),Y
   LDY   #$38
   STA   (FP),Y
   CMP   #$FF
   BNE   .cmp_true@14
   DEY
   LDA   (FP),Y
   CMP   #$FF
   BNE   .cmp_true@14
   DEY
   LDA   (FP),Y
   CMP   #$FF
   BNE   .cmp_true@14
   DEY
   LDA   (FP),Y
   CMP   #$FF
   BNE   .cmp_true@14
   LDA   #$00
   JMP   .cmp_end@15
.cmp_true@14:
   LDA   #$01
.cmp_end@15:
   LDY   #$39
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
.if_end@3:
   LDA   v4
   LDY   #$3D
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
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$3D
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
   INY
   LDA   (FP),Y
   STA   HARGS+4
   LDA   #$00
   STA   HARGS+5
   STA   HARGS+6
   STA   HARGS+7
   LDY   #$02
   STY   HARGS+8
   LDY   #$00
   STY   HARGS+9
   STY   HARGS+10
   STY   HARGS+11
   STY   HARGS+12
   STA   HARGS+13
   STA   HARGS+14
   STA   HARGS+15
   JSR   udivmod64
   LDA   HARGS+24
   LDY   #$42
   STA   (FP),Y
   LDA   HARGS+25
   INY
   STA   (FP),Y
   LDA   HARGS+26
   INY
   STA   (FP),Y
   LDA   HARGS+27
   INY
   STA   (FP),Y
   LDA   HARGS+28
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
   LDA   #$07
   STA   HARGS+4
   LDA   #$00
   STA   HARGS+5
   LDY   #$00
   STY   HARGS+6
   STA   HARGS+7
   JSR   udivmod32
   LDA   HARGS+12
   LDY   #$47
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
   LDY   #$47
   LDA   (FP),Y
   LDY   #$4B
   STA   (FP),Y
   LDY   #$48
   LDA   (FP),Y
   LDY   #$4C
   STA   (FP),Y
   LDY   #$49
   LDA   (FP),Y
   LDY   #$4D
   STA   (FP),Y
   LDY   #$4A
   LDA   (FP),Y
   LDY   #$4E
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$42
   LDA   (FP),Y
   SEC
   LDY   #$4B
   SBC   (FP),Y
   LDY   #$50
   STA   (FP),Y
   LDY   #$43
   LDA   (FP),Y
   LDY   #$4C
   SBC   (FP),Y
   LDY   #$51
   STA   (FP),Y
   LDY   #$44
   LDA   (FP),Y
   LDY   #$4D
   SBC   (FP),Y
   LDY   #$52
   STA   (FP),Y
   LDY   #$45
   LDA   (FP),Y
   LDY   #$4E
   SBC   (FP),Y
   LDY   #$53
   STA   (FP),Y
   LDY   #$46
   LDA   (FP),Y
   LDY   #$4F
   SBC   (FP),Y
   LDY   #$54
   STA   (FP),Y
   LDY   #$50
   LDA   (FP),Y
   LDY   #$55
   STA   (FP),Y
   LDY   #$51
   LDA   (FP),Y
   LDY   #$56
   STA   (FP),Y
   LDY   #$52
   LDA   (FP),Y
   LDY   #$57
   STA   (FP),Y
   LDY   #$53
   LDA   (FP),Y
   LDY   #$58
   STA   (FP),Y
   LDY   #$54
   LDA   (FP),Y
   AND   #$01
   LDY   #$59
   STA   (FP),Y
   LDA   v2
   INY
   STA   (FP),Y
   LDA   v2+1
   INY
   STA   (FP),Y
   LDA   v2+1
   BMI   .sx_neg@17
   LDA   #$00
   JMP   .sx_done@18
.sx_neg@17:
   LDA   #$FF
.sx_done@18:
   LDY   #$5C
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$5A
   LDA   (FP),Y
   LDY   #$5F
   STA   (FP),Y
   LDY   #$5B
   LDA   (FP),Y
   LDY   #$60
   STA   (FP),Y
   LDY   #$5C
   LDA   (FP),Y
   LDY   #$61
   STA   (FP),Y
   INY
   STA   (FP),Y
   AND   #$01
   INY
   STA   (FP),Y
   LDY   #$5F
   LDA   (FP),Y
   SEC
   LDY   #$55
   SBC   (FP),Y
   LDY   #$60
   LDA   (FP),Y
   LDY   #$56
   SBC   (FP),Y
   LDY   #$61
   LDA   (FP),Y
   LDY   #$57
   SBC   (FP),Y
   LDY   #$62
   LDA   (FP),Y
   LDY   #$58
   SBC   (FP),Y
   LDY   #$63
   LDA   (FP),Y
   LDY   #$59
   SBC   (FP),Y
   LDA   #$00
   ROL
   LDY   #$64
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$66
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$67
   STA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@19
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@19
   LDA   #$00
   JMP   .cmp_end@20
.cmp_true@19:
   LDA   #$01
.cmp_end@20:
   LDY   #$68
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
   LDY   #$6A
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   LDY   #$6B
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   LDY   #$6B
   LDA   (FP),Y
   LDY   #$02
   STA   (FP),Y
.if_end@4:
   LDA   v4
   LDY   #$6C
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
   LDA   #$00
   INY
   STA   (FP),Y
   LDA   v3
   INY
   STA   (FP),Y
   LDA   v3+1
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   SEC
   LDY   #$71
   SBC   (FP),Y
   LDY   #$76
   STA   (FP),Y
   LDA   #$00
   LDY   #$72
   SBC   (FP),Y
   LDY   #$77
   STA   (FP),Y
   LDA   #$00
   LDY   #$73
   SBC   (FP),Y
   LDY   #$78
   STA   (FP),Y
   LDA   #$80
   LDY   #$74
   SBC   (FP),Y
   LDY   #$79
   STA   (FP),Y
   LDA   #$00
   LDY   #$75
   SBC   (FP),Y
   LDY   #$7A
   STA   (FP),Y
   LDY   #$76
   LDA   (FP),Y
   LDY   #$7B
   STA   (FP),Y
   LDY   #$77
   LDA   (FP),Y
   LDY   #$7C
   STA   (FP),Y
   LDY   #$78
   LDA   (FP),Y
   LDY   #$7D
   STA   (FP),Y
   LDY   #$79
   LDA   (FP),Y
   LDY   #$7E
   STA   (FP),Y
   LDY   #$7A
   LDA   (FP),Y
   AND   #$01
   LDY   #$7F
   STA   (FP),Y
   LDY   #$7B
   LDA   (FP),Y
   SEC
   LDY   #$6C
   SBC   (FP),Y
   LDY   #$7C
   LDA   (FP),Y
   LDY   #$6D
   SBC   (FP),Y
   LDY   #$7D
   LDA   (FP),Y
   LDY   #$6E
   SBC   (FP),Y
   LDY   #$7E
   LDA   (FP),Y
   LDY   #$6F
   SBC   (FP),Y
   LDY   #$7F
   LDA   (FP),Y
   LDY   #$70
   SBC   (FP),Y
   LDA   #$00
   ROL
   LDY   #$80
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$82
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$83
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
   LDY   #$84
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .cmp_true@25
   LDY   #$01
   LDA   (FP),Y
   ORA   #$20
   LDY   #$86
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   LDY   #$87
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   LDY   #$87
   LDA   (FP),Y
   LDY   #$02
   STA   (FP),Y
   LDA   #$FF
   BNE   .cmp_true@25
   CMP   #$7F
   BNE   .cmp_true@25
   LDA   #$00
   JMP   .cmp_end@26
.cmp_true@25:
   LDA   #$01
.cmp_end@26:
   LDY   #$88
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDA   v1
   INY
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
   LDY   #$8A
   LDA   (FP),Y
   CLC
   SBC   v4
   INY
   LDA   (FP),Y
   SBC   v4+1
   INY
   LDA   (FP),Y
   SBC   v4+2
   INY
   LDA   (FP),Y
   SBC   v4+3
   LDA   #$00
   ROL
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$88
   LDA   (FP),Y
   STA   HARGS
   INY
   LDA   (FP),Y
   STA   HARGS+1
   LDY   #$8E
   LDA   (FP),Y
   STA   HARGS+2
   INY
   LDA   (FP),Y
   STA   HARGS+3
   JSR   sdivmod16
   LDA   HARGS+4
   LDY   #$90
   STA   (FP),Y
   LDA   HARGS+5
   INY
   STA   (FP),Y
   LDA   v1
   ORA   v1+1
   ORA   v1+2
   ORA   v1+3
   BNE   .or_true@6
   LDA   #$34
   INY
   STA   (FP),Y
   LDA   #$A4
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@6
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   JMP   .or_end@7
.or_true@6:
   LDA   #$01
   LDY   #$94
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@7:
   LDY   #$94
   LDA   (FP),Y
   LDY   #$96
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$97
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@28
   LDA   #$00
   JMP   .sx_done@29
.sx_neg@28:
   LDA   #$FF
.sx_done@29:
   LDY   #$98
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   v4
   CLC
   LDY   #$96
   SBC   (FP),Y
   LDA   v4+1
   INY
   SBC   (FP),Y
   LDA   v4+2
   INY
   SBC   (FP),Y
   LDA   v4+3
   INY
   SBC   (FP),Y
   LDA   #$00
   ROL
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$91
   LDA   (FP),Y
   LDY   #$9B
   CMP   (FP),Y
   BNE   .thread@0
   LDY   #$90
   LDA   (FP),Y
   LDY   #$9A
   CMP   (FP),Y
   BEQ   .cmp_true@30
.thread@0:
   LDA   #$00
   JMP   .cmp_end@31
.cmp_true@30:
   LDA   #$01
.cmp_end@31:
   LDY   #$9C
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$9E
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$9F
   STA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@33
   DEY
   LDA   (FP),Y
   CMP   #$01
   BNE   .cmp_true@33
   LDA   #$00
   JMP   .cmp_end@34
.cmp_true@33:
   LDA   #$01
.cmp_end@34:
   LDY   #$A0
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
   LDY   #$A2
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   LDY   #$A3
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   LDY   #$A3
   LDA   (FP),Y
   LDY   #$02
   STA   (FP),Y
.if_end@8:
   LDA   v1
   LDY   #$A4
   STA   (FP),Y
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
   LDY   #$A7
   STA   (FP),Y
   LDA   #$36
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
   LDY   #$AA
   STA   (FP),Y
   LDY   #$A7
   LDA   (FP),Y
   LDY   #$AA
   CMP   (FP),Y
   BNE   .cmp_true@40
   LDY   #$A6
   LDA   (FP),Y
   LDY   #$A9
   CMP   (FP),Y
   BNE   .cmp_true@40
   LDA   #$00
   JMP   .cmp_end@41
.cmp_true@40:
   LDA   #$01
.cmp_end@41:
   LDY   #$AB
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
   LDY   #$AD
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   LDY   #$AE
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   LDY   #$AE
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
   ADC   #$B0
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$AF
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
   DS.B  1

v1:
   DC.L  $23FD7E36

v2:
   DC.W  $8000

v3:
   DS.B  2

v4:
   DC.L  $00000009

v5:
   DS.B  4
