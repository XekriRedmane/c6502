SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24

main:
   SUBROUTINE

   ; prologue: 0 arg bytes, 36 local bytes
   SEC
   LDA   SSP
   SBC   #$26
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$25
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   LDA   u+3
   BNE   .cmp_true@0
   LDA   u+2
   BNE   .cmp_true@0
   LDA   u+1
   CMP   #$80
   BNE   .cmp_true@0
   LDA   u
   CMP   #$0C
   BNE   .cmp_true@0
   LDA   #$00
   JMP   .cmp_end@1
.cmp_true@0:
   LDA   #$01
.cmp_end@1:
   LDY   #$01
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
   ADC   #$26
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$25
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
   SEC
   LDA   #$00
   SBC   #$FE
   LDY   #$03
   STA   (FP),Y
   LDA   #$00
   SBC   #$7F
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$05
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$06
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@3
   LDA   #$00
   JMP   .sx_done@4
.sx_neg@3:
   LDA   #$FF
.sx_done@4:
   LDY   #$07
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   i+3
   CMP   (FP),Y
   BNE   .cmp_true@5
   LDA   i+2
   DEY
   CMP   (FP),Y
   BNE   .cmp_true@5
   LDA   i+1
   DEY
   CMP   (FP),Y
   BNE   .cmp_true@5
   LDA   i
   DEY
   CMP   (FP),Y
   BNE   .cmp_true@5
   LDA   #$00
   JMP   .cmp_end@6
.cmp_true@5:
   LDA   #$01
.cmp_end@6:
   LDY   #$09
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
   ADC   #$26
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$25
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
   SEC
   LDA   #$00
   SBC   #$64
   LDY   #$0B
   STA   (FP),Y
   LDA   #$00
   SBC   #$00
   INY
   STA   (FP),Y
   LDA   #$00
   SBC   #$00
   INY
   STA   (FP),Y
   LDA   #$00
   SBC   #$00
   INY
   STA   (FP),Y
   LDA   #$00
   SBC   #$00
   INY
   STA   (FP),Y
   LDA   #$00
   SBC   #$00
   INY
   STA   (FP),Y
   LDA   #$00
   SBC   #$00
   INY
   STA   (FP),Y
   LDA   #$00
   SBC   #$00
   INY
   STA   (FP),Y
   LDA   l+7
   CMP   (FP),Y
   BNE   .cmp_true@8
   LDA   l+6
   DEY
   CMP   (FP),Y
   BNE   .cmp_true@8
   LDA   l+5
   DEY
   CMP   (FP),Y
   BNE   .cmp_true@8
   LDA   l+4
   DEY
   CMP   (FP),Y
   BNE   .cmp_true@8
   LDA   l+3
   DEY
   CMP   (FP),Y
   BNE   .cmp_true@8
   LDA   l+2
   DEY
   CMP   (FP),Y
   BNE   .cmp_true@8
   LDA   l+1
   DEY
   CMP   (FP),Y
   BNE   .cmp_true@8
   LDA   l
   DEY
   CMP   (FP),Y
   BNE   .cmp_true@8
   LDA   #$00
   JMP   .cmp_end@9
.cmp_true@8:
   LDA   #$01
.cmp_end@9:
   LDY   #$13
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
   ADC   #$26
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$25
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
   LDA   l2+7
   BNE   .cmp_true@11
   LDA   l2+6
   BNE   .cmp_true@11
   LDA   l2+5
   BNE   .cmp_true@11
   LDA   l2+4
   BNE   .cmp_true@11
   LDA   l2+3
   BNE   .cmp_true@11
   LDA   l2+2
   BNE   .cmp_true@11
   LDA   l2+1
   CMP   #$80
   BNE   .cmp_true@11
   LDA   l2
   CMP   #$02
   BNE   .cmp_true@11
   LDA   #$00
   JMP   .cmp_end@12
.cmp_true@11:
   LDA   #$01
.cmp_end@12:
   LDY   #$15
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
   ADC   #$26
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$25
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
   LDA   ul+7
   BNE   .cmp_true@14
   LDA   ul+6
   BNE   .cmp_true@14
   LDA   ul+5
   BNE   .cmp_true@14
   LDA   ul+4
   BNE   .cmp_true@14
   LDA   ul+3
   BNE   .cmp_true@14
   LDA   ul+2
   BNE   .cmp_true@14
   LDA   ul+1
   CMP   #$FF
   BNE   .cmp_true@14
   LDA   ul
   CMP   #$FE
   BNE   .cmp_true@14
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
   BEQ   .if_end@4
   LDX   #$00
   LDA   #$05

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$26
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$25
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
   LDA   ul2+7
   BNE   .cmp_true@17
   LDA   ul2+6
   BNE   .cmp_true@17
   LDA   ul2+5
   BNE   .cmp_true@17
   LDA   ul2+4
   BNE   .cmp_true@17
   LDA   ul2+3
   CMP   #$7F
   BNE   .cmp_true@17
   LDA   ul2+2
   CMP   #$FF
   BNE   .cmp_true@17
   LDA   ul2+1
   CMP   #$FF
   BNE   .cmp_true@17
   LDA   ul2
   CMP   #$F6
   BNE   .cmp_true@17
   LDA   #$00
   JMP   .cmp_end@18
.cmp_true@17:
   LDA   #$01
.cmp_end@18:
   LDY   #$19
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
   ADC   #$26
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$25
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
   SEC
   LDA   #$00
   SBC   #$6A
   LDY   #$1B
   STA   (FP),Y
   LDA   #$00
   SBC   #$7F
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$1D
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$1E
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@20
   LDA   #$00
   JMP   .sx_done@21
.sx_neg@20:
   LDA   #$FF
.sx_done@21:
   LDY   #$1F
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   i2+3
   CMP   (FP),Y
   BNE   .cmp_true@22
   LDA   i2+2
   DEY
   CMP   (FP),Y
   BNE   .cmp_true@22
   LDA   i2+1
   DEY
   CMP   (FP),Y
   BNE   .cmp_true@22
   LDA   i2
   DEY
   CMP   (FP),Y
   BNE   .cmp_true@22
   LDA   #$00
   JMP   .cmp_end@23
.cmp_true@22:
   LDA   #$01
.cmp_end@23:
   LDY   #$21
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
   ADC   #$26
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$25
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
   LDA   ui2+3
   BNE   .cmp_true@25
   LDA   ui2+2
   BNE   .cmp_true@25
   LDA   ui2+1
   CMP   #$80
   BNE   .cmp_true@25
   LDA   ui2
   CMP   #$96
   BNE   .cmp_true@25
   LDA   #$00
   JMP   .cmp_end@26
.cmp_true@25:
   LDA   #$01
.cmp_end@26:
   LDY   #$23
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
   ADC   #$26
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
   ADC   #$26
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$25
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

u:
   DC.L  $0200800C

i:
   DC.L  $00008002

l:
   DC.L  $FFFFFF9C
   DC.L  $00000000

l2:
   DC.L  $00008002
   DC.L  $00000000

ul:
   DC.L  $0000FFFE
   DC.L  $00000000

ul2:
   DC.L  $7FFFFFF6
   DC.L  $00000000

i2:
   DC.L  $80008096

ui2:
   DC.L  $80008096
