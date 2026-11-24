SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
z5	EQU	$26
z6	EQU	$2E
z7	EQU	$35

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
   LDA   g3
   LDY   #$09
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
   LDA   g4
   LDY   #$0D
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

   ; prologue: 0 arg bytes, 28 local bytes
   SEC
   LDA   SSP
   SBC   #$1E
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$1D
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   LDA   z5
   LDY   #$01
   STA   (FP),Y
   LDA   z5+1
   INY
   STA   (FP),Y
   LDA   z5+2
   INY
   STA   (FP),Y
   LDA   z5+3
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
   LDA   z6
   LDY   #$05
   STA   (FP),Y
   LDA   z6+1
   INY
   STA   (FP),Y
   LDA   z6+2
   INY
   STA   (FP),Y
   LDA   z6+3
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
   LDA   z7
   LDY   #$09
   STA   (FP),Y
   LDA   z7+1
   INY
   STA   (FP),Y
   LDA   z7+2
   INY
   STA   (FP),Y
   LDA   z7+3
   INY
   STA   (FP),Y
   LDA   z7+4
   INY
   STA   (FP),Y
   LDA   z7+5
   INY
   STA   (FP),Y
   LDA   z7+6
   INY
   STA   (FP),Y
   LDA   z7+7
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
   LDA   ga8
   LDY   #$15
   STA   (FP),Y
   LDA   ga8+1
   INY
   STA   (FP),Y
   LDA   ga8+2
   INY
   STA   (FP),Y
   LDA   ga8+3
   INY
   STA   (FP),Y
   LDY   #$15
   LDA   (FP),Y
   LDY   #$19
   STA   (FP),Y
   LDY   #$16
   LDA   (FP),Y
   LDY   #$1A
   STA   (FP),Y
   LDY   #$17
   LDA   (FP),Y
   LDY   #$1B
   STA   (FP),Y
   LDY   #$18
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
   JSR   mix

   ; epilogue
   CLC
   LDA   FP
   ADC   #$1E
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$1D
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

   LDA   ga8+4
   LDY   #$01
   STA   (FP),Y
   LDA   ga8+5
   INY
   STA   (FP),Y
   LDA   ga8+6
   INY
   STA   (FP),Y
   LDA   ga8+7
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
   LDA   ga8+8
   LDY   #$09
   STA   (FP),Y
   LDA   ga8+9
   INY
   STA   (FP),Y
   LDA   ga8+10
   INY
   STA   (FP),Y
   LDA   ga8+11
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
   LDY   #$0C
   LDA   (FP),Y
   LDY   #$10
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
   LDA   ga8+12
   LDY   #$11
   STA   (FP),Y
   LDA   ga8+13
   INY
   STA   (FP),Y
   LDA   ga8+14
   INY
   STA   (FP),Y
   LDA   ga8+15
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
   LDY   #$14
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
   LDA   #<gs9
   LDY   #$19
   STA   (FP),Y
   LDA   #>gs9
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
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$1E
   STA   (FP),Y
   LDY   #$02
   LDA   (DPTR),Y
   LDY   #$1F
   STA   (FP),Y
   LDY   #$03
   LDA   (DPTR),Y
   LDY   #$20
   STA   (FP),Y
   LDY   #$04
   LDA   (DPTR),Y
   LDY   #$21
   STA   (FP),Y
   LDY   #$05
   LDA   (DPTR),Y
   LDY   #$22
   STA   (FP),Y
   LDY   #$06
   LDA   (DPTR),Y
   LDY   #$23
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
   LDY   #$20
   LDA   (FP),Y
   LDY   #$27
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

   ; epilogue
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
   RTS

fin3:
   SUBROUTINE

   ; prologue: 0 arg bytes, 9 local bytes
   SEC
   LDA   SSP
   SBC   #$0B
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$0A
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   LDA   #<gs9
   LDY   #$01
   STA   (FP),Y
   LDA   #>gs9
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$08
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

   ; epilogue
   CLC
   LDA   FP
   ADC   #$0B
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$0A
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   RTS

f12:
   SUBROUTINE

   ; prologue: 4 arg bytes, 226 local bytes
   SEC
   LDA   SSP
   SBC   #$E4
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$E3
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
   DEY
   LDA   (FP),Y
   LDY   #$07
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$08
   STA   (FP),Y
   LDA   #$01
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
   LDA   gp10
   STA   DPTR
   LDA   gp10+1
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$0D
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$0E
   STA   (FP),Y
   LDY   #$02
   LDA   (DPTR),Y
   LDY   #$0F
   STA   (FP),Y
   LDY   #$03
   LDA   (DPTR),Y
   LDY   #$10
   STA   (FP),Y
   LDY   #$04
   LDA   (DPTR),Y
   LDY   #$11
   STA   (FP),Y
   LDY   #$05
   LDA   (DPTR),Y
   LDY   #$12
   STA   (FP),Y
   LDY   #$06
   LDA   (DPTR),Y
   LDY   #$13
   STA   (FP),Y
   LDY   #$07
   LDA   (DPTR),Y
   LDY   #$14
   STA   (FP),Y
   LDA   #<ga8
   INY
   STA   (FP),Y
   LDA   #>ga8
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
   LDY   #$19
   STA   (FP),Y
   STA   HARGS
   LDA   #$04
   STA   HARGS+1
   JSR   mul8
   LDA   HARGS+2
   LDY   #$1A
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$15
   LDA   (FP),Y
   CLC
   LDY   #$1B
   ADC   (FP),Y
   LDY   #$1D
   STA   (FP),Y
   LDY   #$16
   LDA   (FP),Y
   LDY   #$1C
   ADC   (FP),Y
   LDY   #$1E
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$1F
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$20
   STA   (FP),Y
   LDY   #$02
   LDA   (DPTR),Y
   LDY   #$21
   STA   (FP),Y
   LDY   #$03
   LDA   (DPTR),Y
   LDY   #$22
   STA   (FP),Y
   LDA   #$5A
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   ORA   #$00
   BPL   .sx_done@1
   LDA   #$FF
.sx_done@1:
   LDY   #$25
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$1F
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
   LDY   #$27
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
   LDY   #$1D
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
   LDY   #$29
   LDA   (FP),Y
   LDY   #$02
   STA   (DPTR),Y
   LDY   #$2A
   LDA   (FP),Y
   LDY   #$03
   STA   (DPTR),Y
   LDY   #$27
   LDA   (FP),Y
   LDY   #$2B
   STA   (FP),Y
   LDY   #$28
   LDA   (FP),Y
   LDY   #$2C
   STA   (FP),Y
   LDY   #$29
   LDA   (FP),Y
   LDY   #$2D
   STA   (FP),Y
   LDY   #$2A
   LDA   (FP),Y
   LDY   #$2E
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@2
   LDA   #$00
   JMP   .sx_done@3
.sx_neg@2:
   LDA   #$FF
.sx_done@3:
   LDY   #$2F
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$0D
   LDA   (FP),Y
   LDY   #$2B
   ORA   (FP),Y
   LDY   #$33
   STA   (FP),Y
   LDY   #$0E
   LDA   (FP),Y
   LDY   #$2C
   ORA   (FP),Y
   LDY   #$34
   STA   (FP),Y
   LDY   #$0F
   LDA   (FP),Y
   LDY   #$2D
   ORA   (FP),Y
   LDY   #$35
   STA   (FP),Y
   LDY   #$10
   LDA   (FP),Y
   LDY   #$2E
   ORA   (FP),Y
   LDY   #$36
   STA   (FP),Y
   LDY   #$11
   LDA   (FP),Y
   LDY   #$2F
   ORA   (FP),Y
   LDY   #$37
   STA   (FP),Y
   LDY   #$12
   LDA   (FP),Y
   LDY   #$30
   ORA   (FP),Y
   LDY   #$38
   STA   (FP),Y
   LDY   #$13
   LDA   (FP),Y
   LDY   #$31
   ORA   (FP),Y
   LDY   #$39
   STA   (FP),Y
   LDY   #$14
   LDA   (FP),Y
   LDY   #$32
   ORA   (FP),Y
   LDY   #$3A
   STA   (FP),Y
   LDY   #$33
   LDA   (FP),Y
   LDY   #$3B
   STA   (FP),Y
   LDY   #$34
   LDA   (FP),Y
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
   SEC
   LDA   SSP
   SBC   #$04
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDY   #$3B
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$3C
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$3D
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$3E
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   JSR   mix
   LDY   #$03
   LDA   (FP),Y
   LDY   #$3F
   STA   (FP),Y
   STA   g3
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
.loop@0_continue:
   LDY   #$42
   STA   (FP),Y
   CLC
   ADC   #$01
   INY
   STA   (FP),Y
   LDY   #$41
   STA   (FP),Y
   INY
   LDA   (FP),Y
   LDY   #$44
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
   BVC   .cmp_novf@4
   EOR   #$80
.cmp_novf@4:
   ASL
   LDA   #$00
   ROL
   LDY   #$46
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
   JMP   .loop@0_break
.loop@1_start:
   LDY   #$49
   LDA   (FP),Y
   INY
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
   BVC   .cmp_novf@5
   EOR   #$80
.cmp_novf@5:
   ASL
   LDA   #$00
   ROL
   LDY   #$4C
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@5
   JMP   .loop@0_continue
.lb_skip@5:
   LDA   g4
   LDY   #$4E
   STA   (FP),Y
   LDA   #$00
   INY
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
   BMI   .sx_neg@6
   LDA   #$00
   JMP   .sx_done@7
.sx_neg@6:
   LDA   #$FF
.sx_done@7:
   LDY   #$52
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
   CMP   (FP),Y
   BNE   .cmp_true@8
   LDA   #$00
   DEY
   CMP   (FP),Y
   BNE   .cmp_true@8
   LDA   #$00
   DEY
   CMP   (FP),Y
   BNE   .cmp_true@8
   LDA   #$00
   DEY
   CMP   (FP),Y
   BNE   .cmp_true@8
   LDA   #$00
   DEY
   CMP   (FP),Y
   BNE   .cmp_true@8
   LDA   #$01
   DEY
   CMP   (FP),Y
   BNE   .cmp_end@9
   LDA   #$00
   DEY
   CMP   (FP),Y
   BNE   .cmp_true@8
   LDA   #$00
   DEY
   CMP   (FP),Y
   BNE   .cmp_true@8
   LDA   #$00
   JMP   .cmp_end@9
.cmp_true@8:
   LDA   #$01
.cmp_end@9:
   LDY   #$58
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
   BEQ   .cmp_true@11
   LDA   #$00
   JMP   .cmp_end@12
.cmp_true@11:
   LDA   #$01
.cmp_end@12:
   LDY   #$5E
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .case@3
   LDY   #$5C
   LDA   (FP),Y
   CMP   #$01
   BEQ   .cmp_true@13
   LDA   #$00
   JMP   .cmp_end@14
.cmp_true@13:
   LDA   #$01
.cmp_end@14:
   LDY   #$60
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@3
   JMP   .case@4
.lb_skip@3:
   LDY   #$5C
   LDA   (FP),Y
   CMP   #$06
   BEQ   .cmp_true@15
   LDA   #$00
   JMP   .cmp_end@16
.cmp_true@15:
   LDA   #$01
.cmp_end@16:
   LDY   #$62
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@2
   JMP   .case@5
.lb_skip@2:
   LDY   #$5C
   LDA   (FP),Y
   CMP   #$07
   BEQ   .cmp_true@17
   LDA   #$00
   JMP   .cmp_end@18
.cmp_true@17:
   LDA   #$01
.cmp_end@18:
   LDY   #$64
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@1
   JMP   .case@6
.lb_skip@1:
   JMP   .switch@2_break
.case@3:
   LDA   gp10
   STA   DPTR
   LDA   gp10+1
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$66
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$67
   STA   (FP),Y
   LDY   #$02
   LDA   (DPTR),Y
   LDY   #$68
   STA   (FP),Y
   LDY   #$03
   LDA   (DPTR),Y
   LDY   #$69
   STA   (FP),Y
   LDY   #$04
   LDA   (DPTR),Y
   LDY   #$6A
   STA   (FP),Y
   LDY   #$05
   LDA   (DPTR),Y
   LDY   #$6B
   STA   (FP),Y
   LDY   #$06
   LDA   (DPTR),Y
   LDY   #$6C
   STA   (FP),Y
   LDY   #$07
   LDA   (DPTR),Y
   LDY   #$6D
   STA   (FP),Y
   LDY   #$66
   LDA   (FP),Y
   CLC
   ADC   #$01
   LDY   #$6E
   STA   (FP),Y
   LDY   #$67
   LDA   (FP),Y
   ADC   #$00
   LDY   #$6F
   STA   (FP),Y
   LDY   #$68
   LDA   (FP),Y
   ADC   #$00
   LDY   #$70
   STA   (FP),Y
   LDY   #$69
   LDA   (FP),Y
   ADC   #$00
   LDY   #$71
   STA   (FP),Y
   LDY   #$6A
   LDA   (FP),Y
   ADC   #$00
   LDY   #$72
   STA   (FP),Y
   LDY   #$6B
   LDA   (FP),Y
   ADC   #$00
   LDY   #$73
   STA   (FP),Y
   LDY   #$6C
   LDA   (FP),Y
   ADC   #$00
   LDY   #$74
   STA   (FP),Y
   LDY   #$6D
   LDA   (FP),Y
   ADC   #$00
   LDY   #$75
   STA   (FP),Y
   LDA   gp10
   STA   DPTR
   LDA   gp10+1
   STA   DPTR+1
   LDY   #$6E
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$6F
   LDA   (FP),Y
   LDY   #$01
   STA   (DPTR),Y
   LDY   #$70
   LDA   (FP),Y
   LDY   #$02
   STA   (DPTR),Y
   LDY   #$71
   LDA   (FP),Y
   LDY   #$03
   STA   (DPTR),Y
   LDY   #$72
   LDA   (FP),Y
   LDY   #$04
   STA   (DPTR),Y
   LDY   #$73
   LDA   (FP),Y
   LDY   #$05
   STA   (DPTR),Y
   LDY   #$74
   LDA   (FP),Y
   LDY   #$06
   STA   (DPTR),Y
   LDY   #$75
   LDA   (FP),Y
   LDY   #$07
   STA   (DPTR),Y
   LDY   #$49
   LDA   (FP),Y
   LDY   #$76
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDA   #$01
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$76
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
   LDY   #$7A
   STA   (FP),Y
   LDA   HARGS+7
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$7C
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$7D
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@19
   LDA   #$00
   JMP   .sx_done@20
.sx_neg@19:
   LDA   #$FF
.sx_done@20:
   LDY   #$7E
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
   LDY   #$6E
   LDA   (FP),Y
   LDY   #$7C
   EOR   (FP),Y
   LDY   #$84
   STA   (FP),Y
   LDY   #$6F
   LDA   (FP),Y
   LDY   #$7D
   EOR   (FP),Y
   LDY   #$85
   STA   (FP),Y
   LDY   #$70
   LDA   (FP),Y
   LDY   #$7E
   EOR   (FP),Y
   LDY   #$86
   STA   (FP),Y
   LDY   #$71
   LDA   (FP),Y
   LDY   #$7F
   EOR   (FP),Y
   LDY   #$87
   STA   (FP),Y
   LDY   #$72
   LDA   (FP),Y
   LDY   #$80
   EOR   (FP),Y
   LDY   #$88
   STA   (FP),Y
   LDY   #$73
   LDA   (FP),Y
   LDY   #$81
   EOR   (FP),Y
   LDY   #$89
   STA   (FP),Y
   LDY   #$74
   LDA   (FP),Y
   LDY   #$82
   EOR   (FP),Y
   LDY   #$8A
   STA   (FP),Y
   LDY   #$75
   LDA   (FP),Y
   LDY   #$83
   EOR   (FP),Y
   LDY   #$8B
   STA   (FP),Y
   LDY   #$84
   LDA   (FP),Y
   LDY   #$8C
   STA   (FP),Y
   LDY   #$85
   LDA   (FP),Y
   LDY   #$8D
   STA   (FP),Y
   LDY   #$86
   LDA   (FP),Y
   LDY   #$8E
   STA   (FP),Y
   LDY   #$87
   LDA   (FP),Y
   LDY   #$8F
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$04
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDY   #$8C
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$8D
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$8E
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$8F
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   JSR   mix
.case@4:
.case@5:
.case@6:
.switch@2_break:
   LDA   g4
   LDY   #$90
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LSR
   LDY   #$93
   STA   (FP),Y
   LDA   #$00
   ROR
   DEY
   STA   (FP),Y
   LDY   #$94
   STA   (FP),Y
   STA   g4
   LDA   (FP),Y
   BNE   .lb_skip@0
   JMP   .if_end@0
.lb_skip@0:
   LDA   g4
   LDY   #$95
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ASL
   LDY   #$97
   STA   (FP),Y
   LDA   #$00
   ROL
   INY
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
   LDY   #$0B
   STA   (FP),Y
   LDY   #$98
   LDA   (FP),Y
   LDY   #$0C
   STA   (FP),Y
   LDA   g4
   LDY   #$99
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   AND   #$5A
   LDY   #$9B
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDA   #$00
   LDY   #$9C
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$9D
   STA   (FP),Y
   STA   g4
   LDA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$A0
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$A1
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@21
   LDA   #$00
   JMP   .sx_done@22
.sx_neg@21:
   LDA   #$FF
.sx_done@22:
   LDY   #$A2
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$03
   CLC
   LDY   #$A0
   ADC   (FP),Y
   LDY   #$A7
   STA   (FP),Y
   LDA   #$00
   LDY   #$A1
   ADC   (FP),Y
   LDY   #$A8
   STA   (FP),Y
   LDA   #$00
   LDY   #$A2
   ADC   (FP),Y
   LDY   #$A9
   STA   (FP),Y
   LDA   #$00
   LDY   #$A3
   ADC   (FP),Y
   LDY   #$AA
   STA   (FP),Y
   LDA   #$00
   LDY   #$A4
   ADC   (FP),Y
   LDY   #$AB
   STA   (FP),Y
   LDA   #$00
   LDY   #$A5
   ADC   (FP),Y
   LDY   #$AC
   STA   (FP),Y
   LDA   #$00
   LDY   #$A6
   ADC   (FP),Y
   LDY   #$AD
   STA   (FP),Y
   LDY   #$A7
   LDA   (FP),Y
   LDY   #$AE
   STA   (FP),Y
   LDY   #$A8
   LDA   (FP),Y
   LDY   #$AF
   STA   (FP),Y
   LDY   #$A9
   LDA   (FP),Y
   LDY   #$B0
   STA   (FP),Y
   LDY   #$AA
   LDA   (FP),Y
   LDY   #$B1
   STA   (FP),Y
   LDY   #$AB
   LDA   (FP),Y
   LDY   #$B2
   STA   (FP),Y
   LDY   #$AC
   LDA   (FP),Y
   LDY   #$B3
   STA   (FP),Y
   LDY   #$AD
   LDA   (FP),Y
   AND   #$01
   LDY   #$B4
   STA   (FP),Y
   LDY   #$AE
   LDA   (FP),Y
   LDY   #$B5
   STA   (FP),Y
   LDY   #$AF
   LDA   (FP),Y
   LDY   #$B6
   STA   (FP),Y
   LDY   #$B0
   LDA   (FP),Y
   LDY   #$B7
   STA   (FP),Y
   LDY   #$B1
   LDA   (FP),Y
   LDY   #$B8
   STA   (FP),Y
   LDY   #$B2
   LDA   (FP),Y
   LDY   #$B9
   STA   (FP),Y
   LDY   #$B3
   LDA   (FP),Y
   LDY   #$BA
   STA   (FP),Y
   LDY   #$B4
   LDA   (FP),Y
   EOR   #$01
   LDY   #$BB
   STA   (FP),Y
   LDY   #$B5
   LDA   (FP),Y
   LDY   #$BC
   STA   (FP),Y
   LDY   #$B6
   LDA   (FP),Y
   LDY   #$BD
   STA   (FP),Y
   LDY   #$B7
   LDA   (FP),Y
   LDY   #$BE
   STA   (FP),Y
   LDY   #$B8
   LDA   (FP),Y
   LDY   #$BF
   STA   (FP),Y
   LDY   #$B9
   LDA   (FP),Y
   LDY   #$C0
   STA   (FP),Y
   LDY   #$BA
   LDA   (FP),Y
   LDY   #$C1
   STA   (FP),Y
   LDY   #$BB
   LDA   (FP),Y
   SEC
   SBC   #$01
   LDY   #$C2
   STA   (FP),Y
   LDY   #$BC
   LDA   (FP),Y
   LDY   #$C3
   STA   (FP),Y
   LDY   #$BD
   LDA   (FP),Y
   LDY   #$C4
   STA   (FP),Y
   LDY   #$BE
   LDA   (FP),Y
   LDY   #$C5
   STA   (FP),Y
   LDY   #$BF
   LDA   (FP),Y
   LDY   #$C6
   STA   (FP),Y
   LDY   #$C0
   LDA   (FP),Y
   LDY   #$C7
   STA   (FP),Y
   LDY   #$C1
   LDA   (FP),Y
   LDY   #$C8
   STA   (FP),Y
   LDY   #$C2
   LDA   (FP),Y
   LDY   #$C9
   STA   (FP),Y
   LDY   #$C3
   LDA   (FP),Y
   LDY   #$CA
   STA   (FP),Y
   LDY   #$C4
   LDA   (FP),Y
   LDY   #$CB
   STA   (FP),Y
   LDY   #$C5
   LDA   (FP),Y
   LDY   #$CC
   STA   (FP),Y
   LDY   #$C6
   LDA   (FP),Y
   LDY   #$CD
   STA   (FP),Y
   LDY   #$C7
   LDA   (FP),Y
   LDY   #$CE
   STA   (FP),Y
   LDY   #$C8
   LDA   (FP),Y
   LDY   #$CF
   STA   (FP),Y
   LDY   #$C9
   LDA   (FP),Y
   AND   #$1F
   LDY   #$D0
   STA   (FP),Y
   LDY   #$CA
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
   INY
   LDA   (FP),Y
   STA   g1+4
   INY
   LDA   (FP),Y
   STA   g1+5
   INY
   LDA   (FP),Y
   STA   g1+6
.if_end@0:
   LDY   #$49
   LDA   (FP),Y
   LDY   #$D1
   STA   (FP),Y
   CLC
   ADC   #$01
   INY
   STA   (FP),Y
   LDY   #$49
   STA   (FP),Y
   JMP   .loop@1_start
.loop@0_break:
   LDY   #$07
   LDA   (FP),Y
   LDY   #$D3
   STA   (FP),Y
   LDY   #$08
   LDA   (FP),Y
   LDY   #$D4
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$D5
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$D6
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@23
   LDA   #$00
   JMP   .sx_done@24
.sx_neg@23:
   LDA   #$FF
.sx_done@24:
   LDY   #$D7
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   gp11
   STA   DPTR
   LDA   gp11+1
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$D9
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$DA
   STA   (FP),Y
   LDY   #$02
   LDA   (DPTR),Y
   LDY   #$DB
   STA   (FP),Y
   LDY   #$03
   LDA   (DPTR),Y
   LDY   #$DC
   STA   (FP),Y
   LDY   #$D5
   LDA   (FP),Y
   SEC
   LDY   #$D9
   SBC   (FP),Y
   LDY   #$DD
   STA   (FP),Y
   LDY   #$D6
   LDA   (FP),Y
   LDY   #$DA
   SBC   (FP),Y
   LDY   #$DE
   STA   (FP),Y
   LDY   #$D7
   LDA   (FP),Y
   LDY   #$DB
   SBC   (FP),Y
   LDY   #$DF
   STA   (FP),Y
   LDY   #$D8
   LDA   (FP),Y
   LDY   #$DC
   SBC   (FP),Y
   LDY   #$E0
   STA   (FP),Y
   LDY   #$DD
   LDA   (FP),Y
   LDY   #$E1
   STA   (FP),Y
   LDY   #$DE
   LDA   (FP),Y
   LDY   #$E2
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
   ADC   #$E8
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$E3
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

   ; prologue: 0 arg bytes, 185 local bytes
   SEC
   LDA   SSP
   SBC   #$BB
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$BA
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
   LDA   #$01
   INY
   STA   (FP),Y
   LDA   #$00
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
   LDY   #$06
   LDA   (FP),Y
   LDY   #$0A
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   g4
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
   ORA   #$00
   BMI   .sx_neg@25
   LDA   #$00
   JMP   .sx_done@26
.sx_neg@25:
   LDA   #$FF
.sx_done@26:
   LDY   #$11
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   ga8
   INY
   STA   (FP),Y
   LDA   ga8+1
   INY
   STA   (FP),Y
   LDA   ga8+2
   INY
   STA   (FP),Y
   LDA   ga8+3
   INY
   STA   (FP),Y
   LDY   #$12
   LDA   (FP),Y
   LDY   #$16
   CMP   (FP),Y
   BNE   .cmp_true@27
   DEY
   CMP   (FP),Y
   BNE   .cmp_true@27
   LDY   #$10
   LDA   (FP),Y
   LDY   #$14
   CMP   (FP),Y
   BNE   .cmp_true@27
   LDY   #$0F
   LDA   (FP),Y
   LDY   #$13
   CMP   (FP),Y
   BNE   .cmp_true@27
   LDA   #$00
   JMP   .cmp_end@28
.cmp_true@27:
   LDA   #$01
.cmp_end@28:
   LDY   #$17
   STA   (FP),Y
   LDA   #$00
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
   BMI   .sx_neg@30
   LDA   #$00
   JMP   .sx_done@31
.sx_neg@30:
   LDA   #$FF
.sx_done@31:
   LDY   #$1B
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
   JSR   mix
   LDA   ga8
   LDY   #$1D
   STA   (FP),Y
   LDA   ga8+1
   INY
   STA   (FP),Y
   LDA   ga8+2
   INY
   STA   (FP),Y
   LDA   ga8+3
   INY
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
   ORA   #$00
   BMI   .sx_neg@32
   LDA   #$00
   JMP   .sx_done@33
.sx_neg@32:
   LDA   #$FF
.sx_done@33:
   LDY   #$25
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
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
   INY
   STA   (FP),Y
   AND   #$1F
   INY
   STA   (FP),Y
   LDY   #$28
   LDA   (FP),Y
   CLC
   SBC   #$00
   INY
   LDA   (FP),Y
   SBC   #$00
   INY
   LDA   (FP),Y
   SBC   #$01
   INY
   LDA   (FP),Y
   SBC   #$00
   INY
   LDA   (FP),Y
   SBC   #$00
   INY
   LDA   (FP),Y
   SBC   #$00
   INY
   LDA   (FP),Y
   SBC   #$00
   LDA   #$00
   ROL
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@6
   JMP   .if_else@2
.lb_skip@6:
   LDA   #$00
   LDY   #$31
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   CLC
   ADC   #$01
   INY
   STA   (FP),Y
   LDY   #$32
   STA   (FP),Y
   INY
   LDA   (FP),Y
   LDY   #$35
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
   BVC   .cmp_novf@34
   EOR   #$80
.cmp_novf@34:
   ASL
   LDA   #$00
   ROL
   LDY   #$37
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   LDA   #<gs9
   INY
   STA   (FP),Y
   LDA   #>gs9
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$01
   LDY   #$3B
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$3C
   STA   (FP),Y
   DEY
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
   LDY   #$02
   LDA   (DPTR),Y
   LDY   #$3F
   STA   (FP),Y
   LDY   #$03
   LDA   (DPTR),Y
   LDY   #$40
   STA   (FP),Y
   LDY   #$04
   LDA   (DPTR),Y
   LDY   #$41
   STA   (FP),Y
   LDY   #$05
   LDA   (DPTR),Y
   LDY   #$42
   STA   (FP),Y
   LDY   #$06
   LDA   (DPTR),Y
   LDY   #$43
   STA   (FP),Y
   LDY   #$3D
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
   BEQ   .lnot_true@35
   LDA   #$00
   JMP   .lnot_end@36
.lnot_true@35:
   LDA   #$01
.lnot_end@36:
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
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   LDY   #$4A
   STA   (FP),Y
   LDA   #$00
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
   DEY
   LDA   (FP),Y
   CLC
   LDY   #$48
   SBC   (FP),Y
   LDY   #$4D
   LDA   (FP),Y
   LDY   #$49
   SBC   (FP),Y
   LDA   #$00
   ROL
   LDY   #$4E
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$50
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$51
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
   LDY   #$50
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$51
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   JSR   f12
   LDY   #$52
   STA   (FP),Y
   TXA
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$54
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$55
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@37
   LDA   #$00
   JMP   .sx_done@38
.sx_neg@37:
   LDA   #$FF
.sx_done@38:
   LDY   #$56
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$54
   LDA   (FP),Y
   LDY   #$07
   STA   (FP),Y
   LDY   #$55
   LDA   (FP),Y
   LDY   #$08
   STA   (FP),Y
   LDY   #$56
   LDA   (FP),Y
   LDY   #$09
   STA   (FP),Y
   INY
   STA   (FP),Y
   JMP   .if_end@1
.if_else@2:
   LDA   #<ga8
   LDY   #$58
   STA   (FP),Y
   LDA   #>ga8
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
   LDY   #$5C
   STA   (FP),Y
   STA   HARGS
   LDA   #$04
   STA   HARGS+1
   JSR   mul8
   LDA   HARGS+2
   LDY   #$5D
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$58
   LDA   (FP),Y
   CLC
   LDY   #$5E
   ADC   (FP),Y
   LDY   #$60
   STA   (FP),Y
   LDY   #$59
   LDA   (FP),Y
   LDY   #$5F
   ADC   (FP),Y
   LDY   #$61
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$62
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$63
   STA   (FP),Y
   LDY   #$02
   LDA   (DPTR),Y
   LDY   #$64
   STA   (FP),Y
   LDY   #$03
   LDA   (DPTR),Y
   LDY   #$65
   STA   (FP),Y
   LDA   #$03
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   ORA   #$00
   BPL   .sx_done@40
   LDA   #$FF
.sx_done@40:
   LDY   #$68
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$62
   LDA   (FP),Y
   LDY   #$66
   AND   (FP),Y
   LDY   #$6A
   STA   (FP),Y
   LDY   #$63
   LDA   (FP),Y
   LDY   #$67
   AND   (FP),Y
   LDY   #$6B
   STA   (FP),Y
   LDY   #$64
   LDA   (FP),Y
   LDY   #$68
   AND   (FP),Y
   LDY   #$6C
   STA   (FP),Y
   LDY   #$65
   LDA   (FP),Y
   LDY   #$69
   AND   (FP),Y
   LDY   #$6D
   STA   (FP),Y
   LDY   #$60
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$6A
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$6B
   LDA   (FP),Y
   LDY   #$01
   STA   (DPTR),Y
   LDY   #$6C
   LDA   (FP),Y
   LDY   #$02
   STA   (DPTR),Y
   LDY   #$6D
   LDA   (FP),Y
   LDY   #$03
   STA   (DPTR),Y
   LDY   #$6A
   LDA   (FP),Y
   LDY   #$6E
   STA   (FP),Y
   LDY   #$6B
   LDA   (FP),Y
   LDY   #$6F
   STA   (FP),Y
   LDY   #$6C
   LDA   (FP),Y
   LDY   #$70
   STA   (FP),Y
   LDY   #$6D
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
   LDY   #$6E
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$6F
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$70
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$71
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   JSR   mix
.if_end@1:
   LDY   #$0C
   LDA   (FP),Y
   LDY   #$72
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@41
   LDA   #$00
   JMP   .sx_done@42
.sx_neg@41:
   LDA   #$FF
.sx_done@42:
   LDY   #$73
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   EOR   #$81
   LDY   #$74
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$75
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$76
   STA   (FP),Y
   LDY   #$0C
   STA   (FP),Y
   LDY   #$77
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@43
   LDA   #$00
   JMP   .sx_done@44
.sx_neg@43:
   LDA   #$FF
.sx_done@44:
   LDY   #$78
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$79
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$7A
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@45
   LDA   #$00
   JMP   .sx_done@46
.sx_neg@45:
   LDA   #$FF
.sx_done@46:
   LDY   #$7B
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   gp11
   STA   DPTR
   LDA   gp11+1
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$7D
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$7E
   STA   (FP),Y
   LDY   #$02
   LDA   (DPTR),Y
   LDY   #$7F
   STA   (FP),Y
   LDY   #$03
   LDA   (DPTR),Y
   LDY   #$80
   STA   (FP),Y
   LDY   #$79
   LDA   (FP),Y
   SEC
   LDY   #$7D
   SBC   (FP),Y
   LDY   #$81
   STA   (FP),Y
   LDY   #$7A
   LDA   (FP),Y
   LDY   #$7E
   SBC   (FP),Y
   LDY   #$82
   STA   (FP),Y
   LDY   #$7B
   LDA   (FP),Y
   LDY   #$7F
   SBC   (FP),Y
   LDY   #$83
   STA   (FP),Y
   LDY   #$7C
   LDA   (FP),Y
   LDY   #$80
   SBC   (FP),Y
   LDY   #$84
   STA   (FP),Y
   LDY   #$81
   LDA   (FP),Y
   LDY   #$07
   STA   (FP),Y
   LDY   #$82
   LDA   (FP),Y
   LDY   #$08
   STA   (FP),Y
   LDY   #$83
   LDA   (FP),Y
   LDY   #$09
   STA   (FP),Y
   LDY   #$84
   LDA   (FP),Y
   LDY   #$0A
   STA   (FP),Y
   LDY   #$07
   LDA   (FP),Y
   LDY   #$85
   STA   (FP),Y
   LDY   #$08
   LDA   (FP),Y
   LDY   #$86
   STA   (FP),Y
   LDY   #$09
   LDA   (FP),Y
   LDY   #$87
   STA   (FP),Y
   LDY   #$0A
   LDA   (FP),Y
   LDY   #$88
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@47
   LDA   #$00
   JMP   .sx_done@48
.sx_neg@47:
   LDA   #$FF
.sx_done@48:
   LDY   #$89
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$85
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
   BEQ   .if_end@3
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
.loop@8_continue:
   LDY   #$8E
   LDA   (FP),Y
   INY
   STA   (FP),Y
   CLC
   ADC   #$01
   INY
   STA   (FP),Y
   LDY   #$8E
   STA   (FP),Y
   INY
   LDA   (FP),Y
   LDY   #$91
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
   BVC   .cmp_novf@49
   EOR   #$80
.cmp_novf@49:
   ASL
   LDA   #$00
   ROL
   LDY   #$93
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .loop@8_break
   LDY   #$8E
   LDA   (FP),Y
   LDY   #$95
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .loop@8_continue
.loop@8_break:
.if_end@3:
   JSR   fin0
   JSR   fin1
   JSR   fin2
   JSR   fin3
   LDA   acc
   LDY   #$B6
   STA   (FP),Y
   LDA   acc+1
   AND   #$7F
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$B8
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$B9
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
   ADC   #$BB
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$BA
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
   DC.B  $03
   DS.B  6

g2:
   DS.B  4

g3:
   DC.B  $FF

g4:
   DC.B  $03

ga8:
   DC.L  $80000000
   DC.L  $0000007F
   DC.L  $00010000
   DC.L  $F06A2798

gs9:
   DS.B  1
   DC.B  $0C
   DS.B  6
   DC.B  $FF

gp10:
   DC.W  z5

gp11:
   DC.W  g2

__zp_init:
   SUBROUTINE

   LDA   #$00
   STA   z5
   STA   z5+2
   STA   z5+3
   STA   z5+4
   STA   z5+5
   STA   z5+6
   STA   z5+7
   STA   z6+2
   STA   z6+3
   STA   z6+4
   STA   z6+5
   STA   z6+6
   STA   z7+1
   STA   z7+2
   STA   z7+3
   STA   z7+4
   STA   z7+5
   STA   z7+6
   STA   z7+7
   LDA   #$01
   STA   z5+1
   LDA   #$FF
   STA   z6
   LDA   #$7F
   STA   z6+1
   LDA   #$03
   STA   z7
   RTS
