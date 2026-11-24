SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24

target:
   SUBROUTINE

   ; prologue: 0 arg bytes, 98 local bytes
   SEC
   LDA   SSP
   SBC   #$64
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$63
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   LDA   #$00
   STA   HARGS
   LDY   #$00
   STY   HARGS+1
   STY   HARGS+2
   STY   HARGS+3
   STY   HARGS+4
   STY   HARGS+5
   STY   HARGS+6
   STY   HARGS+7
   STY   HARGS+8
   STY   HARGS+9
   STY   HARGS+10
   STY   HARGS+11
   STY   HARGS+12
   STY   HARGS+13
   STY   HARGS+14
   STA   HARGS+15
   JSR   ddiv
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
   LDY   #$01
   LDA   (FP),Y
   LDY   #$09
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   LDY   #$0A
   STA   (FP),Y
   LDY   #$03
   LDA   (FP),Y
   LDY   #$0B
   STA   (FP),Y
   LDY   #$04
   LDA   (FP),Y
   LDY   #$0C
   STA   (FP),Y
   LDY   #$05
   LDA   (FP),Y
   LDY   #$0D
   STA   (FP),Y
   LDY   #$06
   LDA   (FP),Y
   LDY   #$0E
   STA   (FP),Y
   LDY   #$07
   LDA   (FP),Y
   LDY   #$0F
   STA   (FP),Y
   LDY   #$08
   LDA   (FP),Y
   LDY   #$10
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
   JSR   dlt
   LDA   HARGS+16
   LDY   #$11
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@6
   LDY   #$10
   LDA   (FP),Y
   BNE   .thread@0
   DEY
   LDA   (FP),Y
   BNE   .thread@0
   DEY
   LDA   (FP),Y
   BNE   .thread@0
   DEY
   LDA   (FP),Y
   BNE   .thread@0
   DEY
   LDA   (FP),Y
   BNE   .thread@0
   DEY
   LDA   (FP),Y
   BNE   .thread@0
   DEY
   LDA   (FP),Y
   BNE   .thread@0
   DEY
   LDA   (FP),Y
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
   BNE   .or_true@6
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   JMP   .or_end@7
.or_true@6:
   LDA   #$01
   LDY   #$15
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@7:
   LDY   #$15
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@4
   LDA   #$00
   STA   HARGS
   LDY   #$00
   STY   HARGS+1
   STY   HARGS+2
   STY   HARGS+3
   STY   HARGS+4
   STY   HARGS+5
   STY   HARGS+6
   STA   HARGS+7
   LDY   #$09
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
   INY
   LDA   (FP),Y
   STA   HARGS+12
   INY
   LDA   (FP),Y
   STA   HARGS+13
   INY
   LDA   (FP),Y
   STA   HARGS+14
   INY
   LDA   (FP),Y
   STA   HARGS+15
   JSR   dlt
   LDA   HARGS+16
   LDY   #$17
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@4
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   JMP   .or_end@5
.or_true@4:
   LDA   #$01
   LDY   #$19
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@5:
   LDY   #$19
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@2
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
   LDY   #$1B
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@2
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   JMP   .or_end@3
.or_true@2:
   LDA   #$01
   LDY   #$1D
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@3:
   LDY   #$1D
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@0
   LDA   #$00
   STA   HARGS
   LDY   #$00
   STY   HARGS+1
   STY   HARGS+2
   STY   HARGS+3
   STY   HARGS+4
   STY   HARGS+5
   STY   HARGS+6
   STA   HARGS+7
   LDY   #$09
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
   INY
   LDA   (FP),Y
   STA   HARGS+12
   INY
   LDA   (FP),Y
   STA   HARGS+13
   INY
   LDA   (FP),Y
   STA   HARGS+14
   INY
   LDA   (FP),Y
   STA   HARGS+15
   JSR   dle
   LDA   HARGS+16
   LDY   #$1F
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@0
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   JMP   .or_end@1
.or_true@0:
   LDA   #$01
   LDY   #$21
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@1:
   LDY   #$21
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@8
   LDX   #$00
   LDA   #$01

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$64
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$63
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
   LDA   #$00
   STA   HARGS+8
   LDY   #$00
   STY   HARGS+9
   STY   HARGS+10
   STY   HARGS+11
   STY   HARGS+12
   STA   HARGS+13
   LDA   #$F0
   STA   HARGS+14
   LDA   #$3F
   STA   HARGS+15
   JSR   dlt
   LDA   HARGS+16
   LDY   #$23
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@15
   LDA   #$3F
   LDY   #$10
   CMP   (FP),Y
   BNE   .thread@1
   LDA   #$F0
   DEY
   CMP   (FP),Y
   BNE   .thread@1
   LDA   #$00
   DEY
   CMP   (FP),Y
   BNE   .cmp_end@4
   DEY
   CMP   (FP),Y
   BNE   .cmp_end@4
   DEY
   CMP   (FP),Y
   BNE   .cmp_end@4
   DEY
   CMP   (FP),Y
   BNE   .cmp_end@4
   DEY
   CMP   (FP),Y
   BNE   .cmp_end@4
   DEY
   CMP   (FP),Y
   BEQ   .cmp_true@3
.thread@1:
   LDA   #$00
   JMP   .cmp_end@4
.cmp_true@3:
   LDA   #$01
.cmp_end@4:
   LDY   #$25
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@15
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   JMP   .or_end@16
.or_true@15:
   LDA   #$01
   LDY   #$27
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@16:
   LDY   #$27
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@13
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
   LDA   #$00
   STA   HARGS+8
   LDY   #$00
   STY   HARGS+9
   STY   HARGS+10
   STY   HARGS+11
   STY   HARGS+12
   STA   HARGS+13
   LDA   #$F0
   STA   HARGS+14
   LDA   #$3F
   STA   HARGS+15
   JSR   dlt
   LDA   HARGS+16
   LDY   #$29
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@13
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   JMP   .or_end@14
.or_true@13:
   LDA   #$01
   LDY   #$2B
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@14:
   LDY   #$2B
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@11
   LDA   #$00
   STA   HARGS
   LDY   #$00
   STY   HARGS+1
   STY   HARGS+2
   STY   HARGS+3
   STY   HARGS+4
   STA   HARGS+5
   LDA   #$F0
   STA   HARGS+6
   LDA   #$3F
   STA   HARGS+7
   LDY   #$09
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
   INY
   LDA   (FP),Y
   STA   HARGS+12
   INY
   LDA   (FP),Y
   STA   HARGS+13
   INY
   LDA   (FP),Y
   STA   HARGS+14
   INY
   LDA   (FP),Y
   STA   HARGS+15
   JSR   dle
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
   BNE   .or_true@11
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   JMP   .or_end@12
.or_true@11:
   LDA   #$01
   LDY   #$2F
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@12:
   LDY   #$2F
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@9
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
   LDA   #$00
   STA   HARGS+8
   LDY   #$00
   STY   HARGS+9
   STY   HARGS+10
   STY   HARGS+11
   STY   HARGS+12
   STA   HARGS+13
   LDA   #$F0
   STA   HARGS+14
   LDA   #$3F
   STA   HARGS+15
   JSR   dle
   LDA   HARGS+16
   LDY   #$31
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@9
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   JMP   .or_end@10
.or_true@9:
   LDA   #$01
   LDY   #$33
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@10:
   LDY   #$33
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@17
   LDX   #$00
   LDA   #$02

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$64
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$63
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
   LDY   #$10
   LDA   (FP),Y
   CMP   (FP),Y
   BNE   .thread@2
   DEY
   LDA   (FP),Y
   CMP   (FP),Y
   BNE   .thread@2
   DEY
   LDA   (FP),Y
   CMP   (FP),Y
   BNE   .thread@2
   DEY
   LDA   (FP),Y
   CMP   (FP),Y
   BNE   .thread@2
   DEY
   LDA   (FP),Y
   CMP   (FP),Y
   BNE   .thread@2
   DEY
   LDA   (FP),Y
   CMP   (FP),Y
   BNE   .thread@2
   DEY
   LDA   (FP),Y
   CMP   (FP),Y
   BNE   .thread@2
   DEY
   LDA   (FP),Y
   CMP   (FP),Y
   BEQ   .cmp_true@6
.thread@2:
   LDA   #$00
   JMP   .cmp_end@7
.cmp_true@6:
   LDA   #$01
.cmp_end@7:
   LDY   #$35
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@18
   LDX   #$00
   LDA   #$03

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$64
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$63
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
.if_end@18:
   LDY   #$10
   LDA   (FP),Y
   CMP   (FP),Y
   BNE   .cmp_true@9
   DEY
   LDA   (FP),Y
   CMP   (FP),Y
   BNE   .cmp_true@9
   DEY
   LDA   (FP),Y
   CMP   (FP),Y
   BNE   .cmp_true@9
   DEY
   LDA   (FP),Y
   CMP   (FP),Y
   BNE   .cmp_true@9
   DEY
   LDA   (FP),Y
   CMP   (FP),Y
   BNE   .cmp_true@9
   DEY
   LDA   (FP),Y
   CMP   (FP),Y
   BNE   .cmp_true@9
   DEY
   LDA   (FP),Y
   CMP   (FP),Y
   BNE   .cmp_true@9
   DEY
   LDA   (FP),Y
   CMP   (FP),Y
   BNE   .cmp_true@9
   LDA   #$00
   JMP   .cmp_end@10
.cmp_true@9:
   LDA   #$01
.cmp_end@10:
   LDY   #$37
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
   LDY   #$39
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@19
   LDX   #$00
   LDA   #$04

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$64
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$63
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
.if_end@19:
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
   LDA   #$00
   STA   HARGS+8
   LDY   #$00
   STY   HARGS+9
   STY   HARGS+10
   STY   HARGS+11
   STY   HARGS+12
   STA   HARGS+13
   LDA   #$10
   STA   HARGS+14
   LDA   #$40
   STA   HARGS+15
   JSR   dmul
   LDA   HARGS+16
   LDY   #$3B
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
   LDY   #$3B
   LDA   (FP),Y
   LDY   #$09
   STA   (FP),Y
   LDY   #$3C
   LDA   (FP),Y
   LDY   #$0A
   STA   (FP),Y
   LDY   #$3D
   LDA   (FP),Y
   LDY   #$0B
   STA   (FP),Y
   LDY   #$3E
   LDA   (FP),Y
   LDY   #$0C
   STA   (FP),Y
   LDY   #$3F
   LDA   (FP),Y
   LDY   #$0D
   STA   (FP),Y
   LDY   #$40
   LDA   (FP),Y
   LDY   #$0E
   STA   (FP),Y
   LDY   #$41
   LDA   (FP),Y
   LDY   #$0F
   STA   (FP),Y
   LDY   #$42
   LDA   (FP),Y
   LDY   #$10
   STA   (FP),Y
   CMP   (FP),Y
   BNE   .thread@3
   DEY
   LDA   (FP),Y
   CMP   (FP),Y
   BNE   .thread@3
   DEY
   LDA   (FP),Y
   CMP   (FP),Y
   BNE   .thread@3
   DEY
   LDA   (FP),Y
   CMP   (FP),Y
   BNE   .thread@3
   DEY
   LDA   (FP),Y
   CMP   (FP),Y
   BNE   .thread@3
   DEY
   LDA   (FP),Y
   CMP   (FP),Y
   BNE   .thread@3
   DEY
   LDA   (FP),Y
   CMP   (FP),Y
   BNE   .thread@3
   DEY
   LDA   (FP),Y
   CMP   (FP),Y
   BEQ   .cmp_true@14
.thread@3:
   LDA   #$00
   JMP   .cmp_end@15
.cmp_true@14:
   LDA   #$01
.cmp_end@15:
   LDY   #$43
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@20
   LDX   #$00
   LDA   #$05

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$64
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$63
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
.if_end@20:
   LDA   #$00
   STA   HARGS
   LDY   #$00
   STY   HARGS+1
   STY   HARGS+2
   STY   HARGS+3
   STA   HARGS+4
   LDA   #$30
   STA   HARGS+5
   LDA   #$A1
   STA   HARGS+6
   LDA   #$40
   STA   HARGS+7
   LDY   #$09
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
   INY
   LDA   (FP),Y
   STA   HARGS+12
   INY
   LDA   (FP),Y
   STA   HARGS+13
   INY
   LDA   (FP),Y
   STA   HARGS+14
   INY
   LDA   (FP),Y
   STA   HARGS+15
   JSR   ddiv
   LDA   HARGS+16
   LDY   #$45
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
   LDY   #$45
   LDA   (FP),Y
   LDY   #$09
   STA   (FP),Y
   LDY   #$46
   LDA   (FP),Y
   LDY   #$0A
   STA   (FP),Y
   LDY   #$47
   LDA   (FP),Y
   LDY   #$0B
   STA   (FP),Y
   LDY   #$48
   LDA   (FP),Y
   LDY   #$0C
   STA   (FP),Y
   LDY   #$49
   LDA   (FP),Y
   LDY   #$0D
   STA   (FP),Y
   LDY   #$4A
   LDA   (FP),Y
   LDY   #$0E
   STA   (FP),Y
   LDY   #$4B
   LDA   (FP),Y
   LDY   #$0F
   STA   (FP),Y
   LDY   #$4C
   LDA   (FP),Y
   LDY   #$10
   STA   (FP),Y
   CMP   (FP),Y
   BNE   .thread@4
   DEY
   LDA   (FP),Y
   CMP   (FP),Y
   BNE   .thread@4
   DEY
   LDA   (FP),Y
   CMP   (FP),Y
   BNE   .thread@4
   DEY
   LDA   (FP),Y
   CMP   (FP),Y
   BNE   .thread@4
   DEY
   LDA   (FP),Y
   CMP   (FP),Y
   BNE   .thread@4
   DEY
   LDA   (FP),Y
   CMP   (FP),Y
   BNE   .thread@4
   DEY
   LDA   (FP),Y
   CMP   (FP),Y
   BNE   .thread@4
   DEY
   LDA   (FP),Y
   CMP   (FP),Y
   BEQ   .cmp_true@17
.thread@4:
   LDA   #$00
   JMP   .cmp_end@18
.cmp_true@17:
   LDA   #$01
.cmp_end@18:
   LDY   #$4D
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@21
   LDX   #$00
   LDA   #$06

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$64
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$63
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
.if_end@21:
   LDY   #$09
   LDA   (FP),Y
   LDY   #$4F
   STA   (FP),Y
   LDY   #$0A
   LDA   (FP),Y
   LDY   #$50
   STA   (FP),Y
   LDY   #$0B
   LDA   (FP),Y
   LDY   #$51
   STA   (FP),Y
   LDY   #$0C
   LDA   (FP),Y
   LDY   #$52
   STA   (FP),Y
   LDY   #$0D
   LDA   (FP),Y
   LDY   #$53
   STA   (FP),Y
   LDY   #$0E
   LDA   (FP),Y
   LDY   #$54
   STA   (FP),Y
   LDY   #$0F
   LDA   (FP),Y
   LDY   #$55
   STA   (FP),Y
   LDY   #$10
   LDA   (FP),Y
   EOR   #$80
   LDY   #$56
   STA   (FP),Y
   LDY   #$09
   LDA   (FP),Y
   LDY   #$57
   STA   (FP),Y
   LDY   #$0A
   LDA   (FP),Y
   LDY   #$58
   STA   (FP),Y
   LDY   #$0B
   LDA   (FP),Y
   LDY   #$59
   STA   (FP),Y
   LDY   #$0C
   LDA   (FP),Y
   LDY   #$5A
   STA   (FP),Y
   LDY   #$0D
   LDA   (FP),Y
   LDY   #$5B
   STA   (FP),Y
   LDY   #$0E
   LDA   (FP),Y
   LDY   #$5C
   STA   (FP),Y
   LDY   #$0F
   LDA   (FP),Y
   LDY   #$5D
   STA   (FP),Y
   LDY   #$10
   LDA   (FP),Y
   EOR   #$80
   LDY   #$5E
   STA   (FP),Y
   LDY   #$56
   LDA   (FP),Y
   LDY   #$5E
   CMP   (FP),Y
   BNE   .thread@5
   LDY   #$55
   LDA   (FP),Y
   LDY   #$5D
   CMP   (FP),Y
   BNE   .thread@5
   LDY   #$54
   LDA   (FP),Y
   LDY   #$5C
   CMP   (FP),Y
   BNE   .thread@5
   LDY   #$53
   LDA   (FP),Y
   LDY   #$5B
   CMP   (FP),Y
   BNE   .thread@5
   LDY   #$52
   LDA   (FP),Y
   LDY   #$5A
   CMP   (FP),Y
   BNE   .thread@5
   LDY   #$51
   LDA   (FP),Y
   LDY   #$59
   CMP   (FP),Y
   BNE   .thread@5
   LDY   #$50
   LDA   (FP),Y
   LDY   #$58
   CMP   (FP),Y
   BNE   .thread@5
   LDY   #$4F
   LDA   (FP),Y
   LDY   #$57
   CMP   (FP),Y
   BEQ   .cmp_true@20
.thread@5:
   LDA   #$00
   JMP   .cmp_end@21
.cmp_true@20:
   LDA   #$01
.cmp_end@21:
   LDY   #$5F
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@22
   LDX   #$00
   LDA   #$07

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$64
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$63
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
.if_end@22:
   LDY   #$09
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   INY
   ORA   (FP),Y
   INY
   ORA   (FP),Y
   INY
   ORA   (FP),Y
   INY
   ORA   (FP),Y
   INY
   ORA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lnot_true@23
   LDA   #$00
   JMP   .lnot_end@24
.lnot_true@23:
   LDA   #$01
.lnot_end@24:
   LDY   #$61
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@23
   LDX   #$00
   LDA   #$08

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$64
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
.if_end@23:
   LDX   #$00
   TXA

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$64
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$63
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

   JSR   target
   LDY   #$01
   STA   (FP),Y
   TXA
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
   ADC   #$04
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$03
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
