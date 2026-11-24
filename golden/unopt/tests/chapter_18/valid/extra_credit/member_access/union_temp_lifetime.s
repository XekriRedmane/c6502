SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24

get_flag:
   SUBROUTINE

   ; prologue: 0 arg bytes, 2 local bytes
   SEC
   LDA   SSP
   SBC   #$04
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$03
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   LDA   @0.flag
   ORA   @0.flag+1
   BEQ   .lnot_true@0
   LDA   #$00
   JMP   .lnot_end@1
.lnot_true@0:
   LDA   #$01
.lnot_end@1:
   LDY   #$01
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   @0.flag
   INY
   LDA   (FP),Y
   STA   @0.flag+1
   TAX
   LDA   @0.flag

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$04
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

main:
   SUBROUTINE

   ; prologue: 0 arg bytes, 62 local bytes
   SEC
   LDA   SSP
   SBC   #$40
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$3F
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
   LDA   #$EA
   LDY   #$00
   STA   (DPTR),Y
   LDA   #$00
   INY
   STA   (DPTR),Y
   INY
   STA   (DPTR),Y
   INY
   STA   (DPTR),Y
   CLC
   LDA   FP
   ADC   #$0B
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
   LDA   #$D2
   LDY   #$00
   STA   (DPTR),Y
   LDA   #$00
   INY
   STA   (DPTR),Y
   INY
   STA   (DPTR),Y
   INY
   STA   (DPTR),Y
   JSR   get_flag
   LDY   #$15
   STA   (FP),Y
   TXA
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .cond_else@0
   LDY   #$01
   LDA   (FP),Y
   LDY   #$17
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   LDY   #$18
   STA   (FP),Y
   LDY   #$03
   LDA   (FP),Y
   LDY   #$19
   STA   (FP),Y
   LDY   #$04
   LDA   (FP),Y
   LDY   #$1A
   STA   (FP),Y
   LDY   #$05
   LDA   (FP),Y
   LDY   #$1B
   STA   (FP),Y
   LDY   #$06
   LDA   (FP),Y
   LDY   #$1C
   STA   (FP),Y
   LDY   #$07
   LDA   (FP),Y
   LDY   #$1D
   STA   (FP),Y
   LDY   #$08
   LDA   (FP),Y
   LDY   #$1E
   STA   (FP),Y
   JMP   .cond_end@1
.cond_else@0:
   LDY   #$0B
   LDA   (FP),Y
   LDY   #$17
   STA   (FP),Y
   LDY   #$0C
   LDA   (FP),Y
   LDY   #$18
   STA   (FP),Y
   LDY   #$0D
   LDA   (FP),Y
   LDY   #$19
   STA   (FP),Y
   LDY   #$0E
   LDA   (FP),Y
   LDY   #$1A
   STA   (FP),Y
   LDY   #$0F
   LDA   (FP),Y
   LDY   #$1B
   STA   (FP),Y
   LDY   #$10
   LDA   (FP),Y
   LDY   #$1C
   STA   (FP),Y
   LDY   #$11
   LDA   (FP),Y
   LDY   #$1D
   STA   (FP),Y
   LDY   #$12
   LDA   (FP),Y
   LDY   #$1E
   STA   (FP),Y
.cond_end@1:
   CLC
   LDA   FP
   ADC   #$17
   LDY   #$1F
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
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
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   SEC
   SBC   #$16
   INY
   STA   (FP),Y
   LDA   #$00
   SBC   #$00
   INY
   STA   (FP),Y
   LDY   #$25
   LDA   (FP),Y
   LDY   #$27
   CMP   (FP),Y
   BNE   .cmp_true@2
   LDY   #$24
   LDA   (FP),Y
   LDY   #$26
   CMP   (FP),Y
   BNE   .cmp_true@2
   LDA   #$00
   JMP   .cmp_end@3
.cmp_true@2:
   LDA   #$01
.cmp_end@3:
   LDY   #$28
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
   ADC   #$40
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$3F
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
   JSR   get_flag
   LDY   #$2A
   STA   (FP),Y
   TXA
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .cond_else@3
   LDY   #$01
   LDA   (FP),Y
   LDY   #$2C
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   LDY   #$2D
   STA   (FP),Y
   LDY   #$03
   LDA   (FP),Y
   LDY   #$2E
   STA   (FP),Y
   LDY   #$04
   LDA   (FP),Y
   LDY   #$2F
   STA   (FP),Y
   LDY   #$05
   LDA   (FP),Y
   LDY   #$30
   STA   (FP),Y
   LDY   #$06
   LDA   (FP),Y
   LDY   #$31
   STA   (FP),Y
   LDY   #$07
   LDA   (FP),Y
   LDY   #$32
   STA   (FP),Y
   LDY   #$08
   LDA   (FP),Y
   LDY   #$33
   STA   (FP),Y
   JMP   .cond_end@4
.cond_else@3:
   LDY   #$0B
   LDA   (FP),Y
   LDY   #$2C
   STA   (FP),Y
   LDY   #$0C
   LDA   (FP),Y
   LDY   #$2D
   STA   (FP),Y
   LDY   #$0D
   LDA   (FP),Y
   LDY   #$2E
   STA   (FP),Y
   LDY   #$0E
   LDA   (FP),Y
   LDY   #$2F
   STA   (FP),Y
   LDY   #$0F
   LDA   (FP),Y
   LDY   #$30
   STA   (FP),Y
   LDY   #$10
   LDA   (FP),Y
   LDY   #$31
   STA   (FP),Y
   LDY   #$11
   LDA   (FP),Y
   LDY   #$32
   STA   (FP),Y
   LDY   #$12
   LDA   (FP),Y
   LDY   #$33
   STA   (FP),Y
.cond_end@4:
   CLC
   LDA   FP
   ADC   #$2C
   LDY   #$34
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   LDY   #$36
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$37
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$38
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   SEC
   SBC   #$2E
   INY
   STA   (FP),Y
   LDA   #$00
   SBC   #$00
   INY
   STA   (FP),Y
   LDY   #$3A
   LDA   (FP),Y
   LDY   #$3C
   CMP   (FP),Y
   BNE   .cmp_true@5
   LDY   #$39
   LDA   (FP),Y
   LDY   #$3B
   CMP   (FP),Y
   BNE   .cmp_true@5
   LDA   #$00
   JMP   .cmp_end@6
.cmp_true@5:
   LDA   #$01
.cmp_end@6:
   LDY   #$3D
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
   LDA   #$02

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
.if_end@5:
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
   LDY   #$3F
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

@0.flag:
   DS.B  2
