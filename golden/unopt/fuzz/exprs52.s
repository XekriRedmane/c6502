SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24

check:
   SUBROUTINE

   ; prologue: 29 arg bytes, 190 local bytes
   SEC
   LDA   SSP
   SBC   #$C0
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$BF
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
   LDY   #$D9
   LDA   (FP),Y
   LDY   #$03
   STA   (FP),Y
   LDY   #$DA
   LDA   (FP),Y
   LDY   #$04
   STA   (FP),Y
   LDY   #$DB
   LDA   (FP),Y
   LDY   #$05
   STA   (FP),Y
   LDY   #$DC
   LDA   (FP),Y
   LDY   #$06
   STA   (FP),Y
   LDY   #$DD
   LDA   (FP),Y
   LDY   #$07
   STA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@0
   DEY
   LDA   (FP),Y
   CMP   #$70
   BNE   .cmp_true@0
   DEY
   LDA   (FP),Y
   CMP   #$1C
   BNE   .cmp_true@0
   DEY
   LDA   (FP),Y
   CMP   #$D3
   BNE   .cmp_true@0
   DEY
   LDA   (FP),Y
   CMP   #$68
   BNE   .cmp_true@0
   LDA   #$00
   JMP   .cmp_end@1
.cmp_true@0:
   LDA   #$01
.cmp_end@1:
   LDY   #$08
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
   LDY   #$0A
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   LDY   #$0B
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   LDY   #$0B
   LDA   (FP),Y
   LDY   #$02
   STA   (FP),Y
.if_end@0:
   LDY   #$C5
   LDA   (FP),Y
   LDY   #$0C
   STA   (FP),Y
   LDY   #$C6
   LDA   (FP),Y
   LDY   #$0D
   STA   (FP),Y
   LDY   #$C7
   LDA   (FP),Y
   LDY   #$0E
   STA   (FP),Y
   LDY   #$C8
   LDA   (FP),Y
   LDY   #$0F
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@3
   LDA   #$00
   JMP   .sx_done@4
.sx_neg@3:
   LDA   #$FF
.sx_done@4:
   LDY   #$10
   STA   (FP),Y
   LDA   #$27
   STA   HARGS
   LDA   #$38
   STA   HARGS+1
   LDA   #$D2
   STA   HARGS+2
   LDA   #$AB
   STA   HARGS+3
   LDA   #$FF
   STA   HARGS+4
   ASL
   LDA   #$00
   ADC   #$FF
   EOR   #$FF
   STA   HARGS+5
   STA   HARGS+6
   STA   HARGS+7
   LDY   #$0C
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
   ASL
   LDA   #$00
   ADC   #$FF
   EOR   #$FF
   STA   HARGS+13
   STA   HARGS+14
   STA   HARGS+15
   JSR   sdivmod64
   LDA   HARGS+24
   LDY   #$11
   STA   (FP),Y
   LDA   HARGS+25
   INY
   STA   (FP),Y
   LDA   HARGS+26
   INY
   STA   (FP),Y
   LDA   HARGS+27
   INY
   STA   (FP),Y
   LDA   HARGS+28
   INY
   STA   (FP),Y
   LDA   #$80
   SEC
   LDY   #$D9
   SBC   (FP),Y
   LDA   #$00
   INY
   SBC   (FP),Y
   LDA   #$00
   INY
   SBC   (FP),Y
   LDA   #$00
   INY
   SBC   (FP),Y
   LDA   #$00
   INY
   SBC   (FP),Y
   BVC   .cmp_novf@5
   EOR   #$80
.cmp_novf@5:
   ASL
   LDA   #$00
   ROL
   EOR   #$01
   LDY   #$16
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$18
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$19
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@6
   LDA   #$00
   JMP   .sx_done@7
.sx_neg@6:
   LDA   #$FF
.sx_done@7:
   LDY   #$1A
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$11
   LDA   (FP),Y
   SEC
   LDY   #$18
   SBC   (FP),Y
   LDY   #$12
   LDA   (FP),Y
   LDY   #$19
   SBC   (FP),Y
   LDY   #$13
   LDA   (FP),Y
   LDY   #$1A
   SBC   (FP),Y
   LDY   #$14
   LDA   (FP),Y
   LDY   #$1B
   SBC   (FP),Y
   LDY   #$15
   LDA   (FP),Y
   LDY   #$1C
   SBC   (FP),Y
   BVC   .cmp_novf@8
   EOR   #$80
.cmp_novf@8:
   ASL
   LDA   #$00
   ROL
   EOR   #$01
   LDY   #$1D
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDA   #$02
   BNE   .or_true@1
   LDY   #$D9
   LDA   (FP),Y
   LDY   #$C9
   ORA   (FP),Y
   LDY   #$1F
   STA   (FP),Y
   LDY   #$DA
   LDA   (FP),Y
   LDY   #$CA
   ORA   (FP),Y
   LDY   #$20
   STA   (FP),Y
   LDY   #$DB
   LDA   (FP),Y
   LDY   #$CB
   ORA   (FP),Y
   LDY   #$21
   STA   (FP),Y
   LDY   #$DC
   LDA   (FP),Y
   LDY   #$CC
   ORA   (FP),Y
   LDY   #$22
   STA   (FP),Y
   LDY   #$DD
   LDA   (FP),Y
   LDY   #$CD
   ORA   (FP),Y
   LDY   #$23
   STA   (FP),Y
   LDY   #$1F
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   INY
   ORA   (FP),Y
   INY
   ORA   (FP),Y
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
   LDY   #$24
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@2:
   LDY   #$1D
   LDA   (FP),Y
   SEC
   LDY   #$24
   SBC   (FP),Y
   LDY   #$1E
   LDA   (FP),Y
   LDY   #$25
   SBC   (FP),Y
   BVC   .cmp_novf@9
   EOR   #$80
.cmp_novf@9:
   ASL
   LDA   #$00
   ROL
   EOR   #$01
   LDY   #$26
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$28
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$29
   STA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@10
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@10
   LDA   #$00
   JMP   .cmp_end@11
.cmp_true@10:
   LDA   #$01
.cmp_end@11:
   LDY   #$2A
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
   LDY   #$2C
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   LDY   #$2D
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   LDY   #$2D
   LDA   (FP),Y
   LDY   #$02
   STA   (FP),Y
.if_end@3:
   LDY   #$D9
   LDA   (FP),Y
   LDY   #$2E
   STA   (FP),Y
   LDY   #$DA
   LDA   (FP),Y
   LDY   #$2F
   STA   (FP),Y
   LDY   #$DB
   LDA   (FP),Y
   LDY   #$30
   STA   (FP),Y
   LDY   #$DC
   LDA   (FP),Y
   LDY   #$31
   STA   (FP),Y
   LDY   #$DD
   LDA   (FP),Y
   LDY   #$32
   STA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@13
   DEY
   LDA   (FP),Y
   CMP   #$70
   BNE   .cmp_true@13
   DEY
   LDA   (FP),Y
   CMP   #$1C
   BNE   .cmp_true@13
   DEY
   LDA   (FP),Y
   CMP   #$D3
   BNE   .cmp_true@13
   DEY
   LDA   (FP),Y
   CMP   #$68
   BNE   .cmp_true@13
   LDA   #$00
   JMP   .cmp_end@14
.cmp_true@13:
   LDA   #$01
.cmp_end@14:
   LDY   #$33
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@4
   LDY   #$01
   LDA   (FP),Y
   ORA   #$04
   LDY   #$35
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   LDY   #$36
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   LDY   #$36
   LDA   (FP),Y
   LDY   #$02
   STA   (FP),Y
.if_end@4:
   LDA   #$FF
   LDY   #$37
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
   LDY   #$C5
   LDA   (FP),Y
   LDY   #$3C
   STA   (FP),Y
   LDY   #$C6
   LDA   (FP),Y
   LDY   #$3D
   STA   (FP),Y
   LDY   #$C7
   LDA   (FP),Y
   LDY   #$3E
   STA   (FP),Y
   LDY   #$C8
   LDA   (FP),Y
   LDY   #$3F
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@16
   LDA   #$00
   JMP   .sx_done@17
.sx_neg@16:
   LDA   #$FF
.sx_done@17:
   LDY   #$40
   STA   (FP),Y
   LDY   #$37
   LDA   (FP),Y
   SEC
   LDY   #$3C
   SBC   (FP),Y
   LDY   #$41
   STA   (FP),Y
   LDY   #$38
   LDA   (FP),Y
   LDY   #$3D
   SBC   (FP),Y
   LDY   #$42
   STA   (FP),Y
   LDY   #$39
   LDA   (FP),Y
   LDY   #$3E
   SBC   (FP),Y
   LDY   #$43
   STA   (FP),Y
   LDY   #$3A
   LDA   (FP),Y
   LDY   #$3F
   SBC   (FP),Y
   LDY   #$44
   STA   (FP),Y
   LDY   #$3B
   LDA   (FP),Y
   LDY   #$40
   SBC   (FP),Y
   LDY   #$45
   STA   (FP),Y
   LDY   #$41
   LDA   (FP),Y
   LDY   #$46
   STA   (FP),Y
   LDY   #$42
   LDA   (FP),Y
   LDY   #$47
   STA   (FP),Y
   LDY   #$43
   LDA   (FP),Y
   LDY   #$48
   STA   (FP),Y
   LDY   #$44
   LDA   (FP),Y
   LDY   #$49
   STA   (FP),Y
   LDY   #$45
   LDA   (FP),Y
   AND   #$01
   LDY   #$4A
   STA   (FP),Y
   LDY   #$46
   LDA   (FP),Y
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
   EOR   #$01
   LDY   #$4F
   STA   (FP),Y
   LDY   #$4B
   LDA   (FP),Y
   LDY   #$50
   STA   (FP),Y
   LDY   #$4C
   LDA   (FP),Y
   LDY   #$51
   STA   (FP),Y
   LDY   #$4D
   LDA   (FP),Y
   LDY   #$52
   STA   (FP),Y
   LDY   #$4E
   LDA   (FP),Y
   LDY   #$53
   STA   (FP),Y
   LDY   #$4F
   LDA   (FP),Y
   SEC
   SBC   #$01
   LDY   #$54
   STA   (FP),Y
   LDY   #$50
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   INY
   ORA   (FP),Y
   INY
   ORA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lnot_true@18
   LDA   #$00
   JMP   .lnot_end@19
.lnot_true@18:
   LDA   #$01
.lnot_end@19:
   LDY   #$55
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$57
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$58
   STA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@20
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@20
   LDA   #$00
   JMP   .cmp_end@21
.cmp_true@20:
   LDA   #$01
.cmp_end@21:
   LDY   #$59
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
   ORA   #$08
   LDY   #$5B
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   LDY   #$5C
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   LDY   #$5C
   LDA   (FP),Y
   LDY   #$02
   STA   (FP),Y
.if_end@5:
   LDA   #$45
   LDY   #$5D
   STA   (FP),Y
   ORA   #$00
   BNE   .or_true@6
   LDY   #$C5
   LDA   (FP),Y
   LDY   #$C1
   AND   (FP),Y
   LDY   #$5E
   STA   (FP),Y
   LDY   #$C6
   LDA   (FP),Y
   LDY   #$C2
   AND   (FP),Y
   LDY   #$5F
   STA   (FP),Y
   LDY   #$C7
   LDA   (FP),Y
   LDY   #$C3
   AND   (FP),Y
   LDY   #$60
   STA   (FP),Y
   LDY   #$C8
   LDA   (FP),Y
   LDY   #$C4
   AND   (FP),Y
   LDY   #$61
   STA   (FP),Y
   LDY   #$5E
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   INY
   ORA   (FP),Y
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
   LDY   #$62
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@7:
   LDY   #$62
   LDA   (FP),Y
   LDY   #$64
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$65
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CMP   #$01
   BNE   .cmp_true@23
   LDA   #$00
   JMP   .cmp_end@24
.cmp_true@23:
   LDA   #$01
.cmp_end@24:
   LDY   #$66
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
   ORA   #$10
   LDY   #$68
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   LDY   #$69
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   LDY   #$69
   LDA   (FP),Y
   LDY   #$02
   STA   (FP),Y
.if_end@8:
   LDY   #$C5
   LDA   (FP),Y
   LDY   #$6A
   STA   (FP),Y
   LDY   #$C6
   LDA   (FP),Y
   LDY   #$6B
   STA   (FP),Y
   LDY   #$C7
   LDA   (FP),Y
   LDY   #$6C
   STA   (FP),Y
   LDY   #$C8
   LDA   (FP),Y
   LDY   #$6D
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@25
   LDA   #$00
   JMP   .sx_done@26
.sx_neg@25:
   LDA   #$FF
.sx_done@26:
   LDY   #$6E
   STA   (FP),Y
   LDY   #$6A
   LDA   (FP),Y
   SEC
   LDY   #$C9
   SBC   (FP),Y
   LDY   #$6F
   STA   (FP),Y
   LDY   #$6B
   LDA   (FP),Y
   LDY   #$CA
   SBC   (FP),Y
   LDY   #$70
   STA   (FP),Y
   LDY   #$6C
   LDA   (FP),Y
   LDY   #$CB
   SBC   (FP),Y
   LDY   #$71
   STA   (FP),Y
   LDY   #$6D
   LDA   (FP),Y
   LDY   #$CC
   SBC   (FP),Y
   LDY   #$72
   STA   (FP),Y
   LDY   #$6E
   LDA   (FP),Y
   LDY   #$CD
   SBC   (FP),Y
   LDY   #$73
   STA   (FP),Y
   LDY   #$6F
   LDA   (FP),Y
   LDY   #$74
   STA   (FP),Y
   LDY   #$70
   LDA   (FP),Y
   LDY   #$75
   STA   (FP),Y
   LDY   #$71
   LDA   (FP),Y
   LDY   #$76
   STA   (FP),Y
   LDY   #$72
   LDA   (FP),Y
   LDY   #$77
   STA   (FP),Y
   LDY   #$73
   LDA   (FP),Y
   AND   #$01
   LDY   #$78
   STA   (FP),Y
   LDY   #$74
   LDA   (FP),Y
   LDY   #$79
   STA   (FP),Y
   LDY   #$75
   LDA   (FP),Y
   LDY   #$7A
   STA   (FP),Y
   LDY   #$76
   LDA   (FP),Y
   LDY   #$7B
   STA   (FP),Y
   LDY   #$77
   LDA   (FP),Y
   LDY   #$7C
   STA   (FP),Y
   LDY   #$78
   LDA   (FP),Y
   EOR   #$01
   LDY   #$7D
   STA   (FP),Y
   LDY   #$79
   LDA   (FP),Y
   LDY   #$7E
   STA   (FP),Y
   LDY   #$7A
   LDA   (FP),Y
   LDY   #$7F
   STA   (FP),Y
   LDY   #$7B
   LDA   (FP),Y
   LDY   #$80
   STA   (FP),Y
   LDY   #$7C
   LDA   (FP),Y
   LDY   #$81
   STA   (FP),Y
   LDY   #$7D
   LDA   (FP),Y
   SEC
   SBC   #$01
   LDY   #$82
   STA   (FP),Y
   LDY   #$C9
   LDA   (FP),Y
   SEC
   SBC   #$FF
   INY
   LDA   (FP),Y
   SBC   #$7F
   INY
   LDA   (FP),Y
   SBC   #$00
   INY
   LDA   (FP),Y
   SBC   #$00
   INY
   LDA   (FP),Y
   SBC   #$00
   BVC   .cmp_novf@27
   EOR   #$80
.cmp_novf@27:
   ASL
   LDA   #$00
   ROL
   LDY   #$83
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$7E
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
   ASL
   LDA   #$00
   ADC   #$FF
   EOR   #$FF
   STA   HARGS+5
   STA   HARGS+6
   STA   HARGS+7
   INY
   LDA   (FP),Y
   STA   HARGS+8
   JSR   asr64
   LDA   HARGS+9
   LDY   #$85
   STA   (FP),Y
   LDA   HARGS+10
   INY
   STA   (FP),Y
   LDA   HARGS+11
   INY
   STA   (FP),Y
   LDA   HARGS+12
   INY
   STA   (FP),Y
   LDA   HARGS+13
   INY
   STA   (FP),Y
   SEC
   LDA   #$00
   LDY   #$85
   SBC   (FP),Y
   LDY   #$8A
   STA   (FP),Y
   LDA   #$00
   LDY   #$86
   SBC   (FP),Y
   LDY   #$8B
   STA   (FP),Y
   LDA   #$00
   LDY   #$87
   SBC   (FP),Y
   LDY   #$8C
   STA   (FP),Y
   LDA   #$00
   LDY   #$88
   SBC   (FP),Y
   LDY   #$8D
   STA   (FP),Y
   LDA   #$00
   LDY   #$89
   SBC   (FP),Y
   LDY   #$8E
   STA   (FP),Y
   LDY   #$8A
   LDA   (FP),Y
   LDY   #$8F
   STA   (FP),Y
   LDY   #$8B
   LDA   (FP),Y
   LDY   #$90
   STA   (FP),Y
   LDY   #$8C
   LDA   (FP),Y
   LDY   #$91
   STA   (FP),Y
   LDY   #$8D
   LDA   (FP),Y
   LDY   #$92
   STA   (FP),Y
   LDY   #$8E
   LDA   (FP),Y
   AND   #$01
   LDY   #$93
   STA   (FP),Y
   LDY   #$8F
   LDA   (FP),Y
   LDY   #$94
   STA   (FP),Y
   LDY   #$90
   LDA   (FP),Y
   LDY   #$95
   STA   (FP),Y
   LDY   #$91
   LDA   (FP),Y
   LDY   #$96
   STA   (FP),Y
   LDY   #$92
   LDA   (FP),Y
   LDY   #$97
   STA   (FP),Y
   LDY   #$93
   LDA   (FP),Y
   EOR   #$01
   LDY   #$98
   STA   (FP),Y
   LDY   #$94
   LDA   (FP),Y
   LDY   #$99
   STA   (FP),Y
   LDY   #$95
   LDA   (FP),Y
   LDY   #$9A
   STA   (FP),Y
   LDY   #$96
   LDA   (FP),Y
   LDY   #$9B
   STA   (FP),Y
   LDY   #$97
   LDA   (FP),Y
   LDY   #$9C
   STA   (FP),Y
   LDY   #$98
   LDA   (FP),Y
   SEC
   SBC   #$01
   LDY   #$9D
   STA   (FP),Y
   LDY   #$99
   LDA   (FP),Y
   LDY   #$9E
   STA   (FP),Y
   LDY   #$9A
   LDA   (FP),Y
   LDY   #$9F
   STA   (FP),Y
   LDY   #$9B
   LDA   (FP),Y
   LDY   #$A0
   STA   (FP),Y
   LDY   #$9C
   LDA   (FP),Y
   LDY   #$A1
   STA   (FP),Y
   LDY   #$9D
   LDA   (FP),Y
   LDY   #$A2
   STA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@28
   DEY
   LDA   (FP),Y
   CMP   #$40
   BNE   .cmp_true@28
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@28
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@28
   DEY
   LDA   (FP),Y
   CMP   #$01
   BNE   .cmp_true@28
   LDA   #$00
   JMP   .cmp_end@29
.cmp_true@28:
   LDA   #$01
.cmp_end@29:
   LDY   #$A3
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
   ORA   #$20
   LDY   #$A5
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   LDY   #$A6
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   LDY   #$A6
   LDA   (FP),Y
   LDY   #$02
   STA   (FP),Y
.if_end@9:
   LDY   #$C1
   LDA   (FP),Y
   LDY   #$A7
   STA   (FP),Y
   LDY   #$C2
   LDA   (FP),Y
   LDY   #$A8
   STA   (FP),Y
   LDY   #$C3
   LDA   (FP),Y
   LDY   #$A9
   STA   (FP),Y
   LDY   #$C4
   LDA   (FP),Y
   LDY   #$AA
   STA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@31
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@31
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@31
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@31
   LDA   #$00
   JMP   .cmp_end@32
.cmp_true@31:
   LDA   #$01
.cmp_end@32:
   LDY   #$AB
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
   ORA   #$40
   LDY   #$AD
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   LDY   #$AE
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   LDY   #$AE
   LDA   (FP),Y
   LDY   #$02
   STA   (FP),Y
.if_end@10:
   LDA   #$FF
   LDY   #$D3
   EOR   (FP),Y
   LDY   #$AF
   STA   (FP),Y
   LDA   #$7F
   LDY   #$D4
   EOR   (FP),Y
   LDY   #$B0
   STA   (FP),Y
   LDA   #$00
   LDY   #$D5
   EOR   (FP),Y
   LDY   #$B1
   STA   (FP),Y
   LDA   #$00
   LDY   #$D6
   EOR   (FP),Y
   LDY   #$B2
   STA   (FP),Y
   LDA   #$00
   LDY   #$D7
   EOR   (FP),Y
   LDY   #$B3
   STA   (FP),Y
   LDA   #$00
   LDY   #$D8
   EOR   (FP),Y
   LDY   #$B4
   STA   (FP),Y
   LDY   #$AF
   LDA   (FP),Y
   LDY   #$B5
   STA   (FP),Y
   LDY   #$B0
   LDA   (FP),Y
   LDY   #$B6
   STA   (FP),Y
   LDY   #$B1
   LDA   (FP),Y
   LDY   #$B7
   STA   (FP),Y
   LDY   #$B2
   LDA   (FP),Y
   LDY   #$B8
   STA   (FP),Y
   LDY   #$B3
   LDA   (FP),Y
   LDY   #$B9
   STA   (FP),Y
   LDY   #$B4
   LDA   (FP),Y
   LDY   #$BA
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
   CMP   #$7F
   BNE   .cmp_true@34
   DEY
   LDA   (FP),Y
   CMP   #$FE
   BNE   .cmp_true@34
   LDA   #$00
   JMP   .cmp_end@35
.cmp_true@34:
   LDA   #$01
.cmp_end@35:
   LDY   #$BB
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
   ORA   #$80
   LDY   #$BD
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   LDY   #$BE
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   LDY   #$BE
   LDA   (FP),Y
   LDY   #$02
   STA   (FP),Y
.if_end@11:
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
   ADC   #$DD
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$BF
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

   SEC
   LDA   SSP
   SBC   #$1D
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   #$00
   LDY   #$01
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
   INY
   STA   (SSP),Y
   LDA   #$80
   INY
   STA   (SSP),Y
   LDA   #$02
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
   INY
   STA   (SSP),Y
   INY
   STA   (SSP),Y
   LDA   #$FF
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
   LDA   #$68
   INY
   STA   (SSP),Y
   LDA   #$D3
   INY
   STA   (SSP),Y
   LDA   #$1C
   INY
   STA   (SSP),Y
   LDA   #$70
   INY
   STA   (SSP),Y
   LDA   #$00
   INY
   STA   (SSP),Y
   JSR   check
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
