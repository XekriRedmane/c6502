SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24

check:
   SUBROUTINE

   ; prologue: 0 arg bytes, 126 local bytes
   SEC
   LDA   SSP
   SBC   #$80
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$7F
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   LDA   #$00
   LDY   #$01
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   v0
   INY
   STA   (FP),Y
   LDA   v0+1
   INY
   STA   (FP),Y
   LDA   v0+2
   INY
   STA   (FP),Y
   LDA   v0+3
   INY
   STA   (FP),Y
   CMP   v2+3
   BNE   .cmp_true@0
   DEY
   LDA   (FP),Y
   CMP   v2+2
   BNE   .cmp_true@0
   DEY
   LDA   (FP),Y
   CMP   v2+1
   BNE   .cmp_true@0
   DEY
   LDA   (FP),Y
   CMP   v2
   BNE   .cmp_true@0
   LDA   #$00
   JMP   .cmp_end@1
.cmp_true@0:
   LDA   #$01
.cmp_end@1:
   LDY   #$07
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDA   v2
   CLC
   SBC   v5
   LDA   v2+1
   SBC   v5+1
   LDA   v2+2
   SBC   v5+2
   LDA   v2+3
   SBC   v5+3
   LDA   #$00
   ROL
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$07
   LDA   (FP),Y
   SEC
   LDY   #$09
   SBC   (FP),Y
   LDY   #$0B
   STA   (FP),Y
   LDY   #$08
   LDA   (FP),Y
   LDY   #$0A
   SBC   (FP),Y
   LDY   #$0C
   STA   (FP),Y
   LDA   v3
   INY
   STA   (FP),Y
   LDA   v3+1
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$0D
   LDA   (FP),Y
   AND   v2
   LDY   #$11
   STA   (FP),Y
   LDY   #$0E
   LDA   (FP),Y
   AND   v2+1
   LDY   #$12
   STA   (FP),Y
   LDY   #$0F
   LDA   (FP),Y
   AND   v2+2
   LDY   #$13
   STA   (FP),Y
   LDY   #$10
   LDA   (FP),Y
   AND   v2+3
   LDY   #$14
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CMP   #$01
   BNE   .cmp_true@3
   DEY
   LDA   (FP),Y
   CMP   #$23
   BNE   .cmp_true@3
   DEY
   LDA   (FP),Y
   CMP   #$45
   BNE   .cmp_true@3
   LDA   #$00
   JMP   .cmp_end@4
.cmp_true@3:
   LDA   #$01
.cmp_end@4:
   LDY   #$15
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$0B
   LDA   (FP),Y
   LDY   #$15
   ORA   (FP),Y
   LDY   #$17
   STA   (FP),Y
   LDY   #$0C
   LDA   (FP),Y
   LDY   #$16
   ORA   (FP),Y
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
   CMP   #$00
   BNE   .cmp_true@6
   DEY
   LDA   (FP),Y
   CMP   #$01
   BNE   .cmp_true@6
   LDA   #$00
   JMP   .cmp_end@7
.cmp_true@6:
   LDA   #$01
.cmp_end@7:
   LDY   #$1B
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@0
   LDY   #$01
   LDA   (FP),Y
   ORA   #$01
   LDY   #$1D
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   LDY   #$1E
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   LDY   #$1E
   LDA   (FP),Y
   LDY   #$02
   STA   (FP),Y
.if_end@0:
   SEC
   LDA   #$00
   SBC   v5
   LDY   #$1F
   STA   (FP),Y
   LDA   #$00
   SBC   v5+1
   INY
   STA   (FP),Y
   LDA   #$00
   SBC   v5+2
   INY
   STA   (FP),Y
   LDA   #$00
   SBC   v5+3
   INY
   STA   (FP),Y
   LDA   #$BB
   LDY   #$1F
   ORA   (FP),Y
   LDY   #$23
   STA   (FP),Y
   LDA   #$9A
   LDY   #$20
   ORA   (FP),Y
   LDY   #$24
   STA   (FP),Y
   LDA   #$30
   LDY   #$21
   ORA   (FP),Y
   LDY   #$25
   STA   (FP),Y
   LDA   #$9B
   LDY   #$22
   ORA   (FP),Y
   LDY   #$26
   STA   (FP),Y
   LDY   #$23
   LDA   (FP),Y
   LDY   #$27
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDA   #$BB
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$29
   LDA   (FP),Y
   LDY   #$2C
   CMP   (FP),Y
   BNE   .cmp_true@9
   LDA   #$00
   JMP   .cmp_end@10
.cmp_true@9:
   LDA   #$01
.cmp_end@10:
   LDY   #$2E
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@1
   LDY   #$01
   LDA   (FP),Y
   ORA   #$02
   LDY   #$30
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   LDY   #$31
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   LDY   #$31
   LDA   (FP),Y
   LDY   #$02
   STA   (FP),Y
.if_end@1:
   LDA   v4
   LDY   #$32
   STA   (FP),Y
   LDA   v4+1
   INY
   STA   (FP),Y
   LDA   v4+2
   INY
   STA   (FP),Y
   LDA   v4+3
   INY
   STA   (FP),Y
   CMP   #$05
   BNE   .cmp_true@11
   DEY
   LDA   (FP),Y
   CMP   #$39
   BNE   .cmp_true@11
   DEY
   LDA   (FP),Y
   CMP   #$22
   BNE   .cmp_true@11
   DEY
   LDA   (FP),Y
   CMP   #$40
   BNE   .cmp_true@11
   LDA   #$00
   JMP   .cmp_end@12
.cmp_true@11:
   LDA   #$01
.cmp_end@12:
   LDY   #$36
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .vthread@0
   LDY   #$01
   LDA   (FP),Y
   ORA   #$04
   LDY   #$38
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   LDY   #$39
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   LDY   #$39
   LDA   (FP),Y
   LDY   #$02
   STA   (FP),Y
   LDA   #$FF
   BEQ   .lnot_true@14
.vthread@0:
   LDA   #$00
   JMP   .lnot_end@15
.lnot_true@14:
   LDA   #$01
.lnot_end@15:
   LDY   #$3A
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@0
   JMP   .and_false@3
.lb_skip@0:
   LDA   #$45
   LDY   #$3C
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
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
   BMI   .sx_neg@16
   LDA   #$00
   JMP   .sx_done@17
.sx_neg@16:
   LDA   #$FF
.sx_done@17:
   LDY   #$41
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$3F
   LDA   (FP),Y
   CLC
   SBC   v5
   INY
   LDA   (FP),Y
   SBC   v5+1
   INY
   LDA   (FP),Y
   SBC   v5+2
   INY
   LDA   (FP),Y
   SBC   v5+3
   LDA   #$00
   ROL
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDA   v1
   INY
   STA   (FP),Y
   LDA   v1+1
   INY
   STA   (FP),Y
   LDA   v1+1
   BMI   .sx_neg@18
   LDA   #$00
   JMP   .sx_done@19
.sx_neg@18:
   LDA   #$FF
.sx_done@19:
   LDY   #$47
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   v2
   CLC
   LDY   #$45
   SBC   (FP),Y
   LDA   v2+1
   INY
   SBC   (FP),Y
   LDA   v2+2
   INY
   SBC   (FP),Y
   LDA   v2+3
   INY
   SBC   (FP),Y
   LDA   #$00
   ROL
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$43
   LDA   (FP),Y
   LDY   #$4B
   STA   (FP),Y
   LDY   #$44
   LDA   (FP),Y
   LDY   #$4C
   STA   (FP),Y
   LDY   #$49
   LDA   (FP),Y
   TAX
   BEQ   .shift_done@21
.shift_loop@20:
   LDY   #$4C
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
   BNE   .shift_loop@20
.shift_done@21:
   LDY   #$4B
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@3
   LDA   #$01
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   JMP   .and_end@4
.and_false@3:
   LDA   #$00
   LDY   #$4D
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@4:
   LDY   #$4D
   LDA   (FP),Y
   LDY   #$4F
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$50
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@22
   LDA   #$00
   JMP   .cmp_end@23
.cmp_true@22:
   LDA   #$01
.cmp_end@23:
   LDY   #$51
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@5
   LDY   #$01
   LDA   (FP),Y
   ORA   #$08
   LDY   #$53
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   LDY   #$54
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   LDY   #$54
   LDA   (FP),Y
   LDY   #$02
   STA   (FP),Y
.if_end@5:
   LDA   v1
   LDY   #$55
   STA   (FP),Y
   LDA   v1+1
   INY
   STA   (FP),Y
   CMP   #$6E
   BNE   .cmp_true@24
   DEY
   LDA   (FP),Y
   CMP   #$42
   BNE   .cmp_true@24
   LDA   #$00
   JMP   .cmp_end@25
.cmp_true@24:
   LDA   #$01
.cmp_end@25:
   LDY   #$57
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@6
   LDY   #$01
   LDA   (FP),Y
   ORA   #$10
   LDY   #$59
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   LDY   #$5A
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   LDY   #$5A
   LDA   (FP),Y
   LDY   #$02
   STA   (FP),Y
.if_end@6:
   LDA   v5
   LDY   #$5B
   STA   (FP),Y
   LDA   v5+1
   INY
   STA   (FP),Y
   LDA   v5+2
   INY
   STA   (FP),Y
   LDA   v5+3
   INY
   STA   (FP),Y
   CMP   #$FF
   BNE   .cmp_true@27
   DEY
   LDA   (FP),Y
   CMP   #$FF
   BNE   .cmp_true@27
   DEY
   LDA   (FP),Y
   CMP   #$FF
   BNE   .cmp_true@27
   DEY
   LDA   (FP),Y
   CMP   #$FF
   BNE   .cmp_true@27
   LDA   #$00
   JMP   .cmp_end@28
.cmp_true@27:
   LDA   #$01
.cmp_end@28:
   LDY   #$5F
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@7
   LDY   #$01
   LDA   (FP),Y
   ORA   #$20
   LDY   #$61
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   LDY   #$62
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   LDY   #$62
   LDA   (FP),Y
   LDY   #$02
   STA   (FP),Y
.if_end@7:
   LDA   v3
   LDY   #$63
   STA   (FP),Y
   LDA   v3+1
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$63
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
   LDA   v4
   STA   HARGS+4
   LDA   v4+1
   STA   HARGS+5
   LDA   v4+2
   STA   HARGS+6
   LDA   v4+3
   STA   HARGS+7
   JSR   sdivmod32
   LDA   HARGS+12
   LDY   #$67
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
   LDY   #$67
   LDA   (FP),Y
   EOR   #$FF
   LDY   #$6B
   STA   (FP),Y
   LDY   #$68
   LDA   (FP),Y
   EOR   #$FF
   LDY   #$6C
   STA   (FP),Y
   LDY   #$69
   LDA   (FP),Y
   EOR   #$FF
   LDY   #$6D
   STA   (FP),Y
   LDY   #$6A
   LDA   (FP),Y
   EOR   #$FF
   LDY   #$6E
   STA   (FP),Y
   LDY   #$6B
   LDA   (FP),Y
   LDY   #$6F
   STA   (FP),Y
   LDY   #$6C
   LDA   (FP),Y
   LDY   #$70
   STA   (FP),Y
   LDY   #$6D
   LDA   (FP),Y
   LDY   #$71
   STA   (FP),Y
   LDY   #$6E
   LDA   (FP),Y
   LDY   #$72
   STA   (FP),Y
   CMP   #$FF
   BNE   .cmp_true@30
   DEY
   LDA   (FP),Y
   CMP   #$FF
   BNE   .cmp_true@30
   DEY
   LDA   (FP),Y
   CMP   #$7F
   BNE   .cmp_true@30
   DEY
   LDA   (FP),Y
   CMP   #$FF
   BNE   .cmp_true@30
   LDA   #$00
   JMP   .cmp_end@31
.cmp_true@30:
   LDA   #$01
.cmp_end@31:
   LDY   #$73
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@8
   LDY   #$01
   LDA   (FP),Y
   ORA   #$40
   LDY   #$75
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   LDY   #$76
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   LDY   #$76
   LDA   (FP),Y
   LDY   #$02
   STA   (FP),Y
.if_end@8:
   LDA   v0
   LDY   #$77
   STA   (FP),Y
   LDA   v0+1
   INY
   STA   (FP),Y
   LDA   v0+2
   INY
   STA   (FP),Y
   LDA   v0+3
   INY
   STA   (FP),Y
   CMP   #$80
   BNE   .cmp_true@33
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@33
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@33
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@33
   LDA   #$00
   JMP   .cmp_end@34
.cmp_true@33:
   LDA   #$01
.cmp_end@34:
   LDY   #$7B
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@9
   LDY   #$01
   LDA   (FP),Y
   ORA   #$80
   LDY   #$7D
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   LDY   #$7E
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   LDY   #$7E
   LDA   (FP),Y
   LDY   #$02
   STA   (FP),Y
.if_end@9:
   LDY   #$02
   LDA   (FP),Y
   TAX
   DEY
   LDA   (FP),Y

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$80
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$7F
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

   ; prologue: 0 arg bytes, 2 local bytes
   SEC
   LDA   SSP
   SBC   #$04
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$03
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   JSR   check
   LDY   #$01
   STA   (FP),Y
   TXA
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
   ADC   #$04
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$03
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

v0:
   DC.L  $80000000

v1:
   DC.W  $6E42

v2:
   DC.L  $80000000

v3:
   DC.W  $8000

v4:
   DC.L  $05392240

v5:
   DC.L  $FFFFFFFF
