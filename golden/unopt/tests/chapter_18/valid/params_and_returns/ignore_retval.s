SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24

return_in_reg:
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

   LDA   #<globl
   LDY   #$01
   STA   (FP),Y
   LDA   #>globl
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
   DEY
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
   LDA   #<globl
   INY
   STA   (FP),Y
   LDA   #>globl
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
   LDY   #$0B
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDA   globl
   LDY   #$00
   STA   (DPTR),Y
   LDA   globl+1
   INY
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

return_in_mem:
   SUBROUTINE

   ; prologue: 2 arg bytes, 52 local bytes
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

   LDA   #<globl2
   LDY   #$01
   STA   (FP),Y
   LDA   #>globl2
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
   LDY   #$03
   LDA   (FP),Y
   STA   HARGS
   INY
   LDA   (FP),Y
   STA   HARGS+1
   INY
   LDA   (FP),Y
   STA   HARGS+2
   INY
   LDA   (FP),Y
   STA   HARGS+3
   INY
   LDA   (FP),Y
   STA   HARGS+4
   INY
   LDA   (FP),Y
   STA   HARGS+5
   INY
   LDA   (FP),Y
   STA   HARGS+6
   INY
   LDA   (FP),Y
   STA   HARGS+7
   LDA   #$00
   STA   HARGS+8
   LDY   #$00
   STY   HARGS+9
   STY   HARGS+10
   STY   HARGS+11
   STY   HARGS+12
   STY   HARGS+13
   STA   HARGS+14
   LDA   #$40
   STA   HARGS+15
   JSR   dmul
   LDA   HARGS+16
   LDY   #$0B
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
   LDA   #<globl2
   INY
   STA   (FP),Y
   LDA   #>globl2
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$0B
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$0C
   LDA   (FP),Y
   LDY   #$01
   STA   (DPTR),Y
   LDY   #$0D
   LDA   (FP),Y
   LDY   #$02
   STA   (DPTR),Y
   LDY   #$0E
   LDA   (FP),Y
   LDY   #$03
   STA   (DPTR),Y
   LDY   #$0F
   LDA   (FP),Y
   LDY   #$04
   STA   (DPTR),Y
   LDY   #$10
   LDA   (FP),Y
   LDY   #$05
   STA   (DPTR),Y
   LDY   #$11
   LDA   (FP),Y
   LDY   #$06
   STA   (DPTR),Y
   LDY   #$12
   LDA   (FP),Y
   LDY   #$07
   STA   (DPTR),Y
   LDA   #<globl2
   LDY   #$15
   STA   (FP),Y
   LDA   #>globl2
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$08
   LDY   #$17
   STA   (FP),Y
   DEY
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
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$19
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$1A
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   HARGS
   INY
   LDA   (FP),Y
   STA   HARGS+1
   LDA   #$03
   STA   HARGS+2
   LDA   #$00
   STA   HARGS+3
   JSR   mul16
   LDA   HARGS+4
   LDY   #$1B
   STA   (FP),Y
   LDA   HARGS+5
   INY
   STA   (FP),Y
   LDA   #<globl2
   INY
   STA   (FP),Y
   LDA   #>globl2
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$08
   LDY   #$1F
   STA   (FP),Y
   DEY
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
   LDY   #$1B
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$1C
   LDA   (FP),Y
   LDY   #$01
   STA   (DPTR),Y
   LDA   #<globl2
   LDY   #$21
   STA   (FP),Y
   LDA   #>globl2
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$0A
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
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$26
   STA   (FP),Y
   LDY   #$02
   LDA   (DPTR),Y
   LDY   #$27
   STA   (FP),Y
   LDY   #$03
   LDA   (DPTR),Y
   LDY   #$28
   STA   (FP),Y
   LDA   #$04
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   ORA   #$00
   BPL   .sx_done@1
   LDA   #$FF
.sx_done@1:
   LDY   #$2B
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$25
   LDA   (FP),Y
   STA   HARGS
   INY
   LDA   (FP),Y
   STA   HARGS+1
   INY
   LDA   (FP),Y
   STA   HARGS+2
   INY
   LDA   (FP),Y
   STA   HARGS+3
   INY
   LDA   (FP),Y
   STA   HARGS+4
   INY
   LDA   (FP),Y
   STA   HARGS+5
   INY
   LDA   (FP),Y
   STA   HARGS+6
   INY
   LDA   (FP),Y
   STA   HARGS+7
   JSR   mul32
   LDA   HARGS+8
   LDY   #$2D
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
   LDA   #<globl2
   INY
   STA   (FP),Y
   LDA   #>globl2
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$0A
   LDY   #$33
   STA   (FP),Y
   DEY
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
   LDY   #$37
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDA   globl2
   LDY   #$00
   STA   (DPTR),Y
   LDA   globl2+1
   INY
   STA   (DPTR),Y
   LDA   globl2+2
   INY
   STA   (DPTR),Y
   LDA   globl2+3
   INY
   STA   (DPTR),Y
   LDA   globl2+4
   INY
   STA   (DPTR),Y
   LDA   globl2+5
   INY
   STA   (DPTR),Y
   LDA   globl2+6
   INY
   STA   (DPTR),Y
   LDA   globl2+7
   INY
   STA   (DPTR),Y
   LDA   globl2+8
   INY
   STA   (DPTR),Y
   LDA   globl2+9
   INY
   STA   (DPTR),Y
   LDA   globl2+10
   INY
   STA   (DPTR),Y
   LDA   globl2+11
   INY
   STA   (DPTR),Y
   LDA   globl2+12
   INY
   STA   (DPTR),Y
   LDA   globl2+13
   INY
   STA   (DPTR),Y

   ; epilogue
   CLC
   LDA   FP
   ADC   #$38
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

   CLC
   LDA   FP
   ADC   #$01
   LDY   #$03
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
   LDY   #$04
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   JSR   return_in_reg
   CLC
   LDA   FP
   ADC   #$05
   LDY   #$07
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
   LDY   #$08
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   JSR   return_in_reg
   LDA   #<globl
   LDY   #$09
   STA   (FP),Y
   LDA   #>globl
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
   LDY   #$0B
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$0C
   STA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@2
   DEY
   LDA   (FP),Y
   CMP   #$02
   BNE   .cmp_true@2
   LDA   #$00
   JMP   .cmp_end@3
.cmp_true@2:
   LDA   #$01
.cmp_end@3:
   LDY   #$0D
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
   PLA
   TAX
   PLA
   RTS
.if_end@0:
   CLC
   LDA   FP
   ADC   #$0F
   LDY   #$1D
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
   LDY   #$1E
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   JSR   return_in_mem
   CLC
   LDA   FP
   ADC   #$1F
   LDY   #$2D
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
   LDY   #$2E
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   JSR   return_in_mem
   LDA   #<globl2
   LDY   #$2F
   STA   (FP),Y
   LDA   #>globl2
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
   LDY   #$31
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$32
   STA   (FP),Y
   LDY   #$02
   LDA   (DPTR),Y
   LDY   #$33
   STA   (FP),Y
   LDY   #$03
   LDA   (DPTR),Y
   LDY   #$34
   STA   (FP),Y
   LDY   #$04
   LDA   (DPTR),Y
   LDY   #$35
   STA   (FP),Y
   LDY   #$05
   LDA   (DPTR),Y
   LDY   #$36
   STA   (FP),Y
   LDY   #$06
   LDA   (DPTR),Y
   LDY   #$37
   STA   (FP),Y
   LDY   #$07
   LDA   (DPTR),Y
   LDY   #$38
   STA   (FP),Y
   CMP   #$40
   BNE   .cmp_true@5
   DEY
   LDA   (FP),Y
   CMP   #$14
   BNE   .cmp_true@5
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@5
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@5
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@5
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@5
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@5
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@5
   LDA   #$00
   JMP   .cmp_end@6
.cmp_true@5:
   LDA   #$01
.cmp_end@6:
   LDY   #$39
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@3
   LDA   #<globl2
   INY
   STA   (FP),Y
   LDA   #>globl2
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$08
   LDY   #$3D
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$3E
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$3F
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$40
   STA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@8
   DEY
   LDA   (FP),Y
   CMP   #$12
   BNE   .cmp_true@8
   LDA   #$00
   JMP   .cmp_end@9
.cmp_true@8:
   LDA   #$01
.cmp_end@9:
   LDY   #$41
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@3
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   JMP   .or_end@4
.or_true@3:
   LDA   #$01
   LDY   #$43
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@4:
   LDY   #$43
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@0
   JMP   .or_true@1
.lb_skip@0:
   LDA   #<globl2
   LDY   #$45
   STA   (FP),Y
   LDA   #>globl2
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$0A
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
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$4A
   STA   (FP),Y
   LDY   #$02
   LDA   (DPTR),Y
   LDY   #$4B
   STA   (FP),Y
   LDY   #$03
   LDA   (DPTR),Y
   LDY   #$4C
   STA   (FP),Y
   LDA   #$C0
   INY
   STA   (FP),Y
   LDA   #$12
   INY
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@11
   LDA   #$00
   JMP   .sx_done@12
.sx_neg@11:
   LDA   #$FF
.sx_done@12:
   LDY   #$4F
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$4C
   LDA   (FP),Y
   LDY   #$50
   CMP   (FP),Y
   BNE   .cmp_true@13
   LDY   #$4B
   LDA   (FP),Y
   LDY   #$4F
   CMP   (FP),Y
   BNE   .cmp_true@13
   LDY   #$4A
   LDA   (FP),Y
   LDY   #$4E
   CMP   (FP),Y
   BNE   .cmp_true@13
   LDY   #$49
   LDA   (FP),Y
   LDY   #$4D
   CMP   (FP),Y
   BNE   .cmp_true@13
   LDA   #$00
   JMP   .cmp_end@14
.cmp_true@13:
   LDA   #$01
.cmp_end@14:
   LDY   #$51
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@1
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   JMP   .or_end@2
.or_true@1:
   LDA   #$01
   LDY   #$53
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@2:
   LDY   #$53
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@5
   LDX   #$00
   LDA   #$02

   ; epilogue
   PHA
   TXA
   PHA
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
   PLA
   TAX
   PLA
   RTS
.if_end@5:
   LDX   #$00
   TXA

   ; epilogue
   PHA
   TXA
   PHA
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
   PLA
   TAX
   PLA
   RTS

globl:
   DS.B  2

globl2:
   DC.L  $00000000
   DC.L  $3FF40000
   DC.W  $0002
   DC.L  $0000012C
