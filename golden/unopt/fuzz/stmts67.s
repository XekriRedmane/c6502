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
   LDA   g1+1
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

   LDA   z5
   LDY   #$01
   STA   (FP),Y
   LDA   z5+1
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
   LDA   ga6
   LDY   #$05
   STA   (FP),Y
   LDA   ga6+1
   INY
   STA   (FP),Y
   LDA   ga6+2
   INY
   STA   (FP),Y
   LDA   ga6+3
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
   LDA   ga6+4
   LDY   #$09
   STA   (FP),Y
   LDA   ga6+5
   INY
   STA   (FP),Y
   LDA   ga6+6
   INY
   STA   (FP),Y
   LDA   ga6+7
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
   LDA   ga6+8
   LDY   #$0D
   STA   (FP),Y
   LDA   ga6+9
   INY
   STA   (FP),Y
   LDA   ga6+10
   INY
   STA   (FP),Y
   LDA   ga6+11
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

fin2:
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

   LDA   ga6+12
   LDY   #$01
   STA   (FP),Y
   LDA   ga6+13
   INY
   STA   (FP),Y
   LDA   ga6+14
   INY
   STA   (FP),Y
   LDA   ga6+15
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
   LDA   #<gs7
   LDY   #$05
   STA   (FP),Y
   LDA   #>gs7
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$01
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
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$0A
   STA   (FP),Y
   LDY   #$02
   LDA   (DPTR),Y
   LDY   #$0B
   STA   (FP),Y
   LDY   #$03
   LDA   (DPTR),Y
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
   LDA   #<gs7
   LDY   #$11
   STA   (FP),Y
   LDA   #>gs7
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$05
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
   LDY   #$01
   LDA   (DPTR),Y
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

f10:
   SUBROUTINE

   ; prologue: 8 arg bytes, 175 local bytes
   SEC
   LDA   SSP
   SBC   #$B1
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$B0
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
   LDA   #$01
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
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$0C
   LDA   (FP),Y
   LDY   #$10
   STA   (FP),Y
   LDY   #$0D
   LDA   (FP),Y
   LDY   #$11
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$09
   STA   (FP),Y
   LDY   #$11
   LDA   (FP),Y
   LDY   #$0A
   STA   (FP),Y
   LDA   #$00
   LDY   #$12
   STA   (FP),Y
   INY
   STA   (FP),Y
.loop@0_start:
   LDA   #$00
   LDY   #$14
   STA   (FP),Y
   INY
   STA   (FP),Y
.loop@1_start:
   LDY   #$13
   LDA   (FP),Y
   LDY   #$16
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDA   #$02
   INY
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
   LDY   #$1A
   STA   (FP),Y
   LDY   #$16
   LDA   (FP),Y
   SEC
   LDY   #$19
   SBC   (FP),Y
   LDY   #$17
   LDA   (FP),Y
   LDY   #$1A
   SBC   (FP),Y
   BVC   .cmp_novf@8
   EOR   #$80
.cmp_novf@8:
   ASL
   LDA   #$00
   ROL
   LDY   #$1B
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$1D
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$1E
   STA   (FP),Y
   LDA   gp8
   STA   DPTR
   LDA   gp8+1
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
   CLC
   LDY   #$09
   ADC   (FP),Y
   LDY   #$21
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$0A
   ADC   (FP),Y
   LDY   #$22
   STA   (FP),Y
   LDY   #$1D
   LDA   (FP),Y
   SEC
   LDY   #$21
   SBC   (FP),Y
   LDY   #$23
   STA   (FP),Y
   LDY   #$1E
   LDA   (FP),Y
   LDY   #$22
   SBC   (FP),Y
   LDY   #$24
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$25
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$26
   STA   (FP),Y
   LDA   #$07
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$25
   LDA   (FP),Y
   LDY   #$27
   AND   (FP),Y
   LDY   #$29
   STA   (FP),Y
   LDY   #$26
   LDA   (FP),Y
   LDY   #$28
   AND   (FP),Y
   LDY   #$2A
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   BEQ   .cmp_true@9
   LDA   #$00
   JMP   .cmp_end@10
.cmp_true@9:
   LDA   #$01
.cmp_end@10:
   LDY   #$2B
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@3
   JMP   .switch@2_break
.lb_skip@3:
   LDY   #$29
   LDA   (FP),Y
   CMP   #$02
   BEQ   .cmp_true@11
   LDA   #$00
   JMP   .cmp_end@12
.cmp_true@11:
   LDA   #$01
.cmp_end@12:
   LDY   #$2D
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .case@4
   LDY   #$29
   LDA   (FP),Y
   CMP   #$05
   BEQ   .cmp_true@13
   LDA   #$00
   JMP   .cmp_end@14
.cmp_true@13:
   LDA   #$01
.cmp_end@14:
   LDY   #$2F
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@2
   JMP   .case@5
.lb_skip@2:
   JMP   .default@6
.case@4:
   LDY   #$09
   LDA   (FP),Y
   SEC
   SBC   #$01
   LDY   #$31
   STA   (FP),Y
   LDY   #$0A
   LDA   (FP),Y
   SBC   #$00
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
   LDY   #$31
   LDA   (FP),Y
   LDY   #$33
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$34
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
   LDY   #$B6
   LDA   (FP),Y
   LDY   #$3B
   STA   (FP),Y
   LDY   #$B7
   LDA   (FP),Y
   LDY   #$3C
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@15
   LDA   #$00
   JMP   .sx_done@16
.sx_neg@15:
   LDA   #$FF
.sx_done@16:
   LDY   #$3D
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
   CLC
   LDY   #$3B
   ADC   (FP),Y
   LDY   #$43
   STA   (FP),Y
   LDA   g2+1
   LDY   #$3C
   ADC   (FP),Y
   LDY   #$44
   STA   (FP),Y
   LDA   g2+2
   LDY   #$3D
   ADC   (FP),Y
   LDY   #$45
   STA   (FP),Y
   LDA   g2+3
   LDY   #$3E
   ADC   (FP),Y
   LDY   #$46
   STA   (FP),Y
   LDA   g2+4
   LDY   #$3F
   ADC   (FP),Y
   LDY   #$47
   STA   (FP),Y
   LDA   g2+5
   LDY   #$40
   ADC   (FP),Y
   LDY   #$48
   STA   (FP),Y
   LDA   g2+6
   LDY   #$41
   ADC   (FP),Y
   LDY   #$49
   STA   (FP),Y
   LDA   g2+7
   LDY   #$42
   ADC   (FP),Y
   LDY   #$4A
   STA   (FP),Y
   LDY   #$33
   LDA   (FP),Y
   SEC
   LDY   #$43
   SBC   (FP),Y
   LDY   #$4B
   STA   (FP),Y
   LDY   #$34
   LDA   (FP),Y
   LDY   #$44
   SBC   (FP),Y
   LDY   #$4C
   STA   (FP),Y
   LDY   #$35
   LDA   (FP),Y
   LDY   #$45
   SBC   (FP),Y
   LDY   #$4D
   STA   (FP),Y
   LDY   #$36
   LDA   (FP),Y
   LDY   #$46
   SBC   (FP),Y
   LDY   #$4E
   STA   (FP),Y
   LDY   #$37
   LDA   (FP),Y
   LDY   #$47
   SBC   (FP),Y
   LDY   #$4F
   STA   (FP),Y
   LDY   #$38
   LDA   (FP),Y
   LDY   #$48
   SBC   (FP),Y
   LDY   #$50
   STA   (FP),Y
   LDY   #$39
   LDA   (FP),Y
   LDY   #$49
   SBC   (FP),Y
   LDY   #$51
   STA   (FP),Y
   LDY   #$3A
   LDA   (FP),Y
   LDY   #$4A
   SBC   (FP),Y
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
   LDY   #$4D
   LDA   (FP),Y
   LDY   #$55
   STA   (FP),Y
   LDY   #$4E
   LDA   (FP),Y
   LDY   #$56
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
   LDY   #$55
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$56
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   JSR   mix
   LDA   #<gs7
   LDY   #$57
   STA   (FP),Y
   LDA   #>gs7
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
   LDA   #$FF
   LDY   #$5D
   STA   (FP),Y
   DEY
   LDA   (FP),Y
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
.case@5:
   LDY   #$15
   LDA   (FP),Y
   LDY   #$5F
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
   LDY   #$5F
   LDA   (FP),Y
   STA   z4
   INY
   LDA   (FP),Y
   STA   z4+1
   INY
   LDA   (FP),Y
   STA   z4+2
   INY
   LDA   (FP),Y
   STA   z4+3
   INY
   LDA   (FP),Y
   STA   z4+4
   INY
   LDA   (FP),Y
   STA   z4+5
   INY
   LDA   (FP),Y
   STA   z4+6
   INY
   LDA   (FP),Y
   STA   z4+7
   LDA   gp9
   STA   DPTR
   LDA   gp9+1
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$67
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$68
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   @4.s17
   LDY   #$69
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   @4.s17+1
   LDY   #$6A
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$6B
   STA   (FP),Y

   ; epilogue
   PHA
   CLC
   LDA   FP
   ADC   #$B9
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$B0
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   PLA
   RTS
.default@6:
   LDA   gp8
   STA   DPTR
   LDA   gp8+1
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$6C
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$6D
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   HARGS
   INY
   LDA   (FP),Y
   STA   HARGS+1
   LDA   #$5A
   STA   HARGS+2
   LDA   #$00
   STA   HARGS+3
   JSR   mul16
   LDA   HARGS+4
   LDY   #$6E
   STA   (FP),Y
   LDA   HARGS+5
   INY
   STA   (FP),Y
   LDA   gp8
   STA   DPTR
   LDA   gp8+1
   STA   DPTR+1
   DEY
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$6F
   LDA   (FP),Y
   LDY   #$01
   STA   (DPTR),Y
   LDY   #$6E
   LDA   (FP),Y
   LDY   #$70
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$71
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$B2
   LDA   (FP),Y
   LDY   #$70
   ORA   (FP),Y
   LDY   #$74
   STA   (FP),Y
   LDY   #$B3
   LDA   (FP),Y
   LDY   #$71
   ORA   (FP),Y
   LDY   #$75
   STA   (FP),Y
   LDY   #$B4
   LDA   (FP),Y
   LDY   #$72
   ORA   (FP),Y
   LDY   #$76
   STA   (FP),Y
   LDY   #$B5
   LDA   (FP),Y
   LDY   #$73
   ORA   (FP),Y
   LDY   #$77
   STA   (FP),Y
   LDY   #$74
   LDA   (FP),Y
   LDY   #$78
   STA   (FP),Y

   ; epilogue
   PHA
   CLC
   LDA   FP
   ADC   #$B9
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$B0
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   PLA
   RTS
.switch@2_break:
   LDY   #$13
   LDA   (FP),Y
   LDY   #$79
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$7B
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$7C
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$13
   LDA   (FP),Y
   LDY   #$7F
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
   LDY   #$7F
   LDA   (FP),Y
   LDY   #$81
   AND   (FP),Y
   LDY   #$83
   STA   (FP),Y
   LDY   #$80
   LDA   (FP),Y
   LDY   #$82
   AND   (FP),Y
   LDY   #$84
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$85
   STA   (FP),Y
   STA   HARGS
   LDA   #$04
   STA   HARGS+1
   JSR   mul8
   LDA   HARGS+2
   LDY   #$86
   STA   (FP),Y
   TAX
   LDY   #$7B
   LDA   (FP),Y
   STA   ga6,X
   INY
   LDA   (FP),Y
   STA   ga6+1,X
   INY
   LDA   (FP),Y
   STA   ga6+2,X
   INY
   LDA   (FP),Y
   STA   ga6+3,X
   LDA   gp8
   STA   DPTR
   LDA   gp8+1
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$87
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$88
   STA   (FP),Y
   LDA   #$02
   INY
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
   LDY   #$87
   LDA   (FP),Y
   STA   HARGS
   INY
   LDA   (FP),Y
   STA   HARGS+1
   LDY   #$8B
   LDA   (FP),Y
   STA   HARGS+2
   INY
   LDA   (FP),Y
   STA   HARGS+3
   JSR   udivmod16
   LDA   HARGS+6
   LDY   #$8D
   STA   (FP),Y
   LDA   HARGS+7
   INY
   STA   (FP),Y
   LDA   g3
   INY
   STA   (FP),Y
   LDA   g3+1
   INY
   STA   (FP),Y
   LDA   g3+1
   BMI   .sx_neg@17
   LDA   #$00
   JMP   .sx_done@18
.sx_neg@17:
   LDA   #$FF
.sx_done@18:
   LDY   #$91
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
   LDY   #$8F
   LDA   (FP),Y
   CLC
   SBC   z4
   INY
   LDA   (FP),Y
   SBC   z4+1
   INY
   LDA   (FP),Y
   SBC   z4+2
   INY
   LDA   (FP),Y
   SBC   z4+3
   INY
   LDA   (FP),Y
   SBC   z4+4
   INY
   LDA   (FP),Y
   SBC   z4+5
   INY
   LDA   (FP),Y
   SBC   z4+6
   INY
   LDA   (FP),Y
   SBC   z4+7
   LDA   #$00
   ROL
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$99
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$9A
   STA   (FP),Y
   LDY   #$8E
   LDA   (FP),Y
   LDY   #$9A
   CMP   (FP),Y
   BNE   .cmp_true@19
   LDY   #$8D
   LDA   (FP),Y
   LDY   #$99
   CMP   (FP),Y
   BNE   .cmp_true@19
   LDA   #$00
   JMP   .cmp_end@20
.cmp_true@19:
   LDA   #$01
.cmp_end@20:
   LDY   #$9B
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   @4.s17
   INY
   LDA   (FP),Y
   STA   @4.s17+1
   LDY   #$15
   LDA   (FP),Y
   CLC
   ADC   #$01
   LDY   #$9D
   STA   (FP),Y
   LDY   #$15
   STA   (FP),Y
   LDY   #$9E
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
   BVC   .cmp_novf@22
   EOR   #$80
.cmp_novf@22:
   ASL
   LDA   #$00
   ROL
   LDY   #$A0
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@1
   JMP   .loop@1_start
.lb_skip@1:
   LDY   #$03
   LDA   (FP),Y
   LDY   #$A2
   STA   (FP),Y
   LDY   #$04
   LDA   (FP),Y
   LDY   #$A3
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   SEC
   SBC   #$01
   LDY   #$A4
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   SBC   #$00
   LDY   #$A5
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$03
   STA   (FP),Y
   LDY   #$A5
   LDA   (FP),Y
   LDY   #$04
   STA   (FP),Y
   LDY   #$13
   LDA   (FP),Y
   CLC
   ADC   #$01
   LDY   #$A6
   STA   (FP),Y
   LDY   #$13
   STA   (FP),Y
   LDY   #$A7
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
   BVC   .cmp_novf@23
   EOR   #$80
.cmp_novf@23:
   ASL
   LDA   #$00
   ROL
   LDY   #$A9
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@0
   JMP   .loop@0_start
.lb_skip@0:
   LDY   #$03
   LDA   (FP),Y
   LDY   #$AB
   STA   (FP),Y
   LDY   #$04
   LDA   (FP),Y
   LDY   #$AC
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LSR
   LSR
   LDY   #$AD
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ASL
   ASL
   ASL
   ASL
   ASL
   ASL
   INY
   ORA   (FP),Y
   STA   (FP),Y
   LDY   #$AF
   STA   (FP),Y

   ; epilogue
   PHA
   CLC
   LDA   FP
   ADC   #$B9
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

f20:
   SUBROUTINE

   ; prologue: 5 arg bytes, 161 local bytes
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

   LDA   #$FF
   LDY   #$01
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   g1
   INY
   STA   (FP),Y
   LDA   g1+1
   INY
   STA   (FP),Y
   LDA   g1
   SEC
   SBC   #$01
   INY
   STA   (FP),Y
   LDA   g1+1
   SBC   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   g1
   INY
   LDA   (FP),Y
   STA   g1+1
   LDA   #$07
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$03
   LDA   (FP),Y
   LDY   #$07
   AND   (FP),Y
   LDY   #$09
   STA   (FP),Y
   LDY   #$04
   LDA   (FP),Y
   LDY   #$08
   AND   (FP),Y
   LDY   #$0A
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   BEQ   .cmp_true@24
   LDA   #$00
   JMP   .cmp_end@25
.cmp_true@24:
   LDA   #$01
.cmp_end@25:
   LDY   #$0B
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .case@8
   LDY   #$09
   LDA   (FP),Y
   CMP   #$04
   BEQ   .cmp_true@26
   LDA   #$00
   JMP   .cmp_end@27
.cmp_true@26:
   LDA   #$01
.cmp_end@27:
   LDY   #$0D
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@6
   JMP   .case@9
.lb_skip@6:
   LDY   #$09
   LDA   (FP),Y
   CMP   #$05
   BEQ   .cmp_true@28
   LDA   #$00
   JMP   .cmp_end@29
.cmp_true@28:
   LDA   #$01
.cmp_end@29:
   LDY   #$0F
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@5
   JMP   .case@10
.lb_skip@5:
   LDY   #$09
   LDA   (FP),Y
   CMP   #$07
   BEQ   .cmp_true@30
   LDA   #$00
   JMP   .cmp_end@31
.cmp_true@30:
   LDA   #$01
.cmp_end@31:
   LDY   #$11
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@4
   JMP   .case@11
.lb_skip@4:
   JMP   .default@12
.case@8:
   LDA   gp8
   STA   DPTR
   LDA   gp8+1
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$13
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$14
   STA   (FP),Y
   LSR
   DEY
   LDA   (FP),Y
   ROR
   LDY   #$16
   STA   (FP),Y
   LDA   #$00
   ROR
   DEY
   STA   (FP),Y
   LDA   gp8
   STA   DPTR
   LDA   gp8+1
   STA   DPTR+1
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$16
   LDA   (FP),Y
   LDY   #$01
   STA   (DPTR),Y
   INY
   LDA   (FP),Y
   LDY   #$17
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
   LDY   #$17
   LDA   (FP),Y
   STA   z4
   INY
   LDA   (FP),Y
   STA   z4+1
   INY
   LDA   (FP),Y
   STA   z4+2
   INY
   LDA   (FP),Y
   STA   z4+3
   INY
   LDA   (FP),Y
   STA   z4+4
   INY
   LDA   (FP),Y
   STA   z4+5
   INY
   LDA   (FP),Y
   STA   z4+6
   INY
   LDA   (FP),Y
   STA   z4+7
   JMP   .switch@7_break
.case@9:
.case@10:
   LDA   #<gs7
   LDY   #$1F
   STA   (FP),Y
   LDA   #>gs7
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$05
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
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$23
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$24
   STA   (FP),Y
   LDY   #$A4
   LDA   (FP),Y
   SEC
   LDY   #$23
   SBC   (FP),Y
   LDY   #$25
   STA   (FP),Y
   LDY   #$A5
   LDA   (FP),Y
   LDY   #$24
   SBC   (FP),Y
   LDY   #$26
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$27
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDA   gp8
   STA   DPTR
   LDA   gp8+1
   STA   DPTR+1
   DEY
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$29
   LDA   (FP),Y
   LDY   #$01
   STA   (DPTR),Y
   JMP   .switch@7_break
.case@11:
   LDY   #$02
   LDA   (FP),Y
   LDY   #$2A
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
   LDA   g3+1
   BMI   .sx_neg@32
   LDA   #$00
   JMP   .sx_done@33
.sx_neg@32:
   LDA   #$FF
.sx_done@33:
   LDY   #$30
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   ga6+8
   INY
   STA   (FP),Y
   LDA   ga6+9
   INY
   STA   (FP),Y
   LDA   ga6+10
   INY
   STA   (FP),Y
   LDA   ga6+11
   INY
   STA   (FP),Y
   LDY   #$2E
   LDA   (FP),Y
   CLC
   LDY   #$32
   ADC   (FP),Y
   LDY   #$36
   STA   (FP),Y
   LDY   #$2F
   LDA   (FP),Y
   LDY   #$33
   ADC   (FP),Y
   LDY   #$37
   STA   (FP),Y
   LDY   #$30
   LDA   (FP),Y
   LDY   #$34
   ADC   (FP),Y
   LDY   #$38
   STA   (FP),Y
   LDY   #$31
   LDA   (FP),Y
   LDY   #$35
   ADC   (FP),Y
   LDY   #$39
   STA   (FP),Y
   LDY   #$36
   LDA   (FP),Y
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
   LDY   #$39
   LDA   (FP),Y
   LDY   #$3D
   STA   (FP),Y
   LDA   z5
   INY
   STA   (FP),Y
   LDA   z5+1
   INY
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
   ADC   #$05
   LDY   #$42
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$43
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$44
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$45
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$46
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$47
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@34
   LDA   #$00
   JMP   .sx_done@35
.sx_neg@34:
   LDA   #$FF
.sx_done@35:
   LDY   #$48
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   ga6+12
   INY
   STA   (FP),Y
   LDA   ga6+13
   INY
   STA   (FP),Y
   LDA   ga6+14
   INY
   STA   (FP),Y
   LDA   ga6+15
   INY
   STA   (FP),Y
   LDY   #$46
   LDA   (FP),Y
   LDY   #$4A
   AND   (FP),Y
   LDY   #$4E
   STA   (FP),Y
   LDY   #$47
   LDA   (FP),Y
   LDY   #$4B
   AND   (FP),Y
   LDY   #$4F
   STA   (FP),Y
   LDY   #$48
   LDA   (FP),Y
   LDY   #$4C
   AND   (FP),Y
   LDY   #$50
   STA   (FP),Y
   LDY   #$49
   LDA   (FP),Y
   LDY   #$4D
   AND   (FP),Y
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
   SEC
   LDA   SSP
   SBC   #$08
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
   LDY   #$3E
   LDA   (FP),Y
   LDY   #$05
   STA   (SSP),Y
   LDY   #$3F
   LDA   (FP),Y
   LDY   #$06
   STA   (SSP),Y
   LDY   #$52
   LDA   (FP),Y
   LDY   #$07
   STA   (SSP),Y
   LDY   #$53
   LDA   (FP),Y
   LDY   #$08
   STA   (SSP),Y
   JSR   f10
   LDY   #$54
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$02
   STA   (FP),Y
   JMP   .switch@7_break
.default@12:
   LDA   #$81
   LDY   #$56
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   ORA   #$00
   BPL   .sx_done@37
   LDA   #$FF
.sx_done@37:
   LDY   #$58
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   @13.s25
   CLC
   LDY   #$56
   ADC   (FP),Y
   LDY   #$5A
   STA   (FP),Y
   LDA   @13.s25+1
   LDY   #$57
   ADC   (FP),Y
   LDY   #$5B
   STA   (FP),Y
   LDA   @13.s25+2
   LDY   #$58
   ADC   (FP),Y
   LDY   #$5C
   STA   (FP),Y
   LDA   @13.s25+3
   LDY   #$59
   ADC   (FP),Y
   LDY   #$5D
   STA   (FP),Y
   LDY   #$5A
   LDA   (FP),Y
   STA   @13.s25
   INY
   LDA   (FP),Y
   STA   @13.s25+1
   INY
   LDA   (FP),Y
   STA   @13.s25+2
   INY
   LDA   (FP),Y
   STA   @13.s25+3
   LDY   #$5A
   LDA   (FP),Y
   LDY   #$5E
   STA   (FP),Y
   LDY   #$5B
   LDA   (FP),Y
   LDY   #$5F
   STA   (FP),Y
   LDY   #$5C
   LDA   (FP),Y
   LDY   #$60
   STA   (FP),Y
   LDY   #$5D
   LDA   (FP),Y
   LDY   #$61
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
   JSR   mix
   LDA   gp8
   STA   DPTR
   LDA   gp8+1
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$62
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$63
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   SEC
   SBC   #$01
   LDY   #$64
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   SBC   #$00
   LDY   #$65
   STA   (FP),Y
   LDA   gp8
   STA   DPTR
   LDA   gp8+1
   STA   DPTR+1
   DEY
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$65
   LDA   (FP),Y
   LDY   #$01
   STA   (DPTR),Y
.switch@7_break:
   LDA   gp8
   STA   DPTR
   LDA   gp8+1
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$66
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$67
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ORA   #$05
   LDY   #$68
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$69
   STA   (FP),Y
   LDA   gp8
   STA   DPTR
   LDA   gp8+1
   STA   DPTR+1
   DEY
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$69
   LDA   (FP),Y
   LDY   #$01
   STA   (DPTR),Y
   LDA   #$00
   LDY   #$6A
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@38
   LDA   #$00
   JMP   .sx_done@39
.sx_neg@38:
   LDA   #$FF
.sx_done@39:
   LDY   #$6F
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   @13.s25
   STA   HARGS
   LDA   @13.s25+1
   STA   HARGS+1
   LDA   @13.s25+2
   STA   HARGS+2
   LDA   @13.s25+3
   STA   HARGS+3
   LDY   #$6D
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
   LDY   #$71
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
   LDA   g3
   INY
   STA   (FP),Y
   ASL
   STA   (FP),Y
   LDA   g3+1
   INY
   STA   (FP),Y
   ROL
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$77
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$78
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@40
   LDA   #$00
   JMP   .sx_done@41
.sx_neg@40:
   LDA   #$FF
.sx_done@41:
   LDY   #$79
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$71
   LDA   (FP),Y
   SEC
   LDY   #$77
   SBC   (FP),Y
   LDY   #$7B
   STA   (FP),Y
   LDY   #$72
   LDA   (FP),Y
   LDY   #$78
   SBC   (FP),Y
   LDY   #$7C
   STA   (FP),Y
   LDY   #$73
   LDA   (FP),Y
   LDY   #$79
   SBC   (FP),Y
   LDY   #$7D
   STA   (FP),Y
   LDY   #$74
   LDA   (FP),Y
   LDY   #$7A
   SBC   (FP),Y
   LDY   #$7E
   STA   (FP),Y
   LDY   #$7B
   LDA   (FP),Y
   LDY   #$7F
   STA   (FP),Y
   LDY   #$7C
   LDA   (FP),Y
   LDY   #$80
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   g3
   INY
   LDA   (FP),Y
   STA   g3+1
   LDA   g2
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
   LDA   #<gs7
   INY
   STA   (FP),Y
   LDA   #>gs7
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$01
   LDY   #$88
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$89
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$8A
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$8B
   STA   (FP),Y
   LDY   #$02
   LDA   (DPTR),Y
   LDY   #$8C
   STA   (FP),Y
   LDY   #$03
   LDA   (DPTR),Y
   LDY   #$8D
   STA   (FP),Y
   LSR
   DEY
   LDA   (FP),Y
   ROR
   LDY   #$91
   STA   (FP),Y
   LDY   #$8B
   LDA   (FP),Y
   ROR
   LDY   #$90
   STA   (FP),Y
   LDY   #$8A
   LDA   (FP),Y
   ROR
   LDY   #$8F
   STA   (FP),Y
   LDA   #$00
   ROR
   DEY
   STA   (FP),Y
   LDY   #$92
   STA   (FP),Y
   LDY   #$8F
   LDA   (FP),Y
   LDY   #$93
   STA   (FP),Y
   LDY   #$A6
   LDA   (FP),Y
   LDY   #$94
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@42
   LDA   #$00
   JMP   .sx_done@43
.sx_neg@42:
   LDA   #$FF
.sx_done@43:
   LDY   #$95
   STA   (FP),Y
   LDY   #$97
   STA   (FP),Y
   ASL
   LDA   (FP),Y
   ROR
   STA   (FP),Y
   LDY   #$94
   LDA   (FP),Y
   LDY   #$96
   STA   (FP),Y
   ROR
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$08
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDY   #$82
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$83
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$84
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$85
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   LDY   #$92
   LDA   (FP),Y
   LDY   #$05
   STA   (SSP),Y
   LDY   #$93
   LDA   (FP),Y
   LDY   #$06
   STA   (SSP),Y
   LDY   #$96
   LDA   (FP),Y
   LDY   #$07
   STA   (SSP),Y
   LDY   #$97
   LDA   (FP),Y
   LDY   #$08
   STA   (SSP),Y
   JSR   f10
   LDY   #$98
   STA   (FP),Y
   LDA   gp8
   STA   DPTR
   LDA   gp8+1
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$99
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$9A
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$01
   LDY   #$9B
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$9C
   STA   (FP),Y
   LDA   gp8
   STA   DPTR
   LDA   gp8+1
   STA   DPTR+1
   DEY
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$9C
   LDA   (FP),Y
   LDY   #$01
   STA   (DPTR),Y
   LDA   ga6+12
   LDY   #$9D
   STA   (FP),Y
   LDA   ga6+13
   INY
   STA   (FP),Y
   LDA   ga6+14
   INY
   STA   (FP),Y
   LDA   ga6+15
   INY
   STA   (FP),Y
   LDY   #$9D
   LDA   (FP),Y
   LDY   #$A1
   STA   (FP),Y

   ; epilogue
   PHA
   CLC
   LDA   FP
   ADC   #$A8
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

   ; prologue: 8 arg bytes, 166 local bytes
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
   LDY   #$AF
   LDA   (FP),Y
   CLC
   ADC   #$FF
   LDY   #$05
   STA   (FP),Y
   LDY   #$B0
   LDA   (FP),Y
   ADC   #$00
   LDY   #$06
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$AF
   STA   (FP),Y
   LDY   #$06
   LDA   (FP),Y
   LDY   #$B0
   STA   (FP),Y
   LDA   g1
   LDY   #$07
   STA   (FP),Y
   LDA   g1+1
   INY
   STA   (FP),Y
   LDY   #$05
   LDA   (FP),Y
   LDY   #$07
   AND   (FP),Y
   LDY   #$09
   STA   (FP),Y
   LDY   #$06
   LDA   (FP),Y
   LDY   #$08
   AND   (FP),Y
   LDY   #$0A
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$0B
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$0C
   STA   (FP),Y
   LDA   #$07
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$0B
   LDA   (FP),Y
   LDY   #$0D
   AND   (FP),Y
   LDY   #$0F
   STA   (FP),Y
   LDY   #$0C
   LDA   (FP),Y
   LDY   #$0E
   AND   (FP),Y
   LDY   #$10
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CMP   #$03
   BEQ   .cmp_true@44
   LDA   #$00
   JMP   .cmp_end@45
.cmp_true@44:
   LDA   #$01
.cmp_end@45:
   LDY   #$11
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .case@14
   LDY   #$0F
   LDA   (FP),Y
   CMP   #$04
   BEQ   .cmp_true@46
   LDA   #$00
   JMP   .cmp_end@47
.cmp_true@46:
   LDA   #$01
.cmp_end@47:
   LDY   #$13
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .case@15
   LDY   #$0F
   LDA   (FP),Y
   CMP   #$06
   BEQ   .cmp_true@48
   LDA   #$00
   JMP   .cmp_end@49
.cmp_true@48:
   LDA   #$01
.cmp_end@49:
   LDY   #$15
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@9
   JMP   .case@16
.lb_skip@9:
   LDY   #$0F
   LDA   (FP),Y
   CMP   #$07
   BEQ   .cmp_true@50
   LDA   #$00
   JMP   .cmp_end@51
.cmp_true@50:
   LDA   #$01
.cmp_end@51:
   LDY   #$17
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@8
   JMP   .case@18
.lb_skip@8:
   JMP   .switch@13_break
.case@14:
.case@15:
   LDA   g2
   SEC
   SBC   #$01
   LDY   #$19
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
   LDY   #$19
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
   LDY   #$19
   LDA   (FP),Y
   LDY   #$21
   STA   (FP),Y
   LDY   #$1A
   LDA   (FP),Y
   LDY   #$22
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   g3
   INY
   LDA   (FP),Y
   STA   g3+1
.case@16:
   LDY   #$AD
   LDA   (FP),Y
   AND   #$5A
   LDY   #$23
   STA   (FP),Y
   LDY   #$AE
   LDA   (FP),Y
   LDA   #$00
   LDY   #$24
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$AD
   STA   (FP),Y
   LDY   #$24
   LDA   (FP),Y
   LDY   #$AE
   STA   (FP),Y
   LDY   #$23
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_else@1
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   JMP   .switch@13_break
.if_else@1:
   LDY   #$AF
   LDA   (FP),Y
   STA   HARGS
   INY
   LDA   (FP),Y
   STA   HARGS+1
   LDA   #$5A
   STA   HARGS+2
   LDA   #$00
   STA   HARGS+3
   JSR   mul16
   LDA   HARGS+4
   LDY   #$2C
   STA   (FP),Y
   LDA   HARGS+5
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$AF
   STA   (FP),Y
   LDY   #$2D
   LDA   (FP),Y
   LDY   #$B0
   STA   (FP),Y
   LDA   g2
   ORA   z4
   LDY   #$2E
   STA   (FP),Y
   LDA   g2+1
   ORA   z4+1
   INY
   STA   (FP),Y
   LDA   g2+2
   ORA   z4+2
   INY
   STA   (FP),Y
   LDA   g2+3
   ORA   z4+3
   INY
   STA   (FP),Y
   LDA   g2+4
   ORA   z4+4
   INY
   STA   (FP),Y
   LDA   g2+5
   ORA   z4+5
   INY
   STA   (FP),Y
   LDA   g2+6
   ORA   z4+6
   INY
   STA   (FP),Y
   LDA   g2+7
   ORA   z4+7
   INY
   STA   (FP),Y
   LDY   #$2E
   LDA   (FP),Y
   LDY   #$36
   STA   (FP),Y
   LDY   #$2F
   LDA   (FP),Y
   LDY   #$37
   STA   (FP),Y
   LDY   #$30
   LDA   (FP),Y
   LDY   #$38
   STA   (FP),Y
   LDY   #$31
   LDA   (FP),Y
   LDY   #$39
   STA   (FP),Y
   LDA   #$01
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$3C
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$3D
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$3C
   LDA   (FP),Y
   CLC
   LDY   #$3E
   ADC   (FP),Y
   LDY   #$40
   STA   (FP),Y
   LDY   #$3D
   LDA   (FP),Y
   LDY   #$3F
   ADC   (FP),Y
   LDY   #$41
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$42
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$43
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
   ADC   #$01
   LDY   #$46
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$47
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$48
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$49
   STA   (FP),Y
   LDY   #$02
   LDA   (DPTR),Y
   LDY   #$4A
   STA   (FP),Y
   LDY   #$03
   LDA   (DPTR),Y
   LDY   #$4B
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
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$48
   LDA   (FP),Y
   SEC
   LDY   #$4E
   SBC   (FP),Y
   LDY   #$52
   STA   (FP),Y
   LDY   #$49
   LDA   (FP),Y
   LDY   #$4F
   SBC   (FP),Y
   LDY   #$53
   STA   (FP),Y
   LDY   #$4A
   LDA   (FP),Y
   LDY   #$50
   SBC   (FP),Y
   LDY   #$54
   STA   (FP),Y
   LDY   #$4B
   LDA   (FP),Y
   LDY   #$51
   SBC   (FP),Y
   LDY   #$55
   STA   (FP),Y
   LDY   #$52
   LDA   (FP),Y
   LDY   #$56
   STA   (FP),Y
   LDY   #$53
   LDA   (FP),Y
   LDY   #$57
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$08
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDY   #$36
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$37
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$38
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$39
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   LDY   #$42
   LDA   (FP),Y
   LDY   #$05
   STA   (SSP),Y
   LDY   #$43
   LDA   (FP),Y
   LDY   #$06
   STA   (SSP),Y
   LDY   #$56
   LDA   (FP),Y
   LDY   #$07
   STA   (SSP),Y
   LDY   #$57
   LDA   (FP),Y
   LDY   #$08
   STA   (SSP),Y
   JSR   f10
   LDY   #$58
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$02
   STA   (FP),Y
.case@18:
.switch@13_break:
   LDA   #$00
   LDY   #$5A
   STA   (FP),Y
   INY
   STA   (FP),Y
.loop@19_start:
   LDA   #$05
   LDY   #$5C
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   ORA   #$00
   BPL   .sx_done@54
   LDA   #$FF
.sx_done@54:
   LDY   #$5E
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
   CLC
   LDY   #$5C
   ADC   (FP),Y
   LDY   #$64
   STA   (FP),Y
   LDA   g2+1
   LDY   #$5D
   ADC   (FP),Y
   LDY   #$65
   STA   (FP),Y
   LDA   g2+2
   LDY   #$5E
   ADC   (FP),Y
   LDY   #$66
   STA   (FP),Y
   LDA   g2+3
   LDY   #$5F
   ADC   (FP),Y
   LDY   #$67
   STA   (FP),Y
   LDA   g2+4
   LDY   #$60
   ADC   (FP),Y
   LDY   #$68
   STA   (FP),Y
   LDA   g2+5
   LDY   #$61
   ADC   (FP),Y
   LDY   #$69
   STA   (FP),Y
   LDA   g2+6
   LDY   #$62
   ADC   (FP),Y
   LDY   #$6A
   STA   (FP),Y
   LDA   g2+7
   LDY   #$63
   ADC   (FP),Y
   LDY   #$6B
   STA   (FP),Y
   LDY   #$64
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
   LDA   gp9
   STA   DPTR
   LDA   gp9+1
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$6C
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$6D
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   SEC
   SBC   #$FF
   LDY   #$6E
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   SBC   #$00
   LDY   #$6F
   STA   (FP),Y
   LDA   gp9
   STA   DPTR
   LDA   gp9+1
   STA   DPTR+1
   DEY
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$6F
   LDA   (FP),Y
   LDY   #$01
   STA   (DPTR),Y
   LDY   #$6E
   LDA   (FP),Y
   LDY   #$70
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$71
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@55
   LDA   #$00
   JMP   .sx_done@56
.sx_neg@55:
   LDA   #$FF
.sx_done@56:
   LDY   #$72
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
   LDY   #$70
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
   LDA   z4
   CLC
   ADC   #$01
   INY
   STA   (FP),Y
   LDA   z4+1
   ADC   #$00
   INY
   STA   (FP),Y
   LDA   z4+2
   ADC   #$00
   INY
   STA   (FP),Y
   LDA   z4+3
   ADC   #$00
   INY
   STA   (FP),Y
   LDA   z4+4
   ADC   #$00
   INY
   STA   (FP),Y
   LDA   z4+5
   ADC   #$00
   INY
   STA   (FP),Y
   LDA   z4+6
   ADC   #$00
   INY
   STA   (FP),Y
   LDA   z4+7
   ADC   #$00
   INY
   STA   (FP),Y
   LDY   #$80
   LDA   (FP),Y
   STA   z4
   INY
   LDA   (FP),Y
   STA   z4+1
   INY
   LDA   (FP),Y
   STA   z4+2
   INY
   LDA   (FP),Y
   STA   z4+3
   INY
   LDA   (FP),Y
   STA   z4+4
   INY
   LDA   (FP),Y
   STA   z4+5
   INY
   LDA   (FP),Y
   STA   z4+6
   INY
   LDA   (FP),Y
   STA   z4+7
   LDA   gp8
   STA   DPTR
   LDA   gp8+1
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$88
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$89
   STA   (FP),Y
   LDA   g1
   INY
   STA   (FP),Y
   LDA   g1+1
   INY
   STA   (FP),Y
   LDY   #$88
   LDA   (FP),Y
   LDY   #$8A
   AND   (FP),Y
   LDY   #$8C
   STA   (FP),Y
   LDY   #$89
   LDA   (FP),Y
   LDY   #$8B
   AND   (FP),Y
   LDY   #$8D
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$8E
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$8F
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
   LDY   #$8E
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$8F
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$90
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$91
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   JSR   mix
   LDY   #$5B
   LDA   (FP),Y
   CLC
   ADC   #$01
   LDY   #$92
   STA   (FP),Y
   LDY   #$5B
   STA   (FP),Y
   LDY   #$93
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
   LDY   #$95
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@7
   JMP   .loop@19_start
.lb_skip@7:
   LDA   #<gs7
   LDY   #$97
   STA   (FP),Y
   LDA   #>gs7
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$05
   LDY   #$99
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$9A
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$9B
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$9C
   STA   (FP),Y
   CMP   g3+1
   BNE   .cmp_true@58
   DEY
   LDA   (FP),Y
   CMP   g3
   BNE   .cmp_true@58
   LDA   #$00
   JMP   .cmp_end@59
.cmp_true@58:
   LDA   #$01
.cmp_end@59:
   LDY   #$9D
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$9F
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$A0
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@61
   LDA   #$00
   JMP   .sx_done@62
.sx_neg@61:
   LDA   #$FF
.sx_done@62:
   LDY   #$A1
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
   LDY   #$9F
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
   ADC   #$B0
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

   ; prologue: 0 arg bytes, 131 local bytes
   SEC
   LDA   SSP
   SBC   #$85
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$84
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
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
.loop@20_start:
   LDY   #$0C
   LDA   (FP),Y
   CLC
   ADC   #$01
   LDY   #$39
   STA   (FP),Y
   LDY   #$0C
   STA   (FP),Y
   LDY   #$3A
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
   BVC   .cmp_novf@71
   EOR   #$80
.cmp_novf@71:
   ASL
   LDA   #$00
   ROL
   LDY   #$3C
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .loop@20_start
   LDA   #$01
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$40
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$41
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
   LDY   #$40
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
   LDA   g2
   STA   HARGS+8
   LDA   g2+1
   STA   HARGS+9
   LDA   g2+2
   STA   HARGS+10
   LDA   g2+3
   STA   HARGS+11
   LDA   g2+4
   STA   HARGS+12
   LDA   g2+5
   STA   HARGS+13
   LDA   g2+6
   STA   HARGS+14
   LDA   g2+7
   STA   HARGS+15
   JSR   mul64
   LDA   HARGS+16
   LDY   #$48
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
   LDY   #$48
   LDA   (FP),Y
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
   LDA   #$02
   INY
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
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   gp9
   STA   DPTR
   LDA   gp9+1
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$5B
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$5C
   STA   (FP),Y
   LDY   #$59
   LDA   (FP),Y
   LDY   #$5B
   EOR   (FP),Y
   LDY   #$5D
   STA   (FP),Y
   LDY   #$5A
   LDA   (FP),Y
   LDY   #$5C
   EOR   (FP),Y
   LDY   #$5E
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$08
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
   LDY   #$56
   LDA   (FP),Y
   LDY   #$05
   STA   (SSP),Y
   LDY   #$57
   LDA   (FP),Y
   LDY   #$06
   STA   (SSP),Y
   LDY   #$5D
   LDA   (FP),Y
   LDY   #$07
   STA   (SSP),Y
   LDY   #$5E
   LDA   (FP),Y
   LDY   #$08
   STA   (SSP),Y
   JSR   f10
   LDY   #$5F
   STA   (FP),Y
   INY
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@72
   LDA   #$00
   JMP   .sx_done@73
.sx_neg@72:
   LDA   #$FF
.sx_done@73:
   LDY   #$61
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
   LDA   #$FF
   LDY   #$64
   STA   (FP),Y
   INY
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@74
   LDA   #$00
   JMP   .sx_done@75
.sx_neg@74:
   LDA   #$FF
.sx_done@75:
   LDY   #$66
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   g1
   INY
   STA   (FP),Y
   LDA   g1+1
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
   LDA   #$80
   INY
   STA   (FP),Y
   ORA   #$00
   BEQ   .cond_else@3
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
   JMP   .cond_end@4
.cond_else@3:
   LDA   z5
   LDY   #$73
   STA   (FP),Y
   LDA   z5+1
   INY
   STA   (FP),Y
.cond_end@4:
   LDY   #$73
   LDA   (FP),Y
   LDY   #$75
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$76
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$08
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDY   #$65
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$66
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$67
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$68
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   LDY   #$6B
   LDA   (FP),Y
   LDY   #$05
   STA   (SSP),Y
   LDY   #$6C
   LDA   (FP),Y
   LDY   #$06
   STA   (SSP),Y
   LDY   #$75
   LDA   (FP),Y
   LDY   #$07
   STA   (SSP),Y
   LDY   #$76
   LDA   (FP),Y
   LDY   #$08
   STA   (SSP),Y
   JSR   f10
   LDY   #$77
   STA   (FP),Y
   LDA   #$02
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$78
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   INY
   ORA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lnot_true@76
   LDA   #$00
   JMP   .lnot_end@77
.lnot_true@76:
   LDA   #$01
.lnot_end@77:
   LDY   #$7C
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$7E
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$7F
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   z5
   INY
   LDA   (FP),Y
   STA   z5+1
   JSR   fin0
   JSR   fin1
   JSR   fin2
   LDA   acc
   LDY   #$80
   STA   (FP),Y
   LDA   acc+1
   AND   #$7F
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
   TAX
   DEY
   LDA   (FP),Y

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$85
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$84
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
   DC.L  $7FFFFFFF
   DC.L  $00000000

g3:
   DC.W  $E112

ga6:
   DC.L  $80000000
   DC.L  $7FFFFFFF
   DS.B  4
   DC.L  $000000FF

gs7:
   DS.B  1
   DC.L  $0000007F
   DC.W  $B7B0

gp8:
   DC.W  z5

gp9:
   DC.W  g1

@4.s17:
   DC.W  $0080

@13.s25:
   DC.L  $00008000

__zp_init:
   SUBROUTINE

   LDA   #$00
   STA   z4+1
   STA   z4+2
   STA   z4+3
   STA   z4+4
   STA   z4+5
   STA   z4+6
   STA   z4+7
   STA   z5
   LDA   #$7F
   STA   z4
   LDA   #$80
   STA   z5+1
   RTS
