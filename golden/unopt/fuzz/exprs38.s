SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24

check:
   SUBROUTINE

   ; prologue: 0 arg bytes, 179 local bytes
   SEC
   LDA   SSP
   SBC   #$B5
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$B4
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
   LDA   v3
   ORA   v3+1
   ORA   v3+2
   ORA   v3+3
   BEQ   .and_false@0
   LDA   v4
   INY
   STA   (FP),Y
   LDA   v4+1
   INY
   STA   (FP),Y
   LDA   v4+1
   BMI   .sx_neg@0
   LDA   #$00
   JMP   .sx_done@1
.sx_neg@0:
   LDA   #$FF
.sx_done@1:
   LDY   #$05
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$03
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   INY
   ORA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@0
   LDA   #$01
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   JMP   .and_end@1
.and_false@0:
   LDA   #$00
   LDY   #$07
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@1:
   LDY   #$07
   LDA   (FP),Y
   LDY   #$09
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$0A
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   HARGS
   INY
   LDA   (FP),Y
   STA   HARGS+1
   LDA   v2
   STA   HARGS+2
   LDA   v2+1
   STA   HARGS+3
   JSR   udivmod16
   LDA   HARGS+4
   LDY   #$0B
   STA   (FP),Y
   LDA   HARGS+5
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$0D
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$0E
   STA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@2
   DEY
   LDA   (FP),Y
   CMP   #$01
   BNE   .cmp_true@2
   LDA   #$00
   JMP   .cmp_end@3
.cmp_true@2:
   LDA   #$01
.cmp_end@3:
   LDY   #$0F
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
.if_end@2:
   LDA   #$45
   LDY   #$13
   STA   (FP),Y
   INY
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@5
   LDA   #$00
   JMP   .sx_done@6
.sx_neg@5:
   LDA   #$FF
.sx_done@6:
   LDY   #$15
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$16
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$17
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@7
   LDA   #$00
   JMP   .sx_done@8
.sx_neg@7:
   LDA   #$FF
.sx_done@8:
   LDY   #$18
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$16
   LDA   (FP),Y
   CLC
   SBC   #$8C
   INY
   LDA   (FP),Y
   SBC   #$4C
   INY
   LDA   (FP),Y
   SBC   #$12
   INY
   LDA   (FP),Y
   SBC   #$91
   LDA   #$00
   ROL
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$1C
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$1D
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@9
   LDA   #$00
   JMP   .sx_done@10
.sx_neg@9:
   LDA   #$FF
.sx_done@10:
   LDY   #$1E
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   v3
   STA   HARGS
   STA   HARGS+4
   LDA   v3+1
   STA   HARGS+1
   STA   HARGS+5
   LDA   v3+2
   STA   HARGS+2
   STA   HARGS+6
   LDA   v3+3
   STA   HARGS+3
   STA   HARGS+7
   JSR   mul32
   LDA   HARGS+8
   LDY   #$20
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
   LDY   #$1C
   LDA   (FP),Y
   LDY   #$20
   AND   (FP),Y
   LDY   #$24
   STA   (FP),Y
   LDY   #$1D
   LDA   (FP),Y
   LDY   #$21
   AND   (FP),Y
   LDY   #$25
   STA   (FP),Y
   LDY   #$1E
   LDA   (FP),Y
   LDY   #$22
   AND   (FP),Y
   LDY   #$26
   STA   (FP),Y
   LDY   #$1F
   LDA   (FP),Y
   LDY   #$23
   AND   (FP),Y
   LDY   #$27
   STA   (FP),Y
   LDY   #$24
   LDA   (FP),Y
   LDY   #$28
   STA   (FP),Y
   LDY   #$25
   LDA   (FP),Y
   LDY   #$29
   STA   (FP),Y
   LDY   #$26
   LDA   (FP),Y
   LDY   #$2A
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
   LDY   #$2B
   LDA   (FP),Y
   LDY   #$2F
   STA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@11
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@11
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@11
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@11
   LDA   #$00
   JMP   .cmp_end@12
.cmp_true@11:
   LDA   #$01
.cmp_end@12:
   LDY   #$30
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
   LDY   #$32
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   LDY   #$33
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   LDY   #$33
   LDA   (FP),Y
   LDY   #$02
   STA   (FP),Y
.if_end@3:
   LDA   v4
   LDY   #$34
   STA   (FP),Y
   LDA   v4+1
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDA   #$80
   INY
   STA   (FP),Y
   LDY   #$35
   LDA   (FP),Y
   LDY   #$37
   CMP   (FP),Y
   BNE   .cmp_true@14
   LDY   #$34
   LDA   (FP),Y
   LDY   #$36
   CMP   (FP),Y
   BNE   .cmp_true@14
   LDA   #$00
   JMP   .cmp_end@15
.cmp_true@14:
   LDA   #$01
.cmp_end@15:
   LDY   #$38
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
   LDY   #$3A
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   LDY   #$3B
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   LDY   #$3B
   LDA   (FP),Y
   LDY   #$02
   STA   (FP),Y
.if_end@4:
   LDA   v3+3
   CMP   v3+3
   BNE   .cmp_true@17
   LDA   v3+2
   CMP   v3+2
   BNE   .cmp_true@17
   LDA   v3+1
   CMP   v3+1
   BNE   .cmp_true@17
   LDA   v3
   CMP   v3
   BNE   .cmp_true@17
   LDA   #$00
   JMP   .cmp_end@18
.cmp_true@17:
   LDA   #$01
.cmp_end@18:
   LDY   #$3C
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$3E
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$3F
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@20
   LDA   #$00
   JMP   .sx_done@21
.sx_neg@20:
   LDA   #$FF
.sx_done@21:
   LDY   #$40
   STA   (FP),Y
   INY
   STA   (FP),Y
   SEC
   LDA   #$00
   SBC   v5
   INY
   STA   (FP),Y
   LDA   #$00
   SBC   v5+1
   INY
   STA   (FP),Y
   LDA   #$00
   SBC   v5+2
   INY
   STA   (FP),Y
   LDA   #$00
   SBC   v5+3
   INY
   STA   (FP),Y
   LDY   #$3E
   LDA   (FP),Y
   SEC
   LDY   #$42
   SBC   (FP),Y
   LDY   #$3F
   LDA   (FP),Y
   LDY   #$43
   SBC   (FP),Y
   LDY   #$40
   LDA   (FP),Y
   LDY   #$44
   SBC   (FP),Y
   LDY   #$41
   LDA   (FP),Y
   LDY   #$45
   SBC   (FP),Y
   BVC   .cmp_novf@22
   EOR   #$80
.cmp_novf@22:
   ASL
   LDA   #$00
   ROL
   LDY   #$46
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDA   v0
   STA   HARGS
   LDA   v0+1
   STA   HARGS+1
   LDA   v0+2
   STA   HARGS+2
   LDA   v0+3
   STA   HARGS+3
   DEY
   LDA   (FP),Y
   STA   HARGS+4
   JSR   asl32
   LDA   HARGS+5
   LDY   #$48
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
   LDY   #$4B
   LDA   (FP),Y
   LDY   #$4F
   STA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@23
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@23
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@23
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@23
   LDA   #$00
   JMP   .cmp_end@24
.cmp_true@23:
   LDA   #$01
.cmp_end@24:
   LDY   #$50
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
   LDY   #$52
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   LDY   #$53
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   LDY   #$53
   LDA   (FP),Y
   LDY   #$02
   STA   (FP),Y
.if_end@5:
   LDA   v1
   BEQ   .and_false@6
   LDA   v3
   LDY   #$54
   STA   (FP),Y
   ORA   #$00
   BEQ   .and_false@6
   LDA   #$01
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   JMP   .and_end@7
.and_false@6:
   LDA   #$00
   LDY   #$55
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@7:
   LDY   #$55
   LDA   (FP),Y
   LDY   #$57
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$58
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$59
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$5A
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@26
   LDA   #$00
   JMP   .cmp_end@27
.cmp_true@26:
   LDA   #$01
.cmp_end@27:
   LDY   #$5B
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
   LDY   #$5D
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   LDY   #$5E
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   LDY   #$5E
   LDA   (FP),Y
   LDY   #$02
   STA   (FP),Y
.if_end@8:
   LDA   #$77
   LDY   #$5F
   STA   (FP),Y
   LDA   #$CC
   INY
   STA   (FP),Y
   LDA   v0
   ORA   v0+1
   ORA   v0+2
   ORA   v0+3
   BEQ   .and_false@11
   LDA   v5
   ORA   v5+1
   ORA   v5+2
   ORA   v5+3
   BEQ   .and_false@11
   LDA   #$01
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   JMP   .and_end@12
.and_false@11:
   LDA   #$00
   LDY   #$61
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@12:
   LDY   #$61
   LDA   (FP),Y
   SEC
   LDY   #$5F
   SBC   (FP),Y
   LDY   #$62
   LDA   (FP),Y
   LDY   #$60
   SBC   (FP),Y
   BVC   .cmp_novf@28
   EOR   #$80
.cmp_novf@28:
   ASL
   LDA   #$00
   ROL
   EOR   #$01
   LDY   #$63
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@0
   JMP   .and_false@9
.lb_skip@0:
   LDA   v5
   LDY   #$65
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
   LDA   v3+3
   CMP   (FP),Y
   BNE   .thread@0
   LDA   v3+2
   DEY
   CMP   (FP),Y
   BNE   .thread@0
   LDA   v3+1
   DEY
   CMP   (FP),Y
   BNE   .thread@0
   LDA   v3
   DEY
   CMP   (FP),Y
   BEQ   .cmp_true@29
.thread@0:
   LDA   #$00
   JMP   .cmp_end@30
.cmp_true@29:
   LDA   #$01
.cmp_end@30:
   LDY   #$69
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$6B
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$6C
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@32
   LDA   #$00
   JMP   .sx_done@33
.sx_neg@32:
   LDA   #$FF
.sx_done@33:
   LDY   #$6D
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   v5
   SEC
   LDY   #$6B
   SBC   (FP),Y
   LDA   v5+1
   INY
   SBC   (FP),Y
   LDA   v5+2
   INY
   SBC   (FP),Y
   LDA   v5+3
   INY
   SBC   (FP),Y
   BVC   .cmp_novf@34
   EOR   #$80
.cmp_novf@34:
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
   INY
   ORA   (FP),Y
   BEQ   .and_false@9
   LDA   #$01
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   JMP   .and_end@10
.and_false@9:
   LDA   #$00
   LDY   #$71
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@10:
   LDY   #$71
   LDA   (FP),Y
   LDY   #$73
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$74
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CMP   #$01
   BNE   .cmp_true@35
   LDA   #$00
   JMP   .cmp_end@36
.cmp_true@35:
   LDA   #$01
.cmp_end@36:
   LDY   #$75
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@13
   LDY   #$01
   LDA   (FP),Y
   ORA   #$20
   LDY   #$77
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   LDY   #$78
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   LDY   #$78
   LDA   (FP),Y
   LDY   #$02
   STA   (FP),Y
.if_end@13:
   LDA   v2
   LDY   #$79
   STA   (FP),Y
   LDA   v2+1
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$79
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   INY
   ORA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@14
   LDA   #$FF
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@37
   LDA   #$00
   JMP   .sx_done@38
.sx_neg@37:
   LDA   #$FF
.sx_done@38:
   LDY   #$7F
   STA   (FP),Y
   SEC
   LDA   #$00
   DEY
   SBC   (FP),Y
   LDY   #$80
   STA   (FP),Y
   LDA   #$00
   DEY
   SBC   (FP),Y
   LDY   #$81
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@14
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   JMP   .or_end@15
.or_true@14:
   LDA   #$01
   LDY   #$82
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@15:
   LDY   #$82
   LDA   (FP),Y
   LDY   #$84
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$85
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CMP   #$01
   BNE   .cmp_true@39
   LDA   #$00
   JMP   .cmp_end@40
.cmp_true@39:
   LDA   #$01
.cmp_end@40:
   LDY   #$86
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@16
   LDY   #$01
   LDA   (FP),Y
   ORA   #$40
   LDY   #$88
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   LDY   #$89
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   LDY   #$89
   LDA   (FP),Y
   LDY   #$02
   STA   (FP),Y
.if_end@16:
   LDA   v1
   LDY   #$8A
   STA   (FP),Y
   LDA   v1
   BMI   .sx_neg@41
   LDA   #$00
   JMP   .sx_done@42
.sx_neg@41:
   LDA   #$FF
.sx_done@42:
   LDY   #$8B
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$8C
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$8D
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@43
   LDA   #$00
   JMP   .sx_done@44
.sx_neg@43:
   LDA   #$FF
.sx_done@44:
   LDY   #$8E
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$8C
   LDA   (FP),Y
   SEC
   SBC   v5
   INY
   LDA   (FP),Y
   SBC   v5+1
   INY
   LDA   (FP),Y
   SBC   v5+2
   INY
   LDA   (FP),Y
   SBC   v5+3
   BVC   .cmp_novf@45
   EOR   #$80
.cmp_novf@45:
   ASL
   LDA   #$00
   ROL
   EOR   #$01
   LDY   #$90
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@17
   LDA   v3
   ORA   v3+1
   ORA   v3+2
   ORA   v3+3
   BEQ   .and_false@19
   LDA   v5
   ORA   v5+1
   ORA   v5+2
   ORA   v5+3
   BEQ   .and_false@19
   LDA   #$01
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   JMP   .and_end@20
.and_false@19:
   LDA   #$00
   LDY   #$92
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@20:
   LDY   #$92
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@17
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   JMP   .or_end@18
.or_true@17:
   LDA   #$01
   LDY   #$94
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@18:
   LDY   #$94
   LDA   (FP),Y
   LDY   #$96
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$97
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@46
   LDA   #$00
   JMP   .sx_done@47
.sx_neg@46:
   LDA   #$FF
.sx_done@47:
   LDY   #$98
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   v0
   CLC
   ADC   v3
   INY
   STA   (FP),Y
   LDA   v0+1
   ADC   v3+1
   INY
   STA   (FP),Y
   LDA   v0+2
   ADC   v3+2
   INY
   STA   (FP),Y
   LDA   v0+3
   ADC   v3+3
   INY
   STA   (FP),Y
   LDA   v4
   ORA   v4+1
   BEQ   .and_false@21
   LDA   v2
   ORA   v2+1
   BEQ   .and_false@21
   LDA   #$01
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   JMP   .and_end@22
.and_false@21:
   LDA   #$00
   LDY   #$9E
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@22:
   LDY   #$9E
   LDA   (FP),Y
   LDY   #$A0
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$A1
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@48
   LDA   #$00
   JMP   .sx_done@49
.sx_neg@48:
   LDA   #$FF
.sx_done@49:
   LDY   #$A2
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$9A
   LDA   (FP),Y
   SEC
   LDY   #$A0
   SBC   (FP),Y
   LDY   #$A4
   STA   (FP),Y
   LDY   #$9B
   LDA   (FP),Y
   LDY   #$A1
   SBC   (FP),Y
   LDY   #$A5
   STA   (FP),Y
   LDY   #$9C
   LDA   (FP),Y
   LDY   #$A2
   SBC   (FP),Y
   LDY   #$A6
   STA   (FP),Y
   LDY   #$9D
   LDA   (FP),Y
   LDY   #$A3
   SBC   (FP),Y
   LDY   #$A7
   STA   (FP),Y
   LDY   #$96
   LDA   (FP),Y
   CLC
   LDY   #$A4
   ADC   (FP),Y
   LDY   #$A8
   STA   (FP),Y
   LDY   #$97
   LDA   (FP),Y
   LDY   #$A5
   ADC   (FP),Y
   LDY   #$A9
   STA   (FP),Y
   LDY   #$98
   LDA   (FP),Y
   LDY   #$A6
   ADC   (FP),Y
   LDY   #$AA
   STA   (FP),Y
   LDY   #$99
   LDA   (FP),Y
   LDY   #$A7
   ADC   (FP),Y
   LDY   #$AB
   STA   (FP),Y
   LDY   #$A8
   LDA   (FP),Y
   LDY   #$AC
   STA   (FP),Y
   LDY   #$A9
   LDA   (FP),Y
   LDY   #$AD
   STA   (FP),Y
   LDY   #$AA
   LDA   (FP),Y
   LDY   #$AE
   STA   (FP),Y
   LDY   #$AB
   LDA   (FP),Y
   LDY   #$AF
   STA   (FP),Y
   CMP   #$80
   BNE   .cmp_true@50
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@50
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@50
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@50
   LDA   #$00
   JMP   .cmp_end@51
.cmp_true@50:
   LDA   #$01
.cmp_end@51:
   LDY   #$B0
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@23
   LDY   #$01
   LDA   (FP),Y
   ORA   #$80
   LDY   #$B2
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   LDY   #$B3
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   LDY   #$B3
   LDA   (FP),Y
   LDY   #$02
   STA   (FP),Y
.if_end@23:
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
   ADC   #$B5
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$B4
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
   DS.B  4

v1:
   DS.B  1

v2:
   DC.W  $0001

v3:
   DC.L  $80000000

v4:
   DC.W  $8000

v5:
   DC.L  $FFFFFFFF
