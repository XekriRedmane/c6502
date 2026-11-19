SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__zpabi_refresh_hit_entities__sprite_xref	EQU	$80
__zpabi_draw_sprite_opaque__sprite_x	EQU	$81
__zpabi_draw_sprite_opaque__sprite_y	EQU	$82
__zpabi_draw_sprite_opaque__tile_src_0	EQU	$83
__zpabi_draw_sprite_opaque__tile_src_1	EQU	$84
__local_refresh_hit_entities__0	EQU	$87
__local_refresh_hit_entities__x	EQU	$88
__local_refresh_hit_entities__player_y	EQU	$89

; @zp-link-meta-begin
; def refresh_hit_entities params=__zpabi_refresh_hit_entities__hit_max,__zpabi_refresh_hit_entities__player_y,__zpabi_refresh_hit_entities__sprite_xref locals=__local_refresh_hit_entities__lo,__local_refresh_hit_entities__hi,__local_refresh_hit_entities__0,__local_refresh_hit_entities__x,__local_refresh_hit_entities__player_y indirect=false in_cycle=false param_regs=A,X,
; ext draw_sprite_opaque params=__zpabi_draw_sprite_opaque__width,__zpabi_draw_sprite_opaque__height,__zpabi_draw_sprite_opaque__sprite_x,__zpabi_draw_sprite_opaque__sprite_y,__zpabi_draw_sprite_opaque__tile_src_0,__zpabi_draw_sprite_opaque__tile_src_1 param_regs=A,X,,,,
; call refresh_hit_entities -> draw_sprite_opaque
; item refresh_hit_entities section=.text local=false zero=false
; item entity_hit_y section=.bss local=false zero=true
; item entity_hit_row section=.bss local=false zero=true
; item entity_hit_state section=.bss local=false zero=true
; item hit_spr_pos_lo section=.rodata local=false zero=false
; item hit_spr_pos_hi section=.rodata local=false zero=false
; item hit_spr_neg_lo section=.rodata local=false zero=false
; item hit_spr_neg_hi section=.rodata local=false zero=false
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

refresh_hit_entities:
   SUBROUTINE

   STA   __local_refresh_hit_entities__x
   STX   __local_refresh_hit_entities__player_y
.loop@0_start:
   LDX   __local_refresh_hit_entities__x
   LDA   entity_hit_y,X
   SEC
   SBC   __local_refresh_hit_entities__player_y
   BCC   .if_end@0
   STA   __local_refresh_hit_entities__0
   CMP   #$2F
   BCS   .if_end@1
   LDA   entity_hit_state,X
   BPL   .if_else@3
   LDX   __zpabi_refresh_hit_entities__sprite_xref
   LDA   hit_spr_neg_hi,X
   STA   __zpabi_draw_sprite_opaque__tile_src_1
   LDA   hit_spr_neg_lo,X
   JMP   .if_end@2
.if_else@3:
   LDX   __zpabi_refresh_hit_entities__sprite_xref
   LDA   hit_spr_pos_hi,X
   STA   __zpabi_draw_sprite_opaque__tile_src_1
   LDA   hit_spr_pos_lo,X
.if_end@2:
   STA   __zpabi_draw_sprite_opaque__tile_src_0
   LDX   __local_refresh_hit_entities__x
   LDA   entity_hit_row,X
   STA   __zpabi_draw_sprite_opaque__sprite_y
   LDA   __local_refresh_hit_entities__0
   STA   __zpabi_draw_sprite_opaque__sprite_x
   LDX   #$05
   LDA   #$07
   JSR   draw_sprite_opaque
.if_end@1:
.if_end@0:
   DEC   __local_refresh_hit_entities__x
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
