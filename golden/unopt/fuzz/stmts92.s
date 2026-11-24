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

   ; prologue: 0 arg bytes, 24 local bytes
   SEC
   LDA   SSP
   SBC   #$1A
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$19
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
   LDA   g2+1
   INY
   STA   (FP),Y
   LDA   g2+2
   INY
   STA   (FP),Y
   LDA   g2+3
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
   LDA   z3+2
   INY
   STA   (FP),Y
   LDA   z3+3
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
   LDA   ga4+2
   INY
   STA   (FP),Y
   LDA   ga4+3
   INY
   STA   (FP),Y
   LDA   ga4+4
   INY
   STA   (FP),Y
   LDA   ga4+5
   INY
   STA   (FP),Y
   LDA   ga4+6
   INY
   STA   (FP),Y
   LDA   ga4+7
   INY
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
   LDY   #$18
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$04
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDY   #$15
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$16
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$17
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$18
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   JSR   mix

   ; epilogue
   CLC
   LDA   FP
   ADC   #$1A
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$19
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

   ; prologue: 0 arg bytes, 46 local bytes
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

   LDA   ga4+8
   LDY   #$01
   STA   (FP),Y
   LDA   ga4+9
   INY
   STA   (FP),Y
   LDA   ga4+10
   INY
   STA   (FP),Y
   LDA   ga4+11
   INY
   STA   (FP),Y
   LDA   ga4+12
   INY
   STA   (FP),Y
   LDA   ga4+13
   INY
   STA   (FP),Y
   LDA   ga4+14
   INY
   STA   (FP),Y
   LDA   ga4+15
   INY
   STA   (FP),Y
   LDY   #$01
   LDA   (FP),Y
   LDY   #$09
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   LDY   #$0A
   STA   (FP),Y
   LDY   #$03
   LDA   (FP),Y
   LDY   #$0B
   STA   (FP),Y
   LDY   #$04
   LDA   (FP),Y
   LDY   #$0C
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
   LDA   ga4+16
   LDY   #$0D
   STA   (FP),Y
   LDA   ga4+17
   INY
   STA   (FP),Y
   LDA   ga4+18
   INY
   STA   (FP),Y
   LDA   ga4+19
   INY
   STA   (FP),Y
   LDA   ga4+20
   INY
   STA   (FP),Y
   LDA   ga4+21
   INY
   STA   (FP),Y
   LDA   ga4+22
   INY
   STA   (FP),Y
   LDA   ga4+23
   INY
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
   LDY   #$18
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$04
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDY   #$15
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$16
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$17
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$18
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   JSR   mix
   LDA   ga4+24
   LDY   #$19
   STA   (FP),Y
   LDA   ga4+25
   INY
   STA   (FP),Y
   LDA   ga4+26
   INY
   STA   (FP),Y
   LDA   ga4+27
   INY
   STA   (FP),Y
   LDA   ga4+28
   INY
   STA   (FP),Y
   LDA   ga4+29
   INY
   STA   (FP),Y
   LDA   ga4+30
   INY
   STA   (FP),Y
   LDA   ga4+31
   INY
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
   SEC
   LDA   SSP
   SBC   #$04
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDY   #$21
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$22
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$23
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$24
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   JSR   mix
   LDA   #<gs5
   LDY   #$25
   STA   (FP),Y
   LDA   #>gs5
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
   DEY
   LDA   (FP),Y
   LDY   #$2B
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$2C
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
   LDY   #$2B
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$2C
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$2D
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$2E
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   JSR   mix

   ; epilogue
   CLC
   LDA   FP
   ADC   #$30
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$2F
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

   LDA   #<gs5
   LDY   #$01
   STA   (FP),Y
   LDA   #>gs5
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$03
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
   LDY   #$05
   LDA   (FP),Y
   LDY   #$09
   STA   (FP),Y
   LDY   #$06
   LDA   (FP),Y
   LDY   #$0A
   STA   (FP),Y
   LDY   #$07
   LDA   (FP),Y
   LDY   #$0B
   STA   (FP),Y
   LDY   #$08
   LDA   (FP),Y
   LDY   #$0C
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

f6:
   SUBROUTINE

   ; prologue: 2 arg bytes, 215 local bytes
   SEC
   LDA   SSP
   SBC   #$D9
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$D8
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
   LDA   #$01
   INY
   STA   (FP),Y
   LDA   #$00
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
   LDA   #$03
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   ORA   #$00
   BPL   .sx_done@1
   LDA   #$FF
.sx_done@1:
   LDY   #$0B
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
   LDA   g2
   CLC
   LDY   #$09
   ADC   (FP),Y
   LDY   #$11
   STA   (FP),Y
   LDA   g2+1
   LDY   #$0A
   ADC   (FP),Y
   LDY   #$12
   STA   (FP),Y
   LDA   g2+2
   LDY   #$0B
   ADC   (FP),Y
   LDY   #$13
   STA   (FP),Y
   LDA   g2+3
   LDY   #$0C
   ADC   (FP),Y
   LDY   #$14
   STA   (FP),Y
   LDA   g2+4
   LDY   #$0D
   ADC   (FP),Y
   LDY   #$15
   STA   (FP),Y
   LDA   g2+5
   LDY   #$0E
   ADC   (FP),Y
   LDY   #$16
   STA   (FP),Y
   LDA   g2+6
   LDY   #$0F
   ADC   (FP),Y
   LDY   #$17
   STA   (FP),Y
   LDA   g2+7
   LDY   #$10
   ADC   (FP),Y
   LDY   #$18
   STA   (FP),Y
   LDY   #$11
   LDA   (FP),Y
   STA   g2
   INY
   LDA   (FP),Y
   STA   g2+1
   INY
   LDA   (FP),Y
   STA   g2+2
   INY
   LDA   (FP),Y
   STA   g2+3
   INY
   LDA   (FP),Y
   STA   g2+4
   INY
   LDA   (FP),Y
   STA   g2+5
   INY
   LDA   (FP),Y
   STA   g2+6
   INY
   LDA   (FP),Y
   STA   g2+7
   LDY   #$DA
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$19
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$1A
   STA   (FP),Y
   LDY   #$02
   LDA   (DPTR),Y
   LDY   #$1B
   STA   (FP),Y
   LDY   #$03
   LDA   (DPTR),Y
   LDY   #$1C
   STA   (FP),Y
   LDY   #$04
   LDA   (DPTR),Y
   LDY   #$1D
   STA   (FP),Y
   LDY   #$19
   LDA   (FP),Y
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
   ORA   #$00
   BMI   .sx_neg@2
   LDA   #$00
   JMP   .sx_done@3
.sx_neg@2:
   LDA   #$FF
.sx_done@3:
   LDY   #$23
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$11
   LDA   (FP),Y
   LDY   #$1E
   AND   (FP),Y
   LDY   #$26
   STA   (FP),Y
   LDY   #$12
   LDA   (FP),Y
   LDY   #$1F
   AND   (FP),Y
   LDY   #$27
   STA   (FP),Y
   LDY   #$13
   LDA   (FP),Y
   LDY   #$20
   AND   (FP),Y
   LDY   #$28
   STA   (FP),Y
   LDY   #$14
   LDA   (FP),Y
   LDY   #$21
   AND   (FP),Y
   LDY   #$29
   STA   (FP),Y
   LDY   #$15
   LDA   (FP),Y
   LDY   #$22
   AND   (FP),Y
   LDY   #$2A
   STA   (FP),Y
   LDY   #$16
   LDA   (FP),Y
   LDY   #$23
   AND   (FP),Y
   LDY   #$2B
   STA   (FP),Y
   LDY   #$17
   LDA   (FP),Y
   LDY   #$24
   AND   (FP),Y
   LDY   #$2C
   STA   (FP),Y
   LDY   #$18
   LDA   (FP),Y
   LDY   #$25
   AND   (FP),Y
   LDY   #$2D
   STA   (FP),Y
   LDY   #$26
   LDA   (FP),Y
   LDY   #$2E
   STA   (FP),Y
   LDY   #$27
   LDA   (FP),Y
   LDY   #$2F
   STA   (FP),Y
   LDY   #$28
   LDA   (FP),Y
   LDY   #$30
   STA   (FP),Y
   LDY   #$29
   LDA   (FP),Y
   LDY   #$31
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
   LDA   #<gs5
   LDY   #$32
   STA   (FP),Y
   LDA   #>gs5
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$03
   LDY   #$34
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$35
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$36
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$37
   STA   (FP),Y
   LDY   #$02
   LDA   (DPTR),Y
   LDY   #$38
   STA   (FP),Y
   LDY   #$03
   LDA   (DPTR),Y
   LDY   #$39
   STA   (FP),Y
   LDY   #$36
   LDA   (FP),Y
   LDY   #$3A
   STA   (FP),Y
   LDY   #$37
   LDA   (FP),Y
   LDY   #$3B
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$3C
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$3D
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
   LDY   #$3C
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$3D
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$3E
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$3F
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   JSR   mix
   LDA   #$00
   LDY   #$40
   STA   (FP),Y
   INY
   STA   (FP),Y
.loop@0_start:
   LDY   #$42
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
   BVC   .cmp_novf@4
   EOR   #$80
.cmp_novf@4:
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
   BNE   .lb_skip@2
   JMP   .loop@0_break
.lb_skip@2:
   LDY   #$DA
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
   LDY   #$04
   LDA   (DPTR),Y
   LDY   #$4A
   STA   (FP),Y
   LDY   #$46
   LDA   (FP),Y
   ORA   #$5A
   LDY   #$4B
   STA   (FP),Y
   LDY   #$47
   LDA   (FP),Y
   LDY   #$4C
   STA   (FP),Y
   LDY   #$48
   LDA   (FP),Y
   LDY   #$4D
   STA   (FP),Y
   LDY   #$49
   LDA   (FP),Y
   LDY   #$4E
   STA   (FP),Y
   LDY   #$4A
   LDA   (FP),Y
   LDY   #$4F
   STA   (FP),Y
   LDY   #$DA
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
   LDY   #$4D
   LDA   (FP),Y
   LDY   #$02
   STA   (DPTR),Y
   LDY   #$4E
   LDA   (FP),Y
   LDY   #$03
   STA   (DPTR),Y
   LDY   #$4F
   LDA   (FP),Y
   LDY   #$04
   STA   (DPTR),Y
   LDA   ga4+16
   LDY   #$50
   STA   (FP),Y
   LDA   ga4+17
   INY
   STA   (FP),Y
   LDA   ga4+18
   INY
   STA   (FP),Y
   LDA   ga4+19
   INY
   STA   (FP),Y
   LDA   ga4+20
   INY
   STA   (FP),Y
   LDA   ga4+21
   INY
   STA   (FP),Y
   LDA   ga4+22
   INY
   STA   (FP),Y
   LDA   ga4+23
   INY
   STA   (FP),Y
   LDY   #$50
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
   INY
   ORA   (FP),Y
   BEQ   .and_false@0
   LDY   #$41
   LDA   (FP),Y
   BEQ   .and_false@0
   LDA   #$01
   LDY   #$58
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   JMP   .and_end@1
.and_false@0:
   LDA   #$00
   LDY   #$58
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@1:
   LDA   #$07
   LDY   #$5A
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$58
   LDA   (FP),Y
   LDY   #$5A
   AND   (FP),Y
   LDY   #$5C
   STA   (FP),Y
   LDY   #$59
   LDA   (FP),Y
   LDY   #$5B
   AND   (FP),Y
   LDY   #$5D
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CMP   #$07
   BEQ   .cmp_true@5
   LDA   #$00
   JMP   .cmp_end@6
.cmp_true@5:
   LDA   #$01
.cmp_end@6:
   LDY   #$5E
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
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
   LDY   #$61
   STA   (FP),Y
   INY
   LDA   (FP),Y
   LDY   #$64
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
   BVC   .cmp_novf@7
   EOR   #$80
.cmp_novf@7:
   ASL
   LDA   #$00
   ROL
   LDY   #$66
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@1
   JMP   .loop@3_break
.lb_skip@1:
   LDY   #$41
   LDA   (FP),Y
   LDY   #$68
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$6A
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$6B
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@8
   LDA   #$00
   JMP   .sx_done@9
.sx_neg@8:
   LDA   #$FF
.sx_done@9:
   LDY   #$6C
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
   LDY   #$6A
   LDA   (FP),Y
   AND   z3
   LDY   #$72
   STA   (FP),Y
   LDY   #$6B
   LDA   (FP),Y
   AND   z3+1
   LDY   #$73
   STA   (FP),Y
   LDY   #$6C
   LDA   (FP),Y
   AND   z3+2
   LDY   #$74
   STA   (FP),Y
   LDY   #$6D
   LDA   (FP),Y
   AND   z3+3
   LDY   #$75
   STA   (FP),Y
   LDY   #$6E
   LDA   (FP),Y
   AND   z3+4
   LDY   #$76
   STA   (FP),Y
   LDY   #$6F
   LDA   (FP),Y
   AND   z3+5
   LDY   #$77
   STA   (FP),Y
   LDY   #$70
   LDA   (FP),Y
   AND   z3+6
   LDY   #$78
   STA   (FP),Y
   LDY   #$71
   LDA   (FP),Y
   AND   z3+7
   LDY   #$79
   STA   (FP),Y
   LDY   #$72
   LDA   (FP),Y
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
   LDA   g2
   STA   HARGS
   LDA   g2+1
   STA   HARGS+1
   LDA   g2+2
   STA   HARGS+2
   LDA   g2+3
   STA   HARGS+3
   LDA   g2+4
   STA   HARGS+4
   LDA   g2+5
   STA   HARGS+5
   LDA   g2+6
   STA   HARGS+6
   LDA   g2+7
   STA   HARGS+7
   LDA   #$06
   STA   HARGS+8
   JSR   lsr64
   LDA   HARGS+9
   LDY   #$7E
   STA   (FP),Y
   LDA   HARGS+10
   INY
   STA   (FP),Y
   LDA   HARGS+11
   INY
   STA   (FP),Y
   LDA   HARGS+12
   INY
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
   LDA   HARGS+16
   INY
   STA   (FP),Y
   LDY   #$7E
   LDA   (FP),Y
   STA   g2
   INY
   LDA   (FP),Y
   STA   g2+1
   INY
   LDA   (FP),Y
   STA   g2+2
   INY
   LDA   (FP),Y
   STA   g2+3
   INY
   LDA   (FP),Y
   STA   g2+4
   INY
   LDA   (FP),Y
   STA   g2+5
   INY
   LDA   (FP),Y
   STA   g2+6
   INY
   LDA   (FP),Y
   STA   g2+7
   LDY   #$7E
   LDA   (FP),Y
   LDY   #$86
   STA   (FP),Y

   ; epilogue
   PHA
   CLC
   LDA   FP
   ADC   #$DB
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$D8
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   PLA
   RTS
.loop@3_break:
   LDA   #$06
   LDY   #$A2
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$A4
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$A5
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@16
   LDA   #$00
   JMP   .sx_done@17
.sx_neg@16:
   LDA   #$FF
.sx_done@17:
   LDY   #$A6
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   STA   HARGS
   LDY   #$80
   STY   HARGS+1
   LDY   #$00
   STY   HARGS+2
   STY   HARGS+3
   STA   HARGS+4
   ASL
   TYA
   ADC   #$FF
   EOR   #$FF
   STA   HARGS+5
   STA   HARGS+6
   STA   HARGS+7
   LDY   #$A4
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
   LDY   #$A9
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
   LDY   #$A9
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
   JMP   .if_end@4
.lb_skip@0:
   LDA   #<gs5
   LDY   #$AE
   STA   (FP),Y
   LDA   #>gs5
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$03
   LDY   #$B0
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$B1
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$B2
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$B3
   STA   (FP),Y
   LDY   #$02
   LDA   (DPTR),Y
   LDY   #$B4
   STA   (FP),Y
   LDY   #$03
   LDA   (DPTR),Y
   LDY   #$B5
   STA   (FP),Y
   LDY   #$B2
   LDA   (FP),Y
   CLC
   ADC   #$01
   LDY   #$B6
   STA   (FP),Y
   LDY   #$B3
   LDA   (FP),Y
   ADC   #$00
   LDY   #$B7
   STA   (FP),Y
   LDY   #$B4
   LDA   (FP),Y
   ADC   #$00
   LDY   #$B8
   STA   (FP),Y
   LDY   #$B5
   LDA   (FP),Y
   ADC   #$00
   LDY   #$B9
   STA   (FP),Y
   LDY   #$B0
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$B6
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$B7
   LDA   (FP),Y
   LDY   #$01
   STA   (DPTR),Y
   LDY   #$B8
   LDA   (FP),Y
   LDY   #$02
   STA   (DPTR),Y
   LDY   #$B9
   LDA   (FP),Y
   LDY   #$03
   STA   (DPTR),Y
   LDY   #$B6
   LDA   (FP),Y
   LDY   #$BA
   STA   (FP),Y
   LDY   #$B7
   LDA   (FP),Y
   LDY   #$BB
   STA   (FP),Y
   LDY   #$B8
   LDA   (FP),Y
   LDY   #$BC
   STA   (FP),Y
   LDY   #$B9
   LDA   (FP),Y
   LDY   #$BD
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$04
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDY   #$BA
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$BB
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$BC
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$BD
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   JSR   mix
.if_end@4:
   LDY   #$41
   LDA   (FP),Y
   LDY   #$BE
   STA   (FP),Y
   CLC
   ADC   #$01
   INY
   STA   (FP),Y
   LDY   #$41
   STA   (FP),Y
   JMP   .loop@0_start
.loop@0_break:
   LDA   #<gs5
   LDY   #$C0
   STA   (FP),Y
   LDA   #>gs5
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$03
   LDY   #$C2
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$C3
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$C4
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$C5
   STA   (FP),Y
   LDY   #$02
   LDA   (DPTR),Y
   LDY   #$C6
   STA   (FP),Y
   LDY   #$03
   LDA   (DPTR),Y
   LDY   #$C7
   STA   (FP),Y
   LDY   #$C4
   LDA   (FP),Y
   LDY   #$C8
   STA   (FP),Y
   LDY   #$C5
   LDA   (FP),Y
   LDY   #$C9
   STA   (FP),Y
   LDY   #$C6
   LDA   (FP),Y
   LDY   #$CA
   STA   (FP),Y
   LDY   #$C7
   LDA   (FP),Y
   LDY   #$CB
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@18
   LDA   #$00
   JMP   .sx_done@19
.sx_neg@18:
   LDA   #$FF
.sx_done@19:
   LDY   #$CC
   STA   (FP),Y
   LDY   #$DA
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$CD
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$CE
   STA   (FP),Y
   LDY   #$02
   LDA   (DPTR),Y
   LDY   #$CF
   STA   (FP),Y
   LDY   #$03
   LDA   (DPTR),Y
   LDY   #$D0
   STA   (FP),Y
   LDY   #$04
   LDA   (DPTR),Y
   LDY   #$D1
   STA   (FP),Y
   LDY   #$C8
   LDA   (FP),Y
   LDY   #$CD
   EOR   (FP),Y
   LDY   #$D2
   STA   (FP),Y
   LDY   #$C9
   LDA   (FP),Y
   LDY   #$CE
   EOR   (FP),Y
   LDY   #$D3
   STA   (FP),Y
   LDY   #$CA
   LDA   (FP),Y
   LDY   #$CF
   EOR   (FP),Y
   LDY   #$D4
   STA   (FP),Y
   LDY   #$CB
   LDA   (FP),Y
   LDY   #$D0
   EOR   (FP),Y
   LDY   #$D5
   STA   (FP),Y
   LDY   #$CC
   LDA   (FP),Y
   LDY   #$D1
   EOR   (FP),Y
   LDY   #$D6
   STA   (FP),Y
   LDY   #$D2
   LDA   (FP),Y
   LDY   #$D7
   STA   (FP),Y

   ; epilogue
   PHA
   CLC
   LDA   FP
   ADC   #$DB
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

f11:
   SUBROUTINE

   ; prologue: 5 arg bytes, 99 local bytes
   SEC
   LDA   SSP
   SBC   #$65
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$64
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   LDA   #$01
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
   LDA   #$04
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$03
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
   LDY   #$05
   LDA   (FP),Y
   LDY   #$0E
   STA   (FP),Y
   LDY   #$06
   LDA   (FP),Y
   LDY   #$0F
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$05
   LDY   #$10
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$11
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$05
   STA   (FP),Y
   LDY   #$11
   LDA   (FP),Y
   LDY   #$06
   STA   (FP),Y
   LDA   #$07
   LDY   #$12
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$10
   LDA   (FP),Y
   LDY   #$12
   AND   (FP),Y
   LDY   #$14
   STA   (FP),Y
   LDY   #$11
   LDA   (FP),Y
   LDY   #$13
   AND   (FP),Y
   LDY   #$15
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CMP   #$02
   BEQ   .cmp_true@20
   LDA   #$00
   JMP   .cmp_end@21
.cmp_true@20:
   LDA   #$01
.cmp_end@21:
   LDY   #$16
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@3
   JMP   .switch@4_break
.lb_skip@3:
   LDY   #$08
   LDA   (FP),Y
   LDY   #$18
   STA   (FP),Y
   INY
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@22
   LDA   #$00
   JMP   .sx_done@23
.sx_neg@22:
   LDA   #$FF
.sx_done@23:
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
   LDX   #$05
.shift_loop@24:
   LDY   #$1B
   LDA   (FP),Y
   ASL
   STA   (FP),Y
   INY
   LDA   (FP),Y
   ROL
   STA   (FP),Y
   DEX
   BNE   .shift_loop@24
   DEY
   LDA   (FP),Y
   LDY   #$1D
   STA   (FP),Y
   LDY   #$08
   STA   (FP),Y
   LDY   #$1E
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@26
   LDA   #$00
   JMP   .sx_done@27
.sx_neg@26:
   LDA   #$FF
.sx_done@27:
   LDY   #$1F
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$20
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$21
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$66
   ORA   (FP),Y
   LDY   #$22
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$67
   ORA   (FP),Y
   LDY   #$23
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$24
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$25
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
   LDY   #$24
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$25
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$26
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$27
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   JSR   mix
   LDY   #$02
   LDA   (FP),Y
   LDY   #$28
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@28
   LDA   #$00
   JMP   .sx_done@29
.sx_neg@28:
   LDA   #$FF
.sx_done@29:
   LDY   #$29
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LSR
   LSR
   LSR
   LSR
   LSR
   LSR
   LDY   #$2A
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ASL
   ASL
   INY
   ORA   (FP),Y
   STA   (FP),Y
   LDY   #$2C
   STA   (FP),Y
   LDY   #$02
   STA   (FP),Y
.switch@4_break:
   CLC
   LDA   FP
   ADC   #$09
   LDY   #$2D
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
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
   LDY   #$2E
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   JSR   f6
   LDY   #$2F
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$02
   STA   (FP),Y
   LDA   z3
   ORA   z3+1
   ORA   z3+2
   ORA   z3+3
   ORA   z3+4
   ORA   z3+5
   ORA   z3+6
   ORA   z3+7
   BEQ   .cond_else@5
   LDA   #$00
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
   ORA   #$00
   BMI   .sx_neg@30
   LDA   #$00
   JMP   .sx_done@31
.sx_neg@30:
   LDA   #$FF
.sx_done@31:
   LDY   #$36
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
   LDY   #$34
   LDA   (FP),Y
   LDY   #$3C
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$36
   LDA   (FP),Y
   LDY   #$3E
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
   JMP   .cond_end@6
.cond_else@5:
   LDA   z3
   LDY   #$3C
   STA   (FP),Y
   LDA   z3+1
   INY
   STA   (FP),Y
   LDA   z3+2
   INY
   STA   (FP),Y
   LDA   z3+3
   INY
   STA   (FP),Y
   LDA   z3+4
   INY
   STA   (FP),Y
   LDA   z3+5
   INY
   STA   (FP),Y
   LDA   z3+6
   INY
   STA   (FP),Y
   LDA   z3+7
   INY
   STA   (FP),Y
.cond_end@6:
   LDY   #$3C
   LDA   (FP),Y
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
   AND   #$03
   LDY   #$4B
   STA   (FP),Y
   LDY   #$44
   LDA   (FP),Y
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
   EOR   #$02
   LDY   #$53
   STA   (FP),Y
   LDY   #$4C
   LDA   (FP),Y
   STA   ga4+8
   INY
   LDA   (FP),Y
   STA   ga4+9
   INY
   LDA   (FP),Y
   STA   ga4+10
   INY
   LDA   (FP),Y
   STA   ga4+11
   INY
   LDA   (FP),Y
   STA   ga4+12
   INY
   LDA   (FP),Y
   STA   ga4+13
   INY
   LDA   (FP),Y
   STA   ga4+14
   INY
   LDA   (FP),Y
   SEC
   SBC   #$02
   STA   ga4+15
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
   LDY   #$56
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$57
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$58
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$59
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$5A
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$5B
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$5A
   EOR   (FP),Y
   LDY   #$5F
   STA   (FP),Y
   LDA   #$00
   LDY   #$5B
   EOR   (FP),Y
   LDY   #$60
   STA   (FP),Y
   LDA   #$01
   LDY   #$5C
   EOR   (FP),Y
   LDY   #$61
   STA   (FP),Y
   LDA   #$00
   LDY   #$5D
   EOR   (FP),Y
   LDY   #$62
   STA   (FP),Y
   LDA   #$00
   LDY   #$5E
   EOR   (FP),Y
   LDY   #$63
   STA   (FP),Y
   LDY   #$5F
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

   ; epilogue
   CLC
   LDA   FP
   ADC   #$6A
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

   ; prologue: 0 arg bytes, 208 local bytes
   SEC
   LDA   SSP
   SBC   #$D2
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$D1
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   LDA   #$F9
   LDY   #$01
   STA   (FP),Y
   LDA   #$CF
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
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
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
   LDY   #$0B
   LDA   (FP),Y
   LDY   #$10
   STA   (FP),Y
   LDY   #$0C
   LDA   (FP),Y
   LDY   #$11
   STA   (FP),Y
   LDY   #$0D
   LDA   (FP),Y
   LDY   #$12
   STA   (FP),Y
   LDY   #$0E
   LDA   (FP),Y
   LDY   #$13
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$0B
   LDA   (FP),Y
   CLC
   ADC   #$01
   LDY   #$15
   STA   (FP),Y
   LDY   #$0C
   LDA   (FP),Y
   ADC   #$00
   LDY   #$16
   STA   (FP),Y
   LDY   #$0D
   LDA   (FP),Y
   ADC   #$00
   LDY   #$17
   STA   (FP),Y
   LDY   #$0E
   LDA   (FP),Y
   ADC   #$00
   LDY   #$18
   STA   (FP),Y
   LDY   #$0F
   LDA   (FP),Y
   ADC   #$00
   LDY   #$19
   STA   (FP),Y
   LDY   #$15
   LDA   (FP),Y
   LDY   #$0B
   STA   (FP),Y
   LDY   #$16
   LDA   (FP),Y
   LDY   #$0C
   STA   (FP),Y
   LDY   #$17
   LDA   (FP),Y
   LDY   #$0D
   STA   (FP),Y
   LDY   #$18
   LDA   (FP),Y
   LDY   #$0E
   STA   (FP),Y
   LDY   #$19
   LDA   (FP),Y
   LDY   #$0F
   STA   (FP),Y
   INY
   LDA   (FP),Y
   LDY   #$1A
   STA   (FP),Y
   LDY   #$11
   LDA   (FP),Y
   LDY   #$1B
   STA   (FP),Y
   LDY   #$12
   LDA   (FP),Y
   LDY   #$1C
   STA   (FP),Y
   LDY   #$13
   LDA   (FP),Y
   LDY   #$1D
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
   LDY   #$1F
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   z3
   LDY   #$1A
   AND   (FP),Y
   LDY   #$22
   STA   (FP),Y
   LDA   z3+1
   LDY   #$1B
   AND   (FP),Y
   LDY   #$23
   STA   (FP),Y
   LDA   z3+2
   LDY   #$1C
   AND   (FP),Y
   LDY   #$24
   STA   (FP),Y
   LDA   z3+3
   LDY   #$1D
   AND   (FP),Y
   LDY   #$25
   STA   (FP),Y
   LDA   z3+4
   LDY   #$1E
   AND   (FP),Y
   LDY   #$26
   STA   (FP),Y
   LDA   z3+5
   LDY   #$1F
   AND   (FP),Y
   LDY   #$27
   STA   (FP),Y
   LDA   z3+6
   LDY   #$20
   AND   (FP),Y
   LDY   #$28
   STA   (FP),Y
   LDA   z3+7
   LDY   #$21
   AND   (FP),Y
   LDY   #$29
   STA   (FP),Y
   LDY   #$22
   LDA   (FP),Y
   LDY   #$2A
   STA   (FP),Y
   LDY   #$23
   LDA   (FP),Y
   LDY   #$2B
   STA   (FP),Y
   LDY   #$24
   LDA   (FP),Y
   LDY   #$2C
   STA   (FP),Y
   LDY   #$25
   LDA   (FP),Y
   LDY   #$2D
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$04
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
   JSR   mix
   LDA   #$00
   LDY   #$2E
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   ga4+24
   INY
   STA   (FP),Y
   LDA   ga4+25
   INY
   STA   (FP),Y
   LDA   ga4+26
   INY
   STA   (FP),Y
   LDA   ga4+27
   INY
   STA   (FP),Y
   LDA   ga4+28
   INY
   STA   (FP),Y
   LDA   ga4+29
   INY
   STA   (FP),Y
   LDA   ga4+30
   INY
   STA   (FP),Y
   LDA   ga4+31
   INY
   STA   (FP),Y
   LDY   #$31
   LDA   (FP),Y
   LDY   #$39
   STA   (FP),Y
   LDY   #$32
   LDA   (FP),Y
   LDY   #$3A
   STA   (FP),Y
   LDY   #$07
   LDA   (FP),Y
   LDY   #$3B
   STA   (FP),Y
   LDY   #$08
   LDA   (FP),Y
   LDY   #$3C
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$3D
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$05
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDY   #$2F
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$30
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
   LDY   #$3D
   LDA   (FP),Y
   LDY   #$05
   STA   (SSP),Y
   JSR   f11
   LDA   HARGS+16
   LDY   #$3E
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
   LDA   #$02
   INY
   STA   (FP),Y
   ORA   #$00
   BEQ   .lnot_true@34
   LDA   #$00
   JMP   .lnot_end@35
.lnot_true@34:
   LDA   #$01
.lnot_end@35:
   LDY   #$44
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$46
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$47
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
   ADC   #$01
   LDY   #$4A
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$4B
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$4C
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$4D
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDA   #$01
   INY
   STA   (FP),Y
   LDY   #$4C
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
   LDY   #$50
   STA   (FP),Y
   LDA   HARGS+5
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$52
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$53
   STA   (FP),Y
   LDA   ga4+8
   INY
   STA   (FP),Y
   LDA   ga4+9
   INY
   STA   (FP),Y
   LDA   ga4+10
   INY
   STA   (FP),Y
   LDA   ga4+11
   INY
   STA   (FP),Y
   LDA   ga4+12
   INY
   STA   (FP),Y
   LDA   ga4+13
   INY
   STA   (FP),Y
   LDA   ga4+14
   INY
   STA   (FP),Y
   LDA   ga4+15
   INY
   STA   (FP),Y
   LDA   #$80
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@36
   LDA   #$00
   JMP   .sx_done@37
.sx_neg@36:
   LDA   #$FF
.sx_done@37:
   LDY   #$5E
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$5F
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$60
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@38
   LDA   #$00
   JMP   .sx_done@39
.sx_neg@38:
   LDA   #$FF
.sx_done@39:
   LDY   #$61
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
   LDY   #$54
   LDA   (FP),Y
   CLC
   LDY   #$5F
   ADC   (FP),Y
   LDY   #$67
   STA   (FP),Y
   LDY   #$55
   LDA   (FP),Y
   LDY   #$60
   ADC   (FP),Y
   LDY   #$68
   STA   (FP),Y
   LDY   #$56
   LDA   (FP),Y
   LDY   #$61
   ADC   (FP),Y
   LDY   #$69
   STA   (FP),Y
   LDY   #$57
   LDA   (FP),Y
   LDY   #$62
   ADC   (FP),Y
   LDY   #$6A
   STA   (FP),Y
   LDY   #$58
   LDA   (FP),Y
   LDY   #$63
   ADC   (FP),Y
   LDY   #$6B
   STA   (FP),Y
   LDY   #$59
   LDA   (FP),Y
   LDY   #$64
   ADC   (FP),Y
   LDY   #$6C
   STA   (FP),Y
   LDY   #$5A
   LDA   (FP),Y
   LDY   #$65
   ADC   (FP),Y
   LDY   #$6D
   STA   (FP),Y
   LDY   #$5B
   LDA   (FP),Y
   LDY   #$66
   ADC   (FP),Y
   LDY   #$6E
   STA   (FP),Y
   LDY   #$67
   LDA   (FP),Y
   LDY   #$6F
   STA   (FP),Y
   LDY   #$68
   LDA   (FP),Y
   LDY   #$70
   STA   (FP),Y
   LDY   #$69
   LDA   (FP),Y
   LDY   #$71
   STA   (FP),Y
   LDY   #$6A
   LDA   (FP),Y
   LDY   #$72
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
   AND   #$03
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
   EOR   #$02
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
   SEC
   SBC   #$02
   LDY   #$86
   STA   (FP),Y
   LDY   #$7F
   LDA   (FP),Y
   LDY   #$87
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$05
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
   LDY   #$52
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$53
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   LDY   #$87
   LDA   (FP),Y
   LDY   #$05
   STA   (SSP),Y
   JSR   f11
   LDA   HARGS+16
   LDY   #$88
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
   LDY   #$88
   LDA   (FP),Y
   LDY   #$8D
   STA   (FP),Y
   LDY   #$0A
   STA   (FP),Y
   LDY   #$07
   LDA   (FP),Y
   LDY   #$8E
   STA   (FP),Y
   LDY   #$08
   LDA   (FP),Y
   LDY   #$8F
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$90
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$91
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
   LDY   #$90
   LDA   (FP),Y
   SEC
   SBC   z3
   LDY   #$98
   STA   (FP),Y
   LDY   #$91
   LDA   (FP),Y
   SBC   z3+1
   LDY   #$99
   STA   (FP),Y
   LDY   #$92
   LDA   (FP),Y
   SBC   z3+2
   LDY   #$9A
   STA   (FP),Y
   LDY   #$93
   LDA   (FP),Y
   SBC   z3+3
   LDY   #$9B
   STA   (FP),Y
   LDY   #$94
   LDA   (FP),Y
   SBC   z3+4
   LDY   #$9C
   STA   (FP),Y
   LDY   #$95
   LDA   (FP),Y
   SBC   z3+5
   LDY   #$9D
   STA   (FP),Y
   LDY   #$96
   LDA   (FP),Y
   SBC   z3+6
   LDY   #$9E
   STA   (FP),Y
   LDY   #$97
   LDA   (FP),Y
   SBC   z3+7
   LDY   #$9F
   STA   (FP),Y
   LDY   #$98
   LDA   (FP),Y
   LDY   #$A0
   STA   (FP),Y
   LDY   #$99
   LDA   (FP),Y
   LDY   #$A1
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
   LDY   #$A0
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$A1
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$A3
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$A4
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   LDY   #$A7
   LDA   (FP),Y
   LDY   #$05
   STA   (SSP),Y
   JSR   f11
   LDA   HARGS+16
   LDY   #$A8
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
   LDY   #$A8
   LDA   (FP),Y
   LDY   #$AD
   STA   (FP),Y
   LDY   #$A9
   LDA   (FP),Y
   LDY   #$AE
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$07
   STA   (FP),Y
   LDY   #$AE
   LDA   (FP),Y
   LDY   #$08
   STA   (FP),Y
   LDA   #$FF
   LDY   #$AF
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$03
   LDA   (FP),Y
   LDY   #$B1
   STA   (FP),Y
   LDY   #$04
   LDA   (FP),Y
   LDY   #$B2
   STA   (FP),Y
   LDY   #$AF
   LDA   (FP),Y
   SEC
   LDY   #$B1
   SBC   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$B2
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
   LDY   #$B5
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$B6
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@40
   LDA   #$00
   JMP   .sx_done@41
.sx_neg@40:
   LDA   #$FF
.sx_done@41:
   LDY   #$B7
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
   LDY   #$B5
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
   INY
   LDA   (FP),Y
   STA   z3+4
   INY
   LDA   (FP),Y
   STA   z3+5
   INY
   LDA   (FP),Y
   STA   z3+6
   INY
   LDA   (FP),Y
   STA   z3+7
   LDA   #$05
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   ORA   #$00
   BPL   .sx_done@43
   LDA   #$FF
.sx_done@43:
   LDY   #$BF
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
   LDA   z3
   STA   HARGS
   LDA   z3+1
   STA   HARGS+1
   LDA   z3+2
   STA   HARGS+2
   LDA   z3+3
   STA   HARGS+3
   LDA   z3+4
   STA   HARGS+4
   LDA   z3+5
   STA   HARGS+5
   LDA   z3+6
   STA   HARGS+6
   LDA   z3+7
   STA   HARGS+7
   LDY   #$BD
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
   INY
   LDA   (FP),Y
   STA   HARGS+15
   JSR   mul64
   LDA   HARGS+16
   LDY   #$C5
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
   LDA   HARGS+23
   INY
   STA   (FP),Y
   LDY   #$C5
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
   INY
   LDA   (FP),Y
   STA   z3+4
   INY
   LDA   (FP),Y
   STA   z3+5
   INY
   LDA   (FP),Y
   STA   z3+6
   INY
   LDA   (FP),Y
   STA   z3+7
   JSR   fin0
   JSR   fin1
   JSR   fin2
   LDA   acc
   LDY   #$CD
   STA   (FP),Y
   LDA   acc+1
   AND   #$7F
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$CF
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$D0
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
   ADC   #$D2
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$D1
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
   DC.B  $07

g2:
   DC.L  $0000007F
   DC.L  $00000000

ga4:
   DC.L  $00007FFF
   DC.L  $00000000
   DC.L  $7FFFFFFF
   DC.L  $00000000
   DC.L  $00008000
   DC.L  $00000000
   DC.L  $00008000
   DC.L  $00000000

gs5:
   DS.B  1
   DC.W  $0002
   DC.L  $00000002

@8.s19:
   DC.W  $0001

__zp_init:
   SUBROUTINE

   LDA   #$00
   STA   z3+2
   STA   z3+3
   STA   z3+4
   STA   z3+5
   STA   z3+6
   STA   z3+7
   LDA   #$FF
   STA   z3
   LDA   #$7F
   STA   z3+1
   RTS
