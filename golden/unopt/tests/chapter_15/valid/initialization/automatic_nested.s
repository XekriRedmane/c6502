SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24

test_simple:
   SUBROUTINE

   ; prologue: 0 arg bytes, 72 local bytes
   SEC
   LDA   SSP
   SBC   #$4A
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$49
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   CLC
   LDA   FP
   ADC   #$01
   LDY   #$13
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDA   #$01
   LDY   #$00
   STA   (DPTR),Y
   LDA   #$00
   INY
   STA   (DPTR),Y
   LDY   #$13
   LDA   (FP),Y
   CLC
   ADC   #$02
   LDY   #$15
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$16
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDA   #$02
   LDY   #$00
   STA   (DPTR),Y
   LDA   #$00
   INY
   STA   (DPTR),Y
   LDY   #$13
   LDA   (FP),Y
   CLC
   ADC   #$04
   LDY   #$17
   STA   (FP),Y
   LDY   #$14
   LDA   (FP),Y
   ADC   #$00
   LDY   #$18
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDA   #$03
   LDY   #$00
   STA   (DPTR),Y
   LDA   #$00
   INY
   STA   (DPTR),Y
   LDY   #$13
   LDA   (FP),Y
   CLC
   ADC   #$06
   LDY   #$19
   STA   (FP),Y
   LDY   #$14
   LDA   (FP),Y
   ADC   #$00
   LDY   #$1A
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDA   #$04
   LDY   #$00
   STA   (DPTR),Y
   LDA   #$00
   INY
   STA   (DPTR),Y
   LDY   #$13
   LDA   (FP),Y
   CLC
   ADC   #$08
   LDY   #$1B
   STA   (FP),Y
   LDY   #$14
   LDA   (FP),Y
   ADC   #$00
   LDY   #$1C
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDA   #$05
   LDY   #$00
   STA   (DPTR),Y
   LDA   #$00
   INY
   STA   (DPTR),Y
   LDY   #$13
   LDA   (FP),Y
   CLC
   ADC   #$0A
   LDY   #$1D
   STA   (FP),Y
   LDY   #$14
   LDA   (FP),Y
   ADC   #$00
   LDY   #$1E
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDA   #$06
   LDY   #$00
   STA   (DPTR),Y
   LDA   #$00
   INY
   STA   (DPTR),Y
   LDY   #$13
   LDA   (FP),Y
   CLC
   ADC   #$0C
   LDY   #$1F
   STA   (FP),Y
   LDY   #$14
   LDA   (FP),Y
   ADC   #$00
   LDY   #$20
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDA   #$07
   LDY   #$00
   STA   (DPTR),Y
   LDA   #$00
   INY
   STA   (DPTR),Y
   LDY   #$13
   LDA   (FP),Y
   CLC
   ADC   #$0E
   LDY   #$21
   STA   (FP),Y
   LDY   #$14
   LDA   (FP),Y
   ADC   #$00
   LDY   #$22
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDA   #$08
   LDY   #$00
   STA   (DPTR),Y
   LDA   #$00
   INY
   STA   (DPTR),Y
   LDY   #$13
   LDA   (FP),Y
   CLC
   ADC   #$10
   LDY   #$23
   STA   (FP),Y
   LDY   #$14
   LDA   (FP),Y
   ADC   #$00
   LDY   #$24
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDA   #$09
   LDY   #$00
   STA   (DPTR),Y
   LDA   #$00
   INY
   STA   (DPTR),Y
   LDY   #$25
   STA   (FP),Y
   INY
   STA   (FP),Y
.loop@0_start:
   LDY   #$25
   LDA   (FP),Y
   SEC
   SBC   #$03
   INY
   LDA   (FP),Y
   SBC   #$00
   BVC   .cmp_novf@0
   EOR   #$80
.cmp_novf@0:
   ASL
   LDA   #$00
   ROL
   LDY   #$27
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
   LDA   #$00
   LDY   #$29
   STA   (FP),Y
   INY
   STA   (FP),Y
.loop@1_start:
   LDY   #$29
   LDA   (FP),Y
   SEC
   SBC   #$03
   INY
   LDA   (FP),Y
   SBC   #$00
   BVC   .cmp_novf@1
   EOR   #$80
.cmp_novf@1:
   ASL
   LDA   #$00
   ROL
   LDY   #$2B
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@0
   JMP   .loop@1_break
.lb_skip@0:
   CLC
   LDA   FP
   ADC   #$01
   LDY   #$31
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   LDY   #$25
   LDA   (FP),Y
   LDY   #$2D
   STA   (FP),Y
   STA   HARGS
   LDA   #$06
   STA   HARGS+1
   JSR   mul8
   LDA   HARGS+2
   LDY   #$2E
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   LDA   (FP),Y
   CLC
   LDY   #$2F
   ADC   (FP),Y
   LDY   #$33
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$30
   ADC   (FP),Y
   LDY   #$34
   STA   (FP),Y
   LDY   #$29
   LDA   (FP),Y
   LDY   #$35
   STA   (FP),Y
   STA   HARGS
   LDA   #$02
   STA   HARGS+1
   JSR   mul8
   LDA   HARGS+2
   LDY   #$36
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$33
   LDA   (FP),Y
   CLC
   LDY   #$37
   ADC   (FP),Y
   LDY   #$39
   STA   (FP),Y
   LDY   #$34
   LDA   (FP),Y
   LDY   #$38
   ADC   (FP),Y
   LDY   #$3A
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$3B
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$3C
   STA   (FP),Y
   LDY   #$25
   LDA   (FP),Y
   STA   HARGS
   INY
   LDA   (FP),Y
   STA   HARGS+1
   LDA   #$03
   STA   HARGS+2
   LDA   #$00
   STA   HARGS+3
   JSR   mul16
   LDA   HARGS+4
   LDY   #$3D
   STA   (FP),Y
   LDA   HARGS+5
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   LDY   #$29
   ADC   (FP),Y
   LDY   #$3F
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$2A
   ADC   (FP),Y
   LDY   #$40
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$01
   LDY   #$41
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$42
   STA   (FP),Y
   LDY   #$3C
   LDA   (FP),Y
   LDY   #$42
   CMP   (FP),Y
   BNE   .cmp_true@2
   LDY   #$3B
   LDA   (FP),Y
   LDY   #$41
   CMP   (FP),Y
   BNE   .cmp_true@2
   LDA   #$00
   JMP   .cmp_end@3
.cmp_true@2:
   LDA   #$01
.cmp_end@3:
   LDY   #$43
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@0
   LDX   #$00
   TXA

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$4A
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$49
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
.if_end@0:
   LDY   #$29
   LDA   (FP),Y
   CLC
   ADC   #$01
   LDY   #$45
   STA   (FP),Y
   LDY   #$2A
   LDA   (FP),Y
   ADC   #$00
   LDY   #$46
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$29
   STA   (FP),Y
   LDY   #$46
   LDA   (FP),Y
   LDY   #$2A
   STA   (FP),Y
   JMP   .loop@1_start
.loop@1_break:
   LDY   #$25
   LDA   (FP),Y
   CLC
   ADC   #$01
   LDY   #$47
   STA   (FP),Y
   LDY   #$26
   LDA   (FP),Y
   ADC   #$00
   LDY   #$48
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$25
   STA   (FP),Y
   LDY   #$48
   LDA   (FP),Y
   LDY   #$26
   STA   (FP),Y
   JMP   .loop@0_start
.loop@0_break:
   LDX   #$00
   LDA   #$01

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$4A
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$49
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

test_partial:
   SUBROUTINE

   ; prologue: 0 arg bytes, 184 local bytes
   SEC
   LDA   SSP
   SBC   #$BA
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$B9
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   CLC
   LDA   FP
   ADC   #$01
   LDY   #$61
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDA   #$01
   LDY   #$00
   STA   (DPTR),Y
   LDA   #$00
   INY
   STA   (DPTR),Y
   LDY   #$61
   LDA   (FP),Y
   CLC
   ADC   #$02
   LDY   #$63
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$64
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDA   #$02
   LDY   #$00
   STA   (DPTR),Y
   LDA   #$00
   INY
   STA   (DPTR),Y
   LDY   #$61
   LDA   (FP),Y
   CLC
   ADC   #$04
   LDY   #$65
   STA   (FP),Y
   LDY   #$62
   LDA   (FP),Y
   ADC   #$00
   LDY   #$66
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDA   #$03
   LDY   #$00
   STA   (DPTR),Y
   LDA   #$00
   INY
   STA   (DPTR),Y
   LDA   #$12
   LDY   #$67
   STA   (FP),Y
.clear@1:
   SEC
   SBC   #$01
   LDY   #$67
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   CLC
   LDA   FP
   ADC   #$01
   INY
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$06
   LDY   #$6C
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$6D
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   LDY   #$68
   ADC   (FP),Y
   LDY   #$6E
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$69
   ADC   (FP),Y
   LDY   #$6F
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDA   #$00
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$67
   LDA   (FP),Y
   BNE   .clear@1
   LDY   #$61
   LDA   (FP),Y
   CLC
   ADC   #$18
   LDY   #$70
   STA   (FP),Y
   LDY   #$62
   LDA   (FP),Y
   ADC   #$00
   LDY   #$71
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDA   #$04
   LDY   #$00
   STA   (DPTR),Y
   LDA   #$00
   INY
   STA   (DPTR),Y
   LDY   #$61
   LDA   (FP),Y
   CLC
   ADC   #$1A
   LDY   #$72
   STA   (FP),Y
   LDY   #$62
   LDA   (FP),Y
   ADC   #$00
   LDY   #$73
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDA   #$05
   LDY   #$00
   STA   (DPTR),Y
   LDA   #$00
   INY
   STA   (DPTR),Y
   LDY   #$61
   LDA   (FP),Y
   CLC
   ADC   #$1C
   LDY   #$74
   STA   (FP),Y
   LDY   #$62
   LDA   (FP),Y
   ADC   #$00
   LDY   #$75
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDA   #$06
   LDY   #$00
   STA   (DPTR),Y
   LDA   #$00
   INY
   STA   (DPTR),Y
   LDA   #$42
   LDY   #$76
   STA   (FP),Y
.clear@2:
   SEC
   SBC   #$01
   LDY   #$76
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   CLC
   LDA   FP
   ADC   #$01
   INY
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$1E
   LDY   #$7B
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$7C
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   LDY   #$77
   ADC   (FP),Y
   LDY   #$7D
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$78
   ADC   (FP),Y
   LDY   #$7E
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDA   #$00
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$76
   LDA   (FP),Y
   BNE   .clear@2
   LDA   #$01
   LDY   #$7F
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
.loop@2_start:
   LDY   #$81
   LDA   (FP),Y
   SEC
   SBC   #$04
   INY
   LDA   (FP),Y
   SBC   #$00
   BVC   .cmp_novf@5
   EOR   #$80
.cmp_novf@5:
   ASL
   LDA   #$00
   ROL
   LDY   #$83
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@5
   JMP   .loop@2_break
.lb_skip@5:
   LDA   #$00
   LDY   #$85
   STA   (FP),Y
   INY
   STA   (FP),Y
.loop@3_start:
   LDY   #$85
   LDA   (FP),Y
   SEC
   SBC   #$02
   INY
   LDA   (FP),Y
   SBC   #$00
   BVC   .cmp_novf@6
   EOR   #$80
.cmp_novf@6:
   ASL
   LDA   #$00
   ROL
   LDY   #$87
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@4
   JMP   .loop@3_break
.lb_skip@4:
   LDA   #$00
   LDY   #$89
   STA   (FP),Y
   INY
   STA   (FP),Y
.loop@4_start:
   LDY   #$89
   LDA   (FP),Y
   SEC
   SBC   #$06
   INY
   LDA   (FP),Y
   SBC   #$00
   BVC   .cmp_novf@7
   EOR   #$80
.cmp_novf@7:
   ASL
   LDA   #$00
   ROL
   LDY   #$8B
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@3
   JMP   .loop@4_break
.lb_skip@3:
   CLC
   LDA   FP
   ADC   #$01
   LDY   #$91
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   LDY   #$81
   LDA   (FP),Y
   LDY   #$8D
   STA   (FP),Y
   STA   HARGS
   LDA   #$18
   STA   HARGS+1
   JSR   mul8
   LDA   HARGS+2
   LDY   #$8E
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   LDA   (FP),Y
   CLC
   LDY   #$8F
   ADC   (FP),Y
   LDY   #$93
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$90
   ADC   (FP),Y
   LDY   #$94
   STA   (FP),Y
   LDY   #$85
   LDA   (FP),Y
   LDY   #$95
   STA   (FP),Y
   STA   HARGS
   LDA   #$0C
   STA   HARGS+1
   JSR   mul8
   LDA   HARGS+2
   LDY   #$96
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$93
   LDA   (FP),Y
   CLC
   LDY   #$97
   ADC   (FP),Y
   LDY   #$99
   STA   (FP),Y
   LDY   #$94
   LDA   (FP),Y
   LDY   #$98
   ADC   (FP),Y
   LDY   #$9A
   STA   (FP),Y
   LDY   #$89
   LDA   (FP),Y
   LDY   #$9B
   STA   (FP),Y
   STA   HARGS
   LDA   #$02
   STA   HARGS+1
   JSR   mul8
   LDA   HARGS+2
   LDY   #$9C
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$99
   LDA   (FP),Y
   CLC
   LDY   #$9D
   ADC   (FP),Y
   LDY   #$9F
   STA   (FP),Y
   LDY   #$9A
   LDA   (FP),Y
   LDY   #$9E
   ADC   (FP),Y
   LDY   #$A0
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$A1
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$A2
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$A3
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$A4
   STA   (FP),Y
   LDA   #$01
   SEC
   LDY   #$81
   SBC   (FP),Y
   LDA   #$00
   INY
   SBC   (FP),Y
   BVC   .cmp_novf@8
   EOR   #$80
.cmp_novf@8:
   ASL
   LDA   #$00
   ROL
   LDY   #$A5
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@5
   LDA   #$00
   SEC
   LDY   #$85
   SBC   (FP),Y
   LDA   #$00
   INY
   SBC   (FP),Y
   BVC   .cmp_novf@9
   EOR   #$80
.cmp_novf@9:
   ASL
   LDA   #$00
   ROL
   LDY   #$A7
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@5
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   JMP   .or_end@6
.or_true@5:
   LDA   #$01
   LDY   #$A9
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@6:
   LDY   #$A9
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@3
   LDA   #$02
   SEC
   LDY   #$89
   SBC   (FP),Y
   LDA   #$00
   INY
   SBC   (FP),Y
   BVC   .cmp_novf@10
   EOR   #$80
.cmp_novf@10:
   ASL
   LDA   #$00
   ROL
   LDY   #$AB
   STA   (FP),Y
   LDA   #$00
   INY
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
   LDY   #$AD
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@4:
   LDY   #$AD
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_else@8
   LDY   #$A3
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@2
   JMP   .if_end@7
.lb_skip@2:
   LDX   #$00
   TXA

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$BA
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$B9
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
.if_else@8:
   LDY   #$A4
   LDA   (FP),Y
   LDY   #$80
   CMP   (FP),Y
   BNE   .cmp_true@11
   LDY   #$A3
   LDA   (FP),Y
   LDY   #$7F
   CMP   (FP),Y
   BNE   .cmp_true@11
   LDA   #$00
   JMP   .cmp_end@12
.cmp_true@11:
   LDA   #$01
.cmp_end@12:
   LDY   #$AF
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@10
   LDX   #$00
   TXA

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$BA
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$B9
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
.if_end@10:
   LDY   #$7F
   LDA   (FP),Y
   CLC
   ADC   #$01
   LDY   #$B1
   STA   (FP),Y
   LDY   #$80
   LDA   (FP),Y
   ADC   #$00
   LDY   #$B2
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$7F
   STA   (FP),Y
   LDY   #$B2
   LDA   (FP),Y
   LDY   #$80
   STA   (FP),Y
.if_end@7:
   LDY   #$89
   LDA   (FP),Y
   CLC
   ADC   #$01
   LDY   #$B3
   STA   (FP),Y
   LDY   #$8A
   LDA   (FP),Y
   ADC   #$00
   LDY   #$B4
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$89
   STA   (FP),Y
   LDY   #$B4
   LDA   (FP),Y
   LDY   #$8A
   STA   (FP),Y
   JMP   .loop@4_start
.loop@4_break:
   LDY   #$85
   LDA   (FP),Y
   CLC
   ADC   #$01
   LDY   #$B5
   STA   (FP),Y
   LDY   #$86
   LDA   (FP),Y
   ADC   #$00
   LDY   #$B6
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$85
   STA   (FP),Y
   LDY   #$B6
   LDA   (FP),Y
   LDY   #$86
   STA   (FP),Y
   JMP   .loop@3_start
.loop@3_break:
   LDY   #$81
   LDA   (FP),Y
   CLC
   ADC   #$01
   LDY   #$B7
   STA   (FP),Y
   LDY   #$82
   LDA   (FP),Y
   ADC   #$00
   LDY   #$B8
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$81
   STA   (FP),Y
   LDY   #$B8
   LDA   (FP),Y
   LDY   #$82
   STA   (FP),Y
   JMP   .loop@2_start
.loop@2_break:
   LDX   #$00
   LDA   #$01

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$BA
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$B9
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

test_non_constant_and_type_conversion:
   SUBROUTINE

   ; prologue: 0 arg bytes, 233 local bytes
   SEC
   LDA   SSP
   SBC   #$EB
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$EA
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   SEC
   LDA   #$00
   SBC   #$04
   LDY   #$01
   STA   (FP),Y
   LDA   #$00
   SBC   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$03
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$04
   STA   (FP),Y
   CLC
   LDA   FP
   ADC   #$03
   INY
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
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
   LDA   @9.x
   STA   HARGS
   LDA   @9.x+1
   STA   HARGS+1
   JSR   i2d
   LDA   HARGS+2
   LDY   #$09
   STA   (FP),Y
   LDA   HARGS+3
   INY
   STA   (FP),Y
   LDA   HARGS+4
   INY
   STA   (FP),Y
   LDA   HARGS+5
   INY
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
   LDA   HARGS+9
   INY
   STA   (FP),Y
   CLC
   LDA   FP
   ADC   #$11
   LDY   #$41
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$09
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$0A
   LDA   (FP),Y
   LDY   #$01
   STA   (DPTR),Y
   LDY   #$0B
   LDA   (FP),Y
   LDY   #$02
   STA   (DPTR),Y
   LDY   #$0C
   LDA   (FP),Y
   LDY   #$03
   STA   (DPTR),Y
   LDY   #$0D
   LDA   (FP),Y
   LDY   #$04
   STA   (DPTR),Y
   LDY   #$0E
   LDA   (FP),Y
   LDY   #$05
   STA   (DPTR),Y
   LDY   #$0F
   LDA   (FP),Y
   LDY   #$06
   STA   (DPTR),Y
   LDY   #$10
   LDA   (FP),Y
   LDY   #$07
   STA   (DPTR),Y
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$43
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$44
   STA   (FP),Y
   LDA   @9.x
   STA   HARGS
   LDA   @9.x+1
   STA   HARGS+1
   DEY
   LDA   (FP),Y
   STA   HARGS+2
   INY
   LDA   (FP),Y
   STA   HARGS+3
   JSR   sdivmod16
   LDA   HARGS+4
   LDY   #$45
   STA   (FP),Y
   LDA   HARGS+5
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   HARGS
   INY
   LDA   (FP),Y
   STA   HARGS+1
   JSR   i2d
   LDA   HARGS+2
   LDY   #$47
   STA   (FP),Y
   LDA   HARGS+3
   INY
   STA   (FP),Y
   LDA   HARGS+4
   INY
   STA   (FP),Y
   LDA   HARGS+5
   INY
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
   LDA   HARGS+9
   INY
   STA   (FP),Y
   LDY   #$41
   LDA   (FP),Y
   CLC
   ADC   #$08
   LDY   #$4F
   STA   (FP),Y
   LDY   #$42
   LDA   (FP),Y
   ADC   #$00
   LDY   #$50
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$47
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$48
   LDA   (FP),Y
   LDY   #$01
   STA   (DPTR),Y
   LDY   #$49
   LDA   (FP),Y
   LDY   #$02
   STA   (DPTR),Y
   LDY   #$4A
   LDA   (FP),Y
   LDY   #$03
   STA   (DPTR),Y
   LDY   #$4B
   LDA   (FP),Y
   LDY   #$04
   STA   (DPTR),Y
   LDY   #$4C
   LDA   (FP),Y
   LDY   #$05
   STA   (DPTR),Y
   LDY   #$4D
   LDA   (FP),Y
   LDY   #$06
   STA   (DPTR),Y
   LDY   #$4E
   LDA   (FP),Y
   LDY   #$07
   STA   (DPTR),Y
   JSR   three
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
   JSR   ul2d
   LDA   HARGS+4
   LDY   #$55
   STA   (FP),Y
   LDA   HARGS+5
   INY
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
   LDA   HARGS+9
   INY
   STA   (FP),Y
   LDA   HARGS+10
   INY
   STA   (FP),Y
   LDA   HARGS+11
   INY
   STA   (FP),Y
   LDY   #$41
   LDA   (FP),Y
   CLC
   ADC   #$10
   LDY   #$5D
   STA   (FP),Y
   LDY   #$42
   LDA   (FP),Y
   ADC   #$00
   LDY   #$5E
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$55
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$56
   LDA   (FP),Y
   LDY   #$01
   STA   (DPTR),Y
   LDY   #$57
   LDA   (FP),Y
   LDY   #$02
   STA   (DPTR),Y
   LDY   #$58
   LDA   (FP),Y
   LDY   #$03
   STA   (DPTR),Y
   LDY   #$59
   LDA   (FP),Y
   LDY   #$04
   STA   (DPTR),Y
   LDY   #$5A
   LDA   (FP),Y
   LDY   #$05
   STA   (DPTR),Y
   LDY   #$5B
   LDA   (FP),Y
   LDY   #$06
   STA   (DPTR),Y
   LDY   #$5C
   LDA   (FP),Y
   LDY   #$07
   STA   (DPTR),Y
   LDA   #$18
   LDY   #$5F
   STA   (FP),Y
.clear@11:
   SEC
   SBC   #$01
   LDY   #$5F
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   CLC
   LDA   FP
   ADC   #$11
   INY
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$18
   LDY   #$64
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$65
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   LDY   #$60
   ADC   (FP),Y
   LDY   #$66
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$61
   ADC   (FP),Y
   LDY   #$67
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDA   #$00
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$5F
   LDA   (FP),Y
   BNE   .clear@11
   CLC
   LDA   FP
   ADC   #$11
   LDY   #$6A
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   LDA   #$00
   STA   HARGS
   LDX   #$08
   LSR   HARGS
.mul_loop@14:
   BCC   .mul_skip@15
   CLC
   ADC   #$10
.mul_skip@15:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@14
   LDY   #$69
   STA   (FP),Y
   LDA   HARGS
   DEY
   STA   (FP),Y
   LDY   #$6A
   LDA   (FP),Y
   CLC
   LDY   #$68
   ADC   (FP),Y
   LDY   #$6C
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$69
   ADC   (FP),Y
   LDY   #$6D
   STA   (FP),Y
   LDA   #$00
   STA   HARGS
   LDX   #$08
   LSR   HARGS
.mul_loop@16:
   BCC   .mul_skip@17
   CLC
   ADC   #$08
.mul_skip@17:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@16
   LDY   #$6F
   STA   (FP),Y
   LDA   HARGS
   DEY
   STA   (FP),Y
   LDY   #$6C
   LDA   (FP),Y
   CLC
   LDY   #$6E
   ADC   (FP),Y
   LDY   #$70
   STA   (FP),Y
   LDY   #$6D
   LDA   (FP),Y
   LDY   #$6F
   ADC   (FP),Y
   LDY   #$71
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$72
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$73
   STA   (FP),Y
   LDY   #$02
   LDA   (DPTR),Y
   LDY   #$74
   STA   (FP),Y
   LDY   #$03
   LDA   (DPTR),Y
   LDY   #$75
   STA   (FP),Y
   LDY   #$04
   LDA   (DPTR),Y
   LDY   #$76
   STA   (FP),Y
   LDY   #$05
   LDA   (DPTR),Y
   LDY   #$77
   STA   (FP),Y
   LDY   #$06
   LDA   (DPTR),Y
   LDY   #$78
   STA   (FP),Y
   LDY   #$07
   LDA   (DPTR),Y
   LDY   #$79
   STA   (FP),Y
   CMP   #$40
   BNE   .cmp_true@18
   DEY
   LDA   (FP),Y
   CMP   #$59
   BNE   .cmp_true@18
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@18
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@18
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@18
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@18
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@18
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@18
   LDA   #$00
   JMP   .cmp_end@19
.cmp_true@18:
   LDA   #$01
.cmp_end@19:
   LDY   #$7A
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@9
   JMP   .or_true@14
.lb_skip@9:
   CLC
   LDA   FP
   ADC   #$11
   LDY   #$7E
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   LDA   #$00
   STA   HARGS
   LDX   #$08
   LSR   HARGS
.mul_loop@21:
   BCC   .mul_skip@22
   CLC
   ADC   #$10
.mul_skip@22:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@21
   LDY   #$7D
   STA   (FP),Y
   LDA   HARGS
   DEY
   STA   (FP),Y
   LDY   #$7E
   LDA   (FP),Y
   CLC
   LDY   #$7C
   ADC   (FP),Y
   LDY   #$80
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$7D
   ADC   (FP),Y
   LDY   #$81
   STA   (FP),Y
   LDA   #$01
   STA   HARGS
   LDA   #$00
   LDX   #$08
   LSR   HARGS
.mul_loop@23:
   BCC   .mul_skip@24
   CLC
   ADC   #$08
.mul_skip@24:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@23
   LDY   #$83
   STA   (FP),Y
   LDA   HARGS
   DEY
   STA   (FP),Y
   LDY   #$80
   LDA   (FP),Y
   CLC
   LDY   #$82
   ADC   (FP),Y
   LDY   #$84
   STA   (FP),Y
   LDY   #$81
   LDA   (FP),Y
   LDY   #$83
   ADC   (FP),Y
   LDY   #$85
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$86
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$87
   STA   (FP),Y
   LDY   #$02
   LDA   (DPTR),Y
   LDY   #$88
   STA   (FP),Y
   LDY   #$03
   LDA   (DPTR),Y
   LDY   #$89
   STA   (FP),Y
   LDY   #$04
   LDA   (DPTR),Y
   LDY   #$8A
   STA   (FP),Y
   LDY   #$05
   LDA   (DPTR),Y
   LDY   #$8B
   STA   (FP),Y
   LDY   #$06
   LDA   (DPTR),Y
   LDY   #$8C
   STA   (FP),Y
   LDY   #$07
   LDA   (DPTR),Y
   LDY   #$8D
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
   LDA   #$39
   INY
   STA   (FP),Y
   LDA   #$40
   EOR   #$80
   INY
   STA   (FP),Y
   LDY   #$8D
   LDA   (FP),Y
   LDY   #$95
   CMP   (FP),Y
   BNE   .cmp_true@25
   LDY   #$8C
   LDA   (FP),Y
   LDY   #$94
   CMP   (FP),Y
   BNE   .cmp_true@25
   LDY   #$8B
   LDA   (FP),Y
   LDY   #$93
   CMP   (FP),Y
   BNE   .cmp_true@25
   LDY   #$8A
   LDA   (FP),Y
   LDY   #$92
   CMP   (FP),Y
   BNE   .cmp_true@25
   LDY   #$89
   LDA   (FP),Y
   LDY   #$91
   CMP   (FP),Y
   BNE   .cmp_true@25
   LDY   #$88
   LDA   (FP),Y
   LDY   #$90
   CMP   (FP),Y
   BNE   .cmp_true@25
   LDY   #$87
   LDA   (FP),Y
   LDY   #$8F
   CMP   (FP),Y
   BNE   .cmp_true@25
   LDY   #$86
   LDA   (FP),Y
   LDY   #$8E
   CMP   (FP),Y
   BNE   .cmp_true@25
   LDA   #$00
   JMP   .cmp_end@26
.cmp_true@25:
   LDA   #$01
.cmp_end@26:
   LDY   #$96
   STA   (FP),Y
   LDA   #$00
   INY
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
   LDY   #$98
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@15:
   LDY   #$98
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@8
   JMP   .or_true@12
.lb_skip@8:
   CLC
   LDA   FP
   ADC   #$11
   LDY   #$9C
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   LDA   #$01
   STA   HARGS
   LDA   #$00
   LDX   #$08
   LSR   HARGS
.mul_loop@28:
   BCC   .mul_skip@29
   CLC
   ADC   #$10
.mul_skip@29:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@28
   LDY   #$9B
   STA   (FP),Y
   LDA   HARGS
   DEY
   STA   (FP),Y
   LDY   #$9C
   LDA   (FP),Y
   CLC
   LDY   #$9A
   ADC   (FP),Y
   LDY   #$9E
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$9B
   ADC   (FP),Y
   LDY   #$9F
   STA   (FP),Y
   LDA   #$00
   STA   HARGS
   LDX   #$08
   LSR   HARGS
.mul_loop@30:
   BCC   .mul_skip@31
   CLC
   ADC   #$08
.mul_skip@31:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@30
   LDY   #$A1
   STA   (FP),Y
   LDA   HARGS
   DEY
   STA   (FP),Y
   LDY   #$9E
   LDA   (FP),Y
   CLC
   LDY   #$A0
   ADC   (FP),Y
   LDY   #$A2
   STA   (FP),Y
   LDY   #$9F
   LDA   (FP),Y
   LDY   #$A1
   ADC   (FP),Y
   LDY   #$A3
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$A4
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$A5
   STA   (FP),Y
   LDY   #$02
   LDA   (DPTR),Y
   LDY   #$A6
   STA   (FP),Y
   LDY   #$03
   LDA   (DPTR),Y
   LDY   #$A7
   STA   (FP),Y
   LDY   #$04
   LDA   (DPTR),Y
   LDY   #$A8
   STA   (FP),Y
   LDY   #$05
   LDA   (DPTR),Y
   LDY   #$A9
   STA   (FP),Y
   LDY   #$06
   LDA   (DPTR),Y
   LDY   #$AA
   STA   (FP),Y
   LDY   #$07
   LDA   (DPTR),Y
   LDY   #$AB
   STA   (FP),Y
   CMP   #$40
   BNE   .cmp_true@32
   DEY
   LDA   (FP),Y
   CMP   #$08
   BNE   .cmp_true@32
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@32
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@32
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@32
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@32
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@32
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@32
   LDA   #$00
   JMP   .cmp_end@33
.cmp_true@32:
   LDA   #$01
.cmp_end@33:
   LDY   #$AC
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@12
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   JMP   .or_end@13
.or_true@12:
   LDA   #$01
   LDY   #$AE
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@13:
   LDY   #$AE
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@16
   LDX   #$00
   TXA

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$EB
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$EA
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
.if_end@16:
   CLC
   LDA   FP
   ADC   #$11
   LDY   #$B2
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   LDA   #$01
   STA   HARGS
   LDA   #$00
   LDX   #$08
   LSR   HARGS
.mul_loop@35:
   BCC   .mul_skip@36
   CLC
   ADC   #$10
.mul_skip@36:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@35
   LDY   #$B1
   STA   (FP),Y
   LDA   HARGS
   DEY
   STA   (FP),Y
   LDY   #$B2
   LDA   (FP),Y
   CLC
   LDY   #$B0
   ADC   (FP),Y
   LDY   #$B4
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$B1
   ADC   (FP),Y
   LDY   #$B5
   STA   (FP),Y
   LDA   #$01
   STA   HARGS
   LDA   #$00
   LDX   #$08
   LSR   HARGS
.mul_loop@37:
   BCC   .mul_skip@38
   CLC
   ADC   #$08
.mul_skip@38:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@37
   LDY   #$B7
   STA   (FP),Y
   LDA   HARGS
   DEY
   STA   (FP),Y
   LDY   #$B4
   LDA   (FP),Y
   CLC
   LDY   #$B6
   ADC   (FP),Y
   LDY   #$B8
   STA   (FP),Y
   LDY   #$B5
   LDA   (FP),Y
   LDY   #$B7
   ADC   (FP),Y
   LDY   #$B9
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$BA
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$BB
   STA   (FP),Y
   LDY   #$02
   LDA   (DPTR),Y
   LDY   #$BC
   STA   (FP),Y
   LDY   #$03
   LDA   (DPTR),Y
   LDY   #$BD
   STA   (FP),Y
   LDY   #$04
   LDA   (DPTR),Y
   LDY   #$BE
   STA   (FP),Y
   LDY   #$05
   LDA   (DPTR),Y
   LDY   #$BF
   STA   (FP),Y
   LDY   #$06
   LDA   (DPTR),Y
   LDY   #$C0
   STA   (FP),Y
   LDY   #$07
   LDA   (DPTR),Y
   LDY   #$C1
   STA   (FP),Y
   LDY   #$BA
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
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@7
   JMP   .or_true@19
.lb_skip@7:
   CLC
   LDA   FP
   ADC   #$11
   LDY   #$C4
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   LDA   #$02
   STA   HARGS
   LDA   #$00
   LDX   #$08
   LSR   HARGS
.mul_loop@39:
   BCC   .mul_skip@40
   CLC
   ADC   #$10
.mul_skip@40:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@39
   LDY   #$C3
   STA   (FP),Y
   LDA   HARGS
   DEY
   STA   (FP),Y
   LDY   #$C4
   LDA   (FP),Y
   CLC
   LDY   #$C2
   ADC   (FP),Y
   LDY   #$C6
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$C3
   ADC   (FP),Y
   LDY   #$C7
   STA   (FP),Y
   LDA   #$00
   STA   HARGS
   LDX   #$08
   LSR   HARGS
.mul_loop@41:
   BCC   .mul_skip@42
   CLC
   ADC   #$08
.mul_skip@42:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@41
   LDY   #$C9
   STA   (FP),Y
   LDA   HARGS
   DEY
   STA   (FP),Y
   LDY   #$C6
   LDA   (FP),Y
   CLC
   LDY   #$C8
   ADC   (FP),Y
   LDY   #$CA
   STA   (FP),Y
   LDY   #$C7
   LDA   (FP),Y
   LDY   #$C9
   ADC   (FP),Y
   LDY   #$CB
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$CC
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$CD
   STA   (FP),Y
   LDY   #$02
   LDA   (DPTR),Y
   LDY   #$CE
   STA   (FP),Y
   LDY   #$03
   LDA   (DPTR),Y
   LDY   #$CF
   STA   (FP),Y
   LDY   #$04
   LDA   (DPTR),Y
   LDY   #$D0
   STA   (FP),Y
   LDY   #$05
   LDA   (DPTR),Y
   LDY   #$D1
   STA   (FP),Y
   LDY   #$06
   LDA   (DPTR),Y
   LDY   #$D2
   STA   (FP),Y
   LDY   #$07
   LDA   (DPTR),Y
   LDY   #$D3
   STA   (FP),Y
   LDY   #$CC
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
   INY
   ORA   (FP),Y
   BNE   .or_true@19
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   JMP   .or_end@20
.or_true@19:
   LDA   #$01
   LDY   #$D4
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@20:
   LDY   #$D4
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@6
   JMP   .or_true@17
.lb_skip@6:
   CLC
   LDA   FP
   ADC   #$11
   LDY   #$D8
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   LDA   #$02
   STA   HARGS
   LDA   #$00
   LDX   #$08
   LSR   HARGS
.mul_loop@43:
   BCC   .mul_skip@44
   CLC
   ADC   #$10
.mul_skip@44:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@43
   LDY   #$D7
   STA   (FP),Y
   LDA   HARGS
   DEY
   STA   (FP),Y
   LDY   #$D8
   LDA   (FP),Y
   CLC
   LDY   #$D6
   ADC   (FP),Y
   LDY   #$DA
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$D7
   ADC   (FP),Y
   LDY   #$DB
   STA   (FP),Y
   LDA   #$01
   STA   HARGS
   LDA   #$00
   LDX   #$08
   LSR   HARGS
.mul_loop@45:
   BCC   .mul_skip@46
   CLC
   ADC   #$08
.mul_skip@46:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@45
   LDY   #$DD
   STA   (FP),Y
   LDA   HARGS
   DEY
   STA   (FP),Y
   LDY   #$DA
   LDA   (FP),Y
   CLC
   LDY   #$DC
   ADC   (FP),Y
   LDY   #$DE
   STA   (FP),Y
   LDY   #$DB
   LDA   (FP),Y
   LDY   #$DD
   ADC   (FP),Y
   LDY   #$DF
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$E0
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$E1
   STA   (FP),Y
   LDY   #$02
   LDA   (DPTR),Y
   LDY   #$E2
   STA   (FP),Y
   LDY   #$03
   LDA   (DPTR),Y
   LDY   #$E3
   STA   (FP),Y
   LDY   #$04
   LDA   (DPTR),Y
   LDY   #$E4
   STA   (FP),Y
   LDY   #$05
   LDA   (DPTR),Y
   LDY   #$E5
   STA   (FP),Y
   LDY   #$06
   LDA   (DPTR),Y
   LDY   #$E6
   STA   (FP),Y
   LDY   #$07
   LDA   (DPTR),Y
   LDY   #$E7
   STA   (FP),Y
   LDY   #$E0
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
   LDY   #$E8
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@18:
   LDY   #$E8
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@21
   LDX   #$00
   TXA

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$EB
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
   TAX
   PLA
   RTS
.if_end@21:
   LDX   #$00
   LDA   #$01

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$EB
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$EA
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

three:
   SUBROUTINE

   LDA   #$03
   STA   HARGS+8
   LDA   #$00
   STA   HARGS+9
   LDY   #$00
   STY   HARGS+10
   STA   HARGS+11
   RTS

test_preserve_stack:
   SUBROUTINE

   ; prologue: 0 arg bytes, 104 local bytes
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

   SEC
   LDA   #$00
   SBC   #$01
   LDY   #$01
   STA   (FP),Y
   LDA   #$00
   SBC   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$03
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$04
   STA   (FP),Y
   LDA   one
   STA   HARGS
   LDA   one+1
   STA   HARGS+1
   LDA   one+2
   STA   HARGS+2
   LDA   one+3
   STA   HARGS+3
   LDA   one+4
   STA   HARGS+4
   LDA   one+5
   STA   HARGS+5
   LDA   one+6
   STA   HARGS+6
   LDA   one+7
   STA   HARGS+7
   LDA   #$02
   STA   HARGS+8
   LDA   #$00
   STA   HARGS+9
   LDY   #$00
   STY   HARGS+10
   STY   HARGS+11
   STY   HARGS+12
   STY   HARGS+13
   STY   HARGS+14
   STA   HARGS+15
   JSR   mul64
   LDA   HARGS+16
   LDY   #$05
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
   LDY   #$05
   LDA   (FP),Y
   LDY   #$0D
   STA   (FP),Y
   LDY   #$06
   LDA   (FP),Y
   LDY   #$0E
   STA   (FP),Y
   CLC
   LDA   FP
   ADC   #$0F
   LDY   #$15
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$0D
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$0E
   LDA   (FP),Y
   LDY   #$01
   STA   (DPTR),Y
   JSR   three
   LDA   HARGS+8
   LDY   #$17
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
   LDY   #$17
   LDA   (FP),Y
   LDY   #$1B
   STA   (FP),Y
   LDY   #$18
   LDA   (FP),Y
   LDY   #$1C
   STA   (FP),Y
   LDY   #$19
   LDA   (FP),Y
   LDY   #$1D
   STA   (FP),Y
   LDY   #$1A
   LDA   (FP),Y
   LDY   #$1E
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
   LDA   one
   CLC
   LDY   #$1B
   ADC   (FP),Y
   LDY   #$23
   STA   (FP),Y
   LDA   one+1
   LDY   #$1C
   ADC   (FP),Y
   LDY   #$24
   STA   (FP),Y
   LDA   one+2
   LDY   #$1D
   ADC   (FP),Y
   LDY   #$25
   STA   (FP),Y
   LDA   one+3
   LDY   #$1E
   ADC   (FP),Y
   LDY   #$26
   STA   (FP),Y
   LDA   one+4
   LDY   #$1F
   ADC   (FP),Y
   LDY   #$27
   STA   (FP),Y
   LDA   one+5
   LDY   #$20
   ADC   (FP),Y
   LDY   #$28
   STA   (FP),Y
   LDA   one+6
   LDY   #$21
   ADC   (FP),Y
   LDY   #$29
   STA   (FP),Y
   LDA   one+7
   LDY   #$22
   ADC   (FP),Y
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
   LDY   #$15
   LDA   (FP),Y
   CLC
   ADC   #$02
   LDY   #$2D
   STA   (FP),Y
   LDY   #$16
   LDA   (FP),Y
   ADC   #$00
   LDY   #$2E
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$2B
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$2C
   LDA   (FP),Y
   LDY   #$01
   STA   (DPTR),Y
   LDY   #$15
   LDA   (FP),Y
   CLC
   ADC   #$04
   LDY   #$2F
   STA   (FP),Y
   LDY   #$16
   LDA   (FP),Y
   ADC   #$00
   LDY   #$30
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDA   #$00
   LDY   #$00
   STA   (DPTR),Y
   INY
   STA   (DPTR),Y
   LDA   #$29
   LDY   #$31
   STA   (FP),Y
   LDA   #$90
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   SEC
   SBC   #$01
   INY
   STA   (FP),Y
   LDA   #$00
   SBC   #$00
   INY
   STA   (FP),Y
   LDY   #$04
   LDA   (FP),Y
   LDY   #$36
   CMP   (FP),Y
   BNE   .cmp_true@47
   LDY   #$03
   LDA   (FP),Y
   LDY   #$35
   CMP   (FP),Y
   BNE   .cmp_true@47
   LDA   #$00
   JMP   .cmp_end@48
.cmp_true@47:
   LDA   #$01
.cmp_end@48:
   LDY   #$37
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@22
   LDX   #$00
   TXA

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$6A
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
.if_end@22:
   LDY   #$32
   LDA   (FP),Y
   CMP   #$90
   BNE   .cmp_true@50
   DEY
   LDA   (FP),Y
   CMP   #$29
   BNE   .cmp_true@50
   LDA   #$00
   JMP   .cmp_end@51
.cmp_true@50:
   LDA   #$01
.cmp_end@51:
   LDY   #$39
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@23
   LDX   #$00
   TXA

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$6A
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
.if_end@23:
   CLC
   LDA   FP
   ADC   #$0F
   LDY   #$3D
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   LDA   #$00
   STA   HARGS
   LDX   #$08
   LSR   HARGS
.mul_loop@53:
   BCC   .mul_skip@54
   CLC
   ADC   #$02
.mul_skip@54:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@53
   LDY   #$3C
   STA   (FP),Y
   LDA   HARGS
   DEY
   STA   (FP),Y
   LDY   #$3D
   LDA   (FP),Y
   CLC
   LDY   #$3B
   ADC   (FP),Y
   LDY   #$3F
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$3C
   ADC   (FP),Y
   LDY   #$40
   STA   (FP),Y
   LDA   #$00
   STA   HARGS
   LDX   #$08
   LSR   HARGS
.mul_loop@55:
   BCC   .mul_skip@56
   CLC
   ADC   #$02
.mul_skip@56:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@55
   LDY   #$42
   STA   (FP),Y
   LDA   HARGS
   DEY
   STA   (FP),Y
   LDY   #$3F
   LDA   (FP),Y
   CLC
   LDY   #$41
   ADC   (FP),Y
   LDY   #$43
   STA   (FP),Y
   LDY   #$40
   LDA   (FP),Y
   LDY   #$42
   ADC   (FP),Y
   LDY   #$44
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$45
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$46
   STA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@57
   DEY
   LDA   (FP),Y
   CMP   #$02
   BNE   .cmp_true@57
   LDA   #$00
   JMP   .cmp_end@58
.cmp_true@57:
   LDA   #$01
.cmp_end@58:
   LDY   #$47
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@11
   JMP   .or_true@26
.lb_skip@11:
   CLC
   LDA   FP
   ADC   #$0F
   LDY   #$4B
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   LDA   #$01
   STA   HARGS
   LDA   #$00
   LDX   #$08
   LSR   HARGS
.mul_loop@60:
   BCC   .mul_skip@61
   CLC
   ADC   #$02
.mul_skip@61:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@60
   LDY   #$4A
   STA   (FP),Y
   LDA   HARGS
   DEY
   STA   (FP),Y
   LDY   #$4B
   LDA   (FP),Y
   CLC
   LDY   #$49
   ADC   (FP),Y
   LDY   #$4D
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$4A
   ADC   (FP),Y
   LDY   #$4E
   STA   (FP),Y
   LDA   #$00
   STA   HARGS
   LDX   #$08
   LSR   HARGS
.mul_loop@62:
   BCC   .mul_skip@63
   CLC
   ADC   #$02
.mul_skip@63:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@62
   LDY   #$50
   STA   (FP),Y
   LDA   HARGS
   DEY
   STA   (FP),Y
   LDY   #$4D
   LDA   (FP),Y
   CLC
   LDY   #$4F
   ADC   (FP),Y
   LDY   #$51
   STA   (FP),Y
   LDY   #$4E
   LDA   (FP),Y
   LDY   #$50
   ADC   (FP),Y
   LDY   #$52
   STA   (FP),Y
   DEY
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
   CMP   #$00
   BNE   .cmp_true@64
   DEY
   LDA   (FP),Y
   CMP   #$04
   BNE   .cmp_true@64
   LDA   #$00
   JMP   .cmp_end@65
.cmp_true@64:
   LDA   #$01
.cmp_end@65:
   LDY   #$55
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@26
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   JMP   .or_end@27
.or_true@26:
   LDA   #$01
   LDY   #$57
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@27:
   LDY   #$57
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@10
   JMP   .or_true@24
.lb_skip@10:
   CLC
   LDA   FP
   ADC   #$0F
   LDY   #$5B
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   LDA   #$02
   STA   HARGS
   LDA   #$00
   LDX   #$08
   LSR   HARGS
.mul_loop@67:
   BCC   .mul_skip@68
   CLC
   ADC   #$02
.mul_skip@68:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@67
   LDY   #$5A
   STA   (FP),Y
   LDA   HARGS
   DEY
   STA   (FP),Y
   LDY   #$5B
   LDA   (FP),Y
   CLC
   LDY   #$59
   ADC   (FP),Y
   LDY   #$5D
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$5A
   ADC   (FP),Y
   LDY   #$5E
   STA   (FP),Y
   LDA   #$00
   STA   HARGS
   LDX   #$08
   LSR   HARGS
.mul_loop@69:
   BCC   .mul_skip@70
   CLC
   ADC   #$02
.mul_skip@70:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@69
   LDY   #$60
   STA   (FP),Y
   LDA   HARGS
   DEY
   STA   (FP),Y
   LDY   #$5D
   LDA   (FP),Y
   CLC
   LDY   #$5F
   ADC   (FP),Y
   LDY   #$61
   STA   (FP),Y
   LDY   #$5E
   LDA   (FP),Y
   LDY   #$60
   ADC   (FP),Y
   LDY   #$62
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$63
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$64
   STA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@71
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@71
   LDA   #$00
   JMP   .cmp_end@72
.cmp_true@71:
   LDA   #$01
.cmp_end@72:
   LDY   #$65
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@24
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   JMP   .or_end@25
.or_true@24:
   LDA   #$01
   LDY   #$67
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@25:
   LDY   #$67
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@28
   LDX   #$00
   TXA

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$6A
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
   TAX
   PLA
   RTS
.if_end@28:
   LDX   #$00
   LDA   #$01

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$6A
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

   ; prologue: 0 arg bytes, 16 local bytes
   SEC
   LDA   SSP
   SBC   #$12
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$11
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   JSR   test_simple
   LDY   #$01
   STA   (FP),Y
   TXA
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lnot_true@74
   LDA   #$00
   JMP   .lnot_end@75
.lnot_true@74:
   LDA   #$01
.lnot_end@75:
   LDY   #$03
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@29
   LDX   #$00
   LDA   #$01

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$12
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$11
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
.if_end@29:
   JSR   test_partial
   LDY   #$05
   STA   (FP),Y
   TXA
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lnot_true@76
   LDA   #$00
   JMP   .lnot_end@77
.lnot_true@76:
   LDA   #$01
.lnot_end@77:
   LDY   #$07
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@30
   LDX   #$00
   LDA   #$02

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$12
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$11
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
.if_end@30:
   JSR   test_non_constant_and_type_conversion
   LDY   #$09
   STA   (FP),Y
   TXA
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lnot_true@78
   LDA   #$00
   JMP   .lnot_end@79
.lnot_true@78:
   LDA   #$01
.lnot_end@79:
   LDY   #$0B
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@31
   LDX   #$00
   LDA   #$03

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$12
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$11
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
.if_end@31:
   JSR   test_preserve_stack
   LDY   #$0D
   STA   (FP),Y
   TXA
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lnot_true@80
   LDA   #$00
   JMP   .lnot_end@81
.lnot_true@80:
   LDA   #$01
.lnot_end@81:
   LDY   #$0F
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@32
   LDX   #$00
   LDA   #$04

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$12
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
   TAX
   PLA
   RTS
.if_end@32:
   LDX   #$00
   TXA

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$12
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$11
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

@9.x:
   DC.W  $0064

one:
   DC.L  $00000001
   DC.L  $00000000
