SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24

f5:
   SUBROUTINE

   ; prologue: 6 arg bytes, 35 local bytes
   SEC
   LDA   SSP
   SBC   #$25
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$24
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   LDY   #$28
   LDA   (FP),Y
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
   DEY
   LDA   (FP),Y
   LDY   #$04
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$05
   STA   (FP),Y
   LDY   #$26
   LDA   (FP),Y
   LDY   #$06
   STA   (FP),Y
   LDY   #$27
   LDA   (FP),Y
   LDY   #$07
   STA   (FP),Y
   LDA   g1
   DEY
   EOR   (FP),Y
   LDY   #$08
   STA   (FP),Y
   LDA   g1+1
   DEY
   EOR   (FP),Y
   LDY   #$09
   STA   (FP),Y
   LDY   #$04
   LDA   (FP),Y
   CLC
   LDY   #$08
   SBC   (FP),Y
   LDY   #$05
   LDA   (FP),Y
   LDY   #$09
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
   LDY   #$0C
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$0D
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@2
   LDA   #$00
   JMP   .sx_done@3
.sx_neg@2:
   LDA   #$FF
.sx_done@3:
   LDY   #$0E
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
   LDY   #$0E
   LDA   (FP),Y
   LDY   #$12
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$28
   LDA   (FP),Y
   LDY   #$14
   STA   (FP),Y
   LDY   #$29
   LDA   (FP),Y
   LDY   #$15
   STA   (FP),Y
   CMP   g1+1
   BNE   .cmp_true@4
   DEY
   LDA   (FP),Y
   CMP   g1
   BNE   .cmp_true@4
   LDA   #$00
   JMP   .cmp_end@5
.cmp_true@4:
   LDA   #$01
.cmp_end@5:
   LDY   #$16
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$18
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$19
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@7
   LDA   #$00
   JMP   .sx_done@8
.sx_neg@7:
   LDA   #$FF
.sx_done@8:
   LDY   #$1A
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$18
   LDA   (FP),Y
   LDY   #$1C
   STA   (FP),Y
   LDY   #$19
   LDA   (FP),Y
   LDY   #$1D
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$1E
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$1F
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   ORA   #$00
   BPL   .sx_done@10
   LDA   #$FF
.sx_done@10:
   LDY   #$22
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$20
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
   ADC   #$2B
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

f4:
   SUBROUTINE

   ; prologue: 9 arg bytes, 65 local bytes
   SEC
   LDA   SSP
   SBC   #$43
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$42
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   LDA   #$FF
   BEQ   .and_false@0
   LDA   g1
   SEC
   SBC   g1
   LDY   #$01
   STA   (FP),Y
   LDA   g1+1
   SBC   g1+1
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@0
   LDA   #$01
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   JMP   .and_end@1
.and_false@0:
   LDA   #$00
   LDY   #$03
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@1:
   LDY   #$03
   LDA   (FP),Y
   LDY   #$05
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$06
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@11
   LDA   #$00
   JMP   .sx_done@12
.sx_neg@11:
   LDA   #$FF
.sx_done@12:
   LDY   #$07
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
   LDY   #$07
   LDA   (FP),Y
   LDY   #$0B
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$49
   LDA   (FP),Y
   LDY   #$0D
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@13
   LDA   #$00
   JMP   .sx_done@14
.sx_neg@13:
   LDA   #$FF
.sx_done@14:
   LDY   #$10
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$11
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$12
   STA   (FP),Y
   LDY   #$49
   LDA   (FP),Y
   LDY   #$11
   ORA   (FP),Y
   LDY   #$13
   STA   (FP),Y
   LDY   #$4A
   LDA   (FP),Y
   LDY   #$12
   ORA   (FP),Y
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
   INY
   STA   (FP),Y
   LDY   #$45
   LDA   (FP),Y
   LDY   #$15
   EOR   (FP),Y
   LDY   #$19
   STA   (FP),Y
   LDY   #$46
   LDA   (FP),Y
   LDY   #$16
   EOR   (FP),Y
   LDY   #$1A
   STA   (FP),Y
   LDY   #$47
   LDA   (FP),Y
   LDY   #$17
   EOR   (FP),Y
   LDY   #$1B
   STA   (FP),Y
   LDY   #$48
   LDA   (FP),Y
   LDY   #$18
   EOR   (FP),Y
   LDY   #$1C
   STA   (FP),Y
   LDY   #$19
   LDA   (FP),Y
   LDY   #$1D
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$0E
   LDA   (FP),Y
   LDY   #$1F
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@15
   LDA   #$00
   JMP   .sx_done@16
.sx_neg@15:
   LDA   #$FF
.sx_done@16:
   LDY   #$20
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ORA   #$01
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
   DEY
   LDA   (FP),Y
   LDY   #$24
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
   LDY   #$25
   STA   (FP),Y
   LDA   HARGS+5
   INY
   STA   (FP),Y
   LDA   #$07
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   ORA   #$00
   BPL   .sx_done@18
   LDA   #$FF
.sx_done@18:
   LDY   #$29
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   g0
   LDY   #$27
   AND   (FP),Y
   LDY   #$2B
   STA   (FP),Y
   LDA   g0+1
   LDY   #$28
   AND   (FP),Y
   LDY   #$2C
   STA   (FP),Y
   LDA   g0+2
   LDY   #$29
   AND   (FP),Y
   LDY   #$2D
   STA   (FP),Y
   LDA   g0+3
   LDY   #$2A
   AND   (FP),Y
   LDY   #$2E
   STA   (FP),Y
   LDY   #$25
   LDA   (FP),Y
   LDY   #$2F
   STA   (FP),Y
   LDY   #$26
   LDA   (FP),Y
   LDY   #$30
   STA   (FP),Y
   LDY   #$2B
   LDA   (FP),Y
   TAX
   BEQ   .shift_done@20
.shift_loop@19:
   LDY   #$2F
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
   LDY   #$2F
   LDA   (FP),Y
   LDY   #$31
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$4B
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
   LDA   #$00
   BNE   .lnot_end@22
   LDA   #$01
.lnot_end@22:
   LDY   #$35
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$33
   LDA   (FP),Y
   CLC
   LDY   #$35
   ADC   (FP),Y
   LDY   #$37
   STA   (FP),Y
   LDY   #$34
   LDA   (FP),Y
   LDY   #$36
   ADC   (FP),Y
   LDY   #$38
   STA   (FP),Y
   LDY   #$4B
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$37
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$38
   LDA   (FP),Y
   LDY   #$01
   STA   (DPTR),Y
   LDY   #$44
   LDA   (FP),Y
   LDY   #$39
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$3B
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$3C
   STA   (FP),Y
   LDY   #$49
   LDA   (FP),Y
   LDY   #$3B
   ORA   (FP),Y
   LDY   #$3D
   STA   (FP),Y
   LDY   #$4A
   LDA   (FP),Y
   LDY   #$3C
   ORA   (FP),Y
   LDY   #$3E
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   SEC
   SBC   #$E8
   INY
   LDA   (FP),Y
   SBC   #$03
   LDA   #$00
   ROL
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$41
   STA   (FP),Y
   CMP   #$01

   ; epilogue
   PHP
   CLC
   LDA   FP
   ADC   #$4C
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
   PLP
   RTS

f3:
   SUBROUTINE

   ; prologue: 3 arg bytes, 37 local bytes
   SEC
   LDA   SSP
   SBC   #$27
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$26
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   LDA   g0
   AND   g0
   LDY   #$01
   STA   (FP),Y
   LDA   g0+1
   AND   g0+1
   INY
   STA   (FP),Y
   LDA   g0+2
   AND   g0+2
   INY
   STA   (FP),Y
   LDA   g0+3
   AND   g0+3
   INY
   STA   (FP),Y
   LDY   #$28
   LDA   (FP),Y
   LDY   #$05
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ORA   #$01
   LDY   #$07
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$08
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$09
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$0A
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@23
   LDA   #$00
   JMP   .sx_done@24
.sx_neg@23:
   LDA   #$FF
.sx_done@24:
   LDY   #$0B
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
   LDY   #$09
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
   LDA   HARGS+12
   LDY   #$0D
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
   LDY   #$0D
   LDA   (FP),Y
   LDY   #$11
   STA   (FP),Y
   INY
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
   LDY   #$14
   STA   (FP),Y
   LDA   #$07
   SEC
   DEY
   SBC   (FP),Y
   LDA   #$00
   INY
   SBC   (FP),Y
   BVC   .cmp_novf@27
   EOR   #$80
.cmp_novf@27:
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
   BNE   .or_true@2
   LDA   g0
   INY
   STA   (FP),Y
   ORA   #$00
   BNE   .or_true@2
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   JMP   .or_end@3
.or_true@2:
   LDA   #$01
   LDY   #$18
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@3:
   LDY   #$18
   LDA   (FP),Y
   LDY   #$1A
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$1B
   STA   (FP),Y
   LDY   #$12
   LDA   (FP),Y
   LDY   #$1C
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   g0
   INY
   STA   (FP),Y
   LDA   g0+1
   INY
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
   BMI   .sx_neg@28
   LDA   #$00
   JMP   .sx_done@29
.sx_neg@28:
   LDA   #$FF
.sx_done@29:
   LDY   #$24
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$22
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
   ADC   #$2A
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

f2:
   SUBROUTINE

   ; prologue: 8 arg bytes, 68 local bytes
   SEC
   LDA   SSP
   SBC   #$46
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$45
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
   LDA   g0
   SEC
   LDY   #$01
   SBC   (FP),Y
   LDA   g0+1
   INY
   SBC   (FP),Y
   LDA   g0+2
   INY
   SBC   (FP),Y
   LDA   g0+3
   INY
   SBC   (FP),Y
   BVC   .cmp_novf@30
   EOR   #$80
.cmp_novf@30:
   ASL
   LDA   #$00
   ROL
   EOR   #$01
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
   ORA   #$00
   BMI   .sx_neg@31
   LDA   #$00
   JMP   .sx_done@32
.sx_neg@31:
   LDA   #$FF
.sx_done@32:
   LDY   #$09
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$4B
   LDA   (FP),Y
   SEC
   LDY   #$07
   SBC   (FP),Y
   LDY   #$0B
   STA   (FP),Y
   LDY   #$4C
   LDA   (FP),Y
   LDY   #$08
   SBC   (FP),Y
   LDY   #$0C
   STA   (FP),Y
   LDY   #$4D
   LDA   (FP),Y
   LDY   #$09
   SBC   (FP),Y
   LDY   #$0D
   STA   (FP),Y
   LDY   #$4E
   LDA   (FP),Y
   LDY   #$0A
   SBC   (FP),Y
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
   LDY   #$0D
   LDA   (FP),Y
   LDY   #$11
   STA   (FP),Y
   LDY   #$0E
   LDA   (FP),Y
   LDY   #$12
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
   LDY   #$13
   LDA   (FP),Y
   LDY   #$17
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$14
   LDA   (FP),Y
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
   LDY   #$13
   LDA   (FP),Y
   LDY   #$1D
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
   ORA   #$00
   BPL   .sx_done@34
   LDA   #$FF
.sx_done@34:
   LDY   #$22
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$47
   LDA   (FP),Y
   LDY   #$20
   AND   (FP),Y
   LDY   #$24
   STA   (FP),Y
   LDY   #$48
   LDA   (FP),Y
   LDY   #$21
   AND   (FP),Y
   LDY   #$25
   STA   (FP),Y
   LDY   #$49
   LDA   (FP),Y
   LDY   #$22
   AND   (FP),Y
   LDY   #$26
   STA   (FP),Y
   LDY   #$4A
   LDA   (FP),Y
   LDY   #$23
   AND   (FP),Y
   LDY   #$27
   STA   (FP),Y
   LDY   #$1E
   LDA   (FP),Y
   LDY   #$28
   STA   (FP),Y
   LDY   #$1F
   LDA   (FP),Y
   LDY   #$29
   STA   (FP),Y
   LDY   #$24
   LDA   (FP),Y
   TAX
   CPX   #$00
   BEQ   .shift_done@36
.shift_loop@35:
   LDY   #$28
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
   LDY   #$28
   LDA   (FP),Y
   LDY   #$2A
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$2B
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@37
   LDA   #$00
   JMP   .sx_done@38
.sx_neg@37:
   LDA   #$FF
.sx_done@38:
   LDY   #$2C
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$13
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   INY
   ORA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lnot_true@39
   LDA   #$00
   JMP   .lnot_end@40
.lnot_true@39:
   LDA   #$01
.lnot_end@40:
   LDY   #$2E
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
   CLC
   LDA   FP
   ADC   #$1B
   INY
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$09
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDY   #$1D
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$2A
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$2B
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$2C
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   LDY   #$2D
   LDA   (FP),Y
   LDY   #$05
   STA   (SSP),Y
   LDY   #$30
   LDA   (FP),Y
   LDY   #$06
   STA   (SSP),Y
   LDY   #$31
   LDA   (FP),Y
   LDY   #$07
   STA   (SSP),Y
   LDY   #$32
   LDA   (FP),Y
   LDY   #$08
   STA   (SSP),Y
   LDY   #$33
   LDA   (FP),Y
   LDY   #$09
   STA   (SSP),Y
   JSR   f4
   LDA   #$00
   ROL
   LDY   #$34
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
   LDY   #$13
   LDA   (FP),Y
   LDY   #$35
   EOR   (FP),Y
   LDY   #$39
   STA   (FP),Y
   LDY   #$14
   LDA   (FP),Y
   LDY   #$36
   EOR   (FP),Y
   LDY   #$3A
   STA   (FP),Y
   LDY   #$15
   LDA   (FP),Y
   LDY   #$37
   EOR   (FP),Y
   LDY   #$3B
   STA   (FP),Y
   LDY   #$16
   LDA   (FP),Y
   LDY   #$38
   EOR   (FP),Y
   LDY   #$3C
   STA   (FP),Y
   LDY   #$39
   LDA   (FP),Y
   LDY   #$13
   STA   (FP),Y
   LDY   #$3A
   LDA   (FP),Y
   LDY   #$14
   STA   (FP),Y
   LDY   #$3B
   LDA   (FP),Y
   LDY   #$15
   STA   (FP),Y
   LDY   #$3C
   LDA   (FP),Y
   LDY   #$16
   STA   (FP),Y
   LDY   #$1B
   LDA   (FP),Y
   LDY   #$3D
   STA   (FP),Y
   LDY   #$1C
   LDA   (FP),Y
   LDY   #$3E
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@41
   LDA   #$00
   JMP   .sx_done@42
.sx_neg@41:
   LDA   #$FF
.sx_done@42:
   LDY   #$3F
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$13
   LDA   (FP),Y
   CLC
   LDY   #$3D
   ADC   (FP),Y
   LDY   #$41
   STA   (FP),Y
   LDY   #$14
   LDA   (FP),Y
   LDY   #$3E
   ADC   (FP),Y
   LDY   #$42
   STA   (FP),Y
   LDY   #$15
   LDA   (FP),Y
   LDY   #$3F
   ADC   (FP),Y
   LDY   #$43
   STA   (FP),Y
   LDY   #$16
   LDA   (FP),Y
   LDY   #$40
   ADC   (FP),Y
   LDY   #$44
   STA   (FP),Y
   LDY   #$41
   LDA   (FP),Y
   LDY   #$13
   STA   (FP),Y
   LDY   #$42
   LDA   (FP),Y
   LDY   #$14
   STA   (FP),Y
   LDY   #$43
   LDA   (FP),Y
   LDY   #$15
   STA   (FP),Y
   LDY   #$44
   LDA   (FP),Y
   LDY   #$16
   STA   (FP),Y
   SEC

   ; epilogue
   PHP
   CLC
   LDA   FP
   ADC   #$4E
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$45
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   PLP
   RTS

f1:
   SUBROUTINE

   ; prologue: 6 arg bytes, 105 local bytes
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

   LDA   #$07
   LDY   #$01
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   ORA   #$00
   BPL   .sx_done@44
   LDA   #$FF
.sx_done@44:
   LDY   #$03
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$01
   LDA   (FP),Y
   SEC
   LDY   #$6C
   SBC   (FP),Y
   LDY   #$05
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   LDY   #$6D
   SBC   (FP),Y
   LDY   #$06
   STA   (FP),Y
   LDY   #$03
   LDA   (FP),Y
   LDY   #$6E
   SBC   (FP),Y
   LDY   #$07
   STA   (FP),Y
   LDY   #$04
   LDA   (FP),Y
   LDY   #$6F
   SBC   (FP),Y
   LDY   #$08
   STA   (FP),Y
   LDA   g0
   INY
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
   LDY   #$0B
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
   BMI   .sx_neg@47
   LDA   #$00
   JMP   .sx_done@48
.sx_neg@47:
   LDA   #$FF
.sx_done@48:
   LDY   #$0E
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$0C
   LDA   (FP),Y
   SEC
   LDY   #$05
   SBC   (FP),Y
   LDY   #$0D
   LDA   (FP),Y
   LDY   #$06
   SBC   (FP),Y
   LDY   #$0E
   LDA   (FP),Y
   LDY   #$07
   SBC   (FP),Y
   LDY   #$0F
   LDA   (FP),Y
   LDY   #$08
   SBC   (FP),Y
   BVC   .cmp_novf@49
   EOR   #$80
.cmp_novf@49:
   ASL
   LDA   #$00
   ROL
   EOR   #$01
   LDY   #$10
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$12
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$6C
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   INY
   ORA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@4
   LDY   #$13
   LDA   (FP),Y
   BNE   .or_true@4
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   JMP   .or_end@5
.or_true@4:
   LDA   #$01
   LDY   #$14
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@5:
   LDY   #$14
   LDA   (FP),Y
   LDY   #$16
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$17
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@50
   LDA   #$00
   JMP   .sx_done@51
.sx_neg@50:
   LDA   #$FF
.sx_done@51:
   LDY   #$18
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   g0
   CLC
   LDY   #$16
   ADC   (FP),Y
   LDY   #$1A
   STA   (FP),Y
   LDA   g0+1
   LDY   #$17
   ADC   (FP),Y
   LDY   #$1B
   STA   (FP),Y
   LDA   g0+2
   LDY   #$18
   ADC   (FP),Y
   LDY   #$1C
   STA   (FP),Y
   LDA   g0+3
   LDY   #$19
   ADC   (FP),Y
   LDY   #$1D
   STA   (FP),Y
   LDY   #$1A
   LDA   (FP),Y
   LDY   #$1E
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$FF
   EOR   #$FF
   INY
   STA   (FP),Y
   LDA   #$00
   EOR   #$FF
   INY
   STA   (FP),Y
   LDY   #$1F
   LDA   (FP),Y
   LDY   #$22
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@52
   LDA   #$00
   JMP   .sx_done@53
.sx_neg@52:
   LDA   #$FF
.sx_done@53:
   LDY   #$23
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   AND   #$07
   LDY   #$24
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDA   #$00
   LDY   #$25
   STA   (FP),Y
   LDY   #$20
   LDA   (FP),Y
   LDY   #$26
   STA   (FP),Y
   LDY   #$21
   LDA   (FP),Y
   LDY   #$27
   STA   (FP),Y
   LDY   #$24
   LDA   (FP),Y
   TAX
   BEQ   .shift_done@55
.shift_loop@54:
   LDY   #$27
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
   BNE   .shift_loop@54
.shift_done@55:
   LDY   #$26
   LDA   (FP),Y
   LDY   #$28
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$29
   STA   (FP),Y
   LDY   #$70
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
   LDA   g1
   INY
   STA   (FP),Y
   LDA   g1+1
   INY
   STA   (FP),Y
   LDY   #$2A
   LDA   (FP),Y
   CLC
   LDY   #$2C
   ADC   (FP),Y
   LDY   #$2E
   STA   (FP),Y
   LDY   #$2B
   LDA   (FP),Y
   LDY   #$2D
   ADC   (FP),Y
   LDY   #$2F
   STA   (FP),Y
   LDY   #$70
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$2E
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$2F
   LDA   (FP),Y
   LDY   #$01
   STA   (DPTR),Y
   LDA   #<f5
   LDY   #$30
   STA   (FP),Y
   LDA   #>f5
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$32
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$33
   STA   (FP),Y
   LDY   #$6C
   LDA   (FP),Y
   LDY   #$34
   STA   (FP),Y
   LDY   #$6D
   LDA   (FP),Y
   LDY   #$35
   STA   (FP),Y
   LDY   #$70
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
   LDA   #$E8
   DEY
   ORA   (FP),Y
   LDY   #$38
   STA   (FP),Y
   LDA   #$03
   DEY
   ORA   (FP),Y
   LDY   #$39
   STA   (FP),Y
   LDY   #$13
   LDA   (FP),Y
   LDY   #$3A
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@56
   LDA   #$00
   JMP   .sx_done@57
.sx_neg@56:
   LDA   #$FF
.sx_done@57:
   LDY   #$3B
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$3C
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$3D
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@58
   LDA   #$00
   JMP   .sx_done@59
.sx_neg@58:
   LDA   #$FF
.sx_done@59:
   LDY   #$3E
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$3C
   LDA   (FP),Y
   SEC
   SBC   g0
   LDY   #$40
   STA   (FP),Y
   LDY   #$3D
   LDA   (FP),Y
   SBC   g0+1
   LDY   #$41
   STA   (FP),Y
   LDY   #$3E
   LDA   (FP),Y
   SBC   g0+2
   LDY   #$42
   STA   (FP),Y
   LDY   #$3F
   LDA   (FP),Y
   SBC   g0+3
   LDY   #$43
   STA   (FP),Y
   LDY   #$40
   LDA   (FP),Y
   LDY   #$44
   STA   (FP),Y
   LDY   #$41
   LDA   (FP),Y
   LDY   #$45
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$06
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDY   #$34
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$35
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
   LDY   #$44
   LDA   (FP),Y
   LDY   #$05
   STA   (SSP),Y
   LDY   #$45
   LDA   (FP),Y
   LDY   #$06
   STA   (SSP),Y
   LDY   #$32
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   JSR   icall
   LDA   HARGS+8
   LDY   #$46
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
   LDY   #$13
   LDA   (FP),Y
   LDY   #$4A
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@60
   LDA   #$00
   JMP   .sx_done@61
.sx_neg@60:
   LDA   #$FF
.sx_done@61:
   LDY   #$4B
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$4A
   LDA   (FP),Y
   LDY   #$46
   EOR   (FP),Y
   LDY   #$4E
   STA   (FP),Y
   LDY   #$4B
   LDA   (FP),Y
   LDY   #$47
   EOR   (FP),Y
   LDY   #$4F
   STA   (FP),Y
   LDY   #$4C
   LDA   (FP),Y
   LDY   #$48
   EOR   (FP),Y
   LDY   #$50
   STA   (FP),Y
   LDY   #$4D
   LDA   (FP),Y
   LDY   #$49
   EOR   (FP),Y
   LDY   #$51
   STA   (FP),Y
   LDY   #$4E
   LDA   (FP),Y
   LDY   #$52
   STA   (FP),Y
   LDY   #$13
   STA   (FP),Y
   LDY   #$1F
   LDA   (FP),Y
   LDY   #$53
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@62
   LDA   #$00
   JMP   .sx_done@63
.sx_neg@62:
   LDA   #$FF
.sx_done@63:
   LDY   #$54
   STA   (FP),Y
   LDA   #$64
   DEY
   AND   (FP),Y
   LDY   #$55
   STA   (FP),Y
   LDA   #$00
   DEY
   AND   (FP),Y
   LDY   #$56
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$57
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$58
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@64
   LDA   #$00
   JMP   .sx_done@65
.sx_neg@64:
   LDA   #$FF
.sx_done@65:
   LDY   #$59
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$1F
   LDA   (FP),Y
   LDY   #$5B
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@66
   LDA   #$00
   JMP   .sx_done@67
.sx_neg@66:
   LDA   #$FF
.sx_done@67:
   LDY   #$5C
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$5D
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$5E
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@68
   LDA   #$00
   JMP   .sx_done@69
.sx_neg@68:
   LDA   #$FF
.sx_done@69:
   LDY   #$5F
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$6C
   LDA   (FP),Y
   CLC
   LDY   #$5D
   ADC   (FP),Y
   LDY   #$61
   STA   (FP),Y
   LDY   #$6D
   LDA   (FP),Y
   LDY   #$5E
   ADC   (FP),Y
   LDY   #$62
   STA   (FP),Y
   LDY   #$6E
   LDA   (FP),Y
   LDY   #$5F
   ADC   (FP),Y
   LDY   #$63
   STA   (FP),Y
   LDY   #$6F
   LDA   (FP),Y
   LDY   #$60
   ADC   (FP),Y
   LDY   #$64
   STA   (FP),Y
   LDY   #$57
   LDA   (FP),Y
   LDY   #$61
   EOR   (FP),Y
   LDY   #$65
   STA   (FP),Y
   LDY   #$58
   LDA   (FP),Y
   LDY   #$62
   EOR   (FP),Y
   LDY   #$66
   STA   (FP),Y
   LDY   #$59
   LDA   (FP),Y
   LDY   #$63
   EOR   (FP),Y
   LDY   #$67
   STA   (FP),Y
   LDY   #$5A
   LDA   (FP),Y
   LDY   #$64
   EOR   (FP),Y
   LDY   #$68
   STA   (FP),Y
   DEY
   ORA   (FP),Y
   DEY
   ORA   (FP),Y
   DEY
   ORA   (FP),Y
   BEQ   .cmp_end@70
   LDA   #$01
.cmp_end@70:
   LDY   #$69
   STA   (FP),Y

   ; epilogue
   PHA
   CLC
   LDA   FP
   ADC   #$71
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

   ; prologue: 9 arg bytes, 77 local bytes
   SEC
   LDA   SSP
   SBC   #$4F
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$4E
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   LDY   #$51
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   LDY   #$52
   LDA   (FP),Y
   LDY   #$02
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@71
   LDA   #$00
   JMP   .sx_done@72
.sx_neg@71:
   LDA   #$FF
.sx_done@72:
   LDY   #$03
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
   BPL   .sx_done@74
   LDA   #$FF
.sx_done@74:
   LDY   #$07
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$05
   LDA   (FP),Y
   ORA   g0
   LDY   #$09
   STA   (FP),Y
   LDY   #$06
   LDA   (FP),Y
   ORA   g0+1
   LDY   #$0A
   STA   (FP),Y
   LDY   #$07
   LDA   (FP),Y
   ORA   g0+2
   LDY   #$0B
   STA   (FP),Y
   LDY   #$08
   LDA   (FP),Y
   ORA   g0+3
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
   LDY   #$0D
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
   LDY   #$11
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
   LDY   #$11
   LDA   (FP),Y
   LDY   #$15
   STA   (FP),Y
   LDY   #$12
   LDA   (FP),Y
   LDY   #$16
   STA   (FP),Y
   LDY   #$13
   LDA   (FP),Y
   LDY   #$17
   STA   (FP),Y
   LDY   #$14
   LDA   (FP),Y
   LDY   #$18
   STA   (FP),Y
   LDY   #$51
   LDA   (FP),Y
   LDY   #$19
   STA   (FP),Y
   LDY   #$52
   LDA   (FP),Y
   LDY   #$1A
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@75
   LDA   #$00
   JMP   .sx_done@76
.sx_neg@75:
   LDA   #$FF
.sx_done@76:
   LDY   #$1B
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$19
   LDA   (FP),Y
   LDY   #$1D
   STA   (FP),Y
   LDY   #$1A
   LDA   (FP),Y
   LDY   #$1E
   STA   (FP),Y
   LDY   #$1B
   LDA   (FP),Y
   LDY   #$1F
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$51
   LDA   (FP),Y
   EOR   #$FF
   LDY   #$21
   STA   (FP),Y
   LDY   #$52
   LDA   (FP),Y
   EOR   #$FF
   LDY   #$22
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
   BMI   .sx_neg@77
   LDA   #$00
   JMP   .sx_done@78
.sx_neg@77:
   LDA   #$FF
.sx_done@78:
   LDY   #$25
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$23
   LDA   (FP),Y
   SEC
   LDY   #$15
   SBC   (FP),Y
   LDY   #$27
   STA   (FP),Y
   LDY   #$24
   LDA   (FP),Y
   LDY   #$16
   SBC   (FP),Y
   LDY   #$28
   STA   (FP),Y
   LDY   #$25
   LDA   (FP),Y
   LDY   #$17
   SBC   (FP),Y
   LDY   #$29
   STA   (FP),Y
   LDY   #$26
   LDA   (FP),Y
   LDY   #$18
   SBC   (FP),Y
   LDY   #$2A
   STA   (FP),Y
   LDY   #$27
   LDA   (FP),Y
   LDY   #$2B
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
   BPL   .sx_done@80
   LDA   #$FF
.sx_done@80:
   LDY   #$2F
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   g0
   LDY   #$2D
   ORA   (FP),Y
   LDY   #$31
   STA   (FP),Y
   LDA   g0+1
   LDY   #$2E
   ORA   (FP),Y
   LDY   #$32
   STA   (FP),Y
   LDA   g0+2
   LDY   #$2F
   ORA   (FP),Y
   LDY   #$33
   STA   (FP),Y
   LDA   g0+3
   LDY   #$30
   ORA   (FP),Y
   LDY   #$34
   STA   (FP),Y
   LDY   #$31
   LDA   (FP),Y
   LDY   #$35
   STA   (FP),Y
   LDY   #$32
   LDA   (FP),Y
   LDY   #$36
   STA   (FP),Y
   LDY   #$33
   LDA   (FP),Y
   LDY   #$37
   STA   (FP),Y
   LDY   #$34
   LDA   (FP),Y
   LDY   #$38
   STA   (FP),Y
   LDY   #$15
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
   LDY   #$35
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
   LDY   #$39
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
   LDY   #$50
   LDA   (FP),Y
   LDY   #$3D
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@81
   LDA   #$00
   JMP   .sx_done@82
.sx_neg@81:
   LDA   #$FF
.sx_done@82:
   LDY   #$3E
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$3F
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$40
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@83
   LDA   #$00
   JMP   .sx_done@84
.sx_neg@83:
   LDA   #$FF
.sx_done@84:
   LDY   #$41
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$3F
   LDA   (FP),Y
   SEC
   LDY   #$1D
   SBC   (FP),Y
   LDY   #$43
   STA   (FP),Y
   LDY   #$40
   LDA   (FP),Y
   LDY   #$1E
   SBC   (FP),Y
   LDY   #$44
   STA   (FP),Y
   LDY   #$41
   LDA   (FP),Y
   LDY   #$1F
   SBC   (FP),Y
   LDY   #$45
   STA   (FP),Y
   LDY   #$42
   LDA   (FP),Y
   LDY   #$20
   SBC   (FP),Y
   LDY   #$46
   STA   (FP),Y
   LDY   #$43
   LDA   (FP),Y
   LDY   #$47
   STA   (FP),Y
   LDY   #$44
   LDA   (FP),Y
   LDY   #$48
   STA   (FP),Y
   LDY   #$45
   LDA   (FP),Y
   LDY   #$49
   STA   (FP),Y
   LDY   #$46
   LDA   (FP),Y
   LDY   #$4A
   STA   (FP),Y
   LDY   #$39
   LDA   (FP),Y
   SEC
   LDY   #$47
   SBC   (FP),Y
   LDY   #$3A
   LDA   (FP),Y
   LDY   #$48
   SBC   (FP),Y
   LDY   #$3B
   LDA   (FP),Y
   LDY   #$49
   SBC   (FP),Y
   LDY   #$3C
   LDA   (FP),Y
   LDY   #$4A
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
   LDY   #$4D
   STA   (FP),Y

   ; epilogue
   PHA
   CLC
   LDA   FP
   ADC   #$58
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

   ; prologue: 0 arg bytes, 36 local bytes
   SEC
   LDA   SSP
   SBC   #$26
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$25
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   LDA   #$25
   LDY   #$01
   STA   (FP),Y
   LDA   #$26
   INY
   STA   (FP),Y
   LDA   #$01
   INY
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@85
   LDA   #$00
   JMP   .sx_done@86
.sx_neg@85:
   LDA   #$FF
.sx_done@86:
   LDY   #$04
   STA   (FP),Y
   INY
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$09
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDY   #$01
   LDA   (FP),Y
   STA   (SSP),Y
   LDA   #$C3
   INY
   STA   (SSP),Y
   LDA   #$00
   INY
   STA   (SSP),Y
   LDA   #$A1
   INY
   STA   (SSP),Y
   LDA   #$00
   INY
   STA   (SSP),Y
   LDY   #$02
   LDA   (FP),Y
   LDY   #$06
   STA   (SSP),Y
   LDY   #$03
   LDA   (FP),Y
   LDY   #$07
   STA   (SSP),Y
   LDY   #$04
   LDA   (FP),Y
   LDY   #$08
   STA   (SSP),Y
   LDY   #$05
   LDA   (FP),Y
   LDY   #$09
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
   LDA   g0
   LDY   #$0F
   EOR   (FP),Y
   LDY   #$13
   STA   (FP),Y
   LDA   g0+1
   LDY   #$10
   EOR   (FP),Y
   LDY   #$14
   STA   (FP),Y
   LDA   g0+2
   LDY   #$11
   EOR   (FP),Y
   LDY   #$15
   STA   (FP),Y
   LDA   g0+3
   LDY   #$12
   EOR   (FP),Y
   LDY   #$16
   STA   (FP),Y
   LDY   #$0B
   LDA   (FP),Y
   LDY   #$13
   EOR   (FP),Y
   LDY   #$17
   STA   (FP),Y
   LDY   #$0C
   LDA   (FP),Y
   LDY   #$14
   EOR   (FP),Y
   LDY   #$18
   STA   (FP),Y
   LDY   #$0D
   LDA   (FP),Y
   LDY   #$15
   EOR   (FP),Y
   LDY   #$19
   STA   (FP),Y
   LDY   #$0E
   LDA   (FP),Y
   LDY   #$16
   EOR   (FP),Y
   LDY   #$1A
   STA   (FP),Y
   LDY   #$17
   LDA   (FP),Y
   LDY   #$0B
   STA   (FP),Y
   LDY   #$18
   LDA   (FP),Y
   LDY   #$0C
   STA   (FP),Y
   LDY   #$19
   LDA   (FP),Y
   LDY   #$0D
   STA   (FP),Y
   LDY   #$1A
   LDA   (FP),Y
   LDY   #$0E
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
   LDA   #$10
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
   LDY   #$0B
   LDA   (FP),Y
   LDY   #$1B
   EOR   (FP),Y
   LDY   #$1F
   STA   (FP),Y
   LDY   #$0C
   LDA   (FP),Y
   LDY   #$1C
   EOR   (FP),Y
   LDY   #$20
   STA   (FP),Y
   LDY   #$0D
   LDA   (FP),Y
   LDY   #$1D
   EOR   (FP),Y
   LDY   #$21
   STA   (FP),Y
   LDY   #$0E
   LDA   (FP),Y
   LDY   #$1E
   EOR   (FP),Y
   LDY   #$22
   STA   (FP),Y
   LDY   #$1F
   LDA   (FP),Y
   LDY   #$23
   STA   (FP),Y
   LDY   #$20
   LDA   (FP),Y
   LDY   #$24
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
   ADC   #$26
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$25
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
   DC.L  $00000088

g1:
   DC.W  $007A
