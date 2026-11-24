SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24

rec:
   SUBROUTINE

   ; prologue: 4 arg bytes, 16 local bytes
   SEC
   LDA   SSP
   SBC   #$12
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$11
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
   INY
   SBC   (FP),Y
   LDA   #$00
   INY
   SBC   (FP),Y
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
   BEQ   .if_end@0
   LDY   #$15
   LDA   (FP),Y
   LDY   #$03
   STA   (FP),Y
   LDY   #$16
   LDA   (FP),Y
   LDA   #$00
   LDY   #$04
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
   ADC   #$16
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$11
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
   LDY   #$13
   LDA   (FP),Y
   SEC
   SBC   #$01
   LDY   #$05
   STA   (FP),Y
   LDY   #$14
   LDA   (FP),Y
   SBC   #$00
   LDY   #$06
   STA   (FP),Y
   LDY   #$15
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
   LDY   #$07
   STA   (FP),Y
   LDA   HARGS+5
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   LDY   #$13
   ADC   (FP),Y
   LDY   #$09
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$14
   ADC   (FP),Y
   LDY   #$0A
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$0B
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   AND   #$03
   LDY   #$0C
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$04
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDY   #$05
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$06
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$0B
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$0C
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   JSR   rec
   LDY   #$0D
   STA   (FP),Y
   TXA
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$01
   LDY   #$0F
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$10
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
   ADC   #$16
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$11
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

   ; prologue: 6 arg bytes, 94 local bytes
   SEC
   LDA   SSP
   SBC   #$60
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$5F
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   LDY   #$63
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   LDY   #$64
   LDA   (FP),Y
   LDY   #$02
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$61
   LDA   (FP),Y
   LDY   #$05
   STA   (FP),Y
   LDY   #$62
   LDA   (FP),Y
   LDY   #$06
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@1
   LDA   #$00
   JMP   .sx_done@2
.sx_neg@1:
   LDA   #$FF
.sx_done@2:
   LDY   #$07
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$01
   LDA   (FP),Y
   SEC
   LDY   #$05
   SBC   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   LDY   #$06
   SBC   (FP),Y
   LDY   #$03
   LDA   (FP),Y
   LDY   #$07
   SBC   (FP),Y
   LDY   #$04
   LDA   (FP),Y
   LDY   #$08
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
   LDY   #$61
   LDA   (FP),Y
   ORA   #$01
   LDY   #$0F
   STA   (FP),Y
   LDY   #$62
   LDA   (FP),Y
   LDY   #$10
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$11
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$12
   STA   (FP),Y
   LDY   #$0D
   LDA   (FP),Y
   STA   HARGS
   INY
   LDA   (FP),Y
   STA   HARGS+1
   LDY   #$11
   LDA   (FP),Y
   STA   HARGS+2
   INY
   LDA   (FP),Y
   STA   HARGS+3
   JSR   udivmod16
   LDA   HARGS+4
   LDY   #$13
   STA   (FP),Y
   LDA   HARGS+5
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ORA   #$01
   LDY   #$15
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$16
   STA   (FP),Y
   LDA   #$FF
   STA   HARGS
   LDA   #$7F
   STA   HARGS+1
   DEY
   LDA   (FP),Y
   STA   HARGS+2
   INY
   LDA   (FP),Y
   STA   HARGS+3
   JSR   udivmod16
   LDA   HARGS+4
   LDY   #$17
   STA   (FP),Y
   LDA   HARGS+5
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
   DEY
   LDA   (FP),Y
   LDY   #$1B
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$1C
   STA   (FP),Y
   LDY   #$61
   LDA   (FP),Y
   LDY   #$1D
   STA   (FP),Y
   LDY   #$62
   LDA   (FP),Y
   LDY   #$1E
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@3
   LDA   #$00
   JMP   .sx_done@4
.sx_neg@3:
   LDA   #$FF
.sx_done@4:
   LDY   #$1F
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$1D
   LDA   (FP),Y
   ORA   g0
   LDY   #$22
   STA   (FP),Y
   LDY   #$1E
   LDA   (FP),Y
   ORA   g0+1
   LDY   #$23
   STA   (FP),Y
   LDY   #$1F
   LDA   (FP),Y
   ORA   g0+2
   LDY   #$24
   STA   (FP),Y
   LDY   #$20
   LDA   (FP),Y
   ORA   g0+3
   LDY   #$25
   STA   (FP),Y
   LDY   #$21
   LDA   (FP),Y
   ORA   g0+4
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
   LDY   #$27
   LDA   (FP),Y
   LDY   #$2B
   STA   (FP),Y
   LDY   #$28
   LDA   (FP),Y
   LDY   #$2C
   STA   (FP),Y
   LDY   #$29
   LDA   (FP),Y
   LDY   #$2D
   STA   (FP),Y
   LDY   #$2A
   LDA   (FP),Y
   LDY   #$2E
   STA   (FP),Y
   LDY   #$61
   LDA   (FP),Y
   LDY   #$2F
   STA   (FP),Y
   LDY   #$62
   LDA   (FP),Y
   LDY   #$30
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@5
   LDA   #$00
   JMP   .sx_done@6
.sx_neg@5:
   LDA   #$FF
.sx_done@6:
   LDY   #$31
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$2B
   LDA   (FP),Y
   LDY   #$2F
   ORA   (FP),Y
   LDY   #$33
   STA   (FP),Y
   LDY   #$2C
   LDA   (FP),Y
   LDY   #$30
   ORA   (FP),Y
   LDY   #$34
   STA   (FP),Y
   LDY   #$2D
   LDA   (FP),Y
   LDY   #$31
   ORA   (FP),Y
   LDY   #$35
   STA   (FP),Y
   LDY   #$2E
   LDA   (FP),Y
   LDY   #$32
   ORA   (FP),Y
   LDY   #$36
   STA   (FP),Y
   LDA   #$FF
   STA   HARGS
   LDA   #$00
   STA   HARGS+1
   LDY   #$0D
   LDA   (FP),Y
   STA   HARGS+2
   INY
   LDA   (FP),Y
   STA   HARGS+3
   JSR   mul16
   LDA   HARGS+4
   LDY   #$37
   STA   (FP),Y
   LDA   HARGS+5
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ORA   #$01
   LDY   #$39
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$3A
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$3B
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$3C
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$33
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
   LDY   #$3B
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
   LDY   #$3F
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
   LDY   #$3F
   LDA   (FP),Y
   LDY   #$43
   STA   (FP),Y
   LDY   #$40
   LDA   (FP),Y
   LDY   #$44
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$45
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$46
   STA   (FP),Y
   LDY   #$65
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
   LDY   #$63
   LDA   (FP),Y
   CLC
   LDY   #$0D
   SBC   (FP),Y
   LDY   #$64
   LDA   (FP),Y
   LDY   #$0E
   SBC   (FP),Y
   LDA   #$00
   ROL
   LDY   #$49
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$47
   LDA   (FP),Y
   CLC
   LDY   #$49
   ADC   (FP),Y
   LDY   #$4B
   STA   (FP),Y
   LDY   #$48
   LDA   (FP),Y
   LDY   #$4A
   ADC   (FP),Y
   LDY   #$4C
   STA   (FP),Y
   LDY   #$65
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$4B
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$4C
   LDA   (FP),Y
   LDY   #$01
   STA   (DPTR),Y
   LDY   #$65
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$4D
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$4E
   STA   (FP),Y
   LDY   #$63
   LDA   (FP),Y
   AND   #$07
   LDY   #$4F
   STA   (FP),Y
   LDY   #$64
   LDA   (FP),Y
   LDA   #$00
   LDY   #$50
   STA   (FP),Y
   LDY   #$4D
   LDA   (FP),Y
   LDY   #$51
   STA   (FP),Y
   LDY   #$4E
   LDA   (FP),Y
   LDY   #$52
   STA   (FP),Y
   LDY   #$4F
   LDA   (FP),Y
   TAX
   CPX   #$00
   BEQ   .shift_done@8
.shift_loop@7:
   LDY   #$51
   LDA   (FP),Y
   ASL
   STA   (FP),Y
   INY
   LDA   (FP),Y
   ROL
   STA   (FP),Y
   DEX
   BNE   .shift_loop@7
.shift_done@8:
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
   LDY   #$51
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$52
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   JSR   rec
   LDY   #$53
   STA   (FP),Y
   TXA
   INY
   STA   (FP),Y
   LDY   #$1B
   LDA   (FP),Y
   CLC
   LDY   #$53
   ADC   (FP),Y
   LDY   #$55
   STA   (FP),Y
   LDY   #$1C
   LDA   (FP),Y
   LDY   #$54
   ADC   (FP),Y
   LDY   #$56
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$1B
   STA   (FP),Y
   LDY   #$56
   LDA   (FP),Y
   LDY   #$1C
   STA   (FP),Y
   LDY   #$2B
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   INY
   ORA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@1
   LDY   #$2B
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   INY
   ORA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@1
   LDA   #$01
   LDY   #$57
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   JMP   .and_end@2
.and_false@1:
   LDA   #$00
   LDY   #$57
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@2:
   LDY   #$57
   LDA   (FP),Y
   LDY   #$59
   STA   (FP),Y
   AND   #$7F
   INY
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
   LDY   #$5B
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

f3:
   SUBROUTINE

   ; prologue: 0 arg bytes, 74 local bytes
   SEC
   LDA   SSP
   SBC   #$4C
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$4B
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
   INY
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@9
   LDA   #$00
   JMP   .sx_done@10
.sx_neg@9:
   LDA   #$FF
.sx_done@10:
   LDY   #$03
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   LDY   #$07
   STA   (FP),Y
   LDY   #$03
   LDA   (FP),Y
   LDY   #$08
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$07
   LDA   (FP),Y
   LDY   #$0C
   STA   (FP),Y
   LDY   #$08
   LDA   (FP),Y
   LDY   #$0D
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
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
   INY
   STA   (FP),Y
   LDY   #$07
   LDA   (FP),Y
   LDY   #$14
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
   LDY   #$16
   LDA   (FP),Y
   LDY   #$1A
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
   LDY   #$1D
   LDA   (FP),Y
   LDY   #$21
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$1E
   LDA   (FP),Y
   CLC
   LDY   #$07
   ADC   (FP),Y
   LDY   #$23
   STA   (FP),Y
   LDY   #$1F
   LDA   (FP),Y
   LDY   #$08
   ADC   (FP),Y
   LDY   #$24
   STA   (FP),Y
   LDY   #$20
   LDA   (FP),Y
   LDY   #$09
   ADC   (FP),Y
   LDY   #$25
   STA   (FP),Y
   LDY   #$21
   LDA   (FP),Y
   LDY   #$0A
   ADC   (FP),Y
   LDY   #$26
   STA   (FP),Y
   LDY   #$22
   LDA   (FP),Y
   LDY   #$0B
   ADC   (FP),Y
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
   LDY   #$27
   LDA   (FP),Y
   AND   #$1F
   LDY   #$2C
   STA   (FP),Y
   LDY   #$28
   LDA   (FP),Y
   LDY   #$2D
   STA   (FP),Y
   LDY   #$29
   LDA   (FP),Y
   LDY   #$2E
   STA   (FP),Y
   LDY   #$2A
   LDA   (FP),Y
   LDY   #$2F
   STA   (FP),Y
   LDY   #$2B
   LDA   (FP),Y
   LDY   #$30
   STA   (FP),Y
   LDY   #$2C
   LDA   (FP),Y
   EOR   #$10
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
   LDY   #$35
   STA   (FP),Y
   LDY   #$31
   LDA   (FP),Y
   SEC
   SBC   #$10
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
   LDY   #$37
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$38
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   JSR   rec
   LDY   #$39
   STA   (FP),Y
   TXA
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$3B
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$3C
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@11
   LDA   #$00
   JMP   .sx_done@12
.sx_neg@11:
   LDA   #$FF
.sx_done@12:
   LDY   #$3D
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$10
   LDA   (FP),Y
   CLC
   LDY   #$3B
   ADC   (FP),Y
   LDY   #$3F
   STA   (FP),Y
   LDY   #$11
   LDA   (FP),Y
   LDY   #$3C
   ADC   (FP),Y
   LDY   #$40
   STA   (FP),Y
   LDY   #$12
   LDA   (FP),Y
   LDY   #$3D
   ADC   (FP),Y
   LDY   #$41
   STA   (FP),Y
   LDY   #$13
   LDA   (FP),Y
   LDY   #$3E
   ADC   (FP),Y
   LDY   #$42
   STA   (FP),Y
   LDY   #$3F
   LDA   (FP),Y
   LDY   #$10
   STA   (FP),Y
   LDY   #$40
   LDA   (FP),Y
   LDY   #$11
   STA   (FP),Y
   LDY   #$41
   LDA   (FP),Y
   LDY   #$12
   STA   (FP),Y
   LDY   #$42
   LDA   (FP),Y
   LDY   #$13
   STA   (FP),Y
   LDA   g0+4
   CMP   g0+4
   BNE   .cmp_true@13
   LDA   g0+3
   CMP   g0+3
   BNE   .cmp_true@13
   LDA   g0+2
   CMP   g0+2
   BNE   .cmp_true@13
   LDA   g0+1
   CMP   g0+1
   BNE   .cmp_true@13
   LDA   g0
   CMP   g0
   BNE   .cmp_true@13
   LDA   #$00
   JMP   .cmp_end@14
.cmp_true@13:
   LDA   #$01
.cmp_end@14:
   LDY   #$43
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDA   #$03
   SEC
   SBC   #$02
   LDA   #$00
   SBC   #$00
   BVC   .cmp_novf@16
   EOR   #$80
.cmp_novf@16:
   ASL
   LDA   #$00
   ROL
   LDY   #$45
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$43
   LDA   (FP),Y
   LDY   #$45
   ORA   (FP),Y
   LDY   #$47
   STA   (FP),Y
   LDY   #$44
   LDA   (FP),Y
   LDY   #$46
   ORA   (FP),Y
   LDY   #$48
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$49
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$4A
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
   ADC   #$4C
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$4B
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

f2:
   SUBROUTINE

   ; prologue: 8 arg bytes, 27 local bytes
   SEC
   LDA   SSP
   SBC   #$1D
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$1C
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
   LDA   #$03
   INY
   STA   (FP),Y
   LDY   #$1E
   LDA   (FP),Y
   CLC
   SBC   (FP),Y
   LDA   #$00
   ROL
   LDY   #$06
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
   LDA   #$02
   INY
   STA   (SSP),Y
   LDA   #$00
   INY
   STA   (SSP),Y
   JSR   rec
   LDY   #$0A
   STA   (FP),Y
   TXA
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$0C
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$0D
   STA   (FP),Y
   LDY   #$03
   LDA   (FP),Y
   CLC
   LDY   #$0C
   ADC   (FP),Y
   LDY   #$0E
   STA   (FP),Y
   LDY   #$04
   LDA   (FP),Y
   LDY   #$0D
   ADC   (FP),Y
   LDY   #$0F
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$03
   STA   (FP),Y
   LDY   #$0F
   LDA   (FP),Y
   LDY   #$04
   STA   (FP),Y
   INY
   LDA   (FP),Y
   AND   (FP),Y
   LDY   #$10
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$05
   LDA   (FP),Y
   LDY   #$13
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$25
   LDA   (FP),Y
   LDY   #$16
   CMP   (FP),Y
   BNE   .thread@0
   LDY   #$24
   LDA   (FP),Y
   LDY   #$15
   CMP   (FP),Y
   BNE   .thread@0
   LDY   #$23
   LDA   (FP),Y
   LDY   #$14
   CMP   (FP),Y
   BNE   .thread@0
   LDY   #$22
   LDA   (FP),Y
   LDY   #$13
   CMP   (FP),Y
   BEQ   .cmp_true@17
.thread@0:
   LDA   #$00
   JMP   .cmp_end@18
.cmp_true@17:
   LDA   #$01
.cmp_end@18:
   LDY   #$17
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$11
   LDA   (FP),Y
   LDY   #$17
   AND   (FP),Y
   LDY   #$19
   STA   (FP),Y
   LDY   #$12
   LDA   (FP),Y
   LDY   #$18
   AND   (FP),Y
   LDY   #$1A
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$1B
   STA   (FP),Y

   ; epilogue
   PHA
   CLC
   LDA   FP
   ADC   #$25
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

   ; prologue: 6 arg bytes, 132 local bytes
   SEC
   LDA   SSP
   SBC   #$86
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$85
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
   ORA   #$00
   BMI   .sx_neg@20
   LDA   #$00
   JMP   .sx_done@21
.sx_neg@20:
   LDA   #$FF
.sx_done@21:
   LDY   #$02
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
   LDY   #$88
   LDA   (FP),Y
   LDY   #$05
   STA   (FP),Y
   LDY   #$03
   LDA   (FP),Y
   TAX
   BEQ   .shift_done@23
.shift_loop@22:
   LDY   #$05
   LDA   (FP),Y
   ASL
   STA   (FP),Y
   DEX
   BNE   .shift_loop@22
.shift_done@23:
   LDY   #$05
   LDA   (FP),Y
   AND   #$7F
   INY
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
   LDA   #$04
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$89
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
   LDY   #$0B
   LDA   (FP),Y
   STA   HARGS+4
   JSR   lsr32
   LDA   HARGS+5
   LDY   #$0D
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
   LDY   #$0D
   LDA   (FP),Y
   CLC
   LDY   #$07
   SBC   (FP),Y
   LDY   #$0E
   LDA   (FP),Y
   LDY   #$08
   SBC   (FP),Y
   LDY   #$0F
   LDA   (FP),Y
   LDY   #$09
   SBC   (FP),Y
   LDY   #$10
   LDA   (FP),Y
   LDY   #$0A
   SBC   (FP),Y
   LDA   #$00
   ROL
   LDY   #$11
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$13
   STA   (FP),Y
   INY
   STA   (FP),Y
   JSR   f3
   LDY   #$15
   STA   (FP),Y
   TXA
   INY
   STA   (FP),Y
   LDY   #$14
   LDA   (FP),Y
   LDY   #$17
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$15
   EOR   (FP),Y
   LDY   #$19
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$16
   EOR   (FP),Y
   LDY   #$1A
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$1B
   STA   (FP),Y
   LDY   #$14
   STA   (FP),Y
   LDY   #$1C
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
   LDA   g0
   CLC
   LDY   #$1C
   ADC   (FP),Y
   LDY   #$21
   STA   (FP),Y
   LDA   g0+1
   LDY   #$1D
   ADC   (FP),Y
   LDY   #$22
   STA   (FP),Y
   LDA   g0+2
   LDY   #$1E
   ADC   (FP),Y
   LDY   #$23
   STA   (FP),Y
   LDA   g0+3
   LDY   #$1F
   ADC   (FP),Y
   LDY   #$24
   STA   (FP),Y
   LDA   g0+4
   LDY   #$20
   ADC   (FP),Y
   LDY   #$25
   STA   (FP),Y
   LDY   #$21
   LDA   (FP),Y
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
   AND   #$1F
   LDY   #$2A
   STA   (FP),Y
   LDY   #$26
   LDA   (FP),Y
   LDY   #$2B
   STA   (FP),Y
   LDY   #$27
   LDA   (FP),Y
   LDY   #$2C
   STA   (FP),Y
   LDY   #$28
   LDA   (FP),Y
   LDY   #$2D
   STA   (FP),Y
   LDY   #$29
   LDA   (FP),Y
   LDY   #$2E
   STA   (FP),Y
   LDY   #$2A
   LDA   (FP),Y
   EOR   #$10
   LDY   #$2F
   STA   (FP),Y
   LDY   #$2B
   LDA   (FP),Y
   LDY   #$30
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
   SEC
   SBC   #$10
   LDY   #$34
   STA   (FP),Y
   LDY   #$30
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
   LDY   #$87
   LDA   (FP),Y
   LDY   #$35
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@24
   LDA   #$00
   JMP   .sx_done@25
.sx_neg@24:
   LDA   #$FF
.sx_done@25:
   LDY   #$36
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   SEC
   SBC   #$FF
   INY
   LDA   (FP),Y
   SBC   #$00
   BVC   .cmp_novf@26
   EOR   #$80
.cmp_novf@26:
   ASL
   LDA   #$00
   ROL
   EOR   #$01
   LDY   #$37
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$39
   STA   (FP),Y
   AND   #$7F
   INY
   STA   (FP),Y
   LDY   #$14
   LDA   (FP),Y
   LDY   #$3B
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDA   #$04
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$3B
   LDA   (FP),Y
   LDY   #$3F
   STA   (FP),Y
   LDY   #$3C
   LDA   (FP),Y
   LDY   #$40
   STA   (FP),Y
   LDY   #$3D
   LDA   (FP),Y
   TAX
   CPX   #$00
   BEQ   .shift_done@28
.shift_loop@27:
   LDY   #$3F
   LDA   (FP),Y
   ASL
   STA   (FP),Y
   INY
   LDA   (FP),Y
   ROL
   STA   (FP),Y
   DEX
   BNE   .shift_loop@27
.shift_done@28:
   LDY   #$14
   LDA   (FP),Y
   LDY   #$41
   STA   (FP),Y
   LDA   #$00
   INY
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
   BMI   .sx_neg@29
   LDA   #$00
   JMP   .sx_done@30
.sx_neg@29:
   LDA   #$FF
.sx_done@30:
   LDY   #$45
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$43
   LDA   (FP),Y
   LDY   #$89
   EOR   (FP),Y
   LDY   #$47
   STA   (FP),Y
   LDY   #$44
   LDA   (FP),Y
   LDY   #$8A
   EOR   (FP),Y
   LDY   #$48
   STA   (FP),Y
   LDY   #$45
   LDA   (FP),Y
   LDY   #$8B
   EOR   (FP),Y
   LDY   #$49
   STA   (FP),Y
   LDY   #$46
   LDA   (FP),Y
   LDY   #$8C
   EOR   (FP),Y
   LDY   #$4A
   STA   (FP),Y
   LDY   #$47
   LDA   (FP),Y
   LDY   #$4B
   STA   (FP),Y
   LDY   #$87
   LDA   (FP),Y
   BEQ   .and_false@3
   LDY   #$89
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   INY
   ORA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@3
   LDA   #$01
   LDY   #$4C
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   JMP   .and_end@4
.and_false@3:
   LDA   #$00
   LDY   #$4C
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@4:
   LDY   #$4C
   LDA   (FP),Y
   LDY   #$4E
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$4F
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@31
   LDA   #$00
   JMP   .sx_done@32
.sx_neg@31:
   LDA   #$FF
.sx_done@32:
   LDY   #$50
   STA   (FP),Y
   INY
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$08
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDY   #$3A
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$3F
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$40
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$4B
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   LDY   #$4E
   LDA   (FP),Y
   LDY   #$05
   STA   (SSP),Y
   LDY   #$4F
   LDA   (FP),Y
   LDY   #$06
   STA   (SSP),Y
   LDY   #$50
   LDA   (FP),Y
   LDY   #$07
   STA   (SSP),Y
   LDY   #$51
   LDA   (FP),Y
   LDY   #$08
   STA   (SSP),Y
   JSR   f2
   LDY   #$52
   STA   (FP),Y
   ORA   #$00
   BEQ   .if_else@6
   LDY   #$14
   LDA   (FP),Y
   LDY   #$53
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$03
   LDY   #$55
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$56
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$57
   STA   (FP),Y
   LDY   #$14
   STA   (FP),Y
   JMP   .if_end@5
.if_else@6:
   LDY   #$14
   LDA   (FP),Y
   LDY   #$58
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   EOR   #$05
   LDY   #$5A
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$5B
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$5C
   STA   (FP),Y
   LDY   #$14
   STA   (FP),Y
.if_end@5:
   LDY   #$5D
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
   LDA   g0
   CLC
   LDY   #$5D
   ADC   (FP),Y
   LDY   #$62
   STA   (FP),Y
   LDA   g0+1
   LDY   #$5E
   ADC   (FP),Y
   LDY   #$63
   STA   (FP),Y
   LDA   g0+2
   LDY   #$5F
   ADC   (FP),Y
   LDY   #$64
   STA   (FP),Y
   LDA   g0+3
   LDY   #$60
   ADC   (FP),Y
   LDY   #$65
   STA   (FP),Y
   LDA   g0+4
   LDY   #$61
   ADC   (FP),Y
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
   LDY   #$69
   STA   (FP),Y
   LDY   #$65
   LDA   (FP),Y
   LDY   #$6A
   STA   (FP),Y
   LDY   #$66
   LDA   (FP),Y
   AND   #$1F
   LDY   #$6B
   STA   (FP),Y
   LDY   #$67
   LDA   (FP),Y
   LDY   #$6C
   STA   (FP),Y
   LDY   #$68
   LDA   (FP),Y
   LDY   #$6D
   STA   (FP),Y
   LDY   #$69
   LDA   (FP),Y
   LDY   #$6E
   STA   (FP),Y
   LDY   #$6A
   LDA   (FP),Y
   LDY   #$6F
   STA   (FP),Y
   LDY   #$6B
   LDA   (FP),Y
   EOR   #$10
   LDY   #$70
   STA   (FP),Y
   LDY   #$6C
   LDA   (FP),Y
   LDY   #$71
   STA   (FP),Y
   LDY   #$6D
   LDA   (FP),Y
   LDY   #$72
   STA   (FP),Y
   LDY   #$6E
   LDA   (FP),Y
   LDY   #$73
   STA   (FP),Y
   LDY   #$6F
   LDA   (FP),Y
   LDY   #$74
   STA   (FP),Y
   LDY   #$70
   LDA   (FP),Y
   SEC
   SBC   #$10
   LDY   #$75
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
   INY
   LDA   (FP),Y
   STA   g0+4
   LDY   #$14
   LDA   (FP),Y
   LDY   #$76
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$E9
   INY
   STA   (FP),Y
   LDA   #$03
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$7C
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$7D
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@33
   LDA   #$00
   JMP   .sx_done@34
.sx_neg@33:
   LDA   #$FF
.sx_done@34:
   LDY   #$7E
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$76
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
   LDY   #$7C
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
   LDY   #$80
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
   DEY
   ORA   (FP),Y
   DEY
   ORA   (FP),Y
   DEY
   ORA   (FP),Y
   BEQ   .cmp_end@35
   LDA   #$01
.cmp_end@35:
   LDY   #$84
   STA   (FP),Y

   ; epilogue
   PHA
   CLC
   LDA   FP
   ADC   #$8C
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

   ; prologue: 10 arg bytes, 73 local bytes
   SEC
   LDA   SSP
   SBC   #$4B
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$4A
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   LDY   #$4F
   LDA   (FP),Y
   ORA   #$01
   LDY   #$01
   STA   (FP),Y
   LDY   #$50
   LDA   (FP),Y
   LDY   #$02
   STA   (FP),Y
   LDY   #$51
   LDA   (FP),Y
   LDY   #$03
   STA   (FP),Y
   LDY   #$52
   LDA   (FP),Y
   LDY   #$04
   STA   (FP),Y
   LDY   #$53
   LDA   (FP),Y
   LDY   #$05
   STA   (FP),Y
   LDY   #$4F
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
   LDY   #$01
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
   LDY   #$06
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
   LDY   #$06
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   INY
   ORA   (FP),Y
   INY
   ORA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@0
   JMP   .and_false@7
.lb_skip@0:
   LDY   #$4D
   LDA   (FP),Y
   LDY   #$0B
   STA   (FP),Y
   LDY   #$4E
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
   LDY   #$0B
   LDA   (FP),Y
   SEC
   LDY   #$4F
   SBC   (FP),Y
   LDY   #$10
   STA   (FP),Y
   LDY   #$0C
   LDA   (FP),Y
   LDY   #$50
   SBC   (FP),Y
   LDY   #$11
   STA   (FP),Y
   LDY   #$0D
   LDA   (FP),Y
   LDY   #$51
   SBC   (FP),Y
   LDY   #$12
   STA   (FP),Y
   LDY   #$0E
   LDA   (FP),Y
   LDY   #$52
   SBC   (FP),Y
   LDY   #$13
   STA   (FP),Y
   LDY   #$0F
   LDA   (FP),Y
   LDY   #$53
   SBC   (FP),Y
   LDY   #$14
   STA   (FP),Y
   LDY   #$10
   LDA   (FP),Y
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
   AND   #$1F
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
   LDY   #$1C
   STA   (FP),Y
   LDY   #$18
   LDA   (FP),Y
   LDY   #$1D
   STA   (FP),Y
   LDY   #$19
   LDA   (FP),Y
   EOR   #$10
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
   LDY   #$21
   STA   (FP),Y
   LDY   #$1D
   LDA   (FP),Y
   LDY   #$22
   STA   (FP),Y
   LDY   #$1E
   LDA   (FP),Y
   SEC
   SBC   #$10
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
   BEQ   .and_false@7
   LDA   #$01
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   JMP   .and_end@8
.and_false@7:
   LDA   #$00
   LDY   #$24
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@8:
   LDY   #$24
   LDA   (FP),Y
   LDY   #$26
   STA   (FP),Y
   AND   #$7F
   INY
   STA   (FP),Y
   INY
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
   LDY   #$29
   LDA   (FP),Y
   SEC
   LDY   #$4F
   SBC   (FP),Y
   LDY   #$2A
   LDA   (FP),Y
   LDY   #$50
   SBC   (FP),Y
   LDY   #$2B
   LDA   (FP),Y
   LDY   #$51
   SBC   (FP),Y
   LDY   #$2C
   LDA   (FP),Y
   LDY   #$52
   SBC   (FP),Y
   LDY   #$2D
   LDA   (FP),Y
   LDY   #$53
   SBC   (FP),Y
   BVC   .cmp_novf@36
   EOR   #$80
.cmp_novf@36:
   ASL
   LDA   #$00
   ROL
   EOR   #$01
   LDY   #$2E
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@9
   LDY   #$4D
   LDA   (FP),Y
   LDY   #$30
   STA   (FP),Y
   LDY   #$4E
   LDA   (FP),Y
   LDY   #$31
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@9
   LDA   #$01
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   JMP   .and_end@10
.and_false@9:
   LDA   #$00
   LDY   #$32
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@10:
   LDY   #$32
   LDA   (FP),Y
   LDY   #$34
   STA   (FP),Y
   AND   #$7F
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$54
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$37
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$38
   STA   (FP),Y
   LDY   #$4F
   LDA   (FP),Y
   LDY   #$39
   STA   (FP),Y
   LDY   #$50
   LDA   (FP),Y
   LDY   #$3A
   STA   (FP),Y
   LDY   #$37
   LDA   (FP),Y
   CLC
   LDY   #$39
   ADC   (FP),Y
   LDY   #$3B
   STA   (FP),Y
   LDY   #$38
   LDA   (FP),Y
   LDY   #$3A
   ADC   (FP),Y
   LDY   #$3C
   STA   (FP),Y
   LDY   #$54
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
   LDY   #$4C
   LDA   (FP),Y
   LDY   #$3D
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$4E
   CMP   (FP),Y
   BNE   .cmp_true@37
   LDY   #$3D
   LDA   (FP),Y
   LDY   #$4D
   CMP   (FP),Y
   BNE   .cmp_true@37
   LDA   #$00
   JMP   .cmp_end@38
.cmp_true@37:
   LDA   #$01
.cmp_end@38:
   LDY   #$3F
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
   LDY   #$3F
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$40
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   JSR   rec
   LDY   #$41
   STA   (FP),Y
   TXA
   INY
   STA   (FP),Y
   LDY   #$28
   LDA   (FP),Y
   LDY   #$43
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   LDY   #$41
   ADC   (FP),Y
   LDY   #$45
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$42
   ADC   (FP),Y
   LDY   #$46
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$47
   STA   (FP),Y
   AND   #$7F
   INY
   STA   (FP),Y
   LDY   #$28
   STA   (FP),Y
   LDY   #$4F
   LDA   (FP),Y
   LDY   #$49
   STA   (FP),Y

   ; epilogue
   PHA
   CLC
   LDA   FP
   ADC   #$55
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
   LDA   #$49
   LDY   #$01
   STA   (SSP),Y
   LDA   #$F8
   INY
   STA   (SSP),Y
   LDA   #$00
   INY
   STA   (SSP),Y
   LDA   #$28
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
   LDA   #$00
   INY
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
   DC.B  $90
   DS.B  4
