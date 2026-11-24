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
   INY
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@0
   LDA   #$00
   JMP   .sx_done@1
.sx_neg@0:
   LDA   #$FF
.sx_done@1:
   LDY   #$03
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
   LDA   g2
   LDY   #$06
   STA   (FP),Y
   LDA   g2+1
   INY
   STA   (FP),Y
   LDA   g2+1
   BMI   .sx_neg@2
   LDA   #$00
   JMP   .sx_done@3
.sx_neg@2:
   LDA   #$FF
.sx_done@3:
   LDY   #$08
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
   LDA   g3
   LDY   #$0A
   STA   (FP),Y
   LDA   g3+1
   INY
   STA   (FP),Y
   LDA   g3+2
   INY
   STA   (FP),Y
   LDA   g3+3
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
   ORA   #$00
   BMI   .sx_neg@4
   LDA   #$00
   JMP   .sx_done@5
.sx_neg@4:
   LDA   #$FF
.sx_done@5:
   LDY   #$10
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
   ORA   #$00
   BMI   .sx_neg@6
   LDA   #$00
   JMP   .sx_done@7
.sx_neg@6:
   LDA   #$FF
.sx_done@7:
   LDY   #$03
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
   ORA   #$00
   BMI   .sx_neg@8
   LDA   #$00
   JMP   .sx_done@9
.sx_neg@8:
   LDA   #$FF
.sx_done@9:
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
   LDA   ga4+3
   LDY   #$0B
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
   LDY   #$0D
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

   ; prologue: 0 arg bytes, 9 local bytes
   SEC
   LDA   SSP
   SBC   #$0B
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$0A
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

   ; epilogue
   CLC
   LDA   FP
   ADC   #$0B
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$0A
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   RTS

f6:
   SUBROUTINE

   ; prologue: 7 arg bytes, 105 local bytes
   SEC
   LDA   SSP
   SBC   #$6B
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$6A
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
   LDA   #$03
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
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
.loop@0_continue:
   LDY   #$10
   LDA   (FP),Y
   INY
   STA   (FP),Y
   CLC
   ADC   #$01
   INY
   STA   (FP),Y
   LDY   #$10
   STA   (FP),Y
   INY
   LDA   (FP),Y
   LDY   #$13
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
   BVC   .cmp_novf@12
   EOR   #$80
.cmp_novf@12:
   ASL
   LDA   #$00
   ROL
   LDY   #$15
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
   LDA   #<gs5
   LDY   #$17
   STA   (FP),Y
   LDA   #>gs5
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$02
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
   ORA   #$00
   BEQ   .lb_skip@2
   JMP   .loop@0_continue
.lb_skip@2:
   LDY   #$0B
   LDA   (FP),Y
   LDY   #$1C
   STA   (FP),Y
   LDY   #$0C
   LDA   (FP),Y
   LDY   #$1D
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$1E
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
   LDY   #$21
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$22
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
   LDA   #$00
   LDY   #$23
   STA   (FP),Y
   INY
   STA   (FP),Y
.loop@1_continue:
   LDY   #$24
   LDA   (FP),Y
   INY
   STA   (FP),Y
   CLC
   ADC   #$01
   INY
   STA   (FP),Y
   LDY   #$24
   STA   (FP),Y
   INY
   LDA   (FP),Y
   LDY   #$27
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
   BVC   .cmp_novf@13
   EOR   #$80
.cmp_novf@13:
   ASL
   LDA   #$00
   ROL
   LDY   #$29
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@1
   JMP   .loop@1_break
.lb_skip@1:
   LDY   #$0E
   LDA   (FP),Y
   LDY   #$2B
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$71
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$2E
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
   LDY   #$30
   STA   (FP),Y
   LDY   #$2C
   LDA   (FP),Y
   LDY   #$2F
   AND   (FP),Y
   LDY   #$31
   STA   (FP),Y
   LDY   #$2D
   LDA   (FP),Y
   LDY   #$30
   AND   (FP),Y
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
   CMP   #$03
   BEQ   .cmp_true@16
   LDA   #$00
   JMP   .cmp_end@17
.cmp_true@16:
   LDA   #$01
.cmp_end@17:
   LDY   #$37
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   LDA   g2
   ORA   g2+1
   BNE   .lb_skip@0
   JMP   .loop@1_continue
.lb_skip@0:
.loop@1_break:
   LDA   #<gs5
   LDY   #$39
   STA   (FP),Y
   LDA   #>gs5
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$01
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
   ORA   #$00
   BEQ   .and_false@2
   LDY   #$10
   LDA   (FP),Y
   BEQ   .and_false@2
   LDA   #$01
   LDY   #$3E
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   JMP   .and_end@3
.and_false@2:
   LDA   #$00
   LDY   #$3E
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@3:
   LDY   #$3E
   LDA   (FP),Y
   LDY   #$40
   STA   (FP),Y
   LDY   #$6C
   STA   (FP),Y
   JMP   .loop@0_continue
.loop@0_break:
   LDA   #<ga4
   LDY   #$41
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
   LDY   #$45
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$41
   LDA   (FP),Y
   CLC
   LDY   #$46
   ADC   (FP),Y
   LDY   #$48
   STA   (FP),Y
   LDY   #$42
   LDA   (FP),Y
   LDY   #$47
   ADC   (FP),Y
   LDY   #$49
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$4A
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
   LDY   #$4C
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
   LDY   #$4D
   STA   (FP),Y
   LDA   HARGS+5
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$4F
   STA   (FP),Y
   LDY   #$48
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$4F
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$4F
   LDA   (FP),Y
   INY
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@20
   LDA   #$00
   JMP   .sx_done@21
.sx_neg@20:
   LDA   #$FF
.sx_done@21:
   LDY   #$51
   STA   (FP),Y
   LDA   g1
   INY
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
   LDY   #$54
   STA   (FP),Y
   LDY   #$50
   LDA   (FP),Y
   LDY   #$53
   ORA   (FP),Y
   LDY   #$55
   STA   (FP),Y
   LDY   #$51
   LDA   (FP),Y
   LDY   #$54
   ORA   (FP),Y
   LDY   #$56
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$57
   STA   (FP),Y
   STA   g1
   LDA   #$7F
   INY
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
   BMI   .sx_neg@26
   LDA   #$00
   JMP   .sx_done@27
.sx_neg@26:
   LDA   #$FF
.sx_done@27:
   LDY   #$5D
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$5B
   LDA   (FP),Y
   CLC
   LDY   #$05
   ADC   (FP),Y
   LDY   #$5F
   STA   (FP),Y
   LDY   #$5C
   LDA   (FP),Y
   LDY   #$06
   ADC   (FP),Y
   LDY   #$60
   STA   (FP),Y
   LDY   #$5D
   LDA   (FP),Y
   LDY   #$07
   ADC   (FP),Y
   LDY   #$61
   STA   (FP),Y
   LDY   #$5E
   LDA   (FP),Y
   LDY   #$08
   ADC   (FP),Y
   LDY   #$62
   STA   (FP),Y
   LDY   #$5F
   LDA   (FP),Y
   LDY   #$63
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
   LDY   #$66
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$67
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$63
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDA   g1
   LDY   #$68
   STA   (FP),Y
   INY
   STA   (FP),Y

   ; epilogue
   PHA
   CLC
   LDA   FP
   ADC   #$72
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

f15:
   SUBROUTINE

   ; prologue: 4 arg bytes, 34 local bytes
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

   LDA   #$03
   LDY   #$01
   STA   (FP),Y
   INY
   STA   (FP),Y
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
   LDA   #$80
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
   LDA   #$01
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$05
   LDA   (FP),Y
   LDY   #$0D
   STA   (FP),Y
   LDY   #$06
   LDA   (FP),Y
   LDY   #$0E
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   g2
   INY
   LDA   (FP),Y
   STA   g2+1
   LDY   #$02
   LDA   (FP),Y
   LDY   #$0F
   STA   (FP),Y
   SEC
   SBC   #$01
   INY
   STA   (FP),Y
   LDY   #$02
   STA   (FP),Y
   SEC
   LDA   #$00
   LDY   #$25
   SBC   (FP),Y
   LDY   #$11
   STA   (FP),Y
   LDA   #$00
   LDY   #$26
   SBC   (FP),Y
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
   LDY   #$0A
   LDA   (FP),Y
   LDY   #$18
   STA   (FP),Y
   ASL
   LDA   (FP),Y
   ROR
   STA   (FP),Y
   LDY   #$09
   LDA   (FP),Y
   LDY   #$17
   STA   (FP),Y
   ROR
   STA   (FP),Y
   INY
   LDA   (FP),Y
   ASL
   LDA   (FP),Y
   ROR
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ROR
   STA   (FP),Y
   LDY   #$09
   STA   (FP),Y
   LDY   #$18
   LDA   (FP),Y
   LDY   #$0A
   STA   (FP),Y
   LDY   #$0C
   LDA   (FP),Y
   LDY   #$19
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@28
   LDA   #$00
   JMP   .sx_done@29
.sx_neg@28:
   LDA   #$FF
.sx_done@29:
   LDY   #$1A
   STA   (FP),Y
   LDY   #$17
   LDA   (FP),Y
   SEC
   LDY   #$19
   SBC   (FP),Y
   LDY   #$1B
   STA   (FP),Y
   LDY   #$18
   LDA   (FP),Y
   LDY   #$1A
   SBC   (FP),Y
   LDY   #$1C
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$1D
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$1E
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$25
   STA   (FP),Y
   LDY   #$1E
   LDA   (FP),Y
   LDY   #$26
   STA   (FP),Y
   LDA   g1
   LDY   #$1F
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
   LDY   #$21
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$05
   STA   (FP),Y
   LDY   #$21
   LDA   (FP),Y
   LDY   #$06
   STA   (FP),Y
   LDY   #$25
   LDA   (FP),Y
   LDY   #$22
   STA   (FP),Y

   ; epilogue
   PHA
   CLC
   LDA   FP
   ADC   #$28
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

   LDA   #$00
   LDY   #$01
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
   LDA   #$FF
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$09
   LDA   (FP),Y
   CLC
   ADC   #$01
   LDY   #$0D
   STA   (FP),Y
   LDY   #$0A
   LDA   (FP),Y
   ADC   #$00
   LDY   #$0E
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$09
   STA   (FP),Y
   LDY   #$0E
   LDA   (FP),Y
   LDY   #$0A
   STA   (FP),Y
   LDY   #$06
   LDA   (FP),Y
   LDY   #$0F
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
   LDY   #$12
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$13
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$14
   STA   (FP),Y
   LDY   #$06
   STA   (FP),Y
   LDY   #$15
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$17
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$18
   STA   (FP),Y
   LDY   #$09
   LDA   (FP),Y
   LDY   #$17
   AND   (FP),Y
   LDY   #$19
   STA   (FP),Y
   LDY   #$0A
   LDA   (FP),Y
   LDY   #$18
   AND   (FP),Y
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
   LDY   #$1B
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$1C
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$1D
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$1E
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   JSR   mix
   LDA   #$01
   LDY   #$1F
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   ORA   #$00
   BPL   .sx_done@33
   LDA   #$FF
.sx_done@33:
   LDY   #$21
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   g3
   LDY   #$1F
   EOR   (FP),Y
   LDY   #$23
   STA   (FP),Y
   LDA   g3+1
   LDY   #$20
   EOR   (FP),Y
   LDY   #$24
   STA   (FP),Y
   LDA   g3+2
   LDY   #$21
   EOR   (FP),Y
   LDY   #$25
   STA   (FP),Y
   LDA   g3+3
   LDY   #$22
   EOR   (FP),Y
   LDY   #$26
   STA   (FP),Y
   LDY   #$23
   LDA   (FP),Y
   STA   g3
   INY
   LDA   (FP),Y
   STA   g3+1
   INY
   LDA   (FP),Y
   STA   g3+2
   INY
   LDA   (FP),Y
   STA   g3+3
   LDA   g1
   INY
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
   LDY   #$29
   STA   (FP),Y
   LDA   ga4+1
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
   LDY   #$2C
   STA   (FP),Y
   LDY   #$28
   LDA   (FP),Y
   LDY   #$2B
   AND   (FP),Y
   LDY   #$2D
   STA   (FP),Y
   LDY   #$29
   LDA   (FP),Y
   LDY   #$2C
   AND   (FP),Y
   LDY   #$2E
   STA   (FP),Y
   LDY   #$30
   STA   (FP),Y
   ASL
   LDA   (FP),Y
   ROR
   STA   (FP),Y
   LDY   #$2D
   LDA   (FP),Y
   LDY   #$2F
   STA   (FP),Y
   ROR
   STA   (FP),Y
   LDY   #$31
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$32
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$09
   STA   (FP),Y
   LDY   #$32
   LDA   (FP),Y
   LDY   #$0A
   STA   (FP),Y
   LDA   ga4+1
   LDY   #$33
   STA   (FP),Y
   ORA   #$00
   BNE   .or_true@4
   LDA   #$05
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$34
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   INY
   ORA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@4
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   JMP   .or_end@5
.or_true@4:
   LDA   #$01
   LDY   #$38
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@5:
   LDA   #<gs5
   LDY   #$3A
   STA   (FP),Y
   LDA   #>gs5
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$01
   LDY   #$3C
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$3D
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$3E
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   SEC
   SBC   #$05
   LDY   #$41
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   SBC   #$00
   LDY   #$42
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$43
   STA   (FP),Y
   LDY   #$3C
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$43
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$43
   LDA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$38
   LDA   (FP),Y
   LDY   #$44
   ORA   (FP),Y
   LDY   #$46
   STA   (FP),Y
   LDY   #$39
   LDA   (FP),Y
   LDY   #$45
   ORA   (FP),Y
   LDY   #$47
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   g2
   INY
   LDA   (FP),Y
   STA   g2+1
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
   DS.B  1

g2:
   DC.W  $007F

g3:
   DC.L  $00000001

ga4:
   DC.B  $00, $02, $FF, $01

gs5:
   DS.B  1
   DC.B  $7F
   DS.B  1
