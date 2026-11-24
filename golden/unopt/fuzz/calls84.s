SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24

f4:
   SUBROUTINE

   ; prologue: 6 arg bytes, 66 local bytes
   SEC
   LDA   SSP
   SBC   #$44
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$43
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   LDY   #$47
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lnot_true@0
   LDA   #$00
   JMP   .lnot_end@1
.lnot_true@0:
   LDA   #$01
.lnot_end@1:
   LDY   #$01
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   AND   #$07
   LDY   #$03
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDA   #$00
   LDY   #$04
   STA   (FP),Y
   LDA   g0
   INY
   STA   (FP),Y
   LDY   #$03
   LDA   (FP),Y
   TAX
   BEQ   .shift_done@3
.shift_loop@2:
   LDY   #$05
   LDA   (FP),Y
   ASL
   LDA   (FP),Y
   ROR
   STA   (FP),Y
   DEX
   BNE   .shift_loop@2
.shift_done@3:
   LDY   #$05
   LDA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$01
   CLC
   INY
   STA   (FP),Y
   LDA   #$00
   ADC   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@0
   LDY   #$45
   LDA   (FP),Y
   LDY   #$09
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$46
   LDA   (FP),Y
   LDY   #$0B
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@4
   LDA   #$00
   JMP   .sx_done@5
.sx_neg@4:
   LDA   #$FF
.sx_done@5:
   LDY   #$0C
   STA   (FP),Y
   LDY   #$09
   LDA   (FP),Y
   SEC
   LDY   #$0B
   SBC   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$0C
   SBC   (FP),Y
   BVC   .cmp_novf@6
   EOR   #$80
.cmp_novf@6:
   ASL
   LDA   #$00
   ROL
   EOR   #$01
   LDY   #$0D
   STA   (FP),Y
   LDA   #$00
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
   LDY   #$0F
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@1:
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
   AND   #$07
   LDY   #$13
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDA   #$00
   LDY   #$14
   STA   (FP),Y
   LDY   #$47
   LDA   (FP),Y
   LDY   #$15
   STA   (FP),Y
   LDY   #$48
   LDA   (FP),Y
   LDY   #$16
   STA   (FP),Y
   LDY   #$13
   LDA   (FP),Y
   TAX
   BEQ   .shift_done@8
.shift_loop@7:
   LDY   #$16
   LDA   (FP),Y
   LSR
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ROR
   STA   (FP),Y
   DEX
   BNE   .shift_loop@7
.shift_done@8:
   LDA   g0
   LDY   #$17
   STA   (FP),Y
   LDA   g0
   BMI   .sx_neg@9
   LDA   #$00
   JMP   .sx_done@10
.sx_neg@9:
   LDA   #$FF
.sx_done@10:
   LDY   #$18
   STA   (FP),Y
   LDY   #$47
   LDA   (FP),Y
   LDY   #$17
   EOR   (FP),Y
   LDY   #$19
   STA   (FP),Y
   LDY   #$48
   LDA   (FP),Y
   LDY   #$18
   EOR   (FP),Y
   LDY   #$1A
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   SEC
   LDY   #$15
   SBC   (FP),Y
   LDY   #$1A
   LDA   (FP),Y
   LDY   #$16
   SBC   (FP),Y
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
   ORA   #$00
   BMI   .sx_neg@11
   LDA   #$00
   JMP   .sx_done@12
.sx_neg@11:
   LDA   #$FF
.sx_done@12:
   LDY   #$1F
   STA   (FP),Y
   LDY   #$1D
   LDA   (FP),Y
   LDY   #$20
   STA   (FP),Y
   LDY   #$1E
   LDA   (FP),Y
   LDY   #$21
   STA   (FP),Y
   LDY   #$1F
   LDA   (FP),Y
   LDY   #$22
   STA   (FP),Y
   LDA   g0
   INY
   STA   (FP),Y
   LDA   g0
   BMI   .sx_neg@13
   LDA   #$00
   JMP   .sx_done@14
.sx_neg@13:
   LDA   #$FF
.sx_done@14:
   LDY   #$24
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ORA   #$01
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
   ORA   #$00
   BMI   .sx_neg@15
   LDA   #$00
   JMP   .sx_done@16
.sx_neg@15:
   LDA   #$FF
.sx_done@16:
   LDY   #$29
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
   ASL
   LDA   #$00
   ADC   #$FF
   EOR   #$FF
   STA   HARGS+3
   LDY   #$27
   LDA   (FP),Y
   STA   HARGS+4
   INY
   LDA   (FP),Y
   STA   HARGS+5
   INY
   LDA   (FP),Y
   STA   HARGS+6
   ASL
   LDA   #$00
   ADC   #$FF
   EOR   #$FF
   STA   HARGS+7
   JSR   sdivmod32
   LDA   HARGS+8
   LDY   #$2A
   STA   (FP),Y
   LDA   HARGS+9
   INY
   STA   (FP),Y
   LDA   HARGS+10
   INY
   STA   (FP),Y
   LDY   #$2A
   LDA   (FP),Y
   LDY   #$2D
   STA   (FP),Y
   LDY   #$2B
   LDA   (FP),Y
   LDY   #$2E
   STA   (FP),Y
   LDY   #$2C
   LDA   (FP),Y
   AND   #$7F
   LDY   #$2F
   STA   (FP),Y
   LDY   #$2D
   LDA   (FP),Y
   LDY   #$30
   STA   (FP),Y
   LDY   #$2E
   LDA   (FP),Y
   LDY   #$31
   STA   (FP),Y
   LDY   #$2F
   LDA   (FP),Y
   EOR   #$40
   LDY   #$32
   STA   (FP),Y
   LDY   #$30
   LDA   (FP),Y
   LDY   #$33
   STA   (FP),Y
   LDY   #$31
   LDA   (FP),Y
   LDY   #$34
   STA   (FP),Y
   LDY   #$32
   LDA   (FP),Y
   SEC
   SBC   #$40
   LDY   #$35
   STA   (FP),Y
   LDY   #$33
   LDA   (FP),Y
   LDY   #$36
   STA   (FP),Y
   LDY   #$34
   LDA   (FP),Y
   LDY   #$37
   STA   (FP),Y
   LDY   #$35
   LDA   (FP),Y
   LDY   #$38
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@17
   LDA   #$00
   JMP   .sx_done@18
.sx_neg@17:
   LDA   #$FF
.sx_done@18:
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
   LDY   #$06
   LDA   (FP),Y
   LDY   #$3E
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$22
   LDA   (FP),Y
   BNE   .cmp_true@19
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@19
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@19
   LDA   #$00
   JMP   .cmp_end@20
.cmp_true@19:
   LDA   #$01
.cmp_end@20:
   LDY   #$40
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$42
   STA   (FP),Y
   CMP   #$01

   ; epilogue
   PHP
   CLC
   LDA   FP
   ADC   #$4A
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

   ; prologue: 1 arg bytes, 84 local bytes
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

   INY
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@22
   LDA   #$00
   JMP   .sx_done@23
.sx_neg@22:
   LDA   #$FF
.sx_done@23:
   LDY   #$02
   STA   (FP),Y
   INY
   STA   (FP),Y
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
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$57
   LDA   (FP),Y
   LDY   #$09
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@24
   LDA   #$00
   JMP   .sx_done@25
.sx_neg@24:
   LDA   #$FF
.sx_done@25:
   LDY   #$0A
   STA   (FP),Y
   LDA   #$E9
   INY
   STA   (FP),Y
   LDA   #$03
   INY
   STA   (FP),Y
   LDY   #$09
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
   LDY   #$0D
   STA   (FP),Y
   LDA   HARGS+5
   INY
   STA   (FP),Y
   LDA   g0
   BNE   .or_true@2
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   JMP   .or_end@3
.or_true@2:
   LDA   #$01
   LDY   #$0F
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@3:
   LDY   #$0D
   LDA   (FP),Y
   CLC
   LDY   #$0F
   ADC   (FP),Y
   LDY   #$11
   STA   (FP),Y
   LDY   #$0E
   LDA   (FP),Y
   LDY   #$10
   ADC   (FP),Y
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
   LDY   #$05
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   INY
   ORA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lnot_true@26
   LDA   #$00
   JMP   .lnot_end@27
.lnot_true@26:
   LDA   #$01
.lnot_end@27:
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
   ORA   #$00
   BMI   .sx_neg@28
   LDA   #$00
   JMP   .sx_done@29
.sx_neg@28:
   LDA   #$FF
.sx_done@29:
   LDY   #$19
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$17
   LDA   (FP),Y
   LDY   #$1B
   STA   (FP),Y
   LDY   #$18
   LDA   (FP),Y
   LDY   #$1C
   STA   (FP),Y
   LDY   #$19
   LDA   (FP),Y
   LDY   #$1D
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   ORA   #$00
   BPL   .sx_done@31
   LDA   #$FF
.sx_done@31:
   LDY   #$21
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$08
   CMP   (FP),Y
   BNE   .thread@0
   DEY
   CMP   (FP),Y
   BNE   .thread@0
   LDY   #$20
   LDA   (FP),Y
   LDY   #$06
   CMP   (FP),Y
   BNE   .thread@0
   DEY
   CMP   (FP),Y
   BEQ   .cmp_true@32
.thread@0:
   LDA   #$00
   JMP   .cmp_end@33
.cmp_true@32:
   LDA   #$01
.cmp_end@33:
   LDY   #$23
   STA   (FP),Y
   LDA   #$00
   INY
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
   BMI   .sx_neg@35
   LDA   #$00
   JMP   .sx_done@36
.sx_neg@35:
   LDA   #$FF
.sx_done@36:
   LDY   #$27
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$07
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   ORA   #$00
   BPL   .sx_done@38
   LDA   #$FF
.sx_done@38:
   LDY   #$2B
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$05
   LDA   (FP),Y
   LDY   #$29
   AND   (FP),Y
   LDY   #$2D
   STA   (FP),Y
   LDY   #$06
   LDA   (FP),Y
   LDY   #$2A
   AND   (FP),Y
   LDY   #$2E
   STA   (FP),Y
   LDY   #$07
   LDA   (FP),Y
   LDY   #$2B
   AND   (FP),Y
   LDY   #$2F
   STA   (FP),Y
   LDY   #$08
   LDA   (FP),Y
   LDY   #$2C
   AND   (FP),Y
   LDY   #$30
   STA   (FP),Y
   LDY   #$05
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
   LDY   #$2D
   LDA   (FP),Y
   STA   HARGS+4
   JSR   asl32
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
   LDY   #$25
   LDA   (FP),Y
   LDY   #$31
   EOR   (FP),Y
   LDY   #$35
   STA   (FP),Y
   LDY   #$26
   LDA   (FP),Y
   LDY   #$32
   EOR   (FP),Y
   LDY   #$36
   STA   (FP),Y
   LDY   #$27
   LDA   (FP),Y
   LDY   #$33
   EOR   (FP),Y
   LDY   #$37
   STA   (FP),Y
   LDY   #$28
   LDA   (FP),Y
   LDY   #$34
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
   LDA   #$64
   BEQ   .lnot_true@39
   LDA   #$00
   JMP   .lnot_end@40
.lnot_true@39:
   LDA   #$01
.lnot_end@40:
   LDY   #$41
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$43
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$E8
   INY
   STA   (FP),Y
   LDA   #$03
   INY
   STA   (FP),Y
   LDY   #$44
   LDA   (FP),Y
   TAX
   CPX   #$00
   BEQ   .shift_done@42
.shift_loop@41:
   LDY   #$47
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
   BNE   .shift_loop@41
.shift_done@42:
   LDY   #$46
   LDA   (FP),Y
   LDY   #$48
   STA   (FP),Y
   LDY   #$3D
   LDA   (FP),Y
   LDY   #$49
   STA   (FP),Y
   LDY   #$3E
   LDA   (FP),Y
   LDY   #$4A
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$06
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDY   #$43
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$48
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDA   #$00
   INY
   STA   (SSP),Y
   INY
   STA   (SSP),Y
   LDY   #$49
   LDA   (FP),Y
   LDY   #$05
   STA   (SSP),Y
   LDY   #$4A
   LDA   (FP),Y
   LDY   #$06
   STA   (SSP),Y
   JSR   f4
   LDA   #$00
   ROL
   LDY   #$4B
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
   LDY   #$4C
   EOR   (FP),Y
   LDY   #$50
   STA   (FP),Y
   LDY   #$3E
   LDA   (FP),Y
   LDY   #$4D
   EOR   (FP),Y
   LDY   #$51
   STA   (FP),Y
   LDY   #$3F
   LDA   (FP),Y
   LDY   #$4E
   EOR   (FP),Y
   LDY   #$52
   STA   (FP),Y
   LDY   #$40
   LDA   (FP),Y
   LDY   #$4F
   EOR   (FP),Y
   LDY   #$53
   STA   (FP),Y
   LDY   #$50
   LDA   (FP),Y
   LDY   #$3D
   STA   (FP),Y
   LDY   #$51
   LDA   (FP),Y
   LDY   #$3E
   STA   (FP),Y
   LDY   #$52
   LDA   (FP),Y
   LDY   #$3F
   STA   (FP),Y
   LDY   #$53
   LDA   (FP),Y
   LDY   #$40
   STA   (FP),Y
   LDY   #$1B
   LDA   (FP),Y
   LDY   #$54
   STA   (FP),Y

   ; epilogue
   PHA
   CLC
   LDA   FP
   ADC   #$57
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

   ; prologue: 0 arg bytes, 136 local bytes
   SEC
   LDA   SSP
   SBC   #$8A
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$89
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   LDA   g0
   CLC
   ADC   g0
   LDY   #$01
   STA   (FP),Y
   AND   #$03
   INY
   STA   (FP),Y
   EOR   #$02
   INY
   STA   (FP),Y
   SEC
   SBC   #$02
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@43
   LDA   #$00
   JMP   .sx_done@44
.sx_neg@43:
   LDA   #$FF
.sx_done@44:
   LDY   #$06
   STA   (FP),Y
   LDA   #$64
   SEC
   DEY
   SBC   (FP),Y
   LDA   #$00
   INY
   SBC   (FP),Y
   BVC   .cmp_novf@45
   EOR   #$80
.cmp_novf@45:
   ASL
   LDA   #$00
   ROL
   EOR   #$01
   LDY   #$07
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$09
   STA   (FP),Y
   AND   #$03
   INY
   STA   (FP),Y
   EOR   #$02
   INY
   STA   (FP),Y
   SEC
   SBC   #$02
   INY
   STA   (FP),Y
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
   LDY   #$0F
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   g0
   INY
   STA   (FP),Y
   LDA   g0
   BMI   .sx_neg@48
   LDA   #$00
   JMP   .sx_done@49
.sx_neg@48:
   LDA   #$FF
.sx_done@49:
   LDY   #$13
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ORA   #$01
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
   LDY   #$0E
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
   LDY   #$16
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
   LDY   #$1A
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
   LDY   #$1A
   LDA   (FP),Y
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
   LDY   #$1D
   LDA   (FP),Y
   LDY   #$21
   STA   (FP),Y
   LDY   #$1E
   LDA   (FP),Y
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
   LDY   #$21
   LDA   (FP),Y
   LDY   #$25
   STA   (FP),Y
   LDA   #$00
   BNE   .lnot_end@53
   LDA   #$01
.lnot_end@53:
   LDY   #$26
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@4
   LDY   #$0D
   LDA   (FP),Y
   LDY   #$28
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@54
   LDA   #$00
   JMP   .sx_done@55
.sx_neg@54:
   LDA   #$FF
.sx_done@55:
   LDY   #$29
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$25
   LDA   (FP),Y
   LDY   #$2B
   CMP   (FP),Y
   BNE   .thread@1
   LDY   #$24
   LDA   (FP),Y
   LDY   #$2A
   CMP   (FP),Y
   BNE   .thread@1
   LDY   #$23
   LDA   (FP),Y
   LDY   #$29
   CMP   (FP),Y
   BNE   .thread@1
   LDY   #$22
   LDA   (FP),Y
   LDY   #$28
   CMP   (FP),Y
   BEQ   .cmp_true@56
.thread@1:
   LDA   #$00
   JMP   .cmp_end@57
.cmp_true@56:
   LDA   #$01
.cmp_end@57:
   LDY   #$2C
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
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
   LDY   #$2E
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@5:
   LDY   #$2E
   LDA   (FP),Y
   LDY   #$30
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$31
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@59
   LDA   #$00
   JMP   .sx_done@60
.sx_neg@59:
   LDA   #$FF
.sx_done@60:
   LDY   #$32
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$30
   LDA   (FP),Y
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
   INY
   STA   (FP),Y
   LDY   #$34
   LDA   (FP),Y
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
   LDY   #$38
   LDA   (FP),Y
   LDY   #$3B
   STA   (FP),Y
   LDY   #$39
   LDA   (FP),Y
   LDY   #$3C
   STA   (FP),Y
   LDY   #$3A
   LDA   (FP),Y
   AND   #$7F
   LDY   #$3D
   STA   (FP),Y
   LDY   #$3B
   LDA   (FP),Y
   LDY   #$3E
   STA   (FP),Y
   LDY   #$3C
   LDA   (FP),Y
   LDY   #$3F
   STA   (FP),Y
   LDY   #$3D
   LDA   (FP),Y
   EOR   #$40
   LDY   #$40
   STA   (FP),Y
   LDY   #$3E
   LDA   (FP),Y
   LDY   #$41
   STA   (FP),Y
   LDY   #$3F
   LDA   (FP),Y
   LDY   #$42
   STA   (FP),Y
   LDY   #$40
   LDA   (FP),Y
   SEC
   SBC   #$40
   LDY   #$43
   STA   (FP),Y
   LDY   #$41
   LDA   (FP),Y
   LDY   #$44
   STA   (FP),Y
   LDA   #$07
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   ORA   #$00
   BPL   .sx_done@62
   LDA   #$FF
.sx_done@62:
   LDY   #$47
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
   BPL   .sx_done@64
   LDA   #$FF
.sx_done@64:
   LDY   #$4B
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$34
   LDA   (FP),Y
   LDY   #$49
   ORA   (FP),Y
   LDY   #$4D
   STA   (FP),Y
   LDY   #$35
   LDA   (FP),Y
   LDY   #$4A
   ORA   (FP),Y
   LDY   #$4E
   STA   (FP),Y
   LDY   #$36
   LDA   (FP),Y
   LDY   #$4B
   ORA   (FP),Y
   LDY   #$4F
   STA   (FP),Y
   LDY   #$37
   LDA   (FP),Y
   LDY   #$4C
   ORA   (FP),Y
   LDY   #$50
   STA   (FP),Y
   LDY   #$45
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
   LDY   #$4D
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
   LDY   #$51
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
   LDY   #$51
   LDA   (FP),Y
   LDY   #$55
   STA   (FP),Y
   LDY   #$52
   LDA   (FP),Y
   LDY   #$56
   STA   (FP),Y
   LDA   #$FF
   BEQ   .and_false@6
   LDA   g0
   BEQ   .and_false@6
   LDA   #$01
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   JMP   .and_end@7
.and_false@6:
   LDA   #$00
   LDY   #$57
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@7:
   LDY   #$57
   LDA   (FP),Y
   LDY   #$59
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$5A
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$06
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDY   #$44
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDA   g0
   INY
   STA   (SSP),Y
   LDY   #$55
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$56
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   LDY   #$59
   LDA   (FP),Y
   LDY   #$05
   STA   (SSP),Y
   LDY   #$5A
   LDA   (FP),Y
   LDY   #$06
   STA   (SSP),Y
   JSR   f4
   LDA   #$00
   ROL
   LDY   #$5B
   STA   (FP),Y
   ORA   #$00
   BEQ   .if_else@9
   LDA   #$03
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   ORA   #$00
   BPL   .sx_done@66
   LDA   #$FF
.sx_done@66:
   LDY   #$5E
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$34
   LDA   (FP),Y
   CLC
   LDY   #$5C
   ADC   (FP),Y
   LDY   #$60
   STA   (FP),Y
   LDY   #$35
   LDA   (FP),Y
   LDY   #$5D
   ADC   (FP),Y
   LDY   #$61
   STA   (FP),Y
   LDY   #$36
   LDA   (FP),Y
   LDY   #$5E
   ADC   (FP),Y
   LDY   #$62
   STA   (FP),Y
   LDY   #$37
   LDA   (FP),Y
   LDY   #$5F
   ADC   (FP),Y
   LDY   #$63
   STA   (FP),Y
   LDY   #$60
   LDA   (FP),Y
   LDY   #$34
   STA   (FP),Y
   LDY   #$61
   LDA   (FP),Y
   LDY   #$35
   STA   (FP),Y
   LDY   #$62
   LDA   (FP),Y
   LDY   #$36
   STA   (FP),Y
   LDY   #$63
   LDA   (FP),Y
   LDY   #$37
   STA   (FP),Y
   JMP   .if_end@8
.if_else@9:
   LDA   #$05
   LDY   #$64
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   ORA   #$00
   BPL   .sx_done@68
   LDA   #$FF
.sx_done@68:
   LDY   #$66
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$34
   LDA   (FP),Y
   LDY   #$64
   EOR   (FP),Y
   LDY   #$68
   STA   (FP),Y
   LDY   #$35
   LDA   (FP),Y
   LDY   #$65
   EOR   (FP),Y
   LDY   #$69
   STA   (FP),Y
   LDY   #$36
   LDA   (FP),Y
   LDY   #$66
   EOR   (FP),Y
   LDY   #$6A
   STA   (FP),Y
   LDY   #$37
   LDA   (FP),Y
   LDY   #$67
   EOR   (FP),Y
   LDY   #$6B
   STA   (FP),Y
   LDY   #$68
   LDA   (FP),Y
   LDY   #$34
   STA   (FP),Y
   LDY   #$69
   LDA   (FP),Y
   LDY   #$35
   STA   (FP),Y
   LDY   #$6A
   LDA   (FP),Y
   LDY   #$36
   STA   (FP),Y
   LDY   #$6B
   LDA   (FP),Y
   LDY   #$37
   STA   (FP),Y
.if_end@8:
   LDA   g0
   LDY   #$6C
   STA   (FP),Y
   LDA   g0
   BMI   .sx_neg@69
   LDA   #$00
   JMP   .sx_done@70
.sx_neg@69:
   LDA   #$FF
.sx_done@70:
   LDY   #$6D
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$37
   LDA   (FP),Y
   LDY   #$6F
   CMP   (FP),Y
   BNE   .cmp_true@71
   LDY   #$36
   LDA   (FP),Y
   LDY   #$6E
   CMP   (FP),Y
   BNE   .cmp_true@71
   LDY   #$35
   LDA   (FP),Y
   LDY   #$6D
   CMP   (FP),Y
   BNE   .cmp_true@71
   LDY   #$34
   LDA   (FP),Y
   LDY   #$6C
   CMP   (FP),Y
   BNE   .cmp_true@71
   LDA   #$00
   JMP   .cmp_end@72
.cmp_true@71:
   LDA   #$01
.cmp_end@72:
   LDY   #$70
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$72
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$73
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@74
   LDA   #$00
   JMP   .sx_done@75
.sx_neg@74:
   LDA   #$FF
.sx_done@75:
   LDY   #$74
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
   BPL   .sx_done@77
   LDA   #$FF
.sx_done@77:
   LDY   #$78
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$34
   LDA   (FP),Y
   LDY   #$76
   ORA   (FP),Y
   LDY   #$7A
   STA   (FP),Y
   LDY   #$35
   LDA   (FP),Y
   LDY   #$77
   ORA   (FP),Y
   LDY   #$7B
   STA   (FP),Y
   LDY   #$36
   LDA   (FP),Y
   LDY   #$78
   ORA   (FP),Y
   LDY   #$7C
   STA   (FP),Y
   LDY   #$37
   LDA   (FP),Y
   LDY   #$79
   ORA   (FP),Y
   LDY   #$7D
   STA   (FP),Y
   LDY   #$72
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
   LDY   #$7A
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
   LDY   #$7E
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
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   ORA   #$00
   BPL   .sx_done@79
   LDA   #$FF
.sx_done@79:
   LDY   #$84
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$81
   LDA   (FP),Y
   LDY   #$85
   CMP   (FP),Y
   BNE   .cmp_true@80
   LDY   #$80
   LDA   (FP),Y
   LDY   #$84
   CMP   (FP),Y
   BNE   .cmp_true@80
   LDY   #$7F
   LDA   (FP),Y
   LDY   #$83
   CMP   (FP),Y
   BNE   .cmp_true@80
   LDY   #$7E
   LDA   (FP),Y
   LDY   #$82
   CMP   (FP),Y
   BNE   .cmp_true@80
   LDA   #$00
   JMP   .cmp_end@81
.cmp_true@80:
   LDA   #$01
.cmp_end@81:
   LDY   #$86
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$88
   STA   (FP),Y
   CMP   #$01

   ; epilogue
   PHP
   CLC
   LDA   FP
   ADC   #$8A
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

f1:
   SUBROUTINE

   ; prologue: 12 arg bytes, 155 local bytes
   SEC
   LDA   SSP
   SBC   #$9D
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$9C
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   LDY   #$A2
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   LDY   #$A3
   LDA   (FP),Y
   LDY   #$02
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
   BMI   .sx_neg@83
   LDA   #$00
   JMP   .sx_done@84
.sx_neg@83:
   LDA   #$FF
.sx_done@84:
   LDY   #$05
   STA   (FP),Y
   LDY   #$03
   LDA   (FP),Y
   LDY   #$06
   STA   (FP),Y
   LDY   #$04
   LDA   (FP),Y
   LDY   #$07
   STA   (FP),Y
   LDY   #$05
   LDA   (FP),Y
   LDY   #$08
   STA   (FP),Y
   LDY   #$9E
   LDA   (FP),Y
   BEQ   .and_false@10
   LDY   #$06
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@10
   LDA   #$01
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   JMP   .and_end@11
.and_false@10:
   LDA   #$00
   LDY   #$09
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@11:
   LDY   #$09
   LDA   (FP),Y
   LDY   #$0B
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$0C
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@85
   LDA   #$00
   JMP   .sx_done@86
.sx_neg@85:
   LDA   #$FF
.sx_done@86:
   LDY   #$0D
   STA   (FP),Y
   LDA   g0
   INY
   STA   (FP),Y
   LDA   g0
   BMI   .sx_neg@87
   LDA   #$00
   JMP   .sx_done@88
.sx_neg@87:
   LDA   #$FF
.sx_done@88:
   LDY   #$0F
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$9F
   LDA   (FP),Y
   LDY   #$0E
   AND   (FP),Y
   LDY   #$11
   STA   (FP),Y
   LDY   #$A0
   LDA   (FP),Y
   LDY   #$0F
   AND   (FP),Y
   LDY   #$12
   STA   (FP),Y
   LDY   #$A1
   LDA   (FP),Y
   LDY   #$10
   AND   (FP),Y
   LDY   #$13
   STA   (FP),Y
   LDY   #$0B
   LDA   (FP),Y
   LDY   #$11
   EOR   (FP),Y
   LDY   #$14
   STA   (FP),Y
   LDY   #$0C
   LDA   (FP),Y
   LDY   #$12
   EOR   (FP),Y
   LDY   #$15
   STA   (FP),Y
   LDY   #$0D
   LDA   (FP),Y
   LDY   #$13
   EOR   (FP),Y
   LDY   #$16
   STA   (FP),Y
   LDY   #$14
   LDA   (FP),Y
   LDY   #$17
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@89
   LDA   #$00
   JMP   .sx_done@90
.sx_neg@89:
   LDA   #$FF
.sx_done@90:
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
   ORA   #$00
   BMI   .sx_neg@91
   LDA   #$00
   JMP   .sx_done@92
.sx_neg@91:
   LDA   #$FF
.sx_done@92:
   LDY   #$1D
   STA   (FP),Y
   LDY   #$1B
   LDA   (FP),Y
   SEC
   LDY   #$06
   SBC   (FP),Y
   LDY   #$1C
   LDA   (FP),Y
   LDY   #$07
   SBC   (FP),Y
   LDY   #$1D
   LDA   (FP),Y
   LDY   #$08
   SBC   (FP),Y
   BVC   .cmp_novf@93
   EOR   #$80
.cmp_novf@93:
   ASL
   LDA   #$00
   ROL
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
   BMI   .sx_neg@94
   LDA   #$00
   JMP   .sx_done@95
.sx_neg@94:
   LDA   #$FF
.sx_done@95:
   LDY   #$22
   STA   (FP),Y
   LDY   #$9E
   LDA   (FP),Y
   LDY   #$23
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@96
   LDA   #$00
   JMP   .sx_done@97
.sx_neg@96:
   LDA   #$FF
.sx_done@97:
   LDY   #$24
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$06
   LDA   (FP),Y
   ORA   #$01
   LDY   #$26
   STA   (FP),Y
   LDY   #$07
   LDA   (FP),Y
   LDY   #$27
   STA   (FP),Y
   LDY   #$08
   LDA   (FP),Y
   LDY   #$28
   STA   (FP),Y
   LDY   #$23
   LDA   (FP),Y
   STA   HARGS
   INY
   LDA   (FP),Y
   STA   HARGS+1
   INY
   LDA   (FP),Y
   STA   HARGS+2
   ASL
   LDA   #$00
   ADC   #$FF
   EOR   #$FF
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
   ASL
   LDA   #$00
   ADC   #$FF
   EOR   #$FF
   STA   HARGS+7
   JSR   sdivmod32
   LDA   HARGS+8
   LDY   #$29
   STA   (FP),Y
   LDA   HARGS+9
   INY
   STA   (FP),Y
   LDA   HARGS+10
   INY
   STA   (FP),Y
   LDY   #$29
   LDA   (FP),Y
   LDY   #$2C
   STA   (FP),Y
   LDY   #$2A
   LDA   (FP),Y
   LDY   #$2D
   STA   (FP),Y
   LDY   #$2B
   LDA   (FP),Y
   AND   #$7F
   LDY   #$2E
   STA   (FP),Y
   LDY   #$2C
   LDA   (FP),Y
   LDY   #$2F
   STA   (FP),Y
   LDY   #$2D
   LDA   (FP),Y
   LDY   #$30
   STA   (FP),Y
   LDY   #$2E
   LDA   (FP),Y
   EOR   #$40
   LDY   #$31
   STA   (FP),Y
   LDY   #$2F
   LDA   (FP),Y
   LDY   #$32
   STA   (FP),Y
   LDY   #$30
   LDA   (FP),Y
   LDY   #$33
   STA   (FP),Y
   LDY   #$31
   LDA   (FP),Y
   SEC
   SBC   #$40
   LDY   #$34
   STA   (FP),Y
   LDY   #$32
   LDA   (FP),Y
   SEC
   LDY   #$20
   SBC   (FP),Y
   LDY   #$33
   LDA   (FP),Y
   LDY   #$21
   SBC   (FP),Y
   LDY   #$34
   LDA   (FP),Y
   LDY   #$22
   SBC   (FP),Y
   BVC   .cmp_novf@98
   EOR   #$80
.cmp_novf@98:
   ASL
   LDA   #$00
   ROL
   EOR   #$01
   LDY   #$35
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$37
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$38
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@99
   LDA   #$00
   JMP   .sx_done@100
.sx_neg@99:
   LDA   #$FF
.sx_done@100:
   LDY   #$39
   STA   (FP),Y
   INY
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
   INY
   STA   (FP),Y
   LDY   #$A8
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$3F
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$40
   STA   (FP),Y
   LDA   #$07
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   ORA   #$00
   BPL   .sx_done@102
   LDA   #$FF
.sx_done@102:
   LDY   #$43
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$41
   LDA   (FP),Y
   SEC
   LDY   #$A2
   SBC   (FP),Y
   LDY   #$42
   LDA   (FP),Y
   LDY   #$A3
   SBC   (FP),Y
   LDY   #$43
   LDA   (FP),Y
   LDY   #$A4
   SBC   (FP),Y
   LDY   #$44
   LDA   (FP),Y
   LDY   #$A5
   SBC   (FP),Y
   LDA   #$00
   ROL
   LDY   #$45
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$3F
   LDA   (FP),Y
   CLC
   LDY   #$45
   ADC   (FP),Y
   LDY   #$47
   STA   (FP),Y
   LDY   #$40
   LDA   (FP),Y
   LDY   #$46
   ADC   (FP),Y
   LDY   #$48
   STA   (FP),Y
   LDY   #$A8
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$47
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$48
   LDA   (FP),Y
   LDY   #$01
   STA   (DPTR),Y
   LDA   #$07
   LDY   #$49
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$01
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   JSR   f3
   LDY   #$4A
   STA   (FP),Y
   INY
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@103
   LDA   #$00
   JMP   .sx_done@104
.sx_neg@103:
   LDA   #$FF
.sx_done@104:
   LDY   #$4C
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$06
   LDA   (FP),Y
   LDY   #$4B
   EOR   (FP),Y
   LDY   #$4E
   STA   (FP),Y
   LDY   #$07
   LDA   (FP),Y
   LDY   #$4C
   EOR   (FP),Y
   LDY   #$4F
   STA   (FP),Y
   LDY   #$08
   LDA   (FP),Y
   LDY   #$4D
   EOR   (FP),Y
   LDY   #$50
   STA   (FP),Y
   LDY   #$4E
   LDA   (FP),Y
   LDY   #$06
   STA   (FP),Y
   LDY   #$4F
   LDA   (FP),Y
   LDY   #$07
   STA   (FP),Y
   LDY   #$50
   LDA   (FP),Y
   LDY   #$08
   STA   (FP),Y
   LDA   g0
   LDY   #$51
   STA   (FP),Y
   LDA   g0
   BMI   .sx_neg@105
   LDA   #$00
   JMP   .sx_done@106
.sx_neg@105:
   LDA   #$FF
.sx_done@106:
   LDY   #$52
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$51
   LDA   (FP),Y
   CLC
   LDY   #$06
   ADC   (FP),Y
   LDY   #$54
   STA   (FP),Y
   LDY   #$52
   LDA   (FP),Y
   LDY   #$07
   ADC   (FP),Y
   LDY   #$55
   STA   (FP),Y
   LDY   #$53
   LDA   (FP),Y
   LDY   #$08
   ADC   (FP),Y
   LDY   #$56
   STA   (FP),Y
   LDY   #$54
   LDA   (FP),Y
   LDY   #$57
   STA   (FP),Y
   LDY   #$55
   LDA   (FP),Y
   LDY   #$58
   STA   (FP),Y
   LDY   #$56
   LDA   (FP),Y
   AND   #$7F
   LDY   #$59
   STA   (FP),Y
   LDY   #$57
   LDA   (FP),Y
   LDY   #$5A
   STA   (FP),Y
   LDY   #$58
   LDA   (FP),Y
   LDY   #$5B
   STA   (FP),Y
   LDY   #$59
   LDA   (FP),Y
   EOR   #$40
   LDY   #$5C
   STA   (FP),Y
   LDY   #$5A
   LDA   (FP),Y
   LDY   #$5D
   STA   (FP),Y
   LDY   #$5B
   LDA   (FP),Y
   LDY   #$5E
   STA   (FP),Y
   LDY   #$5C
   LDA   (FP),Y
   SEC
   SBC   #$40
   LDY   #$5F
   STA   (FP),Y
   LDY   #$5D
   LDA   (FP),Y
   LDY   #$60
   STA   (FP),Y
   AND   #$03
   INY
   STA   (FP),Y
   EOR   #$02
   INY
   STA   (FP),Y
   SEC
   SBC   #$02
   INY
   STA   (FP),Y
   STA   g0
   LDY   #$06
   LDA   (FP),Y
   SEC
   SBC   #$E8
   INY
   LDA   (FP),Y
   SBC   #$03
   INY
   LDA   (FP),Y
   SBC   #$00
   BVC   .cmp_novf@107
   EOR   #$80
.cmp_novf@107:
   ASL
   LDA   #$00
   ROL
   EOR   #$01
   LDY   #$64
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$66
   STA   (FP),Y
   LDY   #$A6
   LDA   (FP),Y
   LDY   #$67
   STA   (FP),Y
   LDY   #$A7
   LDA   (FP),Y
   LDY   #$68
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$67
   LDA   (FP),Y
   SEC
   LDY   #$3B
   SBC   (FP),Y
   LDY   #$68
   LDA   (FP),Y
   LDY   #$3C
   SBC   (FP),Y
   LDY   #$69
   LDA   (FP),Y
   LDY   #$3D
   SBC   (FP),Y
   LDY   #$6A
   LDA   (FP),Y
   LDY   #$3E
   SBC   (FP),Y
   LDA   #$00
   ROL
   LDY   #$6B
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$6D
   STA   (FP),Y
   LDY   #$18
   LDA   (FP),Y
   LDY   #$6E
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@108
   LDA   #$00
   JMP   .sx_done@109
.sx_neg@108:
   LDA   #$FF
.sx_done@109:
   LDY   #$6F
   STA   (FP),Y
   LDY   #$9E
   LDA   (FP),Y
   LDY   #$70
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@110
   LDA   #$00
   JMP   .sx_done@111
.sx_neg@110:
   LDA   #$FF
.sx_done@111:
   LDY   #$71
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$9F
   LDA   (FP),Y
   CLC
   LDY   #$70
   ADC   (FP),Y
   LDY   #$73
   STA   (FP),Y
   LDY   #$A0
   LDA   (FP),Y
   LDY   #$71
   ADC   (FP),Y
   LDY   #$74
   STA   (FP),Y
   LDY   #$A1
   LDA   (FP),Y
   LDY   #$72
   ADC   (FP),Y
   LDY   #$75
   STA   (FP),Y
   LDY   #$73
   LDA   (FP),Y
   LDY   #$76
   STA   (FP),Y
   LDY   #$74
   LDA   (FP),Y
   LDY   #$77
   STA   (FP),Y
   LDY   #$75
   LDA   (FP),Y
   AND   #$7F
   LDY   #$78
   STA   (FP),Y
   LDY   #$76
   LDA   (FP),Y
   LDY   #$79
   STA   (FP),Y
   LDY   #$77
   LDA   (FP),Y
   LDY   #$7A
   STA   (FP),Y
   LDY   #$78
   LDA   (FP),Y
   EOR   #$40
   LDY   #$7B
   STA   (FP),Y
   LDY   #$79
   LDA   (FP),Y
   LDY   #$7C
   STA   (FP),Y
   LDY   #$7A
   LDA   (FP),Y
   LDY   #$7D
   STA   (FP),Y
   LDY   #$7B
   LDA   (FP),Y
   SEC
   SBC   #$40
   LDY   #$7E
   STA   (FP),Y
   LDY   #$7C
   LDA   (FP),Y
   LDY   #$7F
   STA   (FP),Y
   LDY   #$7D
   LDA   (FP),Y
   LDY   #$80
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$06
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDY   #$66
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$6D
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$6E
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$6F
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   LDY   #$7F
   LDA   (FP),Y
   LDY   #$05
   STA   (SSP),Y
   LDY   #$80
   LDA   (FP),Y
   LDY   #$06
   STA   (SSP),Y
   JSR   f4
   LDA   #$00
   ROL
   LDY   #$81
   STA   (FP),Y
   ORA   #$00
   BNE   .lb_skip@0
   JMP   .if_else@13
.lb_skip@0:
   LDY   #$06
   LDA   (FP),Y
   CLC
   ADC   #$03
   LDY   #$82
   STA   (FP),Y
   LDY   #$07
   LDA   (FP),Y
   ADC   #$00
   LDY   #$83
   STA   (FP),Y
   LDY   #$08
   LDA   (FP),Y
   ADC   #$00
   LDY   #$84
   STA   (FP),Y
   LDY   #$82
   LDA   (FP),Y
   LDY   #$85
   STA   (FP),Y
   LDY   #$83
   LDA   (FP),Y
   LDY   #$86
   STA   (FP),Y
   LDY   #$84
   LDA   (FP),Y
   AND   #$7F
   LDY   #$87
   STA   (FP),Y
   LDY   #$85
   LDA   (FP),Y
   LDY   #$88
   STA   (FP),Y
   LDY   #$86
   LDA   (FP),Y
   LDY   #$89
   STA   (FP),Y
   LDY   #$87
   LDA   (FP),Y
   EOR   #$40
   LDY   #$8A
   STA   (FP),Y
   LDY   #$88
   LDA   (FP),Y
   LDY   #$8B
   STA   (FP),Y
   LDY   #$89
   LDA   (FP),Y
   LDY   #$8C
   STA   (FP),Y
   LDY   #$8A
   LDA   (FP),Y
   SEC
   SBC   #$40
   LDY   #$8D
   STA   (FP),Y
   LDY   #$8B
   LDA   (FP),Y
   LDY   #$06
   STA   (FP),Y
   LDY   #$8C
   LDA   (FP),Y
   LDY   #$07
   STA   (FP),Y
   LDY   #$8D
   LDA   (FP),Y
   LDY   #$08
   STA   (FP),Y
   JMP   .if_end@12
.if_else@13:
   LDY   #$06
   LDA   (FP),Y
   EOR   #$05
   LDY   #$8E
   STA   (FP),Y
   LDY   #$07
   LDA   (FP),Y
   LDY   #$8F
   STA   (FP),Y
   LDY   #$08
   LDA   (FP),Y
   LDY   #$90
   STA   (FP),Y
   LDY   #$8E
   LDA   (FP),Y
   LDY   #$06
   STA   (FP),Y
   LDY   #$8F
   LDA   (FP),Y
   LDY   #$07
   STA   (FP),Y
   LDY   #$90
   LDA   (FP),Y
   LDY   #$08
   STA   (FP),Y
.if_end@12:
   LDA   g0
   BEQ   .and_false@14
   LDY   #$A6
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@14
   LDA   #$01
   LDY   #$91
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   JMP   .and_end@15
.and_false@14:
   LDA   #$00
   LDY   #$91
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@15:
   LDY   #$18
   LDA   (FP),Y
   BEQ   .and_false@16
   LDA   #$01
   LDY   #$93
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   JMP   .and_end@17
.and_false@16:
   LDA   #$00
   LDY   #$93
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@17:
   LDY   #$93
   LDA   (FP),Y
   ORA   #$01
   LDY   #$95
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$96
   STA   (FP),Y
   LDY   #$91
   LDA   (FP),Y
   STA   HARGS
   INY
   LDA   (FP),Y
   STA   HARGS+1
   LDY   #$95
   LDA   (FP),Y
   STA   HARGS+2
   INY
   LDA   (FP),Y
   STA   HARGS+3
   JSR   sdivmod16
   LDA   HARGS+6
   LDY   #$97
   STA   (FP),Y
   LDA   HARGS+7
   INY
   STA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@112
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@112
   LDA   #$00
   JMP   .cmp_end@113
.cmp_true@112:
   LDA   #$01
.cmp_end@113:
   LDY   #$99
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$9B
   STA   (FP),Y
   CMP   #$01

   ; epilogue
   PHP
   CLC
   LDA   FP
   ADC   #$A9
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

f0:
   SUBROUTINE

   ; prologue: 7 arg bytes, 48 local bytes
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

   LDA   g0
   LDY   #$01
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   g0
   EOR   g0
   INY
   STA   (FP),Y
   LDY   #$33
   LDA   (FP),Y
   EOR   (FP),Y
   LDY   #$04
   STA   (FP),Y
   INY
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@115
   LDA   #$00
   JMP   .sx_done@116
.sx_neg@115:
   LDA   #$FF
.sx_done@116:
   LDY   #$06
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   AND   #$07
   LDY   #$07
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDA   #$00
   LDY   #$08
   STA   (FP),Y
   LDY   #$03
   LDA   (FP),Y
   LDY   #$09
   STA   (FP),Y
   LDY   #$07
   LDA   (FP),Y
   TAX
   BEQ   .shift_done@118
.shift_loop@117:
   LDY   #$09
   LDA   (FP),Y
   ASL
   LDA   (FP),Y
   ROR
   STA   (FP),Y
   DEX
   BNE   .shift_loop@117
.shift_done@118:
   LDY   #$09
   LDA   (FP),Y
   INY
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@119
   LDA   #$00
   JMP   .sx_done@120
.sx_neg@119:
   LDA   #$FF
.sx_done@120:
   LDY   #$0B
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$0A
   LDA   (FP),Y
   LDY   #$0D
   STA   (FP),Y
   LDY   #$0B
   LDA   (FP),Y
   LDY   #$0E
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$33
   LDA   (FP),Y
   LDY   #$10
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@121
   LDA   #$00
   JMP   .sx_done@122
.sx_neg@121:
   LDA   #$FF
.sx_done@122:
   LDY   #$11
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   AND   #$07
   LDY   #$12
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDA   #$00
   LDY   #$13
   STA   (FP),Y
   LDY   #$33
   LDA   (FP),Y
   LDY   #$14
   STA   (FP),Y
   LDY   #$12
   LDA   (FP),Y
   TAX
   CPX   #$00
   BEQ   .shift_done@124
.shift_loop@123:
   LDY   #$14
   LDA   (FP),Y
   ASL
   LDA   (FP),Y
   ROR
   STA   (FP),Y
   DEX
   BNE   .shift_loop@123
.shift_done@124:
   SEC
   LDA   SSP
   SBC   #$01
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDY   #$14
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   JSR   f3
   LDY   #$15
   STA   (FP),Y
   INY
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@125
   LDA   #$00
   JMP   .sx_done@126
.sx_neg@125:
   LDA   #$FF
.sx_done@126:
   LDY   #$17
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$0D
   LDA   (FP),Y
   LDY   #$16
   EOR   (FP),Y
   LDY   #$19
   STA   (FP),Y
   LDY   #$0E
   LDA   (FP),Y
   LDY   #$17
   EOR   (FP),Y
   LDY   #$1A
   STA   (FP),Y
   LDY   #$0F
   LDA   (FP),Y
   LDY   #$18
   EOR   (FP),Y
   LDY   #$1B
   STA   (FP),Y
   LDY   #$19
   LDA   (FP),Y
   LDY   #$0D
   STA   (FP),Y
   LDY   #$1A
   LDA   (FP),Y
   LDY   #$0E
   STA   (FP),Y
   LDY   #$1B
   LDA   (FP),Y
   LDY   #$0F
   STA   (FP),Y
   LDA   g0
   LDY   #$1C
   STA   (FP),Y
   LDA   g0
   BMI   .sx_neg@127
   LDA   #$00
   JMP   .sx_done@128
.sx_neg@127:
   LDA   #$FF
.sx_done@128:
   LDY   #$1D
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$1C
   LDA   (FP),Y
   CLC
   LDY   #$0D
   ADC   (FP),Y
   LDY   #$1F
   STA   (FP),Y
   LDY   #$1D
   LDA   (FP),Y
   LDY   #$0E
   ADC   (FP),Y
   LDY   #$20
   STA   (FP),Y
   LDY   #$1E
   LDA   (FP),Y
   LDY   #$0F
   ADC   (FP),Y
   LDY   #$21
   STA   (FP),Y
   LDY   #$1F
   LDA   (FP),Y
   LDY   #$22
   STA   (FP),Y
   LDY   #$20
   LDA   (FP),Y
   LDY   #$23
   STA   (FP),Y
   LDY   #$21
   LDA   (FP),Y
   AND   #$7F
   LDY   #$24
   STA   (FP),Y
   LDY   #$22
   LDA   (FP),Y
   LDY   #$25
   STA   (FP),Y
   LDY   #$23
   LDA   (FP),Y
   LDY   #$26
   STA   (FP),Y
   LDY   #$24
   LDA   (FP),Y
   EOR   #$40
   LDY   #$27
   STA   (FP),Y
   LDY   #$25
   LDA   (FP),Y
   LDY   #$28
   STA   (FP),Y
   LDY   #$26
   LDA   (FP),Y
   LDY   #$29
   STA   (FP),Y
   LDY   #$27
   LDA   (FP),Y
   SEC
   SBC   #$40
   LDY   #$2A
   STA   (FP),Y
   LDY   #$28
   LDA   (FP),Y
   LDY   #$2B
   STA   (FP),Y
   AND   #$03
   INY
   STA   (FP),Y
   EOR   #$02
   INY
   STA   (FP),Y
   SEC
   SBC   #$02
   INY
   STA   (FP),Y
   STA   g0
   LDY   #$38
   LDA   (FP),Y
   LDY   #$2F
   STA   (FP),Y
   LDY   #$39
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
   ADC   #$39
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

main:
   SUBROUTINE

   ; prologue: 0 arg bytes, 32 local bytes
   SEC
   LDA   SSP
   SBC   #$22
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$21
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   LDA   #$AB
   LDY   #$01
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   ORA   #$00
   BPL   .sx_done@130
   LDA   #$FF
.sx_done@130:
   LDY   #$03
   STA   (FP),Y
   INY
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$07
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   #$FF
   LDY   #$01
   STA   (SSP),Y
   LDA   (FP),Y
   INY
   STA   (SSP),Y
   LDA   (FP),Y
   INY
   STA   (SSP),Y
   LDA   (FP),Y
   INY
   STA   (SSP),Y
   LDA   (FP),Y
   INY
   STA   (SSP),Y
   LDA   #$63
   INY
   STA   (SSP),Y
   LDA   #$00
   INY
   STA   (SSP),Y
   JSR   f0
   LDY   #$05
   STA   (FP),Y
   TXA
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
   LDA   g0
   INY
   STA   (FP),Y
   LDA   g0
   BMI   .sx_neg@131
   LDA   #$00
   JMP   .sx_done@132
.sx_neg@131:
   LDA   #$FF
.sx_done@132:
   LDY   #$10
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$0B
   LDA   (FP),Y
   LDY   #$0F
   EOR   (FP),Y
   LDY   #$13
   STA   (FP),Y
   LDY   #$0C
   LDA   (FP),Y
   LDY   #$10
   EOR   (FP),Y
   LDY   #$14
   STA   (FP),Y
   LDY   #$0D
   LDA   (FP),Y
   LDY   #$11
   EOR   (FP),Y
   LDY   #$15
   STA   (FP),Y
   LDY   #$0E
   LDA   (FP),Y
   LDY   #$12
   EOR   (FP),Y
   LDY   #$16
   STA   (FP),Y
   LDY   #$13
   LDA   (FP),Y
   LDY   #$0B
   STA   (FP),Y
   LDY   #$14
   LDA   (FP),Y
   LDY   #$0C
   STA   (FP),Y
   LDY   #$15
   LDA   (FP),Y
   LDY   #$0D
   STA   (FP),Y
   LDY   #$16
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
   LDY   #$17
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
   LDY   #$17
   EOR   (FP),Y
   LDY   #$1B
   STA   (FP),Y
   LDY   #$0C
   LDA   (FP),Y
   LDY   #$18
   EOR   (FP),Y
   LDY   #$1C
   STA   (FP),Y
   LDY   #$0D
   LDA   (FP),Y
   LDY   #$19
   EOR   (FP),Y
   LDY   #$1D
   STA   (FP),Y
   LDY   #$0E
   LDA   (FP),Y
   LDY   #$1A
   EOR   (FP),Y
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
   TAX
   DEY
   LDA   (FP),Y

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$22
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$21
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
   DC.B  $01
