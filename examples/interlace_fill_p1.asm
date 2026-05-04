interlace_fill_p1:
   SUBROUTINE

.interlace_fill_p1@asm_ssa_preheader@0:
.interlace_fill_p1@ssa_block@0:
   LDA   #$00
   STA   $87
.loop@0_start:
   LDA   $87
   STA   $83
   LDA   #$00
   STA   $82
   LDA   $83
   SEC
   SBC   #$69
   LDA   $82
   SBC   #$00
   BVC   .cmp_novf@0
.interlace_fill_p1@asm_ssa_block@0:
   EOR   #$80
.cmp_novf@0:
   BMI   .cmp_true@1
.interlace_fill_p1@asm_ssa_block@1:
   LDA   #$00
   JMP   .cmp_end@2
.cmp_true@1:
   LDA   #$01
.cmp_end@2:
   STA   $82
   LDA   $82
   ORA   #$00
   BEQ   .loop@0_break
.interlace_fill_p1@asm_ssa_block@2:
   LDA   #<interlace_p1_offsets
   STA   $85
   LDA   #>interlace_p1_offsets
   STA   $86
   LDA   $87
   STA   $82
   LDA   #$00
   STA   $84
   ASL   $82
   LDA   $84
   STA   $83
   ROL   $83
   LDA   $85
   CLC
   ADC   $82
   STA   DPTR
   LDA   $86
   ADC   $83
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   STA   $85
   LDY   #$01
   LDA   (DPTR),Y
   STA   $84
   LDA   $81
   STA   $83
   LDA   #$00
   STA   $82
   LDA   $85
   CLC
   ADC   $83
   STA   $83
   LDA   $84
   ADC   $82
   STA   $82
   LDA   hires_page1
   CLC
   ADC   $83
   STA   DPTR
   LDA   hires_page1+1
   ADC   $82
   STA   DPTR+1
   LDA   $80
   LDY   #$00
   STA   (DPTR),Y
.loop@0_continue:
   LDA   $87
   CLC
   ADC   #$01
   STA   $82
   LDA   $82
   STA   $87
   JMP   .loop@0_start
.loop@0_break:
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
