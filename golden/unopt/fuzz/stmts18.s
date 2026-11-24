SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
z4	EQU	$26
z5	EQU	$27
z6	EQU	$29

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
   LDA   z4
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

   ; prologue: 0 arg bytes, 22 local bytes
   SEC
   LDA   SSP
   SBC   #$18
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$17
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
   DEY
   LDA   (FP),Y
   LDY   #$03
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$04
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@2
   LDA   #$00
   JMP   .sx_done@3
.sx_neg@2:
   LDA   #$FF
.sx_done@3:
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
   LDA   z6
   LDY   #$07
   STA   (FP),Y
   LDA   z6+1
   INY
   STA   (FP),Y
   LDA   z6+1
   BMI   .sx_neg@4
   LDA   #$00
   JMP   .sx_done@5
.sx_neg@4:
   LDA   #$FF
.sx_done@5:
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
   LDA   ga7
   LDY   #$0B
   STA   (FP),Y
   LDA   ga7+1
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$0D
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$0E
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
   LDA   ga7+2
   LDY   #$11
   STA   (FP),Y
   LDA   ga7+3
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

   ; epilogue
   CLC
   LDA   FP
   ADC   #$18
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$17
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

   LDA   ga7+4
   LDY   #$01
   STA   (FP),Y
   LDA   ga7+5
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
   LDA   ga7+6
   LDY   #$07
   STA   (FP),Y
   LDA   ga7+7
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
   LDA   #<gs8
   LDY   #$0D
   STA   (FP),Y
   LDA   #>gs8
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
   LDY   #$01
   LDA   (DPTR),Y
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
   ORA   #$00
   BMI   .sx_neg@6
   LDA   #$00
   JMP   .sx_done@7
.sx_neg@6:
   LDA   #$FF
.sx_done@7:
   LDY   #$15
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
   LDA   #<gs8
   LDY   #$17
   STA   (FP),Y
   LDA   #>gs8
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$03
   LDY   #$19
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$1A
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
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

f11:
   SUBROUTINE

   ; prologue: 6 arg bytes, 135 local bytes
   SEC
   LDA   SSP
   SBC   #$89
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$88
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
   LDA   #$CC
   INY
   STA   (FP),Y
   LDA   #$C7
   INY
   STA   (FP),Y
   LDA   #$FE
   INY
   STA   (FP),Y
   LDA   #$75
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
   LDA   g2
   EOR   #$03
   INY
   STA   (FP),Y
   LDA   g2+1
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   g2
   INY
   LDA   (FP),Y
   STA   g2+1
   DEY
   LDA   (FP),Y
   LDY   #$0D
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$0E
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$07
   LDA   (FP),Y
   LDY   #$0D
   ORA   (FP),Y
   LDY   #$11
   STA   (FP),Y
   LDY   #$08
   LDA   (FP),Y
   LDY   #$0E
   ORA   (FP),Y
   LDY   #$12
   STA   (FP),Y
   LDY   #$09
   LDA   (FP),Y
   LDY   #$0F
   ORA   (FP),Y
   LDY   #$13
   STA   (FP),Y
   LDY   #$0A
   LDA   (FP),Y
   LDY   #$10
   ORA   (FP),Y
   LDY   #$14
   STA   (FP),Y
   LDY   #$11
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   INY
   ORA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@2
   JMP   .if_else@1
.lb_skip@2:
   LDA   #$00
   LDY   #$15
   STA   (FP),Y
   INY
   STA   (FP),Y
.loop@0_start:
   LDY   #$17
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
   BVC   .cmp_novf@8
   EOR   #$80
.cmp_novf@8:
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
   BNE   .lb_skip@1
   JMP   .loop@0_break
.lb_skip@1:
   LDA   #<gs8
   LDY   #$1B
   STA   (FP),Y
   LDA   #>gs8
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$01
   LDY   #$1D
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
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
   DEY
   LDA   (FP),Y
   LDY   #$21
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$22
   STA   (FP),Y
   LDA   g2
   SEC
   DEY
   SBC   (FP),Y
   LDA   g2+1
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
   LDY   #$25
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$26
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@9
   LDA   #$00
   JMP   .sx_done@10
.sx_neg@9:
   LDA   #$FF
.sx_done@10:
   LDY   #$27
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
   LDA   #$05
   LDY   #$29
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   ORA   #$00
   BPL   .sx_done@12
   LDA   #$FF
.sx_done@12:
   LDY   #$2B
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$8C
   LDA   (FP),Y
   LDY   #$29
   AND   (FP),Y
   LDY   #$2D
   STA   (FP),Y
   LDY   #$8D
   LDA   (FP),Y
   LDY   #$2A
   AND   (FP),Y
   LDY   #$2E
   STA   (FP),Y
   LDY   #$8E
   LDA   (FP),Y
   LDY   #$2B
   AND   (FP),Y
   LDY   #$2F
   STA   (FP),Y
   LDY   #$8F
   LDA   (FP),Y
   LDY   #$2C
   AND   (FP),Y
   LDY   #$30
   STA   (FP),Y
   LDY   #$2D
   LDA   (FP),Y
   LDY   #$8C
   STA   (FP),Y
   LDY   #$2E
   LDA   (FP),Y
   LDY   #$8D
   STA   (FP),Y
   LDY   #$2F
   LDA   (FP),Y
   LDY   #$8E
   STA   (FP),Y
   LDY   #$30
   LDA   (FP),Y
   LDY   #$8F
   STA   (FP),Y
   LDY   #$2D
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   INY
   ORA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@0
   JMP   .loop@0_break
.lb_skip@0:
   LDA   z5
   LDY   #$45
   STA   (FP),Y
   LDA   z5+1
   INY
   STA   (FP),Y
   LDA   #$FF
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$45
   LDA   (FP),Y
   SEC
   LDY   #$47
   SBC   (FP),Y
   LDY   #$49
   STA   (FP),Y
   LDY   #$46
   LDA   (FP),Y
   LDY   #$48
   SBC   (FP),Y
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
   ORA   #$00
   BMI   .sx_neg@15
   LDA   #$00
   JMP   .sx_done@16
.sx_neg@15:
   LDA   #$FF
.sx_done@16:
   LDY   #$4D
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
   LDY   #$16
   LDA   (FP),Y
   LDY   #$4F
   STA   (FP),Y
   CLC
   ADC   #$01
   INY
   STA   (FP),Y
   LDY   #$16
   STA   (FP),Y
   JMP   .loop@0_start
.loop@0_break:
   LDA   z6
   CLC
   ADC   #$03
   LDY   #$51
   STA   (FP),Y
   LDA   z6+1
   ADC   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$53
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   AND   #$3F
   LDY   #$54
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$55
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   EOR   #$20
   LDY   #$56
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$57
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   SEC
   SBC   #$20
   LDY   #$58
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   z6
   INY
   LDA   (FP),Y
   STA   z6+1
   DEY
   LDA   (FP),Y
   LDY   #$8A
   STA   (FP),Y
   LDY   #$58
   LDA   (FP),Y
   LDY   #$8B
   STA   (FP),Y
   JMP   .if_end@0
.if_else@1:
   LDA   #$00
   LDY   #$59
   STA   (FP),Y
   INY
   STA   (FP),Y
.loop@1_continue:
   LDY   #$5A
   LDA   (FP),Y
   INY
   STA   (FP),Y
   CLC
   ADC   #$01
   INY
   STA   (FP),Y
   LDY   #$5A
   STA   (FP),Y
   INY
   LDA   (FP),Y
   LDY   #$5D
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
   BVC   .cmp_novf@17
   EOR   #$80
.cmp_novf@17:
   ASL
   LDA   #$00
   ROL
   LDY   #$5F
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .loop@1_continue
.if_end@0:
   LDA   gp10
   STA   DPTR
   LDA   gp10+1
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$61
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$62
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   SEC
   SBC   #$01
   LDY   #$63
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   SBC   #$00
   LDY   #$64
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$65
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   AND   #$3F
   LDY   #$66
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$67
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   EOR   #$20
   LDY   #$68
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$69
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   SEC
   SBC   #$20
   LDY   #$6A
   STA   (FP),Y
   LDA   gp10
   STA   DPTR
   LDA   gp10+1
   STA   DPTR+1
   DEY
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$6A
   LDA   (FP),Y
   LDY   #$01
   STA   (DPTR),Y
   LDY   #$61
   LDA   (FP),Y
   LDY   #$6B
   STA   (FP),Y
   LDY   #$62
   LDA   (FP),Y
   LDY   #$6C
   STA   (FP),Y
   LDA   #$FF
   INY
   STA   (FP),Y
   LDA   #$7F
   INY
   STA   (FP),Y
   LDY   #$6B
   LDA   (FP),Y
   CLC
   LDY   #$6D
   ADC   (FP),Y
   LDY   #$6F
   STA   (FP),Y
   LDY   #$6C
   LDA   (FP),Y
   LDY   #$6E
   ADC   (FP),Y
   LDY   #$70
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$71
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$72
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
   LDY   #$75
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$76
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$71
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$72
   LDA   (FP),Y
   LDY   #$01
   STA   (DPTR),Y
   LDA   z4
   LDY   #$77
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
.mul_loop@18:
   BCC   .mul_skip@19
   CLC
   ADC   #$01
.mul_skip@19:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@18
   LDY   #$7A
   STA   (FP),Y
   LDA   HARGS
   DEY
   STA   (FP),Y
   LDY   #$7B
   STA   (FP),Y
   STA   z4
   LDA   (FP),Y
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
   LDY   #$7C
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$7D
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$7E
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$7F
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   JSR   mix
   LDA   gp9
   STA   DPTR
   LDA   gp9+1
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$80
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDA   #$06
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$81
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
   LDY   #$85
   STA   (FP),Y
   LDA   HARGS+7
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$87
   STA   (FP),Y

   ; epilogue
   PHA
   CLC
   LDA   FP
   ADC   #$8F
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

f19:
   SUBROUTINE

   ; prologue: 6 arg bytes, 115 local bytes
   SEC
   LDA   SSP
   SBC   #$75
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$74
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
   LDA   z5
   INY
   STA   (FP),Y
   LDA   z5+1
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   EOR   #$5A
   LDY   #$05
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$06
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   z5
   INY
   LDA   (FP),Y
   STA   z5+1
   DEY
   LDA   (FP),Y
   LDY   #$07
   STA   (FP),Y
   LDA   gp9
   STA   DPTR
   LDA   gp9+1
   STA   DPTR+1
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDA   gp9
   STA   DPTR
   LDA   gp9+1
   STA   DPTR+1
   LDA   (DPTR),Y
   LDY   #$08
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$FF
   LDY   #$0B
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$0C
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$0D
   STA   (FP),Y
   LDA   gp9
   STA   DPTR
   LDA   gp9+1
   STA   DPTR+1
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$0D
   LDA   (FP),Y
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
   LDY   #$0E
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$0F
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$10
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$11
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   JSR   mix
   LDA   #<gs8
   LDY   #$12
   STA   (FP),Y
   LDA   #>gs8
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$03
   LDY   #$14
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$15
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$16
   STA   (FP),Y
   CLC
   ADC   #$01
   INY
   STA   (FP),Y
   LDY   #$14
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$17
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDA   #<gs8
   LDY   #$18
   STA   (FP),Y
   LDA   #>gs8
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$01
   LDY   #$1A
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$1B
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$1C
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$1D
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   SEC
   SBC   #$01
   LDY   #$1E
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   SBC   #$00
   LDY   #$1F
   STA   (FP),Y
   LDY   #$1A
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$1E
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$1F
   LDA   (FP),Y
   LDY   #$01
   STA   (DPTR),Y
   LDY   #$79
   LDA   (FP),Y
   LDY   #$20
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$1E
   LDA   (FP),Y
   LDY   #$20
   ORA   (FP),Y
   LDY   #$22
   STA   (FP),Y
   LDY   #$1F
   LDA   (FP),Y
   LDY   #$21
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
   ORA   #$00
   BMI   .sx_neg@20
   LDA   #$00
   JMP   .sx_done@21
.sx_neg@20:
   LDA   #$FF
.sx_done@21:
   LDY   #$26
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
   LDA   #$00
   LDY   #$28
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$04
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$2C
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$2D
   STA   (FP),Y
   LDA   g2
   STA   HARGS
   LDA   g2+1
   STA   HARGS+1
   DEY
   LDA   (FP),Y
   STA   HARGS+2
   INY
   LDA   (FP),Y
   STA   HARGS+3
   JSR   udivmod16
   LDA   HARGS+6
   LDY   #$2E
   STA   (FP),Y
   LDA   HARGS+7
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@3
   JMP   .if_end@3
.lb_skip@3:
   LDA   z4
   LDY   #$30
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$78
   LDA   (FP),Y
   LDY   #$32
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$30
   LDA   (FP),Y
   LDY   #$32
   AND   (FP),Y
   LDY   #$34
   STA   (FP),Y
   LDY   #$31
   LDA   (FP),Y
   LDY   #$33
   AND   (FP),Y
   LDY   #$35
   STA   (FP),Y
   LDY   #$29
   LDA   (FP),Y
   LDY   #$36
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$29
   LDA   (FP),Y
   LDY   #$38
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$36
   LDA   (FP),Y
   SEC
   LDY   #$38
   SBC   (FP),Y
   LDY   #$3A
   STA   (FP),Y
   LDY   #$37
   LDA   (FP),Y
   LDY   #$39
   SBC   (FP),Y
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
   ORA   #$00
   BMI   .sx_neg@22
   LDA   #$00
   JMP   .sx_done@23
.sx_neg@22:
   LDA   #$FF
.sx_done@23:
   LDY   #$3E
   STA   (FP),Y
   INY
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$06
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
   LDY   #$3C
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$3D
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   LDY   #$3E
   LDA   (FP),Y
   LDY   #$05
   STA   (SSP),Y
   LDY   #$3F
   LDA   (FP),Y
   LDY   #$06
   STA   (SSP),Y
   JSR   f11
   LDY   #$40
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
   LDY   #$42
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
   LDY   #$41
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$42
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$43
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$44
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   JSR   mix
   LDA   #<gs8
   LDY   #$45
   STA   (FP),Y
   LDA   #>gs8
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$01
   LDY   #$47
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$48
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$49
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$4A
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ASL
   INY
   LDA   (FP),Y
   ROL
   INY
   STA   (FP),Y
   LDA   #$00
   ADC   #$FF
   EOR   #$FF
   INY
   STA   (FP),Y
   LDY   #$47
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
   LDY   #$76
   LDA   (FP),Y
   LDY   #$4D
   STA   (FP),Y
   LDY   #$77
   LDA   (FP),Y
   LDY   #$4E
   STA   (FP),Y
   LDX   #$05
.shift_loop@26:
   LDY   #$4E
   LDA   (FP),Y
   ASL
   LDA   (FP),Y
   ROR
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ROR
   STA   (FP),Y
   DEX
   BNE   .shift_loop@26
   LDY   #$76
   STA   (FP),Y
   LDY   #$4E
   LDA   (FP),Y
   LDY   #$77
   STA   (FP),Y
.if_end@3:
   LDA   #$FF
   LDY   #$4F
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$78
   LDA   (FP),Y
   LDY   #$51
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$4F
   LDA   (FP),Y
   LDY   #$51
   AND   (FP),Y
   LDY   #$53
   STA   (FP),Y
   LDY   #$50
   LDA   (FP),Y
   LDY   #$52
   AND   (FP),Y
   LDY   #$54
   STA   (FP),Y
   LDA   z4
   INY
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
   LDY   #$57
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$06
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
   LDY   #$56
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$57
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   LDY   #$58
   LDA   (FP),Y
   LDY   #$05
   STA   (SSP),Y
   LDY   #$59
   LDA   (FP),Y
   LDY   #$06
   STA   (SSP),Y
   JSR   f11
   LDY   #$5A
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
   LDY   #$5C
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   LDY   #$5C
   LDA   (FP),Y
   LDY   #$02
   STA   (FP),Y
   LDA   ga7+4
   LDY   #$6F
   STA   (FP),Y
   LDA   ga7+5
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lnot_true@33
   LDA   #$00
   JMP   .lnot_end@34
.lnot_true@33:
   LDA   #$01
.lnot_end@34:
   LDY   #$71
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$73
   STA   (FP),Y

   ; epilogue
   PHA
   CLC
   LDA   FP
   ADC   #$7B
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

f26:
   SUBROUTINE

   ; prologue: 6 arg bytes, 108 local bytes
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

   LDA   #$77
   LDY   #$01
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
.loop@3_start:
   LDY   #$07
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
   BVC   .cmp_novf@35
   EOR   #$80
.cmp_novf@35:
   ASL
   LDA   #$00
   ROL
   LDY   #$09
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@5
   JMP   .loop@3_break
.lb_skip@5:
   LDY   #$06
   LDA   (FP),Y
   LDY   #$0B
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   SEC
   DEY
   SBC   (FP),Y
   LDY   #$0D
   STA   (FP),Y
   LDA   #$00
   DEY
   SBC   (FP),Y
   LDY   #$0E
   STA   (FP),Y
   LDA   #$7F
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
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
   LDY   #$72
   CMP   (FP),Y
   BNE   .thread@0
   LDY   #$12
   LDA   (FP),Y
   LDY   #$71
   CMP   (FP),Y
   BEQ   .cmp_true@36
.thread@0:
   LDA   #$00
   JMP   .cmp_end@37
.cmp_true@36:
   LDA   #$01
.cmp_end@37:
   LDY   #$14
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$16
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
   ADC   #$03
   LDY   #$19
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$1A
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$1B
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDA   #$02
   CLC
   DEY
   ADC   (FP),Y
   LDY   #$1E
   STA   (FP),Y
   LDA   #$00
   DEY
   ADC   (FP),Y
   LDY   #$1F
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$20
   STA   (FP),Y
   CLC
   LDA   FP
   ADC   #$02
   INY
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$06
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
   LDY   #$16
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$20
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   LDY   #$21
   LDA   (FP),Y
   LDY   #$05
   STA   (SSP),Y
   LDY   #$22
   LDA   (FP),Y
   LDY   #$06
   STA   (SSP),Y
   JSR   f19
   LDY   #$23
   STA   (FP),Y
   LDY   #$06
   LDA   (FP),Y
   LDY   #$24
   STA   (FP),Y
   CLC
   ADC   #$01
   INY
   STA   (FP),Y
   LDY   #$06
   STA   (FP),Y
   JMP   .loop@3_start
.loop@3_break:
   LDA   #<gs8
   LDY   #$26
   STA   (FP),Y
   LDA   #>gs8
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$03
   LDY   #$28
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$29
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$2A
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
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
   ORA   #$00
   BMI   .sx_neg@39
   LDA   #$00
   JMP   .sx_done@40
.sx_neg@39:
   LDA   #$FF
.sx_done@40:
   LDY   #$2F
   STA   (FP),Y
   LDA   #$FF
   SEC
   LDY   #$2D
   SBC   (FP),Y
   LDA   #$00
   INY
   SBC   (FP),Y
   LDA   #$00
   INY
   SBC   (FP),Y
   BVC   .cmp_novf@41
   EOR   #$80
.cmp_novf@41:
   ASL
   LDA   #$00
   ROL
   EOR   #$01
   LDY   #$30
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$32
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
   ADC   #$03
   LDY   #$35
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$36
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$32
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$6F
   LDA   (FP),Y
   LDY   #$37
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDA   z4
   INY
   STA   (FP),Y
   LDA   z4
   CLC
   ADC   #$01
   INY
   STA   (FP),Y
   STA   z4
   DEY
   LDA   (FP),Y
   LDY   #$3B
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$37
   LDA   (FP),Y
   CLC
   LDY   #$3B
   ADC   (FP),Y
   LDY   #$3D
   STA   (FP),Y
   LDY   #$38
   LDA   (FP),Y
   LDY   #$3C
   ADC   (FP),Y
   LDY   #$3E
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@4
   JMP   .if_end@4
.lb_skip@4:
   LDY   #$71
   LDA   (FP),Y
   EOR   #$81
   LDY   #$3F
   STA   (FP),Y
   LDY   #$72
   LDA   (FP),Y
   LDY   #$40
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$71
   STA   (FP),Y
   LDY   #$40
   LDA   (FP),Y
   LDY   #$72
   STA   (FP),Y
   LDA   g2
   SEC
   LDY   #$3F
   SBC   (FP),Y
   LDY   #$41
   STA   (FP),Y
   LDA   g2+1
   DEY
   SBC   (FP),Y
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
   LDY   #$04
   STA   (SSP),Y
   JSR   mix
.if_end@4:
   LDA   #$00
   LDY   #$47
   STA   (FP),Y
   INY
   STA   (FP),Y
.loop@4_continue:
   LDY   #$48
   LDA   (FP),Y
   INY
   STA   (FP),Y
   CLC
   ADC   #$01
   INY
   STA   (FP),Y
   LDY   #$48
   STA   (FP),Y
   INY
   LDA   (FP),Y
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
   BVC   .cmp_novf@42
   EOR   #$80
.cmp_novf@42:
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
   BEQ   .loop@4_break
   LDA   g1
   INY
   STA   (FP),Y
   LDA   g1
   BMI   .sx_neg@43
   LDA   #$00
   JMP   .sx_done@44
.sx_neg@43:
   LDA   #$FF
.sx_done@44:
   LDY   #$50
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$51
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$52
   STA   (FP),Y
   LDA   g2
   EOR   #$05
   INY
   STA   (FP),Y
   LDA   g2+1
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   g2
   INY
   LDA   (FP),Y
   STA   g2+1
   LDY   #$51
   LDA   (FP),Y
   SEC
   LDY   #$53
   SBC   (FP),Y
   STA   ga7+2
   DEY
   LDA   (FP),Y
   LDY   #$54
   SBC   (FP),Y
   STA   ga7+3
   JMP   .loop@4_continue
.loop@4_break:
   LDY   #$6F
   LDA   (FP),Y
   LDY   #$55
   STA   (FP),Y
   LDA   #$00
   INY
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
   ADC   #$03
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
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$55
   LDA   (FP),Y
   SEC
   LDY   #$5C
   SBC   (FP),Y
   LDY   #$5E
   STA   (FP),Y
   LDY   #$56
   LDA   (FP),Y
   LDY   #$5D
   SBC   (FP),Y
   LDY   #$5F
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$60
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$61
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@45
   LDA   #$00
   JMP   .sx_done@46
.sx_neg@45:
   LDA   #$FF
.sx_done@46:
   LDY   #$62
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
   LDY   #$60
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$61
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$62
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$63
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   JSR   mix
   LDY   #$71
   LDA   (FP),Y
   AND   #$03
   LDY   #$64
   STA   (FP),Y
   LDY   #$72
   LDA   (FP),Y
   LDA   #$00
   LDY   #$65
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$71
   STA   (FP),Y
   LDY   #$65
   LDA   (FP),Y
   LDY   #$72
   STA   (FP),Y
   LDA   #$03
   LDY   #$66
   STA   (FP),Y
   LDA   #$00
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
   LDY   #$64
   LDA   (FP),Y
   LDY   #$68
   EOR   (FP),Y
   LDY   #$6A
   STA   (FP),Y
   LDY   #$65
   LDA   (FP),Y
   LDY   #$69
   EOR   (FP),Y
   LDY   #$6B
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$6C
   STA   (FP),Y
   STA   z4
   LDX   #$00
   LDA   #$01

   ; epilogue
   PHA
   TXA
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
   TAX
   PLA
   RTS

main:
   SUBROUTINE

   ; prologue: 0 arg bytes, 201 local bytes
   SEC
   LDA   SSP
   SBC   #$CB
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$CA
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
   DEY
   LDA   (FP),Y
   LDY   #$08
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$09
   STA   (FP),Y
   LDA   z6
   ORA   z6+1
   BEQ   .cond_else@5
   LDA   z5
   INY
   STA   (FP),Y
   LDA   z5+1
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
   JMP   .cond_end@6
.cond_else@5:
   LDA   z6
   LDY   #$0C
   STA   (FP),Y
   LDA   z6+1
   INY
   STA   (FP),Y
.cond_end@6:
   LDA   g3
   LDY   #$0E
   STA   (FP),Y
   LDA   #$00
   INY
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
   BMI   .sx_neg@47
   LDA   #$00
   JMP   .sx_done@48
.sx_neg@47:
   LDA   #$FF
.sx_done@48:
   LDY   #$12
   STA   (FP),Y
   LDY   #$10
   LDA   (FP),Y
   CLC
   LDY   #$01
   ADC   (FP),Y
   LDY   #$13
   STA   (FP),Y
   LDY   #$11
   LDA   (FP),Y
   LDY   #$02
   ADC   (FP),Y
   LDY   #$14
   STA   (FP),Y
   LDY   #$12
   LDA   (FP),Y
   LDY   #$03
   ADC   (FP),Y
   LDY   #$15
   STA   (FP),Y
   LDY   #$13
   LDA   (FP),Y
   LDY   #$16
   STA   (FP),Y
   LDY   #$14
   LDA   (FP),Y
   LDY   #$17
   STA   (FP),Y
   LDY   #$15
   LDA   (FP),Y
   AND   #$3F
   LDY   #$18
   STA   (FP),Y
   LDY   #$16
   LDA   (FP),Y
   LDY   #$19
   STA   (FP),Y
   LDY   #$17
   LDA   (FP),Y
   LDY   #$1A
   STA   (FP),Y
   LDY   #$18
   LDA   (FP),Y
   EOR   #$20
   LDY   #$1B
   STA   (FP),Y
   LDY   #$19
   LDA   (FP),Y
   LDY   #$1C
   STA   (FP),Y
   LDY   #$1A
   LDA   (FP),Y
   LDY   #$1D
   STA   (FP),Y
   LDY   #$1B
   LDA   (FP),Y
   SEC
   SBC   #$20
   LDY   #$1E
   STA   (FP),Y
   LDY   #$1C
   LDA   (FP),Y
   LDY   #$1F
   STA   (FP),Y
   LDA   g3
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   CLC
   LDA   FP
   ADC   #$05
   INY
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$06
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
   LDY   #$1F
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$21
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   LDY   #$22
   LDA   (FP),Y
   LDY   #$05
   STA   (SSP),Y
   LDY   #$23
   LDA   (FP),Y
   LDY   #$06
   STA   (SSP),Y
   JSR   f19
   LDY   #$24
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDA   #$01
   INY
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
   LDY   #$29
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$2A
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$2B
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$2C
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   SEC
   SBC   #$01
   LDY   #$2D
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   SBC   #$00
   LDY   #$2E
   STA   (FP),Y
   LDY   #$29
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$2D
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$2E
   LDA   (FP),Y
   LDY   #$01
   STA   (DPTR),Y
   LDY   #$2D
   LDA   (FP),Y
   LDY   #$2F
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$30
   STA   (FP),Y
   LDY   #$25
   LDA   (FP),Y
   SEC
   LDY   #$2F
   SBC   (FP),Y
   LDY   #$31
   STA   (FP),Y
   LDY   #$26
   LDA   (FP),Y
   LDY   #$30
   SBC   (FP),Y
   LDY   #$32
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$33
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$34
   STA   (FP),Y
   LDA   #$07
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$33
   LDA   (FP),Y
   LDY   #$35
   AND   (FP),Y
   LDY   #$37
   STA   (FP),Y
   LDY   #$34
   LDA   (FP),Y
   LDY   #$36
   AND   (FP),Y
   LDY   #$38
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CMP   #$01
   BEQ   .cmp_true@49
   LDA   #$00
   JMP   .cmp_end@50
.cmp_true@49:
   LDA   #$01
.cmp_end@50:
   LDY   #$39
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@10
   JMP   .switch@5_break
.lb_skip@10:
   LDY   #$37
   LDA   (FP),Y
   CMP   #$02
   BEQ   .cmp_true@51
   LDA   #$00
   JMP   .cmp_end@52
.cmp_true@51:
   LDA   #$01
.cmp_end@52:
   LDY   #$3B
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@9
   JMP   .switch@5_break
.lb_skip@9:
   LDY   #$37
   LDA   (FP),Y
   CMP   #$05
   BEQ   .cmp_true@53
   LDA   #$00
   JMP   .cmp_end@54
.cmp_true@53:
   LDA   #$01
.cmp_end@54:
   LDY   #$3D
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .case@8
   LDY   #$37
   LDA   (FP),Y
   CMP   #$06
   BEQ   .cmp_true@55
   LDA   #$00
   JMP   .cmp_end@56
.cmp_true@55:
   LDA   #$01
.cmp_end@56:
   LDY   #$3F
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .case@9
   JMP   .default@10
.case@8:
   LDY   #$01
   LDA   (FP),Y
   LDY   #$41
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   LDY   #$42
   STA   (FP),Y
   LDY   #$08
   LDA   (FP),Y
   LDY   #$43
   STA   (FP),Y
   LDY   #$09
   LDA   (FP),Y
   LDY   #$44
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@57
   LDA   #$00
   JMP   .sx_done@58
.sx_neg@57:
   LDA   #$FF
.sx_done@58:
   LDY   #$45
   STA   (FP),Y
   INY
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$06
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDY   #$41
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$42
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$43
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$44
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   LDY   #$45
   LDA   (FP),Y
   LDY   #$05
   STA   (SSP),Y
   LDY   #$46
   LDA   (FP),Y
   LDY   #$06
   STA   (SSP),Y
   JSR   f11
   LDY   #$47
   STA   (FP),Y
   JMP   .switch@5_break
.case@9:
   LDA   g3
   LDY   #$48
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LSR
   LSR
   LDY   #$4B
   STA   (FP),Y
   LDY   #$48
   LDA   (FP),Y
   ROR
   ROR
   ROR
   AND   #$C0
   LDY   #$4A
   STA   (FP),Y
   LDY   #$4C
   STA   (FP),Y
   STA   g3
   LDA   z4
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LSR
   LDY   #$50
   STA   (FP),Y
   LDA   #$00
   ROR
   DEY
   STA   (FP),Y
   LDY   #$51
   STA   (FP),Y
   STA   z4
   JMP   .switch@5_break
.default@10:
   LDY   #$08
   LDA   (FP),Y
   LDY   #$52
   STA   (FP),Y
   LDY   #$09
   LDA   (FP),Y
   LDY   #$53
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@59
   LDA   #$00
   JMP   .sx_done@60
.sx_neg@59:
   LDA   #$FF
.sx_done@60:
   LDY   #$54
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
   ORA   #$00
   BMI   .sx_neg@61
   LDA   #$00
   JMP   .sx_done@62
.sx_neg@61:
   LDA   #$FF
.sx_done@62:
   LDY   #$5A
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$0C
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$58
   LDA   (FP),Y
   SEC
   LDY   #$5C
   SBC   (FP),Y
   LDY   #$60
   STA   (FP),Y
   LDY   #$59
   LDA   (FP),Y
   LDY   #$5D
   SBC   (FP),Y
   LDY   #$61
   STA   (FP),Y
   LDY   #$5A
   LDA   (FP),Y
   LDY   #$5E
   SBC   (FP),Y
   LDY   #$62
   STA   (FP),Y
   LDY   #$5B
   LDA   (FP),Y
   LDY   #$5F
   SBC   (FP),Y
   LDY   #$63
   STA   (FP),Y
   LDY   #$52
   LDA   (FP),Y
   CLC
   LDY   #$60
   ADC   (FP),Y
   LDY   #$64
   STA   (FP),Y
   LDY   #$53
   LDA   (FP),Y
   LDY   #$61
   ADC   (FP),Y
   LDY   #$65
   STA   (FP),Y
   LDY   #$54
   LDA   (FP),Y
   LDY   #$62
   ADC   (FP),Y
   LDY   #$66
   STA   (FP),Y
   LDY   #$55
   LDA   (FP),Y
   LDY   #$63
   ADC   (FP),Y
   LDY   #$67
   STA   (FP),Y
   LDY   #$64
   LDA   (FP),Y
   LDY   #$68
   STA   (FP),Y
   STA   g1
   LDA   z4
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LSR
   LSR
   LSR
   LDY   #$6C
   STA   (FP),Y
   LDY   #$69
   LDA   (FP),Y
   ASL
   ASL
   ASL
   ASL
   ASL
   LDY   #$6B
   STA   (FP),Y
   LDY   #$6D
   STA   (FP),Y
   STA   z4
.switch@5_break:
   LDA   gp9
   STA   DPTR
   LDA   gp9+1
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$6E
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ORA   #$05
   LDY   #$71
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$72
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$73
   STA   (FP),Y
   LDA   gp9
   STA   DPTR
   LDA   gp9+1
   STA   DPTR+1
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$73
   LDA   (FP),Y
   INY
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
   LDY   #$74
   LDA   (FP),Y
   LDY   #$76
   AND   (FP),Y
   LDY   #$78
   STA   (FP),Y
   LDY   #$75
   LDA   (FP),Y
   LDY   #$77
   AND   (FP),Y
   LDY   #$79
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CMP   #$04
   BEQ   .cmp_true@63
   LDA   #$00
   JMP   .cmp_end@64
.cmp_true@63:
   LDA   #$01
.cmp_end@64:
   LDY   #$7A
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .case@12
   LDY   #$78
   LDA   (FP),Y
   CMP   #$06
   BEQ   .cmp_true@65
   LDA   #$00
   JMP   .cmp_end@66
.cmp_true@65:
   LDA   #$01
.cmp_end@66:
   LDY   #$7C
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@8
   JMP   .case@14
.lb_skip@8:
   LDY   #$78
   LDA   (FP),Y
   CMP   #$07
   BEQ   .cmp_true@67
   LDA   #$00
   JMP   .cmp_end@68
.cmp_true@67:
   LDA   #$01
.cmp_end@68:
   LDY   #$7E
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@7
   JMP   .case@15
.lb_skip@7:
   JMP   .default@16
.case@12:
   LDA   #$00
   LDY   #$80
   STA   (FP),Y
   INY
   STA   (FP),Y
.loop@13_start:
   LDY   #$82
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
   BVC   .cmp_novf@69
   EOR   #$80
.cmp_novf@69:
   ASL
   LDA   #$00
   ROL
   LDY   #$84
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@6
   JMP   .switch@11_break
.lb_skip@6:
   LDA   g3
   LDY   #$86
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDA   z4
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$86
   LDA   (FP),Y
   LDY   #$88
   EOR   (FP),Y
   LDY   #$8A
   STA   (FP),Y
   LDY   #$87
   LDA   (FP),Y
   LDY   #$89
   EOR   (FP),Y
   LDY   #$8B
   STA   (FP),Y
   LDY   #$05
   LDA   (FP),Y
   LDY   #$8C
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$8E
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$8F
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@70
   LDA   #$00
   JMP   .sx_done@71
.sx_neg@70:
   LDA   #$FF
.sx_done@71:
   LDY   #$90
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
   LDA   #$00
   STA   HARGS+3
   LDY   #$01
   LDA   (FP),Y
   STA   HARGS+4
   INY
   LDA   (FP),Y
   STA   HARGS+5
   INY
   LDA   (FP),Y
   STA   HARGS+6
   LDA   #$00
   STA   HARGS+7
   JSR   mul32
   LDA   HARGS+8
   LDY   #$91
   STA   (FP),Y
   LDA   HARGS+9
   INY
   STA   (FP),Y
   LDA   HARGS+10
   INY
   STA   (FP),Y
   LDY   #$91
   LDA   (FP),Y
   LDY   #$94
   STA   (FP),Y
   LDY   #$92
   LDA   (FP),Y
   LDY   #$95
   STA   (FP),Y
   LDY   #$93
   LDA   (FP),Y
   AND   #$3F
   LDY   #$96
   STA   (FP),Y
   LDY   #$94
   LDA   (FP),Y
   LDY   #$97
   STA   (FP),Y
   LDY   #$95
   LDA   (FP),Y
   LDY   #$98
   STA   (FP),Y
   LDY   #$96
   LDA   (FP),Y
   EOR   #$20
   LDY   #$99
   STA   (FP),Y
   LDY   #$97
   LDA   (FP),Y
   LDY   #$9A
   STA   (FP),Y
   LDY   #$98
   LDA   (FP),Y
   LDY   #$9B
   STA   (FP),Y
   LDY   #$99
   LDA   (FP),Y
   SEC
   SBC   #$20
   LDY   #$9C
   STA   (FP),Y
   LDY   #$9A
   LDA   (FP),Y
   LDY   #$9D
   STA   (FP),Y
   LDY   #$81
   LDA   (FP),Y
   BEQ   .lnot_true@72
   LDA   #$00
   JMP   .lnot_end@73
.lnot_true@72:
   LDA   #$01
.lnot_end@73:
   LDY   #$9E
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$A0
   STA   (FP),Y
   CLC
   LDA   FP
   ADC   #$05
   INY
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$06
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDY   #$8A
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$8B
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$9D
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$A0
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   LDY   #$A1
   LDA   (FP),Y
   LDY   #$05
   STA   (SSP),Y
   LDY   #$A2
   LDA   (FP),Y
   LDY   #$06
   STA   (SSP),Y
   JSR   f19
   LDY   #$A3
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
   LDY   #$A4
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$A5
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$A6
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$A7
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   JSR   mix
   LDY   #$81
   LDA   (FP),Y
   LDY   #$A8
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$AA
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$AB
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@74
   LDA   #$00
   JMP   .sx_done@75
.sx_neg@74:
   LDA   #$FF
.sx_done@75:
   LDY   #$AC
   STA   (FP),Y
   LDY   #$03
   CMP   (FP),Y
   BNE   .cmp_true@76
   LDY   #$AB
   LDA   (FP),Y
   LDY   #$02
   CMP   (FP),Y
   BNE   .cmp_true@76
   LDY   #$AA
   LDA   (FP),Y
   LDY   #$01
   CMP   (FP),Y
   BNE   .cmp_true@76
   LDA   #$00
   JMP   .cmp_end@77
.cmp_true@76:
   LDA   #$01
.cmp_end@77:
   LDY   #$AD
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$AF
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$B0
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@79
   LDA   #$00
   JMP   .sx_done@80
.sx_neg@79:
   LDA   #$FF
.sx_done@80:
   LDY   #$B1
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
   LDY   #$AF
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$B0
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$B1
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$B2
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   JSR   mix
   LDA   g3
   LDY   #$B3
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LSR
   LSR
   LSR
   LDY   #$B6
   STA   (FP),Y
   LDY   #$B3
   LDA   (FP),Y
   ASL
   ASL
   ASL
   ASL
   ASL
   LDY   #$B5
   STA   (FP),Y
   LDY   #$B7
   STA   (FP),Y
   STA   g3
   LDA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$81
   LDA   (FP),Y
   LDY   #$BA
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$B8
   LDA   (FP),Y
   LDY   #$BA
   AND   (FP),Y
   LDY   #$BC
   STA   (FP),Y
   LDY   #$B9
   LDA   (FP),Y
   LDY   #$BB
   AND   (FP),Y
   LDY   #$BD
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$BE
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   AND   #$3F
   LDY   #$BF
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$C0
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   EOR   #$20
   LDY   #$C1
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$C2
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   SEC
   SBC   #$20
   LDY   #$C3
   STA   (FP),Y
   LDA   gp10
   STA   DPTR
   LDA   gp10+1
   STA   DPTR+1
   DEY
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$C3
   LDA   (FP),Y
   LDY   #$01
   STA   (DPTR),Y
   LDY   #$81
   LDA   (FP),Y
   LDY   #$C4
   STA   (FP),Y
   CLC
   ADC   #$01
   INY
   STA   (FP),Y
   LDY   #$81
   STA   (FP),Y
   JMP   .loop@13_start
.case@14:
.case@15:
.default@16:
.switch@11_break:
   JSR   fin0
   JSR   fin1
   JSR   fin2
   LDA   acc
   LDY   #$C6
   STA   (FP),Y
   LDA   acc+1
   AND   #$7F
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$C8
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$C9
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
   ADC   #$CB
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$CA
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
   DC.B  $01

g2:
   DC.W  $0080

g3:
   DS.B  1

ga7:
   DS.B  4
   DC.W  $000C
   DC.W  $0003

gs8:
   DS.B  1
   DC.W  $FFFF
   DC.B  $FF

gp9:
   DC.W  g3

gp10:
   DC.W  z6

@3.s16:
   DS.B  2

__zp_init:
   SUBROUTINE

   LDA   #$00
   STA   z4
   STA   z6+1
   LDA   #$FF
   STA   z5
   LDA   #$7F
   STA   z5+1
   LDA   #$0D
   STA   z6
   RTS
