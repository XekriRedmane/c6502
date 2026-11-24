SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24

f4:
   SUBROUTINE

   ; prologue: 0 arg bytes, 102 local bytes
   SEC
   LDA   SSP
   SBC   #$68
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$67
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   LDA   g0
   AND   #$07
   LDY   #$01
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
   LDA   g0
   STA   HARGS
   LDA   g0+1
   STA   HARGS+1
   LDA   g0+2
   STA   HARGS+2
   LDA   g0+3
   STA   HARGS+3
   LDA   g0+4
   STA   HARGS+4
   LDA   g0+5
   STA   HARGS+5
   ASL
   LDA   #$00
   ADC   #$FF
   EOR   #$FF
   STA   HARGS+6
   STA   HARGS+7
   LDY   #$01
   LDA   (FP),Y
   STA   HARGS+8
   JSR   asr64
   LDA   HARGS+9
   LDY   #$07
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
   LDA   g0
   LDY   #$07
   ORA   (FP),Y
   LDY   #$0D
   STA   (FP),Y
   LDA   g0+1
   LDY   #$08
   ORA   (FP),Y
   LDY   #$0E
   STA   (FP),Y
   LDA   g0+2
   LDY   #$09
   ORA   (FP),Y
   LDY   #$0F
   STA   (FP),Y
   LDA   g0+3
   LDY   #$0A
   ORA   (FP),Y
   LDY   #$10
   STA   (FP),Y
   LDA   g0+4
   LDY   #$0B
   ORA   (FP),Y
   LDY   #$11
   STA   (FP),Y
   LDA   g0+5
   LDY   #$0C
   ORA   (FP),Y
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
   LDY   #$13
   LDA   (FP),Y
   LDY   #$18
   STA   (FP),Y
   LDY   #$14
   LDA   (FP),Y
   LDY   #$19
   STA   (FP),Y
   LDY   #$15
   LDA   (FP),Y
   LDY   #$1A
   STA   (FP),Y
   LDY   #$16
   LDA   (FP),Y
   LDY   #$1B
   STA   (FP),Y
   LDY   #$17
   LDA   (FP),Y
   AND   #$01
   LDY   #$1C
   STA   (FP),Y
   LDY   #$18
   LDA   (FP),Y
   LDY   #$1D
   STA   (FP),Y
   LDY   #$19
   LDA   (FP),Y
   LDY   #$1E
   STA   (FP),Y
   LDY   #$1A
   LDA   (FP),Y
   LDY   #$1F
   STA   (FP),Y
   LDY   #$1B
   LDA   (FP),Y
   LDY   #$20
   STA   (FP),Y
   LDY   #$1C
   LDA   (FP),Y
   EOR   #$01
   LDY   #$21
   STA   (FP),Y
   LDY   #$1D
   LDA   (FP),Y
   LDY   #$22
   STA   (FP),Y
   LDY   #$1E
   LDA   (FP),Y
   LDY   #$23
   STA   (FP),Y
   LDY   #$1F
   LDA   (FP),Y
   LDY   #$24
   STA   (FP),Y
   LDY   #$20
   LDA   (FP),Y
   LDY   #$25
   STA   (FP),Y
   LDY   #$21
   LDA   (FP),Y
   SEC
   SBC   #$01
   LDY   #$26
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
   LDY   #$25
   LDA   (FP),Y
   LDY   #$2A
   STA   (FP),Y
   LDY   #$26
   LDA   (FP),Y
   LDY   #$2B
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
   LDA   g0+4
   INY
   STA   (FP),Y
   LDY   #$2C
   LDA   (FP),Y
   LDY   #$31
   STA   (FP),Y
   LDY   #$2D
   LDA   (FP),Y
   LDY   #$32
   STA   (FP),Y
   LDY   #$2E
   LDA   (FP),Y
   LDY   #$33
   STA   (FP),Y
   LDY   #$2F
   LDA   (FP),Y
   LDY   #$34
   STA   (FP),Y
   LDY   #$30
   LDA   (FP),Y
   AND   #$01
   LDY   #$35
   STA   (FP),Y
   LDY   #$31
   LDA   (FP),Y
   LDY   #$36
   STA   (FP),Y
   LDY   #$32
   LDA   (FP),Y
   LDY   #$37
   STA   (FP),Y
   LDY   #$33
   LDA   (FP),Y
   LDY   #$38
   STA   (FP),Y
   LDY   #$34
   LDA   (FP),Y
   LDY   #$39
   STA   (FP),Y
   LDY   #$35
   LDA   (FP),Y
   EOR   #$01
   LDY   #$3A
   STA   (FP),Y
   LDY   #$36
   LDA   (FP),Y
   LDY   #$3B
   STA   (FP),Y
   LDY   #$37
   LDA   (FP),Y
   LDY   #$3C
   STA   (FP),Y
   LDY   #$38
   LDA   (FP),Y
   LDY   #$3D
   STA   (FP),Y
   LDY   #$39
   LDA   (FP),Y
   LDY   #$3E
   STA   (FP),Y
   LDY   #$3A
   LDA   (FP),Y
   SEC
   SBC   #$01
   LDY   #$3F
   STA   (FP),Y
   LDY   #$3B
   LDA   (FP),Y
   LDY   #$40
   STA   (FP),Y
   LDY   #$3C
   LDA   (FP),Y
   LDY   #$41
   STA   (FP),Y
   LDY   #$3D
   LDA   (FP),Y
   LDY   #$42
   STA   (FP),Y
   LDY   #$3E
   LDA   (FP),Y
   LDY   #$43
   STA   (FP),Y
   LDY   #$3F
   LDA   (FP),Y
   LDY   #$44
   STA   (FP),Y
   LDY   #$40
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   INY
   ORA   (FP),Y
   INY
   ORA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@0
   LDA   #$FF
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
   LDY   #$45
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@1:
   LDY   #$45
   LDA   (FP),Y
   LDY   #$47
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$48
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@0
   LDA   #$00
   JMP   .sx_done@1
.sx_neg@0:
   LDA   #$FF
.sx_done@1:
   LDY   #$49
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$40
   LDA   (FP),Y
   LDY   #$47
   ORA   (FP),Y
   LDY   #$4C
   STA   (FP),Y
   LDY   #$41
   LDA   (FP),Y
   LDY   #$48
   ORA   (FP),Y
   LDY   #$4D
   STA   (FP),Y
   LDY   #$42
   LDA   (FP),Y
   LDY   #$49
   ORA   (FP),Y
   LDY   #$4E
   STA   (FP),Y
   LDY   #$43
   LDA   (FP),Y
   LDY   #$4A
   ORA   (FP),Y
   LDY   #$4F
   STA   (FP),Y
   LDY   #$44
   LDA   (FP),Y
   LDY   #$4B
   ORA   (FP),Y
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
   DEY
   LDA   (FP),Y
   LDY   #$53
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$54
   STA   (FP),Y
   LDY   #$27
   LDA   (FP),Y
   LDY   #$55
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$58
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$59
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@2
   LDA   #$00
   JMP   .sx_done@3
.sx_neg@2:
   LDA   #$FF
.sx_done@3:
   LDY   #$5A
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$58
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
   INY
   LDA   (FP),Y
   SBC   g0+4
   INY
   LDA   (FP),Y
   SBC   g0+5
   BVC   .cmp_novf@4
   EOR   #$80
.cmp_novf@4:
   ASL
   LDA   #$00
   ROL
   EOR   #$01
   LDY   #$5E
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$60
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$27
   LDA   (FP),Y
   LDY   #$62
   STA   (FP),Y
   LDY   #$28
   LDA   (FP),Y
   LDY   #$63
   STA   (FP),Y
   SEC
   LDA   #$00
   DEY
   SBC   (FP),Y
   LDY   #$64
   STA   (FP),Y
   LDA   #$00
   DEY
   SBC   (FP),Y
   LDY   #$65
   STA   (FP),Y
   DEY
   ORA   (FP),Y
   BEQ   .cmp_end@5
   LDA   #$01
.cmp_end@5:
   LDY   #$66
   STA   (FP),Y

   ; epilogue
   PHA
   CLC
   LDA   FP
   ADC   #$68
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

f3:
   SUBROUTINE

   ; prologue: 0 arg bytes, 45 local bytes
   SEC
   LDA   SSP
   SBC   #$2F
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$2E
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   LDA   g0
   EOR   g0
   LDY   #$01
   STA   (FP),Y
   LDA   g0+1
   EOR   g0+1
   INY
   STA   (FP),Y
   LDA   g0+2
   EOR   g0+2
   INY
   STA   (FP),Y
   LDA   g0+3
   EOR   g0+3
   INY
   STA   (FP),Y
   LDA   g0+4
   EOR   g0+4
   INY
   STA   (FP),Y
   LDA   g0+5
   EOR   g0+5
   INY
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
   LDY   #$0E
   LDA   (FP),Y
   LDY   #$12
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$0F
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
   INY
   LDA   (FP),Y
   SBC   g0+4
   INY
   LDA   (FP),Y
   SBC   g0+5
   BVC   .cmp_novf@6
   EOR   #$80
.cmp_novf@6:
   ASL
   LDA   #$00
   ROL
   LDY   #$15
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
   BMI   .sx_neg@7
   LDA   #$00
   JMP   .sx_done@8
.sx_neg@7:
   LDA   #$FF
.sx_done@8:
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
   LDY   #$0E
   LDA   (FP),Y
   LDY   #$1E
   CMP   (FP),Y
   BNE   .cmp_true@9
   LDY   #$0D
   LDA   (FP),Y
   LDY   #$1D
   CMP   (FP),Y
   BNE   .cmp_true@9
   LDY   #$0C
   LDA   (FP),Y
   LDY   #$1C
   CMP   (FP),Y
   BNE   .cmp_true@9
   LDY   #$0B
   LDA   (FP),Y
   LDY   #$1B
   CMP   (FP),Y
   BNE   .cmp_true@9
   LDA   #$00
   JMP   .cmp_end@10
.cmp_true@9:
   LDA   #$01
.cmp_end@10:
   LDY   #$1F
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$21
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$22
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@12
   LDA   #$00
   JMP   .sx_done@13
.sx_neg@12:
   LDA   #$FF
.sx_done@13:
   LDY   #$23
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$21
   LDA   (FP),Y
   SEC
   LDY   #$1B
   SBC   (FP),Y
   LDY   #$25
   STA   (FP),Y
   LDY   #$22
   LDA   (FP),Y
   LDY   #$1C
   SBC   (FP),Y
   LDY   #$26
   STA   (FP),Y
   LDY   #$23
   LDA   (FP),Y
   LDY   #$1D
   SBC   (FP),Y
   LDY   #$27
   STA   (FP),Y
   LDY   #$24
   LDA   (FP),Y
   LDY   #$1E
   SBC   (FP),Y
   LDY   #$28
   STA   (FP),Y
   LDY   #$25
   LDA   (FP),Y
   LDY   #$29
   STA   (FP),Y
   LDY   #$26
   LDA   (FP),Y
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
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$29
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

   ; epilogue
   CLC
   LDA   FP
   ADC   #$2F
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

   ; prologue: 12 arg bytes, 86 local bytes
   SEC
   LDA   SSP
   SBC   #$58
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$57
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   INY
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   LDY   #$5A
   LDA   (FP),Y
   AND   #$7F
   LDY   #$02
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$03
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$04
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
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   JSR   f4
   LDY   #$0F
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
   LDY   #$09
   LDA   (FP),Y
   LDY   #$10
   EOR   (FP),Y
   LDY   #$16
   STA   (FP),Y
   LDY   #$0A
   LDA   (FP),Y
   LDY   #$11
   EOR   (FP),Y
   LDY   #$17
   STA   (FP),Y
   LDY   #$0B
   LDA   (FP),Y
   LDY   #$12
   EOR   (FP),Y
   LDY   #$18
   STA   (FP),Y
   LDY   #$0C
   LDA   (FP),Y
   LDY   #$13
   EOR   (FP),Y
   LDY   #$19
   STA   (FP),Y
   LDY   #$0D
   LDA   (FP),Y
   LDY   #$14
   EOR   (FP),Y
   LDY   #$1A
   STA   (FP),Y
   LDY   #$0E
   LDA   (FP),Y
   LDY   #$15
   EOR   (FP),Y
   LDY   #$1B
   STA   (FP),Y
   LDY   #$16
   LDA   (FP),Y
   LDY   #$09
   STA   (FP),Y
   LDY   #$17
   LDA   (FP),Y
   LDY   #$0A
   STA   (FP),Y
   LDY   #$18
   LDA   (FP),Y
   LDY   #$0B
   STA   (FP),Y
   LDY   #$19
   LDA   (FP),Y
   LDY   #$0C
   STA   (FP),Y
   LDY   #$1A
   LDA   (FP),Y
   LDY   #$0D
   STA   (FP),Y
   LDY   #$1B
   LDA   (FP),Y
   LDY   #$0E
   STA   (FP),Y
   LDA   g0
   CLC
   LDY   #$09
   ADC   (FP),Y
   LDY   #$1C
   STA   (FP),Y
   LDA   g0+1
   LDY   #$0A
   ADC   (FP),Y
   LDY   #$1D
   STA   (FP),Y
   LDA   g0+2
   LDY   #$0B
   ADC   (FP),Y
   LDY   #$1E
   STA   (FP),Y
   LDA   g0+3
   LDY   #$0C
   ADC   (FP),Y
   LDY   #$1F
   STA   (FP),Y
   LDA   g0+4
   LDY   #$0D
   ADC   (FP),Y
   LDY   #$20
   STA   (FP),Y
   LDA   g0+5
   LDY   #$0E
   ADC   (FP),Y
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
   LDY   #$21
   LDA   (FP),Y
   AND   #$0F
   LDY   #$27
   STA   (FP),Y
   LDY   #$22
   LDA   (FP),Y
   LDY   #$28
   STA   (FP),Y
   LDY   #$23
   LDA   (FP),Y
   LDY   #$29
   STA   (FP),Y
   LDY   #$24
   LDA   (FP),Y
   LDY   #$2A
   STA   (FP),Y
   LDY   #$25
   LDA   (FP),Y
   LDY   #$2B
   STA   (FP),Y
   LDY   #$26
   LDA   (FP),Y
   LDY   #$2C
   STA   (FP),Y
   LDY   #$27
   LDA   (FP),Y
   EOR   #$08
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
   LDY   #$2D
   LDA   (FP),Y
   SEC
   SBC   #$08
   LDY   #$33
   STA   (FP),Y
   LDY   #$2E
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
   INY
   LDA   (FP),Y
   STA   g0+4
   INY
   LDA   (FP),Y
   STA   g0+5
   LDY   #$5B
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   INY
   ORA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lnot_true@14
   LDA   #$00
   JMP   .lnot_end@15
.lnot_true@14:
   LDA   #$01
.lnot_end@15:
   LDY   #$34
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$36
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$37
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@16
   LDA   #$00
   JMP   .sx_done@17
.sx_neg@16:
   LDA   #$FF
.sx_done@17:
   LDY   #$38
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$5F
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
   LDA   g0
   STA   HARGS+8
   LDA   g0+1
   STA   HARGS+9
   LDA   g0+2
   STA   HARGS+10
   LDA   g0+3
   STA   HARGS+11
   LDA   g0+4
   STA   HARGS+12
   LDA   g0+5
   STA   HARGS+13
   LDA   #$00
   STA   HARGS+6
   STA   HARGS+7
   STA   HARGS+14
   STA   HARGS+15
   JSR   mul64
   LDA   HARGS+16
   LDY   #$3C
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
   LDY   #$3C
   LDA   (FP),Y
   LDY   #$42
   STA   (FP),Y
   LDY   #$3D
   LDA   (FP),Y
   LDY   #$43
   STA   (FP),Y
   LDY   #$3E
   LDA   (FP),Y
   LDY   #$44
   STA   (FP),Y
   LDY   #$3F
   LDA   (FP),Y
   LDY   #$45
   STA   (FP),Y
   LDY   #$40
   LDA   (FP),Y
   LDY   #$46
   STA   (FP),Y
   LDY   #$41
   LDA   (FP),Y
   AND   #$0F
   LDY   #$47
   STA   (FP),Y
   LDY   #$42
   LDA   (FP),Y
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
   EOR   #$08
   LDY   #$4D
   STA   (FP),Y
   LDY   #$48
   LDA   (FP),Y
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
   SEC
   SBC   #$08
   LDY   #$53
   STA   (FP),Y
   LDY   #$3B
   LDA   (FP),Y
   LDY   #$53
   CMP   (FP),Y
   BNE   .thread@0
   LDY   #$3A
   LDA   (FP),Y
   LDY   #$52
   CMP   (FP),Y
   BNE   .thread@0
   LDY   #$39
   LDA   (FP),Y
   LDY   #$51
   CMP   (FP),Y
   BNE   .thread@0
   LDY   #$38
   LDA   (FP),Y
   LDY   #$50
   CMP   (FP),Y
   BNE   .thread@0
   LDY   #$37
   LDA   (FP),Y
   LDY   #$4F
   CMP   (FP),Y
   BNE   .thread@0
   LDY   #$36
   LDA   (FP),Y
   LDY   #$4E
   CMP   (FP),Y
   BEQ   .cmp_true@18
.thread@0:
   LDA   #$00
   JMP   .cmp_end@19
.cmp_true@18:
   LDA   #$01
.cmp_end@19:
   LDY   #$54
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$56
   STA   (FP),Y
   CMP   #$01

   ; epilogue
   PHP
   CLC
   LDA   FP
   ADC   #$64
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

f1:
   SUBROUTINE

   ; prologue: 16 arg bytes, 48 local bytes
   SEC
   LDA   SSP
   SBC   #$32
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$31
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   LDY   #$3F
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   LDY   #$40
   LDA   (FP),Y
   LDY   #$02
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
   LDY   #$38
   LDA   (FP),Y
   SEC
   LDY   #$01
   SBC   (FP),Y
   LDY   #$39
   LDA   (FP),Y
   LDY   #$02
   SBC   (FP),Y
   LDY   #$3A
   LDA   (FP),Y
   LDY   #$03
   SBC   (FP),Y
   LDY   #$3B
   LDA   (FP),Y
   LDY   #$04
   SBC   (FP),Y
   LDY   #$3C
   LDA   (FP),Y
   LDY   #$05
   SBC   (FP),Y
   LDY   #$3D
   LDA   (FP),Y
   LDY   #$06
   SBC   (FP),Y
   BVC   .cmp_novf@21
   EOR   #$80
.cmp_novf@21:
   ASL
   LDA   #$00
   ROL
   EOR   #$01
   LDY   #$07
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$09
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$0A
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@22
   LDA   #$00
   JMP   .sx_done@23
.sx_neg@22:
   LDA   #$FF
.sx_done@23:
   LDY   #$0B
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$09
   LDA   (FP),Y
   LDY   #$0D
   STA   (FP),Y
   LDY   #$0A
   LDA   (FP),Y
   LDY   #$0E
   STA   (FP),Y
   LDY   #$0B
   LDA   (FP),Y
   LDY   #$0F
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$41
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$11
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$12
   STA   (FP),Y
   LDY   #$38
   LDA   (FP),Y
   LDY   #$13
   STA   (FP),Y
   LDY   #$39
   LDA   (FP),Y
   LDY   #$14
   STA   (FP),Y
   LDY   #$11
   LDA   (FP),Y
   CLC
   LDY   #$13
   ADC   (FP),Y
   LDY   #$15
   STA   (FP),Y
   LDY   #$12
   LDA   (FP),Y
   LDY   #$14
   ADC   (FP),Y
   LDY   #$16
   STA   (FP),Y
   LDY   #$41
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$15
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$16
   LDA   (FP),Y
   LDY   #$01
   STA   (DPTR),Y
   JSR   f4
   LDY   #$17
   STA   (FP),Y
   ORA   #$00
   BEQ   .if_else@3
   LDA   #$03
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   ORA   #$00
   BPL   .sx_done@25
   LDA   #$FF
.sx_done@25:
   LDY   #$1A
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$0D
   LDA   (FP),Y
   CLC
   LDY   #$18
   ADC   (FP),Y
   LDY   #$1C
   STA   (FP),Y
   LDY   #$0E
   LDA   (FP),Y
   LDY   #$19
   ADC   (FP),Y
   LDY   #$1D
   STA   (FP),Y
   LDY   #$0F
   LDA   (FP),Y
   LDY   #$1A
   ADC   (FP),Y
   LDY   #$1E
   STA   (FP),Y
   LDY   #$10
   LDA   (FP),Y
   LDY   #$1B
   ADC   (FP),Y
   LDY   #$1F
   STA   (FP),Y
   LDY   #$1C
   LDA   (FP),Y
   LDY   #$0D
   STA   (FP),Y
   LDY   #$1D
   LDA   (FP),Y
   LDY   #$0E
   STA   (FP),Y
   LDY   #$1E
   LDA   (FP),Y
   LDY   #$0F
   STA   (FP),Y
   LDY   #$1F
   LDA   (FP),Y
   LDY   #$10
   STA   (FP),Y
   JMP   .if_end@2
.if_else@3:
   LDA   #$05
   LDY   #$20
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   ORA   #$00
   BPL   .sx_done@27
   LDA   #$FF
.sx_done@27:
   LDY   #$22
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$0D
   LDA   (FP),Y
   LDY   #$20
   EOR   (FP),Y
   LDY   #$24
   STA   (FP),Y
   LDY   #$0E
   LDA   (FP),Y
   LDY   #$21
   EOR   (FP),Y
   LDY   #$25
   STA   (FP),Y
   LDY   #$0F
   LDA   (FP),Y
   LDY   #$22
   EOR   (FP),Y
   LDY   #$26
   STA   (FP),Y
   LDY   #$10
   LDA   (FP),Y
   LDY   #$23
   EOR   (FP),Y
   LDY   #$27
   STA   (FP),Y
   LDY   #$24
   LDA   (FP),Y
   LDY   #$0D
   STA   (FP),Y
   LDY   #$25
   LDA   (FP),Y
   LDY   #$0E
   STA   (FP),Y
   LDY   #$26
   LDA   (FP),Y
   LDY   #$0F
   STA   (FP),Y
   LDY   #$27
   LDA   (FP),Y
   LDY   #$10
   STA   (FP),Y
.if_end@2:
   LDY   #$41
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$28
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$29
   STA   (FP),Y
   LDA   #$E8
   SEC
   DEY
   SBC   (FP),Y
   LDA   #$03
   INY
   SBC   (FP),Y
   BVC   .cmp_novf@28
   EOR   #$80
.cmp_novf@28:
   ASL
   LDA   #$00
   ROL
   EOR   #$01
   LDY   #$2A
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@4
   LDY   #$33
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   INY
   ORA   (FP),Y
   INY
   ORA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@6
   LDY   #$3E
   LDA   (FP),Y
   BEQ   .and_false@6
   LDA   #$01
   LDY   #$2C
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   JMP   .and_end@7
.and_false@6:
   LDA   #$00
   LDY   #$2C
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@7:
   LDY   #$2C
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
   LDY   #$2E
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@5:
   LDY   #$2E
   LDA   (FP),Y
   LDY   #$30
   STA   (FP),Y
   CMP   #$01

   ; epilogue
   PHP
   CLC
   LDA   FP
   ADC   #$42
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

   ; prologue: 12 arg bytes, 65 local bytes
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

   LDY   #$4A
   LDA   (FP),Y
   AND   g0
   LDY   #$01
   STA   (FP),Y
   LDY   #$4B
   LDA   (FP),Y
   AND   g0+1
   LDY   #$02
   STA   (FP),Y
   LDY   #$4C
   LDA   (FP),Y
   AND   g0+2
   LDY   #$03
   STA   (FP),Y
   LDY   #$4D
   LDA   (FP),Y
   AND   g0+3
   LDY   #$04
   STA   (FP),Y
   LDY   #$4E
   LDA   (FP),Y
   AND   g0+4
   LDY   #$05
   STA   (FP),Y
   LDY   #$4F
   LDA   (FP),Y
   AND   g0+5
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
   LDY   #$07
   LDA   (FP),Y
   LDY   #$0C
   STA   (FP),Y
   LDY   #$08
   LDA   (FP),Y
   LDY   #$0D
   STA   (FP),Y
   LDY   #$09
   LDA   (FP),Y
   LDY   #$0E
   STA   (FP),Y
   LDY   #$0A
   LDA   (FP),Y
   LDY   #$0F
   STA   (FP),Y
   LDY   #$0B
   LDA   (FP),Y
   AND   #$01
   LDY   #$10
   STA   (FP),Y
   LDY   #$0C
   LDA   (FP),Y
   LDY   #$11
   STA   (FP),Y
   LDY   #$0D
   LDA   (FP),Y
   LDY   #$12
   STA   (FP),Y
   LDY   #$0E
   LDA   (FP),Y
   LDY   #$13
   STA   (FP),Y
   LDY   #$0F
   LDA   (FP),Y
   LDY   #$14
   STA   (FP),Y
   LDY   #$10
   LDA   (FP),Y
   EOR   #$01
   LDY   #$15
   STA   (FP),Y
   LDY   #$11
   LDA   (FP),Y
   LDY   #$16
   STA   (FP),Y
   LDY   #$12
   LDA   (FP),Y
   LDY   #$17
   STA   (FP),Y
   LDY   #$13
   LDA   (FP),Y
   LDY   #$18
   STA   (FP),Y
   LDY   #$14
   LDA   (FP),Y
   LDY   #$19
   STA   (FP),Y
   LDY   #$15
   LDA   (FP),Y
   SEC
   SBC   #$01
   LDY   #$1A
   STA   (FP),Y
   LDY   #$16
   LDA   (FP),Y
   LDY   #$1B
   STA   (FP),Y
   LDY   #$17
   LDA   (FP),Y
   LDY   #$1C
   STA   (FP),Y
   LDY   #$18
   LDA   (FP),Y
   LDY   #$1D
   STA   (FP),Y
   LDY   #$19
   LDA   (FP),Y
   LDY   #$1E
   STA   (FP),Y
   LDY   #$1A
   LDA   (FP),Y
   LDY   #$1F
   STA   (FP),Y
   LDA   #$E8
   INY
   STA   (FP),Y
   LDA   #$03
   INY
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@29
   LDA   #$00
   JMP   .sx_done@30
.sx_neg@29:
   LDA   #$FF
.sx_done@30:
   LDY   #$22
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$20
   LDA   (FP),Y
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
   INY
   STA   (FP),Y
   LDY   #$49
   LDA   (FP),Y
   CMP   (FP),Y
   BNE   .thread@1
   DEY
   LDA   (FP),Y
   CMP   (FP),Y
   BNE   .thread@1
   DEY
   LDA   (FP),Y
   CMP   (FP),Y
   BNE   .thread@1
   DEY
   LDA   (FP),Y
   CMP   (FP),Y
   BEQ   .cmp_true@31
.thread@1:
   LDA   #$00
   JMP   .cmp_end@32
.cmp_true@31:
   LDA   #$01
.cmp_end@32:
   LDY   #$28
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   SEC
   DEY
   SBC   (FP),Y
   LDY   #$2A
   STA   (FP),Y
   LDA   #$00
   DEY
   SBC   (FP),Y
   LDY   #$2B
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$2C
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$2D
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@34
   LDA   #$00
   JMP   .sx_done@35
.sx_neg@34:
   LDA   #$FF
.sx_done@35:
   LDY   #$2E
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$2C
   LDA   (FP),Y
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
   INY
   STA   (FP),Y
   LDY   #$44
   LDA   (FP),Y
   LDY   #$34
   STA   (FP),Y
   LDY   #$45
   LDA   (FP),Y
   LDY   #$35
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@36
   LDA   #$00
   JMP   .sx_done@37
.sx_neg@36:
   LDA   #$FF
.sx_done@37:
   LDY   #$36
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$34
   LDA   (FP),Y
   ORA   g0
   LDY   #$3A
   STA   (FP),Y
   LDY   #$35
   LDA   (FP),Y
   ORA   g0+1
   LDY   #$3B
   STA   (FP),Y
   LDY   #$36
   LDA   (FP),Y
   ORA   g0+2
   LDY   #$3C
   STA   (FP),Y
   LDY   #$37
   LDA   (FP),Y
   ORA   g0+3
   LDY   #$3D
   STA   (FP),Y
   LDY   #$38
   LDA   (FP),Y
   ORA   g0+4
   LDY   #$3E
   STA   (FP),Y
   LDY   #$39
   LDA   (FP),Y
   ORA   g0+5
   LDY   #$3F
   STA   (FP),Y
   LDY   #$3A
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
   BEQ   .lnot_true@38
   LDA   #$00
   JMP   .lnot_end@39
.lnot_true@38:
   LDA   #$01
.lnot_end@39:
   LDY   #$40
   STA   (FP),Y
   LDA   #$00
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
   ADC   #$4F
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$42
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

   ; prologue: 0 arg bytes, 32 local bytes
   SEC
   LDA   SSP
   SBC   #$22
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$21
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   LDA   #$A8
   LDY   #$01
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   ORA   #$00
   BPL   .sx_done@41
   LDA   #$FF
.sx_done@41:
   LDY   #$03
   STA   (FP),Y
   INY
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$0C
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   #$12
   LDY   #$01
   STA   (SSP),Y
   LDA   #$00
   INY
   STA   (SSP),Y
   DEY
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   DEY
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   DEY
   LDA   (FP),Y
   LDY   #$05
   STA   (SSP),Y
   DEY
   LDA   (FP),Y
   LDY   #$06
   STA   (SSP),Y
   LDA   #$69
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
   JSR   f0
   LDY   #$05
   STA   (FP),Y
   TXA
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$07
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$08
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@42
   LDA   #$00
   JMP   .sx_done@43
.sx_neg@42:
   LDA   #$FF
.sx_done@43:
   LDY   #$09
   STA   (FP),Y
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
   LDY   #$0E
   LDA   (FP),Y
   LDY   #$12
   EOR   (FP),Y
   LDY   #$16
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
   LDY   #$16
   LDA   (FP),Y
   LDY   #$0E
   STA   (FP),Y
   LDY   #$0B
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
   LDY   #$17
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
   LDY   #$0E
   LDA   (FP),Y
   LDY   #$1A
   EOR   (FP),Y
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
   TAX
   DEY
   LDA   (FP),Y

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$22
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$21
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
   DC.B  $5E
   DS.B  5
