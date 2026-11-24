SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24

main:
   SUBROUTINE

   ; prologue: 0 arg bytes, 186 local bytes
   SEC
   LDA   SSP
   SBC   #$BC
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$BB
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
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$24
   INY
   STA   (FP),Y
   LDA   #$40
   INY
   STA   (FP),Y
   CLC
   LDA   FP
   ADC   #$01
   INY
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
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
   CLC
   LDA   FP
   ADC   #$0B
   INY
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
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
   CLC
   LDA   FP
   ADC   #$0F
   INY
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$13
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$14
   STA   (FP),Y
   LDY   #$08
   LDA   (FP),Y
   CMP   #$40
   BNE   .cmp_true@0
   DEY
   LDA   (FP),Y
   CMP   #$24
   BNE   .cmp_true@0
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@0
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@0
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@0
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@0
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@0
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@0
   LDA   #$00
   JMP   .cmp_end@1
.cmp_true@0:
   LDA   #$01
.cmp_end@1:
   LDY   #$15
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
   ADC   #$BC
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$BB
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
   LDY   #$0B
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$17
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$18
   STA   (FP),Y
   LDY   #$02
   LDA   (DPTR),Y
   LDY   #$19
   STA   (FP),Y
   LDY   #$03
   LDA   (DPTR),Y
   LDY   #$1A
   STA   (FP),Y
   LDY   #$04
   LDA   (DPTR),Y
   LDY   #$1B
   STA   (FP),Y
   LDY   #$05
   LDA   (DPTR),Y
   LDY   #$1C
   STA   (FP),Y
   LDY   #$06
   LDA   (DPTR),Y
   LDY   #$1D
   STA   (FP),Y
   LDY   #$07
   LDA   (DPTR),Y
   LDY   #$1E
   STA   (FP),Y
   CMP   #$40
   BNE   .cmp_true@3
   DEY
   LDA   (FP),Y
   CMP   #$24
   BNE   .cmp_true@3
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@3
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@3
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@3
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@3
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@3
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@3
   LDA   #$00
   JMP   .cmp_end@4
.cmp_true@3:
   LDA   #$01
.cmp_end@4:
   LDY   #$1F
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
   ADC   #$BC
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$BB
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
   LDY   #$0F
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$21
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$22
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$23
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$24
   STA   (FP),Y
   LDY   #$02
   LDA   (DPTR),Y
   LDY   #$25
   STA   (FP),Y
   LDY   #$03
   LDA   (DPTR),Y
   LDY   #$26
   STA   (FP),Y
   LDY   #$04
   LDA   (DPTR),Y
   LDY   #$27
   STA   (FP),Y
   LDY   #$05
   LDA   (DPTR),Y
   LDY   #$28
   STA   (FP),Y
   LDY   #$06
   LDA   (DPTR),Y
   LDY   #$29
   STA   (FP),Y
   LDY   #$07
   LDA   (DPTR),Y
   LDY   #$2A
   STA   (FP),Y
   CMP   #$40
   BNE   .cmp_true@6
   DEY
   LDA   (FP),Y
   CMP   #$24
   BNE   .cmp_true@6
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@6
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@6
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@6
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@6
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@6
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@6
   LDA   #$00
   JMP   .cmp_end@7
.cmp_true@6:
   LDA   #$01
.cmp_end@7:
   LDY   #$2B
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@2
   LDX   #$00
   LDA   #$03

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$BC
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$BB
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
.if_end@2:
   LDY   #$13
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$2D
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$2E
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$2F
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$30
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$31
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$32
   STA   (FP),Y
   LDY   #$02
   LDA   (DPTR),Y
   LDY   #$33
   STA   (FP),Y
   LDY   #$03
   LDA   (DPTR),Y
   LDY   #$34
   STA   (FP),Y
   LDY   #$04
   LDA   (DPTR),Y
   LDY   #$35
   STA   (FP),Y
   LDY   #$05
   LDA   (DPTR),Y
   LDY   #$36
   STA   (FP),Y
   LDY   #$06
   LDA   (DPTR),Y
   LDY   #$37
   STA   (FP),Y
   LDY   #$07
   LDA   (DPTR),Y
   LDY   #$38
   STA   (FP),Y
   CMP   #$40
   BNE   .cmp_true@9
   DEY
   LDA   (FP),Y
   CMP   #$24
   BNE   .cmp_true@9
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@9
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@9
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@9
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@9
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@9
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@9
   LDA   #$00
   JMP   .cmp_end@10
.cmp_true@9:
   LDA   #$01
.cmp_end@10:
   LDY   #$39
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@3
   LDX   #$00
   LDA   #$04

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$BC
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$BB
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
.if_end@3:
   CLC
   LDA   FP
   ADC   #$01
   LDY   #$3B
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   LDA   (FP),Y
   LDY   #$0C
   CMP   (FP),Y
   BNE   .cmp_true@12
   LDY   #$3B
   LDA   (FP),Y
   LDY   #$0B
   CMP   (FP),Y
   BNE   .cmp_true@12
   LDA   #$00
   JMP   .cmp_end@13
.cmp_true@12:
   LDA   #$01
.cmp_end@13:
   LDY   #$3D
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@4
   LDX   #$00
   LDA   #$05

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$BC
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$BB
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
.if_end@4:
   LDY   #$0F
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
   LDY   #$0C
   CMP   (FP),Y
   BNE   .cmp_true@15
   LDY   #$3F
   LDA   (FP),Y
   LDY   #$0B
   CMP   (FP),Y
   BNE   .cmp_true@15
   LDA   #$00
   JMP   .cmp_end@16
.cmp_true@15:
   LDA   #$01
.cmp_end@16:
   LDY   #$41
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@5
   LDX   #$00
   LDA   #$06

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$BC
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$BB
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
.if_end@5:
   LDY   #$13
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$43
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$44
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$45
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$46
   STA   (FP),Y
   LDY   #$0C
   CMP   (FP),Y
   BNE   .cmp_true@18
   LDY   #$45
   LDA   (FP),Y
   LDY   #$0B
   CMP   (FP),Y
   BNE   .cmp_true@18
   LDA   #$00
   JMP   .cmp_end@19
.cmp_true@18:
   LDA   #$01
.cmp_end@19:
   LDY   #$47
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@6
   LDX   #$00
   LDA   #$07

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$BC
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$BB
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
.if_end@6:
   LDY   #$13
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$49
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$4A
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$4B
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$4C
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
   INY
   STA   (DPTR),Y
   INY
   STA   (DPTR),Y
   INY
   STA   (DPTR),Y
   INY
   STA   (DPTR),Y
   INY
   STA   (DPTR),Y
   LDA   #$14
   INY
   STA   (DPTR),Y
   LDA   #$40
   INY
   STA   (DPTR),Y
   INY
   LDA   (FP),Y
   CMP   #$40
   BNE   .cmp_true@21
   DEY
   LDA   (FP),Y
   CMP   #$14
   BNE   .cmp_true@21
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@21
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@21
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@21
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@21
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@21
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@21
   LDA   #$00
   JMP   .cmp_end@22
.cmp_true@21:
   LDA   #$01
.cmp_end@22:
   LDY   #$4D
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@7
   LDX   #$00
   LDA   #$08

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$BC
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$BB
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
.if_end@7:
   LDY   #$0B
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$4F
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$50
   STA   (FP),Y
   LDY   #$02
   LDA   (DPTR),Y
   LDY   #$51
   STA   (FP),Y
   LDY   #$03
   LDA   (DPTR),Y
   LDY   #$52
   STA   (FP),Y
   LDY   #$04
   LDA   (DPTR),Y
   LDY   #$53
   STA   (FP),Y
   LDY   #$05
   LDA   (DPTR),Y
   LDY   #$54
   STA   (FP),Y
   LDY   #$06
   LDA   (DPTR),Y
   LDY   #$55
   STA   (FP),Y
   LDY   #$07
   LDA   (DPTR),Y
   LDY   #$56
   STA   (FP),Y
   CMP   #$40
   BNE   .cmp_true@24
   DEY
   LDA   (FP),Y
   CMP   #$14
   BNE   .cmp_true@24
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@24
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@24
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@24
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@24
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@24
   DEY
   LDA   (FP),Y
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
   BEQ   .if_end@8
   LDX   #$00
   LDA   #$09

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$BC
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$BB
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
.if_end@8:
   LDY   #$0F
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$59
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$5A
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$5B
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$5C
   STA   (FP),Y
   LDY   #$02
   LDA   (DPTR),Y
   LDY   #$5D
   STA   (FP),Y
   LDY   #$03
   LDA   (DPTR),Y
   LDY   #$5E
   STA   (FP),Y
   LDY   #$04
   LDA   (DPTR),Y
   LDY   #$5F
   STA   (FP),Y
   LDY   #$05
   LDA   (DPTR),Y
   LDY   #$60
   STA   (FP),Y
   LDY   #$06
   LDA   (DPTR),Y
   LDY   #$61
   STA   (FP),Y
   LDY   #$07
   LDA   (DPTR),Y
   LDY   #$62
   STA   (FP),Y
   CMP   #$40
   BNE   .cmp_true@27
   DEY
   LDA   (FP),Y
   CMP   #$14
   BNE   .cmp_true@27
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@27
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@27
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@27
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@27
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@27
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@27
   LDA   #$00
   JMP   .cmp_end@28
.cmp_true@27:
   LDA   #$01
.cmp_end@28:
   LDY   #$63
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@9
   LDX   #$00
   LDA   #$0A

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$BC
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$BB
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
.if_end@9:
   LDY   #$13
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$65
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$66
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$67
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$68
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$69
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$6A
   STA   (FP),Y
   LDY   #$02
   LDA   (DPTR),Y
   LDY   #$6B
   STA   (FP),Y
   LDY   #$03
   LDA   (DPTR),Y
   LDY   #$6C
   STA   (FP),Y
   LDY   #$04
   LDA   (DPTR),Y
   LDY   #$6D
   STA   (FP),Y
   LDY   #$05
   LDA   (DPTR),Y
   LDY   #$6E
   STA   (FP),Y
   LDY   #$06
   LDA   (DPTR),Y
   LDY   #$6F
   STA   (FP),Y
   LDY   #$07
   LDA   (DPTR),Y
   LDY   #$70
   STA   (FP),Y
   CMP   #$40
   BNE   .cmp_true@30
   DEY
   LDA   (FP),Y
   CMP   #$14
   BNE   .cmp_true@30
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@30
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@30
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@30
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@30
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@30
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@30
   LDA   #$00
   JMP   .cmp_end@31
.cmp_true@30:
   LDA   #$01
.cmp_end@31:
   LDY   #$71
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@10
   LDX   #$00
   LDA   #$0B

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$BC
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$BB
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
.if_end@10:
   LDA   #$00
   LDY   #$73
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$F0
   INY
   STA   (FP),Y
   LDA   #$3F
   INY
   STA   (FP),Y
   CLC
   LDA   FP
   ADC   #$73
   INY
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$7D
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$7E
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$7F
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$80
   STA   (FP),Y
   CLC
   LDA   FP
   ADC   #$7D
   INY
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$83
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$84
   STA   (FP),Y
   LDY   #$13
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$83
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$84
   LDA   (FP),Y
   LDY   #$01
   STA   (DPTR),Y
   LDY   #$13
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$85
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$86
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$87
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$88
   STA   (FP),Y
   LDY   #$7E
   CMP   (FP),Y
   BNE   .cmp_true@33
   LDY   #$87
   LDA   (FP),Y
   LDY   #$7D
   CMP   (FP),Y
   BNE   .cmp_true@33
   LDA   #$00
   JMP   .cmp_end@34
.cmp_true@33:
   LDA   #$01
.cmp_end@34:
   LDY   #$89
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@11
   LDX   #$00
   LDA   #$0C

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$BC
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$BB
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
.if_end@11:
   LDY   #$13
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$8B
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$8C
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$8D
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$8E
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$8F
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$90
   STA   (FP),Y
   LDY   #$02
   LDA   (DPTR),Y
   LDY   #$91
   STA   (FP),Y
   LDY   #$03
   LDA   (DPTR),Y
   LDY   #$92
   STA   (FP),Y
   LDY   #$04
   LDA   (DPTR),Y
   LDY   #$93
   STA   (FP),Y
   LDY   #$05
   LDA   (DPTR),Y
   LDY   #$94
   STA   (FP),Y
   LDY   #$06
   LDA   (DPTR),Y
   LDY   #$95
   STA   (FP),Y
   LDY   #$07
   LDA   (DPTR),Y
   LDY   #$96
   STA   (FP),Y
   CMP   #$3F
   BNE   .cmp_true@36
   DEY
   LDA   (FP),Y
   CMP   #$F0
   BNE   .cmp_true@36
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@36
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@36
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@36
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@36
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@36
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@36
   LDA   #$00
   JMP   .cmp_end@37
.cmp_true@36:
   LDA   #$01
.cmp_end@37:
   LDY   #$97
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@12
   LDX   #$00
   LDA   #$0D

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$BC
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$BB
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
   CLC
   LDA   FP
   ADC   #$7F
   LDY   #$99
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   LDY   #$84
   LDA   (FP),Y
   LDY   #$9A
   CMP   (FP),Y
   BNE   .thread@0
   LDY   #$83
   LDA   (FP),Y
   LDY   #$99
   CMP   (FP),Y
   BEQ   .cmp_true@39
.thread@0:
   LDA   #$00
   JMP   .cmp_end@40
.cmp_true@39:
   LDA   #$01
.cmp_end@40:
   LDY   #$9B
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@13
   LDX   #$00
   LDA   #$0E

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$BC
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$BB
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
.if_end@13:
   LDY   #$0B
   LDA   (FP),Y
   LDY   #$7D
   STA   (FP),Y
   LDY   #$0C
   LDA   (FP),Y
   LDY   #$7E
   STA   (FP),Y
   LDY   #$13
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$9D
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$9E
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$9F
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$A0
   STA   (FP),Y
   LDY   #$0C
   CMP   (FP),Y
   BNE   .cmp_true@42
   LDY   #$9F
   LDA   (FP),Y
   LDY   #$0B
   CMP   (FP),Y
   BNE   .cmp_true@42
   LDA   #$00
   JMP   .cmp_end@43
.cmp_true@42:
   LDA   #$01
.cmp_end@43:
   LDY   #$A1
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@14
   LDX   #$00
   LDA   #$0F

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$BC
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$BB
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
.if_end@14:
   LDY   #$83
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$A3
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$A4
   STA   (FP),Y
   LDY   #$0C
   CMP   (FP),Y
   BNE   .cmp_true@45
   LDY   #$A3
   LDA   (FP),Y
   LDY   #$0B
   CMP   (FP),Y
   BNE   .cmp_true@45
   LDA   #$00
   JMP   .cmp_end@46
.cmp_true@45:
   LDA   #$01
.cmp_end@46:
   LDY   #$A5
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@15
   LDX   #$00
   LDA   #$10

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$BC
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$BB
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
.if_end@15:
   LDY   #$13
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$A7
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$A8
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$A9
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$AA
   STA   (FP),Y
   LDY   #$80
   CMP   (FP),Y
   BNE   .thread@1
   LDY   #$A9
   LDA   (FP),Y
   LDY   #$7F
   CMP   (FP),Y
   BEQ   .cmp_true@48
.thread@1:
   LDA   #$00
   JMP   .cmp_end@49
.cmp_true@48:
   LDA   #$01
.cmp_end@49:
   LDY   #$AB
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@16
   LDX   #$00
   LDA   #$11

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$BC
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$BB
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
.if_end@16:
   LDY   #$13
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$AD
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$AE
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$AF
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$B0
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$B1
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$B2
   STA   (FP),Y
   LDY   #$02
   LDA   (DPTR),Y
   LDY   #$B3
   STA   (FP),Y
   LDY   #$03
   LDA   (DPTR),Y
   LDY   #$B4
   STA   (FP),Y
   LDY   #$04
   LDA   (DPTR),Y
   LDY   #$B5
   STA   (FP),Y
   LDY   #$05
   LDA   (DPTR),Y
   LDY   #$B6
   STA   (FP),Y
   LDY   #$06
   LDA   (DPTR),Y
   LDY   #$B7
   STA   (FP),Y
   LDY   #$07
   LDA   (DPTR),Y
   LDY   #$B8
   STA   (FP),Y
   CMP   #$40
   BNE   .cmp_true@51
   DEY
   LDA   (FP),Y
   CMP   #$14
   BNE   .cmp_true@51
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@51
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@51
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@51
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@51
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@51
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@51
   LDA   #$00
   JMP   .cmp_end@52
.cmp_true@51:
   LDA   #$01
.cmp_end@52:
   LDY   #$B9
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@17
   LDX   #$00
   LDA   #$12

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$BC
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
.if_end@17:
   LDX   #$00
   TXA

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$BC
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$BB
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
