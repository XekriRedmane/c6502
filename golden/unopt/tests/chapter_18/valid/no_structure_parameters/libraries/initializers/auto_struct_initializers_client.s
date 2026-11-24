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

test_full_initialization:
   SUBROUTINE

   ; prologue: 0 arg bytes, 51 local bytes
   SEC
   LDA   SSP
   SBC   #$35
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$34
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   LDA   #<str@0
   LDY   #$01
   STA   (FP),Y
   LDA   #>str@0
   INY
   STA   (FP),Y
   CLC
   LDA   FP
   ADC   #$03
   LDY   #$14
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
   LDY   #$01
   LDA   (FP),Y
   DEY
   STA   (DPTR),Y
   LDY   #$02
   LDA   (FP),Y
   DEY
   STA   (DPTR),Y
   LDY   #$14
   LDA   (FP),Y
   CLC
   ADC   #$02
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
   LDA   #$73
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$14
   LDA   (FP),Y
   CLC
   ADC   #$03
   LDY   #$18
   STA   (FP),Y
   LDY   #$15
   LDA   (FP),Y
   ADC   #$00
   LDY   #$19
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
   LDY   #$14
   LDA   (FP),Y
   CLC
   ADC   #$04
   LDY   #$1A
   STA   (FP),Y
   LDY   #$15
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
   LDA   #$70
   LDY   #$00
   STA   (DPTR),Y
   CLC
   LDA   FP
   ADC   #$03
   LDY   #$1E
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   LDY   #$14
   LDA   (FP),Y
   CLC
   ADC   #$05
   LDY   #$1C
   STA   (FP),Y
   LDY   #$15
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
   INY
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$1F
   LDA   (FP),Y
   LDY   #$01
   STA   (DPTR),Y
   JSR   get_double
   LDA   HARGS+16
   LDY   #$20
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
   LDY   #$14
   LDA   (FP),Y
   CLC
   ADC   #$07
   LDY   #$28
   STA   (FP),Y
   LDY   #$15
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
   LDY   #$20
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$21
   LDA   (FP),Y
   LDY   #$01
   STA   (DPTR),Y
   LDY   #$22
   LDA   (FP),Y
   LDY   #$02
   STA   (DPTR),Y
   LDY   #$23
   LDA   (FP),Y
   LDY   #$03
   STA   (DPTR),Y
   LDY   #$24
   LDA   (FP),Y
   LDY   #$04
   STA   (DPTR),Y
   LDY   #$25
   LDA   (FP),Y
   LDY   #$05
   STA   (DPTR),Y
   LDY   #$26
   LDA   (FP),Y
   LDY   #$06
   STA   (DPTR),Y
   LDY   #$27
   LDA   (FP),Y
   LDY   #$07
   STA   (DPTR),Y
   CLC
   LDA   FP
   ADC   #$03
   LDY   #$2A
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$07
   LDY   #$2C
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$2D
   STA   (FP),Y
   LDY   #$14
   LDA   (FP),Y
   CLC
   ADC   #$0F
   LDY   #$2E
   STA   (FP),Y
   LDY   #$15
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
   LDY   #$2C
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$2D
   LDA   (FP),Y
   LDY   #$01
   STA   (DPTR),Y
   CLC
   LDA   FP
   ADC   #$03
   LDY   #$30
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
   LDY   #$31
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   JSR   validate_full_initialization
   LDY   #$32
   STA   (FP),Y
   TXA
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
   ADC   #$35
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$34
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

test_partial_initialization:
   SUBROUTINE

   ; prologue: 0 arg bytes, 52 local bytes
   SEC
   LDA   SSP
   SBC   #$36
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$35
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
   LDY   #$12
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
   LDA   @12.msg
   LDY   #$00
   STA   (DPTR),Y
   LDA   @12.msg+1
   INY
   STA   (DPTR),Y
   LDA   #$61
   LDY   #$14
   STA   (FP),Y
   LDY   #$12
   LDA   (FP),Y
   CLC
   ADC   #$02
   LDY   #$15
   STA   (FP),Y
   LDY   #$13
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
   LDA   #$62
   LDY   #$17
   STA   (FP),Y
   LDY   #$12
   LDA   (FP),Y
   CLC
   ADC   #$03
   LDY   #$18
   STA   (FP),Y
   LDY   #$13
   LDA   (FP),Y
   ADC   #$00
   LDY   #$19
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$17
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$12
   LDA   (FP),Y
   CLC
   ADC   #$04
   LDY   #$1A
   STA   (FP),Y
   LDY   #$13
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
   LDA   #$00
   LDY   #$00
   STA   (DPTR),Y
   LDA   #$01
   LDY   #$1C
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   ORA   #$00
   BPL   .sx_done@1
   LDA   #$FF
.sx_done@1:
   LDY   #$1E
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$11
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
   LDY   #$12
   LDA   (FP),Y
   CLC
   ADC   #$05
   LDY   #$26
   STA   (FP),Y
   LDY   #$13
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
   LDY   #$24
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$25
   LDA   (FP),Y
   LDY   #$01
   STA   (DPTR),Y
   LDA   #$0A
   LDY   #$28
   STA   (FP),Y
.clear@0:
   SEC
   SBC   #$01
   LDY   #$28
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
   ADC   #$07
   LDY   #$2D
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$2E
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   LDY   #$29
   ADC   (FP),Y
   LDY   #$2F
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$2A
   ADC   (FP),Y
   LDY   #$30
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
   LDY   #$28
   LDA   (FP),Y
   BNE   .clear@0
   CLC
   LDA   FP
   ADC   #$01
   LDY   #$31
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$04
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   DEY
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$32
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDA   @12.msg
   INY
   STA   (SSP),Y
   LDA   @12.msg+1
   INY
   STA   (SSP),Y
   JSR   validate_partial_initialization
   LDY   #$33
   STA   (FP),Y
   TXA
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
   ADC   #$36
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$35
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

test_implicit_type_conversions:
   SUBROUTINE

   ; prologue: 0 arg bytes, 70 local bytes
   SEC
   LDA   SSP
   SBC   #$48
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$47
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   LDA   #$05
   LDY   #$01
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   ORA   #$00
   BPL   .sx_done@3
   LDA   #$FF
.sx_done@3:
   LDY   #$03
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
   LDY   #$01
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
   JSR   malloc
   LDY   #$05
   STA   (FP),Y
   TXA
   INY
   STA   (FP),Y
   CLC
   LDA   FP
   ADC   #$07
   LDY   #$18
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
   LDY   #$05
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$06
   LDA   (FP),Y
   LDY   #$01
   STA   (DPTR),Y
   LDA   @14.i
   STA   HARGS
   LDA   @14.i+1
   STA   HARGS+1
   LDA   #$02
   STA   HARGS+2
   LDA   #$00
   STA   HARGS+3
   JSR   sdivmod16
   LDA   HARGS+4
   LDY   #$1A
   STA   (FP),Y
   LDA   HARGS+5
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$1C
   STA   (FP),Y
   LDY   #$18
   LDA   (FP),Y
   CLC
   ADC   #$02
   LDY   #$1D
   STA   (FP),Y
   LDY   #$19
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
   LDY   #$1C
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDA   @14.i
   STA   HARGS
   LDA   @14.i+1
   STA   HARGS+1
   LDA   #$03
   STA   HARGS+2
   LDA   #$00
   STA   HARGS+3
   JSR   sdivmod16
   LDA   HARGS+4
   LDY   #$1F
   STA   (FP),Y
   LDA   HARGS+5
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$21
   STA   (FP),Y
   LDY   #$18
   LDA   (FP),Y
   CLC
   ADC   #$03
   LDY   #$22
   STA   (FP),Y
   LDY   #$19
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
   LDY   #$21
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDA   @14.i
   STA   HARGS
   LDA   @14.i+1
   STA   HARGS+1
   LDA   #$04
   STA   HARGS+2
   LDA   #$00
   STA   HARGS+3
   JSR   mul16
   LDA   HARGS+4
   LDY   #$24
   STA   (FP),Y
   LDA   HARGS+5
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$26
   STA   (FP),Y
   LDY   #$18
   LDA   (FP),Y
   CLC
   ADC   #$04
   LDY   #$27
   STA   (FP),Y
   LDY   #$19
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
   LDY   #$26
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$18
   LDA   (FP),Y
   CLC
   ADC   #$05
   LDY   #$29
   STA   (FP),Y
   LDY   #$19
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
   LDA   #$00
   LDY   #$00
   STA   (DPTR),Y
   INY
   STA   (DPTR),Y
   LDA   @14.i
   SEC
   SBC   #$01
   LDY   #$2B
   STA   (FP),Y
   LDA   @14.i+1
   SBC   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   HARGS
   INY
   LDA   (FP),Y
   STA   HARGS+1
   JSR   i2d
   LDA   HARGS+2
   LDY   #$2D
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
   LDY   #$18
   LDA   (FP),Y
   CLC
   ADC   #$07
   LDY   #$35
   STA   (FP),Y
   LDY   #$19
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
   LDY   #$2D
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$2E
   LDA   (FP),Y
   LDY   #$01
   STA   (DPTR),Y
   LDY   #$2F
   LDA   (FP),Y
   LDY   #$02
   STA   (DPTR),Y
   LDY   #$30
   LDA   (FP),Y
   LDY   #$03
   STA   (DPTR),Y
   LDY   #$31
   LDA   (FP),Y
   LDY   #$04
   STA   (DPTR),Y
   LDY   #$32
   LDA   (FP),Y
   LDY   #$05
   STA   (DPTR),Y
   LDY   #$33
   LDA   (FP),Y
   LDY   #$06
   STA   (DPTR),Y
   LDY   #$34
   LDA   (FP),Y
   LDY   #$07
   STA   (DPTR),Y
   LDA   #$01
   LDY   #$37
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   ORA   #$00
   BPL   .sx_done@5
   LDA   #$FF
.sx_done@5:
   LDY   #$39
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$08
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
   LDY   #$37
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$38
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$39
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$3A
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   LDY   #$3B
   LDA   (FP),Y
   LDY   #$05
   STA   (SSP),Y
   LDY   #$3C
   LDA   (FP),Y
   LDY   #$06
   STA   (SSP),Y
   LDY   #$3D
   LDA   (FP),Y
   LDY   #$07
   STA   (SSP),Y
   LDY   #$3E
   LDA   (FP),Y
   LDY   #$08
   STA   (SSP),Y
   JSR   calloc
   LDY   #$3F
   STA   (FP),Y
   TXA
   INY
   STA   (FP),Y
   LDY   #$18
   LDA   (FP),Y
   CLC
   ADC   #$0F
   LDY   #$41
   STA   (FP),Y
   LDY   #$19
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
   LDY   #$3F
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$40
   LDA   (FP),Y
   LDY   #$01
   STA   (DPTR),Y
   CLC
   LDA   FP
   ADC   #$07
   LDY   #$43
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
   LDY   #$44
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   JSR   validate_converted
   LDY   #$45
   STA   (FP),Y
   TXA
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
   ADC   #$48
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$47
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

test_single_exp_initializer:
   SUBROUTINE

   ; prologue: 0 arg bytes, 68 local bytes
   SEC
   LDA   SSP
   SBC   #$46
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$45
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   LDA   #$9A
   LDY   #$01
   STA   (FP),Y
   LDA   #$99
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$D9
   INY
   STA   (FP),Y
   LDA   #$5E
   INY
   STA   (FP),Y
   LDA   #$40
   INY
   STA   (FP),Y
   LDA   #<str@2
   INY
   STA   (FP),Y
   LDA   #>str@2
   INY
   STA   (FP),Y
   CLC
   LDA   FP
   ADC   #$0B
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
   LDY   #$09
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$0A
   LDA   (FP),Y
   LDY   #$01
   STA   (DPTR),Y
   LDY   #$1C
   LDA   (FP),Y
   CLC
   ADC   #$02
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
   LDA   #$78
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$1C
   LDA   (FP),Y
   CLC
   ADC   #$03
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
   LDA   #$79
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$1C
   LDA   (FP),Y
   CLC
   ADC   #$04
   LDY   #$22
   STA   (FP),Y
   LDY   #$1D
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
   LDA   #$00
   LDY   #$00
   STA   (DPTR),Y
   CLC
   LDA   FP
   ADC   #$0B
   LDY   #$26
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   LDY   #$1C
   LDA   (FP),Y
   CLC
   ADC   #$05
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
   INY
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$27
   LDA   (FP),Y
   LDY   #$01
   STA   (DPTR),Y
   LDY   #$1C
   LDA   (FP),Y
   CLC
   ADC   #$07
   LDY   #$28
   STA   (FP),Y
   LDY   #$1D
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
   LDA   #$C0
   INY
   STA   (DPTR),Y
   LDA   #$62
   INY
   STA   (DPTR),Y
   LDA   #$40
   INY
   STA   (DPTR),Y
   CLC
   LDA   FP
   ADC   #$01
   LDY   #$2C
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   LDY   #$1C
   LDA   (FP),Y
   CLC
   ADC   #$0F
   LDY   #$2A
   STA   (FP),Y
   LDY   #$1D
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
   INY
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$2D
   LDA   (FP),Y
   LDY   #$01
   STA   (DPTR),Y
   LDY   #$0B
   LDA   (FP),Y
   LDY   #$2E
   STA   (FP),Y
   LDY   #$0C
   LDA   (FP),Y
   LDY   #$2F
   STA   (FP),Y
   LDY   #$0D
   LDA   (FP),Y
   LDY   #$30
   STA   (FP),Y
   LDY   #$0E
   LDA   (FP),Y
   LDY   #$31
   STA   (FP),Y
   LDY   #$0F
   LDA   (FP),Y
   LDY   #$32
   STA   (FP),Y
   LDY   #$10
   LDA   (FP),Y
   LDY   #$33
   STA   (FP),Y
   LDY   #$11
   LDA   (FP),Y
   LDY   #$34
   STA   (FP),Y
   LDY   #$12
   LDA   (FP),Y
   LDY   #$35
   STA   (FP),Y
   LDY   #$13
   LDA   (FP),Y
   LDY   #$36
   STA   (FP),Y
   LDY   #$14
   LDA   (FP),Y
   LDY   #$37
   STA   (FP),Y
   LDY   #$15
   LDA   (FP),Y
   LDY   #$38
   STA   (FP),Y
   LDY   #$16
   LDA   (FP),Y
   LDY   #$39
   STA   (FP),Y
   LDY   #$17
   LDA   (FP),Y
   LDY   #$3A
   STA   (FP),Y
   LDY   #$18
   LDA   (FP),Y
   LDY   #$3B
   STA   (FP),Y
   LDY   #$19
   LDA   (FP),Y
   LDY   #$3C
   STA   (FP),Y
   LDY   #$1A
   LDA   (FP),Y
   LDY   #$3D
   STA   (FP),Y
   LDY   #$1B
   LDA   (FP),Y
   LDY   #$3E
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
   CLC
   LDA   FP
   ADC   #$2E
   INY
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$04
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDY   #$3F
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$40
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$41
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$42
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   JSR   validate_two_structs
   LDY   #$43
   STA   (FP),Y
   TXA
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
   ADC   #$46
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$45
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

   ; prologue: 0 arg bytes, 16 local bytes
   SEC
   LDA   SSP
   SBC   #$12
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$11
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   JSR   test_full_initialization
   LDY   #$01
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
   LDY   #$03
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
   LDA   #$01

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$12
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$11
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
   JSR   test_partial_initialization
   LDY   #$05
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
   LDY   #$07
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
   LDA   #$02

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$12
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$11
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
   JSR   test_implicit_type_conversions
   LDY   #$09
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
   LDY   #$0B
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
   LDA   #$03

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$12
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$11
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
   JSR   test_single_exp_initializer
   LDY   #$0D
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
   LDY   #$0F
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
   LDA   #$04

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$12
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
   TXA

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$12
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$11
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
   DC.B  $49, $27, $6D, $20, $61, $20, $73, $74, $72, $75, $63, $74, $21, $00

str@1:
   DC.B  $41, $6E, $6F, $74, $68, $65, $72, $20, $73, $74, $72, $69, $6E, $67, $20, $6C
   DC.B  $69, $74, $65, $72, $61, $6C, $00

str@2:
   DC.B  $59, $65, $74, $20, $61, $6E, $6F, $74, $68, $65, $72, $20, $73, $74, $72, $69
   DC.B  $6E, $67, $00

@12.msg:
   DC.W  str@1

@14.i:
   DC.W  $0BB8
