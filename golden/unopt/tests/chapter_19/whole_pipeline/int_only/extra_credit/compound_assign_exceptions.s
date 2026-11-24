SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24

main:
   SUBROUTINE

   ; prologue: 0 arg bytes, 216 local bytes
   SEC
   LDA   SSP
   SBC   #$DA
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$D9
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
   LDA   #$00
   INY
   STA   (FP),Y
   ORA   #$00
   BPL   .sx_done@1
   LDA   #$FF
.sx_done@1:
   LDY   #$03
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
   LDY   #$01
   LDA   (FP),Y
   LDY   #$09
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   LDY   #$0A
   STA   (FP),Y
   LDY   #$03
   LDA   (FP),Y
   LDY   #$0B
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
   LDA   #$0A
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   ORA   #$00
   BPL   .sx_done@3
   LDA   #$FF
.sx_done@3:
   LDY   #$13
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
   LDY   #$11
   LDA   (FP),Y
   LDY   #$19
   STA   (FP),Y
   LDY   #$12
   LDA   (FP),Y
   LDY   #$1A
   STA   (FP),Y
   LDY   #$13
   LDA   (FP),Y
   LDY   #$1B
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
   LDA   #$FF
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$7F
   INY
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@4
   LDA   #$00
   JMP   .sx_done@5
.sx_neg@4:
   LDA   #$FF
.sx_done@5:
   LDY   #$25
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$21
   LDA   (FP),Y
   LDY   #$29
   STA   (FP),Y
   LDY   #$22
   LDA   (FP),Y
   LDY   #$2A
   STA   (FP),Y
   LDY   #$23
   LDA   (FP),Y
   LDY   #$2B
   STA   (FP),Y
   LDY   #$24
   LDA   (FP),Y
   LDY   #$2C
   STA   (FP),Y
   LDY   #$25
   LDA   (FP),Y
   LDY   #$2D
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   SEC
   LDA   #$00
   SBC   #$FF
   INY
   STA   (FP),Y
   LDA   #$00
   SBC   #$FF
   INY
   STA   (FP),Y
   LDA   #$00
   SBC   #$FF
   INY
   STA   (FP),Y
   LDA   #$00
   SBC   #$7F
   INY
   STA   (FP),Y
   LDY   #$31
   LDA   (FP),Y
   LDY   #$35
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
   ORA   #$00
   BMI   .sx_neg@6
   LDA   #$00
   JMP   .sx_done@7
.sx_neg@6:
   LDA   #$FF
.sx_done@7:
   LDY   #$39
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$35
   LDA   (FP),Y
   LDY   #$3D
   STA   (FP),Y
   LDY   #$36
   LDA   (FP),Y
   LDY   #$3E
   STA   (FP),Y
   LDY   #$37
   LDA   (FP),Y
   LDY   #$3F
   STA   (FP),Y
   LDY   #$38
   LDA   (FP),Y
   LDY   #$40
   STA   (FP),Y
   LDY   #$39
   LDA   (FP),Y
   LDY   #$41
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   zero
   ORA   zero+1
   ORA   zero+2
   ORA   zero+3
   ORA   zero+4
   ORA   zero+5
   ORA   zero+6
   ORA   zero+7
   BNE   .lb_skip@0
   JMP   .if_end@0
.lb_skip@0:
   LDA   #$00
   LDY   #$45
   STA   (FP),Y
   INY
   STA   (FP),Y
   ORA   #$00
   BPL   .sx_done@9
   LDA   #$FF
.sx_done@9:
   LDY   #$47
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
   LDY   #$09
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
   INY
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
   LDY   #$45
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
   INY
   LDA   (FP),Y
   STA   HARGS+12
   INY
   LDA   (FP),Y
   STA   HARGS+13
   INY
   LDA   (FP),Y
   STA   HARGS+14
   INY
   LDA   (FP),Y
   STA   HARGS+15
   JSR   sdivmod64
   LDA   HARGS+24
   LDY   #$4D
   STA   (FP),Y
   LDA   HARGS+25
   INY
   STA   (FP),Y
   LDA   HARGS+26
   INY
   STA   (FP),Y
   LDA   HARGS+27
   INY
   STA   (FP),Y
   LDA   HARGS+28
   INY
   STA   (FP),Y
   LDA   HARGS+29
   INY
   STA   (FP),Y
   LDA   HARGS+30
   INY
   STA   (FP),Y
   LDA   HARGS+31
   INY
   STA   (FP),Y
   LDY   #$4D
   LDA   (FP),Y
   LDY   #$09
   STA   (FP),Y
   LDY   #$4E
   LDA   (FP),Y
   LDY   #$0A
   STA   (FP),Y
   LDY   #$4F
   LDA   (FP),Y
   LDY   #$0B
   STA   (FP),Y
   LDY   #$50
   LDA   (FP),Y
   LDY   #$0C
   STA   (FP),Y
   LDY   #$51
   LDA   (FP),Y
   LDY   #$0D
   STA   (FP),Y
   LDY   #$52
   LDA   (FP),Y
   LDY   #$0E
   STA   (FP),Y
   LDY   #$53
   LDA   (FP),Y
   LDY   #$0F
   STA   (FP),Y
   LDY   #$54
   LDA   (FP),Y
   LDY   #$10
   STA   (FP),Y
   LDA   #$00
   LDY   #$55
   STA   (FP),Y
   INY
   STA   (FP),Y
   ORA   #$00
   BPL   .sx_done@11
   LDA   #$FF
.sx_done@11:
   LDY   #$57
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
   LDY   #$19
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
   INY
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
   LDY   #$55
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
   INY
   LDA   (FP),Y
   STA   HARGS+12
   INY
   LDA   (FP),Y
   STA   HARGS+13
   INY
   LDA   (FP),Y
   STA   HARGS+14
   INY
   LDA   (FP),Y
   STA   HARGS+15
   JSR   sdivmod64
   LDA   HARGS+16
   LDY   #$5D
   STA   (FP),Y
   LDA   HARGS+17
   INY
   STA   (FP),Y
   LDA   HARGS+18
   INY
   STA   (FP),Y
   LDA   HARGS+19
   INY
   STA   (FP),Y
   LDA   HARGS+20
   INY
   STA   (FP),Y
   LDA   HARGS+21
   INY
   STA   (FP),Y
   LDA   HARGS+22
   INY
   STA   (FP),Y
   LDA   HARGS+23
   INY
   STA   (FP),Y
   LDY   #$5D
   LDA   (FP),Y
   LDY   #$19
   STA   (FP),Y
   LDY   #$5E
   LDA   (FP),Y
   LDY   #$1A
   STA   (FP),Y
   LDY   #$5F
   LDA   (FP),Y
   LDY   #$1B
   STA   (FP),Y
   LDY   #$60
   LDA   (FP),Y
   LDY   #$1C
   STA   (FP),Y
   LDY   #$61
   LDA   (FP),Y
   LDY   #$1D
   STA   (FP),Y
   LDY   #$62
   LDA   (FP),Y
   LDY   #$1E
   STA   (FP),Y
   LDY   #$63
   LDA   (FP),Y
   LDY   #$1F
   STA   (FP),Y
   LDY   #$64
   LDA   (FP),Y
   LDY   #$20
   STA   (FP),Y
   LDA   #$0A
   LDY   #$65
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   ORA   #$00
   BPL   .sx_done@13
   LDA   #$FF
.sx_done@13:
   LDY   #$67
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
   LDY   #$29
   LDA   (FP),Y
   CLC
   LDY   #$65
   ADC   (FP),Y
   LDY   #$6D
   STA   (FP),Y
   LDY   #$2A
   LDA   (FP),Y
   LDY   #$66
   ADC   (FP),Y
   LDY   #$6E
   STA   (FP),Y
   LDY   #$2B
   LDA   (FP),Y
   LDY   #$67
   ADC   (FP),Y
   LDY   #$6F
   STA   (FP),Y
   LDY   #$2C
   LDA   (FP),Y
   LDY   #$68
   ADC   (FP),Y
   LDY   #$70
   STA   (FP),Y
   LDY   #$2D
   LDA   (FP),Y
   LDY   #$69
   ADC   (FP),Y
   LDY   #$71
   STA   (FP),Y
   LDY   #$2E
   LDA   (FP),Y
   LDY   #$6A
   ADC   (FP),Y
   LDY   #$72
   STA   (FP),Y
   LDY   #$2F
   LDA   (FP),Y
   LDY   #$6B
   ADC   (FP),Y
   LDY   #$73
   STA   (FP),Y
   LDY   #$30
   LDA   (FP),Y
   LDY   #$6C
   ADC   (FP),Y
   LDY   #$74
   STA   (FP),Y
   LDY   #$6D
   LDA   (FP),Y
   LDY   #$29
   STA   (FP),Y
   LDY   #$6E
   LDA   (FP),Y
   LDY   #$2A
   STA   (FP),Y
   LDY   #$6F
   LDA   (FP),Y
   LDY   #$2B
   STA   (FP),Y
   LDY   #$70
   LDA   (FP),Y
   LDY   #$2C
   STA   (FP),Y
   LDY   #$71
   LDA   (FP),Y
   LDY   #$2D
   STA   (FP),Y
   LDY   #$72
   LDA   (FP),Y
   LDY   #$2E
   STA   (FP),Y
   LDY   #$73
   LDA   (FP),Y
   LDY   #$2F
   STA   (FP),Y
   LDY   #$74
   LDA   (FP),Y
   LDY   #$30
   STA   (FP),Y
   LDA   #$0A
   LDY   #$75
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   ORA   #$00
   BPL   .sx_done@15
   LDA   #$FF
.sx_done@15:
   LDY   #$77
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
   LDY   #$3D
   LDA   (FP),Y
   SEC
   LDY   #$75
   SBC   (FP),Y
   LDY   #$7D
   STA   (FP),Y
   LDY   #$3E
   LDA   (FP),Y
   LDY   #$76
   SBC   (FP),Y
   LDY   #$7E
   STA   (FP),Y
   LDY   #$3F
   LDA   (FP),Y
   LDY   #$77
   SBC   (FP),Y
   LDY   #$7F
   STA   (FP),Y
   LDY   #$40
   LDA   (FP),Y
   LDY   #$78
   SBC   (FP),Y
   LDY   #$80
   STA   (FP),Y
   LDY   #$41
   LDA   (FP),Y
   LDY   #$79
   SBC   (FP),Y
   LDY   #$81
   STA   (FP),Y
   LDY   #$42
   LDA   (FP),Y
   LDY   #$7A
   SBC   (FP),Y
   LDY   #$82
   STA   (FP),Y
   LDY   #$43
   LDA   (FP),Y
   LDY   #$7B
   SBC   (FP),Y
   LDY   #$83
   STA   (FP),Y
   LDY   #$44
   LDA   (FP),Y
   LDY   #$7C
   SBC   (FP),Y
   LDY   #$84
   STA   (FP),Y
   LDY   #$7D
   LDA   (FP),Y
   LDY   #$3D
   STA   (FP),Y
   LDY   #$7E
   LDA   (FP),Y
   LDY   #$3E
   STA   (FP),Y
   LDY   #$7F
   LDA   (FP),Y
   LDY   #$3F
   STA   (FP),Y
   LDY   #$80
   LDA   (FP),Y
   LDY   #$40
   STA   (FP),Y
   LDY   #$81
   LDA   (FP),Y
   LDY   #$41
   STA   (FP),Y
   LDY   #$82
   LDA   (FP),Y
   LDY   #$42
   STA   (FP),Y
   LDY   #$83
   LDA   (FP),Y
   LDY   #$43
   STA   (FP),Y
   LDY   #$84
   LDA   (FP),Y
   LDY   #$44
   STA   (FP),Y
.if_end@0:
   LDA   #$03
   LDY   #$85
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   ORA   #$00
   BPL   .sx_done@17
   LDA   #$FF
.sx_done@17:
   LDY   #$87
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
   LDY   #$10
   LDA   (FP),Y
   LDY   #$8C
   CMP   (FP),Y
   BNE   .cmp_true@18
   LDY   #$0F
   LDA   (FP),Y
   LDY   #$8B
   CMP   (FP),Y
   BNE   .cmp_true@18
   LDY   #$0E
   LDA   (FP),Y
   LDY   #$8A
   CMP   (FP),Y
   BNE   .cmp_true@18
   LDY   #$0D
   LDA   (FP),Y
   LDY   #$89
   CMP   (FP),Y
   BNE   .cmp_true@18
   LDY   #$0C
   LDA   (FP),Y
   LDY   #$88
   CMP   (FP),Y
   BNE   .cmp_true@18
   LDY   #$0B
   LDA   (FP),Y
   LDY   #$87
   CMP   (FP),Y
   BNE   .cmp_true@18
   LDY   #$0A
   LDA   (FP),Y
   LDY   #$86
   CMP   (FP),Y
   BNE   .cmp_true@18
   LDY   #$09
   LDA   (FP),Y
   LDY   #$85
   CMP   (FP),Y
   BNE   .cmp_true@18
   LDA   #$00
   JMP   .cmp_end@19
.cmp_true@18:
   LDA   #$01
.cmp_end@19:
   LDY   #$8D
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@1
   LDA   #$01
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   ORA   #$00
   BPL   .sx_done@22
   LDA   #$FF
.sx_done@22:
   LDY   #$91
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
   LDY   #$8F
   LDA   (FP),Y
   STA   HARGS+16
   INY
   LDA   (FP),Y
   STA   HARGS+17
   INY
   LDA   (FP),Y
   STA   HARGS+18
   INY
   LDA   (FP),Y
   STA   HARGS+19
   INY
   LDA   (FP),Y
   STA   HARGS+20
   INY
   LDA   (FP),Y
   STA   HARGS+21
   INY
   LDA   (FP),Y
   STA   HARGS+22
   INY
   LDA   (FP),Y
   STA   HARGS+23

   ; epilogue
   CLC
   LDA   FP
   ADC   #$DA
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$D9
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   RTS
.if_end@1:
   LDA   #$0A
   LDY   #$97
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   ORA   #$00
   BPL   .sx_done@24
   LDA   #$FF
.sx_done@24:
   LDY   #$99
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
   LDY   #$20
   LDA   (FP),Y
   LDY   #$9E
   CMP   (FP),Y
   BNE   .cmp_true@25
   LDY   #$1F
   LDA   (FP),Y
   LDY   #$9D
   CMP   (FP),Y
   BNE   .cmp_true@25
   LDY   #$1E
   LDA   (FP),Y
   LDY   #$9C
   CMP   (FP),Y
   BNE   .cmp_true@25
   LDY   #$1D
   LDA   (FP),Y
   LDY   #$9B
   CMP   (FP),Y
   BNE   .cmp_true@25
   LDY   #$1C
   LDA   (FP),Y
   LDY   #$9A
   CMP   (FP),Y
   BNE   .cmp_true@25
   LDY   #$1B
   LDA   (FP),Y
   LDY   #$99
   CMP   (FP),Y
   BNE   .cmp_true@25
   LDY   #$1A
   LDA   (FP),Y
   LDY   #$98
   CMP   (FP),Y
   BNE   .cmp_true@25
   LDY   #$19
   LDA   (FP),Y
   LDY   #$97
   CMP   (FP),Y
   BNE   .cmp_true@25
   LDA   #$00
   JMP   .cmp_end@26
.cmp_true@25:
   LDA   #$01
.cmp_end@26:
   LDY   #$9F
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@2
   LDA   #$02
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   ORA   #$00
   BPL   .sx_done@29
   LDA   #$FF
.sx_done@29:
   LDY   #$A3
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
   LDY   #$A1
   LDA   (FP),Y
   STA   HARGS+16
   INY
   LDA   (FP),Y
   STA   HARGS+17
   INY
   LDA   (FP),Y
   STA   HARGS+18
   INY
   LDA   (FP),Y
   STA   HARGS+19
   INY
   LDA   (FP),Y
   STA   HARGS+20
   INY
   LDA   (FP),Y
   STA   HARGS+21
   INY
   LDA   (FP),Y
   STA   HARGS+22
   INY
   LDA   (FP),Y
   STA   HARGS+23

   ; epilogue
   CLC
   LDA   FP
   ADC   #$DA
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$D9
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   RTS
.if_end@2:
   LDA   #$FF
   LDY   #$A9
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$7F
   INY
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@30
   LDA   #$00
   JMP   .sx_done@31
.sx_neg@30:
   LDA   #$FF
.sx_done@31:
   LDY   #$AD
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$30
   LDA   (FP),Y
   LDY   #$B0
   CMP   (FP),Y
   BNE   .cmp_true@32
   LDY   #$2F
   LDA   (FP),Y
   LDY   #$AF
   CMP   (FP),Y
   BNE   .cmp_true@32
   LDY   #$2E
   LDA   (FP),Y
   LDY   #$AE
   CMP   (FP),Y
   BNE   .cmp_true@32
   LDY   #$2D
   LDA   (FP),Y
   LDY   #$AD
   CMP   (FP),Y
   BNE   .cmp_true@32
   LDY   #$2C
   LDA   (FP),Y
   LDY   #$AC
   CMP   (FP),Y
   BNE   .cmp_true@32
   LDY   #$2B
   LDA   (FP),Y
   LDY   #$AB
   CMP   (FP),Y
   BNE   .cmp_true@32
   LDY   #$2A
   LDA   (FP),Y
   LDY   #$AA
   CMP   (FP),Y
   BNE   .cmp_true@32
   LDY   #$29
   LDA   (FP),Y
   LDY   #$A9
   CMP   (FP),Y
   BNE   .cmp_true@32
   LDA   #$00
   JMP   .cmp_end@33
.cmp_true@32:
   LDA   #$01
.cmp_end@33:
   LDY   #$B1
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@3
   LDA   #$03
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   ORA   #$00
   BPL   .sx_done@36
   LDA   #$FF
.sx_done@36:
   LDY   #$B5
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
   LDY   #$B3
   LDA   (FP),Y
   STA   HARGS+16
   INY
   LDA   (FP),Y
   STA   HARGS+17
   INY
   LDA   (FP),Y
   STA   HARGS+18
   INY
   LDA   (FP),Y
   STA   HARGS+19
   INY
   LDA   (FP),Y
   STA   HARGS+20
   INY
   LDA   (FP),Y
   STA   HARGS+21
   INY
   LDA   (FP),Y
   STA   HARGS+22
   INY
   LDA   (FP),Y
   STA   HARGS+23

   ; epilogue
   CLC
   LDA   FP
   ADC   #$DA
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$D9
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   RTS
.if_end@3:
   SEC
   LDA   #$00
   SBC   #$FF
   LDY   #$BB
   STA   (FP),Y
   LDA   #$00
   SBC   #$FF
   INY
   STA   (FP),Y
   LDA   #$00
   SBC   #$FF
   INY
   STA   (FP),Y
   LDA   #$00
   SBC   #$7F
   INY
   STA   (FP),Y
   LDY   #$BB
   LDA   (FP),Y
   LDY   #$BF
   STA   (FP),Y
   LDY   #$BC
   LDA   (FP),Y
   LDY   #$C0
   STA   (FP),Y
   LDY   #$BD
   LDA   (FP),Y
   LDY   #$C1
   STA   (FP),Y
   LDY   #$BE
   LDA   (FP),Y
   LDY   #$C2
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@37
   LDA   #$00
   JMP   .sx_done@38
.sx_neg@37:
   LDA   #$FF
.sx_done@38:
   LDY   #$C3
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$44
   LDA   (FP),Y
   LDY   #$C6
   CMP   (FP),Y
   BNE   .cmp_true@39
   LDY   #$43
   LDA   (FP),Y
   LDY   #$C5
   CMP   (FP),Y
   BNE   .cmp_true@39
   LDY   #$42
   LDA   (FP),Y
   LDY   #$C4
   CMP   (FP),Y
   BNE   .cmp_true@39
   LDY   #$41
   LDA   (FP),Y
   LDY   #$C3
   CMP   (FP),Y
   BNE   .cmp_true@39
   LDY   #$40
   LDA   (FP),Y
   LDY   #$C2
   CMP   (FP),Y
   BNE   .cmp_true@39
   LDY   #$3F
   LDA   (FP),Y
   LDY   #$C1
   CMP   (FP),Y
   BNE   .cmp_true@39
   LDY   #$3E
   LDA   (FP),Y
   LDY   #$C0
   CMP   (FP),Y
   BNE   .cmp_true@39
   LDY   #$3D
   LDA   (FP),Y
   LDY   #$BF
   CMP   (FP),Y
   BNE   .cmp_true@39
   LDA   #$00
   JMP   .cmp_end@40
.cmp_true@39:
   LDA   #$01
.cmp_end@40:
   LDY   #$C7
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@4
   LDA   #$04
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   ORA   #$00
   BPL   .sx_done@43
   LDA   #$FF
.sx_done@43:
   LDY   #$CB
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
   LDY   #$C9
   LDA   (FP),Y
   STA   HARGS+16
   INY
   LDA   (FP),Y
   STA   HARGS+17
   INY
   LDA   (FP),Y
   STA   HARGS+18
   INY
   LDA   (FP),Y
   STA   HARGS+19
   INY
   LDA   (FP),Y
   STA   HARGS+20
   INY
   LDA   (FP),Y
   STA   HARGS+21
   INY
   LDA   (FP),Y
   STA   HARGS+22
   INY
   LDA   (FP),Y
   STA   HARGS+23

   ; epilogue
   CLC
   LDA   FP
   ADC   #$DA
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$D9
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   RTS
.if_end@4:
   LDA   #$00
   LDY   #$D1
   STA   (FP),Y
   INY
   STA   (FP),Y
   ORA   #$00
   BPL   .sx_done@45
   LDA   #$FF
.sx_done@45:
   LDY   #$D3
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
   LDY   #$D1
   LDA   (FP),Y
   STA   HARGS+16
   INY
   LDA   (FP),Y
   STA   HARGS+17
   INY
   LDA   (FP),Y
   STA   HARGS+18
   INY
   LDA   (FP),Y
   STA   HARGS+19
   INY
   LDA   (FP),Y
   STA   HARGS+20
   INY
   LDA   (FP),Y
   STA   HARGS+21
   INY
   LDA   (FP),Y
   STA   HARGS+22
   INY
   LDA   (FP),Y
   STA   HARGS+23

   ; epilogue
   CLC
   LDA   FP
   ADC   #$DA
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

zero:
   DS.B  8
