SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24

main:
   SUBROUTINE

   ; prologue: 0 arg bytes, 196 local bytes
   SEC
   LDA   SSP
   SBC   #$C6
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$C5
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
   SBC   long_idx
   LDY   #$01
   STA   (FP),Y
   LDA   #$00
   SBC   long_idx+1
   INY
   STA   (FP),Y
   LDA   #$00
   SBC   long_idx+2
   INY
   STA   (FP),Y
   LDA   #$00
   SBC   long_idx+3
   INY
   STA   (FP),Y
   LDY   #$01
   LDA   (FP),Y
   STA   long_idx
   INY
   LDA   (FP),Y
   STA   long_idx+1
   INY
   LDA   (FP),Y
   STA   long_idx+2
   INY
   LDA   (FP),Y
   STA   long_idx+3
   LDA   #<unsigned_arr
   INY
   STA   (FP),Y
   LDA   #>unsigned_arr
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
   ADC   #$04
.mul_skip@1:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@0
   LDY   #$08
   STA   (FP),Y
   LDA   HARGS
   DEY
   STA   (FP),Y
   LDY   #$05
   LDA   (FP),Y
   CLC
   LDY   #$07
   ADC   (FP),Y
   LDY   #$09
   STA   (FP),Y
   LDY   #$06
   LDA   (FP),Y
   LDY   #$08
   ADC   (FP),Y
   LDY   #$0A
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$0B
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$0C
   STA   (FP),Y
   LDY   #$02
   LDA   (DPTR),Y
   LDY   #$0D
   STA   (FP),Y
   LDY   #$03
   LDA   (DPTR),Y
   LDY   #$0E
   STA   (FP),Y
   LDA   #$02
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   ORA   #$00
   BPL   .sx_done@3
   LDA   #$FF
.sx_done@3:
   LDY   #$11
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$0B
   LDA   (FP),Y
   CLC
   LDY   #$0F
   ADC   (FP),Y
   LDY   #$13
   STA   (FP),Y
   LDY   #$0C
   LDA   (FP),Y
   LDY   #$10
   ADC   (FP),Y
   LDY   #$14
   STA   (FP),Y
   LDY   #$0D
   LDA   (FP),Y
   LDY   #$11
   ADC   (FP),Y
   LDY   #$15
   STA   (FP),Y
   LDY   #$0E
   LDA   (FP),Y
   LDY   #$12
   ADC   (FP),Y
   LDY   #$16
   STA   (FP),Y
   LDY   #$09
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$13
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$14
   LDA   (FP),Y
   LDY   #$01
   STA   (DPTR),Y
   LDY   #$15
   LDA   (FP),Y
   LDY   #$02
   STA   (DPTR),Y
   LDY   #$16
   LDA   (FP),Y
   LDY   #$03
   STA   (DPTR),Y
   LDA   unsigned_arr+4
   LDY   #$17
   STA   (FP),Y
   LDA   unsigned_arr+5
   INY
   STA   (FP),Y
   LDA   unsigned_arr+6
   INY
   STA   (FP),Y
   LDA   unsigned_arr+7
   INY
   STA   (FP),Y
   LDY   #$17
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   INY
   ORA   (FP),Y
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
   ADC   #$C6
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$C5
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
   LDA   #<unsigned_arr
   LDY   #$1B
   STA   (FP),Y
   LDA   #>unsigned_arr
   INY
   STA   (FP),Y
   LDA   idx
   INY
   STA   (FP),Y
   STA   HARGS
   LDA   #$04
   STA   HARGS+1
   JSR   mul8
   LDA   HARGS+2
   LDY   #$1E
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$1B
   LDA   (FP),Y
   CLC
   LDY   #$1F
   ADC   (FP),Y
   LDY   #$21
   STA   (FP),Y
   LDY   #$1C
   LDA   (FP),Y
   LDY   #$20
   ADC   (FP),Y
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
   LDY   #$23
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
   LDY   #$27
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
   LDY   #$27
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
   LDA   #$24
   STA   HARGS+14
   LDA   #$40
   STA   HARGS+15
   JSR   dsub
   LDA   HARGS+16
   LDY   #$2F
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
   LDY   #$2F
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
   JSR   d2ul
   LDA   HARGS+8
   LDY   #$37
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
   LDY   #$21
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$37
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$38
   LDA   (FP),Y
   LDY   #$01
   STA   (DPTR),Y
   LDY   #$39
   LDA   (FP),Y
   LDY   #$02
   STA   (DPTR),Y
   LDY   #$3A
   LDA   (FP),Y
   LDY   #$03
   STA   (DPTR),Y
   LDA   idx
   LDY   #$3B
   STA   (FP),Y
   STA   HARGS
   LDA   #$04
   STA   HARGS+1
   JSR   mul8
   LDA   HARGS+2
   LDY   #$3C
   STA   (FP),Y
   TAX
   LDA   unsigned_arr,X
   INY
   STA   (FP),Y
   LDA   unsigned_arr+1,X
   INY
   STA   (FP),Y
   LDA   unsigned_arr+2,X
   INY
   STA   (FP),Y
   LDA   unsigned_arr+3,X
   INY
   STA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@4
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@4
   DEY
   LDA   (FP),Y
   CMP   #$FF
   BNE   .cmp_true@4
   DEY
   LDA   (FP),Y
   CMP   #$F3
   BNE   .cmp_true@4
   LDA   #$00
   JMP   .cmp_end@5
.cmp_true@4:
   LDA   #$01
.cmp_end@5:
   LDY   #$41
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
   ADC   #$C6
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$C5
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
   LDA   #<unsigned_arr
   LDY   #$43
   STA   (FP),Y
   LDA   #>unsigned_arr
   INY
   STA   (FP),Y
   LDA   #$04
   STA   HARGS
   LDA   #$00
   LDX   #$08
   LSR   HARGS
.mul_loop@7:
   BCC   .mul_skip@8
   CLC
   ADC   #$04
.mul_skip@8:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@7
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
   DEY
   LDA   (FP),Y
   LDY   #$49
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$4A
   STA   (FP),Y
   LDA   long_idx
   INY
   STA   (FP),Y
   LDA   long_idx+1
   INY
   STA   (FP),Y
   DEY
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
   LDY   #$4D
   STA   (FP),Y
   LDA   HARGS+5
   INY
   STA   (FP),Y
   LDY   #$49
   LDA   (FP),Y
   CLC
   LDY   #$4D
   ADC   (FP),Y
   LDY   #$4F
   STA   (FP),Y
   LDY   #$4A
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
   LDA   #$0A
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   ORA   #$00
   BPL   .sx_done@10
   LDA   #$FF
.sx_done@10:
   LDY   #$57
   STA   (FP),Y
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
   JSR   udivmod32
   LDA   HARGS+8
   LDY   #$59
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
   LDY   #$4F
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$59
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$5A
   LDA   (FP),Y
   LDY   #$01
   STA   (DPTR),Y
   LDY   #$5B
   LDA   (FP),Y
   LDY   #$02
   STA   (DPTR),Y
   LDY   #$5C
   LDA   (FP),Y
   LDY   #$03
   STA   (DPTR),Y
   LDA   unsigned_arr+12
   LDY   #$5D
   STA   (FP),Y
   LDA   unsigned_arr+13
   INY
   STA   (FP),Y
   LDA   unsigned_arr+14
   INY
   STA   (FP),Y
   LDA   unsigned_arr+15
   INY
   STA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@11
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@11
   DEY
   LDA   (FP),Y
   CMP   #$19
   BNE   .cmp_true@11
   DEY
   LDA   (FP),Y
   CMP   #$99
   BNE   .cmp_true@11
   LDA   #$00
   JMP   .cmp_end@12
.cmp_true@11:
   LDA   #$01
.cmp_end@12:
   LDY   #$61
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
   ADC   #$C6
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$C5
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
   LDA   #$02
   LDY   #$63
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   ORA   #$00
   BPL   .sx_done@15
   LDA   #$FF
.sx_done@15:
   LDY   #$65
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   long_idx
   STA   HARGS
   LDA   long_idx+1
   STA   HARGS+1
   LDA   long_idx+2
   STA   HARGS+2
   LDA   long_idx+3
   STA   HARGS+3
   LDY   #$63
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
   JSR   mul32
   LDA   HARGS+8
   LDY   #$67
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
   LDY   #$67
   LDA   (FP),Y
   STA   long_idx
   INY
   LDA   (FP),Y
   STA   long_idx+1
   INY
   LDA   (FP),Y
   STA   long_idx+2
   INY
   LDA   (FP),Y
   STA   long_idx+3
   LDY   #$67
   LDA   (FP),Y
   LDY   #$6B
   STA   (FP),Y
   LDY   #$68
   LDA   (FP),Y
   LDY   #$6C
   STA   (FP),Y
   DEY
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
   LDY   #$6D
   STA   (FP),Y
   LDA   HARGS+5
   INY
   STA   (FP),Y
   LDY   #$49
   LDA   (FP),Y
   CLC
   LDY   #$6D
   ADC   (FP),Y
   LDY   #$6F
   STA   (FP),Y
   LDY   #$4A
   LDA   (FP),Y
   LDY   #$6E
   ADC   (FP),Y
   LDY   #$70
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$71
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$72
   STA   (FP),Y
   LDY   #$02
   LDA   (DPTR),Y
   LDY   #$73
   STA   (FP),Y
   LDY   #$03
   LDA   (DPTR),Y
   LDY   #$74
   STA   (FP),Y
   LDA   unsigned_arr
   INY
   STA   (FP),Y
   LDA   unsigned_arr+1
   INY
   STA   (FP),Y
   LDA   unsigned_arr+2
   INY
   STA   (FP),Y
   LDA   unsigned_arr+3
   INY
   STA   (FP),Y
   LDY   #$71
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
   JSR   mul32
   LDA   HARGS+8
   LDY   #$79
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
   LDY   #$6F
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
   LDY   #$7B
   LDA   (FP),Y
   LDY   #$02
   STA   (DPTR),Y
   LDY   #$7C
   LDA   (FP),Y
   LDY   #$03
   STA   (DPTR),Y
   LDA   unsigned_arr+8
   LDY   #$7D
   STA   (FP),Y
   LDA   unsigned_arr+9
   INY
   STA   (FP),Y
   LDA   unsigned_arr+10
   INY
   STA   (FP),Y
   LDA   unsigned_arr+11
   INY
   STA   (FP),Y
   LDA   #$0D
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   ORA   #$00
   BPL   .sx_done@17
   LDA   #$FF
.sx_done@17:
   LDY   #$83
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$80
   LDA   (FP),Y
   LDY   #$84
   CMP   (FP),Y
   BNE   .cmp_true@18
   LDY   #$7F
   LDA   (FP),Y
   LDY   #$83
   CMP   (FP),Y
   BNE   .cmp_true@18
   LDY   #$7E
   LDA   (FP),Y
   LDY   #$82
   CMP   (FP),Y
   BNE   .cmp_true@18
   LDY   #$7D
   LDA   (FP),Y
   LDY   #$81
   CMP   (FP),Y
   BNE   .cmp_true@18
   LDA   #$00
   JMP   .cmp_end@19
.cmp_true@18:
   LDA   #$01
.cmp_end@19:
   LDY   #$85
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
   ADC   #$C6
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$C5
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
   LDA   #<unsigned_arr
   LDY   #$87
   STA   (FP),Y
   LDA   #>unsigned_arr
   INY
   STA   (FP),Y
   LDA   idx
   INY
   STA   (FP),Y
   LDA   idx+1
   INY
   STA   (FP),Y
   LDA   idx+1
   BMI   .sx_neg@21
   LDA   #$00
   JMP   .sx_done@22
.sx_neg@21:
   LDA   #$FF
.sx_done@22:
   LDY   #$8B
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$89
   LDA   (FP),Y
   CLC
   ADC   long_idx
   LDY   #$8D
   STA   (FP),Y
   LDY   #$8A
   LDA   (FP),Y
   ADC   long_idx+1
   LDY   #$8E
   STA   (FP),Y
   LDY   #$8B
   LDA   (FP),Y
   ADC   long_idx+2
   LDY   #$8F
   STA   (FP),Y
   LDY   #$8C
   LDA   (FP),Y
   ADC   long_idx+3
   LDY   #$90
   STA   (FP),Y
   LDY   #$8D
   LDA   (FP),Y
   LDY   #$91
   STA   (FP),Y
   LDY   #$8E
   LDA   (FP),Y
   LDY   #$92
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$93
   STA   (FP),Y
   STA   HARGS
   LDA   #$04
   STA   HARGS+1
   JSR   mul8
   LDA   HARGS+2
   LDY   #$94
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$87
   LDA   (FP),Y
   CLC
   LDY   #$95
   ADC   (FP),Y
   LDY   #$97
   STA   (FP),Y
   LDY   #$88
   LDA   (FP),Y
   LDY   #$96
   ADC   (FP),Y
   LDY   #$98
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$99
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$9A
   STA   (FP),Y
   LDY   #$02
   LDA   (DPTR),Y
   LDY   #$9B
   STA   (FP),Y
   LDY   #$03
   LDA   (DPTR),Y
   LDY   #$9C
   STA   (FP),Y
   LDA   #$0A
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   ORA   #$00
   BPL   .sx_done@24
   LDA   #$FF
.sx_done@24:
   LDY   #$9F
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$99
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
   JSR   udivmod32
   LDA   HARGS+12
   LDY   #$A1
   STA   (FP),Y
   LDA   HARGS+13
   INY
   STA   (FP),Y
   LDA   HARGS+14
   INY
   STA   (FP),Y
   LDA   HARGS+15
   INY
   STA   (FP),Y
   LDY   #$97
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$A1
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$A2
   LDA   (FP),Y
   LDY   #$01
   STA   (DPTR),Y
   LDY   #$A3
   LDA   (FP),Y
   LDY   #$02
   STA   (DPTR),Y
   LDY   #$A4
   LDA   (FP),Y
   LDY   #$03
   STA   (DPTR),Y
   LDA   #$05
   LDY   #$A5
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   ORA   #$00
   BPL   .sx_done@26
   LDA   #$FF
.sx_done@26:
   LDY   #$A7
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$A4
   LDA   (FP),Y
   LDY   #$A8
   CMP   (FP),Y
   BNE   .cmp_true@27
   LDY   #$A3
   LDA   (FP),Y
   LDY   #$A7
   CMP   (FP),Y
   BNE   .cmp_true@27
   LDY   #$A2
   LDA   (FP),Y
   LDY   #$A6
   CMP   (FP),Y
   BNE   .cmp_true@27
   LDY   #$A1
   LDA   (FP),Y
   LDY   #$A5
   CMP   (FP),Y
   BNE   .cmp_true@27
   LDA   #$00
   JMP   .cmp_end@28
.cmp_true@27:
   LDA   #$01
.cmp_end@28:
   LDY   #$A9
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
   ADC   #$C6
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$C5
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
   LDA   unsigned_arr
   LDY   #$AB
   STA   (FP),Y
   LDA   unsigned_arr+1
   INY
   STA   (FP),Y
   LDA   unsigned_arr+2
   INY
   STA   (FP),Y
   LDA   unsigned_arr+3
   INY
   STA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@30
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@30
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@30
   DEY
   LDA   (FP),Y
   CMP   #$05
   BNE   .cmp_true@30
   LDA   #$00
   JMP   .cmp_end@31
.cmp_true@30:
   LDA   #$01
.cmp_end@31:
   LDY   #$AF
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
   ADC   #$C6
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$C5
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
   LDA   unsigned_arr+4
   LDY   #$B1
   STA   (FP),Y
   LDA   unsigned_arr+5
   INY
   STA   (FP),Y
   LDA   unsigned_arr+6
   INY
   STA   (FP),Y
   LDA   unsigned_arr+7
   INY
   STA   (FP),Y
   LDY   #$B1
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   INY
   ORA   (FP),Y
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
   ADC   #$C6
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$C5
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
   LDA   unsigned_arr+8
   LDY   #$B5
   STA   (FP),Y
   LDA   unsigned_arr+9
   INY
   STA   (FP),Y
   LDA   unsigned_arr+10
   INY
   STA   (FP),Y
   LDA   unsigned_arr+11
   INY
   STA   (FP),Y
   LDA   #$0D
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   ORA   #$00
   BPL   .sx_done@34
   LDA   #$FF
.sx_done@34:
   LDY   #$BB
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$B8
   LDA   (FP),Y
   LDY   #$BC
   CMP   (FP),Y
   BNE   .cmp_true@35
   LDY   #$B7
   LDA   (FP),Y
   LDY   #$BB
   CMP   (FP),Y
   BNE   .cmp_true@35
   LDY   #$B6
   LDA   (FP),Y
   LDY   #$BA
   CMP   (FP),Y
   BNE   .cmp_true@35
   LDY   #$B5
   LDA   (FP),Y
   LDY   #$B9
   CMP   (FP),Y
   BNE   .cmp_true@35
   LDA   #$00
   JMP   .cmp_end@36
.cmp_true@35:
   LDA   #$01
.cmp_end@36:
   LDY   #$BD
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
   ADC   #$C6
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$C5
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
   LDA   unsigned_arr+12
   LDY   #$BF
   STA   (FP),Y
   LDA   unsigned_arr+13
   INY
   STA   (FP),Y
   LDA   unsigned_arr+14
   INY
   STA   (FP),Y
   LDA   unsigned_arr+15
   INY
   STA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@38
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@38
   DEY
   LDA   (FP),Y
   CMP   #$19
   BNE   .cmp_true@38
   DEY
   LDA   (FP),Y
   CMP   #$99
   BNE   .cmp_true@38
   LDA   #$00
   JMP   .cmp_end@39
.cmp_true@38:
   LDA   #$01
.cmp_end@39:
   LDY   #$C3
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
   ADC   #$C6
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
   ADC   #$C6
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$C5
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

unsigned_arr:
   DC.L  $0000FFFF
   DC.L  $0000FFFE
   DC.L  $0000FFFD
   DC.L  $0000FFFC

idx:
   DC.W  $0002

long_idx:
   DC.L  $00000001
