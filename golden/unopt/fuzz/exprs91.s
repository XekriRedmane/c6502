SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24

check:
   SUBROUTINE

   ; prologue: 0 arg bytes, 159 local bytes
   SEC
   LDA   SSP
   SBC   #$A1
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$A0
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
   LDA   v2
   ORA   v4
   INY
   STA   (FP),Y
   LDA   v2+1
   ORA   v4+1
   INY
   STA   (FP),Y
   LDA   v2+2
   ORA   v4+2
   INY
   STA   (FP),Y
   LDA   v4
   ORA   v4+1
   ORA   v4+2
   BEQ   .and_false@0
   LDA   v5
   ORA   v5+1
   ORA   v5+2
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
   LDY   #$06
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@1:
   LDY   #$03
   LDA   (FP),Y
   STA   HARGS
   INY
   LDA   (FP),Y
   STA   HARGS+1
   INY
   LDA   (FP),Y
   STA   HARGS+2
   LDA   #$00
   STA   HARGS+3
   INY
   LDA   (FP),Y
   STA   HARGS+4
   JSR   lsr32
   LDA   HARGS+5
   LDY   #$08
   STA   (FP),Y
   LDA   HARGS+6
   INY
   STA   (FP),Y
   LDA   HARGS+7
   INY
   STA   (FP),Y
   LDY   #$08
   LDA   (FP),Y
   STA   HARGS
   INY
   LDA   (FP),Y
   STA   HARGS+1
   INY
   LDA   (FP),Y
   STA   HARGS+2
   LDA   v4
   STA   HARGS+4
   LDA   v4+1
   STA   HARGS+5
   LDA   v4+2
   STA   HARGS+6
   LDA   #$00
   STA   HARGS+3
   STA   HARGS+7
   JSR   udivmod32
   LDA   HARGS+12
   LDY   #$0B
   STA   (FP),Y
   LDA   HARGS+13
   INY
   STA   (FP),Y
   LDA   HARGS+14
   INY
   STA   (FP),Y
   LDY   #$0B
   LDA   (FP),Y
   LDY   #$0E
   STA   (FP),Y
   LDY   #$0C
   LDA   (FP),Y
   LDY   #$0F
   STA   (FP),Y
   LDY   #$0D
   LDA   (FP),Y
   LDY   #$10
   STA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@0
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@0
   DEY
   LDA   (FP),Y
   CMP   #$09
   BNE   .cmp_true@0
   LDA   #$00
   JMP   .cmp_end@1
.cmp_true@0:
   LDA   #$01
.cmp_end@1:
   LDY   #$11
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
   LDY   #$13
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   LDY   #$14
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   LDY   #$14
   LDA   (FP),Y
   LDY   #$02
   STA   (FP),Y
.if_end@2:
   LDA   v2
   EOR   v5
   LDY   #$15
   STA   (FP),Y
   LDA   v2+1
   EOR   v5+1
   INY
   STA   (FP),Y
   LDA   v2+2
   EOR   v5+2
   INY
   STA   (FP),Y
   LDY   #$15
   LDA   (FP),Y
   ORA   v4
   LDY   #$18
   STA   (FP),Y
   LDY   #$16
   LDA   (FP),Y
   ORA   v4+1
   LDY   #$19
   STA   (FP),Y
   LDY   #$17
   LDA   (FP),Y
   ORA   v4+2
   LDY   #$1A
   STA   (FP),Y
   LDY   #$18
   LDA   (FP),Y
   LDY   #$1B
   STA   (FP),Y
   LDY   #$19
   LDA   (FP),Y
   LDY   #$1C
   STA   (FP),Y
   LDY   #$1A
   LDA   (FP),Y
   LDY   #$1D
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
   SEC
   SBC   #$11
   INY
   LDA   (FP),Y
   SBC   #$04
   INY
   LDA   (FP),Y
   SBC   #$C2
   INY
   LDA   (FP),Y
   SBC   #$28
   INY
   LDA   (FP),Y
   SBC   #$91
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
   LDY   #$23
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$24
   STA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@3
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@3
   LDA   #$00
   JMP   .cmp_end@4
.cmp_true@3:
   LDA   #$01
.cmp_end@4:
   LDY   #$25
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
.if_end@3:
   LDA   v4
   ORA   v4+1
   ORA   v4+2
   BEQ   .lnot_true@6
   LDA   #$00
   JMP   .lnot_end@7
.lnot_true@6:
   LDA   #$01
.lnot_end@7:
   LDY   #$29
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDA   v0
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$2D
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$2E
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@8
   LDA   #$00
   JMP   .sx_done@9
.sx_neg@8:
   LDA   #$FF
.sx_done@9:
   LDY   #$2F
   STA   (FP),Y
   LDY   #$2D
   LDA   (FP),Y
   LDY   #$30
   STA   (FP),Y
   LDY   #$2E
   LDA   (FP),Y
   LDY   #$31
   STA   (FP),Y
   LDY   #$2F
   LDA   (FP),Y
   AND   #$07
   LDY   #$32
   STA   (FP),Y
   CMP   v2+2
   BNE   .thread@0
   DEY
   LDA   (FP),Y
   CMP   v2+1
   BNE   .thread@0
   DEY
   LDA   (FP),Y
   CMP   v2
   BEQ   .cmp_true@10
.thread@0:
   LDA   #$00
   JMP   .cmp_end@11
.cmp_true@10:
   LDA   #$01
.cmp_end@11:
   LDY   #$33
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$29
   LDA   (FP),Y
   SEC
   LDY   #$33
   SBC   (FP),Y
   LDY   #$2A
   LDA   (FP),Y
   LDY   #$34
   SBC   (FP),Y
   BVC   .cmp_novf@13
   EOR   #$80
.cmp_novf@13:
   ASL
   LDA   #$00
   ROL
   LDY   #$35
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@4
   SEC
   LDA   #$00
   SBC   #$45
   INY
   STA   (FP),Y
   LDA   #$00
   SBC   #$23
   INY
   STA   (FP),Y
   LDA   #$00
   SBC   #$01
   INY
   STA   (FP),Y
   LDY   #$37
   LDA   (FP),Y
   LDY   #$3A
   STA   (FP),Y
   LDY   #$38
   LDA   (FP),Y
   LDY   #$3B
   STA   (FP),Y
   LDY   #$39
   LDA   (FP),Y
   AND   #$1F
   LDY   #$3C
   STA   (FP),Y
   LDA   v2
   LDY   #$3A
   ORA   (FP),Y
   LDY   #$3D
   STA   (FP),Y
   LDA   v2+1
   LDY   #$3B
   ORA   (FP),Y
   LDY   #$3E
   STA   (FP),Y
   LDA   v2+2
   LDY   #$3C
   ORA   (FP),Y
   LDY   #$3F
   STA   (FP),Y
   LDY   #$3D
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   INY
   ORA   (FP),Y
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
   LDY   #$40
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@5:
   LDY   #$40
   LDA   (FP),Y
   LDY   #$42
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$43
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@14
   LDA   #$00
   JMP   .cmp_end@15
.cmp_true@14:
   LDA   #$01
.cmp_end@15:
   LDY   #$44
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
   ORA   #$04
   LDY   #$46
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   LDY   #$47
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   LDY   #$47
   LDA   (FP),Y
   LDY   #$02
   STA   (FP),Y
.if_end@6:
   LDA   v0
   LDY   #$48
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
   LDY   #$48
   LDA   (FP),Y
   LDY   #$4B
   ORA   (FP),Y
   LDY   #$4D
   STA   (FP),Y
   LDY   #$49
   LDA   (FP),Y
   LDY   #$4C
   ORA   (FP),Y
   LDY   #$4E
   STA   (FP),Y
   LDA   #$80
   INY
   STA   (FP),Y
   ORA   #$00
   BEQ   .lnot_true@16
   LDA   #$00
   JMP   .lnot_end@17
.lnot_true@16:
   LDA   #$01
.lnot_end@17:
   LDY   #$50
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$52
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$53
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@18
   LDA   #$00
   JMP   .sx_done@19
.sx_neg@18:
   LDA   #$FF
.sx_done@19:
   LDY   #$54
   STA   (FP),Y
   LDY   #$52
   LDA   (FP),Y
   LDY   #$55
   STA   (FP),Y
   LDY   #$53
   LDA   (FP),Y
   LDY   #$56
   STA   (FP),Y
   LDY   #$54
   LDA   (FP),Y
   AND   #$07
   LDY   #$57
   STA   (FP),Y
   LDA   v0
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDA   v4
   STA   HARGS
   LDA   v4+1
   STA   HARGS+1
   LDA   v4+2
   STA   HARGS+2
   LDA   #$00
   STA   HARGS+3
   DEY
   LDA   (FP),Y
   STA   HARGS+4
   JSR   asl32
   LDA   HARGS+5
   LDY   #$5A
   STA   (FP),Y
   LDA   HARGS+6
   INY
   STA   (FP),Y
   LDA   HARGS+7
   INY
   STA   (FP),Y
   LDY   #$5A
   LDA   (FP),Y
   LDY   #$5D
   STA   (FP),Y
   LDY   #$5B
   LDA   (FP),Y
   LDY   #$5E
   STA   (FP),Y
   LDY   #$5C
   LDA   (FP),Y
   AND   #$07
   LDY   #$5F
   STA   (FP),Y
   LDY   #$55
   LDA   (FP),Y
   CLC
   LDY   #$5D
   SBC   (FP),Y
   LDY   #$56
   LDA   (FP),Y
   LDY   #$5E
   SBC   (FP),Y
   LDY   #$57
   LDA   (FP),Y
   LDY   #$5F
   SBC   (FP),Y
   LDA   #$00
   ROL
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$4D
   LDA   (FP),Y
   SEC
   LDY   #$60
   SBC   (FP),Y
   LDY   #$62
   STA   (FP),Y
   LDY   #$4E
   LDA   (FP),Y
   LDY   #$61
   SBC   (FP),Y
   LDY   #$63
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$64
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$65
   STA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@20
   DEY
   LDA   (FP),Y
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
   BEQ   .if_end@7
   LDY   #$01
   LDA   (FP),Y
   ORA   #$08
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
.if_end@7:
   LDA   #$65
   LDY   #$6A
   STA   (FP),Y
   ORA   #$00
   BEQ   .lnot_true@23
   LDA   #$00
   JMP   .lnot_end@24
.lnot_true@23:
   LDA   #$01
.lnot_end@24:
   LDY   #$6B
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
   LDA   #$48
   DEY
   ORA   (FP),Y
   LDY   #$6F
   STA   (FP),Y
   LDA   #$BD
   DEY
   ORA   (FP),Y
   LDY   #$70
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$71
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$72
   STA   (FP),Y
   CMP   #$BD
   BNE   .cmp_true@25
   DEY
   LDA   (FP),Y
   CMP   #$48
   BNE   .cmp_true@25
   LDA   #$00
   JMP   .cmp_end@26
.cmp_true@25:
   LDA   #$01
.cmp_end@26:
   LDY   #$73
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
.if_end@8:
   LDA   v0
   LDY   #$77
   STA   (FP),Y
   LDA   #$00
   INY
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
   BMI   .sx_neg@28
   LDA   #$00
   JMP   .sx_done@29
.sx_neg@28:
   LDA   #$FF
.sx_done@29:
   LDY   #$7B
   STA   (FP),Y
   LDY   #$79
   LDA   (FP),Y
   LDY   #$7C
   STA   (FP),Y
   LDY   #$7A
   LDA   (FP),Y
   LDY   #$7D
   STA   (FP),Y
   LDY   #$7B
   LDA   (FP),Y
   AND   #$07
   LDY   #$7E
   STA   (FP),Y
   LDY   #$7C
   LDA   (FP),Y
   ORA   v4
   LDY   #$7F
   STA   (FP),Y
   LDY   #$7D
   LDA   (FP),Y
   ORA   v4+1
   LDY   #$80
   STA   (FP),Y
   LDY   #$7E
   LDA   (FP),Y
   ORA   v4+2
   LDY   #$81
   STA   (FP),Y
   LDY   #$7F
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@9
   LDA   #$FF
   EOR   #$FF
   INY
   STA   (FP),Y
   LDA   #$FF
   EOR   #$FF
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
   LDY   #$84
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@10:
   LDY   #$84
   LDA   (FP),Y
   LDY   #$86
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$87
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@30
   LDA   #$00
   JMP   .cmp_end@31
.cmp_true@30:
   LDA   #$01
.cmp_end@31:
   LDY   #$88
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
   ORA   #$20
   LDY   #$8A
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   LDY   #$8B
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   LDY   #$8B
   LDA   (FP),Y
   LDY   #$02
   STA   (FP),Y
.if_end@11:
   LDA   #$0D
   LDY   #$8C
   STA   (FP),Y
   LDA   #$15
   INY
   STA   (FP),Y
   LDA   #$06
   INY
   STA   (FP),Y
   CMP   #$06
   BNE   .cmp_true@32
   DEY
   LDA   (FP),Y
   CMP   #$15
   BNE   .cmp_true@32
   DEY
   LDA   (FP),Y
   CMP   #$0D
   BNE   .cmp_true@32
   LDA   #$00
   JMP   .cmp_end@33
.cmp_true@32:
   LDA   #$01
.cmp_end@33:
   LDY   #$8F
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
   LDY   #$91
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   LDY   #$92
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   LDY   #$92
   LDA   (FP),Y
   LDY   #$02
   STA   (FP),Y
.if_end@12:
   LDA   v1
   ORA   v1
   LDY   #$93
   STA   (FP),Y
   LDA   v1+1
   ORA   v1+1
   INY
   STA   (FP),Y
   LDA   #$02
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$94
   LDA   (FP),Y
   LDY   #$97
   CMP   (FP),Y
   BNE   .cmp_true@35
   LDY   #$93
   LDA   (FP),Y
   LDY   #$96
   CMP   (FP),Y
   BNE   .cmp_true@35
   LDA   #$00
   JMP   .cmp_end@36
.cmp_true@35:
   LDA   #$01
.cmp_end@36:
   LDY   #$98
   STA   (FP),Y
   LDA   #$00
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
   CMP   #$00
   BNE   .cmp_true@38
   DEY
   LDA   (FP),Y
   CMP   #$01
   BNE   .cmp_true@38
   LDA   #$00
   JMP   .cmp_end@39
.cmp_true@38:
   LDA   #$01
.cmp_end@39:
   LDY   #$9C
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
   ORA   #$80
   LDY   #$9E
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   LDY   #$9F
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   LDY   #$9F
   LDA   (FP),Y
   LDY   #$02
   STA   (FP),Y
.if_end@13:
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
   ADC   #$A1
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$A0
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
   DC.W  $FFFF

v2:
   DS.B  2
   DC.B  $04

v3:
   DC.W  $8000

v4:
   DC.B  $0F
   DS.B  2

v5:
   DC.B  $F2
   DC.B  $26
   DC.B  $18
