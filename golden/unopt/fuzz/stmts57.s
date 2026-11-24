SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
z4	EQU	$26
z5	EQU	$28
z6	EQU	$29

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
   BMI   .sx_neg@2
   LDA   #$00
   JMP   .sx_done@3
.sx_neg@2:
   LDA   #$FF
.sx_done@3:
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
   LDA   z5
   BMI   .sx_neg@4
   LDA   #$00
   JMP   .sx_done@5
.sx_neg@4:
   LDA   #$FF
.sx_done@5:
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
   LDA   z6
   LDY   #$05
   STA   (FP),Y
   LDA   z6+1
   INY
   STA   (FP),Y
   LDA   z6+1
   BMI   .sx_neg@6
   LDA   #$00
   JMP   .sx_done@7
.sx_neg@6:
   LDA   #$FF
.sx_done@7:
   LDY   #$07
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
   LDA   ga7
   LDY   #$09
   STA   (FP),Y
   LDA   ga7+1
   INY
   STA   (FP),Y
   LDA   ga7+2
   INY
   STA   (FP),Y
   LDA   ga7+3
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
   LDA   ga7+4
   LDY   #$0D
   STA   (FP),Y
   LDA   ga7+5
   INY
   STA   (FP),Y
   LDA   ga7+6
   INY
   STA   (FP),Y
   LDA   ga7+7
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

   LDA   ga7+8
   LDY   #$01
   STA   (FP),Y
   LDA   ga7+9
   INY
   STA   (FP),Y
   LDA   ga7+10
   INY
   STA   (FP),Y
   LDA   ga7+11
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
   LDA   ga7+12
   LDY   #$05
   STA   (FP),Y
   LDA   ga7+13
   INY
   STA   (FP),Y
   LDA   ga7+14
   INY
   STA   (FP),Y
   LDA   ga7+15
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
   LDA   #<gs8
   LDY   #$09
   STA   (FP),Y
   LDA   #>gs8
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$01
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
   LDA   #<gs8
   LDY   #$13
   STA   (FP),Y
   LDA   #>gs8
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
   INY
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@8
   LDA   #$00
   JMP   .sx_done@9
.sx_neg@8:
   LDA   #$FF
.sx_done@9:
   LDY   #$19
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

f9:
   SUBROUTINE

   ; prologue: 3 arg bytes, 73 local bytes
   SEC
   LDA   SSP
   SBC   #$4B
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$4A
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
   DEY
   LDA   (FP),Y
   LDY   #$03
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$04
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$05
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$06
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@10
   LDA   #$00
   JMP   .sx_done@11
.sx_neg@10:
   LDA   #$FF
.sx_done@11:
   LDY   #$07
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   ga7+8
   INY
   STA   (FP),Y
   LDA   ga7+9
   INY
   STA   (FP),Y
   LDA   ga7+10
   INY
   STA   (FP),Y
   LDA   ga7+11
   INY
   STA   (FP),Y
   LDY   #$05
   LDA   (FP),Y
   CLC
   LDY   #$09
   ADC   (FP),Y
   LDY   #$0D
   STA   (FP),Y
   LDY   #$06
   LDA   (FP),Y
   LDY   #$0A
   ADC   (FP),Y
   LDY   #$0E
   STA   (FP),Y
   LDY   #$07
   LDA   (FP),Y
   LDY   #$0B
   ADC   (FP),Y
   LDY   #$0F
   STA   (FP),Y
   LDY   #$08
   LDA   (FP),Y
   LDY   #$0C
   ADC   (FP),Y
   LDY   #$10
   STA   (FP),Y
   LDA   #$04
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$13
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$14
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@12
   LDA   #$00
   JMP   .sx_done@13
.sx_neg@12:
   LDA   #$FF
.sx_done@13:
   LDY   #$15
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$0D
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
   LDY   #$13
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
   JSR   udivmod32
   LDA   HARGS+8
   LDY   #$17
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
   LDA   g3
   STA   HARGS
   LDA   g3+1
   STA   HARGS+1
   LDA   g3+2
   STA   HARGS+2
   LDA   g3+3
   STA   HARGS+3
   LDA   #$03
   STA   HARGS+4
   JSR   asr32
   LDA   HARGS+5
   LDY   #$1B
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
   LDY   #$1E
   LDA   (FP),Y
   LDY   #$22
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$04
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDY   #$1F
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$20
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$21
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$22
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   JSR   mix
   LDY   #$03
   LDA   (FP),Y
   LDY   #$23
   STA   (FP),Y
   LDY   #$04
   LDA   (FP),Y
   LDY   #$24
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@14
   LDA   #$00
   JMP   .sx_done@15
.sx_neg@14:
   LDA   #$FF
.sx_done@15:
   LDY   #$25
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   g3
   CLC
   ADC   #$01
   INY
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
   LDY   #$27
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
   LDY   #$23
   LDA   (FP),Y
   LDY   #$27
   AND   (FP),Y
   LDY   #$2B
   STA   (FP),Y
   LDY   #$24
   LDA   (FP),Y
   LDY   #$28
   AND   (FP),Y
   LDY   #$2C
   STA   (FP),Y
   LDY   #$25
   LDA   (FP),Y
   LDY   #$29
   AND   (FP),Y
   LDY   #$2D
   STA   (FP),Y
   LDY   #$26
   LDA   (FP),Y
   LDY   #$2A
   AND   (FP),Y
   LDY   #$2E
   STA   (FP),Y
   LDY   #$2B
   LDA   (FP),Y
   LDY   #$2F
   STA   (FP),Y
   LDY   #$2C
   LDA   (FP),Y
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
   SEC
   LDA   SSP
   SBC   #$04
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDY   #$2F
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$30
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$31
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$32
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   JSR   mix
   LDY   #$4D
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$33
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$34
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ORA   #$5A
   LDY   #$35
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$36
   STA   (FP),Y
   LDY   #$4D
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$35
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$36
   LDA   (FP),Y
   LDY   #$01
   STA   (DPTR),Y
   LDA   #<gs8
   LDY   #$37
   STA   (FP),Y
   LDA   #>gs8
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$03
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
   INY
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@16
   LDA   #$00
   JMP   .sx_done@17
.sx_neg@16:
   LDA   #$FF
.sx_done@17:
   LDY   #$3D
   STA   (FP),Y
   LDY   #$35
   LDA   (FP),Y
   LDY   #$3C
   ORA   (FP),Y
   LDY   #$3E
   STA   (FP),Y
   LDY   #$36
   LDA   (FP),Y
   LDY   #$3D
   ORA   (FP),Y
   LDY   #$3F
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$40
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$41
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@18
   LDA   #$00
   JMP   .sx_done@19
.sx_neg@18:
   LDA   #$FF
.sx_done@19:
   LDY   #$42
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
   LDY   #$40
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$41
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$42
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$43
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   JSR   mix
   LDY   #$4C
   LDA   (FP),Y
   LDY   #$44
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$47
   STA   (FP),Y
   LSR
   STA   (FP),Y
   LDY   #$44
   LDA   (FP),Y
   LDY   #$46
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
   LDY   #$48
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$49
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
   ADC   #$4E
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$4A
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

   ; prologue: 17 arg bytes, 84 local bytes
   SEC
   LDA   SSP
   SBC   #$56
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$55
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
   LDA   #$98
   INY
   STA   (FP),Y
   LDA   #$AA
   INY
   STA   (FP),Y
   LDA   #$DD
   INY
   STA   (FP),Y
   LDA   #$69
   INY
   STA   (FP),Y
   LDA   #$40
   INY
   STA   (FP),Y
   LDA   #$83
   INY
   STA   (FP),Y
   LDA   #$CA
   INY
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
   DEY
   LDA   (FP),Y
   LDY   #$0D
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$0E
   STA   (FP),Y
   LDA   #$01
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   ORA   #$00
   BPL   .sx_done@21
   LDA   #$FF
.sx_done@21:
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
   LDY   #$60
   LDA   (FP),Y
   LDY   #$0F
   EOR   (FP),Y
   LDY   #$17
   STA   (FP),Y
   LDY   #$61
   LDA   (FP),Y
   LDY   #$10
   EOR   (FP),Y
   LDY   #$18
   STA   (FP),Y
   LDY   #$62
   LDA   (FP),Y
   LDY   #$11
   EOR   (FP),Y
   LDY   #$19
   STA   (FP),Y
   LDY   #$63
   LDA   (FP),Y
   LDY   #$12
   EOR   (FP),Y
   LDY   #$1A
   STA   (FP),Y
   LDY   #$64
   LDA   (FP),Y
   LDY   #$13
   EOR   (FP),Y
   LDY   #$1B
   STA   (FP),Y
   LDY   #$65
   LDA   (FP),Y
   LDY   #$14
   EOR   (FP),Y
   LDY   #$1C
   STA   (FP),Y
   LDY   #$66
   LDA   (FP),Y
   LDY   #$15
   EOR   (FP),Y
   LDY   #$1D
   STA   (FP),Y
   LDY   #$67
   LDA   (FP),Y
   LDY   #$16
   EOR   (FP),Y
   LDY   #$1E
   STA   (FP),Y
   LDY   #$17
   LDA   (FP),Y
   LDY   #$60
   STA   (FP),Y
   LDY   #$18
   LDA   (FP),Y
   LDY   #$61
   STA   (FP),Y
   LDY   #$19
   LDA   (FP),Y
   LDY   #$62
   STA   (FP),Y
   LDY   #$1A
   LDA   (FP),Y
   LDY   #$63
   STA   (FP),Y
   LDY   #$1B
   LDA   (FP),Y
   LDY   #$64
   STA   (FP),Y
   LDY   #$1C
   LDA   (FP),Y
   LDY   #$65
   STA   (FP),Y
   LDY   #$1D
   LDA   (FP),Y
   LDY   #$66
   STA   (FP),Y
   LDY   #$1E
   LDA   (FP),Y
   LDY   #$67
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   LDY   #$1F
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
   LDY   #$21
   STA   (FP),Y
   LDA   g1
   INY
   STA   (FP),Y
   LDA   g1
   BMI   .sx_neg@24
   LDA   #$00
   JMP   .sx_done@25
.sx_neg@24:
   LDA   #$FF
.sx_done@25:
   LDY   #$23
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   EOR   #$FF
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
   STA   g1
   LDA   (FP),Y
   INY
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@26
   LDA   #$00
   JMP   .sx_done@27
.sx_neg@26:
   LDA   #$FF
.sx_done@27:
   LDY   #$28
   STA   (FP),Y
   LDY   #$20
   LDA   (FP),Y
   LDY   #$27
   ORA   (FP),Y
   LDY   #$29
   STA   (FP),Y
   LDY   #$21
   LDA   (FP),Y
   LDY   #$28
   ORA   (FP),Y
   LDY   #$2A
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$2B
   STA   (FP),Y
   LDA   #<gs8
   INY
   STA   (FP),Y
   LDA   #>gs8
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$03
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
   LDY   #$2B
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDA   @8.s21
   AND   #$03
   LDY   #$30
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   @8.s21
   INY
   LDA   (FP),Y
   STA   @8.s21+1
   DEY
   LDA   (FP),Y
   STA   ga7+12
   INY
   LDA   (FP),Y
   STA   ga7+13
   LDA   (FP),Y
   BMI   .sx_neg@28
   LDA   #$00
   JMP   .sx_done@29
.sx_neg@28:
   LDA   #$FF
.sx_done@29:
   STA   ga7+14
   STA   ga7+15
   LDA   #$FF
   LDY   #$32
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   ORA   #$00
   BPL   .sx_done@31
   LDA   #$FF
.sx_done@31:
   LDY   #$34
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
   LDY   #$32
   AND   (FP),Y
   LDY   #$3A
   STA   (FP),Y
   LDY   #$58
   LDA   (FP),Y
   LDY   #$33
   AND   (FP),Y
   LDY   #$3B
   STA   (FP),Y
   LDY   #$59
   LDA   (FP),Y
   LDY   #$34
   AND   (FP),Y
   LDY   #$3C
   STA   (FP),Y
   LDY   #$5A
   LDA   (FP),Y
   LDY   #$35
   AND   (FP),Y
   LDY   #$3D
   STA   (FP),Y
   LDY   #$5B
   LDA   (FP),Y
   LDY   #$36
   AND   (FP),Y
   LDY   #$3E
   STA   (FP),Y
   LDY   #$5C
   LDA   (FP),Y
   LDY   #$37
   AND   (FP),Y
   LDY   #$3F
   STA   (FP),Y
   LDY   #$5D
   LDA   (FP),Y
   LDY   #$38
   AND   (FP),Y
   LDY   #$40
   STA   (FP),Y
   LDY   #$5E
   LDA   (FP),Y
   LDY   #$39
   AND   (FP),Y
   LDY   #$41
   STA   (FP),Y
   LDY   #$3A
   LDA   (FP),Y
   LDY   #$57
   STA   (FP),Y
   LDY   #$3B
   LDA   (FP),Y
   LDY   #$58
   STA   (FP),Y
   LDY   #$3C
   LDA   (FP),Y
   LDY   #$59
   STA   (FP),Y
   LDY   #$3D
   LDA   (FP),Y
   LDY   #$5A
   STA   (FP),Y
   LDY   #$3E
   LDA   (FP),Y
   LDY   #$5B
   STA   (FP),Y
   LDY   #$3F
   LDA   (FP),Y
   LDY   #$5C
   STA   (FP),Y
   LDY   #$40
   LDA   (FP),Y
   LDY   #$5D
   STA   (FP),Y
   LDY   #$41
   LDA   (FP),Y
   LDY   #$5E
   STA   (FP),Y
   LDA   #$00
   LDY   #$42
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
   SEC
   LDY   #$03
   SBC   (FP),Y
   LDY   #$4C
   STA   (FP),Y
   LDY   #$45
   LDA   (FP),Y
   LDY   #$04
   SBC   (FP),Y
   LDY   #$4D
   STA   (FP),Y
   LDY   #$46
   LDA   (FP),Y
   LDY   #$05
   SBC   (FP),Y
   LDY   #$4E
   STA   (FP),Y
   LDY   #$47
   LDA   (FP),Y
   LDY   #$06
   SBC   (FP),Y
   LDY   #$4F
   STA   (FP),Y
   LDY   #$48
   LDA   (FP),Y
   LDY   #$07
   SBC   (FP),Y
   LDY   #$50
   STA   (FP),Y
   LDY   #$49
   LDA   (FP),Y
   LDY   #$08
   SBC   (FP),Y
   LDY   #$51
   STA   (FP),Y
   LDY   #$4A
   LDA   (FP),Y
   LDY   #$09
   SBC   (FP),Y
   LDY   #$52
   STA   (FP),Y
   LDY   #$4B
   LDA   (FP),Y
   LDY   #$0A
   SBC   (FP),Y
   LDY   #$53
   STA   (FP),Y
   LDY   #$4C
   LDA   (FP),Y
   LDY   #$54
   STA   (FP),Y

   ; epilogue
   PHA
   CLC
   LDA   FP
   ADC   #$67
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

   ; prologue: 0 arg bytes, 232 local bytes
   SEC
   LDA   SSP
   SBC   #$EA
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$E9
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
   LDA   #$02
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
   DEY
   LDA   (FP),Y
   LDY   #$0F
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$10
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@32
   LDA   #$00
   JMP   .sx_done@33
.sx_neg@32:
   LDA   #$FF
.sx_done@33:
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
   LDY   #$01
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
   LDY   #$0F
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
   JSR   udivmod64
   LDA   HARGS+24
   LDY   #$17
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
   LDY   #$17
   LDA   (FP),Y
   LDY   #$1F
   STA   (FP),Y
   STA   g2
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
   LDA   #$06
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
   BMI   .sx_neg@34
   LDA   #$00
   JMP   .sx_done@35
.sx_neg@34:
   LDA   #$FF
.sx_done@35:
   LDY   #$28
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$20
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
   LDY   #$26
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
   JSR   udivmod32
   LDA   HARGS+12
   LDY   #$2A
   STA   (FP),Y
   LDA   HARGS+13
   INY
   STA   (FP),Y
   LDA   HARGS+14
   INY
   STA   (FP),Y
   LDA   HARGS+15
   INY
   STA   (FP),Y
   LDY   #$2A
   LDA   (FP),Y
   LDY   #$2E
   STA   (FP),Y
   LDY   #$2B
   LDA   (FP),Y
   LDY   #$2F
   STA   (FP),Y
   LDY   #$2C
   LDA   (FP),Y
   LDY   #$30
   STA   (FP),Y
   LDY   #$2D
   LDA   (FP),Y
   LDY   #$31
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
   LDA   ga7+4
   INY
   STA   (FP),Y
   LDA   ga7+5
   INY
   STA   (FP),Y
   LDA   ga7+6
   INY
   STA   (FP),Y
   LDA   ga7+7
   INY
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
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$3A
   LDA   (FP),Y
   CLC
   ADC   #$2C
   LDY   #$42
   STA   (FP),Y
   LDY   #$3B
   LDA   (FP),Y
   ADC   #$68
   LDY   #$43
   STA   (FP),Y
   LDY   #$3C
   LDA   (FP),Y
   ADC   #$EE
   LDY   #$44
   STA   (FP),Y
   LDY   #$3D
   LDA   (FP),Y
   ADC   #$76
   LDY   #$45
   STA   (FP),Y
   LDY   #$3E
   LDA   (FP),Y
   ADC   #$56
   LDY   #$46
   STA   (FP),Y
   LDY   #$3F
   LDA   (FP),Y
   ADC   #$E6
   LDY   #$47
   STA   (FP),Y
   LDY   #$40
   LDA   (FP),Y
   ADC   #$BE
   LDY   #$48
   STA   (FP),Y
   LDY   #$41
   LDA   (FP),Y
   ADC   #$EE
   LDY   #$49
   STA   (FP),Y
   LDY   #$42
   LDA   (FP),Y
   LDY   #$4A
   STA   (FP),Y
   LDY   #$43
   LDA   (FP),Y
   LDY   #$4B
   STA   (FP),Y
   LDY   #$44
   LDA   (FP),Y
   LDY   #$4C
   STA   (FP),Y
   LDY   #$45
   LDA   (FP),Y
   LDY   #$4D
   STA   (FP),Y
   LDY   #$46
   LDA   (FP),Y
   LDY   #$4E
   STA   (FP),Y
   LDY   #$47
   LDA   (FP),Y
   LDY   #$4F
   STA   (FP),Y
   LDY   #$48
   LDA   (FP),Y
   LDY   #$50
   STA   (FP),Y
   LDY   #$49
   LDA   (FP),Y
   AND   #$3F
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
   LDY   #$4F
   LDA   (FP),Y
   LDY   #$57
   STA   (FP),Y
   LDY   #$50
   LDA   (FP),Y
   LDY   #$58
   STA   (FP),Y
   LDY   #$51
   LDA   (FP),Y
   EOR   #$20
   LDY   #$59
   STA   (FP),Y
   LDY   #$52
   LDA   (FP),Y
   LDY   #$5A
   STA   (FP),Y
   LDY   #$53
   LDA   (FP),Y
   LDY   #$5B
   STA   (FP),Y
   LDY   #$54
   LDA   (FP),Y
   LDY   #$5C
   STA   (FP),Y
   LDY   #$55
   LDA   (FP),Y
   LDY   #$5D
   STA   (FP),Y
   LDY   #$56
   LDA   (FP),Y
   LDY   #$5E
   STA   (FP),Y
   LDY   #$57
   LDA   (FP),Y
   LDY   #$5F
   STA   (FP),Y
   LDY   #$58
   LDA   (FP),Y
   LDY   #$60
   STA   (FP),Y
   LDY   #$59
   LDA   (FP),Y
   SEC
   SBC   #$20
   LDY   #$61
   STA   (FP),Y
   LDY   #$5A
   LDA   (FP),Y
   LDY   #$62
   STA   (FP),Y
   LDA   z5
   INY
   STA   (FP),Y
   LDA   z5
   BMI   .sx_neg@36
   LDA   #$00
   JMP   .sx_done@37
.sx_neg@36:
   LDA   #$FF
.sx_done@37:
   LDY   #$64
   STA   (FP),Y
   LDA   #$05
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$63
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
   LDY   #$67
   STA   (FP),Y
   LDA   HARGS+7
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$69
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$6A
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@38
   LDA   #$00
   JMP   .sx_done@39
.sx_neg@38:
   LDA   #$FF
.sx_done@39:
   LDY   #$6B
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
   SEC
   LDA   SSP
   SBC   #$11
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDY   #$2E
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$2F
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$30
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$31
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   LDY   #$32
   LDA   (FP),Y
   LDY   #$05
   STA   (SSP),Y
   LDY   #$33
   LDA   (FP),Y
   LDY   #$06
   STA   (SSP),Y
   LDY   #$34
   LDA   (FP),Y
   LDY   #$07
   STA   (SSP),Y
   LDY   #$35
   LDA   (FP),Y
   LDY   #$08
   STA   (SSP),Y
   LDY   #$62
   LDA   (FP),Y
   LDY   #$09
   STA   (SSP),Y
   LDY   #$69
   LDA   (FP),Y
   LDY   #$0A
   STA   (SSP),Y
   LDY   #$6A
   LDA   (FP),Y
   LDY   #$0B
   STA   (SSP),Y
   LDY   #$6B
   LDA   (FP),Y
   LDY   #$0C
   STA   (SSP),Y
   LDY   #$6C
   LDA   (FP),Y
   LDY   #$0D
   STA   (SSP),Y
   LDY   #$6D
   LDA   (FP),Y
   LDY   #$0E
   STA   (SSP),Y
   LDY   #$6E
   LDA   (FP),Y
   LDY   #$0F
   STA   (SSP),Y
   LDY   #$6F
   LDA   (FP),Y
   LDY   #$10
   STA   (SSP),Y
   LDY   #$70
   LDA   (FP),Y
   LDY   #$11
   STA   (SSP),Y
   JSR   f14
   LDY   #$71
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
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$72
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   LDY   #$73
   LDA   (FP),Y
   LDY   #$02
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
   LDY   #$7A
   STA   (FP),Y
   INY
   STA   (FP),Y
.loop@0_continue:
   LDY   #$7B
   LDA   (FP),Y
   INY
   STA   (FP),Y
   CLC
   ADC   #$01
   INY
   STA   (FP),Y
   LDY   #$7B
   STA   (FP),Y
   INY
   LDA   (FP),Y
   LDY   #$7E
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
   BVC   .cmp_novf@40
   EOR   #$80
.cmp_novf@40:
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
   BNE   .lb_skip@3
   JMP   .loop@0_break
.lb_skip@3:
   LDA   g1
   LDY   #$82
   STA   (FP),Y
   LDA   g1
   BMI   .sx_neg@41
   LDA   #$00
   JMP   .sx_done@42
.sx_neg@41:
   LDA   #$FF
.sx_done@42:
   LDY   #$83
   STA   (FP),Y
   LDA   g2
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$82
   LDA   (FP),Y
   CLC
   LDY   #$84
   ADC   (FP),Y
   LDY   #$86
   STA   (FP),Y
   LDY   #$83
   LDA   (FP),Y
   LDY   #$85
   ADC   (FP),Y
   LDY   #$87
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$88
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$89
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@43
   LDA   #$00
   JMP   .sx_done@44
.sx_neg@43:
   LDA   #$FF
.sx_done@44:
   LDY   #$8A
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
   LDA   #<gs8
   INY
   STA   (FP),Y
   LDA   #>gs8
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$03
   LDY   #$92
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$93
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$94
   STA   (FP),Y
   INY
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@45
   LDA   #$00
   JMP   .sx_done@46
.sx_neg@45:
   LDA   #$FF
.sx_done@46:
   LDY   #$96
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$97
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$98
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@47
   LDA   #$00
   JMP   .sx_done@48
.sx_neg@47:
   LDA   #$FF
.sx_done@48:
   LDY   #$99
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
   SEC
   LDA   SSP
   SBC   #$11
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDY   #$88
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$89
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$8A
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$8B
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   LDY   #$8C
   LDA   (FP),Y
   LDY   #$05
   STA   (SSP),Y
   LDY   #$8D
   LDA   (FP),Y
   LDY   #$06
   STA   (SSP),Y
   LDY   #$8E
   LDA   (FP),Y
   LDY   #$07
   STA   (SSP),Y
   LDY   #$8F
   LDA   (FP),Y
   LDY   #$08
   STA   (SSP),Y
   LDY   #$7B
   LDA   (FP),Y
   LDY   #$09
   STA   (SSP),Y
   LDY   #$97
   LDA   (FP),Y
   LDY   #$0A
   STA   (SSP),Y
   LDY   #$98
   LDA   (FP),Y
   LDY   #$0B
   STA   (SSP),Y
   LDY   #$99
   LDA   (FP),Y
   LDY   #$0C
   STA   (SSP),Y
   LDY   #$9A
   LDA   (FP),Y
   LDY   #$0D
   STA   (SSP),Y
   LDY   #$9B
   LDA   (FP),Y
   LDY   #$0E
   STA   (SSP),Y
   LDY   #$9C
   LDA   (FP),Y
   LDY   #$0F
   STA   (SSP),Y
   LDY   #$9D
   LDA   (FP),Y
   LDY   #$10
   STA   (SSP),Y
   LDY   #$9E
   LDA   (FP),Y
   LDY   #$11
   STA   (SSP),Y
   JSR   f14
   LDY   #$9F
   STA   (FP),Y
   LDY   #$7B
   LDA   (FP),Y
   LDY   #$A0
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
   LDY   #$A0
   LDA   (FP),Y
   LDY   #$A2
   AND   (FP),Y
   LDY   #$A4
   STA   (FP),Y
   LDY   #$A1
   LDA   (FP),Y
   LDY   #$A3
   AND   (FP),Y
   LDY   #$A5
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   BEQ   .cmp_true@49
   LDA   #$00
   JMP   .cmp_end@50
.cmp_true@49:
   LDA   #$01
.cmp_end@50:
   LDY   #$A6
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .case@2
   LDY   #$A4
   LDA   (FP),Y
   CMP   #$01
   BEQ   .cmp_true@51
   LDA   #$00
   JMP   .cmp_end@52
.cmp_true@51:
   LDA   #$01
.cmp_end@52:
   LDY   #$A8
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
   LDY   #$A4
   LDA   (FP),Y
   CMP   #$06
   BEQ   .cmp_true@53
   LDA   #$00
   JMP   .cmp_end@54
.cmp_true@53:
   LDA   #$01
.cmp_end@54:
   LDY   #$AA
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@1
   JMP   .loop@0_continue
.lb_skip@1:
   JMP   .default@7
.case@2:
   LDA   z5
   LDY   #$AC
   STA   (FP),Y
   LDA   z5
   BMI   .sx_neg@55
   LDA   #$00
   JMP   .sx_done@56
.sx_neg@55:
   LDA   #$FF
.sx_done@56:
   LDY   #$AD
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$AE
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$AF
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$B0
   STA   (FP),Y
   STA   z5
   LDA   (FP),Y
   INY
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@57
   LDA   #$00
   JMP   .sx_done@58
.sx_neg@57:
   LDA   #$FF
.sx_done@58:
   LDY   #$B2
   STA   (FP),Y
   LDY   #$7B
   LDA   (FP),Y
   LDY   #$B3
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$B1
   LDA   (FP),Y
   LDY   #$B3
   EOR   (FP),Y
   LDY   #$B5
   STA   (FP),Y
   LDY   #$B2
   LDA   (FP),Y
   LDY   #$B4
   EOR   (FP),Y
   LDY   #$B6
   STA   (FP),Y
   LDA   #$07
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$B5
   LDA   (FP),Y
   LDY   #$B7
   AND   (FP),Y
   LDY   #$B9
   STA   (FP),Y
   LDY   #$B6
   LDA   (FP),Y
   LDY   #$B8
   AND   (FP),Y
   LDY   #$BA
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CMP   #$06
   BEQ   .cmp_true@59
   LDA   #$00
   JMP   .cmp_end@60
.cmp_true@59:
   LDA   #$01
.cmp_end@60:
   LDY   #$BB
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   LDA   #$FF
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   ORA   (FP),Y
   BNE   .lb_skip@0
   JMP   .loop@0_continue
.lb_skip@0:
   LDA   #<gs8
   LDY   #$BF
   STA   (FP),Y
   LDA   #>gs8
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$03
   LDY   #$C1
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$C2
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$C3
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
   LDY   #$C5
   STA   (FP),Y
   LDA   #$07
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$C4
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
   LDY   #$C8
   STA   (FP),Y
   LDA   HARGS+7
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$CA
   STA   (FP),Y
   STA   g2
   JMP   .loop@0_continue
.case@5:
   LDY   #$01
   LDA   (FP),Y
   SEC
   SBC   #$01
   LDY   #$CB
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   SBC   #$00
   LDY   #$CC
   STA   (FP),Y
   LDY   #$03
   LDA   (FP),Y
   SBC   #$00
   LDY   #$CD
   STA   (FP),Y
   LDY   #$04
   LDA   (FP),Y
   SBC   #$00
   LDY   #$CE
   STA   (FP),Y
   LDY   #$05
   LDA   (FP),Y
   SBC   #$00
   LDY   #$CF
   STA   (FP),Y
   LDY   #$06
   LDA   (FP),Y
   SBC   #$00
   LDY   #$D0
   STA   (FP),Y
   LDY   #$07
   LDA   (FP),Y
   SBC   #$00
   LDY   #$D1
   STA   (FP),Y
   LDY   #$08
   LDA   (FP),Y
   SBC   #$00
   LDY   #$D2
   STA   (FP),Y
   LDY   #$CB
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   LDY   #$CC
   LDA   (FP),Y
   LDY   #$02
   STA   (FP),Y
   LDY   #$CD
   LDA   (FP),Y
   LDY   #$03
   STA   (FP),Y
   LDY   #$CE
   LDA   (FP),Y
   LDY   #$04
   STA   (FP),Y
   LDY   #$CF
   LDA   (FP),Y
   LDY   #$05
   STA   (FP),Y
   LDY   #$D0
   LDA   (FP),Y
   LDY   #$06
   STA   (FP),Y
   LDY   #$D1
   LDA   (FP),Y
   LDY   #$07
   STA   (FP),Y
   LDY   #$D2
   LDA   (FP),Y
   LDY   #$08
   STA   (FP),Y
   JMP   .loop@0_continue
.default@7:
   LDA   #$00
   LDY   #$DD
   STA   (FP),Y
   INY
   STA   (FP),Y
.loop@8_continue:
   LDY   #$DE
   LDA   (FP),Y
   INY
   STA   (FP),Y
   CLC
   ADC   #$01
   INY
   STA   (FP),Y
   LDY   #$DE
   STA   (FP),Y
   INY
   LDA   (FP),Y
   LDY   #$E1
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
   BVC   .cmp_novf@65
   EOR   #$80
.cmp_novf@65:
   ASL
   LDA   #$00
   ROL
   LDY   #$E3
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .loop@8_continue
   JMP   .loop@0_continue
.loop@0_break:
   JSR   fin0
   JSR   fin1
   JSR   fin2
   LDA   acc
   LDY   #$E5
   STA   (FP),Y
   LDA   acc+1
   AND   #$7F
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$E7
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$E8
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
   ADC   #$EA
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$E9
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
   DC.B  $01

g2:
   DC.B  $01

g3:
   DC.L  $00000080

ga7:
   DC.L  $00010000
   DC.L  $00000080
   DC.L  $00000007
   DC.L  $8B8C1AB3

gs8:
   DS.B  1
   DC.W  $0001
   DC.B  $FF

@3.s13:
   DC.W  $B03F

@8.s21:
   DC.W  $0005

__zp_init:
   SUBROUTINE

   LDA   #$00
   STA   z4+1
   STA   z5
   STA   z6
   LDA   #$02
   STA   z4
   LDA   #$80
   STA   z6+1
   RTS
