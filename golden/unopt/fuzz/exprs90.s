SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24

check:
   SUBROUTINE

   ; prologue: 15 arg bytes, 133 local bytes
   SEC
   LDA   SSP
   SBC   #$87
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$86
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
   LDA   #$FF
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
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
   LDY   #$07
   STA   (FP),Y
   LDY   #$05
   LDA   (FP),Y
   LDY   #$8E
   ORA   (FP),Y
   LDY   #$08
   STA   (FP),Y
   LDY   #$06
   LDA   (FP),Y
   LDY   #$8F
   ORA   (FP),Y
   LDY   #$09
   STA   (FP),Y
   LDY   #$07
   LDA   (FP),Y
   LDY   #$90
   ORA   (FP),Y
   LDY   #$0A
   STA   (FP),Y
   LDY   #$88
   LDA   (FP),Y
   LDY   #$0B
   STA   (FP),Y
   LDY   #$89
   LDA   (FP),Y
   LDY   #$0C
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$08
   LDA   (FP),Y
   CLC
   LDY   #$0B
   ADC   (FP),Y
   LDY   #$0E
   STA   (FP),Y
   LDY   #$09
   LDA   (FP),Y
   LDY   #$0C
   ADC   (FP),Y
   LDY   #$0F
   STA   (FP),Y
   LDY   #$0A
   LDA   (FP),Y
   LDY   #$0D
   ADC   (FP),Y
   LDY   #$10
   STA   (FP),Y
   LDY   #$0E
   LDA   (FP),Y
   LDY   #$11
   STA   (FP),Y
   LDY   #$0F
   LDA   (FP),Y
   LDY   #$12
   STA   (FP),Y
   LDY   #$10
   LDA   (FP),Y
   AND   #$03
   LDY   #$13
   STA   (FP),Y
   LDY   #$11
   LDA   (FP),Y
   LDY   #$14
   STA   (FP),Y
   LDY   #$12
   LDA   (FP),Y
   LDY   #$15
   STA   (FP),Y
   LDY   #$13
   LDA   (FP),Y
   EOR   #$02
   LDY   #$16
   STA   (FP),Y
   LDY   #$14
   LDA   (FP),Y
   LDY   #$17
   STA   (FP),Y
   LDY   #$15
   LDA   (FP),Y
   LDY   #$18
   STA   (FP),Y
   LDY   #$16
   LDA   (FP),Y
   SEC
   SBC   #$02
   LDY   #$19
   STA   (FP),Y
   LDY   #$17
   LDA   (FP),Y
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
   CMP   #$00
   BNE   .cmp_true@2
   DEY
   LDA   (FP),Y
   CMP   #$FF
   BNE   .cmp_true@2
   DEY
   LDA   (FP),Y
   CMP   #$FE
   BNE   .cmp_true@2
   LDA   #$00
   JMP   .cmp_end@3
.cmp_true@2:
   LDA   #$01
.cmp_end@3:
   LDY   #$1D
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
   LDY   #$1F
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   LDY   #$20
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   LDY   #$20
   LDA   (FP),Y
   LDY   #$02
   STA   (FP),Y
.if_end@0:
   LDY   #$8A
   LDA   (FP),Y
   LDY   #$21
   STA   (FP),Y
   LDY   #$8B
   LDA   (FP),Y
   LDY   #$22
   STA   (FP),Y
   LDY   #$8C
   LDA   (FP),Y
   LDY   #$23
   STA   (FP),Y
   LDY   #$8D
   LDA   (FP),Y
   LDY   #$24
   STA   (FP),Y
   CMP   #$80
   BNE   .cmp_true@5
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@5
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@5
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@5
   LDA   #$00
   JMP   .cmp_end@6
.cmp_true@5:
   LDA   #$01
.cmp_end@6:
   LDY   #$25
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
.if_end@1:
   LDY   #$96
   LDA   (FP),Y
   BEQ   .lnot_true@8
   LDA   #$00
   JMP   .lnot_end@9
.lnot_true@8:
   LDA   #$01
.lnot_end@9:
   LDY   #$29
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$2B
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$2C
   STA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@10
   DEY
   LDA   (FP),Y
   CMP   #$01
   BNE   .cmp_true@10
   LDA   #$00
   JMP   .cmp_end@11
.cmp_true@10:
   LDA   #$01
.cmp_end@11:
   LDY   #$2D
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
   LDY   #$2F
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   LDY   #$30
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   LDY   #$30
   LDA   (FP),Y
   LDY   #$02
   STA   (FP),Y
.if_end@2:
   LDA   #$FF
   LDY   #$31
   STA   (FP),Y
   INY
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@13
   LDA   #$00
   JMP   .sx_done@14
.sx_neg@13:
   LDA   #$FF
.sx_done@14:
   LDY   #$33
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$34
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$35
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@15
   LDA   #$00
   JMP   .sx_done@16
.sx_neg@15:
   LDA   #$FF
.sx_done@16:
   LDY   #$36
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$34
   LDA   (FP),Y
   LDY   #$91
   AND   (FP),Y
   LDY   #$38
   STA   (FP),Y
   LDY   #$35
   LDA   (FP),Y
   LDY   #$92
   AND   (FP),Y
   LDY   #$39
   STA   (FP),Y
   LDY   #$36
   LDA   (FP),Y
   LDY   #$93
   AND   (FP),Y
   LDY   #$3A
   STA   (FP),Y
   LDY   #$37
   LDA   (FP),Y
   LDY   #$94
   AND   (FP),Y
   LDY   #$3B
   STA   (FP),Y
   LDY   #$38
   LDA   (FP),Y
   LDY   #$3C
   STA   (FP),Y
   LDY   #$39
   LDA   (FP),Y
   LDY   #$3D
   STA   (FP),Y
   LDY   #$3A
   LDA   (FP),Y
   LDY   #$3E
   STA   (FP),Y
   LDY   #$3B
   LDA   (FP),Y
   LDY   #$3F
   STA   (FP),Y
   CMP   #$80
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
   LDA   #$00
   JMP   .cmp_end@18
.cmp_true@17:
   LDA   #$01
.cmp_end@18:
   LDY   #$40
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
   LDY   #$42
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   LDY   #$43
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   LDY   #$43
   LDA   (FP),Y
   LDY   #$02
   STA   (FP),Y
.if_end@3:
   LDY   #$88
   LDA   (FP),Y
   LDY   #$44
   STA   (FP),Y
   LDY   #$89
   LDA   (FP),Y
   LDY   #$45
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$44
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
   LDY   #$91
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
   LDY   #$48
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
   LDY   #$48
   LDA   (FP),Y
   LDY   #$4C
   STA   (FP),Y
   LDY   #$49
   LDA   (FP),Y
   LDY   #$4D
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$4E
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$4F
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$4E
   LDA   (FP),Y
   SEC
   LDY   #$91
   SBC   (FP),Y
   LDY   #$4F
   LDA   (FP),Y
   LDY   #$92
   SBC   (FP),Y
   LDY   #$50
   LDA   (FP),Y
   LDY   #$93
   SBC   (FP),Y
   LDY   #$51
   LDA   (FP),Y
   LDY   #$94
   SBC   (FP),Y
   BVC   .cmp_novf@20
   EOR   #$80
.cmp_novf@20:
   ASL
   LDA   #$00
   ROL
   LDY   #$52
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$54
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$55
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
   LDY   #$56
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
.if_end@4:
   LDY   #$8A
   LDA   (FP),Y
   LDY   #$5A
   STA   (FP),Y
   LDY   #$8B
   LDA   (FP),Y
   LDY   #$5B
   STA   (FP),Y
   LDY   #$8C
   LDA   (FP),Y
   LDY   #$5C
   STA   (FP),Y
   LDY   #$8D
   LDA   (FP),Y
   LDY   #$5D
   STA   (FP),Y
   CMP   #$80
   BNE   .cmp_true@24
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@24
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@24
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@24
   LDA   #$00
   JMP   .cmp_end@25
.cmp_true@24:
   LDA   #$01
.cmp_end@25:
   LDY   #$5E
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
.if_end@5:
   LDY   #$8A
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
   LDA   HARGS+12
   LDY   #$62
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
   LDY   #$8E
   LDA   (FP),Y
   STA   HARGS
   INY
   LDA   (FP),Y
   STA   HARGS+1
   INY
   LDA   (FP),Y
   STA   HARGS+2
   ASL
   LDA   #$00
   ADC   #$FF
   EOR   #$FF
   STA   HARGS+3
   LDY   #$8E
   LDA   (FP),Y
   STA   HARGS+4
   INY
   LDA   (FP),Y
   STA   HARGS+5
   INY
   LDA   (FP),Y
   STA   HARGS+6
   ASL
   LDA   #$00
   ADC   #$FF
   EOR   #$FF
   STA   HARGS+7
   JSR   sdivmod32
   LDA   HARGS+8
   LDY   #$66
   STA   (FP),Y
   LDA   HARGS+9
   INY
   STA   (FP),Y
   LDA   HARGS+10
   INY
   STA   (FP),Y
   LDY   #$66
   LDA   (FP),Y
   LDY   #$69
   STA   (FP),Y
   LDY   #$67
   LDA   (FP),Y
   LDY   #$6A
   STA   (FP),Y
   LDY   #$68
   LDA   (FP),Y
   AND   #$03
   LDY   #$6B
   STA   (FP),Y
   LDY   #$69
   LDA   (FP),Y
   LDY   #$6C
   STA   (FP),Y
   LDY   #$6A
   LDA   (FP),Y
   LDY   #$6D
   STA   (FP),Y
   LDY   #$6B
   LDA   (FP),Y
   EOR   #$02
   LDY   #$6E
   STA   (FP),Y
   LDY   #$6C
   LDA   (FP),Y
   LDY   #$6F
   STA   (FP),Y
   LDY   #$6D
   LDA   (FP),Y
   LDY   #$70
   STA   (FP),Y
   LDY   #$6E
   LDA   (FP),Y
   SEC
   SBC   #$02
   LDY   #$71
   STA   (FP),Y
   LDY   #$6F
   LDA   (FP),Y
   LDY   #$72
   STA   (FP),Y
   LDY   #$70
   LDA   (FP),Y
   LDY   #$73
   STA   (FP),Y
   LDY   #$71
   LDA   (FP),Y
   LDY   #$74
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@27
   LDA   #$00
   JMP   .sx_done@28
.sx_neg@27:
   LDA   #$FF
.sx_done@28:
   LDY   #$75
   STA   (FP),Y
   LDY   #$62
   LDA   (FP),Y
   SEC
   LDY   #$72
   SBC   (FP),Y
   LDY   #$63
   LDA   (FP),Y
   LDY   #$73
   SBC   (FP),Y
   LDY   #$64
   LDA   (FP),Y
   LDY   #$74
   SBC   (FP),Y
   LDY   #$65
   LDA   (FP),Y
   LDY   #$75
   SBC   (FP),Y
   BVC   .cmp_novf@29
   EOR   #$80
.cmp_novf@29:
   ASL
   LDA   #$00
   ROL
   EOR   #$01
   LDY   #$76
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lnot_true@30
   LDA   #$00
   JMP   .lnot_end@31
.lnot_true@30:
   LDA   #$01
.lnot_end@31:
   LDY   #$78
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$7A
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$7B
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
   LDY   #$7C
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
   LDY   #$7E
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   LDY   #$7F
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   LDY   #$7F
   LDA   (FP),Y
   LDY   #$02
   STA   (FP),Y
.if_end@6:
   LDY   #$88
   LDA   (FP),Y
   LDY   #$80
   STA   (FP),Y
   LDY   #$89
   LDA   (FP),Y
   LDY   #$81
   STA   (FP),Y
   CMP   #$FF
   BNE   .cmp_true@35
   DEY
   LDA   (FP),Y
   CMP   #$FF
   BNE   .cmp_true@35
   LDA   #$00
   JMP   .cmp_end@36
.cmp_true@35:
   LDA   #$01
.cmp_end@36:
   LDY   #$82
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
   ADC   #$96
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$86
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

   ; prologue: 0 arg bytes, 4 local bytes
   SEC
   LDA   SSP
   SBC   #$06
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$05
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   LDA   #$0B
   LDY   #$01
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$0F
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   #$FF
   DEY
   STA   (SSP),Y
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
   LDA   #$00
   INY
   STA   (SSP),Y
   INY
   STA   (SSP),Y
   LDA   #$FE
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
   LDY   #$01
   LDA   (FP),Y
   LDY   #$0E
   STA   (SSP),Y
   LDY   #$02
   LDA   (FP),Y
   LDY   #$0F
   STA   (SSP),Y
   JSR   check
   LDY   #$03
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
   ADC   #$06
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$05
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
