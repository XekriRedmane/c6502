NULL_PAGE_LOW	EQU	$00
TEXT_STREAM_PTR	EQU	$06
RWTS_IOB_PTR	EQU	$08
NULL_PAGE_HIGH	EQU	$0A
HARGS	EQU	$10
SSP	EQU	$15
FP	EQU	$17
DPTR	EQU	$19
MONITOR_YSAV1_1F	EQU	$1F
WNDLFT	EQU	$20
WNDWDTH	EQU	$21
WNDTOP	EQU	$22
WNDBTM	EQU	$23
CH	EQU	$24
CV	EQU	$25
MONITOR_GBAS	EQU	$26
BASL	EQU	$28
DOS_ZP	EQU	$2A
DISK_VM_ZP	EQU	$50
__zpabi_appearance_to_fontchar__appearance	EQU	$80
__zpabi_apply_damage_to_current__damage	EQU	$80
__zpabi_check_adjacent_threats__pos	EQU	$80
__zpabi_check_at_boundary__pos	EQU	$80
__zpabi_check_encounter__pos	EQU	$80
__zpabi_check_hostile__other_0	EQU	$80
__zpabi_classify_entity_type__e_0	EQU	$80
__zpabi_colrow_to_pos__col	EQU	$80
__zpabi_display_message__index	EQU	$80
__zpabi_display_shop__ev_0	EQU	$80
__zpabi_draw_char_at_pos__pos	EQU	$80
__zpabi_enter_combat_state__pos	EQU	$80
__zpabi_find_entity_at_pos__pos	EQU	$80
__zpabi_get_char_record__index	EQU	$80
__zpabi_get_entity_fontchar__e_0	EQU	$80
__zpabi_get_mob_data__index	EQU	$80
__zpabi_handle_encounter__ev_0	EQU	$80
__zpabi_handle_event_slot__ev_0	EQU	$80
__zpabi_input_loop__m_0	EQU	$80
__zpabi_load_char_to_fa__index	EQU	$80
__zpabi_manhattan_distance__pos	EQU	$80
__zpabi_modify_current_stats__amount_0	EQU	$80
__zpabi_num_to_decimal__value_0	EQU	$80
__zpabi_pickup_treasure__ev_0	EQU	$80
__zpabi_pos_to_colrow__pos	EQU	$80
__zpabi_print_entity_name__m_0	EQU	$80
__zpabi_random_in_range__limit	EQU	$80
__zpabi_render_font_char_direct__charset_sel	EQU	$80
__zpabi_render_stream__i	EQU	$80
__zpabi_reorder_char__m_0	EQU	$80
__zpabi_run_chain__state	EQU	$80
__zpabi_rwts_dispatch__command	EQU	$80
__zpabi_scene_loop__scene	EQU	$80
__zpabi_set_char_state__state	EQU	$80
__zpabi_set_cursor_row__row	EQU	$80
__zpabi_setup_input__m_0	EQU	$80
__zpabi_setup_text_pos__name_0	EQU	$80
__zpabi_setup_text_pos_col20__name_0	EQU	$80
__zpabi_trigger_scene_event__ev_0	EQU	$80
__zpabi_tune_player__tune	EQU	$80
__zpabi_update_event_list__pos	EQU	$80
__zpabi_check_hostile__other_1	EQU	$81
__zpabi_classify_entity_type__e_1	EQU	$81
__zpabi_colrow_to_pos__row	EQU	$81
__zpabi_display_message_loop__msg_0	EQU	$81
__zpabi_display_shop__ev_1	EQU	$81
__zpabi_get_entity_fontchar__e_1	EQU	$81
__zpabi_handle_encounter__ev_1	EQU	$81
__zpabi_handle_event_slot__ev_1	EQU	$81
__zpabi_input_loop__m_1	EQU	$81
__zpabi_manhattan_distance__from	EQU	$81
__zpabi_modify_current_stats__amount_1	EQU	$81
__zpabi_num_to_decimal__value_1	EQU	$81
__zpabi_pickup_treasure__ev_1	EQU	$81
__zpabi_print_entity_name__m_1	EQU	$81
__zpabi_reorder_char__m_1	EQU	$81
__zpabi_setup_input__m_1	EQU	$81
__zpabi_setup_text_pos__name_1	EQU	$81
__zpabi_setup_text_pos_col20__name_1	EQU	$81
__zpabi_trigger_scene_event__ev_1	EQU	$81
__zpabi_tune_player__strength	EQU	$81
__zpabi_update_event_list__value_0	EQU	$81
__local_text_renderer__stream_1	EQU	$82
__zpabi_display_message_loop__msg_1	EQU	$82
__zpabi_modify_current_stats__add	EQU	$82
__zpabi_num_to_decimal__out_0	EQU	$82
__zpabi_update_event_list__value_1	EQU	$82
__local_text_renderer__stream_0	EQU	$83
__zpabi_num_to_decimal__out_1	EQU	$83
__zpabi_print_bottom_centered__s_0	EQU	$83
__zpabi_print_centered_plus_col__s_0	EQU	$83
__zpabi_set_text_window_upper_left__row	EQU	$83
__local_text_renderer__at_0	EQU	$84
__zpabi_print_bottom_centered__s_1	EQU	$84
__zpabi_print_centered_plus_col__s_1	EQU	$84
__zpabi_print_string__s_0	EQU	$85
__local_num_to_decimal__digit	EQU	$86
__zpabi_print_string__s_1	EQU	$86
__local_num_to_decimal__0	EQU	$87
__zpabi_hrcg_cout__c	EQU	$87
__local_num_to_decimal__1	EQU	$88
__local_display_message__0	EQU	$89
__local_display_message__msg_0	EQU	$8A
__local_display_message__1	EQU	$8B
__local_display_message_loop__0	EQU	$8E
__local_display_message_loop__msg	EQU	$8F
__local_display_message_loop__1	EQU	$90
__local_display_message_loop__count	EQU	$91
__local_print_centered_plus_col__0	EQU	$93
__local_print_centered_plus_col__1	EQU	$94
__local_print_string__s_0	EQU	$95
__local_print_string__s_1	EQU	$96
__local_print_string__n	EQU	$97
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
MAP_OBJECT_NAMES	EQU	$7FC3
ATTACK_VERB_STRINGS	EQU	$803A
s_PRINT_FONT_CHAR	EQU	$80A4
TUNE_TABLE	EQU	$80AE
TUNE_0	EQU	$80C6
TUNE_1_COMBAT_HIT	EQU	$80C7
TUNE_2_COMBAT_DAMAGE	EQU	$80D7
FONT_DATA	EQU	$83A5
GAME_MESSAGE_STRINGS	EQU	$9AA5
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
; def display_message params=__zpabi_display_message__index locals=__local_display_message__msg_1,__local_display_message__0,__local_display_message__msg_0,__local_display_message__1,__local_display_message__index,__local_display_message__2 indirect=false in_cycle=false changes=__zpabi_display_message__index
; def display_message_loop params=__zpabi_display_message_loop__msg_0,__zpabi_display_message_loop__msg_1 locals=__local_display_message_loop__0,__local_display_message_loop__msg,__local_display_message_loop__1,__local_display_message_loop__count,__local_display_message_loop__2 indirect=false in_cycle=false
; def num_to_decimal params=__zpabi_num_to_decimal__value_0,__zpabi_num_to_decimal__value_1,__zpabi_num_to_decimal__out_0,__zpabi_num_to_decimal__out_1 locals=__local_num_to_decimal__value_1,__local_num_to_decimal__value_0,__local_num_to_decimal__digit,__local_num_to_decimal__0,__local_num_to_decimal__1 indirect=false in_cycle=false changes=__zpabi_num_to_decimal__value_0,__zpabi_num_to_decimal__value_1
; def print_bottom_centered params=__zpabi_print_bottom_centered__s_0,__zpabi_print_bottom_centered__s_1 locals= indirect=false in_cycle=false
; def print_centered_plus_col params=__zpabi_print_centered_plus_col__s_0,__zpabi_print_centered_plus_col__s_1 locals=__local_print_centered_plus_col__0,__local_print_centered_plus_col__1 indirect=false in_cycle=false
; def print_ctrl_ab params= locals= indirect=false in_cycle=false
; def print_ctrl_n params= locals= indirect=false in_cycle=false
; def print_string params=__zpabi_print_string__s_0,__zpabi_print_string__s_1 locals=__local_print_string__s_0,__local_print_string__s_1,__local_print_string__n indirect=false in_cycle=false
; def render_stream params=__zpabi_render_stream__i locals=__local_render_stream__0,__local_render_stream__i indirect=false in_cycle=false changes=__zpabi_render_stream__i
; def scroll_status_line params= locals= indirect=false in_cycle=false
; def set_cursor_row params=__zpabi_set_cursor_row__row locals= indirect=false in_cycle=false
; def set_cursor_row21 params= locals= indirect=false in_cycle=false
; def set_inverse_video params= locals= indirect=false in_cycle=false
; def set_text_window_bottom params= locals= indirect=false in_cycle=false
; def set_text_window_scroll params= locals= indirect=false in_cycle=false
; def set_text_window_upper_left params=__zpabi_set_text_window_upper_left__row locals= indirect=false in_cycle=false
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
; ext appearance_to_fontchar params=__zpabi_appearance_to_fontchar__appearance
; ext apply_damage_to_current params=__zpabi_apply_damage_to_current__damage
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
; ext check_adjacent_threats params=__zpabi_check_adjacent_threats__pos
; ext check_at_boundary params=__zpabi_check_at_boundary__pos
; ext check_attack_eligible params= carry=true
; ext check_can_leave params= carry=true
; ext check_encounter params=__zpabi_check_encounter__pos
; ext check_hostile params=__zpabi_check_hostile__other_0,__zpabi_check_hostile__other_1 carry=true
; ext check_scene_events params=
; ext check_threats_here params=
; ext clamp_char_field params=
; ext classify_entity_type params=__zpabi_classify_entity_type__e_0,__zpabi_classify_entity_type__e_1
; ext clear_map params=
; ext clear_restricted params=
; ext cold_restart params=
; ext colrow_to_pos params=__zpabi_colrow_to_pos__col,__zpabi_colrow_to_pos__row
; ext commit_move params=
; ext configure_input params=
; ext continue_game params=
; ext dec_threshold_wrap params=
; ext delay_with_animation params=
; ext disk_read params= carry=true
; ext disk_write params= carry=true
; ext dispatch_on_mode params=
; ext display_char_stats params=
; ext display_shop params=__zpabi_display_shop__ev_0,__zpabi_display_shop__ev_1
; ext display_status_bar params=
; ext draw_blink_alt params=
; ext draw_blink_normal params=
; ext draw_char_at_pos params=__zpabi_draw_char_at_pos__pos
; ext drop_gold_exec params=
; ext enter_char_select params=
; ext enter_combat_state params=__zpabi_enter_combat_state__pos
; ext enter_location params=
; ext enter_save_game_slot params=
; ext enter_scenario_disk params=
; ext fill_map params=
; ext find_attack_target params=
; ext find_entity_at_pos params=__zpabi_find_entity_at_pos__pos
; ext find_first_pc params= carry=true
; ext find_nearest_event params=
; ext find_next_pc_wrap params=
; ext find_prev_pc params=
; ext first_group_member params=
; ext flush_pending_xp params=
; ext font_pos_to_text_pos params=
; ext format_scenario_disk params=
; ext game_init params=
; ext get_char_record params=__zpabi_get_char_record__index
; ext get_entity_fontchar params=__zpabi_get_entity_fontchar__e_0,__zpabi_get_entity_fontchar__e_1
; ext get_input_table_ptr params=
; ext get_mob_data params=__zpabi_get_mob_data__index
; ext handle_encounter params=__zpabi_handle_encounter__ev_0,__zpabi_handle_encounter__ev_1
; ext handle_event_slot params=__zpabi_handle_event_slot__ev_0,__zpabi_handle_event_slot__ev_1
; ext handle_mob_encounter params=
; ext hrcg_cout params=__zpabi_hrcg_cout__c
; ext hrcg_init params=
; ext idle_anim_handler params=
; ext inc_threshold_wrap params=
; ext init_attack_display params=
; ext init_char_select params=
; ext init_drop_display params=
; ext init_drop_gold params=
; ext init_world params=
; ext input_loop params=__zpabi_input_loop__m_0,__zpabi_input_loop__m_1
; ext integrity_fail params=
; ext load_char_name_ptr params=
; ext load_char_to_fa params=__zpabi_load_char_to_fa__index
; ext main params=
; ext manhattan_distance params=__zpabi_manhattan_distance__pos,__zpabi_manhattan_distance__from
; ext menu_attack_target params=
; ext menu_check_attack params=
; ext menu_drop_dec params=
; ext menu_drop_gold params=
; ext menu_drop_inc params=
; ext menu_game_settings params=
; ext menu_game_settings_enter params=
; ext menu_other_options params=
; ext menu_see_your_lot params=
; ext modify_current_stats params=__zpabi_modify_current_stats__amount_0,__zpabi_modify_current_stats__amount_1,__zpabi_modify_current_stats__add
; ext next_group_member params=
; ext npc_move_ai params=
; ext pick_random_member params=
; ext pickup_treasure params=__zpabi_pickup_treasure__ev_0,__zpabi_pickup_treasure__ev_1
; ext pos_to_colrow params=__zpabi_pos_to_colrow__pos
; ext print_entity_name params=__zpabi_print_entity_name__m_0,__zpabi_print_entity_name__m_1
; ext print_fontchar_at_text_pos params=
; ext print_member_name params=
; ext print_mob_name params=
; ext random_events params=
; ext random_in_range params=__zpabi_random_in_range__limit
; ext read_keyboard params=
; ext render_font_char params=
; ext render_font_char_direct params=__zpabi_render_font_char_direct__charset_sel
; ext reorder_char params=__zpabi_reorder_char__m_0,__zpabi_reorder_char__m_1
; ext reorder_current_char params=
; ext reset_turn_pos params=
; ext resolve_attack params=
; ext retire_character params=
; ext run_chain params=__zpabi_run_chain__state
; ext rwts_dispatch params=__zpabi_rwts_dispatch__command carry=true
; ext save_game params=
; ext scene_loop params=__zpabi_scene_loop__scene
; ext scene_setup params=
; ext select_combat_target params=
; ext select_someone_else params=
; ext set_blink_target params=
; ext set_char_state params=__zpabi_set_char_state__state
; ext set_restricted params=
; ext settings_danger_down params=
; ext settings_danger_up params=
; ext settings_display_init params=
; ext settings_speed_down params=
; ext settings_speed_up params=
; ext setup_char_sprite params=
; ext setup_input params=__zpabi_setup_input__m_0,__zpabi_setup_input__m_1
; ext setup_scene_display params=
; ext setup_text_pos params=__zpabi_setup_text_pos__name_0,__zpabi_setup_text_pos__name_1
; ext setup_text_pos_col20 params=__zpabi_setup_text_pos_col20__name_0,__zpabi_setup_text_pos_col20__name_1
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
; ext trigger_scene_event params=__zpabi_trigger_scene_event__ev_0,__zpabi_trigger_scene_event__ev_1
; ext tune_player params=__zpabi_tune_player__tune,__zpabi_tune_player__strength
; ext update_event_list params=__zpabi_update_event_list__pos,__zpabi_update_event_list__value_0,__zpabi_update_event_list__value_1
; ext use_scenario_disk params=
; ext verify_disk_io params=
; ext verify_integrity params=
; call display_message -> display_message_loop
; call display_message_loop -> delay_with_animation
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
; share member=print_bottom_centered:__zpabi_print_bottom_centered__s_0,__zpabi_print_bottom_centered__s_1 member=print_centered_plus_col:__zpabi_print_centered_plus_col__s_0,__zpabi_print_centered_plus_col__s_1 forward=print_bottom_centered>print_centered_plus_col
; item num_to_decimal section=.text local=false zero=false
; item render_stream section=.text local=true zero=false
; item text_renderer section=.text local=false zero=false
; item text_renderer_no_pause section=.text local=false zero=false
; item display_message section=.text local=false zero=false
; item display_message_loop section=.text local=false zero=false
; item set_text_window_upper_left section=.text local=true zero=false
; item set_text_window_scroll section=.text local=true zero=false
; item set_text_window_scroll@run0 section=.text local=true zero=false
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
; item DECIMAL_PLACES section=.rodata local=true zero=false
; zp NULL_PAGE_LOW size=6 at=$00 def=false init=
; zp TEXT_STREAM_PTR size=2 at=$06 def=false init=
; zp RWTS_IOB_PTR size=2 at=$08 def=false init=
; zp NULL_PAGE_HIGH size=6 at=$0A def=false init=
; zp MONITOR_YSAV1_1F size=1 at=$1F def=false init=
; zp WNDLFT size=1 at=$20 def=false init=
; zp WNDWDTH size=1 at=$21 def=false init=
; zp WNDTOP size=1 at=$22 def=false init=
; zp WNDBTM size=1 at=$23 def=false init=
; zp CH size=1 at=$24 def=false init=
; zp CV size=1 at=$25 def=false init=
; zp MONITOR_GBAS size=2 at=$26 def=false init=
; zp BASL size=2 at=$28 def=false init=
; zp DOS_ZP size=38 at=$2A def=false init=
; zp DISK_VM_ZP size=16 at=$50 def=false init=
; zp MENU_DESC_PTR size=2 at=$C0 def=false init=
; zp MENU_OPTION_PTR size=2 at=$C2 def=false init=
; zp TUNE_PC size=2 at=$E4 def=false init=
; zp MOB_PTR size=2 at=$F4 def=false init=
; zp ENTITY_PTR size=2 at=$F8 def=false init=
; zp CHAR_PTR size=2 at=$FA def=false init=
; runtime hargs_size=5 hargs_need=0
; @zp-link-meta-end

num_to_decimal:
   SUBROUTINE

   LDA   #$00
   STA   NUM_LEADING
   LDX   #$00
.loop@0_start:
   CPX   #$05
   BCS   .loop@0_break
   LDA   DECIMAL_PLACES,X
   STA   __local_num_to_decimal__1
   LDA   DECIMAL_PLACES+5,X
   STA   __local_num_to_decimal__0
   STA   NUM_DIVISOR+1
   LDA   DECIMAL_PLACES,X
   STA   NUM_DIVISOR
   CPX   #$04
   BNE   .if_end@0
   LDA   #$01
   STA   NUM_LEADING
.if_end@0:
   LDA   #$B0
   STA   __local_num_to_decimal__digit
   JMP   .loop@1_continue
.num_to_decimal@rot@0:
   INC   __local_num_to_decimal__digit
.loop@1_continue:
   LDA   __zpabi_num_to_decimal__value_0
   SEC
   SBC   __local_num_to_decimal__1
   STA   __zpabi_num_to_decimal__value_0
   LDA   __zpabi_num_to_decimal__value_1
   SBC   __local_num_to_decimal__0
   STA   __zpabi_num_to_decimal__value_1
   BCS   .num_to_decimal@rot@0
   LDA   __zpabi_num_to_decimal__value_0
   ADC   __local_num_to_decimal__1
   STA   __zpabi_num_to_decimal__value_0
   LDA   __zpabi_num_to_decimal__value_1
   ADC   __local_num_to_decimal__0
   STA   __zpabi_num_to_decimal__value_1
   LDA   __local_num_to_decimal__digit
   STA   NUM_DIGIT
   CMP   #$B0
   BNE   .num_to_decimal@scfold@0
   LDA   NUM_LEADING
   CMP   #$01
   BNE   .if_end@3
.num_to_decimal@scfold@0:
   TXA
   TAY
   LDA   __local_num_to_decimal__digit
   STA   (__zpabi_num_to_decimal__out_0),Y
   LDA   #$01
   STA   NUM_LEADING
.if_end@3:
   INX
   JMP   .loop@0_start
.loop@0_break:
   RTS

render_stream:
   SUBROUTINE

   LDA   #$02
   STA   TEXT_LEFT_MARGIN
   STA   FONT_COL
   LDA   #$12
   STA   TEXT_RIGHT_MARGIN
   LDA   #$01
   STA   FONT_ROW
   JMP   .loop@2_start
.render_stream@rot@1:
   AND   #$7F
   STA   FONT_ROW
   INY
   LDA   (TEXT_STREAM_PTR),Y
   STA   FONT_COL
   INY
   STY   __zpabi_render_stream__i
.loop@2_start:
   LDY   __zpabi_render_stream__i
   LDA   (TEXT_STREAM_PTR),Y
   BMI   .render_stream@rot@1
   CMP   #$7F
   BEQ   .tail@0
   CMP   #$7E
   BNE   .if_else@9
   LDX   #$00
   TYA
   RTS
.if_else@9:
   CMP   #$7D
   BNE   .if_else@11
   LDA   #$00
   STA   TEXT_LEFT_MARGIN
   STA   FONT_COL
   STA   FONT_ROW
   LDA   #$14
   STA   TEXT_RIGHT_MARGIN
   INC   __zpabi_render_stream__i
   JMP   .loop@2_start
.if_else@11:
   STA   FONT_CHARNUM
   STY   TEXT_STREAM_IDX
   LDA   #$02
   STA   FONT_CHARSET
   JSR   render_font_char
   INC   FONT_COL
   LDA   FONT_COL
   CMP   TEXT_RIGHT_MARGIN
   BCC   .if_end@12
   INC   FONT_ROW
   LDA   TEXT_LEFT_MARGIN
   STA   FONT_COL
.if_end@12:
   INC   __zpabi_render_stream__i
   BNE   .loop@2_start
.tail@0:
   LDX   #$01
   LDA   #$00
   RTS

text_renderer:
   SUBROUTINE

   LDA   #$00
   STA   __zpabi_render_stream__i
   JSR   render_stream
   STA   __local_text_renderer__at_0
.loop@3_continue:
   CPX   #$01
   BNE   .jcmp_skip@0
   ORA   #$00
   BEQ   .loop@3_break
.jcmp_skip@0:
   LDA   #$03
   STA   __zpabi_tune_player__tune
   LDA   #$00
   STA   __zpabi_tune_player__strength
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
   STA   __zpabi_render_stream__i
   JSR   render_stream
   STA   __local_text_renderer__at_0
   JMP   .loop@3_continue
.loop@3_break:
   RTS

text_renderer_no_pause:
   SUBROUTINE

   LDA   #$00
   STA   __zpabi_render_stream__i
   JMP   render_stream

display_message:
   SUBROUTINE

   LDA   LOCATIONS+5
   STA   __zpabi_display_message_loop__msg_0
   LDA   LOCATIONS+6
   STA   __zpabi_display_message_loop__msg_1
   LDA   #$00
   STA   MSG_LINE_COUNT
   JMP   .loop@4_start
.loop@5_start:
   LDY   #$00
   LDA   (__local_display_message__msg_0),Y
   STA   __local_display_message__0
   CLC
   ADC   __local_display_message__msg_0
   BCC   .carry_inc@0
   INC   __local_display_message__1
   CLC
.carry_inc@0:
   ADC   #$01
   STA   __zpabi_display_message_loop__msg_0
   LDA   __local_display_message__1
   ADC   #$00
   STA   __zpabi_display_message_loop__msg_1
   STA   __local_display_message__1
   LDA   __zpabi_display_message_loop__msg_0
   STA   __local_display_message__msg_0
   LDA   __local_display_message__0
   BNE   .loop@5_start
   DEC   __zpabi_display_message__index
.loop@4_start:
   LDA   __zpabi_display_message_loop__msg_0
   STA   __local_display_message__msg_0
   LDA   __zpabi_display_message_loop__msg_1
   STA   __local_display_message__1
   LDA   __zpabi_display_message__index
   BNE   .loop@5_start
   JMP   display_message_loop

display_message_loop:
   SUBROUTINE

   LDA   __zpabi_display_message_loop__msg_0
   STA   __local_display_message_loop__msg
   LDA   __zpabi_display_message_loop__msg_1
   STA   __local_display_message_loop__1
.loop@6_start:
   LDY   #$00
   LDA   (__local_display_message_loop__msg),Y
   BNE   .if_end@14
   LDA   MSG_LINE_COUNT
   CMP   #$02
   BNE   .if_end@15
   JSR   scroll_status_line
.if_end@15:
   RTS
.if_end@14:
   LDA   MSG_LINE_COUNT
   STA   __local_display_message_loop__count
   SEC
   SBC   #$01
   BMI   .sx_neg@1
   LDA   #$00
   BEQ   .sx_done@2
.sx_neg@1:
   LDA   #$FF
.sx_done@2:
   ASL
   LDA   #$00
   ROL
   STA   __local_display_message_loop__0
   BNE   .if_end@18
   LDA   __local_display_message_loop__count
   CMP   #$01
   BEQ   .if_end@18
   JSR   scroll_status_line
.if_end@18:
   LDA   __local_display_message_loop__msg
   STA   __zpabi_print_bottom_centered__s_0
   LDA   __local_display_message_loop__1
   STA   __zpabi_print_bottom_centered__s_1
   JSR   print_bottom_centered
   LDA   __local_display_message_loop__0
   BEQ   .if_end@19
   JSR   scroll_status_line
.if_end@19:
   INC   MSG_LINE_COUNT
   LDY   #$00
   LDA   (__local_display_message_loop__msg),Y
   CLC
   ADC   __local_display_message_loop__msg
   BCC   .carry_inc@1
   INC   __local_display_message_loop__1
   CLC
.carry_inc@1:
   ADC   #$01
   STA   __local_display_message_loop__msg
   BCC   .carry_inc@2
   INC   __local_display_message_loop__1
.carry_inc@2:
   LDA   MSG_LINE_COUNT
   CMP   #$02
   BCS   .if_end@22
   LDY   #$00
   LDA   (__local_display_message_loop__msg),Y
   BNE   .loop@6_start
.if_end@22:
   JSR   delay_with_animation
   JSR   print_ctrl_ab
   JMP   .loop@6_start

set_text_window_upper_left:
   SUBROUTINE

   LDA   __zpabi_set_text_window_upper_left__row
   STA   CV
   LDA   #$00
   STA   CH
   LDA   #$96
   STA   __zpabi_hrcg_cout__c
   JMP   hrcg_cout

set_text_window_scroll:
   SUBROUTINE

   JSR   set_text_window_scroll@run0
   LDA   #$93
   STA   __zpabi_hrcg_cout__c
   JMP   hrcg_cout

set_text_window_scroll@run0:
   SUBROUTINE

   LDA   #$8F
   STA   __zpabi_hrcg_cout__c
   JMP   hrcg_cout

scroll_status_line:
   SUBROUTINE

   JSR   set_text_window_scroll
   LDA   #$15
   STA   __zpabi_set_text_window_upper_left__row
   JSR   set_text_window_upper_left
   LDA   #$17
   STA   CV
   LDA   #$83
   STA   __zpabi_hrcg_cout__c
   JSR   hrcg_cout
   LDA   #$00
   STA   __zpabi_set_text_window_upper_left__row
   JSR   set_text_window_upper_left
   JMP   set_text_window_wrap

set_cursor_row:
   SUBROUTINE

   LDA   __zpabi_set_cursor_row__row
   STA   CV
   LDA   #$00
   STA   CH
   JSR   print_ctrl_ab
   LDA   #$86
   STA   __zpabi_hrcg_cout__c
   JMP   hrcg_cout

set_cursor_row21:
   SUBROUTINE

   LDA   #$15
   STA   __zpabi_set_cursor_row__row
   JMP   set_cursor_row

set_text_window_bottom:
   SUBROUTINE

   LDA   #$14
   STA   __zpabi_set_cursor_row__row
   JMP   set_cursor_row

print_bottom_centered:
   SUBROUTINE

   LDA   #$17
   STA   CV
   LDA   #$00
   STA   CH
   JMP   print_centered_plus_col

print_centered_plus_col:
   SUBROUTINE

   LDY   #$00
   LDA   (__zpabi_print_centered_plus_col__s_0),Y
   STA   __local_print_centered_plus_col__0
   LDA   #$28
   SEC
   SBC   __local_print_centered_plus_col__0
   STA   __local_print_centered_plus_col__0
   AND   #$01
   STA   __local_print_centered_plus_col__1
   LSR   __local_print_centered_plus_col__0
   LDA   __local_print_centered_plus_col__0
   CLC
   ADC   __local_print_centered_plus_col__1
   CLC
   ADC   CH
   STA   CH
   JSR   print_ctrl_ab
   LDA   __zpabi_print_centered_plus_col__s_0
   STA   __zpabi_print_string__s_0
   LDA   __zpabi_print_centered_plus_col__s_1
   STA   __zpabi_print_string__s_1
   JMP   print_string

set_inverse_video:
   SUBROUTINE

   LDA   #$89
   STA   __zpabi_hrcg_cout__c
   JMP   hrcg_cout

print_ctrl_n:
   SUBROUTINE

   LDA   #$8E
   STA   __zpabi_hrcg_cout__c
   JMP   hrcg_cout

set_text_window_wrap:
   SUBROUTINE

   JSR   set_text_window_scroll@run0
   LDA   #$97
   STA   __zpabi_hrcg_cout__c
   JMP   hrcg_cout

print_ctrl_ab:
   SUBROUTINE

   LDA   #$81
   STA   __zpabi_hrcg_cout__c
   JSR   hrcg_cout
   LDA   #$B0
   STA   __zpabi_hrcg_cout__c
   JMP   hrcg_cout

print_string:
   SUBROUTINE

   LDY   #$00
   LDA   (__zpabi_print_string__s_0),Y
   STA   __local_print_string__n
   BEQ   .print_string@scfold@0
   CMP   #$81
   BCC   .if_end@25
.print_string@scfold@0:
   RTS
.if_end@25:
   LDA   __zpabi_print_string__s_0
   STA   __local_print_string__s_0
   LDA   __zpabi_print_string__s_1
   STA   __local_print_string__s_1
.loop@7_start:
   INC   __local_print_string__s_0
   BNE   .inc_done@0
   INC   __local_print_string__s_1
.inc_done@0:
   LDY   #$00
   LDA   (__local_print_string__s_0),Y
   STA   __zpabi_hrcg_cout__c
   JSR   hrcg_cout
   DEC   __local_print_string__n
   BNE   .loop@7_start
   RTS

DECIMAL_PLACES:
   DC.B  $10, $E8, $64, $0A, $01, $27, $03, $00, $00, $00
