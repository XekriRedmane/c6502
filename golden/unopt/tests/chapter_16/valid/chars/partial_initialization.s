SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24

main:
   SUBROUTINE

   ; prologue: 0 arg bytes, 176 local bytes
   SEC
   LDA   SSP
   SBC   #$B2
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$B1
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   LDA   static1
   LDY   #$01
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CMP   #$01
   BNE   .cmp_true@0
   LDA   #$00
   JMP   .cmp_end@1
.cmp_true@0:
   LDA   #$01
.cmp_end@1:
   LDY   #$04
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@4
   LDA   static1+1
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CMP   #$02
   BNE   .cmp_true@2
   LDA   #$00
   JMP   .cmp_end@3
.cmp_true@2:
   LDA   #$01
.cmp_end@3:
   LDY   #$09
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
   LDY   #$0B
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@5:
   LDY   #$0B
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@2
   LDA   static1+2
   INY
   STA   (FP),Y
   ORA   #$00
   BNE   .or_true@2
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   JMP   .or_end@3
.or_true@2:
   LDA   #$01
   LDY   #$0E
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@3:
   LDY   #$0E
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@0
   LDA   static1+3
   INY
   STA   (FP),Y
   ORA   #$00
   BNE   .or_true@0
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   JMP   .or_end@1
.or_true@0:
   LDA   #$01
   LDY   #$11
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@1:
   LDY   #$11
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@6
   LDX   #$00
   LDA   #$01

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$B2
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$B1
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
   LDA   static2
   LDY   #$13
   STA   (FP),Y
   INY
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@4
   LDA   #$00
   JMP   .sx_done@5
.sx_neg@4:
   LDA   #$FF
.sx_done@5:
   LDY   #$15
   STA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@6
   DEY
   LDA   (FP),Y
   CMP   #$03
   BNE   .cmp_true@6
   LDA   #$00
   JMP   .cmp_end@7
.cmp_true@6:
   LDA   #$01
.cmp_end@7:
   LDY   #$16
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@11
   LDA   static2+1
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@9
   LDA   #$00
   JMP   .sx_done@10
.sx_neg@9:
   LDA   #$FF
.sx_done@10:
   LDY   #$1A
   STA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@11
   DEY
   LDA   (FP),Y
   CMP   #$04
   BNE   .cmp_true@11
   LDA   #$00
   JMP   .cmp_end@12
.cmp_true@11:
   LDA   #$01
.cmp_end@12:
   LDY   #$1B
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
   LDY   #$1D
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@12:
   LDY   #$1D
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@9
   LDA   static2+2
   INY
   STA   (FP),Y
   ORA   #$00
   BNE   .or_true@9
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   JMP   .or_end@10
.or_true@9:
   LDA   #$01
   LDY   #$20
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@10:
   LDY   #$20
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@7
   LDA   static2+3
   INY
   STA   (FP),Y
   ORA   #$00
   BNE   .or_true@7
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   JMP   .or_end@8
.or_true@7:
   LDA   #$01
   LDY   #$23
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@8:
   LDY   #$23
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@13
   LDX   #$00
   LDA   #$02

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$B2
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$B1
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
   LDA   static3
   LDY   #$25
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CMP   #$05
   BNE   .cmp_true@14
   LDA   #$00
   JMP   .cmp_end@15
.cmp_true@14:
   LDA   #$01
.cmp_end@15:
   LDY   #$28
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@16
   LDA   static3+1
   INY
   STA   (FP),Y
   ORA   #$00
   BNE   .or_true@16
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   JMP   .or_end@17
.or_true@16:
   LDA   #$01
   LDY   #$2B
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@17:
   LDY   #$2B
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@14
   LDA   static3+2
   INY
   STA   (FP),Y
   ORA   #$00
   BNE   .or_true@14
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   JMP   .or_end@15
.or_true@14:
   LDA   #$01
   LDY   #$2E
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@15:
   LDY   #$2E
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
   ADC   #$B2
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$B1
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
   SBC   #$04
   LDY   #$30
   STA   (FP),Y
   LDA   #$00
   SBC   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$32
   STA   (FP),Y
   CLC
   LDA   FP
   ADC   #$33
   LDY   #$38
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
   LDY   #$32
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDA   #$42
   LDY   #$3A
   STA   (FP),Y
   LDY   #$38
   LDA   (FP),Y
   CLC
   ADC   #$01
   LDY   #$3B
   STA   (FP),Y
   LDY   #$39
   LDA   (FP),Y
   ADC   #$00
   LDY   #$3C
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$3A
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$38
   LDA   (FP),Y
   CLC
   ADC   #$02
   LDY   #$3D
   STA   (FP),Y
   LDY   #$39
   LDA   (FP),Y
   ADC   #$00
   LDY   #$3E
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDA   #$04
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$38
   LDA   (FP),Y
   CLC
   ADC   #$03
   LDY   #$3F
   STA   (FP),Y
   LDY   #$39
   LDA   (FP),Y
   ADC   #$00
   LDY   #$40
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDA   #$00
   LDY   #$00
   STA   (DPTR),Y
   INY
   STA   (DPTR),Y
   LDA   static1+2
   LDY   #$41
   STA   (FP),Y
   INY
   STA   (FP),Y
   CLC
   LDA   FP
   ADC   #$43
   LDY   #$46
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
   LDY   #$42
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDA   static1
   LDY   #$48
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   SEC
   DEY
   SBC   (FP),Y
   LDY   #$4B
   STA   (FP),Y
   LDA   #$00
   DEY
   SBC   (FP),Y
   LDY   #$4C
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$4D
   STA   (FP),Y
   LDY   #$46
   LDA   (FP),Y
   CLC
   ADC   #$01
   LDY   #$4E
   STA   (FP),Y
   LDY   #$47
   LDA   (FP),Y
   ADC   #$00
   LDY   #$4F
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$4D
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$46
   LDA   (FP),Y
   CLC
   ADC   #$02
   LDY   #$50
   STA   (FP),Y
   LDY   #$47
   LDA   (FP),Y
   ADC   #$00
   LDY   #$51
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDA   #$00
   LDY   #$00
   STA   (DPTR),Y
   LDA   #$61
   LDY   #$52
   STA   (FP),Y
   CLC
   LDA   FP
   ADC   #$53
   LDY   #$55
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
   LDY   #$52
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$55
   LDA   (FP),Y
   CLC
   ADC   #$01
   LDY   #$57
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$58
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDA   #$00
   LDY   #$00
   STA   (DPTR),Y
   CLC
   LDA   FP
   ADC   #$33
   LDY   #$59
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   LDY   #$5B
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$5C
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$5D
   STA   (FP),Y
   INY
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@16
   LDA   #$00
   JMP   .sx_done@17
.sx_neg@16:
   LDA   #$FF
.sx_done@17:
   LDY   #$5F
   STA   (FP),Y
   SEC
   LDA   #$00
   SBC   #$04
   INY
   STA   (FP),Y
   LDA   #$00
   SBC   #$00
   INY
   STA   (FP),Y
   LDY   #$5F
   LDA   (FP),Y
   LDY   #$61
   CMP   (FP),Y
   BNE   .cmp_true@18
   LDY   #$5E
   LDA   (FP),Y
   LDY   #$60
   CMP   (FP),Y
   BNE   .cmp_true@18
   LDA   #$00
   JMP   .cmp_end@19
.cmp_true@18:
   LDA   #$01
.cmp_end@19:
   LDY   #$62
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@2
   JMP   .or_true@25
.lb_skip@2:
   CLC
   LDA   FP
   ADC   #$33
   LDY   #$64
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$01
   LDY   #$66
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$67
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$68
   STA   (FP),Y
   INY
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@21
   LDA   #$00
   JMP   .sx_done@22
.sx_neg@21:
   LDA   #$FF
.sx_done@22:
   LDY   #$6A
   STA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@23
   DEY
   LDA   (FP),Y
   CMP   #$42
   BNE   .cmp_true@23
   LDA   #$00
   JMP   .cmp_end@24
.cmp_true@23:
   LDA   #$01
.cmp_end@24:
   LDY   #$6B
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@25
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   JMP   .or_end@26
.or_true@25:
   LDA   #$01
   LDY   #$6D
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@26:
   LDY   #$6D
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@1
   JMP   .or_true@23
.lb_skip@1:
   CLC
   LDA   FP
   ADC   #$33
   LDY   #$6F
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$02
   LDY   #$71
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$72
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$73
   STA   (FP),Y
   INY
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@26
   LDA   #$00
   JMP   .sx_done@27
.sx_neg@26:
   LDA   #$FF
.sx_done@27:
   LDY   #$75
   STA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@28
   DEY
   LDA   (FP),Y
   CMP   #$04
   BNE   .cmp_true@28
   LDA   #$00
   JMP   .cmp_end@29
.cmp_true@28:
   LDA   #$01
.cmp_end@29:
   LDY   #$76
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@23
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   JMP   .or_end@24
.or_true@23:
   LDA   #$01
   LDY   #$78
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@24:
   LDY   #$78
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@21
   CLC
   LDA   FP
   ADC   #$33
   INY
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$03
   LDY   #$7C
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$7D
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$7E
   STA   (FP),Y
   ORA   #$00
   BNE   .or_true@21
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   JMP   .or_end@22
.or_true@21:
   LDA   #$01
   LDY   #$7F
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@22:
   LDY   #$7F
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@19
   CLC
   LDA   FP
   ADC   #$33
   INY
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$04
   LDY   #$83
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$84
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$85
   STA   (FP),Y
   ORA   #$00
   BNE   .or_true@19
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   JMP   .or_end@20
.or_true@19:
   LDA   #$01
   LDY   #$86
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@20:
   LDY   #$86
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@27
   LDX   #$00
   LDA   #$04

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$B2
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$B1
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
   CLC
   LDA   FP
   ADC   #$43
   LDY   #$88
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   LDY   #$8A
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$8B
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$8C
   STA   (FP),Y
   ORA   #$00
   BEQ   .lb_skip@0
   JMP   .or_true@30
.lb_skip@0:
   CLC
   LDA   FP
   ADC   #$43
   LDY   #$8D
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$01
   LDY   #$8F
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$90
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$91
   STA   (FP),Y
   INY
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@31
   LDA   #$00
   JMP   .sx_done@32
.sx_neg@31:
   LDA   #$FF
.sx_done@32:
   LDY   #$93
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
   LDY   #$93
   LDA   (FP),Y
   LDY   #$95
   CMP   (FP),Y
   BNE   .cmp_true@33
   LDY   #$92
   LDA   (FP),Y
   LDY   #$94
   CMP   (FP),Y
   BNE   .cmp_true@33
   LDA   #$00
   JMP   .cmp_end@34
.cmp_true@33:
   LDA   #$01
.cmp_end@34:
   LDY   #$96
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@30
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   JMP   .or_end@31
.or_true@30:
   LDA   #$01
   LDY   #$98
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@31:
   LDY   #$98
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@28
   CLC
   LDA   FP
   ADC   #$43
   INY
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$02
   LDY   #$9C
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$9D
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$9E
   STA   (FP),Y
   ORA   #$00
   BNE   .or_true@28
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   JMP   .or_end@29
.or_true@28:
   LDA   #$01
   LDY   #$9F
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@29:
   LDY   #$9F
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
   ADC   #$B2
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$B1
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
   CLC
   LDA   FP
   ADC   #$53
   LDY   #$A1
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   LDY   #$A3
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$A4
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$A5
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CMP   #$61
   BNE   .cmp_true@36
   LDA   #$00
   JMP   .cmp_end@37
.cmp_true@36:
   LDA   #$01
.cmp_end@37:
   LDY   #$A8
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@33
   CLC
   LDA   FP
   ADC   #$53
   INY
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$01
   LDY   #$AC
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$AD
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$AE
   STA   (FP),Y
   ORA   #$00
   BNE   .or_true@33
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   JMP   .or_end@34
.or_true@33:
   LDA   #$01
   LDY   #$AF
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@34:
   LDY   #$AF
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@35
   LDX   #$00
   LDA   #$06

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$B2
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
.if_end@35:
   LDX   #$00
   TXA

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$B2
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$B1
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

static1:
   DC.B  $01, $02, $00, $00

static2:
   DC.B  $03, $04, $00, $00

static3:
   DC.B  $05, $00, $00
