SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24

main:
   SUBROUTINE

   ; prologue: 0 arg bytes, 188 local bytes
   SEC
   LDA   SSP
   SBC   #$BE
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$BD
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   LDA   #<long_nested_arr
   LDY   #$01
   STA   (FP),Y
   LDA   #>long_nested_arr
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
   ADC   #$0C
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
   LDA   unsigned_index
   SEC
   SBC   #$08
   INY
   STA   (FP),Y
   LDA   unsigned_index+1
   SBC   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$09
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$0A
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$0B
   STA   (FP),Y
   STA   HARGS
   LDA   #$04
   STA   HARGS+1
   JSR   mul8
   LDA   HARGS+2
   LDY   #$0C
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$05
   LDA   (FP),Y
   CLC
   LDY   #$0D
   ADC   (FP),Y
   LDY   #$0F
   STA   (FP),Y
   LDY   #$06
   LDA   (FP),Y
   LDY   #$0E
   ADC   (FP),Y
   LDY   #$10
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$11
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$12
   STA   (FP),Y
   LDY   #$02
   LDA   (DPTR),Y
   LDY   #$13
   STA   (FP),Y
   LDY   #$03
   LDA   (DPTR),Y
   LDY   #$14
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
   DEY
   LDA   (FP),Y
   LDY   #$17
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$18
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@2
   LDA   #$00
   JMP   .sx_done@3
.sx_neg@2:
   LDA   #$FF
.sx_done@3:
   LDY   #$19
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$11
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
   LDY   #$17
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
   LDY   #$1B
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
   LDY   #$0F
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$1B
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$1C
   LDA   (FP),Y
   LDY   #$01
   STA   (DPTR),Y
   LDY   #$1D
   LDA   (FP),Y
   LDY   #$02
   STA   (DPTR),Y
   LDY   #$1E
   LDA   (FP),Y
   LDY   #$03
   STA   (DPTR),Y
   SEC
   LDA   #$00
   SBC   #$06
   LDY   #$1F
   STA   (FP),Y
   LDA   #$00
   SBC   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$21
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$22
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@4
   LDA   #$00
   JMP   .sx_done@5
.sx_neg@4:
   LDA   #$FF
.sx_done@5:
   LDY   #$23
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$1E
   LDA   (FP),Y
   LDY   #$24
   CMP   (FP),Y
   BNE   .cmp_true@6
   LDY   #$1D
   LDA   (FP),Y
   LDY   #$23
   CMP   (FP),Y
   BNE   .cmp_true@6
   LDY   #$1C
   LDA   (FP),Y
   LDY   #$22
   CMP   (FP),Y
   BNE   .cmp_true@6
   LDY   #$1B
   LDA   (FP),Y
   LDY   #$21
   CMP   (FP),Y
   BNE   .cmp_true@6
   LDA   #$00
   JMP   .cmp_end@7
.cmp_true@6:
   LDA   #$01
.cmp_end@7:
   LDY   #$25
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
   ADC   #$BE
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$BD
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
   LDA   long_nested_arr+20
   LDY   #$27
   STA   (FP),Y
   LDA   long_nested_arr+21
   INY
   STA   (FP),Y
   LDA   long_nested_arr+22
   INY
   STA   (FP),Y
   LDA   long_nested_arr+23
   INY
   STA   (FP),Y
   SEC
   LDA   #$00
   SBC   #$06
   INY
   STA   (FP),Y
   LDA   #$00
   SBC   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$2D
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$2E
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@9
   LDA   #$00
   JMP   .sx_done@10
.sx_neg@9:
   LDA   #$FF
.sx_done@10:
   LDY   #$2F
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$2A
   LDA   (FP),Y
   LDY   #$30
   CMP   (FP),Y
   BNE   .cmp_true@11
   LDY   #$29
   LDA   (FP),Y
   LDY   #$2F
   CMP   (FP),Y
   BNE   .cmp_true@11
   LDY   #$28
   LDA   (FP),Y
   LDY   #$2E
   CMP   (FP),Y
   BNE   .cmp_true@11
   LDY   #$27
   LDA   (FP),Y
   LDY   #$2D
   CMP   (FP),Y
   BNE   .cmp_true@11
   LDA   #$00
   JMP   .cmp_end@12
.cmp_true@11:
   LDA   #$01
.cmp_end@12:
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
   ADC   #$BE
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$BD
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
   LDY   #$33
   STA   (FP),Y
   INY
   STA   (FP),Y
.loop@0_start:
   LDY   #$33
   LDA   (FP),Y
   SEC
   SBC   #$02
   INY
   LDA   (FP),Y
   SBC   #$00
   BVC   .cmp_novf@14
   EOR   #$80
.cmp_novf@14:
   ASL
   LDA   #$00
   ROL
   LDY   #$35
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@5
   JMP   .loop@0_break
.lb_skip@5:
   LDA   #$00
   LDY   #$37
   STA   (FP),Y
   INY
   STA   (FP),Y
.loop@1_start:
   LDY   #$37
   LDA   (FP),Y
   SEC
   SBC   #$03
   INY
   LDA   (FP),Y
   SBC   #$00
   BVC   .cmp_novf@15
   EOR   #$80
.cmp_novf@15:
   ASL
   LDA   #$00
   ROL
   LDY   #$39
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@4
   JMP   .loop@1_break
.lb_skip@4:
   LDY   #$34
   LDA   (FP),Y
   BNE   .thread@0
   DEY
   LDA   (FP),Y
   CMP   #$01
   BEQ   .cmp_true@16
.thread@0:
   LDA   #$00
   JMP   .cmp_end@17
.cmp_true@16:
   LDA   #$01
.cmp_end@17:
   LDY   #$3B
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@2
   LDY   #$38
   LDA   (FP),Y
   BNE   .thread@1
   DEY
   LDA   (FP),Y
   CMP   #$02
   BEQ   .cmp_true@19
.thread@1:
   LDA   #$00
   JMP   .cmp_end@20
.cmp_true@19:
   LDA   #$01
.cmp_end@20:
   LDY   #$3D
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
   LDY   #$3F
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@3:
   LDY   #$3F
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@3
   JMP   .loop@1_break
.lb_skip@3:
   LDY   #$33
   LDA   (FP),Y
   STA   HARGS
   INY
   LDA   (FP),Y
   STA   HARGS+1
   LDA   #$03
   STA   HARGS+2
   LDA   #$00
   STA   HARGS+3
   JSR   mul16
   LDA   HARGS+4
   LDY   #$41
   STA   (FP),Y
   LDA   HARGS+5
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   LDY   #$37
   ADC   (FP),Y
   LDY   #$43
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$38
   ADC   (FP),Y
   LDY   #$44
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$01
   LDY   #$45
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$46
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$47
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$48
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@22
   LDA   #$00
   JMP   .sx_done@23
.sx_neg@22:
   LDA   #$FF
.sx_done@23:
   LDY   #$49
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$47
   LDA   (FP),Y
   LDY   #$4B
   STA   (FP),Y
   LDY   #$48
   LDA   (FP),Y
   LDY   #$4C
   STA   (FP),Y
   LDY   #$49
   LDA   (FP),Y
   LDY   #$4D
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #<long_nested_arr
   INY
   STA   (FP),Y
   LDA   #>long_nested_arr
   INY
   STA   (FP),Y
   LDY   #$33
   LDA   (FP),Y
   LDY   #$51
   STA   (FP),Y
   STA   HARGS
   LDA   #$0C
   STA   HARGS+1
   JSR   mul8
   LDA   HARGS+2
   LDY   #$52
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$4F
   LDA   (FP),Y
   CLC
   LDY   #$53
   ADC   (FP),Y
   LDY   #$55
   STA   (FP),Y
   LDY   #$50
   LDA   (FP),Y
   LDY   #$54
   ADC   (FP),Y
   LDY   #$56
   STA   (FP),Y
   LDY   #$37
   LDA   (FP),Y
   LDY   #$57
   STA   (FP),Y
   STA   HARGS
   LDA   #$04
   STA   HARGS+1
   JSR   mul8
   LDA   HARGS+2
   LDY   #$58
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$55
   LDA   (FP),Y
   CLC
   LDY   #$59
   ADC   (FP),Y
   LDY   #$5B
   STA   (FP),Y
   LDY   #$56
   LDA   (FP),Y
   LDY   #$5A
   ADC   (FP),Y
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
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$5E
   STA   (FP),Y
   LDY   #$02
   LDA   (DPTR),Y
   LDY   #$5F
   STA   (FP),Y
   LDY   #$03
   LDA   (DPTR),Y
   LDY   #$60
   STA   (FP),Y
   LDY   #$4E
   CMP   (FP),Y
   BNE   .cmp_true@24
   LDY   #$5F
   LDA   (FP),Y
   LDY   #$4D
   CMP   (FP),Y
   BNE   .cmp_true@24
   LDY   #$5E
   LDA   (FP),Y
   LDY   #$4C
   CMP   (FP),Y
   BNE   .cmp_true@24
   LDY   #$5D
   LDA   (FP),Y
   LDY   #$4B
   CMP   (FP),Y
   BNE   .cmp_true@24
   LDA   #$00
   JMP   .cmp_end@25
.cmp_true@24:
   LDA   #$01
.cmp_end@25:
   LDY   #$61
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
   LDA   #$03

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$BE
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$BD
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
   LDY   #$37
   LDA   (FP),Y
   CLC
   ADC   #$01
   LDY   #$63
   STA   (FP),Y
   LDY   #$38
   LDA   (FP),Y
   ADC   #$00
   LDY   #$64
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$37
   STA   (FP),Y
   LDY   #$64
   LDA   (FP),Y
   LDY   #$38
   STA   (FP),Y
   JMP   .loop@1_start
.loop@1_break:
   LDY   #$33
   LDA   (FP),Y
   CLC
   ADC   #$01
   LDY   #$65
   STA   (FP),Y
   LDY   #$34
   LDA   (FP),Y
   ADC   #$00
   LDY   #$66
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$33
   STA   (FP),Y
   LDY   #$66
   LDA   (FP),Y
   LDY   #$34
   STA   (FP),Y
   JMP   .loop@0_start
.loop@0_break:
   LDA   #<dbl_nested_arr
   LDY   #$67
   STA   (FP),Y
   LDA   #>dbl_nested_arr
   INY
   STA   (FP),Y
   LDA   #$01
   STA   HARGS
   LDA   #$00
   LDX   #$08
   LSR   HARGS
.mul_loop@27:
   BCC   .mul_skip@28
   CLC
   ADC   #$10
.mul_skip@28:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@27
   LDY   #$6A
   STA   (FP),Y
   LDA   HARGS
   DEY
   STA   (FP),Y
   LDY   #$67
   LDA   (FP),Y
   CLC
   LDY   #$69
   ADC   (FP),Y
   LDY   #$6B
   STA   (FP),Y
   LDY   #$68
   LDA   (FP),Y
   LDY   #$6A
   ADC   (FP),Y
   LDY   #$6C
   STA   (FP),Y
   LDA   #$01
   STA   HARGS
   LDA   #$00
   LDX   #$08
   LSR   HARGS
.mul_loop@29:
   BCC   .mul_skip@30
   CLC
   ADC   #$08
.mul_skip@30:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@29
   LDY   #$6E
   STA   (FP),Y
   LDA   HARGS
   DEY
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
   LDY   #$04
   LDA   (DPTR),Y
   LDY   #$75
   STA   (FP),Y
   LDY   #$05
   LDA   (DPTR),Y
   LDY   #$76
   STA   (FP),Y
   LDY   #$06
   LDA   (DPTR),Y
   LDY   #$77
   STA   (FP),Y
   LDY   #$07
   LDA   (DPTR),Y
   LDY   #$78
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
   LDA   #$00
   STA   HARGS+8
   LDY   #$00
   STY   HARGS+9
   STY   HARGS+10
   STY   HARGS+11
   STY   HARGS+12
   STA   HARGS+13
   LDA   #$59
   STA   HARGS+14
   LDA   #$40
   STA   HARGS+15
   JSR   dadd
   LDA   HARGS+16
   LDY   #$79
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
   LDY   #$7D
   LDA   (FP),Y
   LDY   #$04
   STA   (DPTR),Y
   LDY   #$7E
   LDA   (FP),Y
   LDY   #$05
   STA   (DPTR),Y
   LDY   #$7F
   LDA   (FP),Y
   LDY   #$06
   STA   (DPTR),Y
   LDY   #$80
   LDA   (FP),Y
   LDY   #$07
   STA   (DPTR),Y
   LDY   #$80
   LDA   (FP),Y
   CMP   #$40
   BNE   .cmp_true@31
   DEY
   LDA   (FP),Y
   CMP   #$69
   BNE   .cmp_true@31
   DEY
   LDA   (FP),Y
   CMP   #$60
   BNE   .cmp_true@31
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@31
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@31
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@31
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@31
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@31
   LDA   #$00
   JMP   .cmp_end@32
.cmp_true@31:
   LDA   #$01
.cmp_end@32:
   LDY   #$81
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
   LDA   #$04

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$BE
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$BD
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
   LDA   #$00
   LDY   #$83
   STA   (FP),Y
   INY
   STA   (FP),Y
.loop@2_start:
   LDY   #$83
   LDA   (FP),Y
   SEC
   SBC   #$03
   INY
   LDA   (FP),Y
   SBC   #$00
   BVC   .cmp_novf@34
   EOR   #$80
.cmp_novf@34:
   ASL
   LDA   #$00
   ROL
   LDY   #$85
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@2
   JMP   .loop@2_break
.lb_skip@2:
   LDA   #$00
   LDY   #$87
   STA   (FP),Y
   INY
   STA   (FP),Y
.loop@3_start:
   LDY   #$87
   LDA   (FP),Y
   SEC
   SBC   #$02
   INY
   LDA   (FP),Y
   SBC   #$00
   BVC   .cmp_novf@35
   EOR   #$80
.cmp_novf@35:
   ASL
   LDA   #$00
   ROL
   LDY   #$89
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@1
   JMP   .loop@3_break
.lb_skip@1:
   LDY   #$84
   LDA   (FP),Y
   BNE   .thread@2
   DEY
   LDA   (FP),Y
   CMP   #$01
   BEQ   .cmp_true@36
.thread@2:
   LDA   #$00
   JMP   .cmp_end@37
.cmp_true@36:
   LDA   #$01
.cmp_end@37:
   LDY   #$8B
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@7
   LDY   #$88
   LDA   (FP),Y
   BNE   .thread@3
   DEY
   LDA   (FP),Y
   CMP   #$01
   BEQ   .cmp_true@39
.thread@3:
   LDA   #$00
   JMP   .cmp_end@40
.cmp_true@39:
   LDA   #$01
.cmp_end@40:
   LDY   #$8D
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
   LDY   #$8F
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@8:
   LDY   #$8F
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@0
   JMP   .loop@3_continue
.lb_skip@0:
   LDY   #$83
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
   LDY   #$91
   STA   (FP),Y
   LDA   HARGS+5
   INY
   STA   (FP),Y
   LDA   #$64
   CLC
   DEY
   ADC   (FP),Y
   LDY   #$93
   STA   (FP),Y
   LDA   #$00
   DEY
   ADC   (FP),Y
   LDY   #$94
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   LDY   #$87
   ADC   (FP),Y
   LDY   #$95
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$88
   ADC   (FP),Y
   LDY   #$96
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$97
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$98
   STA   (FP),Y
   LDA   #<dbl_nested_arr
   INY
   STA   (FP),Y
   LDA   #>dbl_nested_arr
   INY
   STA   (FP),Y
   LDY   #$83
   LDA   (FP),Y
   LDY   #$9B
   STA   (FP),Y
   STA   HARGS
   LDA   #$10
   STA   HARGS+1
   JSR   mul8
   LDA   HARGS+2
   LDY   #$9C
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$99
   LDA   (FP),Y
   CLC
   LDY   #$9D
   ADC   (FP),Y
   LDY   #$9F
   STA   (FP),Y
   LDY   #$9A
   LDA   (FP),Y
   LDY   #$9E
   ADC   (FP),Y
   LDY   #$A0
   STA   (FP),Y
   LDY   #$87
   LDA   (FP),Y
   LDY   #$A1
   STA   (FP),Y
   STA   HARGS
   LDA   #$08
   STA   HARGS+1
   JSR   mul8
   LDA   HARGS+2
   LDY   #$A2
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$9F
   LDA   (FP),Y
   CLC
   LDY   #$A3
   ADC   (FP),Y
   LDY   #$A5
   STA   (FP),Y
   LDY   #$A0
   LDA   (FP),Y
   LDY   #$A4
   ADC   (FP),Y
   LDY   #$A6
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$A7
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$A8
   STA   (FP),Y
   LDY   #$02
   LDA   (DPTR),Y
   LDY   #$A9
   STA   (FP),Y
   LDY   #$03
   LDA   (DPTR),Y
   LDY   #$AA
   STA   (FP),Y
   LDY   #$04
   LDA   (DPTR),Y
   LDY   #$AB
   STA   (FP),Y
   LDY   #$05
   LDA   (DPTR),Y
   LDY   #$AC
   STA   (FP),Y
   LDY   #$06
   LDA   (DPTR),Y
   LDY   #$AD
   STA   (FP),Y
   LDY   #$07
   LDA   (DPTR),Y
   LDY   #$AE
   STA   (FP),Y
   LDY   #$97
   LDA   (FP),Y
   STA   HARGS
   INY
   LDA   (FP),Y
   STA   HARGS+1
   JSR   i2d
   LDA   HARGS+2
   LDY   #$AF
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
   LDY   #$AE
   LDA   (FP),Y
   LDY   #$B6
   CMP   (FP),Y
   BNE   .cmp_true@42
   LDY   #$AD
   LDA   (FP),Y
   LDY   #$B5
   CMP   (FP),Y
   BNE   .cmp_true@42
   LDY   #$AC
   LDA   (FP),Y
   LDY   #$B4
   CMP   (FP),Y
   BNE   .cmp_true@42
   LDY   #$AB
   LDA   (FP),Y
   LDY   #$B3
   CMP   (FP),Y
   BNE   .cmp_true@42
   LDY   #$AA
   LDA   (FP),Y
   LDY   #$B2
   CMP   (FP),Y
   BNE   .cmp_true@42
   LDY   #$A9
   LDA   (FP),Y
   LDY   #$B1
   CMP   (FP),Y
   BNE   .cmp_true@42
   LDY   #$A8
   LDA   (FP),Y
   LDY   #$B0
   CMP   (FP),Y
   BNE   .cmp_true@42
   LDY   #$A7
   LDA   (FP),Y
   LDY   #$AF
   CMP   (FP),Y
   BNE   .cmp_true@42
   LDA   #$00
   JMP   .cmp_end@43
.cmp_true@42:
   LDA   #$01
.cmp_end@43:
   LDY   #$B7
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
   LDA   #$05

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$BE
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$BD
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
.loop@3_continue:
   LDY   #$87
   LDA   (FP),Y
   CLC
   ADC   #$01
   LDY   #$B9
   STA   (FP),Y
   LDY   #$88
   LDA   (FP),Y
   ADC   #$00
   LDY   #$BA
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$87
   STA   (FP),Y
   LDY   #$BA
   LDA   (FP),Y
   LDY   #$88
   STA   (FP),Y
   JMP   .loop@3_start
.loop@3_break:
   LDY   #$83
   LDA   (FP),Y
   CLC
   ADC   #$01
   LDY   #$BB
   STA   (FP),Y
   LDY   #$84
   LDA   (FP),Y
   ADC   #$00
   LDY   #$BC
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$83
   STA   (FP),Y
   LDY   #$BC
   LDA   (FP),Y
   LDY   #$84
   STA   (FP),Y
   JMP   .loop@2_start
.loop@2_break:
   LDX   #$00
   TXA

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$BE
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$BD
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

long_nested_arr:
   DC.L  $00000001
   DC.L  $00000002
   DC.L  $00000003
   DC.L  $00000004
   DC.L  $00000005
   DC.L  $00000006

dbl_nested_arr:
   DC.L  $00000000
   DC.L  $40590000
   DC.L  $00000000
   DC.L  $40594000
   DC.L  $00000000
   DC.L  $40598000
   DC.L  $00000000
   DC.L  $4059C000
   DC.L  $00000000
   DC.L  $405A0000
   DC.L  $00000000
   DC.L  $405A4000

unsigned_index:
   DC.W  $000A
