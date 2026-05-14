__zpabi_refresh_hit_entities_p0	EQU	$80
__zpabi_refresh_hit_entities_p1	EQU	$81
__zpabi_refresh_hit_entities_p2	EQU	$82
__zpabi_draw_sprite_opaque_p0	EQU	$83
__zpabi_draw_sprite_opaque_p1	EQU	$84
__zpabi_draw_sprite_opaque_p2	EQU	$85
__zpabi_draw_sprite_opaque_p3	EQU	$86
__zpabi_draw_sprite_opaque_p4	EQU	$87
__zpabi_draw_sprite_opaque_p5	EQU	$88
__local_refresh_hit_entities_b0	EQU	$89
__local_refresh_hit_entities_b1	EQU	$8A
__local_refresh_hit_entities_b2	EQU	$8B
__local_refresh_hit_entities_b3	EQU	$8C
__local_refresh_hit_entities_b4	EQU	$8D

; @zp-link-meta-begin
; def refresh_hit_entities param_bytes=3 local_bytes=5 indirect=false in_cycle=false
; ext draw_sprite_opaque param_bytes=6
; call refresh_hit_entities -> draw_sprite_opaque
; @zp-link-meta-end

refresh_hit_entities:
   SUBROUTINE

.refresh_hit_entities@asm_ssa_preheader@0:
.refresh_hit_entities@ssa_block@0:
   LDA   __zpabi_refresh_hit_entities_p0
   STA   __local_refresh_hit_entities_b4
.loop@0_start:
   LDX   __local_refresh_hit_entities_b4
   LDA   entity_hit_y,X
   SEC
   SBC   __zpabi_refresh_hit_entities_p1
   BCC   .if_end@0
.refresh_hit_entities@asm_ssa_block@0:
   STA   __local_refresh_hit_entities_b3
   CMP   #$2F
   BCS   .if_end@1
.refresh_hit_entities@asm_ssa_block@1:
   LDA   entity_hit_state,X
   BPL   .if_else@3
.refresh_hit_entities@ssa_block@3:
   LDX   __zpabi_refresh_hit_entities_p2
   LDA   hit_spr_neg_hi,X
   STA   __local_refresh_hit_entities_b1
   LDA   hit_spr_neg_lo,X
   STA   __local_refresh_hit_entities_b0
   JMP   .if_end@2
.if_else@3:
   LDX   __zpabi_refresh_hit_entities_p2
   LDA   hit_spr_pos_hi,X
   STA   __local_refresh_hit_entities_b1
   LDA   hit_spr_pos_lo,X
   STA   __local_refresh_hit_entities_b0
.if_end@2:
   LDA   #$00
   ORA   __local_refresh_hit_entities_b0
   STA   __local_refresh_hit_entities_b2
   LDX   __local_refresh_hit_entities_b4
   LDA   entity_hit_row,X
   STA   __local_refresh_hit_entities_b0
   LDA   #$07
   STA   __zpabi_draw_sprite_opaque_p0
   LDA   #$05
   STA   __zpabi_draw_sprite_opaque_p1
   LDA   __local_refresh_hit_entities_b3
   STA   __zpabi_draw_sprite_opaque_p2
   LDA   __local_refresh_hit_entities_b0
   STA   __zpabi_draw_sprite_opaque_p3
   LDA   __local_refresh_hit_entities_b2
   STA   __zpabi_draw_sprite_opaque_p4
   LDA   __local_refresh_hit_entities_b1
   STA   __zpabi_draw_sprite_opaque_p5
   JSR   draw_sprite_opaque
.if_end@1:
.if_end@0:
   DEC   __local_refresh_hit_entities_b4
.loop@0_continue:
   BPL   .refresh_hit_entities@asm_ssa_split@0
.refresh_hit_entities@asm_ssa_block@2:
   RTS
.refresh_hit_entities@asm_ssa_split@0:
   JMP   .loop@0_start

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
