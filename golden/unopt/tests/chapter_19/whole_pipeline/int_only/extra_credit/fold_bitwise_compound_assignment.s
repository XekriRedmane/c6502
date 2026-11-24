SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24

target:
   SUBROUTINE

   ; prologue: 0 arg bytes, 232 local bytes
   SEC
   LDA   SSP
   SBC   #$EA
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$E9
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
   SBC   #$64
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
   ORA   #$00
   BMI   .sx_neg@0
   LDA   #$00
   JMP   .sx_done@1
.sx_neg@0:
   LDA   #$FF
.sx_done@1:
   LDY   #$05
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
   LDY   #$03
   LDA   (FP),Y
   LDY   #$0B
   STA   (FP),Y
   LDY   #$04
   LDA   (FP),Y
   LDY   #$0C
   STA   (FP),Y
   LDY   #$05
   LDA   (FP),Y
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
   LDA   #$64
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   ORA   #$00
   BPL   .sx_done@3
   LDA   #$FF
.sx_done@3:
   LDY   #$15
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
   LDY   #$13
   LDA   (FP),Y
   LDY   #$1B
   STA   (FP),Y
   LDY   #$14
   LDA   (FP),Y
   LDY   #$1C
   STA   (FP),Y
   LDY   #$15
   LDA   (FP),Y
   LDY   #$1D
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
   LDA   #$C8
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   ORA   #$00
   BPL   .sx_done@5
   LDA   #$FF
.sx_done@5:
   LDY   #$25
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
   LDY   #$23
   LDA   (FP),Y
   LDY   #$2B
   STA   (FP),Y
   LDY   #$24
   LDA   (FP),Y
   LDY   #$2C
   STA   (FP),Y
   LDY   #$25
   LDA   (FP),Y
   LDY   #$2D
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
   LDA   #$2C
   INY
   STA   (FP),Y
   LDA   #$01
   INY
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@6
   LDA   #$00
   JMP   .sx_done@7
.sx_neg@6:
   LDA   #$FF
.sx_done@7:
   LDY   #$35
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
   LDY   #$33
   LDA   (FP),Y
   LDY   #$3B
   STA   (FP),Y
   LDY   #$34
   LDA   (FP),Y
   LDY   #$3C
   STA   (FP),Y
   LDY   #$35
   LDA   (FP),Y
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
   LDA   #$40
   INY
   STA   (FP),Y
   LDA   #$9C
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   ORA   #$00
   BPL   .sx_done@9
   LDA   #$FF
.sx_done@9:
   LDY   #$47
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$43
   LDA   (FP),Y
   LDY   #$4B
   STA   (FP),Y
   LDY   #$44
   LDA   (FP),Y
   LDY   #$4C
   STA   (FP),Y
   LDY   #$45
   LDA   (FP),Y
   LDY   #$4D
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$47
   LDA   (FP),Y
   LDY   #$4F
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$0A
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   ORA   #$00
   BPL   .sx_done@11
   LDA   #$FF
.sx_done@11:
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
   LDY   #$0B
   LDA   (FP),Y
   LDY   #$53
   EOR   (FP),Y
   LDY   #$5B
   STA   (FP),Y
   LDY   #$0C
   LDA   (FP),Y
   LDY   #$54
   EOR   (FP),Y
   LDY   #$5C
   STA   (FP),Y
   LDY   #$0D
   LDA   (FP),Y
   LDY   #$55
   EOR   (FP),Y
   LDY   #$5D
   STA   (FP),Y
   LDY   #$0E
   LDA   (FP),Y
   LDY   #$56
   EOR   (FP),Y
   LDY   #$5E
   STA   (FP),Y
   LDY   #$0F
   LDA   (FP),Y
   LDY   #$57
   EOR   (FP),Y
   LDY   #$5F
   STA   (FP),Y
   LDY   #$10
   LDA   (FP),Y
   LDY   #$58
   EOR   (FP),Y
   LDY   #$60
   STA   (FP),Y
   LDY   #$11
   LDA   (FP),Y
   LDY   #$59
   EOR   (FP),Y
   LDY   #$61
   STA   (FP),Y
   LDY   #$12
   LDA   (FP),Y
   LDY   #$5A
   EOR   (FP),Y
   LDY   #$62
   STA   (FP),Y
   LDY   #$5B
   LDA   (FP),Y
   LDY   #$0B
   STA   (FP),Y
   LDY   #$5C
   LDA   (FP),Y
   LDY   #$0C
   STA   (FP),Y
   LDY   #$5D
   LDA   (FP),Y
   LDY   #$0D
   STA   (FP),Y
   LDY   #$5E
   LDA   (FP),Y
   LDY   #$0E
   STA   (FP),Y
   LDY   #$5F
   LDA   (FP),Y
   LDY   #$0F
   STA   (FP),Y
   LDY   #$60
   LDA   (FP),Y
   LDY   #$10
   STA   (FP),Y
   LDY   #$61
   LDA   (FP),Y
   LDY   #$11
   STA   (FP),Y
   LDY   #$62
   LDA   (FP),Y
   LDY   #$12
   STA   (FP),Y
   LDY   #$1B
   LDA   (FP),Y
   LDY   #$0B
   ORA   (FP),Y
   LDY   #$63
   STA   (FP),Y
   LDY   #$1C
   LDA   (FP),Y
   LDY   #$0C
   ORA   (FP),Y
   LDY   #$64
   STA   (FP),Y
   LDY   #$1D
   LDA   (FP),Y
   LDY   #$0D
   ORA   (FP),Y
   LDY   #$65
   STA   (FP),Y
   LDY   #$1E
   LDA   (FP),Y
   LDY   #$0E
   ORA   (FP),Y
   LDY   #$66
   STA   (FP),Y
   LDY   #$1F
   LDA   (FP),Y
   LDY   #$0F
   ORA   (FP),Y
   LDY   #$67
   STA   (FP),Y
   LDY   #$20
   LDA   (FP),Y
   LDY   #$10
   ORA   (FP),Y
   LDY   #$68
   STA   (FP),Y
   LDY   #$21
   LDA   (FP),Y
   LDY   #$11
   ORA   (FP),Y
   LDY   #$69
   STA   (FP),Y
   LDY   #$22
   LDA   (FP),Y
   LDY   #$12
   ORA   (FP),Y
   LDY   #$6A
   STA   (FP),Y
   LDY   #$63
   LDA   (FP),Y
   LDY   #$1B
   STA   (FP),Y
   LDY   #$64
   LDA   (FP),Y
   LDY   #$1C
   STA   (FP),Y
   LDY   #$65
   LDA   (FP),Y
   LDY   #$1D
   STA   (FP),Y
   LDY   #$66
   LDA   (FP),Y
   LDY   #$1E
   STA   (FP),Y
   LDY   #$67
   LDA   (FP),Y
   LDY   #$1F
   STA   (FP),Y
   LDY   #$68
   LDA   (FP),Y
   LDY   #$20
   STA   (FP),Y
   LDY   #$69
   LDA   (FP),Y
   LDY   #$21
   STA   (FP),Y
   LDY   #$6A
   LDA   (FP),Y
   LDY   #$22
   STA   (FP),Y
   LDA   #$1E
   LDY   #$6B
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   ORA   #$00
   BPL   .sx_done@13
   LDA   #$FF
.sx_done@13:
   LDY   #$6D
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
   LDY   #$2B
   LDA   (FP),Y
   LDY   #$6B
   AND   (FP),Y
   LDY   #$73
   STA   (FP),Y
   LDY   #$2C
   LDA   (FP),Y
   LDY   #$6C
   AND   (FP),Y
   LDY   #$74
   STA   (FP),Y
   LDY   #$2D
   LDA   (FP),Y
   LDY   #$6D
   AND   (FP),Y
   LDY   #$75
   STA   (FP),Y
   LDY   #$2E
   LDA   (FP),Y
   LDY   #$6E
   AND   (FP),Y
   LDY   #$76
   STA   (FP),Y
   LDY   #$2F
   LDA   (FP),Y
   LDY   #$6F
   AND   (FP),Y
   LDY   #$77
   STA   (FP),Y
   LDY   #$30
   LDA   (FP),Y
   LDY   #$70
   AND   (FP),Y
   LDY   #$78
   STA   (FP),Y
   LDY   #$31
   LDA   (FP),Y
   LDY   #$71
   AND   (FP),Y
   LDY   #$79
   STA   (FP),Y
   LDY   #$32
   LDA   (FP),Y
   LDY   #$72
   AND   (FP),Y
   LDY   #$7A
   STA   (FP),Y
   LDY   #$73
   LDA   (FP),Y
   LDY   #$2B
   STA   (FP),Y
   LDY   #$74
   LDA   (FP),Y
   LDY   #$2C
   STA   (FP),Y
   LDY   #$75
   LDA   (FP),Y
   LDY   #$2D
   STA   (FP),Y
   LDY   #$76
   LDA   (FP),Y
   LDY   #$2E
   STA   (FP),Y
   LDY   #$77
   LDA   (FP),Y
   LDY   #$2F
   STA   (FP),Y
   LDY   #$78
   LDA   (FP),Y
   LDY   #$30
   STA   (FP),Y
   LDY   #$79
   LDA   (FP),Y
   LDY   #$31
   STA   (FP),Y
   LDY   #$7A
   LDA   (FP),Y
   LDY   #$32
   STA   (FP),Y
   LDY   #$3B
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
   INY
   LDA   (FP),Y
   STA   HARGS+7
   LDY   #$2B
   LDA   (FP),Y
   STA   HARGS+8
   JSR   asl64
   LDA   HARGS+9
   LDY   #$7B
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
   LDA   HARGS+14
   INY
   STA   (FP),Y
   LDA   HARGS+15
   INY
   STA   (FP),Y
   LDA   HARGS+16
   INY
   STA   (FP),Y
   LDY   #$7B
   LDA   (FP),Y
   LDY   #$3B
   STA   (FP),Y
   LDY   #$7C
   LDA   (FP),Y
   LDY   #$3C
   STA   (FP),Y
   LDY   #$7D
   LDA   (FP),Y
   LDY   #$3D
   STA   (FP),Y
   LDY   #$7E
   LDA   (FP),Y
   LDY   #$3E
   STA   (FP),Y
   LDY   #$7F
   LDA   (FP),Y
   LDY   #$3F
   STA   (FP),Y
   LDY   #$80
   LDA   (FP),Y
   LDY   #$40
   STA   (FP),Y
   LDY   #$81
   LDA   (FP),Y
   LDY   #$41
   STA   (FP),Y
   LDY   #$82
   LDA   (FP),Y
   LDY   #$42
   STA   (FP),Y
   LDA   #$02
   LDY   #$83
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   ORA   #$00
   BPL   .sx_done@15
   LDA   #$FF
.sx_done@15:
   LDY   #$85
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
   LDY   #$2B
   LDA   (FP),Y
   LDY   #$83
   ORA   (FP),Y
   LDY   #$8B
   STA   (FP),Y
   LDY   #$2C
   LDA   (FP),Y
   LDY   #$84
   ORA   (FP),Y
   LDY   #$8C
   STA   (FP),Y
   LDY   #$2D
   LDA   (FP),Y
   LDY   #$85
   ORA   (FP),Y
   LDY   #$8D
   STA   (FP),Y
   LDY   #$2E
   LDA   (FP),Y
   LDY   #$86
   ORA   (FP),Y
   LDY   #$8E
   STA   (FP),Y
   LDY   #$2F
   LDA   (FP),Y
   LDY   #$87
   ORA   (FP),Y
   LDY   #$8F
   STA   (FP),Y
   LDY   #$30
   LDA   (FP),Y
   LDY   #$88
   ORA   (FP),Y
   LDY   #$90
   STA   (FP),Y
   LDY   #$31
   LDA   (FP),Y
   LDY   #$89
   ORA   (FP),Y
   LDY   #$91
   STA   (FP),Y
   LDY   #$32
   LDA   (FP),Y
   LDY   #$8A
   ORA   (FP),Y
   LDY   #$92
   STA   (FP),Y
   LDY   #$8B
   LDA   (FP),Y
   LDY   #$2B
   STA   (FP),Y
   LDY   #$8C
   LDA   (FP),Y
   LDY   #$2C
   STA   (FP),Y
   LDY   #$8D
   LDA   (FP),Y
   LDY   #$2D
   STA   (FP),Y
   LDY   #$8E
   LDA   (FP),Y
   LDY   #$2E
   STA   (FP),Y
   LDY   #$8F
   LDA   (FP),Y
   LDY   #$2F
   STA   (FP),Y
   LDY   #$90
   LDA   (FP),Y
   LDY   #$30
   STA   (FP),Y
   LDY   #$91
   LDA   (FP),Y
   LDY   #$31
   STA   (FP),Y
   LDY   #$92
   LDA   (FP),Y
   LDY   #$32
   STA   (FP),Y
   LDY   #$4B
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
   INY
   LDA   (FP),Y
   STA   HARGS+7
   LDY   #$8B
   LDA   (FP),Y
   STA   HARGS+8
   JSR   asr64
   LDA   HARGS+9
   LDY   #$93
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
   LDA   HARGS+14
   INY
   STA   (FP),Y
   LDA   HARGS+15
   INY
   STA   (FP),Y
   LDA   HARGS+16
   INY
   STA   (FP),Y
   LDY   #$93
   LDA   (FP),Y
   LDY   #$4B
   STA   (FP),Y
   LDY   #$94
   LDA   (FP),Y
   LDY   #$4C
   STA   (FP),Y
   LDY   #$95
   LDA   (FP),Y
   LDY   #$4D
   STA   (FP),Y
   LDY   #$96
   LDA   (FP),Y
   LDY   #$4E
   STA   (FP),Y
   LDY   #$97
   LDA   (FP),Y
   LDY   #$4F
   STA   (FP),Y
   LDY   #$98
   LDA   (FP),Y
   LDY   #$50
   STA   (FP),Y
   LDY   #$99
   LDA   (FP),Y
   LDY   #$51
   STA   (FP),Y
   LDY   #$9A
   LDA   (FP),Y
   LDY   #$52
   STA   (FP),Y
   SEC
   LDA   #$00
   SBC   #$6A
   LDY   #$9B
   STA   (FP),Y
   LDA   #$00
   SBC   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$9D
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$9E
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@16
   LDA   #$00
   JMP   .sx_done@17
.sx_neg@16:
   LDA   #$FF
.sx_done@17:
   LDY   #$9F
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
   LDY   #$12
   LDA   (FP),Y
   LDY   #$A4
   CMP   (FP),Y
   BNE   .thread@0
   LDY   #$11
   LDA   (FP),Y
   LDY   #$A3
   CMP   (FP),Y
   BNE   .thread@0
   LDY   #$10
   LDA   (FP),Y
   LDY   #$A2
   CMP   (FP),Y
   BNE   .thread@0
   LDY   #$0F
   LDA   (FP),Y
   LDY   #$A1
   CMP   (FP),Y
   BNE   .thread@0
   LDY   #$0E
   LDA   (FP),Y
   LDY   #$A0
   CMP   (FP),Y
   BNE   .thread@0
   LDY   #$0D
   LDA   (FP),Y
   LDY   #$9F
   CMP   (FP),Y
   BNE   .thread@0
   LDY   #$0C
   LDA   (FP),Y
   LDY   #$9E
   CMP   (FP),Y
   BNE   .thread@0
   LDY   #$0B
   LDA   (FP),Y
   LDY   #$9D
   CMP   (FP),Y
   BEQ   .cmp_true@18
.thread@0:
   LDA   #$00
   JMP   .cmp_end@19
.cmp_true@18:
   LDA   #$01
.cmp_end@19:
   LDY   #$A5
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@3
   JMP   .and_false@6
.lb_skip@3:
   SEC
   LDA   #$00
   SBC   #$0A
   LDY   #$A7
   STA   (FP),Y
   LDA   #$00
   SBC   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$A9
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$AA
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@21
   LDA   #$00
   JMP   .sx_done@22
.sx_neg@21:
   LDA   #$FF
.sx_done@22:
   LDY   #$AB
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
   LDY   #$22
   LDA   (FP),Y
   LDY   #$B0
   CMP   (FP),Y
   BNE   .thread@1
   LDY   #$21
   LDA   (FP),Y
   LDY   #$AF
   CMP   (FP),Y
   BNE   .thread@1
   LDY   #$20
   LDA   (FP),Y
   LDY   #$AE
   CMP   (FP),Y
   BNE   .thread@1
   LDY   #$1F
   LDA   (FP),Y
   LDY   #$AD
   CMP   (FP),Y
   BNE   .thread@1
   LDY   #$1E
   LDA   (FP),Y
   LDY   #$AC
   CMP   (FP),Y
   BNE   .thread@1
   LDY   #$1D
   LDA   (FP),Y
   LDY   #$AB
   CMP   (FP),Y
   BNE   .thread@1
   LDY   #$1C
   LDA   (FP),Y
   LDY   #$AA
   CMP   (FP),Y
   BNE   .thread@1
   LDY   #$1B
   LDA   (FP),Y
   LDY   #$A9
   CMP   (FP),Y
   BEQ   .cmp_true@23
.thread@1:
   LDA   #$00
   JMP   .cmp_end@24
.cmp_true@23:
   LDA   #$01
.cmp_end@24:
   LDY   #$B1
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
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
   LDY   #$B3
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@7:
   LDY   #$B3
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@2
   JMP   .and_false@4
.lb_skip@2:
   LDA   #$0A
   LDY   #$B5
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   ORA   #$00
   BPL   .sx_done@27
   LDA   #$FF
.sx_done@27:
   LDY   #$B7
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
   LDY   #$32
   LDA   (FP),Y
   LDY   #$BC
   CMP   (FP),Y
   BNE   .thread@2
   LDY   #$31
   LDA   (FP),Y
   LDY   #$BB
   CMP   (FP),Y
   BNE   .thread@2
   LDY   #$30
   LDA   (FP),Y
   LDY   #$BA
   CMP   (FP),Y
   BNE   .thread@2
   LDY   #$2F
   LDA   (FP),Y
   LDY   #$B9
   CMP   (FP),Y
   BNE   .thread@2
   LDY   #$2E
   LDA   (FP),Y
   LDY   #$B8
   CMP   (FP),Y
   BNE   .thread@2
   LDY   #$2D
   LDA   (FP),Y
   LDY   #$B7
   CMP   (FP),Y
   BNE   .thread@2
   LDY   #$2C
   LDA   (FP),Y
   LDY   #$B6
   CMP   (FP),Y
   BNE   .thread@2
   LDY   #$2B
   LDA   (FP),Y
   LDY   #$B5
   CMP   (FP),Y
   BEQ   .cmp_true@28
.thread@2:
   LDA   #$00
   JMP   .cmp_end@29
.cmp_true@28:
   LDA   #$01
.cmp_end@29:
   LDY   #$BD
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@4
   LDA   #$01
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   JMP   .and_end@5
.and_false@4:
   LDA   #$00
   LDY   #$BF
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@5:
   LDY   #$BF
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@1
   JMP   .and_false@2
.lb_skip@1:
   LDA   #$00
   LDY   #$C1
   STA   (FP),Y
   LDA   #$2C
   INY
   STA   (FP),Y
   LDA   #$01
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   ORA   #$00
   BPL   .sx_done@32
   LDA   #$FF
.sx_done@32:
   LDY   #$C5
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$42
   LDA   (FP),Y
   LDY   #$C8
   CMP   (FP),Y
   BNE   .thread@3
   LDY   #$41
   LDA   (FP),Y
   LDY   #$C7
   CMP   (FP),Y
   BNE   .thread@3
   LDY   #$40
   LDA   (FP),Y
   LDY   #$C6
   CMP   (FP),Y
   BNE   .thread@3
   LDY   #$3F
   LDA   (FP),Y
   LDY   #$C5
   CMP   (FP),Y
   BNE   .thread@3
   LDY   #$3E
   LDA   (FP),Y
   LDY   #$C4
   CMP   (FP),Y
   BNE   .thread@3
   LDY   #$3D
   LDA   (FP),Y
   LDY   #$C3
   CMP   (FP),Y
   BNE   .thread@3
   LDY   #$3C
   LDA   (FP),Y
   LDY   #$C2
   CMP   (FP),Y
   BNE   .thread@3
   LDY   #$3B
   LDA   (FP),Y
   LDY   #$C1
   CMP   (FP),Y
   BEQ   .cmp_true@33
.thread@3:
   LDA   #$00
   JMP   .cmp_end@34
.cmp_true@33:
   LDA   #$01
.cmp_end@34:
   LDY   #$C9
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@2
   LDA   #$01
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   JMP   .and_end@3
.and_false@2:
   LDA   #$00
   LDY   #$CB
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@3:
   LDY   #$CB
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@0
   JMP   .and_false@0
.lb_skip@0:
   LDA   #$27
   LDY   #$CD
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   ORA   #$00
   BPL   .sx_done@37
   LDA   #$FF
.sx_done@37:
   LDY   #$CF
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
   LDY   #$52
   LDA   (FP),Y
   LDY   #$D4
   CMP   (FP),Y
   BNE   .thread@4
   LDY   #$51
   LDA   (FP),Y
   LDY   #$D3
   CMP   (FP),Y
   BNE   .thread@4
   LDY   #$50
   LDA   (FP),Y
   LDY   #$D2
   CMP   (FP),Y
   BNE   .thread@4
   LDY   #$4F
   LDA   (FP),Y
   LDY   #$D1
   CMP   (FP),Y
   BNE   .thread@4
   LDY   #$4E
   LDA   (FP),Y
   LDY   #$D0
   CMP   (FP),Y
   BNE   .thread@4
   LDY   #$4D
   LDA   (FP),Y
   LDY   #$CF
   CMP   (FP),Y
   BNE   .thread@4
   LDY   #$4C
   LDA   (FP),Y
   LDY   #$CE
   CMP   (FP),Y
   BNE   .thread@4
   LDY   #$4B
   LDA   (FP),Y
   LDY   #$CD
   CMP   (FP),Y
   BEQ   .cmp_true@38
.thread@4:
   LDA   #$00
   JMP   .cmp_end@39
.cmp_true@38:
   LDA   #$01
.cmp_end@39:
   LDY   #$D5
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@0
   LDA   #$01
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   JMP   .and_end@1
.and_false@0:
   LDA   #$00
   LDY   #$D7
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@1:
   LDY   #$D7
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@8
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   ORA   #$00
   BPL   .sx_done@42
   LDA   #$FF
.sx_done@42:
   LDY   #$DB
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
   LDY   #$D9
   LDA   (FP),Y
   STA   HARGS+16
   INY
   LDA   (FP),Y
   STA   HARGS+17
   INY
   LDA   (FP),Y
   STA   HARGS+18
   INY
   LDA   (FP),Y
   STA   HARGS+19
   INY
   LDA   (FP),Y
   STA   HARGS+20
   INY
   LDA   (FP),Y
   STA   HARGS+21
   INY
   LDA   (FP),Y
   STA   HARGS+22
   INY
   LDA   (FP),Y
   STA   HARGS+23

   ; epilogue
   CLC
   LDA   FP
   ADC   #$EA
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$E9
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   RTS
.if_end@8:
   LDA   #$01
   LDY   #$E1
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   ORA   #$00
   BPL   .sx_done@44
   LDA   #$FF
.sx_done@44:
   LDY   #$E3
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
   LDY   #$E1
   LDA   (FP),Y
   STA   HARGS+16
   INY
   LDA   (FP),Y
   STA   HARGS+17
   INY
   LDA   (FP),Y
   STA   HARGS+18
   INY
   LDA   (FP),Y
   STA   HARGS+19
   INY
   LDA   (FP),Y
   STA   HARGS+20
   INY
   LDA   (FP),Y
   STA   HARGS+21
   INY
   LDA   (FP),Y
   STA   HARGS+22
   INY
   LDA   (FP),Y
   STA   HARGS+23

   ; epilogue
   CLC
   LDA   FP
   ADC   #$EA
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
   RTS

main:
   SUBROUTINE

   ; prologue: 0 arg bytes, 8 local bytes
   SEC
   LDA   SSP
   SBC   #$0A
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$09
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   JSR   target
   LDA   HARGS+16
   LDY   #$01
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
   LDA   HARGS+23
   INY
   STA   (FP),Y
   LDY   #$01
   LDA   (FP),Y
   STA   HARGS+16
   INY
   LDA   (FP),Y
   STA   HARGS+17
   INY
   LDA   (FP),Y
   STA   HARGS+18
   INY
   LDA   (FP),Y
   STA   HARGS+19
   INY
   LDA   (FP),Y
   STA   HARGS+20
   INY
   LDA   (FP),Y
   STA   HARGS+21
   INY
   LDA   (FP),Y
   STA   HARGS+22
   INY
   LDA   (FP),Y
   STA   HARGS+23

   ; epilogue
   CLC
   LDA   FP
   ADC   #$0A
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
   RTS
