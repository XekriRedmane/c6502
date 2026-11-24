SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24

check:
   SUBROUTINE

   ; prologue: 18 arg bytes, 104 local bytes
   SEC
   LDA   SSP
   SBC   #$6A
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$69
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
   LDY   #$75
   LDA   (FP),Y
   LDY   #$03
   STA   (FP),Y
   LDY   #$76
   LDA   (FP),Y
   LDY   #$04
   STA   (FP),Y
   LDY   #$77
   LDA   (FP),Y
   LDY   #$05
   STA   (FP),Y
   LDY   #$78
   LDA   (FP),Y
   LDY   #$06
   STA   (FP),Y
   LDY   #$03
   LDA   (FP),Y
   LDY   #$79
   AND   (FP),Y
   LDY   #$07
   STA   (FP),Y
   LDY   #$04
   LDA   (FP),Y
   LDY   #$7A
   AND   (FP),Y
   LDY   #$08
   STA   (FP),Y
   LDY   #$05
   LDA   (FP),Y
   LDY   #$7B
   AND   (FP),Y
   LDY   #$09
   STA   (FP),Y
   LDY   #$06
   LDA   (FP),Y
   LDY   #$7C
   AND   (FP),Y
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
   CMP   #$00
   BNE   .cmp_true@0
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@0
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@0
   DEY
   LDA   (FP),Y
   CMP   #$08
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
   LDY   #$73
   LDA   (FP),Y
   LDY   #$13
   STA   (FP),Y
   LDY   #$74
   LDA   (FP),Y
   LDY   #$14
   STA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@3
   DEY
   LDA   (FP),Y
   CMP   #$04
   BNE   .cmp_true@3
   LDA   #$00
   JMP   .cmp_end@4
.cmp_true@3:
   LDA   #$01
.cmp_end@4:
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
   LDY   #$75
   LDA   (FP),Y
   LDY   #$19
   STA   (FP),Y
   LDY   #$76
   LDA   (FP),Y
   LDY   #$1A
   STA   (FP),Y
   LDY   #$77
   LDA   (FP),Y
   LDY   #$1B
   STA   (FP),Y
   LDY   #$78
   LDA   (FP),Y
   LDY   #$1C
   STA   (FP),Y
   CMP   #$E6
   BNE   .cmp_true@6
   DEY
   LDA   (FP),Y
   CMP   #$34
   BNE   .cmp_true@6
   DEY
   LDA   (FP),Y
   CMP   #$2C
   BNE   .cmp_true@6
   DEY
   LDA   (FP),Y
   CMP   #$1C
   BNE   .cmp_true@6
   LDA   #$00
   JMP   .cmp_end@7
.cmp_true@6:
   LDA   #$01
.cmp_end@7:
   LDY   #$1D
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
.if_end@2:
   LDA   #$00
   LDY   #$21
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$80
   INY
   STA   (FP),Y
   CMP   #$80
   BNE   .cmp_true@9
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@9
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@9
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@9
   LDA   #$00
   JMP   .cmp_end@10
.cmp_true@9:
   LDA   #$01
.cmp_end@10:
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
   ORA   #$08
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
   LDY   #$6D
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   INY
   ORA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@4
   INY
   LDA   (FP),Y
   LDY   #$29
   STA   (FP),Y
   LDY   #$72
   LDA   (FP),Y
   LDY   #$2A
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@12
   LDA   #$00
   JMP   .sx_done@13
.sx_neg@12:
   LDA   #$FF
.sx_done@13:
   LDY   #$2B
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$79
   LDA   (FP),Y
   SEC
   LDY   #$29
   SBC   (FP),Y
   LDY   #$7A
   LDA   (FP),Y
   LDY   #$2A
   SBC   (FP),Y
   LDY   #$7B
   LDA   (FP),Y
   LDY   #$2B
   SBC   (FP),Y
   LDY   #$7C
   LDA   (FP),Y
   LDY   #$2C
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
   INY
   ORA   (FP),Y
   BNE   .or_true@4
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   JMP   .or_end@5
.or_true@4:
   LDA   #$01
   LDY   #$2F
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@5:
   LDY   #$6B
   LDA   (FP),Y
   EOR   #$FF
   LDY   #$31
   STA   (FP),Y
   LDY   #$6C
   LDA   (FP),Y
   EOR   #$FF
   LDY   #$32
   STA   (FP),Y
   LDY   #$73
   LDA   (FP),Y
   LDY   #$33
   STA   (FP),Y
   LDY   #$74
   LDA   (FP),Y
   LDY   #$34
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@14
   LDA   #$00
   JMP   .sx_done@15
.sx_neg@14:
   LDA   #$FF
.sx_done@15:
   LDY   #$35
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$6D
   LDA   (FP),Y
   CLC
   LDY   #$33
   SBC   (FP),Y
   LDY   #$6E
   LDA   (FP),Y
   LDY   #$34
   SBC   (FP),Y
   LDY   #$6F
   LDA   (FP),Y
   LDY   #$35
   SBC   (FP),Y
   LDY   #$70
   LDA   (FP),Y
   LDY   #$36
   SBC   (FP),Y
   LDA   #$00
   ROL
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$31
   LDA   (FP),Y
   STA   HARGS
   INY
   LDA   (FP),Y
   STA   HARGS+1
   LDY   #$37
   LDA   (FP),Y
   STA   HARGS+2
   INY
   LDA   (FP),Y
   STA   HARGS+3
   JSR   sdivmod16
   LDA   HARGS+6
   LDY   #$39
   STA   (FP),Y
   LDA   HARGS+7
   INY
   STA   (FP),Y
   LDY   #$2F
   LDA   (FP),Y
   SEC
   LDY   #$39
   SBC   (FP),Y
   LDY   #$30
   LDA   (FP),Y
   LDY   #$3A
   SBC   (FP),Y
   BVC   .cmp_novf@16
   EOR   #$80
.cmp_novf@16:
   ASL
   LDA   #$00
   ROL
   LDY   #$3B
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$3D
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$3E
   STA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@17
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@17
   LDA   #$00
   JMP   .cmp_end@18
.cmp_true@17:
   LDA   #$01
.cmp_end@18:
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
   LDY   #$6B
   LDA   (FP),Y
   LDY   #$43
   STA   (FP),Y
   LDY   #$6C
   LDA   (FP),Y
   LDY   #$44
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@20
   LDA   #$00
   JMP   .sx_done@21
.sx_neg@20:
   LDA   #$FF
.sx_done@21:
   LDY   #$45
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$43
   LDA   (FP),Y
   LDY   #$75
   EOR   (FP),Y
   LDY   #$47
   STA   (FP),Y
   LDY   #$44
   LDA   (FP),Y
   LDY   #$76
   EOR   (FP),Y
   LDY   #$48
   STA   (FP),Y
   LDY   #$45
   LDA   (FP),Y
   LDY   #$77
   EOR   (FP),Y
   LDY   #$49
   STA   (FP),Y
   LDY   #$46
   LDA   (FP),Y
   LDY   #$78
   EOR   (FP),Y
   LDY   #$4A
   STA   (FP),Y
   LDY   #$75
   LDA   (FP),Y
   SEC
   LDY   #$47
   SBC   (FP),Y
   LDY   #$76
   LDA   (FP),Y
   LDY   #$48
   SBC   (FP),Y
   LDY   #$77
   LDA   (FP),Y
   LDY   #$49
   SBC   (FP),Y
   LDY   #$78
   LDA   (FP),Y
   LDY   #$4A
   SBC   (FP),Y
   BVC   .cmp_novf@22
   EOR   #$80
.cmp_novf@22:
   ASL
   LDA   #$00
   ROL
   LDY   #$4B
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$4D
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$4E
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@23
   LDA   #$00
   JMP   .sx_done@24
.sx_neg@23:
   LDA   #$FF
.sx_done@24:
   LDY   #$4F
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$75
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
   LDY   #$4D
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
   LDY   #$51
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
   LDY   #$54
   LDA   (FP),Y
   LDY   #$58
   STA   (FP),Y
   CMP   #$E6
   BNE   .cmp_true@25
   DEY
   LDA   (FP),Y
   CMP   #$34
   BNE   .cmp_true@25
   DEY
   LDA   (FP),Y
   CMP   #$2C
   BNE   .cmp_true@25
   DEY
   LDA   (FP),Y
   CMP   #$1C
   BNE   .cmp_true@25
   LDA   #$00
   JMP   .cmp_end@26
.cmp_true@25:
   LDA   #$01
.cmp_end@26:
   LDY   #$59
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
   LDY   #$5B
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   LDY   #$5C
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   LDY   #$5C
   LDA   (FP),Y
   LDY   #$02
   STA   (FP),Y
.if_end@7:
   LDY   #$71
   LDA   (FP),Y
   LDY   #$5D
   STA   (FP),Y
   LDY   #$72
   LDA   (FP),Y
   LDY   #$5E
   STA   (FP),Y
   CMP   #$E0
   BNE   .cmp_true@28
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@28
   LDA   #$00
   JMP   .cmp_end@29
.cmp_true@28:
   LDA   #$01
.cmp_end@29:
   LDY   #$5F
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
   LDY   #$61
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   LDY   #$62
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   LDY   #$62
   LDA   (FP),Y
   LDY   #$02
   STA   (FP),Y
.if_end@8:
   LDY   #$73
   LDA   (FP),Y
   LDY   #$63
   STA   (FP),Y
   LDY   #$74
   LDA   (FP),Y
   LDY   #$64
   STA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@31
   DEY
   LDA   (FP),Y
   CMP   #$04
   BNE   .cmp_true@31
   LDA   #$00
   JMP   .cmp_end@32
.cmp_true@31:
   LDA   #$01
.cmp_end@32:
   LDY   #$65
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
   LDY   #$67
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   LDY   #$68
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   LDY   #$68
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
   ADC   #$7C
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$69
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

   LDA   #$FF
   LDY   #$01
   STA   (FP),Y
   INY
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$12
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   DEY
   LDA   (FP),Y
   STA   (SSP),Y
   INY
   LDA   (FP),Y
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
   LDA   #$00
   INY
   STA   (SSP),Y
   LDA   #$E0
   INY
   STA   (SSP),Y
   LDA   #$04
   INY
   STA   (SSP),Y
   LDA   #$00
   INY
   STA   (SSP),Y
   LDA   #$1C
   INY
   STA   (SSP),Y
   LDA   #$2C
   INY
   STA   (SSP),Y
   LDA   #$34
   INY
   STA   (SSP),Y
   LDA   #$E6
   INY
   STA   (SSP),Y
   LDA   #$0A
   INY
   STA   (SSP),Y
   LDA   #$00
   INY
   STA   (SSP),Y
   INY
   STA   (SSP),Y
   INY
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
