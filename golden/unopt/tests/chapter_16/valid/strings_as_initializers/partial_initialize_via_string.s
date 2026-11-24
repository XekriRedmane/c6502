SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24

test_static:
   SUBROUTINE

   ; prologue: 0 arg bytes, 23 local bytes
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

   LDA   static_arr
   LDY   #$01
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CMP   #$68
   BEQ   .cmp_true@0
   LDA   #$00
   JMP   .cmp_end@1
.cmp_true@0:
   LDA   #$01
.cmp_end@1:
   LDY   #$04
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@2
   LDA   static_arr+1
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CMP   #$69
   BEQ   .cmp_true@2
   LDA   #$00
   JMP   .cmp_end@3
.cmp_true@2:
   LDA   #$01
.cmp_end@3:
   LDY   #$09
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
   LDY   #$0B
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@3:
   LDY   #$0B
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@0
   JMP   .and_false@0
.lb_skip@0:
   LDA   static_arr+2
   LDY   #$0D
   STA   (FP),Y
   ORA   #$00
   BNE   .or_true@6
   LDA   static_arr+3
   INY
   STA   (FP),Y
   ORA   #$00
   BNE   .or_true@6
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   JMP   .or_end@7
.or_true@6:
   LDA   #$01
   LDY   #$0F
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@7:
   LDY   #$0F
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@4
   LDA   static_arr+4
   INY
   STA   (FP),Y
   ORA   #$00
   BNE   .or_true@4
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   JMP   .or_end@5
.or_true@4:
   LDA   #$01
   LDY   #$12
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@5:
   LDY   #$12
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lnot_true@4
   LDA   #$00
   JMP   .lnot_end@5
.lnot_true@4:
   LDA   #$01
.lnot_end@5:
   LDY   #$14
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
   LDY   #$16
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@1:
   TAX
   LDY   #$16
   LDA   (FP),Y

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$19
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

test_static_nested:
   SUBROUTINE

   ; prologue: 0 arg bytes, 49 local bytes
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

   LDA   #$00
   LDY   #$01
   STA   (FP),Y
   INY
   STA   (FP),Y
.loop@0_start:
   LDY   #$01
   LDA   (FP),Y
   SEC
   SBC   #$03
   INY
   LDA   (FP),Y
   SBC   #$00
   BVC   .cmp_novf@6
   EOR   #$80
.cmp_novf@6:
   ASL
   LDA   #$00
   ROL
   LDY   #$03
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@2
   JMP   .loop@0_break
.lb_skip@2:
   LDA   #$00
   LDY   #$05
   STA   (FP),Y
   INY
   STA   (FP),Y
.loop@1_start:
   LDY   #$05
   LDA   (FP),Y
   SEC
   SBC   #$04
   INY
   LDA   (FP),Y
   SBC   #$00
   BVC   .cmp_novf@7
   EOR   #$80
.cmp_novf@7:
   ASL
   LDA   #$00
   ROL
   LDY   #$07
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@1
   JMP   .loop@1_break
.lb_skip@1:
   LDA   #<nested_static_arr
   LDY   #$09
   STA   (FP),Y
   LDA   #>nested_static_arr
   INY
   STA   (FP),Y
   LDY   #$01
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
   LDY   #$09
   LDA   (FP),Y
   CLC
   LDY   #$0D
   ADC   (FP),Y
   LDY   #$0F
   STA   (FP),Y
   LDY   #$0A
   LDA   (FP),Y
   LDY   #$0E
   ADC   (FP),Y
   LDY   #$10
   STA   (FP),Y
   LDY   #$05
   LDA   (FP),Y
   LDY   #$11
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$0F
   LDA   (FP),Y
   CLC
   LDY   #$12
   ADC   (FP),Y
   LDY   #$14
   STA   (FP),Y
   LDY   #$10
   LDA   (FP),Y
   LDY   #$13
   ADC   (FP),Y
   LDY   #$15
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$16
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   BNE   .thread@0
   DEY
   LDA   (FP),Y
   CMP   #$01
   BEQ   .cmp_true@8
.thread@0:
   LDA   #$00
   JMP   .cmp_end@9
.cmp_true@8:
   LDA   #$01
.cmp_end@9:
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
   LDY   #$06
   LDA   (FP),Y
   BNE   .thread@1
   DEY
   LDA   (FP),Y
   BEQ   .cmp_true@11
.thread@1:
   LDA   #$00
   JMP   .cmp_end@12
.cmp_true@11:
   LDA   #$01
.cmp_end@12:
   LDY   #$1C
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
   LDY   #$1E
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@9:
   LDY   #$1E
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_else@11
   LDA   #$62
   INY
   STA   (FP),Y
   LDY   #$19
   STA   (FP),Y
   JMP   .if_end@10
.if_else@11:
   LDY   #$02
   LDA   (FP),Y
   BNE   .thread@2
   DEY
   LDA   (FP),Y
   CMP   #$01
   BEQ   .cmp_true@14
.thread@2:
   LDA   #$00
   JMP   .cmp_end@15
.cmp_true@14:
   LDA   #$01
.cmp_end@15:
   LDY   #$21
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@12
   LDY   #$06
   LDA   (FP),Y
   BNE   .thread@3
   DEY
   LDA   (FP),Y
   CMP   #$01
   BEQ   .cmp_true@17
.thread@3:
   LDA   #$00
   JMP   .cmp_end@18
.cmp_true@17:
   LDA   #$01
.cmp_end@18:
   LDY   #$23
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
   LDY   #$25
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@13:
   LDY   #$25
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@14
   LDA   #$63
   INY
   STA   (FP),Y
   LDY   #$19
   STA   (FP),Y
.if_end@14:
.if_end@10:
   LDY   #$17
   LDA   (FP),Y
   LDY   #$28
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@20
   LDA   #$00
   JMP   .sx_done@21
.sx_neg@20:
   LDA   #$FF
.sx_done@21:
   LDY   #$29
   STA   (FP),Y
   LDY   #$19
   LDA   (FP),Y
   LDY   #$2A
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@22
   LDA   #$00
   JMP   .sx_done@23
.sx_neg@22:
   LDA   #$FF
.sx_done@23:
   LDY   #$2B
   STA   (FP),Y
   LDY   #$29
   LDA   (FP),Y
   LDY   #$2B
   CMP   (FP),Y
   BNE   .cmp_true@24
   LDY   #$28
   LDA   (FP),Y
   LDY   #$2A
   CMP   (FP),Y
   BNE   .cmp_true@24
   LDA   #$00
   JMP   .cmp_end@25
.cmp_true@24:
   LDA   #$01
.cmp_end@25:
   LDY   #$2C
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
.if_end@15:
   LDY   #$05
   LDA   (FP),Y
   CLC
   ADC   #$01
   LDY   #$2E
   STA   (FP),Y
   LDY   #$06
   LDA   (FP),Y
   ADC   #$00
   LDY   #$2F
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$05
   STA   (FP),Y
   LDY   #$2F
   LDA   (FP),Y
   LDY   #$06
   STA   (FP),Y
   JMP   .loop@1_start
.loop@1_break:
   LDY   #$01
   LDA   (FP),Y
   CLC
   ADC   #$01
   LDY   #$30
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   ADC   #$00
   LDY   #$31
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   LDY   #$31
   LDA   (FP),Y
   LDY   #$02
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
   ADC   #$33
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

test_automatic:
   SUBROUTINE

   ; prologue: 0 arg bytes, 46 local bytes
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
   ADC   #$01
   LDY   #$05
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
   LDA   #$61
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$05
   LDA   (FP),Y
   CLC
   ADC   #$01
   LDY   #$07
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$08
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDA   #$62
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$05
   LDA   (FP),Y
   CLC
   ADC   #$02
   LDY   #$09
   STA   (FP),Y
   LDY   #$06
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
   LDA   #$00
   LDY   #$00
   STA   (DPTR),Y
   INY
   STA   (DPTR),Y
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
   CMP   #$61
   BEQ   .cmp_true@27
   LDA   #$00
   JMP   .cmp_end@28
.cmp_true@27:
   LDA   #$01
.cmp_end@28:
   LDY   #$12
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@18
   CLC
   LDA   FP
   ADC   #$01
   INY
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$01
   LDY   #$16
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$17
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
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CMP   #$62
   BEQ   .cmp_true@29
   LDA   #$00
   JMP   .cmp_end@30
.cmp_true@29:
   LDA   #$01
.cmp_end@30:
   LDY   #$1B
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
   LDY   #$1D
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@19:
   LDY   #$1D
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@3
   JMP   .and_false@16
.lb_skip@3:
   CLC
   LDA   FP
   ADC   #$01
   LDY   #$1F
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$02
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
   ORA   #$00
   BNE   .or_true@20
   CLC
   LDA   FP
   ADC   #$01
   INY
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$03
   LDY   #$26
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$27
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$28
   STA   (FP),Y
   ORA   #$00
   BNE   .or_true@20
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   JMP   .or_end@21
.or_true@20:
   LDA   #$01
   LDY   #$29
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@21:
   LDY   #$29
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lnot_true@31
   LDA   #$00
   JMP   .lnot_end@32
.lnot_true@31:
   LDA   #$01
.lnot_end@32:
   LDY   #$2B
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
   LDY   #$2D
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@17:
   TAX
   LDY   #$2D
   LDA   (FP),Y

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$30
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

test_automatic_nested:
   SUBROUTINE

   ; prologue: 0 arg bytes, 134 local bytes
   SEC
   LDA   SSP
   SBC   #$88
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$87
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
   LDA   #$66
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$11
   LDA   (FP),Y
   CLC
   ADC   #$01
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
   LDA   #$6F
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$11
   LDA   (FP),Y
   CLC
   ADC   #$02
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
   LDA   #$6F
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$11
   LDA   (FP),Y
   CLC
   ADC   #$03
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
   LDA   #$00
   LDY   #$00
   STA   (DPTR),Y
   INY
   STA   (DPTR),Y
   INY
   STA   (DPTR),Y
   INY
   STA   (DPTR),Y
   LDY   #$11
   LDA   (FP),Y
   CLC
   ADC   #$07
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
   LDA   #$00
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$11
   LDA   (FP),Y
   CLC
   ADC   #$08
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
   LDA   #$78
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$11
   LDA   (FP),Y
   CLC
   ADC   #$09
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
   LDA   #$00
   LDY   #$00
   STA   (DPTR),Y
   INY
   STA   (DPTR),Y
   LDY   #$11
   LDA   (FP),Y
   CLC
   ADC   #$0B
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
   LDA   #$00
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$11
   LDA   (FP),Y
   CLC
   ADC   #$0C
   LDY   #$21
   STA   (FP),Y
   LDY   #$12
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
   LDA   #$79
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$11
   LDA   (FP),Y
   CLC
   ADC   #$0D
   LDY   #$23
   STA   (FP),Y
   LDY   #$12
   LDA   (FP),Y
   ADC   #$00
   LDY   #$24
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDA   #$7A
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$11
   LDA   (FP),Y
   CLC
   ADC   #$0E
   LDY   #$25
   STA   (FP),Y
   LDY   #$12
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
   LDA   #$00
   LDY   #$00
   STA   (DPTR),Y
   INY
   STA   (DPTR),Y
   LDY   #$27
   STA   (FP),Y
   INY
   STA   (FP),Y
.loop@2_start:
   LDY   #$27
   LDA   (FP),Y
   SEC
   SBC   #$02
   INY
   LDA   (FP),Y
   SBC   #$00
   BVC   .cmp_novf@33
   EOR   #$80
.cmp_novf@33:
   ASL
   LDA   #$00
   ROL
   LDY   #$29
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@8
   JMP   .loop@2_break
.lb_skip@8:
   LDA   #$00
   LDY   #$2B
   STA   (FP),Y
   INY
   STA   (FP),Y
.loop@3_start:
   LDY   #$2B
   LDA   (FP),Y
   SEC
   SBC   #$02
   INY
   LDA   (FP),Y
   SBC   #$00
   BVC   .cmp_novf@34
   EOR   #$80
.cmp_novf@34:
   ASL
   LDA   #$00
   ROL
   LDY   #$2D
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@7
   JMP   .loop@3_break
.lb_skip@7:
   LDA   #$00
   LDY   #$2F
   STA   (FP),Y
   INY
   STA   (FP),Y
.loop@4_start:
   LDY   #$2F
   LDA   (FP),Y
   SEC
   SBC   #$04
   INY
   LDA   (FP),Y
   SBC   #$00
   BVC   .cmp_novf@35
   EOR   #$80
.cmp_novf@35:
   ASL
   LDA   #$00
   ROL
   LDY   #$31
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@6
   JMP   .loop@4_break
.lb_skip@6:
   CLC
   LDA   FP
   ADC   #$01
   LDY   #$37
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   LDY   #$27
   LDA   (FP),Y
   LDY   #$33
   STA   (FP),Y
   STA   HARGS
   LDA   #$08
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
   LDY   #$2B
   LDA   (FP),Y
   LDY   #$3B
   STA   (FP),Y
   STA   HARGS
   LDA   #$04
   STA   HARGS+1
   JSR   mul8
   LDA   HARGS+2
   LDY   #$3C
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$39
   LDA   (FP),Y
   CLC
   LDY   #$3D
   ADC   (FP),Y
   LDY   #$3F
   STA   (FP),Y
   LDY   #$3A
   LDA   (FP),Y
   LDY   #$3E
   ADC   (FP),Y
   LDY   #$40
   STA   (FP),Y
   LDY   #$2F
   LDA   (FP),Y
   LDY   #$41
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$3F
   LDA   (FP),Y
   CLC
   LDY   #$42
   ADC   (FP),Y
   LDY   #$44
   STA   (FP),Y
   LDY   #$40
   LDA   (FP),Y
   LDY   #$43
   ADC   (FP),Y
   LDY   #$45
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
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$28
   LDA   (FP),Y
   BNE   .thread@4
   DEY
   LDA   (FP),Y
   BEQ   .cmp_true@36
.thread@4:
   LDA   #$00
   JMP   .cmp_end@37
.cmp_true@36:
   LDA   #$01
.cmp_end@37:
   LDY   #$4A
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@22
   LDY   #$2C
   LDA   (FP),Y
   BNE   .thread@5
   DEY
   LDA   (FP),Y
   BEQ   .cmp_true@39
.thread@5:
   LDA   #$00
   JMP   .cmp_end@40
.cmp_true@39:
   LDA   #$01
.cmp_end@40:
   LDY   #$4C
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
   LDY   #$4E
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@23:
   LDY   #$4E
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@5
   JMP   .if_else@25
.lb_skip@5:
   LDY   #$30
   LDA   (FP),Y
   BNE   .thread@6
   DEY
   LDA   (FP),Y
   BEQ   .cmp_true@42
.thread@6:
   LDA   #$00
   JMP   .cmp_end@43
.cmp_true@42:
   LDA   #$01
.cmp_end@43:
   LDY   #$50
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_else@27
   LDA   #$66
   INY
   STA   (FP),Y
   LDY   #$49
   STA   (FP),Y
   JMP   .if_end@24
.if_else@27:
   LDY   #$30
   LDA   (FP),Y
   BNE   .thread@7
   DEY
   LDA   (FP),Y
   CMP   #$01
   BEQ   .cmp_true@45
.thread@7:
   LDA   #$00
   JMP   .cmp_end@46
.cmp_true@45:
   LDA   #$01
.cmp_end@46:
   LDY   #$53
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@28
   LDY   #$30
   LDA   (FP),Y
   BNE   .thread@8
   DEY
   LDA   (FP),Y
   CMP   #$02
   BEQ   .cmp_true@48
.thread@8:
   LDA   #$00
   JMP   .cmp_end@49
.cmp_true@48:
   LDA   #$01
.cmp_end@49:
   LDY   #$55
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@28
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   JMP   .or_end@29
.or_true@28:
   LDA   #$01
   LDY   #$57
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@29:
   LDY   #$57
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@4
   JMP   .if_end@24
.lb_skip@4:
   LDA   #$6F
   LDY   #$59
   STA   (FP),Y
   LDY   #$49
   STA   (FP),Y
   JMP   .if_end@24
.if_else@25:
   LDY   #$28
   LDA   (FP),Y
   BNE   .thread@9
   DEY
   LDA   (FP),Y
   CMP   #$01
   BEQ   .cmp_true@51
.thread@9:
   LDA   #$00
   JMP   .cmp_end@52
.cmp_true@51:
   LDA   #$01
.cmp_end@52:
   LDY   #$5A
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@33
   LDY   #$2C
   LDA   (FP),Y
   BNE   .thread@10
   DEY
   LDA   (FP),Y
   BEQ   .cmp_true@54
.thread@10:
   LDA   #$00
   JMP   .cmp_end@55
.cmp_true@54:
   LDA   #$01
.cmp_end@55:
   LDY   #$5C
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@33
   LDA   #$01
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   JMP   .and_end@34
.and_false@33:
   LDA   #$00
   LDY   #$5E
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@34:
   LDY   #$5E
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@31
   LDY   #$30
   LDA   (FP),Y
   BNE   .thread@11
   DEY
   LDA   (FP),Y
   BEQ   .cmp_true@57
.thread@11:
   LDA   #$00
   JMP   .cmp_end@58
.cmp_true@57:
   LDA   #$01
.cmp_end@58:
   LDY   #$60
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@31
   LDA   #$01
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   JMP   .and_end@32
.and_false@31:
   LDA   #$00
   LDY   #$62
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@32:
   LDY   #$62
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_else@36
   LDA   #$78
   INY
   STA   (FP),Y
   LDY   #$49
   STA   (FP),Y
   JMP   .if_end@35
.if_else@36:
   LDY   #$28
   LDA   (FP),Y
   BNE   .thread@12
   DEY
   LDA   (FP),Y
   CMP   #$01
   BEQ   .cmp_true@60
.thread@12:
   LDA   #$00
   JMP   .cmp_end@61
.cmp_true@60:
   LDA   #$01
.cmp_end@61:
   LDY   #$65
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@39
   LDY   #$2C
   LDA   (FP),Y
   BNE   .thread@13
   DEY
   LDA   (FP),Y
   CMP   #$01
   BEQ   .cmp_true@63
.thread@13:
   LDA   #$00
   JMP   .cmp_end@64
.cmp_true@63:
   LDA   #$01
.cmp_end@64:
   LDY   #$67
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@39
   LDA   #$01
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   JMP   .and_end@40
.and_false@39:
   LDA   #$00
   LDY   #$69
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@40:
   LDY   #$69
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@37
   LDY   #$30
   LDA   (FP),Y
   BNE   .thread@14
   DEY
   LDA   (FP),Y
   BEQ   .cmp_true@66
.thread@14:
   LDA   #$00
   JMP   .cmp_end@67
.cmp_true@66:
   LDA   #$01
.cmp_end@67:
   LDY   #$6B
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@37
   LDA   #$01
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   JMP   .and_end@38
.and_false@37:
   LDA   #$00
   LDY   #$6D
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@38:
   LDY   #$6D
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_else@42
   LDA   #$79
   INY
   STA   (FP),Y
   LDY   #$49
   STA   (FP),Y
   JMP   .if_end@41
.if_else@42:
   LDY   #$28
   LDA   (FP),Y
   BNE   .thread@15
   DEY
   LDA   (FP),Y
   CMP   #$01
   BEQ   .cmp_true@69
.thread@15:
   LDA   #$00
   JMP   .cmp_end@70
.cmp_true@69:
   LDA   #$01
.cmp_end@70:
   LDY   #$70
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@45
   LDY   #$2C
   LDA   (FP),Y
   BNE   .thread@16
   DEY
   LDA   (FP),Y
   CMP   #$01
   BEQ   .cmp_true@72
.thread@16:
   LDA   #$00
   JMP   .cmp_end@73
.cmp_true@72:
   LDA   #$01
.cmp_end@73:
   LDY   #$72
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@45
   LDA   #$01
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   JMP   .and_end@46
.and_false@45:
   LDA   #$00
   LDY   #$74
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@46:
   LDY   #$74
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@43
   LDY   #$30
   LDA   (FP),Y
   BNE   .thread@17
   DEY
   LDA   (FP),Y
   CMP   #$01
   BEQ   .cmp_true@75
.thread@17:
   LDA   #$00
   JMP   .cmp_end@76
.cmp_true@75:
   LDA   #$01
.cmp_end@76:
   LDY   #$76
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
   LDY   #$78
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@44:
   LDY   #$78
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@47
   LDA   #$7A
   INY
   STA   (FP),Y
   LDY   #$49
   STA   (FP),Y
.if_end@47:
.if_end@41:
.if_end@35:
.if_end@24:
   LDY   #$47
   LDA   (FP),Y
   LDY   #$7B
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@78
   LDA   #$00
   JMP   .sx_done@79
.sx_neg@78:
   LDA   #$FF
.sx_done@79:
   LDY   #$7C
   STA   (FP),Y
   LDY   #$49
   LDA   (FP),Y
   LDY   #$7D
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@80
   LDA   #$00
   JMP   .sx_done@81
.sx_neg@80:
   LDA   #$FF
.sx_done@81:
   LDY   #$7E
   STA   (FP),Y
   LDY   #$7C
   LDA   (FP),Y
   LDY   #$7E
   CMP   (FP),Y
   BNE   .cmp_true@82
   LDY   #$7B
   LDA   (FP),Y
   LDY   #$7D
   CMP   (FP),Y
   BNE   .cmp_true@82
   LDA   #$00
   JMP   .cmp_end@83
.cmp_true@82:
   LDA   #$01
.cmp_end@83:
   LDY   #$7F
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
   ADC   #$88
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$87
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
   LDY   #$2F
   LDA   (FP),Y
   CLC
   ADC   #$01
   LDY   #$81
   STA   (FP),Y
   LDY   #$30
   LDA   (FP),Y
   ADC   #$00
   LDY   #$82
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$2F
   STA   (FP),Y
   LDY   #$82
   LDA   (FP),Y
   LDY   #$30
   STA   (FP),Y
   JMP   .loop@4_start
.loop@4_break:
   LDY   #$2B
   LDA   (FP),Y
   CLC
   ADC   #$01
   LDY   #$83
   STA   (FP),Y
   LDY   #$2C
   LDA   (FP),Y
   ADC   #$00
   LDY   #$84
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$2B
   STA   (FP),Y
   LDY   #$84
   LDA   (FP),Y
   LDY   #$2C
   STA   (FP),Y
   JMP   .loop@3_start
.loop@3_break:
   LDY   #$27
   LDA   (FP),Y
   CLC
   ADC   #$01
   LDY   #$85
   STA   (FP),Y
   LDY   #$28
   LDA   (FP),Y
   ADC   #$00
   LDY   #$86
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$27
   STA   (FP),Y
   LDY   #$86
   LDA   (FP),Y
   LDY   #$28
   STA   (FP),Y
   JMP   .loop@2_start
.loop@2_break:
   LDX   #$00
   LDA   #$01

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$88
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$87
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

   ; prologue: 0 arg bytes, 16 local bytes
   SEC
   LDA   SSP
   SBC   #$12
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$11
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   JSR   test_static
   LDY   #$01
   STA   (FP),Y
   TXA
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lnot_true@85
   LDA   #$00
   JMP   .lnot_end@86
.lnot_true@85:
   LDA   #$01
.lnot_end@86:
   LDY   #$03
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@49
   LDX   #$00
   LDA   #$01

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$12
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$11
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
.if_end@49:
   JSR   test_static_nested
   LDY   #$05
   STA   (FP),Y
   TXA
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lnot_true@87
   LDA   #$00
   JMP   .lnot_end@88
.lnot_true@87:
   LDA   #$01
.lnot_end@88:
   LDY   #$07
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@50
   LDX   #$00
   LDA   #$02

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$12
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$11
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
   JSR   test_automatic
   LDY   #$09
   STA   (FP),Y
   TXA
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lnot_true@89
   LDA   #$00
   JMP   .lnot_end@90
.lnot_true@89:
   LDA   #$01
.lnot_end@90:
   LDY   #$0B
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
   LDA   #$03

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$12
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$11
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
   JSR   test_automatic_nested
   LDY   #$0D
   STA   (FP),Y
   TXA
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lnot_true@91
   LDA   #$00
   JMP   .lnot_end@92
.lnot_true@91:
   LDA   #$01
.lnot_end@92:
   LDY   #$0F
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@52
   LDX   #$00
   LDA   #$04

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$12
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
.if_end@52:
   LDX   #$00
   TXA

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$12
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$11
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

static_arr:
   DC.B  $68, $69, $00, $00, $00

nested_static_arr:
   DS.B  4
   DC.B  $62, $63, $00, $00
   DS.B  4
