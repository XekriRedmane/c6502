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
   LDA   ga3
   LDY   #$09
   STA   (FP),Y
   LDA   ga3+1
   INY
   STA   (FP),Y
   LDA   ga3+2
   INY
   STA   (FP),Y
   LDA   ga3+3
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
   LDA   ga3+4
   LDY   #$0D
   STA   (FP),Y
   LDA   ga3+5
   INY
   STA   (FP),Y
   LDA   ga3+6
   INY
   STA   (FP),Y
   LDA   ga3+7
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

   LDA   ga3+8
   LDY   #$01
   STA   (FP),Y
   LDA   ga3+9
   INY
   STA   (FP),Y
   LDA   ga3+10
   INY
   STA   (FP),Y
   LDA   ga3+11
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
   LDA   ga3+12
   LDY   #$05
   STA   (FP),Y
   LDA   ga3+13
   INY
   STA   (FP),Y
   LDA   ga3+14
   INY
   STA   (FP),Y
   LDA   ga3+15
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
   LDA   #<gs4
   LDY   #$09
   STA   (FP),Y
   LDA   #>gs4
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
   LDA   #<gs4
   LDY   #$12
   STA   (FP),Y
   LDA   #>gs4
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$02
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

f7:
   SUBROUTINE

   ; prologue: 12 arg bytes, 168 local bytes
   SEC
   LDA   SSP
   SBC   #$AA
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$A9
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
   LDA   #$80
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
   LDA   #<ga3
   INY
   STA   (FP),Y
   LDA   #>ga3
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
   LDY   #$0C
   STA   (FP),Y
   STA   HARGS
   LDA   #$04
   STA   HARGS+1
   JSR   mul8
   LDA   HARGS+2
   LDY   #$0D
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$08
   LDA   (FP),Y
   CLC
   LDY   #$0E
   ADC   (FP),Y
   LDY   #$10
   STA   (FP),Y
   LDY   #$09
   LDA   (FP),Y
   LDY   #$0F
   ADC   (FP),Y
   LDY   #$11
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$12
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$13
   STA   (FP),Y
   LDY   #$02
   LDA   (DPTR),Y
   LDY   #$14
   STA   (FP),Y
   LDY   #$03
   LDA   (DPTR),Y
   LDY   #$15
   STA   (FP),Y
   LDY   #$12
   LDA   (FP),Y
   SEC
   SBC   #$01
   LDY   #$16
   STA   (FP),Y
   LDY   #$13
   LDA   (FP),Y
   SBC   #$00
   LDY   #$17
   STA   (FP),Y
   LDY   #$14
   LDA   (FP),Y
   SBC   #$00
   LDY   #$18
   STA   (FP),Y
   LDY   #$15
   LDA   (FP),Y
   SBC   #$00
   LDY   #$19
   STA   (FP),Y
   LDY   #$10
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$16
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$17
   LDA   (FP),Y
   LDY   #$01
   STA   (DPTR),Y
   LDY   #$18
   LDA   (FP),Y
   LDY   #$02
   STA   (DPTR),Y
   LDY   #$19
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
   LDA   g1
   LDY   #$1A
   STA   (FP),Y
   LDA   g1+1
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
   CMP   #$04
   BEQ   .cmp_true@2
   LDA   #$00
   JMP   .cmp_end@3
.cmp_true@2:
   LDA   #$01
.cmp_end@3:
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
   CMP   #$07
   BEQ   .cmp_true@4
   LDA   #$00
   JMP   .cmp_end@5
.cmp_true@4:
   LDA   #$01
.cmp_end@5:
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
.case@1:
.case@2:
   LDA   #$FF
   LDY   #$24
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
   LDY   #$26
   STA   (FP),Y
   LDA   #$FF
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$25
   LDA   (FP),Y
   CLC
   LDY   #$28
   ADC   (FP),Y
   LDY   #$2A
   STA   (FP),Y
   LDY   #$26
   LDA   (FP),Y
   LDY   #$29
   ADC   (FP),Y
   LDY   #$2B
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$2C
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$2D
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@8
   LDA   #$00
   JMP   .sx_done@9
.sx_neg@8:
   LDA   #$FF
.sx_done@9:
   LDY   #$2E
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$2C
   LDA   (FP),Y
   LDY   #$AB
   STA   (FP),Y
   LDY   #$2D
   LDA   (FP),Y
   LDY   #$AC
   STA   (FP),Y
   LDY   #$2E
   LDA   (FP),Y
   LDY   #$AD
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   LDY   #$32
   STA   (FP),Y
   INY
   STA   (FP),Y
.loop@4_start:
   LDY   #$AB
   LDA   (FP),Y
   LDY   #$34
   STA   (FP),Y
   LDY   #$AC
   LDA   (FP),Y
   LDA   #$00
   LDY   #$35
   STA   (FP),Y
   LDY   #$AD
   LDA   (FP),Y
   LDA   #$00
   LDY   #$36
   STA   (FP),Y
   LDY   #$AE
   LDA   (FP),Y
   LDA   #$00
   LDY   #$37
   STA   (FP),Y
   LDY   #$AF
   LDA   (FP),Y
   LDA   #$00
   LDY   #$38
   STA   (FP),Y
   LDY   #$B0
   LDA   (FP),Y
   LDA   #$00
   LDY   #$39
   STA   (FP),Y
   LDY   #$34
   LDA   (FP),Y
   LDY   #$AB
   STA   (FP),Y
   LDY   #$35
   LDA   (FP),Y
   LDY   #$AC
   STA   (FP),Y
   LDY   #$36
   LDA   (FP),Y
   LDY   #$AD
   STA   (FP),Y
   LDY   #$37
   LDA   (FP),Y
   LDY   #$AE
   STA   (FP),Y
   LDY   #$38
   LDA   (FP),Y
   LDY   #$AF
   STA   (FP),Y
   LDY   #$39
   LDA   (FP),Y
   LDY   #$B0
   STA   (FP),Y
   LDY   #$34
   LDA   (FP),Y
   LDY   #$3A
   STA   (FP),Y
   LDY   #$35
   LDA   (FP),Y
   LDY   #$3B
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   g1
   INY
   LDA   (FP),Y
   STA   g1+1
   LDA   #<gs4
   INY
   STA   (FP),Y
   LDA   #>gs4
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$01
   LDY   #$3E
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$3F
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
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
   DEY
   LDA   (FP),Y
   STA   HARGS
   LDA   #$00
   LDX   #$08
   LSR   HARGS
.mul_loop@10:
   BCC   .mul_skip@11
   CLC
   ADC   #$03
.mul_skip@11:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@10
   LDY   #$44
   STA   (FP),Y
   LDA   HARGS
   DEY
   STA   (FP),Y
   LDY   #$45
   STA   (FP),Y
   LDY   #$3E
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$45
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDA   gp5
   STA   DPTR
   LDA   gp5+1
   STA   DPTR+1
   LDA   (DPTR),Y
   LDY   #$46
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
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
   LDY   #$48
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$49
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$4A
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$4B
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   JSR   mix
   LDY   #$33
   LDA   (FP),Y
   CLC
   ADC   #$01
   LDY   #$4C
   STA   (FP),Y
   LDY   #$33
   STA   (FP),Y
   LDY   #$4D
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
   BVC   .cmp_novf@12
   EOR   #$80
.cmp_novf@12:
   ASL
   LDA   #$00
   ROL
   LDY   #$4F
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@2
   JMP   .loop@4_start
.lb_skip@2:
   LDA   #$00
   LDY   #$51
   STA   (FP),Y
   INY
   STA   (FP),Y
.loop@5_start:
   LDY   #$53
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
   BVC   .cmp_novf@13
   EOR   #$80
.cmp_novf@13:
   ASL
   LDA   #$00
   ROL
   LDY   #$55
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@1
   JMP   .loop@5_break
.lb_skip@1:
   LDA   #$00
   LDY   #$57
   STA   (FP),Y
   INY
   STA   (FP),Y
.loop@6_start:
   LDY   #$59
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
   BVC   .cmp_novf@14
   EOR   #$80
.cmp_novf@14:
   ASL
   LDA   #$00
   ROL
   LDY   #$5B
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@0
   JMP   .loop@6_break
.lb_skip@0:
   LDA   g2
   LDY   #$5D
   STA   (FP),Y
   LDA   g2
   SEC
   SBC   #$01
   INY
   STA   (FP),Y
   STA   g2
   DEY
   LDA   (FP),Y
   LDY   #$5F
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@15
   LDA   #$00
   JMP   .sx_done@16
.sx_neg@15:
   LDA   #$FF
.sx_done@16:
   LDY   #$60
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$5F
   LDA   (FP),Y
   LDY   #$03
   STA   (FP),Y
   LDY   #$60
   LDA   (FP),Y
   LDY   #$04
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$03
   LDA   (FP),Y
   CLC
   ADC   #$5A
   LDY   #$64
   STA   (FP),Y
   LDY   #$04
   LDA   (FP),Y
   ADC   #$00
   LDY   #$65
   STA   (FP),Y
   LDY   #$05
   LDA   (FP),Y
   ADC   #$00
   LDY   #$66
   STA   (FP),Y
   LDY   #$06
   LDA   (FP),Y
   ADC   #$00
   LDY   #$67
   STA   (FP),Y
   LDY   #$07
   LDA   (FP),Y
   ADC   #$00
   LDY   #$68
   STA   (FP),Y
   LDY   #$64
   LDA   (FP),Y
   LDY   #$69
   STA   (FP),Y
   LDY   #$65
   LDA   (FP),Y
   LDY   #$6A
   STA   (FP),Y
   LDY   #$66
   LDA   (FP),Y
   LDY   #$6B
   STA   (FP),Y
   LDY   #$67
   LDA   (FP),Y
   LDY   #$6C
   STA   (FP),Y
   LDY   #$68
   LDA   (FP),Y
   AND   #$01
   LDY   #$6D
   STA   (FP),Y
   LDY   #$69
   LDA   (FP),Y
   LDY   #$6E
   STA   (FP),Y
   LDY   #$6A
   LDA   (FP),Y
   LDY   #$6F
   STA   (FP),Y
   LDY   #$6B
   LDA   (FP),Y
   LDY   #$70
   STA   (FP),Y
   LDY   #$6C
   LDA   (FP),Y
   LDY   #$71
   STA   (FP),Y
   LDY   #$6D
   LDA   (FP),Y
   EOR   #$01
   LDY   #$72
   STA   (FP),Y
   LDY   #$6E
   LDA   (FP),Y
   LDY   #$73
   STA   (FP),Y
   LDY   #$6F
   LDA   (FP),Y
   LDY   #$74
   STA   (FP),Y
   LDY   #$70
   LDA   (FP),Y
   LDY   #$75
   STA   (FP),Y
   LDY   #$71
   LDA   (FP),Y
   LDY   #$76
   STA   (FP),Y
   LDY   #$72
   LDA   (FP),Y
   SEC
   SBC   #$01
   LDY   #$77
   STA   (FP),Y
   LDY   #$73
   LDA   (FP),Y
   LDY   #$03
   STA   (FP),Y
   LDY   #$74
   LDA   (FP),Y
   LDY   #$04
   STA   (FP),Y
   LDY   #$75
   LDA   (FP),Y
   LDY   #$05
   STA   (FP),Y
   LDY   #$76
   LDA   (FP),Y
   LDY   #$06
   STA   (FP),Y
   LDY   #$77
   LDA   (FP),Y
   LDY   #$07
   STA   (FP),Y
   LDY   #$B5
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$78
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$79
   STA   (FP),Y
   LDY   #$02
   LDA   (DPTR),Y
   LDY   #$7A
   STA   (FP),Y
   LDY   #$03
   LDA   (DPTR),Y
   LDY   #$7B
   STA   (FP),Y
   LDY   #$78
   LDA   (FP),Y
   LDY   #$7C
   STA   (FP),Y
   LDY   #$79
   LDA   (FP),Y
   LDY   #$7D
   STA   (FP),Y
   LDY   #$7A
   LDA   (FP),Y
   LDY   #$7E
   STA   (FP),Y
   LDY   #$7B
   LDA   (FP),Y
   LDY   #$7F
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$73
   LDA   (FP),Y
   LDY   #$7C
   AND   (FP),Y
   LDY   #$81
   STA   (FP),Y
   LDY   #$74
   LDA   (FP),Y
   LDY   #$7D
   AND   (FP),Y
   LDY   #$82
   STA   (FP),Y
   LDY   #$75
   LDA   (FP),Y
   LDY   #$7E
   AND   (FP),Y
   LDY   #$83
   STA   (FP),Y
   LDY   #$76
   LDA   (FP),Y
   LDY   #$7F
   AND   (FP),Y
   LDY   #$84
   STA   (FP),Y
   LDY   #$77
   LDA   (FP),Y
   LDY   #$80
   AND   (FP),Y
   LDY   #$85
   STA   (FP),Y
   LDY   #$81
   LDA   (FP),Y
   LDY   #$86
   STA   (FP),Y
   LDY   #$82
   LDA   (FP),Y
   LDY   #$87
   STA   (FP),Y
   LDY   #$83
   LDA   (FP),Y
   LDY   #$88
   STA   (FP),Y
   LDY   #$84
   LDA   (FP),Y
   LDY   #$89
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$04
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDY   #$86
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$87
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$88
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$89
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   JSR   mix
   LDY   #$58
   LDA   (FP),Y
   LDY   #$8A
   STA   (FP),Y
   CLC
   ADC   #$01
   INY
   STA   (FP),Y
   LDY   #$58
   STA   (FP),Y
   JMP   .loop@6_start
.loop@6_break:
   LDA   #$00
   LDY   #$8C
   STA   (FP),Y
   INY
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@17
   LDA   #$00
   JMP   .sx_done@18
.sx_neg@17:
   LDA   #$FF
.sx_done@18:
   LDY   #$8E
   STA   (FP),Y
   LDA   #<gs4
   INY
   STA   (FP),Y
   LDA   #>gs4
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$01
   LDY   #$91
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$92
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$93
   STA   (FP),Y
   SEC
   SBC   #$01
   INY
   STA   (FP),Y
   LDY   #$91
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$94
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$94
   LDA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$8D
   LDA   (FP),Y
   LDY   #$95
   EOR   (FP),Y
   LDY   #$97
   STA   (FP),Y
   LDY   #$8E
   LDA   (FP),Y
   LDY   #$96
   EOR   (FP),Y
   LDY   #$98
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   LDY   #$52
   LDA   (FP),Y
   LDY   #$99
   STA   (FP),Y
   CLC
   ADC   #$01
   INY
   STA   (FP),Y
   LDY   #$52
   STA   (FP),Y
   JMP   .loop@5_start
.loop@5_break:
   LDY   #$02
   LDA   (FP),Y
   BEQ   .cond_else@1
   LDY   #$9B
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$9D
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$9E
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@19
   LDA   #$00
   JMP   .sx_done@20
.sx_neg@19:
   LDA   #$FF
.sx_done@20:
   LDY   #$9F
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$9D
   LDA   (FP),Y
   LDY   #$A1
   STA   (FP),Y
   LDY   #$9E
   LDA   (FP),Y
   LDY   #$A2
   STA   (FP),Y
   LDY   #$9F
   LDA   (FP),Y
   LDY   #$A3
   STA   (FP),Y
   INY
   STA   (FP),Y
   JMP   .cond_end@2
.cond_else@1:
   LDY   #$B5
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$A5
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$A6
   STA   (FP),Y
   LDY   #$02
   LDA   (DPTR),Y
   LDY   #$A7
   STA   (FP),Y
   LDY   #$03
   LDA   (DPTR),Y
   LDY   #$A8
   STA   (FP),Y
   LDY   #$A5
   LDA   (FP),Y
   LDY   #$A1
   STA   (FP),Y
   LDY   #$A6
   LDA   (FP),Y
   LDY   #$A2
   STA   (FP),Y
   LDY   #$A7
   LDA   (FP),Y
   LDY   #$A3
   STA   (FP),Y
   LDY   #$A8
   LDA   (FP),Y
   LDY   #$A4
   STA   (FP),Y
.cond_end@2:
   LDY   #$A1
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
   ADC   #$B6
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$A9
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   RTS

f16:
   SUBROUTINE

   ; prologue: 13 arg bytes, 162 local bytes
   SEC
   LDA   SSP
   SBC   #$A4
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$A3
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
   LDY   #$03
   LDA   (FP),Y
   LDY   #$07
   STA   (FP),Y
   LDY   #$04
   LDA   (FP),Y
   LDY   #$08
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$B0
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
   ORA   #$00
   BMI   .sx_neg@21
   LDA   #$00
   JMP   .sx_done@22
.sx_neg@21:
   LDA   #$FF
.sx_done@22:
   LDY   #$0D
   STA   (FP),Y
   LSR
   DEY
   LDA   (FP),Y
   ROR
   LDY   #$0E
   STA   (FP),Y
   LDY   #$10
   STA   (FP),Y
   LDY   #$B0
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$10
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$AB
   LDA   (FP),Y
   LDY   #$11
   STA   (FP),Y
   LDY   #$AC
   LDA   (FP),Y
   LDY   #$12
   STA   (FP),Y
   LDY   #$AD
   LDA   (FP),Y
   LDY   #$13
   STA   (FP),Y
   LDY   #$AE
   LDA   (FP),Y
   LDY   #$14
   STA   (FP),Y
   LDY   #$AB
   LDA   (FP),Y
   SEC
   SBC   #$01
   LDY   #$15
   STA   (FP),Y
   LDY   #$AC
   LDA   (FP),Y
   SBC   #$00
   LDY   #$16
   STA   (FP),Y
   LDY   #$AD
   LDA   (FP),Y
   SBC   #$00
   LDY   #$17
   STA   (FP),Y
   LDY   #$AE
   LDA   (FP),Y
   SBC   #$00
   LDY   #$18
   STA   (FP),Y
   LDY   #$15
   LDA   (FP),Y
   LDY   #$AB
   STA   (FP),Y
   LDY   #$16
   LDA   (FP),Y
   LDY   #$AC
   STA   (FP),Y
   LDY   #$17
   LDA   (FP),Y
   LDY   #$AD
   STA   (FP),Y
   LDY   #$18
   LDA   (FP),Y
   LDY   #$AE
   STA   (FP),Y
   LDA   #$01
   LDY   #$19
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$80
   INY
   STA   (FP),Y
   LDY   #$1B
   LDA   (FP),Y
   LDY   #$1D
   CMP   (FP),Y
   BNE   .thread@0
   LDY   #$1A
   LDA   (FP),Y
   LDY   #$1C
   CMP   (FP),Y
   BEQ   .cmp_true@23
.thread@0:
   LDA   #$00
   JMP   .cmp_end@24
.cmp_true@23:
   LDA   #$01
.cmp_end@24:
   LDY   #$1E
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$20
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$21
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@26
   LDA   #$00
   JMP   .sx_done@27
.sx_neg@26:
   LDA   #$FF
.sx_done@27:
   LDY   #$22
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$11
   LDA   (FP),Y
   LDY   #$20
   EOR   (FP),Y
   LDY   #$24
   STA   (FP),Y
   LDY   #$12
   LDA   (FP),Y
   LDY   #$21
   EOR   (FP),Y
   LDY   #$25
   STA   (FP),Y
   LDY   #$13
   LDA   (FP),Y
   LDY   #$22
   EOR   (FP),Y
   LDY   #$26
   STA   (FP),Y
   LDY   #$14
   LDA   (FP),Y
   LDY   #$23
   EOR   (FP),Y
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
   LDY   #$27
   LDA   (FP),Y
   LDY   #$2B
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$04
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDY   #$28
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$29
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$2A
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$2B
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   JSR   mix
   LDA   #$FF
   LDY   #$2C
   STA   (FP),Y
   LDA   #$7F
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   INY
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
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   g1
   EOR   #$FF
   INY
   STA   (FP),Y
   LDA   g1+1
   EOR   #$FF
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$38
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$39
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   CLC
   LDA   FP
   ADC   #$07
   INY
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$0C
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
   LDY   #$34
   LDA   (FP),Y
   LDY   #$05
   STA   (SSP),Y
   LDY   #$35
   LDA   (FP),Y
   LDY   #$06
   STA   (SSP),Y
   LDY   #$38
   LDA   (FP),Y
   LDY   #$07
   STA   (SSP),Y
   LDY   #$39
   LDA   (FP),Y
   LDY   #$08
   STA   (SSP),Y
   LDY   #$3A
   LDA   (FP),Y
   LDY   #$09
   STA   (SSP),Y
   LDY   #$3B
   LDA   (FP),Y
   LDY   #$0A
   STA   (SSP),Y
   LDY   #$3C
   LDA   (FP),Y
   LDY   #$0B
   STA   (SSP),Y
   LDY   #$3D
   LDA   (FP),Y
   LDY   #$0C
   STA   (SSP),Y
   JSR   f7
   LDA   HARGS+8
   LDY   #$3E
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
   LDY   #$3E
   LDA   (FP),Y
   LDY   #$42
   STA   (FP),Y
   LDY   #$02
   STA   (FP),Y
   LDY   #$B0
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$43
   STA   (FP),Y
   INY
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@28
   LDA   #$00
   JMP   .sx_done@29
.sx_neg@28:
   LDA   #$FF
.sx_done@29:
   LDY   #$45
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$44
   LDA   (FP),Y
   LDY   #$46
   EOR   (FP),Y
   LDY   #$48
   STA   (FP),Y
   LDY   #$45
   LDA   (FP),Y
   LDY   #$47
   EOR   (FP),Y
   LDY   #$49
   STA   (FP),Y
   LDA   #$03
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$48
   LDA   (FP),Y
   CLC
   LDY   #$4B
   ADC   (FP),Y
   LDY   #$4D
   STA   (FP),Y
   LDY   #$49
   LDA   (FP),Y
   LDY   #$4C
   ADC   (FP),Y
   LDY   #$4E
   STA   (FP),Y
   LDA   #$07
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$4D
   LDA   (FP),Y
   LDY   #$4F
   AND   (FP),Y
   LDY   #$51
   STA   (FP),Y
   LDY   #$4E
   LDA   (FP),Y
   LDY   #$50
   AND   (FP),Y
   LDY   #$52
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CMP   #$07
   BEQ   .cmp_true@30
   LDA   #$00
   JMP   .cmp_end@31
.cmp_true@30:
   LDA   #$01
.cmp_end@31:
   LDY   #$53
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@3
   JMP   .default@9
.lb_skip@3:
   LDY   #$A5
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
   LDA   #$00
   STA   HARGS+6
   STA   HARGS+7
   LDA   #$04
   STA   HARGS+8
   JSR   asl64
   LDA   HARGS+9
   LDY   #$55
   STA   (FP),Y
   LDA   HARGS+10
   INY
   STA   (FP),Y
   LDA   HARGS+11
   INY
   STA   (FP),Y
   LDA   HARGS+12
   INY
   STA   (FP),Y
   LDA   HARGS+13
   INY
   STA   (FP),Y
   LDA   HARGS+14
   INY
   STA   (FP),Y
   LDY   #$55
   LDA   (FP),Y
   LDY   #$5B
   STA   (FP),Y
   LDY   #$56
   LDA   (FP),Y
   LDY   #$5C
   STA   (FP),Y
   LDY   #$57
   LDA   (FP),Y
   LDY   #$5D
   STA   (FP),Y
   LDY   #$58
   LDA   (FP),Y
   LDY   #$5E
   STA   (FP),Y
   LDY   #$59
   LDA   (FP),Y
   LDY   #$5F
   STA   (FP),Y
   LDY   #$5A
   LDA   (FP),Y
   AND   #$0F
   LDY   #$60
   STA   (FP),Y
   LDY   #$5B
   LDA   (FP),Y
   LDY   #$61
   STA   (FP),Y
   LDY   #$5C
   LDA   (FP),Y
   LDY   #$62
   STA   (FP),Y
   LDY   #$5D
   LDA   (FP),Y
   LDY   #$63
   STA   (FP),Y
   LDY   #$5E
   LDA   (FP),Y
   LDY   #$64
   STA   (FP),Y
   LDY   #$5F
   LDA   (FP),Y
   LDY   #$65
   STA   (FP),Y
   LDY   #$60
   LDA   (FP),Y
   EOR   #$08
   LDY   #$66
   STA   (FP),Y
   LDY   #$61
   LDA   (FP),Y
   LDY   #$67
   STA   (FP),Y
   LDY   #$62
   LDA   (FP),Y
   LDY   #$68
   STA   (FP),Y
   LDY   #$63
   LDA   (FP),Y
   LDY   #$69
   STA   (FP),Y
   LDY   #$64
   LDA   (FP),Y
   LDY   #$6A
   STA   (FP),Y
   LDY   #$65
   LDA   (FP),Y
   LDY   #$6B
   STA   (FP),Y
   LDY   #$66
   LDA   (FP),Y
   SEC
   SBC   #$08
   LDY   #$6C
   STA   (FP),Y
   LDY   #$67
   LDA   (FP),Y
   LDY   #$A5
   STA   (FP),Y
   LDY   #$68
   LDA   (FP),Y
   LDY   #$A6
   STA   (FP),Y
   LDY   #$69
   LDA   (FP),Y
   LDY   #$A7
   STA   (FP),Y
   LDY   #$6A
   LDA   (FP),Y
   LDY   #$A8
   STA   (FP),Y
   LDY   #$6B
   LDA   (FP),Y
   LDY   #$A9
   STA   (FP),Y
   LDY   #$6C
   LDA   (FP),Y
   LDY   #$AA
   STA   (FP),Y
   LDA   ga3
   LDY   #$6D
   STA   (FP),Y
   LDA   ga3+1
   INY
   STA   (FP),Y
   LDA   ga3+2
   INY
   STA   (FP),Y
   LDA   ga3+3
   INY
   STA   (FP),Y
   LDY   #$6D
   LDA   (FP),Y
   LDY   #$71
   STA   (FP),Y
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
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$67
   LDA   (FP),Y
   CLC
   LDY   #$71
   ADC   (FP),Y
   LDY   #$77
   STA   (FP),Y
   LDY   #$68
   LDA   (FP),Y
   LDY   #$72
   ADC   (FP),Y
   LDY   #$78
   STA   (FP),Y
   LDY   #$69
   LDA   (FP),Y
   LDY   #$73
   ADC   (FP),Y
   LDY   #$79
   STA   (FP),Y
   LDY   #$6A
   LDA   (FP),Y
   LDY   #$74
   ADC   (FP),Y
   LDY   #$7A
   STA   (FP),Y
   LDY   #$6B
   LDA   (FP),Y
   LDY   #$75
   ADC   (FP),Y
   LDY   #$7B
   STA   (FP),Y
   LDY   #$6C
   LDA   (FP),Y
   LDY   #$76
   ADC   (FP),Y
   LDY   #$7C
   STA   (FP),Y
   LDY   #$77
   LDA   (FP),Y
   LDY   #$7D
   STA   (FP),Y
   LDY   #$78
   LDA   (FP),Y
   LDY   #$7E
   STA   (FP),Y
   LDY   #$79
   LDA   (FP),Y
   LDY   #$7F
   STA   (FP),Y
   LDY   #$7A
   LDA   (FP),Y
   LDY   #$80
   STA   (FP),Y
   LDY   #$7B
   LDA   (FP),Y
   LDY   #$81
   STA   (FP),Y
   LDY   #$7C
   LDA   (FP),Y
   AND   #$0F
   LDY   #$82
   STA   (FP),Y
   LDY   #$7D
   LDA   (FP),Y
   LDY   #$83
   STA   (FP),Y
   LDY   #$7E
   LDA   (FP),Y
   LDY   #$84
   STA   (FP),Y
   LDY   #$7F
   LDA   (FP),Y
   LDY   #$85
   STA   (FP),Y
   LDY   #$80
   LDA   (FP),Y
   LDY   #$86
   STA   (FP),Y
   LDY   #$81
   LDA   (FP),Y
   LDY   #$87
   STA   (FP),Y
   LDY   #$82
   LDA   (FP),Y
   EOR   #$08
   LDY   #$88
   STA   (FP),Y
   LDY   #$83
   LDA   (FP),Y
   LDY   #$89
   STA   (FP),Y
   LDY   #$84
   LDA   (FP),Y
   LDY   #$8A
   STA   (FP),Y
   LDY   #$85
   LDA   (FP),Y
   LDY   #$8B
   STA   (FP),Y
   LDY   #$86
   LDA   (FP),Y
   LDY   #$8C
   STA   (FP),Y
   LDY   #$87
   LDA   (FP),Y
   LDY   #$8D
   STA   (FP),Y
   LDY   #$88
   LDA   (FP),Y
   SEC
   SBC   #$08
   LDY   #$8E
   STA   (FP),Y
   LDY   #$89
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

   ; epilogue
   CLC
   LDA   FP
   ADC   #$B1
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$A3
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   RTS
.default@9:
   LDY   #$B0
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$8F
   STA   (FP),Y
   SEC
   SBC   #$01
   INY
   STA   (FP),Y
   LDY   #$B0
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$90
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$02
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
   BMI   .sx_neg@32
   LDA   #$00
   JMP   .sx_done@33
.sx_neg@32:
   LDA   #$FF
.sx_done@33:
   LDY   #$95
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   ga3+8
   INY
   STA   (FP),Y
   LDA   ga3+9
   INY
   STA   (FP),Y
   LDA   ga3+10
   INY
   STA   (FP),Y
   LDA   ga3+11
   INY
   STA   (FP),Y
   LDY   #$97
   LDA   (FP),Y
   SEC
   LDY   #$93
   SBC   (FP),Y
   LDY   #$98
   LDA   (FP),Y
   LDY   #$94
   SBC   (FP),Y
   LDY   #$99
   LDA   (FP),Y
   LDY   #$95
   SBC   (FP),Y
   LDY   #$9A
   LDA   (FP),Y
   LDY   #$96
   SBC   (FP),Y
   LDA   #$00
   ROL
   LDY   #$9B
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$9D
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$9E
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@34
   LDA   #$00
   JMP   .sx_done@35
.sx_neg@34:
   LDA   #$FF
.sx_done@35:
   LDY   #$9F
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$9D
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

   ; epilogue
   CLC
   LDA   FP
   ADC   #$B1
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

f24:
   SUBROUTINE

   ; prologue: 6 arg bytes, 150 local bytes
   SEC
   LDA   SSP
   SBC   #$98
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$97
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
   LDA   #$7F
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .cond_else@3
   LDA   g2
   INY
   STA   (FP),Y
   LDA   g2
   BMI   .sx_neg@36
   LDA   #$00
   JMP   .sx_done@37
.sx_neg@36:
   LDA   #$FF
.sx_done@37:
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
   JMP   .cond_end@4
.cond_else@3:
   LDA   #<gs4
   LDY   #$11
   STA   (FP),Y
   LDA   #>gs4
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$01
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
   DEY
   LDA   (FP),Y
   LDY   #$0F
   STA   (FP),Y
   LDY   #$17
   LDA   (FP),Y
   LDY   #$10
   STA   (FP),Y
.cond_end@4:
   LDY   #$0F
   LDA   (FP),Y
   LDY   #$18
   STA   (FP),Y
   LDY   #$10
   LDA   (FP),Y
   LDY   #$19
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@38
   LDA   #$00
   JMP   .sx_done@39
.sx_neg@38:
   LDA   #$FF
.sx_done@39:
   LDY   #$1A
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   LDY   #$1E
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@40
   LDA   #$00
   JMP   .sx_done@41
.sx_neg@40:
   LDA   #$FF
.sx_done@41:
   LDY   #$1F
   STA   (FP),Y
   SEC
   LDA   #$00
   DEY
   SBC   (FP),Y
   LDY   #$20
   STA   (FP),Y
   LDA   #$00
   DEY
   SBC   (FP),Y
   LDY   #$21
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$22
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$23
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@42
   LDA   #$00
   JMP   .sx_done@43
.sx_neg@42:
   LDA   #$FF
.sx_done@43:
   LDY   #$24
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #<gs4
   INY
   STA   (FP),Y
   LDA   #>gs4
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$02
   LDY   #$28
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$29
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$2A
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$2B
   STA   (FP),Y
   LDY   #$02
   LDA   (DPTR),Y
   LDY   #$2C
   STA   (FP),Y
   LDY   #$03
   LDA   (DPTR),Y
   LDY   #$2D
   STA   (FP),Y
   LDA   #$05
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$30
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$31
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@44
   LDA   #$00
   JMP   .sx_done@45
.sx_neg@44:
   LDA   #$FF
.sx_done@45:
   LDY   #$32
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$2A
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
   LDY   #$30
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
   LDY   #$34
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
   LDY   #$34
   LDA   (FP),Y
   LDY   #$38
   STA   (FP),Y
   CLC
   LDA   FP
   ADC   #$02
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
   LDY   #$1C
   LDA   (FP),Y
   LDY   #$05
   STA   (SSP),Y
   LDY   #$1D
   LDA   (FP),Y
   LDY   #$06
   STA   (SSP),Y
   LDY   #$22
   LDA   (FP),Y
   LDY   #$07
   STA   (SSP),Y
   LDY   #$23
   LDA   (FP),Y
   LDY   #$08
   STA   (SSP),Y
   LDY   #$24
   LDA   (FP),Y
   LDY   #$09
   STA   (SSP),Y
   LDY   #$25
   LDA   (FP),Y
   LDY   #$0A
   STA   (SSP),Y
   LDY   #$38
   LDA   (FP),Y
   LDY   #$0B
   STA   (SSP),Y
   LDY   #$39
   LDA   (FP),Y
   LDY   #$0C
   STA   (SSP),Y
   LDY   #$3A
   LDA   (FP),Y
   LDY   #$0D
   STA   (SSP),Y
   JSR   f16
   LDA   HARGS+16
   LDY   #$3B
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
   LDA   #<gs4
   INY
   STA   (FP),Y
   LDA   #>gs4
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$01
   LDY   #$43
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$44
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$45
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ORA   #$81
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
   LDY   #$43
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$4A
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$4A
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
   LDY   #$4E
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   JSR   mix
   LDA   ga3+8
   LDY   #$4F
   STA   (FP),Y
   LDA   ga3+9
   INY
   STA   (FP),Y
   LDA   ga3+10
   INY
   STA   (FP),Y
   LDA   ga3+11
   INY
   STA   (FP),Y
   LDY   #$07
   LDA   (FP),Y
   LDY   #$4F
   AND   (FP),Y
   LDY   #$53
   STA   (FP),Y
   LDY   #$08
   LDA   (FP),Y
   LDY   #$50
   AND   (FP),Y
   LDY   #$54
   STA   (FP),Y
   LDY   #$09
   LDA   (FP),Y
   LDY   #$51
   AND   (FP),Y
   LDY   #$55
   STA   (FP),Y
   LDY   #$0A
   LDA   (FP),Y
   LDY   #$52
   AND   (FP),Y
   LDY   #$56
   STA   (FP),Y
   LDY   #$53
   LDA   (FP),Y
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
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$9D
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$5D
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$5E
   STA   (FP),Y
   LDA   #$03
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@46
   LDA   #$00
   JMP   .sx_done@47
.sx_neg@46:
   LDA   #$FF
.sx_done@47:
   LDY   #$61
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$62
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$63
   STA   (FP),Y
   LDY   #$5D
   LDA   (FP),Y
   SEC
   LDY   #$62
   SBC   (FP),Y
   LDY   #$64
   STA   (FP),Y
   LDY   #$5E
   LDA   (FP),Y
   LDY   #$63
   SBC   (FP),Y
   LDY   #$65
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
   CLC
   LDA   FP
   ADC   #$02
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
   LDY   #$57
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$58
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$59
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$5A
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   LDY   #$5B
   LDA   (FP),Y
   LDY   #$05
   STA   (SSP),Y
   LDY   #$5C
   LDA   (FP),Y
   LDY   #$06
   STA   (SSP),Y
   LDY   #$66
   LDA   (FP),Y
   LDY   #$07
   STA   (SSP),Y
   LDY   #$67
   LDA   (FP),Y
   LDY   #$08
   STA   (SSP),Y
   LDY   #$68
   LDA   (FP),Y
   LDY   #$09
   STA   (SSP),Y
   LDY   #$69
   LDA   (FP),Y
   LDY   #$0A
   STA   (SSP),Y
   LDY   #$02
   LDA   (FP),Y
   LDY   #$0B
   STA   (SSP),Y
   LDY   #$6A
   LDA   (FP),Y
   LDY   #$0C
   STA   (SSP),Y
   LDY   #$6B
   LDA   (FP),Y
   LDY   #$0D
   STA   (SSP),Y
   JSR   f16
   LDA   HARGS+16
   LDY   #$6C
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
   LDY   #$6C
   LDA   (FP),Y
   LDY   #$72
   STA   (FP),Y
   LDY   #$6D
   LDA   (FP),Y
   LDY   #$73
   STA   (FP),Y
   LDY   #$6E
   LDA   (FP),Y
   LDY   #$74
   STA   (FP),Y
   LDY   #$6F
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
   LDA   #$02
   LDY   #$76
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$78
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$79
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
   LDY   #$78
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
   LDA   #$FF
   STA   HARGS+8
   STA   HARGS+9
   LDA   #$00
   STA   HARGS+6
   STA   HARGS+7
   STA   HARGS+10
   LDY   #$00
   STY   HARGS+11
   STY   HARGS+12
   STY   HARGS+13
   STA   HARGS+14
   STA   HARGS+15
   JSR   mul64
   LDA   HARGS+16
   LDY   #$7E
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
   LDY   #$7E
   LDA   (FP),Y
   LDY   #$84
   STA   (FP),Y
   LDY   #$7F
   LDA   (FP),Y
   LDY   #$85
   STA   (FP),Y
   LDY   #$80
   LDA   (FP),Y
   LDY   #$86
   STA   (FP),Y
   LDY   #$81
   LDA   (FP),Y
   LDY   #$87
   STA   (FP),Y
   LDY   #$82
   LDA   (FP),Y
   LDY   #$88
   STA   (FP),Y
   LDY   #$83
   LDA   (FP),Y
   AND   #$0F
   LDY   #$89
   STA   (FP),Y
   LDY   #$84
   LDA   (FP),Y
   LDY   #$8A
   STA   (FP),Y
   LDY   #$85
   LDA   (FP),Y
   LDY   #$8B
   STA   (FP),Y
   LDY   #$86
   LDA   (FP),Y
   LDY   #$8C
   STA   (FP),Y
   LDY   #$87
   LDA   (FP),Y
   LDY   #$8D
   STA   (FP),Y
   LDY   #$88
   LDA   (FP),Y
   LDY   #$8E
   STA   (FP),Y
   LDY   #$89
   LDA   (FP),Y
   EOR   #$08
   LDY   #$8F
   STA   (FP),Y
   LDY   #$8A
   LDA   (FP),Y
   LDY   #$90
   STA   (FP),Y
   LDY   #$8B
   LDA   (FP),Y
   LDY   #$91
   STA   (FP),Y
   LDY   #$8C
   LDA   (FP),Y
   LDY   #$92
   STA   (FP),Y
   LDY   #$8D
   LDA   (FP),Y
   LDY   #$93
   STA   (FP),Y
   LDY   #$8E
   LDA   (FP),Y
   LDY   #$94
   STA   (FP),Y
   LDY   #$8F
   LDA   (FP),Y
   SEC
   SBC   #$08
   LDY   #$95
   STA   (FP),Y
   LDY   #$90
   LDA   (FP),Y
   LDY   #$96
   STA   (FP),Y

   ; epilogue
   PHA
   CLC
   LDA   FP
   ADC   #$9E
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

   ; prologue: 0 arg bytes, 198 local bytes
   SEC
   LDA   SSP
   SBC   #$C8
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$C7
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
   LDA   #$FF
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$7F
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
   LDY   #$0A
   LDA   (FP),Y
   LDY   #$0E
   STA   (FP),Y
   LDA   #$03
   INY
   STA   (FP),Y
   LDA   #$00
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
   LDA   #<gs4
   INY
   STA   (FP),Y
   LDA   #>gs4
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$02
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
   LDY   #$02
   LDA   (DPTR),Y
   LDY   #$19
   STA   (FP),Y
   LDY   #$03
   LDA   (DPTR),Y
   LDY   #$1A
   STA   (FP),Y
   LDY   #$05
   LDA   (FP),Y
   LDY   #$1B
   STA   (FP),Y
   LDY   #$06
   LDA   (FP),Y
   LDY   #$1C
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@48
   LDA   #$00
   JMP   .sx_done@49
.sx_neg@48:
   LDA   #$FF
.sx_done@49:
   LDY   #$1D
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$1A
   LDA   (FP),Y
   LDY   #$1E
   CMP   (FP),Y
   BNE   .cmp_true@50
   LDY   #$19
   LDA   (FP),Y
   LDY   #$1D
   CMP   (FP),Y
   BNE   .cmp_true@50
   LDY   #$18
   LDA   (FP),Y
   LDY   #$1C
   CMP   (FP),Y
   BNE   .cmp_true@50
   LDY   #$17
   LDA   (FP),Y
   LDY   #$1B
   CMP   (FP),Y
   BNE   .cmp_true@50
   LDA   #$00
   JMP   .cmp_end@51
.cmp_true@50:
   LDA   #$01
.cmp_end@51:
   LDY   #$1F
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$21
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$22
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@53
   LDA   #$00
   JMP   .sx_done@54
.sx_neg@53:
   LDA   #$FF
.sx_done@54:
   LDY   #$23
   STA   (FP),Y
   INY
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
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   CLC
   LDA   FP
   ADC   #$0B
   INY
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$0C
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
   LDY   #$25
   LDA   (FP),Y
   LDY   #$05
   STA   (SSP),Y
   LDY   #$26
   LDA   (FP),Y
   LDY   #$06
   STA   (SSP),Y
   LDY   #$27
   LDA   (FP),Y
   LDY   #$07
   STA   (SSP),Y
   LDY   #$28
   LDA   (FP),Y
   LDY   #$08
   STA   (SSP),Y
   LDY   #$29
   LDA   (FP),Y
   LDY   #$09
   STA   (SSP),Y
   LDY   #$2A
   LDA   (FP),Y
   LDY   #$0A
   STA   (SSP),Y
   LDY   #$2B
   LDA   (FP),Y
   LDY   #$0B
   STA   (SSP),Y
   LDY   #$2C
   LDA   (FP),Y
   LDY   #$0C
   STA   (SSP),Y
   JSR   f7
   LDA   HARGS+8
   LDY   #$2D
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
   LDY   #$2D
   LDA   (FP),Y
   LDY   #$31
   STA   (FP),Y
   LDY   #$2E
   LDA   (FP),Y
   LDY   #$32
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$11
   STA   (FP),Y
   LDY   #$32
   LDA   (FP),Y
   LDY   #$12
   STA   (FP),Y
   LDA   gp5
   STA   DPTR
   LDA   gp5+1
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
   LDY   #$35
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$36
   STA   (FP),Y
   LDA   gp5
   STA   DPTR
   LDA   gp5+1
   STA   DPTR+1
   DEY
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$36
   LDA   (FP),Y
   LDY   #$01
   STA   (DPTR),Y
   LDA   #$05
   LDY   #$37
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$39
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$3A
   STA   (FP),Y
   LDA   g1
   STA   HARGS
   LDA   g1+1
   STA   HARGS+1
   DEY
   LDA   (FP),Y
   STA   HARGS+2
   INY
   LDA   (FP),Y
   STA   HARGS+3
   JSR   udivmod16
   LDA   HARGS+4
   LDY   #$3B
   STA   (FP),Y
   LDA   HARGS+5
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
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   LDY   #$43
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@55
   LDA   #$00
   JMP   .sx_done@56
.sx_neg@55:
   LDA   #$FF
.sx_done@56:
   LDY   #$44
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$FF
   INY
   STA   (FP),Y
   LDA   #$7F
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$47
   LDA   (FP),Y
   LDY   #$4B
   STA   (FP),Y
   CLC
   LDA   FP
   ADC   #$02
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
   LDY   #$4B
   LDA   (FP),Y
   LDY   #$0B
   STA   (SSP),Y
   LDY   #$4C
   LDA   (FP),Y
   LDY   #$0C
   STA   (SSP),Y
   LDY   #$4D
   LDA   (FP),Y
   LDY   #$0D
   STA   (SSP),Y
   JSR   f16
   LDA   HARGS+16
   LDY   #$4E
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
   LDY   #$4E
   LDA   (FP),Y
   LDY   #$54
   STA   (FP),Y
   LDY   #$4F
   LDA   (FP),Y
   LDY   #$55
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$05
   STA   (FP),Y
   LDY   #$55
   LDA   (FP),Y
   LDY   #$06
   STA   (FP),Y
   LDA   gp6
   STA   DPTR
   LDA   gp6+1
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$56
   STA   (FP),Y
   ORA   #$00
   BNE   .lb_skip@4
   JMP   .if_end@5
.lb_skip@4:
   LDY   #$11
   LDA   (FP),Y
   LDY   #$57
   STA   (FP),Y
   LDY   #$12
   LDA   (FP),Y
   LDY   #$58
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #<gs4
   INY
   STA   (FP),Y
   LDA   #>gs4
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$02
   LDY   #$5D
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$5E
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$5F
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$60
   STA   (FP),Y
   LDY   #$02
   LDA   (DPTR),Y
   LDY   #$61
   STA   (FP),Y
   LDY   #$03
   LDA   (DPTR),Y
   LDY   #$62
   STA   (FP),Y
   LDY   #$5A
   LDA   (FP),Y
   LDY   #$62
   CMP   (FP),Y
   BNE   .cmp_true@57
   LDY   #$59
   LDA   (FP),Y
   LDY   #$61
   CMP   (FP),Y
   BNE   .cmp_true@57
   LDY   #$58
   LDA   (FP),Y
   LDY   #$60
   CMP   (FP),Y
   BNE   .cmp_true@57
   LDY   #$57
   LDA   (FP),Y
   LDY   #$5F
   CMP   (FP),Y
   BNE   .cmp_true@57
   LDA   #$00
   JMP   .cmp_end@58
.cmp_true@57:
   LDA   #$01
.cmp_end@58:
   LDY   #$63
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$65
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$66
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@60
   LDA   #$00
   JMP   .sx_done@61
.sx_neg@60:
   LDA   #$FF
.sx_done@61:
   LDY   #$67
   STA   (FP),Y
   INY
   STA   (FP),Y
   CLC
   LDA   FP
   ADC   #$11
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
   LDY   #$69
   LDA   (FP),Y
   LDY   #$05
   STA   (SSP),Y
   LDY   #$6A
   LDA   (FP),Y
   LDY   #$06
   STA   (SSP),Y
   JSR   f24
   LDY   #$6B
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
   ORA   #$00
   BMI   .sx_neg@62
   LDA   #$00
   JMP   .sx_done@63
.sx_neg@62:
   LDA   #$FF
.sx_done@63:
   LDY   #$74
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   LDY   #$76
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
   LDA   gp6
   STA   DPTR
   LDA   gp6+1
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$7B
   STA   (FP),Y
   CLC
   LDA   FP
   ADC   #$02
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
   LDY   #$70
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$71
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$72
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$73
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   LDY   #$74
   LDA   (FP),Y
   LDY   #$05
   STA   (SSP),Y
   LDY   #$75
   LDA   (FP),Y
   LDY   #$06
   STA   (SSP),Y
   LDY   #$77
   LDA   (FP),Y
   LDY   #$07
   STA   (SSP),Y
   LDY   #$78
   LDA   (FP),Y
   LDY   #$08
   STA   (SSP),Y
   LDY   #$79
   LDA   (FP),Y
   LDY   #$09
   STA   (SSP),Y
   LDY   #$7A
   LDA   (FP),Y
   LDY   #$0A
   STA   (SSP),Y
   LDY   #$7B
   LDA   (FP),Y
   LDY   #$0B
   STA   (SSP),Y
   LDY   #$7C
   LDA   (FP),Y
   LDY   #$0C
   STA   (SSP),Y
   LDY   #$7D
   LDA   (FP),Y
   LDY   #$0D
   STA   (SSP),Y
   JSR   f16
   LDA   HARGS+16
   LDY   #$7E
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
   LDY   #$7E
   LDA   (FP),Y
   LDY   #$84
   STA   (FP),Y
   LDY   #$7F
   LDA   (FP),Y
   LDY   #$85
   STA   (FP),Y
   LDY   #$80
   LDA   (FP),Y
   LDY   #$86
   STA   (FP),Y
   LDY   #$81
   LDA   (FP),Y
   LDY   #$87
   STA   (FP),Y
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
   LDA   #<gs4
   LDY   #$88
   STA   (FP),Y
   LDA   #>gs4
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$01
   LDY   #$8A
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$8B
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$8C
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDA   #$05
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$8D
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
   LDY   #$91
   STA   (FP),Y
   LDA   HARGS+7
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
   BMI   .sx_neg@64
   LDA   #$00
   JMP   .sx_done@65
.sx_neg@64:
   LDA   #$FF
.sx_done@65:
   LDY   #$95
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
   LDY   #$93
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$94
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$95
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$96
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   JSR   mix
.if_end@5:
   LDA   #$00
   LDY   #$97
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   SEC
   SBC   #$01
   LDA   (FP),Y
   SBC   #$00
   BVC   .cmp_novf@66
   EOR   #$80
.cmp_novf@66:
   ASL
   LDA   #$00
   ROL
   LDY   #$9B
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .loop@10_break
   LDA   #<gs4
   INY
   STA   (FP),Y
   LDA   #>gs4
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$01
   LDY   #$9F
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$A0
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$A1
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
   LSR
   LSR
   LDY   #$A4
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ASL
   ASL
   INY
   ORA   (FP),Y
   STA   (FP),Y
   LDY   #$A6
   STA   (FP),Y
   LDY   #$9F
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$A6
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
.loop@10_break:
   LDA   gp6
   STA   DPTR
   LDA   gp6+1
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$A9
   STA   (FP),Y
   ORA   #$00
   BEQ   .lnot_true@67
   LDA   #$00
   JMP   .lnot_end@68
.lnot_true@67:
   LDA   #$01
.lnot_end@68:
   LDY   #$AA
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$AC
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$AD
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@69
   LDA   #$00
   JMP   .sx_done@70
.sx_neg@69:
   LDA   #$FF
.sx_done@70:
   LDY   #$AE
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   gp6
   STA   DPTR
   LDA   gp6+1
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$B2
   STA   (FP),Y
   INY
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@71
   LDA   #$00
   JMP   .sx_done@72
.sx_neg@71:
   LDA   #$FF
.sx_done@72:
   LDY   #$B4
   STA   (FP),Y
   LDY   #$06
   LDA   (FP),Y
   LDY   #$B4
   CMP   (FP),Y
   BNE   .thread@1
   LDY   #$05
   LDA   (FP),Y
   LDY   #$B3
   CMP   (FP),Y
   BEQ   .cmp_true@73
.thread@1:
   LDA   #$00
   JMP   .cmp_end@74
.cmp_true@73:
   LDA   #$01
.cmp_end@74:
   LDY   #$B5
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$B7
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$B8
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@76
   LDA   #$00
   JMP   .sx_done@77
.sx_neg@76:
   LDA   #$FF
.sx_done@77:
   LDY   #$B9
   STA   (FP),Y
   INY
   STA   (FP),Y
   CLC
   LDA   FP
   ADC   #$0B
   INY
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$0C
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDY   #$AC
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$AD
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$AE
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$AF
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   LDY   #$B0
   LDA   (FP),Y
   LDY   #$05
   STA   (SSP),Y
   LDY   #$B1
   LDA   (FP),Y
   LDY   #$06
   STA   (SSP),Y
   LDY   #$B7
   LDA   (FP),Y
   LDY   #$07
   STA   (SSP),Y
   LDY   #$B8
   LDA   (FP),Y
   LDY   #$08
   STA   (SSP),Y
   LDY   #$B9
   LDA   (FP),Y
   LDY   #$09
   STA   (SSP),Y
   LDY   #$BA
   LDA   (FP),Y
   LDY   #$0A
   STA   (SSP),Y
   LDY   #$BB
   LDA   (FP),Y
   LDY   #$0B
   STA   (SSP),Y
   LDY   #$BC
   LDA   (FP),Y
   LDY   #$0C
   STA   (SSP),Y
   JSR   f7
   LDA   HARGS+8
   LDY   #$BD
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
   LDY   #$BD
   LDA   (FP),Y
   LDY   #$C1
   STA   (FP),Y
   LDY   #$BE
   LDA   (FP),Y
   LDY   #$C2
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$05
   STA   (FP),Y
   LDY   #$C2
   LDA   (FP),Y
   LDY   #$06
   STA   (FP),Y
   JSR   fin0
   JSR   fin1
   LDA   acc
   LDY   #$C3
   STA   (FP),Y
   LDA   acc+1
   AND   #$7F
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
   TAX
   DEY
   LDA   (FP),Y

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$C8
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$C7
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
   DC.B  $FF

ga3:
   DC.L  $7FFFFFFF
   DC.L  $00008000
   DC.L  $00000003
   DS.B  4

gs4:
   DS.B  2
   DC.L  $00007FFF

gp5:
   DC.W  g1

gp6:
   DC.W  g2

@13.s23:
   DS.B  1
