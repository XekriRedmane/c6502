SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24

main:
   SUBROUTINE

   ; prologue: 0 arg bytes, 40 local bytes
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

   LDA   #$25
   LDY   #$01
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$04
   LDY   #$03
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$04
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   HARGS
   INY
   LDA   (FP),Y
   STA   HARGS+1
   LDA   #$05
   STA   HARGS+2
   LDA   #$00
   STA   HARGS+3
   JSR   sdivmod16
   LDA   HARGS+4
   LDY   #$05
   STA   (FP),Y
   LDA   HARGS+5
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
   LDY   #$01
   LDA   (FP),Y
   STA   HARGS
   INY
   LDA   (FP),Y
   STA   HARGS+1
   LDA   #$05
   STA   HARGS+2
   LDA   #$00
   STA   HARGS+3
   JSR   sdivmod16
   LDA   HARGS+6
   LDY   #$09
   STA   (FP),Y
   LDA   HARGS+7
   INY
   STA   (FP),Y
   CMP   #$00
   BNE   .thread@0
   DEY
   LDA   (FP),Y
   BEQ   .cmp_true@0
.thread@0:
   LDA   #$00
   JMP   .cmp_end@1
.cmp_true@0:
   LDA   #$01
.cmp_end@1:
   LDY   #$0B
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@5
   JMP   .case@1
.lb_skip@5:
   LDY   #$0A
   LDA   (FP),Y
   BNE   .thread@1
   DEY
   LDA   (FP),Y
   CMP   #$04
   BEQ   .cmp_true@3
.thread@1:
   LDA   #$00
   JMP   .cmp_end@4
.cmp_true@3:
   LDA   #$01
.cmp_end@4:
   LDY   #$0D
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@4
   JMP   .case@3
.lb_skip@4:
   LDY   #$0A
   LDA   (FP),Y
   BNE   .thread@2
   DEY
   LDA   (FP),Y
   CMP   #$03
   BEQ   .cmp_true@6
.thread@2:
   LDA   #$00
   JMP   .cmp_end@7
.cmp_true@6:
   LDA   #$01
.cmp_end@7:
   LDY   #$0F
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@3
   JMP   .case@4
.lb_skip@3:
   LDY   #$0A
   LDA   (FP),Y
   BNE   .thread@3
   DEY
   LDA   (FP),Y
   CMP   #$02
   BEQ   .cmp_true@9
.thread@3:
   LDA   #$00
   JMP   .cmp_end@10
.cmp_true@9:
   LDA   #$01
.cmp_end@10:
   LDY   #$11
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@2
   JMP   .case@5
.lb_skip@2:
   LDY   #$0A
   LDA   (FP),Y
   BNE   .thread@4
   DEY
   LDA   (FP),Y
   CMP   #$01
   BEQ   .cmp_true@12
.thread@4:
   LDA   #$00
   JMP   .cmp_end@13
.cmp_true@12:
   LDA   #$01
.cmp_end@13:
   LDY   #$13
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@1
   JMP   .case@6
.lb_skip@1:
   JMP   .switch@0_break
.case@1:
.loop@2_start:
   LDY   #$01
   LDA   (FP),Y
   SEC
   SBC   #$01
   LDY   #$15
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   SBC   #$00
   LDY   #$16
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   LDY   #$16
   LDA   (FP),Y
   LDY   #$02
   STA   (FP),Y
.case@3:
   LDY   #$01
   LDA   (FP),Y
   SEC
   SBC   #$01
   LDY   #$17
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   SBC   #$00
   LDY   #$18
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   LDY   #$18
   LDA   (FP),Y
   LDY   #$02
   STA   (FP),Y
.case@4:
   LDY   #$01
   LDA   (FP),Y
   SEC
   SBC   #$01
   LDY   #$19
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   SBC   #$00
   LDY   #$1A
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   LDY   #$1A
   LDA   (FP),Y
   LDY   #$02
   STA   (FP),Y
.case@5:
   LDY   #$01
   LDA   (FP),Y
   SEC
   SBC   #$01
   LDY   #$1B
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   SBC   #$00
   LDY   #$1C
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   LDY   #$1C
   LDA   (FP),Y
   LDY   #$02
   STA   (FP),Y
.case@6:
   LDY   #$01
   LDA   (FP),Y
   SEC
   SBC   #$01
   LDY   #$1D
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   SBC   #$00
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
   LDY   #$07
   LDA   (FP),Y
   SEC
   SBC   #$01
   LDY   #$1F
   STA   (FP),Y
   LDY   #$08
   LDA   (FP),Y
   SBC   #$00
   LDY   #$20
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$07
   STA   (FP),Y
   LDY   #$20
   LDA   (FP),Y
   LDY   #$08
   STA   (FP),Y
   LDA   #$00
   SEC
   DEY
   SBC   (FP),Y
   LDA   #$00
   INY
   SBC   (FP),Y
   BVC   .cmp_novf@15
   EOR   #$80
.cmp_novf@15:
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
   BEQ   .lb_skip@0
   JMP   .loop@2_start
.lb_skip@0:
.switch@0_break:
   LDY   #$02
   LDA   (FP),Y
   BNE   .thread@5
   DEY
   LDA   (FP),Y
   BEQ   .cmp_true@16
.thread@5:
   LDA   #$00
   JMP   .cmp_end@17
.cmp_true@16:
   LDA   #$01
.cmp_end@17:
   LDY   #$23
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@0
   LDY   #$08
   LDA   (FP),Y
   BNE   .thread@6
   DEY
   LDA   (FP),Y
   BEQ   .cmp_true@19
.thread@6:
   LDA   #$00
   JMP   .cmp_end@20
.cmp_true@19:
   LDA   #$01
.cmp_end@20:
   LDY   #$25
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
   LDY   #$27
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@1:
   TAX
   LDY   #$27
   LDA   (FP),Y

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$2A
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
   TAX
   PLA
   RTS
