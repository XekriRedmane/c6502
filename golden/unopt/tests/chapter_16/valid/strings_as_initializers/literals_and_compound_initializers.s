SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24

main:
   SUBROUTINE

   ; prologue: 0 arg bytes, 96 local bytes
   SEC
   LDA   SSP
   SBC   #$62
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$61
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   CLC
   LDA   FP
   ADC   #$01
   LDY   #$07
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDA   #$6C
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$07
   LDA   (FP),Y
   CLC
   ADC   #$01
   LDY   #$09
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$0A
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDA   #$6D
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$07
   LDA   (FP),Y
   CLC
   ADC   #$02
   LDY   #$0B
   STA   (FP),Y
   LDY   #$08
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
   LDA   #$6E
   LDY   #$00
   STA   (DPTR),Y
   LDA   #$6F
   LDY   #$0D
   STA   (FP),Y
   LDY   #$07
   LDA   (FP),Y
   CLC
   ADC   #$03
   LDY   #$0E
   STA   (FP),Y
   LDY   #$08
   LDA   (FP),Y
   ADC   #$00
   LDY   #$0F
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$0D
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDA   #$70
   LDY   #$10
   STA   (FP),Y
   LDY   #$07
   LDA   (FP),Y
   CLC
   ADC   #$04
   LDY   #$11
   STA   (FP),Y
   LDY   #$08
   LDA   (FP),Y
   ADC   #$00
   LDY   #$12
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$10
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$07
   LDA   (FP),Y
   CLC
   ADC   #$05
   LDY   #$13
   STA   (FP),Y
   LDY   #$08
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
   LDA   #$00
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$15
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
   BVC   .cmp_novf@0
   EOR   #$80
.cmp_novf@0:
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
   BNE   .lb_skip@3
   JMP   .loop@0_break
.lb_skip@3:
   LDA   #$00
   LDY   #$19
   STA   (FP),Y
   INY
   STA   (FP),Y
.loop@1_start:
   LDY   #$19
   LDA   (FP),Y
   SEC
   SBC   #$04
   INY
   LDA   (FP),Y
   SBC   #$00
   BVC   .cmp_novf@1
   EOR   #$80
.cmp_novf@1:
   ASL
   LDA   #$00
   ROL
   LDY   #$1B
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@2
   JMP   .loop@1_break
.lb_skip@2:
   LDA   #<static_array
   LDY   #$1D
   STA   (FP),Y
   LDA   #>static_array
   INY
   STA   (FP),Y
   LDY   #$15
   LDA   (FP),Y
   LDY   #$1F
   STA   (FP),Y
   STA   HARGS
   LDA   #$04
   STA   HARGS+1
   JSR   mul8
   LDA   HARGS+2
   LDY   #$20
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$1D
   LDA   (FP),Y
   CLC
   LDY   #$21
   ADC   (FP),Y
   LDY   #$23
   STA   (FP),Y
   LDY   #$1E
   LDA   (FP),Y
   LDY   #$22
   ADC   (FP),Y
   LDY   #$24
   STA   (FP),Y
   LDY   #$19
   LDA   (FP),Y
   LDY   #$25
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$23
   LDA   (FP),Y
   CLC
   LDY   #$26
   ADC   (FP),Y
   LDY   #$28
   STA   (FP),Y
   LDY   #$24
   LDA   (FP),Y
   LDY   #$27
   ADC   (FP),Y
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
   INY
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@2
   LDA   #$00
   JMP   .sx_done@3
.sx_neg@2:
   LDA   #$FF
.sx_done@3:
   LDY   #$2C
   STA   (FP),Y
   LDY   #$15
   LDA   (FP),Y
   STA   HARGS
   INY
   LDA   (FP),Y
   STA   HARGS+1
   LDA   #$04
   STA   HARGS+2
   LDA   #$00
   STA   HARGS+3
   JSR   mul16
   LDA   HARGS+4
   LDY   #$2D
   STA   (FP),Y
   LDA   HARGS+5
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   LDY   #$19
   ADC   (FP),Y
   LDY   #$2F
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$1A
   ADC   (FP),Y
   LDY   #$30
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$31
   STA   (FP),Y
   TAX
   LDA   str@0,X
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$2C
   LDA   (FP),Y
   LDY   #$34
   CMP   (FP),Y
   BNE   .cmp_true@4
   LDY   #$2B
   LDA   (FP),Y
   LDY   #$33
   CMP   (FP),Y
   BNE   .cmp_true@4
   LDA   #$00
   JMP   .cmp_end@5
.cmp_true@4:
   LDA   #$01
.cmp_end@5:
   LDY   #$35
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@0
   LDX   #$00
   LDA   #$01

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$62
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$61
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
   LDY   #$19
   LDA   (FP),Y
   CLC
   ADC   #$01
   LDY   #$37
   STA   (FP),Y
   LDY   #$1A
   LDA   (FP),Y
   ADC   #$00
   LDY   #$38
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$19
   STA   (FP),Y
   LDY   #$38
   LDA   (FP),Y
   LDY   #$1A
   STA   (FP),Y
   JMP   .loop@1_start
.loop@1_break:
   LDY   #$15
   LDA   (FP),Y
   CLC
   ADC   #$01
   LDY   #$39
   STA   (FP),Y
   LDY   #$16
   LDA   (FP),Y
   ADC   #$00
   LDY   #$3A
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$15
   STA   (FP),Y
   LDY   #$3A
   LDA   (FP),Y
   LDY   #$16
   STA   (FP),Y
   JMP   .loop@0_start
.loop@0_break:
   LDA   #$00
   LDY   #$3B
   STA   (FP),Y
   INY
   STA   (FP),Y
.loop@2_start:
   LDY   #$3B
   LDA   (FP),Y
   SEC
   SBC   #$02
   INY
   LDA   (FP),Y
   SBC   #$00
   BVC   .cmp_novf@7
   EOR   #$80
.cmp_novf@7:
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
   BNE   .lb_skip@1
   JMP   .loop@2_break
.lb_skip@1:
   LDA   #$00
   LDY   #$3F
   STA   (FP),Y
   INY
   STA   (FP),Y
.loop@3_start:
   LDY   #$3F
   LDA   (FP),Y
   SEC
   SBC   #$03
   INY
   LDA   (FP),Y
   SBC   #$00
   BVC   .cmp_novf@8
   EOR   #$80
.cmp_novf@8:
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
   INY
   ORA   (FP),Y
   BNE   .lb_skip@0
   JMP   .loop@3_break
.lb_skip@0:
   CLC
   LDA   FP
   ADC   #$01
   LDY   #$47
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   LDY   #$3B
   LDA   (FP),Y
   LDY   #$43
   STA   (FP),Y
   STA   HARGS
   LDA   #$03
   STA   HARGS+1
   JSR   mul8
   LDA   HARGS+2
   LDY   #$44
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   LDA   (FP),Y
   CLC
   LDY   #$45
   ADC   (FP),Y
   LDY   #$49
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$46
   ADC   (FP),Y
   LDY   #$4A
   STA   (FP),Y
   LDY   #$3F
   LDA   (FP),Y
   LDY   #$4B
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$49
   LDA   (FP),Y
   CLC
   LDY   #$4C
   ADC   (FP),Y
   LDY   #$4E
   STA   (FP),Y
   LDY   #$4A
   LDA   (FP),Y
   LDY   #$4D
   ADC   (FP),Y
   LDY   #$4F
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$50
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$3B
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
   LDY   #$53
   STA   (FP),Y
   LDA   HARGS+5
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   LDY   #$3F
   ADC   (FP),Y
   LDY   #$55
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$40
   ADC   (FP),Y
   LDY   #$56
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$57
   STA   (FP),Y
   TAX
   LDA   str@1,X
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$51
   LDA   (FP),Y
   LDY   #$59
   CMP   (FP),Y
   BNE   .cmp_true@9
   LDA   #$00
   JMP   .cmp_end@10
.cmp_true@9:
   LDA   #$01
.cmp_end@10:
   LDY   #$5B
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@1
   LDX   #$00
   LDA   #$02

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$62
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$61
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
.if_end@1:
   LDY   #$3F
   LDA   (FP),Y
   CLC
   ADC   #$01
   LDY   #$5D
   STA   (FP),Y
   LDY   #$40
   LDA   (FP),Y
   ADC   #$00
   LDY   #$5E
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$3F
   STA   (FP),Y
   LDY   #$5E
   LDA   (FP),Y
   LDY   #$40
   STA   (FP),Y
   JMP   .loop@3_start
.loop@3_break:
   LDY   #$3B
   LDA   (FP),Y
   CLC
   ADC   #$01
   LDY   #$5F
   STA   (FP),Y
   LDY   #$3C
   LDA   (FP),Y
   ADC   #$00
   LDY   #$60
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$3B
   STA   (FP),Y
   LDY   #$60
   LDA   (FP),Y
   LDY   #$3C
   STA   (FP),Y
   JMP   .loop@2_start
.loop@2_break:
   LDX   #$00
   TXA

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$62
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$61
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

str@0:
   DC.B  $61, $62, $63, $64, $65, $66, $67, $68, $69, $6A, $6B, $00

str@1:
   DC.B  $6C, $6D, $6E, $6F, $70, $00

static_array:
   DC.B  $61, $62, $63, $64
   DC.B  $65, $66, $67, $68
   DC.B  $69, $6A, $6B, $00
