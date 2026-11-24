SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24

rec:
   SUBROUTINE

   ; prologue: 4 arg bytes, 54 local bytes
   SEC
   LDA   SSP
   SBC   #$38
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$37
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   LDY   #$3B
   LDA   (FP),Y
   STA   HARGS
   INY
   LDA   (FP),Y
   STA   HARGS+1
   LDA   #$07
   STA   HARGS+2
   LDA   #$00
   STA   HARGS+3
   JSR   mul16
   LDA   HARGS+4
   LDY   #$01
   STA   (FP),Y
   LDA   HARGS+5
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$03
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$3B
   LDA   (FP),Y
   LDY   #$05
   STA   (FP),Y
   LDY   #$3C
   LDA   (FP),Y
   LDY   #$06
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@0
   LDA   #$00
   JMP   .sx_done@1
.sx_neg@0:
   LDA   #$FF
.sx_done@1:
   LDY   #$07
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$03
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   ORA   #$00
   BPL   .sx_done@3
   LDA   #$FF
.sx_done@3:
   LDY   #$0B
   STA   (FP),Y
   INY
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
   JSR   mul32
   LDA   HARGS+8
   LDY   #$0D
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
   LDY   #$0D
   LDA   (FP),Y
   LDY   #$11
   STA   (FP),Y
   LDY   #$0E
   LDA   (FP),Y
   LDY   #$12
   STA   (FP),Y
   LDY   #$0F
   LDA   (FP),Y
   LDY   #$13
   STA   (FP),Y
   LDY   #$10
   LDA   (FP),Y
   LDY   #$14
   STA   (FP),Y
   LDA   #$00
   SEC
   LDY   #$39
   SBC   (FP),Y
   LDA   #$00
   INY
   SBC   (FP),Y
   BVC   .cmp_novf@4
   EOR   #$80
.cmp_novf@4:
   ASL
   LDA   #$00
   ROL
   EOR   #$01
   LDY   #$15
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@0
   LDY   #$3B
   LDA   (FP),Y
   LDY   #$17
   STA   (FP),Y
   LDY   #$3C
   LDA   (FP),Y
   LDA   #$00
   LDY   #$18
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
   ADC   #$3C
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$37
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
   LDY   #$39
   LDA   (FP),Y
   SEC
   SBC   #$01
   LDY   #$19
   STA   (FP),Y
   LDY   #$3A
   LDA   (FP),Y
   SBC   #$00
   LDY   #$1A
   STA   (FP),Y
   LDY   #$3B
   LDA   (FP),Y
   STA   HARGS
   INY
   LDA   (FP),Y
   STA   HARGS+1
   LDA   #$03
   STA   HARGS+2
   LDA   #$00
   STA   HARGS+3
   JSR   mul16
   LDA   HARGS+4
   LDY   #$1B
   STA   (FP),Y
   LDA   HARGS+5
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   LDY   #$39
   ADC   (FP),Y
   LDY   #$1D
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$3A
   ADC   (FP),Y
   LDY   #$1E
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$1F
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   AND   #$03
   LDY   #$20
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$04
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDY   #$19
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$1A
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$1F
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$20
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   JSR   rec
   LDY   #$21
   STA   (FP),Y
   TXA
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$23
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$24
   STA   (FP),Y
   LDY   #$04
   LDA   (FP),Y
   LDY   #$25
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$23
   LDA   (FP),Y
   CLC
   LDY   #$25
   ADC   (FP),Y
   LDY   #$27
   STA   (FP),Y
   LDY   #$24
   LDA   (FP),Y
   LDY   #$26
   ADC   (FP),Y
   LDY   #$28
   STA   (FP),Y
   LDA   #$0F
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   ORA   #$00
   BPL   .sx_done@6
   LDA   #$FF
.sx_done@6:
   LDY   #$2B
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$11
   LDA   (FP),Y
   LDY   #$29
   AND   (FP),Y
   LDY   #$2D
   STA   (FP),Y
   LDY   #$12
   LDA   (FP),Y
   LDY   #$2A
   AND   (FP),Y
   LDY   #$2E
   STA   (FP),Y
   LDY   #$13
   LDA   (FP),Y
   LDY   #$2B
   AND   (FP),Y
   LDY   #$2F
   STA   (FP),Y
   LDY   #$14
   LDA   (FP),Y
   LDY   #$2C
   AND   (FP),Y
   LDY   #$30
   STA   (FP),Y
   LDY   #$2D
   LDA   (FP),Y
   LDY   #$31
   STA   (FP),Y
   LDY   #$2E
   LDA   (FP),Y
   LDY   #$32
   STA   (FP),Y
   LDY   #$27
   LDA   (FP),Y
   CLC
   LDY   #$31
   ADC   (FP),Y
   LDY   #$33
   STA   (FP),Y
   LDY   #$28
   LDA   (FP),Y
   LDY   #$32
   ADC   (FP),Y
   LDY   #$34
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$35
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   AND   #$03
   LDY   #$36
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
   ADC   #$3C
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$37
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

f4:
   SUBROUTINE

   ; prologue: 2 arg bytes, 97 local bytes
   SEC
   LDA   SSP
   SBC   #$63
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$62
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   LDA   g0
   LDY   #$01
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
   LDY   #$01
   LDA   (FP),Y
   LDY   #$06
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   LDY   #$07
   STA   (FP),Y
   LDY   #$03
   LDA   (FP),Y
   LDY   #$08
   STA   (FP),Y
   LDY   #$04
   LDA   (FP),Y
   LDY   #$09
   STA   (FP),Y
   LDY   #$05
   LDA   (FP),Y
   LDY   #$0A
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
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$0A
   LDA   (FP),Y
   LDY   #$0F
   CMP   (FP),Y
   BNE   .thread@0
   LDY   #$09
   LDA   (FP),Y
   LDY   #$0E
   CMP   (FP),Y
   BNE   .thread@0
   LDY   #$08
   LDA   (FP),Y
   LDY   #$0D
   CMP   (FP),Y
   BNE   .thread@0
   LDY   #$07
   LDA   (FP),Y
   LDY   #$0C
   CMP   (FP),Y
   BNE   .thread@0
   LDY   #$06
   LDA   (FP),Y
   LDY   #$0B
   CMP   (FP),Y
   BEQ   .cmp_true@7
.thread@0:
   LDA   #$00
   JMP   .cmp_end@8
.cmp_true@7:
   LDA   #$01
.cmp_end@8:
   LDY   #$10
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$12
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$13
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   AND   #$03
   LDY   #$14
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDA   #$00
   LDY   #$15
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$16
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   g0
   INY
   STA   (FP),Y
   LDA   g0+1
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$1A
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$17
   LDA   (FP),Y
   LDY   #$1C
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@10
   LDA   #$00
   JMP   .sx_done@11
.sx_neg@10:
   LDA   #$FF
.sx_done@11:
   LDY   #$1D
   STA   (FP),Y
   LDY   #$1B
   LDA   (FP),Y
   LDY   #$1E
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@12
   LDA   #$00
   JMP   .sx_done@13
.sx_neg@12:
   LDA   #$FF
.sx_done@13:
   LDY   #$1F
   STA   (FP),Y
   LDY   #$1C
   LDA   (FP),Y
   SEC
   LDY   #$1E
   SBC   (FP),Y
   LDY   #$20
   STA   (FP),Y
   LDY   #$1D
   LDA   (FP),Y
   LDY   #$1F
   SBC   (FP),Y
   LDY   #$21
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$22
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$23
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@14
   LDA   #$00
   JMP   .sx_done@15
.sx_neg@14:
   LDA   #$FF
.sx_done@15:
   LDY   #$24
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$22
   LDA   (FP),Y
   LDY   #$27
   STA   (FP),Y
   LDY   #$23
   LDA   (FP),Y
   LDY   #$28
   STA   (FP),Y
   LDY   #$24
   LDA   (FP),Y
   LDY   #$29
   STA   (FP),Y
   INY
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
   LDY   #$2C
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
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$2C
   LDA   (FP),Y
   LDY   #$32
   STA   (FP),Y
   LDY   #$64
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$33
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$34
   STA   (FP),Y
   LDY   #$17
   LDA   (FP),Y
   LDY   #$35
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@18
   LDA   #$00
   JMP   .sx_done@19
.sx_neg@18:
   LDA   #$FF
.sx_done@19:
   LDY   #$36
   STA   (FP),Y
   LDY   #$17
   LDA   (FP),Y
   LDY   #$37
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@20
   LDA   #$00
   JMP   .sx_done@21
.sx_neg@20:
   LDA   #$FF
.sx_done@21:
   LDY   #$38
   STA   (FP),Y
   LDY   #$36
   LDA   (FP),Y
   LDY   #$38
   CMP   (FP),Y
   BNE   .cmp_true@22
   LDY   #$35
   LDA   (FP),Y
   LDY   #$37
   CMP   (FP),Y
   BNE   .cmp_true@22
   LDA   #$00
   JMP   .cmp_end@23
.cmp_true@22:
   LDA   #$01
.cmp_end@23:
   LDY   #$39
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$33
   LDA   (FP),Y
   CLC
   LDY   #$39
   ADC   (FP),Y
   LDY   #$3B
   STA   (FP),Y
   LDY   #$34
   LDA   (FP),Y
   LDY   #$3A
   ADC   (FP),Y
   LDY   #$3C
   STA   (FP),Y
   LDY   #$64
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$3B
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$3C
   LDA   (FP),Y
   LDY   #$01
   STA   (DPTR),Y
   LDY   #$17
   LDA   (FP),Y
   LDY   #$3D
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@25
   LDA   #$00
   JMP   .sx_done@26
.sx_neg@25:
   LDA   #$FF
.sx_done@26:
   LDY   #$3E
   STA   (FP),Y
   LDY   #$64
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$3F
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$40
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   SEC
   LDY   #$3D
   SBC   (FP),Y
   LDY   #$40
   LDA   (FP),Y
   LDY   #$3E
   SBC   (FP),Y
   BVC   .cmp_novf@27
   EOR   #$80
.cmp_novf@27:
   ASL
   LDA   #$00
   ROL
   EOR   #$01
   LDY   #$41
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$04
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   #$03
   LDY   #$01
   STA   (SSP),Y
   LDA   #$00
   INY
   STA   (SSP),Y
   LDY   #$41
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$42
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   JSR   rec
   LDY   #$43
   STA   (FP),Y
   TXA
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$45
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$46
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@28
   LDA   #$00
   JMP   .sx_done@29
.sx_neg@28:
   LDA   #$FF
.sx_done@29:
   LDY   #$47
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$06
   LDA   (FP),Y
   CLC
   LDY   #$45
   ADC   (FP),Y
   LDY   #$4A
   STA   (FP),Y
   LDY   #$07
   LDA   (FP),Y
   LDY   #$46
   ADC   (FP),Y
   LDY   #$4B
   STA   (FP),Y
   LDY   #$08
   LDA   (FP),Y
   LDY   #$47
   ADC   (FP),Y
   LDY   #$4C
   STA   (FP),Y
   LDY   #$09
   LDA   (FP),Y
   LDY   #$48
   ADC   (FP),Y
   LDY   #$4D
   STA   (FP),Y
   LDY   #$0A
   LDA   (FP),Y
   LDY   #$49
   ADC   (FP),Y
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
   AND   #$01
   LDY   #$53
   STA   (FP),Y
   LDY   #$4F
   LDA   (FP),Y
   LDY   #$54
   STA   (FP),Y
   LDY   #$50
   LDA   (FP),Y
   LDY   #$55
   STA   (FP),Y
   LDY   #$51
   LDA   (FP),Y
   LDY   #$56
   STA   (FP),Y
   LDY   #$52
   LDA   (FP),Y
   LDY   #$57
   STA   (FP),Y
   LDY   #$53
   LDA   (FP),Y
   EOR   #$01
   LDY   #$58
   STA   (FP),Y
   LDY   #$54
   LDA   (FP),Y
   LDY   #$59
   STA   (FP),Y
   LDY   #$55
   LDA   (FP),Y
   LDY   #$5A
   STA   (FP),Y
   LDY   #$56
   LDA   (FP),Y
   LDY   #$5B
   STA   (FP),Y
   LDY   #$57
   LDA   (FP),Y
   LDY   #$5C
   STA   (FP),Y
   LDY   #$58
   LDA   (FP),Y
   SEC
   SBC   #$01
   LDY   #$5D
   STA   (FP),Y
   LDY   #$59
   LDA   (FP),Y
   LDY   #$06
   STA   (FP),Y
   LDY   #$5A
   LDA   (FP),Y
   LDY   #$07
   STA   (FP),Y
   LDY   #$5B
   LDA   (FP),Y
   LDY   #$08
   STA   (FP),Y
   LDY   #$5C
   LDA   (FP),Y
   LDY   #$09
   STA   (FP),Y
   LDY   #$5D
   LDA   (FP),Y
   LDY   #$0A
   STA   (FP),Y
   LDA   #$E8
   LDY   #$5E
   STA   (FP),Y
   LDA   #$03
   INY
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@30
   LDA   #$00
   JMP   .sx_done@31
.sx_neg@30:
   LDA   #$FF
.sx_done@31:
   LDY   #$60
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$5E
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
   ADC   #$65
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

f3:
   SUBROUTINE

   ; prologue: 2 arg bytes, 36 local bytes
   SEC
   LDA   SSP
   SBC   #$26
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$25
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   LDA   #$01
   LDY   #$01
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   ORA   #$00
   BPL   .sx_done@33
   LDA   #$FF
.sx_done@33:
   LDY   #$03
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   g0
   LDY   #$01
   ORA   (FP),Y
   LDY   #$05
   STA   (FP),Y
   LDA   g0+1
   LDY   #$02
   ORA   (FP),Y
   LDY   #$06
   STA   (FP),Y
   LDA   g0+2
   LDY   #$03
   ORA   (FP),Y
   LDY   #$07
   STA   (FP),Y
   LDA   g0+3
   LDY   #$04
   ORA   (FP),Y
   LDY   #$08
   STA   (FP),Y
   LDA   g0
   STA   HARGS
   LDA   g0+1
   STA   HARGS+1
   LDA   g0+2
   STA   HARGS+2
   LDA   g0+3
   STA   HARGS+3
   LDY   #$05
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
   LDY   #$09
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
   LDA   #$FF
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   ORA   #$00
   BPL   .sx_done@35
   LDA   #$FF
.sx_done@35:
   LDY   #$0F
   STA   (FP),Y
   INY
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
   LDY   #$0B
   LDA   (FP),Y
   LDY   #$0F
   SBC   (FP),Y
   LDY   #$0C
   LDA   (FP),Y
   LDY   #$10
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
   LDY   #$13
   STA   (FP),Y
   DEY
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
   LDY   #$27
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$17
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$18
   STA   (FP),Y
   LDA   #$01
   BEQ   .and_false@1
   LDA   g0
   ORA   g0+1
   ORA   g0+2
   ORA   g0+3
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
   LDY   #$19
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@2:
   LDY   #$17
   LDA   (FP),Y
   CLC
   LDY   #$19
   ADC   (FP),Y
   LDY   #$1B
   STA   (FP),Y
   LDY   #$18
   LDA   (FP),Y
   LDY   #$1A
   ADC   (FP),Y
   LDY   #$1C
   STA   (FP),Y
   LDY   #$27
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$1B
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$1C
   LDA   (FP),Y
   LDY   #$01
   STA   (DPTR),Y
   LDY   #$27
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$1D
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$1E
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
   BMI   .sx_neg@36
   LDA   #$00
   JMP   .sx_done@37
.sx_neg@36:
   LDA   #$FF
.sx_done@37:
   LDY   #$21
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$1F
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

   ; epilogue
   CLC
   LDA   FP
   ADC   #$28
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

   ; prologue: 0 arg bytes, 207 local bytes
   SEC
   LDA   SSP
   SBC   #$D1
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$D0
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   LDA   #$01
   LDY   #$01
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   ORA   #$00
   BPL   .sx_done@39
   LDA   #$FF
.sx_done@39:
   LDY   #$03
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$01
   LDA   (FP),Y
   CLC
   ADC   g0
   LDY   #$05
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   ADC   g0+1
   LDY   #$06
   STA   (FP),Y
   LDY   #$03
   LDA   (FP),Y
   ADC   g0+2
   LDY   #$07
   STA   (FP),Y
   LDY   #$04
   LDA   (FP),Y
   ADC   g0+3
   LDY   #$08
   STA   (FP),Y
   LDA   g0
   CLC
   LDY   #$05
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
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   ORA   #$00
   BPL   .sx_done@41
   LDA   #$FF
.sx_done@41:
   LDY   #$11
   STA   (FP),Y
   INY
   STA   (FP),Y
   CMP   g0+3
   BNE   .thread@1
   CMP   g0+2
   BNE   .thread@1
   LDY   #$10
   LDA   (FP),Y
   CMP   g0+1
   BNE   .thread@1
   CMP   g0
   BEQ   .cmp_true@42
.thread@1:
   LDA   #$00
   JMP   .cmp_end@43
.cmp_true@42:
   LDA   #$01
.cmp_end@43:
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
   LDY   #$0D
   LDA   (FP),Y
   SEC
   SBC   (FP),Y
   LDY   #$17
   STA   (FP),Y
   LDY   #$0E
   LDA   (FP),Y
   SBC   (FP),Y
   LDY   #$18
   STA   (FP),Y
   LDY   #$15
   LDA   (FP),Y
   SEC
   LDY   #$17
   SBC   (FP),Y
   LDY   #$19
   STA   (FP),Y
   LDY   #$16
   LDA   (FP),Y
   LDY   #$18
   SBC   (FP),Y
   LDY   #$1A
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$1B
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$1C
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$1B
   LDA   (FP),Y
   LDY   #$20
   STA   (FP),Y
   LDY   #$1C
   LDA   (FP),Y
   LDY   #$21
   STA   (FP),Y
   LDY   #$1D
   LDA   (FP),Y
   LDY   #$22
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   g0
   ORA   g0+1
   ORA   g0+2
   ORA   g0+3
   BNE   .or_true@3
   LDY   #$0D
   LDA   (FP),Y
   LDY   #$25
   STA   (FP),Y
   LDY   #$0E
   LDA   (FP),Y
   LDY   #$26
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$25
   LDA   (FP),Y
   CLC
   ADC   g0
   LDY   #$29
   STA   (FP),Y
   LDY   #$26
   LDA   (FP),Y
   ADC   g0+1
   LDY   #$2A
   STA   (FP),Y
   LDY   #$27
   LDA   (FP),Y
   ADC   g0+2
   LDY   #$2B
   STA   (FP),Y
   LDY   #$28
   LDA   (FP),Y
   ADC   g0+3
   LDY   #$2C
   STA   (FP),Y
   LDY   #$29
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   INY
   ORA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@3
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   JMP   .or_end@4
.or_true@3:
   LDA   #$01
   LDY   #$2D
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@4:
   LDY   #$2D
   LDA   (FP),Y
   LDY   #$2F
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$30
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@45
   LDA   #$00
   JMP   .sx_done@46
.sx_neg@45:
   LDA   #$FF
.sx_done@46:
   LDY   #$31
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$2F
   LDA   (FP),Y
   LDY   #$34
   STA   (FP),Y
   LDY   #$30
   LDA   (FP),Y
   LDY   #$35
   STA   (FP),Y
   LDY   #$31
   LDA   (FP),Y
   LDY   #$36
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   g0
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$0D
   LDA   (FP),Y
   LDY   #$3B
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$0E
   LDA   (FP),Y
   LDY   #$3E
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$3F
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$40
   STA   (FP),Y
   CLC
   LDA   FP
   ADC   #$3F
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
   LDY   #$42
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   JSR   f4
   LDA   HARGS+8
   LDY   #$43
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
   LDY   #$43
   LDA   (FP),Y
   LDY   #$47
   STA   (FP),Y
   LDY   #$44
   LDA   (FP),Y
   LDY   #$48
   STA   (FP),Y
   LDY   #$45
   LDA   (FP),Y
   LDY   #$49
   STA   (FP),Y
   LDY   #$46
   LDA   (FP),Y
   LDY   #$4A
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$34
   LDA   (FP),Y
   LDY   #$47
   EOR   (FP),Y
   LDY   #$4C
   STA   (FP),Y
   LDY   #$35
   LDA   (FP),Y
   LDY   #$48
   EOR   (FP),Y
   LDY   #$4D
   STA   (FP),Y
   LDY   #$36
   LDA   (FP),Y
   LDY   #$49
   EOR   (FP),Y
   LDY   #$4E
   STA   (FP),Y
   LDY   #$37
   LDA   (FP),Y
   LDY   #$4A
   EOR   (FP),Y
   LDY   #$4F
   STA   (FP),Y
   LDY   #$38
   LDA   (FP),Y
   LDY   #$4B
   EOR   (FP),Y
   LDY   #$50
   STA   (FP),Y
   LDY   #$4C
   LDA   (FP),Y
   LDY   #$34
   STA   (FP),Y
   LDY   #$4D
   LDA   (FP),Y
   LDY   #$35
   STA   (FP),Y
   LDY   #$4E
   LDA   (FP),Y
   LDY   #$36
   STA   (FP),Y
   LDY   #$4F
   LDA   (FP),Y
   LDY   #$37
   STA   (FP),Y
   LDY   #$50
   LDA   (FP),Y
   LDY   #$38
   STA   (FP),Y
   LDY   #$3F
   LDA   (FP),Y
   LDY   #$51
   STA   (FP),Y
   LDY   #$40
   LDA   (FP),Y
   LDY   #$52
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@47
   LDA   #$00
   JMP   .sx_done@48
.sx_neg@47:
   LDA   #$FF
.sx_done@48:
   LDY   #$53
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$34
   LDA   (FP),Y
   CLC
   LDY   #$51
   ADC   (FP),Y
   LDY   #$56
   STA   (FP),Y
   LDY   #$35
   LDA   (FP),Y
   LDY   #$52
   ADC   (FP),Y
   LDY   #$57
   STA   (FP),Y
   LDY   #$36
   LDA   (FP),Y
   LDY   #$53
   ADC   (FP),Y
   LDY   #$58
   STA   (FP),Y
   LDY   #$37
   LDA   (FP),Y
   LDY   #$54
   ADC   (FP),Y
   LDY   #$59
   STA   (FP),Y
   LDY   #$38
   LDA   (FP),Y
   LDY   #$55
   ADC   (FP),Y
   LDY   #$5A
   STA   (FP),Y
   LDY   #$56
   LDA   (FP),Y
   LDY   #$5B
   STA   (FP),Y
   LDY   #$57
   LDA   (FP),Y
   LDY   #$5C
   STA   (FP),Y
   LDY   #$58
   LDA   (FP),Y
   LDY   #$5D
   STA   (FP),Y
   LDY   #$59
   LDA   (FP),Y
   LDY   #$5E
   STA   (FP),Y
   LDY   #$5A
   LDA   (FP),Y
   AND   #$01
   LDY   #$5F
   STA   (FP),Y
   LDY   #$5B
   LDA   (FP),Y
   LDY   #$60
   STA   (FP),Y
   LDY   #$5C
   LDA   (FP),Y
   LDY   #$61
   STA   (FP),Y
   LDY   #$5D
   LDA   (FP),Y
   LDY   #$62
   STA   (FP),Y
   LDY   #$5E
   LDA   (FP),Y
   LDY   #$63
   STA   (FP),Y
   LDY   #$5F
   LDA   (FP),Y
   EOR   #$01
   LDY   #$64
   STA   (FP),Y
   LDY   #$60
   LDA   (FP),Y
   LDY   #$65
   STA   (FP),Y
   LDY   #$61
   LDA   (FP),Y
   LDY   #$66
   STA   (FP),Y
   LDY   #$62
   LDA   (FP),Y
   LDY   #$67
   STA   (FP),Y
   LDY   #$63
   LDA   (FP),Y
   LDY   #$68
   STA   (FP),Y
   LDY   #$64
   LDA   (FP),Y
   SEC
   SBC   #$01
   LDY   #$69
   STA   (FP),Y
   LDY   #$65
   LDA   (FP),Y
   LDY   #$34
   STA   (FP),Y
   LDY   #$66
   LDA   (FP),Y
   LDY   #$35
   STA   (FP),Y
   LDY   #$67
   LDA   (FP),Y
   LDY   #$36
   STA   (FP),Y
   LDY   #$68
   LDA   (FP),Y
   LDY   #$37
   STA   (FP),Y
   LDY   #$69
   LDA   (FP),Y
   LDY   #$38
   STA   (FP),Y
   LDY   #$0D
   LDA   (FP),Y
   LDY   #$6A
   STA   (FP),Y
   LDY   #$0E
   LDA   (FP),Y
   LDY   #$6B
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$6C
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$6D
   STA   (FP),Y
   CLC
   LDA   FP
   ADC   #$6C
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
   LDY   #$6F
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   JSR   f3
   LDA   HARGS+16
   LDY   #$70
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
   LDY   #$34
   LDA   (FP),Y
   LDY   #$76
   STA   (FP),Y
   LDY   #$35
   LDA   (FP),Y
   LDY   #$77
   STA   (FP),Y
   LDY   #$36
   LDA   (FP),Y
   LDY   #$78
   STA   (FP),Y
   LDY   #$37
   LDA   (FP),Y
   LDY   #$79
   STA   (FP),Y
   LDY   #$38
   LDA   (FP),Y
   LDY   #$7A
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@49
   LDA   #$00
   JMP   .sx_done@50
.sx_neg@49:
   LDA   #$FF
.sx_done@50:
   LDY   #$7B
   STA   (FP),Y
   LDY   #$76
   LDA   (FP),Y
   LDY   #$70
   EOR   (FP),Y
   LDY   #$7C
   STA   (FP),Y
   LDY   #$77
   LDA   (FP),Y
   LDY   #$71
   EOR   (FP),Y
   LDY   #$7D
   STA   (FP),Y
   LDY   #$78
   LDA   (FP),Y
   LDY   #$72
   EOR   (FP),Y
   LDY   #$7E
   STA   (FP),Y
   LDY   #$79
   LDA   (FP),Y
   LDY   #$73
   EOR   (FP),Y
   LDY   #$7F
   STA   (FP),Y
   LDY   #$7A
   LDA   (FP),Y
   LDY   #$74
   EOR   (FP),Y
   LDY   #$80
   STA   (FP),Y
   LDY   #$7B
   LDA   (FP),Y
   LDY   #$75
   EOR   (FP),Y
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
   LDY   #$84
   STA   (FP),Y
   LDY   #$7F
   LDA   (FP),Y
   LDY   #$85
   STA   (FP),Y
   LDY   #$80
   LDA   (FP),Y
   LDY   #$86
   STA   (FP),Y
   LDY   #$82
   LDA   (FP),Y
   LDY   #$87
   STA   (FP),Y
   LDY   #$83
   LDA   (FP),Y
   LDY   #$88
   STA   (FP),Y
   LDY   #$84
   LDA   (FP),Y
   LDY   #$89
   STA   (FP),Y
   LDY   #$85
   LDA   (FP),Y
   LDY   #$8A
   STA   (FP),Y
   LDY   #$86
   LDA   (FP),Y
   AND   #$01
   LDY   #$8B
   STA   (FP),Y
   LDY   #$87
   LDA   (FP),Y
   LDY   #$8C
   STA   (FP),Y
   LDY   #$88
   LDA   (FP),Y
   LDY   #$8D
   STA   (FP),Y
   LDY   #$89
   LDA   (FP),Y
   LDY   #$8E
   STA   (FP),Y
   LDY   #$8A
   LDA   (FP),Y
   LDY   #$8F
   STA   (FP),Y
   LDY   #$8B
   LDA   (FP),Y
   EOR   #$01
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
   LDY   #$93
   STA   (FP),Y
   LDY   #$8F
   LDA   (FP),Y
   LDY   #$94
   STA   (FP),Y
   LDY   #$90
   LDA   (FP),Y
   SEC
   SBC   #$01
   LDY   #$95
   STA   (FP),Y
   LDY   #$91
   LDA   (FP),Y
   LDY   #$34
   STA   (FP),Y
   LDY   #$92
   LDA   (FP),Y
   LDY   #$35
   STA   (FP),Y
   LDY   #$93
   LDA   (FP),Y
   LDY   #$36
   STA   (FP),Y
   LDY   #$94
   LDA   (FP),Y
   LDY   #$37
   STA   (FP),Y
   LDY   #$95
   LDA   (FP),Y
   LDY   #$38
   STA   (FP),Y
   LDY   #$6C
   LDA   (FP),Y
   LDY   #$96
   STA   (FP),Y
   LDY   #$6D
   LDA   (FP),Y
   LDY   #$97
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@51
   LDA   #$00
   JMP   .sx_done@52
.sx_neg@51:
   LDA   #$FF
.sx_done@52:
   LDY   #$98
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$34
   LDA   (FP),Y
   CLC
   LDY   #$96
   ADC   (FP),Y
   LDY   #$9B
   STA   (FP),Y
   LDY   #$35
   LDA   (FP),Y
   LDY   #$97
   ADC   (FP),Y
   LDY   #$9C
   STA   (FP),Y
   LDY   #$36
   LDA   (FP),Y
   LDY   #$98
   ADC   (FP),Y
   LDY   #$9D
   STA   (FP),Y
   LDY   #$37
   LDA   (FP),Y
   LDY   #$99
   ADC   (FP),Y
   LDY   #$9E
   STA   (FP),Y
   LDY   #$38
   LDA   (FP),Y
   LDY   #$9A
   ADC   (FP),Y
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
   LDY   #$9E
   LDA   (FP),Y
   LDY   #$A3
   STA   (FP),Y
   LDY   #$9F
   LDA   (FP),Y
   AND   #$01
   LDY   #$A4
   STA   (FP),Y
   LDY   #$A0
   LDA   (FP),Y
   LDY   #$A5
   STA   (FP),Y
   LDY   #$A1
   LDA   (FP),Y
   LDY   #$A6
   STA   (FP),Y
   LDY   #$A2
   LDA   (FP),Y
   LDY   #$A7
   STA   (FP),Y
   LDY   #$A3
   LDA   (FP),Y
   LDY   #$A8
   STA   (FP),Y
   LDY   #$A4
   LDA   (FP),Y
   EOR   #$01
   LDY   #$A9
   STA   (FP),Y
   LDY   #$A5
   LDA   (FP),Y
   LDY   #$AA
   STA   (FP),Y
   LDY   #$A6
   LDA   (FP),Y
   LDY   #$AB
   STA   (FP),Y
   LDY   #$A7
   LDA   (FP),Y
   LDY   #$AC
   STA   (FP),Y
   LDY   #$A8
   LDA   (FP),Y
   LDY   #$AD
   STA   (FP),Y
   LDY   #$A9
   LDA   (FP),Y
   SEC
   SBC   #$01
   LDY   #$AE
   STA   (FP),Y
   LDY   #$AA
   LDA   (FP),Y
   LDY   #$34
   STA   (FP),Y
   LDY   #$AB
   LDA   (FP),Y
   LDY   #$35
   STA   (FP),Y
   LDY   #$AC
   LDA   (FP),Y
   LDY   #$36
   STA   (FP),Y
   LDY   #$AD
   LDA   (FP),Y
   LDY   #$37
   STA   (FP),Y
   LDY   #$AE
   LDA   (FP),Y
   LDY   #$38
   STA   (FP),Y
   LDA   g0
   LDY   #$AF
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
   LDY   #$AF
   LDA   (FP),Y
   CLC
   LDY   #$34
   ADC   (FP),Y
   LDY   #$B4
   STA   (FP),Y
   LDY   #$B0
   LDA   (FP),Y
   LDY   #$35
   ADC   (FP),Y
   LDY   #$B5
   STA   (FP),Y
   LDY   #$B1
   LDA   (FP),Y
   LDY   #$36
   ADC   (FP),Y
   LDY   #$B6
   STA   (FP),Y
   LDY   #$B2
   LDA   (FP),Y
   LDY   #$37
   ADC   (FP),Y
   LDY   #$B7
   STA   (FP),Y
   LDY   #$B3
   LDA   (FP),Y
   LDY   #$38
   ADC   (FP),Y
   LDY   #$B8
   STA   (FP),Y
   LDY   #$B4
   LDA   (FP),Y
   LDY   #$B9
   STA   (FP),Y
   LDY   #$B5
   LDA   (FP),Y
   LDY   #$BA
   STA   (FP),Y
   LDY   #$B6
   LDA   (FP),Y
   LDY   #$BB
   STA   (FP),Y
   LDY   #$B7
   LDA   (FP),Y
   LDY   #$BC
   STA   (FP),Y
   LDY   #$B8
   LDA   (FP),Y
   AND   #$01
   LDY   #$BD
   STA   (FP),Y
   LDY   #$B9
   LDA   (FP),Y
   LDY   #$BE
   STA   (FP),Y
   LDY   #$BA
   LDA   (FP),Y
   LDY   #$BF
   STA   (FP),Y
   LDY   #$BB
   LDA   (FP),Y
   LDY   #$C0
   STA   (FP),Y
   LDY   #$BC
   LDA   (FP),Y
   LDY   #$C1
   STA   (FP),Y
   LDY   #$BD
   LDA   (FP),Y
   EOR   #$01
   LDY   #$C2
   STA   (FP),Y
   LDY   #$BE
   LDA   (FP),Y
   LDY   #$C3
   STA   (FP),Y
   LDY   #$BF
   LDA   (FP),Y
   LDY   #$C4
   STA   (FP),Y
   LDY   #$C0
   LDA   (FP),Y
   LDY   #$C5
   STA   (FP),Y
   LDY   #$C1
   LDA   (FP),Y
   LDY   #$C6
   STA   (FP),Y
   LDY   #$C2
   LDA   (FP),Y
   SEC
   SBC   #$01
   LDY   #$C7
   STA   (FP),Y
   LDY   #$C3
   LDA   (FP),Y
   LDY   #$C8
   STA   (FP),Y
   LDY   #$C4
   LDA   (FP),Y
   LDY   #$C9
   STA   (FP),Y
   LDY   #$C5
   LDA   (FP),Y
   LDY   #$CA
   STA   (FP),Y
   LDY   #$C6
   LDA   (FP),Y
   LDY   #$CB
   STA   (FP),Y
   LDY   #$C8
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
   LDY   #$20
   LDA   (FP),Y
   LDY   #$CC
   STA   (FP),Y
   LDY   #$21
   LDA   (FP),Y
   LDY   #$CD
   STA   (FP),Y
   LDY   #$22
   LDA   (FP),Y
   LDY   #$CE
   STA   (FP),Y
   LDY   #$23
   LDA   (FP),Y
   LDY   #$CF
   STA   (FP),Y
   LDY   #$CC
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
   ADC   #$D1
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

   ; prologue: 0 arg bytes, 63 local bytes
   SEC
   LDA   SSP
   SBC   #$41
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$40
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   LDA   g0
   LDY   #$01
   STA   (FP),Y
   LDA   g0+1
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   AND   #$07
   LDY   #$03
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDA   #$00
   LDY   #$04
   STA   (FP),Y
   LDA   #$07
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$03
   LDA   (FP),Y
   TAX
   BEQ   .shift_done@54
.shift_loop@53:
   LDY   #$05
   LDA   (FP),Y
   ASL
   STA   (FP),Y
   INY
   LDA   (FP),Y
   ROL
   STA   (FP),Y
   DEX
   BNE   .shift_loop@53
.shift_done@54:
   LDY   #$05
   LDA   (FP),Y
   LDY   #$07
   STA   (FP),Y
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
   LDY   #$0A
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$0B
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$0C
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
.loop@0_start:
   LDY   #$0D
   LDA   (FP),Y
   SEC
   SBC   #$03
   INY
   LDA   (FP),Y
   SBC   #$00
   BVC   .cmp_novf@57
   EOR   #$80
.cmp_novf@57:
   ASL
   LDA   #$00
   ROL
   LDY   #$0F
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
   CLC
   LDA   FP
   ADC   #$0B
   LDY   #$11
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
   LDY   #$12
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   JSR   f4
   LDA   HARGS+8
   LDY   #$13
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
   LDY   #$0D
   LDA   (FP),Y
   LDY   #$17
   STA   (FP),Y
   LDY   #$0E
   LDA   (FP),Y
   LDY   #$18
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@58
   LDA   #$00
   JMP   .sx_done@59
.sx_neg@58:
   LDA   #$FF
.sx_done@59:
   LDY   #$19
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$13
   LDA   (FP),Y
   CLC
   LDY   #$17
   ADC   (FP),Y
   LDY   #$1B
   STA   (FP),Y
   LDY   #$14
   LDA   (FP),Y
   LDY   #$18
   ADC   (FP),Y
   LDY   #$1C
   STA   (FP),Y
   LDY   #$15
   LDA   (FP),Y
   LDY   #$19
   ADC   (FP),Y
   LDY   #$1D
   STA   (FP),Y
   LDY   #$16
   LDA   (FP),Y
   LDY   #$1A
   ADC   (FP),Y
   LDY   #$1E
   STA   (FP),Y
   LDY   #$08
   LDA   (FP),Y
   LDY   #$1F
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@60
   LDA   #$00
   JMP   .sx_done@61
.sx_neg@60:
   LDA   #$FF
.sx_done@61:
   LDY   #$20
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$1F
   LDA   (FP),Y
   CLC
   LDY   #$1B
   ADC   (FP),Y
   LDY   #$23
   STA   (FP),Y
   LDY   #$20
   LDA   (FP),Y
   LDY   #$1C
   ADC   (FP),Y
   LDY   #$24
   STA   (FP),Y
   LDY   #$21
   LDA   (FP),Y
   LDY   #$1D
   ADC   (FP),Y
   LDY   #$25
   STA   (FP),Y
   LDY   #$22
   LDA   (FP),Y
   LDY   #$1E
   ADC   (FP),Y
   LDY   #$26
   STA   (FP),Y
   LDY   #$23
   LDA   (FP),Y
   LDY   #$27
   STA   (FP),Y
   LDY   #$08
   STA   (FP),Y
   LDY   #$0D
   LDA   (FP),Y
   LDY   #$28
   STA   (FP),Y
   LDY   #$0E
   LDA   (FP),Y
   LDY   #$29
   STA   (FP),Y
   LDY   #$0D
   LDA   (FP),Y
   CLC
   ADC   #$01
   LDY   #$2A
   STA   (FP),Y
   LDY   #$0E
   LDA   (FP),Y
   ADC   #$00
   LDY   #$2B
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$0D
   STA   (FP),Y
   LDY   #$2B
   LDA   (FP),Y
   LDY   #$0E
   STA   (FP),Y
   JMP   .loop@0_start
.loop@0_break:
   LDY   #$08
   LDA   (FP),Y
   LDY   #$2C
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@62
   LDA   #$00
   JMP   .sx_done@63
.sx_neg@62:
   LDA   #$FF
.sx_done@63:
   LDY   #$2D
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   LDY   #$0B
   ADC   (FP),Y
   LDY   #$2E
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$0C
   ADC   (FP),Y
   LDY   #$2F
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$30
   STA   (FP),Y
   LDY   #$08
   STA   (FP),Y
   LDY   #$31
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@64
   LDA   #$00
   JMP   .sx_done@65
.sx_neg@64:
   LDA   #$FF
.sx_done@65:
   LDY   #$32
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
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
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   ORA   #$00
   BPL   .sx_done@67
   LDA   #$FF
.sx_done@67:
   LDY   #$3B
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   g0+3
   CMP   (FP),Y
   BNE   .cmp_true@68
   LDA   g0+2
   DEY
   CMP   (FP),Y
   BNE   .cmp_true@68
   LDA   g0+1
   DEY
   CMP   (FP),Y
   BNE   .cmp_true@68
   LDA   g0
   DEY
   CMP   (FP),Y
   BNE   .cmp_true@68
   LDA   #$00
   JMP   .cmp_end@69
.cmp_true@68:
   LDA   #$01
.cmp_end@69:
   LDY   #$3D
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$3F
   STA   (FP),Y
   CMP   #$01

   ; epilogue
   PHP
   CLC
   LDA   FP
   ADC   #$41
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

f0:
   SUBROUTINE

   ; prologue: 15 arg bytes, 85 local bytes
   SEC
   LDA   SSP
   SBC   #$57
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$56
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   LDY   #$5C
   LDA   (FP),Y
   CLC
   ADC   (FP),Y
   LDY   #$01
   STA   (FP),Y
   LDY   #$5D
   LDA   (FP),Y
   ADC   (FP),Y
   LDY   #$02
   STA   (FP),Y
   LDY   #$5E
   LDA   (FP),Y
   ADC   (FP),Y
   LDY   #$03
   STA   (FP),Y
   LDY   #$5F
   LDA   (FP),Y
   ADC   (FP),Y
   LDY   #$04
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$01
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
   LDY   #$07
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
   LDY   #$07
   LDA   (FP),Y
   LDY   #$0B
   STA   (FP),Y
   LDY   #$08
   LDA   (FP),Y
   LDY   #$0C
   STA   (FP),Y
   LDY   #$09
   LDA   (FP),Y
   LDY   #$0D
   STA   (FP),Y
   LDY   #$0A
   LDA   (FP),Y
   LDY   #$0E
   STA   (FP),Y
   LDY   #$60
   LDA   (FP),Y
   LDY   #$0F
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
   BMI   .sx_neg@71
   LDA   #$00
   JMP   .sx_done@72
.sx_neg@71:
   LDA   #$FF
.sx_done@72:
   LDY   #$13
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$11
   LDA   (FP),Y
   CLC
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
   LDY   #$17
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$18
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@73
   LDA   #$00
   JMP   .sx_done@74
.sx_neg@73:
   LDA   #$FF
.sx_done@74:
   LDY   #$19
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$17
   LDA   (FP),Y
   LDY   #$1B
   STA   (FP),Y
   LDY   #$18
   LDA   (FP),Y
   LDY   #$1C
   STA   (FP),Y
   LDY   #$19
   LDA   (FP),Y
   LDY   #$1D
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$0B
   LDA   (FP),Y
   LDY   #$1F
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$65
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$21
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$22
   STA   (FP),Y
   LDA   #$03
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$21
   LDA   (FP),Y
   CLC
   LDY   #$23
   ADC   (FP),Y
   LDY   #$25
   STA   (FP),Y
   LDY   #$22
   LDA   (FP),Y
   LDY   #$24
   ADC   (FP),Y
   LDY   #$26
   STA   (FP),Y
   LDY   #$65
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$25
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$26
   LDA   (FP),Y
   LDY   #$01
   STA   (DPTR),Y
   LDY   #$0B
   LDA   (FP),Y
   LDY   #$27
   STA   (FP),Y
   LDY   #$0C
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
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
.loop@1_start:
   LDY   #$2B
   LDA   (FP),Y
   SEC
   SBC   #$03
   INY
   LDA   (FP),Y
   SBC   #$00
   BVC   .cmp_novf@75
   EOR   #$80
.cmp_novf@75:
   ASL
   LDA   #$00
   ROL
   LDY   #$2D
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
   CLC
   LDA   FP
   ADC   #$29
   LDY   #$2F
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
   LDY   #$30
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   JSR   f4
   LDA   HARGS+8
   LDY   #$31
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
   LDY   #$2B
   LDA   (FP),Y
   LDY   #$35
   STA   (FP),Y
   LDY   #$2C
   LDA   (FP),Y
   LDY   #$36
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@76
   LDA   #$00
   JMP   .sx_done@77
.sx_neg@76:
   LDA   #$FF
.sx_done@77:
   LDY   #$37
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$31
   LDA   (FP),Y
   CLC
   LDY   #$35
   ADC   (FP),Y
   LDY   #$39
   STA   (FP),Y
   LDY   #$32
   LDA   (FP),Y
   LDY   #$36
   ADC   (FP),Y
   LDY   #$3A
   STA   (FP),Y
   LDY   #$33
   LDA   (FP),Y
   LDY   #$37
   ADC   (FP),Y
   LDY   #$3B
   STA   (FP),Y
   LDY   #$34
   LDA   (FP),Y
   LDY   #$38
   ADC   (FP),Y
   LDY   #$3C
   STA   (FP),Y
   LDY   #$0B
   LDA   (FP),Y
   CLC
   LDY   #$39
   ADC   (FP),Y
   LDY   #$3D
   STA   (FP),Y
   LDY   #$0C
   LDA   (FP),Y
   LDY   #$3A
   ADC   (FP),Y
   LDY   #$3E
   STA   (FP),Y
   LDY   #$0D
   LDA   (FP),Y
   LDY   #$3B
   ADC   (FP),Y
   LDY   #$3F
   STA   (FP),Y
   LDY   #$0E
   LDA   (FP),Y
   LDY   #$3C
   ADC   (FP),Y
   LDY   #$40
   STA   (FP),Y
   LDY   #$3D
   LDA   (FP),Y
   LDY   #$0B
   STA   (FP),Y
   LDY   #$3E
   LDA   (FP),Y
   LDY   #$0C
   STA   (FP),Y
   LDY   #$3F
   LDA   (FP),Y
   LDY   #$0D
   STA   (FP),Y
   LDY   #$40
   LDA   (FP),Y
   LDY   #$0E
   STA   (FP),Y
   LDY   #$2B
   LDA   (FP),Y
   LDY   #$41
   STA   (FP),Y
   LDY   #$2C
   LDA   (FP),Y
   LDY   #$42
   STA   (FP),Y
   LDY   #$2B
   LDA   (FP),Y
   CLC
   ADC   #$01
   LDY   #$43
   STA   (FP),Y
   LDY   #$2C
   LDA   (FP),Y
   ADC   #$00
   LDY   #$44
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$2B
   STA   (FP),Y
   LDY   #$44
   LDA   (FP),Y
   LDY   #$2C
   STA   (FP),Y
   JMP   .loop@1_start
.loop@1_break:
   LDY   #$29
   LDA   (FP),Y
   LDY   #$45
   STA   (FP),Y
   LDY   #$2A
   LDA   (FP),Y
   LDY   #$46
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@78
   LDA   #$00
   JMP   .sx_done@79
.sx_neg@78:
   LDA   #$FF
.sx_done@79:
   LDY   #$47
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$0B
   LDA   (FP),Y
   CLC
   LDY   #$45
   ADC   (FP),Y
   LDY   #$49
   STA   (FP),Y
   LDY   #$0C
   LDA   (FP),Y
   LDY   #$46
   ADC   (FP),Y
   LDY   #$4A
   STA   (FP),Y
   LDY   #$0D
   LDA   (FP),Y
   LDY   #$47
   ADC   (FP),Y
   LDY   #$4B
   STA   (FP),Y
   LDY   #$0E
   LDA   (FP),Y
   LDY   #$48
   ADC   (FP),Y
   LDY   #$4C
   STA   (FP),Y
   LDY   #$49
   LDA   (FP),Y
   LDY   #$0B
   STA   (FP),Y
   LDY   #$4A
   LDA   (FP),Y
   LDY   #$0C
   STA   (FP),Y
   LDY   #$4B
   LDA   (FP),Y
   LDY   #$0D
   STA   (FP),Y
   LDY   #$4C
   LDA   (FP),Y
   LDY   #$0E
   STA   (FP),Y
   LDA   g0
   CLC
   LDY   #$0B
   ADC   (FP),Y
   LDY   #$4D
   STA   (FP),Y
   LDA   g0+1
   LDY   #$0C
   ADC   (FP),Y
   LDY   #$4E
   STA   (FP),Y
   LDA   g0+2
   LDY   #$0D
   ADC   (FP),Y
   LDY   #$4F
   STA   (FP),Y
   LDA   g0+3
   LDY   #$0E
   ADC   (FP),Y
   LDY   #$50
   STA   (FP),Y
   LDY   #$4D
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
   LDY   #$60
   LDA   (FP),Y
   LDY   #$51
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@80
   LDA   #$00
   JMP   .cmp_end@81
.cmp_true@80:
   LDA   #$01
.cmp_end@81:
   LDY   #$53
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$55
   STA   (FP),Y
   CMP   #$01

   ; epilogue
   PHP
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
   PLP
   RTS

main:
   SUBROUTINE

   ; prologue: 0 arg bytes, 44 local bytes
   SEC
   LDA   SSP
   SBC   #$2E
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$2D
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
   LDA   #$AF
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   ORA   #$00
   BPL   .sx_done@83
   LDA   #$FF
.sx_done@83:
   LDY   #$05
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$67
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   ORA   #$00
   BPL   .sx_done@85
   LDA   #$FF
.sx_done@85:
   LDY   #$09
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$2C
   INY
   STA   (FP),Y
   LDA   #$05
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   ORA   #$00
   BPL   .sx_done@87
   LDA   #$FF
.sx_done@87:
   LDY   #$0E
   STA   (FP),Y
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
   SBC   #$0F
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDY   #$03
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$04
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$05
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$06
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   LDY   #$07
   LDA   (FP),Y
   LDY   #$05
   STA   (SSP),Y
   LDY   #$08
   LDA   (FP),Y
   LDY   #$06
   STA   (SSP),Y
   LDY   #$09
   LDA   (FP),Y
   LDY   #$07
   STA   (SSP),Y
   LDY   #$0A
   LDA   (FP),Y
   LDY   #$08
   STA   (SSP),Y
   LDY   #$0B
   LDA   (FP),Y
   LDY   #$09
   STA   (SSP),Y
   LDY   #$0C
   LDA   (FP),Y
   LDY   #$0A
   STA   (SSP),Y
   LDY   #$0D
   LDA   (FP),Y
   LDY   #$0B
   STA   (SSP),Y
   LDY   #$0E
   LDA   (FP),Y
   LDY   #$0C
   STA   (SSP),Y
   LDY   #$0F
   LDA   (FP),Y
   LDY   #$0D
   STA   (SSP),Y
   LDY   #$10
   LDA   (FP),Y
   LDY   #$0E
   STA   (SSP),Y
   LDY   #$11
   LDA   (FP),Y
   LDY   #$0F
   STA   (SSP),Y
   JSR   f0
   LDA   #$00
   ROL
   LDY   #$12
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
   LDY   #$13
   LDA   (FP),Y
   LDY   #$17
   STA   (FP),Y
   LDY   #$14
   LDA   (FP),Y
   LDY   #$18
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
   LDY   #$17
   LDA   (FP),Y
   LDY   #$1B
   EOR   (FP),Y
   LDY   #$1F
   STA   (FP),Y
   LDY   #$18
   LDA   (FP),Y
   LDY   #$1C
   EOR   (FP),Y
   LDY   #$20
   STA   (FP),Y
   LDY   #$19
   LDA   (FP),Y
   LDY   #$1D
   EOR   (FP),Y
   LDY   #$21
   STA   (FP),Y
   LDY   #$1A
   LDA   (FP),Y
   LDY   #$1E
   EOR   (FP),Y
   LDY   #$22
   STA   (FP),Y
   LDY   #$1F
   LDA   (FP),Y
   LDY   #$17
   STA   (FP),Y
   LDY   #$20
   LDA   (FP),Y
   LDY   #$18
   STA   (FP),Y
   LDY   #$21
   LDA   (FP),Y
   LDY   #$19
   STA   (FP),Y
   LDY   #$22
   LDA   (FP),Y
   LDY   #$1A
   STA   (FP),Y
   LDY   #$17
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
   LDY   #$17
   LDA   (FP),Y
   LDY   #$23
   EOR   (FP),Y
   LDY   #$27
   STA   (FP),Y
   LDY   #$18
   LDA   (FP),Y
   LDY   #$24
   EOR   (FP),Y
   LDY   #$28
   STA   (FP),Y
   LDY   #$19
   LDA   (FP),Y
   LDY   #$25
   EOR   (FP),Y
   LDY   #$29
   STA   (FP),Y
   LDY   #$1A
   LDA   (FP),Y
   LDY   #$26
   EOR   (FP),Y
   LDY   #$2A
   STA   (FP),Y
   LDY   #$27
   LDA   (FP),Y
   LDY   #$2B
   STA   (FP),Y
   LDY   #$28
   LDA   (FP),Y
   LDY   #$2C
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
   ADC   #$2E
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$2D
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
   DC.L  $00000027
