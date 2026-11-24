SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24

addition:
   SUBROUTINE

   ; prologue: 0 arg bytes, 6 local bytes
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

   LDA   ui_a
   CLC
   ADC   ui_b
   LDY   #$01
   STA   (FP),Y
   LDA   ui_a+1
   ADC   ui_b+1
   INY
   STA   (FP),Y
   LDA   ui_a+2
   ADC   ui_b+2
   INY
   STA   (FP),Y
   LDA   ui_a+3
   ADC   ui_b+3
   INY
   STA   (FP),Y
   CMP   #$00
   BNE   .thread@0
   DEY
   LDA   (FP),Y
   BNE   .thread@0
   DEY
   LDA   (FP),Y
   BNE   .thread@0
   DEY
   LDA   (FP),Y
   BEQ   .cmp_true@0
.thread@0:
   LDA   #$00
   JMP   .cmp_end@1
.cmp_true@0:
   LDA   #$01
.cmp_end@1:
   LDY   #$05
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
   ADC   #$08
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
   PLA
   TAX
   PLA
   RTS

subtraction:
   SUBROUTINE

   ; prologue: 0 arg bytes, 10 local bytes
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

   LDA   ul_a
   SEC
   SBC   ul_b
   LDY   #$01
   STA   (FP),Y
   LDA   ul_a+1
   SBC   ul_b+1
   INY
   STA   (FP),Y
   LDA   ul_a+2
   SBC   ul_b+2
   INY
   STA   (FP),Y
   LDA   ul_a+3
   SBC   ul_b+3
   INY
   STA   (FP),Y
   LDA   ul_a+4
   SBC   ul_b+4
   INY
   STA   (FP),Y
   LDA   ul_a+5
   SBC   ul_b+5
   INY
   STA   (FP),Y
   LDA   ul_a+6
   SBC   ul_b+6
   INY
   STA   (FP),Y
   LDA   ul_a+7
   SBC   ul_b+7
   INY
   STA   (FP),Y
   CMP   #$00
   BNE   .thread@1
   DEY
   LDA   (FP),Y
   BNE   .thread@1
   DEY
   LDA   (FP),Y
   BNE   .thread@1
   DEY
   LDA   (FP),Y
   BNE   .thread@1
   DEY
   LDA   (FP),Y
   CMP   #$FF
   BNE   .thread@1
   DEY
   LDA   (FP),Y
   CMP   #$FF
   BNE   .thread@1
   DEY
   LDA   (FP),Y
   CMP   #$FF
   BNE   .thread@1
   DEY
   LDA   (FP),Y
   CMP   #$F6
   BEQ   .cmp_true@3
.thread@1:
   LDA   #$00
   JMP   .cmp_end@4
.cmp_true@3:
   LDA   #$01
.cmp_end@4:
   LDY   #$09
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
   ADC   #$0C
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
   PLA
   TAX
   PLA
   RTS

neg:
   SUBROUTINE

   ; prologue: 0 arg bytes, 10 local bytes
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

   SEC
   LDA   #$00
   SBC   ul_a
   LDY   #$01
   STA   (FP),Y
   LDA   #$00
   SBC   ul_a+1
   INY
   STA   (FP),Y
   LDA   #$00
   SBC   ul_a+2
   INY
   STA   (FP),Y
   LDA   #$00
   SBC   ul_a+3
   INY
   STA   (FP),Y
   LDA   #$00
   SBC   ul_a+4
   INY
   STA   (FP),Y
   LDA   #$00
   SBC   ul_a+5
   INY
   STA   (FP),Y
   LDA   #$00
   SBC   ul_a+6
   INY
   STA   (FP),Y
   LDA   #$00
   SBC   ul_a+7
   INY
   STA   (FP),Y
   CMP   #$00
   BNE   .thread@2
   DEY
   LDA   (FP),Y
   BNE   .thread@2
   DEY
   LDA   (FP),Y
   BNE   .thread@2
   DEY
   LDA   (FP),Y
   BNE   .thread@2
   DEY
   LDA   (FP),Y
   CMP   #$FF
   BNE   .thread@2
   DEY
   LDA   (FP),Y
   CMP   #$FF
   BNE   .thread@2
   DEY
   LDA   (FP),Y
   CMP   #$FF
   BNE   .thread@2
   DEY
   LDA   (FP),Y
   CMP   #$FF
   BEQ   .cmp_true@6
.thread@2:
   LDA   #$00
   JMP   .cmp_end@7
.cmp_true@6:
   LDA   #$01
.cmp_end@7:
   LDY   #$09
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
   ADC   #$0C
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

   LDA   #$FD
   STA   ui_a
   LDA   #$FF
   STA   ui_a+1
   LDA   #$03
   STA   ui_b
   LDA   #$00
   STA   ui_a+2
   STA   ui_a+3
   STA   ui_b+1
   STA   ui_b+2
   STA   ui_b+3
   JSR   addition
   LDY   #$01
   STA   (FP),Y
   TXA
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lnot_true@9
   LDA   #$00
   JMP   .lnot_end@10
.lnot_true@9:
   LDA   #$01
.lnot_end@10:
   LDY   #$03
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
.if_end@0:
   LDA   #$0A
   STA   ul_a
   LDA   #$14
   STA   ul_b
   LDA   #$00
   STA   ul_a+1
   STA   ul_a+2
   STA   ul_a+3
   STA   ul_a+4
   STA   ul_a+5
   STA   ul_a+6
   STA   ul_a+7
   STA   ul_b+1
   STA   ul_b+2
   STA   ul_b+3
   STA   ul_b+4
   STA   ul_b+5
   STA   ul_b+6
   STA   ul_b+7
   JSR   subtraction
   LDY   #$05
   STA   (FP),Y
   TXA
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lnot_true@11
   LDA   #$00
   JMP   .lnot_end@12
.lnot_true@11:
   LDA   #$01
.lnot_end@12:
   LDY   #$07
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
.if_end@1:
   LDA   #$01
   STA   ul_a
   LDA   #$00
   STA   ul_a+1
   STA   ul_a+2
   STA   ul_a+3
   STA   ul_a+4
   STA   ul_a+5
   STA   ul_a+6
   STA   ul_a+7
   JSR   neg
   LDY   #$09
   STA   (FP),Y
   TXA
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lnot_true@13
   LDA   #$00
   JMP   .lnot_end@14
.lnot_true@13:
   LDA   #$01
.lnot_end@14:
   LDY   #$0B
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
.if_end@2:
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

ui_a:
   DS.B  4

ui_b:
   DS.B  4

ul_a:
   DS.B  8

ul_b:
   DS.B  8
