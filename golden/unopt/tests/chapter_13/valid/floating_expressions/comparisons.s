SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24

main:
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

   LDA   fifty_fiveE5
   STA   HARGS
   LDA   fifty_fiveE5+1
   STA   HARGS+1
   LDA   fifty_fiveE5+2
   STA   HARGS+2
   LDA   fifty_fiveE5+3
   STA   HARGS+3
   LDA   fifty_fiveE5+4
   STA   HARGS+4
   LDA   fifty_fiveE5+5
   STA   HARGS+5
   LDA   fifty_fiveE5+6
   STA   HARGS+6
   LDA   fifty_fiveE5+7
   STA   HARGS+7
   LDA   fifty_fourE4
   STA   HARGS+8
   LDA   fifty_fourE4+1
   STA   HARGS+9
   LDA   fifty_fourE4+2
   STA   HARGS+10
   LDA   fifty_fourE4+3
   STA   HARGS+11
   LDA   fifty_fourE4+4
   STA   HARGS+12
   LDA   fifty_fourE4+5
   STA   HARGS+13
   LDA   fifty_fourE4+6
   STA   HARGS+14
   LDA   fifty_fourE4+7
   STA   HARGS+15
   JSR   dlt
   LDA   HARGS+16
   LDY   #$01
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
.if_end@0:
   LDA   #$00
   STA   HARGS
   LDY   #$00
   STY   HARGS+1
   STY   HARGS+2
   STY   HARGS+3
   STY   HARGS+4
   STA   HARGS+5
   LDA   #$10
   STA   HARGS+6
   LDA   #$40
   STA   HARGS+7
   LDA   four
   STA   HARGS+8
   LDA   four+1
   STA   HARGS+9
   LDA   four+2
   STA   HARGS+10
   LDA   four+3
   STA   HARGS+11
   LDA   four+4
   STA   HARGS+12
   LDA   four+5
   STA   HARGS+13
   LDA   four+6
   STA   HARGS+14
   LDA   four+7
   STA   HARGS+15
   JSR   dlt
   LDA   HARGS+16
   LDY   #$03
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
.if_end@1:
   LDA   tiny
   STA   HARGS
   LDA   tiny+1
   STA   HARGS+1
   LDA   tiny+2
   STA   HARGS+2
   LDA   tiny+3
   STA   HARGS+3
   LDA   tiny+4
   STA   HARGS+4
   LDA   tiny+5
   STA   HARGS+5
   LDA   tiny+6
   STA   HARGS+6
   LDA   tiny+7
   STA   HARGS+7
   LDA   #$00
   STA   HARGS+8
   LDY   #$00
   STY   HARGS+9
   STY   HARGS+10
   STY   HARGS+11
   STY   HARGS+12
   STY   HARGS+13
   STY   HARGS+14
   STA   HARGS+15
   JSR   dle
   LDA   HARGS+16
   LDY   #$05
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
.if_end@2:
   LDA   fifty_fiveE5
   STA   HARGS
   LDA   fifty_fiveE5+1
   STA   HARGS+1
   LDA   fifty_fiveE5+2
   STA   HARGS+2
   LDA   fifty_fiveE5+3
   STA   HARGS+3
   LDA   fifty_fiveE5+4
   STA   HARGS+4
   LDA   fifty_fiveE5+5
   STA   HARGS+5
   LDA   fifty_fiveE5+6
   STA   HARGS+6
   LDA   fifty_fiveE5+7
   STA   HARGS+7
   LDA   fifty_fourE4
   STA   HARGS+8
   LDA   fifty_fourE4+1
   STA   HARGS+9
   LDA   fifty_fourE4+2
   STA   HARGS+10
   LDA   fifty_fourE4+3
   STA   HARGS+11
   LDA   fifty_fourE4+4
   STA   HARGS+12
   LDA   fifty_fourE4+5
   STA   HARGS+13
   LDA   fifty_fourE4+6
   STA   HARGS+14
   LDA   fifty_fourE4+7
   STA   HARGS+15
   JSR   dle
   LDA   HARGS+16
   LDY   #$07
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
.if_end@3:
   LDA   tiny+7
   BNE   .thread@0
   LDA   tiny+6
   BNE   .thread@0
   LDA   tiny+5
   BNE   .thread@0
   LDA   tiny+4
   BNE   .thread@0
   LDA   tiny+3
   BNE   .thread@0
   LDA   tiny+2
   BNE   .thread@0
   LDA   tiny+1
   BNE   .thread@0
   LDA   tiny
   BEQ   .cmp_true@0
.thread@0:
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
   BEQ   .if_end@4
   LDX   #$00
   LDA   #$05

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
.if_end@4:
   LDA   point_one+7
   CMP   point_one+7
   BNE   .cmp_true@3
   LDA   point_one+6
   CMP   point_one+6
   BNE   .cmp_true@3
   LDA   point_one+5
   CMP   point_one+5
   BNE   .cmp_true@3
   LDA   point_one+4
   CMP   point_one+4
   BNE   .cmp_true@3
   LDA   point_one+3
   CMP   point_one+3
   BNE   .cmp_true@3
   LDA   point_one+2
   CMP   point_one+2
   BNE   .cmp_true@3
   LDA   point_one+1
   CMP   point_one+1
   BNE   .cmp_true@3
   LDA   point_one
   CMP   point_one
   BNE   .cmp_true@3
   LDA   #$00
   JMP   .cmp_end@4
.cmp_true@3:
   LDA   #$01
.cmp_end@4:
   LDY   #$0B
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
.if_end@5:
   LDA   #$F1
   STA   HARGS
   LDA   #$68
   STA   HARGS+1
   LDA   #$E3
   STA   HARGS+2
   LDA   #$88
   STA   HARGS+3
   LDA   #$B5
   STA   HARGS+4
   LDA   #$F8
   STA   HARGS+5
   LDA   #$D4
   STA   HARGS+6
   LDA   #$3E
   STA   HARGS+7
   LDA   tiny
   STA   HARGS+8
   LDA   tiny+1
   STA   HARGS+9
   LDA   tiny+2
   STA   HARGS+10
   LDA   tiny+3
   STA   HARGS+11
   LDA   tiny+4
   STA   HARGS+12
   LDA   tiny+5
   STA   HARGS+13
   LDA   tiny+6
   STA   HARGS+14
   LDA   tiny+7
   STA   HARGS+15
   JSR   dlt
   LDA   HARGS+16
   LDY   #$0D
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lnot_true@6
   LDA   #$00
   JMP   .lnot_end@7
.lnot_true@6:
   LDA   #$01
.lnot_end@7:
   LDY   #$0F
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
   LDA   #$07

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
.if_end@6:
   LDA   #$F1
   LDY   #$11
   STA   (FP),Y
   LDA   #$68
   INY
   STA   (FP),Y
   LDA   #$E3
   INY
   STA   (FP),Y
   LDA   #$88
   INY
   STA   (FP),Y
   LDA   #$B5
   INY
   STA   (FP),Y
   LDA   #$F8
   INY
   STA   (FP),Y
   LDA   #$04
   INY
   STA   (FP),Y
   LDA   #$3F
   EOR   #$80
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
   LDA   four
   STA   HARGS+8
   LDA   four+1
   STA   HARGS+9
   LDA   four+2
   STA   HARGS+10
   LDA   four+3
   STA   HARGS+11
   LDA   four+4
   STA   HARGS+12
   LDA   four+5
   STA   HARGS+13
   LDA   four+6
   STA   HARGS+14
   LDA   four+7
   STA   HARGS+15
   JSR   dlt
   LDA   HARGS+16
   LDY   #$19
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lnot_true@8
   LDA   #$00
   JMP   .lnot_end@9
.lnot_true@8:
   LDA   #$01
.lnot_end@9:
   LDY   #$1B
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
.if_end@7:
   LDA   tiny
   STA   HARGS
   STA   HARGS+8
   LDA   tiny+1
   STA   HARGS+1
   STA   HARGS+9
   LDA   tiny+2
   STA   HARGS+2
   STA   HARGS+10
   LDA   tiny+3
   STA   HARGS+3
   STA   HARGS+11
   LDA   tiny+4
   STA   HARGS+4
   STA   HARGS+12
   LDA   tiny+5
   STA   HARGS+5
   STA   HARGS+13
   LDA   tiny+6
   STA   HARGS+6
   STA   HARGS+14
   LDA   tiny+7
   STA   HARGS+7
   STA   HARGS+15
   JSR   dle
   LDA   HARGS+16
   LDY   #$1D
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lnot_true@10
   LDA   #$00
   JMP   .lnot_end@11
.lnot_true@10:
   LDA   #$01
.lnot_end@11:
   LDY   #$1F
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
.if_end@8:
   LDA   fifty_fiveE5
   STA   HARGS
   STA   HARGS+8
   LDA   fifty_fiveE5+1
   STA   HARGS+1
   STA   HARGS+9
   LDA   fifty_fiveE5+2
   STA   HARGS+2
   STA   HARGS+10
   LDA   fifty_fiveE5+3
   STA   HARGS+3
   STA   HARGS+11
   LDA   fifty_fiveE5+4
   STA   HARGS+4
   STA   HARGS+12
   LDA   fifty_fiveE5+5
   STA   HARGS+5
   STA   HARGS+13
   LDA   fifty_fiveE5+6
   STA   HARGS+6
   STA   HARGS+14
   LDA   fifty_fiveE5+7
   STA   HARGS+7
   STA   HARGS+15
   JSR   dle
   LDA   HARGS+16
   LDY   #$21
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lnot_true@12
   LDA   #$00
   JMP   .lnot_end@13
.lnot_true@12:
   LDA   #$01
.lnot_end@13:
   LDY   #$23
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
.if_end@9:
   LDA   #$3F
   CMP   point_one+7
   BNE   .thread@1
   LDA   #$B9
   CMP   point_one+6
   BNE   .thread@1
   LDA   #$99
   CMP   point_one+5
   BNE   .thread@1
   CMP   point_one+4
   BNE   .thread@1
   CMP   point_one+3
   BNE   .thread@1
   CMP   point_one+2
   BNE   .thread@1
   CMP   point_one+1
   BNE   .thread@1
   LDA   #$9A
   CMP   point_one
   BEQ   .cmp_true@14
.thread@1:
   LDA   #$00
   JMP   .cmp_end@15
.cmp_true@14:
   LDA   #$01
.cmp_end@15:
   LDY   #$25
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lnot_true@17
   LDA   #$00
   JMP   .lnot_end@18
.lnot_true@17:
   LDA   #$01
.lnot_end@18:
   LDY   #$27
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
.if_end@10:
   LDA   tiny+7
   CMP   #$3E
   BNE   .cmp_true@19
   LDA   tiny+6
   CMP   #$FF
   BNE   .cmp_true@19
   LDA   tiny+5
   CMP   #$75
   BNE   .cmp_true@19
   LDA   tiny+4
   CMP   #$10
   BNE   .cmp_true@19
   LDA   tiny+3
   CMP   #$4D
   BNE   .cmp_true@19
   LDA   tiny+2
   CMP   #$55
   BNE   .cmp_true@19
   LDA   tiny+1
   CMP   #$1D
   BNE   .cmp_true@19
   LDA   tiny
   CMP   #$69
   BNE   .cmp_true@19
   LDA   #$00
   JMP   .cmp_end@20
.cmp_true@19:
   LDA   #$01
.cmp_end@20:
   LDY   #$29
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lnot_true@22
   LDA   #$00
   JMP   .lnot_end@23
.lnot_true@22:
   LDA   #$01
.lnot_end@23:
   LDY   #$2B
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@11
   LDX   #$00
   LDA   #$0C

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
.if_end@11:
   LDA   #$69
   STA   HARGS
   LDA   #$1D
   STA   HARGS+1
   LDA   #$55
   STA   HARGS+2
   LDA   #$4D
   STA   HARGS+3
   LDA   #$10
   STA   HARGS+4
   LDA   #$75
   STA   HARGS+5
   LDA   #$FF
   STA   HARGS+6
   LDA   #$3E
   STA   HARGS+7
   LDA   #$1A
   STA   HARGS+8
   LDA   #$DF
   STA   HARGS+9
   LDA   #$C4
   STA   HARGS+10
   LDA   #$41
   STA   HARGS+11
   LDA   #$66
   STA   HARGS+12
   LDA   #$63
   STA   HARGS+13
   LDA   #$8A
   STA   HARGS+14
   LDA   #$3D
   STA   HARGS+15
   JSR   dlt
   LDA   HARGS+16
   LDY   #$2D
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@12
   LDX   #$00
   LDA   #$0D

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
.if_end@12:
   LDX   #$00
   TXA

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

fifty_fiveE5:
   DC.L  $00000000
   DC.L  $4154FB18

fifty_fourE4:
   DC.L  $00000000
   DC.L  $41207AC0

tiny:
   DC.L  $88E368F1
   DC.L  $3F04F8B5

four:
   DC.L  $00000000
   DC.L  $40100000

point_one:
   DC.L  $9999999A
   DC.L  $3FB99999
