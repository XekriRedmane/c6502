SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24

check:
   SUBROUTINE

   ; prologue: 0 arg bytes, 244 local bytes
   SEC
   LDA   SSP
   SBC   #$F6
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$F5
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
   INY
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@0
   LDA   #$00
   JMP   .sx_done@1
.sx_neg@0:
   LDA   #$FF
.sx_done@1:
   LDY   #$05
   STA   (FP),Y
   LDA   #$0F
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@2
   LDA   #$00
   JMP   .sx_done@3
.sx_neg@2:
   LDA   #$FF
.sx_done@3:
   LDY   #$08
   STA   (FP),Y
   LDY   #$05
   LDA   (FP),Y
   LDY   #$08
   CMP   (FP),Y
   BNE   .cmp_true@4
   LDY   #$04
   LDA   (FP),Y
   LDY   #$07
   CMP   (FP),Y
   BNE   .cmp_true@4
   LDA   #$00
   JMP   .cmp_end@5
.cmp_true@4:
   LDA   #$01
.cmp_end@5:
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
   LDA   #$FF
   LDY   #$0D
   STA   (FP),Y
   INY
   STA   (FP),Y
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
   LDY   #$11
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$0F
   LDA   (FP),Y
   LDY   #$13
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$11
   LDA   (FP),Y
   LDY   #$15
   STA   (FP),Y
   AND   #$3F
   INY
   STA   (FP),Y
   LDA   v2
   CLC
   LDY   #$13
   ADC   (FP),Y
   LDY   #$17
   STA   (FP),Y
   LDA   v2+1
   LDY   #$14
   ADC   (FP),Y
   LDY   #$18
   STA   (FP),Y
   LDA   v2+2
   LDY   #$15
   ADC   (FP),Y
   LDY   #$19
   STA   (FP),Y
   LDA   v2+3
   LDY   #$16
   ADC   (FP),Y
   LDY   #$1A
   STA   (FP),Y
   LDY   #$17
   LDA   (FP),Y
   LDY   #$1B
   STA   (FP),Y
   LDY   #$18
   LDA   (FP),Y
   LDY   #$1C
   STA   (FP),Y
   LDY   #$19
   LDA   (FP),Y
   LDY   #$1D
   STA   (FP),Y
   LDY   #$1A
   LDA   (FP),Y
   AND   #$3F
   LDY   #$1E
   STA   (FP),Y
   LDA   #$7F
   CLC
   LDY   #$1B
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
   LDY   #$21
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$22
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$23
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$24
   STA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@9
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@9
   LDA   #$00
   JMP   .cmp_end@10
.cmp_true@9:
   LDA   #$01
.cmp_end@10:
   LDY   #$25
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
   LDY   #$27
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   LDY   #$28
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   LDY   #$28
   LDA   (FP),Y
   LDY   #$02
   STA   (FP),Y
.if_end@1:
   LDA   #$FF
   LDY   #$29
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
   LDY   #$2B
   STA   (FP),Y
   LDA   v3
   INY
   STA   (FP),Y
   LDA   v3
   BMI   .sx_neg@14
   LDA   #$00
   JMP   .sx_done@15
.sx_neg@14:
   LDA   #$FF
.sx_done@15:
   LDY   #$2D
   STA   (FP),Y
   LDY   #$2A
   LDA   (FP),Y
   CLC
   LDY   #$2C
   ADC   (FP),Y
   LDY   #$2E
   STA   (FP),Y
   LDY   #$2B
   LDA   (FP),Y
   LDY   #$2D
   ADC   (FP),Y
   LDY   #$2F
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$30
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$31
   STA   (FP),Y
   LDA   #$FF
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$31
   LDA   (FP),Y
   LDY   #$33
   CMP   (FP),Y
   BNE   .cmp_true@16
   LDY   #$30
   LDA   (FP),Y
   LDY   #$32
   CMP   (FP),Y
   BNE   .cmp_true@16
   LDA   #$00
   JMP   .cmp_end@17
.cmp_true@16:
   LDA   #$01
.cmp_end@17:
   LDY   #$34
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
   LDY   #$36
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   LDY   #$37
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   LDY   #$37
   LDA   (FP),Y
   LDY   #$02
   STA   (FP),Y
.if_end@2:
   LDA   v5
   LDY   #$38
   STA   (FP),Y
   LDA   v5+1
   INY
   STA   (FP),Y
   LDA   v5+1
   BMI   .sx_neg@19
   LDA   #$00
   JMP   .sx_done@20
.sx_neg@19:
   LDA   #$FF
.sx_done@20:
   LDY   #$3A
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$38
   LDA   (FP),Y
   LDY   #$3E
   STA   (FP),Y
   LDY   #$39
   LDA   (FP),Y
   LDY   #$3F
   STA   (FP),Y
   LDY   #$3A
   LDA   (FP),Y
   LDY   #$40
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   AND   #$03
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
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$44
   LDA   (FP),Y
   SEC
   SBC   v0
   LDY   #$4A
   STA   (FP),Y
   LDY   #$45
   LDA   (FP),Y
   SBC   v0+1
   LDY   #$4B
   STA   (FP),Y
   LDY   #$46
   LDA   (FP),Y
   SBC   v0+2
   LDY   #$4C
   STA   (FP),Y
   LDY   #$47
   LDA   (FP),Y
   SBC   v0+3
   LDY   #$4D
   STA   (FP),Y
   LDY   #$48
   LDA   (FP),Y
   SBC   v0+4
   LDY   #$4E
   STA   (FP),Y
   LDY   #$49
   LDA   (FP),Y
   SBC   v0+5
   LDY   #$4F
   STA   (FP),Y
   LDY   #$4A
   LDA   (FP),Y
   LDY   #$50
   STA   (FP),Y
   LDY   #$4B
   LDA   (FP),Y
   LDY   #$51
   STA   (FP),Y
   LDY   #$4C
   LDA   (FP),Y
   LDY   #$52
   STA   (FP),Y
   LDY   #$4D
   LDA   (FP),Y
   LDY   #$53
   STA   (FP),Y
   LDY   #$4E
   LDA   (FP),Y
   LDY   #$54
   STA   (FP),Y
   LDY   #$4F
   LDA   (FP),Y
   AND   #$03
   LDY   #$55
   STA   (FP),Y
   LDY   #$50
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
   BNE   .lb_skip@0
   JMP   .and_false@3
.lb_skip@0:
   LDA   #$FF
   LDY   #$56
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$59
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$5A
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@21
   LDA   #$00
   JMP   .sx_done@22
.sx_neg@21:
   LDA   #$FF
.sx_done@22:
   LDY   #$5B
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$59
   LDA   (FP),Y
   LDY   #$5F
   STA   (FP),Y
   LDY   #$5A
   LDA   (FP),Y
   LDY   #$60
   STA   (FP),Y
   LDY   #$5B
   LDA   (FP),Y
   LDY   #$61
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   AND   #$03
   INY
   STA   (FP),Y
   LDY   #$5F
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
   INY
   LDA   (FP),Y
   STA   HARGS+5
   LDA   #$45
   STA   HARGS+8
   LDA   #$23
   STA   HARGS+9
   LDA   #$01
   STA   HARGS+10
   LDA   #$00
   STA   HARGS+6
   STA   HARGS+7
   STA   HARGS+11
   LDY   #$00
   STY   HARGS+12
   STY   HARGS+13
   STA   HARGS+14
   STA   HARGS+15
   JSR   mul64
   LDA   HARGS+16
   LDY   #$65
   STA   (FP),Y
   LDA   HARGS+17
   INY
   STA   (FP),Y
   LDA   HARGS+18
   INY
   STA   (FP),Y
   LDA   HARGS+19
   INY
   STA   (FP),Y
   LDA   HARGS+20
   INY
   STA   (FP),Y
   LDA   HARGS+21
   INY
   STA   (FP),Y
   LDY   #$65
   LDA   (FP),Y
   LDY   #$6B
   STA   (FP),Y
   LDY   #$66
   LDA   (FP),Y
   LDY   #$6C
   STA   (FP),Y
   LDY   #$67
   LDA   (FP),Y
   LDY   #$6D
   STA   (FP),Y
   LDY   #$68
   LDA   (FP),Y
   LDY   #$6E
   STA   (FP),Y
   LDY   #$69
   LDA   (FP),Y
   LDY   #$6F
   STA   (FP),Y
   LDY   #$6A
   LDA   (FP),Y
   AND   #$03
   LDY   #$70
   STA   (FP),Y
   LDY   #$6B
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
   BEQ   .and_false@3
   LDA   #$01
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   JMP   .and_end@4
.and_false@3:
   LDA   #$00
   LDY   #$71
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@4:
   LDY   #$71
   LDA   (FP),Y
   LDY   #$73
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$74
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@23
   LDA   #$00
   JMP   .sx_done@24
.sx_neg@23:
   LDA   #$FF
.sx_done@24:
   LDY   #$75
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$73
   LDA   (FP),Y
   LDY   #$79
   STA   (FP),Y
   LDY   #$74
   LDA   (FP),Y
   LDY   #$7A
   STA   (FP),Y
   LDY   #$75
   LDA   (FP),Y
   LDY   #$7B
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   AND   #$03
   INY
   STA   (FP),Y
   LDY   #$3E
   LDA   (FP),Y
   SEC
   LDY   #$79
   SBC   (FP),Y
   LDY   #$3F
   LDA   (FP),Y
   LDY   #$7A
   SBC   (FP),Y
   LDY   #$40
   LDA   (FP),Y
   LDY   #$7B
   SBC   (FP),Y
   LDY   #$41
   LDA   (FP),Y
   LDY   #$7C
   SBC   (FP),Y
   LDY   #$42
   LDA   (FP),Y
   LDY   #$7D
   SBC   (FP),Y
   LDY   #$43
   LDA   (FP),Y
   LDY   #$7E
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
   LDY   #$81
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$82
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
   LDY   #$83
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
   LDY   #$85
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   LDY   #$86
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   LDY   #$86
   LDA   (FP),Y
   LDY   #$02
   STA   (FP),Y
.if_end@5:
   LDA   #$FF
   EOR   v1
   LDY   #$87
   STA   (FP),Y
   LDA   #$FF
   EOR   v1+1
   INY
   STA   (FP),Y
   LDA   #$00
   EOR   v1+2
   INY
   STA   (FP),Y
   LDA   #$00
   EOR   v1+3
   INY
   STA   (FP),Y
   LDA   #$00
   EOR   v1+4
   INY
   STA   (FP),Y
   LDA   #$00
   EOR   v1+5
   INY
   STA   (FP),Y
   LDA   v4
   INY
   STA   (FP),Y
   LDA   v4
   BMI   .sx_neg@28
   LDA   #$00
   JMP   .sx_done@29
.sx_neg@28:
   LDA   #$FF
.sx_done@29:
   LDY   #$8E
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$8F
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$90
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@30
   LDA   #$00
   JMP   .sx_done@31
.sx_neg@30:
   LDA   #$FF
.sx_done@31:
   LDY   #$91
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$8F
   LDA   (FP),Y
   LDY   #$93
   STA   (FP),Y
   LDY   #$90
   LDA   (FP),Y
   LDY   #$94
   STA   (FP),Y
   LDY   #$91
   LDA   (FP),Y
   LDY   #$95
   STA   (FP),Y
   AND   #$3F
   INY
   STA   (FP),Y
   CMP   v2+3
   BNE   .thread@0
   DEY
   LDA   (FP),Y
   CMP   v2+2
   BNE   .thread@0
   DEY
   LDA   (FP),Y
   CMP   v2+1
   BNE   .thread@0
   DEY
   LDA   (FP),Y
   CMP   v2
   BEQ   .cmp_true@32
.thread@0:
   LDA   #$00
   JMP   .cmp_end@33
.cmp_true@32:
   LDA   #$01
.cmp_end@33:
   LDY   #$97
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
   BMI   .sx_neg@35
   LDA   #$00
   JMP   .sx_done@36
.sx_neg@35:
   LDA   #$FF
.sx_done@36:
   LDY   #$9B
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$99
   LDA   (FP),Y
   LDY   #$9F
   STA   (FP),Y
   LDY   #$9A
   LDA   (FP),Y
   LDY   #$A0
   STA   (FP),Y
   LDY   #$9B
   LDA   (FP),Y
   LDY   #$A1
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   AND   #$03
   INY
   STA   (FP),Y
   LDY   #$87
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
   INY
   LDA   (FP),Y
   STA   HARGS+5
   LDA   #$00
   STA   HARGS+6
   STA   HARGS+7
   LDY   #$9F
   LDA   (FP),Y
   STA   HARGS+8
   INY
   LDA   (FP),Y
   STA   HARGS+9
   INY
   LDA   (FP),Y
   STA   HARGS+10
   INY
   LDA   (FP),Y
   STA   HARGS+11
   INY
   LDA   (FP),Y
   STA   HARGS+12
   INY
   LDA   (FP),Y
   STA   HARGS+13
   LDA   #$00
   STA   HARGS+14
   STA   HARGS+15
   JSR   mul64
   LDA   HARGS+16
   LDY   #$A5
   STA   (FP),Y
   LDA   HARGS+17
   INY
   STA   (FP),Y
   LDA   HARGS+18
   INY
   STA   (FP),Y
   LDA   HARGS+19
   INY
   STA   (FP),Y
   LDA   HARGS+20
   INY
   STA   (FP),Y
   LDA   HARGS+21
   INY
   STA   (FP),Y
   LDY   #$A5
   LDA   (FP),Y
   LDY   #$AB
   STA   (FP),Y
   LDY   #$A6
   LDA   (FP),Y
   LDY   #$AC
   STA   (FP),Y
   LDY   #$A7
   LDA   (FP),Y
   LDY   #$AD
   STA   (FP),Y
   LDY   #$A8
   LDA   (FP),Y
   LDY   #$AE
   STA   (FP),Y
   LDY   #$A9
   LDA   (FP),Y
   LDY   #$AF
   STA   (FP),Y
   LDY   #$AA
   LDA   (FP),Y
   AND   #$03
   LDY   #$B0
   STA   (FP),Y
   LDY   #$AB
   LDA   (FP),Y
   LDY   #$B1
   STA   (FP),Y
   LDY   #$AC
   LDA   (FP),Y
   LDY   #$B2
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$B3
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$B4
   STA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@37
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@37
   LDA   #$00
   JMP   .cmp_end@38
.cmp_true@37:
   LDA   #$01
.cmp_end@38:
   LDY   #$B5
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
   LDY   #$B7
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   LDY   #$B8
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   LDY   #$B8
   LDA   (FP),Y
   LDY   #$02
   STA   (FP),Y
.if_end@6:
   LDA   v4
   LDY   #$B9
   STA   (FP),Y
   LDA   v4
   BMI   .sx_neg@40
   LDA   #$00
   JMP   .sx_done@41
.sx_neg@40:
   LDA   #$FF
.sx_done@41:
   LDY   #$BA
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$BB
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$BC
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@42
   LDA   #$00
   JMP   .sx_done@43
.sx_neg@42:
   LDA   #$FF
.sx_done@43:
   LDY   #$BD
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$BB
   LDA   (FP),Y
   LDY   #$C1
   STA   (FP),Y
   LDY   #$BC
   LDA   (FP),Y
   LDY   #$C2
   STA   (FP),Y
   LDY   #$BD
   LDA   (FP),Y
   LDY   #$C3
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   AND   #$03
   INY
   STA   (FP),Y
   LDA   v1
   STA   HARGS
   LDA   v1+1
   STA   HARGS+1
   LDA   v1+2
   STA   HARGS+2
   LDA   v1+3
   STA   HARGS+3
   LDA   v1+4
   STA   HARGS+4
   LDA   v1+5
   STA   HARGS+5
   LDA   #$00
   STA   HARGS+6
   STA   HARGS+7
   LDY   #$C1
   LDA   (FP),Y
   STA   HARGS+8
   INY
   LDA   (FP),Y
   STA   HARGS+9
   INY
   LDA   (FP),Y
   STA   HARGS+10
   INY
   LDA   (FP),Y
   STA   HARGS+11
   INY
   LDA   (FP),Y
   STA   HARGS+12
   INY
   LDA   (FP),Y
   STA   HARGS+13
   LDA   #$00
   STA   HARGS+14
   STA   HARGS+15
   JSR   udivmod64
   LDA   HARGS+24
   LDY   #$C7
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
   LDA   HARGS+29
   INY
   STA   (FP),Y
   LDY   #$C7
   LDA   (FP),Y
   LDY   #$CD
   STA   (FP),Y
   LDY   #$C8
   LDA   (FP),Y
   LDY   #$CE
   STA   (FP),Y
   LDY   #$C9
   LDA   (FP),Y
   LDY   #$CF
   STA   (FP),Y
   LDY   #$CA
   LDA   (FP),Y
   LDY   #$D0
   STA   (FP),Y
   LDY   #$CB
   LDA   (FP),Y
   LDY   #$D1
   STA   (FP),Y
   LDY   #$CC
   LDA   (FP),Y
   LDY   #$D2
   STA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@44
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@44
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@44
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@44
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@44
   DEY
   LDA   (FP),Y
   CMP   #$02
   BNE   .cmp_true@44
   LDA   #$00
   JMP   .cmp_end@45
.cmp_true@44:
   LDA   #$01
.cmp_end@45:
   LDY   #$D3
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
   LDY   #$D5
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   LDY   #$D6
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   LDY   #$D6
   LDA   (FP),Y
   LDY   #$02
   STA   (FP),Y
.if_end@7:
   LDA   #$FF
   LDY   #$D7
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDX   v2
   BEQ   .shift_done@48
.shift_loop@47:
   LDY   #$D7
   LDA   (FP),Y
   ASL
   STA   (FP),Y
   INY
   LDA   (FP),Y
   ROL
   STA   (FP),Y
   DEX
   BNE   .shift_loop@47
.shift_done@48:
   LDY   #$D7
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@8
   LDA   v1
   INY
   STA   (FP),Y
   LDA   v1+1
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@8
   LDA   #$01
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   JMP   .and_end@9
.and_false@8:
   LDA   #$00
   LDY   #$DB
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@9:
   LDY   #$DB
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lnot_true@49
   LDA   #$00
   JMP   .lnot_end@50
.lnot_true@49:
   LDA   #$01
.lnot_end@50:
   LDY   #$DD
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$DF
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$E0
   STA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@51
   DEY
   LDA   (FP),Y
   CMP   #$01
   BNE   .cmp_true@51
   LDA   #$00
   JMP   .cmp_end@52
.cmp_true@51:
   LDA   #$01
.cmp_end@52:
   LDY   #$E1
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
   LDY   #$E3
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   LDY   #$E4
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   LDY   #$E4
   LDA   (FP),Y
   LDY   #$02
   STA   (FP),Y
.if_end@10:
   LDA   v4
   LDY   #$E5
   STA   (FP),Y
   LDA   v4
   BMI   .sx_neg@54
   LDA   #$00
   JMP   .sx_done@55
.sx_neg@54:
   LDA   #$FF
.sx_done@55:
   LDY   #$E6
   STA   (FP),Y
   LDA   v2
   SEC
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
   LDY   #$E5
   LDA   (FP),Y
   LDY   #$E7
   EOR   (FP),Y
   LDY   #$E9
   STA   (FP),Y
   LDY   #$E6
   LDA   (FP),Y
   LDY   #$E8
   EOR   (FP),Y
   LDY   #$EA
   STA   (FP),Y
   SEC
   LDA   #$00
   DEY
   SBC   (FP),Y
   LDY   #$EB
   STA   (FP),Y
   LDA   #$00
   DEY
   SBC   (FP),Y
   LDY   #$EC
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$ED
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$EE
   STA   (FP),Y
   LDA   #$F2
   INY
   STA   (FP),Y
   LDA   #$FF
   INY
   STA   (FP),Y
   LDY   #$EE
   LDA   (FP),Y
   LDY   #$F0
   CMP   (FP),Y
   BNE   .cmp_true@56
   LDY   #$ED
   LDA   (FP),Y
   LDY   #$EF
   CMP   (FP),Y
   BNE   .cmp_true@56
   LDA   #$00
   JMP   .cmp_end@57
.cmp_true@56:
   LDA   #$01
.cmp_end@57:
   LDY   #$F1
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
   LDY   #$F3
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   LDY   #$F4
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   LDY   #$F4
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
   ADC   #$F6
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$F5
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
   DC.B  $0F
   DS.B  5

v1:
   DS.B  5
   DC.B  $02

v2:
   DS.B  4

v3:
   DS.B  1

v4:
   DC.B  $0F

v5:
   DC.W  $8000
