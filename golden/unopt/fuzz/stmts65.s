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

   ; prologue: 0 arg bytes, 14 local bytes
   SEC
   LDA   SSP
   SBC   #$10
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$0F
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
   LDA   g2+1
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
   LDA   g3
   LDY   #$07
   STA   (FP),Y
   LDA   g3+1
   INY
   STA   (FP),Y
   LDA   g3+1
   BMI   .sx_neg@2
   LDA   #$00
   JMP   .sx_done@3
.sx_neg@2:
   LDA   #$FF
.sx_done@3:
   LDY   #$09
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
   LDA   z4
   LDY   #$0B
   STA   (FP),Y
   LDA   z4+1
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

   ; epilogue
   CLC
   LDA   FP
   ADC   #$10
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$0F
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

   LDA   ga5
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
   LDA   ga5+1
   LDY   #$06
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
   LDA   ga5+2
   LDY   #$0B
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
   LDA   ga5+3
   LDY   #$10
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

fin2:
   SUBROUTINE

   ; prologue: 0 arg bytes, 19 local bytes
   SEC
   LDA   SSP
   SBC   #$15
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$14
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
   BMI   .sx_neg@4
   LDA   #$00
   JMP   .sx_done@5
.sx_neg@4:
   LDA   #$FF
.sx_done@5:
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
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$0F
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
   BMI   .sx_neg@6
   LDA   #$00
   JMP   .sx_done@7
.sx_neg@6:
   LDA   #$FF
.sx_done@7:
   LDY   #$12
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
   LDY   #$10
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$11
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$12
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$13
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   JSR   mix

   ; epilogue
   CLC
   LDA   FP
   ADC   #$15
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$14
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   RTS

f7:
   SUBROUTINE

   ; prologue: 15 arg bytes, 134 local bytes
   SEC
   LDA   SSP
   SBC   #$88
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$87
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   LDA   #$FA
   LDY   #$01
   STA   (FP),Y
   LDA   #$5C
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
   LDY   #$89
   LDA   (FP),Y
   LDY   #$05
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@8
   LDA   #$00
   JMP   .sx_done@9
.sx_neg@8:
   LDA   #$FF
.sx_done@9:
   LDY   #$06
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$5A
   LDY   #$07
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$08
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$09
   STA   (FP),Y
   LDY   #$89
   STA   (FP),Y
   LDY   #$0A
   STA   (FP),Y
   SEC
   SBC   #$01
   INY
   STA   (FP),Y
   LDY   #$89
   STA   (FP),Y
   LDY   #$0A
   LDA   (FP),Y
   LDY   #$0C
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@10
   LDA   #$00
   JMP   .sx_done@11
.sx_neg@10:
   LDA   #$FF
.sx_done@11:
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
   LDA   #<gs6
   LDY   #$10
   STA   (FP),Y
   LDA   #>gs6
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$02
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
   EOR   #$FF
   LDY   #$16
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$17
   STA   (FP),Y
   LDY   #$12
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$16
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$17
   LDA   (FP),Y
   LDY   #$01
   STA   (DPTR),Y
   LDY   #$16
   LDA   (FP),Y
   LDY   #$18
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$19
   STA   (FP),Y
   LDA   #$01
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$18
   LDA   (FP),Y
   LDY   #$1A
   EOR   (FP),Y
   LDY   #$1C
   STA   (FP),Y
   LDY   #$19
   LDA   (FP),Y
   LDY   #$1B
   EOR   (FP),Y
   LDY   #$1D
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$1E
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$1F
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   g3
   INY
   LDA   (FP),Y
   STA   g3+1
   LDA   g2
   INY
   STA   (FP),Y
   LDA   g2+1
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$01
   LDY   #$22
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$23
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
.loop@0_start:
   LDA   #$00
   LDY   #$26
   STA   (FP),Y
   INY
   STA   (FP),Y
.loop@1_start:
   LDY   #$28
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
   BVC   .cmp_novf@12
   EOR   #$80
.cmp_novf@12:
   ASL
   LDA   #$00
   ROL
   LDY   #$2A
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .loop@1_break
   LDY   #$27
   LDA   (FP),Y
   LDY   #$2C
   STA   (FP),Y
   CLC
   ADC   #$01
   INY
   STA   (FP),Y
   LDY   #$27
   STA   (FP),Y
   JMP   .loop@1_start
.loop@1_break:
   LDA   g2
   LDY   #$2E
   STA   (FP),Y
   LDA   g2+1
   INY
   STA   (FP),Y
   LDA   #$07
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$2E
   LDA   (FP),Y
   LDY   #$30
   AND   (FP),Y
   LDY   #$32
   STA   (FP),Y
   LDY   #$2F
   LDA   (FP),Y
   LDY   #$31
   AND   (FP),Y
   LDY   #$33
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CMP   #$02
   BEQ   .cmp_true@13
   LDA   #$00
   JMP   .cmp_end@14
.cmp_true@13:
   LDA   #$01
.cmp_end@14:
   LDY   #$34
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .case@3
   LDY   #$32
   LDA   (FP),Y
   CMP   #$04
   BEQ   .cmp_true@15
   LDA   #$00
   JMP   .cmp_end@16
.cmp_true@15:
   LDA   #$01
.cmp_end@16:
   LDY   #$36
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@1
   JMP   .case@4
.lb_skip@1:
   JMP   .switch@2_break
.case@3:
   LDA   #<ga5
   LDY   #$38
   STA   (FP),Y
   LDA   #>ga5
   INY
   STA   (FP),Y
   LDY   #$25
   LDA   (FP),Y
   LDY   #$3A
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
   LDY   #$3A
   LDA   (FP),Y
   LDY   #$3C
   AND   (FP),Y
   LDY   #$3E
   STA   (FP),Y
   LDY   #$3B
   LDA   (FP),Y
   LDY   #$3D
   AND   (FP),Y
   LDY   #$3F
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$40
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$38
   LDA   (FP),Y
   CLC
   LDY   #$41
   ADC   (FP),Y
   LDY   #$43
   STA   (FP),Y
   LDY   #$39
   LDA   (FP),Y
   LDY   #$42
   ADC   (FP),Y
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
   CLC
   ADC   #$01
   INY
   STA   (FP),Y
   LDY   #$43
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$46
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$46
   LDA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$49
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$4A
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@17
   LDA   #$00
   JMP   .sx_done@18
.sx_neg@17:
   LDA   #$FF
.sx_done@18:
   LDY   #$4B
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   g1
   LDY   #$49
   EOR   (FP),Y
   LDY   #$4D
   STA   (FP),Y
   LDA   g1+1
   LDY   #$4A
   EOR   (FP),Y
   LDY   #$4E
   STA   (FP),Y
   LDA   g1+2
   LDY   #$4B
   EOR   (FP),Y
   LDY   #$4F
   STA   (FP),Y
   LDA   g1+3
   LDY   #$4C
   EOR   (FP),Y
   LDY   #$50
   STA   (FP),Y
   LDY   #$4D
   LDA   (FP),Y
   STA   g1
   INY
   LDA   (FP),Y
   STA   g1+1
   INY
   LDA   (FP),Y
   STA   g1+2
   INY
   LDA   (FP),Y
   STA   g1+3
   LDA   #<ga5
   INY
   STA   (FP),Y
   LDA   #>ga5
   INY
   STA   (FP),Y
   LDY   #$25
   LDA   (FP),Y
   LDY   #$53
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
   LDY   #$53
   LDA   (FP),Y
   LDY   #$55
   AND   (FP),Y
   LDY   #$57
   STA   (FP),Y
   LDY   #$54
   LDA   (FP),Y
   LDY   #$56
   AND   (FP),Y
   LDY   #$58
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$59
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$51
   LDA   (FP),Y
   CLC
   LDY   #$5A
   ADC   (FP),Y
   LDY   #$5C
   STA   (FP),Y
   LDY   #$52
   LDA   (FP),Y
   LDY   #$5B
   ADC   (FP),Y
   LDY   #$5D
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$5E
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   AND   #$01
   LDY   #$61
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDA   #$00
   LDY   #$62
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$63
   STA   (FP),Y
   LDY   #$5C
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$63
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$63
   LDA   (FP),Y
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
   ADC   #$97
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$87
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
.case@4:
   LDA   g3
   LDY   #$66
   STA   (FP),Y
   LDA   g3+1
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$03
   EOR   (FP),Y
   LDY   #$68
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$04
   EOR   (FP),Y
   LDY   #$69
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   z4
   INY
   LDA   (FP),Y
   STA   z4+1
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
   LDY   #$6C
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$6D
   STA   (FP),Y
   DEY
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
   DEY
   LDA   (FP),Y
   SEC
   SBC   #$01
   LDY   #$70
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   SBC   #$00
   LDY   #$71
   STA   (FP),Y
   LDY   #$6C
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$70
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$71
   LDA   (FP),Y
   LDY   #$01
   STA   (DPTR),Y
   LDA   #$07
   LDY   #$72
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$70
   LDA   (FP),Y
   LDY   #$72
   AND   (FP),Y
   LDY   #$74
   STA   (FP),Y
   LDY   #$71
   LDA   (FP),Y
   LDY   #$73
   AND   (FP),Y
   LDY   #$75
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CMP   #$02
   BEQ   .cmp_true@19
   LDA   #$00
   JMP   .cmp_end@20
.cmp_true@19:
   LDA   #$01
.cmp_end@20:
   LDY   #$76
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .case@6
   LDY   #$74
   LDA   (FP),Y
   CMP   #$05
   BEQ   .cmp_true@21
   LDA   #$00
   JMP   .cmp_end@22
.cmp_true@21:
   LDA   #$01
.cmp_end@22:
   LDY   #$78
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .case@7
.case@6:
.case@7:
.switch@2_break:
   LDY   #$25
   LDA   (FP),Y
   CLC
   ADC   #$01
   LDY   #$7A
   STA   (FP),Y
   LDY   #$25
   STA   (FP),Y
   LDY   #$7B
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
   BVC   .cmp_novf@23
   EOR   #$80
.cmp_novf@23:
   ASL
   LDA   #$00
   ROL
   LDY   #$7D
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@0
   JMP   .loop@0_start
.lb_skip@0:
   LDA   #$00
   LDY   #$7F
   STA   (FP),Y
   INY
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@24
   LDA   #$00
   JMP   .sx_done@25
.sx_neg@24:
   LDA   #$FF
.sx_done@25:
   LDY   #$81
   STA   (FP),Y
   LDA   ga5+1
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$80
   LDA   (FP),Y
   SEC
   LDY   #$83
   SBC   (FP),Y
   LDY   #$85
   STA   (FP),Y
   LDY   #$81
   LDA   (FP),Y
   LDY   #$84
   SBC   (FP),Y
   LDY   #$86
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
   ADC   #$97
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$87
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

f15:
   SUBROUTINE

   ; prologue: 2 arg bytes, 95 local bytes
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

   LDA   #$7F
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
   LDA   #$00
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
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
.loop@9_continue:
   LDY   #$14
   LDA   (FP),Y
   INY
   STA   (FP),Y
   CLC
   ADC   #$01
   INY
   STA   (FP),Y
   LDY   #$14
   STA   (FP),Y
   INY
   LDA   (FP),Y
   LDY   #$17
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
   BVC   .cmp_novf@26
   EOR   #$80
.cmp_novf@26:
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
   INY
   ORA   (FP),Y
   BNE   .loop@9_continue
   LDA   g3
   LDY   #$03
   STA   (FP),Y
   LDA   g3+1
   INY
   STA   (FP),Y
   LDA   #<gs6
   LDY   #$1C
   STA   (FP),Y
   LDA   #>gs6
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$02
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
   DEY
   LDA   (FP),Y
   LDY   #$22
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$23
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@27
   LDA   #$00
   JMP   .sx_done@28
.sx_neg@27:
   LDA   #$FF
.sx_done@28:
   LDY   #$24
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   g1
   SEC
   LDY   #$22
   SBC   (FP),Y
   LDY   #$26
   STA   (FP),Y
   LDA   g1+1
   LDY   #$23
   SBC   (FP),Y
   LDY   #$27
   STA   (FP),Y
   LDA   g1+2
   LDY   #$24
   SBC   (FP),Y
   LDY   #$28
   STA   (FP),Y
   LDA   g1+3
   LDY   #$25
   SBC   (FP),Y
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
   DEY
   LDA   (FP),Y
   STA   z4
   INY
   LDA   (FP),Y
   STA   z4+1
   LDY   #$62
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$2C
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$2D
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$2E
   STA   (FP),Y
   LDY   #$03
   LDA   (FP),Y
   LDY   #$2F
   STA   (FP),Y
   LDY   #$04
   LDA   (FP),Y
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
   BMI   .sx_neg@29
   LDA   #$00
   JMP   .sx_done@30
.sx_neg@29:
   LDA   #$FF
.sx_done@30:
   LDY   #$33
   STA   (FP),Y
   INY
   STA   (FP),Y
   CLC
   LDA   FP
   ADC   #$0F
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
   LDY   #$2E
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$31
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$32
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$33
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   LDY   #$34
   LDA   (FP),Y
   LDY   #$05
   STA   (SSP),Y
   LDA   #$00
   INY
   STA   (SSP),Y
   LDA   #$80
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
   INY
   STA   (SSP),Y
   LDY   #$35
   LDA   (FP),Y
   LDY   #$0E
   STA   (SSP),Y
   LDY   #$36
   LDA   (FP),Y
   LDY   #$0F
   STA   (SSP),Y
   JSR   f7
   LDY   #$37
   STA   (FP),Y
   TXA
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
.loop@10_start:
   LDY   #$3B
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
   BVC   .cmp_novf@31
   EOR   #$80
.cmp_novf@31:
   ASL
   LDA   #$00
   ROL
   LDY   #$3D
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@3
   JMP   .loop@10_break
.lb_skip@3:
   LDA   g1
   LDY   #$3F
   STA   (FP),Y
   ASL
   STA   (FP),Y
   LDA   g1+1
   INY
   STA   (FP),Y
   ROL
   STA   (FP),Y
   LDA   g1+2
   INY
   STA   (FP),Y
   ROL
   STA   (FP),Y
   LDA   g1+3
   INY
   STA   (FP),Y
   ROL
   STA   (FP),Y
   LDY   #$3F
   LDA   (FP),Y
   ASL
   STA   (FP),Y
   INY
   LDA   (FP),Y
   ROL
   STA   (FP),Y
   INY
   LDA   (FP),Y
   ROL
   STA   (FP),Y
   INY
   LDA   (FP),Y
   ROL
   STA   (FP),Y
   LDY   #$3F
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   INY
   ORA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@2
   JMP   .if_end@0
.lb_skip@2:
   LDA   #$E8
   LDY   #$43
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
   ORA   #$00
   BMI   .sx_neg@32
   LDA   #$00
   JMP   .sx_done@33
.sx_neg@32:
   LDA   #$FF
.sx_done@33:
   LDY   #$48
   STA   (FP),Y
   LDY   #$44
   LDA   (FP),Y
   SEC
   LDY   #$47
   SBC   (FP),Y
   LDY   #$49
   STA   (FP),Y
   LDY   #$45
   LDA   (FP),Y
   LDY   #$48
   SBC   (FP),Y
   LDY   #$4A
   STA   (FP),Y
   LDA   #$07
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$49
   LDA   (FP),Y
   LDY   #$4B
   AND   (FP),Y
   LDY   #$4D
   STA   (FP),Y
   LDY   #$4A
   LDA   (FP),Y
   LDY   #$4C
   AND   (FP),Y
   LDY   #$4E
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CMP   #$01
   BEQ   .cmp_true@34
   LDA   #$00
   JMP   .cmp_end@35
.cmp_true@34:
   LDA   #$01
.cmp_end@35:
   LDY   #$4F
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .switch@11_break
   LDY   #$4D
   LDA   (FP),Y
   CMP   #$03
   BEQ   .cmp_true@36
   LDA   #$00
   JMP   .cmp_end@37
.cmp_true@36:
   LDA   #$01
.cmp_end@37:
   LDY   #$51
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .case@13
   LDY   #$4D
   LDA   (FP),Y
   CMP   #$07
   BEQ   .cmp_true@38
   LDA   #$00
   JMP   .cmp_end@39
.cmp_true@38:
   LDA   #$01
.cmp_end@39:
   LDY   #$53
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .switch@11_break
   JMP   .default@15
.case@13:
   LDA   g3
   SEC
   SBC   #$01
   LDY   #$55
   STA   (FP),Y
   LDA   g3+1
   SBC   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   g3
   INY
   LDA   (FP),Y
   STA   g3+1
.default@15:
.switch@11_break:
.if_end@0:
   LDY   #$3A
   LDA   (FP),Y
   LDY   #$57
   STA   (FP),Y
   CLC
   ADC   #$01
   INY
   STA   (FP),Y
   LDY   #$3A
   STA   (FP),Y
   JMP   .loop@10_start
.loop@10_break:
   LDA   #<gs6
   LDY   #$59
   STA   (FP),Y
   LDA   #>gs6
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
   INY
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@40
   LDA   #$00
   JMP   .sx_done@41
.sx_neg@40:
   LDA   #$FF
.sx_done@41:
   LDY   #$5F
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
   ADC   #$63
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
   TAX
   PLA
   RTS

main:
   SUBROUTINE

   ; prologue: 0 arg bytes, 101 local bytes
   SEC
   LDA   SSP
   SBC   #$67
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$66
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
   DEY
   LDA   (FP),Y
   LDY   #$03
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$04
   STA   (FP),Y
   LDA   #$7F
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$05
   LDA   (FP),Y
   LDY   #$09
   STA   (FP),Y
   LDY   #$06
   LDA   (FP),Y
   LDY   #$0A
   STA   (FP),Y
   INY
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
   LDY   #$0F
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$10
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$11
   STA   (FP),Y
   INY
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@42
   LDA   #$00
   JMP   .sx_done@43
.sx_neg@42:
   LDA   #$FF
.sx_done@43:
   LDY   #$13
   STA   (FP),Y
   LDA   #$02
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$12
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
   LDY   #$16
   STA   (FP),Y
   LDA   HARGS+7
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$18
   STA   (FP),Y
   LDA   g3
   INY
   STA   (FP),Y
   LDA   g3+1
   INY
   STA   (FP),Y
   LDA   g3+1
   BMI   .sx_neg@44
   LDA   #$00
   JMP   .sx_done@45
.sx_neg@44:
   LDA   #$FF
.sx_done@45:
   LDY   #$1B
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   g1
   SEC
   LDY   #$19
   SBC   (FP),Y
   LDY   #$1D
   STA   (FP),Y
   LDA   g1+1
   LDY   #$1A
   SBC   (FP),Y
   LDY   #$1E
   STA   (FP),Y
   LDA   g1+2
   LDY   #$1B
   SBC   (FP),Y
   LDY   #$1F
   STA   (FP),Y
   LDA   g1+3
   LDY   #$1C
   SBC   (FP),Y
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
   LDY   #$1F
   LDA   (FP),Y
   LDY   #$23
   STA   (FP),Y
   LDY   #$20
   LDA   (FP),Y
   LDY   #$24
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
   CLC
   LDA   FP
   ADC   #$09
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
   LDY   #$18
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$21
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$22
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$23
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   LDY   #$24
   LDA   (FP),Y
   LDY   #$05
   STA   (SSP),Y
   LDY   #$27
   LDA   (FP),Y
   LDY   #$06
   STA   (SSP),Y
   LDY   #$28
   LDA   (FP),Y
   LDY   #$07
   STA   (SSP),Y
   LDY   #$29
   LDA   (FP),Y
   LDY   #$08
   STA   (SSP),Y
   LDY   #$2A
   LDA   (FP),Y
   LDY   #$09
   STA   (SSP),Y
   LDY   #$2B
   LDA   (FP),Y
   LDY   #$0A
   STA   (SSP),Y
   LDY   #$2C
   LDA   (FP),Y
   LDY   #$0B
   STA   (SSP),Y
   LDY   #$2D
   LDA   (FP),Y
   LDY   #$0C
   STA   (SSP),Y
   LDY   #$2E
   LDA   (FP),Y
   LDY   #$0D
   STA   (SSP),Y
   LDY   #$2F
   LDA   (FP),Y
   LDY   #$0E
   STA   (SSP),Y
   LDY   #$30
   LDA   (FP),Y
   LDY   #$0F
   STA   (SSP),Y
   JSR   f7
   LDY   #$31
   STA   (FP),Y
   TXA
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$33
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$34
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@46
   LDA   #$00
   JMP   .sx_done@47
.sx_neg@46:
   LDA   #$FF
.sx_done@47:
   LDY   #$35
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$33
   LDA   (FP),Y
   LDY   #$09
   STA   (FP),Y
   LDY   #$34
   LDA   (FP),Y
   LDY   #$0A
   STA   (FP),Y
   LDY   #$35
   LDA   (FP),Y
   LDY   #$0B
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #<gs6
   LDY   #$37
   STA   (FP),Y
   LDA   #>gs6
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$01
   LDY   #$39
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$3A
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$3B
   STA   (FP),Y
   INY
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@48
   LDA   #$00
   JMP   .sx_done@49
.sx_neg@48:
   LDA   #$FF
.sx_done@49:
   LDY   #$3D
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ORA   #$05
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
   LDY   #$39
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$40
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$40
   LDA   (FP),Y
   INY
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@50
   LDA   #$00
   JMP   .sx_done@51
.sx_neg@50:
   LDA   #$FF
.sx_done@51:
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
   LDA   #$0F
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$43
   LDA   (FP),Y
   LDY   #$45
   AND   (FP),Y
   LDY   #$47
   STA   (FP),Y
   LDY   #$44
   LDA   (FP),Y
   LDY   #$46
   AND   (FP),Y
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
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$49
   LDA   (FP),Y
   LDY   #$09
   STA   (FP),Y
   LDY   #$4A
   LDA   (FP),Y
   LDY   #$0A
   STA   (FP),Y
   LDY   #$4B
   LDA   (FP),Y
   LDY   #$0B
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   z4
   LDY   #$4D
   STA   (FP),Y
   LDA   z4+1
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   g1
   LDY   #$4D
   AND   (FP),Y
   LDY   #$51
   STA   (FP),Y
   LDA   g1+1
   LDY   #$4E
   AND   (FP),Y
   LDY   #$52
   STA   (FP),Y
   LDA   g1+2
   LDY   #$4F
   AND   (FP),Y
   LDY   #$53
   STA   (FP),Y
   LDA   g1+3
   LDY   #$50
   AND   (FP),Y
   LDY   #$54
   STA   (FP),Y
   LDY   #$51
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   INY
   ORA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .cond_else@1
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
   INY
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@52
   LDA   #$00
   JMP   .sx_done@53
.sx_neg@52:
   LDA   #$FF
.sx_done@53:
   LDY   #$5B
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$5C
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$5D
   STA   (FP),Y
   JMP   .cond_end@2
.cond_else@1:
   LDA   g3
   LDY   #$5C
   STA   (FP),Y
   LDA   g3+1
   INY
   STA   (FP),Y
.cond_end@2:
   LDA   #<gs6
   LDY   #$5E
   STA   (FP),Y
   LDA   #>gs6
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$02
   LDY   #$60
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$61
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$5C
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$5D
   LDA   (FP),Y
   LDY   #$01
   STA   (DPTR),Y
   JSR   fin0
   JSR   fin1
   JSR   fin2
   LDA   acc
   LDY   #$62
   STA   (FP),Y
   LDA   acc+1
   AND   #$7F
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$64
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$65
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
   ADC   #$67
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$66
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
   DC.L  $0000FFFF

g2:
   DC.W  $0080

g3:
   DC.W  $00FF

ga5:
   DC.B  $01, $FF, $03, $00

gs6:
   DS.B  1
   DC.B  $7F
   DC.W  $0001

@9.s21:
   DC.B  $03

__zp_init:
   SUBROUTINE

   LDA   #$00
   STA   z4+1
   LDA   #$03
   STA   z4
   RTS
