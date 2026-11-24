SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24

fib:
   SUBROUTINE

   ; prologue: 8 arg bytes, 82 local bytes
   SEC
   LDA   SSP
   SBC   #$54
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$53
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
   LDY   #$09
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$03
   LDA   (FP),Y
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
   LDA   #$01
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   ORA   #$00
   BPL   .sx_done@3
   LDA   #$FF
.sx_done@3:
   LDY   #$13
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
   LDY   #$11
   LDA   (FP),Y
   LDY   #$19
   STA   (FP),Y
   LDY   #$12
   LDA   (FP),Y
   LDY   #$1A
   STA   (FP),Y
   LDY   #$13
   LDA   (FP),Y
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
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   ORA   #$00
   BPL   .sx_done@5
   LDA   #$FF
.sx_done@5:
   LDY   #$23
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
   LDY   #$21
   LDA   (FP),Y
   LDY   #$29
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$23
   LDA   (FP),Y
   LDY   #$2B
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
.loop@0_start:
   LDY   #$09
   LDA   (FP),Y
   CLC
   LDY   #$19
   ADC   (FP),Y
   LDY   #$31
   STA   (FP),Y
   LDY   #$0A
   LDA   (FP),Y
   LDY   #$1A
   ADC   (FP),Y
   LDY   #$32
   STA   (FP),Y
   LDY   #$0B
   LDA   (FP),Y
   LDY   #$1B
   ADC   (FP),Y
   LDY   #$33
   STA   (FP),Y
   LDY   #$0C
   LDA   (FP),Y
   LDY   #$1C
   ADC   (FP),Y
   LDY   #$34
   STA   (FP),Y
   LDY   #$0D
   LDA   (FP),Y
   LDY   #$1D
   ADC   (FP),Y
   LDY   #$35
   STA   (FP),Y
   LDY   #$0E
   LDA   (FP),Y
   LDY   #$1E
   ADC   (FP),Y
   LDY   #$36
   STA   (FP),Y
   LDY   #$0F
   LDA   (FP),Y
   LDY   #$1F
   ADC   (FP),Y
   LDY   #$37
   STA   (FP),Y
   LDY   #$10
   LDA   (FP),Y
   LDY   #$20
   ADC   (FP),Y
   LDY   #$38
   STA   (FP),Y
   LDY   #$31
   LDA   (FP),Y
   LDY   #$39
   STA   (FP),Y
   LDY   #$32
   LDA   (FP),Y
   LDY   #$3A
   STA   (FP),Y
   LDY   #$33
   LDA   (FP),Y
   LDY   #$3B
   STA   (FP),Y
   LDY   #$34
   LDA   (FP),Y
   LDY   #$3C
   STA   (FP),Y
   LDY   #$35
   LDA   (FP),Y
   LDY   #$3D
   STA   (FP),Y
   LDY   #$36
   LDA   (FP),Y
   LDY   #$3E
   STA   (FP),Y
   LDY   #$37
   LDA   (FP),Y
   LDY   #$3F
   STA   (FP),Y
   LDY   #$38
   LDA   (FP),Y
   LDY   #$40
   STA   (FP),Y
   LDY   #$19
   LDA   (FP),Y
   LDY   #$09
   STA   (FP),Y
   LDY   #$1A
   LDA   (FP),Y
   LDY   #$0A
   STA   (FP),Y
   LDY   #$1B
   LDA   (FP),Y
   LDY   #$0B
   STA   (FP),Y
   LDY   #$1C
   LDA   (FP),Y
   LDY   #$0C
   STA   (FP),Y
   LDY   #$1D
   LDA   (FP),Y
   LDY   #$0D
   STA   (FP),Y
   LDY   #$1E
   LDA   (FP),Y
   LDY   #$0E
   STA   (FP),Y
   LDY   #$1F
   LDA   (FP),Y
   LDY   #$0F
   STA   (FP),Y
   LDY   #$20
   LDA   (FP),Y
   LDY   #$10
   STA   (FP),Y
   LDY   #$39
   LDA   (FP),Y
   LDY   #$19
   STA   (FP),Y
   LDY   #$3A
   LDA   (FP),Y
   LDY   #$1A
   STA   (FP),Y
   LDY   #$3B
   LDA   (FP),Y
   LDY   #$1B
   STA   (FP),Y
   LDY   #$3C
   LDA   (FP),Y
   LDY   #$1C
   STA   (FP),Y
   LDY   #$3D
   LDA   (FP),Y
   LDY   #$1D
   STA   (FP),Y
   LDY   #$3E
   LDA   (FP),Y
   LDY   #$1E
   STA   (FP),Y
   LDY   #$3F
   LDA   (FP),Y
   LDY   #$1F
   STA   (FP),Y
   LDY   #$40
   LDA   (FP),Y
   LDY   #$20
   STA   (FP),Y
   LDA   #$01
   LDY   #$41
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   ORA   #$00
   BPL   .sx_done@7
   LDA   #$FF
.sx_done@7:
   LDY   #$43
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
   LDY   #$29
   LDA   (FP),Y
   CLC
   LDY   #$41
   ADC   (FP),Y
   LDY   #$49
   STA   (FP),Y
   LDY   #$2A
   LDA   (FP),Y
   LDY   #$42
   ADC   (FP),Y
   LDY   #$4A
   STA   (FP),Y
   LDY   #$2B
   LDA   (FP),Y
   LDY   #$43
   ADC   (FP),Y
   LDY   #$4B
   STA   (FP),Y
   LDY   #$2C
   LDA   (FP),Y
   LDY   #$44
   ADC   (FP),Y
   LDY   #$4C
   STA   (FP),Y
   LDY   #$2D
   LDA   (FP),Y
   LDY   #$45
   ADC   (FP),Y
   LDY   #$4D
   STA   (FP),Y
   LDY   #$2E
   LDA   (FP),Y
   LDY   #$46
   ADC   (FP),Y
   LDY   #$4E
   STA   (FP),Y
   LDY   #$2F
   LDA   (FP),Y
   LDY   #$47
   ADC   (FP),Y
   LDY   #$4F
   STA   (FP),Y
   LDY   #$30
   LDA   (FP),Y
   LDY   #$48
   ADC   (FP),Y
   LDY   #$50
   STA   (FP),Y
   LDY   #$49
   LDA   (FP),Y
   LDY   #$29
   STA   (FP),Y
   LDY   #$4A
   LDA   (FP),Y
   LDY   #$2A
   STA   (FP),Y
   LDY   #$4B
   LDA   (FP),Y
   LDY   #$2B
   STA   (FP),Y
   LDY   #$4C
   LDA   (FP),Y
   LDY   #$2C
   STA   (FP),Y
   LDY   #$4D
   LDA   (FP),Y
   LDY   #$2D
   STA   (FP),Y
   LDY   #$4E
   LDA   (FP),Y
   LDY   #$2E
   STA   (FP),Y
   LDY   #$4F
   LDA   (FP),Y
   LDY   #$2F
   STA   (FP),Y
   LDY   #$50
   LDA   (FP),Y
   LDY   #$30
   STA   (FP),Y
   LDY   #$29
   LDA   (FP),Y
   SEC
   LDY   #$55
   SBC   (FP),Y
   LDY   #$2A
   LDA   (FP),Y
   LDY   #$56
   SBC   (FP),Y
   LDY   #$2B
   LDA   (FP),Y
   LDY   #$57
   SBC   (FP),Y
   LDY   #$2C
   LDA   (FP),Y
   LDY   #$58
   SBC   (FP),Y
   LDY   #$2D
   LDA   (FP),Y
   LDY   #$59
   SBC   (FP),Y
   LDY   #$2E
   LDA   (FP),Y
   LDY   #$5A
   SBC   (FP),Y
   LDY   #$2F
   LDA   (FP),Y
   LDY   #$5B
   SBC   (FP),Y
   LDY   #$30
   LDA   (FP),Y
   LDY   #$5C
   SBC   (FP),Y
   BVC   .cmp_novf@8
   EOR   #$80
.cmp_novf@8:
   ASL
   LDA   #$00
   ROL
   LDY   #$51
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@0
   JMP   .loop@0_start
.lb_skip@0:
   LDY   #$19
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
   ADC   #$5C
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$53
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

   ; prologue: 0 arg bytes, 34 local bytes
   SEC
   LDA   SSP
   SBC   #$24
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$23
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
   LDA   #$00
   INY
   STA   (FP),Y
   ORA   #$00
   BPL   .sx_done@10
   LDA   #$FF
.sx_done@10:
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
   JSR   fib
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
   LDA   #$C2
   INY
   STA   (FP),Y
   LDA   #$2A
   INY
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@11
   LDA   #$00
   JMP   .sx_done@12
.sx_neg@11:
   LDA   #$FF
.sx_done@12:
   LDY   #$13
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
   LDY   #$10
   LDA   (FP),Y
   LDY   #$18
   CMP   (FP),Y
   BNE   .thread@0
   LDY   #$0F
   LDA   (FP),Y
   LDY   #$17
   CMP   (FP),Y
   BNE   .thread@0
   LDY   #$0E
   LDA   (FP),Y
   LDY   #$16
   CMP   (FP),Y
   BNE   .thread@0
   LDY   #$0D
   LDA   (FP),Y
   LDY   #$15
   CMP   (FP),Y
   BNE   .thread@0
   LDY   #$0C
   LDA   (FP),Y
   LDY   #$14
   CMP   (FP),Y
   BNE   .thread@0
   LDY   #$0B
   LDA   (FP),Y
   LDY   #$13
   CMP   (FP),Y
   BNE   .thread@0
   LDY   #$0A
   LDA   (FP),Y
   LDY   #$12
   CMP   (FP),Y
   BNE   .thread@0
   LDY   #$09
   LDA   (FP),Y
   LDY   #$11
   CMP   (FP),Y
   BEQ   .cmp_true@13
.thread@0:
   LDA   #$00
   JMP   .cmp_end@14
.cmp_true@13:
   LDA   #$01
.cmp_end@14:
   LDY   #$19
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$1B
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$1C
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@16
   LDA   #$00
   JMP   .sx_done@17
.sx_neg@16:
   LDA   #$FF
.sx_done@17:
   LDY   #$1D
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
   LDY   #$1B
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
   ADC   #$24
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
