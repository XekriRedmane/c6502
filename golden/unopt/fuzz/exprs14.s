SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24

check:
   SUBROUTINE

   ; prologue: 17 arg bytes, 196 local bytes
   SEC
   LDA   SSP
   SBC   #$C6
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$C5
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
   LDY   #$D4
   LDA   (FP),Y
   LDY   #$03
   STA   (FP),Y
   LDY   #$D5
   LDA   (FP),Y
   LDY   #$04
   STA   (FP),Y
   LDY   #$D6
   LDA   (FP),Y
   LDY   #$05
   STA   (FP),Y
   LDY   #$D7
   LDA   (FP),Y
   LDY   #$06
   STA   (FP),Y
   LDY   #$D3
   LDA   (FP),Y
   LDY   #$07
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@0
   LDA   #$00
   JMP   .sx_done@1
.sx_neg@0:
   LDA   #$FF
.sx_done@1:
   LDY   #$08
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$03
   LDA   (FP),Y
   CLC
   LDY   #$07
   ADC   (FP),Y
   LDY   #$0B
   STA   (FP),Y
   LDY   #$04
   LDA   (FP),Y
   LDY   #$08
   ADC   (FP),Y
   LDY   #$0C
   STA   (FP),Y
   LDY   #$05
   LDA   (FP),Y
   LDY   #$09
   ADC   (FP),Y
   LDY   #$0D
   STA   (FP),Y
   LDY   #$06
   LDA   (FP),Y
   LDY   #$0A
   ADC   (FP),Y
   LDY   #$0E
   STA   (FP),Y
   LDY   #$CF
   LDA   (FP),Y
   SEC
   LDY   #$0B
   SBC   (FP),Y
   LDY   #$D0
   LDA   (FP),Y
   LDY   #$0C
   SBC   (FP),Y
   LDY   #$D1
   LDA   (FP),Y
   LDY   #$0D
   SBC   (FP),Y
   LDY   #$D2
   LDA   (FP),Y
   LDY   #$0E
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
   LDY   #$11
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$12
   STA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@2
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@2
   LDA   #$00
   JMP   .cmp_end@3
.cmp_true@2:
   LDA   #$01
.cmp_end@3:
   LDY   #$13
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
   LDY   #$15
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   LDY   #$16
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   LDY   #$16
   LDA   (FP),Y
   LDY   #$02
   STA   (FP),Y
.if_end@0:
   LDY   #$C8
   LDA   (FP),Y
   LDY   #$17
   STA   (FP),Y
   LDY   #$C9
   LDA   (FP),Y
   LDY   #$18
   STA   (FP),Y
   LDY   #$CA
   LDA   (FP),Y
   LDY   #$19
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$D4
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
   LDY   #$17
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
   LDY   #$1B
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
   LDY   #$1B
   LDA   (FP),Y
   LDY   #$1F
   STA   (FP),Y
   LDY   #$1C
   LDA   (FP),Y
   LDY   #$20
   STA   (FP),Y
   LDY   #$1D
   LDA   (FP),Y
   LDY   #$21
   STA   (FP),Y
   LDY   #$1E
   LDA   (FP),Y
   LDY   #$22
   STA   (FP),Y
   LDY   #$C7
   LDA   (FP),Y
   LDY   #$23
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   STA   HARGS+1
   LDY   #$01
   STY   HARGS
   DEY
   STY   HARGS+2
   STA   HARGS+3
   LDY   #$23
   LDA   (FP),Y
   STA   HARGS+4
   JSR   asl32
   LDA   HARGS+5
   LDY   #$25
   STA   (FP),Y
   LDA   HARGS+6
   INY
   STA   (FP),Y
   LDA   HARGS+7
   INY
   STA   (FP),Y
   LDY   #$25
   LDA   (FP),Y
   LDY   #$28
   STA   (FP),Y
   LDY   #$26
   LDA   (FP),Y
   LDY   #$29
   STA   (FP),Y
   LDY   #$27
   LDA   (FP),Y
   AND   #$3F
   LDY   #$2A
   STA   (FP),Y
   LDY   #$28
   LDA   (FP),Y
   EOR   #$FF
   LDY   #$2B
   STA   (FP),Y
   LDY   #$29
   LDA   (FP),Y
   EOR   #$FF
   LDY   #$2C
   STA   (FP),Y
   LDY   #$2A
   LDA   (FP),Y
   EOR   #$FF
   LDY   #$2D
   STA   (FP),Y
   LDY   #$2B
   LDA   (FP),Y
   LDY   #$2E
   STA   (FP),Y
   LDY   #$2C
   LDA   (FP),Y
   LDY   #$2F
   STA   (FP),Y
   LDY   #$2D
   LDA   (FP),Y
   AND   #$3F
   LDY   #$30
   STA   (FP),Y
   LDY   #$2E
   LDA   (FP),Y
   LDY   #$31
   STA   (FP),Y
   LDY   #$2F
   LDA   (FP),Y
   LDY   #$32
   STA   (FP),Y
   LDY   #$30
   LDA   (FP),Y
   LDY   #$33
   STA   (FP),Y
   LDA   #$00
   INY
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
   LDY   #$31
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
   LDY   #$35
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
   LDY   #$35
   LDA   (FP),Y
   LDY   #$39
   STA   (FP),Y
   LDY   #$36
   LDA   (FP),Y
   LDY   #$3A
   STA   (FP),Y
   LDY   #$37
   LDA   (FP),Y
   LDY   #$3B
   STA   (FP),Y
   LDY   #$38
   LDA   (FP),Y
   LDY   #$3C
   STA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@5
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@5
   DEY
   LDA   (FP),Y
   CMP   #$04
   BNE   .cmp_true@5
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@5
   LDA   #$00
   JMP   .cmp_end@6
.cmp_true@5:
   LDA   #$01
.cmp_end@6:
   LDY   #$3D
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
   LDY   #$3F
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   LDY   #$40
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   LDY   #$40
   LDA   (FP),Y
   LDY   #$02
   STA   (FP),Y
.if_end@1:
   LDA   #$FF
   LDY   #$41
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDA   #$FF
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$43
   LDA   (FP),Y
   LDY   #$46
   CMP   (FP),Y
   BNE   .cmp_true@8
   LDA   #$00
   JMP   .cmp_end@9
.cmp_true@8:
   LDA   #$01
.cmp_end@9:
   LDY   #$48
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
   LDY   #$4A
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   LDY   #$4B
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   LDY   #$4B
   LDA   (FP),Y
   LDY   #$02
   STA   (FP),Y
.if_end@2:
   LDY   #$C7
   LDA   (FP),Y
   LDY   #$4C
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$CF
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
   LDY   #$4C
   LDA   (FP),Y
   STA   HARGS+4
   JSR   asl32
   LDA   HARGS+5
   LDY   #$4E
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
   LDY   #$4E
   LDA   (FP),Y
   EOR   #$FF
   LDY   #$52
   STA   (FP),Y
   LDY   #$4F
   LDA   (FP),Y
   EOR   #$FF
   LDY   #$53
   STA   (FP),Y
   LDY   #$50
   LDA   (FP),Y
   EOR   #$FF
   LDY   #$54
   STA   (FP),Y
   LDY   #$51
   LDA   (FP),Y
   EOR   #$FF
   LDY   #$55
   STA   (FP),Y
   LDA   #$00
   SEC
   LDY   #$CF
   SBC   (FP),Y
   LDA   #$00
   INY
   SBC   (FP),Y
   LDA   #$00
   INY
   SBC   (FP),Y
   LDA   #$80
   INY
   SBC   (FP),Y
   LDA   #$00
   ROL
   LDY   #$56
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$D3
   LDA   (FP),Y
   LDY   #$58
   STA   (FP),Y
   LDY   #$CF
   LDA   (FP),Y
   TAX
   BEQ   .shift_done@11
.shift_loop@10:
   LDY   #$58
   LDA   (FP),Y
   ASL
   STA   (FP),Y
   DEX
   BNE   .shift_loop@10
.shift_done@11:
   LDY   #$58
   LDA   (FP),Y
   AND   #$1F
   INY
   STA   (FP),Y
   EOR   #$10
   INY
   STA   (FP),Y
   SEC
   SBC   #$10
   INY
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
   LDY   #$5D
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   SEC
   LDY   #$56
   SBC   (FP),Y
   LDY   #$5D
   LDA   (FP),Y
   LDY   #$57
   SBC   (FP),Y
   BVC   .cmp_novf@14
   EOR   #$80
.cmp_novf@14:
   ASL
   LDA   #$00
   ROL
   LDY   #$5E
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$52
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
   LDY   #$5E
   LDA   (FP),Y
   STA   HARGS+4
   JSR   lsr32
   LDA   HARGS+5
   LDY   #$60
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
   LDY   #$60
   LDA   (FP),Y
   LDY   #$64
   STA   (FP),Y
   LDY   #$61
   LDA   (FP),Y
   LDY   #$65
   STA   (FP),Y
   LDY   #$62
   LDA   (FP),Y
   LDY   #$66
   STA   (FP),Y
   LDY   #$63
   LDA   (FP),Y
   LDY   #$67
   STA   (FP),Y
   CMP   #$FF
   BNE   .cmp_true@15
   DEY
   LDA   (FP),Y
   CMP   #$FF
   BNE   .cmp_true@15
   DEY
   LDA   (FP),Y
   CMP   #$FF
   BNE   .cmp_true@15
   DEY
   LDA   (FP),Y
   CMP   #$FF
   BNE   .cmp_true@15
   LDA   #$00
   JMP   .cmp_end@16
.cmp_true@15:
   LDA   #$01
.cmp_end@16:
   LDY   #$68
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
.if_end@3:
   LDA   #$07
   LDY   #$6C
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CMP   #$07
   BNE   .cmp_true@18
   LDA   #$00
   JMP   .cmp_end@19
.cmp_true@18:
   LDA   #$01
.cmp_end@19:
   LDY   #$6E
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
   LDY   #$70
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   LDY   #$71
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   LDY   #$71
   LDA   (FP),Y
   LDY   #$02
   STA   (FP),Y
.if_end@4:
   LDY   #$D4
   LDA   (FP),Y
   LDY   #$72
   STA   (FP),Y
   LDY   #$D5
   LDA   (FP),Y
   LDY   #$73
   STA   (FP),Y
   LDY   #$D6
   LDA   (FP),Y
   LDY   #$74
   STA   (FP),Y
   LDY   #$D7
   LDA   (FP),Y
   LDY   #$75
   STA   (FP),Y
   LDY   #$D3
   LDA   (FP),Y
   LDY   #$76
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@20
   LDA   #$00
   JMP   .sx_done@21
.sx_neg@20:
   LDA   #$FF
.sx_done@21:
   LDY   #$77
   STA   (FP),Y
   LDA   #$FF
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$76
   LDA   (FP),Y
   LDY   #$78
   ORA   (FP),Y
   LDY   #$7A
   STA   (FP),Y
   LDY   #$77
   LDA   (FP),Y
   LDY   #$79
   ORA   (FP),Y
   LDY   #$7B
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$7C
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$7D
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@22
   LDA   #$00
   JMP   .sx_done@23
.sx_neg@22:
   LDA   #$FF
.sx_done@23:
   LDY   #$7E
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$FF
   INY
   STA   (FP),Y
   SEC
   SBC   #$80
   INY
   STA   (FP),Y
   LDA   #$3F
   SBC   #$00
   INY
   STA   (FP),Y
   LDA   #$00
   SBC   #$00
   INY
   STA   (FP),Y
   LDY   #$7C
   LDA   (FP),Y
   LDY   #$80
   EOR   (FP),Y
   LDY   #$84
   STA   (FP),Y
   LDY   #$7D
   LDA   (FP),Y
   LDY   #$81
   EOR   (FP),Y
   LDY   #$85
   STA   (FP),Y
   LDY   #$7E
   LDA   (FP),Y
   LDY   #$82
   EOR   (FP),Y
   LDY   #$86
   STA   (FP),Y
   LDY   #$7F
   LDA   (FP),Y
   LDY   #$83
   EOR   (FP),Y
   LDY   #$87
   STA   (FP),Y
   LDY   #$72
   LDA   (FP),Y
   SEC
   LDY   #$84
   SBC   (FP),Y
   LDY   #$73
   LDA   (FP),Y
   LDY   #$85
   SBC   (FP),Y
   LDY   #$74
   LDA   (FP),Y
   LDY   #$86
   SBC   (FP),Y
   LDY   #$75
   LDA   (FP),Y
   LDY   #$87
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
   LDY   #$8A
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$8B
   STA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@24
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@24
   LDA   #$00
   JMP   .cmp_end@25
.cmp_true@24:
   LDA   #$01
.cmp_end@25:
   LDY   #$8C
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .vthread@0
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
   LDA   #$FE
   BEQ   .lnot_true@27
.vthread@0:
   LDA   #$00
   JMP   .lnot_end@28
.lnot_true@27:
   LDA   #$01
.lnot_end@28:
   LDY   #$90
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$92
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$93
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@29
   LDA   #$00
   JMP   .sx_done@30
.sx_neg@29:
   LDA   #$FF
.sx_done@30:
   LDY   #$94
   STA   (FP),Y
   LDY   #$92
   LDA   (FP),Y
   LDY   #$95
   STA   (FP),Y
   LDY   #$93
   LDA   (FP),Y
   LDY   #$96
   STA   (FP),Y
   LDY   #$94
   LDA   (FP),Y
   AND   #$3F
   LDY   #$97
   STA   (FP),Y
   LDY   #$95
   LDA   (FP),Y
   LDY   #$98
   STA   (FP),Y
   LDY   #$96
   LDA   (FP),Y
   LDY   #$99
   STA   (FP),Y
   LDY   #$97
   LDA   (FP),Y
   LDY   #$9A
   STA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@31
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@31
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@31
   LDA   #$00
   JMP   .cmp_end@32
.cmp_true@31:
   LDA   #$01
.cmp_end@32:
   LDY   #$9B
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
   LDY   #$9D
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   LDY   #$9E
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   LDY   #$9E
   LDA   (FP),Y
   LDY   #$02
   STA   (FP),Y
.if_end@6:
   LDY   #$D4
   LDA   (FP),Y
   LDY   #$9F
   STA   (FP),Y
   LDY   #$D5
   LDA   (FP),Y
   LDY   #$A0
   STA   (FP),Y
   LDY   #$D6
   LDA   (FP),Y
   LDY   #$A1
   STA   (FP),Y
   LDY   #$9F
   LDA   (FP),Y
   LDY   #$A2
   STA   (FP),Y
   LDY   #$A0
   LDA   (FP),Y
   LDY   #$A3
   STA   (FP),Y
   LDY   #$A1
   LDA   (FP),Y
   AND   #$3F
   LDY   #$A4
   STA   (FP),Y
   LDY   #$C7
   LDA   (FP),Y
   LDY   #$A5
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$A7
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$A8
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$A9
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$AA
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@34
   LDA   #$00
   JMP   .sx_done@35
.sx_neg@34:
   LDA   #$FF
.sx_done@35:
   LDY   #$AB
   STA   (FP),Y
   LDY   #$A9
   LDA   (FP),Y
   LDY   #$AC
   STA   (FP),Y
   LDY   #$AA
   LDA   (FP),Y
   LDY   #$AD
   STA   (FP),Y
   LDY   #$AB
   LDA   (FP),Y
   AND   #$3F
   LDY   #$AE
   STA   (FP),Y
   LDY   #$A2
   LDA   (FP),Y
   SEC
   LDY   #$AC
   SBC   (FP),Y
   LDY   #$AF
   STA   (FP),Y
   LDY   #$A3
   LDA   (FP),Y
   LDY   #$AD
   SBC   (FP),Y
   LDY   #$B0
   STA   (FP),Y
   LDY   #$A4
   LDA   (FP),Y
   LDY   #$AE
   SBC   (FP),Y
   LDY   #$B1
   STA   (FP),Y
   LDY   #$AF
   LDA   (FP),Y
   LDY   #$B2
   STA   (FP),Y
   LDY   #$B0
   LDA   (FP),Y
   LDY   #$B3
   STA   (FP),Y
   LDY   #$B1
   LDA   (FP),Y
   AND   #$3F
   LDY   #$B4
   STA   (FP),Y
   LDY   #$D3
   LDA   (FP),Y
   LDY   #$B5
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@36
   LDA   #$00
   JMP   .sx_done@37
.sx_neg@36:
   LDA   #$FF
.sx_done@37:
   LDY   #$B6
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$B5
   LDA   (FP),Y
   LDY   #$B8
   STA   (FP),Y
   LDY   #$B6
   LDA   (FP),Y
   LDY   #$B9
   STA   (FP),Y
   AND   #$3F
   INY
   STA   (FP),Y
   LDY   #$B2
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
   LDY   #$B8
   LDA   (FP),Y
   STA   HARGS+4
   INY
   LDA   (FP),Y
   STA   HARGS+5
   INY
   LDA   (FP),Y
   STA   HARGS+6
   LDA   #$00
   STA   HARGS+7
   JSR   udivmod32
   LDA   HARGS+12
   LDY   #$BB
   STA   (FP),Y
   LDA   HARGS+13
   INY
   STA   (FP),Y
   LDA   HARGS+14
   INY
   STA   (FP),Y
   LDY   #$BB
   LDA   (FP),Y
   LDY   #$BE
   STA   (FP),Y
   LDY   #$BC
   LDA   (FP),Y
   LDY   #$BF
   STA   (FP),Y
   LDY   #$BD
   LDA   (FP),Y
   LDY   #$C0
   STA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@38
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@38
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@38
   LDA   #$00
   JMP   .cmp_end@39
.cmp_true@38:
   LDA   #$01
.cmp_end@39:
   LDY   #$C1
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
   LDY   #$C3
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   LDY   #$C4
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   LDY   #$C4
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
   ADC   #$D7
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$C5
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

   LDA   #$00
   LDY   #$01
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$11
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   (FP),Y
   STA   (SSP),Y
   LDA   #$00
   INY
   STA   (SSP),Y
   INY
   STA   (SSP),Y
   LDA   #$20
   INY
   STA   (SSP),Y
   LDA   #$46
   INY
   STA   (SSP),Y
   LDA   #$06
   INY
   STA   (SSP),Y
   LDA   #$80
   INY
   STA   (SSP),Y
   LDA   #$4A
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
   LDA   #$0C
   INY
   STA   (SSP),Y
   LDA   #$00
   INY
   STA   (SSP),Y
   INY
   STA   (SSP),Y
   INY
   STA   (SSP),Y
   LDA   #$80
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
