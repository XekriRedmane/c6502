PLAYER_COL	EQU	$00
PLAYER_ROW	EQU	$01
PLAYER_X_ADJ	EQU	$02
PLAYER_Y_ADJ	EQU	$03
PLAYER_ANIM_STATE	EQU	$04
PLAYER_FACING_DIRECTION	EQU	$05
HARGS	EQU	$06
SSP	EQU	$07
FP	EQU	$09
DPTR	EQU	$0B
map_row@ret	EQU	$0D
bg_row@ret	EQU	$0F
GUARD_LOC_COL	EQU	$12
GUARD_LOC_ROW	EQU	$13
GUARD_ANIM_STATE	EQU	$14
GUARD_FACING_DIRECTION	EQU	$15
GUARD_GOLD_TIMER	EQU	$16
GUARD_X_ADJ	EQU	$17
GUARD_Y_ADJ	EQU	$18
GUARD_NUM	EQU	$19
__dos_zp	EQU	$26
GUARD_RESURRECT_COL	EQU	$53
NOTE_INDEX	EQU	$54
GUARD_GOLD_TIMER_START_VALUE	EQU	$5F
GUARD_PATTERNS	EQU	$60
GUARD_PHASE	EQU	$64
PADDLE_VALUES	EQU	$65
__zpabi_draw_sprite_at_pixel_coords__row	EQU	$80
__zpabi_erase_sprite_at_pixel_coords__row	EQU	$80
__zpabi_play_note__y	EQU	$80
__local_move_player__note_y	EQU	$81
GAME_COLNUM	EQU	$85
GAME_ROWNUM	EQU	$86
DRAW_PAGE	EQU	$87
DIGITS	EQU	$89
FRAME_PERIOD	EQU	$8C
GUARD_COUNT	EQU	$8D
SCORE	EQU	$8E
GOLD_COUNT	EQU	$93
DIDNT_PICK_UP_GOLD	EQU	$94
INPUT_MODE	EQU	$95
DISK_LEVEL_LOC	EQU	$96
GUARD_PATTERN_OFFSET	EQU	$97
LIVES	EQU	$98
ENABLE_SOUND	EQU	$99
ALIVE	EQU	$9A
NOT_FALLING	EQU	$9B
DIG_DIRECTION	EQU	$9C
HI_SCORE_ELIGIBLE	EQU	$9D
KEY_COMMAND	EQU	$9E
KEY_COMMAND_LR	EQU	$9F
DIG_ANIM_STATE	EQU	$A0
VERBATIM	EQU	$A2
LADDER_COUNT	EQU	$A3
FALL_PITCH	EQU	$A4
WIPE_MODE	EQU	$A5
LEVELNUM	EQU	$A6
GAME_MODE	EQU	$A7
DEMO_PTR	EQU	$A8
DEMO_MOVE	EQU	$AA
DEMO_MOVE_TURNS	EQU	$AB
DEMO_RUNNING	EQU	$AC
LEVEL_SAVED	EQU	$AD
__local_keep_digging__0	EQU	$AE
__local_try_moving_left__0	EQU	$AE
__local_try_moving_right__0	EQU	$AE
__local_keep_digging__1	EQU	$AF
__local_try_moving_left__1	EQU	$AF
__local_try_moving_right__1	EQU	$AF
__local_keep_digging__state	EQU	$B0
__local_run_or_swing__0	EQU	$B0
__local_keep_digging__2	EQU	$B1
__local_keep_digging__3	EQU	$B2
__local_leave_cell__2	EQU	$B2
__local_run_or_swing__first	EQU	$B2
__local_keep_digging__col	EQU	$B3
__local_keep_digging__row	EQU	$B4
__local_keep_digging__4	EQU	$B5
__local_keep_digging__5	EQU	$B6
__local_keep_digging__6	EQU	$B7
__local_draw_player__0	EQU	$B9
__local_erase_player__0	EQU	$B9
__local_draw_player__1	EQU	$BA
__local_erase_player__1	EQU	$BA
__local_check_for_gold_picked_up_by_player__2	EQU	$BB
__local_drop_player_in_hole__col	EQU	$BB
__local_check_for_gold_picked_up_by_player__3	EQU	$BC
BLOCK_DATA	EQU	$DF
DOS_CURRENT_TRACK	EQU	$0478
LEVEL_MAP	EQU	$0800
LEVEL_BG	EQU	$0A00
LADDER_LOCS_COL	EQU	$0C00
LADDER_LOCS_ROW	EQU	$0C30
GUARD_LOCS_COL	EQU	$0C60
GUARD_LOCS_ROW	EQU	$0C68
GUARD_GOLD_TIMERS	EQU	$0C70
GUARD_X_ADJS	EQU	$0C78
GUARD_Y_ADJS	EQU	$0C80
GUARD_ANIM_STATES	EQU	$0C88
GUARD_FACING_DIRECTIONS	EQU	$0C90
GUARD_RESURRECTION_TIMERS	EQU	$0C98
BRICK_DIG_COLS	EQU	$0CA0
BRICK_DIG_ROWS	EQU	$0CC0
BRICK_FILL_TIMERS	EQU	$0CE0
DISK_BUFFER	EQU	$0D00
SOUND_DURATION	EQU	$0E00
SOUND_PITCH	EQU	$0E80
SPLASH_SCREEN	EQU	$0F00
ROW_TO_OFFSET_LO	EQU	$1A85
ROW_TO_OFFSET_HI	EQU	$1B45
HALF_SCREEN_COL_TABLE	EQU	$1C35
SCREEN_ROW_TABLE	EQU	$1C51
COL_BYTE_TABLE	EQU	$1C62
COL_SHIFT_TABLE	EQU	$1C7E
HALF_SCREEN_COL_BYTE_TABLE	EQU	$1C9A
HALF_SCREEN_COL_SHIFT_TABLE	EQU	$1D26
DISK_BOOT_SECTOR_DATA	EQU	$1DB2
HI_SCORES	EQU	$1F00
HI_SCORE_MARKER	EQU	$1FF4
HI_SCORE_MASTER_FLAG	EQU	$1FFF
HGR1	EQU	$2000
HGR2	EQU	$4000
DEMO_MOVES	EQU	$9B00
DEMO_LEVELS	EQU	$9E00
PIXEL_SHIFT_TABLE	EQU	$A200
SPRITE_DATA	EQU	$AD00
RWTS	EQU	$B7B5
DOS_IOB	EQU	$B7E8
SEEKABS	EQU	$B9A0
DSKFORM	EQU	$BEAF
KBD	EQU	$C000
KBDSTRB	EQU	$C010
SPKR	EQU	$C030
TXTCLR	EQU	$C050
MIXCLR	EQU	$C052
TXTPAGE1	EQU	$C054
TXTPAGE2	EQU	$C055
HIRES	EQU	$C057
BUTN0	EQU	$C061
BUTN1	EQU	$C062
PADDL	EQU	$C064
PTRIG	EQU	$C070
ROMIN_RDROM_WRRAM2	EQU	$C081
DISK_IO	EQU	$C088

; @zp-link-meta-begin
; def bg_row params=__zpabi_bg_row__row locals= indirect=false in_cycle=false param_regs=A
; def blocks_player params=__zpabi_blocks_player__sprite locals=__local_blocks_player__sprite indirect=false in_cycle=false param_regs=A carry=true
; def check_for_gold_picked_up_by_player params= locals=__local_check_for_gold_picked_up_by_player__0,__local_check_for_gold_picked_up_by_player__1,__local_check_for_gold_picked_up_by_player__2,__local_check_for_gold_picked_up_by_player__3 indirect=false in_cycle=false
; def climb params= locals= indirect=false in_cycle=false carry=true
; def draw_player params= locals=__local_draw_player__0,__local_draw_player__1,__local_draw_player__2 indirect=false in_cycle=false
; def drop_player_in_hole params=__zpabi_drop_player_in_hole__col locals=__local_drop_player_in_hole__0,__local_drop_player_in_hole__1,__local_drop_player_in_hole__col indirect=false in_cycle=false param_regs=A
; def enter_cell params= locals=__local_enter_cell__0,__local_enter_cell__1 indirect=false in_cycle=false
; def erase_player params= locals=__local_erase_player__0,__local_erase_player__1 indirect=false in_cycle=false
; def fall params= locals=__local_fall__0 indirect=false in_cycle=false
; def inc_anim_state params=__zpabi_inc_anim_state__first,__zpabi_inc_anim_state__last locals=__local_inc_anim_state__last,__local_inc_anim_state__first indirect=false in_cycle=false param_regs=A,X
; def keep_digging params= locals=__local_keep_digging__0,__local_keep_digging__1,__local_keep_digging__state,__local_keep_digging__2,__local_keep_digging__3,__local_keep_digging__col,__local_keep_digging__row,__local_keep_digging__4,__local_keep_digging__5,__local_keep_digging__6,__local_keep_digging__7 indirect=false in_cycle=false carry=true
; def leave_cell params= locals=__local_leave_cell__0,__local_leave_cell__1,__local_leave_cell__2 indirect=false in_cycle=false
; def map_row params=__zpabi_map_row__row locals= indirect=false in_cycle=false param_regs=A
; def move_player params= locals=__local_move_player__note_y,__local_move_player__0,__local_move_player__1 indirect=false in_cycle=false
; def nudge_player_towards_exact_column params= locals= indirect=false in_cycle=false
; def nudge_player_towards_exact_row params= locals= indirect=false in_cycle=false
; def run_or_swing params=__zpabi_run_or_swing__first locals=__local_run_or_swing__0,__local_run_or_swing__1,__local_run_or_swing__first indirect=false in_cycle=false param_regs=A
; def stands_on_something params= locals=__local_stands_on_something__0,__local_stands_on_something__1 indirect=false in_cycle=false carry=true
; def start_digging params=__zpabi_start_digging__direction locals=__local_start_digging__direction indirect=false in_cycle=false param_regs=A carry=true
; def step_down params= locals= indirect=false in_cycle=false
; def stop_digging params= locals= indirect=false in_cycle=false carry=true
; def try_moving_down params= locals=__local_try_moving_down__0,__local_try_moving_down__1 indirect=false in_cycle=false carry=true
; def try_moving_left params= locals=__local_try_moving_left__0,__local_try_moving_left__1 indirect=false in_cycle=false
; def try_moving_right params= locals=__local_try_moving_right__0,__local_try_moving_right__1 indirect=false in_cycle=false
; def try_moving_up params= locals=__local_try_moving_up__0,__local_try_moving_up__1 indirect=false in_cycle=false carry=true
; ext DSKFORM params=
; ext RWTS params=__zpabi_RWTS__iob_0,__zpabi_RWTS__iob_1 param_regs=Y,A carry=true
; ext SEEKABS params=__zpabi_SEEKABS__track param_regs=A
; ext access_compressed_level_data params=__zpabi_access_compressed_level_data__command param_regs=A
; ext access_disk_or_reset_game params=
; ext access_hi_score_data_from_disk params=__zpabi_access_hi_score_data_from_disk__command param_regs=A
; ext add_and_update_score params=__zpabi_add_and_update_score__bcd_points_0,__zpabi_add_and_update_score__bcd_points_1 param_regs=A,X
; ext append_note params=__zpabi_append_note__pitch,__zpabi_append_note__duration param_regs=A,X
; ext bad_data_disk params=
; ext bcd_to_decimal2 params=__zpabi_bcd_to_decimal2__bcd param_regs=A
; ext beep params=
; ext char_to_sprite_num params=__zpabi_char_to_sprite_num__c param_regs=A
; ext check_for_input params=
; ext clear_hgr1 params=
; ext clear_hgr2 params=
; ext cold_start params=
; ext compress_and_save_level_data params=
; ext detect_lack_of_joystick params=
; ext draw_guard_sprite params=__zpabi_draw_guard_sprite__sprite param_regs=A
; ext draw_sprite_at_pixel_coords params=__zpabi_draw_sprite_at_pixel_coords__sprite,__zpabi_draw_sprite_at_pixel_coords__half_col,__zpabi_draw_sprite_at_pixel_coords__row param_regs=A,X,
; ext draw_sprite_page1 params=__zpabi_draw_sprite_page1__sprite param_regs=A
; ext draw_sprite_page2 params=__zpabi_draw_sprite_page2__sprite param_regs=A
; ext editor_command_loop params=
; ext enable_next_level_ladders params=
; ext erase_guard params=
; ext erase_sprite_at_pixel_coords params=__zpabi_erase_sprite_at_pixel_coords__sprite,__zpabi_erase_sprite_at_pixel_coords__half_col,__zpabi_erase_sprite_at_pixel_coords__row param_regs=A,X,
; ext half_screen_col_for params=__zpabi_half_screen_col_for__col,__zpabi_half_screen_col_for__x_adj param_regs=A,X
; ext handle_timers params=
; ext hi_score_screen params=
; ext init_game_data params=
; ext iris_wipe params=
; ext level_editor params=
; ext load_guard_data params=
; ext load_level params=__zpabi_load_level__verbatim param_regs=A
; ext load_sound_data params=__zpabi_load_sound_data__notes_0,__zpabi_load_sound_data__notes_1 param_regs=A,X
; ext long_delay params=
; ext move_guards params=
; ext newline params=
; ext play_note params=__zpabi_play_note__pitch,__zpabi_play_note__duration,__zpabi_play_note__y param_regs=A,X,
; ext play_sound params= carry=true
; ext put_char params=__zpabi_put_char__c param_regs=A
; ext put_digit params=__zpabi_put_digit__digit param_regs=A
; ext put_status params=
; ext put_status_draw params=
; ext put_status_level params=
; ext put_status_lives params=
; ext put_string params=__zpabi_put_string__s_0,__zpabi_put_string__s_1 param_regs=A,X
; ext record_hi_score_data_to_disk params=
; ext reset_game params=
; ext row_to_addr params=__zpabi_row_to_addr__row,__zpabi_row_to_addr__page param_regs=A,X
; ext screen_row_for params=__zpabi_screen_row_for__row,__zpabi_screen_row_for__y_adj param_regs=A,X
; ext sound_delay params=__zpabi_sound_delay__index param_regs=A
; ext sound_delay1 params=__zpabi_sound_delay1__count param_regs=A
; ext spinning_game_over params= carry=true
; ext start_level_editor params=
; ext to_decimal3 params=__zpabi_to_decimal3__n param_regs=A
; ext wait_for_key params=__zpabi_wait_for_key__cursor param_regs=A
; ext wait_for_key_with_cursor_page_1 params=__zpabi_wait_for_key_with_cursor_page_1__cursor param_regs=A
; ext wait_key params=
; call check_for_gold_picked_up_by_player -> add_and_update_score
; call check_for_gold_picked_up_by_player -> draw_sprite_page2
; call check_for_gold_picked_up_by_player -> erase_sprite_at_pixel_coords
; call check_for_gold_picked_up_by_player -> load_sound_data
; call climb -> draw_player
; call climb -> inc_anim_state
; call draw_player -> draw_sprite_at_pixel_coords
; call draw_player -> half_screen_col_for
; call draw_player -> screen_row_for
; call drop_player_in_hole -> draw_sprite_page1
; call drop_player_in_hole -> draw_sprite_page2
; call drop_player_in_hole -> map_row
; call enter_cell -> map_row
; call erase_player -> erase_sprite_at_pixel_coords
; call erase_player -> half_screen_col_for
; call erase_player -> screen_row_for
; call fall -> draw_player
; call fall -> erase_player
; call fall -> nudge_player_towards_exact_column
; call fall -> step_down
; call keep_digging -> append_note
; call keep_digging -> draw_player
; call keep_digging -> draw_sprite_at_pixel_coords
; call keep_digging -> draw_sprite_page1
; call keep_digging -> drop_player_in_hole
; call keep_digging -> erase_player
; call keep_digging -> erase_sprite_at_pixel_coords
; call keep_digging -> map_row
; call keep_digging -> nudge_player_towards_exact_column
; call keep_digging -> nudge_player_towards_exact_row
; call keep_digging -> stop_digging
; call leave_cell -> bg_row
; call leave_cell -> map_row
; call move_player -> bg_row
; call move_player -> check_for_input
; call move_player -> fall
; call move_player -> keep_digging
; call move_player -> play_note
; call move_player -> stands_on_something
; call move_player -> start_digging
; call move_player -> try_moving_down
; call move_player -> try_moving_left
; call move_player -> try_moving_right
; call move_player -> try_moving_up
; call nudge_player_towards_exact_column -> check_for_gold_picked_up_by_player
; call nudge_player_towards_exact_row -> check_for_gold_picked_up_by_player
; call run_or_swing -> bg_row
; call run_or_swing -> draw_player
; call run_or_swing -> inc_anim_state
; call stands_on_something -> bg_row
; call stands_on_something -> map_row
; call start_digging -> keep_digging
; call step_down -> check_for_gold_picked_up_by_player
; call step_down -> enter_cell
; call step_down -> leave_cell
; call try_moving_down -> climb
; call try_moving_down -> erase_player
; call try_moving_down -> map_row
; call try_moving_down -> nudge_player_towards_exact_column
; call try_moving_down -> step_down
; call try_moving_left -> blocks_player
; call try_moving_left -> check_for_gold_picked_up_by_player
; call try_moving_left -> enter_cell
; call try_moving_left -> erase_player
; call try_moving_left -> leave_cell
; call try_moving_left -> nudge_player_towards_exact_row
; call try_moving_left -> run_or_swing
; call try_moving_right -> blocks_player
; call try_moving_right -> check_for_gold_picked_up_by_player
; call try_moving_right -> enter_cell
; call try_moving_right -> erase_player
; call try_moving_right -> leave_cell
; call try_moving_right -> nudge_player_towards_exact_row
; call try_moving_right -> run_or_swing
; call try_moving_up -> bg_row
; call try_moving_up -> blocks_player
; call try_moving_up -> check_for_gold_picked_up_by_player
; call try_moving_up -> climb
; call try_moving_up -> enter_cell
; call try_moving_up -> erase_player
; call try_moving_up -> leave_cell
; call try_moving_up -> map_row
; call try_moving_up -> nudge_player_towards_exact_column
; item map_row section=.text local=true zero=false
; item bg_row section=.text local=true zero=false
; item erase_player section=.text local=true zero=false
; item draw_player section=.text local=true zero=false
; item check_for_gold_picked_up_by_player section=.text local=true zero=false
; item inc_anim_state section=.text local=true zero=false
; item nudge_player_towards_exact_column section=.text local=true zero=false
; item nudge_player_towards_exact_row section=.text local=true zero=false
; item leave_cell section=.text local=true zero=false
; item enter_cell section=.text local=true zero=false
; item blocks_player section=.text local=true zero=false
; item step_down section=.text local=true zero=false
; item run_or_swing section=.text local=true zero=false
; item try_moving_left section=.text local=true zero=false
; item try_moving_right section=.text local=true zero=false
; item climb section=.text local=true zero=false
; item try_moving_up section=.text local=true zero=false
; item try_moving_up@run1 section=.text local=true zero=false
; item try_moving_up@run0 section=.text local=true zero=false
; item try_moving_down section=.text local=true zero=false
; item drop_player_in_hole section=.text local=true zero=false
; item stop_digging section=.text local=true zero=false
; item keep_digging section=.text local=true zero=false
; item start_digging section=.text local=true zero=false
; item fall section=.text local=true zero=false
; item stands_on_something section=.text local=true zero=false
; item move_player section=.text local=false zero=false
; item SPRITE_ANIM_SEQS section=.rodata local=true zero=false
; item dig_tables section=.rodata local=true zero=false
; item gold_tune section=.rodata local=true zero=false
; item LEVEL_BG@lo section=.rodata local=true zero=false merge=true
; item LEVEL_BG@hi section=.rodata local=true zero=false merge=true
; item LEVEL_MAP@hi section=.rodata local=true zero=false merge=true
; zp PLAYER_COL size=1 at=$00 def=false init=
; zp PLAYER_ROW size=1 at=$01 def=false init=
; zp PLAYER_X_ADJ size=1 at=$02 def=false init=
; zp PLAYER_Y_ADJ size=1 at=$03 def=false init=
; zp PLAYER_ANIM_STATE size=1 at=$04 def=false init=
; zp PLAYER_FACING_DIRECTION size=1 at=$05 def=false init=
; zp map_row@ret size=2 at=auto def=true init= local=true
; zp bg_row@ret size=2 at=auto def=true init= local=true
; zp GUARD_LOC_COL size=1 at=$12 def=false init=
; zp GUARD_LOC_ROW size=1 at=$13 def=false init=
; zp GUARD_ANIM_STATE size=1 at=$14 def=false init=
; zp GUARD_FACING_DIRECTION size=1 at=$15 def=false init=
; zp GUARD_GOLD_TIMER size=1 at=$16 def=false init=
; zp GUARD_X_ADJ size=1 at=$17 def=false init=
; zp GUARD_Y_ADJ size=1 at=$18 def=false init=
; zp GUARD_NUM size=1 at=$19 def=false init=
; zp __dos_zp size=42 at=$26 def=false init=
; zp GUARD_RESURRECT_COL size=1 at=$53 def=false init=
; zp NOTE_INDEX size=1 at=$54 def=false init=
; zp GUARD_GOLD_TIMER_START_VALUE size=1 at=$5F def=false init=
; zp GUARD_PATTERNS size=3 at=$60 def=false init=
; zp GUARD_PHASE size=1 at=$64 def=false init=
; zp PADDLE_VALUES size=2 at=$65 def=false init=
; zp GAME_COLNUM size=1 at=$85 def=false init=
; zp GAME_ROWNUM size=1 at=$86 def=false init=
; zp DRAW_PAGE size=1 at=$87 def=false init=
; zp DIGITS size=3 at=$89 def=false init=
; zp FRAME_PERIOD size=1 at=$8C def=false init=
; zp GUARD_COUNT size=1 at=$8D def=false init=
; zp SCORE size=4 at=$8E def=false init=
; zp GOLD_COUNT size=1 at=$93 def=false init=
; zp DIDNT_PICK_UP_GOLD size=1 at=$94 def=false init=
; zp INPUT_MODE size=1 at=$95 def=false init=
; zp DISK_LEVEL_LOC size=1 at=$96 def=false init=
; zp GUARD_PATTERN_OFFSET size=1 at=$97 def=false init=
; zp LIVES size=1 at=$98 def=false init=
; zp ENABLE_SOUND size=1 at=$99 def=false init=
; zp ALIVE size=1 at=$9A def=false init=
; zp NOT_FALLING size=1 at=$9B def=false init=
; zp DIG_DIRECTION size=1 at=$9C def=false init=
; zp HI_SCORE_ELIGIBLE size=1 at=$9D def=false init=
; zp KEY_COMMAND size=1 at=$9E def=false init=
; zp KEY_COMMAND_LR size=1 at=$9F def=false init=
; zp DIG_ANIM_STATE size=1 at=$A0 def=false init=
; zp VERBATIM size=1 at=$A2 def=false init=
; zp LADDER_COUNT size=1 at=$A3 def=false init=
; zp FALL_PITCH size=1 at=$A4 def=false init=
; zp WIPE_MODE size=1 at=$A5 def=false init=
; zp LEVELNUM size=1 at=$A6 def=false init=
; zp GAME_MODE size=1 at=$A7 def=false init=
; zp DEMO_PTR size=2 at=$A8 def=false init=
; zp DEMO_MOVE size=1 at=$AA def=false init=
; zp DEMO_MOVE_TURNS size=1 at=$AB def=false init=
; zp DEMO_RUNNING size=1 at=$AC def=false init=
; zp LEVEL_SAVED size=1 at=$AD def=false init=
; zp BLOCK_DATA size=33 at=$DF def=false init=
; runtime hargs_size=1 hargs_need=0
; @zp-link-meta-end

map_row:
   SUBROUTINE

   TAX
   LDA   LEVEL_BG@lo,X
   STA   map_row@ret
   LDA   LEVEL_MAP@hi,X
   STA   map_row@ret+1
   RTS

bg_row:
   SUBROUTINE

   TAX
   LDA   LEVEL_BG@lo,X
   STA   bg_row@ret
   LDA   LEVEL_BG@hi,X
   STA   bg_row@ret+1
   RTS

erase_player:
   SUBROUTINE

   LDX   PLAYER_ANIM_STATE
   LDA   SPRITE_ANIM_SEQS,X
   STA   __local_erase_player__1
   LDX   PLAYER_X_ADJ
   LDA   PLAYER_COL
   JSR   half_screen_col_for
   STA   __local_erase_player__0
   LDX   PLAYER_Y_ADJ
   LDA   PLAYER_ROW
   JSR   screen_row_for
   STA   __zpabi_erase_sprite_at_pixel_coords__row
   LDX   __local_erase_player__0
   LDA   __local_erase_player__1
   JMP   erase_sprite_at_pixel_coords

draw_player:
   SUBROUTINE

   LDX   PLAYER_ANIM_STATE
   LDA   SPRITE_ANIM_SEQS,X
   STA   __local_draw_player__1
   LDX   PLAYER_X_ADJ
   LDA   PLAYER_COL
   JSR   half_screen_col_for
   STA   __local_draw_player__0
   LDX   PLAYER_Y_ADJ
   LDA   PLAYER_ROW
   JSR   screen_row_for
   STA   __zpabi_draw_sprite_at_pixel_coords__row
   LDX   __local_draw_player__0
   LDA   __local_draw_player__1
   JSR   draw_sprite_at_pixel_coords
   ORA   #$00
   BEQ   .if_end@2
   LDA   DIDNT_PICK_UP_GOLD
   BEQ   .if_end@2
   LSR   ALIVE
.if_end@2:
   RTS

check_for_gold_picked_up_by_player:
   SUBROUTINE

   LDA   PLAYER_X_ADJ
   CMP   #$02
   BNE   .check_for_gold_picked_up_by_player@scfold@0
   LDA   PLAYER_Y_ADJ
   CMP   #$02
   BEQ   .if_end@5
.check_for_gold_picked_up_by_player@scfold@0:
   RTS
.if_end@5:
   LDX   PLAYER_ROW
   LDA   LEVEL_BG@lo,X
   STA   __local_check_for_gold_picked_up_by_player__2
   LDA   LEVEL_BG@hi,X
   STA   __local_check_for_gold_picked_up_by_player__3
   LDY   PLAYER_COL
   LDA   (__local_check_for_gold_picked_up_by_player__2),Y
   CMP   #$07
   BEQ   .if_end@6
   RTS
.if_end@6:
   LSR   DIDNT_PICK_UP_GOLD
   DEC   GOLD_COUNT
   STX   GAME_ROWNUM
   STY   GAME_COLNUM
   LDA   #$00
   STA   (__local_check_for_gold_picked_up_by_player__2),Y
   JSR   draw_sprite_page2
   LDX   GAME_COLNUM
   LDY   HALF_SCREEN_COL_TABLE,X
   LDX   GAME_ROWNUM
   LDA   SCREEN_ROW_TABLE,X
   STA   __zpabi_erase_sprite_at_pixel_coords__row
   TYA
   TAX
   LDA   #$07
   JSR   erase_sprite_at_pixel_coords
   LDX   #$02
   LDA   #$50
   JSR   add_and_update_score
   LDX   #>gold_tune
   LDA   #<gold_tune
   JMP   load_sound_data

inc_anim_state:
   SUBROUTINE

   INC   PLAYER_ANIM_STATE
   CMP   PLAYER_ANIM_STATE
   BCS   .inc_anim_state@scfold@0
   CPX   PLAYER_ANIM_STATE
   BCS   .if_end@9
.inc_anim_state@scfold@0:
   STA   PLAYER_ANIM_STATE
.if_end@9:
   RTS

nudge_player_towards_exact_column:
   SUBROUTINE

   LDA   PLAYER_X_ADJ
   CMP   #$02
   BNE   .if_end@10
   RTS
.if_end@10:
   BCS   .if_else@12
   INC   PLAYER_X_ADJ
   JMP   check_for_gold_picked_up_by_player
.if_else@12:
   DEC   PLAYER_X_ADJ
.if_end@11:
   JMP   check_for_gold_picked_up_by_player

nudge_player_towards_exact_row:
   SUBROUTINE

   LDA   PLAYER_Y_ADJ
   CMP   #$02
   BNE   .if_end@13
   RTS
.if_end@13:
   BCS   .if_else@15
   INC   PLAYER_Y_ADJ
   JMP   check_for_gold_picked_up_by_player
.if_else@15:
   DEC   PLAYER_Y_ADJ
.if_end@14:
   JMP   check_for_gold_picked_up_by_player

leave_cell:
   SUBROUTINE

   LDA   PLAYER_ROW
   JSR   bg_row
   LDY   PLAYER_COL
   LDA   (bg_row@ret),Y
   STA   __local_leave_cell__2
   CMP   #$01
   BNE   .cond_else@16
   LDA   #$00
   STA   __local_leave_cell__2
.cond_else@16:
   LDA   PLAYER_ROW
   JSR   map_row
   LDA   __local_leave_cell__2
   LDY   PLAYER_COL
   STA   (map_row@ret),Y
   RTS

enter_cell:
   SUBROUTINE

   LDA   PLAYER_ROW
   JSR   map_row
   LDA   #$09
   LDY   PLAYER_COL
   STA   (map_row@ret),Y
   RTS

blocks_player:
   SUBROUTINE

   CMP   #$01
   BEQ   .or_true@18
   CMP   #$02
   BEQ   .or_true@18
   CMP   #$05
   BEQ   .or_true@18
   CLC
   RTS
.or_true@18:
   SEC
   RTS

step_down:
   SUBROUTINE

   INC   PLAYER_Y_ADJ
   LDA   PLAYER_Y_ADJ
   CMP   #$05
   BCS   .if_end@22
   JMP   check_for_gold_picked_up_by_player
.if_end@22:
   LDA   #$00
   STA   PLAYER_Y_ADJ
   JSR   leave_cell
   INC   PLAYER_ROW
   JMP   enter_cell

run_or_swing:
   SUBROUTINE

   STA   __local_run_or_swing__first
   LDA   PLAYER_ROW
   JSR   bg_row
   LDY   PLAYER_COL
   LDA   (bg_row@ret),Y
   CMP   #$04
   BNE   .if_else@24
   LDA   __local_run_or_swing__first
   CLC
   ADC   #$03
   STA   __local_run_or_swing__0
   LDA   __local_run_or_swing__first
   CLC
   ADC   #$05
   TAX
   LDA   __local_run_or_swing__0
   JSR   inc_anim_state
   JMP   draw_player
.if_else@24:
   LDX   __local_run_or_swing__first
   INX
   INX
   LDA   __local_run_or_swing__first
   JSR   inc_anim_state
.if_end@23:
   JMP   draw_player

try_moving_left:
   SUBROUTINE

   LDA   PLAYER_X_ADJ
   CMP   #$03
   BCS   .if_end@25
   LDA   PLAYER_COL
   BNE   .if_end@26
   RTS
.if_end@26:
   LDX   PLAYER_ROW
   LDA   LEVEL_BG@lo,X
   STA   __local_try_moving_left__0
   LDA   LEVEL_MAP@hi,X
   STA   __local_try_moving_left__1
   LDY   PLAYER_COL
   DEY
   LDA   (__local_try_moving_left__0),Y
   JSR   blocks_player
   BCC   .if_end@27
   RTS
.if_end@27:
.if_end@25:
   JSR   erase_player
   LDA   #$FF
   STA   PLAYER_FACING_DIRECTION
   JSR   nudge_player_towards_exact_row
   DEC   PLAYER_X_ADJ
   BMI   .if_else@29
   JSR   check_for_gold_picked_up_by_player
   JMP   .if_end@28
.if_else@29:
   JSR   leave_cell
   DEC   PLAYER_COL
   JSR   enter_cell
   LDA   #$04
   STA   PLAYER_X_ADJ
.if_end@28:
   LDA   #$00
   JMP   run_or_swing

try_moving_right:
   SUBROUTINE

   LDA   PLAYER_X_ADJ
   CMP   #$02
   BCC   .if_end@30
   LDA   PLAYER_COL
   CMP   #$1B
   BNE   .if_end@31
   RTS
.if_end@31:
   LDX   PLAYER_ROW
   LDA   LEVEL_BG@lo,X
   STA   __local_try_moving_right__0
   LDA   LEVEL_MAP@hi,X
   STA   __local_try_moving_right__1
   LDY   PLAYER_COL
   INY
   LDA   (__local_try_moving_right__0),Y
   JSR   blocks_player
   BCC   .if_end@32
   RTS
.if_end@32:
.if_end@30:
   JSR   erase_player
   LDA   #$01
   STA   PLAYER_FACING_DIRECTION
   JSR   nudge_player_towards_exact_row
   INC   PLAYER_X_ADJ
   LDA   PLAYER_X_ADJ
   CMP   #$05
   BCS   .if_else@34
   JSR   check_for_gold_picked_up_by_player
   JMP   .if_end@33
.if_else@34:
   JSR   leave_cell
   INC   PLAYER_COL
   JSR   enter_cell
   LDA   #$00
   STA   PLAYER_X_ADJ
.if_end@33:
   LDA   #$08
   JMP   run_or_swing

climb:
   SUBROUTINE

   LDX   #$11
   LDA   #$10
   JSR   inc_anim_state
   JSR   draw_player
   CLC
   RTS

try_moving_up:
   SUBROUTINE

   LDA   PLAYER_ROW
   JSR   bg_row
   JSR   try_moving_up@run1
   BNE   .if_else@36
   LDA   PLAYER_Y_ADJ
   CMP   #$03
   BCS   .if_end@35
   LDA   PLAYER_ROW
   BEQ   .tail@0
   SEC
   SBC   #$01
   JSR   map_row
   LDY   PLAYER_COL
   LDA   (map_row@ret),Y
   JSR   blocks_player
   BCC   .if_end@35
   RTS
.if_else@36:
   LDA   PLAYER_Y_ADJ
   CMP   #$03
   BCC   .tail@0
   LDA   PLAYER_ROW
   CLC
   ADC   #$01
   JSR   bg_row
   JSR   try_moving_up@run1
   BEQ   .if_end@41
.tail@0:
   SEC
   RTS
.if_end@41:
.if_end@35:
   JSR   try_moving_up@run0
   DEC   PLAYER_Y_ADJ
   BMI   .if_else@43
   JSR   check_for_gold_picked_up_by_player
   JMP   climb
.if_else@43:
   JSR   leave_cell
   DEC   PLAYER_ROW
   JSR   enter_cell
   LDA   #$04
   STA   PLAYER_Y_ADJ
.if_end@42:
   JMP   climb

try_moving_up@run1:
   SUBROUTINE

   LDY   PLAYER_COL
   LDA   (bg_row@ret),Y
   CMP   #$03
   RTS

try_moving_up@run0:
   SUBROUTINE

   JSR   erase_player
   JMP   nudge_player_towards_exact_column

try_moving_down:
   SUBROUTINE

   LDA   PLAYER_Y_ADJ
   CMP   #$02
   BCC   .if_end@44
   LDA   PLAYER_ROW
   CMP   #$0F
   BCS   .try_moving_down@scfold@0
   ADC   #$01
   JSR   map_row
   LDY   PLAYER_COL
   LDA   (map_row@ret),Y
   CMP   #$02
   BEQ   .try_moving_down@scfold@0
   CMP   #$01
   BNE   .if_end@48
.try_moving_down@scfold@0:
   SEC
   RTS
.if_end@48:
.if_end@44:
   JSR   try_moving_up@run0
   JSR   step_down
   JMP   climb

drop_player_in_hole:
   SUBROUTINE

   STA   __local_drop_player_in_hole__col
   LDA   #$00
   STA   DIG_DIRECTION
   LDA   PLAYER_ROW
   CLC
   ADC   #$01
   STA   GAME_ROWNUM
   LDX   __local_drop_player_in_hole__col
   STX   GAME_COLNUM
   JSR   map_row
   LDA   #$00
   LDY   __local_drop_player_in_hole__col
   STA   (map_row@ret),Y
   JSR   draw_sprite_page1
   LDA   #$00
   JSR   draw_sprite_page2
   DEC   GAME_ROWNUM
   LDA   #$00
   JSR   draw_sprite_page1
   INC   GAME_ROWNUM
   LDX   #$00
.drop_player_in_hole@rot@0:
   LDA   BRICK_FILL_TIMERS,X
   BNE   .if_end@49
   LDA   GAME_ROWNUM
   STA   BRICK_DIG_ROWS,X
   LDA   GAME_COLNUM
   STA   BRICK_DIG_COLS,X
   LDA   #$B4
   STA   BRICK_FILL_TIMERS,X
   RTS
.if_end@49:
   INX
   CPX   #$1E
   BNE   .drop_player_in_hole@rot@0
   RTS

stop_digging:
   SUBROUTINE

   LDA   #$00
   STA   DIG_DIRECTION
   SEC
   RTS

keep_digging:
   SUBROUTINE

   LDA   #$80
   CLC
   SBC   DIG_DIRECTION
   LDA   #$00
   ROL
   STA   __local_keep_digging__6
   BEQ   .cond_else@50
   LDA   #$0C
   STA   __local_keep_digging__5
   JMP   .cond_end@51
.cond_else@50:
   LDA   #$00
   STA   __local_keep_digging__5
.cond_end@51:
   CLC
   ADC   #$0C
   STA   __local_keep_digging__4
   LDA   PLAYER_ROW
   STA   __local_keep_digging__row
   CMP   #$0F
   BCC   .if_end@52
   JMP   stop_digging
.if_end@52:
   LDA   __local_keep_digging__6
   BEQ   .if_else@54
   LDA   PLAYER_COL
   CMP   #$1B
   BCC   .if_end@55
   JMP   stop_digging
.if_end@55:
   ADC   #$01
   STA   __local_keep_digging__col
   JMP   .if_end@53
.if_else@54:
   LDA   PLAYER_COL
   BNE   .if_end@56
   JMP   stop_digging
.if_end@56:
   SEC
   SBC   #$01
   STA   __local_keep_digging__col
.if_end@53:
   LDA   __local_keep_digging__row
   CLC
   ADC   #$01
   JSR   map_row
   LDY   __local_keep_digging__col
   LDA   (map_row@ret),Y
   CMP   #$01
   BEQ   .if_end@57
   JMP   stop_digging
.if_end@57:
   LDX   __local_keep_digging__col
   LDA   HALF_SCREEN_COL_TABLE,X
   STA   __local_keep_digging__3
   LDX   __local_keep_digging__row
   LDA   SCREEN_ROW_TABLE,X
   STA   __local_keep_digging__2
   TXA
   JSR   map_row
   LDY   __local_keep_digging__col
   LDA   (map_row@ret),Y
   BEQ   .if_end@58
   LDA   __local_keep_digging__row
   CLC
   ADC   #$01
   STA   GAME_ROWNUM
   STY   GAME_COLNUM
   LDA   #$01
   JSR   draw_sprite_page1
   LDA   DIG_ANIM_STATE
   CMP   __local_keep_digging__5
   BEQ   .if_end@59
   TAX
   DEX
   LDA   dig_tables,X
   STA   __local_keep_digging__0
   LDA   __local_keep_digging__2
   STA   __zpabi_erase_sprite_at_pixel_coords__row
   LDX   __local_keep_digging__3
   LDA   __local_keep_digging__0
   JSR   erase_sprite_at_pixel_coords
.if_end@59:
   JMP   stop_digging
.if_end@58:
   JSR   try_moving_up@run0
   JSR   nudge_player_towards_exact_row
   LDA   DIG_ANIM_STATE
   STA   __local_keep_digging__state
   SEC
   SBC   __local_keep_digging__5
   STA   __local_keep_digging__1
   LDX   __local_keep_digging__1
   LDA   dig_tables+36,X
   STA   __local_keep_digging__0
   LDY   dig_tables+49,X
   TYA
   TAX
   LDA   __local_keep_digging__0
   JSR   append_note
   LDA   __local_keep_digging__6
   BEQ   .if_else@61
   LDA   __local_keep_digging__state
   CMP   #$12
   BCS   .cond_else@62
   LDA   #$0E
   BNE   .cond_end@63
.cond_else@62:
   LDA   #$08
.cond_end@63:
   STA   PLAYER_ANIM_STATE
   JMP   .if_end@60
.if_else@61:
   LDA   __local_keep_digging__state
   CMP   #$06
   BCS   .cond_else@64
   LDA   #$06
   BNE   .cond_end@65
.cond_else@64:
   LDA   #$00
.cond_end@65:
   STA   PLAYER_ANIM_STATE
.if_end@60:
   JSR   draw_player
   LDA   __local_keep_digging__state
   CMP   __local_keep_digging__4
   BNE   .if_end@66
   LDA   __local_keep_digging__col
   JSR   drop_player_in_hole
   SEC
   RTS
.if_end@66:
   CMP   __local_keep_digging__5
   BEQ   .if_end@67
   TAX
   DEX
   LDA   dig_tables,X
   STA   __local_keep_digging__0
   LDA   __local_keep_digging__2
   STA   __zpabi_erase_sprite_at_pixel_coords__row
   LDX   __local_keep_digging__3
   LDA   __local_keep_digging__0
   JSR   erase_sprite_at_pixel_coords
.if_end@67:
   LDA   __local_keep_digging__col
   STA   GAME_COLNUM
   LDA   __local_keep_digging__row
   STA   GAME_ROWNUM
   LDX   __local_keep_digging__state
   LDA   dig_tables,X
   STA   __local_keep_digging__0
   LDA   __local_keep_digging__2
   STA   __zpabi_draw_sprite_at_pixel_coords__row
   LDX   __local_keep_digging__3
   LDA   __local_keep_digging__0
   JSR   draw_sprite_at_pixel_coords
   INC   GAME_ROWNUM
   LDX   __local_keep_digging__1
   LDA   dig_tables+24,X
   JSR   draw_sprite_page1
   INC   DIG_ANIM_STATE
   CLC
   RTS

start_digging:
   SUBROUTINE

   STA   DIG_DIRECTION
   STA   KEY_COMMAND
   STA   KEY_COMMAND_LR
   CMP   #$01
   BNE   .cond_else@68
   LDA   #$0C
   BNE   .cond_end@69
.cond_else@68:
   LDA   #$00
.cond_end@69:
   STA   DIG_ANIM_STATE
   JMP   keep_digging

fall:
   SUBROUTINE

   LDA   #$00
   STA   NOT_FALLING
   JSR   erase_player
   LDA   PLAYER_FACING_DIRECTION
   BPL   .cond_else@70
   LDA   #$07
   BNE   .cond_end@71
.cond_else@70:
   LDA   #$0F
.cond_end@71:
   STA   PLAYER_ANIM_STATE
   JSR   nudge_player_towards_exact_column
   JSR   step_down
   JMP   draw_player

stands_on_something:
   SUBROUTINE

   LDA   PLAYER_ROW
   CMP   #$0F
   BEQ   .or_true@75
   CLC
   ADC   #$01
   JSR   map_row
   LDY   PLAYER_COL
   LDA   (map_row@ret),Y
   BEQ   .tail@1
   CMP   #$08
   BEQ   .or_true@75
   LDA   PLAYER_ROW
   CLC
   ADC   #$01
   JSR   bg_row
   LDY   PLAYER_COL
   LDA   (bg_row@ret),Y
   CMP   #$01
   BEQ   .or_true@75
   CMP   #$02
   BEQ   .or_true@75
   CMP   #$03
   BEQ   .or_true@75
.tail@1:
   CLC
   RTS
.or_true@75:
   SEC
   RTS

move_player:
   SUBROUTINE

   LDA   #$01
   STA   DIDNT_PICK_UP_GOLD
   LDA   DIG_DIRECTION
   BEQ   .if_end@79
   JMP   keep_digging
.if_end@79:
   LDA   PLAYER_COL
   STA   __local_move_player__note_y
   LDA   PLAYER_ROW
   JSR   bg_row
   JSR   try_moving_up@run1
   BEQ   .if_end@84
   CMP   #$04
   BNE   .move_player@scfold@0
   LDA   PLAYER_Y_ADJ
   CMP   #$02
   BEQ   .if_end@84
.move_player@scfold@0:
   LDA   PLAYER_Y_ADJ
   CMP   #$02
   BCC   .move_player@scfold@2
   JSR   stands_on_something
   BCS   .if_end@87
.move_player@scfold@2:
   JMP   fall
.if_end@87:
   LDA   PLAYER_ROW
   CMP   #$0F
   BNE   .if_end@88
   LDA   #$0F
   STA   __local_move_player__note_y
.if_end@88:
.if_end@84:
   LDA   NOT_FALLING
   BNE   .if_end@89
   LDA   __local_move_player__note_y
   STA   __zpabi_play_note__y
   LDX   #$08
   LDA   #$64
   JSR   play_note
.if_end@89:
   LDA   #$20
   STA   FALL_PITCH
   STA   NOT_FALLING
   JSR   check_for_input
   LDA   KEY_COMMAND
   CMP   #$C9
   BEQ   .case@2
   CMP   #$CB
   BEQ   .case@3
   CMP   #$D5
   BEQ   .case@4
   CMP   #$CF
   BEQ   .case@5
   JMP   .switch@1_break
.case@2:
   JSR   try_moving_up
   BCS   .switch@1_break
   RTS
.case@3:
   JSR   try_moving_down
   BCS   .switch@1_break
   RTS
.case@4:
   LDA   #$FF
   JSR   start_digging
   BCS   .switch@1_break
   RTS
.case@5:
   LDA   #$01
   JSR   start_digging
   BCS   .if_end@93
   RTS
.if_end@93:
.switch@1_break:
   LDA   KEY_COMMAND_LR
   CMP   #$CA
   BNE   .if_else@95
   JMP   try_moving_left
.if_else@95:
   CMP   #$CC
   BNE   .if_end@96
   JSR   try_moving_right
.if_end@96:
   RTS

SPRITE_ANIM_SEQS:
   DC.B  $0B, $0C, $0D, $18, $19, $1A, $0F, $13, $09, $10, $11, $15, $16, $17, $25, $14
   DC.B  $0E, $12

dig_tables:
   DC.B  $1B, $1B, $1C, $1C, $1D, $1D, $1E, $1E, $00, $00, $00, $00, $26, $26, $27, $27
   DC.B  $1D, $1D, $1E, $1E, $00, $00, $00, $00, $1F, $1F, $20, $20, $21, $21, $22, $22
   DC.B  $23, $23, $24, $24, $20, $20, $20, $20, $20, $20, $20, $20, $24, $24, $24, $24
   DC.B  $24, $04, $04, $04, $04, $04, $04, $04, $04, $03, $03, $02, $02, $01

gold_tune:
   DC.B  $07, $45, $06, $55, $05, $44, $04, $54, $03, $43, $02, $53, $00

LEVEL_BG@lo:
   DC.B  $00, $1C, $38, $54, $70, $8C, $A8, $C4, $E0, $FC, $18, $34, $50, $6C, $88, $A4
   DC.B  $C0

LEVEL_BG@hi:
   DC.B  $0A, $0A, $0A, $0A, $0A, $0A, $0A, $0A, $0A, $0A, $0B, $0B, $0B, $0B, $0B, $0B
   DC.B  $0B

LEVEL_MAP@hi:
   DC.B  $08, $08, $08, $08, $08, $08, $08, $08, $08, $08, $09, $09, $09, $09, $09, $09
   DC.B  $09
