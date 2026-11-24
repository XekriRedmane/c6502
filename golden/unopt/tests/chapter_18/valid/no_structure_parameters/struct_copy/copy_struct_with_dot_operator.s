SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24

test_copy_from_member:
   SUBROUTINE

   ; prologue: 0 arg bytes, 182 local bytes
   SEC
   LDA   SSP
   SBC   #$B8
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$B7
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   LDA   #$61
   LDY   #$01
   STA   (FP),Y
   CLC
   LDA   FP
   ADC   #$02
   LDY   #$05
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
   LDY   #$01
   LDA   (FP),Y
   DEY
   STA   (DPTR),Y
   LDA   #$62
   LDY   #$07
   STA   (FP),Y
   LDY   #$05
   LDA   (FP),Y
   CLC
   ADC   #$01
   LDY   #$08
   STA   (FP),Y
   LDY   #$06
   LDA   (FP),Y
   ADC   #$00
   LDY   #$09
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$07
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDA   #$63
   LDY   #$0A
   STA   (FP),Y
   LDY   #$05
   LDA   (FP),Y
   CLC
   ADC   #$02
   LDY   #$0B
   STA   (FP),Y
   LDY   #$06
   LDA   (FP),Y
   ADC   #$00
   LDY   #$0C
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$0A
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   SEC
   LDA   #$00
   SBC   #$01
   LDY   #$0D
   STA   (FP),Y
   LDA   #$00
   SBC   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$0F
   STA   (FP),Y
   CLC
   LDA   FP
   ADC   #$10
   LDY   #$15
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
   LDY   #$0F
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   SEC
   LDA   #$00
   SBC   #$01
   LDY   #$17
   STA   (FP),Y
   LDA   #$00
   SBC   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$19
   STA   (FP),Y
   LDY   #$15
   LDA   (FP),Y
   CLC
   ADC   #$01
   LDY   #$1A
   STA   (FP),Y
   LDY   #$16
   LDA   (FP),Y
   ADC   #$00
   LDY   #$1B
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$19
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   SEC
   LDA   #$00
   SBC   #$01
   LDY   #$1C
   STA   (FP),Y
   LDA   #$00
   SBC   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$1E
   STA   (FP),Y
   LDY   #$15
   LDA   (FP),Y
   CLC
   ADC   #$02
   LDY   #$1F
   STA   (FP),Y
   LDY   #$16
   LDA   (FP),Y
   ADC   #$00
   LDY   #$20
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$1E
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   SEC
   LDA   #$00
   SBC   #$01
   LDY   #$21
   STA   (FP),Y
   LDA   #$00
   SBC   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$23
   STA   (FP),Y
   LDY   #$15
   LDA   (FP),Y
   CLC
   ADC   #$03
   LDY   #$24
   STA   (FP),Y
   LDY   #$16
   LDA   (FP),Y
   ADC   #$00
   LDY   #$25
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$23
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   SEC
   LDA   #$00
   SBC   #$01
   LDY   #$26
   STA   (FP),Y
   LDA   #$00
   SBC   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$28
   STA   (FP),Y
   LDY   #$15
   LDA   (FP),Y
   CLC
   ADC   #$04
   LDY   #$29
   STA   (FP),Y
   LDY   #$16
   LDA   (FP),Y
   ADC   #$00
   LDY   #$2A
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$28
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDA   #$64
   LDY   #$2B
   STA   (FP),Y
   CLC
   LDA   FP
   ADC   #$2C
   LDY   #$2F
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
   LDY   #$2B
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDA   #$65
   LDY   #$31
   STA   (FP),Y
   LDY   #$2F
   LDA   (FP),Y
   CLC
   ADC   #$01
   LDY   #$32
   STA   (FP),Y
   LDY   #$30
   LDA   (FP),Y
   ADC   #$00
   LDY   #$33
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$31
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDA   #$66
   LDY   #$34
   STA   (FP),Y
   LDY   #$2F
   LDA   (FP),Y
   CLC
   ADC   #$02
   LDY   #$35
   STA   (FP),Y
   LDY   #$30
   LDA   (FP),Y
   ADC   #$00
   LDY   #$36
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$34
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDA   #<@0.big_struct
   LDY   #$37
   STA   (FP),Y
   LDA   #>@0.big_struct
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$39
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$3A
   STA   (FP),Y
   LDY   #$02
   LDA   (DPTR),Y
   LDY   #$3B
   STA   (FP),Y
   LDY   #$03
   LDA   (DPTR),Y
   LDY   #$3C
   STA   (FP),Y
   LDY   #$04
   LDA   (DPTR),Y
   LDY   #$3D
   STA   (FP),Y
   LDY   #$39
   LDA   (FP),Y
   LDY   #$10
   STA   (FP),Y
   LDY   #$3A
   LDA   (FP),Y
   LDY   #$11
   STA   (FP),Y
   LDY   #$3B
   LDA   (FP),Y
   LDY   #$12
   STA   (FP),Y
   LDY   #$3C
   LDA   (FP),Y
   LDY   #$13
   STA   (FP),Y
   LDY   #$3D
   LDA   (FP),Y
   LDY   #$14
   STA   (FP),Y
   CLC
   LDA   FP
   ADC   #$10
   LDY   #$3E
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
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$40
   STA   (FP),Y
   INY
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@0
   LDA   #$00
   JMP   .sx_done@1
.sx_neg@0:
   LDA   #$FF
.sx_done@1:
   LDY   #$42
   STA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@2
   DEY
   LDA   (FP),Y
   CMP   #$0A
   BNE   .cmp_true@2
   LDA   #$00
   JMP   .cmp_end@3
.cmp_true@2:
   LDA   #$01
.cmp_end@3:
   LDY   #$43
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@3
   JMP   .or_true@6
.lb_skip@3:
   CLC
   LDA   FP
   ADC   #$10
   LDY   #$45
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$01
   LDY   #$47
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$48
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$49
   STA   (FP),Y
   INY
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@5
   LDA   #$00
   JMP   .sx_done@6
.sx_neg@5:
   LDA   #$FF
.sx_done@6:
   LDY   #$4B
   STA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@7
   DEY
   LDA   (FP),Y
   CMP   #$09
   BNE   .cmp_true@7
   LDA   #$00
   JMP   .cmp_end@8
.cmp_true@7:
   LDA   #$01
.cmp_end@8:
   LDY   #$4C
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
   LDY   #$4E
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@7:
   LDY   #$4E
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@2
   JMP   .or_true@4
.lb_skip@2:
   CLC
   LDA   FP
   ADC   #$10
   LDY   #$50
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$02
   LDY   #$52
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$53
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   LDY   #$54
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$55
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$56
   STA   (FP),Y
   INY
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@10
   LDA   #$00
   JMP   .sx_done@11
.sx_neg@10:
   LDA   #$FF
.sx_done@11:
   LDY   #$58
   STA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@12
   DEY
   LDA   (FP),Y
   CMP   #$08
   BNE   .cmp_true@12
   LDA   #$00
   JMP   .cmp_end@13
.cmp_true@12:
   LDA   #$01
.cmp_end@13:
   LDY   #$59
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
   LDY   #$5B
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@5:
   LDY   #$5B
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@1
   JMP   .or_true@2
.lb_skip@1:
   CLC
   LDA   FP
   ADC   #$10
   LDY   #$5D
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$02
   LDY   #$5F
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$60
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$01
   LDY   #$61
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$62
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$63
   STA   (FP),Y
   INY
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@15
   LDA   #$00
   JMP   .sx_done@16
.sx_neg@15:
   LDA   #$FF
.sx_done@16:
   LDY   #$65
   STA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@17
   DEY
   LDA   (FP),Y
   CMP   #$07
   BNE   .cmp_true@17
   LDA   #$00
   JMP   .cmp_end@18
.cmp_true@17:
   LDA   #$01
.cmp_end@18:
   LDY   #$66
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
   LDY   #$68
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@3:
   LDY   #$68
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@0
   JMP   .or_true@0
.lb_skip@0:
   CLC
   LDA   FP
   ADC   #$10
   LDY   #$6A
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$02
   LDY   #$6C
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$6D
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$02
   LDY   #$6E
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$6F
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$70
   STA   (FP),Y
   INY
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@20
   LDA   #$00
   JMP   .sx_done@21
.sx_neg@20:
   LDA   #$FF
.sx_done@21:
   LDY   #$72
   STA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@22
   DEY
   LDA   (FP),Y
   CMP   #$06
   BNE   .cmp_true@22
   LDA   #$00
   JMP   .cmp_end@23
.cmp_true@22:
   LDA   #$01
.cmp_end@23:
   LDY   #$73
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
   LDY   #$75
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@1:
   LDY   #$75
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@8
   LDX   #$00
   TXA

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$B8
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$B7
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
   CLC
   LDA   FP
   ADC   #$02
   LDY   #$77
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   LDY   #$79
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$7A
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$7B
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CMP   #$61
   BNE   .cmp_true@25
   LDA   #$00
   JMP   .cmp_end@26
.cmp_true@25:
   LDA   #$01
.cmp_end@26:
   LDY   #$7E
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@17
   CLC
   LDA   FP
   ADC   #$02
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
   LDY   #$82
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$83
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$84
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CMP   #$62
   BNE   .cmp_true@27
   LDA   #$00
   JMP   .cmp_end@28
.cmp_true@27:
   LDA   #$01
.cmp_end@28:
   LDY   #$87
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@17
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   JMP   .or_end@18
.or_true@17:
   LDA   #$01
   LDY   #$89
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@18:
   LDY   #$89
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@15
   CLC
   LDA   FP
   ADC   #$02
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
   LDY   #$8D
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$8E
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$8F
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CMP   #$63
   BNE   .cmp_true@29
   LDA   #$00
   JMP   .cmp_end@30
.cmp_true@29:
   LDA   #$01
.cmp_end@30:
   LDY   #$92
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
   LDY   #$94
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@16:
   LDY   #$94
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@13
   CLC
   LDA   FP
   ADC   #$2C
   INY
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   LDY   #$98
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$99
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$9A
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CMP   #$64
   BNE   .cmp_true@31
   LDA   #$00
   JMP   .cmp_end@32
.cmp_true@31:
   LDA   #$01
.cmp_end@32:
   LDY   #$9D
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
   LDY   #$9F
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@14:
   LDY   #$9F
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@11
   CLC
   LDA   FP
   ADC   #$2C
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
   CMP   #$65
   BNE   .cmp_true@33
   LDA   #$00
   JMP   .cmp_end@34
.cmp_true@33:
   LDA   #$01
.cmp_end@34:
   LDY   #$A8
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
   LDY   #$AA
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@12:
   LDY   #$AA
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@9
   CLC
   LDA   FP
   ADC   #$2C
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
   LDY   #$AE
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$AF
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$B0
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CMP   #$66
   BNE   .cmp_true@35
   LDA   #$00
   JMP   .cmp_end@36
.cmp_true@35:
   LDA   #$01
.cmp_end@36:
   LDY   #$B3
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
   LDY   #$B5
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@10:
   LDY   #$B5
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@19
   LDX   #$00
   TXA

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$B8
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
   LDA   #$01

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$B8
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$B7
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

test_copy_to_member:
   SUBROUTINE

   ; prologue: 0 arg bytes, 111 local bytes
   SEC
   LDA   SSP
   SBC   #$71
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$70
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
   SBC   #$01
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
   CLC
   LDA   FP
   ADC   #$04
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
   LDY   #$03
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   SEC
   LDA   #$00
   SBC   #$02
   LDY   #$0B
   STA   (FP),Y
   LDA   #$00
   SBC   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$0D
   STA   (FP),Y
   LDY   #$09
   LDA   (FP),Y
   CLC
   ADC   #$01
   LDY   #$0E
   STA   (FP),Y
   LDY   #$0A
   LDA   (FP),Y
   ADC   #$00
   LDY   #$0F
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$0D
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   SEC
   LDA   #$00
   SBC   #$03
   LDY   #$10
   STA   (FP),Y
   LDA   #$00
   SBC   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$12
   STA   (FP),Y
   LDY   #$09
   LDA   (FP),Y
   CLC
   ADC   #$02
   LDY   #$13
   STA   (FP),Y
   LDY   #$0A
   LDA   (FP),Y
   ADC   #$00
   LDY   #$14
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$12
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   SEC
   LDA   #$00
   SBC   #$04
   LDY   #$15
   STA   (FP),Y
   LDA   #$00
   SBC   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$17
   STA   (FP),Y
   LDY   #$09
   LDA   (FP),Y
   CLC
   ADC   #$03
   LDY   #$18
   STA   (FP),Y
   LDY   #$0A
   LDA   (FP),Y
   ADC   #$00
   LDY   #$19
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$17
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   SEC
   LDA   #$00
   SBC   #$05
   LDY   #$1A
   STA   (FP),Y
   LDA   #$00
   SBC   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$1C
   STA   (FP),Y
   LDY   #$09
   LDA   (FP),Y
   CLC
   ADC   #$04
   LDY   #$1D
   STA   (FP),Y
   LDY   #$0A
   LDA   (FP),Y
   ADC   #$00
   LDY   #$1E
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$1C
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDA   #<@4.big_struct
   LDY   #$1F
   STA   (FP),Y
   LDA   #>@4.big_struct
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$04
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$05
   LDA   (FP),Y
   LDY   #$01
   STA   (DPTR),Y
   LDY   #$06
   LDA   (FP),Y
   LDY   #$02
   STA   (DPTR),Y
   LDY   #$07
   LDA   (FP),Y
   LDY   #$03
   STA   (DPTR),Y
   LDY   #$08
   LDA   (FP),Y
   LDY   #$04
   STA   (DPTR),Y
   LDA   #<@4.big_struct
   LDY   #$21
   STA   (FP),Y
   LDA   #>@4.big_struct
   INY
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
   ORA   #$00
   BMI   .sx_neg@37
   LDA   #$00
   JMP   .sx_done@38
.sx_neg@37:
   LDA   #$FF
.sx_done@38:
   LDY   #$25
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
   LDY   #$25
   LDA   (FP),Y
   LDY   #$27
   CMP   (FP),Y
   BNE   .cmp_true@39
   LDY   #$24
   LDA   (FP),Y
   LDY   #$26
   CMP   (FP),Y
   BNE   .cmp_true@39
   LDA   #$00
   JMP   .cmp_end@40
.cmp_true@39:
   LDA   #$01
.cmp_end@40:
   LDY   #$28
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@7
   JMP   .or_true@26
.lb_skip@7:
   LDA   #<@4.big_struct
   LDY   #$2A
   STA   (FP),Y
   LDA   #>@4.big_struct
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$01
   LDY   #$2C
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$2D
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$2E
   STA   (FP),Y
   INY
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@42
   LDA   #$00
   JMP   .sx_done@43
.sx_neg@42:
   LDA   #$FF
.sx_done@43:
   LDY   #$30
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
   LDY   #$30
   LDA   (FP),Y
   LDY   #$32
   CMP   (FP),Y
   BNE   .cmp_true@44
   LDY   #$2F
   LDA   (FP),Y
   LDY   #$31
   CMP   (FP),Y
   BNE   .cmp_true@44
   LDA   #$00
   JMP   .cmp_end@45
.cmp_true@44:
   LDA   #$01
.cmp_end@45:
   LDY   #$33
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@26
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   JMP   .or_end@27
.or_true@26:
   LDA   #$01
   LDY   #$35
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@27:
   LDY   #$35
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@6
   JMP   .or_true@24
.lb_skip@6:
   LDA   #<@4.big_struct
   LDY   #$37
   STA   (FP),Y
   LDA   #>@4.big_struct
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$02
   LDY   #$39
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$3A
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   LDY   #$3B
   STA   (FP),Y
   DEY
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
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$3D
   STA   (FP),Y
   INY
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@47
   LDA   #$00
   JMP   .sx_done@48
.sx_neg@47:
   LDA   #$FF
.sx_done@48:
   LDY   #$3F
   STA   (FP),Y
   SEC
   LDA   #$00
   SBC   #$03
   INY
   STA   (FP),Y
   LDA   #$00
   SBC   #$00
   INY
   STA   (FP),Y
   LDY   #$3F
   LDA   (FP),Y
   LDY   #$41
   CMP   (FP),Y
   BNE   .cmp_true@49
   LDY   #$3E
   LDA   (FP),Y
   LDY   #$40
   CMP   (FP),Y
   BNE   .cmp_true@49
   LDA   #$00
   JMP   .cmp_end@50
.cmp_true@49:
   LDA   #$01
.cmp_end@50:
   LDY   #$42
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@24
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   JMP   .or_end@25
.or_true@24:
   LDA   #$01
   LDY   #$44
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@25:
   LDY   #$44
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@5
   JMP   .or_true@22
.lb_skip@5:
   LDA   #<@4.big_struct
   LDY   #$46
   STA   (FP),Y
   LDA   #>@4.big_struct
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$02
   LDY   #$48
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$49
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$01
   LDY   #$4A
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$4B
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$4C
   STA   (FP),Y
   INY
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@52
   LDA   #$00
   JMP   .sx_done@53
.sx_neg@52:
   LDA   #$FF
.sx_done@53:
   LDY   #$4E
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
   LDY   #$4E
   LDA   (FP),Y
   LDY   #$50
   CMP   (FP),Y
   BNE   .cmp_true@54
   LDY   #$4D
   LDA   (FP),Y
   LDY   #$4F
   CMP   (FP),Y
   BNE   .cmp_true@54
   LDA   #$00
   JMP   .cmp_end@55
.cmp_true@54:
   LDA   #$01
.cmp_end@55:
   LDY   #$51
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@22
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   JMP   .or_end@23
.or_true@22:
   LDA   #$01
   LDY   #$53
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@23:
   LDY   #$53
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@4
   JMP   .or_true@20
.lb_skip@4:
   LDA   #<@4.big_struct
   LDY   #$55
   STA   (FP),Y
   LDA   #>@4.big_struct
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$02
   LDY   #$57
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$58
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$02
   LDY   #$59
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$5A
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$5B
   STA   (FP),Y
   INY
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@57
   LDA   #$00
   JMP   .sx_done@58
.sx_neg@57:
   LDA   #$FF
.sx_done@58:
   LDY   #$5D
   STA   (FP),Y
   SEC
   LDA   #$00
   SBC   #$05
   INY
   STA   (FP),Y
   LDA   #$00
   SBC   #$00
   INY
   STA   (FP),Y
   LDY   #$5D
   LDA   (FP),Y
   LDY   #$5F
   CMP   (FP),Y
   BNE   .cmp_true@59
   LDY   #$5C
   LDA   (FP),Y
   LDY   #$5E
   CMP   (FP),Y
   BNE   .cmp_true@59
   LDA   #$00
   JMP   .cmp_end@60
.cmp_true@59:
   LDA   #$01
.cmp_end@60:
   LDY   #$60
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@20
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   JMP   .or_end@21
.or_true@20:
   LDA   #$01
   LDY   #$62
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@21:
   LDY   #$62
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@28
   LDX   #$00
   TXA

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$71
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$70
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
   LDA   #<@4.big_struct
   LDY   #$64
   STA   (FP),Y
   LDA   #>@4.big_struct
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$05
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
   ORA   #$00
   BNE   .or_true@29
   LDA   #<@4.big_struct
   INY
   STA   (FP),Y
   LDA   #>@4.big_struct
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$06
   LDY   #$6B
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$6C
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$6D
   STA   (FP),Y
   ORA   #$00
   BNE   .or_true@29
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   JMP   .or_end@30
.or_true@29:
   LDA   #$01
   LDY   #$6E
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@30:
   LDY   #$6E
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@31
   LDX   #$00
   TXA

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$71
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
.if_end@31:
   LDX   #$00
   LDA   #$01

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$71
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$70
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

test_copy_from_nested_member:
   SUBROUTINE

   ; prologue: 0 arg bytes, 120 local bytes
   SEC
   LDA   SSP
   SBC   #$7A
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$79
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
   SBC   #$01
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
   CLC
   LDA   FP
   ADC   #$04
   LDY   #$0D
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
   LDY   #$03
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   SEC
   LDA   #$00
   SBC   #$02
   LDY   #$0F
   STA   (FP),Y
   LDA   #$00
   SBC   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$11
   STA   (FP),Y
   LDY   #$0D
   LDA   (FP),Y
   CLC
   ADC   #$01
   LDY   #$12
   STA   (FP),Y
   LDY   #$0E
   LDA   (FP),Y
   ADC   #$00
   LDY   #$13
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$11
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   SEC
   LDA   #$00
   SBC   #$03
   LDY   #$14
   STA   (FP),Y
   LDA   #$00
   SBC   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$16
   STA   (FP),Y
   LDY   #$0D
   LDA   (FP),Y
   CLC
   ADC   #$02
   LDY   #$17
   STA   (FP),Y
   LDY   #$0E
   LDA   (FP),Y
   ADC   #$00
   LDY   #$18
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$16
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   SEC
   LDA   #$00
   SBC   #$04
   LDY   #$19
   STA   (FP),Y
   LDA   #$00
   SBC   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$1B
   STA   (FP),Y
   LDY   #$0D
   LDA   (FP),Y
   CLC
   ADC   #$03
   LDY   #$1C
   STA   (FP),Y
   LDY   #$0E
   LDA   (FP),Y
   ADC   #$00
   LDY   #$1D
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$1B
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   SEC
   LDA   #$00
   SBC   #$05
   LDY   #$1E
   STA   (FP),Y
   LDA   #$00
   SBC   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$20
   STA   (FP),Y
   LDY   #$0D
   LDA   (FP),Y
   CLC
   ADC   #$04
   LDY   #$21
   STA   (FP),Y
   LDY   #$0E
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
   LDY   #$20
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   SEC
   LDA   #$00
   SBC   #$06
   LDY   #$23
   STA   (FP),Y
   LDA   #$00
   SBC   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$25
   STA   (FP),Y
   LDY   #$0D
   LDA   (FP),Y
   CLC
   ADC   #$05
   LDY   #$26
   STA   (FP),Y
   LDY   #$0E
   LDA   (FP),Y
   ADC   #$00
   LDY   #$27
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$25
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   SEC
   LDA   #$00
   SBC   #$07
   LDY   #$28
   STA   (FP),Y
   LDA   #$00
   SBC   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$2A
   STA   (FP),Y
   LDY   #$0D
   LDA   (FP),Y
   CLC
   ADC   #$06
   LDY   #$2B
   STA   (FP),Y
   LDY   #$0E
   LDA   (FP),Y
   ADC   #$00
   LDY   #$2C
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$2A
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$0D
   LDA   (FP),Y
   CLC
   ADC   #$07
   LDY   #$2D
   STA   (FP),Y
   LDY   #$0E
   LDA   (FP),Y
   ADC   #$00
   LDY   #$2E
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
   CLC
   LDA   FP
   ADC   #$04
   LDY   #$2F
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
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$31
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$32
   STA   (FP),Y
   LDY   #$02
   LDA   (DPTR),Y
   LDY   #$33
   STA   (FP),Y
   LDY   #$03
   LDA   (DPTR),Y
   LDY   #$34
   STA   (FP),Y
   LDY   #$04
   LDA   (DPTR),Y
   LDY   #$35
   STA   (FP),Y
   LDY   #$31
   LDA   (FP),Y
   STA   @7.small_struct
   INY
   LDA   (FP),Y
   STA   @7.small_struct+1
   INY
   LDA   (FP),Y
   STA   @7.small_struct+2
   INY
   LDA   (FP),Y
   STA   @7.small_struct+3
   INY
   LDA   (FP),Y
   STA   @7.small_struct+4
   LDA   #<@7.small_struct
   INY
   STA   (FP),Y
   LDA   #>@7.small_struct
   INY
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
   ORA   #$00
   BMI   .sx_neg@62
   LDA   #$00
   JMP   .sx_done@63
.sx_neg@62:
   LDA   #$FF
.sx_done@63:
   LDY   #$3A
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
   LDY   #$3A
   LDA   (FP),Y
   LDY   #$3C
   CMP   (FP),Y
   BNE   .cmp_true@64
   LDY   #$39
   LDA   (FP),Y
   LDY   #$3B
   CMP   (FP),Y
   BNE   .cmp_true@64
   LDA   #$00
   JMP   .cmp_end@65
.cmp_true@64:
   LDA   #$01
.cmp_end@65:
   LDY   #$3D
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@11
   JMP   .or_true@38
.lb_skip@11:
   LDA   #<@7.small_struct
   LDY   #$3F
   STA   (FP),Y
   LDA   #>@7.small_struct
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$01
   LDY   #$41
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$42
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$43
   STA   (FP),Y
   INY
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@67
   LDA   #$00
   JMP   .sx_done@68
.sx_neg@67:
   LDA   #$FF
.sx_done@68:
   LDY   #$45
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
   LDY   #$45
   LDA   (FP),Y
   LDY   #$47
   CMP   (FP),Y
   BNE   .cmp_true@69
   LDY   #$44
   LDA   (FP),Y
   LDY   #$46
   CMP   (FP),Y
   BNE   .cmp_true@69
   LDA   #$00
   JMP   .cmp_end@70
.cmp_true@69:
   LDA   #$01
.cmp_end@70:
   LDY   #$48
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@38
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   JMP   .or_end@39
.or_true@38:
   LDA   #$01
   LDY   #$4A
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@39:
   LDY   #$4A
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@10
   JMP   .or_true@36
.lb_skip@10:
   LDA   #<@7.small_struct
   LDY   #$4C
   STA   (FP),Y
   LDA   #>@7.small_struct
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$02
   LDY   #$4E
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$4F
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   LDY   #$50
   STA   (FP),Y
   DEY
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
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$52
   STA   (FP),Y
   INY
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@72
   LDA   #$00
   JMP   .sx_done@73
.sx_neg@72:
   LDA   #$FF
.sx_done@73:
   LDY   #$54
   STA   (FP),Y
   SEC
   LDA   #$00
   SBC   #$03
   INY
   STA   (FP),Y
   LDA   #$00
   SBC   #$00
   INY
   STA   (FP),Y
   LDY   #$54
   LDA   (FP),Y
   LDY   #$56
   CMP   (FP),Y
   BNE   .cmp_true@74
   LDY   #$53
   LDA   (FP),Y
   LDY   #$55
   CMP   (FP),Y
   BNE   .cmp_true@74
   LDA   #$00
   JMP   .cmp_end@75
.cmp_true@74:
   LDA   #$01
.cmp_end@75:
   LDY   #$57
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@36
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   JMP   .or_end@37
.or_true@36:
   LDA   #$01
   LDY   #$59
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@37:
   LDY   #$59
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@9
   JMP   .or_true@34
.lb_skip@9:
   LDA   #<@7.small_struct
   LDY   #$5B
   STA   (FP),Y
   LDA   #>@7.small_struct
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$02
   LDY   #$5D
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$5E
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$01
   LDY   #$5F
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$60
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$61
   STA   (FP),Y
   INY
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@77
   LDA   #$00
   JMP   .sx_done@78
.sx_neg@77:
   LDA   #$FF
.sx_done@78:
   LDY   #$63
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
   LDY   #$63
   LDA   (FP),Y
   LDY   #$65
   CMP   (FP),Y
   BNE   .cmp_true@79
   LDY   #$62
   LDA   (FP),Y
   LDY   #$64
   CMP   (FP),Y
   BNE   .cmp_true@79
   LDA   #$00
   JMP   .cmp_end@80
.cmp_true@79:
   LDA   #$01
.cmp_end@80:
   LDY   #$66
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@34
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   JMP   .or_end@35
.or_true@34:
   LDA   #$01
   LDY   #$68
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@35:
   LDY   #$68
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@8
   JMP   .or_true@32
.lb_skip@8:
   LDA   #<@7.small_struct
   LDY   #$6A
   STA   (FP),Y
   LDA   #>@7.small_struct
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$02
   LDY   #$6C
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$6D
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$02
   LDY   #$6E
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$6F
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$70
   STA   (FP),Y
   INY
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@82
   LDA   #$00
   JMP   .sx_done@83
.sx_neg@82:
   LDA   #$FF
.sx_done@83:
   LDY   #$72
   STA   (FP),Y
   SEC
   LDA   #$00
   SBC   #$05
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
   BNE   .cmp_true@84
   LDY   #$71
   LDA   (FP),Y
   LDY   #$73
   CMP   (FP),Y
   BNE   .cmp_true@84
   LDA   #$00
   JMP   .cmp_end@85
.cmp_true@84:
   LDA   #$01
.cmp_end@85:
   LDY   #$75
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@32
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   JMP   .or_end@33
.or_true@32:
   LDA   #$01
   LDY   #$77
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@33:
   LDY   #$77
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@40
   LDX   #$00
   TXA

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$7A
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
.if_end@40:
   LDX   #$00
   LDA   #$01

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$7A
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$79
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

test_copy_to_nested_member:
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

   CLC
   LDA   FP
   ADC   #$01
   LDY   #$0A
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
   LDA   #$00
   LDY   #$00
   STA   (DPTR),Y
   INY
   STA   (DPTR),Y
   INY
   STA   (DPTR),Y
   INY
   STA   (DPTR),Y
   LDY   #$0A
   LDA   (FP),Y
   CLC
   ADC   #$04
   LDY   #$0C
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$0D
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
   LDY   #$0A
   LDA   (FP),Y
   CLC
   ADC   #$06
   LDY   #$0E
   STA   (FP),Y
   LDY   #$0B
   LDA   (FP),Y
   ADC   #$00
   LDY   #$0F
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
   SEC
   SBC   #$01
   LDY   #$10
   STA   (FP),Y
   LDA   #$00
   SBC   #$00
   INY
   STA   (FP),Y
   LDY   #$0A
   LDA   (FP),Y
   CLC
   ADC   #$07
   LDY   #$12
   STA   (FP),Y
   LDY   #$0B
   LDA   (FP),Y
   ADC   #$00
   LDY   #$13
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$10
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$11
   LDA   (FP),Y
   LDY   #$01
   STA   (DPTR),Y
   CLC
   LDA   FP
   ADC   #$01
   LDY   #$14
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
   LDA   @9.small_struct
   LDY   #$00
   STA   (DPTR),Y
   LDA   @9.small_struct+1
   INY
   STA   (DPTR),Y
   LDA   @9.small_struct+2
   INY
   STA   (DPTR),Y
   LDA   @9.small_struct+3
   INY
   STA   (DPTR),Y
   LDA   @9.small_struct+4
   INY
   STA   (DPTR),Y
   CLC
   LDA   FP
   ADC   #$01
   LDY   #$16
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
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$18
   STA   (FP),Y
   INY
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@87
   LDA   #$00
   JMP   .sx_done@88
.sx_neg@87:
   LDA   #$FF
.sx_done@88:
   LDY   #$1A
   STA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@89
   DEY
   LDA   (FP),Y
   CMP   #$32
   BNE   .cmp_true@89
   LDA   #$00
   JMP   .cmp_end@90
.cmp_true@89:
   LDA   #$01
.cmp_end@90:
   LDY   #$1B
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@15
   JMP   .or_true@47
.lb_skip@15:
   CLC
   LDA   FP
   ADC   #$01
   LDY   #$1D
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$01
   LDY   #$1F
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$20
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$21
   STA   (FP),Y
   INY
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@92
   LDA   #$00
   JMP   .sx_done@93
.sx_neg@92:
   LDA   #$FF
.sx_done@93:
   LDY   #$23
   STA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@94
   DEY
   LDA   (FP),Y
   CMP   #$33
   BNE   .cmp_true@94
   LDA   #$00
   JMP   .cmp_end@95
.cmp_true@94:
   LDA   #$01
.cmp_end@95:
   LDY   #$24
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@47
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   JMP   .or_end@48
.or_true@47:
   LDA   #$01
   LDY   #$26
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@48:
   LDY   #$26
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@14
   JMP   .or_true@45
.lb_skip@14:
   CLC
   LDA   FP
   ADC   #$01
   LDY   #$28
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$02
   LDY   #$2A
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$2B
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   LDY   #$2C
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$2D
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$2E
   STA   (FP),Y
   INY
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@97
   LDA   #$00
   JMP   .sx_done@98
.sx_neg@97:
   LDA   #$FF
.sx_done@98:
   LDY   #$30
   STA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@99
   DEY
   LDA   (FP),Y
   CMP   #$34
   BNE   .cmp_true@99
   LDA   #$00
   JMP   .cmp_end@100
.cmp_true@99:
   LDA   #$01
.cmp_end@100:
   LDY   #$31
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@45
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   JMP   .or_end@46
.or_true@45:
   LDA   #$01
   LDY   #$33
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@46:
   LDY   #$33
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@13
   JMP   .or_true@43
.lb_skip@13:
   CLC
   LDA   FP
   ADC   #$01
   LDY   #$35
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$02
   LDY   #$37
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$38
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$01
   LDY   #$39
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$3A
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$3B
   STA   (FP),Y
   INY
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@102
   LDA   #$00
   JMP   .sx_done@103
.sx_neg@102:
   LDA   #$FF
.sx_done@103:
   LDY   #$3D
   STA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@104
   DEY
   LDA   (FP),Y
   CMP   #$35
   BNE   .cmp_true@104
   LDA   #$00
   JMP   .cmp_end@105
.cmp_true@104:
   LDA   #$01
.cmp_end@105:
   LDY   #$3E
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@43
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   JMP   .or_end@44
.or_true@43:
   LDA   #$01
   LDY   #$40
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@44:
   LDY   #$40
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@12
   JMP   .or_true@41
.lb_skip@12:
   CLC
   LDA   FP
   ADC   #$01
   LDY   #$42
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$02
   LDY   #$44
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$45
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$02
   LDY   #$46
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$47
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$48
   STA   (FP),Y
   INY
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@107
   LDA   #$00
   JMP   .sx_done@108
.sx_neg@107:
   LDA   #$FF
.sx_done@108:
   LDY   #$4A
   STA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@109
   DEY
   LDA   (FP),Y
   CMP   #$36
   BNE   .cmp_true@109
   LDA   #$00
   JMP   .cmp_end@110
.cmp_true@109:
   LDA   #$01
.cmp_end@110:
   LDY   #$4B
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@41
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   JMP   .or_end@42
.or_true@41:
   LDA   #$01
   LDY   #$4D
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@42:
   LDY   #$4D
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@49
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
.if_end@49:
   CLC
   LDA   FP
   ADC   #$01
   LDY   #$4F
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$05
   LDY   #$51
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$52
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$53
   STA   (FP),Y
   ORA   #$00
   BNE   .or_true@50
   CLC
   LDA   FP
   ADC   #$01
   INY
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$06
   LDY   #$56
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$57
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$58
   STA   (FP),Y
   ORA   #$00
   BNE   .or_true@50
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   JMP   .or_end@51
.or_true@50:
   LDA   #$01
   LDY   #$59
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@51:
   LDY   #$59
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@52
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
.if_end@52:
   CLC
   LDA   FP
   ADC   #$01
   LDY   #$5B
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$07
   LDY   #$5D
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$5E
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$5F
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$60
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
   LDY   #$60
   LDA   (FP),Y
   LDY   #$62
   CMP   (FP),Y
   BNE   .cmp_true@112
   LDY   #$5F
   LDA   (FP),Y
   LDY   #$61
   CMP   (FP),Y
   BNE   .cmp_true@112
   LDA   #$00
   JMP   .cmp_end@113
.cmp_true@112:
   LDA   #$01
.cmp_end@113:
   LDY   #$63
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@53
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
.if_end@53:
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

test_copy_from_conditional:
   SUBROUTINE

   ; prologue: 0 arg bytes, 210 local bytes
   SEC
   LDA   SSP
   SBC   #$D4
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$D3
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   LDA   #$7F
   LDY   #$01
   STA   (FP),Y
   CLC
   LDA   FP
   ADC   #$02
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
   LDY   #$01
   LDA   (FP),Y
   DEY
   STA   (DPTR),Y
   SEC
   LDA   #$00
   SBC   #$80
   LDY   #$0B
   STA   (FP),Y
   LDA   #$00
   SBC   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$0D
   STA   (FP),Y
   LDY   #$09
   LDA   (FP),Y
   CLC
   ADC   #$01
   LDY   #$0E
   STA   (FP),Y
   LDY   #$0A
   LDA   (FP),Y
   ADC   #$00
   LDY   #$0F
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$0D
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDA   #$3D
   LDY   #$10
   STA   (FP),Y
   LDY   #$09
   LDA   (FP),Y
   CLC
   ADC   #$02
   LDY   #$11
   STA   (FP),Y
   LDY   #$0A
   LDA   (FP),Y
   ADC   #$00
   LDY   #$12
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$10
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDA   #$3E
   LDY   #$13
   STA   (FP),Y
   LDY   #$09
   LDA   (FP),Y
   CLC
   ADC   #$03
   LDY   #$14
   STA   (FP),Y
   LDY   #$0A
   LDA   (FP),Y
   ADC   #$00
   LDY   #$15
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$13
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDA   #$3F
   LDY   #$16
   STA   (FP),Y
   LDY   #$09
   LDA   (FP),Y
   CLC
   ADC   #$04
   LDY   #$17
   STA   (FP),Y
   LDY   #$0A
   LDA   (FP),Y
   ADC   #$00
   LDY   #$18
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$16
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   SEC
   LDA   #$00
   SBC   #$0A
   LDY   #$19
   STA   (FP),Y
   LDA   #$00
   SBC   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$1B
   STA   (FP),Y
   LDY   #$09
   LDA   (FP),Y
   CLC
   ADC   #$05
   LDY   #$1C
   STA   (FP),Y
   LDY   #$0A
   LDA   (FP),Y
   ADC   #$00
   LDY   #$1D
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$1B
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   SEC
   LDA   #$00
   SBC   #$0B
   LDY   #$1E
   STA   (FP),Y
   LDA   #$00
   SBC   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$20
   STA   (FP),Y
   LDY   #$09
   LDA   (FP),Y
   CLC
   ADC   #$06
   LDY   #$21
   STA   (FP),Y
   LDY   #$0A
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
   LDY   #$20
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   CLC
   LDA   FP
   ADC   #$23
   LDY   #$2A
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
   LDA   #$00
   LDY   #$00
   STA   (DPTR),Y
   LDA   #$01
   LDY   #$2C
   STA   (FP),Y
   LDY   #$2A
   LDA   (FP),Y
   CLC
   ADC   #$01
   LDY   #$2D
   STA   (FP),Y
   LDY   #$2B
   LDA   (FP),Y
   ADC   #$00
   LDY   #$2E
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$2C
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDA   #$02
   LDY   #$2F
   STA   (FP),Y
   LDY   #$2A
   LDA   (FP),Y
   CLC
   ADC   #$02
   LDY   #$30
   STA   (FP),Y
   LDY   #$2B
   LDA   (FP),Y
   ADC   #$00
   LDY   #$31
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$2F
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDA   #$03
   LDY   #$32
   STA   (FP),Y
   LDY   #$2A
   LDA   (FP),Y
   CLC
   ADC   #$03
   LDY   #$33
   STA   (FP),Y
   LDY   #$2B
   LDA   (FP),Y
   ADC   #$00
   LDY   #$34
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
   LDA   #$04
   LDY   #$35
   STA   (FP),Y
   LDY   #$2A
   LDA   (FP),Y
   CLC
   ADC   #$04
   LDY   #$36
   STA   (FP),Y
   LDY   #$2B
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
   LDY   #$35
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDA   #$05
   LDY   #$38
   STA   (FP),Y
   LDY   #$2A
   LDA   (FP),Y
   CLC
   ADC   #$05
   LDY   #$39
   STA   (FP),Y
   LDY   #$2B
   LDA   (FP),Y
   ADC   #$00
   LDY   #$3A
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$38
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDA   #$06
   LDY   #$3B
   STA   (FP),Y
   LDY   #$2A
   LDA   (FP),Y
   CLC
   ADC   #$06
   LDY   #$3C
   STA   (FP),Y
   LDY   #$2B
   LDA   (FP),Y
   ADC   #$00
   LDY   #$3D
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$3B
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDA   @13.f
   ORA   @13.f+1
   BEQ   .cond_else@54
   LDY   #$02
   LDA   (FP),Y
   LDY   #$3E
   STA   (FP),Y
   LDY   #$03
   LDA   (FP),Y
   LDY   #$3F
   STA   (FP),Y
   LDY   #$04
   LDA   (FP),Y
   LDY   #$40
   STA   (FP),Y
   LDY   #$05
   LDA   (FP),Y
   LDY   #$41
   STA   (FP),Y
   LDY   #$06
   LDA   (FP),Y
   LDY   #$42
   STA   (FP),Y
   LDY   #$07
   LDA   (FP),Y
   LDY   #$43
   STA   (FP),Y
   LDY   #$08
   LDA   (FP),Y
   LDY   #$44
   STA   (FP),Y
   JMP   .cond_end@55
.cond_else@54:
   LDY   #$23
   LDA   (FP),Y
   LDY   #$3E
   STA   (FP),Y
   LDY   #$24
   LDA   (FP),Y
   LDY   #$3F
   STA   (FP),Y
   LDY   #$25
   LDA   (FP),Y
   LDY   #$40
   STA   (FP),Y
   LDY   #$26
   LDA   (FP),Y
   LDY   #$41
   STA   (FP),Y
   LDY   #$27
   LDA   (FP),Y
   LDY   #$42
   STA   (FP),Y
   LDY   #$28
   LDA   (FP),Y
   LDY   #$43
   STA   (FP),Y
   LDY   #$29
   LDA   (FP),Y
   LDY   #$44
   STA   (FP),Y
.cond_end@55:
   CLC
   LDA   FP
   ADC   #$3E
   LDY   #$45
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
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$47
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$48
   STA   (FP),Y
   LDY   #$02
   LDA   (DPTR),Y
   LDY   #$49
   STA   (FP),Y
   LDY   #$03
   LDA   (DPTR),Y
   LDY   #$4A
   STA   (FP),Y
   LDY   #$04
   LDA   (DPTR),Y
   LDY   #$4B
   STA   (FP),Y
   LDY   #$47
   LDA   (FP),Y
   LDY   #$4C
   STA   (FP),Y
   LDY   #$48
   LDA   (FP),Y
   LDY   #$4D
   STA   (FP),Y
   LDY   #$49
   LDA   (FP),Y
   LDY   #$4E
   STA   (FP),Y
   LDY   #$4A
   LDA   (FP),Y
   LDY   #$4F
   STA   (FP),Y
   LDY   #$4B
   LDA   (FP),Y
   LDY   #$50
   STA   (FP),Y
   CLC
   LDA   FP
   ADC   #$4C
   INY
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
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$53
   STA   (FP),Y
   INY
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@115
   LDA   #$00
   JMP   .sx_done@116
.sx_neg@115:
   LDA   #$FF
.sx_done@116:
   LDY   #$55
   STA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@117
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@117
   LDA   #$00
   JMP   .cmp_end@118
.cmp_true@117:
   LDA   #$01
.cmp_end@118:
   LDY   #$56
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@23
   JMP   .or_true@62
.lb_skip@23:
   CLC
   LDA   FP
   ADC   #$4C
   LDY   #$58
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$01
   LDY   #$5A
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$5B
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$5C
   STA   (FP),Y
   INY
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@120
   LDA   #$00
   JMP   .sx_done@121
.sx_neg@120:
   LDA   #$FF
.sx_done@121:
   LDY   #$5E
   STA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@122
   DEY
   LDA   (FP),Y
   CMP   #$01
   BNE   .cmp_true@122
   LDA   #$00
   JMP   .cmp_end@123
.cmp_true@122:
   LDA   #$01
.cmp_end@123:
   LDY   #$5F
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@62
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   JMP   .or_end@63
.or_true@62:
   LDA   #$01
   LDY   #$61
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@63:
   LDY   #$61
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@22
   JMP   .or_true@60
.lb_skip@22:
   CLC
   LDA   FP
   ADC   #$4C
   LDY   #$63
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$02
   LDY   #$65
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$66
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   LDY   #$67
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$68
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$69
   STA   (FP),Y
   INY
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@125
   LDA   #$00
   JMP   .sx_done@126
.sx_neg@125:
   LDA   #$FF
.sx_done@126:
   LDY   #$6B
   STA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@127
   DEY
   LDA   (FP),Y
   CMP   #$02
   BNE   .cmp_true@127
   LDA   #$00
   JMP   .cmp_end@128
.cmp_true@127:
   LDA   #$01
.cmp_end@128:
   LDY   #$6C
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@60
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   JMP   .or_end@61
.or_true@60:
   LDA   #$01
   LDY   #$6E
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@61:
   LDY   #$6E
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@21
   JMP   .or_true@58
.lb_skip@21:
   CLC
   LDA   FP
   ADC   #$4C
   LDY   #$70
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$02
   LDY   #$72
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$73
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$01
   LDY   #$74
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$75
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$76
   STA   (FP),Y
   INY
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@130
   LDA   #$00
   JMP   .sx_done@131
.sx_neg@130:
   LDA   #$FF
.sx_done@131:
   LDY   #$78
   STA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@132
   DEY
   LDA   (FP),Y
   CMP   #$03
   BNE   .cmp_true@132
   LDA   #$00
   JMP   .cmp_end@133
.cmp_true@132:
   LDA   #$01
.cmp_end@133:
   LDY   #$79
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@58
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   JMP   .or_end@59
.or_true@58:
   LDA   #$01
   LDY   #$7B
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@59:
   LDY   #$7B
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@20
   JMP   .or_true@56
.lb_skip@20:
   CLC
   LDA   FP
   ADC   #$4C
   LDY   #$7D
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$02
   LDY   #$7F
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$80
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$02
   LDY   #$81
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$82
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$83
   STA   (FP),Y
   INY
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@135
   LDA   #$00
   JMP   .sx_done@136
.sx_neg@135:
   LDA   #$FF
.sx_done@136:
   LDY   #$85
   STA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@137
   DEY
   LDA   (FP),Y
   CMP   #$04
   BNE   .cmp_true@137
   LDA   #$00
   JMP   .cmp_end@138
.cmp_true@137:
   LDA   #$01
.cmp_end@138:
   LDY   #$86
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@56
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   JMP   .or_end@57
.or_true@56:
   LDA   #$01
   LDY   #$88
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@57:
   LDY   #$88
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@64
   LDX   #$00
   TXA

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$D4
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$D3
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
.if_end@64:
   LDA   @12.t
   ORA   @12.t+1
   BEQ   .cond_else@65
   LDY   #$02
   LDA   (FP),Y
   LDY   #$8A
   STA   (FP),Y
   LDY   #$03
   LDA   (FP),Y
   LDY   #$8B
   STA   (FP),Y
   LDY   #$04
   LDA   (FP),Y
   LDY   #$8C
   STA   (FP),Y
   LDY   #$05
   LDA   (FP),Y
   LDY   #$8D
   STA   (FP),Y
   LDY   #$06
   LDA   (FP),Y
   LDY   #$8E
   STA   (FP),Y
   LDY   #$07
   LDA   (FP),Y
   LDY   #$8F
   STA   (FP),Y
   LDY   #$08
   LDA   (FP),Y
   LDY   #$90
   STA   (FP),Y
   JMP   .cond_end@66
.cond_else@65:
   LDY   #$23
   LDA   (FP),Y
   LDY   #$8A
   STA   (FP),Y
   LDY   #$24
   LDA   (FP),Y
   LDY   #$8B
   STA   (FP),Y
   LDY   #$25
   LDA   (FP),Y
   LDY   #$8C
   STA   (FP),Y
   LDY   #$26
   LDA   (FP),Y
   LDY   #$8D
   STA   (FP),Y
   LDY   #$27
   LDA   (FP),Y
   LDY   #$8E
   STA   (FP),Y
   LDY   #$28
   LDA   (FP),Y
   LDY   #$8F
   STA   (FP),Y
   LDY   #$29
   LDA   (FP),Y
   LDY   #$90
   STA   (FP),Y
.cond_end@66:
   CLC
   LDA   FP
   ADC   #$8A
   LDY   #$91
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
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$93
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$94
   STA   (FP),Y
   LDY   #$02
   LDA   (DPTR),Y
   LDY   #$95
   STA   (FP),Y
   LDY   #$03
   LDA   (DPTR),Y
   LDY   #$96
   STA   (FP),Y
   LDY   #$04
   LDA   (DPTR),Y
   LDY   #$97
   STA   (FP),Y
   LDY   #$93
   LDA   (FP),Y
   LDY   #$4C
   STA   (FP),Y
   LDY   #$94
   LDA   (FP),Y
   LDY   #$4D
   STA   (FP),Y
   LDY   #$95
   LDA   (FP),Y
   LDY   #$4E
   STA   (FP),Y
   LDY   #$96
   LDA   (FP),Y
   LDY   #$4F
   STA   (FP),Y
   LDY   #$97
   LDA   (FP),Y
   LDY   #$50
   STA   (FP),Y
   CLC
   LDA   FP
   ADC   #$4C
   LDY   #$98
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
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$9A
   STA   (FP),Y
   INY
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@140
   LDA   #$00
   JMP   .sx_done@141
.sx_neg@140:
   LDA   #$FF
.sx_done@141:
   LDY   #$9C
   STA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@142
   DEY
   LDA   (FP),Y
   CMP   #$7F
   BNE   .cmp_true@142
   LDA   #$00
   JMP   .cmp_end@143
.cmp_true@142:
   LDA   #$01
.cmp_end@143:
   LDY   #$9D
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@19
   JMP   .or_true@73
.lb_skip@19:
   CLC
   LDA   FP
   ADC   #$4C
   LDY   #$9F
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$01
   LDY   #$A1
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$A2
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$A3
   STA   (FP),Y
   INY
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@145
   LDA   #$00
   JMP   .sx_done@146
.sx_neg@145:
   LDA   #$FF
.sx_done@146:
   LDY   #$A5
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
   LDY   #$A5
   LDA   (FP),Y
   LDY   #$A7
   CMP   (FP),Y
   BNE   .cmp_true@147
   LDY   #$A4
   LDA   (FP),Y
   LDY   #$A6
   CMP   (FP),Y
   BNE   .cmp_true@147
   LDA   #$00
   JMP   .cmp_end@148
.cmp_true@147:
   LDA   #$01
.cmp_end@148:
   LDY   #$A8
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@73
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   JMP   .or_end@74
.or_true@73:
   LDA   #$01
   LDY   #$AA
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@74:
   LDY   #$AA
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@18
   JMP   .or_true@71
.lb_skip@18:
   CLC
   LDA   FP
   ADC   #$4C
   LDY   #$AC
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$02
   LDY   #$AE
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$AF
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   LDY   #$B0
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$B1
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$B2
   STA   (FP),Y
   INY
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@150
   LDA   #$00
   JMP   .sx_done@151
.sx_neg@150:
   LDA   #$FF
.sx_done@151:
   LDY   #$B4
   STA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@152
   DEY
   LDA   (FP),Y
   CMP   #$3D
   BNE   .cmp_true@152
   LDA   #$00
   JMP   .cmp_end@153
.cmp_true@152:
   LDA   #$01
.cmp_end@153:
   LDY   #$B5
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@71
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   JMP   .or_end@72
.or_true@71:
   LDA   #$01
   LDY   #$B7
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@72:
   LDY   #$B7
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@17
   JMP   .or_true@69
.lb_skip@17:
   CLC
   LDA   FP
   ADC   #$4C
   LDY   #$B9
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$02
   LDY   #$BB
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$BC
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$01
   LDY   #$BD
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$BE
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$BF
   STA   (FP),Y
   INY
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@155
   LDA   #$00
   JMP   .sx_done@156
.sx_neg@155:
   LDA   #$FF
.sx_done@156:
   LDY   #$C1
   STA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@157
   DEY
   LDA   (FP),Y
   CMP   #$3E
   BNE   .cmp_true@157
   LDA   #$00
   JMP   .cmp_end@158
.cmp_true@157:
   LDA   #$01
.cmp_end@158:
   LDY   #$C2
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@69
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   JMP   .or_end@70
.or_true@69:
   LDA   #$01
   LDY   #$C4
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@70:
   LDY   #$C4
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@16
   JMP   .or_true@67
.lb_skip@16:
   CLC
   LDA   FP
   ADC   #$4C
   LDY   #$C6
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$02
   LDY   #$C8
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$C9
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$02
   LDY   #$CA
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$CB
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$CC
   STA   (FP),Y
   INY
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@160
   LDA   #$00
   JMP   .sx_done@161
.sx_neg@160:
   LDA   #$FF
.sx_done@161:
   LDY   #$CE
   STA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@162
   DEY
   LDA   (FP),Y
   CMP   #$3F
   BNE   .cmp_true@162
   LDA   #$00
   JMP   .cmp_end@163
.cmp_true@162:
   LDA   #$01
.cmp_end@163:
   LDY   #$CF
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@67
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   JMP   .or_end@68
.or_true@67:
   LDA   #$01
   LDY   #$D1
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@68:
   LDY   #$D1
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@75
   LDX   #$00
   TXA

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$D4
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
.if_end@75:
   LDX   #$00
   LDA   #$01

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$D4
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$D3
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

test_copy_from_assignment:
   SUBROUTINE

   ; prologue: 0 arg bytes, 185 local bytes
   SEC
   LDA   SSP
   SBC   #$BB
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$BA
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   LDA   #$7F
   LDY   #$01
   STA   (FP),Y
   CLC
   LDA   FP
   ADC   #$02
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
   LDY   #$01
   LDA   (FP),Y
   DEY
   STA   (DPTR),Y
   SEC
   LDA   #$00
   SBC   #$80
   LDY   #$0B
   STA   (FP),Y
   LDA   #$00
   SBC   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$0D
   STA   (FP),Y
   LDY   #$09
   LDA   (FP),Y
   CLC
   ADC   #$01
   LDY   #$0E
   STA   (FP),Y
   LDY   #$0A
   LDA   (FP),Y
   ADC   #$00
   LDY   #$0F
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$0D
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDA   #$3D
   LDY   #$10
   STA   (FP),Y
   LDY   #$09
   LDA   (FP),Y
   CLC
   ADC   #$02
   LDY   #$11
   STA   (FP),Y
   LDY   #$0A
   LDA   (FP),Y
   ADC   #$00
   LDY   #$12
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$10
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDA   #$3E
   LDY   #$13
   STA   (FP),Y
   LDY   #$09
   LDA   (FP),Y
   CLC
   ADC   #$03
   LDY   #$14
   STA   (FP),Y
   LDY   #$0A
   LDA   (FP),Y
   ADC   #$00
   LDY   #$15
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$13
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDA   #$3F
   LDY   #$16
   STA   (FP),Y
   LDY   #$09
   LDA   (FP),Y
   CLC
   ADC   #$04
   LDY   #$17
   STA   (FP),Y
   LDY   #$0A
   LDA   (FP),Y
   ADC   #$00
   LDY   #$18
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$16
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   SEC
   LDA   #$00
   SBC   #$0A
   LDY   #$19
   STA   (FP),Y
   LDA   #$00
   SBC   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$1B
   STA   (FP),Y
   LDY   #$09
   LDA   (FP),Y
   CLC
   ADC   #$05
   LDY   #$1C
   STA   (FP),Y
   LDY   #$0A
   LDA   (FP),Y
   ADC   #$00
   LDY   #$1D
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$1B
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   SEC
   LDA   #$00
   SBC   #$0B
   LDY   #$1E
   STA   (FP),Y
   LDA   #$00
   SBC   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$20
   STA   (FP),Y
   LDY   #$09
   LDA   (FP),Y
   CLC
   ADC   #$06
   LDY   #$21
   STA   (FP),Y
   LDY   #$0A
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
   LDY   #$20
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$02
   LDA   (FP),Y
   STA   @16.big_struct2
   INY
   LDA   (FP),Y
   STA   @16.big_struct2+1
   INY
   LDA   (FP),Y
   STA   @16.big_struct2+2
   INY
   LDA   (FP),Y
   STA   @16.big_struct2+3
   INY
   LDA   (FP),Y
   STA   @16.big_struct2+4
   INY
   LDA   (FP),Y
   STA   @16.big_struct2+5
   INY
   LDA   (FP),Y
   STA   @16.big_struct2+6
   LDA   #<@16.big_struct2
   LDY   #$23
   STA   (FP),Y
   LDA   #>@16.big_struct2
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$25
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$26
   STA   (FP),Y
   LDY   #$02
   LDA   (DPTR),Y
   LDY   #$27
   STA   (FP),Y
   LDY   #$03
   LDA   (DPTR),Y
   LDY   #$28
   STA   (FP),Y
   LDY   #$04
   LDA   (DPTR),Y
   LDY   #$29
   STA   (FP),Y
   LDY   #$25
   LDA   (FP),Y
   STA   @17.small_struct
   INY
   LDA   (FP),Y
   STA   @17.small_struct+1
   INY
   LDA   (FP),Y
   STA   @17.small_struct+2
   INY
   LDA   (FP),Y
   STA   @17.small_struct+3
   INY
   LDA   (FP),Y
   STA   @17.small_struct+4
   LDA   #<@17.small_struct
   INY
   STA   (FP),Y
   LDA   #>@17.small_struct
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$2C
   STA   (FP),Y
   INY
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@165
   LDA   #$00
   JMP   .sx_done@166
.sx_neg@165:
   LDA   #$FF
.sx_done@166:
   LDY   #$2E
   STA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@167
   DEY
   LDA   (FP),Y
   CMP   #$7F
   BNE   .cmp_true@167
   LDA   #$00
   JMP   .cmp_end@168
.cmp_true@167:
   LDA   #$01
.cmp_end@168:
   LDY   #$2F
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@33
   JMP   .or_true@82
.lb_skip@33:
   LDA   #<@17.small_struct
   LDY   #$31
   STA   (FP),Y
   LDA   #>@17.small_struct
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$01
   LDY   #$33
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$34
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$35
   STA   (FP),Y
   INY
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@170
   LDA   #$00
   JMP   .sx_done@171
.sx_neg@170:
   LDA   #$FF
.sx_done@171:
   LDY   #$37
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
   LDY   #$37
   LDA   (FP),Y
   LDY   #$39
   CMP   (FP),Y
   BNE   .cmp_true@172
   LDY   #$36
   LDA   (FP),Y
   LDY   #$38
   CMP   (FP),Y
   BNE   .cmp_true@172
   LDA   #$00
   JMP   .cmp_end@173
.cmp_true@172:
   LDA   #$01
.cmp_end@173:
   LDY   #$3A
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@82
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   JMP   .or_end@83
.or_true@82:
   LDA   #$01
   LDY   #$3C
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@83:
   LDY   #$3C
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@32
   JMP   .or_true@80
.lb_skip@32:
   LDA   #<@17.small_struct
   LDY   #$3E
   STA   (FP),Y
   LDA   #>@17.small_struct
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$02
   LDY   #$40
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$41
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   LDY   #$42
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$43
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$44
   STA   (FP),Y
   INY
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@175
   LDA   #$00
   JMP   .sx_done@176
.sx_neg@175:
   LDA   #$FF
.sx_done@176:
   LDY   #$46
   STA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@177
   DEY
   LDA   (FP),Y
   CMP   #$3D
   BNE   .cmp_true@177
   LDA   #$00
   JMP   .cmp_end@178
.cmp_true@177:
   LDA   #$01
.cmp_end@178:
   LDY   #$47
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@80
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   JMP   .or_end@81
.or_true@80:
   LDA   #$01
   LDY   #$49
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@81:
   LDY   #$49
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@31
   JMP   .or_true@78
.lb_skip@31:
   LDA   #<@17.small_struct
   LDY   #$4B
   STA   (FP),Y
   LDA   #>@17.small_struct
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$02
   LDY   #$4D
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$4E
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$01
   LDY   #$4F
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$50
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$51
   STA   (FP),Y
   INY
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@180
   LDA   #$00
   JMP   .sx_done@181
.sx_neg@180:
   LDA   #$FF
.sx_done@181:
   LDY   #$53
   STA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@182
   DEY
   LDA   (FP),Y
   CMP   #$3E
   BNE   .cmp_true@182
   LDA   #$00
   JMP   .cmp_end@183
.cmp_true@182:
   LDA   #$01
.cmp_end@183:
   LDY   #$54
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@78
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   JMP   .or_end@79
.or_true@78:
   LDA   #$01
   LDY   #$56
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@79:
   LDY   #$56
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@30
   JMP   .or_true@76
.lb_skip@30:
   LDA   #<@17.small_struct
   LDY   #$58
   STA   (FP),Y
   LDA   #>@17.small_struct
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$02
   LDY   #$5A
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$5B
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$02
   LDY   #$5C
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$5D
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$5E
   STA   (FP),Y
   INY
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@185
   LDA   #$00
   JMP   .sx_done@186
.sx_neg@185:
   LDA   #$FF
.sx_done@186:
   LDY   #$60
   STA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@187
   DEY
   LDA   (FP),Y
   CMP   #$3F
   BNE   .cmp_true@187
   LDA   #$00
   JMP   .cmp_end@188
.cmp_true@187:
   LDA   #$01
.cmp_end@188:
   LDY   #$61
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@76
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   JMP   .or_end@77
.or_true@76:
   LDA   #$01
   LDY   #$63
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@77:
   LDY   #$63
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@84
   LDX   #$00
   TXA

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$BB
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$BA
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
.if_end@84:
   LDA   #<@16.big_struct2
   LDY   #$65
   STA   (FP),Y
   LDA   #>@16.big_struct2
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$67
   STA   (FP),Y
   INY
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@190
   LDA   #$00
   JMP   .sx_done@191
.sx_neg@190:
   LDA   #$FF
.sx_done@191:
   LDY   #$69
   STA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@192
   DEY
   LDA   (FP),Y
   CMP   #$7F
   BNE   .cmp_true@192
   LDA   #$00
   JMP   .cmp_end@193
.cmp_true@192:
   LDA   #$01
.cmp_end@193:
   LDY   #$6A
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@29
   JMP   .or_true@95
.lb_skip@29:
   LDA   #<@16.big_struct2
   LDY   #$6C
   STA   (FP),Y
   LDA   #>@16.big_struct2
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$01
   LDY   #$6E
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$6F
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$70
   STA   (FP),Y
   INY
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@195
   LDA   #$00
   JMP   .sx_done@196
.sx_neg@195:
   LDA   #$FF
.sx_done@196:
   LDY   #$72
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
   LDY   #$72
   LDA   (FP),Y
   LDY   #$74
   CMP   (FP),Y
   BNE   .cmp_true@197
   LDY   #$71
   LDA   (FP),Y
   LDY   #$73
   CMP   (FP),Y
   BNE   .cmp_true@197
   LDA   #$00
   JMP   .cmp_end@198
.cmp_true@197:
   LDA   #$01
.cmp_end@198:
   LDY   #$75
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@95
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   JMP   .or_end@96
.or_true@95:
   LDA   #$01
   LDY   #$77
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@96:
   LDY   #$77
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@28
   JMP   .or_true@93
.lb_skip@28:
   LDA   #<@16.big_struct2
   LDY   #$79
   STA   (FP),Y
   LDA   #>@16.big_struct2
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$02
   LDY   #$7B
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$7C
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   LDY   #$7D
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$7E
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$7F
   STA   (FP),Y
   INY
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@200
   LDA   #$00
   JMP   .sx_done@201
.sx_neg@200:
   LDA   #$FF
.sx_done@201:
   LDY   #$81
   STA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@202
   DEY
   LDA   (FP),Y
   CMP   #$3D
   BNE   .cmp_true@202
   LDA   #$00
   JMP   .cmp_end@203
.cmp_true@202:
   LDA   #$01
.cmp_end@203:
   LDY   #$82
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@93
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   JMP   .or_end@94
.or_true@93:
   LDA   #$01
   LDY   #$84
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@94:
   LDY   #$84
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@27
   JMP   .or_true@91
.lb_skip@27:
   LDA   #<@16.big_struct2
   LDY   #$86
   STA   (FP),Y
   LDA   #>@16.big_struct2
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$02
   LDY   #$88
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$89
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$01
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
   INY
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@205
   LDA   #$00
   JMP   .sx_done@206
.sx_neg@205:
   LDA   #$FF
.sx_done@206:
   LDY   #$8E
   STA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@207
   DEY
   LDA   (FP),Y
   CMP   #$3E
   BNE   .cmp_true@207
   LDA   #$00
   JMP   .cmp_end@208
.cmp_true@207:
   LDA   #$01
.cmp_end@208:
   LDY   #$8F
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@91
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   JMP   .or_end@92
.or_true@91:
   LDA   #$01
   LDY   #$91
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@92:
   LDY   #$91
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@26
   JMP   .or_true@89
.lb_skip@26:
   LDA   #<@16.big_struct2
   LDY   #$93
   STA   (FP),Y
   LDA   #>@16.big_struct2
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$02
   LDY   #$95
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$96
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$02
   LDY   #$97
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$98
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$99
   STA   (FP),Y
   INY
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@210
   LDA   #$00
   JMP   .sx_done@211
.sx_neg@210:
   LDA   #$FF
.sx_done@211:
   LDY   #$9B
   STA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@212
   DEY
   LDA   (FP),Y
   CMP   #$3F
   BNE   .cmp_true@212
   LDA   #$00
   JMP   .cmp_end@213
.cmp_true@212:
   LDA   #$01
.cmp_end@213:
   LDY   #$9C
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@89
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   JMP   .or_end@90
.or_true@89:
   LDA   #$01
   LDY   #$9E
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@90:
   LDY   #$9E
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@25
   JMP   .or_true@87
.lb_skip@25:
   LDA   #<@16.big_struct2
   LDY   #$A0
   STA   (FP),Y
   LDA   #>@16.big_struct2
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$05
   LDY   #$A2
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$A3
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$A4
   STA   (FP),Y
   INY
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@215
   LDA   #$00
   JMP   .sx_done@216
.sx_neg@215:
   LDA   #$FF
.sx_done@216:
   LDY   #$A6
   STA   (FP),Y
   SEC
   LDA   #$00
   SBC   #$0A
   INY
   STA   (FP),Y
   LDA   #$00
   SBC   #$00
   INY
   STA   (FP),Y
   LDY   #$A6
   LDA   (FP),Y
   LDY   #$A8
   CMP   (FP),Y
   BNE   .cmp_true@217
   LDY   #$A5
   LDA   (FP),Y
   LDY   #$A7
   CMP   (FP),Y
   BNE   .cmp_true@217
   LDA   #$00
   JMP   .cmp_end@218
.cmp_true@217:
   LDA   #$01
.cmp_end@218:
   LDY   #$A9
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@87
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   JMP   .or_end@88
.or_true@87:
   LDA   #$01
   LDY   #$AB
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@88:
   LDY   #$AB
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@24
   JMP   .or_true@85
.lb_skip@24:
   LDA   #<@16.big_struct2
   LDY   #$AD
   STA   (FP),Y
   LDA   #>@16.big_struct2
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$06
   LDY   #$AF
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$B0
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$B1
   STA   (FP),Y
   INY
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@220
   LDA   #$00
   JMP   .sx_done@221
.sx_neg@220:
   LDA   #$FF
.sx_done@221:
   LDY   #$B3
   STA   (FP),Y
   SEC
   LDA   #$00
   SBC   #$0B
   INY
   STA   (FP),Y
   LDA   #$00
   SBC   #$00
   INY
   STA   (FP),Y
   LDY   #$B3
   LDA   (FP),Y
   LDY   #$B5
   CMP   (FP),Y
   BNE   .cmp_true@222
   LDY   #$B2
   LDA   (FP),Y
   LDY   #$B4
   CMP   (FP),Y
   BNE   .cmp_true@222
   LDA   #$00
   JMP   .cmp_end@223
.cmp_true@222:
   LDA   #$01
.cmp_end@223:
   LDY   #$B6
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@85
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   JMP   .or_end@86
.or_true@85:
   LDA   #$01
   LDY   #$B8
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@86:
   LDY   #$B8
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@97
   LDX   #$00
   TXA

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$BB
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
.if_end@97:
   LDX   #$00
   LDA   #$01

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$BB
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$BA
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

   ; prologue: 0 arg bytes, 24 local bytes
   SEC
   LDA   SSP
   SBC   #$1A
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$19
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   JSR   test_copy_from_member
   LDY   #$01
   STA   (FP),Y
   TXA
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lnot_true@225
   LDA   #$00
   JMP   .lnot_end@226
.lnot_true@225:
   LDA   #$01
.lnot_end@226:
   LDY   #$03
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@98
   LDX   #$00
   LDA   #$01

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$1A
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$19
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
.if_end@98:
   JSR   test_copy_to_member
   LDY   #$05
   STA   (FP),Y
   TXA
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lnot_true@227
   LDA   #$00
   JMP   .lnot_end@228
.lnot_true@227:
   LDA   #$01
.lnot_end@228:
   LDY   #$07
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@99
   LDX   #$00
   LDA   #$02

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$1A
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$19
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
.if_end@99:
   JSR   test_copy_from_nested_member
   LDY   #$09
   STA   (FP),Y
   TXA
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lnot_true@229
   LDA   #$00
   JMP   .lnot_end@230
.lnot_true@229:
   LDA   #$01
.lnot_end@230:
   LDY   #$0B
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@100
   LDX   #$00
   LDA   #$03

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$1A
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$19
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
.if_end@100:
   JSR   test_copy_to_nested_member
   LDY   #$0D
   STA   (FP),Y
   TXA
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lnot_true@231
   LDA   #$00
   JMP   .lnot_end@232
.lnot_true@231:
   LDA   #$01
.lnot_end@232:
   LDY   #$0F
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@101
   LDX   #$00
   LDA   #$04

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$1A
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$19
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
.if_end@101:
   JSR   test_copy_from_conditional
   LDY   #$11
   STA   (FP),Y
   TXA
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lnot_true@233
   LDA   #$00
   JMP   .lnot_end@234
.lnot_true@233:
   LDA   #$01
.lnot_end@234:
   LDY   #$13
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@102
   LDX   #$00
   LDA   #$06

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$1A
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$19
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
.if_end@102:
   JSR   test_copy_from_assignment
   LDY   #$15
   STA   (FP),Y
   TXA
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lnot_true@235
   LDA   #$00
   JMP   .lnot_end@236
.lnot_true@235:
   LDA   #$01
.lnot_end@236:
   LDY   #$17
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@103
   LDX   #$00
   LDA   #$07

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$1A
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
.if_end@103:
   LDX   #$00
   TXA

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$1A
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$19
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

@0.big_struct:
   DC.B  $0A
   DC.B  $09
   DC.B  $08, $07, $06
   DC.B  $05
   DC.B  $04

@4.big_struct:
   DS.B  7

@7.small_struct:
   DS.B  5

@9.small_struct:
   DC.B  $32
   DC.B  $33
   DC.B  $34, $35, $36

@12.t:
   DC.W  $0001

@13.f:
   DS.B  2

@16.big_struct2:
   DS.B  7

@17.small_struct:
   DS.B  5
