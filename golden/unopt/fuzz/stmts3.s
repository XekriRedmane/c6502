SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
z5	EQU	$26
z6	EQU	$28
z7	EQU	$2C

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

   LDA   z5
   LDY   #$01
   STA   (FP),Y
   LDA   z5+1
   INY
   STA   (FP),Y
   LDA   z5+1
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
   LDA   z7+1
   BMI   .sx_neg@6
   LDA   #$00
   JMP   .sx_done@7
.sx_neg@6:
   LDA   #$FF
.sx_done@7:
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
   LDA   ga8
   LDY   #$0D
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
   LDY   #$0D
   LDA   (FP),Y
   LDY   #$11
   STA   (FP),Y
   LDY   #$0E
   LDA   (FP),Y
   LDY   #$12
   STA   (FP),Y
   LDY   #$0F
   LDA   (FP),Y
   LDY   #$13
   STA   (FP),Y
   LDY   #$10
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

   ; prologue: 0 arg bytes, 34 local bytes
   SEC
   LDA   SSP
   SBC   #$24
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$23
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
   DEY
   LDA   (FP),Y
   LDY   #$1F
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$20
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
   LDY   #$1F
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$20
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$21
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$22
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   JSR   mix

   ; epilogue
   CLC
   LDA   FP
   ADC   #$24
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$23
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

   ; prologue: 0 arg bytes, 13 local bytes
   SEC
   LDA   SSP
   SBC   #$0F
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$0E
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
   LDY   #$04
   LDA   (DPTR),Y
   LDY   #$09
   STA   (FP),Y
   LDY   #$05
   LDA   (FP),Y
   LDY   #$0A
   STA   (FP),Y
   LDY   #$06
   LDA   (FP),Y
   LDY   #$0B
   STA   (FP),Y
   LDY   #$07
   LDA   (FP),Y
   LDY   #$0C
   STA   (FP),Y
   LDY   #$08
   LDA   (FP),Y
   LDY   #$0D
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$04
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDY   #$0A
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$0B
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$0C
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$0D
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   JSR   mix

   ; epilogue
   CLC
   LDA   FP
   ADC   #$0F
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$0E
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

   ; prologue: 10 arg bytes, 184 local bytes
   SEC
   LDA   SSP
   SBC   #$BA
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$B9
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
   INY
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@8
   LDA   #$00
   JMP   .sx_done@9
.sx_neg@8:
   LDA   #$FF
.sx_done@9:
   LDY   #$0E
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$BF
   LDA   (FP),Y
   CLC
   ADC   #$01
   LDY   #$10
   STA   (FP),Y
   LDY   #$C0
   LDA   (FP),Y
   ADC   #$00
   LDY   #$11
   STA   (FP),Y
   LDY   #$C1
   LDA   (FP),Y
   ADC   #$00
   LDY   #$12
   STA   (FP),Y
   LDY   #$C2
   LDA   (FP),Y
   ADC   #$00
   LDY   #$13
   STA   (FP),Y
   LDY   #$10
   LDA   (FP),Y
   LDY   #$14
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
   AND   #$1F
   LDY   #$17
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
   LDY   #$17
   LDA   (FP),Y
   EOR   #$10
   LDY   #$1B
   STA   (FP),Y
   LDY   #$18
   LDA   (FP),Y
   LDY   #$1C
   STA   (FP),Y
   LDY   #$19
   LDA   (FP),Y
   LDY   #$1D
   STA   (FP),Y
   LDY   #$1A
   LDA   (FP),Y
   LDY   #$1E
   STA   (FP),Y
   LDY   #$1B
   LDA   (FP),Y
   SEC
   SBC   #$10
   LDY   #$1F
   STA   (FP),Y
   LDY   #$1C
   LDA   (FP),Y
   LDY   #$BF
   STA   (FP),Y
   LDY   #$1D
   LDA   (FP),Y
   LDY   #$C0
   STA   (FP),Y
   LDY   #$1E
   LDA   (FP),Y
   LDY   #$C1
   STA   (FP),Y
   LDY   #$1F
   LDA   (FP),Y
   LDY   #$C2
   STA   (FP),Y
   LDY   #$0C
   LDA   (FP),Y
   SEC
   LDY   #$1C
   SBC   (FP),Y
   LDY   #$20
   STA   (FP),Y
   LDY   #$0D
   LDA   (FP),Y
   LDY   #$1D
   SBC   (FP),Y
   LDY   #$21
   STA   (FP),Y
   LDY   #$0E
   LDA   (FP),Y
   LDY   #$1E
   SBC   (FP),Y
   LDY   #$22
   STA   (FP),Y
   LDY   #$0F
   LDA   (FP),Y
   LDY   #$1F
   SBC   (FP),Y
   LDY   #$23
   STA   (FP),Y
   LDY   #$20
   LDA   (FP),Y
   LDY   #$24
   STA   (FP),Y
   LDY   #$21
   LDA   (FP),Y
   LDY   #$25
   STA   (FP),Y
   LDY   #$22
   LDA   (FP),Y
   LDY   #$26
   STA   (FP),Y
   LDY   #$23
   LDA   (FP),Y
   AND   #$1F
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
   LDY   #$27
   LDA   (FP),Y
   EOR   #$10
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
   LDY   #$2B
   LDA   (FP),Y
   SEC
   SBC   #$10
   LDY   #$2F
   STA   (FP),Y
   LDY   #$2C
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   INY
   ORA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@0
   JMP   .if_end@0
.lb_skip@0:
   LDA   #<ga8
   LDY   #$30
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
   LDY   #$34
   STA   (FP),Y
   STA   HARGS
   LDA   #$04
   STA   HARGS+1
   JSR   mul8
   LDA   HARGS+2
   LDY   #$35
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$30
   LDA   (FP),Y
   CLC
   LDY   #$36
   ADC   (FP),Y
   LDY   #$38
   STA   (FP),Y
   LDY   #$31
   LDA   (FP),Y
   LDY   #$37
   ADC   (FP),Y
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
   LDY   #$02
   LDA   (DPTR),Y
   LDY   #$3C
   STA   (FP),Y
   LDY   #$03
   LDA   (DPTR),Y
   LDY   #$3D
   STA   (FP),Y
   LDY   #$3A
   LDA   (FP),Y
   CLC
   ADC   #$01
   LDY   #$3E
   STA   (FP),Y
   LDY   #$3B
   LDA   (FP),Y
   ADC   #$00
   LDY   #$3F
   STA   (FP),Y
   LDY   #$3C
   LDA   (FP),Y
   ADC   #$00
   LDY   #$40
   STA   (FP),Y
   LDY   #$3D
   LDA   (FP),Y
   ADC   #$00
   LDY   #$41
   STA   (FP),Y
   LDY   #$38
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$3E
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$3F
   LDA   (FP),Y
   LDY   #$01
   STA   (DPTR),Y
   LDY   #$40
   LDA   (FP),Y
   LDY   #$02
   STA   (DPTR),Y
   LDY   #$41
   LDA   (FP),Y
   LDY   #$03
   STA   (DPTR),Y
   LDY   #$BB
   LDA   (FP),Y
   LDY   #$3E
   ORA   (FP),Y
   LDY   #$42
   STA   (FP),Y
   LDY   #$BC
   LDA   (FP),Y
   LDY   #$3F
   ORA   (FP),Y
   LDY   #$43
   STA   (FP),Y
   LDY   #$BD
   LDA   (FP),Y
   LDY   #$40
   ORA   (FP),Y
   LDY   #$44
   STA   (FP),Y
   LDY   #$BE
   LDA   (FP),Y
   LDY   #$41
   ORA   (FP),Y
   LDY   #$45
   STA   (FP),Y
   LDY   #$42
   LDA   (FP),Y
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
   AND   #$1F
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
   EOR   #$10
   LDY   #$4D
   STA   (FP),Y
   LDY   #$4A
   LDA   (FP),Y
   LDY   #$4E
   STA   (FP),Y
   LDY   #$4B
   LDA   (FP),Y
   LDY   #$4F
   STA   (FP),Y
   LDY   #$4C
   LDA   (FP),Y
   LDY   #$50
   STA   (FP),Y
   LDY   #$4D
   LDA   (FP),Y
   SEC
   SBC   #$10
   LDY   #$51
   STA   (FP),Y
   LDA   gp10
   STA   DPTR
   LDA   gp10+1
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
.if_end@0:
   LDA   ga8+12
   LDY   #$52
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
   LDY   #$52
   LDA   (FP),Y
   LDY   #$56
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
   ORA   #$00
   BMI   .sx_neg@12
   LDA   #$00
   JMP   .sx_done@13
.sx_neg@12:
   LDA   #$FF
.sx_done@13:
   LDY   #$5B
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$BB
   LDA   (FP),Y
   LDY   #$5D
   STA   (FP),Y
   ASL
   STA   (FP),Y
   LDY   #$BC
   LDA   (FP),Y
   LDY   #$5E
   STA   (FP),Y
   ROL
   STA   (FP),Y
   LDY   #$BD
   LDA   (FP),Y
   LDY   #$5F
   STA   (FP),Y
   ROL
   STA   (FP),Y
   LDY   #$BE
   LDA   (FP),Y
   LDY   #$60
   STA   (FP),Y
   ROL
   STA   (FP),Y
   LDY   #$5D
   LDA   (FP),Y
   LDY   #$61
   STA   (FP),Y
   LDY   #$5E
   LDA   (FP),Y
   LDY   #$62
   STA   (FP),Y
   LDY   #$5F
   LDA   (FP),Y
   LDY   #$63
   STA   (FP),Y
   LDY   #$60
   LDA   (FP),Y
   AND   #$1F
   LDY   #$64
   STA   (FP),Y
   LDY   #$61
   LDA   (FP),Y
   LDY   #$65
   STA   (FP),Y
   LDY   #$62
   LDA   (FP),Y
   LDY   #$66
   STA   (FP),Y
   LDY   #$63
   LDA   (FP),Y
   LDY   #$67
   STA   (FP),Y
   LDY   #$64
   LDA   (FP),Y
   EOR   #$10
   LDY   #$68
   STA   (FP),Y
   LDY   #$65
   LDA   (FP),Y
   LDY   #$69
   STA   (FP),Y
   LDY   #$66
   LDA   (FP),Y
   LDY   #$6A
   STA   (FP),Y
   LDY   #$67
   LDA   (FP),Y
   LDY   #$6B
   STA   (FP),Y
   LDY   #$68
   LDA   (FP),Y
   SEC
   SBC   #$10
   LDY   #$6C
   STA   (FP),Y
   LDY   #$69
   LDA   (FP),Y
   LDY   #$BB
   STA   (FP),Y
   LDY   #$6A
   LDA   (FP),Y
   LDY   #$BC
   STA   (FP),Y
   LDY   #$6B
   LDA   (FP),Y
   LDY   #$BD
   STA   (FP),Y
   LDY   #$6C
   LDA   (FP),Y
   LDY   #$BE
   STA   (FP),Y
   LDY   #$59
   LDA   (FP),Y
   CLC
   LDY   #$69
   ADC   (FP),Y
   LDY   #$6D
   STA   (FP),Y
   LDY   #$5A
   LDA   (FP),Y
   LDY   #$6A
   ADC   (FP),Y
   LDY   #$6E
   STA   (FP),Y
   LDY   #$5B
   LDA   (FP),Y
   LDY   #$6B
   ADC   (FP),Y
   LDY   #$6F
   STA   (FP),Y
   LDY   #$5C
   LDA   (FP),Y
   LDY   #$6C
   ADC   (FP),Y
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
   LDY   #$70
   LDA   (FP),Y
   AND   #$1F
   LDY   #$74
   STA   (FP),Y
   LDY   #$71
   LDA   (FP),Y
   LDY   #$75
   STA   (FP),Y
   LDY   #$72
   LDA   (FP),Y
   LDY   #$76
   STA   (FP),Y
   LDY   #$73
   LDA   (FP),Y
   LDY   #$77
   STA   (FP),Y
   LDY   #$74
   LDA   (FP),Y
   EOR   #$10
   LDY   #$78
   STA   (FP),Y
   LDY   #$75
   LDA   (FP),Y
   LDY   #$79
   STA   (FP),Y
   LDY   #$76
   LDA   (FP),Y
   LDY   #$7A
   STA   (FP),Y
   LDY   #$77
   LDA   (FP),Y
   LDY   #$7B
   STA   (FP),Y
   LDY   #$78
   LDA   (FP),Y
   SEC
   SBC   #$10
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
   SEC
   LDA   SSP
   SBC   #$04
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDY   #$7D
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$7E
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$7F
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$80
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   JSR   mix
   LDA   z5
   EOR   #$5A
   LDY   #$81
   STA   (FP),Y
   LDA   z5+1
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   z5
   INY
   LDA   (FP),Y
   STA   z5+1
   DEY
   LDA   (FP),Y
   LDY   #$83
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$84
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@14
   LDA   #$00
   JMP   .sx_done@15
.sx_neg@14:
   LDA   #$FF
.sx_done@15:
   LDY   #$85
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$83
   LDA   (FP),Y
   LDY   #$BF
   STA   (FP),Y
   LDY   #$84
   LDA   (FP),Y
   LDY   #$C0
   STA   (FP),Y
   LDY   #$85
   LDA   (FP),Y
   LDY   #$C1
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
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
   LDA   #$00
   INY
   STA   (FP),Y
   LDA   g1
   INY
   STA   (FP),Y
   LDA   g1
   BMI   .sx_neg@16
   LDA   #$00
   JMP   .sx_done@17
.sx_neg@16:
   LDA   #$FF
.sx_done@17:
   LDY   #$8B
   STA   (FP),Y
   LDY   #$88
   LDA   (FP),Y
   LDY   #$8A
   ORA   (FP),Y
   LDY   #$8C
   STA   (FP),Y
   LDY   #$89
   LDA   (FP),Y
   LDY   #$8B
   ORA   (FP),Y
   LDY   #$8D
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   z5
   INY
   LDA   (FP),Y
   STA   z5+1
   LDY   #$C3
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$8E
   STA   (FP),Y
   CLC
   ADC   #$01
   INY
   STA   (FP),Y
   LDY   #$C3
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$8F
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$8E
   LDA   (FP),Y
   LDY   #$90
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDA   z7
   DEY
   AND   (FP),Y
   LDY   #$92
   STA   (FP),Y
   LDA   z7+1
   DEY
   AND   (FP),Y
   LDY   #$93
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$94
   STA   (FP),Y
   STA   g3
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   SEC
   SBC   #$5A
   LDY   #$97
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   SBC   #$00
   LDY   #$98
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$99
   STA   (FP),Y
   STA   g3
   LDA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
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
   LDY   #$9A
   LDA   (FP),Y
   LDY   #$9D
   EOR   (FP),Y
   LDY   #$9F
   STA   (FP),Y
   LDY   #$9B
   LDA   (FP),Y
   LDY   #$9E
   EOR   (FP),Y
   LDY   #$A0
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$A1
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$A2
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@18
   LDA   #$00
   JMP   .sx_done@19
.sx_neg@18:
   LDA   #$FF
.sx_done@19:
   LDY   #$A3
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$A1
   LDA   (FP),Y
   LDY   #$BB
   STA   (FP),Y
   LDY   #$A2
   LDA   (FP),Y
   LDY   #$BC
   STA   (FP),Y
   LDY   #$A3
   LDA   (FP),Y
   LDY   #$BD
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #<gs9
   LDY   #$A5
   STA   (FP),Y
   LDA   #>gs9
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$03
   LDY   #$A7
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$A8
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$A9
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$AA
   STA   (FP),Y
   LDY   #$02
   LDA   (DPTR),Y
   LDY   #$AB
   STA   (FP),Y
   LDY   #$03
   LDA   (DPTR),Y
   LDY   #$AC
   STA   (FP),Y
   LDY   #$04
   LDA   (DPTR),Y
   LDY   #$AD
   STA   (FP),Y
   LDY   #$07
   LDA   (FP),Y
   LDY   #$AE
   STA   (FP),Y
   LDY   #$08
   LDA   (FP),Y
   LDY   #$AF
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$B0
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$B1
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$AD
   LDA   (FP),Y
   LDY   #$B4
   CMP   (FP),Y
   BNE   .cmp_true@20
   LDY   #$AC
   LDA   (FP),Y
   LDY   #$B3
   CMP   (FP),Y
   BNE   .cmp_true@20
   LDY   #$AB
   LDA   (FP),Y
   LDY   #$B2
   CMP   (FP),Y
   BNE   .cmp_true@20
   LDY   #$AA
   LDA   (FP),Y
   LDY   #$B1
   CMP   (FP),Y
   BNE   .cmp_true@20
   LDY   #$A9
   LDA   (FP),Y
   LDY   #$B0
   CMP   (FP),Y
   BNE   .cmp_true@20
   LDA   #$00
   JMP   .cmp_end@21
.cmp_true@20:
   LDA   #$01
.cmp_end@21:
   LDY   #$B5
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$B7
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$B8
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
   ADC   #$C4
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$B9
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

f19:
   SUBROUTINE

   ; prologue: 10 arg bytes, 174 local bytes
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
   LDA   #$FF
   INY
   STA   (FP),Y
   LDA   #$7F
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$08
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$09
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   g1
   INY
   STA   (FP),Y
   DEY
   STA   (FP),Y
   LDA   #$00
   LDY   #$0D
   STA   (FP),Y
   INY
   STA   (FP),Y
.loop@0_continue:
   LDY   #$0E
   LDA   (FP),Y
   INY
   STA   (FP),Y
   CLC
   ADC   #$01
   INY
   STA   (FP),Y
   LDY   #$0E
   STA   (FP),Y
   INY
   LDA   (FP),Y
   LDY   #$11
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
   BVC   .cmp_novf@23
   EOR   #$80
.cmp_novf@23:
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
   BNE   .lb_skip@5
   JMP   .loop@0_break
.lb_skip@5:
   LDA   #$7F
   LDY   #$15
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
   LDY   #$17
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$18
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$19
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@26
   LDA   #$00
   JMP   .sx_done@27
.sx_neg@26:
   LDA   #$FF
.sx_done@27:
   LDY   #$1A
   STA   (FP),Y
   INY
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
   ADC   #$03
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
   LDY   #$04
   LDA   (DPTR),Y
   LDY   #$25
   STA   (FP),Y
   LDY   #$21
   LDA   (FP),Y
   CLC
   ADC   #$01
   LDY   #$26
   STA   (FP),Y
   LDY   #$22
   LDA   (FP),Y
   ADC   #$00
   LDY   #$27
   STA   (FP),Y
   LDY   #$23
   LDA   (FP),Y
   ADC   #$00
   LDY   #$28
   STA   (FP),Y
   LDY   #$24
   LDA   (FP),Y
   ADC   #$00
   LDY   #$29
   STA   (FP),Y
   LDY   #$25
   LDA   (FP),Y
   ADC   #$00
   LDY   #$2A
   STA   (FP),Y
   LDY   #$26
   LDA   (FP),Y
   LDY   #$2B
   STA   (FP),Y
   LDY   #$27
   LDA   (FP),Y
   LDY   #$2C
   STA   (FP),Y
   LDY   #$28
   LDA   (FP),Y
   LDY   #$2D
   STA   (FP),Y
   LDY   #$29
   LDA   (FP),Y
   LDY   #$2E
   STA   (FP),Y
   LDY   #$2A
   LDA   (FP),Y
   AND   #$01
   LDY   #$2F
   STA   (FP),Y
   LDY   #$2B
   LDA   (FP),Y
   LDY   #$30
   STA   (FP),Y
   LDY   #$2C
   LDA   (FP),Y
   LDY   #$31
   STA   (FP),Y
   LDY   #$2D
   LDA   (FP),Y
   LDY   #$32
   STA   (FP),Y
   LDY   #$2E
   LDA   (FP),Y
   LDY   #$33
   STA   (FP),Y
   LDY   #$2F
   LDA   (FP),Y
   EOR   #$01
   LDY   #$34
   STA   (FP),Y
   LDY   #$30
   LDA   (FP),Y
   LDY   #$35
   STA   (FP),Y
   LDY   #$31
   LDA   (FP),Y
   LDY   #$36
   STA   (FP),Y
   LDY   #$32
   LDA   (FP),Y
   LDY   #$37
   STA   (FP),Y
   LDY   #$33
   LDA   (FP),Y
   LDY   #$38
   STA   (FP),Y
   LDY   #$34
   LDA   (FP),Y
   SEC
   SBC   #$01
   LDY   #$39
   STA   (FP),Y
   LDY   #$1F
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
   LDY   #$18
   LDA   (FP),Y
   SEC
   LDY   #$21
   SBC   (FP),Y
   LDY   #$3A
   STA   (FP),Y
   LDY   #$19
   LDA   (FP),Y
   LDY   #$22
   SBC   (FP),Y
   LDY   #$3B
   STA   (FP),Y
   LDY   #$1A
   LDA   (FP),Y
   LDY   #$23
   SBC   (FP),Y
   LDY   #$3C
   STA   (FP),Y
   LDY   #$1B
   LDA   (FP),Y
   LDY   #$24
   SBC   (FP),Y
   LDY   #$3D
   STA   (FP),Y
   LDY   #$1C
   LDA   (FP),Y
   LDY   #$25
   SBC   (FP),Y
   LDY   #$3E
   STA   (FP),Y
   LDY   #$3A
   LDA   (FP),Y
   LDY   #$3F
   STA   (FP),Y
   LDY   #$3B
   LDA   (FP),Y
   LDY   #$40
   STA   (FP),Y
   LDY   #$3C
   LDA   (FP),Y
   LDY   #$41
   STA   (FP),Y
   LDY   #$3D
   LDA   (FP),Y
   LDY   #$42
   STA   (FP),Y
   LDY   #$3E
   LDA   (FP),Y
   AND   #$01
   LDY   #$43
   STA   (FP),Y
   LDY   #$3F
   LDA   (FP),Y
   LDY   #$44
   STA   (FP),Y
   LDY   #$40
   LDA   (FP),Y
   LDY   #$45
   STA   (FP),Y
   LDY   #$41
   LDA   (FP),Y
   LDY   #$46
   STA   (FP),Y
   LDY   #$42
   LDA   (FP),Y
   LDY   #$47
   STA   (FP),Y
   LDY   #$43
   LDA   (FP),Y
   EOR   #$01
   LDY   #$48
   STA   (FP),Y
   LDY   #$44
   LDA   (FP),Y
   LDY   #$49
   STA   (FP),Y
   LDY   #$45
   LDA   (FP),Y
   LDY   #$4A
   STA   (FP),Y
   LDY   #$46
   LDA   (FP),Y
   LDY   #$4B
   STA   (FP),Y
   LDY   #$47
   LDA   (FP),Y
   LDY   #$4C
   STA   (FP),Y
   LDY   #$48
   LDA   (FP),Y
   SEC
   SBC   #$01
   LDY   #$4D
   STA   (FP),Y
   LDY   #$49
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   INY
   ORA   (FP),Y
   INY
   ORA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@4
   JMP   .loop@0_continue
.lb_skip@4:
   LDA   #<gs9
   LDY   #$4E
   STA   (FP),Y
   LDA   #>gs9
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$01
   LDY   #$50
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$51
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$52
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$53
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$54
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$55
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$55
   LDA   (FP),Y
   BNE   .cmp_true@28
   DEY
   LDA   (FP),Y
   CMP   #$02
   BNE   .cmp_true@28
   LDA   #$00
   JMP   .cmp_end@29
.cmp_true@28:
   LDA   #$01
.cmp_end@29:
   LDY   #$58
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDA   #$01
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   SEC
   LDY   #$58
   SBC   (FP),Y
   LDY   #$5C
   LDA   (FP),Y
   LDY   #$59
   SBC   (FP),Y
   BVC   .cmp_novf@31
   EOR   #$80
.cmp_novf@31:
   ASL
   LDA   #$00
   ROL
   EOR   #$01
   LDY   #$5D
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
   LDY   #$5D
   LDA   (FP),Y
   LDY   #$5F
   AND   (FP),Y
   LDY   #$61
   STA   (FP),Y
   LDY   #$5E
   LDA   (FP),Y
   LDY   #$60
   AND   (FP),Y
   LDY   #$62
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
   LDY   #$63
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@3
   JMP   .loop@0_continue
.lb_skip@3:
   LDY   #$61
   LDA   (FP),Y
   CMP   #$03
   BEQ   .cmp_true@34
   LDA   #$00
   JMP   .cmp_end@35
.cmp_true@34:
   LDA   #$01
.cmp_end@35:
   LDY   #$65
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@2
   JMP   .loop@0_continue
.lb_skip@2:
   LDY   #$61
   LDA   (FP),Y
   CMP   #$04
   BEQ   .cmp_true@36
   LDA   #$00
   JMP   .cmp_end@37
.cmp_true@36:
   LDA   #$01
.cmp_end@37:
   LDY   #$67
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@1
   JMP   .loop@0_continue
.lb_skip@1:
   LDY   #$B1
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
   LDA   #$06
   STA   HARGS+4
   JSR   asl32
   LDA   HARGS+5
   LDY   #$69
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
   LDY   #$69
   LDA   (FP),Y
   LDY   #$B1
   STA   (FP),Y
   LDY   #$6A
   LDA   (FP),Y
   LDY   #$B2
   STA   (FP),Y
   LDY   #$6B
   LDA   (FP),Y
   LDY   #$B3
   STA   (FP),Y
   LDY   #$6C
   LDA   (FP),Y
   LDY   #$B4
   STA   (FP),Y
   LDY   #$69
   LDA   (FP),Y
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
   LDY   #$6C
   LDA   (FP),Y
   AND   #$1F
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
   LDY   #$70
   LDA   (FP),Y
   EOR   #$10
   LDY   #$74
   STA   (FP),Y
   LDY   #$71
   LDA   (FP),Y
   LDY   #$75
   STA   (FP),Y
   LDY   #$72
   LDA   (FP),Y
   LDY   #$76
   STA   (FP),Y
   LDY   #$73
   LDA   (FP),Y
   LDY   #$77
   STA   (FP),Y
   LDY   #$74
   LDA   (FP),Y
   SEC
   SBC   #$10
   LDY   #$78
   STA   (FP),Y
   LDA   gp10
   STA   DPTR
   LDA   gp10+1
   STA   DPTR+1
   LDY   #$75
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$76
   LDA   (FP),Y
   LDY   #$01
   STA   (DPTR),Y
   LDY   #$77
   LDA   (FP),Y
   LDY   #$02
   STA   (DPTR),Y
   LDY   #$78
   LDA   (FP),Y
   LDY   #$03
   STA   (DPTR),Y
   LDA   g1
   LDY   #$79
   STA   (FP),Y
   LDA   g1
   BMI   .sx_neg@38
   LDA   #$00
   JMP   .sx_done@39
.sx_neg@38:
   LDA   #$FF
.sx_done@39:
   LDY   #$7A
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
   LDY   #$7B
   STA   (FP),Y
   LDA   HARGS+5
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$7D
   STA   (FP),Y
   STA   g1
   JMP   .loop@0_continue
.loop@0_break:
   LDY   #$B7
   LDA   (FP),Y
   LDY   #$7E
   STA   (FP),Y
   LDY   #$B8
   LDA   (FP),Y
   LDY   #$7F
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$B7
   STA   (FP),Y
   LDY   #$7F
   LDA   (FP),Y
   LDY   #$B8
   STA   (FP),Y
   LDY   #$7E
   LDA   (FP),Y
   LDY   #$80
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$81
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@40
   LDA   #$00
   JMP   .sx_done@41
.sx_neg@40:
   LDA   #$FF
.sx_done@41:
   LDY   #$82
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
   LDY   #$80
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$81
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$82
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$83
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   JSR   mix
   LDA   g4
   STA   HARGS
   LDA   g4+1
   STA   HARGS+1
   LDA   g4+2
   STA   HARGS+2
   LDA   g4+3
   STA   HARGS+3
   LDA   #$03
   STA   HARGS+4
   JSR   asr32
   LDA   HARGS+5
   LDY   #$84
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
   LDY   #$84
   LDA   (FP),Y
   LDY   #$88
   STA   (FP),Y
   LDY   #$85
   LDA   (FP),Y
   LDY   #$89
   STA   (FP),Y
   LDY   #$86
   LDA   (FP),Y
   LDY   #$8A
   STA   (FP),Y
   LDY   #$87
   LDA   (FP),Y
   AND   #$1F
   LDY   #$8B
   STA   (FP),Y
   LDY   #$88
   LDA   (FP),Y
   LDY   #$8C
   STA   (FP),Y
   LDY   #$89
   LDA   (FP),Y
   LDY   #$8D
   STA   (FP),Y
   LDY   #$8A
   LDA   (FP),Y
   LDY   #$8E
   STA   (FP),Y
   LDY   #$8B
   LDA   (FP),Y
   EOR   #$10
   LDY   #$8F
   STA   (FP),Y
   LDY   #$8C
   LDA   (FP),Y
   LDY   #$90
   STA   (FP),Y
   LDY   #$8D
   LDA   (FP),Y
   LDY   #$91
   STA   (FP),Y
   LDY   #$8E
   LDA   (FP),Y
   LDY   #$92
   STA   (FP),Y
   LDY   #$8F
   LDA   (FP),Y
   SEC
   SBC   #$10
   LDY   #$93
   STA   (FP),Y
   LDA   #$02
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
   CLC
   LDA   FP
   ADC   #$0B
   INY
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$0A
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDY   #$90
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$91
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$92
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$93
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   LDY   #$95
   LDA   (FP),Y
   LDY   #$05
   STA   (SSP),Y
   LDY   #$96
   LDA   (FP),Y
   LDY   #$06
   STA   (SSP),Y
   LDY   #$97
   LDA   (FP),Y
   LDY   #$07
   STA   (SSP),Y
   LDY   #$98
   LDA   (FP),Y
   LDY   #$08
   STA   (SSP),Y
   LDY   #$99
   LDA   (FP),Y
   LDY   #$09
   STA   (SSP),Y
   LDY   #$9A
   LDA   (FP),Y
   LDY   #$0A
   STA   (SSP),Y
   JSR   f12
   LDY   #$9B
   STA   (FP),Y
   TXA
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$9D
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$9E
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$9D
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   LDY   #$9E
   LDA   (FP),Y
   LDY   #$02
   STA   (FP),Y
   LDY   #$9F
   LDA   (FP),Y
   LDY   #$03
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$B9
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$A2
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$A3
   STA   (FP),Y
   LDY   #$02
   LDA   (DPTR),Y
   LDY   #$A4
   STA   (FP),Y
   LDY   #$03
   LDA   (DPTR),Y
   LDY   #$A5
   STA   (FP),Y
   LDY   #$A2
   LDA   (FP),Y
   LDY   #$A6
   STA   (FP),Y
   LDY   #$A3
   LDA   (FP),Y
   LDY   #$A7
   STA   (FP),Y
   LDY   #$A4
   LDA   (FP),Y
   LDY   #$A8
   STA   (FP),Y
   LDY   #$A5
   LDA   (FP),Y
   LDY   #$A9
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   CMP   (FP),Y
   BNE   .thread@0
   DEY
   CMP   (FP),Y
   BNE   .thread@0
   DEY
   CMP   (FP),Y
   BNE   .thread@0
   LDA   #$02
   DEY
   CMP   (FP),Y
   BEQ   .cmp_true@42
.thread@0:
   LDA   #$00
   JMP   .cmp_end@43
.cmp_true@42:
   LDA   #$01
.cmp_end@43:
   LDY   #$AB
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$AD
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$AE
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
   ADC   #$BA
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$AF
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

f29:
   SUBROUTINE

   ; prologue: 2 arg bytes, 82 local bytes
   SEC
   LDA   SSP
   SBC   #$54
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$53
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
   INY
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
   LDY   #$0F
   LDA   (FP),Y
   LDY   #$13
   STA   (FP),Y
   LDY   #$10
   LDA   (FP),Y
   LDY   #$14
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   g3
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   HARGS
   LDA   #$00
   LDX   #$08
   LSR   HARGS
.mul_loop@45:
   BCC   .mul_skip@46
   CLC
   ADC   #$5A
.mul_skip@46:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@45
   LDY   #$1A
   STA   (FP),Y
   LDA   HARGS
   DEY
   STA   (FP),Y
   LDY   #$1B
   STA   (FP),Y
   STA   g3
   LDA   (FP),Y
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
   BMI   .sx_neg@47
   LDA   #$00
   JMP   .sx_done@48
.sx_neg@47:
   LDA   #$FF
.sx_done@48:
   LDY   #$20
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   z6
   SEC
   LDY   #$1E
   SBC   (FP),Y
   LDY   #$22
   STA   (FP),Y
   LDA   z6+1
   LDY   #$1F
   SBC   (FP),Y
   LDY   #$23
   STA   (FP),Y
   LDA   z6+2
   LDY   #$20
   SBC   (FP),Y
   LDY   #$24
   STA   (FP),Y
   LDA   z6+3
   LDY   #$21
   SBC   (FP),Y
   LDY   #$25
   STA   (FP),Y
   LDY   #$22
   LDA   (FP),Y
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
   AND   #$1F
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
   LDY   #$29
   LDA   (FP),Y
   EOR   #$10
   LDY   #$2D
   STA   (FP),Y
   LDY   #$2A
   LDA   (FP),Y
   LDY   #$2E
   STA   (FP),Y
   LDY   #$2B
   LDA   (FP),Y
   LDY   #$2F
   STA   (FP),Y
   LDY   #$2C
   LDA   (FP),Y
   LDY   #$30
   STA   (FP),Y
   LDY   #$2D
   LDA   (FP),Y
   SEC
   SBC   #$10
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
   LDY   #$30
   LDA   (FP),Y
   LDY   #$34
   STA   (FP),Y
   LDY   #$31
   LDA   (FP),Y
   LDY   #$35
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
   LDA   g1
   LDY   #$36
   STA   (FP),Y
   LDA   g1
   BMI   .sx_neg@49
   LDA   #$00
   JMP   .sx_done@50
.sx_neg@49:
   LDA   #$FF
.sx_done@50:
   LDY   #$37
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$38
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$39
   STA   (FP),Y
   LDX   #$06
.shift_loop@51:
   LDY   #$38
   LDA   (FP),Y
   ASL
   STA   (FP),Y
   INY
   LDA   (FP),Y
   ROL
   STA   (FP),Y
   DEX
   BNE   .shift_loop@51
   DEY
   LDA   (FP),Y
   LDY   #$3A
   STA   (FP),Y
   STA   g1
   LDA   (FP),Y
   INY
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@53
   LDA   #$00
   JMP   .sx_done@54
.sx_neg@53:
   LDA   #$FF
.sx_done@54:
   LDY   #$3C
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
   LDA   g3
   LDY   #$3F
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$41
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$42
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
   LDY   #$45
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$46
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$47
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$48
   STA   (FP),Y
   LDY   #$41
   LDA   (FP),Y
   LDY   #$47
   AND   (FP),Y
   LDY   #$49
   STA   (FP),Y
   LDY   #$42
   LDA   (FP),Y
   LDY   #$48
   AND   (FP),Y
   LDY   #$4A
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$4B
   STA   (FP),Y
   STA   g2
   LDY   #$09
   LDA   (FP),Y
   SEC
   SBC   z6
   INY
   LDA   (FP),Y
   SBC   z6+1
   INY
   LDA   (FP),Y
   SBC   z6+2
   INY
   LDA   (FP),Y
   SBC   z6+3
   BVC   .cmp_novf@55
   EOR   #$80
.cmp_novf@55:
   ASL
   LDA   #$00
   ROL
   EOR   #$01
   LDY   #$4C
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$4E
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$4F
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@56
   LDA   #$00
   JMP   .sx_done@57
.sx_neg@56:
   LDA   #$FF
.sx_done@57:
   LDY   #$50
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$4E
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
   ADC   #$56
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

   ; prologue: 0 arg bytes, 228 local bytes
   SEC
   LDA   SSP
   SBC   #$E6
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$E5
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   LDA   #$ED
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
   INY
   STA   (FP),Y
   LDA   #$80
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
   LDY   #$0F
   LDA   (FP),Y
   LDY   #$13
   STA   (FP),Y
   LDY   #$10
   LDA   (FP),Y
   LDY   #$14
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   gp11
   STA   DPTR
   LDA   gp11+1
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$17
   STA   (FP),Y
   INY
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@58
   LDA   #$00
   JMP   .sx_done@59
.sx_neg@58:
   LDA   #$FF
.sx_done@59:
   LDY   #$19
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$1A
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$1B
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@60
   LDA   #$00
   JMP   .sx_done@61
.sx_neg@60:
   LDA   #$FF
.sx_done@61:
   LDY   #$1C
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$1A
   LDA   (FP),Y
   AND   z6
   LDY   #$1E
   STA   (FP),Y
   LDY   #$1B
   LDA   (FP),Y
   AND   z6+1
   LDY   #$1F
   STA   (FP),Y
   LDY   #$1C
   LDA   (FP),Y
   AND   z6+2
   LDY   #$20
   STA   (FP),Y
   LDY   #$1D
   LDA   (FP),Y
   AND   z6+3
   LDY   #$21
   STA   (FP),Y
   LDA   g2
   INY
   STA   (FP),Y
   LDA   g2
   BMI   .sx_neg@62
   LDA   #$00
   JMP   .sx_done@63
.sx_neg@62:
   LDA   #$FF
.sx_done@63:
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
   ORA   #$00
   BMI   .sx_neg@64
   LDA   #$00
   JMP   .sx_done@65
.sx_neg@64:
   LDA   #$FF
.sx_done@65:
   LDY   #$26
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$1E
   LDA   (FP),Y
   LDY   #$24
   AND   (FP),Y
   LDY   #$28
   STA   (FP),Y
   LDY   #$1F
   LDA   (FP),Y
   LDY   #$25
   AND   (FP),Y
   LDY   #$29
   STA   (FP),Y
   LDY   #$20
   LDA   (FP),Y
   LDY   #$26
   AND   (FP),Y
   LDY   #$2A
   STA   (FP),Y
   LDY   #$21
   LDA   (FP),Y
   LDY   #$27
   AND   (FP),Y
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
   LDY   #$2B
   LDA   (FP),Y
   LDY   #$2F
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$04
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDY   #$2C
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$2D
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$2E
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$2F
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   JSR   mix
   LDA   g4
   STA   HARGS
   LDA   g4+1
   STA   HARGS+1
   LDA   g4+2
   STA   HARGS+2
   LDA   g4+3
   STA   HARGS+3
   LDA   #$00
   STA   HARGS+4
   JSR   asr32
   LDA   HARGS+5
   LDY   #$30
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
   LDY   #$30
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
   LDY   #$30
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   INY
   ORA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@10
   JMP   .if_else@3
.lb_skip@10:
   LDY   #$0E
   LDA   (FP),Y
   BEQ   .lnot_true@66
   LDA   #$00
   JMP   .lnot_end@67
.lnot_true@66:
   LDA   #$01
.lnot_end@67:
   LDY   #$34
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$36
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$37
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@68
   LDA   #$00
   JMP   .sx_done@69
.sx_neg@68:
   LDA   #$FF
.sx_done@69:
   LDY   #$38
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$0E
   LDA   (FP),Y
   LDY   #$3A
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$3C
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$3D
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@70
   LDA   #$00
   JMP   .sx_done@71
.sx_neg@70:
   LDA   #$FF
.sx_done@71:
   LDY   #$3E
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   gp10
   STA   DPTR
   LDA   gp10+1
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$40
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$41
   STA   (FP),Y
   LDY   #$02
   LDA   (DPTR),Y
   LDY   #$42
   STA   (FP),Y
   LDY   #$03
   LDA   (DPTR),Y
   LDY   #$43
   STA   (FP),Y
   LDY   #$3C
   LDA   (FP),Y
   LDY   #$40
   AND   (FP),Y
   LDY   #$44
   STA   (FP),Y
   LDY   #$3D
   LDA   (FP),Y
   LDY   #$41
   AND   (FP),Y
   LDY   #$45
   STA   (FP),Y
   LDY   #$3E
   LDA   (FP),Y
   LDY   #$42
   AND   (FP),Y
   LDY   #$46
   STA   (FP),Y
   LDY   #$3F
   LDA   (FP),Y
   LDY   #$43
   AND   (FP),Y
   LDY   #$47
   STA   (FP),Y
   CLC
   LDA   FP
   ADC   #$0E
   INY
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$0A
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDY   #$36
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$37
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$38
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$39
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   LDY   #$44
   LDA   (FP),Y
   LDY   #$05
   STA   (SSP),Y
   LDY   #$45
   LDA   (FP),Y
   LDY   #$06
   STA   (SSP),Y
   LDY   #$46
   LDA   (FP),Y
   LDY   #$07
   STA   (SSP),Y
   LDY   #$47
   LDA   (FP),Y
   LDY   #$08
   STA   (SSP),Y
   LDY   #$48
   LDA   (FP),Y
   LDY   #$09
   STA   (SSP),Y
   LDY   #$49
   LDA   (FP),Y
   LDY   #$0A
   STA   (SSP),Y
   JSR   f12
   LDY   #$4A
   STA   (FP),Y
   TXA
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
   LDA   g2
   LDY   #$50
   STA   (FP),Y
   LDA   g2
   SEC
   SBC   #$01
   INY
   STA   (FP),Y
   STA   g2
   INY
   STA   (FP),Y
   LDA   g2
   BMI   .sx_neg@72
   LDA   #$00
   JMP   .sx_done@73
.sx_neg@72:
   LDA   #$FF
.sx_done@73:
   LDY   #$53
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   AND   #$01
   LDY   #$54
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDA   #$00
   LDY   #$55
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$56
   STA   (FP),Y
   STA   g2
   JMP   .if_end@2
.if_else@3:
   LDA   #<gs9
   LDY   #$57
   STA   (FP),Y
   LDA   #>gs9
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$01
   LDY   #$59
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$5A
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$5B
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$5C
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$5D
   STA   (FP),Y
   ASL
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$5E
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
   ASL
   STA   (FP),Y
   INY
   LDA   (FP),Y
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
   LDY   #$59
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$5D
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$5E
   LDA   (FP),Y
   LDY   #$01
   STA   (DPTR),Y
   LDY   #$5D
   LDA   (FP),Y
   LDY   #$5F
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$60
   STA   (FP),Y
   LDA   #$07
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$5F
   LDA   (FP),Y
   LDY   #$61
   AND   (FP),Y
   LDY   #$63
   STA   (FP),Y
   LDY   #$60
   LDA   (FP),Y
   LDY   #$62
   AND   (FP),Y
   LDY   #$64
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CMP   #$04
   BEQ   .cmp_true@74
   LDA   #$00
   JMP   .cmp_end@75
.cmp_true@74:
   LDA   #$01
.cmp_end@75:
   LDY   #$65
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
.if_end@2:
   LDA   #$FF
   LDY   #$67
   STA   (FP),Y
   INY
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@76
   LDA   #$00
   JMP   .sx_done@77
.sx_neg@76:
   LDA   #$FF
.sx_done@77:
   LDY   #$69
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
   BMI   .sx_neg@78
   LDA   #$00
   JMP   .sx_done@79
.sx_neg@78:
   LDA   #$FF
.sx_done@79:
   LDY   #$6C
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$03
   CLC
   LDY   #$6A
   ADC   (FP),Y
   LDY   #$6F
   STA   (FP),Y
   LDA   #$00
   LDY   #$6B
   ADC   (FP),Y
   LDY   #$70
   STA   (FP),Y
   LDA   #$00
   LDY   #$6C
   ADC   (FP),Y
   LDY   #$71
   STA   (FP),Y
   LDA   #$00
   LDY   #$6D
   ADC   (FP),Y
   LDY   #$72
   STA   (FP),Y
   LDA   #$00
   LDY   #$6E
   ADC   (FP),Y
   LDY   #$73
   STA   (FP),Y
   LDY   #$6F
   LDA   (FP),Y
   LDY   #$74
   STA   (FP),Y
   LDY   #$70
   LDA   (FP),Y
   LDY   #$75
   STA   (FP),Y
   LDY   #$71
   LDA   (FP),Y
   LDY   #$76
   STA   (FP),Y
   LDY   #$72
   LDA   (FP),Y
   LDY   #$77
   STA   (FP),Y
   LDY   #$73
   LDA   (FP),Y
   AND   #$01
   LDY   #$78
   STA   (FP),Y
   LDY   #$74
   LDA   (FP),Y
   LDY   #$79
   STA   (FP),Y
   LDY   #$75
   LDA   (FP),Y
   LDY   #$7A
   STA   (FP),Y
   LDY   #$76
   LDA   (FP),Y
   LDY   #$7B
   STA   (FP),Y
   LDY   #$77
   LDA   (FP),Y
   LDY   #$7C
   STA   (FP),Y
   LDY   #$78
   LDA   (FP),Y
   EOR   #$01
   LDY   #$7D
   STA   (FP),Y
   LDY   #$79
   LDA   (FP),Y
   LDY   #$7E
   STA   (FP),Y
   LDY   #$7A
   LDA   (FP),Y
   LDY   #$7F
   STA   (FP),Y
   LDY   #$7B
   LDA   (FP),Y
   LDY   #$80
   STA   (FP),Y
   LDY   #$7C
   LDA   (FP),Y
   LDY   #$81
   STA   (FP),Y
   LDY   #$7D
   LDA   (FP),Y
   SEC
   SBC   #$01
   LDY   #$82
   STA   (FP),Y
   LDY   #$7E
   LDA   (FP),Y
   LDY   #$83
   STA   (FP),Y
   STA   g3
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   SEC
   SBC   #$01
   LDY   #$86
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   SBC   #$00
   LDY   #$87
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$88
   STA   (FP),Y
   STA   g3
   LDA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDA   g1
   INY
   STA   (FP),Y
   LDA   g1
   BMI   .sx_neg@80
   LDA   #$00
   JMP   .sx_done@81
.sx_neg@80:
   LDA   #$FF
.sx_done@81:
   LDY   #$8C
   STA   (FP),Y
   LDY   #$89
   LDA   (FP),Y
   SEC
   LDY   #$8B
   SBC   (FP),Y
   LDY   #$8D
   STA   (FP),Y
   LDY   #$8A
   LDA   (FP),Y
   LDY   #$8C
   SBC   (FP),Y
   LDY   #$8E
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@9
   JMP   .if_end@4
.lb_skip@9:
   LDA   #$00
   LDY   #$8F
   STA   (FP),Y
   INY
   STA   (FP),Y
.loop@8_start:
   LDY   #$91
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
   BVC   .cmp_novf@82
   EOR   #$80
.cmp_novf@82:
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
   BNE   .lb_skip@8
   JMP   .loop@8_break
.lb_skip@8:
   LDA   z6
   LDY   #$95
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
   LDA   z6
   SEC
   SBC   #$01
   INY
   STA   (FP),Y
   LDA   z6+1
   SBC   #$00
   INY
   STA   (FP),Y
   LDA   z6+2
   SBC   #$00
   INY
   STA   (FP),Y
   LDA   z6+3
   SBC   #$00
   INY
   STA   (FP),Y
   LDY   #$99
   LDA   (FP),Y
   LDY   #$9D
   STA   (FP),Y
   LDY   #$9A
   LDA   (FP),Y
   LDY   #$9E
   STA   (FP),Y
   LDY   #$9B
   LDA   (FP),Y
   LDY   #$9F
   STA   (FP),Y
   LDY   #$9C
   LDA   (FP),Y
   AND   #$1F
   LDY   #$A0
   STA   (FP),Y
   LDY   #$9D
   LDA   (FP),Y
   LDY   #$A1
   STA   (FP),Y
   LDY   #$9E
   LDA   (FP),Y
   LDY   #$A2
   STA   (FP),Y
   LDY   #$9F
   LDA   (FP),Y
   LDY   #$A3
   STA   (FP),Y
   LDY   #$A0
   LDA   (FP),Y
   EOR   #$10
   LDY   #$A4
   STA   (FP),Y
   LDY   #$A1
   LDA   (FP),Y
   LDY   #$A5
   STA   (FP),Y
   LDY   #$A2
   LDA   (FP),Y
   LDY   #$A6
   STA   (FP),Y
   LDY   #$A3
   LDA   (FP),Y
   LDY   #$A7
   STA   (FP),Y
   LDY   #$A4
   LDA   (FP),Y
   SEC
   SBC   #$10
   LDY   #$A8
   STA   (FP),Y
   LDY   #$A5
   LDA   (FP),Y
   STA   z6
   INY
   LDA   (FP),Y
   STA   z6+1
   INY
   LDA   (FP),Y
   STA   z6+2
   INY
   LDA   (FP),Y
   STA   z6+3
   LDY   #$95
   LDA   (FP),Y
   LDY   #$A9
   STA   (FP),Y
   LDY   #$96
   LDA   (FP),Y
   LDY   #$AA
   STA   (FP),Y
   LDA   #$07
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$A9
   LDA   (FP),Y
   LDY   #$AB
   AND   (FP),Y
   LDY   #$AD
   STA   (FP),Y
   LDY   #$AA
   LDA   (FP),Y
   LDY   #$AC
   AND   (FP),Y
   LDY   #$AE
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CMP   #$02
   BEQ   .cmp_true@83
   LDA   #$00
   JMP   .cmp_end@84
.cmp_true@83:
   LDA   #$01
.cmp_end@84:
   LDY   #$AF
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .case@10
   LDY   #$AD
   LDA   (FP),Y
   CMP   #$06
   BEQ   .cmp_true@85
   LDA   #$00
   JMP   .cmp_end@86
.cmp_true@85:
   LDA   #$01
.cmp_end@86:
   LDY   #$B1
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@7
   JMP   .case@11
.lb_skip@7:
   LDY   #$AD
   LDA   (FP),Y
   CMP   #$07
   BEQ   .cmp_true@87
   LDA   #$00
   JMP   .cmp_end@88
.cmp_true@87:
   LDA   #$01
.cmp_end@88:
   LDY   #$B3
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@6
   JMP   .case@12
.lb_skip@6:
   JMP   .default@13
.case@10:
   LDY   #$90
   LDA   (FP),Y
   BNE   .or_true@5
   LDA   g3
   BNE   .or_true@5
   LDA   #$00
   LDY   #$B5
   STA   (FP),Y
   INY
   STA   (FP),Y
   JMP   .or_end@6
.or_true@5:
   LDA   #$01
   LDY   #$B5
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@6:
   LDY   #$B5
   LDA   (FP),Y
   LDY   #$B7
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$B8
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@89
   LDA   #$00
   JMP   .sx_done@90
.sx_neg@89:
   LDA   #$FF
.sx_done@90:
   LDY   #$B9
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$0E
   LDA   (FP),Y
   BEQ   .cond_else@7
   LDA   g4
   LDY   #$BB
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
   JMP   .cond_end@8
.cond_else@7:
   LDA   z6
   LDY   #$BB
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
.cond_end@8:
   LDY   #$BB
   LDA   (FP),Y
   LDY   #$BF
   STA   (FP),Y
   LDY   #$BC
   LDA   (FP),Y
   LDY   #$C0
   STA   (FP),Y
   LDY   #$BD
   LDA   (FP),Y
   LDY   #$C1
   STA   (FP),Y
   LDY   #$BE
   LDA   (FP),Y
   AND   #$1F
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
   LDY   #$C1
   LDA   (FP),Y
   LDY   #$C5
   STA   (FP),Y
   LDY   #$C2
   LDA   (FP),Y
   EOR   #$10
   LDY   #$C6
   STA   (FP),Y
   LDY   #$C3
   LDA   (FP),Y
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
   SEC
   SBC   #$10
   LDY   #$CA
   STA   (FP),Y
   CLC
   LDA   FP
   ADC   #$0E
   INY
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$0A
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDY   #$B7
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$B8
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$B9
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$BA
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   LDY   #$C7
   LDA   (FP),Y
   LDY   #$05
   STA   (SSP),Y
   LDY   #$C8
   LDA   (FP),Y
   LDY   #$06
   STA   (SSP),Y
   LDY   #$C9
   LDA   (FP),Y
   LDY   #$07
   STA   (SSP),Y
   LDY   #$CA
   LDA   (FP),Y
   LDY   #$08
   STA   (SSP),Y
   LDY   #$CB
   LDA   (FP),Y
   LDY   #$09
   STA   (SSP),Y
   LDY   #$CC
   LDA   (FP),Y
   LDY   #$0A
   STA   (SSP),Y
   JSR   f12
   LDY   #$CD
   STA   (FP),Y
   TXA
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
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$CF
   LDA   (FP),Y
   LDY   #$08
   STA   (FP),Y
   LDY   #$D0
   LDA   (FP),Y
   LDY   #$09
   STA   (FP),Y
   LDY   #$D1
   LDA   (FP),Y
   LDY   #$0A
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   JMP   .switch@9_break
.case@11:
.case@12:
   LDY   #$90
   LDA   (FP),Y
   BNE   .or_true@9
   LDA   #<gs9
   LDY   #$D4
   STA   (FP),Y
   LDA   #>gs9
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$01
   LDY   #$D6
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$D7
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$D8
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$D9
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@11
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   JMP   .or_end@12
.or_true@11:
   LDA   #$01
   LDY   #$DA
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@12:
   LDY   #$DA
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@9
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   JMP   .or_end@10
.or_true@9:
   LDA   #$01
   LDY   #$DC
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@10:
   LDY   #$DC
   LDA   (FP),Y
   LDY   #$DE
   STA   (FP),Y
   STA   g1
.default@13:
.switch@9_break:
   LDY   #$90
   LDA   (FP),Y
   LDY   #$DF
   STA   (FP),Y
   CLC
   ADC   #$01
   INY
   STA   (FP),Y
   LDY   #$90
   STA   (FP),Y
   JMP   .loop@8_start
.loop@8_break:
.if_end@4:
   JSR   fin0
   JSR   fin1
   JSR   fin2
   JSR   fin3
   LDA   acc
   LDY   #$E1
   STA   (FP),Y
   LDA   acc+1
   AND   #$7F
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$E3
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$E4
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
   ADC   #$E6
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$E5
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
   DC.B  $AC

g3:
   DC.B  $FF

g4:
   DC.L  $0000FFFF

ga8:
   DC.L  $0000007F
   DC.L  $00000002
   DC.L  $80000000
   DC.L  $00000002

gs9:
   DS.B  1
   DC.W  $000A
   DC.B  $0C
   DS.B  4

gp10:
   DC.W  z6

gp11:
   DC.W  g2

@4.s18:
   DS.B  1

@11.s27:
   DC.W  $0003

__zp_init:
   SUBROUTINE

   LDA   #$00
   STA   z5
   STA   z5+1
   LDA   #$FF
   STA   z6
   STA   z6+1
   STA   z6+2
   STA   z6+3
   STA   z7
   STA   z7+1
   RTS
