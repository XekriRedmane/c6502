SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24

test_one_double:
   SUBROUTINE

   ; prologue: 8 arg bytes, 42 local bytes
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

   CLC
   LDA   FP
   ADC   #$2D
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
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$14
   INY
   STA   (FP),Y
   LDA   #$E4
   INY
   STA   (FP),Y
   LDA   #$41
   INY
   STA   (FP),Y
   EOR   #$80
   INY
   STA   (FP),Y
   LDY   #$0A
   LDA   (FP),Y
   LDY   #$12
   CMP   (FP),Y
   BNE   .thread@0
   LDY   #$09
   LDA   (FP),Y
   LDY   #$11
   CMP   (FP),Y
   BNE   .thread@0
   LDY   #$08
   LDA   (FP),Y
   LDY   #$10
   CMP   (FP),Y
   BNE   .thread@0
   LDY   #$07
   LDA   (FP),Y
   LDY   #$0F
   CMP   (FP),Y
   BNE   .thread@0
   LDY   #$06
   LDA   (FP),Y
   LDY   #$0E
   CMP   (FP),Y
   BNE   .thread@0
   LDY   #$05
   LDA   (FP),Y
   LDY   #$0D
   CMP   (FP),Y
   BNE   .thread@0
   LDY   #$04
   LDA   (FP),Y
   LDY   #$0C
   CMP   (FP),Y
   BNE   .thread@0
   LDY   #$03
   LDA   (FP),Y
   LDY   #$0B
   CMP   (FP),Y
   BEQ   .cmp_true@0
.thread@0:
   LDA   #$00
   JMP   .cmp_end@1
.cmp_true@0:
   LDA   #$01
.cmp_end@1:
   LDY   #$13
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@0
   JMP   .and_false@0
.lb_skip@0:
   CLC
   LDA   FP
   ADC   #$2D
   LDY   #$15
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
   LDA   #$14
   INY
   STA   (FP),Y
   LDA   #$E4
   INY
   STA   (FP),Y
   LDA   #$41
   INY
   STA   (FP),Y
   EOR   #$80
   INY
   STA   (FP),Y
   LDY   #$1E
   LDA   (FP),Y
   LDY   #$26
   CMP   (FP),Y
   BNE   .thread@1
   LDY   #$1D
   LDA   (FP),Y
   LDY   #$25
   CMP   (FP),Y
   BNE   .thread@1
   LDY   #$1C
   LDA   (FP),Y
   LDY   #$24
   CMP   (FP),Y
   BNE   .thread@1
   LDY   #$1B
   LDA   (FP),Y
   LDY   #$23
   CMP   (FP),Y
   BNE   .thread@1
   LDY   #$1A
   LDA   (FP),Y
   LDY   #$22
   CMP   (FP),Y
   BNE   .thread@1
   LDY   #$19
   LDA   (FP),Y
   LDY   #$21
   CMP   (FP),Y
   BNE   .thread@1
   LDY   #$18
   LDA   (FP),Y
   LDY   #$20
   CMP   (FP),Y
   BNE   .thread@1
   LDY   #$17
   LDA   (FP),Y
   LDY   #$1F
   CMP   (FP),Y
   BEQ   .cmp_true@3
.thread@1:
   LDA   #$00
   JMP   .cmp_end@4
.cmp_true@3:
   LDA   #$01
.cmp_end@4:
   LDY   #$27
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
   LDY   #$29
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@1:
   TAX
   LDY   #$29
   LDA   (FP),Y

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$34
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

test_has_union_with_double:
   SUBROUTINE

   ; prologue: 8 arg bytes, 26 local bytes
   SEC
   LDA   SSP
   SBC   #$1C
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$1B
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
   ADC   #$1D
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
   CMP   #$49
   BNE   .thread@2
   DEY
   LDA   (FP),Y
   CMP   #$E5
   BNE   .thread@2
   DEY
   LDA   (FP),Y
   CMP   #$A6
   BNE   .thread@2
   DEY
   LDA   (FP),Y
   CMP   #$28
   BNE   .thread@2
   DEY
   LDA   (FP),Y
   CMP   #$DE
   BNE   .thread@2
   DEY
   LDA   (FP),Y
   CMP   #$96
   BNE   .thread@2
   DEY
   LDA   (FP),Y
   CMP   #$EE
   BNE   .thread@2
   DEY
   LDA   (FP),Y
   CMP   #$1D
   BEQ   .cmp_true@6
.thread@2:
   LDA   #$00
   JMP   .cmp_end@7
.cmp_true@6:
   LDA   #$01
.cmp_end@7:
   LDY   #$0B
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@1
   JMP   .and_false@2
.lb_skip@1:
   CLC
   LDA   FP
   ADC   #$1D
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
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$0F
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$10
   STA   (FP),Y
   LDY   #$02
   LDA   (DPTR),Y
   LDY   #$11
   STA   (FP),Y
   LDY   #$03
   LDA   (DPTR),Y
   LDY   #$12
   STA   (FP),Y
   LDY   #$04
   LDA   (DPTR),Y
   LDY   #$13
   STA   (FP),Y
   LDY   #$05
   LDA   (DPTR),Y
   LDY   #$14
   STA   (FP),Y
   LDY   #$06
   LDA   (DPTR),Y
   LDY   #$15
   STA   (FP),Y
   LDY   #$07
   LDA   (DPTR),Y
   LDY   #$16
   STA   (FP),Y
   CMP   #$49
   BNE   .thread@3
   DEY
   LDA   (FP),Y
   CMP   #$E5
   BNE   .thread@3
   DEY
   LDA   (FP),Y
   CMP   #$A6
   BNE   .thread@3
   DEY
   LDA   (FP),Y
   CMP   #$28
   BNE   .thread@3
   DEY
   LDA   (FP),Y
   CMP   #$DE
   BNE   .thread@3
   DEY
   LDA   (FP),Y
   CMP   #$96
   BNE   .thread@3
   DEY
   LDA   (FP),Y
   CMP   #$EE
   BNE   .thread@3
   DEY
   LDA   (FP),Y
   CMP   #$1D
   BEQ   .cmp_true@9
.thread@3:
   LDA   #$00
   JMP   .cmp_end@10
.cmp_true@9:
   LDA   #$01
.cmp_end@10:
   LDY   #$17
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
   LDY   #$19
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@3:
   TAX
   LDY   #$19
   LDA   (FP),Y

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$24
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$1B
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

test_has_struct_with_double:
   SUBROUTINE

   ; prologue: 8 arg bytes, 44 local bytes
   SEC
   LDA   SSP
   SBC   #$2E
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$2D
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
   ADC   #$2F
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
   CMP   #$49
   BNE   .thread@4
   DEY
   LDA   (FP),Y
   CMP   #$E5
   BNE   .thread@4
   DEY
   LDA   (FP),Y
   CMP   #$A6
   BNE   .thread@4
   DEY
   LDA   (FP),Y
   CMP   #$28
   BNE   .thread@4
   DEY
   LDA   (FP),Y
   CMP   #$DE
   BNE   .thread@4
   DEY
   LDA   (FP),Y
   CMP   #$96
   BNE   .thread@4
   DEY
   LDA   (FP),Y
   CMP   #$EE
   BNE   .thread@4
   DEY
   LDA   (FP),Y
   CMP   #$1D
   BEQ   .cmp_true@12
.thread@4:
   LDA   #$00
   JMP   .cmp_end@13
.cmp_true@12:
   LDA   #$01
.cmp_end@13:
   LDY   #$0B
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@3
   JMP   .and_false@6
.lb_skip@3:
   CLC
   LDA   FP
   ADC   #$2F
   LDY   #$0F
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   LDA   #$00
   STA   HARGS
   LDX   #$08
   LSR   HARGS
.mul_loop@15:
   BCC   .mul_skip@16
   CLC
   ADC   #$08
.mul_skip@16:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@15
   LDY   #$0E
   STA   (FP),Y
   LDA   HARGS
   DEY
   STA   (FP),Y
   LDY   #$0F
   LDA   (FP),Y
   CLC
   LDY   #$0D
   ADC   (FP),Y
   LDY   #$11
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$0E
   ADC   (FP),Y
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
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$14
   STA   (FP),Y
   LDY   #$02
   LDA   (DPTR),Y
   LDY   #$15
   STA   (FP),Y
   LDY   #$03
   LDA   (DPTR),Y
   LDY   #$16
   STA   (FP),Y
   LDY   #$04
   LDA   (DPTR),Y
   LDY   #$17
   STA   (FP),Y
   LDY   #$05
   LDA   (DPTR),Y
   LDY   #$18
   STA   (FP),Y
   LDY   #$06
   LDA   (DPTR),Y
   LDY   #$19
   STA   (FP),Y
   LDY   #$07
   LDA   (DPTR),Y
   LDY   #$1A
   STA   (FP),Y
   CMP   #$49
   BNE   .thread@5
   DEY
   LDA   (FP),Y
   CMP   #$E5
   BNE   .thread@5
   DEY
   LDA   (FP),Y
   CMP   #$A6
   BNE   .thread@5
   DEY
   LDA   (FP),Y
   CMP   #$28
   BNE   .thread@5
   DEY
   LDA   (FP),Y
   CMP   #$DE
   BNE   .thread@5
   DEY
   LDA   (FP),Y
   CMP   #$96
   BNE   .thread@5
   DEY
   LDA   (FP),Y
   CMP   #$EE
   BNE   .thread@5
   DEY
   LDA   (FP),Y
   CMP   #$1D
   BEQ   .cmp_true@17
.thread@5:
   LDA   #$00
   JMP   .cmp_end@18
.cmp_true@17:
   LDA   #$01
.cmp_end@18:
   LDY   #$1B
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
   LDY   #$1D
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@7:
   LDY   #$1D
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@2
   JMP   .and_false@4
.lb_skip@2:
   CLC
   LDA   FP
   ADC   #$2F
   LDY   #$1F
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
   LDY   #$21
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$22
   STA   (FP),Y
   LDY   #$02
   LDA   (DPTR),Y
   LDY   #$23
   STA   (FP),Y
   LDY   #$03
   LDA   (DPTR),Y
   LDY   #$24
   STA   (FP),Y
   LDY   #$04
   LDA   (DPTR),Y
   LDY   #$25
   STA   (FP),Y
   LDY   #$05
   LDA   (DPTR),Y
   LDY   #$26
   STA   (FP),Y
   LDY   #$06
   LDA   (DPTR),Y
   LDY   #$27
   STA   (FP),Y
   LDY   #$07
   LDA   (DPTR),Y
   LDY   #$28
   STA   (FP),Y
   CMP   #$49
   BNE   .thread@6
   DEY
   LDA   (FP),Y
   CMP   #$E5
   BNE   .thread@6
   DEY
   LDA   (FP),Y
   CMP   #$A6
   BNE   .thread@6
   DEY
   LDA   (FP),Y
   CMP   #$28
   BNE   .thread@6
   DEY
   LDA   (FP),Y
   CMP   #$DE
   BNE   .thread@6
   DEY
   LDA   (FP),Y
   CMP   #$96
   BNE   .thread@6
   DEY
   LDA   (FP),Y
   CMP   #$EE
   BNE   .thread@6
   DEY
   LDA   (FP),Y
   CMP   #$1D
   BEQ   .cmp_true@20
.thread@6:
   LDA   #$00
   JMP   .cmp_end@21
.cmp_true@20:
   LDA   #$01
.cmp_end@21:
   LDY   #$29
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
   LDY   #$2B
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@5:
   TAX
   LDY   #$2B
   LDA   (FP),Y

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$36
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$2D
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

test_one_int:
   SUBROUTINE

   ; prologue: 8 arg bytes, 29 local bytes
   SEC
   LDA   SSP
   SBC   #$1F
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$1E
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
   ADC   #$20
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
   LDA   #$54
   INY
   STA   (FP),Y
   LDA   #$40
   EOR   #$80
   INY
   STA   (FP),Y
   LDY   #$0A
   LDA   (FP),Y
   LDY   #$12
   CMP   (FP),Y
   BNE   .thread@7
   LDY   #$09
   LDA   (FP),Y
   LDY   #$11
   CMP   (FP),Y
   BNE   .thread@7
   LDY   #$08
   LDA   (FP),Y
   LDY   #$10
   CMP   (FP),Y
   BNE   .thread@7
   LDY   #$07
   LDA   (FP),Y
   LDY   #$0F
   CMP   (FP),Y
   BNE   .thread@7
   LDY   #$06
   LDA   (FP),Y
   LDY   #$0E
   CMP   (FP),Y
   BNE   .thread@7
   LDY   #$05
   LDA   (FP),Y
   LDY   #$0D
   CMP   (FP),Y
   BNE   .thread@7
   LDY   #$04
   LDA   (FP),Y
   LDY   #$0C
   CMP   (FP),Y
   BNE   .thread@7
   LDY   #$03
   LDA   (FP),Y
   LDY   #$0B
   CMP   (FP),Y
   BEQ   .cmp_true@23
.thread@7:
   LDA   #$00
   JMP   .cmp_end@24
.cmp_true@23:
   LDA   #$01
.cmp_end@24:
   LDY   #$13
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@8
   CLC
   LDA   FP
   ADC   #$20
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
   LDY   #$17
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   BEQ   .cmp_true@26
   LDA   #$00
   JMP   .cmp_end@27
.cmp_true@26:
   LDA   #$01
.cmp_end@27:
   LDY   #$1A
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
   LDY   #$1C
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@9:
   TAX
   LDY   #$1C
   LDA   (FP),Y

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$27
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$1E
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

test_one_int_nested:
   SUBROUTINE

   ; prologue: 8 arg bytes, 49 local bytes
   SEC
   LDA   SSP
   SBC   #$33
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$32
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
   ADC   #$34
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
   CMP   #$4B
   BNE   .thread@8
   DEY
   LDA   (FP),Y
   CMP   #$B1
   BNE   .thread@8
   DEY
   LDA   (FP),Y
   CMP   #$F1
   BNE   .thread@8
   DEY
   LDA   (FP),Y
   CMP   #$D0
   BNE   .thread@8
   DEY
   LDA   (FP),Y
   CMP   #$78
   BNE   .thread@8
   DEY
   LDA   (FP),Y
   CMP   #$F1
   BNE   .thread@8
   DEY
   LDA   (FP),Y
   CMP   #$DD
   BNE   .thread@8
   DEY
   LDA   (FP),Y
   CMP   #$6D
   BEQ   .cmp_true@28
.thread@8:
   LDA   #$00
   JMP   .cmp_end@29
.cmp_true@28:
   LDA   #$01
.cmp_end@29:
   LDY   #$0B
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@14
   CLC
   LDA   FP
   ADC   #$34
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
   LDY   #$0F
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CMP   #$6D
   BEQ   .cmp_true@31
   LDA   #$00
   JMP   .cmp_end@32
.cmp_true@31:
   LDA   #$01
.cmp_end@32:
   LDY   #$12
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
   LDY   #$14
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@15:
   LDY   #$14
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@5
   JMP   .and_false@12
.lb_skip@5:
   CLC
   LDA   FP
   ADC   #$34
   LDY   #$16
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
   LDY   #$18
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$19
   STA   (FP),Y
   LDY   #$02
   LDA   (DPTR),Y
   LDY   #$1A
   STA   (FP),Y
   LDY   #$03
   LDA   (DPTR),Y
   LDY   #$1B
   STA   (FP),Y
   LDY   #$04
   LDA   (DPTR),Y
   LDY   #$1C
   STA   (FP),Y
   LDY   #$05
   LDA   (DPTR),Y
   LDY   #$1D
   STA   (FP),Y
   LDY   #$06
   LDA   (DPTR),Y
   LDY   #$1E
   STA   (FP),Y
   LDY   #$07
   LDA   (DPTR),Y
   LDY   #$1F
   STA   (FP),Y
   CMP   #$4B
   BNE   .thread@9
   DEY
   LDA   (FP),Y
   CMP   #$B1
   BNE   .thread@9
   DEY
   LDA   (FP),Y
   CMP   #$F1
   BNE   .thread@9
   DEY
   LDA   (FP),Y
   CMP   #$D0
   BNE   .thread@9
   DEY
   LDA   (FP),Y
   CMP   #$78
   BNE   .thread@9
   DEY
   LDA   (FP),Y
   CMP   #$F1
   BNE   .thread@9
   DEY
   LDA   (FP),Y
   CMP   #$DD
   BNE   .thread@9
   DEY
   LDA   (FP),Y
   CMP   #$6D
   BEQ   .cmp_true@33
.thread@9:
   LDA   #$00
   JMP   .cmp_end@34
.cmp_true@33:
   LDA   #$01
.cmp_end@34:
   LDY   #$20
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@12
   LDA   #$01
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   JMP   .and_end@13
.and_false@12:
   LDA   #$00
   LDY   #$22
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@13:
   LDY   #$22
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@4
   JMP   .and_false@10
.lb_skip@4:
   CLC
   LDA   FP
   ADC   #$34
   LDY   #$24
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
   LDY   #$26
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$27
   STA   (FP),Y
   LDY   #$02
   LDA   (DPTR),Y
   LDY   #$28
   STA   (FP),Y
   LDY   #$03
   LDA   (DPTR),Y
   LDY   #$29
   STA   (FP),Y
   LDY   #$04
   LDA   (DPTR),Y
   LDY   #$2A
   STA   (FP),Y
   LDY   #$05
   LDA   (DPTR),Y
   LDY   #$2B
   STA   (FP),Y
   LDY   #$06
   LDA   (DPTR),Y
   LDY   #$2C
   STA   (FP),Y
   LDY   #$07
   LDA   (DPTR),Y
   LDY   #$2D
   STA   (FP),Y
   CMP   #$4B
   BNE   .thread@10
   DEY
   LDA   (FP),Y
   CMP   #$B1
   BNE   .thread@10
   DEY
   LDA   (FP),Y
   CMP   #$F1
   BNE   .thread@10
   DEY
   LDA   (FP),Y
   CMP   #$D0
   BNE   .thread@10
   DEY
   LDA   (FP),Y
   CMP   #$78
   BNE   .thread@10
   DEY
   LDA   (FP),Y
   CMP   #$F1
   BNE   .thread@10
   DEY
   LDA   (FP),Y
   CMP   #$DD
   BNE   .thread@10
   DEY
   LDA   (FP),Y
   CMP   #$6D
   BEQ   .cmp_true@36
.thread@10:
   LDA   #$00
   JMP   .cmp_end@37
.cmp_true@36:
   LDA   #$01
.cmp_end@37:
   LDY   #$2E
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@10
   LDA   #$01
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   JMP   .and_end@11
.and_false@10:
   LDA   #$00
   LDY   #$30
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@11:
   TAX
   LDY   #$30
   LDA   (FP),Y

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$3B
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$32
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

test_char_int_mixed:
   SUBROUTINE

   ; prologue: 7 arg bytes, 20 local bytes
   SEC
   LDA   SSP
   SBC   #$16
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$15
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
   ADC   #$17
   LDY   #$03
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   LDA   #<str@0
   LDY   #$01
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
   LDY   #$04
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   DEY
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   DEY
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   JSR   strcmp
   LDY   #$05
   STA   (FP),Y
   TXA
   INY
   STA   (FP),Y
   CMP   #$00
   BNE   .thread@11
   DEY
   LDA   (FP),Y
   CMP   #$00
   BEQ   .cmp_true@39
.thread@11:
   LDA   #$00
   JMP   .cmp_end@40
.cmp_true@39:
   LDA   #$01
.cmp_end@40:
   LDY   #$07
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@6
   JMP   .and_false@16
.lb_skip@6:
   CLC
   LDA   FP
   ADC   #$17
   LDY   #$09
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
   LDY   #$0B
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
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
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   CMP   #$5A
   BNE   .thread@12
   CMP   #$59
   BNE   .thread@12
   LDY   #$0E
   LDA   (FP),Y
   CMP   #$58
   BNE   .thread@12
   DEY
   LDA   (FP),Y
   CMP   #$57
   BEQ   .cmp_true@42
.thread@12:
   LDA   #$00
   JMP   .cmp_end@43
.cmp_true@42:
   LDA   #$01
.cmp_end@43:
   LDY   #$11
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@16
   LDA   #$01
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   JMP   .and_end@17
.and_false@16:
   LDA   #$00
   LDY   #$13
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@17:
   TAX
   LDY   #$13
   LDA   (FP),Y

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$1D
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$15
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

test_has_union:
   SUBROUTINE

   ; prologue: 4 arg bytes, 23 local bytes
   SEC
   LDA   SSP
   SBC   #$19
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$18
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
   ADC   #$1A
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
   DEY
   LDA   (FP),Y
   LDY   #$05
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$06
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   CMP   #$FF
   BNE   .thread@13
   LDY   #$06
   LDA   (FP),Y
   CMP   #$CF
   BNE   .thread@13
   DEY
   LDA   (FP),Y
   CMP   #$C7
   BEQ   .cmp_true@45
.thread@13:
   LDA   #$00
   JMP   .cmp_end@46
.cmp_true@45:
   LDA   #$01
.cmp_end@46:
   LDY   #$09
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@7
   JMP   .and_false@18
.lb_skip@7:
   CLC
   LDA   FP
   ADC   #$1A
   LDY   #$0B
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   DEY
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
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$0F
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   SEC
   SBC   #$3C
   INY
   STA   (FP),Y
   LDA   #$00
   SBC   #$00
   INY
   STA   (FP),Y
   LDY   #$11
   LDA   (FP),Y
   LDY   #$13
   CMP   (FP),Y
   BNE   .thread@14
   LDY   #$10
   LDA   (FP),Y
   LDY   #$12
   CMP   (FP),Y
   BEQ   .cmp_true@48
.thread@14:
   LDA   #$00
   JMP   .cmp_end@49
.cmp_true@48:
   LDA   #$01
.cmp_end@49:
   LDY   #$14
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
   LDY   #$16
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@19:
   TAX
   LDY   #$16
   LDA   (FP),Y

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$1D
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$18
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

test_has_struct_with_ints:
   SUBROUTINE

   ; prologue: 8 arg bytes, 23 local bytes
   SEC
   LDA   SSP
   SBC   #$19
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$18
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
   ADC   #$1A
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
   DEY
   LDA   (FP),Y
   LDY   #$05
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$06
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   CMP   #$FF
   BNE   .thread@15
   LDY   #$06
   LDA   (FP),Y
   CMP   #$CF
   BNE   .thread@15
   DEY
   LDA   (FP),Y
   CMP   #$C7
   BEQ   .cmp_true@51
.thread@15:
   LDA   #$00
   JMP   .cmp_end@52
.cmp_true@51:
   LDA   #$01
.cmp_end@52:
   LDY   #$09
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@8
   JMP   .and_false@20
.lb_skip@8:
   CLC
   LDA   FP
   ADC   #$1A
   LDY   #$0B
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   DEY
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
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$0F
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   SEC
   SBC   #$3C
   INY
   STA   (FP),Y
   LDA   #$00
   SBC   #$00
   INY
   STA   (FP),Y
   LDY   #$11
   LDA   (FP),Y
   LDY   #$13
   CMP   (FP),Y
   BNE   .thread@16
   LDY   #$10
   LDA   (FP),Y
   LDY   #$12
   CMP   (FP),Y
   BEQ   .cmp_true@54
.thread@16:
   LDA   #$00
   JMP   .cmp_end@55
.cmp_true@54:
   LDA   #$01
.cmp_end@55:
   LDY   #$14
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
   LDY   #$16
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@21:
   TAX
   LDY   #$16
   LDA   (FP),Y

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$21
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$18
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

test_two_doubles:
   SUBROUTINE

   ; prologue: 16 arg bytes, 48 local bytes
   SEC
   LDA   SSP
   SBC   #$32
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$31
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
   ADC   #$33
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
.mul_loop@57:
   BCC   .mul_skip@58
   CLC
   ADC   #$08
.mul_skip@58:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@57
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
   BNE   .thread@17
   DEY
   LDA   (FP),Y
   CMP   #$24
   BNE   .thread@17
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .thread@17
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .thread@17
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .thread@17
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .thread@17
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .thread@17
   DEY
   LDA   (FP),Y
   CMP   #$00
   BEQ   .cmp_true@59
.thread@17:
   LDA   #$00
   JMP   .cmp_end@60
.cmp_true@59:
   LDA   #$01
.cmp_end@60:
   LDY   #$0F
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@10
   JMP   .and_false@24
.lb_skip@10:
   CLC
   LDA   FP
   ADC   #$33
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
.mul_loop@62:
   BCC   .mul_skip@63
   CLC
   ADC   #$08
.mul_skip@63:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@62
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
   BNE   .thread@18
   DEY
   LDA   (FP),Y
   CMP   #$26
   BNE   .thread@18
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .thread@18
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .thread@18
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .thread@18
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .thread@18
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .thread@18
   DEY
   LDA   (FP),Y
   CMP   #$00
   BEQ   .cmp_true@64
.thread@18:
   LDA   #$00
   JMP   .cmp_end@65
.cmp_true@64:
   LDA   #$01
.cmp_end@65:
   LDY   #$1F
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
   LDY   #$21
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@25:
   LDY   #$21
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@9
   JMP   .and_false@22
.lb_skip@9:
   CLC
   LDA   FP
   ADC   #$33
   LDY   #$23
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
   LDY   #$25
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$26
   STA   (FP),Y
   LDY   #$02
   LDA   (DPTR),Y
   LDY   #$27
   STA   (FP),Y
   LDY   #$03
   LDA   (DPTR),Y
   LDY   #$28
   STA   (FP),Y
   LDY   #$04
   LDA   (DPTR),Y
   LDY   #$29
   STA   (FP),Y
   LDY   #$05
   LDA   (DPTR),Y
   LDY   #$2A
   STA   (FP),Y
   LDY   #$06
   LDA   (DPTR),Y
   LDY   #$2B
   STA   (FP),Y
   LDY   #$07
   LDA   (DPTR),Y
   LDY   #$2C
   STA   (FP),Y
   CMP   #$40
   BNE   .thread@19
   DEY
   LDA   (FP),Y
   CMP   #$24
   BNE   .thread@19
   DEY
   LDA   (FP),Y
   BNE   .thread@19
   DEY
   LDA   (FP),Y
   BNE   .thread@19
   DEY
   LDA   (FP),Y
   BNE   .thread@19
   DEY
   LDA   (FP),Y
   BNE   .thread@19
   DEY
   LDA   (FP),Y
   BNE   .thread@19
   DEY
   LDA   (FP),Y
   BEQ   .cmp_true@67
.thread@19:
   LDA   #$00
   JMP   .cmp_end@68
.cmp_true@67:
   LDA   #$01
.cmp_end@68:
   LDY   #$2D
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
   LDY   #$2F
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@23:
   TAX
   LDY   #$2F
   LDA   (FP),Y

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$42
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$31
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

test_has_xmm_union:
   SUBROUTINE

   ; prologue: 16 arg bytes, 76 local bytes
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

   CLC
   LDA   FP
   ADC   #$4F
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
   BNE   .thread@20
   DEY
   LDA   (FP),Y
   CMP   #$24
   BNE   .thread@20
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .thread@20
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .thread@20
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .thread@20
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .thread@20
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .thread@20
   DEY
   LDA   (FP),Y
   CMP   #$00
   BEQ   .cmp_true@70
.thread@20:
   LDA   #$00
   JMP   .cmp_end@71
.cmp_true@70:
   LDA   #$01
.cmp_end@71:
   LDY   #$0B
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@14
   JMP   .and_false@32
.lb_skip@14:
   CLC
   LDA   FP
   ADC   #$4F
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
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$0F
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$10
   STA   (FP),Y
   LDY   #$02
   LDA   (DPTR),Y
   LDY   #$11
   STA   (FP),Y
   LDY   #$03
   LDA   (DPTR),Y
   LDY   #$12
   STA   (FP),Y
   LDY   #$04
   LDA   (DPTR),Y
   LDY   #$13
   STA   (FP),Y
   LDY   #$05
   LDA   (DPTR),Y
   LDY   #$14
   STA   (FP),Y
   LDY   #$06
   LDA   (DPTR),Y
   LDY   #$15
   STA   (FP),Y
   LDY   #$07
   LDA   (DPTR),Y
   LDY   #$16
   STA   (FP),Y
   CMP   #$40
   BNE   .thread@21
   DEY
   LDA   (FP),Y
   CMP   #$24
   BNE   .thread@21
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .thread@21
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .thread@21
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .thread@21
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .thread@21
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .thread@21
   DEY
   LDA   (FP),Y
   CMP   #$00
   BEQ   .cmp_true@73
.thread@21:
   LDA   #$00
   JMP   .cmp_end@74
.cmp_true@73:
   LDA   #$01
.cmp_end@74:
   LDY   #$17
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
   LDY   #$19
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@33:
   LDY   #$19
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@13
   JMP   .and_false@30
.lb_skip@13:
   CLC
   LDA   FP
   ADC   #$4F
   LDY   #$1B
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
   LDY   #$1D
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$1E
   STA   (FP),Y
   LDY   #$02
   LDA   (DPTR),Y
   LDY   #$1F
   STA   (FP),Y
   LDY   #$03
   LDA   (DPTR),Y
   LDY   #$20
   STA   (FP),Y
   LDY   #$04
   LDA   (DPTR),Y
   LDY   #$21
   STA   (FP),Y
   LDY   #$05
   LDA   (DPTR),Y
   LDY   #$22
   STA   (FP),Y
   LDY   #$06
   LDA   (DPTR),Y
   LDY   #$23
   STA   (FP),Y
   LDY   #$07
   LDA   (DPTR),Y
   LDY   #$24
   STA   (FP),Y
   CMP   #$40
   BNE   .thread@22
   DEY
   LDA   (FP),Y
   CMP   #$24
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
   BEQ   .cmp_true@76
.thread@22:
   LDA   #$00
   JMP   .cmp_end@77
.cmp_true@76:
   LDA   #$01
.cmp_end@77:
   LDY   #$25
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@30
   LDA   #$01
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   JMP   .and_end@31
.and_false@30:
   LDA   #$00
   LDY   #$27
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@31:
   LDY   #$27
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@12
   JMP   .and_false@28
.lb_skip@12:
   CLC
   LDA   FP
   ADC   #$4F
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
.mul_loop@79:
   BCC   .mul_skip@80
   CLC
   ADC   #$08
.mul_skip@80:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@79
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
   CMP   #$40
   BNE   .thread@23
   DEY
   LDA   (FP),Y
   CMP   #$24
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
   BEQ   .cmp_true@81
.thread@23:
   LDA   #$00
   JMP   .cmp_end@82
.cmp_true@81:
   LDA   #$01
.cmp_end@82:
   LDY   #$37
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@28
   LDA   #$01
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   JMP   .and_end@29
.and_false@28:
   LDA   #$00
   LDY   #$39
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@29:
   LDY   #$39
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@11
   JMP   .and_false@26
.lb_skip@11:
   CLC
   LDA   FP
   ADC   #$4F
   LDY   #$3D
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
.mul_loop@84:
   BCC   .mul_skip@85
   CLC
   ADC   #$08
.mul_skip@85:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@84
   LDY   #$3C
   STA   (FP),Y
   LDA   HARGS
   DEY
   STA   (FP),Y
   LDY   #$3D
   LDA   (FP),Y
   CLC
   LDY   #$3B
   ADC   (FP),Y
   LDY   #$3F
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$3C
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
   BNE   .thread@24
   DEY
   LDA   (FP),Y
   CMP   #$26
   BNE   .thread@24
   DEY
   LDA   (FP),Y
   BNE   .thread@24
   DEY
   LDA   (FP),Y
   BNE   .thread@24
   DEY
   LDA   (FP),Y
   BNE   .thread@24
   DEY
   LDA   (FP),Y
   BNE   .thread@24
   DEY
   LDA   (FP),Y
   BNE   .thread@24
   DEY
   LDA   (FP),Y
   BEQ   .cmp_true@86
.thread@24:
   LDA   #$00
   JMP   .cmp_end@87
.cmp_true@86:
   LDA   #$01
.cmp_end@87:
   LDY   #$49
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
   LDY   #$4B
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@27:
   TAX
   LDY   #$4B
   LDA   (FP),Y

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$5E
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

test_dbl_struct:
   SUBROUTINE

   ; prologue: 16 arg bytes, 58 local bytes
   SEC
   LDA   SSP
   SBC   #$3C
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$3B
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
   ADC   #$3D
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
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$14
   INY
   STA   (FP),Y
   LDA   #$E4
   INY
   STA   (FP),Y
   LDA   #$41
   INY
   STA   (FP),Y
   EOR   #$80
   INY
   STA   (FP),Y
   LDY   #$0A
   LDA   (FP),Y
   LDY   #$12
   CMP   (FP),Y
   BNE   .thread@25
   LDY   #$09
   LDA   (FP),Y
   LDY   #$11
   CMP   (FP),Y
   BNE   .thread@25
   LDY   #$08
   LDA   (FP),Y
   LDY   #$10
   CMP   (FP),Y
   BNE   .thread@25
   LDY   #$07
   LDA   (FP),Y
   LDY   #$0F
   CMP   (FP),Y
   BNE   .thread@25
   LDY   #$06
   LDA   (FP),Y
   LDY   #$0E
   CMP   (FP),Y
   BNE   .thread@25
   LDY   #$05
   LDA   (FP),Y
   LDY   #$0D
   CMP   (FP),Y
   BNE   .thread@25
   LDY   #$04
   LDA   (FP),Y
   LDY   #$0C
   CMP   (FP),Y
   BNE   .thread@25
   LDY   #$03
   LDA   (FP),Y
   LDY   #$0B
   CMP   (FP),Y
   BEQ   .cmp_true@89
.thread@25:
   LDA   #$00
   JMP   .cmp_end@90
.cmp_true@89:
   LDA   #$01
.cmp_end@90:
   LDY   #$13
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@16
   JMP   .and_false@36
.lb_skip@16:
   CLC
   LDA   FP
   ADC   #$3D
   LDY   #$15
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
   LDA   #$14
   INY
   STA   (FP),Y
   LDA   #$E4
   INY
   STA   (FP),Y
   LDA   #$41
   INY
   STA   (FP),Y
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
   BEQ   .cmp_true@92
.thread@26:
   LDA   #$00
   JMP   .cmp_end@93
.cmp_true@92:
   LDA   #$01
.cmp_end@93:
   LDY   #$27
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
   LDY   #$29
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@37:
   LDY   #$29
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@15
   JMP   .and_false@34
.lb_skip@15:
   CLC
   LDA   FP
   ADC   #$3D
   LDY   #$2B
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$08
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
   CMP   #$40
   BNE   .thread@27
   DEY
   LDA   (FP),Y
   CMP   #$5E
   BNE   .thread@27
   DEY
   LDA   (FP),Y
   CMP   #$DC
   BNE   .thread@27
   DEY
   LDA   (FP),Y
   CMP   #$CC
   BNE   .thread@27
   DEY
   LDA   (FP),Y
   CMP   #$CC
   BNE   .thread@27
   DEY
   LDA   (FP),Y
   CMP   #$CC
   BNE   .thread@27
   DEY
   LDA   (FP),Y
   CMP   #$CC
   BNE   .thread@27
   DEY
   LDA   (FP),Y
   CMP   #$CD
   BEQ   .cmp_true@95
.thread@27:
   LDA   #$00
   JMP   .cmp_end@96
.cmp_true@95:
   LDA   #$01
.cmp_end@96:
   LDY   #$37
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@34
   LDA   #$01
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   JMP   .and_end@35
.and_false@34:
   LDA   #$00
   LDY   #$39
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@35:
   TAX
   LDY   #$39
   LDA   (FP),Y

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$4C
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$3B
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

test_has_dbl_struct:
   SUBROUTINE

   ; prologue: 16 arg bytes, 58 local bytes
   SEC
   LDA   SSP
   SBC   #$3C
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$3B
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
   ADC   #$3D
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
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$14
   INY
   STA   (FP),Y
   LDA   #$E4
   INY
   STA   (FP),Y
   LDA   #$41
   INY
   STA   (FP),Y
   EOR   #$80
   INY
   STA   (FP),Y
   LDY   #$0A
   LDA   (FP),Y
   LDY   #$12
   CMP   (FP),Y
   BNE   .thread@28
   LDY   #$09
   LDA   (FP),Y
   LDY   #$11
   CMP   (FP),Y
   BNE   .thread@28
   LDY   #$08
   LDA   (FP),Y
   LDY   #$10
   CMP   (FP),Y
   BNE   .thread@28
   LDY   #$07
   LDA   (FP),Y
   LDY   #$0F
   CMP   (FP),Y
   BNE   .thread@28
   LDY   #$06
   LDA   (FP),Y
   LDY   #$0E
   CMP   (FP),Y
   BNE   .thread@28
   LDY   #$05
   LDA   (FP),Y
   LDY   #$0D
   CMP   (FP),Y
   BNE   .thread@28
   LDY   #$04
   LDA   (FP),Y
   LDY   #$0C
   CMP   (FP),Y
   BNE   .thread@28
   LDY   #$03
   LDA   (FP),Y
   LDY   #$0B
   CMP   (FP),Y
   BEQ   .cmp_true@98
.thread@28:
   LDA   #$00
   JMP   .cmp_end@99
.cmp_true@98:
   LDA   #$01
.cmp_end@99:
   LDY   #$13
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@18
   JMP   .and_false@40
.lb_skip@18:
   CLC
   LDA   FP
   ADC   #$3D
   LDY   #$15
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
   LDA   #$14
   INY
   STA   (FP),Y
   LDA   #$E4
   INY
   STA   (FP),Y
   LDA   #$41
   INY
   STA   (FP),Y
   EOR   #$80
   INY
   STA   (FP),Y
   LDY   #$1E
   LDA   (FP),Y
   LDY   #$26
   CMP   (FP),Y
   BNE   .thread@29
   LDY   #$1D
   LDA   (FP),Y
   LDY   #$25
   CMP   (FP),Y
   BNE   .thread@29
   LDY   #$1C
   LDA   (FP),Y
   LDY   #$24
   CMP   (FP),Y
   BNE   .thread@29
   LDY   #$1B
   LDA   (FP),Y
   LDY   #$23
   CMP   (FP),Y
   BNE   .thread@29
   LDY   #$1A
   LDA   (FP),Y
   LDY   #$22
   CMP   (FP),Y
   BNE   .thread@29
   LDY   #$19
   LDA   (FP),Y
   LDY   #$21
   CMP   (FP),Y
   BNE   .thread@29
   LDY   #$18
   LDA   (FP),Y
   LDY   #$20
   CMP   (FP),Y
   BNE   .thread@29
   LDY   #$17
   LDA   (FP),Y
   LDY   #$1F
   CMP   (FP),Y
   BEQ   .cmp_true@101
.thread@29:
   LDA   #$00
   JMP   .cmp_end@102
.cmp_true@101:
   LDA   #$01
.cmp_end@102:
   LDY   #$27
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
   LDY   #$29
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@41:
   LDY   #$29
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@17
   JMP   .and_false@38
.lb_skip@17:
   CLC
   LDA   FP
   ADC   #$3D
   LDY   #$2B
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$08
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
   CMP   #$40
   BNE   .thread@30
   DEY
   LDA   (FP),Y
   CMP   #$5E
   BNE   .thread@30
   DEY
   LDA   (FP),Y
   CMP   #$DC
   BNE   .thread@30
   DEY
   LDA   (FP),Y
   CMP   #$CC
   BNE   .thread@30
   DEY
   LDA   (FP),Y
   CMP   #$CC
   BNE   .thread@30
   DEY
   LDA   (FP),Y
   CMP   #$CC
   BNE   .thread@30
   DEY
   LDA   (FP),Y
   CMP   #$CC
   BNE   .thread@30
   DEY
   LDA   (FP),Y
   CMP   #$CD
   BEQ   .cmp_true@104
.thread@30:
   LDA   #$00
   JMP   .cmp_end@105
.cmp_true@104:
   LDA   #$01
.cmp_end@105:
   LDY   #$37
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
   LDY   #$39
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@39:
   TAX
   LDY   #$39
   LDA   (FP),Y

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$4C
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$3B
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

test_char_arr:
   SUBROUTINE

   ; prologue: 11 arg bytes, 20 local bytes
   SEC
   LDA   SSP
   SBC   #$16
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$15
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
   ADC   #$17
   LDY   #$03
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   LDA   #<str@1
   LDY   #$01
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
   LDY   #$04
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   DEY
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   DEY
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   JSR   strcmp
   LDY   #$05
   STA   (FP),Y
   TXA
   INY
   STA   (FP),Y
   CMP   #$00
   BNE   .thread@31
   DEY
   LDA   (FP),Y
   CMP   #$00
   BEQ   .cmp_true@107
.thread@31:
   LDA   #$00
   JMP   .cmp_end@108
.cmp_true@107:
   LDA   #$01
.cmp_end@108:
   LDY   #$07
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@19
   JMP   .and_false@42
.lb_skip@19:
   CLC
   LDA   FP
   ADC   #$17
   LDY   #$09
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
   LDY   #$0B
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
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
   BMI   .sx_neg@110
   LDA   #$00
   JMP   .sx_done@111
.sx_neg@110:
   LDA   #$FF
.sx_done@111:
   LDY   #$0F
   STA   (FP),Y
   INY
   STA   (FP),Y
   CMP   #$72
   BNE   .thread@32
   CMP   #$61
   BNE   .thread@32
   LDY   #$0E
   LDA   (FP),Y
   CMP   #$68
   BNE   .thread@32
   DEY
   LDA   (FP),Y
   CMP   #$43
   BEQ   .cmp_true@112
.thread@32:
   LDA   #$00
   JMP   .cmp_end@113
.cmp_true@112:
   LDA   #$01
.cmp_end@113:
   LDY   #$11
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@42
   LDA   #$01
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   JMP   .and_end@43
.and_false@42:
   LDA   #$00
   LDY   #$13
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@43:
   TAX
   LDY   #$13
   LDA   (FP),Y

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$21
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$15
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

test_two_arrs:
   SUBROUTINE

   ; prologue: 16 arg bytes, 78 local bytes
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

   CLC
   LDA   FP
   ADC   #$51
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
.mul_loop@115:
   BCC   .mul_skip@116
   CLC
   ADC   #$08
.mul_skip@116:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@115
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
   BNE   .thread@33
   DEY
   LDA   (FP),Y
   CMP   #$FF
   BNE   .thread@33
   DEY
   LDA   (FP),Y
   CMP   #$BD
   BNE   .thread@33
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .thread@33
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .thread@33
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .thread@33
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .thread@33
   DEY
   LDA   (FP),Y
   CMP   #$00
   BEQ   .cmp_true@117
.thread@33:
   LDA   #$00
   JMP   .cmp_end@118
.cmp_true@117:
   LDA   #$01
.cmp_end@118:
   LDY   #$0F
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@22
   JMP   .and_false@48
.lb_skip@22:
   CLC
   LDA   FP
   ADC   #$51
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
.mul_loop@120:
   BCC   .mul_skip@121
   CLC
   ADC   #$08
.mul_skip@121:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@120
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
   BNE   .thread@34
   DEY
   LDA   (FP),Y
   CMP   #$2D
   BNE   .thread@34
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .thread@34
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .thread@34
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .thread@34
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .thread@34
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .thread@34
   DEY
   LDA   (FP),Y
   CMP   #$00
   BEQ   .cmp_true@122
.thread@34:
   LDA   #$00
   JMP   .cmp_end@123
.cmp_true@122:
   LDA   #$01
.cmp_end@123:
   LDY   #$1F
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@48
   LDA   #$01
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   JMP   .and_end@49
.and_false@48:
   LDA   #$00
   LDY   #$21
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@49:
   LDY   #$21
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@21
   JMP   .and_false@46
.lb_skip@21:
   CLC
   LDA   FP
   ADC   #$51
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
.mul_loop@125:
   BCC   .mul_skip@126
   CLC
   ADC   #$04
.mul_skip@126:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@125
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
   LDY   #$29
   LDA   (FP),Y
   LDY   #$2D
   STA   (FP),Y
   LDY   #$2A
   LDA   (FP),Y
   LDY   #$2E
   STA   (FP),Y
   LDY   #$2B
   LDA   (FP),Y
   LDY   #$2F
   STA   (FP),Y
   LDY   #$2C
   LDA   (FP),Y
   LDY   #$30
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@127
   LDA   #$00
   JMP   .sx_done@128
.sx_neg@127:
   LDA   #$FF
.sx_done@128:
   LDY   #$31
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   CMP   #$40
   BNE   .thread@35
   CMP   #$FF
   BNE   .thread@35
   CMP   #$BD
   BNE   .thread@35
   CMP   #$00
   BNE   .thread@35
   LDY   #$30
   LDA   (FP),Y
   CMP   #$00
   BNE   .thread@35
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .thread@35
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .thread@35
   DEY
   LDA   (FP),Y
   CMP   #$00
   BEQ   .cmp_true@129
.thread@35:
   LDA   #$00
   JMP   .cmp_end@130
.cmp_true@129:
   LDA   #$01
.cmp_end@130:
   LDY   #$35
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
   LDY   #$37
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@47:
   LDY   #$37
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@20
   JMP   .and_false@44
.lb_skip@20:
   CLC
   LDA   FP
   ADC   #$51
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
.mul_loop@132:
   BCC   .mul_skip@133
   CLC
   ADC   #$04
.mul_skip@133:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@132
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
   LDY   #$3F
   LDA   (FP),Y
   LDY   #$43
   STA   (FP),Y
   LDY   #$40
   LDA   (FP),Y
   LDY   #$44
   STA   (FP),Y
   LDY   #$41
   LDA   (FP),Y
   LDY   #$45
   STA   (FP),Y
   LDY   #$42
   LDA   (FP),Y
   LDY   #$46
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@134
   LDA   #$00
   JMP   .sx_done@135
.sx_neg@134:
   LDA   #$FF
.sx_done@135:
   LDY   #$47
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   CMP   #$40
   BNE   .thread@36
   CMP   #$2D
   BNE   .thread@36
   CMP   #$00
   BNE   .thread@36
   LDY   #$46
   LDA   (FP),Y
   BNE   .thread@36
   DEY
   LDA   (FP),Y
   BNE   .thread@36
   DEY
   LDA   (FP),Y
   BNE   .thread@36
   DEY
   LDA   (FP),Y
   BEQ   .cmp_true@136
.thread@36:
   LDA   #$00
   JMP   .cmp_end@137
.cmp_true@136:
   LDA   #$01
.cmp_end@137:
   LDY   #$4B
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@44
   LDA   #$01
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   JMP   .and_end@45
.and_false@44:
   LDA   #$00
   LDY   #$4D
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@45:
   TAX
   LDY   #$4D
   LDA   (FP),Y

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$60
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

test_two_eightbyte_has_struct:
   SUBROUTINE

   ; prologue: 16 arg bytes, 48 local bytes
   SEC
   LDA   SSP
   SBC   #$32
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$31
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
   ADC   #$33
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
.mul_loop@139:
   BCC   .mul_skip@140
   CLC
   ADC   #$02
.mul_skip@140:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@139
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
   CMP   #$00
   BNE   .thread@37
   DEY
   LDA   (FP),Y
   CMP   #$64
   BEQ   .cmp_true@141
.thread@37:
   LDA   #$00
   JMP   .cmp_end@142
.cmp_true@141:
   LDA   #$01
.cmp_end@142:
   LDY   #$09
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@25
   JMP   .and_false@54
.lb_skip@25:
   CLC
   LDA   FP
   ADC   #$33
   LDY   #$0D
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
.mul_loop@144:
   BCC   .mul_skip@145
   CLC
   ADC   #$02
.mul_skip@145:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@144
   LDY   #$0C
   STA   (FP),Y
   LDA   HARGS
   DEY
   STA   (FP),Y
   LDY   #$0D
   LDA   (FP),Y
   CLC
   LDY   #$0B
   ADC   (FP),Y
   LDY   #$0F
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$0C
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
   CMP   #$00
   BNE   .thread@38
   DEY
   LDA   (FP),Y
   CMP   #$C8
   BEQ   .cmp_true@146
.thread@38:
   LDA   #$00
   JMP   .cmp_end@147
.cmp_true@146:
   LDA   #$01
.cmp_end@147:
   LDY   #$13
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
   LDY   #$15
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@55:
   LDY   #$15
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@24
   JMP   .and_false@52
.lb_skip@24:
   CLC
   LDA   FP
   ADC   #$33
   LDY   #$19
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
.mul_loop@149:
   BCC   .mul_skip@150
   CLC
   ADC   #$02
.mul_skip@150:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@149
   LDY   #$18
   STA   (FP),Y
   LDA   HARGS
   DEY
   STA   (FP),Y
   LDY   #$19
   LDA   (FP),Y
   CLC
   LDY   #$17
   ADC   (FP),Y
   LDY   #$1B
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$18
   ADC   (FP),Y
   LDY   #$1C
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$1D
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$1E
   STA   (FP),Y
   CMP   #$01
   BNE   .thread@39
   DEY
   LDA   (FP),Y
   CMP   #$2C
   BEQ   .cmp_true@151
.thread@39:
   LDA   #$00
   JMP   .cmp_end@152
.cmp_true@151:
   LDA   #$01
.cmp_end@152:
   LDY   #$1F
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
   LDY   #$21
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@53:
   LDY   #$21
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@23
   JMP   .and_false@50
.lb_skip@23:
   CLC
   LDA   FP
   ADC   #$33
   LDY   #$23
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
   LDY   #$25
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$26
   STA   (FP),Y
   LDY   #$02
   LDA   (DPTR),Y
   LDY   #$27
   STA   (FP),Y
   LDY   #$03
   LDA   (DPTR),Y
   LDY   #$28
   STA   (FP),Y
   LDY   #$04
   LDA   (DPTR),Y
   LDY   #$29
   STA   (FP),Y
   LDY   #$05
   LDA   (DPTR),Y
   LDY   #$2A
   STA   (FP),Y
   LDY   #$06
   LDA   (DPTR),Y
   LDY   #$2B
   STA   (FP),Y
   LDY   #$07
   LDA   (DPTR),Y
   LDY   #$2C
   STA   (FP),Y
   CMP   #$00
   BNE   .thread@40
   DEY
   LDA   (FP),Y
   BNE   .thread@40
   DEY
   LDA   (FP),Y
   BNE   .thread@40
   DEY
   LDA   (FP),Y
   CMP   #$C8
   BNE   .thread@40
   DEY
   LDA   (FP),Y
   BNE   .thread@40
   DEY
   LDA   (FP),Y
   BNE   .thread@40
   DEY
   LDA   (FP),Y
   BNE   .thread@40
   DEY
   LDA   (FP),Y
   CMP   #$64
   BEQ   .cmp_true@154
.thread@40:
   LDA   #$00
   JMP   .cmp_end@155
.cmp_true@154:
   LDA   #$01
.cmp_end@155:
   LDY   #$2D
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@50
   LDA   #$01
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   JMP   .and_end@51
.and_false@50:
   LDA   #$00
   LDY   #$2F
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@51:
   TAX
   LDY   #$2F
   LDA   (FP),Y

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$42
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$31
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

test_two_structs:
   SUBROUTINE

   ; prologue: 10 arg bytes, 33 local bytes
   SEC
   LDA   SSP
   SBC   #$23
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$22
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
   ADC   #$24
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
   CMP   #$78
   BEQ   .cmp_true@157
   LDA   #$00
   JMP   .cmp_end@158
.cmp_true@157:
   LDA   #$01
.cmp_end@158:
   LDY   #$06
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@26
   JMP   .and_false@58
.lb_skip@26:
   CLC
   LDA   FP
   ADC   #$24
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
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$0C
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$0D
   STA   (FP),Y
   LDY   #$02
   LDA   (DPTR),Y
   LDY   #$0E
   STA   (FP),Y
   LDY   #$03
   LDA   (DPTR),Y
   LDY   #$0F
   STA   (FP),Y
   LDY   #$04
   LDA   (DPTR),Y
   LDY   #$10
   STA   (FP),Y
   LDY   #$05
   LDA   (DPTR),Y
   LDY   #$11
   STA   (FP),Y
   LDY   #$06
   LDA   (DPTR),Y
   LDY   #$12
   STA   (FP),Y
   LDY   #$07
   LDA   (DPTR),Y
   LDY   #$13
   STA   (FP),Y
   CMP   #$41
   BNE   .thread@41
   DEY
   LDA   (FP),Y
   CMP   #$55
   BNE   .thread@41
   DEY
   LDA   (FP),Y
   CMP   #$2B
   BNE   .thread@41
   DEY
   LDA   (FP),Y
   CMP   #$EC
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
   BEQ   .cmp_true@159
.thread@41:
   LDA   #$00
   JMP   .cmp_end@160
.cmp_true@159:
   LDA   #$01
.cmp_end@160:
   LDY   #$14
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
   LDY   #$16
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@59:
   LDY   #$16
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@56
   CLC
   LDA   FP
   ADC   #$24
   INY
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$08
   LDY   #$1A
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$1B
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$1C
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$1D
   STA   (FP),Y
   CMP   #$00
   BNE   .thread@42
   DEY
   LDA   (FP),Y
   BEQ   .cmp_true@162
.thread@42:
   LDA   #$00
   JMP   .cmp_end@163
.cmp_true@162:
   LDA   #$01
.cmp_end@163:
   LDY   #$1E
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
   LDY   #$20
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@57:
   TAX
   LDY   #$20
   LDA   (FP),Y

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$2D
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$22
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

test_has_nine_byte_struct:
   SUBROUTINE

   ; prologue: 7 arg bytes, 75 local bytes
   SEC
   LDA   SSP
   SBC   #$4D
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$4C
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
   ADC   #$4E
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
   LDY   #$03
   LDA   (FP),Y
   LDY   #$07
   STA   (FP),Y
   LDY   #$04
   LDA   (FP),Y
   LDY   #$08
   STA   (FP),Y
   LDY   #$05
   LDA   (FP),Y
   LDY   #$09
   STA   (FP),Y
   LDY   #$06
   LDA   (FP),Y
   LDY   #$0A
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@165
   LDA   #$00
   JMP   .sx_done@166
.sx_neg@165:
   LDA   #$FF
.sx_done@166:
   LDY   #$0B
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   SEC
   LDA   #$00
   SBC   #$00
   INY
   STA   (FP),Y
   LDA   #$00
   SBC   #$01
   INY
   STA   (FP),Y
   LDA   #$00
   SBC   #$FF
   INY
   STA   (FP),Y
   LDA   #$00
   SBC   #$00
   INY
   STA   (FP),Y
   LDA   #$00
   SBC   #$FF
   INY
   STA   (FP),Y
   LDA   #$00
   SBC   #$00
   INY
   STA   (FP),Y
   LDA   #$00
   SBC   #$FF
   INY
   STA   (FP),Y
   LDA   #$00
   SBC   #$00
   INY
   STA   (FP),Y
   LDY   #$0E
   LDA   (FP),Y
   LDY   #$16
   CMP   (FP),Y
   BNE   .cmp_true@167
   DEY
   CMP   (FP),Y
   BNE   .cmp_true@167
   DEY
   CMP   (FP),Y
   BNE   .cmp_true@167
   DEY
   CMP   (FP),Y
   BNE   .cmp_true@167
   LDY   #$0A
   LDA   (FP),Y
   LDY   #$12
   CMP   (FP),Y
   BNE   .cmp_true@167
   LDY   #$09
   LDA   (FP),Y
   LDY   #$11
   CMP   (FP),Y
   BNE   .cmp_true@167
   LDY   #$08
   LDA   (FP),Y
   LDY   #$10
   CMP   (FP),Y
   BNE   .cmp_true@167
   LDY   #$07
   LDA   (FP),Y
   LDY   #$0F
   CMP   (FP),Y
   BNE   .cmp_true@167
   LDA   #$00
   JMP   .cmp_end@168
.cmp_true@167:
   LDA   #$01
.cmp_end@168:
   LDY   #$17
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@60
   CLC
   LDA   FP
   ADC   #$4E
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
   LDY   #$1B
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@170
   LDA   #$00
   JMP   .cmp_end@171
.cmp_true@170:
   LDA   #$01
.cmp_end@171:
   LDY   #$1E
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
   LDY   #$20
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@61:
   LDY   #$20
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
   ADC   #$54
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$4C
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
   CLC
   LDA   FP
   ADC   #$4E
   LDY   #$22
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
   LDY   #$24
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$25
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$26
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$27
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@172
   LDA   #$00
   JMP   .sx_done@173
.sx_neg@172:
   LDA   #$FF
.sx_done@173:
   LDY   #$28
   STA   (FP),Y
   INY
   STA   (FP),Y
   SEC
   LDA   #$00
   SBC   #$00
   INY
   STA   (FP),Y
   LDA   #$00
   SBC   #$01
   INY
   STA   (FP),Y
   LDA   #$00
   SBC   #$FF
   INY
   STA   (FP),Y
   LDA   #$00
   SBC   #$00
   INY
   STA   (FP),Y
   LDY   #$29
   LDA   (FP),Y
   LDY   #$2D
   CMP   (FP),Y
   BNE   .cmp_true@174
   DEY
   CMP   (FP),Y
   BNE   .cmp_true@174
   LDY   #$27
   LDA   (FP),Y
   LDY   #$2B
   CMP   (FP),Y
   BNE   .cmp_true@174
   LDY   #$26
   LDA   (FP),Y
   LDY   #$2A
   CMP   (FP),Y
   BNE   .cmp_true@174
   LDA   #$00
   JMP   .cmp_end@175
.cmp_true@174:
   LDA   #$01
.cmp_end@175:
   LDY   #$2E
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
   ADC   #$54
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$4C
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
   LDA   #$00
   LDY   #$30
   STA   (FP),Y
   INY
   STA   (FP),Y
.loop@0_start:
   LDY   #$30
   LDA   (FP),Y
   SEC
   SBC   #$05
   INY
   LDA   (FP),Y
   SBC   #$00
   BVC   .cmp_novf@177
   EOR   #$80
.cmp_novf@177:
   ASL
   LDA   #$00
   ROL
   LDY   #$32
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@27
   JMP   .loop@0_break
.lb_skip@27:
   LDY   #$30
   LDA   (FP),Y
   STA   HARGS
   INY
   LDA   (FP),Y
   STA   HARGS+1
   LDA   #$02
   STA   HARGS+2
   LDA   #$00
   STA   HARGS+3
   JSR   sdivmod16
   LDA   HARGS+6
   LDY   #$34
   STA   (FP),Y
   LDA   HARGS+7
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .cond_else@64
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
   LDY   #$38
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$39
   STA   (FP),Y
   JMP   .cond_end@65
.cond_else@64:
   LDA   #$00
   LDY   #$38
   STA   (FP),Y
   INY
   STA   (FP),Y
.cond_end@65:
   LDY   #$38
   LDA   (FP),Y
   LDY   #$3A
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$3B
   STA   (FP),Y
   CLC
   LDA   FP
   ADC   #$4E
   INY
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$02
   LDY   #$3E
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$3F
   STA   (FP),Y
   LDY   #$30
   LDA   (FP),Y
   LDY   #$40
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$3E
   LDA   (FP),Y
   CLC
   LDY   #$41
   ADC   (FP),Y
   LDY   #$43
   STA   (FP),Y
   LDY   #$3F
   LDA   (FP),Y
   LDY   #$42
   ADC   (FP),Y
   LDY   #$44
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$45
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$3B
   CMP   (FP),Y
   BNE   .cmp_true@178
   LDY   #$46
   LDA   (FP),Y
   LDY   #$3A
   CMP   (FP),Y
   BNE   .cmp_true@178
   LDA   #$00
   JMP   .cmp_end@179
.cmp_true@178:
   LDA   #$01
.cmp_end@179:
   LDY   #$48
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
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
   ADC   #$54
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$4C
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
   LDY   #$30
   LDA   (FP),Y
   CLC
   ADC   #$01
   LDY   #$4A
   STA   (FP),Y
   LDY   #$31
   LDA   (FP),Y
   ADC   #$00
   LDY   #$4B
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$30
   STA   (FP),Y
   LDY   #$4B
   LDA   (FP),Y
   LDY   #$31
   STA   (FP),Y
   JMP   .loop@0_start
.loop@0_break:
   LDX   #$00
   LDA   #$01

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$54
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$4C
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

test_has_uneven_union:
   SUBROUTINE

   ; prologue: 7 arg bytes, 37 local bytes
   SEC
   LDA   SSP
   SBC   #$27
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$26
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
   ADC   #$28
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
   DEY
   LDA   (FP),Y
   LDY   #$05
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$06
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@181
   LDA   #$00
   JMP   .sx_done@182
.sx_neg@181:
   LDA   #$FF
.sx_done@182:
   LDY   #$07
   STA   (FP),Y
   INY
   STA   (FP),Y
   SEC
   LDA   #$00
   SBC   #$FF
   INY
   STA   (FP),Y
   LDA   #$00
   SBC   #$FF
   INY
   STA   (FP),Y
   LDA   #$00
   SBC   #$FF
   INY
   STA   (FP),Y
   LDA   #$00
   SBC   #$7F
   INY
   STA   (FP),Y
   LDY   #$08
   LDA   (FP),Y
   LDY   #$0C
   CMP   (FP),Y
   BNE   .thread@43
   DEY
   CMP   (FP),Y
   BNE   .thread@43
   LDY   #$06
   LDA   (FP),Y
   LDY   #$0A
   CMP   (FP),Y
   BNE   .thread@43
   LDY   #$05
   LDA   (FP),Y
   LDY   #$09
   CMP   (FP),Y
   BEQ   .cmp_true@183
.thread@43:
   LDA   #$00
   JMP   .cmp_end@184
.cmp_true@183:
   LDA   #$01
.cmp_end@184:
   LDY   #$0D
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@28
   JMP   .and_false@69
.lb_skip@28:
   CLC
   LDA   FP
   ADC   #$28
   LDY   #$0F
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$02
   LDY   #$11
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$12
   STA   (FP),Y
   LDA   #<str@2
   INY
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
   CMP   #$00
   BNE   .thread@44
   DEY
   LDA   (FP),Y
   CMP   #$00
   BEQ   .cmp_true@186
.thread@44:
   LDA   #$00
   JMP   .cmp_end@187
.cmp_true@186:
   LDA   #$01
.cmp_end@187:
   LDY   #$17
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@69
   LDA   #$01
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   JMP   .and_end@70
.and_false@69:
   LDA   #$00
   LDY   #$19
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@70:
   LDY   #$19
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@67
   CLC
   LDA   FP
   ADC   #$28
   INY
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$02
   LDY   #$1D
   STA   (FP),Y
   DEY
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
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$1F
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CMP   #$21
   BEQ   .cmp_true@189
   LDA   #$00
   JMP   .cmp_end@190
.cmp_true@189:
   LDA   #$01
.cmp_end@190:
   LDY   #$22
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@67
   LDA   #$01
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   JMP   .and_end@68
.and_false@67:
   LDA   #$00
   LDY   #$24
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@68:
   TAX
   LDY   #$24
   LDA   (FP),Y

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$2E
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$26
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

test_has_other_unions:
   SUBROUTINE

   ; prologue: 16 arg bytes, 52 local bytes
   SEC
   LDA   SSP
   SBC   #$36
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$35
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
   ADC   #$37
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
   LDY   #$03
   LDA   (FP),Y
   LDY   #$07
   STA   (FP),Y
   LDY   #$04
   LDA   (FP),Y
   LDY   #$08
   STA   (FP),Y
   LDY   #$05
   LDA   (FP),Y
   LDY   #$09
   STA   (FP),Y
   LDY   #$06
   LDA   (FP),Y
   LDY   #$0A
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@191
   LDA   #$00
   JMP   .sx_done@192
.sx_neg@191:
   LDA   #$FF
.sx_done@192:
   LDY   #$0B
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   SEC
   LDA   #$00
   SBC   #$00
   INY
   STA   (FP),Y
   LDA   #$00
   SBC   #$01
   INY
   STA   (FP),Y
   LDA   #$00
   SBC   #$FF
   INY
   STA   (FP),Y
   LDA   #$00
   SBC   #$00
   INY
   STA   (FP),Y
   LDA   #$00
   SBC   #$FF
   INY
   STA   (FP),Y
   LDA   #$00
   SBC   #$00
   INY
   STA   (FP),Y
   LDA   #$00
   SBC   #$FF
   INY
   STA   (FP),Y
   LDA   #$00
   SBC   #$00
   INY
   STA   (FP),Y
   LDY   #$0E
   LDA   (FP),Y
   LDY   #$16
   CMP   (FP),Y
   BNE   .cmp_true@193
   DEY
   CMP   (FP),Y
   BNE   .cmp_true@193
   DEY
   CMP   (FP),Y
   BNE   .cmp_true@193
   DEY
   CMP   (FP),Y
   BNE   .cmp_true@193
   LDY   #$0A
   LDA   (FP),Y
   LDY   #$12
   CMP   (FP),Y
   BNE   .cmp_true@193
   LDY   #$09
   LDA   (FP),Y
   LDY   #$11
   CMP   (FP),Y
   BNE   .cmp_true@193
   LDY   #$08
   LDA   (FP),Y
   LDY   #$10
   CMP   (FP),Y
   BNE   .cmp_true@193
   LDY   #$07
   LDA   (FP),Y
   LDY   #$0F
   CMP   (FP),Y
   BNE   .cmp_true@193
   LDA   #$00
   JMP   .cmp_end@194
.cmp_true@193:
   LDA   #$01
.cmp_end@194:
   LDY   #$17
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@71
   LDX   #$00
   TXA

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$46
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$35
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
.if_end@71:
   LDA   #$00
   LDY   #$19
   STA   (FP),Y
   INY
   STA   (FP),Y
.loop@1_start:
   LDY   #$19
   LDA   (FP),Y
   SEC
   SBC   #$05
   INY
   LDA   (FP),Y
   SBC   #$00
   BVC   .cmp_novf@196
   EOR   #$80
.cmp_novf@196:
   ASL
   LDA   #$00
   ROL
   LDY   #$1B
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@29
   JMP   .loop@1_break
.lb_skip@29:
   LDY   #$19
   LDA   (FP),Y
   STA   HARGS
   INY
   LDA   (FP),Y
   STA   HARGS+1
   LDA   #$02
   STA   HARGS+2
   LDA   #$00
   STA   HARGS+3
   JSR   sdivmod16
   LDA   HARGS+6
   LDY   #$1D
   STA   (FP),Y
   LDA   HARGS+7
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .cond_else@72
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
   LDY   #$21
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$22
   STA   (FP),Y
   JMP   .cond_end@73
.cond_else@72:
   LDA   #$00
   LDY   #$21
   STA   (FP),Y
   INY
   STA   (FP),Y
.cond_end@73:
   LDY   #$21
   LDA   (FP),Y
   LDY   #$23
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$24
   STA   (FP),Y
   CLC
   LDA   FP
   ADC   #$37
   INY
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$02
   LDY   #$27
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$28
   STA   (FP),Y
   LDY   #$19
   LDA   (FP),Y
   LDY   #$29
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$27
   LDA   (FP),Y
   CLC
   LDY   #$2A
   ADC   (FP),Y
   LDY   #$2C
   STA   (FP),Y
   LDY   #$28
   LDA   (FP),Y
   LDY   #$2B
   ADC   (FP),Y
   LDY   #$2D
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
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$24
   CMP   (FP),Y
   BNE   .cmp_true@197
   LDY   #$2F
   LDA   (FP),Y
   LDY   #$23
   CMP   (FP),Y
   BNE   .cmp_true@197
   LDA   #$00
   JMP   .cmp_end@198
.cmp_true@197:
   LDA   #$01
.cmp_end@198:
   LDY   #$31
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
   ADC   #$46
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$35
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
   LDY   #$19
   LDA   (FP),Y
   CLC
   ADC   #$01
   LDY   #$33
   STA   (FP),Y
   LDY   #$1A
   LDA   (FP),Y
   ADC   #$00
   LDY   #$34
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$19
   STA   (FP),Y
   LDY   #$34
   LDA   (FP),Y
   LDY   #$1A
   STA   (FP),Y
   JMP   .loop@1_start
.loop@1_break:
   LDX   #$00
   LDA   #$01

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$46
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$35
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

test_union_array:
   SUBROUTINE

   ; prologue: 16 arg bytes, 46 local bytes
   SEC
   LDA   SSP
   SBC   #$30
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$2F
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
   ADC   #$31
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
   LDY   #$0A
   LDA   (FP),Y
   LDY   #$12
   CMP   (FP),Y
   BNE   .thread@45
   LDY   #$09
   LDA   (FP),Y
   LDY   #$11
   CMP   (FP),Y
   BNE   .thread@45
   LDY   #$08
   LDA   (FP),Y
   LDY   #$10
   CMP   (FP),Y
   BNE   .thread@45
   LDY   #$07
   LDA   (FP),Y
   LDY   #$0F
   CMP   (FP),Y
   BNE   .thread@45
   LDY   #$06
   LDA   (FP),Y
   LDY   #$0E
   CMP   (FP),Y
   BNE   .thread@45
   LDY   #$05
   LDA   (FP),Y
   LDY   #$0D
   CMP   (FP),Y
   BNE   .thread@45
   LDY   #$04
   LDA   (FP),Y
   LDY   #$0C
   CMP   (FP),Y
   BNE   .thread@45
   LDY   #$03
   LDA   (FP),Y
   LDY   #$0B
   CMP   (FP),Y
   BEQ   .cmp_true@200
.thread@45:
   LDA   #$00
   JMP   .cmp_end@201
.cmp_true@200:
   LDA   #$01
.cmp_end@201:
   LDY   #$13
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@30
   JMP   .and_false@75
.lb_skip@30:
   CLC
   LDA   FP
   ADC   #$31
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
.mul_loop@203:
   BCC   .mul_skip@204
   CLC
   ADC   #$08
.mul_skip@204:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@203
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
   LDA   #$3E
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
   BNE   .thread@46
   LDY   #$21
   LDA   (FP),Y
   LDY   #$29
   CMP   (FP),Y
   BNE   .thread@46
   LDY   #$20
   LDA   (FP),Y
   LDY   #$28
   CMP   (FP),Y
   BNE   .thread@46
   LDY   #$1F
   LDA   (FP),Y
   LDY   #$27
   CMP   (FP),Y
   BNE   .thread@46
   LDY   #$1E
   LDA   (FP),Y
   LDY   #$26
   CMP   (FP),Y
   BNE   .thread@46
   LDY   #$1D
   LDA   (FP),Y
   LDY   #$25
   CMP   (FP),Y
   BNE   .thread@46
   LDY   #$1C
   LDA   (FP),Y
   LDY   #$24
   CMP   (FP),Y
   BNE   .thread@46
   LDY   #$1B
   LDA   (FP),Y
   LDY   #$23
   CMP   (FP),Y
   BEQ   .cmp_true@205
.thread@46:
   LDA   #$00
   JMP   .cmp_end@206
.cmp_true@205:
   LDA   #$01
.cmp_end@206:
   LDY   #$2B
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@75
   LDA   #$01
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   JMP   .and_end@76
.and_false@75:
   LDA   #$00
   LDY   #$2D
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@76:
   TAX
   LDY   #$2D
   LDA   (FP),Y

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
   LDY   #$2F
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

test_uneven_union_array:
   SUBROUTINE

   ; prologue: 10 arg bytes, 26 local bytes
   SEC
   LDA   SSP
   SBC   #$1C
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$1B
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
   ADC   #$1D
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
.mul_loop@208:
   BCC   .mul_skip@209
   CLC
   ADC   #$05
.mul_skip@209:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@208
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
   LDY   #$05
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$06
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$07
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$08
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   JSR   strcmp
   LDY   #$09
   STA   (FP),Y
   TXA
   INY
   STA   (FP),Y
   CMP   #$00
   BNE   .thread@47
   DEY
   LDA   (FP),Y
   CMP   #$00
   BEQ   .cmp_true@210
.thread@47:
   LDA   #$00
   JMP   .cmp_end@211
.cmp_true@210:
   LDA   #$01
.cmp_end@211:
   LDY   #$0B
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@31
   JMP   .and_false@77
.lb_skip@31:
   CLC
   LDA   FP
   ADC   #$1D
   LDY   #$0F
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
.mul_loop@213:
   BCC   .mul_skip@214
   CLC
   ADC   #$05
.mul_skip@214:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@213
   LDY   #$0E
   STA   (FP),Y
   LDA   HARGS
   DEY
   STA   (FP),Y
   LDY   #$0F
   LDA   (FP),Y
   CLC
   LDY   #$0D
   ADC   (FP),Y
   LDY   #$11
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$0E
   ADC   (FP),Y
   LDY   #$12
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
   CMP   #$00
   BNE   .thread@48
   DEY
   LDA   (FP),Y
   BEQ   .cmp_true@215
.thread@48:
   LDA   #$00
   JMP   .cmp_end@216
.cmp_true@215:
   LDA   #$01
.cmp_end@216:
   LDY   #$17
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
   LDY   #$19
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@78:
   TAX
   LDY   #$19
   LDA   (FP),Y

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$26
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$1B
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

test_has_small_struct_array:
   SUBROUTINE

   ; prologue: 12 arg bytes, 85 local bytes
   SEC
   LDA   SSP
   SBC   #$57
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$56
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
   ADC   #$58
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
.mul_loop@218:
   BCC   .mul_skip@219
   CLC
   ADC   #$04
.mul_skip@219:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@218
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
   LDA   #<str@5
   INY
   STA   (FP),Y
   LDA   #>str@5
   INY
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$04
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDY   #$05
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$06
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$07
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$08
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   JSR   strcmp
   LDY   #$09
   STA   (FP),Y
   TXA
   INY
   STA   (FP),Y
   CMP   #$00
   BNE   .thread@49
   DEY
   LDA   (FP),Y
   CMP   #$00
   BEQ   .cmp_true@220
.thread@49:
   LDA   #$00
   JMP   .cmp_end@221
.cmp_true@220:
   LDA   #$01
.cmp_end@221:
   LDY   #$0B
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@36
   JMP   .and_false@87
.lb_skip@36:
   CLC
   LDA   FP
   ADC   #$58
   LDY   #$0F
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   LDA   #$00
   STA   HARGS
   LDX   #$08
   LSR   HARGS
.mul_loop@223:
   BCC   .mul_skip@224
   CLC
   ADC   #$04
.mul_skip@224:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@223
   LDY   #$0E
   STA   (FP),Y
   LDA   HARGS
   DEY
   STA   (FP),Y
   LDY   #$0F
   LDA   (FP),Y
   CLC
   LDY   #$0D
   ADC   (FP),Y
   LDY   #$11
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$0E
   ADC   (FP),Y
   LDY   #$12
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$03
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
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$15
   STA   (FP),Y
   INY
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@225
   LDA   #$00
   JMP   .sx_done@226
.sx_neg@225:
   LDA   #$FF
.sx_done@226:
   LDY   #$17
   STA   (FP),Y
   CMP   #$00
   BNE   .thread@50
   DEY
   LDA   (FP),Y
   CMP   #$0A
   BEQ   .cmp_true@227
.thread@50:
   LDA   #$00
   JMP   .cmp_end@228
.cmp_true@227:
   LDA   #$01
.cmp_end@228:
   LDY   #$18
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
   LDY   #$1A
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@88:
   LDY   #$1A
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@35
   JMP   .and_false@85
.lb_skip@35:
   CLC
   LDA   FP
   ADC   #$58
   LDY   #$1E
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
.mul_loop@230:
   BCC   .mul_skip@231
   CLC
   ADC   #$04
.mul_skip@231:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@230
   LDY   #$1D
   STA   (FP),Y
   LDA   HARGS
   DEY
   STA   (FP),Y
   LDY   #$1E
   LDA   (FP),Y
   CLC
   LDY   #$1C
   ADC   (FP),Y
   LDY   #$20
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$1D
   ADC   (FP),Y
   LDY   #$21
   STA   (FP),Y
   LDA   #<str@6
   INY
   STA   (FP),Y
   LDA   #>str@6
   INY
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$04
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDY   #$20
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$21
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$22
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$23
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   JSR   strcmp
   LDY   #$24
   STA   (FP),Y
   TXA
   INY
   STA   (FP),Y
   CMP   #$00
   BNE   .thread@51
   DEY
   LDA   (FP),Y
   CMP   #$00
   BEQ   .cmp_true@232
.thread@51:
   LDA   #$00
   JMP   .cmp_end@233
.cmp_true@232:
   LDA   #$01
.cmp_end@233:
   LDY   #$26
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
   LDY   #$28
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@86:
   LDY   #$28
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@34
   JMP   .and_false@83
.lb_skip@34:
   CLC
   LDA   FP
   ADC   #$58
   LDY   #$2C
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
.mul_loop@235:
   BCC   .mul_skip@236
   CLC
   ADC   #$04
.mul_skip@236:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@235
   LDY   #$2B
   STA   (FP),Y
   LDA   HARGS
   DEY
   STA   (FP),Y
   LDY   #$2C
   LDA   (FP),Y
   CLC
   LDY   #$2A
   ADC   (FP),Y
   LDY   #$2E
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$2B
   ADC   (FP),Y
   LDY   #$2F
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$03
   LDY   #$30
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$31
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$32
   STA   (FP),Y
   INY
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@237
   LDA   #$00
   JMP   .sx_done@238
.sx_neg@237:
   LDA   #$FF
.sx_done@238:
   LDY   #$34
   STA   (FP),Y
   CMP   #$00
   BNE   .thread@52
   DEY
   LDA   (FP),Y
   CMP   #$0B
   BEQ   .cmp_true@239
.thread@52:
   LDA   #$00
   JMP   .cmp_end@240
.cmp_true@239:
   LDA   #$01
.cmp_end@240:
   LDY   #$35
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
   LDY   #$37
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@84:
   LDY   #$37
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@33
   JMP   .and_false@81
.lb_skip@33:
   CLC
   LDA   FP
   ADC   #$58
   LDY   #$3B
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
.mul_loop@242:
   BCC   .mul_skip@243
   CLC
   ADC   #$04
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
   LDA   #<str@7
   INY
   STA   (FP),Y
   LDA   #>str@7
   INY
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$04
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDY   #$3D
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$3E
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$3F
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$40
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   JSR   strcmp
   LDY   #$41
   STA   (FP),Y
   TXA
   INY
   STA   (FP),Y
   CMP   #$00
   BNE   .thread@53
   DEY
   LDA   (FP),Y
   CMP   #$00
   BEQ   .cmp_true@244
.thread@53:
   LDA   #$00
   JMP   .cmp_end@245
.cmp_true@244:
   LDA   #$01
.cmp_end@245:
   LDY   #$43
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
   LDY   #$45
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@82:
   LDY   #$45
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@32
   JMP   .and_false@79
.lb_skip@32:
   CLC
   LDA   FP
   ADC   #$58
   LDY   #$49
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
.mul_loop@247:
   BCC   .mul_skip@248
   CLC
   ADC   #$04
.mul_skip@248:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@247
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
   CLC
   ADC   #$03
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
   ORA   #$00
   BMI   .sx_neg@249
   LDA   #$00
   JMP   .sx_done@250
.sx_neg@249:
   LDA   #$FF
.sx_done@250:
   LDY   #$51
   STA   (FP),Y
   CMP   #$00
   BNE   .thread@54
   DEY
   LDA   (FP),Y
   CMP   #$0C
   BEQ   .cmp_true@251
.thread@54:
   LDA   #$00
   JMP   .cmp_end@252
.cmp_true@251:
   LDA   #$01
.cmp_end@252:
   LDY   #$52
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
   LDY   #$54
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@80:
   TAX
   LDY   #$54
   LDA   (FP),Y

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$63
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$56
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

test_gp_and_xmm:
   SUBROUTINE

   ; prologue: 16 arg bytes, 34 local bytes
   SEC
   LDA   SSP
   SBC   #$24
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$23
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
   ADC   #$25
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
.mul_loop@254:
   BCC   .mul_skip@255
   CLC
   ADC   #$08
.mul_skip@255:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@254
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
   BNE   .thread@55
   DEY
   LDA   (FP),Y
   CMP   #$26
   BNE   .thread@55
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .thread@55
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .thread@55
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .thread@55
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .thread@55
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .thread@55
   DEY
   LDA   (FP),Y
   CMP   #$00
   BEQ   .cmp_true@256
.thread@55:
   LDA   #$00
   JMP   .cmp_end@257
.cmp_true@256:
   LDA   #$01
.cmp_end@257:
   LDY   #$0F
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@37
   JMP   .and_false@89
.lb_skip@37:
   CLC
   LDA   FP
   ADC   #$25
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
.mul_loop@259:
   BCC   .mul_skip@260
   CLC
   ADC   #$08
.mul_skip@260:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@259
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
   BNE   .thread@56
   DEY
   LDA   (FP),Y
   CMP   #$28
   BNE   .thread@56
   DEY
   LDA   (FP),Y
   BNE   .thread@56
   DEY
   LDA   (FP),Y
   BNE   .thread@56
   DEY
   LDA   (FP),Y
   BNE   .thread@56
   DEY
   LDA   (FP),Y
   BNE   .thread@56
   DEY
   LDA   (FP),Y
   BNE   .thread@56
   DEY
   LDA   (FP),Y
   BEQ   .cmp_true@261
.thread@56:
   LDA   #$00
   JMP   .cmp_end@262
.cmp_true@261:
   LDA   #$01
.cmp_end@262:
   LDY   #$1F
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
   LDY   #$21
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@90:
   TAX
   LDY   #$21
   LDA   (FP),Y

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$34
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$23
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

test_scalar_and_struct:
   SUBROUTINE

   ; prologue: 9 arg bytes, 33 local bytes
   SEC
   LDA   SSP
   SBC   #$23
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$22
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
   ADC   #$24
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
   SEC
   SBC   #$05
   INY
   STA   (FP),Y
   LDA   #$00
   SBC   #$00
   INY
   STA   (FP),Y
   LDY   #$05
   LDA   (FP),Y
   LDY   #$07
   CMP   (FP),Y
   BNE   .thread@57
   LDY   #$04
   LDA   (FP),Y
   LDY   #$06
   CMP   (FP),Y
   BEQ   .cmp_true@264
.thread@57:
   LDA   #$00
   JMP   .cmp_end@265
.cmp_true@264:
   LDA   #$01
.cmp_end@265:
   LDY   #$08
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@38
   JMP   .and_false@91
.lb_skip@38:
   CLC
   LDA   FP
   ADC   #$24
   LDY   #$0A
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$01
   LDY   #$0C
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$0D
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$0E
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$0F
   STA   (FP),Y
   LDY   #$02
   LDA   (DPTR),Y
   LDY   #$10
   STA   (FP),Y
   LDY   #$03
   LDA   (DPTR),Y
   LDY   #$11
   STA   (FP),Y
   LDY   #$04
   LDA   (DPTR),Y
   LDY   #$12
   STA   (FP),Y
   LDY   #$05
   LDA   (DPTR),Y
   LDY   #$13
   STA   (FP),Y
   LDY   #$06
   LDA   (DPTR),Y
   LDY   #$14
   STA   (FP),Y
   LDY   #$07
   LDA   (DPTR),Y
   LDY   #$15
   STA   (FP),Y
   LDA   #$33
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
   LDA   #$56
   INY
   STA   (FP),Y
   LDA   #$40
   EOR   #$80
   INY
   STA   (FP),Y
   LDY   #$15
   LDA   (FP),Y
   LDY   #$1D
   CMP   (FP),Y
   BNE   .thread@58
   LDY   #$14
   LDA   (FP),Y
   LDY   #$1C
   CMP   (FP),Y
   BNE   .thread@58
   LDY   #$13
   LDA   (FP),Y
   LDY   #$1B
   CMP   (FP),Y
   BNE   .thread@58
   LDY   #$12
   LDA   (FP),Y
   LDY   #$1A
   CMP   (FP),Y
   BNE   .thread@58
   LDY   #$11
   LDA   (FP),Y
   LDY   #$19
   CMP   (FP),Y
   BNE   .thread@58
   LDY   #$10
   LDA   (FP),Y
   LDY   #$18
   CMP   (FP),Y
   BNE   .thread@58
   LDY   #$0F
   LDA   (FP),Y
   LDY   #$17
   CMP   (FP),Y
   BNE   .thread@58
   LDY   #$0E
   LDA   (FP),Y
   LDY   #$16
   CMP   (FP),Y
   BEQ   .cmp_true@267
.thread@58:
   LDA   #$00
   JMP   .cmp_end@268
.cmp_true@267:
   LDA   #$01
.cmp_end@268:
   LDY   #$1E
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
   LDY   #$20
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@92:
   TAX
   LDY   #$20
   LDA   (FP),Y

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$2C
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$22
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

test_has_two_unions:
   SUBROUTINE

   ; prologue: 15 arg bytes, 28 local bytes
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

   CLC
   LDA   FP
   ADC   #$1F
   LDY   #$03
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   LDA   #<str@8
   LDY   #$01
   STA   (FP),Y
   LDA   #>str@8
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
   LDY   #$04
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   DEY
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   DEY
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   JSR   strcmp
   LDY   #$05
   STA   (FP),Y
   TXA
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@93
   LDX   #$00
   TXA

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$2D
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
.if_end@93:
   CLC
   LDA   FP
   ADC   #$1F
   LDY   #$07
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$07
   LDY   #$09
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
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
   LDY   #$04
   LDA   (DPTR),Y
   LDY   #$0F
   STA   (FP),Y
   LDY   #$05
   LDA   (DPTR),Y
   LDY   #$10
   STA   (FP),Y
   LDY   #$06
   LDA   (DPTR),Y
   LDY   #$11
   STA   (FP),Y
   LDY   #$07
   LDA   (DPTR),Y
   LDY   #$12
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
   LDA   #$14
   INY
   STA   (FP),Y
   LDA   #$E4
   INY
   STA   (FP),Y
   LDA   #$41
   INY
   STA   (FP),Y
   EOR   #$80
   INY
   STA   (FP),Y
   LDY   #$12
   LDA   (FP),Y
   LDY   #$1A
   CMP   (FP),Y
   BNE   .cmp_true@270
   LDY   #$11
   LDA   (FP),Y
   LDY   #$19
   CMP   (FP),Y
   BNE   .cmp_true@270
   LDY   #$10
   LDA   (FP),Y
   LDY   #$18
   CMP   (FP),Y
   BNE   .cmp_true@270
   LDY   #$0F
   LDA   (FP),Y
   LDY   #$17
   CMP   (FP),Y
   BNE   .cmp_true@270
   LDY   #$0E
   LDA   (FP),Y
   LDY   #$16
   CMP   (FP),Y
   BNE   .cmp_true@270
   LDY   #$0D
   LDA   (FP),Y
   LDY   #$15
   CMP   (FP),Y
   BNE   .cmp_true@270
   LDY   #$0C
   LDA   (FP),Y
   LDY   #$14
   CMP   (FP),Y
   BNE   .cmp_true@270
   LDY   #$0B
   LDA   (FP),Y
   LDY   #$13
   CMP   (FP),Y
   BNE   .cmp_true@270
   LDA   #$00
   JMP   .cmp_end@271
.cmp_true@270:
   LDA   #$01
.cmp_end@271:
   LDY   #$1B
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@94
   LDX   #$00
   TXA

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$2D
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
.if_end@94:
   LDX   #$00
   LDA   #$01

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$2D
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

test_small_struct_arr_and_dbl:
   SUBROUTINE

   ; prologue: 16 arg bytes, 50 local bytes
   SEC
   LDA   SSP
   SBC   #$34
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$33
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
   ADC   #$35
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
.mul_loop@273:
   BCC   .mul_skip@274
   CLC
   ADC   #$08
.mul_skip@274:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@273
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
   LDA   #$36
   INY
   STA   (FP),Y
   LDA   #$40
   EOR   #$80
   INY
   STA   (FP),Y
   LDY   #$0E
   LDA   (FP),Y
   LDY   #$16
   CMP   (FP),Y
   BNE   .thread@59
   LDY   #$0D
   LDA   (FP),Y
   LDY   #$15
   CMP   (FP),Y
   BNE   .thread@59
   LDY   #$0C
   LDA   (FP),Y
   LDY   #$14
   CMP   (FP),Y
   BNE   .thread@59
   LDY   #$0B
   LDA   (FP),Y
   LDY   #$13
   CMP   (FP),Y
   BNE   .thread@59
   LDY   #$0A
   LDA   (FP),Y
   LDY   #$12
   CMP   (FP),Y
   BNE   .thread@59
   LDY   #$09
   LDA   (FP),Y
   LDY   #$11
   CMP   (FP),Y
   BNE   .thread@59
   LDY   #$08
   LDA   (FP),Y
   LDY   #$10
   CMP   (FP),Y
   BNE   .thread@59
   LDY   #$07
   LDA   (FP),Y
   LDY   #$0F
   CMP   (FP),Y
   BEQ   .cmp_true@275
.thread@59:
   LDA   #$00
   JMP   .cmp_end@276
.cmp_true@275:
   LDA   #$01
.cmp_end@276:
   LDY   #$17
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@39
   JMP   .and_false@95
.lb_skip@39:
   CLC
   LDA   FP
   ADC   #$35
   LDY   #$1B
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
.mul_loop@278:
   BCC   .mul_skip@279
   CLC
   ADC   #$08
.mul_skip@279:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@278
   LDY   #$1A
   STA   (FP),Y
   LDA   HARGS
   DEY
   STA   (FP),Y
   LDY   #$1B
   LDA   (FP),Y
   CLC
   LDY   #$19
   ADC   (FP),Y
   LDY   #$1D
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$1A
   ADC   (FP),Y
   LDY   #$1E
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$1F
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$20
   STA   (FP),Y
   LDY   #$02
   LDA   (DPTR),Y
   LDY   #$21
   STA   (FP),Y
   LDY   #$03
   LDA   (DPTR),Y
   LDY   #$22
   STA   (FP),Y
   LDY   #$04
   LDA   (DPTR),Y
   LDY   #$23
   STA   (FP),Y
   LDY   #$05
   LDA   (DPTR),Y
   LDY   #$24
   STA   (FP),Y
   LDY   #$06
   LDA   (DPTR),Y
   LDY   #$25
   STA   (FP),Y
   LDY   #$07
   LDA   (DPTR),Y
   LDY   #$26
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
   LDA   #$40
   INY
   STA   (FP),Y
   EOR   #$80
   INY
   STA   (FP),Y
   LDY   #$26
   LDA   (FP),Y
   LDY   #$2E
   CMP   (FP),Y
   BNE   .thread@60
   LDY   #$25
   LDA   (FP),Y
   LDY   #$2D
   CMP   (FP),Y
   BNE   .thread@60
   LDY   #$24
   LDA   (FP),Y
   LDY   #$2C
   CMP   (FP),Y
   BNE   .thread@60
   LDY   #$23
   LDA   (FP),Y
   LDY   #$2B
   CMP   (FP),Y
   BNE   .thread@60
   LDY   #$22
   LDA   (FP),Y
   LDY   #$2A
   CMP   (FP),Y
   BNE   .thread@60
   LDY   #$21
   LDA   (FP),Y
   LDY   #$29
   CMP   (FP),Y
   BNE   .thread@60
   LDY   #$20
   LDA   (FP),Y
   LDY   #$28
   CMP   (FP),Y
   BNE   .thread@60
   LDY   #$1F
   LDA   (FP),Y
   LDY   #$27
   CMP   (FP),Y
   BEQ   .cmp_true@280
.thread@60:
   LDA   #$00
   JMP   .cmp_end@281
.cmp_true@280:
   LDA   #$01
.cmp_end@281:
   LDY   #$2F
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@95
   LDA   #$01
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   JMP   .and_end@96
.and_false@95:
   LDA   #$00
   LDY   #$31
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@96:
   TAX
   LDY   #$31
   LDA   (FP),Y

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$44
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$33
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

test_xmm_and_gp:
   SUBROUTINE

   ; prologue: 10 arg bytes, 32 local bytes
   SEC
   LDA   SSP
   SBC   #$22
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$21
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
   ADC   #$23
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
   LDA   #$20
   INY
   STA   (FP),Y
   LDA   #$40
   EOR   #$80
   INY
   STA   (FP),Y
   LDY   #$0A
   LDA   (FP),Y
   LDY   #$12
   CMP   (FP),Y
   BNE   .thread@61
   LDY   #$09
   LDA   (FP),Y
   LDY   #$11
   CMP   (FP),Y
   BNE   .thread@61
   LDY   #$08
   LDA   (FP),Y
   LDY   #$10
   CMP   (FP),Y
   BNE   .thread@61
   LDY   #$07
   LDA   (FP),Y
   LDY   #$0F
   CMP   (FP),Y
   BNE   .thread@61
   LDY   #$06
   LDA   (FP),Y
   LDY   #$0E
   CMP   (FP),Y
   BNE   .thread@61
   LDY   #$05
   LDA   (FP),Y
   LDY   #$0D
   CMP   (FP),Y
   BNE   .thread@61
   LDY   #$04
   LDA   (FP),Y
   LDY   #$0C
   CMP   (FP),Y
   BNE   .thread@61
   LDY   #$03
   LDA   (FP),Y
   LDY   #$0B
   CMP   (FP),Y
   BEQ   .cmp_true@283
.thread@61:
   LDA   #$00
   JMP   .cmp_end@284
.cmp_true@283:
   LDA   #$01
.cmp_end@284:
   LDY   #$13
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@40
   JMP   .and_false@97
.lb_skip@40:
   CLC
   LDA   FP
   ADC   #$23
   LDY   #$15
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$08
   LDY   #$17
   STA   (FP),Y
   DEY
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
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$19
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$1A
   STA   (FP),Y
   SEC
   LDA   #$00
   SBC   #$08
   INY
   STA   (FP),Y
   LDA   #$00
   SBC   #$00
   INY
   STA   (FP),Y
   LDY   #$1A
   LDA   (FP),Y
   LDY   #$1C
   CMP   (FP),Y
   BNE   .thread@62
   LDY   #$19
   LDA   (FP),Y
   LDY   #$1B
   CMP   (FP),Y
   BEQ   .cmp_true@286
.thread@62:
   LDA   #$00
   JMP   .cmp_end@287
.cmp_true@286:
   LDA   #$01
.cmp_end@287:
   LDY   #$1D
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@97
   LDA   #$01
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   JMP   .and_end@98
.and_false@97:
   LDA   #$00
   LDY   #$1F
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@98:
   TAX
   LDY   #$1F
   LDA   (FP),Y

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$2C
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$21
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

test_xmm_and_gp_nested:
   SUBROUTINE

   ; prologue: 16 arg bytes, 32 local bytes
   SEC
   LDA   SSP
   SBC   #$22
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$21
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
   ADC   #$23
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
   LDA   #$20
   INY
   STA   (FP),Y
   LDA   #$40
   EOR   #$80
   INY
   STA   (FP),Y
   LDY   #$0A
   LDA   (FP),Y
   LDY   #$12
   CMP   (FP),Y
   BNE   .thread@63
   LDY   #$09
   LDA   (FP),Y
   LDY   #$11
   CMP   (FP),Y
   BNE   .thread@63
   LDY   #$08
   LDA   (FP),Y
   LDY   #$10
   CMP   (FP),Y
   BNE   .thread@63
   LDY   #$07
   LDA   (FP),Y
   LDY   #$0F
   CMP   (FP),Y
   BNE   .thread@63
   LDY   #$06
   LDA   (FP),Y
   LDY   #$0E
   CMP   (FP),Y
   BNE   .thread@63
   LDY   #$05
   LDA   (FP),Y
   LDY   #$0D
   CMP   (FP),Y
   BNE   .thread@63
   LDY   #$04
   LDA   (FP),Y
   LDY   #$0C
   CMP   (FP),Y
   BNE   .thread@63
   LDY   #$03
   LDA   (FP),Y
   LDY   #$0B
   CMP   (FP),Y
   BEQ   .cmp_true@289
.thread@63:
   LDA   #$00
   JMP   .cmp_end@290
.cmp_true@289:
   LDA   #$01
.cmp_end@290:
   LDY   #$13
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@41
   JMP   .and_false@99
.lb_skip@41:
   CLC
   LDA   FP
   ADC   #$23
   LDY   #$15
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$08
   LDY   #$17
   STA   (FP),Y
   DEY
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
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$19
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$1A
   STA   (FP),Y
   SEC
   LDA   #$00
   SBC   #$08
   INY
   STA   (FP),Y
   LDA   #$00
   SBC   #$00
   INY
   STA   (FP),Y
   LDY   #$1A
   LDA   (FP),Y
   LDY   #$1C
   CMP   (FP),Y
   BNE   .thread@64
   LDY   #$19
   LDA   (FP),Y
   LDY   #$1B
   CMP   (FP),Y
   BEQ   .cmp_true@292
.thread@64:
   LDA   #$00
   JMP   .cmp_end@293
.cmp_true@292:
   LDA   #$01
.cmp_end@293:
   LDY   #$1D
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
   LDY   #$1F
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@100:
   TAX
   LDY   #$1F
   LDA   (FP),Y

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$32
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$21
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

test_lotsa_doubles:
   SUBROUTINE

   ; prologue: 24 arg bytes, 52 local bytes
   SEC
   LDA   SSP
   SBC   #$36
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$35
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
   ADC   #$37
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
.mul_loop@295:
   BCC   .mul_skip@296
   CLC
   ADC   #$08
.mul_skip@296:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@295
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
   BNE   .thread@65
   DEY
   LDA   (FP),Y
   CMP   #$58
   BNE   .thread@65
   DEY
   LDA   (FP),Y
   CMP   #$C0
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
   BEQ   .cmp_true@297
.thread@65:
   LDA   #$00
   JMP   .cmp_end@298
.cmp_true@297:
   LDA   #$01
.cmp_end@298:
   LDY   #$0F
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@43
   JMP   .and_false@103
.lb_skip@43:
   CLC
   LDA   FP
   ADC   #$37
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
.mul_loop@300:
   BCC   .mul_skip@301
   CLC
   ADC   #$08
.mul_skip@301:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@300
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
   BNE   .thread@66
   DEY
   LDA   (FP),Y
   CMP   #$58
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
   BEQ   .cmp_true@302
.thread@66:
   LDA   #$00
   JMP   .cmp_end@303
.cmp_true@302:
   LDA   #$01
.cmp_end@303:
   LDY   #$1F
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
   LDY   #$21
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@104:
   LDY   #$21
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@42
   JMP   .and_false@101
.lb_skip@42:
   CLC
   LDA   FP
   ADC   #$37
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
.mul_loop@305:
   BCC   .mul_skip@306
   CLC
   ADC   #$08
.mul_skip@306:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@305
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
   BNE   .thread@67
   DEY
   LDA   (FP),Y
   CMP   #$58
   BNE   .thread@67
   DEY
   LDA   (FP),Y
   CMP   #$40
   BNE   .thread@67
   DEY
   LDA   (FP),Y
   BNE   .thread@67
   DEY
   LDA   (FP),Y
   BNE   .thread@67
   DEY
   LDA   (FP),Y
   BNE   .thread@67
   DEY
   LDA   (FP),Y
   BNE   .thread@67
   DEY
   LDA   (FP),Y
   BEQ   .cmp_true@307
.thread@67:
   LDA   #$00
   JMP   .cmp_end@308
.cmp_true@307:
   LDA   #$01
.cmp_end@308:
   LDY   #$31
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
   LDY   #$33
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@102:
   TAX
   LDY   #$33
   LDA   (FP),Y

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$4E
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$35
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

test_lotsa_chars:
   SUBROUTINE

   ; prologue: 18 arg bytes, 8 local bytes
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

   CLC
   LDA   FP
   ADC   #$0B
   LDY   #$03
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   LDA   #<str@9
   LDY   #$01
   STA   (FP),Y
   LDA   #>str@9
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
   LDY   #$04
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   DEY
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   DEY
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   JSR   strcmp
   LDY   #$05
   STA   (FP),Y
   TXA
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lnot_true@310
   LDA   #$00
   JMP   .lnot_end@311
.lnot_true@310:
   LDA   #$01
.lnot_end@311:
   LDY   #$07
   STA   (FP),Y
   LDA   #$00
   INY
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
   ADC   #$1C
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

test_contains_large_struct:
   SUBROUTINE

   ; prologue: 20 arg bytes, 34 local bytes
   SEC
   LDA   SSP
   SBC   #$24
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$23
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
   ADC   #$25
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
   CMP   #$00
   BNE   .thread@68
   DEY
   LDA   (FP),Y
   CMP   #$64
   BEQ   .cmp_true@312
.thread@68:
   LDA   #$00
   JMP   .cmp_end@313
.cmp_true@312:
   LDA   #$01
.cmp_end@313:
   LDY   #$05
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@45
   JMP   .and_false@107
.lb_skip@45:
   CLC
   LDA   FP
   ADC   #$25
   LDY   #$07
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$02
   LDY   #$09
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
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
   LDY   #$04
   LDA   (DPTR),Y
   LDY   #$0F
   STA   (FP),Y
   LDY   #$05
   LDA   (DPTR),Y
   LDY   #$10
   STA   (FP),Y
   LDY   #$06
   LDA   (DPTR),Y
   LDY   #$11
   STA   (FP),Y
   LDY   #$07
   LDA   (DPTR),Y
   LDY   #$12
   STA   (FP),Y
   CMP   #$40
   BNE   .thread@69
   DEY
   LDA   (FP),Y
   CMP   #$59
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
   BEQ   .cmp_true@315
.thread@69:
   LDA   #$00
   JMP   .cmp_end@316
.cmp_true@315:
   LDA   #$01
.cmp_end@316:
   LDY   #$13
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@107
   LDA   #$01
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   JMP   .and_end@108
.and_false@107:
   LDA   #$00
   LDY   #$15
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@108:
   LDY   #$15
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@44
   JMP   .and_false@105
.lb_skip@44:
   CLC
   LDA   FP
   ADC   #$25
   LDY   #$17
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$0A
   LDY   #$19
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$1A
   STA   (FP),Y
   LDA   #<str@10
   INY
   STA   (FP),Y
   LDA   #>str@10
   INY
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$04
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDY   #$19
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$1A
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$1B
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$1C
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   JSR   strcmp
   LDY   #$1D
   STA   (FP),Y
   TXA
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lnot_true@318
   LDA   #$00
   JMP   .lnot_end@319
.lnot_true@318:
   LDA   #$01
.lnot_end@319:
   LDY   #$1F
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
   LDY   #$21
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@106:
   TAX
   LDY   #$21
   LDA   (FP),Y

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$38
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$23
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

test_contains_union_array:
   SUBROUTINE

   ; prologue: 32 arg bytes, 145 local bytes
   SEC
   LDA   SSP
   SBC   #$93
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$92
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
   ADC   #$94
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
.mul_loop@320:
   BCC   .mul_skip@321
   CLC
   ADC   #$10
.mul_skip@321:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@320
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
   LDY   #$08
   LDA   (DPTR),Y
   LDY   #$0F
   STA   (FP),Y
   LDY   #$09
   LDA   (DPTR),Y
   LDY   #$10
   STA   (FP),Y
   LDY   #$0A
   LDA   (DPTR),Y
   LDY   #$11
   STA   (FP),Y
   LDY   #$0B
   LDA   (DPTR),Y
   LDY   #$12
   STA   (FP),Y
   LDY   #$0C
   LDA   (DPTR),Y
   LDY   #$13
   STA   (FP),Y
   LDY   #$0D
   LDA   (DPTR),Y
   LDY   #$14
   STA   (FP),Y
   LDY   #$0E
   LDA   (DPTR),Y
   LDY   #$15
   STA   (FP),Y
   LDY   #$0F
   LDA   (DPTR),Y
   LDY   #$16
   STA   (FP),Y
   LDY   #$07
   LDA   (FP),Y
   LDY   #$17
   STA   (FP),Y
   LDY   #$08
   LDA   (FP),Y
   LDY   #$18
   STA   (FP),Y
   LDY   #$09
   LDA   (FP),Y
   LDY   #$19
   STA   (FP),Y
   LDY   #$0A
   LDA   (FP),Y
   LDY   #$1A
   STA   (FP),Y
   LDY   #$0B
   LDA   (FP),Y
   LDY   #$1B
   STA   (FP),Y
   LDY   #$0C
   LDA   (FP),Y
   LDY   #$1C
   STA   (FP),Y
   LDY   #$0D
   LDA   (FP),Y
   LDY   #$1D
   STA   (FP),Y
   LDY   #$0E
   LDA   (FP),Y
   LDY   #$1E
   STA   (FP),Y
   LDY   #$0F
   LDA   (FP),Y
   LDY   #$1F
   STA   (FP),Y
   LDY   #$10
   LDA   (FP),Y
   LDY   #$20
   STA   (FP),Y
   LDY   #$11
   LDA   (FP),Y
   LDY   #$21
   STA   (FP),Y
   LDY   #$12
   LDA   (FP),Y
   LDY   #$22
   STA   (FP),Y
   LDY   #$13
   LDA   (FP),Y
   LDY   #$23
   STA   (FP),Y
   LDY   #$14
   LDA   (FP),Y
   LDY   #$24
   STA   (FP),Y
   LDY   #$15
   LDA   (FP),Y
   LDY   #$25
   STA   (FP),Y
   LDY   #$16
   LDA   (FP),Y
   LDY   #$26
   STA   (FP),Y
   CLC
   LDA   FP
   ADC   #$94
   LDY   #$29
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
.mul_loop@322:
   BCC   .mul_skip@323
   CLC
   ADC   #$10
.mul_skip@323:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@322
   LDY   #$28
   STA   (FP),Y
   LDA   HARGS
   DEY
   STA   (FP),Y
   LDY   #$29
   LDA   (FP),Y
   CLC
   LDY   #$27
   ADC   (FP),Y
   LDY   #$2B
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$28
   ADC   (FP),Y
   LDY   #$2C
   STA   (FP),Y
   DEY
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
   LDY   #$08
   LDA   (DPTR),Y
   LDY   #$35
   STA   (FP),Y
   LDY   #$09
   LDA   (DPTR),Y
   LDY   #$36
   STA   (FP),Y
   LDY   #$0A
   LDA   (DPTR),Y
   LDY   #$37
   STA   (FP),Y
   LDY   #$0B
   LDA   (DPTR),Y
   LDY   #$38
   STA   (FP),Y
   LDY   #$0C
   LDA   (DPTR),Y
   LDY   #$39
   STA   (FP),Y
   LDY   #$0D
   LDA   (DPTR),Y
   LDY   #$3A
   STA   (FP),Y
   LDY   #$0E
   LDA   (DPTR),Y
   LDY   #$3B
   STA   (FP),Y
   LDY   #$0F
   LDA   (DPTR),Y
   LDY   #$3C
   STA   (FP),Y
   LDY   #$2D
   LDA   (FP),Y
   LDY   #$3D
   STA   (FP),Y
   LDY   #$2E
   LDA   (FP),Y
   LDY   #$3E
   STA   (FP),Y
   LDY   #$2F
   LDA   (FP),Y
   LDY   #$3F
   STA   (FP),Y
   LDY   #$30
   LDA   (FP),Y
   LDY   #$40
   STA   (FP),Y
   LDY   #$31
   LDA   (FP),Y
   LDY   #$41
   STA   (FP),Y
   LDY   #$32
   LDA   (FP),Y
   LDY   #$42
   STA   (FP),Y
   LDY   #$33
   LDA   (FP),Y
   LDY   #$43
   STA   (FP),Y
   LDY   #$34
   LDA   (FP),Y
   LDY   #$44
   STA   (FP),Y
   LDY   #$35
   LDA   (FP),Y
   LDY   #$45
   STA   (FP),Y
   LDY   #$36
   LDA   (FP),Y
   LDY   #$46
   STA   (FP),Y
   LDY   #$37
   LDA   (FP),Y
   LDY   #$47
   STA   (FP),Y
   LDY   #$38
   LDA   (FP),Y
   LDY   #$48
   STA   (FP),Y
   LDY   #$39
   LDA   (FP),Y
   LDY   #$49
   STA   (FP),Y
   LDY   #$3A
   LDA   (FP),Y
   LDY   #$4A
   STA   (FP),Y
   LDY   #$3B
   LDA   (FP),Y
   LDY   #$4B
   STA   (FP),Y
   LDY   #$3C
   LDA   (FP),Y
   LDY   #$4C
   STA   (FP),Y
   CLC
   LDA   FP
   ADC   #$17
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
.mul_loop@324:
   BCC   .mul_skip@325
   CLC
   ADC   #$08
.mul_skip@325:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@324
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
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$53
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$54
   STA   (FP),Y
   LDY   #$02
   LDA   (DPTR),Y
   LDY   #$55
   STA   (FP),Y
   LDY   #$03
   LDA   (DPTR),Y
   LDY   #$56
   STA   (FP),Y
   LDY   #$04
   LDA   (DPTR),Y
   LDY   #$57
   STA   (FP),Y
   LDY   #$05
   LDA   (DPTR),Y
   LDY   #$58
   STA   (FP),Y
   LDY   #$06
   LDA   (DPTR),Y
   LDY   #$59
   STA   (FP),Y
   LDY   #$07
   LDA   (DPTR),Y
   LDY   #$5A
   STA   (FP),Y
   CMP   #$40
   BNE   .cmp_true@326
   DEY
   LDA   (FP),Y
   CMP   #$26
   BNE   .cmp_true@326
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@326
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@326
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@326
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@326
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@326
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@326
   LDA   #$00
   JMP   .cmp_end@327
.cmp_true@326:
   LDA   #$01
.cmp_end@327:
   LDY   #$5B
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@46
   JMP   .or_true@109
.lb_skip@46:
   CLC
   LDA   FP
   ADC   #$17
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
.mul_loop@329:
   BCC   .mul_skip@330
   CLC
   ADC   #$08
.mul_skip@330:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@329
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
   CMP   #$40
   BNE   .cmp_true@331
   DEY
   LDA   (FP),Y
   CMP   #$28
   BNE   .cmp_true@331
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@331
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@331
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@331
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@331
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@331
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@331
   LDA   #$00
   JMP   .cmp_end@332
.cmp_true@331:
   LDA   #$01
.cmp_end@332:
   LDY   #$6B
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@109
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   JMP   .or_end@110
.or_true@109:
   LDA   #$01
   LDY   #$6D
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@110:
   LDY   #$6D
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
   ADC   #$B3
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$92
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
   ADC   #$3D
   LDY   #$71
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
.mul_loop@334:
   BCC   .mul_skip@335
   CLC
   ADC   #$08
.mul_skip@335:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@334
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
   STA   HARGS
   INY
   LDA   (FP),Y
   STA   HARGS+1
   JSR   i2d
   LDA   HARGS+2
   LDY   #$7F
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
   LDY   #$7C
   LDA   (FP),Y
   LDY   #$86
   CMP   (FP),Y
   BNE   .cmp_true@336
   LDY   #$7B
   LDA   (FP),Y
   LDY   #$85
   CMP   (FP),Y
   BNE   .cmp_true@336
   LDY   #$7A
   LDA   (FP),Y
   LDY   #$84
   CMP   (FP),Y
   BNE   .cmp_true@336
   LDY   #$79
   LDA   (FP),Y
   LDY   #$83
   CMP   (FP),Y
   BNE   .cmp_true@336
   LDY   #$78
   LDA   (FP),Y
   LDY   #$82
   CMP   (FP),Y
   BNE   .cmp_true@336
   LDY   #$77
   LDA   (FP),Y
   LDY   #$81
   CMP   (FP),Y
   BNE   .cmp_true@336
   LDY   #$76
   LDA   (FP),Y
   LDY   #$80
   CMP   (FP),Y
   BNE   .cmp_true@336
   LDY   #$75
   LDA   (FP),Y
   LDY   #$7F
   CMP   (FP),Y
   BNE   .cmp_true@336
   LDA   #$00
   JMP   .cmp_end@337
.cmp_true@336:
   LDA   #$01
.cmp_end@337:
   LDY   #$87
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@112
   CLC
   LDA   FP
   ADC   #$3D
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
   LDY   #$8B
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@339
   LDA   #$00
   JMP   .cmp_end@340
.cmp_true@339:
   LDA   #$01
.cmp_end@340:
   LDY   #$8E
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@112
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   JMP   .or_end@113
.or_true@112:
   LDA   #$01
   LDY   #$90
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@113:
   LDY   #$90
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@114
   LDX   #$00
   TXA

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$B3
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
.if_end@114:
   LDX   #$00
   LDA   #$01

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$B3
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$92
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
   DC.B  $57, $58, $59, $5A, $00

str@1:
   DC.B  $43, $68, $61, $72, $73, $21, $00

str@2:
   DC.B  $21, $40, $23, $24, $00

str@3:
   DC.B  $51, $57, $45, $52, $00

str@4:
   DC.B  $54, $59, $55, $49, $00

str@5:
   DC.B  $41, $53, $00

str@6:
   DC.B  $44, $46, $00

str@7:
   DC.B  $47, $48, $00

str@8:
   DC.B  $57, $58, $59, $5A, $00

str@9:
   DC.B  $61, $73, $66, $6C, $61, $6B, $6A, $73, $64, $66, $6C, $6B, $6A, $73, $00

str@10:
   DC.B  $41, $20, $73, $74, $72, $75, $63, $74, $21, $00
