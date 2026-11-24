SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24

main:
   SUBROUTINE

   ; prologue: 0 arg bytes, 140 local bytes
   SEC
   LDA   SSP
   SBC   #$8E
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$8D
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
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$F0
   INY
   STA   (FP),Y
   LDA   #$3F
   INY
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
   LDA   #$F0
   INY
   STA   (FP),Y
   LDA   #$3F
   INY
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
   LDA   #$F0
   INY
   STA   (FP),Y
   LDA   #$3F
   INY
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
   LDA   #$F0
   INY
   STA   (FP),Y
   LDA   #$3F
   INY
   STA   (FP),Y
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
   BNE   .thread@0
   LDY   #$02
   LDA   (FP),Y
   LDY   #$0A
   CMP   (FP),Y
   BNE   .thread@0
   LDY   #$01
   LDA   (FP),Y
   LDY   #$09
   CMP   (FP),Y
   BEQ   .cmp_true@0
.thread@0:
   LDA   #$00
   JMP   .cmp_end@1
.cmp_true@0:
   LDA   #$01
.cmp_end@1:
   LDY   #$21
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@2
   LDY   #$08
   LDA   (FP),Y
   LDY   #$18
   CMP   (FP),Y
   BNE   .thread@1
   LDY   #$07
   LDA   (FP),Y
   LDY   #$17
   CMP   (FP),Y
   BNE   .thread@1
   LDY   #$06
   LDA   (FP),Y
   LDY   #$16
   CMP   (FP),Y
   BNE   .thread@1
   LDY   #$05
   LDA   (FP),Y
   LDY   #$15
   CMP   (FP),Y
   BNE   .thread@1
   LDY   #$04
   LDA   (FP),Y
   LDY   #$14
   CMP   (FP),Y
   BNE   .thread@1
   LDY   #$03
   LDA   (FP),Y
   LDY   #$13
   CMP   (FP),Y
   BNE   .thread@1
   LDY   #$02
   LDA   (FP),Y
   LDY   #$12
   CMP   (FP),Y
   BNE   .thread@1
   LDY   #$01
   LDA   (FP),Y
   LDY   #$11
   CMP   (FP),Y
   BEQ   .cmp_true@3
.thread@1:
   LDA   #$00
   JMP   .cmp_end@4
.cmp_true@3:
   LDA   #$01
.cmp_end@4:
   LDY   #$23
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
   LDY   #$25
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@3:
   LDY   #$25
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@0
   LDY   #$08
   LDA   (FP),Y
   LDY   #$20
   CMP   (FP),Y
   BNE   .thread@2
   LDY   #$07
   LDA   (FP),Y
   LDY   #$1F
   CMP   (FP),Y
   BNE   .thread@2
   LDY   #$06
   LDA   (FP),Y
   LDY   #$1E
   CMP   (FP),Y
   BNE   .thread@2
   LDY   #$05
   LDA   (FP),Y
   LDY   #$1D
   CMP   (FP),Y
   BNE   .thread@2
   LDY   #$04
   LDA   (FP),Y
   LDY   #$1C
   CMP   (FP),Y
   BNE   .thread@2
   LDY   #$03
   LDA   (FP),Y
   LDY   #$1B
   CMP   (FP),Y
   BNE   .thread@2
   LDY   #$02
   LDA   (FP),Y
   LDY   #$1A
   CMP   (FP),Y
   BNE   .thread@2
   LDY   #$01
   LDA   (FP),Y
   LDY   #$19
   CMP   (FP),Y
   BEQ   .cmp_true@6
.thread@2:
   LDA   #$00
   JMP   .cmp_end@7
.cmp_true@6:
   LDA   #$01
.cmp_end@7:
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
   LDY   #$29
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lnot_true@9
   LDA   #$00
   JMP   .lnot_end@10
.lnot_true@9:
   LDA   #$01
.lnot_end@10:
   LDY   #$2B
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
   LDA   #$01

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$8E
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$8D
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
   LDY   #$01
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
   INY
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
   JSR   dadd
   LDA   HARGS+16
   LDY   #$2D
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
   LDY   #$2D
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
   LDY   #$11
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
   JSR   dadd
   LDA   HARGS+16
   LDY   #$35
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
   LDY   #$35
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
   LDY   #$19
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
   JSR   dadd
   LDA   HARGS+16
   LDY   #$3D
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
   BNE   .cmp_true@11
   DEY
   LDA   (FP),Y
   CMP   #$10
   BNE   .cmp_true@11
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@11
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@11
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@11
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@11
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@11
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@11
   LDA   #$00
   JMP   .cmp_end@12
.cmp_true@11:
   LDA   #$01
.cmp_end@12:
   LDY   #$45
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
   ADC   #$8E
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$8D
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
   LDA   #$00
   LDY   #$47
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
   LDA   #$C0
   INY
   STA   (FP),Y
   LDA   #$3F
   INY
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
   LDA   #$C0
   INY
   STA   (FP),Y
   LDA   #$3F
   INY
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
   LDA   #$C0
   INY
   STA   (FP),Y
   LDA   #$3F
   INY
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
   LDA   #$C0
   INY
   STA   (FP),Y
   LDA   #$3F
   INY
   STA   (FP),Y
   LDY   #$4E
   LDA   (FP),Y
   LDY   #$56
   CMP   (FP),Y
   BNE   .thread@3
   LDY   #$4D
   LDA   (FP),Y
   LDY   #$55
   CMP   (FP),Y
   BNE   .thread@3
   LDY   #$4C
   LDA   (FP),Y
   LDY   #$54
   CMP   (FP),Y
   BNE   .thread@3
   LDY   #$4B
   LDA   (FP),Y
   LDY   #$53
   CMP   (FP),Y
   BNE   .thread@3
   LDY   #$4A
   LDA   (FP),Y
   LDY   #$52
   CMP   (FP),Y
   BNE   .thread@3
   LDY   #$49
   LDA   (FP),Y
   LDY   #$51
   CMP   (FP),Y
   BNE   .thread@3
   LDY   #$48
   LDA   (FP),Y
   LDY   #$50
   CMP   (FP),Y
   BNE   .thread@3
   LDY   #$47
   LDA   (FP),Y
   LDY   #$4F
   CMP   (FP),Y
   BEQ   .cmp_true@14
.thread@3:
   LDA   #$00
   JMP   .cmp_end@15
.cmp_true@14:
   LDA   #$01
.cmp_end@15:
   LDY   #$67
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@8
   LDY   #$4E
   LDA   (FP),Y
   LDY   #$5E
   CMP   (FP),Y
   BNE   .thread@4
   LDY   #$4D
   LDA   (FP),Y
   LDY   #$5D
   CMP   (FP),Y
   BNE   .thread@4
   LDY   #$4C
   LDA   (FP),Y
   LDY   #$5C
   CMP   (FP),Y
   BNE   .thread@4
   LDY   #$4B
   LDA   (FP),Y
   LDY   #$5B
   CMP   (FP),Y
   BNE   .thread@4
   LDY   #$4A
   LDA   (FP),Y
   LDY   #$5A
   CMP   (FP),Y
   BNE   .thread@4
   LDY   #$49
   LDA   (FP),Y
   LDY   #$59
   CMP   (FP),Y
   BNE   .thread@4
   LDY   #$48
   LDA   (FP),Y
   LDY   #$58
   CMP   (FP),Y
   BNE   .thread@4
   LDY   #$47
   LDA   (FP),Y
   LDY   #$57
   CMP   (FP),Y
   BEQ   .cmp_true@17
.thread@4:
   LDA   #$00
   JMP   .cmp_end@18
.cmp_true@17:
   LDA   #$01
.cmp_end@18:
   LDY   #$69
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
   LDY   #$6B
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@9:
   LDY   #$6B
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@6
   LDY   #$4E
   LDA   (FP),Y
   LDY   #$66
   CMP   (FP),Y
   BNE   .thread@5
   LDY   #$4D
   LDA   (FP),Y
   LDY   #$65
   CMP   (FP),Y
   BNE   .thread@5
   LDY   #$4C
   LDA   (FP),Y
   LDY   #$64
   CMP   (FP),Y
   BNE   .thread@5
   LDY   #$4B
   LDA   (FP),Y
   LDY   #$63
   CMP   (FP),Y
   BNE   .thread@5
   LDY   #$4A
   LDA   (FP),Y
   LDY   #$62
   CMP   (FP),Y
   BNE   .thread@5
   LDY   #$49
   LDA   (FP),Y
   LDY   #$61
   CMP   (FP),Y
   BNE   .thread@5
   LDY   #$48
   LDA   (FP),Y
   LDY   #$60
   CMP   (FP),Y
   BNE   .thread@5
   LDY   #$47
   LDA   (FP),Y
   LDY   #$5F
   CMP   (FP),Y
   BEQ   .cmp_true@20
.thread@5:
   LDA   #$00
   JMP   .cmp_end@21
.cmp_true@20:
   LDA   #$01
.cmp_end@21:
   LDY   #$6D
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
   LDY   #$6F
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@7:
   LDY   #$6F
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lnot_true@23
   LDA   #$00
   JMP   .lnot_end@24
.lnot_true@23:
   LDA   #$01
.lnot_end@24:
   LDY   #$71
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
   LDA   #$03

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$8E
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$8D
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
   LDY   #$47
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
   INY
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
   JSR   dadd
   LDA   HARGS+16
   LDY   #$73
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
   LDY   #$73
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
   LDY   #$57
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
   JSR   dadd
   LDA   HARGS+16
   LDY   #$7B
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
   LDY   #$7B
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
   LDY   #$5F
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
   JSR   dadd
   LDA   HARGS+16
   LDY   #$83
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
   CMP   #$3F
   BNE   .cmp_true@25
   DEY
   LDA   (FP),Y
   CMP   #$E0
   BNE   .cmp_true@25
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@25
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@25
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@25
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@25
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@25
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@25
   LDA   #$00
   JMP   .cmp_end@26
.cmp_true@25:
   LDA   #$01
.cmp_end@26:
   LDY   #$8B
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
   LDA   #$04

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$8E
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
.if_end@11:
   LDX   #$00
   TXA

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$8E
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$8D
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
