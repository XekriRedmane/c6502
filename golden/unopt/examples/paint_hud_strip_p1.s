SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24

paint_hud_strip_p1:
   SUBROUTINE

   ; prologue: 0 arg bytes, 70 local bytes
   SEC
   LDA   SSP
   SBC   #$48
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$47
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
   LDA   #$0F
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
.loop@0_start:
   LDY   #$05
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@0
   LDA   #$00
   JMP   .sx_done@1
.sx_neg@0:
   LDA   #$FF
.sx_done@1:
   LDY   #$06
   STA   (FP),Y
   ASL
   LDA   #$00
   ROL
   EOR   #$01
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@2
   JMP   .loop@0_break
.lb_skip@2:
   LDA   #$00
   LDY   #$09
   STA   (FP),Y
   INY
   STA   (FP),Y
.loop@1_start:
   LDY   #$0B
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   SEC
   SBC   #$07
   INY
   LDA   (FP),Y
   SBC   #$00
   BVC   .cmp_novf@2
   EOR   #$80
.cmp_novf@2:
   ASL
   LDA   #$00
   ROL
   LDY   #$0D
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
   LDY   #$0F
   STA   (FP),Y
   CLC
   ADC   #$01
   INY
   STA   (FP),Y
   LDY   #$02
   STA   (FP),Y
   LDY   #$0F
   LDA   (FP),Y
   LDY   #$11
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDA   HUD_STRIP_SRC
   CLC
   DEY
   ADC   (FP),Y
   LDY   #$13
   STA   (FP),Y
   LDA   HUD_STRIP_SRC+1
   DEY
   ADC   (FP),Y
   LDY   #$14
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$15
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
.loop@2_start:
   LDY   #$19
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$0A
   LDA   (FP),Y
   LDY   #$1B
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$1D
   STA   (FP),Y
   TAX
   LDA   HUD_ROW_COUNT,X
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$19
   LDA   (FP),Y
   SEC
   LDY   #$1F
   SBC   (FP),Y
   LDY   #$1A
   LDA   (FP),Y
   LDY   #$20
   SBC   (FP),Y
   BVC   .cmp_novf@3
   EOR   #$80
.cmp_novf@3:
   ASL
   LDA   #$00
   ROL
   LDY   #$21
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@0
   JMP   .loop@2_break
.lb_skip@0:
   LDA   #<HUD_ROW_GROUPS
   LDY   #$23
   STA   (FP),Y
   LDA   #>HUD_ROW_GROUPS
   INY
   STA   (FP),Y
   LDY   #$0A
   LDA   (FP),Y
   LDY   #$25
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$27
   STA   (FP),Y
   STA   HARGS
   LDA   #$07
   STA   HARGS+1
   JSR   mul8
   LDA   HARGS+2
   LDY   #$28
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$23
   LDA   (FP),Y
   CLC
   LDY   #$29
   ADC   (FP),Y
   LDY   #$2B
   STA   (FP),Y
   LDY   #$24
   LDA   (FP),Y
   LDY   #$2A
   ADC   (FP),Y
   LDY   #$2C
   STA   (FP),Y
   LDY   #$18
   LDA   (FP),Y
   LDY   #$2D
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$2F
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$2B
   LDA   (FP),Y
   CLC
   LDY   #$30
   ADC   (FP),Y
   LDY   #$32
   STA   (FP),Y
   LDY   #$2C
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
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$34
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$37
   STA   (FP),Y
   STA   HARGS
   LDA   #$02
   STA   HARGS+1
   JSR   mul8
   LDA   HARGS+2
   LDY   #$38
   STA   (FP),Y
   TAX
   LDA   HGR1_ROW,X
   INY
   STA   (FP),Y
   LDA   HGR1_ROW+1,X
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$0C
   LDY   #$3B
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$3C
   STA   (FP),Y
   LDY   #$04
   LDA   (FP),Y
   LDY   #$3D
   STA   (FP),Y
   ORA   #$00
   BMI   .sx_neg@4
   LDA   #$00
   JMP   .sx_done@5
.sx_neg@4:
   LDA   #$FF
.sx_done@5:
   LDY   #$3E
   STA   (FP),Y
   LDY   #$3B
   LDA   (FP),Y
   CLC
   LDY   #$3D
   ADC   (FP),Y
   LDY   #$3F
   STA   (FP),Y
   LDY   #$3C
   LDA   (FP),Y
   LDY   #$3E
   ADC   (FP),Y
   LDY   #$40
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$16
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$18
   LDA   (FP),Y
   LDY   #$41
   STA   (FP),Y
   CLC
   ADC   #$01
   INY
   STA   (FP),Y
   LDY   #$18
   STA   (FP),Y
   JMP   .loop@2_start
.loop@2_break:
   LDY   #$0A
   LDA   (FP),Y
   LDY   #$43
   STA   (FP),Y
   CLC
   ADC   #$01
   INY
   STA   (FP),Y
   LDY   #$0A
   STA   (FP),Y
   JMP   .loop@1_start
.loop@1_break:
   LDY   #$04
   LDA   (FP),Y
   LDY   #$45
   STA   (FP),Y
   SEC
   SBC   #$01
   INY
   STA   (FP),Y
   LDY   #$04
   STA   (FP),Y
   JMP   .loop@0_start
.loop@0_break:

   ; epilogue
   CLC
   LDA   FP
   ADC   #$48
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$47
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   RTS

HGR1_ROW:
   DC.W  $2000
   DC.W  $2400
   DC.W  $2800
   DC.W  $2C00
   DC.W  $3000
   DC.W  $3400
   DC.W  $3800
   DC.W  $3C00
   DC.W  $2080
   DC.W  $2480
   DC.W  $2880
   DC.W  $2C80
   DC.W  $3080
   DC.W  $3480
   DC.W  $3880
   DC.W  $3C80
   DC.W  $2100
   DC.W  $2500
   DC.W  $2900
   DC.W  $2D00
   DC.W  $3100
   DC.W  $3500
   DC.W  $3900
   DC.W  $3D00
   DC.W  $2180
   DC.W  $2580
   DC.W  $2980
   DC.W  $2D80
   DC.W  $3180
   DC.W  $3580
   DC.W  $3980
   DC.W  $3D80

HUD_ROW_GROUPS:
   DC.B  $01, $02, $03, $04, $05, $06, $07
   DC.B  $08, $00, $00, $00, $00, $00, $00
   DC.B  $09, $0A, $0B, $0C, $0D, $0E, $0F
   DC.B  $10, $00, $00, $00, $00, $00, $00
   DC.B  $11, $12, $13, $14, $15, $16, $17
   DC.B  $18, $00, $00, $00, $00, $00, $00
   DC.B  $19, $1A, $1B, $1C, $1D, $1E, $1F

HUD_ROW_COUNT:
   DC.B  $07, $01, $07, $01, $07, $01, $07

HUD_STRIP_SRC:
   DC.W  $A30D
