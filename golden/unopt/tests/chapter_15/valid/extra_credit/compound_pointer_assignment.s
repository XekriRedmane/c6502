SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24

int_array:
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

   CLC
   LDA   FP
   ADC   #$01
   LDY   #$0D
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
   LDY   #$0D
   LDA   (FP),Y
   CLC
   ADC   #$02
   LDY   #$0F
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$10
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
   LDY   #$0D
   LDA   (FP),Y
   CLC
   ADC   #$04
   LDY   #$11
   STA   (FP),Y
   LDY   #$0E
   LDA   (FP),Y
   ADC   #$00
   LDY   #$12
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
   LDY   #$0D
   LDA   (FP),Y
   CLC
   ADC   #$06
   LDY   #$13
   STA   (FP),Y
   LDY   #$0E
   LDA   (FP),Y
   ADC   #$00
   LDY   #$14
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
   LDY   #$0D
   LDA   (FP),Y
   CLC
   ADC   #$08
   LDY   #$15
   STA   (FP),Y
   LDY   #$0E
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
   LDA   #$05
   LDY   #$00
   STA   (DPTR),Y
   LDA   #$00
   INY
   STA   (DPTR),Y
   LDY   #$0D
   LDA   (FP),Y
   CLC
   ADC   #$0A
   LDY   #$17
   STA   (FP),Y
   LDY   #$0E
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
   LDA   #$06
   LDY   #$00
   STA   (DPTR),Y
   LDA   #$00
   INY
   STA   (DPTR),Y
   CLC
   LDA   FP
   ADC   #$01
   LDY   #$19
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$1B
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$1C
   STA   (FP),Y
   LDA   #$05
   STA   HARGS
   LDA   #$00
   LDX   #$08
   LSR   HARGS
.mul_loop@0:
   BCC   .mul_skip@1
   CLC
   ADC   #$02
.mul_skip@1:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@0
   LDY   #$1E
   STA   (FP),Y
   LDA   HARGS
   DEY
   STA   (FP),Y
   LDY   #$1B
   LDA   (FP),Y
   CLC
   LDY   #$1D
   ADC   (FP),Y
   LDY   #$1F
   STA   (FP),Y
   LDY   #$1C
   LDA   (FP),Y
   LDY   #$1E
   ADC   (FP),Y
   LDY   #$20
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$1B
   STA   (FP),Y
   LDY   #$20
   LDA   (FP),Y
   LDY   #$1C
   STA   (FP),Y
   LDY   #$1F
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$21
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$22
   STA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@2
   DEY
   LDA   (FP),Y
   CMP   #$06
   BNE   .cmp_true@2
   LDA   #$00
   JMP   .cmp_end@3
.cmp_true@2:
   LDA   #$01
.cmp_end@3:
   LDY   #$23
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
   LDA   #$00
   STA   HARGS
   LDX   #$08
   LSR   HARGS
.mul_loop@5:
   BCC   .mul_skip@6
   CLC
   ADC   #$02
.mul_skip@6:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@5
   LDY   #$26
   STA   (FP),Y
   LDA   HARGS
   DEY
   STA   (FP),Y
   LDY   #$1B
   LDA   (FP),Y
   CLC
   LDY   #$25
   ADC   (FP),Y
   LDY   #$27
   STA   (FP),Y
   LDY   #$1C
   LDA   (FP),Y
   LDY   #$26
   ADC   (FP),Y
   LDY   #$28
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$29
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$2A
   STA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@7
   DEY
   LDA   (FP),Y
   CMP   #$06
   BNE   .cmp_true@7
   LDA   #$00
   JMP   .cmp_end@8
.cmp_true@7:
   LDA   #$01
.cmp_end@8:
   LDY   #$2B
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
   CLC
   LDA   FP
   ADC   #$01
   LDY   #$2F
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   LDA   #$05
   STA   HARGS
   LDA   #$00
   LDX   #$08
   LSR   HARGS
.mul_loop@10:
   BCC   .mul_skip@11
   CLC
   ADC   #$02
.mul_skip@11:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@10
   LDY   #$2E
   STA   (FP),Y
   LDA   HARGS
   DEY
   STA   (FP),Y
   LDY   #$2F
   LDA   (FP),Y
   CLC
   LDY   #$2D
   ADC   (FP),Y
   LDY   #$31
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$2E
   ADC   (FP),Y
   LDY   #$32
   STA   (FP),Y
   LDY   #$1C
   LDA   (FP),Y
   LDY   #$32
   CMP   (FP),Y
   BNE   .cmp_true@12
   LDY   #$1B
   LDA   (FP),Y
   LDY   #$31
   CMP   (FP),Y
   BNE   .cmp_true@12
   LDA   #$00
   JMP   .cmp_end@13
.cmp_true@12:
   LDA   #$01
.cmp_end@13:
   LDY   #$33
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
   LDA   #$03
   STA   HARGS
   LDA   #$00
   LDX   #$08
   LSR   HARGS
.mul_loop@15:
   BCC   .mul_skip@16
   CLC
   ADC   #$02
.mul_skip@16:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@15
   LDY   #$36
   STA   (FP),Y
   LDA   HARGS
   DEY
   STA   (FP),Y
   LDY   #$1B
   LDA   (FP),Y
   SEC
   LDY   #$35
   SBC   (FP),Y
   LDY   #$37
   STA   (FP),Y
   LDY   #$1C
   LDA   (FP),Y
   LDY   #$36
   SBC   (FP),Y
   LDY   #$38
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$1B
   STA   (FP),Y
   LDY   #$38
   LDA   (FP),Y
   LDY   #$1C
   STA   (FP),Y
   LDY   #$37
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
   CMP   #$00
   BNE   .cmp_true@17
   DEY
   LDA   (FP),Y
   CMP   #$03
   BNE   .cmp_true@17
   LDA   #$00
   JMP   .cmp_end@18
.cmp_true@17:
   LDA   #$01
.cmp_end@18:
   LDY   #$3B
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
.if_end@3:
   LDA   #$00
   STA   HARGS
   LDX   #$08
   LSR   HARGS
.mul_loop@20:
   BCC   .mul_skip@21
   CLC
   ADC   #$02
.mul_skip@21:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@20
   LDY   #$3E
   STA   (FP),Y
   LDA   HARGS
   DEY
   STA   (FP),Y
   LDY   #$1B
   LDA   (FP),Y
   CLC
   LDY   #$3D
   ADC   (FP),Y
   LDY   #$3F
   STA   (FP),Y
   LDY   #$1C
   LDA   (FP),Y
   LDY   #$3E
   ADC   (FP),Y
   LDY   #$40
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$41
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$42
   STA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@22
   DEY
   LDA   (FP),Y
   CMP   #$03
   BNE   .cmp_true@22
   LDA   #$00
   JMP   .cmp_end@23
.cmp_true@22:
   LDA   #$01
.cmp_end@23:
   LDY   #$43
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
   LDA   #$05

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
.if_end@4:
   CLC
   LDA   FP
   ADC   #$01
   LDY   #$47
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
.mul_loop@25:
   BCC   .mul_skip@26
   CLC
   ADC   #$02
.mul_skip@26:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@25
   LDY   #$46
   STA   (FP),Y
   LDA   HARGS
   DEY
   STA   (FP),Y
   LDY   #$47
   LDA   (FP),Y
   CLC
   LDY   #$45
   ADC   (FP),Y
   LDY   #$49
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$46
   ADC   (FP),Y
   LDY   #$4A
   STA   (FP),Y
   LDY   #$1C
   LDA   (FP),Y
   LDY   #$4A
   CMP   (FP),Y
   BNE   .cmp_true@27
   LDY   #$1B
   LDA   (FP),Y
   LDY   #$49
   CMP   (FP),Y
   BNE   .cmp_true@27
   LDA   #$00
   JMP   .cmp_end@28
.cmp_true@27:
   LDA   #$01
.cmp_end@28:
   LDY   #$4B
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
   LDA   #$06

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
.if_end@5:
   LDA   i
   SEC
   SBC   #$01
   LDY   #$4D
   STA   (FP),Y
   LDA   i+1
   SBC   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   HARGS
   INY
   LDA   (FP),Y
   STA   HARGS+1
   LDA   #$02
   STA   HARGS+2
   LDA   #$00
   STA   HARGS+3
   JSR   mul16
   LDA   HARGS+4
   LDY   #$4F
   STA   (FP),Y
   LDA   HARGS+5
   INY
   STA   (FP),Y
   LDY   #$1B
   LDA   (FP),Y
   CLC
   LDY   #$4F
   ADC   (FP),Y
   LDY   #$51
   STA   (FP),Y
   LDY   #$1C
   LDA   (FP),Y
   LDY   #$50
   ADC   (FP),Y
   LDY   #$52
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$1B
   STA   (FP),Y
   LDY   #$52
   LDA   (FP),Y
   LDY   #$1C
   STA   (FP),Y
   CLC
   LDA   FP
   ADC   #$01
   LDY   #$55
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   LDA   #$05
   STA   HARGS
   LDA   #$00
   LDX   #$08
   LSR   HARGS
.mul_loop@30:
   BCC   .mul_skip@31
   CLC
   ADC   #$02
.mul_skip@31:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@30
   LDY   #$54
   STA   (FP),Y
   LDA   HARGS
   DEY
   STA   (FP),Y
   LDY   #$55
   LDA   (FP),Y
   CLC
   LDY   #$53
   ADC   (FP),Y
   LDY   #$57
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$54
   ADC   (FP),Y
   LDY   #$58
   STA   (FP),Y
   LDY   #$52
   LDA   (FP),Y
   LDY   #$58
   CMP   (FP),Y
   BNE   .cmp_true@32
   LDY   #$51
   LDA   (FP),Y
   LDY   #$57
   CMP   (FP),Y
   BNE   .cmp_true@32
   LDA   #$00
   JMP   .cmp_end@33
.cmp_true@32:
   LDA   #$01
.cmp_end@33:
   LDY   #$59
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
   LDA   #$07

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
.if_end@6:
   LDY   #$1B
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$5B
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$5C
   STA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@35
   DEY
   LDA   (FP),Y
   CMP   #$06
   BNE   .cmp_true@35
   LDA   #$00
   JMP   .cmp_end@36
.cmp_true@35:
   LDA   #$01
.cmp_end@36:
   LDY   #$5D
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@7
   LDX   #$00
   LDA   #$08

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
.if_end@7:
   LDA   i
   LDY   #$5F
   STA   (FP),Y
   LDA   i+1
   INY
   STA   (FP),Y
   LDA   i+1
   BMI   .sx_neg@38
   LDA   #$00
   JMP   .sx_done@39
.sx_neg@38:
   LDA   #$FF
.sx_done@39:
   LDY   #$61
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$FF
   CLC
   LDY   #$5F
   ADC   (FP),Y
   LDY   #$63
   STA   (FP),Y
   LDA   #$FF
   LDY   #$60
   ADC   (FP),Y
   LDY   #$64
   STA   (FP),Y
   LDA   #$00
   LDY   #$61
   ADC   (FP),Y
   LDY   #$65
   STA   (FP),Y
   LDA   #$00
   LDY   #$62
   ADC   (FP),Y
   LDY   #$66
   STA   (FP),Y
   LDY   #$63
   LDA   (FP),Y
   LDY   #$67
   STA   (FP),Y
   LDY   #$64
   LDA   (FP),Y
   LDY   #$68
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   HARGS
   INY
   LDA   (FP),Y
   STA   HARGS+1
   LDA   #$02
   STA   HARGS+2
   LDA   #$00
   STA   HARGS+3
   JSR   mul16
   LDA   HARGS+4
   LDY   #$69
   STA   (FP),Y
   LDA   HARGS+5
   INY
   STA   (FP),Y
   LDY   #$1B
   LDA   (FP),Y
   SEC
   LDY   #$69
   SBC   (FP),Y
   LDY   #$6B
   STA   (FP),Y
   LDY   #$1C
   LDA   (FP),Y
   LDY   #$6A
   SBC   (FP),Y
   LDY   #$6C
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$1B
   STA   (FP),Y
   LDY   #$6C
   LDA   (FP),Y
   LDY   #$1C
   STA   (FP),Y
   CLC
   LDA   FP
   ADC   #$01
   LDY   #$6F
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
.mul_loop@40:
   BCC   .mul_skip@41
   CLC
   ADC   #$02
.mul_skip@41:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@40
   LDY   #$6E
   STA   (FP),Y
   LDA   HARGS
   DEY
   STA   (FP),Y
   LDY   #$6F
   LDA   (FP),Y
   CLC
   LDY   #$6D
   ADC   (FP),Y
   LDY   #$71
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$6E
   ADC   (FP),Y
   LDY   #$72
   STA   (FP),Y
   LDY   #$6C
   LDA   (FP),Y
   LDY   #$72
   CMP   (FP),Y
   BNE   .cmp_true@42
   LDY   #$6B
   LDA   (FP),Y
   LDY   #$71
   CMP   (FP),Y
   BNE   .cmp_true@42
   LDA   #$00
   JMP   .cmp_end@43
.cmp_true@42:
   LDA   #$01
.cmp_end@43:
   LDY   #$73
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@8
   LDX   #$00
   LDA   #$09

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
.if_end@8:
   LDY   #$1B
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$75
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$76
   STA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@45
   DEY
   LDA   (FP),Y
   CMP   #$03
   BNE   .cmp_true@45
   LDA   #$00
   JMP   .cmp_end@46
.cmp_true@45:
   LDA   #$01
.cmp_end@46:
   LDY   #$77
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@9
   LDX   #$00
   LDA   #$0A

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
.if_end@9:
   LDA   #$FF
   LDY   #$79
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$7F
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
   LDY   #$79
   LDA   (FP),Y
   SEC
   SBC   #$FE
   LDY   #$81
   STA   (FP),Y
   LDY   #$7A
   LDA   (FP),Y
   SBC   #$FF
   LDY   #$82
   STA   (FP),Y
   LDY   #$7B
   LDA   (FP),Y
   SBC   #$FF
   LDY   #$83
   STA   (FP),Y
   LDY   #$7C
   LDA   (FP),Y
   SBC   #$7F
   LDY   #$84
   STA   (FP),Y
   LDY   #$7D
   LDA   (FP),Y
   SBC   #$00
   LDY   #$85
   STA   (FP),Y
   LDY   #$7E
   LDA   (FP),Y
   SBC   #$00
   LDY   #$86
   STA   (FP),Y
   LDY   #$7F
   LDA   (FP),Y
   SBC   #$00
   LDY   #$87
   STA   (FP),Y
   LDY   #$80
   LDA   (FP),Y
   SBC   #$00
   LDY   #$88
   STA   (FP),Y
   LDY   #$81
   LDA   (FP),Y
   LDY   #$89
   STA   (FP),Y
   LDY   #$82
   LDA   (FP),Y
   LDY   #$8A
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   HARGS
   INY
   LDA   (FP),Y
   STA   HARGS+1
   LDA   #$02
   STA   HARGS+2
   LDA   #$00
   STA   HARGS+3
   JSR   mul16
   LDA   HARGS+4
   LDY   #$8B
   STA   (FP),Y
   LDA   HARGS+5
   INY
   STA   (FP),Y
   LDY   #$1B
   LDA   (FP),Y
   CLC
   LDY   #$8B
   ADC   (FP),Y
   LDY   #$8D
   STA   (FP),Y
   LDY   #$1C
   LDA   (FP),Y
   LDY   #$8C
   ADC   (FP),Y
   LDY   #$8E
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$1B
   STA   (FP),Y
   LDY   #$8E
   LDA   (FP),Y
   LDY   #$1C
   STA   (FP),Y
   CLC
   LDA   FP
   ADC   #$01
   LDY   #$91
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   LDA   #$03
   STA   HARGS
   LDA   #$00
   LDX   #$08
   LSR   HARGS
.mul_loop@48:
   BCC   .mul_skip@49
   CLC
   ADC   #$02
.mul_skip@49:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@48
   LDY   #$90
   STA   (FP),Y
   LDA   HARGS
   DEY
   STA   (FP),Y
   LDY   #$91
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
   LDY   #$8E
   LDA   (FP),Y
   LDY   #$94
   CMP   (FP),Y
   BNE   .cmp_true@50
   LDY   #$8D
   LDA   (FP),Y
   LDY   #$93
   CMP   (FP),Y
   BNE   .cmp_true@50
   LDA   #$00
   JMP   .cmp_end@51
.cmp_true@50:
   LDA   #$01
.cmp_end@51:
   LDY   #$95
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
   LDA   #$0B

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
.if_end@10:
   LDY   #$1B
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$97
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$98
   STA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@53
   DEY
   LDA   (FP),Y
   CMP   #$04
   BNE   .cmp_true@53
   LDA   #$00
   JMP   .cmp_end@54
.cmp_true@53:
   LDA   #$01
.cmp_end@54:
   LDY   #$99
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@11
   LDX   #$00
   LDA   #$0C

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
.if_end@11:
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

double_array:
   SUBROUTINE

   ; prologue: 0 arg bytes, 172 local bytes
   SEC
   LDA   SSP
   SBC   #$AE
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$AD
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   LDA   #<@3.arr
   LDY   #$01
   STA   (FP),Y
   LDA   #>@3.arr
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
   LDA   #$05
   STA   HARGS
   LDA   #$00
   LDX   #$08
   LSR   HARGS
.mul_loop@56:
   BCC   .mul_skip@57
   CLC
   ADC   #$08
.mul_skip@57:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@56
   LDY   #$06
   STA   (FP),Y
   LDA   HARGS
   DEY
   STA   (FP),Y
   LDY   #$03
   LDA   (FP),Y
   CLC
   LDY   #$05
   ADC   (FP),Y
   LDY   #$07
   STA   (FP),Y
   LDY   #$04
   LDA   (FP),Y
   LDY   #$06
   ADC   (FP),Y
   LDY   #$08
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$03
   STA   (FP),Y
   LDY   #$08
   LDA   (FP),Y
   LDY   #$04
   STA   (FP),Y
   LDY   #$07
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$09
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$0A
   STA   (FP),Y
   LDY   #$02
   LDA   (DPTR),Y
   LDY   #$0B
   STA   (FP),Y
   LDY   #$03
   LDA   (DPTR),Y
   LDY   #$0C
   STA   (FP),Y
   LDY   #$04
   LDA   (DPTR),Y
   LDY   #$0D
   STA   (FP),Y
   LDY   #$05
   LDA   (DPTR),Y
   LDY   #$0E
   STA   (FP),Y
   LDY   #$06
   LDA   (DPTR),Y
   LDY   #$0F
   STA   (FP),Y
   LDY   #$07
   LDA   (DPTR),Y
   LDY   #$10
   STA   (FP),Y
   CMP   #$40
   BNE   .cmp_true@58
   DEY
   LDA   (FP),Y
   CMP   #$18
   BNE   .cmp_true@58
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@58
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@58
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@58
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@58
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@58
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@58
   LDA   #$00
   JMP   .cmp_end@59
.cmp_true@58:
   LDA   #$01
.cmp_end@59:
   LDY   #$11
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@12
   LDX   #$00
   LDA   #$01

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$AE
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$AD
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
.if_end@12:
   LDA   #$00
   STA   HARGS
   LDX   #$08
   LSR   HARGS
.mul_loop@61:
   BCC   .mul_skip@62
   CLC
   ADC   #$08
.mul_skip@62:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@61
   LDY   #$14
   STA   (FP),Y
   LDA   HARGS
   DEY
   STA   (FP),Y
   LDY   #$03
   LDA   (FP),Y
   CLC
   LDY   #$13
   ADC   (FP),Y
   LDY   #$15
   STA   (FP),Y
   LDY   #$04
   LDA   (FP),Y
   LDY   #$14
   ADC   (FP),Y
   LDY   #$16
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$17
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$18
   STA   (FP),Y
   LDY   #$02
   LDA   (DPTR),Y
   LDY   #$19
   STA   (FP),Y
   LDY   #$03
   LDA   (DPTR),Y
   LDY   #$1A
   STA   (FP),Y
   LDY   #$04
   LDA   (DPTR),Y
   LDY   #$1B
   STA   (FP),Y
   LDY   #$05
   LDA   (DPTR),Y
   LDY   #$1C
   STA   (FP),Y
   LDY   #$06
   LDA   (DPTR),Y
   LDY   #$1D
   STA   (FP),Y
   LDY   #$07
   LDA   (DPTR),Y
   LDY   #$1E
   STA   (FP),Y
   CMP   #$40
   BNE   .cmp_true@63
   DEY
   LDA   (FP),Y
   CMP   #$18
   BNE   .cmp_true@63
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@63
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@63
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@63
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@63
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@63
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@63
   LDA   #$00
   JMP   .cmp_end@64
.cmp_true@63:
   LDA   #$01
.cmp_end@64:
   LDY   #$1F
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@13
   LDX   #$00
   LDA   #$02

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$AE
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$AD
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
.if_end@13:
   LDA   #<@3.arr
   LDY   #$21
   STA   (FP),Y
   LDA   #>@3.arr
   INY
   STA   (FP),Y
   LDA   #$05
   STA   HARGS
   LDA   #$00
   LDX   #$08
   LSR   HARGS
.mul_loop@66:
   BCC   .mul_skip@67
   CLC
   ADC   #$08
.mul_skip@67:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@66
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
   LDY   #$04
   LDA   (FP),Y
   LDY   #$26
   CMP   (FP),Y
   BNE   .cmp_true@68
   LDY   #$03
   LDA   (FP),Y
   LDY   #$25
   CMP   (FP),Y
   BNE   .cmp_true@68
   LDA   #$00
   JMP   .cmp_end@69
.cmp_true@68:
   LDA   #$01
.cmp_end@69:
   LDY   #$27
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@14
   LDX   #$00
   LDA   #$03

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$AE
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$AD
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
.if_end@14:
   LDA   #$03
   STA   HARGS
   LDA   #$00
   LDX   #$08
   LSR   HARGS
.mul_loop@71:
   BCC   .mul_skip@72
   CLC
   ADC   #$08
.mul_skip@72:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@71
   LDY   #$2A
   STA   (FP),Y
   LDA   HARGS
   DEY
   STA   (FP),Y
   LDY   #$03
   LDA   (FP),Y
   SEC
   LDY   #$29
   SBC   (FP),Y
   LDY   #$2B
   STA   (FP),Y
   LDY   #$04
   LDA   (FP),Y
   LDY   #$2A
   SBC   (FP),Y
   LDY   #$2C
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$03
   STA   (FP),Y
   LDY   #$2C
   LDA   (FP),Y
   LDY   #$04
   STA   (FP),Y
   LDY   #$2B
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$2D
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$2E
   STA   (FP),Y
   LDY   #$02
   LDA   (DPTR),Y
   LDY   #$2F
   STA   (FP),Y
   LDY   #$03
   LDA   (DPTR),Y
   LDY   #$30
   STA   (FP),Y
   LDY   #$04
   LDA   (DPTR),Y
   LDY   #$31
   STA   (FP),Y
   LDY   #$05
   LDA   (DPTR),Y
   LDY   #$32
   STA   (FP),Y
   LDY   #$06
   LDA   (DPTR),Y
   LDY   #$33
   STA   (FP),Y
   LDY   #$07
   LDA   (DPTR),Y
   LDY   #$34
   STA   (FP),Y
   CMP   #$40
   BNE   .cmp_true@73
   DEY
   LDA   (FP),Y
   CMP   #$08
   BNE   .cmp_true@73
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@73
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@73
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@73
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@73
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@73
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@73
   LDA   #$00
   JMP   .cmp_end@74
.cmp_true@73:
   LDA   #$01
.cmp_end@74:
   LDY   #$35
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@15
   LDX   #$00
   LDA   #$04

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$AE
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$AD
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
.if_end@15:
   LDA   #$00
   STA   HARGS
   LDX   #$08
   LSR   HARGS
.mul_loop@76:
   BCC   .mul_skip@77
   CLC
   ADC   #$08
.mul_skip@77:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@76
   LDY   #$38
   STA   (FP),Y
   LDA   HARGS
   DEY
   STA   (FP),Y
   LDY   #$03
   LDA   (FP),Y
   CLC
   LDY   #$37
   ADC   (FP),Y
   LDY   #$39
   STA   (FP),Y
   LDY   #$04
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
   LDY   #$02
   LDA   (DPTR),Y
   LDY   #$3D
   STA   (FP),Y
   LDY   #$03
   LDA   (DPTR),Y
   LDY   #$3E
   STA   (FP),Y
   LDY   #$04
   LDA   (DPTR),Y
   LDY   #$3F
   STA   (FP),Y
   LDY   #$05
   LDA   (DPTR),Y
   LDY   #$40
   STA   (FP),Y
   LDY   #$06
   LDA   (DPTR),Y
   LDY   #$41
   STA   (FP),Y
   LDY   #$07
   LDA   (DPTR),Y
   LDY   #$42
   STA   (FP),Y
   CMP   #$40
   BNE   .cmp_true@78
   DEY
   LDA   (FP),Y
   CMP   #$08
   BNE   .cmp_true@78
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@78
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@78
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@78
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@78
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@78
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@78
   LDA   #$00
   JMP   .cmp_end@79
.cmp_true@78:
   LDA   #$01
.cmp_end@79:
   LDY   #$43
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@16
   LDX   #$00
   LDA   #$05

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$AE
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$AD
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
   LDA   #<@3.arr
   LDY   #$45
   STA   (FP),Y
   LDA   #>@3.arr
   INY
   STA   (FP),Y
   LDA   #$02
   STA   HARGS
   LDA   #$00
   LDX   #$08
   LSR   HARGS
.mul_loop@81:
   BCC   .mul_skip@82
   CLC
   ADC   #$08
.mul_skip@82:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@81
   LDY   #$48
   STA   (FP),Y
   LDA   HARGS
   DEY
   STA   (FP),Y
   LDY   #$45
   LDA   (FP),Y
   CLC
   LDY   #$47
   ADC   (FP),Y
   LDY   #$49
   STA   (FP),Y
   LDY   #$46
   LDA   (FP),Y
   LDY   #$48
   ADC   (FP),Y
   LDY   #$4A
   STA   (FP),Y
   LDY   #$04
   LDA   (FP),Y
   LDY   #$4A
   CMP   (FP),Y
   BNE   .cmp_true@83
   LDY   #$03
   LDA   (FP),Y
   LDY   #$49
   CMP   (FP),Y
   BNE   .cmp_true@83
   LDA   #$00
   JMP   .cmp_end@84
.cmp_true@83:
   LDA   #$01
.cmp_end@84:
   LDY   #$4B
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@17
   LDX   #$00
   LDA   #$06

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$AE
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$AD
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
.if_end@17:
   LDA   i
   SEC
   SBC   #$01
   LDY   #$4D
   STA   (FP),Y
   LDA   i+1
   SBC   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   HARGS
   INY
   LDA   (FP),Y
   STA   HARGS+1
   LDA   #$08
   STA   HARGS+2
   LDA   #$00
   STA   HARGS+3
   JSR   mul16
   LDA   HARGS+4
   LDY   #$4F
   STA   (FP),Y
   LDA   HARGS+5
   INY
   STA   (FP),Y
   LDY   #$03
   LDA   (FP),Y
   CLC
   LDY   #$4F
   ADC   (FP),Y
   LDY   #$51
   STA   (FP),Y
   LDY   #$04
   LDA   (FP),Y
   LDY   #$50
   ADC   (FP),Y
   LDY   #$52
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$03
   STA   (FP),Y
   LDY   #$52
   LDA   (FP),Y
   LDY   #$04
   STA   (FP),Y
   LDA   #<@3.arr
   LDY   #$53
   STA   (FP),Y
   LDA   #>@3.arr
   INY
   STA   (FP),Y
   LDA   #$05
   STA   HARGS
   LDA   #$00
   LDX   #$08
   LSR   HARGS
.mul_loop@86:
   BCC   .mul_skip@87
   CLC
   ADC   #$08
.mul_skip@87:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@86
   LDY   #$56
   STA   (FP),Y
   LDA   HARGS
   DEY
   STA   (FP),Y
   LDY   #$53
   LDA   (FP),Y
   CLC
   LDY   #$55
   ADC   (FP),Y
   LDY   #$57
   STA   (FP),Y
   LDY   #$54
   LDA   (FP),Y
   LDY   #$56
   ADC   (FP),Y
   LDY   #$58
   STA   (FP),Y
   LDY   #$52
   LDA   (FP),Y
   LDY   #$58
   CMP   (FP),Y
   BNE   .cmp_true@88
   LDY   #$51
   LDA   (FP),Y
   LDY   #$57
   CMP   (FP),Y
   BNE   .cmp_true@88
   LDA   #$00
   JMP   .cmp_end@89
.cmp_true@88:
   LDA   #$01
.cmp_end@89:
   LDY   #$59
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@18
   LDX   #$00
   LDA   #$07

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$AE
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$AD
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
.if_end@18:
   LDY   #$03
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$5B
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$5C
   STA   (FP),Y
   LDY   #$02
   LDA   (DPTR),Y
   LDY   #$5D
   STA   (FP),Y
   LDY   #$03
   LDA   (DPTR),Y
   LDY   #$5E
   STA   (FP),Y
   LDY   #$04
   LDA   (DPTR),Y
   LDY   #$5F
   STA   (FP),Y
   LDY   #$05
   LDA   (DPTR),Y
   LDY   #$60
   STA   (FP),Y
   LDY   #$06
   LDA   (DPTR),Y
   LDY   #$61
   STA   (FP),Y
   LDY   #$07
   LDA   (DPTR),Y
   LDY   #$62
   STA   (FP),Y
   CMP   #$40
   BNE   .cmp_true@91
   DEY
   LDA   (FP),Y
   CMP   #$18
   BNE   .cmp_true@91
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@91
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@91
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@91
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@91
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@91
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@91
   LDA   #$00
   JMP   .cmp_end@92
.cmp_true@91:
   LDA   #$01
.cmp_end@92:
   LDY   #$63
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@19
   LDX   #$00
   LDA   #$08

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$AE
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$AD
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
.if_end@19:
   LDA   i
   LDY   #$65
   STA   (FP),Y
   LDA   i+1
   INY
   STA   (FP),Y
   LDA   i+1
   BMI   .sx_neg@94
   LDA   #$00
   JMP   .sx_done@95
.sx_neg@94:
   LDA   #$FF
.sx_done@95:
   LDY   #$67
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$FF
   CLC
   LDY   #$65
   ADC   (FP),Y
   LDY   #$69
   STA   (FP),Y
   LDA   #$FF
   LDY   #$66
   ADC   (FP),Y
   LDY   #$6A
   STA   (FP),Y
   LDA   #$00
   LDY   #$67
   ADC   (FP),Y
   LDY   #$6B
   STA   (FP),Y
   LDA   #$00
   LDY   #$68
   ADC   (FP),Y
   LDY   #$6C
   STA   (FP),Y
   LDY   #$69
   LDA   (FP),Y
   LDY   #$6D
   STA   (FP),Y
   LDY   #$6A
   LDA   (FP),Y
   LDY   #$6E
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   HARGS
   INY
   LDA   (FP),Y
   STA   HARGS+1
   LDA   #$08
   STA   HARGS+2
   LDA   #$00
   STA   HARGS+3
   JSR   mul16
   LDA   HARGS+4
   LDY   #$6F
   STA   (FP),Y
   LDA   HARGS+5
   INY
   STA   (FP),Y
   LDY   #$03
   LDA   (FP),Y
   SEC
   LDY   #$6F
   SBC   (FP),Y
   LDY   #$71
   STA   (FP),Y
   LDY   #$04
   LDA   (FP),Y
   LDY   #$70
   SBC   (FP),Y
   LDY   #$72
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$03
   STA   (FP),Y
   LDY   #$72
   LDA   (FP),Y
   LDY   #$04
   STA   (FP),Y
   LDA   #<@3.arr
   LDY   #$73
   STA   (FP),Y
   LDA   #>@3.arr
   INY
   STA   (FP),Y
   LDA   #$02
   STA   HARGS
   LDA   #$00
   LDX   #$08
   LSR   HARGS
.mul_loop@96:
   BCC   .mul_skip@97
   CLC
   ADC   #$08
.mul_skip@97:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@96
   LDY   #$76
   STA   (FP),Y
   LDA   HARGS
   DEY
   STA   (FP),Y
   LDY   #$73
   LDA   (FP),Y
   CLC
   LDY   #$75
   ADC   (FP),Y
   LDY   #$77
   STA   (FP),Y
   LDY   #$74
   LDA   (FP),Y
   LDY   #$76
   ADC   (FP),Y
   LDY   #$78
   STA   (FP),Y
   LDY   #$72
   LDA   (FP),Y
   LDY   #$78
   CMP   (FP),Y
   BNE   .cmp_true@98
   LDY   #$71
   LDA   (FP),Y
   LDY   #$77
   CMP   (FP),Y
   BNE   .cmp_true@98
   LDA   #$00
   JMP   .cmp_end@99
.cmp_true@98:
   LDA   #$01
.cmp_end@99:
   LDY   #$79
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@20
   LDX   #$00
   LDA   #$09

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$AE
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$AD
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
.if_end@20:
   LDY   #$03
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$7B
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$7C
   STA   (FP),Y
   LDY   #$02
   LDA   (DPTR),Y
   LDY   #$7D
   STA   (FP),Y
   LDY   #$03
   LDA   (DPTR),Y
   LDY   #$7E
   STA   (FP),Y
   LDY   #$04
   LDA   (DPTR),Y
   LDY   #$7F
   STA   (FP),Y
   LDY   #$05
   LDA   (DPTR),Y
   LDY   #$80
   STA   (FP),Y
   LDY   #$06
   LDA   (DPTR),Y
   LDY   #$81
   STA   (FP),Y
   LDY   #$07
   LDA   (DPTR),Y
   LDY   #$82
   STA   (FP),Y
   CMP   #$40
   BNE   .cmp_true@101
   DEY
   LDA   (FP),Y
   CMP   #$08
   BNE   .cmp_true@101
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@101
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@101
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@101
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@101
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@101
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@101
   LDA   #$00
   JMP   .cmp_end@102
.cmp_true@101:
   LDA   #$01
.cmp_end@102:
   LDY   #$83
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@21
   LDX   #$00
   LDA   #$0A

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$AE
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$AD
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
   LDA   #$FF
   LDY   #$85
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$7F
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
   LDY   #$85
   LDA   (FP),Y
   SEC
   SBC   #$FE
   LDY   #$8D
   STA   (FP),Y
   LDY   #$86
   LDA   (FP),Y
   SBC   #$FF
   LDY   #$8E
   STA   (FP),Y
   LDY   #$87
   LDA   (FP),Y
   SBC   #$FF
   LDY   #$8F
   STA   (FP),Y
   LDY   #$88
   LDA   (FP),Y
   SBC   #$7F
   LDY   #$90
   STA   (FP),Y
   LDY   #$89
   LDA   (FP),Y
   SBC   #$00
   LDY   #$91
   STA   (FP),Y
   LDY   #$8A
   LDA   (FP),Y
   SBC   #$00
   LDY   #$92
   STA   (FP),Y
   LDY   #$8B
   LDA   (FP),Y
   SBC   #$00
   LDY   #$93
   STA   (FP),Y
   LDY   #$8C
   LDA   (FP),Y
   SBC   #$00
   LDY   #$94
   STA   (FP),Y
   LDY   #$8D
   LDA   (FP),Y
   LDY   #$95
   STA   (FP),Y
   LDY   #$8E
   LDA   (FP),Y
   LDY   #$96
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   HARGS
   INY
   LDA   (FP),Y
   STA   HARGS+1
   LDA   #$08
   STA   HARGS+2
   LDA   #$00
   STA   HARGS+3
   JSR   mul16
   LDA   HARGS+4
   LDY   #$97
   STA   (FP),Y
   LDA   HARGS+5
   INY
   STA   (FP),Y
   LDY   #$03
   LDA   (FP),Y
   CLC
   LDY   #$97
   ADC   (FP),Y
   LDY   #$99
   STA   (FP),Y
   LDY   #$04
   LDA   (FP),Y
   LDY   #$98
   ADC   (FP),Y
   LDY   #$9A
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$03
   STA   (FP),Y
   LDY   #$9A
   LDA   (FP),Y
   LDY   #$04
   STA   (FP),Y
   LDA   #<@3.arr
   LDY   #$9B
   STA   (FP),Y
   LDA   #>@3.arr
   INY
   STA   (FP),Y
   LDA   #$03
   STA   HARGS
   LDA   #$00
   LDX   #$08
   LSR   HARGS
.mul_loop@104:
   BCC   .mul_skip@105
   CLC
   ADC   #$08
.mul_skip@105:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@104
   LDY   #$9E
   STA   (FP),Y
   LDA   HARGS
   DEY
   STA   (FP),Y
   LDY   #$9B
   LDA   (FP),Y
   CLC
   LDY   #$9D
   ADC   (FP),Y
   LDY   #$9F
   STA   (FP),Y
   LDY   #$9C
   LDA   (FP),Y
   LDY   #$9E
   ADC   (FP),Y
   LDY   #$A0
   STA   (FP),Y
   LDY   #$9A
   LDA   (FP),Y
   LDY   #$A0
   CMP   (FP),Y
   BNE   .cmp_true@106
   LDY   #$99
   LDA   (FP),Y
   LDY   #$9F
   CMP   (FP),Y
   BNE   .cmp_true@106
   LDA   #$00
   JMP   .cmp_end@107
.cmp_true@106:
   LDA   #$01
.cmp_end@107:
   LDY   #$A1
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
   LDA   #$0B

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$AE
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$AD
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
   LDY   #$03
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$A3
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$A4
   STA   (FP),Y
   LDY   #$02
   LDA   (DPTR),Y
   LDY   #$A5
   STA   (FP),Y
   LDY   #$03
   LDA   (DPTR),Y
   LDY   #$A6
   STA   (FP),Y
   LDY   #$04
   LDA   (DPTR),Y
   LDY   #$A7
   STA   (FP),Y
   LDY   #$05
   LDA   (DPTR),Y
   LDY   #$A8
   STA   (FP),Y
   LDY   #$06
   LDA   (DPTR),Y
   LDY   #$A9
   STA   (FP),Y
   LDY   #$07
   LDA   (DPTR),Y
   LDY   #$AA
   STA   (FP),Y
   CMP   #$40
   BNE   .cmp_true@109
   DEY
   LDA   (FP),Y
   CMP   #$10
   BNE   .cmp_true@109
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@109
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@109
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@109
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@109
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@109
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@109
   LDA   #$00
   JMP   .cmp_end@110
.cmp_true@109:
   LDA   #$01
.cmp_end@110:
   LDY   #$AB
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
   LDA   #$0C

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$AE
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
.if_end@23:
   LDX   #$00
   TXA

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$AE
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$AD
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

   JSR   int_array
   LDY   #$01
   STA   (FP),Y
   TXA
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
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@24
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
   ADC   #$0A
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$09
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
.if_end@24:
   JSR   double_array
   LDY   #$05
   STA   (FP),Y
   TXA
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$03
   STA   (FP),Y
   LDY   #$06
   LDA   (FP),Y
   LDY   #$04
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@25
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$0C
   LDY   #$07
   STA   (FP),Y
   LDY   #$04
   LDA   (FP),Y
   ADC   #$00
   LDY   #$08
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
   ADC   #$0A
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$09
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
.if_end@25:
   LDX   #$00
   TXA

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$0A
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$09
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

i:
   DC.W  $0004

@3.arr:
   DC.L  $00000000
   DC.L  $3FF00000
   DC.L  $00000000
   DC.L  $40000000
   DC.L  $00000000
   DC.L  $40080000
   DC.L  $00000000
   DC.L  $40100000
   DC.L  $00000000
   DC.L  $40140000
   DC.L  $00000000
   DC.L  $40180000
