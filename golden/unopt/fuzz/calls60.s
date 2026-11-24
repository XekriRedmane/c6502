SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24

f4:
   SUBROUTINE

   ; prologue: 4 arg bytes, 46 local bytes
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

   INY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   INY
   ORA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@0
   LDY   #$31
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   INY
   ORA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@0
   LDA   #$01
   LDY   #$01
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   JMP   .and_end@1
.and_false@0:
   LDA   #$00
   LDY   #$01
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@1:
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
   INY
   STA   (FP),Y
   LDY   #$31
   LDA   (FP),Y
   LDY   #$07
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
   LDY   #$0B
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$0C
   STA   (FP),Y
   LDA   g1
   INY
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
   LDY   #$0F
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   g0+3
   CMP   (FP),Y
   BNE   .thread@0
   LDA   g0+2
   DEY
   CMP   (FP),Y
   BNE   .thread@0
   LDA   g0+1
   DEY
   CMP   (FP),Y
   BNE   .thread@0
   LDA   g0
   DEY
   CMP   (FP),Y
   BEQ   .cmp_true@2
.thread@0:
   LDA   #$00
   JMP   .cmp_end@3
.cmp_true@2:
   LDA   #$01
.cmp_end@3:
   LDY   #$11
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$06
   LDA   (FP),Y
   BEQ   .and_false@2
   LDA   g1
   ORA   g1+1
   BEQ   .and_false@2
   LDA   #$01
   LDY   #$13
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   JMP   .and_end@3
.and_false@2:
   LDA   #$00
   LDY   #$13
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@3:
   LDY   #$13
   LDA   (FP),Y
   ORA   #$01
   LDY   #$15
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$16
   STA   (FP),Y
   LDY   #$11
   LDA   (FP),Y
   STA   HARGS
   INY
   LDA   (FP),Y
   STA   HARGS+1
   LDY   #$15
   LDA   (FP),Y
   STA   HARGS+2
   INY
   LDA   (FP),Y
   STA   HARGS+3
   JSR   sdivmod16
   LDA   HARGS+6
   LDY   #$17
   STA   (FP),Y
   LDA   HARGS+7
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$19
   STA   (FP),Y
   DEY
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
   LDY   #$06
   LDA   (FP),Y
   LDY   #$1D
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$0B
   LDA   (FP),Y
   LDY   #$1F
   STA   (FP),Y
   LDY   #$0C
   LDA   (FP),Y
   LDY   #$20
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
   ORA   #$00
   BPL   .sx_done@6
   LDA   #$FF
.sx_done@6:
   LDY   #$25
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$1F
   LDA   (FP),Y
   LDY   #$23
   ORA   (FP),Y
   LDY   #$27
   STA   (FP),Y
   LDY   #$20
   LDA   (FP),Y
   LDY   #$24
   ORA   (FP),Y
   LDY   #$28
   STA   (FP),Y
   LDY   #$21
   LDA   (FP),Y
   LDY   #$25
   ORA   (FP),Y
   LDY   #$29
   STA   (FP),Y
   LDY   #$22
   LDA   (FP),Y
   LDY   #$26
   ORA   (FP),Y
   LDY   #$2A
   STA   (FP),Y
   LDY   #$27
   LDA   (FP),Y
   LDY   #$2B
   STA   (FP),Y
   LDY   #$28
   LDA   (FP),Y
   LDY   #$2C
   STA   (FP),Y
   LDY   #$29
   LDA   (FP),Y
   LDY   #$2D
   STA   (FP),Y
   LDY   #$2A
   LDA   (FP),Y
   LDY   #$2E
   STA   (FP),Y
   LDY   #$2B
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
   ADC   #$34
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

f3:
   SUBROUTINE

   ; prologue: 1 arg bytes, 27 local bytes
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

   INY
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDA   g1
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$01
   LDA   (FP),Y
   CLC
   LDY   #$04
   ADC   (FP),Y
   LDY   #$06
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   LDY   #$05
   ADC   (FP),Y
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
   LDA   g0
   ORA   g0+1
   ORA   g0+2
   ORA   g0+3
   BEQ   .and_false@6
   LDA   g0
   ORA   g0+1
   ORA   g0+2
   ORA   g0+3
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
   LDY   #$0A
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@7:
   LDY   #$0A
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@4
   LDA   #$EA
   INY
   STA   (FP),Y
   LDA   #$03
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@4
   LDA   #$01
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   JMP   .and_end@5
.and_false@4:
   LDA   #$00
   LDY   #$0E
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@5:
   LDY   #$0E
   LDA   (FP),Y
   LDY   #$10
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$11
   STA   (FP),Y
   LDY   #$1E
   LDA   (FP),Y
   LDY   #$12
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$1E
   LDA   (FP),Y
   LDY   #$14
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$12
   LDA   (FP),Y
   LDY   #$14
   EOR   (FP),Y
   LDY   #$16
   STA   (FP),Y
   LDY   #$13
   LDA   (FP),Y
   LDY   #$15
   EOR   (FP),Y
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
   ADC   #$1E
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

   ; prologue: 0 arg bytes, 71 local bytes
   SEC
   LDA   SSP
   SBC   #$49
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$48
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
   DEY
   LDA   (FP),Y
   LDY   #$03
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$04
   STA   (FP),Y
   LDA   g1
   INY
   STA   (FP),Y
   LDA   g1+1
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
   SEC
   LDY   #$03
   SBC   (FP),Y
   LDY   #$09
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$04
   SBC   (FP),Y
   LDY   #$0A
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   SEC
   LDY   #$05
   SBC   (FP),Y
   LDY   #$0A
   LDA   (FP),Y
   LDY   #$06
   SBC   (FP),Y
   LDA   #$00
   ROL
   LDY   #$0B
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
   ORA   #$00
   BMI   .sx_neg@9
   LDA   #$00
   JMP   .sx_done@10
.sx_neg@9:
   LDA   #$FF
.sx_done@10:
   LDY   #$0F
   STA   (FP),Y
   INY
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
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
.loop@0_start:
   LDY   #$15
   LDA   (FP),Y
   SEC
   SBC   #$03
   INY
   LDA   (FP),Y
   SBC   #$00
   BVC   .cmp_novf@11
   EOR   #$80
.cmp_novf@11:
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
   BNE   .lb_skip@0
   JMP   .loop@0_break
.lb_skip@0:
   LDA   g1
   LDY   #$19
   STA   (FP),Y
   LDA   g1+1
   INY
   STA   (FP),Y
   LDA   g1+1
   BMI   .sx_neg@12
   LDA   #$00
   JMP   .sx_done@13
.sx_neg@12:
   LDA   #$FF
.sx_done@13:
   LDY   #$1B
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   g0
   LDY   #$19
   AND   (FP),Y
   LDY   #$1D
   STA   (FP),Y
   LDA   g0+1
   LDY   #$1A
   AND   (FP),Y
   LDY   #$1E
   STA   (FP),Y
   LDA   g0+2
   LDY   #$1B
   AND   (FP),Y
   LDY   #$1F
   STA   (FP),Y
   LDA   g0+3
   LDY   #$1C
   AND   (FP),Y
   LDY   #$20
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$04
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDY   #$1D
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$1E
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$1F
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$20
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   JSR   f4
   LDA   HARGS+8
   LDY   #$21
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
   LDY   #$15
   LDA   (FP),Y
   LDY   #$25
   STA   (FP),Y
   LDY   #$16
   LDA   (FP),Y
   LDY   #$26
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@14
   LDA   #$00
   JMP   .sx_done@15
.sx_neg@14:
   LDA   #$FF
.sx_done@15:
   LDY   #$27
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$21
   LDA   (FP),Y
   CLC
   LDY   #$25
   ADC   (FP),Y
   LDY   #$29
   STA   (FP),Y
   LDY   #$22
   LDA   (FP),Y
   LDY   #$26
   ADC   (FP),Y
   LDY   #$2A
   STA   (FP),Y
   LDY   #$23
   LDA   (FP),Y
   LDY   #$27
   ADC   (FP),Y
   LDY   #$2B
   STA   (FP),Y
   LDY   #$24
   LDA   (FP),Y
   LDY   #$28
   ADC   (FP),Y
   LDY   #$2C
   STA   (FP),Y
   LDY   #$11
   LDA   (FP),Y
   LDY   #$2D
   STA   (FP),Y
   LDY   #$12
   LDA   (FP),Y
   LDY   #$2E
   STA   (FP),Y
   LDY   #$13
   LDA   (FP),Y
   LDY   #$2F
   STA   (FP),Y
   LDY   #$14
   LDA   (FP),Y
   LDY   #$30
   STA   (FP),Y
   LDY   #$2D
   LDA   (FP),Y
   CLC
   LDY   #$29
   ADC   (FP),Y
   LDY   #$31
   STA   (FP),Y
   LDY   #$2E
   LDA   (FP),Y
   LDY   #$2A
   ADC   (FP),Y
   LDY   #$32
   STA   (FP),Y
   LDY   #$2F
   LDA   (FP),Y
   LDY   #$2B
   ADC   (FP),Y
   LDY   #$33
   STA   (FP),Y
   LDY   #$30
   LDA   (FP),Y
   LDY   #$2C
   ADC   (FP),Y
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
   LDY   #$35
   LDA   (FP),Y
   LDY   #$11
   STA   (FP),Y
   LDY   #$36
   LDA   (FP),Y
   LDY   #$12
   STA   (FP),Y
   LDY   #$37
   LDA   (FP),Y
   LDY   #$13
   STA   (FP),Y
   LDY   #$38
   LDA   (FP),Y
   LDY   #$14
   STA   (FP),Y
   INY
   LDA   (FP),Y
   LDY   #$39
   STA   (FP),Y
   LDY   #$16
   LDA   (FP),Y
   LDY   #$3A
   STA   (FP),Y
   LDY   #$15
   LDA   (FP),Y
   CLC
   ADC   #$01
   LDY   #$3B
   STA   (FP),Y
   LDY   #$16
   LDA   (FP),Y
   ADC   #$00
   LDY   #$3C
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$15
   STA   (FP),Y
   LDY   #$3C
   LDA   (FP),Y
   LDY   #$16
   STA   (FP),Y
   JMP   .loop@0_start
.loop@0_break:
   LDA   g1
   LDY   #$3D
   STA   (FP),Y
   LDA   g1+1
   INY
   STA   (FP),Y
   LDA   g1+1
   BMI   .sx_neg@16
   LDA   #$00
   JMP   .sx_done@17
.sx_neg@16:
   LDA   #$FF
.sx_done@17:
   LDY   #$3F
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$3D
   LDA   (FP),Y
   CLC
   LDY   #$11
   ADC   (FP),Y
   LDY   #$41
   STA   (FP),Y
   LDY   #$3E
   LDA   (FP),Y
   LDY   #$12
   ADC   (FP),Y
   LDY   #$42
   STA   (FP),Y
   LDY   #$3F
   LDA   (FP),Y
   LDY   #$13
   ADC   (FP),Y
   LDY   #$43
   STA   (FP),Y
   LDY   #$40
   LDA   (FP),Y
   LDY   #$14
   ADC   (FP),Y
   LDY   #$44
   STA   (FP),Y
   LDY   #$41
   LDA   (FP),Y
   LDY   #$45
   STA   (FP),Y
   LDY   #$42
   LDA   (FP),Y
   LDY   #$46
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   g1
   INY
   LDA   (FP),Y
   STA   g1+1
   ORA   g1
   BEQ   .cmp_end@18
   LDA   #$01
.cmp_end@18:
   LDY   #$47
   STA   (FP),Y

   ; epilogue
   PHA
   CLC
   LDA   FP
   ADC   #$49
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

   ; prologue: 1 arg bytes, 120 local bytes
   SEC
   LDA   SSP
   SBC   #$7A
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$79
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   LDA   g1
   SEC
   SBC   g1
   LDA   g1+1
   SBC   g1+1
   BVC   .cmp_novf@19
   EOR   #$80
.cmp_novf@19:
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
   LDY   #$7B
   LDA   (FP),Y
   LDY   #$07
   STA   (FP),Y
   LDA   g0
   INY
   STA   (FP),Y
   LDA   g0+1
   INY
   STA   (FP),Y
   LDA   g0+2
   INY
   STA   (FP),Y
   LDA   g0+3
   INY
   STA   (FP),Y
   LDY   #$07
   LDA   (FP),Y
   LDY   #$0C
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$0E
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$0F
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@20
   LDA   #$00
   JMP   .sx_done@21
.sx_neg@20:
   LDA   #$FF
.sx_done@21:
   LDY   #$10
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$0B
   CMP   (FP),Y
   BNE   .cmp_true@22
   DEY
   CMP   (FP),Y
   BNE   .cmp_true@22
   LDY   #$0F
   LDA   (FP),Y
   LDY   #$09
   CMP   (FP),Y
   BNE   .cmp_true@22
   LDY   #$0E
   LDA   (FP),Y
   LDY   #$08
   CMP   (FP),Y
   BNE   .cmp_true@22
   LDA   #$00
   JMP   .cmp_end@23
.cmp_true@22:
   LDA   #$01
.cmp_end@23:
   LDY   #$12
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$14
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$15
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@25
   LDA   #$00
   JMP   .sx_done@26
.sx_neg@25:
   LDA   #$FF
.sx_done@26:
   LDY   #$16
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$7B
   LDA   (FP),Y
   LDY   #$18
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$1A
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$1B
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@27
   LDA   #$00
   JMP   .sx_done@28
.sx_neg@27:
   LDA   #$FF
.sx_done@28:
   LDY   #$1C
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   g0
   CLC
   LDY   #$1A
   ADC   (FP),Y
   LDY   #$1E
   STA   (FP),Y
   LDA   g0+1
   LDY   #$1B
   ADC   (FP),Y
   LDY   #$1F
   STA   (FP),Y
   LDA   g0+2
   LDY   #$1C
   ADC   (FP),Y
   LDY   #$20
   STA   (FP),Y
   LDA   g0+3
   LDY   #$1D
   ADC   (FP),Y
   LDY   #$21
   STA   (FP),Y
   LDA   #$01
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   ORA   #$00
   BPL   .sx_done@30
   LDA   #$FF
.sx_done@30:
   LDY   #$24
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$1E
   LDA   (FP),Y
   LDY   #$22
   ORA   (FP),Y
   LDY   #$26
   STA   (FP),Y
   LDY   #$1F
   LDA   (FP),Y
   LDY   #$23
   ORA   (FP),Y
   LDY   #$27
   STA   (FP),Y
   LDY   #$20
   LDA   (FP),Y
   LDY   #$24
   ORA   (FP),Y
   LDY   #$28
   STA   (FP),Y
   LDY   #$21
   LDA   (FP),Y
   LDY   #$25
   ORA   (FP),Y
   LDY   #$29
   STA   (FP),Y
   LDY   #$14
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
   LDA   HARGS+11
   INY
   STA   (FP),Y
   LDY   #$2A
   LDA   (FP),Y
   LDY   #$2E
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
.loop@1_start:
   LDY   #$30
   LDA   (FP),Y
   SEC
   SBC   #$03
   INY
   LDA   (FP),Y
   SBC   #$00
   BVC   .cmp_novf@31
   EOR   #$80
.cmp_novf@31:
   ASL
   LDA   #$00
   ROL
   LDY   #$32
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@1
   JMP   .loop@1_break
.lb_skip@1:
   LDY   #$07
   LDA   (FP),Y
   LDY   #$34
   STA   (FP),Y
   LDA   #$00
   INY
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
   BMI   .sx_neg@32
   LDA   #$00
   JMP   .sx_done@33
.sx_neg@32:
   LDA   #$FF
.sx_done@33:
   LDY   #$38
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   g0
   SEC
   LDY   #$36
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
   BVC   .cmp_novf@34
   EOR   #$80
.cmp_novf@34:
   ASL
   LDA   #$00
   ROL
   EOR   #$01
   LDY   #$3A
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$3C
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
   LDA   HARGS+8
   LDY   #$3D
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
   LDY   #$30
   LDA   (FP),Y
   LDY   #$41
   STA   (FP),Y
   LDY   #$31
   LDA   (FP),Y
   LDY   #$42
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@35
   LDA   #$00
   JMP   .sx_done@36
.sx_neg@35:
   LDA   #$FF
.sx_done@36:
   LDY   #$43
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$3D
   LDA   (FP),Y
   CLC
   LDY   #$41
   ADC   (FP),Y
   LDY   #$45
   STA   (FP),Y
   LDY   #$3E
   LDA   (FP),Y
   LDY   #$42
   ADC   (FP),Y
   LDY   #$46
   STA   (FP),Y
   LDY   #$3F
   LDA   (FP),Y
   LDY   #$43
   ADC   (FP),Y
   LDY   #$47
   STA   (FP),Y
   LDY   #$40
   LDA   (FP),Y
   LDY   #$44
   ADC   (FP),Y
   LDY   #$48
   STA   (FP),Y
   LDY   #$05
   LDA   (FP),Y
   LDY   #$49
   STA   (FP),Y
   LDY   #$06
   LDA   (FP),Y
   LDY   #$4A
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@37
   LDA   #$00
   JMP   .sx_done@38
.sx_neg@37:
   LDA   #$FF
.sx_done@38:
   LDY   #$4B
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$49
   LDA   (FP),Y
   CLC
   LDY   #$45
   ADC   (FP),Y
   LDY   #$4D
   STA   (FP),Y
   LDY   #$4A
   LDA   (FP),Y
   LDY   #$46
   ADC   (FP),Y
   LDY   #$4E
   STA   (FP),Y
   LDY   #$4B
   LDA   (FP),Y
   LDY   #$47
   ADC   (FP),Y
   LDY   #$4F
   STA   (FP),Y
   LDY   #$4C
   LDA   (FP),Y
   LDY   #$48
   ADC   (FP),Y
   LDY   #$50
   STA   (FP),Y
   LDY   #$4D
   LDA   (FP),Y
   LDY   #$51
   STA   (FP),Y
   LDY   #$4E
   LDA   (FP),Y
   LDY   #$52
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$05
   STA   (FP),Y
   LDY   #$52
   LDA   (FP),Y
   LDY   #$06
   STA   (FP),Y
   LDY   #$30
   LDA   (FP),Y
   LDY   #$53
   STA   (FP),Y
   LDY   #$31
   LDA   (FP),Y
   LDY   #$54
   STA   (FP),Y
   LDY   #$30
   LDA   (FP),Y
   CLC
   ADC   #$01
   LDY   #$55
   STA   (FP),Y
   LDY   #$31
   LDA   (FP),Y
   ADC   #$00
   LDY   #$56
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$30
   STA   (FP),Y
   LDY   #$56
   LDA   (FP),Y
   LDY   #$31
   STA   (FP),Y
   JMP   .loop@1_start
.loop@1_break:
   LDY   #$05
   LDA   (FP),Y
   LDY   #$57
   STA   (FP),Y
   LDY   #$06
   LDA   (FP),Y
   LDY   #$58
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@39
   LDA   #$00
   JMP   .sx_done@40
.sx_neg@39:
   LDA   #$FF
.sx_done@40:
   LDY   #$59
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   g0
   CLC
   LDY   #$57
   ADC   (FP),Y
   LDY   #$5B
   STA   (FP),Y
   LDA   g0+1
   LDY   #$58
   ADC   (FP),Y
   LDY   #$5C
   STA   (FP),Y
   LDA   g0+2
   LDY   #$59
   ADC   (FP),Y
   LDY   #$5D
   STA   (FP),Y
   LDA   g0+3
   LDY   #$5A
   ADC   (FP),Y
   LDY   #$5E
   STA   (FP),Y
   LDY   #$5B
   LDA   (FP),Y
   STA   g0
   INY
   LDA   (FP),Y
   STA   g0+1
   INY
   LDA   (FP),Y
   STA   g0+2
   INY
   LDA   (FP),Y
   STA   g0+3
   LDY   #$05
   LDA   (FP),Y
   LDY   #$5F
   STA   (FP),Y
   LDY   #$06
   LDA   (FP),Y
   LDY   #$60
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@41
   LDA   #$00
   JMP   .sx_done@42
.sx_neg@41:
   LDA   #$FF
.sx_done@42:
   LDY   #$61
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
   LDY   #$5F
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$60
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$61
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$62
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   JSR   f4
   LDA   HARGS+8
   LDY   #$63
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
   LDY   #$2F
   LDA   (FP),Y
   LDY   #$67
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$67
   LDA   (FP),Y
   LDY   #$63
   EOR   (FP),Y
   LDY   #$6B
   STA   (FP),Y
   LDY   #$68
   LDA   (FP),Y
   LDY   #$64
   EOR   (FP),Y
   LDY   #$6C
   STA   (FP),Y
   LDY   #$69
   LDA   (FP),Y
   LDY   #$65
   EOR   (FP),Y
   LDY   #$6D
   STA   (FP),Y
   LDY   #$6A
   LDA   (FP),Y
   LDY   #$66
   EOR   (FP),Y
   LDY   #$6E
   STA   (FP),Y
   LDY   #$6B
   LDA   (FP),Y
   LDY   #$6F
   STA   (FP),Y
   LDY   #$2F
   STA   (FP),Y
   LDY   #$70
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDA   #$FF
   SEC
   DEY
   SBC   (FP),Y
   LDA   #$7F
   INY
   SBC   (FP),Y
   BVC   .cmp_novf@43
   EOR   #$80
.cmp_novf@43:
   ASL
   LDA   #$00
   ROL
   EOR   #$01
   LDY   #$72
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$08
   LDA   (FP),Y
   LDY   #$74
   STA   (FP),Y
   LDY   #$09
   LDA   (FP),Y
   LDY   #$75
   STA   (FP),Y
   LDY   #$72
   LDA   (FP),Y
   SEC
   LDY   #$74
   SBC   (FP),Y
   LDY   #$76
   STA   (FP),Y
   LDY   #$73
   LDA   (FP),Y
   LDY   #$75
   SBC   (FP),Y
   LDY   #$77
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$78
   STA   (FP),Y

   ; epilogue
   PHA
   CLC
   LDA   FP
   ADC   #$7B
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

   ; prologue: 0 arg bytes, 51 local bytes
   SEC
   LDA   SSP
   SBC   #$35
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$34
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
   ORA   g0+2
   ORA   g0+3
   BEQ   .and_false@8
   LDA   g0
   ORA   g0+1
   ORA   g0+2
   ORA   g0+3
   BEQ   .and_false@8
   LDA   #$01
   LDY   #$01
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   JMP   .and_end@9
.and_false@8:
   LDA   #$00
   LDY   #$01
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@9:
   LDY   #$01
   LDA   (FP),Y
   SEC
   SBC   g1
   LDY   #$03
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   SBC   g1+1
   LDY   #$04
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$05
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$64
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$01
   BEQ   .lnot_true@44
   LDA   #$00
.lnot_true@44:
   LDY   #$09
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
   ORA   #$00
   BMI   .sx_neg@46
   LDA   #$00
   JMP   .sx_done@47
.sx_neg@46:
   LDA   #$FF
.sx_done@47:
   LDY   #$0D
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
   LDY   #$0D
   LDA   (FP),Y
   LDY   #$11
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$08
   LDA   (FP),Y
   LDY   #$13
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@48
   LDA   #$00
   JMP   .sx_done@49
.sx_neg@48:
   LDA   #$FF
.sx_done@49:
   LDY   #$14
   STA   (FP),Y
   LDA   #$01
   DEY
   ORA   (FP),Y
   LDY   #$15
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$16
   STA   (FP),Y
   LDY   #$06
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
   LDY   #$15
   LDA   (FP),Y
   SEC
   LDY   #$17
   SBC   (FP),Y
   LDY   #$19
   STA   (FP),Y
   LDY   #$16
   LDA   (FP),Y
   LDY   #$18
   SBC   (FP),Y
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
   BMI   .sx_neg@52
   LDA   #$00
   JMP   .sx_done@53
.sx_neg@52:
   LDA   #$FF
.sx_done@53:
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
   JSR   f2
   LDY   #$23
   STA   (FP),Y
   ORA   #$00
   BEQ   .if_else@11
   LDY   #$06
   LDA   (FP),Y
   LDY   #$24
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@54
   LDA   #$00
   JMP   .sx_done@55
.sx_neg@54:
   LDA   #$FF
.sx_done@55:
   LDY   #$25
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$03
   LDY   #$26
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$27
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$28
   STA   (FP),Y
   LDY   #$06
   STA   (FP),Y
   JMP   .if_end@10
.if_else@11:
   LDY   #$06
   LDA   (FP),Y
   LDY   #$29
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@56
   LDA   #$00
   JMP   .sx_done@57
.sx_neg@56:
   LDA   #$FF
.sx_done@57:
   LDY   #$2A
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   EOR   #$05
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
   LDY   #$06
   STA   (FP),Y
.if_end@10:
   LDY   #$2E
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@58
   LDA   #$00
   JMP   .sx_done@59
.sx_neg@58:
   LDA   #$FF
.sx_done@59:
   LDY   #$2F
   STA   (FP),Y
   LDA   g1
   CLC
   DEY
   ADC   (FP),Y
   LDY   #$30
   STA   (FP),Y
   LDA   g1+1
   DEY
   ADC   (FP),Y
   LDY   #$31
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   g1
   INY
   LDA   (FP),Y
   STA   g1+1
   LDY   #$0F
   LDA   (FP),Y
   LDY   #$32
   STA   (FP),Y
   LDY   #$10
   LDA   (FP),Y
   LDY   #$33
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
   ADC   #$35
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$34
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

   JSR   f0
   LDY   #$01
   STA   (FP),Y
   TXA
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
   BMI   .sx_neg@60
   LDA   #$00
   JMP   .sx_done@61
.sx_neg@60:
   LDA   #$FF
.sx_done@61:
   LDY   #$05
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
   LDA   g1
   INY
   STA   (FP),Y
   LDA   g1+1
   INY
   STA   (FP),Y
   LDA   g1+1
   BMI   .sx_neg@62
   LDA   #$00
   JMP   .sx_done@63
.sx_neg@62:
   LDA   #$FF
.sx_done@63:
   LDY   #$0D
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   g0
   LDY   #$0B
   EOR   (FP),Y
   LDY   #$0F
   STA   (FP),Y
   LDA   g0+1
   LDY   #$0C
   EOR   (FP),Y
   LDY   #$10
   STA   (FP),Y
   LDA   g0+2
   LDY   #$0D
   EOR   (FP),Y
   LDY   #$11
   STA   (FP),Y
   LDA   g0+3
   LDY   #$0E
   EOR   (FP),Y
   LDY   #$12
   STA   (FP),Y
   LDY   #$07
   LDA   (FP),Y
   LDY   #$0F
   EOR   (FP),Y
   LDY   #$13
   STA   (FP),Y
   LDY   #$08
   LDA   (FP),Y
   LDY   #$10
   EOR   (FP),Y
   LDY   #$14
   STA   (FP),Y
   LDY   #$09
   LDA   (FP),Y
   LDY   #$11
   EOR   (FP),Y
   LDY   #$15
   STA   (FP),Y
   LDY   #$0A
   LDA   (FP),Y
   LDY   #$12
   EOR   (FP),Y
   LDY   #$16
   STA   (FP),Y
   LDY   #$13
   LDA   (FP),Y
   LDY   #$07
   STA   (FP),Y
   LDY   #$14
   LDA   (FP),Y
   LDY   #$08
   STA   (FP),Y
   LDY   #$15
   LDA   (FP),Y
   LDY   #$09
   STA   (FP),Y
   LDY   #$16
   LDA   (FP),Y
   LDY   #$0A
   STA   (FP),Y
   LDY   #$07
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
   LDY   #$07
   LDA   (FP),Y
   LDY   #$17
   EOR   (FP),Y
   LDY   #$1B
   STA   (FP),Y
   LDY   #$08
   LDA   (FP),Y
   LDY   #$18
   EOR   (FP),Y
   LDY   #$1C
   STA   (FP),Y
   LDY   #$09
   LDA   (FP),Y
   LDY   #$19
   EOR   (FP),Y
   LDY   #$1D
   STA   (FP),Y
   LDY   #$0A
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
   DC.L  $00000027

g1:
   DC.W  $003A
