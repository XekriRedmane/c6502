SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
z3	EQU	$26
z4	EQU	$27

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

   ; prologue: 0 arg bytes, 26 local bytes
   SEC
   LDA   SSP
   SBC   #$1C
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$1B
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
   LDA   z3
   LDY   #$0B
   STA   (FP),Y
   LDA   z3
   BMI   .sx_neg@4
   LDA   #$00
   JMP   .sx_done@5
.sx_neg@4:
   LDA   #$FF
.sx_done@5:
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
   LDA   z4
   LDY   #$0F
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
   LDA   z4+4
   INY
   STA   (FP),Y
   LDA   z4+5
   INY
   STA   (FP),Y
   LDA   z4+6
   INY
   STA   (FP),Y
   LDA   z4+7
   INY
   STA   (FP),Y
   LDY   #$0F
   LDA   (FP),Y
   LDY   #$17
   STA   (FP),Y
   LDY   #$10
   LDA   (FP),Y
   LDY   #$18
   STA   (FP),Y
   LDY   #$11
   LDA   (FP),Y
   LDY   #$19
   STA   (FP),Y
   LDY   #$12
   LDA   (FP),Y
   LDY   #$1A
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

   ; epilogue
   CLC
   LDA   FP
   ADC   #$1C
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$1B
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

   LDA   ga5
   LDY   #$01
   STA   (FP),Y
   LDA   ga5+1
   INY
   STA   (FP),Y
   LDA   ga5+2
   INY
   STA   (FP),Y
   LDA   ga5+3
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
   LDA   ga5+4
   LDY   #$09
   STA   (FP),Y
   LDA   ga5+5
   INY
   STA   (FP),Y
   LDA   ga5+6
   INY
   STA   (FP),Y
   LDA   ga5+7
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
   LDA   ga5+8
   LDY   #$11
   STA   (FP),Y
   LDA   ga5+9
   INY
   STA   (FP),Y
   LDA   ga5+10
   INY
   STA   (FP),Y
   LDA   ga5+11
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
   LDA   ga5+12
   LDY   #$19
   STA   (FP),Y
   LDA   ga5+13
   INY
   STA   (FP),Y
   LDA   ga5+14
   INY
   STA   (FP),Y
   LDA   ga5+15
   INY
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
   LDY   #$1F
   STA   (FP),Y
   LDY   #$1C
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
   LDA   #<gs6
   LDY   #$0B
   STA   (FP),Y
   LDA   #>gs6
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$03
   LDY   #$0D
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$0E
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$0F
   STA   (FP),Y
   INY
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@6
   LDA   #$00
   JMP   .sx_done@7
.sx_neg@6:
   LDA   #$FF
.sx_done@7:
   LDY   #$11
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

f7:
   SUBROUTINE

   ; prologue: 13 arg bytes, 108 local bytes
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

   LDA   #$CE
   LDY   #$01
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   z3
   INY
   STA   (FP),Y
   LDA   z3
   BMI   .sx_neg@8
   LDA   #$00
   JMP   .sx_done@9
.sx_neg@8:
   LDA   #$FF
.sx_done@9:
   LDY   #$04
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
   ADC   #$03
   LDY   #$07
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$08
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$09
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
   LDY   #$0B
   STA   (FP),Y
   LDY   #$03
   LDA   (FP),Y
   STA   HARGS
   INY
   LDA   (FP),Y
   STA   HARGS+1
   LDY   #$0A
   LDA   (FP),Y
   STA   HARGS+2
   INY
   LDA   (FP),Y
   STA   HARGS+3
   JSR   mul16
   LDA   HARGS+4
   LDY   #$0C
   STA   (FP),Y
   LDA   HARGS+5
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   ga5
   INY
   LDA   (FP),Y
   STA   ga5+1
   LDA   (FP),Y
   BMI   .sx_neg@12
   LDA   #$00
   JMP   .sx_done@13
.sx_neg@12:
   LDA   #$FF
.sx_done@13:
   STA   ga5+2
   STA   ga5+3
   LDA   z3
   LDY   #$0E
   STA   (FP),Y
   LDA   z3
   BMI   .sx_neg@14
   LDA   #$00
   JMP   .sx_done@15
.sx_neg@14:
   LDA   #$FF
.sx_done@15:
   LDY   #$0F
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ORA   #$03
   LDY   #$10
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$11
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$12
   STA   (FP),Y
   STA   z3
   LDA   #<gs6
   INY
   STA   (FP),Y
   LDA   #>gs6
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
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$17
   STA   (FP),Y
   SEC
   SBC   #$01
   INY
   STA   (FP),Y
   LDY   #$15
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$18
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$18
   LDA   (FP),Y
   BNE   .lb_skip@0
   JMP   .if_end@0
.lb_skip@0:
   LDA   g1
   BEQ   .cond_else@1
   LDY   #$19
   STA   (FP),Y
   LDA   g1
   BMI   .sx_neg@16
   LDA   #$00
   JMP   .sx_done@17
.sx_neg@16:
   LDA   #$FF
.sx_done@17:
   LDY   #$1A
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$1B
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$1C
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@18
   LDA   #$00
   JMP   .sx_done@19
.sx_neg@18:
   LDA   #$FF
.sx_done@19:
   LDY   #$1D
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$1B
   LDA   (FP),Y
   LDY   #$1F
   STA   (FP),Y
   LDY   #$1C
   LDA   (FP),Y
   LDY   #$20
   STA   (FP),Y
   LDY   #$1D
   LDA   (FP),Y
   LDY   #$21
   STA   (FP),Y
   INY
   STA   (FP),Y
   JMP   .cond_end@2
.cond_else@1:
   LDA   #$00
   LDY   #$23
   STA   (FP),Y
   LDA   #$80
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$23
   LDA   (FP),Y
   LDY   #$1F
   STA   (FP),Y
   LDY   #$24
   LDA   (FP),Y
   LDY   #$20
   STA   (FP),Y
   LDY   #$25
   LDA   (FP),Y
   LDY   #$21
   STA   (FP),Y
   INY
   STA   (FP),Y
.cond_end@2:
   LDA   @5.s13
   LDY   #$27
   STA   (FP),Y
   ASL
   STA   (FP),Y
   LDA   @5.s13+1
   INY
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
   STA   @5.s13
   INY
   LDA   (FP),Y
   STA   @5.s13+1
   DEY
   LDA   (FP),Y
   LDY   #$29
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$2A
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$1F
   LDA   (FP),Y
   CLC
   LDY   #$29
   ADC   (FP),Y
   LDY   #$2D
   STA   (FP),Y
   LDY   #$20
   LDA   (FP),Y
   LDY   #$2A
   ADC   (FP),Y
   LDY   #$2E
   STA   (FP),Y
   LDY   #$21
   LDA   (FP),Y
   LDY   #$2B
   ADC   (FP),Y
   LDY   #$2F
   STA   (FP),Y
   LDY   #$22
   LDA   (FP),Y
   LDY   #$2C
   ADC   (FP),Y
   LDY   #$30
   STA   (FP),Y
   LDY   #$2D
   LDA   (FP),Y
   LDY   #$31
   STA   (FP),Y
   LDY   #$2E
   LDA   (FP),Y
   LDY   #$32
   STA   (FP),Y
   LDA   #$07
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$31
   LDA   (FP),Y
   LDY   #$33
   AND   (FP),Y
   LDY   #$35
   STA   (FP),Y
   LDY   #$32
   LDA   (FP),Y
   LDY   #$34
   AND   (FP),Y
   LDY   #$36
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   BEQ   .cmp_true@20
   LDA   #$00
   JMP   .cmp_end@21
.cmp_true@20:
   LDA   #$01
.cmp_end@21:
   LDY   #$37
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .switch@0_break
   LDY   #$35
   LDA   (FP),Y
   CMP   #$02
   BEQ   .cmp_true@22
   LDA   #$00
   JMP   .cmp_end@23
.cmp_true@22:
   LDA   #$01
.cmp_end@23:
   LDY   #$39
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
.switch@0_break:
   LDY   #$02
   LDA   (FP),Y
   BEQ   .lnot_true@24
   LDA   #$00
   JMP   .lnot_end@25
.lnot_true@24:
   LDA   #$01
.lnot_end@25:
   LDY   #$3B
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$3D
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$3E
   STA   (FP),Y
   LDA   @5.s13
   INY
   STA   (FP),Y
   LDA   @5.s13+1
   INY
   STA   (FP),Y
   LDY   #$3D
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
   JSR   mul16
   LDA   HARGS+4
   LDY   #$41
   STA   (FP),Y
   LDA   HARGS+5
   INY
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
   LDA   #<gs6
   LDY   #$47
   STA   (FP),Y
   LDA   #>gs6
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$01
   LDY   #$49
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$4A
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$4B
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$4C
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   AND   #$81
   LDY   #$4D
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDA   #$00
   LDY   #$4E
   STA   (FP),Y
   LDY   #$49
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$4D
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$4E
   LDA   (FP),Y
   LDY   #$01
   STA   (DPTR),Y
   LDY   #$4D
   LDA   (FP),Y
   LDY   #$4F
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$50
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
   LDY   #$4F
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$50
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$51
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$52
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   JSR   mix
.if_end@0:
   LDA   g2
   LDY   #$53
   STA   (FP),Y
   LDA   g2+1
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   HARGS
   INY
   LDA   (FP),Y
   STA   HARGS+1
   LDA   #$05
   STA   HARGS+2
   LDA   #$00
   STA   HARGS+3
   JSR   mul16
   LDA   HARGS+4
   LDY   #$55
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
   LDY   #$57
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$58
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@26
   LDA   #$00
   JMP   .sx_done@27
.sx_neg@26:
   LDA   #$FF
.sx_done@27:
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
   LDY   #$57
   LDA   (FP),Y
   LDY   #$72
   EOR   (FP),Y
   LDY   #$5F
   STA   (FP),Y
   LDY   #$58
   LDA   (FP),Y
   LDY   #$73
   EOR   (FP),Y
   LDY   #$60
   STA   (FP),Y
   LDY   #$59
   LDA   (FP),Y
   LDY   #$74
   EOR   (FP),Y
   LDY   #$61
   STA   (FP),Y
   LDY   #$5A
   LDA   (FP),Y
   LDY   #$75
   EOR   (FP),Y
   LDY   #$62
   STA   (FP),Y
   LDY   #$5B
   LDA   (FP),Y
   LDY   #$76
   EOR   (FP),Y
   LDY   #$63
   STA   (FP),Y
   LDY   #$5C
   LDA   (FP),Y
   LDY   #$77
   EOR   (FP),Y
   LDY   #$64
   STA   (FP),Y
   LDY   #$5D
   LDA   (FP),Y
   LDY   #$78
   EOR   (FP),Y
   LDY   #$65
   STA   (FP),Y
   LDY   #$5E
   LDA   (FP),Y
   LDY   #$79
   EOR   (FP),Y
   LDY   #$66
   STA   (FP),Y
   LDY   #$5F
   LDA   (FP),Y
   LDY   #$67
   STA   (FP),Y
   LDY   #$60
   LDA   (FP),Y
   LDY   #$68
   STA   (FP),Y
   LDY   #$61
   LDA   (FP),Y
   LDY   #$69
   STA   (FP),Y
   LDY   #$62
   LDA   (FP),Y
   LDY   #$6A
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$04
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDY   #$67
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$68
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$69
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$6A
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   JSR   mix
   LDA   @5.s13
   LDY   #$6B
   STA   (FP),Y
   LDA   @5.s13+1
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
   ADC   #$7B
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$6D
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

f14:
   SUBROUTINE

   ; prologue: 0 arg bytes, 141 local bytes
   SEC
   LDA   SSP
   SBC   #$8F
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$8E
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
   LDA   #$01
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$0B
   STA   (FP),Y
   DEY
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
   LDA   g2
   INY
   STA   (FP),Y
   LDA   g2+1
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   SEC
   SBC   #$05
   LDY   #$17
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   SBC   #$00
   LDY   #$18
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   g2
   INY
   LDA   (FP),Y
   STA   g2+1
   DEY
   LDA   (FP),Y
   LDY   #$19
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$1A
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
   LDY   #$19
   LDA   (FP),Y
   LDY   #$1F
   EOR   (FP),Y
   LDY   #$21
   STA   (FP),Y
   LDY   #$1A
   LDA   (FP),Y
   LDY   #$20
   EOR   (FP),Y
   LDY   #$22
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$23
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$24
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
   LDY   #$23
   LDA   (FP),Y
   LDY   #$0D
   STA   (FP),Y
   LDY   #$24
   LDA   (FP),Y
   LDY   #$0E
   STA   (FP),Y
   LDY   #$25
   LDA   (FP),Y
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
   LDA   #$00
   LDY   #$2B
   STA   (FP),Y
   INY
   STA   (FP),Y
.loop@4_continue:
   LDY   #$2D
   STA   (FP),Y
   CLC
   ADC   #$01
   INY
   STA   (FP),Y
   LDY   #$2C
   STA   (FP),Y
   INY
   LDA   (FP),Y
   LDY   #$2F
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
   BVC   .cmp_novf@28
   EOR   #$80
.cmp_novf@28:
   ASL
   LDA   #$00
   ROL
   LDY   #$31
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@4
   JMP   .loop@4_break
.lb_skip@4:
   LDA   #$03
   LDY   #$33
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$35
   STA   (FP),Y
   STA   z3
   LDY   #$0D
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
   BNE   .or_true@3
   LDY   #$05
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   INY
   ORA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@3
   LDA   #$00
   LDY   #$36
   STA   (FP),Y
   INY
   STA   (FP),Y
   JMP   .or_end@4
.or_true@3:
   LDA   #$01
   LDY   #$36
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@4:
   LDY   #$36
   LDA   (FP),Y
   LDY   #$38
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$39
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@29
   LDA   #$00
   JMP   .sx_done@30
.sx_neg@29:
   LDA   #$FF
.sx_done@30:
   LDY   #$3A
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$38
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
   ADC   #$8F
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$8E
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   RTS
.loop@5_continue:
   LDY   #$3D
   LDA   (FP),Y
   INY
   STA   (FP),Y
   CLC
   ADC   #$01
   INY
   STA   (FP),Y
   LDY   #$3D
   STA   (FP),Y
   INY
   LDA   (FP),Y
   LDY   #$40
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
   BVC   .cmp_novf@31
   EOR   #$80
.cmp_novf@31:
   ASL
   LDA   #$00
   ROL
   LDY   #$42
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@3
   JMP   .loop@4_continue
.lb_skip@3:
   LDA   #$FF
   LDY   #$44
   STA   (FP),Y
   LDA   #$7F
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@5
   LDY   #$2C
   LDA   (FP),Y
   BEQ   .and_false@5
   LDA   #$01
   LDY   #$46
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   JMP   .and_end@6
.and_false@5:
   LDA   #$00
   LDY   #$46
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@6:
   LDY   #$46
   LDA   (FP),Y
   LDY   #$48
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$49
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$4A
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$4B
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$4A
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
   ADC   #$8F
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$8E
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   RTS
.shift_loop@32:
   LDY   #$51
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
   BNE   .shift_loop@32
   LDA   #$07
   LDY   #$52
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
   CMP   #$07
   BEQ   .cmp_true@34
   LDA   #$00
   JMP   .cmp_end@35
.cmp_true@34:
   LDA   #$01
.cmp_end@35:
   LDY   #$56
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   LDA   #<gs6
   INY
   STA   (FP),Y
   LDA   #>gs6
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$03
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
   BMI   .sx_neg@36
   LDA   #$00
   JMP   .sx_done@37
.sx_neg@36:
   LDA   #$FF
.sx_done@37:
   LDY   #$5E
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
   LDY   #$5D
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$5E
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$5F
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$60
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   JSR   mix
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
   LDA   #$03
   STA   HARGS+4
   JSR   lsr32
   LDA   HARGS+5
   LDY   #$61
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
   LDY   #$61
   LDA   (FP),Y
   LDY   #$05
   STA   (FP),Y
   LDY   #$62
   LDA   (FP),Y
   LDY   #$06
   STA   (FP),Y
   LDY   #$63
   LDA   (FP),Y
   LDY   #$07
   STA   (FP),Y
   LDY   #$64
   LDA   (FP),Y
   LDY   #$08
   STA   (FP),Y
   LDA   #<gs6
   LDY   #$65
   STA   (FP),Y
   LDA   #>gs6
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$01
   LDY   #$67
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$68
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$69
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$6A
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$6B
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$6C
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$61
   LDA   (FP),Y
   LDY   #$6B
   ORA   (FP),Y
   LDY   #$6F
   STA   (FP),Y
   LDY   #$62
   LDA   (FP),Y
   LDY   #$6C
   ORA   (FP),Y
   LDY   #$70
   STA   (FP),Y
   LDY   #$63
   LDA   (FP),Y
   LDY   #$6D
   ORA   (FP),Y
   LDY   #$71
   STA   (FP),Y
   LDY   #$64
   LDA   (FP),Y
   LDY   #$6E
   ORA   (FP),Y
   LDY   #$72
   STA   (FP),Y
   LDY   #$6F
   LDA   (FP),Y
   LDY   #$73
   STA   (FP),Y
   STA   g1
   JMP   .loop@5_continue
.loop@4_break:
   LDA   #$00
   LDY   #$74
   STA   (FP),Y
   INY
   STA   (FP),Y
.loop@9_continue:
   LDY   #$75
   LDA   (FP),Y
   INY
   STA   (FP),Y
   CLC
   ADC   #$01
   INY
   STA   (FP),Y
   LDY   #$75
   STA   (FP),Y
   INY
   LDA   (FP),Y
   LDY   #$78
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
   LDY   #$7A
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@2
   JMP   .loop@9_break
.lb_skip@2:
   LDA   #<gs6
   LDY   #$7C
   STA   (FP),Y
   LDA   #>gs6
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$01
   LDY   #$7E
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$7F
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$80
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$81
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
   LDY   #$7E
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$82
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$83
   LDA   (FP),Y
   LDY   #$01
   STA   (DPTR),Y
   LDY   #$82
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@1
   JMP   .loop@9_continue
.lb_skip@1:
   LDA   #<gs6
   LDY   #$84
   STA   (FP),Y
   LDA   #>gs6
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$01
   LDY   #$86
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$87
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$88
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$89
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$8A
   STA   (FP),Y
   ASL
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$8B
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
   DEY
   LDA   (FP),Y
   LDY   #$8C
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$8D
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$0B
   STA   (FP),Y
   LDY   #$8D
   LDA   (FP),Y
   LDY   #$0C
   STA   (FP),Y
   JMP   .loop@9_continue
.loop@9_break:
   LDY   #$05
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
   ADC   #$8F
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$8E
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   RTS

f22:
   SUBROUTINE

   ; prologue: 3 arg bytes, 81 local bytes
   SEC
   LDA   SSP
   SBC   #$53
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$52
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
   LDA   #$06
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
   LDY   #$03
   LDA   (FP),Y
   LDY   #$0D
   STA   (FP),Y
   LDY   #$04
   LDA   (FP),Y
   LDY   #$0E
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
   LDY   #$03
   STA   (FP),Y
   LDY   #$10
   LDA   (FP),Y
   LDY   #$04
   STA   (FP),Y
   LDA   z4
   LDY   #$11
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
   LDA   z4+4
   INY
   STA   (FP),Y
   LDA   z4+5
   INY
   STA   (FP),Y
   LDA   z4+6
   INY
   STA   (FP),Y
   LDA   z4+7
   INY
   STA   (FP),Y
   LDA   #$07
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$1B
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$1C
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@39
   LDA   #$00
   JMP   .sx_done@40
.sx_neg@39:
   LDA   #$FF
.sx_done@40:
   LDY   #$1D
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
   LDY   #$11
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
   LDY   #$1B
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
   JSR   sdivmod64
   LDA   HARGS+24
   LDY   #$23
   STA   (FP),Y
   LDA   HARGS+25
   INY
   STA   (FP),Y
   LDA   HARGS+26
   INY
   STA   (FP),Y
   LDA   HARGS+27
   INY
   STA   (FP),Y
   LDA   HARGS+28
   INY
   STA   (FP),Y
   LDA   HARGS+29
   INY
   STA   (FP),Y
   LDA   HARGS+30
   INY
   STA   (FP),Y
   LDA   HARGS+31
   INY
   STA   (FP),Y
   LSR
   DEY
   LDA   (FP),Y
   ROR
   LDY   #$32
   STA   (FP),Y
   LDY   #$28
   LDA   (FP),Y
   ROR
   LDY   #$31
   STA   (FP),Y
   LDY   #$27
   LDA   (FP),Y
   ROR
   LDY   #$30
   STA   (FP),Y
   LDY   #$26
   LDA   (FP),Y
   ROR
   LDY   #$2F
   STA   (FP),Y
   LDY   #$25
   LDA   (FP),Y
   ROR
   LDY   #$2E
   STA   (FP),Y
   LDY   #$24
   LDA   (FP),Y
   ROR
   LDY   #$2D
   STA   (FP),Y
   LDY   #$23
   LDA   (FP),Y
   ROR
   LDY   #$2C
   STA   (FP),Y
   LDA   #$00
   ROR
   DEY
   STA   (FP),Y
   LDY   #$33
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
   ADC   #$03
   LDY   #$36
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$37
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$33
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDA   #<gs6
   LDY   #$38
   STA   (FP),Y
   LDA   #>gs6
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$03
   LDY   #$3A
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$3B
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$3C
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$05
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
   BEQ   .lnot_true@41
   LDA   #$00
   JMP   .lnot_end@42
.lnot_true@41:
   LDA   #$01
.lnot_end@42:
   LDY   #$3E
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDA   z3
   INY
   STA   (FP),Y
   LDA   z3
   BMI   .sx_neg@43
   LDA   #$00
   JMP   .sx_done@44
.sx_neg@43:
   LDA   #$FF
.sx_done@44:
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
   SBC   #$0D
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDY   #$3D
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$3E
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$3F
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$40
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   LDY   #$41
   LDA   (FP),Y
   LDY   #$05
   STA   (SSP),Y
   LDY   #$42
   LDA   (FP),Y
   LDY   #$06
   STA   (SSP),Y
   LDY   #$43
   LDA   (FP),Y
   LDY   #$07
   STA   (SSP),Y
   LDY   #$44
   LDA   (FP),Y
   LDY   #$08
   STA   (SSP),Y
   LDY   #$45
   LDA   (FP),Y
   LDY   #$09
   STA   (SSP),Y
   LDY   #$46
   LDA   (FP),Y
   LDY   #$0A
   STA   (SSP),Y
   LDY   #$47
   LDA   (FP),Y
   LDY   #$0B
   STA   (SSP),Y
   LDY   #$48
   LDA   (FP),Y
   LDY   #$0C
   STA   (SSP),Y
   LDY   #$49
   LDA   (FP),Y
   LDY   #$0D
   STA   (SSP),Y
   JSR   f7
   LDY   #$4A
   STA   (FP),Y
   TXA
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
   STA   HARGS+8
   LDY   #$4F
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

   ; prologue: 0 arg bytes, 166 local bytes
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
   LDA   #$FF
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
   LDA   z4
   INY
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
   LDA   z4+4
   INY
   STA   (FP),Y
   LDA   z4+5
   INY
   STA   (FP),Y
   LDA   z4+6
   INY
   STA   (FP),Y
   LDA   z4+7
   INY
   STA   (FP),Y
   LDY   #$0F
   LDA   (FP),Y
   LDY   #$17
   STA   (FP),Y
   LDY   #$10
   LDA   (FP),Y
   LDY   #$18
   STA   (FP),Y
   LDY   #$11
   LDA   (FP),Y
   LDY   #$19
   STA   (FP),Y
   LDY   #$12
   LDA   (FP),Y
   LDY   #$1A
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
   LDY   #$05
   LDA   (FP),Y
   SEC
   SBC   #$01
   LDY   #$1B
   STA   (FP),Y
   LDY   #$06
   LDA   (FP),Y
   SBC   #$00
   LDY   #$1C
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$05
   STA   (FP),Y
   LDY   #$1C
   LDA   (FP),Y
   LDY   #$06
   STA   (FP),Y
   LDY   #$1B
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@8
   JSR   f14
   LDA   HARGS+8
   LDY   #$1D
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
   LDY   #$1D
   LDA   (FP),Y
   LDY   #$21
   STA   (FP),Y
   LDY   #$02
   STA   (FP),Y
.if_end@8:
   LDA   #$00
   LDY   #$22
   STA   (FP),Y
   INY
   STA   (FP),Y
.loop@10_start:
   LDY   #$24
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
   BVC   .cmp_novf@45
   EOR   #$80
.cmp_novf@45:
   ASL
   LDA   #$00
   ROL
   LDY   #$26
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@7
   JMP   .loop@10_break
.lb_skip@7:
   LDA   z3
   LDY   #$28
   STA   (FP),Y
   LDA   z3
   BMI   .sx_neg@46
   LDA   #$00
   JMP   .sx_done@47
.sx_neg@46:
   LDA   #$FF
.sx_done@47:
   LDY   #$29
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   AND   #$81
   LDY   #$2A
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDA   #$00
   LDY   #$2B
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$2C
   STA   (FP),Y
   STA   z3
   LDA   (FP),Y
   INY
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@48
   LDA   #$00
   JMP   .sx_done@49
.sx_neg@48:
   LDA   #$FF
.sx_done@49:
   LDY   #$2E
   STA   (FP),Y
   LDA   g2
   INY
   STA   (FP),Y
   LDA   g2+1
   INY
   STA   (FP),Y
   LDY   #$2D
   LDA   (FP),Y
   LDY   #$2F
   ORA   (FP),Y
   LDY   #$31
   STA   (FP),Y
   LDY   #$2E
   LDA   (FP),Y
   LDY   #$30
   ORA   (FP),Y
   LDY   #$32
   STA   (FP),Y
   LDA   #$07
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$31
   LDA   (FP),Y
   LDY   #$33
   AND   (FP),Y
   LDY   #$35
   STA   (FP),Y
   LDY   #$32
   LDA   (FP),Y
   LDY   #$34
   AND   (FP),Y
   LDY   #$36
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   BEQ   .cmp_true@50
   LDA   #$00
   JMP   .cmp_end@51
.cmp_true@50:
   LDA   #$01
.cmp_end@51:
   LDY   #$37
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .case@12
   LDY   #$35
   LDA   (FP),Y
   CMP   #$03
   BEQ   .cmp_true@52
   LDA   #$00
   JMP   .cmp_end@53
.cmp_true@52:
   LDA   #$01
.cmp_end@53:
   LDY   #$39
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@6
   JMP   .case@13
.lb_skip@6:
   LDY   #$35
   LDA   (FP),Y
   CMP   #$04
   BEQ   .cmp_true@54
   LDA   #$00
   JMP   .cmp_end@55
.cmp_true@54:
   LDA   #$01
.cmp_end@55:
   LDY   #$3B
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .case@14
   LDY   #$35
   LDA   (FP),Y
   CMP   #$07
   BEQ   .cmp_true@56
   LDA   #$00
   JMP   .cmp_end@57
.cmp_true@56:
   LDA   #$01
.cmp_end@57:
   LDY   #$3D
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@5
   JMP   .case@15
.lb_skip@5:
   JMP   .switch@11_break
.case@12:
   JSR   f14
   LDA   HARGS+8
   LDY   #$3F
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
   LDY   #$3F
   LDA   (FP),Y
   LDY   #$43
   STA   (FP),Y
   LDY   #$40
   LDA   (FP),Y
   LDY   #$44
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$05
   STA   (FP),Y
   LDY   #$44
   LDA   (FP),Y
   LDY   #$06
   STA   (FP),Y
.case@13:
.case@14:
   LDY   #$23
   LDA   (FP),Y
   LDY   #$45
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$23
   LDA   (FP),Y
   LDY   #$47
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$45
   LDA   (FP),Y
   SEC
   LDY   #$47
   SBC   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$48
   SBC   (FP),Y
   BVC   .cmp_novf@58
   EOR   #$80
.cmp_novf@58:
   ASL
   LDA   #$00
   ROL
   LDY   #$49
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$4B
   STA   (FP),Y
   LDA   z3
   INY
   STA   (FP),Y
   LDA   z3
   BMI   .sx_neg@59
   LDA   #$00
   JMP   .sx_done@60
.sx_neg@59:
   LDA   #$FF
.sx_done@60:
   LDY   #$4D
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
   JSR   f22
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
.case@15:
   LDA   #$80
   LDY   #$52
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
   LDY   #$54
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$05
   STA   (FP),Y
   LDY   #$54
   LDA   (FP),Y
   LDY   #$06
   STA   (FP),Y
.switch@11_break:
   LDY   #$23
   LDA   (FP),Y
   LDY   #$55
   STA   (FP),Y
   CLC
   ADC   #$01
   INY
   STA   (FP),Y
   LDY   #$23
   STA   (FP),Y
   JMP   .loop@10_start
.loop@10_break:
   LDA   g2
   LDY   #$57
   STA   (FP),Y
   LDA   g2+1
   INY
   STA   (FP),Y
   LDA   #$06
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$57
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
   LDY   #$5B
   STA   (FP),Y
   LDA   HARGS+7
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$5D
   STA   (FP),Y
   LDA   #$01
   INY
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
   LDY   #$5D
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$5E
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$5F
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   JSR   f22
   LDA   HARGS+8
   LDY   #$60
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
   LDA   #$80
   INY
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
   LDY   #$66
   LDA   (FP),Y
   LDY   #$07
   ORA   (FP),Y
   LDY   #$6E
   STA   (FP),Y
   LDY   #$67
   LDA   (FP),Y
   LDY   #$08
   ORA   (FP),Y
   LDY   #$6F
   STA   (FP),Y
   LDY   #$68
   LDA   (FP),Y
   LDY   #$09
   ORA   (FP),Y
   LDY   #$70
   STA   (FP),Y
   LDY   #$69
   LDA   (FP),Y
   LDY   #$0A
   ORA   (FP),Y
   LDY   #$71
   STA   (FP),Y
   LDY   #$6A
   LDA   (FP),Y
   LDY   #$0B
   ORA   (FP),Y
   LDY   #$72
   STA   (FP),Y
   LDY   #$6B
   LDA   (FP),Y
   LDY   #$0C
   ORA   (FP),Y
   LDY   #$73
   STA   (FP),Y
   LDY   #$6C
   LDA   (FP),Y
   LDY   #$0D
   ORA   (FP),Y
   LDY   #$74
   STA   (FP),Y
   LDY   #$6D
   LDA   (FP),Y
   LDY   #$0E
   ORA   (FP),Y
   LDY   #$75
   STA   (FP),Y
   LDY   #$6E
   LDA   (FP),Y
   LDY   #$76
   STA   (FP),Y
   LDY   #$6F
   LDA   (FP),Y
   LDY   #$77
   STA   (FP),Y
   LDA   #$07
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$76
   LDA   (FP),Y
   LDY   #$78
   AND   (FP),Y
   LDY   #$7A
   STA   (FP),Y
   LDY   #$77
   LDA   (FP),Y
   LDY   #$79
   AND   (FP),Y
   LDY   #$7B
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   BEQ   .cmp_true@63
   LDA   #$00
   JMP   .cmp_end@64
.cmp_true@63:
   LDA   #$01
.cmp_end@64:
   LDY   #$7C
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .case@17
   LDY   #$7A
   LDA   (FP),Y
   CMP   #$05
   BEQ   .cmp_true@65
   LDA   #$00
   JMP   .cmp_end@66
.cmp_true@65:
   LDA   #$01
.cmp_end@66:
   LDY   #$7E
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .case@18
   JMP   .default@19
.case@17:
   LDY   #$02
   LDA   (FP),Y
   SEC
   SBC   #$01
   LDY   #$80
   STA   (FP),Y
   LDY   #$02
   STA   (FP),Y
.case@18:
   LDA   g2
   LDY   #$81
   STA   (FP),Y
   LDA   g2+1
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$83
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$84
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@67
   LDA   #$00
   JMP   .sx_done@68
.sx_neg@67:
   LDA   #$FF
.sx_done@68:
   LDY   #$85
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #<ga5
   INY
   STA   (FP),Y
   LDA   #>ga5
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
   LDA   #$04
   STA   HARGS+1
   JSR   mul8
   LDA   HARGS+2
   LDY   #$8C
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$87
   LDA   (FP),Y
   CLC
   LDY   #$8D
   ADC   (FP),Y
   LDY   #$8F
   STA   (FP),Y
   LDY   #$88
   LDA   (FP),Y
   LDY   #$8E
   ADC   (FP),Y
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
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$92
   STA   (FP),Y
   LDY   #$02
   LDA   (DPTR),Y
   LDY   #$93
   STA   (FP),Y
   LDY   #$03
   LDA   (DPTR),Y
   LDY   #$94
   STA   (FP),Y
   LDA   #$01
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   ORA   #$00
   BPL   .sx_done@70
   LDA   #$FF
.sx_done@70:
   LDY   #$97
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$91
   LDA   (FP),Y
   LDY   #$95
   AND   (FP),Y
   LDY   #$99
   STA   (FP),Y
   LDY   #$92
   LDA   (FP),Y
   LDY   #$96
   AND   (FP),Y
   LDY   #$9A
   STA   (FP),Y
   LDY   #$93
   LDA   (FP),Y
   LDY   #$97
   AND   (FP),Y
   LDY   #$9B
   STA   (FP),Y
   LDY   #$94
   LDA   (FP),Y
   LDY   #$98
   AND   (FP),Y
   LDY   #$9C
   STA   (FP),Y
   LDY   #$8F
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$99
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$9A
   LDA   (FP),Y
   LDY   #$01
   STA   (DPTR),Y
   LDY   #$9B
   LDA   (FP),Y
   LDY   #$02
   STA   (DPTR),Y
   LDY   #$9C
   LDA   (FP),Y
   LDY   #$03
   STA   (DPTR),Y
   LDY   #$83
   LDA   (FP),Y
   SEC
   LDY   #$99
   SBC   (FP),Y
   LDY   #$9D
   STA   (FP),Y
   LDY   #$84
   LDA   (FP),Y
   LDY   #$9A
   SBC   (FP),Y
   LDY   #$9E
   STA   (FP),Y
   LDY   #$85
   LDA   (FP),Y
   LDY   #$9B
   SBC   (FP),Y
   LDY   #$9F
   STA   (FP),Y
   LDY   #$86
   LDA   (FP),Y
   LDY   #$9C
   SBC   (FP),Y
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
   DEY
   LDA   (FP),Y
   LDY   #$05
   STA   (FP),Y
   LDY   #$A2
   LDA   (FP),Y
   LDY   #$06
   STA   (FP),Y
.default@19:
   JSR   fin0
   JSR   fin1
   JSR   fin2
   LDA   acc
   LDY   #$A3
   STA   (FP),Y
   LDA   acc+1
   AND   #$7F
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$A5
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$A6
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
   ADC   #$A8
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
   TAX
   PLA
   RTS

acc:
   DS.B  2

g1:
   DC.B  $FF

g2:
   DC.W  $0080

ga5:
   DC.L  $00010000
   DC.L  $000000FF
   DC.L  $0000FFFF
   DC.L  $0000000D

gs6:
   DS.B  1
   DC.W  $0100
   DC.B  $03

@5.s13:
   DC.W  $8000

@7.s18:
   DC.W  $007F

@16.s27:
   DS.B  1

__zp_init:
   SUBROUTINE

   LDA   #$00
   STA   z4+2
   STA   z4+3
   STA   z4+4
   STA   z4+5
   STA   z4+6
   STA   z4+7
   LDA   #$FF
   STA   z3
   STA   z4
   LDA   #$7F
   STA   z4+1
   RTS
