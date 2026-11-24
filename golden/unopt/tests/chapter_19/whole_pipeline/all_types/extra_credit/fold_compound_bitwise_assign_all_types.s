SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24

target_chars:
   SUBROUTINE

   ; prologue: 0 arg bytes, 144 local bytes
   SEC
   LDA   SSP
   SBC   #$92
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$91
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
   SBC   #$80
   LDY   #$01
   STA   (FP),Y
   LDA   #$00
   SBC   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$03
   STA   (FP),Y
   INY
   STA   (FP),Y
   SEC
   LDA   #$00
   SBC   #$78
   INY
   STA   (FP),Y
   LDA   #$00
   SBC   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$07
   STA   (FP),Y
   INY
   STA   (FP),Y
   SEC
   LDA   #$00
   SBC   #$02
   INY
   STA   (FP),Y
   LDA   #$00
   SBC   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$0B
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$01
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$78
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$AA
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$FA
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$FF
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$04
   LDA   (FP),Y
   LDY   #$19
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@0
   LDA   #$00
   JMP   .sx_done@1
.sx_neg@0:
   LDA   #$FF
.sx_done@1:
   LDY   #$1A
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   EOR   #$39
   LDY   #$1B
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   EOR   #$30
   LDY   #$1C
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$1D
   STA   (FP),Y
   LDY   #$04
   STA   (FP),Y
   LDY   #$18
   LDA   (FP),Y
   LDY   #$1E
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$08
   LDA   (FP),Y
   LDY   #$20
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@2
   LDA   #$00
   JMP   .sx_done@3
.sx_neg@2:
   LDA   #$FF
.sx_done@3:
   LDY   #$21
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$1E
   ORA   (FP),Y
   LDY   #$22
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$1F
   ORA   (FP),Y
   LDY   #$23
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$24
   STA   (FP),Y
   LDY   #$08
   STA   (FP),Y
   LDY   #$14
   LDA   (FP),Y
   LDY   #$25
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDA   #$B9
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$25
   LDA   (FP),Y
   SEC
   LDY   #$28
   SBC   (FP),Y
   LDY   #$2A
   STA   (FP),Y
   LDY   #$26
   LDA   (FP),Y
   LDY   #$29
   SBC   (FP),Y
   LDY   #$2B
   STA   (FP),Y
   LDY   #$0C
   LDA   (FP),Y
   LDY   #$2C
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@4
   LDA   #$00
   JMP   .sx_done@5
.sx_neg@4:
   LDA   #$FF
.sx_done@5:
   LDY   #$2D
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$2A
   AND   (FP),Y
   LDY   #$2E
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$2B
   AND   (FP),Y
   LDY   #$2F
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$30
   STA   (FP),Y
   LDY   #$0C
   STA   (FP),Y
   LDY   #$0E
   LDA   (FP),Y
   LDY   #$31
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@6
   LDA   #$00
   JMP   .sx_done@7
.sx_neg@6:
   LDA   #$FF
.sx_done@7:
   LDY   #$32
   STA   (FP),Y
   LSR
   DEY
   LDA   (FP),Y
   ROR
   LDY   #$34
   STA   (FP),Y
   LDA   #$00
   ROR
   DEY
   STA   (FP),Y
   LDY   #$35
   STA   (FP),Y
   LDY   #$0E
   STA   (FP),Y
   LDY   #$10
   LDA   (FP),Y
   LDY   #$36
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@8
   LDA   #$00
   JMP   .sx_done@9
.sx_neg@8:
   LDA   #$FF
.sx_done@9:
   LDY   #$37
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$38
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$39
   STA   (FP),Y
   LDX   #$1F
.shift_loop@10:
   LDY   #$39
   LDA   (FP),Y
   ASL
   LDA   (FP),Y
   ROR
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ROR
   STA   (FP),Y
   DEX
   BNE   .shift_loop@10
   LDY   #$3A
   STA   (FP),Y
   LDY   #$10
   STA   (FP),Y
   LDA   #$20
   LDY   #$3B
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   ORA   #$00
   BPL   .sx_done@13
   LDA   #$FF
.sx_done@13:
   LDY   #$3D
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$3B
   LDA   (FP),Y
   LDY   #$3F
   STA   (FP),Y
   LDY   #$3C
   LDA   (FP),Y
   LDY   #$40
   STA   (FP),Y
   LDY   #$3D
   LDA   (FP),Y
   LDY   #$41
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$18
   LDA   (FP),Y
   LDY   #$43
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ASL
   ASL
   ASL
   ASL
   LDY   #$46
   STA   (FP),Y
   LDA   #$00
   DEY
   STA   (FP),Y
   LDY   #$47
   STA   (FP),Y
   LDY   #$18
   STA   (FP),Y
   LDA   #$01
   LDY   #$48
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   ORA   #$00
   BPL   .sx_done@15
   LDA   #$FF
.sx_done@15:
   LDY   #$4A
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$3F
   LDA   (FP),Y
   SEC
   LDY   #$48
   SBC   (FP),Y
   LDY   #$4C
   STA   (FP),Y
   LDY   #$40
   LDA   (FP),Y
   LDY   #$49
   SBC   (FP),Y
   LDY   #$4D
   STA   (FP),Y
   LDY   #$41
   LDA   (FP),Y
   LDY   #$4A
   SBC   (FP),Y
   LDY   #$4E
   STA   (FP),Y
   LDY   #$42
   LDA   (FP),Y
   LDY   #$4B
   SBC   (FP),Y
   LDY   #$4F
   STA   (FP),Y
   LDY   #$16
   LDA   (FP),Y
   LDY   #$50
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$52
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$53
   STA   (FP),Y
   LDY   #$4C
   LDA   (FP),Y
   TAX
   BEQ   .shift_done@17
.shift_loop@16:
   LDY   #$53
   LDA   (FP),Y
   LSR
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ROR
   STA   (FP),Y
   DEX
   BNE   .shift_loop@16
.shift_done@17:
   LDY   #$52
   LDA   (FP),Y
   LDY   #$54
   STA   (FP),Y
   LDY   #$16
   STA   (FP),Y
   SEC
   LDA   #$00
   SBC   #$8F
   LDY   #$55
   STA   (FP),Y
   LDA   #$00
   SBC   #$01
   INY
   STA   (FP),Y
   LDY   #$14
   LDA   (FP),Y
   LDY   #$57
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$55
   ORA   (FP),Y
   LDY   #$59
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$56
   ORA   (FP),Y
   LDY   #$5A
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$5B
   STA   (FP),Y
   LDY   #$14
   STA   (FP),Y
   SEC
   LDA   #$00
   SBC   #$58
   LDY   #$5C
   STA   (FP),Y
   LDA   #$00
   SBC   #$E5
   INY
   STA   (FP),Y
   LDA   #$00
   SBC   #$11
   INY
   STA   (FP),Y
   LDA   #$00
   SBC   #$00
   INY
   STA   (FP),Y
   LDA   #$00
   SBC   #$01
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
   LDY   #$5C
   LDA   (FP),Y
   LDY   #$64
   STA   (FP),Y
   LDY   #$5D
   LDA   (FP),Y
   LDY   #$65
   STA   (FP),Y
   LDY   #$5E
   LDA   (FP),Y
   LDY   #$66
   STA   (FP),Y
   LDY   #$5F
   LDA   (FP),Y
   LDY   #$67
   STA   (FP),Y
   LDY   #$64
   LDA   (FP),Y
   LDY   #$3F
   STA   (FP),Y
   LDY   #$65
   LDA   (FP),Y
   LDY   #$40
   STA   (FP),Y
   LDY   #$66
   LDA   (FP),Y
   LDY   #$41
   STA   (FP),Y
   LDY   #$67
   LDA   (FP),Y
   LDY   #$42
   STA   (FP),Y
   LDY   #$12
   LDA   (FP),Y
   LDY   #$68
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$68
   LDA   (FP),Y
   LDY   #$3F
   EOR   (FP),Y
   LDY   #$6C
   STA   (FP),Y
   LDY   #$69
   LDA   (FP),Y
   LDY   #$40
   EOR   (FP),Y
   LDY   #$6D
   STA   (FP),Y
   LDY   #$6A
   LDA   (FP),Y
   LDY   #$41
   EOR   (FP),Y
   LDY   #$6E
   STA   (FP),Y
   LDY   #$6B
   LDA   (FP),Y
   LDY   #$42
   EOR   (FP),Y
   LDY   #$6F
   STA   (FP),Y
   LDY   #$6C
   LDA   (FP),Y
   LDY   #$70
   STA   (FP),Y
   LDY   #$12
   STA   (FP),Y
   LDY   #$04
   LDA   (FP),Y
   LDY   #$71
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@18
   LDA   #$00
   JMP   .sx_done@19
.sx_neg@18:
   LDA   #$FF
.sx_done@19:
   LDY   #$72
   STA   (FP),Y
   SEC
   LDA   #$00
   SBC   #$47
   INY
   STA   (FP),Y
   LDA   #$00
   SBC   #$00
   INY
   STA   (FP),Y
   LDY   #$72
   LDA   (FP),Y
   LDY   #$74
   CMP   (FP),Y
   BNE   .cmp_true@20
   LDY   #$71
   LDA   (FP),Y
   LDY   #$73
   CMP   (FP),Y
   BNE   .cmp_true@20
   LDA   #$00
   JMP   .cmp_end@21
.cmp_true@20:
   LDA   #$01
.cmp_end@21:
   LDY   #$75
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
   ADC   #$92
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$91
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
   LDY   #$08
   LDA   (FP),Y
   LDY   #$77
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@23
   LDA   #$00
   JMP   .sx_done@24
.sx_neg@23:
   LDA   #$FF
.sx_done@24:
   LDY   #$78
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
   LDY   #$78
   LDA   (FP),Y
   LDY   #$7A
   CMP   (FP),Y
   BNE   .cmp_true@25
   LDY   #$77
   LDA   (FP),Y
   LDY   #$79
   CMP   (FP),Y
   BNE   .cmp_true@25
   LDA   #$00
   JMP   .cmp_end@26
.cmp_true@25:
   LDA   #$01
.cmp_end@26:
   LDY   #$7B
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
   ADC   #$92
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$91
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
   LDY   #$0C
   LDA   (FP),Y
   LDY   #$7D
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@28
   LDA   #$00
   JMP   .sx_done@29
.sx_neg@28:
   LDA   #$FF
.sx_done@29:
   LDY   #$7E
   STA   (FP),Y
   SEC
   LDA   #$00
   SBC   #$10
   INY
   STA   (FP),Y
   LDA   #$00
   SBC   #$00
   INY
   STA   (FP),Y
   LDY   #$7E
   LDA   (FP),Y
   LDY   #$80
   CMP   (FP),Y
   BNE   .cmp_true@30
   LDY   #$7D
   LDA   (FP),Y
   LDY   #$7F
   CMP   (FP),Y
   BNE   .cmp_true@30
   LDA   #$00
   JMP   .cmp_end@31
.cmp_true@30:
   LDA   #$01
.cmp_end@31:
   LDY   #$81
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
   ADC   #$92
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$91
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
   LDY   #$0E
   LDA   (FP),Y
   LDY   #$83
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@33
   LDA   #$00
   JMP   .sx_done@34
.sx_neg@33:
   LDA   #$FF
.sx_done@34:
   LDY   #$84
   STA   (FP),Y
   SEC
   LDA   #$00
   SBC   #$80
   INY
   STA   (FP),Y
   LDA   #$00
   SBC   #$00
   INY
   STA   (FP),Y
   LDY   #$84
   LDA   (FP),Y
   LDY   #$86
   CMP   (FP),Y
   BNE   .cmp_true@35
   LDY   #$83
   LDA   (FP),Y
   LDY   #$85
   CMP   (FP),Y
   BNE   .cmp_true@35
   LDA   #$00
   JMP   .cmp_end@36
.cmp_true@35:
   LDA   #$01
.cmp_end@36:
   LDY   #$87
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
   ADC   #$92
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$91
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
   LDY   #$10
   LDA   (FP),Y
   BEQ   .if_end@4
   LDX   #$00
   LDA   #$05

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$92
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$91
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
   LDY   #$12
   LDA   (FP),Y
   LDY   #$89
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CMP   #$A8
   BNE   .cmp_true@38
   LDA   #$00
   JMP   .cmp_end@39
.cmp_true@38:
   LDA   #$01
.cmp_end@39:
   LDY   #$8B
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
   ADC   #$92
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$91
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
   LDY   #$14
   LDA   (FP),Y
   LDY   #$8D
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CMP   #$FB
   BNE   .cmp_true@40
   LDA   #$00
   JMP   .cmp_end@41
.cmp_true@40:
   LDA   #$01
.cmp_end@41:
   LDY   #$8F
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
   ADC   #$92
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
.if_end@6:
   LDY   #$16
   LDA   (FP),Y
   BEQ   .if_end@7
   LDX   #$00
   LDA   #$08

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$92
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$91
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
   LDY   #$18
   LDA   (FP),Y
   BEQ   .if_end@8
   LDX   #$00
   LDA   #$09

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$92
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$91
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
   LDX   #$00
   TXA

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$92
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$91
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

target_long_bitwise:
   SUBROUTINE

   ; prologue: 0 arg bytes, 184 local bytes
   SEC
   LDA   SSP
   SBC   #$BA
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$B9
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   LDA   #$FF
   LDY   #$01
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDA   #$FF
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$01
   LDA   (FP),Y
   LDY   #$05
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
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
   SEC
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
   SBC   #$01
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
   LDY   #$09
   LDA   (FP),Y
   LDY   #$11
   STA   (FP),Y
   LDY   #$0A
   LDA   (FP),Y
   LDY   #$12
   STA   (FP),Y
   LDY   #$0B
   LDA   (FP),Y
   LDY   #$13
   STA   (FP),Y
   LDY   #$0C
   LDA   (FP),Y
   LDY   #$14
   STA   (FP),Y
   LDY   #$11
   LDA   (FP),Y
   LDY   #$15
   STA   (FP),Y
   LDY   #$12
   LDA   (FP),Y
   LDY   #$16
   STA   (FP),Y
   LDY   #$13
   LDA   (FP),Y
   LDY   #$17
   STA   (FP),Y
   LDY   #$14
   LDA   (FP),Y
   LDY   #$18
   STA   (FP),Y
   LDY   #$05
   LDA   (FP),Y
   LDY   #$15
   AND   (FP),Y
   LDY   #$19
   STA   (FP),Y
   LDY   #$06
   LDA   (FP),Y
   LDY   #$16
   AND   (FP),Y
   LDY   #$1A
   STA   (FP),Y
   LDY   #$07
   LDA   (FP),Y
   LDY   #$17
   AND   (FP),Y
   LDY   #$1B
   STA   (FP),Y
   LDY   #$08
   LDA   (FP),Y
   LDY   #$18
   AND   (FP),Y
   LDY   #$1C
   STA   (FP),Y
   LDY   #$19
   LDA   (FP),Y
   LDY   #$05
   STA   (FP),Y
   LDY   #$1A
   LDA   (FP),Y
   LDY   #$06
   STA   (FP),Y
   LDY   #$1B
   LDA   (FP),Y
   LDY   #$07
   STA   (FP),Y
   LDY   #$1C
   LDA   (FP),Y
   LDY   #$08
   STA   (FP),Y
   LDY   #$05
   LDA   (FP),Y
   LDY   #$1D
   STA   (FP),Y
   LDY   #$06
   LDA   (FP),Y
   LDY   #$1E
   STA   (FP),Y
   LDY   #$07
   LDA   (FP),Y
   LDY   #$1F
   STA   (FP),Y
   LDY   #$08
   LDA   (FP),Y
   LDY   #$20
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@42
   LDA   #$00
   JMP   .sx_done@43
.sx_neg@42:
   LDA   #$FF
.sx_done@43:
   LDY   #$21
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@44
   CMP   #$FF
   BNE   .cmp_true@44
   CMP   #$00
   BNE   .cmp_true@44
   CMP   #$FF
   BNE   .cmp_true@44
   LDY   #$20
   LDA   (FP),Y
   BNE   .cmp_true@44
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@44
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@44
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@44
   LDA   #$00
   JMP   .cmp_end@45
.cmp_true@44:
   LDA   #$01
.cmp_end@45:
   LDY   #$25
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
   LDA   #$01

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$BA
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$B9
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
   LDY   #$15
   LDA   (FP),Y
   ORA   #$64
   LDY   #$27
   STA   (FP),Y
   LDY   #$16
   LDA   (FP),Y
   LDY   #$28
   STA   (FP),Y
   LDY   #$17
   LDA   (FP),Y
   LDY   #$29
   STA   (FP),Y
   LDY   #$18
   LDA   (FP),Y
   LDY   #$2A
   STA   (FP),Y
   LDY   #$27
   LDA   (FP),Y
   LDY   #$15
   STA   (FP),Y
   LDY   #$28
   LDA   (FP),Y
   LDY   #$16
   STA   (FP),Y
   LDY   #$29
   LDA   (FP),Y
   LDY   #$17
   STA   (FP),Y
   LDY   #$2A
   LDA   (FP),Y
   LDY   #$18
   STA   (FP),Y
   LDY   #$15
   LDA   (FP),Y
   LDY   #$2B
   STA   (FP),Y
   LDY   #$16
   LDA   (FP),Y
   LDY   #$2C
   STA   (FP),Y
   LDY   #$17
   LDA   (FP),Y
   LDY   #$2D
   STA   (FP),Y
   LDY   #$18
   LDA   (FP),Y
   LDY   #$2E
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@47
   LDA   #$00
   JMP   .sx_done@48
.sx_neg@47:
   LDA   #$FF
.sx_done@48:
   LDY   #$2F
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   SEC
   LDA   #$00
   SBC   #$9C
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
   SBC   #$FF
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
   LDY   #$32
   LDA   (FP),Y
   LDY   #$3A
   CMP   (FP),Y
   BNE   .cmp_true@49
   DEY
   CMP   (FP),Y
   BNE   .cmp_true@49
   DEY
   CMP   (FP),Y
   BNE   .cmp_true@49
   DEY
   CMP   (FP),Y
   BNE   .cmp_true@49
   LDY   #$2E
   LDA   (FP),Y
   LDY   #$36
   CMP   (FP),Y
   BNE   .cmp_true@49
   LDY   #$2D
   LDA   (FP),Y
   LDY   #$35
   CMP   (FP),Y
   BNE   .cmp_true@49
   LDY   #$2C
   LDA   (FP),Y
   LDY   #$34
   CMP   (FP),Y
   BNE   .cmp_true@49
   LDY   #$2B
   LDA   (FP),Y
   LDY   #$33
   CMP   (FP),Y
   BNE   .cmp_true@49
   LDA   #$00
   JMP   .cmp_end@50
.cmp_true@49:
   LDA   #$01
.cmp_end@50:
   LDY   #$3B
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
   LDA   #$02

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$BA
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$B9
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
   SEC
   LDA   #$00
   SBC   #$FF
   LDY   #$3D
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
   SBC   #$FF
   INY
   STA   (FP),Y
   LDA   #$00
   SBC   #$7F
   INY
   STA   (FP),Y
   LDY   #$05
   LDA   (FP),Y
   LDY   #$45
   STA   (FP),Y
   LDY   #$06
   LDA   (FP),Y
   LDY   #$46
   STA   (FP),Y
   LDY   #$07
   LDA   (FP),Y
   LDY   #$47
   STA   (FP),Y
   LDY   #$08
   LDA   (FP),Y
   LDY   #$48
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@52
   LDA   #$00
   JMP   .sx_done@53
.sx_neg@52:
   LDA   #$FF
.sx_done@53:
   LDY   #$49
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
   LDY   #$05
   STA   (FP),Y
   LDY   #$56
   LDA   (FP),Y
   LDY   #$06
   STA   (FP),Y
   LDY   #$57
   LDA   (FP),Y
   LDY   #$07
   STA   (FP),Y
   LDY   #$58
   LDA   (FP),Y
   LDY   #$08
   STA   (FP),Y
   LDY   #$05
   LDA   (FP),Y
   LDY   #$59
   STA   (FP),Y
   LDY   #$06
   LDA   (FP),Y
   LDY   #$5A
   STA   (FP),Y
   LDY   #$07
   LDA   (FP),Y
   LDY   #$5B
   STA   (FP),Y
   LDY   #$08
   LDA   (FP),Y
   LDY   #$5C
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@54
   LDA   #$00
   JMP   .sx_done@55
.sx_neg@54:
   LDA   #$FF
.sx_done@55:
   LDY   #$5D
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
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
   SBC   #$7F
   INY
   STA   (FP),Y
   LDY   #$60
   LDA   (FP),Y
   LDY   #$68
   CMP   (FP),Y
   BNE   .cmp_true@56
   DEY
   CMP   (FP),Y
   BNE   .cmp_true@56
   DEY
   CMP   (FP),Y
   BNE   .cmp_true@56
   DEY
   CMP   (FP),Y
   BNE   .cmp_true@56
   LDY   #$5C
   LDA   (FP),Y
   LDY   #$64
   CMP   (FP),Y
   BNE   .cmp_true@56
   LDY   #$5B
   LDA   (FP),Y
   LDY   #$63
   CMP   (FP),Y
   BNE   .cmp_true@56
   LDY   #$5A
   LDA   (FP),Y
   LDY   #$62
   CMP   (FP),Y
   BNE   .cmp_true@56
   LDY   #$59
   LDA   (FP),Y
   LDY   #$61
   CMP   (FP),Y
   BNE   .cmp_true@56
   LDA   #$00
   JMP   .cmp_end@57
.cmp_true@56:
   LDA   #$01
.cmp_end@57:
   LDY   #$69
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
   LDA   #$03

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$BA
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$B9
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
   LDA   #$FF
   LDY   #$6B
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$05
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
   LDY   #$6F
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
   SBC   #$40
   INY
   STA   (FP),Y
   LDY   #$6F
   LDA   (FP),Y
   LDY   #$73
   STA   (FP),Y
   LDY   #$70
   LDA   (FP),Y
   LDY   #$74
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$75
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$76
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$77
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$78
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@59
   LDA   #$00
   JMP   .sx_done@60
.sx_neg@59:
   LDA   #$FF
.sx_done@60:
   LDY   #$79
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$05
   LDA   (FP),Y
   LDY   #$77
   AND   (FP),Y
   LDY   #$7B
   STA   (FP),Y
   LDY   #$06
   LDA   (FP),Y
   LDY   #$78
   AND   (FP),Y
   LDY   #$7C
   STA   (FP),Y
   LDY   #$07
   LDA   (FP),Y
   LDY   #$79
   AND   (FP),Y
   LDY   #$7D
   STA   (FP),Y
   LDY   #$08
   LDA   (FP),Y
   LDY   #$7A
   AND   (FP),Y
   LDY   #$7E
   STA   (FP),Y
   LDY   #$7B
   LDA   (FP),Y
   LDY   #$05
   STA   (FP),Y
   LDY   #$7C
   LDA   (FP),Y
   LDY   #$06
   STA   (FP),Y
   LDY   #$7D
   LDA   (FP),Y
   LDY   #$07
   STA   (FP),Y
   LDY   #$7E
   LDA   (FP),Y
   LDY   #$08
   STA   (FP),Y
   LDY   #$05
   LDA   (FP),Y
   LDY   #$7F
   STA   (FP),Y
   LDY   #$06
   LDA   (FP),Y
   LDY   #$80
   STA   (FP),Y
   LDY   #$07
   LDA   (FP),Y
   LDY   #$81
   STA   (FP),Y
   LDY   #$08
   LDA   (FP),Y
   LDY   #$82
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@61
   LDA   #$00
   JMP   .sx_done@62
.sx_neg@61:
   LDA   #$FF
.sx_done@62:
   LDY   #$83
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   CMP   #$3F
   BNE   .cmp_true@63
   CMP   #$FF
   BNE   .cmp_true@63
   LDY   #$82
   LDA   (FP),Y
   CMP   #$C0
   BNE   .cmp_true@63
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@63
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@63
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@63
   LDA   #$00
   JMP   .cmp_end@64
.cmp_true@63:
   LDA   #$01
.cmp_end@64:
   LDY   #$87
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
   LDA   #$04

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$BA
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$B9
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
   SEC
   LDA   #$00
   SBC   #$00
   LDY   #$89
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
   SBC   #$80
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
   LDY   #$89
   LDA   (FP),Y
   LDY   #$91
   STA   (FP),Y
   LDY   #$8A
   LDA   (FP),Y
   LDY   #$92
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$75
   STA   (FP),Y
   LDY   #$92
   LDA   (FP),Y
   LDY   #$76
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$93
   STA   (FP),Y
   LDY   #$76
   LDA   (FP),Y
   LDY   #$94
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@66
   LDA   #$00
   JMP   .sx_done@67
.sx_neg@66:
   LDA   #$FF
.sx_done@67:
   LDY   #$95
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
   LDY   #$93
   LDA   (FP),Y
   LDA   #$FF
   LDY   #$9B
   STA   (FP),Y
   LDY   #$94
   LDA   (FP),Y
   LDY   #$9C
   STA   (FP),Y
   LDY   #$95
   LDA   (FP),Y
   LDA   #$FF
   LDY   #$9D
   STA   (FP),Y
   LDY   #$96
   LDA   (FP),Y
   LDY   #$9E
   STA   (FP),Y
   LDA   #$FF
   INY
   STA   (FP),Y
   LDY   #$98
   LDA   (FP),Y
   LDY   #$A0
   STA   (FP),Y
   LDA   #$FF
   INY
   STA   (FP),Y
   LDY   #$9A
   LDA   (FP),Y
   LDY   #$A2
   STA   (FP),Y
   LDY   #$9B
   LDA   (FP),Y
   LDY   #$A3
   STA   (FP),Y
   LDY   #$9C
   LDA   (FP),Y
   LDY   #$A4
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$75
   STA   (FP),Y
   LDY   #$A4
   LDA   (FP),Y
   LDY   #$76
   STA   (FP),Y
   LDY   #$A3
   LDA   (FP),Y
   LDY   #$A5
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$A6
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@68
   LDA   #$00
   JMP   .sx_done@69
.sx_neg@68:
   LDA   #$FF
.sx_done@69:
   LDY   #$A7
   STA   (FP),Y
   INY
   STA   (FP),Y
   SEC
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
   SBC   #$7F
   INY
   STA   (FP),Y
   LDY   #$A8
   LDA   (FP),Y
   LDY   #$AC
   CMP   (FP),Y
   BNE   .cmp_true@70
   DEY
   CMP   (FP),Y
   BNE   .cmp_true@70
   LDY   #$A6
   LDA   (FP),Y
   LDY   #$AA
   CMP   (FP),Y
   BNE   .cmp_true@70
   LDY   #$A5
   LDA   (FP),Y
   LDY   #$A9
   CMP   (FP),Y
   BNE   .cmp_true@70
   LDA   #$00
   JMP   .cmp_end@71
.cmp_true@70:
   LDA   #$01
.cmp_end@71:
   LDY   #$AD
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@13
   LDX   #$00
   LDA   #$05

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$BA
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$B9
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
.if_end@13:
   LDY   #$75
   LDA   (FP),Y
   LDY   #$AF
   STA   (FP),Y
   LDY   #$76
   LDA   (FP),Y
   LDY   #$B0
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@73
   LDA   #$00
   JMP   .sx_done@74
.sx_neg@73:
   LDA   #$FF
.sx_done@74:
   LDY   #$B1
   STA   (FP),Y
   INY
   STA   (FP),Y
   SEC
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
   SBC   #$7F
   INY
   STA   (FP),Y
   LDY   #$B2
   LDA   (FP),Y
   LDY   #$B6
   CMP   (FP),Y
   BNE   .cmp_true@75
   DEY
   CMP   (FP),Y
   BNE   .cmp_true@75
   LDY   #$B0
   LDA   (FP),Y
   LDY   #$B4
   CMP   (FP),Y
   BNE   .cmp_true@75
   LDY   #$AF
   LDA   (FP),Y
   LDY   #$B3
   CMP   (FP),Y
   BNE   .cmp_true@75
   LDA   #$00
   JMP   .cmp_end@76
.cmp_true@75:
   LDA   #$01
.cmp_end@76:
   LDY   #$B7
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@14
   LDX   #$00
   LDA   #$06

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$BA
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
.if_end@14:
   LDX   #$00
   TXA

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$BA
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$B9
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

target_long_bitshift:
   SUBROUTINE

   ; prologue: 0 arg bytes, 68 local bytes
   SEC
   LDA   SSP
   SBC   #$46
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$45
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   LDA   #$64
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
   LDX   #$16
.shift_loop@78:
   LDY   #$03
   LDA   (FP),Y
   ASL
   STA   (FP),Y
   INY
   LDA   (FP),Y
   ROL
   STA   (FP),Y
   DEX
   BNE   .shift_loop@78
   DEY
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   LDY   #$04
   LDA   (FP),Y
   LDY   #$02
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$05
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   LDY   #$06
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@80
   LDA   #$00
   JMP   .sx_done@81
.sx_neg@80:
   LDA   #$FF
.sx_done@81:
   LDY   #$07
   STA   (FP),Y
   INY
   STA   (FP),Y
   CMP   #$19
   BNE   .cmp_true@82
   CMP   #$00
   BNE   .cmp_true@82
   LDY   #$06
   LDA   (FP),Y
   BNE   .cmp_true@82
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@82
   LDA   #$00
   JMP   .cmp_end@83
.cmp_true@82:
   LDA   #$01
.cmp_end@83:
   LDY   #$09
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
   LDY   #$45
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
   LDY   #$01
   LDA   (FP),Y
   LDY   #$0B
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   LDY   #$0C
   STA   (FP),Y
   LDX   #$04
.shift_loop@85:
   LDY   #$0C
   LDA   (FP),Y
   ASL
   LDA   (FP),Y
   ROR
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ROR
   STA   (FP),Y
   DEX
   BNE   .shift_loop@85
   LDY   #$01
   STA   (FP),Y
   LDY   #$0C
   LDA   (FP),Y
   LDY   #$02
   STA   (FP),Y
   LDY   #$0B
   LDA   (FP),Y
   LDY   #$0D
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$0E
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@87
   LDA   #$00
   JMP   .sx_done@88
.sx_neg@87:
   LDA   #$FF
.sx_done@88:
   LDY   #$0F
   STA   (FP),Y
   INY
   STA   (FP),Y
   CMP   #$01
   BNE   .cmp_true@89
   CMP   #$90
   BNE   .cmp_true@89
   LDY   #$0E
   LDA   (FP),Y
   BNE   .cmp_true@89
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@89
   LDA   #$00
   JMP   .cmp_end@90
.cmp_true@89:
   LDA   #$01
.cmp_end@90:
   LDY   #$11
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@16
   LDX   #$00
   LDA   #$02

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
   LDY   #$45
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
.if_end@16:
   LDY   #$01
   LDA   (FP),Y
   LDY   #$13
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   LDY   #$14
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@92
   LDA   #$00
   JMP   .sx_done@93
.sx_neg@92:
   LDA   #$FF
.sx_done@93:
   LDY   #$15
   STA   (FP),Y
   INY
   STA   (FP),Y
   CMP   #$01
   BNE   .cmp_true@94
   CMP   #$90
   BNE   .cmp_true@94
   LDY   #$14
   LDA   (FP),Y
   BNE   .cmp_true@94
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@94
   LDA   #$00
   JMP   .cmp_end@95
.cmp_true@94:
   LDA   #$01
.cmp_end@95:
   LDY   #$17
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@17
   LDX   #$00
   LDA   #$03

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
   LDY   #$45
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
   LDA   #$39
   LDY   #$19
   STA   (FP),Y
   LDA   #$30
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$19
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
   LDA   #$21
   STA   HARGS+4
   JSR   asl32
   LDA   HARGS+5
   LDY   #$1D
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
   LDY   #$1D
   LDA   (FP),Y
   LDY   #$19
   STA   (FP),Y
   LDY   #$1E
   LDA   (FP),Y
   LDY   #$1A
   STA   (FP),Y
   LDY   #$1F
   LDA   (FP),Y
   LDY   #$1B
   STA   (FP),Y
   LDY   #$20
   LDA   (FP),Y
   LDY   #$1C
   STA   (FP),Y
   INY
   LDA   (FP),Y
   LDY   #$21
   STA   (FP),Y
   LDY   #$1E
   LDA   (FP),Y
   LDY   #$22
   STA   (FP),Y
   LDY   #$1F
   LDA   (FP),Y
   LDY   #$23
   STA   (FP),Y
   LDY   #$20
   LDA   (FP),Y
   LDY   #$24
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@97
   LDA   #$00
   JMP   .sx_done@98
.sx_neg@97:
   LDA   #$FF
.sx_done@98:
   LDY   #$25
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@99
   CMP   #$60
   BNE   .cmp_true@99
   CMP   #$72
   BNE   .cmp_true@99
   LDY   #$24
   LDA   (FP),Y
   BNE   .cmp_true@99
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@99
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@99
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@99
   LDA   #$00
   JMP   .cmp_end@100
.cmp_true@99:
   LDA   #$01
.cmp_end@100:
   LDY   #$29
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
   LDA   #$04

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
   LDY   #$45
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
   SEC
   LDA   #$00
   LDY   #$19
   SBC   (FP),Y
   LDY   #$2B
   STA   (FP),Y
   LDA   #$00
   LDY   #$1A
   SBC   (FP),Y
   LDY   #$2C
   STA   (FP),Y
   LDA   #$00
   LDY   #$1B
   SBC   (FP),Y
   LDY   #$2D
   STA   (FP),Y
   LDA   #$00
   LDY   #$1C
   SBC   (FP),Y
   LDY   #$2E
   STA   (FP),Y
   LDY   #$2B
   LDA   (FP),Y
   LDY   #$19
   STA   (FP),Y
   LDY   #$2C
   LDA   (FP),Y
   LDY   #$1A
   STA   (FP),Y
   LDY   #$2D
   LDA   (FP),Y
   LDY   #$1B
   STA   (FP),Y
   LDY   #$2E
   LDA   (FP),Y
   LDY   #$1C
   STA   (FP),Y
   LDY   #$19
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
   LDA   #$0A
   STA   HARGS+4
   JSR   asr32
   LDA   HARGS+5
   LDY   #$2F
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
   LDY   #$2F
   LDA   (FP),Y
   LDY   #$19
   STA   (FP),Y
   LDY   #$30
   LDA   (FP),Y
   LDY   #$1A
   STA   (FP),Y
   LDY   #$31
   LDA   (FP),Y
   LDY   #$1B
   STA   (FP),Y
   LDY   #$32
   LDA   (FP),Y
   LDY   #$1C
   STA   (FP),Y
   LDY   #$2F
   LDA   (FP),Y
   LDY   #$33
   STA   (FP),Y
   LDY   #$30
   LDA   (FP),Y
   LDY   #$34
   STA   (FP),Y
   LDY   #$31
   LDA   (FP),Y
   LDY   #$35
   STA   (FP),Y
   LDY   #$32
   LDA   (FP),Y
   LDY   #$36
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@102
   LDA   #$00
   JMP   .sx_done@103
.sx_neg@102:
   LDA   #$FF
.sx_done@103:
   LDY   #$37
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
   SBC   #$00
   INY
   STA   (FP),Y
   LDA   #$00
   SBC   #$80
   INY
   STA   (FP),Y
   LDA   #$00
   SBC   #$1C
   INY
   STA   (FP),Y
   LDA   #$00
   SBC   #$18
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
   LDY   #$3A
   LDA   (FP),Y
   LDY   #$42
   CMP   (FP),Y
   BNE   .cmp_true@104
   DEY
   CMP   (FP),Y
   BNE   .cmp_true@104
   DEY
   CMP   (FP),Y
   BNE   .cmp_true@104
   DEY
   CMP   (FP),Y
   BNE   .cmp_true@104
   LDY   #$36
   LDA   (FP),Y
   LDY   #$3E
   CMP   (FP),Y
   BNE   .cmp_true@104
   LDY   #$35
   LDA   (FP),Y
   LDY   #$3D
   CMP   (FP),Y
   BNE   .cmp_true@104
   LDY   #$34
   LDA   (FP),Y
   LDY   #$3C
   CMP   (FP),Y
   BNE   .cmp_true@104
   LDY   #$33
   LDA   (FP),Y
   LDY   #$3B
   CMP   (FP),Y
   BNE   .cmp_true@104
   LDA   #$00
   JMP   .cmp_end@105
.cmp_true@104:
   LDA   #$01
.cmp_end@105:
   LDY   #$43
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
   LDA   #$05

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
.if_end@19:
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
   LDY   #$45
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

target_unsigned_bitwise:
   SUBROUTINE

   ; prologue: 0 arg bytes, 80 local bytes
   SEC
   LDA   SSP
   SBC   #$52
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$51
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
   LDY   #$01
   LDA   (FP),Y
   LDY   #$05
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
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
   SEC
   LDA   #$00
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
   BMI   .sx_neg@107
   LDA   #$00
   JMP   .sx_done@108
.sx_neg@107:
   LDA   #$FF
.sx_done@108:
   LDY   #$0D
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$05
   LDA   (FP),Y
   LDY   #$0B
   AND   (FP),Y
   LDY   #$0F
   STA   (FP),Y
   LDY   #$06
   LDA   (FP),Y
   LDY   #$0C
   AND   (FP),Y
   LDY   #$10
   STA   (FP),Y
   LDY   #$07
   LDA   (FP),Y
   LDY   #$0D
   AND   (FP),Y
   LDY   #$11
   STA   (FP),Y
   LDY   #$08
   LDA   (FP),Y
   LDY   #$0E
   AND   (FP),Y
   LDY   #$12
   STA   (FP),Y
   LDY   #$0F
   LDA   (FP),Y
   LDY   #$05
   STA   (FP),Y
   LDY   #$10
   LDA   (FP),Y
   LDY   #$06
   STA   (FP),Y
   LDY   #$11
   LDA   (FP),Y
   LDY   #$07
   STA   (FP),Y
   LDY   #$12
   LDA   (FP),Y
   LDY   #$08
   STA   (FP),Y
   LDY   #$05
   LDA   (FP),Y
   LDY   #$13
   STA   (FP),Y
   LDY   #$06
   LDA   (FP),Y
   LDY   #$14
   STA   (FP),Y
   LDY   #$07
   LDA   (FP),Y
   LDY   #$15
   STA   (FP),Y
   LDY   #$08
   LDA   (FP),Y
   LDY   #$16
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
   CMP   #$FF
   BNE   .cmp_true@109
   CMP   #$FE
   BNE   .cmp_true@109
   CMP   #$FD
   BNE   .cmp_true@109
   CMP   #$FC
   BNE   .cmp_true@109
   LDY   #$16
   LDA   (FP),Y
   CMP   #$FB
   BNE   .cmp_true@109
   DEY
   LDA   (FP),Y
   CMP   #$FA
   BNE   .cmp_true@109
   DEY
   LDA   (FP),Y
   CMP   #$F8
   BNE   .cmp_true@109
   DEY
   LDA   (FP),Y
   CMP   #$18
   BNE   .cmp_true@109
   LDA   #$00
   JMP   .cmp_end@110
.cmp_true@109:
   LDA   #$01
.cmp_end@110:
   LDY   #$1B
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
   LDA   #$01

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$52
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$51
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
   LDY   #$05
   LDA   (FP),Y
   LDY   #$1D
   STA   (FP),Y
   LDY   #$06
   LDA   (FP),Y
   LDA   #$FF
   LDY   #$1E
   STA   (FP),Y
   LDY   #$07
   LDA   (FP),Y
   LDA   #$FF
   LDY   #$1F
   STA   (FP),Y
   LDY   #$08
   LDA   (FP),Y
   LDA   #$FF
   LDY   #$20
   STA   (FP),Y
   LDY   #$1D
   LDA   (FP),Y
   LDY   #$05
   STA   (FP),Y
   LDY   #$1E
   LDA   (FP),Y
   LDY   #$06
   STA   (FP),Y
   LDY   #$1F
   LDA   (FP),Y
   LDY   #$07
   STA   (FP),Y
   LDY   #$20
   LDA   (FP),Y
   LDY   #$08
   STA   (FP),Y
   LDY   #$05
   LDA   (FP),Y
   LDY   #$21
   STA   (FP),Y
   LDY   #$06
   LDA   (FP),Y
   LDY   #$22
   STA   (FP),Y
   LDY   #$07
   LDA   (FP),Y
   LDY   #$23
   STA   (FP),Y
   LDY   #$08
   LDA   (FP),Y
   LDY   #$24
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
   CMP   #$FF
   BNE   .cmp_true@112
   CMP   #$FE
   BNE   .cmp_true@112
   CMP   #$FD
   BNE   .cmp_true@112
   CMP   #$FC
   BNE   .cmp_true@112
   LDY   #$24
   LDA   (FP),Y
   CMP   #$FF
   BNE   .cmp_true@112
   DEY
   LDA   (FP),Y
   CMP   #$FF
   BNE   .cmp_true@112
   DEY
   LDA   (FP),Y
   CMP   #$FF
   BNE   .cmp_true@112
   DEY
   LDA   (FP),Y
   CMP   #$18
   BNE   .cmp_true@112
   LDA   #$00
   JMP   .cmp_end@113
.cmp_true@112:
   LDA   #$01
.cmp_end@113:
   LDY   #$29
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
   LDA   #$02

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$52
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$51
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
   LDA   #$40
   LDY   #$2B
   STA   (FP),Y
   LDA   #$E2
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$2D
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$2E
   STA   (FP),Y
   LDA   #$F0
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   SEC
   LDA   #$00
   SBC   #$10
   INY
   STA   (FP),Y
   LDA   #$00
   SBC   #$0F
   INY
   STA   (FP),Y
   LDA   #$00
   SBC   #$0F
   INY
   STA   (FP),Y
   LDA   #$00
   SBC   #$0F
   INY
   STA   (FP),Y
   LDY   #$33
   LDA   (FP),Y
   LDY   #$37
   STA   (FP),Y
   LDY   #$34
   LDA   (FP),Y
   LDY   #$38
   STA   (FP),Y
   LDY   #$35
   LDA   (FP),Y
   LDY   #$39
   STA   (FP),Y
   LDY   #$36
   LDA   (FP),Y
   LDY   #$3A
   STA   (FP),Y
   LDY   #$31
   LDA   (FP),Y
   LDY   #$3B
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$3B
   LDA   (FP),Y
   LDY   #$37
   EOR   (FP),Y
   LDY   #$3F
   STA   (FP),Y
   LDY   #$3C
   LDA   (FP),Y
   LDY   #$38
   EOR   (FP),Y
   LDY   #$40
   STA   (FP),Y
   LDY   #$3D
   LDA   (FP),Y
   LDY   #$39
   EOR   (FP),Y
   LDY   #$41
   STA   (FP),Y
   LDY   #$3E
   LDA   (FP),Y
   LDY   #$3A
   EOR   (FP),Y
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
   DEY
   LDA   (FP),Y
   LDY   #$31
   STA   (FP),Y
   LDY   #$44
   LDA   (FP),Y
   LDY   #$32
   STA   (FP),Y
   LDY   #$43
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@22
   LDX   #$00
   LDA   #$03

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$52
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$51
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
   LDY   #$31
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@23
   LDX   #$00
   LDA   #$04

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$52
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$51
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
   LDY   #$2D
   LDA   (FP),Y
   LDY   #$45
   STA   (FP),Y
   LDY   #$2E
   LDA   (FP),Y
   LDY   #$46
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@115
   LDA   #$00
   JMP   .sx_done@116
.sx_neg@115:
   LDA   #$FF
.sx_done@116:
   LDY   #$47
   STA   (FP),Y
   INY
   STA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@117
   CMP   #$01
   BNE   .cmp_true@117
   LDY   #$46
   LDA   (FP),Y
   CMP   #$E2
   BNE   .cmp_true@117
   DEY
   LDA   (FP),Y
   CMP   #$40
   BNE   .cmp_true@117
   LDA   #$00
   JMP   .cmp_end@118
.cmp_true@117:
   LDA   #$01
.cmp_end@118:
   LDY   #$49
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@24
   LDX   #$00
   LDA   #$05

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$52
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$51
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
.if_end@24:
   SEC
   LDA   #$00
   SBC   #$10
   LDY   #$4B
   STA   (FP),Y
   LDA   #$00
   SBC   #$0F
   INY
   STA   (FP),Y
   LDA   #$00
   SBC   #$0F
   INY
   STA   (FP),Y
   LDA   #$00
   SBC   #$0F
   INY
   STA   (FP),Y
   LDY   #$3A
   LDA   (FP),Y
   LDY   #$4E
   CMP   (FP),Y
   BNE   .cmp_true@120
   LDY   #$39
   LDA   (FP),Y
   LDY   #$4D
   CMP   (FP),Y
   BNE   .cmp_true@120
   LDY   #$38
   LDA   (FP),Y
   LDY   #$4C
   CMP   (FP),Y
   BNE   .cmp_true@120
   LDY   #$37
   LDA   (FP),Y
   LDY   #$4B
   CMP   (FP),Y
   BNE   .cmp_true@120
   LDA   #$00
   JMP   .cmp_end@121
.cmp_true@120:
   LDA   #$01
.cmp_end@121:
   LDY   #$4F
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@25
   LDX   #$00
   LDA   #$06

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$52
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
.if_end@25:
   LDX   #$00
   TXA

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$52
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$51
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

target_unsigned_bitshift:
   SUBROUTINE

   ; prologue: 0 arg bytes, 32 local bytes
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

   SEC
   LDA   #$00
   SBC   #$02
   LDY   #$01
   STA   (FP),Y
   LDA   #$00
   SBC   #$00
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
   DEY
   LDA   (FP),Y
   LDY   #$05
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$06
   STA   (FP),Y
   LDX   #$03
.shift_loop@123:
   LDY   #$06
   LDA   (FP),Y
   ASL
   LDA   (FP),Y
   ROR
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ROR
   STA   (FP),Y
   DEX
   BNE   .shift_loop@123
   LDY   #$03
   STA   (FP),Y
   LDY   #$06
   LDA   (FP),Y
   LDY   #$04
   STA   (FP),Y
   SEC
   LDA   #$00
   SBC   #$01
   LDY   #$07
   STA   (FP),Y
   LDA   #$00
   SBC   #$00
   INY
   STA   (FP),Y
   LDY   #$04
   LDA   (FP),Y
   LDY   #$08
   CMP   (FP),Y
   BNE   .cmp_true@125
   LDY   #$03
   LDA   (FP),Y
   LDY   #$07
   CMP   (FP),Y
   BNE   .cmp_true@125
   LDA   #$00
   JMP   .cmp_end@126
.cmp_true@125:
   LDA   #$01
.cmp_end@126:
   LDY   #$09
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@26
   LDX   #$00
   LDA   #$01

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$22
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
.if_end@26:
   LDA   #$FF
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
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   STA   HARGS
   LDY   #$10
   LDA   (FP),Y
   STA   HARGS+1
   INY
   LDA   (FP),Y
   STA   HARGS+2
   INY
   LDA   (FP),Y
   STA   HARGS+3
   LDA   #$2C
   STA   HARGS+4
   JSR   asl32
   LDA   HARGS+5
   LDY   #$13
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
   LDY   #$13
   LDA   (FP),Y
   LDY   #$0F
   STA   (FP),Y
   LDY   #$14
   LDA   (FP),Y
   LDY   #$10
   STA   (FP),Y
   LDY   #$15
   LDA   (FP),Y
   LDY   #$11
   STA   (FP),Y
   LDY   #$16
   LDA   (FP),Y
   LDY   #$12
   STA   (FP),Y
   LDY   #$0F
   LDA   (FP),Y
   LDY   #$17
   STA   (FP),Y
   LDY   #$10
   LDA   (FP),Y
   LDY   #$18
   STA   (FP),Y
   LDY   #$11
   LDA   (FP),Y
   LDY   #$19
   STA   (FP),Y
   LDY   #$12
   LDA   (FP),Y
   LDY   #$1A
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
   CMP   #$FF
   BNE   .cmp_true@128
   CMP   #$F0
   BNE   .cmp_true@128
   CMP   #$00
   BNE   .cmp_true@128
   LDY   #$1A
   LDA   (FP),Y
   BNE   .cmp_true@128
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@128
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@128
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@128
   LDA   #$00
   JMP   .cmp_end@129
.cmp_true@128:
   LDA   #$01
.cmp_end@129:
   LDY   #$1F
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@27
   LDX   #$00
   LDA   #$02

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$22
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
.if_end@27:
   LDX   #$00
   TXA

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$22
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

main:
   SUBROUTINE

   ; prologue: 0 arg bytes, 10 local bytes
   SEC
   LDA   SSP
   SBC   #$0C
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$0B
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   JSR   target_chars
   LDY   #$01
   STA   (FP),Y
   TXA
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@28
   LDX   #$00
   LDA   #$01

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$0C
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$0B
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
.if_end@28:
   JSR   target_long_bitwise
   LDY   #$03
   STA   (FP),Y
   TXA
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@29
   LDX   #$00
   LDA   #$02

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$0C
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$0B
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
.if_end@29:
   JSR   target_long_bitshift
   LDY   #$05
   STA   (FP),Y
   TXA
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@30
   LDX   #$00
   LDA   #$03

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$0C
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$0B
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
.if_end@30:
   JSR   target_unsigned_bitwise
   LDY   #$07
   STA   (FP),Y
   TXA
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@31
   LDX   #$00
   LDA   #$04

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$0C
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$0B
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
.if_end@31:
   JSR   target_unsigned_bitshift
   LDY   #$09
   STA   (FP),Y
   TXA
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@32
   LDX   #$00
   LDA   #$05

   ; epilogue
   PHA
   TXA
   PHA
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
   PLA
   TAX
   PLA
   RTS
.if_end@32:
   LDX   #$00
   TXA

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$0C
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$0B
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
