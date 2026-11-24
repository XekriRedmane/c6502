SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24

modify_simple_struct:
   SUBROUTINE

   ; prologue: 10 arg bytes, 54 local bytes
   SEC
   LDA   SSP
   SBC   #$38
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$37
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
   LDY   #$3A
   LDA   (FP),Y
   LDY   #$02
   STA   (FP),Y
   LDY   #$3B
   LDA   (FP),Y
   LDY   #$03
   STA   (FP),Y
   LDY   #$3C
   LDA   (FP),Y
   LDY   #$04
   STA   (FP),Y
   LDY   #$3D
   LDA   (FP),Y
   LDY   #$05
   STA   (FP),Y
   LDY   #$3E
   LDA   (FP),Y
   LDY   #$06
   STA   (FP),Y
   LDY   #$3F
   LDA   (FP),Y
   LDY   #$07
   STA   (FP),Y
   LDY   #$40
   LDA   (FP),Y
   LDY   #$08
   STA   (FP),Y
   LDY   #$41
   LDA   (FP),Y
   LDY   #$09
   STA   (FP),Y
   LDY   #$42
   LDA   (FP),Y
   LDY   #$0A
   STA   (FP),Y
   CLC
   LDA   FP
   ADC   #$39
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
   ADC   #$39
   LDY   #$0D
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
   LDY   #$0F
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$10
   STA   (FP),Y
   LDY   #$02
   LDA   (DPTR),Y
   LDY   #$11
   STA   (FP),Y
   LDY   #$03
   LDA   (DPTR),Y
   LDY   #$12
   STA   (FP),Y
   LDY   #$04
   LDA   (DPTR),Y
   LDY   #$13
   STA   (FP),Y
   LDY   #$05
   LDA   (DPTR),Y
   LDY   #$14
   STA   (FP),Y
   LDY   #$06
   LDA   (DPTR),Y
   LDY   #$15
   STA   (FP),Y
   LDY   #$07
   LDA   (DPTR),Y
   LDY   #$16
   STA   (FP),Y
   LDY   #$0F
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
   BNE   .or_true@0
   CLC
   LDA   FP
   ADC   #$39
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
   LDY   #$19
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$1A
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$1B
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$1C
   STA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@0
   DEY
   LDA   (FP),Y
   CMP   #$03
   BNE   .cmp_true@0
   LDA   #$00
   JMP   .cmp_end@1
.cmp_true@0:
   LDA   #$01
.cmp_end@1:
   LDY   #$1D
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
   LDY   #$1F
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@1:
   LDY   #$1F
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@2
   LDX   #$00
   TXA

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$42
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$37
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
   CLC
   LDA   FP
   ADC   #$01
   LDY   #$21
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
   BNE   .cmp_true@3
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@3
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@3
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@3
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@3
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@3
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@3
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@3
   LDA   #$00
   JMP   .cmp_end@4
.cmp_true@3:
   LDA   #$01
.cmp_end@4:
   LDY   #$2B
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@3
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
   ADC   #$08
   LDY   #$2F
   STA   (FP),Y
   DEY
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
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$31
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$32
   STA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@6
   DEY
   LDA   (FP),Y
   CMP   #$03
   BNE   .cmp_true@6
   LDA   #$00
   JMP   .cmp_end@7
.cmp_true@6:
   LDA   #$01
.cmp_end@7:
   LDY   #$33
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@3
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   JMP   .or_end@4
.or_true@3:
   LDA   #$01
   LDY   #$35
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@4:
   LDY   #$35
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@5
   LDX   #$00
   TXA

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$42
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
.if_end@5:
   LDX   #$00
   LDA   #$01

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$42
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$37
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

modify_nested_struct:
   SUBROUTINE

   ; prologue: 16 arg bytes, 182 local bytes
   SEC
   LDA   SSP
   SBC   #$B8
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$B7
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
   LDY   #$BA
   LDA   (FP),Y
   LDY   #$02
   STA   (FP),Y
   LDY   #$BB
   LDA   (FP),Y
   LDY   #$03
   STA   (FP),Y
   LDY   #$BC
   LDA   (FP),Y
   LDY   #$04
   STA   (FP),Y
   LDY   #$BD
   LDA   (FP),Y
   LDY   #$05
   STA   (FP),Y
   LDY   #$BE
   LDA   (FP),Y
   LDY   #$06
   STA   (FP),Y
   LDY   #$BF
   LDA   (FP),Y
   LDY   #$07
   STA   (FP),Y
   LDY   #$C0
   LDA   (FP),Y
   LDY   #$08
   STA   (FP),Y
   LDY   #$C1
   LDA   (FP),Y
   LDY   #$09
   STA   (FP),Y
   LDY   #$C2
   LDA   (FP),Y
   LDY   #$0A
   STA   (FP),Y
   LDY   #$C3
   LDA   (FP),Y
   LDY   #$0B
   STA   (FP),Y
   LDY   #$C4
   LDA   (FP),Y
   LDY   #$0C
   STA   (FP),Y
   LDY   #$C5
   LDA   (FP),Y
   LDY   #$0D
   STA   (FP),Y
   LDY   #$C6
   LDA   (FP),Y
   LDY   #$0E
   STA   (FP),Y
   LDY   #$C7
   LDA   (FP),Y
   LDY   #$0F
   STA   (FP),Y
   LDY   #$C8
   LDA   (FP),Y
   LDY   #$10
   STA   (FP),Y
   LDA   #$0A
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   ORA   #$00
   BPL   .sx_done@10
   LDA   #$FF
.sx_done@10:
   LDY   #$13
   STA   (FP),Y
   INY
   STA   (FP),Y
   CLC
   LDA   FP
   ADC   #$B9
   INY
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$0C
   LDY   #$17
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$18
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$11
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$12
   LDA   (FP),Y
   LDY   #$01
   STA   (DPTR),Y
   LDY   #$13
   LDA   (FP),Y
   LDY   #$02
   STA   (DPTR),Y
   LDY   #$14
   LDA   (FP),Y
   LDY   #$03
   STA   (DPTR),Y
   CLC
   LDA   FP
   ADC   #$B9
   LDY   #$19
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   DEY
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
   LDA   #$C8
   LDY   #$00
   STA   (DPTR),Y
   LDA   #$00
   INY
   STA   (DPTR),Y
   CLC
   LDA   FP
   ADC   #$B9
   LDY   #$1D
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$0A
   LDY   #$1F
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$20
   STA   (FP),Y
   DEY
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
   LDA   #$24
   INY
   STA   (DPTR),Y
   LDA   #$40
   INY
   STA   (DPTR),Y
   CLC
   LDA   FP
   ADC   #$B9
   LDY   #$23
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$0A
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
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$27
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$28
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$08
   LDY   #$29
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$2A
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDA   #$0B
   LDY   #$00
   STA   (DPTR),Y
   LDA   #$00
   INY
   STA   (DPTR),Y
   CLC
   LDA   FP
   ADC   #$B9
   LDY   #$2B
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$08
   LDY   #$2D
   STA   (FP),Y
   DEY
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
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$2F
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$30
   STA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@11
   DEY
   LDA   (FP),Y
   CMP   #$C8
   BNE   .cmp_true@11
   LDA   #$00
   JMP   .cmp_end@12
.cmp_true@11:
   LDA   #$01
.cmp_end@12:
   LDY   #$31
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@7
   JMP   .or_true@12
.lb_skip@7:
   CLC
   LDA   FP
   ADC   #$B9
   LDY   #$33
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
   CMP   #$40
   BNE   .cmp_true@14
   DEY
   LDA   (FP),Y
   CMP   #$10
   BNE   .cmp_true@14
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@14
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@14
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@14
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@14
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@14
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@14
   LDA   #$00
   JMP   .cmp_end@15
.cmp_true@14:
   LDA   #$01
.cmp_end@15:
   LDY   #$3D
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
   LDY   #$3F
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@13:
   LDY   #$3F
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@6
   JMP   .or_true@10
.lb_skip@6:
   CLC
   LDA   FP
   ADC   #$B9
   LDY   #$41
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$0C
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
   LDA   #$0A
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   ORA   #$00
   BPL   .sx_done@18
   LDA   #$FF
.sx_done@18:
   LDY   #$4B
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$48
   LDA   (FP),Y
   LDY   #$4C
   CMP   (FP),Y
   BNE   .cmp_true@19
   LDY   #$47
   LDA   (FP),Y
   LDY   #$4B
   CMP   (FP),Y
   BNE   .cmp_true@19
   LDY   #$46
   LDA   (FP),Y
   LDY   #$4A
   CMP   (FP),Y
   BNE   .cmp_true@19
   LDY   #$45
   LDA   (FP),Y
   LDY   #$49
   CMP   (FP),Y
   BNE   .cmp_true@19
   LDA   #$00
   JMP   .cmp_end@20
.cmp_true@19:
   LDA   #$01
.cmp_end@20:
   LDY   #$4D
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
   LDY   #$4F
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@11:
   LDY   #$4F
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@5
   JMP   .or_true@8
.lb_skip@5:
   CLC
   LDA   FP
   ADC   #$B9
   LDY   #$51
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$0A
   LDY   #$53
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
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
   CMP   #$40
   BNE   .cmp_true@22
   DEY
   LDA   (FP),Y
   CMP   #$24
   BNE   .cmp_true@22
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@22
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@22
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@22
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@22
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@22
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@22
   LDA   #$00
   JMP   .cmp_end@23
.cmp_true@22:
   LDA   #$01
.cmp_end@23:
   LDY   #$5F
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
   LDY   #$61
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@9:
   LDY   #$61
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@4
   JMP   .or_true@6
.lb_skip@4:
   CLC
   LDA   FP
   ADC   #$B9
   LDY   #$63
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$0A
   LDY   #$65
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
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
   CLC
   ADC   #$08
   LDY   #$69
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$6A
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$6B
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$6C
   STA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@25
   DEY
   LDA   (FP),Y
   CMP   #$0B
   BNE   .cmp_true@25
   LDA   #$00
   JMP   .cmp_end@26
.cmp_true@25:
   LDA   #$01
.cmp_end@26:
   LDY   #$6D
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
   LDY   #$6F
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@7:
   LDY   #$6F
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
   ADC   #$C8
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$B7
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
   CLC
   LDA   FP
   ADC   #$01
   LDY   #$71
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$08
   LDY   #$73
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$74
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$75
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$76
   STA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@28
   DEY
   LDA   (FP),Y
   CMP   #$05
   BNE   .cmp_true@28
   LDA   #$00
   JMP   .cmp_end@29
.cmp_true@28:
   LDA   #$01
.cmp_end@29:
   LDY   #$77
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@3
   JMP   .or_true@21
.lb_skip@3:
   CLC
   LDA   FP
   ADC   #$01
   LDY   #$79
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
   LDY   #$7B
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$7C
   STA   (FP),Y
   LDY   #$02
   LDA   (DPTR),Y
   LDY   #$7D
   STA   (FP),Y
   LDY   #$03
   LDA   (DPTR),Y
   LDY   #$7E
   STA   (FP),Y
   LDY   #$04
   LDA   (DPTR),Y
   LDY   #$7F
   STA   (FP),Y
   LDY   #$05
   LDA   (DPTR),Y
   LDY   #$80
   STA   (FP),Y
   LDY   #$06
   LDA   (DPTR),Y
   LDY   #$81
   STA   (FP),Y
   LDY   #$07
   LDA   (DPTR),Y
   LDY   #$82
   STA   (FP),Y
   CMP   #$40
   BNE   .cmp_true@31
   DEY
   LDA   (FP),Y
   CMP   #$10
   BNE   .cmp_true@31
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@31
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@31
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@31
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@31
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@31
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@31
   LDA   #$00
   JMP   .cmp_end@32
.cmp_true@31:
   LDA   #$01
.cmp_end@32:
   LDY   #$83
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@21
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   JMP   .or_end@22
.or_true@21:
   LDA   #$01
   LDY   #$85
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@22:
   LDY   #$85
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@2
   JMP   .or_true@19
.lb_skip@2:
   CLC
   LDA   FP
   ADC   #$01
   LDY   #$87
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$0C
   LDY   #$89
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
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
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$8C
   STA   (FP),Y
   LDY   #$02
   LDA   (DPTR),Y
   LDY   #$8D
   STA   (FP),Y
   LDY   #$03
   LDA   (DPTR),Y
   LDY   #$8E
   STA   (FP),Y
   LDA   #$E8
   INY
   STA   (FP),Y
   LDA   #$03
   INY
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@34
   LDA   #$00
   JMP   .sx_done@35
.sx_neg@34:
   LDA   #$FF
.sx_done@35:
   LDY   #$91
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$8E
   LDA   (FP),Y
   LDY   #$92
   CMP   (FP),Y
   BNE   .cmp_true@36
   LDY   #$8D
   LDA   (FP),Y
   LDY   #$91
   CMP   (FP),Y
   BNE   .cmp_true@36
   LDY   #$8C
   LDA   (FP),Y
   LDY   #$90
   CMP   (FP),Y
   BNE   .cmp_true@36
   LDY   #$8B
   LDA   (FP),Y
   LDY   #$8F
   CMP   (FP),Y
   BNE   .cmp_true@36
   LDA   #$00
   JMP   .cmp_end@37
.cmp_true@36:
   LDA   #$01
.cmp_end@37:
   LDY   #$93
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
   LDY   #$95
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@20:
   LDY   #$95
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@1
   JMP   .or_true@17
.lb_skip@1:
   CLC
   LDA   FP
   ADC   #$01
   LDY   #$97
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$0A
   LDY   #$99
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$9A
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$9B
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$9C
   STA   (FP),Y
   DEY
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
   LDY   #$02
   LDA   (DPTR),Y
   LDY   #$9F
   STA   (FP),Y
   LDY   #$03
   LDA   (DPTR),Y
   LDY   #$A0
   STA   (FP),Y
   LDY   #$04
   LDA   (DPTR),Y
   LDY   #$A1
   STA   (FP),Y
   LDY   #$05
   LDA   (DPTR),Y
   LDY   #$A2
   STA   (FP),Y
   LDY   #$06
   LDA   (DPTR),Y
   LDY   #$A3
   STA   (FP),Y
   LDY   #$07
   LDA   (DPTR),Y
   LDY   #$A4
   STA   (FP),Y
   CMP   #$40
   BNE   .cmp_true@39
   DEY
   LDA   (FP),Y
   CMP   #$24
   BNE   .cmp_true@39
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@39
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@39
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@39
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@39
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@39
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@39
   LDA   #$00
   JMP   .cmp_end@40
.cmp_true@39:
   LDA   #$01
.cmp_end@40:
   LDY   #$A5
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
   LDY   #$A7
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@18:
   LDY   #$A7
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@0
   JMP   .or_true@15
.lb_skip@0:
   CLC
   LDA   FP
   ADC   #$01
   LDY   #$A9
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$0A
   LDY   #$AB
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$AC
   STA   (FP),Y
   DEY
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
   CLC
   ADC   #$08
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
   CMP   #$00
   BNE   .cmp_true@42
   DEY
   LDA   (FP),Y
   CMP   #$0B
   BNE   .cmp_true@42
   LDA   #$00
   JMP   .cmp_end@43
.cmp_true@42:
   LDA   #$01
.cmp_end@43:
   LDY   #$B3
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
   LDY   #$B5
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@16:
   LDY   #$B5
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@23
   LDX   #$00
   TXA

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$C8
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
.if_end@23:
   LDX   #$00
   LDA   #$01

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$C8
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$B7
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
