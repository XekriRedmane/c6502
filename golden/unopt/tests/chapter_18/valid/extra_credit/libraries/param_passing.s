SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24

pass_unions_and_structs:
   SUBROUTINE

   ; prologue: 46 arg bytes, 116 local bytes
   SEC
   LDA   SSP
   SBC   #$76
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$75
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   LDY   #$78
   LDA   (FP),Y
   CMP   #$00
   BNE   .thread@0
   DEY
   LDA   (FP),Y
   CMP   #$01
   BEQ   .cmp_true@0
.thread@0:
   LDA   #$00
   JMP   .cmp_end@1
.cmp_true@0:
   LDA   #$01
.cmp_end@1:
   LDY   #$01
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@8
   LDY   #$7A
   LDA   (FP),Y
   CMP   #$00
   BNE   .thread@1
   DEY
   LDA   (FP),Y
   CMP   #$02
   BEQ   .cmp_true@3
.thread@1:
   LDA   #$00
   JMP   .cmp_end@4
.cmp_true@3:
   LDA   #$01
.cmp_end@4:
   LDY   #$03
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@8
   LDA   #$01
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   JMP   .and_end@9
.and_false@8:
   LDA   #$00
   LDY   #$05
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@9:
   LDY   #$05
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@6
   LDY   #$86
   LDA   (FP),Y
   CMP   #$40
   BNE   .thread@2
   DEY
   LDA   (FP),Y
   CMP   #$10
   BNE   .thread@2
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .thread@2
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .thread@2
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .thread@2
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .thread@2
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .thread@2
   DEY
   LDA   (FP),Y
   CMP   #$00
   BEQ   .cmp_true@6
.thread@2:
   LDA   #$00
   JMP   .cmp_end@7
.cmp_true@6:
   LDA   #$01
.cmp_end@7:
   LDY   #$07
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@6
   LDA   #$01
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   JMP   .and_end@7
.and_false@6:
   LDA   #$00
   LDY   #$09
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@7:
   LDY   #$09
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@4
   LDY   #$A0
   LDA   (FP),Y
   CMP   #$00
   BNE   .thread@3
   DEY
   LDA   (FP),Y
   CMP   #$64
   BEQ   .cmp_true@9
.thread@3:
   LDA   #$00
   JMP   .cmp_end@10
.cmp_true@9:
   LDA   #$01
.cmp_end@10:
   LDY   #$0B
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@4
   LDA   #$01
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   JMP   .and_end@5
.and_false@4:
   LDA   #$00
   LDY   #$0D
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@5:
   LDY   #$0D
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@2
   LDY   #$A2
   LDA   (FP),Y
   CMP   #$00
   BNE   .thread@4
   DEY
   LDA   (FP),Y
   CMP   #$78
   BEQ   .cmp_true@12
.thread@4:
   LDA   #$00
   JMP   .cmp_end@13
.cmp_true@12:
   LDA   #$01
.cmp_end@13:
   LDY   #$0F
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
   LDY   #$11
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@3:
   LDY   #$11
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@0
   LDY   #$A4
   LDA   (FP),Y
   CMP   #$00
   BNE   .thread@5
   DEY
   LDA   (FP),Y
   CMP   #$82
   BEQ   .cmp_true@15
.thread@5:
   LDA   #$00
   JMP   .cmp_end@16
.cmp_true@15:
   LDA   #$01
.cmp_end@16:
   LDY   #$13
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@0
   LDA   #$01
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   JMP   .and_end@1
.and_false@0:
   LDA   #$00
   LDY   #$15
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@1:
   LDY   #$15
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lnot_true@18
   LDA   #$00
   JMP   .lnot_end@19
.lnot_true@18:
   LDA   #$01
.lnot_end@19:
   LDY   #$17
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
   TXA

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$A4
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$75
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
   CLC
   LDA   FP
   ADC   #$7B
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
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$1B
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$1C
   STA   (FP),Y
   SEC
   LDA   #$00
   SBC   #$18
   INY
   STA   (FP),Y
   LDA   #$00
   SBC   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$1F
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$20
   STA   (FP),Y
   LDY   #$1C
   LDA   (FP),Y
   LDY   #$20
   CMP   (FP),Y
   BNE   .thread@6
   LDY   #$1B
   LDA   (FP),Y
   LDY   #$1F
   CMP   (FP),Y
   BEQ   .cmp_true@20
.thread@6:
   LDA   #$00
   JMP   .cmp_end@21
.cmp_true@20:
   LDA   #$01
.cmp_end@21:
   LDY   #$21
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@1
   JMP   .and_false@11
.lb_skip@1:
   CLC
   LDA   FP
   ADC   #$7B
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
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$28
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$29
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$2A
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@23
   LDA   #$00
   JMP   .sx_done@24
.sx_neg@23:
   LDA   #$FF
.sx_done@24:
   LDY   #$2B
   STA   (FP),Y
   INY
   STA   (FP),Y
   CMP   #$07
   BNE   .thread@7
   CMP   #$5B
   BNE   .thread@7
   LDY   #$2A
   LDA   (FP),Y
   CMP   #$CD
   BNE   .thread@7
   DEY
   LDA   (FP),Y
   CMP   #$15
   BEQ   .cmp_true@25
.thread@7:
   LDA   #$00
   JMP   .cmp_end@26
.cmp_true@25:
   LDA   #$01
.cmp_end@26:
   LDY   #$2D
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@11
   LDA   #$01
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   JMP   .and_end@12
.and_false@11:
   LDA   #$00
   LDY   #$2F
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@12:
   LDY   #$2F
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lnot_true@28
   LDA   #$00
   JMP   .lnot_end@29
.lnot_true@28:
   LDA   #$01
.lnot_end@29:
   LDY   #$31
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
   ADC   #$A4
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$75
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
   CLC
   LDA   FP
   ADC   #$87
   LDY   #$35
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   LDA   #$00
   STA   HARGS
   LDX   #$08
   LSR   HARGS
.mul_loop@30:
   BCC   .mul_skip@31
   CLC
   ADC   #$08
.mul_skip@31:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@30
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
   LDA   #$24
   INY
   STA   (FP),Y
   LDA   #$40
   EOR   #$80
   INY
   STA   (FP),Y
   LDY   #$40
   LDA   (FP),Y
   LDY   #$48
   CMP   (FP),Y
   BNE   .thread@8
   LDY   #$3F
   LDA   (FP),Y
   LDY   #$47
   CMP   (FP),Y
   BNE   .thread@8
   LDY   #$3E
   LDA   (FP),Y
   LDY   #$46
   CMP   (FP),Y
   BNE   .thread@8
   LDY   #$3D
   LDA   (FP),Y
   LDY   #$45
   CMP   (FP),Y
   BNE   .thread@8
   LDY   #$3C
   LDA   (FP),Y
   LDY   #$44
   CMP   (FP),Y
   BNE   .thread@8
   LDY   #$3B
   LDA   (FP),Y
   LDY   #$43
   CMP   (FP),Y
   BNE   .thread@8
   LDY   #$3A
   LDA   (FP),Y
   LDY   #$42
   CMP   (FP),Y
   BNE   .thread@8
   LDY   #$39
   LDA   (FP),Y
   LDY   #$41
   CMP   (FP),Y
   BEQ   .cmp_true@32
.thread@8:
   LDA   #$00
   JMP   .cmp_end@33
.cmp_true@32:
   LDA   #$01
.cmp_end@33:
   LDY   #$49
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@0
   JMP   .and_false@14
.lb_skip@0:
   CLC
   LDA   FP
   ADC   #$87
   LDY   #$4D
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
.mul_loop@35:
   BCC   .mul_skip@36
   CLC
   ADC   #$08
.mul_skip@36:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@35
   LDY   #$4C
   STA   (FP),Y
   LDA   HARGS
   DEY
   STA   (FP),Y
   LDY   #$4D
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
   LDY   #$02
   LDA   (DPTR),Y
   LDY   #$53
   STA   (FP),Y
   LDY   #$03
   LDA   (DPTR),Y
   LDY   #$54
   STA   (FP),Y
   LDY   #$04
   LDA   (DPTR),Y
   LDY   #$55
   STA   (FP),Y
   LDY   #$05
   LDA   (DPTR),Y
   LDY   #$56
   STA   (FP),Y
   LDY   #$06
   LDA   (DPTR),Y
   LDY   #$57
   STA   (FP),Y
   LDY   #$07
   LDA   (DPTR),Y
   LDY   #$58
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
   LDA   #$26
   INY
   STA   (FP),Y
   LDA   #$40
   EOR   #$80
   INY
   STA   (FP),Y
   LDY   #$58
   LDA   (FP),Y
   LDY   #$60
   CMP   (FP),Y
   BNE   .thread@9
   LDY   #$57
   LDA   (FP),Y
   LDY   #$5F
   CMP   (FP),Y
   BNE   .thread@9
   LDY   #$56
   LDA   (FP),Y
   LDY   #$5E
   CMP   (FP),Y
   BNE   .thread@9
   LDY   #$55
   LDA   (FP),Y
   LDY   #$5D
   CMP   (FP),Y
   BNE   .thread@9
   LDY   #$54
   LDA   (FP),Y
   LDY   #$5C
   CMP   (FP),Y
   BNE   .thread@9
   LDY   #$53
   LDA   (FP),Y
   LDY   #$5B
   CMP   (FP),Y
   BNE   .thread@9
   LDY   #$52
   LDA   (FP),Y
   LDY   #$5A
   CMP   (FP),Y
   BNE   .thread@9
   LDY   #$51
   LDA   (FP),Y
   LDY   #$59
   CMP   (FP),Y
   BEQ   .cmp_true@37
.thread@9:
   LDA   #$00
   JMP   .cmp_end@38
.cmp_true@37:
   LDA   #$01
.cmp_end@38:
   LDY   #$61
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@14
   LDA   #$01
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   JMP   .and_end@15
.and_false@14:
   LDA   #$00
   LDY   #$63
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@15:
   LDY   #$63
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lnot_true@40
   LDA   #$00
   JMP   .lnot_end@41
.lnot_true@40:
   LDA   #$01
.lnot_end@41:
   LDY   #$65
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
   TXA

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$A4
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$75
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
   CLC
   LDA   FP
   ADC   #$97
   LDY   #$67
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
   LDY   #$69
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$6A
   STA   (FP),Y
   LDY   #$02
   LDA   (DPTR),Y
   LDY   #$6B
   STA   (FP),Y
   LDY   #$03
   LDA   (DPTR),Y
   LDY   #$6C
   STA   (FP),Y
   LDY   #$04
   LDA   (DPTR),Y
   LDY   #$6D
   STA   (FP),Y
   LDY   #$05
   LDA   (DPTR),Y
   LDY   #$6E
   STA   (FP),Y
   LDY   #$06
   LDA   (DPTR),Y
   LDY   #$6F
   STA   (FP),Y
   LDY   #$07
   LDA   (DPTR),Y
   LDY   #$70
   STA   (FP),Y
   CMP   #$40
   BNE   .thread@10
   DEY
   LDA   (FP),Y
   CMP   #$2A
   BNE   .thread@10
   DEY
   LDA   (FP),Y
   BNE   .thread@10
   DEY
   LDA   (FP),Y
   BNE   .thread@10
   DEY
   LDA   (FP),Y
   BNE   .thread@10
   DEY
   LDA   (FP),Y
   BNE   .thread@10
   DEY
   LDA   (FP),Y
   BNE   .thread@10
   DEY
   LDA   (FP),Y
   BEQ   .cmp_true@42
.thread@10:
   LDA   #$00
   JMP   .cmp_end@43
.cmp_true@42:
   LDA   #$01
.cmp_end@43:
   LDY   #$71
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lnot_true@45
   LDA   #$00
   JMP   .lnot_end@46
.lnot_true@45:
   LDA   #$01
.lnot_end@46:
   LDY   #$73
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
   TXA

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$A4
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
.if_end@17:
   LDX   #$00
   LDA   #$01

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$A4
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$75
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

pass_gp_union_in_memory:
   SUBROUTINE

   ; prologue: 40 arg bytes, 128 local bytes
   SEC
   LDA   SSP
   SBC   #$82
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$81
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
   SBC   #$01
   LDY   #$01
   STA   (FP),Y
   LDA   #$00
   SBC   #$00
   INY
   STA   (FP),Y
   LDY   #$98
   LDA   (FP),Y
   LDY   #$02
   CMP   (FP),Y
   BNE   .thread@11
   LDY   #$97
   LDA   (FP),Y
   LDY   #$01
   CMP   (FP),Y
   BEQ   .cmp_true@47
.thread@11:
   LDA   #$00
   JMP   .cmp_end@48
.cmp_true@47:
   LDA   #$01
.cmp_end@48:
   LDY   #$03
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@26
   SEC
   LDA   #$00
   SBC   #$02
   INY
   STA   (FP),Y
   LDA   #$00
   SBC   #$00
   INY
   STA   (FP),Y
   LDY   #$9A
   LDA   (FP),Y
   LDY   #$06
   CMP   (FP),Y
   BNE   .thread@12
   LDY   #$99
   LDA   (FP),Y
   LDY   #$05
   CMP   (FP),Y
   BEQ   .cmp_true@50
.thread@12:
   LDA   #$00
   JMP   .cmp_end@51
.cmp_true@50:
   LDA   #$01
.cmp_end@51:
   LDY   #$07
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@26
   LDA   #$01
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   JMP   .and_end@27
.and_false@26:
   LDA   #$00
   LDY   #$09
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@27:
   LDY   #$09
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@24
   SEC
   LDA   #$00
   SBC   #$03
   INY
   STA   (FP),Y
   LDA   #$00
   SBC   #$00
   INY
   STA   (FP),Y
   LDY   #$9C
   LDA   (FP),Y
   LDY   #$0C
   CMP   (FP),Y
   BNE   .thread@13
   LDY   #$9B
   LDA   (FP),Y
   LDY   #$0B
   CMP   (FP),Y
   BEQ   .cmp_true@53
.thread@13:
   LDA   #$00
   JMP   .cmp_end@54
.cmp_true@53:
   LDA   #$01
.cmp_end@54:
   LDY   #$0D
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@24
   LDA   #$01
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   JMP   .and_end@25
.and_false@24:
   LDA   #$00
   LDY   #$0F
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@25:
   LDY   #$0F
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@22
   SEC
   LDA   #$00
   SBC   #$04
   INY
   STA   (FP),Y
   LDA   #$00
   SBC   #$00
   INY
   STA   (FP),Y
   LDY   #$9E
   LDA   (FP),Y
   LDY   #$12
   CMP   (FP),Y
   BNE   .thread@14
   LDY   #$9D
   LDA   (FP),Y
   LDY   #$11
   CMP   (FP),Y
   BEQ   .cmp_true@56
.thread@14:
   LDA   #$00
   JMP   .cmp_end@57
.cmp_true@56:
   LDA   #$01
.cmp_end@57:
   LDY   #$13
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@22
   LDA   #$01
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   JMP   .and_end@23
.and_false@22:
   LDA   #$00
   LDY   #$15
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@23:
   LDY   #$15
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@20
   SEC
   LDA   #$00
   SBC   #$05
   INY
   STA   (FP),Y
   LDA   #$00
   SBC   #$00
   INY
   STA   (FP),Y
   LDY   #$A0
   LDA   (FP),Y
   LDY   #$18
   CMP   (FP),Y
   BNE   .thread@15
   LDY   #$9F
   LDA   (FP),Y
   LDY   #$17
   CMP   (FP),Y
   BEQ   .cmp_true@59
.thread@15:
   LDA   #$00
   JMP   .cmp_end@60
.cmp_true@59:
   LDA   #$01
.cmp_end@60:
   LDY   #$19
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@20
   LDA   #$01
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   JMP   .and_end@21
.and_false@20:
   LDA   #$00
   LDY   #$1B
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@21:
   LDY   #$1B
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@18
   SEC
   LDA   #$00
   SBC   #$06
   INY
   STA   (FP),Y
   LDA   #$00
   SBC   #$00
   INY
   STA   (FP),Y
   LDY   #$A2
   LDA   (FP),Y
   LDY   #$1E
   CMP   (FP),Y
   BNE   .thread@16
   LDY   #$A1
   LDA   (FP),Y
   LDY   #$1D
   CMP   (FP),Y
   BEQ   .cmp_true@62
.thread@16:
   LDA   #$00
   JMP   .cmp_end@63
.cmp_true@62:
   LDA   #$01
.cmp_end@63:
   LDY   #$1F
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@18
   LDA   #$01
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   JMP   .and_end@19
.and_false@18:
   LDA   #$00
   LDY   #$21
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@19:
   LDY   #$21
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lnot_true@65
   LDA   #$00
   JMP   .lnot_end@66
.lnot_true@65:
   LDA   #$01
.lnot_end@66:
   LDY   #$23
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
   ADC   #$AA
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$81
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
   CLC
   LDA   FP
   ADC   #$83
   LDY   #$27
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   LDA   #$00
   STA   HARGS
   LDX   #$08
   LSR   HARGS
.mul_loop@67:
   BCC   .mul_skip@68
   CLC
   ADC   #$08
.mul_skip@68:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@67
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
   LDY   #$02
   LDA   (DPTR),Y
   LDY   #$2D
   STA   (FP),Y
   LDY   #$03
   LDA   (DPTR),Y
   LDY   #$2E
   STA   (FP),Y
   LDY   #$04
   LDA   (DPTR),Y
   LDY   #$2F
   STA   (FP),Y
   LDY   #$05
   LDA   (DPTR),Y
   LDY   #$30
   STA   (FP),Y
   LDY   #$06
   LDA   (DPTR),Y
   LDY   #$31
   STA   (FP),Y
   LDY   #$07
   LDA   (DPTR),Y
   LDY   #$32
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
   LDA   #$24
   INY
   STA   (FP),Y
   LDA   #$40
   EOR   #$80
   INY
   STA   (FP),Y
   LDY   #$32
   LDA   (FP),Y
   LDY   #$3A
   CMP   (FP),Y
   BNE   .thread@17
   LDY   #$31
   LDA   (FP),Y
   LDY   #$39
   CMP   (FP),Y
   BNE   .thread@17
   LDY   #$30
   LDA   (FP),Y
   LDY   #$38
   CMP   (FP),Y
   BNE   .thread@17
   LDY   #$2F
   LDA   (FP),Y
   LDY   #$37
   CMP   (FP),Y
   BNE   .thread@17
   LDY   #$2E
   LDA   (FP),Y
   LDY   #$36
   CMP   (FP),Y
   BNE   .thread@17
   LDY   #$2D
   LDA   (FP),Y
   LDY   #$35
   CMP   (FP),Y
   BNE   .thread@17
   LDY   #$2C
   LDA   (FP),Y
   LDY   #$34
   CMP   (FP),Y
   BNE   .thread@17
   LDY   #$2B
   LDA   (FP),Y
   LDY   #$33
   CMP   (FP),Y
   BEQ   .cmp_true@69
.thread@17:
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
   BNE   .lb_skip@3
   JMP   .and_false@29
.lb_skip@3:
   CLC
   LDA   FP
   ADC   #$83
   LDY   #$3F
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
.mul_loop@72:
   BCC   .mul_skip@73
   CLC
   ADC   #$08
.mul_skip@73:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@72
   LDY   #$3E
   STA   (FP),Y
   LDA   HARGS
   DEY
   STA   (FP),Y
   LDY   #$3F
   LDA   (FP),Y
   CLC
   LDY   #$3D
   ADC   (FP),Y
   LDY   #$41
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$3E
   ADC   (FP),Y
   LDY   #$42
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$43
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$44
   STA   (FP),Y
   LDY   #$02
   LDA   (DPTR),Y
   LDY   #$45
   STA   (FP),Y
   LDY   #$03
   LDA   (DPTR),Y
   LDY   #$46
   STA   (FP),Y
   LDY   #$04
   LDA   (DPTR),Y
   LDY   #$47
   STA   (FP),Y
   LDY   #$05
   LDA   (DPTR),Y
   LDY   #$48
   STA   (FP),Y
   LDY   #$06
   LDA   (DPTR),Y
   LDY   #$49
   STA   (FP),Y
   LDY   #$07
   LDA   (DPTR),Y
   LDY   #$4A
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
   LDA   #$26
   INY
   STA   (FP),Y
   LDA   #$40
   EOR   #$80
   INY
   STA   (FP),Y
   LDY   #$4A
   LDA   (FP),Y
   LDY   #$52
   CMP   (FP),Y
   BNE   .thread@18
   LDY   #$49
   LDA   (FP),Y
   LDY   #$51
   CMP   (FP),Y
   BNE   .thread@18
   LDY   #$48
   LDA   (FP),Y
   LDY   #$50
   CMP   (FP),Y
   BNE   .thread@18
   LDY   #$47
   LDA   (FP),Y
   LDY   #$4F
   CMP   (FP),Y
   BNE   .thread@18
   LDY   #$46
   LDA   (FP),Y
   LDY   #$4E
   CMP   (FP),Y
   BNE   .thread@18
   LDY   #$45
   LDA   (FP),Y
   LDY   #$4D
   CMP   (FP),Y
   BNE   .thread@18
   LDY   #$44
   LDA   (FP),Y
   LDY   #$4C
   CMP   (FP),Y
   BNE   .thread@18
   LDY   #$43
   LDA   (FP),Y
   LDY   #$4B
   CMP   (FP),Y
   BEQ   .cmp_true@74
.thread@18:
   LDA   #$00
   JMP   .cmp_end@75
.cmp_true@74:
   LDA   #$01
.cmp_end@75:
   LDY   #$53
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@29
   LDA   #$01
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   JMP   .and_end@30
.and_false@29:
   LDA   #$00
   LDY   #$55
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@30:
   LDY   #$55
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lnot_true@77
   LDA   #$00
   JMP   .lnot_end@78
.lnot_true@77:
   LDA   #$01
.lnot_end@78:
   LDY   #$57
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@31
   LDX   #$00
   TXA

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$AA
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$81
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
.if_end@31:
   CLC
   LDA   FP
   ADC   #$93
   LDY   #$59
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
   LDY   #$5B
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$5C
   STA   (FP),Y
   SEC
   LDA   #$00
   SBC   #$18
   INY
   STA   (FP),Y
   LDA   #$00
   SBC   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$5F
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$60
   STA   (FP),Y
   LDY   #$5C
   LDA   (FP),Y
   LDY   #$60
   CMP   (FP),Y
   BNE   .thread@19
   LDY   #$5B
   LDA   (FP),Y
   LDY   #$5F
   CMP   (FP),Y
   BEQ   .cmp_true@79
.thread@19:
   LDA   #$00
   JMP   .cmp_end@80
.cmp_true@79:
   LDA   #$01
.cmp_end@80:
   LDY   #$61
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@2
   JMP   .and_false@32
.lb_skip@2:
   CLC
   LDA   FP
   ADC   #$93
   LDY   #$63
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$02
   LDY   #$65
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
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
   DEY
   LDA   (FP),Y
   LDY   #$69
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$6A
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@82
   LDA   #$00
   JMP   .sx_done@83
.sx_neg@82:
   LDA   #$FF
.sx_done@83:
   LDY   #$6B
   STA   (FP),Y
   INY
   STA   (FP),Y
   CMP   #$07
   BNE   .thread@20
   CMP   #$5B
   BNE   .thread@20
   LDY   #$6A
   LDA   (FP),Y
   CMP   #$CD
   BNE   .thread@20
   DEY
   LDA   (FP),Y
   CMP   #$15
   BEQ   .cmp_true@84
.thread@20:
   LDA   #$00
   JMP   .cmp_end@85
.cmp_true@84:
   LDA   #$01
.cmp_end@85:
   LDY   #$6D
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@32
   LDA   #$01
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   JMP   .and_end@33
.and_false@32:
   LDA   #$00
   LDY   #$6F
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@33:
   LDY   #$6F
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lnot_true@87
   LDA   #$00
   JMP   .lnot_end@88
.lnot_true@87:
   LDA   #$01
.lnot_end@88:
   LDY   #$71
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@34
   LDX   #$00
   TXA

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$AA
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$81
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
.if_end@34:
   CLC
   LDA   FP
   ADC   #$A3
   LDY   #$73
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
   CMP   #$40
   BNE   .thread@21
   DEY
   LDA   (FP),Y
   CMP   #$2A
   BNE   .thread@21
   DEY
   LDA   (FP),Y
   BNE   .thread@21
   DEY
   LDA   (FP),Y
   BNE   .thread@21
   DEY
   LDA   (FP),Y
   BNE   .thread@21
   DEY
   LDA   (FP),Y
   BNE   .thread@21
   DEY
   LDA   (FP),Y
   BNE   .thread@21
   DEY
   LDA   (FP),Y
   BEQ   .cmp_true@89
.thread@21:
   LDA   #$00
   JMP   .cmp_end@90
.cmp_true@89:
   LDA   #$01
.cmp_end@90:
   LDY   #$7D
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lnot_true@92
   LDA   #$00
   JMP   .lnot_end@93
.lnot_true@92:
   LDA   #$01
.lnot_end@93:
   LDY   #$7F
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@35
   LDX   #$00
   TXA

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$AA
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
.if_end@35:
   LDX   #$00
   LDA   #$01

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$AA
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$81
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

pass_xmm_union_in_memory:
   SUBROUTINE

   ; prologue: 80 arg bytes, 156 local bytes
   SEC
   LDA   SSP
   SBC   #$9E
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$9D
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   LDY   #$A6
   LDA   (FP),Y
   CMP   #$3F
   BNE   .thread@22
   DEY
   LDA   (FP),Y
   CMP   #$F0
   BNE   .thread@22
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .thread@22
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .thread@22
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .thread@22
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .thread@22
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .thread@22
   DEY
   LDA   (FP),Y
   CMP   #$00
   BEQ   .cmp_true@94
.thread@22:
   LDA   #$00
   JMP   .cmp_end@95
.cmp_true@94:
   LDA   #$01
.cmp_end@95:
   LDY   #$01
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@40
   LDY   #$AE
   LDA   (FP),Y
   CMP   #$40
   BNE   .thread@23
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .thread@23
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .thread@23
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .thread@23
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .thread@23
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .thread@23
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .thread@23
   DEY
   LDA   (FP),Y
   CMP   #$00
   BEQ   .cmp_true@97
.thread@23:
   LDA   #$00
   JMP   .cmp_end@98
.cmp_true@97:
   LDA   #$01
.cmp_end@98:
   LDY   #$03
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@40
   LDA   #$01
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   JMP   .and_end@41
.and_false@40:
   LDA   #$00
   LDY   #$05
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@41:
   LDY   #$05
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@38
   LDY   #$D6
   LDA   (FP),Y
   CMP   #$40
   BNE   .thread@24
   DEY
   LDA   (FP),Y
   CMP   #$08
   BNE   .thread@24
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .thread@24
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .thread@24
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .thread@24
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .thread@24
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .thread@24
   DEY
   LDA   (FP),Y
   CMP   #$00
   BEQ   .cmp_true@100
.thread@24:
   LDA   #$00
   JMP   .cmp_end@101
.cmp_true@100:
   LDA   #$01
.cmp_end@101:
   LDY   #$07
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@38
   LDA   #$01
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   JMP   .and_end@39
.and_false@38:
   LDA   #$00
   LDY   #$09
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@39:
   LDY   #$09
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@36
   LDY   #$DE
   LDA   (FP),Y
   CMP   #$40
   BNE   .thread@25
   DEY
   LDA   (FP),Y
   CMP   #$10
   BNE   .thread@25
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .thread@25
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .thread@25
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .thread@25
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .thread@25
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .thread@25
   DEY
   LDA   (FP),Y
   CMP   #$00
   BEQ   .cmp_true@103
.thread@25:
   LDA   #$00
   JMP   .cmp_end@104
.cmp_true@103:
   LDA   #$01
.cmp_end@104:
   LDY   #$0B
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@36
   LDA   #$01
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   JMP   .and_end@37
.and_false@36:
   LDA   #$00
   LDY   #$0D
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@37:
   LDY   #$0D
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lnot_true@106
   LDA   #$00
   JMP   .lnot_end@107
.lnot_true@106:
   LDA   #$01
.lnot_end@107:
   LDY   #$0F
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@42
   LDX   #$00
   TXA

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$EE
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$9D
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
.if_end@42:
   CLC
   LDA   FP
   ADC   #$AF
   LDY   #$13
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   LDA   #$00
   STA   HARGS
   LDX   #$08
   LSR   HARGS
.mul_loop@108:
   BCC   .mul_skip@109
   CLC
   ADC   #$08
.mul_skip@109:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@108
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
   LDA   #$24
   INY
   STA   (FP),Y
   LDA   #$40
   EOR   #$80
   INY
   STA   (FP),Y
   LDY   #$1E
   LDA   (FP),Y
   LDY   #$26
   CMP   (FP),Y
   BNE   .thread@26
   LDY   #$1D
   LDA   (FP),Y
   LDY   #$25
   CMP   (FP),Y
   BNE   .thread@26
   LDY   #$1C
   LDA   (FP),Y
   LDY   #$24
   CMP   (FP),Y
   BNE   .thread@26
   LDY   #$1B
   LDA   (FP),Y
   LDY   #$23
   CMP   (FP),Y
   BNE   .thread@26
   LDY   #$1A
   LDA   (FP),Y
   LDY   #$22
   CMP   (FP),Y
   BNE   .thread@26
   LDY   #$19
   LDA   (FP),Y
   LDY   #$21
   CMP   (FP),Y
   BNE   .thread@26
   LDY   #$18
   LDA   (FP),Y
   LDY   #$20
   CMP   (FP),Y
   BNE   .thread@26
   LDY   #$17
   LDA   (FP),Y
   LDY   #$1F
   CMP   (FP),Y
   BEQ   .cmp_true@110
.thread@26:
   LDA   #$00
   JMP   .cmp_end@111
.cmp_true@110:
   LDA   #$01
.cmp_end@111:
   LDY   #$27
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@6
   JMP   .and_false@43
.lb_skip@6:
   CLC
   LDA   FP
   ADC   #$AF
   LDY   #$2B
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
.mul_loop@113:
   BCC   .mul_skip@114
   CLC
   ADC   #$08
.mul_skip@114:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@113
   LDY   #$2A
   STA   (FP),Y
   LDA   HARGS
   DEY
   STA   (FP),Y
   LDY   #$2B
   LDA   (FP),Y
   CLC
   LDY   #$29
   ADC   (FP),Y
   LDY   #$2D
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$2A
   ADC   (FP),Y
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
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$30
   STA   (FP),Y
   LDY   #$02
   LDA   (DPTR),Y
   LDY   #$31
   STA   (FP),Y
   LDY   #$03
   LDA   (DPTR),Y
   LDY   #$32
   STA   (FP),Y
   LDY   #$04
   LDA   (DPTR),Y
   LDY   #$33
   STA   (FP),Y
   LDY   #$05
   LDA   (DPTR),Y
   LDY   #$34
   STA   (FP),Y
   LDY   #$06
   LDA   (DPTR),Y
   LDY   #$35
   STA   (FP),Y
   LDY   #$07
   LDA   (DPTR),Y
   LDY   #$36
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
   LDA   #$26
   INY
   STA   (FP),Y
   LDA   #$40
   EOR   #$80
   INY
   STA   (FP),Y
   LDY   #$36
   LDA   (FP),Y
   LDY   #$3E
   CMP   (FP),Y
   BNE   .thread@27
   LDY   #$35
   LDA   (FP),Y
   LDY   #$3D
   CMP   (FP),Y
   BNE   .thread@27
   LDY   #$34
   LDA   (FP),Y
   LDY   #$3C
   CMP   (FP),Y
   BNE   .thread@27
   LDY   #$33
   LDA   (FP),Y
   LDY   #$3B
   CMP   (FP),Y
   BNE   .thread@27
   LDY   #$32
   LDA   (FP),Y
   LDY   #$3A
   CMP   (FP),Y
   BNE   .thread@27
   LDY   #$31
   LDA   (FP),Y
   LDY   #$39
   CMP   (FP),Y
   BNE   .thread@27
   LDY   #$30
   LDA   (FP),Y
   LDY   #$38
   CMP   (FP),Y
   BNE   .thread@27
   LDY   #$2F
   LDA   (FP),Y
   LDY   #$37
   CMP   (FP),Y
   BEQ   .cmp_true@115
.thread@27:
   LDA   #$00
   JMP   .cmp_end@116
.cmp_true@115:
   LDA   #$01
.cmp_end@116:
   LDY   #$3F
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@43
   LDA   #$01
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   JMP   .and_end@44
.and_false@43:
   LDA   #$00
   LDY   #$41
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@44:
   LDY   #$41
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lnot_true@118
   LDA   #$00
   JMP   .lnot_end@119
.lnot_true@118:
   LDA   #$01
.lnot_end@119:
   LDY   #$43
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@45
   LDX   #$00
   TXA

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$EE
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$9D
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
.if_end@45:
   CLC
   LDA   FP
   ADC   #$BF
   LDY   #$47
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   LDA   #$00
   STA   HARGS
   LDX   #$08
   LSR   HARGS
.mul_loop@120:
   BCC   .mul_skip@121
   CLC
   ADC   #$08
.mul_skip@121:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@120
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
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$4B
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$4C
   STA   (FP),Y
   LDY   #$02
   LDA   (DPTR),Y
   LDY   #$4D
   STA   (FP),Y
   LDY   #$03
   LDA   (DPTR),Y
   LDY   #$4E
   STA   (FP),Y
   LDY   #$04
   LDA   (DPTR),Y
   LDY   #$4F
   STA   (FP),Y
   LDY   #$05
   LDA   (DPTR),Y
   LDY   #$50
   STA   (FP),Y
   LDY   #$06
   LDA   (DPTR),Y
   LDY   #$51
   STA   (FP),Y
   LDY   #$07
   LDA   (DPTR),Y
   LDY   #$52
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
   LDA   #$24
   INY
   STA   (FP),Y
   LDA   #$40
   EOR   #$80
   INY
   STA   (FP),Y
   LDY   #$52
   LDA   (FP),Y
   LDY   #$5A
   CMP   (FP),Y
   BNE   .thread@28
   LDY   #$51
   LDA   (FP),Y
   LDY   #$59
   CMP   (FP),Y
   BNE   .thread@28
   LDY   #$50
   LDA   (FP),Y
   LDY   #$58
   CMP   (FP),Y
   BNE   .thread@28
   LDY   #$4F
   LDA   (FP),Y
   LDY   #$57
   CMP   (FP),Y
   BNE   .thread@28
   LDY   #$4E
   LDA   (FP),Y
   LDY   #$56
   CMP   (FP),Y
   BNE   .thread@28
   LDY   #$4D
   LDA   (FP),Y
   LDY   #$55
   CMP   (FP),Y
   BNE   .thread@28
   LDY   #$4C
   LDA   (FP),Y
   LDY   #$54
   CMP   (FP),Y
   BNE   .thread@28
   LDY   #$4B
   LDA   (FP),Y
   LDY   #$53
   CMP   (FP),Y
   BEQ   .cmp_true@122
.thread@28:
   LDA   #$00
   JMP   .cmp_end@123
.cmp_true@122:
   LDA   #$01
.cmp_end@123:
   LDY   #$5B
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@5
   JMP   .and_false@46
.lb_skip@5:
   CLC
   LDA   FP
   ADC   #$BF
   LDY   #$5F
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
.mul_loop@125:
   BCC   .mul_skip@126
   CLC
   ADC   #$08
.mul_skip@126:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@125
   LDY   #$5E
   STA   (FP),Y
   LDA   HARGS
   DEY
   STA   (FP),Y
   LDY   #$5F
   LDA   (FP),Y
   CLC
   LDY   #$5D
   ADC   (FP),Y
   LDY   #$61
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$5E
   ADC   (FP),Y
   LDY   #$62
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$63
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$64
   STA   (FP),Y
   LDY   #$02
   LDA   (DPTR),Y
   LDY   #$65
   STA   (FP),Y
   LDY   #$03
   LDA   (DPTR),Y
   LDY   #$66
   STA   (FP),Y
   LDY   #$04
   LDA   (DPTR),Y
   LDY   #$67
   STA   (FP),Y
   LDY   #$05
   LDA   (DPTR),Y
   LDY   #$68
   STA   (FP),Y
   LDY   #$06
   LDA   (DPTR),Y
   LDY   #$69
   STA   (FP),Y
   LDY   #$07
   LDA   (DPTR),Y
   LDY   #$6A
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
   LDA   #$26
   INY
   STA   (FP),Y
   LDA   #$40
   EOR   #$80
   INY
   STA   (FP),Y
   LDY   #$6A
   LDA   (FP),Y
   LDY   #$72
   CMP   (FP),Y
   BNE   .thread@29
   LDY   #$69
   LDA   (FP),Y
   LDY   #$71
   CMP   (FP),Y
   BNE   .thread@29
   LDY   #$68
   LDA   (FP),Y
   LDY   #$70
   CMP   (FP),Y
   BNE   .thread@29
   LDY   #$67
   LDA   (FP),Y
   LDY   #$6F
   CMP   (FP),Y
   BNE   .thread@29
   LDY   #$66
   LDA   (FP),Y
   LDY   #$6E
   CMP   (FP),Y
   BNE   .thread@29
   LDY   #$65
   LDA   (FP),Y
   LDY   #$6D
   CMP   (FP),Y
   BNE   .thread@29
   LDY   #$64
   LDA   (FP),Y
   LDY   #$6C
   CMP   (FP),Y
   BNE   .thread@29
   LDY   #$63
   LDA   (FP),Y
   LDY   #$6B
   CMP   (FP),Y
   BEQ   .cmp_true@127
.thread@29:
   LDA   #$00
   JMP   .cmp_end@128
.cmp_true@127:
   LDA   #$01
.cmp_end@128:
   LDY   #$73
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@46
   LDA   #$01
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   JMP   .and_end@47
.and_false@46:
   LDA   #$00
   LDY   #$75
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@47:
   LDY   #$75
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lnot_true@130
   LDA   #$00
   JMP   .lnot_end@131
.lnot_true@130:
   LDA   #$01
.lnot_end@131:
   LDY   #$77
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@48
   LDX   #$00
   TXA

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$EE
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$9D
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
.if_end@48:
   CLC
   LDA   FP
   ADC   #$DF
   LDY   #$7B
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   LDA   #$00
   STA   HARGS
   LDX   #$08
   LSR   HARGS
.mul_loop@132:
   BCC   .mul_skip@133
   CLC
   ADC   #$08
.mul_skip@133:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@132
   LDY   #$7A
   STA   (FP),Y
   LDA   HARGS
   DEY
   STA   (FP),Y
   LDY   #$7B
   LDA   (FP),Y
   CLC
   LDY   #$79
   ADC   (FP),Y
   LDY   #$7D
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$7A
   ADC   (FP),Y
   LDY   #$7E
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$7F
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$80
   STA   (FP),Y
   LDY   #$02
   LDA   (DPTR),Y
   LDY   #$81
   STA   (FP),Y
   LDY   #$03
   LDA   (DPTR),Y
   LDY   #$82
   STA   (FP),Y
   LDY   #$04
   LDA   (DPTR),Y
   LDY   #$83
   STA   (FP),Y
   LDY   #$05
   LDA   (DPTR),Y
   LDY   #$84
   STA   (FP),Y
   LDY   #$06
   LDA   (DPTR),Y
   LDY   #$85
   STA   (FP),Y
   LDY   #$07
   LDA   (DPTR),Y
   LDY   #$86
   STA   (FP),Y
   CMP   #$41
   BNE   .thread@30
   DEY
   LDA   (FP),Y
   CMP   #$14
   BNE   .thread@30
   DEY
   LDA   (FP),Y
   CMP   #$24
   BNE   .thread@30
   DEY
   LDA   (FP),Y
   CMP   #$40
   BNE   .thread@30
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .thread@30
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .thread@30
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .thread@30
   DEY
   LDA   (FP),Y
   CMP   #$00
   BEQ   .cmp_true@134
.thread@30:
   LDA   #$00
   JMP   .cmp_end@135
.cmp_true@134:
   LDA   #$01
.cmp_end@135:
   LDY   #$87
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@4
   JMP   .and_false@49
.lb_skip@4:
   CLC
   LDA   FP
   ADC   #$DF
   LDY   #$8B
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
.mul_loop@137:
   BCC   .mul_skip@138
   CLC
   ADC   #$08
.mul_skip@138:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@137
   LDY   #$8A
   STA   (FP),Y
   LDA   HARGS
   DEY
   STA   (FP),Y
   LDY   #$8B
   LDA   (FP),Y
   CLC
   LDY   #$89
   ADC   (FP),Y
   LDY   #$8D
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$8A
   ADC   (FP),Y
   LDY   #$8E
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$8F
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$90
   STA   (FP),Y
   LDY   #$02
   LDA   (DPTR),Y
   LDY   #$91
   STA   (FP),Y
   LDY   #$03
   LDA   (DPTR),Y
   LDY   #$92
   STA   (FP),Y
   LDY   #$04
   LDA   (DPTR),Y
   LDY   #$93
   STA   (FP),Y
   LDY   #$05
   LDA   (DPTR),Y
   LDY   #$94
   STA   (FP),Y
   LDY   #$06
   LDA   (DPTR),Y
   LDY   #$95
   STA   (FP),Y
   LDY   #$07
   LDA   (DPTR),Y
   LDY   #$96
   STA   (FP),Y
   CMP   #$41
   BNE   .thread@31
   DEY
   LDA   (FP),Y
   CMP   #$8A
   BNE   .thread@31
   DEY
   LDA   (FP),Y
   CMP   #$39
   BNE   .thread@31
   DEY
   LDA   (FP),Y
   CMP   #$DE
   BNE   .thread@31
   DEY
   LDA   (FP),Y
   BNE   .thread@31
   DEY
   LDA   (FP),Y
   BNE   .thread@31
   DEY
   LDA   (FP),Y
   BNE   .thread@31
   DEY
   LDA   (FP),Y
   BEQ   .cmp_true@139
.thread@31:
   LDA   #$00
   JMP   .cmp_end@140
.cmp_true@139:
   LDA   #$01
.cmp_end@140:
   LDY   #$97
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@49
   LDA   #$01
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   JMP   .and_end@50
.and_false@49:
   LDA   #$00
   LDY   #$99
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@50:
   LDY   #$99
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lnot_true@142
   LDA   #$00
   JMP   .lnot_end@143
.lnot_true@142:
   LDA   #$01
.lnot_end@143:
   LDY   #$9B
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
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
   ADC   #$EE
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
.if_end@51:
   LDX   #$00
   LDA   #$01

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$EE
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$9D
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

pass_borderline_union:
   SUBROUTINE

   ; prologue: 21 arg bytes, 28 local bytes
   SEC
   LDA   SSP
   SBC   #$1E
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$1D
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   LDY   #$20
   LDA   (FP),Y
   CMP   #$00
   BNE   .thread@32
   DEY
   LDA   (FP),Y
   CMP   #$01
   BEQ   .cmp_true@144
.thread@32:
   LDA   #$00
   JMP   .cmp_end@145
.cmp_true@144:
   LDA   #$01
.cmp_end@145:
   LDY   #$01
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@58
   LDY   #$22
   LDA   (FP),Y
   CMP   #$00
   BNE   .thread@33
   DEY
   LDA   (FP),Y
   CMP   #$02
   BEQ   .cmp_true@147
.thread@33:
   LDA   #$00
   JMP   .cmp_end@148
.cmp_true@147:
   LDA   #$01
.cmp_end@148:
   LDY   #$03
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@58
   LDA   #$01
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   JMP   .and_end@59
.and_false@58:
   LDA   #$00
   LDY   #$05
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@59:
   LDY   #$05
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@56
   LDY   #$24
   LDA   (FP),Y
   CMP   #$00
   BNE   .thread@34
   DEY
   LDA   (FP),Y
   CMP   #$03
   BEQ   .cmp_true@150
.thread@34:
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
   BEQ   .and_false@56
   LDA   #$01
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   JMP   .and_end@57
.and_false@56:
   LDA   #$00
   LDY   #$09
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@57:
   LDY   #$09
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@54
   LDY   #$26
   LDA   (FP),Y
   CMP   #$00
   BNE   .thread@35
   DEY
   LDA   (FP),Y
   CMP   #$04
   BEQ   .cmp_true@153
.thread@35:
   LDA   #$00
   JMP   .cmp_end@154
.cmp_true@153:
   LDA   #$01
.cmp_end@154:
   LDY   #$0B
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@54
   LDA   #$01
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   JMP   .and_end@55
.and_false@54:
   LDA   #$00
   LDY   #$0D
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@55:
   LDY   #$0D
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@52
   LDY   #$28
   LDA   (FP),Y
   CMP   #$00
   BNE   .thread@36
   DEY
   LDA   (FP),Y
   CMP   #$05
   BEQ   .cmp_true@156
.thread@36:
   LDA   #$00
   JMP   .cmp_end@157
.cmp_true@156:
   LDA   #$01
.cmp_end@157:
   LDY   #$0F
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@52
   LDA   #$01
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   JMP   .and_end@53
.and_false@52:
   LDA   #$00
   LDY   #$11
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@53:
   LDY   #$11
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lnot_true@159
   LDA   #$00
   JMP   .lnot_end@160
.lnot_true@159:
   LDA   #$01
.lnot_end@160:
   LDY   #$13
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@60
   LDX   #$00
   TXA

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$33
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$1D
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
.if_end@60:
   CLC
   LDA   FP
   ADC   #$29
   LDY   #$17
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   LDA   #<str@0
   LDY   #$15
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
   INY
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$18
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
   JSR   strcmp
   LDY   #$19
   STA   (FP),Y
   TXA
   INY
   STA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@161
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@161
   LDA   #$00
   JMP   .cmp_end@162
.cmp_true@161:
   LDA   #$01
.cmp_end@162:
   LDY   #$1B
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@61
   LDX   #$00
   TXA

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$33
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
.if_end@61:
   LDX   #$00
   LDA   #$01

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$33
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$1D
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

pass_borderline_xmm_union:
   SUBROUTINE

   ; prologue: 72 arg bytes, 108 local bytes
   SEC
   LDA   SSP
   SBC   #$6E
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$6D
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   LDY   #$86
   LDA   (FP),Y
   CMP   #$40
   BNE   .thread@37
   DEY
   LDA   (FP),Y
   CMP   #$22
   BNE   .thread@37
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .thread@37
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .thread@37
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .thread@37
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .thread@37
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .thread@37
   DEY
   LDA   (FP),Y
   CMP   #$00
   BEQ   .cmp_true@164
.thread@37:
   LDA   #$00
   JMP   .cmp_end@165
.cmp_true@164:
   LDA   #$01
.cmp_end@165:
   LDY   #$01
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@68
   LDY   #$8E
   LDA   (FP),Y
   CMP   #$40
   BNE   .thread@38
   DEY
   LDA   (FP),Y
   CMP   #$20
   BNE   .thread@38
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .thread@38
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .thread@38
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .thread@38
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .thread@38
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .thread@38
   DEY
   LDA   (FP),Y
   CMP   #$00
   BEQ   .cmp_true@167
.thread@38:
   LDA   #$00
   JMP   .cmp_end@168
.cmp_true@167:
   LDA   #$01
.cmp_end@168:
   LDY   #$03
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@68
   LDA   #$01
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   JMP   .and_end@69
.and_false@68:
   LDA   #$00
   LDY   #$05
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@69:
   LDY   #$05
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@66
   LDY   #$96
   LDA   (FP),Y
   CMP   #$40
   BNE   .thread@39
   DEY
   LDA   (FP),Y
   CMP   #$1C
   BNE   .thread@39
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .thread@39
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .thread@39
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .thread@39
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .thread@39
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .thread@39
   DEY
   LDA   (FP),Y
   CMP   #$00
   BEQ   .cmp_true@170
.thread@39:
   LDA   #$00
   JMP   .cmp_end@171
.cmp_true@170:
   LDA   #$01
.cmp_end@171:
   LDY   #$07
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@66
   LDA   #$01
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   JMP   .and_end@67
.and_false@66:
   LDA   #$00
   LDY   #$09
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@67:
   LDY   #$09
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@64
   LDY   #$9E
   LDA   (FP),Y
   CMP   #$40
   BNE   .thread@40
   DEY
   LDA   (FP),Y
   CMP   #$18
   BNE   .thread@40
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .thread@40
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .thread@40
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .thread@40
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .thread@40
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .thread@40
   DEY
   LDA   (FP),Y
   CMP   #$00
   BEQ   .cmp_true@173
.thread@40:
   LDA   #$00
   JMP   .cmp_end@174
.cmp_true@173:
   LDA   #$01
.cmp_end@174:
   LDY   #$0B
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@64
   LDA   #$01
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   JMP   .and_end@65
.and_false@64:
   LDA   #$00
   LDY   #$0D
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@65:
   LDY   #$0D
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@62
   LDY   #$A6
   LDA   (FP),Y
   CMP   #$40
   BNE   .thread@41
   DEY
   LDA   (FP),Y
   CMP   #$14
   BNE   .thread@41
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .thread@41
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .thread@41
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .thread@41
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .thread@41
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .thread@41
   DEY
   LDA   (FP),Y
   CMP   #$00
   BEQ   .cmp_true@176
.thread@41:
   LDA   #$00
   JMP   .cmp_end@177
.cmp_true@176:
   LDA   #$01
.cmp_end@177:
   LDY   #$0F
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@62
   LDA   #$01
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   JMP   .and_end@63
.and_false@62:
   LDA   #$00
   LDY   #$11
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@63:
   LDY   #$11
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lnot_true@179
   LDA   #$00
   JMP   .lnot_end@180
.lnot_true@179:
   LDA   #$01
.lnot_end@180:
   LDY   #$13
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
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
   ADC   #$B6
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$6D
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
   ADC   #$6F
   LDY   #$17
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   LDA   #$00
   STA   HARGS
   LDX   #$08
   LSR   HARGS
.mul_loop@181:
   BCC   .mul_skip@182
   CLC
   ADC   #$08
.mul_skip@182:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@181
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
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$1B
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$1C
   STA   (FP),Y
   LDY   #$02
   LDA   (DPTR),Y
   LDY   #$1D
   STA   (FP),Y
   LDY   #$03
   LDA   (DPTR),Y
   LDY   #$1E
   STA   (FP),Y
   LDY   #$04
   LDA   (DPTR),Y
   LDY   #$1F
   STA   (FP),Y
   LDY   #$05
   LDA   (DPTR),Y
   LDY   #$20
   STA   (FP),Y
   LDY   #$06
   LDA   (DPTR),Y
   LDY   #$21
   STA   (FP),Y
   LDY   #$07
   LDA   (DPTR),Y
   LDY   #$22
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
   LDA   #$24
   INY
   STA   (FP),Y
   LDA   #$40
   EOR   #$80
   INY
   STA   (FP),Y
   LDY   #$22
   LDA   (FP),Y
   LDY   #$2A
   CMP   (FP),Y
   BNE   .thread@42
   LDY   #$21
   LDA   (FP),Y
   LDY   #$29
   CMP   (FP),Y
   BNE   .thread@42
   LDY   #$20
   LDA   (FP),Y
   LDY   #$28
   CMP   (FP),Y
   BNE   .thread@42
   LDY   #$1F
   LDA   (FP),Y
   LDY   #$27
   CMP   (FP),Y
   BNE   .thread@42
   LDY   #$1E
   LDA   (FP),Y
   LDY   #$26
   CMP   (FP),Y
   BNE   .thread@42
   LDY   #$1D
   LDA   (FP),Y
   LDY   #$25
   CMP   (FP),Y
   BNE   .thread@42
   LDY   #$1C
   LDA   (FP),Y
   LDY   #$24
   CMP   (FP),Y
   BNE   .thread@42
   LDY   #$1B
   LDA   (FP),Y
   LDY   #$23
   CMP   (FP),Y
   BEQ   .cmp_true@183
.thread@42:
   LDA   #$00
   JMP   .cmp_end@184
.cmp_true@183:
   LDA   #$01
.cmp_end@184:
   LDY   #$2B
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@8
   JMP   .and_false@71
.lb_skip@8:
   CLC
   LDA   FP
   ADC   #$6F
   LDY   #$2F
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
.mul_loop@186:
   BCC   .mul_skip@187
   CLC
   ADC   #$08
.mul_skip@187:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@186
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
   LDY   #$04
   LDA   (DPTR),Y
   LDY   #$37
   STA   (FP),Y
   LDY   #$05
   LDA   (DPTR),Y
   LDY   #$38
   STA   (FP),Y
   LDY   #$06
   LDA   (DPTR),Y
   LDY   #$39
   STA   (FP),Y
   LDY   #$07
   LDA   (DPTR),Y
   LDY   #$3A
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
   LDA   #$26
   INY
   STA   (FP),Y
   LDA   #$40
   EOR   #$80
   INY
   STA   (FP),Y
   LDY   #$3A
   LDA   (FP),Y
   LDY   #$42
   CMP   (FP),Y
   BNE   .thread@43
   LDY   #$39
   LDA   (FP),Y
   LDY   #$41
   CMP   (FP),Y
   BNE   .thread@43
   LDY   #$38
   LDA   (FP),Y
   LDY   #$40
   CMP   (FP),Y
   BNE   .thread@43
   LDY   #$37
   LDA   (FP),Y
   LDY   #$3F
   CMP   (FP),Y
   BNE   .thread@43
   LDY   #$36
   LDA   (FP),Y
   LDY   #$3E
   CMP   (FP),Y
   BNE   .thread@43
   LDY   #$35
   LDA   (FP),Y
   LDY   #$3D
   CMP   (FP),Y
   BNE   .thread@43
   LDY   #$34
   LDA   (FP),Y
   LDY   #$3C
   CMP   (FP),Y
   BNE   .thread@43
   LDY   #$33
   LDA   (FP),Y
   LDY   #$3B
   CMP   (FP),Y
   BEQ   .cmp_true@188
.thread@43:
   LDA   #$00
   JMP   .cmp_end@189
.cmp_true@188:
   LDA   #$01
.cmp_end@189:
   LDY   #$43
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@71
   LDA   #$01
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   JMP   .and_end@72
.and_false@71:
   LDA   #$00
   LDY   #$45
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@72:
   LDY   #$45
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lnot_true@191
   LDA   #$00
   JMP   .lnot_end@192
.lnot_true@191:
   LDA   #$01
.lnot_end@192:
   LDY   #$47
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
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
   ADC   #$B6
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$6D
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
   CLC
   LDA   FP
   ADC   #$A7
   LDY   #$4B
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   LDA   #$00
   STA   HARGS
   LDX   #$08
   LSR   HARGS
.mul_loop@193:
   BCC   .mul_skip@194
   CLC
   ADC   #$08
.mul_skip@194:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@193
   LDY   #$4A
   STA   (FP),Y
   LDA   HARGS
   DEY
   STA   (FP),Y
   LDY   #$4B
   LDA   (FP),Y
   CLC
   LDY   #$49
   ADC   (FP),Y
   LDY   #$4D
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$4A
   ADC   (FP),Y
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
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$50
   STA   (FP),Y
   LDY   #$02
   LDA   (DPTR),Y
   LDY   #$51
   STA   (FP),Y
   LDY   #$03
   LDA   (DPTR),Y
   LDY   #$52
   STA   (FP),Y
   LDY   #$04
   LDA   (DPTR),Y
   LDY   #$53
   STA   (FP),Y
   LDY   #$05
   LDA   (DPTR),Y
   LDY   #$54
   STA   (FP),Y
   LDY   #$06
   LDA   (DPTR),Y
   LDY   #$55
   STA   (FP),Y
   LDY   #$07
   LDA   (DPTR),Y
   LDY   #$56
   STA   (FP),Y
   CMP   #$41
   BNE   .thread@44
   DEY
   LDA   (FP),Y
   CMP   #$24
   BNE   .thread@44
   DEY
   LDA   (FP),Y
   CMP   #$24
   BNE   .thread@44
   DEY
   LDA   (FP),Y
   CMP   #$40
   BNE   .thread@44
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .thread@44
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .thread@44
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .thread@44
   DEY
   LDA   (FP),Y
   CMP   #$00
   BEQ   .cmp_true@195
.thread@44:
   LDA   #$00
   JMP   .cmp_end@196
.cmp_true@195:
   LDA   #$01
.cmp_end@196:
   LDY   #$57
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@7
   JMP   .and_false@74
.lb_skip@7:
   CLC
   LDA   FP
   ADC   #$A7
   LDY   #$5B
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
.mul_loop@198:
   BCC   .mul_skip@199
   CLC
   ADC   #$08
.mul_skip@199:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@198
   LDY   #$5A
   STA   (FP),Y
   LDA   HARGS
   DEY
   STA   (FP),Y
   LDY   #$5B
   LDA   (FP),Y
   CLC
   LDY   #$59
   ADC   (FP),Y
   LDY   #$5D
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$5A
   ADC   (FP),Y
   LDY   #$5E
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$5F
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$60
   STA   (FP),Y
   LDY   #$02
   LDA   (DPTR),Y
   LDY   #$61
   STA   (FP),Y
   LDY   #$03
   LDA   (DPTR),Y
   LDY   #$62
   STA   (FP),Y
   LDY   #$04
   LDA   (DPTR),Y
   LDY   #$63
   STA   (FP),Y
   LDY   #$05
   LDA   (DPTR),Y
   LDY   #$64
   STA   (FP),Y
   LDY   #$06
   LDA   (DPTR),Y
   LDY   #$65
   STA   (FP),Y
   LDY   #$07
   LDA   (DPTR),Y
   LDY   #$66
   STA   (FP),Y
   CMP   #$41
   BNE   .thread@45
   DEY
   LDA   (FP),Y
   CMP   #$9A
   BNE   .thread@45
   DEY
   LDA   (FP),Y
   CMP   #$39
   BNE   .thread@45
   DEY
   LDA   (FP),Y
   CMP   #$DE
   BNE   .thread@45
   DEY
   LDA   (FP),Y
   BNE   .thread@45
   DEY
   LDA   (FP),Y
   BNE   .thread@45
   DEY
   LDA   (FP),Y
   BNE   .thread@45
   DEY
   LDA   (FP),Y
   BEQ   .cmp_true@200
.thread@45:
   LDA   #$00
   JMP   .cmp_end@201
.cmp_true@200:
   LDA   #$01
.cmp_end@201:
   LDY   #$67
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@74
   LDA   #$01
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   JMP   .and_end@75
.and_false@74:
   LDA   #$00
   LDY   #$69
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@75:
   LDY   #$69
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lnot_true@203
   LDA   #$00
   JMP   .lnot_end@204
.lnot_true@203:
   LDA   #$01
.lnot_end@204:
   LDY   #$6B
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@76
   LDX   #$00
   TXA

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$B6
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
.if_end@76:
   LDX   #$00
   LDA   #$01

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$B6
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$6D
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

pass_mixed_reg_in_memory:
   SUBROUTINE

   ; prologue: 60 arg bytes, 76 local bytes
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

   LDY   #$56
   LDA   (FP),Y
   CMP   #$40
   BNE   .thread@46
   DEY
   LDA   (FP),Y
   CMP   #$59
   BNE   .thread@46
   DEY
   LDA   (FP),Y
   CMP   #$4C
   BNE   .thread@46
   DEY
   LDA   (FP),Y
   CMP   #$CC
   BNE   .thread@46
   DEY
   LDA   (FP),Y
   CMP   #$CC
   BNE   .thread@46
   DEY
   LDA   (FP),Y
   CMP   #$CC
   BNE   .thread@46
   DEY
   LDA   (FP),Y
   CMP   #$CC
   BNE   .thread@46
   DEY
   LDA   (FP),Y
   CMP   #$CD
   BEQ   .cmp_true@205
.thread@46:
   LDA   #$00
   JMP   .cmp_end@206
.cmp_true@205:
   LDA   #$01
.cmp_end@206:
   LDY   #$01
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@93
   LDY   #$5E
   LDA   (FP),Y
   CMP   #$40
   BNE   .thread@47
   DEY
   LDA   (FP),Y
   CMP   #$59
   BNE   .thread@47
   DEY
   LDA   (FP),Y
   CMP   #$93
   BNE   .thread@47
   DEY
   LDA   (FP),Y
   CMP   #$33
   BNE   .thread@47
   DEY
   LDA   (FP),Y
   CMP   #$33
   BNE   .thread@47
   DEY
   LDA   (FP),Y
   CMP   #$33
   BNE   .thread@47
   DEY
   LDA   (FP),Y
   CMP   #$33
   BNE   .thread@47
   DEY
   LDA   (FP),Y
   CMP   #$33
   BEQ   .cmp_true@208
.thread@47:
   LDA   #$00
   JMP   .cmp_end@209
.cmp_true@208:
   LDA   #$01
.cmp_end@209:
   LDY   #$03
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@93
   LDA   #$01
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   JMP   .and_end@94
.and_false@93:
   LDA   #$00
   LDY   #$05
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@94:
   LDY   #$05
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@91
   LDY   #$66
   LDA   (FP),Y
   CMP   #$40
   BNE   .thread@48
   DEY
   LDA   (FP),Y
   CMP   #$59
   BNE   .thread@48
   DEY
   LDA   (FP),Y
   CMP   #$D9
   BNE   .thread@48
   DEY
   LDA   (FP),Y
   CMP   #$99
   BNE   .thread@48
   DEY
   LDA   (FP),Y
   CMP   #$99
   BNE   .thread@48
   DEY
   LDA   (FP),Y
   CMP   #$99
   BNE   .thread@48
   DEY
   LDA   (FP),Y
   CMP   #$99
   BNE   .thread@48
   DEY
   LDA   (FP),Y
   CMP   #$9A
   BEQ   .cmp_true@211
.thread@48:
   LDA   #$00
   JMP   .cmp_end@212
.cmp_true@211:
   LDA   #$01
.cmp_end@212:
   LDY   #$07
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@91
   LDA   #$01
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   JMP   .and_end@92
.and_false@91:
   LDA   #$00
   LDY   #$09
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@92:
   LDY   #$09
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@89
   LDY   #$6E
   LDA   (FP),Y
   CMP   #$40
   BNE   .thread@49
   DEY
   LDA   (FP),Y
   CMP   #$5A
   BNE   .thread@49
   DEY
   LDA   (FP),Y
   CMP   #$20
   BNE   .thread@49
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .thread@49
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .thread@49
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .thread@49
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .thread@49
   DEY
   LDA   (FP),Y
   CMP   #$00
   BEQ   .cmp_true@214
.thread@49:
   LDA   #$00
   JMP   .cmp_end@215
.cmp_true@214:
   LDA   #$01
.cmp_end@215:
   LDY   #$0B
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@89
   LDA   #$01
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   JMP   .and_end@90
.and_false@89:
   LDA   #$00
   LDY   #$0D
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@90:
   LDY   #$0D
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@87
   LDY   #$70
   LDA   (FP),Y
   CMP   #$00
   BNE   .thread@50
   DEY
   LDA   (FP),Y
   CMP   #$4B
   BEQ   .cmp_true@217
.thread@50:
   LDA   #$00
   JMP   .cmp_end@218
.cmp_true@217:
   LDA   #$01
.cmp_end@218:
   LDY   #$0F
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@87
   LDA   #$01
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   JMP   .and_end@88
.and_false@87:
   LDA   #$00
   LDY   #$11
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@88:
   LDY   #$11
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@85
   LDY   #$72
   LDA   (FP),Y
   CMP   #$00
   BNE   .thread@51
   DEY
   LDA   (FP),Y
   CMP   #$4C
   BEQ   .cmp_true@220
.thread@51:
   LDA   #$00
   JMP   .cmp_end@221
.cmp_true@220:
   LDA   #$01
.cmp_end@221:
   LDY   #$13
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@85
   LDA   #$01
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   JMP   .and_end@86
.and_false@85:
   LDA   #$00
   LDY   #$15
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@86:
   LDY   #$15
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@83
   LDY   #$74
   LDA   (FP),Y
   CMP   #$00
   BNE   .thread@52
   DEY
   LDA   (FP),Y
   CMP   #$4D
   BEQ   .cmp_true@223
.thread@52:
   LDA   #$00
   JMP   .cmp_end@224
.cmp_true@223:
   LDA   #$01
.cmp_end@224:
   LDY   #$17
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@83
   LDA   #$01
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   JMP   .and_end@84
.and_false@83:
   LDA   #$00
   LDY   #$19
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@84:
   LDY   #$19
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@81
   LDY   #$76
   LDA   (FP),Y
   CMP   #$00
   BNE   .thread@53
   DEY
   LDA   (FP),Y
   CMP   #$4E
   BEQ   .cmp_true@226
.thread@53:
   LDA   #$00
   JMP   .cmp_end@227
.cmp_true@226:
   LDA   #$01
.cmp_end@227:
   LDY   #$1B
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@81
   LDA   #$01
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   JMP   .and_end@82
.and_false@81:
   LDA   #$00
   LDY   #$1D
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@82:
   LDY   #$1D
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@79
   LDY   #$78
   LDA   (FP),Y
   CMP   #$00
   BNE   .thread@54
   DEY
   LDA   (FP),Y
   CMP   #$4F
   BEQ   .cmp_true@229
.thread@54:
   LDA   #$00
   JMP   .cmp_end@230
.cmp_true@229:
   LDA   #$01
.cmp_end@230:
   LDY   #$1F
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@79
   LDA   #$01
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   JMP   .and_end@80
.and_false@79:
   LDA   #$00
   LDY   #$21
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@80:
   LDY   #$21
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@77
   LDY   #$7A
   LDA   (FP),Y
   CMP   #$00
   BNE   .thread@55
   DEY
   LDA   (FP),Y
   CMP   #$50
   BEQ   .cmp_true@232
.thread@55:
   LDA   #$00
   JMP   .cmp_end@233
.cmp_true@232:
   LDA   #$01
.cmp_end@233:
   LDY   #$23
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@77
   LDA   #$01
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   JMP   .and_end@78
.and_false@77:
   LDA   #$00
   LDY   #$25
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@78:
   LDY   #$25
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lnot_true@235
   LDA   #$00
   JMP   .lnot_end@236
.lnot_true@235:
   LDA   #$01
.lnot_end@236:
   LDY   #$27
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@95
   LDX   #$00
   TXA

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$8A
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
.if_end@95:
   CLC
   LDA   FP
   ADC   #$7B
   LDY   #$2B
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   LDA   #$00
   STA   HARGS
   LDX   #$08
   LSR   HARGS
.mul_loop@237:
   BCC   .mul_skip@238
   CLC
   ADC   #$08
.mul_skip@238:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@237
   LDY   #$2A
   STA   (FP),Y
   LDA   HARGS
   DEY
   STA   (FP),Y
   LDY   #$2B
   LDA   (FP),Y
   CLC
   LDY   #$29
   ADC   (FP),Y
   LDY   #$2D
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$2A
   ADC   (FP),Y
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
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$30
   STA   (FP),Y
   LDY   #$02
   LDA   (DPTR),Y
   LDY   #$31
   STA   (FP),Y
   LDY   #$03
   LDA   (DPTR),Y
   LDY   #$32
   STA   (FP),Y
   LDY   #$04
   LDA   (DPTR),Y
   LDY   #$33
   STA   (FP),Y
   LDY   #$05
   LDA   (DPTR),Y
   LDY   #$34
   STA   (FP),Y
   LDY   #$06
   LDA   (DPTR),Y
   LDY   #$35
   STA   (FP),Y
   LDY   #$07
   LDA   (DPTR),Y
   LDY   #$36
   STA   (FP),Y
   CMP   #$00
   BNE   .thread@56
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .thread@56
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .thread@56
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .thread@56
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .thread@56
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .thread@56
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .thread@56
   DEY
   LDA   (FP),Y
   CMP   #$00
   BEQ   .cmp_true@239
.thread@56:
   LDA   #$00
   JMP   .cmp_end@240
.cmp_true@239:
   LDA   #$01
.cmp_end@240:
   LDY   #$37
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@9
   JMP   .and_false@96
.lb_skip@9:
   CLC
   LDA   FP
   ADC   #$7B
   LDY   #$3B
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
.mul_loop@242:
   BCC   .mul_skip@243
   CLC
   ADC   #$08
.mul_skip@243:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@242
   LDY   #$3A
   STA   (FP),Y
   LDA   HARGS
   DEY
   STA   (FP),Y
   LDY   #$3B
   LDA   (FP),Y
   CLC
   LDY   #$39
   ADC   (FP),Y
   LDY   #$3D
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$3A
   ADC   (FP),Y
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
   BNE   .thread@57
   DEY
   LDA   (FP),Y
   CMP   #$62
   BNE   .thread@57
   DEY
   LDA   (FP),Y
   CMP   #$D0
   BNE   .thread@57
   DEY
   LDA   (FP),Y
   BNE   .thread@57
   DEY
   LDA   (FP),Y
   BNE   .thread@57
   DEY
   LDA   (FP),Y
   BNE   .thread@57
   DEY
   LDA   (FP),Y
   BNE   .thread@57
   DEY
   LDA   (FP),Y
   BEQ   .cmp_true@244
.thread@57:
   LDA   #$00
   JMP   .cmp_end@245
.cmp_true@244:
   LDA   #$01
.cmp_end@245:
   LDY   #$47
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@96
   LDA   #$01
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   JMP   .and_end@97
.and_false@96:
   LDA   #$00
   LDY   #$49
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@97:
   LDY   #$49
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lnot_true@247
   LDA   #$00
   JMP   .lnot_end@248
.lnot_true@247:
   LDA   #$01
.lnot_end@248:
   LDY   #$4B
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
   ADC   #$8A
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
.if_end@98:
   LDX   #$00
   LDA   #$01

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$8A
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

pass_uneven_union_in_memory:
   SUBROUTINE

   ; prologue: 39 arg bytes, 78 local bytes
   SEC
   LDA   SSP
   SBC   #$50
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$4F
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   LDY   #$52
   LDA   (FP),Y
   CMP   #$04
   BNE   .thread@58
   DEY
   LDA   (FP),Y
   CMP   #$4C
   BEQ   .cmp_true@249
.thread@58:
   LDA   #$00
   JMP   .cmp_end@250
.cmp_true@249:
   LDA   #$01
.cmp_end@250:
   LDY   #$01
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@105
   LDY   #$54
   LDA   (FP),Y
   CMP   #$08
   BNE   .thread@59
   DEY
   LDA   (FP),Y
   CMP   #$98
   BEQ   .cmp_true@252
.thread@59:
   LDA   #$00
   JMP   .cmp_end@253
.cmp_true@252:
   LDA   #$01
.cmp_end@253:
   LDY   #$03
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@105
   LDA   #$01
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   JMP   .and_end@106
.and_false@105:
   LDA   #$00
   LDY   #$05
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@106:
   LDY   #$05
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@103
   LDY   #$56
   LDA   (FP),Y
   CMP   #$0C
   BNE   .thread@60
   DEY
   LDA   (FP),Y
   CMP   #$E4
   BEQ   .cmp_true@255
.thread@60:
   LDA   #$00
   JMP   .cmp_end@256
.cmp_true@255:
   LDA   #$01
.cmp_end@256:
   LDY   #$07
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@103
   LDA   #$01
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   JMP   .and_end@104
.and_false@103:
   LDA   #$00
   LDY   #$09
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@104:
   LDY   #$09
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@101
   LDY   #$58
   LDA   (FP),Y
   CMP   #$11
   BNE   .thread@61
   DEY
   LDA   (FP),Y
   CMP   #$30
   BEQ   .cmp_true@258
.thread@61:
   LDA   #$00
   JMP   .cmp_end@259
.cmp_true@258:
   LDA   #$01
.cmp_end@259:
   LDY   #$0B
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@101
   LDA   #$01
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   JMP   .and_end@102
.and_false@101:
   LDA   #$00
   LDY   #$0D
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@102:
   LDY   #$0D
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@99
   LDY   #$5A
   LDA   (FP),Y
   CMP   #$15
   BNE   .thread@62
   DEY
   LDA   (FP),Y
   CMP   #$7C
   BEQ   .cmp_true@261
.thread@62:
   LDA   #$00
   JMP   .cmp_end@262
.cmp_true@261:
   LDA   #$01
.cmp_end@262:
   LDY   #$0F
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@99
   LDA   #$01
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   JMP   .and_end@100
.and_false@99:
   LDA   #$00
   LDY   #$11
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@100:
   LDY   #$11
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lnot_true@264
   LDA   #$00
   JMP   .lnot_end@265
.lnot_true@264:
   LDA   #$01
.lnot_end@265:
   LDY   #$13
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
   ADC   #$77
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$4F
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
   ADC   #$5B
   LDY   #$17
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   LDA   #$00
   STA   HARGS
   LDX   #$08
   LSR   HARGS
.mul_loop@266:
   BCC   .mul_skip@267
   CLC
   ADC   #$08
.mul_skip@267:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@266
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
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$1B
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$1C
   STA   (FP),Y
   LDY   #$02
   LDA   (DPTR),Y
   LDY   #$1D
   STA   (FP),Y
   LDY   #$03
   LDA   (DPTR),Y
   LDY   #$1E
   STA   (FP),Y
   LDY   #$04
   LDA   (DPTR),Y
   LDY   #$1F
   STA   (FP),Y
   LDY   #$05
   LDA   (DPTR),Y
   LDY   #$20
   STA   (FP),Y
   LDY   #$06
   LDA   (DPTR),Y
   LDY   #$21
   STA   (FP),Y
   LDY   #$07
   LDA   (DPTR),Y
   LDY   #$22
   STA   (FP),Y
   CMP   #$00
   BNE   .thread@63
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .thread@63
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .thread@63
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .thread@63
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .thread@63
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .thread@63
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .thread@63
   DEY
   LDA   (FP),Y
   CMP   #$00
   BEQ   .cmp_true@268
.thread@63:
   LDA   #$00
   JMP   .cmp_end@269
.cmp_true@268:
   LDA   #$01
.cmp_end@269:
   LDY   #$23
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@10
   JMP   .and_false@108
.lb_skip@10:
   CLC
   LDA   FP
   ADC   #$5B
   LDY   #$27
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
.mul_loop@271:
   BCC   .mul_skip@272
   CLC
   ADC   #$08
.mul_skip@272:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@271
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
   LDY   #$02
   LDA   (DPTR),Y
   LDY   #$2D
   STA   (FP),Y
   LDY   #$03
   LDA   (DPTR),Y
   LDY   #$2E
   STA   (FP),Y
   LDY   #$04
   LDA   (DPTR),Y
   LDY   #$2F
   STA   (FP),Y
   LDY   #$05
   LDA   (DPTR),Y
   LDY   #$30
   STA   (FP),Y
   LDY   #$06
   LDA   (DPTR),Y
   LDY   #$31
   STA   (FP),Y
   LDY   #$07
   LDA   (DPTR),Y
   LDY   #$32
   STA   (FP),Y
   CMP   #$40
   BNE   .thread@64
   DEY
   LDA   (FP),Y
   CMP   #$62
   BNE   .thread@64
   DEY
   LDA   (FP),Y
   CMP   #$D0
   BNE   .thread@64
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .thread@64
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .thread@64
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .thread@64
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .thread@64
   DEY
   LDA   (FP),Y
   CMP   #$00
   BEQ   .cmp_true@273
.thread@64:
   LDA   #$00
   JMP   .cmp_end@274
.cmp_true@273:
   LDA   #$01
.cmp_end@274:
   LDY   #$33
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@108
   LDA   #$01
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   JMP   .and_end@109
.and_false@108:
   LDA   #$00
   LDY   #$35
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@109:
   LDY   #$35
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lnot_true@276
   LDA   #$00
   JMP   .lnot_end@277
.lnot_true@276:
   LDA   #$01
.lnot_end@277:
   LDY   #$37
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@110
   LDX   #$00
   TXA

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$77
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$4F
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
.if_end@110:
   CLC
   LDA   FP
   ADC   #$6B
   LDY   #$39
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
   BNE   .thread@65
   DEY
   LDA   (FP),Y
   CMP   #$2A
   BNE   .thread@65
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .thread@65
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .thread@65
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .thread@65
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .thread@65
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .thread@65
   DEY
   LDA   (FP),Y
   CMP   #$00
   BEQ   .cmp_true@278
.thread@65:
   LDA   #$00
   JMP   .cmp_end@279
.cmp_true@278:
   LDA   #$01
.cmp_end@279:
   LDY   #$43
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lnot_true@281
   LDA   #$00
   JMP   .lnot_end@282
.lnot_true@281:
   LDA   #$01
.lnot_end@282:
   LDY   #$45
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@111
   LDX   #$00
   TXA

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$77
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$4F
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
.if_end@111:
   CLC
   LDA   FP
   ADC   #$73
   LDY   #$49
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   LDA   #<str@1
   LDY   #$47
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
   INY
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$4A
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$47
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$48
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   JSR   strcmp
   LDY   #$4B
   STA   (FP),Y
   TXA
   INY
   STA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@283
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@283
   LDA   #$00
   JMP   .cmp_end@284
.cmp_true@283:
   LDA   #$01
.cmp_end@284:
   LDY   #$4D
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
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
   ADC   #$77
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
.if_end@112:
   LDX   #$00
   LDA   #$01

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$77
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$4F
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

pass_in_mem_first:
   SUBROUTINE

   ; prologue: 55 arg bytes, 124 local bytes
   SEC
   LDA   SSP
   SBC   #$7E
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$7D
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
   ADC   #$7F
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
.mul_loop@286:
   BCC   .mul_skip@287
   CLC
   ADC   #$08
.mul_skip@287:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@286
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
   BNE   .thread@66
   DEY
   LDA   (FP),Y
   CMP   #$50
   BNE   .thread@66
   DEY
   LDA   (FP),Y
   CMP   #$80
   BNE   .thread@66
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .thread@66
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .thread@66
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .thread@66
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .thread@66
   DEY
   LDA   (FP),Y
   CMP   #$00
   BEQ   .cmp_true@288
.thread@66:
   LDA   #$00
   JMP   .cmp_end@289
.cmp_true@288:
   LDA   #$01
.cmp_end@289:
   LDY   #$0F
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@14
   JMP   .and_false@115
.lb_skip@14:
   CLC
   LDA   FP
   ADC   #$7F
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
.mul_loop@291:
   BCC   .mul_skip@292
   CLC
   ADC   #$08
.mul_skip@292:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@291
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
   BNE   .thread@67
   DEY
   LDA   (FP),Y
   CMP   #$53
   BNE   .thread@67
   DEY
   LDA   (FP),Y
   CMP   #$40
   BNE   .thread@67
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .thread@67
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .thread@67
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .thread@67
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .thread@67
   DEY
   LDA   (FP),Y
   CMP   #$00
   BEQ   .cmp_true@293
.thread@67:
   LDA   #$00
   JMP   .cmp_end@294
.cmp_true@293:
   LDA   #$01
.cmp_end@294:
   LDY   #$1F
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@115
   LDA   #$01
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   JMP   .and_end@116
.and_false@115:
   LDA   #$00
   LDY   #$21
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@116:
   LDY   #$21
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@13
   JMP   .and_false@113
.lb_skip@13:
   CLC
   LDA   FP
   ADC   #$7F
   LDY   #$25
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
.mul_loop@296:
   BCC   .mul_skip@297
   CLC
   ADC   #$08
.mul_skip@297:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@296
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
   BNE   .thread@68
   DEY
   LDA   (FP),Y
   CMP   #$56
   BNE   .thread@68
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .thread@68
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .thread@68
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .thread@68
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .thread@68
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .thread@68
   DEY
   LDA   (FP),Y
   CMP   #$00
   BEQ   .cmp_true@298
.thread@68:
   LDA   #$00
   JMP   .cmp_end@299
.cmp_true@298:
   LDA   #$01
.cmp_end@299:
   LDY   #$31
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@113
   LDA   #$01
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   JMP   .and_end@114
.and_false@113:
   LDA   #$00
   LDY   #$33
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@114:
   LDY   #$33
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lnot_true@301
   LDA   #$00
   JMP   .lnot_end@302
.lnot_true@301:
   LDA   #$01
.lnot_end@302:
   LDY   #$35
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@117
   LDX   #$00
   TXA

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$B5
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$7D
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
.if_end@117:
   CLC
   LDA   FP
   ADC   #$97
   LDY   #$39
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   LDA   #$00
   STA   HARGS
   LDX   #$08
   LSR   HARGS
.mul_loop@303:
   BCC   .mul_skip@304
   CLC
   ADC   #$08
.mul_skip@304:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@303
   LDY   #$38
   STA   (FP),Y
   LDA   HARGS
   DEY
   STA   (FP),Y
   LDY   #$39
   LDA   (FP),Y
   CLC
   LDY   #$37
   ADC   (FP),Y
   LDY   #$3B
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$38
   ADC   (FP),Y
   LDY   #$3C
   STA   (FP),Y
   DEY
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
   LDY   #$02
   LDA   (DPTR),Y
   LDY   #$3F
   STA   (FP),Y
   LDY   #$03
   LDA   (DPTR),Y
   LDY   #$40
   STA   (FP),Y
   LDY   #$04
   LDA   (DPTR),Y
   LDY   #$41
   STA   (FP),Y
   LDY   #$05
   LDA   (DPTR),Y
   LDY   #$42
   STA   (FP),Y
   LDY   #$06
   LDA   (DPTR),Y
   LDY   #$43
   STA   (FP),Y
   LDY   #$07
   LDA   (DPTR),Y
   LDY   #$44
   STA   (FP),Y
   CMP   #$00
   BNE   .thread@69
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .thread@69
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .thread@69
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .thread@69
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .thread@69
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .thread@69
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .thread@69
   DEY
   LDA   (FP),Y
   CMP   #$00
   BEQ   .cmp_true@305
.thread@69:
   LDA   #$00
   JMP   .cmp_end@306
.cmp_true@305:
   LDA   #$01
.cmp_end@306:
   LDY   #$45
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@12
   JMP   .and_false@118
.lb_skip@12:
   CLC
   LDA   FP
   ADC   #$97
   LDY   #$49
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
.mul_loop@308:
   BCC   .mul_skip@309
   CLC
   ADC   #$08
.mul_skip@309:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@308
   LDY   #$48
   STA   (FP),Y
   LDA   HARGS
   DEY
   STA   (FP),Y
   LDY   #$49
   LDA   (FP),Y
   CLC
   LDY   #$47
   ADC   (FP),Y
   LDY   #$4B
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$48
   ADC   (FP),Y
   LDY   #$4C
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$4D
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$4E
   STA   (FP),Y
   LDY   #$02
   LDA   (DPTR),Y
   LDY   #$4F
   STA   (FP),Y
   LDY   #$03
   LDA   (DPTR),Y
   LDY   #$50
   STA   (FP),Y
   LDY   #$04
   LDA   (DPTR),Y
   LDY   #$51
   STA   (FP),Y
   LDY   #$05
   LDA   (DPTR),Y
   LDY   #$52
   STA   (FP),Y
   LDY   #$06
   LDA   (DPTR),Y
   LDY   #$53
   STA   (FP),Y
   LDY   #$07
   LDA   (DPTR),Y
   LDY   #$54
   STA   (FP),Y
   CMP   #$40
   BNE   .thread@70
   DEY
   LDA   (FP),Y
   CMP   #$62
   BNE   .thread@70
   DEY
   LDA   (FP),Y
   CMP   #$D0
   BNE   .thread@70
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .thread@70
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .thread@70
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .thread@70
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .thread@70
   DEY
   LDA   (FP),Y
   CMP   #$00
   BEQ   .cmp_true@310
.thread@70:
   LDA   #$00
   JMP   .cmp_end@311
.cmp_true@310:
   LDA   #$01
.cmp_end@311:
   LDY   #$55
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@118
   LDA   #$01
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   JMP   .and_end@119
.and_false@118:
   LDA   #$00
   LDY   #$57
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@119:
   LDY   #$57
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lnot_true@313
   LDA   #$00
   JMP   .lnot_end@314
.lnot_true@313:
   LDA   #$01
.lnot_end@314:
   LDY   #$59
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@120
   LDX   #$00
   TXA

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$B5
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$7D
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
.if_end@120:
   CLC
   LDA   FP
   ADC   #$A7
   LDY   #$5D
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   LDA   #<str@2
   LDY   #$5B
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
   LDY   #$5E
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$5B
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$5C
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   JSR   strcmp
   LDY   #$5F
   STA   (FP),Y
   TXA
   INY
   STA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@315
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@315
   LDA   #$00
   JMP   .cmp_end@316
.cmp_true@315:
   LDA   #$01
.cmp_end@316:
   LDY   #$61
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@121
   LDX   #$00
   TXA

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$B5
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$7D
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
.if_end@121:
   CLC
   LDA   FP
   ADC   #$B2
   LDY   #$63
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
   LDY   #$65
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$66
   STA   (FP),Y
   SEC
   LDA   #$00
   SBC   #$18
   INY
   STA   (FP),Y
   LDA   #$00
   SBC   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$69
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$6A
   STA   (FP),Y
   LDY   #$66
   LDA   (FP),Y
   LDY   #$6A
   CMP   (FP),Y
   BNE   .thread@71
   LDY   #$65
   LDA   (FP),Y
   LDY   #$69
   CMP   (FP),Y
   BEQ   .cmp_true@318
.thread@71:
   LDA   #$00
   JMP   .cmp_end@319
.cmp_true@318:
   LDA   #$01
.cmp_end@319:
   LDY   #$6B
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@11
   JMP   .and_false@122
.lb_skip@11:
   CLC
   LDA   FP
   ADC   #$B2
   LDY   #$6D
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$02
   LDY   #$6F
   STA   (FP),Y
   DEY
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
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$71
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$72
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$73
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$74
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@321
   LDA   #$00
   JMP   .sx_done@322
.sx_neg@321:
   LDA   #$FF
.sx_done@322:
   LDY   #$75
   STA   (FP),Y
   INY
   STA   (FP),Y
   CMP   #$07
   BNE   .thread@72
   CMP   #$5B
   BNE   .thread@72
   LDY   #$74
   LDA   (FP),Y
   CMP   #$CD
   BNE   .thread@72
   DEY
   LDA   (FP),Y
   CMP   #$15
   BEQ   .cmp_true@323
.thread@72:
   LDA   #$00
   JMP   .cmp_end@324
.cmp_true@323:
   LDA   #$01
.cmp_end@324:
   LDY   #$77
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@122
   LDA   #$01
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   JMP   .and_end@123
.and_false@122:
   LDA   #$00
   LDY   #$79
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@123:
   LDY   #$79
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lnot_true@326
   LDA   #$00
   JMP   .lnot_end@327
.lnot_true@326:
   LDA   #$01
.lnot_end@327:
   LDY   #$7B
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@124
   LDX   #$00
   TXA

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$B5
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
.if_end@124:
   LDX   #$00
   LDA   #$01

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$B5
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$7D
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
   DC.B  $2B, $5F, $29, $28, $2A, $26, $5E, $25, $24, $23, $00

str@1:
   DC.B  $62, $6F, $6F, $70, $00

str@2:
   DC.B  $2B, $5F, $29, $28, $2A, $26, $5E, $25, $24, $23, $00
