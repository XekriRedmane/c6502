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

   SEC
   LDA   #$00
   SBC   #$14
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
   ORA   #$00
   BMI   .sx_neg@0
   LDA   #$00
   JMP   .sx_done@1
.sx_neg@0:
   LDA   #$FF
.sx_done@1:
   LDY   #$05
   STA   (FP),Y
   INY
   STA   (FP),Y
   CLC
   LDA   FP
   ADC   #$07
   LDY   #$0B
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
   LDY   #$03
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$04
   LDA   (FP),Y
   LDY   #$01
   STA   (DPTR),Y
   LDY   #$05
   LDA   (FP),Y
   LDY   #$02
   STA   (DPTR),Y
   LDY   #$06
   LDA   (FP),Y
   LDY   #$03
   STA   (DPTR),Y
   LDA   #$04
   LDY   #$0D
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
   LDY   #$0D
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$0E
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$0F
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$10
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   JSR   malloc
   LDY   #$11
   STA   (FP),Y
   TXA
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
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$07
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$08
   LDA   (FP),Y
   LDY   #$01
   STA   (DPTR),Y
   LDY   #$09
   LDA   (FP),Y
   LDY   #$02
   STA   (DPTR),Y
   LDY   #$0A
   LDA   (FP),Y
   LDY   #$03
   STA   (DPTR),Y
   LDY   #$13
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
   SEC
   LDA   #$00
   SBC   #$14
   INY
   STA   (FP),Y
   LDA   #$00
   SBC   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$1B
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$1C
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@2
   LDA   #$00
   JMP   .sx_done@3
.sx_neg@2:
   LDA   #$FF
.sx_done@3:
   LDY   #$1D
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$18
   LDA   (FP),Y
   LDY   #$1E
   CMP   (FP),Y
   BNE   .cmp_true@4
   LDY   #$17
   LDA   (FP),Y
   LDY   #$1D
   CMP   (FP),Y
   BNE   .cmp_true@4
   LDY   #$16
   LDA   (FP),Y
   LDY   #$1C
   CMP   (FP),Y
   BNE   .cmp_true@4
   LDY   #$15
   LDA   (FP),Y
   LDY   #$1B
   CMP   (FP),Y
   BNE   .cmp_true@4
   LDA   #$00
   JMP   .cmp_end@5
.cmp_true@4:
   LDA   #$01
.cmp_end@5:
   LDY   #$1F
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@6
   LDY   #$13
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
   SEC
   LDA   #$00
   SBC   #$14
   INY
   STA   (FP),Y
   LDA   #$00
   SBC   #$00
   INY
   STA   (FP),Y
   LDY   #$22
   LDA   (FP),Y
   LDY   #$24
   CMP   (FP),Y
   BNE   .cmp_true@7
   LDY   #$21
   LDA   (FP),Y
   LDY   #$23
   CMP   (FP),Y
   BNE   .cmp_true@7
   LDA   #$00
   JMP   .cmp_end@8
.cmp_true@7:
   LDA   #$01
.cmp_end@8:
   LDY   #$25
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
   LDY   #$27
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@7:
   LDY   #$27
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@4
   LDY   #$13
   LDA   (FP),Y
   CLC
   LDY   #$29
   STA   (FP),Y
   LDY   #$14
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
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$2B
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CMP   #$EC
   BNE   .cmp_true@10
   LDA   #$00
   JMP   .cmp_end@11
.cmp_true@10:
   LDA   #$01
.cmp_end@11:
   LDY   #$2E
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
   LDY   #$30
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@5:
   LDY   #$30
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@2
   LDY   #$13
   LDA   (FP),Y
   CLC
   ADC   #$01
   LDY   #$32
   STA   (FP),Y
   LDY   #$14
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
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CMP   #$FF
   BNE   .cmp_true@12
   LDA   #$00
   JMP   .cmp_end@13
.cmp_true@12:
   LDA   #$01
.cmp_end@13:
   LDY   #$37
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
   LDY   #$39
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@3:
   LDY   #$39
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@0
   LDY   #$13
   LDA   (FP),Y
   CLC
   ADC   #$02
   LDY   #$3B
   STA   (FP),Y
   LDY   #$14
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
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CMP   #$FF
   BNE   .cmp_true@14
   LDA   #$00
   JMP   .cmp_end@15
.cmp_true@14:
   LDA   #$01
.cmp_end@15:
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
   BEQ   .if_end@8
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
.if_end@8:
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

   ; prologue: 0 arg bytes, 94 local bytes
   SEC
   LDA   SSP
   SBC   #$60
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$5F
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
   LDY   #$0F
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
   LDA   #$94
   INY
   STA   (DPTR),Y
   LDA   #$35
   INY
   STA   (DPTR),Y
   LDA   #$77
   INY
   STA   (DPTR),Y
   LDY   #$0F
   LDA   (FP),Y
   CLC
   ADC   #$04
   LDY   #$11
   STA   (FP),Y
   DEY
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
   LDA   #$88
   INY
   STA   (DPTR),Y
   LDA   #$D3
   INY
   STA   (DPTR),Y
   LDA   #$40
   INY
   STA   (DPTR),Y
   LDA   #$02
   LDY   #$13
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$0F
   LDA   (FP),Y
   CLC
   ADC   #$0C
   LDY   #$15
   STA   (FP),Y
   LDY   #$10
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
   LDY   #$13
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$14
   LDA   (FP),Y
   LDY   #$01
   STA   (DPTR),Y
   LDA   #<@8.my_union
   LDY   #$17
   STA   (FP),Y
   LDA   #>@8.my_union
   INY
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
   LDY   #$0A
   LDA   (FP),Y
   DEY
   STA   (DPTR),Y
   LDY   #$0B
   LDA   (FP),Y
   DEY
   STA   (DPTR),Y
   LDY   #$0C
   LDA   (FP),Y
   DEY
   STA   (DPTR),Y
   LDY   #$0D
   LDA   (FP),Y
   DEY
   STA   (DPTR),Y
   LDY   #$0E
   LDA   (FP),Y
   DEY
   STA   (DPTR),Y
   LDA   #<@8.my_union
   LDY   #$19
   STA   (FP),Y
   LDA   #>@8.my_union
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$1B
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$1C
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$1D
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$1E
   STA   (FP),Y
   LDY   #$02
   LDA   (DPTR),Y
   LDY   #$1F
   STA   (FP),Y
   LDY   #$03
   LDA   (DPTR),Y
   LDY   #$20
   STA   (FP),Y
   LDY   #$04
   LDA   (DPTR),Y
   LDY   #$21
   STA   (FP),Y
   LDY   #$05
   LDA   (DPTR),Y
   LDY   #$22
   STA   (FP),Y
   LDY   #$06
   LDA   (DPTR),Y
   LDY   #$23
   STA   (FP),Y
   LDY   #$07
   LDA   (DPTR),Y
   LDY   #$24
   STA   (FP),Y
   LDY   #$08
   LDA   (DPTR),Y
   LDY   #$25
   STA   (FP),Y
   LDY   #$09
   LDA   (DPTR),Y
   LDY   #$26
   STA   (FP),Y
   LDY   #$0A
   LDA   (DPTR),Y
   LDY   #$27
   STA   (FP),Y
   LDY   #$0B
   LDA   (DPTR),Y
   LDY   #$28
   STA   (FP),Y
   LDY   #$0C
   LDA   (DPTR),Y
   LDY   #$29
   STA   (FP),Y
   LDY   #$0D
   LDA   (DPTR),Y
   LDY   #$2A
   STA   (FP),Y
   LDY   #$1D
   LDA   (FP),Y
   LDY   #$2B
   STA   (FP),Y
   LDY   #$1E
   LDA   (FP),Y
   LDY   #$2C
   STA   (FP),Y
   LDY   #$1F
   LDA   (FP),Y
   LDY   #$2D
   STA   (FP),Y
   LDY   #$20
   LDA   (FP),Y
   LDY   #$2E
   STA   (FP),Y
   LDY   #$21
   LDA   (FP),Y
   LDY   #$2F
   STA   (FP),Y
   LDY   #$22
   LDA   (FP),Y
   LDY   #$30
   STA   (FP),Y
   LDY   #$23
   LDA   (FP),Y
   LDY   #$31
   STA   (FP),Y
   LDY   #$24
   LDA   (FP),Y
   LDY   #$32
   STA   (FP),Y
   LDY   #$25
   LDA   (FP),Y
   LDY   #$33
   STA   (FP),Y
   LDY   #$26
   LDA   (FP),Y
   LDY   #$34
   STA   (FP),Y
   LDY   #$27
   LDA   (FP),Y
   LDY   #$35
   STA   (FP),Y
   LDY   #$28
   LDA   (FP),Y
   LDY   #$36
   STA   (FP),Y
   LDY   #$29
   LDA   (FP),Y
   LDY   #$37
   STA   (FP),Y
   LDY   #$2A
   LDA   (FP),Y
   LDY   #$38
   STA   (FP),Y
   CLC
   LDA   FP
   ADC   #$2B
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
   LDY   #$3B
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$3C
   STA   (FP),Y
   LDY   #$02
   LDA   (DPTR),Y
   LDY   #$3D
   STA   (FP),Y
   LDY   #$03
   LDA   (DPTR),Y
   LDY   #$3E
   STA   (FP),Y
   CMP   #$77
   BNE   .cmp_true@16
   DEY
   LDA   (FP),Y
   CMP   #$35
   BNE   .cmp_true@16
   DEY
   LDA   (FP),Y
   CMP   #$94
   BNE   .cmp_true@16
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@16
   LDA   #$00
   JMP   .cmp_end@17
.cmp_true@16:
   LDA   #$01
.cmp_end@17:
   LDY   #$3F
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@1
   JMP   .or_true@11
.lb_skip@1:
   CLC
   LDA   FP
   ADC   #$2B
   LDY   #$41
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$04
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
   LDY   #$04
   LDA   (DPTR),Y
   LDY   #$49
   STA   (FP),Y
   LDY   #$05
   LDA   (DPTR),Y
   LDY   #$4A
   STA   (FP),Y
   LDY   #$06
   LDA   (DPTR),Y
   LDY   #$4B
   STA   (FP),Y
   LDY   #$07
   LDA   (DPTR),Y
   LDY   #$4C
   STA   (FP),Y
   CMP   #$40
   BNE   .cmp_true@19
   DEY
   LDA   (FP),Y
   CMP   #$D3
   BNE   .cmp_true@19
   DEY
   LDA   (FP),Y
   CMP   #$88
   BNE   .cmp_true@19
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@19
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@19
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@19
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@19
   DEY
   LDA   (FP),Y
   CMP   #$00
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
   BNE   .or_true@11
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   JMP   .or_end@12
.or_true@11:
   LDA   #$01
   LDY   #$4F
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@12:
   LDY   #$4F
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@0
   JMP   .or_true@9
.lb_skip@0:
   CLC
   LDA   FP
   ADC   #$2B
   LDY   #$51
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$0C
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
   LDY   #$57
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$58
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   CMP   #$80
   BNE   .cmp_true@22
   CMP   #$00
   BNE   .cmp_true@22
   LDY   #$58
   LDA   (FP),Y
   BNE   .cmp_true@22
   DEY
   LDA   (FP),Y
   CMP   #$02
   BNE   .cmp_true@22
   LDA   #$00
   JMP   .cmp_end@23
.cmp_true@22:
   LDA   #$01
.cmp_end@23:
   LDY   #$5B
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@9
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   JMP   .or_end@10
.or_true@9:
   LDA   #$01
   LDY   #$5D
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@10:
   LDY   #$5D
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@13
   LDX   #$00
   TXA

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$60
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
.if_end@13:
   LDX   #$00
   LDA   #$01

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$60
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$5F
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

test_copy_array_members:
   SUBROUTINE

   ; prologue: 0 arg bytes, 184 local bytes
   SEC
   LDA   SSP
   SBC   #$BA
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$B9
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
   LDY   #$1F
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
   LDA   #$66
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$1F
   LDA   (FP),Y
   CLC
   ADC   #$01
   LDY   #$21
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$22
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDA   #$6F
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$1F
   LDA   (FP),Y
   CLC
   ADC   #$02
   LDY   #$23
   STA   (FP),Y
   LDY   #$20
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
   LDA   #$6F
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$1F
   LDA   (FP),Y
   CLC
   ADC   #$03
   LDY   #$25
   STA   (FP),Y
   LDY   #$20
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
   LDA   #$62
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$1F
   LDA   (FP),Y
   CLC
   ADC   #$04
   LDY   #$27
   STA   (FP),Y
   LDY   #$20
   LDA   (FP),Y
   ADC   #$00
   LDY   #$28
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
   LDY   #$1F
   LDA   (FP),Y
   CLC
   ADC   #$05
   LDY   #$29
   STA   (FP),Y
   LDY   #$20
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
   LDA   #$72
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$1F
   LDA   (FP),Y
   CLC
   ADC   #$06
   LDY   #$2B
   STA   (FP),Y
   LDY   #$20
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
   LDY   #$1F
   LDA   (FP),Y
   CLC
   ADC   #$0A
   LDY   #$2D
   STA   (FP),Y
   LDY   #$20
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
   LDA   #$68
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$1F
   LDA   (FP),Y
   CLC
   ADC   #$0B
   LDY   #$2F
   STA   (FP),Y
   LDY   #$20
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
   LDA   #$65
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$1F
   LDA   (FP),Y
   CLC
   ADC   #$0C
   LDY   #$31
   STA   (FP),Y
   LDY   #$20
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
   LDA   #$6C
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$1F
   LDA   (FP),Y
   CLC
   ADC   #$0D
   LDY   #$33
   STA   (FP),Y
   LDY   #$20
   LDA   (FP),Y
   ADC   #$00
   LDY   #$34
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
   LDY   #$1F
   LDA   (FP),Y
   CLC
   ADC   #$0E
   LDY   #$35
   STA   (FP),Y
   LDY   #$20
   LDA   (FP),Y
   ADC   #$00
   LDY   #$36
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDA   #$6F
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$1F
   LDA   (FP),Y
   CLC
   ADC   #$0F
   LDY   #$37
   STA   (FP),Y
   LDY   #$20
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
   LDA   #$00
   LDY   #$00
   STA   (DPTR),Y
   INY
   STA   (DPTR),Y
   INY
   STA   (DPTR),Y
   INY
   STA   (DPTR),Y
   LDY   #$1F
   LDA   (FP),Y
   CLC
   ADC   #$13
   LDY   #$39
   STA   (FP),Y
   LDY   #$20
   LDA   (FP),Y
   ADC   #$00
   LDY   #$3A
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
   LDY   #$1F
   LDA   (FP),Y
   CLC
   ADC   #$14
   LDY   #$3B
   STA   (FP),Y
   LDY   #$20
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
   LDA   #$69
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$1F
   LDA   (FP),Y
   CLC
   ADC   #$15
   LDY   #$3D
   STA   (FP),Y
   LDY   #$20
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
   LDA   #$74
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$1F
   LDA   (FP),Y
   CLC
   ADC   #$16
   LDY   #$3F
   STA   (FP),Y
   LDY   #$20
   LDA   (FP),Y
   ADC   #$00
   LDY   #$40
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDA   #$73
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$1F
   LDA   (FP),Y
   CLC
   ADC   #$17
   LDY   #$41
   STA   (FP),Y
   LDY   #$20
   LDA   (FP),Y
   ADC   #$00
   LDY   #$42
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
   LDY   #$1F
   LDA   (FP),Y
   CLC
   ADC   #$18
   LDY   #$43
   STA   (FP),Y
   LDY   #$20
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
   LDA   #$75
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$1F
   LDA   (FP),Y
   CLC
   ADC   #$19
   LDY   #$45
   STA   (FP),Y
   LDY   #$20
   LDA   (FP),Y
   ADC   #$00
   LDY   #$46
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
   LDY   #$1F
   LDA   (FP),Y
   CLC
   ADC   #$1A
   LDY   #$47
   STA   (FP),Y
   LDY   #$20
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
   LDA   #$69
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$1F
   LDA   (FP),Y
   CLC
   ADC   #$1B
   LDY   #$49
   STA   (FP),Y
   LDY   #$20
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
   LDA   #$6F
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$1F
   LDA   (FP),Y
   CLC
   ADC   #$1C
   LDY   #$4B
   STA   (FP),Y
   LDY   #$20
   LDA   (FP),Y
   ADC   #$00
   LDY   #$4C
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
   LDY   #$1F
   LDA   (FP),Y
   CLC
   ADC   #$1D
   LDY   #$4D
   STA   (FP),Y
   LDY   #$20
   LDA   (FP),Y
   ADC   #$00
   LDY   #$4E
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
.mul_loop@25:
   BCC   .mul_skip@26
   CLC
   ADC   #$0A
.mul_skip@26:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@25
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
   LDY   #$02
   LDA   (DPTR),Y
   LDY   #$57
   STA   (FP),Y
   LDY   #$03
   LDA   (DPTR),Y
   LDY   #$58
   STA   (FP),Y
   LDY   #$04
   LDA   (DPTR),Y
   LDY   #$59
   STA   (FP),Y
   LDY   #$05
   LDA   (DPTR),Y
   LDY   #$5A
   STA   (FP),Y
   LDY   #$06
   LDA   (DPTR),Y
   LDY   #$5B
   STA   (FP),Y
   LDY   #$07
   LDA   (DPTR),Y
   LDY   #$5C
   STA   (FP),Y
   LDY   #$08
   LDA   (DPTR),Y
   LDY   #$5D
   STA   (FP),Y
   LDY   #$09
   LDA   (DPTR),Y
   LDY   #$5E
   STA   (FP),Y
   LDY   #$55
   LDA   (FP),Y
   LDY   #$5F
   STA   (FP),Y
   LDY   #$56
   LDA   (FP),Y
   LDY   #$60
   STA   (FP),Y
   LDY   #$57
   LDA   (FP),Y
   LDY   #$61
   STA   (FP),Y
   LDY   #$58
   LDA   (FP),Y
   LDY   #$62
   STA   (FP),Y
   LDY   #$59
   LDA   (FP),Y
   LDY   #$63
   STA   (FP),Y
   LDY   #$5A
   LDA   (FP),Y
   LDY   #$64
   STA   (FP),Y
   LDY   #$5B
   LDA   (FP),Y
   LDY   #$65
   STA   (FP),Y
   LDY   #$5C
   LDA   (FP),Y
   LDY   #$66
   STA   (FP),Y
   LDY   #$5D
   LDA   (FP),Y
   LDY   #$67
   STA   (FP),Y
   LDY   #$5E
   LDA   (FP),Y
   LDY   #$68
   STA   (FP),Y
   CLC
   LDA   FP
   ADC   #$69
   LDY   #$73
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
   LDA   #$62
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$73
   LDA   (FP),Y
   CLC
   ADC   #$01
   LDY   #$75
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$76
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
   LDY   #$73
   LDA   (FP),Y
   CLC
   ADC   #$02
   LDY   #$77
   STA   (FP),Y
   LDY   #$74
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
   LDA   #$61
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$73
   LDA   (FP),Y
   CLC
   ADC   #$03
   LDY   #$79
   STA   (FP),Y
   LDY   #$74
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
   LDA   #$68
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$73
   LDA   (FP),Y
   CLC
   ADC   #$04
   LDY   #$7B
   STA   (FP),Y
   LDY   #$74
   LDA   (FP),Y
   ADC   #$00
   LDY   #$7C
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
   LDY   #$73
   LDA   (FP),Y
   CLC
   ADC   #$05
   LDY   #$7D
   STA   (FP),Y
   LDY   #$74
   LDA   (FP),Y
   ADC   #$00
   LDY   #$7E
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
   LDY   #$73
   LDA   (FP),Y
   CLC
   ADC   #$06
   LDY   #$7F
   STA   (FP),Y
   LDY   #$74
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
   LDA   #$61
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$73
   LDA   (FP),Y
   CLC
   ADC   #$07
   LDY   #$81
   STA   (FP),Y
   LDY   #$74
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
   LDA   #$68
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$73
   LDA   (FP),Y
   CLC
   ADC   #$08
   LDY   #$83
   STA   (FP),Y
   LDY   #$74
   LDA   (FP),Y
   ADC   #$00
   LDY   #$84
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
   CLC
   LDA   FP
   ADC   #$01
   LDY   #$87
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
.mul_loop@27:
   BCC   .mul_skip@28
   CLC
   ADC   #$0A
.mul_skip@28:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@27
   LDY   #$86
   STA   (FP),Y
   LDA   HARGS
   DEY
   STA   (FP),Y
   LDY   #$87
   LDA   (FP),Y
   CLC
   LDY   #$85
   ADC   (FP),Y
   LDY   #$89
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$86
   ADC   (FP),Y
   LDY   #$8A
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$69
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$6A
   LDA   (FP),Y
   LDY   #$01
   STA   (DPTR),Y
   LDY   #$6B
   LDA   (FP),Y
   LDY   #$02
   STA   (DPTR),Y
   LDY   #$6C
   LDA   (FP),Y
   LDY   #$03
   STA   (DPTR),Y
   LDY   #$6D
   LDA   (FP),Y
   LDY   #$04
   STA   (DPTR),Y
   LDY   #$6E
   LDA   (FP),Y
   LDY   #$05
   STA   (DPTR),Y
   LDY   #$6F
   LDA   (FP),Y
   LDY   #$06
   STA   (DPTR),Y
   LDY   #$70
   LDA   (FP),Y
   LDY   #$07
   STA   (DPTR),Y
   LDY   #$71
   LDA   (FP),Y
   LDY   #$08
   STA   (DPTR),Y
   LDY   #$72
   LDA   (FP),Y
   LDY   #$09
   STA   (DPTR),Y
   CLC
   LDA   FP
   ADC   #$01
   LDY   #$8D
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   LDA   #$00
   STA   HARGS
   LDX   #$08
   LSR   HARGS
.mul_loop@29:
   BCC   .mul_skip@30
   CLC
   ADC   #$0A
.mul_skip@30:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@29
   LDY   #$8C
   STA   (FP),Y
   LDA   HARGS
   DEY
   STA   (FP),Y
   LDY   #$8D
   LDA   (FP),Y
   CLC
   LDY   #$8B
   ADC   (FP),Y
   LDY   #$8F
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$8C
   ADC   (FP),Y
   LDY   #$90
   STA   (FP),Y
   LDA   #<str@0
   INY
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
   LDY   #$8F
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$90
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$91
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$92
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   JSR   strcmp
   LDY   #$93
   STA   (FP),Y
   TXA
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@3
   JMP   .or_true@16
.lb_skip@3:
   CLC
   LDA   FP
   ADC   #$01
   LDY   #$97
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
.mul_loop@31:
   BCC   .mul_skip@32
   CLC
   ADC   #$0A
.mul_skip@32:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@31
   LDY   #$96
   STA   (FP),Y
   LDA   HARGS
   DEY
   STA   (FP),Y
   LDY   #$97
   LDA   (FP),Y
   CLC
   LDY   #$95
   ADC   (FP),Y
   LDY   #$99
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$96
   ADC   (FP),Y
   LDY   #$9A
   STA   (FP),Y
   LDA   #<str@1
   INY
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
   LDY   #$99
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$9A
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$9B
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$9C
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   JSR   strcmp
   LDY   #$9D
   STA   (FP),Y
   TXA
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
   LDY   #$9F
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@17:
   LDY   #$9F
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@2
   JMP   .or_true@14
.lb_skip@2:
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
.mul_loop@33:
   BCC   .mul_skip@34
   CLC
   ADC   #$0A
.mul_skip@34:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@33
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
   LDA   #<str@2
   INY
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
   LDY   #$A5
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$A6
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$A7
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$A8
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   JSR   strcmp
   LDY   #$A9
   STA   (FP),Y
   TXA
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@14
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   JMP   .or_end@15
.or_true@14:
   LDA   #$01
   LDY   #$AB
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@15:
   LDY   #$AB
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@18
   LDX   #$00
   TXA

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$BA
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$B9
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
.if_end@18:
   CLC
   LDA   FP
   ADC   #$5F
   LDY   #$AF
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   LDA   #<str@3
   LDY   #$AD
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
   INY
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$B0
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$AD
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$AE
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   JSR   strcmp
   LDY   #$B1
   STA   (FP),Y
   TXA
   INY
   STA   (FP),Y
   DEY
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
   ADC   #$BA
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$B9
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
   CLC
   LDA   FP
   ADC   #$69
   LDY   #$B5
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   LDA   #<str@4
   LDY   #$B3
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
   LDY   #$B6
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$B3
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$B4
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   JSR   strcmp
   LDY   #$B7
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
   TXA

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$BA
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
.if_end@20:
   LDX   #$00
   LDA   #$01

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$BA
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$B9
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

   ; prologue: 0 arg bytes, 12 local bytes
   SEC
   LDA   SSP
   SBC   #$0E
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$0D
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
   BEQ   .lnot_true@35
   LDA   #$00
   JMP   .lnot_end@36
.lnot_true@35:
   LDA   #$01
.lnot_end@36:
   LDY   #$03
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@21
   LDX   #$00
   LDA   #$01

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$0E
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$0D
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
   BEQ   .lnot_true@37
   LDA   #$00
   JMP   .lnot_end@38
.lnot_true@37:
   LDA   #$01
.lnot_end@38:
   LDY   #$07
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@22
   LDX   #$00
   LDA   #$02

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$0E
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$0D
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
.if_end@22:
   JSR   test_copy_array_members
   LDY   #$09
   STA   (FP),Y
   TXA
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lnot_true@39
   LDA   #$00
   JMP   .lnot_end@40
.lnot_true@39:
   LDA   #$01
.lnot_end@40:
   LDY   #$0B
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@23
   LDX   #$00
   LDA   #$03

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$0E
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
   TXA

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$0E
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$0D
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
   DC.B  $66, $6F, $6F, $62, $61, $72, $00

str@1:
   DC.B  $68, $65, $6C, $6C, $6F, $00

str@2:
   DC.B  $62, $6C, $61, $68, $62, $6C, $61, $68, $00

str@3:
   DC.B  $66, $6F, $6F, $62, $61, $72, $00

str@4:
   DC.B  $62, $6C, $61, $68, $62, $6C, $61, $68, $00

@8.my_union:
   DS.B  14
