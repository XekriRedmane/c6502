SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
z4	EQU	$26
z5	EQU	$28

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
   LDA   g3+1
   BMI   .sx_neg@2
   LDA   #$00
   JMP   .sx_done@3
.sx_neg@2:
   LDA   #$FF
.sx_done@3:
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
   LDA   z4
   LDY   #$0D
   STA   (FP),Y
   LDA   z4+1
   INY
   STA   (FP),Y
   LDA   z4+1
   BMI   .sx_neg@4
   LDA   #$00
   JMP   .sx_done@5
.sx_neg@4:
   LDA   #$FF
.sx_done@5:
   LDY   #$0F
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
   LDA   ga6
   LDY   #$09
   STA   (FP),Y
   LDA   ga6+1
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
   LDA   ga6+3
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
   LDA   ga6+4
   LDY   #$15
   STA   (FP),Y
   LDA   ga6+5
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

   LDA   ga6+6
   LDY   #$01
   STA   (FP),Y
   LDA   ga6+7
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
   LDA   #<gs7
   LDY   #$07
   STA   (FP),Y
   LDA   #>gs7
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$01
   LDY   #$09
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$0A
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
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
   LDA   #<gs7
   LDY   #$10
   STA   (FP),Y
   LDA   #>gs7
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$02
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

f9:
   SUBROUTINE

   ; prologue: 5 arg bytes, 123 local bytes
   SEC
   LDA   SSP
   SBC   #$7D
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$7C
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
   INY
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
   INY
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
   LDA   #$00
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
   LDY   #$02
   LDA   (FP),Y
   CLC
   ADC   #$01
   LDY   #$0F
   STA   (FP),Y
   LDY   #$02
   STA   (FP),Y
   LDY   #$10
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
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$10
   LDA   (FP),Y
   LDY   #$03
   STA   (FP),Y
   LDY   #$11
   LDA   (FP),Y
   LDY   #$04
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
   LDA   #$00
   LDY   #$18
   STA   (FP),Y
   INY
   STA   (FP),Y
.loop@0_start:
   LDY   #$02
   LDA   (FP),Y
   LDY   #$1A
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@8
   LDA   #$00
   JMP   .sx_done@9
.sx_neg@8:
   LDA   #$FF
.sx_done@9:
   LDY   #$1B
   STA   (FP),Y
   LDA   #$07
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$1A
   LDA   (FP),Y
   LDY   #$1C
   AND   (FP),Y
   LDY   #$1E
   STA   (FP),Y
   LDY   #$1B
   LDA   (FP),Y
   LDY   #$1D
   AND   (FP),Y
   LDY   #$1F
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CMP   #$01
   BEQ   .cmp_true@10
   LDA   #$00
   JMP   .cmp_end@11
.cmp_true@10:
   LDA   #$01
.cmp_end@11:
   LDY   #$20
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .case@2
   LDY   #$1E
   LDA   (FP),Y
   CMP   #$02
   BEQ   .cmp_true@12
   LDA   #$00
   JMP   .cmp_end@13
.cmp_true@12:
   LDA   #$01
.cmp_end@13:
   LDY   #$22
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@4
   JMP   .case@3
.lb_skip@4:
   LDY   #$1E
   LDA   (FP),Y
   CMP   #$03
   BEQ   .cmp_true@14
   LDA   #$00
   JMP   .cmp_end@15
.cmp_true@14:
   LDA   #$01
.cmp_end@15:
   LDY   #$24
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@3
   JMP   .switch@1_break
.lb_skip@3:
   JMP   .default@5
.case@2:
   LDY   #$19
   LDA   (FP),Y
   BEQ   .cond_else@0
   LDY   #$0D
   LDA   (FP),Y
   LDY   #$26
   STA   (FP),Y
   LDY   #$0E
   LDA   (FP),Y
   LDY   #$27
   STA   (FP),Y
   JMP   .cond_end@1
.cond_else@0:
   LDY   #$28
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$2A
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$2B
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$26
   STA   (FP),Y
   LDY   #$2B
   LDA   (FP),Y
   LDY   #$27
   STA   (FP),Y
.cond_end@1:
   LDY   #$26
   LDA   (FP),Y
   LDY   #$2C
   STA   (FP),Y
   LDA   gp8
   STA   DPTR
   LDA   gp8+1
   STA   DPTR+1
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   JMP   .switch@1_break
.case@3:
   LDA   #$7F
   LDY   #$2D
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
   LDY   #$2F
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$30
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$31
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
   ADC   #$02
   LDY   #$34
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$35
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$36
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$37
   STA   (FP),Y
   LDY   #$30
   LDA   (FP),Y
   CLC
   LDY   #$36
   ADC   (FP),Y
   LDY   #$38
   STA   (FP),Y
   LDY   #$31
   LDA   (FP),Y
   LDY   #$37
   ADC   (FP),Y
   LDY   #$39
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$3A
   STA   (FP),Y
   DEY
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
   JMP   .switch@1_break
.default@5:
   LDY   #$0D
   LDA   (FP),Y
   SEC
   SBC   #$05
   LDY   #$3E
   STA   (FP),Y
   LDY   #$0E
   LDA   (FP),Y
   SBC   #$00
   LDY   #$3F
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$0D
   STA   (FP),Y
   LDY   #$3F
   LDA   (FP),Y
   LDY   #$0E
   STA   (FP),Y
.switch@1_break:
   LDY   #$19
   LDA   (FP),Y
   CLC
   ADC   #$01
   LDY   #$4A
   STA   (FP),Y
   LDY   #$19
   STA   (FP),Y
   LDY   #$4B
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
   BVC   .cmp_novf@20
   EOR   #$80
.cmp_novf@20:
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
   BEQ   .lb_skip@2
   JMP   .loop@0_start
.lb_skip@2:
   LDA   g1
   CLC
   ADC   #$01
   LDY   #$4F
   STA   (FP),Y
   STA   g1
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
   LDY   #$50
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$51
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$52
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$53
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   JSR   mix
   LDA   g3
   LDY   #$54
   STA   (FP),Y
   LDA   g3+1
   INY
   STA   (FP),Y
   LDA   g3
   SEC
   SBC   #$01
   INY
   STA   (FP),Y
   LDA   g3+1
   SBC   #$00
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
   LDY   #$54
   LDA   (FP),Y
   LDY   #$58
   ORA   (FP),Y
   LDY   #$5A
   STA   (FP),Y
   LDY   #$55
   LDA   (FP),Y
   LDY   #$59
   ORA   (FP),Y
   LDY   #$5B
   STA   (FP),Y
   LDA   #$07
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$5A
   LDA   (FP),Y
   LDY   #$5C
   AND   (FP),Y
   LDY   #$5E
   STA   (FP),Y
   LDY   #$5B
   LDA   (FP),Y
   LDY   #$5D
   AND   (FP),Y
   LDY   #$5F
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CMP   #$00
   BEQ   .cmp_true@21
   LDA   #$00
   JMP   .cmp_end@22
.cmp_true@21:
   LDA   #$01
.cmp_end@22:
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
   LDY   #$5E
   LDA   (FP),Y
   CMP   #$03
   BEQ   .cmp_true@23
   LDA   #$00
   JMP   .cmp_end@24
.cmp_true@23:
   LDA   #$01
.cmp_end@24:
   LDY   #$62
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@1
   JMP   .switch@6_break
.lb_skip@1:
   LDY   #$5E
   LDA   (FP),Y
   CMP   #$07
   BEQ   .cmp_true@25
   LDA   #$00
   JMP   .cmp_end@26
.cmp_true@25:
   LDA   #$01
.cmp_end@26:
   LDY   #$64
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@0
   JMP   .case@9
.lb_skip@0:
   JMP   .switch@6_break
.case@7:
   LDA   z5
   LDY   #$66
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
   LDA   #$01
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   ORA   #$00
   BPL   .sx_done@28
   LDA   #$FF
.sx_done@28:
   LDY   #$6C
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$66
   LDA   (FP),Y
   LDY   #$6A
   EOR   (FP),Y
   LDY   #$6E
   STA   (FP),Y
   LDY   #$67
   LDA   (FP),Y
   LDY   #$6B
   EOR   (FP),Y
   LDY   #$6F
   STA   (FP),Y
   LDY   #$68
   LDA   (FP),Y
   LDY   #$6C
   EOR   (FP),Y
   LDY   #$70
   STA   (FP),Y
   LDY   #$69
   LDA   (FP),Y
   LDY   #$6D
   EOR   (FP),Y
   LDY   #$71
   STA   (FP),Y
   LDY   #$6E
   LDA   (FP),Y
   STA   z5
   INY
   LDA   (FP),Y
   STA   z5+1
   INY
   LDA   (FP),Y
   STA   z5+2
   INY
   LDA   (FP),Y
   STA   z5+3
   LDY   #$6E
   LDA   (FP),Y
   LDY   #$72
   STA   (FP),Y
   LDY   #$6F
   LDA   (FP),Y
   LDY   #$73
   STA   (FP),Y
   LDY   #$70
   LDA   (FP),Y
   LDY   #$74
   STA   (FP),Y
   LDY   #$71
   LDA   (FP),Y
   LDY   #$75
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$04
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDY   #$72
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$73
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$74
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$75
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   JSR   mix
.case@9:
.switch@6_break:
   LDA   #<gs7
   LDY   #$76
   STA   (FP),Y
   LDA   #>gs7
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$01
   LDY   #$78
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$79
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$7A
   STA   (FP),Y
   INY
   STA   (FP),Y

   ; epilogue
   PHA
   CLC
   LDA   FP
   ADC   #$82
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

f17:
   SUBROUTINE

   ; prologue: 4 arg bytes, 111 local bytes
   SEC
   LDA   SSP
   SBC   #$71
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$70
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
   LDA   #$00
   INY
   STA   (FP),Y
   LDA   #$01
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
   LDY   #$08
   STA   (FP),Y
   ASL
   LDA   (FP),Y
   ROR
   STA   (FP),Y
   LDY   #$05
   LDA   (FP),Y
   LDY   #$07
   STA   (FP),Y
   ROR
   STA   (FP),Y
   LDY   #$05
   STA   (FP),Y
   LDY   #$08
   LDA   (FP),Y
   LDY   #$06
   STA   (FP),Y
   LDA   #$01
   LDY   #$09
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$07
   LDA   (FP),Y
   SEC
   LDY   #$0A
   SBC   (FP),Y
   LDY   #$0C
   STA   (FP),Y
   LDY   #$08
   LDA   (FP),Y
   LDY   #$0B
   SBC   (FP),Y
   LDY   #$0D
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$0E
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$0F
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@29
   LDA   #$00
   JMP   .sx_done@30
.sx_neg@29:
   LDA   #$FF
.sx_done@30:
   LDY   #$10
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
   LDA   g2
   SEC
   SBC   #$01
   LDY   #$12
   STA   (FP),Y
   STA   g2
   LDA   (FP),Y
   INY
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@31
   LDA   #$00
   JMP   .sx_done@32
.sx_neg@31:
   LDA   #$FF
.sx_done@32:
   LDY   #$14
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
   LDA   #$80
   INY
   STA   (FP),Y
   LDY   #$15
   LDA   (FP),Y
   LDY   #$17
   AND   (FP),Y
   LDY   #$19
   STA   (FP),Y
   LDY   #$16
   LDA   (FP),Y
   LDY   #$18
   AND   (FP),Y
   LDY   #$1A
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@7
   JMP   .if_end@2
.lb_skip@7:
   LDA   #$00
   LDY   #$1B
   STA   (FP),Y
   INY
   STA   (FP),Y
.loop@10_continue:
   LDY   #$1C
   LDA   (FP),Y
   INY
   STA   (FP),Y
   CLC
   ADC   #$01
   INY
   STA   (FP),Y
   LDY   #$1C
   STA   (FP),Y
   INY
   LDA   (FP),Y
   LDY   #$1F
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
   BVC   .cmp_novf@33
   EOR   #$80
.cmp_novf@33:
   ASL
   LDA   #$00
   ROL
   LDY   #$21
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@6
   JMP   .loop@10_break
.lb_skip@6:
   LDA   z5
   LDY   #$23
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
   LDY   #$23
   LDA   (FP),Y
   LDY   #$27
   STA   (FP),Y
   LDY   #$24
   LDA   (FP),Y
   LDY   #$28
   STA   (FP),Y
   LDY   #$25
   LDA   (FP),Y
   LDY   #$29
   STA   (FP),Y
   LDY   #$26
   LDA   (FP),Y
   LDY   #$2A
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@34
   LDA   #$00
   JMP   .sx_done@35
.sx_neg@34:
   LDA   #$FF
.sx_done@35:
   LDY   #$2B
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$27
   LDA   (FP),Y
   LDY   #$2F
   STA   (FP),Y
   LDY   #$02
   STA   (FP),Y
   LDA   gp8
   STA   DPTR
   LDA   gp8+1
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$30
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   AND   #$5A
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
   LDA   gp8
   STA   DPTR
   LDA   gp8+1
   STA   DPTR+1
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$35
   LDA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$1C
   LDA   (FP),Y
   LDY   #$38
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$36
   LDA   (FP),Y
   LDY   #$38
   EOR   (FP),Y
   LDY   #$3A
   STA   (FP),Y
   LDY   #$37
   LDA   (FP),Y
   LDY   #$39
   EOR   (FP),Y
   LDY   #$3B
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@5
   JMP   .if_end@3
.lb_skip@5:
   LDA   z4
   LDY   #$3C
   STA   (FP),Y
   LDA   z4+1
   INY
   STA   (FP),Y
   LDA   z4
   CLC
   ADC   #$01
   INY
   STA   (FP),Y
   LDA   z4+1
   ADC   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   z4
   INY
   LDA   (FP),Y
   STA   z4+1
   LDA   g1
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDA   gp8
   STA   DPTR
   LDA   gp8+1
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$42
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$40
   LDA   (FP),Y
   CLC
   LDY   #$43
   ADC   (FP),Y
   LDY   #$45
   STA   (FP),Y
   LDY   #$41
   LDA   (FP),Y
   LDY   #$44
   ADC   (FP),Y
   LDY   #$46
   STA   (FP),Y
   LDY   #$3C
   LDA   (FP),Y
   LDY   #$45
   EOR   (FP),Y
   LDY   #$47
   STA   (FP),Y
   LDY   #$3D
   LDA   (FP),Y
   LDY   #$46
   EOR   (FP),Y
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
   TAX
   DEY
   LDA   (FP),Y

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$75
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$70
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
.if_end@3:
   LDA   #$00
   LDY   #$50
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   CLC
   ADC   #$01
   INY
   STA   (FP),Y
   LDY   #$51
   STA   (FP),Y
   INY
   LDA   (FP),Y
   LDY   #$54
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
   BVC   .cmp_novf@36
   EOR   #$80
.cmp_novf@36:
   ASL
   LDA   #$00
   ROL
   LDY   #$56
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   JMP   .loop@10_continue
.loop@10_break:
.if_end@2:
   LDY   #$73
   LDA   (FP),Y
   LDY   #$68
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$6A
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$6B
   STA   (FP),Y
   LDA   ga6+6
   INY
   STA   (FP),Y
   LDA   ga6+7
   INY
   STA   (FP),Y
   LDY   #$6A
   LDA   (FP),Y
   SEC
   LDY   #$6C
   SBC   (FP),Y
   LDY   #$6E
   STA   (FP),Y
   LDY   #$6B
   LDA   (FP),Y
   LDY   #$6D
   SBC   (FP),Y
   LDY   #$6F
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
   ADC   #$75
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$70
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

   ; prologue: 0 arg bytes, 227 local bytes
   SEC
   LDA   SSP
   SBC   #$E5
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$E4
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   LDA   #$05
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
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
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
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   z5
   INY
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
   LDA   #$05
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   ORA   #$00
   BPL   .sx_done@40
   LDA   #$FF
.sx_done@40:
   LDY   #$15
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$0F
   LDA   (FP),Y
   CLC
   LDY   #$13
   ADC   (FP),Y
   LDY   #$17
   STA   (FP),Y
   LDY   #$10
   LDA   (FP),Y
   LDY   #$14
   ADC   (FP),Y
   LDY   #$18
   STA   (FP),Y
   LDY   #$11
   LDA   (FP),Y
   LDY   #$15
   ADC   (FP),Y
   LDY   #$19
   STA   (FP),Y
   LDY   #$12
   LDA   (FP),Y
   LDY   #$16
   ADC   (FP),Y
   LDY   #$1A
   STA   (FP),Y
   LDY   #$17
   LDA   (FP),Y
   STA   z5
   INY
   LDA   (FP),Y
   STA   z5+1
   INY
   LDA   (FP),Y
   STA   z5+2
   INY
   LDA   (FP),Y
   STA   z5+3
   LDY   #$0B
   LDA   (FP),Y
   SEC
   LDY   #$17
   SBC   (FP),Y
   LDY   #$1B
   STA   (FP),Y
   LDY   #$0C
   LDA   (FP),Y
   LDY   #$18
   SBC   (FP),Y
   LDY   #$1C
   STA   (FP),Y
   LDY   #$0D
   LDA   (FP),Y
   LDY   #$19
   SBC   (FP),Y
   LDY   #$1D
   STA   (FP),Y
   LDY   #$0E
   LDA   (FP),Y
   LDY   #$1A
   SBC   (FP),Y
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
   LDA   #$07
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$1F
   LDA   (FP),Y
   LDY   #$21
   AND   (FP),Y
   LDY   #$23
   STA   (FP),Y
   LDY   #$20
   LDA   (FP),Y
   LDY   #$22
   AND   (FP),Y
   LDY   #$24
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   BEQ   .cmp_true@41
   LDA   #$00
   JMP   .cmp_end@42
.cmp_true@41:
   LDA   #$01
.cmp_end@42:
   LDY   #$25
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .case@13
   LDY   #$23
   LDA   (FP),Y
   CMP   #$02
   BEQ   .cmp_true@43
   LDA   #$00
   JMP   .cmp_end@44
.cmp_true@43:
   LDA   #$01
.cmp_end@44:
   LDY   #$27
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .case@14
   JMP   .switch@12_break
.case@13:
   LDA   z5
   LDY   #$29
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
   LDY   #$29
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
   LDA   #$04
   STA   HARGS+4
   JSR   asl32
   LDA   HARGS+5
   LDY   #$2D
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
   LDY   #$2D
   LDA   (FP),Y
   STA   z5
   INY
   LDA   (FP),Y
   STA   z5+1
   INY
   LDA   (FP),Y
   STA   z5+2
   INY
   LDA   (FP),Y
   STA   z5+3
.case@14:
.switch@12_break:
   LDA   #$00
   LDY   #$31
   STA   (FP),Y
   INY
   STA   (FP),Y
.loop@15_start:
   LDA   #<ga6
   LDY   #$33
   STA   (FP),Y
   LDA   #>ga6
   INY
   STA   (FP),Y
   LDY   #$32
   LDA   (FP),Y
   LDY   #$35
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
   LDY   #$35
   LDA   (FP),Y
   LDY   #$37
   AND   (FP),Y
   LDY   #$39
   STA   (FP),Y
   LDY   #$36
   LDA   (FP),Y
   LDY   #$38
   AND   (FP),Y
   LDY   #$3A
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$3B
   STA   (FP),Y
   STA   HARGS
   LDA   #$02
   STA   HARGS+1
   JSR   mul8
   LDA   HARGS+2
   LDY   #$3C
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$33
   LDA   (FP),Y
   CLC
   LDY   #$3D
   ADC   (FP),Y
   LDY   #$3F
   STA   (FP),Y
   LDY   #$34
   LDA   (FP),Y
   LDY   #$3E
   ADC   (FP),Y
   LDY   #$40
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$41
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$42
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$FF
   LDY   #$43
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$44
   STA   (FP),Y
   LDY   #$3F
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
   LDY   #$43
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
   LDY   #$45
   LDA   (FP),Y
   LDY   #$0B
   STA   (FP),Y
   LDY   #$46
   LDA   (FP),Y
   LDY   #$0C
   STA   (FP),Y
   LDY   #$47
   LDA   (FP),Y
   LDY   #$0D
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$05
   LDY   #$49
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   ORA   #$00
   BPL   .sx_done@46
   LDA   #$FF
.sx_done@46:
   LDY   #$4B
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
   LDY   #$49
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
   LDY   #$4D
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
   LDY   #$4D
   LDA   (FP),Y
   LDY   #$0B
   STA   (FP),Y
   LDY   #$4E
   LDA   (FP),Y
   LDY   #$0C
   STA   (FP),Y
   LDY   #$4F
   LDA   (FP),Y
   LDY   #$0D
   STA   (FP),Y
   LDY   #$50
   LDA   (FP),Y
   LDY   #$0E
   STA   (FP),Y
   LDY   #$4D
   LDA   (FP),Y
   LDY   #$51
   STA   (FP),Y
   LDY   #$4E
   LDA   (FP),Y
   LDY   #$52
   STA   (FP),Y
   LDY   #$4F
   LDA   (FP),Y
   LDY   #$53
   STA   (FP),Y
   LDY   #$50
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
   LDY   #$32
   LDA   (FP),Y
   LDY   #$55
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDA   #$02
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$55
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
   LDA   HARGS+4
   LDY   #$59
   STA   (FP),Y
   LDA   HARGS+5
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@11
   JMP   .cond_else@4
.lb_skip@11:
   LDA   #$FF
   LDY   #$5B
   STA   (FP),Y
   INY
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
   LDA   z5
   INY
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
   LDY   #$5D
   LDA   (FP),Y
   SEC
   LDY   #$61
   SBC   (FP),Y
   LDY   #$65
   STA   (FP),Y
   LDY   #$5E
   LDA   (FP),Y
   LDY   #$62
   SBC   (FP),Y
   LDY   #$66
   STA   (FP),Y
   LDY   #$5F
   LDA   (FP),Y
   LDY   #$63
   SBC   (FP),Y
   LDY   #$67
   STA   (FP),Y
   LDY   #$60
   LDA   (FP),Y
   LDY   #$64
   SBC   (FP),Y
   LDY   #$68
   STA   (FP),Y
   LDY   #$65
   LDA   (FP),Y
   LDY   #$69
   STA   (FP),Y
   LDY   #$66
   LDA   (FP),Y
   LDY   #$6A
   STA   (FP),Y
   LDY   #$67
   LDA   (FP),Y
   LDY   #$6B
   STA   (FP),Y
   LDY   #$68
   LDA   (FP),Y
   LDY   #$6C
   STA   (FP),Y
   JMP   .cond_end@5
.cond_else@4:
   LDY   #$05
   LDA   (FP),Y
   LDY   #$6D
   STA   (FP),Y
   LDY   #$06
   LDA   (FP),Y
   LDY   #$6E
   STA   (FP),Y
   LDA   gp8
   STA   DPTR
   LDA   gp8+1
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$6F
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$6D
   LDA   (FP),Y
   STA   HARGS
   INY
   LDA   (FP),Y
   STA   HARGS+1
   LDY   #$70
   LDA   (FP),Y
   STA   HARGS+2
   INY
   LDA   (FP),Y
   STA   HARGS+3
   JSR   mul16
   LDA   HARGS+4
   LDY   #$72
   STA   (FP),Y
   LDA   HARGS+5
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$74
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$75
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@47
   LDA   #$00
   JMP   .sx_done@48
.sx_neg@47:
   LDA   #$FF
.sx_done@48:
   LDY   #$76
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$74
   LDA   (FP),Y
   LDY   #$69
   STA   (FP),Y
   LDY   #$75
   LDA   (FP),Y
   LDY   #$6A
   STA   (FP),Y
   LDY   #$76
   LDA   (FP),Y
   LDY   #$6B
   STA   (FP),Y
   INY
   STA   (FP),Y
.cond_end@5:
   LDY   #$69
   LDA   (FP),Y
   LDY   #$78
   STA   (FP),Y
   LDY   #$6A
   LDA   (FP),Y
   LDY   #$79
   STA   (FP),Y
   LDY   #$6B
   LDA   (FP),Y
   LDY   #$7A
   STA   (FP),Y
   LDY   #$6C
   LDA   (FP),Y
   LDY   #$7B
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$04
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDY   #$78
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$79
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$7A
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$7B
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   JSR   mix
   LDA   #$00
   LDY   #$7C
   STA   (FP),Y
   INY
   STA   (FP),Y
.loop@16_start:
   LDY   #$7E
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
   BVC   .cmp_novf@49
   EOR   #$80
.cmp_novf@49:
   ASL
   LDA   #$00
   ROL
   LDY   #$80
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@10
   JMP   .loop@16_break
.lb_skip@10:
   LDA   gp8
   STA   DPTR
   LDA   gp8+1
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$82
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDA   #$FF
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$83
   LDA   (FP),Y
   LDY   #$85
   CMP   (FP),Y
   BNE   .cmp_true@50
   LDA   #$00
   JMP   .cmp_end@51
.cmp_true@50:
   LDA   #$01
.cmp_end@51:
   LDY   #$87
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
   LDY   #$87
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
   LDA   HARGS+4
   LDY   #$8B
   STA   (FP),Y
   LDA   HARGS+5
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$8D
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$8E
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@52
   LDA   #$00
   JMP   .sx_done@53
.sx_neg@52:
   LDA   #$FF
.sx_done@53:
   LDY   #$8F
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
   JSR   mix
   LDY   #$32
   LDA   (FP),Y
   LDY   #$91
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$93
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$94
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@54
   LDA   #$00
   JMP   .sx_done@55
.sx_neg@54:
   LDA   #$FF
.sx_done@55:
   LDY   #$95
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   z5
   INY
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
   LDA   #$03
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   ORA   #$00
   BPL   .sx_done@57
   LDA   #$FF
.sx_done@57:
   LDY   #$9D
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$97
   LDA   (FP),Y
   SEC
   LDY   #$9B
   SBC   (FP),Y
   LDY   #$9F
   STA   (FP),Y
   LDY   #$98
   LDA   (FP),Y
   LDY   #$9C
   SBC   (FP),Y
   LDY   #$A0
   STA   (FP),Y
   LDY   #$99
   LDA   (FP),Y
   LDY   #$9D
   SBC   (FP),Y
   LDY   #$A1
   STA   (FP),Y
   LDY   #$9A
   LDA   (FP),Y
   LDY   #$9E
   SBC   (FP),Y
   LDY   #$A2
   STA   (FP),Y
   LDY   #$9F
   LDA   (FP),Y
   STA   z5
   INY
   LDA   (FP),Y
   STA   z5+1
   INY
   LDA   (FP),Y
   STA   z5+2
   INY
   LDA   (FP),Y
   STA   z5+3
   LDY   #$93
   LDA   (FP),Y
   CLC
   LDY   #$9F
   ADC   (FP),Y
   LDY   #$A3
   STA   (FP),Y
   LDY   #$94
   LDA   (FP),Y
   LDY   #$A0
   ADC   (FP),Y
   LDY   #$A4
   STA   (FP),Y
   LDY   #$95
   LDA   (FP),Y
   LDY   #$A1
   ADC   (FP),Y
   LDY   #$A5
   STA   (FP),Y
   LDY   #$96
   LDA   (FP),Y
   LDY   #$A2
   ADC   (FP),Y
   LDY   #$A6
   STA   (FP),Y
   LDY   #$A3
   LDA   (FP),Y
   LDY   #$A7
   STA   (FP),Y
   LDY   #$A4
   LDA   (FP),Y
   LDY   #$A8
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   g3
   INY
   LDA   (FP),Y
   STA   g3+1
   LDY   #$32
   LDA   (FP),Y
   LDY   #$A9
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$AB
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$AC
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
   ADC   #$02
   LDY   #$AF
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$B0
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$B1
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$B2
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   SEC
   SBC   #$01
   LDY   #$B3
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   SBC   #$00
   LDY   #$B4
   STA   (FP),Y
   LDY   #$AF
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$B3
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$B4
   LDA   (FP),Y
   LDY   #$01
   STA   (DPTR),Y
   LDY   #$AB
   LDA   (FP),Y
   LDY   #$B1
   AND   (FP),Y
   LDY   #$B5
   STA   (FP),Y
   LDY   #$AC
   LDA   (FP),Y
   LDY   #$B2
   AND   (FP),Y
   LDY   #$B6
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$B7
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$B8
   STA   (FP),Y
   LDA   #$07
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$B7
   LDA   (FP),Y
   LDY   #$B9
   AND   (FP),Y
   LDY   #$BB
   STA   (FP),Y
   LDY   #$B8
   LDA   (FP),Y
   LDY   #$BA
   AND   (FP),Y
   LDY   #$BC
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   BEQ   .cmp_true@58
   LDA   #$00
   JMP   .cmp_end@59
.cmp_true@58:
   LDA   #$01
.cmp_end@59:
   LDY   #$BD
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .case@18
   LDY   #$BB
   LDA   (FP),Y
   CMP   #$03
   BEQ   .cmp_true@60
   LDA   #$00
   JMP   .cmp_end@61
.cmp_true@60:
   LDA   #$01
.cmp_end@61:
   LDY   #$BF
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .case@19
   LDY   #$BB
   LDA   (FP),Y
   CMP   #$06
   BEQ   .cmp_true@62
   LDA   #$00
   JMP   .cmp_end@63
.cmp_true@62:
   LDA   #$01
.cmp_end@63:
   LDY   #$C1
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@9
   JMP   .case@20
.lb_skip@9:
   JMP   .default@21
.case@18:
.case@19:
   LDA   #$03
   LDY   #$C3
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$C5
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$C6
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@64
   LDA   #$00
   JMP   .sx_done@65
.sx_neg@64:
   LDA   #$FF
.sx_done@65:
   LDY   #$C7
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
   LDY   #$C5
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
   JSR   sdivmod32
   LDA   HARGS+8
   LDY   #$C9
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
   LDY   #$02
   LDA   (FP),Y
   LDY   #$CD
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@66
   LDA   #$00
   JMP   .sx_done@67
.sx_neg@66:
   LDA   #$FF
.sx_done@67:
   LDY   #$CE
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$CF
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$D0
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@68
   LDA   #$00
   JMP   .sx_done@69
.sx_neg@68:
   LDA   #$FF
.sx_done@69:
   LDY   #$D1
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$C9
   LDA   (FP),Y
   SEC
   LDY   #$CF
   SBC   (FP),Y
   LDY   #$D3
   STA   (FP),Y
   LDY   #$CA
   LDA   (FP),Y
   LDY   #$D0
   SBC   (FP),Y
   LDY   #$D4
   STA   (FP),Y
   LDY   #$CB
   LDA   (FP),Y
   LDY   #$D1
   SBC   (FP),Y
   LDY   #$D5
   STA   (FP),Y
   LDY   #$CC
   LDA   (FP),Y
   LDY   #$D2
   SBC   (FP),Y
   LDY   #$D6
   STA   (FP),Y
   LDY   #$D3
   LDA   (FP),Y
   LDY   #$D7
   STA   (FP),Y
   LDY   #$D4
   LDA   (FP),Y
   LDY   #$D8
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   z4
   INY
   LDA   (FP),Y
   STA   z4+1
.case@20:
.default@21:
   LDY   #$7D
   LDA   (FP),Y
   LDY   #$D9
   STA   (FP),Y
   CLC
   ADC   #$01
   INY
   STA   (FP),Y
   LDY   #$7D
   STA   (FP),Y
   JMP   .loop@16_start
.loop@16_break:
   LDY   #$32
   LDA   (FP),Y
   CLC
   ADC   #$01
   LDY   #$DB
   STA   (FP),Y
   LDY   #$32
   STA   (FP),Y
   LDY   #$DC
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
   BVC   .cmp_novf@70
   EOR   #$80
.cmp_novf@70:
   ASL
   LDA   #$00
   ROL
   LDY   #$DE
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@8
   JMP   .loop@15_start
.lb_skip@8:
   JSR   fin0
   JSR   fin1
   JSR   fin2
   LDA   acc
   LDY   #$E0
   STA   (FP),Y
   LDA   acc+1
   AND   #$7F
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$E2
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$E3
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
   ADC   #$E5
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$E4
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
   DS.B  1

g3:
   DC.W  $0002

ga6:
   DC.W  $FFFF
   DC.W  $00FF
   DC.W  $0002
   DC.W  $0080

gs7:
   DS.B  2
   DC.W  $000B

gp8:
   DC.W  g1

@3.s15:
   DC.W  $FFFF

__zp_init:
   SUBROUTINE

   LDA   #$00
   STA   z4+1
   STA   z5+1
   STA   z5+2
   STA   z5+3
   LDA   #$7F
   STA   z4
   LDA   #$80
   STA   z5
   RTS
