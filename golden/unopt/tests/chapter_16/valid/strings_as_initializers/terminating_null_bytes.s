SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24

test_flat_static_with_null_byte:
   SUBROUTINE

   ; prologue: 0 arg bytes, 26 local bytes
   SEC
   LDA   SSP
   SBC   #$1C
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$1B
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   LDA   @2.flat
   LDY   #$01
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CMP   #$64
   BEQ   .cmp_true@0
   LDA   #$00
   JMP   .cmp_end@1
.cmp_true@0:
   LDA   #$01
.cmp_end@1:
   LDY   #$04
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@4
   LDA   @2.flat+1
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CMP   #$6F
   BEQ   .cmp_true@2
   LDA   #$00
   JMP   .cmp_end@3
.cmp_true@2:
   LDA   #$01
.cmp_end@3:
   LDY   #$09
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@4
   LDA   #$01
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   JMP   .and_end@5
.and_false@4:
   LDA   #$00
   LDY   #$0B
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@5:
   LDY   #$0B
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@2
   LDA   @2.flat+2
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CMP   #$67
   BEQ   .cmp_true@4
   LDA   #$00
   JMP   .cmp_end@5
.cmp_true@4:
   LDA   #$01
.cmp_end@5:
   LDY   #$10
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
   LDY   #$12
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@3:
   LDY   #$12
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@0
   LDA   @2.flat+3
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   BEQ   .cmp_true@6
   LDA   #$00
   JMP   .cmp_end@7
.cmp_true@6:
   LDA   #$01
.cmp_end@7:
   LDY   #$17
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
   LDY   #$19
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@1:
   TAX
   LDY   #$19
   LDA   (FP),Y

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$1C
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$1B
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

test_nested_static_with_null_byte:
   SUBROUTINE

   ; prologue: 0 arg bytes, 54 local bytes
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

   LDA   @3.nested
   LDY   #$01
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CMP   #$79
   BEQ   .cmp_true@8
   LDA   #$00
   JMP   .cmp_end@9
.cmp_true@8:
   LDA   #$01
.cmp_end@9:
   LDY   #$04
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@18
   LDA   @3.nested+1
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CMP   #$65
   BEQ   .cmp_true@10
   LDA   #$00
   JMP   .cmp_end@11
.cmp_true@10:
   LDA   #$01
.cmp_end@11:
   LDY   #$09
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@18
   LDA   #$01
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   JMP   .and_end@19
.and_false@18:
   LDA   #$00
   LDY   #$0B
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@19:
   LDY   #$0B
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@16
   LDA   @3.nested+2
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CMP   #$73
   BEQ   .cmp_true@12
   LDA   #$00
   JMP   .cmp_end@13
.cmp_true@12:
   LDA   #$01
.cmp_end@13:
   LDY   #$10
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@16
   LDA   #$01
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   JMP   .and_end@17
.and_false@16:
   LDA   #$00
   LDY   #$12
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@17:
   LDY   #$12
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@14
   LDA   @3.nested+3
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   BEQ   .cmp_true@14
   LDA   #$00
   JMP   .cmp_end@15
.cmp_true@14:
   LDA   #$01
.cmp_end@15:
   LDY   #$17
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@14
   LDA   #$01
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   JMP   .and_end@15
.and_false@14:
   LDA   #$00
   LDY   #$19
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@15:
   LDY   #$19
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@12
   LDA   @3.nested+4
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CMP   #$79
   BEQ   .cmp_true@16
   LDA   #$00
   JMP   .cmp_end@17
.cmp_true@16:
   LDA   #$01
.cmp_end@17:
   LDY   #$1E
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@12
   LDA   #$01
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   JMP   .and_end@13
.and_false@12:
   LDA   #$00
   LDY   #$20
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@13:
   LDY   #$20
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@10
   LDA   @3.nested+5
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CMP   #$75
   BEQ   .cmp_true@18
   LDA   #$00
   JMP   .cmp_end@19
.cmp_true@18:
   LDA   #$01
.cmp_end@19:
   LDY   #$25
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@10
   LDA   #$01
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   JMP   .and_end@11
.and_false@10:
   LDA   #$00
   LDY   #$27
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@11:
   LDY   #$27
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@8
   LDA   @3.nested+6
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CMP   #$70
   BEQ   .cmp_true@20
   LDA   #$00
   JMP   .cmp_end@21
.cmp_true@20:
   LDA   #$01
.cmp_end@21:
   LDY   #$2C
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@8
   LDA   #$01
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   JMP   .and_end@9
.and_false@8:
   LDA   #$00
   LDY   #$2E
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@9:
   LDY   #$2E
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@6
   LDA   @3.nested+7
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   BEQ   .cmp_true@22
   LDA   #$00
   JMP   .cmp_end@23
.cmp_true@22:
   LDA   #$01
.cmp_end@23:
   LDY   #$33
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@6
   LDA   #$01
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   JMP   .and_end@7
.and_false@6:
   LDA   #$00
   LDY   #$35
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@7:
   TAX
   LDY   #$35
   LDA   (FP),Y

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$38
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

test_flat_auto_with_null_byte:
   SUBROUTINE

   ; prologue: 0 arg bytes, 26 local bytes
   SEC
   LDA   SSP
   SBC   #$1C
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$1B
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
   LDY   #$03
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
   LDA   #$78
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$03
   LDA   (FP),Y
   CLC
   ADC   #$01
   LDY   #$05
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$06
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
   LDY   #$07
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   LDY   #$09
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$0A
   STA   (FP),Y
   DEY
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
   CMP   #$78
   BEQ   .cmp_true@24
   LDA   #$00
   JMP   .cmp_end@25
.cmp_true@24:
   LDA   #$01
.cmp_end@25:
   LDY   #$0E
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@20
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
   ADC   #$01
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
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$14
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   BEQ   .cmp_true@26
   LDA   #$00
   JMP   .cmp_end@27
.cmp_true@26:
   LDA   #$01
.cmp_end@27:
   LDY   #$17
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@20
   LDA   #$01
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   JMP   .and_end@21
.and_false@20:
   LDA   #$00
   LDY   #$19
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@21:
   TAX
   LDY   #$19
   LDA   (FP),Y

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$1C
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$1B
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

test_nested_auto_with_null_byte:
   SUBROUTINE

   ; prologue: 0 arg bytes, 174 local bytes
   SEC
   LDA   SSP
   SBC   #$B0
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$AF
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
   LDA   #$61
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$09
   LDA   (FP),Y
   CLC
   ADC   #$01
   LDY   #$0B
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$0C
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
   LDY   #$09
   LDA   (FP),Y
   CLC
   ADC   #$02
   LDY   #$0D
   STA   (FP),Y
   LDY   #$0A
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
   LDA   #$62
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$09
   LDA   (FP),Y
   CLC
   ADC   #$03
   LDY   #$0F
   STA   (FP),Y
   LDY   #$0A
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
   LDA   #$00
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$09
   LDA   (FP),Y
   CLC
   ADC   #$04
   LDY   #$11
   STA   (FP),Y
   LDY   #$0A
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
   LDA   #$63
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$09
   LDA   (FP),Y
   CLC
   ADC   #$05
   LDY   #$13
   STA   (FP),Y
   LDY   #$0A
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
   LDY   #$09
   LDA   (FP),Y
   CLC
   ADC   #$06
   LDY   #$15
   STA   (FP),Y
   LDY   #$0A
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
   LDA   #$64
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$09
   LDA   (FP),Y
   CLC
   ADC   #$07
   LDY   #$17
   STA   (FP),Y
   LDY   #$0A
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
   LDA   #$00
   LDY   #$00
   STA   (DPTR),Y
   CLC
   LDA   FP
   ADC   #$01
   LDY   #$1B
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   LDA   #$00
   STA   HARGS
   LDX   #$08
   LSR   HARGS
.mul_loop@28:
   BCC   .mul_skip@29
   CLC
   ADC   #$04
.mul_skip@29:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@28
   LDY   #$1A
   STA   (FP),Y
   LDA   HARGS
   DEY
   STA   (FP),Y
   LDY   #$1B
   LDA   (FP),Y
   CLC
   LDY   #$19
   ADC   (FP),Y
   LDY   #$1D
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$1A
   ADC   (FP),Y
   LDY   #$1E
   STA   (FP),Y
   LDA   #$00
   STA   HARGS
   LDX   #$08
   LSR   HARGS
.mul_loop@30:
   BCC   .mul_skip@31
   CLC
   ADC   #$02
.mul_skip@31:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@30
   LDY   #$20
   STA   (FP),Y
   LDA   HARGS
   DEY
   STA   (FP),Y
   LDY   #$1D
   LDA   (FP),Y
   CLC
   LDY   #$1F
   ADC   (FP),Y
   LDY   #$21
   STA   (FP),Y
   LDY   #$1E
   LDA   (FP),Y
   LDY   #$20
   ADC   (FP),Y
   LDY   #$22
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
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
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$25
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CMP   #$61
   BEQ   .cmp_true@32
   LDA   #$00
   JMP   .cmp_end@33
.cmp_true@32:
   LDA   #$01
.cmp_end@33:
   LDY   #$28
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@6
   JMP   .and_false@34
.lb_skip@6:
   CLC
   LDA   FP
   ADC   #$01
   LDY   #$2C
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   LDA   #$00
   STA   HARGS
   LDX   #$08
   LSR   HARGS
.mul_loop@34:
   BCC   .mul_skip@35
   CLC
   ADC   #$04
.mul_skip@35:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@34
   LDY   #$2B
   STA   (FP),Y
   LDA   HARGS
   DEY
   STA   (FP),Y
   LDY   #$2C
   LDA   (FP),Y
   CLC
   LDY   #$2A
   ADC   (FP),Y
   LDY   #$2E
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$2B
   ADC   (FP),Y
   LDY   #$2F
   STA   (FP),Y
   LDA   #$00
   STA   HARGS
   LDX   #$08
   LSR   HARGS
.mul_loop@36:
   BCC   .mul_skip@37
   CLC
   ADC   #$02
.mul_skip@37:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@36
   LDY   #$31
   STA   (FP),Y
   LDA   HARGS
   DEY
   STA   (FP),Y
   LDY   #$2E
   LDA   (FP),Y
   CLC
   LDY   #$30
   ADC   (FP),Y
   LDY   #$32
   STA   (FP),Y
   LDY   #$2F
   LDA   (FP),Y
   LDY   #$31
   ADC   (FP),Y
   LDY   #$33
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$01
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
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CMP   #$00
   BEQ   .cmp_true@38
   LDA   #$00
   JMP   .cmp_end@39
.cmp_true@38:
   LDA   #$01
.cmp_end@39:
   LDY   #$39
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@34
   LDA   #$01
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   JMP   .and_end@35
.and_false@34:
   LDA   #$00
   LDY   #$3B
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@35:
   LDY   #$3B
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@5
   JMP   .and_false@32
.lb_skip@5:
   CLC
   LDA   FP
   ADC   #$01
   LDY   #$3F
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   LDA   #$00
   STA   HARGS
   LDX   #$08
   LSR   HARGS
.mul_loop@40:
   BCC   .mul_skip@41
   CLC
   ADC   #$04
.mul_skip@41:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@40
   LDY   #$3E
   STA   (FP),Y
   LDA   HARGS
   DEY
   STA   (FP),Y
   LDY   #$3F
   LDA   (FP),Y
   CLC
   LDY   #$3D
   ADC   (FP),Y
   LDY   #$41
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$3E
   ADC   (FP),Y
   LDY   #$42
   STA   (FP),Y
   LDA   #$01
   STA   HARGS
   LDA   #$00
   LDX   #$08
   LSR   HARGS
.mul_loop@42:
   BCC   .mul_skip@43
   CLC
   ADC   #$02
.mul_skip@43:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@42
   LDY   #$44
   STA   (FP),Y
   LDA   HARGS
   DEY
   STA   (FP),Y
   LDY   #$41
   LDA   (FP),Y
   CLC
   LDY   #$43
   ADC   (FP),Y
   LDY   #$45
   STA   (FP),Y
   LDY   #$42
   LDA   (FP),Y
   LDY   #$44
   ADC   (FP),Y
   LDY   #$46
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
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
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CMP   #$62
   BEQ   .cmp_true@44
   LDA   #$00
   JMP   .cmp_end@45
.cmp_true@44:
   LDA   #$01
.cmp_end@45:
   LDY   #$4C
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@32
   LDA   #$01
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   JMP   .and_end@33
.and_false@32:
   LDA   #$00
   LDY   #$4E
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@33:
   LDY   #$4E
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@4
   JMP   .and_false@30
.lb_skip@4:
   CLC
   LDA   FP
   ADC   #$01
   LDY   #$52
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   LDA   #$00
   STA   HARGS
   LDX   #$08
   LSR   HARGS
.mul_loop@46:
   BCC   .mul_skip@47
   CLC
   ADC   #$04
.mul_skip@47:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@46
   LDY   #$51
   STA   (FP),Y
   LDA   HARGS
   DEY
   STA   (FP),Y
   LDY   #$52
   LDA   (FP),Y
   CLC
   LDY   #$50
   ADC   (FP),Y
   LDY   #$54
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$51
   ADC   (FP),Y
   LDY   #$55
   STA   (FP),Y
   LDA   #$01
   STA   HARGS
   LDA   #$00
   LDX   #$08
   LSR   HARGS
.mul_loop@48:
   BCC   .mul_skip@49
   CLC
   ADC   #$02
.mul_skip@49:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@48
   LDY   #$57
   STA   (FP),Y
   LDA   HARGS
   DEY
   STA   (FP),Y
   LDY   #$54
   LDA   (FP),Y
   CLC
   LDY   #$56
   ADC   (FP),Y
   LDY   #$58
   STA   (FP),Y
   LDY   #$55
   LDA   (FP),Y
   LDY   #$57
   ADC   (FP),Y
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
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CMP   #$00
   BEQ   .cmp_true@50
   LDA   #$00
   JMP   .cmp_end@51
.cmp_true@50:
   LDA   #$01
.cmp_end@51:
   LDY   #$5F
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@30
   LDA   #$01
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   JMP   .and_end@31
.and_false@30:
   LDA   #$00
   LDY   #$61
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@31:
   LDY   #$61
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@3
   JMP   .and_false@28
.lb_skip@3:
   CLC
   LDA   FP
   ADC   #$01
   LDY   #$65
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
   ADC   #$04
.mul_skip@53:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@52
   LDY   #$64
   STA   (FP),Y
   LDA   HARGS
   DEY
   STA   (FP),Y
   LDY   #$65
   LDA   (FP),Y
   CLC
   LDY   #$63
   ADC   (FP),Y
   LDY   #$67
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$64
   ADC   (FP),Y
   LDY   #$68
   STA   (FP),Y
   LDA   #$00
   STA   HARGS
   LDX   #$08
   LSR   HARGS
.mul_loop@54:
   BCC   .mul_skip@55
   CLC
   ADC   #$02
.mul_skip@55:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@54
   LDY   #$6A
   STA   (FP),Y
   LDA   HARGS
   DEY
   STA   (FP),Y
   LDY   #$67
   LDA   (FP),Y
   CLC
   LDY   #$69
   ADC   (FP),Y
   LDY   #$6B
   STA   (FP),Y
   LDY   #$68
   LDA   (FP),Y
   LDY   #$6A
   ADC   (FP),Y
   LDY   #$6C
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   LDY   #$6D
   STA   (FP),Y
   DEY
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
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$6F
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CMP   #$63
   BEQ   .cmp_true@56
   LDA   #$00
   JMP   .cmp_end@57
.cmp_true@56:
   LDA   #$01
.cmp_end@57:
   LDY   #$72
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@28
   LDA   #$01
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   JMP   .and_end@29
.and_false@28:
   LDA   #$00
   LDY   #$74
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@29:
   LDY   #$74
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@2
   JMP   .and_false@26
.lb_skip@2:
   CLC
   LDA   FP
   ADC   #$01
   LDY   #$78
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
   ADC   #$04
.mul_skip@59:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@58
   LDY   #$77
   STA   (FP),Y
   LDA   HARGS
   DEY
   STA   (FP),Y
   LDY   #$78
   LDA   (FP),Y
   CLC
   LDY   #$76
   ADC   (FP),Y
   LDY   #$7A
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$77
   ADC   (FP),Y
   LDY   #$7B
   STA   (FP),Y
   LDA   #$00
   STA   HARGS
   LDX   #$08
   LSR   HARGS
.mul_loop@60:
   BCC   .mul_skip@61
   CLC
   ADC   #$02
.mul_skip@61:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@60
   LDY   #$7D
   STA   (FP),Y
   LDA   HARGS
   DEY
   STA   (FP),Y
   LDY   #$7A
   LDA   (FP),Y
   CLC
   LDY   #$7C
   ADC   (FP),Y
   LDY   #$7E
   STA   (FP),Y
   LDY   #$7B
   LDA   (FP),Y
   LDY   #$7D
   ADC   (FP),Y
   LDY   #$7F
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$01
   LDY   #$80
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
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
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CMP   #$00
   BEQ   .cmp_true@62
   LDA   #$00
   JMP   .cmp_end@63
.cmp_true@62:
   LDA   #$01
.cmp_end@63:
   LDY   #$85
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@26
   LDA   #$01
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   JMP   .and_end@27
.and_false@26:
   LDA   #$00
   LDY   #$87
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@27:
   LDY   #$87
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@1
   JMP   .and_false@24
.lb_skip@1:
   CLC
   LDA   FP
   ADC   #$01
   LDY   #$8B
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
.mul_loop@64:
   BCC   .mul_skip@65
   CLC
   ADC   #$04
.mul_skip@65:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@64
   LDY   #$8A
   STA   (FP),Y
   LDA   HARGS
   DEY
   STA   (FP),Y
   LDY   #$8B
   LDA   (FP),Y
   CLC
   LDY   #$89
   ADC   (FP),Y
   LDY   #$8D
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$8A
   ADC   (FP),Y
   LDY   #$8E
   STA   (FP),Y
   LDA   #$01
   STA   HARGS
   LDA   #$00
   LDX   #$08
   LSR   HARGS
.mul_loop@66:
   BCC   .mul_skip@67
   CLC
   ADC   #$02
.mul_skip@67:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@66
   LDY   #$90
   STA   (FP),Y
   LDA   HARGS
   DEY
   STA   (FP),Y
   LDY   #$8D
   LDA   (FP),Y
   CLC
   LDY   #$8F
   ADC   (FP),Y
   LDY   #$91
   STA   (FP),Y
   LDY   #$8E
   LDA   (FP),Y
   LDY   #$90
   ADC   (FP),Y
   LDY   #$92
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   LDY   #$93
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$94
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$95
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CMP   #$64
   BEQ   .cmp_true@68
   LDA   #$00
   JMP   .cmp_end@69
.cmp_true@68:
   LDA   #$01
.cmp_end@69:
   LDY   #$98
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@24
   LDA   #$01
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   JMP   .and_end@25
.and_false@24:
   LDA   #$00
   LDY   #$9A
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@25:
   LDY   #$9A
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@0
   JMP   .and_false@22
.lb_skip@0:
   CLC
   LDA   FP
   ADC   #$01
   LDY   #$9E
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
.mul_loop@70:
   BCC   .mul_skip@71
   CLC
   ADC   #$04
.mul_skip@71:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@70
   LDY   #$9D
   STA   (FP),Y
   LDA   HARGS
   DEY
   STA   (FP),Y
   LDY   #$9E
   LDA   (FP),Y
   CLC
   LDY   #$9C
   ADC   (FP),Y
   LDY   #$A0
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$9D
   ADC   (FP),Y
   LDY   #$A1
   STA   (FP),Y
   LDA   #$01
   STA   HARGS
   LDA   #$00
   LDX   #$08
   LSR   HARGS
.mul_loop@72:
   BCC   .mul_skip@73
   CLC
   ADC   #$02
.mul_skip@73:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@72
   LDY   #$A3
   STA   (FP),Y
   LDA   HARGS
   DEY
   STA   (FP),Y
   LDY   #$A0
   LDA   (FP),Y
   CLC
   LDY   #$A2
   ADC   (FP),Y
   LDY   #$A4
   STA   (FP),Y
   LDY   #$A1
   LDA   (FP),Y
   LDY   #$A3
   ADC   (FP),Y
   LDY   #$A5
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$01
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
   BEQ   .cmp_true@74
   LDA   #$00
   JMP   .cmp_end@75
.cmp_true@74:
   LDA   #$01
.cmp_end@75:
   LDY   #$AB
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@22
   LDA   #$01
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   JMP   .and_end@23
.and_false@22:
   LDA   #$00
   LDY   #$AD
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@23:
   TAX
   LDY   #$AD
   LDA   (FP),Y

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$B0
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$AF
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

test_flat_static_without_null_byte:
   SUBROUTINE

   ; prologue: 0 arg bytes, 26 local bytes
   SEC
   LDA   SSP
   SBC   #$1C
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$1B
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   LDA   @6.letters
   LDY   #$01
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CMP   #$61
   BEQ   .cmp_true@76
   LDA   #$00
   JMP   .cmp_end@77
.cmp_true@76:
   LDA   #$01
.cmp_end@77:
   LDY   #$04
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@40
   LDA   @6.letters+1
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CMP   #$62
   BEQ   .cmp_true@78
   LDA   #$00
   JMP   .cmp_end@79
.cmp_true@78:
   LDA   #$01
.cmp_end@79:
   LDY   #$09
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@40
   LDA   #$01
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   JMP   .and_end@41
.and_false@40:
   LDA   #$00
   LDY   #$0B
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@41:
   LDY   #$0B
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@38
   LDA   @6.letters+2
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CMP   #$63
   BEQ   .cmp_true@80
   LDA   #$00
   JMP   .cmp_end@81
.cmp_true@80:
   LDA   #$01
.cmp_end@81:
   LDY   #$10
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@38
   LDA   #$01
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   JMP   .and_end@39
.and_false@38:
   LDA   #$00
   LDY   #$12
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@39:
   LDY   #$12
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@36
   LDA   @6.letters+3
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CMP   #$64
   BEQ   .cmp_true@82
   LDA   #$00
   JMP   .cmp_end@83
.cmp_true@82:
   LDA   #$01
.cmp_end@83:
   LDY   #$17
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@36
   LDA   #$01
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   JMP   .and_end@37
.and_false@36:
   LDA   #$00
   LDY   #$19
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@37:
   TAX
   LDY   #$19
   LDA   (FP),Y

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$1C
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$1B
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

test_nested_static_without_null_byte:
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

   LDA   #<nested
   LDY   #$01
   STA   (FP),Y
   LDA   #>nested
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
   LDA   #<nested
   INY
   STA   (FP),Y
   LDA   #>nested
   INY
   STA   (FP),Y
   LDA   #$00
   STA   HARGS
   LDX   #$08
   LSR   HARGS
.mul_loop@84:
   BCC   .mul_skip@85
   CLC
   ADC   #$03
.mul_skip@85:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@84
   LDY   #$08
   STA   (FP),Y
   LDA   HARGS
   DEY
   STA   (FP),Y
   LDY   #$05
   LDA   (FP),Y
   CLC
   LDY   #$07
   ADC   (FP),Y
   LDY   #$09
   STA   (FP),Y
   LDY   #$06
   LDA   (FP),Y
   LDY   #$08
   ADC   (FP),Y
   LDY   #$0A
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$0B
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$0C
   STA   (FP),Y
   LDA   #<nested
   INY
   STA   (FP),Y
   LDA   #>nested
   INY
   STA   (FP),Y
   LDA   #$01
   STA   HARGS
   LDA   #$00
   LDX   #$08
   LSR   HARGS
.mul_loop@86:
   BCC   .mul_skip@87
   CLC
   ADC   #$03
.mul_skip@87:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@86
   LDY   #$10
   STA   (FP),Y
   LDA   HARGS
   DEY
   STA   (FP),Y
   LDY   #$0D
   LDA   (FP),Y
   CLC
   LDY   #$0F
   ADC   (FP),Y
   LDY   #$11
   STA   (FP),Y
   LDY   #$0E
   LDA   (FP),Y
   LDY   #$10
   ADC   (FP),Y
   LDY   #$12
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$13
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$14
   STA   (FP),Y
   LDA   #<nested
   INY
   STA   (FP),Y
   LDA   #>nested
   INY
   STA   (FP),Y
   LDA   #$02
   STA   HARGS
   LDA   #$00
   LDX   #$08
   LSR   HARGS
.mul_loop@88:
   BCC   .mul_skip@89
   CLC
   ADC   #$03
.mul_skip@89:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@88
   LDY   #$18
   STA   (FP),Y
   LDA   HARGS
   DEY
   STA   (FP),Y
   LDY   #$15
   LDA   (FP),Y
   CLC
   LDY   #$17
   ADC   (FP),Y
   LDY   #$19
   STA   (FP),Y
   LDY   #$16
   LDA   (FP),Y
   LDY   #$18
   ADC   (FP),Y
   LDY   #$1A
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$1B
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$1C
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
   LDY   #$03
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$04
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$1D
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$1E
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   JSR   strcmp
   LDY   #$1F
   STA   (FP),Y
   TXA
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@46
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
   LDY   #$0B
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$0C
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$21
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$22
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   JSR   strcmp
   LDY   #$23
   STA   (FP),Y
   TXA
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
   LDY   #$25
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@47:
   LDY   #$25
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@44
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
   LDY   #$13
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$14
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$27
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$28
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   JSR   strcmp
   LDY   #$29
   STA   (FP),Y
   TXA
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
   LDY   #$2B
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@45:
   LDY   #$2B
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@42
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
   LDY   #$1B
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$1C
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
   BNE   .or_true@42
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   JMP   .or_end@43
.or_true@42:
   LDA   #$01
   LDY   #$31
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@43:
   LDY   #$31
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lnot_true@90
   LDA   #$00
   JMP   .lnot_end@91
.lnot_true@90:
   LDA   #$01
.lnot_end@91:
   LDY   #$33
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

test_flat_auto_without_null_byte:
   SUBROUTINE

   ; prologue: 0 arg bytes, 74 local bytes
   SEC
   LDA   SSP
   SBC   #$4C
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$4B
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
   CLC
   LDA   FP
   ADC   #$05
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
   LDA   #$61
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$09
   LDA   (FP),Y
   CLC
   ADC   #$01
   LDY   #$0B
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$0C
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
   LDY   #$09
   LDA   (FP),Y
   CLC
   ADC   #$02
   LDY   #$0D
   STA   (FP),Y
   LDY   #$0A
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
   LDA   #$63
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$09
   LDA   (FP),Y
   CLC
   ADC   #$03
   LDY   #$0F
   STA   (FP),Y
   LDY   #$0A
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
   LDA   #$64
   LDY   #$00
   STA   (DPTR),Y
   SEC
   LDA   #$00
   SBC   #$01
   LDY   #$11
   STA   (FP),Y
   LDA   #$00
   SBC   #$00
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
   SEC
   LDA   #$00
   SBC   #$01
   INY
   STA   (FP),Y
   LDA   #$00
   SBC   #$00
   INY
   STA   (FP),Y
   LDY   #$04
   LDA   (FP),Y
   LDY   #$16
   CMP   (FP),Y
   BNE   .thread@0
   LDY   #$03
   LDA   (FP),Y
   LDY   #$15
   CMP   (FP),Y
   BEQ   .cmp_true@92
.thread@0:
   LDA   #$00
   JMP   .cmp_end@93
.cmp_true@92:
   LDA   #$01
.cmp_end@93:
   LDY   #$17
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@56
   SEC
   LDA   #$00
   SBC   #$01
   INY
   STA   (FP),Y
   LDA   #$00
   SBC   #$00
   INY
   STA   (FP),Y
   LDY   #$14
   LDA   (FP),Y
   LDY   #$1A
   CMP   (FP),Y
   BNE   .thread@1
   LDY   #$13
   LDA   (FP),Y
   LDY   #$19
   CMP   (FP),Y
   BEQ   .cmp_true@95
.thread@1:
   LDA   #$00
   JMP   .cmp_end@96
.cmp_true@95:
   LDA   #$01
.cmp_end@96:
   LDY   #$1B
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@56
   LDA   #$01
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   JMP   .and_end@57
.and_false@56:
   LDA   #$00
   LDY   #$1D
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@57:
   LDY   #$1D
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@54
   CLC
   LDA   FP
   ADC   #$05
   INY
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
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
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$23
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CMP   #$61
   BEQ   .cmp_true@98
   LDA   #$00
   JMP   .cmp_end@99
.cmp_true@98:
   LDA   #$01
.cmp_end@99:
   LDY   #$26
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@54
   LDA   #$01
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   JMP   .and_end@55
.and_false@54:
   LDA   #$00
   LDY   #$28
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@55:
   LDY   #$28
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@52
   CLC
   LDA   FP
   ADC   #$05
   INY
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$01
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
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$2E
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CMP   #$62
   BEQ   .cmp_true@100
   LDA   #$00
   JMP   .cmp_end@101
.cmp_true@100:
   LDA   #$01
.cmp_end@101:
   LDY   #$31
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@52
   LDA   #$01
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   JMP   .and_end@53
.and_false@52:
   LDA   #$00
   LDY   #$33
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@53:
   LDY   #$33
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@50
   CLC
   LDA   FP
   ADC   #$05
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
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$39
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CMP   #$63
   BEQ   .cmp_true@102
   LDA   #$00
   JMP   .cmp_end@103
.cmp_true@102:
   LDA   #$01
.cmp_end@103:
   LDY   #$3C
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@50
   LDA   #$01
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   JMP   .and_end@51
.and_false@50:
   LDA   #$00
   LDY   #$3E
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@51:
   LDY   #$3E
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@48
   CLC
   LDA   FP
   ADC   #$05
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
   LDY   #$42
   STA   (FP),Y
   DEY
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
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$44
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CMP   #$64
   BEQ   .cmp_true@104
   LDA   #$00
   JMP   .cmp_end@105
.cmp_true@104:
   LDA   #$01
.cmp_end@105:
   LDY   #$47
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@48
   LDA   #$01
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   JMP   .and_end@49
.and_false@48:
   LDA   #$00
   LDY   #$49
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@49:
   TAX
   LDY   #$49
   LDA   (FP),Y

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$4C
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$4B
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

test_nested_auto_without_null_byte:
   SUBROUTINE

   ; prologue: 0 arg bytes, 79 local bytes
   SEC
   LDA   SSP
   SBC   #$51
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$50
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
   LDA   #$79
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
   LDA   #$65
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
   LDA   #$73
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
   LDA   #$6E
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$0A
   LDA   (FP),Y
   CLC
   ADC   #$04
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
   LDA   #$6F
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$0A
   LDA   (FP),Y
   CLC
   ADC   #$05
   LDY   #$14
   STA   (FP),Y
   LDY   #$0B
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
   LDA   #$00
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$0A
   LDA   (FP),Y
   CLC
   ADC   #$06
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
   LDA   #$6F
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$0A
   LDA   (FP),Y
   CLC
   ADC   #$07
   LDY   #$18
   STA   (FP),Y
   LDY   #$0B
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
   LDA   #$6B
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$0A
   LDA   (FP),Y
   CLC
   ADC   #$08
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
   LDA   #$00
   LDY   #$00
   STA   (DPTR),Y
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
   LDY   #$1E
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$1F
   STA   (FP),Y
   CLC
   LDA   FP
   ADC   #$01
   LDY   #$22
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   LDA   #$00
   STA   HARGS
   LDX   #$08
   LSR   HARGS
.mul_loop@106:
   BCC   .mul_skip@107
   CLC
   ADC   #$03
.mul_skip@107:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@106
   LDY   #$21
   STA   (FP),Y
   LDA   HARGS
   DEY
   STA   (FP),Y
   LDY   #$22
   LDA   (FP),Y
   CLC
   LDY   #$20
   ADC   (FP),Y
   LDY   #$24
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$21
   ADC   (FP),Y
   LDY   #$25
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$26
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$27
   STA   (FP),Y
   CLC
   LDA   FP
   ADC   #$01
   LDY   #$2A
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
.mul_loop@108:
   BCC   .mul_skip@109
   CLC
   ADC   #$03
.mul_skip@109:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@108
   LDY   #$29
   STA   (FP),Y
   LDA   HARGS
   DEY
   STA   (FP),Y
   LDY   #$2A
   LDA   (FP),Y
   CLC
   LDY   #$28
   ADC   (FP),Y
   LDY   #$2C
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$29
   ADC   (FP),Y
   LDY   #$2D
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$2E
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$2F
   STA   (FP),Y
   CLC
   LDA   FP
   ADC   #$01
   LDY   #$32
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
.mul_loop@110:
   BCC   .mul_skip@111
   CLC
   ADC   #$03
.mul_skip@111:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@110
   LDY   #$31
   STA   (FP),Y
   LDA   HARGS
   DEY
   STA   (FP),Y
   LDY   #$32
   LDA   (FP),Y
   CLC
   LDY   #$30
   ADC   (FP),Y
   LDY   #$34
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$31
   ADC   (FP),Y
   LDY   #$35
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$36
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$37
   STA   (FP),Y
   LDA   #<str@4
   INY
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
   LDY   #$1E
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$1F
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
   JSR   strcmp
   LDY   #$3A
   STA   (FP),Y
   TXA
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@62
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
   LDY   #$26
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$27
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$3C
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$3D
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   JSR   strcmp
   LDY   #$3E
   STA   (FP),Y
   TXA
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@62
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   JMP   .or_end@63
.or_true@62:
   LDA   #$01
   LDY   #$40
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@63:
   LDY   #$40
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@60
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
   LDY   #$2E
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$2F
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$42
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$43
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   JSR   strcmp
   LDY   #$44
   STA   (FP),Y
   TXA
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@60
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   JMP   .or_end@61
.or_true@60:
   LDA   #$01
   LDY   #$46
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@61:
   LDY   #$46
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@58
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
   LDY   #$36
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$37
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$48
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$49
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   JSR   strcmp
   LDY   #$4A
   STA   (FP),Y
   TXA
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@58
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   JMP   .or_end@59
.or_true@58:
   LDA   #$01
   LDY   #$4C
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@59:
   LDY   #$4C
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lnot_true@112
   LDA   #$00
   JMP   .lnot_end@113
.lnot_true@112:
   LDA   #$01
.lnot_end@113:
   LDY   #$4E
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
   ADC   #$51
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$50
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

   ; prologue: 0 arg bytes, 32 local bytes
   SEC
   LDA   SSP
   SBC   #$22
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$21
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   JSR   test_flat_static_with_null_byte
   LDY   #$01
   STA   (FP),Y
   TXA
   INY
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
   LDY   #$03
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@64
   LDX   #$00
   LDA   #$01

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$22
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$21
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
.if_end@64:
   JSR   test_nested_static_with_null_byte
   LDY   #$05
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
   LDY   #$07
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@65
   LDX   #$00
   LDA   #$02

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$22
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$21
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
   JSR   test_flat_auto_with_null_byte
   LDY   #$09
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
   LDY   #$0B
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
   LDA   #$03

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$22
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$21
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
   JSR   test_nested_auto_with_null_byte
   LDY   #$0D
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
   LDY   #$0F
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
   LDA   #$04

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$22
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$21
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
   JSR   test_flat_static_without_null_byte
   LDY   #$11
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
   LDY   #$13
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
   LDA   #$05

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$22
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$21
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
   JSR   test_nested_static_without_null_byte
   LDY   #$15
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
   LDY   #$17
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
   LDA   #$06

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$22
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$21
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
   JSR   test_flat_auto_without_null_byte
   LDY   #$19
   STA   (FP),Y
   TXA
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lnot_true@126
   LDA   #$00
   JMP   .lnot_end@127
.lnot_true@126:
   LDA   #$01
.lnot_end@127:
   LDY   #$1B
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
   LDA   #$07

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$22
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$21
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
   JSR   test_nested_auto_without_null_byte
   LDY   #$1D
   STA   (FP),Y
   TXA
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lnot_true@128
   LDA   #$00
   JMP   .lnot_end@129
.lnot_true@128:
   LDA   #$01
.lnot_end@129:
   LDY   #$1F
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
   LDA   #$08

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$22
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
.if_end@71:
   LDX   #$00
   TXA

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$22
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$21
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
   DC.B  $79, $65, $73, $6E, $6F, $00

str@1:
   DC.B  $79, $65, $73, $6E, $6F, $00

str@2:
   DC.B  $6E, $6F, $00

str@3:
   DC.B  $6F, $6B, $00

str@4:
   DC.B  $79, $65, $73, $6E, $6F, $00

str@5:
   DC.B  $79, $65, $73, $6E, $6F, $00

str@6:
   DC.B  $6E, $6F, $00

str@7:
   DC.B  $6F, $6B, $00

@2.flat:
   DC.B  $64, $6F, $67, $00

@3.nested:
   DC.B  $79, $65, $73, $00
   DC.B  $79, $75, $70, $00

@6.letters:
   DC.B  $61, $62, $63, $64

nested:
   DC.B  $79, $65, $73
   DC.B  $6E, $6F, $00
   DC.B  $6F, $6B, $00
