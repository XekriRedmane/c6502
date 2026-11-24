SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24

get_double:
   SUBROUTINE

   LDA   #$00
   STA   HARGS+16
   LDY   #$00
   STY   HARGS+17
   STA   HARGS+18
   LDA   #$A2
   STA   HARGS+19
   LDA   #$94
   STA   HARGS+20
   LDA   #$1A
   STA   HARGS+21
   LDA   #$7D
   STA   HARGS+22
   LDA   #$42
   STA   HARGS+23
   RTS

accept_params:
   SUBROUTINE

   ; prologue: 30 arg bytes, 41 local bytes
   SEC
   LDA   SSP
   SBC   #$2B
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$2A
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   LDY   #$41
   LDA   (FP),Y
   CMP   #$42
   BNE   .cmp_true@0
   DEY
   LDA   (FP),Y
   CMP   #$8D
   BNE   .cmp_true@0
   DEY
   LDA   (FP),Y
   CMP   #$1A
   BNE   .cmp_true@0
   DEY
   LDA   (FP),Y
   CMP   #$94
   BNE   .cmp_true@0
   DEY
   LDA   (FP),Y
   CMP   #$A2
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
   LDY   #$01
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@12
   LDY   #$43
   LDA   (FP),Y
   BNE   .cmp_true@3
   DEY
   LDA   (FP),Y
   CMP   #$7F
   BNE   .cmp_true@3
   LDA   #$00
   JMP   .cmp_end@4
.cmp_true@3:
   LDA   #$01
.cmp_end@4:
   LDY   #$03
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@12
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   JMP   .or_end@13
.or_true@12:
   LDA   #$01
   LDY   #$05
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@13:
   LDY   #$05
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@10
   LDY   #$47
   LDA   (FP),Y
   BNE   .cmp_true@6
   DEY
   LDA   (FP),Y
   CMP   #$01
   BNE   .cmp_true@6
   DEY
   LDA   (FP),Y
   CMP   #$E2
   BNE   .cmp_true@6
   DEY
   LDA   (FP),Y
   CMP   #$40
   BNE   .cmp_true@6
   LDA   #$00
   JMP   .cmp_end@7
.cmp_true@6:
   LDA   #$01
.cmp_end@7:
   LDY   #$07
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@10
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   JMP   .or_end@11
.or_true@10:
   LDA   #$01
   LDY   #$09
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@11:
   LDY   #$09
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@8
   LDY   #$48
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$0B
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CMP   #$64
   BNE   .cmp_true@9
   LDA   #$00
   JMP   .cmp_end@10
.cmp_true@9:
   LDA   #$01
.cmp_end@10:
   LDY   #$0E
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
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
   LDY   #$10
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@9:
   LDY   #$10
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@0
   JMP   .or_true@6
.lb_skip@0:
   LDY   #$2C
   LDA   (FP),Y
   STA   HARGS
   INY
   LDA   (FP),Y
   STA   HARGS+1
   JSR   i2d
   LDA   HARGS+2
   LDY   #$12
   STA   (FP),Y
   LDA   HARGS+3
   INY
   STA   (FP),Y
   LDA   HARGS+4
   INY
   STA   (FP),Y
   LDA   HARGS+5
   INY
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
   LDA   HARGS+9
   INY
   STA   (FP),Y
   CMP   #$40
   BNE   .cmp_true@11
   DEY
   LDA   (FP),Y
   CMP   #$59
   BNE   .cmp_true@11
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@11
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@11
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@11
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@11
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@11
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@11
   LDA   #$00
   JMP   .cmp_end@12
.cmp_true@11:
   LDA   #$01
.cmp_end@12:
   LDY   #$1A
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@6
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   JMP   .or_end@7
.or_true@6:
   LDA   #$01
   LDY   #$1C
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@7:
   LDY   #$1C
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@4
   LDY   #$2F
   LDA   (FP),Y
   BNE   .cmp_true@14
   DEY
   LDA   (FP),Y
   CMP   #$FE
   BNE   .cmp_true@14
   LDA   #$00
   JMP   .cmp_end@15
.cmp_true@14:
   LDA   #$01
.cmp_end@15:
   LDY   #$1E
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@4
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   JMP   .or_end@5
.or_true@4:
   LDA   #$01
   LDY   #$20
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@5:
   LDY   #$20
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@2
   LDY   #$37
   LDA   (FP),Y
   CMP   #$40
   BNE   .cmp_true@17
   DEY
   LDA   (FP),Y
   CMP   #$FE
   BNE   .cmp_true@17
   DEY
   LDA   (FP),Y
   CMP   #$24
   BNE   .cmp_true@17
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@17
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@17
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@17
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@17
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@17
   LDA   #$00
   JMP   .cmp_end@18
.cmp_true@17:
   LDA   #$01
.cmp_end@18:
   LDY   #$22
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@2
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   JMP   .or_end@3
.or_true@2:
   LDA   #$01
   LDY   #$24
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@3:
   LDY   #$24
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@0
   LDY   #$39
   LDA   (FP),Y
   BNE   .cmp_true@20
   DEY
   LDA   (FP),Y
   CMP   #$64
   BNE   .cmp_true@20
   LDA   #$00
   JMP   .cmp_end@21
.cmp_true@20:
   LDA   #$01
.cmp_end@21:
   LDY   #$26
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
   LDY   #$28
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@1:
   LDY   #$28
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@14
   LDX   #$00
   TXA

   ; epilogue
   PHA
   TXA
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
   TAX
   PLA
   RTS
.if_end@14:
   LDX   #$00
   LDA   #$01

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$49
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$2A
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

test_auto:
   SUBROUTINE

   ; prologue: 0 arg bytes, 232 local bytes
   SEC
   LDA   SSP
   SBC   #$EA
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$E9
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   JSR   get_double
   LDA   HARGS+16
   LDY   #$01
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
   LDA   HARGS+21
   INY
   STA   (FP),Y
   LDA   HARGS+22
   INY
   STA   (FP),Y
   LDA   HARGS+23
   INY
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
   INY
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
   LDA   #$00
   STA   HARGS+8
   LDY   #$00
   STY   HARGS+9
   STY   HARGS+10
   STY   HARGS+11
   STY   HARGS+12
   STY   HARGS+13
   STA   HARGS+14
   LDA   #$40
   STA   HARGS+15
   JSR   dmul
   LDA   HARGS+16
   LDY   #$09
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
   LDA   HARGS+21
   INY
   STA   (FP),Y
   LDA   HARGS+22
   INY
   STA   (FP),Y
   LDA   HARGS+23
   INY
   STA   (FP),Y
   CLC
   LDA   FP
   ADC   #$11
   LDY   #$20
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
   LDY   #$09
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$0A
   LDA   (FP),Y
   LDY   #$01
   STA   (DPTR),Y
   LDY   #$0B
   LDA   (FP),Y
   LDY   #$02
   STA   (DPTR),Y
   LDY   #$0C
   LDA   (FP),Y
   LDY   #$03
   STA   (DPTR),Y
   LDY   #$0D
   LDA   (FP),Y
   LDY   #$04
   STA   (DPTR),Y
   LDY   #$0E
   LDA   (FP),Y
   LDY   #$05
   STA   (DPTR),Y
   LDY   #$0F
   LDA   (FP),Y
   LDY   #$06
   STA   (DPTR),Y
   LDY   #$10
   LDA   (FP),Y
   LDY   #$07
   STA   (DPTR),Y
   LDA   #$7F
   LDY   #$22
   STA   (FP),Y
   CLC
   LDA   FP
   ADC   #$11
   INY
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$08
   LDY   #$25
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$26
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$22
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDA   #$04
   LDY   #$27
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   ORA   #$00
   BPL   .sx_done@24
   LDA   #$FF
.sx_done@24:
   LDY   #$29
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   l
   STA   HARGS
   LDA   l+1
   STA   HARGS+1
   LDA   l+2
   STA   HARGS+2
   LDA   l+3
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
   LDA   HARGS+8
   LDY   #$2B
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
   CLC
   LDA   FP
   ADC   #$11
   INY
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$09
   LDY   #$31
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$32
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$2B
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$2C
   LDA   (FP),Y
   LDY   #$01
   STA   (DPTR),Y
   LDY   #$2D
   LDA   (FP),Y
   LDY   #$02
   STA   (DPTR),Y
   LDY   #$2E
   LDA   (FP),Y
   LDY   #$03
   STA   (DPTR),Y
   LDA   #$64
   LDY   #$33
   STA   (FP),Y
   INY
   STA   (FP),Y
   CLC
   LDA   FP
   ADC   #$34
   LDY   #$39
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   CLC
   LDA   FP
   ADC   #$11
   LDY   #$35
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$0D
   LDY   #$37
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$38
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   INY
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$3A
   LDA   (FP),Y
   LDY   #$01
   STA   (DPTR),Y
   CLC
   LDA   FP
   ADC   #$11
   LDY   #$3B
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
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$3D
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$3E
   STA   (FP),Y
   LDY   #$02
   LDA   (DPTR),Y
   LDY   #$3F
   STA   (FP),Y
   LDY   #$03
   LDA   (DPTR),Y
   LDY   #$40
   STA   (FP),Y
   LDY   #$04
   LDA   (DPTR),Y
   LDY   #$41
   STA   (FP),Y
   LDY   #$05
   LDA   (DPTR),Y
   LDY   #$42
   STA   (FP),Y
   LDY   #$06
   LDA   (DPTR),Y
   LDY   #$43
   STA   (FP),Y
   LDY   #$07
   LDA   (DPTR),Y
   LDY   #$44
   STA   (FP),Y
   CMP   #$42
   BNE   .cmp_true@25
   DEY
   LDA   (FP),Y
   CMP   #$8D
   BNE   .cmp_true@25
   DEY
   LDA   (FP),Y
   CMP   #$1A
   BNE   .cmp_true@25
   DEY
   LDA   (FP),Y
   CMP   #$94
   BNE   .cmp_true@25
   DEY
   LDA   (FP),Y
   CMP   #$A2
   BNE   .cmp_true@25
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@25
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@25
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@25
   LDA   #$00
   JMP   .cmp_end@26
.cmp_true@25:
   LDA   #$01
.cmp_end@26:
   LDY   #$45
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@19
   CLC
   LDA   FP
   ADC   #$11
   INY
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$08
   LDY   #$49
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
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
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CMP   #$7F
   BNE   .cmp_true@28
   LDA   #$00
   JMP   .cmp_end@29
.cmp_true@28:
   LDA   #$01
.cmp_end@29:
   LDY   #$4E
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@19
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   JMP   .or_end@20
.or_true@19:
   LDA   #$01
   LDY   #$50
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@20:
   LDY   #$50
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@2
   JMP   .or_true@17
.lb_skip@2:
   CLC
   LDA   FP
   ADC   #$11
   LDY   #$52
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$09
   LDY   #$54
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$55
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$56
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$57
   STA   (FP),Y
   LDY   #$02
   LDA   (DPTR),Y
   LDY   #$58
   STA   (FP),Y
   LDY   #$03
   LDA   (DPTR),Y
   LDY   #$59
   STA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@30
   DEY
   LDA   (FP),Y
   CMP   #$01
   BNE   .cmp_true@30
   DEY
   LDA   (FP),Y
   CMP   #$E2
   BNE   .cmp_true@30
   DEY
   LDA   (FP),Y
   CMP   #$40
   BNE   .cmp_true@30
   LDA   #$00
   JMP   .cmp_end@31
.cmp_true@30:
   LDA   #$01
.cmp_end@31:
   LDY   #$5A
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@17
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   JMP   .or_end@18
.or_true@17:
   LDA   #$01
   LDY   #$5C
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@18:
   LDY   #$5C
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@1
   JMP   .or_true@15
.lb_skip@1:
   CLC
   LDA   FP
   ADC   #$11
   LDY   #$5E
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$0D
   LDY   #$60
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$61
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$62
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$63
   STA   (FP),Y
   CLC
   LDA   FP
   ADC   #$34
   INY
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   LDY   #$63
   LDA   (FP),Y
   LDY   #$65
   CMP   (FP),Y
   BNE   .cmp_true@33
   LDY   #$62
   LDA   (FP),Y
   LDY   #$64
   CMP   (FP),Y
   BNE   .cmp_true@33
   LDA   #$00
   JMP   .cmp_end@34
.cmp_true@33:
   LDA   #$01
.cmp_end@34:
   LDY   #$66
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@15
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   JMP   .or_end@16
.or_true@15:
   LDA   #$01
   LDY   #$68
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@16:
   LDY   #$68
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@21
   LDX   #$00
   TXA

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$EA
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$E9
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
   CLC
   LDA   FP
   ADC   #$11
   LDY   #$6A
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$6C
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$6D
   STA   (FP),Y
   CLC
   LDA   FP
   ADC   #$11
   INY
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$08
   LDY   #$70
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$71
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$72
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$73
   STA   (FP),Y
   LDY   #$6C
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$74
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$75
   STA   (FP),Y
   LDY   #$02
   LDA   (DPTR),Y
   LDY   #$76
   STA   (FP),Y
   LDY   #$03
   LDA   (DPTR),Y
   LDY   #$77
   STA   (FP),Y
   LDY   #$04
   LDA   (DPTR),Y
   LDY   #$78
   STA   (FP),Y
   LDY   #$05
   LDA   (DPTR),Y
   LDY   #$79
   STA   (FP),Y
   LDY   #$06
   LDA   (DPTR),Y
   LDY   #$7A
   STA   (FP),Y
   LDY   #$07
   LDA   (DPTR),Y
   LDY   #$7B
   STA   (FP),Y
   CMP   #$42
   BNE   .cmp_true@36
   DEY
   LDA   (FP),Y
   CMP   #$8D
   BNE   .cmp_true@36
   DEY
   LDA   (FP),Y
   CMP   #$1A
   BNE   .cmp_true@36
   DEY
   LDA   (FP),Y
   CMP   #$94
   BNE   .cmp_true@36
   DEY
   LDA   (FP),Y
   CMP   #$A2
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
   LDY   #$7C
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@22
   LDY   #$72
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$7E
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CMP   #$7F
   BNE   .cmp_true@39
   LDA   #$00
   JMP   .cmp_end@40
.cmp_true@39:
   LDA   #$01
.cmp_end@40:
   LDY   #$81
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@22
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   JMP   .or_end@23
.or_true@22:
   LDA   #$01
   LDY   #$83
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@23:
   LDY   #$83
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@24
   LDX   #$00
   TXA

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$EA
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$E9
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
.if_end@24:
   CLC
   LDA   FP
   ADC   #$11
   LDY   #$85
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$0D
   LDY   #$87
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$88
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$89
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$8A
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$8B
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CMP   #$64
   BNE   .cmp_true@41
   LDA   #$00
   JMP   .cmp_end@42
.cmp_true@41:
   LDA   #$01
.cmp_end@42:
   LDY   #$8E
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@25
   LDX   #$00
   TXA

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$EA
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$E9
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
.if_end@25:
   CLC
   LDA   FP
   ADC   #$11
   LDY   #$90
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
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$92
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$93
   STA   (FP),Y
   LDY   #$02
   LDA   (DPTR),Y
   LDY   #$94
   STA   (FP),Y
   LDY   #$03
   LDA   (DPTR),Y
   LDY   #$95
   STA   (FP),Y
   LDY   #$04
   LDA   (DPTR),Y
   LDY   #$96
   STA   (FP),Y
   LDY   #$05
   LDA   (DPTR),Y
   LDY   #$97
   STA   (FP),Y
   LDY   #$06
   LDA   (DPTR),Y
   LDY   #$98
   STA   (FP),Y
   LDY   #$07
   LDA   (DPTR),Y
   LDY   #$99
   STA   (FP),Y
   LDY   #$92
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
   INY
   LDA   (FP),Y
   STA   HARGS+5
   INY
   LDA   (FP),Y
   STA   HARGS+6
   INY
   LDA   (FP),Y
   STA   HARGS+7
   LDA   #$00
   STA   HARGS+8
   LDY   #$00
   STY   HARGS+9
   STA   HARGS+10
   LDA   #$20
   STA   HARGS+11
   LDA   #$5F
   STA   HARGS+12
   LDA   #$A0
   STA   HARGS+13
   LDA   #$22
   STA   HARGS+14
   LDA   #$42
   STA   HARGS+15
   JSR   ddiv
   LDA   HARGS+16
   LDY   #$9A
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
   LDA   HARGS+21
   INY
   STA   (FP),Y
   LDA   HARGS+22
   INY
   STA   (FP),Y
   LDA   HARGS+23
   INY
   STA   (FP),Y
   LDY   #$9A
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
   INY
   LDA   (FP),Y
   STA   HARGS+5
   INY
   LDA   (FP),Y
   STA   HARGS+6
   INY
   LDA   (FP),Y
   STA   HARGS+7
   JSR   d2i
   LDA   HARGS+8
   LDY   #$A2
   STA   (FP),Y
   LDA   HARGS+9
   INY
   STA   (FP),Y
   CLC
   LDA   FP
   ADC   #$11
   INY
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$08
   LDY   #$A6
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$A7
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$A8
   STA   (FP),Y
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
.mul_loop@43:
   BCC   .mul_skip@44
   CLC
   ADC   #$02
.mul_skip@44:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@43
   LDY   #$AC
   STA   (FP),Y
   LDA   HARGS
   DEY
   STA   (FP),Y
   CLC
   LDA   FP
   ADC   #$11
   LDY   #$AD
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$09
   LDY   #$AF
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
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
   LDY   #$B1
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
   JSR   l2d
   LDA   HARGS+4
   LDY   #$B5
   STA   (FP),Y
   LDA   HARGS+5
   INY
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
   LDA   HARGS+9
   INY
   STA   (FP),Y
   LDA   HARGS+10
   INY
   STA   (FP),Y
   LDA   HARGS+11
   INY
   STA   (FP),Y
   CLC
   LDA   FP
   ADC   #$11
   INY
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$0D
   LDY   #$BF
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$C0
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$C1
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$C2
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$C3
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   CLC
   LDA   FP
   ADC   #$11
   INY
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
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$C8
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$C9
   STA   (FP),Y
   LDY   #$02
   LDA   (DPTR),Y
   LDY   #$CA
   STA   (FP),Y
   LDY   #$03
   LDA   (DPTR),Y
   LDY   #$CB
   STA   (FP),Y
   LDY   #$04
   LDA   (DPTR),Y
   LDY   #$CC
   STA   (FP),Y
   LDY   #$05
   LDA   (DPTR),Y
   LDY   #$CD
   STA   (FP),Y
   LDY   #$06
   LDA   (DPTR),Y
   LDY   #$CE
   STA   (FP),Y
   LDY   #$07
   LDA   (DPTR),Y
   LDY   #$CF
   STA   (FP),Y
   CLC
   LDA   FP
   ADC   #$11
   INY
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$08
   LDY   #$D2
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$D3
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$D4
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   CLC
   LDA   FP
   ADC   #$11
   INY
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$09
   LDY   #$D9
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$DA
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$DB
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$DC
   STA   (FP),Y
   LDY   #$02
   LDA   (DPTR),Y
   LDY   #$DD
   STA   (FP),Y
   LDY   #$03
   LDA   (DPTR),Y
   LDY   #$DE
   STA   (FP),Y
   CLC
   LDA   FP
   ADC   #$11
   INY
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$0D
   LDY   #$E1
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$E2
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$E3
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$E4
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$1E
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDY   #$A2
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$A3
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$AB
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$AC
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   LDY   #$B5
   LDA   (FP),Y
   LDY   #$05
   STA   (SSP),Y
   LDY   #$B6
   LDA   (FP),Y
   LDY   #$06
   STA   (SSP),Y
   LDY   #$B7
   LDA   (FP),Y
   LDY   #$07
   STA   (SSP),Y
   LDY   #$B8
   LDA   (FP),Y
   LDY   #$08
   STA   (SSP),Y
   LDY   #$B9
   LDA   (FP),Y
   LDY   #$09
   STA   (SSP),Y
   LDY   #$BA
   LDA   (FP),Y
   LDY   #$0A
   STA   (SSP),Y
   LDY   #$BB
   LDA   (FP),Y
   LDY   #$0B
   STA   (SSP),Y
   LDY   #$BC
   LDA   (FP),Y
   LDY   #$0C
   STA   (SSP),Y
   LDY   #$C4
   LDA   (FP),Y
   LDY   #$0D
   STA   (SSP),Y
   LDY   #$C5
   LDA   (FP),Y
   LDY   #$0E
   STA   (SSP),Y
   LDY   #$C8
   LDA   (FP),Y
   LDY   #$0F
   STA   (SSP),Y
   LDY   #$C9
   LDA   (FP),Y
   LDY   #$10
   STA   (SSP),Y
   LDY   #$CA
   LDA   (FP),Y
   LDY   #$11
   STA   (SSP),Y
   LDY   #$CB
   LDA   (FP),Y
   LDY   #$12
   STA   (SSP),Y
   LDY   #$CC
   LDA   (FP),Y
   LDY   #$13
   STA   (SSP),Y
   LDY   #$CD
   LDA   (FP),Y
   LDY   #$14
   STA   (SSP),Y
   LDY   #$CE
   LDA   (FP),Y
   LDY   #$15
   STA   (SSP),Y
   LDY   #$CF
   LDA   (FP),Y
   LDY   #$16
   STA   (SSP),Y
   LDY   #$D5
   LDA   (FP),Y
   LDY   #$17
   STA   (SSP),Y
   LDY   #$D6
   LDA   (FP),Y
   LDY   #$18
   STA   (SSP),Y
   LDY   #$DB
   LDA   (FP),Y
   LDY   #$19
   STA   (SSP),Y
   LDY   #$DC
   LDA   (FP),Y
   LDY   #$1A
   STA   (SSP),Y
   LDY   #$DD
   LDA   (FP),Y
   LDY   #$1B
   STA   (SSP),Y
   LDY   #$DE
   LDA   (FP),Y
   LDY   #$1C
   STA   (SSP),Y
   LDY   #$E3
   LDA   (FP),Y
   LDY   #$1D
   STA   (SSP),Y
   LDY   #$E4
   LDA   (FP),Y
   LDY   #$1E
   STA   (SSP),Y
   JSR   accept_params
   LDY   #$E5
   STA   (FP),Y
   TXA
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lnot_true@45
   LDA   #$00
   JMP   .lnot_end@46
.lnot_true@45:
   LDA   #$01
.lnot_end@46:
   LDY   #$E7
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@26
   LDX   #$00
   TXA

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$EA
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
.if_end@26:
   LDX   #$00
   LDA   #$01

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$EA
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$E9
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

test_static:
   SUBROUTINE

   ; prologue: 0 arg bytes, 215 local bytes
   SEC
   LDA   SSP
   SBC   #$D9
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$D8
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   JSR   get_double
   LDA   HARGS+16
   LDY   #$01
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
   LDA   HARGS+21
   INY
   STA   (FP),Y
   LDA   HARGS+22
   INY
   STA   (FP),Y
   LDA   HARGS+23
   INY
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
   INY
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
   LDA   #$00
   STA   HARGS+8
   LDY   #$00
   STY   HARGS+9
   STY   HARGS+10
   STY   HARGS+11
   STY   HARGS+12
   STY   HARGS+13
   STA   HARGS+14
   LDA   #$40
   STA   HARGS+15
   JSR   dmul
   LDA   HARGS+16
   LDY   #$09
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
   LDA   HARGS+21
   INY
   STA   (FP),Y
   LDA   HARGS+22
   INY
   STA   (FP),Y
   LDA   HARGS+23
   INY
   STA   (FP),Y
   LDA   #<@12.stat
   INY
   STA   (FP),Y
   LDA   #>@12.stat
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$09
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$0A
   LDA   (FP),Y
   LDY   #$01
   STA   (DPTR),Y
   LDY   #$0B
   LDA   (FP),Y
   LDY   #$02
   STA   (DPTR),Y
   LDY   #$0C
   LDA   (FP),Y
   LDY   #$03
   STA   (DPTR),Y
   LDY   #$0D
   LDA   (FP),Y
   LDY   #$04
   STA   (DPTR),Y
   LDY   #$0E
   LDA   (FP),Y
   LDY   #$05
   STA   (DPTR),Y
   LDY   #$0F
   LDA   (FP),Y
   LDY   #$06
   STA   (DPTR),Y
   LDY   #$10
   LDA   (FP),Y
   LDY   #$07
   STA   (DPTR),Y
   LDA   #$7F
   LDY   #$13
   STA   (FP),Y
   LDA   #<@12.stat
   INY
   STA   (FP),Y
   LDA   #>@12.stat
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$08
   LDY   #$16
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$17
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$13
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDA   #$04
   LDY   #$18
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   ORA   #$00
   BPL   .sx_done@48
   LDA   #$FF
.sx_done@48:
   LDY   #$1A
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   l
   STA   HARGS
   LDA   l+1
   STA   HARGS+1
   LDA   l+2
   STA   HARGS+2
   LDA   l+3
   STA   HARGS+3
   LDY   #$18
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
   LDY   #$1C
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
   LDA   #<@12.stat
   INY
   STA   (FP),Y
   LDA   #>@12.stat
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$09
   LDY   #$22
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$23
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$1C
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$1D
   LDA   (FP),Y
   LDY   #$01
   STA   (DPTR),Y
   LDY   #$1E
   LDA   (FP),Y
   LDY   #$02
   STA   (DPTR),Y
   LDY   #$1F
   LDA   (FP),Y
   LDY   #$03
   STA   (DPTR),Y
   LDA   #<@13.chr
   LDY   #$24
   STA   (FP),Y
   LDA   #>@13.chr
   INY
   STA   (FP),Y
   LDA   #<@12.stat
   INY
   STA   (FP),Y
   LDA   #>@12.stat
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$0D
   LDY   #$28
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$29
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$24
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$25
   LDA   (FP),Y
   LDY   #$01
   STA   (DPTR),Y
   LDA   #<@12.stat
   LDY   #$2A
   STA   (FP),Y
   LDA   #>@12.stat
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$2C
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$2D
   STA   (FP),Y
   LDY   #$02
   LDA   (DPTR),Y
   LDY   #$2E
   STA   (FP),Y
   LDY   #$03
   LDA   (DPTR),Y
   LDY   #$2F
   STA   (FP),Y
   LDY   #$04
   LDA   (DPTR),Y
   LDY   #$30
   STA   (FP),Y
   LDY   #$05
   LDA   (DPTR),Y
   LDY   #$31
   STA   (FP),Y
   LDY   #$06
   LDA   (DPTR),Y
   LDY   #$32
   STA   (FP),Y
   LDY   #$07
   LDA   (DPTR),Y
   LDY   #$33
   STA   (FP),Y
   CMP   #$42
   BNE   .cmp_true@49
   DEY
   LDA   (FP),Y
   CMP   #$8D
   BNE   .cmp_true@49
   DEY
   LDA   (FP),Y
   CMP   #$1A
   BNE   .cmp_true@49
   DEY
   LDA   (FP),Y
   CMP   #$94
   BNE   .cmp_true@49
   DEY
   LDA   (FP),Y
   CMP   #$A2
   BNE   .cmp_true@49
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@49
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@49
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@49
   LDA   #$00
   JMP   .cmp_end@50
.cmp_true@49:
   LDA   #$01
.cmp_end@50:
   LDY   #$34
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@31
   LDA   #<@12.stat
   INY
   STA   (FP),Y
   LDA   #>@12.stat
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$08
   LDY   #$38
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$39
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$3A
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CMP   #$7F
   BNE   .cmp_true@52
   LDA   #$00
   JMP   .cmp_end@53
.cmp_true@52:
   LDA   #$01
.cmp_end@53:
   LDY   #$3D
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@31
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   JMP   .or_end@32
.or_true@31:
   LDA   #$01
   LDY   #$3F
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@32:
   LDY   #$3F
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@4
   JMP   .or_true@29
.lb_skip@4:
   LDA   #<@12.stat
   LDY   #$41
   STA   (FP),Y
   LDA   #>@12.stat
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$09
   LDY   #$43
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
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
   LDY   #$02
   LDA   (DPTR),Y
   LDY   #$47
   STA   (FP),Y
   LDY   #$03
   LDA   (DPTR),Y
   LDY   #$48
   STA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@54
   DEY
   LDA   (FP),Y
   CMP   #$01
   BNE   .cmp_true@54
   DEY
   LDA   (FP),Y
   CMP   #$E2
   BNE   .cmp_true@54
   DEY
   LDA   (FP),Y
   CMP   #$40
   BNE   .cmp_true@54
   LDA   #$00
   JMP   .cmp_end@55
.cmp_true@54:
   LDA   #$01
.cmp_end@55:
   LDY   #$49
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@29
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   JMP   .or_end@30
.or_true@29:
   LDA   #$01
   LDY   #$4B
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@30:
   LDY   #$4B
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@3
   JMP   .or_true@27
.lb_skip@3:
   LDA   #<@12.stat
   LDY   #$4D
   STA   (FP),Y
   LDA   #>@12.stat
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$0D
   LDY   #$4F
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$50
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$51
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$52
   STA   (FP),Y
   LDA   #<@13.chr
   INY
   STA   (FP),Y
   LDA   #>@13.chr
   INY
   STA   (FP),Y
   LDY   #$52
   LDA   (FP),Y
   LDY   #$54
   CMP   (FP),Y
   BNE   .cmp_true@57
   LDY   #$51
   LDA   (FP),Y
   LDY   #$53
   CMP   (FP),Y
   BNE   .cmp_true@57
   LDA   #$00
   JMP   .cmp_end@58
.cmp_true@57:
   LDA   #$01
.cmp_end@58:
   LDY   #$55
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@27
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   JMP   .or_end@28
.or_true@27:
   LDA   #$01
   LDY   #$57
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@28:
   LDY   #$57
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@33
   LDX   #$00
   TXA

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$D9
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$D8
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
.if_end@33:
   LDA   #<@12.stat
   LDY   #$59
   STA   (FP),Y
   LDA   #>@12.stat
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
   LDA   #<@12.stat
   INY
   STA   (FP),Y
   LDA   #>@12.stat
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$08
   LDY   #$5F
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$60
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$61
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$62
   STA   (FP),Y
   LDY   #$5B
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$63
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$64
   STA   (FP),Y
   LDY   #$02
   LDA   (DPTR),Y
   LDY   #$65
   STA   (FP),Y
   LDY   #$03
   LDA   (DPTR),Y
   LDY   #$66
   STA   (FP),Y
   LDY   #$04
   LDA   (DPTR),Y
   LDY   #$67
   STA   (FP),Y
   LDY   #$05
   LDA   (DPTR),Y
   LDY   #$68
   STA   (FP),Y
   LDY   #$06
   LDA   (DPTR),Y
   LDY   #$69
   STA   (FP),Y
   LDY   #$07
   LDA   (DPTR),Y
   LDY   #$6A
   STA   (FP),Y
   CMP   #$42
   BNE   .cmp_true@60
   DEY
   LDA   (FP),Y
   CMP   #$8D
   BNE   .cmp_true@60
   DEY
   LDA   (FP),Y
   CMP   #$1A
   BNE   .cmp_true@60
   DEY
   LDA   (FP),Y
   CMP   #$94
   BNE   .cmp_true@60
   DEY
   LDA   (FP),Y
   CMP   #$A2
   BNE   .cmp_true@60
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@60
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@60
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@60
   LDA   #$00
   JMP   .cmp_end@61
.cmp_true@60:
   LDA   #$01
.cmp_end@61:
   LDY   #$6B
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@34
   LDY   #$61
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$6D
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CMP   #$7F
   BNE   .cmp_true@63
   LDA   #$00
   JMP   .cmp_end@64
.cmp_true@63:
   LDA   #$01
.cmp_end@64:
   LDY   #$70
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@34
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   JMP   .or_end@35
.or_true@34:
   LDA   #$01
   LDY   #$72
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@35:
   LDY   #$72
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@36
   LDX   #$00
   TXA

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$D9
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$D8
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
.if_end@36:
   LDA   #<@12.stat
   LDY   #$74
   STA   (FP),Y
   LDA   #>@12.stat
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$0D
   LDY   #$76
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$77
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$78
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$79
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$7A
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CMP   #$64
   BNE   .cmp_true@65
   LDA   #$00
   JMP   .cmp_end@66
.cmp_true@65:
   LDA   #$01
.cmp_end@66:
   LDY   #$7D
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@37
   LDX   #$00
   TXA

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$D9
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$D8
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
.if_end@37:
   LDA   #<@12.stat
   LDY   #$7F
   STA   (FP),Y
   LDA   #>@12.stat
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$81
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$82
   STA   (FP),Y
   LDY   #$02
   LDA   (DPTR),Y
   LDY   #$83
   STA   (FP),Y
   LDY   #$03
   LDA   (DPTR),Y
   LDY   #$84
   STA   (FP),Y
   LDY   #$04
   LDA   (DPTR),Y
   LDY   #$85
   STA   (FP),Y
   LDY   #$05
   LDA   (DPTR),Y
   LDY   #$86
   STA   (FP),Y
   LDY   #$06
   LDA   (DPTR),Y
   LDY   #$87
   STA   (FP),Y
   LDY   #$07
   LDA   (DPTR),Y
   LDY   #$88
   STA   (FP),Y
   LDY   #$81
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
   INY
   LDA   (FP),Y
   STA   HARGS+5
   INY
   LDA   (FP),Y
   STA   HARGS+6
   INY
   LDA   (FP),Y
   STA   HARGS+7
   LDA   #$00
   STA   HARGS+8
   LDY   #$00
   STY   HARGS+9
   STA   HARGS+10
   LDA   #$20
   STA   HARGS+11
   LDA   #$5F
   STA   HARGS+12
   LDA   #$A0
   STA   HARGS+13
   LDA   #$22
   STA   HARGS+14
   LDA   #$42
   STA   HARGS+15
   JSR   ddiv
   LDA   HARGS+16
   LDY   #$89
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
   LDA   HARGS+21
   INY
   STA   (FP),Y
   LDA   HARGS+22
   INY
   STA   (FP),Y
   LDA   HARGS+23
   INY
   STA   (FP),Y
   LDY   #$89
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
   INY
   LDA   (FP),Y
   STA   HARGS+5
   INY
   LDA   (FP),Y
   STA   HARGS+6
   INY
   LDA   (FP),Y
   STA   HARGS+7
   JSR   d2i
   LDA   HARGS+8
   LDY   #$91
   STA   (FP),Y
   LDA   HARGS+9
   INY
   STA   (FP),Y
   LDA   #<@12.stat
   INY
   STA   (FP),Y
   LDA   #>@12.stat
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$08
   LDY   #$95
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$96
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$97
   STA   (FP),Y
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
.mul_loop@67:
   BCC   .mul_skip@68
   CLC
   ADC   #$02
.mul_skip@68:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@67
   LDY   #$9B
   STA   (FP),Y
   LDA   HARGS
   DEY
   STA   (FP),Y
   LDA   #<@12.stat
   LDY   #$9C
   STA   (FP),Y
   LDA   #>@12.stat
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$09
   LDY   #$9E
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$9F
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$A0
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$A1
   STA   (FP),Y
   LDY   #$02
   LDA   (DPTR),Y
   LDY   #$A2
   STA   (FP),Y
   LDY   #$03
   LDA   (DPTR),Y
   LDY   #$A3
   STA   (FP),Y
   LDY   #$A0
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
   JSR   l2d
   LDA   HARGS+4
   LDY   #$A4
   STA   (FP),Y
   LDA   HARGS+5
   INY
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
   LDA   HARGS+9
   INY
   STA   (FP),Y
   LDA   HARGS+10
   INY
   STA   (FP),Y
   LDA   HARGS+11
   INY
   STA   (FP),Y
   LDA   #<@12.stat
   INY
   STA   (FP),Y
   LDA   #>@12.stat
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$0D
   LDY   #$AE
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$AF
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$B0
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$B1
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$B2
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDA   #<@12.stat
   INY
   STA   (FP),Y
   LDA   #>@12.stat
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$B7
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$B8
   STA   (FP),Y
   LDY   #$02
   LDA   (DPTR),Y
   LDY   #$B9
   STA   (FP),Y
   LDY   #$03
   LDA   (DPTR),Y
   LDY   #$BA
   STA   (FP),Y
   LDY   #$04
   LDA   (DPTR),Y
   LDY   #$BB
   STA   (FP),Y
   LDY   #$05
   LDA   (DPTR),Y
   LDY   #$BC
   STA   (FP),Y
   LDY   #$06
   LDA   (DPTR),Y
   LDY   #$BD
   STA   (FP),Y
   LDY   #$07
   LDA   (DPTR),Y
   LDY   #$BE
   STA   (FP),Y
   LDA   #<@12.stat
   INY
   STA   (FP),Y
   LDA   #>@12.stat
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$08
   LDY   #$C1
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$C2
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$C3
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDA   #<@12.stat
   INY
   STA   (FP),Y
   LDA   #>@12.stat
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$09
   LDY   #$C8
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$C9
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$CA
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$CB
   STA   (FP),Y
   LDY   #$02
   LDA   (DPTR),Y
   LDY   #$CC
   STA   (FP),Y
   LDY   #$03
   LDA   (DPTR),Y
   LDY   #$CD
   STA   (FP),Y
   LDA   #<@12.stat
   INY
   STA   (FP),Y
   LDA   #>@12.stat
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$0D
   LDY   #$D0
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$D1
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$D2
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$D3
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$1E
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDY   #$91
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$92
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$9A
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$9B
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   LDY   #$A4
   LDA   (FP),Y
   LDY   #$05
   STA   (SSP),Y
   LDY   #$A5
   LDA   (FP),Y
   LDY   #$06
   STA   (SSP),Y
   LDY   #$A6
   LDA   (FP),Y
   LDY   #$07
   STA   (SSP),Y
   LDY   #$A7
   LDA   (FP),Y
   LDY   #$08
   STA   (SSP),Y
   LDY   #$A8
   LDA   (FP),Y
   LDY   #$09
   STA   (SSP),Y
   LDY   #$A9
   LDA   (FP),Y
   LDY   #$0A
   STA   (SSP),Y
   LDY   #$AA
   LDA   (FP),Y
   LDY   #$0B
   STA   (SSP),Y
   LDY   #$AB
   LDA   (FP),Y
   LDY   #$0C
   STA   (SSP),Y
   LDY   #$B3
   LDA   (FP),Y
   LDY   #$0D
   STA   (SSP),Y
   LDY   #$B4
   LDA   (FP),Y
   LDY   #$0E
   STA   (SSP),Y
   LDY   #$B7
   LDA   (FP),Y
   LDY   #$0F
   STA   (SSP),Y
   LDY   #$B8
   LDA   (FP),Y
   LDY   #$10
   STA   (SSP),Y
   LDY   #$B9
   LDA   (FP),Y
   LDY   #$11
   STA   (SSP),Y
   LDY   #$BA
   LDA   (FP),Y
   LDY   #$12
   STA   (SSP),Y
   LDY   #$BB
   LDA   (FP),Y
   LDY   #$13
   STA   (SSP),Y
   LDY   #$BC
   LDA   (FP),Y
   LDY   #$14
   STA   (SSP),Y
   LDY   #$BD
   LDA   (FP),Y
   LDY   #$15
   STA   (SSP),Y
   LDY   #$BE
   LDA   (FP),Y
   LDY   #$16
   STA   (SSP),Y
   LDY   #$C4
   LDA   (FP),Y
   LDY   #$17
   STA   (SSP),Y
   LDY   #$C5
   LDA   (FP),Y
   LDY   #$18
   STA   (SSP),Y
   LDY   #$CA
   LDA   (FP),Y
   LDY   #$19
   STA   (SSP),Y
   LDY   #$CB
   LDA   (FP),Y
   LDY   #$1A
   STA   (SSP),Y
   LDY   #$CC
   LDA   (FP),Y
   LDY   #$1B
   STA   (SSP),Y
   LDY   #$CD
   LDA   (FP),Y
   LDY   #$1C
   STA   (SSP),Y
   LDY   #$D2
   LDA   (FP),Y
   LDY   #$1D
   STA   (SSP),Y
   LDY   #$D3
   LDA   (FP),Y
   LDY   #$1E
   STA   (SSP),Y
   JSR   accept_params
   LDY   #$D4
   STA   (FP),Y
   TXA
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lnot_true@69
   LDA   #$00
   JMP   .lnot_end@70
.lnot_true@69:
   LDA   #$01
.lnot_end@70:
   LDY   #$D6
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@38
   LDX   #$00
   TXA

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$D9
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
.if_end@38:
   LDX   #$00
   LDA   #$01

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$D9
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$D8
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

   ; prologue: 0 arg bytes, 8 local bytes
   SEC
   LDA   SSP
   SBC   #$0A
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$09
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   JSR   test_auto
   LDY   #$01
   STA   (FP),Y
   TXA
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lnot_true@71
   LDA   #$00
   JMP   .lnot_end@72
.lnot_true@71:
   LDA   #$01
.lnot_end@72:
   LDY   #$03
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@39
   LDX   #$00
   LDA   #$01

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$0A
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$09
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
.if_end@39:
   JSR   test_static
   LDY   #$05
   STA   (FP),Y
   TXA
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lnot_true@73
   LDA   #$00
   JMP   .lnot_end@74
.lnot_true@73:
   LDA   #$01
.lnot_end@74:
   LDY   #$07
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@40
   LDX   #$00
   LDA   #$02

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$0A
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
.if_end@40:
   LDX   #$00
   TXA

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$0A
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$09
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

l:
   DC.L  $0012D687

@12.stat:
   DS.B  15

@13.chr:
   DC.B  $64
