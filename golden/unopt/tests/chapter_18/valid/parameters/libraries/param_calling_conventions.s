SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24

pass_small_structs:
   SUBROUTINE

   ; prologue: 52 arg bytes, 113 local bytes
   SEC
   LDA   SSP
   SBC   #$73
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$72
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
   ADC   #$74
   LDY   #$03
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   LDA   #$00
   STA   HARGS
   LDX   #$08
   LSR   HARGS
.mul_loop@0:
   BCC   .mul_skip@1
   CLC
   ADC   #$08
.mul_skip@1:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@0
   LDY   #$02
   STA   (FP),Y
   LDA   HARGS
   DEY
   STA   (FP),Y
   LDY   #$03
   LDA   (FP),Y
   CLC
   LDY   #$01
   ADC   (FP),Y
   LDY   #$05
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$02
   ADC   (FP),Y
   LDY   #$06
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$07
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$08
   STA   (FP),Y
   LDY   #$02
   LDA   (DPTR),Y
   LDY   #$09
   STA   (FP),Y
   LDY   #$03
   LDA   (DPTR),Y
   LDY   #$0A
   STA   (FP),Y
   LDY   #$04
   LDA   (DPTR),Y
   LDY   #$0B
   STA   (FP),Y
   LDY   #$05
   LDA   (DPTR),Y
   LDY   #$0C
   STA   (FP),Y
   LDY   #$06
   LDA   (DPTR),Y
   LDY   #$0D
   STA   (FP),Y
   LDY   #$07
   LDA   (DPTR),Y
   LDY   #$0E
   STA   (FP),Y
   CMP   #$40
   BNE   .cmp_true@2
   DEY
   LDA   (FP),Y
   CMP   #$4B
   BNE   .cmp_true@2
   DEY
   LDA   (FP),Y
   CMP   #$C0
   BNE   .cmp_true@2
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@2
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@2
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@2
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@2
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@2
   LDA   #$00
   JMP   .cmp_end@3
.cmp_true@2:
   LDA   #$01
.cmp_end@3:
   LDY   #$0F
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@2
   JMP   .or_true@0
.lb_skip@2:
   CLC
   LDA   FP
   ADC   #$74
   LDY   #$13
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
.mul_loop@5:
   BCC   .mul_skip@6
   CLC
   ADC   #$08
.mul_skip@6:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@5
   LDY   #$12
   STA   (FP),Y
   LDA   HARGS
   DEY
   STA   (FP),Y
   LDY   #$13
   LDA   (FP),Y
   CLC
   LDY   #$11
   ADC   (FP),Y
   LDY   #$15
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$12
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
   BNE   .cmp_true@7
   DEY
   LDA   (FP),Y
   CMP   #$46
   BNE   .cmp_true@7
   DEY
   LDA   (FP),Y
   CMP   #$33
   BNE   .cmp_true@7
   DEY
   LDA   (FP),Y
   CMP   #$33
   BNE   .cmp_true@7
   DEY
   LDA   (FP),Y
   CMP   #$33
   BNE   .cmp_true@7
   DEY
   LDA   (FP),Y
   CMP   #$33
   BNE   .cmp_true@7
   DEY
   LDA   (FP),Y
   CMP   #$33
   BNE   .cmp_true@7
   DEY
   LDA   (FP),Y
   CMP   #$33
   BNE   .cmp_true@7
   LDA   #$00
   JMP   .cmp_end@8
.cmp_true@7:
   LDA   #$01
.cmp_end@8:
   LDY   #$1F
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
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
   LDY   #$21
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@1:
   LDY   #$21
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@2
   LDX   #$00
   TXA

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$A7
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$72
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
   ADC   #$84
   LDY   #$23
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$02
   LDY   #$25
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
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
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CMP   #$63
   BNE   .cmp_true@10
   LDA   #$00
   JMP   .cmp_end@11
.cmp_true@10:
   LDA   #$01
.cmp_end@11:
   LDY   #$2A
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@1
   JMP   .or_true@3
.lb_skip@1:
   CLC
   LDA   FP
   ADC   #$84
   LDY   #$2C
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
   LDY   #$2E
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$2F
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$30
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$31
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@12
   LDA   #$00
   JMP   .sx_done@13
.sx_neg@12:
   LDA   #$FF
.sx_done@13:
   LDY   #$32
   STA   (FP),Y
   INY
   STA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@14
   LDY   #$31
   LDA   (FP),Y
   CMP   #$D4
   BNE   .cmp_true@14
   DEY
   LDA   (FP),Y
   CMP   #$30
   BNE   .cmp_true@14
   LDA   #$00
   JMP   .cmp_end@15
.cmp_true@14:
   LDA   #$01
.cmp_end@15:
   LDY   #$34
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
   LDY   #$36
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@4:
   LDY   #$36
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@5
   LDX   #$00
   TXA

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$A7
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$72
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
   CLC
   LDA   FP
   ADC   #$87
   LDY   #$38
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
   LDY   #$3A
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$3B
   STA   (FP),Y
   LDY   #$02
   LDA   (DPTR),Y
   LDY   #$3C
   STA   (FP),Y
   LDY   #$03
   LDA   (DPTR),Y
   LDY   #$3D
   STA   (FP),Y
   LDY   #$04
   LDA   (DPTR),Y
   LDY   #$3E
   STA   (FP),Y
   LDY   #$05
   LDA   (DPTR),Y
   LDY   #$3F
   STA   (FP),Y
   LDY   #$06
   LDA   (DPTR),Y
   LDY   #$40
   STA   (FP),Y
   LDY   #$07
   LDA   (DPTR),Y
   LDY   #$41
   STA   (FP),Y
   CMP   #$40
   BNE   .cmp_true@17
   DEY
   LDA   (FP),Y
   CMP   #$14
   BNE   .cmp_true@17
   DEY
   LDA   (FP),Y
   CMP   #$80
   BNE   .cmp_true@17
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@17
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@17
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@17
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@17
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@17
   LDA   #$00
   JMP   .cmp_end@18
.cmp_true@17:
   LDA   #$01
.cmp_end@18:
   LDY   #$42
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
   TXA

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$A7
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$72
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
   CLC
   LDA   FP
   ADC   #$8F
   LDY   #$44
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$08
   LDY   #$46
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$47
   STA   (FP),Y
   LDA   #<str@0
   INY
   STA   (FP),Y
   LDA   #>str@0
   INY
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$04
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDY   #$46
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$47
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$48
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$49
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   JSR   strcmp
   LDY   #$4A
   STA   (FP),Y
   TXA
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@0
   JMP   .or_true@7
.lb_skip@0:
   CLC
   LDA   FP
   ADC   #$8F
   LDY   #$4C
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
   LDY   #$4E
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$4F
   STA   (FP),Y
   LDY   #$02
   LDA   (DPTR),Y
   LDY   #$50
   STA   (FP),Y
   LDY   #$03
   LDA   (DPTR),Y
   LDY   #$51
   STA   (FP),Y
   LDY   #$04
   LDA   (DPTR),Y
   LDY   #$52
   STA   (FP),Y
   LDY   #$05
   LDA   (DPTR),Y
   LDY   #$53
   STA   (FP),Y
   LDY   #$06
   LDA   (DPTR),Y
   LDY   #$54
   STA   (FP),Y
   LDY   #$07
   LDA   (DPTR),Y
   LDY   #$55
   STA   (FP),Y
   CMP   #$3F
   BNE   .cmp_true@20
   DEY
   LDA   (FP),Y
   CMP   #$F3
   BNE   .cmp_true@20
   DEY
   LDA   (FP),Y
   CMP   #$BE
   BNE   .cmp_true@20
   DEY
   LDA   (FP),Y
   CMP   #$76
   BNE   .cmp_true@20
   DEY
   LDA   (FP),Y
   CMP   #$C8
   BNE   .cmp_true@20
   DEY
   LDA   (FP),Y
   CMP   #$B4
   BNE   .cmp_true@20
   DEY
   LDA   (FP),Y
   CMP   #$39
   BNE   .cmp_true@20
   DEY
   LDA   (FP),Y
   CMP   #$58
   BNE   .cmp_true@20
   LDA   #$00
   JMP   .cmp_end@21
.cmp_true@20:
   LDA   #$01
.cmp_end@21:
   LDY   #$56
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@7
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   JMP   .or_end@8
.or_true@7:
   LDA   #$01
   LDY   #$58
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@8:
   LDY   #$58
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@9
   LDX   #$00
   TXA

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$A7
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$72
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
   CLC
   LDA   FP
   ADC   #$9A
   LDY   #$5A
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$02
   LDY   #$5C
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$5D
   STA   (FP),Y
   LDA   #<str@1
   INY
   STA   (FP),Y
   LDA   #>str@1
   INY
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$04
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDY   #$5C
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$5D
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$5E
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$5F
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   JSR   strcmp
   LDY   #$60
   STA   (FP),Y
   TXA
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@10
   CLC
   LDA   FP
   ADC   #$9A
   INY
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
   LDY   #$64
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$65
   STA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@23
   DEY
   LDA   (FP),Y
   CMP   #$7B
   BNE   .cmp_true@23
   LDA   #$00
   JMP   .cmp_end@24
.cmp_true@23:
   LDA   #$01
.cmp_end@24:
   LDY   #$66
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@10
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   JMP   .or_end@11
.or_true@10:
   LDA   #$01
   LDY   #$68
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@11:
   LDY   #$68
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@12
   LDX   #$00
   TXA

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$A7
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$72
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
   CLC
   LDA   FP
   ADC   #$A4
   LDY   #$6A
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
   LDY   #$6C
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$6D
   STA   (FP),Y
   LDY   #$02
   LDA   (DPTR),Y
   LDY   #$6E
   STA   (FP),Y
   LDY   #$03
   LDA   (DPTR),Y
   LDY   #$6F
   STA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@26
   DEY
   LDA   (FP),Y
   CMP   #$08
   BNE   .cmp_true@26
   DEY
   LDA   (FP),Y
   CMP   #$AA
   BNE   .cmp_true@26
   DEY
   LDA   (FP),Y
   CMP   #$52
   BNE   .cmp_true@26
   LDA   #$00
   JMP   .cmp_end@27
.cmp_true@26:
   LDA   #$01
.cmp_end@27:
   LDY   #$70
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
   TXA

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$A7
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
.if_end@13:
   LDX   #$00
   LDA   #$01

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$A7
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$72
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

a_bunch_of_arguments:
   SUBROUTINE

   ; prologue: 20 arg bytes, 42 local bytes
   SEC
   LDA   SSP
   SBC   #$2C
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$2B
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   LDY   #$2E
   LDA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@29
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@29
   LDA   #$00
   JMP   .cmp_end@30
.cmp_true@29:
   LDA   #$01
.cmp_end@30:
   LDY   #$01
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@22
   LDY   #$30
   LDA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@32
   DEY
   LDA   (FP),Y
   CMP   #$01
   BNE   .cmp_true@32
   LDA   #$00
   JMP   .cmp_end@33
.cmp_true@32:
   LDA   #$01
.cmp_end@33:
   LDY   #$03
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@22
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   JMP   .or_end@23
.or_true@22:
   LDA   #$01
   LDY   #$05
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@23:
   LDY   #$05
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@20
   LDY   #$32
   LDA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@35
   DEY
   LDA   (FP),Y
   CMP   #$02
   BNE   .cmp_true@35
   LDA   #$00
   JMP   .cmp_end@36
.cmp_true@35:
   LDA   #$01
.cmp_end@36:
   LDY   #$07
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@20
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   JMP   .or_end@21
.or_true@20:
   LDA   #$01
   LDY   #$09
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@21:
   LDY   #$09
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@18
   LDY   #$34
   LDA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@38
   DEY
   LDA   (FP),Y
   CMP   #$03
   BNE   .cmp_true@38
   LDA   #$00
   JMP   .cmp_end@39
.cmp_true@38:
   LDA   #$01
.cmp_end@39:
   LDY   #$0B
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@18
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   JMP   .or_end@19
.or_true@18:
   LDA   #$01
   LDY   #$0D
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@19:
   LDY   #$0D
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@16
   LDY   #$36
   LDA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@41
   DEY
   LDA   (FP),Y
   CMP   #$04
   BNE   .cmp_true@41
   LDA   #$00
   JMP   .cmp_end@42
.cmp_true@41:
   LDA   #$01
.cmp_end@42:
   LDY   #$0F
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@16
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   JMP   .or_end@17
.or_true@16:
   LDA   #$01
   LDY   #$11
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@17:
   LDY   #$11
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@14
   LDY   #$40
   LDA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@44
   DEY
   LDA   (FP),Y
   CMP   #$05
   BNE   .cmp_true@44
   LDA   #$00
   JMP   .cmp_end@45
.cmp_true@44:
   LDA   #$01
.cmp_end@45:
   LDY   #$13
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
   LDY   #$15
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@15:
   LDY   #$15
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@24
   LDX   #$00
   TXA

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$40
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$2B
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
   CLC
   LDA   FP
   ADC   #$37
   LDY   #$17
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
   LDY   #$19
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$1A
   STA   (FP),Y
   LDY   #$02
   LDA   (DPTR),Y
   LDY   #$1B
   STA   (FP),Y
   LDY   #$03
   LDA   (DPTR),Y
   LDY   #$1C
   STA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@47
   DEY
   LDA   (FP),Y
   CMP   #$12
   BNE   .cmp_true@47
   DEY
   LDA   (FP),Y
   CMP   #$D6
   BNE   .cmp_true@47
   DEY
   LDA   (FP),Y
   CMP   #$87
   BNE   .cmp_true@47
   LDA   #$00
   JMP   .cmp_end@48
.cmp_true@47:
   LDA   #$01
.cmp_end@48:
   LDY   #$1D
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@3
   JMP   .or_true@25
.lb_skip@3:
   CLC
   LDA   FP
   ADC   #$37
   LDY   #$1F
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$04
   LDY   #$21
   STA   (FP),Y
   DEY
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
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$23
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$24
   STA   (FP),Y
   LDY   #$02
   LDA   (DPTR),Y
   LDY   #$25
   STA   (FP),Y
   LDY   #$03
   LDA   (DPTR),Y
   LDY   #$26
   STA   (FP),Y
   CMP   #$05
   BNE   .cmp_true@50
   DEY
   LDA   (FP),Y
   CMP   #$4F
   BNE   .cmp_true@50
   DEY
   LDA   (FP),Y
   CMP   #$93
   BNE   .cmp_true@50
   DEY
   LDA   (FP),Y
   CMP   #$38
   BNE   .cmp_true@50
   LDA   #$00
   JMP   .cmp_end@51
.cmp_true@50:
   LDA   #$01
.cmp_end@51:
   LDY   #$27
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@25
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   JMP   .or_end@26
.or_true@25:
   LDA   #$01
   LDY   #$29
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@26:
   LDY   #$29
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@27
   LDX   #$00
   TXA

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$40
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
.if_end@27:
   LDX   #$00
   LDA   #$01

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$40
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$2B
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

structs_and_scalars:
   SUBROUTINE

   ; prologue: 42 arg bytes, 74 local bytes
   SEC
   LDA   SSP
   SBC   #$4C
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$4B
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   LDA   #$0A
   LDY   #$01
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   ORA   #$00
   BPL   .sx_done@54
   LDA   #$FF
.sx_done@54:
   LDY   #$03
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$50
   LDA   (FP),Y
   LDY   #$04
   CMP   (FP),Y
   BNE   .cmp_true@55
   LDY   #$4F
   LDA   (FP),Y
   LDY   #$03
   CMP   (FP),Y
   BNE   .cmp_true@55
   LDY   #$4E
   LDA   (FP),Y
   LDY   #$02
   CMP   (FP),Y
   BNE   .cmp_true@55
   LDY   #$4D
   LDA   (FP),Y
   LDY   #$01
   CMP   (FP),Y
   BNE   .cmp_true@55
   LDA   #$00
   JMP   .cmp_end@56
.cmp_true@55:
   LDA   #$01
.cmp_end@56:
   LDY   #$05
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
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
   ADC   #$76
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$4B
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
   LDY   #$58
   LDA   (FP),Y
   CMP   #$40
   BNE   .cmp_true@58
   DEY
   LDA   (FP),Y
   CMP   #$24
   BNE   .cmp_true@58
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@58
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@58
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@58
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@58
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@58
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@58
   LDA   #$00
   JMP   .cmp_end@59
.cmp_true@58:
   LDA   #$01
.cmp_end@59:
   LDY   #$07
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
   TXA

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$76
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$4B
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
   CLC
   LDA   FP
   ADC   #$59
   LDY   #$0B
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   LDA   #<str@2
   LDY   #$09
   STA   (FP),Y
   LDA   #>str@2
   INY
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$04
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   INY
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$0C
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$09
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$0A
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   JSR   strcmp
   LDY   #$0D
   STA   (FP),Y
   TXA
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@30
   LDX   #$00
   TXA

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$76
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$4B
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
   CLC
   LDA   FP
   ADC   #$5E
   LDY   #$0F
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$08
   LDY   #$11
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$12
   STA   (FP),Y
   LDA   #<str@3
   INY
   STA   (FP),Y
   LDA   #>str@3
   INY
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$04
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDY   #$11
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$12
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$13
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$14
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   JSR   strcmp
   LDY   #$15
   STA   (FP),Y
   TXA
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@5
   JMP   .or_true@35
.lb_skip@5:
   CLC
   LDA   FP
   ADC   #$5E
   LDY   #$17
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
   LDY   #$19
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$1A
   STA   (FP),Y
   LDY   #$02
   LDA   (DPTR),Y
   LDY   #$1B
   STA   (FP),Y
   LDY   #$03
   LDA   (DPTR),Y
   LDY   #$1C
   STA   (FP),Y
   LDY   #$04
   LDA   (DPTR),Y
   LDY   #$1D
   STA   (FP),Y
   LDY   #$05
   LDA   (DPTR),Y
   LDY   #$1E
   STA   (FP),Y
   LDY   #$06
   LDA   (DPTR),Y
   LDY   #$1F
   STA   (FP),Y
   LDY   #$07
   LDA   (DPTR),Y
   LDY   #$20
   STA   (FP),Y
   CMP   #$40
   BNE   .cmp_true@61
   DEY
   LDA   (FP),Y
   CMP   #$2F
   BNE   .cmp_true@61
   DEY
   LDA   (FP),Y
   CMP   #$80
   BNE   .cmp_true@61
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@61
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@61
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@61
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@61
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@61
   LDA   #$00
   JMP   .cmp_end@62
.cmp_true@61:
   LDA   #$01
.cmp_end@62:
   LDY   #$21
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@35
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   JMP   .or_end@36
.or_true@35:
   LDA   #$01
   LDY   #$23
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@36:
   LDY   #$23
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@33
   CLC
   LDA   FP
   ADC   #$5E
   INY
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$0F
   LDY   #$27
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
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
   CMP   #$0D
   BNE   .cmp_true@64
   DEY
   LDA   (FP),Y
   CMP   #$05
   BNE   .cmp_true@64
   LDA   #$00
   JMP   .cmp_end@65
.cmp_true@64:
   LDA   #$01
.cmp_end@65:
   LDY   #$2B
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@33
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   JMP   .or_end@34
.or_true@33:
   LDA   #$01
   LDY   #$2D
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@34:
   LDY   #$2D
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@4
   JMP   .or_true@31
.lb_skip@4:
   CLC
   LDA   FP
   ADC   #$5E
   LDY   #$2F
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$0B
   LDY   #$31
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$32
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$33
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$34
   STA   (FP),Y
   LDY   #$02
   LDA   (DPTR),Y
   LDY   #$35
   STA   (FP),Y
   LDY   #$03
   LDA   (DPTR),Y
   LDY   #$36
   STA   (FP),Y
   LDA   #$5C
   INY
   STA   (FP),Y
   LDA   #$11
   INY
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@67
   LDA   #$00
   JMP   .sx_done@68
.sx_neg@67:
   LDA   #$FF
.sx_done@68:
   LDY   #$39
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$36
   LDA   (FP),Y
   LDY   #$3A
   CMP   (FP),Y
   BNE   .cmp_true@69
   LDY   #$35
   LDA   (FP),Y
   LDY   #$39
   CMP   (FP),Y
   BNE   .cmp_true@69
   LDY   #$34
   LDA   (FP),Y
   LDY   #$38
   CMP   (FP),Y
   BNE   .cmp_true@69
   LDY   #$33
   LDA   (FP),Y
   LDY   #$37
   CMP   (FP),Y
   BNE   .cmp_true@69
   LDA   #$00
   JMP   .cmp_end@70
.cmp_true@69:
   LDA   #$01
.cmp_end@70:
   LDY   #$3B
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@31
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   JMP   .or_end@32
.or_true@31:
   LDA   #$01
   LDY   #$3D
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@32:
   LDY   #$3D
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@37
   LDX   #$00
   TXA

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$76
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$4B
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
.if_end@37:
   CLC
   LDA   FP
   ADC   #$6F
   LDY   #$3F
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
   LDY   #$41
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$42
   STA   (FP),Y
   LDY   #$02
   LDA   (DPTR),Y
   LDY   #$43
   STA   (FP),Y
   LDY   #$03
   LDA   (DPTR),Y
   LDY   #$44
   STA   (FP),Y
   LDY   #$04
   LDA   (DPTR),Y
   LDY   #$45
   STA   (FP),Y
   LDY   #$05
   LDA   (DPTR),Y
   LDY   #$46
   STA   (FP),Y
   LDY   #$06
   LDA   (DPTR),Y
   LDY   #$47
   STA   (FP),Y
   LDY   #$07
   LDA   (DPTR),Y
   LDY   #$48
   STA   (FP),Y
   CMP   #$40
   BNE   .cmp_true@72
   DEY
   LDA   (FP),Y
   CMP   #$14
   BNE   .cmp_true@72
   DEY
   LDA   (FP),Y
   CMP   #$80
   BNE   .cmp_true@72
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@72
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@72
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@72
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@72
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@72
   LDA   #$00
   JMP   .cmp_end@73
.cmp_true@72:
   LDA   #$01
.cmp_end@73:
   LDY   #$49
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@38
   LDX   #$00
   TXA

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$76
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
.if_end@38:
   LDX   #$00
   LDA   #$01

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$76
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$4B
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

struct_in_mem:
   SUBROUTINE

   ; prologue: 80 arg bytes, 103 local bytes
   SEC
   LDA   SSP
   SBC   #$69
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$68
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   LDY   #$71
   LDA   (FP),Y
   CMP   #$40
   BNE   .cmp_true@75
   DEY
   LDA   (FP),Y
   CMP   #$24
   BNE   .cmp_true@75
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@75
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@75
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@75
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@75
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@75
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@75
   LDA   #$00
   JMP   .cmp_end@76
.cmp_true@75:
   LDA   #$01
.cmp_end@76:
   LDY   #$01
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@41
   LDY   #$79
   LDA   (FP),Y
   CMP   #$40
   BNE   .cmp_true@78
   DEY
   LDA   (FP),Y
   CMP   #$26
   BNE   .cmp_true@78
   DEY
   LDA   (FP),Y
   CMP   #$40
   BNE   .cmp_true@78
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@78
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@78
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@78
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@78
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@78
   LDA   #$00
   JMP   .cmp_end@79
.cmp_true@78:
   LDA   #$01
.cmp_end@79:
   LDY   #$03
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@41
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   JMP   .or_end@42
.or_true@41:
   LDA   #$01
   LDY   #$05
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@42:
   LDY   #$05
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@39
   LDY   #$81
   LDA   (FP),Y
   CMP   #$40
   BNE   .cmp_true@81
   DEY
   LDA   (FP),Y
   CMP   #$28
   BNE   .cmp_true@81
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@81
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@81
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@81
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@81
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@81
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@81
   LDA   #$00
   JMP   .cmp_end@82
.cmp_true@81:
   LDA   #$01
.cmp_end@82:
   LDY   #$07
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@39
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   JMP   .or_end@40
.or_true@39:
   LDA   #$01
   LDY   #$09
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@40:
   LDY   #$09
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@43
   LDX   #$00
   TXA

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$B9
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$68
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
.if_end@43:
   CLC
   LDA   FP
   ADC   #$82
   LDY   #$0B
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$08
   LDY   #$0D
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$0E
   STA   (FP),Y
   LDA   #<str@4
   INY
   STA   (FP),Y
   LDA   #>str@4
   INY
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$04
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDY   #$0D
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$0E
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$0F
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$10
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   JSR   strcmp
   LDY   #$11
   STA   (FP),Y
   TXA
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@8
   JMP   .or_true@44
.lb_skip@8:
   CLC
   LDA   FP
   ADC   #$82
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
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$15
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$16
   STA   (FP),Y
   LDY   #$02
   LDA   (DPTR),Y
   LDY   #$17
   STA   (FP),Y
   LDY   #$03
   LDA   (DPTR),Y
   LDY   #$18
   STA   (FP),Y
   LDY   #$04
   LDA   (DPTR),Y
   LDY   #$19
   STA   (FP),Y
   LDY   #$05
   LDA   (DPTR),Y
   LDY   #$1A
   STA   (FP),Y
   LDY   #$06
   LDA   (DPTR),Y
   LDY   #$1B
   STA   (FP),Y
   LDY   #$07
   LDA   (DPTR),Y
   LDY   #$1C
   STA   (FP),Y
   CMP   #$3F
   BNE   .cmp_true@84
   DEY
   LDA   (FP),Y
   CMP   #$F3
   BNE   .cmp_true@84
   DEY
   LDA   (FP),Y
   CMP   #$BE
   BNE   .cmp_true@84
   DEY
   LDA   (FP),Y
   CMP   #$76
   BNE   .cmp_true@84
   DEY
   LDA   (FP),Y
   CMP   #$C8
   BNE   .cmp_true@84
   DEY
   LDA   (FP),Y
   CMP   #$B4
   BNE   .cmp_true@84
   DEY
   LDA   (FP),Y
   CMP   #$39
   BNE   .cmp_true@84
   DEY
   LDA   (FP),Y
   CMP   #$58
   BNE   .cmp_true@84
   LDA   #$00
   JMP   .cmp_end@85
.cmp_true@84:
   LDA   #$01
.cmp_end@85:
   LDY   #$1D
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@44
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   JMP   .or_end@45
.or_true@44:
   LDA   #$01
   LDY   #$1F
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@45:
   LDY   #$1F
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@46
   LDX   #$00
   TXA

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$B9
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$68
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
.if_end@46:
   LDY   #$94
   LDA   (FP),Y
   CMP   #$40
   BNE   .cmp_true@87
   DEY
   LDA   (FP),Y
   CMP   #$2A
   BNE   .cmp_true@87
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@87
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@87
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@87
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@87
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@87
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@87
   LDA   #$00
   JMP   .cmp_end@88
.cmp_true@87:
   LDA   #$01
.cmp_end@88:
   LDY   #$21
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@47
   LDX   #$00
   TXA

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$B9
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$68
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
.if_end@47:
   CLC
   LDA   FP
   ADC   #$95
   LDY   #$25
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   LDA   #$00
   STA   HARGS
   LDX   #$08
   LSR   HARGS
.mul_loop@90:
   BCC   .mul_skip@91
   CLC
   ADC   #$08
.mul_skip@91:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@90
   LDY   #$24
   STA   (FP),Y
   LDA   HARGS
   DEY
   STA   (FP),Y
   LDY   #$25
   LDA   (FP),Y
   CLC
   LDY   #$23
   ADC   (FP),Y
   LDY   #$27
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$24
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
   LDY   #$02
   LDA   (DPTR),Y
   LDY   #$2B
   STA   (FP),Y
   LDY   #$03
   LDA   (DPTR),Y
   LDY   #$2C
   STA   (FP),Y
   LDY   #$04
   LDA   (DPTR),Y
   LDY   #$2D
   STA   (FP),Y
   LDY   #$05
   LDA   (DPTR),Y
   LDY   #$2E
   STA   (FP),Y
   LDY   #$06
   LDA   (DPTR),Y
   LDY   #$2F
   STA   (FP),Y
   LDY   #$07
   LDA   (DPTR),Y
   LDY   #$30
   STA   (FP),Y
   CMP   #$40
   BNE   .cmp_true@92
   DEY
   LDA   (FP),Y
   CMP   #$4B
   BNE   .cmp_true@92
   DEY
   LDA   (FP),Y
   CMP   #$C0
   BNE   .cmp_true@92
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@92
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@92
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@92
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@92
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@92
   LDA   #$00
   JMP   .cmp_end@93
.cmp_true@92:
   LDA   #$01
.cmp_end@93:
   LDY   #$31
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@7
   JMP   .or_true@48
.lb_skip@7:
   CLC
   LDA   FP
   ADC   #$95
   LDY   #$35
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
.mul_loop@95:
   BCC   .mul_skip@96
   CLC
   ADC   #$08
.mul_skip@96:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@95
   LDY   #$34
   STA   (FP),Y
   LDA   HARGS
   DEY
   STA   (FP),Y
   LDY   #$35
   LDA   (FP),Y
   CLC
   LDY   #$33
   ADC   (FP),Y
   LDY   #$37
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$34
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
   LDY   #$04
   LDA   (DPTR),Y
   LDY   #$3D
   STA   (FP),Y
   LDY   #$05
   LDA   (DPTR),Y
   LDY   #$3E
   STA   (FP),Y
   LDY   #$06
   LDA   (DPTR),Y
   LDY   #$3F
   STA   (FP),Y
   LDY   #$07
   LDA   (DPTR),Y
   LDY   #$40
   STA   (FP),Y
   CMP   #$40
   BNE   .cmp_true@97
   DEY
   LDA   (FP),Y
   CMP   #$46
   BNE   .cmp_true@97
   DEY
   LDA   (FP),Y
   CMP   #$33
   BNE   .cmp_true@97
   DEY
   LDA   (FP),Y
   CMP   #$33
   BNE   .cmp_true@97
   DEY
   LDA   (FP),Y
   CMP   #$33
   BNE   .cmp_true@97
   DEY
   LDA   (FP),Y
   CMP   #$33
   BNE   .cmp_true@97
   DEY
   LDA   (FP),Y
   CMP   #$33
   BNE   .cmp_true@97
   DEY
   LDA   (FP),Y
   CMP   #$33
   BNE   .cmp_true@97
   LDA   #$00
   JMP   .cmp_end@98
.cmp_true@97:
   LDA   #$01
.cmp_end@98:
   LDY   #$41
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@48
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   JMP   .or_end@49
.or_true@48:
   LDA   #$01
   LDY   #$43
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@49:
   LDY   #$43
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@50
   LDX   #$00
   TXA

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$B9
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$68
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
.if_end@50:
   LDY   #$A5
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   INY
   ORA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@51
   LDX   #$00
   TXA

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$B9
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$68
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
.if_end@51:
   CLC
   LDA   FP
   ADC   #$A9
   LDY   #$45
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
   LDY   #$47
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CMP   #$70
   BNE   .cmp_true@100
   LDA   #$00
   JMP   .cmp_end@101
.cmp_true@100:
   LDA   #$01
.cmp_end@101:
   LDY   #$4A
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@6
   JMP   .or_true@52
.lb_skip@6:
   CLC
   LDA   FP
   ADC   #$A9
   LDY   #$4C
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$01
   LDY   #$4E
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$4F
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$50
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$51
   STA   (FP),Y
   LDY   #$02
   LDA   (DPTR),Y
   LDY   #$52
   STA   (FP),Y
   LDY   #$03
   LDA   (DPTR),Y
   LDY   #$53
   STA   (FP),Y
   LDY   #$04
   LDA   (DPTR),Y
   LDY   #$54
   STA   (FP),Y
   LDY   #$05
   LDA   (DPTR),Y
   LDY   #$55
   STA   (FP),Y
   LDY   #$06
   LDA   (DPTR),Y
   LDY   #$56
   STA   (FP),Y
   LDY   #$07
   LDA   (DPTR),Y
   LDY   #$57
   STA   (FP),Y
   CMP   #$40
   BNE   .cmp_true@102
   DEY
   LDA   (FP),Y
   CMP   #$12
   BNE   .cmp_true@102
   DEY
   LDA   (FP),Y
   CMP   #$3D
   BNE   .cmp_true@102
   DEY
   LDA   (FP),Y
   CMP   #$70
   BNE   .cmp_true@102
   DEY
   LDA   (FP),Y
   CMP   #$A3
   BNE   .cmp_true@102
   DEY
   LDA   (FP),Y
   CMP   #$D7
   BNE   .cmp_true@102
   DEY
   LDA   (FP),Y
   CMP   #$0A
   BNE   .cmp_true@102
   DEY
   LDA   (FP),Y
   CMP   #$3D
   BNE   .cmp_true@102
   LDA   #$00
   JMP   .cmp_end@103
.cmp_true@102:
   LDA   #$01
.cmp_end@103:
   LDY   #$58
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@52
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   JMP   .or_end@53
.or_true@52:
   LDA   #$01
   LDY   #$5A
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@53:
   LDY   #$5A
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@54
   LDX   #$00
   TXA

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$B9
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$68
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
.if_end@54:
   CLC
   LDA   FP
   ADC   #$B2
   LDY   #$5C
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
   LDY   #$5E
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$5F
   STA   (FP),Y
   LDY   #$02
   LDA   (DPTR),Y
   LDY   #$60
   STA   (FP),Y
   LDY   #$03
   LDA   (DPTR),Y
   LDY   #$61
   STA   (FP),Y
   LDY   #$04
   LDA   (DPTR),Y
   LDY   #$62
   STA   (FP),Y
   LDY   #$05
   LDA   (DPTR),Y
   LDY   #$63
   STA   (FP),Y
   LDY   #$06
   LDA   (DPTR),Y
   LDY   #$64
   STA   (FP),Y
   LDY   #$07
   LDA   (DPTR),Y
   LDY   #$65
   STA   (FP),Y
   CMP   #$40
   BNE   .cmp_true@105
   DEY
   LDA   (FP),Y
   CMP   #$14
   BNE   .cmp_true@105
   DEY
   LDA   (FP),Y
   CMP   #$80
   BNE   .cmp_true@105
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@105
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@105
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@105
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@105
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@105
   LDA   #$00
   JMP   .cmp_end@106
.cmp_true@105:
   LDA   #$01
.cmp_end@106:
   LDY   #$66
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@55
   LDX   #$00
   TXA

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$B9
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
.if_end@55:
   LDX   #$00
   LDA   #$01

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$B9
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$68
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

pass_borderline_struct_in_memory:
   SUBROUTINE

   ; prologue: 34 arg bytes, 122 local bytes
   SEC
   LDA   SSP
   SBC   #$7C
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$7B
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
   ADC   #$7D
   LDY   #$01
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
   LDY   #$03
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CMP   #$5F
   BNE   .cmp_true@108
   LDA   #$00
   JMP   .cmp_end@109
.cmp_true@108:
   LDA   #$01
.cmp_end@109:
   LDY   #$06
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@14
   JMP   .or_true@60
.lb_skip@14:
   CLC
   LDA   FP
   ADC   #$7D
   LDY   #$08
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$01
   LDY   #$0A
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$0B
   STA   (FP),Y
   LDA   #$00
   STA   HARGS
   LDX   #$08
   LSR   HARGS
.mul_loop@110:
   BCC   .mul_skip@111
   CLC
   ADC   #$02
.mul_skip@111:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@110
   LDY   #$0D
   STA   (FP),Y
   LDA   HARGS
   DEY
   STA   (FP),Y
   LDY   #$0A
   LDA   (FP),Y
   CLC
   LDY   #$0C
   ADC   (FP),Y
   LDY   #$0E
   STA   (FP),Y
   LDY   #$0B
   LDA   (FP),Y
   LDY   #$0D
   ADC   (FP),Y
   LDY   #$0F
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$10
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$11
   STA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@112
   DEY
   LDA   (FP),Y
   CMP   #$05
   BNE   .cmp_true@112
   LDA   #$00
   JMP   .cmp_end@113
.cmp_true@112:
   LDA   #$01
.cmp_end@113:
   LDY   #$12
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@60
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   JMP   .or_end@61
.or_true@60:
   LDA   #$01
   LDY   #$14
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@61:
   LDY   #$14
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@13
   JMP   .or_true@58
.lb_skip@13:
   CLC
   LDA   FP
   ADC   #$7D
   LDY   #$16
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$01
   LDY   #$18
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$19
   STA   (FP),Y
   LDA   #$01
   STA   HARGS
   LDA   #$00
   LDX   #$08
   LSR   HARGS
.mul_loop@115:
   BCC   .mul_skip@116
   CLC
   ADC   #$02
.mul_skip@116:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@115
   LDY   #$1B
   STA   (FP),Y
   LDA   HARGS
   DEY
   STA   (FP),Y
   LDY   #$18
   LDA   (FP),Y
   CLC
   LDY   #$1A
   ADC   (FP),Y
   LDY   #$1C
   STA   (FP),Y
   LDY   #$19
   LDA   (FP),Y
   LDY   #$1B
   ADC   (FP),Y
   LDY   #$1D
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$1E
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$1F
   STA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@117
   DEY
   LDA   (FP),Y
   CMP   #$06
   BNE   .cmp_true@117
   LDA   #$00
   JMP   .cmp_end@118
.cmp_true@117:
   LDA   #$01
.cmp_end@118:
   LDY   #$20
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@58
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   JMP   .or_end@59
.or_true@58:
   LDA   #$01
   LDY   #$22
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@59:
   LDY   #$22
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@12
   JMP   .or_true@56
.lb_skip@12:
   CLC
   LDA   FP
   ADC   #$7D
   LDY   #$24
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$01
   LDY   #$26
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$27
   STA   (FP),Y
   LDA   #$02
   STA   HARGS
   LDA   #$00
   LDX   #$08
   LSR   HARGS
.mul_loop@120:
   BCC   .mul_skip@121
   CLC
   ADC   #$02
.mul_skip@121:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@120
   LDY   #$29
   STA   (FP),Y
   LDA   HARGS
   DEY
   STA   (FP),Y
   LDY   #$26
   LDA   (FP),Y
   CLC
   LDY   #$28
   ADC   (FP),Y
   LDY   #$2A
   STA   (FP),Y
   LDY   #$27
   LDA   (FP),Y
   LDY   #$29
   ADC   (FP),Y
   LDY   #$2B
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$2C
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$2D
   STA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@122
   DEY
   LDA   (FP),Y
   CMP   #$07
   BNE   .cmp_true@122
   LDA   #$00
   JMP   .cmp_end@123
.cmp_true@122:
   LDA   #$01
.cmp_end@123:
   LDY   #$2E
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@56
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   JMP   .or_end@57
.or_true@56:
   LDA   #$01
   LDY   #$30
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@57:
   LDY   #$30
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@62
   LDX   #$00
   TXA

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$9E
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$7B
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
.if_end@62:
   LDY   #$85
   LDA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@125
   DEY
   LDA   (FP),Y
   CMP   #$21
   BNE   .cmp_true@125
   LDA   #$00
   JMP   .cmp_end@126
.cmp_true@125:
   LDA   #$01
.cmp_end@126:
   LDY   #$32
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@63
   LDX   #$00
   TXA

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$9E
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$7B
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
.if_end@63:
   CLC
   LDA   FP
   ADC   #$86
   LDY   #$34
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
   LDY   #$36
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CMP   #$70
   BNE   .cmp_true@128
   LDA   #$00
   JMP   .cmp_end@129
.cmp_true@128:
   LDA   #$01
.cmp_end@129:
   LDY   #$39
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@11
   JMP   .or_true@64
.lb_skip@11:
   CLC
   LDA   FP
   ADC   #$86
   LDY   #$3B
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$01
   LDY   #$3D
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$3E
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$3F
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$40
   STA   (FP),Y
   LDY   #$02
   LDA   (DPTR),Y
   LDY   #$41
   STA   (FP),Y
   LDY   #$03
   LDA   (DPTR),Y
   LDY   #$42
   STA   (FP),Y
   LDY   #$04
   LDA   (DPTR),Y
   LDY   #$43
   STA   (FP),Y
   LDY   #$05
   LDA   (DPTR),Y
   LDY   #$44
   STA   (FP),Y
   LDY   #$06
   LDA   (DPTR),Y
   LDY   #$45
   STA   (FP),Y
   LDY   #$07
   LDA   (DPTR),Y
   LDY   #$46
   STA   (FP),Y
   CMP   #$40
   BNE   .cmp_true@130
   DEY
   LDA   (FP),Y
   CMP   #$12
   BNE   .cmp_true@130
   DEY
   LDA   (FP),Y
   CMP   #$3D
   BNE   .cmp_true@130
   DEY
   LDA   (FP),Y
   CMP   #$70
   BNE   .cmp_true@130
   DEY
   LDA   (FP),Y
   CMP   #$A3
   BNE   .cmp_true@130
   DEY
   LDA   (FP),Y
   CMP   #$D7
   BNE   .cmp_true@130
   DEY
   LDA   (FP),Y
   CMP   #$0A
   BNE   .cmp_true@130
   DEY
   LDA   (FP),Y
   CMP   #$3D
   BNE   .cmp_true@130
   LDA   #$00
   JMP   .cmp_end@131
.cmp_true@130:
   LDA   #$01
.cmp_end@131:
   LDY   #$47
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@64
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   JMP   .or_end@65
.or_true@64:
   LDA   #$01
   LDY   #$49
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@65:
   LDY   #$49
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@66
   LDX   #$00
   TXA

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$9E
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$7B
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
.if_end@66:
   LDY   #$8F
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@67
   LDX   #$00
   TXA

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$9E
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$7B
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
.if_end@67:
   CLC
   LDA   FP
   ADC   #$91
   LDY   #$4B
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$02
   LDY   #$4D
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$4E
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$4F
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CMP   #$63
   BNE   .cmp_true@133
   LDA   #$00
   JMP   .cmp_end@134
.cmp_true@133:
   LDA   #$01
.cmp_end@134:
   LDY   #$52
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@10
   JMP   .or_true@68
.lb_skip@10:
   CLC
   LDA   FP
   ADC   #$91
   LDY   #$54
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
   LDY   #$56
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
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
   BMI   .sx_neg@135
   LDA   #$00
   JMP   .sx_done@136
.sx_neg@135:
   LDA   #$FF
.sx_done@136:
   LDY   #$5A
   STA   (FP),Y
   INY
   STA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@137
   LDY   #$59
   LDA   (FP),Y
   CMP   #$D4
   BNE   .cmp_true@137
   DEY
   LDA   (FP),Y
   CMP   #$30
   BNE   .cmp_true@137
   LDA   #$00
   JMP   .cmp_end@138
.cmp_true@137:
   LDA   #$01
.cmp_end@138:
   LDY   #$5C
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@68
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   JMP   .or_end@69
.or_true@68:
   LDA   #$01
   LDY   #$5E
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@69:
   LDY   #$5E
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@70
   LDX   #$00
   TXA

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$9E
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$7B
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
.if_end@70:
   CLC
   LDA   FP
   ADC   #$91
   LDY   #$60
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$03
   LDY   #$62
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$63
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$02
   LDY   #$64
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$65
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$66
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CMP   #$63
   BNE   .cmp_true@140
   LDA   #$00
   JMP   .cmp_end@141
.cmp_true@140:
   LDA   #$01
.cmp_end@141:
   LDY   #$69
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@9
   JMP   .or_true@71
.lb_skip@9:
   CLC
   LDA   FP
   ADC   #$91
   LDY   #$6B
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$03
   LDY   #$6D
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$6E
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$6F
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$70
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$71
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$72
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@142
   LDA   #$00
   JMP   .sx_done@143
.sx_neg@142:
   LDA   #$FF
.sx_done@143:
   LDY   #$73
   STA   (FP),Y
   INY
   STA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@144
   LDY   #$72
   LDA   (FP),Y
   CMP   #$D4
   BNE   .cmp_true@144
   DEY
   LDA   (FP),Y
   CMP   #$30
   BNE   .cmp_true@144
   LDA   #$00
   JMP   .cmp_end@145
.cmp_true@144:
   LDA   #$01
.cmp_end@145:
   LDY   #$75
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@71
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   JMP   .or_end@72
.or_true@71:
   LDA   #$01
   LDY   #$77
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@72:
   LDY   #$77
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@73
   LDX   #$00
   TXA

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$9E
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$7B
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
.if_end@73:
   LDY   #$9E
   LDA   (FP),Y
   CMP   #$40
   BNE   .cmp_true@147
   DEY
   LDA   (FP),Y
   CMP   #$1F
   BNE   .cmp_true@147
   DEY
   LDA   (FP),Y
   CMP   #$33
   BNE   .cmp_true@147
   DEY
   LDA   (FP),Y
   CMP   #$33
   BNE   .cmp_true@147
   DEY
   LDA   (FP),Y
   CMP   #$33
   BNE   .cmp_true@147
   DEY
   LDA   (FP),Y
   CMP   #$33
   BNE   .cmp_true@147
   DEY
   LDA   (FP),Y
   CMP   #$33
   BNE   .cmp_true@147
   DEY
   LDA   (FP),Y
   CMP   #$33
   BNE   .cmp_true@147
   LDA   #$00
   JMP   .cmp_end@148
.cmp_true@147:
   LDA   #$01
.cmp_end@148:
   LDY   #$79
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@74
   LDX   #$00
   TXA

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$9E
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
.if_end@74:
   LDX   #$00
   LDA   #$01

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$9E
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$7B
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

pass_uneven_struct_in_mem:
   SUBROUTINE

   ; prologue: 50 arg bytes, 213 local bytes
   SEC
   LDA   SSP
   SBC   #$D7
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$D6
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
   ADC   #$D8
   LDY   #$01
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
   LDY   #$03
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$04
   STA   (FP),Y
   SEC
   LDA   #$00
   SBC   #$01
   INY
   STA   (FP),Y
   LDA   #$00
   SBC   #$00
   INY
   STA   (FP),Y
   LDY   #$04
   LDA   (FP),Y
   LDY   #$06
   CMP   (FP),Y
   BNE   .cmp_true@150
   LDY   #$03
   LDA   (FP),Y
   LDY   #$05
   CMP   (FP),Y
   BNE   .cmp_true@150
   LDA   #$00
   JMP   .cmp_end@151
.cmp_true@150:
   LDA   #$01
.cmp_end@151:
   LDY   #$07
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@75
   LDX   #$00
   TXA

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$09
   STA   SSP
   LDA   FP+1
   ADC   #$01
   STA   SSP+1
   LDY   #$D6
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
.if_end@75:
   CLC
   LDA   FP
   ADC   #$D8
   LDY   #$09
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$02
   LDY   #$0B
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$0C
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
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
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$0F
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CMP   #$7F
   BNE   .cmp_true@153
   LDA   #$00
   JMP   .cmp_end@154
.cmp_true@153:
   LDA   #$01
.cmp_end@154:
   LDY   #$12
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@22
   JMP   .or_true@78
.lb_skip@22:
   CLC
   LDA   FP
   ADC   #$D8
   LDY   #$14
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$02
   LDY   #$16
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$17
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$01
   LDY   #$18
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$19
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$1A
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CMP   #$7E
   BNE   .cmp_true@155
   LDA   #$00
   JMP   .cmp_end@156
.cmp_true@155:
   LDA   #$01
.cmp_end@156:
   LDY   #$1D
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@78
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   JMP   .or_end@79
.or_true@78:
   LDA   #$01
   LDY   #$1F
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@79:
   LDY   #$1F
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@21
   JMP   .or_true@76
.lb_skip@21:
   CLC
   LDA   FP
   ADC   #$D8
   LDY   #$21
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$02
   LDY   #$23
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$24
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$02
   LDY   #$25
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
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
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CMP   #$7D
   BNE   .cmp_true@157
   LDA   #$00
   JMP   .cmp_end@158
.cmp_true@157:
   LDA   #$01
.cmp_end@158:
   LDY   #$2A
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@76
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   JMP   .or_end@77
.or_true@76:
   LDA   #$01
   LDY   #$2C
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@77:
   LDY   #$2C
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@80
   LDX   #$00
   TXA

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$09
   STA   SSP
   LDA   FP+1
   ADC   #$01
   STA   SSP+1
   LDY   #$D6
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
.if_end@80:
   LDY   #$E2
   LDA   (FP),Y
   LDY   #$2E
   STA   (FP),Y
   LDY   #$E3
   LDA   (FP),Y
   LDY   #$2F
   STA   (FP),Y
   LDY   #$E4
   LDA   (FP),Y
   LDY   #$30
   STA   (FP),Y
   LDY   #$E5
   LDA   (FP),Y
   LDY   #$31
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@159
   LDA   #$00
   JMP   .sx_done@160
.sx_neg@159:
   LDA   #$FF
.sx_done@160:
   LDY   #$32
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   CMP   #$7F
   BNE   .cmp_true@161
   CMP   #$FF
   BNE   .cmp_true@161
   LDY   #$31
   LDA   (FP),Y
   CMP   #$FF
   BNE   .cmp_true@161
   DEY
   LDA   (FP),Y
   CMP   #$FF
   BNE   .cmp_true@161
   DEY
   LDA   (FP),Y
   CMP   #$FF
   BNE   .cmp_true@161
   DEY
   LDA   (FP),Y
   CMP   #$FD
   BNE   .cmp_true@161
   LDA   #$00
   JMP   .cmp_end@162
.cmp_true@161:
   LDA   #$01
.cmp_end@162:
   LDY   #$36
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@20
   JMP   .or_true@81
.lb_skip@20:
   LDY   #$E6
   LDA   (FP),Y
   LDY   #$38
   STA   (FP),Y
   LDY   #$E7
   LDA   (FP),Y
   LDY   #$39
   STA   (FP),Y
   LDY   #$E8
   LDA   (FP),Y
   LDY   #$3A
   STA   (FP),Y
   LDY   #$E9
   LDA   (FP),Y
   LDY   #$3B
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@164
   LDA   #$00
   JMP   .sx_done@165
.sx_neg@164:
   LDA   #$FF
.sx_done@165:
   LDY   #$3C
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   CMP   #$7F
   BNE   .cmp_true@166
   CMP   #$FF
   BNE   .cmp_true@166
   LDY   #$3B
   LDA   (FP),Y
   CMP   #$FF
   BNE   .cmp_true@166
   DEY
   LDA   (FP),Y
   CMP   #$FF
   BNE   .cmp_true@166
   DEY
   LDA   (FP),Y
   CMP   #$FF
   BNE   .cmp_true@166
   DEY
   LDA   (FP),Y
   CMP   #$F8
   BNE   .cmp_true@166
   LDA   #$00
   JMP   .cmp_end@167
.cmp_true@166:
   LDA   #$01
.cmp_end@167:
   LDY   #$40
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@81
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   JMP   .or_end@82
.or_true@81:
   LDA   #$01
   LDY   #$42
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@82:
   LDY   #$42
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@83
   LDX   #$00
   TXA

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$09
   STA   SSP
   LDA   FP+1
   ADC   #$01
   STA   SSP+1
   LDY   #$D6
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
.if_end@83:
   CLC
   LDA   FP
   ADC   #$EA
   LDY   #$44
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
   LDY   #$46
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$47
   STA   (FP),Y
   SEC
   LDA   #$00
   SBC   #$05
   INY
   STA   (FP),Y
   LDA   #$00
   SBC   #$00
   INY
   STA   (FP),Y
   LDY   #$47
   LDA   (FP),Y
   LDY   #$49
   CMP   (FP),Y
   BNE   .cmp_true@169
   LDY   #$46
   LDA   (FP),Y
   LDY   #$48
   CMP   (FP),Y
   BNE   .cmp_true@169
   LDA   #$00
   JMP   .cmp_end@170
.cmp_true@169:
   LDA   #$01
.cmp_end@170:
   LDY   #$4A
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@84
   LDX   #$00
   TXA

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$09
   STA   SSP
   LDA   FP+1
   ADC   #$01
   STA   SSP+1
   LDY   #$D6
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
.if_end@84:
   CLC
   LDA   FP
   ADC   #$EA
   LDY   #$4C
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$02
   LDY   #$4E
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$4F
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   LDY   #$50
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$51
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$52
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CMP   #$64
   BNE   .cmp_true@172
   LDA   #$00
   JMP   .cmp_end@173
.cmp_true@172:
   LDA   #$01
.cmp_end@173:
   LDY   #$55
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@19
   JMP   .or_true@87
.lb_skip@19:
   CLC
   LDA   FP
   ADC   #$EA
   LDY   #$57
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$02
   LDY   #$59
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$5A
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$01
   LDY   #$5B
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$5C
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$5D
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CMP   #$65
   BNE   .cmp_true@174
   LDA   #$00
   JMP   .cmp_end@175
.cmp_true@174:
   LDA   #$01
.cmp_end@175:
   LDY   #$60
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@87
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   JMP   .or_end@88
.or_true@87:
   LDA   #$01
   LDY   #$62
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@88:
   LDY   #$62
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@18
   JMP   .or_true@85
.lb_skip@18:
   CLC
   LDA   FP
   ADC   #$EA
   LDY   #$64
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$02
   LDY   #$66
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$67
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$02
   LDY   #$68
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$69
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$6A
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CMP   #$66
   BNE   .cmp_true@176
   LDA   #$00
   JMP   .cmp_end@177
.cmp_true@176:
   LDA   #$01
.cmp_end@177:
   LDY   #$6D
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@85
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   JMP   .or_end@86
.or_true@85:
   LDA   #$01
   LDY   #$6F
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@86:
   LDY   #$6F
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@89
   LDX   #$00
   TXA

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$09
   STA   SSP
   LDA   FP+1
   ADC   #$01
   STA   SSP+1
   LDY   #$D6
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
.if_end@89:
   LDA   #$00
   LDY   #$71
   STA   (FP),Y
   INY
   STA   (FP),Y
.loop@0_start:
   LDY   #$71
   LDA   (FP),Y
   SEC
   SBC   #$05
   INY
   LDA   (FP),Y
   SBC   #$00
   BVC   .cmp_novf@178
   EOR   #$80
.cmp_novf@178:
   ASL
   LDA   #$00
   ROL
   LDY   #$73
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@17
   JMP   .loop@0_break
.lb_skip@17:
   CLC
   LDA   FP
   ADC   #$F4
   LDY   #$78
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   LDY   #$71
   LDA   (FP),Y
   LDY   #$75
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   LDA   (FP),Y
   CLC
   LDY   #$76
   ADC   (FP),Y
   LDY   #$7A
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$77
   ADC   (FP),Y
   LDY   #$7B
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$7C
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDA   #$64
   SEC
   LDY   #$71
   SBC   (FP),Y
   LDY   #$7F
   STA   (FP),Y
   LDA   #$00
   LDY   #$72
   SBC   (FP),Y
   LDY   #$80
   STA   (FP),Y
   LDY   #$7E
   LDA   (FP),Y
   LDY   #$80
   CMP   (FP),Y
   BNE   .cmp_true@179
   LDY   #$7D
   LDA   (FP),Y
   LDY   #$7F
   CMP   (FP),Y
   BNE   .cmp_true@179
   LDA   #$00
   JMP   .cmp_end@180
.cmp_true@179:
   LDA   #$01
.cmp_end@180:
   LDY   #$81
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@90
   LDX   #$00
   TXA

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$09
   STA   SSP
   LDA   FP+1
   ADC   #$01
   STA   SSP+1
   LDY   #$D6
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
.if_end@90:
   LDY   #$71
   LDA   (FP),Y
   CLC
   ADC   #$01
   LDY   #$83
   STA   (FP),Y
   LDY   #$72
   LDA   (FP),Y
   ADC   #$00
   LDY   #$84
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$71
   STA   (FP),Y
   LDY   #$84
   LDA   (FP),Y
   LDY   #$72
   STA   (FP),Y
   JMP   .loop@0_start
.loop@0_break:
   CLC
   LDA   FP
   ADC   #$F9
   LDY   #$85
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
   LDY   #$04
   LDA   (DPTR),Y
   LDY   #$8B
   STA   (FP),Y
   LDY   #$05
   LDA   (DPTR),Y
   LDY   #$8C
   STA   (FP),Y
   LDY   #$06
   LDA   (DPTR),Y
   LDY   #$8D
   STA   (FP),Y
   LDY   #$07
   LDA   (DPTR),Y
   LDY   #$8E
   STA   (FP),Y
   CMP   #$40
   BNE   .cmp_true@182
   DEY
   LDA   (FP),Y
   CMP   #$15
   BNE   .cmp_true@182
   DEY
   LDA   (FP),Y
   CMP   #$61
   BNE   .cmp_true@182
   DEY
   LDA   (FP),Y
   CMP   #$47
   BNE   .cmp_true@182
   DEY
   LDA   (FP),Y
   CMP   #$AE
   BNE   .cmp_true@182
   DEY
   LDA   (FP),Y
   CMP   #$14
   BNE   .cmp_true@182
   DEY
   LDA   (FP),Y
   CMP   #$7A
   BNE   .cmp_true@182
   DEY
   LDA   (FP),Y
   CMP   #$E1
   BNE   .cmp_true@182
   LDA   #$00
   JMP   .cmp_end@183
.cmp_true@182:
   LDA   #$01
.cmp_end@183:
   LDY   #$8F
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@91
   LDX   #$00
   TXA

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$09
   STA   SSP
   LDA   FP+1
   ADC   #$01
   STA   SSP+1
   LDY   #$D6
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
.if_end@91:
   CLC
   LDA   FP
   ADC   #$F9
   LDY   #$91
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$08
   LDY   #$93
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$94
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   LDY   #$95
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$96
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$97
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
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
   LDY   #$99
   LDA   (FP),Y
   LDY   #$9B
   CMP   (FP),Y
   BNE   .cmp_true@185
   LDY   #$98
   LDA   (FP),Y
   LDY   #$9A
   CMP   (FP),Y
   BNE   .cmp_true@185
   LDA   #$00
   JMP   .cmp_end@186
.cmp_true@185:
   LDA   #$01
.cmp_end@186:
   LDY   #$9C
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@16
   JMP   .or_true@94
.lb_skip@16:
   CLC
   LDA   FP
   ADC   #$F9
   LDY   #$9E
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$08
   LDY   #$A0
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$A1
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$01
   LDY   #$A2
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
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
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   SEC
   SBC   #$02
   INY
   STA   (FP),Y
   LDA   #$00
   SBC   #$00
   INY
   STA   (FP),Y
   LDY   #$A6
   LDA   (FP),Y
   LDY   #$A8
   CMP   (FP),Y
   BNE   .cmp_true@188
   LDY   #$A5
   LDA   (FP),Y
   LDY   #$A7
   CMP   (FP),Y
   BNE   .cmp_true@188
   LDA   #$00
   JMP   .cmp_end@189
.cmp_true@188:
   LDA   #$01
.cmp_end@189:
   LDY   #$A9
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@94
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   JMP   .or_end@95
.or_true@94:
   LDA   #$01
   LDY   #$AB
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@95:
   LDY   #$AB
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@15
   JMP   .or_true@92
.lb_skip@15:
   CLC
   LDA   FP
   ADC   #$F9
   LDY   #$AD
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$08
   LDY   #$AF
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$B0
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$02
   LDY   #$B1
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$B2
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$B3
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   SEC
   SBC   #$03
   INY
   STA   (FP),Y
   LDA   #$00
   SBC   #$00
   INY
   STA   (FP),Y
   LDY   #$B5
   LDA   (FP),Y
   LDY   #$B7
   CMP   (FP),Y
   BNE   .cmp_true@191
   LDY   #$B4
   LDA   (FP),Y
   LDY   #$B6
   CMP   (FP),Y
   BNE   .cmp_true@191
   LDA   #$00
   JMP   .cmp_end@192
.cmp_true@191:
   LDA   #$01
.cmp_end@192:
   LDY   #$B8
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@92
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   JMP   .or_end@93
.or_true@92:
   LDA   #$01
   LDY   #$BA
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@93:
   LDY   #$BA
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@96
   LDX   #$00
   TXA

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$09
   STA   SSP
   LDA   FP+1
   ADC   #$01
   STA   SSP+1
   LDY   #$D6
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
.if_end@96:
   CLC
   LDA   FP
   ADC   #$F9
   LDY   #$BC
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$0B
   LDY   #$BE
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$BF
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$C0
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$C1
   STA   (FP),Y
   LDY   #$02
   LDA   (DPTR),Y
   LDY   #$C2
   STA   (FP),Y
   LDY   #$03
   LDA   (DPTR),Y
   LDY   #$C3
   STA   (FP),Y
   LDY   #$C0
   LDA   (FP),Y
   LDY   #$C4
   STA   (FP),Y
   LDY   #$C1
   LDA   (FP),Y
   LDY   #$C5
   STA   (FP),Y
   LDY   #$C2
   LDA   (FP),Y
   LDY   #$C6
   STA   (FP),Y
   LDY   #$C3
   LDA   (FP),Y
   LDY   #$C7
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@194
   LDA   #$00
   JMP   .sx_done@195
.sx_neg@194:
   LDA   #$FF
.sx_done@195:
   LDY   #$C8
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@196
   CMP   #$01
   BNE   .cmp_true@196
   LDY   #$C7
   LDA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@196
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@196
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@196
   DEY
   LDA   (FP),Y
   CMP   #$04
   BNE   .cmp_true@196
   LDA   #$00
   JMP   .cmp_end@197
.cmp_true@196:
   LDA   #$01
.cmp_end@197:
   LDY   #$CC
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@97
   LDX   #$00
   TXA

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$09
   STA   SSP
   LDA   FP+1
   ADC   #$01
   STA   SSP+1
   LDY   #$D6
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
.if_end@97:
   CLC
   LDA   FP
   ADC   #$F9
   LDY   #$CE
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$0F
   LDY   #$D0
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$D1
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$D2
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$D3
   STA   (FP),Y
   CMP   #$27
   BNE   .cmp_true@199
   DEY
   LDA   (FP),Y
   CMP   #$10
   BNE   .cmp_true@199
   LDA   #$00
   JMP   .cmp_end@200
.cmp_true@199:
   LDA   #$01
.cmp_end@200:
   LDY   #$D4
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@98
   LDX   #$00
   TXA

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$09
   STA   SSP
   LDA   FP+1
   ADC   #$01
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
.if_end@98:
   LDX   #$00
   LDA   #$01

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$09
   STA   SSP
   LDA   FP+1
   ADC   #$01
   STA   SSP+1
   LDY   #$D6
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

pass_later_structs_in_regs:
   SUBROUTINE

   ; prologue: 35 arg bytes, 138 local bytes
   SEC
   LDA   SSP
   SBC   #$8C
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$8B
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
   ADC   #$8D
   LDY   #$01
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
   LDY   #$03
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$04
   STA   (FP),Y
   LDY   #$02
   LDA   (DPTR),Y
   LDY   #$05
   STA   (FP),Y
   LDY   #$03
   LDA   (DPTR),Y
   LDY   #$06
   STA   (FP),Y
   LDY   #$04
   LDA   (DPTR),Y
   LDY   #$07
   STA   (FP),Y
   LDY   #$05
   LDA   (DPTR),Y
   LDY   #$08
   STA   (FP),Y
   LDY   #$06
   LDA   (DPTR),Y
   LDY   #$09
   STA   (FP),Y
   LDY   #$07
   LDA   (DPTR),Y
   LDY   #$0A
   STA   (FP),Y
   CMP   #$40
   BNE   .cmp_true@202
   DEY
   LDA   (FP),Y
   CMP   #$15
   BNE   .cmp_true@202
   DEY
   LDA   (FP),Y
   CMP   #$61
   BNE   .cmp_true@202
   DEY
   LDA   (FP),Y
   CMP   #$47
   BNE   .cmp_true@202
   DEY
   LDA   (FP),Y
   CMP   #$AE
   BNE   .cmp_true@202
   DEY
   LDA   (FP),Y
   CMP   #$14
   BNE   .cmp_true@202
   DEY
   LDA   (FP),Y
   CMP   #$7A
   BNE   .cmp_true@202
   DEY
   LDA   (FP),Y
   CMP   #$E1
   BNE   .cmp_true@202
   LDA   #$00
   JMP   .cmp_end@203
.cmp_true@202:
   LDA   #$01
.cmp_end@203:
   LDY   #$0B
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@99
   LDX   #$00
   TXA

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$AF
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$8B
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
.if_end@99:
   CLC
   LDA   FP
   ADC   #$8D
   LDY   #$0D
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$08
   LDY   #$0F
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$10
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   LDY   #$11
   STA   (FP),Y
   DEY
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
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$13
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
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
   LDY   #$15
   LDA   (FP),Y
   LDY   #$17
   CMP   (FP),Y
   BNE   .cmp_true@205
   LDY   #$14
   LDA   (FP),Y
   LDY   #$16
   CMP   (FP),Y
   BNE   .cmp_true@205
   LDA   #$00
   JMP   .cmp_end@206
.cmp_true@205:
   LDA   #$01
.cmp_end@206:
   LDY   #$18
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@26
   JMP   .or_true@102
.lb_skip@26:
   CLC
   LDA   FP
   ADC   #$8D
   LDY   #$1A
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$08
   LDY   #$1C
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$1D
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$01
   LDY   #$1E
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$1F
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$20
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   SEC
   SBC   #$02
   INY
   STA   (FP),Y
   LDA   #$00
   SBC   #$00
   INY
   STA   (FP),Y
   LDY   #$22
   LDA   (FP),Y
   LDY   #$24
   CMP   (FP),Y
   BNE   .cmp_true@208
   LDY   #$21
   LDA   (FP),Y
   LDY   #$23
   CMP   (FP),Y
   BNE   .cmp_true@208
   LDA   #$00
   JMP   .cmp_end@209
.cmp_true@208:
   LDA   #$01
.cmp_end@209:
   LDY   #$25
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@102
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   JMP   .or_end@103
.or_true@102:
   LDA   #$01
   LDY   #$27
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@103:
   LDY   #$27
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@25
   JMP   .or_true@100
.lb_skip@25:
   CLC
   LDA   FP
   ADC   #$8D
   LDY   #$29
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$08
   LDY   #$2B
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$2C
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$02
   LDY   #$2D
   STA   (FP),Y
   DEY
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
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$2F
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   SEC
   SBC   #$03
   INY
   STA   (FP),Y
   LDA   #$00
   SBC   #$00
   INY
   STA   (FP),Y
   LDY   #$31
   LDA   (FP),Y
   LDY   #$33
   CMP   (FP),Y
   BNE   .cmp_true@211
   LDY   #$30
   LDA   (FP),Y
   LDY   #$32
   CMP   (FP),Y
   BNE   .cmp_true@211
   LDA   #$00
   JMP   .cmp_end@212
.cmp_true@211:
   LDA   #$01
.cmp_end@212:
   LDY   #$34
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@100
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   JMP   .or_end@101
.or_true@100:
   LDA   #$01
   LDY   #$36
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@101:
   LDY   #$36
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@104
   LDX   #$00
   TXA

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$AF
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$8B
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
.if_end@104:
   CLC
   LDA   FP
   ADC   #$8D
   LDY   #$38
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$0B
   LDY   #$3A
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$3B
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$3C
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$3D
   STA   (FP),Y
   LDY   #$02
   LDA   (DPTR),Y
   LDY   #$3E
   STA   (FP),Y
   LDY   #$03
   LDA   (DPTR),Y
   LDY   #$3F
   STA   (FP),Y
   LDY   #$3C
   LDA   (FP),Y
   LDY   #$40
   STA   (FP),Y
   LDY   #$3D
   LDA   (FP),Y
   LDY   #$41
   STA   (FP),Y
   LDY   #$3E
   LDA   (FP),Y
   LDY   #$42
   STA   (FP),Y
   LDY   #$3F
   LDA   (FP),Y
   LDY   #$43
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@214
   LDA   #$00
   JMP   .sx_done@215
.sx_neg@214:
   LDA   #$FF
.sx_done@215:
   LDY   #$44
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@216
   CMP   #$01
   BNE   .cmp_true@216
   LDY   #$43
   LDA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@216
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@216
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@216
   DEY
   LDA   (FP),Y
   CMP   #$04
   BNE   .cmp_true@216
   LDA   #$00
   JMP   .cmp_end@217
.cmp_true@216:
   LDA   #$01
.cmp_end@217:
   LDY   #$48
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@105
   LDX   #$00
   TXA

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$AF
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$8B
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
.if_end@105:
   CLC
   LDA   FP
   ADC   #$8D
   LDY   #$4A
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$0F
   LDY   #$4C
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$4D
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$4E
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$4F
   STA   (FP),Y
   CMP   #$27
   BNE   .cmp_true@219
   DEY
   LDA   (FP),Y
   CMP   #$10
   BNE   .cmp_true@219
   LDA   #$00
   JMP   .cmp_end@220
.cmp_true@219:
   LDA   #$01
.cmp_end@220:
   LDY   #$50
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@106
   LDX   #$00
   TXA

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$AF
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$8B
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
.if_end@106:
   CLC
   LDA   FP
   ADC   #$9E
   LDY   #$52
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
   LDY   #$54
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$55
   STA   (FP),Y
   SEC
   LDA   #$00
   SBC   #$01
   INY
   STA   (FP),Y
   LDA   #$00
   SBC   #$00
   INY
   STA   (FP),Y
   LDY   #$55
   LDA   (FP),Y
   LDY   #$57
   CMP   (FP),Y
   BNE   .cmp_true@222
   LDY   #$54
   LDA   (FP),Y
   LDY   #$56
   CMP   (FP),Y
   BNE   .cmp_true@222
   LDA   #$00
   JMP   .cmp_end@223
.cmp_true@222:
   LDA   #$01
.cmp_end@223:
   LDY   #$58
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@107
   LDX   #$00
   TXA

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$AF
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$8B
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
.if_end@107:
   CLC
   LDA   FP
   ADC   #$9E
   LDY   #$5A
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$02
   LDY   #$5C
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$5D
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   LDY   #$5E
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$5F
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$60
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CMP   #$7F
   BNE   .cmp_true@225
   LDA   #$00
   JMP   .cmp_end@226
.cmp_true@225:
   LDA   #$01
.cmp_end@226:
   LDY   #$63
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@24
   JMP   .or_true@110
.lb_skip@24:
   CLC
   LDA   FP
   ADC   #$9E
   LDY   #$65
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$02
   LDY   #$67
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$68
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$01
   LDY   #$69
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$6A
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$6B
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CMP   #$7E
   BNE   .cmp_true@227
   LDA   #$00
   JMP   .cmp_end@228
.cmp_true@227:
   LDA   #$01
.cmp_end@228:
   LDY   #$6E
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@110
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   JMP   .or_end@111
.or_true@110:
   LDA   #$01
   LDY   #$70
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@111:
   LDY   #$70
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@23
   JMP   .or_true@108
.lb_skip@23:
   CLC
   LDA   FP
   ADC   #$9E
   LDY   #$72
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$02
   LDY   #$74
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$75
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$02
   LDY   #$76
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$77
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$78
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CMP   #$7D
   BNE   .cmp_true@229
   LDA   #$00
   JMP   .cmp_end@230
.cmp_true@229:
   LDA   #$01
.cmp_end@230:
   LDY   #$7B
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@108
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   JMP   .or_end@109
.or_true@108:
   LDA   #$01
   LDY   #$7D
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@109:
   LDY   #$7D
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@112
   LDX   #$00
   TXA

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$AF
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$8B
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
.if_end@112:
   CLC
   LDA   FP
   ADC   #$A8
   LDY   #$7F
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
   LDY   #$81
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$82
   STA   (FP),Y
   LDY   #$02
   LDA   (DPTR),Y
   LDY   #$83
   STA   (FP),Y
   LDY   #$03
   LDA   (DPTR),Y
   LDY   #$84
   STA   (FP),Y
   LDY   #$04
   LDA   (DPTR),Y
   LDY   #$85
   STA   (FP),Y
   LDY   #$05
   LDA   (DPTR),Y
   LDY   #$86
   STA   (FP),Y
   LDY   #$06
   LDA   (DPTR),Y
   LDY   #$87
   STA   (FP),Y
   LDY   #$07
   LDA   (DPTR),Y
   LDY   #$88
   STA   (FP),Y
   CMP   #$40
   BNE   .cmp_true@231
   DEY
   LDA   (FP),Y
   CMP   #$14
   BNE   .cmp_true@231
   DEY
   LDA   (FP),Y
   CMP   #$80
   BNE   .cmp_true@231
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@231
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@231
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@231
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@231
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@231
   LDA   #$00
   JMP   .cmp_end@232
.cmp_true@231:
   LDA   #$01
.cmp_end@232:
   LDY   #$89
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@113
   LDX   #$00
   TXA

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$AF
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
.if_end@113:
   LDX   #$00
   LDA   #$01

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$AF
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$8B
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

str@0:
   DC.B  $68, $69, $00

str@1:
   DC.B  $73, $74, $72, $69, $6E, $67, $21, $00

str@2:
   DC.B  $6C, $6D, $6E, $6F, $00

str@3:
   DC.B  $72, $73, $00

str@4:
   DC.B  $68, $69, $00
