SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24

f3:
   SUBROUTINE

   ; prologue: 0 arg bytes, 74 local bytes
   SEC
   LDA   SSP
   SBC   #$4C
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$4B
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
   INY
   STA   (FP),Y
   LDA   g0
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
   BPL   .sx_done@1
   LDA   #$FF
.sx_done@1:
   LDY   #$07
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   g2
   LDY   #$05
   AND   (FP),Y
   LDY   #$09
   STA   (FP),Y
   LDA   g2+1
   LDY   #$06
   AND   (FP),Y
   LDY   #$0A
   STA   (FP),Y
   LDA   g2+2
   LDY   #$07
   AND   (FP),Y
   LDY   #$0B
   STA   (FP),Y
   LDA   g2+3
   LDY   #$08
   AND   (FP),Y
   LDY   #$0C
   STA   (FP),Y
   LDY   #$03
   LDA   (FP),Y
   LDY   #$0D
   STA   (FP),Y
   LDY   #$04
   LDA   (FP),Y
   LDY   #$0E
   STA   (FP),Y
   LDY   #$09
   LDA   (FP),Y
   TAX
   BEQ   .shift_done@3
.shift_loop@2:
   LDY   #$0D
   LDA   (FP),Y
   ASL
   STA   (FP),Y
   INY
   LDA   (FP),Y
   ROL
   STA   (FP),Y
   DEX
   BNE   .shift_loop@2
.shift_done@3:
   LDY   #$0D
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@0
   LDY   #$02
   LDA   (FP),Y
   LDY   #$0F
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
   ORA   #$00
   BMI   .sx_neg@4
   LDA   #$00
   JMP   .sx_done@5
.sx_neg@4:
   LDA   #$FF
.sx_done@5:
   LDY   #$13
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   g2+3
   CMP   (FP),Y
   BNE   .cmp_true@6
   LDA   g2+2
   DEY
   CMP   (FP),Y
   BNE   .cmp_true@6
   LDA   g2+1
   DEY
   CMP   (FP),Y
   BNE   .cmp_true@6
   LDA   g2
   DEY
   CMP   (FP),Y
   BNE   .cmp_true@6
   LDA   #$00
   JMP   .cmp_end@7
.cmp_true@6:
   LDA   #$01
.cmp_end@7:
   LDY   #$15
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@0
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   JMP   .or_end@1
.or_true@0:
   LDA   #$01
   LDY   #$17
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@1:
   LDY   #$17
   LDA   (FP),Y
   LDY   #$19
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$1A
   STA   (FP),Y
   LDA   #$07
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   ORA   #$00
   BPL   .sx_done@10
   LDA   #$FF
.sx_done@10:
   LDY   #$1D
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   g1
   LDY   #$1B
   ORA   (FP),Y
   LDY   #$1F
   STA   (FP),Y
   LDA   g1+1
   LDY   #$1C
   ORA   (FP),Y
   LDY   #$20
   STA   (FP),Y
   LDA   g1+2
   LDY   #$1D
   ORA   (FP),Y
   LDY   #$21
   STA   (FP),Y
   LDA   g1+3
   LDY   #$1E
   ORA   (FP),Y
   LDY   #$22
   STA   (FP),Y
   LDA   g0
   BNE   .or_true@2
   LDY   #$19
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@2
   LDA   #$00
   LDY   #$23
   STA   (FP),Y
   INY
   STA   (FP),Y
   JMP   .or_end@3
.or_true@2:
   LDA   #$01
   LDY   #$23
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@3:
   LDY   #$23
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
   BMI   .sx_neg@11
   LDA   #$00
   JMP   .sx_done@12
.sx_neg@11:
   LDA   #$FF
.sx_done@12:
   LDY   #$29
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$1F
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
   LDY   #$27
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
   LDY   #$2B
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
   LDY   #$2B
   LDA   (FP),Y
   LDY   #$2F
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
   LDY   #$33
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$34
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@13
   LDA   #$00
   JMP   .sx_done@14
.sx_neg@13:
   LDA   #$FF
.sx_done@14:
   LDY   #$35
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   g2
   CLC
   LDY   #$33
   SBC   (FP),Y
   LDA   g2+1
   INY
   SBC   (FP),Y
   LDA   g2+2
   INY
   SBC   (FP),Y
   LDA   g2+3
   INY
   SBC   (FP),Y
   LDA   #$00
   ROL
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$30
   LDA   (FP),Y
   LDY   #$39
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@15
   LDA   #$00
   JMP   .sx_done@16
.sx_neg@15:
   LDA   #$FF
.sx_done@16:
   LDY   #$3A
   STA   (FP),Y
   LDY   #$37
   LDA   (FP),Y
   CLC
   LDY   #$39
   ADC   (FP),Y
   LDY   #$3B
   STA   (FP),Y
   LDY   #$38
   LDA   (FP),Y
   LDY   #$3A
   ADC   (FP),Y
   LDY   #$3C
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$3D
   STA   (FP),Y
   DEY
   LDA   (FP),Y
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
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lnot_true@17
   LDA   #$00
   JMP   .lnot_end@18
.lnot_true@17:
   LDA   #$01
.lnot_end@18:
   LDY   #$41
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$19
   LDA   (FP),Y
   LDY   #$43
   STA   (FP),Y
   LDY   #$1A
   LDA   (FP),Y
   LDY   #$44
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@19
   LDA   #$00
   JMP   .sx_done@20
.sx_neg@19:
   LDA   #$FF
.sx_done@20:
   LDY   #$45
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   g2
   SEC
   LDY   #$43
   SBC   (FP),Y
   LDA   g2+1
   INY
   SBC   (FP),Y
   LDA   g2+2
   INY
   SBC   (FP),Y
   LDA   g2+3
   INY
   SBC   (FP),Y
   LDA   #$00
   ROL
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$41
   LDA   (FP),Y
   CLC
   LDY   #$47
   ADC   (FP),Y
   LDY   #$49
   STA   (FP),Y
   LDY   #$42
   LDA   (FP),Y
   LDY   #$48
   ADC   (FP),Y
   LDY   #$4A
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
   ADC   #$4C
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$4B
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

f2:
   SUBROUTINE

   ; prologue: 6 arg bytes, 132 local bytes
   SEC
   LDA   SSP
   SBC   #$86
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$85
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
   LDY   #$87
   LDA   (FP),Y
   LDY   #$03
   STA   (FP),Y
   LDY   #$88
   LDA   (FP),Y
   LDY   #$04
   STA   (FP),Y
   LDY   #$89
   LDA   (FP),Y
   LDY   #$05
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@21
   LDA   #$00
   JMP   .sx_done@22
.sx_neg@21:
   LDA   #$FF
.sx_done@22:
   LDY   #$06
   STA   (FP),Y
   LDY   #$03
   LDA   (FP),Y
   SEC
   SBC   g1
   INY
   LDA   (FP),Y
   SBC   g1+1
   INY
   LDA   (FP),Y
   SBC   g1+2
   INY
   LDA   (FP),Y
   SBC   g1+3
   BVC   .cmp_novf@23
   EOR   #$80
.cmp_novf@23:
   ASL
   LDA   #$00
   ROL
   EOR   #$01
   LDY   #$07
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$01
   LDA   (FP),Y
   LDY   #$07
   AND   (FP),Y
   LDY   #$09
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   LDY   #$08
   AND   (FP),Y
   LDY   #$0A
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$0B
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
   SEC
   SBC   #$07
   INY
   LDA   (FP),Y
   SBC   #$00
   BVC   .cmp_novf@24
   EOR   #$80
.cmp_novf@24:
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
   DEY
   LDA   (FP),Y
   LDY   #$12
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@25
   LDA   #$00
   JMP   .sx_done@26
.sx_neg@25:
   LDA   #$FF
.sx_done@26:
   LDY   #$13
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   g1
   INY
   STA   (FP),Y
   LDA   g1+1
   INY
   STA   (FP),Y
   LDA   g1+2
   INY
   STA   (FP),Y
   LDA   g1+3
   INY
   STA   (FP),Y
   LDA   #$01
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   ORA   #$00
   BPL   .sx_done@28
   LDA   #$FF
.sx_done@28:
   LDY   #$1B
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   g2
   LDY   #$19
   ORA   (FP),Y
   LDY   #$1D
   STA   (FP),Y
   LDA   g2+1
   LDY   #$1A
   ORA   (FP),Y
   LDY   #$1E
   STA   (FP),Y
   LDA   g2+2
   LDY   #$1B
   ORA   (FP),Y
   LDY   #$1F
   STA   (FP),Y
   LDA   g2+3
   LDY   #$1C
   ORA   (FP),Y
   LDY   #$20
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
   LDY   #$1D
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
   LDY   #$21
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
   LDY   #$27
   STA   (FP),Y
   INY
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
   LDY   #$23
   LDA   (FP),Y
   LDY   #$27
   ORA   (FP),Y
   LDY   #$2B
   STA   (FP),Y
   LDY   #$24
   LDA   (FP),Y
   LDY   #$28
   ORA   (FP),Y
   LDY   #$2C
   STA   (FP),Y
   LDY   #$11
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
   LDY   #$29
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
   LDY   #$2F
   LDA   (FP),Y
   LDY   #$33
   STA   (FP),Y
   LDY   #$31
   LDA   (FP),Y
   LDY   #$34
   STA   (FP),Y
   LDY   #$32
   LDA   (FP),Y
   LDY   #$35
   STA   (FP),Y
   LDY   #$33
   LDA   (FP),Y
   AND   #$03
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
   LDY   #$36
   LDA   (FP),Y
   EOR   #$02
   LDY   #$39
   STA   (FP),Y
   LDY   #$37
   LDA   (FP),Y
   LDY   #$3A
   STA   (FP),Y
   LDY   #$38
   LDA   (FP),Y
   LDY   #$3B
   STA   (FP),Y
   LDY   #$39
   LDA   (FP),Y
   SEC
   SBC   #$02
   LDY   #$3C
   STA   (FP),Y
   LDY   #$3A
   LDA   (FP),Y
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
   LDA   #$64
   SEC
   LDY   #$3D
   SBC   (FP),Y
   LDA   #$00
   INY
   SBC   (FP),Y
   LDA   #$00
   INY
   SBC   (FP),Y
   BVC   .cmp_novf@31
   EOR   #$80
.cmp_novf@31:
   ASL
   LDA   #$00
   ROL
   EOR   #$01
   LDY   #$40
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$42
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$43
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@32
   LDA   #$00
   JMP   .sx_done@33
.sx_neg@32:
   LDA   #$FF
.sx_done@33:
   LDY   #$44
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
   BPL   .sx_done@35
   LDA   #$FF
.sx_done@35:
   LDY   #$48
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$46
   LDA   (FP),Y
   SEC
   SBC   g2
   LDY   #$4A
   STA   (FP),Y
   LDY   #$47
   LDA   (FP),Y
   SBC   g2+1
   LDY   #$4B
   STA   (FP),Y
   LDY   #$48
   LDA   (FP),Y
   SBC   g2+2
   LDY   #$4C
   STA   (FP),Y
   LDY   #$49
   LDA   (FP),Y
   SBC   g2+3
   LDY   #$4D
   STA   (FP),Y
   LDY   #$42
   LDA   (FP),Y
   LDY   #$4A
   EOR   (FP),Y
   LDY   #$4E
   STA   (FP),Y
   LDY   #$43
   LDA   (FP),Y
   LDY   #$4B
   EOR   (FP),Y
   LDY   #$4F
   STA   (FP),Y
   LDY   #$44
   LDA   (FP),Y
   LDY   #$4C
   EOR   (FP),Y
   LDY   #$50
   STA   (FP),Y
   LDY   #$45
   LDA   (FP),Y
   LDY   #$4D
   EOR   (FP),Y
   LDY   #$51
   STA   (FP),Y
   LDY   #$4E
   LDA   (FP),Y
   LDY   #$52
   STA   (FP),Y
   LDY   #$4F
   LDA   (FP),Y
   LDY   #$53
   STA   (FP),Y
   LDY   #$50
   LDA   (FP),Y
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
   LDY   #$54
   LDA   (FP),Y
   AND   #$03
   LDY   #$57
   STA   (FP),Y
   LDY   #$55
   LDA   (FP),Y
   LDY   #$58
   STA   (FP),Y
   LDY   #$56
   LDA   (FP),Y
   LDY   #$59
   STA   (FP),Y
   LDY   #$57
   LDA   (FP),Y
   EOR   #$02
   LDY   #$5A
   STA   (FP),Y
   LDY   #$58
   LDA   (FP),Y
   LDY   #$5B
   STA   (FP),Y
   LDY   #$59
   LDA   (FP),Y
   LDY   #$5C
   STA   (FP),Y
   LDY   #$5A
   LDA   (FP),Y
   SEC
   SBC   #$02
   LDY   #$5D
   STA   (FP),Y
   LDY   #$5B
   LDA   (FP),Y
   LDY   #$5E
   STA   (FP),Y
   LDY   #$5C
   LDA   (FP),Y
   LDY   #$5F
   STA   (FP),Y
   LDY   #$5D
   LDA   (FP),Y
   LDY   #$60
   STA   (FP),Y
   LDY   #$8A
   LDA   (FP),Y
   LDY   #$61
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$63
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$64
   STA   (FP),Y
   LDY   #$0C
   LDA   (FP),Y
   LDY   #$65
   STA   (FP),Y
   LDY   #$8B
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$66
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$67
   STA   (FP),Y
   LDY   #$0C
   LDA   (FP),Y
   LDY   #$68
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   EOR   #$FF
   LDY   #$6A
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   EOR   #$FF
   LDY   #$6B
   STA   (FP),Y
   LDY   #$66
   LDA   (FP),Y
   CLC
   LDY   #$6A
   ADC   (FP),Y
   LDY   #$6C
   STA   (FP),Y
   LDY   #$67
   LDA   (FP),Y
   LDY   #$6B
   ADC   (FP),Y
   LDY   #$6D
   STA   (FP),Y
   LDY   #$8B
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$6C
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$6D
   LDA   (FP),Y
   LDY   #$01
   STA   (DPTR),Y
   SEC
   LDA   #$00
   LDY   #$87
   SBC   (FP),Y
   LDY   #$6E
   STA   (FP),Y
   LDA   #$00
   LDY   #$88
   SBC   (FP),Y
   LDY   #$6F
   STA   (FP),Y
   LDA   #$00
   LDY   #$89
   SBC   (FP),Y
   LDY   #$70
   STA   (FP),Y
   LDY   #$6E
   LDA   (FP),Y
   LDY   #$71
   STA   (FP),Y
   LDY   #$6F
   LDA   (FP),Y
   LDY   #$72
   STA   (FP),Y
   LDY   #$70
   LDA   (FP),Y
   AND   #$03
   LDY   #$73
   STA   (FP),Y
   LDY   #$71
   LDA   (FP),Y
   LDY   #$74
   STA   (FP),Y
   LDY   #$72
   LDA   (FP),Y
   LDY   #$75
   STA   (FP),Y
   LDY   #$73
   LDA   (FP),Y
   EOR   #$02
   LDY   #$76
   STA   (FP),Y
   LDY   #$74
   LDA   (FP),Y
   LDY   #$77
   STA   (FP),Y
   LDY   #$75
   LDA   (FP),Y
   LDY   #$78
   STA   (FP),Y
   LDY   #$76
   LDA   (FP),Y
   SEC
   SBC   #$02
   LDY   #$79
   STA   (FP),Y
   LDA   #$64
   SEC
   LDY   #$63
   SBC   (FP),Y
   LDY   #$7A
   STA   (FP),Y
   LDA   #$00
   LDY   #$64
   SBC   (FP),Y
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
   BMI   .sx_neg@36
   LDA   #$00
   JMP   .sx_done@37
.sx_neg@36:
   LDA   #$FF
.sx_done@37:
   LDY   #$7E
   STA   (FP),Y
   LDY   #$77
   LDA   (FP),Y
   SEC
   LDY   #$7C
   SBC   (FP),Y
   LDY   #$78
   LDA   (FP),Y
   LDY   #$7D
   SBC   (FP),Y
   LDY   #$79
   LDA   (FP),Y
   LDY   #$7E
   SBC   (FP),Y
   BVC   .cmp_novf@38
   EOR   #$80
.cmp_novf@38:
   ASL
   LDA   #$00
   ROL
   EOR   #$01
   LDY   #$7F
   STA   (FP),Y
   LDA   #$00
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
   ORA   #$00
   BMI   .sx_neg@39
   LDA   #$00
   JMP   .sx_done@40
.sx_neg@39:
   LDA   #$FF
.sx_done@40:
   LDY   #$83
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$81
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
   ADC   #$8C
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

f1:
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
   LDA   g1+2
   INY
   STA   (FP),Y
   LDA   g1+3
   INY
   STA   (FP),Y
   LDA   #$01
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   ORA   #$00
   BPL   .sx_done@42
   LDA   #$FF
.sx_done@42:
   LDY   #$07
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   g2
   LDY   #$05
   ORA   (FP),Y
   LDY   #$09
   STA   (FP),Y
   LDA   g2+1
   LDY   #$06
   ORA   (FP),Y
   LDY   #$0A
   STA   (FP),Y
   LDA   g2+2
   LDY   #$07
   ORA   (FP),Y
   LDY   #$0B
   STA   (FP),Y
   LDA   g2+3
   LDY   #$08
   ORA   (FP),Y
   LDY   #$0C
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
   JSR   udivmod32
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
   LDY   #$11
   LDA   (FP),Y
   LDY   #$15
   STA   (FP),Y
   LDY   #$12
   LDA   (FP),Y
   LDY   #$16
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$10
   LDA   (FP),Y
   LDY   #$18
   CMP   (FP),Y
   BNE   .cmp_true@43
   LDY   #$0F
   LDA   (FP),Y
   LDY   #$17
   CMP   (FP),Y
   BNE   .cmp_true@43
   LDY   #$0E
   LDA   (FP),Y
   LDY   #$16
   CMP   (FP),Y
   BNE   .cmp_true@43
   LDY   #$0D
   LDA   (FP),Y
   LDY   #$15
   CMP   (FP),Y
   BNE   .cmp_true@43
   LDA   #$00
   JMP   .cmp_end@44
.cmp_true@43:
   LDA   #$01
.cmp_end@44:
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
   DEY
   LDA   (FP),Y
   LDY   #$1D
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$1E
   STA   (FP),Y
   LDA   g0
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
   BPL   .sx_done@47
   LDA   #$FF
.sx_done@47:
   LDY   #$23
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   g1
   LDY   #$21
   AND   (FP),Y
   LDY   #$25
   STA   (FP),Y
   LDA   g1+1
   LDY   #$22
   AND   (FP),Y
   LDY   #$26
   STA   (FP),Y
   LDA   g1+2
   LDY   #$23
   AND   (FP),Y
   LDY   #$27
   STA   (FP),Y
   LDA   g1+3
   LDY   #$24
   AND   (FP),Y
   LDY   #$28
   STA   (FP),Y
   LDY   #$1F
   LDA   (FP),Y
   LDY   #$29
   STA   (FP),Y
   LDY   #$20
   LDA   (FP),Y
   LDY   #$2A
   STA   (FP),Y
   LDY   #$25
   LDA   (FP),Y
   TAX
   BEQ   .shift_done@49
.shift_loop@48:
   LDY   #$29
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
   LDY   #$29
   LDA   (FP),Y
   LDY   #$2B
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$2C
   STA   (FP),Y
   LDY   #$1D
   LDA   (FP),Y
   EOR   #$FF
   LDY   #$2D
   STA   (FP),Y
   LDY   #$1E
   LDA   (FP),Y
   EOR   #$FF
   LDY   #$2E
   STA   (FP),Y
   LDY   #$2B
   LDA   (FP),Y
   CLC
   LDY   #$2D
   SBC   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$2E
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
   LDY   #$31
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$32
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@50
   LDA   #$00
   JMP   .sx_done@51
.sx_neg@50:
   LDA   #$FF
.sx_done@51:
   LDY   #$33
   STA   (FP),Y
   INY
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
   LDY   #$3B
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDA   #$00
   LDY   #$3C
   STA   (FP),Y
   LDA   #$03
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$3B
   LDA   (FP),Y
   TAX
   BEQ   .shift_done@53
.shift_loop@52:
   LDY   #$3D
   LDA   (FP),Y
   ASL
   STA   (FP),Y
   INY
   LDA   (FP),Y
   ROL
   STA   (FP),Y
   DEX
   BNE   .shift_loop@52
.shift_done@53:
   LDY   #$3D
   LDA   (FP),Y
   SEC
   SBC   #$01
   INY
   LDA   (FP),Y
   SBC   #$00
   BVC   .cmp_novf@54
   EOR   #$80
.cmp_novf@54:
   ASL
   LDA   #$00
   ROL
   EOR   #$01
   LDY   #$3F
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$41
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$42
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$43
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$44
   STA   (FP),Y
   LDA   #$07
   CLC
   LDY   #$1D
   SBC   (FP),Y
   LDA   #$00
   INY
   SBC   (FP),Y
   LDA   #$00
   ROL
   LDY   #$45
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$47
   STA   (FP),Y
   CMP   #$01

   ; epilogue
   PHP
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
   PLP
   RTS

f0:
   SUBROUTINE

   ; prologue: 7 arg bytes, 97 local bytes
   SEC
   LDA   SSP
   SBC   #$63
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$62
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
   LDA   #$00
   INY
   SBC   (FP),Y
   BVC   .cmp_novf@55
   EOR   #$80
.cmp_novf@55:
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
   LDY   #$03
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$04
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@56
   LDA   #$00
   JMP   .sx_done@57
.sx_neg@56:
   LDA   #$FF
.sx_done@57:
   LDY   #$05
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   g1
   INY
   STA   (FP),Y
   LDA   g1+1
   INY
   STA   (FP),Y
   LDA   g1+2
   INY
   STA   (FP),Y
   LDA   g1+3
   INY
   STA   (FP),Y
   LDY   #$07
   LDA   (FP),Y
   CLC
   ADC   g2
   LDY   #$0B
   STA   (FP),Y
   LDY   #$08
   LDA   (FP),Y
   ADC   g2+1
   LDY   #$0C
   STA   (FP),Y
   LDY   #$09
   LDA   (FP),Y
   ADC   g2+2
   LDY   #$0D
   STA   (FP),Y
   LDY   #$0A
   LDA   (FP),Y
   ADC   g2+3
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
   JSR   mul32
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
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$16
   LDA   (FP),Y
   LDY   #$1A
   STA   (FP),Y
   LDY   #$18
   LDA   (FP),Y
   LDY   #$1B
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
   BPL   .sx_done@59
   LDA   #$FF
.sx_done@59:
   LDY   #$1F
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$67
   LDA   (FP),Y
   LDY   #$1D
   AND   (FP),Y
   LDY   #$21
   STA   (FP),Y
   LDY   #$68
   LDA   (FP),Y
   LDY   #$1E
   AND   (FP),Y
   LDY   #$22
   STA   (FP),Y
   LDY   #$69
   LDA   (FP),Y
   LDY   #$1F
   AND   (FP),Y
   LDY   #$23
   STA   (FP),Y
   LDY   #$6A
   LDA   (FP),Y
   LDY   #$20
   AND   (FP),Y
   LDY   #$24
   STA   (FP),Y
   LDY   #$1B
   LDA   (FP),Y
   LDY   #$25
   STA   (FP),Y
   LDY   #$1C
   LDA   (FP),Y
   LDY   #$26
   STA   (FP),Y
   LDY   #$21
   LDA   (FP),Y
   TAX
   CPX   #$00
   BEQ   .shift_done@61
.shift_loop@60:
   LDY   #$25
   LDA   (FP),Y
   ASL
   STA   (FP),Y
   INY
   LDA   (FP),Y
   ROL
   STA   (FP),Y
   DEX
   BNE   .shift_loop@60
.shift_done@61:
   LDY   #$25
   LDA   (FP),Y
   LDY   #$27
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$28
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@62
   LDA   #$00
   JMP   .sx_done@63
.sx_neg@62:
   LDA   #$FF
.sx_done@63:
   LDY   #$29
   STA   (FP),Y
   CLC
   LDA   FP
   ADC   #$19
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
   LDY   #$27
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$28
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$29
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDA   g0
   INY
   STA   (SSP),Y
   LDY   #$2A
   LDA   (FP),Y
   LDY   #$05
   STA   (SSP),Y
   LDY   #$2B
   LDA   (FP),Y
   LDY   #$06
   STA   (SSP),Y
   JSR   f2
   LDA   HARGS+8
   LDY   #$2C
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
   LDY   #$30
   STA   (FP),Y
   LDY   #$16
   LDA   (FP),Y
   LDY   #$31
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@64
   LDA   #$00
   JMP   .sx_done@65
.sx_neg@64:
   LDA   #$FF
.sx_done@65:
   LDY   #$32
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$30
   LDA   (FP),Y
   LDY   #$2C
   EOR   (FP),Y
   LDY   #$34
   STA   (FP),Y
   LDY   #$31
   LDA   (FP),Y
   LDY   #$2D
   EOR   (FP),Y
   LDY   #$35
   STA   (FP),Y
   LDY   #$32
   LDA   (FP),Y
   LDY   #$2E
   EOR   (FP),Y
   LDY   #$36
   STA   (FP),Y
   LDY   #$33
   LDA   (FP),Y
   LDY   #$2F
   EOR   (FP),Y
   LDY   #$37
   STA   (FP),Y
   LDY   #$34
   LDA   (FP),Y
   LDY   #$38
   STA   (FP),Y
   LDY   #$35
   LDA   (FP),Y
   LDY   #$39
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$15
   STA   (FP),Y
   LDY   #$39
   LDA   (FP),Y
   LDY   #$16
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   LDY   #$19
   ADC   (FP),Y
   LDY   #$3A
   STA   (FP),Y
   LDY   #$16
   LDA   (FP),Y
   LDY   #$1A
   ADC   (FP),Y
   LDY   #$3B
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$15
   STA   (FP),Y
   LDY   #$3B
   LDA   (FP),Y
   LDY   #$16
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$3C
   STA   (FP),Y
   LDY   #$16
   LDA   (FP),Y
   LDY   #$3D
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@66
   LDA   #$00
   JMP   .sx_done@67
.sx_neg@66:
   LDA   #$FF
.sx_done@67:
   LDY   #$3E
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   g1
   CLC
   LDY   #$3C
   ADC   (FP),Y
   LDY   #$40
   STA   (FP),Y
   LDA   g1+1
   LDY   #$3D
   ADC   (FP),Y
   LDY   #$41
   STA   (FP),Y
   LDA   g1+2
   LDY   #$3E
   ADC   (FP),Y
   LDY   #$42
   STA   (FP),Y
   LDA   g1+3
   LDY   #$3F
   ADC   (FP),Y
   LDY   #$43
   STA   (FP),Y
   LDY   #$40
   LDA   (FP),Y
   STA   g1
   INY
   LDA   (FP),Y
   STA   g1+1
   INY
   LDA   (FP),Y
   STA   g1+2
   INY
   LDA   (FP),Y
   STA   g1+3
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
.loop@0_start:
   LDY   #$44
   LDA   (FP),Y
   SEC
   SBC   #$03
   INY
   LDA   (FP),Y
   SBC   #$00
   BVC   .cmp_novf@68
   EOR   #$80
.cmp_novf@68:
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
   BNE   .lb_skip@0
   JMP   .loop@0_break
.lb_skip@0:
   JSR   f3
   LDY   #$48
   STA   (FP),Y
   TXA
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   LDY   #$44
   ADC   (FP),Y
   LDY   #$4A
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$45
   ADC   (FP),Y
   LDY   #$4B
   STA   (FP),Y
   LDY   #$15
   LDA   (FP),Y
   CLC
   LDY   #$4A
   ADC   (FP),Y
   LDY   #$4C
   STA   (FP),Y
   LDY   #$16
   LDA   (FP),Y
   LDY   #$4B
   ADC   (FP),Y
   LDY   #$4D
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$15
   STA   (FP),Y
   LDY   #$4D
   LDA   (FP),Y
   LDY   #$16
   STA   (FP),Y
   LDY   #$44
   LDA   (FP),Y
   LDY   #$4E
   STA   (FP),Y
   LDY   #$45
   LDA   (FP),Y
   LDY   #$4F
   STA   (FP),Y
   LDY   #$44
   LDA   (FP),Y
   CLC
   ADC   #$01
   LDY   #$50
   STA   (FP),Y
   LDY   #$45
   LDA   (FP),Y
   ADC   #$00
   LDY   #$51
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$44
   STA   (FP),Y
   LDY   #$51
   LDA   (FP),Y
   LDY   #$45
   STA   (FP),Y
   JMP   .loop@0_start
.loop@0_break:
   LDY   #$15
   LDA   (FP),Y
   LDY   #$52
   STA   (FP),Y
   LDY   #$16
   LDA   (FP),Y
   LDY   #$53
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@69
   LDA   #$00
   JMP   .sx_done@70
.sx_neg@69:
   LDA   #$FF
.sx_done@70:
   LDY   #$54
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   g2
   CLC
   LDY   #$52
   ADC   (FP),Y
   LDY   #$56
   STA   (FP),Y
   LDA   g2+1
   LDY   #$53
   ADC   (FP),Y
   LDY   #$57
   STA   (FP),Y
   LDA   g2+2
   LDY   #$54
   ADC   (FP),Y
   LDY   #$58
   STA   (FP),Y
   LDA   g2+3
   LDY   #$55
   ADC   (FP),Y
   LDY   #$59
   STA   (FP),Y
   LDY   #$56
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
   LDA   #$02
   LDY   #$15
   EOR   (FP),Y
   LDY   #$5A
   STA   (FP),Y
   LDA   #$00
   LDY   #$16
   EOR   (FP),Y
   LDY   #$5B
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$5C
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@71
   LDA   #$00
   JMP   .cmp_end@72
.cmp_true@71:
   LDA   #$01
.cmp_end@72:
   LDY   #$5F
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$61
   STA   (FP),Y
   CMP   #$01

   ; epilogue
   PHP
   CLC
   LDA   FP
   ADC   #$6A
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

   LDA   #$1D
   LDY   #$01
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   ORA   #$00
   BPL   .sx_done@74
   LDA   #$FF
.sx_done@74:
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
   LDA   #$D2
   LDY   #$01
   STA   (SSP),Y
   LDA   #$00
   INY
   STA   (SSP),Y
   INY
   STA   (SSP),Y
   LDY   #$01
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   LDY   #$02
   LDA   (FP),Y
   LDY   #$05
   STA   (SSP),Y
   LDY   #$03
   LDA   (FP),Y
   LDY   #$06
   STA   (SSP),Y
   LDY   #$04
   LDA   (FP),Y
   LDY   #$07
   STA   (SSP),Y
   JSR   f0
   LDA   #$00
   ROL
   LDY   #$05
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
   LDY   #$0E
   LDA   (FP),Y
   EOR   g1
   LDY   #$12
   STA   (FP),Y
   LDY   #$0F
   LDA   (FP),Y
   EOR   g1+1
   LDY   #$13
   STA   (FP),Y
   LDY   #$10
   LDA   (FP),Y
   EOR   g1+2
   LDY   #$14
   STA   (FP),Y
   LDY   #$11
   LDA   (FP),Y
   EOR   g1+3
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
   LDY   #$0A
   LDA   (FP),Y
   LDY   #$1A
   EOR   (FP),Y
   LDY   #$1E
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
   LDY   #$1E
   LDA   (FP),Y
   LDY   #$0A
   STA   (FP),Y
   LDY   #$1F
   LDA   (FP),Y
   LDY   #$0B
   STA   (FP),Y
   LDY   #$20
   LDA   (FP),Y
   LDY   #$0C
   STA   (FP),Y
   LDY   #$21
   LDA   (FP),Y
   LDY   #$0D
   STA   (FP),Y
   LDY   #$0A
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
   LDY   #$0A
   LDA   (FP),Y
   LDY   #$22
   EOR   (FP),Y
   LDY   #$26
   STA   (FP),Y
   LDY   #$0B
   LDA   (FP),Y
   LDY   #$23
   EOR   (FP),Y
   LDY   #$27
   STA   (FP),Y
   LDY   #$0C
   LDA   (FP),Y
   LDY   #$24
   EOR   (FP),Y
   LDY   #$28
   STA   (FP),Y
   LDY   #$0D
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
   DC.B  $7B

g1:
   DC.L  $000000A2

g2:
   DC.L  $00000073
