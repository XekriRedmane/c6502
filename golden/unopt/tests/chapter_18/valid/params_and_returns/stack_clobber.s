SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24

validate_stack_bytes:
   SUBROUTINE

   ; prologue: 2 arg bytes, 6 local bytes
   SEC
   LDA   SSP
   SBC   #$08
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$07
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   LDA   #<to_validate
   LDY   #$01
   STA   (FP),Y
   LDA   #>to_validate
   INY
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
   JSR   strcmp
   LDY   #$05
   STA   (FP),Y
   TXA
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@0
   SEC
   LDA   SSP
   SBC   #$02
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDY   #$09
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$0A
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   JSR   exit
.if_end@0:

   ; epilogue
   CLC
   LDA   FP
   ADC   #$0A
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$07
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   RTS

return_int_struct:
   SUBROUTINE

   ; prologue: 2 arg bytes, 13 local bytes
   SEC
   LDA   SSP
   SBC   #$0F
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$0E
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
   LDY   #$08
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
   LDY   #$08
   LDA   (FP),Y
   CLC
   ADC   #$04
   LDY   #$0A
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$0B
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
   LDY   #$08
   LDA   (FP),Y
   CLC
   ADC   #$06
   LDY   #$0C
   STA   (FP),Y
   LDY   #$09
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
   LDA   #$00
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$10
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

   ; epilogue
   CLC
   LDA   FP
   ADC   #$11
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$0E
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   RTS

validate_one_int_struct:
   SUBROUTINE

   ; prologue: 2 arg bytes, 14 local bytes
   SEC
   LDA   SSP
   SBC   #$10
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$0F
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
.loop@0_start:
   LDY   #$01
   LDA   (FP),Y
   SEC
   SBC   #$07
   INY
   LDA   (FP),Y
   SBC   #$00
   BVC   .cmp_novf@0
   EOR   #$80
.cmp_novf@0:
   ASL
   LDA   #$00
   ROL
   LDY   #$03
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@0
   JMP   .loop@0_break
.lb_skip@0:
   LDA   #<one_int_struct
   LDY   #$05
   STA   (FP),Y
   LDA   #>one_int_struct
   INY
   STA   (FP),Y
   LDY   #$01
   LDA   (FP),Y
   LDY   #$07
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$05
   LDA   (FP),Y
   CLC
   LDY   #$08
   ADC   (FP),Y
   LDY   #$0A
   STA   (FP),Y
   LDY   #$06
   LDA   (FP),Y
   LDY   #$09
   ADC   (FP),Y
   LDY   #$0B
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$0C
   STA   (FP),Y
   ORA   #$00
   BEQ   .if_end@1
   SEC
   LDA   SSP
   SBC   #$02
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
   JSR   exit
.if_end@1:
   LDY   #$01
   LDA   (FP),Y
   CLC
   ADC   #$01
   LDY   #$0D
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   ADC   #$00
   LDY   #$0E
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   LDY   #$0E
   LDA   (FP),Y
   LDY   #$02
   STA   (FP),Y
   JMP   .loop@0_start
.loop@0_break:

   ; epilogue
   CLC
   LDA   FP
   ADC   #$12
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$0F
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   RTS

test_int_struct:
   SUBROUTINE

   ; prologue: 0 arg bytes, 57 local bytes
   SEC
   LDA   SSP
   SBC   #$3B
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$3A
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
   LDA   #$65
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$11
   LDA   (FP),Y
   CLC
   ADC   #$01
   LDY   #$13
   STA   (FP),Y
   DEY
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
   LDA   #$66
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$11
   LDA   (FP),Y
   CLC
   ADC   #$02
   LDY   #$15
   STA   (FP),Y
   LDY   #$12
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
   LDA   #$67
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$11
   LDA   (FP),Y
   CLC
   ADC   #$03
   LDY   #$17
   STA   (FP),Y
   LDY   #$12
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
   LDA   #$68
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$11
   LDA   (FP),Y
   CLC
   ADC   #$04
   LDY   #$19
   STA   (FP),Y
   LDY   #$12
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
   LDA   #$69
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$11
   LDA   (FP),Y
   CLC
   ADC   #$05
   LDY   #$1B
   STA   (FP),Y
   LDY   #$12
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
   LDA   #$6A
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$11
   LDA   (FP),Y
   CLC
   ADC   #$06
   LDY   #$1D
   STA   (FP),Y
   LDY   #$12
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
   LDA   #$6B
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$11
   LDA   (FP),Y
   CLC
   ADC   #$07
   LDY   #$1F
   STA   (FP),Y
   LDY   #$12
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
   LDA   #$6C
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$11
   LDA   (FP),Y
   CLC
   ADC   #$08
   LDY   #$21
   STA   (FP),Y
   LDY   #$12
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
   LDA   #$6D
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$11
   LDA   (FP),Y
   CLC
   ADC   #$09
   LDY   #$23
   STA   (FP),Y
   LDY   #$12
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
   LDA   #$6E
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$11
   LDA   (FP),Y
   CLC
   ADC   #$0A
   LDY   #$25
   STA   (FP),Y
   LDY   #$12
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
   LDA   #$6F
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$11
   LDA   (FP),Y
   CLC
   ADC   #$0B
   LDY   #$27
   STA   (FP),Y
   LDY   #$12
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
   LDA   #$70
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$11
   LDA   (FP),Y
   CLC
   ADC   #$0C
   LDY   #$29
   STA   (FP),Y
   LDY   #$12
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
   LDA   #$71
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$11
   LDA   (FP),Y
   CLC
   ADC   #$0D
   LDY   #$2B
   STA   (FP),Y
   LDY   #$12
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
   LDA   #$72
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$11
   LDA   (FP),Y
   CLC
   ADC   #$0E
   LDY   #$2D
   STA   (FP),Y
   LDY   #$12
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
   LDA   #$73
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$11
   LDA   (FP),Y
   CLC
   ADC   #$0F
   LDY   #$2F
   STA   (FP),Y
   LDY   #$12
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
   LDA   #$00
   LDY   #$00
   STA   (DPTR),Y
   CLC
   LDA   FP
   ADC   #$31
   LDY   #$38
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
   LDY   #$39
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   JSR   return_int_struct
   LDY   #$31
   LDA   (FP),Y
   STA   one_int_struct
   INY
   LDA   (FP),Y
   STA   one_int_struct+1
   INY
   LDA   (FP),Y
   STA   one_int_struct+2
   INY
   LDA   (FP),Y
   STA   one_int_struct+3
   INY
   LDA   (FP),Y
   STA   one_int_struct+4
   INY
   LDA   (FP),Y
   STA   one_int_struct+5
   INY
   LDA   (FP),Y
   STA   one_int_struct+6
   LDY   #$01
   LDA   (FP),Y
   STA   to_validate
   INY
   LDA   (FP),Y
   STA   to_validate+1
   INY
   LDA   (FP),Y
   STA   to_validate+2
   INY
   LDA   (FP),Y
   STA   to_validate+3
   INY
   LDA   (FP),Y
   STA   to_validate+4
   INY
   LDA   (FP),Y
   STA   to_validate+5
   INY
   LDA   (FP),Y
   STA   to_validate+6
   INY
   LDA   (FP),Y
   STA   to_validate+7
   INY
   LDA   (FP),Y
   STA   to_validate+8
   INY
   LDA   (FP),Y
   STA   to_validate+9
   INY
   LDA   (FP),Y
   STA   to_validate+10
   INY
   LDA   (FP),Y
   STA   to_validate+11
   INY
   LDA   (FP),Y
   STA   to_validate+12
   INY
   LDA   (FP),Y
   STA   to_validate+13
   INY
   LDA   (FP),Y
   STA   to_validate+14
   INY
   LDA   (FP),Y
   STA   to_validate+15
   SEC
   LDA   SSP
   SBC   #$02
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   #$01
   LDY   #$01
   STA   (SSP),Y
   LDA   #$00
   INY
   STA   (SSP),Y
   JSR   validate_stack_bytes
   SEC
   LDA   SSP
   SBC   #$02
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   #$02
   LDY   #$01
   STA   (SSP),Y
   LDA   #$00
   INY
   STA   (SSP),Y
   JSR   validate_one_int_struct
   LDX   #$00
   TXA

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$3B
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$3A
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

return_two_int_struct:
   SUBROUTINE

   ; prologue: 2 arg bytes, 60 local bytes
   SEC
   LDA   SSP
   SBC   #$3E
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$3D
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   LDA   #$14
   LDY   #$01
   STA   (FP),Y
   CLC
   LDA   FP
   ADC   #$02
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
   LDY   #$01
   LDA   (FP),Y
   DEY
   STA   (DPTR),Y
   LDA   #$15
   LDY   #$13
   STA   (FP),Y
   LDY   #$11
   LDA   (FP),Y
   CLC
   ADC   #$01
   LDY   #$14
   STA   (FP),Y
   LDY   #$12
   LDA   (FP),Y
   ADC   #$00
   LDY   #$15
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
   LDA   #$16
   LDY   #$16
   STA   (FP),Y
   LDY   #$11
   LDA   (FP),Y
   CLC
   ADC   #$02
   LDY   #$17
   STA   (FP),Y
   LDY   #$12
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
   LDY   #$16
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDA   #$17
   LDY   #$19
   STA   (FP),Y
   LDY   #$11
   LDA   (FP),Y
   CLC
   ADC   #$03
   LDY   #$1A
   STA   (FP),Y
   LDY   #$12
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
   LDY   #$19
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDA   #$18
   LDY   #$1C
   STA   (FP),Y
   LDY   #$11
   LDA   (FP),Y
   CLC
   ADC   #$04
   LDY   #$1D
   STA   (FP),Y
   LDY   #$12
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
   LDA   #$19
   LDY   #$1F
   STA   (FP),Y
   LDY   #$11
   LDA   (FP),Y
   CLC
   ADC   #$05
   LDY   #$20
   STA   (FP),Y
   LDY   #$12
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
   LDY   #$1F
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDA   #$1A
   LDY   #$22
   STA   (FP),Y
   LDY   #$11
   LDA   (FP),Y
   CLC
   ADC   #$06
   LDY   #$23
   STA   (FP),Y
   LDY   #$12
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
   LDA   #$1B
   LDY   #$25
   STA   (FP),Y
   LDY   #$11
   LDA   (FP),Y
   CLC
   ADC   #$07
   LDY   #$26
   STA   (FP),Y
   LDY   #$12
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
   LDY   #$25
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDA   #$1C
   LDY   #$28
   STA   (FP),Y
   LDY   #$11
   LDA   (FP),Y
   CLC
   ADC   #$08
   LDY   #$29
   STA   (FP),Y
   LDY   #$12
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
   LDY   #$28
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDA   #$1D
   LDY   #$2B
   STA   (FP),Y
   LDY   #$11
   LDA   (FP),Y
   CLC
   ADC   #$09
   LDY   #$2C
   STA   (FP),Y
   LDY   #$12
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
   LDY   #$2B
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDA   #$1E
   LDY   #$2E
   STA   (FP),Y
   LDY   #$11
   LDA   (FP),Y
   CLC
   ADC   #$0A
   LDY   #$2F
   STA   (FP),Y
   LDY   #$12
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
   LDA   #$1F
   LDY   #$31
   STA   (FP),Y
   LDY   #$11
   LDA   (FP),Y
   CLC
   ADC   #$0B
   LDY   #$32
   STA   (FP),Y
   LDY   #$12
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
   LDY   #$31
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDA   #$20
   LDY   #$34
   STA   (FP),Y
   LDY   #$11
   LDA   (FP),Y
   CLC
   ADC   #$0C
   LDY   #$35
   STA   (FP),Y
   LDY   #$12
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
   LDY   #$34
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDA   #$21
   LDY   #$37
   STA   (FP),Y
   LDY   #$11
   LDA   (FP),Y
   CLC
   ADC   #$0D
   LDY   #$38
   STA   (FP),Y
   LDY   #$12
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
   LDY   #$37
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDA   #$22
   LDY   #$3A
   STA   (FP),Y
   LDY   #$11
   LDA   (FP),Y
   CLC
   ADC   #$0E
   LDY   #$3B
   STA   (FP),Y
   LDY   #$12
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
   LDY   #$3A
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$3F
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$02
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$03
   LDA   (FP),Y
   LDY   #$01
   STA   (DPTR),Y
   LDY   #$04
   LDA   (FP),Y
   LDY   #$02
   STA   (DPTR),Y
   LDY   #$05
   LDA   (FP),Y
   LDY   #$03
   STA   (DPTR),Y
   LDY   #$06
   LDA   (FP),Y
   LDY   #$04
   STA   (DPTR),Y
   LDY   #$07
   LDA   (FP),Y
   LDY   #$05
   STA   (DPTR),Y
   LDY   #$08
   LDA   (FP),Y
   LDY   #$06
   STA   (DPTR),Y
   LDY   #$09
   LDA   (FP),Y
   LDY   #$07
   STA   (DPTR),Y
   LDY   #$0A
   LDA   (FP),Y
   LDY   #$08
   STA   (DPTR),Y
   LDY   #$0B
   LDA   (FP),Y
   LDY   #$09
   STA   (DPTR),Y
   LDY   #$0C
   LDA   (FP),Y
   LDY   #$0A
   STA   (DPTR),Y
   LDY   #$0D
   LDA   (FP),Y
   LDY   #$0B
   STA   (DPTR),Y
   LDY   #$0E
   LDA   (FP),Y
   LDY   #$0C
   STA   (DPTR),Y
   LDY   #$0F
   LDA   (FP),Y
   LDY   #$0D
   STA   (DPTR),Y
   LDY   #$10
   LDA   (FP),Y
   LDY   #$0E
   STA   (DPTR),Y

   ; epilogue
   CLC
   LDA   FP
   ADC   #$40
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$3D
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   RTS

validate_two_int_struct:
   SUBROUTINE

   ; prologue: 2 arg bytes, 20 local bytes
   SEC
   LDA   SSP
   SBC   #$16
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$15
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
.loop@1_start:
   LDY   #$01
   LDA   (FP),Y
   SEC
   SBC   #$0F
   INY
   LDA   (FP),Y
   SBC   #$00
   BVC   .cmp_novf@1
   EOR   #$80
.cmp_novf@1:
   ASL
   LDA   #$00
   ROL
   LDY   #$03
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@1
   JMP   .loop@1_break
.lb_skip@1:
   LDA   #<two_int_struct
   LDY   #$05
   STA   (FP),Y
   LDA   #>two_int_struct
   INY
   STA   (FP),Y
   LDY   #$01
   LDA   (FP),Y
   LDY   #$07
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$05
   LDA   (FP),Y
   CLC
   LDY   #$08
   ADC   (FP),Y
   LDY   #$0A
   STA   (FP),Y
   LDY   #$06
   LDA   (FP),Y
   LDY   #$09
   ADC   (FP),Y
   LDY   #$0B
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$0C
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$01
   LDA   (FP),Y
   CLC
   ADC   #$14
   LDY   #$0F
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   ADC   #$00
   LDY   #$10
   STA   (FP),Y
   LDY   #$0E
   LDA   (FP),Y
   LDY   #$10
   CMP   (FP),Y
   BNE   .cmp_true@2
   LDY   #$0D
   LDA   (FP),Y
   LDY   #$0F
   CMP   (FP),Y
   BNE   .cmp_true@2
   LDA   #$00
   JMP   .cmp_end@3
.cmp_true@2:
   LDA   #$01
.cmp_end@3:
   LDY   #$11
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@2
   SEC
   LDA   SSP
   SBC   #$02
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDY   #$17
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$18
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   JSR   exit
.if_end@2:
   LDY   #$01
   LDA   (FP),Y
   CLC
   ADC   #$01
   LDY   #$13
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   ADC   #$00
   LDY   #$14
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   LDY   #$14
   LDA   (FP),Y
   LDY   #$02
   STA   (FP),Y
   JMP   .loop@1_start
.loop@1_break:

   ; epilogue
   CLC
   LDA   FP
   ADC   #$18
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$15
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   RTS

test_two_int_struct:
   SUBROUTINE

   ; prologue: 0 arg bytes, 65 local bytes
   SEC
   LDA   SSP
   SBC   #$43
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$42
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
   LDA   #$65
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$11
   LDA   (FP),Y
   CLC
   ADC   #$01
   LDY   #$13
   STA   (FP),Y
   DEY
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
   LDA   #$66
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$11
   LDA   (FP),Y
   CLC
   ADC   #$02
   LDY   #$15
   STA   (FP),Y
   LDY   #$12
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
   LDA   #$67
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$11
   LDA   (FP),Y
   CLC
   ADC   #$03
   LDY   #$17
   STA   (FP),Y
   LDY   #$12
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
   LDA   #$68
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$11
   LDA   (FP),Y
   CLC
   ADC   #$04
   LDY   #$19
   STA   (FP),Y
   LDY   #$12
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
   LDA   #$69
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$11
   LDA   (FP),Y
   CLC
   ADC   #$05
   LDY   #$1B
   STA   (FP),Y
   LDY   #$12
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
   LDA   #$6A
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$11
   LDA   (FP),Y
   CLC
   ADC   #$06
   LDY   #$1D
   STA   (FP),Y
   LDY   #$12
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
   LDA   #$6B
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$11
   LDA   (FP),Y
   CLC
   ADC   #$07
   LDY   #$1F
   STA   (FP),Y
   LDY   #$12
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
   LDA   #$6C
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$11
   LDA   (FP),Y
   CLC
   ADC   #$08
   LDY   #$21
   STA   (FP),Y
   LDY   #$12
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
   LDA   #$6D
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$11
   LDA   (FP),Y
   CLC
   ADC   #$09
   LDY   #$23
   STA   (FP),Y
   LDY   #$12
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
   LDA   #$6E
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$11
   LDA   (FP),Y
   CLC
   ADC   #$0A
   LDY   #$25
   STA   (FP),Y
   LDY   #$12
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
   LDA   #$6F
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$11
   LDA   (FP),Y
   CLC
   ADC   #$0B
   LDY   #$27
   STA   (FP),Y
   LDY   #$12
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
   LDA   #$70
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$11
   LDA   (FP),Y
   CLC
   ADC   #$0C
   LDY   #$29
   STA   (FP),Y
   LDY   #$12
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
   LDA   #$71
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$11
   LDA   (FP),Y
   CLC
   ADC   #$0D
   LDY   #$2B
   STA   (FP),Y
   LDY   #$12
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
   LDA   #$72
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$11
   LDA   (FP),Y
   CLC
   ADC   #$0E
   LDY   #$2D
   STA   (FP),Y
   LDY   #$12
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
   LDA   #$73
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$11
   LDA   (FP),Y
   CLC
   ADC   #$0F
   LDY   #$2F
   STA   (FP),Y
   LDY   #$12
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
   LDA   #$00
   LDY   #$00
   STA   (DPTR),Y
   CLC
   LDA   FP
   ADC   #$31
   LDY   #$40
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
   LDY   #$41
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   JSR   return_two_int_struct
   LDY   #$31
   LDA   (FP),Y
   STA   two_int_struct
   INY
   LDA   (FP),Y
   STA   two_int_struct+1
   INY
   LDA   (FP),Y
   STA   two_int_struct+2
   INY
   LDA   (FP),Y
   STA   two_int_struct+3
   INY
   LDA   (FP),Y
   STA   two_int_struct+4
   INY
   LDA   (FP),Y
   STA   two_int_struct+5
   INY
   LDA   (FP),Y
   STA   two_int_struct+6
   INY
   LDA   (FP),Y
   STA   two_int_struct+7
   INY
   LDA   (FP),Y
   STA   two_int_struct+8
   INY
   LDA   (FP),Y
   STA   two_int_struct+9
   INY
   LDA   (FP),Y
   STA   two_int_struct+10
   INY
   LDA   (FP),Y
   STA   two_int_struct+11
   INY
   LDA   (FP),Y
   STA   two_int_struct+12
   INY
   LDA   (FP),Y
   STA   two_int_struct+13
   INY
   LDA   (FP),Y
   STA   two_int_struct+14
   LDY   #$01
   LDA   (FP),Y
   STA   to_validate
   INY
   LDA   (FP),Y
   STA   to_validate+1
   INY
   LDA   (FP),Y
   STA   to_validate+2
   INY
   LDA   (FP),Y
   STA   to_validate+3
   INY
   LDA   (FP),Y
   STA   to_validate+4
   INY
   LDA   (FP),Y
   STA   to_validate+5
   INY
   LDA   (FP),Y
   STA   to_validate+6
   INY
   LDA   (FP),Y
   STA   to_validate+7
   INY
   LDA   (FP),Y
   STA   to_validate+8
   INY
   LDA   (FP),Y
   STA   to_validate+9
   INY
   LDA   (FP),Y
   STA   to_validate+10
   INY
   LDA   (FP),Y
   STA   to_validate+11
   INY
   LDA   (FP),Y
   STA   to_validate+12
   INY
   LDA   (FP),Y
   STA   to_validate+13
   INY
   LDA   (FP),Y
   STA   to_validate+14
   INY
   LDA   (FP),Y
   STA   to_validate+15
   SEC
   LDA   SSP
   SBC   #$02
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   #$03
   LDY   #$01
   STA   (SSP),Y
   LDA   #$00
   INY
   STA   (SSP),Y
   JSR   validate_stack_bytes
   SEC
   LDA   SSP
   SBC   #$02
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   #$04
   LDY   #$01
   STA   (SSP),Y
   LDA   #$00
   INY
   STA   (SSP),Y
   JSR   validate_two_int_struct
   LDX   #$00
   TXA

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$43
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$42
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

return_one_xmm_struct:
   SUBROUTINE

   ; prologue: 2 arg bytes, 10 local bytes
   SEC
   LDA   SSP
   SBC   #$0C
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$0B
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
   LDY   #$09
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
   LDA   #$50
   INY
   STA   (DPTR),Y
   LDA   #$6D
   INY
   STA   (DPTR),Y
   LDA   #$40
   INY
   STA   (DPTR),Y
   LDY   #$0D
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

   ; epilogue
   CLC
   LDA   FP
   ADC   #$0E
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$0B
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   RTS

validate_one_double_struct:
   SUBROUTINE

   ; prologue: 2 arg bytes, 12 local bytes
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

   LDA   #<one_double_struct
   LDY   #$01
   STA   (FP),Y
   LDA   #>one_double_struct
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
   LDY   #$03
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$04
   STA   (FP),Y
   LDY   #$02
   LDA   (DPTR),Y
   LDY   #$05
   STA   (FP),Y
   LDY   #$03
   LDA   (DPTR),Y
   LDY   #$06
   STA   (FP),Y
   LDY   #$04
   LDA   (DPTR),Y
   LDY   #$07
   STA   (FP),Y
   LDY   #$05
   LDA   (DPTR),Y
   LDY   #$08
   STA   (FP),Y
   LDY   #$06
   LDA   (DPTR),Y
   LDY   #$09
   STA   (FP),Y
   LDY   #$07
   LDA   (DPTR),Y
   LDY   #$0A
   STA   (FP),Y
   CMP   #$40
   BNE   .cmp_true@5
   DEY
   LDA   (FP),Y
   CMP   #$6D
   BNE   .cmp_true@5
   DEY
   LDA   (FP),Y
   CMP   #$50
   BNE   .cmp_true@5
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@5
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@5
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@5
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@5
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@5
   LDA   #$00
   JMP   .cmp_end@6
.cmp_true@5:
   LDA   #$01
.cmp_end@6:
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
   SEC
   LDA   SSP
   SBC   #$02
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
   JSR   exit
.if_end@3:

   ; epilogue
   CLC
   LDA   FP
   ADC   #$10
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
   RTS

test_one_double_struct:
   SUBROUTINE

   ; prologue: 0 arg bytes, 58 local bytes
   SEC
   LDA   SSP
   SBC   #$3C
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$3B
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
   LDA   #$65
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$11
   LDA   (FP),Y
   CLC
   ADC   #$01
   LDY   #$13
   STA   (FP),Y
   DEY
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
   LDA   #$66
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$11
   LDA   (FP),Y
   CLC
   ADC   #$02
   LDY   #$15
   STA   (FP),Y
   LDY   #$12
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
   LDA   #$67
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$11
   LDA   (FP),Y
   CLC
   ADC   #$03
   LDY   #$17
   STA   (FP),Y
   LDY   #$12
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
   LDA   #$68
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$11
   LDA   (FP),Y
   CLC
   ADC   #$04
   LDY   #$19
   STA   (FP),Y
   LDY   #$12
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
   LDA   #$69
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$11
   LDA   (FP),Y
   CLC
   ADC   #$05
   LDY   #$1B
   STA   (FP),Y
   LDY   #$12
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
   LDA   #$6A
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$11
   LDA   (FP),Y
   CLC
   ADC   #$06
   LDY   #$1D
   STA   (FP),Y
   LDY   #$12
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
   LDA   #$6B
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$11
   LDA   (FP),Y
   CLC
   ADC   #$07
   LDY   #$1F
   STA   (FP),Y
   LDY   #$12
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
   LDA   #$6C
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$11
   LDA   (FP),Y
   CLC
   ADC   #$08
   LDY   #$21
   STA   (FP),Y
   LDY   #$12
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
   LDA   #$6D
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$11
   LDA   (FP),Y
   CLC
   ADC   #$09
   LDY   #$23
   STA   (FP),Y
   LDY   #$12
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
   LDA   #$6E
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$11
   LDA   (FP),Y
   CLC
   ADC   #$0A
   LDY   #$25
   STA   (FP),Y
   LDY   #$12
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
   LDA   #$6F
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$11
   LDA   (FP),Y
   CLC
   ADC   #$0B
   LDY   #$27
   STA   (FP),Y
   LDY   #$12
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
   LDA   #$70
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$11
   LDA   (FP),Y
   CLC
   ADC   #$0C
   LDY   #$29
   STA   (FP),Y
   LDY   #$12
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
   LDA   #$71
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$11
   LDA   (FP),Y
   CLC
   ADC   #$0D
   LDY   #$2B
   STA   (FP),Y
   LDY   #$12
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
   LDA   #$72
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$11
   LDA   (FP),Y
   CLC
   ADC   #$0E
   LDY   #$2D
   STA   (FP),Y
   LDY   #$12
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
   LDA   #$73
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$11
   LDA   (FP),Y
   CLC
   ADC   #$0F
   LDY   #$2F
   STA   (FP),Y
   LDY   #$12
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
   LDA   #$00
   LDY   #$00
   STA   (DPTR),Y
   CLC
   LDA   FP
   ADC   #$31
   LDY   #$39
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
   LDY   #$3A
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   JSR   return_one_xmm_struct
   LDY   #$31
   LDA   (FP),Y
   STA   one_double_struct
   INY
   LDA   (FP),Y
   STA   one_double_struct+1
   INY
   LDA   (FP),Y
   STA   one_double_struct+2
   INY
   LDA   (FP),Y
   STA   one_double_struct+3
   INY
   LDA   (FP),Y
   STA   one_double_struct+4
   INY
   LDA   (FP),Y
   STA   one_double_struct+5
   INY
   LDA   (FP),Y
   STA   one_double_struct+6
   INY
   LDA   (FP),Y
   STA   one_double_struct+7
   LDY   #$01
   LDA   (FP),Y
   STA   to_validate
   INY
   LDA   (FP),Y
   STA   to_validate+1
   INY
   LDA   (FP),Y
   STA   to_validate+2
   INY
   LDA   (FP),Y
   STA   to_validate+3
   INY
   LDA   (FP),Y
   STA   to_validate+4
   INY
   LDA   (FP),Y
   STA   to_validate+5
   INY
   LDA   (FP),Y
   STA   to_validate+6
   INY
   LDA   (FP),Y
   STA   to_validate+7
   INY
   LDA   (FP),Y
   STA   to_validate+8
   INY
   LDA   (FP),Y
   STA   to_validate+9
   INY
   LDA   (FP),Y
   STA   to_validate+10
   INY
   LDA   (FP),Y
   STA   to_validate+11
   INY
   LDA   (FP),Y
   STA   to_validate+12
   INY
   LDA   (FP),Y
   STA   to_validate+13
   INY
   LDA   (FP),Y
   STA   to_validate+14
   INY
   LDA   (FP),Y
   STA   to_validate+15
   SEC
   LDA   SSP
   SBC   #$02
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   #$05
   LDY   #$01
   STA   (SSP),Y
   LDA   #$00
   INY
   STA   (SSP),Y
   JSR   validate_stack_bytes
   SEC
   LDA   SSP
   SBC   #$02
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   #$06
   LDY   #$01
   STA   (SSP),Y
   LDA   #$00
   INY
   STA   (SSP),Y
   JSR   validate_one_double_struct
   LDX   #$00
   TXA

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$3C
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$3B
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

return_two_xmm_struct:
   SUBROUTINE

   ; prologue: 2 arg bytes, 20 local bytes
   SEC
   LDA   SSP
   SBC   #$16
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$15
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
   LDA   #$50
   INY
   STA   (DPTR),Y
   LDA   #$6D
   INY
   STA   (DPTR),Y
   LDA   #$40
   INY
   STA   (DPTR),Y
   LDY   #$11
   LDA   (FP),Y
   CLC
   ADC   #$08
   LDY   #$13
   STA   (FP),Y
   DEY
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
   LDA   #$32
   INY
   STA   (DPTR),Y
   LDA   #$85
   INY
   STA   (DPTR),Y
   LDA   #$40
   INY
   STA   (DPTR),Y
   LDY   #$17
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
   LDY   #$0F
   LDA   (FP),Y
   DEY
   STA   (DPTR),Y
   LDY   #$10
   LDA   (FP),Y
   DEY
   STA   (DPTR),Y

   ; epilogue
   CLC
   LDA   FP
   ADC   #$18
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$15
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   RTS

validate_two_doubles_struct:
   SUBROUTINE

   ; prologue: 2 arg bytes, 28 local bytes
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

   LDA   #<two_doubles_struct
   LDY   #$01
   STA   (FP),Y
   LDA   #>two_doubles_struct
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
   LDY   #$03
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$04
   STA   (FP),Y
   LDY   #$02
   LDA   (DPTR),Y
   LDY   #$05
   STA   (FP),Y
   LDY   #$03
   LDA   (DPTR),Y
   LDY   #$06
   STA   (FP),Y
   LDY   #$04
   LDA   (DPTR),Y
   LDY   #$07
   STA   (FP),Y
   LDY   #$05
   LDA   (DPTR),Y
   LDY   #$08
   STA   (FP),Y
   LDY   #$06
   LDA   (DPTR),Y
   LDY   #$09
   STA   (FP),Y
   LDY   #$07
   LDA   (DPTR),Y
   LDY   #$0A
   STA   (FP),Y
   CMP   #$40
   BNE   .cmp_true@8
   DEY
   LDA   (FP),Y
   CMP   #$6D
   BNE   .cmp_true@8
   DEY
   LDA   (FP),Y
   CMP   #$50
   BNE   .cmp_true@8
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@8
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@8
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@8
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@8
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@8
   LDA   #$00
   JMP   .cmp_end@9
.cmp_true@8:
   LDA   #$01
.cmp_end@9:
   LDY   #$0B
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@2
   JMP   .or_true@4
.lb_skip@2:
   LDA   #<two_doubles_struct
   LDY   #$0D
   STA   (FP),Y
   LDA   #>two_doubles_struct
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$08
   LDY   #$0F
   STA   (FP),Y
   DEY
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
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$11
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$12
   STA   (FP),Y
   LDY   #$02
   LDA   (DPTR),Y
   LDY   #$13
   STA   (FP),Y
   LDY   #$03
   LDA   (DPTR),Y
   LDY   #$14
   STA   (FP),Y
   LDY   #$04
   LDA   (DPTR),Y
   LDY   #$15
   STA   (FP),Y
   LDY   #$05
   LDA   (DPTR),Y
   LDY   #$16
   STA   (FP),Y
   LDY   #$06
   LDA   (DPTR),Y
   LDY   #$17
   STA   (FP),Y
   LDY   #$07
   LDA   (DPTR),Y
   LDY   #$18
   STA   (FP),Y
   CMP   #$40
   BNE   .cmp_true@11
   DEY
   LDA   (FP),Y
   CMP   #$85
   BNE   .cmp_true@11
   DEY
   LDA   (FP),Y
   CMP   #$32
   BNE   .cmp_true@11
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@11
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@11
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@11
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@11
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@11
   LDA   #$00
   JMP   .cmp_end@12
.cmp_true@11:
   LDA   #$01
.cmp_end@12:
   LDY   #$19
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
   LDY   #$1B
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@5:
   LDY   #$1B
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@6
   SEC
   LDA   SSP
   SBC   #$02
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDY   #$1F
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$20
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   JSR   exit
.if_end@6:

   ; epilogue
   CLC
   LDA   FP
   ADC   #$20
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
   RTS

test_two_doubles_struct:
   SUBROUTINE

   ; prologue: 0 arg bytes, 66 local bytes
   SEC
   LDA   SSP
   SBC   #$44
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$43
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
   LDA   #$65
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$11
   LDA   (FP),Y
   CLC
   ADC   #$01
   LDY   #$13
   STA   (FP),Y
   DEY
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
   LDA   #$66
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$11
   LDA   (FP),Y
   CLC
   ADC   #$02
   LDY   #$15
   STA   (FP),Y
   LDY   #$12
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
   LDA   #$67
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$11
   LDA   (FP),Y
   CLC
   ADC   #$03
   LDY   #$17
   STA   (FP),Y
   LDY   #$12
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
   LDA   #$68
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$11
   LDA   (FP),Y
   CLC
   ADC   #$04
   LDY   #$19
   STA   (FP),Y
   LDY   #$12
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
   LDA   #$69
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$11
   LDA   (FP),Y
   CLC
   ADC   #$05
   LDY   #$1B
   STA   (FP),Y
   LDY   #$12
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
   LDA   #$6A
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$11
   LDA   (FP),Y
   CLC
   ADC   #$06
   LDY   #$1D
   STA   (FP),Y
   LDY   #$12
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
   LDA   #$6B
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$11
   LDA   (FP),Y
   CLC
   ADC   #$07
   LDY   #$1F
   STA   (FP),Y
   LDY   #$12
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
   LDA   #$6C
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$11
   LDA   (FP),Y
   CLC
   ADC   #$08
   LDY   #$21
   STA   (FP),Y
   LDY   #$12
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
   LDA   #$6D
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$11
   LDA   (FP),Y
   CLC
   ADC   #$09
   LDY   #$23
   STA   (FP),Y
   LDY   #$12
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
   LDA   #$6E
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$11
   LDA   (FP),Y
   CLC
   ADC   #$0A
   LDY   #$25
   STA   (FP),Y
   LDY   #$12
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
   LDA   #$6F
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$11
   LDA   (FP),Y
   CLC
   ADC   #$0B
   LDY   #$27
   STA   (FP),Y
   LDY   #$12
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
   LDA   #$70
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$11
   LDA   (FP),Y
   CLC
   ADC   #$0C
   LDY   #$29
   STA   (FP),Y
   LDY   #$12
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
   LDA   #$71
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$11
   LDA   (FP),Y
   CLC
   ADC   #$0D
   LDY   #$2B
   STA   (FP),Y
   LDY   #$12
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
   LDA   #$72
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$11
   LDA   (FP),Y
   CLC
   ADC   #$0E
   LDY   #$2D
   STA   (FP),Y
   LDY   #$12
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
   LDA   #$73
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$11
   LDA   (FP),Y
   CLC
   ADC   #$0F
   LDY   #$2F
   STA   (FP),Y
   LDY   #$12
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
   LDA   #$00
   LDY   #$00
   STA   (DPTR),Y
   CLC
   LDA   FP
   ADC   #$31
   LDY   #$41
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
   LDY   #$42
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   JSR   return_two_xmm_struct
   LDY   #$31
   LDA   (FP),Y
   STA   two_doubles_struct
   INY
   LDA   (FP),Y
   STA   two_doubles_struct+1
   INY
   LDA   (FP),Y
   STA   two_doubles_struct+2
   INY
   LDA   (FP),Y
   STA   two_doubles_struct+3
   INY
   LDA   (FP),Y
   STA   two_doubles_struct+4
   INY
   LDA   (FP),Y
   STA   two_doubles_struct+5
   INY
   LDA   (FP),Y
   STA   two_doubles_struct+6
   INY
   LDA   (FP),Y
   STA   two_doubles_struct+7
   INY
   LDA   (FP),Y
   STA   two_doubles_struct+8
   INY
   LDA   (FP),Y
   STA   two_doubles_struct+9
   INY
   LDA   (FP),Y
   STA   two_doubles_struct+10
   INY
   LDA   (FP),Y
   STA   two_doubles_struct+11
   INY
   LDA   (FP),Y
   STA   two_doubles_struct+12
   INY
   LDA   (FP),Y
   STA   two_doubles_struct+13
   INY
   LDA   (FP),Y
   STA   two_doubles_struct+14
   INY
   LDA   (FP),Y
   STA   two_doubles_struct+15
   LDY   #$01
   LDA   (FP),Y
   STA   to_validate
   INY
   LDA   (FP),Y
   STA   to_validate+1
   INY
   LDA   (FP),Y
   STA   to_validate+2
   INY
   LDA   (FP),Y
   STA   to_validate+3
   INY
   LDA   (FP),Y
   STA   to_validate+4
   INY
   LDA   (FP),Y
   STA   to_validate+5
   INY
   LDA   (FP),Y
   STA   to_validate+6
   INY
   LDA   (FP),Y
   STA   to_validate+7
   INY
   LDA   (FP),Y
   STA   to_validate+8
   INY
   LDA   (FP),Y
   STA   to_validate+9
   INY
   LDA   (FP),Y
   STA   to_validate+10
   INY
   LDA   (FP),Y
   STA   to_validate+11
   INY
   LDA   (FP),Y
   STA   to_validate+12
   INY
   LDA   (FP),Y
   STA   to_validate+13
   INY
   LDA   (FP),Y
   STA   to_validate+14
   INY
   LDA   (FP),Y
   STA   to_validate+15
   SEC
   LDA   SSP
   SBC   #$02
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   #$07
   LDY   #$01
   STA   (SSP),Y
   LDA   #$00
   INY
   STA   (SSP),Y
   JSR   validate_stack_bytes
   SEC
   LDA   SSP
   SBC   #$02
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   #$08
   LDY   #$01
   STA   (SSP),Y
   LDA   #$00
   INY
   STA   (SSP),Y
   JSR   validate_two_doubles_struct
   LDX   #$00
   TXA

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$44
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$43
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

return_mixed_struct:
   SUBROUTINE

   ; prologue: 2 arg bytes, 14 local bytes
   SEC
   LDA   SSP
   SBC   #$10
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$0F
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   LDA   #$7D
   LDY   #$01
   STA   (FP),Y
   CLC
   LDA   FP
   ADC   #$02
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
   LDY   #$01
   LDA   (FP),Y
   DEY
   STA   (DPTR),Y
   LDY   #$0B
   LDA   (FP),Y
   CLC
   ADC   #$01
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
   LDA   #$32
   INY
   STA   (DPTR),Y
   LDA   #$85
   INY
   STA   (DPTR),Y
   LDA   #$40
   INY
   STA   (DPTR),Y
   LDY   #$11
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$02
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$03
   LDA   (FP),Y
   LDY   #$01
   STA   (DPTR),Y
   LDY   #$04
   LDA   (FP),Y
   LDY   #$02
   STA   (DPTR),Y
   LDY   #$05
   LDA   (FP),Y
   LDY   #$03
   STA   (DPTR),Y
   LDY   #$06
   LDA   (FP),Y
   LDY   #$04
   STA   (DPTR),Y
   LDY   #$07
   LDA   (FP),Y
   LDY   #$05
   STA   (DPTR),Y
   LDY   #$08
   LDA   (FP),Y
   LDY   #$06
   STA   (DPTR),Y
   LDY   #$09
   LDA   (FP),Y
   LDY   #$07
   STA   (DPTR),Y
   LDY   #$0A
   LDA   (FP),Y
   LDY   #$08
   STA   (DPTR),Y

   ; epilogue
   CLC
   LDA   FP
   ADC   #$12
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$0F
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   RTS

validate_mixed_struct:
   SUBROUTINE

   ; prologue: 2 arg bytes, 23 local bytes
   SEC
   LDA   SSP
   SBC   #$19
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$18
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   LDA   #<mixed_struct
   LDY   #$01
   STA   (FP),Y
   LDA   #>mixed_struct
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
   LDY   #$03
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CMP   #$7D
   BNE   .cmp_true@14
   LDA   #$00
   JMP   .cmp_end@15
.cmp_true@14:
   LDA   #$01
.cmp_end@15:
   LDY   #$06
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@3
   JMP   .or_true@7
.lb_skip@3:
   LDA   #<mixed_struct
   LDY   #$08
   STA   (FP),Y
   LDA   #>mixed_struct
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$01
   LDY   #$0A
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$0B
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$0C
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$0D
   STA   (FP),Y
   LDY   #$02
   LDA   (DPTR),Y
   LDY   #$0E
   STA   (FP),Y
   LDY   #$03
   LDA   (DPTR),Y
   LDY   #$0F
   STA   (FP),Y
   LDY   #$04
   LDA   (DPTR),Y
   LDY   #$10
   STA   (FP),Y
   LDY   #$05
   LDA   (DPTR),Y
   LDY   #$11
   STA   (FP),Y
   LDY   #$06
   LDA   (DPTR),Y
   LDY   #$12
   STA   (FP),Y
   LDY   #$07
   LDA   (DPTR),Y
   LDY   #$13
   STA   (FP),Y
   CMP   #$40
   BNE   .cmp_true@16
   DEY
   LDA   (FP),Y
   CMP   #$85
   BNE   .cmp_true@16
   DEY
   LDA   (FP),Y
   CMP   #$32
   BNE   .cmp_true@16
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@16
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@16
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@16
   DEY
   LDA   (FP),Y
   CMP   #$00
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
   LDY   #$14
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@7
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   JMP   .or_end@8
.or_true@7:
   LDA   #$01
   LDY   #$16
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@8:
   LDY   #$16
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@9
   SEC
   LDA   SSP
   SBC   #$02
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDY   #$1A
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$1B
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   JSR   exit
.if_end@9:

   ; epilogue
   CLC
   LDA   FP
   ADC   #$1B
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$18
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   RTS

test_mixed_struct:
   SUBROUTINE

   ; prologue: 0 arg bytes, 59 local bytes
   SEC
   LDA   SSP
   SBC   #$3D
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$3C
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
   LDA   #$65
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$11
   LDA   (FP),Y
   CLC
   ADC   #$01
   LDY   #$13
   STA   (FP),Y
   DEY
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
   LDA   #$66
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$11
   LDA   (FP),Y
   CLC
   ADC   #$02
   LDY   #$15
   STA   (FP),Y
   LDY   #$12
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
   LDA   #$67
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$11
   LDA   (FP),Y
   CLC
   ADC   #$03
   LDY   #$17
   STA   (FP),Y
   LDY   #$12
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
   LDA   #$68
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$11
   LDA   (FP),Y
   CLC
   ADC   #$04
   LDY   #$19
   STA   (FP),Y
   LDY   #$12
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
   LDA   #$69
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$11
   LDA   (FP),Y
   CLC
   ADC   #$05
   LDY   #$1B
   STA   (FP),Y
   LDY   #$12
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
   LDA   #$6A
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$11
   LDA   (FP),Y
   CLC
   ADC   #$06
   LDY   #$1D
   STA   (FP),Y
   LDY   #$12
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
   LDA   #$6B
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$11
   LDA   (FP),Y
   CLC
   ADC   #$07
   LDY   #$1F
   STA   (FP),Y
   LDY   #$12
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
   LDA   #$6C
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$11
   LDA   (FP),Y
   CLC
   ADC   #$08
   LDY   #$21
   STA   (FP),Y
   LDY   #$12
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
   LDA   #$6D
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$11
   LDA   (FP),Y
   CLC
   ADC   #$09
   LDY   #$23
   STA   (FP),Y
   LDY   #$12
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
   LDA   #$6E
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$11
   LDA   (FP),Y
   CLC
   ADC   #$0A
   LDY   #$25
   STA   (FP),Y
   LDY   #$12
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
   LDA   #$6F
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$11
   LDA   (FP),Y
   CLC
   ADC   #$0B
   LDY   #$27
   STA   (FP),Y
   LDY   #$12
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
   LDA   #$70
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$11
   LDA   (FP),Y
   CLC
   ADC   #$0C
   LDY   #$29
   STA   (FP),Y
   LDY   #$12
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
   LDA   #$71
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$11
   LDA   (FP),Y
   CLC
   ADC   #$0D
   LDY   #$2B
   STA   (FP),Y
   LDY   #$12
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
   LDA   #$72
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$11
   LDA   (FP),Y
   CLC
   ADC   #$0E
   LDY   #$2D
   STA   (FP),Y
   LDY   #$12
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
   LDA   #$73
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$11
   LDA   (FP),Y
   CLC
   ADC   #$0F
   LDY   #$2F
   STA   (FP),Y
   LDY   #$12
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
   LDA   #$00
   LDY   #$00
   STA   (DPTR),Y
   CLC
   LDA   FP
   ADC   #$31
   LDY   #$3A
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
   LDY   #$3B
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   JSR   return_mixed_struct
   LDY   #$31
   LDA   (FP),Y
   STA   mixed_struct
   INY
   LDA   (FP),Y
   STA   mixed_struct+1
   INY
   LDA   (FP),Y
   STA   mixed_struct+2
   INY
   LDA   (FP),Y
   STA   mixed_struct+3
   INY
   LDA   (FP),Y
   STA   mixed_struct+4
   INY
   LDA   (FP),Y
   STA   mixed_struct+5
   INY
   LDA   (FP),Y
   STA   mixed_struct+6
   INY
   LDA   (FP),Y
   STA   mixed_struct+7
   INY
   LDA   (FP),Y
   STA   mixed_struct+8
   LDY   #$01
   LDA   (FP),Y
   STA   to_validate
   INY
   LDA   (FP),Y
   STA   to_validate+1
   INY
   LDA   (FP),Y
   STA   to_validate+2
   INY
   LDA   (FP),Y
   STA   to_validate+3
   INY
   LDA   (FP),Y
   STA   to_validate+4
   INY
   LDA   (FP),Y
   STA   to_validate+5
   INY
   LDA   (FP),Y
   STA   to_validate+6
   INY
   LDA   (FP),Y
   STA   to_validate+7
   INY
   LDA   (FP),Y
   STA   to_validate+8
   INY
   LDA   (FP),Y
   STA   to_validate+9
   INY
   LDA   (FP),Y
   STA   to_validate+10
   INY
   LDA   (FP),Y
   STA   to_validate+11
   INY
   LDA   (FP),Y
   STA   to_validate+12
   INY
   LDA   (FP),Y
   STA   to_validate+13
   INY
   LDA   (FP),Y
   STA   to_validate+14
   INY
   LDA   (FP),Y
   STA   to_validate+15
   SEC
   LDA   SSP
   SBC   #$02
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   #$09
   LDY   #$01
   STA   (SSP),Y
   LDA   #$00
   INY
   STA   (SSP),Y
   JSR   validate_stack_bytes
   SEC
   LDA   SSP
   SBC   #$02
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   #$0A
   LDY   #$01
   STA   (SSP),Y
   LDA   #$00
   INY
   STA   (SSP),Y
   JSR   validate_mixed_struct
   LDX   #$00
   TXA

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$3D
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$3C
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

return_stack_struct:
   SUBROUTINE

   ; prologue: 2 arg bytes, 112 local bytes
   SEC
   LDA   SSP
   SBC   #$72
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$71
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   LDA   #$5A
   LDY   #$01
   STA   (FP),Y
   CLC
   LDA   FP
   ADC   #$02
   LDY   #$1E
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
   LDA   #$5B
   LDY   #$20
   STA   (FP),Y
   LDY   #$1E
   LDA   (FP),Y
   CLC
   ADC   #$01
   LDY   #$21
   STA   (FP),Y
   LDY   #$1F
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
   LDY   #$20
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDA   #$5C
   LDY   #$23
   STA   (FP),Y
   LDY   #$1E
   LDA   (FP),Y
   CLC
   ADC   #$02
   LDY   #$24
   STA   (FP),Y
   LDY   #$1F
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
   LDY   #$23
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDA   #$5D
   LDY   #$26
   STA   (FP),Y
   LDY   #$1E
   LDA   (FP),Y
   CLC
   ADC   #$03
   LDY   #$27
   STA   (FP),Y
   LDY   #$1F
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
   LDA   #$5E
   LDY   #$29
   STA   (FP),Y
   LDY   #$1E
   LDA   (FP),Y
   CLC
   ADC   #$04
   LDY   #$2A
   STA   (FP),Y
   LDY   #$1F
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
   LDY   #$29
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDA   #$5F
   LDY   #$2C
   STA   (FP),Y
   LDY   #$1E
   LDA   (FP),Y
   CLC
   ADC   #$05
   LDY   #$2D
   STA   (FP),Y
   LDY   #$1F
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
   LDY   #$2C
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDA   #$60
   LDY   #$2F
   STA   (FP),Y
   LDY   #$1E
   LDA   (FP),Y
   CLC
   ADC   #$06
   LDY   #$30
   STA   (FP),Y
   LDY   #$1F
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
   LDY   #$2F
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDA   #$61
   LDY   #$32
   STA   (FP),Y
   LDY   #$1E
   LDA   (FP),Y
   CLC
   ADC   #$07
   LDY   #$33
   STA   (FP),Y
   LDY   #$1F
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
   LDY   #$32
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDA   #$62
   LDY   #$35
   STA   (FP),Y
   LDY   #$1E
   LDA   (FP),Y
   CLC
   ADC   #$08
   LDY   #$36
   STA   (FP),Y
   LDY   #$1F
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
   LDY   #$35
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDA   #$63
   LDY   #$38
   STA   (FP),Y
   LDY   #$1E
   LDA   (FP),Y
   CLC
   ADC   #$09
   LDY   #$39
   STA   (FP),Y
   LDY   #$1F
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
   LDY   #$38
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDA   #$64
   LDY   #$3B
   STA   (FP),Y
   LDY   #$1E
   LDA   (FP),Y
   CLC
   ADC   #$0A
   LDY   #$3C
   STA   (FP),Y
   LDY   #$1F
   LDA   (FP),Y
   ADC   #$00
   LDY   #$3D
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$3B
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDA   #$65
   LDY   #$3E
   STA   (FP),Y
   LDY   #$1E
   LDA   (FP),Y
   CLC
   ADC   #$0B
   LDY   #$3F
   STA   (FP),Y
   LDY   #$1F
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
   LDY   #$3E
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDA   #$66
   LDY   #$41
   STA   (FP),Y
   LDY   #$1E
   LDA   (FP),Y
   CLC
   ADC   #$0C
   LDY   #$42
   STA   (FP),Y
   LDY   #$1F
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
   LDY   #$41
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDA   #$67
   LDY   #$44
   STA   (FP),Y
   LDY   #$1E
   LDA   (FP),Y
   CLC
   ADC   #$0D
   LDY   #$45
   STA   (FP),Y
   LDY   #$1F
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
   LDY   #$44
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDA   #$68
   LDY   #$47
   STA   (FP),Y
   LDY   #$1E
   LDA   (FP),Y
   CLC
   ADC   #$0E
   LDY   #$48
   STA   (FP),Y
   LDY   #$1F
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
   LDY   #$47
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDA   #$69
   LDY   #$4A
   STA   (FP),Y
   LDY   #$1E
   LDA   (FP),Y
   CLC
   ADC   #$0F
   LDY   #$4B
   STA   (FP),Y
   LDY   #$1F
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
   LDY   #$4A
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDA   #$6A
   LDY   #$4D
   STA   (FP),Y
   LDY   #$1E
   LDA   (FP),Y
   CLC
   ADC   #$10
   LDY   #$4E
   STA   (FP),Y
   LDY   #$1F
   LDA   (FP),Y
   ADC   #$00
   LDY   #$4F
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$4D
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDA   #$6B
   LDY   #$50
   STA   (FP),Y
   LDY   #$1E
   LDA   (FP),Y
   CLC
   ADC   #$11
   LDY   #$51
   STA   (FP),Y
   LDY   #$1F
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
   LDY   #$50
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDA   #$6C
   LDY   #$53
   STA   (FP),Y
   LDY   #$1E
   LDA   (FP),Y
   CLC
   ADC   #$12
   LDY   #$54
   STA   (FP),Y
   LDY   #$1F
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
   LDY   #$53
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDA   #$6D
   LDY   #$56
   STA   (FP),Y
   LDY   #$1E
   LDA   (FP),Y
   CLC
   ADC   #$13
   LDY   #$57
   STA   (FP),Y
   LDY   #$1F
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
   LDY   #$56
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDA   #$6E
   LDY   #$59
   STA   (FP),Y
   LDY   #$1E
   LDA   (FP),Y
   CLC
   ADC   #$14
   LDY   #$5A
   STA   (FP),Y
   LDY   #$1F
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
   LDY   #$59
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDA   #$6F
   LDY   #$5C
   STA   (FP),Y
   LDY   #$1E
   LDA   (FP),Y
   CLC
   ADC   #$15
   LDY   #$5D
   STA   (FP),Y
   LDY   #$1F
   LDA   (FP),Y
   ADC   #$00
   LDY   #$5E
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$5C
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDA   #$70
   LDY   #$5F
   STA   (FP),Y
   LDY   #$1E
   LDA   (FP),Y
   CLC
   ADC   #$16
   LDY   #$60
   STA   (FP),Y
   LDY   #$1F
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
   LDY   #$5F
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDA   #$71
   LDY   #$62
   STA   (FP),Y
   LDY   #$1E
   LDA   (FP),Y
   CLC
   ADC   #$17
   LDY   #$63
   STA   (FP),Y
   LDY   #$1F
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
   LDY   #$62
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDA   #$72
   LDY   #$65
   STA   (FP),Y
   LDY   #$1E
   LDA   (FP),Y
   CLC
   ADC   #$18
   LDY   #$66
   STA   (FP),Y
   LDY   #$1F
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
   LDY   #$65
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDA   #$73
   LDY   #$68
   STA   (FP),Y
   LDY   #$1E
   LDA   (FP),Y
   CLC
   ADC   #$19
   LDY   #$69
   STA   (FP),Y
   LDY   #$1F
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
   LDY   #$68
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDA   #$74
   LDY   #$6B
   STA   (FP),Y
   LDY   #$1E
   LDA   (FP),Y
   CLC
   ADC   #$1A
   LDY   #$6C
   STA   (FP),Y
   LDY   #$1F
   LDA   (FP),Y
   ADC   #$00
   LDY   #$6D
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
   LDA   #$75
   LDY   #$6E
   STA   (FP),Y
   LDY   #$1E
   LDA   (FP),Y
   CLC
   ADC   #$1B
   LDY   #$6F
   STA   (FP),Y
   LDY   #$1F
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
   LDY   #$6E
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$73
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$02
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$03
   LDA   (FP),Y
   LDY   #$01
   STA   (DPTR),Y
   LDY   #$04
   LDA   (FP),Y
   LDY   #$02
   STA   (DPTR),Y
   LDY   #$05
   LDA   (FP),Y
   LDY   #$03
   STA   (DPTR),Y
   LDY   #$06
   LDA   (FP),Y
   LDY   #$04
   STA   (DPTR),Y
   LDY   #$07
   LDA   (FP),Y
   LDY   #$05
   STA   (DPTR),Y
   LDY   #$08
   LDA   (FP),Y
   LDY   #$06
   STA   (DPTR),Y
   LDY   #$09
   LDA   (FP),Y
   LDY   #$07
   STA   (DPTR),Y
   LDY   #$0A
   LDA   (FP),Y
   LDY   #$08
   STA   (DPTR),Y
   LDY   #$0B
   LDA   (FP),Y
   LDY   #$09
   STA   (DPTR),Y
   LDY   #$0C
   LDA   (FP),Y
   LDY   #$0A
   STA   (DPTR),Y
   LDY   #$0D
   LDA   (FP),Y
   LDY   #$0B
   STA   (DPTR),Y
   LDY   #$0E
   LDA   (FP),Y
   LDY   #$0C
   STA   (DPTR),Y
   LDY   #$0F
   LDA   (FP),Y
   LDY   #$0D
   STA   (DPTR),Y
   LDY   #$10
   LDA   (FP),Y
   LDY   #$0E
   STA   (DPTR),Y
   LDY   #$11
   LDA   (FP),Y
   LDY   #$0F
   STA   (DPTR),Y
   LDY   #$12
   LDA   (FP),Y
   LDY   #$10
   STA   (DPTR),Y
   LDY   #$13
   LDA   (FP),Y
   LDY   #$11
   STA   (DPTR),Y
   LDY   #$14
   LDA   (FP),Y
   LDY   #$12
   STA   (DPTR),Y
   LDY   #$15
   LDA   (FP),Y
   LDY   #$13
   STA   (DPTR),Y
   LDY   #$16
   LDA   (FP),Y
   LDY   #$14
   STA   (DPTR),Y
   LDY   #$17
   LDA   (FP),Y
   LDY   #$15
   STA   (DPTR),Y
   LDY   #$18
   LDA   (FP),Y
   LDY   #$16
   STA   (DPTR),Y
   LDY   #$19
   LDA   (FP),Y
   LDY   #$17
   STA   (DPTR),Y
   LDY   #$1A
   LDA   (FP),Y
   LDY   #$18
   STA   (DPTR),Y
   LDY   #$1B
   LDA   (FP),Y
   LDY   #$19
   STA   (DPTR),Y
   LDY   #$1C
   LDA   (FP),Y
   LDY   #$1A
   STA   (DPTR),Y
   LDY   #$1D
   LDA   (FP),Y
   LDY   #$1B
   STA   (DPTR),Y

   ; epilogue
   CLC
   LDA   FP
   ADC   #$74
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$71
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   RTS

validate_stack_struct:
   SUBROUTINE

   ; prologue: 2 arg bytes, 20 local bytes
   SEC
   LDA   SSP
   SBC   #$16
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$15
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
.loop@2_start:
   LDY   #$01
   LDA   (FP),Y
   SEC
   SBC   #$1C
   INY
   LDA   (FP),Y
   SBC   #$00
   BVC   .cmp_novf@19
   EOR   #$80
.cmp_novf@19:
   ASL
   LDA   #$00
   ROL
   LDY   #$03
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@4
   JMP   .loop@2_break
.lb_skip@4:
   LDA   #<stack_struct
   LDY   #$05
   STA   (FP),Y
   LDA   #>stack_struct
   INY
   STA   (FP),Y
   LDY   #$01
   LDA   (FP),Y
   LDY   #$07
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$05
   LDA   (FP),Y
   CLC
   LDY   #$08
   ADC   (FP),Y
   LDY   #$0A
   STA   (FP),Y
   LDY   #$06
   LDA   (FP),Y
   LDY   #$09
   ADC   (FP),Y
   LDY   #$0B
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$0C
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$01
   LDA   (FP),Y
   CLC
   ADC   #$5A
   LDY   #$0F
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   ADC   #$00
   LDY   #$10
   STA   (FP),Y
   LDY   #$0E
   LDA   (FP),Y
   LDY   #$10
   CMP   (FP),Y
   BNE   .cmp_true@20
   LDY   #$0D
   LDA   (FP),Y
   LDY   #$0F
   CMP   (FP),Y
   BNE   .cmp_true@20
   LDA   #$00
   JMP   .cmp_end@21
.cmp_true@20:
   LDA   #$01
.cmp_end@21:
   LDY   #$11
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@10
   SEC
   LDA   SSP
   SBC   #$02
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDY   #$17
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$18
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   JSR   exit
.if_end@10:
   LDY   #$01
   LDA   (FP),Y
   CLC
   ADC   #$01
   LDY   #$13
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   ADC   #$00
   LDY   #$14
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   LDY   #$14
   LDA   (FP),Y
   LDY   #$02
   STA   (FP),Y
   JMP   .loop@2_start
.loop@2_break:

   ; epilogue
   CLC
   LDA   FP
   ADC   #$18
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$15
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   RTS

test_stack_struct:
   SUBROUTINE

   ; prologue: 0 arg bytes, 78 local bytes
   SEC
   LDA   SSP
   SBC   #$50
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$4F
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
   LDA   #$65
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$11
   LDA   (FP),Y
   CLC
   ADC   #$01
   LDY   #$13
   STA   (FP),Y
   DEY
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
   LDA   #$66
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$11
   LDA   (FP),Y
   CLC
   ADC   #$02
   LDY   #$15
   STA   (FP),Y
   LDY   #$12
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
   LDA   #$67
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$11
   LDA   (FP),Y
   CLC
   ADC   #$03
   LDY   #$17
   STA   (FP),Y
   LDY   #$12
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
   LDA   #$68
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$11
   LDA   (FP),Y
   CLC
   ADC   #$04
   LDY   #$19
   STA   (FP),Y
   LDY   #$12
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
   LDA   #$69
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$11
   LDA   (FP),Y
   CLC
   ADC   #$05
   LDY   #$1B
   STA   (FP),Y
   LDY   #$12
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
   LDA   #$6A
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$11
   LDA   (FP),Y
   CLC
   ADC   #$06
   LDY   #$1D
   STA   (FP),Y
   LDY   #$12
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
   LDA   #$6B
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$11
   LDA   (FP),Y
   CLC
   ADC   #$07
   LDY   #$1F
   STA   (FP),Y
   LDY   #$12
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
   LDA   #$6C
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$11
   LDA   (FP),Y
   CLC
   ADC   #$08
   LDY   #$21
   STA   (FP),Y
   LDY   #$12
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
   LDA   #$6D
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$11
   LDA   (FP),Y
   CLC
   ADC   #$09
   LDY   #$23
   STA   (FP),Y
   LDY   #$12
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
   LDA   #$6E
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$11
   LDA   (FP),Y
   CLC
   ADC   #$0A
   LDY   #$25
   STA   (FP),Y
   LDY   #$12
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
   LDA   #$6F
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$11
   LDA   (FP),Y
   CLC
   ADC   #$0B
   LDY   #$27
   STA   (FP),Y
   LDY   #$12
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
   LDA   #$70
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$11
   LDA   (FP),Y
   CLC
   ADC   #$0C
   LDY   #$29
   STA   (FP),Y
   LDY   #$12
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
   LDA   #$71
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$11
   LDA   (FP),Y
   CLC
   ADC   #$0D
   LDY   #$2B
   STA   (FP),Y
   LDY   #$12
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
   LDA   #$72
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$11
   LDA   (FP),Y
   CLC
   ADC   #$0E
   LDY   #$2D
   STA   (FP),Y
   LDY   #$12
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
   LDA   #$73
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$11
   LDA   (FP),Y
   CLC
   ADC   #$0F
   LDY   #$2F
   STA   (FP),Y
   LDY   #$12
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
   LDA   #$00
   LDY   #$00
   STA   (DPTR),Y
   CLC
   LDA   FP
   ADC   #$31
   LDY   #$4D
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
   LDY   #$4E
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   JSR   return_stack_struct
   LDY   #$31
   LDA   (FP),Y
   STA   stack_struct
   INY
   LDA   (FP),Y
   STA   stack_struct+1
   INY
   LDA   (FP),Y
   STA   stack_struct+2
   INY
   LDA   (FP),Y
   STA   stack_struct+3
   INY
   LDA   (FP),Y
   STA   stack_struct+4
   INY
   LDA   (FP),Y
   STA   stack_struct+5
   INY
   LDA   (FP),Y
   STA   stack_struct+6
   INY
   LDA   (FP),Y
   STA   stack_struct+7
   INY
   LDA   (FP),Y
   STA   stack_struct+8
   INY
   LDA   (FP),Y
   STA   stack_struct+9
   INY
   LDA   (FP),Y
   STA   stack_struct+10
   INY
   LDA   (FP),Y
   STA   stack_struct+11
   INY
   LDA   (FP),Y
   STA   stack_struct+12
   INY
   LDA   (FP),Y
   STA   stack_struct+13
   INY
   LDA   (FP),Y
   STA   stack_struct+14
   INY
   LDA   (FP),Y
   STA   stack_struct+15
   INY
   LDA   (FP),Y
   STA   stack_struct+16
   INY
   LDA   (FP),Y
   STA   stack_struct+17
   INY
   LDA   (FP),Y
   STA   stack_struct+18
   INY
   LDA   (FP),Y
   STA   stack_struct+19
   INY
   LDA   (FP),Y
   STA   stack_struct+20
   INY
   LDA   (FP),Y
   STA   stack_struct+21
   INY
   LDA   (FP),Y
   STA   stack_struct+22
   INY
   LDA   (FP),Y
   STA   stack_struct+23
   INY
   LDA   (FP),Y
   STA   stack_struct+24
   INY
   LDA   (FP),Y
   STA   stack_struct+25
   INY
   LDA   (FP),Y
   STA   stack_struct+26
   INY
   LDA   (FP),Y
   STA   stack_struct+27
   LDY   #$01
   LDA   (FP),Y
   STA   to_validate
   INY
   LDA   (FP),Y
   STA   to_validate+1
   INY
   LDA   (FP),Y
   STA   to_validate+2
   INY
   LDA   (FP),Y
   STA   to_validate+3
   INY
   LDA   (FP),Y
   STA   to_validate+4
   INY
   LDA   (FP),Y
   STA   to_validate+5
   INY
   LDA   (FP),Y
   STA   to_validate+6
   INY
   LDA   (FP),Y
   STA   to_validate+7
   INY
   LDA   (FP),Y
   STA   to_validate+8
   INY
   LDA   (FP),Y
   STA   to_validate+9
   INY
   LDA   (FP),Y
   STA   to_validate+10
   INY
   LDA   (FP),Y
   STA   to_validate+11
   INY
   LDA   (FP),Y
   STA   to_validate+12
   INY
   LDA   (FP),Y
   STA   to_validate+13
   INY
   LDA   (FP),Y
   STA   to_validate+14
   INY
   LDA   (FP),Y
   STA   to_validate+15
   SEC
   LDA   SSP
   SBC   #$02
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   #$0B
   LDY   #$01
   STA   (SSP),Y
   LDA   #$00
   INY
   STA   (SSP),Y
   JSR   validate_stack_bytes
   SEC
   LDA   SSP
   SBC   #$02
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   #$0C
   LDY   #$01
   STA   (SSP),Y
   LDA   #$00
   INY
   STA   (SSP),Y
   JSR   validate_stack_struct
   LDX   #$00
   TXA

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$50
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$4F
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

return_irregular_stack_struct:
   SUBROUTINE

   ; prologue: 2 arg bytes, 76 local bytes
   SEC
   LDA   SSP
   SBC   #$4E
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$4D
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   LDA   #$46
   LDY   #$01
   STA   (FP),Y
   CLC
   LDA   FP
   ADC   #$02
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
   LDY   #$01
   LDA   (FP),Y
   DEY
   STA   (DPTR),Y
   LDA   #$47
   LDY   #$17
   STA   (FP),Y
   LDY   #$15
   LDA   (FP),Y
   CLC
   ADC   #$01
   LDY   #$18
   STA   (FP),Y
   LDY   #$16
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
   LDA   #$48
   LDY   #$1A
   STA   (FP),Y
   LDY   #$15
   LDA   (FP),Y
   CLC
   ADC   #$02
   LDY   #$1B
   STA   (FP),Y
   LDY   #$16
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
   LDY   #$1A
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDA   #$49
   LDY   #$1D
   STA   (FP),Y
   LDY   #$15
   LDA   (FP),Y
   CLC
   ADC   #$03
   LDY   #$1E
   STA   (FP),Y
   LDY   #$16
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
   LDY   #$1D
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDA   #$4A
   LDY   #$20
   STA   (FP),Y
   LDY   #$15
   LDA   (FP),Y
   CLC
   ADC   #$04
   LDY   #$21
   STA   (FP),Y
   LDY   #$16
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
   LDY   #$20
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDA   #$4B
   LDY   #$23
   STA   (FP),Y
   LDY   #$15
   LDA   (FP),Y
   CLC
   ADC   #$05
   LDY   #$24
   STA   (FP),Y
   LDY   #$16
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
   LDY   #$23
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDA   #$4C
   LDY   #$26
   STA   (FP),Y
   LDY   #$15
   LDA   (FP),Y
   CLC
   ADC   #$06
   LDY   #$27
   STA   (FP),Y
   LDY   #$16
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
   LDA   #$4D
   LDY   #$29
   STA   (FP),Y
   LDY   #$15
   LDA   (FP),Y
   CLC
   ADC   #$07
   LDY   #$2A
   STA   (FP),Y
   LDY   #$16
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
   LDY   #$29
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDA   #$4E
   LDY   #$2C
   STA   (FP),Y
   LDY   #$15
   LDA   (FP),Y
   CLC
   ADC   #$08
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
   LDY   #$2C
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDA   #$4F
   LDY   #$2F
   STA   (FP),Y
   LDY   #$15
   LDA   (FP),Y
   CLC
   ADC   #$09
   LDY   #$30
   STA   (FP),Y
   LDY   #$16
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
   LDY   #$2F
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDA   #$50
   LDY   #$32
   STA   (FP),Y
   LDY   #$15
   LDA   (FP),Y
   CLC
   ADC   #$0A
   LDY   #$33
   STA   (FP),Y
   LDY   #$16
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
   LDY   #$32
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDA   #$51
   LDY   #$35
   STA   (FP),Y
   LDY   #$15
   LDA   (FP),Y
   CLC
   ADC   #$0B
   LDY   #$36
   STA   (FP),Y
   LDY   #$16
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
   LDY   #$35
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDA   #$52
   LDY   #$38
   STA   (FP),Y
   LDY   #$15
   LDA   (FP),Y
   CLC
   ADC   #$0C
   LDY   #$39
   STA   (FP),Y
   LDY   #$16
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
   LDY   #$38
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDA   #$53
   LDY   #$3B
   STA   (FP),Y
   LDY   #$15
   LDA   (FP),Y
   CLC
   ADC   #$0D
   LDY   #$3C
   STA   (FP),Y
   LDY   #$16
   LDA   (FP),Y
   ADC   #$00
   LDY   #$3D
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$3B
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDA   #$54
   LDY   #$3E
   STA   (FP),Y
   LDY   #$15
   LDA   (FP),Y
   CLC
   ADC   #$0E
   LDY   #$3F
   STA   (FP),Y
   LDY   #$16
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
   LDY   #$3E
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDA   #$55
   LDY   #$41
   STA   (FP),Y
   LDY   #$15
   LDA   (FP),Y
   CLC
   ADC   #$0F
   LDY   #$42
   STA   (FP),Y
   LDY   #$16
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
   LDY   #$41
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDA   #$56
   LDY   #$44
   STA   (FP),Y
   LDY   #$15
   LDA   (FP),Y
   CLC
   ADC   #$10
   LDY   #$45
   STA   (FP),Y
   LDY   #$16
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
   LDY   #$44
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDA   #$57
   LDY   #$47
   STA   (FP),Y
   LDY   #$15
   LDA   (FP),Y
   CLC
   ADC   #$11
   LDY   #$48
   STA   (FP),Y
   LDY   #$16
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
   LDY   #$47
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDA   #$58
   LDY   #$4A
   STA   (FP),Y
   LDY   #$15
   LDA   (FP),Y
   CLC
   ADC   #$12
   LDY   #$4B
   STA   (FP),Y
   LDY   #$16
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
   LDY   #$4A
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$4F
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$02
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$03
   LDA   (FP),Y
   LDY   #$01
   STA   (DPTR),Y
   LDY   #$04
   LDA   (FP),Y
   LDY   #$02
   STA   (DPTR),Y
   LDY   #$05
   LDA   (FP),Y
   LDY   #$03
   STA   (DPTR),Y
   LDY   #$06
   LDA   (FP),Y
   LDY   #$04
   STA   (DPTR),Y
   LDY   #$07
   LDA   (FP),Y
   LDY   #$05
   STA   (DPTR),Y
   LDY   #$08
   LDA   (FP),Y
   LDY   #$06
   STA   (DPTR),Y
   LDY   #$09
   LDA   (FP),Y
   LDY   #$07
   STA   (DPTR),Y
   LDY   #$0A
   LDA   (FP),Y
   LDY   #$08
   STA   (DPTR),Y
   LDY   #$0B
   LDA   (FP),Y
   LDY   #$09
   STA   (DPTR),Y
   LDY   #$0C
   LDA   (FP),Y
   LDY   #$0A
   STA   (DPTR),Y
   LDY   #$0D
   LDA   (FP),Y
   LDY   #$0B
   STA   (DPTR),Y
   LDY   #$0E
   LDA   (FP),Y
   LDY   #$0C
   STA   (DPTR),Y
   LDY   #$0F
   LDA   (FP),Y
   LDY   #$0D
   STA   (DPTR),Y
   LDY   #$10
   LDA   (FP),Y
   LDY   #$0E
   STA   (DPTR),Y
   LDY   #$11
   LDA   (FP),Y
   LDY   #$0F
   STA   (DPTR),Y
   LDY   #$12
   LDA   (FP),Y
   LDY   #$10
   STA   (DPTR),Y
   LDY   #$13
   LDA   (FP),Y
   LDY   #$11
   STA   (DPTR),Y
   LDY   #$14
   LDA   (FP),Y
   LDY   #$12
   STA   (DPTR),Y

   ; epilogue
   CLC
   LDA   FP
   ADC   #$50
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$4D
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   RTS

validate_irregular_stack_struct:
   SUBROUTINE

   ; prologue: 2 arg bytes, 20 local bytes
   SEC
   LDA   SSP
   SBC   #$16
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$15
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
.loop@3_start:
   LDY   #$01
   LDA   (FP),Y
   SEC
   SBC   #$13
   INY
   LDA   (FP),Y
   SBC   #$00
   BVC   .cmp_novf@23
   EOR   #$80
.cmp_novf@23:
   ASL
   LDA   #$00
   ROL
   LDY   #$03
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@5
   JMP   .loop@3_break
.lb_skip@5:
   LDA   #<irregular_stack_struct
   LDY   #$05
   STA   (FP),Y
   LDA   #>irregular_stack_struct
   INY
   STA   (FP),Y
   LDY   #$01
   LDA   (FP),Y
   LDY   #$07
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$05
   LDA   (FP),Y
   CLC
   LDY   #$08
   ADC   (FP),Y
   LDY   #$0A
   STA   (FP),Y
   LDY   #$06
   LDA   (FP),Y
   LDY   #$09
   ADC   (FP),Y
   LDY   #$0B
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$0C
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$01
   LDA   (FP),Y
   CLC
   ADC   #$46
   LDY   #$0F
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   ADC   #$00
   LDY   #$10
   STA   (FP),Y
   LDY   #$0E
   LDA   (FP),Y
   LDY   #$10
   CMP   (FP),Y
   BNE   .cmp_true@24
   LDY   #$0D
   LDA   (FP),Y
   LDY   #$0F
   CMP   (FP),Y
   BNE   .cmp_true@24
   LDA   #$00
   JMP   .cmp_end@25
.cmp_true@24:
   LDA   #$01
.cmp_end@25:
   LDY   #$11
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@11
   SEC
   LDA   SSP
   SBC   #$02
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDY   #$17
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$18
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   JSR   exit
.if_end@11:
   LDY   #$01
   LDA   (FP),Y
   CLC
   ADC   #$01
   LDY   #$13
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   ADC   #$00
   LDY   #$14
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   LDY   #$14
   LDA   (FP),Y
   LDY   #$02
   STA   (FP),Y
   JMP   .loop@3_start
.loop@3_break:

   ; epilogue
   CLC
   LDA   FP
   ADC   #$18
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$15
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   RTS

test_irregular_stack_struct:
   SUBROUTINE

   ; prologue: 0 arg bytes, 69 local bytes
   SEC
   LDA   SSP
   SBC   #$47
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$46
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
   LDA   #$65
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$11
   LDA   (FP),Y
   CLC
   ADC   #$01
   LDY   #$13
   STA   (FP),Y
   DEY
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
   LDA   #$66
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$11
   LDA   (FP),Y
   CLC
   ADC   #$02
   LDY   #$15
   STA   (FP),Y
   LDY   #$12
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
   LDA   #$67
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$11
   LDA   (FP),Y
   CLC
   ADC   #$03
   LDY   #$17
   STA   (FP),Y
   LDY   #$12
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
   LDA   #$68
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$11
   LDA   (FP),Y
   CLC
   ADC   #$04
   LDY   #$19
   STA   (FP),Y
   LDY   #$12
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
   LDA   #$69
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$11
   LDA   (FP),Y
   CLC
   ADC   #$05
   LDY   #$1B
   STA   (FP),Y
   LDY   #$12
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
   LDA   #$6A
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$11
   LDA   (FP),Y
   CLC
   ADC   #$06
   LDY   #$1D
   STA   (FP),Y
   LDY   #$12
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
   LDA   #$6B
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$11
   LDA   (FP),Y
   CLC
   ADC   #$07
   LDY   #$1F
   STA   (FP),Y
   LDY   #$12
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
   LDA   #$6C
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$11
   LDA   (FP),Y
   CLC
   ADC   #$08
   LDY   #$21
   STA   (FP),Y
   LDY   #$12
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
   LDA   #$6D
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$11
   LDA   (FP),Y
   CLC
   ADC   #$09
   LDY   #$23
   STA   (FP),Y
   LDY   #$12
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
   LDA   #$6E
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$11
   LDA   (FP),Y
   CLC
   ADC   #$0A
   LDY   #$25
   STA   (FP),Y
   LDY   #$12
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
   LDA   #$6F
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$11
   LDA   (FP),Y
   CLC
   ADC   #$0B
   LDY   #$27
   STA   (FP),Y
   LDY   #$12
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
   LDA   #$70
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$11
   LDA   (FP),Y
   CLC
   ADC   #$0C
   LDY   #$29
   STA   (FP),Y
   LDY   #$12
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
   LDA   #$71
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$11
   LDA   (FP),Y
   CLC
   ADC   #$0D
   LDY   #$2B
   STA   (FP),Y
   LDY   #$12
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
   LDA   #$72
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$11
   LDA   (FP),Y
   CLC
   ADC   #$0E
   LDY   #$2D
   STA   (FP),Y
   LDY   #$12
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
   LDA   #$73
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$11
   LDA   (FP),Y
   CLC
   ADC   #$0F
   LDY   #$2F
   STA   (FP),Y
   LDY   #$12
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
   LDA   #$00
   LDY   #$00
   STA   (DPTR),Y
   CLC
   LDA   FP
   ADC   #$31
   LDY   #$44
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
   LDY   #$45
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   JSR   return_irregular_stack_struct
   LDY   #$31
   LDA   (FP),Y
   STA   irregular_stack_struct
   INY
   LDA   (FP),Y
   STA   irregular_stack_struct+1
   INY
   LDA   (FP),Y
   STA   irregular_stack_struct+2
   INY
   LDA   (FP),Y
   STA   irregular_stack_struct+3
   INY
   LDA   (FP),Y
   STA   irregular_stack_struct+4
   INY
   LDA   (FP),Y
   STA   irregular_stack_struct+5
   INY
   LDA   (FP),Y
   STA   irregular_stack_struct+6
   INY
   LDA   (FP),Y
   STA   irregular_stack_struct+7
   INY
   LDA   (FP),Y
   STA   irregular_stack_struct+8
   INY
   LDA   (FP),Y
   STA   irregular_stack_struct+9
   INY
   LDA   (FP),Y
   STA   irregular_stack_struct+10
   INY
   LDA   (FP),Y
   STA   irregular_stack_struct+11
   INY
   LDA   (FP),Y
   STA   irregular_stack_struct+12
   INY
   LDA   (FP),Y
   STA   irregular_stack_struct+13
   INY
   LDA   (FP),Y
   STA   irregular_stack_struct+14
   INY
   LDA   (FP),Y
   STA   irregular_stack_struct+15
   INY
   LDA   (FP),Y
   STA   irregular_stack_struct+16
   INY
   LDA   (FP),Y
   STA   irregular_stack_struct+17
   INY
   LDA   (FP),Y
   STA   irregular_stack_struct+18
   LDY   #$01
   LDA   (FP),Y
   STA   to_validate
   INY
   LDA   (FP),Y
   STA   to_validate+1
   INY
   LDA   (FP),Y
   STA   to_validate+2
   INY
   LDA   (FP),Y
   STA   to_validate+3
   INY
   LDA   (FP),Y
   STA   to_validate+4
   INY
   LDA   (FP),Y
   STA   to_validate+5
   INY
   LDA   (FP),Y
   STA   to_validate+6
   INY
   LDA   (FP),Y
   STA   to_validate+7
   INY
   LDA   (FP),Y
   STA   to_validate+8
   INY
   LDA   (FP),Y
   STA   to_validate+9
   INY
   LDA   (FP),Y
   STA   to_validate+10
   INY
   LDA   (FP),Y
   STA   to_validate+11
   INY
   LDA   (FP),Y
   STA   to_validate+12
   INY
   LDA   (FP),Y
   STA   to_validate+13
   INY
   LDA   (FP),Y
   STA   to_validate+14
   INY
   LDA   (FP),Y
   STA   to_validate+15
   SEC
   LDA   SSP
   SBC   #$02
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   #$0D
   LDY   #$01
   STA   (SSP),Y
   LDA   #$00
   INY
   STA   (SSP),Y
   JSR   validate_stack_bytes
   SEC
   LDA   SSP
   SBC   #$02
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   #$0E
   LDY   #$01
   STA   (SSP),Y
   LDA   #$00
   INY
   STA   (SSP),Y
   JSR   validate_irregular_stack_struct
   LDX   #$00
   TXA

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$47
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$46
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

   ; prologue: 0 arg bytes, 14 local bytes
   SEC
   LDA   SSP
   SBC   #$10
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$0F
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   JSR   test_int_struct
   LDY   #$01
   STA   (FP),Y
   TXA
   INY
   STA   (FP),Y
   JSR   test_two_int_struct
   LDY   #$03
   STA   (FP),Y
   TXA
   INY
   STA   (FP),Y
   JSR   test_one_double_struct
   LDY   #$05
   STA   (FP),Y
   TXA
   INY
   STA   (FP),Y
   JSR   test_two_doubles_struct
   LDY   #$07
   STA   (FP),Y
   TXA
   INY
   STA   (FP),Y
   JSR   test_mixed_struct
   LDY   #$09
   STA   (FP),Y
   TXA
   INY
   STA   (FP),Y
   JSR   test_stack_struct
   LDY   #$0B
   STA   (FP),Y
   TXA
   INY
   STA   (FP),Y
   JSR   test_irregular_stack_struct
   LDY   #$0D
   STA   (FP),Y
   TXA
   INY
   STA   (FP),Y
   LDX   #$00
   TXA

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$10
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

str@0:
   DC.B  $65, $66, $67, $68, $69, $6A, $6B, $6C, $6D, $6E, $6F, $70, $71, $72, $73, $00

to_validate:
   DS.B  16

one_int_struct:
   DS.B  7

two_int_struct:
   DS.B  15

one_double_struct:
   DS.B  8

two_doubles_struct:
   DS.B  16

mixed_struct:
   DS.B  9

stack_struct:
   DS.B  28

irregular_stack_struct:
   DS.B  19
