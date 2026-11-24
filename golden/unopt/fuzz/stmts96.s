SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24

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

   ; prologue: 0 arg bytes, 21 local bytes
   SEC
   LDA   SSP
   SBC   #$17
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$16
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
   LDA   ga3
   LDY   #$0A
   STA   (FP),Y
   LDA   ga3+1
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$0C
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$0D
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@0
   LDA   #$00
   JMP   .sx_done@1
.sx_neg@0:
   LDA   #$FF
.sx_done@1:
   LDY   #$0E
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
   LDA   ga3+2
   LDY   #$10
   STA   (FP),Y
   LDA   ga3+3
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$12
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$13
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@2
   LDA   #$00
   JMP   .sx_done@3
.sx_neg@2:
   LDA   #$FF
.sx_done@3:
   LDY   #$14
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

   ; epilogue
   CLC
   LDA   FP
   ADC   #$17
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$16
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

   ; prologue: 0 arg bytes, 31 local bytes
   SEC
   LDA   SSP
   SBC   #$21
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$20
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   LDA   ga3+4
   LDY   #$01
   STA   (FP),Y
   LDA   ga3+5
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
   BMI   .sx_neg@4
   LDA   #$00
   JMP   .sx_done@5
.sx_neg@4:
   LDA   #$FF
.sx_done@5:
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
   LDA   ga3+6
   LDY   #$07
   STA   (FP),Y
   LDA   ga3+7
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
   ORA   #$00
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
   LDA   #<gs4
   LDY   #$0D
   STA   (FP),Y
   LDA   #>gs4
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
   LDA   #<gs4
   LDY   #$16
   STA   (FP),Y
   LDA   #>gs4
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$02
   LDY   #$18
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
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

   ; epilogue
   CLC
   LDA   FP
   ADC   #$21
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$20
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   RTS

f5:
   SUBROUTINE

   ; prologue: 2 arg bytes, 73 local bytes
   SEC
   LDA   SSP
   SBC   #$4B
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$4A
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   LDA   #$02
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
   LDA   g1
   SEC
   SBC   g1
   INY
   STA   (FP),Y
   LDA   g1+1
   SBC   g1+1
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
   LDA   #<gs4
   LDY   #$0D
   STA   (FP),Y
   LDA   #>gs4
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$02
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
   LDA   g1
   LDY   #$00
   STA   (DPTR),Y
   LDA   g1+1
   INY
   STA   (DPTR),Y
   LDY   #$4C
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
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$03
   LDA   (FP),Y
   LDY   #$14
   STA   (FP),Y
   LDY   #$04
   LDA   (FP),Y
   LDY   #$15
   STA   (FP),Y
   LDY   #$03
   LDA   (FP),Y
   SEC
   SBC   #$01
   LDY   #$16
   STA   (FP),Y
   LDY   #$04
   LDA   (FP),Y
   SBC   #$00
   LDY   #$17
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$03
   STA   (FP),Y
   LDY   #$17
   LDA   (FP),Y
   LDY   #$04
   STA   (FP),Y
   LDY   #$12
   LDA   (FP),Y
   LDY   #$14
   AND   (FP),Y
   LDY   #$18
   STA   (FP),Y
   LDY   #$13
   LDA   (FP),Y
   LDY   #$15
   AND   (FP),Y
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
   LDA   #<gs4
   INY
   STA   (FP),Y
   LDA   #>gs4
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
   LDY   #$1A
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$1B
   LDA   (FP),Y
   LDY   #$01
   STA   (DPTR),Y
   LDY   #$4C
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$20
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
   LDA   #$F4
   LDY   #$25
   STA   (FP),Y
   LDA   #$CD
   INY
   STA   (FP),Y
   LDA   #$47
   INY
   STA   (FP),Y
   LDA   #$3D
   INY
   STA   (FP),Y
   LDA   #<gs4
   INY
   STA   (FP),Y
   LDA   #>gs4
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$02
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
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$01
   LDY   #$2F
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$30
   STA   (FP),Y
   LDY   #$2B
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$2F
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$30
   LDA   (FP),Y
   LDY   #$01
   STA   (DPTR),Y
   LDY   #$2F
   LDA   (FP),Y
   LDY   #$31
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$32
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$25
   LDA   (FP),Y
   LDY   #$31
   ORA   (FP),Y
   LDY   #$35
   STA   (FP),Y
   LDY   #$26
   LDA   (FP),Y
   LDY   #$32
   ORA   (FP),Y
   LDY   #$36
   STA   (FP),Y
   LDY   #$27
   LDA   (FP),Y
   LDY   #$33
   ORA   (FP),Y
   LDY   #$37
   STA   (FP),Y
   LDY   #$28
   LDA   (FP),Y
   LDY   #$34
   ORA   (FP),Y
   LDY   #$38
   STA   (FP),Y
   LDY   #$35
   LDA   (FP),Y
   LDY   #$39
   STA   (FP),Y
   LDY   #$36
   LDA   (FP),Y
   LDY   #$3A
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$03
   STA   (FP),Y
   LDY   #$3A
   LDA   (FP),Y
   LDY   #$04
   STA   (FP),Y
   LDY   #$06
   LDA   (FP),Y
   LDY   #$3B
   STA   (FP),Y
   ORA   #$00
   BEQ   .if_end@0
   LDY   #$03
   LDA   (FP),Y
   SEC
   SBC   #$05
   LDY   #$3C
   STA   (FP),Y
   LDY   #$04
   LDA   (FP),Y
   SBC   #$00
   LDY   #$3D
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$03
   STA   (FP),Y
   LDY   #$3D
   LDA   (FP),Y
   LDY   #$04
   STA   (FP),Y
.if_end@0:
   LDA   #<gs4
   LDY   #$3E
   STA   (FP),Y
   LDA   #>gs4
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$02
   LDY   #$40
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$41
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$42
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$43
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$44
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
   LDY   #$45
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
   ADC   #$4D
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

f10:
   SUBROUTINE

   ; prologue: 4 arg bytes, 129 local bytes
   SEC
   LDA   SSP
   SBC   #$83
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$82
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
   LDA   #$80
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
   LDA   #$7F
   INY
   STA   (FP),Y
   INY
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
   SBC   #$02
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   DEY
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$10
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   JSR   f5
   LDA   HARGS+16
   LDY   #$11
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
   LDY   #$11
   LDA   (FP),Y
   LDY   #$16
   STA   (FP),Y
   LDY   #$12
   LDA   (FP),Y
   LDY   #$17
   STA   (FP),Y
   LDY   #$13
   LDA   (FP),Y
   LDY   #$18
   STA   (FP),Y
   LDY   #$14
   LDA   (FP),Y
   LDY   #$19
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$04
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDY   #$16
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$17
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$18
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$19
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   JSR   mix
   LDA   #<gs4
   LDY   #$1A
   STA   (FP),Y
   LDA   #>gs4
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$01
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
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$1E
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$21
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$22
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@8
   LDA   #$00
   JMP   .sx_done@9
.sx_neg@8:
   LDA   #$FF
.sx_done@9:
   LDY   #$23
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$09
   LDA   (FP),Y
   LDY   #$25
   STA   (FP),Y
   LDY   #$0A
   LDA   (FP),Y
   LDY   #$26
   STA   (FP),Y
   LDY   #$0B
   LDA   (FP),Y
   LDY   #$27
   STA   (FP),Y
   LDY   #$0C
   LDA   (FP),Y
   LDY   #$28
   STA   (FP),Y
   LDY   #$09
   LDA   (FP),Y
   SEC
   SBC   #$01
   LDY   #$29
   STA   (FP),Y
   LDY   #$0A
   LDA   (FP),Y
   SBC   #$00
   LDY   #$2A
   STA   (FP),Y
   LDY   #$0B
   LDA   (FP),Y
   SBC   #$00
   LDY   #$2B
   STA   (FP),Y
   LDY   #$0C
   LDA   (FP),Y
   SBC   #$00
   LDY   #$2C
   STA   (FP),Y
   LDY   #$29
   LDA   (FP),Y
   LDY   #$09
   STA   (FP),Y
   LDY   #$2A
   LDA   (FP),Y
   LDY   #$0A
   STA   (FP),Y
   LDY   #$2B
   LDA   (FP),Y
   LDY   #$0B
   STA   (FP),Y
   LDY   #$2C
   LDA   (FP),Y
   LDY   #$0C
   STA   (FP),Y
   LDY   #$21
   LDA   (FP),Y
   SEC
   LDY   #$25
   SBC   (FP),Y
   LDY   #$2D
   STA   (FP),Y
   LDY   #$22
   LDA   (FP),Y
   LDY   #$26
   SBC   (FP),Y
   LDY   #$2E
   STA   (FP),Y
   LDY   #$23
   LDA   (FP),Y
   LDY   #$27
   SBC   (FP),Y
   LDY   #$2F
   STA   (FP),Y
   LDY   #$24
   LDA   (FP),Y
   LDY   #$28
   SBC   (FP),Y
   LDY   #$30
   STA   (FP),Y
   LDY   #$2D
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   INY
   ORA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@0
   JMP   .if_else@2
.lb_skip@0:
   LDY   #$86
   LDA   (FP),Y
   LDY   #$31
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   AND   #$01
   LDY   #$33
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDA   #$00
   LDY   #$34
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$35
   STA   (FP),Y
   LDY   #$86
   STA   (FP),Y
   LDA   #<gs4
   LDY   #$36
   STA   (FP),Y
   LDA   #>gs4
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$02
   LDY   #$38
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
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
   DEY
   LDA   (FP),Y
   LDY   #$3C
   STA   (FP),Y
   ASL
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$3D
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
   LDY   #$38
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$3C
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$3D
   LDA   (FP),Y
   LDY   #$01
   STA   (DPTR),Y
   LDY   #$3C
   LDA   (FP),Y
   STA   g1
   INY
   LDA   (FP),Y
   STA   g1+1
   LDY   #$03
   LDA   (FP),Y
   LDY   #$3E
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
   LDY   #$40
   STA   (FP),Y
   LDA   #$02
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$3F
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
   LDY   #$43
   STA   (FP),Y
   LDA   HARGS+7
   INY
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
   BMI   .sx_neg@12
   LDA   #$00
   JMP   .sx_done@13
.sx_neg@12:
   LDA   #$FF
.sx_done@13:
   LDY   #$47
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
   LDY   #$45
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$46
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$47
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$48
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   JSR   mix
   JMP   .if_end@1
.if_else@2:
   LDA   g2
   LDY   #$49
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   SEC
   SBC   #$03
   LDY   #$4C
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   SBC   #$00
   LDY   #$4D
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$4E
   STA   (FP),Y
   STA   g2
   LDA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$51
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$52
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDA   #$80
   INY
   STA   (FP),Y
   LDY   #$51
   LDA   (FP),Y
   SEC
   LDY   #$53
   SBC   (FP),Y
   LDY   #$55
   STA   (FP),Y
   LDY   #$52
   LDA   (FP),Y
   LDY   #$54
   SBC   (FP),Y
   LDY   #$56
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$57
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$58
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
   LDY   #$57
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$58
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$59
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$5A
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   JSR   mix
   CLC
   LDA   FP
   ADC   #$0E
   LDY   #$5B
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
   LDY   #$5C
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   JSR   f5
   LDA   HARGS+16
   LDY   #$5D
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
   LDY   #$5D
   LDA   (FP),Y
   LDY   #$62
   STA   (FP),Y
   LDY   #$0E
   STA   (FP),Y
.if_end@1:
   CLC
   LDA   FP
   ADC   #$0E
   LDY   #$63
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
   LDY   #$64
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   JSR   f5
   LDA   HARGS+16
   LDY   #$65
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
   LDA   #$FF
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   ORA   (FP),Y
   BNE   .or_true@3
   LDA   #$7F
   BNE   .or_true@3
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   JMP   .or_end@4
.or_true@3:
   LDA   #$01
   LDY   #$6C
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@4:
   LDA   #$07
   LDY   #$6E
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$6C
   LDA   (FP),Y
   LDY   #$6E
   AND   (FP),Y
   LDY   #$70
   STA   (FP),Y
   LDY   #$6D
   LDA   (FP),Y
   LDY   #$6F
   AND   (FP),Y
   LDY   #$71
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CMP   #$01
   BEQ   .cmp_true@14
   LDA   #$00
   JMP   .cmp_end@15
.cmp_true@14:
   LDA   #$01
.cmp_end@15:
   LDY   #$72
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .case@1
   LDY   #$70
   LDA   (FP),Y
   CMP   #$06
   BEQ   .cmp_true@16
   LDA   #$00
   JMP   .cmp_end@17
.cmp_true@16:
   LDA   #$01
.cmp_end@17:
   LDY   #$74
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .case@2
   JMP   .switch@0_break
.case@1:
   LDY   #$84
   LDA   (FP),Y
   SEC
   SBC   #$FF
   LDY   #$76
   STA   (FP),Y
   LDY   #$85
   LDA   (FP),Y
   SBC   #$00
   LDY   #$77
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$84
   STA   (FP),Y
   LDY   #$77
   LDA   (FP),Y
   LDY   #$85
   STA   (FP),Y
   LDY   #$76
   LDA   (FP),Y
   LDY   #$78
   STA   (FP),Y

   ; epilogue
   PHA
   CLC
   LDA   FP
   ADC   #$87
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$82
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   PLA
   RTS
.case@2:
.switch@0_break:
   LDA   #$01
   LDY   #$79
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDA   g2
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$7A
   LDA   (FP),Y
   CLC
   LDY   #$7D
   ADC   (FP),Y
   LDY   #$7F
   STA   (FP),Y
   LDY   #$7B
   LDA   (FP),Y
   LDY   #$7E
   ADC   (FP),Y
   LDY   #$80
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$81
   STA   (FP),Y

   ; epilogue
   PHA
   CLC
   LDA   FP
   ADC   #$87
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

f18:
   SUBROUTINE

   ; prologue: 5 arg bytes, 166 local bytes
   SEC
   LDA   SSP
   SBC   #$A8
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$A7
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
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$01
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   @15.s24
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   AND   #$05
   LDY   #$0D
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDA   #$00
   LDY   #$0E
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$0F
   STA   (FP),Y
   STA   @15.s24
   LDA   #<ga3
   INY
   STA   (FP),Y
   LDA   #>ga3
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
   LDY   #$14
   STA   (FP),Y
   STA   HARGS
   LDA   #$02
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
   DEY
   LDA   (FP),Y
   SEC
   SBC   #$05
   LDY   #$1C
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   SBC   #$00
   LDY   #$1D
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$1E
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   AND   #$01
   LDY   #$1F
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$20
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   EOR   #$01
   LDY   #$21
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$22
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   SEC
   SBC   #$01
   LDY   #$23
   STA   (FP),Y
   LDY   #$18
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$22
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$23
   LDA   (FP),Y
   LDY   #$01
   STA   (DPTR),Y
   LDA   g2
   LDY   #$24
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$22
   LDA   (FP),Y
   CLC
   LDY   #$25
   ADC   (FP),Y
   LDY   #$27
   STA   (FP),Y
   LDY   #$23
   LDA   (FP),Y
   LDY   #$26
   ADC   (FP),Y
   LDY   #$28
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
   BMI   .sx_neg@18
   LDA   #$00
   JMP   .sx_done@19
.sx_neg@18:
   LDA   #$FF
.sx_done@19:
   LDY   #$2B
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
   LDA   #$00
   LDY   #$2D
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
   ORA   #$00
   BMI   .sx_neg@20
   LDA   #$00
   JMP   .sx_done@21
.sx_neg@20:
   LDA   #$FF
.sx_done@21:
   LDY   #$33
   STA   (FP),Y
   LDA   @15.s24
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$32
   LDA   (FP),Y
   LDY   #$34
   AND   (FP),Y
   LDY   #$36
   STA   (FP),Y
   LDY   #$33
   LDA   (FP),Y
   LDY   #$35
   AND   (FP),Y
   LDY   #$37
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$38
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   ASL
   STA   (FP),Y
   LDA   #$80
   INY
   STA   (FP),Y
   ROL
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   ROL
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   ROL
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   ROL
   STA   (FP),Y
   LDY   #$39
   LDA   (FP),Y
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
   AND   #$03
   LDY   #$42
   STA   (FP),Y
   LDY   #$3E
   LDA   (FP),Y
   LDY   #$43
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$04
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
   LDY   #$38
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$43
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   JSR   f10
   LDY   #$44
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
   LDY   #$46
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
   LDY   #$45
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$46
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$47
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$48
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   JSR   mix
   LDA   #$00
   LDY   #$49
   STA   (FP),Y
   INY
   STA   (FP),Y
.loop@3_start:
   LDY   #$4B
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
   BVC   .cmp_novf@24
   EOR   #$80
.cmp_novf@24:
   ASL
   LDA   #$00
   ROL
   LDY   #$4D
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
   LDY   #$4A
   LDA   (FP),Y
   LDY   #$4F
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   EOR   #$FF
   LDY   #$51
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   EOR   #$FF
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
   LDA   @15.s24
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$57
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$58
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$AC
   AND   (FP),Y
   LDY   #$59
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$AD
   AND   (FP),Y
   LDY   #$5A
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$5B
   STA   (FP),Y
   LDY   #$4A
   LDA   (FP),Y
   LDY   #$5C
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$5E
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$5F
   STA   (FP),Y
   LDY   #$AB
   CMP   (FP),Y
   BNE   .cmp_true@25
   LDY   #$5E
   LDA   (FP),Y
   LDY   #$AA
   CMP   (FP),Y
   BNE   .cmp_true@25
   LDA   #$00
   JMP   .cmp_end@26
.cmp_true@25:
   LDA   #$01
.cmp_end@26:
   LDY   #$60
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$62
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
   LDY   #$5B
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$62
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   JSR   f10
   LDY   #$63
   STA   (FP),Y
   INY
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@28
   LDA   #$00
   JMP   .sx_done@29
.sx_neg@28:
   LDA   #$FF
.sx_done@29:
   LDY   #$65
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
   LDY   #$64
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$65
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$66
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$67
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   JSR   mix
   LDY   #$AA
   LDA   (FP),Y
   CLC
   ADC   #$FF
   LDY   #$68
   STA   (FP),Y
   LDY   #$AB
   LDA   (FP),Y
   ADC   #$00
   LDY   #$69
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$AA
   STA   (FP),Y
   LDY   #$69
   LDA   (FP),Y
   LDY   #$AB
   STA   (FP),Y
   LDY   #$0A
   LDA   (FP),Y
   LDY   #$6A
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$4A
   LDA   (FP),Y
   LDY   #$6C
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$6A
   LDA   (FP),Y
   CLC
   LDY   #$6C
   ADC   (FP),Y
   LDY   #$6E
   STA   (FP),Y
   LDY   #$6B
   LDA   (FP),Y
   LDY   #$6D
   ADC   (FP),Y
   LDY   #$6F
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$70
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$71
   STA   (FP),Y
   LDY   #$68
   LDA   (FP),Y
   LDY   #$70
   ORA   (FP),Y
   LDY   #$72
   STA   (FP),Y
   LDY   #$69
   LDA   (FP),Y
   LDY   #$71
   ORA   (FP),Y
   LDY   #$73
   STA   (FP),Y
   LDA   #<gs4
   INY
   STA   (FP),Y
   LDA   #>gs4
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$02
   LDY   #$76
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$77
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$72
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$73
   LDA   (FP),Y
   LDY   #$01
   STA   (DPTR),Y
   LDY   #$4A
   LDA   (FP),Y
   LDY   #$78
   STA   (FP),Y
   CLC
   ADC   #$01
   INY
   STA   (FP),Y
   LDY   #$4A
   STA   (FP),Y
   JMP   .loop@3_start
.loop@3_break:
   LDA   #<gs4
   LDY   #$7A
   STA   (FP),Y
   LDA   #>gs4
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$02
   LDY   #$7C
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
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
   LDA   #<gs4
   INY
   STA   (FP),Y
   LDA   #>gs4
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$01
   LDY   #$82
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$83
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$84
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$87
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$88
   STA   (FP),Y
   LDY   #$7F
   LDA   (FP),Y
   LDY   #$88
   CMP   (FP),Y
   BNE   .cmp_true@30
   LDY   #$7E
   LDA   (FP),Y
   LDY   #$87
   CMP   (FP),Y
   BNE   .cmp_true@30
   LDA   #$00
   JMP   .cmp_end@31
.cmp_true@30:
   LDA   #$01
.cmp_end@31:
   LDY   #$89
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
   LDA   #$00
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
   LDY   #$8F
   STA   (FP),Y
   LDA   #$06
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$8E
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
   LDY   #$92
   STA   (FP),Y
   LDA   HARGS+7
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$94
   STA   (FP),Y
   LDY   #$A9
   LDA   (FP),Y
   BEQ   .lnot_true@35
   LDA   #$00
   JMP   .lnot_end@36
.lnot_true@35:
   LDA   #$01
.lnot_end@36:
   LDY   #$95
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$97
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$04
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDY   #$8B
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$8C
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$94
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$97
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   JSR   f10
   LDY   #$98
   STA   (FP),Y
   LDA   ga3+4
   INY
   STA   (FP),Y
   LDA   ga3+5
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$9B
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@5
   LDY   #$A9
   LDA   (FP),Y
   LDY   #$9E
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@37
   LDA   #$00
   JMP   .sx_done@38
.sx_neg@37:
   LDA   #$FF
.sx_done@38:
   LDY   #$9F
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   HARGS
   INY
   LDA   (FP),Y
   STA   HARGS+1
   LDA   #$81
   STA   HARGS+2
   LDA   #$00
   STA   HARGS+3
   JSR   mul16
   LDA   HARGS+4
   LDY   #$A0
   STA   (FP),Y
   LDA   HARGS+5
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$A2
   STA   (FP),Y
   LDY   #$A9
   STA   (FP),Y

   ; epilogue
   PHA
   CLC
   LDA   FP
   ADC   #$AD
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$A7
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   PLA
   RTS
.if_end@5:
   LDA   #$03
   LDY   #$A3
   STA   (FP),Y
   ORA   #$00
   BNE   .or_true@6
   LDA   #$01
   BNE   .or_true@6
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   JMP   .or_end@7
.or_true@6:
   LDA   #$01
   LDY   #$A4
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@7:
   LDY   #$A4
   LDA   (FP),Y
   LDY   #$A6
   STA   (FP),Y

   ; epilogue
   PHA
   CLC
   LDA   FP
   ADC   #$AD
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

   ; prologue: 0 arg bytes, 73 local bytes
   SEC
   LDA   SSP
   SBC   #$4B
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$4A
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   LDA   #$02
   LDY   #$01
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   LDY   #$05
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
   LDY   #$07
   STA   (FP),Y
   LDY   #$04
   LDA   (FP),Y
   LDY   #$08
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$06
   LDA   (FP),Y
   LDY   #$08
   ORA   (FP),Y
   LDY   #$0A
   STA   (FP),Y
   LDY   #$07
   LDA   (FP),Y
   LDY   #$09
   ORA   (FP),Y
   LDY   #$0B
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   LDY   #$0C
   STA   (FP),Y
   ORA   #$00
   BEQ   .cond_else@8
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@41
   LDA   #$00
   JMP   .sx_done@42
.sx_neg@41:
   LDA   #$FF
.sx_done@42:
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
   JMP   .cond_end@9
.cond_else@8:
   LDA   #$2E
   LDY   #$12
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$10
   STA   (FP),Y
   LDY   #$14
   LDA   (FP),Y
   LDY   #$11
   STA   (FP),Y
.cond_end@9:
   LDY   #$0A
   LDA   (FP),Y
   LDY   #$10
   EOR   (FP),Y
   LDY   #$15
   STA   (FP),Y
   LDY   #$0B
   LDA   (FP),Y
   LDY   #$11
   EOR   (FP),Y
   LDY   #$16
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$17
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$18
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@43
   LDA   #$00
   JMP   .sx_done@44
.sx_neg@43:
   LDA   #$FF
.sx_done@44:
   LDY   #$19
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
   LDA   g2
   LDY   #$1B
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
   LDA   #<gs4
   LDY   #$20
   STA   (FP),Y
   LDA   #>gs4
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$02
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
   SBC   #$5A
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
   INY
   ORA   (FP),Y
   BEQ   .if_else@11
   CLC
   LDA   FP
   ADC   #$04
   INY
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
   LDY   #$29
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   JSR   f5
   LDA   HARGS+16
   LDY   #$2A
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
   JMP   .if_end@10
.if_else@11:
   CLC
   LDA   FP
   ADC   #$04
   LDY   #$2F
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
   LDY   #$30
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   JSR   f5
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
   LDA   #<gs4
   INY
   STA   (FP),Y
   LDA   #>gs4
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$01
   LDY   #$38
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
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
.if_end@10:
   LDY   #$04
   LDA   (FP),Y
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
   LDA   #$00
   LDY   #$42
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$43
   STA   (FP),Y
   LDY   #$04
   STA   (FP),Y
   LDY   #$44
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   ga3+6
   INY
   LDA   (FP),Y
   STA   ga3+7
   JSR   fin0
   JSR   fin1
   LDA   acc
   LDY   #$46
   STA   (FP),Y
   LDA   acc+1
   AND   #$7F
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$48
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$49
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
   ADC   #$4B
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$4A
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
   DC.W  $FFFF

g2:
   DS.B  1

ga3:
   DC.W  $0080
   DC.W  $0003
   DC.W  $FFFF
   DC.W  $007F

gs4:
   DS.B  1
   DC.B  $FF
   DC.W  $FFFF

@2.s9:
   DC.B  $80

@8.s17:
   DC.B  $FF

@15.s24:
   DS.B  1
