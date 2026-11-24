SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24

main:
   SUBROUTINE

   ; prologue: 0 arg bytes, 154 local bytes
   SEC
   LDA   SSP
   SBC   #$9C
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$9B
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
.loop@0_start:
   LDY   #$01
   LDA   (FP),Y
   SEC
   SBC   #$64
   INY
   LDA   (FP),Y
   SBC   #$00
   BVC   .cmp_novf@0
   EOR   #$80
.cmp_novf@0:
   ASL
   LDA   #$00
   ROL
   LDY   #$03
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@3
   JMP   .loop@0_break
.lb_skip@3:
   LDA   #$00
   LDY   #$05
   STA   (FP),Y
   INY
   STA   (FP),Y
.loop@1_start:
   LDY   #$05
   LDA   (FP),Y
   SEC
   SBC   #$05
   INY
   LDA   (FP),Y
   SBC   #$00
   BVC   .cmp_novf@1
   EOR   #$80
.cmp_novf@1:
   ASL
   LDA   #$00
   ROL
   LDY   #$07
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@2
   JMP   .loop@1_break
.lb_skip@2:
   LDY   #$01
   LDA   (FP),Y
   STA   HARGS
   INY
   LDA   (FP),Y
   STA   HARGS+1
   LDA   #$05
   STA   HARGS+2
   LDA   #$00
   STA   HARGS+3
   JSR   mul16
   LDA   HARGS+4
   LDY   #$09
   STA   (FP),Y
   LDA   HARGS+5
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   LDY   #$05
   ADC   (FP),Y
   LDY   #$0B
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$06
   ADC   (FP),Y
   LDY   #$0C
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
   BMI   .sx_neg@2
   LDA   #$00
   JMP   .sx_done@3
.sx_neg@2:
   LDA   #$FF
.sx_done@3:
   LDY   #$0F
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #<x
   INY
   STA   (FP),Y
   LDA   #>x
   INY
   STA   (FP),Y
   LDY   #$01
   LDA   (FP),Y
   STA   HARGS
   INY
   LDA   (FP),Y
   STA   HARGS+1
   LDA   #$14
   STA   HARGS+2
   LDA   #$00
   STA   HARGS+3
   JSR   mul16
   LDA   HARGS+4
   LDY   #$13
   STA   (FP),Y
   LDA   HARGS+5
   INY
   STA   (FP),Y
   LDY   #$11
   LDA   (FP),Y
   CLC
   LDY   #$13
   ADC   (FP),Y
   LDY   #$15
   STA   (FP),Y
   LDY   #$12
   LDA   (FP),Y
   LDY   #$14
   ADC   (FP),Y
   LDY   #$16
   STA   (FP),Y
   LDY   #$05
   LDA   (FP),Y
   LDY   #$17
   STA   (FP),Y
   STA   HARGS
   LDA   #$04
   STA   HARGS+1
   JSR   mul8
   LDA   HARGS+2
   LDY   #$18
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$15
   LDA   (FP),Y
   CLC
   LDY   #$19
   ADC   (FP),Y
   LDY   #$1B
   STA   (FP),Y
   LDY   #$16
   LDA   (FP),Y
   LDY   #$1A
   ADC   (FP),Y
   LDY   #$1C
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
   LDY   #$0F
   LDA   (FP),Y
   LDY   #$02
   STA   (DPTR),Y
   LDY   #$10
   LDA   (FP),Y
   LDY   #$03
   STA   (DPTR),Y
   LDY   #$05
   LDA   (FP),Y
   CLC
   ADC   #$01
   LDY   #$1D
   STA   (FP),Y
   LDY   #$06
   LDA   (FP),Y
   ADC   #$00
   LDY   #$1E
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$05
   STA   (FP),Y
   LDY   #$1E
   LDA   (FP),Y
   LDY   #$06
   STA   (FP),Y
   JMP   .loop@1_start
.loop@1_break:
   LDY   #$01
   LDA   (FP),Y
   CLC
   ADC   #$01
   LDY   #$1F
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   ADC   #$00
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
   JMP   .loop@0_start
.loop@0_break:
   LDA   #<x
   LDY   #$21
   STA   (FP),Y
   LDA   #>x
   INY
   STA   (FP),Y
   LDA   #$14
   STA   HARGS
   LDA   #$00
   LDX   #$08
   LSR   HARGS
.mul_loop@4:
   BCC   .mul_skip@5
   CLC
   ADC   #$14
.mul_skip@5:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@4
   LDY   #$24
   STA   (FP),Y
   LDA   HARGS
   DEY
   STA   (FP),Y
   LDY   #$21
   LDA   (FP),Y
   CLC
   LDY   #$23
   ADC   (FP),Y
   LDY   #$25
   STA   (FP),Y
   LDY   #$22
   LDA   (FP),Y
   LDY   #$24
   ADC   (FP),Y
   LDY   #$26
   STA   (FP),Y
   LDA   #$03
   STA   HARGS
   LDA   #$00
   LDX   #$08
   LSR   HARGS
.mul_loop@6:
   BCC   .mul_skip@7
   CLC
   ADC   #$04
.mul_skip@7:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@6
   LDY   #$28
   STA   (FP),Y
   LDA   HARGS
   DEY
   STA   (FP),Y
   LDY   #$25
   LDA   (FP),Y
   CLC
   LDY   #$27
   ADC   (FP),Y
   LDY   #$29
   STA   (FP),Y
   LDY   #$26
   LDA   (FP),Y
   LDY   #$28
   ADC   (FP),Y
   LDY   #$2A
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$2B
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$2C
   STA   (FP),Y
   LDY   #$02
   LDA   (DPTR),Y
   LDY   #$2D
   STA   (FP),Y
   LDY   #$03
   LDA   (DPTR),Y
   LDY   #$2E
   STA   (FP),Y
   LDA   #<x
   INY
   STA   (FP),Y
   LDA   #>x
   INY
   STA   (FP),Y
   LDA   #$14
   STA   HARGS
   LDA   #$00
   LDX   #$08
   LSR   HARGS
.mul_loop@8:
   BCC   .mul_skip@9
   CLC
   ADC   #$14
.mul_skip@9:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@8
   LDY   #$32
   STA   (FP),Y
   LDA   HARGS
   DEY
   STA   (FP),Y
   LDY   #$2F
   LDA   (FP),Y
   CLC
   LDY   #$31
   ADC   (FP),Y
   LDY   #$33
   STA   (FP),Y
   LDY   #$30
   LDA   (FP),Y
   LDY   #$32
   ADC   (FP),Y
   LDY   #$34
   STA   (FP),Y
   LDA   #$03
   STA   HARGS
   LDA   #$00
   LDX   #$08
   LSR   HARGS
.mul_loop@10:
   BCC   .mul_skip@11
   CLC
   ADC   #$04
.mul_skip@11:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@10
   LDY   #$36
   STA   (FP),Y
   LDA   HARGS
   DEY
   STA   (FP),Y
   LDY   #$33
   LDA   (FP),Y
   CLC
   LDY   #$35
   ADC   (FP),Y
   LDY   #$37
   STA   (FP),Y
   LDY   #$34
   LDA   (FP),Y
   LDY   #$36
   ADC   (FP),Y
   LDY   #$38
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$39
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$3A
   STA   (FP),Y
   LDY   #$02
   LDA   (DPTR),Y
   LDY   #$3B
   STA   (FP),Y
   LDY   #$03
   LDA   (DPTR),Y
   LDY   #$3C
   STA   (FP),Y
   LDY   #$2E
   LDA   (FP),Y
   LDY   #$3C
   CMP   (FP),Y
   BNE   .cmp_true@12
   LDY   #$2D
   LDA   (FP),Y
   LDY   #$3B
   CMP   (FP),Y
   BNE   .cmp_true@12
   LDY   #$2C
   LDA   (FP),Y
   LDY   #$3A
   CMP   (FP),Y
   BNE   .cmp_true@12
   LDY   #$2B
   LDA   (FP),Y
   LDY   #$39
   CMP   (FP),Y
   BNE   .cmp_true@12
   LDA   #$00
   JMP   .cmp_end@13
.cmp_true@12:
   LDA   #$01
.cmp_end@13:
   LDY   #$3D
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
   LDA   #$01

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$9C
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$9B
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
   LDA   #<x
   LDY   #$3F
   STA   (FP),Y
   LDA   #>x
   INY
   STA   (FP),Y
   LDA   #$5A
   STA   HARGS
   LDA   #$00
   LDX   #$08
   LSR   HARGS
.mul_loop@15:
   BCC   .mul_skip@16
   CLC
   ADC   #$14
.mul_skip@16:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@15
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
   LDA   #$03
   STA   HARGS
   LDA   #$00
   LDX   #$08
   LSR   HARGS
.mul_loop@17:
   BCC   .mul_skip@18
   CLC
   ADC   #$04
.mul_skip@18:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@17
   LDY   #$46
   STA   (FP),Y
   LDA   HARGS
   DEY
   STA   (FP),Y
   LDY   #$43
   LDA   (FP),Y
   CLC
   LDY   #$45
   ADC   (FP),Y
   LDY   #$47
   STA   (FP),Y
   LDY   #$44
   LDA   (FP),Y
   LDY   #$46
   ADC   (FP),Y
   LDY   #$48
   STA   (FP),Y
   LDA   #<x
   INY
   STA   (FP),Y
   LDA   #>x
   INY
   STA   (FP),Y
   LDA   #$5A
   STA   HARGS
   LDA   #$00
   LDX   #$08
   LSR   HARGS
.mul_loop@19:
   BCC   .mul_skip@20
   CLC
   ADC   #$14
.mul_skip@20:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@19
   LDY   #$4C
   STA   (FP),Y
   LDA   HARGS
   DEY
   STA   (FP),Y
   LDY   #$49
   LDA   (FP),Y
   CLC
   LDY   #$4B
   ADC   (FP),Y
   LDY   #$4D
   STA   (FP),Y
   LDY   #$4A
   LDA   (FP),Y
   LDY   #$4C
   ADC   (FP),Y
   LDY   #$4E
   STA   (FP),Y
   LDA   #$03
   STA   HARGS
   LDA   #$00
   LDX   #$08
   LSR   HARGS
.mul_loop@21:
   BCC   .mul_skip@22
   CLC
   ADC   #$04
.mul_skip@22:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@21
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
   LDY   #$48
   LDA   (FP),Y
   LDY   #$52
   CMP   (FP),Y
   BNE   .cmp_true@23
   LDY   #$47
   LDA   (FP),Y
   LDY   #$51
   CMP   (FP),Y
   BNE   .cmp_true@23
   LDA   #$00
   JMP   .cmp_end@24
.cmp_true@23:
   LDA   #$01
.cmp_end@24:
   LDY   #$53
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@1
   LDX   #$00
   LDA   #$02

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$9C
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$9B
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
   LDA   #$00
   LDY   #$55
   STA   (FP),Y
   INY
   STA   (FP),Y
.loop@2_start:
   LDY   #$55
   LDA   (FP),Y
   SEC
   SBC   #$64
   INY
   LDA   (FP),Y
   SBC   #$00
   BVC   .cmp_novf@26
   EOR   #$80
.cmp_novf@26:
   ASL
   LDA   #$00
   ROL
   LDY   #$57
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@1
   JMP   .loop@2_break
.lb_skip@1:
   LDA   #$00
   LDY   #$59
   STA   (FP),Y
   INY
   STA   (FP),Y
.loop@3_start:
   LDY   #$59
   LDA   (FP),Y
   SEC
   SBC   #$05
   INY
   LDA   (FP),Y
   SBC   #$00
   BVC   .cmp_novf@27
   EOR   #$80
.cmp_novf@27:
   ASL
   LDA   #$00
   ROL
   LDY   #$5B
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@0
   JMP   .loop@3_break
.lb_skip@0:
   LDA   #<x
   LDY   #$5D
   STA   (FP),Y
   LDA   #>x
   INY
   STA   (FP),Y
   LDY   #$55
   LDA   (FP),Y
   STA   HARGS
   INY
   LDA   (FP),Y
   STA   HARGS+1
   LDA   #$14
   STA   HARGS+2
   LDA   #$00
   STA   HARGS+3
   JSR   mul16
   LDA   HARGS+4
   LDY   #$5F
   STA   (FP),Y
   LDA   HARGS+5
   INY
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
   LDY   #$59
   LDA   (FP),Y
   STA   HARGS
   INY
   LDA   (FP),Y
   STA   HARGS+1
   LDA   #$04
   STA   HARGS+2
   LDA   #$00
   STA   HARGS+3
   JSR   mul16
   LDA   HARGS+4
   LDY   #$63
   STA   (FP),Y
   LDA   HARGS+5
   INY
   STA   (FP),Y
   LDY   #$61
   LDA   (FP),Y
   CLC
   LDY   #$63
   ADC   (FP),Y
   LDY   #$65
   STA   (FP),Y
   LDY   #$62
   LDA   (FP),Y
   LDY   #$64
   ADC   (FP),Y
   LDY   #$66
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$67
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$68
   STA   (FP),Y
   LDY   #$02
   LDA   (DPTR),Y
   LDY   #$69
   STA   (FP),Y
   LDY   #$03
   LDA   (DPTR),Y
   LDY   #$6A
   STA   (FP),Y
   LDA   #<x
   INY
   STA   (FP),Y
   LDA   #>x
   INY
   STA   (FP),Y
   LDY   #$55
   LDA   (FP),Y
   STA   HARGS
   INY
   LDA   (FP),Y
   STA   HARGS+1
   LDA   #$14
   STA   HARGS+2
   LDA   #$00
   STA   HARGS+3
   JSR   mul16
   LDA   HARGS+4
   LDY   #$6D
   STA   (FP),Y
   LDA   HARGS+5
   INY
   STA   (FP),Y
   LDY   #$6B
   LDA   (FP),Y
   CLC
   LDY   #$6D
   ADC   (FP),Y
   LDY   #$6F
   STA   (FP),Y
   LDY   #$6C
   LDA   (FP),Y
   LDY   #$6E
   ADC   (FP),Y
   LDY   #$70
   STA   (FP),Y
   LDY   #$59
   LDA   (FP),Y
   LDY   #$71
   STA   (FP),Y
   STA   HARGS
   LDA   #$04
   STA   HARGS+1
   JSR   mul8
   LDA   HARGS+2
   LDY   #$72
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$6F
   LDA   (FP),Y
   CLC
   LDY   #$73
   ADC   (FP),Y
   LDY   #$75
   STA   (FP),Y
   LDY   #$70
   LDA   (FP),Y
   LDY   #$74
   ADC   (FP),Y
   LDY   #$76
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$77
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$78
   STA   (FP),Y
   LDY   #$02
   LDA   (DPTR),Y
   LDY   #$79
   STA   (FP),Y
   LDY   #$03
   LDA   (DPTR),Y
   LDY   #$7A
   STA   (FP),Y
   LDY   #$6A
   LDA   (FP),Y
   LDY   #$7A
   CMP   (FP),Y
   BNE   .cmp_true@28
   LDY   #$69
   LDA   (FP),Y
   LDY   #$79
   CMP   (FP),Y
   BNE   .cmp_true@28
   LDY   #$68
   LDA   (FP),Y
   LDY   #$78
   CMP   (FP),Y
   BNE   .cmp_true@28
   LDY   #$67
   LDA   (FP),Y
   LDY   #$77
   CMP   (FP),Y
   BNE   .cmp_true@28
   LDA   #$00
   JMP   .cmp_end@29
.cmp_true@28:
   LDA   #$01
.cmp_end@29:
   LDY   #$7B
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@2
   LDX   #$00
   LDA   #$03

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$9C
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$9B
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
   LDY   #$59
   LDA   (FP),Y
   CLC
   ADC   #$01
   LDY   #$7D
   STA   (FP),Y
   LDY   #$5A
   LDA   (FP),Y
   ADC   #$00
   LDY   #$7E
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$59
   STA   (FP),Y
   LDY   #$7E
   LDA   (FP),Y
   LDY   #$5A
   STA   (FP),Y
   JMP   .loop@3_start
.loop@3_break:
   LDY   #$55
   LDA   (FP),Y
   CLC
   ADC   #$01
   LDY   #$7F
   STA   (FP),Y
   LDY   #$56
   LDA   (FP),Y
   ADC   #$00
   LDY   #$80
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$55
   STA   (FP),Y
   LDY   #$80
   LDA   (FP),Y
   LDY   #$56
   STA   (FP),Y
   JMP   .loop@2_start
.loop@2_break:
   LDA   #<x
   LDY   #$81
   STA   (FP),Y
   LDA   #>x
   INY
   STA   (FP),Y
   LDA   #$4B
   STA   HARGS
   LDA   #$00
   LDX   #$08
   LSR   HARGS
.mul_loop@31:
   BCC   .mul_skip@32
   CLC
   ADC   #$14
.mul_skip@32:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@31
   LDY   #$84
   STA   (FP),Y
   LDA   HARGS
   DEY
   STA   (FP),Y
   LDY   #$81
   LDA   (FP),Y
   CLC
   LDY   #$83
   ADC   (FP),Y
   LDY   #$85
   STA   (FP),Y
   LDY   #$82
   LDA   (FP),Y
   LDY   #$84
   ADC   (FP),Y
   LDY   #$86
   STA   (FP),Y
   LDA   #$04
   STA   HARGS
   LDA   #$00
   LDX   #$08
   LSR   HARGS
.mul_loop@33:
   BCC   .mul_skip@34
   CLC
   ADC   #$04
.mul_skip@34:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@33
   LDY   #$88
   STA   (FP),Y
   LDA   HARGS
   DEY
   STA   (FP),Y
   LDY   #$85
   LDA   (FP),Y
   CLC
   LDY   #$87
   ADC   (FP),Y
   LDY   #$89
   STA   (FP),Y
   LDY   #$86
   LDA   (FP),Y
   LDY   #$88
   ADC   (FP),Y
   LDY   #$8A
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDA   #$F0
   LDY   #$00
   STA   (DPTR),Y
   LDA   #$55
   INY
   STA   (DPTR),Y
   LDA   #$00
   INY
   STA   (DPTR),Y
   INY
   STA   (DPTR),Y
   LDA   #<x
   LDY   #$8B
   STA   (FP),Y
   LDA   #>x
   INY
   STA   (FP),Y
   LDA   #$4B
   STA   HARGS
   LDA   #$00
   LDX   #$08
   LSR   HARGS
.mul_loop@35:
   BCC   .mul_skip@36
   CLC
   ADC   #$14
.mul_skip@36:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@35
   LDY   #$8E
   STA   (FP),Y
   LDA   HARGS
   DEY
   STA   (FP),Y
   LDY   #$8B
   LDA   (FP),Y
   CLC
   LDY   #$8D
   ADC   (FP),Y
   LDY   #$8F
   STA   (FP),Y
   LDY   #$8C
   LDA   (FP),Y
   LDY   #$8E
   ADC   (FP),Y
   LDY   #$90
   STA   (FP),Y
   LDA   #$04
   STA   HARGS
   LDA   #$00
   LDX   #$08
   LSR   HARGS
.mul_loop@37:
   BCC   .mul_skip@38
   CLC
   ADC   #$04
.mul_skip@38:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@37
   LDY   #$92
   STA   (FP),Y
   LDA   HARGS
   DEY
   STA   (FP),Y
   LDY   #$8F
   LDA   (FP),Y
   CLC
   LDY   #$91
   ADC   (FP),Y
   LDY   #$93
   STA   (FP),Y
   LDY   #$90
   LDA   (FP),Y
   LDY   #$92
   ADC   (FP),Y
   LDY   #$94
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$95
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$96
   STA   (FP),Y
   LDY   #$02
   LDA   (DPTR),Y
   LDY   #$97
   STA   (FP),Y
   LDY   #$03
   LDA   (DPTR),Y
   LDY   #$98
   STA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@39
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@39
   DEY
   LDA   (FP),Y
   CMP   #$55
   BNE   .cmp_true@39
   DEY
   LDA   (FP),Y
   CMP   #$F0
   BNE   .cmp_true@39
   LDA   #$00
   JMP   .cmp_end@40
.cmp_true@39:
   LDA   #$01
.cmp_end@40:
   LDY   #$99
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
   LDA   #$04

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$9C
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
.if_end@3:
   LDX   #$00
   TXA

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$9C
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$9B
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

x:
   DS.B  2000
