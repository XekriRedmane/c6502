SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24

check:
   SUBROUTINE

   ; prologue: 0 arg bytes, 253 local bytes
   SEC
   LDA   SSP
   SBC   #$FF
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$FE
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
   LDA   v0+1
   INY
   STA   (FP),Y
   LDA   v0+2
   INY
   STA   (FP),Y
   LDA   v0+3
   INY
   STA   (FP),Y
   LDY   #$03
   LDA   (FP),Y
   LDA   #$00
   LDY   #$07
   STA   (FP),Y
   LDY   #$04
   LDA   (FP),Y
   LDA   #$00
   LDY   #$08
   STA   (FP),Y
   LDY   #$05
   LDA   (FP),Y
   LDA   #$00
   LDY   #$09
   STA   (FP),Y
   LDY   #$06
   LDA   (FP),Y
   LDA   #$00
   LDY   #$0A
   STA   (FP),Y
   LDY   #$07
   LDA   (FP),Y
   LDY   #$0B
   STA   (FP),Y
   LDY   #$08
   LDA   (FP),Y
   LDY   #$0C
   STA   (FP),Y
   LDY   #$09
   LDA   (FP),Y
   LDY   #$0D
   STA   (FP),Y
   LDY   #$0A
   LDA   (FP),Y
   LDY   #$0E
   STA   (FP),Y
   LDY   #$0B
   LDA   (FP),Y
   BNE   .cmp_true@0
   LDA   #$00
   JMP   .cmp_end@1
.cmp_true@0:
   LDA   #$01
.cmp_end@1:
   LDY   #$0F
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
   LDY   #$11
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   LDY   #$12
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   LDY   #$12
   LDA   (FP),Y
   LDY   #$02
   STA   (FP),Y
.if_end@0:
   LDA   v3
   CLC
   SBC   #$00
   LDA   v3+1
   SBC   #$00
   LDA   v3+2
   SBC   #$00
   LDA   v3+3
   SBC   #$00
   LDA   v3+4
   SBC   #$00
   LDA   #$00
   ROL
   LDY   #$13
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   SEC
   DEY
   SBC   (FP),Y
   LDY   #$15
   STA   (FP),Y
   LDA   #$00
   DEY
   SBC   (FP),Y
   LDY   #$16
   STA   (FP),Y
   LDA   v5
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$19
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$1A
   STA   (FP),Y
   LDA   v1+1
   CMP   (FP),Y
   BNE   .cmp_true@2
   LDA   v1
   DEY
   CMP   (FP),Y
   BNE   .cmp_true@2
   LDA   #$00
   JMP   .cmp_end@3
.cmp_true@2:
   LDA   #$01
.cmp_end@3:
   LDY   #$1B
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lnot_true@5
   LDA   #$00
   JMP   .lnot_end@6
.lnot_true@5:
   LDA   #$01
.lnot_end@6:
   LDY   #$1D
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$15
   LDA   (FP),Y
   STA   HARGS
   INY
   LDA   (FP),Y
   STA   HARGS+1
   LDY   #$1D
   LDA   (FP),Y
   STA   HARGS+2
   INY
   LDA   (FP),Y
   STA   HARGS+3
   JSR   mul16
   LDA   HARGS+4
   LDY   #$1F
   STA   (FP),Y
   LDA   HARGS+5
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
   BNE   .cmp_true@7
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@7
   LDA   #$00
   JMP   .cmp_end@8
.cmp_true@7:
   LDA   #$01
.cmp_end@8:
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
   LDA   v1
   LDY   #$27
   STA   (FP),Y
   LDA   v1+1
   INY
   STA   (FP),Y
   CMP   #$80
   BNE   .cmp_true@10
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@10
   LDA   #$00
   JMP   .cmp_end@11
.cmp_true@10:
   LDA   #$01
.cmp_end@11:
   LDY   #$29
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
.if_end@2:
   LDA   v5
   LDY   #$2D
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$2F
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$30
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@13
   LDA   #$00
   JMP   .sx_done@14
.sx_neg@13:
   LDA   #$FF
.sx_done@14:
   LDY   #$31
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$2F
   LDA   (FP),Y
   LDY   #$34
   STA   (FP),Y
   LDY   #$30
   LDA   (FP),Y
   LDY   #$35
   STA   (FP),Y
   LDY   #$31
   LDA   (FP),Y
   LDY   #$36
   STA   (FP),Y
   INY
   STA   (FP),Y
   AND   #$01
   INY
   STA   (FP),Y
   LDY   #$34
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
   LDA   v3
   STA   HARGS+8
   LDA   v3+1
   STA   HARGS+9
   LDA   v3+2
   STA   HARGS+10
   LDA   v3+3
   STA   HARGS+11
   LDA   v3+4
   STA   HARGS+12
   LDA   #$00
   STA   HARGS+5
   STA   HARGS+6
   STA   HARGS+7
   STA   HARGS+13
   STA   HARGS+14
   STA   HARGS+15
   JSR   mul64
   LDA   HARGS+16
   LDY   #$39
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
   LDY   #$39
   LDA   (FP),Y
   LDY   #$3E
   STA   (FP),Y
   LDY   #$3A
   LDA   (FP),Y
   LDY   #$3F
   STA   (FP),Y
   LDY   #$3B
   LDA   (FP),Y
   LDY   #$40
   STA   (FP),Y
   LDY   #$3C
   LDA   (FP),Y
   LDY   #$41
   STA   (FP),Y
   LDY   #$3D
   LDA   (FP),Y
   AND   #$01
   LDY   #$42
   STA   (FP),Y
   LDA   #$00
   CLC
   LDY   #$3E
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
   LDY   #$45
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$46
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@15
   LDA   #$00
   JMP   .sx_done@16
.sx_neg@15:
   LDA   #$FF
.sx_done@16:
   LDY   #$47
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$45
   LDA   (FP),Y
   LDY   #$4A
   STA   (FP),Y
   LDY   #$46
   LDA   (FP),Y
   LDY   #$4B
   STA   (FP),Y
   LDY   #$47
   LDA   (FP),Y
   LDY   #$4C
   STA   (FP),Y
   INY
   STA   (FP),Y
   AND   #$01
   INY
   STA   (FP),Y
   LDA   v3
   SEC
   LDY   #$4A
   SBC   (FP),Y
   LDY   #$4F
   STA   (FP),Y
   LDA   v3+1
   LDY   #$4B
   SBC   (FP),Y
   LDY   #$50
   STA   (FP),Y
   LDA   v3+2
   LDY   #$4C
   SBC   (FP),Y
   LDY   #$51
   STA   (FP),Y
   LDA   v3+3
   LDY   #$4D
   SBC   (FP),Y
   LDY   #$52
   STA   (FP),Y
   LDA   v3+4
   LDY   #$4E
   SBC   (FP),Y
   LDY   #$53
   STA   (FP),Y
   LDY   #$4F
   LDA   (FP),Y
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
   AND   #$01
   LDY   #$58
   STA   (FP),Y
   LDY   #$54
   LDA   (FP),Y
   LDY   #$59
   STA   (FP),Y
   LDY   #$55
   LDA   (FP),Y
   LDY   #$5A
   STA   (FP),Y
   LDY   #$56
   LDA   (FP),Y
   LDY   #$5B
   STA   (FP),Y
   LDY   #$57
   LDA   (FP),Y
   LDY   #$5C
   STA   (FP),Y
   LDY   #$58
   LDA   (FP),Y
   LDY   #$5D
   STA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@17
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@17
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@17
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@17
   DEY
   LDA   (FP),Y
   CMP   #$0E
   BNE   .cmp_true@17
   LDA   #$00
   JMP   .cmp_end@18
.cmp_true@17:
   LDA   #$01
.cmp_end@18:
   LDY   #$5E
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
   LDY   #$60
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   LDY   #$61
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   LDY   #$61
   LDA   (FP),Y
   LDY   #$02
   STA   (FP),Y
.if_end@3:
   LDA   #$02
   LDY   #$62
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$62
   LDA   (FP),Y
   CMP   #$02
   BNE   .cmp_true@20
   LDA   #$00
   JMP   .cmp_end@21
.cmp_true@20:
   LDA   #$01
.cmp_end@21:
   LDY   #$66
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
.if_end@4:
   LDA   v1
   LDY   #$6A
   STA   (FP),Y
   LDA   v1+1
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$6A
   LDA   (FP),Y
   ORA   v3
   LDY   #$6F
   STA   (FP),Y
   LDY   #$6B
   LDA   (FP),Y
   ORA   v3+1
   LDY   #$70
   STA   (FP),Y
   LDY   #$6C
   LDA   (FP),Y
   ORA   v3+2
   LDY   #$71
   STA   (FP),Y
   LDY   #$6D
   LDA   (FP),Y
   ORA   v3+3
   LDY   #$72
   STA   (FP),Y
   LDY   #$6E
   LDA   (FP),Y
   ORA   v3+4
   LDY   #$73
   STA   (FP),Y
   LDA   v1
   CLC
   SBC   v1
   LDA   v1+1
   SBC   v1+1
   LDA   #$00
   ROL
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
   BMI   .sx_neg@22
   LDA   #$00
   JMP   .sx_done@23
.sx_neg@22:
   LDA   #$FF
.sx_done@23:
   LDY   #$78
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
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
   INY
   STA   (FP),Y
   AND   #$01
   INY
   STA   (FP),Y
   LDY   #$6F
   LDA   (FP),Y
   SEC
   LDY   #$7B
   SBC   (FP),Y
   LDY   #$80
   STA   (FP),Y
   LDY   #$70
   LDA   (FP),Y
   LDY   #$7C
   SBC   (FP),Y
   LDY   #$81
   STA   (FP),Y
   LDY   #$71
   LDA   (FP),Y
   LDY   #$7D
   SBC   (FP),Y
   LDY   #$82
   STA   (FP),Y
   LDY   #$72
   LDA   (FP),Y
   LDY   #$7E
   SBC   (FP),Y
   LDY   #$83
   STA   (FP),Y
   LDY   #$73
   LDA   (FP),Y
   LDY   #$7F
   SBC   (FP),Y
   LDY   #$84
   STA   (FP),Y
   LDY   #$80
   LDA   (FP),Y
   LDY   #$85
   STA   (FP),Y
   LDY   #$81
   LDA   (FP),Y
   LDY   #$86
   STA   (FP),Y
   LDY   #$82
   LDA   (FP),Y
   LDY   #$87
   STA   (FP),Y
   LDY   #$83
   LDA   (FP),Y
   LDY   #$88
   STA   (FP),Y
   LDY   #$84
   LDA   (FP),Y
   AND   #$01
   LDY   #$89
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
   LDA   #$00
   INY
   STA   (FP),Y
   LDA   v3
   STA   HARGS
   LDA   v3+1
   STA   HARGS+1
   LDA   v3+2
   STA   HARGS+2
   LDA   v3+3
   STA   HARGS+3
   LDA   v3+4
   STA   HARGS+4
   LDA   #$00
   STA   HARGS+5
   STA   HARGS+6
   STA   HARGS+7
   LDY   #$8A
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
   LDA   #$00
   STA   HARGS+13
   STA   HARGS+14
   STA   HARGS+15
   JSR   udivmod64
   LDA   HARGS+24
   LDY   #$8F
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
   SEC
   LDA   #$00
   LDY   #$8F
   SBC   (FP),Y
   LDY   #$94
   STA   (FP),Y
   LDA   #$00
   LDY   #$90
   SBC   (FP),Y
   LDY   #$95
   STA   (FP),Y
   LDA   #$00
   LDY   #$91
   SBC   (FP),Y
   LDY   #$96
   STA   (FP),Y
   LDA   #$00
   LDY   #$92
   SBC   (FP),Y
   LDY   #$97
   STA   (FP),Y
   LDA   #$00
   LDY   #$93
   SBC   (FP),Y
   LDY   #$98
   STA   (FP),Y
   LDY   #$94
   LDA   (FP),Y
   LDY   #$99
   STA   (FP),Y
   LDY   #$95
   LDA   (FP),Y
   LDY   #$9A
   STA   (FP),Y
   LDY   #$96
   LDA   (FP),Y
   LDY   #$9B
   STA   (FP),Y
   LDY   #$97
   LDA   (FP),Y
   LDY   #$9C
   STA   (FP),Y
   LDY   #$98
   LDA   (FP),Y
   AND   #$01
   LDY   #$9D
   STA   (FP),Y
   LDY   #$99
   LDA   (FP),Y
   SEC
   LDY   #$85
   SBC   (FP),Y
   LDY   #$9A
   LDA   (FP),Y
   LDY   #$86
   SBC   (FP),Y
   LDY   #$9B
   LDA   (FP),Y
   LDY   #$87
   SBC   (FP),Y
   LDY   #$9C
   LDA   (FP),Y
   LDY   #$88
   SBC   (FP),Y
   LDY   #$9D
   LDA   (FP),Y
   LDY   #$89
   SBC   (FP),Y
   LDA   #$00
   ROL
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
   BNE   .cmp_true@24
   DEY
   LDA   (FP),Y
   CMP   #$01
   BNE   .cmp_true@24
   LDA   #$00
   JMP   .cmp_end@25
.cmp_true@24:
   LDA   #$01
.cmp_end@25:
   LDY   #$A2
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
.if_end@5:
   LDA   #$00
   LDY   #$A6
   STA   (FP),Y
   INY
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@27
   LDA   #$00
   JMP   .sx_done@28
.sx_neg@27:
   LDA   #$FF
.sx_done@28:
   LDY   #$A8
   STA   (FP),Y
   LDA   v1
   INY
   STA   (FP),Y
   LDA   v1+1
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$A9
   LDA   (FP),Y
   ORA   v4
   LDY   #$AD
   STA   (FP),Y
   LDY   #$AA
   LDA   (FP),Y
   ORA   v4+1
   LDY   #$AE
   STA   (FP),Y
   LDY   #$AB
   LDA   (FP),Y
   ORA   v4+2
   LDY   #$AF
   STA   (FP),Y
   LDY   #$AC
   LDA   (FP),Y
   ORA   v4+3
   LDY   #$B0
   STA   (FP),Y
   LDY   #$AD
   LDA   (FP),Y
   LDY   #$B1
   STA   (FP),Y
   LDY   #$AE
   LDA   (FP),Y
   LDY   #$B2
   STA   (FP),Y
   LDY   #$AF
   LDA   (FP),Y
   LDY   #$B3
   STA   (FP),Y
   LDY   #$B0
   LDA   (FP),Y
   LDY   #$B4
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$B1
   LDA   (FP),Y
   CLC
   SBC   v3
   INY
   LDA   (FP),Y
   SBC   v3+1
   INY
   LDA   (FP),Y
   SBC   v3+2
   INY
   LDA   (FP),Y
   SBC   v3+3
   INY
   LDA   (FP),Y
   SBC   v3+4
   LDA   #$00
   ROL
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$A7
   LDA   (FP),Y
   SEC
   LDY   #$B6
   SBC   (FP),Y
   LDY   #$A8
   LDA   (FP),Y
   LDY   #$B7
   SBC   (FP),Y
   BVC   .cmp_novf@29
   EOR   #$80
.cmp_novf@29:
   ASL
   LDA   #$00
   ROL
   LDY   #$B8
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$BA
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$BB
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
   LDY   #$BC
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
   ORA   #$40
   LDY   #$BE
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   LDY   #$BF
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   LDY   #$BF
   LDA   (FP),Y
   LDY   #$02
   STA   (FP),Y
.if_end@6:
   LDA   v0
   LDY   #$C0
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
   LDA   #$00
   INY
   STA   (FP),Y
   LDA   #$01
   SEC
   LDY   #$C0
   SBC   (FP),Y
   LDY   #$C5
   STA   (FP),Y
   LDA   #$00
   LDY   #$C1
   SBC   (FP),Y
   LDY   #$C6
   STA   (FP),Y
   LDA   #$00
   LDY   #$C2
   SBC   (FP),Y
   LDY   #$C7
   STA   (FP),Y
   LDA   #$00
   LDY   #$C3
   SBC   (FP),Y
   LDY   #$C8
   STA   (FP),Y
   LDA   #$00
   LDY   #$C4
   SBC   (FP),Y
   LDY   #$C9
   STA   (FP),Y
   LDY   #$C5
   LDA   (FP),Y
   LDY   #$CA
   STA   (FP),Y
   LDY   #$C6
   LDA   (FP),Y
   LDY   #$CB
   STA   (FP),Y
   LDY   #$C7
   LDA   (FP),Y
   LDY   #$CC
   STA   (FP),Y
   LDY   #$C8
   LDA   (FP),Y
   LDY   #$CD
   STA   (FP),Y
   LDY   #$C9
   LDA   (FP),Y
   AND   #$01
   LDY   #$CE
   STA   (FP),Y
   LDY   #$CA
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   INY
   ORA   (FP),Y
   INY
   ORA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lnot_true@33
   LDA   #$00
   JMP   .lnot_end@34
.lnot_true@33:
   LDA   #$01
.lnot_end@34:
   LDY   #$CF
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$D1
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$D2
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@35
   LDA   #$00
   JMP   .sx_done@36
.sx_neg@35:
   LDA   #$FF
.sx_done@36:
   LDY   #$D3
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$D1
   LDA   (FP),Y
   LDY   #$D6
   STA   (FP),Y
   LDY   #$D2
   LDA   (FP),Y
   LDY   #$D7
   STA   (FP),Y
   LDY   #$D3
   LDA   (FP),Y
   LDY   #$D8
   STA   (FP),Y
   INY
   STA   (FP),Y
   AND   #$01
   INY
   STA   (FP),Y
   LDA   v5
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$DD
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$DE
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@37
   LDA   #$00
   JMP   .sx_done@38
.sx_neg@37:
   LDA   #$FF
.sx_done@38:
   LDY   #$DF
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$DD
   LDA   (FP),Y
   LDY   #$E2
   STA   (FP),Y
   LDY   #$DE
   LDA   (FP),Y
   LDY   #$E3
   STA   (FP),Y
   LDY   #$DF
   LDA   (FP),Y
   LDY   #$E4
   STA   (FP),Y
   INY
   STA   (FP),Y
   AND   #$01
   INY
   STA   (FP),Y
   LDA   v3
   STA   HARGS
   LDA   v3+1
   STA   HARGS+1
   LDA   v3+2
   STA   HARGS+2
   LDA   v3+3
   STA   HARGS+3
   LDA   v3+4
   STA   HARGS+4
   LDA   #$00
   STA   HARGS+5
   STA   HARGS+6
   STA   HARGS+7
   LDY   #$E2
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
   LDA   #$00
   STA   HARGS+13
   STA   HARGS+14
   STA   HARGS+15
   JSR   udivmod64
   LDA   HARGS+24
   LDY   #$E7
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
   LDY   #$E7
   LDA   (FP),Y
   EOR   #$FF
   LDY   #$EC
   STA   (FP),Y
   LDY   #$E8
   LDA   (FP),Y
   EOR   #$FF
   LDY   #$ED
   STA   (FP),Y
   LDY   #$E9
   LDA   (FP),Y
   EOR   #$FF
   LDY   #$EE
   STA   (FP),Y
   LDY   #$EA
   LDA   (FP),Y
   EOR   #$FF
   LDY   #$EF
   STA   (FP),Y
   LDY   #$EB
   LDA   (FP),Y
   EOR   #$FF
   LDY   #$F0
   STA   (FP),Y
   LDY   #$EC
   LDA   (FP),Y
   LDY   #$F1
   STA   (FP),Y
   LDY   #$ED
   LDA   (FP),Y
   LDY   #$F2
   STA   (FP),Y
   LDY   #$EE
   LDA   (FP),Y
   LDY   #$F3
   STA   (FP),Y
   LDY   #$EF
   LDA   (FP),Y
   LDY   #$F4
   STA   (FP),Y
   LDY   #$F0
   LDA   (FP),Y
   AND   #$01
   LDY   #$F5
   STA   (FP),Y
   LDY   #$DA
   LDA   (FP),Y
   LDY   #$F5
   CMP   (FP),Y
   BNE   .cmp_true@39
   LDY   #$D9
   LDA   (FP),Y
   LDY   #$F4
   CMP   (FP),Y
   BNE   .cmp_true@39
   LDY   #$D8
   LDA   (FP),Y
   LDY   #$F3
   CMP   (FP),Y
   BNE   .cmp_true@39
   LDY   #$D7
   LDA   (FP),Y
   LDY   #$F2
   CMP   (FP),Y
   BNE   .cmp_true@39
   LDY   #$D6
   LDA   (FP),Y
   LDY   #$F1
   CMP   (FP),Y
   BNE   .cmp_true@39
   LDA   #$00
   JMP   .cmp_end@40
.cmp_true@39:
   LDA   #$01
.cmp_end@40:
   LDY   #$F6
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$F8
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$F9
   STA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@42
   DEY
   LDA   (FP),Y
   CMP   #$01
   BNE   .cmp_true@42
   LDA   #$00
   JMP   .cmp_end@43
.cmp_true@42:
   LDA   #$01
.cmp_end@43:
   LDY   #$FA
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
   ORA   #$80
   LDY   #$FC
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   LDY   #$FD
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   LDY   #$FD
   LDA   (FP),Y
   LDY   #$02
   STA   (FP),Y
.if_end@7:
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
   ADC   #$FF
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$FE
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
   DC.L  $10000000

v1:
   DC.W  $8000

v2:
   DC.B  $08

v3:
   DC.B  $0E
   DS.B  4

v4:
   DS.B  4

v5:
   DC.B  $FF
