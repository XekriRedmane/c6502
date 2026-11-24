SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
z5	EQU	$26
z6	EQU	$2C
z7	EQU	$30

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
   SEC
   LDA   SSP
   SBC   #$04
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   z6
   LDY   #$01
   STA   (SSP),Y
   LDA   z6+1
   INY
   STA   (SSP),Y
   LDA   z6+2
   INY
   STA   (SSP),Y
   LDA   z6+3
   INY
   STA   (SSP),Y
   JSR   mix
   LDA   z7
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
   LDA   ga8
   LDY   #$09
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
   LDA   ga8+4
   INY
   STA   (FP),Y
   LDA   ga8+5
   INY
   STA   (FP),Y
   LDY   #$09
   LDA   (FP),Y
   LDY   #$0F
   STA   (FP),Y
   LDY   #$0A
   LDA   (FP),Y
   LDY   #$10
   STA   (FP),Y
   LDY   #$0B
   LDA   (FP),Y
   LDY   #$11
   STA   (FP),Y
   LDY   #$0C
   LDA   (FP),Y
   LDY   #$12
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

   ; prologue: 0 arg bytes, 42 local bytes
   SEC
   LDA   SSP
   SBC   #$2C
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$2B
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   LDA   ga8+6
   LDY   #$01
   STA   (FP),Y
   LDA   ga8+7
   INY
   STA   (FP),Y
   LDA   ga8+8
   INY
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
   LDY   #$01
   LDA   (FP),Y
   LDY   #$07
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   LDY   #$08
   STA   (FP),Y
   LDY   #$03
   LDA   (FP),Y
   LDY   #$09
   STA   (FP),Y
   LDY   #$04
   LDA   (FP),Y
   LDY   #$0A
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
   LDA   ga8+12
   LDY   #$0B
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
   LDA   ga8+16
   INY
   STA   (FP),Y
   LDA   ga8+17
   INY
   STA   (FP),Y
   LDY   #$0B
   LDA   (FP),Y
   LDY   #$11
   STA   (FP),Y
   LDY   #$0C
   LDA   (FP),Y
   LDY   #$12
   STA   (FP),Y
   LDY   #$0D
   LDA   (FP),Y
   LDY   #$13
   STA   (FP),Y
   LDY   #$0E
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
   LDA   ga8+18
   LDY   #$15
   STA   (FP),Y
   LDA   ga8+19
   INY
   STA   (FP),Y
   LDA   ga8+20
   INY
   STA   (FP),Y
   LDA   ga8+21
   INY
   STA   (FP),Y
   LDA   ga8+22
   INY
   STA   (FP),Y
   LDA   ga8+23
   INY
   STA   (FP),Y
   LDY   #$15
   LDA   (FP),Y
   LDY   #$1B
   STA   (FP),Y
   LDY   #$16
   LDA   (FP),Y
   LDY   #$1C
   STA   (FP),Y
   LDY   #$17
   LDA   (FP),Y
   LDY   #$1D
   STA   (FP),Y
   LDY   #$18
   LDA   (FP),Y
   LDY   #$1E
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$04
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
   LDY   #$1D
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$1E
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   JSR   mix
   LDA   #<gs9
   LDY   #$1F
   STA   (FP),Y
   LDA   #>gs9
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$01
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
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$23
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$24
   STA   (FP),Y
   LDY   #$02
   LDA   (DPTR),Y
   LDY   #$25
   STA   (FP),Y
   LDY   #$03
   LDA   (DPTR),Y
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
   LDY   #$25
   LDA   (FP),Y
   LDY   #$29
   STA   (FP),Y
   LDY   #$26
   LDA   (FP),Y
   LDY   #$2A
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$04
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDY   #$27
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$28
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$29
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$2A
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   JSR   mix

   ; epilogue
   CLC
   LDA   FP
   ADC   #$2C
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$2B
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

   ; prologue: 0 arg bytes, 10 local bytes
   SEC
   LDA   SSP
   SBC   #$0C
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$0B
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

   ; epilogue
   CLC
   LDA   FP
   ADC   #$0C
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$0B
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   RTS

f11:
   SUBROUTINE

   ; prologue: 3 arg bytes, 102 local bytes
   SEC
   LDA   SSP
   SBC   #$68
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$67
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
   LDY   #$69
   LDA   (FP),Y
   LDY   #$09
   STA   (FP),Y
   LDY   #$6A
   LDA   (FP),Y
   LDY   #$0A
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@2
   LDA   #$00
   JMP   .sx_done@3
.sx_neg@2:
   LDA   #$FF
.sx_done@3:
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
   LDA   z6
   LDY   #$0D
   STA   (FP),Y
   LDA   z6+1
   INY
   STA   (FP),Y
   LDA   #<gs9
   INY
   STA   (FP),Y
   LDA   #>gs9
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$05
   LDY   #$11
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$12
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$0D
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$0E
   LDA   (FP),Y
   LDY   #$01
   STA   (DPTR),Y
   LDA   #$00
   LDY   #$13
   STA   (FP),Y
   INY
   STA   (FP),Y
.loop@0_start:
   LDA   @3.s15
   LDY   #$15
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$03
   LDY   #$18
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$19
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$1A
   STA   (FP),Y
   STA   @3.s15
   LDY   #$6B
   LDA   (FP),Y
   CLC
   ADC   #$01
   LDY   #$1B
   STA   (FP),Y
   LDY   #$6B
   STA   (FP),Y
   ORA   #$00
   BEQ   .if_end@0
   LDA   g1
   LDY   #$1C
   STA   (FP),Y
   LDA   g1+1
   INY
   STA   (FP),Y
   LDA   g1
   CLC
   ADC   #$01
   INY
   STA   (FP),Y
   LDA   g1+1
   ADC   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   g1
   INY
   LDA   (FP),Y
   STA   g1+1
   LDA   gp10
   STA   DPTR
   LDA   gp10+1
   STA   DPTR+1
   LDA   g2
   LDY   #$00
   STA   (DPTR),Y
   LDA   g2+1
   INY
   STA   (DPTR),Y
   LDA   g2+2
   INY
   STA   (DPTR),Y
   LDA   g2+3
   INY
   STA   (DPTR),Y
   LDA   g2+4
   INY
   STA   (DPTR),Y
   LDA   g2+5
   INY
   STA   (DPTR),Y
.if_end@0:
   LDA   #<gs9
   LDY   #$20
   STA   (FP),Y
   LDA   #>gs9
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$05
   LDY   #$22
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$23
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$24
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$25
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   SEC
   SBC   #$FF
   LDY   #$26
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   SBC   #$00
   LDY   #$27
   STA   (FP),Y
   LDY   #$22
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
   LDY   #$26
   LDA   (FP),Y
   LDY   #$28
   STA   (FP),Y
   LDY   #$6B
   STA   (FP),Y
   LDA   g4
   SEC
   SBC   #$01
   LDY   #$29
   STA   (FP),Y
   STA   g4
   LDY   #$14
   LDA   (FP),Y
   CLC
   ADC   #$01
   LDY   #$2A
   STA   (FP),Y
   LDY   #$14
   STA   (FP),Y
   LDY   #$2B
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
   LDY   #$2D
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
   LDA   #<gs9
   LDY   #$2F
   STA   (FP),Y
   LDA   #>gs9
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$01
   LDY   #$31
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$32
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$33
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$34
   STA   (FP),Y
   LDY   #$02
   LDA   (DPTR),Y
   LDY   #$35
   STA   (FP),Y
   LDY   #$03
   LDA   (DPTR),Y
   LDY   #$36
   STA   (FP),Y
   LDY   #$33
   LDA   (FP),Y
   CLC
   ADC   #$01
   LDY   #$37
   STA   (FP),Y
   LDY   #$34
   LDA   (FP),Y
   ADC   #$00
   LDY   #$38
   STA   (FP),Y
   LDY   #$35
   LDA   (FP),Y
   ADC   #$00
   LDY   #$39
   STA   (FP),Y
   LDY   #$36
   LDA   (FP),Y
   ADC   #$00
   LDY   #$3A
   STA   (FP),Y
   LDY   #$31
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$37
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$38
   LDA   (FP),Y
   LDY   #$01
   STA   (DPTR),Y
   LDY   #$39
   LDA   (FP),Y
   LDY   #$02
   STA   (DPTR),Y
   LDY   #$3A
   LDA   (FP),Y
   LDY   #$03
   STA   (DPTR),Y
   LDY   #$37
   LDA   (FP),Y
   LDY   #$3B
   STA   (FP),Y
   LDY   #$38
   LDA   (FP),Y
   LDY   #$3C
   STA   (FP),Y
   LDY   #$39
   LDA   (FP),Y
   LDY   #$3D
   STA   (FP),Y
   LDY   #$3A
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
   LDA   #<gs9
   LDY   #$3F
   STA   (FP),Y
   LDA   #>gs9
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$01
   LDY   #$41
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$42
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$43
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$44
   STA   (FP),Y
   LDY   #$02
   LDA   (DPTR),Y
   LDY   #$45
   STA   (FP),Y
   LDY   #$03
   LDA   (DPTR),Y
   LDY   #$46
   STA   (FP),Y
   LDY   #$43
   LDA   (FP),Y
   LDY   #$47
   STA   (FP),Y
   LDY   #$44
   LDA   (FP),Y
   LDY   #$48
   STA   (FP),Y
   LDY   #$45
   LDA   (FP),Y
   LDY   #$49
   STA   (FP),Y
   LDY   #$46
   LDA   (FP),Y
   LDY   #$4A
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@5
   LDA   #$00
   JMP   .sx_done@6
.sx_neg@5:
   LDA   #$FF
.sx_done@6:
   LDY   #$4B
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   g2
   SEC
   LDY   #$47
   SBC   (FP),Y
   LDY   #$4D
   STA   (FP),Y
   LDA   g2+1
   LDY   #$48
   SBC   (FP),Y
   LDY   #$4E
   STA   (FP),Y
   LDA   g2+2
   LDY   #$49
   SBC   (FP),Y
   LDY   #$4F
   STA   (FP),Y
   LDA   g2+3
   LDY   #$4A
   SBC   (FP),Y
   LDY   #$50
   STA   (FP),Y
   LDA   g2+4
   LDY   #$4B
   SBC   (FP),Y
   LDY   #$51
   STA   (FP),Y
   LDA   g2+5
   LDY   #$4C
   SBC   (FP),Y
   LDY   #$52
   STA   (FP),Y
   LDY   #$4D
   LDA   (FP),Y
   LDY   #$53
   STA   (FP),Y
   LDY   #$4E
   LDA   (FP),Y
   LDY   #$54
   STA   (FP),Y
   LDY   #$4F
   LDA   (FP),Y
   LDY   #$55
   STA   (FP),Y
   LDY   #$50
   LDA   (FP),Y
   LDY   #$56
   STA   (FP),Y
   LDY   #$51
   LDA   (FP),Y
   LDY   #$57
   STA   (FP),Y
   LDY   #$52
   LDA   (FP),Y
   AND   #$3F
   LDY   #$58
   STA   (FP),Y
   LDY   #$53
   LDA   (FP),Y
   LDY   #$59
   STA   (FP),Y
   LDY   #$54
   LDA   (FP),Y
   LDY   #$5A
   STA   (FP),Y
   LDY   #$55
   LDA   (FP),Y
   LDY   #$5B
   STA   (FP),Y
   LDY   #$56
   LDA   (FP),Y
   LDY   #$5C
   STA   (FP),Y
   LDY   #$57
   LDA   (FP),Y
   LDY   #$5D
   STA   (FP),Y
   LDY   #$58
   LDA   (FP),Y
   EOR   #$20
   LDY   #$5E
   STA   (FP),Y
   LDY   #$59
   LDA   (FP),Y
   LDY   #$5F
   STA   (FP),Y
   LDY   #$5A
   LDA   (FP),Y
   LDY   #$60
   STA   (FP),Y
   LDY   #$5B
   LDA   (FP),Y
   LDY   #$61
   STA   (FP),Y
   LDY   #$5C
   LDA   (FP),Y
   LDY   #$62
   STA   (FP),Y
   LDY   #$5D
   LDA   (FP),Y
   LDY   #$63
   STA   (FP),Y
   LDY   #$5E
   LDA   (FP),Y
   SEC
   SBC   #$20
   LDY   #$64
   STA   (FP),Y
   LDY   #$5F
   LDA   (FP),Y
   LDY   #$65
   STA   (FP),Y
   LDY   #$60
   LDA   (FP),Y
   LDY   #$66
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
   ADC   #$6B
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$67
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

   ; prologue: 0 arg bytes, 105 local bytes
   SEC
   LDA   SSP
   SBC   #$6B
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$6A
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
   LDY   #$07
   LDA   (FP),Y
   LDY   #$0F
   STA   (FP),Y
   LDY   #$08
   LDA   (FP),Y
   LDY   #$10
   STA   (FP),Y
   LDY   #$07
   LDA   (FP),Y
   CLC
   ADC   #$01
   LDY   #$11
   STA   (FP),Y
   LDY   #$08
   LDA   (FP),Y
   ADC   #$00
   LDY   #$12
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$07
   STA   (FP),Y
   LDY   #$12
   LDA   (FP),Y
   LDY   #$08
   STA   (FP),Y
   LDA   #$00
   LDY   #$13
   STA   (FP),Y
   INY
   STA   (FP),Y
.loop@1_start:
   LDY   #$15
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
   BVC   .cmp_novf@7
   EOR   #$80
.cmp_novf@7:
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
   BNE   .lb_skip@1
   JMP   .loop@1_break
.lb_skip@1:
   LDA   z6
   LDY   #$19
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
   LDY   #$1B
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
   JMP   .loop@1_continue
.loop@2_start:
   LDY   #$1F
   LDA   (FP),Y
   INY
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
   BVC   .cmp_novf@10
   EOR   #$80
.cmp_novf@10:
   ASL
   LDA   #$00
   ROL
   LDY   #$22
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .loop@2_break
   LDA   z7
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
   LDA   z7
   LDY   #$28
   STA   (FP),Y
   LDA   z7
   CLC
   ADC   #$01
   INY
   STA   (FP),Y
   STA   z7
   LDY   #$1F
   LDA   (FP),Y
   LDY   #$2A
   STA   (FP),Y
   CLC
   ADC   #$01
   INY
   STA   (FP),Y
   LDY   #$1F
   STA   (FP),Y
   JMP   .loop@2_start
.loop@2_break:
.loop@1_continue:
   LDY   #$14
   LDA   (FP),Y
   LDY   #$2C
   STA   (FP),Y
   CLC
   ADC   #$01
   INY
   STA   (FP),Y
   LDY   #$14
   STA   (FP),Y
   JMP   .loop@1_start
.loop@1_break:
   LDA   #<gs9
   LDY   #$2E
   STA   (FP),Y
   LDA   #>gs9
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$05
   LDY   #$30
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$31
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$32
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$33
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .cond_else@1
   LDA   g3
   INY
   STA   (FP),Y
   LDA   g3+1
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
   LDY   #$34
   LDA   (FP),Y
   LDY   #$3A
   STA   (FP),Y
   LDY   #$35
   LDA   (FP),Y
   LDY   #$3B
   STA   (FP),Y
   LDY   #$36
   LDA   (FP),Y
   LDY   #$3C
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   JMP   .cond_end@2
.cond_else@1:
   LDA   g2
   LDY   #$3A
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
   LDA   g2+4
   INY
   STA   (FP),Y
   LDA   g2+5
   INY
   STA   (FP),Y
.cond_end@2:
   LDA   g4
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
   BMI   .sx_neg@11
   LDA   #$00
   JMP   .sx_done@12
.sx_neg@11:
   LDA   #$FF
.sx_done@12:
   LDY   #$44
   STA   (FP),Y
   INY
   STA   (FP),Y
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
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$4A
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$4B
   STA   (FP),Y
   LDY   #$02
   LDA   (DPTR),Y
   LDY   #$4C
   STA   (FP),Y
   LDY   #$03
   LDA   (DPTR),Y
   LDY   #$4D
   STA   (FP),Y
   LDY   #$42
   LDA   (FP),Y
   LDY   #$4A
   EOR   (FP),Y
   LDY   #$4E
   STA   (FP),Y
   LDY   #$43
   LDA   (FP),Y
   LDY   #$4B
   EOR   (FP),Y
   LDY   #$4F
   STA   (FP),Y
   LDY   #$44
   LDA   (FP),Y
   LDY   #$4C
   EOR   (FP),Y
   LDY   #$50
   STA   (FP),Y
   LDY   #$45
   LDA   (FP),Y
   LDY   #$4D
   EOR   (FP),Y
   LDY   #$51
   STA   (FP),Y
   LDY   #$4E
   LDA   (FP),Y
   LDY   #$52
   STA   (FP),Y
   LDY   #$4F
   LDA   (FP),Y
   LDY   #$53
   STA   (FP),Y
   LDY   #$50
   LDA   (FP),Y
   LDY   #$54
   STA   (FP),Y
   LDY   #$51
   LDA   (FP),Y
   LDY   #$55
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@13
   LDA   #$00
   JMP   .sx_done@14
.sx_neg@13:
   LDA   #$FF
.sx_done@14:
   LDY   #$56
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$3A
   LDA   (FP),Y
   LDY   #$52
   AND   (FP),Y
   LDY   #$58
   STA   (FP),Y
   LDY   #$3B
   LDA   (FP),Y
   LDY   #$53
   AND   (FP),Y
   LDY   #$59
   STA   (FP),Y
   LDY   #$3C
   LDA   (FP),Y
   LDY   #$54
   AND   (FP),Y
   LDY   #$5A
   STA   (FP),Y
   LDY   #$3D
   LDA   (FP),Y
   LDY   #$55
   AND   (FP),Y
   LDY   #$5B
   STA   (FP),Y
   LDY   #$3E
   LDA   (FP),Y
   LDY   #$56
   AND   (FP),Y
   LDY   #$5C
   STA   (FP),Y
   LDY   #$3F
   LDA   (FP),Y
   LDY   #$57
   AND   (FP),Y
   LDY   #$5D
   STA   (FP),Y
   LDY   #$58
   LDA   (FP),Y
   LDY   #$5E
   STA   (FP),Y
   LDY   #$59
   LDA   (FP),Y
   LDY   #$5F
   STA   (FP),Y
   LDY   #$5A
   LDA   (FP),Y
   LDY   #$60
   STA   (FP),Y
   LDY   #$5B
   LDA   (FP),Y
   LDY   #$61
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$04
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDY   #$5E
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$5F
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$60
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$61
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   JSR   mix
   LDA   g1
   AND   #$05
   LDY   #$62
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
   DEY
   LDA   (FP),Y
   LDY   #$64
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$65
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   g3
   INY
   LDA   (FP),Y
   STA   g3+1
   JSR   fin0
   JSR   fin1
   JSR   fin2
   JSR   fin3
   LDA   acc
   LDY   #$66
   STA   (FP),Y
   LDA   acc+1
   AND   #$7F
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$68
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$69
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
   ADC   #$6B
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$6A
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
   DC.W  $007F

g2:
   DC.B  $05
   DC.B  $1B
   DC.B  $EE
   DC.B  $3A
   DC.B  $5F
   DC.B  $E7

g3:
   DC.W  $7048

g4:
   DC.B  $01

ga8:
   DC.B  $01
   DS.B  5
   DC.B  $03
   DS.B  5
   DC.B  $FF
   DC.B  $FF
   DC.B  $FF
   DC.B  $7F
   DS.B  2
   DC.B  $18
   DC.B  $FF
   DC.B  $66
   DC.B  $9E
   DC.B  $26
   DC.B  $1A

gs9:
   DS.B  1
   DC.L  $00000100
   DC.W  $0001

gp10:
   DC.W  g2

@3.s15:
   DC.B  $03

__zp_init:
   SUBROUTINE

   LDA   #$00
   STA   z5+1
   STA   z5+2
   STA   z5+3
   STA   z5+4
   STA   z5+5
   STA   z6+1
   STA   z6+2
   STA   z6+3
   LDA   #$01
   STA   z5
   LDA   #$02
   STA   z6
   LDA   #$80
   STA   z7
   RTS
