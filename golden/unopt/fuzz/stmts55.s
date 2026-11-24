SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
z4	EQU	$26

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

   SEC
   LDA   SSP
   SBC   #$04
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   g1
   LDY   #$01
   STA   (SSP),Y
   LDA   g1+1
   INY
   STA   (SSP),Y
   LDA   g1+2
   INY
   STA   (SSP),Y
   LDA   g1+3
   INY
   STA   (SSP),Y
   JSR   mix
   LDA   g2
   LDY   #$01
   STA   (FP),Y
   LDA   g2
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
   LDA   g3
   LDY   #$05
   STA   (FP),Y
   LDA   g3+1
   INY
   STA   (FP),Y
   LDA   g3+2
   INY
   STA   (FP),Y
   LDA   g3+3
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
   LDA   z4
   LDY   #$09
   STA   (FP),Y
   LDA   z4+1
   INY
   STA   (FP),Y
   LDA   z4+2
   INY
   STA   (FP),Y
   LDA   z4+3
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

   ; prologue: 0 arg bytes, 16 local bytes
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

   LDA   ga5
   LDY   #$01
   STA   (FP),Y
   LDA   ga5+1
   INY
   STA   (FP),Y
   LDA   ga5+2
   INY
   STA   (FP),Y
   LDA   ga5+3
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
   LDA   ga5+4
   LDY   #$05
   STA   (FP),Y
   LDA   ga5+5
   INY
   STA   (FP),Y
   LDA   ga5+6
   INY
   STA   (FP),Y
   LDA   ga5+7
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
   LDA   ga5+8
   LDY   #$09
   STA   (FP),Y
   LDA   ga5+9
   INY
   STA   (FP),Y
   LDA   ga5+10
   INY
   STA   (FP),Y
   LDA   ga5+11
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
   LDA   ga5+12
   LDY   #$0D
   STA   (FP),Y
   LDA   ga5+13
   INY
   STA   (FP),Y
   LDA   ga5+14
   INY
   STA   (FP),Y
   LDA   ga5+15
   INY
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$04
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDY   #$0D
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$0E
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$0F
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$10
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   JSR   mix

   ; epilogue
   CLC
   LDA   FP
   ADC   #$12
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
   RTS

fin2:
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

   LDA   #<gs6
   LDY   #$01
   STA   (FP),Y
   LDA   #>gs6
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$01
   LDY   #$03
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$04
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$05
   STA   (FP),Y
   INY
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@2
   LDA   #$00
   JMP   .sx_done@3
.sx_neg@2:
   LDA   #$FF
.sx_done@3:
   LDY   #$07
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
   LDY   #$06
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$07
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$08
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$09
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   JSR   mix
   LDA   #<gs6
   LDY   #$0A
   STA   (FP),Y
   LDA   #>gs6
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$02
   LDY   #$0C
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$0D
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$0E
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
   LDY   #$10
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

f8:
   SUBROUTINE

   ; prologue: 0 arg bytes, 136 local bytes
   SEC
   LDA   SSP
   SBC   #$8A
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$89
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
   LDA   #$FF
   INY
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
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$07
   LDA   (FP),Y
   STA   z4
   INY
   LDA   (FP),Y
   STA   z4+1
   INY
   LDA   (FP),Y
   STA   z4+2
   INY
   LDA   (FP),Y
   STA   z4+3
   INY
   LDA   (FP),Y
   STA   z4+4
   INY
   LDA   (FP),Y
   STA   z4+5
   INY
   LDA   (FP),Y
   STA   z4+6
   INY
   LDA   (FP),Y
   STA   z4+7
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   CLC
   ADC   #$01
   INY
   STA   (FP),Y
   LDY   #$10
   STA   (FP),Y
   INY
   LDA   (FP),Y
   LDY   #$13
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
   BVC   .cmp_novf@8
   EOR   #$80
.cmp_novf@8:
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
   INY
   ORA   (FP),Y
   BNE   .lb_skip@0
   JMP   .loop@0_break
.lb_skip@0:
   LDA   #$00
   LDY   #$17
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #<gs6
   INY
   STA   (FP),Y
   LDA   #>gs6
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$01
   LDY   #$1B
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$1C
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$1D
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
   LDY   #$1F
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ASL
   INY
   LDA   (FP),Y
   ROL
   INY
   STA   (FP),Y
   LDA   #$00
   ADC   #$FF
   EOR   #$FF
   INY
   STA   (FP),Y
   LDA   #<gs6
   INY
   STA   (FP),Y
   LDA   #>gs6
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$02
   LDY   #$24
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$25
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$26
   STA   (FP),Y
   INY
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@11
   LDA   #$00
   JMP   .sx_done@12
.sx_neg@11:
   LDA   #$FF
.sx_done@12:
   LDY   #$28
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$29
   STA   (FP),Y
   ASL
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$2A
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
   LDY   #$20
   LDA   (FP),Y
   SEC
   LDY   #$29
   SBC   (FP),Y
   LDY   #$21
   LDA   (FP),Y
   LDY   #$2A
   SBC   (FP),Y
   BVC   .cmp_novf@13
   EOR   #$80
.cmp_novf@13:
   ASL
   LDA   #$00
   ROL
   EOR   #$01
   LDY   #$2B
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$2D
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$2E
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@14
   LDA   #$00
   JMP   .sx_done@15
.sx_neg@14:
   LDA   #$FF
.sx_done@15:
   LDY   #$2F
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
   LDY   #$2D
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
   INY
   LDA   (FP),Y
   STA   HARGS+22
   INY
   LDA   (FP),Y
   STA   HARGS+23

   ; epilogue
   CLC
   LDA   FP
   ADC   #$8A
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$89
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   RTS
.loop@0_break:
   LDA   ga5+8
   LDY   #$67
   STA   (FP),Y
   LDA   ga5+9
   INY
   STA   (FP),Y
   LDA   ga5+10
   INY
   STA   (FP),Y
   LDA   ga5+11
   INY
   STA   (FP),Y
   LDA   g2
   INY
   STA   (FP),Y
   LDA   g2
   BMI   .sx_neg@33
   LDA   #$00
   JMP   .sx_done@34
.sx_neg@33:
   LDA   #$FF
.sx_done@34:
   LDY   #$6C
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   EOR   #$5A
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
   STA   g2
   LDA   (FP),Y
   INY
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@35
   LDA   #$00
   JMP   .sx_done@36
.sx_neg@35:
   LDA   #$FF
.sx_done@36:
   LDY   #$71
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$72
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$73
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@37
   LDA   #$00
   JMP   .sx_done@38
.sx_neg@37:
   LDA   #$FF
.sx_done@38:
   LDY   #$74
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$67
   LDA   (FP),Y
   CLC
   LDY   #$72
   ADC   (FP),Y
   LDY   #$76
   STA   (FP),Y
   LDY   #$68
   LDA   (FP),Y
   LDY   #$73
   ADC   (FP),Y
   LDY   #$77
   STA   (FP),Y
   LDY   #$69
   LDA   (FP),Y
   LDY   #$74
   ADC   (FP),Y
   LDY   #$78
   STA   (FP),Y
   LDY   #$6A
   LDA   (FP),Y
   LDY   #$75
   ADC   (FP),Y
   LDY   #$79
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$04
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDY   #$76
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$77
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
   JSR   mix
   LDA   gp7
   STA   DPTR
   LDA   gp7+1
   STA   DPTR+1
   LDA   z4
   LDY   #$00
   STA   (DPTR),Y
   LDA   z4+1
   INY
   STA   (DPTR),Y
   LDA   z4+2
   INY
   STA   (DPTR),Y
   LDA   z4+3
   INY
   STA   (DPTR),Y
   LDA   z4+4
   INY
   STA   (DPTR),Y
   LDA   z4+5
   INY
   STA   (DPTR),Y
   LDA   z4+6
   INY
   STA   (DPTR),Y
   LDA   z4+7
   INY
   STA   (DPTR),Y
   LDA   #$00
   LDY   #$7A
   STA   (FP),Y
   INY
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@39
   LDA   #$00
   JMP   .sx_done@40
.sx_neg@39:
   LDA   #$FF
.sx_done@40:
   LDY   #$7C
   STA   (FP),Y
   LDA   #$01
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$7B
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
   LDY   #$7F
   STA   (FP),Y
   LDA   HARGS+7
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$81
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$82
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@41
   LDA   #$00
   JMP   .sx_done@42
.sx_neg@41:
   LDA   #$FF
.sx_done@42:
   LDY   #$83
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
   LDY   #$81
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
   INY
   LDA   (FP),Y
   STA   HARGS+22
   INY
   LDA   (FP),Y
   STA   HARGS+23

   ; epilogue
   CLC
   LDA   FP
   ADC   #$8A
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

f14:
   SUBROUTINE

   ; prologue: 2 arg bytes, 178 local bytes
   SEC
   LDA   SSP
   SBC   #$B4
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$B3
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
   LDA   #<ga5
   INY
   STA   (FP),Y
   LDA   #>ga5
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   STA   HARGS
   LDA   #$04
   STA   HARGS+1
   JSR   mul8
   LDA   HARGS+2
   LDY   #$0A
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$05
   LDA   (FP),Y
   CLC
   LDY   #$0B
   ADC   (FP),Y
   LDY   #$0D
   STA   (FP),Y
   LDY   #$06
   LDA   (FP),Y
   LDY   #$0C
   ADC   (FP),Y
   LDY   #$0E
   STA   (FP),Y
   DEY
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
   LDY   #$02
   LDA   (DPTR),Y
   LDY   #$11
   STA   (FP),Y
   LDY   #$03
   LDA   (DPTR),Y
   LDY   #$12
   STA   (FP),Y
   LDY   #$0F
   LDA   (FP),Y
   ASL
   INY
   LDA   (FP),Y
   ROL
   LDY   #$13
   STA   (FP),Y
   LDY   #$11
   LDA   (FP),Y
   ROL
   LDY   #$14
   STA   (FP),Y
   LDY   #$12
   LDA   (FP),Y
   ROL
   LDY   #$15
   STA   (FP),Y
   LDA   #$00
   ROL
   INY
   STA   (FP),Y
   LDY   #$0D
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
   LDY   #$15
   LDA   (FP),Y
   LDY   #$02
   STA   (DPTR),Y
   LDY   #$16
   LDA   (FP),Y
   LDY   #$03
   STA   (DPTR),Y
   LDA   #$00
   LDY   #$17
   STA   (FP),Y
   INY
   STA   (FP),Y
.loop@7_start:
   LDA   #$06
   LDY   #$19
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   INY
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
   INY
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
   LDY   #$21
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   gp7
   STA   DPTR
   LDA   gp7+1
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$25
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$26
   STA   (FP),Y
   LDY   #$02
   LDA   (DPTR),Y
   LDY   #$27
   STA   (FP),Y
   LDY   #$03
   LDA   (DPTR),Y
   LDY   #$28
   STA   (FP),Y
   LDY   #$04
   LDA   (DPTR),Y
   LDY   #$29
   STA   (FP),Y
   LDY   #$05
   LDA   (DPTR),Y
   LDY   #$2A
   STA   (FP),Y
   LDY   #$06
   LDA   (DPTR),Y
   LDY   #$2B
   STA   (FP),Y
   LDY   #$07
   LDA   (DPTR),Y
   LDY   #$2C
   STA   (FP),Y
   LDY   #$1D
   LDA   (FP),Y
   CLC
   LDY   #$25
   ADC   (FP),Y
   LDY   #$2D
   STA   (FP),Y
   LDY   #$1E
   LDA   (FP),Y
   LDY   #$26
   ADC   (FP),Y
   LDY   #$2E
   STA   (FP),Y
   LDY   #$1F
   LDA   (FP),Y
   LDY   #$27
   ADC   (FP),Y
   LDY   #$2F
   STA   (FP),Y
   LDY   #$20
   LDA   (FP),Y
   LDY   #$28
   ADC   (FP),Y
   LDY   #$30
   STA   (FP),Y
   LDY   #$21
   LDA   (FP),Y
   LDY   #$29
   ADC   (FP),Y
   LDY   #$31
   STA   (FP),Y
   LDY   #$22
   LDA   (FP),Y
   LDY   #$2A
   ADC   (FP),Y
   LDY   #$32
   STA   (FP),Y
   LDY   #$23
   LDA   (FP),Y
   LDY   #$2B
   ADC   (FP),Y
   LDY   #$33
   STA   (FP),Y
   LDY   #$24
   LDA   (FP),Y
   LDY   #$2C
   ADC   (FP),Y
   LDY   #$34
   STA   (FP),Y
   LDY   #$2D
   LDA   (FP),Y
   LDY   #$35
   STA   (FP),Y
   LDY   #$2E
   LDA   (FP),Y
   LDY   #$36
   STA   (FP),Y
   LDY   #$2F
   LDA   (FP),Y
   LDY   #$37
   STA   (FP),Y
   LDY   #$30
   LDA   (FP),Y
   LDY   #$38
   STA   (FP),Y
   LDY   #$31
   LDA   (FP),Y
   LDY   #$39
   STA   (FP),Y
   LDY   #$32
   LDA   (FP),Y
   LDY   #$3A
   STA   (FP),Y
   LDY   #$33
   LDA   (FP),Y
   LDY   #$3B
   STA   (FP),Y
   LDY   #$34
   LDA   (FP),Y
   AND   #$1F
   LDY   #$3C
   STA   (FP),Y
   LDY   #$35
   LDA   (FP),Y
   LDY   #$3D
   STA   (FP),Y
   LDY   #$36
   LDA   (FP),Y
   LDY   #$3E
   STA   (FP),Y
   LDY   #$37
   LDA   (FP),Y
   LDY   #$3F
   STA   (FP),Y
   LDY   #$38
   LDA   (FP),Y
   LDY   #$40
   STA   (FP),Y
   LDY   #$39
   LDA   (FP),Y
   LDY   #$41
   STA   (FP),Y
   LDY   #$3A
   LDA   (FP),Y
   LDY   #$42
   STA   (FP),Y
   LDY   #$3B
   LDA   (FP),Y
   LDY   #$43
   STA   (FP),Y
   LDY   #$3C
   LDA   (FP),Y
   EOR   #$10
   LDY   #$44
   STA   (FP),Y
   LDY   #$3D
   LDA   (FP),Y
   LDY   #$45
   STA   (FP),Y
   LDY   #$3E
   LDA   (FP),Y
   LDY   #$46
   STA   (FP),Y
   LDY   #$3F
   LDA   (FP),Y
   LDY   #$47
   STA   (FP),Y
   LDY   #$40
   LDA   (FP),Y
   LDY   #$48
   STA   (FP),Y
   LDY   #$41
   LDA   (FP),Y
   LDY   #$49
   STA   (FP),Y
   LDY   #$42
   LDA   (FP),Y
   LDY   #$4A
   STA   (FP),Y
   LDY   #$43
   LDA   (FP),Y
   LDY   #$4B
   STA   (FP),Y
   LDY   #$44
   LDA   (FP),Y
   SEC
   SBC   #$10
   LDY   #$4C
   STA   (FP),Y
   LDY   #$45
   LDA   (FP),Y
   LDY   #$4D
   STA   (FP),Y
   LDY   #$46
   LDA   (FP),Y
   LDY   #$4E
   STA   (FP),Y
   LDA   #$07
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$4D
   LDA   (FP),Y
   LDY   #$4F
   AND   (FP),Y
   LDY   #$51
   STA   (FP),Y
   LDY   #$4E
   LDA   (FP),Y
   LDY   #$50
   AND   (FP),Y
   LDY   #$52
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CMP   #$02
   BEQ   .cmp_true@45
   LDA   #$00
   JMP   .cmp_end@46
.cmp_true@45:
   LDA   #$01
.cmp_end@46:
   LDY   #$53
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .case@9
   LDY   #$51
   LDA   (FP),Y
   CMP   #$03
   BEQ   .cmp_true@47
   LDA   #$00
   JMP   .cmp_end@48
.cmp_true@47:
   LDA   #$01
.cmp_end@48:
   LDY   #$55
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@5
   JMP   .switch@8_break
.lb_skip@5:
   LDY   #$51
   LDA   (FP),Y
   CMP   #$07
   BEQ   .cmp_true@49
   LDA   #$00
   JMP   .cmp_end@50
.cmp_true@49:
   LDA   #$01
.cmp_end@50:
   LDY   #$57
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@4
   JMP   .case@13
.lb_skip@4:
   JMP   .default@14
.case@9:
   LDY   #$18
   LDA   (FP),Y
   LDY   #$59
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
   LDY   #$59
   LDA   (FP),Y
   LDY   #$5B
   AND   (FP),Y
   LDY   #$5D
   STA   (FP),Y
   LDY   #$5A
   LDA   (FP),Y
   LDY   #$5C
   AND   (FP),Y
   LDY   #$5E
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$5F
   STA   (FP),Y
   STA   HARGS
   LDA   #$04
   STA   HARGS+1
   JSR   mul8
   LDA   HARGS+2
   LDY   #$60
   STA   (FP),Y
   TAX
   LDA   ga5,X
   INY
   STA   (FP),Y
   LDA   ga5+1,X
   INY
   STA   (FP),Y
   LDA   ga5+2,X
   INY
   STA   (FP),Y
   LDA   ga5+3,X
   INY
   STA   (FP),Y
   LDY   #$61
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   INY
   ORA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@0
   LDY   #$18
   LDA   (FP),Y
   BEQ   .and_false@0
   LDA   #$01
   LDY   #$65
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   JMP   .and_end@1
.and_false@0:
   LDA   #$00
   LDY   #$65
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@1:
   LDA   #$07
   LDY   #$67
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$65
   LDA   (FP),Y
   LDY   #$67
   AND   (FP),Y
   LDY   #$69
   STA   (FP),Y
   LDY   #$66
   LDA   (FP),Y
   LDY   #$68
   AND   (FP),Y
   LDY   #$6A
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CMP   #$04
   BEQ   .cmp_true@51
   LDA   #$00
   JMP   .cmp_end@52
.cmp_true@51:
   LDA   #$01
.cmp_end@52:
   LDY   #$6B
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@3
   JMP   .switch@8_break
.lb_skip@3:
   LDA   #<gs6
   LDY   #$6D
   STA   (FP),Y
   LDA   #>gs6
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$02
   LDY   #$6F
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$70
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$71
   STA   (FP),Y
   INY
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@53
   LDA   #$00
   JMP   .sx_done@54
.sx_neg@53:
   LDA   #$FF
.sx_done@54:
   LDY   #$73
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   HARGS
   INY
   LDA   (FP),Y
   STA   HARGS+1
   LDA   #$5A
   STA   HARGS+2
   LDA   #$00
   STA   HARGS+3
   JSR   mul16
   LDA   HARGS+4
   LDY   #$74
   STA   (FP),Y
   LDA   HARGS+5
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$76
   STA   (FP),Y
   LDY   #$6F
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$76
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   JMP   .switch@8_break
.case@13:
.default@14:
   LDA   #$00
   LDY   #$77
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   SEC
   SBC   #$03
   LDA   (FP),Y
   SBC   #$00
   BVC   .cmp_novf@55
   EOR   #$80
.cmp_novf@55:
   ASL
   LDA   #$00
   ROL
   LDY   #$7B
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@2
   JMP   .loop@15_break
.lb_skip@2:
   LDY   #$78
   LDA   (FP),Y
   LDY   #$7D
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDA   @7.s17
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   SEC
   LDY   #$7D
   SBC   (FP),Y
   LDY   #$80
   LDA   (FP),Y
   LDY   #$7E
   SBC   (FP),Y
   BVC   .cmp_novf@56
   EOR   #$80
.cmp_novf@56:
   ASL
   LDA   #$00
   ROL
   EOR   #$01
   LDY   #$81
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$83
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$84
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@57
   LDA   #$00
   JMP   .sx_done@58
.sx_neg@57:
   LDA   #$FF
.sx_done@58:
   LDY   #$85
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
   LDY   #$83
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$84
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$85
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$86
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   JSR   mix
.loop@15_break:
   LDA   #<gs6
   LDY   #$95
   STA   (FP),Y
   LDA   #>gs6
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$02
   LDY   #$97
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$98
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$99
   STA   (FP),Y
   INY
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@59
   LDA   #$00
   JMP   .sx_done@60
.sx_neg@59:
   LDA   #$FF
.sx_done@60:
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
   CMP   #$04
   BEQ   .cmp_true@61
   LDA   #$00
   JMP   .cmp_end@62
.cmp_true@61:
   LDA   #$01
.cmp_end@62:
   LDY   #$A0
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
.switch@8_break:
   LDY   #$18
   LDA   (FP),Y
   CLC
   ADC   #$01
   LDY   #$A2
   STA   (FP),Y
   LDY   #$18
   STA   (FP),Y
   LDY   #$A3
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
   BVC   .cmp_novf@63
   EOR   #$80
.cmp_novf@63:
   ASL
   LDA   #$00
   ROL
   LDY   #$A5
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@1
   JMP   .loop@7_start
.lb_skip@1:
   LDY   #$03
   LDA   (FP),Y
   LDY   #$A7
   STA   (FP),Y
   LDY   #$04
   LDA   (FP),Y
   LDY   #$A8
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@64
   LDA   #$00
   JMP   .sx_done@65
.sx_neg@64:
   LDA   #$FF
.sx_done@65:
   LDY   #$A9
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
   LDA   z4+7
   CMP   (FP),Y
   BNE   .cmp_true@66
   LDA   z4+6
   DEY
   CMP   (FP),Y
   BNE   .cmp_true@66
   LDA   z4+5
   DEY
   CMP   (FP),Y
   BNE   .cmp_true@66
   LDA   z4+4
   DEY
   CMP   (FP),Y
   BNE   .cmp_true@66
   LDA   z4+3
   DEY
   CMP   (FP),Y
   BNE   .cmp_true@66
   LDA   z4+2
   DEY
   CMP   (FP),Y
   BNE   .cmp_true@66
   LDA   z4+1
   DEY
   CMP   (FP),Y
   BNE   .cmp_true@66
   LDA   z4
   DEY
   CMP   (FP),Y
   BNE   .cmp_true@66
   LDA   #$00
   JMP   .cmp_end@67
.cmp_true@66:
   LDA   #$01
.cmp_end@67:
   LDY   #$AF
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$B1
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$B2
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
   ADC   #$B6
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$B3
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

   ; prologue: 0 arg bytes, 195 local bytes
   SEC
   LDA   SSP
   SBC   #$C5
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$C4
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
   LDA   #$80
   INY
   STA   (FP),Y
   LDA   #$00
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
   LDA   z4
   SEC
   SBC   #$01
   INY
   STA   (FP),Y
   LDA   z4+1
   SBC   #$00
   INY
   STA   (FP),Y
   LDA   z4+2
   SBC   #$00
   INY
   STA   (FP),Y
   LDA   z4+3
   SBC   #$00
   INY
   STA   (FP),Y
   LDA   z4+4
   SBC   #$00
   INY
   STA   (FP),Y
   LDA   z4+5
   SBC   #$00
   INY
   STA   (FP),Y
   LDA   z4+6
   SBC   #$00
   INY
   STA   (FP),Y
   LDA   z4+7
   SBC   #$00
   INY
   STA   (FP),Y
   LDY   #$09
   LDA   (FP),Y
   LDY   #$11
   STA   (FP),Y
   LDY   #$0A
   LDA   (FP),Y
   LDY   #$12
   STA   (FP),Y
   LDY   #$0B
   LDA   (FP),Y
   LDY   #$13
   STA   (FP),Y
   LDY   #$0C
   LDA   (FP),Y
   LDY   #$14
   STA   (FP),Y
   LDY   #$0D
   LDA   (FP),Y
   LDY   #$15
   STA   (FP),Y
   LDY   #$0E
   LDA   (FP),Y
   LDY   #$16
   STA   (FP),Y
   LDY   #$0F
   LDA   (FP),Y
   LDY   #$17
   STA   (FP),Y
   LDY   #$10
   LDA   (FP),Y
   AND   #$1F
   LDY   #$18
   STA   (FP),Y
   LDY   #$11
   LDA   (FP),Y
   LDY   #$19
   STA   (FP),Y
   LDY   #$12
   LDA   (FP),Y
   LDY   #$1A
   STA   (FP),Y
   LDY   #$13
   LDA   (FP),Y
   LDY   #$1B
   STA   (FP),Y
   LDY   #$14
   LDA   (FP),Y
   LDY   #$1C
   STA   (FP),Y
   LDY   #$15
   LDA   (FP),Y
   LDY   #$1D
   STA   (FP),Y
   LDY   #$16
   LDA   (FP),Y
   LDY   #$1E
   STA   (FP),Y
   LDY   #$17
   LDA   (FP),Y
   LDY   #$1F
   STA   (FP),Y
   LDY   #$18
   LDA   (FP),Y
   EOR   #$10
   LDY   #$20
   STA   (FP),Y
   LDY   #$19
   LDA   (FP),Y
   LDY   #$21
   STA   (FP),Y
   LDY   #$1A
   LDA   (FP),Y
   LDY   #$22
   STA   (FP),Y
   LDY   #$1B
   LDA   (FP),Y
   LDY   #$23
   STA   (FP),Y
   LDY   #$1C
   LDA   (FP),Y
   LDY   #$24
   STA   (FP),Y
   LDY   #$1D
   LDA   (FP),Y
   LDY   #$25
   STA   (FP),Y
   LDY   #$1E
   LDA   (FP),Y
   LDY   #$26
   STA   (FP),Y
   LDY   #$1F
   LDA   (FP),Y
   LDY   #$27
   STA   (FP),Y
   LDY   #$20
   LDA   (FP),Y
   SEC
   SBC   #$10
   LDY   #$28
   STA   (FP),Y
   LDY   #$21
   LDA   (FP),Y
   STA   z4
   INY
   LDA   (FP),Y
   STA   z4+1
   INY
   LDA   (FP),Y
   STA   z4+2
   INY
   LDA   (FP),Y
   STA   z4+3
   INY
   LDA   (FP),Y
   STA   z4+4
   INY
   LDA   (FP),Y
   STA   z4+5
   INY
   LDA   (FP),Y
   STA   z4+6
   INY
   LDA   (FP),Y
   STA   z4+7
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
   STA   z4
   LDY   #$2C
   LDA   (FP),Y
   STA   z4+1
   INY
   LDA   (FP),Y
   STA   z4+2
   INY
   LDA   (FP),Y
   STA   z4+3
   INY
   LDA   (FP),Y
   STA   z4+4
   INY
   LDA   (FP),Y
   STA   z4+5
   INY
   LDA   (FP),Y
   STA   z4+6
   INY
   LDA   (FP),Y
   STA   z4+7
   LDY   #$07
   LDA   (FP),Y
   LDY   #$33
   STA   (FP),Y
   LDY   #$08
   LDA   (FP),Y
   LDY   #$34
   STA   (FP),Y
   LDX   #$06
.shift_loop@69:
   LDY   #$33
   LDA   (FP),Y
   ASL
   STA   (FP),Y
   INY
   LDA   (FP),Y
   ROL
   STA   (FP),Y
   DEX
   BNE   .shift_loop@69
   DEY
   LDA   (FP),Y
   LDY   #$07
   STA   (FP),Y
   LDY   #$34
   LDA   (FP),Y
   LDY   #$08
   STA   (FP),Y
   LDY   #$03
   LDA   (FP),Y
   LDY   #$35
   STA   (FP),Y
   LDY   #$04
   LDA   (FP),Y
   LDY   #$36
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@71
   LDA   #$00
   JMP   .sx_done@72
.sx_neg@71:
   LDA   #$FF
.sx_done@72:
   LDY   #$37
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
   LDA   z4
   SEC
   LDY   #$35
   SBC   (FP),Y
   LDY   #$3D
   STA   (FP),Y
   LDA   z4+1
   LDY   #$36
   SBC   (FP),Y
   LDY   #$3E
   STA   (FP),Y
   LDA   z4+2
   LDY   #$37
   SBC   (FP),Y
   LDY   #$3F
   STA   (FP),Y
   LDA   z4+3
   LDY   #$38
   SBC   (FP),Y
   LDY   #$40
   STA   (FP),Y
   LDA   z4+4
   LDY   #$39
   SBC   (FP),Y
   LDY   #$41
   STA   (FP),Y
   LDA   z4+5
   LDY   #$3A
   SBC   (FP),Y
   LDY   #$42
   STA   (FP),Y
   LDA   z4+6
   LDY   #$3B
   SBC   (FP),Y
   LDY   #$43
   STA   (FP),Y
   LDA   z4+7
   LDY   #$3C
   SBC   (FP),Y
   LDY   #$44
   STA   (FP),Y
   LDY   #$3D
   LDA   (FP),Y
   LDY   #$45
   STA   (FP),Y
   LDY   #$3E
   LDA   (FP),Y
   LDY   #$46
   STA   (FP),Y
   LDY   #$3F
   LDA   (FP),Y
   LDY   #$47
   STA   (FP),Y
   LDY   #$40
   LDA   (FP),Y
   LDY   #$48
   STA   (FP),Y
   LDY   #$41
   LDA   (FP),Y
   LDY   #$49
   STA   (FP),Y
   LDY   #$42
   LDA   (FP),Y
   LDY   #$4A
   STA   (FP),Y
   LDY   #$43
   LDA   (FP),Y
   LDY   #$4B
   STA   (FP),Y
   LDY   #$44
   LDA   (FP),Y
   AND   #$1F
   LDY   #$4C
   STA   (FP),Y
   LDY   #$45
   LDA   (FP),Y
   LDY   #$4D
   STA   (FP),Y
   LDY   #$46
   LDA   (FP),Y
   LDY   #$4E
   STA   (FP),Y
   LDY   #$47
   LDA   (FP),Y
   LDY   #$4F
   STA   (FP),Y
   LDY   #$48
   LDA   (FP),Y
   LDY   #$50
   STA   (FP),Y
   LDY   #$49
   LDA   (FP),Y
   LDY   #$51
   STA   (FP),Y
   LDY   #$4A
   LDA   (FP),Y
   LDY   #$52
   STA   (FP),Y
   LDY   #$4B
   LDA   (FP),Y
   LDY   #$53
   STA   (FP),Y
   LDY   #$4C
   LDA   (FP),Y
   EOR   #$10
   LDY   #$54
   STA   (FP),Y
   LDY   #$4D
   LDA   (FP),Y
   LDY   #$55
   STA   (FP),Y
   LDY   #$4E
   LDA   (FP),Y
   LDY   #$56
   STA   (FP),Y
   LDY   #$4F
   LDA   (FP),Y
   LDY   #$57
   STA   (FP),Y
   LDY   #$50
   LDA   (FP),Y
   LDY   #$58
   STA   (FP),Y
   LDY   #$51
   LDA   (FP),Y
   LDY   #$59
   STA   (FP),Y
   LDY   #$52
   LDA   (FP),Y
   LDY   #$5A
   STA   (FP),Y
   LDY   #$53
   LDA   (FP),Y
   LDY   #$5B
   STA   (FP),Y
   LDY   #$54
   LDA   (FP),Y
   SEC
   SBC   #$10
   LDY   #$5C
   STA   (FP),Y
   LDY   #$55
   LDA   (FP),Y
   LDY   #$5D
   STA   (FP),Y
   LDY   #$56
   LDA   (FP),Y
   LDY   #$5E
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
   LDY   #$5E
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   JSR   f14
   LDY   #$5F
   STA   (FP),Y
   TXA
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$61
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$62
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$03
   STA   (FP),Y
   LDY   #$62
   LDA   (FP),Y
   LDY   #$04
   STA   (FP),Y
   LDA   g3
   LDY   #$63
   STA   (FP),Y
   LDA   g3+1
   INY
   STA   (FP),Y
   LDA   g3+2
   INY
   STA   (FP),Y
   LDA   g3+3
   INY
   STA   (FP),Y
   LDA   g3+4
   INY
   STA   (FP),Y
   LDA   g3+5
   INY
   STA   (FP),Y
   LDA   g3+6
   INY
   STA   (FP),Y
   LDA   g3+7
   INY
   STA   (FP),Y
   LDA   g3
   CLC
   ADC   #$01
   INY
   STA   (FP),Y
   LDA   g3+1
   ADC   #$00
   INY
   STA   (FP),Y
   LDA   g3+2
   ADC   #$00
   INY
   STA   (FP),Y
   LDA   g3+3
   ADC   #$00
   INY
   STA   (FP),Y
   LDA   g3+4
   ADC   #$00
   INY
   STA   (FP),Y
   LDA   g3+5
   ADC   #$00
   INY
   STA   (FP),Y
   LDA   g3+6
   ADC   #$00
   INY
   STA   (FP),Y
   LDA   g3+7
   ADC   #$00
   INY
   STA   (FP),Y
   LDY   #$6B
   LDA   (FP),Y
   LDY   #$73
   STA   (FP),Y
   LDY   #$6C
   LDA   (FP),Y
   LDY   #$74
   STA   (FP),Y
   LDY   #$6D
   LDA   (FP),Y
   LDY   #$75
   STA   (FP),Y
   LDY   #$6E
   LDA   (FP),Y
   LDY   #$76
   STA   (FP),Y
   LDY   #$6F
   LDA   (FP),Y
   LDY   #$77
   STA   (FP),Y
   LDY   #$70
   LDA   (FP),Y
   LDY   #$78
   STA   (FP),Y
   LDY   #$71
   LDA   (FP),Y
   LDY   #$79
   STA   (FP),Y
   LDY   #$72
   LDA   (FP),Y
   AND   #$1F
   LDY   #$7A
   STA   (FP),Y
   LDY   #$73
   LDA   (FP),Y
   LDY   #$7B
   STA   (FP),Y
   LDY   #$74
   LDA   (FP),Y
   LDY   #$7C
   STA   (FP),Y
   LDY   #$75
   LDA   (FP),Y
   LDY   #$7D
   STA   (FP),Y
   LDY   #$76
   LDA   (FP),Y
   LDY   #$7E
   STA   (FP),Y
   LDY   #$77
   LDA   (FP),Y
   LDY   #$7F
   STA   (FP),Y
   LDY   #$78
   LDA   (FP),Y
   LDY   #$80
   STA   (FP),Y
   LDY   #$79
   LDA   (FP),Y
   LDY   #$81
   STA   (FP),Y
   LDY   #$7A
   LDA   (FP),Y
   EOR   #$10
   LDY   #$82
   STA   (FP),Y
   LDY   #$7B
   LDA   (FP),Y
   LDY   #$83
   STA   (FP),Y
   LDY   #$7C
   LDA   (FP),Y
   LDY   #$84
   STA   (FP),Y
   LDY   #$7D
   LDA   (FP),Y
   LDY   #$85
   STA   (FP),Y
   LDY   #$7E
   LDA   (FP),Y
   LDY   #$86
   STA   (FP),Y
   LDY   #$7F
   LDA   (FP),Y
   LDY   #$87
   STA   (FP),Y
   LDY   #$80
   LDA   (FP),Y
   LDY   #$88
   STA   (FP),Y
   LDY   #$81
   LDA   (FP),Y
   LDY   #$89
   STA   (FP),Y
   LDY   #$82
   LDA   (FP),Y
   SEC
   SBC   #$10
   LDY   #$8A
   STA   (FP),Y
   LDY   #$83
   LDA   (FP),Y
   STA   g3
   INY
   LDA   (FP),Y
   STA   g3+1
   INY
   LDA   (FP),Y
   STA   g3+2
   INY
   LDA   (FP),Y
   STA   g3+3
   INY
   LDA   (FP),Y
   STA   g3+4
   INY
   LDA   (FP),Y
   STA   g3+5
   INY
   LDA   (FP),Y
   STA   g3+6
   INY
   LDA   (FP),Y
   STA   g3+7
   LDY   #$07
   LDA   (FP),Y
   LDY   #$8B
   STA   (FP),Y
   LDY   #$08
   LDA   (FP),Y
   LDY   #$8C
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@73
   LDA   #$00
   JMP   .sx_done@74
.sx_neg@73:
   LDA   #$FF
.sx_done@74:
   LDY   #$8D
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$8B
   LDA   (FP),Y
   ORA   g1
   LDY   #$8F
   STA   (FP),Y
   LDY   #$8C
   LDA   (FP),Y
   ORA   g1+1
   LDY   #$90
   STA   (FP),Y
   LDY   #$8D
   LDA   (FP),Y
   ORA   g1+2
   LDY   #$91
   STA   (FP),Y
   LDY   #$8E
   LDA   (FP),Y
   ORA   g1+3
   LDY   #$92
   STA   (FP),Y
   LDY   #$8F
   LDA   (FP),Y
   LDY   #$93
   STA   (FP),Y
   LDY   #$90
   LDA   (FP),Y
   LDY   #$94
   STA   (FP),Y
   LDY   #$91
   LDA   (FP),Y
   LDY   #$95
   STA   (FP),Y
   LDY   #$92
   LDA   (FP),Y
   LDY   #$96
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
   LDY   #$63
   LDA   (FP),Y
   SEC
   LDY   #$93
   SBC   (FP),Y
   LDY   #$9B
   STA   (FP),Y
   LDY   #$64
   LDA   (FP),Y
   LDY   #$94
   SBC   (FP),Y
   LDY   #$9C
   STA   (FP),Y
   LDY   #$65
   LDA   (FP),Y
   LDY   #$95
   SBC   (FP),Y
   LDY   #$9D
   STA   (FP),Y
   LDY   #$66
   LDA   (FP),Y
   LDY   #$96
   SBC   (FP),Y
   LDY   #$9E
   STA   (FP),Y
   LDY   #$67
   LDA   (FP),Y
   LDY   #$97
   SBC   (FP),Y
   LDY   #$9F
   STA   (FP),Y
   LDY   #$68
   LDA   (FP),Y
   LDY   #$98
   SBC   (FP),Y
   LDY   #$A0
   STA   (FP),Y
   LDY   #$69
   LDA   (FP),Y
   LDY   #$99
   SBC   (FP),Y
   LDY   #$A1
   STA   (FP),Y
   LDY   #$6A
   LDA   (FP),Y
   LDY   #$9A
   SBC   (FP),Y
   LDY   #$A2
   STA   (FP),Y
   LDY   #$9B
   LDA   (FP),Y
   LDY   #$A3
   STA   (FP),Y
   LDY   #$9C
   LDA   (FP),Y
   LDY   #$A4
   STA   (FP),Y
   LDY   #$9D
   LDA   (FP),Y
   LDY   #$A5
   STA   (FP),Y
   LDY   #$9E
   LDA   (FP),Y
   LDY   #$A6
   STA   (FP),Y
   LDY   #$9F
   LDA   (FP),Y
   LDY   #$A7
   STA   (FP),Y
   LDY   #$A0
   LDA   (FP),Y
   LDY   #$A8
   STA   (FP),Y
   LDY   #$A1
   LDA   (FP),Y
   LDY   #$A9
   STA   (FP),Y
   LDY   #$A2
   LDA   (FP),Y
   AND   #$1F
   LDY   #$AA
   STA   (FP),Y
   LDY   #$A3
   LDA   (FP),Y
   LDY   #$AB
   STA   (FP),Y
   LDY   #$A4
   LDA   (FP),Y
   LDY   #$AC
   STA   (FP),Y
   LDY   #$A5
   LDA   (FP),Y
   LDY   #$AD
   STA   (FP),Y
   LDY   #$A6
   LDA   (FP),Y
   LDY   #$AE
   STA   (FP),Y
   LDY   #$A7
   LDA   (FP),Y
   LDY   #$AF
   STA   (FP),Y
   LDY   #$A8
   LDA   (FP),Y
   LDY   #$B0
   STA   (FP),Y
   LDY   #$A9
   LDA   (FP),Y
   LDY   #$B1
   STA   (FP),Y
   LDY   #$AA
   LDA   (FP),Y
   EOR   #$10
   LDY   #$B2
   STA   (FP),Y
   LDY   #$AB
   LDA   (FP),Y
   LDY   #$B3
   STA   (FP),Y
   LDY   #$AC
   LDA   (FP),Y
   LDY   #$B4
   STA   (FP),Y
   LDY   #$AD
   LDA   (FP),Y
   LDY   #$B5
   STA   (FP),Y
   LDY   #$AE
   LDA   (FP),Y
   LDY   #$B6
   STA   (FP),Y
   LDY   #$AF
   LDA   (FP),Y
   LDY   #$B7
   STA   (FP),Y
   LDY   #$B0
   LDA   (FP),Y
   LDY   #$B8
   STA   (FP),Y
   LDY   #$B1
   LDA   (FP),Y
   LDY   #$B9
   STA   (FP),Y
   LDY   #$B2
   LDA   (FP),Y
   SEC
   SBC   #$10
   LDY   #$BA
   STA   (FP),Y
   LDY   #$B3
   LDA   (FP),Y
   LDY   #$BB
   STA   (FP),Y
   LDA   #<gs6
   INY
   STA   (FP),Y
   LDA   #>gs6
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$01
   LDY   #$BE
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$BF
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$BB
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   JSR   fin0
   JSR   fin1
   JSR   fin2
   LDA   acc
   LDY   #$C0
   STA   (FP),Y
   LDA   acc+1
   AND   #$7F
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$C2
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$C3
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
   ADC   #$C5
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$C4
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
   DC.L  $00000100

g2:
   DC.B  $FF

g3:
   DC.L  $8E29336A
   DC.L  $FBBC97CC

ga5:
   DC.L  $00007FFF
   DC.L  $00000080
   DC.L  $0000000B
   DC.L  $00000001

gs6:
   DS.B  3

gp7:
   DC.W  g3

@1.s11:
   DC.L  $00000001

@7.s17:
   DC.B  $02

__zp_init:
   SUBROUTINE

   LDA   #$00
   STA   z4+1
   STA   z4+2
   STA   z4+3
   STA   z4+4
   STA   z4+5
   STA   z4+6
   STA   z4+7
   LDA   #$02
   STA   z4
   RTS
