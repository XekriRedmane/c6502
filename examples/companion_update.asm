SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
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
__zpabi_active_neg_step__player_floor	EQU	$88
__zpabi_active_pos_step__player_floor	EQU	$88
__zpabi_compute_screen_x__player_y	EQU	$88
__zpabi_drift_step__out_sprite_y_0	EQU	$88
__zpabi_find_active_entity__out_row_0	EQU	$88
__zpabi_player_catch__screen_x	EQU	$88
__zpabi_compute_screen_x__sprite_xref	EQU	$89
__zpabi_drift_step__out_sprite_y_1	EQU	$89
__zpabi_find_active_entity__out_row_1	EQU	$89
__zpabi_player_catch__player_col	EQU	$89
__zpabi_entity_proximity__screen_x	EQU	$8A
__local_active_neg_step__0	EQU	$8B
__local_active_pos_step__0	EQU	$8B
__local_compute_screen_x__0	EQU	$8B
__local_player_catch__0	EQU	$8B
__zpabi_entity_proximity__hit_max	EQU	$8B
__zpabi_find_active_entity__hit_max	EQU	$8B
__zpabi_smc_body_draw__slot	EQU	$8B
__local_active_neg_step__1	EQU	$8C
__local_active_pos_step__1	EQU	$8C
__local_drift_step__pos_1	EQU	$8C
__local_player_catch__1	EQU	$8C
__zpabi_smc_body_draw__sprite_x	EQU	$8C
__local_compute_screen_x__2	EQU	$8D
__local_drift_step__0	EQU	$8D
__zpabi_smc_body_draw__sprite_y	EQU	$8D
__local_compute_screen_x__3	EQU	$8E
__local_entity_proximity__entity_row	EQU	$8E
__zpabi_smc_body_draw__frame_idx	EQU	$8E
__zpabi_smc_body_draw__state	EQU	$8F
__zpabi_smc_body_draw__page_flag	EQU	$90
__zpabi_draw_sprite__width	EQU	$91
__zpabi_draw_sprite__height	EQU	$92
__zpabi_draw_sprite__sprite_x	EQU	$93
__zpabi_draw_sprite__sprite_y	EQU	$94
__zpabi_draw_sprite__tile_src_0	EQU	$95
__zpabi_draw_sprite__tile_src_1	EQU	$96
__zpabi_draw_sprite__page_flag	EQU	$97
__local_companion_update__0	EQU	$98
__local_companion_update__1	EQU	$99
__local_companion_update__2	EQU	$9A
__local_companion_update__3	EQU	$9B
__local_companion_update__slot	EQU	$9C
__local_companion_update__sprite_y	EQU	$9D
__local_smc_body_draw__hi	EQU	$9F
__local_smc_body_draw__lo	EQU	$A0
__local_smc_body_draw__0	EQU	$A1
__local_smc_body_draw__1	EQU	$A2
__local_smc_body_draw__2	EQU	$A3

; @zp-link-meta-begin
; def active_neg_step params=__zpabi_active_neg_step__slot,__zpabi_active_neg_step__player_floor locals=__local_active_neg_step__0,__local_active_neg_step__1 indirect=false in_cycle=false
; def active_pos_step params=__zpabi_active_pos_step__slot,__zpabi_active_pos_step__player_floor locals=__local_active_pos_step__0,__local_active_pos_step__1 indirect=false in_cycle=false
; def companion_update params=__zpabi_companion_update__gate,__zpabi_companion_update__player_y,__zpabi_companion_update__sprite_xref,__zpabi_companion_update__player_col,__zpabi_companion_update__player_floor,__zpabi_companion_update__hit_max,__zpabi_companion_update__page_flag locals=__local_companion_update__0,__local_companion_update__1,__local_companion_update__2,__local_companion_update__3,__local_companion_update__slot,__local_companion_update__sprite_y,__local_companion_update__5 indirect=false in_cycle=false
; def compute_screen_x params=__zpabi_compute_screen_x__slot,__zpabi_compute_screen_x__player_y,__zpabi_compute_screen_x__sprite_xref locals=__local_compute_screen_x__0,__local_compute_screen_x__1,__local_compute_screen_x__2,__local_compute_screen_x__3 indirect=false in_cycle=false
; def drift_step params=__zpabi_drift_step__slot,__zpabi_drift_step__out_sprite_y_0,__zpabi_drift_step__out_sprite_y_1 locals=__local_drift_step__pos_0,__local_drift_step__pos_1,__local_drift_step__0 indirect=false in_cycle=false
; def entity_proximity params=__zpabi_entity_proximity__slot,__zpabi_entity_proximity__screen_x,__zpabi_entity_proximity__hit_max locals=__local_entity_proximity__0,__local_entity_proximity__1,__local_entity_proximity__entity_row indirect=false in_cycle=false
; def find_active_entity params=__zpabi_find_active_entity__hit_max,__zpabi_find_active_entity__out_row_0,__zpabi_find_active_entity__out_row_1 locals=__local_find_active_entity__0 indirect=false in_cycle=false
; def player_catch params=__zpabi_player_catch__slot,__zpabi_player_catch__screen_x,__zpabi_player_catch__player_col locals=__local_player_catch__0,__local_player_catch__1 indirect=false in_cycle=false
; def smc_body_draw params=__zpabi_smc_body_draw__slot,__zpabi_smc_body_draw__sprite_x,__zpabi_smc_body_draw__sprite_y,__zpabi_smc_body_draw__frame_idx,__zpabi_smc_body_draw__state,__zpabi_smc_body_draw__page_flag locals=__local_smc_body_draw__hi,__local_smc_body_draw__lo,__local_smc_body_draw__0,__local_smc_body_draw__1,__local_smc_body_draw__2,__local_smc_body_draw__3,__local_smc_body_draw__4,__local_smc_body_draw__5 indirect=false in_cycle=false
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
; share member=entity_proximity:__zpabi_entity_proximity__screen_x holder=companion_update live=companion_update:entity_proximity,smc_body_draw forward=companion_update>entity_proximity writer=companion_update
; share member=entity_proximity:__zpabi_entity_proximity__hit_max member=find_active_entity:__zpabi_find_active_entity__hit_max forward=entity_proximity>find_active_entity
; item compute_screen_x section=.text local=true zero=false
; item find_active_entity section=.text local=true zero=false
; item entity_proximity section=.text local=true zero=false
; item smc_body_draw section=.text local=true zero=false
; item player_catch section=.text local=true zero=false
; item active_pos_step section=.text local=true zero=false
; item active_neg_step section=.text local=true zero=false
; item drift_step section=.text local=true zero=false
; item companion_update section=.text local=false zero=false
; item proj_screen_col section=.rodata local=true zero=false
; item proj_frame_idx section=.rodata local=true zero=false
; item companion_pos_pose1_lo section=.rodata local=true zero=false
; item companion_pos_pose1_hi section=.rodata local=true zero=false
; item companion_pos_pose2_lo section=.rodata local=true zero=false
; item companion_pos_pose2_hi section=.rodata local=true zero=false
; item companion_pos_pose3_lo section=.rodata local=true zero=false
; item companion_pos_pose3_hi section=.rodata local=true zero=false
; item companion_neg_pose1_lo section=.rodata local=true zero=false
; item companion_neg_pose1_hi section=.rodata local=true zero=false
; item companion_neg_pose2_lo section=.rodata local=true zero=false
; item companion_neg_pose2_hi section=.rodata local=true zero=false
; item companion_neg_pose3_lo section=.rodata local=true zero=false
; item companion_neg_pose3_hi section=.rodata local=true zero=false
; item companion_pos_lo_tbl section=.rodata local=true zero=false
; item companion_pos_hi_tbl section=.rodata local=true zero=false
; item companion_neg_lo_tbl section=.rodata local=true zero=false
; item companion_neg_hi_tbl section=.rodata local=true zero=false
; item pos_walk_next section=.bss local=true zero=true
; item neg_walk_next section=.bss local=true zero=true
; runtime hargs_size=32 hargs_need=11
; @zp-link-meta-end

compute_screen_x:
   SUBROUTINE

   LDX   __zpabi_compute_screen_x__player_y
   LDA   perspective_xoff_hi,X
   STA   __local_compute_screen_x__3
   LDA   perspective_xoff_lo,X
   SEC
   SBC   __zpabi_compute_screen_x__sprite_xref
   STA   __local_compute_screen_x__2
   BCS   .carry_inc@0
   DEC   __local_compute_screen_x__3
.carry_inc@0:
   LDX   __zpabi_compute_screen_x__slot
   LDA   companion_pos_lo,X
   SEC
   SBC   __local_compute_screen_x__2
   STA   __local_compute_screen_x__0
   LDA   companion_pos_hi,X
   SBC   __local_compute_screen_x__3
   TAX
   LDA   __local_compute_screen_x__0
   RTS

find_active_entity:
   SUBROUTINE

   LDX   __zpabi_find_active_entity__hit_max
.loop@0_start:
   TXA
   BMI   .loop@0_break
   LDA   entity_hit_state,X
   BMI   .if_end@0
   LDA   entity_hit_row,X
   SEC
   SBC   #$08
   LDY   #$00
   STA   (__zpabi_find_active_entity__out_row_0),Y
   LDA   #$01
   RTS
.if_end@0:
   DEX
   JMP   .loop@0_start
.loop@0_break:
   LDA   #$00
   RTS

entity_proximity:
   SUBROUTINE

   LDA   #<__local_entity_proximity__entity_row
   STA   __zpabi_find_active_entity__out_row_0
   LDA   #>__local_entity_proximity__entity_row
   STA   __zpabi_find_active_entity__out_row_1
   JSR   find_active_entity
   ORA   #$00
   BNE   .if_end@1
   RTS
.if_end@1:
   LDX   __zpabi_entity_proximity__slot
   LDA   __local_entity_proximity__entity_row
   CMP   companion_row,X
   BEQ   .if_end@2
   RTS
.if_end@2:
   LDA   __zpabi_entity_proximity__screen_x
   CMP   #$40
   BCC   .if_end@5
   CMP   #$47
   BCS   .if_end@5
   LDA   #$FF
   STA   companion_state,X
   LDA   companion_row,X
   ADC   #$04
   STA   companion_row,X
   RTS
.if_end@5:
   LDA   companion_dir,X
   BMI   .if_else@7
   LDA   __zpabi_entity_proximity__screen_x
   CMP   #$30
   BCC   .if_end@6
   CMP   #$38
   BCS   .if_end@6
   LDA   #$00
   STA   companion_state,X
   JMP   .if_end@6
.if_else@7:
   LDA   __zpabi_entity_proximity__screen_x
   CMP   #$50
   BCC   .if_end@13
   CMP   #$58
   BCS   .if_end@13
   LDA   #$00
   STA   companion_state,X
.if_end@13:
.if_end@6:
   RTS

smc_body_draw:
   SUBROUTINE

   LDX   __zpabi_smc_body_draw__slot
   LDA   companion_dir,X
   AND   #$80
   STA   __local_smc_body_draw__lo
   LDA   #$00
   CMP   #$00
   BNE   .cmp_true@0
   LDA   __local_smc_body_draw__lo
   CMP   #$00
   BNE   .cmp_true@0
   LDA   #$00
   JMP   .cmp_end@1
.cmp_true@0:
   LDA   #$01
.cmp_end@1:
   STA   __local_smc_body_draw__2
   LDA   __zpabi_smc_body_draw__state
   BNE   .if_else@15
   LDA   #$00
   STA   __local_smc_body_draw__hi
   JMP   .if_end@14
.if_else@15:
   LDA   __local_smc_body_draw__2
   BEQ   .cond_else@16
   LDA   #<neg_walk_next
   STA   __local_smc_body_draw__0
   LDA   #>neg_walk_next
   STA   __local_smc_body_draw__1
   JMP   .cond_end@17
.cond_else@16:
   LDA   #<pos_walk_next
   STA   __local_smc_body_draw__0
   LDY   #>pos_walk_next
   STY   __local_smc_body_draw__1
.cond_end@17:
   LDY   #$00
   LDA   (__local_smc_body_draw__0),Y
   STA   __local_smc_body_draw__hi
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
   LDY   #$00
   STA   (__local_smc_body_draw__0),Y
.if_end@14:
   LDA   __local_smc_body_draw__2
   BEQ   .if_else@19
   LDA   __local_smc_body_draw__hi
   ASL
   TAX
   LDA   companion_neg_lo_tbl,X
   STA   __local_smc_body_draw__hi
   LDA   companion_neg_lo_tbl+1,X
   STA   __local_smc_body_draw__lo
   LDY   __zpabi_smc_body_draw__frame_idx
   LDA   (__local_smc_body_draw__hi),Y
   STA   __local_smc_body_draw__lo
   LDA   companion_neg_hi_tbl,X
   STA   __local_smc_body_draw__0
   LDA   companion_neg_hi_tbl+1,X
   STA   __local_smc_body_draw__1
   LDA   (__local_smc_body_draw__0),Y
   JMP   .if_end@18
.if_else@19:
   LDA   __local_smc_body_draw__hi
   ASL
   TAX
   LDA   companion_pos_lo_tbl,X
   STA   __local_smc_body_draw__hi
   LDA   companion_pos_lo_tbl+1,X
   STA   __local_smc_body_draw__lo
   LDY   __zpabi_smc_body_draw__frame_idx
   LDA   (__local_smc_body_draw__hi),Y
   STA   __local_smc_body_draw__lo
   LDA   companion_pos_hi_tbl,X
   STA   __local_smc_body_draw__0
   LDA   companion_pos_hi_tbl+1,X
   STA   __local_smc_body_draw__1
   LDA   (__local_smc_body_draw__0),Y
.if_end@18:
   STA   __zpabi_draw_sprite__tile_src_1
   LDA   __local_smc_body_draw__lo
   STA   __zpabi_draw_sprite__tile_src_0
   LDA   #$03
   STA   __zpabi_draw_sprite__width
   LDA   #$08
   STA   __zpabi_draw_sprite__height
   LDA   __zpabi_smc_body_draw__sprite_x
   STA   __zpabi_draw_sprite__sprite_x
   LDA   __zpabi_smc_body_draw__sprite_y
   STA   __zpabi_draw_sprite__sprite_y
   LDA   __zpabi_smc_body_draw__page_flag
   STA   __zpabi_draw_sprite__page_flag
   JMP   draw_sprite

player_catch:
   SUBROUTINE

   LDA   __zpabi_player_catch__screen_x
   CMP   #$40
   BCC   .player_catch@scfold@0
   CMP   #$50
   BCC   .if_end@22
.player_catch@scfold@0:
   RTS
.if_end@22:
   LDA   __zpabi_player_catch__player_col
   SEC
   SBC   #$08
   STA   __local_player_catch__0
   LDX   __zpabi_player_catch__slot
   LDA   companion_row,X
   STA   __local_player_catch__1
   LDA   __local_player_catch__0
   CMP   __local_player_catch__1
   BCC   .if_end@23
   RTS
.if_end@23:
   ADC   #$1A
   CMP   __local_player_catch__1
   BCS   .if_end@24
   RTS
.if_end@24:
   LDA   #$FF
   STA   hit_flag
   RTS

active_pos_step:
   SUBROUTINE

   JSR   prng
   CMP   #$05
   BCS   .if_end@25
   LDX   __zpabi_active_pos_step__slot
   LDA   #$FF
   STA   companion_dir,X
.if_end@25:
   LDX   __zpabi_active_pos_step__slot
   LDA   companion_pos_hi,X
   STA   __local_active_pos_step__1
   LDA   companion_pos_lo,X
   CLC
   ADC   #$03
   STA   __local_active_pos_step__0
   BCC   .carry_inc@1
   INC   __local_active_pos_step__1
.carry_inc@1:
   STA   companion_pos_lo,X
   LDA   __local_active_pos_step__1
   STA   companion_pos_hi,X
   LDA   #$00
   CMP   #$00
   BNE   .if_end@28
   LDA   __local_active_pos_step__1
   CMP   #$03
   BNE   .if_end@28
   LDA   __local_active_pos_step__0
   CMP   #$52
   BCC   .if_end@28
   LDA   #$FF
   STA   companion_dir,X
   LDX   __zpabi_active_pos_step__player_floor
   LDA   floor_thresh,X
   CLC
   ADC   #$0B
   LDX   __zpabi_active_pos_step__slot
   STA   companion_row,X
   LDA   #$00
   RTS
.if_end@28:
   LDA   #$01
   RTS

active_neg_step:
   SUBROUTINE

   JSR   prng
   CMP   #$05
   BCS   .if_end@29
   LDX   __zpabi_active_neg_step__slot
   LDA   #$01
   STA   companion_dir,X
.if_end@29:
   LDX   __zpabi_active_neg_step__slot
   LDA   companion_pos_hi,X
   STA   __local_active_neg_step__1
   LDA   companion_pos_lo,X
   SEC
   SBC   #$03
   STA   __local_active_neg_step__0
   BCS   .carry_inc@2
   DEC   __local_active_neg_step__1
.carry_inc@2:
   STA   companion_pos_lo,X
   LDA   __local_active_neg_step__1
   STA   companion_pos_hi,X
   ORA   #$00
   BNE   .if_end@32
   LDA   __local_active_neg_step__0
   CMP   #$3E
   BCS   .if_end@32
   LDA   #$01
   STA   companion_dir,X
   LDX   __zpabi_active_neg_step__player_floor
   LDA   floor_thresh,X
   ADC   #$0B
   LDX   __zpabi_active_neg_step__slot
   STA   companion_row,X
   LDA   #$00
   RTS
.if_end@32:
   LDA   #$01
   RTS

drift_step:
   SUBROUTINE

   LDX   __zpabi_drift_step__slot
   LDA   companion_row,X
   CMP   #$63
   BEQ   .drift_step@scfold@0
   CMP   #$8B
   BEQ   .drift_step@scfold@0
   CMP   #$B3
   BNE   .if_else@38
.drift_step@scfold@0:
   SEC
   SBC   #$04
   LDY   #$00
   STA   (__zpabi_drift_step__out_sprite_y_0),Y
   LDX   __zpabi_drift_step__slot
   LDA   #$01
   STA   companion_state,X
   LDA   companion_pos_hi,X
   STA   __local_drift_step__pos_1
   LDA   companion_pos_lo,X
   STA   __local_drift_step__0
   LDA   companion_dir,X
   BPL   .if_else@40
   LDA   __local_drift_step__0
   SEC
   SBC   #$03
   BCS   .if_end@39
   DEC   __local_drift_step__pos_1
   JMP   .if_end@39
.if_else@40:
   LDA   __local_drift_step__0
   CLC
   ADC   #$03
   BCC   .carry_inc@4
   INC   __local_drift_step__pos_1
.carry_inc@4:
.if_end@39:
   STA   companion_pos_lo,X
   LDA   __local_drift_step__pos_1
   STA   companion_pos_hi,X
   JMP   .if_end@37
.if_else@38:
   CLC
   ADC   #$04
   STA   companion_row,X
   LDY   #$00
   STA   (__zpabi_drift_step__out_sprite_y_0),Y
.if_end@37:
   RTS

companion_update:
   SUBROUTINE

   LDA   __zpabi_companion_update__gate
   BPL   .if_end@41
   RTS
.if_end@41:
   LDA   #$01
   STA   __local_companion_update__slot
.loop@1_start:
   LDX   __local_companion_update__slot
   LDA   companion_state,X
   BPL   .if_end@42
   STX   __zpabi_drift_step__slot
   LDA   #<__local_companion_update__sprite_y
   STA   __zpabi_drift_step__out_sprite_y_0
   LDA   #>__local_companion_update__sprite_y
   STA   __zpabi_drift_step__out_sprite_y_1
   JSR   drift_step
   LDA   __local_companion_update__slot
   STA   __zpabi_compute_screen_x__slot
   LDA   __zpabi_companion_update__player_y
   STA   __zpabi_compute_screen_x__player_y
   LDA   __zpabi_companion_update__sprite_xref
   STA   __zpabi_compute_screen_x__sprite_xref
   JSR   compute_screen_x
   STA   __local_companion_update__3
   LDX   __local_companion_update__3
   LDA   proj_screen_col,X
   STA   __local_companion_update__2
   LDA   proj_frame_idx,X
   STA   __local_companion_update__1
   LDX   __local_companion_update__slot
   LDA   companion_state,X
   STA   __local_companion_update__0
   STX   __zpabi_smc_body_draw__slot
   LDA   __local_companion_update__2
   STA   __zpabi_smc_body_draw__sprite_x
   LDA   __local_companion_update__sprite_y
   STA   __zpabi_smc_body_draw__sprite_y
   LDA   __local_companion_update__1
   STA   __zpabi_smc_body_draw__frame_idx
   LDA   __local_companion_update__0
   STA   __zpabi_smc_body_draw__state
   LDA   __zpabi_companion_update__page_flag
   STA   __zpabi_smc_body_draw__page_flag
   JSR   smc_body_draw
   LDA   __local_companion_update__slot
   STA   __zpabi_player_catch__slot
   LDA   __local_companion_update__3
   STA   __zpabi_player_catch__screen_x
   LDA   __zpabi_companion_update__player_col
   STA   __zpabi_player_catch__player_col
   JSR   player_catch
   JMP   .loop@1_continue
.if_end@42:
   BNE   .if_else@44
   LDA   #$01
   STA   companion_state,X
   JMP   .if_end@43
.if_else@44:
   LDA   companion_dir,X
   BPL   .cond_else@45
   STX   __zpabi_active_neg_step__slot
   LDA   __zpabi_companion_update__player_floor
   STA   __zpabi_active_neg_step__player_floor
   JSR   active_neg_step
   JMP   .cond_end@46
.cond_else@45:
   STX   __zpabi_active_pos_step__slot
   LDA   __zpabi_companion_update__player_floor
   STA   __zpabi_active_pos_step__player_floor
   JSR   active_pos_step
.cond_end@46:
   ORA   #$00
   BNE   .lb_skip@1
   JMP   .loop@1_continue
.lb_skip@1:
.if_end@43:
   LDA   __local_companion_update__slot
   STA   __zpabi_compute_screen_x__slot
   LDA   __zpabi_companion_update__player_y
   STA   __zpabi_compute_screen_x__player_y
   LDA   __zpabi_companion_update__sprite_xref
   STA   __zpabi_compute_screen_x__sprite_xref
   JSR   compute_screen_x
   STA   __local_companion_update__1
   TXA
   BNE   .loop@1_continue
   LDA   __local_companion_update__1
   STA   __zpabi_entity_proximity__screen_x
   CMP   #$9A
   BCS   .loop@1_continue
   LDA   __local_companion_update__slot
   STA   __zpabi_entity_proximity__slot
   LDA   __zpabi_companion_update__hit_max
   STA   __zpabi_entity_proximity__hit_max
   JSR   entity_proximity
   LDX   __local_companion_update__slot
   LDA   companion_row,X
   STA   __local_companion_update__3
   LDX   __zpabi_entity_proximity__screen_x
   LDA   proj_screen_col,X
   STA   __local_companion_update__2
   LDA   proj_frame_idx,X
   STA   __local_companion_update__1
   LDX   __local_companion_update__slot
   LDA   companion_state,X
   STA   __local_companion_update__0
   STX   __zpabi_smc_body_draw__slot
   LDA   __local_companion_update__2
   STA   __zpabi_smc_body_draw__sprite_x
   LDA   __local_companion_update__3
   STA   __zpabi_smc_body_draw__sprite_y
   LDA   __local_companion_update__1
   STA   __zpabi_smc_body_draw__frame_idx
   LDA   __local_companion_update__0
   STA   __zpabi_smc_body_draw__state
   LDA   __zpabi_companion_update__page_flag
   STA   __zpabi_smc_body_draw__page_flag
   JSR   smc_body_draw
   LDA   __local_companion_update__slot
   STA   __zpabi_player_catch__slot
   LDA   __zpabi_entity_proximity__screen_x
   STA   __zpabi_player_catch__screen_x
   LDA   __zpabi_companion_update__player_col
   STA   __zpabi_player_catch__player_col
   JSR   player_catch
.loop@1_continue:
   DEC   __local_companion_update__slot
   BMI   .lb_skip@0
   JMP   .loop@1_start
.lb_skip@0:
   RTS

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

companion_pos_lo_tbl:
   DC.W  companion_pos_pose1_lo
   DC.W  companion_pos_pose2_lo
   DC.W  companion_pos_pose3_lo

companion_pos_hi_tbl:
   DC.W  companion_pos_pose1_hi
   DC.W  companion_pos_pose2_hi
   DC.W  companion_pos_pose3_hi

companion_neg_lo_tbl:
   DC.W  companion_neg_pose1_lo
   DC.W  companion_neg_pose2_lo
   DC.W  companion_neg_pose3_lo

companion_neg_hi_tbl:
   DC.W  companion_neg_pose1_hi
   DC.W  companion_neg_pose2_hi
   DC.W  companion_neg_pose3_hi

pos_walk_next:
   DS.B  1

neg_walk_next:
   DS.B  1
