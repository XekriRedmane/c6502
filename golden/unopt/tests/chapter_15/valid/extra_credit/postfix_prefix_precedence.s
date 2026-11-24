SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24

main:
   SUBROUTINE

   ; prologue: 0 arg bytes, 100 local bytes
   SEC
   LDA   SSP
   SBC   #$66
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$65
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
   LDY   #$0B
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
   LDY   #$0B
   LDA   (FP),Y
   CLC
   ADC   #$02
   LDY   #$0D
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$0E
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
   LDY   #$0B
   LDA   (FP),Y
   CLC
   ADC   #$04
   LDY   #$0F
   STA   (FP),Y
   LDY   #$0C
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
   LDA   #$03
   LDY   #$00
   STA   (DPTR),Y
   LDA   #$00
   INY
   STA   (DPTR),Y
   LDY   #$0B
   LDA   (FP),Y
   CLC
   ADC   #$06
   LDY   #$11
   STA   (FP),Y
   LDY   #$0C
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
   LDA   #$04
   LDY   #$00
   STA   (DPTR),Y
   LDA   #$00
   INY
   STA   (DPTR),Y
   LDY   #$0B
   LDA   (FP),Y
   CLC
   ADC   #$08
   LDY   #$13
   STA   (FP),Y
   LDY   #$0C
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
   LDA   #$05
   LDY   #$00
   STA   (DPTR),Y
   LDA   #$00
   INY
   STA   (DPTR),Y
   CLC
   LDA   FP
   ADC   #$01
   LDY   #$17
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
.mul_loop@0:
   BCC   .mul_skip@1
   CLC
   ADC   #$02
.mul_skip@1:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@0
   LDY   #$16
   STA   (FP),Y
   LDA   HARGS
   DEY
   STA   (FP),Y
   LDY   #$17
   LDA   (FP),Y
   CLC
   LDY   #$15
   ADC   (FP),Y
   LDY   #$19
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$16
   ADC   (FP),Y
   LDY   #$1A
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$1B
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$1C
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$1D
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$1E
   STA   (FP),Y
   LDA   #$01
   STA   HARGS
   LDA   #$00
   LDX   #$08
   LSR   HARGS
.mul_loop@2:
   BCC   .mul_skip@3
   CLC
   ADC   #$02
.mul_skip@3:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@2
   LDY   #$20
   STA   (FP),Y
   LDA   HARGS
   DEY
   STA   (FP),Y
   LDY   #$1B
   LDA   (FP),Y
   SEC
   LDY   #$1F
   SBC   (FP),Y
   LDY   #$21
   STA   (FP),Y
   LDY   #$1C
   LDA   (FP),Y
   LDY   #$20
   SBC   (FP),Y
   LDY   #$22
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$1B
   STA   (FP),Y
   LDY   #$22
   LDA   (FP),Y
   LDY   #$1C
   STA   (FP),Y
   LDA   idx
   STA   HARGS
   LDA   idx+1
   STA   HARGS+1
   LDA   #$02
   STA   HARGS+2
   LDA   #$00
   STA   HARGS+3
   JSR   mul16
   LDA   HARGS+4
   LDY   #$23
   STA   (FP),Y
   LDA   HARGS+5
   INY
   STA   (FP),Y
   LDY   #$1D
   LDA   (FP),Y
   CLC
   LDY   #$23
   ADC   (FP),Y
   LDY   #$25
   STA   (FP),Y
   LDY   #$1E
   LDA   (FP),Y
   LDY   #$24
   ADC   (FP),Y
   LDY   #$26
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$27
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$28
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$01
   LDY   #$29
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$2A
   STA   (FP),Y
   LDY   #$25
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$29
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$2A
   LDA   (FP),Y
   LDY   #$01
   STA   (DPTR),Y
   LDY   #$29
   LDA   (FP),Y
   LDY   #$2B
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$2C
   STA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@4
   DEY
   LDA   (FP),Y
   CMP   #$06
   BNE   .cmp_true@4
   LDA   #$00
   JMP   .cmp_end@5
.cmp_true@4:
   LDA   #$01
.cmp_end@5:
   LDY   #$2D
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
   ADC   #$66
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$65
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
   LDY   #$1B
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$2F
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$30
   STA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@7
   DEY
   LDA   (FP),Y
   CMP   #$01
   BNE   .cmp_true@7
   LDA   #$00
   JMP   .cmp_end@8
.cmp_true@7:
   LDA   #$01
.cmp_end@8:
   LDY   #$31
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
   ADC   #$66
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$65
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
   LDY   #$33
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   LDY   #$1C
   LDA   (FP),Y
   LDY   #$34
   CMP   (FP),Y
   BNE   .cmp_true@10
   LDY   #$1B
   LDA   (FP),Y
   LDY   #$33
   CMP   (FP),Y
   BNE   .cmp_true@10
   LDA   #$00
   JMP   .cmp_end@11
.cmp_true@10:
   LDA   #$01
.cmp_end@11:
   LDY   #$35
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
   ADC   #$66
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$65
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
   LDY   #$1B
   LDA   (FP),Y
   LDY   #$37
   STA   (FP),Y
   LDY   #$1C
   LDA   (FP),Y
   LDY   #$38
   STA   (FP),Y
   LDA   #$01
   STA   HARGS
   LDA   #$00
   LDX   #$08
   LSR   HARGS
.mul_loop@13:
   BCC   .mul_skip@14
   CLC
   ADC   #$02
.mul_skip@14:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@13
   LDY   #$3A
   STA   (FP),Y
   LDA   HARGS
   DEY
   STA   (FP),Y
   LDY   #$1B
   LDA   (FP),Y
   CLC
   LDY   #$39
   ADC   (FP),Y
   LDY   #$3B
   STA   (FP),Y
   LDY   #$1C
   LDA   (FP),Y
   LDY   #$3A
   ADC   (FP),Y
   LDY   #$3C
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$1B
   STA   (FP),Y
   LDY   #$3C
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
   LDY   #$3D
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$3E
   STA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@15
   DEY
   LDA   (FP),Y
   CMP   #$01
   BNE   .cmp_true@15
   LDA   #$00
   JMP   .cmp_end@16
.cmp_true@15:
   LDA   #$01
.cmp_end@16:
   LDY   #$3F
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
   ADC   #$66
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$65
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
   LDY   #$1B
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
   BNE   .cmp_true@18
   DEY
   LDA   (FP),Y
   CMP   #$02
   BNE   .cmp_true@18
   LDA   #$00
   JMP   .cmp_end@19
.cmp_true@18:
   LDA   #$01
.cmp_end@19:
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
   ADC   #$66
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$65
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
   LDA   #$00
   LDY   #$45
   STA   (FP),Y
   INY
   STA   (FP),Y
.loop@0_start:
   LDY   #$45
   LDA   (FP),Y
   SEC
   SBC   #$04
   INY
   LDA   (FP),Y
   SBC   #$00
   BVC   .cmp_novf@21
   EOR   #$80
.cmp_novf@21:
   ASL
   LDA   #$00
   ROL
   LDY   #$47
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@0
   JMP   .loop@0_break
.lb_skip@0:
   CLC
   LDA   FP
   ADC   #$01
   LDY   #$4D
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   LDY   #$45
   LDA   (FP),Y
   LDY   #$49
   STA   (FP),Y
   STA   HARGS
   LDA   #$02
   STA   HARGS+1
   JSR   mul8
   LDA   HARGS+2
   LDY   #$4A
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   LDA   (FP),Y
   CLC
   LDY   #$4B
   ADC   (FP),Y
   LDY   #$4F
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$4C
   ADC   (FP),Y
   LDY   #$50
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$51
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$52
   STA   (FP),Y
   LDY   #$45
   LDA   (FP),Y
   CLC
   ADC   #$01
   LDY   #$53
   STA   (FP),Y
   LDY   #$46
   LDA   (FP),Y
   ADC   #$00
   LDY   #$54
   STA   (FP),Y
   LDY   #$52
   LDA   (FP),Y
   LDY   #$54
   CMP   (FP),Y
   BNE   .cmp_true@22
   LDY   #$51
   LDA   (FP),Y
   LDY   #$53
   CMP   (FP),Y
   BNE   .cmp_true@22
   LDA   #$00
   JMP   .cmp_end@23
.cmp_true@22:
   LDA   #$01
.cmp_end@23:
   LDY   #$55
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
   ADC   #$66
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$65
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
   LDY   #$45
   LDA   (FP),Y
   LDY   #$57
   STA   (FP),Y
   LDY   #$46
   LDA   (FP),Y
   LDY   #$58
   STA   (FP),Y
   LDY   #$45
   LDA   (FP),Y
   CLC
   ADC   #$01
   LDY   #$59
   STA   (FP),Y
   LDY   #$46
   LDA   (FP),Y
   ADC   #$00
   LDY   #$5A
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$45
   STA   (FP),Y
   LDY   #$5A
   LDA   (FP),Y
   LDY   #$46
   STA   (FP),Y
   JMP   .loop@0_start
.loop@0_break:
   CLC
   LDA   FP
   ADC   #$01
   LDY   #$5D
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   LDA   #$04
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
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$61
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$62
   STA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@27
   DEY
   LDA   (FP),Y
   CMP   #$06
   BNE   .cmp_true@27
   LDA   #$00
   JMP   .cmp_end@28
.cmp_true@27:
   LDA   #$01
.cmp_end@28:
   LDY   #$63
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
   ADC   #$66
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
   ADC   #$66
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$65
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

idx:
   DC.W  $0003
