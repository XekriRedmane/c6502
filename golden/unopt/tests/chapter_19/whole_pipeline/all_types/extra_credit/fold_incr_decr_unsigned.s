SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24

target:
   SUBROUTINE

   ; prologue: 0 arg bytes, 208 local bytes
   SEC
   LDA   SSP
   SBC   #$D2
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$D1
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
   INY
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
   LDY   #$09
   LDA   (FP),Y
   SEC
   SBC   #$01
   LDY   #$11
   STA   (FP),Y
   LDY   #$0A
   LDA   (FP),Y
   SBC   #$00
   LDY   #$12
   STA   (FP),Y
   LDY   #$0B
   LDA   (FP),Y
   SBC   #$00
   LDY   #$13
   STA   (FP),Y
   LDY   #$0C
   LDA   (FP),Y
   SBC   #$00
   LDY   #$14
   STA   (FP),Y
   LDY   #$0D
   LDA   (FP),Y
   SBC   #$00
   LDY   #$15
   STA   (FP),Y
   LDY   #$0E
   LDA   (FP),Y
   SBC   #$00
   LDY   #$16
   STA   (FP),Y
   LDY   #$0F
   LDA   (FP),Y
   SBC   #$00
   LDY   #$17
   STA   (FP),Y
   LDY   #$10
   LDA   (FP),Y
   SBC   #$00
   LDY   #$18
   STA   (FP),Y
   LDY   #$11
   LDA   (FP),Y
   LDY   #$09
   STA   (FP),Y
   LDY   #$12
   LDA   (FP),Y
   LDY   #$0A
   STA   (FP),Y
   LDY   #$13
   LDA   (FP),Y
   LDY   #$0B
   STA   (FP),Y
   LDY   #$14
   LDA   (FP),Y
   LDY   #$0C
   STA   (FP),Y
   LDY   #$15
   LDA   (FP),Y
   LDY   #$0D
   STA   (FP),Y
   LDY   #$16
   LDA   (FP),Y
   LDY   #$0E
   STA   (FP),Y
   LDY   #$17
   LDA   (FP),Y
   LDY   #$0F
   STA   (FP),Y
   LDY   #$18
   LDA   (FP),Y
   LDY   #$10
   STA   (FP),Y
   INY
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
   LDY   #$14
   LDA   (FP),Y
   LDY   #$1C
   STA   (FP),Y
   LDY   #$15
   LDA   (FP),Y
   LDY   #$1D
   STA   (FP),Y
   LDY   #$16
   LDA   (FP),Y
   LDY   #$1E
   STA   (FP),Y
   LDY   #$17
   LDA   (FP),Y
   LDY   #$1F
   STA   (FP),Y
   LDY   #$18
   LDA   (FP),Y
   LDY   #$20
   STA   (FP),Y
   LDY   #$09
   LDA   (FP),Y
   LDY   #$21
   STA   (FP),Y
   LDY   #$0A
   LDA   (FP),Y
   LDY   #$22
   STA   (FP),Y
   LDY   #$0B
   LDA   (FP),Y
   LDY   #$23
   STA   (FP),Y
   LDY   #$0C
   LDA   (FP),Y
   LDY   #$24
   STA   (FP),Y
   LDY   #$0D
   LDA   (FP),Y
   LDY   #$25
   STA   (FP),Y
   LDY   #$0E
   LDA   (FP),Y
   LDY   #$26
   STA   (FP),Y
   LDY   #$0F
   LDA   (FP),Y
   LDY   #$27
   STA   (FP),Y
   LDY   #$10
   LDA   (FP),Y
   LDY   #$28
   STA   (FP),Y
   LDY   #$09
   LDA   (FP),Y
   SEC
   SBC   #$01
   LDY   #$29
   STA   (FP),Y
   LDY   #$0A
   LDA   (FP),Y
   SBC   #$00
   LDY   #$2A
   STA   (FP),Y
   LDY   #$0B
   LDA   (FP),Y
   SBC   #$00
   LDY   #$2B
   STA   (FP),Y
   LDY   #$0C
   LDA   (FP),Y
   SBC   #$00
   LDY   #$2C
   STA   (FP),Y
   LDY   #$0D
   LDA   (FP),Y
   SBC   #$00
   LDY   #$2D
   STA   (FP),Y
   LDY   #$0E
   LDA   (FP),Y
   SBC   #$00
   LDY   #$2E
   STA   (FP),Y
   LDY   #$0F
   LDA   (FP),Y
   SBC   #$00
   LDY   #$2F
   STA   (FP),Y
   LDY   #$10
   LDA   (FP),Y
   SBC   #$00
   LDY   #$30
   STA   (FP),Y
   LDY   #$29
   LDA   (FP),Y
   LDY   #$09
   STA   (FP),Y
   LDY   #$2A
   LDA   (FP),Y
   LDY   #$0A
   STA   (FP),Y
   LDY   #$2B
   LDA   (FP),Y
   LDY   #$0B
   STA   (FP),Y
   LDY   #$2C
   LDA   (FP),Y
   LDY   #$0C
   STA   (FP),Y
   LDY   #$2D
   LDA   (FP),Y
   LDY   #$0D
   STA   (FP),Y
   LDY   #$2E
   LDA   (FP),Y
   LDY   #$0E
   STA   (FP),Y
   LDY   #$2F
   LDA   (FP),Y
   LDY   #$0F
   STA   (FP),Y
   LDY   #$30
   LDA   (FP),Y
   LDY   #$10
   STA   (FP),Y
   LDY   #$21
   LDA   (FP),Y
   LDY   #$31
   STA   (FP),Y
   LDY   #$22
   LDA   (FP),Y
   LDY   #$32
   STA   (FP),Y
   LDY   #$23
   LDA   (FP),Y
   LDY   #$33
   STA   (FP),Y
   LDY   #$24
   LDA   (FP),Y
   LDY   #$34
   STA   (FP),Y
   LDY   #$25
   LDA   (FP),Y
   LDY   #$35
   STA   (FP),Y
   LDY   #$26
   LDA   (FP),Y
   LDY   #$36
   STA   (FP),Y
   LDY   #$27
   LDA   (FP),Y
   LDY   #$37
   STA   (FP),Y
   LDY   #$28
   LDA   (FP),Y
   LDY   #$38
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
   LDA   #$00
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
   LDY   #$41
   STA   (FP),Y
   LDY   #$3A
   LDA   (FP),Y
   LDY   #$42
   STA   (FP),Y
   LDY   #$3B
   LDA   (FP),Y
   LDY   #$43
   STA   (FP),Y
   LDY   #$3C
   LDA   (FP),Y
   LDY   #$44
   STA   (FP),Y
   LDY   #$3D
   LDA   (FP),Y
   LDY   #$45
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$41
   LDA   (FP),Y
   LDY   #$49
   STA   (FP),Y
   LDY   #$42
   LDA   (FP),Y
   LDY   #$4A
   STA   (FP),Y
   LDY   #$43
   LDA   (FP),Y
   LDY   #$4B
   STA   (FP),Y
   LDY   #$44
   LDA   (FP),Y
   LDY   #$4C
   STA   (FP),Y
   LDY   #$45
   LDA   (FP),Y
   LDY   #$4D
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$41
   LDA   (FP),Y
   CLC
   ADC   #$01
   LDY   #$51
   STA   (FP),Y
   LDY   #$42
   LDA   (FP),Y
   ADC   #$00
   LDY   #$52
   STA   (FP),Y
   LDY   #$43
   LDA   (FP),Y
   ADC   #$00
   LDY   #$53
   STA   (FP),Y
   LDY   #$44
   LDA   (FP),Y
   ADC   #$00
   LDY   #$54
   STA   (FP),Y
   LDY   #$45
   LDA   (FP),Y
   ADC   #$00
   LDY   #$55
   STA   (FP),Y
   LDY   #$46
   LDA   (FP),Y
   ADC   #$00
   LDY   #$56
   STA   (FP),Y
   LDY   #$47
   LDA   (FP),Y
   ADC   #$00
   LDY   #$57
   STA   (FP),Y
   LDY   #$48
   LDA   (FP),Y
   ADC   #$00
   LDY   #$58
   STA   (FP),Y
   LDY   #$51
   LDA   (FP),Y
   LDY   #$41
   STA   (FP),Y
   LDY   #$52
   LDA   (FP),Y
   LDY   #$42
   STA   (FP),Y
   LDY   #$53
   LDA   (FP),Y
   LDY   #$43
   STA   (FP),Y
   LDY   #$54
   LDA   (FP),Y
   LDY   #$44
   STA   (FP),Y
   LDY   #$55
   LDA   (FP),Y
   LDY   #$45
   STA   (FP),Y
   LDY   #$56
   LDA   (FP),Y
   LDY   #$46
   STA   (FP),Y
   LDY   #$57
   LDA   (FP),Y
   LDY   #$47
   STA   (FP),Y
   LDY   #$58
   LDA   (FP),Y
   LDY   #$48
   STA   (FP),Y
   INY
   LDA   (FP),Y
   LDY   #$59
   STA   (FP),Y
   LDY   #$4A
   LDA   (FP),Y
   LDY   #$5A
   STA   (FP),Y
   LDY   #$4B
   LDA   (FP),Y
   LDY   #$5B
   STA   (FP),Y
   LDY   #$4C
   LDA   (FP),Y
   LDY   #$5C
   STA   (FP),Y
   LDY   #$4D
   LDA   (FP),Y
   LDY   #$5D
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$41
   LDA   (FP),Y
   CLC
   ADC   #$01
   LDY   #$61
   STA   (FP),Y
   LDY   #$42
   LDA   (FP),Y
   ADC   #$00
   LDY   #$62
   STA   (FP),Y
   LDY   #$43
   LDA   (FP),Y
   ADC   #$00
   LDY   #$63
   STA   (FP),Y
   LDY   #$44
   LDA   (FP),Y
   ADC   #$00
   LDY   #$64
   STA   (FP),Y
   LDY   #$45
   LDA   (FP),Y
   ADC   #$00
   LDY   #$65
   STA   (FP),Y
   LDY   #$46
   LDA   (FP),Y
   ADC   #$00
   LDY   #$66
   STA   (FP),Y
   LDY   #$47
   LDA   (FP),Y
   ADC   #$00
   LDY   #$67
   STA   (FP),Y
   LDY   #$48
   LDA   (FP),Y
   ADC   #$00
   LDY   #$68
   STA   (FP),Y
   LDY   #$61
   LDA   (FP),Y
   LDY   #$41
   STA   (FP),Y
   LDY   #$62
   LDA   (FP),Y
   LDY   #$42
   STA   (FP),Y
   LDY   #$63
   LDA   (FP),Y
   LDY   #$43
   STA   (FP),Y
   LDY   #$64
   LDA   (FP),Y
   LDY   #$44
   STA   (FP),Y
   LDY   #$65
   LDA   (FP),Y
   LDY   #$45
   STA   (FP),Y
   LDY   #$66
   LDA   (FP),Y
   LDY   #$46
   STA   (FP),Y
   LDY   #$67
   LDA   (FP),Y
   LDY   #$47
   STA   (FP),Y
   LDY   #$68
   LDA   (FP),Y
   LDY   #$48
   STA   (FP),Y
   LDY   #$61
   LDA   (FP),Y
   LDY   #$69
   STA   (FP),Y
   LDY   #$62
   LDA   (FP),Y
   LDY   #$6A
   STA   (FP),Y
   LDY   #$63
   LDA   (FP),Y
   LDY   #$6B
   STA   (FP),Y
   LDY   #$64
   LDA   (FP),Y
   LDY   #$6C
   STA   (FP),Y
   LDY   #$65
   LDA   (FP),Y
   LDY   #$6D
   STA   (FP),Y
   LDY   #$66
   LDA   (FP),Y
   LDY   #$6E
   STA   (FP),Y
   LDY   #$67
   LDA   (FP),Y
   LDY   #$6F
   STA   (FP),Y
   LDY   #$68
   LDA   (FP),Y
   LDY   #$70
   STA   (FP),Y
   LDA   #$FE
   INY
   STA   (FP),Y
   LDA   #$FF
   INY
   STA   (FP),Y
   INY
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
   LDY   #$10
   LDA   (FP),Y
   LDY   #$78
   CMP   (FP),Y
   BNE   .thread@0
   LDY   #$0F
   LDA   (FP),Y
   LDY   #$77
   CMP   (FP),Y
   BNE   .thread@0
   LDY   #$0E
   LDA   (FP),Y
   LDY   #$76
   CMP   (FP),Y
   BNE   .thread@0
   LDY   #$0D
   LDA   (FP),Y
   LDY   #$75
   CMP   (FP),Y
   BNE   .thread@0
   LDY   #$0C
   LDA   (FP),Y
   LDY   #$74
   CMP   (FP),Y
   BNE   .thread@0
   LDY   #$0B
   LDA   (FP),Y
   LDY   #$73
   CMP   (FP),Y
   BNE   .thread@0
   LDY   #$0A
   LDA   (FP),Y
   LDY   #$72
   CMP   (FP),Y
   BNE   .thread@0
   LDY   #$09
   LDA   (FP),Y
   LDY   #$71
   CMP   (FP),Y
   BEQ   .cmp_true@2
.thread@0:
   LDA   #$00
   JMP   .cmp_end@3
.cmp_true@2:
   LDA   #$01
.cmp_end@3:
   LDY   #$79
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@3
   JMP   .and_false@2
.lb_skip@3:
   LDA   #$FF
   LDY   #$7B
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
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
   LDY   #$20
   LDA   (FP),Y
   LDY   #$82
   CMP   (FP),Y
   BNE   .thread@1
   LDY   #$1F
   LDA   (FP),Y
   LDY   #$81
   CMP   (FP),Y
   BNE   .thread@1
   LDY   #$1E
   LDA   (FP),Y
   LDY   #$80
   CMP   (FP),Y
   BNE   .thread@1
   LDY   #$1D
   LDA   (FP),Y
   LDY   #$7F
   CMP   (FP),Y
   BNE   .thread@1
   LDY   #$1C
   LDA   (FP),Y
   LDY   #$7E
   CMP   (FP),Y
   BNE   .thread@1
   LDY   #$1B
   LDA   (FP),Y
   LDY   #$7D
   CMP   (FP),Y
   BNE   .thread@1
   LDY   #$1A
   LDA   (FP),Y
   LDY   #$7C
   CMP   (FP),Y
   BNE   .thread@1
   LDY   #$19
   LDA   (FP),Y
   LDY   #$7B
   CMP   (FP),Y
   BEQ   .cmp_true@5
.thread@1:
   LDA   #$00
   JMP   .cmp_end@6
.cmp_true@5:
   LDA   #$01
.cmp_end@6:
   LDY   #$83
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@2
   LDA   #$01
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   JMP   .and_end@3
.and_false@2:
   LDA   #$00
   LDY   #$85
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@3:
   LDY   #$85
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@2
   JMP   .and_false@0
.lb_skip@2:
   LDA   #$FF
   LDY   #$87
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
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
   LDY   #$38
   LDA   (FP),Y
   LDY   #$8E
   CMP   (FP),Y
   BNE   .thread@2
   LDY   #$37
   LDA   (FP),Y
   LDY   #$8D
   CMP   (FP),Y
   BNE   .thread@2
   LDY   #$36
   LDA   (FP),Y
   LDY   #$8C
   CMP   (FP),Y
   BNE   .thread@2
   LDY   #$35
   LDA   (FP),Y
   LDY   #$8B
   CMP   (FP),Y
   BNE   .thread@2
   LDY   #$34
   LDA   (FP),Y
   LDY   #$8A
   CMP   (FP),Y
   BNE   .thread@2
   LDY   #$33
   LDA   (FP),Y
   LDY   #$89
   CMP   (FP),Y
   BNE   .thread@2
   LDY   #$32
   LDA   (FP),Y
   LDY   #$88
   CMP   (FP),Y
   BNE   .thread@2
   LDY   #$31
   LDA   (FP),Y
   LDY   #$87
   CMP   (FP),Y
   BEQ   .cmp_true@8
.thread@2:
   LDA   #$00
   JMP   .cmp_end@9
.cmp_true@8:
   LDA   #$01
.cmp_end@9:
   LDY   #$8F
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
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
   LDY   #$91
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@1:
   LDY   #$91
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lnot_true@11
   LDA   #$00
   JMP   .lnot_end@12
.lnot_true@11:
   LDA   #$01
.lnot_end@12:
   LDY   #$93
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@4
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
   LDY   #$97
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
   LDY   #$95
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
   ADC   #$D2
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$D1
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   RTS
.if_end@4:
   LDA   #$01
   LDY   #$9D
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   ORA   #$00
   BPL   .sx_done@16
   LDA   #$FF
.sx_done@16:
   LDY   #$9F
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
   LDY   #$48
   LDA   (FP),Y
   LDY   #$A4
   CMP   (FP),Y
   BNE   .thread@3
   LDY   #$47
   LDA   (FP),Y
   LDY   #$A3
   CMP   (FP),Y
   BNE   .thread@3
   LDY   #$46
   LDA   (FP),Y
   LDY   #$A2
   CMP   (FP),Y
   BNE   .thread@3
   LDY   #$45
   LDA   (FP),Y
   LDY   #$A1
   CMP   (FP),Y
   BNE   .thread@3
   LDY   #$44
   LDA   (FP),Y
   LDY   #$A0
   CMP   (FP),Y
   BNE   .thread@3
   LDY   #$43
   LDA   (FP),Y
   LDY   #$9F
   CMP   (FP),Y
   BNE   .thread@3
   LDY   #$42
   LDA   (FP),Y
   LDY   #$9E
   CMP   (FP),Y
   BNE   .thread@3
   LDY   #$41
   LDA   (FP),Y
   LDY   #$9D
   CMP   (FP),Y
   BEQ   .cmp_true@17
.thread@3:
   LDA   #$00
   JMP   .cmp_end@18
.cmp_true@17:
   LDA   #$01
.cmp_end@18:
   LDY   #$A5
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@1
   JMP   .and_false@7
.lb_skip@1:
   LDA   #$FF
   LDY   #$A7
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
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
   LDY   #$60
   LDA   (FP),Y
   LDY   #$AE
   CMP   (FP),Y
   BNE   .thread@4
   DEY
   CMP   (FP),Y
   BNE   .thread@4
   DEY
   CMP   (FP),Y
   BNE   .thread@4
   DEY
   CMP   (FP),Y
   BNE   .thread@4
   LDY   #$5C
   LDA   (FP),Y
   LDY   #$AA
   CMP   (FP),Y
   BNE   .thread@4
   LDY   #$5B
   LDA   (FP),Y
   LDY   #$A9
   CMP   (FP),Y
   BNE   .thread@4
   LDY   #$5A
   LDA   (FP),Y
   LDY   #$A8
   CMP   (FP),Y
   BNE   .thread@4
   LDY   #$59
   LDA   (FP),Y
   LDY   #$A7
   CMP   (FP),Y
   BEQ   .cmp_true@20
.thread@4:
   LDA   #$00
   JMP   .cmp_end@21
.cmp_true@20:
   LDA   #$01
.cmp_end@21:
   LDY   #$AF
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@7
   LDA   #$01
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   JMP   .and_end@8
.and_false@7:
   LDA   #$00
   LDY   #$B1
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@8:
   LDY   #$B1
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@0
   JMP   .and_false@5
.lb_skip@0:
   LDA   #$01
   LDY   #$B3
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   ORA   #$00
   BPL   .sx_done@24
   LDA   #$FF
.sx_done@24:
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
   LDY   #$70
   LDA   (FP),Y
   LDY   #$BA
   CMP   (FP),Y
   BNE   .thread@5
   LDY   #$6F
   LDA   (FP),Y
   LDY   #$B9
   CMP   (FP),Y
   BNE   .thread@5
   LDY   #$6E
   LDA   (FP),Y
   LDY   #$B8
   CMP   (FP),Y
   BNE   .thread@5
   LDY   #$6D
   LDA   (FP),Y
   LDY   #$B7
   CMP   (FP),Y
   BNE   .thread@5
   LDY   #$6C
   LDA   (FP),Y
   LDY   #$B6
   CMP   (FP),Y
   BNE   .thread@5
   LDY   #$6B
   LDA   (FP),Y
   LDY   #$B5
   CMP   (FP),Y
   BNE   .thread@5
   LDY   #$6A
   LDA   (FP),Y
   LDY   #$B4
   CMP   (FP),Y
   BNE   .thread@5
   LDY   #$69
   LDA   (FP),Y
   LDY   #$B3
   CMP   (FP),Y
   BEQ   .cmp_true@25
.thread@5:
   LDA   #$00
   JMP   .cmp_end@26
.cmp_true@25:
   LDA   #$01
.cmp_end@26:
   LDY   #$BB
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
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
   LDY   #$BD
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@6:
   LDY   #$BD
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lnot_true@28
   LDA   #$00
   JMP   .lnot_end@29
.lnot_true@28:
   LDA   #$01
.lnot_end@29:
   LDY   #$BF
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@9
   LDA   #$02
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   ORA   #$00
   BPL   .sx_done@31
   LDA   #$FF
.sx_done@31:
   LDY   #$C3
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
   LDY   #$C1
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
   ADC   #$D2
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$D1
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   RTS
.if_end@9:
   LDA   #$00
   LDY   #$C9
   STA   (FP),Y
   INY
   STA   (FP),Y
   ORA   #$00
   BPL   .sx_done@33
   LDA   #$FF
.sx_done@33:
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
   ADC   #$D2
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

   ; prologue: 0 arg bytes, 8 local bytes
   SEC
   LDA   SSP
   SBC   #$0A
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$09
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   JSR   target
   LDA   HARGS+16
   LDY   #$01
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
   LDY   #$01
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
   ADC   #$0A
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
