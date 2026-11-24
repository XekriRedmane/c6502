SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24

f4:
   SUBROUTINE

   ; prologue: 3 arg bytes, 29 local bytes
   SEC
   LDA   SSP
   SBC   #$1F
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$1E
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
   EOR   #$64
   LDY   #$03
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$04
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   EOR   #$02
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
   ORA   #$00
   BMI   .sx_neg@0
   LDA   #$00
   JMP   .sx_done@1
.sx_neg@0:
   LDA   #$FF
.sx_done@1:
   LDY   #$09
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$07
   LDA   (FP),Y
   LDY   #$0C
   STA   (FP),Y
   LDY   #$08
   LDA   (FP),Y
   LDY   #$0D
   STA   (FP),Y
   LDY   #$09
   LDA   (FP),Y
   LDY   #$0E
   STA   (FP),Y
   INY
   STA   (FP),Y
   AND   #$03
   INY
   STA   (FP),Y
   LDY   #$0C
   LDA   (FP),Y
   LDY   #$11
   STA   (FP),Y
   LDY   #$0D
   LDA   (FP),Y
   LDY   #$12
   STA   (FP),Y
   LDY   #$0E
   LDA   (FP),Y
   LDY   #$13
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$10
   LDA   (FP),Y
   LDY   #$15
   STA   (FP),Y
   LDY   #$21
   LDA   (FP),Y
   LDY   #$16
   STA   (FP),Y
   LDY   #$22
   LDA   (FP),Y
   LDY   #$17
   STA   (FP),Y
   LDA   #$00
   SEC
   LDY   #$21
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
   LDY   #$21
   LDA   (FP),Y
   LDY   #$1C
   STA   (FP),Y
   LDY   #$22
   LDA   (FP),Y
   LDY   #$1D
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
   LDY   #$1E
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

   ; prologue: 10 arg bytes, 40 local bytes
   SEC
   LDA   SSP
   SBC   #$2A
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$29
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   LDA   #$01
   LDY   #$01
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   ORA   #$00
   BPL   .sx_done@4
   LDA   #$FF
.sx_done@4:
   LDY   #$03
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
   LDY   #$03
   LDA   (FP),Y
   LDY   #$07
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
   BPL   .sx_done@6
   LDA   #$FF
.sx_done@6:
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
   LDY   #$33
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$11
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$12
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$02
   LDY   #$13
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$14
   STA   (FP),Y
   LDY   #$33
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
   LDA   #$00
   LDY   #$15
   STA   (FP),Y
   INY
   STA   (FP),Y
   ORA   #$00
   BPL   .sx_done@8
   LDA   #$FF
.sx_done@8:
   LDY   #$17
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$0D
   LDA   (FP),Y
   CLC
   LDY   #$15
   SBC   (FP),Y
   LDY   #$0E
   LDA   (FP),Y
   LDY   #$16
   SBC   (FP),Y
   LDY   #$0F
   LDA   (FP),Y
   LDY   #$17
   SBC   (FP),Y
   LDY   #$10
   LDA   (FP),Y
   LDY   #$18
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
   LDY   #$1B
   STA   (FP),Y
   LDY   #$05
   LDA   (FP),Y
   LDY   #$1C
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$03
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDY   #$1B
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$1D
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$1E
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   JSR   f4
   LDY   #$1F
   STA   (FP),Y
   TXA
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
   LDA   #$00
   INY
   STA   (FP),Y
   INY
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
   LDY   #$0F
   LDA   (FP),Y
   LDY   #$23
   EOR   (FP),Y
   LDY   #$27
   STA   (FP),Y
   LDY   #$10
   LDA   (FP),Y
   LDY   #$24
   EOR   (FP),Y
   LDY   #$28
   STA   (FP),Y
   LDY   #$25
   LDA   (FP),Y
   LDY   #$0D
   STA   (FP),Y
   LDY   #$26
   LDA   (FP),Y
   LDY   #$0E
   STA   (FP),Y
   LDY   #$27
   LDA   (FP),Y
   LDY   #$0F
   STA   (FP),Y
   LDY   #$28
   LDA   (FP),Y
   LDY   #$10
   STA   (FP),Y
   LDA   #$01

   ; epilogue
   PHA
   CLC
   LDA   FP
   ADC   #$34
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$29
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

   ; prologue: 12 arg bytes, 62 local bytes
   SEC
   LDA   SSP
   SBC   #$40
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$3F
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   LDY   #$45
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   LDY   #$46
   LDA   (FP),Y
   LDY   #$02
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@9
   LDA   #$00
   JMP   .sx_done@10
.sx_neg@9:
   LDA   #$FF
.sx_done@10:
   LDY   #$03
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
   LDY   #$03
   LDA   (FP),Y
   LDY   #$07
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
   LDY   #$09
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$0A
   STA   (FP),Y
   LDA   g0
   ORA   g0+1
   ORA   g0+2
   ORA   g0+3
   ORA   g0+4
   BNE   .or_true@0
   LDY   #$45
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@0
   LDA   #$00
   LDY   #$0B
   STA   (FP),Y
   INY
   STA   (FP),Y
   JMP   .or_end@1
.or_true@0:
   LDA   #$01
   LDY   #$0B
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@1:
   LDY   #$09
   LDA   (FP),Y
   CLC
   LDY   #$0B
   ADC   (FP),Y
   LDY   #$0D
   STA   (FP),Y
   LDY   #$0A
   LDA   (FP),Y
   LDY   #$0C
   ADC   (FP),Y
   LDY   #$0E
   STA   (FP),Y
   LDY   #$4B
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$0D
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$0E
   LDA   (FP),Y
   LDY   #$01
   STA   (DPTR),Y
   SEC
   LDA   #$00
   LDY   #$43
   SBC   (FP),Y
   LDY   #$0F
   STA   (FP),Y
   LDA   #$00
   LDY   #$44
   SBC   (FP),Y
   LDY   #$10
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$11
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   AND   #$01
   LDY   #$12
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$13
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   EOR   #$01
   LDY   #$14
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$15
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   SEC
   SBC   #$01
   LDY   #$16
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$17
   STA   (FP),Y
   LDY   #$4B
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$18
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$19
   STA   (FP),Y
   LDA   #$FF
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$18
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
   LDY   #$1C
   STA   (FP),Y
   LDA   HARGS+7
   INY
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$03
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDY   #$17
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$1C
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$1D
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   JSR   f4
   LDY   #$1E
   STA   (FP),Y
   TXA
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
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$05
   LDA   (FP),Y
   LDY   #$20
   EOR   (FP),Y
   LDY   #$24
   STA   (FP),Y
   LDY   #$06
   LDA   (FP),Y
   LDY   #$21
   EOR   (FP),Y
   LDY   #$25
   STA   (FP),Y
   LDY   #$07
   LDA   (FP),Y
   LDY   #$22
   EOR   (FP),Y
   LDY   #$26
   STA   (FP),Y
   LDY   #$08
   LDA   (FP),Y
   LDY   #$23
   EOR   (FP),Y
   LDY   #$27
   STA   (FP),Y
   LDY   #$24
   LDA   (FP),Y
   LDY   #$05
   STA   (FP),Y
   LDY   #$25
   LDA   (FP),Y
   LDY   #$06
   STA   (FP),Y
   LDY   #$26
   LDA   (FP),Y
   LDY   #$07
   STA   (FP),Y
   LDY   #$27
   LDA   (FP),Y
   LDY   #$08
   STA   (FP),Y
   LDA   g0
   LDY   #$28
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
   LDY   #$2F
   STA   (FP),Y
   LDA   #$07
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   ORA   #$00
   BPL   .sx_done@12
   LDA   #$FF
.sx_done@12:
   LDY   #$32
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$47
   LDA   (FP),Y
   LDY   #$30
   AND   (FP),Y
   LDY   #$34
   STA   (FP),Y
   LDY   #$48
   LDA   (FP),Y
   LDY   #$31
   AND   (FP),Y
   LDY   #$35
   STA   (FP),Y
   LDY   #$49
   LDA   (FP),Y
   LDY   #$32
   AND   (FP),Y
   LDY   #$36
   STA   (FP),Y
   LDY   #$4A
   LDA   (FP),Y
   LDY   #$33
   AND   (FP),Y
   LDY   #$37
   STA   (FP),Y
   LDY   #$47
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
   LDY   #$34
   LDA   (FP),Y
   STA   HARGS+4
   JSR   asl32
   LDA   HARGS+5
   LDY   #$38
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
   LDY   #$2C
   LDA   (FP),Y
   SEC
   LDY   #$38
   SBC   (FP),Y
   LDY   #$2D
   LDA   (FP),Y
   LDY   #$39
   SBC   (FP),Y
   LDY   #$2E
   LDA   (FP),Y
   LDY   #$3A
   SBC   (FP),Y
   LDY   #$2F
   LDA   (FP),Y
   LDY   #$3B
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
   LDY   #$3E
   STA   (FP),Y

   ; epilogue
   PHA
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
   PLA
   RTS

f1:
   SUBROUTINE

   ; prologue: 8 arg bytes, 117 local bytes
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

   INY
   LDA   (FP),Y
   EOR   #$FF
   LDY   #$01
   STA   (FP),Y
   LDY   #$79
   LDA   (FP),Y
   EOR   #$FF
   LDY   #$02
   STA   (FP),Y
   LDY   #$7A
   LDA   (FP),Y
   EOR   #$FF
   LDY   #$03
   STA   (FP),Y
   LDY   #$7B
   LDA   (FP),Y
   EOR   #$FF
   LDY   #$04
   STA   (FP),Y
   LDA   #$03
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   ORA   #$00
   BPL   .sx_done@14
   LDA   #$FF
.sx_done@14:
   LDY   #$07
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$01
   LDA   (FP),Y
   CLC
   LDY   #$05
   SBC   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   LDY   #$06
   SBC   (FP),Y
   LDY   #$03
   LDA   (FP),Y
   LDY   #$07
   SBC   (FP),Y
   LDY   #$04
   LDA   (FP),Y
   LDY   #$08
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
   LDY   #$0B
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$0C
   STA   (FP),Y
   LDY   #$7C
   LDA   (FP),Y
   LDY   #$0D
   STA   (FP),Y
   LDY   #$7D
   LDA   (FP),Y
   LDY   #$0E
   STA   (FP),Y
   LDY   #$7E
   LDA   (FP),Y
   LDY   #$0F
   STA   (FP),Y
   LDY   #$7F
   LDA   (FP),Y
   LDY   #$10
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$0D
   LDA   (FP),Y
   LDY   #$12
   STA   (FP),Y
   LDY   #$0E
   LDA   (FP),Y
   LDY   #$13
   STA   (FP),Y
   LDY   #$0F
   LDA   (FP),Y
   LDY   #$14
   STA   (FP),Y
   LDY   #$10
   LDA   (FP),Y
   LDY   #$15
   STA   (FP),Y
   LDY   #$11
   LDA   (FP),Y
   LDY   #$16
   STA   (FP),Y
   LDY   #$0B
   LDA   (FP),Y
   LDY   #$17
   STA   (FP),Y
   LDY   #$0C
   LDA   (FP),Y
   LDY   #$18
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@15
   LDA   #$00
   JMP   .sx_done@16
.sx_neg@15:
   LDA   #$FF
.sx_done@16:
   LDY   #$19
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$17
   LDA   (FP),Y
   LDY   #$1C
   STA   (FP),Y
   LDY   #$18
   LDA   (FP),Y
   LDY   #$1D
   STA   (FP),Y
   LDY   #$19
   LDA   (FP),Y
   LDY   #$1E
   STA   (FP),Y
   INY
   STA   (FP),Y
   AND   #$03
   INY
   STA   (FP),Y
   LDY   #$1C
   LDA   (FP),Y
   LDY   #$21
   STA   (FP),Y
   LDY   #$1D
   LDA   (FP),Y
   LDY   #$22
   STA   (FP),Y
   LDY   #$1E
   LDA   (FP),Y
   LDY   #$23
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$20
   LDA   (FP),Y
   LDY   #$25
   STA   (FP),Y
   LDY   #$0B
   LDA   (FP),Y
   LDY   #$26
   STA   (FP),Y
   LDY   #$0C
   LDA   (FP),Y
   LDY   #$27
   STA   (FP),Y
   LDY   #$7C
   LDA   (FP),Y
   LDY   #$28
   STA   (FP),Y
   LDY   #$7D
   LDA   (FP),Y
   LDY   #$29
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$2A
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   AND   #$01
   LDY   #$2B
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$2C
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   EOR   #$01
   LDY   #$2D
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$2E
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   SEC
   SBC   #$01
   LDY   #$2F
   STA   (FP),Y
   LDA   #$64
   BEQ   .and_false@2
   LDA   #$FF
   BEQ   .and_false@2
   LDA   #$01
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   JMP   .and_end@3
.and_false@2:
   LDA   #$00
   LDY   #$30
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@3:
   LDY   #$30
   LDA   (FP),Y
   LDY   #$32
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   AND   #$01
   LDY   #$33
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$34
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   EOR   #$01
   LDY   #$35
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$36
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   SEC
   SBC   #$01
   LDY   #$37
   STA   (FP),Y
   LDY   #$12
   LDA   (FP),Y
   LDY   #$38
   STA   (FP),Y
   LDY   #$13
   LDA   (FP),Y
   LDY   #$39
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$3A
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   AND   #$01
   LDY   #$3B
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$3C
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   EOR   #$01
   LDY   #$3D
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$3E
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   SEC
   SBC   #$01
   LDY   #$3F
   STA   (FP),Y
   LDA   #$E8
   SEC
   LDY   #$0B
   SBC   (FP),Y
   LDA   #$03
   INY
   SBC   (FP),Y
   BVC   .cmp_novf@17
   EOR   #$80
.cmp_novf@17:
   ASL
   LDA   #$00
   ROL
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
   BMI   .sx_neg@18
   LDA   #$00
   JMP   .sx_done@19
.sx_neg@18:
   LDA   #$FF
.sx_done@19:
   LDY   #$44
   STA   (FP),Y
   INY
   STA   (FP),Y
   CLC
   LDA   FP
   ADC   #$26
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
   LDY   #$2E
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$2F
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$36
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$37
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   LDY   #$3E
   LDA   (FP),Y
   LDY   #$05
   STA   (SSP),Y
   LDY   #$3F
   LDA   (FP),Y
   LDY   #$06
   STA   (SSP),Y
   LDY   #$42
   LDA   (FP),Y
   LDY   #$07
   STA   (SSP),Y
   LDY   #$43
   LDA   (FP),Y
   LDY   #$08
   STA   (SSP),Y
   LDY   #$44
   LDA   (FP),Y
   LDY   #$09
   STA   (SSP),Y
   LDY   #$45
   LDA   (FP),Y
   LDY   #$0A
   STA   (SSP),Y
   LDY   #$46
   LDA   (FP),Y
   LDY   #$0B
   STA   (SSP),Y
   LDY   #$47
   LDA   (FP),Y
   LDY   #$0C
   STA   (SSP),Y
   JSR   f2
   LDY   #$48
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
   LDY   #$21
   LDA   (FP),Y
   LDY   #$49
   EOR   (FP),Y
   LDY   #$4E
   STA   (FP),Y
   LDY   #$22
   LDA   (FP),Y
   LDY   #$4A
   EOR   (FP),Y
   LDY   #$4F
   STA   (FP),Y
   LDY   #$23
   LDA   (FP),Y
   LDY   #$4B
   EOR   (FP),Y
   LDY   #$50
   STA   (FP),Y
   LDY   #$24
   LDA   (FP),Y
   LDY   #$4C
   EOR   (FP),Y
   LDY   #$51
   STA   (FP),Y
   LDY   #$25
   LDA   (FP),Y
   LDY   #$4D
   EOR   (FP),Y
   LDY   #$52
   STA   (FP),Y
   LDY   #$4E
   LDA   (FP),Y
   LDY   #$21
   STA   (FP),Y
   LDY   #$4F
   LDA   (FP),Y
   LDY   #$22
   STA   (FP),Y
   LDY   #$50
   LDA   (FP),Y
   LDY   #$23
   STA   (FP),Y
   LDY   #$51
   LDA   (FP),Y
   LDY   #$24
   STA   (FP),Y
   LDY   #$52
   LDA   (FP),Y
   LDY   #$25
   STA   (FP),Y
   INY
   LDA   (FP),Y
   LDY   #$53
   STA   (FP),Y
   LDY   #$27
   LDA   (FP),Y
   LDY   #$54
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@20
   LDA   #$00
   JMP   .sx_done@21
.sx_neg@20:
   LDA   #$FF
.sx_done@21:
   LDY   #$55
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$53
   LDA   (FP),Y
   LDY   #$58
   STA   (FP),Y
   LDY   #$54
   LDA   (FP),Y
   LDY   #$59
   STA   (FP),Y
   LDY   #$55
   LDA   (FP),Y
   LDY   #$5A
   STA   (FP),Y
   INY
   STA   (FP),Y
   AND   #$03
   INY
   STA   (FP),Y
   LDY   #$21
   LDA   (FP),Y
   CLC
   LDY   #$58
   ADC   (FP),Y
   LDY   #$5D
   STA   (FP),Y
   LDY   #$22
   LDA   (FP),Y
   LDY   #$59
   ADC   (FP),Y
   LDY   #$5E
   STA   (FP),Y
   LDY   #$23
   LDA   (FP),Y
   LDY   #$5A
   ADC   (FP),Y
   LDY   #$5F
   STA   (FP),Y
   LDY   #$24
   LDA   (FP),Y
   LDY   #$5B
   ADC   (FP),Y
   LDY   #$60
   STA   (FP),Y
   LDY   #$25
   LDA   (FP),Y
   LDY   #$5C
   ADC   (FP),Y
   LDY   #$61
   STA   (FP),Y
   LDY   #$5D
   LDA   (FP),Y
   LDY   #$62
   STA   (FP),Y
   LDY   #$5E
   LDA   (FP),Y
   LDY   #$63
   STA   (FP),Y
   LDY   #$5F
   LDA   (FP),Y
   LDY   #$64
   STA   (FP),Y
   LDY   #$60
   LDA   (FP),Y
   LDY   #$65
   STA   (FP),Y
   LDY   #$61
   LDA   (FP),Y
   AND   #$03
   LDY   #$66
   STA   (FP),Y
   LDY   #$62
   LDA   (FP),Y
   LDY   #$21
   STA   (FP),Y
   LDY   #$63
   LDA   (FP),Y
   LDY   #$22
   STA   (FP),Y
   LDY   #$64
   LDA   (FP),Y
   LDY   #$23
   STA   (FP),Y
   LDY   #$65
   LDA   (FP),Y
   LDY   #$24
   STA   (FP),Y
   LDY   #$66
   LDA   (FP),Y
   LDY   #$25
   STA   (FP),Y
   LDA   #$01
   BEQ   .and_false@4
   LDA   #$02
   BEQ   .and_false@4
   LDA   #$01
   LDY   #$67
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   JMP   .and_end@5
.and_false@4:
   LDA   #$00
   LDY   #$67
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@5:
   LDY   #$67
   LDA   (FP),Y
   LDY   #$69
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$6A
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@22
   LDA   #$00
   JMP   .sx_done@23
.sx_neg@22:
   LDA   #$FF
.sx_done@23:
   LDY   #$6B
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
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
   INY
   STA   (FP),Y
   AND   #$03
   INY
   STA   (FP),Y
   LDY   #$6E
   LDA   (FP),Y
   CLC
   SBC   g0
   INY
   LDA   (FP),Y
   SBC   g0+1
   INY
   LDA   (FP),Y
   SBC   g0+2
   INY
   LDA   (FP),Y
   SBC   g0+3
   INY
   LDA   (FP),Y
   SBC   g0+4
   LDA   #$00
   ROL
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$75
   STA   (FP),Y
   CMP   #$01

   ; epilogue
   PHP
   CLC
   LDA   FP
   ADC   #$7F
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

   ; prologue: 5 arg bytes, 192 local bytes
   SEC
   LDA   SSP
   SBC   #$C2
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$C1
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   LDA   g0
   AND   #$07
   LDY   #$01
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
   LDY   #$C3
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
   LDA   #$00
   STA   HARGS+5
   STA   HARGS+6
   STA   HARGS+7
   LDY   #$01
   LDA   (FP),Y
   STA   HARGS+8
   JSR   lsr64
   LDA   HARGS+9
   LDY   #$06
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
   LDA   g1
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
   ORA   #$00
   BMI   .sx_neg@24
   LDA   #$00
   JMP   .sx_done@25
.sx_neg@24:
   LDA   #$FF
.sx_done@25:
   LDY   #$0F
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$0D
   LDA   (FP),Y
   LDY   #$12
   STA   (FP),Y
   LDY   #$0E
   LDA   (FP),Y
   LDY   #$13
   STA   (FP),Y
   LDY   #$0F
   LDA   (FP),Y
   LDY   #$14
   STA   (FP),Y
   INY
   STA   (FP),Y
   AND   #$03
   INY
   STA   (FP),Y
   LDY   #$12
   LDA   (FP),Y
   SEC
   LDY   #$06
   SBC   (FP),Y
   LDY   #$13
   LDA   (FP),Y
   LDY   #$07
   SBC   (FP),Y
   LDY   #$14
   LDA   (FP),Y
   LDY   #$08
   SBC   (FP),Y
   LDY   #$15
   LDA   (FP),Y
   LDY   #$09
   SBC   (FP),Y
   LDY   #$16
   LDA   (FP),Y
   LDY   #$0A
   SBC   (FP),Y
   LDA   #$00
   ROL
   LDY   #$17
   STA   (FP),Y
   LDA   #$00
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
   LDA   g0
   LDY   #$C3
   ORA   (FP),Y
   LDY   #$1B
   STA   (FP),Y
   LDA   g0+1
   LDY   #$C4
   ORA   (FP),Y
   LDY   #$1C
   STA   (FP),Y
   LDA   g0+2
   LDY   #$C5
   ORA   (FP),Y
   LDY   #$1D
   STA   (FP),Y
   LDA   g0+3
   LDY   #$C6
   ORA   (FP),Y
   LDY   #$1E
   STA   (FP),Y
   LDA   g0+4
   LDY   #$C7
   ORA   (FP),Y
   LDY   #$1F
   STA   (FP),Y
   LDY   #$1B
   LDA   (FP),Y
   LDY   #$20
   STA   (FP),Y
   LDY   #$C3
   LDA   (FP),Y
   LDY   #$21
   STA   (FP),Y
   LDY   #$C4
   LDA   (FP),Y
   LDY   #$22
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$03
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDY   #$20
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$21
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$22
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   JSR   f4
   LDY   #$23
   STA   (FP),Y
   TXA
   INY
   STA   (FP),Y
   LDY   #$19
   LDA   (FP),Y
   LDY   #$25
   STA   (FP),Y
   LDY   #$1A
   LDA   (FP),Y
   LDY   #$26
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$23
   EOR   (FP),Y
   LDY   #$27
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$24
   EOR   (FP),Y
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
   DEY
   LDA   (FP),Y
   LDY   #$19
   STA   (FP),Y
   LDY   #$2A
   LDA   (FP),Y
   LDY   #$1A
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$2B
   STA   (FP),Y
   LDY   #$1A
   LDA   (FP),Y
   LDY   #$2C
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@26
   LDA   #$00
   JMP   .sx_done@27
.sx_neg@26:
   LDA   #$FF
.sx_done@27:
   LDY   #$2D
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$2B
   LDA   (FP),Y
   LDY   #$30
   STA   (FP),Y
   LDY   #$2C
   LDA   (FP),Y
   LDY   #$31
   STA   (FP),Y
   LDY   #$2D
   LDA   (FP),Y
   LDY   #$32
   STA   (FP),Y
   INY
   STA   (FP),Y
   AND   #$03
   INY
   STA   (FP),Y
   LDA   g0
   CLC
   LDY   #$30
   ADC   (FP),Y
   LDY   #$35
   STA   (FP),Y
   LDA   g0+1
   LDY   #$31
   ADC   (FP),Y
   LDY   #$36
   STA   (FP),Y
   LDA   g0+2
   LDY   #$32
   ADC   (FP),Y
   LDY   #$37
   STA   (FP),Y
   LDA   g0+3
   LDY   #$33
   ADC   (FP),Y
   LDY   #$38
   STA   (FP),Y
   LDA   g0+4
   LDY   #$34
   ADC   (FP),Y
   LDY   #$39
   STA   (FP),Y
   LDY   #$35
   LDA   (FP),Y
   LDY   #$3A
   STA   (FP),Y
   LDY   #$36
   LDA   (FP),Y
   LDY   #$3B
   STA   (FP),Y
   LDY   #$37
   LDA   (FP),Y
   LDY   #$3C
   STA   (FP),Y
   LDY   #$38
   LDA   (FP),Y
   LDY   #$3D
   STA   (FP),Y
   LDY   #$39
   LDA   (FP),Y
   AND   #$03
   LDY   #$3E
   STA   (FP),Y
   LDY   #$3A
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
   INY
   LDA   (FP),Y
   STA   g0+4
   LDY   #$19
   LDA   (FP),Y
   LDY   #$3F
   STA   (FP),Y
   LDY   #$1A
   LDA   (FP),Y
   LDY   #$40
   STA   (FP),Y
   LDA   g0
   INY
   STA   (FP),Y
   LDA   g0+1
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$43
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   AND   #$01
   LDY   #$44
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$45
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   EOR   #$01
   LDY   #$46
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$47
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   SEC
   SBC   #$01
   LDY   #$48
   STA   (FP),Y
   LDA   g1
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$4B
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$4C
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@28
   LDA   #$00
   JMP   .sx_done@29
.sx_neg@28:
   LDA   #$FF
.sx_done@29:
   LDY   #$4D
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$4B
   LDA   (FP),Y
   LDY   #$50
   STA   (FP),Y
   LDY   #$4C
   LDA   (FP),Y
   LDY   #$51
   STA   (FP),Y
   LDY   #$4D
   LDA   (FP),Y
   LDY   #$52
   STA   (FP),Y
   INY
   STA   (FP),Y
   AND   #$03
   INY
   STA   (FP),Y
   LDY   #$50
   LDA   (FP),Y
   SEC
   LDY   #$C3
   SBC   (FP),Y
   LDY   #$55
   STA   (FP),Y
   LDY   #$51
   LDA   (FP),Y
   LDY   #$C4
   SBC   (FP),Y
   LDY   #$56
   STA   (FP),Y
   LDY   #$52
   LDA   (FP),Y
   LDY   #$C5
   SBC   (FP),Y
   LDY   #$57
   STA   (FP),Y
   LDY   #$53
   LDA   (FP),Y
   LDY   #$C6
   SBC   (FP),Y
   LDY   #$58
   STA   (FP),Y
   LDY   #$54
   LDA   (FP),Y
   LDY   #$C7
   SBC   (FP),Y
   LDY   #$59
   STA   (FP),Y
   LDY   #$55
   LDA   (FP),Y
   LDY   #$5A
   STA   (FP),Y
   LDY   #$56
   LDA   (FP),Y
   LDY   #$5B
   STA   (FP),Y
   LDY   #$57
   LDA   (FP),Y
   LDY   #$5C
   STA   (FP),Y
   LDY   #$58
   LDA   (FP),Y
   LDY   #$5D
   STA   (FP),Y
   LDY   #$59
   LDA   (FP),Y
   AND   #$03
   LDY   #$5E
   STA   (FP),Y
   LDY   #$5A
   LDA   (FP),Y
   LDY   #$5F
   STA   (FP),Y
   LDY   #$5B
   LDA   (FP),Y
   LDY   #$60
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$61
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   AND   #$01
   LDY   #$62
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$63
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   EOR   #$01
   LDY   #$64
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$65
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   SEC
   SBC   #$01
   LDY   #$66
   STA   (FP),Y
   LDA   #$07
   BNE   .or_true@6
   LDY   #$C3
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   INY
   ORA   (FP),Y
   INY
   ORA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@6
   LDA   #$00
   LDY   #$67
   STA   (FP),Y
   INY
   STA   (FP),Y
   JMP   .or_end@7
.or_true@6:
   LDA   #$01
   LDY   #$67
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@7:
   LDY   #$67
   LDA   (FP),Y
   LDY   #$69
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   AND   #$01
   LDY   #$6A
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$6B
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   EOR   #$01
   LDY   #$6C
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$6D
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   SEC
   SBC   #$01
   LDY   #$6E
   STA   (FP),Y
   LDA   g1
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$71
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$72
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@30
   LDA   #$00
   JMP   .sx_done@31
.sx_neg@30:
   LDA   #$FF
.sx_done@31:
   LDY   #$73
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$71
   LDA   (FP),Y
   LDY   #$76
   STA   (FP),Y
   LDY   #$72
   LDA   (FP),Y
   LDY   #$77
   STA   (FP),Y
   LDY   #$73
   LDA   (FP),Y
   LDY   #$78
   STA   (FP),Y
   INY
   STA   (FP),Y
   AND   #$03
   INY
   STA   (FP),Y
   LDA   g0
   LDY   #$76
   AND   (FP),Y
   LDY   #$7B
   STA   (FP),Y
   LDA   g0+1
   LDY   #$77
   AND   (FP),Y
   LDY   #$7C
   STA   (FP),Y
   LDA   g0+2
   LDY   #$78
   AND   (FP),Y
   LDY   #$7D
   STA   (FP),Y
   LDA   g0+3
   LDY   #$79
   AND   (FP),Y
   LDY   #$7E
   STA   (FP),Y
   LDA   g0+4
   LDY   #$7A
   AND   (FP),Y
   LDY   #$7F
   STA   (FP),Y
   LDY   #$7B
   LDA   (FP),Y
   LDY   #$80
   STA   (FP),Y
   LDY   #$7C
   LDA   (FP),Y
   LDY   #$81
   STA   (FP),Y
   LDY   #$7D
   LDA   (FP),Y
   LDY   #$82
   STA   (FP),Y
   LDY   #$7E
   LDA   (FP),Y
   LDY   #$83
   STA   (FP),Y
   CLC
   LDA   FP
   ADC   #$3F
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
   LDY   #$47
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$48
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$65
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$66
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   LDY   #$6D
   LDA   (FP),Y
   LDY   #$05
   STA   (SSP),Y
   LDY   #$6E
   LDA   (FP),Y
   LDY   #$06
   STA   (SSP),Y
   LDY   #$80
   LDA   (FP),Y
   LDY   #$07
   STA   (SSP),Y
   LDY   #$81
   LDA   (FP),Y
   LDY   #$08
   STA   (SSP),Y
   LDY   #$82
   LDA   (FP),Y
   LDY   #$09
   STA   (SSP),Y
   LDY   #$83
   LDA   (FP),Y
   LDY   #$0A
   STA   (SSP),Y
   LDY   #$84
   LDA   (FP),Y
   LDY   #$0B
   STA   (SSP),Y
   LDY   #$85
   LDA   (FP),Y
   LDY   #$0C
   STA   (SSP),Y
   JSR   f2
   LDY   #$86
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$19
   LDA   (FP),Y
   LDY   #$87
   EOR   (FP),Y
   LDY   #$89
   STA   (FP),Y
   LDY   #$1A
   LDA   (FP),Y
   LDY   #$88
   EOR   (FP),Y
   LDY   #$8A
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$19
   STA   (FP),Y
   LDY   #$8A
   LDA   (FP),Y
   LDY   #$1A
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   LDY   #$3F
   ADC   (FP),Y
   LDY   #$8B
   STA   (FP),Y
   LDY   #$1A
   LDA   (FP),Y
   LDY   #$40
   ADC   (FP),Y
   LDY   #$8C
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$19
   STA   (FP),Y
   LDY   #$8C
   LDA   (FP),Y
   LDY   #$1A
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$8D
   STA   (FP),Y
   LDY   #$1A
   LDA   (FP),Y
   LDY   #$8E
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@32
   LDA   #$00
   JMP   .sx_done@33
.sx_neg@32:
   LDA   #$FF
.sx_done@33:
   LDY   #$8F
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$8D
   LDA   (FP),Y
   LDY   #$92
   STA   (FP),Y
   LDY   #$8E
   LDA   (FP),Y
   LDY   #$93
   STA   (FP),Y
   LDY   #$8F
   LDA   (FP),Y
   LDY   #$94
   STA   (FP),Y
   INY
   STA   (FP),Y
   AND   #$03
   INY
   STA   (FP),Y
   LDA   g0
   CLC
   LDY   #$92
   ADC   (FP),Y
   LDY   #$97
   STA   (FP),Y
   LDA   g0+1
   LDY   #$93
   ADC   (FP),Y
   LDY   #$98
   STA   (FP),Y
   LDA   g0+2
   LDY   #$94
   ADC   (FP),Y
   LDY   #$99
   STA   (FP),Y
   LDA   g0+3
   LDY   #$95
   ADC   (FP),Y
   LDY   #$9A
   STA   (FP),Y
   LDA   g0+4
   LDY   #$96
   ADC   (FP),Y
   LDY   #$9B
   STA   (FP),Y
   LDY   #$97
   LDA   (FP),Y
   LDY   #$9C
   STA   (FP),Y
   LDY   #$98
   LDA   (FP),Y
   LDY   #$9D
   STA   (FP),Y
   LDY   #$99
   LDA   (FP),Y
   LDY   #$9E
   STA   (FP),Y
   LDY   #$9A
   LDA   (FP),Y
   LDY   #$9F
   STA   (FP),Y
   LDY   #$9B
   LDA   (FP),Y
   AND   #$03
   LDY   #$A0
   STA   (FP),Y
   LDY   #$9C
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
   INY
   LDA   (FP),Y
   STA   g0+4
   LDY   #$19
   LDA   (FP),Y
   LDY   #$A1
   STA   (FP),Y
   LDY   #$1A
   LDA   (FP),Y
   LDY   #$A2
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@34
   LDA   #$00
   JMP   .sx_done@35
.sx_neg@34:
   LDA   #$FF
.sx_done@35:
   LDY   #$A3
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$A1
   LDA   (FP),Y
   LDY   #$A6
   STA   (FP),Y
   LDY   #$A2
   LDA   (FP),Y
   LDY   #$A7
   STA   (FP),Y
   LDY   #$A3
   LDA   (FP),Y
   LDY   #$A8
   STA   (FP),Y
   INY
   STA   (FP),Y
   AND   #$03
   INY
   STA   (FP),Y
   CMP   g0+4
   BNE   .cmp_true@36
   DEY
   LDA   (FP),Y
   CMP   g0+3
   BNE   .cmp_true@36
   CMP   g0+2
   BNE   .cmp_true@36
   LDY   #$A7
   LDA   (FP),Y
   CMP   g0+1
   BNE   .cmp_true@36
   DEY
   LDA   (FP),Y
   CMP   g0
   BNE   .cmp_true@36
   LDA   #$00
   JMP   .cmp_end@37
.cmp_true@36:
   LDA   #$01
.cmp_end@37:
   LDY   #$AB
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@0
   JMP   .or_true@8
.lb_skip@0:
   LDY   #$19
   LDA   (FP),Y
   ORA   #$01
   LDY   #$AD
   STA   (FP),Y
   LDY   #$1A
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
   ORA   #$00
   BMI   .sx_neg@39
   LDA   #$00
   JMP   .sx_done@40
.sx_neg@39:
   LDA   #$FF
.sx_done@40:
   LDY   #$B1
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$AF
   LDA   (FP),Y
   LDY   #$B4
   STA   (FP),Y
   LDY   #$B0
   LDA   (FP),Y
   LDY   #$B5
   STA   (FP),Y
   LDY   #$B1
   LDA   (FP),Y
   LDY   #$B6
   STA   (FP),Y
   INY
   STA   (FP),Y
   AND   #$03
   INY
   STA   (FP),Y
   LDY   #$C3
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
   LDA   #$00
   STA   HARGS+5
   STA   HARGS+6
   STA   HARGS+7
   LDY   #$B4
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
   LDA   #$00
   STA   HARGS+13
   STA   HARGS+14
   STA   HARGS+15
   JSR   udivmod64
   LDA   HARGS+16
   LDY   #$B9
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
   LDY   #$B9
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   INY
   ORA   (FP),Y
   INY
   ORA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@8
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   JMP   .or_end@9
.or_true@8:
   LDA   #$01
   LDY   #$BE
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@9:
   LDY   #$BE
   LDA   (FP),Y
   LDY   #$C0
   STA   (FP),Y

   ; epilogue
   PHA
   CLC
   LDA   FP
   ADC   #$C7
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
   LDA   SSP
   SBC   #$05
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   #$BF
   LDY   #$01
   STA   (SSP),Y
   LDA   #$00
   INY
   STA   (SSP),Y
   INY
   STA   (SSP),Y
   INY
   STA   (SSP),Y
   INY
   STA   (SSP),Y
   JSR   f0
   LDY   #$01
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
   LDY   #$03
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
   LDA   g0+2
   INY
   STA   (FP),Y
   LDA   g0+3
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
   LDY   #$06
   LDA   (FP),Y
   LDY   #$12
   EOR   (FP),Y
   LDY   #$16
   STA   (FP),Y
   LDY   #$07
   LDA   (FP),Y
   LDY   #$13
   EOR   (FP),Y
   LDY   #$17
   STA   (FP),Y
   LDY   #$08
   LDA   (FP),Y
   LDY   #$14
   EOR   (FP),Y
   LDY   #$18
   STA   (FP),Y
   LDY   #$09
   LDA   (FP),Y
   LDY   #$15
   EOR   (FP),Y
   LDY   #$19
   STA   (FP),Y
   LDY   #$16
   LDA   (FP),Y
   LDY   #$06
   STA   (FP),Y
   LDY   #$17
   LDA   (FP),Y
   LDY   #$07
   STA   (FP),Y
   LDY   #$18
   LDA   (FP),Y
   LDY   #$08
   STA   (FP),Y
   LDY   #$19
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
   LDY   #$1A
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
   LDY   #$22
   STA   (FP),Y
   LDY   #$1F
   LDA   (FP),Y
   LDY   #$23
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
   ADC   #$25
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$24
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
   DC.B  $34
   DS.B  4

g1:
   DC.B  $57
