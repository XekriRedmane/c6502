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
   LDA   g2
   BMI   .sx_neg@0
   LDA   #$00
   JMP   .sx_done@1
.sx_neg@0:
   LDA   #$FF
.sx_done@1:
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
   LDA   g4+1
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

   ; prologue: 0 arg bytes, 32 local bytes
   SEC
   LDA   SSP
   SBC   #$22
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$21
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
   LDA   ga7
   LDY   #$09
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
   LDA   ga7+8
   LDY   #$15
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
   LDY   #$20
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$04
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDY   #$1D
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$1E
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$1F
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$20
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   JSR   mix

   ; epilogue
   CLC
   LDA   FP
   ADC   #$22
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$21
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

   ; prologue: 0 arg bytes, 43 local bytes
   SEC
   LDA   SSP
   SBC   #$2D
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$2C
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
   INY
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@2
   LDA   #$00
   JMP   .sx_done@3
.sx_neg@2:
   LDA   #$FF
.sx_done@3:
   LDY   #$1F
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
   LDA   #<gs8
   LDY   #$22
   STA   (FP),Y
   LDA   #>gs8
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
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$27
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$28
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$29
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
   LDY   #$28
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$29
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$2A
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$2B
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   JSR   mix

   ; epilogue
   CLC
   LDA   FP
   ADC   #$2D
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$2C
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

   ; prologue: 12 arg bytes, 90 local bytes
   SEC
   LDA   SSP
   SBC   #$5C
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$5B
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
   LDA   #$02
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
   LDA   #$FF
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   z5
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   AND   #$01
   LDY   #$0B
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDA   #$00
   LDY   #$0C
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$0D
   STA   (FP),Y
   STA   z5
   LDA   #$FF
   INY
   STA   (FP),Y
   LDA   #$7F
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@0
   JMP   .if_end@0
.lb_skip@0:
   LDA   #<ga7
   LDY   #$10
   STA   (FP),Y
   LDA   #>ga7
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
   LDA   #$08
   STA   HARGS+1
   JSR   mul8
   LDA   HARGS+2
   LDY   #$15
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$10
   LDA   (FP),Y
   CLC
   LDY   #$16
   ADC   (FP),Y
   LDY   #$18
   STA   (FP),Y
   LDY   #$11
   LDA   (FP),Y
   LDY   #$17
   ADC   (FP),Y
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
   LDY   #$04
   LDA   (DPTR),Y
   LDY   #$1E
   STA   (FP),Y
   LDY   #$05
   LDA   (DPTR),Y
   LDY   #$1F
   STA   (FP),Y
   LDY   #$06
   LDA   (DPTR),Y
   LDY   #$20
   STA   (FP),Y
   LDY   #$07
   LDA   (DPTR),Y
   LDY   #$21
   STA   (FP),Y
   LDA   #$03
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   ORA   #$00
   BPL   .sx_done@5
   LDA   #$FF
.sx_done@5:
   LDY   #$24
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
   LDY   #$1D
   LDA   (FP),Y
   LDY   #$25
   EOR   (FP),Y
   LDY   #$2D
   STA   (FP),Y
   LDY   #$1E
   LDA   (FP),Y
   LDY   #$26
   EOR   (FP),Y
   LDY   #$2E
   STA   (FP),Y
   LDY   #$1F
   LDA   (FP),Y
   LDY   #$27
   EOR   (FP),Y
   LDY   #$2F
   STA   (FP),Y
   LDY   #$20
   LDA   (FP),Y
   LDY   #$28
   EOR   (FP),Y
   LDY   #$30
   STA   (FP),Y
   LDY   #$21
   LDA   (FP),Y
   LDY   #$29
   EOR   (FP),Y
   LDY   #$31
   STA   (FP),Y
   LDY   #$18
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$2A
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$2B
   LDA   (FP),Y
   LDY   #$01
   STA   (DPTR),Y
   LDY   #$2C
   LDA   (FP),Y
   LDY   #$02
   STA   (DPTR),Y
   LDY   #$2D
   LDA   (FP),Y
   LDY   #$03
   STA   (DPTR),Y
   LDY   #$2E
   LDA   (FP),Y
   LDY   #$04
   STA   (DPTR),Y
   LDY   #$2F
   LDA   (FP),Y
   LDY   #$05
   STA   (DPTR),Y
   LDY   #$30
   LDA   (FP),Y
   LDY   #$06
   STA   (DPTR),Y
   LDY   #$31
   LDA   (FP),Y
   LDY   #$07
   STA   (DPTR),Y
   LDY   #$2A
   LDA   (FP),Y
   LDY   #$32
   STA   (FP),Y
   LDY   #$2B
   LDA   (FP),Y
   LDY   #$33
   STA   (FP),Y
   LDY   #$2C
   LDA   (FP),Y
   LDY   #$34
   STA   (FP),Y
   LDY   #$2D
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
   LDY   #$05
   LDA   (FP),Y
   LDY   #$36
   STA   (FP),Y
   LDY   #$06
   LDA   (FP),Y
   LDY   #$37
   STA   (FP),Y
   LDY   #$05
   LDA   (FP),Y
   CLC
   ADC   #$01
   LDY   #$38
   STA   (FP),Y
   LDY   #$06
   LDA   (FP),Y
   ADC   #$00
   LDY   #$39
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$05
   STA   (FP),Y
   LDY   #$39
   LDA   (FP),Y
   LDY   #$06
   STA   (FP),Y
   LDY   #$36
   LDA   (FP),Y
   LDY   #$3A
   STA   (FP),Y
   LDY   #$37
   LDA   (FP),Y
   LDY   #$3B
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
   LDA   #$05
   LDY   #$3E
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
   LDY   #$3F
   LDA   (FP),Y
   LDY   #$41
   AND   (FP),Y
   LDY   #$43
   STA   (FP),Y
   LDY   #$40
   LDA   (FP),Y
   LDY   #$42
   AND   (FP),Y
   LDY   #$44
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$45
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$46
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@6
   LDA   #$00
   JMP   .sx_done@7
.sx_neg@6:
   LDA   #$FF
.sx_done@7:
   LDY   #$47
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$45
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
   ADC   #$68
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$5B
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   RTS
.if_end@0:
   LDA   #$FF
   LDY   #$49
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   ORA   #$00
   BPL   .sx_done@9
   LDA   #$FF
.sx_done@9:
   LDY   #$4B
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   z6
   LDY   #$49
   EOR   (FP),Y
   LDY   #$4D
   STA   (FP),Y
   LDA   z6+1
   LDY   #$4A
   EOR   (FP),Y
   LDY   #$4E
   STA   (FP),Y
   LDA   z6+2
   LDY   #$4B
   EOR   (FP),Y
   LDY   #$4F
   STA   (FP),Y
   LDA   z6+3
   LDY   #$4C
   EOR   (FP),Y
   LDY   #$50
   STA   (FP),Y
   LDY   #$4D
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
   LDY   #$4D
   LDA   (FP),Y
   LDY   #$51
   STA   (FP),Y
   LDY   #$4E
   LDA   (FP),Y
   LDY   #$52
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
   ADC   #$02
   LDY   #$55
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$56
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$51
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$52
   LDA   (FP),Y
   LDY   #$01
   STA   (DPTR),Y
   LDY   #$5F
   LDA   (FP),Y
   LDY   #$57
   STA   (FP),Y
   LDY   #$60
   LDA   (FP),Y
   LDY   #$58
   STA   (FP),Y
   LDY   #$61
   LDA   (FP),Y
   LDY   #$59
   STA   (FP),Y
   LDY   #$62
   LDA   (FP),Y
   LDY   #$5A
   STA   (FP),Y
   LDY   #$57
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
   ADC   #$68
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

f16:
   SUBROUTINE

   ; prologue: 4 arg bytes, 153 local bytes
   SEC
   LDA   SSP
   SBC   #$9B
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$9A
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
   LDA   #$80
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
   LDA   #$03
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$9D
   STA   (FP),Y
   LDA   #$00
   LDY   #$09
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   g1
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$0A
   LDA   (FP),Y
   SEC
   LDY   #$0C
   SBC   (FP),Y
   LDY   #$0E
   STA   (FP),Y
   LDY   #$0B
   LDA   (FP),Y
   LDY   #$0D
   SBC   (FP),Y
   LDY   #$0F
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   ga7+8
   INY
   LDA   (FP),Y
   STA   ga7+9
   LDA   (FP),Y
   BMI   .sx_neg@10
   LDA   #$00
   JMP   .sx_done@11
.sx_neg@10:
   LDA   #$FF
.sx_done@11:
   STA   ga7+10
   STA   ga7+11
   STA   ga7+12
   STA   ga7+13
   STA   ga7+14
   STA   ga7+15
   LDA   #$80
   LDY   #$10
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   z5
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$16
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$17
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@12
   LDA   #$00
   JMP   .sx_done@13
.sx_neg@12:
   LDA   #$FF
.sx_done@13:
   LDY   #$18
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$10
   LDA   (FP),Y
   CLC
   LDY   #$16
   ADC   (FP),Y
   LDY   #$1A
   STA   (FP),Y
   LDY   #$11
   LDA   (FP),Y
   LDY   #$17
   ADC   (FP),Y
   LDY   #$1B
   STA   (FP),Y
   LDY   #$12
   LDA   (FP),Y
   LDY   #$18
   ADC   (FP),Y
   LDY   #$1C
   STA   (FP),Y
   LDY   #$13
   LDA   (FP),Y
   LDY   #$19
   ADC   (FP),Y
   LDY   #$1D
   STA   (FP),Y
   LDY   #$1A
   LDA   (FP),Y
   LDY   #$1E
   STA   (FP),Y
   LDY   #$1B
   LDA   (FP),Y
   LDY   #$1F
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
   ORA   #$00
   BMI   .sx_neg@14
   LDA   #$00
   JMP   .sx_done@15
.sx_neg@14:
   LDA   #$FF
.sx_done@15:
   LDY   #$24
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
   ADC   #$06
   INY
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$0C
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
   LDY   #$23
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$24
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   LDY   #$25
   LDA   (FP),Y
   LDY   #$05
   STA   (SSP),Y
   LDY   #$26
   LDA   (FP),Y
   LDY   #$06
   STA   (SSP),Y
   LDY   #$27
   LDA   (FP),Y
   LDY   #$07
   STA   (SSP),Y
   LDY   #$28
   LDA   (FP),Y
   LDY   #$08
   STA   (SSP),Y
   LDY   #$29
   LDA   (FP),Y
   LDY   #$09
   STA   (SSP),Y
   LDY   #$2A
   LDA   (FP),Y
   LDY   #$0A
   STA   (SSP),Y
   LDY   #$2B
   LDA   (FP),Y
   LDY   #$0B
   STA   (SSP),Y
   LDY   #$2C
   LDA   (FP),Y
   LDY   #$0C
   STA   (SSP),Y
   JSR   f9
   LDA   HARGS+8
   LDY   #$2D
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
   LDY   #$2D
   LDA   (FP),Y
   LDY   #$31
   STA   (FP),Y
   LDY   #$2E
   LDA   (FP),Y
   LDY   #$32
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$03
   STA   (FP),Y
   LDY   #$32
   LDA   (FP),Y
   LDY   #$04
   STA   (FP),Y
   LDY   #$06
   LDA   (FP),Y
   LDY   #$33
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@16
   LDA   #$00
   JMP   .sx_done@17
.sx_neg@16:
   LDA   #$FF
.sx_done@17:
   LDY   #$34
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$35
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$36
   STA   (FP),Y
   LDA   #$FF
   INY
   STA   (FP),Y
   LDA   #$7F
   INY
   STA   (FP),Y
   LDY   #$35
   LDA   (FP),Y
   CLC
   LDY   #$37
   ADC   (FP),Y
   LDY   #$39
   STA   (FP),Y
   LDY   #$36
   LDA   (FP),Y
   LDY   #$38
   ADC   (FP),Y
   LDY   #$3A
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$3B
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$3C
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$3B
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
   LDA   g1
   INY
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
   LDY   #$41
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
   INY
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@20
   LDA   #$00
   JMP   .sx_done@21
.sx_neg@20:
   LDA   #$FF
.sx_done@21:
   LDY   #$48
   STA   (FP),Y
   LDY   #$40
   LDA   (FP),Y
   LDY   #$47
   EOR   (FP),Y
   LDY   #$49
   STA   (FP),Y
   LDY   #$41
   LDA   (FP),Y
   LDY   #$48
   EOR   (FP),Y
   LDY   #$4A
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   ga7
   INY
   LDA   (FP),Y
   STA   ga7+1
   LDA   (FP),Y
   BMI   .sx_neg@22
   LDA   #$00
   JMP   .sx_done@23
.sx_neg@22:
   LDA   #$FF
.sx_done@23:
   STA   ga7+2
   STA   ga7+3
   STA   ga7+4
   STA   ga7+5
   STA   ga7+6
   STA   ga7+7
   LDY   #$06
   LDA   (FP),Y
   LDY   #$4B
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@24
   LDA   #$00
   JMP   .sx_done@25
.sx_neg@24:
   LDA   #$FF
.sx_done@25:
   LDY   #$4C
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$81
   LDY   #$4D
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$4E
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$4F
   STA   (FP),Y
   LDY   #$06
   STA   (FP),Y
   LDY   #$50
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@26
   LDA   #$00
   JMP   .sx_done@27
.sx_neg@26:
   LDA   #$FF
.sx_done@27:
   LDY   #$51
   STA   (FP),Y
   LDA   #$07
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$50
   LDA   (FP),Y
   LDY   #$52
   AND   (FP),Y
   LDY   #$54
   STA   (FP),Y
   LDY   #$51
   LDA   (FP),Y
   LDY   #$53
   AND   (FP),Y
   LDY   #$55
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CMP   #$03
   BEQ   .cmp_true@28
   LDA   #$00
   JMP   .cmp_end@29
.cmp_true@28:
   LDA   #$01
.cmp_end@29:
   LDY   #$56
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@2
   JMP   .switch@0_break
.lb_skip@2:
   LDY   #$54
   LDA   (FP),Y
   CMP   #$05
   BEQ   .cmp_true@30
   LDA   #$00
   JMP   .cmp_end@31
.cmp_true@30:
   LDA   #$01
.cmp_end@31:
   LDY   #$58
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .case@2
   LDY   #$54
   LDA   (FP),Y
   CMP   #$06
   BEQ   .cmp_true@32
   LDA   #$00
   JMP   .cmp_end@33
.cmp_true@32:
   LDA   #$01
.cmp_end@33:
   LDY   #$5A
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .case@3
.case@2:
.case@3:
   LDA   g1
   LDY   #$5C
   STA   (FP),Y
   INY
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@34
   LDA   #$00
   JMP   .sx_done@35
.sx_neg@34:
   LDA   #$FF
.sx_done@35:
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
   LDY   #$9E
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$61
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$62
   STA   (FP),Y
   LDY   #$5F
   LDA   (FP),Y
   LDY   #$61
   ORA   (FP),Y
   LDY   #$63
   STA   (FP),Y
   LDY   #$60
   LDA   (FP),Y
   LDY   #$62
   ORA   (FP),Y
   LDY   #$64
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@1
   JMP   .if_end@1
.lb_skip@1:
   LDA   #$00
   LDY   #$65
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$7F
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
   LDY   #$69
   STA   (FP),Y
   LDA   g2
   INY
   STA   (FP),Y
   LDA   g2
   BMI   .sx_neg@38
   LDA   #$00
   JMP   .sx_done@39
.sx_neg@38:
   LDA   #$FF
.sx_done@39:
   LDY   #$6B
   STA   (FP),Y
   LDY   #$68
   LDA   (FP),Y
   CLC
   LDY   #$6A
   ADC   (FP),Y
   LDY   #$6C
   STA   (FP),Y
   LDY   #$69
   LDA   (FP),Y
   LDY   #$6B
   ADC   (FP),Y
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
   LDA   g1
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
   CLC
   LDA   FP
   ADC   #$06
   INY
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$0C
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
   LDY   #$72
   LDA   (FP),Y
   LDY   #$05
   STA   (SSP),Y
   LDY   #$73
   LDA   (FP),Y
   LDY   #$06
   STA   (SSP),Y
   LDY   #$74
   LDA   (FP),Y
   LDY   #$07
   STA   (SSP),Y
   LDY   #$75
   LDA   (FP),Y
   LDY   #$08
   STA   (SSP),Y
   LDY   #$76
   LDA   (FP),Y
   LDY   #$09
   STA   (SSP),Y
   LDY   #$77
   LDA   (FP),Y
   LDY   #$0A
   STA   (SSP),Y
   LDY   #$78
   LDA   (FP),Y
   LDY   #$0B
   STA   (SSP),Y
   LDY   #$79
   LDA   (FP),Y
   LDY   #$0C
   STA   (SSP),Y
   JSR   f9
   LDA   HARGS+8
   LDY   #$7A
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
   LDA   g3
   INY
   STA   (FP),Y
   LDA   g3+1
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
   DEY
   LDA   (FP),Y
   STA   g3
   INY
   LDA   (FP),Y
   STA   g3+1
.if_end@1:
.switch@0_break:
   LDY   #$9C
   LDA   (FP),Y
   LDY   #$87
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$89
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$8A
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@41
   LDA   #$00
   JMP   .sx_done@42
.sx_neg@41:
   LDA   #$FF
.sx_done@42:
   LDY   #$8B
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
   LDY   #$89
   LDA   (FP),Y
   LDY   #$91
   STA   (FP),Y
   LDA   #$00
   LDY   #$8A
   AND   (FP),Y
   LDY   #$92
   STA   (FP),Y
   LDA   #$00
   LDY   #$8B
   AND   (FP),Y
   LDY   #$93
   STA   (FP),Y
   LDA   #$00
   LDY   #$8C
   AND   (FP),Y
   LDY   #$94
   STA   (FP),Y
   LDA   #$00
   LDY   #$8D
   AND   (FP),Y
   LDY   #$95
   STA   (FP),Y
   LDA   #$00
   LDY   #$8E
   AND   (FP),Y
   LDY   #$96
   STA   (FP),Y
   LDA   #$00
   LDY   #$8F
   AND   (FP),Y
   LDY   #$97
   STA   (FP),Y
   LDA   #$00
   LDY   #$90
   AND   (FP),Y
   LDY   #$98
   STA   (FP),Y
   LDY   #$91
   LDA   (FP),Y
   LDY   #$99
   STA   (FP),Y

   ; epilogue
   PHA
   CLC
   LDA   FP
   ADC   #$9F
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

   ; prologue: 0 arg bytes, 189 local bytes
   SEC
   LDA   SSP
   SBC   #$BF
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$BE
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
   LDA   #$0E
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
   LDY   #$09
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$0A
   STA   (FP),Y
   LDA   z6
   STA   HARGS
   LDA   z6+1
   STA   HARGS+1
   LDA   z6+2
   STA   HARGS+2
   LDA   z6+3
   STA   HARGS+3
   LDA   #$05
   STA   HARGS+4
   JSR   asr32
   LDA   HARGS+5
   LDY   #$0B
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
   LDY   #$0B
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
   LDY   #$03
   LDA   (FP),Y
   EOR   #$FF
   LDY   #$0F
   STA   (FP),Y
   LDY   #$04
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
   LDA   g3
   CLC
   ADC   #$03
   LDY   #$11
   STA   (FP),Y
   LDA   g3+1
   ADC   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   g3
   INY
   LDA   (FP),Y
   STA   g3+1
   LDA   g1
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$03
   LDY   #$15
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$16
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$17
   STA   (FP),Y
   STA   g1
   LDA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$1A
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$1B
   STA   (FP),Y
   LDA   g4
   DEY
   ORA   (FP),Y
   LDY   #$1C
   STA   (FP),Y
   LDA   g4+1
   DEY
   ORA   (FP),Y
   LDY   #$1D
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@4
   JMP   .if_end@2
.lb_skip@4:
   LDA   ga7+8
   LDY   #$1E
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
   LDA   ga7+16
   INY
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
   LDY   #$1E
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
   LDY   #$2E
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
   LDY   #$2E
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
   LDY   #$03
   LDA   (FP),Y
   SEC
   SBC   #$03
   LDY   #$36
   STA   (FP),Y
   LDY   #$04
   LDA   (FP),Y
   SBC   #$00
   LDY   #$37
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$03
   STA   (FP),Y
   LDY   #$37
   LDA   (FP),Y
   LDY   #$04
   STA   (FP),Y
   LDA   z5
   LDY   #$38
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LSR
   LDY   #$3B
   STA   (FP),Y
   LDA   #$00
   ROR
   DEY
   STA   (FP),Y
   LDY   #$3C
   STA   (FP),Y
   STA   z5
   LDA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   g3
   INY
   LDA   (FP),Y
   STA   g3+1
.if_end@3:
   LDA   #<gs8
   LDY   #$3F
   STA   (FP),Y
   LDA   #>gs8
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
   INY
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@43
   LDA   #$00
   JMP   .sx_done@44
.sx_neg@43:
   LDA   #$FF
.sx_done@44:
   LDY   #$45
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$46
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$47
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   SEC
   SBC   g3
   INY
   LDA   (FP),Y
   SBC   g3+1
   LDA   #$00
   ROL
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   ga7+24
   INY
   LDA   (FP),Y
   STA   ga7+25
   LDA   (FP),Y
   BMI   .sx_neg@45
   LDA   #$00
   JMP   .sx_done@46
.sx_neg@45:
   LDA   #$FF
.sx_done@46:
   STA   ga7+26
   STA   ga7+27
   STA   ga7+28
   STA   ga7+29
   STA   ga7+30
   STA   ga7+31
   LDY   #$09
   LDA   (FP),Y
   LDY   #$4A
   STA   (FP),Y
   ASL
   STA   (FP),Y
   LDY   #$0A
   LDA   (FP),Y
   LDY   #$4B
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
   LDY   #$09
   STA   (FP),Y
   LDY   #$4B
   LDA   (FP),Y
   LDY   #$0A
   STA   (FP),Y
   LDY   #$4A
   LDA   (FP),Y
   CLC
   ADC   g3
   LDY   #$4C
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   g3+1
   LDY   #$4D
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@3
   JMP   .if_end@4
.lb_skip@3:
   LDA   #<gs8
   LDY   #$4E
   STA   (FP),Y
   LDA   #>gs8
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$02
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
   INY
   ORA   (FP),Y
   BEQ   .cond_else@5
   LDA   g2
   INY
   STA   (FP),Y
   LDA   g2
   BMI   .sx_neg@47
   LDA   #$00
   JMP   .sx_done@48
.sx_neg@47:
   LDA   #$FF
.sx_done@48:
   LDY   #$55
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$56
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$57
   STA   (FP),Y
   JMP   .cond_end@6
.cond_else@5:
   LDA   #<gs8
   LDY   #$58
   STA   (FP),Y
   LDA   #>gs8
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$01
   LDY   #$5A
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$5B
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$5C
   STA   (FP),Y
   INY
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@49
   LDA   #$00
   JMP   .sx_done@50
.sx_neg@49:
   LDA   #$FF
.sx_done@50:
   LDY   #$5E
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$56
   STA   (FP),Y
   LDY   #$5E
   LDA   (FP),Y
   LDY   #$57
   STA   (FP),Y
.cond_end@6:
   LDY   #$56
   LDA   (FP),Y
   LDY   #$5F
   STA   (FP),Y
   LDY   #$57
   LDA   (FP),Y
   LDY   #$60
   STA   (FP),Y
   LDY   #$09
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@7
   LDA   #$7F
   LDY   #$61
   STA   (FP),Y
   ORA   #$00
   BEQ   .and_false@7
   LDA   #$01
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   JMP   .and_end@8
.and_false@7:
   LDA   #$00
   LDY   #$62
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@8:
   LDY   #$62
   LDA   (FP),Y
   LDY   #$64
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$65
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@51
   LDA   #$00
   JMP   .sx_done@52
.sx_neg@51:
   LDA   #$FF
.sx_done@52:
   LDY   #$66
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
   ADC   #$06
   INY
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$0C
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDY   #$5F
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$60
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$64
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$65
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   LDY   #$66
   LDA   (FP),Y
   LDY   #$05
   STA   (SSP),Y
   LDY   #$67
   LDA   (FP),Y
   LDY   #$06
   STA   (SSP),Y
   LDY   #$68
   LDA   (FP),Y
   LDY   #$07
   STA   (SSP),Y
   LDY   #$69
   LDA   (FP),Y
   LDY   #$08
   STA   (SSP),Y
   LDY   #$6A
   LDA   (FP),Y
   LDY   #$09
   STA   (SSP),Y
   LDY   #$6B
   LDA   (FP),Y
   LDY   #$0A
   STA   (SSP),Y
   LDY   #$6C
   LDA   (FP),Y
   LDY   #$0B
   STA   (SSP),Y
   LDY   #$6D
   LDA   (FP),Y
   LDY   #$0C
   STA   (SSP),Y
   JSR   f9
   LDA   HARGS+8
   LDY   #$6E
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
   LDY   #$6E
   LDA   (FP),Y
   LDY   #$72
   STA   (FP),Y
   LDY   #$6F
   LDA   (FP),Y
   LDY   #$73
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$09
   STA   (FP),Y
   LDY   #$73
   LDA   (FP),Y
   LDY   #$0A
   STA   (FP),Y
   LDA   #<ga7
   LDY   #$74
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
   LDY   #$78
   STA   (FP),Y
   STA   HARGS
   LDA   #$08
   STA   HARGS+1
   JSR   mul8
   LDA   HARGS+2
   LDY   #$79
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$74
   LDA   (FP),Y
   CLC
   LDY   #$7A
   ADC   (FP),Y
   LDY   #$7C
   STA   (FP),Y
   LDY   #$75
   LDA   (FP),Y
   LDY   #$7B
   ADC   (FP),Y
   LDY   #$7D
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$7E
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$7F
   STA   (FP),Y
   LDY   #$02
   LDA   (DPTR),Y
   LDY   #$80
   STA   (FP),Y
   LDY   #$03
   LDA   (DPTR),Y
   LDY   #$81
   STA   (FP),Y
   LDY   #$04
   LDA   (DPTR),Y
   LDY   #$82
   STA   (FP),Y
   LDY   #$05
   LDA   (DPTR),Y
   LDY   #$83
   STA   (FP),Y
   LDY   #$06
   LDA   (DPTR),Y
   LDY   #$84
   STA   (FP),Y
   LDY   #$07
   LDA   (DPTR),Y
   LDY   #$85
   STA   (FP),Y
   LDY   #$7E
   LDA   (FP),Y
   SEC
   SBC   #$01
   LDY   #$86
   STA   (FP),Y
   LDY   #$7F
   LDA   (FP),Y
   SBC   #$00
   LDY   #$87
   STA   (FP),Y
   LDY   #$80
   LDA   (FP),Y
   SBC   #$00
   LDY   #$88
   STA   (FP),Y
   LDY   #$81
   LDA   (FP),Y
   SBC   #$00
   LDY   #$89
   STA   (FP),Y
   LDY   #$82
   LDA   (FP),Y
   SBC   #$00
   LDY   #$8A
   STA   (FP),Y
   LDY   #$83
   LDA   (FP),Y
   SBC   #$00
   LDY   #$8B
   STA   (FP),Y
   LDY   #$84
   LDA   (FP),Y
   SBC   #$00
   LDY   #$8C
   STA   (FP),Y
   LDY   #$85
   LDA   (FP),Y
   SBC   #$00
   LDY   #$8D
   STA   (FP),Y
   LDY   #$7C
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$86
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$87
   LDA   (FP),Y
   LDY   #$01
   STA   (DPTR),Y
   LDY   #$88
   LDA   (FP),Y
   LDY   #$02
   STA   (DPTR),Y
   LDY   #$89
   LDA   (FP),Y
   LDY   #$03
   STA   (DPTR),Y
   LDY   #$8A
   LDA   (FP),Y
   LDY   #$04
   STA   (DPTR),Y
   LDY   #$8B
   LDA   (FP),Y
   LDY   #$05
   STA   (DPTR),Y
   LDY   #$8C
   LDA   (FP),Y
   LDY   #$06
   STA   (DPTR),Y
   LDY   #$8D
   LDA   (FP),Y
   LDY   #$07
   STA   (DPTR),Y
   LDY   #$86
   LDA   (FP),Y
   LDY   #$8E
   STA   (FP),Y
   LDY   #$87
   LDA   (FP),Y
   LDY   #$8F
   STA   (FP),Y
   LDY   #$88
   LDA   (FP),Y
   LDY   #$90
   STA   (FP),Y
   LDY   #$89
   LDA   (FP),Y
   LDY   #$91
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
.if_end@4:
.if_end@2:
   LDY   #$03
   LDA   (FP),Y
   LDY   #$92
   STA   (FP),Y
   LDY   #$04
   LDA   (FP),Y
   LDY   #$93
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@53
   LDA   #$00
   JMP   .sx_done@54
.sx_neg@53:
   LDA   #$FF
.sx_done@54:
   LDY   #$94
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
   LDY   #$92
   LDA   (FP),Y
   LDY   #$9A
   STA   (FP),Y
   LDY   #$93
   LDA   (FP),Y
   LDY   #$9B
   STA   (FP),Y
   LDA   #$00
   LDY   #$94
   AND   (FP),Y
   LDY   #$9C
   STA   (FP),Y
   LDA   #$00
   LDY   #$95
   AND   (FP),Y
   LDY   #$9D
   STA   (FP),Y
   LDA   #$00
   LDY   #$96
   AND   (FP),Y
   LDY   #$9E
   STA   (FP),Y
   LDA   #$00
   LDY   #$97
   AND   (FP),Y
   LDY   #$9F
   STA   (FP),Y
   LDA   #$00
   LDY   #$98
   AND   (FP),Y
   LDY   #$A0
   STA   (FP),Y
   LDA   #$00
   LDY   #$99
   AND   (FP),Y
   LDY   #$A1
   STA   (FP),Y
   LDY   #$9A
   LDA   (FP),Y
   LDY   #$A2
   STA   (FP),Y
   LDY   #$9B
   LDA   (FP),Y
   LDY   #$A3
   STA   (FP),Y
   LDY   #$03
   LDA   (FP),Y
   LDY   #$A4
   STA   (FP),Y
   LDY   #$04
   LDA   (FP),Y
   LDY   #$A5
   STA   (FP),Y
   LDA   g4
   SEC
   DEY
   SBC   (FP),Y
   LDA   g4+1
   INY
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
   LDY   #$A8
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$A9
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@55
   LDA   #$00
   JMP   .sx_done@56
.sx_neg@55:
   LDA   #$FF
.sx_done@56:
   LDY   #$AA
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
   ADC   #$06
   INY
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$0C
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDY   #$A2
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$A3
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$A8
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$A9
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   LDY   #$AA
   LDA   (FP),Y
   LDY   #$05
   STA   (SSP),Y
   LDY   #$AB
   LDA   (FP),Y
   LDY   #$06
   STA   (SSP),Y
   LDY   #$AC
   LDA   (FP),Y
   LDY   #$07
   STA   (SSP),Y
   LDY   #$AD
   LDA   (FP),Y
   LDY   #$08
   STA   (SSP),Y
   LDY   #$AE
   LDA   (FP),Y
   LDY   #$09
   STA   (SSP),Y
   LDY   #$AF
   LDA   (FP),Y
   LDY   #$0A
   STA   (SSP),Y
   LDY   #$B0
   LDA   (FP),Y
   LDY   #$0B
   STA   (SSP),Y
   LDY   #$B1
   LDA   (FP),Y
   LDY   #$0C
   STA   (SSP),Y
   JSR   f9
   LDA   HARGS+8
   LDY   #$B2
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
   LDY   #$B2
   LDA   (FP),Y
   LDY   #$B6
   STA   (FP),Y
   LDY   #$B3
   LDA   (FP),Y
   LDY   #$B7
   STA   (FP),Y
   LDY   #$B4
   LDA   (FP),Y
   LDY   #$B8
   STA   (FP),Y
   LDY   #$B5
   LDA   (FP),Y
   LDY   #$B9
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$04
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDY   #$B6
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$B7
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$B8
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$B9
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   JSR   mix
   JSR   fin0
   JSR   fin1
   JSR   fin2
   LDA   acc
   LDY   #$BA
   STA   (FP),Y
   LDA   acc+1
   AND   #$7F
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$BC
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$BD
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
   ADC   #$BF
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$BE
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
   DC.B  $04

g2:
   DS.B  1

g3:
   DC.W  $000D

g4:
   DC.W  $00FF

ga7:
   DC.L  $00000100
   DC.L  $00000000
   DC.L  $0000007F
   DC.L  $00000000
   DC.L  $00000003
   DC.L  $00000000
   DC.L  $00000100
   DC.L  $00000000

gs8:
   DS.B  1
   DC.B  $02
   DS.B  2

@10.s22:
   DC.W  $007F

__zp_init:
   SUBROUTINE

   LDA   #$00
   STA   z6+1
   STA   z6+2
   STA   z6+3
   LDA   #$03
   STA   z5
   LDA   #$FF
   STA   z6
   RTS
