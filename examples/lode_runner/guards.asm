PLAYER_COL	EQU	$00
PLAYER_ROW	EQU	$01
PLAYER_X_ADJ	EQU	$02
PLAYER_Y_ADJ	EQU	$03
PLAYER_ANIM_STATE	EQU	$04
PLAYER_FACING_DIRECTION	EQU	$05
HARGS	EQU	$06
SSP	EQU	$10
GUARD_LOC_COL	EQU	$12
GUARD_LOC_ROW	EQU	$13
GUARD_ANIM_STATE	EQU	$14
GUARD_FACING_DIRECTION	EQU	$15
GUARD_GOLD_TIMER	EQU	$16
GUARD_X_ADJ	EQU	$17
GUARD_Y_ADJ	EQU	$18
GUARD_NUM	EQU	$19
FP	EQU	$1A
DPTR	EQU	$1C
best_guard_dist	EQU	$1E
guard_action	EQU	$1F
__dos_zp	EQU	$26
GUARD_RESURRECT_COL	EQU	$53
NOTE_INDEX	EQU	$54
GUARD_GOLD_TIMER_START_VALUE	EQU	$5F
GUARD_PATTERNS	EQU	$60
GUARD_PHASE	EQU	$64
PADDLE_VALUES	EQU	$65
__local_guard_blocked_above__0	EQU	$80
__local_guard_is_supported__0	EQU	$80
__zpabi_consider_column__down	EQU	$80
__zpabi_draw_sprite_at_pixel_coords__row	EQU	$80
__zpabi_erase_sprite_at_pixel_coords__row	EQU	$80
__zpabi_play_note__y	EQU	$80
__local_guard_blocked_above__1	EQU	$81
__local_guard_is_supported__1	EQU	$81
__zpabi_consider_column__up	EQU	$81
__local_move_guards__pattern	EQU	$82
__local_move_guard__0	EQU	$84
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
__local_determine_guard_move__c	EQU	$AE
__local_guard_fall__0	EQU	$AE
__local_try_guard_move_down__0	EQU	$AE
__local_try_guard_move_left__0	EQU	$AE
__local_try_guard_move_right__0	EQU	$AE
__local_determine_guard_move__0	EQU	$AF
__local_guard_fall__1	EQU	$AF
__local_try_guard_move_down__1	EQU	$AF
__local_try_guard_move_left__1	EQU	$AF
__local_try_guard_move_right__1	EQU	$AF
__local_try_guard_move_up__col	EQU	$AF
__local_guard_fall_into_row_below__0	EQU	$B0
__local_try_guard_move_down__col	EQU	$B0
__local_guard_fall_into_row_below__1	EQU	$B1
__local_try_guard_move_left__row	EQU	$B1
__local_try_guard_move_right__row	EQU	$B1
__local_check_for_gold_picked_up_by_guard__0	EQU	$B2
__local_draw_guard_sprite__0	EQU	$B2
__local_erase_guard__0	EQU	$B2
__local_guard_fall_into_row_below__2	EQU	$B2
__local_inc_guard_anim_state__0	EQU	$B2
__local_check_for_gold_picked_up_by_guard__1	EQU	$B3
__local_consider_column__0	EQU	$B3
__local_draw_guard_sprite__sprite	EQU	$B3
__local_erase_guard__1	EQU	$B3
__local_guard_drop_gold__1	EQU	$B3
__local_guard_fall_into_row_below__col	EQU	$B3
__local_guard_in_hole__0	EQU	$B3
__local_guard_left_col_limit__col	EQU	$B3
__local_guard_right_col_limit__col	EQU	$B3
__local_check_for_gold_picked_up_by_guard__col	EQU	$B4
__local_consider_column__1	EQU	$B4
__local_enter_cell__0	EQU	$B4
__local_guard_drop_gold__2	EQU	$B4
__local_guard_left_col_limit__0	EQU	$B4
__local_guard_right_col_limit__0	EQU	$B4
__local_guard_run_at_player__col	EQU	$B4
__local_inc_guard_anim_state__first	EQU	$B4
__local_leave_cell__0	EQU	$B4
__local_check_for_gold_picked_up_by_guard__row	EQU	$B5
__local_consider_column__col	EQU	$B5
__local_enter_cell__1	EQU	$B5
__local_guard_left_col_limit__1	EQU	$B5
__local_guard_right_col_limit__1	EQU	$B5
__local_guard_run_at_player__row	EQU	$B5
__local_leave_cell__1	EQU	$B5
__local_consider_column__row	EQU	$B6
__local_enter_cell__row	EQU	$B6
__local_guard_can_cross__0	EQU	$B6
__local_guard_left_col_limit__2	EQU	$B6
__local_guard_right_col_limit__2	EQU	$B6
__local_leave_cell__2	EQU	$B6
__local_guard_can_cross__1	EQU	$B7
__local_guard_find_candidate_row_above__0	EQU	$B7
__local_guard_find_candidate_row_below__right_row	EQU	$B7
__local_guard_left_col_limit__3	EQU	$B7
__local_guard_right_col_limit__3	EQU	$B7
__local_leave_cell__row	EQU	$B7
__local_pseudo_distance__col	EQU	$B7
__local_guard_can_cross__row	EQU	$B8
__local_guard_find_candidate_row_above__1	EQU	$B8
__local_guard_find_candidate_row_below__0	EQU	$B8
__local_guard_left_col_limit__row	EQU	$B8
__local_guard_right_col_limit__row	EQU	$B8
__local_pseudo_distance__row	EQU	$B8
__local_guard_find_candidate_row_above__2	EQU	$B9
__local_guard_find_candidate_row_below__1	EQU	$B9
__local_guard_find_candidate_row_above__3	EQU	$BA
__local_guard_find_candidate_row_below__row	EQU	$BA
__local_guard_find_candidate_row_above__4	EQU	$BB
__local_guard_find_candidate_row_below__col	EQU	$BB
__local_guard_find_candidate_row_above__5	EQU	$BC
__local_guard_find_candidate_row_above__row	EQU	$BD
__local_guard_find_candidate_row_above__col	EQU	$BE
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
; def check_for_gold_picked_up_by_guard params= locals=__local_check_for_gold_picked_up_by_guard__0,__local_check_for_gold_picked_up_by_guard__1,__local_check_for_gold_picked_up_by_guard__col,__local_check_for_gold_picked_up_by_guard__row indirect=false in_cycle=false
; def consider_column params=__zpabi_consider_column__row,__zpabi_consider_column__col,__zpabi_consider_column__down,__zpabi_consider_column__up locals=__local_consider_column__0,__local_consider_column__1,__local_consider_column__col,__local_consider_column__row indirect=false in_cycle=false param_regs=A,X,,
; def consider_move params=__zpabi_consider_move__dist,__zpabi_consider_move__action locals=__local_consider_move__action,__local_consider_move__dist indirect=false in_cycle=false param_regs=A,X
; def determine_guard_move params= locals=__local_determine_guard_move__c,__local_determine_guard_move__0,__local_determine_guard_move__row,__local_determine_guard_move__col,__local_determine_guard_move__1 indirect=false in_cycle=false
; def draw_gold_at_game_cell params= locals=__local_draw_gold_at_game_cell__0 indirect=false in_cycle=false
; def draw_guard params= locals=__local_draw_guard__0 indirect=false in_cycle=false
; def draw_guard_sprite params=__zpabi_draw_guard_sprite__sprite locals=__local_draw_guard_sprite__0,__local_draw_guard_sprite__sprite indirect=false in_cycle=false param_regs=A
; def enter_cell params=__zpabi_enter_cell__row,__zpabi_enter_cell__col locals=__local_enter_cell__0,__local_enter_cell__1,__local_enter_cell__row indirect=false in_cycle=false param_regs=A,X
; def erase_guard params= locals=__local_erase_guard__0,__local_erase_guard__1 indirect=false in_cycle=false
; def guard_blocked_above params= locals=__local_guard_blocked_above__0,__local_guard_blocked_above__1 indirect=false in_cycle=false carry=true
; def guard_can_cross params=__zpabi_guard_can_cross__row,__zpabi_guard_can_cross__col locals=__local_guard_can_cross__0,__local_guard_can_cross__1,__local_guard_can_cross__row indirect=false in_cycle=false param_regs=A,X carry=true
; def guard_do_nothing params= locals=__local_guard_do_nothing__0 indirect=false in_cycle=false
; def guard_drop_gold params= locals=__local_guard_drop_gold__0,__local_guard_drop_gold__1,__local_guard_drop_gold__2,__local_guard_drop_gold__col indirect=false in_cycle=false
; def guard_fall params= locals=__local_guard_fall__0,__local_guard_fall__1 indirect=false in_cycle=false
; def guard_fall_into_row_below params= locals=__local_guard_fall_into_row_below__0,__local_guard_fall_into_row_below__1,__local_guard_fall_into_row_below__2,__local_guard_fall_into_row_below__col indirect=false in_cycle=false
; def guard_find_candidate_row_above params=__zpabi_guard_find_candidate_row_above__row,__zpabi_guard_find_candidate_row_above__col locals=__local_guard_find_candidate_row_above__0,__local_guard_find_candidate_row_above__1,__local_guard_find_candidate_row_above__2,__local_guard_find_candidate_row_above__3,__local_guard_find_candidate_row_above__4,__local_guard_find_candidate_row_above__5,__local_guard_find_candidate_row_above__row,__local_guard_find_candidate_row_above__col indirect=false in_cycle=false param_regs=A,X
; def guard_find_candidate_row_below params=__zpabi_guard_find_candidate_row_below__row,__zpabi_guard_find_candidate_row_below__col locals=__local_guard_find_candidate_row_below__right_row,__local_guard_find_candidate_row_below__0,__local_guard_find_candidate_row_below__1,__local_guard_find_candidate_row_below__row,__local_guard_find_candidate_row_below__col,__local_guard_find_candidate_row_below__2 indirect=false in_cycle=false param_regs=A,X
; def guard_half_col params= locals=__local_guard_half_col__0 indirect=false in_cycle=false
; def guard_in_hole params= locals=__local_guard_in_hole__0,__local_guard_in_hole__1 indirect=false in_cycle=false carry=true
; def guard_is_supported params= locals=__local_guard_is_supported__0,__local_guard_is_supported__1 indirect=false in_cycle=false carry=true
; def guard_left_col_limit params=__zpabi_guard_left_col_limit__row,__zpabi_guard_left_col_limit__col locals=__local_guard_left_col_limit__col,__local_guard_left_col_limit__0,__local_guard_left_col_limit__1,__local_guard_left_col_limit__2,__local_guard_left_col_limit__3,__local_guard_left_col_limit__row indirect=false in_cycle=false param_regs=A,X
; def guard_right_col_limit params=__zpabi_guard_right_col_limit__row,__zpabi_guard_right_col_limit__col locals=__local_guard_right_col_limit__col,__local_guard_right_col_limit__0,__local_guard_right_col_limit__1,__local_guard_right_col_limit__2,__local_guard_right_col_limit__3,__local_guard_right_col_limit__row indirect=false in_cycle=false param_regs=A,X
; def guard_run_at_player params=__zpabi_guard_run_at_player__row,__zpabi_guard_run_at_player__col locals=__local_guard_run_at_player__0,__local_guard_run_at_player__col,__local_guard_run_at_player__row indirect=false in_cycle=false param_regs=A,X
; def guard_screen_row params= locals=__local_guard_screen_row__0 indirect=false in_cycle=false
; def guard_take_move params= locals=__local_guard_take_move__0 indirect=false in_cycle=false
; def inc_guard_anim_state params=__zpabi_inc_guard_anim_state__first,__zpabi_inc_guard_anim_state__last locals=__local_inc_guard_anim_state__0,__local_inc_guard_anim_state__last,__local_inc_guard_anim_state__first indirect=false in_cycle=false param_regs=A,X
; def is_floor params=__zpabi_is_floor__sprite locals=__local_is_floor__sprite indirect=false in_cycle=false param_regs=A carry=true
; def leave_cell params=__zpabi_leave_cell__row,__zpabi_leave_cell__col locals=__local_leave_cell__0,__local_leave_cell__1,__local_leave_cell__2,__local_leave_cell__row indirect=false in_cycle=false param_regs=A,X
; def load_guard_data params= locals= indirect=false in_cycle=false
; def move_guard params= locals=__local_move_guard__0 indirect=false in_cycle=false
; def move_guards params= locals=__local_move_guards__pattern,__local_move_guards__0 indirect=false in_cycle=false
; def nudge_guard_towards_exact_column params= locals= indirect=false in_cycle=false
; def nudge_guard_towards_exact_row params= locals= indirect=false in_cycle=false
; def pseudo_distance params=__zpabi_pseudo_distance__row,__zpabi_pseudo_distance__col locals=__local_pseudo_distance__col,__local_pseudo_distance__row indirect=false in_cycle=false param_regs=A,X
; def store_guard_data params= locals= indirect=false in_cycle=false
; def try_guard_move_down params= locals=__local_try_guard_move_down__0,__local_try_guard_move_down__1,__local_try_guard_move_down__col indirect=false in_cycle=false
; def try_guard_move_left params= locals=__local_try_guard_move_left__0,__local_try_guard_move_left__1,__local_try_guard_move_left__col,__local_try_guard_move_left__row indirect=false in_cycle=false
; def try_guard_move_right params= locals=__local_try_guard_move_right__0,__local_try_guard_move_right__1,__local_try_guard_move_right__col,__local_try_guard_move_right__row indirect=false in_cycle=false
; def try_guard_move_up params= locals=__local_try_guard_move_up__0,__local_try_guard_move_up__col indirect=false in_cycle=false
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
; ext draw_sprite_at_pixel_coords params=__zpabi_draw_sprite_at_pixel_coords__sprite,__zpabi_draw_sprite_at_pixel_coords__half_col,__zpabi_draw_sprite_at_pixel_coords__row param_regs=A,X,
; ext draw_sprite_page1 params=__zpabi_draw_sprite_page1__sprite param_regs=A
; ext draw_sprite_page2 params=__zpabi_draw_sprite_page2__sprite param_regs=A
; ext editor_command_loop params=
; ext enable_next_level_ladders params=
; ext erase_sprite_at_pixel_coords params=__zpabi_erase_sprite_at_pixel_coords__sprite,__zpabi_erase_sprite_at_pixel_coords__half_col,__zpabi_erase_sprite_at_pixel_coords__row param_regs=A,X,
; ext half_screen_col_for params=__zpabi_half_screen_col_for__col,__zpabi_half_screen_col_for__x_adj param_regs=A,X
; ext handle_timers params=
; ext hi_score_screen params=
; ext init_game_data params=
; ext iris_wipe params=
; ext level_editor params=
; ext load_level params=__zpabi_load_level__verbatim param_regs=A
; ext load_sound_data params=__zpabi_load_sound_data__notes_0,__zpabi_load_sound_data__notes_1 param_regs=A,X
; ext long_delay params=
; ext move_player params=
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
; call check_for_gold_picked_up_by_guard -> draw_sprite_page2
; call check_for_gold_picked_up_by_guard -> erase_sprite_at_pixel_coords
; call consider_column -> consider_move
; call consider_column -> guard_find_candidate_row_above
; call consider_column -> guard_find_candidate_row_below
; call consider_column -> pseudo_distance
; call determine_guard_move -> consider_column
; call determine_guard_move -> guard_in_hole
; call determine_guard_move -> guard_left_col_limit
; call determine_guard_move -> guard_right_col_limit
; call determine_guard_move -> guard_run_at_player
; call draw_gold_at_game_cell -> draw_sprite_at_pixel_coords
; call draw_gold_at_game_cell -> draw_sprite_page2
; call draw_guard -> draw_guard_sprite
; call draw_guard_sprite -> draw_sprite_at_pixel_coords
; call draw_guard_sprite -> guard_half_col
; call draw_guard_sprite -> guard_screen_row
; call erase_guard -> erase_sprite_at_pixel_coords
; call erase_guard -> guard_half_col
; call erase_guard -> guard_screen_row
; call guard_do_nothing -> guard_in_hole
; call guard_drop_gold -> draw_gold_at_game_cell
; call guard_fall -> add_and_update_score
; call guard_fall -> check_for_gold_picked_up_by_guard
; call guard_fall -> draw_guard
; call guard_fall -> erase_guard
; call guard_fall -> guard_fall_into_row_below
; call guard_fall -> load_sound_data
; call guard_fall -> nudge_guard_towards_exact_column
; call guard_fall_into_row_below -> draw_gold_at_game_cell
; call guard_fall_into_row_below -> enter_cell
; call guard_fall_into_row_below -> leave_cell
; call guard_find_candidate_row_above -> is_floor
; call guard_find_candidate_row_below -> is_floor
; call guard_half_col -> half_screen_col_for
; call guard_is_supported -> is_floor
; call guard_left_col_limit -> is_floor
; call guard_right_col_limit -> is_floor
; call guard_run_at_player -> guard_can_cross
; call guard_screen_row -> screen_row_for
; call guard_take_move -> determine_guard_move
; call guard_take_move -> try_guard_move_down
; call guard_take_move -> try_guard_move_left
; call guard_take_move -> try_guard_move_right
; call guard_take_move -> try_guard_move_up
; call move_guard -> draw_guard
; call move_guard -> erase_guard
; call move_guard -> guard_fall
; call move_guard -> guard_in_hole
; call move_guard -> guard_is_supported
; call move_guard -> guard_take_move
; call move_guard -> load_guard_data
; call move_guard -> store_guard_data
; call move_guards -> move_guard
; call nudge_guard_towards_exact_column -> check_for_gold_picked_up_by_guard
; call nudge_guard_towards_exact_row -> check_for_gold_picked_up_by_guard
; call try_guard_move_down -> check_for_gold_picked_up_by_guard
; call try_guard_move_down -> draw_guard
; call try_guard_move_down -> enter_cell
; call try_guard_move_down -> erase_guard
; call try_guard_move_down -> guard_drop_gold
; call try_guard_move_down -> inc_guard_anim_state
; call try_guard_move_down -> leave_cell
; call try_guard_move_down -> nudge_guard_towards_exact_column
; call try_guard_move_left -> check_for_gold_picked_up_by_guard
; call try_guard_move_left -> draw_guard
; call try_guard_move_left -> enter_cell
; call try_guard_move_left -> erase_guard
; call try_guard_move_left -> guard_drop_gold
; call try_guard_move_left -> inc_guard_anim_state
; call try_guard_move_left -> leave_cell
; call try_guard_move_left -> nudge_guard_towards_exact_row
; call try_guard_move_right -> check_for_gold_picked_up_by_guard
; call try_guard_move_right -> draw_guard
; call try_guard_move_right -> enter_cell
; call try_guard_move_right -> erase_guard
; call try_guard_move_right -> guard_drop_gold
; call try_guard_move_right -> inc_guard_anim_state
; call try_guard_move_right -> leave_cell
; call try_guard_move_right -> nudge_guard_towards_exact_row
; call try_guard_move_up -> check_for_gold_picked_up_by_guard
; call try_guard_move_up -> draw_guard
; call try_guard_move_up -> enter_cell
; call try_guard_move_up -> erase_guard
; call try_guard_move_up -> guard_blocked_above
; call try_guard_move_up -> guard_do_nothing
; call try_guard_move_up -> guard_drop_gold
; call try_guard_move_up -> inc_guard_anim_state
; call try_guard_move_up -> leave_cell
; call try_guard_move_up -> nudge_guard_towards_exact_column
; item move_guards section=.text local=false zero=false
; item guard_in_hole section=.text local=true zero=false
; item store_guard_data section=.text local=true zero=false
; item load_guard_data section=.text local=false zero=false
; item guard_half_col section=.text local=true zero=false
; item guard_screen_row section=.text local=true zero=false
; item erase_guard section=.text local=false zero=false
; item draw_guard_sprite section=.text local=false zero=false
; item draw_guard section=.text local=true zero=false
; item draw_gold_at_game_cell section=.text local=true zero=false
; item check_for_gold_picked_up_by_guard section=.text local=true zero=false
; item guard_drop_gold section=.text local=true zero=false
; item inc_guard_anim_state section=.text local=true zero=false
; item nudge_guard_towards_exact_column section=.text local=true zero=false
; item nudge_guard_towards_exact_row section=.text local=true zero=false
; item leave_cell section=.text local=true zero=false
; item enter_cell section=.text local=true zero=false
; item is_floor section=.text local=true zero=false
; item guard_is_supported section=.text local=true zero=false
; item guard_fall_into_row_below section=.text local=true zero=false
; item guard_fall section=.text local=true zero=false
; item guard_fall@run0 section=.text local=true zero=false
; item guard_do_nothing section=.text local=true zero=false
; item guard_blocked_above section=.text local=true zero=false
; item try_guard_move_up section=.text local=true zero=false
; item try_guard_move_up@tail1 section=.text local=true zero=false
; item try_guard_move_up@tail0 section=.text local=true zero=false
; item try_guard_move_down section=.text local=true zero=false
; item try_guard_move_left section=.text local=true zero=false
; item try_guard_move_right section=.text local=true zero=false
; item guard_take_move section=.text local=true zero=false
; item move_guard section=.text local=true zero=false
; item pseudo_distance section=.text local=true zero=false
; item guard_find_candidate_row_above section=.text local=true zero=false
; item guard_find_candidate_row_below section=.text local=true zero=false
; item guard_left_col_limit section=.text local=true zero=false
; item guard_right_col_limit section=.text local=true zero=false
; item consider_move section=.text local=true zero=false
; item consider_column section=.text local=true zero=false
; item guard_can_cross section=.text local=true zero=false
; item guard_run_at_player section=.text local=true zero=false
; item determine_guard_move section=.text local=true zero=false
; item GUARD_PATTERNS_LIST section=.rodata local=false zero=false
; item GUARD_ANIM_SPRITES section=.rodata local=true zero=false
; item GUARD_X_ADJ_TABLE section=.rodata local=true zero=false
; item guard_in_hole_tune section=.rodata local=true zero=false
; item LEVEL_BG@lo section=.rodata local=true zero=false merge=true
; item LEVEL_BG@hi section=.rodata local=true zero=false merge=true
; item LEVEL_MAP@hi section=.rodata local=true zero=false merge=true
; item __zp_init section=.text local=false zero=false
; zp PLAYER_COL size=1 at=$00 def=false init=
; zp PLAYER_ROW size=1 at=$01 def=false init=
; zp PLAYER_X_ADJ size=1 at=$02 def=false init=
; zp PLAYER_Y_ADJ size=1 at=$03 def=false init=
; zp PLAYER_ANIM_STATE size=1 at=$04 def=false init=
; zp PLAYER_FACING_DIRECTION size=1 at=$05 def=false init=
; zp GUARD_LOC_COL size=1 at=$12 def=false init=
; zp GUARD_LOC_ROW size=1 at=$13 def=false init=
; zp GUARD_ANIM_STATE size=1 at=$14 def=false init=
; zp GUARD_FACING_DIRECTION size=1 at=$15 def=false init=
; zp GUARD_GOLD_TIMER size=1 at=$16 def=false init=
; zp GUARD_X_ADJ size=1 at=$17 def=false init=
; zp GUARD_Y_ADJ size=1 at=$18 def=false init=
; zp GUARD_NUM size=1 at=$19 def=false init=
; zp best_guard_dist size=1 at=auto def=true init=$00 weight=3 local=true
; zp guard_action size=1 at=auto def=true init=$00 weight=3 local=true
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
; runtime hargs_size=10 hargs_need=0
; @zp-link-meta-end

move_guards:
   SUBROUTINE

   LDA   GUARD_COUNT
   BNE   .if_end@0
   RTS
.if_end@0:
   INC   GUARD_PHASE
   LDA   GUARD_PHASE
   CMP   #$03
   BCC   .if_end@1
   LDA   #$00
   STA   GUARD_PHASE
.if_end@1:
   LDX   GUARD_PHASE
   LDA   GUARD_PATTERNS,X
   STA   __local_move_guards__pattern
.loop@0_start:
   LSR
   BCC   .if_end@2
   JSR   move_guard
   LDA   ALIVE
   BNE   .if_end@3
   RTS
.if_end@3:
.if_end@2:
   LSR   __local_move_guards__pattern
   LDA   __local_move_guards__pattern
   BNE   .loop@0_start
   RTS

guard_in_hole:
   SUBROUTINE

   LDA   GUARD_GOLD_TIMER
   BMI   .sx_neg@0
   LDA   #$00
   BEQ   .sx_done@1
.sx_neg@0:
   LDA   #$FF
.sx_done@1:
   STA   __local_guard_in_hole__0
   LDA   #$00
   SEC
   SBC   GUARD_GOLD_TIMER
   LDA   #$00
   SBC   __local_guard_in_hole__0
   BVC   .cmp_novf@2
   EOR   #$80
.cmp_novf@2:
   ASL
   RTS

store_guard_data:
   SUBROUTINE

   LDX   GUARD_NUM
   LDA   GUARD_LOC_COL
   STA   GUARD_LOCS_COL,X
   LDA   GUARD_LOC_ROW
   STA   GUARD_LOCS_ROW,X
   LDA   GUARD_X_ADJ
   STA   GUARD_X_ADJS,X
   LDA   GUARD_Y_ADJ
   STA   GUARD_Y_ADJS,X
   LDA   GUARD_GOLD_TIMER
   STA   GUARD_GOLD_TIMERS,X
   LDA   GUARD_FACING_DIRECTION
   STA   GUARD_FACING_DIRECTIONS,X
   LDA   GUARD_ANIM_STATE
   STA   GUARD_ANIM_STATES,X
   RTS

load_guard_data:
   SUBROUTINE

   LDX   GUARD_NUM
   LDA   GUARD_LOCS_COL,X
   STA   GUARD_LOC_COL
   LDA   GUARD_LOCS_ROW,X
   STA   GUARD_LOC_ROW
   LDA   GUARD_X_ADJS,X
   STA   GUARD_X_ADJ
   LDA   GUARD_Y_ADJS,X
   STA   GUARD_Y_ADJ
   LDA   GUARD_ANIM_STATES,X
   STA   GUARD_ANIM_STATE
   LDA   GUARD_FACING_DIRECTIONS,X
   STA   GUARD_FACING_DIRECTION
   LDA   GUARD_GOLD_TIMERS,X
   STA   GUARD_GOLD_TIMER
   RTS

guard_half_col:
   SUBROUTINE

   LDX   GUARD_X_ADJ
   LDA   GUARD_LOC_COL
   JMP   half_screen_col_for

guard_screen_row:
   SUBROUTINE

   LDX   GUARD_Y_ADJ
   LDA   GUARD_LOC_ROW
   JMP   screen_row_for

erase_guard:
   SUBROUTINE

   LDX   GUARD_ANIM_STATE
   LDA   GUARD_ANIM_SPRITES,X
   STA   __local_erase_guard__1
   JSR   guard_half_col
   STA   __local_erase_guard__0
   JSR   guard_screen_row
   STA   __zpabi_erase_sprite_at_pixel_coords__row
   LDX   __local_erase_guard__0
   LDA   __local_erase_guard__1
   JMP   erase_sprite_at_pixel_coords

draw_guard_sprite:
   SUBROUTINE

   STA   __local_draw_guard_sprite__sprite
   JSR   guard_half_col
   STA   __local_draw_guard_sprite__0
   JSR   guard_screen_row
   STA   __zpabi_draw_sprite_at_pixel_coords__row
   LDX   __local_draw_guard_sprite__0
   LDA   __local_draw_guard_sprite__sprite
   JMP   draw_sprite_at_pixel_coords

draw_guard:
   SUBROUTINE

   LDX   GUARD_ANIM_STATE
   LDA   GUARD_ANIM_SPRITES,X
   JMP   draw_guard_sprite

draw_gold_at_game_cell:
   SUBROUTINE

   LDA   #$07
   JSR   draw_sprite_page2
   LDX   GAME_COLNUM
   LDY   HALF_SCREEN_COL_TABLE,X
   LDX   GAME_ROWNUM
   LDA   SCREEN_ROW_TABLE,X
   STA   __zpabi_draw_sprite_at_pixel_coords__row
   TYA
   TAX
   LDA   #$07
   JMP   draw_sprite_at_pixel_coords

check_for_gold_picked_up_by_guard:
   SUBROUTINE

   LDA   GUARD_X_ADJ
   CMP   #$02
   BNE   .check_for_gold_picked_up_by_guard@scfold@0
   LDA   GUARD_Y_ADJ
   CMP   #$02
   BEQ   .if_end@6
.check_for_gold_picked_up_by_guard@scfold@0:
   RTS
.if_end@6:
   LDA   GUARD_LOC_ROW
   STA   __local_check_for_gold_picked_up_by_guard__row
   LDA   GUARD_LOC_COL
   STA   __local_check_for_gold_picked_up_by_guard__col
   LDX   GUARD_LOC_ROW
   LDA   LEVEL_BG@lo,X
   STA   __local_check_for_gold_picked_up_by_guard__0
   LDA   LEVEL_BG@hi,X
   STA   __local_check_for_gold_picked_up_by_guard__1
   LDY   GUARD_LOC_COL
   LDA   (__local_check_for_gold_picked_up_by_guard__0),Y
   CMP   #$07
   BNE   .check_for_gold_picked_up_by_guard@scfold@1
   LDA   GUARD_GOLD_TIMER
   BPL   .if_end@9
.check_for_gold_picked_up_by_guard@scfold@1:
   RTS
.if_end@9:
   LDA   #$FF
   SEC
   SBC   GUARD_RESURRECT_COL
   STA   GUARD_GOLD_TIMER
   LDA   LEVEL_BG@lo,X
   STA   __local_check_for_gold_picked_up_by_guard__0
   LDA   LEVEL_BG@hi,X
   STA   __local_check_for_gold_picked_up_by_guard__1
   LDA   #$00
   STA   (__local_check_for_gold_picked_up_by_guard__0),Y
   STX   GAME_ROWNUM
   STY   GAME_COLNUM
   JSR   draw_sprite_page2
   LDX   __local_check_for_gold_picked_up_by_guard__col
   LDY   HALF_SCREEN_COL_TABLE,X
   LDX   __local_check_for_gold_picked_up_by_guard__row
   LDA   SCREEN_ROW_TABLE,X
   STA   __zpabi_erase_sprite_at_pixel_coords__row
   TYA
   TAX
   LDA   #$07
   JMP   erase_sprite_at_pixel_coords

guard_drop_gold:
   SUBROUTINE

   LDA   GUARD_GOLD_TIMER
   BPL   .guard_drop_gold@scfold@0
   CLC
   ADC   #$01
   STA   GUARD_GOLD_TIMER
   BEQ   .if_end@12
.guard_drop_gold@scfold@0:
   RTS
.if_end@12:
   LDX   GUARD_LOC_ROW
   LDA   GUARD_LOC_COL
   STX   GAME_ROWNUM
   STA   GAME_COLNUM
   LDA   LEVEL_BG@lo,X
   STA   __local_guard_drop_gold__1
   LDA   LEVEL_BG@hi,X
   STA   __local_guard_drop_gold__2
   LDY   GUARD_LOC_COL
   LDA   (__local_guard_drop_gold__1),Y
   BNE   .if_else@14
   LDA   #$07
   STA   (__local_guard_drop_gold__1),Y
   JMP   draw_gold_at_game_cell
.if_else@14:
   DEC   GUARD_GOLD_TIMER
   RTS

inc_guard_anim_state:
   SUBROUTINE

   STA   __local_inc_guard_anim_state__first
   LDA   GUARD_ANIM_STATE
   CLC
   ADC   #$01
   STA   __local_inc_guard_anim_state__0
   STA   GUARD_ANIM_STATE
   LDA   __local_inc_guard_anim_state__first
   CMP   __local_inc_guard_anim_state__0
   BCS   .vthread@0
   CPX   __local_inc_guard_anim_state__0
   BCS   .if_end@17
.vthread@0:
   STA   GUARD_ANIM_STATE
.if_end@17:
   RTS

nudge_guard_towards_exact_column:
   SUBROUTINE

   LDA   GUARD_X_ADJ
   CMP   #$02
   BNE   .if_end@18
   RTS
.if_end@18:
   BCS   .if_else@20
   INC   GUARD_X_ADJ
   JMP   check_for_gold_picked_up_by_guard
.if_else@20:
   DEC   GUARD_X_ADJ
.if_end@19:
   JMP   check_for_gold_picked_up_by_guard

nudge_guard_towards_exact_row:
   SUBROUTINE

   LDA   GUARD_Y_ADJ
   CMP   #$02
   BNE   .if_end@21
   RTS
.if_end@21:
   BCS   .if_else@23
   INC   GUARD_Y_ADJ
   JMP   check_for_gold_picked_up_by_guard
.if_else@23:
   DEC   GUARD_Y_ADJ
.if_end@22:
   JMP   check_for_gold_picked_up_by_guard

leave_cell:
   SUBROUTINE

   STA   __local_leave_cell__row
   TXA
   TAY
   LDX   __local_leave_cell__row
   LDA   LEVEL_BG@lo,X
   STA   __local_leave_cell__0
   LDA   LEVEL_BG@hi,X
   STA   __local_leave_cell__1
   LDA   (__local_leave_cell__0),Y
   STA   __local_leave_cell__2
   CMP   #$01
   BNE   .cond_else@24
   LDA   #$00
   STA   __local_leave_cell__2
.cond_else@24:
   LDA   LEVEL_MAP@hi,X
   STA   __local_leave_cell__1
   LDA   __local_leave_cell__2
   STA   (__local_leave_cell__0),Y
   RTS

enter_cell:
   SUBROUTINE

   STA   __local_enter_cell__row
   TXA
   TAY
   LDX   __local_enter_cell__row
   LDA   LEVEL_BG@lo,X
   STA   __local_enter_cell__0
   LDA   LEVEL_MAP@hi,X
   STA   __local_enter_cell__1
   LDA   (__local_enter_cell__0),Y
   CMP   #$09
   BNE   .if_end@26
   LSR   ALIVE
.if_end@26:
   LDA   LEVEL_BG@lo,X
   STA   __local_enter_cell__0
   LDA   LEVEL_MAP@hi,X
   STA   __local_enter_cell__1
   LDA   #$08
   STA   (__local_enter_cell__0),Y
   RTS

is_floor:
   SUBROUTINE

   CMP   #$01
   BEQ   .or_true@27
   CMP   #$02
   BEQ   .or_true@27
   CMP   #$03
   BEQ   .or_true@27
   CLC
   RTS
.or_true@27:
   SEC
   RTS

guard_is_supported:
   SUBROUTINE

   LDX   GUARD_LOC_ROW
   LDY   GUARD_LOC_COL
   LDA   LEVEL_BG@lo,X
   STA   __local_guard_is_supported__0
   LDA   LEVEL_BG@hi,X
   STA   __local_guard_is_supported__1
   LDA   (__local_guard_is_supported__0),Y
   CMP   #$03
   BEQ   .tail@0
   CMP   #$04
   BNE   .if_end@34
   LDA   GUARD_Y_ADJ
   CMP   #$02
   BNE   .vthread@1
   SEC
   RTS
.if_end@34:
   LDA   GUARD_Y_ADJ
.vthread@1:
   CMP   #$02
   BCC   .guard_is_supported@scfold@0
   CPX   #$0F
   BEQ   .tail@0
   LDA   LEVEL_BG@lo+1,X
   STA   __local_guard_is_supported__0
   LDA   LEVEL_MAP@hi+1,X
   STA   __local_guard_is_supported__1
   LDA   (__local_guard_is_supported__0),Y
   BEQ   .guard_is_supported@scfold@0
   CMP   #$09
   BNE   .if_end@39
.guard_is_supported@scfold@0:
   CLC
   RTS
.if_end@39:
   CMP   #$08
   BNE   .if_end@40
.tail@0:
   SEC
   RTS
.if_end@40:
   LDA   LEVEL_BG@hi+1,X
   STA   __local_guard_is_supported__1
   LDA   (__local_guard_is_supported__0),Y
   JMP   is_floor

guard_fall_into_row_below:
   SUBROUTINE

   LDA   GUARD_LOC_COL
   STA   __local_guard_fall_into_row_below__col
   LDA   #$00
   STA   GUARD_Y_ADJ
   LDX   GUARD_LOC_COL
   LDA   GUARD_LOC_ROW
   JSR   leave_cell
   LDA   GUARD_LOC_ROW
   CLC
   ADC   #$01
   STA   __local_guard_fall_into_row_below__2
   STA   GUARD_LOC_ROW
   LDX   __local_guard_fall_into_row_below__2
   LDA   LEVEL_BG@lo,X
   STA   __local_guard_fall_into_row_below__0
   LDA   LEVEL_BG@hi,X
   STA   __local_guard_fall_into_row_below__1
   LDY   __local_guard_fall_into_row_below__col
   LDA   (__local_guard_fall_into_row_below__0),Y
   CMP   #$01
   BNE   .if_end@43
   LDA   GUARD_GOLD_TIMER
   BPL   .if_end@43
   DEX
   STX   GAME_ROWNUM
   STY   GAME_COLNUM
   LDA   LEVEL_BG@lo,X
   STA   __local_guard_fall_into_row_below__0
   LDA   LEVEL_BG@hi,X
   STA   __local_guard_fall_into_row_below__1
   LDA   (__local_guard_fall_into_row_below__0),Y
   BNE   .if_else@45
   LDA   LEVEL_MAP@hi,X
   STA   __local_guard_fall_into_row_below__1
   LDA   #$07
   STA   (__local_guard_fall_into_row_below__0),Y
   LDA   LEVEL_BG@hi,X
   STA   __local_guard_fall_into_row_below__1
   LDA   #$07
   STA   (__local_guard_fall_into_row_below__0),Y
   JSR   draw_gold_at_game_cell
   JMP   .if_end@44
.if_else@45:
   DEC   GOLD_COUNT
.if_end@44:
   LDA   #$00
   STA   GUARD_GOLD_TIMER
.if_end@43:
   LDX   __local_guard_fall_into_row_below__col
   LDA   __local_guard_fall_into_row_below__2
   JMP   enter_cell

guard_fall:
   SUBROUTINE

   JSR   guard_fall@run0
   LDA   GUARD_FACING_DIRECTION
   BPL   .cond_else@46
   LDA   #$06
   BNE   .cond_end@47
.cond_else@46:
   LDA   #$0D
.cond_end@47:
   STA   GUARD_ANIM_STATE
   INC   GUARD_Y_ADJ
   LDA   GUARD_Y_ADJ
   CMP   #$05
   BCC   .if_else@49
   JSR   guard_fall_into_row_below
   JMP   draw_guard
.if_else@49:
   CMP   #$02
   BNE   .if_end@50
   JSR   check_for_gold_picked_up_by_guard
   LDX   GUARD_LOC_ROW
   LDA   LEVEL_BG@lo,X
   STA   __local_guard_fall__0
   LDA   LEVEL_BG@hi,X
   STA   __local_guard_fall__1
   LDY   GUARD_LOC_COL
   LDA   (__local_guard_fall__0),Y
   CMP   #$01
   BNE   .if_end@51
   LDA   GUARD_GOLD_TIMER
   BPL   .if_end@52
   DEC   GOLD_COUNT
.if_end@52:
   LDA   GUARD_GOLD_TIMER_START_VALUE
   STA   GUARD_GOLD_TIMER
   LDX   #$00
   LDA   #$75
   JSR   add_and_update_score
   LDX   #>guard_in_hole_tune
   LDA   #<guard_in_hole_tune
   JSR   load_sound_data
.if_end@51:
.if_end@50:
.if_end@48:
   JMP   draw_guard

guard_fall@run0:
   SUBROUTINE

   JSR   erase_guard
   JMP   nudge_guard_towards_exact_column

guard_do_nothing:
   SUBROUTINE

   JSR   guard_in_hole
   BCC   .if_end@53
   INC   GUARD_GOLD_TIMER
.if_end@53:
   RTS

guard_blocked_above:
   SUBROUTINE

   LDX   GUARD_LOC_ROW
   BEQ   .or_true@55
   LDA   LEVEL_BG@lo-1,X
   STA   __local_guard_blocked_above__0
   LDA   LEVEL_MAP@hi-1,X
   STA   __local_guard_blocked_above__1
   LDY   GUARD_LOC_COL
   LDA   (__local_guard_blocked_above__0),Y
   CMP   #$01
   BEQ   .or_true@55
   CMP   #$02
   BEQ   .or_true@55
   CMP   #$05
   BEQ   .or_true@55
   CMP   #$08
   BEQ   .or_true@55
   CLC
   RTS
.or_true@55:
   SEC
   RTS

try_guard_move_up:
   SUBROUTINE

   LDA   GUARD_Y_ADJ
   CMP   #$03
   BCS   .if_end@63
   JSR   guard_blocked_above
   BCC   .if_end@63
   JMP   guard_do_nothing
.if_end@63:
   JSR   guard_fall@run0
   LDA   GUARD_LOC_COL
   STA   __local_try_guard_move_up__col
   DEC   GUARD_Y_ADJ
   BPL   .if_else@65
   JSR   guard_drop_gold
   LDX   __local_try_guard_move_up__col
   LDA   GUARD_LOC_ROW
   JSR   leave_cell
   DEC   GUARD_LOC_ROW
   LDA   GUARD_LOC_ROW
   LDX   __local_try_guard_move_up__col
   JSR   enter_cell
   LDA   #$04
   STA   GUARD_Y_ADJ
   JMP   .if_end@64
.if_else@65:
   JSR   check_for_gold_picked_up_by_guard
.if_end@64:
   JMP   try_guard_move_up@tail1

try_guard_move_up@tail1:
   SUBROUTINE

   LDX   #$0F
   LDA   #$0E
   JMP   try_guard_move_up@tail0

try_guard_move_up@tail0:
   SUBROUTINE

   JSR   inc_guard_anim_state
   JMP   draw_guard

try_guard_move_down:
   SUBROUTINE

   LDA   GUARD_LOC_COL
   STA   __local_try_guard_move_down__col
   LDA   GUARD_Y_ADJ
   CMP   #$02
   BCC   .if_end@66
   LDX   GUARD_LOC_ROW
   CPX   #$0F
   BCC   .if_end@67
   RTS
.if_end@67:
   LDA   LEVEL_BG@lo+1,X
   STA   __local_try_guard_move_down__0
   LDA   LEVEL_MAP@hi+1,X
   STA   __local_try_guard_move_down__1
   LDY   __local_try_guard_move_down__col
   LDA   (__local_try_guard_move_down__0),Y
   CMP   #$02
   BEQ   .try_guard_move_down@scfold@0
   CMP   #$08
   BEQ   .try_guard_move_down@scfold@0
   CMP   #$01
   BNE   .if_end@72
.try_guard_move_down@scfold@0:
   RTS
.if_end@72:
.if_end@66:
   JSR   guard_fall@run0
   INC   GUARD_Y_ADJ
   LDA   GUARD_Y_ADJ
   CMP   #$05
   BCC   .if_else@74
   JSR   guard_drop_gold
   LDX   __local_try_guard_move_down__col
   LDA   GUARD_LOC_ROW
   JSR   leave_cell
   INC   GUARD_LOC_ROW
   LDA   GUARD_LOC_ROW
   LDX   __local_try_guard_move_down__col
   JSR   enter_cell
   LDA   #$00
   STA   GUARD_Y_ADJ
   JMP   .if_end@73
.if_else@74:
   JSR   check_for_gold_picked_up_by_guard
.if_end@73:
   JMP   try_guard_move_up@tail1

try_guard_move_left:
   SUBROUTINE

   LDA   GUARD_LOC_ROW
   STA   __local_try_guard_move_left__row
   LDA   GUARD_X_ADJ
   CMP   #$03
   BCS   .if_end@75
   LDA   GUARD_LOC_COL
   BNE   .if_end@76
   RTS
.if_end@76:
   LDX   __local_try_guard_move_left__row
   LDA   LEVEL_BG@lo,X
   STA   __local_try_guard_move_left__0
   LDA   LEVEL_MAP@hi,X
   STA   __local_try_guard_move_left__1
   LDY   GUARD_LOC_COL
   DEY
   LDA   (__local_try_guard_move_left__0),Y
   CMP   #$08
   BEQ   .try_guard_move_left@scfold@0
   CMP   #$02
   BEQ   .try_guard_move_left@scfold@0
   CMP   #$01
   BEQ   .try_guard_move_left@scfold@0
   LDA   LEVEL_BG@hi,X
   STA   __local_try_guard_move_left__1
   LDA   (__local_try_guard_move_left__0),Y
   CMP   #$05
   BNE   .if_end@83
.try_guard_move_left@scfold@0:
   RTS
.if_end@83:
.if_end@75:
   JSR   erase_guard
   JSR   nudge_guard_towards_exact_row
   LDA   #$FF
   STA   GUARD_FACING_DIRECTION
   DEC   GUARD_X_ADJ
   BPL   .if_else@85
   JSR   guard_drop_gold
   LDX   GUARD_LOC_COL
   LDA   __local_try_guard_move_left__row
   JSR   leave_cell
   LDX   GUARD_LOC_COL
   DEX
   STX   GUARD_LOC_COL
   LDA   __local_try_guard_move_left__row
   JSR   enter_cell
   LDA   #$04
   STA   GUARD_X_ADJ
   JMP   .if_end@84
.if_else@85:
   JSR   check_for_gold_picked_up_by_guard
.if_end@84:
   LDX   __local_try_guard_move_left__row
   LDA   LEVEL_BG@lo,X
   STA   __local_try_guard_move_left__0
   LDA   LEVEL_BG@hi,X
   STA   __local_try_guard_move_left__1
   LDY   GUARD_LOC_COL
   LDA   (__local_try_guard_move_left__0),Y
   CMP   #$04
   BNE   .if_else@87
   LDX   #$05
   LDA   #$03
   JMP   try_guard_move_up@tail0
.if_else@87:
   LDX   #$02
   LDA   #$00
   JSR   inc_guard_anim_state
.if_end@86:
   JMP   draw_guard

try_guard_move_right:
   SUBROUTINE

   LDA   GUARD_LOC_ROW
   STA   __local_try_guard_move_right__row
   LDA   GUARD_X_ADJ
   CMP   #$02
   BCC   .if_end@88
   LDA   GUARD_LOC_COL
   CMP   #$1B
   BNE   .if_end@89
   RTS
.if_end@89:
   LDX   __local_try_guard_move_right__row
   LDA   LEVEL_BG@lo,X
   STA   __local_try_guard_move_right__0
   LDA   LEVEL_MAP@hi,X
   STA   __local_try_guard_move_right__1
   LDY   GUARD_LOC_COL
   INY
   LDA   (__local_try_guard_move_right__0),Y
   CMP   #$08
   BEQ   .try_guard_move_right@scfold@0
   CMP   #$02
   BEQ   .try_guard_move_right@scfold@0
   CMP   #$01
   BEQ   .try_guard_move_right@scfold@0
   LDA   LEVEL_BG@hi,X
   STA   __local_try_guard_move_right__1
   LDA   (__local_try_guard_move_right__0),Y
   CMP   #$05
   BNE   .if_end@96
.try_guard_move_right@scfold@0:
   RTS
.if_end@96:
.if_end@88:
   JSR   erase_guard
   JSR   nudge_guard_towards_exact_row
   LDA   #$01
   STA   GUARD_FACING_DIRECTION
   INC   GUARD_X_ADJ
   LDA   GUARD_X_ADJ
   CMP   #$05
   BCC   .if_else@98
   JSR   guard_drop_gold
   LDX   GUARD_LOC_COL
   LDA   __local_try_guard_move_right__row
   JSR   leave_cell
   LDX   GUARD_LOC_COL
   INX
   STX   GUARD_LOC_COL
   LDA   __local_try_guard_move_right__row
   JSR   enter_cell
   LDA   #$00
   STA   GUARD_X_ADJ
   JMP   .if_end@97
.if_else@98:
   JSR   check_for_gold_picked_up_by_guard
.if_end@97:
   LDX   __local_try_guard_move_right__row
   LDA   LEVEL_BG@lo,X
   STA   __local_try_guard_move_right__0
   LDA   LEVEL_BG@hi,X
   STA   __local_try_guard_move_right__1
   LDY   GUARD_LOC_COL
   LDA   (__local_try_guard_move_right__0),Y
   CMP   #$04
   BNE   .if_else@100
   LDX   #$0C
   LDA   #$0A
   JMP   try_guard_move_up@tail0
.if_else@100:
   LDX   #$09
   LDA   #$07
   JSR   inc_guard_anim_state
.if_end@99:
   JMP   draw_guard

guard_take_move:
   SUBROUTINE

   JSR   determine_guard_move
   CMP   #$01
   BEQ   .case@2
   CMP   #$02
   BEQ   .case@3
   CMP   #$03
   BEQ   .case@4
   CMP   #$04
   BEQ   .case@5
   RTS
.case@2:
   JMP   try_guard_move_left
.case@3:
   JMP   try_guard_move_right
.case@4:
   JMP   try_guard_move_up
.case@5:
   JMP   try_guard_move_down

move_guard:
   SUBROUTINE

   LDA   GUARD_NUM
   CLC
   ADC   #$01
   STA   __local_move_guard__0
   STA   GUARD_NUM
   LDA   GUARD_COUNT
   CMP   __local_move_guard__0
   BCS   .if_end@101
   LDA   #$01
   STA   GUARD_NUM
.if_end@101:
   JSR   load_guard_data
   JSR   guard_in_hole
   BCC   .if_else@103
   DEC   GUARD_GOLD_TIMER
   LDA   GUARD_GOLD_TIMER
   CMP   #$0D
   BCC   .if_else@105
   LDX   GUARD_NUM
   LDA   GUARD_RESURRECTION_TIMERS,X
   BNE   .if_end@102
   JSR   draw_guard
   JMP   store_guard_data
.if_else@105:
   CMP   #$07
   BCC   .if_else@108
   JSR   erase_guard
   LDA   GUARD_GOLD_TIMER
   SEC
   SBC   #$07
   TAX
   LDA   GUARD_X_ADJ_TABLE,X
   STA   GUARD_X_ADJ
   JSR   draw_guard
   JMP   store_guard_data
.if_else@108:
   JSR   guard_take_move
   JMP   store_guard_data
.if_else@103:
   JSR   guard_is_supported
   BCC   .if_else@110
   JSR   guard_take_move
   JMP   store_guard_data
.if_else@110:
   JSR   guard_fall
.if_end@109:
.if_end@102:
   JMP   store_guard_data

pseudo_distance:
   SUBROUTINE

   STA   __local_pseudo_distance__row
   STX   __local_pseudo_distance__col
   CMP   PLAYER_ROW
   BNE   .if_end@111
   TXA
   SEC
   SBC   GUARD_LOC_COL
   BCC   .if_end@112
   RTS
.if_end@112:
   LDA   GUARD_LOC_COL
   SEC
   SBC   __local_pseudo_distance__col
   RTS
.if_end@111:
   LDA   PLAYER_ROW
   CMP   __local_pseudo_distance__row
   BCS   .if_end@113
   LDA   __local_pseudo_distance__row
   SEC
   SBC   PLAYER_ROW
   CLC
   ADC   #$C8
   RTS
.if_end@113:
   SBC   __local_pseudo_distance__row
   CLC
   ADC   #$64
   RTS

guard_find_candidate_row_above:
   SUBROUTINE

   STA   __local_guard_find_candidate_row_above__row
   STX   __local_guard_find_candidate_row_above__col
.loop@6_start:
   LDX   __local_guard_find_candidate_row_above__row
   LDA   LEVEL_BG@lo,X
   STA   __local_guard_find_candidate_row_above__4
   LDA   LEVEL_BG@hi,X
   STA   __local_guard_find_candidate_row_above__5
   LDY   __local_guard_find_candidate_row_above__col
   LDA   (__local_guard_find_candidate_row_above__4),Y
   CMP   #$03
   BEQ   .if_end@114
   TXA
   RTS
.if_end@114:
   DEX
   STX   __local_guard_find_candidate_row_above__3
   TYA
   BEQ   .if_end@119
   SEC
   SBC   #$01
   STA   __local_guard_find_candidate_row_above__2
   LDY   __local_guard_find_candidate_row_above__2
   LDA   (__local_guard_find_candidate_row_above__4),Y
   JSR   is_floor
   BCS   .guard_find_candidate_row_above@scfold@0
   LDX   __local_guard_find_candidate_row_above__3
   LDA   LEVEL_BG@lo,X
   STA   __local_guard_find_candidate_row_above__0
   LDA   LEVEL_BG@hi,X
   STA   __local_guard_find_candidate_row_above__1
   LDY   __local_guard_find_candidate_row_above__2
   LDA   (__local_guard_find_candidate_row_above__0),Y
   CMP   #$04
   BNE   .if_end@119
.guard_find_candidate_row_above@scfold@0:
   LDA   PLAYER_ROW
   CMP   __local_guard_find_candidate_row_above__3
   BCS   .tail@1
.if_end@119:
   LDA   __local_guard_find_candidate_row_above__col
   CMP   #$1B
   BEQ   .if_end@125
   LDX   __local_guard_find_candidate_row_above__row
   LDA   LEVEL_BG@lo,X
   STA   __local_guard_find_candidate_row_above__0
   LDA   LEVEL_BG@hi,X
   STA   __local_guard_find_candidate_row_above__1
   LDA   __local_guard_find_candidate_row_above__col
   CLC
   ADC   #$01
   STA   __local_guard_find_candidate_row_above__2
   LDY   __local_guard_find_candidate_row_above__2
   LDA   (__local_guard_find_candidate_row_above__0),Y
   JSR   is_floor
   BCS   .guard_find_candidate_row_above@scfold@1
   LDX   __local_guard_find_candidate_row_above__3
   LDA   LEVEL_BG@lo,X
   STA   __local_guard_find_candidate_row_above__0
   LDA   LEVEL_BG@hi,X
   STA   __local_guard_find_candidate_row_above__1
   LDY   __local_guard_find_candidate_row_above__2
   LDA   (__local_guard_find_candidate_row_above__0),Y
   CMP   #$04
   BNE   .if_end@125
.guard_find_candidate_row_above@scfold@1:
   LDA   PLAYER_ROW
   CMP   __local_guard_find_candidate_row_above__3
   BCC   .if_end@126
.tail@1:
   LDA   __local_guard_find_candidate_row_above__3
   RTS
.if_end@126:
.if_end@125:
   LDA   __local_guard_find_candidate_row_above__3
   BNE   .if_end@127
   RTS
.if_end@127:
   STA   __local_guard_find_candidate_row_above__row
   JMP   .loop@6_start

guard_find_candidate_row_below:
   SUBROUTINE

   STA   __local_guard_find_candidate_row_below__row
   STX   __local_guard_find_candidate_row_below__col
.loop@7_start:
   LDA   __local_guard_find_candidate_row_below__row
   CMP   #$0F
   BNE   .lb_skip@0
   JMP   .loop@7_break
.lb_skip@0:
   LDX   __local_guard_find_candidate_row_below__row
   LDA   LEVEL_BG@lo+1,X
   STA   __local_guard_find_candidate_row_below__right_row
   LDA   LEVEL_BG@hi+1,X
   STA   __local_guard_find_candidate_row_below__0
   LDY   __local_guard_find_candidate_row_below__col
   LDA   (__local_guard_find_candidate_row_below__right_row),Y
   CMP   #$01
   BEQ   .guard_find_candidate_row_below@scfold@0
   CMP   #$02
   BNE   .if_end@130
.guard_find_candidate_row_below@scfold@0:
   TXA
   RTS
.if_end@130:
   LDA   LEVEL_BG@lo,X
   STA   __local_guard_find_candidate_row_below__0
   LDA   LEVEL_BG@hi,X
   STA   __local_guard_find_candidate_row_below__1
   LDA   (__local_guard_find_candidate_row_below__0),Y
   BEQ   .loop@7_continue
   TYA
   BEQ   .guard_find_candidate_row_below@asm_ssa_split@0
   DEY
   LDA   (__local_guard_find_candidate_row_below__0),Y
   CMP   #$04
   BNE   .if_else@134
   TXA
   CMP   PLAYER_ROW
   BCC   .if_end@135
   RTS
.if_end@135:
   STA   __local_guard_find_candidate_row_below__right_row
   JMP   .if_end@133
.if_else@134:
   LDA   LEVEL_BG@lo+1,X
   STA   __local_guard_find_candidate_row_below__0
   LDA   LEVEL_BG@hi+1,X
   STA   __local_guard_find_candidate_row_below__1
   INX
   STX   __local_guard_find_candidate_row_below__right_row
   LDA   (__local_guard_find_candidate_row_below__0),Y
   JSR   is_floor
   BCC   .if_end@136
   LDA   __local_guard_find_candidate_row_below__row
   CMP   PLAYER_ROW
   BCC   .if_end@137
   RTS
.if_end@137:
.if_end@136:
.if_end@133:
.if_end@132:
   LDA   __local_guard_find_candidate_row_below__col
   CMP   #$1B
   BCS   .if_end@142
   LDX   __local_guard_find_candidate_row_below__right_row
   LDA   LEVEL_BG@lo,X
   STA   __local_guard_find_candidate_row_below__right_row
   LDA   LEVEL_BG@hi,X
   STA   __local_guard_find_candidate_row_below__0
   LDY   __local_guard_find_candidate_row_below__col
   INY
   LDA   (__local_guard_find_candidate_row_below__right_row),Y
   CMP   #$04
   BEQ   .guard_find_candidate_row_below@scfold@1
   LDX   __local_guard_find_candidate_row_below__row
   LDA   LEVEL_BG@lo+1,X
   STA   __local_guard_find_candidate_row_below__right_row
   LDA   LEVEL_BG@hi+1,X
   STA   __local_guard_find_candidate_row_below__0
   LDA   (__local_guard_find_candidate_row_below__right_row),Y
   JSR   is_floor
   BCC   .if_end@142
.guard_find_candidate_row_below@scfold@1:
   LDA   __local_guard_find_candidate_row_below__row
   CMP   PLAYER_ROW
   BCC   .if_end@143
   RTS
.if_end@143:
.if_end@142:
.loop@7_continue:
   INC   __local_guard_find_candidate_row_below__row
   JMP   .loop@7_start
.loop@7_break:
   LDA   #$0F
   RTS
.guard_find_candidate_row_below@asm_ssa_split@0:
   STX   __local_guard_find_candidate_row_below__right_row
   JMP   .if_end@132

guard_left_col_limit:
   SUBROUTINE

   STA   __local_guard_left_col_limit__row
   STX   __local_guard_left_col_limit__col
.loop@8_continue:
   LDA   __local_guard_left_col_limit__col
   BEQ   .vthread@2
   LDX   __local_guard_left_col_limit__row
   LDA   LEVEL_BG@lo,X
   STA   __local_guard_left_col_limit__2
   LDA   LEVEL_MAP@hi,X
   STA   __local_guard_left_col_limit__3
   LDA   __local_guard_left_col_limit__col
   SEC
   SBC   #$01
   STA   __local_guard_left_col_limit__1
   LDY   __local_guard_left_col_limit__1
   LDA   (__local_guard_left_col_limit__2),Y
   CMP   #$01
   BEQ   .loop@8_break
   CMP   #$02
   BEQ   .loop@8_break
   CMP   #$03
   BEQ   .if_end@151
   CMP   #$04
   BEQ   .if_end@151
   CPX   #$0F
   BEQ   .if_end@151
   LDA   LEVEL_BG@lo+1,X
   STA   __local_guard_left_col_limit__col
   LDA   LEVEL_BG@hi+1,X
   STA   __local_guard_left_col_limit__0
   LDA   (__local_guard_left_col_limit__col),Y
   JSR   is_floor
   BCS   .if_end@152
   LDA   __local_guard_left_col_limit__1
   STA   __local_guard_left_col_limit__col
   JMP   .loop@8_break
.if_end@152:
.if_end@151:
   LDA   __local_guard_left_col_limit__1
   STA   __local_guard_left_col_limit__col
   JMP   .loop@8_continue
.loop@8_break:
   LDA   __local_guard_left_col_limit__col
.vthread@2:
   RTS

guard_right_col_limit:
   SUBROUTINE

   STA   __local_guard_right_col_limit__row
   STX   __local_guard_right_col_limit__col
.loop@9_continue:
   LDA   __local_guard_right_col_limit__col
   CMP   #$1B
   BEQ   .vthread@3
   LDX   __local_guard_right_col_limit__row
   LDA   LEVEL_BG@lo,X
   STA   __local_guard_right_col_limit__2
   LDA   LEVEL_MAP@hi,X
   STA   __local_guard_right_col_limit__3
   LDA   __local_guard_right_col_limit__col
   CLC
   ADC   #$01
   STA   __local_guard_right_col_limit__1
   LDY   __local_guard_right_col_limit__1
   LDA   (__local_guard_right_col_limit__2),Y
   CMP   #$01
   BEQ   .loop@9_break
   CMP   #$02
   BEQ   .loop@9_break
   CMP   #$03
   BEQ   .if_end@160
   CMP   #$04
   BEQ   .if_end@160
   CPX   #$0F
   BEQ   .if_end@160
   LDA   LEVEL_BG@lo+1,X
   STA   __local_guard_right_col_limit__col
   LDA   LEVEL_BG@hi+1,X
   STA   __local_guard_right_col_limit__0
   LDA   (__local_guard_right_col_limit__col),Y
   JSR   is_floor
   BCS   .if_end@161
   LDA   __local_guard_right_col_limit__1
   STA   __local_guard_right_col_limit__col
   JMP   .loop@9_break
.if_end@161:
.if_end@160:
   LDA   __local_guard_right_col_limit__1
   STA   __local_guard_right_col_limit__col
   JMP   .loop@9_continue
.loop@9_break:
   LDA   __local_guard_right_col_limit__col
.vthread@3:
   RTS

consider_move:
   SUBROUTINE

   CMP   best_guard_dist
   BCS   .if_end@162
   STA   best_guard_dist
   STX   guard_action
.if_end@162:
   RTS

consider_column:
   SUBROUTINE

   STA   __local_consider_column__row
   STX   __local_consider_column__col
   CMP   #$0F
   BEQ   .if_end@163
   LDX   __local_consider_column__row
   LDA   LEVEL_BG@lo+1,X
   STA   __local_consider_column__0
   LDA   LEVEL_BG@hi+1,X
   STA   __local_consider_column__1
   LDY   __local_consider_column__col
   LDA   (__local_consider_column__0),Y
   CMP   #$01
   BEQ   .if_end@166
   CMP   #$02
   BEQ   .if_end@166
   LDX   __local_consider_column__col
   LDA   __local_consider_column__row
   JSR   guard_find_candidate_row_below
   LDX   __local_consider_column__col
   JSR   pseudo_distance
   LDX   __zpabi_consider_column__down
   JSR   consider_move
.if_end@166:
.if_end@163:
   LDA   __local_consider_column__row
   BEQ   .if_end@169
   LDX   __local_consider_column__row
   LDA   LEVEL_BG@lo,X
   STA   __local_consider_column__0
   LDA   LEVEL_BG@hi,X
   STA   __local_consider_column__1
   LDY   __local_consider_column__col
   LDA   (__local_consider_column__0),Y
   CMP   #$03
   BNE   .if_end@169
   LDX   __local_consider_column__col
   LDA   __local_consider_column__row
   JSR   guard_find_candidate_row_above
   LDX   __local_consider_column__col
   JSR   pseudo_distance
   LDX   __zpabi_consider_column__up
   JSR   consider_move
.if_end@169:
   RTS

guard_can_cross:
   SUBROUTINE

   STA   __local_guard_can_cross__row
   TXA
   TAY
   LDX   __local_guard_can_cross__row
   LDA   LEVEL_BG@lo,X
   STA   __local_guard_can_cross__0
   LDA   LEVEL_BG@hi,X
   STA   __local_guard_can_cross__1
   LDA   (__local_guard_can_cross__0),Y
   CMP   #$03
   BEQ   .tail@2
   CMP   #$04
   BEQ   .tail@2
   CPX   #$0F
   BEQ   .tail@2
   LDA   LEVEL_BG@lo+1,X
   STA   __local_guard_can_cross__0
   LDA   LEVEL_BG@hi+1,X
   STA   __local_guard_can_cross__1
   LDA   (__local_guard_can_cross__0),Y
   BEQ   .and_false@175
   CMP   #$05
   BEQ   .and_false@175
.tail@2:
   SEC
   RTS
.and_false@175:
   CLC
   RTS

guard_run_at_player:
   SUBROUTINE

   STA   __local_guard_run_at_player__row
   STX   __local_guard_run_at_player__col
   CPX   PLAYER_COL
   BCS   .if_end@177
.loop@10_start:
   INC   __local_guard_run_at_player__col
   LDX   __local_guard_run_at_player__col
   LDA   __local_guard_run_at_player__row
   JSR   guard_can_cross
   BCC   .tail@3
   LDA   __local_guard_run_at_player__col
   CMP   PLAYER_COL
   BNE   .loop@10_start
   LDA   #$02
   RTS
.if_end@177:
.loop@11_start:
   DEC   __local_guard_run_at_player__col
   LDX   __local_guard_run_at_player__col
   LDA   __local_guard_run_at_player__row
   JSR   guard_can_cross
   BCS   .if_end@179
.tail@3:
   LDA   #$00
   RTS
.if_end@179:
   LDA   __local_guard_run_at_player__col
   CMP   PLAYER_COL
   BNE   .loop@11_start
   LDA   #$01
   RTS

determine_guard_move:
   SUBROUTINE

   LDX   GUARD_LOC_ROW
   LDA   LEVEL_BG@lo,X
   STA   __local_determine_guard_move__c
   LDA   LEVEL_BG@hi,X
   STA   __local_determine_guard_move__0
   LDY   GUARD_LOC_COL
   LDA   (__local_determine_guard_move__c),Y
   CMP   #$01
   BNE   .if_end@182
   JSR   guard_in_hole
   BCC   .if_end@182
   LDA   #$03
   RTS
.if_end@182:
   LDA   GUARD_LOC_ROW
   CMP   PLAYER_ROW
   BNE   .if_end@183
   LDX   GUARD_LOC_COL
   JSR   guard_run_at_player
   ORA   #$00
   BEQ   .if_end@184
   RTS
.if_end@184:
.if_end@183:
   LDA   #$00
   STA   guard_action
   LDA   #$FF
   STA   best_guard_dist
   LDA   #$04
   STA   __zpabi_consider_column__down
   LDA   #$03
   STA   __zpabi_consider_column__up
   LDX   GUARD_LOC_COL
   LDA   GUARD_LOC_ROW
   JSR   consider_column
   LDX   GUARD_LOC_COL
   LDA   GUARD_LOC_ROW
   JSR   guard_left_col_limit
   STA   __local_determine_guard_move__c
   JMP   .loop@12_start
.determine_guard_move@rot@0:
   LDA   #$01
   STA   __zpabi_consider_column__down
   STA   __zpabi_consider_column__up
   LDX   __local_determine_guard_move__c
   LDA   GUARD_LOC_ROW
   JSR   consider_column
   INC   __local_determine_guard_move__c
.loop@12_start:
   LDA   __local_determine_guard_move__c
   CMP   GUARD_LOC_COL
   BNE   .determine_guard_move@rot@0
   LDX   GUARD_LOC_COL
   LDA   GUARD_LOC_ROW
   JSR   guard_right_col_limit
   STA   __local_determine_guard_move__c
   JMP   .loop@13_start
.determine_guard_move@rot@1:
   LDA   #$02
   STA   __zpabi_consider_column__down
   STA   __zpabi_consider_column__up
   LDX   __local_determine_guard_move__c
   LDA   GUARD_LOC_ROW
   JSR   consider_column
   DEC   __local_determine_guard_move__c
.loop@13_start:
   LDA   __local_determine_guard_move__c
   CMP   GUARD_LOC_COL
   BNE   .determine_guard_move@rot@1
   LDA   guard_action
   RTS

GUARD_PATTERNS_LIST:
   DC.B  $00, $00, $00, $00, $01, $01, $01, $01, $01, $01, $03, $01, $01, $03, $03, $03
   DC.B  $03, $03, $03, $03, $07, $03, $07, $07, $07, $07, $07, $07, $07, $0F, $07, $0F
   DC.B  $0F, $0F, $0F, $0F

GUARD_ANIM_SPRITES:
   DC.B  $08, $2B, $2C, $30, $31, $32, $36, $28, $29, $2A, $2D, $2E, $2F, $35, $33, $34

GUARD_X_ADJ_TABLE:
   DC.B  $02, $01, $02, $03, $02, $01

guard_in_hole_tune:
   DC.B  $06, $20, $04, $30, $02, $40, $00

LEVEL_BG@lo:
   DC.B  $00, $1C, $38, $54, $70, $8C, $A8, $C4, $E0, $FC, $18, $34, $50, $6C, $88, $A4
   DC.B  $C0

LEVEL_BG@hi:
   DC.B  $0A, $0A, $0A, $0A, $0A, $0A, $0A, $0A, $0A, $0A, $0B, $0B, $0B, $0B, $0B, $0B
   DC.B  $0B

LEVEL_MAP@hi:
   DC.B  $08, $08, $08, $08, $08, $08, $08, $08, $08, $08, $09, $09, $09, $09, $09, $09
   DC.B  $09

__zp_init:
   SUBROUTINE

   LDA   #$00
   STA   guard_action
   STA   best_guard_dist
   RTS
