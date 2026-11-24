SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24

test_copy_from_member_pointer:
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

   LDA   #$0A
   LDY   #$01
   STA   (FP),Y
.clear@0:
   SEC
   SBC   #$01
   LDY   #$01
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   CLC
   LDA   FP
   ADC   #$04
   LDY   #$0E
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   LDY   #$02
   ADC   (FP),Y
   LDY   #$10
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$03
   ADC   (FP),Y
   LDY   #$11
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
   LDA   (FP),Y
   BNE   .clear@0
   LDA   #$0C
   LDY   #$12
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
   LDY   #$12
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$13
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$14
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$15
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   JSR   malloc
   LDY   #$16
   STA   (FP),Y
   TXA
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$18
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$19
   STA   (FP),Y
   LDA   #$64
   INY
   STA   (FP),Y
   LDY   #$18
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   INY
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$18
   LDA   (FP),Y
   CLC
   ADC   #$02
   LDY   #$1B
   STA   (FP),Y
   LDY   #$19
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
   LDA   #$80
   INY
   STA   (DPTR),Y
   LDA   #$35
   INY
   STA   (DPTR),Y
   LDA   #$40
   INY
   STA   (DPTR),Y
   LDA   #$A1
   LDY   #$1D
   STA   (FP),Y
   LDA   #$86
   INY
   STA   (FP),Y
   LDY   #$18
   LDA   (FP),Y
   CLC
   ADC   #$02
   LDY   #$1F
   STA   (FP),Y
   LDY   #$19
   LDA   (FP),Y
   ADC   #$00
   LDY   #$20
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$08
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
   LDY   #$1D
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$1E
   LDA   (FP),Y
   LDY   #$01
   STA   (DPTR),Y
   LDY   #$18
   LDA   (FP),Y
   CLC
   ADC   #$02
   LDY   #$23
   STA   (FP),Y
   LDY   #$19
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
   LDY   #$08
   LDA   (DPTR),Y
   LDY   #$2D
   STA   (FP),Y
   LDY   #$09
   LDA   (DPTR),Y
   LDY   #$2E
   STA   (FP),Y
   LDY   #$25
   LDA   (FP),Y
   LDY   #$04
   STA   (FP),Y
   LDY   #$26
   LDA   (FP),Y
   LDY   #$05
   STA   (FP),Y
   LDY   #$27
   LDA   (FP),Y
   LDY   #$06
   STA   (FP),Y
   LDY   #$28
   LDA   (FP),Y
   LDY   #$07
   STA   (FP),Y
   LDY   #$29
   LDA   (FP),Y
   LDY   #$08
   STA   (FP),Y
   LDY   #$2A
   LDA   (FP),Y
   LDY   #$09
   STA   (FP),Y
   LDY   #$2B
   LDA   (FP),Y
   LDY   #$0A
   STA   (FP),Y
   LDY   #$2C
   LDA   (FP),Y
   LDY   #$0B
   STA   (FP),Y
   LDY   #$2D
   LDA   (FP),Y
   LDY   #$0C
   STA   (FP),Y
   LDY   #$2E
   LDA   (FP),Y
   LDY   #$0D
   STA   (FP),Y
   CLC
   LDA   FP
   ADC   #$04
   LDY   #$2F
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
   BNE   .cmp_true@0
   DEY
   LDA   (FP),Y
   CMP   #$35
   BNE   .cmp_true@0
   DEY
   LDA   (FP),Y
   CMP   #$80
   BNE   .cmp_true@0
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@0
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@0
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@0
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@0
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@0
   LDA   #$00
   JMP   .cmp_end@1
.cmp_true@0:
   LDA   #$01
.cmp_end@1:
   LDY   #$39
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@0
   JMP   .or_true@1
.lb_skip@0:
   CLC
   LDA   FP
   ADC   #$04
   LDY   #$3B
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$08
   LDY   #$3D
   STA   (FP),Y
   DEY
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
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$3F
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$40
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$41
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$42
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@3
   LDA   #$00
   JMP   .sx_done@4
.sx_neg@3:
   LDA   #$FF
.sx_done@4:
   LDY   #$43
   STA   (FP),Y
   INY
   STA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@5
   CMP   #$01
   BNE   .cmp_true@5
   LDY   #$42
   LDA   (FP),Y
   CMP   #$86
   BNE   .cmp_true@5
   DEY
   LDA   (FP),Y
   CMP   #$A1
   BNE   .cmp_true@5
   LDA   #$00
   JMP   .cmp_end@6
.cmp_true@5:
   LDA   #$01
.cmp_end@6:
   LDY   #$45
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@1
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   JMP   .or_end@2
.or_true@1:
   LDA   #$01
   LDY   #$47
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@2:
   LDY   #$47
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@3
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
.if_end@3:
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

test_copy_to_member_pointer:
   SUBROUTINE

   ; prologue: 0 arg bytes, 62 local bytes
   SEC
   LDA   SSP
   SBC   #$40
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$3F
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
   LDA   #$D0
   INY
   STA   (DPTR),Y
   LDA   #$58
   INY
   STA   (DPTR),Y
   LDA   #$40
   INY
   STA   (DPTR),Y
   LDA   #$06
   LDY   #$0D
   STA   (FP),Y
   LDA   #$12
   INY
   STA   (FP),Y
   LDY   #$0B
   LDA   (FP),Y
   CLC
   ADC   #$08
   LDY   #$0F
   STA   (FP),Y
   LDY   #$0C
   LDA   (FP),Y
   ADC   #$00
   LDY   #$10
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
   LDA   #$01
   LDY   #$11
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   ORA   #$00
   BPL   .sx_done@9
   LDA   #$FF
.sx_done@9:
   LDY   #$13
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$0C
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
   SBC   #$08
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
   JSR   calloc
   LDY   #$19
   STA   (FP),Y
   TXA
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
   CLC
   ADC   #$02
   LDY   #$1D
   STA   (FP),Y
   DEY
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
   LDY   #$1B
   LDA   (FP),Y
   CLC
   ADC   #$02
   LDY   #$1F
   STA   (FP),Y
   LDY   #$1C
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
   LDY   #$02
   LDA   (DPTR),Y
   LDY   #$23
   STA   (FP),Y
   LDY   #$03
   LDA   (DPTR),Y
   LDY   #$24
   STA   (FP),Y
   LDY   #$04
   LDA   (DPTR),Y
   LDY   #$25
   STA   (FP),Y
   LDY   #$05
   LDA   (DPTR),Y
   LDY   #$26
   STA   (FP),Y
   LDY   #$06
   LDA   (DPTR),Y
   LDY   #$27
   STA   (FP),Y
   LDY   #$07
   LDA   (DPTR),Y
   LDY   #$28
   STA   (FP),Y
   CMP   #$40
   BNE   .cmp_true@10
   DEY
   LDA   (FP),Y
   CMP   #$58
   BNE   .cmp_true@10
   DEY
   LDA   (FP),Y
   CMP   #$D0
   BNE   .cmp_true@10
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@10
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@10
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@10
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@10
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@10
   LDA   #$00
   JMP   .cmp_end@11
.cmp_true@10:
   LDA   #$01
.cmp_end@11:
   LDY   #$29
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@1
   JMP   .or_true@4
.lb_skip@1:
   LDY   #$1B
   LDA   (FP),Y
   CLC
   ADC   #$02
   LDY   #$2B
   STA   (FP),Y
   LDY   #$1C
   LDA   (FP),Y
   ADC   #$00
   LDY   #$2C
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
   DEY
   LDA   (FP),Y
   LDY   #$31
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$32
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@13
   LDA   #$00
   JMP   .sx_done@14
.sx_neg@13:
   LDA   #$FF
.sx_done@14:
   LDY   #$33
   STA   (FP),Y
   INY
   STA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@15
   CMP   #$0F
   BNE   .cmp_true@15
   LDY   #$32
   LDA   (FP),Y
   CMP   #$12
   BNE   .cmp_true@15
   DEY
   LDA   (FP),Y
   CMP   #$06
   BNE   .cmp_true@15
   LDA   #$00
   JMP   .cmp_end@16
.cmp_true@15:
   LDA   #$01
.cmp_end@16:
   LDY   #$35
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
   LDY   #$37
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@5:
   LDY   #$37
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@6
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
   LDY   #$3F
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
   LDY   #$1B
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$39
   STA   (FP),Y
   ORA   #$00
   BNE   .or_true@7
   LDY   #$1B
   LDA   (FP),Y
   CLC
   ADC   #$01
   LDY   #$3A
   STA   (FP),Y
   LDY   #$1C
   LDA   (FP),Y
   ADC   #$00
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
   ORA   #$00
   BNE   .or_true@7
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   JMP   .or_end@8
.or_true@7:
   LDA   #$01
   LDY   #$3D
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@8:
   LDY   #$3D
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@9
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
.if_end@9:
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
   LDY   #$3F
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

test_copy_from_nested_member_pointer:
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
   LDA   #$D0
   INY
   STA   (DPTR),Y
   LDA   #$58
   INY
   STA   (DPTR),Y
   LDA   #$40
   INY
   STA   (DPTR),Y
   LDA   #$06
   LDY   #$0D
   STA   (FP),Y
   LDA   #$12
   INY
   STA   (FP),Y
   LDY   #$0B
   LDA   (FP),Y
   CLC
   ADC   #$08
   LDY   #$0F
   STA   (FP),Y
   LDY   #$0C
   LDA   (FP),Y
   ADC   #$00
   LDY   #$10
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
   LDA   #$01
   LDY   #$11
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   ORA   #$00
   BPL   .sx_done@19
   LDA   #$FF
.sx_done@19:
   LDY   #$13
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$10
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
   SBC   #$08
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
   JSR   calloc
   LDY   #$19
   STA   (FP),Y
   TXA
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
   LDA   #$01
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   ORA   #$00
   BPL   .sx_done@21
   LDA   #$FF
.sx_done@21:
   LDY   #$1F
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$0C
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
   SBC   #$08
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDY   #$1D
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$1E
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$1F
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$20
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   LDY   #$21
   LDA   (FP),Y
   LDY   #$05
   STA   (SSP),Y
   LDY   #$22
   LDA   (FP),Y
   LDY   #$06
   STA   (SSP),Y
   LDY   #$23
   LDA   (FP),Y
   LDY   #$07
   STA   (SSP),Y
   LDY   #$24
   LDA   (FP),Y
   LDY   #$08
   STA   (SSP),Y
   JSR   calloc
   LDY   #$25
   STA   (FP),Y
   TXA
   INY
   STA   (FP),Y
   LDY   #$1B
   LDA   (FP),Y
   CLC
   ADC   #$02
   LDY   #$27
   STA   (FP),Y
   LDY   #$1C
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
   LDY   #$25
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$26
   LDA   (FP),Y
   LDY   #$01
   STA   (DPTR),Y
   SEC
   LDA   #$00
   SBC   #$05
   LDY   #$29
   STA   (FP),Y
   LDA   #$00
   SBC   #$00
   INY
   STA   (FP),Y
   LDY   #$1B
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
   LDA   #$65
   LDY   #$2B
   STA   (FP),Y
   LDY   #$1B
   LDA   (FP),Y
   CLC
   ADC   #$02
   LDY   #$2C
   STA   (FP),Y
   LDY   #$1C
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
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$2E
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$2F
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
   LDA   #$66
   LDY   #$30
   STA   (FP),Y
   LDY   #$1B
   LDA   (FP),Y
   CLC
   ADC   #$02
   LDY   #$31
   STA   (FP),Y
   LDY   #$1C
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
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$01
   LDY   #$35
   STA   (FP),Y
   DEY
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
   LDY   #$30
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$1B
   LDA   (FP),Y
   CLC
   ADC   #$02
   LDY   #$37
   STA   (FP),Y
   LDY   #$1C
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
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$39
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
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
   LDA   #$60
   INY
   STA   (DPTR),Y
   LDA   #$53
   INY
   STA   (DPTR),Y
   LDA   #$40
   INY
   STA   (DPTR),Y
   LDY   #$1B
   LDA   (FP),Y
   CLC
   ADC   #$02
   LDY   #$3D
   STA   (FP),Y
   LDY   #$1C
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
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$3F
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$40
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$02
   LDY   #$41
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$42
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$08
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
   LDA   #$58
   LDY   #$00
   STA   (DPTR),Y
   LDA   #$00
   INY
   STA   (DPTR),Y
   LDY   #$1B
   LDA   (FP),Y
   CLC
   ADC   #$02
   LDY   #$45
   STA   (FP),Y
   LDY   #$1C
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
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$47
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$48
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$02
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
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$4C
   STA   (FP),Y
   LDY   #$02
   LDA   (DPTR),Y
   LDY   #$4D
   STA   (FP),Y
   LDY   #$03
   LDA   (DPTR),Y
   LDY   #$4E
   STA   (FP),Y
   LDY   #$04
   LDA   (DPTR),Y
   LDY   #$4F
   STA   (FP),Y
   LDY   #$05
   LDA   (DPTR),Y
   LDY   #$50
   STA   (FP),Y
   LDY   #$06
   LDA   (DPTR),Y
   LDY   #$51
   STA   (FP),Y
   LDY   #$07
   LDA   (DPTR),Y
   LDY   #$52
   STA   (FP),Y
   LDY   #$08
   LDA   (DPTR),Y
   LDY   #$53
   STA   (FP),Y
   LDY   #$09
   LDA   (DPTR),Y
   LDY   #$54
   STA   (FP),Y
   LDY   #$4B
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   LDY   #$4C
   LDA   (FP),Y
   LDY   #$02
   STA   (FP),Y
   LDY   #$4D
   LDA   (FP),Y
   LDY   #$03
   STA   (FP),Y
   LDY   #$4E
   LDA   (FP),Y
   LDY   #$04
   STA   (FP),Y
   LDY   #$4F
   LDA   (FP),Y
   LDY   #$05
   STA   (FP),Y
   LDY   #$50
   LDA   (FP),Y
   LDY   #$06
   STA   (FP),Y
   LDY   #$51
   LDA   (FP),Y
   LDY   #$07
   STA   (FP),Y
   LDY   #$52
   LDA   (FP),Y
   LDY   #$08
   STA   (FP),Y
   LDY   #$53
   LDA   (FP),Y
   LDY   #$09
   STA   (FP),Y
   LDY   #$54
   LDA   (FP),Y
   LDY   #$0A
   STA   (FP),Y
   CLC
   LDA   FP
   ADC   #$01
   LDY   #$55
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
   CMP   #$53
   BNE   .cmp_true@22
   DEY
   LDA   (FP),Y
   CMP   #$60
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
   BNE   .or_true@10
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
   LDY   #$63
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$64
   STA   (FP),Y
   DEY
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
   CMP   #$00
   BNE   .cmp_true@25
   DEY
   LDA   (FP),Y
   CMP   #$58
   BNE   .cmp_true@25
   LDA   #$00
   JMP   .cmp_end@26
.cmp_true@25:
   LDA   #$01
.cmp_end@26:
   LDY   #$67
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
   LDY   #$69
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@11:
   LDY   #$69
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
.if_end@12:
   LDY   #$1B
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
   SEC
   LDA   #$00
   SBC   #$05
   INY
   STA   (FP),Y
   LDA   #$00
   SBC   #$00
   INY
   STA   (FP),Y
   LDY   #$6C
   LDA   (FP),Y
   LDY   #$6E
   CMP   (FP),Y
   BNE   .cmp_true@28
   LDY   #$6B
   LDA   (FP),Y
   LDY   #$6D
   CMP   (FP),Y
   BNE   .cmp_true@28
   LDA   #$00
   JMP   .cmp_end@29
.cmp_true@28:
   LDA   #$01
.cmp_end@29:
   LDY   #$6F
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@13
   LDY   #$1B
   LDA   (FP),Y
   CLC
   ADC   #$04
   LDY   #$71
   STA   (FP),Y
   LDY   #$1C
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
   BNE   .or_true@13
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   JMP   .or_end@14
.or_true@13:
   LDA   #$01
   LDY   #$74
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@14:
   LDY   #$74
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
.if_end@15:
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

test_copy_to_nested_member_pointer:
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
   LDA   #$D0
   INY
   STA   (DPTR),Y
   LDA   #$58
   INY
   STA   (DPTR),Y
   LDA   #$40
   INY
   STA   (DPTR),Y
   LDA   #$06
   LDY   #$0D
   STA   (FP),Y
   LDA   #$12
   INY
   STA   (FP),Y
   LDY   #$0B
   LDA   (FP),Y
   CLC
   ADC   #$08
   LDY   #$0F
   STA   (FP),Y
   LDY   #$0C
   LDA   (FP),Y
   ADC   #$00
   LDY   #$10
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
   LDA   #$01
   LDY   #$11
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   ORA   #$00
   BPL   .sx_done@32
   LDA   #$FF
.sx_done@32:
   LDY   #$13
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$10
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
   SBC   #$08
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
   JSR   calloc
   LDY   #$19
   STA   (FP),Y
   TXA
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
   LDA   #$01
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   ORA   #$00
   BPL   .sx_done@34
   LDA   #$FF
.sx_done@34:
   LDY   #$1F
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$0C
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
   SBC   #$08
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDY   #$1D
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$1E
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$1F
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$20
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   LDY   #$21
   LDA   (FP),Y
   LDY   #$05
   STA   (SSP),Y
   LDY   #$22
   LDA   (FP),Y
   LDY   #$06
   STA   (SSP),Y
   LDY   #$23
   LDA   (FP),Y
   LDY   #$07
   STA   (SSP),Y
   LDY   #$24
   LDA   (FP),Y
   LDY   #$08
   STA   (SSP),Y
   JSR   calloc
   LDY   #$25
   STA   (FP),Y
   TXA
   INY
   STA   (FP),Y
   LDY   #$1B
   LDA   (FP),Y
   CLC
   ADC   #$02
   LDY   #$27
   STA   (FP),Y
   LDY   #$1C
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
   LDY   #$25
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$26
   LDA   (FP),Y
   LDY   #$01
   STA   (DPTR),Y
   LDY   #$1B
   LDA   (FP),Y
   CLC
   ADC   #$02
   LDY   #$29
   STA   (FP),Y
   LDY   #$1C
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
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$2C
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$02
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
   LDY   #$1B
   LDA   (FP),Y
   CLC
   ADC   #$02
   LDY   #$2F
   STA   (FP),Y
   LDY   #$1C
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
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$02
   LDY   #$33
   STA   (FP),Y
   DEY
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
   BNE   .cmp_true@35
   DEY
   LDA   (FP),Y
   CMP   #$58
   BNE   .cmp_true@35
   DEY
   LDA   (FP),Y
   CMP   #$D0
   BNE   .cmp_true@35
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@35
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@35
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@35
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@35
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@35
   LDA   #$00
   JMP   .cmp_end@36
.cmp_true@35:
   LDA   #$01
.cmp_end@36:
   LDY   #$3D
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@2
   JMP   .or_true@16
.lb_skip@2:
   LDY   #$1B
   LDA   (FP),Y
   CLC
   ADC   #$02
   LDY   #$3F
   STA   (FP),Y
   LDY   #$1C
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
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$41
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$42
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$02
   LDY   #$43
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$44
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$08
   LDY   #$45
   STA   (FP),Y
   DEY
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
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$47
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$48
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$49
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$4A
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@38
   LDA   #$00
   JMP   .sx_done@39
.sx_neg@38:
   LDA   #$FF
.sx_done@39:
   LDY   #$4B
   STA   (FP),Y
   INY
   STA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@40
   CMP   #$0F
   BNE   .cmp_true@40
   LDY   #$4A
   LDA   (FP),Y
   CMP   #$12
   BNE   .cmp_true@40
   DEY
   LDA   (FP),Y
   CMP   #$06
   BNE   .cmp_true@40
   LDA   #$00
   JMP   .cmp_end@41
.cmp_true@40:
   LDA   #$01
.cmp_end@41:
   LDY   #$4D
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
   LDY   #$4F
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@17:
   LDY   #$4F
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
.if_end@18:
   LDY   #$1B
   LDA   (FP),Y
   CLC
   ADC   #$02
   LDY   #$51
   STA   (FP),Y
   LDY   #$1C
   LDA   (FP),Y
   ADC   #$00
   LDY   #$52
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$53
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
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
   ORA   #$00
   BNE   .or_true@19
   LDY   #$1B
   LDA   (FP),Y
   CLC
   ADC   #$02
   LDY   #$56
   STA   (FP),Y
   LDY   #$1C
   LDA   (FP),Y
   ADC   #$00
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
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$01
   LDY   #$5A
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$5B
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$5C
   STA   (FP),Y
   ORA   #$00
   BNE   .or_true@19
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   JMP   .or_end@20
.or_true@19:
   LDA   #$01
   LDY   #$5D
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@20:
   LDY   #$5D
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
.if_end@21:
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

test_mixed_nested_access:
   SUBROUTINE

   ; prologue: 0 arg bytes, 239 local bytes
   SEC
   LDA   SSP
   SBC   #$F1
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$F0
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
   LDY   #$11
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
   LDA   #$64
   LDY   #$00
   STA   (DPTR),Y
   LDA   #$00
   INY
   STA   (DPTR),Y
   LDA   #$0E
   LDY   #$13
   STA   (FP),Y
.clear@22:
   SEC
   SBC   #$01
   LDY   #$13
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
   ADC   #$02
   LDY   #$18
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$19
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   LDY   #$14
   ADC   (FP),Y
   LDY   #$1A
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$15
   ADC   (FP),Y
   LDY   #$1B
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
   LDY   #$13
   LDA   (FP),Y
   BNE   .clear@22
   LDA   #$01
   LDY   #$1C
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   ORA   #$00
   BPL   .sx_done@44
   LDA   #$FF
.sx_done@44:
   LDY   #$1E
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$10
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
   SBC   #$08
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
   LDY   #$20
   LDA   (FP),Y
   LDY   #$05
   STA   (SSP),Y
   LDY   #$21
   LDA   (FP),Y
   LDY   #$06
   STA   (SSP),Y
   LDY   #$22
   LDA   (FP),Y
   LDY   #$07
   STA   (SSP),Y
   LDY   #$23
   LDA   (FP),Y
   LDY   #$08
   STA   (SSP),Y
   JSR   calloc
   LDY   #$24
   STA   (FP),Y
   TXA
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$26
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$27
   STA   (FP),Y
   LDA   #$FF
   INY
   STA   (FP),Y
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
   LDA   #$01
   LDY   #$2C
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   ORA   #$00
   BPL   .sx_done@46
   LDA   #$FF
.sx_done@46:
   LDY   #$2E
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$10
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
   SBC   #$08
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDY   #$2C
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$2D
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$2E
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$2F
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   LDY   #$30
   LDA   (FP),Y
   LDY   #$05
   STA   (SSP),Y
   LDY   #$31
   LDA   (FP),Y
   LDY   #$06
   STA   (SSP),Y
   LDY   #$32
   LDA   (FP),Y
   LDY   #$07
   STA   (SSP),Y
   LDY   #$33
   LDA   (FP),Y
   LDY   #$08
   STA   (SSP),Y
   JSR   calloc
   LDY   #$34
   STA   (FP),Y
   TXA
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
   LDY   #$34
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$35
   LDA   (FP),Y
   LDY   #$01
   STA   (DPTR),Y
   LDA   #$7D
   LDY   #$3A
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
   ADC   #$02
   LDY   #$3D
   STA   (FP),Y
   DEY
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
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$3F
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$40
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
   LDA   #$7E
   LDY   #$41
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
   ADC   #$02
   LDY   #$44
   STA   (FP),Y
   DEY
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
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$46
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$47
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$01
   LDY   #$48
   STA   (FP),Y
   DEY
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
   LDY   #$41
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDA   #$00
   LDY   #$4A
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
   LDA   #$49
   INY
   STA   (FP),Y
   LDA   #$40
   EOR   #$80
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
   ADC   #$02
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
   LDY   #$4A
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$4B
   LDA   (FP),Y
   LDY   #$01
   STA   (DPTR),Y
   LDY   #$4C
   LDA   (FP),Y
   LDY   #$02
   STA   (DPTR),Y
   LDY   #$4D
   LDA   (FP),Y
   LDY   #$03
   STA   (DPTR),Y
   LDY   #$4E
   LDA   (FP),Y
   LDY   #$04
   STA   (DPTR),Y
   LDY   #$4F
   LDA   (FP),Y
   LDY   #$05
   STA   (DPTR),Y
   LDY   #$50
   LDA   (FP),Y
   LDY   #$06
   STA   (DPTR),Y
   LDY   #$51
   LDA   (FP),Y
   LDY   #$07
   STA   (DPTR),Y
   SEC
   LDA   #$00
   SBC   #$46
   LDY   #$5A
   STA   (FP),Y
   LDA   #$00
   SBC   #$00
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
   ADC   #$02
   LDY   #$5E
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$5F
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$60
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$61
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$02
   LDY   #$62
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$63
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$08
   LDY   #$64
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$65
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$5A
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$5B
   LDA   (FP),Y
   LDY   #$01
   STA   (DPTR),Y
   LDA   #$65
   LDY   #$66
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
   ADC   #$04
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
   LDY   #$66
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDA   #$66
   LDY   #$6B
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
   ADC   #$04
   LDY   #$6E
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$6F
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$01
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
   LDY   #$6B
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   SEC
   LDA   #$00
   SBC   #$FF
   LDY   #$72
   STA   (FP),Y
   LDA   #$00
   SBC   #$FF
   INY
   STA   (FP),Y
   LDA   #$00
   SBC   #$FF
   INY
   STA   (FP),Y
   LDA   #$00
   SBC   #$7F
   INY
   STA   (FP),Y
   LDY   #$72
   LDA   (FP),Y
   LDY   #$76
   STA   (FP),Y
   LDY   #$73
   LDA   (FP),Y
   LDY   #$77
   STA   (FP),Y
   LDY   #$26
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$76
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$77
   LDA   (FP),Y
   LDY   #$01
   STA   (DPTR),Y
   LDA   #$01
   LDY   #$78
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   ORA   #$00
   BPL   .sx_done@48
   LDA   #$FF
.sx_done@48:
   LDY   #$7A
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$10
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
   SBC   #$08
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDY   #$78
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$79
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$7A
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$7B
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   LDY   #$7C
   LDA   (FP),Y
   LDY   #$05
   STA   (SSP),Y
   LDY   #$7D
   LDA   (FP),Y
   LDY   #$06
   STA   (SSP),Y
   LDY   #$7E
   LDA   (FP),Y
   LDY   #$07
   STA   (SSP),Y
   LDY   #$7F
   LDA   (FP),Y
   LDY   #$08
   STA   (SSP),Y
   JSR   calloc
   LDY   #$80
   STA   (FP),Y
   TXA
   INY
   STA   (FP),Y
   LDY   #$26
   LDA   (FP),Y
   CLC
   ADC   #$02
   LDY   #$82
   STA   (FP),Y
   LDY   #$27
   LDA   (FP),Y
   ADC   #$00
   LDY   #$83
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$80
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$81
   LDA   (FP),Y
   LDY   #$01
   STA   (DPTR),Y
   LDA   #$05
   LDY   #$84
   STA   (FP),Y
   LDY   #$26
   LDA   (FP),Y
   CLC
   ADC   #$02
   LDY   #$85
   STA   (FP),Y
   LDY   #$27
   LDA   (FP),Y
   ADC   #$00
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
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$84
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDA   #$06
   LDY   #$89
   STA   (FP),Y
   LDY   #$26
   LDA   (FP),Y
   CLC
   ADC   #$02
   LDY   #$8A
   STA   (FP),Y
   LDY   #$27
   LDA   (FP),Y
   ADC   #$00
   LDY   #$8B
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$8C
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$8D
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$01
   LDY   #$8E
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$8F
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$89
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$26
   LDA   (FP),Y
   CLC
   ADC   #$04
   LDY   #$90
   STA   (FP),Y
   LDY   #$27
   LDA   (FP),Y
   ADC   #$00
   LDY   #$91
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$02
   LDY   #$92
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$93
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
   LDA   #$84
   INY
   STA   (DPTR),Y
   LDA   #$D7
   INY
   STA   (DPTR),Y
   LDA   #$C7
   INY
   STA   (DPTR),Y
   LDA   #$41
   INY
   STA   (DPTR),Y
   SEC
   LDA   #$00
   SBC   #$05
   LDY   #$94
   STA   (FP),Y
   LDA   #$00
   SBC   #$00
   INY
   STA   (FP),Y
   LDY   #$26
   LDA   (FP),Y
   CLC
   ADC   #$04
   LDY   #$96
   STA   (FP),Y
   LDY   #$27
   LDA   (FP),Y
   ADC   #$00
   LDY   #$97
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$02
   LDY   #$98
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$99
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$08
   LDY   #$9A
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$9B
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$94
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$95
   LDA   (FP),Y
   LDY   #$01
   STA   (DPTR),Y
   LDY   #$26
   LDA   (FP),Y
   CLC
   ADC   #$04
   LDY   #$9C
   STA   (FP),Y
   LDY   #$27
   LDA   (FP),Y
   ADC   #$00
   LDY   #$9D
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$02
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
   LDY   #$04
   LDA   (DPTR),Y
   LDY   #$A4
   STA   (FP),Y
   LDY   #$05
   LDA   (DPTR),Y
   LDY   #$A5
   STA   (FP),Y
   LDY   #$06
   LDA   (DPTR),Y
   LDY   #$A6
   STA   (FP),Y
   LDY   #$07
   LDA   (DPTR),Y
   LDY   #$A7
   STA   (FP),Y
   LDY   #$08
   LDA   (DPTR),Y
   LDY   #$A8
   STA   (FP),Y
   LDY   #$09
   LDA   (DPTR),Y
   LDY   #$A9
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
   ADC   #$02
   LDY   #$AC
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$AD
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$AE
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$AF
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$02
   LDY   #$B0
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$B1
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$A0
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$A1
   LDA   (FP),Y
   LDY   #$01
   STA   (DPTR),Y
   LDY   #$A2
   LDA   (FP),Y
   LDY   #$02
   STA   (DPTR),Y
   LDY   #$A3
   LDA   (FP),Y
   LDY   #$03
   STA   (DPTR),Y
   LDY   #$A4
   LDA   (FP),Y
   LDY   #$04
   STA   (DPTR),Y
   LDY   #$A5
   LDA   (FP),Y
   LDY   #$05
   STA   (DPTR),Y
   LDY   #$A6
   LDA   (FP),Y
   LDY   #$06
   STA   (DPTR),Y
   LDY   #$A7
   LDA   (FP),Y
   LDY   #$07
   STA   (DPTR),Y
   LDY   #$A8
   LDA   (FP),Y
   LDY   #$08
   STA   (DPTR),Y
   LDY   #$A9
   LDA   (FP),Y
   LDY   #$09
   STA   (DPTR),Y
   CLC
   LDA   FP
   ADC   #$01
   LDY   #$B2
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$02
   LDY   #$B4
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$B5
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$B6
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$B7
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$02
   LDY   #$B8
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$B9
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$BA
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$BB
   STA   (FP),Y
   LDY   #$02
   LDA   (DPTR),Y
   LDY   #$BC
   STA   (FP),Y
   LDY   #$03
   LDA   (DPTR),Y
   LDY   #$BD
   STA   (FP),Y
   LDY   #$04
   LDA   (DPTR),Y
   LDY   #$BE
   STA   (FP),Y
   LDY   #$05
   LDA   (DPTR),Y
   LDY   #$BF
   STA   (FP),Y
   LDY   #$06
   LDA   (DPTR),Y
   LDY   #$C0
   STA   (FP),Y
   LDY   #$07
   LDA   (DPTR),Y
   LDY   #$C1
   STA   (FP),Y
   CMP   #$41
   BNE   .cmp_true@49
   DEY
   LDA   (FP),Y
   CMP   #$C7
   BNE   .cmp_true@49
   DEY
   LDA   (FP),Y
   CMP   #$D7
   BNE   .cmp_true@49
   DEY
   LDA   (FP),Y
   CMP   #$84
   BNE   .cmp_true@49
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@49
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@49
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@49
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@49
   LDA   #$00
   JMP   .cmp_end@50
.cmp_true@49:
   LDA   #$01
.cmp_end@50:
   LDY   #$C2
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@4
   JMP   .or_true@23
.lb_skip@4:
   CLC
   LDA   FP
   ADC   #$01
   LDY   #$C4
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$02
   LDY   #$C6
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$C7
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
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$02
   LDY   #$CA
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$CB
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$08
   LDY   #$CC
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$CD
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$CE
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$CF
   STA   (FP),Y
   SEC
   LDA   #$00
   SBC   #$05
   INY
   STA   (FP),Y
   LDA   #$00
   SBC   #$00
   INY
   STA   (FP),Y
   LDY   #$CF
   LDA   (FP),Y
   LDY   #$D1
   CMP   (FP),Y
   BNE   .cmp_true@52
   LDY   #$CE
   LDA   (FP),Y
   LDY   #$D0
   CMP   (FP),Y
   BNE   .cmp_true@52
   LDA   #$00
   JMP   .cmp_end@53
.cmp_true@52:
   LDA   #$01
.cmp_end@53:
   LDY   #$D2
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@23
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   JMP   .or_end@24
.or_true@23:
   LDA   #$01
   LDY   #$D4
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@24:
   LDY   #$D4
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
   ADC   #$F1
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$F0
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
   ADC   #$01
   LDY   #$D6
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$02
   LDY   #$D8
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$D9
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$DA
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$DB
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$DC
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CMP   #$7D
   BNE   .cmp_true@55
   LDA   #$00
   JMP   .cmp_end@56
.cmp_true@55:
   LDA   #$01
.cmp_end@56:
   LDY   #$DF
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@3
   JMP   .or_true@26
.lb_skip@3:
   CLC
   LDA   FP
   ADC   #$01
   LDY   #$E1
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$02
   LDY   #$E3
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$E4
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$E5
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$E6
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$01
   LDY   #$E7
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$E8
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$E9
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CMP   #$7E
   BNE   .cmp_true@57
   LDA   #$00
   JMP   .cmp_end@58
.cmp_true@57:
   LDA   #$01
.cmp_end@58:
   LDY   #$EC
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@26
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   JMP   .or_end@27
.or_true@26:
   LDA   #$01
   LDY   #$EE
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@27:
   LDY   #$EE
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@28
   LDX   #$00
   TXA

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$F1
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
.if_end@28:
   LDX   #$00
   LDA   #$01

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$F1
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$F0
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

test_member_from_cast:
   SUBROUTINE

   ; prologue: 0 arg bytes, 50 local bytes
   SEC
   LDA   SSP
   SBC   #$34
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$33
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
   LDA   #$34
   INY
   STA   (DPTR),Y
   LDA   #$40
   INY
   STA   (DPTR),Y
   LDY   #$0B
   LDA   (FP),Y
   CLC
   ADC   #$08
   LDY   #$0D
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$0E
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
   LDA   #$00
   INY
   STA   (DPTR),Y
   LDA   #$01
   LDY   #$0F
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   ORA   #$00
   BPL   .sx_done@60
   LDA   #$FF
.sx_done@60:
   LDY   #$11
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$0C
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
   SBC   #$08
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDY   #$0F
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$10
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$11
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$12
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   LDY   #$13
   LDA   (FP),Y
   LDY   #$05
   STA   (SSP),Y
   LDY   #$14
   LDA   (FP),Y
   LDY   #$06
   STA   (SSP),Y
   LDY   #$15
   LDA   (FP),Y
   LDY   #$07
   STA   (SSP),Y
   LDY   #$16
   LDA   (FP),Y
   LDY   #$08
   STA   (SSP),Y
   JSR   calloc
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
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$02
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
   LDY   #$19
   LDA   (FP),Y
   CLC
   ADC   #$02
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
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$1F
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$20
   STA   (FP),Y
   LDY   #$02
   LDA   (DPTR),Y
   LDY   #$21
   STA   (FP),Y
   LDY   #$03
   LDA   (DPTR),Y
   LDY   #$22
   STA   (FP),Y
   LDY   #$04
   LDA   (DPTR),Y
   LDY   #$23
   STA   (FP),Y
   LDY   #$05
   LDA   (DPTR),Y
   LDY   #$24
   STA   (FP),Y
   LDY   #$06
   LDA   (DPTR),Y
   LDY   #$25
   STA   (FP),Y
   LDY   #$07
   LDA   (DPTR),Y
   LDY   #$26
   STA   (FP),Y
   CMP   #$40
   BNE   .cmp_true@61
   DEY
   LDA   (FP),Y
   CMP   #$34
   BNE   .cmp_true@61
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@61
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@61
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@61
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@61
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@61
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@61
   LDA   #$00
   JMP   .cmp_end@62
.cmp_true@61:
   LDA   #$01
.cmp_end@62:
   LDY   #$27
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@29
   LDY   #$19
   LDA   (FP),Y
   CLC
   ADC   #$02
   LDY   #$29
   STA   (FP),Y
   LDY   #$1A
   LDA   (FP),Y
   ADC   #$00
   LDY   #$2A
   STA   (FP),Y
   DEY
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
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$2D
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$2E
   STA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@64
   DEY
   LDA   (FP),Y
   CMP   #$0A
   BNE   .cmp_true@64
   LDA   #$00
   JMP   .cmp_end@65
.cmp_true@64:
   LDA   #$01
.cmp_end@65:
   LDY   #$2F
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
   LDY   #$31
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@30:
   LDY   #$31
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@31
   LDX   #$00
   TXA

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$34
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
.if_end@31:
   LDX   #$00
   LDA   #$01

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$34
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$33
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

   ; prologue: 0 arg bytes, 24 local bytes
   SEC
   LDA   SSP
   SBC   #$1A
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$19
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   JSR   test_copy_from_member_pointer
   LDY   #$01
   STA   (FP),Y
   TXA
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lnot_true@67
   LDA   #$00
   JMP   .lnot_end@68
.lnot_true@67:
   LDA   #$01
.lnot_end@68:
   LDY   #$03
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@32
   LDX   #$00
   LDA   #$01

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$1A
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$19
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
.if_end@32:
   JSR   test_copy_to_member_pointer
   LDY   #$05
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
   LDY   #$07
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@33
   LDX   #$00
   LDA   #$02

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$1A
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$19
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
   JSR   test_copy_from_nested_member_pointer
   LDY   #$09
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
   LDY   #$0B
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@34
   LDX   #$00
   LDA   #$03

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$1A
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$19
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
.if_end@34:
   JSR   test_copy_to_nested_member_pointer
   LDY   #$0D
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
   LDY   #$0F
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
   LDA   #$04

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$1A
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$19
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
   JSR   test_mixed_nested_access
   LDY   #$11
   STA   (FP),Y
   TXA
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lnot_true@75
   LDA   #$00
   JMP   .lnot_end@76
.lnot_true@75:
   LDA   #$01
.lnot_end@76:
   LDY   #$13
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
   LDA   #$05

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$1A
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$19
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
   JSR   test_member_from_cast
   LDY   #$15
   STA   (FP),Y
   TXA
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lnot_true@77
   LDA   #$00
   JMP   .lnot_end@78
.lnot_true@77:
   LDA   #$01
.lnot_end@78:
   LDY   #$17
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
   LDA   #$06

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$1A
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
.if_end@37:
   LDX   #$00
   TXA

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$1A
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$19
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
