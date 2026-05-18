__zpabi_companion_update__gate	EQU	$80
__zpabi_companion_update__player_y	EQU	$81
__zpabi_companion_update__sprite_xref	EQU	$82
__zpabi_companion_update__player_col	EQU	$83
__zpabi_companion_update__player_floor	EQU	$84
__zpabi_companion_update__hit_max	EQU	$85
__zpabi_companion_update__page_flag	EQU	$86
__zpabi_active_neg_step__slot	EQU	$87
__zpabi_active_pos_step__slot	EQU	$87
__zpabi_compute_screen_x__slot	EQU	$87
__zpabi_drift_step__slot	EQU	$87
__zpabi_entity_proximity__slot	EQU	$87
__zpabi_player_catch__slot	EQU	$87
__zpabi_smc_body_draw__slot	EQU	$87
__zpabi_active_neg_step__player_floor	EQU	$88
__zpabi_active_pos_step__player_floor	EQU	$88
__zpabi_compute_screen_x__player_y	EQU	$88
__zpabi_drift_step__out_sprite_y_0	EQU	$88
__zpabi_entity_proximity__screen_x	EQU	$88
__zpabi_player_catch__screen_x	EQU	$88
__zpabi_smc_body_draw__sprite_x	EQU	$88
__local_active_neg_step__0	EQU	$89
__local_active_pos_step__0	EQU	$89
__zpabi_compute_screen_x__sprite_xref	EQU	$89
__zpabi_drift_step__out_sprite_y_1	EQU	$89
__zpabi_entity_proximity__hit_max	EQU	$89
__zpabi_player_catch__player_col	EQU	$89
__zpabi_smc_body_draw__sprite_y	EQU	$89
__local_active_neg_step__1	EQU	$8A
__local_active_pos_step__1	EQU	$8A
__local_drift_step__pos_1	EQU	$8A
__local_player_catch__0	EQU	$8A
__zpabi_find_active_entity__hit_max	EQU	$8A
__zpabi_smc_body_draw__frame_idx	EQU	$8A
__local_drift_step__pos_0	EQU	$8B
__local_player_catch__1	EQU	$8B
__zpabi_find_active_entity__out_row_0	EQU	$8B
__zpabi_smc_body_draw__state	EQU	$8B
__local_compute_screen_x__2	EQU	$8C
__local_drift_step__0	EQU	$8C
__zpabi_find_active_entity__out_row_1	EQU	$8C
__zpabi_smc_body_draw__page_flag	EQU	$8C
__local_compute_screen_x__3	EQU	$8D
__local_entity_proximity__0	EQU	$8D
__zpabi_draw_sprite__width	EQU	$8D
__local_entity_proximity__1	EQU	$8E
__zpabi_draw_sprite__height	EQU	$8E
__local_entity_proximity__entity_row	EQU	$8F
__zpabi_draw_sprite__sprite_x	EQU	$8F
__zpabi_draw_sprite__sprite_y	EQU	$90
__zpabi_draw_sprite__tile_src_0	EQU	$91
__zpabi_draw_sprite__tile_src_1	EQU	$92
__zpabi_draw_sprite__page_flag	EQU	$93
__local_companion_update__0	EQU	$94
__local_companion_update__1	EQU	$95
__local_companion_update__3	EQU	$97
__local_companion_update__4	EQU	$98
__local_companion_update__slot	EQU	$99
__local_companion_update__sprite_y	EQU	$9A
__local_smc_body_draw__hi	EQU	$9B
__local_smc_body_draw__lo	EQU	$9C
__local_smc_body_draw__0	EQU	$9D

; @zp-link-meta-begin
; def active_neg_step params=__zpabi_active_neg_step__slot,__zpabi_active_neg_step__player_floor locals=__local_active_neg_step__0,__local_active_neg_step__1 indirect=false in_cycle=false
; def active_pos_step params=__zpabi_active_pos_step__slot,__zpabi_active_pos_step__player_floor locals=__local_active_pos_step__0,__local_active_pos_step__1 indirect=false in_cycle=false
; def companion_update params=__zpabi_companion_update__gate,__zpabi_companion_update__player_y,__zpabi_companion_update__sprite_xref,__zpabi_companion_update__player_col,__zpabi_companion_update__player_floor,__zpabi_companion_update__hit_max,__zpabi_companion_update__page_flag locals=__local_companion_update__0,__local_companion_update__1,__local_companion_update__2,__local_companion_update__3,__local_companion_update__4,__local_companion_update__slot,__local_companion_update__sprite_y indirect=false in_cycle=false
; def compute_screen_x params=__zpabi_compute_screen_x__slot,__zpabi_compute_screen_x__player_y,__zpabi_compute_screen_x__sprite_xref locals=__local_compute_screen_x__0,__local_compute_screen_x__1,__local_compute_screen_x__2,__local_compute_screen_x__3 indirect=false in_cycle=false
; def drift_step params=__zpabi_drift_step__slot,__zpabi_drift_step__out_sprite_y_0,__zpabi_drift_step__out_sprite_y_1 locals=__local_drift_step__pos_1,__local_drift_step__pos_0,__local_drift_step__0 indirect=false in_cycle=false
; def entity_proximity params=__zpabi_entity_proximity__slot,__zpabi_entity_proximity__screen_x,__zpabi_entity_proximity__hit_max locals=__local_entity_proximity__0,__local_entity_proximity__1,__local_entity_proximity__entity_row indirect=false in_cycle=false
; def find_active_entity params=__zpabi_find_active_entity__hit_max,__zpabi_find_active_entity__out_row_0,__zpabi_find_active_entity__out_row_1 locals=__local_find_active_entity__0 indirect=false in_cycle=false
; def player_catch params=__zpabi_player_catch__slot,__zpabi_player_catch__screen_x,__zpabi_player_catch__player_col locals=__local_player_catch__0,__local_player_catch__1 indirect=false in_cycle=false
; def smc_body_draw params=__zpabi_smc_body_draw__slot,__zpabi_smc_body_draw__sprite_x,__zpabi_smc_body_draw__sprite_y,__zpabi_smc_body_draw__frame_idx,__zpabi_smc_body_draw__state,__zpabi_smc_body_draw__page_flag locals=__local_smc_body_draw__hi,__local_smc_body_draw__lo,__local_smc_body_draw__0 indirect=false in_cycle=false
; ext draw_sprite params=__zpabi_draw_sprite__width,__zpabi_draw_sprite__height,__zpabi_draw_sprite__sprite_x,__zpabi_draw_sprite__sprite_y,__zpabi_draw_sprite__tile_src_0,__zpabi_draw_sprite__tile_src_1,__zpabi_draw_sprite__page_flag
; ext prng params=
; call active_neg_step -> prng
; call active_pos_step -> prng
; call companion_update -> active_neg_step
; call companion_update -> active_pos_step
; call companion_update -> compute_screen_x
; call companion_update -> drift_step
; call companion_update -> entity_proximity
; call companion_update -> player_catch
; call companion_update -> smc_body_draw
; call entity_proximity -> find_active_entity
; call smc_body_draw -> draw_sprite
; @zp-link-meta-end

compute_screen_x:
   SUBROUTINE

.compute_screen_x@asm_ssa_block@0:
   LDX   __zpabi_compute_screen_x__player_y
   LDA   perspective_xoff_lo,X
   SEC
   SBC   __zpabi_compute_screen_x__sprite_xref
   STA   __local_compute_screen_x__3
   LDA   perspective_xoff_hi,X
   SBC   #$00
   STA   __local_compute_screen_x__2
   LDX   __zpabi_compute_screen_x__slot
   LDA   companion_pos_lo,X
   SEC
   SBC   __local_compute_screen_x__3
   STA   HARGS
   LDA   companion_pos_hi,X
   SBC   __local_compute_screen_x__2
   STA   HARGS+1
   RTS

find_active_entity:
   SUBROUTINE

.find_active_entity@asm_ssa_preheader@0:
.find_active_entity@ssa_block@0:
   LDX   __zpabi_find_active_entity__hit_max
.loop@0_start:
   TXA
   BMI   .loop@0_break
.find_active_entity@asm_ssa_block@0:
   TXA
   BMI   .sx_neg@0
.find_active_entity@asm_ssa_block@1:
   JMP   .sx_done@1
.sx_neg@0:
.sx_done@1:
   LDA   entity_hit_state,X
   BMI   .if_end@0
.find_active_entity@asm_ssa_block@2:
   TXA
   BMI   .sx_neg@2
.find_active_entity@asm_ssa_block@3:
   JMP   .sx_done@3
.sx_neg@2:
.sx_done@3:
   LDA   entity_hit_row,X
   SEC
   SBC   #$08
   LDY   #$00
   STA   (__local_player_catch__1),Y
   LDA   #$01
   RTS
.if_end@0:
.loop@0_continue:
   DEX
   JMP   .loop@0_start
.loop@0_break:
   LDA   #$00
   RTS

entity_proximity:
   SUBROUTINE

.entity_proximity@asm_ssa_block@0:
   LDA   #<__local_entity_proximity__entity_row
   STA   __local_entity_proximity__0
   LDA   #>__local_entity_proximity__entity_row
   STA   __local_entity_proximity__0+1
   LDA   __zpabi_entity_proximity__hit_max
   STA   __zpabi_find_active_entity__hit_max
   LDA   __local_entity_proximity__0
   STA   __zpabi_find_active_entity__out_row_0
   LDA   __local_entity_proximity__1
   STA   __zpabi_find_active_entity__out_row_1
   JSR   find_active_entity
   BEQ   .lnot_true@4
.entity_proximity@asm_ssa_block@1:
   LDA   #$00
   JMP   .lnot_end@5
.lnot_true@4:
   LDA   #$01
.lnot_end@5:
   ORA   #$00
   BEQ   .if_end@1
.entity_proximity@asm_ssa_block@2:
   RTS
.if_end@1:
   LDX   __zpabi_entity_proximity__slot
   LDA   companion_row,X
   STA   __local_entity_proximity__0
   LDA   __local_entity_proximity__entity_row
   CMP   __local_entity_proximity__0
   BEQ   .if_end@2
.entity_proximity@asm_ssa_block@3:
   RTS
.if_end@2:
   LDA   __zpabi_entity_proximity__screen_x
   CMP   #$40
   BCC   .and_false@3
.entity_proximity@asm_ssa_block@4:
   CMP   #$47
   BCS   .and_false@3
.entity_proximity@ssa_block@4:
   LDA   #$01
   STA   __local_entity_proximity__1
   JMP   .and_end@4
.and_false@3:
   LDA   #$00
   STA   __local_entity_proximity__1
.and_end@4:
   LDA   __local_entity_proximity__1
   BEQ   .if_end@5
.entity_proximity@asm_ssa_block@5:
   LDX   __zpabi_entity_proximity__slot
   LDA   #$FF
   STA   companion_state,X
   LDA   companion_row,X
   CLC
   ADC   #$04
   STA   companion_row,X
   RTS
.if_end@5:
   LDX   __zpabi_entity_proximity__slot
   LDA   companion_dir,X
   BMI   .if_else@7
.entity_proximity@asm_ssa_block@6:
   LDA   __zpabi_entity_proximity__screen_x
   CMP   #$30
   BCC   .and_false@8
.entity_proximity@asm_ssa_block@7:
   CMP   #$38
   BCS   .and_false@8
.entity_proximity@ssa_block@8:
   LDA   #$01
   STA   __local_entity_proximity__1
   JMP   .and_end@9
.and_false@8:
   LDA   #$00
   STA   __local_entity_proximity__1
.and_end@9:
   LDA   __local_entity_proximity__1
   BEQ   .if_end@10
.entity_proximity@asm_ssa_block@8:
   LDX   __zpabi_entity_proximity__slot
   LDA   #$00
   STA   companion_state,X
.if_end@10:
   JMP   .if_end@6
.if_else@7:
   LDA   __zpabi_entity_proximity__screen_x
   CMP   #$50
   BCC   .and_false@11
.entity_proximity@asm_ssa_block@9:
   CMP   #$58
   BCS   .and_false@11
.entity_proximity@ssa_block@11:
   LDA   #$01
   STA   __local_entity_proximity__1
   JMP   .and_end@12
.and_false@11:
   LDA   #$00
   STA   __local_entity_proximity__1
.and_end@12:
   LDA   __local_entity_proximity__1
   BEQ   .if_end@13
.entity_proximity@asm_ssa_block@10:
   LDX   __zpabi_entity_proximity__slot
   LDA   #$00
   STA   companion_state,X
.if_end@13:
.if_end@6:
   RTS

smc_body_draw:
   SUBROUTINE

.smc_body_draw@asm_ssa_block@0:
   LDX   __zpabi_smc_body_draw__slot
   LDA   companion_dir,X
   AND   #$80
   STA   __local_smc_body_draw__lo
   LDA   #$00
   STA   __local_smc_body_draw__hi
   CMP   #$00
   BNE   .cmp_differ@8
.smc_body_draw@asm_ssa_block@1:
   LDA   __local_smc_body_draw__lo
   CMP   #$00
.cmp_differ@8:
   BNE   .cmp_true@6
.smc_body_draw@asm_ssa_block@2:
   LDA   #$00
   JMP   .cmp_end@7
.cmp_true@6:
   LDA   #$01
.cmp_end@7:
   STA   __local_smc_body_draw__0
   LDA   __zpabi_smc_body_draw__state
   BNE   .if_else@15
.smc_body_draw@ssa_block@1:
   LDA   #$00
   STA   __local_smc_body_draw__hi
   JMP   .if_end@14
.if_else@15:
   LDA   __local_smc_body_draw__0
   BEQ   .cond_else@16
.smc_body_draw@ssa_block@2:
   JMP   .cond_end@17
.cond_else@16:
.cond_end@17:
   LDA   #<pos_walk_next
   STA   DPTR
   LDA   #>pos_walk_next
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   STA   __local_smc_body_draw__hi
   LDA   #<pos_walk_next
   STA   DPTR
   LDA   #>pos_walk_next
   STA   DPTR+1
   LDA   (DPTR),Y
   STA   __local_smc_body_draw__lo
   CLC
   ADC   #$01
   STA   HARGS
   LDA   #$00
   ADC   #$00
   STA   HARGS+1
   LDA   #$03
   STA   HARGS+2
   LDA   #$00
   STA   HARGS+3
   JSR   sdivmod16
   LDA   HARGS+6
   STA   __local_smc_body_draw__lo
   LDA   #<pos_walk_next
   STA   DPTR
   LDA   #>pos_walk_next
   STA   DPTR+1
   LDA   __local_smc_body_draw__lo
   LDY   #$00
   STA   (DPTR),Y
.if_end@14:
   LDA   __local_smc_body_draw__0
   BEQ   .if_else@19
.smc_body_draw@ssa_block@3:
   LDA   __local_smc_body_draw__hi
   CMP   #$00
   BEQ   .dispatch@0@case@0
.smc_body_draw@asm_ssa_block@3:
   CMP   #$01
   BEQ   .dispatch@0@case@1
.smc_body_draw@asm_ssa_block@4:
   LDX   __zpabi_smc_body_draw__frame_idx
   LDA   companion_neg_pose3_lo,X
   STA   __local_smc_body_draw__lo
   JMP   .dispatch@0@end
.dispatch@0@case@0:
   LDX   __zpabi_smc_body_draw__frame_idx
   LDA   companion_neg_pose1_lo,X
   STA   __local_smc_body_draw__lo
   JMP   .dispatch@0@end
.dispatch@0@case@1:
   LDX   __zpabi_smc_body_draw__frame_idx
   LDA   companion_neg_pose2_lo,X
   STA   __local_smc_body_draw__lo
   JMP   .dispatch@0@end
.dispatch@0@end:
   LDA   __local_smc_body_draw__hi
   CMP   #$00
   BEQ   .dispatch@1@case@0
.smc_body_draw@asm_ssa_block@5:
   CMP   #$01
   BEQ   .dispatch@1@case@1
.smc_body_draw@asm_ssa_block@6:
   LDX   __zpabi_smc_body_draw__frame_idx
   LDA   companion_neg_pose3_hi,X
   STA   __local_smc_body_draw__hi
   JMP   .dispatch@1@end
.dispatch@1@case@0:
   LDX   __zpabi_smc_body_draw__frame_idx
   LDA   companion_neg_pose1_hi,X
   STA   __local_smc_body_draw__hi
   JMP   .dispatch@1@end
.dispatch@1@case@1:
   LDX   __zpabi_smc_body_draw__frame_idx
   LDA   companion_neg_pose2_hi,X
   STA   __local_smc_body_draw__hi
   JMP   .dispatch@1@end
.dispatch@1@end:
   JMP   .if_end@18
.if_else@19:
   LDA   __local_smc_body_draw__hi
   CMP   #$00
   BEQ   .dispatch@2@case@0
.smc_body_draw@asm_ssa_block@7:
   CMP   #$01
   BEQ   .dispatch@2@case@1
.smc_body_draw@asm_ssa_block@8:
   LDX   __zpabi_smc_body_draw__frame_idx
   LDA   companion_pos_pose3_lo,X
   STA   __local_smc_body_draw__lo
   JMP   .dispatch@2@end
.dispatch@2@case@0:
   LDX   __zpabi_smc_body_draw__frame_idx
   LDA   companion_pos_pose1_lo,X
   STA   __local_smc_body_draw__lo
   JMP   .dispatch@2@end
.dispatch@2@case@1:
   LDX   __zpabi_smc_body_draw__frame_idx
   LDA   companion_pos_pose2_lo,X
   STA   __local_smc_body_draw__lo
   JMP   .dispatch@2@end
.dispatch@2@end:
   LDA   __local_smc_body_draw__hi
   CMP   #$00
   BEQ   .dispatch@3@case@0
.smc_body_draw@asm_ssa_block@9:
   CMP   #$01
   BEQ   .dispatch@3@case@1
.smc_body_draw@asm_ssa_block@10:
   LDX   __zpabi_smc_body_draw__frame_idx
   LDA   companion_pos_pose3_hi,X
   STA   __local_smc_body_draw__hi
   JMP   .dispatch@3@end
.dispatch@3@case@0:
   LDX   __zpabi_smc_body_draw__frame_idx
   LDA   companion_pos_pose1_hi,X
   STA   __local_smc_body_draw__hi
   JMP   .dispatch@3@end
.dispatch@3@case@1:
   LDX   __zpabi_smc_body_draw__frame_idx
   LDA   companion_pos_pose2_hi,X
   STA   __local_smc_body_draw__hi
   JMP   .dispatch@3@end
.dispatch@3@end:
.if_end@18:
   LDA   #$03
   STA   __zpabi_draw_sprite__width
   LDA   #$08
   STA   __zpabi_draw_sprite__height
   LDA   __zpabi_smc_body_draw__sprite_x
   STA   __zpabi_draw_sprite__sprite_x
   LDA   __zpabi_smc_body_draw__sprite_y
   STA   __zpabi_draw_sprite__sprite_y
   LDA   __local_smc_body_draw__lo
   STA   __zpabi_draw_sprite__tile_src_0
   LDA   __local_smc_body_draw__hi
   STA   __zpabi_draw_sprite__tile_src_1
   LDA   __zpabi_smc_body_draw__page_flag
   STA   __zpabi_draw_sprite__page_flag
   JMP   draw_sprite

player_catch:
   SUBROUTINE

.player_catch@asm_ssa_block@0:
   LDA   __zpabi_player_catch__screen_x
   CMP   #$40
   BCC   .or_true@20
.player_catch@asm_ssa_block@1:
   CMP   #$50
   BCS   .or_true@20
.player_catch@ssa_block@2:
   LDA   #$00
   STA   __local_player_catch__1
   JMP   .or_end@21
.or_true@20:
   LDA   #$01
   STA   __local_player_catch__1
.or_end@21:
   LDA   __local_player_catch__1
   BEQ   .if_end@22
.player_catch@asm_ssa_block@2:
   RTS
.if_end@22:
   LDA   __zpabi_player_catch__player_col
   SEC
   SBC   #$08
   STA   __local_player_catch__1
   LDX   __zpabi_player_catch__slot
   LDA   companion_row,X
   STA   __local_player_catch__0
   LDA   __local_player_catch__1
   CMP   __local_player_catch__0
   BCC   .if_end@23
.player_catch@asm_ssa_block@3:
   RTS
.if_end@23:
   CLC
   ADC   #$1A
   STA   __local_player_catch__1
   LDA   companion_row,X
   STA   __local_player_catch__0
   LDA   __local_player_catch__1
   CMP   __local_player_catch__0
   BCS   .if_end@24
.player_catch@asm_ssa_block@4:
   RTS
.if_end@24:
   LDA   #$FF
   STA   hit_flag
   RTS

active_pos_step:
   SUBROUTINE

.active_pos_step@asm_ssa_block@0:
   LDY   __zpabi_active_pos_step__slot
   JSR   prng
   CMP   #$05
   BCS   .if_end@25
.active_pos_step@asm_ssa_block@1:
   LDA   #$FF
   STA   companion_dir,Y
.if_end@25:
   LDA   companion_pos_hi,Y
   STA   __local_active_pos_step__0
   LDA   companion_pos_lo,Y
   CLC
   ADC   #$03
   STA   __local_active_pos_step__1
   LDA   __local_active_pos_step__0
   ADC   #$00
   STA   __local_active_pos_step__0
   LDA   __local_active_pos_step__1
   STA   companion_pos_lo,Y
   LDA   __local_active_pos_step__0
   STA   companion_pos_hi,Y
   LDA   #$00
   CMP   #$00
   BNE   .and_false@26
.active_pos_step@asm_ssa_block@2:
   LDA   __local_active_pos_step__0
   CMP   #$03
   BNE   .and_false@26
.active_pos_step@asm_ssa_block@3:
   LDA   __local_active_pos_step__1
   CMP   #$52
   BCC   .and_false@26
.active_pos_step@ssa_block@3:
   LDA   #$01
   STA   __local_active_pos_step__1
   JMP   .and_end@27
.and_false@26:
   LDA   #$00
   STA   __local_active_pos_step__1
.and_end@27:
   LDA   __local_active_pos_step__1
   BEQ   .if_end@28
.active_pos_step@asm_ssa_block@4:
   LDA   #$FF
   STA   companion_dir,Y
   LDX   __zpabi_active_pos_step__player_floor
   LDA   floor_thresh,X
   CLC
   ADC   #$0B
   STA   companion_row,Y
   LDA   #$00
   RTS
.if_end@28:
   LDA   #$01
   RTS

active_neg_step:
   SUBROUTINE

.active_neg_step@asm_ssa_block@0:
   LDY   __zpabi_active_neg_step__slot
   JSR   prng
   CMP   #$05
   BCS   .if_end@29
.active_neg_step@asm_ssa_block@1:
   LDA   #$01
   STA   companion_dir,Y
.if_end@29:
   LDA   companion_pos_hi,Y
   STA   __local_active_neg_step__0
   LDA   companion_pos_lo,Y
   SEC
   SBC   #$03
   STA   __local_active_neg_step__1
   LDA   __local_active_neg_step__0
   SBC   #$00
   STA   __local_active_neg_step__0
   LDA   __local_active_neg_step__1
   STA   companion_pos_lo,Y
   LDA   __local_active_neg_step__0
   STA   companion_pos_hi,Y
   ORA   #$00
   BNE   .and_false@30
.active_neg_step@asm_ssa_block@2:
   LDA   __local_active_neg_step__1
   CMP   #$3E
   BCS   .and_false@30
.active_neg_step@ssa_block@3:
   LDA   #$01
   STA   __local_active_neg_step__1
   JMP   .and_end@31
.and_false@30:
   LDA   #$00
   STA   __local_active_neg_step__1
.and_end@31:
   LDA   __local_active_neg_step__1
   BEQ   .if_end@32
.active_neg_step@asm_ssa_block@3:
   LDA   #$01
   STA   companion_dir,Y
   LDX   __zpabi_active_neg_step__player_floor
   LDA   floor_thresh,X
   CLC
   ADC   #$0B
   STA   companion_row,Y
   LDA   #$00
   RTS
.if_end@32:
   LDA   #$01
   RTS

drift_step:
   SUBROUTINE

.drift_step@asm_ssa_block@0:
   LDX   __zpabi_drift_step__slot
   LDA   companion_row,X
   STA   __local_drift_step__0
   CMP   #$63
   BEQ   .or_true@35
.drift_step@asm_ssa_block@1:
   CMP   #$8B
   BEQ   .or_true@35
.drift_step@ssa_block@2:
   LDA   #$00
   STA   __local_drift_step__pos_0
   JMP   .or_end@36
.or_true@35:
   LDA   #$01
   STA   __local_drift_step__pos_0
.or_end@36:
   LDA   __local_drift_step__pos_0
   BNE   .or_true@33
.drift_step@asm_ssa_block@2:
   LDA   __local_drift_step__0
   CMP   #$B3
   BEQ   .or_true@33
.drift_step@ssa_block@4:
   LDA   #$00
   STA   __local_drift_step__pos_0
   JMP   .or_end@34
.or_true@33:
   LDA   #$01
   STA   __local_drift_step__pos_0
.or_end@34:
   LDA   __local_drift_step__pos_0
   BEQ   .if_else@38
.drift_step@asm_ssa_block@3:
   LDA   __local_drift_step__0
   SEC
   SBC   #$04
   STA   __local_drift_step__pos_1
   LDA   #$00
   SBC   #$00
   LDA   __local_drift_step__pos_1
   LDY   #$00
   STA   (__zpabi_drift_step__out_sprite_y_0),Y
   LDX   __zpabi_drift_step__slot
   LDA   #$01
   STA   companion_state,X
   LDA   companion_pos_hi,X
   STA   __local_drift_step__pos_1
   LDA   companion_pos_lo,X
   STA   __local_drift_step__pos_0
   LDA   __local_drift_step__pos_1
   STA   __local_drift_step__0
   LDA   companion_dir,X
   BPL   .if_else@40
.drift_step@ssa_block@6:
   LDA   __local_drift_step__pos_0
   SEC
   SBC   #$03
   STA   __local_drift_step__pos_0
   LDA   __local_drift_step__0
   SBC   #$00
   STA   __local_drift_step__pos_1
   JMP   .if_end@39
.if_else@40:
   LDA   __local_drift_step__pos_0
   CLC
   ADC   #$03
   STA   __local_drift_step__pos_0
   LDA   __local_drift_step__0
   ADC   #$00
   STA   __local_drift_step__pos_1
.if_end@39:
   LDX   __zpabi_drift_step__slot
   LDA   __local_drift_step__pos_0
   STA   companion_pos_lo,X
   LDA   __local_drift_step__pos_1
   STA   companion_pos_hi,X
   JMP   .if_end@37
.if_else@38:
   LDA   __local_drift_step__0
   CLC
   ADC   #$04
   LDX   __zpabi_drift_step__slot
   STA   companion_row,X
   LDY   #$00
   STA   (__zpabi_drift_step__out_sprite_y_0),Y
.if_end@37:
   RTS

companion_update:
   SUBROUTINE

.companion_update@asm_ssa_block@0:
   LDA   __zpabi_companion_update__gate
   BPL   .if_end@41
.companion_update@asm_ssa_block@1:
   RTS
.if_end@41:
   LDX   #$01
.loop@1_start:
   LDA   companion_state,X
   STA   __local_companion_update__0
   BMI   .lb_skip@0
   JMP   .if_end@42
.lb_skip@0:
.companion_update@asm_ssa_block@2:
   LDA   #<__local_companion_update__sprite_y
   STA   __local_companion_update__0
   LDA   #>__local_companion_update__sprite_y
   STA   __local_companion_update__0+1
   STX   __zpabi_drift_step__slot
   LDA   __local_companion_update__0
   STA   __zpabi_drift_step__out_sprite_y_0
   LDA   __local_companion_update__1
   STA   __zpabi_drift_step__out_sprite_y_1
   STX   __local_companion_update__slot
   JSR   drift_step
   LDX   __local_companion_update__slot
   STX   __zpabi_compute_screen_x__slot
   LDA   __zpabi_companion_update__player_y
   STA   __zpabi_compute_screen_x__player_y
   LDA   __zpabi_companion_update__sprite_xref
   STA   __zpabi_compute_screen_x__sprite_xref
   STX   __local_companion_update__slot
   JSR   compute_screen_x
   LDX   __local_companion_update__slot
   LDA   HARGS
   STA   __local_companion_update__3
   LDY   HARGS
   LDA   proj_frame_idx,Y
   STA   __local_companion_update__1
   LDA   companion_state,X
   STA   __local_companion_update__0
   STX   __zpabi_smc_body_draw__slot
   LDA   proj_screen_col,Y
   STA   __zpabi_smc_body_draw__sprite_x
   LDA   __local_companion_update__sprite_y
   STA   __zpabi_smc_body_draw__sprite_y
   LDA   proj_frame_idx,Y
   STA   __zpabi_smc_body_draw__frame_idx
   LDA   __local_companion_update__0
   STA   __zpabi_smc_body_draw__state
   LDA   __zpabi_companion_update__page_flag
   STA   __zpabi_smc_body_draw__page_flag
   STX   __local_companion_update__slot
   JSR   smc_body_draw
   LDX   __local_companion_update__slot
   STX   __zpabi_player_catch__slot
   LDA   __local_companion_update__3
   STA   __zpabi_player_catch__screen_x
   LDA   __zpabi_companion_update__player_col
   STA   __zpabi_player_catch__player_col
   STX   __local_companion_update__slot
   JSR   player_catch
   LDX   __local_companion_update__slot
   JMP   .loop@1_continue
.if_end@42:
   BNE   .if_else@44
.companion_update@asm_ssa_block@3:
   LDA   #$01
   STA   companion_state,X
   JMP   .if_end@43
.if_else@44:
   LDA   companion_dir,X
   STA   __local_companion_update__0
   LDA   companion_dir,X
   BPL   .cond_else@45
.companion_update@ssa_block@4:
   STX   __zpabi_active_neg_step__slot
   LDA   __zpabi_companion_update__player_floor
   STA   __zpabi_active_neg_step__player_floor
   STX   __local_companion_update__slot
   JSR   active_neg_step
   LDX   __local_companion_update__slot
   STA   __local_companion_update__0
   JMP   .cond_end@46
.cond_else@45:
   STX   __zpabi_active_pos_step__slot
   LDA   __zpabi_companion_update__player_floor
   STA   __zpabi_active_pos_step__player_floor
   STX   __local_companion_update__slot
   JSR   active_pos_step
   LDX   __local_companion_update__slot
   STA   __local_companion_update__0
.cond_end@46:
   LDA   __local_companion_update__0
   BEQ   .lnot_true@9
.companion_update@asm_ssa_block@4:
   LDA   #$00
   JMP   .lnot_end@10
.lnot_true@9:
   LDA   #$01
.lnot_end@10:
   STA   __local_companion_update__0
   ORA   #$00
   BEQ   .if_end@47
.companion_update@asm_ssa_block@5:
   JMP   .loop@1_continue
.if_end@47:
.if_end@43:
   STX   __zpabi_compute_screen_x__slot
   LDA   __zpabi_companion_update__player_y
   STA   __zpabi_compute_screen_x__player_y
   LDA   __zpabi_companion_update__sprite_xref
   STA   __zpabi_compute_screen_x__sprite_xref
   STX   __local_companion_update__slot
   JSR   compute_screen_x
   LDX   __local_companion_update__slot
   LDA   HARGS
   STA   __local_companion_update__4
   LDA   HARGS+1
   STA   __local_companion_update__0
   ORA   #$00
   BEQ   .if_end@48
.companion_update@asm_ssa_block@6:
   JMP   .loop@1_continue
.if_end@48:
   LDA   __local_companion_update__4
   CMP   #$9A
   BCC   .if_end@49
.companion_update@asm_ssa_block@7:
   JMP   .loop@1_continue
.if_end@49:
   STX   __zpabi_entity_proximity__slot
   LDA   __local_companion_update__4
   STA   __zpabi_entity_proximity__screen_x
   LDA   __zpabi_companion_update__hit_max
   STA   __zpabi_entity_proximity__hit_max
   STX   __local_companion_update__slot
   JSR   entity_proximity
   LDX   __local_companion_update__slot
   LDA   companion_row,X
   STA   __local_companion_update__3
   LDY   __local_companion_update__4
   LDA   proj_frame_idx,Y
   STA   __local_companion_update__1
   LDA   companion_state,X
   STA   __local_companion_update__0
   STX   __zpabi_smc_body_draw__slot
   LDA   proj_screen_col,Y
   STA   __zpabi_smc_body_draw__sprite_x
   LDA   companion_row,X
   STA   __zpabi_smc_body_draw__sprite_y
   LDA   proj_frame_idx,Y
   STA   __zpabi_smc_body_draw__frame_idx
   LDA   __local_companion_update__0
   STA   __zpabi_smc_body_draw__state
   LDA   __zpabi_companion_update__page_flag
   STA   __zpabi_smc_body_draw__page_flag
   STX   __local_companion_update__slot
   JSR   smc_body_draw
   LDX   __local_companion_update__slot
   STX   __zpabi_player_catch__slot
   LDA   __local_companion_update__4
   STA   __zpabi_player_catch__screen_x
   LDA   __zpabi_companion_update__player_col
   STA   __zpabi_player_catch__player_col
   STX   __local_companion_update__slot
   JSR   player_catch
   LDX   __local_companion_update__slot
.loop@1_continue:
   DEX
   BPL   .companion_update@asm_ssa_split@0
.companion_update@asm_ssa_block@8:
   RTS
.companion_update@asm_ssa_split@0:
   JMP   .loop@1_start

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

proj_frame_idx:
   DC.B  $00, $01, $02, $03, $04, $05, $06, $00, $01, $02, $03, $04, $05, $06, $00, $01
   DC.B  $02, $03, $04, $05, $06, $00, $01, $02, $03, $04, $05, $06, $00, $01, $02, $03
   DC.B  $04, $05, $06, $00, $01, $02, $03, $04, $05, $06, $00, $01, $02, $03, $04, $05
   DC.B  $06, $00, $01, $02, $03, $04, $05, $06, $00, $01, $02, $03, $04, $05, $06, $00
   DC.B  $01, $02, $03, $04, $05, $06, $00, $01, $02, $03, $04, $05, $06, $00, $01, $02
   DC.B  $03, $04, $05, $06, $00, $01, $02, $03, $04, $05, $06, $00, $01, $02, $03, $04
   DC.B  $05, $06, $00, $01, $02, $03, $04, $05, $06, $00, $01, $02, $03, $04, $05, $06
   DC.B  $00, $01, $02, $03, $04, $05, $06, $00, $01, $02, $03, $04, $05, $06, $00, $01
   DC.B  $02, $03, $04, $05, $06, $00, $01, $02, $03, $04, $05, $06, $00, $01, $02, $03
   DC.B  $04, $05, $06, $00, $01, $02, $03, $04, $05, $06, $00, $01, $02, $03, $04, $05
   DC.B  $06, $00, $01, $02, $03

companion_pos_pose1_lo:
   DC.B  $51, $71, $91, $B1, $D1, $F1, $11

companion_pos_pose1_hi:
   DC.B  $A0, $A0, $A0, $A0, $A0, $A0, $A1

companion_pos_pose2_lo:
   DC.B  $31, $51, $71, $91, $B1, $D1, $F1

companion_pos_pose2_hi:
   DC.B  $A1, $A1, $A1, $A1, $A1, $A1, $A1

companion_pos_pose3_lo:
   DC.B  $11, $31, $51, $71, $91, $B1, $D1

companion_pos_pose3_hi:
   DC.B  $A2, $A2, $A2, $A2, $A2, $A2, $A2

companion_neg_pose1_lo:
   DC.B  $B1, $D1, $F1, $11, $31, $51, $71

companion_neg_pose1_hi:
   DC.B  $9D, $9D, $9D, $9E, $9E, $9E, $9E

companion_neg_pose2_lo:
   DC.B  $91, $B1, $D1, $F1, $11, $31, $51

companion_neg_pose2_hi:
   DC.B  $9E, $9E, $9E, $9E, $9F, $9F, $9F

companion_neg_pose3_lo:
   DC.B  $71, $91, $B1, $D1, $F1, $11, $31

companion_neg_pose3_hi:
   DC.B  $9F, $9F, $9F, $9F, $9F, $A0, $A0

pos_walk_next:
   DS.B  1
