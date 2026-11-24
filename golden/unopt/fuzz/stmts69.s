SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
z3	EQU	$26

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

   LDA   g1
   LDY   #$01
   STA   (FP),Y
   LDA   g1+1
   INY
   STA   (FP),Y
   LDA   g1+1
   BMI   .sx_neg@0
   LDA   #$00
   JMP   .sx_done@1
.sx_neg@0:
   LDA   #$FF
.sx_done@1:
   LDY   #$03
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
   LDA   z3
   LDY   #$09
   STA   (FP),Y
   LDA   z3+1
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
   LDA   ga4
   LDY   #$0D
   STA   (FP),Y
   LDA   ga4+1
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

fin1:
   SUBROUTINE

   ; prologue: 0 arg bytes, 26 local bytes
   SEC
   LDA   SSP
   SBC   #$1C
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$1B
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   LDA   ga4+2
   LDY   #$01
   STA   (FP),Y
   LDA   ga4+3
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
   LDA   ga4+4
   LDY   #$07
   STA   (FP),Y
   LDA   ga4+5
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
   LDA   ga4+6
   LDY   #$0D
   STA   (FP),Y
   LDA   ga4+7
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
   LDA   #<gs5
   LDY   #$13
   STA   (FP),Y
   LDA   #>gs5
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$01
   LDY   #$15
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$16
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$17
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$18
   STA   (FP),Y
   LDY   #$02
   LDA   (DPTR),Y
   LDY   #$19
   STA   (FP),Y
   LDY   #$03
   LDA   (DPTR),Y
   LDY   #$1A
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$04
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDY   #$17
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$18
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$19
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$1A
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   JSR   mix

   ; epilogue
   CLC
   LDA   FP
   ADC   #$1C
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$1B
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

   ; prologue: 0 arg bytes, 8 local bytes
   SEC
   LDA   SSP
   SBC   #$0A
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$09
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   LDA   #<gs5
   LDY   #$01
   STA   (FP),Y
   LDA   #>gs5
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$05
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
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$06
   STA   (FP),Y
   LDY   #$02
   LDA   (DPTR),Y
   LDY   #$07
   STA   (FP),Y
   LDY   #$03
   LDA   (DPTR),Y
   LDY   #$08
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

   ; epilogue
   CLC
   LDA   FP
   ADC   #$0A
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$09
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   RTS

f6:
   SUBROUTINE

   ; prologue: 4 arg bytes, 171 local bytes
   SEC
   LDA   SSP
   SBC   #$AD
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$AC
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
   LDA   #$02
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
   LDY   #$01
   LDA   (FP),Y
   LDY   #$0D
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   LDY   #$0E
   STA   (FP),Y
   LDY   #$03
   LDA   (FP),Y
   LDY   #$0F
   STA   (FP),Y
   LDY   #$04
   LDA   (FP),Y
   LDY   #$10
   STA   (FP),Y
   LDY   #$05
   LDA   (FP),Y
   LDY   #$11
   STA   (FP),Y
   LDY   #$06
   LDA   (FP),Y
   LDY   #$12
   STA   (FP),Y
   LDY   #$07
   LDA   (FP),Y
   LDY   #$13
   STA   (FP),Y
   LDY   #$08
   LDA   (FP),Y
   LDY   #$14
   STA   (FP),Y
   LDY   #$01
   LDA   (FP),Y
   SEC
   SBC   #$01
   LDY   #$15
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   SBC   #$00
   LDY   #$16
   STA   (FP),Y
   LDY   #$03
   LDA   (FP),Y
   SBC   #$00
   LDY   #$17
   STA   (FP),Y
   LDY   #$04
   LDA   (FP),Y
   SBC   #$00
   LDY   #$18
   STA   (FP),Y
   LDY   #$05
   LDA   (FP),Y
   SBC   #$00
   LDY   #$19
   STA   (FP),Y
   LDY   #$06
   LDA   (FP),Y
   SBC   #$00
   LDY   #$1A
   STA   (FP),Y
   LDY   #$07
   LDA   (FP),Y
   SBC   #$00
   LDY   #$1B
   STA   (FP),Y
   LDY   #$08
   LDA   (FP),Y
   SBC   #$00
   LDY   #$1C
   STA   (FP),Y
   LDY   #$15
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   LDY   #$16
   LDA   (FP),Y
   LDY   #$02
   STA   (FP),Y
   LDY   #$17
   LDA   (FP),Y
   LDY   #$03
   STA   (FP),Y
   LDY   #$18
   LDA   (FP),Y
   LDY   #$04
   STA   (FP),Y
   LDY   #$19
   LDA   (FP),Y
   LDY   #$05
   STA   (FP),Y
   LDY   #$1A
   LDA   (FP),Y
   LDY   #$06
   STA   (FP),Y
   LDY   #$1B
   LDA   (FP),Y
   LDY   #$07
   STA   (FP),Y
   LDY   #$1C
   LDA   (FP),Y
   LDY   #$08
   STA   (FP),Y
   LDA   #<gs5
   LDY   #$1D
   STA   (FP),Y
   LDA   #>gs5
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$01
   LDY   #$1F
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$20
   STA   (FP),Y
   DEY
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
   LDY   #$02
   LDA   (DPTR),Y
   LDY   #$23
   STA   (FP),Y
   LDY   #$03
   LDA   (DPTR),Y
   LDY   #$24
   STA   (FP),Y
   LDY   #$21
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
   LDA   #$03
   STA   HARGS+4
   JSR   lsr32
   LDA   HARGS+5
   LDY   #$25
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
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$0D
   LDA   (FP),Y
   LDY   #$29
   AND   (FP),Y
   LDY   #$31
   STA   (FP),Y
   LDY   #$0E
   LDA   (FP),Y
   LDY   #$2A
   AND   (FP),Y
   LDY   #$32
   STA   (FP),Y
   LDY   #$0F
   LDA   (FP),Y
   LDY   #$2B
   AND   (FP),Y
   LDY   #$33
   STA   (FP),Y
   LDY   #$10
   LDA   (FP),Y
   LDY   #$2C
   AND   (FP),Y
   LDY   #$34
   STA   (FP),Y
   LDY   #$11
   LDA   (FP),Y
   LDY   #$2D
   AND   (FP),Y
   LDY   #$35
   STA   (FP),Y
   LDY   #$12
   LDA   (FP),Y
   LDY   #$2E
   AND   (FP),Y
   LDY   #$36
   STA   (FP),Y
   LDY   #$13
   LDA   (FP),Y
   LDY   #$2F
   AND   (FP),Y
   LDY   #$37
   STA   (FP),Y
   LDY   #$14
   LDA   (FP),Y
   LDY   #$30
   AND   (FP),Y
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
   LDA   #$07
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$39
   LDA   (FP),Y
   LDY   #$3B
   AND   (FP),Y
   LDY   #$3D
   STA   (FP),Y
   LDY   #$3A
   LDA   (FP),Y
   LDY   #$3C
   AND   (FP),Y
   LDY   #$3E
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CMP   #$03
   BEQ   .cmp_true@2
   LDA   #$00
   JMP   .cmp_end@3
.cmp_true@2:
   LDA   #$01
.cmp_end@3:
   LDY   #$3F
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .case@1
   LDY   #$3D
   LDA   (FP),Y
   CMP   #$06
   BEQ   .cmp_true@4
   LDA   #$00
   JMP   .cmp_end@5
.cmp_true@4:
   LDA   #$01
.cmp_end@5:
   LDY   #$41
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@5
   JMP   .case@4
.lb_skip@5:
   LDY   #$3D
   LDA   (FP),Y
   CMP   #$07
   BEQ   .cmp_true@6
   LDA   #$00
   JMP   .cmp_end@7
.cmp_true@6:
   LDA   #$01
.cmp_end@7:
   LDY   #$43
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@4
   JMP   .case@10
.lb_skip@4:
   JMP   .default@11
.case@1:
   LDA   #$00
   LDY   #$45
   STA   (FP),Y
   INY
   STA   (FP),Y
.loop@2_start:
   LDY   #$47
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
   BVC   .cmp_novf@8
   EOR   #$80
.cmp_novf@8:
   ASL
   LDA   #$00
   ROL
   LDY   #$49
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@3
   JMP   .loop@2_break
.lb_skip@3:
   LDA   #$00
   LDY   #$4B
   STA   (FP),Y
   INY
   STA   (FP),Y
.loop@3_start:
   LDA   z3
   ORA   #$81
   LDY   #$4D
   STA   (FP),Y
   LDA   z3+1
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   z3
   INY
   LDA   (FP),Y
   STA   z3+1
   LDY   #$4C
   LDA   (FP),Y
   CLC
   ADC   #$01
   LDY   #$4F
   STA   (FP),Y
   LDY   #$4C
   STA   (FP),Y
   LDY   #$50
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
   BVC   .cmp_novf@9
   EOR   #$80
.cmp_novf@9:
   ASL
   LDA   #$00
   ROL
   LDY   #$52
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .loop@3_start
   LDY   #$46
   LDA   (FP),Y
   LDY   #$54
   STA   (FP),Y
   CLC
   ADC   #$01
   INY
   STA   (FP),Y
   LDY   #$46
   STA   (FP),Y
   JMP   .loop@2_start
.loop@2_break:
.case@4:
   LDA   #<gs5
   LDY   #$56
   STA   (FP),Y
   LDA   #>gs5
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$05
   LDY   #$58
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$59
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$5A
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$5B
   STA   (FP),Y
   LDY   #$02
   LDA   (DPTR),Y
   LDY   #$5C
   STA   (FP),Y
   LDY   #$03
   LDA   (DPTR),Y
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
   ORA   #$00
   BMI   .sx_neg@10
   LDA   #$00
   JMP   .sx_done@11
.sx_neg@10:
   LDA   #$FF
.sx_done@11:
   LDY   #$62
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$5D
   LDA   (FP),Y
   LDY   #$63
   CMP   (FP),Y
   BNE   .cmp_true@12
   LDY   #$5C
   LDA   (FP),Y
   LDY   #$62
   CMP   (FP),Y
   BNE   .cmp_true@12
   LDY   #$5B
   LDA   (FP),Y
   LDY   #$61
   CMP   (FP),Y
   BNE   .cmp_true@12
   LDY   #$5A
   LDA   (FP),Y
   LDY   #$60
   CMP   (FP),Y
   BNE   .cmp_true@12
   LDA   #$00
   JMP   .cmp_end@13
.cmp_true@12:
   LDA   #$01
.cmp_end@13:
   LDY   #$64
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@2
   JMP   .if_else@1
.lb_skip@2:
   LDA   #<gs5
   LDY   #$66
   STA   (FP),Y
   LDA   #>gs5
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$05
   LDY   #$68
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$69
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$6A
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$6B
   STA   (FP),Y
   LDY   #$02
   LDA   (DPTR),Y
   LDY   #$6C
   STA   (FP),Y
   LDY   #$03
   LDA   (DPTR),Y
   LDY   #$6D
   STA   (FP),Y
   LDY   #$6A
   LDA   (FP),Y
   LDY   #$6E
   STA   (FP),Y
   LDY   #$6B
   LDA   (FP),Y
   LDY   #$6F
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
   ADC   #$B1
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$AC
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
.if_else@1:
   LDA   #$00
   LDY   #$79
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
   STA   HARGS
   LDX   #$08
   LSR   HARGS
.mul_loop@16:
   BCC   .mul_skip@17
   CLC
   LDY   #$79
   ADC   (FP),Y
.mul_skip@17:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@16
   LDY   #$80
   STA   (FP),Y
   LDA   HARGS
   DEY
   STA   (FP),Y
   LDY   #$81
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$82
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
   LDY   #$81
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$82
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$83
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$84
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   JSR   mix
   LDA   #$02
   LDY   #$85
   STA   (FP),Y
   INY
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@18
   LDA   #$00
   JMP   .sx_done@19
.sx_neg@18:
   LDA   #$FF
.sx_done@19:
   LDY   #$87
   STA   (FP),Y
   LDA   #$07
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$86
   LDA   (FP),Y
   LDY   #$88
   AND   (FP),Y
   LDY   #$8A
   STA   (FP),Y
   LDY   #$87
   LDA   (FP),Y
   LDY   #$89
   AND   (FP),Y
   LDY   #$8B
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CMP   #$01
   BEQ   .cmp_true@20
   LDA   #$00
   JMP   .cmp_end@21
.cmp_true@20:
   LDA   #$01
.cmp_end@21:
   LDY   #$8C
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .case@7
   LDY   #$8A
   LDA   (FP),Y
   CMP   #$03
   BEQ   .cmp_true@22
   LDA   #$00
   JMP   .cmp_end@23
.cmp_true@22:
   LDA   #$01
.cmp_end@23:
   LDY   #$8E
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@1
   JMP   .switch@0_break
.lb_skip@1:
   LDY   #$8A
   LDA   (FP),Y
   CMP   #$07
   BEQ   .cmp_true@24
   LDA   #$00
   JMP   .cmp_end@25
.cmp_true@24:
   LDA   #$01
.cmp_end@25:
   LDY   #$90
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@0
   JMP   .switch@0_break
.lb_skip@0:
   JMP   .switch@0_break
.case@7:
   LDA   #<gs5
   LDY   #$92
   STA   (FP),Y
   LDA   #>gs5
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$01
   LDY   #$94
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$95
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$96
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$97
   STA   (FP),Y
   LDY   #$02
   LDA   (DPTR),Y
   LDY   #$98
   STA   (FP),Y
   LDY   #$03
   LDA   (DPTR),Y
   LDY   #$99
   STA   (FP),Y
   LDA   #$81
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   ORA   #$00
   BPL   .sx_done@27
   LDA   #$FF
.sx_done@27:
   LDY   #$9C
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$96
   LDA   (FP),Y
   SEC
   LDY   #$9A
   SBC   (FP),Y
   LDY   #$9E
   STA   (FP),Y
   LDY   #$97
   LDA   (FP),Y
   LDY   #$9B
   SBC   (FP),Y
   LDY   #$9F
   STA   (FP),Y
   LDY   #$98
   LDA   (FP),Y
   LDY   #$9C
   SBC   (FP),Y
   LDY   #$A0
   STA   (FP),Y
   LDY   #$99
   LDA   (FP),Y
   LDY   #$9D
   SBC   (FP),Y
   LDY   #$A1
   STA   (FP),Y
   LDY   #$94
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$9E
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$9F
   LDA   (FP),Y
   LDY   #$01
   STA   (DPTR),Y
   LDY   #$A0
   LDA   (FP),Y
   LDY   #$02
   STA   (DPTR),Y
   LDY   #$A1
   LDA   (FP),Y
   LDY   #$03
   STA   (DPTR),Y
   SEC
   LDA   SSP
   SBC   #$04
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDY   #$9E
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$9F
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$A0
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$A1
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   JSR   mix
.case@10:
.default@11:
.switch@0_break:
   LDA   #$00
   LDY   #$A2
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   SEC
   LDY   #$A2
   SBC   (FP),Y
   LDY   #$A6
   STA   (FP),Y
   LDA   #$00
   LDY   #$A3
   SBC   (FP),Y
   LDY   #$A7
   STA   (FP),Y
   LDA   #$00
   LDY   #$A4
   SBC   (FP),Y
   LDY   #$A8
   STA   (FP),Y
   LDA   #$00
   LDY   #$A5
   SBC   (FP),Y
   LDY   #$A9
   STA   (FP),Y
   LDY   #$A6
   LDA   (FP),Y
   LDY   #$AA
   STA   (FP),Y
   LDY   #$A7
   LDA   (FP),Y
   LDY   #$AB
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
   ADC   #$B1
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$AC
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

f14:
   SUBROUTINE

   ; prologue: 6 arg bytes, 89 local bytes
   SEC
   LDA   SSP
   SBC   #$5B
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$5A
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   LDA   #$5D
   LDY   #$01
   STA   (FP),Y
   LDA   #$5B
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
   DEY
   LDA   (FP),Y
   SEC
   SBC   #$81
   LDY   #$05
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   SBC   #$00
   LDY   #$06
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$03
   STA   (FP),Y
   LDY   #$06
   LDA   (FP),Y
   LDY   #$04
   STA   (FP),Y
   LDA   g1
   LDY   #$07
   STA   (FP),Y
   LDA   g1+1
   INY
   STA   (FP),Y
   LDA   g1+1
   BMI   .sx_neg@28
   LDA   #$00
   JMP   .sx_done@29
.sx_neg@28:
   LDA   #$FF
.sx_done@29:
   LDY   #$09
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$5D
   LDA   (FP),Y
   SEC
   LDY   #$07
   SBC   (FP),Y
   LDY   #$0B
   STA   (FP),Y
   LDY   #$5E
   LDA   (FP),Y
   LDY   #$08
   SBC   (FP),Y
   LDY   #$0C
   STA   (FP),Y
   LDY   #$5F
   LDA   (FP),Y
   LDY   #$09
   SBC   (FP),Y
   LDY   #$0D
   STA   (FP),Y
   LDY   #$60
   LDA   (FP),Y
   LDY   #$0A
   SBC   (FP),Y
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
   DEY
   LDA   (FP),Y
   LDY   #$03
   STA   (FP),Y
   LDY   #$10
   LDA   (FP),Y
   LDY   #$04
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$01
   LDY   #$11
   STA   (FP),Y
   LDY   #$04
   LDA   (FP),Y
   ADC   #$00
   LDY   #$12
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$03
   STA   (FP),Y
   LDY   #$12
   LDA   (FP),Y
   LDY   #$04
   STA   (FP),Y
   LDY   #$11
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
   SEC
   LDA   SSP
   SBC   #$04
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDY   #$13
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$14
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$15
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$16
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   JSR   mix
   LDY   #$61
   LDA   (FP),Y
   LDY   #$17
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@30
   LDA   #$00
   JMP   .sx_done@31
.sx_neg@30:
   LDA   #$FF
.sx_done@31:
   LDY   #$18
   STA   (FP),Y
   LDA   #$07
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$17
   LDA   (FP),Y
   LDY   #$19
   AND   (FP),Y
   LDY   #$1B
   STA   (FP),Y
   LDY   #$18
   LDA   (FP),Y
   LDY   #$1A
   AND   (FP),Y
   LDY   #$1C
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CMP   #$03
   BEQ   .cmp_true@32
   LDA   #$00
   JMP   .cmp_end@33
.cmp_true@32:
   LDA   #$01
.cmp_end@33:
   LDY   #$1D
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .case@13
   LDY   #$1B
   LDA   (FP),Y
   CMP   #$04
   BEQ   .cmp_true@34
   LDA   #$00
   JMP   .cmp_end@35
.cmp_true@34:
   LDA   #$01
.cmp_end@35:
   LDY   #$1F
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .case@14
   LDY   #$1B
   LDA   (FP),Y
   CMP   #$07
   BEQ   .cmp_true@36
   LDA   #$00
   JMP   .cmp_end@37
.cmp_true@36:
   LDA   #$01
.cmp_end@37:
   LDY   #$21
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@9
   JMP   .case@20
.lb_skip@9:
   JMP   .default@21
.case@13:
.case@14:
   LDA   #$07
   LDY   #$23
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDA   g1
   DEY
   AND   (FP),Y
   LDY   #$25
   STA   (FP),Y
   LDA   g1+1
   DEY
   AND   (FP),Y
   LDY   #$26
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CMP   #$03
   BEQ   .cmp_true@38
   LDA   #$00
   JMP   .cmp_end@39
.cmp_true@38:
   LDA   #$01
.cmp_end@39:
   LDY   #$27
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@8
   JMP   .switch@12_break
.lb_skip@8:
   LDY   #$25
   LDA   (FP),Y
   CMP   #$07
   BEQ   .cmp_true@40
   LDA   #$00
   JMP   .cmp_end@41
.cmp_true@40:
   LDA   #$01
.cmp_end@41:
   LDY   #$29
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@7
   JMP   .switch@12_break
.lb_skip@7:
   LDA   #$00
   LDY   #$2B
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   CLC
   ADC   #$01
   INY
   STA   (FP),Y
   LDY   #$2C
   STA   (FP),Y
   INY
   LDA   (FP),Y
   LDY   #$2F
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
   BVC   .cmp_novf@42
   EOR   #$80
.cmp_novf@42:
   ASL
   LDA   #$00
   ROL
   LDY   #$31
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@6
   JMP   .switch@12_break
.lb_skip@6:
   LDA   @11.s19
   LDY   #$33
   STA   (FP),Y
   LDA   @11.s19+1
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
   LDY   #$33
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$34
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$35
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$36
   LDA   (FP),Y
.xjump@0:
   LDY   #$04
   STA   (SSP),Y
   JSR   mix
   JMP   .switch@12_break
.case@20:
   LDA   #$5A
   LDY   #$37
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   ORA   #$00
   BPL   .sx_done@44
   LDA   #$FF
.sx_done@44:
   LDY   #$39
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$5D
   LDA   (FP),Y
   SEC
   LDY   #$37
   SBC   (FP),Y
   LDY   #$3B
   STA   (FP),Y
   LDY   #$5E
   LDA   (FP),Y
   LDY   #$38
   SBC   (FP),Y
   LDY   #$3C
   STA   (FP),Y
   LDY   #$5F
   LDA   (FP),Y
   LDY   #$39
   SBC   (FP),Y
   LDY   #$3D
   STA   (FP),Y
   LDY   #$60
   LDA   (FP),Y
   LDY   #$3A
   SBC   (FP),Y
   LDY   #$3E
   STA   (FP),Y
   LDY   #$3B
   LDA   (FP),Y
   LDY   #$5D
   STA   (FP),Y
   LDY   #$3C
   LDA   (FP),Y
   LDY   #$5E
   STA   (FP),Y
   LDY   #$3D
   LDA   (FP),Y
   LDY   #$5F
   STA   (FP),Y
   LDY   #$3E
   LDA   (FP),Y
   LDY   #$60
   STA   (FP),Y
   LDA   ga4+4
   LDY   #$3F
   STA   (FP),Y
   LDA   ga4+5
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   AND   z3
   LDY   #$41
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   AND   z3+1
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
   LDY   #$43
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$44
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$45
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$46
   LDA   (FP),Y
   JMP   .xjump@0
.default@21:
   LDA   ga4+6
   LDY   #$47
   STA   (FP),Y
   LDA   ga4+7
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .cond_else@2
   LDY   #$5D
   LDA   (FP),Y
   LDY   #$49
   STA   (FP),Y
   LDY   #$5E
   LDA   (FP),Y
   LDY   #$4A
   STA   (FP),Y
   LDY   #$5F
   LDA   (FP),Y
   LDY   #$4B
   STA   (FP),Y
   LDY   #$60
   LDA   (FP),Y
   LDY   #$4C
   STA   (FP),Y
   JMP   .cond_end@3
.cond_else@2:
   LDA   #$00
   LDY   #$4D
   STA   (FP),Y
   INY
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@45
   LDA   #$00
   JMP   .sx_done@46
.sx_neg@45:
   LDA   #$FF
.sx_done@46:
   LDY   #$4F
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$50
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$51
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@47
   LDA   #$00
   JMP   .sx_done@48
.sx_neg@47:
   LDA   #$FF
.sx_done@48:
   LDY   #$52
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$50
   LDA   (FP),Y
   LDY   #$49
   STA   (FP),Y
   LDY   #$51
   LDA   (FP),Y
   LDY   #$4A
   STA   (FP),Y
   LDY   #$52
   LDA   (FP),Y
   LDY   #$4B
   STA   (FP),Y
   INY
   STA   (FP),Y
.cond_end@3:
   LDY   #$49
   LDA   (FP),Y
   LDY   #$54
   STA   (FP),Y
   LDY   #$4A
   LDA   (FP),Y
   LDY   #$55
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
   ADC   #$61
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$5A
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
.switch@12_break:
   LDA   g2
   LDY   #$56
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LSR
   LSR
   LDY   #$59
   STA   (FP),Y
   LDY   #$56
   LDA   (FP),Y
   ROR
   ROR
   ROR
   AND   #$C0
   LDY   #$58
   STA   (FP),Y
   INY
   LDA   (FP),Y
   TAX
   DEY
   LDA   (FP),Y

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$61
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$5A
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

   ; prologue: 0 arg bytes, 152 local bytes
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

   LDA   #$09
   LDY   #$01
   STA   (FP),Y
   LDA   #$00
   INY
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
   LDA   #$00
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
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
.loop@22_start:
   LDY   #$11
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
   BVC   .cmp_novf@49
   EOR   #$80
.cmp_novf@49:
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
   BNE   .lb_skip@10
   JMP   .loop@22_break
.lb_skip@10:
   LDY   #$05
   LDA   (FP),Y
   LDY   #$15
   STA   (FP),Y
   LDY   #$06
   LDA   (FP),Y
   LDY   #$16
   STA   (FP),Y
   LDY   #$07
   LDA   (FP),Y
   LDY   #$17
   STA   (FP),Y
   LDY   #$08
   LDA   (FP),Y
   LDY   #$18
   STA   (FP),Y
   LDA   g2
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$1B
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$1C
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@50
   LDA   #$00
   JMP   .sx_done@51
.sx_neg@50:
   LDA   #$FF
.sx_done@51:
   LDY   #$1D
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$15
   LDA   (FP),Y
   CLC
   LDY   #$1B
   ADC   (FP),Y
   LDY   #$1F
   STA   (FP),Y
   LDY   #$16
   LDA   (FP),Y
   LDY   #$1C
   ADC   (FP),Y
   LDY   #$20
   STA   (FP),Y
   LDY   #$17
   LDA   (FP),Y
   LDY   #$1D
   ADC   (FP),Y
   LDY   #$21
   STA   (FP),Y
   LDY   #$18
   LDA   (FP),Y
   LDY   #$1E
   ADC   (FP),Y
   LDY   #$22
   STA   (FP),Y
   LDY   #$1F
   LDA   (FP),Y
   STA   ga4+4
   INY
   LDA   (FP),Y
   STA   ga4+5
   LDY   #$10
   LDA   (FP),Y
   LDY   #$23
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
   LDY   #$23
   LDA   (FP),Y
   LDY   #$25
   AND   (FP),Y
   LDY   #$27
   STA   (FP),Y
   LDY   #$24
   LDA   (FP),Y
   LDY   #$26
   AND   (FP),Y
   LDY   #$28
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$29
   STA   (FP),Y
   STA   HARGS
   LDA   #$02
   STA   HARGS+1
   JSR   mul8
   LDA   HARGS+2
   LDY   #$2A
   STA   (FP),Y
   TAX
   LDA   ga4,X
   INY
   STA   (FP),Y
   LDA   ga4+1,X
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
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #<gs5
   INY
   STA   (FP),Y
   LDA   #>gs5
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$05
   LDY   #$33
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$34
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$35
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$36
   STA   (FP),Y
   LDY   #$02
   LDA   (DPTR),Y
   LDY   #$37
   STA   (FP),Y
   LDY   #$03
   LDA   (DPTR),Y
   LDY   #$38
   STA   (FP),Y
   LDY   #$35
   LDA   (FP),Y
   CLC
   LDY   #$2D
   SBC   (FP),Y
   LDY   #$36
   LDA   (FP),Y
   LDY   #$2E
   SBC   (FP),Y
   LDY   #$37
   LDA   (FP),Y
   LDY   #$2F
   SBC   (FP),Y
   LDY   #$38
   LDA   (FP),Y
   LDY   #$30
   SBC   (FP),Y
   LDA   #$00
   ROL
   LDY   #$39
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   g1
   INY
   LDA   (FP),Y
   STA   g1+1
   LDA   #$E4
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$3E
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$3F
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@52
   LDA   #$00
   JMP   .sx_done@53
.sx_neg@52:
   LDA   #$FF
.sx_done@53:
   LDY   #$40
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #<gs5
   INY
   STA   (FP),Y
   LDA   #>gs5
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$05
   LDY   #$44
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$45
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$46
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$47
   STA   (FP),Y
   LDY   #$02
   LDA   (DPTR),Y
   LDY   #$48
   STA   (FP),Y
   LDY   #$03
   LDA   (DPTR),Y
   LDY   #$49
   STA   (FP),Y
   LDA   #$5A
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   ORA   #$00
   BPL   .sx_done@55
   LDA   #$FF
.sx_done@55:
   LDY   #$4C
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$46
   LDA   (FP),Y
   LDY   #$4A
   AND   (FP),Y
   LDY   #$4E
   STA   (FP),Y
   LDY   #$47
   LDA   (FP),Y
   LDY   #$4B
   AND   (FP),Y
   LDY   #$4F
   STA   (FP),Y
   LDY   #$48
   LDA   (FP),Y
   LDY   #$4C
   AND   (FP),Y
   LDY   #$50
   STA   (FP),Y
   LDY   #$49
   LDA   (FP),Y
   LDY   #$4D
   AND   (FP),Y
   LDY   #$51
   STA   (FP),Y
   LDY   #$44
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$4E
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$4F
   LDA   (FP),Y
   LDY   #$01
   STA   (DPTR),Y
   LDY   #$50
   LDA   (FP),Y
   LDY   #$02
   STA   (DPTR),Y
   LDY   #$51
   LDA   (FP),Y
   LDY   #$03
   STA   (DPTR),Y
   LDY   #$3E
   LDA   (FP),Y
   CLC
   LDY   #$4E
   ADC   (FP),Y
   LDY   #$52
   STA   (FP),Y
   LDY   #$3F
   LDA   (FP),Y
   LDY   #$4F
   ADC   (FP),Y
   LDY   #$53
   STA   (FP),Y
   LDY   #$40
   LDA   (FP),Y
   LDY   #$50
   ADC   (FP),Y
   LDY   #$54
   STA   (FP),Y
   LDY   #$41
   LDA   (FP),Y
   LDY   #$51
   ADC   (FP),Y
   LDY   #$55
   STA   (FP),Y
   LDY   #$52
   LDA   (FP),Y
   LDY   #$56
   STA   (FP),Y
   STA   g2
   LDY   #$10
   LDA   (FP),Y
   LDY   #$57
   STA   (FP),Y
   CLC
   ADC   #$01
   INY
   STA   (FP),Y
   LDY   #$10
   STA   (FP),Y
   JMP   .loop@22_start
.loop@22_break:
   LDY   #$05
   LDA   (FP),Y
   LDY   #$59
   STA   (FP),Y
   LDY   #$06
   LDA   (FP),Y
   LDY   #$5A
   STA   (FP),Y
   LDY   #$07
   LDA   (FP),Y
   LDY   #$5B
   STA   (FP),Y
   LDY   #$08
   LDA   (FP),Y
   LDY   #$5C
   STA   (FP),Y
   LDY   #$59
   LDA   (FP),Y
   LDY   #$5D
   STA   (FP),Y
   LDY   #$5A
   LDA   (FP),Y
   LDY   #$5E
   STA   (FP),Y
   LDY   #$5B
   LDA   (FP),Y
   LDY   #$5F
   STA   (FP),Y
   LDY   #$5C
   LDA   (FP),Y
   LDY   #$60
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$04
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDY   #$5D
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$5E
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$5F
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$60
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   JSR   mix
   LDA   g2
   LDY   #$61
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   EOR   #$03
   LDY   #$63
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$64
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$65
   STA   (FP),Y
   STA   g2
   LDA   #<gs5
   INY
   STA   (FP),Y
   LDA   #>gs5
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$01
   LDY   #$68
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$69
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$6A
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$6B
   STA   (FP),Y
   LDY   #$02
   LDA   (DPTR),Y
   LDY   #$6C
   STA   (FP),Y
   LDY   #$03
   LDA   (DPTR),Y
   LDY   #$6D
   STA   (FP),Y
   LDA   #$05
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
   LDY   #$6E
   LDA   (FP),Y
   CLC
   LDY   #$70
   ADC   (FP),Y
   LDY   #$72
   STA   (FP),Y
   LDY   #$6F
   LDA   (FP),Y
   LDY   #$71
   ADC   (FP),Y
   LDY   #$73
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$74
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$75
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$6A
   LDA   (FP),Y
   SEC
   LDY   #$74
   SBC   (FP),Y
   LDY   #$6B
   LDA   (FP),Y
   LDY   #$75
   SBC   (FP),Y
   LDY   #$6C
   LDA   (FP),Y
   LDY   #$76
   SBC   (FP),Y
   LDY   #$6D
   LDA   (FP),Y
   LDY   #$77
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
   LDY   #$7A
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$7B
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@56
   LDA   #$00
   JMP   .sx_done@57
.sx_neg@56:
   LDA   #$FF
.sx_done@57:
   LDY   #$7C
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
   LDY   #$7A
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$7B
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$7C
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$7D
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   JSR   mix
   LDA   g1
   SEC
   SBC   #$81
   LDY   #$7E
   STA   (FP),Y
   LDA   g1+1
   SBC   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   g1
   INY
   LDA   (FP),Y
   STA   g1+1
   DEY
   LDA   (FP),Y
   LDY   #$80
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$81
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@58
   LDA   #$00
   JMP   .sx_done@59
.sx_neg@58:
   LDA   #$FF
.sx_done@59:
   LDY   #$82
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #<gs5
   INY
   STA   (FP),Y
   LDA   #>gs5
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$05
   LDY   #$86
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$87
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$80
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$81
   LDA   (FP),Y
   LDY   #$01
   STA   (DPTR),Y
   LDY   #$82
   LDA   (FP),Y
   LDY   #$02
   STA   (DPTR),Y
   LDY   #$83
   LDA   (FP),Y
   LDY   #$03
   STA   (DPTR),Y
   LDA   z3
   ORA   z3+1
   BEQ   .cond_else@4
   LDA   #$03
   LDY   #$88
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$8B
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$8C
   STA   (FP),Y
   JMP   .cond_end@5
.cond_else@4:
   LDA   #$FF
   LDY   #$8D
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$8B
   STA   (FP),Y
   INY
   STA   (FP),Y
.cond_end@5:
   LDA   g1
   CLC
   LDY   #$8B
   ADC   (FP),Y
   LDY   #$8F
   STA   (FP),Y
   LDA   g1+1
   LDY   #$8C
   ADC   (FP),Y
   LDY   #$90
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$91
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$92
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@60
   LDA   #$00
   JMP   .sx_done@61
.sx_neg@60:
   LDA   #$FF
.sx_done@61:
   LDY   #$93
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
   LDY   #$91
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$92
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$93
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$94
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   JSR   mix
   JSR   fin0
   JSR   fin1
   JSR   fin2
   LDA   acc
   LDY   #$95
   STA   (FP),Y
   LDA   acc+1
   AND   #$7F
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$97
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$98
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
   ADC   #$9A
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$99
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
   DC.W  $0100

g2:
   DC.B  $FF

ga4:
   DC.W  $0100
   DC.W  $0080
   DC.W  $0002
   DC.W  $007F

gs5:
   DS.B  1
   DC.L  $00008000
   DC.L  $00008000

@11.s19:
   DC.W  $0100

__zp_init:
   SUBROUTINE

   LDA   #$00
   STA   z3
   STA   z3+1
   RTS
