SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24

fun1:
   SUBROUTINE

   ; prologue: 2 arg bytes, 0 local bytes
   SEC
   LDA   SSP
   SBC   #$02
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$01
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   INY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDA   globvar_1
   LDY   #$00
   STA   (DPTR),Y

   ; epilogue
   CLC
   LDA   FP
   ADC   #$04
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
   RTS

fun2:
   SUBROUTINE

   ; prologue: 2 arg bytes, 0 local bytes
   SEC
   LDA   SSP
   SBC   #$02
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$01
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   INY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDA   globvar_2
   LDY   #$00
   STA   (DPTR),Y
   LDA   globvar_2+1
   INY
   STA   (DPTR),Y

   ; epilogue
   CLC
   LDA   FP
   ADC   #$04
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   RTS

fun3:
   SUBROUTINE

   ; prologue: 2 arg bytes, 0 local bytes
   SEC
   LDA   SSP
   SBC   #$02
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$01
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   INY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDA   globvar_3
   LDY   #$00
   STA   (DPTR),Y
   LDA   globvar_3+1
   INY
   STA   (DPTR),Y
   LDA   globvar_3+2
   INY
   STA   (DPTR),Y

   ; epilogue
   CLC
   LDA   FP
   ADC   #$04
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   DEY
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   RTS

fun4:
   SUBROUTINE

   ; prologue: 2 arg bytes, 0 local bytes
   SEC
   LDA   SSP
   SBC   #$02
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$01
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   INY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDA   globvar_4
   LDY   #$00
   STA   (DPTR),Y
   LDA   globvar_4+1
   INY
   STA   (DPTR),Y
   LDA   globvar_4+2
   INY
   STA   (DPTR),Y
   LDA   globvar_4+3
   INY
   STA   (DPTR),Y

   ; epilogue
   CLC
   LDA   FP
   ADC   #$04
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$01
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   RTS

fun5:
   SUBROUTINE

   ; prologue: 2 arg bytes, 0 local bytes
   SEC
   LDA   SSP
   SBC   #$02
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$01
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   INY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDA   globvar_5
   LDY   #$00
   STA   (DPTR),Y
   LDA   globvar_5+1
   INY
   STA   (DPTR),Y
   LDA   globvar_5+2
   INY
   STA   (DPTR),Y
   LDA   globvar_5+3
   INY
   STA   (DPTR),Y
   LDA   globvar_5+4
   INY
   STA   (DPTR),Y

   ; epilogue
   CLC
   LDA   FP
   ADC   #$04
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$01
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   RTS

fun6:
   SUBROUTINE

   ; prologue: 2 arg bytes, 0 local bytes
   SEC
   LDA   SSP
   SBC   #$02
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$01
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   INY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDA   globvar_6
   LDY   #$00
   STA   (DPTR),Y
   LDA   globvar_6+1
   INY
   STA   (DPTR),Y
   LDA   globvar_6+2
   INY
   STA   (DPTR),Y
   LDA   globvar_6+3
   INY
   STA   (DPTR),Y
   LDA   globvar_6+4
   INY
   STA   (DPTR),Y
   LDA   globvar_6+5
   INY
   STA   (DPTR),Y

   ; epilogue
   CLC
   LDA   FP
   ADC   #$04
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$01
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   RTS

fun7:
   SUBROUTINE

   ; prologue: 2 arg bytes, 0 local bytes
   SEC
   LDA   SSP
   SBC   #$02
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$01
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   INY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDA   globvar_7
   LDY   #$00
   STA   (DPTR),Y
   LDA   globvar_7+1
   INY
   STA   (DPTR),Y
   LDA   globvar_7+2
   INY
   STA   (DPTR),Y
   LDA   globvar_7+3
   INY
   STA   (DPTR),Y
   LDA   globvar_7+4
   INY
   STA   (DPTR),Y
   LDA   globvar_7+5
   INY
   STA   (DPTR),Y
   LDA   globvar_7+6
   INY
   STA   (DPTR),Y

   ; epilogue
   CLC
   LDA   FP
   ADC   #$04
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$01
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   RTS

fun8:
   SUBROUTINE

   ; prologue: 2 arg bytes, 0 local bytes
   SEC
   LDA   SSP
   SBC   #$02
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$01
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   INY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDA   globvar_8
   LDY   #$00
   STA   (DPTR),Y
   LDA   globvar_8+1
   INY
   STA   (DPTR),Y
   LDA   globvar_8+2
   INY
   STA   (DPTR),Y
   LDA   globvar_8+3
   INY
   STA   (DPTR),Y
   LDA   globvar_8+4
   INY
   STA   (DPTR),Y
   LDA   globvar_8+5
   INY
   STA   (DPTR),Y
   LDA   globvar_8+6
   INY
   STA   (DPTR),Y
   LDA   globvar_8+7
   INY
   STA   (DPTR),Y

   ; epilogue
   CLC
   LDA   FP
   ADC   #$04
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$01
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   RTS

fun9:
   SUBROUTINE

   ; prologue: 2 arg bytes, 0 local bytes
   SEC
   LDA   SSP
   SBC   #$02
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$01
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   INY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDA   globvar_9
   LDY   #$00
   STA   (DPTR),Y
   LDA   globvar_9+1
   INY
   STA   (DPTR),Y
   LDA   globvar_9+2
   INY
   STA   (DPTR),Y
   LDA   globvar_9+3
   INY
   STA   (DPTR),Y
   LDA   globvar_9+4
   INY
   STA   (DPTR),Y
   LDA   globvar_9+5
   INY
   STA   (DPTR),Y
   LDA   globvar_9+6
   INY
   STA   (DPTR),Y
   LDA   globvar_9+7
   INY
   STA   (DPTR),Y
   LDA   globvar_9+8
   INY
   STA   (DPTR),Y

   ; epilogue
   CLC
   LDA   FP
   ADC   #$04
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$01
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   RTS

fun10:
   SUBROUTINE

   ; prologue: 2 arg bytes, 0 local bytes
   SEC
   LDA   SSP
   SBC   #$02
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$01
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   INY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDA   globvar_10
   LDY   #$00
   STA   (DPTR),Y
   LDA   globvar_10+1
   INY
   STA   (DPTR),Y
   LDA   globvar_10+2
   INY
   STA   (DPTR),Y
   LDA   globvar_10+3
   INY
   STA   (DPTR),Y
   LDA   globvar_10+4
   INY
   STA   (DPTR),Y
   LDA   globvar_10+5
   INY
   STA   (DPTR),Y
   LDA   globvar_10+6
   INY
   STA   (DPTR),Y
   LDA   globvar_10+7
   INY
   STA   (DPTR),Y
   LDA   globvar_10+8
   INY
   STA   (DPTR),Y
   LDA   globvar_10+9
   INY
   STA   (DPTR),Y

   ; epilogue
   CLC
   LDA   FP
   ADC   #$04
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$01
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   RTS

fun11:
   SUBROUTINE

   ; prologue: 2 arg bytes, 0 local bytes
   SEC
   LDA   SSP
   SBC   #$02
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$01
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   INY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDA   globvar_11
   LDY   #$00
   STA   (DPTR),Y
   LDA   globvar_11+1
   INY
   STA   (DPTR),Y
   LDA   globvar_11+2
   INY
   STA   (DPTR),Y
   LDA   globvar_11+3
   INY
   STA   (DPTR),Y
   LDA   globvar_11+4
   INY
   STA   (DPTR),Y
   LDA   globvar_11+5
   INY
   STA   (DPTR),Y
   LDA   globvar_11+6
   INY
   STA   (DPTR),Y
   LDA   globvar_11+7
   INY
   STA   (DPTR),Y
   LDA   globvar_11+8
   INY
   STA   (DPTR),Y
   LDA   globvar_11+9
   INY
   STA   (DPTR),Y
   LDA   globvar_11+10
   INY
   STA   (DPTR),Y

   ; epilogue
   CLC
   LDA   FP
   ADC   #$04
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$01
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   RTS

fun12:
   SUBROUTINE

   ; prologue: 2 arg bytes, 0 local bytes
   SEC
   LDA   SSP
   SBC   #$02
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$01
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   INY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDA   globvar_12
   LDY   #$00
   STA   (DPTR),Y
   LDA   globvar_12+1
   INY
   STA   (DPTR),Y
   LDA   globvar_12+2
   INY
   STA   (DPTR),Y
   LDA   globvar_12+3
   INY
   STA   (DPTR),Y
   LDA   globvar_12+4
   INY
   STA   (DPTR),Y
   LDA   globvar_12+5
   INY
   STA   (DPTR),Y
   LDA   globvar_12+6
   INY
   STA   (DPTR),Y
   LDA   globvar_12+7
   INY
   STA   (DPTR),Y
   LDA   globvar_12+8
   INY
   STA   (DPTR),Y
   LDA   globvar_12+9
   INY
   STA   (DPTR),Y
   LDA   globvar_12+10
   INY
   STA   (DPTR),Y
   LDA   globvar_12+11
   INY
   STA   (DPTR),Y

   ; epilogue
   CLC
   LDA   FP
   ADC   #$04
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$01
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   RTS

fun13:
   SUBROUTINE

   ; prologue: 2 arg bytes, 0 local bytes
   SEC
   LDA   SSP
   SBC   #$02
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$01
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   INY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDA   globvar_13
   LDY   #$00
   STA   (DPTR),Y
   LDA   globvar_13+1
   INY
   STA   (DPTR),Y
   LDA   globvar_13+2
   INY
   STA   (DPTR),Y
   LDA   globvar_13+3
   INY
   STA   (DPTR),Y
   LDA   globvar_13+4
   INY
   STA   (DPTR),Y
   LDA   globvar_13+5
   INY
   STA   (DPTR),Y
   LDA   globvar_13+6
   INY
   STA   (DPTR),Y
   LDA   globvar_13+7
   INY
   STA   (DPTR),Y
   LDA   globvar_13+8
   INY
   STA   (DPTR),Y
   LDA   globvar_13+9
   INY
   STA   (DPTR),Y
   LDA   globvar_13+10
   INY
   STA   (DPTR),Y
   LDA   globvar_13+11
   INY
   STA   (DPTR),Y
   LDA   globvar_13+12
   INY
   STA   (DPTR),Y

   ; epilogue
   CLC
   LDA   FP
   ADC   #$04
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$01
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   RTS

fun14:
   SUBROUTINE

   ; prologue: 2 arg bytes, 0 local bytes
   SEC
   LDA   SSP
   SBC   #$02
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$01
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   INY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDA   globvar_14
   LDY   #$00
   STA   (DPTR),Y
   LDA   globvar_14+1
   INY
   STA   (DPTR),Y
   LDA   globvar_14+2
   INY
   STA   (DPTR),Y
   LDA   globvar_14+3
   INY
   STA   (DPTR),Y
   LDA   globvar_14+4
   INY
   STA   (DPTR),Y
   LDA   globvar_14+5
   INY
   STA   (DPTR),Y
   LDA   globvar_14+6
   INY
   STA   (DPTR),Y
   LDA   globvar_14+7
   INY
   STA   (DPTR),Y
   LDA   globvar_14+8
   INY
   STA   (DPTR),Y
   LDA   globvar_14+9
   INY
   STA   (DPTR),Y
   LDA   globvar_14+10
   INY
   STA   (DPTR),Y
   LDA   globvar_14+11
   INY
   STA   (DPTR),Y
   LDA   globvar_14+12
   INY
   STA   (DPTR),Y
   LDA   globvar_14+13
   INY
   STA   (DPTR),Y

   ; epilogue
   CLC
   LDA   FP
   ADC   #$04
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$01
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   RTS

fun15:
   SUBROUTINE

   ; prologue: 2 arg bytes, 0 local bytes
   SEC
   LDA   SSP
   SBC   #$02
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$01
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   INY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDA   globvar_15
   LDY   #$00
   STA   (DPTR),Y
   LDA   globvar_15+1
   INY
   STA   (DPTR),Y
   LDA   globvar_15+2
   INY
   STA   (DPTR),Y
   LDA   globvar_15+3
   INY
   STA   (DPTR),Y
   LDA   globvar_15+4
   INY
   STA   (DPTR),Y
   LDA   globvar_15+5
   INY
   STA   (DPTR),Y
   LDA   globvar_15+6
   INY
   STA   (DPTR),Y
   LDA   globvar_15+7
   INY
   STA   (DPTR),Y
   LDA   globvar_15+8
   INY
   STA   (DPTR),Y
   LDA   globvar_15+9
   INY
   STA   (DPTR),Y
   LDA   globvar_15+10
   INY
   STA   (DPTR),Y
   LDA   globvar_15+11
   INY
   STA   (DPTR),Y
   LDA   globvar_15+12
   INY
   STA   (DPTR),Y
   LDA   globvar_15+13
   INY
   STA   (DPTR),Y
   LDA   globvar_15+14
   INY
   STA   (DPTR),Y

   ; epilogue
   CLC
   LDA   FP
   ADC   #$04
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$01
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   RTS

fun16:
   SUBROUTINE

   ; prologue: 2 arg bytes, 0 local bytes
   SEC
   LDA   SSP
   SBC   #$02
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$01
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   INY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDA   globvar_16
   LDY   #$00
   STA   (DPTR),Y
   LDA   globvar_16+1
   INY
   STA   (DPTR),Y
   LDA   globvar_16+2
   INY
   STA   (DPTR),Y
   LDA   globvar_16+3
   INY
   STA   (DPTR),Y
   LDA   globvar_16+4
   INY
   STA   (DPTR),Y
   LDA   globvar_16+5
   INY
   STA   (DPTR),Y
   LDA   globvar_16+6
   INY
   STA   (DPTR),Y
   LDA   globvar_16+7
   INY
   STA   (DPTR),Y
   LDA   globvar_16+8
   INY
   STA   (DPTR),Y
   LDA   globvar_16+9
   INY
   STA   (DPTR),Y
   LDA   globvar_16+10
   INY
   STA   (DPTR),Y
   LDA   globvar_16+11
   INY
   STA   (DPTR),Y
   LDA   globvar_16+12
   INY
   STA   (DPTR),Y
   LDA   globvar_16+13
   INY
   STA   (DPTR),Y
   LDA   globvar_16+14
   INY
   STA   (DPTR),Y
   LDA   globvar_16+15
   INY
   STA   (DPTR),Y

   ; epilogue
   CLC
   LDA   FP
   ADC   #$04
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$01
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   RTS

fun17:
   SUBROUTINE

   ; prologue: 2 arg bytes, 0 local bytes
   SEC
   LDA   SSP
   SBC   #$02
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$01
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   INY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDA   globvar_17
   LDY   #$00
   STA   (DPTR),Y
   LDA   globvar_17+1
   INY
   STA   (DPTR),Y
   LDA   globvar_17+2
   INY
   STA   (DPTR),Y
   LDA   globvar_17+3
   INY
   STA   (DPTR),Y
   LDA   globvar_17+4
   INY
   STA   (DPTR),Y
   LDA   globvar_17+5
   INY
   STA   (DPTR),Y
   LDA   globvar_17+6
   INY
   STA   (DPTR),Y
   LDA   globvar_17+7
   INY
   STA   (DPTR),Y
   LDA   globvar_17+8
   INY
   STA   (DPTR),Y
   LDA   globvar_17+9
   INY
   STA   (DPTR),Y
   LDA   globvar_17+10
   INY
   STA   (DPTR),Y
   LDA   globvar_17+11
   INY
   STA   (DPTR),Y
   LDA   globvar_17+12
   INY
   STA   (DPTR),Y
   LDA   globvar_17+13
   INY
   STA   (DPTR),Y
   LDA   globvar_17+14
   INY
   STA   (DPTR),Y
   LDA   globvar_17+15
   INY
   STA   (DPTR),Y
   LDA   globvar_17+16
   INY
   STA   (DPTR),Y

   ; epilogue
   CLC
   LDA   FP
   ADC   #$04
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$01
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   RTS

fun18:
   SUBROUTINE

   ; prologue: 2 arg bytes, 0 local bytes
   SEC
   LDA   SSP
   SBC   #$02
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$01
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   INY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDA   globvar_18
   LDY   #$00
   STA   (DPTR),Y
   LDA   globvar_18+1
   INY
   STA   (DPTR),Y
   LDA   globvar_18+2
   INY
   STA   (DPTR),Y
   LDA   globvar_18+3
   INY
   STA   (DPTR),Y
   LDA   globvar_18+4
   INY
   STA   (DPTR),Y
   LDA   globvar_18+5
   INY
   STA   (DPTR),Y
   LDA   globvar_18+6
   INY
   STA   (DPTR),Y
   LDA   globvar_18+7
   INY
   STA   (DPTR),Y
   LDA   globvar_18+8
   INY
   STA   (DPTR),Y
   LDA   globvar_18+9
   INY
   STA   (DPTR),Y
   LDA   globvar_18+10
   INY
   STA   (DPTR),Y
   LDA   globvar_18+11
   INY
   STA   (DPTR),Y
   LDA   globvar_18+12
   INY
   STA   (DPTR),Y
   LDA   globvar_18+13
   INY
   STA   (DPTR),Y
   LDA   globvar_18+14
   INY
   STA   (DPTR),Y
   LDA   globvar_18+15
   INY
   STA   (DPTR),Y
   LDA   globvar_18+16
   INY
   STA   (DPTR),Y
   LDA   globvar_18+17
   INY
   STA   (DPTR),Y

   ; epilogue
   CLC
   LDA   FP
   ADC   #$04
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$01
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   RTS

fun19:
   SUBROUTINE

   ; prologue: 2 arg bytes, 0 local bytes
   SEC
   LDA   SSP
   SBC   #$02
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$01
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   INY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDA   globvar_19
   LDY   #$00
   STA   (DPTR),Y
   LDA   globvar_19+1
   INY
   STA   (DPTR),Y
   LDA   globvar_19+2
   INY
   STA   (DPTR),Y
   LDA   globvar_19+3
   INY
   STA   (DPTR),Y
   LDA   globvar_19+4
   INY
   STA   (DPTR),Y
   LDA   globvar_19+5
   INY
   STA   (DPTR),Y
   LDA   globvar_19+6
   INY
   STA   (DPTR),Y
   LDA   globvar_19+7
   INY
   STA   (DPTR),Y
   LDA   globvar_19+8
   INY
   STA   (DPTR),Y
   LDA   globvar_19+9
   INY
   STA   (DPTR),Y
   LDA   globvar_19+10
   INY
   STA   (DPTR),Y
   LDA   globvar_19+11
   INY
   STA   (DPTR),Y
   LDA   globvar_19+12
   INY
   STA   (DPTR),Y
   LDA   globvar_19+13
   INY
   STA   (DPTR),Y
   LDA   globvar_19+14
   INY
   STA   (DPTR),Y
   LDA   globvar_19+15
   INY
   STA   (DPTR),Y
   LDA   globvar_19+16
   INY
   STA   (DPTR),Y
   LDA   globvar_19+17
   INY
   STA   (DPTR),Y
   LDA   globvar_19+18
   INY
   STA   (DPTR),Y

   ; epilogue
   CLC
   LDA   FP
   ADC   #$04
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$01
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   RTS

fun20:
   SUBROUTINE

   ; prologue: 2 arg bytes, 0 local bytes
   SEC
   LDA   SSP
   SBC   #$02
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$01
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   INY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDA   globvar_20
   LDY   #$00
   STA   (DPTR),Y
   LDA   globvar_20+1
   INY
   STA   (DPTR),Y
   LDA   globvar_20+2
   INY
   STA   (DPTR),Y
   LDA   globvar_20+3
   INY
   STA   (DPTR),Y
   LDA   globvar_20+4
   INY
   STA   (DPTR),Y
   LDA   globvar_20+5
   INY
   STA   (DPTR),Y
   LDA   globvar_20+6
   INY
   STA   (DPTR),Y
   LDA   globvar_20+7
   INY
   STA   (DPTR),Y
   LDA   globvar_20+8
   INY
   STA   (DPTR),Y
   LDA   globvar_20+9
   INY
   STA   (DPTR),Y
   LDA   globvar_20+10
   INY
   STA   (DPTR),Y
   LDA   globvar_20+11
   INY
   STA   (DPTR),Y
   LDA   globvar_20+12
   INY
   STA   (DPTR),Y
   LDA   globvar_20+13
   INY
   STA   (DPTR),Y
   LDA   globvar_20+14
   INY
   STA   (DPTR),Y
   LDA   globvar_20+15
   INY
   STA   (DPTR),Y
   LDA   globvar_20+16
   INY
   STA   (DPTR),Y
   LDA   globvar_20+17
   INY
   STA   (DPTR),Y
   LDA   globvar_20+18
   INY
   STA   (DPTR),Y
   LDA   globvar_20+19
   INY
   STA   (DPTR),Y

   ; epilogue
   CLC
   LDA   FP
   ADC   #$04
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$01
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   RTS

fun21:
   SUBROUTINE

   ; prologue: 2 arg bytes, 0 local bytes
   SEC
   LDA   SSP
   SBC   #$02
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$01
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   INY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDA   globvar_21
   LDY   #$00
   STA   (DPTR),Y
   LDA   globvar_21+1
   INY
   STA   (DPTR),Y
   LDA   globvar_21+2
   INY
   STA   (DPTR),Y
   LDA   globvar_21+3
   INY
   STA   (DPTR),Y
   LDA   globvar_21+4
   INY
   STA   (DPTR),Y
   LDA   globvar_21+5
   INY
   STA   (DPTR),Y
   LDA   globvar_21+6
   INY
   STA   (DPTR),Y
   LDA   globvar_21+7
   INY
   STA   (DPTR),Y
   LDA   globvar_21+8
   INY
   STA   (DPTR),Y
   LDA   globvar_21+9
   INY
   STA   (DPTR),Y
   LDA   globvar_21+10
   INY
   STA   (DPTR),Y
   LDA   globvar_21+11
   INY
   STA   (DPTR),Y
   LDA   globvar_21+12
   INY
   STA   (DPTR),Y
   LDA   globvar_21+13
   INY
   STA   (DPTR),Y
   LDA   globvar_21+14
   INY
   STA   (DPTR),Y
   LDA   globvar_21+15
   INY
   STA   (DPTR),Y
   LDA   globvar_21+16
   INY
   STA   (DPTR),Y
   LDA   globvar_21+17
   INY
   STA   (DPTR),Y
   LDA   globvar_21+18
   INY
   STA   (DPTR),Y
   LDA   globvar_21+19
   INY
   STA   (DPTR),Y
   LDA   globvar_21+20
   INY
   STA   (DPTR),Y

   ; epilogue
   CLC
   LDA   FP
   ADC   #$04
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$01
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   RTS

fun22:
   SUBROUTINE

   ; prologue: 2 arg bytes, 0 local bytes
   SEC
   LDA   SSP
   SBC   #$02
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$01
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   INY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDA   globvar_22
   LDY   #$00
   STA   (DPTR),Y
   LDA   globvar_22+1
   INY
   STA   (DPTR),Y
   LDA   globvar_22+2
   INY
   STA   (DPTR),Y
   LDA   globvar_22+3
   INY
   STA   (DPTR),Y
   LDA   globvar_22+4
   INY
   STA   (DPTR),Y
   LDA   globvar_22+5
   INY
   STA   (DPTR),Y
   LDA   globvar_22+6
   INY
   STA   (DPTR),Y
   LDA   globvar_22+7
   INY
   STA   (DPTR),Y
   LDA   globvar_22+8
   INY
   STA   (DPTR),Y
   LDA   globvar_22+9
   INY
   STA   (DPTR),Y
   LDA   globvar_22+10
   INY
   STA   (DPTR),Y
   LDA   globvar_22+11
   INY
   STA   (DPTR),Y
   LDA   globvar_22+12
   INY
   STA   (DPTR),Y
   LDA   globvar_22+13
   INY
   STA   (DPTR),Y
   LDA   globvar_22+14
   INY
   STA   (DPTR),Y
   LDA   globvar_22+15
   INY
   STA   (DPTR),Y
   LDA   globvar_22+16
   INY
   STA   (DPTR),Y
   LDA   globvar_22+17
   INY
   STA   (DPTR),Y
   LDA   globvar_22+18
   INY
   STA   (DPTR),Y
   LDA   globvar_22+19
   INY
   STA   (DPTR),Y
   LDA   globvar_22+20
   INY
   STA   (DPTR),Y
   LDA   globvar_22+21
   INY
   STA   (DPTR),Y

   ; epilogue
   CLC
   LDA   FP
   ADC   #$04
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$01
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   RTS

fun23:
   SUBROUTINE

   ; prologue: 2 arg bytes, 0 local bytes
   SEC
   LDA   SSP
   SBC   #$02
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$01
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   INY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDA   globvar_23
   LDY   #$00
   STA   (DPTR),Y
   LDA   globvar_23+1
   INY
   STA   (DPTR),Y
   LDA   globvar_23+2
   INY
   STA   (DPTR),Y
   LDA   globvar_23+3
   INY
   STA   (DPTR),Y
   LDA   globvar_23+4
   INY
   STA   (DPTR),Y
   LDA   globvar_23+5
   INY
   STA   (DPTR),Y
   LDA   globvar_23+6
   INY
   STA   (DPTR),Y
   LDA   globvar_23+7
   INY
   STA   (DPTR),Y
   LDA   globvar_23+8
   INY
   STA   (DPTR),Y
   LDA   globvar_23+9
   INY
   STA   (DPTR),Y
   LDA   globvar_23+10
   INY
   STA   (DPTR),Y
   LDA   globvar_23+11
   INY
   STA   (DPTR),Y
   LDA   globvar_23+12
   INY
   STA   (DPTR),Y
   LDA   globvar_23+13
   INY
   STA   (DPTR),Y
   LDA   globvar_23+14
   INY
   STA   (DPTR),Y
   LDA   globvar_23+15
   INY
   STA   (DPTR),Y
   LDA   globvar_23+16
   INY
   STA   (DPTR),Y
   LDA   globvar_23+17
   INY
   STA   (DPTR),Y
   LDA   globvar_23+18
   INY
   STA   (DPTR),Y
   LDA   globvar_23+19
   INY
   STA   (DPTR),Y
   LDA   globvar_23+20
   INY
   STA   (DPTR),Y
   LDA   globvar_23+21
   INY
   STA   (DPTR),Y
   LDA   globvar_23+22
   INY
   STA   (DPTR),Y

   ; epilogue
   CLC
   LDA   FP
   ADC   #$04
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$01
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   RTS

fun24:
   SUBROUTINE

   ; prologue: 2 arg bytes, 0 local bytes
   SEC
   LDA   SSP
   SBC   #$02
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$01
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   INY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDA   globvar_24
   LDY   #$00
   STA   (DPTR),Y
   LDA   globvar_24+1
   INY
   STA   (DPTR),Y
   LDA   globvar_24+2
   INY
   STA   (DPTR),Y
   LDA   globvar_24+3
   INY
   STA   (DPTR),Y
   LDA   globvar_24+4
   INY
   STA   (DPTR),Y
   LDA   globvar_24+5
   INY
   STA   (DPTR),Y
   LDA   globvar_24+6
   INY
   STA   (DPTR),Y
   LDA   globvar_24+7
   INY
   STA   (DPTR),Y
   LDA   globvar_24+8
   INY
   STA   (DPTR),Y
   LDA   globvar_24+9
   INY
   STA   (DPTR),Y
   LDA   globvar_24+10
   INY
   STA   (DPTR),Y
   LDA   globvar_24+11
   INY
   STA   (DPTR),Y
   LDA   globvar_24+12
   INY
   STA   (DPTR),Y
   LDA   globvar_24+13
   INY
   STA   (DPTR),Y
   LDA   globvar_24+14
   INY
   STA   (DPTR),Y
   LDA   globvar_24+15
   INY
   STA   (DPTR),Y
   LDA   globvar_24+16
   INY
   STA   (DPTR),Y
   LDA   globvar_24+17
   INY
   STA   (DPTR),Y
   LDA   globvar_24+18
   INY
   STA   (DPTR),Y
   LDA   globvar_24+19
   INY
   STA   (DPTR),Y
   LDA   globvar_24+20
   INY
   STA   (DPTR),Y
   LDA   globvar_24+21
   INY
   STA   (DPTR),Y
   LDA   globvar_24+22
   INY
   STA   (DPTR),Y
   LDA   globvar_24+23
   INY
   STA   (DPTR),Y

   ; epilogue
   CLC
   LDA   FP
   ADC   #$04
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$01
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   RTS
