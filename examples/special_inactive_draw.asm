__zpabi_special_inactive_draw__special_row	EQU	$80
__zpabi_special_inactive_draw__special_pos_hi	EQU	$81
__zpabi_special_inactive_draw__page_flag	EQU	$82
__zpabi_draw_sprite__width	EQU	$83
__zpabi_draw_sprite__height	EQU	$84
__zpabi_draw_sprite__sprite_x	EQU	$85
__zpabi_draw_sprite__sprite_y	EQU	$86
__zpabi_draw_sprite__tile_src_0	EQU	$87
__zpabi_draw_sprite__tile_src_1	EQU	$88
__zpabi_draw_sprite__page_flag	EQU	$89

; @zp-link-meta-begin
; def special_inactive_draw params=__zpabi_special_inactive_draw__special_row,__zpabi_special_inactive_draw__special_pos_hi,__zpabi_special_inactive_draw__page_flag locals=__local_special_inactive_draw__0 indirect=false in_cycle=false
; ext draw_sprite params=__zpabi_draw_sprite__width,__zpabi_draw_sprite__height,__zpabi_draw_sprite__sprite_x,__zpabi_draw_sprite__sprite_y,__zpabi_draw_sprite__tile_src_0,__zpabi_draw_sprite__tile_src_1,__zpabi_draw_sprite__page_flag
; call special_inactive_draw -> draw_sprite
; @zp-link-meta-end

special_inactive_draw:
   SUBROUTINE

   LDX   __zpabi_special_inactive_draw__special_pos_hi
   LDA   #$02
   STA   __zpabi_draw_sprite__width
   LDA   #$06
   STA   __zpabi_draw_sprite__height
   LDA   proj_screen_col,X
   STA   __zpabi_draw_sprite__sprite_x
   LDA   __zpabi_special_inactive_draw__special_row
   STA   __zpabi_draw_sprite__sprite_y
   LDA   #<special_peek_sprite
   STA   __zpabi_draw_sprite__tile_src_0
   LDA   #>special_peek_sprite
   STA   __zpabi_draw_sprite__tile_src_1
   LDA   __zpabi_special_inactive_draw__page_flag
   STA   __zpabi_draw_sprite__page_flag
   JMP   draw_sprite

proj_screen_col:
   DC.B  $00, $00, $00, $00, $01, $01, $01, $02, $02, $02, $02, $03, $03, $03, $04, $04
   DC.B  $04, $04, $05, $05, $05, $06, $06, $06, $06, $07, $07, $07, $08, $08, $08, $08
   DC.B  $09, $09, $09, $0A, $0A, $0A, $0A, $0B, $0B, $0B, $0C, $0C, $0C, $0C, $0D, $0D
   DC.B  $0D, $0E, $0E, $0E, $0E, $0F, $0F, $0F, $10, $10, $10, $10, $11, $11, $11, $12
   DC.B  $12, $12, $12, $13, $13, $13, $14, $14, $14, $14, $15, $15, $15, $16, $16, $16
   DC.B  $16, $17, $17, $17, $18, $18, $18, $18, $19, $19, $19, $1A, $1A, $1A, $1A, $1B
   DC.B  $1B, $1B, $1C, $1C, $1C, $1C, $1D, $1D, $1D, $1E, $1E, $1E, $1E, $1F, $1F, $1F
   DC.B  $20, $20, $20, $20, $21, $21, $21, $22, $22, $22, $22, $23, $23, $23, $24, $24
   DC.B  $24, $24, $25, $25
