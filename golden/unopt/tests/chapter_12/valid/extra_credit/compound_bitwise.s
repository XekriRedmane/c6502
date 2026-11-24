SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24

main:
   SUBROUTINE

   ; prologue: 0 arg bytes, 100 local bytes
   SEC
   LDA   SSP
   SBC   #$66
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$65
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   LDA   #$F8
   LDY   #$01
   STA   (FP),Y
   LDA   #$F9
   INY
   STA   (FP),Y
   LDA   #$FA
   INY
   STA   (FP),Y
   LDA   #$FB
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
   SEC
   SBC   #$E8
   INY
   STA   (FP),Y
   LDA   #$00
   SBC   #$03
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$0B
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$0C
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@0
   LDA   #$00
   JMP   .sx_done@1
.sx_neg@0:
   LDA   #$FF
.sx_done@1:
   LDY   #$0D
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
   LDY   #$01
   LDA   (FP),Y
   LDY   #$0B
   AND   (FP),Y
   LDY   #$13
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   LDY   #$0C
   AND   (FP),Y
   LDY   #$14
   STA   (FP),Y
   LDY   #$03
   LDA   (FP),Y
   LDY   #$0D
   AND   (FP),Y
   LDY   #$15
   STA   (FP),Y
   LDY   #$04
   LDA   (FP),Y
   LDY   #$0E
   AND   (FP),Y
   LDY   #$16
   STA   (FP),Y
   LDY   #$05
   LDA   (FP),Y
   LDY   #$0F
   AND   (FP),Y
   LDY   #$17
   STA   (FP),Y
   LDY   #$06
   LDA   (FP),Y
   LDY   #$10
   AND   (FP),Y
   LDY   #$18
   STA   (FP),Y
   LDY   #$07
   LDA   (FP),Y
   LDY   #$11
   AND   (FP),Y
   LDY   #$19
   STA   (FP),Y
   LDY   #$08
   LDA   (FP),Y
   LDY   #$12
   AND   (FP),Y
   LDY   #$1A
   STA   (FP),Y
   LDY   #$13
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   LDY   #$14
   LDA   (FP),Y
   LDY   #$02
   STA   (FP),Y
   LDY   #$15
   LDA   (FP),Y
   LDY   #$03
   STA   (FP),Y
   LDY   #$16
   LDA   (FP),Y
   LDY   #$04
   STA   (FP),Y
   LDY   #$17
   LDA   (FP),Y
   LDY   #$05
   STA   (FP),Y
   LDY   #$18
   LDA   (FP),Y
   LDY   #$06
   STA   (FP),Y
   LDY   #$19
   LDA   (FP),Y
   LDY   #$07
   STA   (FP),Y
   LDY   #$1A
   LDA   (FP),Y
   LDY   #$08
   STA   (FP),Y
   LDY   #$04
   LDA   (FP),Y
   CMP   #$FB
   BNE   .cmp_true@2
   DEY
   LDA   (FP),Y
   CMP   #$FA
   BNE   .cmp_true@2
   DEY
   LDA   (FP),Y
   CMP   #$F8
   BNE   .cmp_true@2
   DEY
   LDA   (FP),Y
   CMP   #$18
   BNE   .cmp_true@2
   LDA   #$00
   JMP   .cmp_end@3
.cmp_true@2:
   LDA   #$01
.cmp_end@3:
   LDY   #$1B
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
   ADC   #$66
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$65
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
   LDY   #$1D
   STA   (FP),Y
   LDA   #$FF
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
   LDY   #$01
   LDA   (FP),Y
   LDY   #$1D
   ORA   (FP),Y
   LDY   #$25
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   LDY   #$1E
   ORA   (FP),Y
   LDY   #$26
   STA   (FP),Y
   LDY   #$03
   LDA   (FP),Y
   LDY   #$1F
   ORA   (FP),Y
   LDY   #$27
   STA   (FP),Y
   LDY   #$04
   LDA   (FP),Y
   LDY   #$20
   ORA   (FP),Y
   LDY   #$28
   STA   (FP),Y
   LDY   #$05
   LDA   (FP),Y
   LDY   #$21
   ORA   (FP),Y
   LDY   #$29
   STA   (FP),Y
   LDY   #$06
   LDA   (FP),Y
   LDY   #$22
   ORA   (FP),Y
   LDY   #$2A
   STA   (FP),Y
   LDY   #$07
   LDA   (FP),Y
   LDY   #$23
   ORA   (FP),Y
   LDY   #$2B
   STA   (FP),Y
   LDY   #$08
   LDA   (FP),Y
   LDY   #$24
   ORA   (FP),Y
   LDY   #$2C
   STA   (FP),Y
   LDY   #$25
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   LDY   #$26
   LDA   (FP),Y
   LDY   #$02
   STA   (FP),Y
   LDY   #$27
   LDA   (FP),Y
   LDY   #$03
   STA   (FP),Y
   LDY   #$28
   LDA   (FP),Y
   LDY   #$04
   STA   (FP),Y
   LDY   #$29
   LDA   (FP),Y
   LDY   #$05
   STA   (FP),Y
   LDY   #$2A
   LDA   (FP),Y
   LDY   #$06
   STA   (FP),Y
   LDY   #$2B
   LDA   (FP),Y
   LDY   #$07
   STA   (FP),Y
   LDY   #$2C
   LDA   (FP),Y
   LDY   #$08
   STA   (FP),Y
   LDY   #$04
   LDA   (FP),Y
   CMP   #$FB
   BNE   .cmp_true@5
   DEY
   LDA   (FP),Y
   CMP   #$FA
   BNE   .cmp_true@5
   DEY
   LDA   (FP),Y
   CMP   #$FF
   BNE   .cmp_true@5
   DEY
   LDA   (FP),Y
   CMP   #$18
   BNE   .cmp_true@5
   LDA   #$00
   JMP   .cmp_end@6
.cmp_true@5:
   LDA   #$01
.cmp_end@6:
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
   ADC   #$66
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$65
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
   LDA   #$64
   LDY   #$2F
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDA   #$F0
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   SEC
   SBC   #$10
   INY
   STA   (FP),Y
   LDA   #$00
   SBC   #$0F
   INY
   STA   (FP),Y
   LDA   #$00
   SBC   #$00
   INY
   STA   (FP),Y
   LDA   #$00
   SBC   #$00
   INY
   STA   (FP),Y
   LDA   #$00
   SBC   #$00
   INY
   STA   (FP),Y
   LDA   #$00
   SBC   #$00
   INY
   STA   (FP),Y
   LDA   #$00
   SBC   #$00
   INY
   STA   (FP),Y
   LDA   #$00
   SBC   #$00
   INY
   STA   (FP),Y
   LDY   #$35
   LDA   (FP),Y
   LDY   #$3D
   STA   (FP),Y
   LDY   #$36
   LDA   (FP),Y
   LDY   #$3E
   STA   (FP),Y
   LDY   #$37
   LDA   (FP),Y
   LDY   #$3F
   STA   (FP),Y
   LDY   #$38
   LDA   (FP),Y
   LDY   #$40
   STA   (FP),Y
   LDY   #$39
   LDA   (FP),Y
   LDY   #$41
   STA   (FP),Y
   LDY   #$3A
   LDA   (FP),Y
   LDY   #$42
   STA   (FP),Y
   LDY   #$3B
   LDA   (FP),Y
   LDY   #$43
   STA   (FP),Y
   LDY   #$3C
   LDA   (FP),Y
   LDY   #$44
   STA   (FP),Y
   LDY   #$31
   LDA   (FP),Y
   LDY   #$45
   STA   (FP),Y
   LDY   #$32
   LDA   (FP),Y
   LDY   #$46
   STA   (FP),Y
   LDY   #$33
   LDA   (FP),Y
   LDY   #$47
   STA   (FP),Y
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
   LDY   #$45
   LDA   (FP),Y
   LDY   #$3D
   EOR   (FP),Y
   LDY   #$4D
   STA   (FP),Y
   LDY   #$46
   LDA   (FP),Y
   LDY   #$3E
   EOR   (FP),Y
   LDY   #$4E
   STA   (FP),Y
   LDY   #$47
   LDA   (FP),Y
   LDY   #$3F
   EOR   (FP),Y
   LDY   #$4F
   STA   (FP),Y
   LDY   #$48
   LDA   (FP),Y
   LDY   #$40
   EOR   (FP),Y
   LDY   #$50
   STA   (FP),Y
   LDY   #$49
   LDA   (FP),Y
   LDY   #$41
   EOR   (FP),Y
   LDY   #$51
   STA   (FP),Y
   LDY   #$4A
   LDA   (FP),Y
   LDY   #$42
   EOR   (FP),Y
   LDY   #$52
   STA   (FP),Y
   LDY   #$4B
   LDA   (FP),Y
   LDY   #$43
   EOR   (FP),Y
   LDY   #$53
   STA   (FP),Y
   LDY   #$4C
   LDA   (FP),Y
   LDY   #$44
   EOR   (FP),Y
   LDY   #$54
   STA   (FP),Y
   LDY   #$4D
   LDA   (FP),Y
   LDY   #$55
   STA   (FP),Y
   LDY   #$4E
   LDA   (FP),Y
   LDY   #$56
   STA   (FP),Y
   LDY   #$4F
   LDA   (FP),Y
   LDY   #$57
   STA   (FP),Y
   LDY   #$50
   LDA   (FP),Y
   LDY   #$58
   STA   (FP),Y
   LDY   #$55
   LDA   (FP),Y
   LDY   #$31
   STA   (FP),Y
   LDY   #$56
   LDA   (FP),Y
   LDY   #$32
   STA   (FP),Y
   LDY   #$57
   LDA   (FP),Y
   LDY   #$33
   STA   (FP),Y
   LDY   #$58
   LDA   (FP),Y
   LDY   #$34
   STA   (FP),Y
   LDY   #$55
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   INY
   ORA   (FP),Y
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
   ADC   #$66
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$65
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
   LDY   #$31
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
   ADC   #$66
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$65
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
   LDY   #$2F
   LDA   (FP),Y
   CMP   #$64
   BNE   .cmp_true@8
   LDA   #$00
   JMP   .cmp_end@9
.cmp_true@8:
   LDA   #$01
.cmp_end@9:
   LDY   #$59
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
   ADC   #$66
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$65
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
   SEC
   LDA   #$00
   SBC   #$10
   LDY   #$5B
   STA   (FP),Y
   LDA   #$00
   SBC   #$0F
   INY
   STA   (FP),Y
   LDA   #$00
   SBC   #$00
   INY
   STA   (FP),Y
   LDA   #$00
   SBC   #$00
   INY
   STA   (FP),Y
   LDA   #$00
   SBC   #$00
   INY
   STA   (FP),Y
   LDA   #$00
   SBC   #$00
   INY
   STA   (FP),Y
   LDA   #$00
   SBC   #$00
   INY
   STA   (FP),Y
   LDA   #$00
   SBC   #$00
   INY
   STA   (FP),Y
   LDY   #$44
   LDA   (FP),Y
   LDY   #$62
   CMP   (FP),Y
   BNE   .cmp_true@10
   LDY   #$43
   LDA   (FP),Y
   LDY   #$61
   CMP   (FP),Y
   BNE   .cmp_true@10
   LDY   #$42
   LDA   (FP),Y
   LDY   #$60
   CMP   (FP),Y
   BNE   .cmp_true@10
   LDY   #$41
   LDA   (FP),Y
   LDY   #$5F
   CMP   (FP),Y
   BNE   .cmp_true@10
   LDY   #$40
   LDA   (FP),Y
   LDY   #$5E
   CMP   (FP),Y
   BNE   .cmp_true@10
   LDY   #$3F
   LDA   (FP),Y
   LDY   #$5D
   CMP   (FP),Y
   BNE   .cmp_true@10
   LDY   #$3E
   LDA   (FP),Y
   LDY   #$5C
   CMP   (FP),Y
   BNE   .cmp_true@10
   LDY   #$3D
   LDA   (FP),Y
   LDY   #$5B
   CMP   (FP),Y
   BNE   .cmp_true@10
   LDA   #$00
   JMP   .cmp_end@11
.cmp_true@10:
   LDA   #$01
.cmp_end@11:
   LDY   #$63
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
   ADC   #$66
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
   ADC   #$66
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$65
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
