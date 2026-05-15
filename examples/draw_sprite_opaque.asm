__zpabi_draw_sprite_opaque_p0	EQU	$80
__zpabi_draw_sprite_opaque_p1	EQU	$81
__zpabi_draw_sprite_opaque_p2	EQU	$82
__zpabi_draw_sprite_opaque_p3	EQU	$83
__zpabi_draw_sprite_opaque_p4	EQU	$84
__zpabi_draw_sprite_opaque_p5	EQU	$85
__zpabi_draw_sprite_opaque_p6	EQU	$86
__local_draw_sprite_opaque_b0	EQU	$87
__local_draw_sprite_opaque_b1	EQU	$88
__local_draw_sprite_opaque_b2	EQU	$89
__local_draw_sprite_opaque_b3	EQU	$8A
__local_draw_sprite_opaque_b4	EQU	$8B
__local_draw_sprite_opaque_b5	EQU	$8C
__local_draw_sprite_opaque_b6	EQU	$8D
__local_draw_sprite_opaque_b7	EQU	$8E
__local_draw_sprite_opaque_b8	EQU	$8F

; @zp-link-meta-begin
; def draw_sprite_opaque param_bytes=7 local_bytes=9 indirect=false in_cycle=false
; @zp-link-meta-end

draw_sprite_opaque:
   SUBROUTINE

.draw_sprite_opaque@asm_ssa_block@0:
   LDA   __zpabi_draw_sprite_opaque_p6
   BPL   .cond_else@0
.draw_sprite_opaque@ssa_block@1:
   LDA   #<screen_row_addr_hi2
   STA   __local_draw_sprite_opaque_b7
   LDA   #>screen_row_addr_hi2
   STA   __local_draw_sprite_opaque_b7+1
   JMP   .cond_end@1
.cond_else@0:
   LDA   #<screen_row_addr_hi
   STA   __local_draw_sprite_opaque_b7
   LDA   #>screen_row_addr_hi
   STA   __local_draw_sprite_opaque_b7+1
.cond_end@1:
   LDA   #$00
   STA   __local_draw_sprite_opaque_b6
   LDA   __zpabi_draw_sprite_opaque_p1
   STA   __local_draw_sprite_opaque_b5
   LDX   __zpabi_draw_sprite_opaque_p3
.loop@0_start:
   LDA   __local_draw_sprite_opaque_b5
   BEQ   .loop@0_break
.draw_sprite_opaque@ssa_block@2:
   TXA
   TAY
   LDA   ($8E),Y
   STA   __local_draw_sprite_opaque_b1
   LDA   screen_row_addr_lo,X
   STA   __local_draw_sprite_opaque_b0
   STA   __local_draw_sprite_opaque_b3
   LDA   __local_draw_sprite_opaque_b1
   STA   __local_draw_sprite_opaque_b4
   LDA   __zpabi_draw_sprite_opaque_p0
   STA   __local_draw_sprite_opaque_b2
   LDA   __zpabi_draw_sprite_opaque_p2
   STA   __local_draw_sprite_opaque_b1
.loop@1_start:
   LDA   __local_draw_sprite_opaque_b2
   BMI   .loop@1_break
.draw_sprite_opaque@asm_ssa_block@1:
   LDA   __local_draw_sprite_opaque_b1
   CMP   #$28
   BCS   .if_end@2
.draw_sprite_opaque@asm_ssa_block@2:
   LDY   __local_draw_sprite_opaque_b6
   LDA   ($84),Y
   STA   __local_draw_sprite_opaque_b0
   PHA
   LDY   __local_draw_sprite_opaque_b1
   PLA
   STA   ($8A),Y
.if_end@2:
.loop@1_continue:
   DEC   __local_draw_sprite_opaque_b2
   DEC   __local_draw_sprite_opaque_b1
   INC   __local_draw_sprite_opaque_b6
   JMP   .loop@1_start
.loop@1_break:
.loop@0_continue:
   DEC   __local_draw_sprite_opaque_b5
   INX
   JMP   .loop@0_start
.loop@0_break:
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
