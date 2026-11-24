SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24

target_from_int:
   SUBROUTINE

   LDA   #$00
   STA   HARGS+16
   LDY   #$00
   STY   HARGS+17
   STY   HARGS+18
   STY   HARGS+19
   STA   HARGS+20
   LDA   #$40
   STA   HARGS+21
   LDY   #$8F
   STY   HARGS+22
   STA   HARGS+23
   RTS

target_from_uint:
   SUBROUTINE

   LDA   #$00
   STA   HARGS+16
   LDY   #$00
   STY   HARGS+17
   STY   HARGS+18
   STA   HARGS+19
   LDA   #$FF
   STA   HARGS+21
   LDA   #$EF
   STA   HARGS+22
   LDA   #$40
   STA   HARGS+20
   STA   HARGS+23
   RTS

target_from_long:
   SUBROUTINE

   LDA   #$00
   STA   HARGS+16
   LDY   #$00
   STY   HARGS+17
   STY   HARGS+18
   STY   HARGS+19
   STY   HARGS+20
   STA   HARGS+21
   LDA   #$D0
   STA   HARGS+22
   LDA   #$43
   STA   HARGS+23
   RTS

target_from_ulong:
   SUBROUTINE

   LDA   #$00
   STA   HARGS+16
   LDY   #$00
   STY   HARGS+17
   STY   HARGS+18
   STY   HARGS+19
   STA   HARGS+20
   LDA   #$04
   STA   HARGS+21
   LDA   #$90
   STA   HARGS+22
   LDA   #$40
   STA   HARGS+23
   RTS

target_implicit:
   SUBROUTINE

   LDA   #$00
   STA   HARGS+16
   LDY   #$00
   STY   HARGS+17
   STY   HARGS+18
   STY   HARGS+19
   STA   HARGS+20
   LDA   #$40
   STA   HARGS+21
   LDY   #$8F
   STY   HARGS+22
   STA   HARGS+23
   RTS

main:
   SUBROUTINE

   ; prologue: 0 arg bytes, 50 local bytes
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

   JSR   target_from_int
   LDA   HARGS+16
   LDY   #$01
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
   CMP   #$40
   BNE   .cmp_true@0
   DEY
   LDA   (FP),Y
   CMP   #$8F
   BNE   .cmp_true@0
   DEY
   LDA   (FP),Y
   CMP   #$40
   BNE   .cmp_true@0
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@0
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@0
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@0
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@0
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@0
   LDA   #$00
   JMP   .cmp_end@1
.cmp_true@0:
   LDA   #$01
.cmp_end@1:
   LDY   #$09
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
   ADC   #$34
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
.if_end@0:
   JSR   target_from_uint
   LDA   HARGS+16
   LDY   #$0B
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
   BNE   .cmp_true@3
   DEY
   LDA   (FP),Y
   CMP   #$EF
   BNE   .cmp_true@3
   DEY
   LDA   (FP),Y
   CMP   #$FF
   BNE   .cmp_true@3
   DEY
   LDA   (FP),Y
   CMP   #$FF
   BNE   .cmp_true@3
   DEY
   LDA   (FP),Y
   CMP   #$FF
   BNE   .cmp_true@3
   DEY
   LDA   (FP),Y
   CMP   #$40
   BNE   .cmp_true@3
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@3
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@3
   LDA   #$00
   JMP   .cmp_end@4
.cmp_true@3:
   LDA   #$01
.cmp_end@4:
   LDY   #$13
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
   ADC   #$34
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
.if_end@1:
   JSR   target_from_long
   LDA   HARGS+16
   LDY   #$15
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
   CMP   #$43
   BNE   .cmp_true@6
   DEY
   LDA   (FP),Y
   CMP   #$D0
   BNE   .cmp_true@6
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@6
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@6
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@6
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@6
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@6
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@6
   LDA   #$00
   JMP   .cmp_end@7
.cmp_true@6:
   LDA   #$01
.cmp_end@7:
   LDY   #$1D
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
   ADC   #$34
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
.if_end@2:
   JSR   target_from_ulong
   LDA   HARGS+16
   LDY   #$1F
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
   CMP   #$43
   BNE   .cmp_true@9
   DEY
   LDA   (FP),Y
   CMP   #$E0
   BNE   .cmp_true@9
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@9
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@9
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@9
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@9
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@9
   DEY
   LDA   (FP),Y
   CMP   #$01
   BNE   .cmp_true@9
   LDA   #$00
   JMP   .cmp_end@10
.cmp_true@9:
   LDA   #$01
.cmp_end@10:
   LDY   #$27
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
   ADC   #$34
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
.if_end@3:
   JSR   target_implicit
   LDA   HARGS+16
   LDY   #$29
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
   CMP   #$40
   BNE   .cmp_true@12
   DEY
   LDA   (FP),Y
   CMP   #$8F
   BNE   .cmp_true@12
   DEY
   LDA   (FP),Y
   CMP   #$40
   BNE   .cmp_true@12
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@12
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@12
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@12
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@12
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@12
   LDA   #$00
   JMP   .cmp_end@13
.cmp_true@12:
   LDA   #$01
.cmp_end@13:
   LDY   #$31
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
   ADC   #$34
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
.if_end@4:
   LDX   #$00
   TXA

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
