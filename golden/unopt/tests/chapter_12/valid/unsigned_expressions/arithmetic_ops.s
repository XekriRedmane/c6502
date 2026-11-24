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
   ADC   #$05
   LDY   #$01
   STA   (FP),Y
   LDA   ui_a+1
   ADC   #$80
   INY
   STA   (FP),Y
   LDA   ui_a+2
   ADC   #$00
   INY
   STA   (FP),Y
   LDA   ui_a+3
   ADC   #$00
   INY
   STA   (FP),Y
   CMP   #$00
   BNE   .thread@0
   DEY
   LDA   (FP),Y
   BNE   .thread@0
   DEY
   LDA   (FP),Y
   CMP   #$80
   BNE   .thread@0
   DEY
   LDA   (FP),Y
   CMP   #$0F
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
   CMP   #$7C
   BNE   .thread@1
   DEY
   LDA   (FP),Y
   CMP   #$18
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

multiplication:
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
   STA   HARGS
   LDA   ui_a+1
   STA   HARGS+1
   LDA   ui_a+2
   STA   HARGS+2
   LDA   ui_a+3
   STA   HARGS+3
   LDA   ui_b
   STA   HARGS+4
   LDA   ui_b+1
   STA   HARGS+5
   LDA   ui_b+2
   STA   HARGS+6
   LDA   ui_b+3
   STA   HARGS+7
   JSR   mul32
   LDA   HARGS+8
   LDY   #$01
   STA   (FP),Y
   LDA   HARGS+9
   INY
   STA   (FP),Y
   LDA   HARGS+10
   INY
   STA   (FP),Y
   LDA   HARGS+11
   INY
   STA   (FP),Y
   CMP   #$00
   BNE   .thread@2
   DEY
   LDA   (FP),Y
   BNE   .thread@2
   DEY
   LDA   (FP),Y
   CMP   #$C0
   BNE   .thread@2
   DEY
   LDA   (FP),Y
   BEQ   .cmp_true@6
.thread@2:
   LDA   #$00
   JMP   .cmp_end@7
.cmp_true@6:
   LDA   #$01
.cmp_end@7:
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

division:
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

   LDA   ui_a
   STA   HARGS
   LDA   ui_a+1
   STA   HARGS+1
   LDA   ui_a+2
   STA   HARGS+2
   LDA   ui_a+3
   STA   HARGS+3
   LDA   ui_b
   STA   HARGS+4
   LDA   ui_b+1
   STA   HARGS+5
   LDA   ui_b+2
   STA   HARGS+6
   LDA   ui_b+3
   STA   HARGS+7
   JSR   udivmod32
   LDA   HARGS+8
   LDY   #$01
   STA   (FP),Y
   LDA   HARGS+9
   INY
   STA   (FP),Y
   LDA   HARGS+10
   INY
   STA   (FP),Y
   LDA   HARGS+11
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   ORA   #$00
   BPL   .sx_done@10
   LDA   #$FF
.sx_done@10:
   LDY   #$07
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$04
   LDA   (FP),Y
   LDY   #$08
   CMP   (FP),Y
   BNE   .thread@3
   LDY   #$03
   LDA   (FP),Y
   LDY   #$07
   CMP   (FP),Y
   BNE   .thread@3
   LDY   #$02
   LDA   (FP),Y
   LDY   #$06
   CMP   (FP),Y
   BNE   .thread@3
   LDY   #$01
   LDA   (FP),Y
   LDY   #$05
   CMP   (FP),Y
   BEQ   .cmp_true@11
.thread@3:
   LDA   #$00
   JMP   .cmp_end@12
.cmp_true@11:
   LDA   #$01
.cmp_end@12:
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

division_large_dividend:
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

   LDA   ui_a
   STA   HARGS
   LDA   ui_a+1
   STA   HARGS+1
   LDA   ui_a+2
   STA   HARGS+2
   LDA   ui_a+3
   STA   HARGS+3
   LDA   ui_b
   STA   HARGS+4
   LDA   ui_b+1
   STA   HARGS+5
   LDA   ui_b+2
   STA   HARGS+6
   LDA   ui_b+3
   STA   HARGS+7
   JSR   udivmod32
   LDA   HARGS+8
   LDY   #$01
   STA   (FP),Y
   LDA   HARGS+9
   INY
   STA   (FP),Y
   LDA   HARGS+10
   INY
   STA   (FP),Y
   LDA   HARGS+11
   INY
   STA   (FP),Y
   LDA   #$02
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   ORA   #$00
   BPL   .sx_done@15
   LDA   #$FF
.sx_done@15:
   LDY   #$07
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$04
   LDA   (FP),Y
   LDY   #$08
   CMP   (FP),Y
   BNE   .thread@4
   LDY   #$03
   LDA   (FP),Y
   LDY   #$07
   CMP   (FP),Y
   BNE   .thread@4
   LDY   #$02
   LDA   (FP),Y
   LDY   #$06
   CMP   (FP),Y
   BNE   .thread@4
   LDY   #$01
   LDA   (FP),Y
   LDY   #$05
   CMP   (FP),Y
   BEQ   .cmp_true@16
.thread@4:
   LDA   #$00
   JMP   .cmp_end@17
.cmp_true@16:
   LDA   #$01
.cmp_end@17:
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

division_by_literal:
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
   STA   HARGS
   LDA   ul_a+1
   STA   HARGS+1
   LDA   ul_a+2
   STA   HARGS+2
   LDA   ul_a+3
   STA   HARGS+3
   LDA   ul_a+4
   STA   HARGS+4
   LDA   ul_a+5
   STA   HARGS+5
   LDA   ul_a+6
   STA   HARGS+6
   LDA   ul_a+7
   STA   HARGS+7
   LDA   #$05
   STA   HARGS+8
   LDA   #$00
   STA   HARGS+9
   LDY   #$00
   STY   HARGS+10
   STY   HARGS+11
   STY   HARGS+12
   STY   HARGS+13
   STY   HARGS+14
   STA   HARGS+15
   JSR   udivmod64
   LDA   HARGS+16
   LDY   #$01
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
   CMP   #$00
   BNE   .thread@5
   DEY
   LDA   (FP),Y
   BNE   .thread@5
   DEY
   LDA   (FP),Y
   BNE   .thread@5
   DEY
   LDA   (FP),Y
   BNE   .thread@5
   DEY
   LDA   (FP),Y
   BNE   .thread@5
   DEY
   LDA   (FP),Y
   CMP   #$33
   BNE   .thread@5
   DEY
   LDA   (FP),Y
   CMP   #$33
   BNE   .thread@5
   DEY
   LDA   (FP),Y
   CMP   #$33
   BEQ   .cmp_true@19
.thread@5:
   LDA   #$00
   JMP   .cmp_end@20
.cmp_true@19:
   LDA   #$01
.cmp_end@20:
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

remaind:
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

   LDA   ul_b
   STA   HARGS
   LDA   ul_b+1
   STA   HARGS+1
   LDA   ul_b+2
   STA   HARGS+2
   LDA   ul_b+3
   STA   HARGS+3
   LDA   ul_b+4
   STA   HARGS+4
   LDA   ul_b+5
   STA   HARGS+5
   LDA   ul_b+6
   STA   HARGS+6
   LDA   ul_b+7
   STA   HARGS+7
   LDA   ul_a
   STA   HARGS+8
   LDA   ul_a+1
   STA   HARGS+9
   LDA   ul_a+2
   STA   HARGS+10
   LDA   ul_a+3
   STA   HARGS+11
   LDA   ul_a+4
   STA   HARGS+12
   LDA   ul_a+5
   STA   HARGS+13
   LDA   ul_a+6
   STA   HARGS+14
   LDA   ul_a+7
   STA   HARGS+15
   JSR   udivmod64
   LDA   HARGS+24
   LDY   #$01
   STA   (FP),Y
   LDA   HARGS+25
   INY
   STA   (FP),Y
   LDA   HARGS+26
   INY
   STA   (FP),Y
   LDA   HARGS+27
   INY
   STA   (FP),Y
   LDA   HARGS+28
   INY
   STA   (FP),Y
   LDA   HARGS+29
   INY
   STA   (FP),Y
   LDA   HARGS+30
   INY
   STA   (FP),Y
   LDA   HARGS+31
   INY
   STA   (FP),Y
   CMP   #$00
   BNE   .thread@6
   DEY
   LDA   (FP),Y
   BNE   .thread@6
   DEY
   LDA   (FP),Y
   BNE   .thread@6
   DEY
   LDA   (FP),Y
   BNE   .thread@6
   DEY
   LDA   (FP),Y
   BNE   .thread@6
   DEY
   LDA   (FP),Y
   BNE   .thread@6
   DEY
   LDA   (FP),Y
   BNE   .thread@6
   DEY
   LDA   (FP),Y
   CMP   #$05
   BEQ   .cmp_true@22
.thread@6:
   LDA   #$00
   JMP   .cmp_end@23
.cmp_true@22:
   LDA   #$01
.cmp_end@23:
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

complement:
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

   LDA   ui_a
   EOR   #$FF
   LDY   #$01
   STA   (FP),Y
   LDA   ui_a+1
   EOR   #$FF
   INY
   STA   (FP),Y
   LDA   ui_a+2
   EOR   #$FF
   INY
   STA   (FP),Y
   LDA   ui_a+3
   EOR   #$FF
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   ORA   #$00
   BPL   .sx_done@26
   LDA   #$FF
.sx_done@26:
   LDY   #$07
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$04
   LDA   (FP),Y
   LDY   #$08
   CMP   (FP),Y
   BNE   .thread@7
   LDY   #$03
   LDA   (FP),Y
   LDY   #$07
   CMP   (FP),Y
   BNE   .thread@7
   LDY   #$02
   LDA   (FP),Y
   LDY   #$06
   CMP   (FP),Y
   BNE   .thread@7
   LDY   #$01
   LDA   (FP),Y
   LDY   #$05
   CMP   (FP),Y
   BEQ   .cmp_true@27
.thread@7:
   LDA   #$00
   JMP   .cmp_end@28
.cmp_true@27:
   LDA   #$01
.cmp_end@28:
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

   LDA   #$0A
   STA   ui_a
   LDA   #$00
   STA   ui_a+1
   STA   ui_a+2
   STA   ui_a+3
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
   BEQ   .lnot_true@30
   LDA   #$00
   JMP   .lnot_end@31
.lnot_true@30:
   LDA   #$01
.lnot_end@31:
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
.if_end@0:
   LDA   #$80
   STA   ul_a+1
   LDA   #$FF
   STA   ul_a+2
   STA   ul_a+3
   LDA   #$E8
   STA   ul_b
   LDA   #$03
   STA   ul_b+1
   LDA   #$00
   STA   ul_a
   STA   ul_a+4
   STA   ul_a+5
   STA   ul_a+6
   STA   ul_a+7
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
   BEQ   .lnot_true@32
   LDA   #$00
   JMP   .lnot_end@33
.lnot_true@32:
   LDA   #$01
.lnot_end@33:
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
.if_end@1:
   LDA   #$40
   STA   ui_a+1
   LDA   #$03
   STA   ui_b
   LDA   #$00
   STA   ui_a
   STA   ui_a+2
   STA   ui_a+3
   STA   ui_b+1
   STA   ui_b+2
   STA   ui_b+3
   JSR   multiplication
   LDY   #$09
   STA   (FP),Y
   TXA
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lnot_true@34
   LDA   #$00
   JMP   .lnot_end@35
.lnot_true@34:
   LDA   #$01
.lnot_end@35:
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
.if_end@2:
   LDA   #$64
   STA   ui_a
   LDA   #$FE
   STA   ui_b
   LDA   #$FF
   STA   ui_b+1
   LDA   #$00
   STA   ui_a+1
   STA   ui_a+2
   STA   ui_a+3
   STA   ui_b+2
   STA   ui_b+3
   JSR   division
   LDY   #$0D
   STA   (FP),Y
   TXA
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lnot_true@36
   LDA   #$00
   JMP   .lnot_end@37
.lnot_true@36:
   LDA   #$01
.lnot_end@37:
   LDY   #$0F
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
.if_end@3:
   LDA   #$FE
   STA   ui_a
   LDA   #$FF
   STA   ui_a+1
   STA   ui_b
   LDA   #$7F
   STA   ui_b+1
   LDA   #$00
   STA   ui_a+2
   STA   ui_a+3
   STA   ui_b+2
   STA   ui_b+3
   JSR   division_large_dividend
   LDY   #$11
   STA   (FP),Y
   TXA
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lnot_true@38
   LDA   #$00
   JMP   .lnot_end@39
.lnot_true@38:
   LDA   #$01
.lnot_end@39:
   LDY   #$13
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
.if_end@4:
   LDA   #$FF
   STA   ul_a
   STA   ul_a+1
   STA   ul_a+2
   LDA   #$00
   STA   ul_a+3
   STA   ul_a+4
   STA   ul_a+5
   STA   ul_a+6
   STA   ul_a+7
   JSR   division_by_literal
   LDY   #$15
   STA   (FP),Y
   TXA
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lnot_true@40
   LDA   #$00
   JMP   .lnot_end@41
.lnot_true@40:
   LDA   #$01
.lnot_end@41:
   LDY   #$17
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
.if_end@5:
   LDA   #$64
   STA   ul_a
   LDA   #$A5
   STA   ul_b
   LDA   #$FF
   STA   ul_b+1
   STA   ul_b+2
   STA   ul_b+3
   LDA   #$00
   STA   ul_a+1
   STA   ul_a+2
   STA   ul_a+3
   STA   ul_a+4
   STA   ul_a+5
   STA   ul_a+6
   STA   ul_a+7
   STA   ul_b+4
   STA   ul_b+5
   STA   ul_b+6
   STA   ul_b+7
   JSR   remaind
   LDY   #$19
   STA   (FP),Y
   TXA
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lnot_true@42
   LDA   #$00
   JMP   .lnot_end@43
.lnot_true@42:
   LDA   #$01
.lnot_end@43:
   LDY   #$1B
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
.if_end@6:
   LDA   #$FF
   STA   ui_a
   STA   ui_a+1
   LDA   #$00
   STA   ui_a+2
   STA   ui_a+3
   JSR   complement
   LDY   #$1D
   STA   (FP),Y
   TXA
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lnot_true@44
   LDA   #$00
   JMP   .lnot_end@45
.lnot_true@44:
   LDA   #$01
.lnot_end@45:
   LDY   #$1F
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
.if_end@7:
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

ui_a:
   DS.B  4

ui_b:
   DS.B  4

ul_a:
   DS.B  8

ul_b:
   DS.B  8
