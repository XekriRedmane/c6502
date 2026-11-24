SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24

target:
   SUBROUTINE

   ; prologue: 2 arg bytes, 60 local bytes
   SEC
   LDA   SSP
   SBC   #$3E
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$3D
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   LDA   #$03
   LDY   #$01
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$3F
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@0
   LDY   #$01
   LDA   (FP),Y
   LDY   #$03
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$04
   STA   (FP),Y
   LDY   #$01
   LDA   (FP),Y
   CLC
   ADC   #$01
   LDY   #$05
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   ADC   #$00
   LDY   #$06
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   LDY   #$06
   LDA   (FP),Y
   LDY   #$02
   STA   (FP),Y
.if_end@0:
   LDA   #$0A
   LDY   #$07
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$3F
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@1
   LDY   #$07
   LDA   (FP),Y
   LDY   #$09
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$0A
   STA   (FP),Y
   LDY   #$07
   LDA   (FP),Y
   SEC
   SBC   #$01
   LDY   #$0B
   STA   (FP),Y
   LDY   #$08
   LDA   (FP),Y
   SBC   #$00
   LDY   #$0C
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$07
   STA   (FP),Y
   LDY   #$0C
   LDA   (FP),Y
   LDY   #$08
   STA   (FP),Y
.if_end@1:
   SEC
   LDA   #$00
   SBC   #$0C
   LDY   #$0D
   STA   (FP),Y
   LDA   #$00
   SBC   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$0F
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$10
   STA   (FP),Y
   LDY   #$3F
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@2
   LDY   #$0F
   LDA   (FP),Y
   CLC
   ADC   #$01
   LDY   #$11
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$12
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$0F
   STA   (FP),Y
   LDY   #$12
   LDA   (FP),Y
   LDY   #$10
   STA   (FP),Y
.if_end@2:
   SEC
   LDA   #$00
   SBC   #$64
   LDY   #$13
   STA   (FP),Y
   LDA   #$00
   SBC   #$00
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
   LDY   #$3F
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@3
   LDY   #$15
   LDA   (FP),Y
   SEC
   SBC   #$01
   LDY   #$17
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   SBC   #$00
   LDY   #$18
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$15
   STA   (FP),Y
   LDY   #$18
   LDA   (FP),Y
   LDY   #$16
   STA   (FP),Y
.if_end@3:
   LDY   #$3F
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@0
   JMP   .if_else@5
.lb_skip@0:
   LDY   #$02
   LDA   (FP),Y
   BNE   .thread@0
   DEY
   LDA   (FP),Y
   CMP   #$04
   BEQ   .cmp_true@0
.thread@0:
   LDA   #$00
   JMP   .cmp_end@1
.cmp_true@0:
   LDA   #$01
.cmp_end@1:
   LDY   #$19
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@10
   LDY   #$08
   LDA   (FP),Y
   BNE   .thread@1
   DEY
   LDA   (FP),Y
   CMP   #$09
   BEQ   .cmp_true@3
.thread@1:
   LDA   #$00
   JMP   .cmp_end@4
.cmp_true@3:
   LDA   #$01
.cmp_end@4:
   LDY   #$1B
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
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
   LDY   #$1D
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@11:
   LDY   #$1D
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@8
   SEC
   LDA   #$00
   SBC   #$0B
   INY
   STA   (FP),Y
   LDA   #$00
   SBC   #$00
   INY
   STA   (FP),Y
   LDY   #$10
   LDA   (FP),Y
   LDY   #$20
   CMP   (FP),Y
   BNE   .thread@2
   LDY   #$0F
   LDA   (FP),Y
   LDY   #$1F
   CMP   (FP),Y
   BEQ   .cmp_true@6
.thread@2:
   LDA   #$00
   JMP   .cmp_end@7
.cmp_true@6:
   LDA   #$01
.cmp_end@7:
   LDY   #$21
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@8
   LDA   #$01
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   JMP   .and_end@9
.and_false@8:
   LDA   #$00
   LDY   #$23
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@9:
   LDY   #$23
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@6
   SEC
   LDA   #$00
   SBC   #$65
   INY
   STA   (FP),Y
   LDA   #$00
   SBC   #$00
   INY
   STA   (FP),Y
   LDY   #$16
   LDA   (FP),Y
   LDY   #$26
   CMP   (FP),Y
   BNE   .thread@3
   LDY   #$15
   LDA   (FP),Y
   LDY   #$25
   CMP   (FP),Y
   BEQ   .cmp_true@9
.thread@3:
   LDA   #$00
   JMP   .cmp_end@10
.cmp_true@9:
   LDA   #$01
.cmp_end@10:
   LDY   #$27
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
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
   LDY   #$29
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@7:
   LDY   #$29
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@12
   LDX   #$00
   TXA

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$40
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$3D
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
.if_end@12:
   LDX   #$00
   LDA   #$01

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$40
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$3D
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
.if_else@5:
   LDY   #$02
   LDA   (FP),Y
   BNE   .thread@4
   DEY
   LDA   (FP),Y
   CMP   #$03
   BEQ   .cmp_true@12
.thread@4:
   LDA   #$00
   JMP   .cmp_end@13
.cmp_true@12:
   LDA   #$01
.cmp_end@13:
   LDY   #$2B
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@17
   LDY   #$08
   LDA   (FP),Y
   BNE   .thread@5
   DEY
   LDA   (FP),Y
   CMP   #$0A
   BEQ   .cmp_true@15
.thread@5:
   LDA   #$00
   JMP   .cmp_end@16
.cmp_true@15:
   LDA   #$01
.cmp_end@16:
   LDY   #$2D
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@17
   LDA   #$01
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   JMP   .and_end@18
.and_false@17:
   LDA   #$00
   LDY   #$2F
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@18:
   LDY   #$2F
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@15
   SEC
   LDA   #$00
   SBC   #$0C
   INY
   STA   (FP),Y
   LDA   #$00
   SBC   #$00
   INY
   STA   (FP),Y
   LDY   #$10
   LDA   (FP),Y
   LDY   #$32
   CMP   (FP),Y
   BNE   .thread@6
   LDY   #$0F
   LDA   (FP),Y
   LDY   #$31
   CMP   (FP),Y
   BEQ   .cmp_true@18
.thread@6:
   LDA   #$00
   JMP   .cmp_end@19
.cmp_true@18:
   LDA   #$01
.cmp_end@19:
   LDY   #$33
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@15
   LDA   #$01
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   JMP   .and_end@16
.and_false@15:
   LDA   #$00
   LDY   #$35
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@16:
   LDY   #$35
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@13
   SEC
   LDA   #$00
   SBC   #$64
   INY
   STA   (FP),Y
   LDA   #$00
   SBC   #$00
   INY
   STA   (FP),Y
   LDY   #$16
   LDA   (FP),Y
   LDY   #$38
   CMP   (FP),Y
   BNE   .thread@7
   LDY   #$15
   LDA   (FP),Y
   LDY   #$37
   CMP   (FP),Y
   BEQ   .cmp_true@21
.thread@7:
   LDA   #$00
   JMP   .cmp_end@22
.cmp_true@21:
   LDA   #$01
.cmp_end@22:
   LDY   #$39
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@13
   LDA   #$01
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   JMP   .and_end@14
.and_false@13:
   LDA   #$00
   LDY   #$3B
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@14:
   LDY   #$3B
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@19
   LDX   #$00
   TXA

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$40
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
   TAX
   PLA
   RTS
.if_end@19:
   LDX   #$00
   LDA   #$01

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$40
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$3D
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

   ; prologue: 0 arg bytes, 4 local bytes
   SEC
   LDA   SSP
   SBC   #$06
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$05
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
   SBC   #$02
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   #$00
   LDY   #$01
   STA   (SSP),Y
   INY
   STA   (SSP),Y
   JSR   target
   LDY   #$01
   STA   (FP),Y
   TXA
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@20
   LDX   #$00
   LDA   #$01

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$06
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$05
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
.if_end@20:
   SEC
   LDA   SSP
   SBC   #$02
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   #$01
   LDY   #$01
   STA   (SSP),Y
   LDA   #$00
   INY
   STA   (SSP),Y
   JSR   target
   LDY   #$03
   STA   (FP),Y
   TXA
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@21
   LDX   #$00
   LDA   #$02

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$06
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
   TAX
   PLA
   RTS
.if_end@21:
   LDX   #$00
   TXA

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$06
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$05
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
