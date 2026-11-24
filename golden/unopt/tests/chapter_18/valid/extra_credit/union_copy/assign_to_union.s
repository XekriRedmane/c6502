SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24

main:
   SUBROUTINE

   ; prologue: 0 arg bytes, 182 local bytes
   SEC
   LDA   SSP
   SBC   #$B8
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$B7
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
   LDY   #$19
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
   LDY   #$19
   LDA   (FP),Y
   CLC
   ADC   #$02
   LDY   #$1B
   STA   (FP),Y
   DEY
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
   LDA   #$02
   LDY   #$00
   STA   (DPTR),Y
   LDA   #$00
   INY
   STA   (DPTR),Y
   CLC
   LDA   FP
   ADC   #$1D
   LDY   #$35
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
   LDA   #$00
   LDY   #$00
   STA   (DPTR),Y
   INY
   STA   (DPTR),Y
   INY
   STA   (DPTR),Y
   INY
   STA   (DPTR),Y
   LDY   #$01
   LDA   (FP),Y
   LDY   #$1D
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   LDY   #$1E
   STA   (FP),Y
   LDY   #$03
   LDA   (FP),Y
   LDY   #$1F
   STA   (FP),Y
   LDY   #$04
   LDA   (FP),Y
   LDY   #$20
   STA   (FP),Y
   LDY   #$05
   LDA   (FP),Y
   LDY   #$21
   STA   (FP),Y
   LDY   #$06
   LDA   (FP),Y
   LDY   #$22
   STA   (FP),Y
   LDY   #$07
   LDA   (FP),Y
   LDY   #$23
   STA   (FP),Y
   LDY   #$08
   LDA   (FP),Y
   LDY   #$24
   STA   (FP),Y
   LDY   #$09
   LDA   (FP),Y
   LDY   #$25
   STA   (FP),Y
   LDY   #$0A
   LDA   (FP),Y
   LDY   #$26
   STA   (FP),Y
   LDY   #$0B
   LDA   (FP),Y
   LDY   #$27
   STA   (FP),Y
   LDY   #$0C
   LDA   (FP),Y
   LDY   #$28
   STA   (FP),Y
   LDY   #$0D
   LDA   (FP),Y
   LDY   #$29
   STA   (FP),Y
   LDY   #$0E
   LDA   (FP),Y
   LDY   #$2A
   STA   (FP),Y
   LDY   #$0F
   LDA   (FP),Y
   LDY   #$2B
   STA   (FP),Y
   LDY   #$10
   LDA   (FP),Y
   LDY   #$2C
   STA   (FP),Y
   LDY   #$11
   LDA   (FP),Y
   LDY   #$2D
   STA   (FP),Y
   LDY   #$12
   LDA   (FP),Y
   LDY   #$2E
   STA   (FP),Y
   LDY   #$13
   LDA   (FP),Y
   LDY   #$2F
   STA   (FP),Y
   LDY   #$14
   LDA   (FP),Y
   LDY   #$30
   STA   (FP),Y
   LDY   #$15
   LDA   (FP),Y
   LDY   #$31
   STA   (FP),Y
   LDY   #$16
   LDA   (FP),Y
   LDY   #$32
   STA   (FP),Y
   LDY   #$17
   LDA   (FP),Y
   LDY   #$33
   STA   (FP),Y
   LDY   #$18
   LDA   (FP),Y
   LDY   #$34
   STA   (FP),Y
   CLC
   LDA   FP
   ADC   #$1D
   LDY   #$37
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
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$39
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$3A
   STA   (FP),Y
   CMP   #$00
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
   LDY   #$3B
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
   ADC   #$B8
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$B7
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
   CLC
   LDA   FP
   ADC   #$1D
   LDY   #$3D
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$02
   LDY   #$3F
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
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
   BNE   .cmp_true@3
   DEY
   LDA   (FP),Y
   CMP   #$02
   BNE   .cmp_true@3
   LDA   #$00
   JMP   .cmp_end@4
.cmp_true@3:
   LDA   #$01
.cmp_end@4:
   LDY   #$43
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
   ADC   #$B8
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$B7
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
   LDY   #$45
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
   LDA   #$34
   INY
   STA   (FP),Y
   LDA   #$40
   EOR   #$80
   INY
   STA   (FP),Y
   CLC
   LDA   FP
   ADC   #$01
   LDY   #$4F
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   LDA   #$00
   STA   HARGS
   LDX   #$08
   LSR   HARGS
.mul_loop@6:
   BCC   .mul_skip@7
   CLC
   ADC   #$08
.mul_skip@7:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@6
   LDY   #$4E
   STA   (FP),Y
   LDA   HARGS
   DEY
   STA   (FP),Y
   LDY   #$4F
   LDA   (FP),Y
   CLC
   LDY   #$4D
   ADC   (FP),Y
   LDY   #$51
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$4E
   ADC   (FP),Y
   LDY   #$52
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$45
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$46
   LDA   (FP),Y
   LDY   #$01
   STA   (DPTR),Y
   LDY   #$47
   LDA   (FP),Y
   LDY   #$02
   STA   (DPTR),Y
   LDY   #$48
   LDA   (FP),Y
   LDY   #$03
   STA   (DPTR),Y
   LDY   #$49
   LDA   (FP),Y
   LDY   #$04
   STA   (DPTR),Y
   LDY   #$4A
   LDA   (FP),Y
   LDY   #$05
   STA   (DPTR),Y
   LDY   #$4B
   LDA   (FP),Y
   LDY   #$06
   STA   (DPTR),Y
   LDY   #$4C
   LDA   (FP),Y
   LDY   #$07
   STA   (DPTR),Y
   LDA   #$00
   LDY   #$53
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
   LDA   #$3E
   INY
   STA   (FP),Y
   LDA   #$40
   EOR   #$80
   INY
   STA   (FP),Y
   CLC
   LDA   FP
   ADC   #$01
   LDY   #$5D
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
.mul_loop@8:
   BCC   .mul_skip@9
   CLC
   ADC   #$08
.mul_skip@9:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@8
   LDY   #$5C
   STA   (FP),Y
   LDA   HARGS
   DEY
   STA   (FP),Y
   LDY   #$5D
   LDA   (FP),Y
   CLC
   LDY   #$5B
   ADC   (FP),Y
   LDY   #$5F
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$5C
   ADC   (FP),Y
   LDY   #$60
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$53
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$54
   LDA   (FP),Y
   LDY   #$01
   STA   (DPTR),Y
   LDY   #$55
   LDA   (FP),Y
   LDY   #$02
   STA   (DPTR),Y
   LDY   #$56
   LDA   (FP),Y
   LDY   #$03
   STA   (DPTR),Y
   LDY   #$57
   LDA   (FP),Y
   LDY   #$04
   STA   (DPTR),Y
   LDY   #$58
   LDA   (FP),Y
   LDY   #$05
   STA   (DPTR),Y
   LDY   #$59
   LDA   (FP),Y
   LDY   #$06
   STA   (DPTR),Y
   LDY   #$5A
   LDA   (FP),Y
   LDY   #$07
   STA   (DPTR),Y
   LDA   #$00
   LDY   #$61
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
   LDA   #$44
   INY
   STA   (FP),Y
   LDA   #$40
   EOR   #$80
   INY
   STA   (FP),Y
   CLC
   LDA   FP
   ADC   #$01
   LDY   #$6B
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
.mul_loop@10:
   BCC   .mul_skip@11
   CLC
   ADC   #$08
.mul_skip@11:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@10
   LDY   #$6A
   STA   (FP),Y
   LDA   HARGS
   DEY
   STA   (FP),Y
   LDY   #$6B
   LDA   (FP),Y
   CLC
   LDY   #$69
   ADC   (FP),Y
   LDY   #$6D
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$6A
   ADC   (FP),Y
   LDY   #$6E
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$61
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$62
   LDA   (FP),Y
   LDY   #$01
   STA   (DPTR),Y
   LDY   #$63
   LDA   (FP),Y
   LDY   #$02
   STA   (DPTR),Y
   LDY   #$64
   LDA   (FP),Y
   LDY   #$03
   STA   (DPTR),Y
   LDY   #$65
   LDA   (FP),Y
   LDY   #$04
   STA   (DPTR),Y
   LDY   #$66
   LDA   (FP),Y
   LDY   #$05
   STA   (DPTR),Y
   LDY   #$67
   LDA   (FP),Y
   LDY   #$06
   STA   (DPTR),Y
   LDY   #$68
   LDA   (FP),Y
   LDY   #$07
   STA   (DPTR),Y
   LDY   #$01
   LDA   (FP),Y
   LDY   #$1D
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   LDY   #$1E
   STA   (FP),Y
   LDY   #$03
   LDA   (FP),Y
   LDY   #$1F
   STA   (FP),Y
   LDY   #$04
   LDA   (FP),Y
   LDY   #$20
   STA   (FP),Y
   LDY   #$05
   LDA   (FP),Y
   LDY   #$21
   STA   (FP),Y
   LDY   #$06
   LDA   (FP),Y
   LDY   #$22
   STA   (FP),Y
   LDY   #$07
   LDA   (FP),Y
   LDY   #$23
   STA   (FP),Y
   LDY   #$08
   LDA   (FP),Y
   LDY   #$24
   STA   (FP),Y
   LDY   #$09
   LDA   (FP),Y
   LDY   #$25
   STA   (FP),Y
   LDY   #$0A
   LDA   (FP),Y
   LDY   #$26
   STA   (FP),Y
   LDY   #$0B
   LDA   (FP),Y
   LDY   #$27
   STA   (FP),Y
   LDY   #$0C
   LDA   (FP),Y
   LDY   #$28
   STA   (FP),Y
   LDY   #$0D
   LDA   (FP),Y
   LDY   #$29
   STA   (FP),Y
   LDY   #$0E
   LDA   (FP),Y
   LDY   #$2A
   STA   (FP),Y
   LDY   #$0F
   LDA   (FP),Y
   LDY   #$2B
   STA   (FP),Y
   LDY   #$10
   LDA   (FP),Y
   LDY   #$2C
   STA   (FP),Y
   LDY   #$11
   LDA   (FP),Y
   LDY   #$2D
   STA   (FP),Y
   LDY   #$12
   LDA   (FP),Y
   LDY   #$2E
   STA   (FP),Y
   LDY   #$13
   LDA   (FP),Y
   LDY   #$2F
   STA   (FP),Y
   LDY   #$14
   LDA   (FP),Y
   LDY   #$30
   STA   (FP),Y
   LDY   #$15
   LDA   (FP),Y
   LDY   #$31
   STA   (FP),Y
   LDY   #$16
   LDA   (FP),Y
   LDY   #$32
   STA   (FP),Y
   LDY   #$17
   LDA   (FP),Y
   LDY   #$33
   STA   (FP),Y
   LDY   #$18
   LDA   (FP),Y
   LDY   #$34
   STA   (FP),Y
   CLC
   LDA   FP
   ADC   #$1D
   LDY   #$71
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   LDA   #$00
   STA   HARGS
   LDX   #$08
   LSR   HARGS
.mul_loop@12:
   BCC   .mul_skip@13
   CLC
   ADC   #$08
.mul_skip@13:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@12
   LDY   #$70
   STA   (FP),Y
   LDA   HARGS
   DEY
   STA   (FP),Y
   LDY   #$71
   LDA   (FP),Y
   CLC
   LDY   #$6F
   ADC   (FP),Y
   LDY   #$73
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$70
   ADC   (FP),Y
   LDY   #$74
   STA   (FP),Y
   DEY
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
   LDY   #$02
   LDA   (DPTR),Y
   LDY   #$77
   STA   (FP),Y
   LDY   #$03
   LDA   (DPTR),Y
   LDY   #$78
   STA   (FP),Y
   LDY   #$04
   LDA   (DPTR),Y
   LDY   #$79
   STA   (FP),Y
   LDY   #$05
   LDA   (DPTR),Y
   LDY   #$7A
   STA   (FP),Y
   LDY   #$06
   LDA   (DPTR),Y
   LDY   #$7B
   STA   (FP),Y
   LDY   #$07
   LDA   (DPTR),Y
   LDY   #$7C
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
   LDA   #$34
   INY
   STA   (FP),Y
   LDA   #$40
   EOR   #$80
   INY
   STA   (FP),Y
   LDY   #$7C
   LDA   (FP),Y
   LDY   #$84
   CMP   (FP),Y
   BNE   .cmp_true@14
   LDY   #$7B
   LDA   (FP),Y
   LDY   #$83
   CMP   (FP),Y
   BNE   .cmp_true@14
   LDY   #$7A
   LDA   (FP),Y
   LDY   #$82
   CMP   (FP),Y
   BNE   .cmp_true@14
   LDY   #$79
   LDA   (FP),Y
   LDY   #$81
   CMP   (FP),Y
   BNE   .cmp_true@14
   LDY   #$78
   LDA   (FP),Y
   LDY   #$80
   CMP   (FP),Y
   BNE   .cmp_true@14
   LDY   #$77
   LDA   (FP),Y
   LDY   #$7F
   CMP   (FP),Y
   BNE   .cmp_true@14
   LDY   #$76
   LDA   (FP),Y
   LDY   #$7E
   CMP   (FP),Y
   BNE   .cmp_true@14
   LDY   #$75
   LDA   (FP),Y
   LDY   #$7D
   CMP   (FP),Y
   BNE   .cmp_true@14
   LDA   #$00
   JMP   .cmp_end@15
.cmp_true@14:
   LDA   #$01
.cmp_end@15:
   LDY   #$85
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
   ADC   #$B8
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$B7
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
   CLC
   LDA   FP
   ADC   #$1D
   LDY   #$89
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
.mul_loop@17:
   BCC   .mul_skip@18
   CLC
   ADC   #$08
.mul_skip@18:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@17
   LDY   #$88
   STA   (FP),Y
   LDA   HARGS
   DEY
   STA   (FP),Y
   LDY   #$89
   LDA   (FP),Y
   CLC
   LDY   #$87
   ADC   (FP),Y
   LDY   #$8B
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$88
   ADC   (FP),Y
   LDY   #$8C
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$8D
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$8E
   STA   (FP),Y
   LDY   #$02
   LDA   (DPTR),Y
   LDY   #$8F
   STA   (FP),Y
   LDY   #$03
   LDA   (DPTR),Y
   LDY   #$90
   STA   (FP),Y
   LDY   #$04
   LDA   (DPTR),Y
   LDY   #$91
   STA   (FP),Y
   LDY   #$05
   LDA   (DPTR),Y
   LDY   #$92
   STA   (FP),Y
   LDY   #$06
   LDA   (DPTR),Y
   LDY   #$93
   STA   (FP),Y
   LDY   #$07
   LDA   (DPTR),Y
   LDY   #$94
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
   LDA   #$3E
   INY
   STA   (FP),Y
   LDA   #$40
   EOR   #$80
   INY
   STA   (FP),Y
   LDY   #$94
   LDA   (FP),Y
   LDY   #$9C
   CMP   (FP),Y
   BNE   .cmp_true@19
   LDY   #$93
   LDA   (FP),Y
   LDY   #$9B
   CMP   (FP),Y
   BNE   .cmp_true@19
   LDY   #$92
   LDA   (FP),Y
   LDY   #$9A
   CMP   (FP),Y
   BNE   .cmp_true@19
   LDY   #$91
   LDA   (FP),Y
   LDY   #$99
   CMP   (FP),Y
   BNE   .cmp_true@19
   LDY   #$90
   LDA   (FP),Y
   LDY   #$98
   CMP   (FP),Y
   BNE   .cmp_true@19
   LDY   #$8F
   LDA   (FP),Y
   LDY   #$97
   CMP   (FP),Y
   BNE   .cmp_true@19
   LDY   #$8E
   LDA   (FP),Y
   LDY   #$96
   CMP   (FP),Y
   BNE   .cmp_true@19
   LDY   #$8D
   LDA   (FP),Y
   LDY   #$95
   CMP   (FP),Y
   BNE   .cmp_true@19
   LDA   #$00
   JMP   .cmp_end@20
.cmp_true@19:
   LDA   #$01
.cmp_end@20:
   LDY   #$9D
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
   ADC   #$B8
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$B7
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
   CLC
   LDA   FP
   ADC   #$1D
   LDY   #$A1
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
.mul_loop@22:
   BCC   .mul_skip@23
   CLC
   ADC   #$08
.mul_skip@23:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@22
   LDY   #$A0
   STA   (FP),Y
   LDA   HARGS
   DEY
   STA   (FP),Y
   LDY   #$A1
   LDA   (FP),Y
   CLC
   LDY   #$9F
   ADC   (FP),Y
   LDY   #$A3
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$A0
   ADC   (FP),Y
   LDY   #$A4
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$A5
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$A6
   STA   (FP),Y
   LDY   #$02
   LDA   (DPTR),Y
   LDY   #$A7
   STA   (FP),Y
   LDY   #$03
   LDA   (DPTR),Y
   LDY   #$A8
   STA   (FP),Y
   LDY   #$04
   LDA   (DPTR),Y
   LDY   #$A9
   STA   (FP),Y
   LDY   #$05
   LDA   (DPTR),Y
   LDY   #$AA
   STA   (FP),Y
   LDY   #$06
   LDA   (DPTR),Y
   LDY   #$AB
   STA   (FP),Y
   LDY   #$07
   LDA   (DPTR),Y
   LDY   #$AC
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
   LDA   #$44
   INY
   STA   (FP),Y
   LDA   #$40
   EOR   #$80
   INY
   STA   (FP),Y
   LDY   #$AC
   LDA   (FP),Y
   LDY   #$B4
   CMP   (FP),Y
   BNE   .cmp_true@24
   LDY   #$AB
   LDA   (FP),Y
   LDY   #$B3
   CMP   (FP),Y
   BNE   .cmp_true@24
   LDY   #$AA
   LDA   (FP),Y
   LDY   #$B2
   CMP   (FP),Y
   BNE   .cmp_true@24
   LDY   #$A9
   LDA   (FP),Y
   LDY   #$B1
   CMP   (FP),Y
   BNE   .cmp_true@24
   LDY   #$A8
   LDA   (FP),Y
   LDY   #$B0
   CMP   (FP),Y
   BNE   .cmp_true@24
   LDY   #$A7
   LDA   (FP),Y
   LDY   #$AF
   CMP   (FP),Y
   BNE   .cmp_true@24
   LDY   #$A6
   LDA   (FP),Y
   LDY   #$AE
   CMP   (FP),Y
   BNE   .cmp_true@24
   LDY   #$A5
   LDA   (FP),Y
   LDY   #$AD
   CMP   (FP),Y
   BNE   .cmp_true@24
   LDA   #$00
   JMP   .cmp_end@25
.cmp_true@24:
   LDA   #$01
.cmp_end@25:
   LDY   #$B5
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
   ADC   #$B8
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
.if_end@4:
   LDX   #$00
   TXA

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
   LDY   #$B7
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
