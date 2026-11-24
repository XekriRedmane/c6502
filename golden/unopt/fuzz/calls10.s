SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24

f2:
   SUBROUTINE

   ; prologue: 0 arg bytes, 157 local bytes
   SEC
   LDA   SSP
   SBC   #$9F
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$9E
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   LDA   g1
   LDY   #$01
   STA   (FP),Y
   LDA   g1
   BMI   .sx_neg@0
   LDA   #$00
   JMP   .sx_done@1
.sx_neg@0:
   LDA   #$FF
.sx_done@1:
   LDY   #$02
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$03
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$04
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@2
   LDA   #$00
   JMP   .sx_done@3
.sx_neg@2:
   LDA   #$FF
.sx_done@3:
   LDY   #$05
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   CMP   g0+6
   BNE   .thread@0
   CMP   g0+5
   BNE   .thread@0
   CMP   g0+4
   BNE   .thread@0
   CMP   g0+3
   BNE   .thread@0
   CMP   g0+2
   BNE   .thread@0
   LDY   #$04
   LDA   (FP),Y
   CMP   g0+1
   BNE   .thread@0
   DEY
   LDA   (FP),Y
   CMP   g0
   BEQ   .cmp_true@4
.thread@0:
   LDA   #$00
   JMP   .cmp_end@5
.cmp_true@4:
   LDA   #$01
.cmp_end@5:
   LDY   #$0A
   STA   (FP),Y
   LDA   #$00
   INY
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
   BMI   .sx_neg@7
   LDA   #$00
   JMP   .sx_done@8
.sx_neg@7:
   LDA   #$FF
.sx_done@8:
   LDY   #$0E
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$0C
   LDA   (FP),Y
   LDY   #$10
   STA   (FP),Y
   LDY   #$0D
   LDA   (FP),Y
   LDY   #$11
   STA   (FP),Y
   LDY   #$0E
   LDA   (FP),Y
   LDY   #$12
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   g0
   INY
   STA   (FP),Y
   LDA   g0+1
   INY
   STA   (FP),Y
   LDA   g0+2
   INY
   STA   (FP),Y
   LDA   g0+3
   INY
   STA   (FP),Y
   LDA   g0+4
   INY
   STA   (FP),Y
   LDA   g0+5
   INY
   STA   (FP),Y
   LDA   g0+6
   INY
   STA   (FP),Y
   LDY   #$14
   LDA   (FP),Y
   LDY   #$1B
   STA   (FP),Y
   LDY   #$15
   LDA   (FP),Y
   LDY   #$1C
   STA   (FP),Y
   LDY   #$16
   LDA   (FP),Y
   LDY   #$1D
   STA   (FP),Y
   LDY   #$17
   LDA   (FP),Y
   LDY   #$1E
   STA   (FP),Y
   LDY   #$18
   LDA   (FP),Y
   LDY   #$1F
   STA   (FP),Y
   LDY   #$19
   LDA   (FP),Y
   LDY   #$20
   STA   (FP),Y
   LDY   #$1A
   LDA   (FP),Y
   AND   #$1F
   LDY   #$21
   STA   (FP),Y
   LDY   #$1B
   LDA   (FP),Y
   LDY   #$22
   STA   (FP),Y
   LDY   #$1C
   LDA   (FP),Y
   LDY   #$23
   STA   (FP),Y
   LDY   #$1D
   LDA   (FP),Y
   LDY   #$24
   STA   (FP),Y
   LDY   #$1E
   LDA   (FP),Y
   LDY   #$25
   STA   (FP),Y
   LDY   #$1F
   LDA   (FP),Y
   LDY   #$26
   STA   (FP),Y
   LDY   #$20
   LDA   (FP),Y
   LDY   #$27
   STA   (FP),Y
   LDY   #$21
   LDA   (FP),Y
   LDY   #$28
   STA   (FP),Y
   LDA   g0
   INY
   STA   (FP),Y
   LDA   g0+1
   INY
   STA   (FP),Y
   LDA   g0+2
   INY
   STA   (FP),Y
   LDA   g0+3
   INY
   STA   (FP),Y
   LDA   g0+4
   INY
   STA   (FP),Y
   LDA   g0+5
   INY
   STA   (FP),Y
   LDA   g0+6
   INY
   STA   (FP),Y
   LDY   #$29
   LDA   (FP),Y
   LDY   #$30
   STA   (FP),Y
   LDY   #$2A
   LDA   (FP),Y
   LDY   #$31
   STA   (FP),Y
   LDY   #$2B
   LDA   (FP),Y
   LDY   #$32
   STA   (FP),Y
   LDY   #$2C
   LDA   (FP),Y
   LDY   #$33
   STA   (FP),Y
   LDY   #$2D
   LDA   (FP),Y
   LDY   #$34
   STA   (FP),Y
   LDY   #$2E
   LDA   (FP),Y
   LDY   #$35
   STA   (FP),Y
   LDY   #$2F
   LDA   (FP),Y
   AND   #$1F
   LDY   #$36
   STA   (FP),Y
   LDY   #$30
   LDA   (FP),Y
   CLC
   LDY   #$22
   SBC   (FP),Y
   LDY   #$31
   LDA   (FP),Y
   LDY   #$23
   SBC   (FP),Y
   LDY   #$32
   LDA   (FP),Y
   LDY   #$24
   SBC   (FP),Y
   LDY   #$33
   LDA   (FP),Y
   LDY   #$25
   SBC   (FP),Y
   LDY   #$34
   LDA   (FP),Y
   LDY   #$26
   SBC   (FP),Y
   LDY   #$35
   LDA   (FP),Y
   LDY   #$27
   SBC   (FP),Y
   LDY   #$36
   LDA   (FP),Y
   LDY   #$28
   SBC   (FP),Y
   LDA   #$00
   ROL
   LDY   #$37
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$39
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$3A
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@9
   LDA   #$00
   JMP   .sx_done@10
.sx_neg@9:
   LDA   #$FF
.sx_done@10:
   LDY   #$3B
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$39
   LDA   (FP),Y
   LDY   #$40
   STA   (FP),Y
   LDY   #$3A
   LDA   (FP),Y
   LDY   #$41
   STA   (FP),Y
   LDY   #$3B
   LDA   (FP),Y
   LDY   #$42
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   AND   #$1F
   INY
   STA   (FP),Y
   SEC
   LDA   #$00
   LDY   #$22
   SBC   (FP),Y
   LDY   #$47
   STA   (FP),Y
   LDA   #$00
   LDY   #$23
   SBC   (FP),Y
   LDY   #$48
   STA   (FP),Y
   LDA   #$00
   LDY   #$24
   SBC   (FP),Y
   LDY   #$49
   STA   (FP),Y
   LDA   #$00
   LDY   #$25
   SBC   (FP),Y
   LDY   #$4A
   STA   (FP),Y
   LDA   #$00
   LDY   #$26
   SBC   (FP),Y
   LDY   #$4B
   STA   (FP),Y
   LDA   #$00
   LDY   #$27
   SBC   (FP),Y
   LDY   #$4C
   STA   (FP),Y
   LDA   #$00
   LDY   #$28
   SBC   (FP),Y
   LDY   #$4D
   STA   (FP),Y
   LDY   #$47
   LDA   (FP),Y
   LDY   #$4E
   STA   (FP),Y
   LDY   #$48
   LDA   (FP),Y
   LDY   #$4F
   STA   (FP),Y
   LDY   #$49
   LDA   (FP),Y
   LDY   #$50
   STA   (FP),Y
   LDY   #$4A
   LDA   (FP),Y
   LDY   #$51
   STA   (FP),Y
   LDY   #$4B
   LDA   (FP),Y
   LDY   #$52
   STA   (FP),Y
   LDY   #$4C
   LDA   (FP),Y
   LDY   #$53
   STA   (FP),Y
   LDY   #$4D
   LDA   (FP),Y
   AND   #$1F
   LDY   #$54
   STA   (FP),Y
   LDY   #$40
   LDA   (FP),Y
   SEC
   LDY   #$4E
   SBC   (FP),Y
   LDY   #$55
   STA   (FP),Y
   LDY   #$41
   LDA   (FP),Y
   LDY   #$4F
   SBC   (FP),Y
   LDY   #$56
   STA   (FP),Y
   LDY   #$42
   LDA   (FP),Y
   LDY   #$50
   SBC   (FP),Y
   LDY   #$57
   STA   (FP),Y
   LDY   #$43
   LDA   (FP),Y
   LDY   #$51
   SBC   (FP),Y
   LDY   #$58
   STA   (FP),Y
   LDY   #$44
   LDA   (FP),Y
   LDY   #$52
   SBC   (FP),Y
   LDY   #$59
   STA   (FP),Y
   LDY   #$45
   LDA   (FP),Y
   LDY   #$53
   SBC   (FP),Y
   LDY   #$5A
   STA   (FP),Y
   LDY   #$46
   LDA   (FP),Y
   LDY   #$54
   SBC   (FP),Y
   LDY   #$5B
   STA   (FP),Y
   LDY   #$55
   LDA   (FP),Y
   LDY   #$5C
   STA   (FP),Y
   LDY   #$56
   LDA   (FP),Y
   LDY   #$5D
   STA   (FP),Y
   LDY   #$57
   LDA   (FP),Y
   LDY   #$5E
   STA   (FP),Y
   LDY   #$58
   LDA   (FP),Y
   LDY   #$5F
   STA   (FP),Y
   LDY   #$59
   LDA   (FP),Y
   LDY   #$60
   STA   (FP),Y
   LDY   #$5A
   LDA   (FP),Y
   LDY   #$61
   STA   (FP),Y
   LDY   #$5B
   LDA   (FP),Y
   AND   #$1F
   LDY   #$62
   STA   (FP),Y
   LDY   #$5C
   LDA   (FP),Y
   LDY   #$63
   STA   (FP),Y
   LDY   #$5D
   LDA   (FP),Y
   LDY   #$64
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$65
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$66
   STA   (FP),Y
   LDA   #$02
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   ORA   #$00
   BPL   .sx_done@12
   LDA   #$FF
.sx_done@12:
   LDY   #$69
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$01
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   ORA   #$00
   BPL   .sx_done@14
   LDA   #$FF
.sx_done@14:
   LDY   #$6D
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$10
   LDA   (FP),Y
   LDY   #$6B
   ORA   (FP),Y
   LDY   #$6F
   STA   (FP),Y
   LDY   #$11
   LDA   (FP),Y
   LDY   #$6C
   ORA   (FP),Y
   LDY   #$70
   STA   (FP),Y
   LDY   #$12
   LDA   (FP),Y
   LDY   #$6D
   ORA   (FP),Y
   LDY   #$71
   STA   (FP),Y
   LDY   #$13
   LDA   (FP),Y
   LDY   #$6E
   ORA   (FP),Y
   LDY   #$72
   STA   (FP),Y
   LDY   #$67
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
   LDY   #$6F
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
   LDY   #$73
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
   LDY   #$73
   LDA   (FP),Y
   LDY   #$77
   STA   (FP),Y
   LDY   #$74
   LDA   (FP),Y
   LDY   #$78
   STA   (FP),Y
   LDY   #$75
   LDA   (FP),Y
   LDY   #$79
   STA   (FP),Y
   LDY   #$76
   LDA   (FP),Y
   LDY   #$7A
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@15
   LDA   #$00
   JMP   .sx_done@16
.sx_neg@15:
   LDA   #$FF
.sx_done@16:
   LDY   #$7B
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   CMP   g0+6
   BNE   .thread@1
   CMP   g0+5
   BNE   .thread@1
   CMP   g0+4
   BNE   .thread@1
   LDY   #$7A
   LDA   (FP),Y
   CMP   g0+3
   BNE   .thread@1
   DEY
   LDA   (FP),Y
   CMP   g0+2
   BNE   .thread@1
   DEY
   LDA   (FP),Y
   CMP   g0+1
   BNE   .thread@1
   DEY
   LDA   (FP),Y
   CMP   g0
   BEQ   .cmp_true@17
.thread@1:
   LDA   #$00
   JMP   .cmp_end@18
.cmp_true@17:
   LDA   #$01
.cmp_end@18:
   LDY   #$7E
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$80
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$10
   LDA   (FP),Y
   LDY   #$82
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$65
   LDA   (FP),Y
   LDY   #$84
   STA   (FP),Y
   LDY   #$66
   LDA   (FP),Y
   LDY   #$85
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$10
   LDA   (FP),Y
   LDY   #$84
   AND   (FP),Y
   LDY   #$88
   STA   (FP),Y
   LDY   #$11
   LDA   (FP),Y
   LDY   #$85
   AND   (FP),Y
   LDY   #$89
   STA   (FP),Y
   LDY   #$12
   LDA   (FP),Y
   LDY   #$86
   AND   (FP),Y
   LDY   #$8A
   STA   (FP),Y
   LDY   #$13
   LDA   (FP),Y
   LDY   #$87
   AND   (FP),Y
   LDY   #$8B
   STA   (FP),Y
   LDY   #$81
   LDA   (FP),Y
   LDY   #$8C
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@20
   LDA   #$00
   JMP   .sx_done@21
.sx_neg@20:
   LDA   #$FF
.sx_done@21:
   LDY   #$8D
   STA   (FP),Y
   LDY   #$83
   LDA   (FP),Y
   LDY   #$8E
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$8C
   LDA   (FP),Y
   LDY   #$8E
   AND   (FP),Y
   LDY   #$90
   STA   (FP),Y
   LDY   #$8D
   LDA   (FP),Y
   LDY   #$8F
   AND   (FP),Y
   LDY   #$91
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
   BMI   .sx_neg@22
   LDA   #$00
   JMP   .sx_done@23
.sx_neg@22:
   LDA   #$FF
.sx_done@23:
   LDY   #$94
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$88
   LDA   (FP),Y
   LDY   #$92
   AND   (FP),Y
   LDY   #$96
   STA   (FP),Y
   LDY   #$89
   LDA   (FP),Y
   LDY   #$93
   AND   (FP),Y
   LDY   #$97
   STA   (FP),Y
   LDY   #$8A
   LDA   (FP),Y
   LDY   #$94
   AND   (FP),Y
   LDY   #$98
   STA   (FP),Y
   LDY   #$8B
   LDA   (FP),Y
   LDY   #$95
   AND   (FP),Y
   LDY   #$99
   STA   (FP),Y
   LDY   #$96
   LDA   (FP),Y
   LDY   #$9A
   STA   (FP),Y
   LDY   #$97
   LDA   (FP),Y
   LDY   #$9B
   STA   (FP),Y
   LDY   #$98
   LDA   (FP),Y
   LDY   #$9C
   STA   (FP),Y
   LDY   #$99
   LDA   (FP),Y
   LDY   #$9D
   STA   (FP),Y
   LDY   #$9A
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

   ; epilogue
   CLC
   LDA   FP
   ADC   #$9F
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   INY
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   RTS

f1:
   SUBROUTINE

   ; prologue: 2 arg bytes, 125 local bytes
   SEC
   LDA   SSP
   SBC   #$7F
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$7E
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   LDA   #$03
   LDY   #$01
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$04
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$05
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@24
   LDA   #$00
   JMP   .sx_done@25
.sx_neg@24:
   LDA   #$FF
.sx_done@25:
   LDY   #$06
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   g0
   SEC
   LDY   #$04
   SBC   (FP),Y
   LDA   g0+1
   INY
   SBC   (FP),Y
   LDA   g0+2
   INY
   SBC   (FP),Y
   LDA   g0+3
   INY
   SBC   (FP),Y
   LDA   g0+4
   INY
   SBC   (FP),Y
   LDA   g0+5
   INY
   SBC   (FP),Y
   LDA   g0+6
   INY
   SBC   (FP),Y
   BVC   .cmp_novf@26
   EOR   #$80
.cmp_novf@26:
   ASL
   LDA   #$00
   ROL
   EOR   #$01
   LDY   #$0B
   STA   (FP),Y
   LDA   #$00
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
   ORA   #$00
   BMI   .sx_neg@27
   LDA   #$00
   JMP   .sx_done@28
.sx_neg@27:
   LDA   #$FF
.sx_done@28:
   LDY   #$0F
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$0D
   LDA   (FP),Y
   LDY   #$11
   STA   (FP),Y
   LDY   #$0E
   LDA   (FP),Y
   LDY   #$12
   STA   (FP),Y
   LDY   #$0F
   LDA   (FP),Y
   LDY   #$13
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$11
   LDA   (FP),Y
   LDY   #$15
   STA   (FP),Y
   LDY   #$12
   LDA   (FP),Y
   LDY   #$16
   STA   (FP),Y
   LDY   #$13
   LDA   (FP),Y
   LDY   #$17
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
   CMP   g0+6
   BNE   .cmp_true@29
   CMP   g0+5
   BNE   .cmp_true@29
   CMP   g0+4
   BNE   .cmp_true@29
   LDY   #$18
   LDA   (FP),Y
   CMP   g0+3
   BNE   .cmp_true@29
   DEY
   LDA   (FP),Y
   CMP   g0+2
   BNE   .cmp_true@29
   DEY
   LDA   (FP),Y
   CMP   g0+1
   BNE   .cmp_true@29
   DEY
   LDA   (FP),Y
   CMP   g0
   BNE   .cmp_true@29
   LDA   #$00
   JMP   .cmp_end@30
.cmp_true@29:
   LDA   #$01
.cmp_end@30:
   LDY   #$1C
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$11
   LDA   (FP),Y
   SEC
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
   LDY   #$1E
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$1D
   LDA   (FP),Y
   LDY   #$1F
   CMP   (FP),Y
   BNE   .cmp_true@32
   LDY   #$1C
   LDA   (FP),Y
   LDY   #$1E
   CMP   (FP),Y
   BNE   .cmp_true@32
   LDA   #$00
   JMP   .cmp_end@33
.cmp_true@32:
   LDA   #$01
.cmp_end@33:
   LDY   #$20
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$22
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$23
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@35
   LDA   #$00
   JMP   .sx_done@36
.sx_neg@35:
   LDA   #$FF
.sx_done@36:
   LDY   #$24
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$22
   LDA   (FP),Y
   LDY   #$29
   STA   (FP),Y
   LDY   #$23
   LDA   (FP),Y
   LDY   #$2A
   STA   (FP),Y
   LDY   #$24
   LDA   (FP),Y
   LDY   #$2B
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   g0
   SEC
   SBC   #$E8
   LDA   g0+1
   SBC   #$03
   LDA   g0+2
   SBC   #$00
   LDA   g0+3
   SBC   #$00
   LDA   g0+4
   SBC   #$00
   LDA   g0+5
   SBC   #$00
   LDA   g0+6
   SBC   #$00
   BVC   .cmp_novf@37
   EOR   #$80
.cmp_novf@37:
   ASL
   LDA   #$00
   ROL
   LDY   #$30
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$32
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$35
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$36
   STA   (FP),Y
   LDY   #$29
   LDA   (FP),Y
   LDY   #$37
   STA   (FP),Y
   LDY   #$2A
   LDA   (FP),Y
   LDY   #$38
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$39
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$3A
   STA   (FP),Y
   LDY   #$11
   LDA   (FP),Y
   LDY   #$3B
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$80
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$3D
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$3E
   STA   (FP),Y
   LDY   #$3C
   LDA   (FP),Y
   LDY   #$3F
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDA   #$64
   DEY
   CMP   (FP),Y
   BEQ   .cmp_true@38
   LDA   #$00
   JMP   .cmp_end@39
.cmp_true@38:
   LDA   #$01
.cmp_end@39:
   LDY   #$41
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$3D
   LDA   (FP),Y
   CLC
   LDY   #$41
   ADC   (FP),Y
   LDY   #$43
   STA   (FP),Y
   LDY   #$3E
   LDA   (FP),Y
   LDY   #$42
   ADC   (FP),Y
   LDY   #$44
   STA   (FP),Y
   LDY   #$80
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$43
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$44
   LDA   (FP),Y
   LDY   #$01
   STA   (DPTR),Y
   JSR   f2
   LDA   HARGS+8
   LDY   #$45
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
   LDY   #$39
   LDA   (FP),Y
   LDY   #$49
   STA   (FP),Y
   LDY   #$3A
   LDA   (FP),Y
   LDY   #$4A
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@40
   LDA   #$00
   JMP   .sx_done@41
.sx_neg@40:
   LDA   #$FF
.sx_done@41:
   LDY   #$4B
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$49
   LDA   (FP),Y
   LDY   #$45
   EOR   (FP),Y
   LDY   #$4D
   STA   (FP),Y
   LDY   #$4A
   LDA   (FP),Y
   LDY   #$46
   EOR   (FP),Y
   LDY   #$4E
   STA   (FP),Y
   LDY   #$4B
   LDA   (FP),Y
   LDY   #$47
   EOR   (FP),Y
   LDY   #$4F
   STA   (FP),Y
   LDY   #$4C
   LDA   (FP),Y
   LDY   #$48
   EOR   (FP),Y
   LDY   #$50
   STA   (FP),Y
   LDY   #$4D
   LDA   (FP),Y
   LDY   #$51
   STA   (FP),Y
   LDY   #$4E
   LDA   (FP),Y
   LDY   #$52
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$39
   STA   (FP),Y
   LDY   #$52
   LDA   (FP),Y
   LDY   #$3A
   STA   (FP),Y
   LDY   #$80
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$53
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$54
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$55
   STA   (FP),Y
   ORA   #$00
   BEQ   .lb_skip@0
   JMP   .or_true@0
.lb_skip@0:
   LDA   g1
   LDY   #$56
   STA   (FP),Y
   LDA   g1
   BMI   .sx_neg@42
   LDA   #$00
   JMP   .sx_done@43
.sx_neg@42:
   LDA   #$FF
.sx_done@43:
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
   ORA   #$00
   BMI   .sx_neg@44
   LDA   #$00
   JMP   .sx_done@45
.sx_neg@44:
   LDA   #$FF
.sx_done@45:
   LDY   #$5A
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$29
   LDA   (FP),Y
   SEC
   LDY   #$58
   SBC   (FP),Y
   LDY   #$5F
   STA   (FP),Y
   LDY   #$2A
   LDA   (FP),Y
   LDY   #$59
   SBC   (FP),Y
   LDY   #$60
   STA   (FP),Y
   LDY   #$2B
   LDA   (FP),Y
   LDY   #$5A
   SBC   (FP),Y
   LDY   #$61
   STA   (FP),Y
   LDY   #$2C
   LDA   (FP),Y
   LDY   #$5B
   SBC   (FP),Y
   LDY   #$62
   STA   (FP),Y
   LDY   #$2D
   LDA   (FP),Y
   LDY   #$5C
   SBC   (FP),Y
   LDY   #$63
   STA   (FP),Y
   LDY   #$2E
   LDA   (FP),Y
   LDY   #$5D
   SBC   (FP),Y
   LDY   #$64
   STA   (FP),Y
   LDY   #$2F
   LDA   (FP),Y
   LDY   #$5E
   SBC   (FP),Y
   LDY   #$65
   STA   (FP),Y
   LDY   #$5F
   LDA   (FP),Y
   LDY   #$66
   STA   (FP),Y
   LDY   #$60
   LDA   (FP),Y
   LDY   #$67
   STA   (FP),Y
   LDY   #$61
   LDA   (FP),Y
   LDY   #$68
   STA   (FP),Y
   LDY   #$62
   LDA   (FP),Y
   LDY   #$69
   STA   (FP),Y
   LDY   #$63
   LDA   (FP),Y
   LDY   #$6A
   STA   (FP),Y
   LDY   #$64
   LDA   (FP),Y
   LDY   #$6B
   STA   (FP),Y
   LDY   #$65
   LDA   (FP),Y
   AND   #$01
   LDY   #$6C
   STA   (FP),Y
   LDY   #$66
   LDA   (FP),Y
   LDY   #$6D
   STA   (FP),Y
   LDY   #$67
   LDA   (FP),Y
   LDY   #$6E
   STA   (FP),Y
   LDY   #$68
   LDA   (FP),Y
   LDY   #$6F
   STA   (FP),Y
   LDY   #$69
   LDA   (FP),Y
   LDY   #$70
   STA   (FP),Y
   LDY   #$6A
   LDA   (FP),Y
   LDY   #$71
   STA   (FP),Y
   LDY   #$6B
   LDA   (FP),Y
   LDY   #$72
   STA   (FP),Y
   LDY   #$6C
   LDA   (FP),Y
   EOR   #$01
   LDY   #$73
   STA   (FP),Y
   LDY   #$6D
   LDA   (FP),Y
   LDY   #$74
   STA   (FP),Y
   LDY   #$6E
   LDA   (FP),Y
   LDY   #$75
   STA   (FP),Y
   LDY   #$6F
   LDA   (FP),Y
   LDY   #$76
   STA   (FP),Y
   LDY   #$70
   LDA   (FP),Y
   LDY   #$77
   STA   (FP),Y
   LDY   #$71
   LDA   (FP),Y
   LDY   #$78
   STA   (FP),Y
   LDY   #$72
   LDA   (FP),Y
   LDY   #$79
   STA   (FP),Y
   LDY   #$73
   LDA   (FP),Y
   SEC
   SBC   #$01
   LDY   #$7A
   STA   (FP),Y
   LDY   #$74
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   INY
   ORA   (FP),Y
   INY
   ORA   (FP),Y
   INY
   ORA   (FP),Y
   INY
   ORA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@0
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   JMP   .or_end@1
.or_true@0:
   LDA   #$01
   LDY   #$7B
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@1:
   LDY   #$7B
   LDA   (FP),Y
   LDY   #$7D
   STA   (FP),Y

   ; epilogue
   PHA
   CLC
   LDA   FP
   ADC   #$81
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   INY
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   PLA
   RTS

f0:
   SUBROUTINE

   ; prologue: 1 arg bytes, 128 local bytes
   SEC
   LDA   SSP
   SBC   #$82
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$81
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   LDA   g0
   LDY   #$01
   STA   (FP),Y
   LDA   g0+1
   INY
   STA   (FP),Y
   LDA   g0+2
   INY
   STA   (FP),Y
   LDA   g0+3
   INY
   STA   (FP),Y
   LDY   #$01
   LDA   (FP),Y
   LDY   #$05
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   LDY   #$06
   STA   (FP),Y
   LDY   #$03
   LDA   (FP),Y
   LDY   #$07
   STA   (FP),Y
   LDY   #$04
   LDA   (FP),Y
   LDY   #$08
   STA   (FP),Y
   JSR   f2
   LDA   HARGS+8
   LDY   #$09
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
   LDY   #$05
   LDA   (FP),Y
   LDY   #$0D
   STA   (FP),Y
   LDY   #$06
   LDA   (FP),Y
   LDY   #$0E
   STA   (FP),Y
   LDY   #$07
   LDA   (FP),Y
   LDY   #$0F
   STA   (FP),Y
   LDY   #$08
   LDA   (FP),Y
   LDY   #$10
   STA   (FP),Y
   LDY   #$0D
   LDA   (FP),Y
   LDY   #$09
   EOR   (FP),Y
   LDY   #$11
   STA   (FP),Y
   LDY   #$0E
   LDA   (FP),Y
   LDY   #$0A
   EOR   (FP),Y
   LDY   #$12
   STA   (FP),Y
   LDY   #$0F
   LDA   (FP),Y
   LDY   #$0B
   EOR   (FP),Y
   LDY   #$13
   STA   (FP),Y
   LDY   #$10
   LDA   (FP),Y
   LDY   #$0C
   EOR   (FP),Y
   LDY   #$14
   STA   (FP),Y
   LDY   #$11
   LDA   (FP),Y
   LDY   #$15
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
   LDY   #$05
   STA   (FP),Y
   LDY   #$16
   LDA   (FP),Y
   LDY   #$06
   STA   (FP),Y
   LDY   #$17
   LDA   (FP),Y
   LDY   #$07
   STA   (FP),Y
   LDY   #$18
   LDA   (FP),Y
   LDY   #$08
   STA   (FP),Y
   LDY   #$05
   LDA   (FP),Y
   LDY   #$19
   STA   (FP),Y
   LDY   #$06
   LDA   (FP),Y
   LDY   #$1A
   STA   (FP),Y
   LDY   #$07
   LDA   (FP),Y
   LDY   #$1B
   STA   (FP),Y
   LDY   #$08
   LDA   (FP),Y
   LDY   #$1C
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@46
   LDA   #$00
   JMP   .sx_done@47
.sx_neg@46:
   LDA   #$FF
.sx_done@47:
   LDY   #$1D
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   g0
   CLC
   LDY   #$19
   ADC   (FP),Y
   LDY   #$20
   STA   (FP),Y
   LDA   g0+1
   LDY   #$1A
   ADC   (FP),Y
   LDY   #$21
   STA   (FP),Y
   LDA   g0+2
   LDY   #$1B
   ADC   (FP),Y
   LDY   #$22
   STA   (FP),Y
   LDA   g0+3
   LDY   #$1C
   ADC   (FP),Y
   LDY   #$23
   STA   (FP),Y
   LDA   g0+4
   LDY   #$1D
   ADC   (FP),Y
   LDY   #$24
   STA   (FP),Y
   LDA   g0+5
   LDY   #$1E
   ADC   (FP),Y
   LDY   #$25
   STA   (FP),Y
   LDA   g0+6
   LDY   #$1F
   ADC   (FP),Y
   LDY   #$26
   STA   (FP),Y
   LDY   #$20
   LDA   (FP),Y
   LDY   #$27
   STA   (FP),Y
   LDY   #$21
   LDA   (FP),Y
   LDY   #$28
   STA   (FP),Y
   LDY   #$22
   LDA   (FP),Y
   LDY   #$29
   STA   (FP),Y
   LDY   #$23
   LDA   (FP),Y
   LDY   #$2A
   STA   (FP),Y
   LDY   #$24
   LDA   (FP),Y
   LDY   #$2B
   STA   (FP),Y
   LDY   #$25
   LDA   (FP),Y
   LDY   #$2C
   STA   (FP),Y
   LDY   #$26
   LDA   (FP),Y
   AND   #$01
   LDY   #$2D
   STA   (FP),Y
   LDY   #$27
   LDA   (FP),Y
   LDY   #$2E
   STA   (FP),Y
   LDY   #$28
   LDA   (FP),Y
   LDY   #$2F
   STA   (FP),Y
   LDY   #$29
   LDA   (FP),Y
   LDY   #$30
   STA   (FP),Y
   LDY   #$2A
   LDA   (FP),Y
   LDY   #$31
   STA   (FP),Y
   LDY   #$2B
   LDA   (FP),Y
   LDY   #$32
   STA   (FP),Y
   LDY   #$2C
   LDA   (FP),Y
   LDY   #$33
   STA   (FP),Y
   LDY   #$2D
   LDA   (FP),Y
   EOR   #$01
   LDY   #$34
   STA   (FP),Y
   LDY   #$2E
   LDA   (FP),Y
   LDY   #$35
   STA   (FP),Y
   LDY   #$2F
   LDA   (FP),Y
   LDY   #$36
   STA   (FP),Y
   LDY   #$30
   LDA   (FP),Y
   LDY   #$37
   STA   (FP),Y
   LDY   #$31
   LDA   (FP),Y
   LDY   #$38
   STA   (FP),Y
   LDY   #$32
   LDA   (FP),Y
   LDY   #$39
   STA   (FP),Y
   LDY   #$33
   LDA   (FP),Y
   LDY   #$3A
   STA   (FP),Y
   LDY   #$34
   LDA   (FP),Y
   SEC
   SBC   #$01
   LDY   #$3B
   STA   (FP),Y
   LDY   #$35
   LDA   (FP),Y
   STA   g0
   INY
   LDA   (FP),Y
   STA   g0+1
   INY
   LDA   (FP),Y
   STA   g0+2
   INY
   LDA   (FP),Y
   STA   g0+3
   INY
   LDA   (FP),Y
   STA   g0+4
   INY
   LDA   (FP),Y
   STA   g0+5
   INY
   LDA   (FP),Y
   STA   g0+6
   LDY   #$05
   LDA   (FP),Y
   LDY   #$3C
   STA   (FP),Y
   LDY   #$06
   LDA   (FP),Y
   LDY   #$3D
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$3E
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$3F
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
.loop@0_start:
   LDY   #$40
   LDA   (FP),Y
   SEC
   SBC   #$03
   INY
   LDA   (FP),Y
   SBC   #$00
   BVC   .cmp_novf@48
   EOR   #$80
.cmp_novf@48:
   ASL
   LDA   #$00
   ROL
   LDY   #$42
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@1
   JMP   .loop@0_break
.lb_skip@1:
   CLC
   LDA   FP
   ADC   #$3E
   LDY   #$44
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$02
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   DEY
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$45
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   JSR   f1
   LDY   #$46
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   LDY   #$40
   ADC   (FP),Y
   LDY   #$49
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$41
   ADC   (FP),Y
   LDY   #$4A
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$4B
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$4C
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@49
   LDA   #$00
   JMP   .sx_done@50
.sx_neg@49:
   LDA   #$FF
.sx_done@50:
   LDY   #$4D
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$05
   LDA   (FP),Y
   CLC
   LDY   #$4B
   ADC   (FP),Y
   LDY   #$4F
   STA   (FP),Y
   LDY   #$06
   LDA   (FP),Y
   LDY   #$4C
   ADC   (FP),Y
   LDY   #$50
   STA   (FP),Y
   LDY   #$07
   LDA   (FP),Y
   LDY   #$4D
   ADC   (FP),Y
   LDY   #$51
   STA   (FP),Y
   LDY   #$08
   LDA   (FP),Y
   LDY   #$4E
   ADC   (FP),Y
   LDY   #$52
   STA   (FP),Y
   LDY   #$4F
   LDA   (FP),Y
   LDY   #$05
   STA   (FP),Y
   LDY   #$50
   LDA   (FP),Y
   LDY   #$06
   STA   (FP),Y
   LDY   #$51
   LDA   (FP),Y
   LDY   #$07
   STA   (FP),Y
   LDY   #$52
   LDA   (FP),Y
   LDY   #$08
   STA   (FP),Y
   LDY   #$40
   LDA   (FP),Y
   LDY   #$53
   STA   (FP),Y
   LDY   #$41
   LDA   (FP),Y
   LDY   #$54
   STA   (FP),Y
   LDY   #$40
   LDA   (FP),Y
   CLC
   ADC   #$01
   LDY   #$55
   STA   (FP),Y
   LDY   #$41
   LDA   (FP),Y
   ADC   #$00
   LDY   #$56
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$40
   STA   (FP),Y
   LDY   #$56
   LDA   (FP),Y
   LDY   #$41
   STA   (FP),Y
   JMP   .loop@0_start
.loop@0_break:
   LDY   #$3E
   LDA   (FP),Y
   LDY   #$57
   STA   (FP),Y
   LDY   #$3F
   LDA   (FP),Y
   LDY   #$58
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@51
   LDA   #$00
   JMP   .sx_done@52
.sx_neg@51:
   LDA   #$FF
.sx_done@52:
   LDY   #$59
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$05
   LDA   (FP),Y
   CLC
   LDY   #$57
   ADC   (FP),Y
   LDY   #$5B
   STA   (FP),Y
   LDY   #$06
   LDA   (FP),Y
   LDY   #$58
   ADC   (FP),Y
   LDY   #$5C
   STA   (FP),Y
   LDY   #$07
   LDA   (FP),Y
   LDY   #$59
   ADC   (FP),Y
   LDY   #$5D
   STA   (FP),Y
   LDY   #$08
   LDA   (FP),Y
   LDY   #$5A
   ADC   (FP),Y
   LDY   #$5E
   STA   (FP),Y
   LDY   #$5B
   LDA   (FP),Y
   LDY   #$05
   STA   (FP),Y
   LDY   #$5C
   LDA   (FP),Y
   LDY   #$06
   STA   (FP),Y
   LDY   #$5D
   LDA   (FP),Y
   LDY   #$07
   STA   (FP),Y
   LDY   #$5E
   LDA   (FP),Y
   LDY   #$08
   STA   (FP),Y
   LDA   g1
   LDY   #$5F
   STA   (FP),Y
   LDA   g1
   BMI   .sx_neg@53
   LDA   #$00
   JMP   .sx_done@54
.sx_neg@53:
   LDA   #$FF
.sx_done@54:
   LDY   #$60
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$5F
   LDA   (FP),Y
   CLC
   LDY   #$05
   ADC   (FP),Y
   LDY   #$63
   STA   (FP),Y
   LDY   #$60
   LDA   (FP),Y
   LDY   #$06
   ADC   (FP),Y
   LDY   #$64
   STA   (FP),Y
   LDY   #$61
   LDA   (FP),Y
   LDY   #$07
   ADC   (FP),Y
   LDY   #$65
   STA   (FP),Y
   LDY   #$62
   LDA   (FP),Y
   LDY   #$08
   ADC   (FP),Y
   LDY   #$66
   STA   (FP),Y
   LDY   #$63
   LDA   (FP),Y
   LDY   #$67
   STA   (FP),Y
   STA   g1
   LDA   g0
   AND   #$07
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
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$05
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
   LDY   #$68
   LDA   (FP),Y
   STA   HARGS+4
   JSR   asl32
   LDA   HARGS+5
   LDY   #$6F
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
   LDY   #$73
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@55
   LDA   #$00
   JMP   .sx_done@56
.sx_neg@55:
   LDA   #$FF
.sx_done@56:
   LDY   #$74
   STA   (FP),Y
   SEC
   LDA   #$00
   DEY
   SBC   (FP),Y
   LDY   #$75
   STA   (FP),Y
   LDA   #$00
   DEY
   SBC   (FP),Y
   LDY   #$76
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$77
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$78
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@57
   LDA   #$00
   JMP   .sx_done@58
.sx_neg@57:
   LDA   #$FF
.sx_done@58:
   LDY   #$79
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$6F
   LDA   (FP),Y
   SEC
   LDY   #$77
   SBC   (FP),Y
   LDY   #$70
   LDA   (FP),Y
   LDY   #$78
   SBC   (FP),Y
   LDY   #$71
   LDA   (FP),Y
   LDY   #$79
   SBC   (FP),Y
   LDY   #$72
   LDA   (FP),Y
   LDY   #$7A
   SBC   (FP),Y
   BVC   .cmp_novf@59
   EOR   #$80
.cmp_novf@59:
   ASL
   LDA   #$00
   ROL
   EOR   #$01
   LDY   #$7B
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$7D
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$7E
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@60
   LDA   #$00
   JMP   .sx_done@61
.sx_neg@60:
   LDA   #$FF
.sx_done@61:
   LDY   #$7F
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$7D
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

   ; epilogue
   CLC
   LDA   FP
   ADC   #$83
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   INY
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   RTS

main:
   SUBROUTINE

   ; prologue: 0 arg bytes, 35 local bytes
   SEC
   LDA   SSP
   SBC   #$25
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$24
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   LDA   #$D1
   LDY   #$01
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$01
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   (FP),Y
   STA   (SSP),Y
   JSR   f0
   LDA   HARGS+8
   LDY   #$02
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
   LDY   #$02
   LDA   (FP),Y
   LDY   #$06
   STA   (FP),Y
   LDY   #$03
   LDA   (FP),Y
   LDY   #$07
   STA   (FP),Y
   LDY   #$04
   LDA   (FP),Y
   LDY   #$08
   STA   (FP),Y
   LDY   #$05
   LDA   (FP),Y
   LDY   #$09
   STA   (FP),Y
   LDA   g0
   INY
   STA   (FP),Y
   LDA   g0+1
   INY
   STA   (FP),Y
   LDA   g0+2
   INY
   STA   (FP),Y
   LDA   g0+3
   INY
   STA   (FP),Y
   LDA   g1
   INY
   STA   (FP),Y
   LDA   g1
   BMI   .sx_neg@62
   LDA   #$00
   JMP   .sx_done@63
.sx_neg@62:
   LDA   #$FF
.sx_done@63:
   LDY   #$0F
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$0A
   LDA   (FP),Y
   LDY   #$0E
   EOR   (FP),Y
   LDY   #$12
   STA   (FP),Y
   LDY   #$0B
   LDA   (FP),Y
   LDY   #$0F
   EOR   (FP),Y
   LDY   #$13
   STA   (FP),Y
   LDY   #$0C
   LDA   (FP),Y
   LDY   #$10
   EOR   (FP),Y
   LDY   #$14
   STA   (FP),Y
   LDY   #$0D
   LDA   (FP),Y
   LDY   #$11
   EOR   (FP),Y
   LDY   #$15
   STA   (FP),Y
   LDY   #$06
   LDA   (FP),Y
   LDY   #$12
   EOR   (FP),Y
   LDY   #$16
   STA   (FP),Y
   LDY   #$07
   LDA   (FP),Y
   LDY   #$13
   EOR   (FP),Y
   LDY   #$17
   STA   (FP),Y
   LDY   #$08
   LDA   (FP),Y
   LDY   #$14
   EOR   (FP),Y
   LDY   #$18
   STA   (FP),Y
   LDY   #$09
   LDA   (FP),Y
   LDY   #$15
   EOR   (FP),Y
   LDY   #$19
   STA   (FP),Y
   LDY   #$16
   LDA   (FP),Y
   LDY   #$06
   STA   (FP),Y
   LDY   #$17
   LDA   (FP),Y
   LDY   #$07
   STA   (FP),Y
   LDY   #$18
   LDA   (FP),Y
   LDY   #$08
   STA   (FP),Y
   LDY   #$19
   LDA   (FP),Y
   LDY   #$09
   STA   (FP),Y
   LDY   #$06
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
   LDA   #$10
   STA   HARGS+4
   JSR   asr32
   LDA   HARGS+5
   LDY   #$1A
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
   LDY   #$06
   LDA   (FP),Y
   LDY   #$1A
   EOR   (FP),Y
   LDY   #$1E
   STA   (FP),Y
   LDY   #$07
   LDA   (FP),Y
   LDY   #$1B
   EOR   (FP),Y
   LDY   #$1F
   STA   (FP),Y
   LDY   #$08
   LDA   (FP),Y
   LDY   #$1C
   EOR   (FP),Y
   LDY   #$20
   STA   (FP),Y
   LDY   #$09
   LDA   (FP),Y
   LDY   #$1D
   EOR   (FP),Y
   LDY   #$21
   STA   (FP),Y
   LDY   #$1E
   LDA   (FP),Y
   LDY   #$22
   STA   (FP),Y
   LDY   #$1F
   LDA   (FP),Y
   LDY   #$23
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
   ADC   #$25
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$24
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

g0:
   DC.B  $93
   DS.B  6

g1:
   DC.B  $34
