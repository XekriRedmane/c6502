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

   LDA   g1
   LDY   #$01
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
   LDA   g3
   LDY   #$09
   STA   (FP),Y
   LDA   g3+1
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
   LDA   z4
   LDY   #$0D
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

fin1:
   SUBROUTINE

   ; prologue: 0 arg bytes, 44 local bytes
   SEC
   LDA   SSP
   SBC   #$2E
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$2D
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
   LDA   ga5+4
   INY
   STA   (FP),Y
   LDA   ga5+5
   INY
   STA   (FP),Y
   LDA   ga5+6
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
   LDY   #$04
   LDA   (FP),Y
   LDY   #$0B
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$04
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDY   #$08
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$09
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$0A
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$0B
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   JSR   mix
   LDA   ga5+7
   LDY   #$0C
   STA   (FP),Y
   LDA   ga5+8
   INY
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
   LDA   ga5+12
   INY
   STA   (FP),Y
   LDA   ga5+13
   INY
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
   LDY   #$0F
   LDA   (FP),Y
   LDY   #$16
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
   LDA   ga5+14
   LDY   #$17
   STA   (FP),Y
   LDA   ga5+15
   INY
   STA   (FP),Y
   LDA   ga5+16
   INY
   STA   (FP),Y
   LDA   ga5+17
   INY
   STA   (FP),Y
   LDA   ga5+18
   INY
   STA   (FP),Y
   LDA   ga5+19
   INY
   STA   (FP),Y
   LDA   ga5+20
   INY
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
   SEC
   LDA   SSP
   SBC   #$04
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDY   #$1E
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$1F
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$20
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$21
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   JSR   mix
   LDA   ga5+21
   LDY   #$22
   STA   (FP),Y
   LDA   ga5+22
   INY
   STA   (FP),Y
   LDA   ga5+23
   INY
   STA   (FP),Y
   LDA   ga5+24
   INY
   STA   (FP),Y
   LDA   ga5+25
   INY
   STA   (FP),Y
   LDA   ga5+26
   INY
   STA   (FP),Y
   LDA   ga5+27
   INY
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
   LDY   #$2B
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$2C
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   JSR   mix

   ; epilogue
   CLC
   LDA   FP
   ADC   #$2E
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$2D
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

   ; prologue: 0 arg bytes, 23 local bytes
   SEC
   LDA   SSP
   SBC   #$19
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$18
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
   LDY   #$04
   LDA   (DPTR),Y
   LDY   #$09
   STA   (FP),Y
   LDY   #$05
   LDA   (DPTR),Y
   LDY   #$0A
   STA   (FP),Y
   LDY   #$06
   LDA   (DPTR),Y
   LDY   #$0B
   STA   (FP),Y
   LDY   #$05
   LDA   (FP),Y
   LDY   #$0C
   STA   (FP),Y
   LDY   #$06
   LDA   (FP),Y
   LDY   #$0D
   STA   (FP),Y
   LDY   #$07
   LDA   (FP),Y
   LDY   #$0E
   STA   (FP),Y
   LDY   #$08
   LDA   (FP),Y
   LDY   #$0F
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
   ADC   #$08
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
   LDY   #$02
   LDA   (DPTR),Y
   LDY   #$16
   STA   (FP),Y
   LDY   #$03
   LDA   (DPTR),Y
   LDY   #$17
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$04
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDY   #$14
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$15
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$16
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$17
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   JSR   mix

   ; epilogue
   CLC
   LDA   FP
   ADC   #$19
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$18
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

   ; prologue: 7 arg bytes, 200 local bytes
   SEC
   LDA   SSP
   SBC   #$CA
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$C9
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
   LDA   #$03
   INY
   STA   (FP),Y
   LDA   #$00
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
   LDY   #$CF
   LDA   (FP),Y
   LDY   #$11
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   AND   #$5A
   LDY   #$13
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDA   #$00
   LDY   #$14
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$15
   STA   (FP),Y
   LDY   #$CF
   STA   (FP),Y
   LDY   #$16
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$18
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$19
   STA   (FP),Y
   LDY   #$0F
   LDA   (FP),Y
   LDY   #$18
   AND   (FP),Y
   LDY   #$1A
   STA   (FP),Y
   LDY   #$10
   LDA   (FP),Y
   LDY   #$19
   AND   (FP),Y
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
   LDY   #$1C
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$1D
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$1E
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$1F
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   JSR   mix
   LDA   ga5
   LDY   #$20
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
   LDA   ga5+4
   INY
   STA   (FP),Y
   LDA   ga5+5
   INY
   STA   (FP),Y
   LDA   ga5+6
   INY
   STA   (FP),Y
   LDY   #$20
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
   BEQ   .cond_else@0
   LDY   #$09
   LDA   (FP),Y
   LDY   #$27
   STA   (FP),Y
   LDY   #$0A
   LDA   (FP),Y
   LDY   #$28
   STA   (FP),Y
   LDY   #$0B
   LDA   (FP),Y
   LDY   #$29
   STA   (FP),Y
   LDY   #$0C
   LDA   (FP),Y
   LDY   #$2A
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@0
   LDA   #$00
   JMP   .sx_done@1
.sx_neg@0:
   LDA   #$FF
.sx_done@1:
   LDY   #$2B
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
   LDY   #$2A
   LDA   (FP),Y
   LDY   #$31
   STA   (FP),Y
   LDY   #$2B
   LDA   (FP),Y
   LDY   #$32
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   JMP   .cond_end@1
.cond_else@0:
   LDA   #$7F
   LDY   #$2E
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
.cond_end@1:
   LDY   #$2E
   LDA   (FP),Y
   LDY   #$35
   STA   (FP),Y
   LDY   #$2F
   LDA   (FP),Y
   LDY   #$36
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$CB
   STA   (FP),Y
   LDY   #$36
   LDA   (FP),Y
   LDY   #$CC
   STA   (FP),Y
   LDA   #$03
   LDY   #$37
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   ORA   #$00
   BPL   .sx_done@3
   LDA   #$FF
.sx_done@3:
   LDY   #$39
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$09
   LDA   (FP),Y
   LDY   #$37
   ORA   (FP),Y
   LDY   #$3B
   STA   (FP),Y
   LDY   #$0A
   LDA   (FP),Y
   LDY   #$38
   ORA   (FP),Y
   LDY   #$3C
   STA   (FP),Y
   LDY   #$0B
   LDA   (FP),Y
   LDY   #$39
   ORA   (FP),Y
   LDY   #$3D
   STA   (FP),Y
   LDY   #$0C
   LDA   (FP),Y
   LDY   #$3A
   ORA   (FP),Y
   LDY   #$3E
   STA   (FP),Y
   LDY   #$3B
   LDA   (FP),Y
   LDY   #$09
   STA   (FP),Y
   LDY   #$3C
   LDA   (FP),Y
   LDY   #$0A
   STA   (FP),Y
   LDY   #$3D
   LDA   (FP),Y
   LDY   #$0B
   STA   (FP),Y
   LDY   #$3E
   LDA   (FP),Y
   LDY   #$0C
   STA   (FP),Y
   LDA   gp8
   STA   DPTR
   LDA   gp8+1
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$3F
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$40
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$41
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$42
   STA   (FP),Y
   LDX   #$05
.shift_loop@4:
   LDY   #$41
   LDA   (FP),Y
   ASL
   STA   (FP),Y
   INY
   LDA   (FP),Y
   ROL
   STA   (FP),Y
   DEX
   BNE   .shift_loop@4
   LDA   gp8
   STA   DPTR
   LDA   gp8+1
   STA   DPTR+1
   DEY
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$42
   LDA   (FP),Y
   LDY   #$01
   STA   (DPTR),Y
   LDA   gp7
   STA   DPTR
   LDA   gp7+1
   STA   DPTR+1
   DEY
   LDA   (DPTR),Y
   LDY   #$43
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$44
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   SBC   g2
   INY
   LDA   (FP),Y
   SBC   g2+1
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
   LDY   #$41
   LDA   (FP),Y
   LDY   #$47
   EOR   (FP),Y
   LDY   #$49
   STA   (FP),Y
   LDY   #$42
   LDA   (FP),Y
   LDY   #$48
   EOR   (FP),Y
   LDY   #$4A
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$4B
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$4C
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
   LDY   #$4B
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$4C
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$4D
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$4E
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   JSR   mix
   LDA   #$FF
   LDY   #$4F
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   SEC
   LDY   #$CD
   SBC   (FP),Y
   LDY   #$51
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$CE
   SBC   (FP),Y
   LDY   #$52
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$53
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$54
   STA   (FP),Y
   LDY   #$03
   LDA   (FP),Y
   EOR   #$5A
   LDY   #$55
   STA   (FP),Y
   LDY   #$04
   LDA   (FP),Y
   LDY   #$56
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$03
   STA   (FP),Y
   LDY   #$56
   LDA   (FP),Y
   LDY   #$04
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
   INY
   ORA   (FP),Y
   BNE   .lb_skip@2
   JMP   .if_end@2
.lb_skip@2:
   LDA   g1
   LDY   #$59
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
   LDA   gp7
   STA   DPTR
   LDA   gp7+1
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
   ORA   #$05
   LDY   #$5F
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$60
   STA   (FP),Y
   LDA   gp7
   STA   DPTR
   LDA   gp7+1
   STA   DPTR+1
   DEY
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$60
   LDA   (FP),Y
   LDY   #$01
   STA   (DPTR),Y
   LDY   #$5F
   LDA   (FP),Y
   LDY   #$61
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$62
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$59
   LDA   (FP),Y
   CLC
   LDY   #$61
   ADC   (FP),Y
   LDY   #$65
   STA   (FP),Y
   LDY   #$5A
   LDA   (FP),Y
   LDY   #$62
   ADC   (FP),Y
   LDY   #$66
   STA   (FP),Y
   LDY   #$5B
   LDA   (FP),Y
   LDY   #$63
   ADC   (FP),Y
   LDY   #$67
   STA   (FP),Y
   LDY   #$5C
   LDA   (FP),Y
   LDY   #$64
   ADC   (FP),Y
   LDY   #$68
   STA   (FP),Y
   LDY   #$65
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   INY
   ORA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@1
   JMP   .if_end@3
.lb_skip@1:
   LDA   #$02
   LDY   #$69
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
   LDY   #$6A
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
   LDY   #$6E
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
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@0
   JMP   .if_else@5
.lb_skip@0:
   LDA   #$00
   LDY   #$74
   STA   (FP),Y
   ORA   #$00
   BEQ   .cond_else@6
   LDA   #<gs6
   INY
   STA   (FP),Y
   LDA   #>gs6
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$08
   LDY   #$77
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$78
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$79
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$7A
   STA   (FP),Y
   LDY   #$02
   LDA   (DPTR),Y
   LDY   #$7B
   STA   (FP),Y
   LDY   #$03
   LDA   (DPTR),Y
   LDY   #$7C
   STA   (FP),Y
   LDY   #$79
   LDA   (FP),Y
   LDY   #$7D
   STA   (FP),Y
   LDY   #$7A
   LDA   (FP),Y
   LDY   #$7E
   STA   (FP),Y
   LDY   #$7B
   LDA   (FP),Y
   LDY   #$7F
   STA   (FP),Y
   LDY   #$7C
   LDA   (FP),Y
   LDY   #$80
   STA   (FP),Y
   JMP   .cond_end@7
.cond_else@6:
   LDA   g1
   LDY   #$81
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
   LDY   #$81
   LDA   (FP),Y
   LDY   #$7D
   STA   (FP),Y
   LDY   #$82
   LDA   (FP),Y
   LDY   #$7E
   STA   (FP),Y
   LDY   #$83
   LDA   (FP),Y
   LDY   #$7F
   STA   (FP),Y
   LDY   #$84
   LDA   (FP),Y
   LDY   #$80
   STA   (FP),Y
.cond_end@7:
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
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$85
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
   ADC   #$D1
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$C9
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   RTS
.if_else@5:
   LDA   gp8
   STA   DPTR
   LDA   gp8+1
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$A6
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$A7
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$03
   ORA   (FP),Y
   LDY   #$A8
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$04
   ORA   (FP),Y
   LDY   #$A9
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$AA
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$AB
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
   LDY   #$AA
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$AB
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$AC
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$AD
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   JSR   mix
   LDA   @5.s17+2
   LDY   #$B0
   STA   (FP),Y
   LSR
   STA   (FP),Y
   LDA   @5.s17+1
   DEY
   STA   (FP),Y
   ROR
   STA   (FP),Y
   LDA   @5.s17
   DEY
   STA   (FP),Y
   ROR
   STA   (FP),Y
   LDY   #$B0
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
   STA   @5.s17
   INY
   LDA   (FP),Y
   STA   @5.s17+1
   INY
   LDA   (FP),Y
   STA   @5.s17+2
.if_end@3:
.if_end@2:
   LDA   #<gs6
   LDY   #$B1
   STA   (FP),Y
   LDA   #>gs6
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$08
   LDY   #$B3
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$B4
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$B5
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$B6
   STA   (FP),Y
   LDY   #$02
   LDA   (DPTR),Y
   LDY   #$B7
   STA   (FP),Y
   LDY   #$03
   LDA   (DPTR),Y
   LDY   #$B8
   STA   (FP),Y
   LDY   #$03
   LDA   (FP),Y
   LDY   #$B9
   STA   (FP),Y
   LDY   #$04
   LDA   (FP),Y
   LDY   #$BA
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$B5
   LDA   (FP),Y
   SEC
   LDY   #$B9
   SBC   (FP),Y
   LDY   #$BD
   STA   (FP),Y
   LDY   #$B6
   LDA   (FP),Y
   LDY   #$BA
   SBC   (FP),Y
   LDY   #$BE
   STA   (FP),Y
   LDY   #$B7
   LDA   (FP),Y
   LDY   #$BB
   SBC   (FP),Y
   LDY   #$BF
   STA   (FP),Y
   LDY   #$B8
   LDA   (FP),Y
   LDY   #$BC
   SBC   (FP),Y
   LDY   #$C0
   STA   (FP),Y
   LDY   #$BD
   LDA   (FP),Y
   LDY   #$C1
   STA   (FP),Y
   LDY   #$BE
   LDA   (FP),Y
   LDY   #$C2
   STA   (FP),Y
   LDY   #$BF
   LDA   (FP),Y
   LDY   #$C3
   STA   (FP),Y
   LDY   #$C0
   LDA   (FP),Y
   LDY   #$C4
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
   LDY   #$C1
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
   ADC   #$D1
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

f18:
   SUBROUTINE

   ; prologue: 4 arg bytes, 108 local bytes
   SEC
   LDA   SSP
   SBC   #$6E
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$6D
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
   LDA   #$01
   INY
   STA   (FP),Y
   LDA   #$00
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
   LDY   #$07
   LDA   (FP),Y
   LDY   #$0B
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$03
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
   DEY
   LDA   (FP),Y
   LDY   #$13
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$14
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@8
   LDA   #$00
   JMP   .sx_done@9
.sx_neg@8:
   LDA   #$FF
.sx_done@9:
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
   INY
   STA   (FP),Y
   LDA   #$00
   STA   HARGS
   LDY   #$01
   STY   HARGS+1
   DEY
   STY   HARGS+2
   STY   HARGS+3
   STY   HARGS+4
   STY   HARGS+5
   STY   HARGS+6
   STA   HARGS+7
   LDY   #$13
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
   JSR   sdivmod64
   LDA   HARGS+16
   LDY   #$1B
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
   LDY   #$1B
   LDA   (FP),Y
   LDY   #$23
   STA   (FP),Y
   LDY   #$1C
   LDA   (FP),Y
   LDY   #$24
   STA   (FP),Y
   LDY   #$71
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$25
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$26
   STA   (FP),Y
   LDA   ga5+21
   INY
   STA   (FP),Y
   LDA   ga5+22
   INY
   STA   (FP),Y
   LDA   ga5+23
   INY
   STA   (FP),Y
   LDA   ga5+24
   INY
   STA   (FP),Y
   LDA   ga5+25
   INY
   STA   (FP),Y
   LDA   ga5+26
   INY
   STA   (FP),Y
   LDA   ga5+27
   INY
   STA   (FP),Y
   LDY   #$27
   LDA   (FP),Y
   LDY   #$2E
   STA   (FP),Y
   CLC
   LDA   FP
   ADC   #$10
   INY
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$07
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
   LDY   #$25
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$26
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   LDY   #$2E
   LDA   (FP),Y
   LDY   #$05
   STA   (SSP),Y
   LDY   #$2F
   LDA   (FP),Y
   LDY   #$06
   STA   (SSP),Y
   LDY   #$30
   LDA   (FP),Y
   LDY   #$07
   STA   (SSP),Y
   JSR   f9
   LDA   HARGS+16
   LDY   #$31
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
   LDY   #$31
   LDA   (FP),Y
   LDY   #$39
   STA   (FP),Y
   LDY   #$10
   STA   (FP),Y
   LDA   #<gs6
   LDY   #$3A
   STA   (FP),Y
   LDA   #>gs6
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$08
   LDY   #$3C
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$3D
   STA   (FP),Y
   DEY
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
   LDY   #$02
   LDA   (DPTR),Y
   LDY   #$40
   STA   (FP),Y
   LDY   #$03
   LDA   (DPTR),Y
   LDY   #$41
   STA   (FP),Y
   LDA   #$01
   INY
   STA   (FP),Y
   LDA   #$00
   INY
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
   BMI   .sx_neg@10
   LDA   #$00
   JMP   .sx_done@11
.sx_neg@10:
   LDA   #$FF
.sx_done@11:
   LDY   #$46
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$3E
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
   LDY   #$44
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
   LDY   #$48
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
   LDY   #$48
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$49
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$4A
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$4B
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   JSR   mix
   LDA   #<gs6
   LDY   #$4C
   STA   (FP),Y
   LDA   #>gs6
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$01
   LDY   #$4E
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$4F
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$50
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$51
   STA   (FP),Y
   LDY   #$02
   LDA   (DPTR),Y
   LDY   #$52
   STA   (FP),Y
   LDY   #$03
   LDA   (DPTR),Y
   LDY   #$53
   STA   (FP),Y
   LDY   #$04
   LDA   (DPTR),Y
   LDY   #$54
   STA   (FP),Y
   LDY   #$05
   LDA   (DPTR),Y
   LDY   #$55
   STA   (FP),Y
   LDY   #$06
   LDA   (DPTR),Y
   LDY   #$56
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
   BEQ   .cond_else@8
   LDY   #$70
   LDA   (FP),Y
   LDY   #$57
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@12
   LDA   #$00
   JMP   .sx_done@13
.sx_neg@12:
   LDA   #$FF
.sx_done@13:
   LDY   #$58
   STA   (FP),Y
   DEY
   LDA   (FP),Y
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
   DEY
   LDA   (FP),Y
   LDY   #$5C
   STA   (FP),Y
   JMP   .cond_end@9
.cond_else@8:
   LDY   #$03
   LDA   (FP),Y
   LDY   #$5B
   STA   (FP),Y
   LDY   #$04
   LDA   (FP),Y
   LDY   #$5C
   STA   (FP),Y
.cond_end@9:
   LDY   #$10
   LDA   (FP),Y
   LDY   #$5D
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@14
   LDA   #$00
   JMP   .sx_done@15
.sx_neg@14:
   LDA   #$FF
.sx_done@15:
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
   LDY   #$5B
   LDA   (FP),Y
   SEC
   LDY   #$5F
   SBC   (FP),Y
   LDY   #$61
   STA   (FP),Y
   LDY   #$5C
   LDA   (FP),Y
   LDY   #$60
   SBC   (FP),Y
   LDY   #$62
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$63
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$64
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
   LDY   #$63
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$64
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$65
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$66
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   JSR   mix
   LDA   g1
   LDY   #$67
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
   LDY   #$67
   LDA   (FP),Y
   LDY   #$6B
   STA   (FP),Y
   LDY   #$68
   LDA   (FP),Y
   LDY   #$6C
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
   ADC   #$72
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$6D
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

f27:
   SUBROUTINE

   ; prologue: 2 arg bytes, 162 local bytes
   SEC
   LDA   SSP
   SBC   #$A4
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$A3
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
   LDA   #$00
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
   LDY   #$03
   LDA   (FP),Y
   LDY   #$07
   STA   (FP),Y
   INY
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
   LDA   #$03
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$80
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$FF
   INY
   STA   (FP),Y
   LDA   #$7F
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$17
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$18
   STA   (FP),Y
   LDY   #$14
   LDA   (FP),Y
   LDY   #$19
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@16
   LDA   #$00
   JMP   .sx_done@17
.sx_neg@16:
   LDA   #$FF
.sx_done@17:
   LDY   #$1A
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ORA   #$01
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
   LDY   #$14
   STA   (FP),Y
   LDA   #$00
   LDY   #$1E
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$17
   LDA   (FP),Y
   ORA   #$05
   LDY   #$20
   STA   (FP),Y
   LDY   #$18
   LDA   (FP),Y
   LDY   #$21
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$17
   STA   (FP),Y
   LDY   #$21
   LDA   (FP),Y
   LDY   #$18
   STA   (FP),Y
   LDY   #$20
   LDA   (FP),Y
   LDY   #$22
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$23
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   EOR   z4
   LDY   #$24
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   EOR   z4+1
   LDY   #$25
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$26
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$27
   STA   (FP),Y
   LDY   #$A5
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$26
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$27
   LDA   (FP),Y
   LDY   #$01
   STA   (DPTR),Y
   LDA   #<gs6
   LDY   #$28
   STA   (FP),Y
   LDA   #>gs6
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$08
   LDY   #$2A
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$2B
   STA   (FP),Y
   DEY
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
   LDY   #$02
   LDA   (DPTR),Y
   LDY   #$2E
   STA   (FP),Y
   LDY   #$03
   LDA   (DPTR),Y
   LDY   #$2F
   STA   (FP),Y
   LDA   #$81
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   ORA   #$00
   BPL   .sx_done@19
   LDA   #$FF
.sx_done@19:
   LDY   #$32
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$2C
   LDA   (FP),Y
   LDY   #$30
   AND   (FP),Y
   LDY   #$34
   STA   (FP),Y
   LDY   #$2D
   LDA   (FP),Y
   LDY   #$31
   AND   (FP),Y
   LDY   #$35
   STA   (FP),Y
   LDY   #$2E
   LDA   (FP),Y
   LDY   #$32
   AND   (FP),Y
   LDY   #$36
   STA   (FP),Y
   LDY   #$2F
   LDA   (FP),Y
   LDY   #$33
   AND   (FP),Y
   LDY   #$37
   STA   (FP),Y
   LDY   #$2A
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$34
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$35
   LDA   (FP),Y
   LDY   #$01
   STA   (DPTR),Y
   LDY   #$36
   LDA   (FP),Y
   LDY   #$02
   STA   (DPTR),Y
   LDY   #$37
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
   LDY   #$34
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$35
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$36
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$37
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   JSR   mix
   LDA   g3
   EOR   #$FF
   LDY   #$38
   STA   (FP),Y
   LDA   g3+1
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   g3
   INY
   LDA   (FP),Y
   STA   g3+1
   LDA   #$00
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
   ORA   #$00
   BPL   .sx_done@21
   LDA   #$FF
.sx_done@21:
   LDY   #$3E
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$05
   LDA   (FP),Y
   CLC
   LDY   #$3C
   ADC   (FP),Y
   LDY   #$40
   STA   (FP),Y
   LDY   #$06
   LDA   (FP),Y
   LDY   #$3D
   ADC   (FP),Y
   LDY   #$41
   STA   (FP),Y
   LDY   #$07
   LDA   (FP),Y
   LDY   #$3E
   ADC   (FP),Y
   LDY   #$42
   STA   (FP),Y
   LDY   #$08
   LDA   (FP),Y
   LDY   #$3F
   ADC   (FP),Y
   LDY   #$43
   STA   (FP),Y
   LDY   #$40
   LDA   (FP),Y
   LDY   #$05
   STA   (FP),Y
   LDY   #$41
   LDA   (FP),Y
   LDY   #$06
   STA   (FP),Y
   LDY   #$42
   LDA   (FP),Y
   LDY   #$07
   STA   (FP),Y
   LDY   #$43
   LDA   (FP),Y
   LDY   #$08
   STA   (FP),Y
   LDA   g2
   CLC
   ADC   #$05
   LDY   #$44
   STA   (FP),Y
   LDA   g2+1
   ADC   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   g2
   INY
   LDA   (FP),Y
   STA   g2+1
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
   LDY   #$46
   LDA   (FP),Y
   SEC
   SBC   #$01
   LDY   #$4A
   STA   (FP),Y
   LDY   #$47
   LDA   (FP),Y
   SBC   #$00
   LDY   #$4B
   STA   (FP),Y
   LDY   #$48
   LDA   (FP),Y
   SBC   #$00
   LDY   #$4C
   STA   (FP),Y
   LDY   #$49
   LDA   (FP),Y
   SBC   #$00
   LDY   #$4D
   STA   (FP),Y
   LDY   #$4A
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
   LDY   #$14
   LDA   (FP),Y
   LDY   #$4E
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@22
   LDA   #$00
   JMP   .sx_done@23
.sx_neg@22:
   LDA   #$FF
.sx_done@23:
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
   BMI   .sx_neg@24
   LDA   #$00
   JMP   .sx_done@25
.sx_neg@24:
   LDA   #$FF
.sx_done@25:
   LDY   #$52
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$46
   LDA   (FP),Y
   CLC
   LDY   #$50
   ADC   (FP),Y
   LDY   #$54
   STA   (FP),Y
   LDY   #$47
   LDA   (FP),Y
   LDY   #$51
   ADC   (FP),Y
   LDY   #$55
   STA   (FP),Y
   LDY   #$48
   LDA   (FP),Y
   LDY   #$52
   ADC   (FP),Y
   LDY   #$56
   STA   (FP),Y
   LDY   #$49
   LDA   (FP),Y
   LDY   #$53
   ADC   (FP),Y
   LDY   #$57
   STA   (FP),Y
   LDY   #$54
   LDA   (FP),Y
   LDY   #$58
   STA   (FP),Y
   LDY   #$55
   LDA   (FP),Y
   LDY   #$59
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
   ADC   #$A6
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$A3
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

   ; prologue: 0 arg bytes, 126 local bytes
   SEC
   LDA   SSP
   SBC   #$80
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$7F
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   LDA   #$58
   LDY   #$01
   STA   (FP),Y
   LDA   #$79
   INY
   STA   (FP),Y
   LDA   #$9C
   INY
   STA   (FP),Y
   LDA   #$F2
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
   LDA   #$FF
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
   LDA   gp7
   STA   DPTR
   LDA   gp7+1
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$0F
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
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
   LDA   gp7
   STA   DPTR
   LDA   gp7+1
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
   INY
   ORA   (FP),Y
   BNE   .lb_skip@4
   JMP   .if_end@10
.lb_skip@4:
   LDA   #$00
   LDY   #$17
   STA   (FP),Y
   INY
   STA   (FP),Y
.loop@2_start:
   LDY   #$18
   LDA   (FP),Y
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
   DEY
   LDA   (FP),Y
   CLC
   ADC   g3
   LDY   #$1D
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   g3+1
   LDY   #$1E
   STA   (FP),Y
   LDY   #$0A
   LDA   (FP),Y
   LDY   #$1F
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@40
   LDA   #$00
   JMP   .sx_done@41
.sx_neg@40:
   LDA   #$FF
.sx_done@41:
   LDY   #$20
   STA   (FP),Y
   LDY   #$0A
   LDA   (FP),Y
   LDY   #$21
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@42
   LDA   #$00
   JMP   .sx_done@43
.sx_neg@42:
   LDA   #$FF
.sx_done@43:
   LDY   #$22
   STA   (FP),Y
   LDY   #$20
   LDA   (FP),Y
   LDY   #$22
   CMP   (FP),Y
   BNE   .thread@0
   LDY   #$1F
   LDA   (FP),Y
   LDY   #$21
   CMP   (FP),Y
   BEQ   .cmp_true@44
.thread@0:
   LDA   #$00
   JMP   .cmp_end@45
.cmp_true@44:
   LDA   #$01
.cmp_end@45:
   LDY   #$23
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$25
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$26
   STA   (FP),Y
   LDY   #$1E
   LDA   (FP),Y
   LDY   #$26
   CMP   (FP),Y
   BNE   .thread@1
   LDY   #$1D
   LDA   (FP),Y
   LDY   #$25
   CMP   (FP),Y
   BEQ   .cmp_true@47
.thread@1:
   LDA   #$00
   JMP   .cmp_end@48
.cmp_true@47:
   LDA   #$01
.cmp_end@48:
   LDY   #$27
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$29
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$2A
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@50
   LDA   #$00
   JMP   .sx_done@51
.sx_neg@50:
   LDA   #$FF
.sx_done@51:
   LDY   #$2B
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$29
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
   LDA   g3
   EOR   #$03
   INY
   STA   (FP),Y
   LDA   g3+1
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   g3
   INY
   LDA   (FP),Y
   STA   g3+1
   LDY   #$0A
   LDA   (FP),Y
   BEQ   .lnot_true@52
   LDA   #$00
   JMP   .lnot_end@53
.lnot_true@52:
   LDA   #$01
.lnot_end@53:
   LDY   #$2F
   STA   (FP),Y
   LDA   #$00
   INY
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
   BMI   .sx_neg@54
   LDA   #$00
   JMP   .sx_done@55
.sx_neg@54:
   LDA   #$FF
.sx_done@55:
   LDY   #$33
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
   ADC   #$08
   LDY   #$37
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$38
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$31
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$32
   LDA   (FP),Y
   LDY   #$01
   STA   (DPTR),Y
   LDY   #$33
   LDA   (FP),Y
   LDY   #$02
   STA   (DPTR),Y
   LDY   #$34
   LDA   (FP),Y
   LDY   #$03
   STA   (DPTR),Y
   LDY   #$18
   LDA   (FP),Y
   CLC
   ADC   #$01
   LDY   #$39
   STA   (FP),Y
   LDY   #$18
   STA   (FP),Y
   LDY   #$3A
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
   BVC   .cmp_novf@56
   EOR   #$80
.cmp_novf@56:
   ASL
   LDA   #$00
   ROL
   LDY   #$3C
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@3
   JMP   .loop@2_start
.lb_skip@3:
   LDA   g1
   LDY   #$3E
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
   LDY   #$3E
   LDA   (FP),Y
   SEC
   SBC   #$01
   LDY   #$42
   STA   (FP),Y
   LDY   #$3F
   LDA   (FP),Y
   SBC   #$00
   LDY   #$43
   STA   (FP),Y
   LDY   #$40
   LDA   (FP),Y
   SBC   #$00
   LDY   #$44
   STA   (FP),Y
   LDY   #$41
   LDA   (FP),Y
   SBC   #$00
   LDY   #$45
   STA   (FP),Y
   LDY   #$42
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
   LDA   #$05
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   ORA   #$00
   BPL   .sx_done@58
   LDA   #$FF
.sx_done@58:
   LDY   #$48
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
   LDY   #$46
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
   LDY   #$4A
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
   LDY   #$4A
   LDA   (FP),Y
   LDY   #$05
   STA   (FP),Y
   LDY   #$4B
   LDA   (FP),Y
   LDY   #$06
   STA   (FP),Y
   LDY   #$4C
   LDA   (FP),Y
   LDY   #$07
   STA   (FP),Y
   LDY   #$4D
   LDA   (FP),Y
   LDY   #$08
   STA   (FP),Y
.if_end@10:
   LDA   gp7
   STA   DPTR
   LDA   gp7+1
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$4E
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$4F
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   SEC
   SBC   #$01
   LDY   #$50
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   SBC   #$00
   LDY   #$51
   STA   (FP),Y
   LDA   gp7
   STA   DPTR
   LDA   gp7+1
   STA   DPTR+1
   DEY
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$51
   LDA   (FP),Y
   LDY   #$01
   STA   (DPTR),Y
   LDY   #$50
   LDA   (FP),Y
   LDY   #$52
   STA   (FP),Y
   LDY   #$0A
   STA   (FP),Y
   LDY   #$05
   LDA   (FP),Y
   EOR   #$FF
   LDY   #$53
   STA   (FP),Y
   LDY   #$06
   LDA   (FP),Y
   EOR   #$FF
   LDY   #$54
   STA   (FP),Y
   LDY   #$07
   LDA   (FP),Y
   EOR   #$FF
   LDY   #$55
   STA   (FP),Y
   LDY   #$08
   LDA   (FP),Y
   EOR   #$FF
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
   LDA   #$06
   LDY   #$57
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$59
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$5A
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
   LDY   #$59
   LDA   (FP),Y
   STA   ga5+21
   INY
   LDA   (FP),Y
   STA   ga5+22
   INY
   LDA   (FP),Y
   STA   ga5+23
   INY
   LDA   (FP),Y
   STA   ga5+24
   INY
   LDA   (FP),Y
   STA   ga5+25
   INY
   LDA   (FP),Y
   STA   ga5+26
   INY
   LDA   (FP),Y
   STA   ga5+27
   LDA   #<gs6
   INY
   STA   (FP),Y
   LDA   #>gs6
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$08
   LDY   #$62
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$63
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$64
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$65
   STA   (FP),Y
   LDY   #$02
   LDA   (DPTR),Y
   LDY   #$66
   STA   (FP),Y
   LDY   #$03
   LDA   (DPTR),Y
   LDY   #$67
   STA   (FP),Y
   LDA   #$81
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   ORA   #$00
   BPL   .sx_done@60
   LDA   #$FF
.sx_done@60:
   LDY   #$6A
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$64
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
   LDY   #$6C
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
   LDY   #$62
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$6C
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$6D
   LDA   (FP),Y
   LDY   #$01
   STA   (DPTR),Y
   LDY   #$6E
   LDA   (FP),Y
   LDY   #$02
   STA   (DPTR),Y
   LDY   #$6F
   LDA   (FP),Y
   LDY   #$03
   STA   (DPTR),Y
   LDY   #$6C
   LDA   (FP),Y
   LDY   #$70
   STA   (FP),Y
   LDY   #$6D
   LDA   (FP),Y
   LDY   #$71
   STA   (FP),Y
   LDY   #$6E
   LDA   (FP),Y
   LDY   #$72
   STA   (FP),Y
   LDY   #$6F
   LDA   (FP),Y
   LDY   #$73
   STA   (FP),Y
   LDA   #$00
   INY
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
   LDY   #$79
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$7A
   STA   (FP),Y
   DEY
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
   LDY   #$72
   LDA   (FP),Y
   LDY   #$02
   STA   (DPTR),Y
   LDY   #$73
   LDA   (FP),Y
   LDY   #$03
   STA   (DPTR),Y
   LDY   #$74
   LDA   (FP),Y
   LDY   #$04
   STA   (DPTR),Y
   LDY   #$75
   LDA   (FP),Y
   LDY   #$05
   STA   (DPTR),Y
   LDY   #$76
   LDA   (FP),Y
   LDY   #$06
   STA   (DPTR),Y
   JSR   fin0
   JSR   fin1
   JSR   fin2
   LDA   acc
   LDY   #$7B
   STA   (FP),Y
   LDA   acc+1
   AND   #$7F
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$7D
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$7E
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
   ADC   #$80
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$7F
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
   DC.W  $7FFF

g3:
   DC.W  $FFFF

ga5:
   DC.B  $FF
   DC.B  $FF
   DC.B  $FF
   DC.B  $7F
   DS.B  3
   DC.B  $02
   DS.B  9
   DC.B  $80
   DS.B  3
   DC.B  $FF
   DC.B  $FF
   DC.B  $FF
   DC.B  $7F
   DS.B  3

gs6:
   DS.B  3
   DC.B  $01
   DS.B  4
   DC.L  $7FFFFFFF

gp7:
   DC.W  z4

gp8:
   DC.W  g3

@5.s17:
   DC.B  $FF
   DC.B  $7F
   DS.B  1

@12.s26:
   DC.B  $FF

@18.s34:
   DC.B  $05

__zp_init:
   SUBROUTINE

   LDA   #$00
   STA   z4
   STA   z4+1
   RTS
