interlace_blit_p1:
   SUBROUTINE

.interlace_blit_p1@asm_ssa_preheader@0:
.interlace_blit_p1@ssa_block@0:
   LDY   #$00
   LDX   $82
.loop@0_start:
   TXA
   CMP   #$28
   BCC   .if_else@1
.interlace_blit_p1@ssa_block@1:
   TYA
   CLC
   ADC   #$23
   TAY
   LDA   #$00
   ADC   #$00
   JMP   .if_end@0
.if_else@1:
   LDA   $80
   STA   DPTR
   LDA   $81
   STA   DPTR+1
   LDA   (DPTR),Y
   STA   $20A8,X
   STA   $2328,X
   STA   $21D0,X
   INY
   LDA   (DPTR),Y
   STA   $24A8,X
   STA   $2728,X
   STA   $25D0,X
   INY
   LDA   (DPTR),Y
   STA   $28A8,X
   STA   $2B28,X
   STA   $29D0,X
   INY
   LDA   (DPTR),Y
   STA   $2CA8,X
   STA   $2F28,X
   STA   $2DD0,X
   INY
   LDA   (DPTR),Y
   STA   $30A8,X
   STA   $3328,X
   STA   $31D0,X
   INY
   LDA   (DPTR),Y
   STA   $34A8,X
   STA   $3728,X
   STA   $35D0,X
   INY
   LDA   (DPTR),Y
   STA   $38A8,X
   STA   $3B28,X
   STA   $39D0,X
   INY
   LDA   (DPTR),Y
   STA   $3CA8,X
   STA   $3F28,X
   STA   $3DD0,X
   INY
   LDA   (DPTR),Y
   STA   $2128,X
   STA   $23A8,X
   STA   $2250,X
   INY
   LDA   (DPTR),Y
   STA   $2528,X
   STA   $27A8,X
   STA   $2650,X
   INY
   LDA   (DPTR),Y
   STA   $2928,X
   STA   $2BA8,X
   STA   $2A50,X
   INY
   LDA   (DPTR),Y
   STA   $2D28,X
   STA   $2FA8,X
   STA   $2E50,X
   INY
   LDA   (DPTR),Y
   STA   $3128,X
   STA   $33A8,X
   STA   $3250,X
   INY
   LDA   (DPTR),Y
   STA   $3528,X
   STA   $37A8,X
   STA   $3650,X
   INY
   LDA   (DPTR),Y
   STA   $3928,X
   STA   $3BA8,X
   STA   $3A50,X
   INY
   LDA   (DPTR),Y
   STA   $3D28,X
   STA   $3FA8,X
   STA   $3E50,X
   INY
   LDA   (DPTR),Y
   STA   $21A8,X
   STA   $2050,X
   STA   $22D0,X
   INY
   LDA   (DPTR),Y
   STA   $25A8,X
   STA   $2450,X
   STA   $26D0,X
   INY
   LDA   (DPTR),Y
   STA   $29A8,X
   STA   $2850,X
   STA   $2AD0,X
   INY
   LDA   (DPTR),Y
   STA   $2DA8,X
   STA   $2C50,X
   STA   $2ED0,X
   INY
   LDA   (DPTR),Y
   STA   $31A8,X
   STA   $3050,X
   STA   $32D0,X
   INY
   LDA   (DPTR),Y
   STA   $35A8,X
   STA   $3450,X
   STA   $36D0,X
   INY
   LDA   (DPTR),Y
   STA   $39A8,X
   STA   $3850,X
   STA   $3AD0,X
   INY
   LDA   (DPTR),Y
   STA   $3DA8,X
   STA   $3C50,X
   STA   $3ED0,X
   INY
   LDA   (DPTR),Y
   STA   $2228,X
   STA   $20D0,X
   STA   $2350,X
   INY
   LDA   (DPTR),Y
   STA   $2628,X
   STA   $24D0,X
   STA   $2750,X
   INY
   LDA   (DPTR),Y
   STA   $2A28,X
   STA   $28D0,X
   STA   $2B50,X
   INY
   LDA   (DPTR),Y
   STA   $2E28,X
   STA   $2CD0,X
   STA   $2F50,X
   INY
   LDA   (DPTR),Y
   STA   $3228,X
   STA   $30D0,X
   STA   $3350,X
   INY
   LDA   (DPTR),Y
   STA   $3628,X
   STA   $34D0,X
   STA   $3750,X
   INY
   LDA   (DPTR),Y
   STA   $3A28,X
   STA   $38D0,X
   STA   $3B50,X
   INY
   LDA   (DPTR),Y
   STA   $3E28,X
   STA   $3CD0,X
   STA   $3F50,X
   INY
   LDA   (DPTR),Y
   STA   $22A8,X
   STA   $2150,X
   STA   $23D0,X
   INY
   LDA   (DPTR),Y
   STA   $26A8,X
   STA   $2550,X
   STA   $27D0,X
   INY
   LDA   (DPTR),Y
   STA   $2AA8,X
   STA   $2950,X
   STA   $2BD0,X
   INY
.if_end@0:
   DEX
.loop@0_continue:
   TXA
   CMP   $83
   BNE   .interlace_blit_p1@asm_ssa_split@0
.interlace_blit_p1@asm_ssa_block@0:
   RTS
.interlace_blit_p1@asm_ssa_split@0:
   JMP   .loop@0_start
