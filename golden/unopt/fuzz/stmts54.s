SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
z5	EQU	$26
z6	EQU	$27

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
   LDA   g3
   LDY   #$09
   STA   (FP),Y
   LDA   g3+1
   INY
   STA   (FP),Y
   LDA   g3+1
   BMI   .sx_neg@0
   LDA   #$00
   JMP   .sx_done@1
.sx_neg@0:
   LDA   #$FF
.sx_done@1:
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
   LDA   g4
   LDY   #$0D
   STA   (FP),Y
   LDA   g4+1
   INY
   STA   (FP),Y
   LDA   g4+2
   INY
   STA   (FP),Y
   LDA   g4+3
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

   ; prologue: 0 arg bytes, 33 local bytes
   SEC
   LDA   SSP
   SBC   #$23
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$22
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
   INY
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@2
   LDA   #$00
   JMP   .sx_done@3
.sx_neg@2:
   LDA   #$FF
.sx_done@3:
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
   LDA   z6
   LDY   #$06
   STA   (FP),Y
   LDA   z6+1
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
   LDA   ga7
   LDY   #$0A
   STA   (FP),Y
   LDA   ga7+1
   INY
   STA   (FP),Y
   LDA   ga7+2
   INY
   STA   (FP),Y
   LDA   ga7+3
   INY
   STA   (FP),Y
   LDA   ga7+4
   INY
   STA   (FP),Y
   LDA   ga7+5
   INY
   STA   (FP),Y
   LDA   ga7+6
   INY
   STA   (FP),Y
   LDA   ga7+7
   INY
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
   LDA   ga7+8
   LDY   #$16
   STA   (FP),Y
   LDA   ga7+9
   INY
   STA   (FP),Y
   LDA   ga7+10
   INY
   STA   (FP),Y
   LDA   ga7+11
   INY
   STA   (FP),Y
   LDA   ga7+12
   INY
   STA   (FP),Y
   LDA   ga7+13
   INY
   STA   (FP),Y
   LDA   ga7+14
   INY
   STA   (FP),Y
   LDA   ga7+15
   INY
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
   LDY   #$20
   STA   (FP),Y
   LDY   #$19
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

   ; epilogue
   CLC
   LDA   FP
   ADC   #$23
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$22
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

   ; prologue: 0 arg bytes, 49 local bytes
   SEC
   LDA   SSP
   SBC   #$33
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$32
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   LDA   ga7+16
   LDY   #$01
   STA   (FP),Y
   LDA   ga7+17
   INY
   STA   (FP),Y
   LDA   ga7+18
   INY
   STA   (FP),Y
   LDA   ga7+19
   INY
   STA   (FP),Y
   LDA   ga7+20
   INY
   STA   (FP),Y
   LDA   ga7+21
   INY
   STA   (FP),Y
   LDA   ga7+22
   INY
   STA   (FP),Y
   LDA   ga7+23
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
   LDA   ga7+24
   LDY   #$0D
   STA   (FP),Y
   LDA   ga7+25
   INY
   STA   (FP),Y
   LDA   ga7+26
   INY
   STA   (FP),Y
   LDA   ga7+27
   INY
   STA   (FP),Y
   LDA   ga7+28
   INY
   STA   (FP),Y
   LDA   ga7+29
   INY
   STA   (FP),Y
   LDA   ga7+30
   INY
   STA   (FP),Y
   LDA   ga7+31
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
   LDA   #<gs8
   LDY   #$19
   STA   (FP),Y
   LDA   #>gs8
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
   LDY   #$07
   LDA   (DPTR),Y
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
   LDY   #$28
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$04
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDY   #$25
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$26
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$27
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$28
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   JSR   mix
   LDA   #<gs8
   LDY   #$29
   STA   (FP),Y
   LDA   #>gs8
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$09
   LDY   #$2B
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
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

   ; epilogue
   CLC
   LDA   FP
   ADC   #$33
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$32
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

   ; prologue: 2 arg bytes, 112 local bytes
   SEC
   LDA   SSP
   SBC   #$72
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$71
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   LDA   #$0A
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
   LDA   z5
   INY
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
   LDY   #$0D
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LSR
   LSR
   LSR
   LSR
   LSR
   LDY   #$0E
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ASL
   ASL
   ASL
   INY
   ORA   (FP),Y
   STA   (FP),Y
   LDY   #$10
   STA   (FP),Y
   STA   z5
   LDA   #<gs8
   INY
   STA   (FP),Y
   LDA   #>gs8
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$01
   LDY   #$13
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$14
   STA   (FP),Y
   DEY
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
   LDY   #$02
   LDA   (DPTR),Y
   LDY   #$17
   STA   (FP),Y
   LDY   #$03
   LDA   (DPTR),Y
   LDY   #$18
   STA   (FP),Y
   LDY   #$04
   LDA   (DPTR),Y
   LDY   #$19
   STA   (FP),Y
   LDY   #$05
   LDA   (DPTR),Y
   LDY   #$1A
   STA   (FP),Y
   LDY   #$06
   LDA   (DPTR),Y
   LDY   #$1B
   STA   (FP),Y
   LDY   #$07
   LDA   (DPTR),Y
   LDY   #$1C
   STA   (FP),Y
   LDA   #$FF
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   ORA   #$00
   BPL   .sx_done@7
   LDA   #$FF
.sx_done@7:
   LDY   #$1F
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
   LDY   #$15
   LDA   (FP),Y
   LDY   #$1D
   EOR   (FP),Y
   LDY   #$25
   STA   (FP),Y
   LDY   #$16
   LDA   (FP),Y
   LDY   #$1E
   EOR   (FP),Y
   LDY   #$26
   STA   (FP),Y
   LDY   #$17
   LDA   (FP),Y
   LDY   #$1F
   EOR   (FP),Y
   LDY   #$27
   STA   (FP),Y
   LDY   #$18
   LDA   (FP),Y
   LDY   #$20
   EOR   (FP),Y
   LDY   #$28
   STA   (FP),Y
   LDY   #$19
   LDA   (FP),Y
   LDY   #$21
   EOR   (FP),Y
   LDY   #$29
   STA   (FP),Y
   LDY   #$1A
   LDA   (FP),Y
   LDY   #$22
   EOR   (FP),Y
   LDY   #$2A
   STA   (FP),Y
   LDY   #$1B
   LDA   (FP),Y
   LDY   #$23
   EOR   (FP),Y
   LDY   #$2B
   STA   (FP),Y
   LDY   #$1C
   LDA   (FP),Y
   LDY   #$24
   EOR   (FP),Y
   LDY   #$2C
   STA   (FP),Y
   LDY   #$13
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$25
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$26
   LDA   (FP),Y
   LDY   #$01
   STA   (DPTR),Y
   LDY   #$27
   LDA   (FP),Y
   LDY   #$02
   STA   (DPTR),Y
   LDY   #$28
   LDA   (FP),Y
   LDY   #$03
   STA   (DPTR),Y
   LDY   #$29
   LDA   (FP),Y
   LDY   #$04
   STA   (DPTR),Y
   LDY   #$2A
   LDA   (FP),Y
   LDY   #$05
   STA   (DPTR),Y
   LDY   #$2B
   LDA   (FP),Y
   LDY   #$06
   STA   (DPTR),Y
   LDY   #$2C
   LDA   (FP),Y
   LDY   #$07
   STA   (DPTR),Y
   LDA   #<ga7
   LDY   #$2D
   STA   (FP),Y
   LDA   #>ga7
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
   LDY   #$31
   STA   (FP),Y
   STA   HARGS
   LDA   #$08
   STA   HARGS+1
   JSR   mul8
   LDA   HARGS+2
   LDY   #$32
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$2D
   LDA   (FP),Y
   CLC
   LDY   #$33
   ADC   (FP),Y
   LDY   #$35
   STA   (FP),Y
   LDY   #$2E
   LDA   (FP),Y
   LDY   #$34
   ADC   (FP),Y
   LDY   #$36
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$37
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$38
   STA   (FP),Y
   LDY   #$02
   LDA   (DPTR),Y
   LDY   #$39
   STA   (FP),Y
   LDY   #$03
   LDA   (DPTR),Y
   LDY   #$3A
   STA   (FP),Y
   LDY   #$04
   LDA   (DPTR),Y
   LDY   #$3B
   STA   (FP),Y
   LDY   #$05
   LDA   (DPTR),Y
   LDY   #$3C
   STA   (FP),Y
   LDY   #$06
   LDA   (DPTR),Y
   LDY   #$3D
   STA   (FP),Y
   LDY   #$07
   LDA   (DPTR),Y
   LDY   #$3E
   STA   (FP),Y
   LDA   #$FF
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   ORA   #$00
   BPL   .sx_done@9
   LDA   #$FF
.sx_done@9:
   LDY   #$41
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
   INY
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
   LDY   #$47
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
   LDY   #$35
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$47
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$48
   LDA   (FP),Y
   LDY   #$01
   STA   (DPTR),Y
   LDY   #$49
   LDA   (FP),Y
   LDY   #$02
   STA   (DPTR),Y
   LDY   #$4A
   LDA   (FP),Y
   LDY   #$03
   STA   (DPTR),Y
   LDY   #$4B
   LDA   (FP),Y
   LDY   #$04
   STA   (DPTR),Y
   LDY   #$4C
   LDA   (FP),Y
   LDY   #$05
   STA   (DPTR),Y
   LDY   #$4D
   LDA   (FP),Y
   LDY   #$06
   STA   (DPTR),Y
   LDY   #$4E
   LDA   (FP),Y
   LDY   #$07
   STA   (DPTR),Y
   LDA   #$FF
   LDY   #$4F
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
   LDY   #$51
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$52
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$53
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@12
   LDA   #$00
   JMP   .sx_done@13
.sx_neg@12:
   LDA   #$FF
.sx_done@13:
   LDY   #$54
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
   LDY   #$47
   LDA   (FP),Y
   LDY   #$52
   EOR   (FP),Y
   LDY   #$5A
   STA   (FP),Y
   LDY   #$48
   LDA   (FP),Y
   LDY   #$53
   EOR   (FP),Y
   LDY   #$5B
   STA   (FP),Y
   LDY   #$49
   LDA   (FP),Y
   LDY   #$54
   EOR   (FP),Y
   LDY   #$5C
   STA   (FP),Y
   LDY   #$4A
   LDA   (FP),Y
   LDY   #$55
   EOR   (FP),Y
   LDY   #$5D
   STA   (FP),Y
   LDY   #$4B
   LDA   (FP),Y
   LDY   #$56
   EOR   (FP),Y
   LDY   #$5E
   STA   (FP),Y
   LDY   #$4C
   LDA   (FP),Y
   LDY   #$57
   EOR   (FP),Y
   LDY   #$5F
   STA   (FP),Y
   LDY   #$4D
   LDA   (FP),Y
   LDY   #$58
   EOR   (FP),Y
   LDY   #$60
   STA   (FP),Y
   LDY   #$4E
   LDA   (FP),Y
   LDY   #$59
   EOR   (FP),Y
   LDY   #$61
   STA   (FP),Y
   LDY   #$5A
   LDA   (FP),Y
   LDY   #$62
   STA   (FP),Y
   LDY   #$5B
   LDA   (FP),Y
   LDY   #$63
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$05
   STA   (FP),Y
   LDY   #$63
   LDA   (FP),Y
   LDY   #$06
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   LDY   #$64
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$66
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$67
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$68
   STA   (FP),Y
   LDY   #$02
   STA   (FP),Y
   LDA   z5
   LDY   #$69
   STA   (FP),Y
   INY
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@14
   LDA   #$00
   JMP   .sx_done@15
.sx_neg@14:
   LDA   #$FF
.sx_done@15:
   LDY   #$6B
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
   DEY
   LDA   (FP),Y
   LDY   #$70
   STA   (FP),Y

   ; epilogue
   PHA
   CLC
   LDA   FP
   ADC   #$74
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

f14:
   SUBROUTINE

   ; prologue: 8 arg bytes, 174 local bytes
   SEC
   LDA   SSP
   SBC   #$B0
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$AF
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
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$05
   LDA   (FP),Y
   LDY   #$0D
   STA   (FP),Y
   LDY   #$06
   LDA   (FP),Y
   LDY   #$0E
   STA   (FP),Y
   LDA   z5
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@16
   LDA   #$00
   JMP   .sx_done@17
.sx_neg@16:
   LDA   #$FF
.sx_done@17:
   LDY   #$11
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   EOR   #$01
   LDY   #$12
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$13
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$14
   STA   (FP),Y
   STA   z5
   LDA   (FP),Y
   INY
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@18
   LDA   #$00
   JMP   .sx_done@19
.sx_neg@18:
   LDA   #$FF
.sx_done@19:
   LDY   #$16
   STA   (FP),Y
   LDY   #$0D
   LDA   (FP),Y
   LDY   #$15
   EOR   (FP),Y
   LDY   #$17
   STA   (FP),Y
   LDY   #$0E
   LDA   (FP),Y
   LDY   #$16
   EOR   (FP),Y
   LDY   #$18
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$19
   STA   (FP),Y
   LDA   #<gs8
   INY
   STA   (FP),Y
   LDA   #>gs8
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$09
   LDY   #$1C
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$1D
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$19
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDA   #$00
   LDY   #$1E
   STA   (FP),Y
   INY
   STA   (FP),Y
.loop@0_continue:
   LDY   #$1F
   LDA   (FP),Y
   INY
   STA   (FP),Y
   CLC
   ADC   #$01
   INY
   STA   (FP),Y
   LDY   #$1F
   STA   (FP),Y
   INY
   LDA   (FP),Y
   LDY   #$22
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
   BVC   .cmp_novf@20
   EOR   #$80
.cmp_novf@20:
   ASL
   LDA   #$00
   ROL
   LDY   #$24
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
   LDA   #$FF
   LDY   #$26
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   ORA   #$00
   BPL   .sx_done@22
   LDA   #$FF
.sx_done@22:
   LDY   #$28
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$B3
   LDA   (FP),Y
   LDY   #$26
   AND   (FP),Y
   LDY   #$2A
   STA   (FP),Y
   LDY   #$B4
   LDA   (FP),Y
   LDY   #$27
   AND   (FP),Y
   LDY   #$2B
   STA   (FP),Y
   LDY   #$B5
   LDA   (FP),Y
   LDY   #$28
   AND   (FP),Y
   LDY   #$2C
   STA   (FP),Y
   LDY   #$B6
   LDA   (FP),Y
   LDY   #$29
   AND   (FP),Y
   LDY   #$2D
   STA   (FP),Y
   LDY   #$2A
   LDA   (FP),Y
   LDY   #$B3
   STA   (FP),Y
   LDY   #$2B
   LDA   (FP),Y
   LDY   #$B4
   STA   (FP),Y
   LDY   #$2C
   LDA   (FP),Y
   LDY   #$B5
   STA   (FP),Y
   LDY   #$2D
   LDA   (FP),Y
   LDY   #$B6
   STA   (FP),Y
   LDY   #$05
   LDA   (FP),Y
   LDY   #$2A
   AND   (FP),Y
   LDY   #$2E
   STA   (FP),Y
   LDY   #$06
   LDA   (FP),Y
   LDY   #$2B
   AND   (FP),Y
   LDY   #$2F
   STA   (FP),Y
   LDY   #$07
   LDA   (FP),Y
   LDY   #$2C
   AND   (FP),Y
   LDY   #$30
   STA   (FP),Y
   LDY   #$08
   LDA   (FP),Y
   LDY   #$2D
   AND   (FP),Y
   LDY   #$31
   STA   (FP),Y
   LDY   #$2E
   LDA   (FP),Y
   LDY   #$32
   STA   (FP),Y
   LDY   #$2F
   LDA   (FP),Y
   LDY   #$33
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   g3
   INY
   LDA   (FP),Y
   STA   g3+1
   LDA   #<ga7
   INY
   STA   (FP),Y
   LDA   #>ga7
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
   LDY   #$38
   STA   (FP),Y
   STA   HARGS
   LDA   #$08
   STA   HARGS+1
   JSR   mul8
   LDA   HARGS+2
   LDY   #$39
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$34
   LDA   (FP),Y
   CLC
   LDY   #$3A
   ADC   (FP),Y
   LDY   #$3C
   STA   (FP),Y
   LDY   #$35
   LDA   (FP),Y
   LDY   #$3B
   ADC   (FP),Y
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
   LDY   #$04
   LDA   (DPTR),Y
   LDY   #$42
   STA   (FP),Y
   LDY   #$05
   LDA   (DPTR),Y
   LDY   #$43
   STA   (FP),Y
   LDY   #$06
   LDA   (DPTR),Y
   LDY   #$44
   STA   (FP),Y
   LDY   #$07
   LDA   (DPTR),Y
   LDY   #$45
   STA   (FP),Y
   LDA   #$5A
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   ORA   #$00
   BPL   .sx_done@24
   LDA   #$FF
.sx_done@24:
   LDY   #$48
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
   LDY   #$3E
   LDA   (FP),Y
   LDY   #$46
   AND   (FP),Y
   LDY   #$4E
   STA   (FP),Y
   LDY   #$3F
   LDA   (FP),Y
   LDY   #$47
   AND   (FP),Y
   LDY   #$4F
   STA   (FP),Y
   LDY   #$40
   LDA   (FP),Y
   LDY   #$48
   AND   (FP),Y
   LDY   #$50
   STA   (FP),Y
   LDY   #$41
   LDA   (FP),Y
   LDY   #$49
   AND   (FP),Y
   LDY   #$51
   STA   (FP),Y
   LDY   #$42
   LDA   (FP),Y
   LDY   #$4A
   AND   (FP),Y
   LDY   #$52
   STA   (FP),Y
   LDY   #$43
   LDA   (FP),Y
   LDY   #$4B
   AND   (FP),Y
   LDY   #$53
   STA   (FP),Y
   LDY   #$44
   LDA   (FP),Y
   LDY   #$4C
   AND   (FP),Y
   LDY   #$54
   STA   (FP),Y
   LDY   #$45
   LDA   (FP),Y
   LDY   #$4D
   AND   (FP),Y
   LDY   #$55
   STA   (FP),Y
   LDY   #$3C
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
   LDY   #$52
   LDA   (FP),Y
   LDY   #$04
   STA   (DPTR),Y
   LDY   #$53
   LDA   (FP),Y
   LDY   #$05
   STA   (DPTR),Y
   LDY   #$54
   LDA   (FP),Y
   LDY   #$06
   STA   (DPTR),Y
   LDY   #$55
   LDA   (FP),Y
   LDY   #$07
   STA   (DPTR),Y
   LDY   #$4E
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
   BNE   .lb_skip@1
   JMP   .loop@0_continue
.lb_skip@1:
   LDY   #$B1
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .cond_else@2
   LDY   #$1F
   LDA   (FP),Y
   LDY   #$56
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$58
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$59
   STA   (FP),Y
   JMP   .cond_end@3
.cond_else@2:
   LDA   #$00
   LDY   #$5A
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$58
   STA   (FP),Y
   INY
   STA   (FP),Y
.cond_end@3:
   LDY   #$58
   LDA   (FP),Y
   LDY   #$5D
   STA   (FP),Y
   LDY   #$B7
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$5D
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   JMP   .loop@0_continue
.loop@0_break:
   LDA   g1
   LDY   #$81
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDA   #$7F
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$81
   LDA   (FP),Y
   CLC
   LDY   #$83
   ADC   (FP),Y
   LDY   #$85
   STA   (FP),Y
   LDY   #$82
   LDA   (FP),Y
   LDY   #$84
   ADC   (FP),Y
   LDY   #$86
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@0
   JMP   .if_end@4
.lb_skip@0:
   LDY   #$B7
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$87
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
   LDY   #$89
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$8A
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$8B
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   SEC
   LDY   #$B1
   SBC   (FP),Y
   LDY   #$8C
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$B2
   SBC   (FP),Y
   LDY   #$8D
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$8E
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$8F
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
   LDY   #$8E
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$8F
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$90
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$91
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   JSR   mix
   LDA   #<gs8
   LDY   #$92
   STA   (FP),Y
   LDA   #>gs8
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$09
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
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   AND   #$01
   LDY   #$99
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDA   #$00
   LDY   #$9A
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$9B
   STA   (FP),Y
   LDY   #$94
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$9B
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
.if_end@4:
   LDA   #$01
   LDY   #$9C
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   ORA   #$00
   BPL   .sx_done@28
   LDA   #$FF
.sx_done@28:
   LDY   #$9E
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
   LDA   g4
   LDY   #$9C
   AND   (FP),Y
   LDY   #$A4
   STA   (FP),Y
   LDA   g4+1
   LDY   #$9D
   AND   (FP),Y
   LDY   #$A5
   STA   (FP),Y
   LDA   g4+2
   LDY   #$9E
   AND   (FP),Y
   LDY   #$A6
   STA   (FP),Y
   LDA   g4+3
   LDY   #$9F
   AND   (FP),Y
   LDY   #$A7
   STA   (FP),Y
   LDA   g4+4
   LDY   #$A0
   AND   (FP),Y
   LDY   #$A8
   STA   (FP),Y
   LDA   g4+5
   LDY   #$A1
   AND   (FP),Y
   LDY   #$A9
   STA   (FP),Y
   LDA   g4+6
   LDY   #$A2
   AND   (FP),Y
   LDY   #$AA
   STA   (FP),Y
   LDA   g4+7
   LDY   #$A3
   AND   (FP),Y
   LDY   #$AB
   STA   (FP),Y
   LDY   #$A4
   LDA   (FP),Y
   STA   g4
   INY
   LDA   (FP),Y
   STA   g4+1
   INY
   LDA   (FP),Y
   STA   g4+2
   INY
   LDA   (FP),Y
   STA   g4+3
   INY
   LDA   (FP),Y
   STA   g4+4
   INY
   LDA   (FP),Y
   STA   g4+5
   INY
   LDA   (FP),Y
   STA   g4+6
   INY
   LDA   (FP),Y
   STA   g4+7
   LDA   #$7F
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$AE
   STA   (FP),Y

   ; epilogue
   PHA
   CLC
   LDA   FP
   ADC   #$B8
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

   ; prologue: 0 arg bytes, 167 local bytes
   SEC
   LDA   SSP
   SBC   #$A9
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$A8
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   LDA   #$03
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
   LDA   #$05
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDA   #$01
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
   LDA   #$FF
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   g3
   INY
   STA   (FP),Y
   LDA   g3+1
   INY
   STA   (FP),Y
   LDA   g3
   SEC
   SBC   #$01
   INY
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
   LDA   g2
   INY
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
   LDA   #$5A
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   ORA   #$00
   BPL   .sx_done@30
   LDA   #$FF
.sx_done@30:
   LDY   #$23
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$1D
   LDA   (FP),Y
   LDY   #$21
   ORA   (FP),Y
   LDY   #$25
   STA   (FP),Y
   LDY   #$1E
   LDA   (FP),Y
   LDY   #$22
   ORA   (FP),Y
   LDY   #$26
   STA   (FP),Y
   LDY   #$1F
   LDA   (FP),Y
   LDY   #$23
   ORA   (FP),Y
   LDY   #$27
   STA   (FP),Y
   LDY   #$20
   LDA   (FP),Y
   LDY   #$24
   ORA   (FP),Y
   LDY   #$28
   STA   (FP),Y
   LDY   #$25
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
   LDA   #<gs8
   INY
   STA   (FP),Y
   LDA   #>gs8
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$01
   LDY   #$2B
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
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
   LDY   #$02
   LDA   (DPTR),Y
   LDY   #$2F
   STA   (FP),Y
   LDY   #$03
   LDA   (DPTR),Y
   LDY   #$30
   STA   (FP),Y
   LDY   #$04
   LDA   (DPTR),Y
   LDY   #$31
   STA   (FP),Y
   LDY   #$05
   LDA   (DPTR),Y
   LDY   #$32
   STA   (FP),Y
   LDY   #$06
   LDA   (DPTR),Y
   LDY   #$33
   STA   (FP),Y
   LDY   #$07
   LDA   (DPTR),Y
   LDY   #$34
   STA   (FP),Y
   LDY   #$2D
   LDA   (FP),Y
   SEC
   SBC   #$01
   LDY   #$35
   STA   (FP),Y
   LDY   #$2E
   LDA   (FP),Y
   SBC   #$00
   LDY   #$36
   STA   (FP),Y
   LDY   #$2F
   LDA   (FP),Y
   SBC   #$00
   LDY   #$37
   STA   (FP),Y
   LDY   #$30
   LDA   (FP),Y
   SBC   #$00
   LDY   #$38
   STA   (FP),Y
   LDY   #$31
   LDA   (FP),Y
   SBC   #$00
   LDY   #$39
   STA   (FP),Y
   LDY   #$32
   LDA   (FP),Y
   SBC   #$00
   LDY   #$3A
   STA   (FP),Y
   LDY   #$33
   LDA   (FP),Y
   SBC   #$00
   LDY   #$3B
   STA   (FP),Y
   LDY   #$34
   LDA   (FP),Y
   SBC   #$00
   LDY   #$3C
   STA   (FP),Y
   LDY   #$2B
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$35
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$36
   LDA   (FP),Y
   LDY   #$01
   STA   (DPTR),Y
   LDY   #$37
   LDA   (FP),Y
   LDY   #$02
   STA   (DPTR),Y
   LDY   #$38
   LDA   (FP),Y
   LDY   #$03
   STA   (DPTR),Y
   LDY   #$39
   LDA   (FP),Y
   LDY   #$04
   STA   (DPTR),Y
   LDY   #$3A
   LDA   (FP),Y
   LDY   #$05
   STA   (DPTR),Y
   LDY   #$3B
   LDA   (FP),Y
   LDY   #$06
   STA   (DPTR),Y
   LDY   #$3C
   LDA   (FP),Y
   LDY   #$07
   STA   (DPTR),Y
   LDY   #$35
   LDA   (FP),Y
   LDY   #$3D
   STA   (FP),Y
   LDY   #$12
   STA   (FP),Y
   LDY   #$18
   LDA   (FP),Y
   LDY   #$3E
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@31
   LDA   #$00
   JMP   .sx_done@32
.sx_neg@31:
   LDA   #$FF
.sx_done@32:
   LDY   #$3F
   STA   (FP),Y
   LDA   z5
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@33
   LDA   #$00
   JMP   .sx_done@34
.sx_neg@33:
   LDA   #$FF
.sx_done@34:
   LDY   #$42
   STA   (FP),Y
   LDY   #$3E
   LDA   (FP),Y
   CLC
   LDY   #$41
   ADC   (FP),Y
   LDY   #$43
   STA   (FP),Y
   LDY   #$3F
   LDA   (FP),Y
   LDY   #$42
   ADC   (FP),Y
   LDY   #$44
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@4
   JMP   .if_end@5
.lb_skip@4:
   LDA   #<ga7
   LDY   #$45
   STA   (FP),Y
   LDA   #>ga7
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
   LDY   #$49
   STA   (FP),Y
   STA   HARGS
   LDA   #$08
   STA   HARGS+1
   JSR   mul8
   LDA   HARGS+2
   LDY   #$4A
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$45
   LDA   (FP),Y
   CLC
   LDY   #$4B
   ADC   (FP),Y
   LDY   #$4D
   STA   (FP),Y
   LDY   #$46
   LDA   (FP),Y
   LDY   #$4C
   ADC   (FP),Y
   LDY   #$4E
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$4F
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$50
   STA   (FP),Y
   LDY   #$02
   LDA   (DPTR),Y
   LDY   #$51
   STA   (FP),Y
   LDY   #$03
   LDA   (DPTR),Y
   LDY   #$52
   STA   (FP),Y
   LDY   #$04
   LDA   (DPTR),Y
   LDY   #$53
   STA   (FP),Y
   LDY   #$05
   LDA   (DPTR),Y
   LDY   #$54
   STA   (FP),Y
   LDY   #$06
   LDA   (DPTR),Y
   LDY   #$55
   STA   (FP),Y
   LDY   #$07
   LDA   (DPTR),Y
   LDY   #$56
   STA   (FP),Y
   LDA   #$05
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   ORA   #$00
   BPL   .sx_done@36
   LDA   #$FF
.sx_done@36:
   LDY   #$59
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
   LDY   #$4F
   LDA   (FP),Y
   SEC
   LDY   #$57
   SBC   (FP),Y
   LDY   #$5F
   STA   (FP),Y
   LDY   #$50
   LDA   (FP),Y
   LDY   #$58
   SBC   (FP),Y
   LDY   #$60
   STA   (FP),Y
   LDY   #$51
   LDA   (FP),Y
   LDY   #$59
   SBC   (FP),Y
   LDY   #$61
   STA   (FP),Y
   LDY   #$52
   LDA   (FP),Y
   LDY   #$5A
   SBC   (FP),Y
   LDY   #$62
   STA   (FP),Y
   LDY   #$53
   LDA   (FP),Y
   LDY   #$5B
   SBC   (FP),Y
   LDY   #$63
   STA   (FP),Y
   LDY   #$54
   LDA   (FP),Y
   LDY   #$5C
   SBC   (FP),Y
   LDY   #$64
   STA   (FP),Y
   LDY   #$55
   LDA   (FP),Y
   LDY   #$5D
   SBC   (FP),Y
   LDY   #$65
   STA   (FP),Y
   LDY   #$56
   LDA   (FP),Y
   LDY   #$5E
   SBC   (FP),Y
   LDY   #$66
   STA   (FP),Y
   LDY   #$4D
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$5F
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$60
   LDA   (FP),Y
   LDY   #$01
   STA   (DPTR),Y
   LDY   #$61
   LDA   (FP),Y
   LDY   #$02
   STA   (DPTR),Y
   LDY   #$62
   LDA   (FP),Y
   LDY   #$03
   STA   (DPTR),Y
   LDY   #$63
   LDA   (FP),Y
   LDY   #$04
   STA   (DPTR),Y
   LDY   #$64
   LDA   (FP),Y
   LDY   #$05
   STA   (DPTR),Y
   LDY   #$65
   LDA   (FP),Y
   LDY   #$06
   STA   (DPTR),Y
   LDY   #$66
   LDA   (FP),Y
   LDY   #$07
   STA   (DPTR),Y
   LDA   ga7+8
   LDY   #$67
   STA   (FP),Y
   LDA   ga7+9
   INY
   STA   (FP),Y
   LDA   ga7+10
   INY
   STA   (FP),Y
   LDA   ga7+11
   INY
   STA   (FP),Y
   LDA   ga7+12
   INY
   STA   (FP),Y
   LDA   ga7+13
   INY
   STA   (FP),Y
   LDA   ga7+14
   INY
   STA   (FP),Y
   LDA   ga7+15
   INY
   STA   (FP),Y
   LDY   #$67
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
   LDA   #$03
   STA   HARGS+8
   JSR   asl64
   LDA   HARGS+9
   LDY   #$6F
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
   LDY   #$6F
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
   BNE   .lb_skip@3
   JMP   .if_end@6
.lb_skip@3:
   LDA   g2
   LDY   #$77
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
   LDY   #$77
   LDA   (FP),Y
   LDY   #$7B
   STA   (FP),Y
   LDY   #$78
   LDA   (FP),Y
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
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$7B
   LDA   (FP),Y
   SEC
   SBC   g4
   LDY   #$83
   STA   (FP),Y
   LDY   #$7C
   LDA   (FP),Y
   SBC   g4+1
   LDY   #$84
   STA   (FP),Y
   LDY   #$7D
   LDA   (FP),Y
   SBC   g4+2
   LDY   #$85
   STA   (FP),Y
   LDY   #$7E
   LDA   (FP),Y
   SBC   g4+3
   LDY   #$86
   STA   (FP),Y
   LDY   #$7F
   LDA   (FP),Y
   SBC   g4+4
   LDY   #$87
   STA   (FP),Y
   LDY   #$80
   LDA   (FP),Y
   SBC   g4+5
   LDY   #$88
   STA   (FP),Y
   LDY   #$81
   LDA   (FP),Y
   SBC   g4+6
   LDY   #$89
   STA   (FP),Y
   LDY   #$82
   LDA   (FP),Y
   SBC   g4+7
   LDY   #$8A
   STA   (FP),Y
   LDA   #<gs8
   INY
   STA   (FP),Y
   LDA   #>gs8
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$01
   LDY   #$8D
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$8E
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$83
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$84
   LDA   (FP),Y
   LDY   #$01
   STA   (DPTR),Y
   LDY   #$85
   LDA   (FP),Y
   LDY   #$02
   STA   (DPTR),Y
   LDY   #$86
   LDA   (FP),Y
   LDY   #$03
   STA   (DPTR),Y
   LDY   #$87
   LDA   (FP),Y
   LDY   #$04
   STA   (DPTR),Y
   LDY   #$88
   LDA   (FP),Y
   LDY   #$05
   STA   (DPTR),Y
   LDY   #$89
   LDA   (FP),Y
   LDY   #$06
   STA   (DPTR),Y
   LDY   #$8A
   LDA   (FP),Y
   LDY   #$07
   STA   (DPTR),Y
   LDA   g2
   LDY   #$8F
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
   LDA   #$5A
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   ORA   #$00
   BPL   .sx_done@38
   LDA   #$FF
.sx_done@38:
   LDY   #$95
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$8F
   LDA   (FP),Y
   LDY   #$93
   EOR   (FP),Y
   LDY   #$97
   STA   (FP),Y
   LDY   #$90
   LDA   (FP),Y
   LDY   #$94
   EOR   (FP),Y
   LDY   #$98
   STA   (FP),Y
   LDY   #$91
   LDA   (FP),Y
   LDY   #$95
   EOR   (FP),Y
   LDY   #$99
   STA   (FP),Y
   LDY   #$92
   LDA   (FP),Y
   LDY   #$96
   EOR   (FP),Y
   LDY   #$9A
   STA   (FP),Y
   LDY   #$97
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
   SEC
   LDA   SSP
   SBC   #$04
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDY   #$97
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$98
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$99
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$9A
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   JSR   mix
   LDA   z5
   LDY   #$9B
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
   LDY   #$9D
   STA   (FP),Y
   LDA   #$07
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$9C
   LDA   (FP),Y
   LDY   #$9E
   AND   (FP),Y
   LDY   #$A0
   STA   (FP),Y
   LDY   #$9D
   LDA   (FP),Y
   LDY   #$9F
   AND   (FP),Y
   LDY   #$A1
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CMP   #$01
   BEQ   .cmp_true@41
   LDA   #$00
   JMP   .cmp_end@42
.cmp_true@41:
   LDA   #$01
.cmp_end@42:
   LDY   #$A2
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
.if_end@6:
.if_end@5:
   JSR   fin0
   JSR   fin1
   JSR   fin2
   LDA   acc
   LDY   #$A4
   STA   (FP),Y
   LDA   acc+1
   AND   #$7F
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$A6
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$A7
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
   ADC   #$A9
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$A8
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
   DC.B  $FF

g2:
   DC.L  $00000080

g3:
   DC.W  $7FFF

g4:
   DC.L  $00008000
   DC.L  $00000000

ga7:
   DC.L  $00008000
   DC.L  $00000000
   DC.L  $7FFFFFFF
   DC.L  $00000000
   DC.L  $0000007F
   DC.L  $00000000
   DC.L  $0000FFFF
   DC.L  $00000000

gs8:
   DS.B  9
   DC.B  $FF

__zp_init:
   SUBROUTINE

   LDA   #$35
   STA   z5
   LDA   #$FF
   STA   z6
   LDA   #$7F
   STA   z6+1
   RTS
