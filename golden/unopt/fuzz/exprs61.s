SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24

check:
   SUBROUTINE

   ; prologue: 0 arg bytes, 230 local bytes
   SEC
   LDA   SSP
   SBC   #$E8
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$E7
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
   LDA   v5+1
   STA   HARGS+1
   LDA   v5+2
   STA   HARGS+2
   LDA   v5+3
   STA   HARGS+3
   LDA   v5+4
   STA   HARGS+4
   LDA   v5+5
   STA   HARGS+5
   LDA   v5+6
   STA   HARGS+6
   LDA   #$00
   STA   HARGS+7
   LDA   v5
   STA   HARGS
   STA   HARGS+8
   JSR   asl64
   LDA   HARGS+9
   LDY   #$03
   STA   (FP),Y
   LDA   HARGS+10
   INY
   STA   (FP),Y
   LDA   HARGS+11
   INY
   STA   (FP),Y
   LDA   HARGS+12
   INY
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
   LDY   #$03
   LDA   (FP),Y
   LDY   #$0A
   STA   (FP),Y
   LDY   #$04
   LDA   (FP),Y
   LDY   #$0B
   STA   (FP),Y
   LDY   #$05
   LDA   (FP),Y
   LDY   #$0C
   STA   (FP),Y
   LDY   #$06
   LDA   (FP),Y
   LDY   #$0D
   STA   (FP),Y
   LDY   #$07
   LDA   (FP),Y
   LDY   #$0E
   STA   (FP),Y
   LDY   #$08
   LDA   (FP),Y
   LDY   #$0F
   STA   (FP),Y
   LDY   #$09
   LDA   (FP),Y
   AND   #$7F
   LDY   #$10
   STA   (FP),Y
   LDA   v4
   ORA   v4+1
   BNE   .or_true@0
   LDA   v0
   BNE   .or_true@0
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   JMP   .or_end@1
.or_true@0:
   LDA   #$01
   LDY   #$11
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@1:
   LDY   #$11
   LDA   (FP),Y
   LDY   #$13
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$14
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@0
   LDA   #$00
   JMP   .sx_done@1
.sx_neg@0:
   LDA   #$FF
.sx_done@1:
   LDY   #$15
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$13
   LDA   (FP),Y
   LDY   #$1A
   STA   (FP),Y
   LDY   #$14
   LDA   (FP),Y
   LDY   #$1B
   STA   (FP),Y
   LDY   #$15
   LDA   (FP),Y
   LDY   #$1C
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   AND   #$7F
   INY
   STA   (FP),Y
   LDY   #$1A
   LDA   (FP),Y
   SEC
   LDY   #$0A
   SBC   (FP),Y
   LDY   #$1B
   LDA   (FP),Y
   LDY   #$0B
   SBC   (FP),Y
   LDY   #$1C
   LDA   (FP),Y
   LDY   #$0C
   SBC   (FP),Y
   LDY   #$1D
   LDA   (FP),Y
   LDY   #$0D
   SBC   (FP),Y
   LDY   #$1E
   LDA   (FP),Y
   LDY   #$0E
   SBC   (FP),Y
   LDY   #$1F
   LDA   (FP),Y
   LDY   #$0F
   SBC   (FP),Y
   LDY   #$20
   LDA   (FP),Y
   LDY   #$10
   SBC   (FP),Y
   LDA   #$00
   ROL
   LDY   #$21
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$23
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$24
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@2
   LDA   #$00
   JMP   .sx_done@3
.sx_neg@2:
   LDA   #$FF
.sx_done@3:
   LDY   #$25
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$7F
   STA   HARGS
   LDA   #$00
   STA   HARGS+1
   LDY   #$00
   STY   HARGS+2
   STA   HARGS+3
   LDY   #$23
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
   LDY   #$27
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
   LDY   #$27
   LDA   (FP),Y
   LDY   #$2B
   STA   (FP),Y
   LDY   #$28
   LDA   (FP),Y
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
   CMP   #$00
   BNE   .cmp_true@4
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@4
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@4
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@4
   LDA   #$00
   JMP   .cmp_end@5
.cmp_true@4:
   LDA   #$01
.cmp_end@5:
   LDY   #$2F
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
   LDY   #$31
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   LDY   #$32
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   LDY   #$32
   LDA   (FP),Y
   LDY   #$02
   STA   (FP),Y
.if_end@2:
   LDA   v4
   LDY   #$33
   STA   (FP),Y
   LDA   v4+1
   INY
   STA   (FP),Y
   LDA   v4+1
   BMI   .sx_neg@7
   LDA   #$00
   JMP   .sx_done@8
.sx_neg@7:
   LDA   #$FF
.sx_done@8:
   LDY   #$35
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   v3
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
   ORA   #$00
   BMI   .sx_neg@9
   LDA   #$00
   JMP   .sx_done@10
.sx_neg@9:
   LDA   #$FF
.sx_done@10:
   LDY   #$3B
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$39
   LDA   (FP),Y
   SEC
   LDY   #$33
   SBC   (FP),Y
   LDY   #$3A
   LDA   (FP),Y
   LDY   #$34
   SBC   (FP),Y
   LDY   #$3B
   LDA   (FP),Y
   LDY   #$35
   SBC   (FP),Y
   LDY   #$3C
   LDA   (FP),Y
   LDY   #$36
   SBC   (FP),Y
   LDA   #$00
   ROL
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
   CMP   #$00
   BNE   .cmp_true@11
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@11
   LDA   #$00
   JMP   .cmp_end@12
.cmp_true@11:
   LDA   #$01
.cmp_end@12:
   LDY   #$41
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
   LDY   #$43
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   LDY   #$44
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   LDY   #$44
   LDA   (FP),Y
   LDY   #$02
   STA   (FP),Y
.if_end@3:
   LDA   v3
   LDY   #$45
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
   LDY   #$46
   LDA   (FP),Y
   LDY   #$49
   CMP   (FP),Y
   BNE   .cmp_true@14
   LDA   #$00
   JMP   .cmp_end@15
.cmp_true@14:
   LDA   #$01
.cmp_end@15:
   LDY   #$4B
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
   LDY   #$4D
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   LDY   #$4E
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   LDY   #$4E
   LDA   (FP),Y
   LDY   #$02
   STA   (FP),Y
.if_end@4:
   LDA   v5
   LDY   #$4F
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
   LDA   v5+4
   INY
   STA   (FP),Y
   LDA   v5+5
   INY
   STA   (FP),Y
   LDA   v5+6
   INY
   STA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@16
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@16
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@16
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@16
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@16
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@16
   DEY
   LDA   (FP),Y
   CMP   #$0A
   BNE   .cmp_true@16
   LDA   #$00
   JMP   .cmp_end@17
.cmp_true@16:
   LDA   #$01
.cmp_end@17:
   LDY   #$56
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
   LDY   #$58
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   LDY   #$59
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   LDY   #$59
   LDA   (FP),Y
   LDY   #$02
   STA   (FP),Y
.if_end@5:
   LDA   v0
   LDY   #$5A
   STA   (FP),Y
   LDA   v0
   BMI   .sx_neg@19
   LDA   #$00
   JMP   .sx_done@20
.sx_neg@19:
   LDA   #$FF
.sx_done@20:
   LDY   #$5B
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$5A
   LDA   (FP),Y
   LDY   #$61
   STA   (FP),Y
   LDY   #$5B
   LDA   (FP),Y
   LDY   #$62
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   AND   #$7F
   INY
   STA   (FP),Y
   LDY   #$61
   LDA   (FP),Y
   LDY   #$68
   STA   (FP),Y
   LDA   #$01
   LDY   #$62
   ORA   (FP),Y
   LDY   #$69
   STA   (FP),Y
   LDY   #$63
   LDA   (FP),Y
   LDY   #$6A
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$67
   LDA   (FP),Y
   LDY   #$6E
   STA   (FP),Y
   LDA   v5
   EOR   #$FF
   INY
   STA   (FP),Y
   LDA   v5+1
   EOR   #$FF
   INY
   STA   (FP),Y
   LDA   v5+2
   EOR   #$FF
   INY
   STA   (FP),Y
   LDA   v5+3
   EOR   #$FF
   INY
   STA   (FP),Y
   LDA   v5+4
   EOR   #$FF
   INY
   STA   (FP),Y
   LDA   v5+5
   EOR   #$FF
   INY
   STA   (FP),Y
   LDA   v5+6
   EOR   #$FF
   INY
   STA   (FP),Y
   LDY   #$6F
   LDA   (FP),Y
   LDY   #$76
   STA   (FP),Y
   LDY   #$70
   LDA   (FP),Y
   LDY   #$77
   STA   (FP),Y
   LDY   #$71
   LDA   (FP),Y
   LDY   #$78
   STA   (FP),Y
   LDY   #$72
   LDA   (FP),Y
   LDY   #$79
   STA   (FP),Y
   LDY   #$73
   LDA   (FP),Y
   LDY   #$7A
   STA   (FP),Y
   LDY   #$74
   LDA   (FP),Y
   LDY   #$7B
   STA   (FP),Y
   LDY   #$75
   LDA   (FP),Y
   AND   #$7F
   LDY   #$7C
   STA   (FP),Y
   LDY   #$68
   LDA   (FP),Y
   LDY   #$76
   EOR   (FP),Y
   LDY   #$7D
   STA   (FP),Y
   LDY   #$69
   LDA   (FP),Y
   LDY   #$77
   EOR   (FP),Y
   LDY   #$7E
   STA   (FP),Y
   LDY   #$6A
   LDA   (FP),Y
   LDY   #$78
   EOR   (FP),Y
   LDY   #$7F
   STA   (FP),Y
   LDY   #$6B
   LDA   (FP),Y
   LDY   #$79
   EOR   (FP),Y
   LDY   #$80
   STA   (FP),Y
   LDY   #$6C
   LDA   (FP),Y
   LDY   #$7A
   EOR   (FP),Y
   LDY   #$81
   STA   (FP),Y
   LDY   #$6D
   LDA   (FP),Y
   LDY   #$7B
   EOR   (FP),Y
   LDY   #$82
   STA   (FP),Y
   LDY   #$6E
   LDA   (FP),Y
   LDY   #$7C
   EOR   (FP),Y
   LDY   #$83
   STA   (FP),Y
   LDA   #$80
   LDY   #$7D
   ORA   (FP),Y
   LDY   #$84
   STA   (FP),Y
   LDY   #$7E
   LDA   (FP),Y
   LDY   #$85
   STA   (FP),Y
   LDY   #$7F
   LDA   (FP),Y
   LDY   #$86
   STA   (FP),Y
   LDY   #$80
   LDA   (FP),Y
   LDY   #$87
   STA   (FP),Y
   LDY   #$81
   LDA   (FP),Y
   LDY   #$88
   STA   (FP),Y
   LDY   #$82
   LDA   (FP),Y
   LDY   #$89
   STA   (FP),Y
   LDY   #$83
   LDA   (FP),Y
   LDY   #$8A
   STA   (FP),Y
   LDY   #$84
   LDA   (FP),Y
   LDY   #$8B
   STA   (FP),Y
   LDY   #$85
   LDA   (FP),Y
   LDY   #$8C
   STA   (FP),Y
   LDY   #$86
   LDA   (FP),Y
   LDY   #$8D
   STA   (FP),Y
   LDY   #$87
   LDA   (FP),Y
   LDY   #$8E
   STA   (FP),Y
   LDY   #$88
   LDA   (FP),Y
   LDY   #$8F
   STA   (FP),Y
   LDY   #$89
   LDA   (FP),Y
   LDY   #$90
   STA   (FP),Y
   LDY   #$8A
   LDA   (FP),Y
   LDY   #$91
   STA   (FP),Y
   CMP   #$7F
   BNE   .cmp_true@21
   DEY
   LDA   (FP),Y
   CMP   #$FF
   BNE   .cmp_true@21
   DEY
   LDA   (FP),Y
   CMP   #$FF
   BNE   .cmp_true@21
   DEY
   LDA   (FP),Y
   CMP   #$FF
   BNE   .cmp_true@21
   DEY
   LDA   (FP),Y
   CMP   #$FF
   BNE   .cmp_true@21
   DEY
   LDA   (FP),Y
   CMP   #$FE
   BNE   .cmp_true@21
   DEY
   LDA   (FP),Y
   CMP   #$F5
   BNE   .cmp_true@21
   LDA   #$00
   JMP   .cmp_end@22
.cmp_true@21:
   LDA   #$01
.cmp_end@22:
   LDY   #$92
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
   LDY   #$94
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   LDY   #$95
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   LDY   #$95
   LDA   (FP),Y
   LDY   #$02
   STA   (FP),Y
.if_end@6:
   SEC
   LDA   #$00
   SBC   #$7F
   LDY   #$96
   STA   (FP),Y
   LDA   #$00
   SBC   #$00
   INY
   STA   (FP),Y
   LDA   #$02
   STA   HARGS
   LDA   #$00
   STA   HARGS+1
   DEY
   LDA   (FP),Y
   STA   HARGS+2
   INY
   LDA   (FP),Y
   STA   HARGS+3
   JSR   sdivmod16
   LDA   HARGS+6
   LDY   #$98
   STA   (FP),Y
   LDA   HARGS+7
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$9A
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$9B
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@24
   LDA   #$00
   JMP   .sx_done@25
.sx_neg@24:
   LDA   #$FF
.sx_done@25:
   LDY   #$9C
   STA   (FP),Y
   INY
   STA   (FP),Y
   CMP   v1+3
   BNE   .cmp_true@26
   CMP   v1+2
   BNE   .cmp_true@26
   LDY   #$9B
   LDA   (FP),Y
   CMP   v1+1
   BNE   .cmp_true@26
   DEY
   LDA   (FP),Y
   CMP   v1
   BNE   .cmp_true@26
   LDA   #$00
   JMP   .cmp_end@27
.cmp_true@26:
   LDA   #$01
.cmp_end@27:
   LDY   #$9E
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$A0
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$A1
   STA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@29
   DEY
   LDA   (FP),Y
   CMP   #$01
   BNE   .cmp_true@29
   LDA   #$00
   JMP   .cmp_end@30
.cmp_true@29:
   LDA   #$01
.cmp_end@30:
   LDY   #$A2
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
   LDY   #$A4
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   LDY   #$A5
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   LDY   #$A5
   LDA   (FP),Y
   LDY   #$02
   STA   (FP),Y
.if_end@7:
   LDA   v2
   LDY   #$A6
   STA   (FP),Y
   LDA   v2
   BMI   .sx_neg@32
   LDA   #$00
   JMP   .sx_done@33
.sx_neg@32:
   LDA   #$FF
.sx_done@33:
   LDY   #$A7
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   SEC
   SBC   v4
   INY
   LDA   (FP),Y
   SBC   v4+1
   BVC   .cmp_novf@34
   EOR   #$80
.cmp_novf@34:
   ASL
   LDA   #$00
   ROL
   LDY   #$A8
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDA   v2
   INY
   STA   (FP),Y
   LDA   v2
   BMI   .sx_neg@35
   LDA   #$00
   JMP   .sx_done@36
.sx_neg@35:
   LDA   #$FF
.sx_done@36:
   LDY   #$AB
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   SEC
   LDY   #$A8
   SBC   (FP),Y
   LDY   #$AB
   LDA   (FP),Y
   LDY   #$A9
   SBC   (FP),Y
   BVC   .cmp_novf@37
   EOR   #$80
.cmp_novf@37:
   ASL
   LDA   #$00
   ROL
   EOR   #$01
   LDY   #$AC
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDA   v4
   CLC
   DEY
   ADC   (FP),Y
   LDY   #$AE
   STA   (FP),Y
   LDA   v4+1
   DEY
   ADC   (FP),Y
   LDY   #$AF
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$B0
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$B1
   STA   (FP),Y
   CMP   #$55
   BNE   .cmp_true@38
   DEY
   LDA   (FP),Y
   CMP   #$AB
   BNE   .cmp_true@38
   LDA   #$00
   JMP   .cmp_end@39
.cmp_true@38:
   LDA   #$01
.cmp_end@39:
   LDY   #$B2
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
   LDY   #$B4
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   LDY   #$B5
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   LDY   #$B5
   LDA   (FP),Y
   LDY   #$02
   STA   (FP),Y
.if_end@8:
   LDA   v1
   LDY   #$B6
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
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   v5
   SEC
   LDY   #$B6
   SBC   (FP),Y
   LDY   #$BD
   STA   (FP),Y
   LDA   v5+1
   LDY   #$B7
   SBC   (FP),Y
   LDY   #$BE
   STA   (FP),Y
   LDA   v5+2
   LDY   #$B8
   SBC   (FP),Y
   LDY   #$BF
   STA   (FP),Y
   LDA   v5+3
   LDY   #$B9
   SBC   (FP),Y
   LDY   #$C0
   STA   (FP),Y
   LDA   v5+4
   LDY   #$BA
   SBC   (FP),Y
   LDY   #$C1
   STA   (FP),Y
   LDA   v5+5
   LDY   #$BB
   SBC   (FP),Y
   LDY   #$C2
   STA   (FP),Y
   LDA   v5+6
   LDY   #$BC
   SBC   (FP),Y
   LDY   #$C3
   STA   (FP),Y
   LDY   #$BD
   LDA   (FP),Y
   LDY   #$C4
   STA   (FP),Y
   LDY   #$BE
   LDA   (FP),Y
   LDY   #$C5
   STA   (FP),Y
   LDY   #$BF
   LDA   (FP),Y
   LDY   #$C6
   STA   (FP),Y
   LDY   #$C0
   LDA   (FP),Y
   LDY   #$C7
   STA   (FP),Y
   LDY   #$C1
   LDA   (FP),Y
   LDY   #$C8
   STA   (FP),Y
   LDY   #$C2
   LDA   (FP),Y
   LDY   #$C9
   STA   (FP),Y
   LDY   #$C3
   LDA   (FP),Y
   AND   #$7F
   LDY   #$CA
   STA   (FP),Y
   LDA   v0
   INY
   STA   (FP),Y
   LDA   v0
   BMI   .sx_neg@41
   LDA   #$00
   JMP   .sx_done@42
.sx_neg@41:
   LDA   #$FF
.sx_done@42:
   LDY   #$CC
   STA   (FP),Y
   LDA   v4
   SEC
   DEY
   SBC   (FP),Y
   LDA   v4+1
   INY
   SBC   (FP),Y
   BVC   .cmp_novf@43
   EOR   #$80
.cmp_novf@43:
   ASL
   LDA   #$00
   ROL
   EOR   #$01
   LDY   #$CD
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$CF
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$D0
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@44
   LDA   #$00
   JMP   .sx_done@45
.sx_neg@44:
   LDA   #$FF
.sx_done@45:
   LDY   #$D1
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$CF
   LDA   (FP),Y
   LDY   #$D6
   STA   (FP),Y
   LDY   #$D0
   LDA   (FP),Y
   LDY   #$D7
   STA   (FP),Y
   LDY   #$D1
   LDA   (FP),Y
   LDY   #$D8
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   AND   #$7F
   INY
   STA   (FP),Y
   LDY   #$D6
   LDA   (FP),Y
   CLC
   LDY   #$C4
   SBC   (FP),Y
   LDY   #$D7
   LDA   (FP),Y
   LDY   #$C5
   SBC   (FP),Y
   LDY   #$D8
   LDA   (FP),Y
   LDY   #$C6
   SBC   (FP),Y
   LDY   #$D9
   LDA   (FP),Y
   LDY   #$C7
   SBC   (FP),Y
   LDY   #$DA
   LDA   (FP),Y
   LDY   #$C8
   SBC   (FP),Y
   LDY   #$DB
   LDA   (FP),Y
   LDY   #$C9
   SBC   (FP),Y
   LDY   #$DC
   LDA   (FP),Y
   LDY   #$CA
   SBC   (FP),Y
   LDA   #$00
   ROL
   LDY   #$DD
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@9
   LDA   v1
   ORA   v1+1
   ORA   v1+2
   ORA   v1+3
   BNE   .or_true@9
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   JMP   .or_end@10
.or_true@9:
   LDA   #$01
   LDY   #$DF
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@10:
   LDY   #$DF
   LDA   (FP),Y
   LDY   #$E1
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$E2
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@46
   LDA   #$00
   JMP   .cmp_end@47
.cmp_true@46:
   LDA   #$01
.cmp_end@47:
   LDY   #$E3
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
   LDY   #$E5
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   LDY   #$E6
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   LDY   #$E6
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
   ADC   #$E8
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$E7
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
   DS.B  4

v2:
   DC.B  $80

v3:
   DC.B  $80

v4:
   DC.W  $55AB

v5:
   DC.B  $0A
   DS.B  6
