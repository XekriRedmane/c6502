SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24

clear_page1:
   SUBROUTINE

   ; prologue: 2 arg bytes, 61 local bytes
   SEC
   LDA   SSP
   SBC   #$3F
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$3E
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
   LDY   #$01
   STA   (FP),Y
.loop@0_start:
   LDA   #$00
   LDY   #$02
   STA   (FP),Y
   INY
   STA   (FP),Y
.loop@1_start:
   LDA   #$22
   CLC
   LDY   #$02
   SBC   (FP),Y
   LDA   #$00
   INY
   SBC   (FP),Y
   LDA   #$00
   ROL
   INY
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
   LDA   #$00
   LDY   #$06
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   LDY   #$07
   STA   (FP),Y
   LDY   #$03
   LDA   (FP),Y
   LDY   #$08
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$09
   STA   (FP),Y
   STA   HARGS
   LDA   #$02
   STA   HARGS+1
   JSR   mul8
   LDA   HARGS+2
   LDY   #$0A
   STA   (FP),Y
   TAX
   LDA   STRIPE_ROW,X
   INY
   STA   (FP),Y
   LDA   STRIPE_ROW+1,X
   INY
   STA   (FP),Y
   LDY   #$01
   LDA   (FP),Y
   LDY   #$0D
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$0B
   LDA   (FP),Y
   CLC
   LDY   #$0D
   ADC   (FP),Y
   LDY   #$0F
   STA   (FP),Y
   LDY   #$0C
   LDA   (FP),Y
   LDY   #$0E
   ADC   (FP),Y
   LDY   #$10
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
   LDY   #$02
   LDA   (FP),Y
   LDY   #$11
   STA   (FP),Y
   LDY   #$03
   LDA   (FP),Y
   LDY   #$12
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   CLC
   ADC   #$01
   LDY   #$13
   STA   (FP),Y
   LDY   #$03
   LDA   (FP),Y
   ADC   #$00
   LDY   #$14
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$02
   STA   (FP),Y
   LDY   #$14
   LDA   (FP),Y
   LDY   #$03
   STA   (FP),Y
   JMP   .loop@1_start
.loop@1_break:
   LDA   #$00
   LDY   #$15
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$02
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDY   #$01
   LDA   (FP),Y
   STA   (SSP),Y
   LDY   #$15
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   JSR   interlace_fill_p1
   LDY   #$01
   LDA   (FP),Y
   LDY   #$16
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$41
   LDA   (FP),Y
   LDY   #$18
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$16
   LDA   (FP),Y
   LDY   #$18
   CMP   (FP),Y
   BEQ   .cmp_true@0
   LDA   #$00
   JMP   .cmp_end@1
.cmp_true@0:
   LDA   #$01
.cmp_end@1:
   LDY   #$1A
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .loop@0_break
   LDY   #$01
   LDA   (FP),Y
   LDY   #$1C
   STA   (FP),Y
   SEC
   SBC   #$01
   INY
   STA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   JMP   .loop@0_start
.loop@0_break:
   LDA   #$27
   LDY   #$1E
   STA   (FP),Y
   INY
   STA   (FP),Y
.loop@2_start:
   LDY   #$20
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDA   TEXT_STRIP_SRC
   CLC
   DEY
   ADC   (FP),Y
   LDY   #$22
   STA   (FP),Y
   LDA   TEXT_STRIP_SRC+1
   DEY
   ADC   (FP),Y
   LDY   #$23
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$24
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
.loop@3_start:
   LDA   #$0C
   CLC
   LDY   #$26
   SBC   (FP),Y
   LDA   #$00
   INY
   SBC   (FP),Y
   LDA   #$00
   ROL
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@0
   JMP   .loop@3_break
.lb_skip@0:
   LDY   #$26
   LDA   (FP),Y
   LDY   #$2A
   STA   (FP),Y
   LDY   #$27
   LDA   (FP),Y
   LDY   #$2B
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$2C
   STA   (FP),Y
   STA   HARGS
   LDA   #$02
   STA   HARGS+1
   JSR   mul8
   LDA   HARGS+2
   LDY   #$2D
   STA   (FP),Y
   TAX
   LDA   TEXT_ROW,X
   INY
   STA   (FP),Y
   LDA   TEXT_ROW+1,X
   INY
   STA   (FP),Y
   LDY   #$1F
   LDA   (FP),Y
   LDY   #$30
   STA   (FP),Y
   LDA   #$00
   INY
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
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$25
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$26
   LDA   (FP),Y
   LDY   #$34
   STA   (FP),Y
   LDY   #$27
   LDA   (FP),Y
   LDY   #$35
   STA   (FP),Y
   LDY   #$26
   LDA   (FP),Y
   CLC
   ADC   #$01
   LDY   #$36
   STA   (FP),Y
   LDY   #$27
   LDA   (FP),Y
   ADC   #$00
   LDY   #$37
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$26
   STA   (FP),Y
   LDY   #$37
   LDA   (FP),Y
   LDY   #$27
   STA   (FP),Y
   JMP   .loop@3_start
.loop@3_break:
   LDY   #$1F
   LDA   (FP),Y
   LDY   #$38
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   BEQ   .cmp_true@2
   LDA   #$00
   JMP   .cmp_end@3
.cmp_true@2:
   LDA   #$01
.cmp_end@3:
   LDY   #$3A
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .loop@2_break
   LDY   #$1F
   LDA   (FP),Y
   LDY   #$3C
   STA   (FP),Y
   SEC
   SBC   #$01
   INY
   STA   (FP),Y
   LDY   #$1F
   STA   (FP),Y
   JMP   .loop@2_start
.loop@2_break:

   ; epilogue
   CLC
   LDA   FP
   ADC   #$41
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$3E
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   RTS

TEXT_STRIP_SRC:
   DC.W  $0300

STRIPE_ROW:
   DC.W  $2600
   DC.W  $2A00
   DC.W  $2E00
   DC.W  $3200
   DC.W  $3600
   DC.W  $3A00
   DC.W  $3E00
   DC.W  $2280
   DC.W  $2680
   DC.W  $2A80
   DC.W  $2E80
   DC.W  $3280
   DC.W  $3680
   DC.W  $3A80
   DC.W  $3E80
   DC.W  $2300
   DC.W  $2700
   DC.W  $2B00
   DC.W  $2F00
   DC.W  $3300
   DC.W  $3700
   DC.W  $3B00
   DC.W  $3F00
   DC.W  $2380
   DC.W  $2780
   DC.W  $2B80
   DC.W  $2F80
   DC.W  $3380
   DC.W  $3780
   DC.W  $3B80
   DC.W  $3F80
   DC.W  $2028
   DC.W  $2428
   DC.W  $2828

TEXT_ROW:
   DC.W  $3028
   DC.W  $3428
   DC.W  $3828
   DC.W  $3C28
   DC.W  $32A8
   DC.W  $36A8
   DC.W  $3AA8
   DC.W  $3EA8
   DC.W  $3150
   DC.W  $3550
   DC.W  $3950
   DC.W  $3D50
