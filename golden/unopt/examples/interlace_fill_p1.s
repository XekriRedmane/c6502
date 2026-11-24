SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24

interlace_fill_p1:
   SUBROUTINE

   ; prologue: 2 arg bytes, 24 local bytes
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

   LDA   #$00
   LDY   #$01
   STA   (FP),Y
   INY
   STA   (FP),Y
.loop@0_start:
   LDY   #$03
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   SEC
   SBC   #$69
   INY
   LDA   (FP),Y
   SBC   #$00
   BVC   .cmp_novf@0
   EOR   #$80
.cmp_novf@0:
   ASL
   LDA   #$00
   ROL
   LDY   #$05
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@0
   JMP   .loop@0_break
.lb_skip@0:
   LDY   #$02
   LDA   (FP),Y
   LDY   #$07
   STA   (FP),Y
   LDA   #$00
   INY
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
   LDA   interlace_p1_offsets,X
   INY
   STA   (FP),Y
   LDA   interlace_p1_offsets+1,X
   INY
   STA   (FP),Y
   LDY   #$1C
   LDA   (FP),Y
   LDY   #$0D
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$0F
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$10
   STA   (FP),Y
   LDY   #$0B
   LDA   (FP),Y
   CLC
   LDY   #$0F
   ADC   (FP),Y
   LDY   #$11
   STA   (FP),Y
   LDY   #$0C
   LDA   (FP),Y
   LDY   #$10
   ADC   (FP),Y
   LDY   #$12
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$13
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$14
   STA   (FP),Y
   LDA   hires_page1
   CLC
   DEY
   ADC   (FP),Y
   LDY   #$15
   STA   (FP),Y
   LDA   hires_page1+1
   DEY
   ADC   (FP),Y
   LDY   #$16
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
   LDY   #$17
   STA   (FP),Y
   CLC
   ADC   #$01
   INY
   STA   (FP),Y
   LDY   #$02
   STA   (FP),Y
   JMP   .loop@0_start
.loop@0_break:

   ; epilogue
   CLC
   LDA   FP
   ADC   #$1C
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$19
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   RTS

hires_page1:
   DC.W  $2000

interlace_p1_offsets:
   DC.W  $00A8
   DC.W  $0328
   DC.W  $01D0
   DC.W  $04A8
   DC.W  $0728
   DC.W  $05D0
   DC.W  $08A8
   DC.W  $0B28
   DC.W  $09D0
   DC.W  $0CA8
   DC.W  $0F28
   DC.W  $0DD0
   DC.W  $10A8
   DC.W  $1328
   DC.W  $11D0
   DC.W  $14A8
   DC.W  $1728
   DC.W  $15D0
   DC.W  $18A8
   DC.W  $1B28
   DC.W  $19D0
   DC.W  $1CA8
   DC.W  $1F28
   DC.W  $1DD0
   DC.W  $0128
   DC.W  $03A8
   DC.W  $0250
   DC.W  $0528
   DC.W  $07A8
   DC.W  $0650
   DC.W  $0928
   DC.W  $0BA8
   DC.W  $0A50
   DC.W  $0D28
   DC.W  $0FA8
   DC.W  $0E50
   DC.W  $1128
   DC.W  $13A8
   DC.W  $1250
   DC.W  $1528
   DC.W  $17A8
   DC.W  $1650
   DC.W  $1928
   DC.W  $1BA8
   DC.W  $1A50
   DC.W  $1D28
   DC.W  $1FA8
   DC.W  $1E50
   DC.W  $01A8
   DC.W  $0050
   DC.W  $02D0
   DC.W  $05A8
   DC.W  $0450
   DC.W  $06D0
   DC.W  $09A8
   DC.W  $0850
   DC.W  $0AD0
   DC.W  $0DA8
   DC.W  $0C50
   DC.W  $0ED0
   DC.W  $11A8
   DC.W  $1050
   DC.W  $12D0
   DC.W  $15A8
   DC.W  $1450
   DC.W  $16D0
   DC.W  $19A8
   DC.W  $1850
   DC.W  $1AD0
   DC.W  $1DA8
   DC.W  $1C50
   DC.W  $1ED0
   DC.W  $0228
   DC.W  $00D0
   DC.W  $0350
   DC.W  $0628
   DC.W  $04D0
   DC.W  $0750
   DC.W  $0A28
   DC.W  $08D0
   DC.W  $0B50
   DC.W  $0E28
   DC.W  $0CD0
   DC.W  $0F50
   DC.W  $1228
   DC.W  $10D0
   DC.W  $1350
   DC.W  $1628
   DC.W  $14D0
   DC.W  $1750
   DC.W  $1A28
   DC.W  $18D0
   DC.W  $1B50
   DC.W  $1E28
   DC.W  $1CD0
   DC.W  $1F50
   DC.W  $02A8
   DC.W  $0150
   DC.W  $03D0
   DC.W  $06A8
   DC.W  $0550
   DC.W  $07D0
   DC.W  $0AA8
   DC.W  $0950
   DC.W  $0BD0
