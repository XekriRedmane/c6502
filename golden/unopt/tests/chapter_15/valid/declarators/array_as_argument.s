SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24

array_param:
   SUBROUTINE

   ; prologue: 2 arg bytes, 4 local bytes
   SEC
   LDA   SSP
   SBC   #$06
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$05
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   LDA   #$04
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
   LDY   #$02
   STA   (FP),Y
   LDA   HARGS
   DEY
   STA   (FP),Y
   LDY   #$07
   LDA   (FP),Y
   CLC
   LDY   #$01
   ADC   (FP),Y
   LDY   #$03
   STA   (FP),Y
   LDY   #$08
   LDA   (FP),Y
   LDY   #$02
   ADC   (FP),Y
   LDY   #$04
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
   LDX   #$00

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$08
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$05
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

nested_array_param:
   SUBROUTINE

   ; prologue: 2 arg bytes, 8 local bytes
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

   LDA   #$01
   STA   HARGS
   LDA   #$00
   LDX   #$08
   LSR   HARGS
.mul_loop@2:
   BCC   .mul_skip@3
   CLC
   ADC   #$06
.mul_skip@3:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@2
   LDY   #$02
   STA   (FP),Y
   LDA   HARGS
   DEY
   STA   (FP),Y
   LDY   #$0B
   LDA   (FP),Y
   CLC
   LDY   #$01
   ADC   (FP),Y
   LDY   #$03
   STA   (FP),Y
   LDY   #$0C
   LDA   (FP),Y
   LDY   #$02
   ADC   (FP),Y
   LDY   #$04
   STA   (FP),Y
   LDA   #$01
   STA   HARGS
   LDA   #$00
   LDX   #$08
   LSR   HARGS
.mul_loop@4:
   BCC   .mul_skip@5
   CLC
   ADC   #$02
.mul_skip@5:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@4
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
   LDX   #$00

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$0C
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

main:
   SUBROUTINE

   ; prologue: 0 arg bytes, 202 local bytes
   SEC
   LDA   SSP
   SBC   #$CC
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$CB
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
   LDY   #$11
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
   LDA   #$08
   LDY   #$00
   STA   (DPTR),Y
   LDA   #$00
   INY
   STA   (DPTR),Y
   LDY   #$11
   LDA   (FP),Y
   CLC
   ADC   #$02
   LDY   #$13
   STA   (FP),Y
   DEY
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
   LDA   #$07
   LDY   #$00
   STA   (DPTR),Y
   LDA   #$00
   INY
   STA   (DPTR),Y
   LDY   #$11
   LDA   (FP),Y
   CLC
   ADC   #$04
   LDY   #$15
   STA   (FP),Y
   LDY   #$12
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
   LDA   #$06
   LDY   #$00
   STA   (DPTR),Y
   LDA   #$00
   INY
   STA   (DPTR),Y
   LDY   #$11
   LDA   (FP),Y
   CLC
   ADC   #$06
   LDY   #$17
   STA   (FP),Y
   LDY   #$12
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
   LDA   #$05
   LDY   #$00
   STA   (DPTR),Y
   LDA   #$00
   INY
   STA   (DPTR),Y
   LDY   #$11
   LDA   (FP),Y
   CLC
   ADC   #$08
   LDY   #$19
   STA   (FP),Y
   LDY   #$12
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
   LDY   #$11
   LDA   (FP),Y
   CLC
   ADC   #$0A
   LDY   #$1B
   STA   (FP),Y
   LDY   #$12
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
   LDA   #$03
   LDY   #$00
   STA   (DPTR),Y
   LDA   #$00
   INY
   STA   (DPTR),Y
   LDY   #$11
   LDA   (FP),Y
   CLC
   ADC   #$0C
   LDY   #$1D
   STA   (FP),Y
   LDY   #$12
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
   LDA   #$02
   LDY   #$00
   STA   (DPTR),Y
   LDA   #$00
   INY
   STA   (DPTR),Y
   LDY   #$11
   LDA   (FP),Y
   CLC
   ADC   #$0E
   LDY   #$1F
   STA   (FP),Y
   LDY   #$12
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
   LDA   #$01
   LDY   #$00
   STA   (DPTR),Y
   LDA   #$00
   INY
   STA   (DPTR),Y
   CLC
   LDA   FP
   ADC   #$01
   LDY   #$21
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$02
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   DEY
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$22
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   JSR   array_param
   LDY   #$23
   STA   (FP),Y
   TXA
   INY
   STA   (FP),Y
   CLC
   LDA   FP
   ADC   #$01
   LDY   #$27
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
.mul_loop@6:
   BCC   .mul_skip@7
   CLC
   ADC   #$02
.mul_skip@7:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@6
   LDY   #$26
   STA   (FP),Y
   LDA   HARGS
   DEY
   STA   (FP),Y
   LDY   #$27
   LDA   (FP),Y
   CLC
   LDY   #$25
   ADC   (FP),Y
   LDY   #$29
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$26
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
   ADC   #$CC
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$CB
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
   LDY   #$2D
   STA   (FP),Y
   INY
   STA   (FP),Y
.loop@0_start:
   LDY   #$2D
   LDA   (FP),Y
   SEC
   SBC   #$08
   INY
   LDA   (FP),Y
   SBC   #$00
   BVC   .cmp_novf@8
   EOR   #$80
.cmp_novf@8:
   ASL
   LDA   #$00
   ROL
   LDY   #$2F
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@4
   JMP   .loop@0_break
.lb_skip@4:
   LDY   #$2E
   LDA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@9
   DEY
   LDA   (FP),Y
   CMP   #$04
   BNE   .cmp_true@9
   LDA   #$00
   JMP   .cmp_end@10
.cmp_true@9:
   LDA   #$01
.cmp_end@10:
   LDY   #$31
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@3
   JMP   .and_false@1
.lb_skip@3:
   CLC
   LDA   FP
   ADC   #$01
   LDY   #$37
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   LDY   #$2D
   LDA   (FP),Y
   LDY   #$33
   STA   (FP),Y
   STA   HARGS
   LDA   #$02
   STA   HARGS+1
   JSR   mul8
   LDA   HARGS+2
   LDY   #$34
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   LDA   (FP),Y
   CLC
   LDY   #$35
   ADC   (FP),Y
   LDY   #$39
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$36
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
   LDA   #$08
   SEC
   LDY   #$2D
   SBC   (FP),Y
   LDY   #$3D
   STA   (FP),Y
   LDA   #$00
   LDY   #$2E
   SBC   (FP),Y
   LDY   #$3E
   STA   (FP),Y
   LDY   #$3C
   LDA   (FP),Y
   LDY   #$3E
   CMP   (FP),Y
   BNE   .cmp_true@12
   LDY   #$3B
   LDA   (FP),Y
   LDY   #$3D
   CMP   (FP),Y
   BNE   .cmp_true@12
   LDA   #$00
   JMP   .cmp_end@13
.cmp_true@12:
   LDA   #$01
.cmp_end@13:
   LDY   #$3F
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@1
   LDA   #$01
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   JMP   .and_end@2
.and_false@1:
   LDA   #$00
   LDY   #$41
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@2:
   LDY   #$41
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
   ADC   #$CC
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$CB
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
   LDY   #$2D
   LDA   (FP),Y
   CLC
   ADC   #$01
   LDY   #$43
   STA   (FP),Y
   LDY   #$2E
   LDA   (FP),Y
   ADC   #$00
   LDY   #$44
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$2D
   STA   (FP),Y
   LDY   #$44
   LDA   (FP),Y
   LDY   #$2E
   STA   (FP),Y
   JMP   .loop@0_start
.loop@0_break:
   SEC
   LDA   #$00
   SBC   #$01
   LDY   #$45
   STA   (FP),Y
   LDA   #$00
   SBC   #$00
   INY
   STA   (FP),Y
   CLC
   LDA   FP
   ADC   #$47
   LDY   #$5F
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
   LDY   #$45
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$46
   LDA   (FP),Y
   LDY   #$01
   STA   (DPTR),Y
   SEC
   LDA   #$00
   SBC   #$01
   LDY   #$61
   STA   (FP),Y
   LDA   #$00
   SBC   #$00
   INY
   STA   (FP),Y
   LDY   #$5F
   LDA   (FP),Y
   CLC
   ADC   #$02
   LDY   #$63
   STA   (FP),Y
   LDY   #$60
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
   LDY   #$61
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$62
   LDA   (FP),Y
   LDY   #$01
   STA   (DPTR),Y
   SEC
   LDA   #$00
   SBC   #$01
   LDY   #$65
   STA   (FP),Y
   LDA   #$00
   SBC   #$00
   INY
   STA   (FP),Y
   LDY   #$5F
   LDA   (FP),Y
   CLC
   ADC   #$04
   LDY   #$67
   STA   (FP),Y
   LDY   #$60
   LDA   (FP),Y
   ADC   #$00
   LDY   #$68
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$65
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$66
   LDA   (FP),Y
   LDY   #$01
   STA   (DPTR),Y
   SEC
   LDA   #$00
   SBC   #$02
   LDY   #$69
   STA   (FP),Y
   LDA   #$00
   SBC   #$00
   INY
   STA   (FP),Y
   LDY   #$5F
   LDA   (FP),Y
   CLC
   ADC   #$06
   LDY   #$6B
   STA   (FP),Y
   LDY   #$60
   LDA   (FP),Y
   ADC   #$00
   LDY   #$6C
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$69
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$6A
   LDA   (FP),Y
   LDY   #$01
   STA   (DPTR),Y
   SEC
   LDA   #$00
   SBC   #$02
   LDY   #$6D
   STA   (FP),Y
   LDA   #$00
   SBC   #$00
   INY
   STA   (FP),Y
   LDY   #$5F
   LDA   (FP),Y
   CLC
   ADC   #$08
   LDY   #$6F
   STA   (FP),Y
   LDY   #$60
   LDA   (FP),Y
   ADC   #$00
   LDY   #$70
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$6D
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$6E
   LDA   (FP),Y
   LDY   #$01
   STA   (DPTR),Y
   SEC
   LDA   #$00
   SBC   #$02
   LDY   #$71
   STA   (FP),Y
   LDA   #$00
   SBC   #$00
   INY
   STA   (FP),Y
   LDY   #$5F
   LDA   (FP),Y
   CLC
   ADC   #$0A
   LDY   #$73
   STA   (FP),Y
   LDY   #$60
   LDA   (FP),Y
   ADC   #$00
   LDY   #$74
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$71
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$72
   LDA   (FP),Y
   LDY   #$01
   STA   (DPTR),Y
   SEC
   LDA   #$00
   SBC   #$03
   LDY   #$75
   STA   (FP),Y
   LDA   #$00
   SBC   #$00
   INY
   STA   (FP),Y
   LDY   #$5F
   LDA   (FP),Y
   CLC
   ADC   #$0C
   LDY   #$77
   STA   (FP),Y
   LDY   #$60
   LDA   (FP),Y
   ADC   #$00
   LDY   #$78
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$75
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$76
   LDA   (FP),Y
   LDY   #$01
   STA   (DPTR),Y
   SEC
   LDA   #$00
   SBC   #$03
   LDY   #$79
   STA   (FP),Y
   LDA   #$00
   SBC   #$00
   INY
   STA   (FP),Y
   LDY   #$5F
   LDA   (FP),Y
   CLC
   ADC   #$0E
   LDY   #$7B
   STA   (FP),Y
   LDY   #$60
   LDA   (FP),Y
   ADC   #$00
   LDY   #$7C
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$79
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$7A
   LDA   (FP),Y
   LDY   #$01
   STA   (DPTR),Y
   SEC
   LDA   #$00
   SBC   #$03
   LDY   #$7D
   STA   (FP),Y
   LDA   #$00
   SBC   #$00
   INY
   STA   (FP),Y
   LDY   #$5F
   LDA   (FP),Y
   CLC
   ADC   #$10
   LDY   #$7F
   STA   (FP),Y
   LDY   #$60
   LDA   (FP),Y
   ADC   #$00
   LDY   #$80
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$7D
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$7E
   LDA   (FP),Y
   LDY   #$01
   STA   (DPTR),Y
   SEC
   LDA   #$00
   SBC   #$04
   LDY   #$81
   STA   (FP),Y
   LDA   #$00
   SBC   #$00
   INY
   STA   (FP),Y
   LDY   #$5F
   LDA   (FP),Y
   CLC
   ADC   #$12
   LDY   #$83
   STA   (FP),Y
   LDY   #$60
   LDA   (FP),Y
   ADC   #$00
   LDY   #$84
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$81
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$82
   LDA   (FP),Y
   LDY   #$01
   STA   (DPTR),Y
   SEC
   LDA   #$00
   SBC   #$04
   LDY   #$85
   STA   (FP),Y
   LDA   #$00
   SBC   #$00
   INY
   STA   (FP),Y
   LDY   #$5F
   LDA   (FP),Y
   CLC
   ADC   #$14
   LDY   #$87
   STA   (FP),Y
   LDY   #$60
   LDA   (FP),Y
   ADC   #$00
   LDY   #$88
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$85
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$86
   LDA   (FP),Y
   LDY   #$01
   STA   (DPTR),Y
   SEC
   LDA   #$00
   SBC   #$04
   LDY   #$89
   STA   (FP),Y
   LDA   #$00
   SBC   #$00
   INY
   STA   (FP),Y
   LDY   #$5F
   LDA   (FP),Y
   CLC
   ADC   #$16
   LDY   #$8B
   STA   (FP),Y
   LDY   #$60
   LDA   (FP),Y
   ADC   #$00
   LDY   #$8C
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$89
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$8A
   LDA   (FP),Y
   LDY   #$01
   STA   (DPTR),Y
   CLC
   LDA   FP
   ADC   #$47
   LDY   #$8D
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$02
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   DEY
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$8E
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   JSR   nested_array_param
   LDY   #$8F
   STA   (FP),Y
   TXA
   INY
   STA   (FP),Y
   CLC
   LDA   FP
   ADC   #$47
   LDY   #$93
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
.mul_loop@15:
   BCC   .mul_skip@16
   CLC
   ADC   #$06
.mul_skip@16:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@15
   LDY   #$92
   STA   (FP),Y
   LDA   HARGS
   DEY
   STA   (FP),Y
   LDY   #$93
   LDA   (FP),Y
   CLC
   LDY   #$91
   ADC   (FP),Y
   LDY   #$95
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$92
   ADC   (FP),Y
   LDY   #$96
   STA   (FP),Y
   LDA   #$01
   STA   HARGS
   LDA   #$00
   LDX   #$08
   LSR   HARGS
.mul_loop@17:
   BCC   .mul_skip@18
   CLC
   ADC   #$02
.mul_skip@18:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@17
   LDY   #$98
   STA   (FP),Y
   LDA   HARGS
   DEY
   STA   (FP),Y
   LDY   #$95
   LDA   (FP),Y
   CLC
   LDY   #$97
   ADC   (FP),Y
   LDY   #$99
   STA   (FP),Y
   LDY   #$96
   LDA   (FP),Y
   LDY   #$98
   ADC   (FP),Y
   LDY   #$9A
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$9B
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$9C
   STA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@19
   DEY
   LDA   (FP),Y
   CMP   #$01
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
   BEQ   .if_end@4
   LDX   #$00
   LDA   #$03

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$CC
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$CB
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
   LDY   #$9F
   STA   (FP),Y
   INY
   STA   (FP),Y
.loop@1_start:
   LDY   #$9F
   LDA   (FP),Y
   SEC
   SBC   #$04
   INY
   LDA   (FP),Y
   SBC   #$00
   BVC   .cmp_novf@22
   EOR   #$80
.cmp_novf@22:
   ASL
   LDA   #$00
   ROL
   LDY   #$A1
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
   SEC
   LDA   #$00
   SBC   #$01
   LDY   #$A3
   STA   (FP),Y
   LDA   #$00
   SBC   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   SEC
   LDY   #$9F
   SBC   (FP),Y
   LDY   #$A5
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$A0
   SBC   (FP),Y
   LDY   #$A6
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$A7
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$A8
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
.loop@2_start:
   LDY   #$A9
   LDA   (FP),Y
   SEC
   SBC   #$03
   INY
   LDA   (FP),Y
   SBC   #$00
   BVC   .cmp_novf@23
   EOR   #$80
.cmp_novf@23:
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
   BNE   .lb_skip@1
   JMP   .loop@2_break
.lb_skip@1:
   LDY   #$A0
   LDA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@24
   DEY
   LDA   (FP),Y
   CMP   #$01
   BNE   .cmp_true@24
   LDA   #$00
   JMP   .cmp_end@25
.cmp_true@24:
   LDA   #$01
.cmp_end@25:
   LDY   #$AD
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@7
   LDY   #$AA
   LDA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@27
   DEY
   LDA   (FP),Y
   CMP   #$01
   BNE   .cmp_true@27
   LDA   #$00
   JMP   .cmp_end@28
.cmp_true@27:
   LDA   #$01
.cmp_end@28:
   LDY   #$AF
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
   LDY   #$B1
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@8:
   LDY   #$B1
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@0
   JMP   .and_false@5
.lb_skip@0:
   CLC
   LDA   FP
   ADC   #$47
   LDY   #$B7
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   LDY   #$9F
   LDA   (FP),Y
   LDY   #$B3
   STA   (FP),Y
   STA   HARGS
   LDA   #$06
   STA   HARGS+1
   JSR   mul8
   LDA   HARGS+2
   LDY   #$B4
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   LDA   (FP),Y
   CLC
   LDY   #$B5
   ADC   (FP),Y
   LDY   #$B9
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$B6
   ADC   (FP),Y
   LDY   #$BA
   STA   (FP),Y
   LDY   #$A9
   LDA   (FP),Y
   LDY   #$BB
   STA   (FP),Y
   STA   HARGS
   LDA   #$02
   STA   HARGS+1
   JSR   mul8
   LDA   HARGS+2
   LDY   #$BC
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$B9
   LDA   (FP),Y
   CLC
   LDY   #$BD
   ADC   (FP),Y
   LDY   #$BF
   STA   (FP),Y
   LDY   #$BA
   LDA   (FP),Y
   LDY   #$BE
   ADC   (FP),Y
   LDY   #$C0
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$C1
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$C2
   STA   (FP),Y
   LDY   #$A8
   CMP   (FP),Y
   BNE   .cmp_true@30
   LDY   #$C1
   LDA   (FP),Y
   LDY   #$A7
   CMP   (FP),Y
   BNE   .cmp_true@30
   LDA   #$00
   JMP   .cmp_end@31
.cmp_true@30:
   LDA   #$01
.cmp_end@31:
   LDY   #$C3
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
   LDY   #$C5
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@6:
   LDY   #$C5
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@9
   LDX   #$00
   LDA   #$04

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$CC
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$CB
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
   LDY   #$A9
   LDA   (FP),Y
   CLC
   ADC   #$01
   LDY   #$C7
   STA   (FP),Y
   LDY   #$AA
   LDA   (FP),Y
   ADC   #$00
   LDY   #$C8
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$A9
   STA   (FP),Y
   LDY   #$C8
   LDA   (FP),Y
   LDY   #$AA
   STA   (FP),Y
   JMP   .loop@2_start
.loop@2_break:
   LDY   #$9F
   LDA   (FP),Y
   CLC
   ADC   #$01
   LDY   #$C9
   STA   (FP),Y
   LDY   #$A0
   LDA   (FP),Y
   ADC   #$00
   LDY   #$CA
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$9F
   STA   (FP),Y
   LDY   #$CA
   LDA   (FP),Y
   LDY   #$A0
   STA   (FP),Y
   JMP   .loop@1_start
.loop@1_break:
   LDX   #$00
   TXA

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$CC
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$CB
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
