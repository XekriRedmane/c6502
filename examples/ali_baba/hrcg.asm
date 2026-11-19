NULL_PAGE_LOW	EQU	$00
TEXT_STREAM_PTR	EQU	$06
RWTS_IOB_PTR	EQU	$08
NULL_PAGE_HIGH	EQU	$0A
HARGS	EQU	$10
SSP	EQU	$1B
FP	EQU	$1D
MONITOR_YSAV1_1F	EQU	$1F
WNDLFT	EQU	$20
WNDWDTH	EQU	$21
WNDTOP	EQU	$22
WNDBTM	EQU	$23
TEXT_COL	EQU	$24
TEXT_ROW	EQU	$25
MONITOR_GBAS	EQU	$26
BASL	EQU	$28
DOS_ZP	EQU	$2A
DISK_VM_ZP	EQU	$50
DPTR	EQU	$60
hrcg_row_address@ret	EQU	$62
hrcg_saved_valid	EQU	$64
hrcg_saved_ch	EQU	$65
hrcg_next	EQU	$66
hrcg_saved_cv	EQU	$67
__local_hrcg_cout__next	EQU	$80
__zpabi_modify_current_stats__add	EQU	$80
__zpabi_num_to_decimal__out_0	EQU	$80
__zpabi_update_event_list__value_0	EQU	$80
__local_hrcg_cout__0	EQU	$81
__zpabi_num_to_decimal__out_1	EQU	$81
__zpabi_update_event_list__value_1	EQU	$81
__local_hrcg_draw_char__glyph	EQU	$82
__local_hrcg_clear_from__row	EQU	$83
__local_hrcg_draw_char__0	EQU	$83
__local_hrcg_set_top_left__top	EQU	$83
__local_hrcg_draw_char__2	EQU	$86
__local_hrcg_draw_char__3	EQU	$87
__local_hrcg_draw_char__raw	EQU	$88
__local_hrcg_scroll_up__n	EQU	$8B
__local_hrcg_scroll_up__dst	EQU	$8E
__local_hrcg_scroll_up__2	EQU	$8F
__local_hrcg_scroll_up__row	EQU	$90
__local_hrcg_fill_line__col	EQU	$95
__local_hrcg_row_address__0	EQU	$96
__local_hrcg_row_address__1	EQU	$97
__local_hrcg_row_address__row	EQU	$98
__local_hrcg_row_address__2	EQU	$99
__local_hrcg_row_address__3	EQU	$9A
__local_hrcg_row_address__col	EQU	$9B
MENU_DESC_PTR	EQU	$C0
MENU_OPTION_PTR	EQU	$C2
TUNE_PC	EQU	$E4
MOB_PTR	EQU	$F4
ENTITY_PTR	EQU	$F8
CHAR_PTR	EQU	$FA
SOFTEV	EQU	$03F2
PWREDUP	EQU	$03F4
LOCATIONS	EQU	$4000
LOCATIONS_END	EQU	$4237
EVENT_THRESHOLD	EQU	$4239
SCENE_COMBAT_FLAG	EQU	$423A
SCENE_P3_DONE	EQU	$423B
SCENE_P1_DONE	EQU	$423C
SCENE_HOSTILE_DONE	EQU	$423D
SCENE_EVENT_RESULT	EQU	$423E
MOBS	EQU	$4C80
INPUT_MODE	EQU	$5A00
TOTAL_MOB_COUNT	EQU	$5A01
CURRENT_PLAYER	EQU	$5A02
CURRENT_ROOM	EQU	$5A03
CHAR_SPRITE	EQU	$5A04
CHAR_AI_MODE	EQU	$5A05
AI_WAIT_FLAG	EQU	$5A06
BROWSE_CHAR_INDEX	EQU	$5A07
VIEW_MAX_COL	EQU	$5A08
VIEW_MAX_ROW	EQU	$5A09
VIEW_MIN_COL	EQU	$5A0A
VIEW_MIN_ROW	EQU	$5A0B
FONT_COL	EQU	$5A0C
FONT_ROW	EQU	$5A0D
FONT_CHARNUM	EQU	$5A0E
FONT_CHARSET	EQU	$5A0F
DAMAGE_AMOUNT	EQU	$5A10
NEAREST_POS	EQU	$5A13
CANDIDATE_POS	EQU	$5A15
HEAL_NOTIFIED	EQU	$5A16
LOCATION_ID	EQU	$5A17
PRNG_OUTPUT	EQU	$5A18
PRNG_STATE	EQU	$5A19
NUM_DIGIT	EQU	$5A1B
NUM_DIVISOR	EQU	$5A1C
NUM_LEADING	EQU	$5A1E
ENCOUNTER_RESULT	EQU	$5A1F
COLOCATE_LINK	EQU	$5A20
COLOCATE_POS	EQU	$5A21
COLOCATE_WRAPPED	EQU	$5A22
HOSTILE_APP	EQU	$5A23
HOSTILE_A	EQU	$5A24
HOSTILE_Y	EQU	$5A25
ADJACENT_THREAT	EQU	$5A26
ADJACENT_POS	EQU	$5A27
SOURCE_CHAR	EQU	$5A28
ACTIVE_CHAR	EQU	$5A29
BOUNDS_MIN_COL	EQU	$5A51
BOUNDS_MAX_COL	EQU	$5A52
BOUNDS_MIN_ROW	EQU	$5A53
BOUNDS_MAX_ROW	EQU	$5A54
LOCATION_POS	EQU	$5A55
LOCATION_STYLE	EQU	$5A56
MSG_LINE_COUNT	EQU	$5A57
EVENT_SLOT_BYTE	EQU	$5A59
EVENT_SUB_VALUE	EQU	$5A5A
SAVED_EVENT_PTR	EQU	$5A5B
AI_PREV_DIR	EQU	$5A5D
EVENT_POS	EQU	$5A5E
EVENT_DEST_POS	EQU	$5A5F
TURN_START_COL	EQU	$5A61
TURN_START_ROW	EQU	$5A62
CURRENT_COL	EQU	$5A63
CURRENT_ROW	EQU	$5A64
MOVE_POINTS	EQU	$5A65
STEPS_TAKEN	EQU	$5A66
INPUT_DIR	EQU	$5A67
AI_BEST_DIST	EQU	$5A68
DIR_SCORES	EQU	$5A6A
AI_TARGET_POS	EQU	$5A71
AI_TARGET_COL	EQU	$5A72
AI_TARGET_ROW	EQU	$5A73
IS_PLAYER_TURN	EQU	$5A74
SAVED_MOB_LINK	EQU	$5A75
COMBAT_WILLINGNESS	EQU	$5A76
TARGET_DEFENSE	EQU	$5A77
COMBAT_STRENGTH	EQU	$5A78
MOB_DEFENSE	EQU	$5A79
DROP_AMOUNT	EQU	$5A7A
DROP_STEP	EQU	$5A7C
STAT_SCALE	EQU	$5A7E
STAT_LEVEL_BITS	EQU	$5A7F
SHOP_ITEM_MASK	EQU	$5A80
SHOP_PRICE	EQU	$5A81
SHOP_VALID_MASK	EQU	$5A83
LOCATION_STYLE_BITS	EQU	$5A84
SHOP_ITEM_OFFSET	EQU	$5A85
TARGET_PTR	EQU	$5A86
BEST_THREAT	EQU	$5A88
SCENE_FOUND_FLAG	EQU	$5A89
SCENE_HOSTILE_FLAG	EQU	$5A8A
SCENE_GROUP_IDX	EQU	$5A8B
WAIT_LOOP_COUNT	EQU	$5A8C
TUNE_LOOP_CTR	EQU	$5A8D
TUNE_LOOP_ADDR	EQU	$5A8E
TUNE_GOSUB_ADDR	EQU	$5A90
TUNE_PRNG_MASK	EQU	$5A92
TUNE_ACTION_VEC	EQU	$5A93
TEXT_RIGHT_MARGIN	EQU	$5A96
TEXT_LEFT_MARGIN	EQU	$5A97
TEXT_STREAM_IDX	EQU	$5A98
SCENE_NUMBER	EQU	$5A99
SAVED_ENTITY_PTR	EQU	$5A9C
SAVED_MOB_PTR	EQU	$5AA0
BLINK_COL	EQU	$5AA2
BLINK_ROW	EQU	$5AA3
BLINK_CHAR	EQU	$5AA4
SAVED_AI_MODE	EQU	$5AA6
ATTRACT_FLAG	EQU	$5AA7
TURN_TUNE_GUARD	EQU	$5AA9
INPUT_OPTION_COUNT	EQU	$5AAA
CURRENT_SELECTION	EQU	$5AAB
INPUT_BUFFER	EQU	$5AAC
SCENE_TEXT_DATA	EQU	$5C00
STATS_TEMPLATE	EQU	$7D83
STATS_PROMPT_KEYBOARD	EQU	$7E01
STATS_PROMPT_JOYSTICK	EQU	$7E18
STATUS_BORDER_DATA	EQU	$7E2F
S_PRESS_SPACE	EQU	$7E97
S_PRESS_BUTTON	EQU	$7EBA
SHOP_TABLE	EQU	$7EDD
SHOP_EFFECT_TABLE	EQU	$7F6D
XP_AWARD_TABLE	EQU	$7FBE
S_A_WALL	EQU	$7FC3
S_A_STATUE	EQU	$7FCA
S_A_TREASURE_CHEST	EQU	$7FD3
S_EMPTY_SPACE	EQU	$7FE4
S_AN_ILLUSION	EQU	$7FF0
S_A_DOOR	EQU	$7FFC
S_AN_INVISIBLE_WALL	EQU	$8003
S_SOME_RUNE_WRITING	EQU	$8016
S_A_TRADING_OUTPOST	EQU	$8028
ATTACK_VERB_STRINGS	EQU	$803A
s_PRINT_FONT_CHAR	EQU	$80A4
TUNE_TABLE	EQU	$80AE
TUNE_0	EQU	$80C6
TUNE_1_COMBAT_HIT	EQU	$80C7
TUNE_2_COMBAT_DAMAGE	EQU	$80D7
FONT_DATA	EQU	$83A5
GAME_MESSAGE_STRINGS	EQU	$9AA5
DISK_IO_CODE	EQU	$B300
DISK_READ_FIELD	EQU	$B300
RWTS_ENTRY	EQU	$B7B5
RWTS_IOB	EQU	$B7E8
KBD	EQU	$C000
KBDSTRB	EQU	$C010
SPKR	EQU	$C030
TXTCLR	EQU	$C050
MIXCLR	EQU	$C052
TXTPAGE1	EQU	$C054
HIRES	EQU	$C057
BUTN	EQU	$C061
PADDL	EQU	$C064
PTRIG	EQU	$C070
DISK_MOTOR_ON	EQU	$C0E9
DISK_BOOT_ROM	EQU	$C600
SETPWRC	EQU	$FB6F
VTAB	EQU	$FC22
VTABZ	EQU	$FC24
CLREOP	EQU	$FC42
HOME	EQU	$FC58
SCROLL	EQU	$FC70
CLREOL	EQU	$FC9C
WAIT	EQU	$FCA8

; @zp-link-meta-begin
; def hrcg_advance_cursor params= locals= indirect=false in_cycle=false
; def hrcg_clear_from params=__zpabi_hrcg_clear_from__row,__zpabi_hrcg_clear_from__col locals=__local_hrcg_clear_from__row indirect=false in_cycle=false param_regs=A,X
; def hrcg_control params=__zpabi_hrcg_control__code locals=__local_hrcg_control__code indirect=false in_cycle=false param_regs=A
; def hrcg_cout params=__zpabi_hrcg_cout__c locals=__local_hrcg_cout__next,__local_hrcg_cout__0 indirect=false in_cycle=false param_regs=A
; def hrcg_draw_char params=__zpabi_hrcg_draw_char__c,__zpabi_hrcg_draw_char__raw locals=__local_hrcg_draw_char__glyph,__local_hrcg_draw_char__0,__local_hrcg_draw_char__dst,__local_hrcg_draw_char__1,__local_hrcg_draw_char__2,__local_hrcg_draw_char__3,__local_hrcg_draw_char__raw indirect=false in_cycle=false param_regs=A,X
; def hrcg_fill_line params=__zpabi_hrcg_fill_line__row,__zpabi_hrcg_fill_line__col locals=__local_hrcg_fill_line__row,__local_hrcg_fill_line__n,__local_hrcg_fill_line__line,__local_hrcg_fill_line__0,__local_hrcg_fill_line__col indirect=false in_cycle=false param_regs=A,X
; def hrcg_init params= locals= indirect=false in_cycle=false
; def hrcg_line_feed params= locals=__local_hrcg_line_feed__0 indirect=false in_cycle=false
; def hrcg_restore params= locals= indirect=false in_cycle=false
; def hrcg_restore_column params= locals=__local_hrcg_restore_column__old indirect=false in_cycle=false
; def hrcg_row_address params=__zpabi_hrcg_row_address__row,__zpabi_hrcg_row_address__col locals=__local_hrcg_row_address__0,__local_hrcg_row_address__1,__local_hrcg_row_address__row,__local_hrcg_row_address__2,__local_hrcg_row_address__3,__local_hrcg_row_address__col indirect=false in_cycle=false param_regs=A,X
; def hrcg_save params= locals= indirect=false in_cycle=false
; def hrcg_scroll_up params= locals=__local_hrcg_scroll_up__0,__local_hrcg_scroll_up__n,__local_hrcg_scroll_up__src,__local_hrcg_scroll_up__1,__local_hrcg_scroll_up__dst,__local_hrcg_scroll_up__2,__local_hrcg_scroll_up__row indirect=false in_cycle=false
; def hrcg_select_font params=__zpabi_hrcg_select_font__c locals=__local_hrcg_select_font__0,__local_hrcg_select_font__1 indirect=false in_cycle=false param_regs=A
; def hrcg_set_top_left params= locals=__local_hrcg_set_top_left__top indirect=false in_cycle=false
; def hrcg_shifted_control params=__zpabi_hrcg_shifted_control__code locals=__local_hrcg_shifted_control__code indirect=false in_cycle=false param_regs=A
; ext CLREOL params=
; ext CLREOP params=
; ext DISK_BOOT_ROM params=
; ext DISK_READ_FIELD params= carry=true
; ext HOME params=
; ext RWTS_ENTRY params=__zpabi_RWTS_ENTRY__iob_0,__zpabi_RWTS_ENTRY__iob_1 param_regs=Y,A carry=true
; ext SCROLL params=
; ext SETPWRC params=
; ext VTAB params=
; ext VTABZ params=__zpabi_VTABZ__row param_regs=A
; ext WAIT params=__zpabi_WAIT__a param_regs=A
; ext adj_drop_dec params=
; ext adj_drop_inc params=
; ext ai_choose_target params=
; ext anim_check_frame params=
; ext anim_tick_and_wait params=
; ext appearance_to_fontchar params=__zpabi_appearance_to_fontchar__appearance param_regs=A
; ext apply_damage_to_current params=__zpabi_apply_damage_to_current__damage param_regs=A
; ext attack_down params=
; ext attack_left params=
; ext attack_right params=
; ext attack_up params=
; ext auto_heal params=
; ext border_anim_step params=
; ext browse_handler params=
; ext cancel_scenario params=
; ext char_select_here params=
; ext char_select_home params=
; ext char_select_init params=
; ext char_select_next params=
; ext char_select_prev params=
; ext check_adjacent_threats params=__zpabi_check_adjacent_threats__pos param_regs=A
; ext check_at_boundary params=__zpabi_check_at_boundary__pos param_regs=A
; ext check_attack_eligible params= carry=true
; ext check_can_leave params= carry=true
; ext check_encounter params=__zpabi_check_encounter__pos param_regs=A
; ext check_hostile params=__zpabi_check_hostile__other_0,__zpabi_check_hostile__other_1 param_regs=A,X carry=true
; ext check_scene_events params=
; ext check_threats_here params=
; ext clamp_char_field params=
; ext classify_entity_type params=__zpabi_classify_entity_type__e_0,__zpabi_classify_entity_type__e_1 param_regs=A,X
; ext clear_map params=
; ext clear_restricted params=
; ext cold_restart params=
; ext colrow_to_pos params=__zpabi_colrow_to_pos__col,__zpabi_colrow_to_pos__row param_regs=A,X
; ext commit_move params=
; ext configure_input params=
; ext continue_game params=
; ext dec_threshold_wrap params=
; ext delay_with_animation params=
; ext disk_read params= carry=true
; ext disk_write params= carry=true
; ext dispatch_on_mode params=
; ext display_char_stats params=
; ext display_message params=__zpabi_display_message__index param_regs=A
; ext display_message_loop params=__zpabi_display_message_loop__msg_0,__zpabi_display_message_loop__msg_1 param_regs=A,X
; ext display_shop params=__zpabi_display_shop__ev_0,__zpabi_display_shop__ev_1 param_regs=A,X
; ext display_status_bar params=
; ext draw_blink_alt params=
; ext draw_blink_normal params=
; ext draw_char_at_pos params=__zpabi_draw_char_at_pos__pos param_regs=A
; ext drop_gold_exec params=
; ext enter_char_select params=
; ext enter_combat_state params=__zpabi_enter_combat_state__pos param_regs=A
; ext enter_location params=
; ext enter_save_game_slot params=
; ext enter_scenario_disk params=
; ext fill_map params=
; ext find_attack_target params=
; ext find_entity_at_pos params=__zpabi_find_entity_at_pos__pos param_regs=A
; ext find_first_pc params= carry=true
; ext find_nearest_event params=
; ext find_next_pc_wrap params=
; ext find_prev_pc params=
; ext first_group_member params=
; ext flush_pending_xp params=
; ext font_pos_to_text_pos params=
; ext format_scenario_disk params=
; ext game_init params=
; ext get_char_record params=__zpabi_get_char_record__index param_regs=A
; ext get_entity_fontchar params=__zpabi_get_entity_fontchar__e_0,__zpabi_get_entity_fontchar__e_1 param_regs=A,X
; ext get_input_table_ptr params=
; ext get_mob_data params=__zpabi_get_mob_data__index param_regs=A
; ext handle_encounter params=__zpabi_handle_encounter__ev_0,__zpabi_handle_encounter__ev_1 param_regs=A,X
; ext handle_event_slot params=__zpabi_handle_event_slot__ev_0,__zpabi_handle_event_slot__ev_1 param_regs=A,X
; ext handle_mob_encounter params=
; ext idle_anim_handler params=
; ext inc_threshold_wrap params=
; ext init_attack_display params=
; ext init_char_select params=
; ext init_drop_display params=
; ext init_drop_gold params=
; ext init_world params=
; ext input_loop params=__zpabi_input_loop__m_0,__zpabi_input_loop__m_1 param_regs=A,X
; ext integrity_fail params=
; ext load_char_name_ptr params=
; ext load_char_to_fa params=__zpabi_load_char_to_fa__index param_regs=A
; ext main params=
; ext manhattan_distance params=__zpabi_manhattan_distance__pos,__zpabi_manhattan_distance__from param_regs=A,X
; ext menu_attack_target params=
; ext menu_check_attack params=
; ext menu_drop_dec params=
; ext menu_drop_gold params=
; ext menu_drop_inc params=
; ext menu_game_settings params=
; ext menu_game_settings_enter params=
; ext menu_other_options params=
; ext menu_see_your_lot params=
; ext modify_current_stats params=__zpabi_modify_current_stats__amount_0,__zpabi_modify_current_stats__amount_1,__zpabi_modify_current_stats__add param_regs=A,X,
; ext next_group_member params=
; ext npc_move_ai params=
; ext num_to_decimal params=__zpabi_num_to_decimal__value_0,__zpabi_num_to_decimal__value_1,__zpabi_num_to_decimal__out_0,__zpabi_num_to_decimal__out_1 param_regs=A,X,,
; ext pick_random_member params=
; ext pickup_treasure params=__zpabi_pickup_treasure__ev_0,__zpabi_pickup_treasure__ev_1 param_regs=A,X
; ext pos_to_colrow params=__zpabi_pos_to_colrow__pos param_regs=A
; ext print_bottom_centered params=__zpabi_print_bottom_centered__s_0,__zpabi_print_bottom_centered__s_1 param_regs=A,X
; ext print_centered_plus_col params=__zpabi_print_centered_plus_col__s_0,__zpabi_print_centered_plus_col__s_1 param_regs=A,X
; ext print_ctrl_ab params=
; ext print_ctrl_n params=
; ext print_entity_name params=__zpabi_print_entity_name__m_0,__zpabi_print_entity_name__m_1 param_regs=A,X
; ext print_fontchar_at_text_pos params=
; ext print_member_name params=
; ext print_mob_name params=
; ext print_string params=__zpabi_print_string__s_0,__zpabi_print_string__s_1 param_regs=A,X
; ext random_events params=
; ext random_in_range params=__zpabi_random_in_range__limit param_regs=A
; ext read_keyboard params=
; ext render_font_char params=
; ext render_font_char_direct params=__zpabi_render_font_char_direct__charset_sel param_regs=A
; ext reorder_char params=__zpabi_reorder_char__m_0,__zpabi_reorder_char__m_1 param_regs=A,X
; ext reorder_current_char params=
; ext reset_turn_pos params=
; ext resolve_attack params=
; ext retire_character params=
; ext run_chain params=__zpabi_run_chain__state param_regs=A
; ext rwts_dispatch params=__zpabi_rwts_dispatch__command param_regs=A carry=true
; ext save_game params=
; ext scene_loop params=__zpabi_scene_loop__scene param_regs=A
; ext scene_setup params=
; ext scroll_status_line params=
; ext select_combat_target params=
; ext select_someone_else params=
; ext set_blink_target params=
; ext set_char_state params=__zpabi_set_char_state__state param_regs=A
; ext set_cursor_row21 params=
; ext set_inverse_video params=
; ext set_restricted params=
; ext set_text_window_bottom params=
; ext set_text_window_wrap params=
; ext settings_danger_down params=
; ext settings_danger_up params=
; ext settings_display_init params=
; ext settings_speed_down params=
; ext settings_speed_up params=
; ext setup_char_sprite params=
; ext setup_input params=__zpabi_setup_input__m_0,__zpabi_setup_input__m_1 param_regs=A,X
; ext setup_scene_display params=
; ext setup_text_pos params=__zpabi_setup_text_pos__name_0,__zpabi_setup_text_pos__name_1 param_regs=A,X
; ext setup_text_pos_col20 params=__zpabi_setup_text_pos_col20__name_0,__zpabi_setup_text_pos_col20__name_1 param_regs=A,X
; ext shop_buy_handler params=
; ext show_locked_msg params=
; ext slot_dec_enter params=
; ext slot_dec_reenter params=
; ext slot_inc_enter params=
; ext slot_inc_reenter params=
; ext start_combat params=
; ext start_game_input params=
; ext step_prng params=
; ext teardown_input params=
; ext text_renderer params=
; ext text_renderer_no_pause params=
; ext tick_anim_counter params=
; ext timed_wait params=
; ext toggle_sound params=
; ext trigger_scene_event params=__zpabi_trigger_scene_event__ev_0,__zpabi_trigger_scene_event__ev_1 param_regs=A,X
; ext tune_player params=__zpabi_tune_player__tune,__zpabi_tune_player__strength param_regs=A,X
; ext update_event_list params=__zpabi_update_event_list__pos,__zpabi_update_event_list__value_0,__zpabi_update_event_list__value_1 param_regs=A,,
; ext use_scenario_disk params=
; ext verify_disk_io params=
; ext verify_integrity params=
; call hrcg_advance_cursor -> hrcg_line_feed
; call hrcg_clear_from -> hrcg_fill_line
; call hrcg_control -> CLREOP
; call hrcg_control -> HOME
; call hrcg_control -> hrcg_clear_from
; call hrcg_control -> hrcg_restore
; call hrcg_control -> hrcg_restore_column
; call hrcg_control -> hrcg_save
; call hrcg_control -> hrcg_set_top_left
; call hrcg_cout -> hrcg_control
; call hrcg_cout -> hrcg_draw_char
; call hrcg_cout -> hrcg_select_font
; call hrcg_cout -> hrcg_shifted_control
; call hrcg_draw_char -> hrcg_advance_cursor
; call hrcg_draw_char -> hrcg_row_address
; call hrcg_fill_line -> hrcg_row_address
; call hrcg_init -> VTAB
; call hrcg_line_feed -> SCROLL
; call hrcg_line_feed -> VTABZ
; call hrcg_line_feed -> hrcg_scroll_up
; call hrcg_restore_column -> hrcg_line_feed
; call hrcg_scroll_up -> hrcg_fill_line
; call hrcg_scroll_up -> hrcg_row_address
; call hrcg_set_top_left -> VTAB
; item hrcg_row_address section=.text local=true zero=false
; item hrcg_fill_line section=.text local=true zero=false
; item hrcg_clear_from section=.text local=true zero=false
; item hrcg_scroll_up section=.text local=true zero=false
; item hrcg_line_feed section=.text local=true zero=false
; item hrcg_advance_cursor section=.text local=true zero=false
; item hrcg_draw_char section=.text local=true zero=false
; item hrcg_select_font section=.text local=true zero=false
; item hrcg_select_font@run0 section=.text local=true zero=false
; item hrcg_save section=.text local=true zero=false
; item hrcg_restore_column section=.text local=true zero=false
; item hrcg_restore section=.text local=true zero=false
; item hrcg_set_top_left section=.text local=true zero=false
; item hrcg_shifted_control section=.text local=true zero=false
; item hrcg_control section=.text local=true zero=false
; item hrcg_init section=.text local=false zero=false
; item hrcg_cout section=.text local=false zero=false
; item hrcg section=.bss local=true zero=true
; item hrcg_saved section=.bss local=true zero=true
; item STD_FONT_DATA section=.rodata local=true zero=false
; item FONT_DATA@lo section=.rodata local=true zero=false merge=true
; item FONT_DATA@hi section=.rodata local=true zero=false merge=true
; item __zp_init section=.text local=false zero=false
; zp NULL_PAGE_LOW size=6 at=$00 def=false init=
; zp TEXT_STREAM_PTR size=2 at=$06 def=false init=
; zp RWTS_IOB_PTR size=2 at=$08 def=false init=
; zp NULL_PAGE_HIGH size=6 at=$0A def=false init=
; zp MONITOR_YSAV1_1F size=1 at=$1F def=false init=
; zp WNDLFT size=1 at=$20 def=false init=
; zp WNDWDTH size=1 at=$21 def=false init=
; zp WNDTOP size=1 at=$22 def=false init=
; zp WNDBTM size=1 at=$23 def=false init=
; zp TEXT_COL size=1 at=$24 def=false init=
; zp TEXT_ROW size=1 at=$25 def=false init=
; zp MONITOR_GBAS size=2 at=$26 def=false init=
; zp BASL size=2 at=$28 def=false init=
; zp DOS_ZP size=38 at=$2A def=false init=
; zp DISK_VM_ZP size=16 at=$50 def=false init=
; zp hrcg_row_address@ret size=2 at=auto def=true init= local=true
; zp hrcg_saved_valid size=1 at=auto def=true init=$00 weight=6 local=true
; zp hrcg_saved_ch size=1 at=auto def=true init=$00 weight=5 local=true
; zp hrcg_next size=1 at=auto def=true init=$00 weight=4 local=true
; zp hrcg_saved_cv size=1 at=auto def=true init=$00 weight=4 local=true
; zp MENU_DESC_PTR size=2 at=$C0 def=false init=
; zp MENU_OPTION_PTR size=2 at=$C2 def=false init=
; zp TUNE_PC size=2 at=$E4 def=false init=
; zp MOB_PTR size=2 at=$F4 def=false init=
; zp ENTITY_PTR size=2 at=$F8 def=false init=
; zp CHAR_PTR size=2 at=$FA def=false init=
; runtime hargs_size=11 hargs_need=0
; @zp-link-meta-end

hrcg_row_address:
   SUBROUTINE

   STA   __local_hrcg_row_address__row
   STX   __local_hrcg_row_address__col
   AND   #$07
   LSR
   STA   __local_hrcg_row_address__3
   LDA   #$00
   ROR
   STA   __local_hrcg_row_address__1
   LDA   __local_hrcg_row_address__row
   LSR
   LSR
   LSR
   ASL
   ASL
   ASL
   STA   __local_hrcg_row_address__0
   ASL
   STA   __local_hrcg_row_address__row
   LDA   #$00
   ROL
   ASL   __local_hrcg_row_address__row
   ROL
   STA   __local_hrcg_row_address__2
   LDA   __local_hrcg_row_address__row
   CLC
   ADC   __local_hrcg_row_address__0
   BCC   .carry_inc@0
   INC   __local_hrcg_row_address__2
   CLC
.carry_inc@0:
   ADC   __local_hrcg_row_address__1
   STA   __local_hrcg_row_address__1
   LDA   __local_hrcg_row_address__2
   ADC   __local_hrcg_row_address__3
   CLC
   ADC   #$20
   STA   __local_hrcg_row_address__2
   LDA   __local_hrcg_row_address__1
   CLC
   ADC   WNDLFT
   CLC
   ADC   __local_hrcg_row_address__col
   STA   hrcg_row_address@ret
   LDA   __local_hrcg_row_address__2
   STA   hrcg_row_address@ret+1
   RTS

hrcg_fill_line:
   SUBROUTINE

   STX   __local_hrcg_fill_line__col
   LDX   #$00
   JSR   hrcg_row_address
   LDX   #$08
.hrcg_fill_line@rot@0:
   LDY   __local_hrcg_fill_line__col
.loop@1_start:
   LDA   hrcg
   STA   (hrcg_row_address@ret),Y
   INY
   CPY   WNDWDTH
   BCC   .loop@1_start
   LDA   hrcg_row_address@ret+1
   CLC
   ADC   #$04
   STA   hrcg_row_address@ret+1
   DEX
   BNE   .hrcg_fill_line@rot@0
   RTS

hrcg_clear_from:
   SUBROUTINE

   STA   __local_hrcg_clear_from__row
.loop@2_start:
   JSR   hrcg_fill_line
   INC   __local_hrcg_clear_from__row
   LDA   __local_hrcg_clear_from__row
   CMP   WNDBTM
   BCC   .hrcg_clear_from@asm_ssa_split@0
   RTS
.hrcg_clear_from@asm_ssa_split@0:
   LDX   #$00
   BEQ   .loop@2_start

hrcg_scroll_up:
   SUBROUTINE

   LDA   WNDTOP
   STA   __local_hrcg_scroll_up__row
.loop@3_start:
   LDA   __local_hrcg_scroll_up__row
   CLC
   ADC   #$01
   STA   hrcg_row_address@ret
   LDA   #$00
   ADC   #$00
   STA   __local_hrcg_scroll_up__n
   LDA   hrcg_row_address@ret
   SEC
   SBC   WNDBTM
   LDA   __local_hrcg_scroll_up__n
   SBC   #$00
   BCS   .loop@3_break
   LDX   #$00
   LDA   __local_hrcg_scroll_up__row
   JSR   hrcg_row_address
   LDA   hrcg_row_address@ret
   STA   __local_hrcg_scroll_up__dst
   LDA   hrcg_row_address@ret+1
   STA   __local_hrcg_scroll_up__2
   INC   __local_hrcg_scroll_up__row
   LDX   #$00
   LDA   __local_hrcg_scroll_up__row
   JSR   hrcg_row_address
   LDA   #$08
   STA   __local_hrcg_scroll_up__n
.loop@4_start:
   LDA   __local_hrcg_scroll_up__n
   BEQ   .loop@3_start
   LDY   #$00
   BEQ   .loop@5_start
.hrcg_scroll_up@rot@1:
   LDA   (hrcg_row_address@ret),Y
   STA   (__local_hrcg_scroll_up__dst),Y
   INY
.loop@5_start:
   CPY   WNDWDTH
   BCC   .hrcg_scroll_up@rot@1
   LDA   __local_hrcg_scroll_up__2
   CLC
   ADC   #$04
   STA   __local_hrcg_scroll_up__2
   LDA   hrcg_row_address@ret+1
   CLC
   ADC   #$04
   STA   hrcg_row_address@ret+1
   DEC   __local_hrcg_scroll_up__n
   JMP   .loop@4_start
.loop@3_break:
   LDA   WNDBTM
   SBC   #$01
   LDX   #$00
   JMP   hrcg_fill_line

hrcg_line_feed:
   SUBROUTINE

   INC   TEXT_ROW
   LDA   TEXT_ROW
   CMP   WNDBTM
   BCS   .if_else@1
   JMP   VTABZ
.if_else@1:
   LDA   hrcg+1
   BEQ   .if_else@3
   LDA   WNDTOP
   STA   TEXT_ROW
   JMP   VTABZ
.if_else@3:
   DEC   TEXT_ROW
   JSR   hrcg_scroll_up
   JMP   SCROLL

hrcg_advance_cursor:
   SUBROUTINE

   INC   TEXT_COL
   LDA   TEXT_COL
   CMP   WNDWDTH
   BCS   .if_end@4
   RTS
.if_end@4:
   LDA   #$00
   STA   TEXT_COL
   JMP   hrcg_line_feed

hrcg_draw_char:
   SUBROUTINE

   STA   __local_hrcg_draw_char__glyph
   STX   __local_hrcg_draw_char__raw
   SEC
   SBC   #$20
   STA   hrcg_row_address@ret
   LDA   hrcg+2
   BEQ   .hrcg_draw_char@scfold@0
   LDA   __local_hrcg_draw_char__glyph
   CMP   #$41
   BCC   .hrcg_draw_char@scfold@0
   CMP   #$5B
   BCS   .hrcg_draw_char@scfold@0
.if_end@9:
   LSR
   LSR
   LSR
   LSR
   LSR
   STA   __local_hrcg_draw_char__0
   LDA   __local_hrcg_draw_char__glyph
   ASL
   ASL
   ASL
   CLC
   ADC   hrcg+3
   STA   __local_hrcg_draw_char__2
   LDA   hrcg+4
   ADC   __local_hrcg_draw_char__0
   STA   __local_hrcg_draw_char__3
   LDX   TEXT_COL
   LDA   TEXT_ROW
   JSR   hrcg_row_address
   LDA   __local_hrcg_draw_char__raw
   LDY   TEXT_COL
   STA   (BASL),Y
   LDX   #$00
   BEQ   .loop@6_start
.hrcg_draw_char@rot@2:
   TXA
   TAY
   LDA   (__local_hrcg_draw_char__2),Y
   EOR   hrcg
   LDY   #$00
   STA   (hrcg_row_address@ret),Y
   LDA   hrcg_row_address@ret+1
   ADC   #$04
   STA   hrcg_row_address@ret+1
   INX
.loop@6_start:
   CPX   #$08
   BCC   .hrcg_draw_char@rot@2
   JMP   hrcg_advance_cursor
.hrcg_draw_char@scfold@0:
   LDA   hrcg_row_address@ret
   STA   __local_hrcg_draw_char__glyph
   JMP   .if_end@9

hrcg_select_font:
   SUBROUTINE

   TAX
   CPX   #$31
   BCC   .if_else@13
   CPX   #$3A
   BCS   .if_else@13
   LDA   FONT_DATA@lo-49,X
   STA   hrcg+3
   LDA   FONT_DATA@hi-49,X
   STA   hrcg+4
   RTS
.if_else@13:
   JSR   hrcg_select_font@run0
   RTS

hrcg_select_font@run0:
   SUBROUTINE

   LDA   #<STD_FONT_DATA
   STA   hrcg+3
   LDA   #>STD_FONT_DATA
   STA   hrcg+4
   RTS

hrcg_save:
   SUBROUTINE

   LDA   hrcg_saved_valid
   BNE   .if_end@14
   LDA   #$01
   STA   hrcg_saved_valid
   LDA   hrcg
   STA   hrcg_saved
   LDA   hrcg+1
   STA   hrcg_saved+1
   LDA   hrcg+2
   STA   hrcg_saved+2
   LDA   hrcg+3
   STA   hrcg_saved+3
   LDA   hrcg+4
   STA   hrcg_saved+4
.if_end@14:
   LDA   TEXT_COL
   STA   hrcg_saved_ch
   LDA   TEXT_ROW
   STA   hrcg_saved_cv
   RTS

hrcg_restore_column:
   SUBROUTINE

   LDA   TEXT_COL
   LDX   hrcg_saved_ch
   STX   TEXT_COL
   CMP   hrcg_saved_ch
   BCC   .if_end@15
   JSR   hrcg_line_feed
.if_end@15:
   RTS

hrcg_restore:
   SUBROUTINE

   LDA   hrcg_saved_valid
   BNE   .if_end@16
   RTS
.if_end@16:
   LDA   hrcg_saved
   STA   hrcg
   LDA   hrcg_saved+1
   STA   hrcg+1
   LDA   hrcg_saved+2
   STA   hrcg+2
   LDA   hrcg_saved+3
   STA   hrcg+3
   LDA   hrcg_saved+4
   STA   hrcg+4
   LDA   #$00
   STA   hrcg_saved_valid
   STA   hrcg_saved_ch
   LDA   WNDTOP
   STA   hrcg_saved_cv
   RTS

hrcg_set_top_left:
   SUBROUTINE

   LDA   TEXT_COL
   CLC
   ADC   WNDLFT
   CMP   #$28
   BCC   .if_end@17
   LDA   #$27
.if_end@17:
   STA   WNDLFT
   LDA   WNDWDTH
   SEC
   SBC   TEXT_COL
   STA   WNDWDTH
   LDA   #$00
   STA   TEXT_COL
   LDA   TEXT_ROW
   STA   __local_hrcg_set_top_left__top
   CMP   #$18
   BCC   .if_end@18
   LDA   #$17
   STA   __local_hrcg_set_top_left__top
.if_end@18:
   STA   WNDTOP
   LDA   hrcg_saved_valid
   BNE   .if_end@19
   LDA   __local_hrcg_set_top_left__top
   STA   hrcg_saved_cv
.if_end@19:
   JMP   VTAB

hrcg_shifted_control:
   SUBROUTINE

   CMP   #$13
   BNE   .if_else@21
   LDA   #$00
   STA   hrcg+1
   RTS
.if_else@21:
   CMP   #$17
   BNE   .if_end@22
   LDA   #$01
   STA   hrcg+1
.if_end@22:
   RTS

hrcg_control:
   SUBROUTINE

   CMP   #$01
   BEQ   .case@8
   CMP   #$02
   BEQ   .case@9
   CMP   #$03
   BEQ   .case@10
   CMP   #$04
   BEQ   .case@11
   CMP   #$06
   BEQ   .case@12
   CMP   #$09
   BEQ   .case@13
   CMP   #$0B
   BEQ   .case@14
   CMP   #$0C
   BEQ   .case@15
   CMP   #$0E
   BEQ   .case@16
   CMP   #$0F
   BEQ   .case@17
   CMP   #$10
   BEQ   .case@18
   CMP   #$16
   BEQ   .case@19
   RTS
.case@8:
   LDA   #$80
   STA   hrcg_next
   RTS
.case@9:
   JMP   hrcg_save
.case@10:
   JMP   hrcg_restore_column
.case@11:
   JMP   hrcg_restore
.case@12:
   LDX   TEXT_COL
   LDA   TEXT_ROW
   JSR   hrcg_clear_from
   JMP   CLREOP
.case@13:
   LDA   #$7F
   STA   hrcg
   RTS
.case@14:
   LDA   #$00
   STA   hrcg+2
   RTS
.case@15:
   LDA   #$01
   STA   hrcg+2
   RTS
.case@16:
   LDA   #$00
   STA   hrcg
   RTS
.case@17:
   LDA   #$40
   STA   hrcg_next
   RTS
.case@18:
   LDA   WNDTOP
   STA   TEXT_ROW
   LDA   #$00
   STA   TEXT_COL
   LDX   #$00
   LDA   WNDTOP
   JSR   hrcg_clear_from
   JMP   HOME
.case@19:
   JMP   hrcg_set_top_left

hrcg_init:
   SUBROUTINE

   LDA   MIXCLR
   LDA   HIRES
   LDA   #$00
   STA   hrcg
   LDY   #$00
   STY   hrcg+1
   STA   hrcg+2
   STA   hrcg_saved_valid
   STA   hrcg_saved_ch
   STA   hrcg_saved_cv
   LDA   TEXT_COL
   CLC
   ADC   WNDLFT
   STA   TEXT_COL
   LDA   #$00
   STA   WNDLFT
   STA   WNDTOP
   LDA   #$28
   STA   WNDWDTH
   LDA   #$18
   STA   WNDBTM
   JSR   VTAB
   JSR   hrcg_select_font@run0
   LDA   TXTPAGE1
   LDA   TXTCLR
   RTS

hrcg_cout:
   SUBROUTINE

   TAX
   AND   #$7F
   STA   __local_hrcg_cout__0
   LDA   hrcg_next
   STA   __local_hrcg_cout__next
   LDY   #$00
   STY   hrcg_next
   CMP   #$80
   BNE   .if_else@24
   LDA   __local_hrcg_cout__0
   JMP   hrcg_select_font
.if_else@24:
   LDA   __local_hrcg_cout__0
   CMP   #$20
   BCC   .if_else@26
   LDA   __local_hrcg_cout__next
   BNE   .if_end@25
   LDA   __local_hrcg_cout__0
   JMP   hrcg_draw_char
.if_else@26:
   CMP   #$1B
   BCS   .if_end@28
   LDA   __local_hrcg_cout__next
   CMP   #$40
   BNE   .if_else@30
   LDA   __local_hrcg_cout__0
   JMP   hrcg_shifted_control
.if_else@30:
   LDA   __local_hrcg_cout__0
   JSR   hrcg_control
.if_end@28:
.if_end@25:
   RTS

hrcg:
   DS.B  5

hrcg_saved:
   DS.B  5

STD_FONT_DATA:
   DC.B  $00, $00, $00, $00, $00, $00, $00, $00, $08, $08, $08, $08, $08, $00, $08, $00
   DC.B  $14, $14, $14, $00, $00, $00, $00, $00, $08, $1C, $3E, $7F, $1C, $1C, $1C, $1C
   DC.B  $08, $3C, $0A, $1C, $28, $1E, $08, $00, $06, $26, $10, $08, $04, $32, $30, $00
   DC.B  $04, $0A, $0A, $04, $2A, $12, $2C, $00, $08, $08, $08, $00, $00, $00, $00, $00
   DC.B  $08, $04, $02, $02, $02, $04, $08, $00, $08, $10, $20, $20, $20, $10, $08, $00
   DC.B  $08, $2A, $1C, $08, $1C, $2A, $08, $00, $00, $08, $08, $3E, $08, $08, $00, $00
   DC.B  $00, $00, $00, $00, $00, $8C, $88, $84, $00, $00, $00, $3E, $00, $00, $00, $00
   DC.B  $00, $00, $00, $00, $00, $00, $8C, $00, $00, $20, $10, $08, $04, $02, $00, $00
   DC.B  $8C, $92, $22, $22, $22, $92, $8C, $00, $08, $0C, $08, $08, $08, $08, $1C, $00
   DC.B  $1C, $22, $20, $10, $84, $82, $3E, $00, $3E, $90, $10, $18, $20, $22, $1C, $00
   DC.B  $10, $18, $14, $12, $3E, $10, $10, $00, $3E, $02, $1E, $20, $20, $22, $1C, $00
   DC.B  $18, $04, $82, $1E, $22, $22, $1C, $00, $3E, $90, $10, $88, $08, $84, $04, $00
   DC.B  $1C, $22, $22, $1C, $22, $22, $1C, $00, $1C, $22, $22, $3C, $90, $10, $0C, $00
   DC.B  $00, $00, $8C, $00, $00, $8C, $00, $00, $00, $00, $8C, $00, $00, $8C, $88, $84
   DC.B  $08, $0C, $7E, $7F, $7E, $0C, $08, $00, $00, $00, $3E, $00, $3E, $00, $00, $00
   DC.B  $08, $18, $3F, $7F, $3F, $18, $08, $00, $1C, $22, $90, $88, $08, $00, $08, $00
   DC.B  $1C, $1C, $1C, $1C, $7F, $3E, $1C, $08, $08, $8C, $14, $92, $3E, $22, $22, $00
   DC.B  $1E, $22, $22, $1E, $22, $22, $1E, $00, $1C, $22, $02, $02, $02, $22, $1C, $00
   DC.B  $1E, $22, $22, $22, $22, $22, $1E, $00, $3E, $02, $02, $1E, $02, $02, $3E, $00
   DC.B  $3E, $02, $02, $1E, $02, $02, $02, $00, $3C, $02, $02, $02, $32, $22, $3C, $00
   DC.B  $22, $22, $22, $3E, $22, $22, $22, $00, $1C, $08, $08, $08, $08, $08, $1C, $00
   DC.B  $20, $20, $20, $20, $20, $22, $1C, $00, $22, $12, $0A, $06, $0A, $12, $22, $00
   DC.B  $02, $02, $02, $02, $02, $02, $3E, $00, $22, $36, $2A, $2A, $22, $22, $22, $00
   DC.B  $22, $22, $26, $2A, $32, $22, $22, $00, $1C, $22, $22, $22, $22, $22, $1C, $00
   DC.B  $1E, $22, $22, $1E, $02, $02, $02, $00, $1C, $22, $22, $22, $2A, $12, $2C, $00
   DC.B  $1E, $22, $22, $1E, $0A, $12, $22, $00, $1C, $22, $02, $1C, $20, $22, $1C, $00
   DC.B  $3E, $08, $08, $08, $08, $08, $08, $00, $22, $22, $22, $22, $22, $22, $1C, $00
   DC.B  $22, $22, $22, $92, $14, $8C, $08, $00, $22, $22, $22, $2A, $2A, $36, $22, $00
   DC.B  $22, $92, $14, $08, $14, $92, $22, $00, $22, $92, $14, $08, $08, $08, $08, $00
   DC.B  $3E, $90, $10, $08, $04, $82, $3E, $00, $3E, $06, $06, $06, $06, $06, $3E, $00
   DC.B  $00, $02, $04, $08, $10, $20, $00, $00, $3E, $30, $30, $30, $30, $30, $3E, $00
   DC.B  $00, $00, $08, $14, $22, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $7F
   DC.B  $04, $08, $10, $00, $00, $00, $00, $00, $00, $00, $1C, $20, $3C, $22, $3C, $00
   DC.B  $02, $02, $1E, $22, $22, $22, $1E, $00, $00, $00, $3C, $02, $02, $02, $3C, $00
   DC.B  $20, $20, $3C, $22, $22, $22, $3C, $00, $00, $00, $1C, $22, $3E, $02, $3C, $00
   DC.B  $18, $24, $04, $1E, $04, $04, $04, $00, $00, $00, $1C, $22, $22, $3C, $20, $1C
   DC.B  $02, $02, $1E, $22, $22, $22, $22, $00, $08, $00, $0C, $08, $08, $08, $1C, $00
   DC.B  $10, $00, $18, $10, $10, $10, $12, $0C, $02, $02, $22, $12, $0E, $12, $22, $00
   DC.B  $0C, $08, $08, $08, $08, $08, $1C, $00, $00, $00, $36, $2A, $2A, $2A, $22, $00
   DC.B  $00, $00, $1E, $22, $22, $22, $22, $00, $00, $00, $1C, $22, $22, $22, $1C, $00
   DC.B  $00, $00, $1E, $22, $22, $1E, $02, $02, $00, $00, $3C, $22, $22, $3C, $20, $20
   DC.B  $00, $00, $3A, $06, $02, $02, $02, $00, $00, $00, $3C, $02, $1C, $20, $1E, $00
   DC.B  $04, $04, $1E, $04, $04, $24, $18, $00, $00, $00, $22, $22, $22, $32, $2C, $00
   DC.B  $00, $00, $22, $92, $14, $8C, $08, $00, $00, $00, $22, $22, $2A, $2A, $36, $00
   DC.B  $00, $00, $22, $14, $08, $14, $22, $00, $00, $00, $22, $92, $14, $8C, $08, $86
   DC.B  $00, $00, $3E, $10, $08, $04, $3E, $00, $38, $0C, $0C, $06, $0C, $0C, $38, $00
   DC.B  $08, $08, $08, $08, $08, $08, $08, $08, $0E, $18, $18, $30, $18, $18, $0E, $00
   DC.B  $2C, $1A, $00, $00, $00, $00, $00, $00, $7F, $7F, $7F, $7F, $7F, $7F, $7F, $7F

FONT_DATA@lo:
   DC.B  $A5, $A5, $A5, $A5, $A5, $A5

FONT_DATA@hi:
   DC.B  $83, $86, $89, $8C, $8F, $92

__zp_init:
   SUBROUTINE

   LDA   #$00
   STA   hrcg_saved_valid
   STA   hrcg_saved_ch
   STA   hrcg_saved_cv
   STA   hrcg_next
   RTS
