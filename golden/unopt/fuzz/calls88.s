SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24

f5:
   SUBROUTINE

   ; prologue: 19 arg bytes, 99 local bytes
   SEC
   LDA   SSP
   SBC   #$65
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$64
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   LDY   #$6C
   LDA   (FP),Y
   BEQ   .and_false@0
   LDY   #$73
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
   BEQ   .and_false@0
   LDA   #$01
   LDY   #$01
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   JMP   .and_end@1
.and_false@0:
   LDA   #$00
   LDY   #$01
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@1:
   LDY   #$01
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
   LDY   #$6D
   LDA   (FP),Y
   SEC
   LDY   #$03
   SBC   (FP),Y
   LDY   #$6E
   LDA   (FP),Y
   LDY   #$04
   SBC   (FP),Y
   LDY   #$6F
   LDA   (FP),Y
   LDY   #$05
   SBC   (FP),Y
   LDY   #$70
   LDA   (FP),Y
   LDY   #$06
   SBC   (FP),Y
   LDY   #$71
   LDA   (FP),Y
   LDY   #$07
   SBC   (FP),Y
   LDY   #$72
   LDA   (FP),Y
   LDY   #$08
   SBC   (FP),Y
   BVC   .cmp_novf@2
   EOR   #$80
.cmp_novf@2:
   ASL
   LDA   #$00
   ROL
   LDY   #$09
   STA   (FP),Y
   LDA   #$00
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
   DEY
   LDA   (FP),Y
   LDY   #$0D
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$0E
   STA   (FP),Y
   LDA   #$64
   LDY   #$6D
   EOR   (FP),Y
   LDY   #$0F
   STA   (FP),Y
   LDA   #$00
   LDY   #$6E
   EOR   (FP),Y
   LDY   #$10
   STA   (FP),Y
   LDA   #$00
   LDY   #$6F
   EOR   (FP),Y
   LDY   #$11
   STA   (FP),Y
   LDA   #$00
   LDY   #$70
   EOR   (FP),Y
   LDY   #$12
   STA   (FP),Y
   LDA   #$00
   LDY   #$71
   EOR   (FP),Y
   LDY   #$13
   STA   (FP),Y
   LDA   #$00
   LDY   #$72
   EOR   (FP),Y
   LDY   #$14
   STA   (FP),Y
   LDY   #$66
   LDA   (FP),Y
   AND   #$07
   LDY   #$15
   STA   (FP),Y
   LDY   #$67
   LDA   (FP),Y
   LDA   #$00
   LDY   #$16
   STA   (FP),Y
   LDY   #$68
   LDA   (FP),Y
   LDA   #$00
   LDY   #$17
   STA   (FP),Y
   LDY   #$69
   LDA   (FP),Y
   LDA   #$00
   LDY   #$18
   STA   (FP),Y
   LDY   #$6A
   LDA   (FP),Y
   LDA   #$00
   LDY   #$19
   STA   (FP),Y
   LDY   #$6B
   LDA   (FP),Y
   LDA   #$00
   LDY   #$1A
   STA   (FP),Y
   LDY   #$6D
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
   ASL
   LDA   #$00
   ADC   #$FF
   EOR   #$FF
   STA   HARGS+6
   STA   HARGS+7
   LDY   #$15
   LDA   (FP),Y
   STA   HARGS+8
   JSR   asr64
   LDA   HARGS+9
   LDY   #$1B
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
   LDY   #$1B
   LDA   (FP),Y
   ORA   #$01
   LDY   #$21
   STA   (FP),Y
   LDY   #$1C
   LDA   (FP),Y
   LDY   #$22
   STA   (FP),Y
   LDY   #$1D
   LDA   (FP),Y
   LDY   #$23
   STA   (FP),Y
   LDY   #$1E
   LDA   (FP),Y
   LDY   #$24
   STA   (FP),Y
   LDY   #$1F
   LDA   (FP),Y
   LDY   #$25
   STA   (FP),Y
   LDY   #$20
   LDA   (FP),Y
   LDY   #$26
   STA   (FP),Y
   LDY   #$0F
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
   ASL
   LDA   #$00
   ADC   #$FF
   EOR   #$FF
   STA   HARGS+6
   STA   HARGS+7
   LDY   #$21
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
   ASL
   LDA   #$00
   ADC   #$FF
   EOR   #$FF
   STA   HARGS+14
   STA   HARGS+15
   JSR   sdivmod64
   LDA   HARGS+24
   LDY   #$27
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
   LDA   HARGS+29
   INY
   STA   (FP),Y
   LDY   #$27
   LDA   (FP),Y
   LDY   #$2D
   STA   (FP),Y
   LDY   #$28
   LDA   (FP),Y
   LDY   #$2E
   STA   (FP),Y
   LDY   #$29
   LDA   (FP),Y
   LDY   #$2F
   STA   (FP),Y
   LDY   #$2A
   LDA   (FP),Y
   LDY   #$30
   STA   (FP),Y
   LDY   #$2B
   LDA   (FP),Y
   LDY   #$31
   STA   (FP),Y
   LDY   #$2C
   LDA   (FP),Y
   LDY   #$32
   STA   (FP),Y
   LDY   #$73
   LDA   (FP),Y
   ORA   #$01
   LDY   #$33
   STA   (FP),Y
   LDY   #$74
   LDA   (FP),Y
   LDY   #$34
   STA   (FP),Y
   LDY   #$75
   LDA   (FP),Y
   LDY   #$35
   STA   (FP),Y
   LDY   #$76
   LDA   (FP),Y
   LDY   #$36
   STA   (FP),Y
   LDY   #$77
   LDA   (FP),Y
   LDY   #$37
   STA   (FP),Y
   LDY   #$78
   LDA   (FP),Y
   LDY   #$38
   STA   (FP),Y
   LDY   #$66
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
   ASL
   LDA   #$00
   ADC   #$FF
   EOR   #$FF
   STA   HARGS+6
   STA   HARGS+7
   LDY   #$33
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
   ASL
   LDA   #$00
   ADC   #$FF
   EOR   #$FF
   STA   HARGS+14
   STA   HARGS+15
   JSR   sdivmod64
   LDA   HARGS+24
   LDY   #$39
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
   LDA   HARGS+29
   INY
   STA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@3
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@3
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@3
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@3
   DEY
   LDA   (FP),Y
   CMP   #$7F
   BNE   .cmp_true@3
   DEY
   LDA   (FP),Y
   CMP   #$FF
   BNE   .cmp_true@3
   LDA   #$00
   JMP   .cmp_end@4
.cmp_true@3:
   LDA   #$01
.cmp_end@4:
   LDY   #$3F
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$41
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$E8
   SEC
   LDY   #$66
   SBC   (FP),Y
   LDY   #$43
   STA   (FP),Y
   LDA   #$03
   LDY   #$67
   SBC   (FP),Y
   LDY   #$44
   STA   (FP),Y
   LDA   #$00
   LDY   #$68
   SBC   (FP),Y
   LDY   #$45
   STA   (FP),Y
   LDA   #$00
   LDY   #$69
   SBC   (FP),Y
   LDY   #$46
   STA   (FP),Y
   LDA   #$00
   LDY   #$6A
   SBC   (FP),Y
   LDY   #$47
   STA   (FP),Y
   LDA   #$00
   LDY   #$6B
   SBC   (FP),Y
   LDY   #$48
   STA   (FP),Y
   LDY   #$43
   LDA   (FP),Y
   LDY   #$49
   STA   (FP),Y
   LDY   #$44
   LDA   (FP),Y
   LDY   #$4A
   STA   (FP),Y
   LDY   #$45
   LDA   (FP),Y
   LDY   #$4B
   STA   (FP),Y
   LDY   #$46
   LDA   (FP),Y
   LDY   #$4C
   STA   (FP),Y
   LDY   #$47
   LDA   (FP),Y
   LDY   #$4D
   STA   (FP),Y
   LDY   #$48
   LDA   (FP),Y
   AND   #$1F
   LDY   #$4E
   STA   (FP),Y
   LDY   #$49
   LDA   (FP),Y
   LDY   #$4F
   STA   (FP),Y
   LDY   #$4A
   LDA   (FP),Y
   LDY   #$50
   STA   (FP),Y
   LDY   #$4B
   LDA   (FP),Y
   LDY   #$51
   STA   (FP),Y
   LDY   #$4C
   LDA   (FP),Y
   LDY   #$52
   STA   (FP),Y
   LDY   #$4D
   LDA   (FP),Y
   LDY   #$53
   STA   (FP),Y
   LDY   #$4E
   LDA   (FP),Y
   EOR   #$10
   LDY   #$54
   STA   (FP),Y
   LDY   #$4F
   LDA   (FP),Y
   LDY   #$55
   STA   (FP),Y
   LDY   #$50
   LDA   (FP),Y
   LDY   #$56
   STA   (FP),Y
   LDY   #$51
   LDA   (FP),Y
   LDY   #$57
   STA   (FP),Y
   LDY   #$52
   LDA   (FP),Y
   LDY   #$58
   STA   (FP),Y
   LDY   #$53
   LDA   (FP),Y
   LDY   #$59
   STA   (FP),Y
   LDY   #$54
   LDA   (FP),Y
   SEC
   SBC   #$10
   LDY   #$5A
   STA   (FP),Y
   LDY   #$55
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
   BEQ   .and_false@2
   LDY   #$6D
   LDA   (FP),Y
   EOR   #$FF
   LDY   #$5B
   STA   (FP),Y
   LDY   #$6E
   LDA   (FP),Y
   EOR   #$FF
   LDY   #$5C
   STA   (FP),Y
   LDY   #$6F
   LDA   (FP),Y
   EOR   #$FF
   LDY   #$5D
   STA   (FP),Y
   LDY   #$70
   LDA   (FP),Y
   EOR   #$FF
   LDY   #$5E
   STA   (FP),Y
   LDY   #$71
   LDA   (FP),Y
   EOR   #$FF
   LDY   #$5F
   STA   (FP),Y
   LDY   #$72
   LDA   (FP),Y
   EOR   #$FF
   LDY   #$60
   STA   (FP),Y
   LDY   #$5B
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
   LDY   #$61
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@3:
   LDY   #$61
   LDA   (FP),Y
   LDY   #$63
   STA   (FP),Y
   CMP   #$01

   ; epilogue
   PHP
   CLC
   LDA   FP
   ADC   #$78
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
   PLP
   RTS

f4:
   SUBROUTINE

   ; prologue: 6 arg bytes, 130 local bytes
   SEC
   LDA   SSP
   SBC   #$84
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$83
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
   ORA   #$00
   BPL   .sx_done@7
   LDA   #$FF
.sx_done@7:
   LDY   #$03
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   g0
   SEC
   LDY   #$01
   SBC   (FP),Y
   LDA   g0+1
   INY
   SBC   (FP),Y
   LDA   g0+2
   INY
   SBC   (FP),Y
   LDA   g0+3
   INY
   SBC   (FP),Y
   LDA   #$00
   ROL
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$87
   LDA   (FP),Y
   AND   #$07
   LDY   #$07
   STA   (FP),Y
   LDY   #$88
   LDA   (FP),Y
   LDA   #$00
   LDY   #$08
   STA   (FP),Y
   LDA   #$64
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$07
   LDA   (FP),Y
   TAX
   BEQ   .shift_done@9
.shift_loop@8:
   LDY   #$0A
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
   BNE   .shift_loop@8
.shift_done@9:
   LDY   #$05
   LDA   (FP),Y
   LDY   #$09
   ORA   (FP),Y
   LDY   #$0B
   STA   (FP),Y
   LDY   #$06
   LDA   (FP),Y
   LDY   #$0A
   ORA   (FP),Y
   LDY   #$0C
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$0D
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$0E
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$0F
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$10
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$03
   LDY   #$11
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$12
   STA   (FP),Y
   LDY   #$85
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lnot_true@10
   LDA   #$00
   JMP   .lnot_end@11
.lnot_true@10:
   LDA   #$01
.lnot_end@11:
   LDY   #$13
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$15
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$16
   STA   (FP),Y
   LDY   #$12
   LDA   (FP),Y
   LDY   #$16
   CMP   (FP),Y
   BNE   .thread@0
   LDY   #$11
   LDA   (FP),Y
   LDY   #$15
   CMP   (FP),Y
   BEQ   .cmp_true@12
.thread@0:
   LDA   #$00
   JMP   .cmp_end@13
.cmp_true@12:
   LDA   #$01
.cmp_end@13:
   LDY   #$17
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$19
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$1A
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@15
   LDA   #$00
   JMP   .sx_done@16
.sx_neg@15:
   LDA   #$FF
.sx_done@16:
   LDY   #$1B
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$19
   LDA   (FP),Y
   LDY   #$1D
   STA   (FP),Y
   LDY   #$1A
   LDA   (FP),Y
   LDY   #$1E
   STA   (FP),Y
   LDY   #$1B
   LDA   (FP),Y
   LDY   #$1F
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$87
   LDA   (FP),Y
   AND   #$07
   LDY   #$21
   STA   (FP),Y
   LDY   #$88
   LDA   (FP),Y
   LDA   #$00
   LDY   #$22
   STA   (FP),Y
   LDY   #$1D
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
   JSR   lsr32
   LDA   HARGS+5
   LDY   #$23
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
   LDY   #$23
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   INY
   ORA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@4
   LDY   #$1D
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   INY
   ORA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@4
   LDA   #$01
   LDY   #$27
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   JMP   .and_end@5
.and_false@4:
   LDA   #$00
   LDY   #$27
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@5:
   LDY   #$27
   LDA   (FP),Y
   LDY   #$29
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$2A
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@17
   LDA   #$00
   JMP   .sx_done@18
.sx_neg@17:
   LDA   #$FF
.sx_done@18:
   LDY   #$2B
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$29
   LDA   (FP),Y
   LDY   #$2F
   STA   (FP),Y
   LDY   #$2A
   LDA   (FP),Y
   LDY   #$30
   STA   (FP),Y
   LDY   #$2B
   LDA   (FP),Y
   LDY   #$31
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$87
   LDA   (FP),Y
   LDY   #$35
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
.loop@0_start:
   LDY   #$37
   LDA   (FP),Y
   SEC
   SBC   #$03
   INY
   LDA   (FP),Y
   SBC   #$00
   BVC   .cmp_novf@19
   EOR   #$80
.cmp_novf@19:
   ASL
   LDA   #$00
   ROL
   LDY   #$39
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@0
   JMP   .loop@0_break
.lb_skip@0:
   LDA   #$03
   SEC
   LDY   #$0F
   SBC   (FP),Y
   LDA   #$00
   INY
   SBC   (FP),Y
   LDA   #$00
   ROL
   LDY   #$3B
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$3D
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$3E
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@20
   LDA   #$00
   JMP   .sx_done@21
.sx_neg@20:
   LDA   #$FF
.sx_done@21:
   LDY   #$3F
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$87
   LDA   (FP),Y
   LDY   #$43
   STA   (FP),Y
   LDY   #$88
   LDA   (FP),Y
   LDY   #$44
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$43
   LDA   (FP),Y
   SEC
   SBC   g0
   INY
   LDA   (FP),Y
   SBC   g0+1
   INY
   LDA   (FP),Y
   SBC   g0+2
   INY
   LDA   (FP),Y
   SBC   g0+3
   LDA   #$00
   ROL
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$49
   STA   (FP),Y
   LDY   #$89
   LDA   (FP),Y
   LDY   #$4A
   STA   (FP),Y
   LDY   #$8A
   LDA   (FP),Y
   LDY   #$4B
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@22
   LDA   #$00
   JMP   .sx_done@23
.sx_neg@22:
   LDA   #$FF
.sx_done@23:
   LDY   #$4C
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$36
   LDA   (FP),Y
   LDY   #$50
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@24
   LDA   #$00
   JMP   .sx_done@25
.sx_neg@24:
   LDA   #$FF
.sx_done@25:
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
   ORA   #$00
   BMI   .sx_neg@26
   LDA   #$00
   JMP   .sx_done@27
.sx_neg@26:
   LDA   #$FF
.sx_done@27:
   LDY   #$54
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$2F
   LDA   (FP),Y
   SEC
   LDY   #$52
   SBC   (FP),Y
   LDY   #$30
   LDA   (FP),Y
   LDY   #$53
   SBC   (FP),Y
   LDY   #$31
   LDA   (FP),Y
   LDY   #$54
   SBC   (FP),Y
   LDY   #$32
   LDA   (FP),Y
   LDY   #$55
   SBC   (FP),Y
   LDY   #$33
   LDA   (FP),Y
   LDY   #$56
   SBC   (FP),Y
   LDY   #$34
   LDA   (FP),Y
   LDY   #$57
   SBC   (FP),Y
   BVC   .cmp_novf@28
   EOR   #$80
.cmp_novf@28:
   ASL
   LDA   #$00
   ROL
   EOR   #$01
   LDY   #$58
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$5A
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$5B
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@29
   LDA   #$00
   JMP   .sx_done@30
.sx_neg@29:
   LDA   #$FF
.sx_done@30:
   LDY   #$5C
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$13
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDY   #$3D
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$3E
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$3F
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$40
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   LDY   #$41
   LDA   (FP),Y
   LDY   #$05
   STA   (SSP),Y
   LDY   #$42
   LDA   (FP),Y
   LDY   #$06
   STA   (SSP),Y
   LDY   #$49
   LDA   (FP),Y
   LDY   #$07
   STA   (SSP),Y
   LDY   #$4A
   LDA   (FP),Y
   LDY   #$08
   STA   (SSP),Y
   LDY   #$4B
   LDA   (FP),Y
   LDY   #$09
   STA   (SSP),Y
   LDY   #$4C
   LDA   (FP),Y
   LDY   #$0A
   STA   (SSP),Y
   LDY   #$4D
   LDA   (FP),Y
   LDY   #$0B
   STA   (SSP),Y
   LDY   #$4E
   LDA   (FP),Y
   LDY   #$0C
   STA   (SSP),Y
   LDY   #$4F
   LDA   (FP),Y
   LDY   #$0D
   STA   (SSP),Y
   LDY   #$5A
   LDA   (FP),Y
   LDY   #$0E
   STA   (SSP),Y
   LDY   #$5B
   LDA   (FP),Y
   LDY   #$0F
   STA   (SSP),Y
   LDY   #$5C
   LDA   (FP),Y
   LDY   #$10
   STA   (SSP),Y
   LDY   #$5D
   LDA   (FP),Y
   LDY   #$11
   STA   (SSP),Y
   LDY   #$5E
   LDA   (FP),Y
   LDY   #$12
   STA   (SSP),Y
   LDY   #$5F
   LDA   (FP),Y
   LDY   #$13
   STA   (SSP),Y
   JSR   f5
   LDA   #$00
   ROL
   LDY   #$60
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   LDY   #$37
   ADC   (FP),Y
   LDY   #$63
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$38
   ADC   (FP),Y
   LDY   #$64
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$65
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$66
   STA   (FP),Y
   LDY   #$0F
   LDA   (FP),Y
   CLC
   LDY   #$65
   ADC   (FP),Y
   LDY   #$67
   STA   (FP),Y
   LDY   #$10
   LDA   (FP),Y
   LDY   #$66
   ADC   (FP),Y
   LDY   #$68
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$0F
   STA   (FP),Y
   LDY   #$68
   LDA   (FP),Y
   LDY   #$10
   STA   (FP),Y
   LDY   #$37
   LDA   (FP),Y
   LDY   #$69
   STA   (FP),Y
   LDY   #$38
   LDA   (FP),Y
   LDY   #$6A
   STA   (FP),Y
   LDY   #$37
   LDA   (FP),Y
   CLC
   ADC   #$01
   LDY   #$6B
   STA   (FP),Y
   LDY   #$38
   LDA   (FP),Y
   ADC   #$00
   LDY   #$6C
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$37
   STA   (FP),Y
   LDY   #$6C
   LDA   (FP),Y
   LDY   #$38
   STA   (FP),Y
   JMP   .loop@0_start
.loop@0_break:
   LDY   #$0F
   LDA   (FP),Y
   LDY   #$6D
   STA   (FP),Y
   LDY   #$10
   LDA   (FP),Y
   LDY   #$6E
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   g0
   CLC
   LDY   #$6D
   ADC   (FP),Y
   LDY   #$71
   STA   (FP),Y
   LDA   g0+1
   LDY   #$6E
   ADC   (FP),Y
   LDY   #$72
   STA   (FP),Y
   LDA   g0+2
   LDY   #$6F
   ADC   (FP),Y
   LDY   #$73
   STA   (FP),Y
   LDA   g0+3
   LDY   #$70
   ADC   (FP),Y
   LDY   #$74
   STA   (FP),Y
   LDY   #$71
   LDA   (FP),Y
   STA   g0
   INY
   LDA   (FP),Y
   STA   g0+1
   INY
   LDA   (FP),Y
   STA   g0+2
   INY
   LDA   (FP),Y
   STA   g0+3
   LDY   #$36
   LDA   (FP),Y
   LDY   #$75
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@31
   LDA   #$00
   JMP   .sx_done@32
.sx_neg@31:
   LDA   #$FF
.sx_done@32:
   LDY   #$76
   STA   (FP),Y
   LDA   #$01
   DEY
   ORA   (FP),Y
   LDY   #$77
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$78
   STA   (FP),Y
   LDY   #$89
   LDA   (FP),Y
   LDY   #$79
   STA   (FP),Y
   LDY   #$8A
   LDA   (FP),Y
   LDY   #$7A
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@33
   LDA   #$00
   JMP   .sx_done@34
.sx_neg@33:
   LDA   #$FF
.sx_done@34:
   LDY   #$7B
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$79
   LDA   (FP),Y
   SEC
   SBC   g0
   INY
   LDA   (FP),Y
   SBC   g0+1
   INY
   LDA   (FP),Y
   SBC   g0+2
   INY
   LDA   (FP),Y
   SBC   g0+3
   LDA   #$00
   ROL
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$77
   LDA   (FP),Y
   CLC
   LDY   #$7D
   ADC   (FP),Y
   LDY   #$7F
   STA   (FP),Y
   LDY   #$78
   LDA   (FP),Y
   LDY   #$7E
   ADC   (FP),Y
   LDY   #$80
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$81
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$82
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
   ADC   #$8A
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$83
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

f3:
   SUBROUTINE

   ; prologue: 2 arg bytes, 206 local bytes
   SEC
   LDA   SSP
   SBC   #$D0
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$CF
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   LDA   g0
   ORA   g0+1
   ORA   g0+2
   ORA   g0+3
   BEQ   .lnot_true@35
   LDA   #$00
   JMP   .lnot_end@36
.lnot_true@35:
   LDA   #$01
.lnot_end@36:
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
   DEY
   LDA   (FP),Y
   LDY   #$05
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$06
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   AND   #$07
   LDY   #$07
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDA   #$00
   LDY   #$08
   STA   (FP),Y
   LDY   #$05
   LDA   (FP),Y
   LDY   #$09
   STA   (FP),Y
   LDY   #$06
   LDA   (FP),Y
   LDY   #$0A
   STA   (FP),Y
   LDY   #$07
   LDA   (FP),Y
   TAX
   BEQ   .shift_done@38
.shift_loop@37:
   LDY   #$0A
   LDA   (FP),Y
   LSR
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ROR
   STA   (FP),Y
   DEX
   BNE   .shift_loop@37
.shift_done@38:
   LDY   #$06
   LDA   (FP),Y
   CMP   (FP),Y
   BNE   .thread@1
   DEY
   LDA   (FP),Y
   CMP   (FP),Y
   BEQ   .cmp_true@39
.thread@1:
   LDA   #$00
   JMP   .cmp_end@40
.cmp_true@39:
   LDA   #$01
.cmp_end@40:
   LDY   #$0B
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$0D
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$0E
   STA   (FP),Y
   LDY   #$09
   LDA   (FP),Y
   CLC
   LDY   #$0D
   SBC   (FP),Y
   LDY   #$0A
   LDA   (FP),Y
   LDY   #$0E
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
   LDY   #$11
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$12
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@42
   LDA   #$00
   JMP   .sx_done@43
.sx_neg@42:
   LDA   #$FF
.sx_done@43:
   LDY   #$13
   STA   (FP),Y
   INY
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
   INY
   STA   (FP),Y
   LDY   #$05
   LDA   (FP),Y
   LDY   #$19
   STA   (FP),Y
   LDY   #$06
   LDA   (FP),Y
   LDY   #$1A
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   g0
   CLC
   LDY   #$19
   SBC   (FP),Y
   LDA   g0+1
   INY
   SBC   (FP),Y
   LDA   g0+2
   INY
   SBC   (FP),Y
   LDA   g0+3
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
   LDY   #$1F
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$20
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@44
   LDA   #$00
   JMP   .sx_done@45
.sx_neg@44:
   LDA   #$FF
.sx_done@45:
   LDY   #$21
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$15
   LDA   (FP),Y
   LDY   #$1F
   AND   (FP),Y
   LDY   #$23
   STA   (FP),Y
   LDY   #$16
   LDA   (FP),Y
   LDY   #$20
   AND   (FP),Y
   LDY   #$24
   STA   (FP),Y
   LDY   #$17
   LDA   (FP),Y
   LDY   #$21
   AND   (FP),Y
   LDY   #$25
   STA   (FP),Y
   LDY   #$18
   LDA   (FP),Y
   LDY   #$22
   AND   (FP),Y
   LDY   #$26
   STA   (FP),Y
   LDY   #$23
   LDA   (FP),Y
   LDY   #$27
   STA   (FP),Y
   LDY   #$24
   LDA   (FP),Y
   LDY   #$28
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$29
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$2A
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
   INY
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@46
   LDA   #$00
   JMP   .sx_done@47
.sx_neg@46:
   LDA   #$FF
.sx_done@47:
   LDY   #$2F
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$2B
   LDA   (FP),Y
   LDY   #$31
   STA   (FP),Y
   LDY   #$2C
   LDA   (FP),Y
   LDY   #$32
   STA   (FP),Y
   LDY   #$2D
   LDA   (FP),Y
   LDY   #$33
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$2F
   LDA   (FP),Y
   LDY   #$35
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$05
   LDA   (FP),Y
   LDY   #$37
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$D1
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
   LDY   #$29
   LDA   (FP),Y
   LDY   #$3B
   STA   (FP),Y
   LDY   #$2A
   LDA   (FP),Y
   LDY   #$3C
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@48
   LDA   #$00
   JMP   .sx_done@49
.sx_neg@48:
   LDA   #$FF
.sx_done@49:
   LDY   #$3D
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$15
   LDA   (FP),Y
   SEC
   LDY   #$3B
   SBC   (FP),Y
   LDY   #$16
   LDA   (FP),Y
   LDY   #$3C
   SBC   (FP),Y
   LDY   #$17
   LDA   (FP),Y
   LDY   #$3D
   SBC   (FP),Y
   LDY   #$18
   LDA   (FP),Y
   LDY   #$3E
   SBC   (FP),Y
   BVC   .cmp_novf@50
   EOR   #$80
.cmp_novf@50:
   ASL
   LDA   #$00
   ROL
   LDY   #$3F
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$39
   LDA   (FP),Y
   CLC
   LDY   #$3F
   ADC   (FP),Y
   LDY   #$41
   STA   (FP),Y
   LDY   #$3A
   LDA   (FP),Y
   LDY   #$40
   ADC   (FP),Y
   LDY   #$42
   STA   (FP),Y
   LDY   #$D1
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$41
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$42
   LDA   (FP),Y
   LDY   #$01
   STA   (DPTR),Y
   LDA   #$00
   LDY   #$43
   STA   (FP),Y
   INY
   STA   (FP),Y
.loop@1_start:
   LDY   #$43
   LDA   (FP),Y
   SEC
   SBC   #$03
   INY
   LDA   (FP),Y
   SBC   #$00
   BVC   .cmp_novf@51
   EOR   #$80
.cmp_novf@51:
   ASL
   LDA   #$00
   ROL
   LDY   #$45
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@1
   JMP   .loop@1_break
.lb_skip@1:
   LDA   g0
   LDY   #$47
   STA   (FP),Y
   LDA   g0+1
   INY
   STA   (FP),Y
   LDA   g0+2
   INY
   STA   (FP),Y
   LDA   g0+3
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$31
   LDA   (FP),Y
   LDY   #$4D
   STA   (FP),Y
   LDY   #$38
   LDA   (FP),Y
   LDY   #$4E
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$50
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$51
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@52
   LDA   #$00
   JMP   .sx_done@53
.sx_neg@52:
   LDA   #$FF
.sx_done@53:
   LDY   #$52
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$31
   LDA   (FP),Y
   LDY   #$50
   EOR   (FP),Y
   LDY   #$56
   STA   (FP),Y
   LDY   #$32
   LDA   (FP),Y
   LDY   #$51
   EOR   (FP),Y
   LDY   #$57
   STA   (FP),Y
   LDY   #$33
   LDA   (FP),Y
   LDY   #$52
   EOR   (FP),Y
   LDY   #$58
   STA   (FP),Y
   LDY   #$34
   LDA   (FP),Y
   LDY   #$53
   EOR   (FP),Y
   LDY   #$59
   STA   (FP),Y
   LDY   #$35
   LDA   (FP),Y
   LDY   #$54
   EOR   (FP),Y
   LDY   #$5A
   STA   (FP),Y
   LDY   #$36
   LDA   (FP),Y
   LDY   #$55
   EOR   (FP),Y
   LDY   #$5B
   STA   (FP),Y
   LDY   #$29
   LDA   (FP),Y
   LDY   #$5C
   STA   (FP),Y
   LDY   #$2A
   LDA   (FP),Y
   LDY   #$5D
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@54
   LDA   #$00
   JMP   .sx_done@55
.sx_neg@54:
   LDA   #$FF
.sx_done@55:
   LDY   #$5E
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$13
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDY   #$47
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$48
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$49
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$4A
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   LDY   #$4B
   LDA   (FP),Y
   LDY   #$05
   STA   (SSP),Y
   LDY   #$4C
   LDA   (FP),Y
   LDY   #$06
   STA   (SSP),Y
   LDY   #$4D
   LDA   (FP),Y
   LDY   #$07
   STA   (SSP),Y
   LDY   #$56
   LDA   (FP),Y
   LDY   #$08
   STA   (SSP),Y
   LDY   #$57
   LDA   (FP),Y
   LDY   #$09
   STA   (SSP),Y
   LDY   #$58
   LDA   (FP),Y
   LDY   #$0A
   STA   (SSP),Y
   LDY   #$59
   LDA   (FP),Y
   LDY   #$0B
   STA   (SSP),Y
   LDY   #$5A
   LDA   (FP),Y
   LDY   #$0C
   STA   (SSP),Y
   LDY   #$5B
   LDA   (FP),Y
   LDY   #$0D
   STA   (SSP),Y
   LDY   #$5C
   LDA   (FP),Y
   LDY   #$0E
   STA   (SSP),Y
   LDY   #$5D
   LDA   (FP),Y
   LDY   #$0F
   STA   (SSP),Y
   LDY   #$5E
   LDA   (FP),Y
   LDY   #$10
   STA   (SSP),Y
   LDY   #$5F
   LDA   (FP),Y
   LDY   #$11
   STA   (SSP),Y
   LDY   #$60
   LDA   (FP),Y
   LDY   #$12
   STA   (SSP),Y
   LDY   #$61
   LDA   (FP),Y
   LDY   #$13
   STA   (SSP),Y
   JSR   f5
   LDA   #$00
   ROL
   LDY   #$62
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   LDY   #$43
   ADC   (FP),Y
   LDY   #$65
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$44
   ADC   (FP),Y
   LDY   #$66
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$67
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$68
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@56
   LDA   #$00
   JMP   .sx_done@57
.sx_neg@56:
   LDA   #$FF
.sx_done@57:
   LDY   #$69
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$31
   LDA   (FP),Y
   CLC
   LDY   #$67
   ADC   (FP),Y
   LDY   #$6D
   STA   (FP),Y
   LDY   #$32
   LDA   (FP),Y
   LDY   #$68
   ADC   (FP),Y
   LDY   #$6E
   STA   (FP),Y
   LDY   #$33
   LDA   (FP),Y
   LDY   #$69
   ADC   (FP),Y
   LDY   #$6F
   STA   (FP),Y
   LDY   #$34
   LDA   (FP),Y
   LDY   #$6A
   ADC   (FP),Y
   LDY   #$70
   STA   (FP),Y
   LDY   #$35
   LDA   (FP),Y
   LDY   #$6B
   ADC   (FP),Y
   LDY   #$71
   STA   (FP),Y
   LDY   #$36
   LDA   (FP),Y
   LDY   #$6C
   ADC   (FP),Y
   LDY   #$72
   STA   (FP),Y
   LDY   #$6D
   LDA   (FP),Y
   LDY   #$73
   STA   (FP),Y
   LDY   #$6E
   LDA   (FP),Y
   LDY   #$74
   STA   (FP),Y
   LDY   #$6F
   LDA   (FP),Y
   LDY   #$75
   STA   (FP),Y
   LDY   #$70
   LDA   (FP),Y
   LDY   #$76
   STA   (FP),Y
   LDY   #$71
   LDA   (FP),Y
   LDY   #$77
   STA   (FP),Y
   LDY   #$72
   LDA   (FP),Y
   AND   #$1F
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
   EOR   #$10
   LDY   #$7E
   STA   (FP),Y
   LDY   #$79
   LDA   (FP),Y
   LDY   #$7F
   STA   (FP),Y
   LDY   #$7A
   LDA   (FP),Y
   LDY   #$80
   STA   (FP),Y
   LDY   #$7B
   LDA   (FP),Y
   LDY   #$81
   STA   (FP),Y
   LDY   #$7C
   LDA   (FP),Y
   LDY   #$82
   STA   (FP),Y
   LDY   #$7D
   LDA   (FP),Y
   LDY   #$83
   STA   (FP),Y
   LDY   #$7E
   LDA   (FP),Y
   SEC
   SBC   #$10
   LDY   #$84
   STA   (FP),Y
   LDY   #$7F
   LDA   (FP),Y
   LDY   #$31
   STA   (FP),Y
   LDY   #$80
   LDA   (FP),Y
   LDY   #$32
   STA   (FP),Y
   LDY   #$81
   LDA   (FP),Y
   LDY   #$33
   STA   (FP),Y
   LDY   #$82
   LDA   (FP),Y
   LDY   #$34
   STA   (FP),Y
   LDY   #$83
   LDA   (FP),Y
   LDY   #$35
   STA   (FP),Y
   LDY   #$84
   LDA   (FP),Y
   LDY   #$36
   STA   (FP),Y
   LDY   #$43
   LDA   (FP),Y
   LDY   #$85
   STA   (FP),Y
   LDY   #$44
   LDA   (FP),Y
   LDY   #$86
   STA   (FP),Y
   LDY   #$43
   LDA   (FP),Y
   CLC
   ADC   #$01
   LDY   #$87
   STA   (FP),Y
   LDY   #$44
   LDA   (FP),Y
   ADC   #$00
   LDY   #$88
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$43
   STA   (FP),Y
   LDY   #$88
   LDA   (FP),Y
   LDY   #$44
   STA   (FP),Y
   JMP   .loop@1_start
.loop@1_break:
   LDA   g0
   LDY   #$89
   STA   (FP),Y
   LDA   g0+1
   INY
   STA   (FP),Y
   LDA   g0+2
   INY
   STA   (FP),Y
   LDA   g0+3
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$89
   LDA   (FP),Y
   CLC
   LDY   #$31
   ADC   (FP),Y
   LDY   #$8F
   STA   (FP),Y
   LDY   #$8A
   LDA   (FP),Y
   LDY   #$32
   ADC   (FP),Y
   LDY   #$90
   STA   (FP),Y
   LDY   #$8B
   LDA   (FP),Y
   LDY   #$33
   ADC   (FP),Y
   LDY   #$91
   STA   (FP),Y
   LDY   #$8C
   LDA   (FP),Y
   LDY   #$34
   ADC   (FP),Y
   LDY   #$92
   STA   (FP),Y
   LDY   #$8D
   LDA   (FP),Y
   LDY   #$35
   ADC   (FP),Y
   LDY   #$93
   STA   (FP),Y
   LDY   #$8E
   LDA   (FP),Y
   LDY   #$36
   ADC   (FP),Y
   LDY   #$94
   STA   (FP),Y
   LDY   #$8F
   LDA   (FP),Y
   LDY   #$95
   STA   (FP),Y
   LDY   #$90
   LDA   (FP),Y
   LDY   #$96
   STA   (FP),Y
   LDY   #$91
   LDA   (FP),Y
   LDY   #$97
   STA   (FP),Y
   LDY   #$92
   LDA   (FP),Y
   LDY   #$98
   STA   (FP),Y
   LDY   #$93
   LDA   (FP),Y
   LDY   #$99
   STA   (FP),Y
   LDY   #$94
   LDA   (FP),Y
   AND   #$1F
   LDY   #$9A
   STA   (FP),Y
   LDY   #$95
   LDA   (FP),Y
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
   EOR   #$10
   LDY   #$A0
   STA   (FP),Y
   LDY   #$9B
   LDA   (FP),Y
   LDY   #$A1
   STA   (FP),Y
   LDY   #$9C
   LDA   (FP),Y
   LDY   #$A2
   STA   (FP),Y
   LDY   #$9D
   LDA   (FP),Y
   LDY   #$A3
   STA   (FP),Y
   LDY   #$9E
   LDA   (FP),Y
   LDY   #$A4
   STA   (FP),Y
   LDY   #$9F
   LDA   (FP),Y
   LDY   #$A5
   STA   (FP),Y
   LDY   #$A0
   LDA   (FP),Y
   SEC
   SBC   #$10
   LDY   #$A6
   STA   (FP),Y
   LDY   #$A1
   LDA   (FP),Y
   LDY   #$A7
   STA   (FP),Y
   LDY   #$A2
   LDA   (FP),Y
   LDY   #$A8
   STA   (FP),Y
   LDY   #$A3
   LDA   (FP),Y
   LDY   #$A9
   STA   (FP),Y
   LDY   #$A4
   LDA   (FP),Y
   LDY   #$AA
   STA   (FP),Y
   LDY   #$A7
   LDA   (FP),Y
   STA   g0
   INY
   LDA   (FP),Y
   STA   g0+1
   INY
   LDA   (FP),Y
   STA   g0+2
   INY
   LDA   (FP),Y
   STA   g0+3
   LDY   #$29
   LDA   (FP),Y
   LDY   #$AB
   STA   (FP),Y
   LDY   #$2A
   LDA   (FP),Y
   LDY   #$AC
   STA   (FP),Y
   LDA   #$03
   BEQ   .lnot_true@58
   LDA   #$00
   JMP   .lnot_end@59
.lnot_true@58:
   LDA   #$01
.lnot_end@59:
   LDY   #$AD
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$AF
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$B0
   STA   (FP),Y
   LDY   #$D1
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$B1
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$B2
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$06
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDY   #$AB
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$AC
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$AF
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$B0
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   LDY   #$B1
   LDA   (FP),Y
   LDY   #$05
   STA   (SSP),Y
   LDY   #$B2
   LDA   (FP),Y
   LDY   #$06
   STA   (SSP),Y
   JSR   f4
   LDY   #$B3
   STA   (FP),Y
   TXA
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$B5
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$B6
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$15
   LDA   (FP),Y
   LDY   #$B5
   EOR   (FP),Y
   LDY   #$B9
   STA   (FP),Y
   LDY   #$16
   LDA   (FP),Y
   LDY   #$B6
   EOR   (FP),Y
   LDY   #$BA
   STA   (FP),Y
   LDY   #$17
   LDA   (FP),Y
   LDY   #$B7
   EOR   (FP),Y
   LDY   #$BB
   STA   (FP),Y
   LDY   #$18
   LDA   (FP),Y
   LDY   #$B8
   EOR   (FP),Y
   LDY   #$BC
   STA   (FP),Y
   LDY   #$B9
   LDA   (FP),Y
   LDY   #$15
   STA   (FP),Y
   LDY   #$BA
   LDA   (FP),Y
   LDY   #$16
   STA   (FP),Y
   LDY   #$BB
   LDA   (FP),Y
   LDY   #$17
   STA   (FP),Y
   LDY   #$BC
   LDA   (FP),Y
   LDY   #$18
   STA   (FP),Y
   LDY   #$15
   LDA   (FP),Y
   LDY   #$BD
   STA   (FP),Y
   LDY   #$16
   LDA   (FP),Y
   LDY   #$BE
   STA   (FP),Y
   LDY   #$17
   LDA   (FP),Y
   LDY   #$BF
   STA   (FP),Y
   LDY   #$18
   LDA   (FP),Y
   LDY   #$C0
   STA   (FP),Y
   LDA   g0
   CLC
   LDY   #$BD
   ADC   (FP),Y
   LDY   #$C1
   STA   (FP),Y
   LDA   g0+1
   LDY   #$BE
   ADC   (FP),Y
   LDY   #$C2
   STA   (FP),Y
   LDA   g0+2
   LDY   #$BF
   ADC   (FP),Y
   LDY   #$C3
   STA   (FP),Y
   LDA   g0+3
   LDY   #$C0
   ADC   (FP),Y
   LDY   #$C4
   STA   (FP),Y
   LDY   #$C1
   LDA   (FP),Y
   STA   g0
   INY
   LDA   (FP),Y
   STA   g0+1
   INY
   LDA   (FP),Y
   STA   g0+2
   INY
   LDA   (FP),Y
   STA   g0+3
   LDA   #$FF
   SEC
   LDY   #$31
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
   LDA   #$00
   INY
   SBC   (FP),Y
   BVC   .cmp_novf@60
   EOR   #$80
.cmp_novf@60:
   ASL
   LDA   #$00
   ROL
   LDY   #$C5
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$38
   LDA   (FP),Y
   BEQ   .and_false@6
   LDA   g0
   ORA   g0+1
   ORA   g0+2
   ORA   g0+3
   BEQ   .and_false@6
   LDA   #$01
   LDY   #$C7
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   JMP   .and_end@7
.and_false@6:
   LDA   #$00
   LDY   #$C7
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@7:
   LDY   #$C5
   LDA   (FP),Y
   SEC
   LDY   #$C7
   SBC   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$C8
   SBC   (FP),Y
   BVC   .cmp_novf@61
   EOR   #$80
.cmp_novf@61:
   ASL
   LDA   #$00
   ROL
   EOR   #$01
   LDY   #$C9
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$CB
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$CC
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@62
   LDA   #$00
   JMP   .sx_done@63
.sx_neg@62:
   LDA   #$FF
.sx_done@63:
   LDY   #$CD
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$CB
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

   ; epilogue
   CLC
   LDA   FP
   ADC   #$D2
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

f2:
   SUBROUTINE

   ; prologue: 10 arg bytes, 100 local bytes
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

   LDY   #$6D
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   LDY   #$6E
   LDA   (FP),Y
   LDY   #$02
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@64
   LDA   #$00
   JMP   .sx_done@65
.sx_neg@64:
   LDA   #$FF
.sx_done@65:
   LDY   #$03
   STA   (FP),Y
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
   INY
   STA   (FP),Y
   LDY   #$6D
   LDA   (FP),Y
   LDY   #$09
   STA   (FP),Y
   LDY   #$6E
   LDA   (FP),Y
   LDY   #$0A
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@66
   LDA   #$00
   JMP   .sx_done@67
.sx_neg@66:
   LDA   #$FF
.sx_done@67:
   LDY   #$0B
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$07
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   ORA   #$00
   BPL   .sx_done@69
   LDA   #$FF
.sx_done@69:
   LDY   #$0F
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   g0
   LDY   #$0D
   AND   (FP),Y
   LDY   #$11
   STA   (FP),Y
   LDA   g0+1
   LDY   #$0E
   AND   (FP),Y
   LDY   #$12
   STA   (FP),Y
   LDA   g0+2
   LDY   #$0F
   AND   (FP),Y
   LDY   #$13
   STA   (FP),Y
   LDA   g0+3
   LDY   #$10
   AND   (FP),Y
   LDY   #$14
   STA   (FP),Y
   LDY   #$05
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
   LDY   #$11
   LDA   (FP),Y
   STA   HARGS+4
   JSR   asl32
   LDA   HARGS+5
   LDY   #$15
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
   LDY   #$09
   LDA   (FP),Y
   LDY   #$15
   AND   (FP),Y
   LDY   #$19
   STA   (FP),Y
   LDY   #$0A
   LDA   (FP),Y
   LDY   #$16
   AND   (FP),Y
   LDY   #$1A
   STA   (FP),Y
   LDY   #$0B
   LDA   (FP),Y
   LDY   #$17
   AND   (FP),Y
   LDY   #$1B
   STA   (FP),Y
   LDY   #$0C
   LDA   (FP),Y
   LDY   #$18
   AND   (FP),Y
   LDY   #$1C
   STA   (FP),Y
   LDY   #$19
   LDA   (FP),Y
   LDY   #$1D
   STA   (FP),Y
   LDY   #$1A
   LDA   (FP),Y
   LDY   #$1E
   STA   (FP),Y
   LDY   #$1B
   LDA   (FP),Y
   LDY   #$1F
   STA   (FP),Y
   LDY   #$1C
   LDA   (FP),Y
   LDY   #$20
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$1D
   LDA   (FP),Y
   LDY   #$23
   STA   (FP),Y
   LDY   #$1E
   LDA   (FP),Y
   LDY   #$24
   STA   (FP),Y
   LDY   #$1F
   LDA   (FP),Y
   LDY   #$25
   STA   (FP),Y
   LDY   #$20
   LDA   (FP),Y
   LDY   #$26
   STA   (FP),Y
   LDY   #$21
   LDA   (FP),Y
   LDY   #$27
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$69
   LDA   (FP),Y
   LDY   #$29
   STA   (FP),Y
   LDY   #$6A
   LDA   (FP),Y
   LDY   #$2A
   STA   (FP),Y
   LDY   #$6B
   LDA   (FP),Y
   LDY   #$2B
   STA   (FP),Y
   LDY   #$6C
   LDA   (FP),Y
   LDY   #$2C
   STA   (FP),Y
   LDA   g0
   LDY   #$29
   EOR   (FP),Y
   LDY   #$2D
   STA   (FP),Y
   LDA   g0+1
   LDY   #$2A
   EOR   (FP),Y
   LDY   #$2E
   STA   (FP),Y
   LDA   g0+2
   LDY   #$2B
   EOR   (FP),Y
   LDY   #$2F
   STA   (FP),Y
   LDA   g0+3
   LDY   #$2C
   EOR   (FP),Y
   LDY   #$30
   STA   (FP),Y
   LDA   #$64
   LDY   #$67
   EOR   (FP),Y
   LDY   #$31
   STA   (FP),Y
   LDA   #$00
   LDY   #$68
   EOR   (FP),Y
   LDY   #$32
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$33
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$34
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@70
   LDA   #$00
   JMP   .sx_done@71
.sx_neg@70:
   LDA   #$FF
.sx_done@71:
   LDY   #$35
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$30
   LDA   (FP),Y
   LDY   #$36
   CMP   (FP),Y
   BNE   .thread@2
   LDY   #$2F
   LDA   (FP),Y
   LDY   #$35
   CMP   (FP),Y
   BNE   .thread@2
   LDY   #$2E
   LDA   (FP),Y
   LDY   #$34
   CMP   (FP),Y
   BNE   .thread@2
   LDY   #$2D
   LDA   (FP),Y
   LDY   #$33
   CMP   (FP),Y
   BEQ   .cmp_true@72
.thread@2:
   LDA   #$00
   JMP   .cmp_end@73
.cmp_true@72:
   LDA   #$01
.cmp_end@73:
   LDY   #$37
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$39
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$3A
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@75
   LDA   #$00
   JMP   .sx_done@76
.sx_neg@75:
   LDA   #$FF
.sx_done@76:
   LDY   #$3B
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$39
   LDA   (FP),Y
   LDY   #$3D
   STA   (FP),Y
   LDY   #$3A
   LDA   (FP),Y
   LDY   #$3E
   STA   (FP),Y
   LDY   #$3B
   LDA   (FP),Y
   LDY   #$3F
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$67
   LDA   (FP),Y
   ORA   #$01
   LDY   #$41
   STA   (FP),Y
   LDY   #$68
   LDA   (FP),Y
   LDY   #$42
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$43
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$44
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@77
   LDA   #$00
   JMP   .sx_done@78
.sx_neg@77:
   LDA   #$FF
.sx_done@78:
   LDY   #$45
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$69
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
   LDY   #$43
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
   JSR   sdivmod32
   LDA   HARGS+8
   LDY   #$47
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
   LDY   #$6D
   LDA   (FP),Y
   ORA   #$01
   LDY   #$4B
   STA   (FP),Y
   LDY   #$6E
   LDA   (FP),Y
   LDY   #$4C
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$4D
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$4E
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@79
   LDA   #$00
   JMP   .sx_done@80
.sx_neg@79:
   LDA   #$FF
.sx_done@80:
   LDY   #$4F
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$47
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
   LDY   #$4D
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
   JSR   sdivmod32
   LDA   HARGS+12
   LDY   #$51
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
   LDY   #$51
   LDA   (FP),Y
   LDY   #$55
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$6F
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$57
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$58
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   LDY   #$67
   ADC   (FP),Y
   LDY   #$59
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$68
   ADC   (FP),Y
   LDY   #$5A
   STA   (FP),Y
   LDY   #$6F
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$59
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$5A
   LDA   (FP),Y
   LDY   #$01
   STA   (DPTR),Y
   LDY   #$67
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@10
   LDY   #$05
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   INY
   ORA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@10
   LDA   #$00
   LDY   #$5B
   STA   (FP),Y
   INY
   STA   (FP),Y
   JMP   .or_end@11
.or_true@10:
   LDA   #$01
   LDY   #$5B
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@11:
   LDY   #$5B
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@8
   LDY   #$6F
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$5D
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$5E
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@8
   LDA   #$01
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   JMP   .and_end@9
.and_false@8:
   LDA   #$00
   LDY   #$5F
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@9:
   LDY   #$5F
   LDA   (FP),Y
   LDY   #$61
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$62
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@81
   LDA   #$00
   JMP   .sx_done@82
.sx_neg@81:
   LDA   #$FF
.sx_done@82:
   LDY   #$63
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$61
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

   ; epilogue
   CLC
   LDA   FP
   ADC   #$70
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

f1:
   SUBROUTINE

   ; prologue: 2 arg bytes, 65 local bytes
   SEC
   LDA   SSP
   SBC   #$43
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$42
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   LDA   g0
   ORA   g0+1
   ORA   g0+2
   ORA   g0+3
   BNE   .or_true@14
   INY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@14
   LDA   #$00
   LDY   #$01
   STA   (FP),Y
   INY
   STA   (FP),Y
   JMP   .or_end@15
.or_true@14:
   LDA   #$01
   LDY   #$01
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@15:
   LDY   #$01
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@12
   LDY   #$44
   LDA   (FP),Y
   LDY   #$03
   STA   (FP),Y
   LDY   #$45
   LDA   (FP),Y
   LDY   #$04
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@83
   LDA   #$00
   JMP   .sx_done@84
.sx_neg@83:
   LDA   #$FF
.sx_done@84:
   LDY   #$05
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   g0
   CLC
   LDY   #$03
   SBC   (FP),Y
   LDA   g0+1
   INY
   SBC   (FP),Y
   LDA   g0+2
   INY
   SBC   (FP),Y
   LDA   g0+3
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
   BNE   .or_true@12
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   JMP   .or_end@13
.or_true@12:
   LDA   #$01
   LDY   #$09
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@13:
   LDY   #$09
   LDA   (FP),Y
   LDY   #$0B
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$0C
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@85
   LDA   #$00
   JMP   .sx_done@86
.sx_neg@85:
   LDA   #$FF
.sx_done@86:
   LDY   #$0D
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$0B
   LDA   (FP),Y
   LDY   #$0F
   STA   (FP),Y
   LDY   #$0C
   LDA   (FP),Y
   LDY   #$10
   STA   (FP),Y
   LDY   #$0D
   LDA   (FP),Y
   LDY   #$11
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$0F
   LDA   (FP),Y
   LDY   #$13
   STA   (FP),Y
   LDY   #$10
   LDA   (FP),Y
   LDY   #$14
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$15
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$16
   STA   (FP),Y
   CLC
   LDA   FP
   ADC   #$15
   INY
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$02
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   DEY
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$18
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   JSR   f3
   LDA   HARGS+8
   LDY   #$19
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
   LDY   #$0F
   LDA   (FP),Y
   LDY   #$1D
   STA   (FP),Y
   LDY   #$10
   LDA   (FP),Y
   LDY   #$1E
   STA   (FP),Y
   LDY   #$11
   LDA   (FP),Y
   LDY   #$1F
   STA   (FP),Y
   LDY   #$12
   LDA   (FP),Y
   LDY   #$20
   STA   (FP),Y
   LDY   #$1D
   LDA   (FP),Y
   LDY   #$19
   EOR   (FP),Y
   LDY   #$21
   STA   (FP),Y
   LDY   #$1E
   LDA   (FP),Y
   LDY   #$1A
   EOR   (FP),Y
   LDY   #$22
   STA   (FP),Y
   LDY   #$1F
   LDA   (FP),Y
   LDY   #$1B
   EOR   (FP),Y
   LDY   #$23
   STA   (FP),Y
   LDY   #$20
   LDA   (FP),Y
   LDY   #$1C
   EOR   (FP),Y
   LDY   #$24
   STA   (FP),Y
   LDY   #$21
   LDA   (FP),Y
   LDY   #$25
   STA   (FP),Y
   LDY   #$22
   LDA   (FP),Y
   LDY   #$26
   STA   (FP),Y
   LDY   #$23
   LDA   (FP),Y
   LDY   #$27
   STA   (FP),Y
   LDY   #$24
   LDA   (FP),Y
   LDY   #$28
   STA   (FP),Y
   LDY   #$25
   LDA   (FP),Y
   LDY   #$0F
   STA   (FP),Y
   LDY   #$26
   LDA   (FP),Y
   LDY   #$10
   STA   (FP),Y
   LDY   #$27
   LDA   (FP),Y
   LDY   #$11
   STA   (FP),Y
   LDY   #$28
   LDA   (FP),Y
   LDY   #$12
   STA   (FP),Y
   LDY   #$15
   LDA   (FP),Y
   LDY   #$29
   STA   (FP),Y
   LDY   #$16
   LDA   (FP),Y
   LDY   #$2A
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@87
   LDA   #$00
   JMP   .sx_done@88
.sx_neg@87:
   LDA   #$FF
.sx_done@88:
   LDY   #$2B
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$0F
   LDA   (FP),Y
   CLC
   LDY   #$29
   ADC   (FP),Y
   LDY   #$2D
   STA   (FP),Y
   LDY   #$10
   LDA   (FP),Y
   LDY   #$2A
   ADC   (FP),Y
   LDY   #$2E
   STA   (FP),Y
   LDY   #$11
   LDA   (FP),Y
   LDY   #$2B
   ADC   (FP),Y
   LDY   #$2F
   STA   (FP),Y
   LDY   #$12
   LDA   (FP),Y
   LDY   #$2C
   ADC   (FP),Y
   LDY   #$30
   STA   (FP),Y
   LDY   #$2D
   LDA   (FP),Y
   LDY   #$0F
   STA   (FP),Y
   LDY   #$2E
   LDA   (FP),Y
   LDY   #$10
   STA   (FP),Y
   LDY   #$2F
   LDA   (FP),Y
   LDY   #$11
   STA   (FP),Y
   LDY   #$30
   LDA   (FP),Y
   LDY   #$12
   STA   (FP),Y
   LDY   #$0F
   LDA   (FP),Y
   LDY   #$31
   STA   (FP),Y
   LDY   #$10
   LDA   (FP),Y
   LDY   #$32
   STA   (FP),Y
   LDY   #$11
   LDA   (FP),Y
   LDY   #$33
   STA   (FP),Y
   LDY   #$12
   LDA   (FP),Y
   LDY   #$34
   STA   (FP),Y
   LDA   g0
   CLC
   LDY   #$31
   ADC   (FP),Y
   LDY   #$35
   STA   (FP),Y
   LDA   g0+1
   LDY   #$32
   ADC   (FP),Y
   LDY   #$36
   STA   (FP),Y
   LDA   g0+2
   LDY   #$33
   ADC   (FP),Y
   LDY   #$37
   STA   (FP),Y
   LDA   g0+3
   LDY   #$34
   ADC   (FP),Y
   LDY   #$38
   STA   (FP),Y
   LDY   #$35
   LDA   (FP),Y
   STA   g0
   INY
   LDA   (FP),Y
   STA   g0+1
   INY
   LDA   (FP),Y
   STA   g0+2
   INY
   LDA   (FP),Y
   STA   g0+3
   LDA   #$FF
   BNE   .or_true@16
   LDA   g0
   ORA   g0+1
   ORA   g0+2
   ORA   g0+3
   BNE   .or_true@16
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   JMP   .or_end@17
.or_true@16:
   LDA   #$01
   LDY   #$39
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@17:
   LDY   #$39
   LDA   (FP),Y
   AND   #$07
   LDY   #$3B
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDA   #$00
   LDY   #$3C
   STA   (FP),Y
   LDA   g0
   STA   HARGS
   LDA   g0+1
   STA   HARGS+1
   LDA   g0+2
   STA   HARGS+2
   LDA   g0+3
   STA   HARGS+3
   DEY
   LDA   (FP),Y
   STA   HARGS+4
   JSR   asl32
   LDA   HARGS+5
   LDY   #$3D
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
   LDY   #$3D
   LDA   (FP),Y
   LDY   #$41
   STA   (FP),Y

   ; epilogue
   PHA
   CLC
   LDA   FP
   ADC   #$45
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
   RTS

f0:
   SUBROUTINE

   ; prologue: 10 arg bytes, 81 local bytes
   SEC
   LDA   SSP
   SBC   #$53
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$52
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   LDA   g0
   ORA   g0+1
   ORA   g0+2
   ORA   g0+3
   BNE   .lb_skip@2
   JMP   .and_false@18
.lb_skip@2:
   SEC
   LDA   #$00
   LDY   #$54
   SBC   (FP),Y
   LDY   #$01
   STA   (FP),Y
   LDA   #$00
   LDY   #$55
   SBC   (FP),Y
   LDY   #$02
   STA   (FP),Y
   LDA   #$00
   LDY   #$56
   SBC   (FP),Y
   LDY   #$03
   STA   (FP),Y
   LDA   #$00
   LDY   #$57
   SBC   (FP),Y
   LDY   #$04
   STA   (FP),Y
   LDA   #$00
   LDY   #$58
   SBC   (FP),Y
   LDY   #$05
   STA   (FP),Y
   LDA   #$00
   LDY   #$59
   SBC   (FP),Y
   LDY   #$06
   STA   (FP),Y
   LDY   #$01
   LDA   (FP),Y
   LDY   #$07
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   LDY   #$08
   STA   (FP),Y
   LDY   #$03
   LDA   (FP),Y
   LDY   #$09
   STA   (FP),Y
   LDY   #$04
   LDA   (FP),Y
   LDY   #$0A
   STA   (FP),Y
   LDY   #$05
   LDA   (FP),Y
   LDY   #$0B
   STA   (FP),Y
   LDY   #$06
   LDA   (FP),Y
   AND   #$1F
   LDY   #$0C
   STA   (FP),Y
   LDY   #$07
   LDA   (FP),Y
   LDY   #$0D
   STA   (FP),Y
   LDY   #$08
   LDA   (FP),Y
   LDY   #$0E
   STA   (FP),Y
   LDY   #$09
   LDA   (FP),Y
   LDY   #$0F
   STA   (FP),Y
   LDY   #$0A
   LDA   (FP),Y
   LDY   #$10
   STA   (FP),Y
   LDY   #$0B
   LDA   (FP),Y
   LDY   #$11
   STA   (FP),Y
   LDY   #$0C
   LDA   (FP),Y
   EOR   #$10
   LDY   #$12
   STA   (FP),Y
   LDY   #$0D
   LDA   (FP),Y
   LDY   #$13
   STA   (FP),Y
   LDY   #$0E
   LDA   (FP),Y
   LDY   #$14
   STA   (FP),Y
   LDY   #$0F
   LDA   (FP),Y
   LDY   #$15
   STA   (FP),Y
   LDY   #$10
   LDA   (FP),Y
   LDY   #$16
   STA   (FP),Y
   LDY   #$11
   LDA   (FP),Y
   LDY   #$17
   STA   (FP),Y
   LDY   #$12
   LDA   (FP),Y
   SEC
   SBC   #$10
   LDY   #$18
   STA   (FP),Y
   LDY   #$13
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
   BEQ   .and_false@18
   LDA   #$01
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   JMP   .and_end@19
.and_false@18:
   LDA   #$00
   LDY   #$19
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@19:
   LDY   #$19
   LDA   (FP),Y
   LDY   #$1B
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$1C
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$1D
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$1E
   STA   (FP),Y
   LDY   #$5C
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$1F
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$20
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   LDY   #$5A
   ADC   (FP),Y
   LDY   #$21
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$5B
   ADC   (FP),Y
   LDY   #$22
   STA   (FP),Y
   LDY   #$5C
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$21
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$22
   LDA   (FP),Y
   LDY   #$01
   STA   (DPTR),Y
   LDY   #$1D
   LDA   (FP),Y
   LDY   #$23
   STA   (FP),Y
   LDY   #$1E
   LDA   (FP),Y
   LDY   #$24
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$25
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$26
   STA   (FP),Y
   CLC
   LDA   FP
   ADC   #$25
   INY
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$02
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   DEY
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$28
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   JSR   f3
   LDA   HARGS+8
   LDY   #$29
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
   LDY   #$1D
   LDA   (FP),Y
   LDY   #$2D
   STA   (FP),Y
   LDY   #$1E
   LDA   (FP),Y
   LDY   #$2E
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$2D
   LDA   (FP),Y
   LDY   #$29
   EOR   (FP),Y
   LDY   #$31
   STA   (FP),Y
   LDY   #$2E
   LDA   (FP),Y
   LDY   #$2A
   EOR   (FP),Y
   LDY   #$32
   STA   (FP),Y
   LDY   #$2F
   LDA   (FP),Y
   LDY   #$2B
   EOR   (FP),Y
   LDY   #$33
   STA   (FP),Y
   LDY   #$30
   LDA   (FP),Y
   LDY   #$2C
   EOR   (FP),Y
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
   DEY
   LDA   (FP),Y
   LDY   #$1D
   STA   (FP),Y
   LDY   #$36
   LDA   (FP),Y
   LDY   #$1E
   STA   (FP),Y
   LDY   #$25
   LDA   (FP),Y
   LDY   #$37
   STA   (FP),Y
   LDY   #$26
   LDA   (FP),Y
   LDY   #$38
   STA   (FP),Y
   LDY   #$1D
   LDA   (FP),Y
   CLC
   LDY   #$37
   ADC   (FP),Y
   LDY   #$39
   STA   (FP),Y
   LDY   #$1E
   LDA   (FP),Y
   LDY   #$38
   ADC   (FP),Y
   LDY   #$3A
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$1D
   STA   (FP),Y
   LDY   #$3A
   LDA   (FP),Y
   LDY   #$1E
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$3B
   STA   (FP),Y
   LDY   #$1E
   LDA   (FP),Y
   LDY   #$3C
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   g0
   CLC
   LDY   #$3B
   ADC   (FP),Y
   LDY   #$3F
   STA   (FP),Y
   LDA   g0+1
   LDY   #$3C
   ADC   (FP),Y
   LDY   #$40
   STA   (FP),Y
   LDA   g0+2
   LDY   #$3D
   ADC   (FP),Y
   LDY   #$41
   STA   (FP),Y
   LDA   g0+3
   LDY   #$3E
   ADC   (FP),Y
   LDY   #$42
   STA   (FP),Y
   LDY   #$3F
   LDA   (FP),Y
   STA   g0
   INY
   LDA   (FP),Y
   STA   g0+1
   INY
   LDA   (FP),Y
   STA   g0+2
   INY
   LDA   (FP),Y
   STA   g0+3
   LDY   #$5C
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$43
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$44
   STA   (FP),Y
   LDA   #$00
   CMP   (FP),Y
   BNE   .vthread@0
   LDA   #$01
   DEY
   CMP   (FP),Y
   BEQ   .cmp_true@89
   LDA   #$00
.vthread@0:
.cmp_true@89:
   LDY   #$45
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$5A
   LDA   (FP),Y
   LDY   #$47
   STA   (FP),Y
   LDY   #$5B
   LDA   (FP),Y
   LDY   #$48
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@92
   LDA   #$00
   JMP   .sx_done@93
.sx_neg@92:
   LDA   #$FF
.sx_done@93:
   LDY   #$49
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$59
   CMP   (FP),Y
   BNE   .cmp_true@94
   DEY
   CMP   (FP),Y
   BNE   .cmp_true@94
   DEY
   CMP   (FP),Y
   BNE   .cmp_true@94
   DEY
   CMP   (FP),Y
   BNE   .cmp_true@94
   LDY   #$48
   LDA   (FP),Y
   LDY   #$55
   CMP   (FP),Y
   BNE   .cmp_true@94
   LDY   #$47
   LDA   (FP),Y
   LDY   #$54
   CMP   (FP),Y
   BNE   .cmp_true@94
   LDA   #$00
   JMP   .cmp_end@95
.cmp_true@94:
   LDA   #$01
.cmp_end@95:
   LDY   #$4D
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$45
   LDA   (FP),Y
   SEC
   LDY   #$4D
   SBC   (FP),Y
   LDY   #$46
   LDA   (FP),Y
   LDY   #$4E
   SBC   (FP),Y
   BVC   .cmp_novf@97
   EOR   #$80
.cmp_novf@97:
   ASL
   LDA   #$00
   ROL
   LDY   #$4F
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$51
   STA   (FP),Y

   ; epilogue
   PHA
   CLC
   LDA   FP
   ADC   #$5D
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
   RTS

main:
   SUBROUTINE

   ; prologue: 0 arg bytes, 31 local bytes
   SEC
   LDA   SSP
   SBC   #$21
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$20
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   LDA   #$05
   LDY   #$01
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   CLC
   LDA   FP
   ADC   #$01
   INY
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$0A
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   #$8C
   LDY   #$01
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
   LDA   #$AE
   INY
   STA   (SSP),Y
   LDA   #$00
   INY
   STA   (SSP),Y
   LDY   #$03
   LDA   (FP),Y
   LDY   #$09
   STA   (SSP),Y
   LDY   #$04
   LDA   (FP),Y
   LDY   #$0A
   STA   (SSP),Y
   JSR   f0
   LDY   #$05
   STA   (FP),Y
   INY
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@98
   LDA   #$00
   JMP   .sx_done@99
.sx_neg@98:
   LDA   #$FF
.sx_done@99:
   LDY   #$07
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$06
   LDA   (FP),Y
   LDY   #$0A
   STA   (FP),Y
   LDY   #$07
   LDA   (FP),Y
   LDY   #$0B
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   g0
   INY
   STA   (FP),Y
   LDA   g0+1
   INY
   STA   (FP),Y
   LDA   g0+2
   INY
   STA   (FP),Y
   LDA   g0+3
   INY
   STA   (FP),Y
   LDY   #$0A
   LDA   (FP),Y
   LDY   #$0E
   EOR   (FP),Y
   LDY   #$12
   STA   (FP),Y
   LDY   #$0B
   LDA   (FP),Y
   LDY   #$0F
   EOR   (FP),Y
   LDY   #$13
   STA   (FP),Y
   LDY   #$0C
   LDA   (FP),Y
   LDY   #$10
   EOR   (FP),Y
   LDY   #$14
   STA   (FP),Y
   LDY   #$0D
   LDA   (FP),Y
   LDY   #$11
   EOR   (FP),Y
   LDY   #$15
   STA   (FP),Y
   LDY   #$12
   LDA   (FP),Y
   LDY   #$0A
   STA   (FP),Y
   LDY   #$13
   LDA   (FP),Y
   LDY   #$0B
   STA   (FP),Y
   LDY   #$14
   LDA   (FP),Y
   LDY   #$0C
   STA   (FP),Y
   LDY   #$15
   LDA   (FP),Y
   LDY   #$0D
   STA   (FP),Y
   LDY   #$0A
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
   LDA   #$10
   STA   HARGS+4
   JSR   asr32
   LDA   HARGS+5
   LDY   #$16
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
   LDY   #$0A
   LDA   (FP),Y
   LDY   #$16
   EOR   (FP),Y
   LDY   #$1A
   STA   (FP),Y
   LDY   #$0B
   LDA   (FP),Y
   LDY   #$17
   EOR   (FP),Y
   LDY   #$1B
   STA   (FP),Y
   LDY   #$0C
   LDA   (FP),Y
   LDY   #$18
   EOR   (FP),Y
   LDY   #$1C
   STA   (FP),Y
   LDY   #$0D
   LDA   (FP),Y
   LDY   #$19
   EOR   (FP),Y
   LDY   #$1D
   STA   (FP),Y
   LDY   #$1A
   LDA   (FP),Y
   LDY   #$1E
   STA   (FP),Y
   LDY   #$1B
   LDA   (FP),Y
   LDY   #$1F
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
   ADC   #$21
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$20
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

g0:
   DC.L  $0000002F
