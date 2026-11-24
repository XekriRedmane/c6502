SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24

check:
   SUBROUTINE

   ; prologue: 15 arg bytes, 219 local bytes
   SEC
   LDA   SSP
   SBC   #$DD
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$DC
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
   LDY   #$E0
   LDA   (FP),Y
   LDY   #$E9
   ORA   (FP),Y
   LDY   #$03
   STA   (FP),Y
   LDY   #$E1
   LDA   (FP),Y
   LDY   #$EA
   ORA   (FP),Y
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
   CMP   #$00
   BNE   .cmp_true@0
   DEY
   LDA   (FP),Y
   CMP   #$01
   BNE   .cmp_true@0
   LDA   #$00
   JMP   .cmp_end@1
.cmp_true@0:
   LDA   #$01
.cmp_end@1:
   LDY   #$07
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@0
   LDY   #$01
   LDA   (FP),Y
   ORA   #$01
   LDY   #$09
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   LDY   #$0A
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   LDY   #$0A
   LDA   (FP),Y
   LDY   #$02
   STA   (FP),Y
.if_end@0:
   LDY   #$DE
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@2
   JMP   .and_false@1
.lb_skip@2:
   LDY   #$E9
   LDA   (FP),Y
   LDY   #$0B
   STA   (FP),Y
   LDY   #$EA
   LDA   (FP),Y
   LDY   #$0C
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
   LDY   #$EB
   LDA   (FP),Y
   LDY   #$11
   STA   (FP),Y
   LDY   #$EC
   LDA   (FP),Y
   LDY   #$12
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
   LDY   #$11
   LDA   (FP),Y
   CLC
   LDY   #$E2
   ADC   (FP),Y
   LDY   #$17
   STA   (FP),Y
   LDY   #$12
   LDA   (FP),Y
   LDY   #$E3
   ADC   (FP),Y
   LDY   #$18
   STA   (FP),Y
   LDY   #$13
   LDA   (FP),Y
   LDY   #$E4
   ADC   (FP),Y
   LDY   #$19
   STA   (FP),Y
   LDY   #$14
   LDA   (FP),Y
   LDY   #$E5
   ADC   (FP),Y
   LDY   #$1A
   STA   (FP),Y
   LDY   #$15
   LDA   (FP),Y
   LDY   #$E6
   ADC   (FP),Y
   LDY   #$1B
   STA   (FP),Y
   LDY   #$16
   LDA   (FP),Y
   LDY   #$E7
   ADC   (FP),Y
   LDY   #$1C
   STA   (FP),Y
   LDY   #$0B
   LDA   (FP),Y
   CLC
   LDY   #$17
   ADC   (FP),Y
   LDY   #$1D
   STA   (FP),Y
   LDY   #$0C
   LDA   (FP),Y
   LDY   #$18
   ADC   (FP),Y
   LDY   #$1E
   STA   (FP),Y
   LDY   #$0D
   LDA   (FP),Y
   LDY   #$19
   ADC   (FP),Y
   LDY   #$1F
   STA   (FP),Y
   LDY   #$0E
   LDA   (FP),Y
   LDY   #$1A
   ADC   (FP),Y
   LDY   #$20
   STA   (FP),Y
   LDY   #$0F
   LDA   (FP),Y
   LDY   #$1B
   ADC   (FP),Y
   LDY   #$21
   STA   (FP),Y
   LDY   #$10
   LDA   (FP),Y
   LDY   #$1C
   ADC   (FP),Y
   LDY   #$22
   STA   (FP),Y
   LDY   #$1D
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
   BEQ   .and_false@1
   LDA   #$01
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   JMP   .and_end@2
.and_false@1:
   LDA   #$00
   LDY   #$23
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@2:
   LDY   #$23
   LDA   (FP),Y
   LDY   #$25
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$26
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@3
   LDA   #$00
   JMP   .cmp_end@4
.cmp_true@3:
   LDA   #$01
.cmp_end@4:
   LDY   #$27
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@3
   LDY   #$01
   LDA   (FP),Y
   ORA   #$02
   LDY   #$29
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   LDY   #$2A
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   LDY   #$2A
   LDA   (FP),Y
   LDY   #$02
   STA   (FP),Y
.if_end@3:
   LDY   #$E8
   LDA   (FP),Y
   BEQ   .lnot_true@5
   LDA   #$00
   JMP   .lnot_end@6
.lnot_true@5:
   LDA   #$01
.lnot_end@6:
   LDY   #$2B
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDA   #$FF
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$2C
   LDA   (FP),Y
   LDY   #$2E
   CMP   (FP),Y
   BNE   .thread@0
   LDY   #$2B
   LDA   (FP),Y
   LDY   #$2D
   CMP   (FP),Y
   BEQ   .cmp_true@7
.thread@0:
   LDA   #$00
   JMP   .cmp_end@8
.cmp_true@7:
   LDA   #$01
.cmp_end@8:
   LDY   #$2F
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$31
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$32
   STA   (FP),Y
   LDY   #$E0
   LDA   (FP),Y
   TAX
   BEQ   .shift_done@11
.shift_loop@10:
   LDY   #$31
   LDA   (FP),Y
   ASL
   STA   (FP),Y
   INY
   LDA   (FP),Y
   ROL
   STA   (FP),Y
   DEX
   BNE   .shift_loop@10
.shift_done@11:
   LDY   #$31
   LDA   (FP),Y
   LDY   #$33
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$34
   STA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@12
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@12
   LDA   #$00
   JMP   .cmp_end@13
.cmp_true@12:
   LDA   #$01
.cmp_end@13:
   LDY   #$35
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@1
   JMP   .or_true@5
.lb_skip@1:
   LDY   #$01
   LDA   (FP),Y
   ORA   #$04
   LDY   #$37
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   LDY   #$38
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   LDY   #$38
   LDA   (FP),Y
   LDY   #$02
   STA   (FP),Y
   LDA   #$FF
   BEQ   .lb_skip@0
   JMP   .or_true@5
.lb_skip@0:
   LDY   #$E8
   LDA   (FP),Y
   BEQ   .and_false@7
   LDY   #$DE
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@7
   LDA   #$01
   LDY   #$39
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   JMP   .and_end@8
.and_false@7:
   LDA   #$00
   LDY   #$39
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@8:
   LDY   #$39
   LDA   (FP),Y
   LDY   #$3B
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$3C
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@15
   LDA   #$00
   JMP   .sx_done@16
.sx_neg@15:
   LDA   #$FF
.sx_done@16:
   LDY   #$3D
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
   LDY   #$3B
   LDA   (FP),Y
   LDY   #$43
   STA   (FP),Y
   LDY   #$3C
   LDA   (FP),Y
   LDY   #$44
   STA   (FP),Y
   LDY   #$3D
   LDA   (FP),Y
   LDY   #$45
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   AND   #$0F
   INY
   STA   (FP),Y
   LDY   #$43
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
   BNE   .or_true@5
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   JMP   .or_end@6
.or_true@5:
   LDA   #$01
   LDY   #$4B
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@6:
   LDY   #$4B
   LDA   (FP),Y
   LDY   #$4D
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$4E
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CMP   #$01
   BNE   .cmp_true@17
   LDA   #$00
   JMP   .cmp_end@18
.cmp_true@17:
   LDA   #$01
.cmp_end@18:
   LDY   #$4F
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@9
   LDY   #$01
   LDA   (FP),Y
   ORA   #$08
   LDY   #$51
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   LDY   #$52
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   LDY   #$52
   LDA   (FP),Y
   LDY   #$02
   STA   (FP),Y
.if_end@9:
   LDY   #$E0
   LDA   (FP),Y
   SEC
   LDY   #$EB
   SBC   (FP),Y
   LDY   #$E1
   LDA   (FP),Y
   LDY   #$EC
   SBC   (FP),Y
   LDA   #$00
   ROL
   LDY   #$53
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDA   #$0E
   SEC
   LDY   #$E0
   SBC   (FP),Y
   LDA   #$0C
   INY
   SBC   (FP),Y
   LDA   #$00
   ROL
   LDY   #$55
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$53
   LDA   (FP),Y
   LDY   #$55
   EOR   (FP),Y
   LDY   #$57
   STA   (FP),Y
   LDY   #$54
   LDA   (FP),Y
   LDY   #$56
   EOR   (FP),Y
   LDY   #$58
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$59
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$5A
   STA   (FP),Y
   LDA   #$07
   SEC
   DEY
   SBC   (FP),Y
   LDY   #$5B
   STA   (FP),Y
   LDA   #$00
   DEY
   SBC   (FP),Y
   LDY   #$5C
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$5D
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$5E
   STA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@19
   DEY
   LDA   (FP),Y
   CMP   #$07
   BNE   .cmp_true@19
   LDA   #$00
   JMP   .cmp_end@20
.cmp_true@19:
   LDA   #$01
.cmp_end@20:
   LDY   #$5F
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@10
   LDY   #$01
   LDA   (FP),Y
   ORA   #$10
   LDY   #$61
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   LDY   #$62
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   LDY   #$62
   LDA   (FP),Y
   LDY   #$02
   STA   (FP),Y
.if_end@10:
   LDY   #$E8
   LDA   (FP),Y
   LDY   #$63
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$65
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$66
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   LDY   #$E9
   SBC   (FP),Y
   LDY   #$66
   LDA   (FP),Y
   LDY   #$EA
   SBC   (FP),Y
   LDA   #$00
   ROL
   LDY   #$67
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$DE
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lnot_true@22
   LDA   #$00
   JMP   .lnot_end@23
.lnot_true@22:
   LDA   #$01
.lnot_end@23:
   LDY   #$69
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$67
   LDA   (FP),Y
   SEC
   LDY   #$69
   SBC   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$6A
   SBC   (FP),Y
   BVC   .cmp_novf@24
   EOR   #$80
.cmp_novf@24:
   ASL
   LDA   #$00
   ROL
   EOR   #$01
   LDY   #$6B
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$6D
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$6E
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@25
   LDA   #$00
   JMP   .sx_done@26
.sx_neg@25:
   LDA   #$FF
.sx_done@26:
   LDY   #$6F
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$6D
   LDA   (FP),Y
   LDY   #$E2
   EOR   (FP),Y
   LDY   #$73
   STA   (FP),Y
   LDY   #$6E
   LDA   (FP),Y
   LDY   #$E3
   EOR   (FP),Y
   LDY   #$74
   STA   (FP),Y
   LDY   #$6F
   LDA   (FP),Y
   LDY   #$E4
   EOR   (FP),Y
   LDY   #$75
   STA   (FP),Y
   LDY   #$70
   LDA   (FP),Y
   LDY   #$E5
   EOR   (FP),Y
   LDY   #$76
   STA   (FP),Y
   LDY   #$71
   LDA   (FP),Y
   LDY   #$E6
   EOR   (FP),Y
   LDY   #$77
   STA   (FP),Y
   LDY   #$72
   LDA   (FP),Y
   LDY   #$E7
   EOR   (FP),Y
   LDY   #$78
   STA   (FP),Y
   LDY   #$73
   LDA   (FP),Y
   LDY   #$79
   STA   (FP),Y
   LDY   #$74
   LDA   (FP),Y
   LDY   #$7A
   STA   (FP),Y
   LDY   #$75
   LDA   (FP),Y
   LDY   #$7B
   STA   (FP),Y
   LDY   #$76
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
   CMP   #$80
   BNE   .cmp_true@27
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@27
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@27
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@27
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@27
   DEY
   LDA   (FP),Y
   CMP   #$01
   BNE   .cmp_true@27
   LDA   #$00
   JMP   .cmp_end@28
.cmp_true@27:
   LDA   #$01
.cmp_end@28:
   LDY   #$7F
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@11
   LDY   #$01
   LDA   (FP),Y
   ORA   #$20
   LDY   #$81
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   LDY   #$82
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   LDY   #$82
   LDA   (FP),Y
   LDY   #$02
   STA   (FP),Y
.if_end@11:
   LDA   #$00
   LDY   #$83
   STA   (FP),Y
   INY
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@30
   LDA   #$00
   JMP   .sx_done@31
.sx_neg@30:
   LDA   #$FF
.sx_done@31:
   LDY   #$85
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$86
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$87
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   LDY   #$88
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$89
   STA   (FP),Y
   LDY   #$E9
   LDA   (FP),Y
   CLC
   LDY   #$E0
   SBC   (FP),Y
   LDY   #$EA
   LDA   (FP),Y
   LDY   #$E1
   SBC   (FP),Y
   LDA   #$00
   ROL
   LDY   #$8A
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$8C
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$8D
   STA   (FP),Y
   LDY   #$88
   LDA   (FP),Y
   SEC
   LDY   #$8C
   SBC   (FP),Y
   LDY   #$89
   LDA   (FP),Y
   LDY   #$8D
   SBC   (FP),Y
   LDA   #$00
   ROL
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$90
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$91
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@32
   LDA   #$00
   JMP   .sx_done@33
.sx_neg@32:
   LDA   #$FF
.sx_done@33:
   LDY   #$92
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$E2
   LDA   (FP),Y
   LDY   #$90
   AND   (FP),Y
   LDY   #$96
   STA   (FP),Y
   LDY   #$E3
   LDA   (FP),Y
   LDY   #$91
   AND   (FP),Y
   LDY   #$97
   STA   (FP),Y
   LDY   #$E4
   LDA   (FP),Y
   LDY   #$92
   AND   (FP),Y
   LDY   #$98
   STA   (FP),Y
   LDY   #$E5
   LDA   (FP),Y
   LDY   #$93
   AND   (FP),Y
   LDY   #$99
   STA   (FP),Y
   LDY   #$E6
   LDA   (FP),Y
   LDY   #$94
   AND   (FP),Y
   LDY   #$9A
   STA   (FP),Y
   LDY   #$E7
   LDA   (FP),Y
   LDY   #$95
   AND   (FP),Y
   LDY   #$9B
   STA   (FP),Y
   LDY   #$96
   LDA   (FP),Y
   LDY   #$9C
   STA   (FP),Y
   LDY   #$97
   LDA   (FP),Y
   LDY   #$9D
   STA   (FP),Y
   LDY   #$98
   LDA   (FP),Y
   LDY   #$9E
   STA   (FP),Y
   LDY   #$99
   LDA   (FP),Y
   LDY   #$9F
   STA   (FP),Y
   LDY   #$9A
   LDA   (FP),Y
   LDY   #$A0
   STA   (FP),Y
   LDY   #$9B
   LDA   (FP),Y
   LDY   #$A1
   STA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@34
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@34
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@34
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@34
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@34
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@34
   LDA   #$00
   JMP   .cmp_end@35
.cmp_true@34:
   LDA   #$01
.cmp_end@35:
   LDY   #$A2
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@12
   LDY   #$01
   LDA   (FP),Y
   ORA   #$40
   LDY   #$A4
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   LDY   #$A5
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   LDY   #$A5
   LDA   (FP),Y
   LDY   #$02
   STA   (FP),Y
.if_end@12:
   LDY   #$DE
   LDA   (FP),Y
   LDY   #$A6
   STA   (FP),Y
   LDY   #$DF
   LDA   (FP),Y
   LDY   #$A7
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
   LDA   #$59
   CLC
   LDY   #$A6
   SBC   (FP),Y
   LDA   #$89
   INY
   SBC   (FP),Y
   LDA   #$2E
   INY
   SBC   (FP),Y
   LDA   #$7F
   INY
   SBC   (FP),Y
   LDA   #$5D
   INY
   SBC   (FP),Y
   LDA   #$13
   INY
   SBC   (FP),Y
   LDA   #$29
   INY
   SBC   (FP),Y
   LDA   #$0B
   INY
   SBC   (FP),Y
   LDA   #$00
   ROL
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$B0
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$B1
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@37
   LDA   #$00
   JMP   .sx_done@38
.sx_neg@37:
   LDA   #$FF
.sx_done@38:
   LDY   #$B2
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
   LDY   #$B0
   LDA   (FP),Y
   LDY   #$B8
   STA   (FP),Y
   LDY   #$B1
   LDA   (FP),Y
   LDY   #$B9
   STA   (FP),Y
   LDY   #$B2
   LDA   (FP),Y
   LDY   #$BA
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   AND   #$0F
   INY
   STA   (FP),Y
   LDA   #$C0
   CLC
   LDY   #$B8
   ADC   (FP),Y
   LDY   #$C0
   STA   (FP),Y
   LDA   #$27
   LDY   #$B9
   ADC   (FP),Y
   LDY   #$C1
   STA   (FP),Y
   LDA   #$AD
   LDY   #$BA
   ADC   (FP),Y
   LDY   #$C2
   STA   (FP),Y
   LDA   #$9B
   LDY   #$BB
   ADC   (FP),Y
   LDY   #$C3
   STA   (FP),Y
   LDA   #$69
   LDY   #$BC
   ADC   (FP),Y
   LDY   #$C4
   STA   (FP),Y
   LDA   #$90
   LDY   #$BD
   ADC   (FP),Y
   LDY   #$C5
   STA   (FP),Y
   LDA   #$1B
   LDY   #$BE
   ADC   (FP),Y
   LDY   #$C6
   STA   (FP),Y
   LDA   #$00
   LDY   #$BF
   ADC   (FP),Y
   LDY   #$C7
   STA   (FP),Y
   LDY   #$C0
   LDA   (FP),Y
   LDY   #$C8
   STA   (FP),Y
   LDY   #$C1
   LDA   (FP),Y
   LDY   #$C9
   STA   (FP),Y
   LDY   #$C2
   LDA   (FP),Y
   LDY   #$CA
   STA   (FP),Y
   LDY   #$C3
   LDA   (FP),Y
   LDY   #$CB
   STA   (FP),Y
   LDY   #$C4
   LDA   (FP),Y
   LDY   #$CC
   STA   (FP),Y
   LDY   #$C5
   LDA   (FP),Y
   LDY   #$CD
   STA   (FP),Y
   LDY   #$C6
   LDA   (FP),Y
   LDY   #$CE
   STA   (FP),Y
   LDY   #$C7
   LDA   (FP),Y
   AND   #$0F
   LDY   #$CF
   STA   (FP),Y
   LDY   #$C8
   LDA   (FP),Y
   LDY   #$D0
   STA   (FP),Y
   LDY   #$C9
   LDA   (FP),Y
   LDY   #$D1
   STA   (FP),Y
   LDY   #$CA
   LDA   (FP),Y
   LDY   #$D2
   STA   (FP),Y
   LDY   #$CB
   LDA   (FP),Y
   LDY   #$D3
   STA   (FP),Y
   LDY   #$CC
   LDA   (FP),Y
   LDY   #$D4
   STA   (FP),Y
   LDY   #$CD
   LDA   (FP),Y
   LDY   #$D5
   STA   (FP),Y
   LDY   #$CE
   LDA   (FP),Y
   LDY   #$D6
   STA   (FP),Y
   LDY   #$CF
   LDA   (FP),Y
   LDY   #$D7
   STA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@39
   DEY
   LDA   (FP),Y
   CMP   #$1B
   BNE   .cmp_true@39
   DEY
   LDA   (FP),Y
   CMP   #$90
   BNE   .cmp_true@39
   DEY
   LDA   (FP),Y
   CMP   #$69
   BNE   .cmp_true@39
   DEY
   LDA   (FP),Y
   CMP   #$9B
   BNE   .cmp_true@39
   DEY
   LDA   (FP),Y
   CMP   #$AD
   BNE   .cmp_true@39
   DEY
   LDA   (FP),Y
   CMP   #$27
   BNE   .cmp_true@39
   DEY
   LDA   (FP),Y
   CMP   #$C1
   BNE   .cmp_true@39
   LDA   #$00
   JMP   .cmp_end@40
.cmp_true@39:
   LDA   #$01
.cmp_end@40:
   LDY   #$D8
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@13
   LDY   #$01
   LDA   (FP),Y
   ORA   #$80
   LDY   #$DA
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   LDY   #$DB
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   LDY   #$DB
   LDA   (FP),Y
   LDY   #$02
   STA   (FP),Y
.if_end@13:
   LDY   #$02
   LDA   (FP),Y
   TAX
   DEY
   LDA   (FP),Y

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$EC
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$DC
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

   ; prologue: 0 arg bytes, 3 local bytes
   SEC
   LDA   SSP
   SBC   #$05
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$04
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   LDA   #$80
   LDY   #$01
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$0F
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   #$00
   STA   (SSP),Y
   INY
   STA   (SSP),Y
   LDA   #$01
   INY
   STA   (SSP),Y
   LDA   #$00
   INY
   STA   (SSP),Y
   INY
   STA   (SSP),Y
   INY
   STA   (SSP),Y
   INY
   STA   (SSP),Y
   INY
   STA   (SSP),Y
   INY
   STA   (SSP),Y
   LDA   #$80
   INY
   STA   (SSP),Y
   LDY   #$01
   LDA   (FP),Y
   LDY   #$0B
   STA   (SSP),Y
   LDA   #$01
   INY
   STA   (SSP),Y
   LDA   #$00
   INY
   STA   (SSP),Y
   LDA   #$01
   INY
   STA   (SSP),Y
   LDA   #$00
   INY
   STA   (SSP),Y
   JSR   check
   LDY   #$02
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
   ADC   #$05
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$04
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
