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
   LDA   g3+1
   INY
   STA   (FP),Y
   LDA   g3+1
   BMI   .sx_neg@0
   LDA   #$00
   JMP   .sx_done@1
.sx_neg@0:
   LDA   #$FF
.sx_done@1:
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
   LDA   ga6
   LDY   #$07
   STA   (FP),Y
   LDA   ga6+1
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
   LDA   ga6+2
   LDY   #$0D
   STA   (FP),Y
   LDA   ga6+3
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
   LDA   ga6+4
   LDY   #$13
   STA   (FP),Y
   LDA   ga6+5
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

   ; prologue: 0 arg bytes, 27 local bytes
   SEC
   LDA   SSP
   SBC   #$1D
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$1C
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
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$0C
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$0D
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$0E
   STA   (FP),Y
   ORA   #$00
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
   LDA   #<gs7
   LDY   #$11
   STA   (FP),Y
   LDA   #>gs7
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$03
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
   LDY   #$02
   LDA   (DPTR),Y
   LDY   #$17
   STA   (FP),Y
   LDY   #$15
   LDA   (FP),Y
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
   ORA   #$00
   BMI   .sx_neg@6
   LDA   #$00
   JMP   .sx_done@7
.sx_neg@6:
   LDA   #$FF
.sx_done@7:
   LDY   #$1B
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$04
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDY   #$18
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$19
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$1A
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$1B
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   JSR   mix

   ; epilogue
   CLC
   LDA   FP
   ADC   #$1D
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$1C
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

   ; prologue: 6 arg bytes, 144 local bytes
   SEC
   LDA   SSP
   SBC   #$92
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$91
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
   LDA   #$80
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$03
   LDA   (FP),Y
   LDY   #$09
   STA   (FP),Y
   LDY   #$04
   LDA   (FP),Y
   LDY   #$0A
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
   DEY
   LDA   (FP),Y
   LDY   #$03
   STA   (FP),Y
   LDY   #$0C
   LDA   (FP),Y
   LDY   #$04
   STA   (FP),Y
   LDY   #$0B
   LDA   (FP),Y
   EOR   @5.s17
   LDY   #$0D
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   EOR   @5.s17+1
   LDY   #$0E
   STA   (FP),Y
   LDA   #$07
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$0D
   LDA   (FP),Y
   LDY   #$0F
   AND   (FP),Y
   LDY   #$11
   STA   (FP),Y
   LDY   #$0E
   LDA   (FP),Y
   LDY   #$10
   AND   (FP),Y
   LDY   #$12
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   BEQ   .cmp_true@8
   LDA   #$00
   JMP   .cmp_end@9
.cmp_true@8:
   LDA   #$01
.cmp_end@9:
   LDY   #$13
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .case@1
   LDY   #$11
   LDA   (FP),Y
   CMP   #$02
   BEQ   .cmp_true@10
   LDA   #$00
   JMP   .cmp_end@11
.cmp_true@10:
   LDA   #$01
.cmp_end@11:
   LDY   #$15
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@5
   JMP   .switch@0_break
.lb_skip@5:
   LDY   #$11
   LDA   (FP),Y
   CMP   #$04
   BEQ   .cmp_true@12
   LDA   #$00
   JMP   .cmp_end@13
.cmp_true@12:
   LDA   #$01
.cmp_end@13:
   LDY   #$17
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@4
   JMP   .case@14
.lb_skip@4:
   LDY   #$11
   LDA   (FP),Y
   CMP   #$05
   BEQ   .cmp_true@14
   LDA   #$00
   JMP   .cmp_end@15
.cmp_true@14:
   LDA   #$01
.cmp_end@15:
   LDY   #$19
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@3
   JMP   .case@15
.lb_skip@3:
   JMP   .switch@0_break
.case@1:
   LDA   #<gs7
   LDY   #$1B
   STA   (FP),Y
   LDA   #>gs7
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$03
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
   LDY   #$02
   LDA   (DPTR),Y
   LDY   #$21
   STA   (FP),Y
   LDA   z5
   INY
   STA   (FP),Y
   LDA   z5+1
   INY
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
   BMI   .sx_neg@16
   LDA   #$00
   JMP   .sx_done@17
.sx_neg@16:
   LDA   #$FF
.sx_done@17:
   LDY   #$26
   STA   (FP),Y
   LDY   #$1F
   LDA   (FP),Y
   LDY   #$24
   EOR   (FP),Y
   LDY   #$27
   STA   (FP),Y
   LDY   #$20
   LDA   (FP),Y
   LDY   #$25
   EOR   (FP),Y
   LDY   #$28
   STA   (FP),Y
   LDY   #$21
   LDA   (FP),Y
   LDY   #$26
   EOR   (FP),Y
   LDY   #$29
   STA   (FP),Y
   LDY   #$27
   LDA   (FP),Y
   LDY   #$2A
   STA   (FP),Y
   LDY   #$28
   LDA   (FP),Y
   LDY   #$2B
   STA   (FP),Y
   LDA   #$07
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$2A
   LDA   (FP),Y
   LDY   #$2C
   AND   (FP),Y
   LDY   #$2E
   STA   (FP),Y
   LDY   #$2B
   LDA   (FP),Y
   LDY   #$2D
   AND   (FP),Y
   LDY   #$2F
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CMP   #$02
   BEQ   .cmp_true@18
   LDA   #$00
   JMP   .cmp_end@19
.cmp_true@18:
   LDA   #$01
.cmp_end@19:
   LDY   #$30
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .case@3
   LDY   #$2E
   LDA   (FP),Y
   CMP   #$06
   BEQ   .cmp_true@20
   LDA   #$00
   JMP   .cmp_end@21
.cmp_true@20:
   LDA   #$01
.cmp_end@21:
   LDY   #$32
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@2
   JMP   .case@4
.lb_skip@2:
   JMP   .default@10
.case@3:
   LDA   #<ga6
   LDY   #$34
   STA   (FP),Y
   LDA   #>ga6
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
   LDY   #$38
   STA   (FP),Y
   STA   HARGS
   LDA   #$02
   STA   HARGS+1
   JSR   mul8
   LDA   HARGS+2
   LDY   #$39
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$34
   LDA   (FP),Y
   CLC
   LDY   #$3A
   ADC   (FP),Y
   LDY   #$3C
   STA   (FP),Y
   LDY   #$35
   LDA   (FP),Y
   LDY   #$3B
   ADC   (FP),Y
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
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$3F
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
   LDY   #$40
   STA   (FP),Y
   LDA   HARGS+5
   INY
   STA   (FP),Y
   LDY   #$3C
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$40
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$41
   LDA   (FP),Y
   LDY   #$01
   STA   (DPTR),Y
   JMP   .switch@0_break
.case@4:
   LDA   g2
   LDY   #$42
   STA   (FP),Y
   LDA   g2+1
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
   DEY
   LDA   (FP),Y
   STA   g2
   INY
   LDA   (FP),Y
   STA   g2+1
   LDY   #$42
   LDA   (FP),Y
   LDY   #$46
   STA   (FP),Y
   LDY   #$43
   LDA   (FP),Y
   LDY   #$47
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$46
   LDA   (FP),Y
   SEC
   SBC   #$02
   LDY   #$49
   STA   (FP),Y
   LDY   #$47
   LDA   (FP),Y
   SBC   #$00
   LDY   #$4A
   STA   (FP),Y
   LDY   #$48
   LDA   (FP),Y
   SBC   #$00
   LDY   #$4B
   STA   (FP),Y
   LDY   #$49
   LDA   (FP),Y
   LDY   #$4C
   STA   (FP),Y
   LDY   #$4A
   LDA   (FP),Y
   LDY   #$4D
   STA   (FP),Y
   LDY   #$4B
   LDA   (FP),Y
   AND   #$03
   LDY   #$4E
   STA   (FP),Y
   LDY   #$4C
   LDA   (FP),Y
   LDY   #$4F
   STA   (FP),Y
   LDY   #$4D
   LDA   (FP),Y
   LDY   #$50
   STA   (FP),Y
   LDY   #$4E
   LDA   (FP),Y
   EOR   #$02
   LDY   #$51
   STA   (FP),Y
   LDY   #$4F
   LDA   (FP),Y
   LDY   #$52
   STA   (FP),Y
   LDY   #$50
   LDA   (FP),Y
   LDY   #$53
   STA   (FP),Y
   LDY   #$51
   LDA   (FP),Y
   SEC
   SBC   #$02
   LDY   #$54
   STA   (FP),Y
   LDY   #$52
   LDA   (FP),Y
   LDY   #$55
   STA   (FP),Y
   LDY   #$53
   LDA   (FP),Y
   LDY   #$56
   STA   (FP),Y
   LDA   #$07
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$55
   LDA   (FP),Y
   LDY   #$57
   AND   (FP),Y
   LDY   #$59
   STA   (FP),Y
   LDY   #$56
   LDA   (FP),Y
   LDY   #$58
   AND   (FP),Y
   LDY   #$5A
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   BEQ   .cmp_true@22
   LDA   #$00
   JMP   .cmp_end@23
.cmp_true@22:
   LDA   #$01
.cmp_end@23:
   LDY   #$5B
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@1
   JMP   .switch@5_break
.lb_skip@1:
   LDY   #$59
   LDA   (FP),Y
   CMP   #$02
   BEQ   .cmp_true@24
   LDA   #$00
   JMP   .cmp_end@25
.cmp_true@24:
   LDA   #$01
.cmp_end@25:
   LDY   #$5D
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@0
   JMP   .switch@5_break
.lb_skip@0:
   LDY   #$59
   LDA   (FP),Y
   CMP   #$04
   BEQ   .cmp_true@26
   LDA   #$00
   JMP   .cmp_end@27
.cmp_true@26:
   LDA   #$01
.cmp_end@27:
   LDY   #$5F
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .switch@5_break
   LDY   #$59
   LDA   (FP),Y
   CMP   #$05
   BEQ   .cmp_true@28
   LDA   #$00
   JMP   .cmp_end@29
.cmp_true@28:
   LDA   #$01
.cmp_end@29:
   LDY   #$61
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .switch@5_break
   LDA   z4
   EOR   #$03
   INY
   STA   (FP),Y
   LDA   z4+1
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   z4
   INY
   LDA   (FP),Y
   STA   z4+1
   DEY
   LDA   (FP),Y
   LDY   #$65
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$66
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   g3
   INY
   LDA   (FP),Y
   STA   g3+1
.switch@5_break:
   LDY   #$03
   LDA   (FP),Y
   LDY   #$67
   STA   (FP),Y
   LDY   #$04
   LDA   (FP),Y
   LDY   #$68
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$69
   STA   (FP),Y
   LDY   #$93
   STA   (FP),Y
   JMP   .switch@0_break
.default@10:
   LDA   #$00
   LDY   #$6A
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   SEC
   SBC   #$02
   LDA   (FP),Y
   SBC   #$00
   BVC   .cmp_novf@30
   EOR   #$80
.cmp_novf@30:
   ASL
   LDA   #$00
   ROL
   LDY   #$6E
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   LDA   #$00
   LDY   #$76
   STA   (FP),Y
   INY
   STA   (FP),Y
.loop@12_continue:
   LDY   #$77
   LDA   (FP),Y
   INY
   STA   (FP),Y
   CLC
   ADC   #$01
   INY
   STA   (FP),Y
   LDY   #$77
   STA   (FP),Y
   INY
   LDA   (FP),Y
   LDY   #$7A
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
   BVC   .cmp_novf@31
   EOR   #$80
.cmp_novf@31:
   ASL
   LDA   #$00
   ROL
   LDY   #$7C
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .switch@0_break
   LDY   #$77
   LDA   (FP),Y
   LDY   #$7E
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDA   g3
   INY
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
   LDY   #$7E
   LDA   (FP),Y
   LDY   #$80
   AND   (FP),Y
   LDY   #$84
   STA   (FP),Y
   LDY   #$7F
   LDA   (FP),Y
   LDY   #$81
   AND   (FP),Y
   LDY   #$85
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$03
   STA   (FP),Y
   LDY   #$85
   LDA   (FP),Y
   LDY   #$04
   STA   (FP),Y
   JMP   .loop@12_continue
.case@14:
.case@15:
.switch@0_break:
   LDA   #$00
   LDY   #$86
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$01
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDA   g2
   INY
   STA   (FP),Y
   LDA   g2+1
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$8A
   LDA   (FP),Y
   SEC
   LDY   #$86
   SBC   (FP),Y
   LDY   #$8B
   LDA   (FP),Y
   LDY   #$87
   SBC   (FP),Y
   LDY   #$8C
   LDA   (FP),Y
   LDY   #$88
   SBC   (FP),Y
   LDY   #$8D
   LDA   (FP),Y
   LDY   #$89
   SBC   (FP),Y
   LDA   #$00
   ROL
   LDY   #$8E
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$90
   STA   (FP),Y

   ; epilogue
   PHA
   CLC
   LDA   FP
   ADC   #$98
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

   ; prologue: 6 arg bytes, 212 local bytes
   SEC
   LDA   SSP
   SBC   #$D6
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$D5
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
   LDA   z5
   INY
   STA   (FP),Y
   LDA   z5+1
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
   STA   z5
   INY
   LDA   (FP),Y
   STA   z5+1
   LDA   #$00
   INY
   STA   (FP),Y
   LDA   #$80
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
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$18
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$19
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$1A
   STA   (FP),Y
   LDY   #$11
   LDA   (FP),Y
   CLC
   LDY   #$19
   ADC   (FP),Y
   LDY   #$1B
   STA   (FP),Y
   LDY   #$12
   LDA   (FP),Y
   LDY   #$1A
   ADC   (FP),Y
   LDY   #$1C
   STA   (FP),Y
   LDY   #$0B
   LDA   (FP),Y
   LSR
   LSR
   LSR
   LSR
   LSR
   LDY   #$1E
   STA   (FP),Y
   LDY   #$0B
   LDA   (FP),Y
   ASL
   ASL
   ASL
   LDY   #$1D
   STA   (FP),Y
   LDY   #$1F
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$20
   STA   (FP),Y
   LDY   #$1B
   LDA   (FP),Y
   CLC
   LDY   #$1F
   SBC   (FP),Y
   LDY   #$1C
   LDA   (FP),Y
   LDY   #$20
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
   LDY   #$23
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$24
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@32
   LDA   #$00
   JMP   .sx_done@33
.sx_neg@32:
   LDA   #$FF
.sx_done@33:
   LDY   #$25
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
   LDY   #$23
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$24
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$25
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$26
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
   LDY   #$27
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$28
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
   LDA   gp8
   STA   DPTR
   LDA   gp8+1
   STA   DPTR+1
   DEY
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$2A
   LDA   (FP),Y
   LDY   #$01
   STA   (DPTR),Y
   LDY   #$29
   LDA   (FP),Y
   LDY   #$2B
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$2C
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@34
   LDA   #$00
   JMP   .sx_done@35
.sx_neg@34:
   LDA   #$FF
.sx_done@35:
   LDY   #$2D
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
   LDY   #$2B
   LDA   (FP),Y
   EOR   #$7F
   LDY   #$33
   STA   (FP),Y
   LDY   #$2C
   LDA   (FP),Y
   LDY   #$34
   STA   (FP),Y
   LDY   #$2D
   LDA   (FP),Y
   LDY   #$35
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
   LDY   #$33
   LDA   (FP),Y
   LDY   #$3B
   STA   (FP),Y
   LDY   #$34
   LDA   (FP),Y
   LDY   #$3C
   STA   (FP),Y
   LDA   gp9
   STA   DPTR
   LDA   gp9+1
   STA   DPTR+1
   DEY
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$3C
   LDA   (FP),Y
   LDY   #$01
   STA   (DPTR),Y
   LDA   #<gs7
   LDY   #$3D
   STA   (FP),Y
   LDA   #>gs7
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$01
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
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$41
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$42
   STA   (FP),Y
   LDY   #$44
   STA   (FP),Y
   ASL
   LDA   (FP),Y
   ROR
   STA   (FP),Y
   LDY   #$41
   LDA   (FP),Y
   LDY   #$43
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
   INY
   ORA   (FP),Y
   BNE   .lb_skip@8
   JMP   .if_end@0
.lb_skip@8:
   LDA   #$7F
   LDY   #$45
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
   LDY   #$47
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$48
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$49
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@38
   LDA   #$00
   JMP   .sx_done@39
.sx_neg@38:
   LDA   #$FF
.sx_done@39:
   LDY   #$4A
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$DB
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$4C
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$4D
   STA   (FP),Y
   LDY   #$02
   LDA   (DPTR),Y
   LDY   #$4E
   STA   (FP),Y
   LDY   #$03
   LDA   (DPTR),Y
   LDY   #$4F
   STA   (FP),Y
   LDY   #$4C
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
   LDA   #$5A
   STA   HARGS+4
   LDA   #$00
   STA   HARGS+5
   LDY   #$00
   STY   HARGS+6
   STA   HARGS+7
   JSR   mul32
   LDA   HARGS+8
   LDY   #$50
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
   LDY   #$50
   LDA   (FP),Y
   LDY   #$54
   STA   (FP),Y
   LDY   #$51
   LDA   (FP),Y
   LDY   #$55
   STA   (FP),Y
   LDY   #$52
   LDA   (FP),Y
   LDY   #$56
   STA   (FP),Y
   LDY   #$53
   LDA   (FP),Y
   AND   #$01
   LDY   #$57
   STA   (FP),Y
   LDY   #$54
   LDA   (FP),Y
   LDY   #$58
   STA   (FP),Y
   LDY   #$55
   LDA   (FP),Y
   LDY   #$59
   STA   (FP),Y
   LDY   #$56
   LDA   (FP),Y
   LDY   #$5A
   STA   (FP),Y
   LDY   #$57
   LDA   (FP),Y
   EOR   #$01
   LDY   #$5B
   STA   (FP),Y
   LDY   #$58
   LDA   (FP),Y
   LDY   #$5C
   STA   (FP),Y
   LDY   #$59
   LDA   (FP),Y
   LDY   #$5D
   STA   (FP),Y
   LDY   #$5A
   LDA   (FP),Y
   LDY   #$5E
   STA   (FP),Y
   LDY   #$5B
   LDA   (FP),Y
   SEC
   SBC   #$01
   LDY   #$5F
   STA   (FP),Y
   LDY   #$DB
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$5C
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$5D
   LDA   (FP),Y
   LDY   #$01
   STA   (DPTR),Y
   LDY   #$5E
   LDA   (FP),Y
   LDY   #$02
   STA   (DPTR),Y
   LDY   #$5F
   LDA   (FP),Y
   LDY   #$03
   STA   (DPTR),Y
   LDY   #$48
   LDA   (FP),Y
   LDY   #$5C
   ORA   (FP),Y
   LDY   #$60
   STA   (FP),Y
   LDY   #$49
   LDA   (FP),Y
   LDY   #$5D
   ORA   (FP),Y
   LDY   #$61
   STA   (FP),Y
   LDY   #$4A
   LDA   (FP),Y
   LDY   #$5E
   ORA   (FP),Y
   LDY   #$62
   STA   (FP),Y
   LDY   #$4B
   LDA   (FP),Y
   LDY   #$5F
   ORA   (FP),Y
   LDY   #$63
   STA   (FP),Y
   LDY   #$60
   LDA   (FP),Y
   LDY   #$64
   STA   (FP),Y
   LDY   #$61
   LDA   (FP),Y
   LDY   #$65
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   z4
   INY
   LDA   (FP),Y
   STA   z4+1
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
.loop@16_start:
   LDY   #$68
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
   BVC   .cmp_novf@40
   EOR   #$80
.cmp_novf@40:
   ASL
   LDA   #$00
   ROL
   LDY   #$6A
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@7
   JMP   .loop@16_break
.lb_skip@7:
   LDA   g1
   SEC
   SBC   #$01
   LDY   #$6C
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
   DEY
   LDA   (FP),Y
   LDY   #$6E
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$6F
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   @13.s26
   CLC
   LDY   #$6E
   ADC   (FP),Y
   LDY   #$72
   STA   (FP),Y
   LDA   @13.s26+1
   LDY   #$6F
   ADC   (FP),Y
   LDY   #$73
   STA   (FP),Y
   LDA   @13.s26+2
   LDY   #$70
   ADC   (FP),Y
   LDY   #$74
   STA   (FP),Y
   LDA   @13.s26+3
   LDY   #$71
   ADC   (FP),Y
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
   LDY   #$67
   LDA   (FP),Y
   LDY   #$76
   STA   (FP),Y
   CLC
   ADC   #$01
   INY
   STA   (FP),Y
   LDY   #$67
   STA   (FP),Y
   JMP   .loop@16_start
.loop@16_break:
.if_end@0:
   LDA   ga6+2
   LDY   #$78
   STA   (FP),Y
   LDA   ga6+3
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@6
   JMP   .if_else@2
.lb_skip@6:
   LDY   #$0B
   LDA   (FP),Y
   LDY   #$7A
   STA   (FP),Y
   LDY   #$0C
   LDA   (FP),Y
   LDY   #$7B
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@41
   LDA   #$00
   JMP   .sx_done@42
.sx_neg@41:
   LDA   #$FF
.sx_done@42:
   LDY   #$7C
   STA   (FP),Y
   LDA   #$00
   STA   HARGS
   LDY   #$00
   STY   HARGS+1
   STY   HARGS+2
   STA   HARGS+3
   LDY   #$7A
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
   LDY   #$7D
   STA   (FP),Y
   LDA   HARGS+9
   INY
   STA   (FP),Y
   LDA   HARGS+10
   INY
   STA   (FP),Y
   LDY   #$7D
   LDA   (FP),Y
   LDY   #$80
   STA   (FP),Y
   LDY   #$7E
   LDA   (FP),Y
   LDY   #$81
   STA   (FP),Y
   LDY   #$7F
   LDA   (FP),Y
   AND   #$03
   LDY   #$82
   STA   (FP),Y
   LDY   #$80
   LDA   (FP),Y
   LDY   #$83
   STA   (FP),Y
   LDY   #$81
   LDA   (FP),Y
   LDY   #$84
   STA   (FP),Y
   LDY   #$82
   LDA   (FP),Y
   EOR   #$02
   LDY   #$85
   STA   (FP),Y
   LDY   #$83
   LDA   (FP),Y
   LDY   #$86
   STA   (FP),Y
   LDY   #$84
   LDA   (FP),Y
   LDY   #$87
   STA   (FP),Y
   LDY   #$85
   LDA   (FP),Y
   SEC
   SBC   #$02
   LDY   #$88
   STA   (FP),Y
   LDY   #$86
   LDA   (FP),Y
   LDY   #$89
   STA   (FP),Y
   LDY   #$87
   LDA   (FP),Y
   LDY   #$8A
   STA   (FP),Y
   LDY   #$88
   LDA   (FP),Y
   LDY   #$8B
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@43
   LDA   #$00
   JMP   .sx_done@44
.sx_neg@43:
   LDA   #$FF
.sx_done@44:
   LDY   #$8C
   STA   (FP),Y
   LDA   @13.s26
   STA   HARGS
   LDA   @13.s26+1
   STA   HARGS+1
   LDA   @13.s26+2
   STA   HARGS+2
   LDA   @13.s26+3
   STA   HARGS+3
   LDA   #$06
   STA   HARGS+4
   JSR   asl32
   LDA   HARGS+5
   LDY   #$8D
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
   LDY   #$89
   LDA   (FP),Y
   SEC
   LDY   #$8D
   SBC   (FP),Y
   LDY   #$91
   STA   (FP),Y
   LDY   #$8A
   LDA   (FP),Y
   LDY   #$8E
   SBC   (FP),Y
   LDY   #$92
   STA   (FP),Y
   LDY   #$8B
   LDA   (FP),Y
   LDY   #$8F
   SBC   (FP),Y
   LDY   #$93
   STA   (FP),Y
   LDY   #$8C
   LDA   (FP),Y
   LDY   #$90
   SBC   (FP),Y
   LDY   #$94
   STA   (FP),Y
   LDY   #$91
   LDA   (FP),Y
   LDY   #$95
   STA   (FP),Y
   LDY   #$92
   LDA   (FP),Y
   LDY   #$96
   STA   (FP),Y
   LDY   #$93
   LDA   (FP),Y
   LDY   #$97
   STA   (FP),Y
   LDY   #$95
   LDA   (FP),Y
   LDY   #$98
   STA   (FP),Y
   LDY   #$96
   LDA   (FP),Y
   LDY   #$99
   STA   (FP),Y
   LDY   #$97
   LDA   (FP),Y
   AND   #$03
   LDY   #$9A
   STA   (FP),Y
   LDY   #$98
   LDA   (FP),Y
   LDY   #$9B
   STA   (FP),Y
   LDY   #$99
   LDA   (FP),Y
   LDY   #$9C
   STA   (FP),Y
   LDY   #$9A
   LDA   (FP),Y
   EOR   #$02
   LDY   #$9D
   STA   (FP),Y
   LDY   #$9B
   LDA   (FP),Y
   LDY   #$9E
   STA   (FP),Y
   LDY   #$9C
   LDA   (FP),Y
   LDY   #$9F
   STA   (FP),Y
   LDY   #$9D
   LDA   (FP),Y
   SEC
   SBC   #$02
   LDY   #$A0
   STA   (FP),Y
   LDY   #$9E
   LDA   (FP),Y
   STA   HARGS+8
   INY
   LDA   (FP),Y
   STA   HARGS+9
   INY
   LDA   (FP),Y
   STA   HARGS+10

   ; epilogue
   CLC
   LDA   FP
   ADC   #$DC
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$D5
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   RTS
.if_else@2:
   LDA   gp9
   STA   DPTR
   LDA   gp9+1
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$BD
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$BE
   STA   (FP),Y
   LDY   #$C0
   STA   (FP),Y
   LSR
   STA   (FP),Y
   LDY   #$BD
   LDA   (FP),Y
   LDY   #$BF
   STA   (FP),Y
   ROR
   STA   (FP),Y
   LDY   #$D8
   LDA   (FP),Y
   LDY   #$C2
   STA   (FP),Y
   LSR
   STA   (FP),Y
   LDY   #$D7
   LDA   (FP),Y
   LDY   #$C1
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
   INY
   LDA   (FP),Y
   LSR
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ROR
   STA   (FP),Y
   LDY   #$D7
   STA   (FP),Y
   LDY   #$C2
   LDA   (FP),Y
   LDY   #$D8
   STA   (FP),Y
   LDY   #$BF
   LDA   (FP),Y
   LDY   #$C1
   AND   (FP),Y
   LDY   #$C3
   STA   (FP),Y
   LDY   #$C0
   LDA   (FP),Y
   LDY   #$C2
   AND   (FP),Y
   LDY   #$C4
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$C5
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$C6
   STA   (FP),Y
   LDA   #$00
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
   ADC   #$03
   LDY   #$CA
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$CB
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$C5
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$C6
   LDA   (FP),Y
   LDY   #$01
   STA   (DPTR),Y
   LDY   #$C7
   LDA   (FP),Y
   LDY   #$02
   STA   (DPTR),Y
   LDA   z5
   LDY   #$CC
   STA   (FP),Y
   LDA   z5+1
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@3
   LDA   gp9
   STA   DPTR
   LDA   gp9+1
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$CE
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$CF
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@3
   LDA   #$01
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   JMP   .and_end@4
.and_false@3:
   LDA   #$00
   LDY   #$D0
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@4:
   LDY   #$D0
   LDA   (FP),Y
   LDY   #$D2
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$D3
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@45
   LDA   #$00
   JMP   .sx_done@46
.sx_neg@45:
   LDA   #$FF
.sx_done@46:
   LDY   #$D4
   STA   (FP),Y
   LDY   #$D2
   LDA   (FP),Y
   STA   HARGS+8
   INY
   LDA   (FP),Y
   STA   HARGS+9
   INY
   LDA   (FP),Y
   STA   HARGS+10

   ; epilogue
   CLC
   LDA   FP
   ADC   #$DC
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

   ; prologue: 0 arg bytes, 130 local bytes
   SEC
   LDA   SSP
   SBC   #$84
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$83
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
   DEY
   LDA   (FP),Y
   LDY   #$08
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDA   #$80
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
.loop@17_continue:
   LDY   #$12
   LDA   (FP),Y
   INY
   STA   (FP),Y
   CLC
   ADC   #$01
   INY
   STA   (FP),Y
   LDY   #$12
   STA   (FP),Y
   INY
   LDA   (FP),Y
   LDY   #$15
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
   BVC   .cmp_novf@47
   EOR   #$80
.cmp_novf@47:
   ASL
   LDA   #$00
   ROL
   LDY   #$17
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@10
   JMP   .loop@17_break
.lb_skip@10:
   JMP   .loop@17_continue
.loop@18_continue:
   LDY   #$20
   LDA   (FP),Y
   INY
   STA   (FP),Y
   CLC
   ADC   #$01
   INY
   STA   (FP),Y
   LDY   #$20
   STA   (FP),Y
   INY
   LDA   (FP),Y
   LDY   #$23
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
   BVC   .cmp_novf@48
   EOR   #$80
.cmp_novf@48:
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
   BEQ   .loop@18_break
   LDY   #$02
   LDA   (FP),Y
   LDY   #$27
   STA   (FP),Y
   SEC
   SBC   #$01
   INY
   STA   (FP),Y
   LDY   #$02
   STA   (FP),Y
   JMP   .loop@18_continue
.loop@18_break:
   LDY   #$04
   LDA   (FP),Y
   LDY   #$29
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LSR
   DEY
   LDA   (FP),Y
   ROR
   LDY   #$2B
   STA   (FP),Y
   LDY   #$2D
   STA   (FP),Y
   LDY   #$04
   STA   (FP),Y
   ORA   #$00
   BEQ   .if_else@6
   LDA   g2
   EOR   #$01
   LDY   #$2E
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
   LDY   #$30
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$31
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
   LDY   #$30
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$31
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$32
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$33
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   JSR   mix
   JMP   .loop@17_continue
.if_else@6:
   LDY   #$07
   LDA   (FP),Y
   LDY   #$34
   STA   (FP),Y
   LDY   #$08
   LDA   (FP),Y
   LDY   #$35
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   SEC
   SBC   #$01
   LDY   #$36
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   SBC   #$00
   LDY   #$37
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$07
   STA   (FP),Y
   LDY   #$37
   LDA   (FP),Y
   LDY   #$08
   STA   (FP),Y
   JMP   .loop@17_continue
.loop@17_break:
   LDY   #$07
   LDA   (FP),Y
   LDY   #$38
   STA   (FP),Y
   LDY   #$08
   LDA   (FP),Y
   LDY   #$39
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
   LDY   #$3A
   STA   (FP),Y
   LDA   HARGS+5
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$07
   STA   (FP),Y
   LDY   #$3B
   LDA   (FP),Y
   LDY   #$08
   STA   (FP),Y
   LDY   #$3A
   LDA   (FP),Y
   LDY   #$3C
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$3D
   STA   (FP),Y
   LDA   #$07
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$3C
   LDA   (FP),Y
   LDY   #$3E
   AND   (FP),Y
   LDY   #$40
   STA   (FP),Y
   LDY   #$3D
   LDA   (FP),Y
   LDY   #$3F
   AND   (FP),Y
   LDY   #$41
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CMP   #$03
   BEQ   .cmp_true@49
   LDA   #$00
   JMP   .cmp_end@50
.cmp_true@49:
   LDA   #$01
.cmp_end@50:
   LDY   #$42
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@9
   JMP   .switch@19_break
.lb_skip@9:
   LDY   #$04
   LDA   (FP),Y
   LDY   #$44
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$46
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$47
   STA   (FP),Y
   LDA   g1
   SEC
   DEY
   SBC   (FP),Y
   LDY   #$48
   STA   (FP),Y
   LDA   g1+1
   DEY
   SBC   (FP),Y
   LDY   #$49
   STA   (FP),Y
   LDY   #$04
   LDA   (FP),Y
   LDY   #$4A
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$4C
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$4D
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ORA   g2
   LDY   #$4E
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ORA   g2+1
   LDY   #$4F
   STA   (FP),Y
   CLC
   LDA   FP
   ADC   #$0D
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
   LDY   #$48
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$49
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$4E
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$4F
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   LDY   #$50
   LDA   (FP),Y
   LDY   #$05
   STA   (SSP),Y
   LDY   #$51
   LDA   (FP),Y
   LDY   #$06
   STA   (SSP),Y
   JSR   f20
   LDA   HARGS+8
   LDY   #$52
   STA   (FP),Y
   LDA   HARGS+9
   INY
   STA   (FP),Y
   LDA   HARGS+10
   INY
   STA   (FP),Y
   LDY   #$52
   LDA   (FP),Y
   LDY   #$55
   STA   (FP),Y
   LDY   #$53
   LDA   (FP),Y
   LDY   #$56
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$07
   STA   (FP),Y
   LDY   #$56
   LDA   (FP),Y
   LDY   #$08
   STA   (FP),Y
   LDA   #$00
   LDY   #$57
   STA   (FP),Y
   ORA   #$00
   BNE   .or_true@7
   LDA   #$7F
   INY
   STA   (FP),Y
   ORA   #$00
   BNE   .or_true@7
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   JMP   .or_end@8
.or_true@7:
   LDA   #$01
   LDY   #$59
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@8:
   LDY   #$02
   LDA   (FP),Y
   LDY   #$5B
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
.mul_loop@51:
   BCC   .mul_skip@52
   CLC
   ADC   #$01
.mul_skip@52:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@51
   LDY   #$5E
   STA   (FP),Y
   LDA   HARGS
   DEY
   STA   (FP),Y
   LDY   #$5F
   STA   (FP),Y
   LDY   #$02
   STA   (FP),Y
   LDY   #$60
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$59
   LDA   (FP),Y
   LDY   #$60
   AND   (FP),Y
   LDY   #$62
   STA   (FP),Y
   LDY   #$5A
   LDA   (FP),Y
   LDY   #$61
   AND   (FP),Y
   LDY   #$63
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$0B
   STA   (FP),Y
   LDY   #$63
   LDA   (FP),Y
   LDY   #$0C
   STA   (FP),Y
.switch@19_break:
   LDA   gp8
   STA   DPTR
   LDA   gp8+1
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$64
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$65
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .cond_else@11
   LDA   z4
   INY
   STA   (FP),Y
   LDA   z4+1
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   INY
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
   INY
   STA   (FP),Y
   JMP   .cond_end@12
.cond_else@11:
   LDY   #$0D
   LDA   (FP),Y
   LDY   #$6A
   STA   (FP),Y
   LDY   #$0E
   LDA   (FP),Y
   LDY   #$6B
   STA   (FP),Y
   LDY   #$0F
   LDA   (FP),Y
   LDY   #$6C
   STA   (FP),Y
   LDY   #$10
   LDA   (FP),Y
   LDY   #$6D
   STA   (FP),Y
.cond_end@12:
   LDY   #$6A
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   INY
   ORA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@9
   LDA   #$FF
   INY
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
   LDY   #$70
   STA   (FP),Y
   LDA   z5
   INY
   STA   (FP),Y
   LDA   z5+1
   INY
   STA   (FP),Y
   LDY   #$6F
   LDA   (FP),Y
   CLC
   LDY   #$71
   ADC   (FP),Y
   LDY   #$73
   STA   (FP),Y
   LDY   #$70
   LDA   (FP),Y
   LDY   #$72
   ADC   (FP),Y
   LDY   #$74
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@9
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   JMP   .or_end@10
.or_true@9:
   LDA   #$01
   LDY   #$75
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@10:
   LDA   #$07
   LDY   #$77
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$75
   LDA   (FP),Y
   LDY   #$77
   AND   (FP),Y
   LDY   #$79
   STA   (FP),Y
   LDY   #$76
   LDA   (FP),Y
   LDY   #$78
   AND   (FP),Y
   LDY   #$7A
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CMP   #$05
   BEQ   .cmp_true@55
   LDA   #$00
   JMP   .cmp_end@56
.cmp_true@55:
   LDA   #$01
.cmp_end@56:
   LDY   #$7B
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .case@23
   LDY   #$79
   LDA   (FP),Y
   CMP   #$06
   BEQ   .cmp_true@57
   LDA   #$00
   JMP   .cmp_end@58
.cmp_true@57:
   LDA   #$01
.cmp_end@58:
   LDY   #$7D
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .case@24
.case@23:
.case@24:
   JSR   fin0
   JSR   fin1
   JSR   fin2
   LDA   acc
   LDY   #$7F
   STA   (FP),Y
   LDA   acc+1
   AND   #$7F
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$81
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$82
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
   ADC   #$84
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$83
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
   DC.W  $8000

g3:
   DC.W  $000B

ga6:
   DC.W  $0007
   DC.W  $007F
   DC.W  $0001
   DC.W  $7FFF

gs7:
   DS.B  1
   DC.W  $D768
   DC.B  $FF
   DS.B  2

gp8:
   DC.W  g3

gp9:
   DC.W  z4

@5.s17:
   DC.W  $FFFF

@13.s26:
   DC.L  $00000002

__zp_init:
   SUBROUTINE

   LDA   #$00
   STA   z4+1
   STA   z5
   LDA   #$09
   STA   z4
   LDA   #$80
   STA   z5+1
   RTS
