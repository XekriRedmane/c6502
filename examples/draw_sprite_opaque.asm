__zpabi_draw_sprite_opaque__width	EQU	$80
__zpabi_draw_sprite_opaque__height	EQU	$81
__zpabi_draw_sprite_opaque__sprite_x	EQU	$82
__zpabi_draw_sprite_opaque__sprite_y	EQU	$83
__zpabi_draw_sprite_opaque__tile_src_0	EQU	$84
__zpabi_draw_sprite_opaque__tile_src_1	EQU	$85
__zpabi_draw_sprite_opaque__page_flag	EQU	$86
__local_draw_sprite_opaque__x	EQU	$88
__local_draw_sprite_opaque__row_remain	EQU	$89
__local_draw_sprite_opaque__1	EQU	$8A
__local_draw_sprite_opaque__2	EQU	$8B
__local_draw_sprite_opaque__h	EQU	$8C
__local_draw_sprite_opaque__y	EQU	$8D
__local_draw_sprite_opaque__3	EQU	$8E
__local_draw_sprite_opaque__4	EQU	$8F

; @zp-link-meta-begin
; def draw_sprite_opaque params=__zpabi_draw_sprite_opaque__width,__zpabi_draw_sprite_opaque__height,__zpabi_draw_sprite_opaque__sprite_x,__zpabi_draw_sprite_opaque__sprite_y,__zpabi_draw_sprite_opaque__tile_src_0,__zpabi_draw_sprite_opaque__tile_src_1,__zpabi_draw_sprite_opaque__page_flag locals=__local_draw_sprite_opaque__0,__local_draw_sprite_opaque__x,__local_draw_sprite_opaque__row_remain,__local_draw_sprite_opaque__1,__local_draw_sprite_opaque__2,__local_draw_sprite_opaque__h,__local_draw_sprite_opaque__y,__local_draw_sprite_opaque__3,__local_draw_sprite_opaque__4 indirect=false in_cycle=false
; @zp-link-meta-end

draw_sprite_opaque:
   SUBROUTINE

   LDA   __zpabi_draw_sprite_opaque__page_flag
   BPL   .cond_else@0
   LDA   #<screen_row_addr_hi2
   STA   __local_draw_sprite_opaque__3
   LDA   #>screen_row_addr_hi2
   STA   __local_draw_sprite_opaque__4
   JMP   .cond_end@1
.cond_else@0:
   LDA   #<screen_row_addr_hi
   STA   __local_draw_sprite_opaque__3
   LDA   #>screen_row_addr_hi
   STA   __local_draw_sprite_opaque__4
.cond_end@1:
   LDA   #$00
   STA   __local_draw_sprite_opaque__y
   LDA   __zpabi_draw_sprite_opaque__height
   STA   __local_draw_sprite_opaque__h
   LDX   __zpabi_draw_sprite_opaque__sprite_y
.loop@0_start:
   LDA   __local_draw_sprite_opaque__h
   BEQ   .loop@0_break
   TXA
   TAY
   LDA   (__local_draw_sprite_opaque__3),Y
   STA   __local_draw_sprite_opaque__x
   LDA   screen_row_addr_lo,X
   STA   __local_draw_sprite_opaque__1
   LDA   __local_draw_sprite_opaque__x
   STA   __local_draw_sprite_opaque__2
   LDA   __zpabi_draw_sprite_opaque__width
   STA   __local_draw_sprite_opaque__row_remain
   LDA   __zpabi_draw_sprite_opaque__sprite_x
   STA   __local_draw_sprite_opaque__x
.loop@1_start:
   LDA   __local_draw_sprite_opaque__row_remain
   BMI   .loop@1_break
   LDA   __local_draw_sprite_opaque__x
   CMP   #$28
   BCS   .if_end@2
   LDY   __local_draw_sprite_opaque__y
   LDA   (__zpabi_draw_sprite_opaque__tile_src_0),Y
   LDY   __local_draw_sprite_opaque__x
   STA   (__local_draw_sprite_opaque__1),Y
.if_end@2:
   DEC   __local_draw_sprite_opaque__row_remain
   DEC   __local_draw_sprite_opaque__x
   INC   __local_draw_sprite_opaque__y
   JMP   .loop@1_start
.loop@1_break:
   DEC   __local_draw_sprite_opaque__h
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
