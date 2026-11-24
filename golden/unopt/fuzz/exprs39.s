SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24

check:
   SUBROUTINE

   ; prologue: 17 arg bytes, 149 local bytes
   SEC
   LDA   SSP
   SBC   #$97
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$96
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
   LDA   #$07
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
   DEY
   LDA   (FP),Y
   LDY   #$06
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$07
   STA   (FP),Y
   LDY   #$99
   LDA   (FP),Y
   LDY   #$07
   CMP   (FP),Y
   BNE   .cmp_true@2
   LDY   #$98
   LDA   (FP),Y
   LDY   #$06
   CMP   (FP),Y
   BNE   .cmp_true@2
   LDA   #$00
   JMP   .cmp_end@3
.cmp_true@2:
   LDA   #$01
.cmp_end@3:
   LDY   #$08
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDA   #$FF
   BEQ   .and_false@0
   LDY   #$9A
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@0
   LDA   #$01
   LDY   #$0A
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   JMP   .and_end@1
.and_false@0:
   LDA   #$00
   LDY   #$0A
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@1:
   LDY   #$08
   LDA   (FP),Y
   SEC
   LDY   #$0A
   SBC   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$0B
   SBC   (FP),Y
   BVC   .cmp_novf@5
   EOR   #$80
.cmp_novf@5:
   ASL
   LDA   #$00
   ROL
   EOR   #$01
   LDY   #$0C
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$0E
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$0F
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@6
   LDA   #$00
   JMP   .sx_done@7
.sx_neg@6:
   LDA   #$FF
.sx_done@7:
   LDY   #$10
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$A1
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
   LDY   #$0E
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
   LDY   #$12
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
   LDY   #$12
   LDA   (FP),Y
   LDY   #$16
   STA   (FP),Y
   LDY   #$13
   LDA   (FP),Y
   LDY   #$17
   STA   (FP),Y
   LDY   #$14
   LDA   (FP),Y
   LDY   #$18
   STA   (FP),Y
   LDY   #$15
   LDA   (FP),Y
   LDY   #$19
   STA   (FP),Y
   CMP   #$AC
   BNE   .cmp_true@8
   DEY
   LDA   (FP),Y
   CMP   #$BF
   BNE   .cmp_true@8
   DEY
   LDA   (FP),Y
   CMP   #$57
   BNE   .cmp_true@8
   DEY
   LDA   (FP),Y
   CMP   #$09
   BNE   .cmp_true@8
   LDA   #$00
   JMP   .cmp_end@9
.cmp_true@8:
   LDA   #$01
.cmp_end@9:
   LDY   #$1A
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
   LDY   #$1C
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   LDY   #$1D
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   LDY   #$1D
   LDA   (FP),Y
   LDY   #$02
   STA   (FP),Y
.if_end@2:
   LDA   #$FF
   LDY   #$1E
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$7F
   INY
   STA   (FP),Y
   CMP   #$7F
   BNE   .cmp_true@11
   DEY
   LDA   (FP),Y
   CMP   #$FF
   BNE   .cmp_true@11
   DEY
   LDA   (FP),Y
   CMP   #$FF
   BNE   .cmp_true@11
   DEY
   LDA   (FP),Y
   CMP   #$FF
   BNE   .cmp_true@11
   LDA   #$00
   JMP   .cmp_end@12
.cmp_true@11:
   LDA   #$01
.cmp_end@12:
   LDY   #$22
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
   LDY   #$24
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   LDY   #$25
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   LDY   #$25
   LDA   (FP),Y
   LDY   #$02
   STA   (FP),Y
.if_end@3:
   LDY   #$A5
   LDA   (FP),Y
   LDY   #$26
   STA   (FP),Y
   LDY   #$A6
   LDA   (FP),Y
   LDY   #$27
   STA   (FP),Y
   LDY   #$A7
   LDA   (FP),Y
   LDY   #$28
   STA   (FP),Y
   LDY   #$A8
   LDA   (FP),Y
   LDY   #$29
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$FF
   SEC
   LDY   #$26
   SBC   (FP),Y
   LDA   #$7F
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
   INY
   SBC   (FP),Y
   LDA   #$00
   INY
   SBC   (FP),Y
   BVC   .cmp_novf@14
   EOR   #$80
.cmp_novf@14:
   ASL
   LDA   #$00
   ROL
   EOR   #$01
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
   CMP   #$00
   BNE   .cmp_true@15
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@15
   LDA   #$00
   JMP   .cmp_end@16
.cmp_true@15:
   LDA   #$01
.cmp_end@16:
   LDY   #$31
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
   LDY   #$33
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   LDY   #$34
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   LDY   #$34
   LDA   (FP),Y
   LDY   #$02
   STA   (FP),Y
.if_end@4:
   LDY   #$A1
   LDA   (FP),Y
   LDY   #$35
   STA   (FP),Y
   LDY   #$A2
   LDA   (FP),Y
   LDY   #$36
   STA   (FP),Y
   LDY   #$A3
   LDA   (FP),Y
   LDY   #$37
   STA   (FP),Y
   LDY   #$A4
   LDA   (FP),Y
   LDY   #$38
   STA   (FP),Y
   CMP   #$AC
   BNE   .cmp_true@18
   DEY
   LDA   (FP),Y
   CMP   #$BF
   BNE   .cmp_true@18
   DEY
   LDA   (FP),Y
   CMP   #$57
   BNE   .cmp_true@18
   DEY
   LDA   (FP),Y
   CMP   #$09
   BNE   .cmp_true@18
   LDA   #$00
   JMP   .cmp_end@19
.cmp_true@18:
   LDA   #$01
.cmp_end@19:
   LDY   #$39
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
.if_end@5:
   LDY   #$9A
   LDA   (FP),Y
   LDY   #$3D
   STA   (FP),Y
   LDY   #$9B
   LDA   (FP),Y
   LDY   #$3E
   STA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@21
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@21
   LDA   #$00
   JMP   .cmp_end@22
.cmp_true@21:
   LDA   #$01
.cmp_end@22:
   LDY   #$3F
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
.if_end@6:
   LDY   #$98
   LDA   (FP),Y
   LDY   #$43
   STA   (FP),Y
   LDY   #$99
   LDA   (FP),Y
   LDY   #$44
   STA   (FP),Y
   CMP   #$42
   BNE   .cmp_true@24
   DEY
   LDA   (FP),Y
   CMP   #$79
   BNE   .cmp_true@24
   LDA   #$00
   JMP   .cmp_end@25
.cmp_true@24:
   LDA   #$01
.cmp_end@25:
   LDY   #$45
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
.if_end@7:
   LDY   #$9A
   LDA   (FP),Y
   LDY   #$49
   STA   (FP),Y
   LDY   #$9B
   LDA   (FP),Y
   LDY   #$4A
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$9D
   LDA   (FP),Y
   LDY   #$49
   ORA   (FP),Y
   LDY   #$4D
   STA   (FP),Y
   LDY   #$9E
   LDA   (FP),Y
   LDY   #$4A
   ORA   (FP),Y
   LDY   #$4E
   STA   (FP),Y
   LDY   #$9F
   LDA   (FP),Y
   LDY   #$4B
   ORA   (FP),Y
   LDY   #$4F
   STA   (FP),Y
   LDY   #$A0
   LDA   (FP),Y
   LDY   #$4C
   ORA   (FP),Y
   LDY   #$50
   STA   (FP),Y
   LDY   #$A1
   LDA   (FP),Y
   LDY   #$51
   STA   (FP),Y
   LDY   #$A2
   LDA   (FP),Y
   LDY   #$52
   STA   (FP),Y
   LDY   #$A3
   LDA   (FP),Y
   LDY   #$53
   STA   (FP),Y
   LDY   #$A4
   LDA   (FP),Y
   LDY   #$54
   STA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@27
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@27
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@27
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@27
   LDA   #$00
   JMP   .cmp_end@28
.cmp_true@27:
   LDA   #$01
.cmp_end@28:
   LDY   #$55
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$57
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$58
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@30
   LDA   #$00
   JMP   .sx_done@31
.sx_neg@30:
   LDA   #$FF
.sx_done@31:
   LDY   #$59
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$4D
   LDA   (FP),Y
   LDY   #$57
   EOR   (FP),Y
   LDY   #$5B
   STA   (FP),Y
   LDY   #$4E
   LDA   (FP),Y
   LDY   #$58
   EOR   (FP),Y
   LDY   #$5C
   STA   (FP),Y
   LDY   #$4F
   LDA   (FP),Y
   LDY   #$59
   EOR   (FP),Y
   LDY   #$5D
   STA   (FP),Y
   LDY   #$50
   LDA   (FP),Y
   LDY   #$5A
   EOR   (FP),Y
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
   LDY   #$5E
   LDA   (FP),Y
   LDY   #$62
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
   ORA   #$00
   BEQ   .and_false@8
   LDY   #$98
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@8
   LDA   #$01
   LDY   #$67
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   JMP   .and_end@9
.and_false@8:
   LDA   #$00
   LDY   #$67
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@9:
   LDY   #$67
   LDA   (FP),Y
   LDY   #$69
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$6A
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@32
   LDA   #$00
   JMP   .sx_done@33
.sx_neg@32:
   LDA   #$FF
.sx_done@33:
   LDY   #$6B
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$5F
   LDA   (FP),Y
   CLC
   LDY   #$69
   SBC   (FP),Y
   LDY   #$60
   LDA   (FP),Y
   LDY   #$6A
   SBC   (FP),Y
   LDY   #$61
   LDA   (FP),Y
   LDY   #$6B
   SBC   (FP),Y
   LDY   #$62
   LDA   (FP),Y
   LDY   #$6C
   SBC   (FP),Y
   LDY   #$63
   LDA   (FP),Y
   LDY   #$6D
   SBC   (FP),Y
   LDY   #$64
   LDA   (FP),Y
   LDY   #$6E
   SBC   (FP),Y
   LDY   #$65
   LDA   (FP),Y
   LDY   #$6F
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
   LDY   #$72
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$73
   STA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@34
   DEY
   LDA   (FP),Y
   CMP   #$01
   BNE   .cmp_true@34
   LDA   #$00
   JMP   .cmp_end@35
.cmp_true@34:
   LDA   #$01
.cmp_end@35:
   LDY   #$74
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
   LDY   #$76
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   LDY   #$77
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   LDY   #$77
   LDA   (FP),Y
   LDY   #$02
   STA   (FP),Y
.if_end@10:
   LDA   #$FF
   LDY   #$78
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$7B
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$7C
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@37
   LDA   #$00
   JMP   .sx_done@38
.sx_neg@37:
   LDA   #$FF
.sx_done@38:
   LDY   #$7D
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$7B
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
   LDY   #$A5
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
   LDA   HARGS+8
   LDY   #$7F
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
   LDY   #$7F
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
   LDY   #$9A
   LDA   (FP),Y
   STA   HARGS+4
   JSR   lsr32
   LDA   HARGS+5
   LDY   #$83
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
   LDY   #$83
   LDA   (FP),Y
   LDY   #$87
   STA   (FP),Y
   LDY   #$84
   LDA   (FP),Y
   LDY   #$88
   STA   (FP),Y
   LDY   #$85
   LDA   (FP),Y
   LDY   #$89
   STA   (FP),Y
   LDY   #$86
   LDA   (FP),Y
   LDY   #$8A
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$02
   SEC
   LDY   #$87
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
   INY
   SBC   (FP),Y
   LDA   #$00
   INY
   SBC   (FP),Y
   BVC   .cmp_novf@39
   EOR   #$80
.cmp_novf@39:
   ASL
   LDA   #$00
   ROL
   LDY   #$8E
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$90
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$91
   STA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@40
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@40
   LDA   #$00
   JMP   .cmp_end@41
.cmp_true@40:
   LDA   #$01
.cmp_end@41:
   LDY   #$92
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
   ADC   #$A8
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$96
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
   LDA   #$79
   STA   (SSP),Y
   LDA   #$42
   INY
   STA   (SSP),Y
   LDA   #$00
   INY
   STA   (SSP),Y
   INY
   STA   (SSP),Y
   LDY   #$01
   LDA   (FP),Y
   LDY   #$05
   STA   (SSP),Y
   LDA   #$D5
   INY
   STA   (SSP),Y
   LDA   #$45
   INY
   STA   (SSP),Y
   LDA   #$DC
   INY
   STA   (SSP),Y
   LDA   #$B5
   INY
   STA   (SSP),Y
   LDA   #$09
   INY
   STA   (SSP),Y
   LDA   #$57
   INY
   STA   (SSP),Y
   LDA   #$BF
   INY
   STA   (SSP),Y
   LDA   #$AC
   INY
   STA   (SSP),Y
   LDA   #$FF
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
