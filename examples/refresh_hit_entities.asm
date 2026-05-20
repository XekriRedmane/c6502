__zpabi_refresh_hit_entities__hit_max	EQU	$80
__zpabi_refresh_hit_entities__player_y	EQU	$81
__zpabi_refresh_hit_entities__sprite_xref	EQU	$82
__zpabi_draw_sprite_opaque__width	EQU	$83
__zpabi_draw_sprite_opaque__height	EQU	$84
__zpabi_draw_sprite_opaque__sprite_x	EQU	$85
__zpabi_draw_sprite_opaque__sprite_y	EQU	$86
__zpabi_draw_sprite_opaque__tile_src_0	EQU	$87
__zpabi_draw_sprite_opaque__tile_src_1	EQU	$88
__local_refresh_hit_entities__hi	EQU	$89
__local_refresh_hit_entities__0	EQU	$8A
__local_refresh_hit_entities__lo	EQU	$8B
__local_refresh_hit_entities__1	EQU	$8C
__local_refresh_hit_entities__x	EQU	$8D

; @zp-link-meta-begin
; def refresh_hit_entities params=__zpabi_refresh_hit_entities__hit_max,__zpabi_refresh_hit_entities__player_y,__zpabi_refresh_hit_entities__sprite_xref locals=__local_refresh_hit_entities__hi,__local_refresh_hit_entities__0,__local_refresh_hit_entities__lo,__local_refresh_hit_entities__1,__local_refresh_hit_entities__x indirect=false in_cycle=false
; ext draw_sprite_opaque params=__zpabi_draw_sprite_opaque__width,__zpabi_draw_sprite_opaque__height,__zpabi_draw_sprite_opaque__sprite_x,__zpabi_draw_sprite_opaque__sprite_y,__zpabi_draw_sprite_opaque__tile_src_0,__zpabi_draw_sprite_opaque__tile_src_1
; call refresh_hit_entities -> draw_sprite_opaque
; @zp-link-meta-end

refresh_hit_entities:
   SUBROUTINE

   LDX   __zpabi_refresh_hit_entities__hit_max
.loop@0_start:
   LDA   entity_hit_y,X
   CMP   __zpabi_refresh_hit_entities__player_y
   BCC   .if_end@0
   SEC
   SBC   __zpabi_refresh_hit_entities__player_y
   STA   __local_refresh_hit_entities__1
   CMP   #$2F
   BCS   .if_end@1
   LDA   entity_hit_state,X
   BPL   .if_else@3
   LDY   __zpabi_refresh_hit_entities__sprite_xref
   LDA   hit_spr_neg_hi,Y
   STA   __local_refresh_hit_entities__hi
   LDA   hit_spr_neg_lo,Y
   JMP   .if_end@2
.if_else@3:
   LDY   __zpabi_refresh_hit_entities__sprite_xref
   LDA   hit_spr_pos_hi,Y
   STA   __local_refresh_hit_entities__hi
   LDA   hit_spr_pos_lo,Y
.if_end@2:
   STA   __local_refresh_hit_entities__lo
   LDA   __local_refresh_hit_entities__hi
   STA   __local_refresh_hit_entities__0
   LDA   #$07
   STA   __zpabi_draw_sprite_opaque__width
   LDA   #$05
   STA   __zpabi_draw_sprite_opaque__height
   LDA   __local_refresh_hit_entities__1
   STA   __zpabi_draw_sprite_opaque__sprite_x
   LDA   entity_hit_row,X
   STA   __zpabi_draw_sprite_opaque__sprite_y
   LDA   __local_refresh_hit_entities__lo
   STA   __zpabi_draw_sprite_opaque__tile_src_0
   LDA   __local_refresh_hit_entities__0
   STA   __zpabi_draw_sprite_opaque__tile_src_1
   STX   __local_refresh_hit_entities__x
   JSR   draw_sprite_opaque
   LDX   __local_refresh_hit_entities__x
.if_end@1:
.if_end@0:
   DEX
   BPL   .loop@0_start
   RTS

entity_hit_y:
   DS.B  12

entity_hit_row:
   DS.B  12

entity_hit_state:
   DS.B  12

hit_spr_pos_lo:
   DC.B  $C0, $E8, $10, $38, $60, $88, $B0

hit_spr_pos_hi:
   DC.B  $8A, $8A, $8B, $8B, $8B, $8B, $8B

hit_spr_neg_lo:
   DC.B  $D4, $FC, $24, $4C, $74, $9C, $C4

hit_spr_neg_hi:
   DC.B  $7A, $7A, $7B, $7B, $7B, $7B, $7B
