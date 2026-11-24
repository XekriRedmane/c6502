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
   LDA   g4
   LDY   #$0D
   STA   (FP),Y
   LDA   g4+1
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
   LDA   ga5+1
   LDY   #$06
   STA   (FP),Y
   INY
   STA   (FP),Y
   ORA   #$00
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
   ORA   #$00
   BMI   .sx_neg@4
   LDA   #$00
   JMP   .sx_done@5
.sx_neg@4:
   LDA   #$FF
.sx_done@5:
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
   LDA   ga5+3
   LDY   #$10
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
   LDY   #$12
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
   INY
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@8
   LDA   #$00
   JMP   .sx_done@9
.sx_neg@8:
   LDA   #$FF
.sx_done@9:
   LDY   #$07
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
   LDA   #<gs6
   LDY   #$0A
   STA   (FP),Y
   LDA   #>gs6
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$02
   LDY   #$0C
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$0D
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$0E
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$0F
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$10
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$11
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

f8:
   SUBROUTINE

   ; prologue: 11 arg bytes, 134 local bytes
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

   LDA   #$FF
   LDY   #$01
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
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
   LDY   #$07
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$08
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   EOR   g4
   LDY   #$09
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   EOR   g4+1
   LDY   #$0A
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@0
   LDY   #$91
   LDA   (FP),Y
   LDY   #$0B
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@10
   LDA   #$00
   JMP   .sx_done@11
.sx_neg@10:
   LDA   #$FF
.sx_done@11:
   LDY   #$0C
   STA   (FP),Y
   LDA   #$86
   INY
   STA   (FP),Y
   LDA   #$7B
   INY
   STA   (FP),Y
   LDY   #$0C
   LDA   (FP),Y
   LDY   #$0E
   CMP   (FP),Y
   BNE   .thread@0
   LDY   #$0B
   LDA   (FP),Y
   LDY   #$0D
   CMP   (FP),Y
   BEQ   .cmp_true@12
.thread@0:
   LDA   #$00
   JMP   .cmp_end@13
.cmp_true@12:
   LDA   #$01
.cmp_end@13:
   LDY   #$0F
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   ga5+3
.if_end@0:
   LDY   #$04
   LDA   (FP),Y
   LDY   #$12
   STA   (FP),Y
   ASL
   LDA   (FP),Y
   ROR
   STA   (FP),Y
   LDY   #$03
   LDA   (FP),Y
   LDY   #$11
   STA   (FP),Y
   ROR
   STA   (FP),Y
   LDY   #$03
   STA   (FP),Y
   LDY   #$12
   LDA   (FP),Y
   LDY   #$04
   STA   (FP),Y
   LDY   #$11
   LDA   (FP),Y
   LDY   #$13
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$14
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@15
   LDA   #$00
   JMP   .sx_done@16
.sx_neg@15:
   LDA   #$FF
.sx_done@16:
   LDY   #$15
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
   LDA   g3
   LDY   #$17
   STA   (FP),Y
   LDA   g3
   CLC
   ADC   #$01
   INY
   STA   (FP),Y
   AND   #$7F
   INY
   STA   (FP),Y
   STA   g3
   LDY   #$17
   LDA   (FP),Y
   LDY   #$1A
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
   CMP   #$03
   BEQ   .cmp_true@17
   LDA   #$00
   JMP   .cmp_end@18
.cmp_true@17:
   LDA   #$01
.cmp_end@18:
   LDY   #$20
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .case@1
   LDY   #$1E
   LDA   (FP),Y
   CMP   #$04
   BEQ   .cmp_true@19
   LDA   #$00
   JMP   .cmp_end@20
.cmp_true@19:
   LDA   #$01
.cmp_end@20:
   LDY   #$22
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
   CMP   #$05
   BEQ   .cmp_true@21
   LDA   #$00
   JMP   .cmp_end@22
.cmp_true@21:
   LDA   #$01
.cmp_end@22:
   LDY   #$24
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@1
   JMP   .switch@0_break
.lb_skip@1:
   LDY   #$1E
   LDA   (FP),Y
   CMP   #$07
   BEQ   .cmp_true@23
   LDA   #$00
   JMP   .cmp_end@24
.cmp_true@23:
   LDA   #$01
.cmp_end@24:
   LDY   #$26
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@0
   JMP   .case@4
.lb_skip@0:
   JMP   .default@5
.case@1:
.case@2:
   LDY   #$92
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$28
   STA   (FP),Y
   INY
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@25
   LDA   #$00
   JMP   .sx_done@26
.sx_neg@25:
   LDA   #$FF
.sx_done@26:
   LDY   #$2A
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$2B
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDA   #$00
   LDY   #$2C
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$2D
   STA   (FP),Y
   LDY   #$92
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$2D
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$2D
   LDA   (FP),Y
   INY
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@27
   LDA   #$00
   JMP   .sx_done@28
.sx_neg@27:
   LDA   #$FF
.sx_done@28:
   LDY   #$2F
   STA   (FP),Y
   LDY   #$03
   LDA   (FP),Y
   SEC
   LDY   #$2E
   SBC   (FP),Y
   LDY   #$30
   STA   (FP),Y
   LDY   #$04
   LDA   (FP),Y
   LDY   #$2F
   SBC   (FP),Y
   LDY   #$31
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$32
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$33
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@29
   LDA   #$00
   JMP   .sx_done@30
.sx_neg@29:
   LDA   #$FF
.sx_done@30:
   LDY   #$34
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
   LDY   #$32
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$33
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$34
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$35
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   JSR   mix
   LDA   g1
   LDY   #$36
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   SEC
   DEY
   SBC   (FP),Y
   LDY   #$38
   STA   (FP),Y
   LDA   #$00
   DEY
   SBC   (FP),Y
   LDY   #$39
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_else@2
   LDA   g1
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
   LDY   #$3D
   STA   (FP),Y
   LDY   #$3A
   LDA   (FP),Y
   ASL
   ASL
   ASL
   LDY   #$3C
   STA   (FP),Y
   INY
   LDA   (FP),Y
   TAX
   DEY
   LDA   (FP),Y

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$93
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
.if_else@2:
   LDA   gp7
   STA   DPTR
   LDA   gp7+1
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$46
   STA   (FP),Y
   SEC
   SBC   #$01
   INY
   STA   (FP),Y
   AND   #$7F
   INY
   STA   (FP),Y
   LDA   gp7
   STA   DPTR
   LDA   gp7+1
   STA   DPTR+1
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   JMP   .switch@0_break
.case@4:
   LDA   g3
   LDY   #$49
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$92
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$4B
   STA   (FP),Y
   INY
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@31
   LDA   #$00
   JMP   .sx_done@32
.sx_neg@31:
   LDA   #$FF
.sx_done@32:
   LDY   #$4D
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$05
   LDY   #$4E
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$4F
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$50
   STA   (FP),Y
   LDY   #$92
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$50
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$50
   LDA   (FP),Y
   INY
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@33
   LDA   #$00
   JMP   .sx_done@34
.sx_neg@33:
   LDA   #$FF
.sx_done@34:
   LDY   #$52
   STA   (FP),Y
   LDY   #$49
   LDA   (FP),Y
   LDY   #$51
   EOR   (FP),Y
   LDY   #$53
   STA   (FP),Y
   LDY   #$4A
   LDA   (FP),Y
   LDY   #$52
   EOR   (FP),Y
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
   ORA   #$00
   BMI   .sx_neg@35
   LDA   #$00
   JMP   .sx_done@36
.sx_neg@35:
   LDA   #$FF
.sx_done@36:
   LDY   #$57
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
   LDY   #$55
   LDA   (FP),Y
   LDY   #$89
   STA   (FP),Y
   LDY   #$56
   LDA   (FP),Y
   LDY   #$8A
   STA   (FP),Y
   LDY   #$57
   LDA   (FP),Y
   LDY   #$8B
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
   JMP   .switch@0_break
.default@5:
   LDY   #$03
   LDA   (FP),Y
   LDY   #$5D
   STA   (FP),Y
   LDY   #$04
   LDA   (FP),Y
   LDY   #$5E
   STA   (FP),Y
   LDX   #$05
.shift_loop@37:
   LDY   #$5D
   LDA   (FP),Y
   ASL
   STA   (FP),Y
   INY
   LDA   (FP),Y
   ROL
   STA   (FP),Y
   DEX
   BNE   .shift_loop@37
   DEY
   LDA   (FP),Y
   LDY   #$03
   STA   (FP),Y
   LDY   #$5E
   LDA   (FP),Y
   LDY   #$04
   STA   (FP),Y
   LDA   #$7F
   LDY   #$5D
   EOR   (FP),Y
   LDY   #$5F
   STA   (FP),Y
   LDA   #$00
   DEY
   EOR   (FP),Y
   LDY   #$60
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$61
   STA   (FP),Y
   LDY   #$92
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$61
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
.switch@0_break:
   LDA   g2
   SEC
   SBC   #$01
   LDY   #$62
   STA   (FP),Y
   LDA   g2+1
   SBC   #$00
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
   LDY   #$64
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$65
   STA   (FP),Y
   LDA   #$07
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$64
   LDA   (FP),Y
   LDY   #$66
   AND   (FP),Y
   LDY   #$68
   STA   (FP),Y
   LDY   #$65
   LDA   (FP),Y
   LDY   #$67
   AND   (FP),Y
   LDY   #$69
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CMP   #$03
   BEQ   .cmp_true@39
   LDA   #$00
   JMP   .cmp_end@40
.cmp_true@39:
   LDA   #$01
.cmp_end@40:
   LDY   #$6A
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .switch@6_break
   LDY   #$68
   LDA   (FP),Y
   CMP   #$04
   BEQ   .cmp_true@41
   LDA   #$00
   JMP   .cmp_end@42
.cmp_true@41:
   LDA   #$01
.cmp_end@42:
   LDY   #$6C
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .switch@6_break
   LDY   #$68
   LDA   (FP),Y
   CMP   #$05
   BEQ   .cmp_true@43
   LDA   #$00
   JMP   .cmp_end@44
.cmp_true@43:
   LDA   #$01
.cmp_end@44:
   LDY   #$6E
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .switch@6_break
   LDY   #$68
   LDA   (FP),Y
   CMP   #$06
   BEQ   .cmp_true@45
   LDA   #$00
   JMP   .cmp_end@46
.cmp_true@45:
   LDA   #$01
.cmp_end@46:
   LDY   #$70
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
.switch@6_break:
   LDA   #$02
   LDY   #$72
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$75
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$76
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@47
   LDA   #$00
   JMP   .sx_done@48
.sx_neg@47:
   LDA   #$FF
.sx_done@48:
   LDY   #$77
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
   LDY   #$89
   LDA   (FP),Y
   LDY   #$75
   ORA   (FP),Y
   LDY   #$7D
   STA   (FP),Y
   LDY   #$8A
   LDA   (FP),Y
   LDY   #$76
   ORA   (FP),Y
   LDY   #$7E
   STA   (FP),Y
   LDY   #$8B
   LDA   (FP),Y
   LDY   #$77
   ORA   (FP),Y
   LDY   #$7F
   STA   (FP),Y
   LDY   #$8C
   LDA   (FP),Y
   LDY   #$78
   ORA   (FP),Y
   LDY   #$80
   STA   (FP),Y
   LDY   #$8D
   LDA   (FP),Y
   LDY   #$79
   ORA   (FP),Y
   LDY   #$81
   STA   (FP),Y
   LDY   #$8E
   LDA   (FP),Y
   LDY   #$7A
   ORA   (FP),Y
   LDY   #$82
   STA   (FP),Y
   LDY   #$8F
   LDA   (FP),Y
   LDY   #$7B
   ORA   (FP),Y
   LDY   #$83
   STA   (FP),Y
   LDY   #$90
   LDA   (FP),Y
   LDY   #$7C
   ORA   (FP),Y
   LDY   #$84
   STA   (FP),Y
   LDY   #$7D
   LDA   (FP),Y
   LDY   #$85
   STA   (FP),Y
   LDY   #$7E
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
   ADC   #$93
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

   ; prologue: 0 arg bytes, 174 local bytes
   SEC
   LDA   SSP
   SBC   #$B0
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$AF
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
   LDA   #$80
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
   LDA   g1
   INY
   STA   (FP),Y
   LDA   g1
   SEC
   SBC   #$01
   INY
   STA   (FP),Y
   STA   g1
   LDA   #$03
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@49
   LDA   #$00
   JMP   .sx_done@50
.sx_neg@49:
   LDA   #$FF
.sx_done@50:
   LDY   #$13
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$14
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$15
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
   ADC   #$02
   LDY   #$18
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$19
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$1A
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$1B
   STA   (FP),Y
   LDY   #$14
   LDA   (FP),Y
   STA   HARGS
   INY
   LDA   (FP),Y
   STA   HARGS+1
   LDY   #$1A
   LDA   (FP),Y
   STA   HARGS+2
   INY
   LDA   (FP),Y
   STA   HARGS+3
   JSR   mul16
   LDA   HARGS+4
   LDY   #$1C
   STA   (FP),Y
   LDA   HARGS+5
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$1E
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$1F
   STA   (FP),Y
   LDA   #$07
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$1E
   LDA   (FP),Y
   LDY   #$20
   AND   (FP),Y
   LDY   #$22
   STA   (FP),Y
   LDY   #$1F
   LDA   (FP),Y
   LDY   #$21
   AND   (FP),Y
   LDY   #$23
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   BEQ   .cmp_true@51
   LDA   #$00
   JMP   .cmp_end@52
.cmp_true@51:
   LDA   #$01
.cmp_end@52:
   LDY   #$24
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@4
   JMP   .switch@12_break
.lb_skip@4:
   LDA   #$00
   LDY   #$26
   STA   (FP),Y
   LDA   #$80
   INY
   STA   (FP),Y
   SEC
   LDA   #$00
   DEY
   SBC   (FP),Y
   LDY   #$28
   STA   (FP),Y
   LDA   #$00
   DEY
   SBC   (FP),Y
   LDY   #$29
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@3
   JMP   .switch@12_break
.lb_skip@3:
   LDY   #$0E
   LDA   (FP),Y
   LDY   #$51
   STA   (FP),Y
   SEC
   SBC   #$01
   INY
   STA   (FP),Y
   LDY   #$0E
   STA   (FP),Y
   LDY   #$51
   LDA   (FP),Y
   LDY   #$53
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@55
   LDA   #$00
   JMP   .sx_done@56
.sx_neg@55:
   LDA   #$FF
.sx_done@56:
   LDY   #$54
   STA   (FP),Y
   LDA   g3
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$53
   LDA   (FP),Y
   LDY   #$55
   ORA   (FP),Y
   LDY   #$57
   STA   (FP),Y
   LDY   #$54
   LDA   (FP),Y
   LDY   #$56
   ORA   (FP),Y
   LDY   #$58
   STA   (FP),Y
   DEY
   LDA   (FP),Y
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
   ADC   #$01
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
   LDA   #$00
   LDY   #$5E
   STA   (FP),Y
   INY
   STA   (FP),Y
.loop@15_start:
   LDY   #$60
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
   BVC   .cmp_novf@57
   EOR   #$80
.cmp_novf@57:
   ASL
   LDA   #$00
   ROL
   LDY   #$62
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .loop@15_break
   LDY   #$5F
   LDA   (FP),Y
   LDY   #$77
   STA   (FP),Y
   CLC
   ADC   #$01
   INY
   STA   (FP),Y
   LDY   #$5F
   STA   (FP),Y
   JMP   .loop@15_start
.loop@15_break:
.switch@12_break:
   LDA   #<gs6
   LDY   #$79
   STA   (FP),Y
   LDA   #>gs6
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$01
   LDY   #$7B
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$7C
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$7D
   STA   (FP),Y
   CLC
   ADC   #$01
   INY
   STA   (FP),Y
   LDY   #$7B
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$7E
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$7E
   LDA   (FP),Y
   INY
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@58
   LDA   #$00
   JMP   .sx_done@59
.sx_neg@58:
   LDA   #$FF
.sx_done@59:
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
   LDA   #<gs6
   INY
   STA   (FP),Y
   LDA   #>gs6
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$02
   LDY   #$85
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$86
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$87
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$88
   STA   (FP),Y
   LDY   #$81
   LDA   (FP),Y
   LDY   #$87
   EOR   (FP),Y
   LDY   #$89
   STA   (FP),Y
   LDY   #$82
   LDA   (FP),Y
   LDY   #$88
   EOR   (FP),Y
   LDY   #$8A
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@2
   JMP   .if_end@7
.lb_skip@2:
   LDY   #$01
   LDA   (FP),Y
   SEC
   SBC   #$01
   LDY   #$8B
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   SBC   #$00
   LDY   #$8C
   STA   (FP),Y
   LDY   #$03
   LDA   (FP),Y
   SBC   #$00
   LDY   #$8D
   STA   (FP),Y
   LDY   #$04
   LDA   (FP),Y
   SBC   #$00
   LDY   #$8E
   STA   (FP),Y
   LDY   #$05
   LDA   (FP),Y
   SBC   #$00
   LDY   #$8F
   STA   (FP),Y
   LDY   #$06
   LDA   (FP),Y
   SBC   #$00
   LDY   #$90
   STA   (FP),Y
   LDY   #$07
   LDA   (FP),Y
   SBC   #$00
   LDY   #$91
   STA   (FP),Y
   LDY   #$08
   LDA   (FP),Y
   SBC   #$00
   LDY   #$92
   STA   (FP),Y
   LDY   #$8B
   LDA   (FP),Y
   LDY   #$93
   STA   (FP),Y
   LDY   #$8C
   LDA   (FP),Y
   LDY   #$94
   STA   (FP),Y
   LDY   #$8D
   LDA   (FP),Y
   LDY   #$95
   STA   (FP),Y
   LDY   #$8E
   LDA   (FP),Y
   LDY   #$96
   STA   (FP),Y
   LDY   #$8F
   LDA   (FP),Y
   LDY   #$97
   STA   (FP),Y
   LDY   #$90
   LDA   (FP),Y
   LDY   #$98
   STA   (FP),Y
   LDY   #$91
   LDA   (FP),Y
   LDY   #$99
   STA   (FP),Y
   LDY   #$92
   LDA   (FP),Y
   AND   #$01
   LDY   #$9A
   STA   (FP),Y
   LDY   #$93
   LDA   (FP),Y
   LDY   #$9B
   STA   (FP),Y
   LDY   #$94
   LDA   (FP),Y
   LDY   #$9C
   STA   (FP),Y
   LDY   #$95
   LDA   (FP),Y
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
   LDY   #$98
   LDA   (FP),Y
   LDY   #$A0
   STA   (FP),Y
   LDY   #$99
   LDA   (FP),Y
   LDY   #$A1
   STA   (FP),Y
   LDY   #$9A
   LDA   (FP),Y
   EOR   #$01
   LDY   #$A2
   STA   (FP),Y
   LDY   #$9B
   LDA   (FP),Y
   LDY   #$A3
   STA   (FP),Y
   LDY   #$9C
   LDA   (FP),Y
   LDY   #$A4
   STA   (FP),Y
   LDY   #$9D
   LDA   (FP),Y
   LDY   #$A5
   STA   (FP),Y
   LDY   #$9E
   LDA   (FP),Y
   LDY   #$A6
   STA   (FP),Y
   LDY   #$9F
   LDA   (FP),Y
   LDY   #$A7
   STA   (FP),Y
   LDY   #$A0
   LDA   (FP),Y
   LDY   #$A8
   STA   (FP),Y
   LDY   #$A1
   LDA   (FP),Y
   LDY   #$A9
   STA   (FP),Y
   LDY   #$A2
   LDA   (FP),Y
   SEC
   SBC   #$01
   LDY   #$AA
   STA   (FP),Y
   LDY   #$A3
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   LDY   #$A4
   LDA   (FP),Y
   LDY   #$02
   STA   (FP),Y
   LDY   #$A5
   LDA   (FP),Y
   LDY   #$03
   STA   (FP),Y
   LDY   #$A6
   LDA   (FP),Y
   LDY   #$04
   STA   (FP),Y
   LDY   #$A7
   LDA   (FP),Y
   LDY   #$05
   STA   (FP),Y
   LDY   #$A8
   LDA   (FP),Y
   LDY   #$06
   STA   (FP),Y
   LDY   #$A9
   LDA   (FP),Y
   LDY   #$07
   STA   (FP),Y
   LDY   #$AA
   LDA   (FP),Y
   LDY   #$08
   STA   (FP),Y
.if_end@7:
   JSR   fin0
   JSR   fin1
   JSR   fin2
   LDA   acc
   LDY   #$AB
   STA   (FP),Y
   LDA   acc+1
   AND   #$7F
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$AD
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$AE
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
   ADC   #$B0
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$AF
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
   DC.B  $02

g2:
   DC.W  $0080

g3:
   DC.B  $7F

g4:
   DC.W  $007F

ga5:
   DC.B  $00, $03, $00, $FF

gs6:
   DS.B  1
   DC.B  $7F
   DC.W  $8000

gp7:
   DC.W  g3

@4.s13:
   DC.W  $0100
