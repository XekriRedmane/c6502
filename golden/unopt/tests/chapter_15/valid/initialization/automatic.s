SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24

test_simple:
   SUBROUTINE

   ; prologue: 0 arg bytes, 82 local bytes
   SEC
   LDA   SSP
   SBC   #$54
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$53
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
   LDY   #$19
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
   LDA   #$FF
   LDY   #$00
   STA   (DPTR),Y
   INY
   STA   (DPTR),Y
   INY
   STA   (DPTR),Y
   INY
   STA   (DPTR),Y
   LDA   #$00
   INY
   STA   (DPTR),Y
   INY
   STA   (DPTR),Y
   INY
   STA   (DPTR),Y
   INY
   STA   (DPTR),Y
   LDY   #$19
   LDA   (FP),Y
   CLC
   ADC   #$08
   LDY   #$1B
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$1C
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDA   #$FF
   LDY   #$00
   STA   (DPTR),Y
   INY
   STA   (DPTR),Y
   INY
   STA   (DPTR),Y
   LDA   #$7F
   INY
   STA   (DPTR),Y
   LDA   #$00
   INY
   STA   (DPTR),Y
   INY
   STA   (DPTR),Y
   INY
   STA   (DPTR),Y
   INY
   STA   (DPTR),Y
   LDY   #$19
   LDA   (FP),Y
   CLC
   ADC   #$10
   LDY   #$1D
   STA   (FP),Y
   LDY   #$1A
   LDA   (FP),Y
   ADC   #$00
   LDY   #$1E
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDA   #$64
   LDY   #$00
   STA   (DPTR),Y
   LDA   #$00
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
   INY
   STA   (DPTR),Y
   INY
   STA   (DPTR),Y
   CLC
   LDA   FP
   ADC   #$01
   LDY   #$21
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   LDA   #$00
   STA   HARGS
   LDX   #$08
   LSR   HARGS
.mul_loop@0:
   BCC   .mul_skip@1
   CLC
   ADC   #$08
.mul_skip@1:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@0
   LDY   #$20
   STA   (FP),Y
   LDA   HARGS
   DEY
   STA   (FP),Y
   LDY   #$21
   LDA   (FP),Y
   CLC
   LDY   #$1F
   ADC   (FP),Y
   LDY   #$23
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$20
   ADC   (FP),Y
   LDY   #$24
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$25
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$26
   STA   (FP),Y
   LDY   #$02
   LDA   (DPTR),Y
   LDY   #$27
   STA   (FP),Y
   LDY   #$03
   LDA   (DPTR),Y
   LDY   #$28
   STA   (FP),Y
   LDY   #$04
   LDA   (DPTR),Y
   LDY   #$29
   STA   (FP),Y
   LDY   #$05
   LDA   (DPTR),Y
   LDY   #$2A
   STA   (FP),Y
   LDY   #$06
   LDA   (DPTR),Y
   LDY   #$2B
   STA   (FP),Y
   LDY   #$07
   LDA   (DPTR),Y
   LDY   #$2C
   STA   (FP),Y
   CMP   #$00
   BNE   .thread@0
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .thread@0
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .thread@0
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .thread@0
   DEY
   LDA   (FP),Y
   CMP   #$FF
   BNE   .thread@0
   DEY
   LDA   (FP),Y
   CMP   #$FF
   BNE   .thread@0
   DEY
   LDA   (FP),Y
   CMP   #$FF
   BNE   .thread@0
   DEY
   LDA   (FP),Y
   CMP   #$FF
   BEQ   .cmp_true@2
.thread@0:
   LDA   #$00
   JMP   .cmp_end@3
.cmp_true@2:
   LDA   #$01
.cmp_end@3:
   LDY   #$2D
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@1
   JMP   .and_false@2
.lb_skip@1:
   CLC
   LDA   FP
   ADC   #$01
   LDY   #$31
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   LDA   #$01
   STA   HARGS
   LDA   #$00
   LDX   #$08
   LSR   HARGS
.mul_loop@5:
   BCC   .mul_skip@6
   CLC
   ADC   #$08
.mul_skip@6:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@5
   LDY   #$30
   STA   (FP),Y
   LDA   HARGS
   DEY
   STA   (FP),Y
   LDY   #$31
   LDA   (FP),Y
   CLC
   LDY   #$2F
   ADC   (FP),Y
   LDY   #$33
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$30
   ADC   (FP),Y
   LDY   #$34
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$35
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$36
   STA   (FP),Y
   LDY   #$02
   LDA   (DPTR),Y
   LDY   #$37
   STA   (FP),Y
   LDY   #$03
   LDA   (DPTR),Y
   LDY   #$38
   STA   (FP),Y
   LDY   #$04
   LDA   (DPTR),Y
   LDY   #$39
   STA   (FP),Y
   LDY   #$05
   LDA   (DPTR),Y
   LDY   #$3A
   STA   (FP),Y
   LDY   #$06
   LDA   (DPTR),Y
   LDY   #$3B
   STA   (FP),Y
   LDY   #$07
   LDA   (DPTR),Y
   LDY   #$3C
   STA   (FP),Y
   CMP   #$00
   BNE   .thread@1
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .thread@1
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .thread@1
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .thread@1
   DEY
   LDA   (FP),Y
   CMP   #$7F
   BNE   .thread@1
   DEY
   LDA   (FP),Y
   CMP   #$FF
   BNE   .thread@1
   DEY
   LDA   (FP),Y
   CMP   #$FF
   BNE   .thread@1
   DEY
   LDA   (FP),Y
   CMP   #$FF
   BEQ   .cmp_true@7
.thread@1:
   LDA   #$00
   JMP   .cmp_end@8
.cmp_true@7:
   LDA   #$01
.cmp_end@8:
   LDY   #$3D
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
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
   LDY   #$3F
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@3:
   LDY   #$3F
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@0
   JMP   .and_false@0
.lb_skip@0:
   CLC
   LDA   FP
   ADC   #$01
   LDY   #$43
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   LDA   #$02
   STA   HARGS
   LDA   #$00
   LDX   #$08
   LSR   HARGS
.mul_loop@10:
   BCC   .mul_skip@11
   CLC
   ADC   #$08
.mul_skip@11:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@10
   LDY   #$42
   STA   (FP),Y
   LDA   HARGS
   DEY
   STA   (FP),Y
   LDY   #$43
   LDA   (FP),Y
   CLC
   LDY   #$41
   ADC   (FP),Y
   LDY   #$45
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$42
   ADC   (FP),Y
   LDY   #$46
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$47
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$48
   STA   (FP),Y
   LDY   #$02
   LDA   (DPTR),Y
   LDY   #$49
   STA   (FP),Y
   LDY   #$03
   LDA   (DPTR),Y
   LDY   #$4A
   STA   (FP),Y
   LDY   #$04
   LDA   (DPTR),Y
   LDY   #$4B
   STA   (FP),Y
   LDY   #$05
   LDA   (DPTR),Y
   LDY   #$4C
   STA   (FP),Y
   LDY   #$06
   LDA   (DPTR),Y
   LDY   #$4D
   STA   (FP),Y
   LDY   #$07
   LDA   (DPTR),Y
   LDY   #$4E
   STA   (FP),Y
   CMP   #$00
   BNE   .thread@2
   DEY
   LDA   (FP),Y
   BNE   .thread@2
   DEY
   LDA   (FP),Y
   BNE   .thread@2
   DEY
   LDA   (FP),Y
   BNE   .thread@2
   DEY
   LDA   (FP),Y
   BNE   .thread@2
   DEY
   LDA   (FP),Y
   BNE   .thread@2
   DEY
   LDA   (FP),Y
   BNE   .thread@2
   DEY
   LDA   (FP),Y
   CMP   #$64
   BEQ   .cmp_true@12
.thread@2:
   LDA   #$00
   JMP   .cmp_end@13
.cmp_true@12:
   LDA   #$01
.cmp_end@13:
   LDY   #$4F
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
   LDY   #$51
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@1:
   TAX
   LDY   #$51
   LDA   (FP),Y

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$54
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$53
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

test_partial:
   SUBROUTINE

   ; prologue: 0 arg bytes, 141 local bytes
   SEC
   LDA   SSP
   SBC   #$8F
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$8E
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
   LDY   #$29
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
   LDA   #$F0
   INY
   STA   (DPTR),Y
   LDA   #$3F
   INY
   STA   (DPTR),Y
   LDY   #$29
   LDA   (FP),Y
   CLC
   ADC   #$08
   LDY   #$2B
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$2C
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
   LDA   #$B0
   INY
   STA   (DPTR),Y
   LDA   #$C4
   INY
   STA   (DPTR),Y
   LDA   #$32
   INY
   STA   (DPTR),Y
   LDA   #$41
   INY
   STA   (DPTR),Y
   LDA   #$18
   LDY   #$2D
   STA   (FP),Y
.clear@4:
   SEC
   SBC   #$01
   LDY   #$2D
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
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
   CLC
   ADC   #$10
   LDY   #$32
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$33
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   LDY   #$2E
   ADC   (FP),Y
   LDY   #$34
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$2F
   ADC   (FP),Y
   LDY   #$35
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
   LDY   #$2D
   LDA   (FP),Y
   BNE   .clear@4
   CLC
   LDA   FP
   ADC   #$01
   LDY   #$38
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   LDA   #$00
   STA   HARGS
   LDX   #$08
   LSR   HARGS
.mul_loop@15:
   BCC   .mul_skip@16
   CLC
   ADC   #$08
.mul_skip@16:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@15
   LDY   #$37
   STA   (FP),Y
   LDA   HARGS
   DEY
   STA   (FP),Y
   LDY   #$38
   LDA   (FP),Y
   CLC
   LDY   #$36
   ADC   (FP),Y
   LDY   #$3A
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$37
   ADC   (FP),Y
   LDY   #$3B
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$3C
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$3D
   STA   (FP),Y
   LDY   #$02
   LDA   (DPTR),Y
   LDY   #$3E
   STA   (FP),Y
   LDY   #$03
   LDA   (DPTR),Y
   LDY   #$3F
   STA   (FP),Y
   LDY   #$04
   LDA   (DPTR),Y
   LDY   #$40
   STA   (FP),Y
   LDY   #$05
   LDA   (DPTR),Y
   LDY   #$41
   STA   (FP),Y
   LDY   #$06
   LDA   (DPTR),Y
   LDY   #$42
   STA   (FP),Y
   LDY   #$07
   LDA   (DPTR),Y
   LDY   #$43
   STA   (FP),Y
   CMP   #$3F
   BNE   .thread@3
   DEY
   LDA   (FP),Y
   CMP   #$F0
   BNE   .thread@3
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .thread@3
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .thread@3
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .thread@3
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .thread@3
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .thread@3
   DEY
   LDA   (FP),Y
   CMP   #$00
   BEQ   .cmp_true@17
.thread@3:
   LDA   #$00
   JMP   .cmp_end@18
.cmp_true@17:
   LDA   #$01
.cmp_end@18:
   LDY   #$44
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@5
   JMP   .and_false@11
.lb_skip@5:
   CLC
   LDA   FP
   ADC   #$01
   LDY   #$48
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   LDA   #$01
   STA   HARGS
   LDA   #$00
   LDX   #$08
   LSR   HARGS
.mul_loop@20:
   BCC   .mul_skip@21
   CLC
   ADC   #$08
.mul_skip@21:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@20
   LDY   #$47
   STA   (FP),Y
   LDA   HARGS
   DEY
   STA   (FP),Y
   LDY   #$48
   LDA   (FP),Y
   CLC
   LDY   #$46
   ADC   (FP),Y
   LDY   #$4A
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$47
   ADC   (FP),Y
   LDY   #$4B
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$4C
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$4D
   STA   (FP),Y
   LDY   #$02
   LDA   (DPTR),Y
   LDY   #$4E
   STA   (FP),Y
   LDY   #$03
   LDA   (DPTR),Y
   LDY   #$4F
   STA   (FP),Y
   LDY   #$04
   LDA   (DPTR),Y
   LDY   #$50
   STA   (FP),Y
   LDY   #$05
   LDA   (DPTR),Y
   LDY   #$51
   STA   (FP),Y
   LDY   #$06
   LDA   (DPTR),Y
   LDY   #$52
   STA   (FP),Y
   LDY   #$07
   LDA   (DPTR),Y
   LDY   #$53
   STA   (FP),Y
   CMP   #$41
   BNE   .thread@4
   DEY
   LDA   (FP),Y
   CMP   #$32
   BNE   .thread@4
   DEY
   LDA   (FP),Y
   CMP   #$C4
   BNE   .thread@4
   DEY
   LDA   (FP),Y
   CMP   #$B0
   BNE   .thread@4
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .thread@4
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .thread@4
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .thread@4
   DEY
   LDA   (FP),Y
   CMP   #$00
   BEQ   .cmp_true@22
.thread@4:
   LDA   #$00
   JMP   .cmp_end@23
.cmp_true@22:
   LDA   #$01
.cmp_end@23:
   LDY   #$54
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@11
   LDA   #$01
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   JMP   .and_end@12
.and_false@11:
   LDA   #$00
   LDY   #$56
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@12:
   LDY   #$56
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@4
   JMP   .and_false@9
.lb_skip@4:
   CLC
   LDA   FP
   ADC   #$01
   LDY   #$5A
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   LDA   #$02
   STA   HARGS
   LDA   #$00
   LDX   #$08
   LSR   HARGS
.mul_loop@25:
   BCC   .mul_skip@26
   CLC
   ADC   #$08
.mul_skip@26:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@25
   LDY   #$59
   STA   (FP),Y
   LDA   HARGS
   DEY
   STA   (FP),Y
   LDY   #$5A
   LDA   (FP),Y
   CLC
   LDY   #$58
   ADC   (FP),Y
   LDY   #$5C
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$59
   ADC   (FP),Y
   LDY   #$5D
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$5E
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$5F
   STA   (FP),Y
   LDY   #$02
   LDA   (DPTR),Y
   LDY   #$60
   STA   (FP),Y
   LDY   #$03
   LDA   (DPTR),Y
   LDY   #$61
   STA   (FP),Y
   LDY   #$04
   LDA   (DPTR),Y
   LDY   #$62
   STA   (FP),Y
   LDY   #$05
   LDA   (DPTR),Y
   LDY   #$63
   STA   (FP),Y
   LDY   #$06
   LDA   (DPTR),Y
   LDY   #$64
   STA   (FP),Y
   LDY   #$07
   LDA   (DPTR),Y
   LDY   #$65
   STA   (FP),Y
   LDY   #$5E
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   INY
   ORA   (FP),Y
   INY
   ORA   (FP),Y
   INY
   ORA   (FP),Y
   INY
   ORA   (FP),Y
   INY
   ORA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lnot_true@27
   LDA   #$00
   JMP   .lnot_end@28
.lnot_true@27:
   LDA   #$01
.lnot_end@28:
   LDY   #$66
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@9
   LDA   #$01
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   JMP   .and_end@10
.and_false@9:
   LDA   #$00
   LDY   #$68
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@10:
   LDY   #$68
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@3
   JMP   .and_false@7
.lb_skip@3:
   CLC
   LDA   FP
   ADC   #$01
   LDY   #$6C
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   LDA   #$03
   STA   HARGS
   LDA   #$00
   LDX   #$08
   LSR   HARGS
.mul_loop@29:
   BCC   .mul_skip@30
   CLC
   ADC   #$08
.mul_skip@30:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@29
   LDY   #$6B
   STA   (FP),Y
   LDA   HARGS
   DEY
   STA   (FP),Y
   LDY   #$6C
   LDA   (FP),Y
   CLC
   LDY   #$6A
   ADC   (FP),Y
   LDY   #$6E
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$6B
   ADC   (FP),Y
   LDY   #$6F
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$70
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$71
   STA   (FP),Y
   LDY   #$02
   LDA   (DPTR),Y
   LDY   #$72
   STA   (FP),Y
   LDY   #$03
   LDA   (DPTR),Y
   LDY   #$73
   STA   (FP),Y
   LDY   #$04
   LDA   (DPTR),Y
   LDY   #$74
   STA   (FP),Y
   LDY   #$05
   LDA   (DPTR),Y
   LDY   #$75
   STA   (FP),Y
   LDY   #$06
   LDA   (DPTR),Y
   LDY   #$76
   STA   (FP),Y
   LDY   #$07
   LDA   (DPTR),Y
   LDY   #$77
   STA   (FP),Y
   LDY   #$70
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   INY
   ORA   (FP),Y
   INY
   ORA   (FP),Y
   INY
   ORA   (FP),Y
   INY
   ORA   (FP),Y
   INY
   ORA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lnot_true@31
   LDA   #$00
   JMP   .lnot_end@32
.lnot_true@31:
   LDA   #$01
.lnot_end@32:
   LDY   #$78
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@7
   LDA   #$01
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   JMP   .and_end@8
.and_false@7:
   LDA   #$00
   LDY   #$7A
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@8:
   LDY   #$7A
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@2
   JMP   .and_false@5
.lb_skip@2:
   CLC
   LDA   FP
   ADC   #$01
   LDY   #$7E
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   LDA   #$04
   STA   HARGS
   LDA   #$00
   LDX   #$08
   LSR   HARGS
.mul_loop@33:
   BCC   .mul_skip@34
   CLC
   ADC   #$08
.mul_skip@34:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@33
   LDY   #$7D
   STA   (FP),Y
   LDA   HARGS
   DEY
   STA   (FP),Y
   LDY   #$7E
   LDA   (FP),Y
   CLC
   LDY   #$7C
   ADC   (FP),Y
   LDY   #$80
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$7D
   ADC   (FP),Y
   LDY   #$81
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$82
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$83
   STA   (FP),Y
   LDY   #$02
   LDA   (DPTR),Y
   LDY   #$84
   STA   (FP),Y
   LDY   #$03
   LDA   (DPTR),Y
   LDY   #$85
   STA   (FP),Y
   LDY   #$04
   LDA   (DPTR),Y
   LDY   #$86
   STA   (FP),Y
   LDY   #$05
   LDA   (DPTR),Y
   LDY   #$87
   STA   (FP),Y
   LDY   #$06
   LDA   (DPTR),Y
   LDY   #$88
   STA   (FP),Y
   LDY   #$07
   LDA   (DPTR),Y
   LDY   #$89
   STA   (FP),Y
   LDY   #$82
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   INY
   ORA   (FP),Y
   INY
   ORA   (FP),Y
   INY
   ORA   (FP),Y
   INY
   ORA   (FP),Y
   INY
   ORA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lnot_true@35
   LDA   #$00
   JMP   .lnot_end@36
.lnot_true@35:
   LDA   #$01
.lnot_end@36:
   LDY   #$8A
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@5
   LDA   #$01
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   JMP   .and_end@6
.and_false@5:
   LDA   #$00
   LDY   #$8C
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@6:
   TAX
   LDY   #$8C
   LDA   (FP),Y

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$8F
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$8E
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

test_non_constant:
   SUBROUTINE

   ; prologue: 10 arg bytes, 242 local bytes
   SEC
   LDA   SSP
   SBC   #$F4
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$F3
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   LDY   #$FD
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDA   #$01
   LDY   #$00
   STA   (DPTR),Y
   LDA   #$00
   INY
   STA   (DPTR),Y
   JSR   three
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
   BMI   .sx_neg@37
   LDA   #$00
   JMP   .sx_done@38
.sx_neg@37:
   LDA   #$FF
.sx_done@38:
   LDY   #$05
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
   LDY   #$F5
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
   LDY   #$03
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
   INY
   LDA   (FP),Y
   STA   HARGS+13
   INY
   LDA   (FP),Y
   STA   HARGS+14
   INY
   LDA   (FP),Y
   STA   HARGS+15
   JSR   mul64
   LDA   HARGS+16
   LDY   #$0B
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
   LDY   #$0B
   LDA   (FP),Y
   LDY   #$13
   STA   (FP),Y
   LDY   #$0C
   LDA   (FP),Y
   LDY   #$14
   STA   (FP),Y
   LDY   #$0D
   LDA   (FP),Y
   LDY   #$15
   STA   (FP),Y
   LDY   #$0E
   LDA   (FP),Y
   LDY   #$16
   STA   (FP),Y
   LDY   #$0F
   LDA   (FP),Y
   LDY   #$17
   STA   (FP),Y
   LDY   #$10
   LDA   (FP),Y
   LDY   #$18
   STA   (FP),Y
   LDY   #$11
   LDA   (FP),Y
   LDY   #$19
   STA   (FP),Y
   LDY   #$12
   LDA   (FP),Y
   LDY   #$1A
   STA   (FP),Y
   CLC
   LDA   FP
   ADC   #$1B
   LDY   #$43
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
   LDY   #$F5
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$F6
   LDA   (FP),Y
   LDY   #$01
   STA   (DPTR),Y
   LDY   #$F7
   LDA   (FP),Y
   LDY   #$02
   STA   (DPTR),Y
   LDY   #$F8
   LDA   (FP),Y
   LDY   #$03
   STA   (DPTR),Y
   LDY   #$F9
   LDA   (FP),Y
   LDY   #$04
   STA   (DPTR),Y
   LDY   #$FA
   LDA   (FP),Y
   LDY   #$05
   STA   (DPTR),Y
   LDY   #$FB
   LDA   (FP),Y
   LDY   #$06
   STA   (DPTR),Y
   LDY   #$FC
   LDA   (FP),Y
   LDY   #$07
   STA   (DPTR),Y
   JSR   three
   LDY   #$45
   STA   (FP),Y
   TXA
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$47
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$48
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@39
   LDA   #$00
   JMP   .sx_done@40
.sx_neg@39:
   LDA   #$FF
.sx_done@40:
   LDY   #$49
   STA   (FP),Y
   INY
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
   LDA   #$07
   STA   HARGS+4
   LDA   #$00
   STA   HARGS+5
   LDY   #$00
   STY   HARGS+6
   STA   HARGS+7
   JSR   mul32
   LDA   HARGS+8
   LDY   #$4B
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
   LDY   #$4B
   LDA   (FP),Y
   LDY   #$4F
   STA   (FP),Y
   LDY   #$4C
   LDA   (FP),Y
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
   ORA   #$00
   BMI   .sx_neg@41
   LDA   #$00
   JMP   .sx_done@42
.sx_neg@41:
   LDA   #$FF
.sx_done@42:
   LDY   #$53
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$43
   LDA   (FP),Y
   CLC
   ADC   #$08
   LDY   #$57
   STA   (FP),Y
   LDY   #$44
   LDA   (FP),Y
   ADC   #$00
   LDY   #$58
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$4F
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$50
   LDA   (FP),Y
   LDY   #$01
   STA   (DPTR),Y
   LDY   #$51
   LDA   (FP),Y
   LDY   #$02
   STA   (DPTR),Y
   LDY   #$52
   LDA   (FP),Y
   LDY   #$03
   STA   (DPTR),Y
   LDY   #$53
   LDA   (FP),Y
   LDY   #$04
   STA   (DPTR),Y
   LDY   #$54
   LDA   (FP),Y
   LDY   #$05
   STA   (DPTR),Y
   LDY   #$55
   LDA   (FP),Y
   LDY   #$06
   STA   (DPTR),Y
   LDY   #$56
   LDA   (FP),Y
   LDY   #$07
   STA   (DPTR),Y
   LDY   #$FD
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
   LDY   #$5B
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$5C
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@43
   LDA   #$00
   JMP   .sx_done@44
.sx_neg@43:
   LDA   #$FF
.sx_done@44:
   LDY   #$5D
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
   SEC
   LDA   #$00
   LDY   #$5B
   SBC   (FP),Y
   LDY   #$63
   STA   (FP),Y
   LDA   #$00
   LDY   #$5C
   SBC   (FP),Y
   LDY   #$64
   STA   (FP),Y
   LDA   #$00
   LDY   #$5D
   SBC   (FP),Y
   LDY   #$65
   STA   (FP),Y
   LDA   #$00
   LDY   #$5E
   SBC   (FP),Y
   LDY   #$66
   STA   (FP),Y
   LDA   #$00
   LDY   #$5F
   SBC   (FP),Y
   LDY   #$67
   STA   (FP),Y
   LDA   #$00
   LDY   #$60
   SBC   (FP),Y
   LDY   #$68
   STA   (FP),Y
   LDA   #$00
   LDY   #$61
   SBC   (FP),Y
   LDY   #$69
   STA   (FP),Y
   LDA   #$00
   LDY   #$62
   SBC   (FP),Y
   LDY   #$6A
   STA   (FP),Y
   LDY   #$43
   LDA   (FP),Y
   CLC
   ADC   #$10
   LDY   #$6B
   STA   (FP),Y
   LDY   #$44
   LDA   (FP),Y
   ADC   #$00
   LDY   #$6C
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$63
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$64
   LDA   (FP),Y
   LDY   #$01
   STA   (DPTR),Y
   LDY   #$65
   LDA   (FP),Y
   LDY   #$02
   STA   (DPTR),Y
   LDY   #$66
   LDA   (FP),Y
   LDY   #$03
   STA   (DPTR),Y
   LDY   #$67
   LDA   (FP),Y
   LDY   #$04
   STA   (DPTR),Y
   LDY   #$68
   LDA   (FP),Y
   LDY   #$05
   STA   (DPTR),Y
   LDY   #$69
   LDA   (FP),Y
   LDY   #$06
   STA   (DPTR),Y
   LDY   #$6A
   LDA   (FP),Y
   LDY   #$07
   STA   (DPTR),Y
   LDY   #$F5
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   INY
   ORA   (FP),Y
   INY
   ORA   (FP),Y
   INY
   ORA   (FP),Y
   INY
   ORA   (FP),Y
   INY
   ORA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .cond_else@13
   LDA   #$02
   LDY   #$6D
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   JMP   .cond_end@14
.cond_else@13:
   LDA   #$03
   LDY   #$6D
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.cond_end@14:
   LDY   #$6D
   LDA   (FP),Y
   LDY   #$6F
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$70
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@45
   LDA   #$00
   JMP   .sx_done@46
.sx_neg@45:
   LDA   #$FF
.sx_done@46:
   LDY   #$71
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
   LDY   #$13
   LDA   (FP),Y
   CLC
   LDY   #$6F
   ADC   (FP),Y
   LDY   #$77
   STA   (FP),Y
   LDY   #$14
   LDA   (FP),Y
   LDY   #$70
   ADC   (FP),Y
   LDY   #$78
   STA   (FP),Y
   LDY   #$15
   LDA   (FP),Y
   LDY   #$71
   ADC   (FP),Y
   LDY   #$79
   STA   (FP),Y
   LDY   #$16
   LDA   (FP),Y
   LDY   #$72
   ADC   (FP),Y
   LDY   #$7A
   STA   (FP),Y
   LDY   #$17
   LDA   (FP),Y
   LDY   #$73
   ADC   (FP),Y
   LDY   #$7B
   STA   (FP),Y
   LDY   #$18
   LDA   (FP),Y
   LDY   #$74
   ADC   (FP),Y
   LDY   #$7C
   STA   (FP),Y
   LDY   #$19
   LDA   (FP),Y
   LDY   #$75
   ADC   (FP),Y
   LDY   #$7D
   STA   (FP),Y
   LDY   #$1A
   LDA   (FP),Y
   LDY   #$76
   ADC   (FP),Y
   LDY   #$7E
   STA   (FP),Y
   LDY   #$43
   LDA   (FP),Y
   CLC
   ADC   #$18
   LDY   #$7F
   STA   (FP),Y
   LDY   #$44
   LDA   (FP),Y
   ADC   #$00
   LDY   #$80
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$77
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$78
   LDA   (FP),Y
   LDY   #$01
   STA   (DPTR),Y
   LDY   #$79
   LDA   (FP),Y
   LDY   #$02
   STA   (DPTR),Y
   LDY   #$7A
   LDA   (FP),Y
   LDY   #$03
   STA   (DPTR),Y
   LDY   #$7B
   LDA   (FP),Y
   LDY   #$04
   STA   (DPTR),Y
   LDY   #$7C
   LDA   (FP),Y
   LDY   #$05
   STA   (DPTR),Y
   LDY   #$7D
   LDA   (FP),Y
   LDY   #$06
   STA   (DPTR),Y
   LDY   #$7E
   LDA   (FP),Y
   LDY   #$07
   STA   (DPTR),Y
   LDY   #$43
   LDA   (FP),Y
   CLC
   ADC   #$20
   LDY   #$81
   STA   (FP),Y
   LDY   #$44
   LDA   (FP),Y
   ADC   #$00
   LDY   #$82
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
   INY
   STA   (DPTR),Y
   INY
   STA   (DPTR),Y
   CLC
   LDA   FP
   ADC   #$1B
   LDY   #$85
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   LDA   #$00
   STA   HARGS
   LDX   #$08
   LSR   HARGS
.mul_loop@47:
   BCC   .mul_skip@48
   CLC
   ADC   #$08
.mul_skip@48:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@47
   LDY   #$84
   STA   (FP),Y
   LDA   HARGS
   DEY
   STA   (FP),Y
   LDY   #$85
   LDA   (FP),Y
   CLC
   LDY   #$83
   ADC   (FP),Y
   LDY   #$87
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$84
   ADC   (FP),Y
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
   LDY   #$02
   LDA   (DPTR),Y
   LDY   #$8B
   STA   (FP),Y
   LDY   #$03
   LDA   (DPTR),Y
   LDY   #$8C
   STA   (FP),Y
   LDY   #$04
   LDA   (DPTR),Y
   LDY   #$8D
   STA   (FP),Y
   LDY   #$05
   LDA   (DPTR),Y
   LDY   #$8E
   STA   (FP),Y
   LDY   #$06
   LDA   (DPTR),Y
   LDY   #$8F
   STA   (FP),Y
   LDY   #$07
   LDA   (DPTR),Y
   LDY   #$90
   STA   (FP),Y
   SEC
   LDA   #$00
   SBC   #$00
   INY
   STA   (FP),Y
   LDA   #$00
   SBC   #$27
   INY
   STA   (FP),Y
   LDA   #$00
   SBC   #$B9
   INY
   STA   (FP),Y
   LDA   #$00
   SBC   #$29
   INY
   STA   (FP),Y
   LDA   #$00
   SBC   #$00
   INY
   STA   (FP),Y
   LDA   #$00
   SBC   #$00
   INY
   STA   (FP),Y
   LDA   #$00
   SBC   #$00
   INY
   STA   (FP),Y
   LDA   #$00
   SBC   #$00
   INY
   STA   (FP),Y
   LDY   #$90
   LDA   (FP),Y
   LDY   #$98
   CMP   (FP),Y
   BNE   .thread@5
   LDY   #$8F
   LDA   (FP),Y
   LDY   #$97
   CMP   (FP),Y
   BNE   .thread@5
   LDY   #$8E
   LDA   (FP),Y
   LDY   #$96
   CMP   (FP),Y
   BNE   .thread@5
   LDY   #$8D
   LDA   (FP),Y
   LDY   #$95
   CMP   (FP),Y
   BNE   .thread@5
   LDY   #$8C
   LDA   (FP),Y
   LDY   #$94
   CMP   (FP),Y
   BNE   .thread@5
   LDY   #$8B
   LDA   (FP),Y
   LDY   #$93
   CMP   (FP),Y
   BNE   .thread@5
   LDY   #$8A
   LDA   (FP),Y
   LDY   #$92
   CMP   (FP),Y
   BNE   .thread@5
   LDY   #$89
   LDA   (FP),Y
   LDY   #$91
   CMP   (FP),Y
   BEQ   .cmp_true@49
.thread@5:
   LDA   #$00
   JMP   .cmp_end@50
.cmp_true@49:
   LDA   #$01
.cmp_end@50:
   LDY   #$99
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@9
   JMP   .and_false@21
.lb_skip@9:
   CLC
   LDA   FP
   ADC   #$1B
   LDY   #$9D
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   LDA   #$01
   STA   HARGS
   LDA   #$00
   LDX   #$08
   LSR   HARGS
.mul_loop@52:
   BCC   .mul_skip@53
   CLC
   ADC   #$08
.mul_skip@53:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@52
   LDY   #$9C
   STA   (FP),Y
   LDA   HARGS
   DEY
   STA   (FP),Y
   LDY   #$9D
   LDA   (FP),Y
   CLC
   LDY   #$9B
   ADC   (FP),Y
   LDY   #$9F
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$9C
   ADC   (FP),Y
   LDY   #$A0
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$A1
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$A2
   STA   (FP),Y
   LDY   #$02
   LDA   (DPTR),Y
   LDY   #$A3
   STA   (FP),Y
   LDY   #$03
   LDA   (DPTR),Y
   LDY   #$A4
   STA   (FP),Y
   LDY   #$04
   LDA   (DPTR),Y
   LDY   #$A5
   STA   (FP),Y
   LDY   #$05
   LDA   (DPTR),Y
   LDY   #$A6
   STA   (FP),Y
   LDY   #$06
   LDA   (DPTR),Y
   LDY   #$A7
   STA   (FP),Y
   LDY   #$07
   LDA   (DPTR),Y
   LDY   #$A8
   STA   (FP),Y
   CMP   #$00
   BNE   .thread@6
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .thread@6
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .thread@6
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .thread@6
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .thread@6
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .thread@6
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .thread@6
   DEY
   LDA   (FP),Y
   CMP   #$15
   BEQ   .cmp_true@54
.thread@6:
   LDA   #$00
   JMP   .cmp_end@55
.cmp_true@54:
   LDA   #$01
.cmp_end@55:
   LDY   #$A9
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@21
   LDA   #$01
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   JMP   .and_end@22
.and_false@21:
   LDA   #$00
   LDY   #$AB
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@22:
   LDY   #$AB
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@8
   JMP   .and_false@19
.lb_skip@8:
   CLC
   LDA   FP
   ADC   #$1B
   LDY   #$AF
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   LDA   #$02
   STA   HARGS
   LDA   #$00
   LDX   #$08
   LSR   HARGS
.mul_loop@57:
   BCC   .mul_skip@58
   CLC
   ADC   #$08
.mul_skip@58:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@57
   LDY   #$AE
   STA   (FP),Y
   LDA   HARGS
   DEY
   STA   (FP),Y
   LDY   #$AF
   LDA   (FP),Y
   CLC
   LDY   #$AD
   ADC   (FP),Y
   LDY   #$B1
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$AE
   ADC   (FP),Y
   LDY   #$B2
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$B3
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$B4
   STA   (FP),Y
   LDY   #$02
   LDA   (DPTR),Y
   LDY   #$B5
   STA   (FP),Y
   LDY   #$03
   LDA   (DPTR),Y
   LDY   #$B6
   STA   (FP),Y
   LDY   #$04
   LDA   (DPTR),Y
   LDY   #$B7
   STA   (FP),Y
   LDY   #$05
   LDA   (DPTR),Y
   LDY   #$B8
   STA   (FP),Y
   LDY   #$06
   LDA   (DPTR),Y
   LDY   #$B9
   STA   (FP),Y
   LDY   #$07
   LDA   (DPTR),Y
   LDY   #$BA
   STA   (FP),Y
   SEC
   LDA   #$00
   SBC   #$01
   INY
   STA   (FP),Y
   LDA   #$00
   SBC   #$00
   INY
   STA   (FP),Y
   LDA   #$00
   SBC   #$00
   INY
   STA   (FP),Y
   LDA   #$00
   SBC   #$00
   INY
   STA   (FP),Y
   LDA   #$00
   SBC   #$00
   INY
   STA   (FP),Y
   LDA   #$00
   SBC   #$00
   INY
   STA   (FP),Y
   LDA   #$00
   SBC   #$00
   INY
   STA   (FP),Y
   LDA   #$00
   SBC   #$00
   INY
   STA   (FP),Y
   LDY   #$BA
   LDA   (FP),Y
   LDY   #$C2
   CMP   (FP),Y
   BNE   .thread@7
   LDY   #$B9
   LDA   (FP),Y
   LDY   #$C1
   CMP   (FP),Y
   BNE   .thread@7
   LDY   #$B8
   LDA   (FP),Y
   LDY   #$C0
   CMP   (FP),Y
   BNE   .thread@7
   LDY   #$B7
   LDA   (FP),Y
   LDY   #$BF
   CMP   (FP),Y
   BNE   .thread@7
   LDY   #$B6
   LDA   (FP),Y
   LDY   #$BE
   CMP   (FP),Y
   BNE   .thread@7
   LDY   #$B5
   LDA   (FP),Y
   LDY   #$BD
   CMP   (FP),Y
   BNE   .thread@7
   LDY   #$B4
   LDA   (FP),Y
   LDY   #$BC
   CMP   (FP),Y
   BNE   .thread@7
   LDY   #$B3
   LDA   (FP),Y
   LDY   #$BB
   CMP   (FP),Y
   BEQ   .cmp_true@59
.thread@7:
   LDA   #$00
   JMP   .cmp_end@60
.cmp_true@59:
   LDA   #$01
.cmp_end@60:
   LDY   #$C3
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@19
   LDA   #$01
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   JMP   .and_end@20
.and_false@19:
   LDA   #$00
   LDY   #$C5
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@20:
   LDY   #$C5
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@7
   JMP   .and_false@17
.lb_skip@7:
   CLC
   LDA   FP
   ADC   #$1B
   LDY   #$C9
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   LDA   #$03
   STA   HARGS
   LDA   #$00
   LDX   #$08
   LSR   HARGS
.mul_loop@62:
   BCC   .mul_skip@63
   CLC
   ADC   #$08
.mul_skip@63:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@62
   LDY   #$C8
   STA   (FP),Y
   LDA   HARGS
   DEY
   STA   (FP),Y
   LDY   #$C9
   LDA   (FP),Y
   CLC
   LDY   #$C7
   ADC   (FP),Y
   LDY   #$CB
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$C8
   ADC   (FP),Y
   LDY   #$CC
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$CD
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$CE
   STA   (FP),Y
   LDY   #$02
   LDA   (DPTR),Y
   LDY   #$CF
   STA   (FP),Y
   LDY   #$03
   LDA   (DPTR),Y
   LDY   #$D0
   STA   (FP),Y
   LDY   #$04
   LDA   (DPTR),Y
   LDY   #$D1
   STA   (FP),Y
   LDY   #$05
   LDA   (DPTR),Y
   LDY   #$D2
   STA   (FP),Y
   LDY   #$06
   LDA   (DPTR),Y
   LDY   #$D3
   STA   (FP),Y
   LDY   #$07
   LDA   (DPTR),Y
   LDY   #$D4
   STA   (FP),Y
   SEC
   LDA   #$00
   SBC   #$FE
   INY
   STA   (FP),Y
   LDA   #$00
   SBC   #$74
   INY
   STA   (FP),Y
   LDA   #$00
   SBC   #$2B
   INY
   STA   (FP),Y
   LDA   #$00
   SBC   #$7D
   INY
   STA   (FP),Y
   LDA   #$00
   SBC   #$00
   INY
   STA   (FP),Y
   LDA   #$00
   SBC   #$00
   INY
   STA   (FP),Y
   LDA   #$00
   SBC   #$00
   INY
   STA   (FP),Y
   LDA   #$00
   SBC   #$00
   INY
   STA   (FP),Y
   LDY   #$D4
   LDA   (FP),Y
   LDY   #$DC
   CMP   (FP),Y
   BNE   .thread@8
   LDY   #$D3
   LDA   (FP),Y
   LDY   #$DB
   CMP   (FP),Y
   BNE   .thread@8
   LDY   #$D2
   LDA   (FP),Y
   LDY   #$DA
   CMP   (FP),Y
   BNE   .thread@8
   LDY   #$D1
   LDA   (FP),Y
   LDY   #$D9
   CMP   (FP),Y
   BNE   .thread@8
   LDY   #$D0
   LDA   (FP),Y
   LDY   #$D8
   CMP   (FP),Y
   BNE   .thread@8
   LDY   #$CF
   LDA   (FP),Y
   LDY   #$D7
   CMP   (FP),Y
   BNE   .thread@8
   LDY   #$CE
   LDA   (FP),Y
   LDY   #$D6
   CMP   (FP),Y
   BNE   .thread@8
   LDY   #$CD
   LDA   (FP),Y
   LDY   #$D5
   CMP   (FP),Y
   BEQ   .cmp_true@64
.thread@8:
   LDA   #$00
   JMP   .cmp_end@65
.cmp_true@64:
   LDA   #$01
.cmp_end@65:
   LDY   #$DD
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
   LDY   #$DF
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@18:
   LDY   #$DF
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@6
   JMP   .and_false@15
.lb_skip@6:
   CLC
   LDA   FP
   ADC   #$1B
   LDY   #$E3
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   LDA   #$04
   STA   HARGS
   LDA   #$00
   LDX   #$08
   LSR   HARGS
.mul_loop@67:
   BCC   .mul_skip@68
   CLC
   ADC   #$08
.mul_skip@68:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@67
   LDY   #$E2
   STA   (FP),Y
   LDA   HARGS
   DEY
   STA   (FP),Y
   LDY   #$E3
   LDA   (FP),Y
   CLC
   LDY   #$E1
   ADC   (FP),Y
   LDY   #$E5
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$E2
   ADC   (FP),Y
   LDY   #$E6
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$E7
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$E8
   STA   (FP),Y
   LDY   #$02
   LDA   (DPTR),Y
   LDY   #$E9
   STA   (FP),Y
   LDY   #$03
   LDA   (DPTR),Y
   LDY   #$EA
   STA   (FP),Y
   LDY   #$04
   LDA   (DPTR),Y
   LDY   #$EB
   STA   (FP),Y
   LDY   #$05
   LDA   (DPTR),Y
   LDY   #$EC
   STA   (FP),Y
   LDY   #$06
   LDA   (DPTR),Y
   LDY   #$ED
   STA   (FP),Y
   LDY   #$07
   LDA   (DPTR),Y
   LDY   #$EE
   STA   (FP),Y
   CMP   #$00
   BNE   .thread@9
   DEY
   LDA   (FP),Y
   BNE   .thread@9
   DEY
   LDA   (FP),Y
   BNE   .thread@9
   DEY
   LDA   (FP),Y
   BNE   .thread@9
   DEY
   LDA   (FP),Y
   BNE   .thread@9
   DEY
   LDA   (FP),Y
   BNE   .thread@9
   DEY
   LDA   (FP),Y
   BNE   .thread@9
   DEY
   LDA   (FP),Y
   BEQ   .cmp_true@69
.thread@9:
   LDA   #$00
   JMP   .cmp_end@70
.cmp_true@69:
   LDA   #$01
.cmp_end@70:
   LDY   #$EF
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
   LDY   #$F1
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@16:
   TAX
   LDY   #$F1
   LDA   (FP),Y

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$FE
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$F3
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

three:
   SUBROUTINE

   LDX   #$00
   LDA   #$03
   RTS

test_type_conversion:
   SUBROUTINE

   ; prologue: 2 arg bytes, 158 local bytes
   SEC
   LDA   SSP
   SBC   #$A0
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$9F
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   SEC
   LDA   #$00
   SBC   #$64
   LDY   #$01
   STA   (FP),Y
   LDA   #$00
   SBC   #$00
   INY
   STA   (FP),Y
   LDY   #$A1
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$01
   LDA   (FP),Y
   DEY
   STA   (DPTR),Y
   LDY   #$02
   LDA   (FP),Y
   DEY
   STA   (DPTR),Y
   CLC
   LDA   FP
   ADC   #$03
   LDY   #$23
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
   LDA   #$00
   LDY   #$00
   STA   (DPTR),Y
   LDA   #$CA
   INY
   STA   (DPTR),Y
   LDA   #$9A
   INY
   STA   (DPTR),Y
   LDA   #$3B
   INY
   STA   (DPTR),Y
   LDA   #$00
   INY
   STA   (DPTR),Y
   INY
   STA   (DPTR),Y
   INY
   STA   (DPTR),Y
   INY
   STA   (DPTR),Y
   LDY   #$A1
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$25
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
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
   BMI   .sx_neg@72
   LDA   #$00
   JMP   .sx_done@73
.sx_neg@72:
   LDA   #$FF
.sx_done@73:
   LDY   #$29
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
   LDY   #$23
   LDA   (FP),Y
   CLC
   ADC   #$08
   LDY   #$2F
   STA   (FP),Y
   LDY   #$24
   LDA   (FP),Y
   ADC   #$00
   LDY   #$30
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$27
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$28
   LDA   (FP),Y
   LDY   #$01
   STA   (DPTR),Y
   LDY   #$29
   LDA   (FP),Y
   LDY   #$02
   STA   (DPTR),Y
   LDY   #$2A
   LDA   (FP),Y
   LDY   #$03
   STA   (DPTR),Y
   LDY   #$2B
   LDA   (FP),Y
   LDY   #$04
   STA   (DPTR),Y
   LDY   #$2C
   LDA   (FP),Y
   LDY   #$05
   STA   (DPTR),Y
   LDY   #$2D
   LDA   (FP),Y
   LDY   #$06
   STA   (DPTR),Y
   LDY   #$2E
   LDA   (FP),Y
   LDY   #$07
   STA   (DPTR),Y
   LDA   #$FF
   LDY   #$31
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
   INY
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
   LDY   #$23
   LDA   (FP),Y
   CLC
   ADC   #$10
   LDY   #$3D
   STA   (FP),Y
   LDY   #$24
   LDA   (FP),Y
   ADC   #$00
   LDY   #$3E
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$35
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$36
   LDA   (FP),Y
   LDY   #$01
   STA   (DPTR),Y
   LDY   #$37
   LDA   (FP),Y
   LDY   #$02
   STA   (DPTR),Y
   LDY   #$38
   LDA   (FP),Y
   LDY   #$03
   STA   (DPTR),Y
   LDY   #$39
   LDA   (FP),Y
   LDY   #$04
   STA   (DPTR),Y
   LDY   #$3A
   LDA   (FP),Y
   LDY   #$05
   STA   (DPTR),Y
   LDY   #$3B
   LDA   (FP),Y
   LDY   #$06
   STA   (DPTR),Y
   LDY   #$3C
   LDA   (FP),Y
   LDY   #$07
   STA   (DPTR),Y
   SEC
   LDA   #$00
   SBC   global_one
   LDY   #$3F
   STA   (FP),Y
   LDA   #$00
   SBC   global_one+1
   INY
   STA   (FP),Y
   LDA   #$00
   SBC   global_one+2
   INY
   STA   (FP),Y
   LDA   #$00
   SBC   global_one+3
   INY
   STA   (FP),Y
   LDA   #$00
   SBC   global_one+4
   INY
   STA   (FP),Y
   LDA   #$00
   SBC   global_one+5
   INY
   STA   (FP),Y
   LDA   #$00
   SBC   global_one+6
   INY
   STA   (FP),Y
   LDA   #$00
   SBC   global_one+7
   INY
   STA   (FP),Y
   LDY   #$3F
   LDA   (FP),Y
   LDY   #$47
   STA   (FP),Y
   LDY   #$40
   LDA   (FP),Y
   LDY   #$48
   STA   (FP),Y
   LDY   #$41
   LDA   (FP),Y
   LDY   #$49
   STA   (FP),Y
   LDY   #$42
   LDA   (FP),Y
   LDY   #$4A
   STA   (FP),Y
   LDY   #$43
   LDA   (FP),Y
   LDY   #$4B
   STA   (FP),Y
   LDY   #$44
   LDA   (FP),Y
   LDY   #$4C
   STA   (FP),Y
   LDY   #$45
   LDA   (FP),Y
   LDY   #$4D
   STA   (FP),Y
   LDY   #$46
   LDA   (FP),Y
   LDY   #$4E
   STA   (FP),Y
   LDY   #$23
   LDA   (FP),Y
   CLC
   ADC   #$18
   LDY   #$4F
   STA   (FP),Y
   LDY   #$24
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
   LDY   #$47
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$48
   LDA   (FP),Y
   LDY   #$01
   STA   (DPTR),Y
   LDY   #$49
   LDA   (FP),Y
   LDY   #$02
   STA   (DPTR),Y
   LDY   #$4A
   LDA   (FP),Y
   LDY   #$03
   STA   (DPTR),Y
   LDY   #$4B
   LDA   (FP),Y
   LDY   #$04
   STA   (DPTR),Y
   LDY   #$4C
   LDA   (FP),Y
   LDY   #$05
   STA   (DPTR),Y
   LDY   #$4D
   LDA   (FP),Y
   LDY   #$06
   STA   (DPTR),Y
   LDY   #$4E
   LDA   (FP),Y
   LDY   #$07
   STA   (DPTR),Y
   CLC
   LDA   FP
   ADC   #$03
   LDY   #$53
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   LDA   #$00
   STA   HARGS
   LDX   #$08
   LSR   HARGS
.mul_loop@74:
   BCC   .mul_skip@75
   CLC
   ADC   #$08
.mul_skip@75:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@74
   LDY   #$52
   STA   (FP),Y
   LDA   HARGS
   DEY
   STA   (FP),Y
   LDY   #$53
   LDA   (FP),Y
   CLC
   LDY   #$51
   ADC   (FP),Y
   LDY   #$55
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$52
   ADC   (FP),Y
   LDY   #$56
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$57
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$58
   STA   (FP),Y
   LDY   #$02
   LDA   (DPTR),Y
   LDY   #$59
   STA   (FP),Y
   LDY   #$03
   LDA   (DPTR),Y
   LDY   #$5A
   STA   (FP),Y
   LDY   #$04
   LDA   (DPTR),Y
   LDY   #$5B
   STA   (FP),Y
   LDY   #$05
   LDA   (DPTR),Y
   LDY   #$5C
   STA   (FP),Y
   LDY   #$06
   LDA   (DPTR),Y
   LDY   #$5D
   STA   (FP),Y
   LDY   #$07
   LDA   (DPTR),Y
   LDY   #$5E
   STA   (FP),Y
   CMP   #$00
   BNE   .thread@10
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .thread@10
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .thread@10
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .thread@10
   DEY
   LDA   (FP),Y
   CMP   #$3B
   BNE   .thread@10
   DEY
   LDA   (FP),Y
   CMP   #$9A
   BNE   .thread@10
   DEY
   LDA   (FP),Y
   CMP   #$CA
   BNE   .thread@10
   DEY
   LDA   (FP),Y
   CMP   #$00
   BEQ   .cmp_true@76
.thread@10:
   LDA   #$00
   JMP   .cmp_end@77
.cmp_true@76:
   LDA   #$01
.cmp_end@77:
   LDY   #$5F
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@12
   JMP   .and_false@27
.lb_skip@12:
   CLC
   LDA   FP
   ADC   #$03
   LDY   #$63
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   LDA   #$01
   STA   HARGS
   LDA   #$00
   LDX   #$08
   LSR   HARGS
.mul_loop@79:
   BCC   .mul_skip@80
   CLC
   ADC   #$08
.mul_skip@80:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@79
   LDY   #$62
   STA   (FP),Y
   LDA   HARGS
   DEY
   STA   (FP),Y
   LDY   #$63
   LDA   (FP),Y
   CLC
   LDY   #$61
   ADC   (FP),Y
   LDY   #$65
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$62
   ADC   (FP),Y
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
   LDY   #$02
   LDA   (DPTR),Y
   LDY   #$69
   STA   (FP),Y
   LDY   #$03
   LDA   (DPTR),Y
   LDY   #$6A
   STA   (FP),Y
   LDY   #$04
   LDA   (DPTR),Y
   LDY   #$6B
   STA   (FP),Y
   LDY   #$05
   LDA   (DPTR),Y
   LDY   #$6C
   STA   (FP),Y
   LDY   #$06
   LDA   (DPTR),Y
   LDY   #$6D
   STA   (FP),Y
   LDY   #$07
   LDA   (DPTR),Y
   LDY   #$6E
   STA   (FP),Y
   CMP   #$00
   BNE   .thread@11
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .thread@11
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .thread@11
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .thread@11
   DEY
   LDA   (FP),Y
   CMP   #$FF
   BNE   .thread@11
   DEY
   LDA   (FP),Y
   CMP   #$FF
   BNE   .thread@11
   DEY
   LDA   (FP),Y
   CMP   #$FF
   BNE   .thread@11
   DEY
   LDA   (FP),Y
   CMP   #$9C
   BEQ   .cmp_true@81
.thread@11:
   LDA   #$00
   JMP   .cmp_end@82
.cmp_true@81:
   LDA   #$01
.cmp_end@82:
   LDY   #$6F
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@27
   LDA   #$01
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   JMP   .and_end@28
.and_false@27:
   LDA   #$00
   LDY   #$71
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@28:
   LDY   #$71
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@11
   JMP   .and_false@25
.lb_skip@11:
   CLC
   LDA   FP
   ADC   #$03
   LDY   #$75
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   LDA   #$02
   STA   HARGS
   LDA   #$00
   LDX   #$08
   LSR   HARGS
.mul_loop@84:
   BCC   .mul_skip@85
   CLC
   ADC   #$08
.mul_skip@85:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@84
   LDY   #$74
   STA   (FP),Y
   LDA   HARGS
   DEY
   STA   (FP),Y
   LDY   #$75
   LDA   (FP),Y
   CLC
   LDY   #$73
   ADC   (FP),Y
   LDY   #$77
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$74
   ADC   (FP),Y
   LDY   #$78
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$79
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$7A
   STA   (FP),Y
   LDY   #$02
   LDA   (DPTR),Y
   LDY   #$7B
   STA   (FP),Y
   LDY   #$03
   LDA   (DPTR),Y
   LDY   #$7C
   STA   (FP),Y
   LDY   #$04
   LDA   (DPTR),Y
   LDY   #$7D
   STA   (FP),Y
   LDY   #$05
   LDA   (DPTR),Y
   LDY   #$7E
   STA   (FP),Y
   LDY   #$06
   LDA   (DPTR),Y
   LDY   #$7F
   STA   (FP),Y
   LDY   #$07
   LDA   (DPTR),Y
   LDY   #$80
   STA   (FP),Y
   LDA   #$FF
   INY
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
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$80
   LDA   (FP),Y
   LDY   #$88
   CMP   (FP),Y
   BNE   .thread@12
   LDY   #$7F
   LDA   (FP),Y
   LDY   #$87
   CMP   (FP),Y
   BNE   .thread@12
   LDY   #$7E
   LDA   (FP),Y
   LDY   #$86
   CMP   (FP),Y
   BNE   .thread@12
   LDY   #$7D
   LDA   (FP),Y
   LDY   #$85
   CMP   (FP),Y
   BNE   .thread@12
   LDY   #$7C
   LDA   (FP),Y
   LDY   #$84
   CMP   (FP),Y
   BNE   .thread@12
   LDY   #$7B
   LDA   (FP),Y
   LDY   #$83
   CMP   (FP),Y
   BNE   .thread@12
   LDY   #$7A
   LDA   (FP),Y
   LDY   #$82
   CMP   (FP),Y
   BNE   .thread@12
   LDY   #$79
   LDA   (FP),Y
   LDY   #$81
   CMP   (FP),Y
   BEQ   .cmp_true@86
.thread@12:
   LDA   #$00
   JMP   .cmp_end@87
.cmp_true@86:
   LDA   #$01
.cmp_end@87:
   LDY   #$89
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@25
   LDA   #$01
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   JMP   .and_end@26
.and_false@25:
   LDA   #$00
   LDY   #$8B
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@26:
   LDY   #$8B
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@10
   JMP   .and_false@23
.lb_skip@10:
   CLC
   LDA   FP
   ADC   #$03
   LDY   #$8F
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   LDA   #$03
   STA   HARGS
   LDA   #$00
   LDX   #$08
   LSR   HARGS
.mul_loop@89:
   BCC   .mul_skip@90
   CLC
   ADC   #$08
.mul_skip@90:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@89
   LDY   #$8E
   STA   (FP),Y
   LDA   HARGS
   DEY
   STA   (FP),Y
   LDY   #$8F
   LDA   (FP),Y
   CLC
   LDY   #$8D
   ADC   (FP),Y
   LDY   #$91
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$8E
   ADC   (FP),Y
   LDY   #$92
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$93
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$94
   STA   (FP),Y
   LDY   #$02
   LDA   (DPTR),Y
   LDY   #$95
   STA   (FP),Y
   LDY   #$03
   LDA   (DPTR),Y
   LDY   #$96
   STA   (FP),Y
   LDY   #$04
   LDA   (DPTR),Y
   LDY   #$97
   STA   (FP),Y
   LDY   #$05
   LDA   (DPTR),Y
   LDY   #$98
   STA   (FP),Y
   LDY   #$06
   LDA   (DPTR),Y
   LDY   #$99
   STA   (FP),Y
   LDY   #$07
   LDA   (DPTR),Y
   LDY   #$9A
   STA   (FP),Y
   CMP   #$00
   BNE   .thread@13
   DEY
   LDA   (FP),Y
   BNE   .thread@13
   DEY
   LDA   (FP),Y
   BNE   .thread@13
   DEY
   LDA   (FP),Y
   BNE   .thread@13
   DEY
   LDA   (FP),Y
   CMP   #$FF
   BNE   .thread@13
   DEY
   LDA   (FP),Y
   CMP   #$FF
   BNE   .thread@13
   DEY
   LDA   (FP),Y
   CMP   #$FF
   BNE   .thread@13
   DEY
   LDA   (FP),Y
   CMP   #$FF
   BEQ   .cmp_true@91
.thread@13:
   LDA   #$00
   JMP   .cmp_end@92
.cmp_true@91:
   LDA   #$01
.cmp_end@92:
   LDY   #$9B
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@23
   LDA   #$01
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   JMP   .and_end@24
.and_false@23:
   LDA   #$00
   LDY   #$9D
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@24:
   TAX
   LDY   #$9D
   LDA   (FP),Y

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$A2
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$9F
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

test_preserve_stack:
   SUBROUTINE

   ; prologue: 0 arg bytes, 90 local bytes
   SEC
   LDA   SSP
   SBC   #$5C
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$5B
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   SEC
   LDA   #$00
   SBC   #$01
   LDY   #$01
   STA   (FP),Y
   LDA   #$00
   SBC   #$00
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
   LDA   global_one
   STA   HARGS
   LDA   global_one+1
   STA   HARGS+1
   LDA   global_one+2
   STA   HARGS+2
   LDA   global_one+3
   STA   HARGS+3
   LDA   global_one+4
   STA   HARGS+4
   LDA   global_one+5
   STA   HARGS+5
   LDA   global_one+6
   STA   HARGS+6
   LDA   global_one+7
   STA   HARGS+7
   LDA   #$02
   STA   HARGS+8
   LDA   #$00
   STA   HARGS+9
   LDY   #$00
   STY   HARGS+10
   STY   HARGS+11
   STY   HARGS+12
   STY   HARGS+13
   STY   HARGS+14
   STA   HARGS+15
   JSR   mul64
   LDA   HARGS+16
   LDY   #$05
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
   LDY   #$05
   LDA   (FP),Y
   LDY   #$0D
   STA   (FP),Y
   LDY   #$06
   LDA   (FP),Y
   LDY   #$0E
   STA   (FP),Y
   CLC
   LDA   FP
   ADC   #$0F
   LDY   #$15
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
   LDY   #$0D
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$0E
   LDA   (FP),Y
   LDY   #$01
   STA   (DPTR),Y
   JSR   three
   LDY   #$17
   STA   (FP),Y
   TXA
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
   ORA   #$00
   BMI   .sx_neg@94
   LDA   #$00
   JMP   .sx_done@95
.sx_neg@94:
   LDA   #$FF
.sx_done@95:
   LDY   #$1B
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
   LDA   global_one
   CLC
   LDY   #$19
   ADC   (FP),Y
   LDY   #$21
   STA   (FP),Y
   LDA   global_one+1
   LDY   #$1A
   ADC   (FP),Y
   LDY   #$22
   STA   (FP),Y
   LDA   global_one+2
   LDY   #$1B
   ADC   (FP),Y
   LDY   #$23
   STA   (FP),Y
   LDA   global_one+3
   LDY   #$1C
   ADC   (FP),Y
   LDY   #$24
   STA   (FP),Y
   LDA   global_one+4
   LDY   #$1D
   ADC   (FP),Y
   LDY   #$25
   STA   (FP),Y
   LDA   global_one+5
   LDY   #$1E
   ADC   (FP),Y
   LDY   #$26
   STA   (FP),Y
   LDA   global_one+6
   LDY   #$1F
   ADC   (FP),Y
   LDY   #$27
   STA   (FP),Y
   LDA   global_one+7
   LDY   #$20
   ADC   (FP),Y
   LDY   #$28
   STA   (FP),Y
   LDY   #$21
   LDA   (FP),Y
   LDY   #$29
   STA   (FP),Y
   LDY   #$22
   LDA   (FP),Y
   LDY   #$2A
   STA   (FP),Y
   LDY   #$15
   LDA   (FP),Y
   CLC
   ADC   #$02
   LDY   #$2B
   STA   (FP),Y
   LDY   #$16
   LDA   (FP),Y
   ADC   #$00
   LDY   #$2C
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$29
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$2A
   LDA   (FP),Y
   LDY   #$01
   STA   (DPTR),Y
   LDY   #$15
   LDA   (FP),Y
   CLC
   ADC   #$04
   LDY   #$2D
   STA   (FP),Y
   LDY   #$16
   LDA   (FP),Y
   ADC   #$00
   LDY   #$2E
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
   LDA   #$29
   LDY   #$2F
   STA   (FP),Y
   LDA   #$90
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   SEC
   SBC   #$01
   INY
   STA   (FP),Y
   LDA   #$00
   SBC   #$00
   INY
   STA   (FP),Y
   LDY   #$04
   LDA   (FP),Y
   LDY   #$34
   CMP   (FP),Y
   BNE   .cmp_true@96
   LDY   #$03
   LDA   (FP),Y
   LDY   #$33
   CMP   (FP),Y
   BNE   .cmp_true@96
   LDA   #$00
   JMP   .cmp_end@97
.cmp_true@96:
   LDA   #$01
.cmp_end@97:
   LDY   #$35
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@29
   LDX   #$00
   TXA

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$5C
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$5B
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
.if_end@29:
   LDY   #$30
   LDA   (FP),Y
   CMP   #$90
   BNE   .cmp_true@99
   DEY
   LDA   (FP),Y
   CMP   #$29
   BNE   .cmp_true@99
   LDA   #$00
   JMP   .cmp_end@100
.cmp_true@99:
   LDA   #$01
.cmp_end@100:
   LDY   #$37
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@30
   LDX   #$00
   TXA

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$5C
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$5B
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
.if_end@30:
   CLC
   LDA   FP
   ADC   #$0F
   LDY   #$3B
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   LDA   #$00
   STA   HARGS
   LDX   #$08
   LSR   HARGS
.mul_loop@102:
   BCC   .mul_skip@103
   CLC
   ADC   #$02
.mul_skip@103:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@102
   LDY   #$3A
   STA   (FP),Y
   LDA   HARGS
   DEY
   STA   (FP),Y
   LDY   #$3B
   LDA   (FP),Y
   CLC
   LDY   #$39
   ADC   (FP),Y
   LDY   #$3D
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$3A
   ADC   (FP),Y
   LDY   #$3E
   STA   (FP),Y
   DEY
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
   CMP   #$00
   BNE   .thread@14
   DEY
   LDA   (FP),Y
   CMP   #$02
   BEQ   .cmp_true@104
.thread@14:
   LDA   #$00
   JMP   .cmp_end@105
.cmp_true@104:
   LDA   #$01
.cmp_end@105:
   LDY   #$41
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@14
   JMP   .and_false@33
.lb_skip@14:
   CLC
   LDA   FP
   ADC   #$0F
   LDY   #$45
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   LDA   #$01
   STA   HARGS
   LDA   #$00
   LDX   #$08
   LSR   HARGS
.mul_loop@107:
   BCC   .mul_skip@108
   CLC
   ADC   #$02
.mul_skip@108:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@107
   LDY   #$44
   STA   (FP),Y
   LDA   HARGS
   DEY
   STA   (FP),Y
   LDY   #$45
   LDA   (FP),Y
   CLC
   LDY   #$43
   ADC   (FP),Y
   LDY   #$47
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$44
   ADC   (FP),Y
   LDY   #$48
   STA   (FP),Y
   DEY
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
   CMP   #$00
   BNE   .thread@15
   DEY
   LDA   (FP),Y
   CMP   #$04
   BEQ   .cmp_true@109
.thread@15:
   LDA   #$00
   JMP   .cmp_end@110
.cmp_true@109:
   LDA   #$01
.cmp_end@110:
   LDY   #$4B
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@33
   LDA   #$01
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   JMP   .and_end@34
.and_false@33:
   LDA   #$00
   LDY   #$4D
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@34:
   LDY   #$4D
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@13
   JMP   .and_false@31
.lb_skip@13:
   CLC
   LDA   FP
   ADC   #$0F
   LDY   #$51
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   LDA   #$02
   STA   HARGS
   LDA   #$00
   LDX   #$08
   LSR   HARGS
.mul_loop@112:
   BCC   .mul_skip@113
   CLC
   ADC   #$02
.mul_skip@113:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@112
   LDY   #$50
   STA   (FP),Y
   LDA   HARGS
   DEY
   STA   (FP),Y
   LDY   #$51
   LDA   (FP),Y
   CLC
   LDY   #$4F
   ADC   (FP),Y
   LDY   #$53
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$50
   ADC   (FP),Y
   LDY   #$54
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$55
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$56
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lnot_true@114
   LDA   #$00
   JMP   .lnot_end@115
.lnot_true@114:
   LDA   #$01
.lnot_end@115:
   LDY   #$57
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@31
   LDA   #$01
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   JMP   .and_end@32
.and_false@31:
   LDA   #$00
   LDY   #$59
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@32:
   TAX
   LDY   #$59
   LDA   (FP),Y

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$5C
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$5B
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

   ; prologue: 0 arg bytes, 42 local bytes
   SEC
   LDA   SSP
   SBC   #$2C
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$2B
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   JSR   test_simple
   LDY   #$01
   STA   (FP),Y
   TXA
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lnot_true@116
   LDA   #$00
   JMP   .lnot_end@117
.lnot_true@116:
   LDA   #$01
.lnot_end@117:
   LDY   #$03
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@35
   LDX   #$00
   LDA   #$01

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$2C
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$2B
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
.if_end@35:
   JSR   test_partial
   LDY   #$05
   STA   (FP),Y
   TXA
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lnot_true@118
   LDA   #$00
   JMP   .lnot_end@119
.lnot_true@118:
   LDA   #$01
.lnot_end@119:
   LDY   #$07
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@36
   LDX   #$00
   LDA   #$02

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$2C
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$2B
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
   SEC
   LDA   #$00
   SBC   #$00
   LDY   #$09
   STA   (FP),Y
   LDA   #$00
   SBC   #$27
   INY
   STA   (FP),Y
   LDA   #$00
   SBC   #$B9
   INY
   STA   (FP),Y
   LDA   #$00
   SBC   #$29
   INY
   STA   (FP),Y
   LDA   #$00
   SBC   #$00
   INY
   STA   (FP),Y
   LDA   #$00
   SBC   #$00
   INY
   STA   (FP),Y
   LDA   #$00
   SBC   #$00
   INY
   STA   (FP),Y
   LDA   #$00
   SBC   #$00
   INY
   STA   (FP),Y
   LDY   #$09
   LDA   (FP),Y
   LDY   #$11
   STA   (FP),Y
   LDY   #$0A
   LDA   (FP),Y
   LDY   #$12
   STA   (FP),Y
   LDY   #$0B
   LDA   (FP),Y
   LDY   #$13
   STA   (FP),Y
   LDY   #$0C
   LDA   (FP),Y
   LDY   #$14
   STA   (FP),Y
   LDY   #$0D
   LDA   (FP),Y
   LDY   #$15
   STA   (FP),Y
   LDY   #$0E
   LDA   (FP),Y
   LDY   #$16
   STA   (FP),Y
   LDY   #$0F
   LDA   (FP),Y
   LDY   #$17
   STA   (FP),Y
   LDY   #$10
   LDA   (FP),Y
   LDY   #$18
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   INY
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
   SBC   #$0A
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDY   #$11
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$12
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$13
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$14
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   LDY   #$15
   LDA   (FP),Y
   LDY   #$05
   STA   (SSP),Y
   LDY   #$16
   LDA   (FP),Y
   LDY   #$06
   STA   (SSP),Y
   LDY   #$17
   LDA   (FP),Y
   LDY   #$07
   STA   (SSP),Y
   LDY   #$18
   LDA   (FP),Y
   LDY   #$08
   STA   (SSP),Y
   LDY   #$1B
   LDA   (FP),Y
   LDY   #$09
   STA   (SSP),Y
   LDY   #$1C
   LDA   (FP),Y
   LDY   #$0A
   STA   (SSP),Y
   JSR   test_non_constant
   LDY   #$1D
   STA   (FP),Y
   TXA
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lnot_true@120
   LDA   #$00
   JMP   .lnot_end@121
.lnot_true@120:
   LDA   #$01
.lnot_end@121:
   LDY   #$1F
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
   LDA   #$03

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$2C
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$2B
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
   CLC
   LDA   FP
   ADC   #$19
   LDY   #$21
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$02
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   DEY
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$22
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   JSR   test_type_conversion
   LDY   #$23
   STA   (FP),Y
   TXA
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lnot_true@122
   LDA   #$00
   JMP   .lnot_end@123
.lnot_true@122:
   LDA   #$01
.lnot_end@123:
   LDY   #$25
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
   LDA   #$04

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$2C
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$2B
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
   JSR   test_preserve_stack
   LDY   #$27
   STA   (FP),Y
   TXA
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lnot_true@124
   LDA   #$00
   JMP   .lnot_end@125
.lnot_true@124:
   LDA   #$01
.lnot_end@125:
   LDY   #$29
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
   LDA   #$05

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$2C
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
.if_end@39:
   LDX   #$00
   TXA

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$2C
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$2B
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

global_one:
   DC.L  $00000001
   DC.L  $00000000
