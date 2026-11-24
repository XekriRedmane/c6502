SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24

check:
   SUBROUTINE

   ; prologue: 12 arg bytes, 170 local bytes
   SEC
   LDA   SSP
   SBC   #$AC
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$AB
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
   LDY   #$AD
   LDA   (FP),Y
   LDY   #$03
   STA   (FP),Y
   LDY   #$AE
   LDA   (FP),Y
   LDY   #$04
   STA   (FP),Y
   LDY   #$AF
   LDA   (FP),Y
   LDY   #$05
   STA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@0
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@0
   DEY
   LDA   (FP),Y
   CMP   #$01
   BNE   .cmp_true@0
   LDA   #$00
   JMP   .cmp_end@1
.cmp_true@0:
   LDA   #$01
.cmp_end@1:
   LDY   #$06
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
   LDY   #$08
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   LDY   #$09
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   LDY   #$09
   LDA   (FP),Y
   LDY   #$02
   STA   (FP),Y
.if_end@0:
   LDY   #$AD
   LDA   (FP),Y
   ASL
   INY
   LDA   (FP),Y
   ROL
   LDY   #$0A
   STA   (FP),Y
   LDY   #$AF
   LDA   (FP),Y
   ROL
   LDY   #$0B
   STA   (FP),Y
   LDA   #$00
   ROL
   INY
   STA   (FP),Y
   LDY   #$B1
   LDA   (FP),Y
   LDY   #$0D
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@3
   LDA   #$00
   JMP   .sx_done@4
.sx_neg@3:
   LDA   #$FF
.sx_done@4:
   LDY   #$0E
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$0F
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$10
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@5
   LDA   #$00
   JMP   .sx_done@6
.sx_neg@5:
   LDA   #$FF
.sx_done@6:
   LDY   #$11
   STA   (FP),Y
   LDY   #$0F
   LDA   (FP),Y
   LDY   #$12
   STA   (FP),Y
   LDY   #$10
   LDA   (FP),Y
   LDY   #$13
   STA   (FP),Y
   LDY   #$11
   LDA   (FP),Y
   AND   #$07
   LDY   #$14
   STA   (FP),Y
   LDY   #$12
   LDA   (FP),Y
   SEC
   SBC   #$02
   LDY   #$15
   STA   (FP),Y
   LDY   #$13
   LDA   (FP),Y
   SBC   #$00
   LDY   #$16
   STA   (FP),Y
   LDY   #$14
   LDA   (FP),Y
   SBC   #$00
   LDY   #$17
   STA   (FP),Y
   LDY   #$15
   LDA   (FP),Y
   LDY   #$18
   STA   (FP),Y
   LDY   #$16
   LDA   (FP),Y
   LDY   #$19
   STA   (FP),Y
   LDY   #$17
   LDA   (FP),Y
   AND   #$07
   LDY   #$1A
   STA   (FP),Y
   LDY   #$0A
   LDA   (FP),Y
   CLC
   LDY   #$18
   ADC   (FP),Y
   LDY   #$1B
   STA   (FP),Y
   LDY   #$0B
   LDA   (FP),Y
   LDY   #$19
   ADC   (FP),Y
   LDY   #$1C
   STA   (FP),Y
   LDY   #$0C
   LDA   (FP),Y
   LDY   #$1A
   ADC   (FP),Y
   LDY   #$1D
   STA   (FP),Y
   LDY   #$1B
   LDA   (FP),Y
   LDY   #$1E
   STA   (FP),Y
   LDY   #$1C
   LDA   (FP),Y
   LDY   #$1F
   STA   (FP),Y
   LDY   #$1D
   LDA   (FP),Y
   AND   #$07
   LDY   #$20
   STA   (FP),Y
   SEC
   LDA   #$00
   LDY   #$1E
   SBC   (FP),Y
   LDY   #$21
   STA   (FP),Y
   LDA   #$00
   LDY   #$1F
   SBC   (FP),Y
   LDY   #$22
   STA   (FP),Y
   LDA   #$00
   LDY   #$20
   SBC   (FP),Y
   LDY   #$23
   STA   (FP),Y
   LDY   #$21
   LDA   (FP),Y
   LDY   #$24
   STA   (FP),Y
   LDY   #$22
   LDA   (FP),Y
   LDY   #$25
   STA   (FP),Y
   LDY   #$23
   LDA   (FP),Y
   AND   #$07
   LDY   #$26
   STA   (FP),Y
   LDY   #$24
   LDA   (FP),Y
   LDY   #$27
   STA   (FP),Y
   LDY   #$25
   LDA   (FP),Y
   LDY   #$28
   STA   (FP),Y
   LDY   #$26
   LDA   (FP),Y
   LDY   #$29
   STA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@7
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@7
   DEY
   LDA   (FP),Y
   CMP   #$82
   BNE   .cmp_true@7
   LDA   #$00
   JMP   .cmp_end@8
.cmp_true@7:
   LDA   #$01
.cmp_end@8:
   LDY   #$2A
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
   LDY   #$2C
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   LDY   #$2D
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   LDY   #$2D
   LDA   (FP),Y
   LDY   #$02
   STA   (FP),Y
.if_end@1:
   LDY   #$B6
   LDA   (FP),Y
   LDY   #$2E
   STA   (FP),Y
   LDY   #$B7
   LDA   (FP),Y
   LDY   #$2F
   STA   (FP),Y
   LDY   #$B8
   LDA   (FP),Y
   LDY   #$30
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$2E
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
   LDA   #$00
   STA   HARGS+4
   LDY   #$00
   STY   HARGS+5
   STA   HARGS+6
   LDA   #$80
   STA   HARGS+7
   JSR   sdivmod32
   LDA   HARGS+8
   LDY   #$32
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
   LDY   #$32
   LDA   (FP),Y
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
   CMP   #$00
   BNE   .cmp_true@10
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@10
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@10
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@10
   LDA   #$00
   JMP   .cmp_end@11
.cmp_true@10:
   LDA   #$01
.cmp_end@11:
   LDY   #$3A
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
   LDY   #$3C
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   LDY   #$3D
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   LDY   #$3D
   LDA   (FP),Y
   LDY   #$02
   STA   (FP),Y
.if_end@2:
   LDY   #$B1
   LDA   (FP),Y
   LDY   #$3E
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@13
   LDA   #$00
   JMP   .sx_done@14
.sx_neg@13:
   LDA   #$FF
.sx_done@14:
   LDY   #$3F
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$40
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$41
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@15
   LDA   #$00
   JMP   .sx_done@16
.sx_neg@15:
   LDA   #$FF
.sx_done@16:
   LDY   #$42
   STA   (FP),Y
   LDY   #$40
   LDA   (FP),Y
   LDY   #$43
   STA   (FP),Y
   LDY   #$41
   LDA   (FP),Y
   LDY   #$44
   STA   (FP),Y
   LDY   #$42
   LDA   (FP),Y
   AND   #$07
   LDY   #$45
   STA   (FP),Y
   LDY   #$B6
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
   LDY   #$43
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
   LDY   #$46
   STA   (FP),Y
   LDA   HARGS+13
   INY
   STA   (FP),Y
   LDA   HARGS+14
   INY
   STA   (FP),Y
   LDA   #$02
   LDY   #$46
   AND   (FP),Y
   LDY   #$49
   STA   (FP),Y
   LDA   #$00
   LDY   #$47
   AND   (FP),Y
   LDY   #$4A
   STA   (FP),Y
   LDA   #$00
   LDY   #$48
   AND   (FP),Y
   LDY   #$4B
   STA   (FP),Y
   LDY   #$49
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@3
   LDA   #$02
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@17
   LDA   #$00
   JMP   .sx_done@18
.sx_neg@17:
   LDA   #$FF
.sx_done@18:
   LDY   #$4E
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   EOR   #$FF
   LDY   #$4F
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   EOR   #$FF
   LDY   #$50
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@3
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   JMP   .or_end@4
.or_true@3:
   LDA   #$01
   LDY   #$51
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@4:
   LDY   #$51
   LDA   (FP),Y
   LDY   #$53
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$54
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CMP   #$01
   BNE   .cmp_true@19
   LDA   #$00
   JMP   .cmp_end@20
.cmp_true@19:
   LDA   #$01
.cmp_end@20:
   LDY   #$55
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
   LDY   #$57
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   LDY   #$58
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   LDY   #$58
   LDA   (FP),Y
   LDY   #$02
   STA   (FP),Y
.if_end@5:
   LDA   #$45
   LDY   #$59
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
   LDY   #$5B
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$5C
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$5D
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@23
   LDA   #$00
   JMP   .sx_done@24
.sx_neg@23:
   LDA   #$FF
.sx_done@24:
   LDY   #$5E
   STA   (FP),Y
   LDY   #$5C
   LDA   (FP),Y
   LDY   #$5F
   STA   (FP),Y
   LDY   #$5D
   LDA   (FP),Y
   LDY   #$60
   STA   (FP),Y
   LDY   #$5E
   LDA   (FP),Y
   AND   #$07
   LDY   #$61
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
   LDA   #$00
   STA   HARGS+3
   LDY   #$B6
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
   LDY   #$62
   STA   (FP),Y
   LDA   HARGS+13
   INY
   STA   (FP),Y
   LDA   HARGS+14
   INY
   STA   (FP),Y
   LDY   #$62
   LDA   (FP),Y
   LDY   #$65
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$68
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$69
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@25
   LDA   #$00
   JMP   .sx_done@26
.sx_neg@25:
   LDA   #$FF
.sx_done@26:
   LDY   #$6A
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$B6
   LDA   (FP),Y
   LDY   #$6C
   STA   (FP),Y
   LDY   #$B7
   LDA   (FP),Y
   LDY   #$6D
   STA   (FP),Y
   LDY   #$B8
   LDA   (FP),Y
   LDY   #$6E
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDA   #$80
   CLC
   LDY   #$6C
   ADC   (FP),Y
   LDY   #$70
   STA   (FP),Y
   LDA   #$00
   LDY   #$6D
   ADC   (FP),Y
   LDY   #$71
   STA   (FP),Y
   LDA   #$00
   LDY   #$6E
   ADC   (FP),Y
   LDY   #$72
   STA   (FP),Y
   LDA   #$00
   LDY   #$6F
   ADC   (FP),Y
   LDY   #$73
   STA   (FP),Y
   LDY   #$B0
   LDA   (FP),Y
   BEQ   .and_false@6
   LDA   #$01
   LDY   #$74
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   JMP   .and_end@7
.and_false@6:
   LDA   #$00
   LDY   #$74
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@7:
   LDY   #$74
   LDA   (FP),Y
   LDY   #$76
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$77
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@27
   LDA   #$00
   JMP   .sx_done@28
.sx_neg@27:
   LDA   #$FF
.sx_done@28:
   LDY   #$78
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$70
   LDA   (FP),Y
   LDY   #$76
   EOR   (FP),Y
   LDY   #$7A
   STA   (FP),Y
   LDY   #$71
   LDA   (FP),Y
   LDY   #$77
   EOR   (FP),Y
   LDY   #$7B
   STA   (FP),Y
   LDY   #$72
   LDA   (FP),Y
   LDY   #$78
   EOR   (FP),Y
   LDY   #$7C
   STA   (FP),Y
   LDY   #$73
   LDA   (FP),Y
   LDY   #$79
   EOR   (FP),Y
   LDY   #$7D
   STA   (FP),Y
   LDY   #$6B
   LDA   (FP),Y
   LDY   #$7D
   CMP   (FP),Y
   BNE   .cmp_true@29
   DEY
   CMP   (FP),Y
   BNE   .cmp_true@29
   LDY   #$69
   LDA   (FP),Y
   LDY   #$7B
   CMP   (FP),Y
   BNE   .cmp_true@29
   LDY   #$68
   LDA   (FP),Y
   LDY   #$7A
   CMP   (FP),Y
   BNE   .cmp_true@29
   LDA   #$00
   JMP   .cmp_end@30
.cmp_true@29:
   LDA   #$01
.cmp_end@30:
   LDY   #$7E
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$80
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$81
   STA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@32
   DEY
   LDA   (FP),Y
   CMP   #$01
   BNE   .cmp_true@32
   LDA   #$00
   JMP   .cmp_end@33
.cmp_true@32:
   LDA   #$01
.cmp_end@33:
   LDY   #$82
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
   LDY   #$84
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   LDY   #$85
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   LDY   #$85
   LDA   (FP),Y
   LDY   #$02
   STA   (FP),Y
.if_end@8:
   LDA   #$01
   LDY   #$86
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$B1
   LDA   (FP),Y
   LDY   #$89
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@35
   LDA   #$00
   JMP   .sx_done@36
.sx_neg@35:
   LDA   #$FF
.sx_done@36:
   LDY   #$8A
   STA   (FP),Y
   LDY   #$88
   LDA   (FP),Y
   LDY   #$8A
   CMP   (FP),Y
   BNE   .cmp_true@37
   LDY   #$87
   LDA   (FP),Y
   LDY   #$89
   CMP   (FP),Y
   BNE   .cmp_true@37
   LDA   #$00
   JMP   .cmp_end@38
.cmp_true@37:
   LDA   #$01
.cmp_end@38:
   LDY   #$8B
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@9
   LDY   #$B6
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@9
   LDA   #$01
   LDY   #$8D
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   JMP   .and_end@10
.and_false@9:
   LDA   #$00
   LDY   #$8D
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@10:
   LDY   #$8D
   LDA   (FP),Y
   LDY   #$8F
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$90
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@40
   LDA   #$00
   JMP   .sx_done@41
.sx_neg@40:
   LDA   #$FF
.sx_done@41:
   LDY   #$91
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$FF
   SEC
   LDY   #$8F
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
   BVC   .cmp_novf@42
   EOR   #$80
.cmp_novf@42:
   ASL
   LDA   #$00
   ROL
   EOR   #$01
   LDY   #$94
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$96
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$97
   STA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@43
   DEY
   LDA   (FP),Y
   CMP   #$01
   BNE   .cmp_true@43
   LDA   #$00
   JMP   .cmp_end@44
.cmp_true@43:
   LDA   #$01
.cmp_end@44:
   LDY   #$98
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
   LDY   #$9A
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   LDY   #$9B
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   LDY   #$9B
   LDA   (FP),Y
   LDY   #$02
   STA   (FP),Y
.if_end@11:
   LDY   #$B6
   LDA   (FP),Y
   LDY   #$9C
   STA   (FP),Y
   LDY   #$B7
   LDA   (FP),Y
   LDY   #$9D
   STA   (FP),Y
   LDY   #$B8
   LDA   (FP),Y
   LDY   #$9E
   STA   (FP),Y
   CMP   #$01
   BNE   .cmp_true@46
   DEY
   LDA   (FP),Y
   CMP   #$96
   BNE   .cmp_true@46
   DEY
   LDA   (FP),Y
   CMP   #$09
   BNE   .cmp_true@46
   LDA   #$00
   JMP   .cmp_end@47
.cmp_true@46:
   LDA   #$01
.cmp_end@47:
   LDY   #$9F
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
.if_end@12:
   LDY   #$B4
   LDA   (FP),Y
   LDY   #$A3
   STA   (FP),Y
   LDY   #$B5
   LDA   (FP),Y
   LDY   #$A4
   STA   (FP),Y
   LDA   #$FF
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$A4
   LDA   (FP),Y
   LDY   #$A6
   CMP   (FP),Y
   BNE   .cmp_true@49
   LDY   #$A3
   LDA   (FP),Y
   LDY   #$A5
   CMP   (FP),Y
   BNE   .cmp_true@49
   LDA   #$00
   JMP   .cmp_end@50
.cmp_true@49:
   LDA   #$01
.cmp_end@50:
   LDY   #$A7
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
   LDY   #$A9
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   LDY   #$AA
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   LDY   #$AA
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
   ADC   #$B8
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$AB
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

   LDA   #$80
   LDY   #$01
   STA   (FP),Y
   LDA   #$FF
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$0C
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   #$01
   LDY   #$01
   STA   (SSP),Y
   LDA   #$00
   INY
   STA   (SSP),Y
   INY
   STA   (SSP),Y
   INY
   STA   (SSP),Y
   LDY   #$01
   LDA   (FP),Y
   LDY   #$05
   STA   (SSP),Y
   LDA   #$00
   INY
   STA   (SSP),Y
   INY
   STA   (SSP),Y
   LDY   #$02
   LDA   (FP),Y
   LDY   #$08
   STA   (SSP),Y
   LDY   #$03
   LDA   (FP),Y
   LDY   #$09
   STA   (SSP),Y
   LDA   #$09
   INY
   STA   (SSP),Y
   LDA   #$96
   INY
   STA   (SSP),Y
   LDA   #$01
   INY
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
