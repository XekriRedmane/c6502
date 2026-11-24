SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24

check:
   SUBROUTINE

   ; prologue: 12 arg bytes, 189 local bytes
   SEC
   LDA   SSP
   SBC   #$BF
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$BE
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
   LDY   #$C3
   LDA   (FP),Y
   LDY   #$03
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$C0
   LDA   (FP),Y
   LDY   #$05
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@0
   LDA   #$00
   JMP   .sx_done@1
.sx_neg@0:
   LDA   #$FF
.sx_done@1:
   LDY   #$06
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   EOR   #$FF
   LDY   #$07
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   EOR   #$FF
   LDY   #$08
   STA   (FP),Y
   LDY   #$03
   LDA   (FP),Y
   STA   HARGS
   INY
   LDA   (FP),Y
   STA   HARGS+1
   LDY   #$07
   LDA   (FP),Y
   STA   HARGS+2
   INY
   LDA   (FP),Y
   STA   HARGS+3
   JSR   mul16
   LDA   HARGS+4
   LDY   #$09
   STA   (FP),Y
   LDA   HARGS+5
   INY
   STA   (FP),Y
   LDA   #$FF
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$09
   LDA   (FP),Y
   STA   HARGS
   INY
   LDA   (FP),Y
   STA   HARGS+1
   LDY   #$0C
   LDA   (FP),Y
   STA   HARGS+2
   INY
   LDA   (FP),Y
   STA   HARGS+3
   JSR   mul16
   LDA   HARGS+4
   LDY   #$0E
   STA   (FP),Y
   LDA   HARGS+5
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$10
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$11
   STA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@2
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@2
   LDA   #$00
   JMP   .cmp_end@3
.cmp_true@2:
   LDA   #$01
.cmp_end@3:
   LDY   #$12
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .or_true@3
   LDY   #$01
   LDA   (FP),Y
   ORA   #$01
   LDY   #$14
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   LDY   #$15
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   LDY   #$15
   LDA   (FP),Y
   LDY   #$02
   STA   (FP),Y
   LDA   #$80
   BNE   .or_true@3
   LDA   #$02
   LDY   #$16
   STA   (FP),Y
   ORA   #$00
   BNE   .or_true@3
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   JMP   .or_end@4
.or_true@3:
   LDA   #$01
   LDY   #$17
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@4:
   LDY   #$17
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@1
   LDY   #$C1
   LDA   (FP),Y
   LDY   #$19
   STA   (FP),Y
   LDY   #$C2
   LDA   (FP),Y
   LDY   #$1A
   STA   (FP),Y
   LDY   #$C4
   LDA   (FP),Y
   TAX
   BEQ   .shift_done@6
.shift_loop@5:
   LDY   #$19
   LDA   (FP),Y
   ASL
   STA   (FP),Y
   INY
   LDA   (FP),Y
   ROL
   STA   (FP),Y
   DEX
   BNE   .shift_loop@5
.shift_done@6:
   LDY   #$19
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@1
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   JMP   .or_end@2
.or_true@1:
   LDA   #$01
   LDY   #$1B
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@2:
   LDY   #$1B
   LDA   (FP),Y
   LDY   #$1D
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$1E
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@7
   LDA   #$00
   JMP   .sx_done@8
.sx_neg@7:
   LDA   #$FF
.sx_done@8:
   LDY   #$1F
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$C1
   LDA   (FP),Y
   LDY   #$21
   STA   (FP),Y
   LDY   #$C2
   LDA   (FP),Y
   LDY   #$22
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   STA   HARGS
   LDY   #$00
   STY   HARGS+1
   STA   HARGS+2
   LDA   #$80
   STA   HARGS+3
   LDY   #$21
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
   JSR   udivmod32
   LDA   HARGS+8
   LDY   #$25
   STA   (FP),Y
   LDA   HARGS+9
   INY
   STA   (FP),Y
   LDA   HARGS+10
   INY
   STA   (FP),Y
   LDA   HARGS+11
   INY
   STA   (FP),Y
   LDY   #$C0
   LDA   (FP),Y
   LDY   #$29
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@9
   LDA   #$00
   JMP   .sx_done@10
.sx_neg@9:
   LDA   #$FF
.sx_done@10:
   LDY   #$2A
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$2B
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$2C
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@11
   LDA   #$00
   JMP   .sx_done@12
.sx_neg@11:
   LDA   #$FF
.sx_done@12:
   LDY   #$2D
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$25
   LDA   (FP),Y
   SEC
   LDY   #$2B
   SBC   (FP),Y
   LDY   #$2F
   STA   (FP),Y
   LDY   #$26
   LDA   (FP),Y
   LDY   #$2C
   SBC   (FP),Y
   LDY   #$30
   STA   (FP),Y
   LDY   #$27
   LDA   (FP),Y
   LDY   #$2D
   SBC   (FP),Y
   LDY   #$31
   STA   (FP),Y
   LDY   #$28
   LDA   (FP),Y
   LDY   #$2E
   SBC   (FP),Y
   LDY   #$32
   STA   (FP),Y
   LDY   #$1D
   LDA   (FP),Y
   LDY   #$2F
   ORA   (FP),Y
   LDY   #$33
   STA   (FP),Y
   LDY   #$1E
   LDA   (FP),Y
   LDY   #$30
   ORA   (FP),Y
   LDY   #$34
   STA   (FP),Y
   LDY   #$1F
   LDA   (FP),Y
   LDY   #$31
   ORA   (FP),Y
   LDY   #$35
   STA   (FP),Y
   LDY   #$20
   LDA   (FP),Y
   LDY   #$32
   ORA   (FP),Y
   LDY   #$36
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
   CMP   #$00
   BNE   .cmp_true@13
   DEY
   LDA   (FP),Y
   CMP   #$03
   BNE   .cmp_true@13
   DEY
   LDA   (FP),Y
   CMP   #$AA
   BNE   .cmp_true@13
   DEY
   LDA   (FP),Y
   CMP   #$BF
   BNE   .cmp_true@13
   LDA   #$00
   JMP   .cmp_end@14
.cmp_true@13:
   LDA   #$01
.cmp_end@14:
   LDY   #$3B
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@5
   LDY   #$01
   LDA   (FP),Y
   ORA   #$02
   LDY   #$3D
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   LDY   #$3E
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   LDY   #$3E
   LDA   (FP),Y
   LDY   #$02
   STA   (FP),Y
.if_end@5:
   LDA   #$00
   LDY   #$3F
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
   LDY   #$41
   STA   (FP),Y
   LDY   #$C0
   LDA   (FP),Y
   LDY   #$42
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@18
   LDA   #$00
   JMP   .sx_done@19
.sx_neg@18:
   LDA   #$FF
.sx_done@19:
   LDY   #$43
   STA   (FP),Y
   LDY   #$40
   LDA   (FP),Y
   LDY   #$42
   ORA   (FP),Y
   LDY   #$44
   STA   (FP),Y
   LDY   #$41
   LDA   (FP),Y
   LDY   #$43
   ORA   (FP),Y
   LDY   #$45
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@6
   LDY   #$CA
   LDA   (FP),Y
   LDY   #$46
   STA   (FP),Y
   LDY   #$CB
   LDA   (FP),Y
   LDY   #$47
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$46
   LDA   (FP),Y
   SEC
   SBC   #$80
   LDY   #$4A
   STA   (FP),Y
   LDY   #$47
   LDA   (FP),Y
   SBC   #$00
   LDY   #$4B
   STA   (FP),Y
   LDY   #$48
   LDA   (FP),Y
   SBC   #$00
   LDY   #$4C
   STA   (FP),Y
   LDY   #$49
   LDA   (FP),Y
   SBC   #$00
   LDY   #$4D
   STA   (FP),Y
   LDY   #$4A
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   INY
   ORA   (FP),Y
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
   LDY   #$4E
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@7:
   LDY   #$4E
   LDA   (FP),Y
   EOR   #$FF
   LDY   #$50
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   EOR   #$FF
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
   LDA   #$FE
   INY
   STA   (FP),Y
   LDA   #$FF
   INY
   STA   (FP),Y
   LDY   #$53
   LDA   (FP),Y
   LDY   #$55
   CMP   (FP),Y
   BNE   .cmp_true@20
   LDY   #$52
   LDA   (FP),Y
   LDY   #$54
   CMP   (FP),Y
   BNE   .cmp_true@20
   LDA   #$00
   JMP   .cmp_end@21
.cmp_true@20:
   LDA   #$01
.cmp_end@21:
   LDY   #$56
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@8
   LDY   #$01
   LDA   (FP),Y
   ORA   #$04
   LDY   #$58
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   LDY   #$59
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   LDY   #$59
   LDA   (FP),Y
   LDY   #$02
   STA   (FP),Y
.if_end@8:
   LDY   #$C1
   LDA   (FP),Y
   LDY   #$5A
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@23
   LDA   #$00
   JMP   .sx_done@24
.sx_neg@23:
   LDA   #$FF
.sx_done@24:
   LDY   #$5D
   STA   (FP),Y
   LDA   #$E8
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@25
   LDA   #$00
   JMP   .sx_done@26
.sx_neg@25:
   LDA   #$FF
.sx_done@26:
   LDY   #$60
   STA   (FP),Y
   LDY   #$5D
   LDA   (FP),Y
   LDY   #$60
   CMP   (FP),Y
   BNE   .cmp_true@27
   LDY   #$5C
   LDA   (FP),Y
   LDY   #$5F
   CMP   (FP),Y
   BNE   .cmp_true@27
   LDA   #$00
   JMP   .cmp_end@28
.cmp_true@27:
   LDA   #$01
.cmp_end@28:
   LDY   #$61
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
   LDY   #$63
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   LDY   #$64
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   LDY   #$64
   LDA   (FP),Y
   LDY   #$02
   STA   (FP),Y
.if_end@9:
   LDA   #$07
   LDY   #$65
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$68
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$69
   STA   (FP),Y
   LDY   #$C8
   LDA   (FP),Y
   STA   HARGS
   INY
   LDA   (FP),Y
   STA   HARGS+1
   LDY   #$68
   LDA   (FP),Y
   STA   HARGS+2
   INY
   LDA   (FP),Y
   STA   HARGS+3
   JSR   udivmod16
   LDA   HARGS+6
   LDY   #$6A
   STA   (FP),Y
   LDA   HARGS+7
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@10
   LDA   #$00
   BEQ   .and_false@10
   LDA   #$01
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   JMP   .and_end@11
.and_false@10:
   LDA   #$00
   LDY   #$6C
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@11:
   LDY   #$6C
   LDA   (FP),Y
   LDY   #$6E
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$6F
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@30
   LDA   #$00
   JMP   .sx_done@31
.sx_neg@30:
   LDA   #$FF
.sx_done@31:
   LDY   #$70
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$C4
   LDA   (FP),Y
   SEC
   LDY   #$6E
   SBC   (FP),Y
   LDY   #$C5
   LDA   (FP),Y
   LDY   #$6F
   SBC   (FP),Y
   LDY   #$C6
   LDA   (FP),Y
   LDY   #$70
   SBC   (FP),Y
   LDY   #$C7
   LDA   (FP),Y
   LDY   #$71
   SBC   (FP),Y
   BVC   .cmp_novf@32
   EOR   #$80
.cmp_novf@32:
   ASL
   LDA   #$00
   ROL
   LDY   #$72
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$74
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$75
   STA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@33
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@33
   LDA   #$00
   JMP   .cmp_end@34
.cmp_true@33:
   LDA   #$01
.cmp_end@34:
   LDY   #$76
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
   ORA   #$10
   LDY   #$78
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   LDY   #$79
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   LDY   #$79
   LDA   (FP),Y
   LDY   #$02
   STA   (FP),Y
.if_end@12:
   LDY   #$C1
   LDA   (FP),Y
   LDY   #$7A
   STA   (FP),Y
   LDY   #$C2
   LDA   (FP),Y
   LDY   #$7B
   STA   (FP),Y
   CMP   #$22
   BNE   .cmp_true@36
   DEY
   LDA   (FP),Y
   CMP   #$E8
   BNE   .cmp_true@36
   LDA   #$00
   JMP   .cmp_end@37
.cmp_true@36:
   LDA   #$01
.cmp_end@37:
   LDY   #$7C
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
   ORA   #$20
   LDY   #$7E
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   LDY   #$7F
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   LDY   #$7F
   LDA   (FP),Y
   LDY   #$02
   STA   (FP),Y
.if_end@13:
   LDY   #$C3
   LDA   (FP),Y
   LDY   #$80
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$82
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$83
   STA   (FP),Y
   LDY   #$C2
   CMP   (FP),Y
   BNE   .thread@0
   LDY   #$82
   LDA   (FP),Y
   LDY   #$C1
   CMP   (FP),Y
   BEQ   .cmp_true@39
.thread@0:
   LDA   #$00
   JMP   .cmp_end@40
.cmp_true@39:
   LDA   #$01
.cmp_end@40:
   LDY   #$84
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   INY
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
   LDY   #$88
   STA   (FP),Y
   LDY   #$84
   LDA   (FP),Y
   LDY   #$87
   ORA   (FP),Y
   LDY   #$89
   STA   (FP),Y
   LDY   #$85
   LDA   (FP),Y
   LDY   #$88
   ORA   (FP),Y
   LDY   #$8A
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$8B
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$8C
   STA   (FP),Y
   LDY   #$C9
   CMP   (FP),Y
   BNE   .cmp_true@44
   LDY   #$8B
   LDA   (FP),Y
   LDY   #$C8
   CMP   (FP),Y
   BNE   .cmp_true@44
   LDA   #$00
   JMP   .cmp_end@45
.cmp_true@44:
   LDA   #$01
.cmp_end@45:
   LDY   #$8D
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$8F
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$90
   STA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@47
   DEY
   LDA   (FP),Y
   CMP   #$01
   BNE   .cmp_true@47
   LDA   #$00
   JMP   .cmp_end@48
.cmp_true@47:
   LDA   #$01
.cmp_end@48:
   LDY   #$91
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@14
   LDY   #$01
   LDA   (FP),Y
   ORA   #$40
   LDY   #$93
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   LDY   #$94
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   LDY   #$94
   LDA   (FP),Y
   LDY   #$02
   STA   (FP),Y
.if_end@14:
   LDY   #$C3
   LDA   (FP),Y
   LDY   #$95
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$97
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$98
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@50
   LDA   #$00
   JMP   .sx_done@51
.sx_neg@50:
   LDA   #$FF
.sx_done@51:
   LDY   #$99
   STA   (FP),Y
   INY
   STA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@52
   LDY   #$98
   LDA   (FP),Y
   BNE   .cmp_true@52
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@52
   LDA   #$00
   JMP   .cmp_end@53
.cmp_true@52:
   LDA   #$01
.cmp_end@53:
   LDY   #$9B
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDA   #$02
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDA   #$80
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@55
   LDA   #$00
   JMP   .sx_done@56
.sx_neg@55:
   LDA   #$FF
.sx_done@56:
   LDY   #$A2
   STA   (FP),Y
   LDY   #$9E
   LDA   (FP),Y
   SEC
   LDY   #$A1
   SBC   (FP),Y
   LDY   #$9F
   LDA   (FP),Y
   LDY   #$A2
   SBC   (FP),Y
   BVC   .cmp_novf@57
   EOR   #$80
.cmp_novf@57:
   ASL
   LDA   #$00
   ROL
   EOR   #$01
   LDY   #$A3
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$9B
   LDA   (FP),Y
   STA   HARGS
   INY
   LDA   (FP),Y
   STA   HARGS+1
   LDY   #$A3
   LDA   (FP),Y
   STA   HARGS+2
   INY
   LDA   (FP),Y
   STA   HARGS+3
   JSR   sdivmod16
   LDA   HARGS+6
   LDY   #$A5
   STA   (FP),Y
   LDA   HARGS+7
   INY
   STA   (FP),Y
   LDY   #$C1
   LDA   (FP),Y
   LDY   #$A7
   STA   (FP),Y
   LDY   #$C2
   LDA   (FP),Y
   LDY   #$A8
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$FF
   SEC
   LDY   #$A7
   SBC   (FP),Y
   LDA   #$FF
   INY
   SBC   (FP),Y
   LDA   #$FF
   INY
   SBC   (FP),Y
   LDA   #$7F
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
   INY
   ORA   (FP),Y
   BEQ   .and_false@15
   LDA   #$FF
   INY
   STA   (FP),Y
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
   LDY   #$CA
   LDA   (FP),Y
   SEC
   LDY   #$B0
   SBC   (FP),Y
   LDY   #$B2
   STA   (FP),Y
   LDY   #$CB
   LDA   (FP),Y
   LDY   #$B1
   SBC   (FP),Y
   LDY   #$B3
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@15
   LDA   #$01
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   JMP   .and_end@16
.and_false@15:
   LDA   #$00
   LDY   #$B4
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@16:
   LDY   #$B4
   LDA   (FP),Y
   SEC
   LDY   #$A5
   SBC   (FP),Y
   LDY   #$B5
   LDA   (FP),Y
   LDY   #$A6
   SBC   (FP),Y
   BVC   .cmp_novf@58
   EOR   #$80
.cmp_novf@58:
   ASL
   LDA   #$00
   ROL
   EOR   #$01
   LDY   #$B6
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$B8
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$B9
   STA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@59
   DEY
   LDA   (FP),Y
   CMP   #$01
   BNE   .cmp_true@59
   LDA   #$00
   JMP   .cmp_end@60
.cmp_true@59:
   LDA   #$01
.cmp_end@60:
   LDY   #$BA
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@17
   LDY   #$01
   LDA   (FP),Y
   ORA   #$80
   LDY   #$BC
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   LDY   #$BD
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   LDY   #$BD
   LDA   (FP),Y
   LDY   #$02
   STA   (FP),Y
.if_end@17:
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
   ADC   #$CB
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$BE
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

   ; prologue: 0 arg bytes, 4 local bytes
   SEC
   LDA   SSP
   SBC   #$06
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$05
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
   SEC
   LDA   SSP
   SBC   #$0C
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   DEY
   LDA   (FP),Y
   STA   (SSP),Y
   LDA   #$E8
   INY
   STA   (SSP),Y
   LDA   #$22
   INY
   STA   (SSP),Y
   DEY
   LDA   (FP),Y
   LDY   #$04
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
   LDA   #$06
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
   LDY   #$03
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
   ADC   #$06
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$05
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
