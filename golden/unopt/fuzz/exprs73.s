SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24

check:
   SUBROUTINE

   ; prologue: 0 arg bytes, 238 local bytes
   SEC
   LDA   SSP
   SBC   #$F0
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$EF
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
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDA   v2
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$07
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$08
   STA   (FP),Y
   LDA   #$FF
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
   LDY   #$0B
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$0C
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$0D
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@2
   LDA   #$00
   JMP   .sx_done@3
.sx_neg@2:
   LDA   #$FF
.sx_done@3:
   LDY   #$0E
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$0C
   LDA   (FP),Y
   SEC
   INY
   LDA   (FP),Y
   SBC   #$80
   INY
   LDA   (FP),Y
   SBC   #$00
   INY
   LDA   (FP),Y
   SBC   #$00
   INY
   LDA   (FP),Y
   SBC   #$00
   BVC   .cmp_novf@4
   EOR   #$80
.cmp_novf@4:
   ASL
   LDA   #$00
   ROL
   EOR   #$01
   LDY   #$11
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$07
   LDA   (FP),Y
   STA   HARGS
   INY
   LDA   (FP),Y
   STA   HARGS+1
   LDY   #$11
   LDA   (FP),Y
   STA   HARGS+2
   INY
   LDA   (FP),Y
   STA   HARGS+3
   JSR   mul16
   LDA   HARGS+4
   LDY   #$13
   STA   (FP),Y
   LDA   HARGS+5
   INY
   STA   (FP),Y
   LDY   #$03
   LDA   (FP),Y
   STA   HARGS
   INY
   LDA   (FP),Y
   STA   HARGS+1
   LDY   #$13
   LDA   (FP),Y
   STA   HARGS+2
   INY
   LDA   (FP),Y
   STA   HARGS+3
   JSR   mul16
   LDA   HARGS+4
   LDY   #$15
   STA   (FP),Y
   LDA   HARGS+5
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$17
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$18
   STA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@5
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@5
   LDA   #$00
   JMP   .cmp_end@6
.cmp_true@5:
   LDA   #$01
.cmp_end@6:
   LDY   #$19
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
   LDY   #$1B
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   LDY   #$1C
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   LDY   #$1C
   LDA   (FP),Y
   LDY   #$02
   STA   (FP),Y
.if_end@0:
   LDA   v3
   ORA   v3+1
   ORA   v3+2
   ORA   v3+3
   ORA   v3+4
   BEQ   .lnot_true@8
   LDA   #$00
   JMP   .lnot_end@9
.lnot_true@8:
   LDA   #$01
.lnot_end@9:
   LDY   #$1D
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$1F
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$20
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@10
   LDA   #$00
   JMP   .sx_done@11
.sx_neg@10:
   LDA   #$FF
.sx_done@11:
   LDY   #$21
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$1F
   LDA   (FP),Y
   LDY   #$24
   STA   (FP),Y
   LDY   #$20
   LDA   (FP),Y
   LDY   #$25
   STA   (FP),Y
   LDY   #$21
   LDA   (FP),Y
   LDY   #$26
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@12
   LDY   #$25
   LDA   (FP),Y
   BNE   .cmp_true@12
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@12
   LDA   #$00
   JMP   .cmp_end@13
.cmp_true@12:
   LDA   #$01
.cmp_end@13:
   LDY   #$29
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
.if_end@1:
   LDA   #$FF
   LDY   #$2D
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CMP   #$FF
   BNE   .cmp_true@15
   LDA   #$00
   JMP   .cmp_end@16
.cmp_true@15:
   LDA   #$01
.cmp_end@16:
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
   ORA   #$04
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
   LDA   v0
   LDY   #$33
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
   CMP   #$FF
   BNE   .cmp_true@17
   DEY
   LDA   (FP),Y
   CMP   #$FF
   BNE   .cmp_true@17
   DEY
   LDA   (FP),Y
   CMP   #$FF
   BNE   .cmp_true@17
   DEY
   LDA   (FP),Y
   CMP   #$FF
   BNE   .cmp_true@17
   LDA   #$00
   JMP   .cmp_end@18
.cmp_true@17:
   LDA   #$01
.cmp_end@18:
   LDY   #$37
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
   LDY   #$39
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   LDY   #$3A
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   LDY   #$3A
   LDA   (FP),Y
   LDY   #$02
   STA   (FP),Y
.if_end@3:
   LDA   v0
   LDY   #$3B
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
   LDA   v0+3
   BMI   .sx_neg@20
   LDA   #$00
   JMP   .sx_done@21
.sx_neg@20:
   LDA   #$FF
.sx_done@21:
   LDY   #$3F
   STA   (FP),Y
   LDY   #$3B
   LDA   (FP),Y
   EOR   #$FF
   LDY   #$40
   STA   (FP),Y
   LDY   #$3C
   LDA   (FP),Y
   EOR   #$FF
   LDY   #$41
   STA   (FP),Y
   LDY   #$3D
   LDA   (FP),Y
   EOR   #$FF
   LDY   #$42
   STA   (FP),Y
   LDY   #$3E
   LDA   (FP),Y
   EOR   #$FF
   LDY   #$43
   STA   (FP),Y
   LDY   #$3F
   LDA   (FP),Y
   EOR   #$FF
   LDY   #$44
   STA   (FP),Y
   LDA   v0
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$48
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$49
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@22
   LDA   #$00
   JMP   .sx_done@23
.sx_neg@22:
   LDA   #$FF
.sx_done@23:
   LDY   #$4A
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
   LDA   v4
   LDY   #$4C
   EOR   (FP),Y
   LDY   #$50
   STA   (FP),Y
   LDA   v4+1
   LDY   #$4D
   EOR   (FP),Y
   LDY   #$51
   STA   (FP),Y
   LDA   v4+2
   LDY   #$4E
   EOR   (FP),Y
   LDY   #$52
   STA   (FP),Y
   LDA   v4+3
   LDY   #$4F
   EOR   (FP),Y
   LDY   #$53
   STA   (FP),Y
   LDY   #$48
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
   LDY   #$50
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
   LDY   #$54
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
   LDY   #$54
   LDA   (FP),Y
   LDY   #$58
   STA   (FP),Y
   LDY   #$55
   LDA   (FP),Y
   LDY   #$59
   STA   (FP),Y
   LDY   #$56
   LDA   (FP),Y
   LDY   #$5A
   STA   (FP),Y
   LDY   #$57
   LDA   (FP),Y
   LDY   #$5B
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@24
   LDA   #$00
   JMP   .sx_done@25
.sx_neg@24:
   LDA   #$FF
.sx_done@25:
   LDY   #$5C
   STA   (FP),Y
   LDY   #$40
   LDA   (FP),Y
   SEC
   LDY   #$58
   SBC   (FP),Y
   LDY   #$5D
   STA   (FP),Y
   LDY   #$41
   LDA   (FP),Y
   LDY   #$59
   SBC   (FP),Y
   LDY   #$5E
   STA   (FP),Y
   LDY   #$42
   LDA   (FP),Y
   LDY   #$5A
   SBC   (FP),Y
   LDY   #$5F
   STA   (FP),Y
   LDY   #$43
   LDA   (FP),Y
   LDY   #$5B
   SBC   (FP),Y
   LDY   #$60
   STA   (FP),Y
   LDY   #$44
   LDA   (FP),Y
   LDY   #$5C
   SBC   (FP),Y
   LDY   #$61
   STA   (FP),Y
   LDY   #$5D
   LDA   (FP),Y
   LDY   #$62
   STA   (FP),Y
   LDY   #$5E
   LDA   (FP),Y
   LDY   #$63
   STA   (FP),Y
   LDY   #$5F
   LDA   (FP),Y
   LDY   #$64
   STA   (FP),Y
   LDY   #$60
   LDA   (FP),Y
   LDY   #$65
   STA   (FP),Y
   LDY   #$61
   LDA   (FP),Y
   AND   #$03
   LDY   #$66
   STA   (FP),Y
   LDY   #$62
   LDA   (FP),Y
   LDY   #$67
   STA   (FP),Y
   LDY   #$63
   LDA   (FP),Y
   LDY   #$68
   STA   (FP),Y
   LDY   #$64
   LDA   (FP),Y
   LDY   #$69
   STA   (FP),Y
   LDY   #$65
   LDA   (FP),Y
   LDY   #$6A
   STA   (FP),Y
   LDY   #$66
   LDA   (FP),Y
   EOR   #$02
   LDY   #$6B
   STA   (FP),Y
   LDY   #$67
   LDA   (FP),Y
   LDY   #$6C
   STA   (FP),Y
   LDY   #$68
   LDA   (FP),Y
   LDY   #$6D
   STA   (FP),Y
   LDY   #$69
   LDA   (FP),Y
   LDY   #$6E
   STA   (FP),Y
   LDY   #$6A
   LDA   (FP),Y
   LDY   #$6F
   STA   (FP),Y
   LDY   #$6B
   LDA   (FP),Y
   SEC
   SBC   #$02
   LDY   #$70
   STA   (FP),Y
   LDY   #$6C
   LDA   (FP),Y
   LDY   #$71
   STA   (FP),Y
   LDY   #$6D
   LDA   (FP),Y
   LDY   #$72
   STA   (FP),Y
   LDY   #$6E
   LDA   (FP),Y
   LDY   #$73
   STA   (FP),Y
   LDY   #$6F
   LDA   (FP),Y
   LDY   #$74
   STA   (FP),Y
   LDY   #$70
   LDA   (FP),Y
   LDY   #$75
   STA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@26
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@26
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@26
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@26
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@26
   LDA   #$00
   JMP   .cmp_end@27
.cmp_true@26:
   LDA   #$01
.cmp_end@27:
   LDY   #$76
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
.if_end@4:
   LDA   v5
   LDY   #$7A
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
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
   LDY   #$7A
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
   LDA   HARGS+12
   LDY   #$7E
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
   LDA   #$07
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   ORA   #$00
   BPL   .sx_done@30
   LDA   #$FF
.sx_done@30:
   LDY   #$84
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$82
   LDA   (FP),Y
   SEC
   LDY   #$7E
   SBC   (FP),Y
   LDY   #$83
   LDA   (FP),Y
   LDY   #$7F
   SBC   (FP),Y
   LDY   #$84
   LDA   (FP),Y
   LDY   #$80
   SBC   (FP),Y
   LDY   #$85
   LDA   (FP),Y
   LDY   #$81
   SBC   (FP),Y
   BVC   .cmp_novf@31
   EOR   #$80
.cmp_novf@31:
   ASL
   LDA   #$00
   ROL
   EOR   #$01
   LDY   #$86
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   SEC
   DEY
   SBC   (FP),Y
   LDY   #$88
   STA   (FP),Y
   LDA   #$00
   DEY
   SBC   (FP),Y
   LDY   #$89
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$8A
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$8B
   STA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@32
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@32
   LDA   #$00
   JMP   .cmp_end@33
.cmp_true@32:
   LDA   #$01
.cmp_end@33:
   LDY   #$8C
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
   LDY   #$8E
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   LDY   #$8F
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   LDY   #$8F
   LDA   (FP),Y
   LDY   #$02
   STA   (FP),Y
.if_end@5:
   LDA   v2
   BEQ   .lb_skip@0
   JMP   .or_true@6
.lb_skip@0:
   LDA   v1
   LDY   #$90
   STA   (FP),Y
   LDA   v1+1
   INY
   STA   (FP),Y
   LDA   v1+1
   BMI   .sx_neg@35
   LDA   #$00
   JMP   .sx_done@36
.sx_neg@35:
   LDA   #$FF
.sx_done@36:
   LDY   #$92
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   v3+4
   CMP   (FP),Y
   BNE   .cmp_true@37
   LDA   v3+3
   DEY
   CMP   (FP),Y
   BNE   .cmp_true@37
   LDA   v3+2
   DEY
   CMP   (FP),Y
   BNE   .cmp_true@37
   LDA   v3+1
   DEY
   CMP   (FP),Y
   BNE   .cmp_true@37
   LDA   v3
   DEY
   CMP   (FP),Y
   BNE   .cmp_true@37
   LDA   #$00
   JMP   .cmp_end@38
.cmp_true@37:
   LDA   #$01
.cmp_end@38:
   LDY   #$95
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDA   #$FF
   BEQ   .and_false@8
   LDA   v4
   ORA   v4+1
   ORA   v4+2
   ORA   v4+3
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
   LDY   #$97
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@9:
   LDY   #$96
   LDA   (FP),Y
   LDY   #$98
   CMP   (FP),Y
   BNE   .cmp_true@40
   LDY   #$95
   LDA   (FP),Y
   LDY   #$97
   CMP   (FP),Y
   BNE   .cmp_true@40
   LDA   #$00
   JMP   .cmp_end@41
.cmp_true@40:
   LDA   #$01
.cmp_end@41:
   LDY   #$99
   STA   (FP),Y
   LDA   #$00
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
   LDY   #$9B
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@7:
   LDY   #$9B
   LDA   (FP),Y
   LDY   #$9D
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$9E
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
   LDY   #$9F
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
.if_end@10:
   LDA   v0
   LDY   #$A3
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
   LDA   v0+3
   BMI   .sx_neg@45
   LDA   #$00
   JMP   .sx_done@46
.sx_neg@45:
   LDA   #$FF
.sx_done@46:
   LDY   #$A7
   STA   (FP),Y
   LDA   v3
   SEC
   LDY   #$A3
   SBC   (FP),Y
   LDY   #$A8
   STA   (FP),Y
   LDA   v3+1
   LDY   #$A4
   SBC   (FP),Y
   LDY   #$A9
   STA   (FP),Y
   LDA   v3+2
   LDY   #$A5
   SBC   (FP),Y
   LDY   #$AA
   STA   (FP),Y
   LDA   v3+3
   LDY   #$A6
   SBC   (FP),Y
   LDY   #$AB
   STA   (FP),Y
   LDA   v3+4
   LDY   #$A7
   SBC   (FP),Y
   LDY   #$AC
   STA   (FP),Y
   LDY   #$A8
   LDA   (FP),Y
   LDY   #$AD
   STA   (FP),Y
   LDY   #$A9
   LDA   (FP),Y
   LDY   #$AE
   STA   (FP),Y
   LDY   #$AA
   LDA   (FP),Y
   LDY   #$AF
   STA   (FP),Y
   LDY   #$AB
   LDA   (FP),Y
   LDY   #$B0
   STA   (FP),Y
   LDY   #$AC
   LDA   (FP),Y
   AND   #$03
   LDY   #$B1
   STA   (FP),Y
   LDY   #$AD
   LDA   (FP),Y
   LDY   #$B2
   STA   (FP),Y
   LDY   #$AE
   LDA   (FP),Y
   LDY   #$B3
   STA   (FP),Y
   LDY   #$AF
   LDA   (FP),Y
   LDY   #$B4
   STA   (FP),Y
   LDY   #$B0
   LDA   (FP),Y
   LDY   #$B5
   STA   (FP),Y
   LDY   #$B1
   LDA   (FP),Y
   EOR   #$02
   LDY   #$B6
   STA   (FP),Y
   LDY   #$B2
   LDA   (FP),Y
   LDY   #$B7
   STA   (FP),Y
   LDY   #$B3
   LDA   (FP),Y
   LDY   #$B8
   STA   (FP),Y
   LDY   #$B4
   LDA   (FP),Y
   LDY   #$B9
   STA   (FP),Y
   LDY   #$B5
   LDA   (FP),Y
   LDY   #$BA
   STA   (FP),Y
   LDY   #$B6
   LDA   (FP),Y
   SEC
   SBC   #$02
   LDY   #$BB
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
   LDA   v0+3
   BMI   .sx_neg@47
   LDA   #$00
   JMP   .sx_done@48
.sx_neg@47:
   LDA   #$FF
.sx_done@48:
   LDY   #$C0
   STA   (FP),Y
   LDY   #$B7
   LDA   (FP),Y
   SEC
   LDY   #$BC
   SBC   (FP),Y
   LDY   #$C1
   STA   (FP),Y
   LDY   #$B8
   LDA   (FP),Y
   LDY   #$BD
   SBC   (FP),Y
   LDY   #$C2
   STA   (FP),Y
   LDY   #$B9
   LDA   (FP),Y
   LDY   #$BE
   SBC   (FP),Y
   LDY   #$C3
   STA   (FP),Y
   LDY   #$BA
   LDA   (FP),Y
   LDY   #$BF
   SBC   (FP),Y
   LDY   #$C4
   STA   (FP),Y
   LDY   #$BB
   LDA   (FP),Y
   LDY   #$C0
   SBC   (FP),Y
   LDY   #$C5
   STA   (FP),Y
   LDY   #$C1
   LDA   (FP),Y
   LDY   #$C6
   STA   (FP),Y
   LDY   #$C2
   LDA   (FP),Y
   LDY   #$C7
   STA   (FP),Y
   LDY   #$C3
   LDA   (FP),Y
   LDY   #$C8
   STA   (FP),Y
   LDY   #$C4
   LDA   (FP),Y
   LDY   #$C9
   STA   (FP),Y
   LDY   #$C5
   LDA   (FP),Y
   AND   #$03
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
   LDY   #$CE
   STA   (FP),Y
   LDY   #$CA
   LDA   (FP),Y
   EOR   #$02
   LDY   #$CF
   STA   (FP),Y
   LDY   #$CB
   LDA   (FP),Y
   LDY   #$D0
   STA   (FP),Y
   LDY   #$CC
   LDA   (FP),Y
   LDY   #$D1
   STA   (FP),Y
   LDY   #$CD
   LDA   (FP),Y
   LDY   #$D2
   STA   (FP),Y
   LDY   #$CE
   LDA   (FP),Y
   LDY   #$D3
   STA   (FP),Y
   LDY   #$CF
   LDA   (FP),Y
   SEC
   SBC   #$02
   LDY   #$D4
   STA   (FP),Y
   LDA   v2
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$D7
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$D8
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@49
   LDA   #$00
   JMP   .sx_done@50
.sx_neg@49:
   LDA   #$FF
.sx_done@50:
   LDY   #$D9
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   v0+3
   CMP   (FP),Y
   BNE   .cmp_true@51
   LDA   v0+2
   DEY
   CMP   (FP),Y
   BNE   .cmp_true@51
   LDA   v0+1
   DEY
   CMP   (FP),Y
   BNE   .cmp_true@51
   LDA   v0
   DEY
   CMP   (FP),Y
   BNE   .cmp_true@51
   LDA   #$00
   JMP   .cmp_end@52
.cmp_true@51:
   LDA   #$01
.cmp_end@52:
   LDY   #$DB
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDA   v4
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$DB
   LDA   (FP),Y
   LDY   #$DE
   ORA   (FP),Y
   LDY   #$E0
   STA   (FP),Y
   LDY   #$DC
   LDA   (FP),Y
   LDY   #$DF
   ORA   (FP),Y
   LDY   #$E1
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$E2
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$E3
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@54
   LDA   #$00
   JMP   .sx_done@55
.sx_neg@54:
   LDA   #$FF
.sx_done@55:
   LDY   #$E4
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$D4
   LDA   (FP),Y
   LDY   #$E6
   CMP   (FP),Y
   BNE   .thread@0
   LDY   #$D3
   LDA   (FP),Y
   LDY   #$E5
   CMP   (FP),Y
   BNE   .thread@0
   LDY   #$D2
   LDA   (FP),Y
   LDY   #$E4
   CMP   (FP),Y
   BNE   .thread@0
   LDY   #$D1
   LDA   (FP),Y
   LDY   #$E3
   CMP   (FP),Y
   BNE   .thread@0
   LDY   #$D0
   LDA   (FP),Y
   LDY   #$E2
   CMP   (FP),Y
   BEQ   .cmp_true@56
.thread@0:
   LDA   #$00
   JMP   .cmp_end@57
.cmp_true@56:
   LDA   #$01
.cmp_end@57:
   LDY   #$E7
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$E9
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$EA
   STA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@59
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@59
   LDA   #$00
   JMP   .cmp_end@60
.cmp_true@59:
   LDA   #$01
.cmp_end@60:
   LDY   #$EB
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
   LDY   #$ED
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   LDY   #$EE
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   LDY   #$EE
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
   ADC   #$F0
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$EF
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
   DC.L  $FFFFFFFF

v1:
   DC.W  $0001

v2:
   DC.B  $01

v3:
   DC.B  $DF
   DC.B  $7C
   DC.B  $C5
   DC.B  $33
   DC.B  $01

v4:
   DC.L  $695E57A1

v5:
   DC.B  $7F
