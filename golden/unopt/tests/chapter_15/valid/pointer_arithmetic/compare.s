SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24

gt:
   SUBROUTINE

   ; prologue: 4 arg bytes, 6 local bytes
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

   INY
   LDA   (FP),Y
   CLC
   LDY   #$0B
   SBC   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$0C
   SBC   (FP),Y
   LDA   #$00
   ROL
   LDY   #$01
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$03
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$04
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@0
   LDA   #$00
   JMP   .sx_done@1
.sx_neg@0:
   LDA   #$FF
.sx_done@1:
   LDY   #$05
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$03
   LDA   (FP),Y
   STA   HARGS+8
   INY
   LDA   (FP),Y
   STA   HARGS+9
   INY
   LDA   (FP),Y
   STA   HARGS+10
   INY
   LDA   (FP),Y
   STA   HARGS+11

   ; epilogue
   CLC
   LDA   FP
   ADC   #$0C
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
   RTS

lt:
   SUBROUTINE

   ; prologue: 4 arg bytes, 6 local bytes
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

   LDY   #$0B
   LDA   (FP),Y
   CLC
   LDY   #$09
   SBC   (FP),Y
   LDY   #$0C
   LDA   (FP),Y
   LDY   #$0A
   SBC   (FP),Y
   LDA   #$00
   ROL
   LDY   #$01
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$03
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$04
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@2
   LDA   #$00
   JMP   .sx_done@3
.sx_neg@2:
   LDA   #$FF
.sx_done@3:
   LDY   #$05
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$03
   LDA   (FP),Y
   STA   HARGS+8
   INY
   LDA   (FP),Y
   STA   HARGS+9
   INY
   LDA   (FP),Y
   STA   HARGS+10
   INY
   LDA   (FP),Y
   STA   HARGS+11

   ; epilogue
   CLC
   LDA   FP
   ADC   #$0C
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
   RTS

ge:
   SUBROUTINE

   ; prologue: 4 arg bytes, 6 local bytes
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

   INY
   LDA   (FP),Y
   SEC
   LDY   #$0B
   SBC   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$0C
   SBC   (FP),Y
   LDA   #$00
   ROL
   LDY   #$01
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$03
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$04
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@4
   LDA   #$00
   JMP   .sx_done@5
.sx_neg@4:
   LDA   #$FF
.sx_done@5:
   LDY   #$05
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$03
   LDA   (FP),Y
   STA   HARGS+8
   INY
   LDA   (FP),Y
   STA   HARGS+9
   INY
   LDA   (FP),Y
   STA   HARGS+10
   INY
   LDA   (FP),Y
   STA   HARGS+11

   ; epilogue
   CLC
   LDA   FP
   ADC   #$0C
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
   RTS

le:
   SUBROUTINE

   ; prologue: 4 arg bytes, 6 local bytes
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

   LDY   #$0B
   LDA   (FP),Y
   SEC
   LDY   #$09
   SBC   (FP),Y
   LDY   #$0C
   LDA   (FP),Y
   LDY   #$0A
   SBC   (FP),Y
   LDA   #$00
   ROL
   LDY   #$01
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$03
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$04
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@6
   LDA   #$00
   JMP   .sx_done@7
.sx_neg@6:
   LDA   #$FF
.sx_done@7:
   LDY   #$05
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$03
   LDA   (FP),Y
   STA   HARGS+8
   INY
   LDA   (FP),Y
   STA   HARGS+9
   INY
   LDA   (FP),Y
   STA   HARGS+10
   INY
   LDA   (FP),Y
   STA   HARGS+11

   ; epilogue
   CLC
   LDA   FP
   ADC   #$0C
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
   RTS

gt_nested:
   SUBROUTINE

   ; prologue: 4 arg bytes, 6 local bytes
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

   INY
   LDA   (FP),Y
   CLC
   LDY   #$0B
   SBC   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$0C
   SBC   (FP),Y
   LDA   #$00
   ROL
   LDY   #$01
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$03
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$04
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@8
   LDA   #$00
   JMP   .sx_done@9
.sx_neg@8:
   LDA   #$FF
.sx_done@9:
   LDY   #$05
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$03
   LDA   (FP),Y
   STA   HARGS+8
   INY
   LDA   (FP),Y
   STA   HARGS+9
   INY
   LDA   (FP),Y
   STA   HARGS+10
   INY
   LDA   (FP),Y
   STA   HARGS+11

   ; epilogue
   CLC
   LDA   FP
   ADC   #$0C
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
   RTS

ge_nested:
   SUBROUTINE

   ; prologue: 4 arg bytes, 6 local bytes
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

   INY
   LDA   (FP),Y
   SEC
   LDY   #$0B
   SBC   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$0C
   SBC   (FP),Y
   LDA   #$00
   ROL
   LDY   #$01
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$03
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$04
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@10
   LDA   #$00
   JMP   .sx_done@11
.sx_neg@10:
   LDA   #$FF
.sx_done@11:
   LDY   #$05
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$03
   LDA   (FP),Y
   STA   HARGS+8
   INY
   LDA   (FP),Y
   STA   HARGS+9
   INY
   LDA   (FP),Y
   STA   HARGS+10
   INY
   LDA   (FP),Y
   STA   HARGS+11

   ; epilogue
   CLC
   LDA   FP
   ADC   #$0C
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
   RTS

main:
   SUBROUTINE

   ; prologue: 0 arg bytes, 226 local bytes
   SEC
   LDA   SSP
   SBC   #$E4
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$E3
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
.mul_loop@12:
   BCC   .mul_skip@13
   CLC
   ADC   #$04
.mul_skip@13:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@12
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
   CLC
   LDA   FP
   ADC   #$01
   LDY   #$1F
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
.mul_loop@14:
   BCC   .mul_skip@15
   CLC
   ADC   #$04
.mul_skip@15:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@14
   LDY   #$1E
   STA   (FP),Y
   LDA   HARGS
   DEY
   STA   (FP),Y
   LDY   #$1F
   LDA   (FP),Y
   CLC
   LDY   #$1D
   ADC   (FP),Y
   LDY   #$21
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$1E
   ADC   (FP),Y
   LDY   #$22
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$23
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$24
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$04
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDY   #$1B
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$1C
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$23
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$24
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   JSR   gt
   LDA   HARGS+8
   LDY   #$25
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
   LDY   #$25
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
   ADC   #$E4
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$E3
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
   SEC
   LDA   SSP
   SBC   #$04
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDY   #$1B
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$1C
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$23
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$24
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   JSR   lt
   LDA   HARGS+8
   LDY   #$29
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
   LDY   #$29
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   INY
   ORA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lnot_true@16
   LDA   #$00
   JMP   .lnot_end@17
.lnot_true@16:
   LDA   #$01
.lnot_end@17:
   LDY   #$2D
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
   ADC   #$E4
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$E3
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
   SEC
   LDA   SSP
   SBC   #$04
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDY   #$1B
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$1C
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
   JSR   ge
   LDA   HARGS+8
   LDY   #$2F
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
   LDY   #$2F
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   INY
   ORA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lnot_true@18
   LDA   #$00
   JMP   .lnot_end@19
.lnot_true@18:
   LDA   #$01
.lnot_end@19:
   LDY   #$33
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
   ADC   #$E4
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$E3
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
   SEC
   LDA   SSP
   SBC   #$04
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDY   #$23
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$24
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
   JSR   le
   LDA   HARGS+8
   LDY   #$35
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
   LDY   #$35
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   INY
   ORA   (FP),Y
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
   ADC   #$E4
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$E3
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
   CLC
   LDA   FP
   ADC   #$01
   LDY   #$3B
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   LDA   #$05
   STA   HARGS
   LDA   #$00
   LDX   #$08
   LSR   HARGS
.mul_loop@20:
   BCC   .mul_skip@21
   CLC
   ADC   #$04
.mul_skip@21:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@20
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
   LDY   #$3F
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$40
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$04
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   DEY
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$40
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$23
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$24
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   JSR   gt
   LDA   HARGS+8
   LDY   #$41
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
   LDY   #$41
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   INY
   ORA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lnot_true@22
   LDA   #$00
   JMP   .lnot_end@23
.lnot_true@22:
   LDA   #$01
.lnot_end@23:
   LDY   #$45
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
   ADC   #$E4
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$E3
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
   LDA   #$01
   STA   HARGS
   LDA   #$00
   LDX   #$08
   LSR   HARGS
.mul_loop@24:
   BCC   .mul_skip@25
   CLC
   ADC   #$04
.mul_skip@25:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@24
   LDY   #$48
   STA   (FP),Y
   LDA   HARGS
   DEY
   STA   (FP),Y
   LDY   #$23
   LDA   (FP),Y
   CLC
   LDY   #$47
   ADC   (FP),Y
   LDY   #$49
   STA   (FP),Y
   LDY   #$24
   LDA   (FP),Y
   LDY   #$48
   ADC   (FP),Y
   LDY   #$4A
   STA   (FP),Y
   LDY   #$40
   LDA   (FP),Y
   LDY   #$4A
   CMP   (FP),Y
   BNE   .cmp_true@26
   LDY   #$3F
   LDA   (FP),Y
   LDY   #$49
   CMP   (FP),Y
   BNE   .cmp_true@26
   LDA   #$00
   JMP   .cmp_end@27
.cmp_true@26:
   LDA   #$01
.cmp_end@27:
   LDY   #$4B
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
   ADC   #$E4
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$E3
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
   ADC   #$4D
   LDY   #$9F
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   LDA   #$03
   STA   HARGS
   LDA   #$00
   LDX   #$08
   LSR   HARGS
.mul_loop@29:
   BCC   .mul_skip@30
   CLC
   ADC   #$14
.mul_skip@30:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@29
   LDY   #$9E
   STA   (FP),Y
   LDA   HARGS
   DEY
   STA   (FP),Y
   LDY   #$9F
   LDA   (FP),Y
   CLC
   LDY   #$9D
   ADC   (FP),Y
   LDY   #$A1
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$9E
   ADC   (FP),Y
   LDY   #$A2
   STA   (FP),Y
   LDA   #$02
   STA   HARGS
   LDA   #$00
   LDX   #$08
   LSR   HARGS
.mul_loop@31:
   BCC   .mul_skip@32
   CLC
   ADC   #$04
.mul_skip@32:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@31
   LDY   #$A4
   STA   (FP),Y
   LDA   HARGS
   DEY
   STA   (FP),Y
   LDY   #$A1
   LDA   (FP),Y
   CLC
   LDY   #$A3
   ADC   (FP),Y
   LDY   #$A5
   STA   (FP),Y
   LDY   #$A2
   LDA   (FP),Y
   LDY   #$A4
   ADC   (FP),Y
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
   CLC
   LDA   FP
   ADC   #$4D
   LDY   #$AB
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   LDA   #$03
   STA   HARGS
   LDA   #$00
   LDX   #$08
   LSR   HARGS
.mul_loop@33:
   BCC   .mul_skip@34
   CLC
   ADC   #$14
.mul_skip@34:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@33
   LDY   #$AA
   STA   (FP),Y
   LDA   HARGS
   DEY
   STA   (FP),Y
   LDY   #$AB
   LDA   (FP),Y
   CLC
   LDY   #$A9
   ADC   (FP),Y
   LDY   #$AD
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$AA
   ADC   (FP),Y
   LDY   #$AE
   STA   (FP),Y
   LDA   #$03
   STA   HARGS
   LDA   #$00
   LDX   #$08
   LSR   HARGS
.mul_loop@35:
   BCC   .mul_skip@36
   CLC
   ADC   #$04
.mul_skip@36:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@35
   LDY   #$B0
   STA   (FP),Y
   LDA   HARGS
   DEY
   STA   (FP),Y
   LDY   #$AD
   LDA   (FP),Y
   CLC
   LDY   #$AF
   ADC   (FP),Y
   LDY   #$B1
   STA   (FP),Y
   LDY   #$AE
   LDA   (FP),Y
   LDY   #$B0
   ADC   (FP),Y
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
   SEC
   LDA   SSP
   SBC   #$04
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   DEY
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$B4
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$A7
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$A8
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   JSR   lt
   LDA   HARGS+8
   LDY   #$B5
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
   LDY   #$B5
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
   ADC   #$E4
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$E3
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
   SEC
   LDA   SSP
   SBC   #$04
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDY   #$B3
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$B4
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$A7
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$A8
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   JSR   ge
   LDA   HARGS+8
   LDY   #$B9
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
   LDY   #$B9
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   INY
   ORA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lnot_true@37
   LDA   #$00
   JMP   .lnot_end@38
.lnot_true@37:
   LDA   #$01
.lnot_end@38:
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
   ADC   #$E4
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$E3
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
   CLC
   LDA   FP
   ADC   #$4D
   LDY   #$BF
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$C1
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$C2
   STA   (FP),Y
   CLC
   LDA   FP
   ADC   #$4D
   LDY   #$C5
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   LDA   #$03
   STA   HARGS
   LDA   #$00
   LDX   #$08
   LSR   HARGS
.mul_loop@39:
   BCC   .mul_skip@40
   CLC
   ADC   #$14
.mul_skip@40:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@39
   LDY   #$C4
   STA   (FP),Y
   LDA   HARGS
   DEY
   STA   (FP),Y
   LDY   #$C5
   LDA   (FP),Y
   CLC
   LDY   #$C3
   ADC   (FP),Y
   LDY   #$C7
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$C4
   ADC   (FP),Y
   LDY   #$C8
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$C9
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$CA
   STA   (FP),Y
   CLC
   LDA   FP
   ADC   #$4D
   LDY   #$CD
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
.mul_loop@41:
   BCC   .mul_skip@42
   CLC
   ADC   #$14
.mul_skip@42:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@41
   LDY   #$CC
   STA   (FP),Y
   LDA   HARGS
   DEY
   STA   (FP),Y
   LDY   #$CD
   LDA   (FP),Y
   CLC
   LDY   #$CB
   ADC   (FP),Y
   LDY   #$CF
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$CC
   ADC   (FP),Y
   LDY   #$D0
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$D1
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$D2
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$04
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDY   #$C1
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$C2
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$C9
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$CA
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   JSR   ge_nested
   LDA   HARGS+8
   LDY   #$D3
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
   LDY   #$D3
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   INY
   ORA   (FP),Y
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
   ADC   #$E4
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$E3
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
   SEC
   LDA   SSP
   SBC   #$04
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDY   #$D1
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$D2
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$C9
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$CA
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   JSR   gt_nested
   LDA   HARGS+8
   LDY   #$D7
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
   LDY   #$D7
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   INY
   ORA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lnot_true@43
   LDA   #$00
   JMP   .lnot_end@44
.lnot_true@43:
   LDA   #$01
.lnot_end@44:
   LDY   #$DB
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@9
   LDX   #$00
   LDA   #$0A

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$E4
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$E3
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
   LDA   #$01
   STA   HARGS
   LDA   #$00
   LDX   #$08
   LSR   HARGS
.mul_loop@45:
   BCC   .mul_skip@46
   CLC
   ADC   #$14
.mul_skip@46:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@45
   LDY   #$DE
   STA   (FP),Y
   LDA   HARGS
   DEY
   STA   (FP),Y
   LDY   #$D1
   LDA   (FP),Y
   SEC
   LDY   #$DD
   SBC   (FP),Y
   LDY   #$DF
   STA   (FP),Y
   LDY   #$D2
   LDA   (FP),Y
   LDY   #$DE
   SBC   (FP),Y
   LDY   #$E0
   STA   (FP),Y
   LDY   #$CA
   LDA   (FP),Y
   LDY   #$E0
   CMP   (FP),Y
   BNE   .cmp_true@47
   LDY   #$C9
   LDA   (FP),Y
   LDY   #$DF
   CMP   (FP),Y
   BNE   .cmp_true@47
   LDA   #$00
   JMP   .cmp_end@48
.cmp_true@47:
   LDA   #$01
.cmp_end@48:
   LDY   #$E1
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
   LDA   #$0B

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$E4
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
.if_end@10:
   LDX   #$00
   TXA

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$E4
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$E3
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
