SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
z5	EQU	$26

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

   ; prologue: 0 arg bytes, 13 local bytes
   SEC
   LDA   SSP
   SBC   #$0F
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$0E
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
   LDA   g2
   BMI   .sx_neg@2
   LDA   #$00
   JMP   .sx_done@3
.sx_neg@2:
   LDA   #$FF
.sx_done@3:
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
   SEC
   LDA   SSP
   SBC   #$04
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   g3
   LDY   #$01
   STA   (SSP),Y
   LDA   g3+1
   INY
   STA   (SSP),Y
   LDA   g3+2
   INY
   STA   (SSP),Y
   LDA   g3+3
   INY
   STA   (SSP),Y
   JSR   mix
   LDA   g4
   LDY   #$09
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
   LDY   #$0B
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

   ; epilogue
   CLC
   LDA   FP
   ADC   #$0F
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$0E
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
   LDA   z5
   LDY   #$01
   STA   (SSP),Y
   LDA   z5+1
   INY
   STA   (SSP),Y
   LDA   z5+2
   INY
   STA   (SSP),Y
   LDA   z5+3
   INY
   STA   (SSP),Y
   JSR   mix
   LDA   ga6
   LDY   #$01
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
   LDA   ga6+4
   LDY   #$05
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
   LDA   ga6+8
   LDY   #$09
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

fin2:
   SUBROUTINE

   ; prologue: 0 arg bytes, 21 local bytes
   SEC
   LDA   SSP
   SBC   #$17
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$16
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
   LDA   #<gs7
   LDY   #$0D
   STA   (FP),Y
   LDA   #>gs7
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$05
   LDY   #$0F
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$10
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$11
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
   LDY   #$12
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$13
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$14
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$15
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   JSR   mix

   ; epilogue
   CLC
   LDA   FP
   ADC   #$17
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$16
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
   LDY   #$07
   LDA   (FP),Y
   LDY   #$0B
   STA   (FP),Y
   LDY   #$08
   LDA   (FP),Y
   LDY   #$0C
   STA   (FP),Y
   LDY   #$09
   LDA   (FP),Y
   LDY   #$0D
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   gp9
   STA   DPTR
   LDA   gp9+1
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$0F
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$10
   STA   (FP),Y
   LDY   #$02
   LDA   (DPTR),Y
   LDY   #$11
   STA   (FP),Y
   LDY   #$03
   LDA   (DPTR),Y
   LDY   #$12
   STA   (FP),Y
   LDY   #$0F
   LDA   (FP),Y
   SEC
   SBC   #$01
   LDY   #$13
   STA   (FP),Y
   LDY   #$10
   LDA   (FP),Y
   SBC   #$00
   LDY   #$14
   STA   (FP),Y
   LDY   #$11
   LDA   (FP),Y
   SBC   #$00
   LDY   #$15
   STA   (FP),Y
   LDY   #$12
   LDA   (FP),Y
   SBC   #$00
   LDY   #$16
   STA   (FP),Y
   LDA   gp9
   STA   DPTR
   LDA   gp9+1
   STA   DPTR+1
   LDY   #$13
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$14
   LDA   (FP),Y
   LDY   #$01
   STA   (DPTR),Y
   LDY   #$15
   LDA   (FP),Y
   LDY   #$02
   STA   (DPTR),Y
   LDY   #$16
   LDA   (FP),Y
   LDY   #$03
   STA   (DPTR),Y
   LDY   #$0B
   LDA   (FP),Y
   LDY   #$0F
   EOR   (FP),Y
   LDY   #$17
   STA   (FP),Y
   LDY   #$0C
   LDA   (FP),Y
   LDY   #$10
   EOR   (FP),Y
   LDY   #$18
   STA   (FP),Y
   LDY   #$0D
   LDA   (FP),Y
   LDY   #$11
   EOR   (FP),Y
   LDY   #$19
   STA   (FP),Y
   LDY   #$0E
   LDA   (FP),Y
   LDY   #$12
   EOR   (FP),Y
   LDY   #$1A
   STA   (FP),Y
   LDY   #$17
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   INY
   ORA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@1
   JMP   .if_else@1
.lb_skip@1:
   LDA   gp9
   STA   DPTR
   LDA   gp9+1
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$1B
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$1C
   STA   (FP),Y
   LDY   #$02
   LDA   (DPTR),Y
   LDY   #$1D
   STA   (FP),Y
   LDY   #$03
   LDA   (DPTR),Y
   LDY   #$1E
   STA   (FP),Y
   LDY   #$1B
   LDA   (FP),Y
   SEC
   SBC   #$01
   LDY   #$1F
   STA   (FP),Y
   LDY   #$1C
   LDA   (FP),Y
   SBC   #$00
   LDY   #$20
   STA   (FP),Y
   LDY   #$1D
   LDA   (FP),Y
   SBC   #$00
   LDY   #$21
   STA   (FP),Y
   LDY   #$1E
   LDA   (FP),Y
   SBC   #$00
   LDY   #$22
   STA   (FP),Y
   LDA   gp9
   STA   DPTR
   LDA   gp9+1
   STA   DPTR+1
   LDY   #$1F
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$20
   LDA   (FP),Y
   LDY   #$01
   STA   (DPTR),Y
   LDY   #$21
   LDA   (FP),Y
   LDY   #$02
   STA   (DPTR),Y
   LDY   #$22
   LDA   (FP),Y
   LDY   #$03
   STA   (DPTR),Y
   LDA   #<gs7
   LDY   #$23
   STA   (FP),Y
   LDA   #>gs7
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$01
   LDY   #$25
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$26
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDA   z5
   LDY   #$00
   STA   (DPTR),Y
   LDA   z5+1
   INY
   STA   (DPTR),Y
   LDA   z5+2
   INY
   STA   (DPTR),Y
   LDA   z5+3
   INY
   STA   (DPTR),Y
   LDA   gp9
   STA   DPTR
   LDA   gp9+1
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$27
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$28
   STA   (FP),Y
   LDY   #$02
   LDA   (DPTR),Y
   LDY   #$29
   STA   (FP),Y
   LDY   #$03
   LDA   (DPTR),Y
   LDY   #$2A
   STA   (FP),Y
   LDA   #$05
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   ORA   #$00
   BPL   .sx_done@7
   LDA   #$FF
.sx_done@7:
   LDY   #$2D
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$27
   LDA   (FP),Y
   LDY   #$2B
   ORA   (FP),Y
   LDY   #$2F
   STA   (FP),Y
   LDY   #$28
   LDA   (FP),Y
   LDY   #$2C
   ORA   (FP),Y
   LDY   #$30
   STA   (FP),Y
   LDY   #$29
   LDA   (FP),Y
   LDY   #$2D
   ORA   (FP),Y
   LDY   #$31
   STA   (FP),Y
   LDY   #$2A
   LDA   (FP),Y
   LDY   #$2E
   ORA   (FP),Y
   LDY   #$32
   STA   (FP),Y
   LDA   gp9
   STA   DPTR
   LDA   gp9+1
   STA   DPTR+1
   LDY   #$2F
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$30
   LDA   (FP),Y
   LDY   #$01
   STA   (DPTR),Y
   LDY   #$31
   LDA   (FP),Y
   LDY   #$02
   STA   (DPTR),Y
   LDY   #$32
   LDA   (FP),Y
   LDY   #$03
   STA   (DPTR),Y
   LDY   #$2F
   LDA   (FP),Y
   LDY   #$33
   STA   (FP),Y
   STA   g4
   JMP   .if_end@0
.if_else@1:
   LDA   #<gs7
   LDY   #$34
   STA   (FP),Y
   LDA   #>gs7
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$05
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
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$38
   STA   (FP),Y
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
.mul_loop@8:
   BCC   .mul_skip@9
   CLC
   ADC   #$01
.mul_skip@9:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@8
   LDY   #$3C
   STA   (FP),Y
   LDA   HARGS
   DEY
   STA   (FP),Y
   LDY   #$3D
   STA   (FP),Y
   LDY   #$36
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$3D
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDA   #<gs7
   LDY   #$3E
   STA   (FP),Y
   LDA   #>gs7
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$01
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
   LDY   #$02
   LDA   (DPTR),Y
   LDY   #$44
   STA   (FP),Y
   LDY   #$03
   LDA   (DPTR),Y
   LDY   #$45
   STA   (FP),Y
   LDY   #$42
   LDA   (FP),Y
   SEC
   SBC   #$01
   LDY   #$46
   STA   (FP),Y
   LDY   #$43
   LDA   (FP),Y
   SBC   #$00
   LDY   #$47
   STA   (FP),Y
   LDY   #$44
   LDA   (FP),Y
   SBC   #$00
   LDY   #$48
   STA   (FP),Y
   LDY   #$45
   LDA   (FP),Y
   SBC   #$00
   LDY   #$49
   STA   (FP),Y
   LDY   #$40
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$46
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$47
   LDA   (FP),Y
   LDY   #$01
   STA   (DPTR),Y
   LDY   #$48
   LDA   (FP),Y
   LDY   #$02
   STA   (DPTR),Y
   LDY   #$49
   LDA   (FP),Y
   LDY   #$03
   STA   (DPTR),Y
.if_end@0:
   LDA   #<ga6
   LDY   #$4A
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
   LDY   #$4E
   STA   (FP),Y
   STA   HARGS
   LDA   #$04
   STA   HARGS+1
   JSR   mul8
   LDA   HARGS+2
   LDY   #$4F
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$4A
   LDA   (FP),Y
   CLC
   LDY   #$50
   ADC   (FP),Y
   LDY   #$52
   STA   (FP),Y
   LDY   #$4B
   LDA   (FP),Y
   LDY   #$51
   ADC   (FP),Y
   LDY   #$53
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$54
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$55
   STA   (FP),Y
   LDY   #$02
   LDA   (DPTR),Y
   LDY   #$56
   STA   (FP),Y
   LDY   #$03
   LDA   (DPTR),Y
   LDY   #$57
   STA   (FP),Y
   LDY   #$5B
   STA   (FP),Y
   LSR
   STA   (FP),Y
   LDY   #$56
   LDA   (FP),Y
   LDY   #$5A
   STA   (FP),Y
   ROR
   STA   (FP),Y
   LDY   #$55
   LDA   (FP),Y
   LDY   #$59
   STA   (FP),Y
   ROR
   STA   (FP),Y
   LDY   #$54
   LDA   (FP),Y
   LDY   #$58
   STA   (FP),Y
   ROR
   STA   (FP),Y
   LDY   #$5B
   LDA   (FP),Y
   LSR
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ROR
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ROR
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ROR
   STA   (FP),Y
   LDY   #$52
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$58
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$59
   LDA   (FP),Y
   LDY   #$01
   STA   (DPTR),Y
   LDY   #$5A
   LDA   (FP),Y
   LDY   #$02
   STA   (DPTR),Y
   LDY   #$5B
   LDA   (FP),Y
   LDY   #$03
   STA   (DPTR),Y
   LDA   #$01
   LDY   #$5C
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$5E
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$5F
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$58
   LDA   (FP),Y
   LDY   #$5E
   ORA   (FP),Y
   LDY   #$62
   STA   (FP),Y
   LDY   #$59
   LDA   (FP),Y
   LDY   #$5F
   ORA   (FP),Y
   LDY   #$63
   STA   (FP),Y
   LDY   #$5A
   LDA   (FP),Y
   LDY   #$60
   ORA   (FP),Y
   LDY   #$64
   STA   (FP),Y
   LDY   #$5B
   LDA   (FP),Y
   LDY   #$61
   ORA   (FP),Y
   LDY   #$65
   STA   (FP),Y
   LDY   #$62
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   INY
   ORA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@0
   JMP   .if_end@2
.lb_skip@0:
   LDA   #$00
   LDY   #$66
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$06
   INY
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
   LDY   #$66
   LDA   (FP),Y
   STA   HARGS
   INY
   LDA   (FP),Y
   STA   HARGS+1
   LDY   #$6A
   LDA   (FP),Y
   STA   HARGS+2
   INY
   LDA   (FP),Y
   STA   HARGS+3
   JSR   udivmod16
   LDA   HARGS+6
   LDY   #$6C
   STA   (FP),Y
   LDA   HARGS+7
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$6E
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$6F
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$03
   STA   (FP),Y
   LDY   #$6F
   LDA   (FP),Y
   LDY   #$04
   STA   (FP),Y
   LDA   g3
   CLC
   ADC   #$01
   LDY   #$70
   STA   (FP),Y
   LDA   g3+1
   ADC   #$00
   INY
   STA   (FP),Y
   LDA   g3+2
   ADC   #$00
   INY
   STA   (FP),Y
   LDA   g3+3
   ADC   #$00
   INY
   STA   (FP),Y
   LDY   #$70
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
   LDA   z5
   CLC
   LDY   #$70
   ADC   (FP),Y
   LDY   #$74
   STA   (FP),Y
   LDA   z5+1
   LDY   #$71
   ADC   (FP),Y
   LDY   #$75
   STA   (FP),Y
   LDA   z5+2
   LDY   #$72
   ADC   (FP),Y
   LDY   #$76
   STA   (FP),Y
   LDA   z5+3
   LDY   #$73
   ADC   (FP),Y
   LDY   #$77
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$04
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDY   #$74
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$75
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$76
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$77
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   JSR   mix
   LDA   #$05
   LDY   #$78
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$03
   LDA   (FP),Y
   STA   HARGS
   INY
   LDA   (FP),Y
   STA   HARGS+1
   LDY   #$78
   LDA   (FP),Y
   STA   HARGS+2
   INY
   LDA   (FP),Y
   STA   HARGS+3
   JSR   sdivmod16
   LDA   HARGS+4
   LDY   #$7A
   STA   (FP),Y
   LDA   HARGS+5
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$7C
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$7D
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@10
   LDA   #$00
   JMP   .sx_done@11
.sx_neg@10:
   LDA   #$FF
.sx_done@11:
   LDY   #$7E
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
   LDY   #$7C
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$7D
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$7E
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$7F
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   JSR   mix
.if_end@2:
   LDA   gp8
   STA   DPTR
   LDA   gp8+1
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$80
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$81
   STA   (FP),Y
   LDY   #$02
   LDA   (DPTR),Y
   LDY   #$82
   STA   (FP),Y
   LDY   #$03
   LDA   (DPTR),Y
   LDY   #$83
   STA   (FP),Y
   LDY   #$80
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
   LDY   #$84
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
   LDA   gp8
   STA   DPTR
   LDA   gp8+1
   STA   DPTR+1
   LDY   #$84
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$85
   LDA   (FP),Y
   LDY   #$01
   STA   (DPTR),Y
   LDY   #$86
   LDA   (FP),Y
   LDY   #$02
   STA   (DPTR),Y
   LDY   #$87
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
   LDY   #$84
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$85
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$86
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$87
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
   LDY   #$88
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$89
   STA   (FP),Y
   LDY   #$02
   LDA   (DPTR),Y
   LDY   #$8A
   STA   (FP),Y
   LDY   #$03
   LDA   (DPTR),Y
   LDY   #$8B
   STA   (FP),Y
   LDA   #$5A
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   ORA   #$00
   BPL   .sx_done@13
   LDA   #$FF
.sx_done@13:
   LDY   #$8E
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$88
   LDA   (FP),Y
   LDY   #$8C
   EOR   (FP),Y
   LDY   #$90
   STA   (FP),Y
   LDY   #$89
   LDA   (FP),Y
   LDY   #$8D
   EOR   (FP),Y
   LDY   #$91
   STA   (FP),Y
   LDY   #$8A
   LDA   (FP),Y
   LDY   #$8E
   EOR   (FP),Y
   LDY   #$92
   STA   (FP),Y
   LDY   #$8B
   LDA   (FP),Y
   LDY   #$8F
   EOR   (FP),Y
   LDY   #$93
   STA   (FP),Y
   LDA   gp8
   STA   DPTR
   LDA   gp8+1
   STA   DPTR+1
   LDY   #$90
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$91
   LDA   (FP),Y
   LDY   #$01
   STA   (DPTR),Y
   LDY   #$92
   LDA   (FP),Y
   LDY   #$02
   STA   (DPTR),Y
   LDY   #$93
   LDA   (FP),Y
   LDY   #$03
   STA   (DPTR),Y
   LDY   #$90
   LDA   (FP),Y
   LDY   #$94
   STA   (FP),Y
   LDY   #$91
   LDA   (FP),Y
   LDY   #$95
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   g1
   INY
   LDA   (FP),Y
   STA   g1+1
   LDY   #$04
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

f14:
   SUBROUTINE

   ; prologue: 2 arg bytes, 140 local bytes
   SEC
   LDA   SSP
   SBC   #$8E
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$8D
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
   LDA   #$7F
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
   LDA   g2
   BEQ   .cond_else@3
   LDY   #$02
   LDA   (FP),Y
   LDY   #$07
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
   JMP   .cond_end@4
.cond_else@3:
   LDA   #$02
   LDY   #$0B
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$09
   STA   (FP),Y
   LDY   #$0D
   LDA   (FP),Y
   LDY   #$0A
   STA   (FP),Y
.cond_end@4:
   LDA   #$02
   LDY   #$0E
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$09
   LDA   (FP),Y
   STA   HARGS
   INY
   LDA   (FP),Y
   STA   HARGS+1
   LDY   #$0E
   LDA   (FP),Y
   STA   HARGS+2
   INY
   LDA   (FP),Y
   STA   HARGS+3
   JSR   sdivmod16
   LDA   HARGS+4
   LDY   #$10
   STA   (FP),Y
   LDA   HARGS+5
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$12
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
   LDY   #$12
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDA   #$03
   LDY   #$17
   STA   (FP),Y
   INY
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
   LDY   #$18
   LDA   (FP),Y
   LDY   #$1A
   AND   (FP),Y
   LDY   #$1C
   STA   (FP),Y
   LDY   #$19
   LDA   (FP),Y
   LDY   #$1B
   AND   (FP),Y
   LDY   #$1D
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   BEQ   .cmp_true@14
   LDA   #$00
   JMP   .cmp_end@15
.cmp_true@14:
   LDA   #$01
.cmp_end@15:
   LDY   #$1E
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .case@1
   LDY   #$1C
   LDA   (FP),Y
   CMP   #$02
   BEQ   .cmp_true@16
   LDA   #$00
   JMP   .cmp_end@17
.cmp_true@16:
   LDA   #$01
.cmp_end@17:
   LDY   #$20
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
   LDY   #$1C
   LDA   (FP),Y
   CMP   #$05
   BEQ   .cmp_true@18
   LDA   #$00
   JMP   .cmp_end@19
.cmp_true@18:
   LDA   #$01
.cmp_end@19:
   LDY   #$22
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
   JMP   .default@11
.case@1:
   LDA   z5
   LDY   #$24
   STA   (FP),Y
   LDA   z5+1
   INY
   STA   (FP),Y
   LDA   #$07
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$24
   LDA   (FP),Y
   LDY   #$26
   AND   (FP),Y
   LDY   #$28
   STA   (FP),Y
   LDY   #$25
   LDA   (FP),Y
   LDY   #$27
   AND   (FP),Y
   LDY   #$29
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CMP   #$03
   BEQ   .cmp_true@20
   LDA   #$00
   JMP   .cmp_end@21
.cmp_true@20:
   LDA   #$01
.cmp_end@21:
   LDY   #$2A
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .case@3
   LDY   #$28
   LDA   (FP),Y
   CMP   #$04
   BEQ   .cmp_true@22
   LDA   #$00
   JMP   .cmp_end@23
.cmp_true@22:
   LDA   #$01
.cmp_end@23:
   LDY   #$2C
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@4
   JMP   .case@4
.lb_skip@4:
   LDY   #$28
   LDA   (FP),Y
   CMP   #$05
   BEQ   .cmp_true@24
   LDA   #$00
   JMP   .cmp_end@25
.cmp_true@24:
   LDA   #$01
.cmp_end@25:
   LDY   #$2E
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@3
   JMP   .case@6
.lb_skip@3:
   LDY   #$28
   LDA   (FP),Y
   CMP   #$06
   BEQ   .cmp_true@26
   LDA   #$00
   JMP   .cmp_end@27
.cmp_true@26:
   LDA   #$01
.cmp_end@27:
   LDY   #$30
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@2
   JMP   .case@7
.lb_skip@2:
   JMP   .default@8
.case@3:
   LDA   #<gs7
   LDY   #$32
   STA   (FP),Y
   LDA   #>gs7
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$01
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
   LDY   #$02
   LDA   (DPTR),Y
   LDY   #$38
   STA   (FP),Y
   LDY   #$03
   LDA   (DPTR),Y
   LDY   #$39
   STA   (FP),Y
   LDA   #$03
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   ORA   #$00
   BPL   .sx_done@29
   LDA   #$FF
.sx_done@29:
   LDY   #$3C
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$36
   LDA   (FP),Y
   SEC
   LDY   #$3A
   SBC   (FP),Y
   LDY   #$3E
   STA   (FP),Y
   LDY   #$37
   LDA   (FP),Y
   LDY   #$3B
   SBC   (FP),Y
   LDY   #$3F
   STA   (FP),Y
   LDY   #$38
   LDA   (FP),Y
   LDY   #$3C
   SBC   (FP),Y
   LDY   #$40
   STA   (FP),Y
   LDY   #$39
   LDA   (FP),Y
   LDY   #$3D
   SBC   (FP),Y
   LDY   #$41
   STA   (FP),Y
   LDY   #$34
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$3E
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$3F
   LDA   (FP),Y
   LDY   #$01
   STA   (DPTR),Y
   LDY   #$40
   LDA   (FP),Y
   LDY   #$02
   STA   (DPTR),Y
   LDY   #$41
   LDA   (FP),Y
   LDY   #$03
   STA   (DPTR),Y
   LDA   gp9
   STA   DPTR
   LDA   gp9+1
   STA   DPTR+1
   LDY   #$3E
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$3F
   LDA   (FP),Y
   LDY   #$01
   STA   (DPTR),Y
   LDY   #$40
   LDA   (FP),Y
   LDY   #$02
   STA   (DPTR),Y
   LDY   #$41
   LDA   (FP),Y
   LDY   #$03
   STA   (DPTR),Y
   JMP   .switch@0_break
.case@4:
   LDA   #$00
   LDY   #$42
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
   BVC   .cmp_novf@30
   EOR   #$80
.cmp_novf@30:
   ASL
   LDA   #$00
   ROL
   LDY   #$46
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   JSR   f10
   LDY   #$71
   STA   (FP),Y
   TXA
   INY
   STA   (FP),Y
   JMP   .switch@0_break
.case@6:
   LDA   #<gs7
   LDY   #$73
   STA   (FP),Y
   LDA   #>gs7
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$05
   LDY   #$75
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$76
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$77
   STA   (FP),Y
   INY
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
   ADC   #$90
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$8D
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
.case@7:
.default@8:
   LDA   g2
   LDY   #$7A
   STA   (FP),Y
   LDA   g2
   BMI   .sx_neg@37
   LDA   #$00
   JMP   .sx_done@38
.sx_neg@37:
   LDA   #$FF
.sx_done@38:
   LDY   #$7B
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$7C
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$7D
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@39
   LDA   #$00
   JMP   .sx_done@40
.sx_neg@39:
   LDA   #$FF
.sx_done@40:
   LDY   #$7E
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$7C
   LDA   (FP),Y
   EOR   z5
   LDY   #$80
   STA   (FP),Y
   LDY   #$7D
   LDA   (FP),Y
   EOR   z5+1
   LDY   #$81
   STA   (FP),Y
   LDY   #$7E
   LDA   (FP),Y
   EOR   z5+2
   LDY   #$82
   STA   (FP),Y
   LDY   #$7F
   LDA   (FP),Y
   EOR   z5+3
   LDY   #$83
   STA   (FP),Y
   LDY   #$80
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
   LDA   #$00
   STA   HARGS+4
   JSR   asl32
   LDA   HARGS+5
   LDY   #$84
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
   LDY   #$84
   LDA   (FP),Y
   LDY   #$88
   STA   (FP),Y
   LDY   #$85
   LDA   (FP),Y
   LDY   #$89
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$05
   STA   (FP),Y
   LDY   #$89
   LDA   (FP),Y
   LDY   #$06
   STA   (FP),Y
.case@9:
.case@10:
.default@11:
.switch@0_break:
   LDA   g3
   LDY   #$8A
   STA   (FP),Y
   INY
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@41
   LDA   #$00
   JMP   .sx_done@42
.sx_neg@41:
   LDA   #$FF
.sx_done@42:
   LDY   #$8C
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
   ADC   #$90
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$8D
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

   ; prologue: 0 arg bytes, 117 local bytes
   SEC
   LDA   SSP
   SBC   #$77
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$76
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
   LDA   #$00
   INY
   STA   (FP),Y
   LDA   #$7F
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   g4
   INY
   STA   (FP),Y
   ORA   #$00
   BEQ   .lnot_true@43
   LDA   #$00
   JMP   .lnot_end@44
.lnot_true@43:
   LDA   #$01
.lnot_end@44:
   LDY   #$08
   STA   (FP),Y
   LDA   #$00
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
   LDY   #$09
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   JSR   f14
   LDY   #$0A
   STA   (FP),Y
   TXA
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$0C
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$0D
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@45
   LDA   #$00
   JMP   .sx_done@46
.sx_neg@45:
   LDA   #$FF
.sx_done@46:
   LDY   #$0E
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
   JSR   f10
   LDY   #$10
   STA   (FP),Y
   TXA
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
   LDY   #$14
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$15
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$16
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$17
   STA   (FP),Y
   LDY   #$02
   LDA   (DPTR),Y
   LDY   #$18
   STA   (FP),Y
   LDY   #$03
   LDA   (DPTR),Y
   LDY   #$19
   STA   (FP),Y
   LDA   #$FF
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   ORA   #$00
   BPL   .sx_done@48
   LDA   #$FF
.sx_done@48:
   LDY   #$1C
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$16
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
   LDY   #$1E
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
   LDY   #$14
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$1E
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$1F
   LDA   (FP),Y
   LDY   #$01
   STA   (DPTR),Y
   LDY   #$20
   LDA   (FP),Y
   LDY   #$02
   STA   (DPTR),Y
   LDY   #$21
   LDA   (FP),Y
   LDY   #$03
   STA   (DPTR),Y
   LDY   #$1E
   LDA   (FP),Y
   LDY   #$22
   STA   (FP),Y
   LDY   #$1F
   LDA   (FP),Y
   LDY   #$23
   STA   (FP),Y
   LDA   #$07
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$22
   LDA   (FP),Y
   LDY   #$24
   AND   (FP),Y
   LDY   #$26
   STA   (FP),Y
   LDY   #$23
   LDA   (FP),Y
   LDY   #$25
   AND   (FP),Y
   LDY   #$27
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CMP   #$01
   BEQ   .cmp_true@49
   LDA   #$00
   JMP   .cmp_end@50
.cmp_true@49:
   LDA   #$01
.cmp_end@50:
   LDY   #$28
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@9
   JMP   .switch@12_break
.lb_skip@9:
   LDY   #$26
   LDA   (FP),Y
   CMP   #$03
   BEQ   .cmp_true@51
   LDA   #$00
   JMP   .cmp_end@52
.cmp_true@51:
   LDA   #$01
.cmp_end@52:
   LDY   #$2A
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .case@14
   LDY   #$26
   LDA   (FP),Y
   CMP   #$04
   BEQ   .cmp_true@53
   LDA   #$00
   JMP   .cmp_end@54
.cmp_true@53:
   LDA   #$01
.cmp_end@54:
   LDY   #$2C
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .case@15
   LDY   #$26
   LDA   (FP),Y
   CMP   #$07
   BEQ   .cmp_true@55
   LDA   #$00
   JMP   .cmp_end@56
.cmp_true@55:
   LDA   #$01
.cmp_end@56:
   LDY   #$2E
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@8
   JMP   .case@16
.lb_skip@8:
   JMP   .switch@12_break
.case@14:
.case@15:
   LDA   #$00
   LDY   #$30
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$01
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$04
   LDA   (FP),Y
   LDY   #$34
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@57
   LDA   #$00
   JMP   .sx_done@58
.sx_neg@57:
   LDA   #$FF
.sx_done@58:
   LDY   #$35
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$36
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$37
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@59
   LDA   #$00
   JMP   .sx_done@60
.sx_neg@59:
   LDA   #$FF
.sx_done@60:
   LDY   #$38
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$30
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
   LDY   #$36
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
   LDY   #$3A
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
   LDY   #$02
   LDA   (DPTR),Y
   LDY   #$44
   STA   (FP),Y
   LDY   #$03
   LDA   (DPTR),Y
   LDY   #$45
   STA   (FP),Y
   LDA   g3
   LDY   #$42
   AND   (FP),Y
   LDY   #$46
   STA   (FP),Y
   LDA   g3+1
   LDY   #$43
   AND   (FP),Y
   LDY   #$47
   STA   (FP),Y
   LDA   g3+2
   LDY   #$44
   AND   (FP),Y
   LDY   #$48
   STA   (FP),Y
   LDA   g3+3
   LDY   #$45
   AND   (FP),Y
   LDY   #$49
   STA   (FP),Y
   LDY   #$3A
   LDA   (FP),Y
   LDY   #$46
   EOR   (FP),Y
   LDY   #$4A
   STA   (FP),Y
   LDY   #$3B
   LDA   (FP),Y
   LDY   #$47
   EOR   (FP),Y
   LDY   #$4B
   STA   (FP),Y
   LDY   #$3C
   LDA   (FP),Y
   LDY   #$48
   EOR   (FP),Y
   LDY   #$4C
   STA   (FP),Y
   LDY   #$3D
   LDA   (FP),Y
   LDY   #$49
   EOR   (FP),Y
   LDY   #$4D
   STA   (FP),Y
   LDY   #$4A
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   INY
   ORA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@7
   JMP   .if_else@6
.lb_skip@7:
   LDA   ga6
   LDY   #$4E
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
   LDA   ga6
   INY
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
   LDY   #$4E
   LDA   (FP),Y
   LDY   #$52
   AND   (FP),Y
   LDY   #$56
   STA   (FP),Y
   LDY   #$4F
   LDA   (FP),Y
   LDY   #$53
   AND   (FP),Y
   LDY   #$57
   STA   (FP),Y
   LDY   #$50
   LDA   (FP),Y
   LDY   #$54
   AND   (FP),Y
   LDY   #$58
   STA   (FP),Y
   LDY   #$51
   LDA   (FP),Y
   LDY   #$55
   AND   (FP),Y
   LDY   #$59
   STA   (FP),Y
   LDY   #$56
   LDA   (FP),Y
   LDY   #$5A
   STA   (FP),Y
   STA   g4
   LDA   g3
   ORA   g3+1
   ORA   g3+2
   ORA   g3+3
   BEQ   .if_else@8
   JSR   f10
   LDY   #$5B
   STA   (FP),Y
   TXA
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   LDY   #$5C
   LDA   (FP),Y
   LDY   #$02
   STA   (FP),Y
   JMP   .if_end@7
.if_else@8:
   LDY   #$01
   LDA   (FP),Y
   CLC
   ADC   #$01
   LDY   #$5D
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   ADC   #$00
   LDY   #$5E
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   LDY   #$5E
   LDA   (FP),Y
   LDY   #$02
   STA   (FP),Y
   LDY   #$06
   LDA   (FP),Y
   LDY   #$5F
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
   LDY   #$61
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
   LDY   #$60
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$61
   LDA   (FP),Y
   LDY   #$01
   STA   (DPTR),Y
   LDY   #$62
   LDA   (FP),Y
   LDY   #$02
   STA   (DPTR),Y
   LDY   #$63
   LDA   (FP),Y
   LDY   #$03
   STA   (DPTR),Y
.if_end@7:
   LDA   g1
   SEC
   SBC   #$01
   LDY   #$68
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
   LDA   #$7F
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$68
   LDA   (FP),Y
   CLC
   LDY   #$6B
   ADC   (FP),Y
   LDY   #$6D
   STA   (FP),Y
   LDY   #$69
   LDA   (FP),Y
   LDY   #$6C
   ADC   (FP),Y
   LDY   #$6E
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@9
   JSR   f10
   LDY   #$6F
   STA   (FP),Y
   TXA
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$71
   STA   (FP),Y
   LDY   #$06
   STA   (FP),Y
.if_end@9:
.if_else@6:
.case@16:
.switch@12_break:
   JSR   fin0
   JSR   fin1
   JSR   fin2
   LDA   acc
   LDY   #$72
   STA   (FP),Y
   LDA   acc+1
   AND   #$7F
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
   TAX
   DEY
   LDA   (FP),Y

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$77
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$76
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
   DC.W  $7FFF

g2:
   DC.B  $02

g3:
   DC.L  $7FFFFFFF

g4:
   DC.B  $FF

ga6:
   DC.L  $00000002
   DS.B  4
   DC.L  $00007FFF
   DS.B  4

gs7:
   DS.B  1
   DC.L  $58752AD9
   DS.B  1

gp8:
   DC.W  z5

gp9:
   DC.W  g3

@5.s18:
   DC.L  $2810FC09

__zp_init:
   SUBROUTINE

   LDA   #$00
   STA   z5+1
   STA   z5+2
   STA   z5+3
   LDA   #$0C
   STA   z5
   RTS
