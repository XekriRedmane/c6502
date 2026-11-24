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
   LDA   z4
   LDY   #$0D
   STA   (FP),Y
   LDA   z4
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

   LDA   ga5
   LDY   #$01
   STA   (FP),Y
   LDA   ga5+1
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
   LDA   ga5+2
   LDY   #$07
   STA   (FP),Y
   LDA   ga5+3
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
   LDA   ga5+4
   LDY   #$0D
   STA   (FP),Y
   LDA   ga5+5
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$0F
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$10
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
   LDA   ga5+6
   LDY   #$13
   STA   (FP),Y
   LDA   ga5+7
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$15
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$16
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
   LDA   #<gs6
   LDY   #$09
   STA   (FP),Y
   LDA   #>gs6
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$05
   LDY   #$0B
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$0C
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$0D
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$0E
   STA   (FP),Y
   LDY   #$02
   LDA   (DPTR),Y
   LDY   #$0F
   STA   (FP),Y
   LDY   #$03
   LDA   (DPTR),Y
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

f8:
   SUBROUTINE

   ; prologue: 1 arg bytes, 161 local bytes
   SEC
   LDA   SSP
   SBC   #$A3
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$A2
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   LDA   #$68
   LDY   #$01
   STA   (FP),Y
   LDA   #$28
   INY
   STA   (FP),Y
   LDA   #$FE
   INY
   STA   (FP),Y
   LDA   #$DE
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
   DEY
   LDA   (FP),Y
   LDY   #$0C
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$0D
   STA   (FP),Y
   LDA   #$07
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$0C
   LDA   (FP),Y
   LDY   #$0E
   AND   (FP),Y
   LDY   #$10
   STA   (FP),Y
   LDY   #$0D
   LDA   (FP),Y
   LDY   #$0F
   AND   (FP),Y
   LDY   #$11
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CMP   #$02
   BEQ   .cmp_true@2
   LDA   #$00
   JMP   .cmp_end@3
.cmp_true@2:
   LDA   #$01
.cmp_end@3:
   LDY   #$12
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .case@1
   LDY   #$10
   LDA   (FP),Y
   CMP   #$03
   BEQ   .cmp_true@4
   LDA   #$00
   JMP   .cmp_end@5
.cmp_true@4:
   LDA   #$01
.cmp_end@5:
   LDY   #$14
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@2
   JMP   .case@3
.lb_skip@2:
   LDY   #$10
   LDA   (FP),Y
   CMP   #$06
   BEQ   .cmp_true@6
   LDA   #$00
   JMP   .cmp_end@7
.cmp_true@6:
   LDA   #$01
.cmp_end@7:
   LDY   #$16
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
   JMP   .default@5
.case@1:
   LDA   #$00
   LDY   #$18
   STA   (FP),Y
   INY
   STA   (FP),Y
.loop@2_start:
   LDA   g3
   CLC
   ADC   #$01
   LDY   #$1A
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
   DEY
   LDA   (FP),Y
   LDY   #$1C
   STA   (FP),Y
   LDY   #$A4
   STA   (FP),Y
   LDA   g2
   EOR   #$FF
   LDY   #$1D
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
   LDY   #$1D
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
   LDY   #$19
   LDA   (FP),Y
   CLC
   ADC   #$01
   LDY   #$22
   STA   (FP),Y
   LDY   #$19
   STA   (FP),Y
   LDY   #$23
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
   LDY   #$25
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@0
   JMP   .loop@2_start
.lb_skip@0:
.case@3:
   LDA   z4
   LDY   #$27
   STA   (FP),Y
   LDA   z4
   BMI   .sx_neg@9
   LDA   #$00
   JMP   .sx_done@10
.sx_neg@9:
   LDA   #$FF
.sx_done@10:
   LDY   #$28
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
   LDY   #$29
   STA   (FP),Y
   LDA   HARGS+5
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$2B
   STA   (FP),Y
   STA   z4
   LDA   (FP),Y
   INY
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@11
   LDA   #$00
   JMP   .sx_done@12
.sx_neg@11:
   LDA   #$FF
.sx_done@12:
   LDY   #$2D
   STA   (FP),Y
   LDA   #$02
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$2C
   LDA   (FP),Y
   LDY   #$2F
   EOR   (FP),Y
   LDY   #$31
   STA   (FP),Y
   LDY   #$2D
   LDA   (FP),Y
   LDY   #$30
   EOR   (FP),Y
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
   BMI   .sx_neg@13
   LDA   #$00
   JMP   .sx_done@14
.sx_neg@13:
   LDA   #$FF
.sx_done@14:
   LDY   #$35
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
   LDY   #$33
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$34
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$35
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$36
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   JSR   mix
   JMP   .switch@0_break
.case@4:
.default@5:
   LDA   #<gs6
   LDY   #$37
   STA   (FP),Y
   LDA   #>gs6
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$05
   LDY   #$39
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$3A
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$3B
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$3C
   STA   (FP),Y
   LDY   #$02
   LDA   (DPTR),Y
   LDY   #$3D
   STA   (FP),Y
   LDY   #$03
   LDA   (DPTR),Y
   LDY   #$3E
   STA   (FP),Y
   LDA   #$5A
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   ORA   #$00
   BPL   .sx_done@16
   LDA   #$FF
.sx_done@16:
   LDY   #$41
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$3B
   LDA   (FP),Y
   LDY   #$3F
   EOR   (FP),Y
   LDY   #$43
   STA   (FP),Y
   LDY   #$3C
   LDA   (FP),Y
   LDY   #$40
   EOR   (FP),Y
   LDY   #$44
   STA   (FP),Y
   LDY   #$3D
   LDA   (FP),Y
   LDY   #$41
   EOR   (FP),Y
   LDY   #$45
   STA   (FP),Y
   LDY   #$3E
   LDA   (FP),Y
   LDY   #$42
   EOR   (FP),Y
   LDY   #$46
   STA   (FP),Y
   LDY   #$39
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$43
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$44
   LDA   (FP),Y
   LDY   #$01
   STA   (DPTR),Y
   LDY   #$45
   LDA   (FP),Y
   LDY   #$02
   STA   (DPTR),Y
   LDY   #$46
   LDA   (FP),Y
   LDY   #$03
   STA   (DPTR),Y
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
   LDY   #$02
   LDA   (DPTR),Y
   LDY   #$4D
   STA   (FP),Y
   LDY   #$03
   LDA   (DPTR),Y
   LDY   #$4E
   STA   (FP),Y
   LDA   #$5A
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   ORA   #$00
   BPL   .sx_done@18
   LDA   #$FF
.sx_done@18:
   LDY   #$51
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$4B
   LDA   (FP),Y
   LDY   #$4F
   AND   (FP),Y
   LDY   #$53
   STA   (FP),Y
   LDY   #$4C
   LDA   (FP),Y
   LDY   #$50
   AND   (FP),Y
   LDY   #$54
   STA   (FP),Y
   LDY   #$4D
   LDA   (FP),Y
   LDY   #$51
   AND   (FP),Y
   LDY   #$55
   STA   (FP),Y
   LDY   #$4E
   LDA   (FP),Y
   LDY   #$52
   AND   (FP),Y
   LDY   #$56
   STA   (FP),Y
   LDY   #$49
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$53
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$54
   LDA   (FP),Y
   LDY   #$01
   STA   (DPTR),Y
   LDY   #$55
   LDA   (FP),Y
   LDY   #$02
   STA   (DPTR),Y
   LDY   #$56
   LDA   (FP),Y
   LDY   #$03
   STA   (DPTR),Y
.switch@0_break:
   LDA   z4
   LDY   #$57
   STA   (FP),Y
   LDA   z4
   BMI   .sx_neg@19
   LDA   #$00
   JMP   .sx_done@20
.sx_neg@19:
   LDA   #$FF
.sx_done@20:
   LDY   #$58
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$59
   STA   (FP),Y
   ASL
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$5A
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
   LDY   #$5B
   STA   (FP),Y
   STA   z4
   LDA   (FP),Y
   INY
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@21
   LDA   #$00
   JMP   .sx_done@22
.sx_neg@21:
   LDA   #$FF
.sx_done@22:
   LDY   #$5D
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$5E
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$5F
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@23
   LDA   #$00
   JMP   .sx_done@24
.sx_neg@23:
   LDA   #$FF
.sx_done@24:
   LDY   #$60
   STA   (FP),Y
   INY
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
   LDY   #$64
   STA   (FP),Y
   AND   #$0F
   INY
   STA   (FP),Y
   LDY   #$06
   LDA   (FP),Y
   LDY   #$62
   EOR   (FP),Y
   LDY   #$66
   STA   (FP),Y
   LDY   #$07
   LDA   (FP),Y
   LDY   #$63
   EOR   (FP),Y
   LDY   #$67
   STA   (FP),Y
   LDY   #$08
   LDA   (FP),Y
   LDY   #$64
   EOR   (FP),Y
   LDY   #$68
   STA   (FP),Y
   LDY   #$09
   LDA   (FP),Y
   LDY   #$65
   EOR   (FP),Y
   LDY   #$69
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$04
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
   LDY   #$69
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   JSR   mix
   LDA   #<gs6
   LDY   #$6A
   STA   (FP),Y
   LDA   #>gs6
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$01
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
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$6F
   STA   (FP),Y
   LDY   #$02
   LDA   (DPTR),Y
   LDY   #$70
   STA   (FP),Y
   LDY   #$03
   LDA   (DPTR),Y
   LDY   #$71
   STA   (FP),Y
   LDY   #$6E
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
   LDY   #$72
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
   LDY   #$6C
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
   LDA   #$FF
   LDY   #$76
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$79
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$7A
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@25
   LDA   #$00
   JMP   .sx_done@26
.sx_neg@25:
   LDA   #$FF
.sx_done@26:
   LDY   #$7B
   STA   (FP),Y
   INY
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
   LDY   #$7F
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$80
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$81
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$82
   STA   (FP),Y
   LDY   #$02
   LDA   (DPTR),Y
   LDY   #$83
   STA   (FP),Y
   LDY   #$03
   LDA   (DPTR),Y
   LDY   #$84
   STA   (FP),Y
   LDY   #$81
   LDA   (FP),Y
   LDY   #$85
   STA   (FP),Y
   ASL
   STA   (FP),Y
   LDY   #$82
   LDA   (FP),Y
   LDY   #$86
   STA   (FP),Y
   ROL
   STA   (FP),Y
   LDY   #$83
   LDA   (FP),Y
   LDY   #$87
   STA   (FP),Y
   ROL
   STA   (FP),Y
   LDY   #$84
   LDA   (FP),Y
   LDY   #$88
   STA   (FP),Y
   ROL
   STA   (FP),Y
   LDY   #$7F
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$85
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$86
   LDA   (FP),Y
   LDY   #$01
   STA   (DPTR),Y
   LDY   #$87
   LDA   (FP),Y
   LDY   #$02
   STA   (DPTR),Y
   LDY   #$88
   LDA   (FP),Y
   LDY   #$03
   STA   (DPTR),Y
   LDY   #$79
   LDA   (FP),Y
   LDY   #$85
   ORA   (FP),Y
   LDY   #$89
   STA   (FP),Y
   LDY   #$7A
   LDA   (FP),Y
   LDY   #$86
   ORA   (FP),Y
   LDY   #$8A
   STA   (FP),Y
   LDY   #$7B
   LDA   (FP),Y
   LDY   #$87
   ORA   (FP),Y
   LDY   #$8B
   STA   (FP),Y
   LDY   #$7C
   LDA   (FP),Y
   LDY   #$88
   ORA   (FP),Y
   LDY   #$8C
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$04
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDY   #$89
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$8A
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$8B
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$8C
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   JSR   mix
   LDY   #$A4
   LDA   (FP),Y
   LDY   #$8D
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@27
   LDA   #$00
   JMP   .sx_done@28
.sx_neg@27:
   LDA   #$FF
.sx_done@28:
   LDY   #$8E
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   AND   #$81
   LDY   #$8F
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDA   #$00
   LDY   #$90
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$91
   STA   (FP),Y
   LDY   #$A4
   STA   (FP),Y
   LDA   g1
   CLC
   ADC   #$81
   LDY   #$92
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
   LDY   #$06
   LDA   (FP),Y
   SEC
   LDY   #$94
   SBC   (FP),Y
   LDY   #$98
   STA   (FP),Y
   LDY   #$07
   LDA   (FP),Y
   LDY   #$95
   SBC   (FP),Y
   LDY   #$99
   STA   (FP),Y
   LDY   #$08
   LDA   (FP),Y
   LDY   #$96
   SBC   (FP),Y
   LDY   #$9A
   STA   (FP),Y
   LDY   #$09
   LDA   (FP),Y
   LDY   #$97
   SBC   (FP),Y
   LDY   #$9B
   STA   (FP),Y
   LDY   #$98
   LDA   (FP),Y
   LDY   #$9C
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
   AND   #$0F
   LDY   #$9F
   STA   (FP),Y
   LDY   #$9C
   LDA   (FP),Y
   LDY   #$A0
   STA   (FP),Y
   LDY   #$9D
   LDA   (FP),Y
   LDY   #$A1
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
   ADC   #$A4
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$A2
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

f13:
   SUBROUTINE

   ; prologue: 0 arg bytes, 204 local bytes
   SEC
   LDA   SSP
   SBC   #$CE
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$CD
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
   LDA   z4
   INY
   STA   (FP),Y
   LDA   z4
   BMI   .sx_neg@29
   LDA   #$00
   JMP   .sx_done@30
.sx_neg@29:
   LDA   #$FF
.sx_done@30:
   LDY   #$0A
   STA   (FP),Y
   LDA   z4
   INY
   STA   (FP),Y
   LDA   z4
   BMI   .sx_neg@31
   LDA   #$00
   JMP   .sx_done@32
.sx_neg@31:
   LDA   #$FF
.sx_done@32:
   LDY   #$0C
   STA   (FP),Y
   LDY   #$09
   LDA   (FP),Y
   LDY   #$0B
   AND   (FP),Y
   LDY   #$0D
   STA   (FP),Y
   LDY   #$0A
   LDA   (FP),Y
   LDY   #$0C
   AND   (FP),Y
   LDY   #$0E
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$0F
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$01
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   JSR   f8
   LDY   #$10
   STA   (FP),Y
   TXA
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
   LDY   #$15
   LDA   (FP),Y
   LDY   #$1A
   STA   (FP),Y
   LDY   #$16
   LDA   (FP),Y
   AND   #$01
   LDY   #$1B
   STA   (FP),Y
   LDY   #$17
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
   LDY   #$17
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   INY
   ORA   (FP),Y
   INY
   ORA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_else@1
   LDA   ga5
   INY
   STA   (FP),Y
   LDA   ga5+1
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$1E
   STA   (FP),Y
   LDA   gp7
   STA   DPTR
   LDA   gp7+1
   STA   DPTR+1
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDA   gp7
   STA   DPTR
   LDA   gp7+1
   STA   DPTR+1
   LDA   (DPTR),Y
   LDY   #$1F
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
   LDY   #$21
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   SEC
   SBC   #$FF
   LDY   #$22
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   SBC   #$00
   LDY   #$23
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$24
   STA   (FP),Y
   LDA   gp7
   STA   DPTR
   LDA   gp7+1
   STA   DPTR+1
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   JMP   .if_end@0
.if_else@1:
   LDA   z4
   LDY   #$25
   STA   (FP),Y
   LDA   z4
   BMI   .sx_neg@35
   LDA   #$00
   JMP   .sx_done@36
.sx_neg@35:
   LDA   #$FF
.sx_done@36:
   LDY   #$26
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
   BMI   .sx_neg@37
   LDA   #$00
   JMP   .sx_done@38
.sx_neg@37:
   LDA   #$FF
.sx_done@38:
   LDY   #$29
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
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
   INY
   STA   (FP),Y
   AND   #$01
   INY
   STA   (FP),Y
   LDA   #$01
   CLC
   LDY   #$2C
   ADC   (FP),Y
   LDY   #$31
   STA   (FP),Y
   LDA   #$00
   LDY   #$2D
   ADC   (FP),Y
   LDY   #$32
   STA   (FP),Y
   LDA   #$00
   LDY   #$2E
   ADC   (FP),Y
   LDY   #$33
   STA   (FP),Y
   LDA   #$00
   LDY   #$2F
   ADC   (FP),Y
   LDY   #$34
   STA   (FP),Y
   LDA   #$00
   LDY   #$30
   ADC   (FP),Y
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
   LDY   #$39
   STA   (FP),Y
   LDY   #$35
   LDA   (FP),Y
   AND   #$01
   LDY   #$3A
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
   LDY   #$3D
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$3E
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$3F
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$40
   STA   (FP),Y
   LDY   #$02
   LDA   (DPTR),Y
   LDY   #$41
   STA   (FP),Y
   LDY   #$03
   LDA   (DPTR),Y
   LDY   #$42
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
   LDY   #$45
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$46
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$36
   LDA   (FP),Y
   LDY   #$45
   ORA   (FP),Y
   LDY   #$4A
   STA   (FP),Y
   LDY   #$37
   LDA   (FP),Y
   LDY   #$46
   ORA   (FP),Y
   LDY   #$4B
   STA   (FP),Y
   LDY   #$38
   LDA   (FP),Y
   LDY   #$47
   ORA   (FP),Y
   LDY   #$4C
   STA   (FP),Y
   LDY   #$39
   LDA   (FP),Y
   LDY   #$48
   ORA   (FP),Y
   LDY   #$4D
   STA   (FP),Y
   LDY   #$3A
   LDA   (FP),Y
   LDY   #$49
   ORA   (FP),Y
   LDY   #$4E
   STA   (FP),Y
   LDY   #$4A
   LDA   (FP),Y
   LDY   #$4F
   STA   (FP),Y
   LDY   #$4B
   LDA   (FP),Y
   LDY   #$50
   STA   (FP),Y
   LDY   #$4C
   LDA   (FP),Y
   LDY   #$51
   STA   (FP),Y
   LDY   #$4D
   LDA   (FP),Y
   LDY   #$52
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
   LDY   #$07
   LDA   (FP),Y
   SEC
   SBC   #$81
   LDY   #$53
   STA   (FP),Y
   LDY   #$08
   LDA   (FP),Y
   SBC   #$00
   LDY   #$54
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$07
   STA   (FP),Y
   LDY   #$54
   LDA   (FP),Y
   LDY   #$08
   STA   (FP),Y
   LDY   #$53
   LDA   (FP),Y
   LDY   #$55
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$56
   STA   (FP),Y
   LDA   g3
   CLC
   DEY
   ADC   (FP),Y
   LDY   #$57
   STA   (FP),Y
   LDA   g3+1
   DEY
   ADC   (FP),Y
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
   LDA   #$07
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$59
   LDA   (FP),Y
   LDY   #$5B
   AND   (FP),Y
   LDY   #$5D
   STA   (FP),Y
   LDY   #$5A
   LDA   (FP),Y
   LDY   #$5C
   AND   (FP),Y
   LDY   #$5E
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   BEQ   .cmp_true@39
   LDA   #$00
   JMP   .cmp_end@40
.cmp_true@39:
   LDA   #$01
.cmp_end@40:
   LDY   #$5F
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .case@7
   LDY   #$5D
   LDA   (FP),Y
   CMP   #$01
   BEQ   .cmp_true@41
   LDA   #$00
   JMP   .cmp_end@42
.cmp_true@41:
   LDA   #$01
.cmp_end@42:
   LDY   #$61
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@6
   JMP   .switch@6_break
.lb_skip@6:
   JMP   .default@9
.case@7:
   LDA   #<ga5
   LDY   #$63
   STA   (FP),Y
   LDA   #>ga5
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
   LDY   #$67
   STA   (FP),Y
   STA   HARGS
   LDA   #$02
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
   LDY   #$63
   LDA   (FP),Y
   CLC
   LDY   #$69
   ADC   (FP),Y
   LDY   #$6B
   STA   (FP),Y
   LDY   #$64
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
   DEY
   LDA   (FP),Y
   ORA   #$5A
   LDY   #$6F
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$70
   STA   (FP),Y
   LDY   #$6B
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$6F
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$70
   LDA   (FP),Y
   LDY   #$01
   STA   (DPTR),Y
   LDY   #$6F
   LDA   (FP),Y
   STA   g3
   INY
   LDA   (FP),Y
   STA   g3+1
   LDA   #<gs6
   INY
   STA   (FP),Y
   LDA   #>gs6
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$05
   LDY   #$73
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$74
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$75
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$76
   STA   (FP),Y
   LDY   #$02
   LDA   (DPTR),Y
   LDY   #$77
   STA   (FP),Y
   LDY   #$03
   LDA   (DPTR),Y
   LDY   #$78
   STA   (FP),Y
   LDY   #$75
   LDA   (FP),Y
   SEC
   SBC   #$01
   LDY   #$79
   STA   (FP),Y
   LDY   #$76
   LDA   (FP),Y
   SBC   #$00
   LDY   #$7A
   STA   (FP),Y
   LDY   #$77
   LDA   (FP),Y
   SBC   #$00
   LDY   #$7B
   STA   (FP),Y
   LDY   #$78
   LDA   (FP),Y
   SBC   #$00
   LDY   #$7C
   STA   (FP),Y
   LDY   #$73
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
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
   JMP   .switch@6_break
.default@9:
   LDA   #$FF
   LDY   #$7D
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$7F
   INY
   STA   (FP),Y
   LDY   #$7D
   LDA   (FP),Y
   EOR   #$FF
   LDY   #$81
   STA   (FP),Y
   LDY   #$7E
   LDA   (FP),Y
   EOR   #$FF
   LDY   #$82
   STA   (FP),Y
   LDY   #$7F
   LDA   (FP),Y
   EOR   #$FF
   LDY   #$83
   STA   (FP),Y
   LDY   #$80
   LDA   (FP),Y
   EOR   #$FF
   LDY   #$84
   STA   (FP),Y
   LDY   #$81
   LDA   (FP),Y
   LDY   #$85
   STA   (FP),Y
   LDY   #$82
   LDA   (FP),Y
   LDY   #$86
   STA   (FP),Y
   LDA   #$07
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$85
   LDA   (FP),Y
   LDY   #$87
   AND   (FP),Y
   LDY   #$89
   STA   (FP),Y
   LDY   #$86
   LDA   (FP),Y
   LDY   #$88
   AND   (FP),Y
   LDY   #$8A
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CMP   #$01
   BEQ   .cmp_true@43
   LDA   #$00
   JMP   .cmp_end@44
.cmp_true@43:
   LDA   #$01
.cmp_end@44:
   LDY   #$8B
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .case@11
   LDY   #$89
   LDA   (FP),Y
   CMP   #$03
   BEQ   .cmp_true@45
   LDA   #$00
   JMP   .cmp_end@46
.cmp_true@45:
   LDA   #$01
.cmp_end@46:
   LDY   #$8D
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@5
   JMP   .switch@10_break
.lb_skip@5:
   LDY   #$89
   LDA   (FP),Y
   CMP   #$06
   BEQ   .cmp_true@47
   LDA   #$00
   JMP   .cmp_end@48
.cmp_true@47:
   LDA   #$01
.cmp_end@48:
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
   JMP   .case@13
.lb_skip@4:
   LDY   #$89
   LDA   (FP),Y
   CMP   #$07
   BEQ   .cmp_true@49
   LDA   #$00
   JMP   .cmp_end@50
.cmp_true@49:
   LDA   #$01
.cmp_end@50:
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
   JMP   .case@14
.lb_skip@3:
   JMP   .default@15
.case@11:
   LDY   #$03
   LDA   (FP),Y
   STA   HARGS
   INY
   LDA   (FP),Y
   STA   HARGS+1
   LDA   #$01
   STA   HARGS+2
   LDA   #$00
   STA   HARGS+3
   JSR   mul16
   LDA   HARGS+4
   LDY   #$93
   STA   (FP),Y
   LDA   HARGS+5
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$03
   STA   (FP),Y
   LDY   #$94
   LDA   (FP),Y
   LDY   #$04
   STA   (FP),Y
   LDY   #$93
   LDA   (FP),Y
   LDY   #$95
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$96
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
   LDY   #$95
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$96
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$97
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$98
   LDA   (FP),Y
.xjump@0:
   LDY   #$04
   STA   (SSP),Y
   JSR   mix
   JMP   .switch@10_break
.case@13:
   LDY   #$04
   LDA   (FP),Y
   LDY   #$9A
   STA   (FP),Y
   LSR
   STA   (FP),Y
   LDY   #$03
   LDA   (FP),Y
   LDY   #$99
   STA   (FP),Y
   ROR
   STA   (FP),Y
   INY
   LDA   (FP),Y
   LSR
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ROR
   STA   (FP),Y
   INY
   LDA   (FP),Y
   LSR
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ROR
   STA   (FP),Y
   LDY   #$03
   STA   (FP),Y
   LDY   #$9A
   LDA   (FP),Y
   LDY   #$04
   STA   (FP),Y
   LDY   #$99
   LDA   (FP),Y
   LDY   #$9B
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$9C
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   INY
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
   ADC   #$05
   LDY   #$A1
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$A2
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$A3
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$A4
   STA   (FP),Y
   LDY   #$02
   LDA   (DPTR),Y
   LDY   #$A5
   STA   (FP),Y
   LDY   #$03
   LDA   (DPTR),Y
   LDY   #$A6
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
   ADC   #$05
   LDY   #$A9
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$AA
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$AB
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$AC
   STA   (FP),Y
   LDY   #$02
   LDA   (DPTR),Y
   LDY   #$AD
   STA   (FP),Y
   LDY   #$03
   LDA   (DPTR),Y
   LDY   #$AE
   STA   (FP),Y
   LDY   #$A3
   LDA   (FP),Y
   CLC
   LDY   #$AB
   ADC   (FP),Y
   LDY   #$AF
   STA   (FP),Y
   LDY   #$A4
   LDA   (FP),Y
   LDY   #$AC
   ADC   (FP),Y
   LDY   #$B0
   STA   (FP),Y
   LDY   #$A5
   LDA   (FP),Y
   LDY   #$AD
   ADC   (FP),Y
   LDY   #$B1
   STA   (FP),Y
   LDY   #$A6
   LDA   (FP),Y
   LDY   #$AE
   ADC   (FP),Y
   LDY   #$B2
   STA   (FP),Y
   LDY   #$9B
   LDA   (FP),Y
   LDY   #$AF
   AND   (FP),Y
   LDY   #$B3
   STA   (FP),Y
   LDY   #$9C
   LDA   (FP),Y
   LDY   #$B0
   AND   (FP),Y
   LDY   #$B4
   STA   (FP),Y
   LDY   #$9D
   LDA   (FP),Y
   LDY   #$B1
   AND   (FP),Y
   LDY   #$B5
   STA   (FP),Y
   LDY   #$9E
   LDA   (FP),Y
   LDY   #$B2
   AND   (FP),Y
   LDY   #$B6
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$04
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDY   #$B3
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$B4
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$B5
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$B6
   LDA   (FP),Y
   JMP   .xjump@0
.case@14:
   LDA   g2
   LDY   #$B7
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
   LDA   g2
   CLC
   ADC   #$01
   INY
   STA   (FP),Y
   LDA   g2+1
   ADC   #$00
   INY
   STA   (FP),Y
   LDA   g2+2
   ADC   #$00
   INY
   STA   (FP),Y
   LDA   g2+3
   ADC   #$00
   INY
   STA   (FP),Y
   LDA   g2+4
   ADC   #$00
   INY
   STA   (FP),Y
   LDY   #$BC
   LDA   (FP),Y
   LDY   #$C1
   STA   (FP),Y
   LDY   #$BD
   LDA   (FP),Y
   LDY   #$C2
   STA   (FP),Y
   LDY   #$BE
   LDA   (FP),Y
   LDY   #$C3
   STA   (FP),Y
   LDY   #$BF
   LDA   (FP),Y
   LDY   #$C4
   STA   (FP),Y
   LDY   #$C0
   LDA   (FP),Y
   AND   #$01
   LDY   #$C5
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
.default@15:
.switch@10_break:
.switch@6_break:
.if_end@0:
   LDA   z4
   LDY   #$C6
   STA   (FP),Y
   LDA   z4
   BMI   .sx_neg@51
   LDA   #$00
   JMP   .sx_done@52
.sx_neg@51:
   LDA   #$FF
.sx_done@52:
   LDY   #$C7
   STA   (FP),Y
   LDA   #$FF
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$C6
   LDA   (FP),Y
   CLC
   LDY   #$C8
   ADC   (FP),Y
   LDY   #$CA
   STA   (FP),Y
   LDY   #$C7
   LDA   (FP),Y
   LDY   #$C9
   ADC   (FP),Y
   LDY   #$CB
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$CC
   STA   (FP),Y

   ; epilogue
   PHA
   CLC
   LDA   FP
   ADC   #$CE
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

   ; prologue: 0 arg bytes, 75 local bytes
   SEC
   LDA   SSP
   SBC   #$4D
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$4C
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
   LDA   #$00
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
   LDY   #$0F
   STA   (FP),Y
   INY
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@53
   LDA   #$00
   JMP   .sx_done@54
.sx_neg@53:
   LDA   #$FF
.sx_done@54:
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
   LDX   #$06
.shift_loop@55:
   LDY   #$12
   LDA   (FP),Y
   ASL
   STA   (FP),Y
   INY
   LDA   (FP),Y
   ROL
   STA   (FP),Y
   DEX
   BNE   .shift_loop@55
   DEY
   LDA   (FP),Y
   LDY   #$14
   STA   (FP),Y
   LDA   gp7
   STA   DPTR
   LDA   gp7+1
   STA   DPTR+1
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDA   #<gs6
   LDY   #$15
   STA   (FP),Y
   LDA   #>gs6
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$05
   LDY   #$17
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$18
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
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
   LDA   #<gs6
   INY
   STA   (FP),Y
   LDA   #>gs6
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$05
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
   LDY   #$19
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$1A
   LDA   (FP),Y
   LDY   #$01
   STA   (DPTR),Y
   LDY   #$1B
   LDA   (FP),Y
   LDY   #$02
   STA   (DPTR),Y
   LDY   #$1C
   LDA   (FP),Y
   LDY   #$03
   STA   (DPTR),Y
   LDY   #$0E
   LDA   (FP),Y
   LDY   #$21
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
   LDA   g3
   CLC
   ADC   #$01
   LDY   #$25
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
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
.loop@16_continue:
   LDY   #$28
   LDA   (FP),Y
   INY
   STA   (FP),Y
   CLC
   ADC   #$01
   INY
   STA   (FP),Y
   LDY   #$28
   STA   (FP),Y
   INY
   LDA   (FP),Y
   LDY   #$2B
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
   BVC   .cmp_novf@57
   EOR   #$80
.cmp_novf@57:
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
   BEQ   .loop@16_break
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   SEC
   SBC   #$03
   LDA   (FP),Y
   SBC   #$00
   BVC   .cmp_novf@58
   EOR   #$80
.cmp_novf@58:
   ASL
   LDA   #$00
   ROL
   LDY   #$33
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   JMP   .loop@16_continue
.loop@16_break:
   LDA   g2
   AND   #$01
   LDY   #$37
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
   LDY   #$37
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
   LDY   #$09
   LDA   (FP),Y
   LDY   #$3C
   STA   (FP),Y
   LDY   #$0A
   LDA   (FP),Y
   LDY   #$3D
   STA   (FP),Y
   LDY   #$0B
   LDA   (FP),Y
   LDY   #$3E
   STA   (FP),Y
   LDY   #$0C
   LDA   (FP),Y
   LDY   #$3F
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$37
   LDA   (FP),Y
   LDY   #$3C
   EOR   (FP),Y
   LDY   #$41
   STA   (FP),Y
   LDY   #$38
   LDA   (FP),Y
   LDY   #$3D
   EOR   (FP),Y
   LDY   #$42
   STA   (FP),Y
   LDY   #$39
   LDA   (FP),Y
   LDY   #$3E
   EOR   (FP),Y
   LDY   #$43
   STA   (FP),Y
   LDY   #$3A
   LDA   (FP),Y
   LDY   #$3F
   EOR   (FP),Y
   LDY   #$44
   STA   (FP),Y
   LDY   #$3B
   LDA   (FP),Y
   LDY   #$40
   EOR   (FP),Y
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
   DEY
   LDA   (FP),Y
   STA   ga5+4
   INY
   LDA   (FP),Y
   STA   ga5+5
   JSR   fin0
   JSR   fin1
   JSR   fin2
   LDA   acc
   LDY   #$48
   STA   (FP),Y
   LDA   acc+1
   AND   #$7F
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$4A
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$4B
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
   ADC   #$4D
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$4C
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
   DC.B  $FF
   DC.B  $FF
   DS.B  3

g3:
   DC.W  $FFFF

ga5:
   DC.W  $0080
   DC.W  $FFFF
   DC.W  $0001
   DC.W  $007F

gs6:
   DS.B  1
   DC.L  $00008000
   DC.L  $0000000B

gp7:
   DC.W  z4

@5.s16:
   DC.L  $00000002

__zp_init:
   SUBROUTINE

   LDA   #$01
   STA   z4
   RTS
