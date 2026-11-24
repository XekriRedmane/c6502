SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24

check_args:
   SUBROUTINE

   ; prologue: 16 arg bytes, 22 local bytes
   SEC
   LDA   SSP
   SBC   #$18
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$17
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   LDA   #$02
   LDY   #$01
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   ORA   #$00
   BPL   .sx_done@1
   LDA   #$FF
.sx_done@1:
   LDY   #$03
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
   LDY   #$20
   LDA   (FP),Y
   LDY   #$08
   CMP   (FP),Y
   BNE   .thread@0
   LDY   #$1F
   LDA   (FP),Y
   LDY   #$07
   CMP   (FP),Y
   BNE   .thread@0
   LDY   #$1E
   LDA   (FP),Y
   LDY   #$06
   CMP   (FP),Y
   BNE   .thread@0
   LDY   #$1D
   LDA   (FP),Y
   LDY   #$05
   CMP   (FP),Y
   BNE   .thread@0
   LDY   #$1C
   LDA   (FP),Y
   LDY   #$04
   CMP   (FP),Y
   BNE   .thread@0
   LDY   #$1B
   LDA   (FP),Y
   LDY   #$03
   CMP   (FP),Y
   BNE   .thread@0
   LDY   #$1A
   LDA   (FP),Y
   LDY   #$02
   CMP   (FP),Y
   BNE   .thread@0
   LDY   #$19
   LDA   (FP),Y
   LDY   #$01
   CMP   (FP),Y
   BEQ   .cmp_true@2
.thread@0:
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
   BNE   .lb_skip@0
   JMP   .and_false@0
.lb_skip@0:
   LDA   #$00
   LDY   #$0B
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
   LDA   #$18
   INY
   STA   (FP),Y
   LDA   #$40
   EOR   #$80
   INY
   STA   (FP),Y
   LDY   #$28
   LDA   (FP),Y
   LDY   #$12
   CMP   (FP),Y
   BNE   .thread@1
   LDY   #$27
   LDA   (FP),Y
   LDY   #$11
   CMP   (FP),Y
   BNE   .thread@1
   LDY   #$26
   LDA   (FP),Y
   LDY   #$10
   CMP   (FP),Y
   BNE   .thread@1
   LDY   #$25
   LDA   (FP),Y
   LDY   #$0F
   CMP   (FP),Y
   BNE   .thread@1
   LDY   #$24
   LDA   (FP),Y
   LDY   #$0E
   CMP   (FP),Y
   BNE   .thread@1
   LDY   #$23
   LDA   (FP),Y
   LDY   #$0D
   CMP   (FP),Y
   BNE   .thread@1
   LDY   #$22
   LDA   (FP),Y
   LDY   #$0C
   CMP   (FP),Y
   BNE   .thread@1
   LDY   #$21
   LDA   (FP),Y
   LDY   #$0B
   CMP   (FP),Y
   BEQ   .cmp_true@5
.thread@1:
   LDA   #$00
   JMP   .cmp_end@6
.cmp_true@5:
   LDA   #$01
.cmp_end@6:
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
   TAX
   LDY   #$15
   LDA   (FP),Y

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$28
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$17
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

return_double:
   SUBROUTINE

   LDA   #$00
   STA   HARGS+16
   LDY   #$00
   STY   HARGS+17
   STY   HARGS+18
   STA   HARGS+19
   LDA   #$65
   STA   HARGS+20
   LDA   #$CD
   STA   HARGS+21
   LDA   #$ED
   STA   HARGS+22
   LDA   #$41
   STA   HARGS+23
   RTS

check_assignment:
   SUBROUTINE

   ; prologue: 8 arg bytes, 6 local bytes
   SEC
   LDA   SSP
   SBC   #$08
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$07
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
   LDY   #$09
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
   JSR   d2i
   LDA   HARGS+8
   LDY   #$03
   STA   (FP),Y
   LDA   HARGS+9
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   LDY   #$04
   LDA   (FP),Y
   LDY   #$02
   STA   (FP),Y
   CMP   #$00
   BNE   .thread@2
   DEY
   LDA   (FP),Y
   CMP   #$04
   BEQ   .cmp_true@8
.thread@2:
   LDA   #$00
   JMP   .cmp_end@9
.cmp_true@8:
   LDA   #$01
.cmp_end@9:
   LDY   #$05
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
   ADC   #$10
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$07
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

   ; prologue: 0 arg bytes, 38 local bytes
   SEC
   LDA   SSP
   SBC   #$28
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$27
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
   SBC   #$06
   LDY   #$01
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
   LDY   #$03
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
   SEC
   LDA   SSP
   SBC   #$10
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   #$02
   LDY   #$01
   STA   (SSP),Y
   LDA   #$00
   INY
   STA   (SSP),Y
   INY
   STA   (SSP),Y
   INY
   STA   (SSP),Y
   INY
   STA   (SSP),Y
   INY
   STA   (SSP),Y
   INY
   STA   (SSP),Y
   INY
   STA   (SSP),Y
   LDY   #$03
   LDA   (FP),Y
   LDY   #$09
   STA   (SSP),Y
   LDY   #$04
   LDA   (FP),Y
   LDY   #$0A
   STA   (SSP),Y
   LDY   #$05
   LDA   (FP),Y
   LDY   #$0B
   STA   (SSP),Y
   LDY   #$06
   LDA   (FP),Y
   LDY   #$0C
   STA   (SSP),Y
   LDY   #$07
   LDA   (FP),Y
   LDY   #$0D
   STA   (SSP),Y
   LDY   #$08
   LDA   (FP),Y
   LDY   #$0E
   STA   (SSP),Y
   LDY   #$09
   LDA   (FP),Y
   LDY   #$0F
   STA   (SSP),Y
   LDY   #$0A
   LDA   (FP),Y
   LDY   #$10
   STA   (SSP),Y
   JSR   check_args
   LDY   #$0B
   STA   (FP),Y
   TXA
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lnot_true@11
   LDA   #$00
   JMP   .lnot_end@12
.lnot_true@11:
   LDA   #$01
.lnot_end@12:
   LDY   #$0D
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
   LDA   #$01

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$28
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$27
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
   JSR   return_double
   LDA   HARGS+16
   LDY   #$0F
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
   CMP   #$41
   BNE   .cmp_true@13
   DEY
   LDA   (FP),Y
   CMP   #$ED
   BNE   .cmp_true@13
   DEY
   LDA   (FP),Y
   CMP   #$CD
   BNE   .cmp_true@13
   DEY
   LDA   (FP),Y
   CMP   #$65
   BNE   .cmp_true@13
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@13
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@13
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@13
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@13
   LDA   #$00
   JMP   .cmp_end@14
.cmp_true@13:
   LDA   #$01
.cmp_end@14:
   LDY   #$17
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
   LDA   #$02

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$28
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$27
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
   SEC
   LDA   SSP
   SBC   #$08
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   #$9A
   LDY   #$01
   STA   (SSP),Y
   LDA   #$99
   INY
   STA   (SSP),Y
   INY
   STA   (SSP),Y
   INY
   STA   (SSP),Y
   INY
   STA   (SSP),Y
   INY
   STA   (SSP),Y
   LDA   #$13
   INY
   STA   (SSP),Y
   LDA   #$40
   INY
   STA   (SSP),Y
   JSR   check_assignment
   LDY   #$19
   STA   (FP),Y
   TXA
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lnot_true@16
   LDA   #$00
   JMP   .lnot_end@17
.lnot_true@16:
   LDA   #$01
.lnot_end@17:
   LDY   #$1B
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
   ADC   #$28
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$27
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
   LDY   #$1D
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$65
   INY
   STA   (FP),Y
   LDA   #$CD
   INY
   STA   (FP),Y
   LDA   #$ED
   INY
   STA   (FP),Y
   LDA   #$41
   INY
   STA   (FP),Y
   CMP   #$41
   BNE   .cmp_true@18
   DEY
   LDA   (FP),Y
   CMP   #$ED
   BNE   .cmp_true@18
   DEY
   LDA   (FP),Y
   CMP   #$CD
   BNE   .cmp_true@18
   DEY
   LDA   (FP),Y
   CMP   #$65
   BNE   .cmp_true@18
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@18
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@18
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@18
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@18
   LDA   #$00
   JMP   .cmp_end@19
.cmp_true@18:
   LDA   #$01
.cmp_end@19:
   LDY   #$25
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
   LDA   #$04

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$28
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
.if_end@5:
   LDX   #$00
   TXA

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$28
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$27
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
