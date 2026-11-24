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

f5:
   SUBROUTINE

   ; prologue: 12 arg bytes, 35 local bytes
   SEC
   LDA   SSP
   SBC   #$25
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$24
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   LDA   #$64
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
   LDA   #$FF
   STA   HARGS
   LDA   #$7F
   STA   HARGS+1
   LDY   #$2E
   LDA   (FP),Y
   STA   HARGS+2
   INY
   LDA   (FP),Y
   STA   HARGS+3
   JSR   mul16
   LDA   HARGS+4
   LDY   #$05
   STA   (FP),Y
   LDA   HARGS+5
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ORA   #$01
   LDY   #$07
   STA   (FP),Y
   DEY
   LDA   (FP),Y
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
   JSR   udivmod16
   LDA   HARGS+4
   LDY   #$09
   STA   (FP),Y
   LDA   HARGS+5
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
   LDY   #$30
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$0F
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$10
   STA   (FP),Y
   LDY   #$0D
   LDA   (FP),Y
   ORA   (FP),Y
   LDY   #$11
   STA   (FP),Y
   LDY   #$0E
   LDA   (FP),Y
   ORA   (FP),Y
   LDY   #$12
   STA   (FP),Y
   LDY   #$0F
   LDA   (FP),Y
   CLC
   LDY   #$11
   ADC   (FP),Y
   LDY   #$13
   STA   (FP),Y
   LDY   #$10
   LDA   (FP),Y
   LDY   #$12
   ADC   (FP),Y
   LDY   #$14
   STA   (FP),Y
   LDY   #$30
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$13
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$14
   LDA   (FP),Y
   LDY   #$01
   STA   (DPTR),Y
   LDY   #$30
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$15
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$16
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$17
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$18
   STA   (FP),Y
   LDY   #$2E
   LDA   (FP),Y
   LDY   #$17
   EOR   (FP),Y
   LDY   #$19
   STA   (FP),Y
   LDY   #$2F
   LDA   (FP),Y
   LDY   #$18
   EOR   (FP),Y
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
   LDY   #$1B
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$1C
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   JSR   rec
   LDY   #$1D
   STA   (FP),Y
   TXA
   INY
   STA   (FP),Y
   LDY   #$0D
   LDA   (FP),Y
   CLC
   LDY   #$1D
   ADC   (FP),Y
   LDY   #$1F
   STA   (FP),Y
   LDY   #$0E
   LDA   (FP),Y
   LDY   #$1E
   ADC   (FP),Y
   LDY   #$20
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$0D
   STA   (FP),Y
   LDY   #$20
   LDA   (FP),Y
   LDY   #$0E
   STA   (FP),Y
   LDY   #$2F
   LDA   (FP),Y
   BNE   .cmp_true@7
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@7
   LDA   #$00
   JMP   .cmp_end@8
.cmp_true@7:
   LDA   #$01
.cmp_end@8:
   LDY   #$21
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$23
   STA   (FP),Y
   CMP   #$01

   ; epilogue
   PHP
   CLC
   LDA   FP
   ADC   #$31
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

   ; prologue: 5 arg bytes, 80 local bytes
   SEC
   LDA   SSP
   SBC   #$52
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$51
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
   BMI   .sx_neg@10
   LDA   #$00
   JMP   .sx_done@11
.sx_neg@10:
   LDA   #$FF
.sx_done@11:
   LDY   #$02
   STA   (FP),Y
   INY
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
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$54
   LDA   (FP),Y
   LDY   #$09
   STA   (FP),Y
   LDY   #$55
   LDA   (FP),Y
   LDY   #$0A
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   EOR   #$FF
   LDY   #$0B
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   EOR   #$FF
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
   LDY   #$05
   LDA   (FP),Y
   LDY   #$0F
   STA   (FP),Y
   LDY   #$06
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
   LDY   #$53
   LDA   (FP),Y
   LDY   #$13
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@12
   LDA   #$00
   JMP   .sx_done@13
.sx_neg@12:
   LDA   #$FF
.sx_done@13:
   LDY   #$14
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
   JSR   sdivmod16
   LDA   HARGS+6
   LDY   #$17
   STA   (FP),Y
   LDA   HARGS+7
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
   BMI   .sx_neg@14
   LDA   #$00
   JMP   .sx_done@15
.sx_neg@14:
   LDA   #$FF
.sx_done@15:
   LDY   #$1B
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$05
   LDA   (FP),Y
   LDY   #$1D
   STA   (FP),Y
   LDY   #$06
   LDA   (FP),Y
   LDY   #$1E
   STA   (FP),Y
   LDY   #$07
   LDA   (FP),Y
   LDY   #$1F
   STA   (FP),Y
   LDY   #$08
   LDA   (FP),Y
   LDY   #$20
   STA   (FP),Y
   LDY   #$1D
   LDA   (FP),Y
   SEC
   LDY   #$54
   SBC   (FP),Y
   LDY   #$21
   STA   (FP),Y
   LDY   #$1E
   LDA   (FP),Y
   LDY   #$55
   SBC   (FP),Y
   LDY   #$22
   STA   (FP),Y
   LDY   #$1F
   LDA   (FP),Y
   LDY   #$56
   SBC   (FP),Y
   LDY   #$23
   STA   (FP),Y
   LDY   #$20
   LDA   (FP),Y
   LDY   #$57
   SBC   (FP),Y
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
   LDY   #$05
   LDA   (FP),Y
   LDY   #$27
   STA   (FP),Y
   LDY   #$06
   LDA   (FP),Y
   LDY   #$28
   STA   (FP),Y
   LDA   #$07
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   ORA   #$00
   BPL   .sx_done@17
   LDA   #$FF
.sx_done@17:
   LDY   #$2B
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$05
   LDA   (FP),Y
   LDY   #$29
   AND   (FP),Y
   LDY   #$2D
   STA   (FP),Y
   LDY   #$06
   LDA   (FP),Y
   LDY   #$2A
   AND   (FP),Y
   LDY   #$2E
   STA   (FP),Y
   LDY   #$07
   LDA   (FP),Y
   LDY   #$2B
   AND   (FP),Y
   LDY   #$2F
   STA   (FP),Y
   LDY   #$08
   LDA   (FP),Y
   LDY   #$2C
   AND   (FP),Y
   LDY   #$30
   STA   (FP),Y
   LDA   #$FF
   INY
   STA   (FP),Y
   LDA   #$7F
   INY
   STA   (FP),Y
   LDY   #$2D
   LDA   (FP),Y
   TAX
   CPX   #$00
   BEQ   .shift_done@19
.shift_loop@18:
   LDY   #$32
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
   BNE   .shift_loop@18
.shift_done@19:
   LDY   #$31
   LDA   (FP),Y
   LDY   #$33
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$34
   STA   (FP),Y
   CLC
   LDA   FP
   ADC   #$11
   INY
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$0C
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
   LDY   #$1B
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$1C
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   LDY   #$25
   LDA   (FP),Y
   LDY   #$05
   STA   (SSP),Y
   LDY   #$26
   LDA   (FP),Y
   LDY   #$06
   STA   (SSP),Y
   LDY   #$27
   LDA   (FP),Y
   LDY   #$07
   STA   (SSP),Y
   LDY   #$28
   LDA   (FP),Y
   LDY   #$08
   STA   (SSP),Y
   LDY   #$33
   LDA   (FP),Y
   LDY   #$09
   STA   (SSP),Y
   LDY   #$34
   LDA   (FP),Y
   LDY   #$0A
   STA   (SSP),Y
   LDY   #$35
   LDA   (FP),Y
   LDY   #$0B
   STA   (SSP),Y
   LDY   #$36
   LDA   (FP),Y
   LDY   #$0C
   STA   (SSP),Y
   JSR   f5
   LDA   #$00
   ROL
   LDY   #$37
   STA   (FP),Y
   ORA   #$00
   BEQ   .if_else@2
   LDA   #$03
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   ORA   #$00
   BPL   .sx_done@21
   LDA   #$FF
.sx_done@21:
   LDY   #$3A
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$05
   LDA   (FP),Y
   CLC
   LDY   #$38
   ADC   (FP),Y
   LDY   #$3C
   STA   (FP),Y
   LDY   #$06
   LDA   (FP),Y
   LDY   #$39
   ADC   (FP),Y
   LDY   #$3D
   STA   (FP),Y
   LDY   #$07
   LDA   (FP),Y
   LDY   #$3A
   ADC   (FP),Y
   LDY   #$3E
   STA   (FP),Y
   LDY   #$08
   LDA   (FP),Y
   LDY   #$3B
   ADC   (FP),Y
   LDY   #$3F
   STA   (FP),Y
   LDY   #$3C
   LDA   (FP),Y
   LDY   #$05
   STA   (FP),Y
   LDY   #$3D
   LDA   (FP),Y
   LDY   #$06
   STA   (FP),Y
   LDY   #$3E
   LDA   (FP),Y
   LDY   #$07
   STA   (FP),Y
   LDY   #$3F
   LDA   (FP),Y
   LDY   #$08
   STA   (FP),Y
   JMP   .if_end@1
.if_else@2:
   LDA   #$05
   LDY   #$40
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   ORA   #$00
   BPL   .sx_done@23
   LDA   #$FF
.sx_done@23:
   LDY   #$42
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$05
   LDA   (FP),Y
   LDY   #$40
   EOR   (FP),Y
   LDY   #$44
   STA   (FP),Y
   LDY   #$06
   LDA   (FP),Y
   LDY   #$41
   EOR   (FP),Y
   LDY   #$45
   STA   (FP),Y
   LDY   #$07
   LDA   (FP),Y
   LDY   #$42
   EOR   (FP),Y
   LDY   #$46
   STA   (FP),Y
   LDY   #$08
   LDA   (FP),Y
   LDY   #$43
   EOR   (FP),Y
   LDY   #$47
   STA   (FP),Y
   LDY   #$44
   LDA   (FP),Y
   LDY   #$05
   STA   (FP),Y
   LDY   #$45
   LDA   (FP),Y
   LDY   #$06
   STA   (FP),Y
   LDY   #$46
   LDA   (FP),Y
   LDY   #$07
   STA   (FP),Y
   LDY   #$47
   LDA   (FP),Y
   LDY   #$08
   STA   (FP),Y
.if_end@1:
   LDY   #$11
   LDA   (FP),Y
   LDY   #$48
   STA   (FP),Y
   LDY   #$12
   LDA   (FP),Y
   LDY   #$49
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@24
   LDA   #$00
   JMP   .sx_done@25
.sx_neg@24:
   LDA   #$FF
.sx_done@25:
   LDY   #$4A
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$05
   LDA   (FP),Y
   CLC
   LDY   #$48
   ADC   (FP),Y
   LDY   #$4C
   STA   (FP),Y
   LDY   #$06
   LDA   (FP),Y
   LDY   #$49
   ADC   (FP),Y
   LDY   #$4D
   STA   (FP),Y
   LDY   #$07
   LDA   (FP),Y
   LDY   #$4A
   ADC   (FP),Y
   LDY   #$4E
   STA   (FP),Y
   LDY   #$08
   LDA   (FP),Y
   LDY   #$4B
   ADC   (FP),Y
   LDY   #$4F
   STA   (FP),Y
   LDY   #$4C
   LDA   (FP),Y
   LDY   #$05
   STA   (FP),Y
   LDY   #$4D
   LDA   (FP),Y
   LDY   #$06
   STA   (FP),Y
   LDY   #$4E
   LDA   (FP),Y
   LDY   #$07
   STA   (FP),Y
   LDY   #$4F
   LDA   (FP),Y
   LDY   #$08
   STA   (FP),Y
   LDY   #$57
   LDA   (FP),Y
   DEY
   ORA   (FP),Y
   DEY
   ORA   (FP),Y
   DEY
   ORA   (FP),Y
   BEQ   .cmp_end@26
   LDA   #$01
.cmp_end@26:
   LDY   #$50
   STA   (FP),Y

   ; epilogue
   PHA
   CLC
   LDA   FP
   ADC   #$57
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

   ; prologue: 2 arg bytes, 73 local bytes
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

   LDA   #$02
   EOR   #$FF
   LDY   #$01
   STA   (FP),Y
   LDA   #$00
   EOR   #$FF
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
   LDA   #$FF
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$03
   LDA   (FP),Y
   TAX
   BEQ   .shift_done@28
.shift_loop@27:
   LDY   #$05
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
   LDY   #$05
   LDA   (FP),Y
   LDY   #$07
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$08
   STA   (FP),Y
   LDA   g0
   INY
   STA   (FP),Y
   AND   #$03
   INY
   STA   (FP),Y
   EOR   #$02
   INY
   STA   (FP),Y
   SEC
   SBC   #$02
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@29
   LDA   #$00
   JMP   .sx_done@30
.sx_neg@29:
   LDA   #$FF
.sx_done@30:
   LDY   #$0F
   STA   (FP),Y
   LDY   #$4D
   CMP   (FP),Y
   BNE   .thread@0
   LDY   #$0E
   LDA   (FP),Y
   LDY   #$4C
   CMP   (FP),Y
   BEQ   .cmp_true@31
.thread@0:
   LDA   #$00
   JMP   .cmp_end@32
.cmp_true@31:
   LDA   #$01
.cmp_end@32:
   LDY   #$10
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$12
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
.loop@0_start:
   LDY   #$14
   LDA   (FP),Y
   SEC
   SBC   #$03
   INY
   LDA   (FP),Y
   SBC   #$00
   BVC   .cmp_novf@34
   EOR   #$80
.cmp_novf@34:
   ASL
   LDA   #$00
   ROL
   LDY   #$16
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
   LDY   #$4C
   LDA   (FP),Y
   LDY   #$18
   STA   (FP),Y
   AND   #$03
   INY
   STA   (FP),Y
   EOR   #$02
   INY
   STA   (FP),Y
   SEC
   SBC   #$02
   INY
   STA   (FP),Y
   LDY   #$13
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
   SEC
   LDA   SSP
   SBC   #$05
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDY   #$1B
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$1C
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$1D
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$1E
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   LDY   #$1F
   LDA   (FP),Y
   LDY   #$05
   STA   (SSP),Y
   JSR   f4
   LDY   #$20
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   LDY   #$14
   ADC   (FP),Y
   LDY   #$23
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$15
   ADC   (FP),Y
   LDY   #$24
   STA   (FP),Y
   LDY   #$13
   LDA   (FP),Y
   LDY   #$25
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   LDY   #$23
   ADC   (FP),Y
   LDY   #$27
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$24
   ADC   (FP),Y
   LDY   #$28
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$29
   STA   (FP),Y
   LDY   #$13
   STA   (FP),Y
   INY
   LDA   (FP),Y
   LDY   #$2A
   STA   (FP),Y
   LDY   #$15
   LDA   (FP),Y
   LDY   #$2B
   STA   (FP),Y
   LDY   #$14
   LDA   (FP),Y
   CLC
   ADC   #$01
   LDY   #$2C
   STA   (FP),Y
   LDY   #$15
   LDA   (FP),Y
   ADC   #$00
   LDY   #$2D
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$14
   STA   (FP),Y
   LDY   #$2D
   LDA   (FP),Y
   LDY   #$15
   STA   (FP),Y
   JMP   .loop@0_start
.loop@0_break:
   LDY   #$13
   LDA   (FP),Y
   LDY   #$2E
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$07
   ORA   (FP),Y
   LDY   #$30
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$08
   ORA   (FP),Y
   LDY   #$31
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
   LDY   #$30
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$31
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   JSR   rec
   LDY   #$32
   STA   (FP),Y
   TXA
   INY
   STA   (FP),Y
   LDY   #$07
   LDA   (FP),Y
   CLC
   LDY   #$32
   ADC   (FP),Y
   LDY   #$34
   STA   (FP),Y
   LDY   #$08
   LDA   (FP),Y
   LDY   #$33
   ADC   (FP),Y
   LDY   #$35
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$07
   STA   (FP),Y
   LDY   #$35
   LDA   (FP),Y
   LDY   #$08
   STA   (FP),Y
   LDA   g0
   LDY   #$36
   STA   (FP),Y
   LDA   g0+1
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$13
   LDA   (FP),Y
   LDY   #$3A
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
   LDA   g0
   CLC
   DEY
   SBC   (FP),Y
   LDA   g0+1
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
   LDY   #$40
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$41
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@35
   LDA   #$00
   JMP   .sx_done@36
.sx_neg@35:
   LDA   #$FF
.sx_done@36:
   LDY   #$42
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$36
   LDA   (FP),Y
   LDY   #$40
   AND   (FP),Y
   LDY   #$44
   STA   (FP),Y
   LDY   #$37
   LDA   (FP),Y
   LDY   #$41
   AND   (FP),Y
   LDY   #$45
   STA   (FP),Y
   LDY   #$38
   LDA   (FP),Y
   LDY   #$42
   AND   (FP),Y
   LDY   #$46
   STA   (FP),Y
   LDY   #$39
   LDA   (FP),Y
   LDY   #$43
   AND   (FP),Y
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
   TAX
   DEY
   LDA   (FP),Y

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$4D
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$4A
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

   ; prologue: 6 arg bytes, 54 local bytes
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

   LDA   #$FF
   LDY   #$01
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   ORA   #$00
   BPL   .sx_done@38
   LDA   #$FF
.sx_done@38:
   LDY   #$03
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   g0
   INY
   STA   (FP),Y
   LDA   g0+1
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$39
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
   JSR   mul32
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
   LDY   #$01
   LDA   (FP),Y
   LDY   #$09
   AND   (FP),Y
   LDY   #$0D
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   LDY   #$0A
   AND   (FP),Y
   LDY   #$0E
   STA   (FP),Y
   LDY   #$03
   LDA   (FP),Y
   LDY   #$0B
   AND   (FP),Y
   LDY   #$0F
   STA   (FP),Y
   LDY   #$04
   LDA   (FP),Y
   LDY   #$0C
   AND   (FP),Y
   LDY   #$10
   STA   (FP),Y
   LDY   #$0D
   LDA   (FP),Y
   LDY   #$11
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$3D
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$13
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$14
   STA   (FP),Y
   LDA   #$FF
   CLC
   ADC   #$07
   INY
   STA   (FP),Y
   LDA   #$00
   ADC   #$00
   INY
   STA   (FP),Y
   LDY   #$13
   LDA   (FP),Y
   CLC
   LDY   #$15
   ADC   (FP),Y
   LDY   #$17
   STA   (FP),Y
   LDY   #$14
   LDA   (FP),Y
   LDY   #$16
   ADC   (FP),Y
   LDY   #$18
   STA   (FP),Y
   LDY   #$3D
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$17
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$18
   LDA   (FP),Y
   LDY   #$01
   STA   (DPTR),Y
   LDA   #$64
   LDY   #$19
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   ORA   #$00
   BPL   .sx_done@40
   LDA   #$FF
.sx_done@40:
   LDY   #$1B
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$39
   LDA   (FP),Y
   CLC
   LDY   #$19
   ADC   (FP),Y
   LDY   #$1D
   STA   (FP),Y
   LDY   #$3A
   LDA   (FP),Y
   LDY   #$1A
   ADC   (FP),Y
   LDY   #$1E
   STA   (FP),Y
   LDY   #$3B
   LDA   (FP),Y
   LDY   #$1B
   ADC   (FP),Y
   LDY   #$1F
   STA   (FP),Y
   LDY   #$3C
   LDA   (FP),Y
   LDY   #$1C
   ADC   (FP),Y
   LDY   #$20
   STA   (FP),Y
   LDY   #$1D
   LDA   (FP),Y
   LDY   #$21
   STA   (FP),Y
   LDY   #$1E
   LDA   (FP),Y
   LDY   #$22
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
   LDY   #$21
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$22
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   JSR   rec
   LDY   #$23
   STA   (FP),Y
   TXA
   INY
   STA   (FP),Y
   LDY   #$12
   LDA   (FP),Y
   LDY   #$25
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@41
   LDA   #$00
   JMP   .sx_done@42
.sx_neg@41:
   LDA   #$FF
.sx_done@42:
   LDY   #$26
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   LDY   #$23
   ADC   (FP),Y
   LDY   #$27
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$24
   ADC   (FP),Y
   LDY   #$28
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$29
   STA   (FP),Y
   LDY   #$12
   STA   (FP),Y
   LDA   #$00
   LDY   #$2A
   STA   (FP),Y
   INY
   STA   (FP),Y
   ORA   #$00
   BPL   .sx_done@44
   LDA   #$FF
.sx_done@44:
   LDY   #$2C
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$39
   LDA   (FP),Y
   LDY   #$2A
   AND   (FP),Y
   LDY   #$2E
   STA   (FP),Y
   LDY   #$3A
   LDA   (FP),Y
   LDY   #$2B
   AND   (FP),Y
   LDY   #$2F
   STA   (FP),Y
   LDY   #$3B
   LDA   (FP),Y
   LDY   #$2C
   AND   (FP),Y
   LDY   #$30
   STA   (FP),Y
   LDY   #$3C
   LDA   (FP),Y
   LDY   #$2D
   AND   (FP),Y
   LDY   #$31
   STA   (FP),Y
   LDY   #$39
   LDA   (FP),Y
   LDY   #$2E
   EOR   (FP),Y
   LDY   #$32
   STA   (FP),Y
   LDY   #$3A
   LDA   (FP),Y
   LDY   #$2F
   EOR   (FP),Y
   LDY   #$33
   STA   (FP),Y
   LDY   #$3B
   LDA   (FP),Y
   LDY   #$30
   EOR   (FP),Y
   LDY   #$34
   STA   (FP),Y
   LDY   #$3C
   LDA   (FP),Y
   LDY   #$31
   EOR   (FP),Y
   LDY   #$35
   STA   (FP),Y
   DEY
   ORA   (FP),Y
   DEY
   ORA   (FP),Y
   DEY
   ORA   (FP),Y
   BEQ   .cmp_end@45
   LDA   #$01
.cmp_end@45:
   LDY   #$36
   STA   (FP),Y

   ; epilogue
   PHA
   CLC
   LDA   FP
   ADC   #$3E
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

   ; prologue: 4 arg bytes, 46 local bytes
   SEC
   LDA   SSP
   SBC   #$30
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$2F
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
   LDY   #$32
   LDA   (FP),Y
   LDY   #$02
   STA   (FP),Y
   LDY   #$31
   LDA   (FP),Y
   LDY   #$03
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
   LDY   #$05
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$06
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$07
   STA   (FP),Y
   LDY   #$01
   LDA   (FP),Y
   CLC
   LDY   #$06
   ADC   (FP),Y
   LDY   #$08
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   LDY   #$07
   ADC   (FP),Y
   LDY   #$09
   STA   (FP),Y
   DEY
   LDA   (FP),Y
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
   DEY
   LDA   (FP),Y
   LDY   #$0D
   STA   (FP),Y
   LDA   g0
   ORA   #$01
   INY
   STA   (FP),Y
   LDA   g0+1
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
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$31
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
   LDY   #$10
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
   LDA   HARGS+12
   LDY   #$14
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
   LDY   #$14
   LDA   (FP),Y
   LDY   #$18
   STA   (FP),Y
   LDY   #$15
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
   LDY   #$0C
   LDA   (FP),Y
   LDY   #$1C
   STA   (FP),Y
   LDY   #$0D
   LDA   (FP),Y
   LDY   #$1D
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   HARGS
   INY
   LDA   (FP),Y
   STA   HARGS+1
   LDA   g0
   STA   HARGS+2
   LDA   g0+1
   STA   HARGS+3
   JSR   mul16
   LDA   HARGS+4
   LDY   #$1E
   STA   (FP),Y
   LDA   HARGS+5
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$20
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$21
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
   LDY   #$20
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$21
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   JSR   rec
   LDY   #$22
   STA   (FP),Y
   TXA
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$24
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$25
   STA   (FP),Y
   LDY   #$1A
   LDA   (FP),Y
   CLC
   LDY   #$24
   ADC   (FP),Y
   LDY   #$26
   STA   (FP),Y
   LDY   #$1B
   LDA   (FP),Y
   LDY   #$25
   ADC   (FP),Y
   LDY   #$27
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$1A
   STA   (FP),Y
   LDY   #$27
   LDA   (FP),Y
   LDY   #$1B
   STA   (FP),Y
   LDA   #$03
   LDY   #$28
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$0C
   LDA   (FP),Y
   STA   HARGS
   INY
   LDA   (FP),Y
   STA   HARGS+1
   LDY   #$28
   LDA   (FP),Y
   STA   HARGS+2
   INY
   LDA   (FP),Y
   STA   HARGS+3
   JSR   sdivmod16
   LDA   HARGS+4
   LDY   #$2A
   STA   (FP),Y
   LDA   HARGS+5
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$0C
   EOR   (FP),Y
   LDY   #$2C
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$0D
   EOR   (FP),Y
   LDY   #$2D
   STA   (FP),Y
   DEY
   ORA   (FP),Y
   BEQ   .cmp_end@48
   LDA   #$01
.cmp_end@48:
   LDY   #$2E
   STA   (FP),Y

   ; epilogue
   PHA
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
   PLA
   RTS

f0:
   SUBROUTINE

   ; prologue: 9 arg bytes, 152 local bytes
   SEC
   LDA   SSP
   SBC   #$9A
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$99
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   LDY   #$A3
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDA   #$64
   SEC
   DEY
   SBC   (FP),Y
   LDA   #$00
   INY
   SBC   (FP),Y
   BVC   .cmp_novf@49
   EOR   #$80
.cmp_novf@49:
   ASL
   LDA   #$00
   ROL
   LDY   #$03
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$05
   STA   (FP),Y
   AND   #$03
   INY
   STA   (FP),Y
   EOR   #$02
   INY
   STA   (FP),Y
   SEC
   SBC   #$02
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$9F
   LDA   (FP),Y
   LDY   #$0A
   STA   (FP),Y
   LDY   #$A0
   LDA   (FP),Y
   LDY   #$0B
   STA   (FP),Y
   LDY   #$A1
   LDA   (FP),Y
   LDY   #$0C
   STA   (FP),Y
   LDY   #$A2
   LDA   (FP),Y
   LDY   #$0D
   STA   (FP),Y
   LDA   g0
   INY
   STA   (FP),Y
   LDA   g0+1
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$A2
   LDA   (FP),Y
   LDY   #$11
   CMP   (FP),Y
   BNE   .thread@1
   LDY   #$A1
   LDA   (FP),Y
   LDY   #$10
   CMP   (FP),Y
   BNE   .thread@1
   LDY   #$A0
   LDA   (FP),Y
   LDY   #$0F
   CMP   (FP),Y
   BNE   .thread@1
   LDY   #$9F
   LDA   (FP),Y
   LDY   #$0E
   CMP   (FP),Y
   BEQ   .cmp_true@50
.thread@1:
   LDA   #$00
   JMP   .cmp_end@51
.cmp_true@50:
   LDA   #$01
.cmp_end@51:
   LDY   #$12
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$9B
   LDA   (FP),Y
   SEC
   SBC   (FP),Y
   LDY   #$14
   STA   (FP),Y
   LDY   #$9C
   LDA   (FP),Y
   SBC   (FP),Y
   LDY   #$15
   STA   (FP),Y
   LDY   #$9D
   LDA   (FP),Y
   SBC   (FP),Y
   LDY   #$16
   STA   (FP),Y
   LDY   #$9E
   LDA   (FP),Y
   SBC   (FP),Y
   LDY   #$17
   STA   (FP),Y
   LDA   #$07
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   ORA   #$00
   BPL   .sx_done@54
   LDA   #$FF
.sx_done@54:
   LDY   #$1A
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$14
   LDA   (FP),Y
   LDY   #$18
   AND   (FP),Y
   LDY   #$1C
   STA   (FP),Y
   LDY   #$15
   LDA   (FP),Y
   LDY   #$19
   AND   (FP),Y
   LDY   #$1D
   STA   (FP),Y
   LDY   #$16
   LDA   (FP),Y
   LDY   #$1A
   AND   (FP),Y
   LDY   #$1E
   STA   (FP),Y
   LDY   #$17
   LDA   (FP),Y
   LDY   #$1B
   AND   (FP),Y
   LDY   #$1F
   STA   (FP),Y
   LDY   #$12
   LDA   (FP),Y
   LDY   #$20
   STA   (FP),Y
   LDY   #$13
   LDA   (FP),Y
   LDY   #$21
   STA   (FP),Y
   LDY   #$1C
   LDA   (FP),Y
   TAX
   BEQ   .shift_done@56
.shift_loop@55:
   LDY   #$20
   LDA   (FP),Y
   ASL
   STA   (FP),Y
   INY
   LDA   (FP),Y
   ROL
   STA   (FP),Y
   DEX
   BNE   .shift_loop@55
.shift_done@56:
   LDY   #$20
   LDA   (FP),Y
   LDY   #$22
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$23
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@57
   LDA   #$00
   JMP   .sx_done@58
.sx_neg@57:
   LDA   #$FF
.sx_done@58:
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
   LDA   #$07
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDA   g0
   INY
   STA   (FP),Y
   LDA   g0+1
   INY
   STA   (FP),Y
   LDY   #$2A
   LDA   (FP),Y
   TAX
   BEQ   .shift_done@60
.shift_loop@59:
   LDY   #$2C
   LDA   (FP),Y
   ASL
   STA   (FP),Y
   INY
   LDA   (FP),Y
   ROL
   STA   (FP),Y
   DEX
   BNE   .shift_loop@59
.shift_done@60:
   LDY   #$2C
   LDA   (FP),Y
   LDY   #$2E
   STA   (FP),Y
   AND   #$03
   INY
   STA   (FP),Y
   EOR   #$02
   INY
   STA   (FP),Y
   SEC
   SBC   #$02
   INY
   STA   (FP),Y
   LDY   #$09
   LDA   (FP),Y
   LDY   #$32
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@61
   LDA   #$00
   JMP   .sx_done@62
.sx_neg@61:
   LDA   #$FF
.sx_done@62:
   LDY   #$33
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$9F
   LDA   (FP),Y
   LDY   #$32
   AND   (FP),Y
   LDY   #$36
   STA   (FP),Y
   LDY   #$A0
   LDA   (FP),Y
   LDY   #$33
   AND   (FP),Y
   LDY   #$37
   STA   (FP),Y
   LDY   #$A1
   LDA   (FP),Y
   LDY   #$34
   AND   (FP),Y
   LDY   #$38
   STA   (FP),Y
   LDY   #$A2
   LDA   (FP),Y
   LDY   #$35
   AND   (FP),Y
   LDY   #$39
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$05
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDY   #$31
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$36
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$37
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$38
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   LDY   #$39
   LDA   (FP),Y
   LDY   #$05
   STA   (SSP),Y
   JSR   f4
   LDY   #$3A
   STA   (FP),Y
   ORA   #$00
   BEQ   .if_else@4
   LDA   #$03
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   ORA   #$00
   BPL   .sx_done@64
   LDA   #$FF
.sx_done@64:
   LDY   #$3D
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$0A
   LDA   (FP),Y
   CLC
   LDY   #$3B
   ADC   (FP),Y
   LDY   #$3F
   STA   (FP),Y
   LDY   #$0B
   LDA   (FP),Y
   LDY   #$3C
   ADC   (FP),Y
   LDY   #$40
   STA   (FP),Y
   LDY   #$0C
   LDA   (FP),Y
   LDY   #$3D
   ADC   (FP),Y
   LDY   #$41
   STA   (FP),Y
   LDY   #$0D
   LDA   (FP),Y
   LDY   #$3E
   ADC   (FP),Y
   LDY   #$42
   STA   (FP),Y
   LDY   #$3F
   LDA   (FP),Y
   LDY   #$0A
   STA   (FP),Y
   LDY   #$40
   LDA   (FP),Y
   LDY   #$0B
   STA   (FP),Y
   LDY   #$41
   LDA   (FP),Y
   LDY   #$0C
   STA   (FP),Y
   LDY   #$42
   LDA   (FP),Y
   LDY   #$0D
   STA   (FP),Y
   JMP   .if_end@3
.if_else@4:
   LDA   #$05
   LDY   #$43
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   ORA   #$00
   BPL   .sx_done@66
   LDA   #$FF
.sx_done@66:
   LDY   #$45
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$0A
   LDA   (FP),Y
   LDY   #$43
   EOR   (FP),Y
   LDY   #$47
   STA   (FP),Y
   LDY   #$0B
   LDA   (FP),Y
   LDY   #$44
   EOR   (FP),Y
   LDY   #$48
   STA   (FP),Y
   LDY   #$0C
   LDA   (FP),Y
   LDY   #$45
   EOR   (FP),Y
   LDY   #$49
   STA   (FP),Y
   LDY   #$0D
   LDA   (FP),Y
   LDY   #$46
   EOR   (FP),Y
   LDY   #$4A
   STA   (FP),Y
   LDY   #$47
   LDA   (FP),Y
   LDY   #$0A
   STA   (FP),Y
   LDY   #$48
   LDA   (FP),Y
   LDY   #$0B
   STA   (FP),Y
   LDY   #$49
   LDA   (FP),Y
   LDY   #$0C
   STA   (FP),Y
   LDY   #$4A
   LDA   (FP),Y
   LDY   #$0D
   STA   (FP),Y
.if_end@3:
   LDA   g0
   LDY   #$4B
   STA   (FP),Y
   LDA   g0+1
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$4B
   LDA   (FP),Y
   CLC
   LDY   #$0A
   ADC   (FP),Y
   LDY   #$4F
   STA   (FP),Y
   LDY   #$4C
   LDA   (FP),Y
   LDY   #$0B
   ADC   (FP),Y
   LDY   #$50
   STA   (FP),Y
   LDY   #$4D
   LDA   (FP),Y
   LDY   #$0C
   ADC   (FP),Y
   LDY   #$51
   STA   (FP),Y
   LDY   #$4E
   LDA   (FP),Y
   LDY   #$0D
   ADC   (FP),Y
   LDY   #$52
   STA   (FP),Y
   LDY   #$4F
   LDA   (FP),Y
   LDY   #$53
   STA   (FP),Y
   LDY   #$50
   LDA   (FP),Y
   LDY   #$54
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   g0
   INY
   LDA   (FP),Y
   STA   g0+1
   LDY   #$09
   LDA   (FP),Y
   LDY   #$55
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@67
   LDA   #$00
   JMP   .sx_done@68
.sx_neg@67:
   LDA   #$FF
.sx_done@68:
   LDY   #$56
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$57
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$58
   STA   (FP),Y
   LDA   #$FF
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   ORA   #$00
   BPL   .sx_done@70
   LDA   #$FF
.sx_done@70:
   LDY   #$5B
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$9F
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
   LDY   #$59
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
   LDY   #$5D
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
   LDY   #$5D
   LDA   (FP),Y
   LDY   #$61
   STA   (FP),Y
   LDY   #$5E
   LDA   (FP),Y
   LDY   #$62
   STA   (FP),Y
   LDY   #$5F
   LDA   (FP),Y
   LDY   #$63
   STA   (FP),Y
   LDY   #$60
   LDA   (FP),Y
   LDY   #$64
   STA   (FP),Y
   LDA   #$EB
   BEQ   .lnot_true@71
   LDA   #$00
   JMP   .lnot_end@72
.lnot_true@71:
   LDA   #$01
.lnot_end@72:
   LDY   #$65
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$9B
   LDA   (FP),Y
   SEC
   SBC   (FP),Y
   INY
   LDA   (FP),Y
   SBC   (FP),Y
   INY
   LDA   (FP),Y
   SBC   (FP),Y
   INY
   LDA   (FP),Y
   SBC   (FP),Y
   LDA   #$00
   ROL
   LDY   #$67
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$9B
   LDA   (FP),Y
   ORA   (FP),Y
   LDY   #$69
   STA   (FP),Y
   LDY   #$9C
   LDA   (FP),Y
   ORA   (FP),Y
   LDY   #$6A
   STA   (FP),Y
   LDY   #$9D
   LDA   (FP),Y
   ORA   (FP),Y
   LDY   #$6B
   STA   (FP),Y
   LDY   #$9E
   LDA   (FP),Y
   ORA   (FP),Y
   LDY   #$6C
   STA   (FP),Y
   LDY   #$69
   LDA   (FP),Y
   LDY   #$6D
   STA   (FP),Y
   LDY   #$6A
   LDA   (FP),Y
   LDY   #$6E
   STA   (FP),Y
   CLC
   LDA   FP
   ADC   #$57
   INY
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$0C
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDY   #$61
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$62
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$63
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$64
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   LDY   #$65
   LDA   (FP),Y
   LDY   #$05
   STA   (SSP),Y
   LDY   #$66
   LDA   (FP),Y
   LDY   #$06
   STA   (SSP),Y
   LDY   #$67
   LDA   (FP),Y
   LDY   #$07
   STA   (SSP),Y
   LDY   #$68
   LDA   (FP),Y
   LDY   #$08
   STA   (SSP),Y
   LDY   #$6D
   LDA   (FP),Y
   LDY   #$09
   STA   (SSP),Y
   LDY   #$6E
   LDA   (FP),Y
   LDY   #$0A
   STA   (SSP),Y
   LDY   #$6F
   LDA   (FP),Y
   LDY   #$0B
   STA   (SSP),Y
   LDY   #$70
   LDA   (FP),Y
   LDY   #$0C
   STA   (SSP),Y
   JSR   f5
   LDA   #$00
   ROL
   LDY   #$71
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$09
   LDA   (FP),Y
   LDY   #$74
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@73
   LDA   #$00
   JMP   .sx_done@74
.sx_neg@73:
   LDA   #$FF
.sx_done@74:
   LDY   #$75
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$72
   EOR   (FP),Y
   LDY   #$76
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$73
   EOR   (FP),Y
   LDY   #$77
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$78
   STA   (FP),Y
   AND   #$03
   INY
   STA   (FP),Y
   EOR   #$02
   INY
   STA   (FP),Y
   SEC
   SBC   #$02
   INY
   STA   (FP),Y
   LDY   #$09
   STA   (FP),Y
   LDY   #$7C
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@75
   LDA   #$00
   JMP   .sx_done@76
.sx_neg@75:
   LDA   #$FF
.sx_done@76:
   LDY   #$7D
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   LDY   #$57
   ADC   (FP),Y
   LDY   #$7E
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$58
   ADC   (FP),Y
   LDY   #$7F
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$80
   STA   (FP),Y
   AND   #$03
   INY
   STA   (FP),Y
   EOR   #$02
   INY
   STA   (FP),Y
   SEC
   SBC   #$02
   INY
   STA   (FP),Y
   LDY   #$09
   STA   (FP),Y
   LDY   #$84
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@77
   LDA   #$00
   JMP   .sx_done@78
.sx_neg@77:
   LDA   #$FF
.sx_done@78:
   LDY   #$85
   STA   (FP),Y
   LDA   g0
   CLC
   DEY
   ADC   (FP),Y
   LDY   #$86
   STA   (FP),Y
   LDA   g0+1
   DEY
   ADC   (FP),Y
   LDY   #$87
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   g0
   INY
   LDA   (FP),Y
   STA   g0+1
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
   LDA   #$FF
   INY
   STA   (SSP),Y
   LDA   #$00
   INY
   STA   (SSP),Y
   JSR   rec
   LDY   #$88
   STA   (FP),Y
   TXA
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$8A
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$8B
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@79
   LDA   #$00
   JMP   .sx_done@80
.sx_neg@79:
   LDA   #$FF
.sx_done@80:
   LDY   #$8C
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$0A
   LDA   (FP),Y
   CLC
   LDY   #$8A
   ADC   (FP),Y
   LDY   #$8E
   STA   (FP),Y
   LDY   #$0B
   LDA   (FP),Y
   LDY   #$8B
   ADC   (FP),Y
   LDY   #$8F
   STA   (FP),Y
   LDY   #$0C
   LDA   (FP),Y
   LDY   #$8C
   ADC   (FP),Y
   LDY   #$90
   STA   (FP),Y
   LDY   #$0D
   LDA   (FP),Y
   LDY   #$8D
   ADC   (FP),Y
   LDY   #$91
   STA   (FP),Y
   LDY   #$8E
   LDA   (FP),Y
   LDY   #$0A
   STA   (FP),Y
   LDY   #$8F
   LDA   (FP),Y
   LDY   #$0B
   STA   (FP),Y
   LDY   #$90
   LDA   (FP),Y
   LDY   #$0C
   STA   (FP),Y
   LDY   #$91
   LDA   (FP),Y
   LDY   #$0D
   STA   (FP),Y
   LDA   #$00
   LDY   #$92
   STA   (FP),Y
   INY
   STA   (FP),Y
   ORA   #$00
   BPL   .sx_done@82
   LDA   #$FF
.sx_done@82:
   LDY   #$94
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$A2
   LDA   (FP),Y
   LDY   #$95
   CMP   (FP),Y
   BNE   .cmp_true@83
   LDY   #$A1
   LDA   (FP),Y
   LDY   #$94
   CMP   (FP),Y
   BNE   .cmp_true@83
   LDY   #$A0
   LDA   (FP),Y
   LDY   #$93
   CMP   (FP),Y
   BNE   .cmp_true@83
   LDY   #$9F
   LDA   (FP),Y
   LDY   #$92
   CMP   (FP),Y
   BNE   .cmp_true@83
   LDA   #$00
   JMP   .cmp_end@84
.cmp_true@83:
   LDA   #$01
.cmp_end@84:
   LDY   #$96
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$98
   STA   (FP),Y
   CMP   #$01

   ; epilogue
   PHP
   CLC
   LDA   FP
   ADC   #$A3
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

   ; prologue: 0 arg bytes, 36 local bytes
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

   LDA   #$9D
   LDY   #$01
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   ORA   #$00
   BPL   .sx_done@87
   LDA   #$FF
.sx_done@87:
   LDY   #$03
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$80
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   ORA   #$00
   BPL   .sx_done@89
   LDA   #$FF
.sx_done@89:
   LDY   #$07
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$D9
   INY
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$09
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDY   #$01
   LDA   (FP),Y
   STA   (SSP),Y
   INY
   LDA   (FP),Y
   STA   (SSP),Y
   INY
   LDA   (FP),Y
   STA   (SSP),Y
   INY
   LDA   (FP),Y
   STA   (SSP),Y
   INY
   LDA   (FP),Y
   STA   (SSP),Y
   INY
   LDA   (FP),Y
   STA   (SSP),Y
   INY
   LDA   (FP),Y
   STA   (SSP),Y
   INY
   LDA   (FP),Y
   STA   (SSP),Y
   INY
   LDA   (FP),Y
   STA   (SSP),Y
   JSR   f0
   LDA   #$00
   ROL
   LDY   #$0A
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
   INY
   STA   (FP),Y
   LDA   g0
   INY
   STA   (FP),Y
   LDA   g0+1
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$0F
   LDA   (FP),Y
   LDY   #$13
   EOR   (FP),Y
   LDY   #$17
   STA   (FP),Y
   LDY   #$10
   LDA   (FP),Y
   LDY   #$14
   EOR   (FP),Y
   LDY   #$18
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
   LDY   #$17
   LDA   (FP),Y
   LDY   #$0F
   STA   (FP),Y
   LDY   #$18
   LDA   (FP),Y
   LDY   #$10
   STA   (FP),Y
   LDY   #$19
   LDA   (FP),Y
   LDY   #$11
   STA   (FP),Y
   LDY   #$1A
   LDA   (FP),Y
   LDY   #$12
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$1B
   STA   (FP),Y
   LDY   #$12
   LDA   (FP),Y
   LDY   #$1C
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$0F
   LDA   (FP),Y
   LDY   #$1B
   EOR   (FP),Y
   LDY   #$1F
   STA   (FP),Y
   LDY   #$10
   LDA   (FP),Y
   LDY   #$1C
   EOR   (FP),Y
   LDY   #$20
   STA   (FP),Y
   LDY   #$11
   LDA   (FP),Y
   LDY   #$1D
   EOR   (FP),Y
   LDY   #$21
   STA   (FP),Y
   LDY   #$12
   LDA   (FP),Y
   LDY   #$1E
   EOR   (FP),Y
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
   TAX
   DEY
   LDA   (FP),Y

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$26
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$25
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
   DC.W  $0052
