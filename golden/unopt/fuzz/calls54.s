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

f3:
   SUBROUTINE

   ; prologue: 4 arg bytes, 43 local bytes
   SEC
   LDA   SSP
   SBC   #$2D
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$2C
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
   LDY   #$2F
   LDA   (FP),Y
   LDY   #$02
   STA   (FP),Y
   LDY   #$30
   LDA   (FP),Y
   CLC
   LDY   #$01
   SBC   (FP),Y
   LDY   #$31
   LDA   (FP),Y
   LDY   #$02
   SBC   (FP),Y
   LDA   #$00
   ROL
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDA   g1
   INY
   STA   (FP),Y
   LDA   g1+1
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   g0+3
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
   BEQ   .cmp_true@7
.thread@0:
   LDA   #$00
   JMP   .cmp_end@8
.cmp_true@7:
   LDA   #$01
.cmp_end@8:
   LDY   #$09
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   AND   #$07
   LDY   #$0B
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDA   #$00
   LDY   #$0C
   STA   (FP),Y
   LDY   #$03
   LDA   (FP),Y
   LDY   #$0D
   STA   (FP),Y
   LDY   #$04
   LDA   (FP),Y
   LDY   #$0E
   STA   (FP),Y
   LDY   #$0B
   LDA   (FP),Y
   TAX
   BEQ   .shift_done@11
.shift_loop@10:
   LDY   #$0E
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
   BNE   .shift_loop@10
.shift_done@11:
   LDY   #$0D
   LDA   (FP),Y
   LDY   #$0F
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@12
   LDA   #$00
   JMP   .sx_done@13
.sx_neg@12:
   LDA   #$FF
.sx_done@13:
   LDY   #$12
   STA   (FP),Y
   LDY   #$10
   LDA   (FP),Y
   LDY   #$13
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@14
   LDA   #$00
   JMP   .sx_done@15
.sx_neg@14:
   LDA   #$FF
.sx_done@15:
   LDY   #$14
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   AND   #$07
   LDY   #$15
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDA   #$00
   LDY   #$16
   STA   (FP),Y
   LDA   #$E8
   INY
   STA   (FP),Y
   LDA   #$03
   INY
   STA   (FP),Y
   LDY   #$15
   LDA   (FP),Y
   TAX
   BEQ   .shift_done@17
.shift_loop@16:
   LDY   #$18
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
   BNE   .shift_loop@16
.shift_done@17:
   LDY   #$11
   LDA   (FP),Y
   SEC
   LDY   #$17
   SBC   (FP),Y
   LDY   #$12
   LDA   (FP),Y
   LDY   #$18
   SBC   (FP),Y
   BVC   .cmp_novf@18
   EOR   #$80
.cmp_novf@18:
   ASL
   LDA   #$00
   ROL
   LDY   #$19
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$1B
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$10
   LDA   (FP),Y
   LDY   #$1D
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   g1
   ORA   #$01
   INY
   STA   (FP),Y
   LDA   g1+1
   INY
   STA   (FP),Y
   LDA   g1
   STA   HARGS
   LDA   g1+1
   STA   HARGS+1
   DEY
   LDA   (FP),Y
   STA   HARGS+2
   INY
   LDA   (FP),Y
   STA   HARGS+3
   JSR   udivmod16
   LDA   HARGS+6
   LDY   #$21
   STA   (FP),Y
   LDA   HARGS+7
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@1
   LDY   #$1C
   LDA   (FP),Y
   LDY   #$23
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$2E
   LDA   (FP),Y
   ORA   #$01
   LDY   #$25
   STA   (FP),Y
   LDY   #$2F
   LDA   (FP),Y
   LDY   #$26
   STA   (FP),Y
   LDY   #$23
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
   JSR   sdivmod16
   LDA   HARGS+6
   LDY   #$27
   STA   (FP),Y
   LDA   HARGS+7
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
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
   LDY   #$29
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@2:
   LDY   #$29
   LDA   (FP),Y
   LDY   #$2B
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

f2:
   SUBROUTINE

   ; prologue: 9 arg bytes, 62 local bytes
   SEC
   LDA   SSP
   SBC   #$40
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$3F
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   LDY   #$47
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   LDA   #$00
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
   LDA   g1
   INY
   STA   (FP),Y
   LDA   g1+1
   INY
   STA   (FP),Y
   LDY   #$03
   LDA   (FP),Y
   TAX
   BEQ   .shift_done@20
.shift_loop@19:
   LDY   #$06
   LDA   (FP),Y
   LSR
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ROR
   STA   (FP),Y
   DEX
   BNE   .shift_loop@19
.shift_done@20:
   SEC
   LDA   #$00
   LDY   #$05
   SBC   (FP),Y
   LDY   #$07
   STA   (FP),Y
   LDA   #$00
   DEY
   SBC   (FP),Y
   LDY   #$08
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$09
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$0A
   STA   (FP),Y
   LDA   #$00
   INY
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
   LDY   #$43
   LDA   (FP),Y
   LDY   #$11
   STA   (FP),Y
   LDY   #$44
   LDA   (FP),Y
   LDY   #$12
   STA   (FP),Y
   LDY   #$45
   LDA   (FP),Y
   LDY   #$13
   STA   (FP),Y
   LDY   #$46
   LDA   (FP),Y
   LDY   #$14
   STA   (FP),Y
   LDY   #$11
   LDA   (FP),Y
   LDY   #$15
   STA   (FP),Y
   LDY   #$12
   LDA   (FP),Y
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
   LDY   #$47
   LDA   (FP),Y
   LDY   #$19
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   EOR   #$FF
   LDY   #$1B
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   EOR   #$FF
   LDY   #$1C
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$1D
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$0D
   LDA   (FP),Y
   LDY   #$1F
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$48
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
   LDY   #$20
   LDA   (FP),Y
   LDY   #$23
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   LDY   #$25
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$26
   STA   (FP),Y
   LDY   #$21
   LDA   (FP),Y
   CLC
   LDY   #$25
   ADC   (FP),Y
   LDY   #$27
   STA   (FP),Y
   LDY   #$22
   LDA   (FP),Y
   LDY   #$26
   ADC   (FP),Y
   LDY   #$28
   STA   (FP),Y
   LDY   #$48
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$27
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$28
   LDA   (FP),Y
   LDY   #$01
   STA   (DPTR),Y
   LDA   g1
   LDY   #$29
   STA   (FP),Y
   LDA   g1+1
   INY
   STA   (FP),Y
   LDA   #$03
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   ORA   #$00
   BPL   .sx_done@22
   LDA   #$FF
.sx_done@22:
   LDY   #$2D
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$0D
   LDA   (FP),Y
   SEC
   LDY   #$2B
   SBC   (FP),Y
   LDY   #$2F
   STA   (FP),Y
   LDY   #$0E
   LDA   (FP),Y
   LDY   #$2C
   SBC   (FP),Y
   LDY   #$30
   STA   (FP),Y
   LDY   #$0F
   LDA   (FP),Y
   LDY   #$2D
   SBC   (FP),Y
   LDY   #$31
   STA   (FP),Y
   LDY   #$10
   LDA   (FP),Y
   LDY   #$2E
   SBC   (FP),Y
   LDY   #$32
   STA   (FP),Y
   LDY   #$2F
   LDA   (FP),Y
   LDY   #$33
   STA   (FP),Y
   LDY   #$30
   LDA   (FP),Y
   LDY   #$34
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$04
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDY   #$29
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$2A
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$33
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$34
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   JSR   f3
   LDA   #$00
   ROL
   LDY   #$35
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$1E
   LDA   (FP),Y
   LDY   #$38
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$36
   EOR   (FP),Y
   LDY   #$3A
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$37
   EOR   (FP),Y
   LDY   #$3B
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$3C
   STA   (FP),Y
   LDY   #$1E
   STA   (FP),Y
   LDY   #$48
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$3D
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$3E
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
   ADC   #$49
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$3F
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

f1:
   SUBROUTINE

   ; prologue: 9 arg bytes, 80 local bytes
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

   LDA   #$FF
   LDY   #$01
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   ORA   #$00
   BPL   .sx_done@24
   LDA   #$FF
.sx_done@24:
   LDY   #$03
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   g0
   CLC
   LDY   #$01
   ADC   (FP),Y
   LDY   #$05
   STA   (FP),Y
   LDA   g0+1
   LDY   #$02
   ADC   (FP),Y
   LDY   #$06
   STA   (FP),Y
   LDA   g0+2
   LDY   #$03
   ADC   (FP),Y
   LDY   #$07
   STA   (FP),Y
   LDA   g0+3
   LDY   #$04
   ADC   (FP),Y
   LDY   #$08
   STA   (FP),Y
   LDY   #$05
   LDA   (FP),Y
   LDY   #$09
   STA   (FP),Y
   INY
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@25
   LDA   #$00
   JMP   .sx_done@26
.sx_neg@25:
   LDA   #$FF
.sx_done@26:
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
   LDA   #$07
   INY
   STA   (FP),Y
   LDA   #$00
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
   ORA   #$00
   BMI   .sx_neg@27
   LDA   #$00
   JMP   .sx_done@28
.sx_neg@27:
   LDA   #$FF
.sx_done@28:
   LDY   #$12
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$53
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
   JSR   sdivmod32
   LDA   HARGS+8
   LDY   #$14
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
   LDY   #$57
   LDA   (FP),Y
   LDY   #$18
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@29
   LDA   #$00
   JMP   .sx_done@30
.sx_neg@29:
   LDA   #$FF
.sx_done@30:
   LDY   #$19
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
   JSR   mul16
   LDA   HARGS+4
   LDY   #$1A
   STA   (FP),Y
   LDA   HARGS+5
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$1C
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$1D
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@31
   LDA   #$00
   JMP   .sx_done@32
.sx_neg@31:
   LDA   #$FF
.sx_done@32:
   LDY   #$1E
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$14
   LDA   (FP),Y
   LDY   #$1C
   ORA   (FP),Y
   LDY   #$20
   STA   (FP),Y
   LDY   #$15
   LDA   (FP),Y
   LDY   #$1D
   ORA   (FP),Y
   LDY   #$21
   STA   (FP),Y
   LDY   #$16
   LDA   (FP),Y
   LDY   #$1E
   ORA   (FP),Y
   LDY   #$22
   STA   (FP),Y
   LDY   #$17
   LDA   (FP),Y
   LDY   #$1F
   ORA   (FP),Y
   LDY   #$23
   STA   (FP),Y
   LDY   #$20
   LDA   (FP),Y
   LDY   #$24
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$FF
   BNE   .or_true@3
   LDY   #$0C
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@3
   LDA   #$00
   LDY   #$26
   STA   (FP),Y
   INY
   STA   (FP),Y
   JMP   .or_end@4
.or_true@3:
   LDA   #$01
   LDY   #$26
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@4:
   LDY   #$58
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   INY
   ORA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@5
   LDA   g1
   ORA   g1+1
   BNE   .or_true@5
   LDA   #$00
   LDY   #$28
   STA   (FP),Y
   INY
   STA   (FP),Y
   JMP   .or_end@6
.or_true@5:
   LDA   #$01
   LDY   #$28
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@6:
   LDY   #$28
   LDA   (FP),Y
   LDY   #$2A
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$2B
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$04
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDY   #$26
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$27
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$2A
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$2B
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   JSR   f3
   LDA   #$00
   ROL
   LDY   #$2C
   STA   (FP),Y
   ORA   #$00
   BEQ   .if_else@8
   LDY   #$25
   LDA   (FP),Y
   LDY   #$2D
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$03
   LDY   #$2F
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$30
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$31
   STA   (FP),Y
   LDY   #$25
   STA   (FP),Y
   JMP   .if_end@7
.if_else@8:
   LDY   #$25
   LDA   (FP),Y
   LDY   #$32
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   EOR   #$05
   LDY   #$34
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$35
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$36
   STA   (FP),Y
   LDY   #$25
   STA   (FP),Y
.if_end@7:
   LDY   #$37
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   g0
   CLC
   LDY   #$37
   ADC   (FP),Y
   LDY   #$3B
   STA   (FP),Y
   LDA   g0+1
   LDY   #$38
   ADC   (FP),Y
   LDY   #$3C
   STA   (FP),Y
   LDA   g0+2
   LDY   #$39
   ADC   (FP),Y
   LDY   #$3D
   STA   (FP),Y
   LDA   g0+3
   LDY   #$3A
   ADC   (FP),Y
   LDY   #$3E
   STA   (FP),Y
   LDY   #$3B
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
   LDA   g0
   INY
   STA   (FP),Y
   LDA   g0+1
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
   LDY   #$25
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
   LDY   #$25
   STA   (FP),Y
   LDY   #$58
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
   BVC   .cmp_novf@33
   EOR   #$80
.cmp_novf@33:
   ASL
   LDA   #$00
   ROL
   LDY   #$48
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   AND   #$07
   LDY   #$4A
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDA   #$00
   LDY   #$4B
   STA   (FP),Y
   LDA   #$64
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$4A
   LDA   (FP),Y
   TAX
   BEQ   .shift_done@35
.shift_loop@34:
   LDY   #$4C
   LDA   (FP),Y
   ASL
   STA   (FP),Y
   INY
   LDA   (FP),Y
   ROL
   STA   (FP),Y
   DEX
   BNE   .shift_loop@34
.shift_done@35:
   LDY   #$4D
   LDA   (FP),Y
   BNE   .cmp_true@36
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@36
   LDA   #$00
   JMP   .cmp_end@37
.cmp_true@36:
   LDA   #$01
.cmp_end@37:
   LDY   #$4E
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$50
   STA   (FP),Y
   CMP   #$01

   ; epilogue
   PHP
   CLC
   LDA   FP
   ADC   #$5B
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

   ; prologue: 8 arg bytes, 124 local bytes
   SEC
   LDA   SSP
   SBC   #$7E
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$7D
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   LDY   #$84
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
   ORA   #$00
   BMI   .sx_neg@39
   LDA   #$00
   JMP   .sx_done@40
.sx_neg@39:
   LDA   #$FF
.sx_done@40:
   LDY   #$05
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$01
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   ORA   #$00
   BPL   .sx_done@42
   LDA   #$FF
.sx_done@42:
   LDY   #$09
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$80
   LDA   (FP),Y
   LDY   #$07
   ORA   (FP),Y
   LDY   #$0B
   STA   (FP),Y
   LDY   #$81
   LDA   (FP),Y
   LDY   #$08
   ORA   (FP),Y
   LDY   #$0C
   STA   (FP),Y
   LDY   #$82
   LDA   (FP),Y
   LDY   #$09
   ORA   (FP),Y
   LDY   #$0D
   STA   (FP),Y
   LDY   #$83
   LDA   (FP),Y
   LDY   #$0A
   ORA   (FP),Y
   LDY   #$0E
   STA   (FP),Y
   LDY   #$03
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
   LDY   #$0F
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
   LDY   #$7F
   LDA   (FP),Y
   BNE   .or_true@9
   LDA   #$00
   LDY   #$13
   STA   (FP),Y
   INY
   STA   (FP),Y
   JMP   .or_end@10
.or_true@9:
   LDA   #$01
   LDY   #$13
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@10:
   LDY   #$13
   LDA   (FP),Y
   LDY   #$15
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$16
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@43
   LDA   #$00
   JMP   .sx_done@44
.sx_neg@43:
   LDA   #$FF
.sx_done@44:
   LDY   #$17
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$0F
   LDA   (FP),Y
   LDY   #$15
   AND   (FP),Y
   LDY   #$19
   STA   (FP),Y
   LDY   #$10
   LDA   (FP),Y
   LDY   #$16
   AND   (FP),Y
   LDY   #$1A
   STA   (FP),Y
   LDY   #$11
   LDA   (FP),Y
   LDY   #$17
   AND   (FP),Y
   LDY   #$1B
   STA   (FP),Y
   LDY   #$12
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
   DEY
   LDA   (FP),Y
   LDY   #$1F
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$20
   STA   (FP),Y
   LDY   #$80
   LDA   (FP),Y
   LDY   #$21
   STA   (FP),Y
   LDY   #$81
   LDA   (FP),Y
   LDY   #$22
   STA   (FP),Y
   LDY   #$82
   LDA   (FP),Y
   LDY   #$23
   STA   (FP),Y
   LDY   #$83
   LDA   (FP),Y
   LDY   #$24
   STA   (FP),Y
   LDY   #$21
   LDA   (FP),Y
   LDY   #$25
   STA   (FP),Y
   INY
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
   BPL   .sx_done@46
   LDA   #$FF
.sx_done@46:
   LDY   #$2A
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$80
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
   LDY   #$28
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
   LDY   #$2C
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
   LDA   g1
   INY
   STA   (FP),Y
   LDA   g1+1
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$2C
   LDA   (FP),Y
   SEC
   LDY   #$30
   SBC   (FP),Y
   LDY   #$34
   STA   (FP),Y
   LDY   #$2D
   LDA   (FP),Y
   LDY   #$31
   SBC   (FP),Y
   LDY   #$35
   STA   (FP),Y
   LDY   #$2E
   LDA   (FP),Y
   LDY   #$32
   SBC   (FP),Y
   LDY   #$36
   STA   (FP),Y
   LDY   #$2F
   LDA   (FP),Y
   LDY   #$33
   SBC   (FP),Y
   LDY   #$37
   STA   (FP),Y
   LDY   #$34
   LDA   (FP),Y
   LDY   #$38
   STA   (FP),Y
   LDY   #$35
   LDA   (FP),Y
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
   LDY   #$80
   LDA   (FP),Y
   LDY   #$3C
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$85
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$3E
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$3F
   STA   (FP),Y
   LDY   #$7F
   LDA   (FP),Y
   LDY   #$40
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   AND   #$07
   LDY   #$42
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDA   #$00
   LDY   #$43
   STA   (FP),Y
   LDA   #$FF
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$42
   LDA   (FP),Y
   TAX
   BEQ   .shift_done@48
.shift_loop@47:
   LDY   #$45
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
   BNE   .shift_loop@47
.shift_done@48:
   LDY   #$3E
   LDA   (FP),Y
   CLC
   LDY   #$44
   ADC   (FP),Y
   LDY   #$46
   STA   (FP),Y
   LDY   #$3F
   LDA   (FP),Y
   LDY   #$45
   ADC   (FP),Y
   LDY   #$47
   STA   (FP),Y
   LDY   #$85
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$46
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$47
   LDA   (FP),Y
   LDY   #$01
   STA   (DPTR),Y
   LDA   g0
   LDY   #$48
   STA   (FP),Y
   LDA   g0+1
   INY
   STA   (FP),Y
   LDY   #$1F
   LDA   (FP),Y
   LDY   #$4A
   STA   (FP),Y
   LDY   #$20
   LDA   (FP),Y
   LDY   #$4B
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@49
   LDA   #$00
   JMP   .sx_done@50
.sx_neg@49:
   LDA   #$FF
.sx_done@50:
   LDY   #$4C
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$4A
   LDA   (FP),Y
   LDY   #$4E
   STA   (FP),Y
   LDY   #$4B
   LDA   (FP),Y
   LDY   #$4F
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$04
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDY   #$48
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$49
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$4E
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$4F
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   JSR   f3
   LDA   #$00
   ROL
   LDY   #$50
   STA   (FP),Y
   ORA   #$00
   BEQ   .if_else@12
   LDY   #$3A
   LDA   (FP),Y
   CLC
   ADC   #$03
   LDY   #$51
   STA   (FP),Y
   LDY   #$3B
   LDA   (FP),Y
   ADC   #$00
   LDY   #$52
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$3A
   STA   (FP),Y
   LDY   #$52
   LDA   (FP),Y
   LDY   #$3B
   STA   (FP),Y
   JMP   .if_end@11
.if_else@12:
   LDY   #$3A
   LDA   (FP),Y
   EOR   #$05
   LDY   #$53
   STA   (FP),Y
   LDY   #$3B
   LDA   (FP),Y
   LDY   #$54
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$3A
   STA   (FP),Y
   LDY   #$54
   LDA   (FP),Y
   LDY   #$3B
   STA   (FP),Y
.if_end@11:
   LDY   #$1F
   LDA   (FP),Y
   LDY   #$55
   STA   (FP),Y
   LDY   #$20
   LDA   (FP),Y
   LDY   #$56
   STA   (FP),Y
   LDA   g1
   ORA   g1+1
   BEQ   .and_false@13
   LDA   #$02
   BEQ   .and_false@13
   LDA   #$01
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   JMP   .and_end@14
.and_false@13:
   LDA   #$00
   LDY   #$57
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@14:
   LDY   #$84
   LDA   (FP),Y
   LDY   #$59
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$3A
   LDA   (FP),Y
   LDY   #$5D
   STA   (FP),Y
   CLC
   LDA   FP
   ADC   #$55
   INY
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$09
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDY   #$57
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$58
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$59
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$5A
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   LDY   #$5B
   LDA   (FP),Y
   LDY   #$05
   STA   (SSP),Y
   LDY   #$5C
   LDA   (FP),Y
   LDY   #$06
   STA   (SSP),Y
   LDY   #$5D
   LDA   (FP),Y
   LDY   #$07
   STA   (SSP),Y
   LDY   #$5E
   LDA   (FP),Y
   LDY   #$08
   STA   (SSP),Y
   LDY   #$5F
   LDA   (FP),Y
   LDY   #$09
   STA   (SSP),Y
   JSR   f2
   LDY   #$60
   STA   (FP),Y
   TXA
   INY
   STA   (FP),Y
   LDY   #$3D
   LDA   (FP),Y
   LDY   #$62
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$60
   EOR   (FP),Y
   LDY   #$64
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$61
   EOR   (FP),Y
   LDY   #$65
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$66
   STA   (FP),Y
   LDY   #$3D
   STA   (FP),Y
   LDY   #$67
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   LDY   #$55
   ADC   (FP),Y
   LDY   #$69
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$56
   ADC   (FP),Y
   LDY   #$6A
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$6B
   STA   (FP),Y
   LDY   #$3D
   STA   (FP),Y
   LDY   #$6C
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDA   g1
   CLC
   DEY
   ADC   (FP),Y
   LDY   #$6E
   STA   (FP),Y
   LDA   g1+1
   DEY
   ADC   (FP),Y
   LDY   #$6F
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   g1
   INY
   LDA   (FP),Y
   STA   g1+1
   LDY   #$80
   LDA   (FP),Y
   LDY   #$70
   STA   (FP),Y
   LDY   #$81
   LDA   (FP),Y
   LDY   #$71
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
   LDY   #$70
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$71
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   JSR   rec
   LDY   #$72
   STA   (FP),Y
   TXA
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$74
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$75
   STA   (FP),Y
   LDY   #$3A
   LDA   (FP),Y
   CLC
   LDY   #$74
   ADC   (FP),Y
   LDY   #$76
   STA   (FP),Y
   LDY   #$3B
   LDA   (FP),Y
   LDY   #$75
   ADC   (FP),Y
   LDY   #$77
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$3A
   STA   (FP),Y
   LDY   #$77
   LDA   (FP),Y
   LDY   #$3B
   STA   (FP),Y
   LDY   #$80
   LDA   (FP),Y
   LDY   #$78
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
   LDY   #$79
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
   ADC   #$86
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

   ; prologue: 0 arg bytes, 40 local bytes
   SEC
   LDA   SSP
   SBC   #$2A
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$29
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
   LDA   #$F4
   INY
   STA   (FP),Y
   LDA   #$95
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   ORA   #$00
   BPL   .sx_done@52
   LDA   #$FF
.sx_done@52:
   LDY   #$06
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$AD
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
   SBC   #$08
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
   JSR   f0
   LDA   HARGS+8
   LDY   #$0B
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
   LDA   g1
   INY
   STA   (FP),Y
   LDA   g1+1
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   g0
   LDY   #$13
   EOR   (FP),Y
   LDY   #$17
   STA   (FP),Y
   LDA   g0+1
   LDY   #$14
   EOR   (FP),Y
   LDY   #$18
   STA   (FP),Y
   LDA   g0+2
   LDY   #$15
   EOR   (FP),Y
   LDY   #$19
   STA   (FP),Y
   LDA   g0+3
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
   LDY   #$12
   LDA   (FP),Y
   LDY   #$1A
   EOR   (FP),Y
   LDY   #$1E
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
   LDY   #$1E
   LDA   (FP),Y
   LDY   #$12
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
   LDA   #$10
   STA   HARGS+4
   JSR   asr32
   LDA   HARGS+5
   LDY   #$1F
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
   LDY   #$12
   LDA   (FP),Y
   LDY   #$22
   EOR   (FP),Y
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
   TAX
   DEY
   LDA   (FP),Y

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$2A
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$29
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
   DC.L  $0000004C

g1:
   DC.W  $007C
