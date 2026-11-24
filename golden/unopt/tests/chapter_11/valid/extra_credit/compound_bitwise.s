SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24

main:
   SUBROUTINE

   ; prologue: 0 arg bytes, 136 local bytes
   SEC
   LDA   SSP
   SBC   #$8A
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$89
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
   LDY   #$19
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   LDY   #$1A
   LDA   (FP),Y
   LDY   #$02
   STA   (FP),Y
   LDY   #$1B
   LDA   (FP),Y
   LDY   #$03
   STA   (FP),Y
   LDY   #$1C
   LDA   (FP),Y
   LDY   #$04
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
   CMP   #$00
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
   ADC   #$8A
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$89
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
   LDY   #$11
   LDA   (FP),Y
   ORA   #$64
   LDY   #$23
   STA   (FP),Y
   LDY   #$12
   LDA   (FP),Y
   LDY   #$24
   STA   (FP),Y
   LDY   #$13
   LDA   (FP),Y
   LDY   #$25
   STA   (FP),Y
   LDY   #$14
   LDA   (FP),Y
   LDY   #$26
   STA   (FP),Y
   LDY   #$15
   LDA   (FP),Y
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
   LDY   #$23
   LDA   (FP),Y
   LDY   #$11
   STA   (FP),Y
   LDY   #$24
   LDA   (FP),Y
   LDY   #$12
   STA   (FP),Y
   LDY   #$25
   LDA   (FP),Y
   LDY   #$13
   STA   (FP),Y
   LDY   #$26
   LDA   (FP),Y
   LDY   #$14
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
   SEC
   LDA   #$00
   SBC   #$9C
   LDY   #$2B
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
   LDY   #$18
   LDA   (FP),Y
   LDY   #$32
   CMP   (FP),Y
   BNE   .cmp_true@3
   LDY   #$17
   LDA   (FP),Y
   LDY   #$31
   CMP   (FP),Y
   BNE   .cmp_true@3
   LDY   #$16
   LDA   (FP),Y
   LDY   #$30
   CMP   (FP),Y
   BNE   .cmp_true@3
   LDY   #$15
   LDA   (FP),Y
   LDY   #$2F
   CMP   (FP),Y
   BNE   .cmp_true@3
   LDY   #$14
   LDA   (FP),Y
   LDY   #$2E
   CMP   (FP),Y
   BNE   .cmp_true@3
   LDY   #$13
   LDA   (FP),Y
   LDY   #$2D
   CMP   (FP),Y
   BNE   .cmp_true@3
   LDY   #$12
   LDA   (FP),Y
   LDY   #$2C
   CMP   (FP),Y
   BNE   .cmp_true@3
   LDY   #$11
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
   ADC   #$8A
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$89
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
   LDA   #$00
   SBC   #$FF
   LDY   #$35
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
   LDY   #$01
   LDA   (FP),Y
   LDY   #$35
   EOR   (FP),Y
   LDY   #$3D
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   LDY   #$36
   EOR   (FP),Y
   LDY   #$3E
   STA   (FP),Y
   LDY   #$03
   LDA   (FP),Y
   LDY   #$37
   EOR   (FP),Y
   LDY   #$3F
   STA   (FP),Y
   LDY   #$04
   LDA   (FP),Y
   LDY   #$38
   EOR   (FP),Y
   LDY   #$40
   STA   (FP),Y
   LDY   #$05
   LDA   (FP),Y
   LDY   #$39
   EOR   (FP),Y
   LDY   #$41
   STA   (FP),Y
   LDY   #$06
   LDA   (FP),Y
   LDY   #$3A
   EOR   (FP),Y
   LDY   #$42
   STA   (FP),Y
   LDY   #$07
   LDA   (FP),Y
   LDY   #$3B
   EOR   (FP),Y
   LDY   #$43
   STA   (FP),Y
   LDY   #$08
   LDA   (FP),Y
   LDY   #$3C
   EOR   (FP),Y
   LDY   #$44
   STA   (FP),Y
   LDY   #$3D
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   LDY   #$3E
   LDA   (FP),Y
   LDY   #$02
   STA   (FP),Y
   LDY   #$3F
   LDA   (FP),Y
   LDY   #$03
   STA   (FP),Y
   LDY   #$40
   LDA   (FP),Y
   LDY   #$04
   STA   (FP),Y
   LDY   #$41
   LDA   (FP),Y
   LDY   #$05
   STA   (FP),Y
   LDY   #$42
   LDA   (FP),Y
   LDY   #$06
   STA   (FP),Y
   LDY   #$43
   LDA   (FP),Y
   LDY   #$07
   STA   (FP),Y
   LDY   #$44
   LDA   (FP),Y
   LDY   #$08
   STA   (FP),Y
   SEC
   LDA   #$00
   SBC   #$FF
   LDY   #$45
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
   LDY   #$08
   LDA   (FP),Y
   LDY   #$4C
   CMP   (FP),Y
   BNE   .cmp_true@6
   LDY   #$07
   LDA   (FP),Y
   LDY   #$4B
   CMP   (FP),Y
   BNE   .cmp_true@6
   LDY   #$06
   LDA   (FP),Y
   LDY   #$4A
   CMP   (FP),Y
   BNE   .cmp_true@6
   LDY   #$05
   LDA   (FP),Y
   LDY   #$49
   CMP   (FP),Y
   BNE   .cmp_true@6
   LDY   #$04
   LDA   (FP),Y
   LDY   #$48
   CMP   (FP),Y
   BNE   .cmp_true@6
   LDY   #$03
   LDA   (FP),Y
   LDY   #$47
   CMP   (FP),Y
   BNE   .cmp_true@6
   LDY   #$02
   LDA   (FP),Y
   LDY   #$46
   CMP   (FP),Y
   BNE   .cmp_true@6
   LDY   #$01
   LDA   (FP),Y
   LDY   #$45
   CMP   (FP),Y
   BNE   .cmp_true@6
   LDA   #$00
   JMP   .cmp_end@7
.cmp_true@6:
   LDA   #$01
.cmp_end@7:
   LDY   #$4D
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
   ADC   #$8A
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$89
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
   LDA   #$FF
   LDY   #$01
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
   LDY   #$4F
   STA   (FP),Y
   LDA   #$00
   SBC   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$51
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$52
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$53
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$54
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@9
   LDA   #$00
   JMP   .sx_done@10
.sx_neg@9:
   LDA   #$FF
.sx_done@10:
   LDY   #$55
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
   LDY   #$53
   AND   (FP),Y
   LDY   #$5B
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   LDY   #$54
   AND   (FP),Y
   LDY   #$5C
   STA   (FP),Y
   LDY   #$03
   LDA   (FP),Y
   LDY   #$55
   AND   (FP),Y
   LDY   #$5D
   STA   (FP),Y
   LDY   #$04
   LDA   (FP),Y
   LDY   #$56
   AND   (FP),Y
   LDY   #$5E
   STA   (FP),Y
   LDY   #$05
   LDA   (FP),Y
   LDY   #$57
   AND   (FP),Y
   LDY   #$5F
   STA   (FP),Y
   LDY   #$06
   LDA   (FP),Y
   LDY   #$58
   AND   (FP),Y
   LDY   #$60
   STA   (FP),Y
   LDY   #$07
   LDA   (FP),Y
   LDY   #$59
   AND   (FP),Y
   LDY   #$61
   STA   (FP),Y
   LDY   #$08
   LDA   (FP),Y
   LDY   #$5A
   AND   (FP),Y
   LDY   #$62
   STA   (FP),Y
   LDY   #$5B
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   LDY   #$5C
   LDA   (FP),Y
   LDY   #$02
   STA   (FP),Y
   LDY   #$5D
   LDA   (FP),Y
   LDY   #$03
   STA   (FP),Y
   LDY   #$5E
   LDA   (FP),Y
   LDY   #$04
   STA   (FP),Y
   LDY   #$5F
   LDA   (FP),Y
   LDY   #$05
   STA   (FP),Y
   LDY   #$60
   LDA   (FP),Y
   LDY   #$06
   STA   (FP),Y
   LDY   #$61
   LDA   (FP),Y
   LDY   #$07
   STA   (FP),Y
   LDY   #$62
   LDA   (FP),Y
   LDY   #$08
   STA   (FP),Y
   CMP   #$00
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
   CMP   #$3F
   BNE   .cmp_true@11
   DEY
   LDA   (FP),Y
   CMP   #$FF
   BNE   .cmp_true@11
   DEY
   LDA   (FP),Y
   CMP   #$FF
   BNE   .cmp_true@11
   DEY
   LDA   (FP),Y
   CMP   #$C0
   BNE   .cmp_true@11
   LDA   #$00
   JMP   .cmp_end@12
.cmp_true@11:
   LDA   #$01
.cmp_end@12:
   LDY   #$63
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
   ADC   #$8A
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$89
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
   LDA   #$00
   SBC   #$80
   LDY   #$65
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
   LDY   #$65
   LDA   (FP),Y
   LDY   #$6D
   STA   (FP),Y
   LDY   #$66
   LDA   (FP),Y
   LDY   #$6E
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$51
   STA   (FP),Y
   LDY   #$6E
   LDA   (FP),Y
   LDY   #$52
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$6F
   STA   (FP),Y
   LDY   #$52
   LDA   (FP),Y
   LDY   #$70
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@14
   LDA   #$00
   JMP   .sx_done@15
.sx_neg@14:
   LDA   #$FF
.sx_done@15:
   LDY   #$71
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
   LDY   #$6F
   LDA   (FP),Y
   LDA   #$FF
   LDY   #$77
   STA   (FP),Y
   LDY   #$70
   LDA   (FP),Y
   LDY   #$78
   STA   (FP),Y
   LDY   #$71
   LDA   (FP),Y
   LDA   #$FF
   LDY   #$79
   STA   (FP),Y
   LDY   #$72
   LDA   (FP),Y
   LDY   #$7A
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$77
   LDA   (FP),Y
   LDY   #$7F
   STA   (FP),Y
   LDY   #$78
   LDA   (FP),Y
   LDY   #$80
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$51
   STA   (FP),Y
   LDY   #$80
   LDA   (FP),Y
   LDY   #$52
   STA   (FP),Y
   SEC
   LDA   #$00
   SBC   #$01
   LDY   #$81
   STA   (FP),Y
   LDA   #$00
   SBC   #$00
   INY
   STA   (FP),Y
   LDY   #$80
   LDA   (FP),Y
   LDY   #$82
   CMP   (FP),Y
   BNE   .cmp_true@16
   LDY   #$7F
   LDA   (FP),Y
   LDY   #$81
   CMP   (FP),Y
   BNE   .cmp_true@16
   LDA   #$00
   JMP   .cmp_end@17
.cmp_true@16:
   LDA   #$01
.cmp_end@17:
   LDY   #$83
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
   ADC   #$8A
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$89
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
   SBC   #$01
   LDY   #$85
   STA   (FP),Y
   LDA   #$00
   SBC   #$00
   INY
   STA   (FP),Y
   LDY   #$52
   LDA   (FP),Y
   LDY   #$86
   CMP   (FP),Y
   BNE   .cmp_true@19
   LDY   #$51
   LDA   (FP),Y
   LDY   #$85
   CMP   (FP),Y
   BNE   .cmp_true@19
   LDA   #$00
   JMP   .cmp_end@20
.cmp_true@19:
   LDA   #$01
.cmp_end@20:
   LDY   #$87
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
   ADC   #$8A
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
   ADC   #$8A
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$89
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
