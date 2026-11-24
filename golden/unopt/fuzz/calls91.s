SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24

f2:
   SUBROUTINE

   ; prologue: 11 arg bytes, 70 local bytes
   SEC
   LDA   SSP
   SBC   #$48
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$47
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   LDA   #$00
   SEC
   SBC   g1
   LDA   #$00
   SBC   g1+1
   BVC   .cmp_novf@0
   EOR   #$80
.cmp_novf@0:
   ASL
   LDA   #$00
   ROL
   EOR   #$01
   LDY   #$01
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@0
   LDA   g2
   AND   #$07
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   g1
   INY
   STA   (FP),Y
   LDA   g1+1
   INY
   STA   (FP),Y
   LDY   #$03
   LDA   (FP),Y
   TAX
   BEQ   .shift_done@2
.shift_loop@1:
   LDY   #$07
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
   BNE   .shift_loop@1
.shift_done@2:
   LDY   #$06
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
   LDY   #$08
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@1:
   LDY   #$08
   LDA   (FP),Y
   LDY   #$0A
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$0B
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@3
   LDA   #$00
   JMP   .sx_done@4
.sx_neg@3:
   LDA   #$FF
.sx_done@4:
   LDY   #$0C
   STA   (FP),Y
   INY
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
   INY
   STA   (FP),Y
   LDA   g1
   INY
   STA   (FP),Y
   LDA   g1+1
   INY
   STA   (FP),Y
   LDA   g1+1
   BMI   .sx_neg@5
   LDA   #$00
   JMP   .sx_done@6
.sx_neg@5:
   LDA   #$FF
.sx_done@6:
   LDY   #$14
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$50
   LDA   (FP),Y
   LDY   #$12
   ORA   (FP),Y
   LDY   #$16
   STA   (FP),Y
   LDY   #$51
   LDA   (FP),Y
   LDY   #$13
   ORA   (FP),Y
   LDY   #$17
   STA   (FP),Y
   LDY   #$52
   LDA   (FP),Y
   LDY   #$14
   ORA   (FP),Y
   LDY   #$18
   STA   (FP),Y
   LDY   #$53
   LDA   (FP),Y
   LDY   #$15
   ORA   (FP),Y
   LDY   #$19
   STA   (FP),Y
   LDA   g1
   ORA   g1+1
   BEQ   .and_false@2
   LDA   g2
   ORA   g2+1
   ORA   g2+2
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
   LDY   #$1A
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@3:
   LDY   #$1A
   LDA   (FP),Y
   AND   #$07
   LDY   #$1C
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDA   #$00
   LDY   #$1D
   STA   (FP),Y
   LDY   #$16
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
   LDY   #$1C
   LDA   (FP),Y
   STA   HARGS+4
   JSR   lsr32
   LDA   HARGS+5
   LDY   #$1E
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
   LDY   #$1E
   LDA   (FP),Y
   LDY   #$22
   STA   (FP),Y
   LDY   #$1F
   LDA   (FP),Y
   LDY   #$23
   STA   (FP),Y
   LDY   #$20
   LDA   (FP),Y
   LDY   #$24
   STA   (FP),Y
   LDY   #$21
   LDA   (FP),Y
   LDY   #$25
   STA   (FP),Y
   LDA   #$FF
   INY
   STA   (FP),Y
   LDA   #$7F
   INY
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@7
   LDA   #$00
   JMP   .sx_done@8
.sx_neg@7:
   LDA   #$FF
.sx_done@8:
   LDY   #$28
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$49
   LDA   (FP),Y
   LDY   #$26
   AND   (FP),Y
   LDY   #$2A
   STA   (FP),Y
   LDY   #$4A
   LDA   (FP),Y
   LDY   #$27
   AND   (FP),Y
   LDY   #$2B
   STA   (FP),Y
   LDY   #$4B
   LDA   (FP),Y
   LDY   #$28
   AND   (FP),Y
   LDY   #$2C
   STA   (FP),Y
   LDY   #$4C
   LDA   (FP),Y
   LDY   #$29
   AND   (FP),Y
   LDY   #$2D
   STA   (FP),Y
   LDY   #$2A
   LDA   (FP),Y
   LDY   #$2E
   STA   (FP),Y
   LDY   #$2B
   LDA   (FP),Y
   LDY   #$2F
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$30
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$31
   STA   (FP),Y
   LDA   g1
   INY
   STA   (FP),Y
   LDA   g1+1
   INY
   STA   (FP),Y
   LDA   g1+1
   BMI   .sx_neg@9
   LDA   #$00
   JMP   .sx_done@10
.sx_neg@9:
   LDA   #$FF
.sx_done@10:
   LDY   #$34
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$32
   LDA   (FP),Y
   LDY   #$0E
   EOR   (FP),Y
   LDY   #$36
   STA   (FP),Y
   LDY   #$33
   LDA   (FP),Y
   LDY   #$0F
   EOR   (FP),Y
   LDY   #$37
   STA   (FP),Y
   LDY   #$34
   LDA   (FP),Y
   LDY   #$10
   EOR   (FP),Y
   LDY   #$38
   STA   (FP),Y
   LDY   #$35
   LDA   (FP),Y
   LDY   #$11
   EOR   (FP),Y
   LDY   #$39
   STA   (FP),Y
   LDY   #$36
   LDA   (FP),Y
   LDY   #$3A
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$0E
   LDA   (FP),Y
   LDY   #$3C
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$30
   LDA   (FP),Y
   AND   #$07
   LDY   #$3E
   STA   (FP),Y
   LDY   #$31
   LDA   (FP),Y
   LDA   #$00
   LDY   #$3F
   STA   (FP),Y
   LDY   #$3B
   LDA   (FP),Y
   LDY   #$40
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@11
   LDA   #$00
   JMP   .sx_done@12
.sx_neg@11:
   LDA   #$FF
.sx_done@12:
   LDY   #$41
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$42
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$43
   STA   (FP),Y
   LDY   #$3E
   LDA   (FP),Y
   SEC
   LDY   #$42
   SBC   (FP),Y
   LDY   #$3F
   LDA   (FP),Y
   LDY   #$43
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
   LDY   #$46
   STA   (FP),Y

   ; epilogue
   PHA
   CLC
   LDA   FP
   ADC   #$53
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

f1:
   SUBROUTINE

   ; prologue: 7 arg bytes, 93 local bytes
   SEC
   LDA   SSP
   SBC   #$5F
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$5E
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   LDA   g1
   LDY   #$01
   STA   (FP),Y
   LDA   g1+1
   INY
   STA   (FP),Y
   LDY   #$62
   LDA   (FP),Y
   LDY   #$01
   AND   (FP),Y
   LDY   #$03
   STA   (FP),Y
   LDY   #$63
   LDA   (FP),Y
   LDY   #$02
   AND   (FP),Y
   LDY   #$04
   STA   (FP),Y
   LDA   g0
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$08
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$09
   STA   (FP),Y
   LDY   #$03
   LDA   (FP),Y
   LDY   #$08
   AND   (FP),Y
   LDY   #$0A
   STA   (FP),Y
   LDY   #$04
   LDA   (FP),Y
   LDY   #$09
   AND   (FP),Y
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
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$0C
   LDA   (FP),Y
   LDY   #$0F
   STA   (FP),Y
   LDY   #$0D
   LDA   (FP),Y
   LDY   #$10
   STA   (FP),Y
   LDY   #$0E
   LDA   (FP),Y
   LDY   #$11
   STA   (FP),Y
   LDA   #$07
   LDY   #$60
   EOR   (FP),Y
   LDY   #$12
   STA   (FP),Y
   LDA   #$00
   LDY   #$61
   EOR   (FP),Y
   LDY   #$13
   STA   (FP),Y
   SEC
   LDA   #$00
   LDY   #$62
   SBC   (FP),Y
   LDY   #$14
   STA   (FP),Y
   LDA   #$00
   LDY   #$63
   SBC   (FP),Y
   LDY   #$15
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   LDY   #$12
   SBC   (FP),Y
   LDY   #$15
   LDA   (FP),Y
   LDY   #$13
   SBC   (FP),Y
   LDA   #$00
   ROL
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
   DEY
   LDA   (FP),Y
   LDY   #$1A
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$1B
   STA   (FP),Y
   LDA   #$01
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$60
   LDA   (FP),Y
   CLC
   LDY   #$62
   SBC   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$63
   SBC   (FP),Y
   LDA   #$00
   ROL
   LDY   #$1E
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   SEC
   DEY
   SBC   (FP),Y
   LDY   #$20
   STA   (FP),Y
   LDA   #$00
   DEY
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
   BMI   .sx_neg@13
   LDA   #$00
   JMP   .sx_done@14
.sx_neg@13:
   LDA   #$FF
.sx_done@14:
   LDY   #$24
   STA   (FP),Y
   INY
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
   INY
   STA   (FP),Y
   LDA   #$03
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   ORA   #$00
   BPL   .sx_done@16
   LDA   #$FF
.sx_done@16:
   LDY   #$2C
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$62
   LDA   (FP),Y
   LDY   #$2E
   STA   (FP),Y
   LDY   #$63
   LDA   (FP),Y
   LDY   #$2F
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$64
   LDA   (FP),Y
   CLC
   LDY   #$2E
   ADC   (FP),Y
   LDY   #$31
   STA   (FP),Y
   LDY   #$65
   LDA   (FP),Y
   LDY   #$2F
   ADC   (FP),Y
   LDY   #$32
   STA   (FP),Y
   LDY   #$66
   LDA   (FP),Y
   LDY   #$30
   ADC   (FP),Y
   LDY   #$33
   STA   (FP),Y
   LDY   #$31
   LDA   (FP),Y
   LDY   #$34
   STA   (FP),Y
   LDY   #$32
   LDA   (FP),Y
   LDY   #$35
   STA   (FP),Y
   LDY   #$33
   LDA   (FP),Y
   AND   #$1F
   LDY   #$36
   STA   (FP),Y
   LDY   #$62
   LDA   (FP),Y
   LDY   #$37
   STA   (FP),Y
   LDY   #$63
   LDA   (FP),Y
   LDY   #$38
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$37
   LDA   (FP),Y
   CLC
   SBC   g2
   INY
   LDA   (FP),Y
   SBC   g2+1
   INY
   LDA   (FP),Y
   SBC   g2+2
   LDA   #$00
   ROL
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$3C
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$3D
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@17
   LDA   #$00
   JMP   .sx_done@18
.sx_neg@17:
   LDA   #$FF
.sx_done@18:
   LDY   #$3E
   STA   (FP),Y
   INY
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$0B
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDY   #$2A
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$2B
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$2C
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$2D
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   LDY   #$34
   LDA   (FP),Y
   LDY   #$05
   STA   (SSP),Y
   LDY   #$35
   LDA   (FP),Y
   LDY   #$06
   STA   (SSP),Y
   LDY   #$36
   LDA   (FP),Y
   LDY   #$07
   STA   (SSP),Y
   LDY   #$3C
   LDA   (FP),Y
   LDY   #$08
   STA   (SSP),Y
   LDY   #$3D
   LDA   (FP),Y
   LDY   #$09
   STA   (SSP),Y
   LDY   #$3E
   LDA   (FP),Y
   LDY   #$0A
   STA   (SSP),Y
   LDY   #$3F
   LDA   (FP),Y
   LDY   #$0B
   STA   (SSP),Y
   JSR   f2
   LDY   #$40
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$1D
   LDA   (FP),Y
   LDY   #$43
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@19
   LDA   #$00
   JMP   .sx_done@20
.sx_neg@19:
   LDA   #$FF
.sx_done@20:
   LDY   #$44
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$41
   EOR   (FP),Y
   LDY   #$45
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$42
   EOR   (FP),Y
   LDY   #$46
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$47
   STA   (FP),Y
   LDY   #$1D
   STA   (FP),Y
   LDY   #$48
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@21
   LDA   #$00
   JMP   .sx_done@22
.sx_neg@21:
   LDA   #$FF
.sx_done@22:
   LDY   #$49
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$48
   LDA   (FP),Y
   LDY   #$4B
   STA   (FP),Y
   LDY   #$49
   LDA   (FP),Y
   LDY   #$4C
   STA   (FP),Y
   AND   #$07
   INY
   STA   (FP),Y
   LDA   g2
   CLC
   LDY   #$4B
   ADC   (FP),Y
   LDY   #$4E
   STA   (FP),Y
   LDA   g2+1
   LDY   #$4C
   ADC   (FP),Y
   LDY   #$4F
   STA   (FP),Y
   LDA   g2+2
   LDY   #$4D
   ADC   (FP),Y
   LDY   #$50
   STA   (FP),Y
   LDY   #$4E
   LDA   (FP),Y
   LDY   #$51
   STA   (FP),Y
   LDY   #$4F
   LDA   (FP),Y
   LDY   #$52
   STA   (FP),Y
   LDY   #$50
   LDA   (FP),Y
   AND   #$07
   LDY   #$53
   STA   (FP),Y
   LDY   #$51
   LDA   (FP),Y
   STA   g2
   INY
   LDA   (FP),Y
   STA   g2+1
   INY
   LDA   (FP),Y
   STA   g2+2
   LDA   #$07
   BEQ   .and_false@4
   LDA   #$03
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
   LDY   #$54
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@5:
   LDY   #$66
   LDA   (FP),Y
   CMP   (FP),Y
   BNE   .thread@0
   DEY
   LDA   (FP),Y
   CMP   (FP),Y
   BNE   .thread@0
   DEY
   LDA   (FP),Y
   CMP   (FP),Y
   BEQ   .cmp_true@23
.thread@0:
   LDA   #$00
   JMP   .cmp_end@24
.cmp_true@23:
   LDA   #$01
.cmp_end@24:
   LDY   #$56
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$55
   LDA   (FP),Y
   LDY   #$57
   CMP   (FP),Y
   BNE   .thread@1
   LDY   #$54
   LDA   (FP),Y
   LDY   #$56
   CMP   (FP),Y
   BEQ   .cmp_true@26
.thread@1:
   LDA   #$00
   JMP   .cmp_end@27
.cmp_true@26:
   LDA   #$01
.cmp_end@27:
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
   LDY   #$5A
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
   RTS

f0:
   SUBROUTINE

   ; prologue: 2 arg bytes, 60 local bytes
   SEC
   LDA   SSP
   SBC   #$3E
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$3D
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   LDA   #$03
   LDY   #$01
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDA   g0
   STA   HARGS
   LDA   g0+1
   STA   HARGS+1
   DEY
   LDA   (FP),Y
   STA   HARGS+2
   INY
   LDA   (FP),Y
   STA   HARGS+3
   JSR   sdivmod16
   LDA   HARGS+6
   LDY   #$03
   STA   (FP),Y
   LDA   HARGS+7
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$05
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   g0
   INY
   STA   (FP),Y
   LDA   g0+1
   INY
   STA   (FP),Y
   LDA   g0+1
   BMI   .sx_neg@31
   LDA   #$00
   JMP   .sx_done@32
.sx_neg@31:
   LDA   #$FF
.sx_done@32:
   LDY   #$09
   STA   (FP),Y
   LDY   #$07
   LDA   (FP),Y
   LDY   #$0A
   STA   (FP),Y
   LDY   #$08
   LDA   (FP),Y
   LDY   #$0B
   STA   (FP),Y
   LDY   #$09
   LDA   (FP),Y
   AND   #$1F
   LDY   #$0C
   STA   (FP),Y
   LDY   #$3F
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@6
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@6
   LDA   #$01
   LDY   #$0D
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   JMP   .and_end@7
.and_false@6:
   LDA   #$00
   LDY   #$0D
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@7:
   LDY   #$0D
   LDA   (FP),Y
   LDY   #$0F
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$10
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@33
   LDA   #$00
   JMP   .sx_done@34
.sx_neg@33:
   LDA   #$FF
.sx_done@34:
   LDY   #$11
   STA   (FP),Y
   LDY   #$0F
   LDA   (FP),Y
   LDY   #$12
   STA   (FP),Y
   LDY   #$10
   LDA   (FP),Y
   LDY   #$13
   STA   (FP),Y
   LDY   #$11
   LDA   (FP),Y
   AND   #$1F
   LDY   #$14
   STA   (FP),Y
   LDY   #$0C
   LDA   (FP),Y
   LDY   #$14
   CMP   (FP),Y
   BNE   .thread@2
   LDY   #$0B
   LDA   (FP),Y
   LDY   #$13
   CMP   (FP),Y
   BNE   .thread@2
   LDY   #$0A
   LDA   (FP),Y
   LDY   #$12
   CMP   (FP),Y
   BEQ   .cmp_true@35
.thread@2:
   LDA   #$00
   JMP   .cmp_end@36
.cmp_true@35:
   LDA   #$01
.cmp_end@36:
   LDY   #$15
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$17
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$06
   LDA   (FP),Y
   LDY   #$19
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@38
   LDA   #$00
   JMP   .sx_done@39
.sx_neg@38:
   LDA   #$FF
.sx_done@39:
   LDY   #$1A
   STA   (FP),Y
   LDA   #$7F
   CMP   g1+1
   BNE   .cmp_true@40
   LDA   #$FF
   CMP   g1
   BNE   .cmp_true@40
   LDA   #$00
   JMP   .cmp_end@41
.cmp_true@40:
   LDA   #$01
.cmp_end@41:
   LDY   #$1B
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$19
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
   JSR   mul16
   LDA   HARGS+4
   LDY   #$1D
   STA   (FP),Y
   LDA   HARGS+5
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$1F
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   CMP   g2+2
   BNE   .vthread@0
   CMP   g2+1
   BNE   .vthread@0
   LDA   #$01
   CMP   g2
   BEQ   .cmp_true@43
   LDA   #$00
.vthread@0:
.cmp_true@43:
   LDY   #$21
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDA   g1+1
   CMP   (FP),Y
   BNE   .thread@4
   LDA   g1
   DEY
   CMP   (FP),Y
   BEQ   .cmp_true@46
.thread@4:
   LDA   #$00
   JMP   .cmp_end@47
.cmp_true@46:
   LDA   #$01
.cmp_end@47:
   LDY   #$23
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$25
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$06
   LDA   (FP),Y
   LDY   #$27
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$26
   LDA   (FP),Y
   LDY   #$29
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$26
   LDA   (FP),Y
   LDY   #$2B
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ORA   #$01
   LDY   #$2D
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$2E
   STA   (FP),Y
   LDY   #$29
   LDA   (FP),Y
   STA   HARGS
   INY
   LDA   (FP),Y
   STA   HARGS+1
   LDY   #$2D
   LDA   (FP),Y
   STA   HARGS+2
   INY
   LDA   (FP),Y
   STA   HARGS+3
   JSR   sdivmod16
   LDA   HARGS+4
   LDY   #$2F
   STA   (FP),Y
   LDA   HARGS+5
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@8
   LDY   #$28
   LDA   (FP),Y
   LDY   #$31
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$06
   LDA   (FP),Y
   LDY   #$33
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@49
   LDA   #$00
   JMP   .sx_done@50
.sx_neg@49:
   LDA   #$FF
.sx_done@50:
   LDY   #$34
   STA   (FP),Y
   LDY   #$31
   LDA   (FP),Y
   LDY   #$33
   EOR   (FP),Y
   LDY   #$35
   STA   (FP),Y
   LDY   #$32
   LDA   (FP),Y
   LDY   #$34
   EOR   (FP),Y
   LDY   #$36
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@8
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   JMP   .or_end@9
.or_true@8:
   LDA   #$01
   LDY   #$37
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@9:
   LDY   #$37
   LDA   (FP),Y
   LDY   #$39
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$3A
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@51
   LDA   #$00
   JMP   .sx_done@52
.sx_neg@51:
   LDA   #$FF
.sx_done@52:
   LDY   #$3B
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$39
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
   ADC   #$40
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

   ; prologue: 0 arg bytes, 42 local bytes
   SEC
   LDA   SSP
   SBC   #$2C
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$2B
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
   SBC   #$02
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   #$25
   LDY   #$01
   STA   (SSP),Y
   LDA   #$01
   INY
   STA   (SSP),Y
   JSR   f0
   LDA   HARGS+8
   LDY   #$01
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
   LDY   #$04
   LDA   (FP),Y
   LDY   #$08
   STA   (FP),Y
   LDA   g0
   INY
   STA   (FP),Y
   LDA   g0+1
   INY
   STA   (FP),Y
   LDA   g0+1
   BMI   .sx_neg@53
   LDA   #$00
   JMP   .sx_done@54
.sx_neg@53:
   LDA   #$FF
.sx_done@54:
   LDY   #$0B
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   g1
   INY
   STA   (FP),Y
   LDA   g1+1
   INY
   STA   (FP),Y
   LDA   g1+1
   BMI   .sx_neg@55
   LDA   #$00
   JMP   .sx_done@56
.sx_neg@55:
   LDA   #$FF
.sx_done@56:
   LDY   #$0F
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$09
   LDA   (FP),Y
   LDY   #$0D
   EOR   (FP),Y
   LDY   #$11
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
   LDA   g2
   INY
   STA   (FP),Y
   LDA   g2+1
   INY
   STA   (FP),Y
   LDA   g2+2
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$11
   LDA   (FP),Y
   LDY   #$15
   EOR   (FP),Y
   LDY   #$19
   STA   (FP),Y
   LDY   #$12
   LDA   (FP),Y
   LDY   #$16
   EOR   (FP),Y
   LDY   #$1A
   STA   (FP),Y
   LDY   #$13
   LDA   (FP),Y
   LDY   #$17
   EOR   (FP),Y
   LDY   #$1B
   STA   (FP),Y
   LDY   #$14
   LDA   (FP),Y
   LDY   #$18
   EOR   (FP),Y
   LDY   #$1C
   STA   (FP),Y
   LDY   #$05
   LDA   (FP),Y
   LDY   #$19
   EOR   (FP),Y
   LDY   #$1D
   STA   (FP),Y
   LDY   #$06
   LDA   (FP),Y
   LDY   #$1A
   EOR   (FP),Y
   LDY   #$1E
   STA   (FP),Y
   LDY   #$07
   LDA   (FP),Y
   LDY   #$1B
   EOR   (FP),Y
   LDY   #$1F
   STA   (FP),Y
   LDY   #$08
   LDA   (FP),Y
   LDY   #$1C
   EOR   (FP),Y
   LDY   #$20
   STA   (FP),Y
   LDY   #$1D
   LDA   (FP),Y
   LDY   #$05
   STA   (FP),Y
   LDY   #$1E
   LDA   (FP),Y
   LDY   #$06
   STA   (FP),Y
   LDY   #$1F
   LDA   (FP),Y
   LDY   #$07
   STA   (FP),Y
   LDY   #$20
   LDA   (FP),Y
   LDY   #$08
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
   LDA   #$10
   STA   HARGS+4
   JSR   asr32
   LDA   HARGS+5
   LDY   #$21
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
   LDY   #$05
   LDA   (FP),Y
   LDY   #$21
   EOR   (FP),Y
   LDY   #$25
   STA   (FP),Y
   LDY   #$06
   LDA   (FP),Y
   LDY   #$22
   EOR   (FP),Y
   LDY   #$26
   STA   (FP),Y
   LDY   #$07
   LDA   (FP),Y
   LDY   #$23
   EOR   (FP),Y
   LDY   #$27
   STA   (FP),Y
   LDY   #$08
   LDA   (FP),Y
   LDY   #$24
   EOR   (FP),Y
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
   TAX
   DEY
   LDA   (FP),Y

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$2C
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$2B
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
   DC.W  $00AF

g1:
   DC.W  $0066

g2:
   DC.B  $C8
   DS.B  2
