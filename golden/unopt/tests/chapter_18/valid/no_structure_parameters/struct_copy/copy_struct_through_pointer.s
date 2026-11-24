SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24

test_copy_to_pointer:
   SUBROUTINE

   ; prologue: 0 arg bytes, 67 local bytes
   SEC
   LDA   SSP
   SBC   #$45
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$44
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
   LDY   #$0A
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
   LDA   #$21
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$0A
   LDA   (FP),Y
   CLC
   ADC   #$01
   LDY   #$0C
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$0D
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDA   #$3F
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$0A
   LDA   (FP),Y
   CLC
   ADC   #$02
   LDY   #$0E
   STA   (FP),Y
   LDY   #$0B
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
   LDA   #$00
   LDY   #$00
   STA   (DPTR),Y
   SEC
   SBC   #$14
   LDY   #$10
   STA   (FP),Y
   LDA   #$00
   SBC   #$00
   INY
   STA   (FP),Y
   LDY   #$0A
   LDA   (FP),Y
   CLC
   ADC   #$03
   LDY   #$12
   STA   (FP),Y
   LDY   #$0B
   LDA   (FP),Y
   ADC   #$00
   LDY   #$13
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
   LDY   #$11
   LDA   (FP),Y
   LDY   #$01
   STA   (DPTR),Y
   SEC
   LDA   #$00
   SBC   #$1E
   LDY   #$14
   STA   (FP),Y
   LDA   #$00
   SBC   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$16
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$17
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@0
   LDA   #$00
   JMP   .sx_done@1
.sx_neg@0:
   LDA   #$FF
.sx_done@1:
   LDY   #$18
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$0A
   LDA   (FP),Y
   CLC
   ADC   #$05
   LDY   #$1A
   STA   (FP),Y
   LDY   #$0B
   LDA   (FP),Y
   ADC   #$00
   LDY   #$1B
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$16
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$17
   LDA   (FP),Y
   LDY   #$01
   STA   (DPTR),Y
   LDY   #$18
   LDA   (FP),Y
   LDY   #$02
   STA   (DPTR),Y
   LDY   #$19
   LDA   (FP),Y
   LDY   #$03
   STA   (DPTR),Y
   LDA   #$09
   LDY   #$1C
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$04
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDY   #$1C
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
   LDY   #$1F
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   JSR   malloc
   LDY   #$20
   STA   (FP),Y
   TXA
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$22
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$23
   STA   (FP),Y
   DEY
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
   LDY   #$03
   LDA   (FP),Y
   DEY
   STA   (DPTR),Y
   LDY   #$04
   LDA   (FP),Y
   DEY
   STA   (DPTR),Y
   LDY   #$05
   LDA   (FP),Y
   DEY
   STA   (DPTR),Y
   LDY   #$06
   LDA   (FP),Y
   DEY
   STA   (DPTR),Y
   LDY   #$07
   LDA   (FP),Y
   DEY
   STA   (DPTR),Y
   LDY   #$08
   LDA   (FP),Y
   DEY
   STA   (DPTR),Y
   LDY   #$09
   LDA   (FP),Y
   DEY
   STA   (DPTR),Y
   LDA   #<str@0
   LDY   #$24
   STA   (FP),Y
   LDA   #>str@0
   INY
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$04
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDY   #$22
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$23
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$24
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$25
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   JSR   strcmp
   LDY   #$26
   STA   (FP),Y
   TXA
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@2
   LDY   #$22
   LDA   (FP),Y
   CLC
   ADC   #$03
   LDY   #$28
   STA   (FP),Y
   LDY   #$23
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
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$2A
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$2B
   STA   (FP),Y
   SEC
   LDA   #$00
   SBC   #$14
   INY
   STA   (FP),Y
   LDA   #$00
   SBC   #$00
   INY
   STA   (FP),Y
   LDY   #$2B
   LDA   (FP),Y
   LDY   #$2D
   CMP   (FP),Y
   BNE   .cmp_true@2
   LDY   #$2A
   LDA   (FP),Y
   LDY   #$2C
   CMP   (FP),Y
   BNE   .cmp_true@2
   LDA   #$00
   JMP   .cmp_end@3
.cmp_true@2:
   LDA   #$01
.cmp_end@3:
   LDY   #$2E
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
   LDY   #$30
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@3:
   LDY   #$30
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@0
   JMP   .or_true@0
.lb_skip@0:
   LDY   #$22
   LDA   (FP),Y
   CLC
   ADC   #$03
   LDY   #$32
   STA   (FP),Y
   LDY   #$23
   LDA   (FP),Y
   ADC   #$00
   LDY   #$33
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$02
   LDY   #$34
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$35
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$36
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$37
   STA   (FP),Y
   LDY   #$02
   LDA   (DPTR),Y
   LDY   #$38
   STA   (FP),Y
   LDY   #$03
   LDA   (DPTR),Y
   LDY   #$39
   STA   (FP),Y
   SEC
   LDA   #$00
   SBC   #$1E
   INY
   STA   (FP),Y
   LDA   #$00
   SBC   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$3C
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$3D
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@5
   LDA   #$00
   JMP   .sx_done@6
.sx_neg@5:
   LDA   #$FF
.sx_done@6:
   LDY   #$3E
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$39
   LDA   (FP),Y
   LDY   #$3F
   CMP   (FP),Y
   BNE   .cmp_true@7
   LDY   #$38
   LDA   (FP),Y
   LDY   #$3E
   CMP   (FP),Y
   BNE   .cmp_true@7
   LDY   #$37
   LDA   (FP),Y
   LDY   #$3D
   CMP   (FP),Y
   BNE   .cmp_true@7
   LDY   #$36
   LDA   (FP),Y
   LDY   #$3C
   CMP   (FP),Y
   BNE   .cmp_true@7
   LDA   #$00
   JMP   .cmp_end@8
.cmp_true@7:
   LDA   #$01
.cmp_end@8:
   LDY   #$40
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
   LDY   #$42
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@1:
   LDY   #$42
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@4
   LDX   #$00
   TXA

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$45
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
.if_end@4:
   LDX   #$00
   LDA   #$01

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$45
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$44
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

test_copy_from_pointer:
   SUBROUTINE

   ; prologue: 0 arg bytes, 63 local bytes
   SEC
   LDA   SSP
   SBC   #$41
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$40
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   LDA   #<@5.my_struct
   LDY   #$01
   STA   (FP),Y
   LDA   #>@5.my_struct
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
   LDA   #$09
   INY
   STA   (FP),Y
.clear@5:
   SEC
   SBC   #$01
   LDY   #$05
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   CLC
   LDA   FP
   ADC   #$08
   LDY   #$11
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   LDY   #$06
   ADC   (FP),Y
   LDY   #$13
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$07
   ADC   (FP),Y
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
   LDY   #$05
   LDA   (FP),Y
   BNE   .clear@5
   LDY   #$03
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$15
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$16
   STA   (FP),Y
   LDY   #$02
   LDA   (DPTR),Y
   LDY   #$17
   STA   (FP),Y
   LDY   #$03
   LDA   (DPTR),Y
   LDY   #$18
   STA   (FP),Y
   LDY   #$04
   LDA   (DPTR),Y
   LDY   #$19
   STA   (FP),Y
   LDY   #$05
   LDA   (DPTR),Y
   LDY   #$1A
   STA   (FP),Y
   LDY   #$06
   LDA   (DPTR),Y
   LDY   #$1B
   STA   (FP),Y
   LDY   #$07
   LDA   (DPTR),Y
   LDY   #$1C
   STA   (FP),Y
   LDY   #$08
   LDA   (DPTR),Y
   LDY   #$1D
   STA   (FP),Y
   LDY   #$15
   LDA   (FP),Y
   LDY   #$08
   STA   (FP),Y
   LDY   #$16
   LDA   (FP),Y
   LDY   #$09
   STA   (FP),Y
   LDY   #$17
   LDA   (FP),Y
   LDY   #$0A
   STA   (FP),Y
   LDY   #$18
   LDA   (FP),Y
   LDY   #$0B
   STA   (FP),Y
   LDY   #$19
   LDA   (FP),Y
   LDY   #$0C
   STA   (FP),Y
   LDY   #$1A
   LDA   (FP),Y
   LDY   #$0D
   STA   (FP),Y
   LDY   #$1B
   LDA   (FP),Y
   LDY   #$0E
   STA   (FP),Y
   LDY   #$1C
   LDA   (FP),Y
   LDY   #$0F
   STA   (FP),Y
   LDY   #$1D
   LDA   (FP),Y
   LDY   #$10
   STA   (FP),Y
   CLC
   LDA   FP
   ADC   #$08
   LDY   #$20
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   LDA   #<str@1
   LDY   #$1E
   STA   (FP),Y
   LDA   #>str@1
   INY
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$04
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   INY
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$21
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$1E
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$1F
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   JSR   strcmp
   LDY   #$22
   STA   (FP),Y
   TXA
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@8
   CLC
   LDA   FP
   ADC   #$08
   INY
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$03
   LDY   #$26
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$27
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$28
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$29
   STA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@10
   DEY
   LDA   (FP),Y
   CMP   #$4D
   BNE   .cmp_true@10
   LDA   #$00
   JMP   .cmp_end@11
.cmp_true@10:
   LDA   #$01
.cmp_end@11:
   LDY   #$2A
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
   LDY   #$2C
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@9:
   LDY   #$2C
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@1
   JMP   .or_true@6
.lb_skip@1:
   CLC
   LDA   FP
   ADC   #$08
   LDY   #$2E
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$03
   LDY   #$30
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$31
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$02
   LDY   #$32
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$33
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$34
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$35
   STA   (FP),Y
   LDY   #$02
   LDA   (DPTR),Y
   LDY   #$36
   STA   (FP),Y
   LDY   #$03
   LDA   (DPTR),Y
   LDY   #$37
   STA   (FP),Y
   LDA   #$4E
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   ORA   #$00
   BPL   .sx_done@14
   LDA   #$FF
.sx_done@14:
   LDY   #$3A
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$37
   LDA   (FP),Y
   LDY   #$3B
   CMP   (FP),Y
   BNE   .cmp_true@15
   LDY   #$36
   LDA   (FP),Y
   LDY   #$3A
   CMP   (FP),Y
   BNE   .cmp_true@15
   LDY   #$35
   LDA   (FP),Y
   LDY   #$39
   CMP   (FP),Y
   BNE   .cmp_true@15
   LDY   #$34
   LDA   (FP),Y
   LDY   #$38
   CMP   (FP),Y
   BNE   .cmp_true@15
   LDA   #$00
   JMP   .cmp_end@16
.cmp_true@15:
   LDA   #$01
.cmp_end@16:
   LDY   #$3C
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
   LDY   #$3E
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@7:
   LDY   #$3E
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@10
   LDX   #$00
   TXA

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$41
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
.if_end@10:
   LDX   #$00
   LDA   #$01

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$41
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$40
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

test_copy_to_and_from_pointer:
   SUBROUTINE

   ; prologue: 0 arg bytes, 72 local bytes
   SEC
   LDA   SSP
   SBC   #$4A
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$49
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
   LDY   #$0A
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
   LDA   #$2B
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$0A
   LDA   (FP),Y
   CLC
   ADC   #$01
   LDY   #$0C
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$0D
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDA   #$2D
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$0A
   LDA   (FP),Y
   CLC
   ADC   #$02
   LDY   #$0E
   STA   (FP),Y
   LDY   #$0B
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
   LDA   #$00
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$0A
   LDA   (FP),Y
   CLC
   ADC   #$03
   LDY   #$10
   STA   (FP),Y
   LDY   #$0B
   LDA   (FP),Y
   ADC   #$00
   LDY   #$11
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDA   #$E8
   LDY   #$00
   STA   (DPTR),Y
   LDA   #$03
   INY
   STA   (DPTR),Y
   LDA   #$E9
   LDY   #$12
   STA   (FP),Y
   LDA   #$03
   INY
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@18
   LDA   #$00
   JMP   .sx_done@19
.sx_neg@18:
   LDA   #$FF
.sx_done@19:
   LDY   #$14
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$0A
   LDA   (FP),Y
   CLC
   ADC   #$05
   LDY   #$16
   STA   (FP),Y
   LDY   #$0B
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
   LDY   #$12
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$13
   LDA   (FP),Y
   LDY   #$01
   STA   (DPTR),Y
   LDY   #$14
   LDA   (FP),Y
   LDY   #$02
   STA   (DPTR),Y
   LDY   #$15
   LDA   (FP),Y
   LDY   #$03
   STA   (DPTR),Y
   CLC
   LDA   FP
   ADC   #$01
   LDY   #$18
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
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
   LDA   #$09
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$04
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDY   #$1C
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
   LDY   #$1F
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   JSR   malloc
   LDY   #$20
   STA   (FP),Y
   TXA
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$22
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$23
   STA   (FP),Y
   LDY   #$1A
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$24
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$25
   STA   (FP),Y
   LDY   #$02
   LDA   (DPTR),Y
   LDY   #$26
   STA   (FP),Y
   LDY   #$03
   LDA   (DPTR),Y
   LDY   #$27
   STA   (FP),Y
   LDY   #$04
   LDA   (DPTR),Y
   LDY   #$28
   STA   (FP),Y
   LDY   #$05
   LDA   (DPTR),Y
   LDY   #$29
   STA   (FP),Y
   LDY   #$06
   LDA   (DPTR),Y
   LDY   #$2A
   STA   (FP),Y
   LDY   #$07
   LDA   (DPTR),Y
   LDY   #$2B
   STA   (FP),Y
   LDY   #$08
   LDA   (DPTR),Y
   LDY   #$2C
   STA   (FP),Y
   LDY   #$22
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   INY
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$25
   LDA   (FP),Y
   LDY   #$01
   STA   (DPTR),Y
   LDY   #$26
   LDA   (FP),Y
   LDY   #$02
   STA   (DPTR),Y
   LDY   #$27
   LDA   (FP),Y
   LDY   #$03
   STA   (DPTR),Y
   LDY   #$28
   LDA   (FP),Y
   LDY   #$04
   STA   (DPTR),Y
   LDY   #$29
   LDA   (FP),Y
   LDY   #$05
   STA   (DPTR),Y
   LDY   #$2A
   LDA   (FP),Y
   LDY   #$06
   STA   (DPTR),Y
   LDY   #$2B
   LDA   (FP),Y
   LDY   #$07
   STA   (DPTR),Y
   LDY   #$2C
   LDA   (FP),Y
   LDY   #$08
   STA   (DPTR),Y
   LDA   #<str@2
   LDY   #$2D
   STA   (FP),Y
   LDA   #>str@2
   INY
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$04
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDY   #$22
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$23
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$2D
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$2E
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   JSR   strcmp
   LDY   #$2F
   STA   (FP),Y
   TXA
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@13
   LDY   #$22
   LDA   (FP),Y
   CLC
   ADC   #$03
   LDY   #$31
   STA   (FP),Y
   LDY   #$23
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
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$33
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$34
   STA   (FP),Y
   CMP   #$03
   BNE   .cmp_true@20
   DEY
   LDA   (FP),Y
   CMP   #$E8
   BNE   .cmp_true@20
   LDA   #$00
   JMP   .cmp_end@21
.cmp_true@20:
   LDA   #$01
.cmp_end@21:
   LDY   #$35
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@13
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   JMP   .or_end@14
.or_true@13:
   LDA   #$01
   LDY   #$37
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@14:
   LDY   #$37
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@2
   JMP   .or_true@11
.lb_skip@2:
   LDY   #$22
   LDA   (FP),Y
   CLC
   ADC   #$03
   LDY   #$39
   STA   (FP),Y
   LDY   #$23
   LDA   (FP),Y
   ADC   #$00
   LDY   #$3A
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$02
   LDY   #$3B
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$3C
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
   LDA   #$E9
   INY
   STA   (FP),Y
   LDA   #$03
   INY
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@23
   LDA   #$00
   JMP   .sx_done@24
.sx_neg@23:
   LDA   #$FF
.sx_done@24:
   LDY   #$43
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$40
   LDA   (FP),Y
   LDY   #$44
   CMP   (FP),Y
   BNE   .cmp_true@25
   LDY   #$3F
   LDA   (FP),Y
   LDY   #$43
   CMP   (FP),Y
   BNE   .cmp_true@25
   LDY   #$3E
   LDA   (FP),Y
   LDY   #$42
   CMP   (FP),Y
   BNE   .cmp_true@25
   LDY   #$3D
   LDA   (FP),Y
   LDY   #$41
   CMP   (FP),Y
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
   BNE   .or_true@11
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   JMP   .or_end@12
.or_true@11:
   LDA   #$01
   LDY   #$47
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@12:
   LDY   #$47
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@15
   LDX   #$00
   TXA

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$4A
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
.if_end@15:
   LDX   #$00
   LDA   #$01

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$4A
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$49
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

test_copy_to_array_elem:
   SUBROUTINE

   ; prologue: 0 arg bytes, 117 local bytes
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

   CLC
   LDA   FP
   ADC   #$01
   LDY   #$0A
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
   LDA   #$0A
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$0A
   LDA   (FP),Y
   CLC
   ADC   #$01
   LDY   #$0C
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$0D
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDA   #$09
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$0A
   LDA   (FP),Y
   CLC
   ADC   #$02
   LDY   #$0E
   STA   (FP),Y
   LDY   #$0B
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
   LDA   #$00
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$0A
   LDA   (FP),Y
   CLC
   ADC   #$03
   LDY   #$10
   STA   (FP),Y
   LDY   #$0B
   LDA   (FP),Y
   ADC   #$00
   LDY   #$11
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDA   #$10
   LDY   #$00
   STA   (DPTR),Y
   LDA   #$27
   INY
   STA   (DPTR),Y
   LDA   #$20
   LDY   #$12
   STA   (FP),Y
   LDA   #$4E
   INY
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@28
   LDA   #$00
   JMP   .sx_done@29
.sx_neg@28:
   LDA   #$FF
.sx_done@29:
   LDY   #$14
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$0A
   LDA   (FP),Y
   CLC
   ADC   #$05
   LDY   #$16
   STA   (FP),Y
   LDY   #$0B
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
   LDY   #$12
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$13
   LDA   (FP),Y
   LDY   #$01
   STA   (DPTR),Y
   LDY   #$14
   LDA   (FP),Y
   LDY   #$02
   STA   (DPTR),Y
   LDY   #$15
   LDA   (FP),Y
   LDY   #$03
   STA   (DPTR),Y
   LDY   #$01
   LDA   (FP),Y
   STA   @12.arr+9
   INY
   LDA   (FP),Y
   STA   @12.arr+10
   INY
   LDA   (FP),Y
   STA   @12.arr+11
   INY
   LDA   (FP),Y
   STA   @12.arr+12
   INY
   LDA   (FP),Y
   STA   @12.arr+13
   INY
   LDA   (FP),Y
   STA   @12.arr+14
   INY
   LDA   (FP),Y
   STA   @12.arr+15
   INY
   LDA   (FP),Y
   STA   @12.arr+16
   INY
   LDA   (FP),Y
   STA   @12.arr+17
   LDA   #<@12.arr
   LDY   #$18
   STA   (FP),Y
   LDA   #>@12.arr
   INY
   STA   (FP),Y
   LDA   #$01
   STA   HARGS
   LDA   #$00
   LDX   #$08
   LSR   HARGS
.mul_loop@30:
   BCC   .mul_skip@31
   CLC
   ADC   #$09
.mul_skip@31:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@30
   LDY   #$1B
   STA   (FP),Y
   LDA   HARGS
   DEY
   STA   (FP),Y
   LDY   #$18
   LDA   (FP),Y
   CLC
   LDY   #$1A
   ADC   (FP),Y
   LDY   #$1C
   STA   (FP),Y
   LDY   #$19
   LDA   (FP),Y
   LDY   #$1B
   ADC   (FP),Y
   LDY   #$1D
   STA   (FP),Y
   LDA   #<str@3
   INY
   STA   (FP),Y
   LDA   #>str@3
   INY
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$04
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDY   #$1C
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
   LDY   #$1F
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   JSR   strcmp
   LDY   #$20
   STA   (FP),Y
   TXA
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@5
   JMP   .or_true@18
.lb_skip@5:
   LDA   #<@12.arr
   LDY   #$22
   STA   (FP),Y
   LDA   #>@12.arr
   INY
   STA   (FP),Y
   LDA   #$01
   STA   HARGS
   LDA   #$00
   LDX   #$08
   LSR   HARGS
.mul_loop@32:
   BCC   .mul_skip@33
   CLC
   ADC   #$09
.mul_skip@33:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@32
   LDY   #$25
   STA   (FP),Y
   LDA   HARGS
   DEY
   STA   (FP),Y
   LDY   #$22
   LDA   (FP),Y
   CLC
   LDY   #$24
   ADC   (FP),Y
   LDY   #$26
   STA   (FP),Y
   LDY   #$23
   LDA   (FP),Y
   LDY   #$25
   ADC   (FP),Y
   LDY   #$27
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$03
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
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$2A
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$2B
   STA   (FP),Y
   CMP   #$27
   BNE   .cmp_true@34
   DEY
   LDA   (FP),Y
   CMP   #$10
   BNE   .cmp_true@34
   LDA   #$00
   JMP   .cmp_end@35
.cmp_true@34:
   LDA   #$01
.cmp_end@35:
   LDY   #$2C
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@18
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   JMP   .or_end@19
.or_true@18:
   LDA   #$01
   LDY   #$2E
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@19:
   LDY   #$2E
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@4
   JMP   .or_true@16
.lb_skip@4:
   LDA   #<@12.arr
   LDY   #$30
   STA   (FP),Y
   LDA   #>@12.arr
   INY
   STA   (FP),Y
   LDA   #$01
   STA   HARGS
   LDA   #$00
   LDX   #$08
   LSR   HARGS
.mul_loop@37:
   BCC   .mul_skip@38
   CLC
   ADC   #$09
.mul_skip@38:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@37
   LDY   #$33
   STA   (FP),Y
   LDA   HARGS
   DEY
   STA   (FP),Y
   LDY   #$30
   LDA   (FP),Y
   CLC
   LDY   #$32
   ADC   (FP),Y
   LDY   #$34
   STA   (FP),Y
   LDY   #$31
   LDA   (FP),Y
   LDY   #$33
   ADC   (FP),Y
   LDY   #$35
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$03
   LDY   #$36
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$37
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$02
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
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$3B
   STA   (FP),Y
   LDY   #$02
   LDA   (DPTR),Y
   LDY   #$3C
   STA   (FP),Y
   LDY   #$03
   LDA   (DPTR),Y
   LDY   #$3D
   STA   (FP),Y
   LDA   #$20
   INY
   STA   (FP),Y
   LDA   #$4E
   INY
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@39
   LDA   #$00
   JMP   .sx_done@40
.sx_neg@39:
   LDA   #$FF
.sx_done@40:
   LDY   #$40
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$3D
   LDA   (FP),Y
   LDY   #$41
   CMP   (FP),Y
   BNE   .cmp_true@41
   LDY   #$3C
   LDA   (FP),Y
   LDY   #$40
   CMP   (FP),Y
   BNE   .cmp_true@41
   LDY   #$3B
   LDA   (FP),Y
   LDY   #$3F
   CMP   (FP),Y
   BNE   .cmp_true@41
   LDY   #$3A
   LDA   (FP),Y
   LDY   #$3E
   CMP   (FP),Y
   BNE   .cmp_true@41
   LDA   #$00
   JMP   .cmp_end@42
.cmp_true@41:
   LDA   #$01
.cmp_end@42:
   LDY   #$42
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@16
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   JMP   .or_end@17
.or_true@16:
   LDA   #$01
   LDY   #$44
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@17:
   LDY   #$44
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@20
   LDX   #$00
   TXA

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$77
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$76
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
   LDA   #<@12.arr
   LDY   #$46
   STA   (FP),Y
   LDA   #>@12.arr
   INY
   STA   (FP),Y
   LDA   #$00
   STA   HARGS
   LDX   #$08
   LSR   HARGS
.mul_loop@44:
   BCC   .mul_skip@45
   CLC
   ADC   #$09
.mul_skip@45:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@44
   LDY   #$49
   STA   (FP),Y
   LDA   HARGS
   DEY
   STA   (FP),Y
   LDY   #$46
   LDA   (FP),Y
   CLC
   LDY   #$48
   ADC   (FP),Y
   LDY   #$4A
   STA   (FP),Y
   LDY   #$47
   LDA   (FP),Y
   LDY   #$49
   ADC   (FP),Y
   LDY   #$4B
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$03
   LDY   #$4C
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$4D
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$4E
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$4F
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@3
   JMP   .or_true@25
.lb_skip@3:
   LDA   #<@12.arr
   LDY   #$50
   STA   (FP),Y
   LDA   #>@12.arr
   INY
   STA   (FP),Y
   LDA   #$00
   STA   HARGS
   LDX   #$08
   LSR   HARGS
.mul_loop@46:
   BCC   .mul_skip@47
   CLC
   ADC   #$09
.mul_skip@47:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@46
   LDY   #$53
   STA   (FP),Y
   LDA   HARGS
   DEY
   STA   (FP),Y
   LDY   #$50
   LDA   (FP),Y
   CLC
   LDY   #$52
   ADC   (FP),Y
   LDY   #$54
   STA   (FP),Y
   LDY   #$51
   LDA   (FP),Y
   LDY   #$53
   ADC   (FP),Y
   LDY   #$55
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$03
   LDY   #$56
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$57
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$02
   LDY   #$58
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$59
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$5A
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$5B
   STA   (FP),Y
   LDY   #$02
   LDA   (DPTR),Y
   LDY   #$5C
   STA   (FP),Y
   LDY   #$03
   LDA   (DPTR),Y
   LDY   #$5D
   STA   (FP),Y
   LDY   #$5A
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   INY
   ORA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@25
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   JMP   .or_end@26
.or_true@25:
   LDA   #$01
   LDY   #$5E
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@26:
   LDY   #$5E
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@23
   LDA   #<@12.arr
   INY
   STA   (FP),Y
   LDA   #>@12.arr
   INY
   STA   (FP),Y
   LDA   #$02
   STA   HARGS
   LDA   #$00
   LDX   #$08
   LSR   HARGS
.mul_loop@48:
   BCC   .mul_skip@49
   CLC
   ADC   #$09
.mul_skip@49:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@48
   LDY   #$63
   STA   (FP),Y
   LDA   HARGS
   DEY
   STA   (FP),Y
   LDY   #$60
   LDA   (FP),Y
   CLC
   LDY   #$62
   ADC   (FP),Y
   LDY   #$64
   STA   (FP),Y
   LDY   #$61
   LDA   (FP),Y
   LDY   #$63
   ADC   (FP),Y
   LDY   #$65
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   LDY   #$66
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$67
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$68
   STA   (FP),Y
   ORA   #$00
   BNE   .or_true@23
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   JMP   .or_end@24
.or_true@23:
   LDA   #$01
   LDY   #$69
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@24:
   LDY   #$69
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@21
   LDA   #<@12.arr
   INY
   STA   (FP),Y
   LDA   #>@12.arr
   INY
   STA   (FP),Y
   LDA   #$02
   STA   HARGS
   LDA   #$00
   LDX   #$08
   LSR   HARGS
.mul_loop@50:
   BCC   .mul_skip@51
   CLC
   ADC   #$09
.mul_skip@51:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@50
   LDY   #$6E
   STA   (FP),Y
   LDA   HARGS
   DEY
   STA   (FP),Y
   LDY   #$6B
   LDA   (FP),Y
   CLC
   LDY   #$6D
   ADC   (FP),Y
   LDY   #$6F
   STA   (FP),Y
   LDY   #$6C
   LDA   (FP),Y
   LDY   #$6E
   ADC   (FP),Y
   LDY   #$70
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$01
   LDY   #$71
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$72
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$73
   STA   (FP),Y
   ORA   #$00
   BNE   .or_true@21
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   JMP   .or_end@22
.or_true@21:
   LDA   #$01
   LDY   #$74
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@22:
   LDY   #$74
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@27
   LDX   #$00
   TXA

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$77
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
.if_end@27:
   LDX   #$00
   LDA   #$01

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$77
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$76
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

test_copy_from_array_elem:
   SUBROUTINE

   ; prologue: 0 arg bytes, 150 local bytes
   SEC
   LDA   SSP
   SBC   #$98
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$97
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
   LDY   #$1C
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
   LDA   #$61
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$1C
   LDA   (FP),Y
   CLC
   ADC   #$01
   LDY   #$1E
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$1F
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDA   #$62
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$1C
   LDA   (FP),Y
   CLC
   ADC   #$02
   LDY   #$20
   STA   (FP),Y
   LDY   #$1D
   LDA   (FP),Y
   ADC   #$00
   LDY   #$21
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
   SEC
   SBC   #$B8
   LDY   #$22
   STA   (FP),Y
   LDA   #$00
   SBC   #$0B
   INY
   STA   (FP),Y
   LDY   #$1C
   LDA   (FP),Y
   CLC
   ADC   #$03
   LDY   #$24
   STA   (FP),Y
   LDY   #$1D
   LDA   (FP),Y
   ADC   #$00
   LDY   #$25
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
   LDY   #$23
   LDA   (FP),Y
   LDY   #$01
   STA   (DPTR),Y
   SEC
   LDA   #$00
   SBC   #$A0
   LDY   #$26
   STA   (FP),Y
   LDA   #$00
   SBC   #$0F
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$28
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$29
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@52
   LDA   #$00
   JMP   .sx_done@53
.sx_neg@52:
   LDA   #$FF
.sx_done@53:
   LDY   #$2A
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$1C
   LDA   (FP),Y
   CLC
   ADC   #$05
   LDY   #$2C
   STA   (FP),Y
   LDY   #$1D
   LDA   (FP),Y
   ADC   #$00
   LDY   #$2D
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$28
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$29
   LDA   (FP),Y
   LDY   #$01
   STA   (DPTR),Y
   LDY   #$2A
   LDA   (FP),Y
   LDY   #$02
   STA   (DPTR),Y
   LDY   #$2B
   LDA   (FP),Y
   LDY   #$03
   STA   (DPTR),Y
   LDY   #$1C
   LDA   (FP),Y
   CLC
   ADC   #$09
   LDY   #$2E
   STA   (FP),Y
   LDY   #$1D
   LDA   (FP),Y
   ADC   #$00
   LDY   #$2F
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDA   #$63
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$1C
   LDA   (FP),Y
   CLC
   ADC   #$0A
   LDY   #$30
   STA   (FP),Y
   LDY   #$1D
   LDA   (FP),Y
   ADC   #$00
   LDY   #$31
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
   LDY   #$1C
   LDA   (FP),Y
   CLC
   ADC   #$0B
   LDY   #$32
   STA   (FP),Y
   LDY   #$1D
   LDA   (FP),Y
   ADC   #$00
   LDY   #$33
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
   SEC
   SBC   #$88
   LDY   #$34
   STA   (FP),Y
   LDA   #$00
   SBC   #$13
   INY
   STA   (FP),Y
   LDY   #$1C
   LDA   (FP),Y
   CLC
   ADC   #$0C
   LDY   #$36
   STA   (FP),Y
   LDY   #$1D
   LDA   (FP),Y
   ADC   #$00
   LDY   #$37
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$34
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$35
   LDA   (FP),Y
   LDY   #$01
   STA   (DPTR),Y
   SEC
   LDA   #$00
   SBC   #$70
   LDY   #$38
   STA   (FP),Y
   LDA   #$00
   SBC   #$17
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$3A
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$3B
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@54
   LDA   #$00
   JMP   .sx_done@55
.sx_neg@54:
   LDA   #$FF
.sx_done@55:
   LDY   #$3C
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$1C
   LDA   (FP),Y
   CLC
   ADC   #$0E
   LDY   #$3E
   STA   (FP),Y
   LDY   #$1D
   LDA   (FP),Y
   ADC   #$00
   LDY   #$3F
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$3A
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$3B
   LDA   (FP),Y
   LDY   #$01
   STA   (DPTR),Y
   LDY   #$3C
   LDA   (FP),Y
   LDY   #$02
   STA   (DPTR),Y
   LDY   #$3D
   LDA   (FP),Y
   LDY   #$03
   STA   (DPTR),Y
   LDY   #$1C
   LDA   (FP),Y
   CLC
   ADC   #$12
   LDY   #$40
   STA   (FP),Y
   LDY   #$1D
   LDA   (FP),Y
   ADC   #$00
   LDY   #$41
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDA   #$65
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$1C
   LDA   (FP),Y
   CLC
   ADC   #$13
   LDY   #$42
   STA   (FP),Y
   LDY   #$1D
   LDA   (FP),Y
   ADC   #$00
   LDY   #$43
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDA   #$66
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$1C
   LDA   (FP),Y
   CLC
   ADC   #$14
   LDY   #$44
   STA   (FP),Y
   LDY   #$1D
   LDA   (FP),Y
   ADC   #$00
   LDY   #$45
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
   SEC
   SBC   #$58
   LDY   #$46
   STA   (FP),Y
   LDA   #$00
   SBC   #$1B
   INY
   STA   (FP),Y
   LDY   #$1C
   LDA   (FP),Y
   CLC
   ADC   #$15
   LDY   #$48
   STA   (FP),Y
   LDY   #$1D
   LDA   (FP),Y
   ADC   #$00
   LDY   #$49
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$46
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$47
   LDA   (FP),Y
   LDY   #$01
   STA   (DPTR),Y
   SEC
   LDA   #$00
   SBC   #$40
   LDY   #$4A
   STA   (FP),Y
   LDA   #$00
   SBC   #$1F
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$4C
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$4D
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@56
   LDA   #$00
   JMP   .sx_done@57
.sx_neg@56:
   LDA   #$FF
.sx_done@57:
   LDY   #$4E
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$1C
   LDA   (FP),Y
   CLC
   ADC   #$17
   LDY   #$50
   STA   (FP),Y
   LDY   #$1D
   LDA   (FP),Y
   ADC   #$00
   LDY   #$51
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$4C
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$4D
   LDA   (FP),Y
   LDY   #$01
   STA   (DPTR),Y
   LDY   #$4E
   LDA   (FP),Y
   LDY   #$02
   STA   (DPTR),Y
   LDY   #$4F
   LDA   (FP),Y
   LDY   #$03
   STA   (DPTR),Y
   LDA   #$09
   LDY   #$52
   STA   (FP),Y
.clear@28:
   SEC
   SBC   #$01
   LDY   #$52
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   CLC
   LDA   FP
   ADC   #$55
   LDY   #$5E
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   LDY   #$53
   ADC   (FP),Y
   LDY   #$60
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$54
   ADC   (FP),Y
   LDY   #$61
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
   LDY   #$52
   LDA   (FP),Y
   BNE   .clear@28
   CLC
   LDA   FP
   ADC   #$01
   LDY   #$64
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
.mul_loop@58:
   BCC   .mul_skip@59
   CLC
   ADC   #$09
.mul_skip@59:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@58
   LDY   #$63
   STA   (FP),Y
   LDA   HARGS
   DEY
   STA   (FP),Y
   LDY   #$64
   LDA   (FP),Y
   CLC
   LDY   #$62
   ADC   (FP),Y
   LDY   #$66
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$63
   ADC   (FP),Y
   LDY   #$67
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$68
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$69
   STA   (FP),Y
   LDY   #$02
   LDA   (DPTR),Y
   LDY   #$6A
   STA   (FP),Y
   LDY   #$03
   LDA   (DPTR),Y
   LDY   #$6B
   STA   (FP),Y
   LDY   #$04
   LDA   (DPTR),Y
   LDY   #$6C
   STA   (FP),Y
   LDY   #$05
   LDA   (DPTR),Y
   LDY   #$6D
   STA   (FP),Y
   LDY   #$06
   LDA   (DPTR),Y
   LDY   #$6E
   STA   (FP),Y
   LDY   #$07
   LDA   (DPTR),Y
   LDY   #$6F
   STA   (FP),Y
   LDY   #$08
   LDA   (DPTR),Y
   LDY   #$70
   STA   (FP),Y
   LDY   #$68
   LDA   (FP),Y
   LDY   #$55
   STA   (FP),Y
   LDY   #$69
   LDA   (FP),Y
   LDY   #$56
   STA   (FP),Y
   LDY   #$6A
   LDA   (FP),Y
   LDY   #$57
   STA   (FP),Y
   LDY   #$6B
   LDA   (FP),Y
   LDY   #$58
   STA   (FP),Y
   LDY   #$6C
   LDA   (FP),Y
   LDY   #$59
   STA   (FP),Y
   LDY   #$6D
   LDA   (FP),Y
   LDY   #$5A
   STA   (FP),Y
   LDY   #$6E
   LDA   (FP),Y
   LDY   #$5B
   STA   (FP),Y
   LDY   #$6F
   LDA   (FP),Y
   LDY   #$5C
   STA   (FP),Y
   LDY   #$70
   LDA   (FP),Y
   LDY   #$5D
   STA   (FP),Y
   CLC
   LDA   FP
   ADC   #$55
   LDY   #$73
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   LDA   #<str@4
   LDY   #$71
   STA   (FP),Y
   LDA   #>str@4
   INY
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$04
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   INY
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$74
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$71
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$72
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   JSR   strcmp
   LDY   #$75
   STA   (FP),Y
   TXA
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@7
   JMP   .or_true@31
.lb_skip@7:
   CLC
   LDA   FP
   ADC   #$55
   LDY   #$77
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$03
   LDY   #$79
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$7A
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
   SEC
   LDA   #$00
   SBC   #$88
   INY
   STA   (FP),Y
   LDA   #$00
   SBC   #$13
   INY
   STA   (FP),Y
   LDY   #$7C
   LDA   (FP),Y
   LDY   #$7E
   CMP   (FP),Y
   BNE   .cmp_true@60
   LDY   #$7B
   LDA   (FP),Y
   LDY   #$7D
   CMP   (FP),Y
   BNE   .cmp_true@60
   LDA   #$00
   JMP   .cmp_end@61
.cmp_true@60:
   LDA   #$01
.cmp_end@61:
   LDY   #$7F
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
   LDY   #$81
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@32:
   LDY   #$81
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@6
   JMP   .or_true@29
.lb_skip@6:
   CLC
   LDA   FP
   ADC   #$55
   LDY   #$83
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$03
   LDY   #$85
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$86
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$02
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
   LDY   #$02
   LDA   (DPTR),Y
   LDY   #$8B
   STA   (FP),Y
   LDY   #$03
   LDA   (DPTR),Y
   LDY   #$8C
   STA   (FP),Y
   SEC
   LDA   #$00
   SBC   #$70
   INY
   STA   (FP),Y
   LDA   #$00
   SBC   #$17
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$8F
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$90
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@63
   LDA   #$00
   JMP   .sx_done@64
.sx_neg@63:
   LDA   #$FF
.sx_done@64:
   LDY   #$91
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$8C
   LDA   (FP),Y
   LDY   #$92
   CMP   (FP),Y
   BNE   .cmp_true@65
   LDY   #$8B
   LDA   (FP),Y
   LDY   #$91
   CMP   (FP),Y
   BNE   .cmp_true@65
   LDY   #$8A
   LDA   (FP),Y
   LDY   #$90
   CMP   (FP),Y
   BNE   .cmp_true@65
   LDY   #$89
   LDA   (FP),Y
   LDY   #$8F
   CMP   (FP),Y
   BNE   .cmp_true@65
   LDA   #$00
   JMP   .cmp_end@66
.cmp_true@65:
   LDA   #$01
.cmp_end@66:
   LDY   #$93
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
   LDY   #$95
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@30:
   LDY   #$95
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
   ADC   #$98
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
.if_end@33:
   LDX   #$00
   LDA   #$01

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$98
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$97
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

test_copy_to_and_from_array_elem:
   SUBROUTINE

   ; prologue: 0 arg bytes, 252 local bytes
   SEC
   LDA   SSP
   SBC   #$FE
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$FD
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
   LDY   #$1C
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
   LDA   #$61
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$1C
   LDA   (FP),Y
   CLC
   ADC   #$01
   LDY   #$1E
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$1F
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDA   #$62
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$1C
   LDA   (FP),Y
   CLC
   ADC   #$02
   LDY   #$20
   STA   (FP),Y
   LDY   #$1D
   LDA   (FP),Y
   ADC   #$00
   LDY   #$21
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
   SEC
   SBC   #$B8
   LDY   #$22
   STA   (FP),Y
   LDA   #$00
   SBC   #$0B
   INY
   STA   (FP),Y
   LDY   #$1C
   LDA   (FP),Y
   CLC
   ADC   #$03
   LDY   #$24
   STA   (FP),Y
   LDY   #$1D
   LDA   (FP),Y
   ADC   #$00
   LDY   #$25
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
   LDY   #$23
   LDA   (FP),Y
   LDY   #$01
   STA   (DPTR),Y
   SEC
   LDA   #$00
   SBC   #$A0
   LDY   #$26
   STA   (FP),Y
   LDA   #$00
   SBC   #$0F
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$28
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$29
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@68
   LDA   #$00
   JMP   .sx_done@69
.sx_neg@68:
   LDA   #$FF
.sx_done@69:
   LDY   #$2A
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$1C
   LDA   (FP),Y
   CLC
   ADC   #$05
   LDY   #$2C
   STA   (FP),Y
   LDY   #$1D
   LDA   (FP),Y
   ADC   #$00
   LDY   #$2D
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$28
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$29
   LDA   (FP),Y
   LDY   #$01
   STA   (DPTR),Y
   LDY   #$2A
   LDA   (FP),Y
   LDY   #$02
   STA   (DPTR),Y
   LDY   #$2B
   LDA   (FP),Y
   LDY   #$03
   STA   (DPTR),Y
   LDY   #$1C
   LDA   (FP),Y
   CLC
   ADC   #$09
   LDY   #$2E
   STA   (FP),Y
   LDY   #$1D
   LDA   (FP),Y
   ADC   #$00
   LDY   #$2F
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDA   #$63
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$1C
   LDA   (FP),Y
   CLC
   ADC   #$0A
   LDY   #$30
   STA   (FP),Y
   LDY   #$1D
   LDA   (FP),Y
   ADC   #$00
   LDY   #$31
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
   LDY   #$1C
   LDA   (FP),Y
   CLC
   ADC   #$0B
   LDY   #$32
   STA   (FP),Y
   LDY   #$1D
   LDA   (FP),Y
   ADC   #$00
   LDY   #$33
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
   SEC
   SBC   #$88
   LDY   #$34
   STA   (FP),Y
   LDA   #$00
   SBC   #$13
   INY
   STA   (FP),Y
   LDY   #$1C
   LDA   (FP),Y
   CLC
   ADC   #$0C
   LDY   #$36
   STA   (FP),Y
   LDY   #$1D
   LDA   (FP),Y
   ADC   #$00
   LDY   #$37
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$34
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$35
   LDA   (FP),Y
   LDY   #$01
   STA   (DPTR),Y
   SEC
   LDA   #$00
   SBC   #$70
   LDY   #$38
   STA   (FP),Y
   LDA   #$00
   SBC   #$17
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$3A
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$3B
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@70
   LDA   #$00
   JMP   .sx_done@71
.sx_neg@70:
   LDA   #$FF
.sx_done@71:
   LDY   #$3C
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$1C
   LDA   (FP),Y
   CLC
   ADC   #$0E
   LDY   #$3E
   STA   (FP),Y
   LDY   #$1D
   LDA   (FP),Y
   ADC   #$00
   LDY   #$3F
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$3A
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$3B
   LDA   (FP),Y
   LDY   #$01
   STA   (DPTR),Y
   LDY   #$3C
   LDA   (FP),Y
   LDY   #$02
   STA   (DPTR),Y
   LDY   #$3D
   LDA   (FP),Y
   LDY   #$03
   STA   (DPTR),Y
   LDY   #$1C
   LDA   (FP),Y
   CLC
   ADC   #$12
   LDY   #$40
   STA   (FP),Y
   LDY   #$1D
   LDA   (FP),Y
   ADC   #$00
   LDY   #$41
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDA   #$65
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$1C
   LDA   (FP),Y
   CLC
   ADC   #$13
   LDY   #$42
   STA   (FP),Y
   LDY   #$1D
   LDA   (FP),Y
   ADC   #$00
   LDY   #$43
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDA   #$66
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$1C
   LDA   (FP),Y
   CLC
   ADC   #$14
   LDY   #$44
   STA   (FP),Y
   LDY   #$1D
   LDA   (FP),Y
   ADC   #$00
   LDY   #$45
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
   SEC
   SBC   #$58
   LDY   #$46
   STA   (FP),Y
   LDA   #$00
   SBC   #$1B
   INY
   STA   (FP),Y
   LDY   #$1C
   LDA   (FP),Y
   CLC
   ADC   #$15
   LDY   #$48
   STA   (FP),Y
   LDY   #$1D
   LDA   (FP),Y
   ADC   #$00
   LDY   #$49
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$46
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$47
   LDA   (FP),Y
   LDY   #$01
   STA   (DPTR),Y
   SEC
   LDA   #$00
   SBC   #$40
   LDY   #$4A
   STA   (FP),Y
   LDA   #$00
   SBC   #$1F
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$4C
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$4D
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@72
   LDA   #$00
   JMP   .sx_done@73
.sx_neg@72:
   LDA   #$FF
.sx_done@73:
   LDY   #$4E
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$1C
   LDA   (FP),Y
   CLC
   ADC   #$17
   LDY   #$50
   STA   (FP),Y
   LDY   #$1D
   LDA   (FP),Y
   ADC   #$00
   LDY   #$51
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$4C
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$4D
   LDA   (FP),Y
   LDY   #$01
   STA   (DPTR),Y
   LDY   #$4E
   LDA   (FP),Y
   LDY   #$02
   STA   (DPTR),Y
   LDY   #$4F
   LDA   (FP),Y
   LDY   #$03
   STA   (DPTR),Y
   CLC
   LDA   FP
   ADC   #$01
   LDY   #$54
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
.mul_loop@74:
   BCC   .mul_skip@75
   CLC
   ADC   #$09
.mul_skip@75:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@74
   LDY   #$53
   STA   (FP),Y
   LDA   HARGS
   DEY
   STA   (FP),Y
   LDY   #$54
   LDA   (FP),Y
   CLC
   LDY   #$52
   ADC   (FP),Y
   LDY   #$56
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$53
   ADC   (FP),Y
   LDY   #$57
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$58
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$59
   STA   (FP),Y
   LDY   #$02
   LDA   (DPTR),Y
   LDY   #$5A
   STA   (FP),Y
   LDY   #$03
   LDA   (DPTR),Y
   LDY   #$5B
   STA   (FP),Y
   LDY   #$04
   LDA   (DPTR),Y
   LDY   #$5C
   STA   (FP),Y
   LDY   #$05
   LDA   (DPTR),Y
   LDY   #$5D
   STA   (FP),Y
   LDY   #$06
   LDA   (DPTR),Y
   LDY   #$5E
   STA   (FP),Y
   LDY   #$07
   LDA   (DPTR),Y
   LDY   #$5F
   STA   (FP),Y
   LDY   #$08
   LDA   (DPTR),Y
   LDY   #$60
   STA   (FP),Y
   CLC
   LDA   FP
   ADC   #$01
   LDY   #$63
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   LDA   #$00
   STA   HARGS
   LDX   #$08
   LSR   HARGS
.mul_loop@76:
   BCC   .mul_skip@77
   CLC
   ADC   #$09
.mul_skip@77:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@76
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
   LDY   #$58
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$59
   LDA   (FP),Y
   LDY   #$01
   STA   (DPTR),Y
   LDY   #$5A
   LDA   (FP),Y
   LDY   #$02
   STA   (DPTR),Y
   LDY   #$5B
   LDA   (FP),Y
   LDY   #$03
   STA   (DPTR),Y
   LDY   #$5C
   LDA   (FP),Y
   LDY   #$04
   STA   (DPTR),Y
   LDY   #$5D
   LDA   (FP),Y
   LDY   #$05
   STA   (DPTR),Y
   LDY   #$5E
   LDA   (FP),Y
   LDY   #$06
   STA   (DPTR),Y
   LDY   #$5F
   LDA   (FP),Y
   LDY   #$07
   STA   (DPTR),Y
   LDY   #$60
   LDA   (FP),Y
   LDY   #$08
   STA   (DPTR),Y
   CLC
   LDA   FP
   ADC   #$01
   LDY   #$69
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   LDA   #$00
   STA   HARGS
   LDX   #$08
   LSR   HARGS
.mul_loop@78:
   BCC   .mul_skip@79
   CLC
   ADC   #$09
.mul_skip@79:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@78
   LDY   #$68
   STA   (FP),Y
   LDA   HARGS
   DEY
   STA   (FP),Y
   LDY   #$69
   LDA   (FP),Y
   CLC
   LDY   #$67
   ADC   (FP),Y
   LDY   #$6B
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$68
   ADC   (FP),Y
   LDY   #$6C
   STA   (FP),Y
   LDA   #<str@5
   INY
   STA   (FP),Y
   LDA   #>str@5
   INY
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$04
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDY   #$6B
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$6C
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$6D
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$6E
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   JSR   strcmp
   LDY   #$6F
   STA   (FP),Y
   TXA
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@13
   JMP   .or_true@36
.lb_skip@13:
   CLC
   LDA   FP
   ADC   #$01
   LDY   #$73
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   LDA   #$00
   STA   HARGS
   LDX   #$08
   LSR   HARGS
.mul_loop@80:
   BCC   .mul_skip@81
   CLC
   ADC   #$09
.mul_skip@81:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@80
   LDY   #$72
   STA   (FP),Y
   LDA   HARGS
   DEY
   STA   (FP),Y
   LDY   #$73
   LDA   (FP),Y
   CLC
   LDY   #$71
   ADC   (FP),Y
   LDY   #$75
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$72
   ADC   (FP),Y
   LDY   #$76
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$03
   LDY   #$77
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
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
   SEC
   LDA   #$00
   SBC   #$58
   INY
   STA   (FP),Y
   LDA   #$00
   SBC   #$1B
   INY
   STA   (FP),Y
   LDY   #$7A
   LDA   (FP),Y
   LDY   #$7C
   CMP   (FP),Y
   BNE   .cmp_true@82
   LDY   #$79
   LDA   (FP),Y
   LDY   #$7B
   CMP   (FP),Y
   BNE   .cmp_true@82
   LDA   #$00
   JMP   .cmp_end@83
.cmp_true@82:
   LDA   #$01
.cmp_end@83:
   LDY   #$7D
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@36
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   JMP   .or_end@37
.or_true@36:
   LDA   #$01
   LDY   #$7F
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@37:
   LDY   #$7F
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@12
   JMP   .or_true@34
.lb_skip@12:
   CLC
   LDA   FP
   ADC   #$01
   LDY   #$83
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   LDA   #$00
   STA   HARGS
   LDX   #$08
   LSR   HARGS
.mul_loop@85:
   BCC   .mul_skip@86
   CLC
   ADC   #$09
.mul_skip@86:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@85
   LDY   #$82
   STA   (FP),Y
   LDA   HARGS
   DEY
   STA   (FP),Y
   LDY   #$83
   LDA   (FP),Y
   CLC
   LDY   #$81
   ADC   (FP),Y
   LDY   #$85
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$82
   ADC   (FP),Y
   LDY   #$86
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$03
   LDY   #$87
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$88
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$02
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
   SEC
   LDA   #$00
   SBC   #$40
   INY
   STA   (FP),Y
   LDA   #$00
   SBC   #$1F
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$91
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$92
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@87
   LDA   #$00
   JMP   .sx_done@88
.sx_neg@87:
   LDA   #$FF
.sx_done@88:
   LDY   #$93
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$8E
   LDA   (FP),Y
   LDY   #$94
   CMP   (FP),Y
   BNE   .cmp_true@89
   LDY   #$8D
   LDA   (FP),Y
   LDY   #$93
   CMP   (FP),Y
   BNE   .cmp_true@89
   LDY   #$8C
   LDA   (FP),Y
   LDY   #$92
   CMP   (FP),Y
   BNE   .cmp_true@89
   LDY   #$8B
   LDA   (FP),Y
   LDY   #$91
   CMP   (FP),Y
   BNE   .cmp_true@89
   LDA   #$00
   JMP   .cmp_end@90
.cmp_true@89:
   LDA   #$01
.cmp_end@90:
   LDY   #$95
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
   LDY   #$97
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@35:
   LDY   #$97
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
   ADC   #$FE
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$FD
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
   CLC
   LDA   FP
   ADC   #$01
   LDY   #$9B
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
.mul_loop@92:
   BCC   .mul_skip@93
   CLC
   ADC   #$09
.mul_skip@93:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@92
   LDY   #$9A
   STA   (FP),Y
   LDA   HARGS
   DEY
   STA   (FP),Y
   LDY   #$9B
   LDA   (FP),Y
   CLC
   LDY   #$99
   ADC   (FP),Y
   LDY   #$9D
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$9A
   ADC   (FP),Y
   LDY   #$9E
   STA   (FP),Y
   LDA   #<str@6
   INY
   STA   (FP),Y
   LDA   #>str@6
   INY
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$04
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDY   #$9D
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$9E
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$9F
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$A0
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   JSR   strcmp
   LDY   #$A1
   STA   (FP),Y
   TXA
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@11
   JMP   .or_true@41
.lb_skip@11:
   CLC
   LDA   FP
   ADC   #$01
   LDY   #$A5
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
.mul_loop@94:
   BCC   .mul_skip@95
   CLC
   ADC   #$09
.mul_skip@95:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@94
   LDY   #$A4
   STA   (FP),Y
   LDA   HARGS
   DEY
   STA   (FP),Y
   LDY   #$A5
   LDA   (FP),Y
   CLC
   LDY   #$A3
   ADC   (FP),Y
   LDY   #$A7
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$A4
   ADC   (FP),Y
   LDY   #$A8
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$03
   LDY   #$A9
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$AA
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$AB
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$AC
   STA   (FP),Y
   SEC
   LDA   #$00
   SBC   #$88
   INY
   STA   (FP),Y
   LDA   #$00
   SBC   #$13
   INY
   STA   (FP),Y
   LDY   #$AC
   LDA   (FP),Y
   LDY   #$AE
   CMP   (FP),Y
   BNE   .cmp_true@96
   LDY   #$AB
   LDA   (FP),Y
   LDY   #$AD
   CMP   (FP),Y
   BNE   .cmp_true@96
   LDA   #$00
   JMP   .cmp_end@97
.cmp_true@96:
   LDA   #$01
.cmp_end@97:
   LDY   #$AF
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@41
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   JMP   .or_end@42
.or_true@41:
   LDA   #$01
   LDY   #$B1
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@42:
   LDY   #$B1
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@10
   JMP   .or_true@39
.lb_skip@10:
   CLC
   LDA   FP
   ADC   #$01
   LDY   #$B5
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
.mul_loop@99:
   BCC   .mul_skip@100
   CLC
   ADC   #$09
.mul_skip@100:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@99
   LDY   #$B4
   STA   (FP),Y
   LDA   HARGS
   DEY
   STA   (FP),Y
   LDY   #$B5
   LDA   (FP),Y
   CLC
   LDY   #$B3
   ADC   (FP),Y
   LDY   #$B7
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$B4
   ADC   (FP),Y
   LDY   #$B8
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$03
   LDY   #$B9
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$BA
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$02
   LDY   #$BB
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$BC
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$BD
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$BE
   STA   (FP),Y
   LDY   #$02
   LDA   (DPTR),Y
   LDY   #$BF
   STA   (FP),Y
   LDY   #$03
   LDA   (DPTR),Y
   LDY   #$C0
   STA   (FP),Y
   SEC
   LDA   #$00
   SBC   #$70
   INY
   STA   (FP),Y
   LDA   #$00
   SBC   #$17
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$C3
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$C4
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@101
   LDA   #$00
   JMP   .sx_done@102
.sx_neg@101:
   LDA   #$FF
.sx_done@102:
   LDY   #$C5
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$C0
   LDA   (FP),Y
   LDY   #$C6
   CMP   (FP),Y
   BNE   .cmp_true@103
   LDY   #$BF
   LDA   (FP),Y
   LDY   #$C5
   CMP   (FP),Y
   BNE   .cmp_true@103
   LDY   #$BE
   LDA   (FP),Y
   LDY   #$C4
   CMP   (FP),Y
   BNE   .cmp_true@103
   LDY   #$BD
   LDA   (FP),Y
   LDY   #$C3
   CMP   (FP),Y
   BNE   .cmp_true@103
   LDA   #$00
   JMP   .cmp_end@104
.cmp_true@103:
   LDA   #$01
.cmp_end@104:
   LDY   #$C7
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@39
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   JMP   .or_end@40
.or_true@39:
   LDA   #$01
   LDY   #$C9
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@40:
   LDY   #$C9
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@43
   LDX   #$00
   TXA

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
   LDY   #$FD
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
.if_end@43:
   CLC
   LDA   FP
   ADC   #$01
   LDY   #$CD
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
.mul_loop@106:
   BCC   .mul_skip@107
   CLC
   ADC   #$09
.mul_skip@107:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@106
   LDY   #$CC
   STA   (FP),Y
   LDA   HARGS
   DEY
   STA   (FP),Y
   LDY   #$CD
   LDA   (FP),Y
   CLC
   LDY   #$CB
   ADC   (FP),Y
   LDY   #$CF
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$CC
   ADC   (FP),Y
   LDY   #$D0
   STA   (FP),Y
   LDA   #<str@7
   INY
   STA   (FP),Y
   LDA   #>str@7
   INY
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$04
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDY   #$CF
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$D0
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$D1
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$D2
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   JSR   strcmp
   LDY   #$D3
   STA   (FP),Y
   TXA
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@9
   JMP   .or_true@46
.lb_skip@9:
   CLC
   LDA   FP
   ADC   #$01
   LDY   #$D7
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
.mul_loop@108:
   BCC   .mul_skip@109
   CLC
   ADC   #$09
.mul_skip@109:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@108
   LDY   #$D6
   STA   (FP),Y
   LDA   HARGS
   DEY
   STA   (FP),Y
   LDY   #$D7
   LDA   (FP),Y
   CLC
   LDY   #$D5
   ADC   (FP),Y
   LDY   #$D9
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$D6
   ADC   (FP),Y
   LDY   #$DA
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$03
   LDY   #$DB
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$DC
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$DD
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$DE
   STA   (FP),Y
   SEC
   LDA   #$00
   SBC   #$58
   INY
   STA   (FP),Y
   LDA   #$00
   SBC   #$1B
   INY
   STA   (FP),Y
   LDY   #$DE
   LDA   (FP),Y
   LDY   #$E0
   CMP   (FP),Y
   BNE   .cmp_true@110
   LDY   #$DD
   LDA   (FP),Y
   LDY   #$DF
   CMP   (FP),Y
   BNE   .cmp_true@110
   LDA   #$00
   JMP   .cmp_end@111
.cmp_true@110:
   LDA   #$01
.cmp_end@111:
   LDY   #$E1
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@46
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   JMP   .or_end@47
.or_true@46:
   LDA   #$01
   LDY   #$E3
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@47:
   LDY   #$E3
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@8
   JMP   .or_true@44
.lb_skip@8:
   CLC
   LDA   FP
   ADC   #$01
   LDY   #$E7
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
.mul_loop@113:
   BCC   .mul_skip@114
   CLC
   ADC   #$09
.mul_skip@114:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@113
   LDY   #$E6
   STA   (FP),Y
   LDA   HARGS
   DEY
   STA   (FP),Y
   LDY   #$E7
   LDA   (FP),Y
   CLC
   LDY   #$E5
   ADC   (FP),Y
   LDY   #$E9
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$E6
   ADC   (FP),Y
   LDY   #$EA
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$03
   LDY   #$EB
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$EC
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$02
   LDY   #$ED
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$EE
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$EF
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$F0
   STA   (FP),Y
   LDY   #$02
   LDA   (DPTR),Y
   LDY   #$F1
   STA   (FP),Y
   LDY   #$03
   LDA   (DPTR),Y
   LDY   #$F2
   STA   (FP),Y
   SEC
   LDA   #$00
   SBC   #$40
   INY
   STA   (FP),Y
   LDA   #$00
   SBC   #$1F
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$F5
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$F6
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@115
   LDA   #$00
   JMP   .sx_done@116
.sx_neg@115:
   LDA   #$FF
.sx_done@116:
   LDY   #$F7
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$F2
   LDA   (FP),Y
   LDY   #$F8
   CMP   (FP),Y
   BNE   .cmp_true@117
   LDY   #$F1
   LDA   (FP),Y
   LDY   #$F7
   CMP   (FP),Y
   BNE   .cmp_true@117
   LDY   #$F0
   LDA   (FP),Y
   LDY   #$F6
   CMP   (FP),Y
   BNE   .cmp_true@117
   LDY   #$EF
   LDA   (FP),Y
   LDY   #$F5
   CMP   (FP),Y
   BNE   .cmp_true@117
   LDA   #$00
   JMP   .cmp_end@118
.cmp_true@117:
   LDA   #$01
.cmp_end@118:
   LDY   #$F9
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@44
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   JMP   .or_end@45
.or_true@44:
   LDA   #$01
   LDY   #$FB
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@45:
   LDY   #$FB
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@48
   LDX   #$00
   TXA

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
.if_end@48:
   LDX   #$00
   LDA   #$01

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
   LDY   #$FD
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

test_copy_array_element_with_padding:
   SUBROUTINE

   ; prologue: 0 arg bytes, 175 local bytes
   SEC
   LDA   SSP
   SBC   #$B1
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$B0
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
   LDY   #$10
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
   LDY   #$10
   LDA   (FP),Y
   CLC
   ADC   #$02
   LDY   #$12
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$13
   STA   (FP),Y
   DEY
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
   LDA   #$02
   LDY   #$14
   STA   (FP),Y
   LDY   #$10
   LDA   (FP),Y
   CLC
   ADC   #$04
   LDY   #$15
   STA   (FP),Y
   LDY   #$11
   LDA   (FP),Y
   ADC   #$00
   LDY   #$16
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$14
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$10
   LDA   (FP),Y
   CLC
   ADC   #$05
   LDY   #$17
   STA   (FP),Y
   LDY   #$11
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
   LDA   #$03
   LDY   #$00
   STA   (DPTR),Y
   LDA   #$00
   INY
   STA   (DPTR),Y
   LDY   #$10
   LDA   (FP),Y
   CLC
   ADC   #$07
   LDY   #$19
   STA   (FP),Y
   LDY   #$11
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
   LDA   #$04
   LDY   #$00
   STA   (DPTR),Y
   LDA   #$00
   INY
   STA   (DPTR),Y
   LDA   #$05
   LDY   #$1B
   STA   (FP),Y
   LDY   #$10
   LDA   (FP),Y
   CLC
   ADC   #$09
   LDY   #$1C
   STA   (FP),Y
   LDY   #$11
   LDA   (FP),Y
   ADC   #$00
   LDY   #$1D
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$1B
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$10
   LDA   (FP),Y
   CLC
   ADC   #$0A
   LDY   #$1E
   STA   (FP),Y
   LDY   #$11
   LDA   (FP),Y
   ADC   #$00
   LDY   #$1F
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDA   #$06
   LDY   #$00
   STA   (DPTR),Y
   LDA   #$00
   INY
   STA   (DPTR),Y
   LDY   #$10
   LDA   (FP),Y
   CLC
   ADC   #$0C
   LDY   #$20
   STA   (FP),Y
   LDY   #$11
   LDA   (FP),Y
   ADC   #$00
   LDY   #$21
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDA   #$07
   LDY   #$00
   STA   (DPTR),Y
   LDA   #$00
   INY
   STA   (DPTR),Y
   LDA   #$08
   LDY   #$22
   STA   (FP),Y
   LDY   #$10
   LDA   (FP),Y
   CLC
   ADC   #$0E
   LDY   #$23
   STA   (FP),Y
   LDY   #$11
   LDA   (FP),Y
   ADC   #$00
   LDY   #$24
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
   CLC
   LDA   FP
   ADC   #$25
   LDY   #$2A
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
   LDA   #$09
   LDY   #$00
   STA   (DPTR),Y
   LDA   #$00
   INY
   STA   (DPTR),Y
   LDY   #$2A
   LDA   (FP),Y
   CLC
   ADC   #$02
   LDY   #$2C
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$2D
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDA   #$09
   LDY   #$00
   STA   (DPTR),Y
   LDA   #$00
   INY
   STA   (DPTR),Y
   LDA   #$09
   LDY   #$2E
   STA   (FP),Y
   LDY   #$2A
   LDA   (FP),Y
   CLC
   ADC   #$04
   LDY   #$2F
   STA   (FP),Y
   LDY   #$2B
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
   LDY   #$2E
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   CLC
   LDA   FP
   ADC   #$01
   LDY   #$33
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
.mul_loop@120:
   BCC   .mul_skip@121
   CLC
   ADC   #$05
.mul_skip@121:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@120
   LDY   #$32
   STA   (FP),Y
   LDA   HARGS
   DEY
   STA   (FP),Y
   LDY   #$33
   LDA   (FP),Y
   CLC
   LDY   #$31
   ADC   (FP),Y
   LDY   #$35
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$32
   ADC   (FP),Y
   LDY   #$36
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$25
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$26
   LDA   (FP),Y
   LDY   #$01
   STA   (DPTR),Y
   LDY   #$27
   LDA   (FP),Y
   LDY   #$02
   STA   (DPTR),Y
   LDY   #$28
   LDA   (FP),Y
   LDY   #$03
   STA   (DPTR),Y
   LDY   #$29
   LDA   (FP),Y
   LDY   #$04
   STA   (DPTR),Y
   CLC
   LDA   FP
   ADC   #$01
   LDY   #$39
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   LDA   #$00
   STA   HARGS
   LDX   #$08
   LSR   HARGS
.mul_loop@122:
   BCC   .mul_skip@123
   CLC
   ADC   #$05
.mul_skip@123:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@122
   LDY   #$38
   STA   (FP),Y
   LDA   HARGS
   DEY
   STA   (FP),Y
   LDY   #$39
   LDA   (FP),Y
   CLC
   LDY   #$37
   ADC   (FP),Y
   LDY   #$3B
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$38
   ADC   (FP),Y
   LDY   #$3C
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
   CMP   #$00
   BNE   .cmp_true@124
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@124
   LDA   #$00
   JMP   .cmp_end@125
.cmp_true@124:
   LDA   #$01
.cmp_end@125:
   LDY   #$3F
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@21
   JMP   .or_true@63
.lb_skip@21:
   CLC
   LDA   FP
   ADC   #$01
   LDY   #$43
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   LDA   #$00
   STA   HARGS
   LDX   #$08
   LSR   HARGS
.mul_loop@127:
   BCC   .mul_skip@128
   CLC
   ADC   #$05
.mul_skip@128:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@127
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
   CLC
   ADC   #$02
   LDY   #$47
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
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
   BNE   .cmp_true@129
   DEY
   LDA   (FP),Y
   CMP   #$01
   BNE   .cmp_true@129
   LDA   #$00
   JMP   .cmp_end@130
.cmp_true@129:
   LDA   #$01
.cmp_end@130:
   LDY   #$4B
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@63
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   JMP   .or_end@64
.or_true@63:
   LDA   #$01
   LDY   #$4D
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@64:
   LDY   #$4D
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@20
   JMP   .or_true@61
.lb_skip@20:
   CLC
   LDA   FP
   ADC   #$01
   LDY   #$51
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   LDA   #$00
   STA   HARGS
   LDX   #$08
   LSR   HARGS
.mul_loop@132:
   BCC   .mul_skip@133
   CLC
   ADC   #$05
.mul_skip@133:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@132
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
   CLC
   ADC   #$04
   LDY   #$55
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
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
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CMP   #$02
   BNE   .cmp_true@134
   LDA   #$00
   JMP   .cmp_end@135
.cmp_true@134:
   LDA   #$01
.cmp_end@135:
   LDY   #$5A
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@61
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   JMP   .or_end@62
.or_true@61:
   LDA   #$01
   LDY   #$5C
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@62:
   LDY   #$5C
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@19
   JMP   .or_true@59
.lb_skip@19:
   CLC
   LDA   FP
   ADC   #$01
   LDY   #$60
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
.mul_loop@136:
   BCC   .mul_skip@137
   CLC
   ADC   #$05
.mul_skip@137:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@136
   LDY   #$5F
   STA   (FP),Y
   LDA   HARGS
   DEY
   STA   (FP),Y
   LDY   #$60
   LDA   (FP),Y
   CLC
   LDY   #$5E
   ADC   (FP),Y
   LDY   #$62
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$5F
   ADC   (FP),Y
   LDY   #$63
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$64
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$65
   STA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@138
   DEY
   LDA   (FP),Y
   CMP   #$09
   BNE   .cmp_true@138
   LDA   #$00
   JMP   .cmp_end@139
.cmp_true@138:
   LDA   #$01
.cmp_end@139:
   LDY   #$66
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@59
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   JMP   .or_end@60
.or_true@59:
   LDA   #$01
   LDY   #$68
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@60:
   LDY   #$68
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@18
   JMP   .or_true@57
.lb_skip@18:
   CLC
   LDA   FP
   ADC   #$01
   LDY   #$6C
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
.mul_loop@141:
   BCC   .mul_skip@142
   CLC
   ADC   #$05
.mul_skip@142:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@141
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
   CLC
   ADC   #$02
   LDY   #$70
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$71
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$72
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$73
   STA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@143
   DEY
   LDA   (FP),Y
   CMP   #$09
   BNE   .cmp_true@143
   LDA   #$00
   JMP   .cmp_end@144
.cmp_true@143:
   LDA   #$01
.cmp_end@144:
   LDY   #$74
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@57
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   JMP   .or_end@58
.or_true@57:
   LDA   #$01
   LDY   #$76
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@58:
   LDY   #$76
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@17
   JMP   .or_true@55
.lb_skip@17:
   CLC
   LDA   FP
   ADC   #$01
   LDY   #$7A
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
.mul_loop@146:
   BCC   .mul_skip@147
   CLC
   ADC   #$05
.mul_skip@147:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@146
   LDY   #$79
   STA   (FP),Y
   LDA   HARGS
   DEY
   STA   (FP),Y
   LDY   #$7A
   LDA   (FP),Y
   CLC
   LDY   #$78
   ADC   (FP),Y
   LDY   #$7C
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$79
   ADC   (FP),Y
   LDY   #$7D
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$04
   LDY   #$7E
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$7F
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$80
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CMP   #$09
   BNE   .cmp_true@148
   LDA   #$00
   JMP   .cmp_end@149
.cmp_true@148:
   LDA   #$01
.cmp_end@149:
   LDY   #$83
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@55
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   JMP   .or_end@56
.or_true@55:
   LDA   #$01
   LDY   #$85
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@56:
   LDY   #$85
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@16
   JMP   .or_true@53
.lb_skip@16:
   CLC
   LDA   FP
   ADC   #$01
   LDY   #$89
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
.mul_loop@150:
   BCC   .mul_skip@151
   CLC
   ADC   #$05
.mul_skip@151:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@150
   LDY   #$88
   STA   (FP),Y
   LDA   HARGS
   DEY
   STA   (FP),Y
   LDY   #$89
   LDA   (FP),Y
   CLC
   LDY   #$87
   ADC   (FP),Y
   LDY   #$8B
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$88
   ADC   (FP),Y
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
   CMP   #$00
   BNE   .cmp_true@152
   DEY
   LDA   (FP),Y
   CMP   #$06
   BNE   .cmp_true@152
   LDA   #$00
   JMP   .cmp_end@153
.cmp_true@152:
   LDA   #$01
.cmp_end@153:
   LDY   #$8F
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@53
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   JMP   .or_end@54
.or_true@53:
   LDA   #$01
   LDY   #$91
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@54:
   LDY   #$91
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@15
   JMP   .or_true@51
.lb_skip@15:
   CLC
   LDA   FP
   ADC   #$01
   LDY   #$95
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
.mul_loop@155:
   BCC   .mul_skip@156
   CLC
   ADC   #$05
.mul_skip@156:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@155
   LDY   #$94
   STA   (FP),Y
   LDA   HARGS
   DEY
   STA   (FP),Y
   LDY   #$95
   LDA   (FP),Y
   CLC
   LDY   #$93
   ADC   (FP),Y
   LDY   #$97
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$94
   ADC   (FP),Y
   LDY   #$98
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$02
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
   CMP   #$00
   BNE   .cmp_true@157
   DEY
   LDA   (FP),Y
   CMP   #$07
   BNE   .cmp_true@157
   LDA   #$00
   JMP   .cmp_end@158
.cmp_true@157:
   LDA   #$01
.cmp_end@158:
   LDY   #$9D
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@51
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   JMP   .or_end@52
.or_true@51:
   LDA   #$01
   LDY   #$9F
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@52:
   LDY   #$9F
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@14
   JMP   .or_true@49
.lb_skip@14:
   CLC
   LDA   FP
   ADC   #$01
   LDY   #$A3
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
.mul_loop@160:
   BCC   .mul_skip@161
   CLC
   ADC   #$05
.mul_skip@161:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@160
   LDY   #$A2
   STA   (FP),Y
   LDA   HARGS
   DEY
   STA   (FP),Y
   LDY   #$A3
   LDA   (FP),Y
   CLC
   LDY   #$A1
   ADC   (FP),Y
   LDY   #$A5
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$A2
   ADC   (FP),Y
   LDY   #$A6
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$04
   LDY   #$A7
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
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
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CMP   #$08
   BNE   .cmp_true@162
   LDA   #$00
   JMP   .cmp_end@163
.cmp_true@162:
   LDA   #$01
.cmp_end@163:
   LDY   #$AC
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@49
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   JMP   .or_end@50
.or_true@49:
   LDA   #$01
   LDY   #$AE
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@50:
   LDY   #$AE
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@65
   LDX   #$00
   TXA

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$B1
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
.if_end@65:
   LDX   #$00
   LDA   #$01

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$B1
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$B0
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

   ; prologue: 0 arg bytes, 28 local bytes
   SEC
   LDA   SSP
   SBC   #$1E
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$1D
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   JSR   test_copy_to_pointer
   LDY   #$01
   STA   (FP),Y
   TXA
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lnot_true@164
   LDA   #$00
   JMP   .lnot_end@165
.lnot_true@164:
   LDA   #$01
.lnot_end@165:
   LDY   #$03
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@66
   LDX   #$00
   LDA   #$01

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$1E
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$1D
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
.if_end@66:
   JSR   test_copy_from_pointer
   LDY   #$05
   STA   (FP),Y
   TXA
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lnot_true@166
   LDA   #$00
   JMP   .lnot_end@167
.lnot_true@166:
   LDA   #$01
.lnot_end@167:
   LDY   #$07
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@67
   LDX   #$00
   LDA   #$02

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$1E
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$1D
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
.if_end@67:
   JSR   test_copy_to_and_from_pointer
   LDY   #$09
   STA   (FP),Y
   TXA
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lnot_true@168
   LDA   #$00
   JMP   .lnot_end@169
.lnot_true@168:
   LDA   #$01
.lnot_end@169:
   LDY   #$0B
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@68
   LDX   #$00
   LDA   #$03

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$1E
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$1D
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
.if_end@68:
   JSR   test_copy_to_array_elem
   LDY   #$0D
   STA   (FP),Y
   TXA
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lnot_true@170
   LDA   #$00
   JMP   .lnot_end@171
.lnot_true@170:
   LDA   #$01
.lnot_end@171:
   LDY   #$0F
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@69
   LDX   #$00
   LDA   #$04

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$1E
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$1D
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
.if_end@69:
   JSR   test_copy_from_array_elem
   LDY   #$11
   STA   (FP),Y
   TXA
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lnot_true@172
   LDA   #$00
   JMP   .lnot_end@173
.lnot_true@172:
   LDA   #$01
.lnot_end@173:
   LDY   #$13
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@70
   LDX   #$00
   LDA   #$05

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$1E
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$1D
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
.if_end@70:
   JSR   test_copy_to_and_from_array_elem
   LDY   #$15
   STA   (FP),Y
   TXA
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lnot_true@174
   LDA   #$00
   JMP   .lnot_end@175
.lnot_true@174:
   LDA   #$01
.lnot_end@175:
   LDY   #$17
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@71
   LDX   #$00
   LDA   #$06

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$1E
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$1D
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
.if_end@71:
   JSR   test_copy_array_element_with_padding
   LDY   #$19
   STA   (FP),Y
   TXA
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lnot_true@176
   LDA   #$00
   JMP   .lnot_end@177
.lnot_true@176:
   LDA   #$01
.lnot_end@177:
   LDY   #$1B
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@72
   LDX   #$00
   LDA   #$07

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$1E
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
.if_end@72:
   LDX   #$00
   TXA

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$1E
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$1D
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
   DC.B  $21, $3F, $00

str@1:
   DC.B  $28, $29, $00

str@2:
   DC.B  $2B, $2D, $00

str@3:
   DC.B  $0A, $09, $00

str@4:
   DC.B  $63, $64, $00

str@5:
   DC.B  $65, $66, $00

str@6:
   DC.B  $63, $64, $00

str@7:
   DC.B  $65, $66, $00

@5.my_struct:
   DC.B  $28, $29, $00
   DC.W  $004D
   DC.L  $0000004E

@12.arr:
   DS.B  27
