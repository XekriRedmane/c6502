SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24

check:
   SUBROUTINE

   ; prologue: 13 arg bytes, 204 local bytes
   SEC
   LDA   SSP
   SBC   #$CE
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$CD
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
   LDY   #$DB
   LDA   (FP),Y
   LDY   #$03
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$D0
   LDA   (FP),Y
   LDY   #$05
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   SEC
   LDY   #$03
   SBC   (FP),Y
   LDY   #$06
   LDA   (FP),Y
   LDY   #$04
   SBC   (FP),Y
   BVC   .cmp_novf@0
   EOR   #$80
.cmp_novf@0:
   ASL
   LDA   #$00
   ROL
   EOR   #$01
   LDY   #$07
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDA   #$45
   INY
   STA   (FP),Y
   LDA   #$23
   INY
   STA   (FP),Y
   LDY   #$07
   LDA   (FP),Y
   TAX
   BEQ   .shift_done@2
.shift_loop@1:
   LDY   #$09
   LDA   (FP),Y
   ASL
   STA   (FP),Y
   INY
   LDA   (FP),Y
   ROL
   STA   (FP),Y
   DEX
   BNE   .shift_loop@1
.shift_done@2:
   LDY   #$D7
   LDA   (FP),Y
   CLC
   SBC   (FP),Y
   INY
   LDA   (FP),Y
   SBC   (FP),Y
   INY
   LDA   (FP),Y
   SBC   (FP),Y
   INY
   LDA   (FP),Y
   SBC   (FP),Y
   LDA   #$00
   ROL
   LDY   #$0B
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$DB
   LDA   (FP),Y
   LDY   #$0D
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   EOR   #$FF
   LDY   #$0F
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   EOR   #$FF
   LDY   #$10
   STA   (FP),Y
   LDY   #$0B
   LDA   (FP),Y
   SEC
   LDY   #$0F
   SBC   (FP),Y
   LDY   #$0C
   LDA   (FP),Y
   LDY   #$10
   SBC   (FP),Y
   BVC   .cmp_novf@3
   EOR   #$80
.cmp_novf@3:
   ASL
   LDA   #$00
   ROL
   LDY   #$11
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$13
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$14
   STA   (FP),Y
   LDY   #$09
   LDA   (FP),Y
   CLC
   LDY   #$13
   ADC   (FP),Y
   LDY   #$15
   STA   (FP),Y
   LDY   #$0A
   LDA   (FP),Y
   LDY   #$14
   ADC   (FP),Y
   LDY   #$16
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$17
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$18
   STA   (FP),Y
   CMP   #$46
   BNE   .cmp_true@4
   DEY
   LDA   (FP),Y
   CMP   #$8A
   BNE   .cmp_true@4
   LDA   #$00
   JMP   .cmp_end@5
.cmp_true@4:
   LDA   #$01
.cmp_end@5:
   LDY   #$19
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@1
   JMP   .or_true@1
.lb_skip@1:
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
   LDA   #$FF
   BEQ   .lb_skip@0
   JMP   .or_true@1
.lb_skip@0:
   LDY   #$CF
   LDA   (FP),Y
   LDY   #$1D
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$D5
   LDA   (FP),Y
   CLC
   LDY   #$1D
   ADC   (FP),Y
   LDY   #$1F
   STA   (FP),Y
   LDY   #$D6
   LDA   (FP),Y
   LDY   #$1E
   ADC   (FP),Y
   LDY   #$20
   STA   (FP),Y
   LDY   #$D0
   LDA   (FP),Y
   LDY   #$21
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   SEC
   LDY   #$D5
   SBC   (FP),Y
   LDY   #$22
   LDA   (FP),Y
   LDY   #$D6
   SBC   (FP),Y
   BVC   .cmp_novf@7
   EOR   #$80
.cmp_novf@7:
   ASL
   LDA   #$00
   ROL
   LDY   #$23
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$1F
   LDA   (FP),Y
   LDY   #$23
   EOR   (FP),Y
   LDY   #$25
   STA   (FP),Y
   LDY   #$20
   LDA   (FP),Y
   LDY   #$24
   EOR   (FP),Y
   LDY   #$26
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@1
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   JMP   .or_end@2
.or_true@1:
   LDA   #$01
   LDY   #$27
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@2:
   LDY   #$27
   LDA   (FP),Y
   LDY   #$29
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$2A
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CMP   #$01
   BNE   .cmp_true@8
   LDA   #$00
   JMP   .cmp_end@9
.cmp_true@8:
   LDA   #$01
.cmp_end@9:
   LDY   #$2B
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
.if_end@3:
   LDY   #$D7
   LDA   (FP),Y
   LDY   #$2F
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$32
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$33
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@10
   LDA   #$00
   JMP   .sx_done@11
.sx_neg@10:
   LDA   #$FF
.sx_done@11:
   LDY   #$34
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$DB
   LDA   (FP),Y
   LDY   #$36
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$38
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$39
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@12
   LDA   #$00
   JMP   .sx_done@13
.sx_neg@12:
   LDA   #$FF
.sx_done@13:
   LDY   #$3A
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$D7
   LDA   (FP),Y
   LDY   #$38
   ORA   (FP),Y
   LDY   #$3C
   STA   (FP),Y
   LDY   #$D8
   LDA   (FP),Y
   LDY   #$39
   ORA   (FP),Y
   LDY   #$3D
   STA   (FP),Y
   LDY   #$D9
   LDA   (FP),Y
   LDY   #$3A
   ORA   (FP),Y
   LDY   #$3E
   STA   (FP),Y
   LDY   #$DA
   LDA   (FP),Y
   LDY   #$3B
   ORA   (FP),Y
   LDY   #$3F
   STA   (FP),Y
   LDY   #$32
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
   LDY   #$3C
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
   LDY   #$40
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
   LDY   #$DB
   LDA   (FP),Y
   LDY   #$44
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$D0
   LDA   (FP),Y
   LDY   #$46
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$44
   LDA   (FP),Y
   LDY   #$46
   AND   (FP),Y
   LDY   #$48
   STA   (FP),Y
   LDY   #$45
   LDA   (FP),Y
   LDY   #$47
   AND   (FP),Y
   LDY   #$49
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   LDY   #$D5
   ADC   (FP),Y
   LDY   #$4A
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$D6
   ADC   (FP),Y
   LDY   #$4B
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$4C
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$4D
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@14
   LDA   #$00
   JMP   .sx_done@15
.sx_neg@14:
   LDA   #$FF
.sx_done@15:
   LDY   #$4E
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$40
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
   LDY   #$50
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
   LDY   #$50
   LDA   (FP),Y
   LDY   #$54
   STA   (FP),Y
   LDY   #$51
   LDA   (FP),Y
   LDY   #$55
   STA   (FP),Y
   LDY   #$52
   LDA   (FP),Y
   LDY   #$56
   STA   (FP),Y
   LDY   #$53
   LDA   (FP),Y
   LDY   #$57
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
   LDA   #$00
   JMP   .cmp_end@17
.cmp_true@16:
   LDA   #$01
.cmp_end@17:
   LDY   #$58
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
   LDY   #$5A
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   LDY   #$5B
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   LDY   #$5B
   LDA   (FP),Y
   LDY   #$02
   STA   (FP),Y
.if_end@4:
   LDY   #$D5
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@7
   LDA   #$7F
   BEQ   .and_false@7
   LDA   #$01
   LDY   #$5C
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   JMP   .and_end@8
.and_false@7:
   LDA   #$00
   LDY   #$5C
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@8:
   LDY   #$5C
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@5
   LDY   #$D1
   LDA   (FP),Y
   EOR   #$02
   LDY   #$5E
   STA   (FP),Y
   LDY   #$D2
   LDA   (FP),Y
   LDY   #$5F
   STA   (FP),Y
   LDY   #$D3
   LDA   (FP),Y
   LDY   #$60
   STA   (FP),Y
   LDY   #$D4
   LDA   (FP),Y
   LDY   #$61
   STA   (FP),Y
   LDY   #$5E
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   INY
   ORA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@5
   LDA   #$01
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   JMP   .and_end@6
.and_false@5:
   LDA   #$00
   LDY   #$62
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@6:
   LDY   #$62
   LDA   (FP),Y
   LDY   #$64
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@19
   LDA   #$00
   JMP   .sx_done@20
.sx_neg@19:
   LDA   #$FF
.sx_done@20:
   LDY   #$67
   STA   (FP),Y
   LDA   #$01
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@21
   LDA   #$00
   JMP   .sx_done@22
.sx_neg@21:
   LDA   #$FF
.sx_done@22:
   LDY   #$6A
   STA   (FP),Y
   LDY   #$67
   LDA   (FP),Y
   LDY   #$6A
   CMP   (FP),Y
   BNE   .cmp_true@23
   LDY   #$66
   LDA   (FP),Y
   LDY   #$69
   CMP   (FP),Y
   BNE   .cmp_true@23
   LDA   #$00
   JMP   .cmp_end@24
.cmp_true@23:
   LDA   #$01
.cmp_end@24:
   LDY   #$6B
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
   ORA   #$08
   LDY   #$6D
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   LDY   #$6E
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   LDY   #$6E
   LDA   (FP),Y
   LDY   #$02
   STA   (FP),Y
.if_end@9:
   LDY   #$D5
   LDA   (FP),Y
   LDY   #$6F
   STA   (FP),Y
   LDY   #$D6
   LDA   (FP),Y
   LDY   #$70
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   HARGS
   INY
   LDA   (FP),Y
   STA   HARGS+1
   LDA   #$FF
   STA   HARGS+2
   LDA   #$7F
   STA   HARGS+3
   JSR   mul16
   LDA   HARGS+4
   LDY   #$71
   STA   (FP),Y
   LDA   HARGS+5
   INY
   STA   (FP),Y
   LDY   #$D5
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@10
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@10
   LDA   #$00
   LDY   #$73
   STA   (FP),Y
   INY
   STA   (FP),Y
   JMP   .or_end@11
.or_true@10:
   LDA   #$01
   LDY   #$73
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@11:
   LDY   #$71
   LDA   (FP),Y
   LDY   #$75
   STA   (FP),Y
   LDY   #$72
   LDA   (FP),Y
   LDY   #$76
   STA   (FP),Y
   LDY   #$73
   LDA   (FP),Y
   TAX
   BEQ   .shift_done@27
.shift_loop@26:
   LDY   #$76
   LDA   (FP),Y
   LSR
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ROR
   STA   (FP),Y
   DEX
   BNE   .shift_loop@26
.shift_done@27:
   LDY   #$D0
   LDA   (FP),Y
   LDY   #$77
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   SEC
   DEY
   SBC   (FP),Y
   LDY   #$79
   STA   (FP),Y
   LDA   #$00
   DEY
   SBC   (FP),Y
   LDY   #$7A
   STA   (FP),Y
   LDY   #$75
   LDA   (FP),Y
   LDY   #$7B
   STA   (FP),Y
   LDY   #$76
   LDA   (FP),Y
   LDY   #$7C
   STA   (FP),Y
   LDY   #$79
   LDA   (FP),Y
   TAX
   BEQ   .shift_done@29
.shift_loop@28:
   LDY   #$7C
   LDA   (FP),Y
   LSR
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ROR
   STA   (FP),Y
   DEX
   BNE   .shift_loop@28
.shift_done@29:
   LDY   #$7B
   LDA   (FP),Y
   LDY   #$7D
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$7E
   STA   (FP),Y
   CMP   #$7C
   BNE   .cmp_true@30
   DEY
   LDA   (FP),Y
   CMP   #$32
   BNE   .cmp_true@30
   LDA   #$00
   JMP   .cmp_end@31
.cmp_true@30:
   LDA   #$01
.cmp_end@31:
   LDY   #$7F
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
   ORA   #$10
   LDY   #$81
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   LDY   #$82
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   LDY   #$82
   LDA   (FP),Y
   LDY   #$02
   STA   (FP),Y
.if_end@12:
   LDY   #$CF
   LDA   (FP),Y
   LDY   #$83
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$85
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$86
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@33
   LDA   #$00
   JMP   .sx_done@34
.sx_neg@33:
   LDA   #$FF
.sx_done@34:
   LDY   #$87
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$D1
   LDA   (FP),Y
   LDY   #$85
   ORA   (FP),Y
   LDY   #$89
   STA   (FP),Y
   LDY   #$D2
   LDA   (FP),Y
   LDY   #$86
   ORA   (FP),Y
   LDY   #$8A
   STA   (FP),Y
   LDY   #$D3
   LDA   (FP),Y
   LDY   #$87
   ORA   (FP),Y
   LDY   #$8B
   STA   (FP),Y
   LDY   #$D4
   LDA   (FP),Y
   LDY   #$88
   ORA   (FP),Y
   LDY   #$8C
   STA   (FP),Y
   LDY   #$DB
   LDA   (FP),Y
   LDY   #$8D
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$CF
   LDA   (FP),Y
   LDY   #$8F
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$91
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$92
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@35
   LDA   #$00
   JMP   .sx_done@36
.sx_neg@35:
   LDA   #$FF
.sx_done@36:
   LDY   #$93
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$D1
   LDA   (FP),Y
   SEC
   LDY   #$91
   SBC   (FP),Y
   LDY   #$D2
   LDA   (FP),Y
   LDY   #$92
   SBC   (FP),Y
   LDY   #$D3
   LDA   (FP),Y
   LDY   #$93
   SBC   (FP),Y
   LDY   #$D4
   LDA   (FP),Y
   LDY   #$94
   SBC   (FP),Y
   LDA   #$00
   ROL
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$8D
   LDA   (FP),Y
   LDY   #$95
   EOR   (FP),Y
   LDY   #$97
   STA   (FP),Y
   LDY   #$8E
   LDA   (FP),Y
   LDY   #$96
   EOR   (FP),Y
   LDY   #$98
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
   BMI   .sx_neg@37
   LDA   #$00
   JMP   .sx_done@38
.sx_neg@37:
   LDA   #$FF
.sx_done@38:
   LDY   #$9B
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$99
   LDA   (FP),Y
   SEC
   LDY   #$89
   SBC   (FP),Y
   LDY   #$9A
   LDA   (FP),Y
   LDY   #$8A
   SBC   (FP),Y
   LDY   #$9B
   LDA   (FP),Y
   LDY   #$8B
   SBC   (FP),Y
   LDY   #$9C
   LDA   (FP),Y
   LDY   #$8C
   SBC   (FP),Y
   LDA   #$00
   ROL
   LDY   #$9D
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$9F
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$A0
   STA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@39
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@39
   LDA   #$00
   JMP   .cmp_end@40
.cmp_true@39:
   LDA   #$01
.cmp_end@40:
   LDY   #$A1
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
.if_end@13:
   LDY   #$D0
   LDA   (FP),Y
   LDY   #$A5
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
   LDY   #$A6
   LDA   (FP),Y
   LDY   #$A9
   CMP   (FP),Y
   BNE   .cmp_true@42
   LDA   #$00
   JMP   .cmp_end@43
.cmp_true@42:
   LDA   #$01
.cmp_end@43:
   LDY   #$AB
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@14
   LDY   #$01
   LDA   (FP),Y
   ORA   #$40
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
.if_end@14:
   LDY   #$CF
   LDA   (FP),Y
   LDY   #$AF
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$DB
   LDA   (FP),Y
   LDY   #$B1
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$AF
   LDA   (FP),Y
   LDY   #$B1
   CMP   (FP),Y
   BNE   .cmp_true@44
   LDA   #$00
   JMP   .cmp_end@45
.cmp_true@44:
   LDA   #$01
.cmp_end@45:
   LDY   #$B3
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDA   #$07
   CLC
   LDY   #$D7
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
   LDY   #$B5
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$B3
   LDA   (FP),Y
   LDY   #$B5
   AND   (FP),Y
   LDY   #$B7
   STA   (FP),Y
   LDY   #$B4
   LDA   (FP),Y
   LDY   #$B6
   AND   (FP),Y
   LDY   #$B8
   STA   (FP),Y
   LDA   #$01
   BNE   .or_true@15
   LDA   #$C3
   BNE   .or_true@15
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   JMP   .or_end@16
.or_true@15:
   LDA   #$01
   LDY   #$B9
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@16:
   LDY   #$B9
   LDA   (FP),Y
   LDY   #$BB
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$BC
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@46
   LDA   #$00
   JMP   .sx_done@47
.sx_neg@46:
   LDA   #$FF
.sx_done@47:
   LDY   #$BD
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$D0
   LDA   (FP),Y
   LDY   #$BF
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$BB
   LDA   (FP),Y
   SEC
   LDY   #$BF
   SBC   (FP),Y
   LDY   #$BC
   LDA   (FP),Y
   LDY   #$C0
   SBC   (FP),Y
   LDY   #$BD
   LDA   (FP),Y
   LDY   #$C1
   SBC   (FP),Y
   LDY   #$BE
   LDA   (FP),Y
   LDY   #$C2
   SBC   (FP),Y
   BVC   .cmp_novf@48
   EOR   #$80
.cmp_novf@48:
   ASL
   LDA   #$00
   ROL
   LDY   #$C3
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$B7
   LDA   (FP),Y
   CLC
   LDY   #$C3
   ADC   (FP),Y
   LDY   #$C5
   STA   (FP),Y
   LDY   #$B8
   LDA   (FP),Y
   LDY   #$C4
   ADC   (FP),Y
   LDY   #$C6
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$C7
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$C8
   STA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@49
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@49
   LDA   #$00
   JMP   .cmp_end@50
.cmp_true@49:
   LDA   #$01
.cmp_end@50:
   LDY   #$C9
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@17
   LDY   #$01
   LDA   (FP),Y
   ORA   #$80
   LDY   #$CB
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   LDY   #$CC
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   LDY   #$CC
   LDA   (FP),Y
   LDY   #$02
   STA   (FP),Y
.if_end@17:
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
   ADC   #$DB
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$CD
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

   ; prologue: 0 arg bytes, 5 local bytes
   SEC
   LDA   SSP
   SBC   #$07
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$06
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   LDA   #$01
   LDY   #$01
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$0D
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDY   #$01
   LDA   (FP),Y
   STA   (SSP),Y
   INY
   LDA   (FP),Y
   STA   (SSP),Y
   LDA   #$3F
   INY
   STA   (SSP),Y
   LDA   #$7F
   INY
   STA   (SSP),Y
   LDA   #$95
   INY
   STA   (SSP),Y
   LDA   #$7E
   INY
   STA   (SSP),Y
   LDA   #$9C
   INY
   STA   (SSP),Y
   LDA   #$07
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
   LDY   #$03
   LDA   (FP),Y
   LDY   #$0D
   STA   (SSP),Y
   JSR   check
   LDY   #$04
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
   ADC   #$07
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$06
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
