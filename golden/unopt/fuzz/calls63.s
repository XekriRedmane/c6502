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

f5:
   SUBROUTINE

   ; prologue: 8 arg bytes, 42 local bytes
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

   LDY   #$33
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   LDY   #$34
   LDA   (FP),Y
   LDY   #$02
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   SEC
   LDY   #$33
   SBC   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   LDY   #$34
   SBC   (FP),Y
   BVC   .cmp_novf@1
   EOR   #$80
.cmp_novf@1:
   ASL
   LDA   #$00
   ROL
   EOR   #$01
   LDY   #$03
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   EOR   #$FF
   LDY   #$05
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   EOR   #$FF
   LDY   #$06
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$07
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$08
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
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
   LDY   #$09
   LDA   (FP),Y
   LDY   #$0D
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$0B
   LDA   (FP),Y
   LDY   #$0F
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$01
   LDA   (FP),Y
   LDY   #$11
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   LDY   #$14
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@4
   LDA   #$00
   JMP   .sx_done@5
.sx_neg@4:
   LDA   #$FF
.sx_done@5:
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
   LDY   #$13
   LDA   (FP),Y
   LDY   #$1A
   STA   (FP),Y
   LDY   #$14
   LDA   (FP),Y
   LDY   #$1B
   STA   (FP),Y
   LDY   #$15
   LDA   (FP),Y
   LDY   #$1C
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   AND   #$7F
   INY
   STA   (FP),Y
   LDY   #$12
   LDA   (FP),Y
   LDY   #$21
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
   LDY   #$1A
   LDA   (FP),Y
   CLC
   LDY   #$21
   SBC   (FP),Y
   LDY   #$1B
   LDA   (FP),Y
   LDY   #$22
   SBC   (FP),Y
   LDY   #$1C
   LDA   (FP),Y
   LDY   #$23
   SBC   (FP),Y
   LDY   #$1D
   LDA   (FP),Y
   LDY   #$24
   SBC   (FP),Y
   LDY   #$1E
   LDA   (FP),Y
   LDY   #$25
   SBC   (FP),Y
   LDY   #$1F
   LDA   (FP),Y
   LDY   #$26
   SBC   (FP),Y
   LDY   #$20
   LDA   (FP),Y
   LDY   #$27
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
   LDY   #$2A
   STA   (FP),Y
   CMP   #$01

   ; epilogue
   PHP
   CLC
   LDA   FP
   ADC   #$34
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

   ; prologue: 6 arg bytes, 119 local bytes
   SEC
   LDA   SSP
   SBC   #$79
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$78
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
   LDA   #$7F
   INY
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@6
   LDA   #$00
   JMP   .sx_done@7
.sx_neg@6:
   LDA   #$FF
.sx_done@7:
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
   LDA   g0+3
   BMI   .sx_neg@8
   LDA   #$00
   JMP   .sx_done@9
.sx_neg@8:
   LDA   #$FF
.sx_done@9:
   LDY   #$0D
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$09
   LDA   (FP),Y
   LDY   #$10
   STA   (FP),Y
   LDY   #$0A
   LDA   (FP),Y
   LDY   #$11
   STA   (FP),Y
   LDY   #$0B
   LDA   (FP),Y
   LDY   #$12
   STA   (FP),Y
   LDY   #$0C
   LDA   (FP),Y
   LDY   #$13
   STA   (FP),Y
   LDY   #$0D
   LDA   (FP),Y
   LDY   #$14
   STA   (FP),Y
   INY
   STA   (FP),Y
   AND   #$7F
   INY
   STA   (FP),Y
   LDA   g1
   SEC
   LDY   #$10
   SBC   (FP),Y
   LDY   #$17
   STA   (FP),Y
   LDA   g1+1
   LDY   #$11
   SBC   (FP),Y
   LDY   #$18
   STA   (FP),Y
   LDA   g1+2
   LDY   #$12
   SBC   (FP),Y
   LDY   #$19
   STA   (FP),Y
   LDA   g1+3
   LDY   #$13
   SBC   (FP),Y
   LDY   #$1A
   STA   (FP),Y
   LDA   g1+4
   LDY   #$14
   SBC   (FP),Y
   LDY   #$1B
   STA   (FP),Y
   LDA   g1+5
   LDY   #$15
   SBC   (FP),Y
   LDY   #$1C
   STA   (FP),Y
   LDA   g1+6
   LDY   #$16
   SBC   (FP),Y
   LDY   #$1D
   STA   (FP),Y
   LDY   #$17
   LDA   (FP),Y
   LDY   #$1E
   STA   (FP),Y
   LDY   #$18
   LDA   (FP),Y
   LDY   #$1F
   STA   (FP),Y
   LDY   #$19
   LDA   (FP),Y
   LDY   #$20
   STA   (FP),Y
   LDY   #$1A
   LDA   (FP),Y
   LDY   #$21
   STA   (FP),Y
   LDY   #$1B
   LDA   (FP),Y
   LDY   #$22
   STA   (FP),Y
   LDY   #$1C
   LDA   (FP),Y
   LDY   #$23
   STA   (FP),Y
   LDY   #$1D
   LDA   (FP),Y
   AND   #$7F
   LDY   #$24
   STA   (FP),Y
   LDA   g0+3
   LDY   #$08
   CMP   (FP),Y
   BNE   .thread@0
   LDA   g0+2
   DEY
   CMP   (FP),Y
   BNE   .thread@0
   LDA   g0+1
   DEY
   CMP   (FP),Y
   BNE   .thread@0
   LDA   g0
   DEY
   CMP   (FP),Y
   BEQ   .cmp_true@10
.thread@0:
   LDA   #$00
   JMP   .cmp_end@11
.cmp_true@10:
   LDA   #$01
.cmp_end@11:
   LDY   #$25
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$27
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$28
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@13
   LDA   #$00
   JMP   .sx_done@14
.sx_neg@13:
   LDA   #$FF
.sx_done@14:
   LDY   #$29
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$27
   LDA   (FP),Y
   LDY   #$2E
   STA   (FP),Y
   LDY   #$28
   LDA   (FP),Y
   LDY   #$2F
   STA   (FP),Y
   LDY   #$29
   LDA   (FP),Y
   LDY   #$30
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   AND   #$7F
   INY
   STA   (FP),Y
   LDY   #$2E
   LDA   (FP),Y
   SEC
   LDY   #$1E
   SBC   (FP),Y
   LDY   #$2F
   LDA   (FP),Y
   LDY   #$1F
   SBC   (FP),Y
   LDY   #$30
   LDA   (FP),Y
   LDY   #$20
   SBC   (FP),Y
   LDY   #$31
   LDA   (FP),Y
   LDY   #$21
   SBC   (FP),Y
   LDY   #$32
   LDA   (FP),Y
   LDY   #$22
   SBC   (FP),Y
   LDY   #$33
   LDA   (FP),Y
   LDY   #$23
   SBC   (FP),Y
   LDY   #$34
   LDA   (FP),Y
   LDY   #$24
   SBC   (FP),Y
   LDA   #$00
   ROL
   LDY   #$35
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$37
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   g1
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$FF
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   ORA   #$00
   BPL   .sx_done@16
   LDA   #$FF
.sx_done@16:
   LDY   #$3D
   STA   (FP),Y
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
   LDY   #$41
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$7E
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
   DEY
   LDA   (FP),Y
   STA   HARGS+4
   JSR   asl32
   LDA   HARGS+5
   LDY   #$47
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
   LDY   #$47
   LDA   (FP),Y
   LDY   #$4B
   STA   (FP),Y
   LDY   #$48
   LDA   (FP),Y
   LDY   #$4C
   STA   (FP),Y
   LDY   #$43
   LDA   (FP),Y
   CLC
   LDY   #$4B
   ADC   (FP),Y
   LDY   #$4D
   STA   (FP),Y
   LDY   #$44
   LDA   (FP),Y
   LDY   #$4C
   ADC   (FP),Y
   LDY   #$4E
   STA   (FP),Y
   LDY   #$7E
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$4D
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$4E
   LDA   (FP),Y
   LDY   #$01
   STA   (DPTR),Y
   LDY   #$7C
   LDA   (FP),Y
   LDY   #$4F
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$51
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$52
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   SEC
   LDY   #$7A
   SBC   (FP),Y
   LDY   #$52
   LDA   (FP),Y
   LDY   #$7B
   SBC   (FP),Y
   LDA   #$00
   ROL
   LDY   #$53
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$55
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$56
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@17
   LDA   #$00
   JMP   .sx_done@18
.sx_neg@17:
   LDA   #$FF
.sx_done@18:
   LDY   #$57
   STA   (FP),Y
   INY
   STA   (FP),Y
   SEC
   LDA   #$00
   LDY   #$05
   SBC   (FP),Y
   LDY   #$59
   STA   (FP),Y
   LDA   #$00
   LDY   #$06
   SBC   (FP),Y
   LDY   #$5A
   STA   (FP),Y
   LDA   #$00
   LDY   #$07
   SBC   (FP),Y
   LDY   #$5B
   STA   (FP),Y
   LDA   #$00
   LDY   #$08
   SBC   (FP),Y
   LDY   #$5C
   STA   (FP),Y
   LDY   #$59
   LDA   (FP),Y
   LDY   #$5D
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$38
   LDA   (FP),Y
   LDY   #$5F
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$08
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDY   #$55
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$56
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$57
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$58
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   LDY   #$5D
   LDA   (FP),Y
   LDY   #$05
   STA   (SSP),Y
   LDY   #$5E
   LDA   (FP),Y
   LDY   #$06
   STA   (SSP),Y
   LDY   #$5F
   LDA   (FP),Y
   LDY   #$07
   STA   (SSP),Y
   LDY   #$60
   LDA   (FP),Y
   LDY   #$08
   STA   (SSP),Y
   JSR   f5
   LDA   #$00
   ROL
   LDY   #$61
   STA   (FP),Y
   ORA   #$00
   BEQ   .if_else@2
   LDY   #$38
   LDA   (FP),Y
   LDY   #$62
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$03
   LDY   #$64
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$65
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$66
   STA   (FP),Y
   LDY   #$38
   STA   (FP),Y
   JMP   .if_end@1
.if_else@2:
   LDY   #$38
   LDA   (FP),Y
   LDY   #$67
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   EOR   #$05
   LDY   #$69
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$6A
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$6B
   STA   (FP),Y
   LDY   #$38
   STA   (FP),Y
.if_end@1:
   LDY   #$7E
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$6C
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$6D
   STA   (FP),Y
   LDY   #$7E
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$6E
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$6F
   STA   (FP),Y
   LDY   #$6D
   LDA   (FP),Y
   LDY   #$6F
   CMP   (FP),Y
   BNE   .thread@1
   LDY   #$6C
   LDA   (FP),Y
   LDY   #$6E
   CMP   (FP),Y
   BEQ   .cmp_true@19
.thread@1:
   LDA   #$00
   JMP   .cmp_end@20
.cmp_true@19:
   LDA   #$01
.cmp_end@20:
   LDY   #$70
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@3
   LDY   #$7A
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@3
   LDA   #$01
   LDY   #$72
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   JMP   .and_end@4
.and_false@3:
   LDA   #$00
   LDY   #$72
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@4:
   LDY   #$72
   LDA   (FP),Y
   LDY   #$74
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$75
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@22
   LDA   #$00
   JMP   .sx_done@23
.sx_neg@22:
   LDA   #$FF
.sx_done@23:
   LDY   #$76
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$74
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
   ADC   #$7F
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

   ; prologue: 7 arg bytes, 147 local bytes
   SEC
   LDA   SSP
   SBC   #$95
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$94
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   LDY   #$9C
   LDA   (FP),Y
   CMP   (FP),Y
   BNE   .cmp_true@24
   DEY
   LDA   (FP),Y
   CMP   (FP),Y
   BNE   .cmp_true@24
   DEY
   LDA   (FP),Y
   CMP   (FP),Y
   BNE   .cmp_true@24
   DEY
   LDA   (FP),Y
   CMP   (FP),Y
   BNE   .cmp_true@24
   DEY
   LDA   (FP),Y
   CMP   (FP),Y
   BNE   .cmp_true@24
   DEY
   LDA   (FP),Y
   CMP   (FP),Y
   BNE   .cmp_true@24
   DEY
   LDA   (FP),Y
   CMP   (FP),Y
   BNE   .cmp_true@24
   LDA   #$00
   JMP   .cmp_end@25
.cmp_true@24:
   LDA   #$01
.cmp_end@25:
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
   ORA   #$00
   BMI   .sx_neg@27
   LDA   #$00
   JMP   .sx_done@28
.sx_neg@27:
   LDA   #$FF
.sx_done@28:
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
   LDY   #$03
   LDA   (FP),Y
   LDY   #$0A
   STA   (FP),Y
   LDY   #$04
   LDA   (FP),Y
   LDY   #$0B
   STA   (FP),Y
   LDY   #$05
   LDA   (FP),Y
   LDY   #$0C
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   AND   #$7F
   INY
   STA   (FP),Y
   LDY   #$0A
   LDA   (FP),Y
   LDY   #$11
   STA   (FP),Y
   LDY   #$0B
   LDA   (FP),Y
   LDY   #$12
   STA   (FP),Y
   LDY   #$0C
   LDA   (FP),Y
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
   LDA   g0+3
   BMI   .sx_neg@29
   LDA   #$00
   JMP   .sx_done@30
.sx_neg@29:
   LDA   #$FF
.sx_done@30:
   LDY   #$1D
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$19
   LDA   (FP),Y
   LDY   #$20
   STA   (FP),Y
   LDY   #$1A
   LDA   (FP),Y
   LDY   #$21
   STA   (FP),Y
   LDY   #$1B
   LDA   (FP),Y
   LDY   #$22
   STA   (FP),Y
   LDY   #$1C
   LDA   (FP),Y
   LDY   #$23
   STA   (FP),Y
   LDY   #$1D
   LDA   (FP),Y
   LDY   #$24
   STA   (FP),Y
   INY
   STA   (FP),Y
   AND   #$7F
   INY
   STA   (FP),Y
   LDY   #$20
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
   LDA   g1
   STA   HARGS+8
   LDA   g1+1
   STA   HARGS+9
   LDA   g1+2
   STA   HARGS+10
   LDA   g1+3
   STA   HARGS+11
   LDA   g1+4
   STA   HARGS+12
   LDA   g1+5
   STA   HARGS+13
   LDA   g1+6
   STA   HARGS+14
   LDA   #$00
   STA   HARGS+7
   STA   HARGS+15
   JSR   mul64
   LDA   HARGS+16
   LDY   #$27
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
   LDY   #$27
   LDA   (FP),Y
   LDY   #$2E
   STA   (FP),Y
   LDY   #$28
   LDA   (FP),Y
   LDY   #$2F
   STA   (FP),Y
   LDY   #$29
   LDA   (FP),Y
   LDY   #$30
   STA   (FP),Y
   LDY   #$2A
   LDA   (FP),Y
   LDY   #$31
   STA   (FP),Y
   LDY   #$2B
   LDA   (FP),Y
   LDY   #$32
   STA   (FP),Y
   LDY   #$2C
   LDA   (FP),Y
   LDY   #$33
   STA   (FP),Y
   LDY   #$2D
   LDA   (FP),Y
   AND   #$7F
   LDY   #$34
   STA   (FP),Y
   LDA   g0
   ORA   g0+1
   ORA   g0+2
   ORA   g0+3
   BEQ   .lnot_true@31
   LDA   #$00
   JMP   .lnot_end@32
.lnot_true@31:
   LDA   #$01
.lnot_end@32:
   LDY   #$35
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$37
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$38
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@33
   LDA   #$00
   JMP   .sx_done@34
.sx_neg@33:
   LDA   #$FF
.sx_done@34:
   LDY   #$39
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$37
   LDA   (FP),Y
   LDY   #$3E
   STA   (FP),Y
   LDY   #$38
   LDA   (FP),Y
   LDY   #$3F
   STA   (FP),Y
   LDY   #$39
   LDA   (FP),Y
   LDY   #$40
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   AND   #$7F
   INY
   STA   (FP),Y
   LDY   #$2E
   LDA   (FP),Y
   SEC
   LDY   #$3E
   SBC   (FP),Y
   LDY   #$2F
   LDA   (FP),Y
   LDY   #$3F
   SBC   (FP),Y
   LDY   #$30
   LDA   (FP),Y
   LDY   #$40
   SBC   (FP),Y
   LDY   #$31
   LDA   (FP),Y
   LDY   #$41
   SBC   (FP),Y
   LDY   #$32
   LDA   (FP),Y
   LDY   #$42
   SBC   (FP),Y
   LDY   #$33
   LDA   (FP),Y
   LDY   #$43
   SBC   (FP),Y
   LDY   #$34
   LDA   (FP),Y
   LDY   #$44
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
   LDY   #$47
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$48
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@35
   LDA   #$00
   JMP   .sx_done@36
.sx_neg@35:
   LDA   #$FF
.sx_done@36:
   LDY   #$49
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$47
   LDA   (FP),Y
   LDY   #$4E
   STA   (FP),Y
   LDY   #$48
   LDA   (FP),Y
   LDY   #$4F
   STA   (FP),Y
   LDY   #$49
   LDA   (FP),Y
   LDY   #$50
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   AND   #$7F
   INY
   STA   (FP),Y
   LDY   #$4E
   LDA   (FP),Y
   LDY   #$55
   STA   (FP),Y
   LDY   #$4F
   LDA   (FP),Y
   LDY   #$56
   STA   (FP),Y
   LDY   #$50
   LDA   (FP),Y
   LDY   #$57
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$54
   LDA   (FP),Y
   LDY   #$5B
   STA   (FP),Y
   LDA   #$64
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
   LDY   #$15
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   INY
   ORA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lnot_true@37
   LDA   #$00
   JMP   .lnot_end@38
.lnot_true@37:
   LDA   #$01
.lnot_end@38:
   LDY   #$63
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
   LDY   #$63
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$64
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   JSR   rec
   LDY   #$65
   STA   (FP),Y
   TXA
   INY
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
   BMI   .sx_neg@39
   LDA   #$00
   JMP   .sx_done@40
.sx_neg@39:
   LDA   #$FF
.sx_done@40:
   LDY   #$69
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$67
   LDA   (FP),Y
   LDY   #$6E
   STA   (FP),Y
   LDY   #$68
   LDA   (FP),Y
   LDY   #$6F
   STA   (FP),Y
   LDY   #$69
   LDA   (FP),Y
   LDY   #$70
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
   LDY   #$5C
   LDA   (FP),Y
   CLC
   LDY   #$6E
   ADC   (FP),Y
   LDY   #$75
   STA   (FP),Y
   LDY   #$5D
   LDA   (FP),Y
   LDY   #$6F
   ADC   (FP),Y
   LDY   #$76
   STA   (FP),Y
   LDY   #$5E
   LDA   (FP),Y
   LDY   #$70
   ADC   (FP),Y
   LDY   #$77
   STA   (FP),Y
   LDY   #$5F
   LDA   (FP),Y
   LDY   #$71
   ADC   (FP),Y
   LDY   #$78
   STA   (FP),Y
   LDY   #$60
   LDA   (FP),Y
   LDY   #$72
   ADC   (FP),Y
   LDY   #$79
   STA   (FP),Y
   LDY   #$61
   LDA   (FP),Y
   LDY   #$73
   ADC   (FP),Y
   LDY   #$7A
   STA   (FP),Y
   LDY   #$62
   LDA   (FP),Y
   LDY   #$74
   ADC   (FP),Y
   LDY   #$7B
   STA   (FP),Y
   LDY   #$75
   LDA   (FP),Y
   LDY   #$7C
   STA   (FP),Y
   LDY   #$76
   LDA   (FP),Y
   LDY   #$7D
   STA   (FP),Y
   LDY   #$77
   LDA   (FP),Y
   LDY   #$7E
   STA   (FP),Y
   LDY   #$78
   LDA   (FP),Y
   LDY   #$7F
   STA   (FP),Y
   LDY   #$79
   LDA   (FP),Y
   LDY   #$80
   STA   (FP),Y
   LDY   #$7A
   LDA   (FP),Y
   LDY   #$81
   STA   (FP),Y
   LDY   #$7B
   LDA   (FP),Y
   AND   #$07
   LDY   #$82
   STA   (FP),Y
   LDY   #$7C
   LDA   (FP),Y
   LDY   #$5C
   STA   (FP),Y
   LDY   #$7D
   LDA   (FP),Y
   LDY   #$5D
   STA   (FP),Y
   LDY   #$7E
   LDA   (FP),Y
   LDY   #$5E
   STA   (FP),Y
   LDY   #$7F
   LDA   (FP),Y
   LDY   #$5F
   STA   (FP),Y
   LDY   #$80
   LDA   (FP),Y
   LDY   #$60
   STA   (FP),Y
   LDY   #$81
   LDA   (FP),Y
   LDY   #$61
   STA   (FP),Y
   LDY   #$82
   LDA   (FP),Y
   LDY   #$62
   STA   (FP),Y
   LDA   g1
   ORA   #$01
   LDY   #$83
   STA   (FP),Y
   LDA   g1+1
   INY
   STA   (FP),Y
   LDA   g1+2
   INY
   STA   (FP),Y
   LDA   g1+3
   INY
   STA   (FP),Y
   LDA   g1+4
   INY
   STA   (FP),Y
   LDA   g1+5
   INY
   STA   (FP),Y
   LDA   g1+6
   INY
   STA   (FP),Y
   LDY   #$55
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
   LDY   #$83
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
   JSR   udivmod64
   LDA   HARGS+16
   LDY   #$8A
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
   LDY   #$8A
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
   BEQ   .lnot_true@41
   LDA   #$00
   JMP   .lnot_end@42
.lnot_true@41:
   LDA   #$01
.lnot_end@42:
   LDY   #$91
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$93
   STA   (FP),Y

   ; epilogue
   PHA
   CLC
   LDA   FP
   ADC   #$9C
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

f2:
   SUBROUTINE

   ; prologue: 0 arg bytes, 156 local bytes
   SEC
   LDA   SSP
   SBC   #$9E
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$9D
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
   BEQ   .and_false@5
   LDA   g1
   ORA   g1+1
   ORA   g1+2
   ORA   g1+3
   ORA   g1+4
   ORA   g1+5
   ORA   g1+6
   BEQ   .and_false@5
   LDA   #$01
   LDY   #$01
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   JMP   .and_end@6
.and_false@5:
   LDA   #$00
   LDY   #$01
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@6:
   LDY   #$01
   LDA   (FP),Y
   LDY   #$03
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$04
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@43
   LDA   #$00
   JMP   .sx_done@44
.sx_neg@43:
   LDA   #$FF
.sx_done@44:
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
   LDY   #$03
   LDA   (FP),Y
   LDY   #$0A
   STA   (FP),Y
   LDY   #$04
   LDA   (FP),Y
   LDY   #$0B
   STA   (FP),Y
   LDY   #$05
   LDA   (FP),Y
   LDY   #$0C
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   AND   #$7F
   INY
   STA   (FP),Y
   LDY   #$0A
   LDA   (FP),Y
   SEC
   SBC   g1
   INY
   LDA   (FP),Y
   SBC   g1+1
   INY
   LDA   (FP),Y
   SBC   g1+2
   INY
   LDA   (FP),Y
   SBC   g1+3
   INY
   LDA   (FP),Y
   SBC   g1+4
   INY
   LDA   (FP),Y
   SBC   g1+5
   INY
   LDA   (FP),Y
   SBC   g1+6
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
   ORA   #$00
   BMI   .sx_neg@45
   LDA   #$00
   JMP   .sx_done@46
.sx_neg@45:
   LDA   #$FF
.sx_done@46:
   LDY   #$15
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
   LDY   #$15
   LDA   (FP),Y
   LDY   #$19
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   g1
   ORA   g1+1
   ORA   g1+2
   ORA   g1+3
   ORA   g1+4
   ORA   g1+5
   ORA   g1+6
   BNE   .or_true@7
   LDA   g0
   ORA   g0+1
   ORA   g0+2
   ORA   g0+3
   BNE   .or_true@7
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   JMP   .or_end@8
.or_true@7:
   LDA   #$01
   LDY   #$1B
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@8:
   LDY   #$1B
   LDA   (FP),Y
   LDY   #$1D
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$1E
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@47
   LDA   #$00
   JMP   .sx_done@48
.sx_neg@47:
   LDA   #$FF
.sx_done@48:
   LDY   #$1F
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$1D
   LDA   (FP),Y
   LDY   #$24
   STA   (FP),Y
   LDY   #$1E
   LDA   (FP),Y
   LDY   #$25
   STA   (FP),Y
   LDY   #$1F
   LDA   (FP),Y
   LDY   #$26
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
   LDY   #$24
   LDA   (FP),Y
   LDY   #$2B
   STA   (FP),Y
   LDY   #$25
   LDA   (FP),Y
   LDY   #$2C
   STA   (FP),Y
   LDY   #$26
   LDA   (FP),Y
   LDY   #$2D
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$2A
   LDA   (FP),Y
   LDY   #$31
   STA   (FP),Y
   LDA   g0
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$17
   LDA   (FP),Y
   LDY   #$34
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$18
   LDA   (FP),Y
   LDY   #$37
   STA   (FP),Y
   LDY   #$19
   LDA   (FP),Y
   LDY   #$38
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
   LDY   #$36
   LDA   (FP),Y
   LDY   #$3D
   STA   (FP),Y
   LDY   #$37
   LDA   (FP),Y
   LDY   #$3E
   STA   (FP),Y
   LDY   #$38
   LDA   (FP),Y
   LDY   #$3F
   STA   (FP),Y
   LDY   #$39
   LDA   (FP),Y
   LDY   #$40
   STA   (FP),Y
   LDY   #$35
   LDA   (FP),Y
   LDY   #$41
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ORA   #$01
   LDY   #$43
   STA   (FP),Y
   DEY
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
   ORA   #$00
   BMI   .sx_neg@49
   LDA   #$00
   JMP   .sx_done@50
.sx_neg@49:
   LDA   #$FF
.sx_done@50:
   LDY   #$47
   STA   (FP),Y
   INY
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
   LDY   #$45
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
   LDY   #$49
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
   LDY   #$49
   LDA   (FP),Y
   LDY   #$4D
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
   LDA   g0+3
   BMI   .sx_neg@51
   LDA   #$00
   JMP   .sx_done@52
.sx_neg@51:
   LDA   #$FF
.sx_done@52:
   LDY   #$52
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$4E
   LDA   (FP),Y
   LDY   #$55
   STA   (FP),Y
   LDY   #$4F
   LDA   (FP),Y
   LDY   #$56
   STA   (FP),Y
   LDY   #$50
   LDA   (FP),Y
   LDY   #$57
   STA   (FP),Y
   LDY   #$51
   LDA   (FP),Y
   LDY   #$58
   STA   (FP),Y
   LDY   #$52
   LDA   (FP),Y
   LDY   #$59
   STA   (FP),Y
   INY
   STA   (FP),Y
   AND   #$7F
   INY
   STA   (FP),Y
   LDY   #$55
   LDA   (FP),Y
   SEC
   SBC   g1
   INY
   LDA   (FP),Y
   SBC   g1+1
   INY
   LDA   (FP),Y
   SBC   g1+2
   INY
   LDA   (FP),Y
   SBC   g1+3
   INY
   LDA   (FP),Y
   SBC   g1+4
   INY
   LDA   (FP),Y
   SBC   g1+5
   INY
   LDA   (FP),Y
   SBC   g1+6
   LDA   #$00
   ROL
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$5E
   STA   (FP),Y
   LDY   #$35
   LDA   (FP),Y
   LDY   #$5F
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   SEC
   DEY
   SBC   (FP),Y
   LDA   #$00
   INY
   SBC   (FP),Y
   BVC   .cmp_novf@53
   EOR   #$80
.cmp_novf@53:
   ASL
   LDA   #$00
   ROL
   EOR   #$01
   LDY   #$61
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$08
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
   LDY   #$4D
   LDA   (FP),Y
   LDY   #$05
   STA   (SSP),Y
   LDY   #$5E
   LDA   (FP),Y
   LDY   #$06
   STA   (SSP),Y
   LDY   #$61
   LDA   (FP),Y
   LDY   #$07
   STA   (SSP),Y
   LDY   #$62
   LDA   (FP),Y
   LDY   #$08
   STA   (SSP),Y
   JSR   f5
   LDA   #$00
   ROL
   LDY   #$63
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
   LDY   #$17
   LDA   (FP),Y
   LDY   #$64
   EOR   (FP),Y
   LDY   #$68
   STA   (FP),Y
   LDY   #$18
   LDA   (FP),Y
   LDY   #$65
   EOR   (FP),Y
   LDY   #$69
   STA   (FP),Y
   LDY   #$19
   LDA   (FP),Y
   LDY   #$66
   EOR   (FP),Y
   LDY   #$6A
   STA   (FP),Y
   LDY   #$1A
   LDA   (FP),Y
   LDY   #$67
   EOR   (FP),Y
   LDY   #$6B
   STA   (FP),Y
   LDY   #$68
   LDA   (FP),Y
   LDY   #$17
   STA   (FP),Y
   LDY   #$69
   LDA   (FP),Y
   LDY   #$18
   STA   (FP),Y
   LDY   #$6A
   LDA   (FP),Y
   LDY   #$19
   STA   (FP),Y
   LDY   #$6B
   LDA   (FP),Y
   LDY   #$1A
   STA   (FP),Y
   LDY   #$17
   LDA   (FP),Y
   LDY   #$6C
   STA   (FP),Y
   LDY   #$18
   LDA   (FP),Y
   LDY   #$6D
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$6E
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$6F
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
.loop@0_start:
   LDY   #$70
   LDA   (FP),Y
   SEC
   SBC   #$03
   INY
   LDA   (FP),Y
   SBC   #$00
   BVC   .cmp_novf@54
   EOR   #$80
.cmp_novf@54:
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
   INY
   ORA   (FP),Y
   BNE   .lb_skip@0
   JMP   .loop@0_break
.lb_skip@0:
   LDY   #$2B
   LDA   (FP),Y
   LDY   #$74
   STA   (FP),Y
   LDY   #$2C
   LDA   (FP),Y
   LDY   #$75
   STA   (FP),Y
   LDA   #$02
   EOR   #$FF
   INY
   STA   (FP),Y
   LDA   #$00
   EOR   #$FF
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$78
   STA   (FP),Y
   LDY   #$2B
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
   BEQ   .and_false@9
   LDA   g0
   ORA   g0+1
   ORA   g0+2
   ORA   g0+3
   BEQ   .and_false@9
   LDA   #$01
   LDY   #$79
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   JMP   .and_end@10
.and_false@9:
   LDA   #$00
   LDY   #$79
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@10:
   LDY   #$79
   LDA   (FP),Y
   LDY   #$7B
   STA   (FP),Y
   CLC
   LDA   FP
   ADC   #$6E
   INY
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$06
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDY   #$74
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$75
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$78
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$7B
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   LDY   #$7C
   LDA   (FP),Y
   LDY   #$05
   STA   (SSP),Y
   LDY   #$7D
   LDA   (FP),Y
   LDY   #$06
   STA   (SSP),Y
   JSR   f4
   LDA   HARGS+8
   LDY   #$7E
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
   LDY   #$70
   LDA   (FP),Y
   LDY   #$82
   STA   (FP),Y
   LDY   #$71
   LDA   (FP),Y
   LDY   #$83
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@55
   LDA   #$00
   JMP   .sx_done@56
.sx_neg@55:
   LDA   #$FF
.sx_done@56:
   LDY   #$84
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$7E
   LDA   (FP),Y
   CLC
   LDY   #$82
   ADC   (FP),Y
   LDY   #$86
   STA   (FP),Y
   LDY   #$7F
   LDA   (FP),Y
   LDY   #$83
   ADC   (FP),Y
   LDY   #$87
   STA   (FP),Y
   LDY   #$80
   LDA   (FP),Y
   LDY   #$84
   ADC   (FP),Y
   LDY   #$88
   STA   (FP),Y
   LDY   #$81
   LDA   (FP),Y
   LDY   #$85
   ADC   (FP),Y
   LDY   #$89
   STA   (FP),Y
   LDY   #$33
   LDA   (FP),Y
   LDY   #$8A
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$8A
   LDA   (FP),Y
   CLC
   LDY   #$86
   ADC   (FP),Y
   LDY   #$8E
   STA   (FP),Y
   LDY   #$8B
   LDA   (FP),Y
   LDY   #$87
   ADC   (FP),Y
   LDY   #$8F
   STA   (FP),Y
   LDY   #$8C
   LDA   (FP),Y
   LDY   #$88
   ADC   (FP),Y
   LDY   #$90
   STA   (FP),Y
   LDY   #$8D
   LDA   (FP),Y
   LDY   #$89
   ADC   (FP),Y
   LDY   #$91
   STA   (FP),Y
   LDY   #$8E
   LDA   (FP),Y
   LDY   #$92
   STA   (FP),Y
   LDY   #$33
   STA   (FP),Y
   LDY   #$70
   LDA   (FP),Y
   LDY   #$93
   STA   (FP),Y
   LDY   #$71
   LDA   (FP),Y
   LDY   #$94
   STA   (FP),Y
   LDY   #$70
   LDA   (FP),Y
   CLC
   ADC   #$01
   LDY   #$95
   STA   (FP),Y
   LDY   #$71
   LDA   (FP),Y
   ADC   #$00
   LDY   #$96
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$70
   STA   (FP),Y
   LDY   #$96
   LDA   (FP),Y
   LDY   #$71
   STA   (FP),Y
   JMP   .loop@0_start
.loop@0_break:
   LDY   #$33
   LDA   (FP),Y
   LDY   #$97
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   LDY   #$6E
   ADC   (FP),Y
   LDY   #$99
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$6F
   ADC   (FP),Y
   LDY   #$9A
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$9B
   STA   (FP),Y
   LDY   #$33
   STA   (FP),Y
   LDY   #$2B
   LDA   (FP),Y
   LDY   #$9C
   STA   (FP),Y

   ; epilogue
   PHA
   CLC
   LDA   FP
   ADC   #$9E
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

   ; prologue: 11 arg bytes, 115 local bytes
   SEC
   LDA   SSP
   SBC   #$75
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$74
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   LDY   #$77
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   LDY   #$78
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
   INY
   STA   (FP),Y
   LDY   #$01
   LDA   (FP),Y
   LDY   #$08
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   LDY   #$09
   STA   (FP),Y
   LDY   #$03
   LDA   (FP),Y
   LDY   #$0A
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$77
   LDA   (FP),Y
   ORA   #$01
   LDY   #$0F
   STA   (FP),Y
   LDY   #$78
   LDA   (FP),Y
   LDY   #$10
   STA   (FP),Y
   LDA   #$02
   STA   HARGS
   LDA   #$00
   STA   HARGS+1
   DEY
   LDA   (FP),Y
   STA   HARGS+2
   INY
   LDA   (FP),Y
   STA   HARGS+3
   JSR   udivmod16
   LDA   HARGS+4
   LDY   #$11
   STA   (FP),Y
   LDA   HARGS+5
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
   LDA   g1
   SEC
   LDY   #$13
   SBC   (FP),Y
   LDA   g1+1
   INY
   SBC   (FP),Y
   LDA   g1+2
   INY
   SBC   (FP),Y
   LDA   g1+3
   INY
   SBC   (FP),Y
   LDA   g1+4
   INY
   SBC   (FP),Y
   LDA   g1+5
   INY
   SBC   (FP),Y
   LDA   g1+6
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
   LDY   #$1C
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$7F
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$1E
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$1F
   STA   (FP),Y
   LDY   #$79
   LDA   (FP),Y
   SEC
   SBC   #$07
   INY
   LDA   (FP),Y
   SBC   #$00
   BVC   .cmp_novf@57
   EOR   #$80
.cmp_novf@57:
   ASL
   LDA   #$00
   ROL
   LDY   #$20
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$1E
   LDA   (FP),Y
   CLC
   LDY   #$20
   ADC   (FP),Y
   LDY   #$22
   STA   (FP),Y
   LDY   #$1F
   LDA   (FP),Y
   LDY   #$21
   ADC   (FP),Y
   LDY   #$23
   STA   (FP),Y
   LDY   #$7F
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$22
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$23
   LDA   (FP),Y
   LDY   #$01
   STA   (DPTR),Y
   LDA   #$07
   LDY   #$24
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   ORA   #$00
   BPL   .sx_done@59
   LDA   #$FF
.sx_done@59:
   LDY   #$26
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   g0
   LDY   #$24
   AND   (FP),Y
   LDY   #$28
   STA   (FP),Y
   LDA   g0+1
   LDY   #$25
   AND   (FP),Y
   LDY   #$29
   STA   (FP),Y
   LDA   g0+2
   LDY   #$26
   AND   (FP),Y
   LDY   #$2A
   STA   (FP),Y
   LDA   g0+3
   LDY   #$27
   AND   (FP),Y
   LDY   #$2B
   STA   (FP),Y
   LDA   #$03
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$28
   LDA   (FP),Y
   TAX
   BEQ   .shift_done@61
.shift_loop@60:
   LDY   #$2C
   LDA   (FP),Y
   ASL
   STA   (FP),Y
   INY
   LDA   (FP),Y
   ROL
   STA   (FP),Y
   DEX
   BNE   .shift_loop@60
.shift_done@61:
   LDY   #$2C
   LDA   (FP),Y
   LDY   #$2E
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$2F
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@62
   LDA   #$00
   JMP   .sx_done@63
.sx_neg@62:
   LDA   #$FF
.sx_done@63:
   LDY   #$30
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$79
   LDA   (FP),Y
   LDY   #$32
   STA   (FP),Y
   LDY   #$7A
   LDA   (FP),Y
   LDY   #$33
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@64
   LDA   #$00
   JMP   .sx_done@65
.sx_neg@64:
   LDA   #$FF
.sx_done@65:
   LDY   #$34
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   g0+3
   CMP   (FP),Y
   BNE   .thread@2
   LDA   g0+2
   DEY
   CMP   (FP),Y
   BNE   .thread@2
   LDA   g0+1
   DEY
   CMP   (FP),Y
   BNE   .thread@2
   LDA   g0
   DEY
   CMP   (FP),Y
   BEQ   .cmp_true@66
.thread@2:
   LDA   #$00
   JMP   .cmp_end@67
.cmp_true@66:
   LDA   #$01
.cmp_end@67:
   LDY   #$36
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$38
   STA   (FP),Y
   LDY   #$7B
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   INY
   ORA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@11
   LDY   #$7B
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   INY
   ORA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@11
   LDA   #$00
   LDY   #$39
   STA   (FP),Y
   INY
   STA   (FP),Y
   JMP   .or_end@12
.or_true@11:
   LDA   #$01
   LDY   #$39
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@12:
   LDY   #$39
   LDA   (FP),Y
   LDY   #$3B
   STA   (FP),Y
   LDY   #$7F
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$3C
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$3D
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$08
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDY   #$2E
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$2F
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$30
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$31
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   LDY   #$38
   LDA   (FP),Y
   LDY   #$05
   STA   (SSP),Y
   LDY   #$3B
   LDA   (FP),Y
   LDY   #$06
   STA   (SSP),Y
   LDY   #$3C
   LDA   (FP),Y
   LDY   #$07
   STA   (SSP),Y
   LDY   #$3D
   LDA   (FP),Y
   LDY   #$08
   STA   (SSP),Y
   JSR   f5
   LDA   #$00
   ROL
   LDY   #$3E
   STA   (FP),Y
   ORA   #$00
   BNE   .lb_skip@1
   JMP   .if_else@14
.lb_skip@1:
   LDY   #$08
   LDA   (FP),Y
   CLC
   ADC   #$03
   LDY   #$3F
   STA   (FP),Y
   LDY   #$09
   LDA   (FP),Y
   ADC   #$00
   LDY   #$40
   STA   (FP),Y
   LDY   #$0A
   LDA   (FP),Y
   ADC   #$00
   LDY   #$41
   STA   (FP),Y
   LDY   #$0B
   LDA   (FP),Y
   ADC   #$00
   LDY   #$42
   STA   (FP),Y
   LDY   #$0C
   LDA   (FP),Y
   ADC   #$00
   LDY   #$43
   STA   (FP),Y
   LDY   #$0D
   LDA   (FP),Y
   ADC   #$00
   LDY   #$44
   STA   (FP),Y
   LDY   #$0E
   LDA   (FP),Y
   ADC   #$00
   LDY   #$45
   STA   (FP),Y
   LDY   #$3F
   LDA   (FP),Y
   LDY   #$46
   STA   (FP),Y
   LDY   #$40
   LDA   (FP),Y
   LDY   #$47
   STA   (FP),Y
   LDY   #$41
   LDA   (FP),Y
   LDY   #$48
   STA   (FP),Y
   LDY   #$42
   LDA   (FP),Y
   LDY   #$49
   STA   (FP),Y
   LDY   #$43
   LDA   (FP),Y
   LDY   #$4A
   STA   (FP),Y
   LDY   #$44
   LDA   (FP),Y
   LDY   #$4B
   STA   (FP),Y
   LDY   #$45
   LDA   (FP),Y
   AND   #$07
   LDY   #$4C
   STA   (FP),Y
   LDY   #$46
   LDA   (FP),Y
   LDY   #$08
   STA   (FP),Y
   LDY   #$47
   LDA   (FP),Y
   LDY   #$09
   STA   (FP),Y
   LDY   #$48
   LDA   (FP),Y
   LDY   #$0A
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
   JMP   .if_end@13
.if_else@14:
   LDY   #$08
   LDA   (FP),Y
   EOR   #$05
   LDY   #$4D
   STA   (FP),Y
   LDY   #$09
   LDA   (FP),Y
   LDY   #$4E
   STA   (FP),Y
   LDY   #$0A
   LDA   (FP),Y
   LDY   #$4F
   STA   (FP),Y
   LDY   #$0B
   LDA   (FP),Y
   LDY   #$50
   STA   (FP),Y
   LDY   #$0C
   LDA   (FP),Y
   LDY   #$51
   STA   (FP),Y
   LDY   #$0D
   LDA   (FP),Y
   LDY   #$52
   STA   (FP),Y
   LDY   #$0E
   LDA   (FP),Y
   LDY   #$53
   STA   (FP),Y
   LDY   #$4D
   LDA   (FP),Y
   LDY   #$08
   STA   (FP),Y
   LDY   #$4E
   LDA   (FP),Y
   LDY   #$09
   STA   (FP),Y
   LDY   #$4F
   LDA   (FP),Y
   LDY   #$0A
   STA   (FP),Y
   LDY   #$50
   LDA   (FP),Y
   LDY   #$0B
   STA   (FP),Y
   LDY   #$51
   LDA   (FP),Y
   LDY   #$0C
   STA   (FP),Y
   LDY   #$52
   LDA   (FP),Y
   LDY   #$0D
   STA   (FP),Y
   LDY   #$53
   LDA   (FP),Y
   LDY   #$0E
   STA   (FP),Y
.if_end@13:
   LDA   g1
   ORA   #$01
   LDY   #$54
   STA   (FP),Y
   LDA   g1+1
   INY
   STA   (FP),Y
   LDA   g1+2
   INY
   STA   (FP),Y
   LDA   g1+3
   INY
   STA   (FP),Y
   LDA   g1+4
   INY
   STA   (FP),Y
   LDA   g1+5
   INY
   STA   (FP),Y
   LDA   g1+6
   INY
   STA   (FP),Y
   LDY   #$08
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
   LDY   #$54
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
   JSR   udivmod64
   LDA   HARGS+16
   LDY   #$5B
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
   LDY   #$5B
   LDA   (FP),Y
   LDY   #$62
   STA   (FP),Y
   LDY   #$5C
   LDA   (FP),Y
   LDY   #$63
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
   LDY   #$62
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$63
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   JSR   rec
   LDY   #$64
   STA   (FP),Y
   TXA
   INY
   STA   (FP),Y
   LDY   #$1D
   LDA   (FP),Y
   LDY   #$66
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@69
   LDA   #$00
   JMP   .sx_done@70
.sx_neg@69:
   LDA   #$FF
.sx_done@70:
   LDY   #$67
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   LDY   #$64
   ADC   (FP),Y
   LDY   #$68
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$65
   ADC   (FP),Y
   LDY   #$69
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$6A
   STA   (FP),Y
   LDY   #$1D
   STA   (FP),Y
   LDY   #$79
   LDA   (FP),Y
   LDY   #$6B
   STA   (FP),Y
   LDY   #$7A
   LDA   (FP),Y
   LDY   #$6C
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@71
   LDA   #$00
   JMP   .sx_done@72
.sx_neg@71:
   LDA   #$FF
.sx_done@72:
   LDY   #$6D
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$7B
   LDA   (FP),Y
   LDY   #$6B
   ORA   (FP),Y
   LDY   #$6F
   STA   (FP),Y
   LDY   #$7C
   LDA   (FP),Y
   LDY   #$6C
   ORA   (FP),Y
   LDY   #$70
   STA   (FP),Y
   LDY   #$7D
   LDA   (FP),Y
   LDY   #$6D
   ORA   (FP),Y
   LDY   #$71
   STA   (FP),Y
   LDY   #$7E
   LDA   (FP),Y
   LDY   #$6E
   ORA   (FP),Y
   LDY   #$72
   STA   (FP),Y
   DEY
   ORA   (FP),Y
   DEY
   ORA   (FP),Y
   DEY
   ORA   (FP),Y
   BEQ   .cmp_end@73
   LDA   #$01
.cmp_end@73:
   LDY   #$73
   STA   (FP),Y

   ; epilogue
   PHA
   CLC
   LDA   FP
   ADC   #$80
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

   ; prologue: 6 arg bytes, 95 local bytes
   SEC
   LDA   SSP
   SBC   #$61
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$60
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
   BVC   .cmp_novf@74
   EOR   #$80
.cmp_novf@74:
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
   LDY   #$03
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$04
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@75
   LDA   #$00
   JMP   .sx_done@76
.sx_neg@75:
   LDA   #$FF
.sx_done@76:
   LDY   #$05
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   g0
   AND   g0
   INY
   STA   (FP),Y
   LDA   g0+1
   AND   g0+1
   INY
   STA   (FP),Y
   LDA   g0+2
   AND   g0+2
   INY
   STA   (FP),Y
   LDA   g0+3
   AND   g0+3
   INY
   STA   (FP),Y
   LDY   #$03
   LDA   (FP),Y
   SEC
   LDY   #$07
   SBC   (FP),Y
   LDY   #$04
   LDA   (FP),Y
   LDY   #$08
   SBC   (FP),Y
   LDY   #$05
   LDA   (FP),Y
   LDY   #$09
   SBC   (FP),Y
   LDY   #$06
   LDA   (FP),Y
   LDY   #$0A
   SBC   (FP),Y
   BVC   .cmp_novf@77
   EOR   #$80
.cmp_novf@77:
   ASL
   LDA   #$00
   ROL
   EOR   #$01
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
   DEY
   LDA   (FP),Y
   LDY   #$0F
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$10
   STA   (FP),Y
   LDA   g1
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$62
   LDA   (FP),Y
   LDY   #$13
   STA   (FP),Y
   LDY   #$63
   LDA   (FP),Y
   LDY   #$14
   STA   (FP),Y
   LDY   #$64
   LDA   (FP),Y
   LDY   #$15
   STA   (FP),Y
   LDY   #$65
   LDA   (FP),Y
   LDY   #$16
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@78
   LDA   #$00
   JMP   .sx_done@79
.sx_neg@78:
   LDA   #$FF
.sx_done@79:
   LDY   #$17
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$13
   LDA   (FP),Y
   LDY   #$1A
   STA   (FP),Y
   LDY   #$14
   LDA   (FP),Y
   LDY   #$1B
   STA   (FP),Y
   LDY   #$15
   LDA   (FP),Y
   LDY   #$1C
   STA   (FP),Y
   LDY   #$16
   LDA   (FP),Y
   LDY   #$1D
   STA   (FP),Y
   LDY   #$17
   LDA   (FP),Y
   LDY   #$1E
   STA   (FP),Y
   INY
   STA   (FP),Y
   AND   #$7F
   INY
   STA   (FP),Y
   LDY   #$1A
   LDA   (FP),Y
   SEC
   SBC   g1
   LDY   #$21
   STA   (FP),Y
   LDY   #$1B
   LDA   (FP),Y
   SBC   g1+1
   LDY   #$22
   STA   (FP),Y
   LDY   #$1C
   LDA   (FP),Y
   SBC   g1+2
   LDY   #$23
   STA   (FP),Y
   LDY   #$1D
   LDA   (FP),Y
   SBC   g1+3
   LDY   #$24
   STA   (FP),Y
   LDY   #$1E
   LDA   (FP),Y
   SBC   g1+4
   LDY   #$25
   STA   (FP),Y
   LDY   #$1F
   LDA   (FP),Y
   SBC   g1+5
   LDY   #$26
   STA   (FP),Y
   LDY   #$20
   LDA   (FP),Y
   SBC   g1+6
   LDY   #$27
   STA   (FP),Y
   LDY   #$21
   LDA   (FP),Y
   LDY   #$28
   STA   (FP),Y
   LDY   #$22
   LDA   (FP),Y
   LDY   #$29
   STA   (FP),Y
   LDY   #$23
   LDA   (FP),Y
   LDY   #$2A
   STA   (FP),Y
   LDY   #$24
   LDA   (FP),Y
   LDY   #$2B
   STA   (FP),Y
   LDY   #$25
   LDA   (FP),Y
   LDY   #$2C
   STA   (FP),Y
   LDY   #$26
   LDA   (FP),Y
   LDY   #$2D
   STA   (FP),Y
   LDY   #$27
   LDA   (FP),Y
   AND   #$7F
   LDY   #$2E
   STA   (FP),Y
   LDY   #$28
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
   BEQ   .lb_skip@2
   JMP   .or_true@15
.lb_skip@2:
   LDY   #$12
   LDA   (FP),Y
   LDY   #$2F
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@80
   LDA   #$00
   JMP   .sx_done@81
.sx_neg@80:
   LDA   #$FF
.sx_done@81:
   LDY   #$30
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$31
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$32
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@82
   LDA   #$00
   JMP   .sx_done@83
.sx_neg@82:
   LDA   #$FF
.sx_done@83:
   LDY   #$33
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$31
   LDA   (FP),Y
   LDY   #$62
   EOR   (FP),Y
   LDY   #$35
   STA   (FP),Y
   LDY   #$32
   LDA   (FP),Y
   LDY   #$63
   EOR   (FP),Y
   LDY   #$36
   STA   (FP),Y
   LDY   #$33
   LDA   (FP),Y
   LDY   #$64
   EOR   (FP),Y
   LDY   #$37
   STA   (FP),Y
   LDY   #$34
   LDA   (FP),Y
   LDY   #$65
   EOR   (FP),Y
   LDY   #$38
   STA   (FP),Y
   LDY   #$35
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   INY
   ORA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@15
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   JMP   .or_end@16
.or_true@15:
   LDA   #$01
   LDY   #$39
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@16:
   LDY   #$39
   LDA   (FP),Y
   LDY   #$3B
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$3C
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@84
   LDA   #$00
   JMP   .sx_done@85
.sx_neg@84:
   LDA   #$FF
.sx_done@85:
   LDY   #$3D
   STA   (FP),Y
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
   LDY   #$41
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$66
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
   LDA   g0+3
   BMI   .sx_neg@86
   LDA   #$00
   JMP   .sx_done@87
.sx_neg@86:
   LDA   #$FF
.sx_done@87:
   LDY   #$49
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$45
   LDA   (FP),Y
   LDY   #$4C
   STA   (FP),Y
   LDY   #$46
   LDA   (FP),Y
   LDY   #$4D
   STA   (FP),Y
   LDY   #$47
   LDA   (FP),Y
   LDY   #$4E
   STA   (FP),Y
   LDY   #$48
   LDA   (FP),Y
   LDY   #$4F
   STA   (FP),Y
   LDY   #$49
   LDA   (FP),Y
   LDY   #$50
   STA   (FP),Y
   INY
   STA   (FP),Y
   AND   #$7F
   INY
   STA   (FP),Y
   CMP   g1+6
   BNE   .cmp_true@88
   DEY
   LDA   (FP),Y
   CMP   g1+5
   BNE   .cmp_true@88
   CMP   g1+4
   BNE   .cmp_true@88
   LDY   #$4F
   LDA   (FP),Y
   CMP   g1+3
   BNE   .cmp_true@88
   DEY
   LDA   (FP),Y
   CMP   g1+2
   BNE   .cmp_true@88
   DEY
   LDA   (FP),Y
   CMP   g1+1
   BNE   .cmp_true@88
   DEY
   LDA   (FP),Y
   CMP   g1
   BNE   .cmp_true@88
   LDA   #$00
   JMP   .cmp_end@89
.cmp_true@88:
   LDA   #$01
.cmp_end@89:
   LDY   #$53
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$43
   LDA   (FP),Y
   CLC
   LDY   #$53
   ADC   (FP),Y
   LDY   #$55
   STA   (FP),Y
   LDY   #$44
   LDA   (FP),Y
   LDY   #$54
   ADC   (FP),Y
   LDY   #$56
   STA   (FP),Y
   LDY   #$66
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$55
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$56
   LDA   (FP),Y
   LDY   #$01
   STA   (DPTR),Y
   LDA   g0
   SEC
   SBC   g0
   LDA   g0+1
   SBC   g0+1
   LDA   g0+2
   SBC   g0+2
   LDA   g0+3
   SBC   g0+3
   BVC   .cmp_novf@91
   EOR   #$80
.cmp_novf@91:
   ASL
   LDA   #$00
   ROL
   EOR   #$01
   LDY   #$57
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   CLC
   SBC   g1
   LDA   #$00
   SBC   g1+1
   LDA   #$00
   SBC   g1+2
   LDA   #$00
   SBC   g1+3
   LDA   #$00
   SBC   g1+4
   LDA   #$00
   SBC   g1+5
   LDA   #$00
   SBC   g1+6
   LDA   #$00
   ROL
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   AND   #$07
   LDY   #$5B
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDA   #$00
   LDY   #$5C
   STA   (FP),Y
   LDY   #$57
   LDA   (FP),Y
   LDY   #$5D
   STA   (FP),Y
   LDY   #$58
   LDA   (FP),Y
   LDY   #$5E
   STA   (FP),Y
   LDY   #$5B
   LDA   (FP),Y
   TAX
   BEQ   .shift_done@93
.shift_loop@92:
   LDY   #$5D
   LDA   (FP),Y
   ASL
   STA   (FP),Y
   INY
   LDA   (FP),Y
   ROL
   STA   (FP),Y
   DEX
   BNE   .shift_loop@92
.shift_done@93:
   LDY   #$5D
   LDA   (FP),Y
   LDY   #$5F
   STA   (FP),Y

   ; epilogue
   PHA
   CLC
   LDA   FP
   ADC   #$67
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

   ; prologue: 0 arg bytes, 39 local bytes
   SEC
   LDA   SSP
   SBC   #$29
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$28
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
   LDA   #$AA
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   ORA   #$00
   BPL   .sx_done@95
   LDA   #$FF
.sx_done@95:
   LDY   #$05
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
   SBC   #$06
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
   JSR   f0
   LDY   #$09
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
   INY
   STA   (FP),Y
   LDA   g1
   INY
   STA   (FP),Y
   LDA   g1+1
   INY
   STA   (FP),Y
   LDA   g1+2
   INY
   STA   (FP),Y
   LDA   g1+3
   INY
   STA   (FP),Y
   LDA   g0
   LDY   #$12
   EOR   (FP),Y
   LDY   #$16
   STA   (FP),Y
   LDA   g0+1
   LDY   #$13
   EOR   (FP),Y
   LDY   #$17
   STA   (FP),Y
   LDA   g0+2
   LDY   #$14
   EOR   (FP),Y
   LDY   #$18
   STA   (FP),Y
   LDA   g0+3
   LDY   #$15
   EOR   (FP),Y
   LDY   #$19
   STA   (FP),Y
   LDY   #$0E
   LDA   (FP),Y
   LDY   #$16
   EOR   (FP),Y
   LDY   #$1A
   STA   (FP),Y
   LDY   #$0F
   LDA   (FP),Y
   LDY   #$17
   EOR   (FP),Y
   LDY   #$1B
   STA   (FP),Y
   LDY   #$10
   LDA   (FP),Y
   LDY   #$18
   EOR   (FP),Y
   LDY   #$1C
   STA   (FP),Y
   LDY   #$11
   LDA   (FP),Y
   LDY   #$19
   EOR   (FP),Y
   LDY   #$1D
   STA   (FP),Y
   LDY   #$1A
   LDA   (FP),Y
   LDY   #$0E
   STA   (FP),Y
   LDY   #$1B
   LDA   (FP),Y
   LDY   #$0F
   STA   (FP),Y
   LDY   #$1C
   LDA   (FP),Y
   LDY   #$10
   STA   (FP),Y
   LDY   #$1D
   LDA   (FP),Y
   LDY   #$11
   STA   (FP),Y
   LDY   #$0E
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
   LDY   #$0E
   LDA   (FP),Y
   LDY   #$1E
   EOR   (FP),Y
   LDY   #$22
   STA   (FP),Y
   LDY   #$0F
   LDA   (FP),Y
   LDY   #$1F
   EOR   (FP),Y
   LDY   #$23
   STA   (FP),Y
   LDY   #$10
   LDA   (FP),Y
   LDY   #$20
   EOR   (FP),Y
   LDY   #$24
   STA   (FP),Y
   LDY   #$11
   LDA   (FP),Y
   LDY   #$21
   EOR   (FP),Y
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
   TAX
   DEY
   LDA   (FP),Y

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$29
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$28
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
   DC.L  $00000040

g1:
   DC.B  $A8
   DS.B  6
