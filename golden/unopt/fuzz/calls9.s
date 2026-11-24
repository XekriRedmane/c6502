SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24

rec:
   SUBROUTINE

   ; prologue: 4 arg bytes, 16 local bytes
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

   LDA   #$00
   SEC
   INY
   SBC   (FP),Y
   LDA   #$00
   INY
   SBC   (FP),Y
   BVC   .cmp_novf@0
   EOR   #$80
.cmp_novf@0:
   ASL
   LDA   #$00
   ROL
   EOR   #$01
   LDY   #$01
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@0
   LDY   #$15
   LDA   (FP),Y
   LDY   #$03
   STA   (FP),Y
   LDY   #$16
   LDA   (FP),Y
   LDA   #$00
   LDY   #$04
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
   ADC   #$16
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
   PLA
   TAX
   PLA
   RTS
.if_end@0:
   LDY   #$13
   LDA   (FP),Y
   SEC
   SBC   #$01
   LDY   #$05
   STA   (FP),Y
   LDY   #$14
   LDA   (FP),Y
   SBC   #$00
   LDY   #$06
   STA   (FP),Y
   LDY   #$15
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
   LDY   #$07
   STA   (FP),Y
   LDA   HARGS+5
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   LDY   #$13
   ADC   (FP),Y
   LDY   #$09
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$14
   ADC   (FP),Y
   LDY   #$0A
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$0B
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   AND   #$03
   LDY   #$0C
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
   LDY   #$0B
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$0C
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   JSR   rec
   LDY   #$0D
   STA   (FP),Y
   TXA
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
   TAX
   DEY
   LDA   (FP),Y

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$16
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
   PLA
   TAX
   PLA
   RTS

f5:
   SUBROUTINE

   ; prologue: 0 arg bytes, 10 local bytes
   SEC
   LDA   SSP
   SBC   #$0C
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$0B
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   LDA   #$02
   BEQ   .and_false@1
   LDA   g0
   ORA   g0+1
   BEQ   .and_false@1
   LDA   #$01
   LDY   #$01
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   JMP   .and_end@2
.and_false@1:
   LDA   #$00
   LDY   #$01
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@2:
   LDY   #$01
   LDA   (FP),Y
   SEC
   SBC   g0
   INY
   LDA   (FP),Y
   SBC   g0+1
   BVC   .cmp_novf@1
   EOR   #$80
.cmp_novf@1:
   ASL
   LDA   #$00
   ROL
   EOR   #$01
   LDY   #$03
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$05
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
   ADC   #$0C
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$0B
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

   ; prologue: 0 arg bytes, 35 local bytes
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

   SEC
   LDA   #$00
   SBC   g2
   LDY   #$01
   STA   (FP),Y
   LDA   #$00
   SBC   g2+1
   INY
   STA   (FP),Y
   LDA   #$00
   SBC   g2+2
   INY
   STA   (FP),Y
   LDA   #$00
   SBC   g2+3
   INY
   STA   (FP),Y
   LDA   #$01
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   ORA   #$00
   BPL   .sx_done@3
   LDA   #$FF
.sx_done@3:
   LDY   #$07
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$01
   LDA   (FP),Y
   LDY   #$05
   ORA   (FP),Y
   LDY   #$09
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   LDY   #$06
   ORA   (FP),Y
   LDY   #$0A
   STA   (FP),Y
   LDY   #$03
   LDA   (FP),Y
   LDY   #$07
   ORA   (FP),Y
   LDY   #$0B
   STA   (FP),Y
   LDY   #$04
   LDA   (FP),Y
   LDY   #$08
   ORA   (FP),Y
   LDY   #$0C
   STA   (FP),Y
   LDA   g2
   STA   HARGS
   LDA   g2+1
   STA   HARGS+1
   LDA   g2+2
   STA   HARGS+2
   LDA   g2+3
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
   JSR   udivmod32
   LDA   HARGS+8
   LDY   #$0D
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
   LDY   #$0D
   LDA   (FP),Y
   LDY   #$11
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$01
   SEC
   SBC   g0
   LDA   #$00
   SBC   g0+1
   BVC   .cmp_novf@4
   EOR   #$80
.cmp_novf@4:
   ASL
   LDA   #$00
   ROL
   LDY   #$13
   STA   (FP),Y
   LDA   #$00
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
   ORA   #$00
   BMI   .sx_neg@5
   LDA   #$00
   JMP   .sx_done@6
.sx_neg@5:
   LDA   #$FF
.sx_done@6:
   LDY   #$17
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   g2+3
   CMP   (FP),Y
   BNE   .thread@0
   LDA   g2+2
   DEY
   CMP   (FP),Y
   BNE   .thread@0
   LDA   g2+1
   DEY
   CMP   (FP),Y
   BNE   .thread@0
   LDA   g2
   DEY
   CMP   (FP),Y
   BEQ   .cmp_true@7
.thread@0:
   LDA   #$00
   JMP   .cmp_end@8
.cmp_true@7:
   LDA   #$01
.cmp_end@8:
   LDY   #$19
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
   ORA   #$00
   BMI   .sx_neg@10
   LDA   #$00
   JMP   .sx_done@11
.sx_neg@10:
   LDA   #$FF
.sx_done@11:
   LDY   #$1D
   STA   (FP),Y
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
   INY
   STA   (FP),Y
   LDY   #$12
   LDA   (FP),Y
   LDY   #$23
   STA   (FP),Y
   CLC

   ; epilogue
   PHP
   CLC
   LDA   FP
   ADC   #$25
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

   ; prologue: 2 arg bytes, 101 local bytes
   SEC
   LDA   SSP
   SBC   #$67
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$66
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   LDA   g1
   BNE   .or_true@3
   LDA   g0
   LDY   #$01
   STA   (FP),Y
   LDA   g0+1
   INY
   STA   (FP),Y
   LDA   g0+1
   BMI   .sx_neg@12
   LDA   #$00
   JMP   .sx_done@13
.sx_neg@12:
   LDA   #$FF
.sx_done@13:
   LDY   #$03
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$01
   LDA   (FP),Y
   SEC
   SBC   g2
   INY
   LDA   (FP),Y
   SBC   g2+1
   INY
   LDA   (FP),Y
   SBC   g2+2
   INY
   LDA   (FP),Y
   SBC   g2+3
   LDA   #$00
   ROL
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@3
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   JMP   .or_end@4
.or_true@3:
   LDA   #$01
   LDY   #$07
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@4:
   LDY   #$07
   LDA   (FP),Y
   LDY   #$09
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$0A
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@14
   LDA   #$00
   JMP   .sx_done@15
.sx_neg@14:
   LDA   #$FF
.sx_done@15:
   LDY   #$0B
   STA   (FP),Y
   INY
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
   INY
   STA   (FP),Y
   LDA   g2
   INY
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
   LDY   #$11
   LDA   (FP),Y
   SEC
   LDY   #$0D
   SBC   (FP),Y
   LDY   #$12
   LDA   (FP),Y
   LDY   #$0E
   SBC   (FP),Y
   LDY   #$13
   LDA   (FP),Y
   LDY   #$0F
   SBC   (FP),Y
   LDY   #$14
   LDA   (FP),Y
   LDY   #$10
   SBC   (FP),Y
   BVC   .cmp_novf@16
   EOR   #$80
.cmp_novf@16:
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
   LDY   #$17
   STA   (FP),Y
   DEY
   LDA   (FP),Y
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
   LDA   g1
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
   DEY
   LDA   (FP),Y
   ORA   #$01
   LDY   #$1F
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$20
   STA   (FP),Y
   LDY   #$1B
   LDA   (FP),Y
   STA   HARGS
   INY
   LDA   (FP),Y
   STA   HARGS+1
   LDY   #$1F
   LDA   (FP),Y
   STA   HARGS+2
   INY
   LDA   (FP),Y
   STA   HARGS+3
   JSR   sdivmod16
   LDA   HARGS+4
   LDY   #$21
   STA   (FP),Y
   LDA   HARGS+5
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
   AND   #$07
   LDY   #$25
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDA   #$00
   LDY   #$26
   STA   (FP),Y
   LDY   #$21
   LDA   (FP),Y
   LDY   #$27
   STA   (FP),Y
   LDY   #$22
   LDA   (FP),Y
   LDY   #$28
   STA   (FP),Y
   LDY   #$25
   LDA   (FP),Y
   TAX
   BEQ   .shift_done@18
.shift_loop@17:
   LDY   #$28
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
   BNE   .shift_loop@17
.shift_done@18:
   LDY   #$27
   LDA   (FP),Y
   LDY   #$29
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   g2
   INY
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
   LDY   #$0D
   LDA   (FP),Y
   LDY   #$33
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$68
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$35
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$36
   STA   (FP),Y
   LDY   #$19
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lnot_true@19
   LDA   #$00
   JMP   .lnot_end@20
.lnot_true@19:
   LDA   #$01
.lnot_end@20:
   LDY   #$37
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$35
   LDA   (FP),Y
   CLC
   LDY   #$37
   ADC   (FP),Y
   LDY   #$39
   STA   (FP),Y
   LDY   #$36
   LDA   (FP),Y
   LDY   #$38
   ADC   (FP),Y
   LDY   #$3A
   STA   (FP),Y
   LDY   #$68
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$39
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$3A
   LDA   (FP),Y
   LDY   #$01
   STA   (DPTR),Y
   LDA   #$00
   LDY   #$3B
   STA   (FP),Y
   INY
   STA   (FP),Y
.loop@0_start:
   LDY   #$3B
   LDA   (FP),Y
   SEC
   SBC   #$03
   INY
   LDA   (FP),Y
   SBC   #$00
   BVC   .cmp_novf@21
   EOR   #$80
.cmp_novf@21:
   ASL
   LDA   #$00
   ROL
   LDY   #$3D
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@0
   JMP   .loop@0_break
.lb_skip@0:
   JSR   f5
   LDY   #$3F
   STA   (FP),Y
   TXA
   INY
   STA   (FP),Y
   LDY   #$3B
   LDA   (FP),Y
   LDY   #$41
   STA   (FP),Y
   LDY   #$3C
   LDA   (FP),Y
   LDY   #$42
   STA   (FP),Y
   LDY   #$3F
   LDA   (FP),Y
   CLC
   LDY   #$41
   ADC   (FP),Y
   LDY   #$43
   STA   (FP),Y
   LDY   #$40
   LDA   (FP),Y
   LDY   #$42
   ADC   (FP),Y
   LDY   #$44
   STA   (FP),Y
   DEY
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
   LDY   #$0D
   LDA   (FP),Y
   CLC
   LDY   #$45
   ADC   (FP),Y
   LDY   #$49
   STA   (FP),Y
   LDY   #$0E
   LDA   (FP),Y
   LDY   #$46
   ADC   (FP),Y
   LDY   #$4A
   STA   (FP),Y
   LDY   #$0F
   LDA   (FP),Y
   LDY   #$47
   ADC   (FP),Y
   LDY   #$4B
   STA   (FP),Y
   LDY   #$10
   LDA   (FP),Y
   LDY   #$48
   ADC   (FP),Y
   LDY   #$4C
   STA   (FP),Y
   LDY   #$49
   LDA   (FP),Y
   LDY   #$0D
   STA   (FP),Y
   LDY   #$4A
   LDA   (FP),Y
   LDY   #$0E
   STA   (FP),Y
   LDY   #$4B
   LDA   (FP),Y
   LDY   #$0F
   STA   (FP),Y
   LDY   #$4C
   LDA   (FP),Y
   LDY   #$10
   STA   (FP),Y
   LDY   #$3B
   LDA   (FP),Y
   LDY   #$4D
   STA   (FP),Y
   LDY   #$3C
   LDA   (FP),Y
   LDY   #$4E
   STA   (FP),Y
   LDY   #$3B
   LDA   (FP),Y
   CLC
   ADC   #$01
   LDY   #$4F
   STA   (FP),Y
   LDY   #$3C
   LDA   (FP),Y
   ADC   #$00
   LDY   #$50
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$3B
   STA   (FP),Y
   LDY   #$50
   LDA   (FP),Y
   LDY   #$3C
   STA   (FP),Y
   JMP   .loop@0_start
.loop@0_break:
   LDY   #$0D
   LDA   (FP),Y
   LDY   #$51
   STA   (FP),Y
   LDY   #$0E
   LDA   (FP),Y
   LDY   #$52
   STA   (FP),Y
   LDY   #$0F
   LDA   (FP),Y
   LDY   #$53
   STA   (FP),Y
   LDY   #$10
   LDA   (FP),Y
   LDY   #$54
   STA   (FP),Y
   LDA   g2
   CLC
   LDY   #$51
   ADC   (FP),Y
   LDY   #$55
   STA   (FP),Y
   LDA   g2+1
   LDY   #$52
   ADC   (FP),Y
   LDY   #$56
   STA   (FP),Y
   LDA   g2+2
   LDY   #$53
   ADC   (FP),Y
   LDY   #$57
   STA   (FP),Y
   LDA   g2+3
   LDY   #$54
   ADC   (FP),Y
   LDY   #$58
   STA   (FP),Y
   LDY   #$55
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
   LDY   #$34
   LDA   (FP),Y
   LDY   #$59
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$5B
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$5C
   STA   (FP),Y
   LDY   #$19
   LDA   (FP),Y
   CLC
   LDY   #$5B
   SBC   (FP),Y
   LDY   #$1A
   LDA   (FP),Y
   LDY   #$5C
   SBC   (FP),Y
   LDA   #$00
   ROL
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$68
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
   DEY
   LDA   (FP),Y
   ORA   #$01
   LDY   #$61
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$62
   STA   (FP),Y
   LDY   #$5D
   LDA   (FP),Y
   STA   HARGS
   INY
   LDA   (FP),Y
   STA   HARGS+1
   LDY   #$61
   LDA   (FP),Y
   STA   HARGS+2
   INY
   LDA   (FP),Y
   STA   HARGS+3
   JSR   sdivmod16
   LDA   HARGS+6
   LDY   #$63
   STA   (FP),Y
   LDA   HARGS+7
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$65
   STA   (FP),Y

   ; epilogue
   PHA
   CLC
   LDA   FP
   ADC   #$69
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

   ; prologue: 9 arg bytes, 30 local bytes
   SEC
   LDA   SSP
   SBC   #$20
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$1F
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   LDA   g0
   ORA   g0+1
   BNE   .or_true@5
   LDA   g1
   BNE   .or_true@5
   LDA   #$00
   LDY   #$01
   STA   (FP),Y
   INY
   STA   (FP),Y
   JMP   .or_end@6
.or_true@5:
   LDA   #$01
   LDY   #$01
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@6:
   LDY   #$01
   LDA   (FP),Y
   EOR   #$FF
   LDY   #$03
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   EOR   #$FF
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
   BMI   .sx_neg@22
   LDA   #$00
   JMP   .sx_done@23
.sx_neg@22:
   LDA   #$FF
.sx_done@23:
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
   LDY   #$27
   LDA   (FP),Y
   LDY   #$0D
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$28
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$0F
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$10
   STA   (FP),Y
   LDY   #$09
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   INY
   ORA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@7
   LDY   #$25
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@7
   LDA   #$00
   LDY   #$11
   STA   (FP),Y
   INY
   STA   (FP),Y
   JMP   .or_end@8
.or_true@7:
   LDA   #$01
   LDY   #$11
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@8:
   LDY   #$0F
   LDA   (FP),Y
   CLC
   LDY   #$11
   ADC   (FP),Y
   LDY   #$13
   STA   (FP),Y
   LDY   #$10
   LDA   (FP),Y
   LDY   #$12
   ADC   (FP),Y
   LDY   #$14
   STA   (FP),Y
   LDY   #$28
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$13
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$14
   LDA   (FP),Y
   LDY   #$01
   STA   (DPTR),Y
   LDY   #$27
   LDA   (FP),Y
   LDY   #$15
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$04
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   #$03
   LDY   #$01
   STA   (SSP),Y
   LDA   #$00
   INY
   STA   (SSP),Y
   LDY   #$15
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$16
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   JSR   rec
   LDY   #$17
   STA   (FP),Y
   TXA
   INY
   STA   (FP),Y
   LDY   #$0E
   LDA   (FP),Y
   LDY   #$19
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@24
   LDA   #$00
   JMP   .sx_done@25
.sx_neg@24:
   LDA   #$FF
.sx_done@25:
   LDY   #$1A
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   LDY   #$17
   ADC   (FP),Y
   LDY   #$1B
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$18
   ADC   (FP),Y
   LDY   #$1C
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$1D
   STA   (FP),Y
   LDY   #$0E
   STA   (FP),Y
   LDY   #$1E
   STA   (FP),Y

   ; epilogue
   PHA
   CLC
   LDA   FP
   ADC   #$29
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
   STA   HARGS
   LDA   #$00
   LDX   #$08
   LSR   HARGS
.mul_loop@26:
   BCC   .mul_skip@27
   CLC
.mul_skip@27:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@26
   LDY   #$06
   STA   (FP),Y
   LDA   HARGS
   DEY
   STA   (FP),Y
   LDY   #$01
   LDA   (FP),Y
   SEC
   LDY   #$05
   SBC   (FP),Y
   LDY   #$07
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   LDY   #$06
   SBC   (FP),Y
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
   DEY
   LDA   (FP),Y
   LDY   #$0B
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$0C
   STA   (FP),Y
   JSR   f5
   LDY   #$0D
   STA   (FP),Y
   TXA
   INY
   STA   (FP),Y
   LDY   #$0B
   LDA   (FP),Y
   LDY   #$0D
   EOR   (FP),Y
   LDY   #$0F
   STA   (FP),Y
   LDY   #$0C
   LDA   (FP),Y
   LDY   #$0E
   EOR   (FP),Y
   LDY   #$10
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$0B
   STA   (FP),Y
   LDY   #$10
   LDA   (FP),Y
   LDY   #$0C
   STA   (FP),Y
   LDA   g0
   LDY   #$11
   STA   (FP),Y
   LDA   g0+1
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   LDY   #$0B
   ADC   (FP),Y
   LDY   #$13
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$0C
   ADC   (FP),Y
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
   STA   g0
   INY
   LDA   (FP),Y
   STA   g0+1
   LDY   #$0B
   LDA   (FP),Y
   LDY   #$17
   STA   (FP),Y
   LDY   #$0C
   LDA   (FP),Y
   LDY   #$18
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   g2
   CLC
   LDY   #$17
   ADC   (FP),Y
   LDY   #$1B
   STA   (FP),Y
   LDA   g2+1
   LDY   #$18
   ADC   (FP),Y
   LDY   #$1C
   STA   (FP),Y
   LDA   g2+2
   LDY   #$19
   ADC   (FP),Y
   LDY   #$1D
   STA   (FP),Y
   LDA   g2+3
   LDY   #$1A
   ADC   (FP),Y
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
   SEC
   LDA   SSP
   SBC   #$04
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   #$03
   LDY   #$01
   STA   (SSP),Y
   LDA   #$00
   INY
   STA   (SSP),Y
   LDY   #$1F
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$20
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   JSR   rec
   LDY   #$21
   STA   (FP),Y
   TXA
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
   LDY   #$0B
   LDA   (FP),Y
   CLC
   LDY   #$23
   ADC   (FP),Y
   LDY   #$25
   STA   (FP),Y
   LDY   #$0C
   LDA   (FP),Y
   LDY   #$24
   ADC   (FP),Y
   LDY   #$26
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$0B
   STA   (FP),Y
   LDY   #$26
   LDA   (FP),Y
   LDY   #$0C
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$27
   STA   (FP),Y
   LDY   #$0C
   LDA   (FP),Y
   LDY   #$28
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$27
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
   ADC   #$2C
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

f0:
   SUBROUTINE

   ; prologue: 0 arg bytes, 23 local bytes
   SEC
   LDA   SSP
   SBC   #$19
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$18
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   SEC
   LDA   #$00
   SBC   #$01
   LDY   #$01
   STA   (FP),Y
   LDA   #$00
   SBC   #$00
   INY
   STA   (FP),Y
   LDA   g0
   SEC
   DEY
   SBC   (FP),Y
   LDY   #$03
   STA   (FP),Y
   LDA   g0+1
   DEY
   SBC   (FP),Y
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
   DEY
   LDA   (FP),Y
   LDY   #$07
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$08
   STA   (FP),Y
   LDA   g0
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
   LDY   #$0A
   LDA   (FP),Y
   LDY   #$0D
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@28
   LDA   #$00
   JMP   .sx_done@29
.sx_neg@28:
   LDA   #$FF
.sx_done@29:
   LDY   #$0E
   STA   (FP),Y
   LDY   #$0B
   LDA   (FP),Y
   LDY   #$0D
   ORA   (FP),Y
   LDY   #$0F
   STA   (FP),Y
   LDY   #$0C
   LDA   (FP),Y
   LDY   #$0E
   ORA   (FP),Y
   LDY   #$10
   STA   (FP),Y
   LDY   #$0A
   LDA   (FP),Y
   LDY   #$11
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@30
   LDA   #$00
   JMP   .sx_done@31
.sx_neg@30:
   LDA   #$FF
.sx_done@31:
   LDY   #$12
   STA   (FP),Y
   LDY   #$0F
   LDA   (FP),Y
   CLC
   LDY   #$11
   ADC   (FP),Y
   LDY   #$13
   STA   (FP),Y
   LDY   #$10
   LDA   (FP),Y
   LDY   #$12
   ADC   (FP),Y
   LDY   #$14
   STA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@32
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@32
   LDA   #$00
   JMP   .cmp_end@33
.cmp_true@32:
   LDA   #$01
.cmp_end@33:
   LDY   #$15
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$17
   STA   (FP),Y
   CMP   #$01

   ; epilogue
   PHP
   CLC
   LDA   FP
   ADC   #$19
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

main:
   SUBROUTINE

   ; prologue: 0 arg bytes, 43 local bytes
   SEC
   LDA   SSP
   SBC   #$2D
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$2C
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   JSR   f0
   LDA   #$00
   ROL
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
   LDY   #$02
   LDA   (FP),Y
   LDY   #$06
   STA   (FP),Y
   LDY   #$03
   LDA   (FP),Y
   LDY   #$07
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   g0
   INY
   STA   (FP),Y
   LDA   g0+1
   INY
   STA   (FP),Y
   LDA   g0+1
   BMI   .sx_neg@35
   LDA   #$00
   JMP   .sx_done@36
.sx_neg@35:
   LDA   #$FF
.sx_done@36:
   LDY   #$0C
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
   LDY   #$0A
   LDA   (FP),Y
   LDY   #$0E
   EOR   (FP),Y
   LDY   #$12
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
   LDA   g2
   INY
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
   LDY   #$12
   LDA   (FP),Y
   LDY   #$16
   EOR   (FP),Y
   LDY   #$1A
   STA   (FP),Y
   LDY   #$13
   LDA   (FP),Y
   LDY   #$17
   EOR   (FP),Y
   LDY   #$1B
   STA   (FP),Y
   LDY   #$14
   LDA   (FP),Y
   LDY   #$18
   EOR   (FP),Y
   LDY   #$1C
   STA   (FP),Y
   LDY   #$15
   LDA   (FP),Y
   LDY   #$19
   EOR   (FP),Y
   LDY   #$1D
   STA   (FP),Y
   LDY   #$06
   LDA   (FP),Y
   LDY   #$1A
   EOR   (FP),Y
   LDY   #$1E
   STA   (FP),Y
   LDY   #$07
   LDA   (FP),Y
   LDY   #$1B
   EOR   (FP),Y
   LDY   #$1F
   STA   (FP),Y
   LDY   #$08
   LDA   (FP),Y
   LDY   #$1C
   EOR   (FP),Y
   LDY   #$20
   STA   (FP),Y
   LDY   #$09
   LDA   (FP),Y
   LDY   #$1D
   EOR   (FP),Y
   LDY   #$21
   STA   (FP),Y
   LDY   #$1E
   LDA   (FP),Y
   LDY   #$06
   STA   (FP),Y
   LDY   #$1F
   LDA   (FP),Y
   LDY   #$07
   STA   (FP),Y
   LDY   #$20
   LDA   (FP),Y
   LDY   #$08
   STA   (FP),Y
   LDY   #$21
   LDA   (FP),Y
   LDY   #$09
   STA   (FP),Y
   LDY   #$06
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
   LDY   #$22
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
   LDY   #$06
   LDA   (FP),Y
   LDY   #$22
   EOR   (FP),Y
   LDY   #$26
   STA   (FP),Y
   LDY   #$07
   LDA   (FP),Y
   LDY   #$23
   EOR   (FP),Y
   LDY   #$27
   STA   (FP),Y
   LDY   #$08
   LDA   (FP),Y
   LDY   #$24
   EOR   (FP),Y
   LDY   #$28
   STA   (FP),Y
   LDY   #$09
   LDA   (FP),Y
   LDY   #$25
   EOR   (FP),Y
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
   TAX
   DEY
   LDA   (FP),Y

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$2D
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$2C
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
   DC.W  $0044

g1:
   DC.B  $2F

g2:
   DC.L  $00000001
