SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24

check:
   SUBROUTINE

   ; prologue: 21 arg bytes, 176 local bytes
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

   LDA   #$00
   LDY   #$01
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$BF
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
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   LDY   #$BD
   SBC   (FP),Y
   LDY   #$06
   LDA   (FP),Y
   LDY   #$BE
   SBC   (FP),Y
   LDA   #$00
   ROL
   LDY   #$07
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$04
   LDA   (FP),Y
   LDY   #$08
   CMP   (FP),Y
   BNE   .thread@0
   LDY   #$03
   LDA   (FP),Y
   LDY   #$07
   CMP   (FP),Y
   BEQ   .cmp_true@0
.thread@0:
   LDA   #$00
   JMP   .cmp_end@1
.cmp_true@0:
   LDA   #$01
.cmp_end@1:
   LDY   #$09
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@0
   LDA   #$7F
   BNE   .or_true@0
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   JMP   .or_end@1
.or_true@0:
   LDA   #$01
   LDY   #$0B
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@1:
   LDY   #$0B
   LDA   (FP),Y
   LDY   #$0D
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$0E
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CMP   #$01
   BNE   .cmp_true@3
   LDA   #$00
   JMP   .cmp_end@4
.cmp_true@3:
   LDA   #$01
.cmp_end@4:
   LDY   #$0F
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@2
   LDY   #$01
   LDA   (FP),Y
   ORA   #$01
   LDY   #$11
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   LDY   #$12
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   LDY   #$12
   LDA   (FP),Y
   LDY   #$02
   STA   (FP),Y
.if_end@2:
   LDY   #$B3
   LDA   (FP),Y
   LDY   #$13
   STA   (FP),Y
   LDY   #$B4
   LDA   (FP),Y
   LDY   #$14
   STA   (FP),Y
   LDY   #$B5
   LDA   (FP),Y
   LDY   #$15
   STA   (FP),Y
   LDY   #$B6
   LDA   (FP),Y
   LDY   #$16
   STA   (FP),Y
   LDY   #$B7
   LDA   (FP),Y
   LDY   #$17
   STA   (FP),Y
   LDY   #$B8
   LDA   (FP),Y
   LDY   #$18
   STA   (FP),Y
   LDY   #$B9
   LDA   (FP),Y
   LDY   #$19
   STA   (FP),Y
   LDY   #$BA
   LDA   (FP),Y
   LDY   #$1A
   STA   (FP),Y
   CMP   #$FF
   BNE   .cmp_true@5
   DEY
   LDA   (FP),Y
   CMP   #$B6
   BNE   .cmp_true@5
   DEY
   LDA   (FP),Y
   CMP   #$A9
   BNE   .cmp_true@5
   DEY
   LDA   (FP),Y
   CMP   #$C6
   BNE   .cmp_true@5
   DEY
   LDA   (FP),Y
   CMP   #$D8
   BNE   .cmp_true@5
   DEY
   LDA   (FP),Y
   CMP   #$02
   BNE   .cmp_true@5
   DEY
   LDA   (FP),Y
   CMP   #$DB
   BNE   .cmp_true@5
   DEY
   LDA   (FP),Y
   CMP   #$89
   BNE   .cmp_true@5
   LDA   #$00
   JMP   .cmp_end@6
.cmp_true@5:
   LDA   #$01
.cmp_end@6:
   LDY   #$1B
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
   LDY   #$1D
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   LDY   #$1E
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   LDY   #$1E
   LDA   (FP),Y
   LDY   #$02
   STA   (FP),Y
.if_end@3:
   LDY   #$BF
   LDA   (FP),Y
   LDY   #$1F
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDA   #$D1
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$20
   LDA   (FP),Y
   LDY   #$23
   CMP   (FP),Y
   BNE   .cmp_true@8
   LDA   #$00
   JMP   .cmp_end@9
.cmp_true@8:
   LDA   #$01
.cmp_end@9:
   LDY   #$25
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
   LDY   #$27
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   LDY   #$28
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   LDY   #$28
   LDA   (FP),Y
   LDY   #$02
   STA   (FP),Y
.if_end@4:
   LDA   #$FF
   LDY   #$29
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
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$2A
   LDA   (FP),Y
   CMP   #$FF
   BNE   .cmp_true@10
   DEY
   LDA   (FP),Y
   CMP   #$FF
   BNE   .cmp_true@10
   LDA   #$00
   JMP   .cmp_end@11
.cmp_true@10:
   LDA   #$01
.cmp_end@11:
   LDY   #$31
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
   LDY   #$33
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   LDY   #$34
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   LDY   #$34
   LDA   (FP),Y
   LDY   #$02
   STA   (FP),Y
.if_end@5:
   LDY   #$C0
   LDA   (FP),Y
   LDY   #$35
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDA   #$FF
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@13
   LDA   #$00
   JMP   .sx_done@14
.sx_neg@13:
   LDA   #$FF
.sx_done@14:
   LDY   #$39
   STA   (FP),Y
   LDY   #$36
   LDA   (FP),Y
   LDY   #$39
   CMP   (FP),Y
   BNE   .thread@1
   LDY   #$35
   LDA   (FP),Y
   LDY   #$38
   CMP   (FP),Y
   BEQ   .cmp_true@15
.thread@1:
   LDA   #$00
   JMP   .cmp_end@16
.cmp_true@15:
   LDA   #$01
.cmp_end@16:
   LDY   #$3A
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDA   #$45
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$3F
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$40
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@18
   LDA   #$00
   JMP   .sx_done@19
.sx_neg@18:
   LDA   #$FF
.sx_done@19:
   LDY   #$41
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
   LDY   #$BA
   LDA   (FP),Y
   LDY   #$46
   CMP   (FP),Y
   BNE   .thread@2
   LDY   #$B9
   LDA   (FP),Y
   LDY   #$45
   CMP   (FP),Y
   BNE   .thread@2
   LDY   #$B8
   LDA   (FP),Y
   LDY   #$44
   CMP   (FP),Y
   BNE   .thread@2
   LDY   #$B7
   LDA   (FP),Y
   LDY   #$43
   CMP   (FP),Y
   BNE   .thread@2
   LDY   #$B6
   LDA   (FP),Y
   LDY   #$42
   CMP   (FP),Y
   BNE   .thread@2
   LDY   #$B5
   LDA   (FP),Y
   LDY   #$41
   CMP   (FP),Y
   BNE   .thread@2
   LDY   #$B4
   LDA   (FP),Y
   LDY   #$40
   CMP   (FP),Y
   BNE   .thread@2
   LDY   #$B3
   LDA   (FP),Y
   LDY   #$3F
   CMP   (FP),Y
   BEQ   .cmp_true@20
.thread@2:
   LDA   #$00
   JMP   .cmp_end@21
.cmp_true@20:
   LDA   #$01
.cmp_end@21:
   LDY   #$47
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$3A
   LDA   (FP),Y
   LDY   #$47
   AND   (FP),Y
   LDY   #$49
   STA   (FP),Y
   LDY   #$3B
   LDA   (FP),Y
   LDY   #$48
   AND   (FP),Y
   LDY   #$4A
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$4B
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$4C
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@23
   LDA   #$00
   JMP   .sx_done@24
.sx_neg@23:
   LDA   #$FF
.sx_done@24:
   LDY   #$4D
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$4B
   LDA   (FP),Y
   LDY   #$52
   STA   (FP),Y
   LDY   #$4C
   LDA   (FP),Y
   LDY   #$53
   STA   (FP),Y
   LDY   #$4D
   LDA   (FP),Y
   LDY   #$54
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   AND   #$07
   INY
   STA   (FP),Y
   LDY   #$52
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
   LDA   #$00
   STA   HARGS+7
   LDY   #$C1
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
   LDA   #$00
   STA   HARGS+15
   JSR   mul64
   LDA   HARGS+16
   LDY   #$59
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
   LDY   #$59
   LDA   (FP),Y
   LDY   #$60
   STA   (FP),Y
   LDY   #$5A
   LDA   (FP),Y
   LDY   #$61
   STA   (FP),Y
   LDY   #$5B
   LDA   (FP),Y
   LDY   #$62
   STA   (FP),Y
   LDY   #$5C
   LDA   (FP),Y
   LDY   #$63
   STA   (FP),Y
   LDY   #$5D
   LDA   (FP),Y
   LDY   #$64
   STA   (FP),Y
   LDY   #$5E
   LDA   (FP),Y
   LDY   #$65
   STA   (FP),Y
   LDY   #$5F
   LDA   (FP),Y
   AND   #$07
   LDY   #$66
   STA   (FP),Y
   LDY   #$60
   LDA   (FP),Y
   LDY   #$67
   STA   (FP),Y
   LDY   #$61
   LDA   (FP),Y
   LDY   #$68
   STA   (FP),Y
   LDY   #$62
   LDA   (FP),Y
   LDY   #$69
   STA   (FP),Y
   LDY   #$63
   LDA   (FP),Y
   LDY   #$6A
   STA   (FP),Y
   LDY   #$64
   LDA   (FP),Y
   LDY   #$6B
   STA   (FP),Y
   LDY   #$65
   LDA   (FP),Y
   LDY   #$6C
   STA   (FP),Y
   LDY   #$66
   LDA   (FP),Y
   LDY   #$6D
   STA   (FP),Y
   CMP   #$00
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
   LDY   #$6E
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@6
   LDY   #$01
   LDA   (FP),Y
   ORA   #$10
   LDY   #$70
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   LDY   #$71
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   LDY   #$71
   LDA   (FP),Y
   LDY   #$02
   STA   (FP),Y
.if_end@6:
   LDY   #$BF
   LDA   (FP),Y
   LDY   #$72
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$C0
   LDA   (FP),Y
   SEC
   SBC   (FP),Y
   LDA   #$00
   ROL
   LDY   #$74
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$72
   LDA   (FP),Y
   SEC
   LDY   #$74
   SBC   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$75
   SBC   (FP),Y
   BVC   .cmp_novf@28
   EOR   #$80
.cmp_novf@28:
   ASL
   LDA   #$00
   ROL
   EOR   #$01
   LDY   #$76
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$BB
   LDA   (FP),Y
   LDY   #$78
   STA   (FP),Y
   LDY   #$BC
   LDA   (FP),Y
   LDY   #$79
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@29
   LDA   #$00
   JMP   .sx_done@30
.sx_neg@29:
   LDA   #$FF
.sx_done@30:
   LDY   #$7A
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   CMP   (FP),Y
   BNE   .cmp_end@32
   DEY
   CMP   (FP),Y
   BNE   .cmp_end@32
   DEY
   CMP   (FP),Y
   BNE   .cmp_end@32
   LDA   #$07
   DEY
   CMP   (FP),Y
   BEQ   .cmp_true@31
   LDA   #$00
   JMP   .cmp_end@32
.cmp_true@31:
   LDA   #$01
.cmp_end@32:
   LDY   #$7C
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDA   #$E4
   SEC
   SBC   #$07
   LDA   #$F6
   SBC   #$00
   LDA   #$F7
   SBC   #$00
   LDA   #$71
   SBC   #$00
   LDA   #$B8
   SBC   #$00
   LDA   #$70
   SBC   #$00
   LDA   #$2F
   SBC   #$00
   LDA   #$FF
   SBC   #$00
   BVC   .cmp_novf@34
   EOR   #$80
.cmp_novf@34:
   ASL
   LDA   #$00
   ROL
   LDY   #$7E
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$7C
   LDA   (FP),Y
   CLC
   LDY   #$7E
   ADC   (FP),Y
   LDY   #$80
   STA   (FP),Y
   LDY   #$7D
   LDA   (FP),Y
   LDY   #$7F
   ADC   (FP),Y
   LDY   #$81
   STA   (FP),Y
   LDY   #$76
   LDA   (FP),Y
   LDY   #$80
   AND   (FP),Y
   LDY   #$82
   STA   (FP),Y
   LDY   #$77
   LDA   (FP),Y
   LDY   #$81
   AND   (FP),Y
   LDY   #$83
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$84
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$85
   STA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@35
   DEY
   LDA   (FP),Y
   CMP   #$01
   BNE   .cmp_true@35
   LDA   #$00
   JMP   .cmp_end@36
.cmp_true@35:
   LDA   #$01
.cmp_end@36:
   LDY   #$86
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@7
   LDY   #$01
   LDA   (FP),Y
   ORA   #$20
   LDY   #$88
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   LDY   #$89
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   LDY   #$89
   LDA   (FP),Y
   LDY   #$02
   STA   (FP),Y
.if_end@7:
   LDY   #$BB
   LDA   (FP),Y
   LDY   #$8A
   STA   (FP),Y
   LDY   #$BC
   LDA   (FP),Y
   LDY   #$8B
   STA   (FP),Y
   CMP   #$60
   BNE   .cmp_true@38
   DEY
   LDA   (FP),Y
   CMP   #$E0
   BNE   .cmp_true@38
   LDA   #$00
   JMP   .cmp_end@39
.cmp_true@38:
   LDA   #$01
.cmp_end@39:
   LDY   #$8C
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
   ORA   #$40
   LDY   #$8E
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   LDY   #$8F
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   LDY   #$8F
   LDA   (FP),Y
   LDY   #$02
   STA   (FP),Y
.if_end@8:
   LDY   #$BD
   LDA   (FP),Y
   LDY   #$90
   STA   (FP),Y
   LDY   #$BE
   LDA   (FP),Y
   LDY   #$91
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
   LDY   #$C1
   LDA   (FP),Y
   ORA   (FP),Y
   LDY   #$97
   STA   (FP),Y
   LDY   #$C2
   LDA   (FP),Y
   ORA   (FP),Y
   LDY   #$98
   STA   (FP),Y
   LDY   #$C3
   LDA   (FP),Y
   ORA   (FP),Y
   LDY   #$99
   STA   (FP),Y
   LDY   #$C4
   LDA   (FP),Y
   ORA   (FP),Y
   LDY   #$9A
   STA   (FP),Y
   LDY   #$C5
   LDA   (FP),Y
   ORA   (FP),Y
   LDY   #$9B
   STA   (FP),Y
   LDY   #$C6
   LDA   (FP),Y
   ORA   (FP),Y
   LDY   #$9C
   STA   (FP),Y
   LDY   #$C7
   LDA   (FP),Y
   ORA   (FP),Y
   LDY   #$9D
   STA   (FP),Y
   LDY   #$90
   LDA   (FP),Y
   LDY   #$97
   AND   (FP),Y
   LDY   #$9E
   STA   (FP),Y
   LDY   #$91
   LDA   (FP),Y
   LDY   #$98
   AND   (FP),Y
   LDY   #$9F
   STA   (FP),Y
   LDY   #$92
   LDA   (FP),Y
   LDY   #$99
   AND   (FP),Y
   LDY   #$A0
   STA   (FP),Y
   LDY   #$93
   LDA   (FP),Y
   LDY   #$9A
   AND   (FP),Y
   LDY   #$A1
   STA   (FP),Y
   LDY   #$94
   LDA   (FP),Y
   LDY   #$9B
   AND   (FP),Y
   LDY   #$A2
   STA   (FP),Y
   LDY   #$95
   LDA   (FP),Y
   LDY   #$9C
   AND   (FP),Y
   LDY   #$A3
   STA   (FP),Y
   LDY   #$96
   LDA   (FP),Y
   LDY   #$9D
   AND   (FP),Y
   LDY   #$A4
   STA   (FP),Y
   LDY   #$9E
   LDA   (FP),Y
   LDY   #$A5
   STA   (FP),Y
   LDY   #$9F
   LDA   (FP),Y
   LDY   #$A6
   STA   (FP),Y
   LDY   #$A0
   LDA   (FP),Y
   LDY   #$A7
   STA   (FP),Y
   LDY   #$A1
   LDA   (FP),Y
   LDY   #$A8
   STA   (FP),Y
   LDY   #$A5
   LDA   (FP),Y
   LDY   #$A9
   STA   (FP),Y
   LDY   #$A6
   LDA   (FP),Y
   LDY   #$AA
   STA   (FP),Y
   LDY   #$A7
   LDA   (FP),Y
   LDY   #$AB
   STA   (FP),Y
   LDY   #$A8
   LDA   (FP),Y
   LDY   #$AC
   STA   (FP),Y
   LDY   #$AA
   LDA   (FP),Y
   BNE   .cmp_true@41
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@41
   LDA   #$00
   JMP   .cmp_end@42
.cmp_true@41:
   LDA   #$01
.cmp_end@42:
   LDY   #$AD
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
   ORA   #$80
   LDY   #$AF
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   LDY   #$B0
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   LDY   #$B0
   LDA   (FP),Y
   LDY   #$02
   STA   (FP),Y
.if_end@9:
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
   ADC   #$C7
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

   LDA   #$D1
   LDY   #$01
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$15
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   #$89
   STA   (SSP),Y
   LDA   #$DB
   INY
   STA   (SSP),Y
   LDA   #$02
   INY
   STA   (SSP),Y
   LDA   #$D8
   INY
   STA   (SSP),Y
   LDA   #$C6
   INY
   STA   (SSP),Y
   LDA   #$A9
   INY
   STA   (SSP),Y
   LDA   #$B6
   INY
   STA   (SSP),Y
   LDA   #$FF
   INY
   STA   (SSP),Y
   LDA   #$E0
   INY
   STA   (SSP),Y
   LDA   #$60
   INY
   STA   (SSP),Y
   LDA   #$00
   INY
   STA   (SSP),Y
   LDA   #$80
   INY
   STA   (SSP),Y
   LDY   #$01
   LDA   (FP),Y
   LDY   #$0D
   STA   (SSP),Y
   LDA   #$40
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
