SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
z3	EQU	$26
z4	EQU	$2A
z5	EQU	$2C

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

   ; prologue: 0 arg bytes, 12 local bytes
   SEC
   LDA   SSP
   SBC   #$0E
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$0D
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
   LDA   g1
   BMI   .sx_neg@0
   LDA   #$00
   JMP   .sx_done@1
.sx_neg@0:
   LDA   #$FF
.sx_done@1:
   LDY   #$02
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
   JSR   mix
   LDA   g2
   LDY   #$05
   STA   (FP),Y
   LDA   g2
   BMI   .sx_neg@2
   LDA   #$00
   JMP   .sx_done@3
.sx_neg@2:
   LDA   #$FF
.sx_done@3:
   LDY   #$06
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
   LDY   #$05
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$06
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$07
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$08
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   JSR   mix
   SEC
   LDA   SSP
   SBC   #$04
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   z3
   LDY   #$01
   STA   (SSP),Y
   LDA   z3+1
   INY
   STA   (SSP),Y
   LDA   z3+2
   INY
   STA   (SSP),Y
   LDA   z3+3
   INY
   STA   (SSP),Y
   JSR   mix
   LDA   z4
   LDY   #$09
   STA   (FP),Y
   LDA   z4+1
   INY
   STA   (FP),Y
   LDA   z4+1
   BMI   .sx_neg@4
   LDA   #$00
   JMP   .sx_done@5
.sx_neg@4:
   LDA   #$FF
.sx_done@5:
   LDY   #$0B
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
   LDY   #$09
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$0A
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
   JSR   mix

   ; epilogue
   CLC
   LDA   FP
   ADC   #$0E
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$0D
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

   ; prologue: 0 arg bytes, 18 local bytes
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
   LDA   ga6+1
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
   BMI   .sx_neg@6
   LDA   #$00
   JMP   .sx_done@7
.sx_neg@6:
   LDA   #$FF
.sx_done@7:
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
   LDA   ga6+2
   LDY   #$07
   STA   (FP),Y
   LDA   ga6+3
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
   BMI   .sx_neg@8
   LDA   #$00
   JMP   .sx_done@9
.sx_neg@8:
   LDA   #$FF
.sx_done@9:
   LDY   #$0B
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
   LDY   #$09
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$0A
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
   JSR   mix
   LDA   ga6+4
   LDY   #$0D
   STA   (FP),Y
   LDA   ga6+5
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
   ORA   #$00
   BMI   .sx_neg@10
   LDA   #$00
   JMP   .sx_done@11
.sx_neg@10:
   LDA   #$FF
.sx_done@11:
   LDY   #$11
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
   LDY   #$0F
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$10
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$11
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$12
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   JSR   mix

   ; epilogue
   CLC
   LDA   FP
   ADC   #$14
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$13
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

   ; prologue: 0 arg bytes, 29 local bytes
   SEC
   LDA   SSP
   SBC   #$1F
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$1E
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   LDA   ga6+6
   LDY   #$01
   STA   (FP),Y
   LDA   ga6+7
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
   BMI   .sx_neg@12
   LDA   #$00
   JMP   .sx_done@13
.sx_neg@12:
   LDA   #$FF
.sx_done@13:
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
   LDA   #<gs7
   LDY   #$07
   STA   (FP),Y
   LDA   #>gs7
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$01
   LDY   #$09
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$0A
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$0B
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$0C
   STA   (FP),Y
   LDY   #$02
   LDA   (DPTR),Y
   LDY   #$0D
   STA   (FP),Y
   LDY   #$03
   LDA   (DPTR),Y
   LDY   #$0E
   STA   (FP),Y
   LDY   #$04
   LDA   (DPTR),Y
   LDY   #$0F
   STA   (FP),Y
   LDY   #$05
   LDA   (DPTR),Y
   LDY   #$10
   STA   (FP),Y
   LDY   #$06
   LDA   (DPTR),Y
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
   LDY   #$0E
   LDA   (FP),Y
   LDY   #$15
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$04
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDY   #$12
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$13
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$14
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$15
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   JSR   mix
   LDA   #<gs7
   LDY   #$16
   STA   (FP),Y
   LDA   #>gs7
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$08
   LDY   #$18
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$19
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$1A
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$1B
   STA   (FP),Y
   LDY   #$02
   LDA   (DPTR),Y
   LDY   #$1C
   STA   (FP),Y
   LDY   #$03
   LDA   (DPTR),Y
   LDY   #$1D
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$04
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDY   #$1A
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$1B
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$1C
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$1D
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   JSR   mix

   ; epilogue
   CLC
   LDA   FP
   ADC   #$1F
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$1E
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

   ; prologue: 5 arg bytes, 244 local bytes
   SEC
   LDA   SSP
   SBC   #$F6
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$F5
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
   INY
   STA   (FP),Y
   LDA   #$80
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$05
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$06
   STA   (FP),Y
   LDA   g2
   INY
   STA   (FP),Y
   LDA   g2
   BMI   .sx_neg@14
   LDA   #$00
   JMP   .sx_done@15
.sx_neg@14:
   LDA   #$FF
.sx_done@15:
   LDY   #$08
   STA   (FP),Y
   LDA   #$06
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$07
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
   LDY   #$0B
   STA   (FP),Y
   LDA   HARGS+7
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@2
   JMP   .if_else@1
.lb_skip@2:
   LDA   #$00
   LDY   #$0D
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   SEC
   SBC   #$04
   LDA   (FP),Y
   SBC   #$00
   BVC   .cmp_novf@16
   EOR   #$80
.cmp_novf@16:
   ASL
   LDA   #$00
   ROL
   LDY   #$11
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@1
   JMP   .loop@0_break
.lb_skip@1:
   LDA   ga6+4
   LDY   #$13
   STA   (FP),Y
   LDA   ga6+5
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
   ORA   #$00
   BMI   .sx_neg@17
   LDA   #$00
   JMP   .sx_done@18
.sx_neg@17:
   LDA   #$FF
.sx_done@18:
   LDY   #$17
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
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
   LDY   #$1E
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$1F
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$20
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$21
   STA   (FP),Y
   LDY   #$02
   LDA   (DPTR),Y
   LDY   #$22
   STA   (FP),Y
   LDY   #$03
   LDA   (DPTR),Y
   LDY   #$23
   STA   (FP),Y
   LDY   #$04
   LDA   (DPTR),Y
   LDY   #$24
   STA   (FP),Y
   LDY   #$05
   LDA   (DPTR),Y
   LDY   #$25
   STA   (FP),Y
   LDY   #$06
   LDA   (DPTR),Y
   LDY   #$26
   STA   (FP),Y
   LDY   #$1B
   LDA   (FP),Y
   LDY   #$26
   CMP   (FP),Y
   BNE   .thread@0
   LDY   #$1A
   LDA   (FP),Y
   LDY   #$25
   CMP   (FP),Y
   BNE   .thread@0
   LDY   #$19
   LDA   (FP),Y
   LDY   #$24
   CMP   (FP),Y
   BNE   .thread@0
   LDY   #$18
   LDA   (FP),Y
   LDY   #$23
   CMP   (FP),Y
   BNE   .thread@0
   LDY   #$17
   LDA   (FP),Y
   LDY   #$22
   CMP   (FP),Y
   BNE   .thread@0
   LDY   #$16
   LDA   (FP),Y
   LDY   #$21
   CMP   (FP),Y
   BNE   .thread@0
   LDY   #$15
   LDA   (FP),Y
   LDY   #$20
   CMP   (FP),Y
   BEQ   .cmp_true@19
.thread@0:
   LDA   #$00
   JMP   .cmp_end@20
.cmp_true@19:
   LDA   #$01
.cmp_end@20:
   LDY   #$27
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$F9
   LDA   (FP),Y
   LDY   #$29
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   AND   #$03
   LDY   #$2B
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDA   #$00
   LDY   #$2C
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$2D
   STA   (FP),Y
   LDY   #$F9
   STA   (FP),Y
   LDY   #$2E
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$27
   LDA   (FP),Y
   SEC
   LDY   #$2E
   SBC   (FP),Y
   LDY   #$30
   STA   (FP),Y
   LDY   #$28
   LDA   (FP),Y
   LDY   #$2F
   SBC   (FP),Y
   LDY   #$31
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$32
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$33
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@22
   LDA   #$00
   JMP   .sx_done@23
.sx_neg@22:
   LDA   #$FF
.sx_done@23:
   LDY   #$34
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
   LDY   #$35
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   JSR   mix
   LDA   @4.s15
   CLC
   ADC   #$FF
   LDY   #$36
   STA   (FP),Y
   LDA   @4.s15+1
   ADC   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   @4.s15
   INY
   LDA   (FP),Y
   STA   @4.s15+1
   DEY
   LDA   (FP),Y
   LDY   #$38
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$39
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@24
   LDA   #$00
   JMP   .sx_done@25
.sx_neg@24:
   LDA   #$FF
.sx_done@25:
   LDY   #$3A
   STA   (FP),Y
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
   ADC   #$08
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
   LDY   #$38
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$39
   LDA   (FP),Y
   LDY   #$01
   STA   (DPTR),Y
   LDY   #$3A
   LDA   (FP),Y
   LDY   #$02
   STA   (DPTR),Y
   LDY   #$3B
   LDA   (FP),Y
   LDY   #$03
   STA   (DPTR),Y
   LDY   #$0E
   LDA   (FP),Y
   LDY   #$40
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
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
   DEY
   LDA   (FP),Y
   LDY   #$45
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@26
   LDA   #$00
   JMP   .sx_done@27
.sx_neg@26:
   LDA   #$FF
.sx_done@27:
   LDY   #$46
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$44
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
   ADC   #$FB
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$F5
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   RTS
.loop@0_break:
   LDA   #<gs7
   LDY   #$7F
   STA   (FP),Y
   LDA   #>gs7
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$01
   LDY   #$81
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$82
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$83
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$84
   STA   (FP),Y
   LDY   #$02
   LDA   (DPTR),Y
   LDY   #$85
   STA   (FP),Y
   LDY   #$03
   LDA   (DPTR),Y
   LDY   #$86
   STA   (FP),Y
   LDY   #$04
   LDA   (DPTR),Y
   LDY   #$87
   STA   (FP),Y
   LDY   #$05
   LDA   (DPTR),Y
   LDY   #$88
   STA   (FP),Y
   LDY   #$06
   LDA   (DPTR),Y
   LDY   #$89
   STA   (FP),Y
   LDA   #$03
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$8C
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$8D
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@30
   LDA   #$00
   JMP   .sx_done@31
.sx_neg@30:
   LDA   #$FF
.sx_done@31:
   LDY   #$8E
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$83
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
   LDY   #$8C
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
   LDY   #$93
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
   LDY   #$93
   LDA   (FP),Y
   LDY   #$9A
   STA   (FP),Y
   LDY   #$94
   LDA   (FP),Y
   LDY   #$9B
   STA   (FP),Y
   LDA   #$07
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$9A
   LDA   (FP),Y
   LDY   #$9C
   AND   (FP),Y
   LDY   #$9E
   STA   (FP),Y
   LDY   #$9B
   LDA   (FP),Y
   LDY   #$9D
   AND   (FP),Y
   LDY   #$9F
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CMP   #$02
   BEQ   .cmp_true@32
   LDA   #$00
   JMP   .cmp_end@33
.cmp_true@32:
   LDA   #$01
.cmp_end@33:
   LDY   #$A0
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@0
   JMP   .default@3
.lb_skip@0:
   LDA   #<gs7
   LDY   #$A2
   STA   (FP),Y
   LDA   #>gs7
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$08
   LDY   #$A4
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$A5
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$A6
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$A7
   STA   (FP),Y
   LDY   #$02
   LDA   (DPTR),Y
   LDY   #$A8
   STA   (FP),Y
   LDY   #$03
   LDA   (DPTR),Y
   LDY   #$A9
   STA   (FP),Y
   SEC
   LDA   #$00
   LDY   #$A6
   SBC   (FP),Y
   LDY   #$AA
   STA   (FP),Y
   LDA   #$00
   LDY   #$A7
   SBC   (FP),Y
   LDY   #$AB
   STA   (FP),Y
   LDA   #$00
   LDY   #$A8
   SBC   (FP),Y
   LDY   #$AC
   STA   (FP),Y
   LDA   #$00
   LDY   #$A9
   SBC   (FP),Y
   LDY   #$AD
   STA   (FP),Y
   LDY   #$F9
   LDA   (FP),Y
   LDY   #$AE
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   SEC
   SBC   #$5A
   LDY   #$B0
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   SBC   #$00
   LDY   #$B1
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$B2
   STA   (FP),Y
   LDY   #$F9
   STA   (FP),Y
   LDY   #$B3
   STA   (FP),Y
   LDA   #$00
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
   ORA   #$00
   BMI   .sx_neg@34
   LDA   #$00
   JMP   .sx_done@35
.sx_neg@34:
   LDA   #$FF
.sx_done@35:
   LDY   #$B7
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$AA
   LDA   (FP),Y
   LDY   #$B5
   AND   (FP),Y
   LDY   #$B9
   STA   (FP),Y
   LDY   #$AB
   LDA   (FP),Y
   LDY   #$B6
   AND   (FP),Y
   LDY   #$BA
   STA   (FP),Y
   LDY   #$AC
   LDA   (FP),Y
   LDY   #$B7
   AND   (FP),Y
   LDY   #$BB
   STA   (FP),Y
   LDY   #$AD
   LDA   (FP),Y
   LDY   #$B8
   AND   (FP),Y
   LDY   #$BC
   STA   (FP),Y
   LDY   #$B9
   LDA   (FP),Y
   STA   z3
   INY
   LDA   (FP),Y
   STA   z3+1
   INY
   LDA   (FP),Y
   STA   z3+2
   INY
   LDA   (FP),Y
   STA   z3+3
   LDY   #$02
   LDA   (FP),Y
   LDY   #$BD
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LSR
   DEY
   LDA   (FP),Y
   ROR
   LDY   #$BF
   STA   (FP),Y
   LDY   #$C1
   STA   (FP),Y
   LDY   #$02
   STA   (FP),Y
   LDY   #$C2
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
   LDY   #$C2
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$C3
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$C4
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$C5
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   JSR   mix
   JMP   .switch@1_break
.default@3:
   LDA   #$5A
   LDY   #$C6
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   ORA   #$00
   BPL   .sx_done@37
   LDA   #$FF
.sx_done@37:
   LDY   #$C8
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   z3
   LDY   #$C6
   EOR   (FP),Y
   LDY   #$CA
   STA   (FP),Y
   LDA   z3+1
   LDY   #$C7
   EOR   (FP),Y
   LDY   #$CB
   STA   (FP),Y
   LDA   z3+2
   LDY   #$C8
   EOR   (FP),Y
   LDY   #$CC
   STA   (FP),Y
   LDA   z3+3
   LDY   #$C9
   EOR   (FP),Y
   LDY   #$CD
   STA   (FP),Y
   LDY   #$CA
   LDA   (FP),Y
   STA   z3
   INY
   LDA   (FP),Y
   STA   z3+1
   INY
   LDA   (FP),Y
   STA   z3+2
   INY
   LDA   (FP),Y
   STA   z3+3
.switch@1_break:
   LDA   z4
   LDY   #$CE
   STA   (FP),Y
   LDA   z4+1
   INY
   STA   (FP),Y
   LDA   z4
   SEC
   SBC   #$01
   INY
   STA   (FP),Y
   LDA   z4+1
   SBC   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   z4
   INY
   LDA   (FP),Y
   STA   z4+1
   LDY   #$CE
   LDA   (FP),Y
   LDY   #$D2
   STA   (FP),Y
   STA   g2
   JMP   .if_end@0
.if_else@1:
   LDY   #$FA
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$D3
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$D4
   STA   (FP),Y
   LDY   #$02
   LDA   (DPTR),Y
   LDY   #$D5
   STA   (FP),Y
   LDY   #$03
   LDA   (DPTR),Y
   LDY   #$D6
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDA   #$80
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$D7
   LDA   (FP),Y
   LDY   #$DB
   STA   (FP),Y
   LDY   #$D8
   LDA   (FP),Y
   LDY   #$DC
   STA   (FP),Y
   LDY   #$D9
   LDA   (FP),Y
   LDY   #$DD
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$D3
   LDA   (FP),Y
   SEC
   LDY   #$DB
   SBC   (FP),Y
   LDY   #$DF
   STA   (FP),Y
   LDY   #$D4
   LDA   (FP),Y
   LDY   #$DC
   SBC   (FP),Y
   LDY   #$E0
   STA   (FP),Y
   LDY   #$D5
   LDA   (FP),Y
   LDY   #$DD
   SBC   (FP),Y
   LDY   #$E1
   STA   (FP),Y
   LDY   #$D6
   LDA   (FP),Y
   LDY   #$DE
   SBC   (FP),Y
   LDY   #$E2
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$04
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDY   #$DF
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$E0
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$E1
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$E2
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   JSR   mix
.if_end@0:
   LDA   ga6+2
   LDY   #$E3
   STA   (FP),Y
   LDA   ga6+3
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$E5
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$E6
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@38
   LDA   #$00
   JMP   .sx_done@39
.sx_neg@38:
   LDA   #$FF
.sx_done@39:
   LDY   #$E7
   STA   (FP),Y
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
   ADC   #$08
   LDY   #$EB
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$EC
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$ED
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$EE
   STA   (FP),Y
   LDY   #$02
   LDA   (DPTR),Y
   LDY   #$EF
   STA   (FP),Y
   LDY   #$03
   LDA   (DPTR),Y
   LDY   #$F0
   STA   (FP),Y
   LDY   #$E5
   LDA   (FP),Y
   LDY   #$ED
   AND   (FP),Y
   LDY   #$F1
   STA   (FP),Y
   LDY   #$E6
   LDA   (FP),Y
   LDY   #$EE
   AND   (FP),Y
   LDY   #$F2
   STA   (FP),Y
   LDY   #$E7
   LDA   (FP),Y
   LDY   #$EF
   AND   (FP),Y
   LDY   #$F3
   STA   (FP),Y
   LDY   #$E8
   LDA   (FP),Y
   LDY   #$F0
   AND   (FP),Y
   LDY   #$F4
   STA   (FP),Y
   LDY   #$F1
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
   ADC   #$FB
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

f17:
   SUBROUTINE

   ; prologue: 0 arg bytes, 170 local bytes
   SEC
   LDA   SSP
   SBC   #$AC
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$AB
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   LDA   #$02
   LDY   #$01
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$FF
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$03
   LDA   (FP),Y
   LDY   #$07
   STA   (FP),Y
   LDY   #$04
   LDA   (FP),Y
   LDY   #$08
   STA   (FP),Y
   LDY   #$05
   LDA   (FP),Y
   LDY   #$09
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
.loop@4_start:
   LDA   #$00
   LDY   #$0D
   STA   (FP),Y
   INY
   STA   (FP),Y
.loop@5_start:
   LDY   #$0F
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
   LDY   #$11
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@4
   JMP   .loop@5_break
.lb_skip@4:
   LDA   z5
   ORA   z5+1
   ORA   z5+2
   ORA   z5+3
   BEQ   .cond_else@2
   LDA   #$7F
   LDY   #$13
   STA   (FP),Y
   INY
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@41
   LDA   #$00
   JMP   .sx_done@42
.sx_neg@41:
   LDA   #$FF
.sx_done@42:
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
   JMP   .cond_end@3
.cond_else@2:
   LDA   gp8
   STA   DPTR
   LDA   gp8+1
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$18
   STA   (FP),Y
   INY
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@43
   LDA   #$00
   JMP   .sx_done@44
.sx_neg@43:
   LDA   #$FF
.sx_done@44:
   LDY   #$1A
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$16
   STA   (FP),Y
   LDY   #$1A
   LDA   (FP),Y
   LDY   #$17
   STA   (FP),Y
.cond_end@3:
   LDY   #$16
   LDA   (FP),Y
   LDY   #$1B
   STA   (FP),Y
   LDY   #$17
   LDA   (FP),Y
   LDY   #$1C
   STA   (FP),Y
   LDY   #$0C
   LDA   (FP),Y
   LDY   #$1D
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDA   #$03
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$1D
   LDA   (FP),Y
   LDY   #$1F
   AND   (FP),Y
   LDY   #$21
   STA   (FP),Y
   LDY   #$1E
   LDA   (FP),Y
   LDY   #$20
   AND   (FP),Y
   LDY   #$22
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$23
   STA   (FP),Y
   STA   HARGS
   LDA   #$02
   STA   HARGS+1
   JSR   mul8
   LDA   HARGS+2
   LDY   #$24
   STA   (FP),Y
   TAX
   LDA   ga6,X
   INY
   STA   (FP),Y
   LDA   ga6+1,X
   INY
   STA   (FP),Y
   LDA   #$07
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$25
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
   LDA   HARGS+4
   LDY   #$29
   STA   (FP),Y
   LDA   HARGS+5
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$2B
   STA   (FP),Y
   CLC
   LDA   FP
   ADC   #$07
   INY
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
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
   LDY   #$2B
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$2C
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   LDY   #$2D
   LDA   (FP),Y
   LDY   #$05
   STA   (SSP),Y
   JSR   f9
   LDA   HARGS+8
   LDY   #$2E
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
   SEC
   LDA   SSP
   SBC   #$04
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
   JSR   mix
   LDA   z5
   STA   HARGS
   LDA   z5+1
   STA   HARGS+1
   LDA   z5+2
   STA   HARGS+2
   LDA   z5+3
   STA   HARGS+3
   LDA   #$05
   STA   HARGS+4
   JSR   asl32
   LDA   HARGS+5
   LDY   #$32
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
   LDY   #$32
   LDA   (FP),Y
   LDY   #$36
   STA   (FP),Y
   LDY   #$33
   LDA   (FP),Y
   LDY   #$37
   STA   (FP),Y
   LDY   #$0E
   LDA   (FP),Y
   LDY   #$38
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDA   #$03
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$38
   LDA   (FP),Y
   LDY   #$3A
   AND   (FP),Y
   LDY   #$3C
   STA   (FP),Y
   LDY   #$39
   LDA   (FP),Y
   LDY   #$3B
   AND   (FP),Y
   LDY   #$3D
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$3E
   STA   (FP),Y
   STA   HARGS
   LDA   #$02
   STA   HARGS+1
   JSR   mul8
   LDA   HARGS+2
   LDY   #$3F
   STA   (FP),Y
   TAX
   LDY   #$36
   LDA   (FP),Y
   STA   ga6,X
   INY
   LDA   (FP),Y
   STA   ga6+1,X
   LDY   #$02
   LDA   (FP),Y
   LDY   #$40
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$42
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$43
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@45
   LDA   #$00
   JMP   .sx_done@46
.sx_neg@45:
   LDA   #$FF
.sx_done@46:
   LDY   #$44
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$42
   LDA   (FP),Y
   SEC
   SBC   z5
   LDY   #$46
   STA   (FP),Y
   LDY   #$43
   LDA   (FP),Y
   SBC   z5+1
   LDY   #$47
   STA   (FP),Y
   LDY   #$44
   LDA   (FP),Y
   SBC   z5+2
   LDY   #$48
   STA   (FP),Y
   LDY   #$45
   LDA   (FP),Y
   SBC   z5+3
   LDY   #$49
   STA   (FP),Y
   LDY   #$46
   LDA   (FP),Y
   LDY   #$4A
   STA   (FP),Y
   LDY   #$47
   LDA   (FP),Y
   LDY   #$4B
   STA   (FP),Y
   LDY   #$48
   LDA   (FP),Y
   LDY   #$4C
   STA   (FP),Y
   LDY   #$49
   LDA   (FP),Y
   LDY   #$4D
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
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
   LDY   #$53
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$54
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$4A
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$4B
   LDA   (FP),Y
   LDY   #$01
   STA   (DPTR),Y
   LDY   #$4C
   LDA   (FP),Y
   LDY   #$02
   STA   (DPTR),Y
   LDY   #$4D
   LDA   (FP),Y
   LDY   #$03
   STA   (DPTR),Y
   LDY   #$4E
   LDA   (FP),Y
   LDY   #$04
   STA   (DPTR),Y
   LDY   #$4F
   LDA   (FP),Y
   LDY   #$05
   STA   (DPTR),Y
   LDY   #$50
   LDA   (FP),Y
   LDY   #$06
   STA   (DPTR),Y
   LDA   #<gs7
   LDY   #$55
   STA   (FP),Y
   LDA   #>gs7
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$01
   LDY   #$57
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$58
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$59
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$5A
   STA   (FP),Y
   LDY   #$02
   LDA   (DPTR),Y
   LDY   #$5B
   STA   (FP),Y
   LDY   #$03
   LDA   (DPTR),Y
   LDY   #$5C
   STA   (FP),Y
   LDY   #$04
   LDA   (DPTR),Y
   LDY   #$5D
   STA   (FP),Y
   LDY   #$05
   LDA   (DPTR),Y
   LDY   #$5E
   STA   (FP),Y
   LDY   #$06
   LDA   (DPTR),Y
   LDY   #$5F
   STA   (FP),Y
   LDY   #$59
   LDA   (FP),Y
   SEC
   SBC   #$05
   LDY   #$60
   STA   (FP),Y
   LDY   #$5A
   LDA   (FP),Y
   SBC   #$00
   LDY   #$61
   STA   (FP),Y
   LDY   #$5B
   LDA   (FP),Y
   SBC   #$00
   LDY   #$62
   STA   (FP),Y
   LDY   #$5C
   LDA   (FP),Y
   SBC   #$00
   LDY   #$63
   STA   (FP),Y
   LDY   #$5D
   LDA   (FP),Y
   SBC   #$00
   LDY   #$64
   STA   (FP),Y
   LDY   #$5E
   LDA   (FP),Y
   SBC   #$00
   LDY   #$65
   STA   (FP),Y
   LDY   #$5F
   LDA   (FP),Y
   SBC   #$00
   LDY   #$66
   STA   (FP),Y
   LDY   #$57
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$60
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$61
   LDA   (FP),Y
   LDY   #$01
   STA   (DPTR),Y
   LDY   #$62
   LDA   (FP),Y
   LDY   #$02
   STA   (DPTR),Y
   LDY   #$63
   LDA   (FP),Y
   LDY   #$03
   STA   (DPTR),Y
   LDY   #$64
   LDA   (FP),Y
   LDY   #$04
   STA   (DPTR),Y
   LDY   #$65
   LDA   (FP),Y
   LDY   #$05
   STA   (DPTR),Y
   LDY   #$66
   LDA   (FP),Y
   LDY   #$06
   STA   (DPTR),Y
   LDY   #$0E
   LDA   (FP),Y
   LDY   #$67
   STA   (FP),Y
   CLC
   ADC   #$01
   INY
   STA   (FP),Y
   LDY   #$0E
   STA   (FP),Y
   JMP   .loop@5_start
.loop@5_break:
   LDY   #$0C
   LDA   (FP),Y
   CLC
   ADC   #$01
   LDY   #$69
   STA   (FP),Y
   LDY   #$0C
   STA   (FP),Y
   LDY   #$6A
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   SEC
   SBC   #$01
   INY
   LDA   (FP),Y
   SBC   #$00
   BVC   .cmp_novf@47
   EOR   #$80
.cmp_novf@47:
   ASL
   LDA   #$00
   ROL
   LDY   #$6C
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@3
   JMP   .loop@4_start
.lb_skip@3:
   LDA   z3+3
   LDY   #$71
   STA   (FP),Y
   LSR
   STA   (FP),Y
   LDA   z3+2
   DEY
   STA   (FP),Y
   ROR
   STA   (FP),Y
   LDA   z3+1
   DEY
   STA   (FP),Y
   ROR
   STA   (FP),Y
   LDA   z3
   DEY
   STA   (FP),Y
   ROR
   STA   (FP),Y
   LDY   #$71
   LDA   (FP),Y
   LSR
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ROR
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ROR
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ROR
   STA   (FP),Y
   LDY   #$72
   STA   (FP),Y
   LDY   #$6F
   LDA   (FP),Y
   LDY   #$73
   STA   (FP),Y
   LDA   g2
   INY
   STA   (FP),Y
   LDA   g2
   BMI   .sx_neg@48
   LDA   #$00
   JMP   .sx_done@49
.sx_neg@48:
   LDA   #$FF
.sx_done@49:
   LDY   #$75
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   EOR   #$FF
   LDY   #$76
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   EOR   #$FF
   LDY   #$77
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$78
   STA   (FP),Y
   CLC
   LDA   FP
   ADC   #$07
   INY
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$05
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDY   #$72
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$73
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$78
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$79
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   LDY   #$7A
   LDA   (FP),Y
   LDY   #$05
   STA   (SSP),Y
   JSR   f9
   LDA   HARGS+8
   LDY   #$7B
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
   LDY   #$7B
   LDA   (FP),Y
   LDY   #$07
   STA   (FP),Y
   LDY   #$7C
   LDA   (FP),Y
   LDY   #$08
   STA   (FP),Y
   LDY   #$7D
   LDA   (FP),Y
   LDY   #$09
   STA   (FP),Y
   LDY   #$7E
   LDA   (FP),Y
   LDY   #$0A
   STA   (FP),Y
   LDA   #<gs7
   LDY   #$7F
   STA   (FP),Y
   LDA   #>gs7
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$08
   LDY   #$81
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$82
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$83
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$84
   STA   (FP),Y
   LDY   #$02
   LDA   (DPTR),Y
   LDY   #$85
   STA   (FP),Y
   LDY   #$03
   LDA   (DPTR),Y
   LDY   #$86
   STA   (FP),Y
   LDA   #$03
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   ORA   #$00
   BPL   .sx_done@51
   LDA   #$FF
.sx_done@51:
   LDY   #$89
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$83
   LDA   (FP),Y
   LDY   #$87
   EOR   (FP),Y
   LDY   #$8B
   STA   (FP),Y
   LDY   #$84
   LDA   (FP),Y
   LDY   #$88
   EOR   (FP),Y
   LDY   #$8C
   STA   (FP),Y
   LDY   #$85
   LDA   (FP),Y
   LDY   #$89
   EOR   (FP),Y
   LDY   #$8D
   STA   (FP),Y
   LDY   #$86
   LDA   (FP),Y
   LDY   #$8A
   EOR   (FP),Y
   LDY   #$8E
   STA   (FP),Y
   LDY   #$81
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$8B
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$8C
   LDA   (FP),Y
   LDY   #$01
   STA   (DPTR),Y
   LDY   #$8D
   LDA   (FP),Y
   LDY   #$02
   STA   (DPTR),Y
   LDY   #$8E
   LDA   (FP),Y
   LDY   #$03
   STA   (DPTR),Y
   LDY   #$8B
   LDA   (FP),Y
   LDY   #$8F
   STA   (FP),Y
   LDY   #$8C
   LDA   (FP),Y
   LDY   #$90
   STA   (FP),Y
   LDA   #$07
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$8F
   LDA   (FP),Y
   LDY   #$91
   AND   (FP),Y
   LDY   #$93
   STA   (FP),Y
   LDY   #$90
   LDA   (FP),Y
   LDY   #$92
   AND   (FP),Y
   LDY   #$94
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CMP   #$02
   BEQ   .cmp_true@52
   LDA   #$00
   JMP   .cmp_end@53
.cmp_true@52:
   LDA   #$01
.cmp_end@53:
   LDY   #$95
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .default@8
   LDY   #$02
   LDA   (FP),Y
   CLC
   ADC   #$01
   LDY   #$97
   STA   (FP),Y
   LDY   #$02
   STA   (FP),Y
   JMP   .switch@6_break
.default@8:
   LDY   #$07
   LDA   (FP),Y
   CLC
   ADC   #$01
   LDY   #$98
   STA   (FP),Y
   LDY   #$08
   LDA   (FP),Y
   ADC   #$00
   LDY   #$99
   STA   (FP),Y
   LDY   #$09
   LDA   (FP),Y
   ADC   #$00
   LDY   #$9A
   STA   (FP),Y
   LDY   #$0A
   LDA   (FP),Y
   ADC   #$00
   LDY   #$9B
   STA   (FP),Y
   LDY   #$98
   LDA   (FP),Y
   LDY   #$07
   STA   (FP),Y
   LDY   #$99
   LDA   (FP),Y
   LDY   #$08
   STA   (FP),Y
   LDY   #$9A
   LDA   (FP),Y
   LDY   #$09
   STA   (FP),Y
   LDY   #$9B
   LDA   (FP),Y
   LDY   #$0A
   STA   (FP),Y
.switch@6_break:
   LDA   #<gs7
   LDY   #$9C
   STA   (FP),Y
   LDA   #>gs7
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$08
   LDY   #$9E
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$9F
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$A0
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$A1
   STA   (FP),Y
   LDY   #$02
   LDA   (DPTR),Y
   LDY   #$A2
   STA   (FP),Y
   LDY   #$03
   LDA   (DPTR),Y
   LDY   #$A3
   STA   (FP),Y
   LDY   #$A0
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   INY
   ORA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .cond_else@4
   LDA   g1
   INY
   STA   (FP),Y
   LDA   g1
   BMI   .sx_neg@54
   LDA   #$00
   JMP   .sx_done@55
.sx_neg@54:
   LDA   #$FF
.sx_done@55:
   LDY   #$A5
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$A6
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$A7
   STA   (FP),Y
   JMP   .cond_end@5
.cond_else@4:
   LDA   ga6+6
   LDY   #$A8
   STA   (FP),Y
   LDA   ga6+7
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$A6
   STA   (FP),Y
   LDY   #$A9
   LDA   (FP),Y
   LDY   #$A7
   STA   (FP),Y
.cond_end@5:
   LDY   #$A6
   LDA   (FP),Y
   LDY   #$AA
   STA   (FP),Y

   ; epilogue
   PHA
   CLC
   LDA   FP
   ADC   #$AC
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

f22:
   SUBROUTINE

   ; prologue: 13 arg bytes, 159 local bytes
   SEC
   LDA   SSP
   SBC   #$A1
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$A0
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   LDA   #$80
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
   DEY
   LDA   (FP),Y
   LDY   #$12
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$13
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDA   #$80
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
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
   LDY   #$16
   LDA   (FP),Y
   LDY   #$1A
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$FF
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDA   #$07
   INY
   STA   (FP),Y
   LDA   #$00
   INY
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
   JSR   sdivmod16
   LDA   HARGS+6
   LDY   #$21
   STA   (FP),Y
   LDA   HARGS+7
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
   LDY   #$27
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$28
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$29
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$2A
   STA   (FP),Y
   LDY   #$02
   LDA   (DPTR),Y
   LDY   #$2B
   STA   (FP),Y
   LDY   #$03
   LDA   (DPTR),Y
   LDY   #$2C
   STA   (FP),Y
   LDY   #$04
   LDA   (DPTR),Y
   LDY   #$2D
   STA   (FP),Y
   LDY   #$05
   LDA   (DPTR),Y
   LDY   #$2E
   STA   (FP),Y
   LDY   #$06
   LDA   (DPTR),Y
   LDY   #$2F
   STA   (FP),Y
   LDY   #$29
   LDA   (FP),Y
   ASL
   INY
   LDA   (FP),Y
   ROL
   LDY   #$30
   STA   (FP),Y
   LDY   #$37
   STA   (FP),Y
   CLC
   LDA   FP
   ADC   #$18
   INY
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$05
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDY   #$23
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$24
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
   JSR   f9
   LDA   HARGS+8
   LDY   #$3A
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
   SEC
   LDA   SSP
   SBC   #$04
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDY   #$3A
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$3B
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$3C
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$3D
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   JSR   mix
   LDA   #$00
   LDY   #$3E
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$AD
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$41
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$42
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$43
   STA   (FP),Y
   INY
   STA   (FP),Y
   CLC
   LDA   FP
   ADC   #$18
   INY
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$05
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDY   #$3F
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$40
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$44
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$45
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   LDY   #$46
   LDA   (FP),Y
   LDY   #$05
   STA   (SSP),Y
   JSR   f9
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
   JSR   f17
   LDY   #$4B
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
   SEC
   LDA   SSP
   SBC   #$04
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDY   #$4C
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$4D
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
   JSR   mix
   LDA   #$00
   LDY   #$50
   STA   (FP),Y
   LDA   #$01
   INY
   STA   (FP),Y
   LDA   z4
   INY
   STA   (FP),Y
   CLC
   LDA   FP
   ADC   #$18
   INY
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$05
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDY   #$50
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$51
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$52
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$53
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   LDY   #$54
   LDA   (FP),Y
   LDY   #$05
   STA   (SSP),Y
   JSR   f9
   LDA   HARGS+8
   LDY   #$55
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
   LDY   #$02
   LDA   (DPTR),Y
   LDY   #$5F
   STA   (FP),Y
   LDY   #$03
   LDA   (DPTR),Y
   LDY   #$60
   STA   (FP),Y
   LDY   #$04
   LDA   (DPTR),Y
   LDY   #$61
   STA   (FP),Y
   LDY   #$05
   LDA   (DPTR),Y
   LDY   #$62
   STA   (FP),Y
   LDY   #$06
   LDA   (DPTR),Y
   LDY   #$63
   STA   (FP),Y
   LDY   #$5D
   LDA   (FP),Y
   CLC
   ADC   #$01
   LDY   #$64
   STA   (FP),Y
   LDY   #$5E
   LDA   (FP),Y
   ADC   #$00
   LDY   #$65
   STA   (FP),Y
   LDY   #$5F
   LDA   (FP),Y
   ADC   #$00
   LDY   #$66
   STA   (FP),Y
   LDY   #$60
   LDA   (FP),Y
   ADC   #$00
   LDY   #$67
   STA   (FP),Y
   LDY   #$61
   LDA   (FP),Y
   ADC   #$00
   LDY   #$68
   STA   (FP),Y
   LDY   #$62
   LDA   (FP),Y
   ADC   #$00
   LDY   #$69
   STA   (FP),Y
   LDY   #$63
   LDA   (FP),Y
   ADC   #$00
   LDY   #$6A
   STA   (FP),Y
   LDY   #$5B
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$64
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$65
   LDA   (FP),Y
   LDY   #$01
   STA   (DPTR),Y
   LDY   #$66
   LDA   (FP),Y
   LDY   #$02
   STA   (DPTR),Y
   LDY   #$67
   LDA   (FP),Y
   LDY   #$03
   STA   (DPTR),Y
   LDY   #$68
   LDA   (FP),Y
   LDY   #$04
   STA   (DPTR),Y
   LDY   #$69
   LDA   (FP),Y
   LDY   #$05
   STA   (DPTR),Y
   LDY   #$6A
   LDA   (FP),Y
   LDY   #$06
   STA   (DPTR),Y
   LDY   #$5D
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
   BNE   .lb_skip@8
   JMP   .if_end@6
.lb_skip@8:
   LDA   #$00
   LDY   #$6B
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$6F
   STA   (FP),Y
   LSR
   STA   (FP),Y
   LDY   #$6C
   LDA   (FP),Y
   LDY   #$6E
   STA   (FP),Y
   ROR
   STA   (FP),Y
   INY
   LDA   (FP),Y
   LSR
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ROR
   STA   (FP),Y
   INY
   LDA   (FP),Y
   LSR
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ROR
   STA   (FP),Y
   LDA   #$03
   LDY   #$70
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$6E
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
   LDA   HARGS+4
   LDY   #$72
   STA   (FP),Y
   LDA   HARGS+5
   INY
   STA   (FP),Y
   LDA   #$07
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$72
   LDA   (FP),Y
   LDY   #$74
   AND   (FP),Y
   LDY   #$76
   STA   (FP),Y
   LDY   #$73
   LDA   (FP),Y
   LDY   #$75
   AND   (FP),Y
   LDY   #$77
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   BEQ   .cmp_true@56
   LDA   #$00
   JMP   .cmp_end@57
.cmp_true@56:
   LDA   #$01
.cmp_end@57:
   LDY   #$78
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .case@10
   LDY   #$76
   LDA   (FP),Y
   CMP   #$04
   BEQ   .cmp_true@58
   LDA   #$00
   JMP   .cmp_end@59
.cmp_true@58:
   LDA   #$01
.cmp_end@59:
   LDY   #$7A
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@7
   JMP   .case@12
.lb_skip@7:
   LDY   #$76
   LDA   (FP),Y
   CMP   #$06
   BEQ   .cmp_true@60
   LDA   #$00
   JMP   .cmp_end@61
.cmp_true@60:
   LDA   #$01
.cmp_end@61:
   LDY   #$7C
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@6
   JMP   .case@13
.lb_skip@6:
   JMP   .default@14
.case@10:
   LDA   #$00
   LDY   #$7E
   STA   (FP),Y
   INY
   STA   (FP),Y
.loop@11_start:
   LDY   #$80
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
   BVC   .cmp_novf@62
   EOR   #$80
.cmp_novf@62:
   ASL
   LDA   #$00
   ROL
   LDY   #$82
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@5
   JMP   .switch@9_break
.lb_skip@5:
   LDY   #$A6
   LDA   (FP),Y
   SEC
   SBC   #$01
   LDY   #$84
   STA   (FP),Y
   LDY   #$A7
   LDA   (FP),Y
   SBC   #$00
   LDY   #$85
   STA   (FP),Y
   LDY   #$A8
   LDA   (FP),Y
   SBC   #$00
   LDY   #$86
   STA   (FP),Y
   LDY   #$A9
   LDA   (FP),Y
   SBC   #$00
   LDY   #$87
   STA   (FP),Y
   LDY   #$AA
   LDA   (FP),Y
   SBC   #$00
   LDY   #$88
   STA   (FP),Y
   LDY   #$AB
   LDA   (FP),Y
   SBC   #$00
   LDY   #$89
   STA   (FP),Y
   LDY   #$AC
   LDA   (FP),Y
   SBC   #$00
   LDY   #$8A
   STA   (FP),Y
   LDY   #$84
   LDA   (FP),Y
   LDY   #$A6
   STA   (FP),Y
   LDY   #$85
   LDA   (FP),Y
   LDY   #$A7
   STA   (FP),Y
   LDY   #$86
   LDA   (FP),Y
   LDY   #$A8
   STA   (FP),Y
   LDY   #$87
   LDA   (FP),Y
   LDY   #$A9
   STA   (FP),Y
   LDY   #$88
   LDA   (FP),Y
   LDY   #$AA
   STA   (FP),Y
   LDY   #$89
   LDA   (FP),Y
   LDY   #$AB
   STA   (FP),Y
   LDY   #$8A
   LDA   (FP),Y
   LDY   #$AC
   STA   (FP),Y
   LDY   #$7F
   LDA   (FP),Y
   LDY   #$8B
   STA   (FP),Y
   CLC
   ADC   #$01
   INY
   STA   (FP),Y
   LDY   #$7F
   STA   (FP),Y
   JMP   .loop@11_start
.case@12:
   LDY   #$AD
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$8D
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$8E
   STA   (FP),Y
   LDY   #$90
   STA   (FP),Y
   LSR
   STA   (FP),Y
   LDY   #$8D
   LDA   (FP),Y
   LDY   #$8F
   STA   (FP),Y
   ROR
   STA   (FP),Y
   INY
   LDA   (FP),Y
   LSR
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ROR
   STA   (FP),Y
   INY
   LDA   (FP),Y
   LSR
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ROR
   STA   (FP),Y
   INY
   LDA   (FP),Y
   LSR
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ROR
   STA   (FP),Y
   LDY   #$AD
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$8F
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$90
   LDA   (FP),Y
   LDY   #$01
   STA   (DPTR),Y
   LDA   z4
   LDY   #$91
   STA   (FP),Y
   LDA   z4+1
   INY
   STA   (FP),Y
   LDY   #$8F
   LDA   (FP),Y
   LDY   #$91
   AND   (FP),Y
   LDY   #$93
   STA   (FP),Y
   LDY   #$90
   LDA   (FP),Y
   LDY   #$92
   AND   (FP),Y
   LDY   #$94
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@7
   JMP   .switch@9_break
.shift_loop@65:
   LDY   #$9A
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
   BNE   .shift_loop@65
   LDA   (FP),Y
   LDY   #$9B
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$9C
   STA   (FP),Y
   LDY   #$AD
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$9B
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$9C
   LDA   (FP),Y
   LDY   #$01
   STA   (DPTR),Y
.if_end@7:
.case@13:
.default@14:
.switch@9_break:
.if_end@6:
   LDA   #$FF
   LDY   #$9D
   STA   (FP),Y
   INY
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
   ADC   #$AE
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$A0
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

   ; prologue: 0 arg bytes, 96 local bytes
   SEC
   LDA   SSP
   SBC   #$62
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$61
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
   INY
   STA   (FP),Y
   LDA   #$80
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
   LDA   #$7F
   INY
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
   LDA   #$78
   INY
   STA   (FP),Y
   LDA   #$FC
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
   LDX   #$06
.shift_loop@67:
   LDY   #$1A
   LDA   (FP),Y
   LSR
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ROR
   STA   (FP),Y
   DEX
   BNE   .shift_loop@67
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
   LDA   #<gs7
   INY
   STA   (FP),Y
   LDA   #>gs7
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$08
   LDY   #$21
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$22
   STA   (FP),Y
   DEY
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
   LDY   #$1D
   LDA   (FP),Y
   LDY   #$02
   STA   (DPTR),Y
   LDY   #$1E
   LDA   (FP),Y
   LDY   #$03
   STA   (DPTR),Y
   LDA   #<ga6
   LDY   #$23
   STA   (FP),Y
   LDA   #>ga6
   INY
   STA   (FP),Y
   LDA   #$01
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$27
   STA   (FP),Y
   STA   HARGS
   LDA   #$02
   STA   HARGS+1
   JSR   mul8
   LDA   HARGS+2
   LDY   #$28
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$23
   LDA   (FP),Y
   CLC
   LDY   #$29
   ADC   (FP),Y
   LDY   #$2B
   STA   (FP),Y
   LDY   #$24
   LDA   (FP),Y
   LDY   #$2A
   ADC   (FP),Y
   LDY   #$2C
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$2D
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$2E
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$FF
   LDY   #$2F
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$30
   STA   (FP),Y
   LDY   #$2B
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$2F
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$30
   LDA   (FP),Y
   LDY   #$01
   STA   (DPTR),Y
   LDY   #$2F
   LDA   (FP),Y
   LDY   #$31
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$32
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@69
   LDA   #$00
   JMP   .sx_done@70
.sx_neg@69:
   LDA   #$FF
.sx_done@70:
   LDY   #$33
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
   LDY   #$31
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$32
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
   JSR   mix
   LDA   #$0D
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
   BMI   .sx_neg@71
   LDA   #$00
   JMP   .sx_done@72
.sx_neg@71:
   LDA   #$FF
.sx_done@72:
   LDY   #$39
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
   LDY   #$37
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$38
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$39
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$3A
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   JSR   mix
   LDY   #$15
   LDA   (FP),Y
   LSR
   LSR
   LSR
   LDY   #$3C
   STA   (FP),Y
   LDY   #$15
   LDA   (FP),Y
   ASL
   ASL
   ASL
   ASL
   ASL
   LDY   #$3B
   STA   (FP),Y
   LDY   #$3D
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$3E
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
   JSR   mix
   LDA   #<gs7
   LDY   #$41
   STA   (FP),Y
   LDA   #>gs7
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$01
   LDY   #$43
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$44
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$45
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$46
   STA   (FP),Y
   LDY   #$02
   LDA   (DPTR),Y
   LDY   #$47
   STA   (FP),Y
   LDY   #$03
   LDA   (DPTR),Y
   LDY   #$48
   STA   (FP),Y
   LDY   #$04
   LDA   (DPTR),Y
   LDY   #$49
   STA   (FP),Y
   LDY   #$05
   LDA   (DPTR),Y
   LDY   #$4A
   STA   (FP),Y
   LDY   #$06
   LDA   (DPTR),Y
   LDY   #$4B
   STA   (FP),Y
   LSR
   DEY
   LDA   (FP),Y
   ROR
   LDY   #$52
   STA   (FP),Y
   LDY   #$49
   LDA   (FP),Y
   ROR
   LDY   #$51
   STA   (FP),Y
   LDY   #$48
   LDA   (FP),Y
   ROR
   LDY   #$50
   STA   (FP),Y
   LDY   #$47
   LDA   (FP),Y
   ROR
   LDY   #$4F
   STA   (FP),Y
   LDY   #$46
   LDA   (FP),Y
   ROR
   LDY   #$4E
   STA   (FP),Y
   LDY   #$45
   LDA   (FP),Y
   ROR
   LDY   #$4D
   STA   (FP),Y
   LDA   #$00
   ROR
   DEY
   STA   (FP),Y
   LDY   #$43
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$4C
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$4D
   LDA   (FP),Y
   LDY   #$01
   STA   (DPTR),Y
   LDY   #$4E
   LDA   (FP),Y
   LDY   #$02
   STA   (DPTR),Y
   LDY   #$4F
   LDA   (FP),Y
   LDY   #$03
   STA   (DPTR),Y
   LDY   #$50
   LDA   (FP),Y
   LDY   #$04
   STA   (DPTR),Y
   LDY   #$51
   LDA   (FP),Y
   LDY   #$05
   STA   (DPTR),Y
   LDY   #$52
   LDA   (FP),Y
   LDY   #$06
   STA   (DPTR),Y
   LDY   #$4C
   LDA   (FP),Y
   LDY   #$53
   STA   (FP),Y
   LDY   #$4D
   LDA   (FP),Y
   LDY   #$54
   STA   (FP),Y
   LDY   #$4E
   LDA   (FP),Y
   LDY   #$55
   STA   (FP),Y
   LDY   #$4F
   LDA   (FP),Y
   LDY   #$56
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$04
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDY   #$53
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$54
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$55
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$56
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   JSR   mix
   LDA   gp8
   STA   DPTR
   LDA   gp8+1
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$57
   STA   (FP),Y
   INY
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@73
   LDA   #$00
   JMP   .sx_done@74
.sx_neg@73:
   LDA   #$FF
.sx_done@74:
   LDY   #$59
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$81
   LDY   #$5A
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$5B
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$5C
   STA   (FP),Y
   LDA   gp8
   STA   DPTR
   LDA   gp8+1
   STA   DPTR+1
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   JSR   fin0
   JSR   fin1
   JSR   fin2
   LDA   acc
   LDY   #$5D
   STA   (FP),Y
   LDA   acc+1
   AND   #$7F
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$5F
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$60
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
   ADC   #$62
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$61
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
   DS.B  1

g2:
   DC.B  $0F

ga6:
   DS.B  2
   DC.W  $0080
   DC.W  $00FF
   DC.W  $0002

gs7:
   DS.B  1
   DC.B  $FF
   DS.B  6
   DC.L  $00000001

gp8:
   DC.W  g1

@4.s15:
   DC.W  $0003

__zp_init:
   SUBROUTINE

   LDA   #$00
   STA   z3+2
   STA   z3+3
   STA   z5+1
   STA   z5+2
   STA   z5+3
   LDA   #$FF
   STA   z3
   LDA   #$7F
   STA   z3+1
   LDA   #$C2
   STA   z4
   LDA   #$C6
   STA   z4+1
   LDA   #$01
   STA   z5
   RTS
