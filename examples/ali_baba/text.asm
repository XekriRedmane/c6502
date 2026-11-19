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
next_string@ret	EQU	$62
__local_display_message__msg	EQU	$80
__local_text_renderer__stream_1	EQU	$80
__zpabi_modify_current_stats__add	EQU	$80
__zpabi_num_to_decimal__out_0	EQU	$80
__zpabi_update_event_list__value_0	EQU	$80
__local_display_message__0	EQU	$81
__local_text_renderer__stream_0	EQU	$81
__zpabi_num_to_decimal__out_1	EQU	$81
__zpabi_update_event_list__value_1	EQU	$81
__local_display_message__1	EQU	$82
__local_text_renderer__at_0	EQU	$82
__local_display_message__index	EQU	$83
__local_num_to_decimal__value_1	EQU	$83
__local_num_to_decimal__value_0	EQU	$84
__local_display_message_loop__msg_0	EQU	$85
__local_num_to_decimal__place_1	EQU	$85
__local_render_stream__i	EQU	$85
__local_display_message_loop__msg_1	EQU	$86
__local_num_to_decimal__place_0	EQU	$86
__local_num_to_decimal__started	EQU	$87
__local_next_string__s	EQU	$88
__local_num_to_decimal__out	EQU	$88
__local_print_bottom_centered__s_0	EQU	$88
__local_next_string__1	EQU	$89
__local_num_to_decimal__1	EQU	$89
__local_print_centered_plus_col__0	EQU	$89
__local_print_centered_plus_col__1	EQU	$8A
__local_print_centered_plus_col__s	EQU	$8B
__local_print_centered_plus_col__2	EQU	$8C
__local_print_string__i	EQU	$8E
__local_print_string__1	EQU	$8F
__local_print_string__s	EQU	$90
__local_print_string__2	EQU	$91
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
; def display_message params=__zpabi_display_message__index locals=__local_display_message__msg,__local_display_message__0,__local_display_message__1,__local_display_message__index indirect=false in_cycle=false param_regs=A
; def display_message_loop params=__zpabi_display_message_loop__msg_0,__zpabi_display_message_loop__msg_1 locals=__local_display_message_loop__count,__local_display_message_loop__msg_0,__local_display_message_loop__msg_1 indirect=false in_cycle=false param_regs=A,X
; def next_string params=__zpabi_next_string__s_0,__zpabi_next_string__s_1 locals=__local_next_string__0,__local_next_string__s,__local_next_string__1 indirect=false in_cycle=false param_regs=A,X
; def num_to_decimal params=__zpabi_num_to_decimal__value_0,__zpabi_num_to_decimal__value_1,__zpabi_num_to_decimal__out_0,__zpabi_num_to_decimal__out_1 locals=__local_num_to_decimal__0,__local_num_to_decimal__value_1,__local_num_to_decimal__value_0,__local_num_to_decimal__place_1,__local_num_to_decimal__place_0,__local_num_to_decimal__started,__local_num_to_decimal__out,__local_num_to_decimal__1 indirect=false in_cycle=false param_regs=A,X,,
; def print_bottom_centered params=__zpabi_print_bottom_centered__s_0,__zpabi_print_bottom_centered__s_1 locals=__local_print_bottom_centered__s_1,__local_print_bottom_centered__s_0 indirect=false in_cycle=false param_regs=A,X
; def print_centered_plus_col params=__zpabi_print_centered_plus_col__s_0,__zpabi_print_centered_plus_col__s_1 locals=__local_print_centered_plus_col__0,__local_print_centered_plus_col__1,__local_print_centered_plus_col__s,__local_print_centered_plus_col__2 indirect=false in_cycle=false param_regs=A,X
; def print_ctrl_ab params= locals= indirect=false in_cycle=false
; def print_ctrl_n params= locals= indirect=false in_cycle=false
; def print_string params=__zpabi_print_string__s_0,__zpabi_print_string__s_1 locals=__local_print_string__0,__local_print_string__i,__local_print_string__1,__local_print_string__s,__local_print_string__2 indirect=false in_cycle=false param_regs=A,X
; def render_stream params=__zpabi_render_stream__i locals=__local_render_stream__0,__local_render_stream__i indirect=false in_cycle=false param_regs=A
; def scroll_status_line params= locals= indirect=false in_cycle=false
; def set_cursor_row params=__zpabi_set_cursor_row__row locals=__local_set_cursor_row__row indirect=false in_cycle=false param_regs=A
; def set_cursor_row21 params= locals= indirect=false in_cycle=false
; def set_inverse_video params= locals= indirect=false in_cycle=false
; def set_text_window_bottom params= locals= indirect=false in_cycle=false
; def set_text_window_scroll params= locals= indirect=false in_cycle=false
; def set_text_window_upper_left params=__zpabi_set_text_window_upper_left__row locals=__local_set_text_window_upper_left__row indirect=false in_cycle=false param_regs=A
; def set_text_window_wrap params= locals= indirect=false in_cycle=false
; def text_renderer params= locals=__local_text_renderer__stream_1,__local_text_renderer__stream_0,__local_text_renderer__at_0,__local_text_renderer__0 indirect=false in_cycle=false
; def text_renderer_no_pause params= locals= indirect=false in_cycle=false
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
; ext hrcg_cout params=__zpabi_hrcg_cout__c param_regs=A
; ext hrcg_init params=
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
; ext pick_random_member params=
; ext pickup_treasure params=__zpabi_pickup_treasure__ev_0,__zpabi_pickup_treasure__ev_1 param_regs=A,X
; ext pos_to_colrow params=__zpabi_pos_to_colrow__pos param_regs=A
; ext print_entity_name params=__zpabi_print_entity_name__m_0,__zpabi_print_entity_name__m_1 param_regs=A,X
; ext print_fontchar_at_text_pos params=
; ext print_member_name params=
; ext print_mob_name params=
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
; ext select_combat_target params=
; ext select_someone_else params=
; ext set_blink_target params=
; ext set_char_state params=__zpabi_set_char_state__state param_regs=A
; ext set_restricted params=
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
; ext tick_anim_counter params=
; ext timed_wait params=
; ext toggle_sound params=
; ext trigger_scene_event params=__zpabi_trigger_scene_event__ev_0,__zpabi_trigger_scene_event__ev_1 param_regs=A,X
; ext tune_player params=__zpabi_tune_player__tune,__zpabi_tune_player__strength param_regs=A,X
; ext update_event_list params=__zpabi_update_event_list__pos,__zpabi_update_event_list__value_0,__zpabi_update_event_list__value_1 param_regs=A,,
; ext use_scenario_disk params=
; ext verify_disk_io params=
; ext verify_integrity params=
; call display_message -> display_message_loop
; call display_message -> next_string
; call display_message_loop -> delay_with_animation
; call display_message_loop -> next_string
; call display_message_loop -> print_bottom_centered
; call display_message_loop -> print_ctrl_ab
; call display_message_loop -> scroll_status_line
; call print_bottom_centered -> print_centered_plus_col
; call print_centered_plus_col -> print_ctrl_ab
; call print_centered_plus_col -> print_string
; call print_ctrl_ab -> hrcg_cout
; call print_ctrl_n -> hrcg_cout
; call print_string -> hrcg_cout
; call render_stream -> render_font_char
; call scroll_status_line -> hrcg_cout
; call scroll_status_line -> set_text_window_scroll
; call scroll_status_line -> set_text_window_upper_left
; call scroll_status_line -> set_text_window_wrap
; call set_cursor_row -> hrcg_cout
; call set_cursor_row -> print_ctrl_ab
; call set_cursor_row21 -> set_cursor_row
; call set_inverse_video -> hrcg_cout
; call set_text_window_bottom -> set_cursor_row
; call set_text_window_scroll -> hrcg_cout
; call set_text_window_upper_left -> hrcg_cout
; call set_text_window_wrap -> hrcg_cout
; call text_renderer -> display_status_bar
; call text_renderer -> render_stream
; call text_renderer -> tune_player
; call text_renderer_no_pause -> render_stream
; item num_to_decimal section=.text local=false zero=false
; item render_stream section=.text local=true zero=false
; item text_renderer section=.text local=false zero=false
; item text_renderer_no_pause section=.text local=false zero=false
; item next_string section=.text local=true zero=false
; item display_message section=.text local=false zero=false
; item display_message_loop section=.text local=false zero=false
; item set_text_window_upper_left section=.text local=true zero=false
; item set_text_window_scroll section=.text local=true zero=false
; item scroll_status_line section=.text local=false zero=false
; item set_cursor_row section=.text local=true zero=false
; item set_cursor_row21 section=.text local=false zero=false
; item set_text_window_bottom section=.text local=false zero=false
; item print_bottom_centered section=.text local=false zero=false
; item print_centered_plus_col section=.text local=false zero=false
; item set_inverse_video section=.text local=false zero=false
; item print_ctrl_n section=.text local=false zero=false
; item set_text_window_wrap section=.text local=false zero=false
; item print_ctrl_ab section=.text local=false zero=false
; item print_string section=.text local=false zero=false
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
; zp next_string@ret size=2 at=auto def=true init= local=true
; zp MENU_DESC_PTR size=2 at=$C0 def=false init=
; zp MENU_OPTION_PTR size=2 at=$C2 def=false init=
; zp TUNE_PC size=2 at=$E4 def=false init=
; zp MOB_PTR size=2 at=$F4 def=false init=
; zp ENTITY_PTR size=2 at=$F8 def=false init=
; zp CHAR_PTR size=2 at=$FA def=false init=
; runtime hargs_size=11 hargs_need=9
; @zp-link-meta-end

num_to_decimal:
   SUBROUTINE

   STA   __local_num_to_decimal__value_0
   STX   __local_num_to_decimal__value_1
   LDA   __zpabi_num_to_decimal__out_0
   STA   __local_num_to_decimal__out
   LDA   __zpabi_num_to_decimal__out_1
   STA   __local_num_to_decimal__1
   LDA   #$00
   STA   __local_num_to_decimal__started
   LDA   #$10
   STA   __local_num_to_decimal__place_0
   LDY   #$27
   STY   __local_num_to_decimal__place_1
.loop@0_start:
   LDA   __local_num_to_decimal__place_0
   ORA   __local_num_to_decimal__place_1
   BEQ   .loop@0_break
   LDA   __local_num_to_decimal__value_0
   STA   HARGS
   LDA   __local_num_to_decimal__value_1
   STA   HARGS+1
   LDA   __local_num_to_decimal__place_0
   STA   HARGS+2
   LDA   __local_num_to_decimal__place_1
   STA   HARGS+3
   JSR   udivmod16
   LDA   HARGS+6
   STA   __local_num_to_decimal__value_0
   LDA   HARGS+7
   STA   __local_num_to_decimal__value_1
   LDA   HARGS+4
   BNE   .num_to_decimal@scfold@0
   LDA   __local_num_to_decimal__started
   BNE   .num_to_decimal@scfold@0
   LDA   __local_num_to_decimal__place_1
   BNE   .if_end@4
   LDA   __local_num_to_decimal__place_0
   CMP   #$01
   BNE   .vthread@0
.num_to_decimal@scfold@0:
   LDA   #$B0
   CLC
   ADC   HARGS+4
   LDY   #$00
   STA   (__local_num_to_decimal__out),Y
   LDA   #$01
   STA   __local_num_to_decimal__started
.if_end@4:
   LDA   __local_num_to_decimal__place_0
.vthread@0:
   STA   HARGS
   LDA   __local_num_to_decimal__place_1
   STA   HARGS+1
   LDA   #$0A
   STA   HARGS+2
   LDA   #$00
   STA   HARGS+3
   JSR   udivmod16
   LDA   HARGS+4
   STA   __local_num_to_decimal__place_0
   LDA   HARGS+5
   STA   __local_num_to_decimal__place_1
   INC   __local_num_to_decimal__out
   BNE   .loop@0_start
   INC   __local_num_to_decimal__1
   JMP   .loop@0_start
.loop@0_break:
   RTS

render_stream:
   SUBROUTINE

   STA   __local_render_stream__i
   LDA   #$02
   STA   TEXT_LEFT_MARGIN
   STA   FONT_COL
   LDA   #$12
   STA   TEXT_RIGHT_MARGIN
   LDA   #$01
   STA   FONT_ROW
   JMP   .loop@1_start
.render_stream@rot@0:
   AND   #$7F
   STA   FONT_ROW
   INY
   LDA   (TEXT_STREAM_PTR),Y
   STA   FONT_COL
   INY
   STY   __local_render_stream__i
.loop@1_start:
   LDY   __local_render_stream__i
   LDA   (TEXT_STREAM_PTR),Y
   BMI   .render_stream@rot@0
   CMP   #$7F
   BEQ   .tail@0
   CMP   #$7E
   BNE   .if_else@10
   LDX   #$00
   TYA
   RTS
.if_else@10:
   CMP   #$7D
   BNE   .if_else@12
   LDA   #$00
   STA   TEXT_LEFT_MARGIN
   STA   FONT_COL
   STA   FONT_ROW
   LDA   #$14
   STA   TEXT_RIGHT_MARGIN
   INC   __local_render_stream__i
   JMP   .loop@1_start
.if_else@12:
   STA   FONT_CHARNUM
   LDA   #$02
   STA   FONT_CHARSET
   JSR   render_font_char
   INC   FONT_COL
   LDA   FONT_COL
   CMP   TEXT_RIGHT_MARGIN
   BCC   .if_end@13
   INC   FONT_ROW
   LDA   TEXT_LEFT_MARGIN
   STA   FONT_COL
.if_end@13:
   INC   __local_render_stream__i
   BNE   .loop@1_start
.tail@0:
   LDX   #$01
   LDA   #$00
   RTS

text_renderer:
   SUBROUTINE

   LDA   #$00
   JSR   render_stream
   STA   __local_text_renderer__at_0
.loop@2_continue:
   CPX   #$01
   BNE   .jcmp_skip@1
   ORA   #$00
   BEQ   .loop@2_break
.jcmp_skip@1:
   LDX   #$00
   LDA   #$03
   JSR   tune_player
   LDA   TEXT_STREAM_PTR
   STA   __local_text_renderer__stream_0
   LDA   TEXT_STREAM_PTR+1
   STA   __local_text_renderer__stream_1
   JSR   display_status_bar
   LDA   __local_text_renderer__stream_0
   STA   TEXT_STREAM_PTR
   LDA   __local_text_renderer__stream_1
   STA   TEXT_STREAM_PTR+1
   LDA   __local_text_renderer__at_0
   CLC
   ADC   #$01
   JSR   render_stream
   STA   __local_text_renderer__at_0
   JMP   .loop@2_continue
.loop@2_break:
   RTS

text_renderer_no_pause:
   SUBROUTINE

   LDA   #$00
   JMP   render_stream

next_string:
   SUBROUTINE

   STA   __local_next_string__s
   STX   __local_next_string__1
   LDY   #$00
   CLC
   ADC   (__local_next_string__s),Y
   BCC   .carry_inc@0
   INC   __local_next_string__1
   CLC
.carry_inc@0:
   ADC   #$01
   STA   next_string@ret
   LDA   __local_next_string__1
   ADC   #$00
   STA   next_string@ret+1
   RTS

display_message:
   SUBROUTINE

   STA   __local_display_message__index
   LDA   LOCATIONS+5
   STA   __local_display_message__msg
   LDX   LOCATIONS+6
   LDY   #$00
   STY   MSG_LINE_COUNT
   JMP   .loop@3_start
.display_message@rot@1:
   LDX   __local_display_message__1
   LDA   __local_display_message__0
   JSR   next_string
   LDA   next_string@ret
   STA   __local_display_message__0
   LDA   next_string@ret+1
   STA   __local_display_message__1
.loop@4_continue:
   LDY   #$00
   LDA   (__local_display_message__0),Y
   BNE   .display_message@rot@1
   LDA   __local_display_message__0
   CLC
   ADC   #$01
   STA   __local_display_message__msg
   LDA   __local_display_message__1
   ADC   #$00
   TAX
   DEC   __local_display_message__index
.loop@3_start:
   LDA   __local_display_message__msg
   STA   __local_display_message__0
   STX   __local_display_message__1
   LDA   __local_display_message__index
   BNE   .loop@4_continue
   LDA   __local_display_message__msg
   JMP   display_message_loop

display_message_loop:
   SUBROUTINE

   STA   __local_display_message_loop__msg_0
   STX   __local_display_message_loop__msg_1
   JMP   .loop@5_continue
.display_message_loop@rot@3:
   LDA   MSG_LINE_COUNT
   BEQ   .display_message_loop@scfold@0
   CMP   #$81
   BCC   .if_else@18
.display_message_loop@scfold@0:
   LDX   __local_display_message_loop__msg_1
   LDA   __local_display_message_loop__msg_0
   JSR   print_bottom_centered
   JSR   scroll_status_line
   JMP   .if_end@17
.if_else@18:
   CMP   #$01
   BEQ   .if_end@19
   JSR   scroll_status_line
.if_end@19:
   LDX   __local_display_message_loop__msg_1
   LDA   __local_display_message_loop__msg_0
   JSR   print_bottom_centered
.if_end@17:
   INC   MSG_LINE_COUNT
   LDX   __local_display_message_loop__msg_1
   LDA   __local_display_message_loop__msg_0
   JSR   next_string
   LDA   next_string@ret
   STA   __local_display_message_loop__msg_0
   LDA   next_string@ret+1
   STA   __local_display_message_loop__msg_1
   LDA   MSG_LINE_COUNT
   CMP   #$02
   BCS   .display_message_loop@scfold@1
   LDY   #$00
   LDA   (next_string@ret),Y
   BNE   .loop@5_continue
.display_message_loop@scfold@1:
   JSR   delay_with_animation
   JSR   print_ctrl_ab
.loop@5_continue:
   LDY   #$00
   LDA   (__local_display_message_loop__msg_0),Y
   BNE   .display_message_loop@rot@3
   LDA   MSG_LINE_COUNT
   CMP   #$02
   BNE   .if_end@23
   JSR   scroll_status_line
.if_end@23:
   RTS

set_text_window_upper_left:
   SUBROUTINE

   STA   TEXT_ROW
   LDA   #$00
   STA   TEXT_COL
   LDA   #$96
   JMP   hrcg_cout

set_text_window_scroll:
   SUBROUTINE

   LDA   #$8F
   JSR   hrcg_cout
   LDA   #$93
   JMP   hrcg_cout

scroll_status_line:
   SUBROUTINE

   JSR   set_text_window_scroll
   LDA   #$15
   JSR   set_text_window_upper_left
   LDA   #$17
   STA   TEXT_ROW
   LDA   #$83
   JSR   hrcg_cout
   LDA   #$00
   JSR   set_text_window_upper_left
   JMP   set_text_window_wrap

set_cursor_row:
   SUBROUTINE

   STA   TEXT_ROW
   LDA   #$00
   STA   TEXT_COL
   JSR   print_ctrl_ab
   LDA   #$86
   JMP   hrcg_cout

set_cursor_row21:
   SUBROUTINE

   LDA   #$15
   JMP   set_cursor_row

set_text_window_bottom:
   SUBROUTINE

   LDA   #$14
   JMP   set_cursor_row

print_bottom_centered:
   SUBROUTINE

   STA   __local_print_bottom_centered__s_0
   LDA   #$17
   STA   TEXT_ROW
   LDA   #$00
   STA   TEXT_COL
   LDA   __local_print_bottom_centered__s_0
   JMP   print_centered_plus_col

print_centered_plus_col:
   SUBROUTINE

   STA   __local_print_centered_plus_col__s
   STX   __local_print_centered_plus_col__2
   LDY   #$00
   LDA   (__local_print_centered_plus_col__s),Y
   STA   __local_print_centered_plus_col__0
   LDA   #$28
   SEC
   SBC   __local_print_centered_plus_col__0
   CLC
   ADC   #$01
   STA   __local_print_centered_plus_col__1
   TYA
   ADC   #$00
   LSR
   LDA   __local_print_centered_plus_col__1
   ROR
   CLC
   ADC   TEXT_COL
   STA   TEXT_COL
   JSR   print_ctrl_ab
   LDX   __local_print_centered_plus_col__2
   LDA   __local_print_centered_plus_col__s
   JMP   print_string

set_inverse_video:
   SUBROUTINE

   LDA   #$89
   JMP   hrcg_cout

print_ctrl_n:
   SUBROUTINE

   LDA   #$8E
   JMP   hrcg_cout

set_text_window_wrap:
   SUBROUTINE

   LDA   #$8F
   JSR   hrcg_cout
   LDA   #$97
   JMP   hrcg_cout

print_ctrl_ab:
   SUBROUTINE

   LDA   #$81
   JSR   hrcg_cout
   LDA   #$B0
   JMP   hrcg_cout

print_string:
   SUBROUTINE

   STA   __local_print_string__s
   STX   __local_print_string__2
   LDY   #$00
   LDA   (__local_print_string__s),Y
   STA   __local_print_string__1
   CMP   #$81
   BCC   .if_end@24
   RTS
.if_end@24:
   LDY   #$01
   STY   __local_print_string__i
   JMP   .loop@6_start
.print_string@rot@4:
   LDY   __local_print_string__i
   LDA   (__local_print_string__s),Y
   JSR   hrcg_cout
   INC   __local_print_string__i
.loop@6_start:
   LDA   __local_print_string__1
   CMP   __local_print_string__i
   BCS   .print_string@rot@4
   RTS
