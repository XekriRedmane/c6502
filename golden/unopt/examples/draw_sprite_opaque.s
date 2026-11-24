SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24

draw_sprite_opaque:
   SUBROUTINE

   ; prologue: 7 arg bytes, 71 local bytes
   SEC
   LDA   SSP
   SBC   #$49
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$48
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   LDY   #$50
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   AND   #$80
   LDY   #$03
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDA   #$00
   LDY   #$04
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .cond_else@0
   LDA   #<screen_row_addr_hi2
   INY
   STA   (FP),Y
   LDA   #>screen_row_addr_hi2
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$07
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$08
   STA   (FP),Y
   JMP   .cond_end@1
.cond_else@0:
   LDA   #<screen_row_addr_hi
   LDY   #$09
   STA   (FP),Y
   LDA   #>screen_row_addr_hi
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$07
   STA   (FP),Y
   LDY   #$0A
   LDA   (FP),Y
   LDY   #$08
   STA   (FP),Y
.cond_end@1:
   LDY   #$07
   LDA   (FP),Y
   LDY   #$0B
   STA   (FP),Y
   LDY   #$08
   LDA   (FP),Y
   LDY   #$0C
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$4B
   LDA   (FP),Y
   LDY   #$0F
   STA   (FP),Y
.loop@0_start:
   LDY   #$0F
   LDA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@0
   LDA   #$00
   JMP   .cmp_end@1
.cmp_true@0:
   LDA   #$01
.cmp_end@1:
   LDY   #$12
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@1
   JMP   .loop@0_break
.lb_skip@1:
   LDY   #$4D
   LDA   (FP),Y
   LDY   #$14
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$0B
   LDA   (FP),Y
   CLC
   LDY   #$14
   ADC   (FP),Y
   LDY   #$16
   STA   (FP),Y
   LDY   #$0C
   LDA   (FP),Y
   LDY   #$15
   ADC   (FP),Y
   LDY   #$17
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$18
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$1C
   STA   (FP),Y
   LDA   #$00
   DEY
   STA   (FP),Y
   LDY   #$4D
   LDA   (FP),Y
   LDY   #$1D
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$1F
   STA   (FP),Y
   TAX
   LDA   screen_row_addr_lo,X
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$23
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$24
   STA   (FP),Y
   LDY   #$1B
   LDA   (FP),Y
   LDY   #$23
   ORA   (FP),Y
   LDY   #$25
   STA   (FP),Y
   LDY   #$1C
   LDA   (FP),Y
   LDY   #$24
   ORA   (FP),Y
   LDY   #$26
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$27
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$28
   STA   (FP),Y
   LDY   #$4A
   LDA   (FP),Y
   LDY   #$29
   STA   (FP),Y
   LDY   #$4C
   LDA   (FP),Y
   LDY   #$2A
   STA   (FP),Y
.loop@1_start:
   LDY   #$29
   LDA   (FP),Y
   LDY   #$2B
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   AND   #$80
   LDY   #$2D
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDA   #$00
   LDY   #$2E
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   BEQ   .cmp_true@2
   LDA   #$00
   JMP   .cmp_end@3
.cmp_true@2:
   LDA   #$01
.cmp_end@3:
   LDY   #$2F
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@0
   JMP   .loop@1_break
.lb_skip@0:
   LDY   #$2A
   LDA   (FP),Y
   LDY   #$31
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
   BVC   .cmp_novf@4
   EOR   #$80
.cmp_novf@4:
   ASL
   LDA   #$00
   ROL
   LDY   #$33
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@2
   LDY   #$0E
   LDA   (FP),Y
   LDY   #$35
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$4E
   LDA   (FP),Y
   CLC
   LDY   #$35
   ADC   (FP),Y
   LDY   #$37
   STA   (FP),Y
   LDY   #$4F
   LDA   (FP),Y
   LDY   #$36
   ADC   (FP),Y
   LDY   #$38
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$39
   STA   (FP),Y
   LDY   #$2A
   LDA   (FP),Y
   LDY   #$3A
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$27
   LDA   (FP),Y
   CLC
   LDY   #$3A
   ADC   (FP),Y
   LDY   #$3C
   STA   (FP),Y
   LDY   #$28
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
   LDY   #$39
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
.if_end@2:
   LDY   #$29
   LDA   (FP),Y
   LDY   #$3E
   STA   (FP),Y
   SEC
   SBC   #$01
   INY
   STA   (FP),Y
   LDY   #$29
   STA   (FP),Y
   INY
   LDA   (FP),Y
   LDY   #$40
   STA   (FP),Y
   SEC
   SBC   #$01
   INY
   STA   (FP),Y
   LDY   #$2A
   STA   (FP),Y
   LDY   #$0E
   LDA   (FP),Y
   LDY   #$42
   STA   (FP),Y
   CLC
   ADC   #$01
   INY
   STA   (FP),Y
   LDY   #$0E
   STA   (FP),Y
   JMP   .loop@1_start
.loop@1_break:
   LDY   #$0F
   LDA   (FP),Y
   LDY   #$44
   STA   (FP),Y
   SEC
   SBC   #$01
   INY
   STA   (FP),Y
   LDY   #$0F
   STA   (FP),Y
   LDY   #$4D
   LDA   (FP),Y
   LDY   #$46
   STA   (FP),Y
   CLC
   ADC   #$01
   INY
   STA   (FP),Y
   LDY   #$4D
   STA   (FP),Y
   JMP   .loop@0_start
.loop@0_break:

   ; epilogue
   CLC
   LDA   FP
   ADC   #$50
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$48
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   RTS

screen_row_addr_hi:
   DC.B  $20, $24, $28, $2C, $30, $34, $38, $3C, $20, $24, $28, $2C, $30, $34, $38, $3C
   DC.B  $21, $25, $29, $2D, $31, $35, $39, $3D, $21, $25, $29, $2D, $31, $35, $39, $3D
   DC.B  $22, $26, $2A, $2E, $32, $36, $3A, $3E, $22, $26, $2A, $2E, $32, $36, $3A, $3E
   DC.B  $23, $27, $2B, $2F, $33, $37, $3B, $3F, $23, $27, $2B, $2F, $33, $37, $3B, $3F
   DC.B  $20, $24, $28, $2C, $30, $34, $38, $3C, $20, $24, $28, $2C, $30, $34, $38, $3C
   DC.B  $21, $25, $29, $2D, $31, $35, $39, $3D, $21, $25, $29, $2D, $31, $35, $39, $3D
   DC.B  $22, $26, $2A, $2E, $32, $36, $3A, $3E, $22, $26, $2A, $2E, $32, $36, $3A, $3E
   DC.B  $23, $27, $2B, $2F, $33, $37, $3B, $3F, $23, $27, $2B, $2F, $33, $37, $3B, $3F
   DC.B  $20, $24, $28, $2C, $30, $34, $38, $3C, $20, $24, $28, $2C, $30, $34, $38, $3C
   DC.B  $21, $25, $29, $2D, $31, $35, $39, $3D, $21, $25, $29, $2D, $31, $35, $39, $3D
   DC.B  $22, $26, $2A, $2E, $32, $36, $3A, $3E, $22, $26, $2A, $2E, $32, $36, $3A, $3E
   DC.B  $23, $27, $2B, $2F, $33, $37, $3B, $3F, $23, $27, $2B, $2F, $33, $37, $3B, $3F

screen_row_addr_lo:
   DC.B  $00, $00, $00, $00, $00, $00, $00, $00, $80, $80, $80, $80, $80, $80, $80, $80
   DC.B  $00, $00, $00, $00, $00, $00, $00, $00, $80, $80, $80, $80, $80, $80, $80, $80
   DC.B  $00, $00, $00, $00, $00, $00, $00, $00, $80, $80, $80, $80, $80, $80, $80, $80
   DC.B  $00, $00, $00, $00, $00, $00, $00, $00, $80, $80, $80, $80, $80, $80, $80, $80
   DC.B  $28, $28, $28, $28, $28, $28, $28, $28, $A8, $A8, $A8, $A8, $A8, $A8, $A8, $A8
   DC.B  $28, $28, $28, $28, $28, $28, $28, $28, $A8, $A8, $A8, $A8, $A8, $A8, $A8, $A8
   DC.B  $28, $28, $28, $28, $28, $28, $28, $28, $A8, $A8, $A8, $A8, $A8, $A8, $A8, $A8
   DC.B  $28, $28, $28, $28, $28, $28, $28, $28, $A8, $A8, $A8, $A8, $A8, $A8, $A8, $A8
   DC.B  $50, $50, $50, $50, $50, $50, $50, $50, $D0, $D0, $D0, $D0, $D0, $D0, $D0, $D0
   DC.B  $50, $50, $50, $50, $50, $50, $50, $50, $D0, $D0, $D0, $D0, $D0, $D0, $D0, $D0
   DC.B  $50, $50, $50, $50, $50, $50, $50, $50, $D0, $D0, $D0, $D0, $D0, $D0, $D0, $D0
   DC.B  $50, $50, $50, $50, $50, $50, $50, $50, $D0, $D0, $D0, $D0, $D0, $D0, $D0, $D0

screen_row_addr_hi2:
   DC.B  $40, $44, $48, $4C, $50, $54, $58, $5C, $40, $44, $48, $4C, $50, $54, $58, $5C
   DC.B  $41, $45, $49, $4D, $51, $55, $59, $5D, $41, $45, $49, $4D, $51, $55, $59, $5D
   DC.B  $42, $46, $4A, $4E, $52, $56, $5A, $5E, $42, $46, $4A, $4E, $52, $56, $5A, $5E
   DC.B  $43, $47, $4B, $4F, $53, $57, $5B, $5F, $43, $47, $4B, $4F, $53, $57, $5B, $5F
   DC.B  $40, $44, $48, $4C, $50, $54, $58, $5C, $40, $44, $48, $4C, $50, $54, $58, $5C
   DC.B  $41, $45, $49, $4D, $51, $55, $59, $5D, $41, $45, $49, $4D, $51, $55, $59, $5D
   DC.B  $42, $46, $4A, $4E, $52, $56, $5A, $5E, $42, $46, $4A, $4E, $52, $56, $5A, $5E
   DC.B  $43, $47, $4B, $4F, $53, $57, $5B, $5F, $43, $47, $4B, $4F, $53, $57, $5B, $5F
   DC.B  $40, $44, $48, $4C, $50, $54, $58, $5C, $40, $44, $48, $4C, $50, $54, $58, $5C
   DC.B  $41, $45, $49, $4D, $51, $55, $59, $5D, $41, $45, $49, $4D, $51, $55, $59, $5D
   DC.B  $42, $46, $4A, $4E, $52, $56, $5A, $5E, $42, $46, $4A, $4E, $52, $56, $5A, $5E
   DC.B  $43, $47, $4B, $4F, $53, $57, $5B, $5F, $43, $47, $4B, $4F, $53, $57, $5B, $5F
