SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
z4	EQU	$26

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
   LDA   g3
   LDY   #$0B
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
   LDA   z4
   LDY   #$0F
   STA   (FP),Y
   LDA   z4+1
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

   LDA   ga5
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
   LDA   ga5+1
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
   LDA   ga5+2
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
   LDA   ga5+3
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

   LDA   #<gs6
   LDY   #$01
   STA   (FP),Y
   LDA   #>gs6
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$01
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
   LDA   (DPTR),Y
   LDY   #$0A
   STA   (FP),Y
   LDY   #$06
   LDA   (DPTR),Y
   LDY   #$0B
   STA   (FP),Y
   LDY   #$07
   LDA   (DPTR),Y
   LDY   #$0C
   STA   (FP),Y
   LDY   #$05
   LDA   (FP),Y
   LDY   #$0D
   STA   (FP),Y
   LDY   #$06
   LDA   (FP),Y
   LDY   #$0E
   STA   (FP),Y
   LDY   #$07
   LDA   (FP),Y
   LDY   #$0F
   STA   (FP),Y
   LDY   #$08
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
   LDA   #<gs6
   LDY   #$11
   STA   (FP),Y
   LDA   #>gs6
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$09
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

f9:
   SUBROUTINE

   ; prologue: 2 arg bytes, 134 local bytes
   SEC
   LDA   SSP
   SBC   #$88
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$87
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
   LDA   #$05
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   ORA   #$00
   BPL   .sx_done@1
   LDA   #$FF
.sx_done@1:
   LDY   #$0B
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$05
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
   LDY   #$0D
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
   LDY   #$0D
   LDA   (FP),Y
   LDY   #$05
   STA   (FP),Y
   LDY   #$0E
   LDA   (FP),Y
   LDY   #$06
   STA   (FP),Y
   LDY   #$0F
   LDA   (FP),Y
   LDY   #$07
   STA   (FP),Y
   LDY   #$10
   LDA   (FP),Y
   LDY   #$08
   STA   (FP),Y
   LDA   #$FF
   LDY   #$11
   STA   (FP),Y
   INY
   STA   (FP),Y
   ORA   (FP),Y
   BNE   .lb_skip@2
   JMP   .if_else@1
.lb_skip@2:
   LDA   ga5+1
   LDY   #$13
   STA   (FP),Y
   ORA   #$00
   BEQ   .lnot_true@2
   LDA   #$00
   JMP   .lnot_end@3
.lnot_true@2:
   LDA   #$01
.lnot_end@3:
   LDY   #$14
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
   BMI   .sx_neg@4
   LDA   #$00
   JMP   .sx_done@5
.sx_neg@4:
   LDA   #$FF
.sx_done@5:
   LDY   #$18
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
   LDA   #$02
   LDY   #$1A
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDA   gp7
   STA   DPTR
   LDA   gp7+1
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$1C
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   SEC
   DEY
   SBC   (FP),Y
   LDY   #$1F
   STA   (FP),Y
   LDA   #$00
   DEY
   SBC   (FP),Y
   LDY   #$20
   STA   (FP),Y
   LDY   #$1A
   LDA   (FP),Y
   CLC
   LDY   #$1F
   ADC   (FP),Y
   LDY   #$21
   STA   (FP),Y
   LDY   #$1B
   LDA   (FP),Y
   LDY   #$20
   ADC   (FP),Y
   LDY   #$22
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@1
   JMP   .if_end@0
.lb_skip@1:
   LDA   z4
   LDY   #$23
   STA   (FP),Y
   ASL
   STA   (FP),Y
   LDA   z4+1
   INY
   STA   (FP),Y
   ROL
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   z4
   INY
   LDA   (FP),Y
   STA   z4+1
   DEY
   LDA   (FP),Y
   LDY   #$25
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$26
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
   ADC   #$8A
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$87
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
.if_else@1:
   LDA   #$00
   LDY   #$4F
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
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   gp8
   STA   DPTR
   LDA   gp8+1
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
   LDY   #$51
   LDA   (FP),Y
   LDY   #$59
   AND   (FP),Y
   LDY   #$61
   STA   (FP),Y
   LDY   #$52
   LDA   (FP),Y
   LDY   #$5A
   AND   (FP),Y
   LDY   #$62
   STA   (FP),Y
   LDY   #$53
   LDA   (FP),Y
   LDY   #$5B
   AND   (FP),Y
   LDY   #$63
   STA   (FP),Y
   LDY   #$54
   LDA   (FP),Y
   LDY   #$5C
   AND   (FP),Y
   LDY   #$64
   STA   (FP),Y
   LDY   #$55
   LDA   (FP),Y
   LDY   #$5D
   AND   (FP),Y
   LDY   #$65
   STA   (FP),Y
   LDY   #$56
   LDA   (FP),Y
   LDY   #$5E
   AND   (FP),Y
   LDY   #$66
   STA   (FP),Y
   LDY   #$57
   LDA   (FP),Y
   LDY   #$5F
   AND   (FP),Y
   LDY   #$67
   STA   (FP),Y
   LDY   #$58
   LDA   (FP),Y
   LDY   #$60
   AND   (FP),Y
   LDY   #$68
   STA   (FP),Y
   LDY   #$61
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
   BNE   .lb_skip@0
   JMP   .if_else@4
.lb_skip@0:
   LDA   g3
   CLC
   ADC   #$01
   LDY   #$69
   STA   (FP),Y
   STA   g3
   LDA   #<gs6
   INY
   STA   (FP),Y
   LDA   #>gs6
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$09
   LDY   #$6C
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$6D
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$6E
   STA   (FP),Y
   ORA   #$00
   BNE   .or_true@5
   LDA   gp7
   STA   DPTR
   LDA   gp7+1
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$6F
   STA   (FP),Y
   ORA   #$00
   BNE   .or_true@5
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   JMP   .or_end@6
.or_true@5:
   LDA   #$01
   LDY   #$70
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@6:
   LDY   #$70
   LDA   (FP),Y
   LDY   #$72
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$73
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@12
   LDA   #$00
   JMP   .sx_done@13
.sx_neg@12:
   LDA   #$FF
.sx_done@13:
   LDY   #$74
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$72
   LDA   (FP),Y
   LDY   #$05
   STA   (FP),Y
   LDY   #$73
   LDA   (FP),Y
   LDY   #$06
   STA   (FP),Y
   LDY   #$74
   LDA   (FP),Y
   LDY   #$07
   STA   (FP),Y
   INY
   STA   (FP),Y
   JMP   .if_end@3
.if_else@4:
   LDA   g3
   LDY   #$76
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   EOR   @2.s12
   LDY   #$78
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   EOR   @2.s12+1
   LDY   #$79
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$7A
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$7B
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@14
   LDA   #$00
   JMP   .sx_done@15
.sx_neg@14:
   LDA   #$FF
.sx_done@15:
   LDY   #$7C
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
   LDY   #$7A
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$7B
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$7C
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$7D
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   JSR   mix
.if_end@3:
.if_end@0:
   LDA   #$01
   LDY   #$7E
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
   LDY   #$80
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$81
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$82
   STA   (FP),Y
   LDA   z4
   DEY
   EOR   (FP),Y
   LDY   #$83
   STA   (FP),Y
   LDA   z4+1
   DEY
   EOR   (FP),Y
   LDY   #$84
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$85
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$86
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
   ADC   #$8A
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$87
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

   LDA   #$80
   LDY   #$01
   STA   (FP),Y
   INY
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
   LDA   #$00
   INY
   STA   (FP),Y
   LDA   #$01
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
   LDA   ga5
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDA   #$04
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$10
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
   LDY   #$14
   STA   (FP),Y
   LDA   HARGS+7
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
   BMI   .sx_neg@18
   LDA   #$00
   JMP   .sx_done@19
.sx_neg@18:
   LDA   #$FF
.sx_done@19:
   LDY   #$18
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
   LDY   #$16
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
   LDA   gp7
   STA   DPTR
   LDA   gp7+1
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
   LSR
   LSR
   LSR
   LSR
   LDY   #$22
   STA   (FP),Y
   LDY   #$1F
   LDA   (FP),Y
   ASL
   ASL
   ASL
   ASL
   LDY   #$21
   STA   (FP),Y
   LDY   #$23
   STA   (FP),Y
   LDA   gp7
   STA   DPTR
   LDA   gp7+1
   STA   DPTR+1
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$23
   LDA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$26
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$27
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@20
   LDA   #$00
   JMP   .sx_done@21
.sx_neg@20:
   LDA   #$FF
.sx_done@21:
   LDY   #$28
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
   LDY   #$26
   AND   (FP),Y
   LDY   #$2E
   STA   (FP),Y
   LDA   g2+1
   LDY   #$27
   AND   (FP),Y
   LDY   #$2F
   STA   (FP),Y
   LDA   g2+2
   LDY   #$28
   AND   (FP),Y
   LDY   #$30
   STA   (FP),Y
   LDA   g2+3
   LDY   #$29
   AND   (FP),Y
   LDY   #$31
   STA   (FP),Y
   LDA   g2+4
   LDY   #$2A
   AND   (FP),Y
   LDY   #$32
   STA   (FP),Y
   LDA   g2+5
   LDY   #$2B
   AND   (FP),Y
   LDY   #$33
   STA   (FP),Y
   LDA   g2+6
   LDY   #$2C
   AND   (FP),Y
   LDY   #$34
   STA   (FP),Y
   LDA   g2+7
   LDY   #$2D
   AND   (FP),Y
   LDY   #$35
   STA   (FP),Y
   LDY   #$2E
   LDA   (FP),Y
   LDY   #$36
   STA   (FP),Y
   LDY   #$2F
   LDA   (FP),Y
   LDY   #$37
   STA   (FP),Y
   LDA   #$07
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$36
   LDA   (FP),Y
   LDY   #$38
   AND   (FP),Y
   LDY   #$3A
   STA   (FP),Y
   LDY   #$37
   LDA   (FP),Y
   LDY   #$39
   AND   (FP),Y
   LDY   #$3B
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   BEQ   .cmp_true@22
   LDA   #$00
   JMP   .cmp_end@23
.cmp_true@22:
   LDA   #$01
.cmp_end@23:
   LDY   #$3C
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@7
   JMP   .switch@4_break
.lb_skip@7:
   LDY   #$3A
   LDA   (FP),Y
   CMP   #$04
   BEQ   .cmp_true@24
   LDA   #$00
   JMP   .cmp_end@25
.cmp_true@24:
   LDA   #$01
.cmp_end@25:
   LDY   #$3E
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@6
   JMP   .switch@4_break
.lb_skip@6:
   LDA   gp7
   STA   DPTR
   LDA   gp7+1
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$40
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDA   ga5+1
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$41
   LDA   (FP),Y
   LDY   #$44
   CMP   (FP),Y
   BNE   .cmp_true@26
   LDA   #$00
   JMP   .cmp_end@27
.cmp_true@26:
   LDA   #$01
.cmp_end@27:
   LDY   #$46
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
   CMP   #$05
   BEQ   .cmp_true@28
   LDA   #$00
   JMP   .cmp_end@29
.cmp_true@28:
   LDA   #$01
.cmp_end@29:
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
   JMP   .switch@8_break
.lb_skip@5:
   LDA   #<gs6
   LDY   #$4E
   STA   (FP),Y
   LDA   #>gs6
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$09
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
   INY
   STA   (FP),Y
   LDA   #$00
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
.mul_loop@30:
   BCC   .mul_skip@31
   CLC
   LDY   #$53
   ADC   (FP),Y
.mul_skip@31:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@30
   LDY   #$58
   STA   (FP),Y
   LDA   HARGS
   DEY
   STA   (FP),Y
   LDY   #$59
   STA   (FP),Y
   LDA   #<gs6
   INY
   STA   (FP),Y
   LDA   #>gs6
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$09
   LDY   #$5C
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$5D
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$59
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDA   #<gs6
   LDY   #$5E
   STA   (FP),Y
   LDA   #>gs6
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$01
   LDY   #$60
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
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
   LDY   #$04
   LDA   (DPTR),Y
   LDY   #$66
   STA   (FP),Y
   LDY   #$05
   LDA   (DPTR),Y
   LDY   #$67
   STA   (FP),Y
   LDY   #$06
   LDA   (DPTR),Y
   LDY   #$68
   STA   (FP),Y
   LDY   #$07
   LDA   (DPTR),Y
   LDY   #$69
   STA   (FP),Y
   LDA   #$81
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   ORA   #$00
   BPL   .sx_done@33
   LDA   #$FF
.sx_done@33:
   LDY   #$6C
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
   LDY   #$62
   LDA   (FP),Y
   LDY   #$6A
   ORA   (FP),Y
   LDY   #$72
   STA   (FP),Y
   LDY   #$63
   LDA   (FP),Y
   LDY   #$6B
   ORA   (FP),Y
   LDY   #$73
   STA   (FP),Y
   LDY   #$64
   LDA   (FP),Y
   LDY   #$6C
   ORA   (FP),Y
   LDY   #$74
   STA   (FP),Y
   LDY   #$65
   LDA   (FP),Y
   LDY   #$6D
   ORA   (FP),Y
   LDY   #$75
   STA   (FP),Y
   LDY   #$66
   LDA   (FP),Y
   LDY   #$6E
   ORA   (FP),Y
   LDY   #$76
   STA   (FP),Y
   LDY   #$67
   LDA   (FP),Y
   LDY   #$6F
   ORA   (FP),Y
   LDY   #$77
   STA   (FP),Y
   LDY   #$68
   LDA   (FP),Y
   LDY   #$70
   ORA   (FP),Y
   LDY   #$78
   STA   (FP),Y
   LDY   #$69
   LDA   (FP),Y
   LDY   #$71
   ORA   (FP),Y
   LDY   #$79
   STA   (FP),Y
   LDY   #$60
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
   LDY   #$74
   LDA   (FP),Y
   LDY   #$02
   STA   (DPTR),Y
   LDY   #$75
   LDA   (FP),Y
   LDY   #$03
   STA   (DPTR),Y
   LDY   #$76
   LDA   (FP),Y
   LDY   #$04
   STA   (DPTR),Y
   LDY   #$77
   LDA   (FP),Y
   LDY   #$05
   STA   (DPTR),Y
   LDY   #$78
   LDA   (FP),Y
   LDY   #$06
   STA   (DPTR),Y
   LDY   #$79
   LDA   (FP),Y
   LDY   #$07
   STA   (DPTR),Y
   LDY   #$72
   LDA   (FP),Y
   LDY   #$7A
   STA   (FP),Y
   LDY   #$73
   LDA   (FP),Y
   LDY   #$7B
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   g1
   INY
   LDA   (FP),Y
   STA   g1+1
.switch@8_break:
   LDA   #$5A
   LDY   #$7C
   STA   (FP),Y
   LDA   #$87
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   SEC
   LDY   #$0D
   SBC   (FP),Y
   LDY   #$7D
   LDA   (FP),Y
   LDY   #$0E
   SBC   (FP),Y
   LDA   #$00
   ROL
   LDY   #$7E
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
   LDY   #$7E
   LDA   (FP),Y
   LDY   #$80
   AND   (FP),Y
   LDY   #$82
   STA   (FP),Y
   LDY   #$7F
   LDA   (FP),Y
   LDY   #$81
   AND   (FP),Y
   LDY   #$83
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CMP   #$02
   BEQ   .cmp_true@34
   LDA   #$00
   JMP   .cmp_end@35
.cmp_true@34:
   LDA   #$01
.cmp_end@35:
   LDY   #$84
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@4
   JMP   .switch@10_break
.lb_skip@4:
   LDY   #$82
   LDA   (FP),Y
   CMP   #$06
   BEQ   .cmp_true@36
   LDA   #$00
   JMP   .cmp_end@37
.cmp_true@36:
   LDA   #$01
.cmp_end@37:
   LDY   #$86
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .case@12
   LDY   #$82
   LDA   (FP),Y
   CMP   #$07
   BEQ   .cmp_true@38
   LDA   #$00
   JMP   .cmp_end@39
.cmp_true@38:
   LDA   #$01
.cmp_end@39:
   LDY   #$88
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@3
   JMP   .case@13
.lb_skip@3:
   JMP   .switch@10_break
.case@12:
   LDA   #<gs6
   LDY   #$8A
   STA   (FP),Y
   LDA   #>gs6
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$01
   LDY   #$8C
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$8D
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$8E
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$8F
   STA   (FP),Y
   LDY   #$02
   LDA   (DPTR),Y
   LDY   #$90
   STA   (FP),Y
   LDY   #$03
   LDA   (DPTR),Y
   LDY   #$91
   STA   (FP),Y
   LDY   #$04
   LDA   (DPTR),Y
   LDY   #$92
   STA   (FP),Y
   LDY   #$05
   LDA   (DPTR),Y
   LDY   #$93
   STA   (FP),Y
   LDY   #$06
   LDA   (DPTR),Y
   LDY   #$94
   STA   (FP),Y
   LDY   #$07
   LDA   (DPTR),Y
   LDY   #$95
   STA   (FP),Y
   LDY   #$8E
   LDA   (FP),Y
   EOR   #$FF
   LDY   #$96
   STA   (FP),Y
   LDY   #$8F
   LDA   (FP),Y
   EOR   #$FF
   LDY   #$97
   STA   (FP),Y
   LDY   #$90
   LDA   (FP),Y
   EOR   #$FF
   LDY   #$98
   STA   (FP),Y
   LDY   #$91
   LDA   (FP),Y
   EOR   #$FF
   LDY   #$99
   STA   (FP),Y
   LDY   #$92
   LDA   (FP),Y
   EOR   #$FF
   LDY   #$9A
   STA   (FP),Y
   LDY   #$93
   LDA   (FP),Y
   EOR   #$FF
   LDY   #$9B
   STA   (FP),Y
   LDY   #$94
   LDA   (FP),Y
   EOR   #$FF
   LDY   #$9C
   STA   (FP),Y
   LDY   #$95
   LDA   (FP),Y
   EOR   #$FF
   LDY   #$9D
   STA   (FP),Y
   LDY   #$96
   LDA   (FP),Y
   LDY   #$9E
   STA   (FP),Y
   LDY   #$97
   LDA   (FP),Y
   LDY   #$9F
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$0D
   STA   (FP),Y
   LDY   #$9F
   LDA   (FP),Y
   LDY   #$0E
   STA   (FP),Y
.case@13:
.switch@10_break:
.switch@4_break:
   LDA   #<gs6
   LDY   #$A0
   STA   (FP),Y
   LDA   #>gs6
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$01
   LDY   #$A2
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$A3
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$A4
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$A5
   STA   (FP),Y
   LDY   #$02
   LDA   (DPTR),Y
   LDY   #$A6
   STA   (FP),Y
   LDY   #$03
   LDA   (DPTR),Y
   LDY   #$A7
   STA   (FP),Y
   LDY   #$04
   LDA   (DPTR),Y
   LDY   #$A8
   STA   (FP),Y
   LDY   #$05
   LDA   (DPTR),Y
   LDY   #$A9
   STA   (FP),Y
   LDY   #$06
   LDA   (DPTR),Y
   LDY   #$AA
   STA   (FP),Y
   LDY   #$07
   LDA   (DPTR),Y
   LDY   #$AB
   STA   (FP),Y
   LDY   #$A4
   LDA   (FP),Y
   ASL
   INY
   LDA   (FP),Y
   ROL
   LDY   #$AC
   STA   (FP),Y
   LDY   #$A6
   LDA   (FP),Y
   ROL
   LDY   #$AD
   STA   (FP),Y
   LDY   #$A7
   LDA   (FP),Y
   ROL
   LDY   #$AE
   STA   (FP),Y
   LDY   #$A8
   LDA   (FP),Y
   ROL
   LDY   #$AF
   STA   (FP),Y
   LDY   #$A9
   LDA   (FP),Y
   ROL
   LDY   #$B0
   STA   (FP),Y
   LDY   #$AA
   LDA   (FP),Y
   ROL
   LDY   #$B1
   STA   (FP),Y
   LDY   #$AB
   LDA   (FP),Y
   ROL
   LDY   #$B2
   STA   (FP),Y
   LDA   #$00
   ROL
   INY
   STA   (FP),Y
   LDY   #$A2
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$AC
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$AD
   LDA   (FP),Y
   LDY   #$01
   STA   (DPTR),Y
   LDY   #$AE
   LDA   (FP),Y
   LDY   #$02
   STA   (DPTR),Y
   LDY   #$AF
   LDA   (FP),Y
   LDY   #$03
   STA   (DPTR),Y
   LDY   #$B0
   LDA   (FP),Y
   LDY   #$04
   STA   (DPTR),Y
   LDY   #$B1
   LDA   (FP),Y
   LDY   #$05
   STA   (DPTR),Y
   LDY   #$B2
   LDA   (FP),Y
   LDY   #$06
   STA   (DPTR),Y
   LDY   #$B3
   LDA   (FP),Y
   LDY   #$07
   STA   (DPTR),Y
   LDA   #$00
   LDY   #$B4
   STA   (FP),Y
   INY
   STA   (FP),Y
.loop@14_continue:
   LDY   #$B5
   LDA   (FP),Y
   INY
   STA   (FP),Y
   CLC
   ADC   #$01
   INY
   STA   (FP),Y
   LDY   #$B5
   STA   (FP),Y
   INY
   LDA   (FP),Y
   LDY   #$B8
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
   BVC   .cmp_novf@40
   EOR   #$80
.cmp_novf@40:
   ASL
   LDA   #$00
   ROL
   LDY   #$BA
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .loop@14_break
   LDY   #$02
   LDA   (FP),Y
   LDY   #$BC
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LSR
   DEY
   LDA   (FP),Y
   ROR
   LDY   #$BE
   STA   (FP),Y
   LDY   #$C0
   STA   (FP),Y
   LDY   #$02
   STA   (FP),Y
   LDY   #$C1
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
   LDY   #$C1
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
   JMP   .loop@14_continue
.loop@14_break:
   LDA   #<gs6
   LDY   #$C9
   STA   (FP),Y
   LDA   #>gs6
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$01
   LDY   #$CB
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$CC
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$CD
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$CE
   STA   (FP),Y
   LDY   #$02
   LDA   (DPTR),Y
   LDY   #$CF
   STA   (FP),Y
   LDY   #$03
   LDA   (DPTR),Y
   LDY   #$D0
   STA   (FP),Y
   LDY   #$04
   LDA   (DPTR),Y
   LDY   #$D1
   STA   (FP),Y
   LDY   #$05
   LDA   (DPTR),Y
   LDY   #$D2
   STA   (FP),Y
   LDY   #$06
   LDA   (DPTR),Y
   LDY   #$D3
   STA   (FP),Y
   LDY   #$07
   LDA   (DPTR),Y
   LDY   #$D4
   STA   (FP),Y
   LDY   #$CD
   LDA   (FP),Y
   CLC
   ADC   #$01
   LDY   #$D5
   STA   (FP),Y
   LDY   #$CE
   LDA   (FP),Y
   ADC   #$00
   LDY   #$D6
   STA   (FP),Y
   LDY   #$CF
   LDA   (FP),Y
   ADC   #$00
   LDY   #$D7
   STA   (FP),Y
   LDY   #$D0
   LDA   (FP),Y
   ADC   #$00
   LDY   #$D8
   STA   (FP),Y
   LDY   #$D1
   LDA   (FP),Y
   ADC   #$00
   LDY   #$D9
   STA   (FP),Y
   LDY   #$D2
   LDA   (FP),Y
   ADC   #$00
   LDY   #$DA
   STA   (FP),Y
   LDY   #$D3
   LDA   (FP),Y
   ADC   #$00
   LDY   #$DB
   STA   (FP),Y
   LDY   #$D4
   LDA   (FP),Y
   ADC   #$00
   LDY   #$DC
   STA   (FP),Y
   LDY   #$CB
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$D5
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$D6
   LDA   (FP),Y
   LDY   #$01
   STA   (DPTR),Y
   LDY   #$D7
   LDA   (FP),Y
   LDY   #$02
   STA   (DPTR),Y
   LDY   #$D8
   LDA   (FP),Y
   LDY   #$03
   STA   (DPTR),Y
   LDY   #$D9
   LDA   (FP),Y
   LDY   #$04
   STA   (DPTR),Y
   LDY   #$DA
   LDA   (FP),Y
   LDY   #$05
   STA   (DPTR),Y
   LDY   #$DB
   LDA   (FP),Y
   LDY   #$06
   STA   (DPTR),Y
   LDY   #$DC
   LDA   (FP),Y
   LDY   #$07
   STA   (DPTR),Y
   LDY   #$D5
   LDA   (FP),Y
   LDY   #$DD
   STA   (FP),Y
   LDY   #$D6
   LDA   (FP),Y
   LDY   #$DE
   STA   (FP),Y
   LDY   #$D7
   LDA   (FP),Y
   LDY   #$DF
   STA   (FP),Y
   LDY   #$D8
   LDA   (FP),Y
   LDY   #$E0
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$04
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDY   #$DD
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$DE
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$DF
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$E0
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   JSR   mix
   JSR   fin0
   JSR   fin1
   JSR   fin2
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
   DS.B  2

g2:
   DC.L  $0000000A
   DC.L  $00000000

g3:
   DC.B  $FF

ga5:
   DC.B  $FF, $FF, $80, $00

gs6:
   DS.B  1
   DC.L  $00000002
   DC.L  $00000000
   DC.B  $FF

gp7:
   DC.W  g3

gp8:
   DC.W  g2

@2.s12:
   DC.W  $F4C8

__zp_init:
   SUBROUTINE

   LDA   #$00
   STA   z4+1
   LDA   #$80
   STA   z4
   RTS
