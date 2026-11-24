SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24

callee:
   SUBROUTINE

   ; prologue: 16 arg bytes, 8 local bytes
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

   INY
   LDA   (FP),Y
   CLC
   LDY   #$13
   ADC   (FP),Y
   LDY   #$01
   STA   (FP),Y
   LDY   #$0C
   LDA   (FP),Y
   LDY   #$14
   ADC   (FP),Y
   LDY   #$02
   STA   (FP),Y
   LDY   #$0D
   LDA   (FP),Y
   LDY   #$15
   ADC   (FP),Y
   LDY   #$03
   STA   (FP),Y
   LDY   #$0E
   LDA   (FP),Y
   LDY   #$16
   ADC   (FP),Y
   LDY   #$04
   STA   (FP),Y
   LDY   #$0F
   LDA   (FP),Y
   LDY   #$17
   ADC   (FP),Y
   LDY   #$05
   STA   (FP),Y
   LDY   #$10
   LDA   (FP),Y
   LDY   #$18
   ADC   (FP),Y
   LDY   #$06
   STA   (FP),Y
   LDY   #$11
   LDA   (FP),Y
   LDY   #$19
   ADC   (FP),Y
   LDY   #$07
   STA   (FP),Y
   LDY   #$12
   LDA   (FP),Y
   LDY   #$1A
   ADC   (FP),Y
   LDY   #$08
   STA   (FP),Y
   LDY   #$01
   LDA   (FP),Y
   STA   HARGS+16
   INY
   LDA   (FP),Y
   STA   HARGS+17
   INY
   LDA   (FP),Y
   STA   HARGS+18
   INY
   LDA   (FP),Y
   STA   HARGS+19
   INY
   LDA   (FP),Y
   STA   HARGS+20
   INY
   LDA   (FP),Y
   STA   HARGS+21
   INY
   LDA   (FP),Y
   STA   HARGS+22
   INY
   LDA   (FP),Y
   STA   HARGS+23

   ; epilogue
   CLC
   LDA   FP
   ADC   #$1A
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

target:
   SUBROUTINE

   ; prologue: 8 arg bytes, 24 local bytes
   SEC
   LDA   SSP
   SBC   #$1A
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$19
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
   INY
   ORA   (FP),Y
   INY
   ORA   (FP),Y
   INY
   ORA   (FP),Y
   INY
   ORA   (FP),Y
   INY
   ORA   (FP),Y
   INY
   ORA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@0
   JMP   .if_else@1
.lb_skip@0:
   LDA   #$14
   LDY   #$01
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   ORA   #$00
   BPL   .sx_done@1
   LDA   #$FF
.sx_done@1:
   LDY   #$03
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$01
   LDA   (FP),Y
   STA   @4.y
   INY
   LDA   (FP),Y
   STA   @4.y+1
   INY
   LDA   (FP),Y
   STA   @4.y+2
   INY
   LDA   (FP),Y
   STA   @4.y+3
   INY
   LDA   (FP),Y
   STA   @4.y+4
   INY
   LDA   (FP),Y
   STA   @4.y+5
   INY
   LDA   (FP),Y
   STA   @4.y+6
   INY
   LDA   (FP),Y
   STA   @4.y+7
   LDA   @4.y
   STA   @3.x
   LDA   @4.y+1
   STA   @3.x+1
   LDA   @4.y+2
   STA   @3.x+2
   LDA   @4.y+3
   STA   @3.x+3
   LDA   @4.y+4
   STA   @3.x+4
   LDA   @4.y+5
   STA   @3.x+5
   LDA   @4.y+6
   STA   @3.x+6
   LDA   @4.y+7
   STA   @3.x+7
   JMP   .if_end@0
.if_else@1:
   LDA   #$64
   LDY   #$09
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   ORA   #$00
   BPL   .sx_done@3
   LDA   #$FF
.sx_done@3:
   LDY   #$0B
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$09
   LDA   (FP),Y
   STA   @4.y
   INY
   LDA   (FP),Y
   STA   @4.y+1
   INY
   LDA   (FP),Y
   STA   @4.y+2
   INY
   LDA   (FP),Y
   STA   @4.y+3
   INY
   LDA   (FP),Y
   STA   @4.y+4
   INY
   LDA   (FP),Y
   STA   @4.y+5
   INY
   LDA   (FP),Y
   STA   @4.y+6
   INY
   LDA   (FP),Y
   STA   @4.y+7
   LDA   @4.y
   STA   @3.x
   LDA   @4.y+1
   STA   @3.x+1
   LDA   @4.y+2
   STA   @3.x+2
   LDA   @4.y+3
   STA   @3.x+3
   LDA   @4.y+4
   STA   @3.x+4
   LDA   @4.y+5
   STA   @3.x+5
   LDA   @4.y+6
   STA   @3.x+6
   LDA   @4.y+7
   STA   @3.x+7
.if_end@0:
   SEC
   LDA   SSP
   SBC   #$10
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   @3.x
   LDY   #$01
   STA   (SSP),Y
   LDA   @3.x+1
   INY
   STA   (SSP),Y
   LDA   @3.x+2
   INY
   STA   (SSP),Y
   LDA   @3.x+3
   INY
   STA   (SSP),Y
   LDA   @3.x+4
   INY
   STA   (SSP),Y
   LDA   @3.x+5
   INY
   STA   (SSP),Y
   LDA   @3.x+6
   INY
   STA   (SSP),Y
   LDA   @3.x+7
   INY
   STA   (SSP),Y
   LDA   @4.y
   INY
   STA   (SSP),Y
   LDA   @4.y+1
   INY
   STA   (SSP),Y
   LDA   @4.y+2
   INY
   STA   (SSP),Y
   LDA   @4.y+3
   INY
   STA   (SSP),Y
   LDA   @4.y+4
   INY
   STA   (SSP),Y
   LDA   @4.y+5
   INY
   STA   (SSP),Y
   LDA   @4.y+6
   INY
   STA   (SSP),Y
   LDA   @4.y+7
   INY
   STA   (SSP),Y
   JSR   callee
   LDA   HARGS+16
   LDY   #$11
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
   LDY   #$11
   LDA   (FP),Y
   STA   HARGS+16
   INY
   LDA   (FP),Y
   STA   HARGS+17
   INY
   LDA   (FP),Y
   STA   HARGS+18
   INY
   LDA   (FP),Y
   STA   HARGS+19
   INY
   LDA   (FP),Y
   STA   HARGS+20
   INY
   LDA   (FP),Y
   STA   HARGS+21
   INY
   LDA   (FP),Y
   STA   HARGS+22
   INY
   LDA   (FP),Y
   STA   HARGS+23

   ; epilogue
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
   RTS

main:
   SUBROUTINE

   ; prologue: 0 arg bytes, 84 local bytes
   SEC
   LDA   SSP
   SBC   #$56
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$55
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
   ORA   #$00
   BPL   .sx_done@5
   LDA   #$FF
.sx_done@5:
   LDY   #$03
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
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
   JSR   target
   LDA   HARGS+16
   LDY   #$09
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
   LDY   #$09
   LDA   (FP),Y
   LDY   #$11
   STA   (FP),Y
   LDY   #$0A
   LDA   (FP),Y
   LDY   #$12
   STA   (FP),Y
   LDY   #$0B
   LDA   (FP),Y
   LDY   #$13
   STA   (FP),Y
   LDY   #$0C
   LDA   (FP),Y
   LDY   #$14
   STA   (FP),Y
   LDY   #$0D
   LDA   (FP),Y
   LDY   #$15
   STA   (FP),Y
   LDY   #$0E
   LDA   (FP),Y
   LDY   #$16
   STA   (FP),Y
   LDY   #$0F
   LDA   (FP),Y
   LDY   #$17
   STA   (FP),Y
   LDY   #$10
   LDA   (FP),Y
   LDY   #$18
   STA   (FP),Y
   LDA   #$C8
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   ORA   #$00
   BPL   .sx_done@7
   LDA   #$FF
.sx_done@7:
   LDY   #$1B
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$18
   LDA   (FP),Y
   LDY   #$20
   CMP   (FP),Y
   BNE   .cmp_true@8
   LDY   #$17
   LDA   (FP),Y
   LDY   #$1F
   CMP   (FP),Y
   BNE   .cmp_true@8
   LDY   #$16
   LDA   (FP),Y
   LDY   #$1E
   CMP   (FP),Y
   BNE   .cmp_true@8
   LDY   #$15
   LDA   (FP),Y
   LDY   #$1D
   CMP   (FP),Y
   BNE   .cmp_true@8
   LDY   #$14
   LDA   (FP),Y
   LDY   #$1C
   CMP   (FP),Y
   BNE   .cmp_true@8
   LDY   #$13
   LDA   (FP),Y
   LDY   #$1B
   CMP   (FP),Y
   BNE   .cmp_true@8
   LDY   #$12
   LDA   (FP),Y
   LDY   #$1A
   CMP   (FP),Y
   BNE   .cmp_true@8
   LDY   #$11
   LDA   (FP),Y
   LDY   #$19
   CMP   (FP),Y
   BNE   .cmp_true@8
   LDA   #$00
   JMP   .cmp_end@9
.cmp_true@8:
   LDA   #$01
.cmp_end@9:
   LDY   #$21
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@2
   LDA   #$01
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   ORA   #$00
   BPL   .sx_done@12
   LDA   #$FF
.sx_done@12:
   LDY   #$25
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$23
   LDA   (FP),Y
   STA   HARGS+16
   INY
   LDA   (FP),Y
   STA   HARGS+17
   INY
   LDA   (FP),Y
   STA   HARGS+18
   INY
   LDA   (FP),Y
   STA   HARGS+19
   INY
   LDA   (FP),Y
   STA   HARGS+20
   INY
   LDA   (FP),Y
   STA   HARGS+21
   INY
   LDA   (FP),Y
   STA   HARGS+22
   INY
   LDA   (FP),Y
   STA   HARGS+23

   ; epilogue
   CLC
   LDA   FP
   ADC   #$56
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$55
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   RTS
.if_end@2:
   LDA   #$01
   LDY   #$2B
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   ORA   #$00
   BPL   .sx_done@14
   LDA   #$FF
.sx_done@14:
   LDY   #$2D
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
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
   LDY   #$2B
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$2C
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
   LDY   #$2F
   LDA   (FP),Y
   LDY   #$05
   STA   (SSP),Y
   LDY   #$30
   LDA   (FP),Y
   LDY   #$06
   STA   (SSP),Y
   LDY   #$31
   LDA   (FP),Y
   LDY   #$07
   STA   (SSP),Y
   LDY   #$32
   LDA   (FP),Y
   LDY   #$08
   STA   (SSP),Y
   JSR   target
   LDA   HARGS+16
   LDY   #$33
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
   LDY   #$33
   LDA   (FP),Y
   LDY   #$11
   STA   (FP),Y
   LDY   #$34
   LDA   (FP),Y
   LDY   #$12
   STA   (FP),Y
   LDY   #$35
   LDA   (FP),Y
   LDY   #$13
   STA   (FP),Y
   LDY   #$36
   LDA   (FP),Y
   LDY   #$14
   STA   (FP),Y
   LDY   #$37
   LDA   (FP),Y
   LDY   #$15
   STA   (FP),Y
   LDY   #$38
   LDA   (FP),Y
   LDY   #$16
   STA   (FP),Y
   LDY   #$39
   LDA   (FP),Y
   LDY   #$17
   STA   (FP),Y
   LDY   #$3A
   LDA   (FP),Y
   LDY   #$18
   STA   (FP),Y
   LDA   #$28
   LDY   #$3B
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   ORA   #$00
   BPL   .sx_done@16
   LDA   #$FF
.sx_done@16:
   LDY   #$3D
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$18
   LDA   (FP),Y
   LDY   #$42
   CMP   (FP),Y
   BNE   .cmp_true@17
   LDY   #$17
   LDA   (FP),Y
   LDY   #$41
   CMP   (FP),Y
   BNE   .cmp_true@17
   LDY   #$16
   LDA   (FP),Y
   LDY   #$40
   CMP   (FP),Y
   BNE   .cmp_true@17
   LDY   #$15
   LDA   (FP),Y
   LDY   #$3F
   CMP   (FP),Y
   BNE   .cmp_true@17
   LDY   #$14
   LDA   (FP),Y
   LDY   #$3E
   CMP   (FP),Y
   BNE   .cmp_true@17
   LDY   #$13
   LDA   (FP),Y
   LDY   #$3D
   CMP   (FP),Y
   BNE   .cmp_true@17
   LDY   #$12
   LDA   (FP),Y
   LDY   #$3C
   CMP   (FP),Y
   BNE   .cmp_true@17
   LDY   #$11
   LDA   (FP),Y
   LDY   #$3B
   CMP   (FP),Y
   BNE   .cmp_true@17
   LDA   #$00
   JMP   .cmp_end@18
.cmp_true@17:
   LDA   #$01
.cmp_end@18:
   LDY   #$43
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@3
   LDA   #$02
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   ORA   #$00
   BPL   .sx_done@21
   LDA   #$FF
.sx_done@21:
   LDY   #$47
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$45
   LDA   (FP),Y
   STA   HARGS+16
   INY
   LDA   (FP),Y
   STA   HARGS+17
   INY
   LDA   (FP),Y
   STA   HARGS+18
   INY
   LDA   (FP),Y
   STA   HARGS+19
   INY
   LDA   (FP),Y
   STA   HARGS+20
   INY
   LDA   (FP),Y
   STA   HARGS+21
   INY
   LDA   (FP),Y
   STA   HARGS+22
   INY
   LDA   (FP),Y
   STA   HARGS+23

   ; epilogue
   CLC
   LDA   FP
   ADC   #$56
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$55
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   RTS
.if_end@3:
   LDA   #$00
   LDY   #$4D
   STA   (FP),Y
   INY
   STA   (FP),Y
   ORA   #$00
   BPL   .sx_done@23
   LDA   #$FF
.sx_done@23:
   LDY   #$4F
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$4D
   LDA   (FP),Y
   STA   HARGS+16
   INY
   LDA   (FP),Y
   STA   HARGS+17
   INY
   LDA   (FP),Y
   STA   HARGS+18
   INY
   LDA   (FP),Y
   STA   HARGS+19
   INY
   LDA   (FP),Y
   STA   HARGS+20
   INY
   LDA   (FP),Y
   STA   HARGS+21
   INY
   LDA   (FP),Y
   STA   HARGS+22
   INY
   LDA   (FP),Y
   STA   HARGS+23

   ; epilogue
   CLC
   LDA   FP
   ADC   #$56
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

@3.x:
   DS.B  8

@4.y:
   DS.B  8
