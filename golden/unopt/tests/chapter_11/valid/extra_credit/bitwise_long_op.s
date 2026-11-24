SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24

main:
   SUBROUTINE

   ; prologue: 0 arg bytes, 222 local bytes
   SEC
   LDA   SSP
   SBC   #$E0
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$DF
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
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   SEC
   INY
   STA   (FP),Y
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
   LDY   #$0D
   LDA   (FP),Y
   LDY   #$15
   STA   (FP),Y
   LDY   #$0E
   LDA   (FP),Y
   LDY   #$16
   STA   (FP),Y
   LDY   #$0F
   LDA   (FP),Y
   LDY   #$17
   STA   (FP),Y
   LDY   #$10
   LDA   (FP),Y
   LDY   #$18
   STA   (FP),Y
   LDY   #$01
   LDA   (FP),Y
   LDY   #$11
   AND   (FP),Y
   LDY   #$19
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   LDY   #$12
   AND   (FP),Y
   LDY   #$1A
   STA   (FP),Y
   LDY   #$03
   LDA   (FP),Y
   LDY   #$13
   AND   (FP),Y
   LDY   #$1B
   STA   (FP),Y
   LDY   #$04
   LDA   (FP),Y
   LDY   #$14
   AND   (FP),Y
   LDY   #$1C
   STA   (FP),Y
   LDY   #$05
   LDA   (FP),Y
   LDY   #$15
   AND   (FP),Y
   LDY   #$1D
   STA   (FP),Y
   LDY   #$06
   LDA   (FP),Y
   LDY   #$16
   AND   (FP),Y
   LDY   #$1E
   STA   (FP),Y
   LDY   #$07
   LDA   (FP),Y
   LDY   #$17
   AND   (FP),Y
   LDY   #$1F
   STA   (FP),Y
   LDY   #$08
   LDA   (FP),Y
   LDY   #$18
   AND   (FP),Y
   LDY   #$20
   STA   (FP),Y
   LDY   #$1B
   LDA   (FP),Y
   CMP   #$FF
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
   LDY   #$21
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
   ADC   #$E0
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$DF
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
   LDY   #$01
   LDA   (FP),Y
   LDY   #$11
   ORA   (FP),Y
   LDY   #$23
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   LDY   #$12
   ORA   (FP),Y
   LDY   #$24
   STA   (FP),Y
   LDY   #$03
   LDA   (FP),Y
   LDY   #$13
   ORA   (FP),Y
   LDY   #$25
   STA   (FP),Y
   LDY   #$04
   LDA   (FP),Y
   LDY   #$14
   ORA   (FP),Y
   LDY   #$26
   STA   (FP),Y
   LDY   #$05
   LDA   (FP),Y
   LDY   #$15
   ORA   (FP),Y
   LDY   #$27
   STA   (FP),Y
   LDY   #$06
   LDA   (FP),Y
   LDY   #$16
   ORA   (FP),Y
   LDY   #$28
   STA   (FP),Y
   LDY   #$07
   LDA   (FP),Y
   LDY   #$17
   ORA   (FP),Y
   LDY   #$29
   STA   (FP),Y
   LDY   #$08
   LDA   (FP),Y
   LDY   #$18
   ORA   (FP),Y
   LDY   #$2A
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
   LDY   #$2A
   LDA   (FP),Y
   LDY   #$32
   CMP   (FP),Y
   BNE   .cmp_true@3
   LDY   #$29
   LDA   (FP),Y
   LDY   #$31
   CMP   (FP),Y
   BNE   .cmp_true@3
   LDY   #$28
   LDA   (FP),Y
   LDY   #$30
   CMP   (FP),Y
   BNE   .cmp_true@3
   LDY   #$27
   LDA   (FP),Y
   LDY   #$2F
   CMP   (FP),Y
   BNE   .cmp_true@3
   LDY   #$26
   LDA   (FP),Y
   LDY   #$2E
   CMP   (FP),Y
   BNE   .cmp_true@3
   LDY   #$25
   LDA   (FP),Y
   LDY   #$2D
   CMP   (FP),Y
   BNE   .cmp_true@3
   LDY   #$24
   LDA   (FP),Y
   LDY   #$2C
   CMP   (FP),Y
   BNE   .cmp_true@3
   LDY   #$23
   LDA   (FP),Y
   LDY   #$2B
   CMP   (FP),Y
   BNE   .cmp_true@3
   LDA   #$00
   JMP   .cmp_end@4
.cmp_true@3:
   LDA   #$01
.cmp_end@4:
   LDY   #$33
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
   ADC   #$E0
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$DF
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
   LDY   #$01
   LDA   (FP),Y
   LDY   #$11
   EOR   (FP),Y
   LDY   #$35
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   LDY   #$12
   EOR   (FP),Y
   LDY   #$36
   STA   (FP),Y
   LDY   #$03
   LDA   (FP),Y
   LDY   #$13
   EOR   (FP),Y
   LDY   #$37
   STA   (FP),Y
   LDY   #$04
   LDA   (FP),Y
   LDY   #$14
   EOR   (FP),Y
   LDY   #$38
   STA   (FP),Y
   LDY   #$05
   LDA   (FP),Y
   LDY   #$15
   EOR   (FP),Y
   LDY   #$39
   STA   (FP),Y
   LDY   #$06
   LDA   (FP),Y
   LDY   #$16
   EOR   (FP),Y
   LDY   #$3A
   STA   (FP),Y
   LDY   #$07
   LDA   (FP),Y
   LDY   #$17
   EOR   (FP),Y
   LDY   #$3B
   STA   (FP),Y
   LDY   #$08
   LDA   (FP),Y
   LDY   #$18
   EOR   (FP),Y
   LDY   #$3C
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
   LDA   #$00
   SBC   #$00
   INY
   STA   (FP),Y
   LDY   #$3C
   LDA   (FP),Y
   LDY   #$44
   CMP   (FP),Y
   BNE   .cmp_true@6
   LDY   #$3B
   LDA   (FP),Y
   LDY   #$43
   CMP   (FP),Y
   BNE   .cmp_true@6
   LDY   #$3A
   LDA   (FP),Y
   LDY   #$42
   CMP   (FP),Y
   BNE   .cmp_true@6
   LDY   #$39
   LDA   (FP),Y
   LDY   #$41
   CMP   (FP),Y
   BNE   .cmp_true@6
   LDY   #$38
   LDA   (FP),Y
   LDY   #$40
   CMP   (FP),Y
   BNE   .cmp_true@6
   LDY   #$37
   LDA   (FP),Y
   LDY   #$3F
   CMP   (FP),Y
   BNE   .cmp_true@6
   LDY   #$36
   LDA   (FP),Y
   LDY   #$3E
   CMP   (FP),Y
   BNE   .cmp_true@6
   LDY   #$35
   LDA   (FP),Y
   LDY   #$3D
   CMP   (FP),Y
   BNE   .cmp_true@6
   LDA   #$00
   JMP   .cmp_end@7
.cmp_true@6:
   LDA   #$01
.cmp_end@7:
   LDY   #$45
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
   ADC   #$E0
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$DF
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
   LDA   #$00
   SBC   #$01
   LDY   #$47
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
   LDA   #$00
   SBC   #$00
   INY
   STA   (FP),Y
   LDY   #$47
   LDA   (FP),Y
   LDA   #$00
   LDY   #$4F
   STA   (FP),Y
   LDY   #$48
   LDA   (FP),Y
   LDA   #$00
   LDY   #$50
   STA   (FP),Y
   LDY   #$49
   LDA   (FP),Y
   AND   #$01
   LDY   #$51
   STA   (FP),Y
   LDY   #$4A
   LDA   (FP),Y
   LDA   #$00
   LDY   #$52
   STA   (FP),Y
   LDY   #$4B
   LDA   (FP),Y
   LDA   #$00
   LDY   #$53
   STA   (FP),Y
   LDY   #$4C
   LDA   (FP),Y
   LDA   #$00
   LDY   #$54
   STA   (FP),Y
   LDY   #$4D
   LDA   (FP),Y
   LDA   #$00
   LDY   #$55
   STA   (FP),Y
   LDY   #$4E
   LDA   (FP),Y
   LDA   #$00
   LDY   #$56
   STA   (FP),Y
   LDY   #$51
   LDA   (FP),Y
   CMP   #$01
   BNE   .cmp_true@9
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@9
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@9
   LDA   #$00
   JMP   .cmp_end@10
.cmp_true@9:
   LDA   #$01
.cmp_end@10:
   LDY   #$57
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
   ADC   #$E0
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$DF
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
   LDA   #$00
   LDY   #$59
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$01
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
   LDY   #$5B
   LDA   (FP),Y
   CMP   #$01
   BNE   .cmp_true@12
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@12
   LDA   #$00
   JMP   .cmp_end@13
.cmp_true@12:
   LDA   #$01
.cmp_end@13:
   LDY   #$61
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
   ADC   #$E0
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$DF
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
   LDY   #$63
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$01
   EOR   #$04
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
   LDY   #$65
   LDA   (FP),Y
   CMP   #$05
   BNE   .cmp_true@15
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@15
   LDA   #$00
   JMP   .cmp_end@16
.cmp_true@15:
   LDA   #$01
.cmp_end@16:
   LDY   #$6B
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
   ADC   #$E0
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$DF
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
   LDA   #$FF
   LDY   #$6D
   STA   (FP),Y
   INY
   STA   (FP),Y
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
   SEC
   SBC   #$40
   INY
   STA   (FP),Y
   LDA   #$00
   SBC   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$77
   STA   (FP),Y
   DEY
   LDA   (FP),Y
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
   DEY
   LDA   (FP),Y
   LDY   #$7B
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$7C
   STA   (FP),Y
   LDY   #$77
   LDA   (FP),Y
   LDY   #$7D
   STA   (FP),Y
   LDY   #$78
   LDA   (FP),Y
   LDY   #$7E
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@18
   LDA   #$00
   JMP   .sx_done@19
.sx_neg@18:
   LDA   #$FF
.sx_done@19:
   LDY   #$7F
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
   LDY   #$7D
   LDA   (FP),Y
   LDY   #$6D
   AND   (FP),Y
   LDY   #$85
   STA   (FP),Y
   LDY   #$7E
   LDA   (FP),Y
   LDY   #$6E
   AND   (FP),Y
   LDY   #$86
   STA   (FP),Y
   LDY   #$7F
   LDA   (FP),Y
   LDY   #$6F
   AND   (FP),Y
   LDY   #$87
   STA   (FP),Y
   LDY   #$80
   LDA   (FP),Y
   LDY   #$70
   AND   (FP),Y
   LDY   #$88
   STA   (FP),Y
   LDY   #$81
   LDA   (FP),Y
   LDY   #$71
   AND   (FP),Y
   LDY   #$89
   STA   (FP),Y
   LDY   #$82
   LDA   (FP),Y
   LDY   #$72
   AND   (FP),Y
   LDY   #$8A
   STA   (FP),Y
   LDY   #$83
   LDA   (FP),Y
   LDY   #$73
   AND   (FP),Y
   LDY   #$8B
   STA   (FP),Y
   LDY   #$84
   LDA   (FP),Y
   LDY   #$74
   AND   (FP),Y
   LDY   #$8C
   STA   (FP),Y
   LDY   #$88
   LDA   (FP),Y
   CMP   #$3F
   BNE   .cmp_true@20
   DEY
   LDA   (FP),Y
   CMP   #$FF
   BNE   .cmp_true@20
   DEY
   LDA   (FP),Y
   CMP   #$FF
   BNE   .cmp_true@20
   DEY
   LDA   (FP),Y
   CMP   #$C0
   BNE   .cmp_true@20
   LDA   #$00
   JMP   .cmp_end@21
.cmp_true@20:
   LDA   #$01
.cmp_end@21:
   LDY   #$8D
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
   ADC   #$E0
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$DF
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
   LDY   #$77
   LDA   (FP),Y
   LDY   #$8F
   STA   (FP),Y
   LDY   #$78
   LDA   (FP),Y
   LDY   #$90
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@23
   LDA   #$00
   JMP   .sx_done@24
.sx_neg@23:
   LDA   #$FF
.sx_done@24:
   LDY   #$91
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
   LDY   #$6D
   LDA   (FP),Y
   LDY   #$8F
   ORA   (FP),Y
   LDY   #$97
   STA   (FP),Y
   LDY   #$6E
   LDA   (FP),Y
   LDY   #$90
   ORA   (FP),Y
   LDY   #$98
   STA   (FP),Y
   LDY   #$6F
   LDA   (FP),Y
   LDY   #$91
   ORA   (FP),Y
   LDY   #$99
   STA   (FP),Y
   LDY   #$70
   LDA   (FP),Y
   LDY   #$92
   ORA   (FP),Y
   LDY   #$9A
   STA   (FP),Y
   LDY   #$71
   LDA   (FP),Y
   LDY   #$93
   ORA   (FP),Y
   LDY   #$9B
   STA   (FP),Y
   LDY   #$72
   LDA   (FP),Y
   LDY   #$94
   ORA   (FP),Y
   LDY   #$9C
   STA   (FP),Y
   LDY   #$73
   LDA   (FP),Y
   LDY   #$95
   ORA   (FP),Y
   LDY   #$9D
   STA   (FP),Y
   LDY   #$74
   LDA   (FP),Y
   LDY   #$96
   ORA   (FP),Y
   LDY   #$9E
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
   DEY
   LDA   (FP),Y
   LDY   #$A1
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$A2
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@25
   LDA   #$00
   JMP   .sx_done@26
.sx_neg@25:
   LDA   #$FF
.sx_done@26:
   LDY   #$A3
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
   LDY   #$9E
   LDA   (FP),Y
   LDY   #$A8
   CMP   (FP),Y
   BNE   .cmp_true@27
   LDY   #$9D
   LDA   (FP),Y
   LDY   #$A7
   CMP   (FP),Y
   BNE   .cmp_true@27
   LDY   #$9C
   LDA   (FP),Y
   LDY   #$A6
   CMP   (FP),Y
   BNE   .cmp_true@27
   LDY   #$9B
   LDA   (FP),Y
   LDY   #$A5
   CMP   (FP),Y
   BNE   .cmp_true@27
   LDY   #$9A
   LDA   (FP),Y
   LDY   #$A4
   CMP   (FP),Y
   BNE   .cmp_true@27
   LDY   #$99
   LDA   (FP),Y
   LDY   #$A3
   CMP   (FP),Y
   BNE   .cmp_true@27
   LDY   #$98
   LDA   (FP),Y
   LDY   #$A2
   CMP   (FP),Y
   BNE   .cmp_true@27
   LDY   #$97
   LDA   (FP),Y
   LDY   #$A1
   CMP   (FP),Y
   BNE   .cmp_true@27
   LDA   #$00
   JMP   .cmp_end@28
.cmp_true@27:
   LDA   #$01
.cmp_end@28:
   LDY   #$A9
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
   ADC   #$E0
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$DF
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
   LDY   #$77
   LDA   (FP),Y
   LDY   #$AB
   STA   (FP),Y
   LDY   #$78
   LDA   (FP),Y
   LDY   #$AC
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@30
   LDA   #$00
   JMP   .sx_done@31
.sx_neg@30:
   LDA   #$FF
.sx_done@31:
   LDY   #$AD
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
   LDY   #$6D
   LDA   (FP),Y
   LDY   #$AB
   EOR   (FP),Y
   LDY   #$B3
   STA   (FP),Y
   LDY   #$6E
   LDA   (FP),Y
   LDY   #$AC
   EOR   (FP),Y
   LDY   #$B4
   STA   (FP),Y
   LDY   #$6F
   LDA   (FP),Y
   LDY   #$AD
   EOR   (FP),Y
   LDY   #$B5
   STA   (FP),Y
   LDY   #$70
   LDA   (FP),Y
   LDY   #$AE
   EOR   (FP),Y
   LDY   #$B6
   STA   (FP),Y
   LDY   #$71
   LDA   (FP),Y
   LDY   #$AF
   EOR   (FP),Y
   LDY   #$B7
   STA   (FP),Y
   LDY   #$72
   LDA   (FP),Y
   LDY   #$B0
   EOR   (FP),Y
   LDY   #$B8
   STA   (FP),Y
   LDY   #$73
   LDA   (FP),Y
   LDY   #$B1
   EOR   (FP),Y
   LDY   #$B9
   STA   (FP),Y
   LDY   #$74
   LDA   (FP),Y
   LDY   #$B2
   EOR   (FP),Y
   LDY   #$BA
   STA   (FP),Y
   SEC
   LDA   #$00
   SBC   #$C1
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
   SBC   #$3F
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
   LDY   #$BA
   LDA   (FP),Y
   LDY   #$C2
   CMP   (FP),Y
   BNE   .cmp_true@32
   LDY   #$B9
   LDA   (FP),Y
   LDY   #$C1
   CMP   (FP),Y
   BNE   .cmp_true@32
   LDY   #$B8
   LDA   (FP),Y
   LDY   #$C0
   CMP   (FP),Y
   BNE   .cmp_true@32
   LDY   #$B7
   LDA   (FP),Y
   LDY   #$BF
   CMP   (FP),Y
   BNE   .cmp_true@32
   LDY   #$B6
   LDA   (FP),Y
   LDY   #$BE
   CMP   (FP),Y
   BNE   .cmp_true@32
   LDY   #$B5
   LDA   (FP),Y
   LDY   #$BD
   CMP   (FP),Y
   BNE   .cmp_true@32
   LDY   #$B4
   LDA   (FP),Y
   LDY   #$BC
   CMP   (FP),Y
   BNE   .cmp_true@32
   LDY   #$B3
   LDA   (FP),Y
   LDY   #$BB
   CMP   (FP),Y
   BNE   .cmp_true@32
   LDA   #$00
   JMP   .cmp_end@33
.cmp_true@32:
   LDA   #$01
.cmp_end@33:
   LDY   #$C3
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
   ADC   #$E0
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$DF
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
   LDY   #$7B
   LDA   (FP),Y
   LDY   #$C5
   STA   (FP),Y
   LDY   #$7C
   LDA   (FP),Y
   LDY   #$C6
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@35
   LDA   #$00
   JMP   .sx_done@36
.sx_neg@35:
   LDA   #$FF
.sx_done@36:
   LDY   #$C7
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
   LDY   #$C5
   LDA   (FP),Y
   EOR   #$FF
   LDY   #$CD
   STA   (FP),Y
   LDY   #$C6
   LDA   (FP),Y
   EOR   #$FF
   LDY   #$CE
   STA   (FP),Y
   LDY   #$C7
   LDA   (FP),Y
   EOR   #$FF
   LDY   #$CF
   STA   (FP),Y
   LDY   #$C8
   LDA   (FP),Y
   EOR   #$3F
   LDY   #$D0
   STA   (FP),Y
   LDY   #$C9
   LDA   (FP),Y
   LDY   #$D1
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$FF
   EOR   #$FF
   INY
   STA   (FP),Y
   LDA   #$FF
   EOR   #$FF
   INY
   STA   (FP),Y
   LDA   #$FF
   EOR   #$FF
   INY
   STA   (FP),Y
   LDA   #$3F
   EOR   #$FF
   INY
   STA   (FP),Y
   LDA   #$00
   EOR   #$FF
   INY
   STA   (FP),Y
   LDA   #$00
   EOR   #$FF
   INY
   STA   (FP),Y
   LDA   #$00
   EOR   #$FF
   INY
   STA   (FP),Y
   LDA   #$00
   EOR   #$FF
   INY
   STA   (FP),Y
   LDY   #$D4
   LDA   (FP),Y
   LDY   #$DC
   CMP   (FP),Y
   BNE   .cmp_true@37
   DEY
   CMP   (FP),Y
   BNE   .cmp_true@37
   DEY
   CMP   (FP),Y
   BNE   .cmp_true@37
   DEY
   CMP   (FP),Y
   BNE   .cmp_true@37
   LDY   #$D0
   LDA   (FP),Y
   LDY   #$D8
   CMP   (FP),Y
   BNE   .cmp_true@37
   LDY   #$CF
   LDA   (FP),Y
   LDY   #$D7
   CMP   (FP),Y
   BNE   .cmp_true@37
   LDY   #$CE
   LDA   (FP),Y
   LDY   #$D6
   CMP   (FP),Y
   BNE   .cmp_true@37
   LDY   #$CD
   LDA   (FP),Y
   LDY   #$D5
   CMP   (FP),Y
   BNE   .cmp_true@37
   LDA   #$00
   JMP   .cmp_end@38
.cmp_true@37:
   LDA   #$01
.cmp_end@38:
   LDY   #$DD
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
   ADC   #$E0
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
.if_end@9:
   LDX   #$00
   TXA

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$E0
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$DF
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
