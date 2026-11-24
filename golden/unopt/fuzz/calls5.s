SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24

f4:
   SUBROUTINE

   ; prologue: 3 arg bytes, 18 local bytes
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

   LDA   g0
   LDY   #$01
   STA   (FP),Y
   LDA   g0+1
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
   INY
   STA   (FP),Y
   LDY   #$16
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$07
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$08
   STA   (FP),Y
   LDA   #$E8
   INY
   STA   (FP),Y
   LDA   #$03
   INY
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@0
   LDA   #$00
   JMP   .sx_done@1
.sx_neg@0:
   LDA   #$FF
.sx_done@1:
   LDY   #$0B
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$09
   LDA   (FP),Y
   CLC
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
   LDA   #$00
   ROL
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$07
   LDA   (FP),Y
   CLC
   LDY   #$0D
   ADC   (FP),Y
   LDY   #$0F
   STA   (FP),Y
   LDY   #$08
   LDA   (FP),Y
   LDY   #$0E
   ADC   (FP),Y
   LDY   #$10
   STA   (FP),Y
   LDY   #$16
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$0F
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$10
   LDA   (FP),Y
   LDY   #$01
   STA   (DPTR),Y
   LDA   g0
   LDY   #$11
   STA   (FP),Y
   LDA   g0+1
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
   ADC   #$17
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$13
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

   ; prologue: 3 arg bytes, 62 local bytes
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

   LDY   #$43
   LDA   (FP),Y
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
   LDY   #$42
   LDA   (FP),Y
   LDY   #$05
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@2
   LDA   #$00
   JMP   .sx_done@3
.sx_neg@2:
   LDA   #$FF
.sx_done@3:
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
   LDA   #$07
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   ORA   #$00
   BPL   .sx_done@5
   LDA   #$FF
.sx_done@5:
   LDY   #$0B
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   g2
   LDY   #$09
   AND   (FP),Y
   LDY   #$0D
   STA   (FP),Y
   LDA   g2+1
   LDY   #$0A
   AND   (FP),Y
   LDY   #$0E
   STA   (FP),Y
   LDA   g2+2
   LDY   #$0B
   AND   (FP),Y
   LDY   #$0F
   STA   (FP),Y
   LDA   g2+3
   LDY   #$0C
   AND   (FP),Y
   LDY   #$10
   STA   (FP),Y
   LDY   #$07
   LDA   (FP),Y
   LDY   #$11
   STA   (FP),Y
   LDY   #$08
   LDA   (FP),Y
   LDY   #$12
   STA   (FP),Y
   LDY   #$0D
   LDA   (FP),Y
   TAX
   BEQ   .shift_done@7
.shift_loop@6:
   LDY   #$12
   LDA   (FP),Y
   LSR
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ROR
   STA   (FP),Y
   DEX
   BNE   .shift_loop@6
.shift_done@7:
   LDY   #$11
   LDA   (FP),Y
   ORA   #$01
   LDY   #$13
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$14
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$15
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$03
   LDA   (FP),Y
   LDY   #$17
   STA   (FP),Y
   LDY   #$04
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
   LDY   #$16
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
   BPL   .sx_done@9
   LDA   #$FF
.sx_done@9:
   LDY   #$1F
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   g2
   LDY   #$1D
   AND   (FP),Y
   LDY   #$21
   STA   (FP),Y
   LDA   g2+1
   LDY   #$1E
   AND   (FP),Y
   LDY   #$22
   STA   (FP),Y
   LDA   g2+2
   LDY   #$1F
   AND   (FP),Y
   LDY   #$23
   STA   (FP),Y
   LDA   g2+3
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
   BEQ   .shift_done@11
.shift_loop@10:
   LDY   #$26
   LDA   (FP),Y
   LSR
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ROR
   STA   (FP),Y
   DEX
   BNE   .shift_loop@10
.shift_done@11:
   LDY   #$25
   LDA   (FP),Y
   LDY   #$27
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
   SBC   #$03
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
   JSR   f4
   LDY   #$2A
   STA   (FP),Y
   TXA
   INY
   STA   (FP),Y
   LDY   #$03
   LDA   (FP),Y
   LDY   #$2A
   EOR   (FP),Y
   LDY   #$2C
   STA   (FP),Y
   LDY   #$04
   LDA   (FP),Y
   LDY   #$2B
   EOR   (FP),Y
   LDY   #$2D
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$03
   STA   (FP),Y
   LDY   #$2D
   LDA   (FP),Y
   LDY   #$04
   STA   (FP),Y
   LDY   #$19
   LDA   (FP),Y
   LDY   #$2E
   STA   (FP),Y
   LDY   #$1A
   LDA   (FP),Y
   LDY   #$2F
   STA   (FP),Y
   LDY   #$03
   LDA   (FP),Y
   CLC
   LDY   #$2E
   ADC   (FP),Y
   LDY   #$30
   STA   (FP),Y
   LDY   #$04
   LDA   (FP),Y
   LDY   #$2F
   ADC   (FP),Y
   LDY   #$31
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$03
   STA   (FP),Y
   LDY   #$31
   LDA   (FP),Y
   LDY   #$04
   STA   (FP),Y
   LDY   #$16
   LDA   (FP),Y
   LDY   #$32
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$34
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$35
   STA   (FP),Y
   LDY   #$41
   LDA   (FP),Y
   LDY   #$36
   STA   (FP),Y
   LDA   #$00
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
   LDY   #$04
   LDA   (FP),Y
   LDY   #$39
   CMP   (FP),Y
   BNE   .cmp_true@12
   LDY   #$03
   LDA   (FP),Y
   LDY   #$38
   CMP   (FP),Y
   BNE   .cmp_true@12
   LDA   #$00
   JMP   .cmp_end@13
.cmp_true@12:
   LDA   #$01
.cmp_end@13:
   LDY   #$3A
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$34
   LDA   (FP),Y
   SEC
   LDY   #$3A
   SBC   (FP),Y
   LDY   #$35
   LDA   (FP),Y
   LDY   #$3B
   SBC   (FP),Y
   BVC   .cmp_novf@15
   EOR   #$80
.cmp_novf@15:
   ASL
   LDA   #$00
   ROL
   LDY   #$3C
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
   ADC   #$43
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

   ; prologue: 7 arg bytes, 98 local bytes
   SEC
   LDA   SSP
   SBC   #$64
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$63
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
   LDA   g0+1
   CMP   #$7F
   BNE   .thread@0
   LDA   g0
   CMP   #$FF
   BEQ   .cmp_true@16
.thread@0:
   LDA   #$00
   JMP   .cmp_end@17
.cmp_true@16:
   LDA   #$01
.cmp_end@17:
   LDY   #$03
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   LDY   #$04
   CMP   (FP),Y
   BNE   .cmp_true@19
   LDY   #$01
   LDA   (FP),Y
   LDY   #$03
   CMP   (FP),Y
   BNE   .cmp_true@19
   LDA   #$00
   JMP   .cmp_end@20
.cmp_true@19:
   LDA   #$01
.cmp_end@20:
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
   BMI   .sx_neg@22
   LDA   #$00
   JMP   .sx_done@23
.sx_neg@22:
   LDA   #$FF
.sx_done@23:
   LDY   #$09
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
   LDA   #$FF
   INY
   STA   (FP),Y
   LDA   #$7F
   INY
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@24
   LDA   #$00
   JMP   .sx_done@25
.sx_neg@24:
   LDA   #$FF
.sx_done@25:
   LDY   #$11
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$67
   LDA   (FP),Y
   LDY   #$0F
   AND   (FP),Y
   LDY   #$13
   STA   (FP),Y
   LDY   #$68
   LDA   (FP),Y
   LDY   #$10
   AND   (FP),Y
   LDY   #$14
   STA   (FP),Y
   LDY   #$69
   LDA   (FP),Y
   LDY   #$11
   AND   (FP),Y
   LDY   #$15
   STA   (FP),Y
   LDY   #$6A
   LDA   (FP),Y
   LDY   #$12
   AND   (FP),Y
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
   LDY   #$15
   LDA   (FP),Y
   LDY   #$19
   STA   (FP),Y
   LDY   #$16
   LDA   (FP),Y
   LDY   #$1A
   STA   (FP),Y
   LDY   #$0B
   LDA   (FP),Y
   LDY   #$1B
   STA   (FP),Y
   LDY   #$0C
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
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
.loop@0_start:
   LDY   #$1F
   LDA   (FP),Y
   SEC
   SBC   #$03
   INY
   LDA   (FP),Y
   SBC   #$00
   BVC   .cmp_novf@26
   EOR   #$80
.cmp_novf@26:
   ASL
   LDA   #$00
   ROL
   LDY   #$21
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
   LDA   g0
   ORA   g0+1
   BEQ   .and_false@0
   LDY   #$17
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   INY
   ORA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@0
   LDA   #$01
   LDY   #$23
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   JMP   .and_end@1
.and_false@0:
   LDA   #$00
   LDY   #$23
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@1:
   LDY   #$23
   LDA   (FP),Y
   LDY   #$25
   STA   (FP),Y
   CLC
   LDA   FP
   ADC   #$1D
   INY
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$03
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDY   #$25
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$26
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$27
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   JSR   f4
   LDY   #$28
   STA   (FP),Y
   TXA
   INY
   STA   (FP),Y
   LDY   #$1F
   LDA   (FP),Y
   LDY   #$2A
   STA   (FP),Y
   LDY   #$20
   LDA   (FP),Y
   LDY   #$2B
   STA   (FP),Y
   LDY   #$28
   LDA   (FP),Y
   CLC
   LDY   #$2A
   ADC   (FP),Y
   LDY   #$2C
   STA   (FP),Y
   LDY   #$29
   LDA   (FP),Y
   LDY   #$2B
   ADC   (FP),Y
   LDY   #$2D
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$2E
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$2F
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$17
   LDA   (FP),Y
   CLC
   LDY   #$2E
   ADC   (FP),Y
   LDY   #$32
   STA   (FP),Y
   LDY   #$18
   LDA   (FP),Y
   LDY   #$2F
   ADC   (FP),Y
   LDY   #$33
   STA   (FP),Y
   LDY   #$19
   LDA   (FP),Y
   LDY   #$30
   ADC   (FP),Y
   LDY   #$34
   STA   (FP),Y
   LDY   #$1A
   LDA   (FP),Y
   LDY   #$31
   ADC   (FP),Y
   LDY   #$35
   STA   (FP),Y
   LDY   #$32
   LDA   (FP),Y
   LDY   #$17
   STA   (FP),Y
   LDY   #$33
   LDA   (FP),Y
   LDY   #$18
   STA   (FP),Y
   LDY   #$34
   LDA   (FP),Y
   LDY   #$19
   STA   (FP),Y
   LDY   #$35
   LDA   (FP),Y
   LDY   #$1A
   STA   (FP),Y
   LDY   #$1F
   LDA   (FP),Y
   LDY   #$36
   STA   (FP),Y
   LDY   #$20
   LDA   (FP),Y
   LDY   #$37
   STA   (FP),Y
   LDY   #$1F
   LDA   (FP),Y
   CLC
   ADC   #$01
   LDY   #$38
   STA   (FP),Y
   LDY   #$20
   LDA   (FP),Y
   ADC   #$00
   LDY   #$39
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$1F
   STA   (FP),Y
   LDY   #$39
   LDA   (FP),Y
   LDY   #$20
   STA   (FP),Y
   JMP   .loop@0_start
.loop@0_break:
   LDY   #$1D
   LDA   (FP),Y
   LDY   #$3A
   STA   (FP),Y
   LDY   #$1E
   LDA   (FP),Y
   LDY   #$3B
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@27
   LDA   #$00
   JMP   .sx_done@28
.sx_neg@27:
   LDA   #$FF
.sx_done@28:
   LDY   #$3C
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$17
   LDA   (FP),Y
   CLC
   LDY   #$3A
   ADC   (FP),Y
   LDY   #$3E
   STA   (FP),Y
   LDY   #$18
   LDA   (FP),Y
   LDY   #$3B
   ADC   (FP),Y
   LDY   #$3F
   STA   (FP),Y
   LDY   #$19
   LDA   (FP),Y
   LDY   #$3C
   ADC   (FP),Y
   LDY   #$40
   STA   (FP),Y
   LDY   #$1A
   LDA   (FP),Y
   LDY   #$3D
   ADC   (FP),Y
   LDY   #$41
   STA   (FP),Y
   LDY   #$3E
   LDA   (FP),Y
   LDY   #$17
   STA   (FP),Y
   LDY   #$3F
   LDA   (FP),Y
   LDY   #$18
   STA   (FP),Y
   LDY   #$40
   LDA   (FP),Y
   LDY   #$19
   STA   (FP),Y
   LDY   #$41
   LDA   (FP),Y
   LDY   #$1A
   STA   (FP),Y
   LDA   g0+1
   CMP   g0+1
   BNE   .cmp_true@29
   LDA   g0
   CMP   g0
   BNE   .cmp_true@29
   LDA   #$00
   JMP   .cmp_end@30
.cmp_true@29:
   LDA   #$01
.cmp_end@30:
   LDY   #$42
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$44
   STA   (FP),Y
   LDA   #$07
   INY
   STA   (FP),Y
   LDY   #$65
   LDA   (FP),Y
   LDY   #$46
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   SEC
   SBC   g0
   INY
   LDA   (FP),Y
   SBC   g0+1
   BVC   .cmp_novf@32
   EOR   #$80
.cmp_novf@32:
   ASL
   LDA   #$00
   ROL
   LDY   #$48
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$4A
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$03
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDY   #$44
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$45
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$4A
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   JSR   f3
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
   LDY   #$17
   LDA   (FP),Y
   LDY   #$4C
   EOR   (FP),Y
   LDY   #$50
   STA   (FP),Y
   LDY   #$18
   LDA   (FP),Y
   LDY   #$4D
   EOR   (FP),Y
   LDY   #$51
   STA   (FP),Y
   LDY   #$19
   LDA   (FP),Y
   LDY   #$4E
   EOR   (FP),Y
   LDY   #$52
   STA   (FP),Y
   LDY   #$1A
   LDA   (FP),Y
   LDY   #$4F
   EOR   (FP),Y
   LDY   #$53
   STA   (FP),Y
   LDY   #$50
   LDA   (FP),Y
   LDY   #$17
   STA   (FP),Y
   LDY   #$51
   LDA   (FP),Y
   LDY   #$18
   STA   (FP),Y
   LDY   #$52
   LDA   (FP),Y
   LDY   #$19
   STA   (FP),Y
   LDY   #$53
   LDA   (FP),Y
   LDY   #$1A
   STA   (FP),Y
   LDA   g2
   LDY   #$54
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
   LDY   #$6A
   CMP   (FP),Y
   BNE   .thread@1
   LDY   #$56
   LDA   (FP),Y
   LDY   #$69
   CMP   (FP),Y
   BNE   .thread@1
   LDY   #$55
   LDA   (FP),Y
   LDY   #$68
   CMP   (FP),Y
   BNE   .thread@1
   LDY   #$54
   LDA   (FP),Y
   LDY   #$67
   CMP   (FP),Y
   BEQ   .cmp_true@33
.thread@1:
   LDA   #$00
   JMP   .cmp_end@34
.cmp_true@33:
   LDA   #$01
.cmp_end@34:
   LDY   #$58
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
   ORA   #$00
   BPL   .sx_done@37
   LDA   #$FF
.sx_done@37:
   LDY   #$5C
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$17
   LDA   (FP),Y
   CLC
   LDY   #$5A
   SBC   (FP),Y
   LDY   #$18
   LDA   (FP),Y
   LDY   #$5B
   SBC   (FP),Y
   LDY   #$19
   LDA   (FP),Y
   LDY   #$5C
   SBC   (FP),Y
   LDY   #$1A
   LDA   (FP),Y
   LDY   #$5D
   SBC   (FP),Y
   LDA   #$00
   ROL
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$59
   LDA   (FP),Y
   LDY   #$5F
   CMP   (FP),Y
   BNE   .cmp_true@38
   LDY   #$58
   LDA   (FP),Y
   LDY   #$5E
   CMP   (FP),Y
   BNE   .cmp_true@38
   LDA   #$00
   JMP   .cmp_end@39
.cmp_true@38:
   LDA   #$01
.cmp_end@39:
   LDY   #$60
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$62
   STA   (FP),Y

   ; epilogue
   PHA
   CLC
   LDA   FP
   ADC   #$6B
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

   ; prologue: 3 arg bytes, 84 local bytes
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

   LDA   #$07
   LDY   #$01
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   ORA   #$00
   BPL   .sx_done@42
   LDA   #$FF
.sx_done@42:
   LDY   #$03
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   g2
   LDY   #$01
   AND   (FP),Y
   LDY   #$05
   STA   (FP),Y
   LDA   g2+1
   LDY   #$02
   AND   (FP),Y
   LDY   #$06
   STA   (FP),Y
   LDA   g2+2
   LDY   #$03
   AND   (FP),Y
   LDY   #$07
   STA   (FP),Y
   LDA   g2+3
   LDY   #$04
   AND   (FP),Y
   LDY   #$08
   STA   (FP),Y
   LDA   g1
   STA   HARGS
   LDA   g1+1
   STA   HARGS+1
   LDA   g1+2
   STA   HARGS+2
   LDA   g1+3
   STA   HARGS+3
   LDY   #$05
   LDA   (FP),Y
   STA   HARGS+4
   JSR   asl32
   LDA   HARGS+5
   LDY   #$09
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
   LDY   #$09
   LDA   (FP),Y
   LDY   #$11
   AND   (FP),Y
   LDY   #$15
   STA   (FP),Y
   LDY   #$0A
   LDA   (FP),Y
   LDY   #$12
   AND   (FP),Y
   LDY   #$16
   STA   (FP),Y
   LDY   #$0B
   LDA   (FP),Y
   LDY   #$13
   AND   (FP),Y
   LDY   #$17
   STA   (FP),Y
   LDY   #$0C
   LDA   (FP),Y
   LDY   #$14
   AND   (FP),Y
   LDY   #$18
   STA   (FP),Y
   LDY   #$15
   LDA   (FP),Y
   LDY   #$19
   STA   (FP),Y
   LDY   #$16
   LDA   (FP),Y
   LDY   #$1A
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
   INY
   STA   (FP),Y
   LDY   #$1A
   LDA   (FP),Y
   LDY   #$20
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
.loop@1_start:
   LDY   #$23
   LDA   (FP),Y
   SEC
   SBC   #$03
   INY
   LDA   (FP),Y
   SBC   #$00
   BVC   .cmp_novf@43
   EOR   #$80
.cmp_novf@43:
   ASL
   LDA   #$00
   ROL
   LDY   #$25
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
   LDA   #$02
   LDY   #$27
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   ORA   #$00
   BPL   .sx_done@45
   LDA   #$FF
.sx_done@45:
   LDY   #$29
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   g1
   SEC
   LDY   #$27
   SBC   (FP),Y
   LDY   #$2B
   STA   (FP),Y
   LDA   g1+1
   LDY   #$28
   SBC   (FP),Y
   LDY   #$2C
   STA   (FP),Y
   LDA   g1+2
   LDY   #$29
   SBC   (FP),Y
   LDY   #$2D
   STA   (FP),Y
   LDA   g1+3
   LDY   #$2A
   SBC   (FP),Y
   LDY   #$2E
   STA   (FP),Y
   LDY   #$2B
   LDA   (FP),Y
   LDY   #$2F
   STA   (FP),Y
   CLC
   LDA   FP
   ADC   #$21
   INY
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$03
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
   JSR   f4
   LDY   #$32
   STA   (FP),Y
   TXA
   INY
   STA   (FP),Y
   LDY   #$23
   LDA   (FP),Y
   LDY   #$34
   STA   (FP),Y
   LDY   #$24
   LDA   (FP),Y
   LDY   #$35
   STA   (FP),Y
   LDY   #$32
   LDA   (FP),Y
   CLC
   LDY   #$34
   ADC   (FP),Y
   LDY   #$36
   STA   (FP),Y
   LDY   #$33
   LDA   (FP),Y
   LDY   #$35
   ADC   (FP),Y
   LDY   #$37
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
   LDY   #$19
   LDA   (FP),Y
   CLC
   LDY   #$38
   ADC   (FP),Y
   LDY   #$3C
   STA   (FP),Y
   LDY   #$1A
   LDA   (FP),Y
   LDY   #$39
   ADC   (FP),Y
   LDY   #$3D
   STA   (FP),Y
   LDY   #$1B
   LDA   (FP),Y
   LDY   #$3A
   ADC   (FP),Y
   LDY   #$3E
   STA   (FP),Y
   LDY   #$1C
   LDA   (FP),Y
   LDY   #$3B
   ADC   (FP),Y
   LDY   #$3F
   STA   (FP),Y
   LDY   #$3C
   LDA   (FP),Y
   LDY   #$19
   STA   (FP),Y
   LDY   #$3D
   LDA   (FP),Y
   LDY   #$1A
   STA   (FP),Y
   LDY   #$3E
   LDA   (FP),Y
   LDY   #$1B
   STA   (FP),Y
   LDY   #$3F
   LDA   (FP),Y
   LDY   #$1C
   STA   (FP),Y
   LDY   #$23
   LDA   (FP),Y
   LDY   #$40
   STA   (FP),Y
   LDY   #$24
   LDA   (FP),Y
   LDY   #$41
   STA   (FP),Y
   LDY   #$23
   LDA   (FP),Y
   CLC
   ADC   #$01
   LDY   #$42
   STA   (FP),Y
   LDY   #$24
   LDA   (FP),Y
   ADC   #$00
   LDY   #$43
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$23
   STA   (FP),Y
   LDY   #$43
   LDA   (FP),Y
   LDY   #$24
   STA   (FP),Y
   JMP   .loop@1_start
.loop@1_break:
   LDY   #$21
   LDA   (FP),Y
   LDY   #$44
   STA   (FP),Y
   LDY   #$22
   LDA   (FP),Y
   LDY   #$45
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@46
   LDA   #$00
   JMP   .sx_done@47
.sx_neg@46:
   LDA   #$FF
.sx_done@47:
   LDY   #$46
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$19
   LDA   (FP),Y
   CLC
   LDY   #$44
   ADC   (FP),Y
   LDY   #$48
   STA   (FP),Y
   LDY   #$1A
   LDA   (FP),Y
   LDY   #$45
   ADC   (FP),Y
   LDY   #$49
   STA   (FP),Y
   LDY   #$1B
   LDA   (FP),Y
   LDY   #$46
   ADC   (FP),Y
   LDY   #$4A
   STA   (FP),Y
   LDY   #$1C
   LDA   (FP),Y
   LDY   #$47
   ADC   (FP),Y
   LDY   #$4B
   STA   (FP),Y
   LDY   #$48
   LDA   (FP),Y
   LDY   #$19
   STA   (FP),Y
   LDY   #$49
   LDA   (FP),Y
   LDY   #$1A
   STA   (FP),Y
   LDY   #$4A
   LDA   (FP),Y
   LDY   #$1B
   STA   (FP),Y
   LDY   #$4B
   LDA   (FP),Y
   LDY   #$1C
   STA   (FP),Y
   LDY   #$59
   LDA   (FP),Y
   LDY   #$4C
   STA   (FP),Y
   LDA   #$00
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
   LDY   #$57
   LDA   (FP),Y
   CLC
   LDY   #$4E
   SBC   (FP),Y
   LDY   #$58
   LDA   (FP),Y
   LDY   #$4F
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
   SEC
   SBC   g0
   INY
   LDA   (FP),Y
   SBC   g0+1
   BVC   .cmp_novf@48
   EOR   #$80
.cmp_novf@48:
   ASL
   LDA   #$00
   ROL
   EOR   #$01
   LDY   #$52
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$54
   STA   (FP),Y

   ; epilogue
   PHA
   CLC
   LDA   FP
   ADC   #$59
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

   ; prologue: 0 arg bytes, 31 local bytes
   SEC
   LDA   SSP
   SBC   #$21
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$20
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
   BPL   .sx_done@50
   LDA   #$FF
.sx_done@50:
   LDY   #$03
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   g2
   LDY   #$01
   AND   (FP),Y
   LDY   #$05
   STA   (FP),Y
   LDA   g2+1
   LDY   #$02
   AND   (FP),Y
   LDY   #$06
   STA   (FP),Y
   LDA   g2+2
   LDY   #$03
   AND   (FP),Y
   LDY   #$07
   STA   (FP),Y
   LDA   g2+3
   LDY   #$04
   AND   (FP),Y
   LDY   #$08
   STA   (FP),Y
   LDA   g0
   INY
   STA   (FP),Y
   LDA   g0+1
   INY
   STA   (FP),Y
   LDY   #$05
   LDA   (FP),Y
   TAX
   BEQ   .shift_done@52
.shift_loop@51:
   LDY   #$0A
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
   BNE   .shift_loop@51
.shift_done@52:
   LDY   #$09
   LDA   (FP),Y
   LDY   #$0B
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$0C
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@53
   LDA   #$00
   JMP   .sx_done@54
.sx_neg@53:
   LDA   #$FF
.sx_done@54:
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
   LDA   g1+3
   CMP   (FP),Y
   BNE   .cmp_true@55
   LDA   g1+2
   DEY
   CMP   (FP),Y
   BNE   .cmp_true@55
   LDA   g1+1
   DEY
   CMP   (FP),Y
   BNE   .cmp_true@55
   LDA   g1
   DEY
   CMP   (FP),Y
   BNE   .cmp_true@55
   LDA   #$00
   JMP   .cmp_end@56
.cmp_true@55:
   LDA   #$01
.cmp_end@56:
   LDY   #$13
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   ORA   #$00
   BPL   .sx_done@59
   LDA   #$FF
.sx_done@59:
   LDY   #$17
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$15
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
   BVC   .cmp_novf@60
   EOR   #$80
.cmp_novf@60:
   ASL
   LDA   #$00
   ROL
   EOR   #$01
   LDY   #$19
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ORA   #$01
   LDY   #$1B
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$1C
   STA   (FP),Y
   LDY   #$13
   LDA   (FP),Y
   STA   HARGS
   INY
   LDA   (FP),Y
   STA   HARGS+1
   LDY   #$1B
   LDA   (FP),Y
   STA   HARGS+2
   INY
   LDA   (FP),Y
   STA   HARGS+3
   JSR   sdivmod16
   LDA   HARGS+4
   LDY   #$1D
   STA   (FP),Y
   LDA   HARGS+5
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$1F
   STA   (FP),Y

   ; epilogue
   PHA
   CLC
   LDA   FP
   ADC   #$21
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

   ; prologue: 0 arg bytes, 39 local bytes
   SEC
   LDA   SSP
   SBC   #$29
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$28
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
   BMI   .sx_neg@61
   LDA   #$00
   JMP   .sx_done@62
.sx_neg@61:
   LDA   #$FF
.sx_done@62:
   LDY   #$0C
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
   LDY   #$12
   LDA   (FP),Y
   EOR   g2
   LDY   #$16
   STA   (FP),Y
   LDY   #$13
   LDA   (FP),Y
   EOR   g2+1
   LDY   #$17
   STA   (FP),Y
   LDY   #$14
   LDA   (FP),Y
   EOR   g2+2
   LDY   #$18
   STA   (FP),Y
   LDY   #$15
   LDA   (FP),Y
   EOR   g2+3
   LDY   #$19
   STA   (FP),Y
   LDY   #$06
   LDA   (FP),Y
   LDY   #$16
   EOR   (FP),Y
   LDY   #$1A
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
   LDY   #$1A
   LDA   (FP),Y
   LDY   #$06
   STA   (FP),Y
   LDY   #$1B
   LDA   (FP),Y
   LDY   #$07
   STA   (FP),Y
   LDY   #$1C
   LDA   (FP),Y
   LDY   #$08
   STA   (FP),Y
   LDY   #$1D
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
   LDY   #$1E
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
   LDY   #$1E
   EOR   (FP),Y
   LDY   #$22
   STA   (FP),Y
   LDY   #$07
   LDA   (FP),Y
   LDY   #$1F
   EOR   (FP),Y
   LDY   #$23
   STA   (FP),Y
   LDY   #$08
   LDA   (FP),Y
   LDY   #$20
   EOR   (FP),Y
   LDY   #$24
   STA   (FP),Y
   LDY   #$09
   LDA   (FP),Y
   LDY   #$21
   EOR   (FP),Y
   LDY   #$25
   STA   (FP),Y
   LDY   #$22
   LDA   (FP),Y
   LDY   #$26
   STA   (FP),Y
   LDY   #$23
   LDA   (FP),Y
   LDY   #$27
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
   ADC   #$29
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$28
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
   DC.W  $00B0

g1:
   DC.L  $000000A6

g2:
   DC.L  $00000007
