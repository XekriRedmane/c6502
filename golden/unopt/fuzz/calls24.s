SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24

f5:
   SUBROUTINE

   ; prologue: 0 arg bytes, 42 local bytes
   SEC
   LDA   SSP
   SBC   #$2C
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$2B
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   LDA   g0
   LDY   #$01
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDA   g0
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDA   g0
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
.mul_loop@0:
   BCC   .mul_skip@1
   CLC
   LDY   #$03
   ADC   (FP),Y
.mul_skip@1:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@0
   LDY   #$08
   STA   (FP),Y
   LDA   HARGS
   DEY
   STA   (FP),Y
   ORA   #$01
   LDY   #$09
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$0A
   STA   (FP),Y
   LDY   #$01
   LDA   (FP),Y
   STA   HARGS
   INY
   LDA   (FP),Y
   STA   HARGS+1
   LDY   #$09
   LDA   (FP),Y
   STA   HARGS+2
   INY
   LDA   (FP),Y
   STA   HARGS+3
   JSR   sdivmod16
   LDA   HARGS+6
   LDY   #$0B
   STA   (FP),Y
   LDA   HARGS+7
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$0D
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   g2
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$0E
   LDA   (FP),Y
   LDY   #$11
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDA   #$07
   SEC
   DEY
   SBC   (FP),Y
   LDA   #$00
   INY
   SBC   (FP),Y
   BVC   .cmp_novf@2
   EOR   #$80
.cmp_novf@2:
   ASL
   LDA   #$00
   ROL
   LDY   #$13
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$0F
   LDA   (FP),Y
   LDY   #$13
   EOR   (FP),Y
   LDY   #$15
   STA   (FP),Y
   LDY   #$10
   LDA   (FP),Y
   LDY   #$14
   EOR   (FP),Y
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
   LDA   #$64
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$0E
   LDA   (FP),Y
   LDY   #$1B
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
   TAX
   BEQ   .shift_done@4
.shift_loop@3:
   LDY   #$20
   LDA   (FP),Y
   LSR
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ROR
   STA   (FP),Y
   DEX
   BNE   .shift_loop@3
.shift_done@4:
   LDY   #$1F
   LDA   (FP),Y
   AND   #$07
   LDY   #$21
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDA   #$00
   LDY   #$22
   STA   (FP),Y
   LDA   #$FF
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$21
   LDA   (FP),Y
   TAX
   BEQ   .shift_done@6
.shift_loop@5:
   LDY   #$24
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
   BNE   .shift_loop@5
.shift_done@6:
   LDY   #$23
   LDA   (FP),Y
   LDY   #$25
   STA   (FP),Y
   DEY
   LDA   (FP),Y
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
   LDA   g0
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
   ADC   #$2C
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$2B
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

f4:
   SUBROUTINE

   ; prologue: 0 arg bytes, 8 local bytes
   SEC
   LDA   SSP
   SBC   #$0A
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$09
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
   LDA   g1
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   SEC
   LDY   #$01
   SBC   (FP),Y
   LDY   #$04
   LDA   (FP),Y
   LDY   #$02
   SBC   (FP),Y
   BVC   .cmp_novf@7
   EOR   #$80
.cmp_novf@7:
   ASL
   LDA   #$00
   ROL
   LDY   #$05
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
   LDX   #$03
   LDA   #$E8

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$0A
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
   TAX
   PLA
   RTS

f3:
   SUBROUTINE

   ; prologue: 5 arg bytes, 46 local bytes
   SEC
   LDA   SSP
   SBC   #$30
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$2F
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
   DEY
   LDA   (FP),Y
   LDY   #$03
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$04
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@8
   LDA   #$00
   JMP   .sx_done@9
.sx_neg@8:
   LDA   #$FF
.sx_done@9:
   LDY   #$05
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$01
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   ORA   #$00
   BPL   .sx_done@11
   LDA   #$FF
.sx_done@11:
   LDY   #$09
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$31
   LDA   (FP),Y
   LDY   #$07
   ORA   (FP),Y
   LDY   #$0B
   STA   (FP),Y
   LDY   #$32
   LDA   (FP),Y
   LDY   #$08
   ORA   (FP),Y
   LDY   #$0C
   STA   (FP),Y
   LDY   #$33
   LDA   (FP),Y
   LDY   #$09
   ORA   (FP),Y
   LDY   #$0D
   STA   (FP),Y
   LDY   #$34
   LDA   (FP),Y
   LDY   #$0A
   ORA   (FP),Y
   LDY   #$0E
   STA   (FP),Y
   LDY   #$03
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
   LDY   #$0B
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
   LDY   #$0F
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
   LDY   #$0F
   LDA   (FP),Y
   LDY   #$13
   STA   (FP),Y
   LDY   #$10
   LDA   (FP),Y
   LDY   #$14
   STA   (FP),Y
   LDY   #$11
   LDA   (FP),Y
   LDY   #$15
   STA   (FP),Y
   LDY   #$12
   LDA   (FP),Y
   LDY   #$16
   STA   (FP),Y
   LDA   #$FF
   INY
   STA   (FP),Y
   LDA   #$7F
   INY
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@12
   LDA   #$00
   JMP   .sx_done@13
.sx_neg@12:
   LDA   #$FF
.sx_done@13:
   LDY   #$19
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$16
   LDA   (FP),Y
   LDY   #$1A
   CMP   (FP),Y
   BNE   .cmp_true@14
   LDY   #$15
   LDA   (FP),Y
   LDY   #$19
   CMP   (FP),Y
   BNE   .cmp_true@14
   LDY   #$14
   LDA   (FP),Y
   LDY   #$18
   CMP   (FP),Y
   BNE   .cmp_true@14
   LDY   #$13
   LDA   (FP),Y
   LDY   #$17
   CMP   (FP),Y
   BNE   .cmp_true@14
   LDA   #$00
   JMP   .cmp_end@15
.cmp_true@14:
   LDA   #$01
.cmp_end@15:
   LDY   #$1B
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$1D
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$35
   LDA   (FP),Y
   LDY   #$20
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDA   g2
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$1E
   LDA   (FP),Y
   LDY   #$24
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   AND   #$07
   LDY   #$26
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDA   #$00
   LDY   #$27
   STA   (FP),Y
   LDY   #$22
   LDA   (FP),Y
   LDY   #$28
   STA   (FP),Y
   LDY   #$23
   LDA   (FP),Y
   LDY   #$29
   STA   (FP),Y
   LDY   #$26
   LDA   (FP),Y
   TAX
   BEQ   .shift_done@18
.shift_loop@17:
   LDY   #$29
   LDA   (FP),Y
   LSR
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ROR
   STA   (FP),Y
   DEX
   BNE   .shift_loop@17
.shift_done@18:
   LDY   #$28
   LDA   (FP),Y
   AND   #$07
   LDY   #$2A
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDA   #$00
   LDY   #$2B
   STA   (FP),Y
   LDY   #$20
   LDA   (FP),Y
   LDY   #$2C
   STA   (FP),Y
   LDY   #$21
   LDA   (FP),Y
   LDY   #$2D
   STA   (FP),Y
   LDY   #$2A
   LDA   (FP),Y
   TAX
   BEQ   .shift_done@20
.shift_loop@19:
   LDY   #$2C
   LDA   (FP),Y
   ASL
   STA   (FP),Y
   INY
   LDA   (FP),Y
   ROL
   STA   (FP),Y
   DEX
   BNE   .shift_loop@19
.shift_done@20:
   LDY   #$2D
   LDA   (FP),Y
   DEY
   ORA   (FP),Y
   BEQ   .cmp_end@21
   LDA   #$01
.cmp_end@21:
   LDY   #$2E
   STA   (FP),Y

   ; epilogue
   PHA
   CLC
   LDA   FP
   ADC   #$35
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

f2:
   SUBROUTINE

   ; prologue: 10 arg bytes, 113 local bytes
   SEC
   LDA   SSP
   SBC   #$73
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$72
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
   DEY
   LDA   (FP),Y
   ORA   #$01
   LDY   #$03
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$04
   STA   (FP),Y
   LDA   #$64
   STA   HARGS
   LDA   #$00
   STA   HARGS+1
   DEY
   LDA   (FP),Y
   STA   HARGS+2
   INY
   LDA   (FP),Y
   STA   HARGS+3
   JSR   sdivmod16
   LDA   HARGS+6
   LDY   #$05
   STA   (FP),Y
   LDA   HARGS+7
   INY
   STA   (FP),Y
   LDA   #$07
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   ORA   #$00
   BPL   .sx_done@23
   LDA   #$FF
.sx_done@23:
   LDY   #$09
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$7A
   LDA   (FP),Y
   LDY   #$07
   AND   (FP),Y
   LDY   #$0B
   STA   (FP),Y
   LDY   #$7B
   LDA   (FP),Y
   LDY   #$08
   AND   (FP),Y
   LDY   #$0C
   STA   (FP),Y
   LDY   #$7C
   LDA   (FP),Y
   LDY   #$09
   AND   (FP),Y
   LDY   #$0D
   STA   (FP),Y
   LDY   #$7D
   LDA   (FP),Y
   LDY   #$0A
   AND   (FP),Y
   LDY   #$0E
   STA   (FP),Y
   LDY   #$05
   LDA   (FP),Y
   LDY   #$0F
   STA   (FP),Y
   LDY   #$06
   LDA   (FP),Y
   LDY   #$10
   STA   (FP),Y
   LDY   #$0B
   LDA   (FP),Y
   TAX
   BEQ   .shift_done@25
.shift_loop@24:
   LDY   #$10
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
   BNE   .shift_loop@24
.shift_done@25:
   LDY   #$0F
   LDA   (FP),Y
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
   DEY
   LDA   (FP),Y
   LDY   #$14
   STA   (FP),Y
   LDA   #$03
   EOR   #$FF
   INY
   STA   (FP),Y
   LDA   #$00
   EOR   #$FF
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$17
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
   LDY   #$1B
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$1C
   STA   (FP),Y
   SEC
   LDA   #$00
   LDY   #$7A
   SBC   (FP),Y
   LDY   #$1D
   STA   (FP),Y
   LDA   #$00
   LDY   #$7B
   SBC   (FP),Y
   LDY   #$1E
   STA   (FP),Y
   LDA   #$00
   LDY   #$7C
   SBC   (FP),Y
   LDY   #$1F
   STA   (FP),Y
   LDA   #$00
   LDY   #$7D
   SBC   (FP),Y
   LDY   #$20
   STA   (FP),Y
   LDA   g0
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$18
   LDA   (FP),Y
   LDY   #$23
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$21
   LDA   (FP),Y
   LDY   #$23
   EOR   (FP),Y
   LDY   #$25
   STA   (FP),Y
   LDY   #$22
   LDA   (FP),Y
   LDY   #$24
   EOR   (FP),Y
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
   BMI   .sx_neg@26
   LDA   #$00
   JMP   .sx_done@27
.sx_neg@26:
   LDA   #$FF
.sx_done@27:
   LDY   #$29
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$27
   LDA   (FP),Y
   CLC
   LDY   #$1D
   SBC   (FP),Y
   LDY   #$28
   LDA   (FP),Y
   LDY   #$1E
   SBC   (FP),Y
   LDY   #$29
   LDA   (FP),Y
   LDY   #$1F
   SBC   (FP),Y
   LDY   #$2A
   LDA   (FP),Y
   LDY   #$20
   SBC   (FP),Y
   LDA   #$00
   ROL
   LDY   #$2B
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$2D
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$2E
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@28
   LDA   #$00
   JMP   .sx_done@29
.sx_neg@28:
   LDA   #$FF
.sx_done@29:
   LDY   #$2F
   STA   (FP),Y
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
   LDY   #$2F
   LDA   (FP),Y
   LDY   #$33
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$13
   LDA   (FP),Y
   LDY   #$35
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   g1
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   g0
   INY
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
   ORA   #$00
   BMI   .sx_neg@30
   LDA   #$00
   JMP   .sx_done@31
.sx_neg@30:
   LDA   #$FF
.sx_done@31:
   LDY   #$3F
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$3D
   LDA   (FP),Y
   SEC
   LDY   #$31
   SBC   (FP),Y
   LDY   #$3E
   LDA   (FP),Y
   LDY   #$32
   SBC   (FP),Y
   LDY   #$3F
   LDA   (FP),Y
   LDY   #$33
   SBC   (FP),Y
   LDY   #$40
   LDA   (FP),Y
   LDY   #$34
   SBC   (FP),Y
   BVC   .cmp_novf@32
   EOR   #$80
.cmp_novf@32:
   ASL
   LDA   #$00
   ROL
   LDY   #$41
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$43
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$05
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDY   #$37
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$38
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$39
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$3A
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   LDY   #$43
   LDA   (FP),Y
   LDY   #$05
   STA   (SSP),Y
   JSR   f3
   LDY   #$44
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$18
   LDA   (FP),Y
   LDY   #$47
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$45
   EOR   (FP),Y
   LDY   #$49
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$46
   EOR   (FP),Y
   LDY   #$4A
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$4B
   STA   (FP),Y
   LDY   #$18
   STA   (FP),Y
   JSR   f5
   LDY   #$4C
   STA   (FP),Y
   TXA
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$4E
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$4F
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@33
   LDA   #$00
   JMP   .sx_done@34
.sx_neg@33:
   LDA   #$FF
.sx_done@34:
   LDY   #$50
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$31
   LDA   (FP),Y
   LDY   #$4E
   EOR   (FP),Y
   LDY   #$52
   STA   (FP),Y
   LDY   #$32
   LDA   (FP),Y
   LDY   #$4F
   EOR   (FP),Y
   LDY   #$53
   STA   (FP),Y
   LDY   #$33
   LDA   (FP),Y
   LDY   #$50
   EOR   (FP),Y
   LDY   #$54
   STA   (FP),Y
   LDY   #$34
   LDA   (FP),Y
   LDY   #$51
   EOR   (FP),Y
   LDY   #$55
   STA   (FP),Y
   LDY   #$52
   LDA   (FP),Y
   LDY   #$31
   STA   (FP),Y
   LDY   #$53
   LDA   (FP),Y
   LDY   #$32
   STA   (FP),Y
   LDY   #$54
   LDA   (FP),Y
   LDY   #$33
   STA   (FP),Y
   LDY   #$55
   LDA   (FP),Y
   LDY   #$34
   STA   (FP),Y
   LDA   g1
   LDY   #$56
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$56
   LDA   (FP),Y
   CLC
   LDY   #$31
   ADC   (FP),Y
   LDY   #$5A
   STA   (FP),Y
   LDY   #$57
   LDA   (FP),Y
   LDY   #$32
   ADC   (FP),Y
   LDY   #$5B
   STA   (FP),Y
   LDY   #$58
   LDA   (FP),Y
   LDY   #$33
   ADC   (FP),Y
   LDY   #$5C
   STA   (FP),Y
   LDY   #$59
   LDA   (FP),Y
   LDY   #$34
   ADC   (FP),Y
   LDY   #$5D
   STA   (FP),Y
   LDY   #$5A
   LDA   (FP),Y
   LDY   #$5E
   STA   (FP),Y
   STA   g1
   LDY   #$1B
   LDA   (FP),Y
   LDY   #$5F
   STA   (FP),Y
   LDY   #$1C
   LDA   (FP),Y
   LDY   #$60
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$74
   LDA   (FP),Y
   CLC
   LDY   #$5F
   ADC   (FP),Y
   LDY   #$63
   STA   (FP),Y
   LDY   #$75
   LDA   (FP),Y
   LDY   #$60
   ADC   (FP),Y
   LDY   #$64
   STA   (FP),Y
   LDY   #$76
   LDA   (FP),Y
   LDY   #$61
   ADC   (FP),Y
   LDY   #$65
   STA   (FP),Y
   LDY   #$77
   LDA   (FP),Y
   LDY   #$62
   ADC   (FP),Y
   LDY   #$66
   STA   (FP),Y
   LDY   #$1B
   LDA   (FP),Y
   ORA   #$64
   LDY   #$67
   STA   (FP),Y
   LDY   #$1C
   LDA   (FP),Y
   LDY   #$68
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$69
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$6A
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$63
   LDA   (FP),Y
   LDY   #$69
   ORA   (FP),Y
   LDY   #$6D
   STA   (FP),Y
   LDY   #$64
   LDA   (FP),Y
   LDY   #$6A
   ORA   (FP),Y
   LDY   #$6E
   STA   (FP),Y
   LDY   #$65
   LDA   (FP),Y
   LDY   #$6B
   ORA   (FP),Y
   LDY   #$6F
   STA   (FP),Y
   LDY   #$66
   LDA   (FP),Y
   LDY   #$6C
   ORA   (FP),Y
   LDY   #$70
   STA   (FP),Y
   LDY   #$6D
   LDA   (FP),Y
   LDY   #$71
   STA   (FP),Y

   ; epilogue
   PHA
   CLC
   LDA   FP
   ADC   #$7D
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

f1:
   SUBROUTINE

   ; prologue: 2 arg bytes, 92 local bytes
   SEC
   LDA   SSP
   SBC   #$5E
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$5D
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   LDA   g2
   LDY   #$01
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDA   g0
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   AND   #$07
   LDY   #$05
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDA   #$00
   LDY   #$06
   STA   (FP),Y
   LDY   #$01
   LDA   (FP),Y
   LDY   #$07
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   LDY   #$08
   STA   (FP),Y
   LDY   #$05
   LDA   (FP),Y
   TAX
   BEQ   .shift_done@36
.shift_loop@35:
   LDY   #$07
   LDA   (FP),Y
   ASL
   STA   (FP),Y
   INY
   LDA   (FP),Y
   ROL
   STA   (FP),Y
   DEX
   BNE   .shift_loop@35
.shift_done@36:
   LDA   g2
   LDY   #$09
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ORA   #$01
   LDY   #$0B
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$0C
   STA   (FP),Y
   LDA   #$02
   STA   HARGS
   LDA   #$00
   STA   HARGS+1
   DEY
   LDA   (FP),Y
   STA   HARGS+2
   INY
   LDA   (FP),Y
   STA   HARGS+3
   JSR   sdivmod16
   LDA   HARGS+6
   LDY   #$0D
   STA   (FP),Y
   LDA   HARGS+7
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   SEC
   LDY   #$07
   SBC   (FP),Y
   LDY   #$0E
   LDA   (FP),Y
   LDY   #$08
   SBC   (FP),Y
   BVC   .cmp_novf@37
   EOR   #$80
.cmp_novf@37:
   ASL
   LDA   #$00
   ROL
   EOR   #$01
   LDY   #$0F
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$11
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$03
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   ORA   #$00
   BPL   .sx_done@39
   LDA   #$FF
.sx_done@39:
   LDY   #$15
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$13
   LDA   (FP),Y
   LDY   #$17
   STA   (FP),Y
   LDY   #$14
   LDA   (FP),Y
   LDY   #$18
   STA   (FP),Y
   LDY   #$15
   LDA   (FP),Y
   LDY   #$19
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   g1
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDA   #$03
   SEC
   DEY
   SBC   (FP),Y
   LDA   #$00
   INY
   SBC   (FP),Y
   BVC   .cmp_novf@40
   EOR   #$80
.cmp_novf@40:
   ASL
   LDA   #$00
   ROL
   LDY   #$1D
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDA   g2
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDA   g1
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$1F
   LDA   (FP),Y
   LDY   #$21
   EOR   (FP),Y
   LDY   #$23
   STA   (FP),Y
   LDY   #$20
   LDA   (FP),Y
   LDY   #$22
   EOR   (FP),Y
   LDY   #$24
   STA   (FP),Y
   LDY   #$1D
   LDA   (FP),Y
   LDY   #$23
   ORA   (FP),Y
   LDY   #$25
   STA   (FP),Y
   LDY   #$1E
   LDA   (FP),Y
   LDY   #$24
   ORA   (FP),Y
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
   DEY
   LDA   (FP),Y
   LDY   #$29
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$2A
   STA   (FP),Y
   LDY   #$5F
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$2B
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$2C
   STA   (FP),Y
   LDY   #$12
   LDA   (FP),Y
   LDY   #$2D
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   SEC
   SBC   #$FF
   LDY   #$2F
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   SBC   #$7F
   LDY   #$30
   STA   (FP),Y
   LDY   #$2B
   LDA   (FP),Y
   CLC
   LDY   #$2F
   ADC   (FP),Y
   LDY   #$31
   STA   (FP),Y
   LDY   #$2C
   LDA   (FP),Y
   LDY   #$30
   ADC   (FP),Y
   LDY   #$32
   STA   (FP),Y
   LDY   #$5F
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$31
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$32
   LDA   (FP),Y
   LDY   #$01
   STA   (DPTR),Y
   LDY   #$5F
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
   LDY   #$35
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$36
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$29
   EOR   (FP),Y
   LDY   #$37
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$2A
   EOR   (FP),Y
   LDY   #$38
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$39
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$3A
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   EOR   #$FF
   INY
   STA   (FP),Y
   LDA   #$00
   EOR   #$FF
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$3F
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$05
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDY   #$39
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$3A
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$3B
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$3C
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   LDY   #$3F
   LDA   (FP),Y
   LDY   #$05
   STA   (SSP),Y
   JSR   f3
   LDY   #$40
   STA   (FP),Y
   ORA   #$00
   BEQ   .if_else@1
   LDY   #$29
   LDA   (FP),Y
   CLC
   ADC   #$03
   LDY   #$41
   STA   (FP),Y
   LDY   #$2A
   LDA   (FP),Y
   ADC   #$00
   LDY   #$42
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$29
   STA   (FP),Y
   LDY   #$42
   LDA   (FP),Y
   LDY   #$2A
   STA   (FP),Y
   JMP   .if_end@0
.if_else@1:
   LDY   #$29
   LDA   (FP),Y
   EOR   #$05
   LDY   #$43
   STA   (FP),Y
   LDY   #$2A
   LDA   (FP),Y
   LDY   #$44
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$29
   STA   (FP),Y
   LDY   #$44
   LDA   (FP),Y
   LDY   #$2A
   STA   (FP),Y
.if_end@0:
   LDA   g0
   LDY   #$45
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   LDY   #$29
   ADC   (FP),Y
   LDY   #$47
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$2A
   ADC   (FP),Y
   LDY   #$48
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$49
   STA   (FP),Y
   STA   g0
   LDA   g1
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDA   #$64
   CLC
   DEY
   ADC   (FP),Y
   LDY   #$4C
   STA   (FP),Y
   LDA   #$00
   DEY
   ADC   (FP),Y
   LDY   #$4D
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$4E
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$4F
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@41
   LDA   #$00
   JMP   .sx_done@42
.sx_neg@41:
   LDA   #$FF
.sx_done@42:
   LDY   #$50
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$29
   LDA   (FP),Y
   LDY   #$52
   STA   (FP),Y
   LDY   #$2A
   LDA   (FP),Y
   LDY   #$53
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$17
   LDA   (FP),Y
   LDY   #$52
   AND   (FP),Y
   LDY   #$56
   STA   (FP),Y
   LDY   #$18
   LDA   (FP),Y
   LDY   #$53
   AND   (FP),Y
   LDY   #$57
   STA   (FP),Y
   LDY   #$19
   LDA   (FP),Y
   LDY   #$54
   AND   (FP),Y
   LDY   #$58
   STA   (FP),Y
   LDY   #$1A
   LDA   (FP),Y
   LDY   #$55
   AND   (FP),Y
   LDY   #$59
   STA   (FP),Y
   LDY   #$4E
   LDA   (FP),Y
   SEC
   LDY   #$56
   SBC   (FP),Y
   LDY   #$4F
   LDA   (FP),Y
   LDY   #$57
   SBC   (FP),Y
   LDY   #$50
   LDA   (FP),Y
   LDY   #$58
   SBC   (FP),Y
   LDY   #$51
   LDA   (FP),Y
   LDY   #$59
   SBC   (FP),Y
   BVC   .cmp_novf@43
   EOR   #$80
.cmp_novf@43:
   ASL
   LDA   #$00
   ROL
   LDY   #$5A
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$5C
   STA   (FP),Y

   ; epilogue
   PHA
   CLC
   LDA   FP
   ADC   #$60
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

f0:
   SUBROUTINE

   ; prologue: 5 arg bytes, 129 local bytes
   SEC
   LDA   SSP
   SBC   #$83
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$82
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   LDY   #$88
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@44
   LDA   #$00
   JMP   .sx_done@45
.sx_neg@44:
   LDA   #$FF
.sx_done@45:
   LDY   #$02
   STA   (FP),Y
   LDY   #$88
   LDA   (FP),Y
   LDY   #$03
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@46
   LDA   #$00
   JMP   .sx_done@47
.sx_neg@46:
   LDA   #$FF
.sx_done@47:
   LDY   #$04
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   AND   #$07
   LDY   #$05
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDA   #$00
   LDY   #$06
   STA   (FP),Y
   LDY   #$01
   LDA   (FP),Y
   LDY   #$07
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   LDY   #$08
   STA   (FP),Y
   LDY   #$05
   LDA   (FP),Y
   TAX
   BEQ   .shift_done@49
.shift_loop@48:
   LDY   #$07
   LDA   (FP),Y
   ASL
   STA   (FP),Y
   INY
   LDA   (FP),Y
   ROL
   STA   (FP),Y
   DEX
   BNE   .shift_loop@48
.shift_done@49:
   LDY   #$07
   LDA   (FP),Y
   EOR   #$FF
   LDY   #$09
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   EOR   #$FF
   LDY   #$0A
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$0B
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   g0
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
   BMI   .sx_neg@50
   LDA   #$00
   JMP   .sx_done@51
.sx_neg@50:
   LDA   #$FF
.sx_done@51:
   LDY   #$11
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$01
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   ORA   #$00
   BPL   .sx_done@53
   LDA   #$FF
.sx_done@53:
   LDY   #$15
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$84
   LDA   (FP),Y
   LDY   #$13
   ORA   (FP),Y
   LDY   #$17
   STA   (FP),Y
   LDY   #$85
   LDA   (FP),Y
   LDY   #$14
   ORA   (FP),Y
   LDY   #$18
   STA   (FP),Y
   LDY   #$86
   LDA   (FP),Y
   LDY   #$15
   ORA   (FP),Y
   LDY   #$19
   STA   (FP),Y
   LDY   #$87
   LDA   (FP),Y
   LDY   #$16
   ORA   (FP),Y
   LDY   #$1A
   STA   (FP),Y
   LDY   #$0F
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
   LDY   #$17
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
   LDY   #$1B
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
   LDA   g2
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$0C
   LDA   (FP),Y
   LDY   #$21
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
   LDY   #$25
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$26
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@54
   LDA   #$00
   JMP   .sx_done@55
.sx_neg@54:
   LDA   #$FF
.sx_done@55:
   LDY   #$27
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$1B
   LDA   (FP),Y
   SEC
   LDY   #$25
   SBC   (FP),Y
   LDY   #$1C
   LDA   (FP),Y
   LDY   #$26
   SBC   (FP),Y
   LDY   #$1D
   LDA   (FP),Y
   LDY   #$27
   SBC   (FP),Y
   LDY   #$1E
   LDA   (FP),Y
   LDY   #$28
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
   LDY   #$2B
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
   AND   #$07
   LDY   #$2F
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDA   #$00
   LDY   #$30
   STA   (FP),Y
   LDY   #$84
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
   LDY   #$2F
   LDA   (FP),Y
   STA   HARGS+4
   JSR   lsr32
   LDA   HARGS+5
   LDY   #$31
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
   LDY   #$31
   LDA   (FP),Y
   LDY   #$84
   EOR   (FP),Y
   LDY   #$35
   STA   (FP),Y
   LDY   #$32
   LDA   (FP),Y
   LDY   #$85
   EOR   (FP),Y
   LDY   #$36
   STA   (FP),Y
   LDY   #$33
   LDA   (FP),Y
   LDY   #$86
   EOR   (FP),Y
   LDY   #$37
   STA   (FP),Y
   LDY   #$34
   LDA   (FP),Y
   LDY   #$87
   EOR   (FP),Y
   LDY   #$38
   STA   (FP),Y
   LDY   #$35
   LDA   (FP),Y
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
   LDY   #$3A
   LDA   (FP),Y
   LDY   #$3E
   STA   (FP),Y
   LDY   #$3B
   LDA   (FP),Y
   LDY   #$3F
   STA   (FP),Y
   LDY   #$3C
   LDA   (FP),Y
   LDY   #$40
   STA   (FP),Y
   LDA   g2
   INY
   STA   (FP),Y
   LDA   #$00
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
   ORA   #$00
   BMI   .sx_neg@56
   LDA   #$00
   JMP   .sx_done@57
.sx_neg@56:
   LDA   #$FF
.sx_done@57:
   LDY   #$45
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$84
   LDA   (FP),Y
   SEC
   LDY   #$43
   SBC   (FP),Y
   LDY   #$85
   LDA   (FP),Y
   LDY   #$44
   SBC   (FP),Y
   LDY   #$86
   LDA   (FP),Y
   LDY   #$45
   SBC   (FP),Y
   LDY   #$87
   LDA   (FP),Y
   LDY   #$46
   SBC   (FP),Y
   LDA   #$00
   ROL
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$0C
   LDA   (FP),Y
   LDY   #$49
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$2C
   LDA   (FP),Y
   LDY   #$4B
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   SEC
   LDY   #$49
   SBC   (FP),Y
   LDY   #$4C
   LDA   (FP),Y
   LDY   #$4A
   SBC   (FP),Y
   BVC   .cmp_novf@58
   EOR   #$80
.cmp_novf@58:
   ASL
   LDA   #$00
   ROL
   EOR   #$01
   LDY   #$4D
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   SEC
   LDY   #$47
   SBC   (FP),Y
   LDY   #$4E
   LDA   (FP),Y
   LDY   #$48
   SBC   (FP),Y
   BVC   .cmp_novf@59
   EOR   #$80
.cmp_novf@59:
   ASL
   LDA   #$00
   ROL
   EOR   #$01
   LDY   #$4F
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$51
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$52
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$53
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$54
   STA   (FP),Y
   LDA   #$01
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   STA   HARGS+1
   LDA   #$01
   STA   HARGS
   DEY
   LDA   (FP),Y
   STA   HARGS+2
   INY
   LDA   (FP),Y
   STA   HARGS+3
   JSR   sdivmod16
   LDA   HARGS+6
   LDY   #$57
   STA   (FP),Y
   LDA   HARGS+7
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$59
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$5A
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@60
   LDA   #$00
   JMP   .sx_done@61
.sx_neg@60:
   LDA   #$FF
.sx_done@61:
   LDY   #$5B
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$3D
   LDA   (FP),Y
   LDY   #$5D
   STA   (FP),Y
   LDY   #$3E
   LDA   (FP),Y
   LDY   #$5E
   STA   (FP),Y
   LDA   g0
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$5F
   LDA   (FP),Y
   LDY   #$63
   STA   (FP),Y
   LDY   #$60
   LDA   (FP),Y
   LDY   #$64
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$0A
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDY   #$59
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$5A
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$5B
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$5C
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   LDY   #$5D
   LDA   (FP),Y
   LDY   #$05
   STA   (SSP),Y
   LDY   #$5E
   LDA   (FP),Y
   LDY   #$06
   STA   (SSP),Y
   LDY   #$63
   LDA   (FP),Y
   LDY   #$07
   STA   (SSP),Y
   LDY   #$64
   LDA   (FP),Y
   LDY   #$08
   STA   (SSP),Y
   LDY   #$65
   LDA   (FP),Y
   LDY   #$09
   STA   (SSP),Y
   LDY   #$66
   LDA   (FP),Y
   LDY   #$0A
   STA   (SSP),Y
   JSR   f2
   LDY   #$67
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
   LDY   #$3D
   LDA   (FP),Y
   LDY   #$68
   EOR   (FP),Y
   LDY   #$6C
   STA   (FP),Y
   LDY   #$3E
   LDA   (FP),Y
   LDY   #$69
   EOR   (FP),Y
   LDY   #$6D
   STA   (FP),Y
   LDY   #$3F
   LDA   (FP),Y
   LDY   #$6A
   EOR   (FP),Y
   LDY   #$6E
   STA   (FP),Y
   LDY   #$40
   LDA   (FP),Y
   LDY   #$6B
   EOR   (FP),Y
   LDY   #$6F
   STA   (FP),Y
   LDY   #$6C
   LDA   (FP),Y
   LDY   #$3D
   STA   (FP),Y
   LDY   #$6D
   LDA   (FP),Y
   LDY   #$3E
   STA   (FP),Y
   LDY   #$6E
   LDA   (FP),Y
   LDY   #$3F
   STA   (FP),Y
   LDY   #$6F
   LDA   (FP),Y
   LDY   #$40
   STA   (FP),Y
   LDA   g0
   LDY   #$70
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$70
   LDA   (FP),Y
   CLC
   LDY   #$3D
   ADC   (FP),Y
   LDY   #$74
   STA   (FP),Y
   LDY   #$71
   LDA   (FP),Y
   LDY   #$3E
   ADC   (FP),Y
   LDY   #$75
   STA   (FP),Y
   LDY   #$72
   LDA   (FP),Y
   LDY   #$3F
   ADC   (FP),Y
   LDY   #$76
   STA   (FP),Y
   LDY   #$73
   LDA   (FP),Y
   LDY   #$40
   ADC   (FP),Y
   LDY   #$77
   STA   (FP),Y
   LDY   #$74
   LDA   (FP),Y
   LDY   #$78
   STA   (FP),Y
   STA   g0
   LDY   #$53
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@2
   LDY   #$0C
   LDA   (FP),Y
   LDY   #$79
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ORA   #$01
   LDY   #$7B
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$7C
   STA   (FP),Y
   LDA   #$FF
   STA   HARGS
   LDA   #$7F
   STA   HARGS+1
   DEY
   LDA   (FP),Y
   STA   HARGS+2
   INY
   LDA   (FP),Y
   STA   HARGS+3
   JSR   sdivmod16
   LDA   HARGS+6
   LDY   #$7D
   STA   (FP),Y
   LDA   HARGS+7
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@2
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   JMP   .or_end@3
.or_true@2:
   LDA   #$01
   LDY   #$7F
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@3:
   LDY   #$7F
   LDA   (FP),Y
   LDY   #$81
   STA   (FP),Y

   ; epilogue
   PHA
   CLC
   LDA   FP
   ADC   #$88
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

   ; prologue: 0 arg bytes, 48 local bytes
   SEC
   LDA   SSP
   SBC   #$32
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$31
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   LDA   #$79
   LDY   #$01
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   ORA   #$00
   BPL   .sx_done@63
   LDA   #$FF
.sx_done@63:
   LDY   #$03
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$43
   INY
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$05
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
   INY
   LDA   (FP),Y
   STA   (SSP),Y
   JSR   f0
   LDY   #$06
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
   LDA   g0
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
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$0F
   LDA   (FP),Y
   LDY   #$13
   EOR   (FP),Y
   LDY   #$17
   STA   (FP),Y
   LDY   #$10
   LDA   (FP),Y
   LDY   #$14
   EOR   (FP),Y
   LDY   #$18
   STA   (FP),Y
   LDY   #$11
   LDA   (FP),Y
   LDY   #$15
   EOR   (FP),Y
   LDY   #$19
   STA   (FP),Y
   LDY   #$12
   LDA   (FP),Y
   LDY   #$16
   EOR   (FP),Y
   LDY   #$1A
   STA   (FP),Y
   LDA   g2
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$17
   LDA   (FP),Y
   LDY   #$1B
   EOR   (FP),Y
   LDY   #$1F
   STA   (FP),Y
   LDY   #$18
   LDA   (FP),Y
   LDY   #$1C
   EOR   (FP),Y
   LDY   #$20
   STA   (FP),Y
   LDY   #$19
   LDA   (FP),Y
   LDY   #$1D
   EOR   (FP),Y
   LDY   #$21
   STA   (FP),Y
   LDY   #$1A
   LDA   (FP),Y
   LDY   #$1E
   EOR   (FP),Y
   LDY   #$22
   STA   (FP),Y
   LDY   #$0B
   LDA   (FP),Y
   LDY   #$1F
   EOR   (FP),Y
   LDY   #$23
   STA   (FP),Y
   LDY   #$0C
   LDA   (FP),Y
   LDY   #$20
   EOR   (FP),Y
   LDY   #$24
   STA   (FP),Y
   LDY   #$0D
   LDA   (FP),Y
   LDY   #$21
   EOR   (FP),Y
   LDY   #$25
   STA   (FP),Y
   LDY   #$0E
   LDA   (FP),Y
   LDY   #$22
   EOR   (FP),Y
   LDY   #$26
   STA   (FP),Y
   LDY   #$23
   LDA   (FP),Y
   LDY   #$0B
   STA   (FP),Y
   LDY   #$24
   LDA   (FP),Y
   LDY   #$0C
   STA   (FP),Y
   LDY   #$25
   LDA   (FP),Y
   LDY   #$0D
   STA   (FP),Y
   LDY   #$26
   LDA   (FP),Y
   LDY   #$0E
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$27
   STA   (FP),Y
   LDY   #$0E
   LDA   (FP),Y
   LDY   #$28
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$0B
   LDA   (FP),Y
   LDY   #$27
   EOR   (FP),Y
   LDY   #$2B
   STA   (FP),Y
   LDY   #$0C
   LDA   (FP),Y
   LDY   #$28
   EOR   (FP),Y
   LDY   #$2C
   STA   (FP),Y
   LDY   #$0D
   LDA   (FP),Y
   LDY   #$29
   EOR   (FP),Y
   LDY   #$2D
   STA   (FP),Y
   LDY   #$0E
   LDA   (FP),Y
   LDY   #$2A
   EOR   (FP),Y
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
   TAX
   DEY
   LDA   (FP),Y

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$32
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$31
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

g0:
   DC.B  $37

g1:
   DC.B  $31

g2:
   DC.B  $AB
