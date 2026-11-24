SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
z4	EQU	$26
z5	EQU	$2E

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

   ; prologue: 0 arg bytes, 12 local bytes
   SEC
   LDA   SSP
   SBC   #$0E
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$0D
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   SEC
   LDA   SSP
   SBC   #$04
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   g1
   LDY   #$01
   STA   (SSP),Y
   LDA   g1+1
   INY
   STA   (SSP),Y
   LDA   g1+2
   INY
   STA   (SSP),Y
   LDA   g1+3
   INY
   STA   (SSP),Y
   JSR   mix
   LDA   g2
   LDY   #$01
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
   LDA   g3
   LDY   #$05
   STA   (FP),Y
   LDA   g3+1
   INY
   STA   (FP),Y
   LDA   g3+2
   INY
   STA   (FP),Y
   LDA   #$00
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
   LDA   z4
   LDY   #$09
   STA   (FP),Y
   LDA   z4+1
   INY
   STA   (FP),Y
   LDA   z4+2
   INY
   STA   (FP),Y
   LDA   z4+3
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

   ; epilogue
   CLC
   LDA   FP
   ADC   #$0E
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$0D
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

   ; prologue: 0 arg bytes, 19 local bytes
   SEC
   LDA   SSP
   SBC   #$15
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$14
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
   LDA   ga6
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
   LDA   ga6+1
   LDY   #$0A
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
   LDA   ga6+2
   LDY   #$0F
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
   LDY   #$10
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$11
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$12
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$13
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   JSR   mix

   ; epilogue
   CLC
   LDA   FP
   ADC   #$15
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$14
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

   LDA   ga6+3
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
   LDA   #<gs7
   LDY   #$06
   STA   (FP),Y
   LDA   #>gs7
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$01
   LDY   #$08
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$09
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$0A
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
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
   LDA   #<gs7
   LDY   #$10
   STA   (FP),Y
   LDA   #>gs7
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$03
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
   DEY
   LDA   (FP),Y
   LDY   #$16
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$17
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

f10:
   SUBROUTINE

   ; prologue: 4 arg bytes, 120 local bytes
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

   LDA   #$00
   LDY   #$01
   STA   (FP),Y
   LDA   #$80
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
   INY
   STA   (FP),Y
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
   INY
   STA   (FP),Y
.loop@0_start:
   LDY   #$0C
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
   BVC   .cmp_novf@0
   EOR   #$80
.cmp_novf@0:
   ASL
   LDA   #$00
   ROL
   LDY   #$0E
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@3
   JMP   .loop@0_break
.lb_skip@3:
   LDA   #$00
   LDY   #$10
   STA   (FP),Y
   INY
   STA   (FP),Y
.loop@1_start:
   LDY   #$12
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
   BVC   .cmp_novf@1
   EOR   #$80
.cmp_novf@1:
   ASL
   LDA   #$00
   ROL
   LDY   #$14
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@2
   JMP   .loop@1_break
.lb_skip@2:
   LDA   g3
   AND   #$01
   LDY   #$16
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$16
   LDA   (FP),Y
   STA   g3
   INY
   LDA   (FP),Y
   STA   g3+1
   INY
   LDA   (FP),Y
   STA   g3+2
   LDY   #$7D
   LDA   (FP),Y
   LDY   #$19
   STA   (FP),Y
   LDY   #$7E
   LDA   (FP),Y
   LDY   #$1A
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@2
   LDA   #$00
   JMP   .sx_done@3
.sx_neg@2:
   LDA   #$FF
.sx_done@3:
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
   AND   #$07
   LDY   #$1E
   STA   (FP),Y
   LDY   #$16
   LDA   (FP),Y
   LDY   #$1C
   AND   (FP),Y
   LDY   #$1F
   STA   (FP),Y
   LDY   #$17
   LDA   (FP),Y
   LDY   #$1D
   AND   (FP),Y
   LDY   #$20
   STA   (FP),Y
   LDY   #$18
   LDA   (FP),Y
   LDY   #$1E
   AND   (FP),Y
   LDY   #$21
   STA   (FP),Y
   LDY   #$1F
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@0
   LDA   #<gs7
   INY
   STA   (FP),Y
   LDA   #>gs7
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$03
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
   AND   #$7F
   INY
   STA   (FP),Y

   ; epilogue
   PHA
   CLC
   LDA   FP
   ADC   #$7E
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
   RTS
.if_end@0:
   LDY   #$11
   LDA   (FP),Y
   LDY   #$2A
   STA   (FP),Y
   CLC
   ADC   #$01
   INY
   STA   (FP),Y
   LDY   #$11
   STA   (FP),Y
   JMP   .loop@1_start
.loop@1_break:
   LDA   #<gs7
   LDY   #$2C
   STA   (FP),Y
   LDA   #>gs7
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$01
   LDY   #$2E
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$2F
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$30
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$31
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$32
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
   LDY   #$34
   STA   (FP),Y
   LDY   #$0B
   LDA   (FP),Y
   LDY   #$35
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$7D
   AND   (FP),Y
   LDY   #$37
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$7E
   AND   (FP),Y
   LDY   #$38
   STA   (FP),Y
   LDY   #$33
   LDA   (FP),Y
   LDY   #$37
   AND   (FP),Y
   LDY   #$39
   STA   (FP),Y
   LDY   #$34
   LDA   (FP),Y
   LDY   #$38
   AND   (FP),Y
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
   ORA   #$00
   BMI   .sx_neg@6
   LDA   #$00
   JMP   .sx_done@7
.sx_neg@6:
   LDA   #$FF
.sx_done@7:
   LDY   #$3D
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
   LDY   #$0B
   LDA   (FP),Y
   LDY   #$3F
   STA   (FP),Y
   CLC
   ADC   #$01
   INY
   STA   (FP),Y
   LDY   #$0B
   STA   (FP),Y
   JMP   .loop@0_start
.loop@0_break:
   LDA   #<gs7
   LDY   #$41
   STA   (FP),Y
   LDA   #>gs7
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$03
   LDY   #$43
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$44
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$45
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$46
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ORA   #$03
   LDY   #$47
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$48
   STA   (FP),Y
   LDY   #$43
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
   LDY   #$47
   LDA   (FP),Y
   LDY   #$49
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$4A
   STA   (FP),Y
   LDA   #$07
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$49
   LDA   (FP),Y
   LDY   #$4B
   AND   (FP),Y
   LDY   #$4D
   STA   (FP),Y
   LDY   #$4A
   LDA   (FP),Y
   LDY   #$4C
   AND   (FP),Y
   LDY   #$4E
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CMP   #$01
   BEQ   .cmp_true@8
   LDA   #$00
   JMP   .cmp_end@9
.cmp_true@8:
   LDA   #$01
.cmp_end@9:
   LDY   #$4F
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .case@3
   LDY   #$4D
   LDA   (FP),Y
   CMP   #$05
   BEQ   .cmp_true@10
   LDA   #$00
   JMP   .cmp_end@11
.cmp_true@10:
   LDA   #$01
.cmp_end@11:
   LDY   #$51
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@1
   JMP   .case@4
.lb_skip@1:
   LDY   #$4D
   LDA   (FP),Y
   CMP   #$07
   BEQ   .cmp_true@12
   LDA   #$00
   JMP   .cmp_end@13
.cmp_true@12:
   LDA   #$01
.cmp_end@13:
   LDY   #$53
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@0
   JMP   .case@7
.lb_skip@0:
   JMP   .switch@2_break
.case@3:
   LDA   #$A8
   LDY   #$55
   STA   (FP),Y
   LDA   #$AB
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
   ORA   #$00
   BMI   .sx_neg@14
   LDA   #$00
   JMP   .sx_done@15
.sx_neg@14:
   LDA   #$FF
.sx_done@15:
   LDY   #$59
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
   JMP   .switch@2_break
.case@4:
   LDA   #$00
   LDY   #$5B
   STA   (FP),Y
   INY
   STA   (FP),Y
.loop@5_start:
   LDY   #$5D
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
   BVC   .cmp_novf@16
   EOR   #$80
.cmp_novf@16:
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
   BEQ   .switch@2_break
   LDY   #$09
   LDA   (FP),Y
   LDY   #$61
   STA   (FP),Y
   SEC
   SBC   #$01
   INY
   STA   (FP),Y
   LDY   #$09
   STA   (FP),Y
   JMP   .loop@5_continue
.loop@6_continue:
   LDY   #$6E
   LDA   (FP),Y
   INY
   STA   (FP),Y
   CLC
   ADC   #$01
   INY
   STA   (FP),Y
   LDY   #$6E
   STA   (FP),Y
   INY
   LDA   (FP),Y
   LDY   #$71
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
   LDY   #$73
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .loop@6_continue
.loop@5_continue:
   LDY   #$5C
   LDA   (FP),Y
   LDY   #$75
   STA   (FP),Y
   CLC
   ADC   #$01
   INY
   STA   (FP),Y
   LDY   #$5C
   STA   (FP),Y
   JMP   .loop@5_start
.case@7:
.switch@2_break:
   LDA   #$FF
   LDY   #$77
   STA   (FP),Y
   AND   #$7F
   INY
   STA   (FP),Y

   ; epilogue
   PHA
   CLC
   LDA   FP
   ADC   #$7E
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

   ; prologue: 0 arg bytes, 149 local bytes
   SEC
   LDA   SSP
   SBC   #$97
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$96
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
   LDA   #$01
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
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
   INY
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
   ORA   (FP),Y
   BEQ   .cond_else@1
   LDY   #$07
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
   LDA   #$05
   STA   HARGS+4
   JSR   asl32
   LDA   HARGS+5
   LDY   #$0F
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
   LDY   #$0F
   LDA   (FP),Y
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
   JMP   .cond_end@2
.cond_else@1:
   LDY   #$0D
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@3
   LDA   g2
   ORA   g2+1
   BNE   .or_true@3
   LDA   #$00
   LDY   #$17
   STA   (FP),Y
   INY
   STA   (FP),Y
   JMP   .or_end@4
.or_true@3:
   LDA   #$01
   LDY   #$17
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@4:
   LDY   #$17
   LDA   (FP),Y
   LDY   #$19
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$1A
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@18
   LDA   #$00
   JMP   .sx_done@19
.sx_neg@18:
   LDA   #$FF
.sx_done@19:
   LDY   #$1B
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$19
   LDA   (FP),Y
   LDY   #$13
   STA   (FP),Y
   LDY   #$1A
   LDA   (FP),Y
   LDY   #$14
   STA   (FP),Y
   LDY   #$1B
   LDA   (FP),Y
   LDY   #$15
   STA   (FP),Y
   INY
   STA   (FP),Y
.cond_end@2:
   LDY   #$13
   LDA   (FP),Y
   LDY   #$1D
   STA   (FP),Y
   LDY   #$14
   LDA   (FP),Y
   LDY   #$1E
   STA   (FP),Y
   LDY   #$15
   LDA   (FP),Y
   LDY   #$1F
   STA   (FP),Y
   LDY   #$16
   LDA   (FP),Y
   LDY   #$20
   STA   (FP),Y
   LDY   #$1D
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
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
.loop@8_continue:
   LDY   #$22
   LDA   (FP),Y
   INY
   STA   (FP),Y
   CLC
   ADC   #$01
   INY
   STA   (FP),Y
   LDY   #$22
   STA   (FP),Y
   INY
   LDA   (FP),Y
   LDY   #$25
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
   BVC   .cmp_novf@20
   EOR   #$80
.cmp_novf@20:
   ASL
   LDA   #$00
   ROL
   LDY   #$27
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
   LDA   #$5A
   LDY   #$29
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   ORA   #$00
   BPL   .sx_done@22
   LDA   #$FF
.sx_done@22:
   LDY   #$2B
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$07
   LDA   (FP),Y
   SEC
   LDY   #$29
   SBC   (FP),Y
   LDY   #$2D
   STA   (FP),Y
   LDY   #$08
   LDA   (FP),Y
   LDY   #$2A
   SBC   (FP),Y
   LDY   #$2E
   STA   (FP),Y
   LDY   #$09
   LDA   (FP),Y
   LDY   #$2B
   SBC   (FP),Y
   LDY   #$2F
   STA   (FP),Y
   LDY   #$0A
   LDA   (FP),Y
   LDY   #$2C
   SBC   (FP),Y
   LDY   #$30
   STA   (FP),Y
   LDY   #$2D
   LDA   (FP),Y
   LDY   #$07
   STA   (FP),Y
   LDY   #$2E
   LDA   (FP),Y
   LDY   #$08
   STA   (FP),Y
   LDY   #$2F
   LDA   (FP),Y
   LDY   #$09
   STA   (FP),Y
   LDY   #$30
   LDA   (FP),Y
   LDY   #$0A
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   LDY   #$31
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@23
   LDA   #$00
   JMP   .sx_done@24
.sx_neg@23:
   LDA   #$FF
.sx_done@24:
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
   ORA   #$00
   BMI   .sx_neg@25
   LDA   #$00
   JMP   .sx_done@26
.sx_neg@25:
   LDA   #$FF
.sx_done@26:
   LDY   #$35
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$2D
   LDA   (FP),Y
   LDY   #$33
   ORA   (FP),Y
   LDY   #$37
   STA   (FP),Y
   LDY   #$2E
   LDA   (FP),Y
   LDY   #$34
   ORA   (FP),Y
   LDY   #$38
   STA   (FP),Y
   LDY   #$2F
   LDA   (FP),Y
   LDY   #$35
   ORA   (FP),Y
   LDY   #$39
   STA   (FP),Y
   LDY   #$30
   LDA   (FP),Y
   LDY   #$36
   ORA   (FP),Y
   LDY   #$3A
   STA   (FP),Y
   LDY   #$37
   LDA   (FP),Y
   LDY   #$3B
   STA   (FP),Y
   LDY   #$38
   LDA   (FP),Y
   LDY   #$3C
   STA   (FP),Y
   LDA   #<gs7
   INY
   STA   (FP),Y
   LDA   #>gs7
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$03
   LDY   #$3F
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$40
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$3B
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$3C
   LDA   (FP),Y
   LDY   #$01
   STA   (DPTR),Y
   LDA   gp8
   STA   DPTR
   LDA   gp8+1
   STA   DPTR+1
   DEY
   LDA   (DPTR),Y
   LDY   #$41
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$42
   STA   (FP),Y
   LDY   #$02
   LDA   (DPTR),Y
   LDY   #$43
   STA   (FP),Y
   LSR
   DEY
   LDA   (FP),Y
   ROR
   LDY   #$46
   STA   (FP),Y
   LDY   #$41
   LDA   (FP),Y
   ROR
   LDY   #$45
   STA   (FP),Y
   LDA   #$00
   ROR
   DEY
   STA   (FP),Y
   AND   #$FF
   LDY   #$47
   STA   (FP),Y
   LDY   #$45
   LDA   (FP),Y
   LDY   #$48
   STA   (FP),Y
   LDY   #$46
   LDA   (FP),Y
   AND   #$07
   LDY   #$49
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   LDY   #$4A
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@27
   LDA   #$00
   JMP   .sx_done@28
.sx_neg@27:
   LDA   #$FF
.sx_done@28:
   LDY   #$4B
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$4C
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDA   #$00
   LDY   #$4D
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$4E
   STA   (FP),Y
   LDY   #$02
   STA   (FP),Y
   LDY   #$4F
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@29
   LDA   #$00
   JMP   .sx_done@30
.sx_neg@29:
   LDA   #$FF
.sx_done@30:
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
   ORA   #$00
   BMI   .sx_neg@31
   LDA   #$00
   JMP   .sx_done@32
.sx_neg@31:
   LDA   #$FF
.sx_done@32:
   LDY   #$53
   STA   (FP),Y
   LDY   #$51
   LDA   (FP),Y
   LDY   #$54
   STA   (FP),Y
   LDY   #$52
   LDA   (FP),Y
   LDY   #$55
   STA   (FP),Y
   LDY   #$53
   LDA   (FP),Y
   AND   #$07
   LDY   #$56
   STA   (FP),Y
   LDY   #$47
   LDA   (FP),Y
   CLC
   LDY   #$54
   ADC   (FP),Y
   LDY   #$57
   STA   (FP),Y
   LDY   #$48
   LDA   (FP),Y
   LDY   #$55
   ADC   (FP),Y
   LDY   #$58
   STA   (FP),Y
   LDY   #$49
   LDA   (FP),Y
   LDY   #$56
   ADC   (FP),Y
   LDY   #$59
   STA   (FP),Y
   LDY   #$57
   LDA   (FP),Y
   LDY   #$5A
   STA   (FP),Y
   LDY   #$58
   LDA   (FP),Y
   LDY   #$5B
   STA   (FP),Y
   LDY   #$59
   LDA   (FP),Y
   AND   #$07
   LDY   #$5C
   STA   (FP),Y
   LDY   #$5A
   LDA   (FP),Y
   LDY   #$5D
   STA   (FP),Y
   LDY   #$5B
   LDA   (FP),Y
   LDY   #$5E
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
   BEQ   .cmp_true@33
   LDA   #$00
   JMP   .cmp_end@34
.cmp_true@33:
   LDA   #$01
.cmp_end@34:
   LDY   #$63
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   LDY   #$22
   LDA   (FP),Y
   BNE   .lb_skip@7
   JMP   .if_end@5
.lb_skip@7:
   LDA   g2
   STA   HARGS
   LDA   g2+1
   STA   HARGS+1
   LDA   #$05
   STA   HARGS+2
   LDA   #$00
   STA   HARGS+3
   JSR   mul16
   LDA   HARGS+4
   LDY   #$65
   STA   (FP),Y
   LDA   HARGS+5
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
   LDY   #$67
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$68
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$67
   LDA   (FP),Y
   EOR   g3
   LDY   #$6A
   STA   (FP),Y
   LDY   #$68
   LDA   (FP),Y
   EOR   g3+1
   LDY   #$6B
   STA   (FP),Y
   LDY   #$69
   LDA   (FP),Y
   EOR   g3+2
   LDY   #$6C
   STA   (FP),Y
   LDY   #$6A
   LDA   (FP),Y
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
   LDA   #$00
   INY
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
.if_end@5:
   LDY   #$22
   LDA   (FP),Y
   LDY   #$71
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$73
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$74
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@35
   LDA   #$00
   JMP   .sx_done@36
.sx_neg@35:
   LDA   #$FF
.sx_done@36:
   LDY   #$75
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$07
   LDA   (FP),Y
   LDY   #$73
   EOR   (FP),Y
   LDY   #$77
   STA   (FP),Y
   LDY   #$08
   LDA   (FP),Y
   LDY   #$74
   EOR   (FP),Y
   LDY   #$78
   STA   (FP),Y
   LDY   #$09
   LDA   (FP),Y
   LDY   #$75
   EOR   (FP),Y
   LDY   #$79
   STA   (FP),Y
   LDY   #$0A
   LDA   (FP),Y
   LDY   #$76
   EOR   (FP),Y
   LDY   #$7A
   STA   (FP),Y
   LDY   #$77
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   INY
   ORA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@6
   JMP   .loop@8_continue
.lb_skip@6:
   LDY   #$0D
   LDA   (FP),Y
   CLC
   ADC   #$01
   LDY   #$7B
   STA   (FP),Y
   LDY   #$0E
   LDA   (FP),Y
   ADC   #$00
   LDY   #$7C
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$0D
   STA   (FP),Y
   LDY   #$7C
   LDA   (FP),Y
   LDY   #$0E
   STA   (FP),Y
   LDY   #$7B
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@5
   JMP   .loop@8_break
.lb_skip@5:
   LDA   #$00
   LDY   #$7D
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   CLC
   ADC   #$01
   INY
   STA   (FP),Y
   LDY   #$7E
   STA   (FP),Y
   INY
   LDA   (FP),Y
   LDY   #$81
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
   BVC   .cmp_novf@37
   EOR   #$80
.cmp_novf@37:
   ASL
   LDA   #$00
   ROL
   LDY   #$83
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@4
   JMP   .loop@8_continue
.lb_skip@4:
   LDA   #$80
   LDY   #$85
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$8C
   STA   (FP),Y
   LSR
   STA   (FP),Y
   LDY   #$87
   LDA   (FP),Y
   LDY   #$8B
   STA   (FP),Y
   ROR
   STA   (FP),Y
   LDY   #$86
   LDA   (FP),Y
   LDY   #$8A
   STA   (FP),Y
   ROR
   STA   (FP),Y
   LDY   #$85
   LDA   (FP),Y
   LDY   #$89
   STA   (FP),Y
   ROR
   STA   (FP),Y
   LDY   #$8D
   STA   (FP),Y
   LDY   #$8A
   LDA   (FP),Y
   LDY   #$8E
   STA   (FP),Y
   LDY   #$0D
   LDA   (FP),Y
   LDY   #$8F
   STA   (FP),Y
   LDY   #$0E
   LDA   (FP),Y
   LDY   #$90
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$04
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDY   #$8D
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$8E
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$8F
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$90
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   JSR   f10
   LDY   #$91
   STA   (FP),Y
   JMP   .loop@8_continue
.loop@8_break:
   JSR   fin0
   JSR   fin1
   JSR   fin2
   LDA   acc
   LDY   #$92
   STA   (FP),Y
   LDA   acc+1
   AND   #$7F
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$94
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$95
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
   ADC   #$97
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$96
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
   DC.L  $00000080

g2:
   DC.W  $0002

g3:
   DC.B  $7F
   DS.B  2

ga6:
   DC.B  $01, $00, $00, $00

gs7:
   DS.B  1
   DC.W  $8000
   DC.W  $7FFF

gp8:
   DC.W  g3

gp9:
   DC.W  g2

__zp_init:
   SUBROUTINE

   LDA   #$00
   STA   z4
   STA   z4+2
   STA   z4+3
   STA   z4+4
   STA   z4+5
   STA   z4+6
   STA   z4+7
   STA   z5+1
   STA   z5+2
   STA   z5+3
   STA   z5+4
   STA   z5+5
   STA   z5+6
   STA   z5+7
   LDA   #$01
   STA   z4+1
   LDA   #$FF
   STA   z5
   RTS
