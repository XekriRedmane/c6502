SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24

check:
   SUBROUTINE

   ; prologue: 0 arg bytes, 164 local bytes
   SEC
   LDA   SSP
   SBC   #$A6
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$A5
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
   ORA   #$00
   BMI   .sx_neg@0
   LDA   #$00
   JMP   .sx_done@1
.sx_neg@0:
   LDA   #$FF
.sx_done@1:
   LDY   #$05
   STA   (FP),Y
   LDA   #$01
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
   LDA   v5
   LDY   #$0D
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDA   v3
   STA   HARGS
   LDA   v3+1
   STA   HARGS+1
   DEY
   LDA   (FP),Y
   STA   HARGS+2
   INY
   LDA   (FP),Y
   STA   HARGS+3
   JSR   sdivmod16
   LDA   HARGS+4
   LDY   #$0F
   STA   (FP),Y
   LDA   HARGS+5
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$11
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$12
   STA   (FP),Y
   LDA   #$31
   INY
   STA   (FP),Y
   LDA   #$FF
   INY
   STA   (FP),Y
   LDY   #$12
   LDA   (FP),Y
   LDY   #$14
   CMP   (FP),Y
   BNE   .cmp_true@7
   LDY   #$11
   LDA   (FP),Y
   LDY   #$13
   CMP   (FP),Y
   BNE   .cmp_true@7
   LDA   #$00
   JMP   .cmp_end@8
.cmp_true@7:
   LDA   #$01
.cmp_end@8:
   LDY   #$15
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
.if_end@1:
   LDA   v5
   LDY   #$19
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDA   v3
   SEC
   DEY
   SBC   (FP),Y
   LDY   #$1B
   STA   (FP),Y
   LDA   v3+1
   DEY
   SBC   (FP),Y
   LDY   #$1C
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   HARGS
   INY
   LDA   (FP),Y
   STA   HARGS+1
   LDA   v3
   STA   HARGS+2
   LDA   v3+1
   STA   HARGS+3
   JSR   sdivmod16
   LDA   HARGS+4
   LDY   #$1D
   STA   (FP),Y
   LDA   HARGS+5
   INY
   STA   (FP),Y
   LDA   v5
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDA   v0
   INY
   STA   (FP),Y
   LDA   v0
   BMI   .sx_neg@10
   LDA   #$00
   JMP   .sx_done@11
.sx_neg@10:
   LDA   #$FF
.sx_done@11:
   LDY   #$22
   STA   (FP),Y
   LDY   #$1F
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
   LDY   #$23
   STA   (FP),Y
   LDA   HARGS+7
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   HARGS
   INY
   LDA   (FP),Y
   STA   HARGS+1
   LDA   v3
   STA   HARGS+2
   LDA   v3+1
   STA   HARGS+3
   JSR   sdivmod16
   LDA   HARGS+4
   LDY   #$25
   STA   (FP),Y
   LDA   HARGS+5
   INY
   STA   (FP),Y
   LDY   #$1D
   LDA   (FP),Y
   LDY   #$25
   ORA   (FP),Y
   LDY   #$27
   STA   (FP),Y
   LDY   #$1E
   LDA   (FP),Y
   LDY   #$26
   ORA   (FP),Y
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
   CMP   #$00
   BNE   .cmp_true@12
   DEY
   LDA   (FP),Y
   CMP   #$01
   BNE   .cmp_true@12
   LDA   #$00
   JMP   .cmp_end@13
.cmp_true@12:
   LDA   #$01
.cmp_end@13:
   LDY   #$2B
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
   LDY   #$2D
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   LDY   #$2E
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   LDY   #$2E
   LDA   (FP),Y
   LDY   #$02
   STA   (FP),Y
.if_end@2:
   LDA   v3
   LDY   #$2F
   STA   (FP),Y
   LDA   v3+1
   INY
   STA   (FP),Y
   LDA   v3+1
   BMI   .sx_neg@15
   LDA   #$00
   JMP   .sx_done@16
.sx_neg@15:
   LDA   #$FF
.sx_done@16:
   LDY   #$31
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$2F
   LDA   (FP),Y
   SEC
   SBC   v2
   LDY   #$33
   STA   (FP),Y
   LDY   #$30
   LDA   (FP),Y
   SBC   v2+1
   LDY   #$34
   STA   (FP),Y
   LDY   #$31
   LDA   (FP),Y
   SBC   v2+2
   LDY   #$35
   STA   (FP),Y
   LDY   #$32
   LDA   (FP),Y
   SBC   v2+3
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
   LDY   #$36
   LDA   (FP),Y
   AND   #$7F
   LDY   #$3A
   STA   (FP),Y
   LDA   v0
   INY
   STA   (FP),Y
   LDA   v0
   BMI   .sx_neg@17
   LDA   #$00
   JMP   .sx_done@18
.sx_neg@17:
   LDA   #$FF
.sx_done@18:
   LDY   #$3C
   STA   (FP),Y
   LDA   v1
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$3B
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
   LDY   #$3F
   STA   (FP),Y
   LDA   HARGS+5
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$41
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$42
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@19
   LDA   #$00
   JMP   .sx_done@20
.sx_neg@19:
   LDA   #$FF
.sx_done@20:
   LDY   #$43
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$41
   LDA   (FP),Y
   LDY   #$45
   STA   (FP),Y
   LDY   #$42
   LDA   (FP),Y
   LDY   #$46
   STA   (FP),Y
   LDY   #$43
   LDA   (FP),Y
   LDY   #$47
   STA   (FP),Y
   AND   #$7F
   INY
   STA   (FP),Y
   LDY   #$37
   LDA   (FP),Y
   LDY   #$45
   ORA   (FP),Y
   LDY   #$49
   STA   (FP),Y
   LDY   #$38
   LDA   (FP),Y
   LDY   #$46
   ORA   (FP),Y
   LDY   #$4A
   STA   (FP),Y
   LDY   #$39
   LDA   (FP),Y
   LDY   #$47
   ORA   (FP),Y
   LDY   #$4B
   STA   (FP),Y
   LDY   #$3A
   LDA   (FP),Y
   LDY   #$48
   ORA   (FP),Y
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
   LDY   #$4B
   LDA   (FP),Y
   LDY   #$4F
   STA   (FP),Y
   LDY   #$4C
   LDA   (FP),Y
   LDY   #$50
   STA   (FP),Y
   CMP   #$7F
   BNE   .cmp_true@21
   DEY
   LDA   (FP),Y
   CMP   #$FF
   BNE   .cmp_true@21
   DEY
   LDA   (FP),Y
   CMP   #$98
   BNE   .cmp_true@21
   DEY
   LDA   (FP),Y
   CMP   #$FF
   BNE   .cmp_true@21
   LDA   #$00
   JMP   .cmp_end@22
.cmp_true@21:
   LDA   #$01
.cmp_end@22:
   LDY   #$51
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
.if_end@3:
   LDA   v3
   LDY   #$55
   STA   (FP),Y
   LDA   v3+1
   INY
   STA   (FP),Y
   LDA   v3+1
   BMI   .sx_neg@24
   LDA   #$00
   JMP   .sx_done@25
.sx_neg@24:
   LDA   #$FF
.sx_done@25:
   LDY   #$57
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$55
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
   LDA   #$45
   STA   HARGS+4
   LDA   #$23
   STA   HARGS+5
   LDA   #$01
   STA   HARGS+6
   LDA   #$00
   STA   HARGS+7
   JSR   udivmod32
   LDA   HARGS+8
   LDY   #$59
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
   LDA   v4
   INY
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
   LDY   #$5D
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
   LDY   #$61
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
   LDY   #$61
   LDA   (FP),Y
   CLC
   LDY   #$59
   SBC   (FP),Y
   LDY   #$62
   LDA   (FP),Y
   LDY   #$5A
   SBC   (FP),Y
   LDY   #$63
   LDA   (FP),Y
   LDY   #$5B
   SBC   (FP),Y
   LDY   #$64
   LDA   (FP),Y
   LDY   #$5C
   SBC   (FP),Y
   LDA   #$00
   ROL
   LDY   #$65
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$67
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$68
   STA   (FP),Y
   LDA   v0
   INY
   STA   (FP),Y
   LDA   v0
   BMI   .sx_neg@26
   LDA   #$00
   JMP   .sx_done@27
.sx_neg@26:
   LDA   #$FF
.sx_done@27:
   LDY   #$6A
   STA   (FP),Y
   LDA   v5
   BEQ   .and_false@4
   LDA   #$00
   INY
   STA   (FP),Y
   ORA   #$00
   BEQ   .and_false@4
   LDA   #$01
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   JMP   .and_end@5
.and_false@4:
   LDA   #$00
   LDY   #$6C
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@5:
   LDY   #$6C
   LDA   (FP),Y
   LDY   #$6E
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$6F
   STA   (FP),Y
   LDY   #$69
   LDA   (FP),Y
   STA   HARGS
   INY
   LDA   (FP),Y
   STA   HARGS+1
   LDY   #$6E
   LDA   (FP),Y
   STA   HARGS+2
   INY
   LDA   (FP),Y
   STA   HARGS+3
   JSR   mul16
   LDA   HARGS+4
   LDY   #$70
   STA   (FP),Y
   LDA   HARGS+5
   INY
   STA   (FP),Y
   LDY   #$67
   LDA   (FP),Y
   SEC
   LDY   #$70
   SBC   (FP),Y
   LDY   #$72
   STA   (FP),Y
   LDY   #$68
   LDA   (FP),Y
   LDY   #$71
   SBC   (FP),Y
   LDY   #$73
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$74
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$75
   STA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@28
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@28
   LDA   #$00
   JMP   .cmp_end@29
.cmp_true@28:
   LDA   #$01
.cmp_end@29:
   LDY   #$76
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
   LDY   #$78
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   LDY   #$79
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   LDY   #$79
   LDA   (FP),Y
   LDY   #$02
   STA   (FP),Y
.if_end@6:
   LDA   #$07
   LDY   #$7A
   STA   (FP),Y
   INY
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@31
   LDA   #$00
   JMP   .sx_done@32
.sx_neg@31:
   LDA   #$FF
.sx_done@32:
   LDY   #$7C
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$7D
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$7E
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@33
   LDA   #$00
   JMP   .sx_done@34
.sx_neg@33:
   LDA   #$FF
.sx_done@34:
   LDY   #$7F
   STA   (FP),Y
   INY
   STA   (FP),Y
   CMP   v4+3
   BNE   .thread@0
   CMP   v4+2
   BNE   .thread@0
   LDY   #$7E
   LDA   (FP),Y
   CMP   v4+1
   BNE   .thread@0
   DEY
   LDA   (FP),Y
   CMP   v4
   BEQ   .cmp_true@35
.thread@0:
   LDA   #$00
   JMP   .cmp_end@36
.cmp_true@35:
   LDA   #$01
.cmp_end@36:
   LDY   #$81
   STA   (FP),Y
   LDA   #$00
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
   LDY   #$85
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
   LDY   #$87
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   LDY   #$88
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   LDY   #$88
   LDA   (FP),Y
   LDY   #$02
   STA   (FP),Y
.if_end@7:
   LDA   v4
   LDY   #$89
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
   CMP   #$80
   BNE   .cmp_true@41
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@41
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@41
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@41
   LDA   #$00
   JMP   .cmp_end@42
.cmp_true@41:
   LDA   #$01
.cmp_end@42:
   LDY   #$8D
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
   LDY   #$8F
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   LDY   #$90
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   LDY   #$90
   LDA   (FP),Y
   LDY   #$02
   STA   (FP),Y
.if_end@8:
   LDA   v3
   LDY   #$91
   STA   (FP),Y
   LDA   v3+1
   INY
   STA   (FP),Y
   LDA   v3+1
   BMI   .sx_neg@44
   LDA   #$00
   JMP   .sx_done@45
.sx_neg@44:
   LDA   #$FF
.sx_done@45:
   LDY   #$93
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$91
   LDA   (FP),Y
   LDY   #$95
   STA   (FP),Y
   LDY   #$92
   LDA   (FP),Y
   LDY   #$96
   STA   (FP),Y
   LDY   #$93
   LDA   (FP),Y
   LDY   #$97
   STA   (FP),Y
   AND   #$7F
   INY
   STA   (FP),Y
   LDY   #$95
   LDA   (FP),Y
   SEC
   SBC   v2
   LDY   #$99
   STA   (FP),Y
   LDY   #$96
   LDA   (FP),Y
   SBC   v2+1
   LDY   #$9A
   STA   (FP),Y
   LDY   #$97
   LDA   (FP),Y
   SBC   v2+2
   LDY   #$9B
   STA   (FP),Y
   LDY   #$98
   LDA   (FP),Y
   SBC   v2+3
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
   LDY   #$9C
   LDA   (FP),Y
   LDY   #$A0
   STA   (FP),Y
   CMP   #$FF
   BNE   .cmp_true@46
   DEY
   LDA   (FP),Y
   CMP   #$FF
   BNE   .cmp_true@46
   DEY
   LDA   (FP),Y
   CMP   #$98
   BNE   .cmp_true@46
   DEY
   LDA   (FP),Y
   CMP   #$51
   BNE   .cmp_true@46
   LDA   #$00
   JMP   .cmp_end@47
.cmp_true@46:
   LDA   #$01
.cmp_end@47:
   LDY   #$A1
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
   ADC   #$A6
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$A5
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
   DC.B  $01

v1:
   DC.B  $FF

v2:
   DC.L  $80000000

v3:
   DC.W  $9851

v4:
   DC.L  $80000000

v5:
   DC.B  $80
