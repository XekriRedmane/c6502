SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24

main:
   SUBROUTINE

   ; prologue: 0 arg bytes, 86 local bytes
   SEC
   LDA   SSP
   SBC   #$58
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$57
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
   LDA   #$01
   LDY   #$00
   STA   (DPTR),Y
   LDA   #$00
   INY
   STA   (DPTR),Y
   LDY   #$21
   LDA   (FP),Y
   CLC
   ADC   #$02
   LDY   #$23
   STA   (FP),Y
   DEY
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
   LDA   #$02
   LDY   #$00
   STA   (DPTR),Y
   LDA   #$00
   INY
   STA   (DPTR),Y
   LDY   #$21
   LDA   (FP),Y
   CLC
   ADC   #$04
   LDY   #$25
   STA   (FP),Y
   LDY   #$22
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
   LDA   #$03
   LDY   #$00
   STA   (DPTR),Y
   LDA   #$00
   INY
   STA   (DPTR),Y
   LDY   #$21
   LDA   (FP),Y
   CLC
   ADC   #$06
   LDY   #$27
   STA   (FP),Y
   LDY   #$22
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
   LDA   #$04
   LDY   #$00
   STA   (DPTR),Y
   LDA   #$00
   INY
   STA   (DPTR),Y
   LDY   #$21
   LDA   (FP),Y
   CLC
   ADC   #$08
   LDY   #$29
   STA   (FP),Y
   LDY   #$22
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
   LDA   #$05
   LDY   #$00
   STA   (DPTR),Y
   LDA   #$00
   INY
   STA   (DPTR),Y
   LDY   #$21
   LDA   (FP),Y
   CLC
   ADC   #$0A
   LDY   #$2B
   STA   (FP),Y
   LDY   #$22
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
   LDA   #$06
   LDY   #$00
   STA   (DPTR),Y
   LDA   #$00
   INY
   STA   (DPTR),Y
   LDY   #$21
   LDA   (FP),Y
   CLC
   ADC   #$0C
   LDY   #$2D
   STA   (FP),Y
   LDY   #$22
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
   LDA   #$07
   LDY   #$00
   STA   (DPTR),Y
   LDA   #$00
   INY
   STA   (DPTR),Y
   LDY   #$21
   LDA   (FP),Y
   CLC
   ADC   #$0E
   LDY   #$2F
   STA   (FP),Y
   LDY   #$22
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
   LDA   #$08
   LDY   #$00
   STA   (DPTR),Y
   LDA   #$00
   INY
   STA   (DPTR),Y
   LDY   #$21
   LDA   (FP),Y
   CLC
   ADC   #$10
   LDY   #$31
   STA   (FP),Y
   LDY   #$22
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
   LDA   #$09
   LDY   #$00
   STA   (DPTR),Y
   LDA   #$00
   INY
   STA   (DPTR),Y
   LDY   #$21
   LDA   (FP),Y
   CLC
   ADC   #$12
   LDY   #$33
   STA   (FP),Y
   LDY   #$22
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
   LDA   #$0A
   LDY   #$00
   STA   (DPTR),Y
   LDA   #$00
   INY
   STA   (DPTR),Y
   LDY   #$21
   LDA   (FP),Y
   CLC
   ADC   #$14
   LDY   #$35
   STA   (FP),Y
   LDY   #$22
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
   LDA   #$0B
   LDY   #$00
   STA   (DPTR),Y
   LDA   #$00
   INY
   STA   (DPTR),Y
   LDY   #$21
   LDA   (FP),Y
   CLC
   ADC   #$16
   LDY   #$37
   STA   (FP),Y
   LDY   #$22
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
   LDA   #$0C
   LDY   #$00
   STA   (DPTR),Y
   LDA   #$00
   INY
   STA   (DPTR),Y
   LDY   #$21
   LDA   (FP),Y
   CLC
   ADC   #$18
   LDY   #$39
   STA   (FP),Y
   LDY   #$22
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
   LDA   #$0D
   LDY   #$00
   STA   (DPTR),Y
   LDA   #$00
   INY
   STA   (DPTR),Y
   LDY   #$21
   LDA   (FP),Y
   CLC
   ADC   #$1A
   LDY   #$3B
   STA   (FP),Y
   LDY   #$22
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
   LDA   #$0E
   LDY   #$00
   STA   (DPTR),Y
   LDA   #$00
   INY
   STA   (DPTR),Y
   LDY   #$21
   LDA   (FP),Y
   CLC
   ADC   #$1C
   LDY   #$3D
   STA   (FP),Y
   LDY   #$22
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
   LDA   #$0F
   LDY   #$00
   STA   (DPTR),Y
   LDA   #$00
   INY
   STA   (DPTR),Y
   LDY   #$21
   LDA   (FP),Y
   CLC
   ADC   #$1E
   LDY   #$3F
   STA   (FP),Y
   LDY   #$22
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
   LDA   #$10
   LDY   #$00
   STA   (DPTR),Y
   LDA   #$00
   INY
   STA   (DPTR),Y
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
.mul_loop@0:
   BCC   .mul_skip@1
   CLC
   ADC   #$08
.mul_skip@1:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@0
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
   LDA   #$01
   STA   HARGS
   LDA   #$00
   LDX   #$08
   LSR   HARGS
.mul_loop@2:
   BCC   .mul_skip@3
   CLC
   ADC   #$02
.mul_skip@3:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@2
   LDY   #$4A
   STA   (FP),Y
   LDA   HARGS
   DEY
   STA   (FP),Y
   LDY   #$47
   LDA   (FP),Y
   CLC
   LDY   #$49
   ADC   (FP),Y
   LDY   #$4B
   STA   (FP),Y
   LDY   #$48
   LDA   (FP),Y
   LDY   #$4A
   ADC   (FP),Y
   LDY   #$4C
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$4D
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$4E
   STA   (FP),Y
   SEC
   LDA   #$00
   DEY
   SBC   (FP),Y
   LDY   #$4F
   STA   (FP),Y
   LDA   #$00
   DEY
   SBC   (FP),Y
   LDY   #$50
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$51
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$52
   STA   (FP),Y
   SEC
   LDA   #$00
   SBC   #$0B
   INY
   STA   (FP),Y
   LDA   #$00
   SBC   #$00
   INY
   STA   (FP),Y
   LDY   #$52
   LDA   (FP),Y
   LDY   #$54
   CMP   (FP),Y
   BNE   .thread@0
   LDY   #$51
   LDA   (FP),Y
   LDY   #$53
   CMP   (FP),Y
   BEQ   .cmp_true@4
.thread@0:
   LDA   #$00
   JMP   .cmp_end@5
.cmp_true@4:
   LDA   #$01
.cmp_end@5:
   LDY   #$55
   STA   (FP),Y
   LDA   #$00
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
   ADC   #$58
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$57
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
