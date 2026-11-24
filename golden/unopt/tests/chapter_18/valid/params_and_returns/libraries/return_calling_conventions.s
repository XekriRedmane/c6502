SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24

return_int_struct:
   SUBROUTINE

   ; prologue: 2 arg bytes, 8 local bytes
   SEC
   LDA   SSP
   SBC   #$0A
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$09
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
   LDY   #$04
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
   LDA   #$02
   LDY   #$06
   STA   (FP),Y
   LDY   #$04
   LDA   (FP),Y
   CLC
   ADC   #$02
   LDY   #$07
   STA   (FP),Y
   LDY   #$05
   LDA   (FP),Y
   ADC   #$00
   LDY   #$08
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$06
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$0B
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

   ; epilogue
   CLC
   LDA   FP
   ADC   #$0C
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$09
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   RTS

return_two_int_struct:
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
   LDA   #$0A
   LDY   #$00
   STA   (DPTR),Y
   LDA   #$00
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
   LDA   #$31
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
   LDA   #$32
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
   LDA   #$33
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
   LDA   #$34
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
   LDA   #$35
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
   LDA   #$36
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
   LDA   #$37
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
   LDA   #$38
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$1F
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

return_double_struct:
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
   LDA   #$28
   INY
   STA   (DPTR),Y
   LDA   #$59
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

return_two_double_struct:
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
   LDA   #$9A
   LDY   #$00
   STA   (DPTR),Y
   LDA   #$99
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
   LDA   #$21
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
   LDA   #$33
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
   LDA   #$1F
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

return_mixed:
   SUBROUTINE

   ; prologue: 2 arg bytes, 19 local bytes
   SEC
   LDA   SSP
   SBC   #$15
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$14
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
   LDY   #$0C
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
   LDA   #$24
   INY
   STA   (DPTR),Y
   LDA   #$40
   INY
   STA   (DPTR),Y
   LDY   #$0C
   LDA   (FP),Y
   CLC
   ADC   #$08
   LDY   #$0E
   STA   (FP),Y
   DEY
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
   LDA   #$61
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$0C
   LDA   (FP),Y
   CLC
   ADC   #$09
   LDY   #$10
   STA   (FP),Y
   LDY   #$0D
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
   LDA   #$62
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$0C
   LDA   (FP),Y
   CLC
   ADC   #$0A
   LDY   #$12
   STA   (FP),Y
   LDY   #$0D
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
   LDA   #$00
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$16
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

   ; epilogue
   CLC
   LDA   FP
   ADC   #$17
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$14
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   RTS

return_mixed2:
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

   LDA   #$7F
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
   LDA   #$55
   INY
   STA   (DPTR),Y
   LDA   #$7A
   INY
   STA   (DPTR),Y
   LDA   #$65
   INY
   STA   (DPTR),Y
   LDA   #$04
   INY
   STA   (DPTR),Y
   LDA   #$7E
   INY
   STA   (DPTR),Y
   LDA   #$2E
   INY
   STA   (DPTR),Y
   LDA   #$49
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

return_on_stack:
   SUBROUTINE

   ; prologue: 2 arg bytes, 29 local bytes
   SEC
   LDA   SSP
   SBC   #$1F
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$1E
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
   LDA   #$F4
   INY
   STA   (DPTR),Y
   LDA   #$3F
   INY
   STA   (DPTR),Y
   LDY   #$12
   LDA   (FP),Y
   CLC
   ADC   #$08
   LDY   #$14
   STA   (FP),Y
   DEY
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
   LDA   #$78
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$12
   LDA   (FP),Y
   CLC
   ADC   #$09
   LDY   #$16
   STA   (FP),Y
   LDY   #$13
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
   LDA   #$79
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$12
   LDA   (FP),Y
   CLC
   ADC   #$0A
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
   LDA   #$00
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$12
   LDA   (FP),Y
   CLC
   ADC   #$0B
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
   LDY   #$12
   LDA   (FP),Y
   CLC
   ADC   #$0F
   LDY   #$1C
   STA   (FP),Y
   LDY   #$13
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
   LDA   #$2C
   LDY   #$00
   STA   (DPTR),Y
   LDA   #$00
   INY
   STA   (DPTR),Y
   LDY   #$20
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
   LDY   #$11
   LDA   (FP),Y
   DEY
   STA   (DPTR),Y

   ; epilogue
   CLC
   LDA   FP
   ADC   #$21
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$1E
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   RTS

leaf_call:
   SUBROUTINE

   ; prologue: 17 arg bytes, 55 local bytes
   SEC
   LDA   SSP
   SBC   #$39
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$38
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
   ADC   #$3A
   LDY   #$01
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
   LDY   #$03
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CMP   #$5F
   BNE   .cmp_true@0
   LDA   #$00
   JMP   .cmp_end@1
.cmp_true@0:
   LDA   #$01
.cmp_end@1:
   LDY   #$06
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
   CLC
   LDA   FP
   ADC   #$3A
   LDY   #$08
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
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
   LDA   #$00
   STA   HARGS
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
   LDY   #$0D
   STA   (FP),Y
   LDA   HARGS
   DEY
   STA   (FP),Y
   LDY   #$0A
   LDA   (FP),Y
   CLC
   LDY   #$0C
   ADC   (FP),Y
   LDY   #$0E
   STA   (FP),Y
   LDY   #$0B
   LDA   (FP),Y
   LDY   #$0D
   ADC   (FP),Y
   LDY   #$0F
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$10
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$11
   STA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@4
   DEY
   LDA   (FP),Y
   CMP   #$05
   BNE   .cmp_true@4
   LDA   #$00
   JMP   .cmp_end@5
.cmp_true@4:
   LDA   #$01
.cmp_end@5:
   LDY   #$12
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
   LDY   #$14
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@5:
   LDY   #$14
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@1
   JMP   .or_true@2
.lb_skip@1:
   CLC
   LDA   FP
   ADC   #$3A
   LDY   #$16
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$01
   LDY   #$18
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$19
   STA   (FP),Y
   LDA   #$01
   STA   HARGS
   LDA   #$00
   LDX   #$08
   LSR   HARGS
.mul_loop@7:
   BCC   .mul_skip@8
   CLC
   ADC   #$02
.mul_skip@8:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@7
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
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$1E
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$1F
   STA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@9
   DEY
   LDA   (FP),Y
   CMP   #$06
   BNE   .cmp_true@9
   LDA   #$00
   JMP   .cmp_end@10
.cmp_true@9:
   LDA   #$01
.cmp_end@10:
   LDY   #$20
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
   LDY   #$22
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@3:
   LDY   #$22
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@0
   JMP   .or_true@0
.lb_skip@0:
   CLC
   LDA   FP
   ADC   #$3A
   LDY   #$24
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$01
   LDY   #$26
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$27
   STA   (FP),Y
   LDA   #$02
   STA   HARGS
   LDA   #$00
   LDX   #$08
   LSR   HARGS
.mul_loop@12:
   BCC   .mul_skip@13
   CLC
   ADC   #$02
.mul_skip@13:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@12
   LDY   #$29
   STA   (FP),Y
   LDA   HARGS
   DEY
   STA   (FP),Y
   LDY   #$26
   LDA   (FP),Y
   CLC
   LDY   #$28
   ADC   (FP),Y
   LDY   #$2A
   STA   (FP),Y
   LDY   #$27
   LDA   (FP),Y
   LDY   #$29
   ADC   (FP),Y
   LDY   #$2B
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$2C
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$2D
   STA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@14
   DEY
   LDA   (FP),Y
   CMP   #$07
   BNE   .cmp_true@14
   LDA   #$00
   JMP   .cmp_end@15
.cmp_true@14:
   LDA   #$01
.cmp_end@15:
   LDY   #$2E
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
   LDY   #$30
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@1:
   LDY   #$30
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
   ADC   #$4A
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$38
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
   LDY   #$42
   LDA   (FP),Y
   BNE   .cmp_true@17
   DEY
   LDA   (FP),Y
   CMP   #$70
   BNE   .cmp_true@17
   LDA   #$00
   JMP   .cmp_end@18
.cmp_true@17:
   LDA   #$01
.cmp_end@18:
   LDY   #$32
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@7
   LDY   #$4A
   LDA   (FP),Y
   CMP   #$40
   BNE   .cmp_true@20
   DEY
   LDA   (FP),Y
   CMP   #$12
   BNE   .cmp_true@20
   DEY
   LDA   (FP),Y
   CMP   #$3D
   BNE   .cmp_true@20
   DEY
   LDA   (FP),Y
   CMP   #$70
   BNE   .cmp_true@20
   DEY
   LDA   (FP),Y
   CMP   #$A3
   BNE   .cmp_true@20
   DEY
   LDA   (FP),Y
   CMP   #$D7
   BNE   .cmp_true@20
   DEY
   LDA   (FP),Y
   CMP   #$0A
   BNE   .cmp_true@20
   DEY
   LDA   (FP),Y
   CMP   #$3D
   BNE   .cmp_true@20
   LDA   #$00
   JMP   .cmp_end@21
.cmp_true@20:
   LDA   #$01
.cmp_end@21:
   LDY   #$34
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
   LDY   #$36
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@8:
   LDY   #$36
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
.if_end@9:
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
   LDY   #$38
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

pass_and_return_regs:
   SUBROUTINE

   ; prologue: 40 arg bytes, 129 local bytes
   SEC
   LDA   SSP
   SBC   #$83
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$82
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
   LDA   #$7A
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
   LDA   #$79
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
   LDA   #$78
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
   LDA   #$77
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
   LDA   #$76
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
   LDA   #$75
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
   LDA   #$74
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
   LDA   #$11
   LDY   #$19
   STA   (FP),Y
.clear@10:
   SEC
   SBC   #$01
   LDY   #$19
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   CLC
   LDA   FP
   ADC   #$1C
   LDY   #$2D
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   LDY   #$1A
   ADC   (FP),Y
   LDY   #$2F
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$1B
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
   LDY   #$19
   LDA   (FP),Y
   BNE   .clear@10
   CLC
   LDA   FP
   ADC   #$90
   LDY   #$31
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
   LDY   #$33
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   CLC
   LDA   FP
   ADC   #$90
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
   LDY   #$04
   LDA   (DPTR),Y
   LDY   #$3E
   STA   (FP),Y
   LDY   #$05
   LDA   (DPTR),Y
   LDY   #$3F
   STA   (FP),Y
   LDY   #$06
   LDA   (DPTR),Y
   LDY   #$40
   STA   (FP),Y
   LDY   #$07
   LDA   (DPTR),Y
   LDY   #$41
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$11
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDY   #$9B
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$9C
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$9D
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$9E
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   LDY   #$9F
   LDA   (FP),Y
   LDY   #$05
   STA   (SSP),Y
   LDY   #$A0
   LDA   (FP),Y
   LDY   #$06
   STA   (SSP),Y
   LDY   #$A1
   LDA   (FP),Y
   LDY   #$07
   STA   (SSP),Y
   LDY   #$34
   LDA   (FP),Y
   LDY   #$08
   STA   (SSP),Y
   LDY   #$35
   LDA   (FP),Y
   LDY   #$09
   STA   (SSP),Y
   LDY   #$3A
   LDA   (FP),Y
   LDY   #$0A
   STA   (SSP),Y
   LDY   #$3B
   LDA   (FP),Y
   LDY   #$0B
   STA   (SSP),Y
   LDY   #$3C
   LDA   (FP),Y
   LDY   #$0C
   STA   (SSP),Y
   LDY   #$3D
   LDA   (FP),Y
   LDY   #$0D
   STA   (SSP),Y
   LDY   #$3E
   LDA   (FP),Y
   LDY   #$0E
   STA   (SSP),Y
   LDY   #$3F
   LDA   (FP),Y
   LDY   #$0F
   STA   (SSP),Y
   LDY   #$40
   LDA   (FP),Y
   LDY   #$10
   STA   (SSP),Y
   LDY   #$41
   LDA   (FP),Y
   LDY   #$11
   STA   (SSP),Y
   JSR   leaf_call
   LDY   #$42
   STA   (FP),Y
   TXA
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lnot_true@23
   LDA   #$00
   JMP   .lnot_end@24
.lnot_true@23:
   LDA   #$01
.lnot_end@24:
   LDY   #$44
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@6
   JMP   .if_end@11
.lb_skip@6:
   CLC
   LDA   FP
   ADC   #$1C
   LDY   #$46
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$0F
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
   LDA   #$01
   LDY   #$00
   STA   (DPTR),Y
   LDA   #$00
   INY
   STA   (DPTR),Y
   LDY   #$84
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$1C
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$1D
   LDA   (FP),Y
   LDY   #$01
   STA   (DPTR),Y
   LDY   #$1E
   LDA   (FP),Y
   LDY   #$02
   STA   (DPTR),Y
   LDY   #$1F
   LDA   (FP),Y
   LDY   #$03
   STA   (DPTR),Y
   LDY   #$20
   LDA   (FP),Y
   LDY   #$04
   STA   (DPTR),Y
   LDY   #$21
   LDA   (FP),Y
   LDY   #$05
   STA   (DPTR),Y
   LDY   #$22
   LDA   (FP),Y
   LDY   #$06
   STA   (DPTR),Y
   LDY   #$23
   LDA   (FP),Y
   LDY   #$07
   STA   (DPTR),Y
   LDY   #$24
   LDA   (FP),Y
   LDY   #$08
   STA   (DPTR),Y
   LDY   #$25
   LDA   (FP),Y
   LDY   #$09
   STA   (DPTR),Y
   LDY   #$26
   LDA   (FP),Y
   LDY   #$0A
   STA   (DPTR),Y
   LDY   #$27
   LDA   (FP),Y
   LDY   #$0B
   STA   (DPTR),Y
   LDY   #$28
   LDA   (FP),Y
   LDY   #$0C
   STA   (DPTR),Y
   LDY   #$29
   LDA   (FP),Y
   LDY   #$0D
   STA   (DPTR),Y
   LDY   #$2A
   LDA   (FP),Y
   LDY   #$0E
   STA   (DPTR),Y
   LDY   #$2B
   LDA   (FP),Y
   LDY   #$0F
   STA   (DPTR),Y
   LDY   #$2C
   LDA   (FP),Y
   LDY   #$10
   STA   (DPTR),Y

   ; epilogue
   CLC
   LDA   FP
   ADC   #$AB
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$82
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   RTS
.if_end@11:
   LDY   #$87
   LDA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@25
   DEY
   LDA   (FP),Y
   CMP   #$06
   BNE   .cmp_true@25
   LDA   #$00
   JMP   .cmp_end@26
.cmp_true@25:
   LDA   #$01
.cmp_end@26:
   LDY   #$4A
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@18
   LDY   #$8F
   LDA   (FP),Y
   CMP   #$40
   BNE   .cmp_true@28
   DEY
   LDA   (FP),Y
   CMP   #$10
   BNE   .cmp_true@28
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@28
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@28
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@28
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@28
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@28
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@28
   LDA   #$00
   JMP   .cmp_end@29
.cmp_true@28:
   LDA   #$01
.cmp_end@29:
   LDY   #$4C
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
   LDY   #$4E
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@19:
   LDY   #$4E
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@16
   LDY   #$9A
   LDA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@31
   DEY
   LDA   (FP),Y
   CMP   #$05
   BNE   .cmp_true@31
   LDA   #$00
   JMP   .cmp_end@32
.cmp_true@31:
   LDA   #$01
.cmp_end@32:
   LDY   #$50
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
   LDY   #$52
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@17:
   LDY   #$52
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@14
   LDA   #$4D
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   ORA   #$00
   BPL   .sx_done@35
   LDA   #$FF
.sx_done@35:
   LDY   #$56
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$A5
   LDA   (FP),Y
   LDY   #$57
   CMP   (FP),Y
   BNE   .cmp_true@36
   LDY   #$A4
   LDA   (FP),Y
   LDY   #$56
   CMP   (FP),Y
   BNE   .cmp_true@36
   LDY   #$A3
   LDA   (FP),Y
   LDY   #$55
   CMP   (FP),Y
   BNE   .cmp_true@36
   LDY   #$A2
   LDA   (FP),Y
   LDY   #$54
   CMP   (FP),Y
   BNE   .cmp_true@36
   LDA   #$00
   JMP   .cmp_end@37
.cmp_true@36:
   LDA   #$01
.cmp_end@37:
   LDY   #$58
   STA   (FP),Y
   LDA   #$00
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
   LDY   #$5A
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@15:
   LDY   #$5A
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@12
   LDY   #$AB
   LDA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@39
   DEY
   LDA   (FP),Y
   CMP   #$63
   BNE   .cmp_true@39
   LDA   #$00
   JMP   .cmp_end@40
.cmp_true@39:
   LDA   #$01
.cmp_end@40:
   LDY   #$5C
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
   LDY   #$5E
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@13:
   LDY   #$5E
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@5
   JMP   .if_end@20
.lb_skip@5:
   CLC
   LDA   FP
   ADC   #$1C
   LDY   #$60
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$0F
   LDY   #$62
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$63
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
   LDY   #$84
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$1C
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$1D
   LDA   (FP),Y
   LDY   #$01
   STA   (DPTR),Y
   LDY   #$1E
   LDA   (FP),Y
   LDY   #$02
   STA   (DPTR),Y
   LDY   #$1F
   LDA   (FP),Y
   LDY   #$03
   STA   (DPTR),Y
   LDY   #$20
   LDA   (FP),Y
   LDY   #$04
   STA   (DPTR),Y
   LDY   #$21
   LDA   (FP),Y
   LDY   #$05
   STA   (DPTR),Y
   LDY   #$22
   LDA   (FP),Y
   LDY   #$06
   STA   (DPTR),Y
   LDY   #$23
   LDA   (FP),Y
   LDY   #$07
   STA   (DPTR),Y
   LDY   #$24
   LDA   (FP),Y
   LDY   #$08
   STA   (DPTR),Y
   LDY   #$25
   LDA   (FP),Y
   LDY   #$09
   STA   (DPTR),Y
   LDY   #$26
   LDA   (FP),Y
   LDY   #$0A
   STA   (DPTR),Y
   LDY   #$27
   LDA   (FP),Y
   LDY   #$0B
   STA   (DPTR),Y
   LDY   #$28
   LDA   (FP),Y
   LDY   #$0C
   STA   (DPTR),Y
   LDY   #$29
   LDA   (FP),Y
   LDY   #$0D
   STA   (DPTR),Y
   LDY   #$2A
   LDA   (FP),Y
   LDY   #$0E
   STA   (DPTR),Y
   LDY   #$2B
   LDA   (FP),Y
   LDY   #$0F
   STA   (DPTR),Y
   LDY   #$2C
   LDA   (FP),Y
   LDY   #$10
   STA   (DPTR),Y

   ; epilogue
   CLC
   LDA   FP
   ADC   #$AB
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$82
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   RTS
.if_end@20:
   CLC
   LDA   FP
   ADC   #$A6
   LDY   #$64
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
   LDY   #$66
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$67
   STA   (FP),Y
   LDY   #$02
   LDA   (DPTR),Y
   LDY   #$68
   STA   (FP),Y
   LDY   #$03
   LDA   (DPTR),Y
   LDY   #$69
   STA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@42
   DEY
   LDA   (FP),Y
   CMP   #$08
   BNE   .cmp_true@42
   DEY
   LDA   (FP),Y
   CMP   #$AA
   BNE   .cmp_true@42
   DEY
   LDA   (FP),Y
   CMP   #$52
   BNE   .cmp_true@42
   LDA   #$00
   JMP   .cmp_end@43
.cmp_true@42:
   LDA   #$01
.cmp_end@43:
   LDY   #$6A
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@4
   JMP   .if_end@21
.lb_skip@4:
   CLC
   LDA   FP
   ADC   #$1C
   LDY   #$6C
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$0F
   LDY   #$6E
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$6F
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
   LDY   #$84
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$1C
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$1D
   LDA   (FP),Y
   LDY   #$01
   STA   (DPTR),Y
   LDY   #$1E
   LDA   (FP),Y
   LDY   #$02
   STA   (DPTR),Y
   LDY   #$1F
   LDA   (FP),Y
   LDY   #$03
   STA   (DPTR),Y
   LDY   #$20
   LDA   (FP),Y
   LDY   #$04
   STA   (DPTR),Y
   LDY   #$21
   LDA   (FP),Y
   LDY   #$05
   STA   (DPTR),Y
   LDY   #$22
   LDA   (FP),Y
   LDY   #$06
   STA   (DPTR),Y
   LDY   #$23
   LDA   (FP),Y
   LDY   #$07
   STA   (DPTR),Y
   LDY   #$24
   LDA   (FP),Y
   LDY   #$08
   STA   (DPTR),Y
   LDY   #$25
   LDA   (FP),Y
   LDY   #$09
   STA   (DPTR),Y
   LDY   #$26
   LDA   (FP),Y
   LDY   #$0A
   STA   (DPTR),Y
   LDY   #$27
   LDA   (FP),Y
   LDY   #$0B
   STA   (DPTR),Y
   LDY   #$28
   LDA   (FP),Y
   LDY   #$0C
   STA   (DPTR),Y
   LDY   #$29
   LDA   (FP),Y
   LDY   #$0D
   STA   (DPTR),Y
   LDY   #$2A
   LDA   (FP),Y
   LDY   #$0E
   STA   (DPTR),Y
   LDY   #$2B
   LDA   (FP),Y
   LDY   #$0F
   STA   (DPTR),Y
   LDY   #$2C
   LDA   (FP),Y
   LDY   #$10
   STA   (DPTR),Y

   ; epilogue
   CLC
   LDA   FP
   ADC   #$AB
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$82
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   RTS
.if_end@21:
   CLC
   LDA   FP
   ADC   #$01
   LDY   #$72
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   LDA   #<str@0
   LDY   #$70
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
   INY
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$73
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$70
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$71
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   JSR   strcmp
   LDY   #$74
   STA   (FP),Y
   TXA
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@3
   JMP   .if_end@22
.lb_skip@3:
   CLC
   LDA   FP
   ADC   #$1C
   LDY   #$76
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$0F
   LDY   #$78
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$79
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
   LDY   #$84
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$1C
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$1D
   LDA   (FP),Y
   LDY   #$01
   STA   (DPTR),Y
   LDY   #$1E
   LDA   (FP),Y
   LDY   #$02
   STA   (DPTR),Y
   LDY   #$1F
   LDA   (FP),Y
   LDY   #$03
   STA   (DPTR),Y
   LDY   #$20
   LDA   (FP),Y
   LDY   #$04
   STA   (DPTR),Y
   LDY   #$21
   LDA   (FP),Y
   LDY   #$05
   STA   (DPTR),Y
   LDY   #$22
   LDA   (FP),Y
   LDY   #$06
   STA   (DPTR),Y
   LDY   #$23
   LDA   (FP),Y
   LDY   #$07
   STA   (DPTR),Y
   LDY   #$24
   LDA   (FP),Y
   LDY   #$08
   STA   (DPTR),Y
   LDY   #$25
   LDA   (FP),Y
   LDY   #$09
   STA   (DPTR),Y
   LDY   #$26
   LDA   (FP),Y
   LDY   #$0A
   STA   (DPTR),Y
   LDY   #$27
   LDA   (FP),Y
   LDY   #$0B
   STA   (DPTR),Y
   LDY   #$28
   LDA   (FP),Y
   LDY   #$0C
   STA   (DPTR),Y
   LDY   #$29
   LDA   (FP),Y
   LDY   #$0D
   STA   (DPTR),Y
   LDY   #$2A
   LDA   (FP),Y
   LDY   #$0E
   STA   (DPTR),Y
   LDY   #$2B
   LDA   (FP),Y
   LDY   #$0F
   STA   (DPTR),Y
   LDY   #$2C
   LDA   (FP),Y
   LDY   #$10
   STA   (DPTR),Y

   ; epilogue
   CLC
   LDA   FP
   ADC   #$AB
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$82
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   RTS
.if_end@22:
   LDA   #$64
   LDY   #$7A
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   ORA   #$00
   BPL   .sx_done@46
   LDA   #$FF
.sx_done@46:
   LDY   #$7C
   STA   (FP),Y
   INY
   STA   (FP),Y
   CLC
   LDA   FP
   ADC   #$1C
   INY
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$0B
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
   LDY   #$7A
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$7B
   LDA   (FP),Y
   LDY   #$01
   STA   (DPTR),Y
   LDY   #$7C
   LDA   (FP),Y
   LDY   #$02
   STA   (DPTR),Y
   LDY   #$7D
   LDA   (FP),Y
   LDY   #$03
   STA   (DPTR),Y
   LDY   #$84
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$1C
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$1D
   LDA   (FP),Y
   LDY   #$01
   STA   (DPTR),Y
   LDY   #$1E
   LDA   (FP),Y
   LDY   #$02
   STA   (DPTR),Y
   LDY   #$1F
   LDA   (FP),Y
   LDY   #$03
   STA   (DPTR),Y
   LDY   #$20
   LDA   (FP),Y
   LDY   #$04
   STA   (DPTR),Y
   LDY   #$21
   LDA   (FP),Y
   LDY   #$05
   STA   (DPTR),Y
   LDY   #$22
   LDA   (FP),Y
   LDY   #$06
   STA   (DPTR),Y
   LDY   #$23
   LDA   (FP),Y
   LDY   #$07
   STA   (DPTR),Y
   LDY   #$24
   LDA   (FP),Y
   LDY   #$08
   STA   (DPTR),Y
   LDY   #$25
   LDA   (FP),Y
   LDY   #$09
   STA   (DPTR),Y
   LDY   #$26
   LDA   (FP),Y
   LDY   #$0A
   STA   (DPTR),Y
   LDY   #$27
   LDA   (FP),Y
   LDY   #$0B
   STA   (DPTR),Y
   LDY   #$28
   LDA   (FP),Y
   LDY   #$0C
   STA   (DPTR),Y
   LDY   #$29
   LDA   (FP),Y
   LDY   #$0D
   STA   (DPTR),Y
   LDY   #$2A
   LDA   (FP),Y
   LDY   #$0E
   STA   (DPTR),Y
   LDY   #$2B
   LDA   (FP),Y
   LDY   #$0F
   STA   (DPTR),Y
   LDY   #$2C
   LDA   (FP),Y
   LDY   #$10
   STA   (DPTR),Y

   ; epilogue
   CLC
   LDA   FP
   ADC   #$AB
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$82
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   RTS

str@0:
   DC.B  $7A, $79, $78, $77, $76, $75, $74, $00
