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

   ; prologue: 0 arg bytes, 25 local bytes
   SEC
   LDA   SSP
   SBC   #$1B
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$1A
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
   LDA   g2
   LDY   #$07
   STA   (FP),Y
   LDA   g2
   BMI   .sx_neg@0
   LDA   #$00
   JMP   .sx_done@1
.sx_neg@0:
   LDA   #$FF
.sx_done@1:
   LDY   #$08
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
   LDA   z3
   LDY   #$0B
   STA   (FP),Y
   LDA   z3
   BMI   .sx_neg@2
   LDA   #$00
   JMP   .sx_done@3
.sx_neg@2:
   LDA   #$FF
.sx_done@3:
   LDY   #$0C
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
   LDY   #$0B
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$0C
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$0D
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$0E
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   JSR   mix
   LDA   ga4
   LDY   #$0F
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
   LDY   #$0F
   LDA   (FP),Y
   LDY   #$16
   STA   (FP),Y
   LDY   #$10
   LDA   (FP),Y
   LDY   #$17
   STA   (FP),Y
   LDY   #$11
   LDA   (FP),Y
   LDY   #$18
   STA   (FP),Y
   LDY   #$12
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

   ; epilogue
   CLC
   LDA   FP
   ADC   #$1B
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$1A
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

   ; prologue: 0 arg bytes, 45 local bytes
   SEC
   LDA   SSP
   SBC   #$2F
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$2E
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   LDA   ga4+7
   LDY   #$01
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
   LDA   ga4+14
   LDY   #$0C
   STA   (FP),Y
   LDA   ga4+15
   INY
   STA   (FP),Y
   LDA   ga4+16
   INY
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
   LDA   ga4+21
   LDY   #$17
   STA   (FP),Y
   LDA   ga4+22
   INY
   STA   (FP),Y
   LDA   ga4+23
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
   LDA   #<gs5
   LDY   #$22
   STA   (FP),Y
   LDA   #>gs5
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$01
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
   LDY   #$02
   LDA   (DPTR),Y
   LDY   #$28
   STA   (FP),Y
   LDY   #$03
   LDA   (DPTR),Y
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

   ; epilogue
   CLC
   LDA   FP
   ADC   #$2F
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$2E
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

f7:
   SUBROUTINE

   ; prologue: 4 arg bytes, 101 local bytes
   SEC
   LDA   SSP
   SBC   #$67
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$66
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
   LDA   #$00
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
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
.loop@0_start:
   LDY   #$0E
   LDA   (FP),Y
   BEQ   .cond_else@0
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$11
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$12
   STA   (FP),Y
   JMP   .cond_end@1
.cond_else@0:
   LDY   #$13
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$11
   STA   (FP),Y
   LDY   #$14
   LDA   (FP),Y
   LDY   #$12
   STA   (FP),Y
.cond_end@1:
   LDY   #$11
   LDA   (FP),Y
   LDY   #$68
   STA   (FP),Y
   LDY   #$12
   LDA   (FP),Y
   LDY   #$69
   STA   (FP),Y
   LDA   g1
   LDY   #$15
   STA   (FP),Y
   LDA   g1+1
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   HARGS
   INY
   LDA   (FP),Y
   STA   HARGS+1
   LDA   #$03
   STA   HARGS+2
   LDA   #$00
   STA   HARGS+3
   JSR   mul16
   LDA   HARGS+4
   LDY   #$17
   STA   (FP),Y
   LDA   HARGS+5
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
   INY
   ORA   (FP),Y
   BNE   .lb_skip@2
   JMP   .if_else@3
.lb_skip@2:
   LDY   #$68
   LDA   (FP),Y
   AND   #$03
   LDY   #$19
   STA   (FP),Y
   LDY   #$69
   LDA   (FP),Y
   LDA   #$00
   LDY   #$1A
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$68
   STA   (FP),Y
   LDY   #$1A
   LDA   (FP),Y
   LDY   #$69
   STA   (FP),Y
   INY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$1B
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$1C
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ORA   #$5A
   LDY   #$1D
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$1E
   STA   (FP),Y
   LDY   #$6A
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$1D
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$1E
   LDA   (FP),Y
   LDY   #$01
   STA   (DPTR),Y
   LDY   #$1D
   LDA   (FP),Y
   LDY   #$1F
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$20
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@4
   LDA   #$00
   JMP   .sx_done@5
.sx_neg@4:
   LDA   #$FF
.sx_done@5:
   LDY   #$21
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
.if_else@3:
   LDY   #$0E
   LDA   (FP),Y
   CLC
   ADC   #$01
   LDY   #$2B
   STA   (FP),Y
   LDY   #$0E
   STA   (FP),Y
   LDY   #$2C
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
   BVC   .cmp_novf@6
   EOR   #$80
.cmp_novf@6:
   ASL
   LDA   #$00
   ROL
   LDY   #$2E
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@1
   JMP   .loop@0_start
.lb_skip@1:
   LDA   @3.s12
   CLC
   ADC   #$01
   LDY   #$30
   STA   (FP),Y
   LDA   @3.s12+1
   ADC   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   @3.s12
   INY
   LDA   (FP),Y
   STA   @3.s12+1
   DEY
   LDA   (FP),Y
   LDY   #$32
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$33
   STA   (FP),Y
   LDA   #$07
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
   CMP   #$01
   BEQ   .cmp_true@7
   LDA   #$00
   JMP   .cmp_end@8
.cmp_true@7:
   LDA   #$01
.cmp_end@8:
   LDY   #$38
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@0
   JMP   .switch@1_break
.lb_skip@0:
   LDA   g2
   LDY   #$3A
   STA   (FP),Y
   LDA   g2
   BMI   .sx_neg@9
   LDA   #$00
   JMP   .sx_done@10
.sx_neg@9:
   LDA   #$FF
.sx_done@10:
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
   BMI   .sx_neg@11
   LDA   #$00
   JMP   .sx_done@12
.sx_neg@11:
   LDA   #$FF
.sx_done@12:
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
   LDY   #$44
   LDA   (FP),Y
   LDY   #$4C
   STA   (FP),Y
   LDY   #$45
   LDA   (FP),Y
   LDY   #$4D
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   g1
   INY
   LDA   (FP),Y
   STA   g1+1
.switch@1_break:
   LDY   #$68
   LDA   (FP),Y
   CLC
   ADC   #$01
   LDY   #$4E
   STA   (FP),Y
   LDY   #$69
   LDA   (FP),Y
   ADC   #$00
   LDY   #$4F
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$68
   STA   (FP),Y
   LDY   #$4F
   LDA   (FP),Y
   LDY   #$69
   STA   (FP),Y
   LDY   #$03
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .cond_else@4
   LDA   ga4
   LDY   #$50
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
   LDY   #$50
   LDA   (FP),Y
   LDY   #$57
   STA   (FP),Y
   LDY   #$51
   LDA   (FP),Y
   LDY   #$58
   STA   (FP),Y
   LDY   #$52
   LDA   (FP),Y
   LDY   #$59
   STA   (FP),Y
   LDY   #$53
   LDA   (FP),Y
   LDY   #$5A
   STA   (FP),Y
   LDY   #$54
   LDA   (FP),Y
   LDY   #$5B
   STA   (FP),Y
   LDY   #$55
   LDA   (FP),Y
   LDY   #$5C
   STA   (FP),Y
   LDY   #$56
   LDA   (FP),Y
   LDY   #$5D
   STA   (FP),Y
   JMP   .cond_end@5
.cond_else@4:
   LDA   @3.s12
   LDY   #$5E
   STA   (FP),Y
   LDA   @3.s12+1
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
   LDY   #$5E
   LDA   (FP),Y
   LDY   #$57
   STA   (FP),Y
   LDY   #$5F
   LDA   (FP),Y
   LDY   #$58
   STA   (FP),Y
   LDY   #$60
   LDA   (FP),Y
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
.cond_end@5:
   LDY   #$57
   LDA   (FP),Y
   LDY   #$65
   STA   (FP),Y

   ; epilogue
   PHA
   CLC
   LDA   FP
   ADC   #$6B
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

   ; prologue: 5 arg bytes, 104 local bytes
   SEC
   LDA   SSP
   SBC   #$6A
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$69
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
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$07
   LDA   (FP),Y
   LDY   #$0B
   STA   (FP),Y
   LDY   #$08
   LDA   (FP),Y
   LDY   #$0C
   STA   (FP),Y
   LDY   #$09
   LDA   (FP),Y
   LDY   #$0D
   STA   (FP),Y
   LDY   #$0A
   LDA   (FP),Y
   LDY   #$0E
   STA   (FP),Y
   LDA   gp6
   STA   DPTR
   LDA   gp6+1
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$0F
   STA   (FP),Y
   INY
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@13
   LDA   #$00
   JMP   .sx_done@14
.sx_neg@13:
   LDA   #$FF
.sx_done@14:
   LDY   #$11
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$12
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$13
   STA   (FP),Y
   CLC
   LDA   FP
   ADC   #$03
   INY
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
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
   JSR   f7
   LDY   #$16
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
   LDY   #$17
   LDA   (FP),Y
   LDY   #$0B
   STA   (FP),Y
   LDY   #$18
   LDA   (FP),Y
   LDY   #$0C
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   ga4
   LDY   #$1B
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
   LDA   g2
   INY
   STA   (FP),Y
   LDA   g2
   BMI   .sx_neg@15
   LDA   #$00
   JMP   .sx_done@16
.sx_neg@15:
   LDA   #$FF
.sx_done@16:
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
   BMI   .sx_neg@17
   LDA   #$00
   JMP   .sx_done@18
.sx_neg@17:
   LDA   #$FF
.sx_done@18:
   LDY   #$26
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$24
   LDA   (FP),Y
   LDY   #$2B
   STA   (FP),Y
   LDY   #$25
   LDA   (FP),Y
   LDY   #$2C
   STA   (FP),Y
   LDY   #$26
   LDA   (FP),Y
   LDY   #$2D
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   AND   #$7F
   INY
   STA   (FP),Y
   LDY   #$1B
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
   LDA   #$00
   STA   HARGS+7
   LDY   #$2B
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
   LDA   #$00
   STA   HARGS+15
   JSR   mul64
   LDA   HARGS+16
   LDY   #$32
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
   LDY   #$32
   LDA   (FP),Y
   LDY   #$39
   STA   (FP),Y
   LDY   #$33
   LDA   (FP),Y
   LDY   #$3A
   STA   (FP),Y
   LDY   #$34
   LDA   (FP),Y
   LDY   #$3B
   STA   (FP),Y
   LDY   #$35
   LDA   (FP),Y
   LDY   #$3C
   STA   (FP),Y
   LDY   #$36
   LDA   (FP),Y
   LDY   #$3D
   STA   (FP),Y
   LDY   #$37
   LDA   (FP),Y
   LDY   #$3E
   STA   (FP),Y
   LDY   #$38
   LDA   (FP),Y
   AND   #$7F
   LDY   #$3F
   STA   (FP),Y
   LDY   #$39
   LDA   (FP),Y
   LDY   #$40
   STA   (FP),Y
   LDY   #$3A
   LDA   (FP),Y
   LDY   #$41
   STA   (FP),Y
   LDY   #$3B
   LDA   (FP),Y
   LDY   #$42
   STA   (FP),Y
   LDY   #$3C
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
   LDY   #$40
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$41
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$42
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$43
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   JSR   mix
   LDA   #$00
   LDY   #$44
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   CLC
   ADC   #$01
   INY
   STA   (FP),Y
   LDY   #$45
   STA   (FP),Y
   INY
   LDA   (FP),Y
   LDY   #$48
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
   BVC   .cmp_novf@19
   EOR   #$80
.cmp_novf@19:
   ASL
   LDA   #$00
   ROL
   LDY   #$4A
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@3
   JMP   .loop@3_break
.lb_skip@3:
   LDA   #$01
   LDY   #$4C
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   ORA   #$00
   BPL   .sx_done@21
   LDA   #$FF
.sx_done@21:
   LDY   #$4E
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$0B
   LDA   (FP),Y
   CLC
   LDY   #$4C
   ADC   (FP),Y
   LDY   #$50
   STA   (FP),Y
   LDY   #$0C
   LDA   (FP),Y
   LDY   #$4D
   ADC   (FP),Y
   LDY   #$51
   STA   (FP),Y
   LDY   #$0D
   LDA   (FP),Y
   LDY   #$4E
   ADC   (FP),Y
   LDY   #$52
   STA   (FP),Y
   LDY   #$0E
   LDA   (FP),Y
   LDY   #$4F
   ADC   (FP),Y
   LDY   #$53
   STA   (FP),Y
   LDY   #$50
   LDA   (FP),Y
   LDY   #$0B
   STA   (FP),Y
   LDY   #$51
   LDA   (FP),Y
   LDY   #$0C
   STA   (FP),Y
   LDY   #$52
   LDA   (FP),Y
   LDY   #$0D
   STA   (FP),Y
   LDY   #$53
   LDA   (FP),Y
   LDY   #$0E
   STA   (FP),Y
   LDY   #$45
   LDA   (FP),Y
   LDY   #$54
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$56
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$57
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@22
   LDA   #$00
   JMP   .sx_done@23
.sx_neg@22:
   LDA   #$FF
.sx_done@23:
   LDY   #$58
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$0B
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
   LDY   #$56
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
   LDY   #$5A
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
   LDY   #$5A
   LDA   (FP),Y
   LDY   #$5E
   STA   (FP),Y
   LDY   #$5B
   LDA   (FP),Y
   LDY   #$5F
   STA   (FP),Y
   CLC
   LDA   FP
   ADC   #$03
   INY
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
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
   JSR   f7
   LDY   #$62
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$06
   STA   (FP),Y
.loop@3_break:
   LDY   #$03
   LDA   (FP),Y
   LDY   #$64
   STA   (FP),Y
   LDY   #$04
   LDA   (FP),Y
   LDY   #$65
   STA   (FP),Y
   LDY   #$6E
   LDA   (FP),Y
   LDY   #$64
   AND   (FP),Y
   LDY   #$66
   STA   (FP),Y
   LDY   #$6F
   LDA   (FP),Y
   LDY   #$65
   AND   (FP),Y
   LDY   #$67
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$68
   STA   (FP),Y

   ; epilogue
   PHA
   CLC
   LDA   FP
   ADC   #$6F
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

f22:
   SUBROUTINE

   ; prologue: 6 arg bytes, 143 local bytes
   SEC
   LDA   SSP
   SBC   #$91
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$90
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   LDA   #$2B
   LDY   #$01
   STA   (FP),Y
   LDA   #$27
   INY
   STA   (FP),Y
   LDA   #$77
   INY
   STA   (FP),Y
   LDA   #$6B
   INY
   STA   (FP),Y
   LDA   #$11
   INY
   STA   (FP),Y
   LDA   #$30
   INY
   STA   (FP),Y
   LDA   #$13
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
   LDY   #$0A
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$0B
   STA   (FP),Y
   LDY   #$93
   LDA   (FP),Y
   LDY   #$0C
   STA   (FP),Y
   LDY   #$94
   LDA   (FP),Y
   LDY   #$0D
   STA   (FP),Y
   CLC
   LDA   FP
   ADC   #$0A
   INY
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
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
   JSR   f7
   LDY   #$10
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
   LDY   #$92
   LDA   (FP),Y
   LDY   #$15
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@24
   LDA   #$00
   JMP   .sx_done@25
.sx_neg@24:
   LDA   #$FF
.sx_done@25:
   LDY   #$16
   STA   (FP),Y
   LSR
   DEY
   LDA   (FP),Y
   ROR
   LDY   #$18
   STA   (FP),Y
   LDA   #$00
   ROR
   DEY
   STA   (FP),Y
   LDY   #$19
   STA   (FP),Y
   LDY   #$92
   STA   (FP),Y
   LDY   #$1A
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@26
   LDA   #$00
   JMP   .sx_done@27
.sx_neg@26:
   LDA   #$FF
.sx_done@27:
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
   LDA   g2
   LDY   #$1E
   STA   (FP),Y
   LDA   g2
   BMI   .sx_neg@28
   LDA   #$00
   JMP   .sx_done@29
.sx_neg@28:
   LDA   #$FF
.sx_done@29:
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
   LDA   @17.s28
   SEC
   DEY
   SBC   (FP),Y
   LDY   #$22
   STA   (FP),Y
   LDA   @17.s28+1
   DEY
   SBC   (FP),Y
   LDY   #$23
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@6
   JMP   .if_end@6
.lb_skip@6:
   LDA   #$00
   LDY   #$24
   STA   (FP),Y
   INY
   STA   (FP),Y
.loop@4_start:
   LDY   #$26
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
   BVC   .cmp_novf@30
   EOR   #$80
.cmp_novf@30:
   ASL
   LDA   #$00
   ROL
   LDY   #$28
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .loop@4_break
   LDY   #$25
   LDA   (FP),Y
   LDY   #$2E
   STA   (FP),Y
   CLC
   ADC   #$01
   INY
   STA   (FP),Y
   LDY   #$25
   STA   (FP),Y
   JMP   .loop@4_start
.loop@4_break:
   LDY   #$97
   LDA   (FP),Y
   LDY   #$30
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   EOR   #$5A
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
   LDY   #$97
   STA   (FP),Y
   LDA   g2
   LDY   #$35
   STA   (FP),Y
   LDA   g2
   BMI   .sx_neg@31
   LDA   #$00
   JMP   .sx_done@32
.sx_neg@31:
   LDA   #$FF
.sx_done@32:
   LDY   #$36
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$37
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$38
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   SBC   @17.s28
   INY
   LDA   (FP),Y
   SBC   @17.s28+1
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
   BNE   .lb_skip@5
   JMP   .if_else@8
.lb_skip@5:
   LDA   @17.s28
   ORA   @17.s28+1
   BNE   .lb_skip@4
   JMP   .if_end@7
.lb_skip@4:
   LDY   #$01
   LDA   (FP),Y
   LDY   #$3B
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   LDY   #$3C
   STA   (FP),Y
   LDY   #$03
   LDA   (FP),Y
   LDY   #$3D
   STA   (FP),Y
   LDY   #$04
   LDA   (FP),Y
   LDY   #$3E
   STA   (FP),Y
   LDY   #$05
   LDA   (FP),Y
   LDY   #$3F
   STA   (FP),Y
   LDY   #$06
   LDA   (FP),Y
   LDY   #$40
   STA   (FP),Y
   LDY   #$07
   LDA   (FP),Y
   LDY   #$41
   STA   (FP),Y
   LDY   #$3B
   LDA   (FP),Y
   SEC
   SBC   #$5A
   LDY   #$42
   STA   (FP),Y
   LDY   #$3C
   LDA   (FP),Y
   SBC   #$00
   LDY   #$43
   STA   (FP),Y
   LDY   #$3D
   LDA   (FP),Y
   SBC   #$00
   LDY   #$44
   STA   (FP),Y
   LDY   #$3E
   LDA   (FP),Y
   SBC   #$00
   LDY   #$45
   STA   (FP),Y
   LDY   #$3F
   LDA   (FP),Y
   SBC   #$00
   LDY   #$46
   STA   (FP),Y
   LDY   #$40
   LDA   (FP),Y
   SBC   #$00
   LDY   #$47
   STA   (FP),Y
   LDY   #$41
   LDA   (FP),Y
   SBC   #$00
   LDY   #$48
   STA   (FP),Y
   LDY   #$42
   LDA   (FP),Y
   LDY   #$49
   STA   (FP),Y
   LDY   #$43
   LDA   (FP),Y
   LDY   #$4A
   STA   (FP),Y
   LDY   #$44
   LDA   (FP),Y
   LDY   #$4B
   STA   (FP),Y
   LDY   #$45
   LDA   (FP),Y
   LDY   #$4C
   STA   (FP),Y
   LDY   #$46
   LDA   (FP),Y
   LDY   #$4D
   STA   (FP),Y
   LDY   #$47
   LDA   (FP),Y
   LDY   #$4E
   STA   (FP),Y
   LDY   #$48
   LDA   (FP),Y
   AND   #$7F
   LDY   #$4F
   STA   (FP),Y
   LDY   #$49
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   LDY   #$4A
   LDA   (FP),Y
   LDY   #$02
   STA   (FP),Y
   LDY   #$4B
   LDA   (FP),Y
   LDY   #$03
   STA   (FP),Y
   LDY   #$4C
   LDA   (FP),Y
   LDY   #$04
   STA   (FP),Y
   LDY   #$4D
   LDA   (FP),Y
   LDY   #$05
   STA   (FP),Y
   LDY   #$4E
   LDA   (FP),Y
   LDY   #$06
   STA   (FP),Y
   LDY   #$4F
   LDA   (FP),Y
   LDY   #$07
   STA   (FP),Y
   LDY   #$49
   LDA   (FP),Y
   LDY   #$50
   STA   (FP),Y
   LDY   #$4A
   LDA   (FP),Y
   LDY   #$51
   STA   (FP),Y
   LDY   #$4B
   LDA   (FP),Y
   LDY   #$52
   STA   (FP),Y
   LDY   #$4C
   LDA   (FP),Y
   LDY   #$53
   STA   (FP),Y
   LDY   #$4D
   LDA   (FP),Y
   LDY   #$54
   STA   (FP),Y
   LDY   #$4E
   LDA   (FP),Y
   LDY   #$55
   STA   (FP),Y
   LDY   #$4F
   LDA   (FP),Y
   AND   #$07
   LDY   #$56
   STA   (FP),Y
   LDY   #$50
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

   ; epilogue
   CLC
   LDA   FP
   ADC   #$97
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$90
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   RTS
.if_else@8:
   LDA   gp6
   STA   DPTR
   LDA   gp6+1
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$57
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
   LDY   #$59
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
   BEQ   .cmp_true@35
   LDA   #$00
   JMP   .cmp_end@36
.cmp_true@35:
   LDA   #$01
.cmp_end@36:
   LDY   #$5E
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .case@6
   LDY   #$5C
   LDA   (FP),Y
   CMP   #$04
   BEQ   .cmp_true@37
   LDA   #$00
   JMP   .cmp_end@38
.cmp_true@37:
   LDA   #$01
.cmp_end@38:
   LDY   #$60
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .case@7
   LDY   #$5C
   LDA   (FP),Y
   CMP   #$06
   BEQ   .cmp_true@39
   LDA   #$00
   JMP   .cmp_end@40
.cmp_true@39:
   LDA   #$01
.cmp_end@40:
   LDY   #$62
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .case@8
   JMP   .default@9
.case@6:
   LDY   #$0A
   LDA   (FP),Y
   LDY   #$64
   STA   (FP),Y
   LDY   #$0B
   LDA   (FP),Y
   LDY   #$65
   STA   (FP),Y
   LDY   #$0A
   LDA   (FP),Y
   SEC
   SBC   #$01
   LDY   #$66
   STA   (FP),Y
   LDY   #$0B
   LDA   (FP),Y
   SBC   #$00
   LDY   #$67
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$0A
   STA   (FP),Y
   LDY   #$67
   LDA   (FP),Y
   LDY   #$0B
   STA   (FP),Y
.case@7:
.case@8:
.default@9:
.if_end@7:
.if_end@6:
   LDA   #$01
   LDY   #$68
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$6B
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$6C
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@41
   LDA   #$00
   JMP   .sx_done@42
.sx_neg@41:
   LDA   #$FF
.sx_done@42:
   LDY   #$6D
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$6B
   LDA   (FP),Y
   LDY   #$72
   STA   (FP),Y
   LDY   #$6C
   LDA   (FP),Y
   LDY   #$73
   STA   (FP),Y
   LDY   #$6D
   LDA   (FP),Y
   LDY   #$74
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   AND   #$7F
   INY
   STA   (FP),Y
   LDA   ga4
   INY
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
   LDY   #$78
   LDA   (FP),Y
   LDY   #$7F
   CMP   (FP),Y
   BNE   .thread@0
   LDY   #$77
   LDA   (FP),Y
   LDY   #$7E
   CMP   (FP),Y
   BNE   .thread@0
   DEY
   CMP   (FP),Y
   BNE   .thread@0
   DEY
   CMP   (FP),Y
   BNE   .thread@0
   DEY
   CMP   (FP),Y
   BNE   .thread@0
   LDY   #$73
   LDA   (FP),Y
   LDY   #$7A
   CMP   (FP),Y
   BNE   .thread@0
   LDY   #$72
   LDA   (FP),Y
   LDY   #$79
   CMP   (FP),Y
   BEQ   .cmp_true@43
.thread@0:
   LDA   #$00
   JMP   .cmp_end@44
.cmp_true@43:
   LDA   #$01
.cmp_end@44:
   LDY   #$80
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$82
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$83
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@46
   LDA   #$00
   JMP   .sx_done@47
.sx_neg@46:
   LDA   #$FF
.sx_done@47:
   LDY   #$84
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$82
   LDA   (FP),Y
   LDY   #$89
   STA   (FP),Y
   LDY   #$83
   LDA   (FP),Y
   LDY   #$8A
   STA   (FP),Y
   LDY   #$84
   LDA   (FP),Y
   LDY   #$8B
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   AND   #$07
   INY
   STA   (FP),Y
   LDY   #$89
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

   ; epilogue
   CLC
   LDA   FP
   ADC   #$97
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

   ; prologue: 0 arg bytes, 200 local bytes
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

   LDA   #$01
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
   LDA   #$02
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
   LDA   gp6
   STA   DPTR
   LDA   gp6+1
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$09
   STA   (FP),Y
   INY
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@48
   LDA   #$00
   JMP   .sx_done@49
.sx_neg@48:
   LDA   #$FF
.sx_done@49:
   LDY   #$0B
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
   BMI   .sx_neg@50
   LDA   #$00
   JMP   .sx_done@51
.sx_neg@50:
   LDA   #$FF
.sx_done@51:
   LDY   #$0E
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
   ADC   #$01
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
   LDY   #$0F
   LDA   (FP),Y
   LDY   #$17
   CMP   (FP),Y
   BNE   .thread@1
   LDY   #$0E
   LDA   (FP),Y
   LDY   #$16
   CMP   (FP),Y
   BNE   .thread@1
   LDY   #$0D
   LDA   (FP),Y
   LDY   #$15
   CMP   (FP),Y
   BNE   .thread@1
   LDY   #$0C
   LDA   (FP),Y
   LDY   #$14
   CMP   (FP),Y
   BEQ   .cmp_true@52
.thread@1:
   LDA   #$00
   JMP   .cmp_end@53
.cmp_true@52:
   LDA   #$01
.cmp_end@53:
   LDY   #$18
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
   ORA   #$00
   BMI   .sx_neg@55
   LDA   #$00
   JMP   .sx_done@56
.sx_neg@55:
   LDA   #$FF
.sx_done@56:
   LDY   #$1C
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
   LDA   z3
   LDY   #$1E
   STA   (FP),Y
   LDA   z3
   BMI   .sx_neg@57
   LDA   #$00
   JMP   .sx_done@58
.sx_neg@57:
   LDA   #$FF
.sx_done@58:
   LDY   #$1F
   STA   (FP),Y
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
   ADC   #$01
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
   LDY   #$1E
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$1F
   LDA   (FP),Y
   LDY   #$01
   STA   (DPTR),Y
   LDY   #$20
   LDA   (FP),Y
   LDY   #$02
   STA   (DPTR),Y
   LDY   #$21
   LDA   (FP),Y
   LDY   #$03
   STA   (DPTR),Y
   LDA   g2
   LDY   #$26
   STA   (FP),Y
   LDA   g2
   BMI   .sx_neg@59
   LDA   #$00
   JMP   .sx_done@60
.sx_neg@59:
   LDA   #$FF
.sx_done@60:
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
   LDY   #$2C
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$2D
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$2E
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$2F
   STA   (FP),Y
   LDY   #$28
   LDA   (FP),Y
   SEC
   LDY   #$2E
   SBC   (FP),Y
   LDY   #$30
   STA   (FP),Y
   LDY   #$29
   LDA   (FP),Y
   LDY   #$2F
   SBC   (FP),Y
   LDY   #$31
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$32
   STA   (FP),Y
   LDA   g1
   INY
   STA   (FP),Y
   LDA   g1+1
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$35
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$36
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
   LDA   z3
   INY
   STA   (FP),Y
   LDA   z3
   BMI   .sx_neg@61
   LDA   #$00
   JMP   .sx_done@62
.sx_neg@61:
   LDA   #$FF
.sx_done@62:
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
   LDY   #$3E
   LDA   (FP),Y
   LDY   #$42
   CMP   (FP),Y
   BNE   .thread@2
   LDY   #$3D
   LDA   (FP),Y
   LDY   #$41
   CMP   (FP),Y
   BEQ   .cmp_true@63
.thread@2:
   LDA   #$00
   JMP   .cmp_end@64
.cmp_true@63:
   LDA   #$01
.cmp_end@64:
   LDY   #$43
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$45
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$06
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDY   #$32
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
   LDY   #$38
   LDA   (FP),Y
   LDY   #$05
   STA   (SSP),Y
   LDY   #$45
   LDA   (FP),Y
   LDY   #$06
   STA   (SSP),Y
   JSR   f22
   LDA   HARGS+16
   LDY   #$46
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
   LDY   #$46
   LDA   (FP),Y
   LDY   #$4D
   STA   (FP),Y
   LDY   #$47
   LDA   (FP),Y
   LDY   #$4E
   STA   (FP),Y
   LDY   #$48
   LDA   (FP),Y
   LDY   #$4F
   STA   (FP),Y
   LDY   #$49
   LDA   (FP),Y
   LDY   #$50
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$04
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDY   #$4D
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$4E
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$4F
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$50
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   JSR   mix
   LDA   #$00
   LDY   #$51
   STA   (FP),Y
   INY
   STA   (FP),Y
.loop@10_start:
   LDY   #$53
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
   BVC   .cmp_novf@66
   EOR   #$80
.cmp_novf@66:
   ASL
   LDA   #$00
   ROL
   LDY   #$55
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@9
   JMP   .loop@10_break
.lb_skip@9:
   LDA   #<gs5
   LDY   #$57
   STA   (FP),Y
   LDA   #>gs5
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$05
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
   SEC
   SBC   #$01
   LDY   #$5D
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   SBC   #$00
   LDY   #$5E
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
   LDA   #<ga4
   LDY   #$5F
   STA   (FP),Y
   LDA   #>ga4
   INY
   STA   (FP),Y
   LDY   #$52
   LDA   (FP),Y
   LDY   #$61
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
   LDY   #$61
   LDA   (FP),Y
   LDY   #$63
   AND   (FP),Y
   LDY   #$65
   STA   (FP),Y
   LDY   #$62
   LDA   (FP),Y
   LDY   #$64
   AND   (FP),Y
   LDY   #$66
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$67
   STA   (FP),Y
   STA   HARGS
   LDA   #$07
   STA   HARGS+1
   JSR   mul8
   LDA   HARGS+2
   LDY   #$68
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$5F
   LDA   (FP),Y
   CLC
   LDY   #$69
   ADC   (FP),Y
   LDY   #$6B
   STA   (FP),Y
   LDY   #$60
   LDA   (FP),Y
   LDY   #$6A
   ADC   (FP),Y
   LDY   #$6C
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$6D
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$6E
   STA   (FP),Y
   LDY   #$02
   LDA   (DPTR),Y
   LDY   #$6F
   STA   (FP),Y
   LDY   #$03
   LDA   (DPTR),Y
   LDY   #$70
   STA   (FP),Y
   LDY   #$04
   LDA   (DPTR),Y
   LDY   #$71
   STA   (FP),Y
   LDY   #$05
   LDA   (DPTR),Y
   LDY   #$72
   STA   (FP),Y
   LDY   #$06
   LDA   (DPTR),Y
   LDY   #$73
   STA   (FP),Y
   LDY   #$6D
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
   LDA   #$00
   STA   HARGS+7
   LDA   #$04
   STA   HARGS+8
   JSR   asl64
   LDA   HARGS+9
   LDY   #$74
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
   LDY   #$74
   LDA   (FP),Y
   LDY   #$7B
   STA   (FP),Y
   LDY   #$75
   LDA   (FP),Y
   LDY   #$7C
   STA   (FP),Y
   LDY   #$76
   LDA   (FP),Y
   LDY   #$7D
   STA   (FP),Y
   LDY   #$77
   LDA   (FP),Y
   LDY   #$7E
   STA   (FP),Y
   LDY   #$78
   LDA   (FP),Y
   LDY   #$7F
   STA   (FP),Y
   LDY   #$79
   LDA   (FP),Y
   LDY   #$80
   STA   (FP),Y
   LDY   #$7A
   LDA   (FP),Y
   AND   #$7F
   LDY   #$81
   STA   (FP),Y
   LDY   #$6B
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$7B
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$7C
   LDA   (FP),Y
   LDY   #$01
   STA   (DPTR),Y
   LDY   #$7D
   LDA   (FP),Y
   LDY   #$02
   STA   (DPTR),Y
   LDY   #$7E
   LDA   (FP),Y
   LDY   #$03
   STA   (DPTR),Y
   LDY   #$7F
   LDA   (FP),Y
   LDY   #$04
   STA   (DPTR),Y
   LDY   #$80
   LDA   (FP),Y
   LDY   #$05
   STA   (DPTR),Y
   LDY   #$81
   LDA   (FP),Y
   LDY   #$06
   STA   (DPTR),Y
   LDA   #$FF
   LDY   #$82
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$82
   LDA   (FP),Y
   LDY   #$86
   STA   (FP),Y
   LDY   #$83
   LDA   (FP),Y
   LDY   #$87
   STA   (FP),Y
   LDY   #$84
   LDA   (FP),Y
   LDY   #$88
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
   LDY   #$7B
   LDA   (FP),Y
   SEC
   LDY   #$86
   SBC   (FP),Y
   LDY   #$8D
   STA   (FP),Y
   LDY   #$7C
   LDA   (FP),Y
   LDY   #$87
   SBC   (FP),Y
   LDY   #$8E
   STA   (FP),Y
   LDY   #$7D
   LDA   (FP),Y
   LDY   #$88
   SBC   (FP),Y
   LDY   #$8F
   STA   (FP),Y
   LDY   #$7E
   LDA   (FP),Y
   LDY   #$89
   SBC   (FP),Y
   LDY   #$90
   STA   (FP),Y
   LDY   #$7F
   LDA   (FP),Y
   LDY   #$8A
   SBC   (FP),Y
   LDY   #$91
   STA   (FP),Y
   LDY   #$80
   LDA   (FP),Y
   LDY   #$8B
   SBC   (FP),Y
   LDY   #$92
   STA   (FP),Y
   LDY   #$81
   LDA   (FP),Y
   LDY   #$8C
   SBC   (FP),Y
   LDY   #$93
   STA   (FP),Y
   LDY   #$8D
   LDA   (FP),Y
   LDY   #$94
   STA   (FP),Y
   LDY   #$8E
   LDA   (FP),Y
   LDY   #$95
   STA   (FP),Y
   LDY   #$8F
   LDA   (FP),Y
   LDY   #$96
   STA   (FP),Y
   LDY   #$90
   LDA   (FP),Y
   LDY   #$97
   STA   (FP),Y
   LDY   #$91
   LDA   (FP),Y
   LDY   #$98
   STA   (FP),Y
   LDY   #$92
   LDA   (FP),Y
   LDY   #$99
   STA   (FP),Y
   LDY   #$93
   LDA   (FP),Y
   AND   #$7F
   LDY   #$9A
   STA   (FP),Y
   LDY   #$94
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
   BNE   .lb_skip@8
   JMP   .if_end@10
.lb_skip@8:
   LDA   g1
   LDY   #$9B
   STA   (FP),Y
   LDA   g1+1
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
   LDY   #$9B
   LDA   (FP),Y
   CLC
   LDY   #$A0
   SBC   (FP),Y
   LDY   #$9C
   LDA   (FP),Y
   LDY   #$A1
   SBC   (FP),Y
   LDA   #$00
   ROL
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   CLC
   LDA   FP
   ADC   #$07
   INY
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$04
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
   LDY   #$A4
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$A5
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   JSR   f7
   LDY   #$A6
   STA   (FP),Y
   LDA   z3
   INY
   STA   (FP),Y
   LDA   z3
   BMI   .sx_neg@67
   LDA   #$00
   JMP   .sx_done@68
.sx_neg@67:
   LDA   #$FF
.sx_done@68:
   LDY   #$A8
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   SEC
   SBC   #$FF
   LDY   #$A9
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   SBC   #$00
   LDY   #$AA
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$AB
   STA   (FP),Y
   STA   z3
   LDA   (FP),Y
   INY
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@69
   LDA   #$00
   JMP   .sx_done@70
.sx_neg@69:
   LDA   #$FF
.sx_done@70:
   LDY   #$AD
   STA   (FP),Y
   LDY   #$03
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lnot_true@71
   LDA   #$00
   JMP   .lnot_end@72
.lnot_true@71:
   LDA   #$01
.lnot_end@72:
   LDY   #$AE
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$AC
   LDA   (FP),Y
   LDY   #$AE
   EOR   (FP),Y
   LDY   #$B0
   STA   (FP),Y
   LDY   #$AD
   LDA   (FP),Y
   LDY   #$AF
   EOR   (FP),Y
   LDY   #$B1
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@7
   JMP   .if_end@11
.lb_skip@7:
   LDY   #$03
   LDA   (FP),Y
   LDY   #$B2
   STA   (FP),Y
   LDY   #$04
   LDA   (FP),Y
   LDY   #$B3
   STA   (FP),Y
   LDY   #$52
   LDA   (FP),Y
   LDY   #$B4
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   EOR   #$FF
   LDY   #$B6
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   EOR   #$FF
   LDY   #$B7
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$B8
   STA   (FP),Y
   LDA   gp6
   STA   DPTR
   LDA   gp6+1
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$B9
   STA   (FP),Y
   INY
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@73
   LDA   #$00
   JMP   .sx_done@74
.sx_neg@73:
   LDA   #$FF
.sx_done@74:
   LDY   #$BB
   STA   (FP),Y
   LDA   g2
   INY
   STA   (FP),Y
   LDA   g2
   BMI   .sx_neg@75
   LDA   #$00
   JMP   .sx_done@76
.sx_neg@75:
   LDA   #$FF
.sx_done@76:
   LDY   #$BD
   STA   (FP),Y
   LDY   #$BA
   LDA   (FP),Y
   LDY   #$BC
   AND   (FP),Y
   LDY   #$BE
   STA   (FP),Y
   LDY   #$BB
   LDA   (FP),Y
   LDY   #$BD
   AND   (FP),Y
   LDY   #$BF
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$C0
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$C1
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$05
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDY   #$B2
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$B3
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$B8
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$C0
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   LDY   #$C1
   LDA   (FP),Y
   LDY   #$05
   STA   (SSP),Y
   JSR   f14
   LDY   #$C2
   STA   (FP),Y
   JMP   .loop@10_break
.if_end@11:
.if_end@10:
   LDY   #$52
   LDA   (FP),Y
   LDY   #$C3
   STA   (FP),Y
   CLC
   ADC   #$01
   INY
   STA   (FP),Y
   LDY   #$52
   STA   (FP),Y
   JMP   .loop@10_start
.loop@10_break:
   JSR   fin0
   JSR   fin1
   JSR   fin2
   LDA   acc
   LDY   #$C5
   STA   (FP),Y
   LDA   acc+1
   AND   #$7F
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$C7
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$C8
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
   ADC   #$CA
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
   PLA
   TAX
   PLA
   RTS

acc:
   DS.B  2

g1:
   DC.W  $8000

g2:
   DC.B  $01

ga4:
   DC.B  $7F
   DS.B  6
   DC.B  $02
   DS.B  7
   DC.B  $01
   DS.B  5
   DC.B  $03
   DS.B  6

gs5:
   DS.B  1
   DC.L  $00000001
   DC.W  $FFFF

gp6:
   DC.W  g2

@3.s12:
   DC.W  $7FFF

@17.s28:
   DS.B  2

__zp_init:
   SUBROUTINE

   LDA   #$FF
   STA   z3
   RTS
