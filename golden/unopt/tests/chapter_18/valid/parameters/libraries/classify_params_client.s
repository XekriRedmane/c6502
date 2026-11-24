SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24

main:
   SUBROUTINE

   ; prologue: 0 arg bytes, 226 local bytes
   SEC
   LDA   SSP
   SBC   #$E4
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$E3
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
   LDY   #$0B
   LDA   (FP),Y
   CLC
   ADC   #$02
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
   LDA   #$6C
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$0B
   LDA   (FP),Y
   CLC
   ADC   #$03
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
   LDA   #$6D
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$0B
   LDA   (FP),Y
   CLC
   ADC   #$04
   LDY   #$11
   STA   (FP),Y
   LDY   #$0C
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
   LDA   #$6E
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$0B
   LDA   (FP),Y
   CLC
   ADC   #$05
   LDY   #$13
   STA   (FP),Y
   LDY   #$0C
   LDA   (FP),Y
   ADC   #$00
   LDY   #$14
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
   LDY   #$0B
   LDA   (FP),Y
   CLC
   ADC   #$06
   LDY   #$15
   STA   (FP),Y
   LDY   #$0C
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
   LDA   #$70
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$0B
   LDA   (FP),Y
   CLC
   ADC   #$07
   LDY   #$17
   STA   (FP),Y
   LDY   #$0C
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
   LDA   #$71
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$0B
   LDA   (FP),Y
   CLC
   ADC   #$08
   LDY   #$19
   STA   (FP),Y
   LDY   #$0C
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
   LDA   #$72
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$0B
   LDA   (FP),Y
   CLC
   ADC   #$09
   LDY   #$1B
   STA   (FP),Y
   LDY   #$0C
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
   SEC
   LDA   SSP
   SBC   #$0A
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   INY
   LDA   (FP),Y
   STA   (SSP),Y
   INY
   LDA   (FP),Y
   STA   (SSP),Y
   INY
   LDA   (FP),Y
   STA   (SSP),Y
   INY
   LDA   (FP),Y
   STA   (SSP),Y
   INY
   LDA   (FP),Y
   STA   (SSP),Y
   INY
   LDA   (FP),Y
   STA   (SSP),Y
   INY
   LDA   (FP),Y
   STA   (SSP),Y
   INY
   LDA   (FP),Y
   STA   (SSP),Y
   INY
   LDA   (FP),Y
   STA   (SSP),Y
   INY
   LDA   (FP),Y
   STA   (SSP),Y
   JSR   test_twelve_bytes
   LDY   #$1D
   STA   (FP),Y
   TXA
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lnot_true@0
   LDA   #$00
   JMP   .lnot_end@1
.lnot_true@0:
   LDA   #$01
.lnot_end@1:
   LDY   #$1F
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
   ADC   #$E4
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$E3
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
   LDA   #$7F
   LDY   #$21
   STA   (FP),Y
   CLC
   LDA   FP
   ADC   #$22
   LDY   #$26
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
   LDY   #$21
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDA   #$FF
   LDY   #$28
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$26
   LDA   (FP),Y
   CLC
   ADC   #$01
   LDY   #$2A
   STA   (FP),Y
   LDY   #$27
   LDA   (FP),Y
   ADC   #$00
   LDY   #$2B
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
   SEC
   LDA   #$00
   SBC   #$80
   LDY   #$2C
   STA   (FP),Y
   LDA   #$00
   SBC   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$2E
   STA   (FP),Y
   LDY   #$26
   LDA   (FP),Y
   CLC
   ADC   #$03
   LDY   #$2F
   STA   (FP),Y
   LDY   #$27
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
   JSR   test_nested_ints
   LDY   #$31
   STA   (FP),Y
   TXA
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lnot_true@2
   LDA   #$00
   JMP   .lnot_end@3
.lnot_true@2:
   LDA   #$01
.lnot_end@3:
   LDY   #$33
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
   ADC   #$E4
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$E3
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
   LDA   #$7F
   LDY   #$35
   STA   (FP),Y
   CLC
   LDA   FP
   ADC   #$36
   LDY   #$3A
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
   LDY   #$35
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDA   #$FF
   LDY   #$3C
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$3A
   LDA   (FP),Y
   CLC
   ADC   #$01
   LDY   #$3E
   STA   (FP),Y
   LDY   #$3B
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
   LDY   #$3C
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$3D
   LDA   (FP),Y
   LDY   #$01
   STA   (DPTR),Y
   SEC
   LDA   #$00
   SBC   #$80
   LDY   #$40
   STA   (FP),Y
   LDA   #$00
   SBC   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$42
   STA   (FP),Y
   LDY   #$3A
   LDA   (FP),Y
   CLC
   ADC   #$03
   LDY   #$43
   STA   (FP),Y
   LDY   #$3B
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
   LDY   #$42
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   SEC
   LDA   SSP
   SBC   #$04
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDY   #$36
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$37
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$38
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$39
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   JSR   test_flattened_ints
   LDY   #$45
   STA   (FP),Y
   TXA
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lnot_true@4
   LDA   #$00
   JMP   .lnot_end@5
.lnot_true@4:
   LDA   #$01
.lnot_end@5:
   LDY   #$47
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
   ADC   #$E4
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$E3
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
   LDA   #$40
   LDY   #$49
   STA   (FP),Y
   LDA   #$0D
   INY
   STA   (FP),Y
   CLC
   LDA   FP
   ADC   #$4B
   LDY   #$5F
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
   LDY   #$49
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$4A
   LDA   (FP),Y
   LDY   #$01
   STA   (DPTR),Y
   LDY   #$5F
   LDA   (FP),Y
   CLC
   ADC   #$02
   LDY   #$61
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$62
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
   LDA   #$40
   INY
   STA   (DPTR),Y
   LDA   #$37
   INY
   STA   (DPTR),Y
   LDA   #$40
   INY
   STA   (DPTR),Y
   LDY   #$5F
   LDA   (FP),Y
   CLC
   ADC   #$0A
   LDY   #$63
   STA   (FP),Y
   LDY   #$60
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
   LDA   #$61
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$5F
   LDA   (FP),Y
   CLC
   ADC   #$0B
   LDY   #$65
   STA   (FP),Y
   LDY   #$60
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
   LDA   #$62
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$5F
   LDA   (FP),Y
   CLC
   ADC   #$0C
   LDY   #$67
   STA   (FP),Y
   LDY   #$60
   LDA   (FP),Y
   ADC   #$00
   LDY   #$68
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
   LDY   #$5F
   LDA   (FP),Y
   CLC
   ADC   #$0D
   LDY   #$69
   STA   (FP),Y
   LDY   #$60
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
   LDA   #$64
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$5F
   LDA   (FP),Y
   CLC
   ADC   #$0E
   LDY   #$6B
   STA   (FP),Y
   LDY   #$60
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
   LDA   #$65
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$5F
   LDA   (FP),Y
   CLC
   ADC   #$0F
   LDY   #$6D
   STA   (FP),Y
   LDY   #$60
   LDA   (FP),Y
   ADC   #$00
   LDY   #$6E
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
   LDY   #$5F
   LDA   (FP),Y
   CLC
   ADC   #$10
   LDY   #$6F
   STA   (FP),Y
   LDY   #$60
   LDA   (FP),Y
   ADC   #$00
   LDY   #$70
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDA   #$67
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$5F
   LDA   (FP),Y
   CLC
   ADC   #$11
   LDY   #$71
   STA   (FP),Y
   LDY   #$60
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
   LDA   #$68
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$5F
   LDA   (FP),Y
   CLC
   ADC   #$12
   LDY   #$73
   STA   (FP),Y
   LDY   #$60
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
   LDA   #$69
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$5F
   LDA   (FP),Y
   CLC
   ADC   #$13
   LDY   #$75
   STA   (FP),Y
   LDY   #$60
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
   LDA   #$00
   LDY   #$00
   STA   (DPTR),Y
   SEC
   LDA   SSP
   SBC   #$14
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDY   #$4B
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$4C
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$4D
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$4E
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   LDY   #$4F
   LDA   (FP),Y
   LDY   #$05
   STA   (SSP),Y
   LDY   #$50
   LDA   (FP),Y
   LDY   #$06
   STA   (SSP),Y
   LDY   #$51
   LDA   (FP),Y
   LDY   #$07
   STA   (SSP),Y
   LDY   #$52
   LDA   (FP),Y
   LDY   #$08
   STA   (SSP),Y
   LDY   #$53
   LDA   (FP),Y
   LDY   #$09
   STA   (SSP),Y
   LDY   #$54
   LDA   (FP),Y
   LDY   #$0A
   STA   (SSP),Y
   LDY   #$55
   LDA   (FP),Y
   LDY   #$0B
   STA   (SSP),Y
   LDY   #$56
   LDA   (FP),Y
   LDY   #$0C
   STA   (SSP),Y
   LDY   #$57
   LDA   (FP),Y
   LDY   #$0D
   STA   (SSP),Y
   LDY   #$58
   LDA   (FP),Y
   LDY   #$0E
   STA   (SSP),Y
   LDY   #$59
   LDA   (FP),Y
   LDY   #$0F
   STA   (SSP),Y
   LDY   #$5A
   LDA   (FP),Y
   LDY   #$10
   STA   (SSP),Y
   LDY   #$5B
   LDA   (FP),Y
   LDY   #$11
   STA   (SSP),Y
   LDY   #$5C
   LDA   (FP),Y
   LDY   #$12
   STA   (SSP),Y
   LDY   #$5D
   LDA   (FP),Y
   LDY   #$13
   STA   (SSP),Y
   LDY   #$5E
   LDA   (FP),Y
   LDY   #$14
   STA   (SSP),Y
   JSR   test_large
   LDY   #$77
   STA   (FP),Y
   TXA
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lnot_true@6
   LDA   #$00
   JMP   .lnot_end@7
.lnot_true@6:
   LDA   #$01
.lnot_end@7:
   LDY   #$79
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
   ADC   #$E4
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$E3
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
   ADC   #$7B
   LDY   #$7F
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
   LDA   #$E7
   LDY   #$00
   STA   (DPTR),Y
   LDA   #$03
   INY
   STA   (DPTR),Y
   LDY   #$7F
   LDA   (FP),Y
   CLC
   ADC   #$02
   LDY   #$81
   STA   (FP),Y
   DEY
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
   LDA   #$78
   LDY   #$00
   STA   (DPTR),Y
   LDA   #$03
   INY
   STA   (DPTR),Y
   SEC
   LDA   SSP
   SBC   #$04
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDY   #$7B
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$7C
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$7D
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$7E
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   JSR   test_two_ints
   LDY   #$83
   STA   (FP),Y
   TXA
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lnot_true@8
   LDA   #$00
   JMP   .lnot_end@9
.lnot_true@8:
   LDA   #$01
.lnot_end@9:
   LDY   #$85
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
   ADC   #$E4
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$E3
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
   CLC
   LDA   FP
   ADC   #$87
   LDY   #$8F
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
   LDA   #$40
   INY
   STA   (DPTR),Y
   LDA   #$89
   INY
   STA   (DPTR),Y
   LDA   #$D8
   INY
   STA   (DPTR),Y
   LDA   #$40
   INY
   STA   (DPTR),Y
   SEC
   LDA   SSP
   SBC   #$08
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDY   #$87
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$88
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$89
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$8A
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   LDY   #$8B
   LDA   (FP),Y
   LDY   #$05
   STA   (SSP),Y
   LDY   #$8C
   LDA   (FP),Y
   LDY   #$06
   STA   (SSP),Y
   LDY   #$8D
   LDA   (FP),Y
   LDY   #$07
   STA   (SSP),Y
   LDY   #$8E
   LDA   (FP),Y
   LDY   #$08
   STA   (SSP),Y
   JSR   test_nested_double
   LDY   #$91
   STA   (FP),Y
   TXA
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lnot_true@10
   LDA   #$00
   JMP   .lnot_end@11
.lnot_true@10:
   LDA   #$01
.lnot_end@11:
   LDY   #$93
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
   ADC   #$E4
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$E3
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
   CLC
   LDA   FP
   ADC   #$95
   LDY   #$9E
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
   LDA   #$40
   INY
   STA   (DPTR),Y
   LDA   #$8F
   INY
   STA   (DPTR),Y
   LDA   #$40
   INY
   STA   (DPTR),Y
   LDA   #$78
   LDY   #$A0
   STA   (FP),Y
   LDY   #$9E
   LDA   (FP),Y
   CLC
   ADC   #$08
   LDY   #$A1
   STA   (FP),Y
   LDY   #$9F
   LDA   (FP),Y
   ADC   #$00
   LDY   #$A2
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
   SEC
   LDA   SSP
   SBC   #$09
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDY   #$95
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$96
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$97
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$98
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   LDY   #$99
   LDA   (FP),Y
   LDY   #$05
   STA   (SSP),Y
   LDY   #$9A
   LDA   (FP),Y
   LDY   #$06
   STA   (SSP),Y
   LDY   #$9B
   LDA   (FP),Y
   LDY   #$07
   STA   (SSP),Y
   LDY   #$9C
   LDA   (FP),Y
   LDY   #$08
   STA   (SSP),Y
   LDY   #$9D
   LDA   (FP),Y
   LDY   #$09
   STA   (SSP),Y
   JSR   test_two_eightbytes
   LDY   #$A3
   STA   (FP),Y
   TXA
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lnot_true@12
   LDA   #$00
   JMP   .lnot_end@13
.lnot_true@12:
   LDA   #$01
.lnot_end@13:
   LDY   #$A5
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
   ADC   #$E4
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$E3
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
   CLC
   LDA   FP
   ADC   #$A7
   LDY   #$BD
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
   LDA   #$76
   LDY   #$00
   STA   (DPTR),Y
   LDA   #$3B
   INY
   STA   (DPTR),Y
   LDA   #$77
   INY
   STA   (DPTR),Y
   LDA   #$30
   INY
   STA   (DPTR),Y
   LDA   #$D1
   INY
   STA   (DPTR),Y
   LDA   #$42
   INY
   STA   (DPTR),Y
   LDA   #$EE
   INY
   STA   (DPTR),Y
   LDA   #$7F
   INY
   STA   (DPTR),Y
   LDA   #$76
   LDY   #$BF
   STA   (FP),Y
   LDA   #$3B
   INY
   STA   (FP),Y
   LDA   #$77
   INY
   STA   (FP),Y
   LDA   #$30
   INY
   STA   (FP),Y
   LDA   #$D1
   INY
   STA   (FP),Y
   LDA   #$42
   INY
   STA   (FP),Y
   LDA   #$EE
   INY
   STA   (FP),Y
   LDA   #$7F
   EOR   #$80
   INY
   STA   (FP),Y
   LDY   #$BD
   LDA   (FP),Y
   CLC
   ADC   #$08
   LDY   #$C7
   STA   (FP),Y
   LDY   #$BE
   LDA   (FP),Y
   ADC   #$00
   LDY   #$C8
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$BF
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$C0
   LDA   (FP),Y
   LDY   #$01
   STA   (DPTR),Y
   LDY   #$C1
   LDA   (FP),Y
   LDY   #$02
   STA   (DPTR),Y
   LDY   #$C2
   LDA   (FP),Y
   LDY   #$03
   STA   (DPTR),Y
   LDY   #$C3
   LDA   (FP),Y
   LDY   #$04
   STA   (DPTR),Y
   LDY   #$C4
   LDA   (FP),Y
   LDY   #$05
   STA   (DPTR),Y
   LDY   #$C5
   LDA   (FP),Y
   LDY   #$06
   STA   (DPTR),Y
   LDY   #$C6
   LDA   (FP),Y
   LDY   #$07
   STA   (DPTR),Y
   SEC
   LDA   #$00
   SBC   #$FF
   LDY   #$C9
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
   LDY   #$C9
   LDA   (FP),Y
   LDY   #$CD
   STA   (FP),Y
   LDY   #$CA
   LDA   (FP),Y
   LDY   #$CE
   STA   (FP),Y
   LDY   #$BD
   LDA   (FP),Y
   CLC
   ADC   #$10
   LDY   #$CF
   STA   (FP),Y
   LDY   #$BE
   LDA   (FP),Y
   ADC   #$00
   LDY   #$D0
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$CD
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$CE
   LDA   (FP),Y
   LDY   #$01
   STA   (DPTR),Y
   SEC
   LDA   #$00
   SBC   #$FF
   LDY   #$D1
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
   SBC   #$FF
   INY
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
   SBC   #$FF
   INY
   STA   (FP),Y
   LDA   #$00
   SBC   #$7F
   INY
   STA   (FP),Y
   LDY   #$D1
   LDA   (FP),Y
   LDY   #$D9
   STA   (FP),Y
   LDY   #$D2
   LDA   (FP),Y
   LDY   #$DA
   STA   (FP),Y
   LDY   #$D3
   LDA   (FP),Y
   LDY   #$DB
   STA   (FP),Y
   LDY   #$D4
   LDA   (FP),Y
   LDY   #$DC
   STA   (FP),Y
   LDY   #$BD
   LDA   (FP),Y
   CLC
   ADC   #$12
   LDY   #$DD
   STA   (FP),Y
   LDY   #$BE
   LDA   (FP),Y
   ADC   #$00
   LDY   #$DE
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$D9
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$DA
   LDA   (FP),Y
   LDY   #$01
   STA   (DPTR),Y
   LDY   #$DB
   LDA   (FP),Y
   LDY   #$02
   STA   (DPTR),Y
   LDY   #$DC
   LDA   (FP),Y
   LDY   #$03
   STA   (DPTR),Y
   SEC
   LDA   SSP
   SBC   #$16
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDY   #$A7
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$A8
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$A9
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$AA
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   LDY   #$AB
   LDA   (FP),Y
   LDY   #$05
   STA   (SSP),Y
   LDY   #$AC
   LDA   (FP),Y
   LDY   #$06
   STA   (SSP),Y
   LDY   #$AD
   LDA   (FP),Y
   LDY   #$07
   STA   (SSP),Y
   LDY   #$AE
   LDA   (FP),Y
   LDY   #$08
   STA   (SSP),Y
   LDY   #$AF
   LDA   (FP),Y
   LDY   #$09
   STA   (SSP),Y
   LDY   #$B0
   LDA   (FP),Y
   LDY   #$0A
   STA   (SSP),Y
   LDY   #$B1
   LDA   (FP),Y
   LDY   #$0B
   STA   (SSP),Y
   LDY   #$B2
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
   LDY   #$B5
   LDA   (FP),Y
   LDY   #$0F
   STA   (SSP),Y
   LDY   #$B6
   LDA   (FP),Y
   LDY   #$10
   STA   (SSP),Y
   LDY   #$B7
   LDA   (FP),Y
   LDY   #$11
   STA   (SSP),Y
   LDY   #$B8
   LDA   (FP),Y
   LDY   #$12
   STA   (SSP),Y
   LDY   #$B9
   LDA   (FP),Y
   LDY   #$13
   STA   (SSP),Y
   LDY   #$BA
   LDA   (FP),Y
   LDY   #$14
   STA   (SSP),Y
   LDY   #$BB
   LDA   (FP),Y
   LDY   #$15
   STA   (SSP),Y
   LDY   #$BC
   LDA   (FP),Y
   LDY   #$16
   STA   (SSP),Y
   JSR   test_pass_in_memory
   LDY   #$DF
   STA   (FP),Y
   TXA
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lnot_true@14
   LDA   #$00
   JMP   .lnot_end@15
.lnot_true@14:
   LDA   #$01
.lnot_end@15:
   LDY   #$E1
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
   ADC   #$E4
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
.if_end@7:
   LDX   #$00
   TXA

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$E4
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$E3
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
