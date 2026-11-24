SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24

main:
   SUBROUTINE

   ; prologue: 0 arg bytes, 212 local bytes
   SEC
   LDA   SSP
   SBC   #$D6
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$D5
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   LDA   #<arr
   LDY   #$01
   STA   (FP),Y
   LDA   #>arr
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
   ADC   #$30
.mul_skip@1:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@0
   LDY   #$04
   STA   (FP),Y
   LDA   HARGS
   DEY
   STA   (FP),Y
   LDY   #$01
   LDA   (FP),Y
   CLC
   LDY   #$03
   ADC   (FP),Y
   LDY   #$05
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   LDY   #$04
   ADC   (FP),Y
   LDY   #$06
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$07
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$08
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$09
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$0A
   STA   (FP),Y
   LDA   #$01
   STA   HARGS
   LDA   #$00
   LDX   #$08
   LSR   HARGS
.mul_loop@2:
   BCC   .mul_skip@3
   CLC
   ADC   #$30
.mul_skip@3:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@2
   LDY   #$0C
   STA   (FP),Y
   LDA   HARGS
   DEY
   STA   (FP),Y
   LDY   #$07
   LDA   (FP),Y
   SEC
   LDY   #$0B
   SBC   (FP),Y
   LDY   #$0D
   STA   (FP),Y
   LDY   #$08
   LDA   (FP),Y
   LDY   #$0C
   SBC   (FP),Y
   LDY   #$0E
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$07
   STA   (FP),Y
   LDY   #$0E
   LDA   (FP),Y
   LDY   #$08
   STA   (FP),Y
   LDA   #<arr
   LDY   #$0F
   STA   (FP),Y
   LDA   #>arr
   INY
   STA   (FP),Y
   LDA   #$01
   STA   HARGS
   LDA   #$00
   LDX   #$08
   LSR   HARGS
.mul_loop@4:
   BCC   .mul_skip@5
   CLC
   ADC   #$30
.mul_skip@5:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@4
   LDY   #$12
   STA   (FP),Y
   LDA   HARGS
   DEY
   STA   (FP),Y
   LDY   #$0F
   LDA   (FP),Y
   CLC
   LDY   #$11
   ADC   (FP),Y
   LDY   #$13
   STA   (FP),Y
   LDY   #$10
   LDA   (FP),Y
   LDY   #$12
   ADC   (FP),Y
   LDY   #$14
   STA   (FP),Y
   LDY   #$0A
   LDA   (FP),Y
   LDY   #$14
   CMP   (FP),Y
   BNE   .cmp_true@6
   LDY   #$09
   LDA   (FP),Y
   LDY   #$13
   CMP   (FP),Y
   BNE   .cmp_true@6
   LDA   #$00
   JMP   .cmp_end@7
.cmp_true@6:
   LDA   #$01
.cmp_end@7:
   LDY   #$15
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
   ADC   #$D6
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$D5
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
.mul_loop@9:
   BCC   .mul_skip@10
   CLC
   ADC   #$30
.mul_skip@10:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@9
   LDY   #$18
   STA   (FP),Y
   LDA   HARGS
   DEY
   STA   (FP),Y
   LDY   #$07
   LDA   (FP),Y
   CLC
   LDY   #$17
   ADC   (FP),Y
   LDY   #$19
   STA   (FP),Y
   LDY   #$08
   LDA   (FP),Y
   LDY   #$18
   ADC   (FP),Y
   LDY   #$1A
   STA   (FP),Y
   LDA   #$01
   STA   HARGS
   LDA   #$00
   LDX   #$08
   LSR   HARGS
.mul_loop@11:
   BCC   .mul_skip@12
   CLC
   ADC   #$10
.mul_skip@12:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@11
   LDY   #$1C
   STA   (FP),Y
   LDA   HARGS
   DEY
   STA   (FP),Y
   LDY   #$19
   LDA   (FP),Y
   CLC
   LDY   #$1B
   ADC   (FP),Y
   LDY   #$1D
   STA   (FP),Y
   LDY   #$1A
   LDA   (FP),Y
   LDY   #$1C
   ADC   (FP),Y
   LDY   #$1E
   STA   (FP),Y
   LDA   #<arr
   INY
   STA   (FP),Y
   LDA   #>arr
   INY
   STA   (FP),Y
   LDA   #$00
   STA   HARGS
   LDX   #$08
   LSR   HARGS
.mul_loop@13:
   BCC   .mul_skip@14
   CLC
   ADC   #$30
.mul_skip@14:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@13
   LDY   #$22
   STA   (FP),Y
   LDA   HARGS
   DEY
   STA   (FP),Y
   LDY   #$1F
   LDA   (FP),Y
   CLC
   LDY   #$21
   ADC   (FP),Y
   LDY   #$23
   STA   (FP),Y
   LDY   #$20
   LDA   (FP),Y
   LDY   #$22
   ADC   (FP),Y
   LDY   #$24
   STA   (FP),Y
   LDA   #$01
   STA   HARGS
   LDA   #$00
   LDX   #$08
   LSR   HARGS
.mul_loop@15:
   BCC   .mul_skip@16
   CLC
   ADC   #$10
.mul_skip@16:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@15
   LDY   #$26
   STA   (FP),Y
   LDA   HARGS
   DEY
   STA   (FP),Y
   LDY   #$23
   LDA   (FP),Y
   CLC
   LDY   #$25
   ADC   (FP),Y
   LDY   #$27
   STA   (FP),Y
   LDY   #$24
   LDA   (FP),Y
   LDY   #$26
   ADC   (FP),Y
   LDY   #$28
   STA   (FP),Y
   LDY   #$1E
   LDA   (FP),Y
   LDY   #$28
   CMP   (FP),Y
   BNE   .cmp_true@17
   LDY   #$1D
   LDA   (FP),Y
   LDY   #$27
   CMP   (FP),Y
   BNE   .cmp_true@17
   LDA   #$00
   JMP   .cmp_end@18
.cmp_true@17:
   LDA   #$01
.cmp_end@18:
   LDY   #$29
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
   ADC   #$D6
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$D5
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
   LDA   #$01
   STA   HARGS
   LDA   #$00
   LDX   #$08
   LSR   HARGS
.mul_loop@20:
   BCC   .mul_skip@21
   CLC
   ADC   #$30
.mul_skip@21:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@20
   LDY   #$2C
   STA   (FP),Y
   LDA   HARGS
   DEY
   STA   (FP),Y
   LDY   #$07
   LDA   (FP),Y
   CLC
   LDY   #$2B
   ADC   (FP),Y
   LDY   #$2D
   STA   (FP),Y
   LDY   #$08
   LDA   (FP),Y
   LDY   #$2C
   ADC   (FP),Y
   LDY   #$2E
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$07
   STA   (FP),Y
   LDY   #$2E
   LDA   (FP),Y
   LDY   #$08
   STA   (FP),Y
   LDA   #$00
   STA   HARGS
   LDX   #$08
   LSR   HARGS
.mul_loop@22:
   BCC   .mul_skip@23
   CLC
   ADC   #$30
.mul_skip@23:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@22
   LDY   #$30
   STA   (FP),Y
   LDA   HARGS
   DEY
   STA   (FP),Y
   LDY   #$2D
   LDA   (FP),Y
   CLC
   LDY   #$2F
   ADC   (FP),Y
   LDY   #$31
   STA   (FP),Y
   LDY   #$2E
   LDA   (FP),Y
   LDY   #$30
   ADC   (FP),Y
   LDY   #$32
   STA   (FP),Y
   LDA   #$02
   STA   HARGS
   LDA   #$00
   LDX   #$08
   LSR   HARGS
.mul_loop@24:
   BCC   .mul_skip@25
   CLC
   ADC   #$10
.mul_skip@25:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@24
   LDY   #$34
   STA   (FP),Y
   LDA   HARGS
   DEY
   STA   (FP),Y
   LDY   #$31
   LDA   (FP),Y
   CLC
   LDY   #$33
   ADC   (FP),Y
   LDY   #$35
   STA   (FP),Y
   LDY   #$32
   LDA   (FP),Y
   LDY   #$34
   ADC   (FP),Y
   LDY   #$36
   STA   (FP),Y
   LDA   #$03
   STA   HARGS
   LDA   #$00
   LDX   #$08
   LSR   HARGS
.mul_loop@26:
   BCC   .mul_skip@27
   CLC
   ADC   #$04
.mul_skip@27:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@26
   LDY   #$38
   STA   (FP),Y
   LDA   HARGS
   DEY
   STA   (FP),Y
   LDY   #$35
   LDA   (FP),Y
   CLC
   LDY   #$37
   ADC   (FP),Y
   LDY   #$39
   STA   (FP),Y
   LDY   #$36
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
   LDA   #$18
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   ORA   #$00
   BPL   .sx_done@29
   LDA   #$FF
.sx_done@29:
   LDY   #$41
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$3E
   LDA   (FP),Y
   LDY   #$42
   CMP   (FP),Y
   BNE   .cmp_true@30
   LDY   #$3D
   LDA   (FP),Y
   LDY   #$41
   CMP   (FP),Y
   BNE   .cmp_true@30
   LDY   #$3C
   LDA   (FP),Y
   LDY   #$40
   CMP   (FP),Y
   BNE   .cmp_true@30
   LDY   #$3B
   LDA   (FP),Y
   LDY   #$3F
   CMP   (FP),Y
   BNE   .cmp_true@30
   LDA   #$00
   JMP   .cmp_end@31
.cmp_true@30:
   LDA   #$01
.cmp_end@31:
   LDY   #$43
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
   ADC   #$D6
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$D5
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
   LDA   #$00
   STA   HARGS
   LDX   #$08
   LSR   HARGS
.mul_loop@33:
   BCC   .mul_skip@34
   CLC
   ADC   #$30
.mul_skip@34:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@33
   LDY   #$46
   STA   (FP),Y
   LDA   HARGS
   DEY
   STA   (FP),Y
   LDY   #$07
   LDA   (FP),Y
   CLC
   LDY   #$45
   ADC   (FP),Y
   LDY   #$47
   STA   (FP),Y
   LDY   #$08
   LDA   (FP),Y
   LDY   #$46
   ADC   (FP),Y
   LDY   #$48
   STA   (FP),Y
   LDA   #$02
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
   LDY   #$4A
   STA   (FP),Y
   LDA   HARGS
   DEY
   STA   (FP),Y
   LDY   #$47
   LDA   (FP),Y
   CLC
   LDY   #$49
   ADC   (FP),Y
   LDY   #$4B
   STA   (FP),Y
   LDY   #$48
   LDA   (FP),Y
   LDY   #$4A
   ADC   (FP),Y
   LDY   #$4C
   STA   (FP),Y
   LDA   #$03
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
   LDY   #$4E
   STA   (FP),Y
   LDA   HARGS
   DEY
   STA   (FP),Y
   LDY   #$4B
   LDA   (FP),Y
   CLC
   LDY   #$4D
   ADC   (FP),Y
   LDY   #$4F
   STA   (FP),Y
   LDY   #$4C
   LDA   (FP),Y
   LDY   #$4E
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
   LDY   #$02
   LDA   (DPTR),Y
   LDY   #$53
   STA   (FP),Y
   LDY   #$03
   LDA   (DPTR),Y
   LDY   #$54
   STA   (FP),Y
   LDA   #$18
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   ORA   #$00
   BPL   .sx_done@40
   LDA   #$FF
.sx_done@40:
   LDY   #$57
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$54
   LDA   (FP),Y
   LDY   #$58
   CMP   (FP),Y
   BNE   .cmp_true@41
   LDY   #$53
   LDA   (FP),Y
   LDY   #$57
   CMP   (FP),Y
   BNE   .cmp_true@41
   LDY   #$52
   LDA   (FP),Y
   LDY   #$56
   CMP   (FP),Y
   BNE   .cmp_true@41
   LDY   #$51
   LDA   (FP),Y
   LDY   #$55
   CMP   (FP),Y
   BNE   .cmp_true@41
   LDA   #$00
   JMP   .cmp_end@42
.cmp_true@41:
   LDA   #$01
.cmp_end@42:
   LDY   #$59
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
   ADC   #$D6
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$D5
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
   LDA   #<arr
   LDY   #$5B
   STA   (FP),Y
   LDA   #>arr
   INY
   STA   (FP),Y
   LDA   #$00
   STA   HARGS
   LDX   #$08
   LSR   HARGS
.mul_loop@44:
   BCC   .mul_skip@45
   CLC
   ADC   #$30
.mul_skip@45:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@44
   LDY   #$5E
   STA   (FP),Y
   LDA   HARGS
   DEY
   STA   (FP),Y
   LDY   #$5B
   LDA   (FP),Y
   CLC
   LDY   #$5D
   ADC   (FP),Y
   LDY   #$5F
   STA   (FP),Y
   LDY   #$5C
   LDA   (FP),Y
   LDY   #$5E
   ADC   (FP),Y
   LDY   #$60
   STA   (FP),Y
   LDA   #$01
   STA   HARGS
   LDA   #$00
   LDX   #$08
   LSR   HARGS
.mul_loop@46:
   BCC   .mul_skip@47
   CLC
   ADC   #$10
.mul_skip@47:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@46
   LDY   #$62
   STA   (FP),Y
   LDA   HARGS
   DEY
   STA   (FP),Y
   LDY   #$5F
   LDA   (FP),Y
   CLC
   LDY   #$61
   ADC   (FP),Y
   LDY   #$63
   STA   (FP),Y
   LDY   #$60
   LDA   (FP),Y
   LDY   #$62
   ADC   (FP),Y
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
   DEY
   LDA   (FP),Y
   LDY   #$67
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$68
   STA   (FP),Y
   LDA   #$01
   STA   HARGS
   LDA   #$00
   LDX   #$08
   LSR   HARGS
.mul_loop@48:
   BCC   .mul_skip@49
   CLC
   ADC   #$10
.mul_skip@49:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@48
   LDY   #$6A
   STA   (FP),Y
   LDA   HARGS
   DEY
   STA   (FP),Y
   LDY   #$65
   LDA   (FP),Y
   CLC
   LDY   #$69
   ADC   (FP),Y
   LDY   #$6B
   STA   (FP),Y
   LDY   #$66
   LDA   (FP),Y
   LDY   #$6A
   ADC   (FP),Y
   LDY   #$6C
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$65
   STA   (FP),Y
   LDY   #$6C
   LDA   (FP),Y
   LDY   #$66
   STA   (FP),Y
   LDA   #$00
   STA   HARGS
   LDX   #$08
   LSR   HARGS
.mul_loop@50:
   BCC   .mul_skip@51
   CLC
   ADC   #$10
.mul_skip@51:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@50
   LDY   #$6E
   STA   (FP),Y
   LDA   HARGS
   DEY
   STA   (FP),Y
   LDY   #$67
   LDA   (FP),Y
   CLC
   LDY   #$6D
   ADC   (FP),Y
   LDY   #$6F
   STA   (FP),Y
   LDY   #$68
   LDA   (FP),Y
   LDY   #$6E
   ADC   (FP),Y
   LDY   #$70
   STA   (FP),Y
   LDA   #$02
   STA   HARGS
   LDA   #$00
   LDX   #$08
   LSR   HARGS
.mul_loop@52:
   BCC   .mul_skip@53
   CLC
   ADC   #$04
.mul_skip@53:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@52
   LDY   #$72
   STA   (FP),Y
   LDA   HARGS
   DEY
   STA   (FP),Y
   LDY   #$6F
   LDA   (FP),Y
   CLC
   LDY   #$71
   ADC   (FP),Y
   LDY   #$73
   STA   (FP),Y
   LDY   #$70
   LDA   (FP),Y
   LDY   #$72
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
   LDA   #$07
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   ORA   #$00
   BPL   .sx_done@55
   LDA   #$FF
.sx_done@55:
   LDY   #$7B
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$78
   LDA   (FP),Y
   LDY   #$7C
   CMP   (FP),Y
   BNE   .cmp_true@56
   LDY   #$77
   LDA   (FP),Y
   LDY   #$7B
   CMP   (FP),Y
   BNE   .cmp_true@56
   LDY   #$76
   LDA   (FP),Y
   LDY   #$7A
   CMP   (FP),Y
   BNE   .cmp_true@56
   LDY   #$75
   LDA   (FP),Y
   LDY   #$79
   CMP   (FP),Y
   BNE   .cmp_true@56
   LDA   #$00
   JMP   .cmp_end@57
.cmp_true@56:
   LDA   #$01
.cmp_end@57:
   LDY   #$7D
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
   ADC   #$D6
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$D5
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
   STA   HARGS
   LDX   #$08
   LSR   HARGS
.mul_loop@59:
   BCC   .mul_skip@60
   CLC
   ADC   #$10
.mul_skip@60:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@59
   LDY   #$80
   STA   (FP),Y
   LDA   HARGS
   DEY
   STA   (FP),Y
   LDY   #$65
   LDA   (FP),Y
   CLC
   LDY   #$7F
   ADC   (FP),Y
   LDY   #$81
   STA   (FP),Y
   LDY   #$66
   LDA   (FP),Y
   LDY   #$80
   ADC   (FP),Y
   LDY   #$82
   STA   (FP),Y
   LDA   #$02
   STA   HARGS
   LDA   #$00
   LDX   #$08
   LSR   HARGS
.mul_loop@61:
   BCC   .mul_skip@62
   CLC
   ADC   #$04
.mul_skip@62:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@61
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
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$87
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$88
   STA   (FP),Y
   LDY   #$02
   LDA   (DPTR),Y
   LDY   #$89
   STA   (FP),Y
   LDY   #$03
   LDA   (DPTR),Y
   LDY   #$8A
   STA   (FP),Y
   LDA   #$0B
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   ORA   #$00
   BPL   .sx_done@64
   LDA   #$FF
.sx_done@64:
   LDY   #$8D
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$8A
   LDA   (FP),Y
   LDY   #$8E
   CMP   (FP),Y
   BNE   .cmp_true@65
   LDY   #$89
   LDA   (FP),Y
   LDY   #$8D
   CMP   (FP),Y
   BNE   .cmp_true@65
   LDY   #$88
   LDA   (FP),Y
   LDY   #$8C
   CMP   (FP),Y
   BNE   .cmp_true@65
   LDY   #$87
   LDA   (FP),Y
   LDY   #$8B
   CMP   (FP),Y
   BNE   .cmp_true@65
   LDA   #$00
   JMP   .cmp_end@66
.cmp_true@65:
   LDA   #$01
.cmp_end@66:
   LDY   #$8F
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
   ADC   #$D6
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$D5
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
   LDA   #$01
   STA   HARGS
   LDA   #$00
   LDX   #$08
   LSR   HARGS
.mul_loop@68:
   BCC   .mul_skip@69
   CLC
   ADC   #$10
.mul_skip@69:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@68
   LDY   #$92
   STA   (FP),Y
   LDA   HARGS
   DEY
   STA   (FP),Y
   LDY   #$65
   LDA   (FP),Y
   SEC
   LDY   #$91
   SBC   (FP),Y
   LDY   #$93
   STA   (FP),Y
   LDY   #$66
   LDA   (FP),Y
   LDY   #$92
   SBC   (FP),Y
   LDY   #$94
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$65
   STA   (FP),Y
   LDY   #$94
   LDA   (FP),Y
   LDY   #$66
   STA   (FP),Y
   LDA   #$00
   STA   HARGS
   LDX   #$08
   LSR   HARGS
.mul_loop@70:
   BCC   .mul_skip@71
   CLC
   ADC   #$10
.mul_skip@71:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@70
   LDY   #$96
   STA   (FP),Y
   LDA   HARGS
   DEY
   STA   (FP),Y
   LDY   #$93
   LDA   (FP),Y
   CLC
   LDY   #$95
   ADC   (FP),Y
   LDY   #$97
   STA   (FP),Y
   LDY   #$94
   LDA   (FP),Y
   LDY   #$96
   ADC   (FP),Y
   LDY   #$98
   STA   (FP),Y
   LDA   #$01
   STA   HARGS
   LDA   #$00
   LDX   #$08
   LSR   HARGS
.mul_loop@72:
   BCC   .mul_skip@73
   CLC
   ADC   #$04
.mul_skip@73:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@72
   LDY   #$9A
   STA   (FP),Y
   LDA   HARGS
   DEY
   STA   (FP),Y
   LDY   #$97
   LDA   (FP),Y
   CLC
   LDY   #$99
   ADC   (FP),Y
   LDY   #$9B
   STA   (FP),Y
   LDY   #$98
   LDA   (FP),Y
   LDY   #$9A
   ADC   (FP),Y
   LDY   #$9C
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$9D
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$9E
   STA   (FP),Y
   LDY   #$02
   LDA   (DPTR),Y
   LDY   #$9F
   STA   (FP),Y
   LDY   #$03
   LDA   (DPTR),Y
   LDY   #$A0
   STA   (FP),Y
   LDA   #$06
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   ORA   #$00
   BPL   .sx_done@75
   LDA   #$FF
.sx_done@75:
   LDY   #$A3
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$A0
   LDA   (FP),Y
   LDY   #$A4
   CMP   (FP),Y
   BNE   .cmp_true@76
   LDY   #$9F
   LDA   (FP),Y
   LDY   #$A3
   CMP   (FP),Y
   BNE   .cmp_true@76
   LDY   #$9E
   LDA   (FP),Y
   LDY   #$A2
   CMP   (FP),Y
   BNE   .cmp_true@76
   LDY   #$9D
   LDA   (FP),Y
   LDY   #$A1
   CMP   (FP),Y
   BNE   .cmp_true@76
   LDA   #$00
   JMP   .cmp_end@77
.cmp_true@76:
   LDA   #$01
.cmp_end@77:
   LDY   #$A5
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
   ADC   #$D6
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$D5
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
   LDA   #<arr
   LDY   #$A7
   STA   (FP),Y
   LDA   #>arr
   INY
   STA   (FP),Y
   LDA   #$01
   STA   HARGS
   LDA   #$00
   LDX   #$08
   LSR   HARGS
.mul_loop@79:
   BCC   .mul_skip@80
   CLC
   ADC   #$30
.mul_skip@80:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@79
   LDY   #$AA
   STA   (FP),Y
   LDA   HARGS
   DEY
   STA   (FP),Y
   LDY   #$A7
   LDA   (FP),Y
   CLC
   LDY   #$A9
   ADC   (FP),Y
   LDY   #$AB
   STA   (FP),Y
   LDY   #$A8
   LDA   (FP),Y
   LDY   #$AA
   ADC   (FP),Y
   LDY   #$AC
   STA   (FP),Y
   LDA   #$02
   STA   HARGS
   LDA   #$00
   LDX   #$08
   LSR   HARGS
.mul_loop@81:
   BCC   .mul_skip@82
   CLC
   ADC   #$10
.mul_skip@82:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@81
   LDY   #$AE
   STA   (FP),Y
   LDA   HARGS
   DEY
   STA   (FP),Y
   LDY   #$AB
   LDA   (FP),Y
   CLC
   LDY   #$AD
   ADC   (FP),Y
   LDY   #$AF
   STA   (FP),Y
   LDY   #$AC
   LDA   (FP),Y
   LDY   #$AE
   ADC   (FP),Y
   LDY   #$B0
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$B1
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$B2
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$B3
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$B4
   STA   (FP),Y
   LDA   #$01
   STA   HARGS
   LDA   #$00
   LDX   #$08
   LSR   HARGS
.mul_loop@83:
   BCC   .mul_skip@84
   CLC
   ADC   #$04
.mul_skip@84:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@83
   LDY   #$B6
   STA   (FP),Y
   LDA   HARGS
   DEY
   STA   (FP),Y
   LDY   #$B1
   LDA   (FP),Y
   SEC
   LDY   #$B5
   SBC   (FP),Y
   LDY   #$B7
   STA   (FP),Y
   LDY   #$B2
   LDA   (FP),Y
   LDY   #$B6
   SBC   (FP),Y
   LDY   #$B8
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$B1
   STA   (FP),Y
   LDY   #$B8
   LDA   (FP),Y
   LDY   #$B2
   STA   (FP),Y
   LDA   #$02
   STA   HARGS
   LDA   #$00
   LDX   #$08
   LSR   HARGS
.mul_loop@85:
   BCC   .mul_skip@86
   CLC
   ADC   #$04
.mul_skip@86:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@85
   LDY   #$BA
   STA   (FP),Y
   LDA   HARGS
   DEY
   STA   (FP),Y
   LDY   #$B3
   LDA   (FP),Y
   CLC
   LDY   #$B9
   ADC   (FP),Y
   LDY   #$BB
   STA   (FP),Y
   LDY   #$B4
   LDA   (FP),Y
   LDY   #$BA
   ADC   (FP),Y
   LDY   #$BC
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$BD
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$BE
   STA   (FP),Y
   LDY   #$02
   LDA   (DPTR),Y
   LDY   #$BF
   STA   (FP),Y
   LDY   #$03
   LDA   (DPTR),Y
   LDY   #$C0
   STA   (FP),Y
   LDA   #$17
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   ORA   #$00
   BPL   .sx_done@88
   LDA   #$FF
.sx_done@88:
   LDY   #$C3
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$C0
   LDA   (FP),Y
   LDY   #$C4
   CMP   (FP),Y
   BNE   .cmp_true@89
   LDY   #$BF
   LDA   (FP),Y
   LDY   #$C3
   CMP   (FP),Y
   BNE   .cmp_true@89
   LDY   #$BE
   LDA   (FP),Y
   LDY   #$C2
   CMP   (FP),Y
   BNE   .cmp_true@89
   LDY   #$BD
   LDA   (FP),Y
   LDY   #$C1
   CMP   (FP),Y
   BNE   .cmp_true@89
   LDA   #$00
   JMP   .cmp_end@90
.cmp_true@89:
   LDA   #$01
.cmp_end@90:
   LDY   #$C5
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
   ADC   #$D6
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$D5
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
   LDA   #$02
   STA   HARGS
   LDA   #$00
   LDX   #$08
   LSR   HARGS
.mul_loop@92:
   BCC   .mul_skip@93
   CLC
   ADC   #$04
.mul_skip@93:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@92
   LDY   #$C8
   STA   (FP),Y
   LDA   HARGS
   DEY
   STA   (FP),Y
   LDY   #$B1
   LDA   (FP),Y
   CLC
   LDY   #$C7
   ADC   (FP),Y
   LDY   #$C9
   STA   (FP),Y
   LDY   #$B2
   LDA   (FP),Y
   LDY   #$C8
   ADC   (FP),Y
   LDY   #$CA
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$CB
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$CC
   STA   (FP),Y
   LDY   #$02
   LDA   (DPTR),Y
   LDY   #$CD
   STA   (FP),Y
   LDY   #$03
   LDA   (DPTR),Y
   LDY   #$CE
   STA   (FP),Y
   LDA   #$16
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   ORA   #$00
   BPL   .sx_done@95
   LDA   #$FF
.sx_done@95:
   LDY   #$D1
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$CE
   LDA   (FP),Y
   LDY   #$D2
   CMP   (FP),Y
   BNE   .cmp_true@96
   LDY   #$CD
   LDA   (FP),Y
   LDY   #$D1
   CMP   (FP),Y
   BNE   .cmp_true@96
   LDY   #$CC
   LDA   (FP),Y
   LDY   #$D0
   CMP   (FP),Y
   BNE   .cmp_true@96
   LDY   #$CB
   LDA   (FP),Y
   LDY   #$CF
   CMP   (FP),Y
   BNE   .cmp_true@96
   LDA   #$00
   JMP   .cmp_end@97
.cmp_true@96:
   LDA   #$01
.cmp_end@97:
   LDY   #$D3
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
   ADC   #$D6
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
.if_end@8:
   LDX   #$00
   TXA

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$D6
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$D5
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

arr:
   DC.L  $00000001
   DC.L  $00000002
   DC.L  $00000003
   DC.L  $00000004
   DC.L  $00000005
   DC.L  $00000006
   DC.L  $00000007
   DC.L  $00000008
   DC.L  $00000009
   DC.L  $0000000A
   DC.L  $0000000B
   DC.L  $0000000C
   DC.L  $0000000D
   DC.L  $0000000E
   DC.L  $0000000F
   DC.L  $00000010
   DC.L  $00000011
   DC.L  $00000012
   DC.L  $00000013
   DC.L  $00000014
   DC.L  $00000015
   DC.L  $00000016
   DC.L  $00000017
   DC.L  $00000018
