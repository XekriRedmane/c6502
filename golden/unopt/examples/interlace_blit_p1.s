SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24

interlace_blit_p1:
   SUBROUTINE

   ; prologue: 4 arg bytes, 99 local bytes
   SEC
   LDA   SSP
   SBC   #$65
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$64
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
   LDY   #$68
   LDA   (FP),Y
   LDY   #$03
   STA   (FP),Y
.loop@0_start:
   LDY   #$03
   LDA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   SEC
   SBC   #$28
   INY
   LDA   (FP),Y
   SBC   #$00
   BVC   .cmp_novf@0
   EOR   #$80
.cmp_novf@0:
   ASL
   LDA   #$00
   ROL
   EOR   #$01
   LDY   #$06
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_else@1
   LDA   #$23
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   LDY   #$0B
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   LDY   #$09
   ADC   (FP),Y
   LDY   #$0D
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$0A
   ADC   (FP),Y
   LDY   #$0E
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$0F
   STA   (FP),Y
   LDY   #$02
   STA   (FP),Y
   JMP   .if_end@0
.if_else@1:
   LDA   #$00
   LDY   #$10
   STA   (FP),Y
   INY
   STA   (FP),Y
.loop@1_start:
   LDY   #$12
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   SEC
   SBC   #$23
   INY
   LDA   (FP),Y
   SBC   #$00
   BVC   .cmp_novf@1
   EOR   #$80
.cmp_novf@1:
   ASL
   LDA   #$00
   ROL
   LDY   #$14
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
   LDY   #$02
   LDA   (FP),Y
   LDY   #$16
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$66
   LDA   (FP),Y
   CLC
   LDY   #$16
   ADC   (FP),Y
   LDY   #$18
   STA   (FP),Y
   LDY   #$67
   LDA   (FP),Y
   LDY   #$17
   ADC   (FP),Y
   LDY   #$19
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$1A
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #<HIRES_P1_ROW
   INY
   STA   (FP),Y
   LDA   #>HIRES_P1_ROW
   INY
   STA   (FP),Y
   LDY   #$11
   LDA   (FP),Y
   LDY   #$1E
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$20
   STA   (FP),Y
   STA   HARGS
   LDA   #$06
   STA   HARGS+1
   JSR   mul8
   LDA   HARGS+2
   LDY   #$21
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$1C
   LDA   (FP),Y
   CLC
   LDY   #$22
   ADC   (FP),Y
   LDY   #$24
   STA   (FP),Y
   LDY   #$1D
   LDA   (FP),Y
   LDY   #$23
   ADC   (FP),Y
   LDY   #$25
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
   LDY   #$27
   STA   (FP),Y
   LDA   HARGS
   DEY
   STA   (FP),Y
   LDY   #$24
   LDA   (FP),Y
   CLC
   LDY   #$26
   ADC   (FP),Y
   LDY   #$28
   STA   (FP),Y
   LDY   #$25
   LDA   (FP),Y
   LDY   #$27
   ADC   (FP),Y
   LDY   #$29
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$2A
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$2B
   STA   (FP),Y
   LDY   #$03
   LDA   (FP),Y
   LDY   #$2C
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$2A
   LDA   (FP),Y
   CLC
   LDY   #$2C
   ADC   (FP),Y
   LDY   #$2E
   STA   (FP),Y
   LDY   #$2B
   LDA   (FP),Y
   LDY   #$2D
   ADC   (FP),Y
   LDY   #$2F
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
   LDA   #<HIRES_P1_ROW
   LDY   #$30
   STA   (FP),Y
   LDA   #>HIRES_P1_ROW
   INY
   STA   (FP),Y
   LDY   #$11
   LDA   (FP),Y
   LDY   #$32
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$34
   STA   (FP),Y
   STA   HARGS
   LDA   #$06
   STA   HARGS+1
   JSR   mul8
   LDA   HARGS+2
   LDY   #$35
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$30
   LDA   (FP),Y
   CLC
   LDY   #$36
   ADC   (FP),Y
   LDY   #$38
   STA   (FP),Y
   LDY   #$31
   LDA   (FP),Y
   LDY   #$37
   ADC   (FP),Y
   LDY   #$39
   STA   (FP),Y
   LDA   #$01
   STA   HARGS
   LDA   #$00
   LDX   #$08
   LSR   HARGS
.mul_loop@4:
   BCC   .mul_skip@5
   CLC
   ADC   #$02
.mul_skip@5:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@4
   LDY   #$3B
   STA   (FP),Y
   LDA   HARGS
   DEY
   STA   (FP),Y
   LDY   #$38
   LDA   (FP),Y
   CLC
   LDY   #$3A
   ADC   (FP),Y
   LDY   #$3C
   STA   (FP),Y
   LDY   #$39
   LDA   (FP),Y
   LDY   #$3B
   ADC   (FP),Y
   LDY   #$3D
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$3E
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$3F
   STA   (FP),Y
   LDY   #$03
   LDA   (FP),Y
   LDY   #$40
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$3E
   LDA   (FP),Y
   CLC
   LDY   #$40
   ADC   (FP),Y
   LDY   #$42
   STA   (FP),Y
   LDY   #$3F
   LDA   (FP),Y
   LDY   #$41
   ADC   (FP),Y
   LDY   #$43
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
   LDA   #<HIRES_P1_ROW
   LDY   #$44
   STA   (FP),Y
   LDA   #>HIRES_P1_ROW
   INY
   STA   (FP),Y
   LDY   #$11
   LDA   (FP),Y
   LDY   #$46
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$48
   STA   (FP),Y
   STA   HARGS
   LDA   #$06
   STA   HARGS+1
   JSR   mul8
   LDA   HARGS+2
   LDY   #$49
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$44
   LDA   (FP),Y
   CLC
   LDY   #$4A
   ADC   (FP),Y
   LDY   #$4C
   STA   (FP),Y
   LDY   #$45
   LDA   (FP),Y
   LDY   #$4B
   ADC   (FP),Y
   LDY   #$4D
   STA   (FP),Y
   LDA   #$02
   STA   HARGS
   LDA   #$00
   LDX   #$08
   LSR   HARGS
.mul_loop@6:
   BCC   .mul_skip@7
   CLC
   ADC   #$02
.mul_skip@7:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@6
   LDY   #$4F
   STA   (FP),Y
   LDA   HARGS
   DEY
   STA   (FP),Y
   LDY   #$4C
   LDA   (FP),Y
   CLC
   LDY   #$4E
   ADC   (FP),Y
   LDY   #$50
   STA   (FP),Y
   LDY   #$4D
   LDA   (FP),Y
   LDY   #$4F
   ADC   (FP),Y
   LDY   #$51
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$52
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$53
   STA   (FP),Y
   LDY   #$03
   LDA   (FP),Y
   LDY   #$54
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$52
   LDA   (FP),Y
   CLC
   LDY   #$54
   ADC   (FP),Y
   LDY   #$56
   STA   (FP),Y
   LDY   #$53
   LDA   (FP),Y
   LDY   #$55
   ADC   (FP),Y
   LDY   #$57
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
   LDY   #$02
   LDA   (FP),Y
   LDY   #$58
   STA   (FP),Y
   CLC
   ADC   #$01
   INY
   STA   (FP),Y
   LDY   #$02
   STA   (FP),Y
   LDY   #$11
   LDA   (FP),Y
   LDY   #$5A
   STA   (FP),Y
   CLC
   ADC   #$01
   INY
   STA   (FP),Y
   LDY   #$11
   STA   (FP),Y
   JMP   .loop@1_start
.loop@1_break:
.if_end@0:
   LDY   #$03
   LDA   (FP),Y
   LDY   #$5C
   STA   (FP),Y
   SEC
   SBC   #$01
   INY
   STA   (FP),Y
   LDY   #$03
   STA   (FP),Y
   LDY   #$5E
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$69
   LDA   (FP),Y
   LDY   #$60
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$5E
   LDA   (FP),Y
   LDY   #$60
   CMP   (FP),Y
   BNE   .cmp_true@8
   LDA   #$00
   JMP   .cmp_end@9
.cmp_true@8:
   LDA   #$01
.cmp_end@9:
   LDY   #$62
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

   ; epilogue
   CLC
   LDA   FP
   ADC   #$69
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$64
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   RTS

HIRES_P1_ROW:
   DC.W  $20A8
   DC.W  $2328
   DC.W  $21D0
   DC.W  $24A8
   DC.W  $2728
   DC.W  $25D0
   DC.W  $28A8
   DC.W  $2B28
   DC.W  $29D0
   DC.W  $2CA8
   DC.W  $2F28
   DC.W  $2DD0
   DC.W  $30A8
   DC.W  $3328
   DC.W  $31D0
   DC.W  $34A8
   DC.W  $3728
   DC.W  $35D0
   DC.W  $38A8
   DC.W  $3B28
   DC.W  $39D0
   DC.W  $3CA8
   DC.W  $3F28
   DC.W  $3DD0
   DC.W  $2128
   DC.W  $23A8
   DC.W  $2250
   DC.W  $2528
   DC.W  $27A8
   DC.W  $2650
   DC.W  $2928
   DC.W  $2BA8
   DC.W  $2A50
   DC.W  $2D28
   DC.W  $2FA8
   DC.W  $2E50
   DC.W  $3128
   DC.W  $33A8
   DC.W  $3250
   DC.W  $3528
   DC.W  $37A8
   DC.W  $3650
   DC.W  $3928
   DC.W  $3BA8
   DC.W  $3A50
   DC.W  $3D28
   DC.W  $3FA8
   DC.W  $3E50
   DC.W  $21A8
   DC.W  $2050
   DC.W  $22D0
   DC.W  $25A8
   DC.W  $2450
   DC.W  $26D0
   DC.W  $29A8
   DC.W  $2850
   DC.W  $2AD0
   DC.W  $2DA8
   DC.W  $2C50
   DC.W  $2ED0
   DC.W  $31A8
   DC.W  $3050
   DC.W  $32D0
   DC.W  $35A8
   DC.W  $3450
   DC.W  $36D0
   DC.W  $39A8
   DC.W  $3850
   DC.W  $3AD0
   DC.W  $3DA8
   DC.W  $3C50
   DC.W  $3ED0
   DC.W  $2228
   DC.W  $20D0
   DC.W  $2350
   DC.W  $2628
   DC.W  $24D0
   DC.W  $2750
   DC.W  $2A28
   DC.W  $28D0
   DC.W  $2B50
   DC.W  $2E28
   DC.W  $2CD0
   DC.W  $2F50
   DC.W  $3228
   DC.W  $30D0
   DC.W  $3350
   DC.W  $3628
   DC.W  $34D0
   DC.W  $3750
   DC.W  $3A28
   DC.W  $38D0
   DC.W  $3B50
   DC.W  $3E28
   DC.W  $3CD0
   DC.W  $3F50
   DC.W  $22A8
   DC.W  $2150
   DC.W  $23D0
   DC.W  $26A8
   DC.W  $2550
   DC.W  $27D0
   DC.W  $2AA8
   DC.W  $2950
   DC.W  $2BD0
