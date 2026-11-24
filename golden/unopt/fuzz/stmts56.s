SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
z3	EQU	$26
z4	EQU	$27
z5	EQU	$29

mix:
   SUBROUTINE

   ; prologue: 4 arg bytes, 18 local bytes
   SEC
   LDA   SSP
   SBC   #$14
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$13
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   LDA   acc
   LDY   #$01
   STA   (FP),Y
   ASL
   STA   (FP),Y
   LDA   acc+1
   INY
   STA   (FP),Y
   ROL
   STA   (FP),Y
   LDA   acc+1
   ASL
   LDA   #$00
   ROL
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$01
   LDA   (FP),Y
   LDY   #$03
   ORA   (FP),Y
   LDY   #$05
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   LDY   #$04
   ORA   (FP),Y
   LDY   #$06
   STA   (FP),Y
   LDY   #$15
   LDA   (FP),Y
   LDY   #$07
   STA   (FP),Y
   LDY   #$16
   LDA   (FP),Y
   LDY   #$08
   STA   (FP),Y
   LDY   #$05
   LDA   (FP),Y
   LDY   #$07
   EOR   (FP),Y
   LDY   #$09
   STA   (FP),Y
   LDY   #$06
   LDA   (FP),Y
   LDY   #$08
   EOR   (FP),Y
   LDY   #$0A
   STA   (FP),Y
   LDY   #$17
   LDA   (FP),Y
   LDY   #$0B
   STA   (FP),Y
   LDY   #$18
   LDA   (FP),Y
   LDY   #$0C
   STA   (FP),Y
   LDA   #$00
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
   LDY   #$09
   LDA   (FP),Y
   LDY   #$0F
   EOR   (FP),Y
   LDY   #$11
   STA   (FP),Y
   LDY   #$0A
   LDA   (FP),Y
   LDY   #$10
   EOR   (FP),Y
   LDY   #$12
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   acc
   INY
   LDA   (FP),Y
   STA   acc+1

   ; epilogue
   CLC
   LDA   FP
   ADC   #$18
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

fin0:
   SUBROUTINE

   ; prologue: 0 arg bytes, 20 local bytes
   SEC
   LDA   SSP
   SBC   #$16
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$15
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
   SEC
   LDA   SSP
   SBC   #$04
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
   JSR   mix
   LDA   g2
   LDY   #$07
   STA   (FP),Y
   LDA   g2+1
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$04
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDY   #$07
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$08
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$09
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$0A
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   JSR   mix
   LDA   z3
   LDY   #$0B
   STA   (FP),Y
   LDA   z3
   BMI   .sx_neg@2
   LDA   #$00
   JMP   .sx_done@3
.sx_neg@2:
   LDA   #$FF
.sx_done@3:
   LDY   #$0C
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$04
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDY   #$0B
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$0C
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$0D
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$0E
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   JSR   mix
   LDA   z4
   LDY   #$0F
   STA   (FP),Y
   LDA   z4+1
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
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$04
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDY   #$11
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$12
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$13
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$14
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   JSR   mix

   ; epilogue
   CLC
   LDA   FP
   ADC   #$16
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$15
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   RTS

fin1:
   SUBROUTINE

   ; prologue: 0 arg bytes, 15 local bytes
   SEC
   LDA   SSP
   SBC   #$11
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$10
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
   SBC   #$04
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   z5
   LDY   #$01
   STA   (SSP),Y
   LDA   z5+1
   INY
   STA   (SSP),Y
   LDA   z5+2
   INY
   STA   (SSP),Y
   LDA   z5+3
   INY
   STA   (SSP),Y
   JSR   mix
   LDA   ga6
   LDY   #$01
   STA   (FP),Y
   INY
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@4
   LDA   #$00
   JMP   .sx_done@5
.sx_neg@4:
   LDA   #$FF
.sx_done@5:
   LDY   #$03
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$04
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDY   #$02
   LDA   (FP),Y
   DEY
   STA   (SSP),Y
   LDY   #$03
   LDA   (FP),Y
   DEY
   STA   (SSP),Y
   LDY   #$04
   LDA   (FP),Y
   DEY
   STA   (SSP),Y
   LDY   #$05
   LDA   (FP),Y
   DEY
   STA   (SSP),Y
   JSR   mix
   LDA   ga6+1
   LDY   #$06
   STA   (FP),Y
   INY
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@6
   LDA   #$00
   JMP   .sx_done@7
.sx_neg@6:
   LDA   #$FF
.sx_done@7:
   LDY   #$08
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$04
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDY   #$07
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$08
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$09
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$0A
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   JSR   mix
   LDA   ga6+2
   LDY   #$0B
   STA   (FP),Y
   INY
   STA   (FP),Y
   ORA   #$00
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
   SEC
   LDA   SSP
   SBC   #$04
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDY   #$0C
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$0D
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$0E
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$0F
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   JSR   mix

   ; epilogue
   CLC
   LDA   FP
   ADC   #$11
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$10
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   RTS

fin2:
   SUBROUTINE

   ; prologue: 0 arg bytes, 25 local bytes
   SEC
   LDA   SSP
   SBC   #$1B
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$1A
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   LDA   ga6+3
   LDY   #$01
   STA   (FP),Y
   INY
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@10
   LDA   #$00
   JMP   .sx_done@11
.sx_neg@10:
   LDA   #$FF
.sx_done@11:
   LDY   #$03
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$04
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDY   #$02
   LDA   (FP),Y
   DEY
   STA   (SSP),Y
   LDY   #$03
   LDA   (FP),Y
   DEY
   STA   (SSP),Y
   LDY   #$04
   LDA   (FP),Y
   DEY
   STA   (SSP),Y
   LDY   #$05
   LDA   (FP),Y
   DEY
   STA   (SSP),Y
   JSR   mix
   LDA   #<gs7
   LDY   #$06
   STA   (FP),Y
   LDA   #>gs7
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$01
   LDY   #$08
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$09
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$0A
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
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
   ORA   #$00
   BMI   .sx_neg@12
   LDA   #$00
   JMP   .sx_done@13
.sx_neg@12:
   LDA   #$FF
.sx_done@13:
   LDY   #$0E
   STA   (FP),Y
   INY
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$04
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDY   #$0C
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$0D
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$0E
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$0F
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   JSR   mix
   LDA   #<gs7
   LDY   #$10
   STA   (FP),Y
   LDA   #>gs7
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$03
   LDY   #$12
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$13
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$14
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$15
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$16
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$17
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$04
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDY   #$16
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$17
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$18
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$19
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   JSR   mix

   ; epilogue
   CLC
   LDA   FP
   ADC   #$1B
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$1A
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   RTS

f9:
   SUBROUTINE

   ; prologue: 10 arg bytes, 123 local bytes
   SEC
   LDA   SSP
   SBC   #$7D
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$7C
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
   LDA   #$01
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
   INY
   STA   (FP),Y
   LDA   #$80
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
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
.loop@0_start:
   LDY   #$11
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   SEC
   SBC   #$02
   INY
   LDA   (FP),Y
   SBC   #$00
   BVC   .cmp_novf@14
   EOR   #$80
.cmp_novf@14:
   ASL
   LDA   #$00
   ROL
   LDY   #$13
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@4
   JMP   .loop@0_break
.lb_skip@4:
   LDY   #$82
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
   LDA   #$00
   STA   HARGS+6
   STA   HARGS+7
   LDY   #$05
   STY   HARGS+8
   LDY   #$00
   STY   HARGS+9
   STY   HARGS+10
   STY   HARGS+11
   STY   HARGS+12
   STY   HARGS+13
   STA   HARGS+14
   STA   HARGS+15
   JSR   mul64
   LDA   HARGS+16
   LDY   #$15
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
   LDY   #$15
   LDA   (FP),Y
   LDY   #$82
   STA   (FP),Y
   LDY   #$16
   LDA   (FP),Y
   LDY   #$83
   STA   (FP),Y
   LDY   #$17
   LDA   (FP),Y
   LDY   #$84
   STA   (FP),Y
   LDY   #$18
   LDA   (FP),Y
   LDY   #$85
   STA   (FP),Y
   LDY   #$19
   LDA   (FP),Y
   LDY   #$86
   STA   (FP),Y
   LDY   #$1A
   LDA   (FP),Y
   LDY   #$87
   STA   (FP),Y
   LDA   z3
   SEC
   SBC   #$01
   LDY   #$1B
   STA   (FP),Y
   STA   z3
   LDA   #$02
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$1E
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$1F
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@15
   LDA   #$00
   JMP   .sx_done@16
.sx_neg@15:
   LDA   #$FF
.sx_done@16:
   LDY   #$20
   STA   (FP),Y
   INY
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
   LDY   #$1E
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
   LDY   #$22
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
   LDA   #$02
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$28
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$29
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@17
   LDA   #$00
   JMP   .sx_done@18
.sx_neg@17:
   LDA   #$FF
.sx_done@18:
   LDY   #$2A
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$22
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
   JSR   udivmod32
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
   LDY   #$2C
   LDA   (FP),Y
   STA   z5
   INY
   LDA   (FP),Y
   STA   z5+1
   INY
   LDA   (FP),Y
   STA   z5+2
   INY
   LDA   (FP),Y
   STA   z5+3
   LDY   #$10
   LDA   (FP),Y
   LDY   #$30
   STA   (FP),Y
   CLC
   ADC   #$01
   INY
   STA   (FP),Y
   LDY   #$10
   STA   (FP),Y
   JMP   .loop@0_start
.loop@0_break:
   LDA   z5
   CLC
   ADC   #$01
   LDY   #$32
   STA   (FP),Y
   LDA   z5+1
   ADC   #$00
   INY
   STA   (FP),Y
   LDA   z5+2
   ADC   #$00
   INY
   STA   (FP),Y
   LDA   z5+3
   ADC   #$00
   INY
   STA   (FP),Y
   LDY   #$32
   LDA   (FP),Y
   STA   z5
   INY
   LDA   (FP),Y
   STA   z5+1
   INY
   LDA   (FP),Y
   STA   z5+2
   INY
   LDA   (FP),Y
   STA   z5+3
   LDA   g1
   INY
   STA   (FP),Y
   LDA   g1+1
   INY
   STA   (FP),Y
   LDA   #$07
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$36
   LDA   (FP),Y
   LDY   #$38
   AND   (FP),Y
   LDY   #$3A
   STA   (FP),Y
   LDY   #$37
   LDA   (FP),Y
   LDY   #$39
   AND   (FP),Y
   LDY   #$3B
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CMP   #$01
   BEQ   .cmp_true@19
   LDA   #$00
   JMP   .cmp_end@20
.cmp_true@19:
   LDA   #$01
.cmp_end@20:
   LDY   #$3C
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .case@2
   LDY   #$3A
   LDA   (FP),Y
   CMP   #$02
   BEQ   .cmp_true@21
   LDA   #$00
   JMP   .cmp_end@22
.cmp_true@21:
   LDA   #$01
.cmp_end@22:
   LDY   #$3E
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@3
   JMP   .case@3
.lb_skip@3:
   JMP   .default@9
.case@2:
   LDA   z4
   LDY   #$40
   STA   (FP),Y
   LDA   z4+1
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   HARGS
   INY
   LDA   (FP),Y
   STA   HARGS+1
   LDA   #$81
   STA   HARGS+2
   LDA   #$00
   STA   HARGS+3
   JSR   mul16
   LDA   HARGS+4
   LDY   #$42
   STA   (FP),Y
   LDA   HARGS+5
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   z4
   INY
   LDA   (FP),Y
   STA   z4+1
   DEY
   LDA   (FP),Y
   LDY   #$44
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$45
   STA   (FP),Y
   LDA   #<gs7
   INY
   STA   (FP),Y
   LDA   #>gs7
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$01
   LDY   #$48
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$49
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$44
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$45
   LDA   (FP),Y
   LDY   #$01
   STA   (DPTR),Y
   LDA   ga6+2
   LDY   #$4A
   STA   (FP),Y
   ORA   #$00
   BEQ   .cond_else@0
   LDA   g1
   INY
   STA   (FP),Y
   LDA   g1+1
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$4D
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$4E
   STA   (FP),Y
   JMP   .cond_end@1
.cond_else@0:
   LDA   #$7F
   LDY   #$4F
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$4D
   STA   (FP),Y
   LDY   #$51
   LDA   (FP),Y
   LDY   #$4E
   STA   (FP),Y
.cond_end@1:
   LDY   #$4D
   LDA   (FP),Y
   LDY   #$52
   STA   (FP),Y
   LDY   #$4E
   LDA   (FP),Y
   LDY   #$53
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@23
   LDA   #$00
   JMP   .sx_done@24
.sx_neg@23:
   LDA   #$FF
.sx_done@24:
   LDY   #$54
   STA   (FP),Y
   INY
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$04
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDY   #$52
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$53
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$54
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$55
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   JSR   mix
.case@3:
   LDA   z3
   LDY   #$56
   STA   (FP),Y
   LDA   z3
   BMI   .sx_neg@25
   LDA   #$00
   JMP   .sx_done@26
.sx_neg@25:
   LDA   #$FF
.sx_done@26:
   LDY   #$57
   STA   (FP),Y
   LDA   #$07
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$56
   LDA   (FP),Y
   LDY   #$58
   AND   (FP),Y
   LDY   #$5A
   STA   (FP),Y
   LDY   #$57
   LDA   (FP),Y
   LDY   #$59
   AND   (FP),Y
   LDY   #$5B
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   BEQ   .cmp_true@27
   LDA   #$00
   JMP   .cmp_end@28
.cmp_true@27:
   LDA   #$01
.cmp_end@28:
   LDY   #$5C
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .case@5
   LDY   #$5A
   LDA   (FP),Y
   CMP   #$05
   BEQ   .cmp_true@29
   LDA   #$00
   JMP   .cmp_end@30
.cmp_true@29:
   LDA   #$01
.cmp_end@30:
   LDY   #$5E
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@2
   JMP   .case@7
.lb_skip@2:
   LDY   #$5A
   LDA   (FP),Y
   CMP   #$06
   BEQ   .cmp_true@31
   LDA   #$00
   JMP   .cmp_end@32
.cmp_true@31:
   LDA   #$01
.cmp_end@32:
   LDY   #$60
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@1
   JMP   .case@8
.lb_skip@1:
   JMP   .switch@4_break
.case@5:
   LDA   #$03
   LDY   #$62
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
   INY
   STA   (FP),Y
   SEC
   LDY   #$62
   SBC   (FP),Y
   LDY   #$67
   LDA   (FP),Y
   LDY   #$63
   SBC   (FP),Y
   LDY   #$68
   LDA   (FP),Y
   LDY   #$64
   SBC   (FP),Y
   LDY   #$69
   LDA   (FP),Y
   LDY   #$65
   SBC   (FP),Y
   LDA   #$00
   ROL
   LDY   #$6A
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$6C
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$6D
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   g2
   INY
   LDA   (FP),Y
   STA   g2+1
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
.loop@6_start:
   LDA   gp8
   STA   DPTR
   LDA   gp8+1
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$70
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$71
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ORA   #$05
   LDY   #$72
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$73
   STA   (FP),Y
   LDA   gp8
   STA   DPTR
   LDA   gp8+1
   STA   DPTR+1
   DEY
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$73
   LDA   (FP),Y
   LDY   #$01
   STA   (DPTR),Y
   LDY   #$6F
   LDA   (FP),Y
   CLC
   ADC   #$01
   LDY   #$74
   STA   (FP),Y
   LDY   #$6F
   STA   (FP),Y
   LDY   #$75
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   SEC
   SBC   #$03
   INY
   LDA   (FP),Y
   SBC   #$00
   BVC   .cmp_novf@33
   EOR   #$80
.cmp_novf@33:
   ASL
   LDA   #$00
   ROL
   LDY   #$77
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@0
   JMP   .loop@6_start
.lb_skip@0:
.case@7:
.case@8:
.switch@4_break:
.default@9:
   LDA   z4
   LDY   #$79
   STA   (FP),Y
   LDA   z4+1
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$7B
   STA   (FP),Y

   ; epilogue
   PHA
   CLC
   LDA   FP
   ADC   #$87
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

f16:
   SUBROUTINE

   ; prologue: 14 arg bytes, 146 local bytes
   SEC
   LDA   SSP
   SBC   #$94
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$93
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
   LDA   #$01
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
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$01
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
   LDA   g2
   INY
   STA   (FP),Y
   ASL
   STA   (FP),Y
   LDA   g2+1
   INY
   STA   (FP),Y
   ROL
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ASL
   STA   (FP),Y
   INY
   LDA   (FP),Y
   ROL
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   g2
   INY
   LDA   (FP),Y
   STA   g2+1
   DEY
   LDA   (FP),Y
   STA   z4
   INY
   LDA   (FP),Y
   STA   z4+1
   LDA   ga6
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@34
   LDA   #$00
   JMP   .sx_done@35
.sx_neg@34:
   LDA   #$FF
.sx_done@35:
   LDY   #$11
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
   SEC
   LDY   #$95
   SBC   (FP),Y
   LDY   #$14
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$96
   SBC   (FP),Y
   LDY   #$15
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$16
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$17
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$A6
   INY
   STA   (FP),Y
   INY
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
   LDY   #$1B
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
   LDY   #$1F
   STA   (FP),Y
   LDA   HARGS+7
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
   BMI   .sx_neg@36
   LDA   #$00
   JMP   .sx_done@37
.sx_neg@36:
   LDA   #$FF
.sx_done@37:
   LDY   #$23
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$0A
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDY   #$16
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$17
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$18
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$19
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   LDY   #$21
   LDA   (FP),Y
   LDY   #$05
   STA   (SSP),Y
   LDY   #$22
   LDA   (FP),Y
   LDY   #$06
   STA   (SSP),Y
   LDY   #$23
   LDA   (FP),Y
   LDY   #$07
   STA   (SSP),Y
   LDY   #$24
   LDA   (FP),Y
   LDY   #$08
   STA   (SSP),Y
   LDY   #$25
   LDA   (FP),Y
   LDY   #$09
   STA   (SSP),Y
   LDY   #$26
   LDA   (FP),Y
   LDY   #$0A
   STA   (SSP),Y
   JSR   f9
   LDY   #$27
   STA   (FP),Y
   INY
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@38
   LDA   #$00
   JMP   .sx_done@39
.sx_neg@38:
   LDA   #$FF
.sx_done@39:
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
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$28
   LDA   (FP),Y
   LDY   #$30
   STA   (FP),Y
   LDY   #$29
   LDA   (FP),Y
   LDY   #$31
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
   AND   #$0F
   INY
   STA   (FP),Y
   LDY   #$30
   LDA   (FP),Y
   LDY   #$05
   STA   (FP),Y
   LDY   #$31
   LDA   (FP),Y
   LDY   #$06
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
   LDY   #$37
   LDA   (FP),Y
   LDY   #$0C
   STA   (FP),Y
   LDA   #$00
   LDY   #$38
   STA   (FP),Y
   INY
   STA   (FP),Y
.loop@10_continue:
   LDY   #$39
   LDA   (FP),Y
   INY
   STA   (FP),Y
   CLC
   ADC   #$01
   INY
   STA   (FP),Y
   LDY   #$39
   STA   (FP),Y
   INY
   LDA   (FP),Y
   LDY   #$3C
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   SEC
   SBC   #$02
   INY
   LDA   (FP),Y
   SBC   #$00
   BVC   .cmp_novf@40
   EOR   #$80
.cmp_novf@40:
   ASL
   LDA   #$00
   ROL
   LDY   #$3E
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@6
   JMP   .loop@10_break
.lb_skip@6:
   LDA   #$00
   LDY   #$40
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   SEC
   SBC   #$01
   LDA   (FP),Y
   SBC   #$00
   BVC   .cmp_novf@41
   EOR   #$80
.cmp_novf@41:
   ASL
   LDA   #$00
   ROL
   LDY   #$44
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@5
   JMP   .loop@11_break
.lb_skip@5:
   LDA   z3
   LDY   #$46
   STA   (FP),Y
   LDA   z3
   BMI   .sx_neg@42
   LDA   #$00
   JMP   .sx_done@43
.sx_neg@42:
   LDA   #$FF
.sx_done@43:
   LDY   #$47
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   EOR   #$81
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
   STA   z3
   LDA   (FP),Y
   INY
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@44
   LDA   #$00
   JMP   .sx_done@45
.sx_neg@44:
   LDA   #$FF
.sx_done@45:
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
   BMI   .sx_neg@46
   LDA   #$00
   JMP   .sx_done@47
.sx_neg@46:
   LDA   #$FF
.sx_done@47:
   LDY   #$4F
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$97
   LDA   (FP),Y
   LDY   #$4D
   EOR   (FP),Y
   LDY   #$53
   STA   (FP),Y
   LDY   #$98
   LDA   (FP),Y
   LDY   #$4E
   EOR   (FP),Y
   LDY   #$54
   STA   (FP),Y
   LDY   #$99
   LDA   (FP),Y
   LDY   #$4F
   EOR   (FP),Y
   LDY   #$55
   STA   (FP),Y
   LDY   #$9A
   LDA   (FP),Y
   LDY   #$50
   EOR   (FP),Y
   LDY   #$56
   STA   (FP),Y
   LDY   #$9B
   LDA   (FP),Y
   LDY   #$51
   EOR   (FP),Y
   LDY   #$57
   STA   (FP),Y
   LDY   #$9C
   LDA   (FP),Y
   LDY   #$52
   EOR   (FP),Y
   LDY   #$58
   STA   (FP),Y
   LDY   #$53
   LDA   (FP),Y
   LDY   #$59
   STA   (FP),Y

   ; epilogue
   PHA
   CLC
   LDA   FP
   ADC   #$A2
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$93
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   PLA
   RTS
.loop@11_break:
   LDA   gp8
   STA   DPTR
   LDA   gp8+1
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$8E
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$8F
   STA   (FP),Y
   LDY   #$91
   STA   (FP),Y
   LSR
   STA   (FP),Y
   LDY   #$8E
   LDA   (FP),Y
   LDY   #$90
   STA   (FP),Y
   ROR
   STA   (FP),Y
   LDA   gp8
   STA   DPTR
   LDA   gp8+1
   STA   DPTR+1
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$91
   LDA   (FP),Y
   LDY   #$01
   STA   (DPTR),Y
   JMP   .loop@10_continue
.loop@10_break:
   LDY   #$05
   LDA   (FP),Y
   LDY   #$92
   STA   (FP),Y

   ; epilogue
   PHA
   CLC
   LDA   FP
   ADC   #$A2
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

f26:
   SUBROUTINE

   ; prologue: 3 arg bytes, 95 local bytes
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

   LDA   #$00
   LDY   #$01
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
   LDA   #$FF
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
   LDA   #$80
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$0F
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$10
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
.loop@20_continue:
   LDY   #$12
   LDA   (FP),Y
   INY
   STA   (FP),Y
   CLC
   ADC   #$01
   INY
   STA   (FP),Y
   LDY   #$12
   STA   (FP),Y
   INY
   LDA   (FP),Y
   LDY   #$15
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   SEC
   SBC   #$04
   INY
   LDA   (FP),Y
   SBC   #$00
   BVC   .cmp_novf@60
   EOR   #$80
.cmp_novf@60:
   ASL
   LDA   #$00
   ROL
   LDY   #$17
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@9
   JMP   .loop@20_break
.lb_skip@9:
   LDY   #$62
   LDA   (FP),Y
   LDY   #$19
   STA   (FP),Y
   CLC
   ADC   #$01
   INY
   STA   (FP),Y
   LDY   #$62
   STA   (FP),Y
   LDY   #$19
   LDA   (FP),Y
   BNE   .lb_skip@8
   JMP   .if_else@3
.lb_skip@8:
   LDY   #$62
   LDA   (FP),Y
   LDY   #$1B
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$1D
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$1E
   STA   (FP),Y
   LDA   g2+1
   CMP   (FP),Y
   BNE   .cmp_true@61
   LDA   g2
   DEY
   CMP   (FP),Y
   BNE   .cmp_true@61
   LDA   #$00
   JMP   .cmp_end@62
.cmp_true@61:
   LDA   #$01
.cmp_end@62:
   LDY   #$1F
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDA   #<gs7
   INY
   STA   (FP),Y
   LDA   #>gs7
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$01
   LDY   #$23
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$24
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$1F
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$20
   LDA   (FP),Y
   LDY   #$01
   STA   (DPTR),Y
   LDY   #$12
   LDA   (FP),Y
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
   LDA   z4
   INY
   STA   (FP),Y
   LDA   z4+1
   INY
   STA   (FP),Y
   LDY   #$27
   LDA   (FP),Y
   SEC
   LDY   #$29
   SBC   (FP),Y
   LDY   #$2B
   STA   (FP),Y
   LDY   #$28
   LDA   (FP),Y
   LDY   #$2A
   SBC   (FP),Y
   LDY   #$2C
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$2D
   STA   (FP),Y

   ; epilogue
   PHA
   CLC
   LDA   FP
   ADC   #$64
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$60
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   PLA
   RTS
.if_else@3:
   LDA   #$00
   LDY   #$2E
   STA   (FP),Y
   INY
   STA   (FP),Y
.loop@21_continue:
   LDY   #$2F
   LDA   (FP),Y
   INY
   STA   (FP),Y
   CLC
   ADC   #$01
   INY
   STA   (FP),Y
   LDY   #$2F
   STA   (FP),Y
   INY
   LDA   (FP),Y
   LDY   #$32
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   SEC
   SBC   #$04
   INY
   LDA   (FP),Y
   SBC   #$00
   BVC   .cmp_novf@64
   EOR   #$80
.cmp_novf@64:
   ASL
   LDA   #$00
   ROL
   LDY   #$34
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .loop@21_continue
   LDA   #<gs7
   INY
   STA   (FP),Y
   LDA   #>gs7
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$03
   LDY   #$38
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$39
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$3A
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$3B
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   SEC
   SBC   #$01
   LDY   #$3C
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   SBC   #$00
   LDY   #$3D
   STA   (FP),Y
   LDY   #$38
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$3C
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$3D
   LDA   (FP),Y
   LDY   #$01
   STA   (DPTR),Y
   LDY   #$3A
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@7
   JMP   .loop@20_continue
.lb_skip@7:
   LDY   #$06
   LDA   (FP),Y
   LDY   #$3E
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@65
   LDA   #$00
   JMP   .sx_done@66
.sx_neg@65:
   LDA   #$FF
.sx_done@66:
   LDY   #$3F
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$04
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDY   #$3E
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
   LDY   #$41
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   JSR   mix
   LDA   z5
   SEC
   SBC   #$01
   LDY   #$42
   STA   (FP),Y
   LDA   z5+1
   SBC   #$00
   INY
   STA   (FP),Y
   LDA   z5+2
   SBC   #$00
   INY
   STA   (FP),Y
   LDA   z5+3
   SBC   #$00
   INY
   STA   (FP),Y
   LDY   #$42
   LDA   (FP),Y
   STA   z5
   INY
   LDA   (FP),Y
   STA   z5+1
   INY
   LDA   (FP),Y
   STA   z5+2
   INY
   LDA   (FP),Y
   STA   z5+3
   LDY   #$06
   LDA   (FP),Y
   LDY   #$46
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@67
   LDA   #$00
   JMP   .sx_done@68
.sx_neg@67:
   LDA   #$FF
.sx_done@68:
   LDY   #$47
   STA   (FP),Y
   LDA   #$07
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$46
   LDA   (FP),Y
   LDY   #$48
   AND   (FP),Y
   LDY   #$4A
   STA   (FP),Y
   LDY   #$47
   LDA   (FP),Y
   LDY   #$49
   AND   (FP),Y
   LDY   #$4B
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CMP   #$03
   BEQ   .cmp_true@69
   LDA   #$00
   JMP   .cmp_end@70
.cmp_true@69:
   LDA   #$01
.cmp_end@70:
   LDY   #$4C
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .default@24
   LDY   #$06
   LDA   (FP),Y
   LDY   #$4E
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@71
   LDA   #$00
   JMP   .sx_done@72
.sx_neg@71:
   LDA   #$FF
.sx_done@72:
   LDY   #$4F
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   HARGS
   INY
   LDA   (FP),Y
   STA   HARGS+1
   LDA   #$05
   STA   HARGS+2
   LDA   #$00
   STA   HARGS+3
   JSR   mul16
   LDA   HARGS+4
   LDY   #$50
   STA   (FP),Y
   LDA   HARGS+5
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$52
   STA   (FP),Y
   LDY   #$06
   STA   (FP),Y
   LDY   #$53
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@73
   LDA   #$00
   JMP   .sx_done@74
.sx_neg@73:
   LDA   #$FF
.sx_done@74:
   LDY   #$54
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   g2
   INY
   LDA   (FP),Y
   STA   g2+1
.default@24:
   LDY   #$12
   LDA   (FP),Y
   LDY   #$55
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
   SBC   #$04
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
   JSR   mix
   JMP   .loop@20_continue
.loop@20_break:
   LDA   #<gs7
   LDY   #$59
   STA   (FP),Y
   LDA   #>gs7
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$03
   LDY   #$5B
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$5C
   STA   (FP),Y
   DEY
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
   LDY   #$5F
   STA   (FP),Y

   ; epilogue
   PHA
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
   PLA
   RTS

main:
   SUBROUTINE

   ; prologue: 0 arg bytes, 77 local bytes
   SEC
   LDA   SSP
   SBC   #$4F
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$4E
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
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$F3
   INY
   STA   (FP),Y
   LDA   #$2F
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
   LDA   #$02
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
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
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   ga6+1
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@75
   LDA   #$00
   JMP   .sx_done@76
.sx_neg@75:
   LDA   #$FF
.sx_done@76:
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
   LDA   z4
   INY
   STA   (FP),Y
   LDA   z4+1
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ORA   #$01
   LDY   #$18
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$19
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   z4
   INY
   LDA   (FP),Y
   STA   z4+1
   LDY   #$14
   LDA   (FP),Y
   LDY   #$18
   EOR   (FP),Y
   LDY   #$1A
   STA   (FP),Y
   LDY   #$15
   LDA   (FP),Y
   LDY   #$19
   EOR   (FP),Y
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
   LDA   #$07
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$1C
   LDA   (FP),Y
   LDY   #$1E
   AND   (FP),Y
   LDY   #$20
   STA   (FP),Y
   LDY   #$1D
   LDA   (FP),Y
   LDY   #$1F
   AND   (FP),Y
   LDY   #$21
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CMP   #$01
   BEQ   .cmp_true@77
   LDA   #$00
   JMP   .cmp_end@78
.cmp_true@77:
   LDA   #$01
.cmp_end@78:
   LDY   #$22
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@11
   JMP   .switch@25_break
.lb_skip@11:
   LDY   #$20
   LDA   (FP),Y
   CMP   #$04
   BEQ   .cmp_true@79
   LDA   #$00
   JMP   .cmp_end@80
.cmp_true@79:
   LDA   #$01
.cmp_end@80:
   LDY   #$24
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .case@27
   LDY   #$20
   LDA   (FP),Y
   CMP   #$06
   BEQ   .cmp_true@81
   LDA   #$00
   JMP   .cmp_end@82
.cmp_true@81:
   LDA   #$01
.cmp_end@82:
   LDY   #$26
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@10
   JMP   .case@28
.lb_skip@10:
   JMP   .switch@25_break
.case@27:
   LDA   #$FF
   LDY   #$28
   STA   (FP),Y
   LDA   #$7F
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$2A
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$2B
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   z5
   CLC
   LDY   #$2A
   ADC   (FP),Y
   LDY   #$2E
   STA   (FP),Y
   LDA   z5+1
   LDY   #$2B
   ADC   (FP),Y
   LDY   #$2F
   STA   (FP),Y
   LDA   z5+2
   LDY   #$2C
   ADC   (FP),Y
   LDY   #$30
   STA   (FP),Y
   LDA   z5+3
   LDY   #$2D
   ADC   (FP),Y
   LDY   #$31
   STA   (FP),Y
   LDY   #$2E
   LDA   (FP),Y
   LDY   #$32
   STA   (FP),Y
   CLC
   LDA   FP
   ADC   #$0D
   INY
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$03
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDY   #$32
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$33
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$34
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   JSR   f26
   LDY   #$35
   STA   (FP),Y
.case@28:
.switch@25_break:
   LDA   z3
   LDY   #$36
   STA   (FP),Y
   LDA   z3
   BMI   .sx_neg@83
   LDA   #$00
   JMP   .sx_done@84
.sx_neg@83:
   LDA   #$FF
.sx_done@84:
   LDY   #$37
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   z4
   INY
   LDA   (FP),Y
   STA   z4+1
   LDA   g2
   INY
   STA   (FP),Y
   LDA   g2+1
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$3A
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$3B
   STA   (FP),Y
   LDA   #<gs7
   INY
   STA   (FP),Y
   LDA   #>gs7
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$03
   LDY   #$3E
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$3F
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$40
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$41
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$FF
   LDY   #$42
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$43
   STA   (FP),Y
   LDY   #$3E
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$42
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$43
   LDA   (FP),Y
   LDY   #$01
   STA   (DPTR),Y
   LDY   #$3A
   LDA   (FP),Y
   LDY   #$42
   AND   (FP),Y
   LDY   #$44
   STA   (FP),Y
   LDY   #$3B
   LDA   (FP),Y
   LDY   #$43
   AND   (FP),Y
   LDY   #$45
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$46
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$47
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$04
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDY   #$46
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$47
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$48
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$49
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   JSR   mix
   JSR   fin0
   JSR   fin1
   JSR   fin2
   LDA   acc
   LDY   #$4A
   STA   (FP),Y
   LDA   acc+1
   AND   #$7F
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$4C
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$4D
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
   LDY   #$4E
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

acc:
   DS.B  2

g1:
   DS.B  2

g2:
   DC.W  $0003

ga6:
   DC.B  $FF, $FF, $01, $00

gs7:
   DS.B  1
   DC.W  $FB70
   DC.W  $000C

gp8:
   DC.W  g2

@11.s23:
   DS.B  8

__zp_init:
   SUBROUTINE

   LDA   #$00
   STA   z4
   STA   z5+1
   STA   z5+2
   STA   z5+3
   LDA   #$80
   STA   z3
   STA   z4+1
   STA   z5
   RTS
