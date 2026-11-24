SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24

fun:
   SUBROUTINE

   ; prologue: 30 arg bytes, 76 local bytes
   SEC
   LDA   SSP
   SBC   #$4E
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$4D
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   INY
   LDA   (FP),Y
   STA   HARGS
   INY
   LDA   (FP),Y
   STA   HARGS+1
   JSR   i2d
   LDA   HARGS+2
   LDY   #$01
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
   LDY   #$6C
   CMP   (FP),Y
   BNE   .cmp_true@0
   LDY   #$07
   LDA   (FP),Y
   LDY   #$6B
   CMP   (FP),Y
   BNE   .cmp_true@0
   LDY   #$06
   LDA   (FP),Y
   LDY   #$6A
   CMP   (FP),Y
   BNE   .cmp_true@0
   LDY   #$05
   LDA   (FP),Y
   LDY   #$69
   CMP   (FP),Y
   BNE   .cmp_true@0
   LDY   #$04
   LDA   (FP),Y
   LDY   #$68
   CMP   (FP),Y
   BNE   .cmp_true@0
   LDY   #$03
   LDA   (FP),Y
   LDY   #$67
   CMP   (FP),Y
   BNE   .cmp_true@0
   LDY   #$02
   LDA   (FP),Y
   LDY   #$66
   CMP   (FP),Y
   BNE   .cmp_true@0
   LDY   #$01
   LDA   (FP),Y
   LDY   #$65
   CMP   (FP),Y
   BNE   .cmp_true@0
   LDA   #$00
   JMP   .cmp_end@1
.cmp_true@0:
   LDA   #$01
.cmp_end@1:
   LDY   #$09
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@0
   JMP   .if_end@0
.lb_skip@0:
   LDY   #$4F
   LDA   (FP),Y
   CLC
   ADC   #$01
   LDY   #$0B
   STA   (FP),Y
   LDY   #$50
   LDA   (FP),Y
   ADC   #$00
   LDY   #$0C
   STA   (FP),Y
   LDY   #$59
   LDA   (FP),Y
   CLC
   ADC   #$01
   LDY   #$0D
   STA   (FP),Y
   LDY   #$5A
   LDA   (FP),Y
   ADC   #$00
   LDY   #$0E
   STA   (FP),Y
   LDY   #$63
   LDA   (FP),Y
   CLC
   ADC   #$01
   LDY   #$0F
   STA   (FP),Y
   LDY   #$64
   LDA   (FP),Y
   ADC   #$00
   LDY   #$10
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$1E
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDY   #$0B
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$0C
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$51
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$52
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   LDY   #$53
   LDA   (FP),Y
   LDY   #$05
   STA   (SSP),Y
   LDY   #$54
   LDA   (FP),Y
   LDY   #$06
   STA   (SSP),Y
   LDY   #$55
   LDA   (FP),Y
   LDY   #$07
   STA   (SSP),Y
   LDY   #$56
   LDA   (FP),Y
   LDY   #$08
   STA   (SSP),Y
   LDY   #$57
   LDA   (FP),Y
   LDY   #$09
   STA   (SSP),Y
   LDY   #$58
   LDA   (FP),Y
   LDY   #$0A
   STA   (SSP),Y
   LDY   #$0D
   LDA   (FP),Y
   LDY   #$0B
   STA   (SSP),Y
   LDY   #$0E
   LDA   (FP),Y
   LDY   #$0C
   STA   (SSP),Y
   LDY   #$5B
   LDA   (FP),Y
   LDY   #$0D
   STA   (SSP),Y
   LDY   #$5C
   LDA   (FP),Y
   LDY   #$0E
   STA   (SSP),Y
   LDY   #$5D
   LDA   (FP),Y
   LDY   #$0F
   STA   (SSP),Y
   LDY   #$5E
   LDA   (FP),Y
   LDY   #$10
   STA   (SSP),Y
   LDY   #$5F
   LDA   (FP),Y
   LDY   #$11
   STA   (SSP),Y
   LDY   #$60
   LDA   (FP),Y
   LDY   #$12
   STA   (SSP),Y
   LDY   #$61
   LDA   (FP),Y
   LDY   #$13
   STA   (SSP),Y
   LDY   #$62
   LDA   (FP),Y
   LDY   #$14
   STA   (SSP),Y
   LDY   #$0F
   LDA   (FP),Y
   LDY   #$15
   STA   (SSP),Y
   LDY   #$10
   LDA   (FP),Y
   LDY   #$16
   STA   (SSP),Y
   LDY   #$65
   LDA   (FP),Y
   LDY   #$17
   STA   (SSP),Y
   LDY   #$66
   LDA   (FP),Y
   LDY   #$18
   STA   (SSP),Y
   LDY   #$67
   LDA   (FP),Y
   LDY   #$19
   STA   (SSP),Y
   LDY   #$68
   LDA   (FP),Y
   LDY   #$1A
   STA   (SSP),Y
   LDY   #$69
   LDA   (FP),Y
   LDY   #$1B
   STA   (SSP),Y
   LDY   #$6A
   LDA   (FP),Y
   LDY   #$1C
   STA   (SSP),Y
   LDY   #$6B
   LDA   (FP),Y
   LDY   #$1D
   STA   (SSP),Y
   LDY   #$6C
   LDA   (FP),Y
   LDY   #$1E
   STA   (SSP),Y
   JSR   fun
   LDY   #$11
   STA   (FP),Y
   TXA
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
   LDA   #$00
   STA   HARGS+8
   LDY   #$00
   STY   HARGS+9
   STY   HARGS+10
   STY   HARGS+11
   STY   HARGS+12
   STA   HARGS+13
   LDA   #$F0
   STA   HARGS+14
   LDA   #$3F
   STA   HARGS+15
   JSR   dsub
   LDA   HARGS+16
   LDY   #$15
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
   LDY   #$5B
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
   LDA   #$00
   STA   HARGS+8
   LDY   #$00
   STY   HARGS+9
   STY   HARGS+10
   STY   HARGS+11
   STY   HARGS+12
   STA   HARGS+13
   LDA   #$F0
   STA   HARGS+14
   LDA   #$3F
   STA   HARGS+15
   JSR   dsub
   LDA   HARGS+16
   LDY   #$1D
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
   LDY   #$65
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
   LDA   #$00
   STA   HARGS+8
   LDY   #$00
   STY   HARGS+9
   STY   HARGS+10
   STY   HARGS+11
   STY   HARGS+12
   STA   HARGS+13
   LDA   #$F0
   STA   HARGS+14
   LDA   #$3F
   STA   HARGS+15
   JSR   dsub
   LDA   HARGS+16
   LDY   #$25
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
   SEC
   LDA   SSP
   SBC   #$1E
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDY   #$4F
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$50
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$15
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$16
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   LDY   #$17
   LDA   (FP),Y
   LDY   #$05
   STA   (SSP),Y
   LDY   #$18
   LDA   (FP),Y
   LDY   #$06
   STA   (SSP),Y
   LDY   #$19
   LDA   (FP),Y
   LDY   #$07
   STA   (SSP),Y
   LDY   #$1A
   LDA   (FP),Y
   LDY   #$08
   STA   (SSP),Y
   LDY   #$1B
   LDA   (FP),Y
   LDY   #$09
   STA   (SSP),Y
   LDY   #$1C
   LDA   (FP),Y
   LDY   #$0A
   STA   (SSP),Y
   LDY   #$59
   LDA   (FP),Y
   LDY   #$0B
   STA   (SSP),Y
   LDY   #$5A
   LDA   (FP),Y
   LDY   #$0C
   STA   (SSP),Y
   LDY   #$1D
   LDA   (FP),Y
   LDY   #$0D
   STA   (SSP),Y
   LDY   #$1E
   LDA   (FP),Y
   LDY   #$0E
   STA   (SSP),Y
   LDY   #$1F
   LDA   (FP),Y
   LDY   #$0F
   STA   (SSP),Y
   LDY   #$20
   LDA   (FP),Y
   LDY   #$10
   STA   (SSP),Y
   LDY   #$21
   LDA   (FP),Y
   LDY   #$11
   STA   (SSP),Y
   LDY   #$22
   LDA   (FP),Y
   LDY   #$12
   STA   (SSP),Y
   LDY   #$23
   LDA   (FP),Y
   LDY   #$13
   STA   (SSP),Y
   LDY   #$24
   LDA   (FP),Y
   LDY   #$14
   STA   (SSP),Y
   LDY   #$63
   LDA   (FP),Y
   LDY   #$15
   STA   (SSP),Y
   LDY   #$64
   LDA   (FP),Y
   LDY   #$16
   STA   (SSP),Y
   LDY   #$25
   LDA   (FP),Y
   LDY   #$17
   STA   (SSP),Y
   LDY   #$26
   LDA   (FP),Y
   LDY   #$18
   STA   (SSP),Y
   LDY   #$27
   LDA   (FP),Y
   LDY   #$19
   STA   (SSP),Y
   LDY   #$28
   LDA   (FP),Y
   LDY   #$1A
   STA   (SSP),Y
   LDY   #$29
   LDA   (FP),Y
   LDY   #$1B
   STA   (SSP),Y
   LDY   #$2A
   LDA   (FP),Y
   LDY   #$1C
   STA   (SSP),Y
   LDY   #$2B
   LDA   (FP),Y
   LDY   #$1D
   STA   (SSP),Y
   LDY   #$2C
   LDA   (FP),Y
   LDY   #$1E
   STA   (SSP),Y
   JSR   fun
   LDY   #$2D
   STA   (FP),Y
   TXA
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$2F
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$30
   STA   (FP),Y
   LDY   #$13
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@1
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
   ADC   #$6C
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$4D
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
.if_end@1:
   LDY   #$2F
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@2
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
   ADC   #$6C
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$4D
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
.if_end@2:
.if_end@0:
   LDY   #$4F
   LDA   (FP),Y
   CLC
   ADC   #$02
   LDY   #$31
   STA   (FP),Y
   LDY   #$50
   LDA   (FP),Y
   ADC   #$00
   LDY   #$32
   STA   (FP),Y
   LDY   #$5A
   LDA   (FP),Y
   LDY   #$32
   CMP   (FP),Y
   BNE   .cmp_true@3
   LDY   #$59
   LDA   (FP),Y
   LDY   #$31
   CMP   (FP),Y
   BNE   .cmp_true@3
   LDA   #$00
   JMP   .cmp_end@4
.cmp_true@3:
   LDA   #$01
.cmp_end@4:
   LDY   #$33
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@3
   LDX   #$00
   LDA   #$02

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$6C
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$4D
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
.if_end@3:
   LDY   #$4F
   LDA   (FP),Y
   CLC
   ADC   #$04
   LDY   #$35
   STA   (FP),Y
   LDY   #$50
   LDA   (FP),Y
   ADC   #$00
   LDY   #$36
   STA   (FP),Y
   LDY   #$64
   LDA   (FP),Y
   LDY   #$36
   CMP   (FP),Y
   BNE   .cmp_true@6
   LDY   #$63
   LDA   (FP),Y
   LDY   #$35
   CMP   (FP),Y
   BNE   .cmp_true@6
   LDA   #$00
   JMP   .cmp_end@7
.cmp_true@6:
   LDA   #$01
.cmp_end@7:
   LDY   #$37
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@4
   LDX   #$00
   LDA   #$03

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$6C
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$4D
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
.if_end@4:
   LDY   #$65
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
   LDA   #$00
   STA   HARGS+8
   LDY   #$00
   STY   HARGS+9
   STY   HARGS+10
   STY   HARGS+11
   STY   HARGS+12
   STA   HARGS+13
   LDA   #$10
   STA   HARGS+14
   LDA   #$40
   STA   HARGS+15
   JSR   dsub
   LDA   HARGS+16
   LDY   #$39
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
   LDY   #$58
   LDA   (FP),Y
   LDY   #$40
   CMP   (FP),Y
   BNE   .cmp_true@9
   LDY   #$57
   LDA   (FP),Y
   LDY   #$3F
   CMP   (FP),Y
   BNE   .cmp_true@9
   LDY   #$56
   LDA   (FP),Y
   LDY   #$3E
   CMP   (FP),Y
   BNE   .cmp_true@9
   LDY   #$55
   LDA   (FP),Y
   LDY   #$3D
   CMP   (FP),Y
   BNE   .cmp_true@9
   LDY   #$54
   LDA   (FP),Y
   LDY   #$3C
   CMP   (FP),Y
   BNE   .cmp_true@9
   LDY   #$53
   LDA   (FP),Y
   LDY   #$3B
   CMP   (FP),Y
   BNE   .cmp_true@9
   LDY   #$52
   LDA   (FP),Y
   LDY   #$3A
   CMP   (FP),Y
   BNE   .cmp_true@9
   LDY   #$51
   LDA   (FP),Y
   LDY   #$39
   CMP   (FP),Y
   BNE   .cmp_true@9
   LDA   #$00
   JMP   .cmp_end@10
.cmp_true@9:
   LDA   #$01
.cmp_end@10:
   LDY   #$41
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@5
   LDX   #$00
   LDA   #$0B

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$6C
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$4D
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
.if_end@5:
   LDY   #$65
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
   LDA   #$00
   STA   HARGS+8
   LDY   #$00
   STY   HARGS+9
   STY   HARGS+10
   STY   HARGS+11
   STY   HARGS+12
   STY   HARGS+13
   STA   HARGS+14
   LDA   #$40
   STA   HARGS+15
   JSR   dsub
   LDA   HARGS+16
   LDY   #$43
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
   LDY   #$62
   LDA   (FP),Y
   LDY   #$4A
   CMP   (FP),Y
   BNE   .cmp_true@12
   LDY   #$61
   LDA   (FP),Y
   LDY   #$49
   CMP   (FP),Y
   BNE   .cmp_true@12
   LDY   #$60
   LDA   (FP),Y
   LDY   #$48
   CMP   (FP),Y
   BNE   .cmp_true@12
   LDY   #$5F
   LDA   (FP),Y
   LDY   #$47
   CMP   (FP),Y
   BNE   .cmp_true@12
   LDY   #$5E
   LDA   (FP),Y
   LDY   #$46
   CMP   (FP),Y
   BNE   .cmp_true@12
   LDY   #$5D
   LDA   (FP),Y
   LDY   #$45
   CMP   (FP),Y
   BNE   .cmp_true@12
   LDY   #$5C
   LDA   (FP),Y
   LDY   #$44
   CMP   (FP),Y
   BNE   .cmp_true@12
   LDY   #$5B
   LDA   (FP),Y
   LDY   #$43
   CMP   (FP),Y
   BNE   .cmp_true@12
   LDA   #$00
   JMP   .cmp_end@13
.cmp_true@12:
   LDA   #$01
.cmp_end@13:
   LDY   #$4B
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@6
   LDX   #$00
   LDA   #$0C

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$6C
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
.if_end@6:
   LDX   #$00
   TXA

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$6C
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$4D
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

   SEC
   LDA   SSP
   SBC   #$1E
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
   INY
   STA   (SSP),Y
   INY
   STA   (SSP),Y
   INY
   STA   (SSP),Y
   INY
   STA   (SSP),Y
   INY
   STA   (SSP),Y
   LDA   #$40
   INY
   STA   (SSP),Y
   LDA   #$03
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
   INY
   STA   (SSP),Y
   INY
   STA   (SSP),Y
   INY
   STA   (SSP),Y
   LDA   #$10
   INY
   STA   (SSP),Y
   LDA   #$40
   INY
   STA   (SSP),Y
   LDA   #$05
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
   INY
   STA   (SSP),Y
   INY
   STA   (SSP),Y
   INY
   STA   (SSP),Y
   LDA   #$18
   INY
   STA   (SSP),Y
   LDA   #$40
   INY
   STA   (SSP),Y
   JSR   fun
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
