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
   LDA   z3
   LDY   #$09
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
   LDA   ga4
   LDY   #$0E
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

fin1:
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

   LDA   ga4+1
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
   LDA   ga4+2
   LDY   #$06
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
   LDA   ga4+3
   LDY   #$0B
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
   LDA   #<gs5
   LDY   #$10
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
   INY
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@0
   LDA   #$00
   JMP   .sx_done@1
.sx_neg@0:
   LDA   #$FF
.sx_done@1:
   LDY   #$16
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
   ADC   #$02
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
   ORA   #$00
   BMI   .sx_neg@2
   LDA   #$00
   JMP   .sx_done@3
.sx_neg@2:
   LDA   #$FF
.sx_done@3:
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

f8:
   SUBROUTINE

   ; prologue: 3 arg bytes, 119 local bytes
   SEC
   LDA   SSP
   SBC   #$79
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$78
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
   LDA   #<ga4
   INY
   STA   (FP),Y
   LDA   #>ga4
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
   LDY   #$07
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$03
   LDA   (FP),Y
   CLC
   LDY   #$08
   ADC   (FP),Y
   LDY   #$0A
   STA   (FP),Y
   LDY   #$04
   LDA   (FP),Y
   LDY   #$09
   ADC   (FP),Y
   LDY   #$0B
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$0C
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
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
   LDY   #$11
   STA   (FP),Y
   LDY   #$0A
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$11
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDA   ga4+3
   LDY   #$12
   STA   (FP),Y
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
   LSR
   LSR
   LDY   #$16
   STA   (FP),Y
   LDY   #$13
   LDA   (FP),Y
   ASL
   ASL
   ASL
   LDY   #$15
   STA   (FP),Y
   LDY   #$17
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$18
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@4
   LDA   #$00
   JMP   .sx_done@5
.sx_neg@4:
   LDA   #$FF
.sx_done@5:
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
   LDA   #<gs5
   LDY   #$1B
   STA   (FP),Y
   LDA   #>gs5
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
   ORA   #$00
   BEQ   .if_else@1
   LDY   #$02
   LDA   (FP),Y
   LDY   #$20
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@6
   LDA   #$00
   JMP   .sx_done@7
.sx_neg@6:
   LDA   #$FF
.sx_done@7:
   LDY   #$21
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   HARGS
   INY
   LDA   (FP),Y
   STA   HARGS+1
   LDA   #$FF
   STA   HARGS+2
   LDA   #$00
   STA   HARGS+3
   JSR   mul16
   LDA   HARGS+4
   LDY   #$22
   STA   (FP),Y
   LDA   HARGS+5
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$24
   STA   (FP),Y
   LDY   #$02
   STA   (FP),Y
   ORA   #$00
   BEQ   .if_end@2
   LDY   #$25
   STA   (FP),Y
   CLC
   ADC   #$01
   INY
   STA   (FP),Y
   LDY   #$02
   STA   (FP),Y
.if_end@2:
   LDA   z3
   LDY   #$27
   STA   (FP),Y
   SEC
   SBC   #$01
   INY
   STA   (FP),Y
   STA   z3
   JMP   .if_end@0
.if_else@1:
   LDY   #$7A
   LDA   (FP),Y
   CLC
   ADC   #$FF
   LDY   #$29
   STA   (FP),Y
   LDY   #$7B
   LDA   (FP),Y
   ADC   #$00
   LDY   #$2A
   STA   (FP),Y
   LDY   #$7C
   LDA   (FP),Y
   ADC   #$00
   LDY   #$2B
   STA   (FP),Y
   LDY   #$29
   LDA   (FP),Y
   LDY   #$2C
   STA   (FP),Y
   LDY   #$2A
   LDA   (FP),Y
   LDY   #$2D
   STA   (FP),Y
   LDY   #$2B
   LDA   (FP),Y
   AND   #$7F
   LDY   #$2E
   STA   (FP),Y
   LDY   #$2C
   LDA   (FP),Y
   LDY   #$7A
   STA   (FP),Y
   LDY   #$2D
   LDA   (FP),Y
   LDY   #$7B
   STA   (FP),Y
   LDY   #$2E
   LDA   (FP),Y
   LDY   #$7C
   STA   (FP),Y
   LDY   #$2C
   LDA   (FP),Y
   LDY   #$2F
   STA   (FP),Y
   LDY   #$02
   STA   (FP),Y
   LDA   #$00
   LDY   #$30
   STA   (FP),Y
   INY
   STA   (FP),Y
.loop@0_start:
   LDY   #$32
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
   BVC   .cmp_novf@8
   EOR   #$80
.cmp_novf@8:
   ASL
   LDA   #$00
   ROL
   LDY   #$34
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@0
   JMP   .loop@0_break
.lb_skip@0:
   LDA   #$01
   LDY   #$36
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   ORA   #$00
   BPL   .sx_done@10
   LDA   #$FF
.sx_done@10:
   LDY   #$38
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   g1
   CLC
   LDY   #$36
   ADC   (FP),Y
   LDY   #$3A
   STA   (FP),Y
   LDA   g1+1
   LDY   #$37
   ADC   (FP),Y
   LDY   #$3B
   STA   (FP),Y
   LDA   g1+2
   LDY   #$38
   ADC   (FP),Y
   LDY   #$3C
   STA   (FP),Y
   LDA   g1+3
   LDY   #$39
   ADC   (FP),Y
   LDY   #$3D
   STA   (FP),Y
   LDY   #$3A
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
   LDA   #<gs5
   INY
   STA   (FP),Y
   LDA   #>gs5
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
   LDA   #<ga4
   INY
   STA   (FP),Y
   LDA   #>ga4
   INY
   STA   (FP),Y
   LDY   #$31
   LDA   (FP),Y
   LDY   #$46
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
   LDY   #$46
   LDA   (FP),Y
   LDY   #$48
   AND   (FP),Y
   LDY   #$4A
   STA   (FP),Y
   LDY   #$47
   LDA   (FP),Y
   LDY   #$49
   AND   (FP),Y
   LDY   #$4B
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$4C
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$44
   LDA   (FP),Y
   CLC
   LDY   #$4D
   ADC   (FP),Y
   LDY   #$4F
   STA   (FP),Y
   LDY   #$45
   LDA   (FP),Y
   LDY   #$4E
   ADC   (FP),Y
   LDY   #$50
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$51
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ORA   #$81
   LDY   #$54
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$55
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$56
   STA   (FP),Y
   LDY   #$4F
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$56
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$56
   LDA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$42
   LDA   (FP),Y
   LDY   #$57
   ORA   (FP),Y
   LDY   #$59
   STA   (FP),Y
   LDY   #$43
   LDA   (FP),Y
   LDY   #$58
   ORA   (FP),Y
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
   ORA   #$00
   BMI   .sx_neg@11
   LDA   #$00
   JMP   .sx_done@12
.sx_neg@11:
   LDA   #$FF
.sx_done@12:
   LDY   #$5D
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
   LDY   #$5B
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$5C
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$5D
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$5E
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   JSR   mix
   LDA   gp6
   STA   DPTR
   LDA   gp6+1
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$5F
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$60
   STA   (FP),Y
   LDY   #$02
   LDA   (DPTR),Y
   LDY   #$61
   STA   (FP),Y
   LDY   #$03
   LDA   (DPTR),Y
   LDY   #$62
   STA   (FP),Y
   LDY   #$5F
   LDA   (FP),Y
   SEC
   SBC   #$01
   LDY   #$63
   STA   (FP),Y
   LDY   #$60
   LDA   (FP),Y
   SBC   #$00
   LDY   #$64
   STA   (FP),Y
   LDY   #$61
   LDA   (FP),Y
   SBC   #$00
   LDY   #$65
   STA   (FP),Y
   LDY   #$62
   LDA   (FP),Y
   SBC   #$00
   LDY   #$66
   STA   (FP),Y
   LDA   gp6
   STA   DPTR
   LDA   gp6+1
   STA   DPTR+1
   LDY   #$63
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$64
   LDA   (FP),Y
   LDY   #$01
   STA   (DPTR),Y
   LDY   #$65
   LDA   (FP),Y
   LDY   #$02
   STA   (DPTR),Y
   LDY   #$66
   LDA   (FP),Y
   LDY   #$03
   STA   (DPTR),Y
   LDY   #$31
   LDA   (FP),Y
   LDY   #$67
   STA   (FP),Y
   CLC
   ADC   #$01
   INY
   STA   (FP),Y
   LDY   #$31
   STA   (FP),Y
   JMP   .loop@0_start
.loop@0_break:
.if_end@0:
   LDA   #$03
   LDY   #$69
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
   BMI   .sx_neg@13
   LDA   #$00
   JMP   .sx_done@14
.sx_neg@13:
   LDA   #$FF
.sx_done@14:
   LDY   #$6D
   STA   (FP),Y
   LDY   #$6B
   LDA   (FP),Y
   LDY   #$6E
   STA   (FP),Y
   LDY   #$6C
   LDA   (FP),Y
   LDY   #$6F
   STA   (FP),Y
   LDY   #$6D
   LDA   (FP),Y
   AND   #$7F
   LDY   #$70
   STA   (FP),Y
   LDY   #$7A
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
   LDY   #$6E
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
   JSR   udivmod32
   LDA   HARGS+8
   LDY   #$71
   STA   (FP),Y
   LDA   HARGS+9
   INY
   STA   (FP),Y
   LDA   HARGS+10
   INY
   STA   (FP),Y
   LDY   #$71
   LDA   (FP),Y
   LDY   #$74
   STA   (FP),Y
   LDY   #$72
   LDA   (FP),Y
   LDY   #$75
   STA   (FP),Y
   LDY   #$73
   LDA   (FP),Y
   LDY   #$76
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$74
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
   ADC   #$7C
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

f12:
   SUBROUTINE

   ; prologue: 2 arg bytes, 120 local bytes
   SEC
   LDA   SSP
   SBC   #$7A
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$79
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
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$7B
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$15
   STA   (FP),Y
   INY
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@15
   LDA   #$00
   JMP   .sx_done@16
.sx_neg@15:
   LDA   #$FF
.sx_done@16:
   LDY   #$17
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$16
   LDA   (FP),Y
   LDY   #$19
   STA   (FP),Y
   LDY   #$17
   LDA   (FP),Y
   LDY   #$1A
   STA   (FP),Y
   AND   #$7F
   INY
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$03
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
   JSR   f8
   LDA   HARGS+8
   LDY   #$1C
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
   LDY   #$1C
   LDA   (FP),Y
   LDY   #$09
   STA   (FP),Y
   LDY   #$1D
   LDA   (FP),Y
   LDY   #$0A
   STA   (FP),Y
   LDY   #$1E
   LDA   (FP),Y
   LDY   #$0B
   STA   (FP),Y
   LDY   #$1F
   LDA   (FP),Y
   LDY   #$0C
   STA   (FP),Y
   LDA   z3
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
   LDA   #$05
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$27
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$28
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@17
   LDA   #$00
   JMP   .sx_done@18
.sx_neg@17:
   LDA   #$FF
.sx_done@18:
   LDY   #$29
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$21
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
   LDY   #$27
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
   LDY   #$2B
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
   LDY   #$2B
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   INY
   ORA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@1
   JMP   .if_else@4
.lb_skip@1:
   LDA   @5.s17
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
   BMI   .sx_neg@19
   LDA   #$00
   JMP   .sx_done@20
.sx_neg@19:
   LDA   #$FF
.sx_done@20:
   LDY   #$33
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
   LDA   #$05
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   ORA   #$00
   BPL   .sx_done@22
   LDA   #$FF
.sx_done@22:
   LDY   #$3B
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
   LDY   #$0D
   LDA   (FP),Y
   CLC
   LDY   #$39
   ADC   (FP),Y
   LDY   #$41
   STA   (FP),Y
   LDY   #$0E
   LDA   (FP),Y
   LDY   #$3A
   ADC   (FP),Y
   LDY   #$42
   STA   (FP),Y
   LDY   #$0F
   LDA   (FP),Y
   LDY   #$3B
   ADC   (FP),Y
   LDY   #$43
   STA   (FP),Y
   LDY   #$10
   LDA   (FP),Y
   LDY   #$3C
   ADC   (FP),Y
   LDY   #$44
   STA   (FP),Y
   LDY   #$11
   LDA   (FP),Y
   LDY   #$3D
   ADC   (FP),Y
   LDY   #$45
   STA   (FP),Y
   LDY   #$12
   LDA   (FP),Y
   LDY   #$3E
   ADC   (FP),Y
   LDY   #$46
   STA   (FP),Y
   LDY   #$13
   LDA   (FP),Y
   LDY   #$3F
   ADC   (FP),Y
   LDY   #$47
   STA   (FP),Y
   LDY   #$14
   LDA   (FP),Y
   LDY   #$40
   ADC   (FP),Y
   LDY   #$48
   STA   (FP),Y
   LDY   #$41
   LDA   (FP),Y
   LDY   #$0D
   STA   (FP),Y
   LDY   #$42
   LDA   (FP),Y
   LDY   #$0E
   STA   (FP),Y
   LDY   #$43
   LDA   (FP),Y
   LDY   #$0F
   STA   (FP),Y
   LDY   #$44
   LDA   (FP),Y
   LDY   #$10
   STA   (FP),Y
   LDY   #$45
   LDA   (FP),Y
   LDY   #$11
   STA   (FP),Y
   LDY   #$46
   LDA   (FP),Y
   LDY   #$12
   STA   (FP),Y
   LDY   #$47
   LDA   (FP),Y
   LDY   #$13
   STA   (FP),Y
   LDY   #$48
   LDA   (FP),Y
   LDY   #$14
   STA   (FP),Y
   LDY   #$31
   LDA   (FP),Y
   LDY   #$41
   EOR   (FP),Y
   LDY   #$49
   STA   (FP),Y
   LDY   #$32
   LDA   (FP),Y
   LDY   #$42
   EOR   (FP),Y
   LDY   #$4A
   STA   (FP),Y
   LDY   #$33
   LDA   (FP),Y
   LDY   #$43
   EOR   (FP),Y
   LDY   #$4B
   STA   (FP),Y
   LDY   #$34
   LDA   (FP),Y
   LDY   #$44
   EOR   (FP),Y
   LDY   #$4C
   STA   (FP),Y
   LDY   #$35
   LDA   (FP),Y
   LDY   #$45
   EOR   (FP),Y
   LDY   #$4D
   STA   (FP),Y
   LDY   #$36
   LDA   (FP),Y
   LDY   #$46
   EOR   (FP),Y
   LDY   #$4E
   STA   (FP),Y
   LDY   #$37
   LDA   (FP),Y
   LDY   #$47
   EOR   (FP),Y
   LDY   #$4F
   STA   (FP),Y
   LDY   #$38
   LDA   (FP),Y
   LDY   #$48
   EOR   (FP),Y
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
   LDY   #$53
   STA   (FP),Y
   LDY   #$4C
   LDA   (FP),Y
   LDY   #$54
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$04
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDY   #$51
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$52
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$53
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$54
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   JSR   mix
   JMP   .if_end@3
.if_else@4:
   LDA   g1
   STA   HARGS
   LDA   g1+1
   STA   HARGS+1
   LDA   g1+2
   STA   HARGS+2
   LDA   g1+3
   STA   HARGS+3
   LDA   #$02
   STA   HARGS+4
   JSR   asr32
   LDA   HARGS+5
   LDY   #$55
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
   LDY   #$55
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
.if_end@3:
   LDA   gp7
   STA   DPTR
   LDA   gp7+1
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$59
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$5A
   STA   (FP),Y
   LDY   #$02
   LDA   (DPTR),Y
   LDY   #$5B
   STA   (FP),Y
   LDY   #$03
   LDA   (DPTR),Y
   LDY   #$5C
   STA   (FP),Y
   LDY   #$04
   LDA   (DPTR),Y
   LDY   #$5D
   STA   (FP),Y
   LDY   #$05
   LDA   (DPTR),Y
   LDY   #$5E
   STA   (FP),Y
   LDY   #$06
   LDA   (DPTR),Y
   LDY   #$5F
   STA   (FP),Y
   LDY   #$07
   LDA   (DPTR),Y
   LDY   #$60
   STA   (FP),Y
   LDA   #$05
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   ORA   #$00
   BPL   .sx_done@24
   LDA   #$FF
.sx_done@24:
   LDY   #$63
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
   LDY   #$59
   LDA   (FP),Y
   SEC
   LDY   #$61
   SBC   (FP),Y
   LDY   #$69
   STA   (FP),Y
   LDY   #$5A
   LDA   (FP),Y
   LDY   #$62
   SBC   (FP),Y
   LDY   #$6A
   STA   (FP),Y
   LDY   #$5B
   LDA   (FP),Y
   LDY   #$63
   SBC   (FP),Y
   LDY   #$6B
   STA   (FP),Y
   LDY   #$5C
   LDA   (FP),Y
   LDY   #$64
   SBC   (FP),Y
   LDY   #$6C
   STA   (FP),Y
   LDY   #$5D
   LDA   (FP),Y
   LDY   #$65
   SBC   (FP),Y
   LDY   #$6D
   STA   (FP),Y
   LDY   #$5E
   LDA   (FP),Y
   LDY   #$66
   SBC   (FP),Y
   LDY   #$6E
   STA   (FP),Y
   LDY   #$5F
   LDA   (FP),Y
   LDY   #$67
   SBC   (FP),Y
   LDY   #$6F
   STA   (FP),Y
   LDY   #$60
   LDA   (FP),Y
   LDY   #$68
   SBC   (FP),Y
   LDY   #$70
   STA   (FP),Y
   LDA   gp7
   STA   DPTR
   LDA   gp7+1
   STA   DPTR+1
   LDY   #$69
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$6A
   LDA   (FP),Y
   LDY   #$01
   STA   (DPTR),Y
   LDY   #$6B
   LDA   (FP),Y
   LDY   #$02
   STA   (DPTR),Y
   LDY   #$6C
   LDA   (FP),Y
   LDY   #$03
   STA   (DPTR),Y
   LDY   #$6D
   LDA   (FP),Y
   LDY   #$04
   STA   (DPTR),Y
   LDY   #$6E
   LDA   (FP),Y
   LDY   #$05
   STA   (DPTR),Y
   LDY   #$6F
   LDA   (FP),Y
   LDY   #$06
   STA   (DPTR),Y
   LDY   #$70
   LDA   (FP),Y
   LDY   #$07
   STA   (DPTR),Y
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
   SEC
   LDA   SSP
   SBC   #$04
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDY   #$71
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$72
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$73
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$74
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   JSR   mix
   LDA   #$01
   LDY   #$75
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$04
   CMP   (FP),Y
   BNE   .cmp_true@25
   LDY   #$75
   LDA   (FP),Y
   LDY   #$03
   CMP   (FP),Y
   BNE   .cmp_true@25
   LDA   #$00
   JMP   .cmp_end@26
.cmp_true@25:
   LDA   #$01
.cmp_end@26:
   LDY   #$77
   STA   (FP),Y
   LDA   #$00
   INY
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
   ADC   #$7C
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$79
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

f18:
   SUBROUTINE

   ; prologue: 3 arg bytes, 157 local bytes
   SEC
   LDA   SSP
   SBC   #$9F
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$9E
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
   LDA   #$FF
   INY
   STA   (FP),Y
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
   LDY   #$05
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   HARGS
   INY
   LDA   (FP),Y
   STA   HARGS+1
   LDA   #$FF
   STA   HARGS+2
   LDA   #$00
   STA   HARGS+3
   JSR   mul16
   LDA   HARGS+4
   LDY   #$06
   STA   (FP),Y
   LDA   HARGS+5
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$08
   STA   (FP),Y
   LDY   #$03
   STA   (FP),Y
   LDA   g2
   LDY   #$09
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
   LDA   g2+6
   INY
   STA   (FP),Y
   LDA   g2+7
   INY
   STA   (FP),Y
   LDA   g2
   SEC
   SBC   #$01
   INY
   STA   (FP),Y
   LDA   g2+1
   SBC   #$00
   INY
   STA   (FP),Y
   LDA   g2+2
   SBC   #$00
   INY
   STA   (FP),Y
   LDA   g2+3
   SBC   #$00
   INY
   STA   (FP),Y
   LDA   g2+4
   SBC   #$00
   INY
   STA   (FP),Y
   LDA   g2+5
   SBC   #$00
   INY
   STA   (FP),Y
   LDA   g2+6
   SBC   #$00
   INY
   STA   (FP),Y
   LDA   g2+7
   SBC   #$00
   INY
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
   LDY   #$09
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
   BNE   .lb_skip@8
   JMP   .if_end@5
.lb_skip@8:
   LDA   gp6
   STA   DPTR
   LDA   gp6+1
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
   LDY   #$A0
   LDA   (FP),Y
   LDY   #$1D
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$1F
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$20
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@30
   LDA   #$00
   JMP   .sx_done@31
.sx_neg@30:
   LDA   #$FF
.sx_done@31:
   LDY   #$21
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$19
   LDA   (FP),Y
   CLC
   LDY   #$1F
   ADC   (FP),Y
   LDY   #$23
   STA   (FP),Y
   LDY   #$1A
   LDA   (FP),Y
   LDY   #$20
   ADC   (FP),Y
   LDY   #$24
   STA   (FP),Y
   LDY   #$1B
   LDA   (FP),Y
   LDY   #$21
   ADC   (FP),Y
   LDY   #$25
   STA   (FP),Y
   LDY   #$1C
   LDA   (FP),Y
   LDY   #$22
   ADC   (FP),Y
   LDY   #$26
   STA   (FP),Y
   LDY   #$23
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   INY
   ORA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@7
   JMP   .if_end@6
.lb_skip@7:
   LDY   #$01
   LDA   (FP),Y
   LDY   #$27
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@32
   LDA   #$00
   JMP   .sx_done@33
.sx_neg@32:
   LDA   #$FF
.sx_done@33:
   LDY   #$28
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
   LDY   #$27
   LDA   (FP),Y
   CLC
   LDY   #$2D
   ADC   (FP),Y
   LDY   #$2F
   STA   (FP),Y
   LDY   #$28
   LDA   (FP),Y
   LDY   #$2E
   ADC   (FP),Y
   LDY   #$30
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
   BMI   .sx_neg@34
   LDA   #$00
   JMP   .sx_done@35
.sx_neg@34:
   LDA   #$FF
.sx_done@35:
   LDY   #$33
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
   LDY   #$31
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
   ADC   #$A2
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$9E
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   RTS
.loop@1_start:
   LDA   #$00
   LDY   #$47
   STA   (FP),Y
   LDA   #$01
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   EOR   #$FF
   LDY   #$49
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   EOR   #$FF
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
   SEC
   LDA   SSP
   SBC   #$03
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
   JSR   f8
   LDA   HARGS+8
   LDY   #$4E
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
   LDY   #$46
   LDA   (FP),Y
   CLC
   ADC   #$01
   LDY   #$52
   STA   (FP),Y
   LDY   #$46
   STA   (FP),Y
   LDY   #$53
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
   BVC   .cmp_novf@38
   EOR   #$80
.cmp_novf@38:
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
   BEQ   .lb_skip@6
   JMP   .loop@1_start
.lb_skip@6:
.if_end@6:
   LDA   #$00
   LDY   #$57
   STA   (FP),Y
   INY
   STA   (FP),Y
.loop@2_continue:
   LDY   #$58
   LDA   (FP),Y
   INY
   STA   (FP),Y
   CLC
   ADC   #$01
   INY
   STA   (FP),Y
   LDY   #$58
   STA   (FP),Y
   INY
   LDA   (FP),Y
   LDY   #$5B
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
   BVC   .cmp_novf@39
   EOR   #$80
.cmp_novf@39:
   ASL
   LDA   #$00
   ROL
   LDY   #$5D
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@5
   JMP   .loop@2_break
.lb_skip@5:
   LDY   #$58
   LDA   (FP),Y
   LDY   #$5F
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$61
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$62
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@40
   LDA   #$00
   JMP   .sx_done@41
.sx_neg@40:
   LDA   #$FF
.sx_done@41:
   LDY   #$63
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
   LDY   #$69
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$6A
   STA   (FP),Y
   LDY   #$02
   LDA   (DPTR),Y
   LDY   #$6B
   STA   (FP),Y
   LDY   #$03
   LDA   (DPTR),Y
   LDY   #$6C
   STA   (FP),Y
   LDY   #$04
   LDA   (DPTR),Y
   LDY   #$6D
   STA   (FP),Y
   LDY   #$05
   LDA   (DPTR),Y
   LDY   #$6E
   STA   (FP),Y
   LDY   #$06
   LDA   (DPTR),Y
   LDY   #$6F
   STA   (FP),Y
   LDY   #$07
   LDA   (DPTR),Y
   LDY   #$70
   STA   (FP),Y
   LDA   #$01
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   ORA   #$00
   BPL   .sx_done@43
   LDA   #$FF
.sx_done@43:
   LDY   #$73
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
   LDY   #$69
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
   LDY   #$79
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
   LDA   gp7
   STA   DPTR
   LDA   gp7+1
   STA   DPTR+1
   LDY   #$79
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$7A
   LDA   (FP),Y
   LDY   #$01
   STA   (DPTR),Y
   LDY   #$7B
   LDA   (FP),Y
   LDY   #$02
   STA   (DPTR),Y
   LDY   #$7C
   LDA   (FP),Y
   LDY   #$03
   STA   (DPTR),Y
   LDY   #$7D
   LDA   (FP),Y
   LDY   #$04
   STA   (DPTR),Y
   LDY   #$7E
   LDA   (FP),Y
   LDY   #$05
   STA   (DPTR),Y
   LDY   #$7F
   LDA   (FP),Y
   LDY   #$06
   STA   (DPTR),Y
   LDY   #$80
   LDA   (FP),Y
   LDY   #$07
   STA   (DPTR),Y
   LDY   #$61
   LDA   (FP),Y
   LDY   #$79
   EOR   (FP),Y
   LDY   #$81
   STA   (FP),Y
   LDY   #$62
   LDA   (FP),Y
   LDY   #$7A
   EOR   (FP),Y
   LDY   #$82
   STA   (FP),Y
   LDY   #$63
   LDA   (FP),Y
   LDY   #$7B
   EOR   (FP),Y
   LDY   #$83
   STA   (FP),Y
   LDY   #$64
   LDA   (FP),Y
   LDY   #$7C
   EOR   (FP),Y
   LDY   #$84
   STA   (FP),Y
   LDY   #$65
   LDA   (FP),Y
   LDY   #$7D
   EOR   (FP),Y
   LDY   #$85
   STA   (FP),Y
   LDY   #$66
   LDA   (FP),Y
   LDY   #$7E
   EOR   (FP),Y
   LDY   #$86
   STA   (FP),Y
   LDY   #$67
   LDA   (FP),Y
   LDY   #$7F
   EOR   (FP),Y
   LDY   #$87
   STA   (FP),Y
   LDY   #$68
   LDA   (FP),Y
   LDY   #$80
   EOR   (FP),Y
   LDY   #$88
   STA   (FP),Y
   LDY   #$81
   LDA   (FP),Y
   LDY   #$89
   STA   (FP),Y
   LDY   #$82
   LDA   (FP),Y
   LDY   #$8A
   STA   (FP),Y
   LDA   #$07
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$89
   LDA   (FP),Y
   LDY   #$8B
   AND   (FP),Y
   LDY   #$8D
   STA   (FP),Y
   LDY   #$8A
   LDA   (FP),Y
   LDY   #$8C
   AND   (FP),Y
   LDY   #$8E
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CMP   #$01
   BEQ   .cmp_true@44
   LDA   #$00
   JMP   .cmp_end@45
.cmp_true@44:
   LDA   #$01
.cmp_end@45:
   LDY   #$8F
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@4
   JMP   .loop@2_continue
.lb_skip@4:
   LDY   #$8D
   LDA   (FP),Y
   CMP   #$03
   BEQ   .cmp_true@46
   LDA   #$00
   JMP   .cmp_end@47
.cmp_true@46:
   LDA   #$01
.cmp_end@47:
   LDY   #$91
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@3
   JMP   .loop@2_continue
.lb_skip@3:
   LDY   #$8D
   LDA   (FP),Y
   CMP   #$06
   BEQ   .cmp_true@48
   LDA   #$00
   JMP   .cmp_end@49
.cmp_true@48:
   LDA   #$01
.cmp_end@49:
   LDY   #$93
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@2
   JMP   .loop@2_continue
.lb_skip@2:
   JMP   .loop@2_continue
.loop@2_break:
.if_end@5:
   LDA   ga4+3
   LDY   #$95
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
   LDY   #$96
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
   ADC   #$A2
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

   ; prologue: 0 arg bytes, 163 local bytes
   SEC
   LDA   SSP
   SBC   #$A5
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$A4
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
   DEY
   LDA   (FP),Y
   LDY   #$07
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$08
   STA   (FP),Y
   LDA   #$7F
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$FF
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$05
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   ORA   #$00
   BPL   .sx_done@51
   LDA   #$FF
.sx_done@51:
   LDY   #$0F
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
   LDY   #$0D
   AND   (FP),Y
   LDY   #$15
   STA   (FP),Y
   LDA   g2+1
   LDY   #$0E
   AND   (FP),Y
   LDY   #$16
   STA   (FP),Y
   LDA   g2+2
   LDY   #$0F
   AND   (FP),Y
   LDY   #$17
   STA   (FP),Y
   LDA   g2+3
   LDY   #$10
   AND   (FP),Y
   LDY   #$18
   STA   (FP),Y
   LDA   g2+4
   LDY   #$11
   AND   (FP),Y
   LDY   #$19
   STA   (FP),Y
   LDA   g2+5
   LDY   #$12
   AND   (FP),Y
   LDY   #$1A
   STA   (FP),Y
   LDA   g2+6
   LDY   #$13
   AND   (FP),Y
   LDY   #$1B
   STA   (FP),Y
   LDA   g2+7
   LDY   #$14
   AND   (FP),Y
   LDY   #$1C
   STA   (FP),Y
   LDY   #$15
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
   LDY   #$15
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
   BNE   .lb_skip@14
   JMP   .if_end@7
.lb_skip@14:
   CLC
   LDA   FP
   ADC   #$0A
   LDY   #$1D
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
   LDY   #$1E
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   JSR   f12
   LDY   #$1F
   STA   (FP),Y
   TXA
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
   LDY   #$23
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$24
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$25
   STA   (FP),Y
   SEC
   SBC   #$01
   INY
   STA   (FP),Y
   LDY   #$23
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$26
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$25
   LDA   (FP),Y
   LDY   #$27
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@52
   LDA   #$00
   JMP   .sx_done@53
.sx_neg@52:
   LDA   #$FF
.sx_done@53:
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
   LDY   #$07
   LDA   (FP),Y
   CLC
   LDY   #$29
   ADC   (FP),Y
   LDY   #$2B
   STA   (FP),Y
   LDY   #$08
   LDA   (FP),Y
   LDY   #$2A
   ADC   (FP),Y
   LDY   #$2C
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$2D
   STA   (FP),Y
   DEY
   LDA   (FP),Y
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
   LDY   #$2D
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
.if_end@7:
   LDA   #$00
   LDY   #$35
   STA   (FP),Y
   INY
   STA   (FP),Y
.loop@8_start:
   LDA   #$FF
   LDY   #$37
   STA   (FP),Y
   ASL
   STA   (FP),Y
   LDA   #$7F
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
   LDY   #$37
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
   BNE   .lb_skip@13
   JMP   .loop@8_break
.lb_skip@13:
   LDY   #$0A
   LDA   (FP),Y
   LDY   #$3F
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@54
   LDA   #$00
   JMP   .sx_done@55
.sx_neg@54:
   LDA   #$FF
.sx_done@55:
   LDY   #$40
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   AND   #$5A
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
   LDY   #$0A
   STA   (FP),Y
   LDA   #<ga4
   LDY   #$44
   STA   (FP),Y
   LDA   #>ga4
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
   LDY   #$48
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$44
   LDA   (FP),Y
   CLC
   LDY   #$49
   ADC   (FP),Y
   LDY   #$4B
   STA   (FP),Y
   LDY   #$45
   LDA   (FP),Y
   LDY   #$4A
   ADC   (FP),Y
   LDY   #$4C
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$4D
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$81
   LDY   #$50
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$51
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$52
   STA   (FP),Y
   LDY   #$4B
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$52
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$07
   LDA   (FP),Y
   LDY   #$53
   STA   (FP),Y
   LDY   #$08
   LDA   (FP),Y
   LDY   #$54
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
   LDA   g2
   SEC
   LDY   #$53
   SBC   (FP),Y
   LDY   #$5B
   STA   (FP),Y
   LDA   g2+1
   LDY   #$54
   SBC   (FP),Y
   LDY   #$5C
   STA   (FP),Y
   LDA   g2+2
   LDY   #$55
   SBC   (FP),Y
   LDY   #$5D
   STA   (FP),Y
   LDA   g2+3
   LDY   #$56
   SBC   (FP),Y
   LDY   #$5E
   STA   (FP),Y
   LDA   g2+4
   LDY   #$57
   SBC   (FP),Y
   LDY   #$5F
   STA   (FP),Y
   LDA   g2+5
   LDY   #$58
   SBC   (FP),Y
   LDY   #$60
   STA   (FP),Y
   LDA   g2+6
   LDY   #$59
   SBC   (FP),Y
   LDY   #$61
   STA   (FP),Y
   LDA   g2+7
   LDY   #$5A
   SBC   (FP),Y
   LDY   #$62
   STA   (FP),Y
   LDY   #$5B
   LDA   (FP),Y
   LDY   #$63
   STA   (FP),Y
   LDY   #$5C
   LDA   (FP),Y
   LDY   #$64
   STA   (FP),Y
   LDY   #$5D
   LDA   (FP),Y
   LDY   #$65
   STA   (FP),Y
   LDY   #$63
   LDA   (FP),Y
   LDY   #$66
   STA   (FP),Y
   LDY   #$64
   LDA   (FP),Y
   LDY   #$67
   STA   (FP),Y
   LDY   #$65
   LDA   (FP),Y
   AND   #$7F
   LDY   #$68
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$03
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDY   #$66
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$67
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$68
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   JSR   f8
   LDA   HARGS+8
   LDY   #$69
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
   LDY   #$70
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$04
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDY   #$6D
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$6E
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$6F
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$70
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
   LDY   #$71
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$72
   STA   (FP),Y
   LDY   #$02
   LDA   (DPTR),Y
   LDY   #$73
   STA   (FP),Y
   LDY   #$03
   LDA   (DPTR),Y
   LDY   #$74
   STA   (FP),Y
   LDY   #$04
   LDA   (DPTR),Y
   LDY   #$75
   STA   (FP),Y
   LDY   #$05
   LDA   (DPTR),Y
   LDY   #$76
   STA   (FP),Y
   LDY   #$06
   LDA   (DPTR),Y
   LDY   #$77
   STA   (FP),Y
   LDY   #$07
   LDA   (DPTR),Y
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
   LDA   #$07
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$79
   LDA   (FP),Y
   LDY   #$7B
   AND   (FP),Y
   LDY   #$7D
   STA   (FP),Y
   LDY   #$7A
   LDA   (FP),Y
   LDY   #$7C
   AND   (FP),Y
   LDY   #$7E
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CMP   #$02
   BEQ   .cmp_true@56
   LDA   #$00
   JMP   .cmp_end@57
.cmp_true@56:
   LDA   #$01
.cmp_end@57:
   LDY   #$7F
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .case@10
   LDY   #$7D
   LDA   (FP),Y
   CMP   #$04
   BEQ   .cmp_true@58
   LDA   #$00
   JMP   .cmp_end@59
.cmp_true@58:
   LDA   #$01
.cmp_end@59:
   LDY   #$81
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@12
   JMP   .switch@9_break
.lb_skip@12:
   LDY   #$7D
   LDA   (FP),Y
   CMP   #$07
   BEQ   .cmp_true@60
   LDA   #$00
   JMP   .cmp_end@61
.cmp_true@60:
   LDA   #$01
.cmp_end@61:
   LDY   #$83
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@11
   JMP   .case@13
.lb_skip@11:
   JMP   .default@14
.case@10:
   LDA   #$00
   LDY   #$85
   STA   (FP),Y
   INY
   STA   (FP),Y
.loop@11_continue:
   LDY   #$86
   LDA   (FP),Y
   INY
   STA   (FP),Y
   CLC
   ADC   #$01
   INY
   STA   (FP),Y
   LDY   #$86
   STA   (FP),Y
   INY
   LDA   (FP),Y
   LDY   #$89
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
   BVC   .cmp_novf@62
   EOR   #$80
.cmp_novf@62:
   ASL
   LDA   #$00
   ROL
   LDY   #$8B
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@10
   JMP   .switch@9_break
.lb_skip@10:
   LDA   #<gs5
   LDY   #$8D
   STA   (FP),Y
   LDA   #>gs5
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$01
   LDY   #$8F
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$90
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$91
   STA   (FP),Y
   INY
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@63
   LDA   #$00
   JMP   .sx_done@64
.sx_neg@63:
   LDA   #$FF
.sx_done@64:
   LDY   #$93
   STA   (FP),Y
   LDY   #$0A
   LDA   (FP),Y
   SEC
   SBC   #$01
   LDY   #$94
   STA   (FP),Y
   LDY   #$0A
   STA   (FP),Y
   LDY   #$95
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@65
   LDA   #$00
   JMP   .sx_done@66
.sx_neg@65:
   LDA   #$FF
.sx_done@66:
   LDY   #$96
   STA   (FP),Y
   LDY   #$92
   LDA   (FP),Y
   LDY   #$95
   ORA   (FP),Y
   LDY   #$97
   STA   (FP),Y
   LDY   #$93
   LDA   (FP),Y
   LDY   #$96
   ORA   (FP),Y
   LDY   #$98
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$99
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$9A
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$03
   STA   (FP),Y
   LDY   #$9A
   LDA   (FP),Y
   LDY   #$04
   STA   (FP),Y
   JMP   .loop@11_continue
.case@13:
.default@14:
.switch@9_break:
   LDY   #$36
   LDA   (FP),Y
   CLC
   ADC   #$01
   LDY   #$9B
   STA   (FP),Y
   LDY   #$36
   STA   (FP),Y
   LDY   #$9C
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
   BVC   .cmp_novf@67
   EOR   #$80
.cmp_novf@67:
   ASL
   LDA   #$00
   ROL
   LDY   #$9E
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@9
   JMP   .loop@8_start
.lb_skip@9:
.loop@8_break:
   JSR   fin0
   JSR   fin1
   JSR   fin2
   LDA   acc
   LDY   #$A0
   STA   (FP),Y
   LDA   acc+1
   AND   #$7F
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$A2
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$A3
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
   ADC   #$A5
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$A4
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
   DC.L  $00000003

g2:
   DC.L  $000000FF
   DC.L  $00000000

ga4:
   DC.B  $80, $80, $FF, $03

gs5:
   DS.B  1
   DC.B  $FF
   DC.W  $007F

gp6:
   DC.W  g1

gp7:
   DC.W  g2

@5.s17:
   DC.B  $80

__zp_init:
   SUBROUTINE

   LDA   #$00
   STA   z3
   RTS
