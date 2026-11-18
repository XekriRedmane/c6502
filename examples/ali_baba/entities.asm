NULL_PAGE_LOW	EQU	$00
TEXT_STREAM_PTR	EQU	$06
RWTS_IOB_PTR	EQU	$08
NULL_PAGE_HIGH	EQU	$0A
HARGS	EQU	$10
SSP	EQU	$1A
FP	EQU	$1C
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
DPTR	EQU	$60
__local_draw_blink_alt__sprite	EQU	$80
__zpabi_apply_damage_to_current__damage	EQU	$80
__zpabi_check_adjacent_threats__pos	EQU	$80
__zpabi_check_encounter__pos	EQU	$80
__zpabi_check_hostile__other_0	EQU	$80
__zpabi_classify_entity_type__e_0	EQU	$80
__zpabi_colrow_to_pos__col	EQU	$80
__zpabi_display_message__index	EQU	$80
__zpabi_display_message_loop__msg_0	EQU	$80
__zpabi_display_shop__ev_0	EQU	$80
__zpabi_draw_char_at_pos__pos	EQU	$80
__zpabi_enter_combat_state__pos	EQU	$80
__zpabi_follow_link__link	EQU	$80
__zpabi_handle_encounter__ev_0	EQU	$80
__zpabi_handle_event_slot__ev_0	EQU	$80
__zpabi_hrcg_cout__c	EQU	$80
__zpabi_input_loop__m_0	EQU	$80
__zpabi_load_char_to_fa__index	EQU	$80
__zpabi_manhattan_distance__pos	EQU	$80
__zpabi_modify_current_stats__amount_0	EQU	$80
__zpabi_num_to_decimal__value_0	EQU	$80
__zpabi_pickup_treasure__ev_0	EQU	$80
__zpabi_print_centered_plus_col__s_0	EQU	$80
__zpabi_print_entity_name__m_0	EQU	$80
__zpabi_random_in_range__limit	EQU	$80
__zpabi_reorder_char__m_0	EQU	$80
__zpabi_run_chain__state	EQU	$80
__zpabi_rwts_dispatch__command	EQU	$80
__zpabi_scene_loop__scene	EQU	$80
__zpabi_set_char_state__state	EQU	$80
__zpabi_setup_input__m_0	EQU	$80
__zpabi_setup_text_pos__name_0	EQU	$80
__zpabi_setup_text_pos_col20__name_0	EQU	$80
__zpabi_trigger_scene_event__ev_0	EQU	$80
__zpabi_tune_player__tune	EQU	$80
__zpabi_update_event_list__pos	EQU	$80
__zpabi_check_hostile__other_1	EQU	$81
__zpabi_classify_entity_type__e_1	EQU	$81
__zpabi_colrow_to_pos__row	EQU	$81
__zpabi_display_message_loop__msg_1	EQU	$81
__zpabi_display_shop__ev_1	EQU	$81
__zpabi_find_entity_at_pos__pos	EQU	$81
__zpabi_get_entity_fontchar__e_0	EQU	$81
__zpabi_handle_encounter__ev_1	EQU	$81
__zpabi_handle_event_slot__ev_1	EQU	$81
__zpabi_input_loop__m_1	EQU	$81
__zpabi_manhattan_distance__from	EQU	$81
__zpabi_modify_current_stats__amount_1	EQU	$81
__zpabi_num_to_decimal__value_1	EQU	$81
__zpabi_pickup_treasure__ev_1	EQU	$81
__zpabi_pos_to_colrow__pos	EQU	$81
__zpabi_print_centered_plus_col__s_1	EQU	$81
__zpabi_print_entity_name__m_1	EQU	$81
__zpabi_render_font_char_direct__charset_sel	EQU	$81
__zpabi_reorder_char__m_1	EQU	$81
__zpabi_setup_input__m_1	EQU	$81
__zpabi_setup_text_pos__name_1	EQU	$81
__zpabi_setup_text_pos_col20__name_1	EQU	$81
__zpabi_trigger_scene_event__ev_1	EQU	$81
__zpabi_tune_player__strength	EQU	$81
__zpabi_update_event_list__value_0	EQU	$81
__local_classify_entity_type__0	EQU	$82
__zpabi_adjust_pc_count__m_0	EQU	$82
__zpabi_check_at_boundary__pos	EQU	$82
__zpabi_get_char_record__index	EQU	$82
__zpabi_get_entity_fontchar__e_1	EQU	$82
__zpabi_get_mob_data__index	EQU	$82
__zpabi_modify_current_stats__add	EQU	$82
__zpabi_num_to_decimal__out_0	EQU	$82
__zpabi_print_bottom_centered__s_0	EQU	$82
__zpabi_print_string__s_0	EQU	$82
__zpabi_update_event_list__value_1	EQU	$82
__local_classify_entity_type__1	EQU	$83
__zpabi_adjust_pc_count__m_1	EQU	$83
__zpabi_appearance_to_fontchar__appearance	EQU	$83
__zpabi_num_to_decimal__out_1	EQU	$83
__zpabi_print_bottom_centered__s_1	EQU	$83
__zpabi_print_string__s_1	EQU	$83
__zpabi_adjust_pc_count__delta	EQU	$84
__zpabi_adjust_pc_count__loc_0	EQU	$85
__local_find_entity_at_pos__link	EQU	$86
__local_get_entity_fontchar__sprite	EQU	$86
__zpabi_adjust_pc_count__loc_1	EQU	$86
__local_find_entity_at_pos__ev	EQU	$87
__local_get_entity_fontchar__0	EQU	$87
__local_reorder_char__0	EQU	$87
__local_find_entity_at_pos__0	EQU	$88
__local_render_font_char_direct__sprite	EQU	$88
__local_reorder_char__1	EQU	$88
__local_reorder_char__2	EQU	$89
__local_reorder_char__link	EQU	$8A
__local_reorder_char__3	EQU	$8B
__local_reorder_char__4	EQU	$8C
__local_reorder_char__5	EQU	$8D
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
; def adjust_pc_count params=__zpabi_adjust_pc_count__loc_0,__zpabi_adjust_pc_count__loc_1,__zpabi_adjust_pc_count__m_0,__zpabi_adjust_pc_count__m_1,__zpabi_adjust_pc_count__delta locals=__local_adjust_pc_count__0 indirect=false in_cycle=false
; def appearance_to_fontchar params=__zpabi_appearance_to_fontchar__appearance locals=__local_appearance_to_fontchar__sprite indirect=false in_cycle=false changes=__zpabi_appearance_to_fontchar__appearance
; def classify_entity_type params=__zpabi_classify_entity_type__e_0,__zpabi_classify_entity_type__e_1 locals=__local_classify_entity_type__0,__local_classify_entity_type__1 indirect=false in_cycle=false
; def draw_blink_alt params= locals=__local_draw_blink_alt__sprite indirect=false in_cycle=false
; def draw_blink_char params= locals= indirect=false in_cycle=false
; def draw_blink_normal params= locals= indirect=false in_cycle=false
; def draw_char_at_pos params=__zpabi_draw_char_at_pos__pos locals=__local_draw_char_at_pos__0,__local_draw_char_at_pos__1 indirect=false in_cycle=false
; def find_entity_at_pos params=__zpabi_find_entity_at_pos__pos locals=__local_find_entity_at_pos__link,__local_find_entity_at_pos__ev,__local_find_entity_at_pos__0 indirect=false in_cycle=false
; def first_group_member params= locals=__local_first_group_member__link indirect=false in_cycle=false
; def follow_link params=__zpabi_follow_link__link locals= indirect=false in_cycle=false
; def font_pos_to_text_pos params= locals= indirect=false in_cycle=false
; def get_entity_fontchar params=__zpabi_get_entity_fontchar__e_0,__zpabi_get_entity_fontchar__e_1 locals=__local_get_entity_fontchar__sprite,__local_get_entity_fontchar__0,__local_get_entity_fontchar__1 indirect=false in_cycle=false
; def load_char_name_ptr params= locals= indirect=false in_cycle=false
; def load_char_to_fa params=__zpabi_load_char_to_fa__index locals= indirect=false in_cycle=false
; def next_group_member params= locals= indirect=false in_cycle=false
; def print_entity_name params=__zpabi_print_entity_name__m_0,__zpabi_print_entity_name__m_1 locals= indirect=false in_cycle=false
; def print_fontchar_at_text_pos params= locals=__local_print_fontchar_at_text_pos__selector indirect=false in_cycle=false
; def print_member_name params= locals= indirect=false in_cycle=false
; def print_mob_name params= locals= indirect=false in_cycle=false
; def render_font_char params= locals= indirect=false in_cycle=false
; def render_font_char_direct params=__zpabi_render_font_char_direct__charset_sel locals=__local_render_font_char_direct__charset_sel,__local_render_font_char_direct__sprite indirect=false in_cycle=false changes=__zpabi_render_font_char_direct__charset_sel
; def reorder_char params=__zpabi_reorder_char__m_0,__zpabi_reorder_char__m_1 locals=__local_reorder_char__0,__local_reorder_char__1,__local_reorder_char__2,__local_reorder_char__link,__local_reorder_char__3,__local_reorder_char__4,__local_reorder_char__5,__local_reorder_char__6 indirect=false in_cycle=false
; def reorder_current_char params= locals= indirect=false in_cycle=false
; def set_blink_target params= locals=__local_set_blink_target__0,__local_set_blink_target__1 indirect=false in_cycle=false
; def setup_char_sprite params= locals=__local_setup_char_sprite__0,__local_setup_char_sprite__1 indirect=false in_cycle=false
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
; ext display_message params=__zpabi_display_message__index
; ext display_message_loop params=__zpabi_display_message_loop__msg_0,__zpabi_display_message_loop__msg_1
; ext display_shop params=__zpabi_display_shop__ev_0,__zpabi_display_shop__ev_1
; ext display_status_bar params=
; ext drop_gold_exec params=
; ext enter_char_select params=
; ext enter_combat_state params=__zpabi_enter_combat_state__pos
; ext enter_location params=
; ext enter_save_game_slot params=
; ext enter_scenario_disk params=
; ext fill_map params=
; ext find_attack_target params=
; ext find_first_pc params= carry=true
; ext find_nearest_event params=
; ext find_next_pc_wrap params=
; ext find_prev_pc params=
; ext flush_pending_xp params=
; ext format_scenario_disk params=
; ext game_init params=
; ext get_char_record params=__zpabi_get_char_record__index
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
; ext npc_move_ai params=
; ext num_to_decimal params=__zpabi_num_to_decimal__value_0,__zpabi_num_to_decimal__value_1,__zpabi_num_to_decimal__out_0,__zpabi_num_to_decimal__out_1
; ext pick_random_member params=
; ext pickup_treasure params=__zpabi_pickup_treasure__ev_0,__zpabi_pickup_treasure__ev_1
; ext pos_to_colrow params=__zpabi_pos_to_colrow__pos
; ext print_bottom_centered params=__zpabi_print_bottom_centered__s_0,__zpabi_print_bottom_centered__s_1
; ext print_centered_plus_col params=__zpabi_print_centered_plus_col__s_0,__zpabi_print_centered_plus_col__s_1
; ext print_ctrl_ab params=
; ext print_ctrl_n params=
; ext print_string params=__zpabi_print_string__s_0,__zpabi_print_string__s_1
; ext random_events params=
; ext random_in_range params=__zpabi_random_in_range__limit
; ext read_keyboard params=
; ext reset_turn_pos params=
; ext resolve_attack params=
; ext retire_character params=
; ext run_chain params=__zpabi_run_chain__state
; ext rwts_dispatch params=__zpabi_rwts_dispatch__command carry=true
; ext save_game params=
; ext scene_loop params=__zpabi_scene_loop__scene
; ext scene_setup params=
; ext scroll_status_line params=
; ext select_combat_target params=
; ext select_someone_else params=
; ext set_char_state params=__zpabi_set_char_state__state
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
; ext text_renderer params=
; ext text_renderer_no_pause params=
; ext tick_anim_counter params=
; ext timed_wait params=
; ext toggle_sound params=
; ext trigger_scene_event params=__zpabi_trigger_scene_event__ev_0,__zpabi_trigger_scene_event__ev_1
; ext tune_player params=__zpabi_tune_player__tune,__zpabi_tune_player__strength
; ext update_event_list params=__zpabi_update_event_list__pos,__zpabi_update_event_list__value_0,__zpabi_update_event_list__value_1
; ext use_scenario_disk params=
; ext verify_disk_io params=
; ext verify_integrity params=
; call draw_blink_alt -> draw_blink_char
; call draw_blink_char -> render_font_char
; call draw_blink_normal -> draw_blink_char
; call draw_blink_normal -> print_ctrl_n
; call draw_char_at_pos -> find_entity_at_pos
; call draw_char_at_pos -> get_entity_fontchar
; call draw_char_at_pos -> pos_to_colrow
; call draw_char_at_pos -> render_font_char
; call find_entity_at_pos -> check_at_boundary
; call find_entity_at_pos -> get_mob_data
; call first_group_member -> follow_link
; call follow_link -> get_mob_data
; call get_entity_fontchar -> appearance_to_fontchar
; call load_char_name_ptr -> setup_text_pos_col20
; call load_char_to_fa -> get_char_record
; call next_group_member -> follow_link
; call print_entity_name -> print_bottom_centered
; call print_entity_name -> scroll_status_line
; call print_entity_name -> set_cursor_row21
; call print_fontchar_at_text_pos -> render_font_char_direct
; call print_member_name -> print_bottom_centered
; call print_mob_name -> print_bottom_centered
; call render_font_char -> font_pos_to_text_pos
; call render_font_char -> print_fontchar_at_text_pos
; call render_font_char_direct -> print_string
; call reorder_char -> adjust_pc_count
; call reorder_char -> get_char_record
; call reorder_char -> get_mob_data
; call reorder_char -> step_prng
; call reorder_current_char -> reorder_char
; call set_blink_target -> appearance_to_fontchar
; call set_blink_target -> get_entity_fontchar
; call set_blink_target -> pos_to_colrow
; call setup_char_sprite -> pos_to_colrow
; call setup_char_sprite -> render_font_char
; share member=adjust_pc_count:__zpabi_adjust_pc_count__loc_0,__zpabi_adjust_pc_count__loc_1 holder=reorder_char live=reorder_char:adjust_pc_count,get_mob_data writer=reorder_char
; item load_char_name_ptr section=.text local=false zero=false
; item setup_char_sprite section=.text local=false zero=false
; item setup_char_sprite@run2 section=.text local=true zero=false
; item setup_char_sprite@tail0 section=.text local=true zero=false
; item find_entity_at_pos section=.text local=false zero=false
; item get_entity_fontchar section=.text local=false zero=false
; item appearance_to_fontchar section=.text local=false zero=false
; item print_mob_name section=.text local=false zero=false
; item adjust_pc_count section=.text local=true zero=false
; item reorder_char section=.text local=false zero=false
; item reorder_char@run3 section=.text local=true zero=false
; item reorder_char@run1 section=.text local=true zero=false
; item print_entity_name section=.text local=false zero=false
; item draw_char_at_pos section=.text local=false zero=false
; item classify_entity_type section=.text local=false zero=false
; item classify_entity_type@run0 section=.text local=true zero=false
; item follow_link section=.text local=true zero=false
; item first_group_member section=.text local=false zero=false
; item next_group_member section=.text local=false zero=false
; item print_member_name section=.text local=false zero=false
; item reorder_current_char section=.text local=false zero=false
; item set_blink_target section=.text local=false zero=false
; item load_char_to_fa section=.text local=false zero=false
; item render_font_char section=.text local=false zero=false
; item print_fontchar_at_text_pos section=.text local=false zero=false
; item render_font_char_direct section=.text local=false zero=false
; item draw_blink_char section=.text local=true zero=false
; item draw_blink_alt section=.text local=false zero=false
; item draw_blink_normal section=.text local=false zero=false
; item font_pos_to_text_pos section=.text local=false zero=false
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
; runtime hargs_size=10 hargs_need=0
; @zp-link-meta-end

load_char_name_ptr:
   SUBROUTINE

   LDY   #$00
   LDA   (ENTITY_PTR),Y
   STA   __zpabi_setup_text_pos_col20__name_0
   INY
   LDA   (ENTITY_PTR),Y
   STA   __zpabi_setup_text_pos_col20__name_1
   JMP   setup_text_pos_col20

setup_char_sprite:
   SUBROUTINE

   LDY   #$03
   LDA   (ENTITY_PTR),Y
   JSR   setup_char_sprite@run2
   STA   FONT_COL
   STA   BLINK_COL
   TXA
   STA   FONT_ROW
   STA   BLINK_ROW
   LDA   CHAR_SPRITE
   STA   FONT_CHARNUM
   STA   BLINK_CHAR
   JMP   setup_char_sprite@tail0

setup_char_sprite@run2:
   SUBROUTINE

   STA   __zpabi_pos_to_colrow__pos
   JMP   pos_to_colrow

setup_char_sprite@tail0:
   SUBROUTINE

   LDA   #$01
   STA   FONT_CHARSET
   JMP   render_font_char

find_entity_at_pos:
   SUBROUTINE

   LDA   __zpabi_find_entity_at_pos__pos
   STA   LOCATION_ID
   LDY   #$02
   LDA   (CHAR_PTR),Y
   JMP   .loop@0_start
.find_entity_at_pos@rot@0:
   STA   __zpabi_get_mob_data__index
   JSR   get_mob_data
   STA   __local_find_entity_at_pos__ev
   STX   __local_find_entity_at_pos__0
   LDY   #$03
   LDA   (__local_find_entity_at_pos__ev),Y
   CMP   __zpabi_find_entity_at_pos__pos
   BNE   .if_end@0
   LDA   __local_find_entity_at_pos__ev
   STA   __local_find_entity_at_pos__link
   TXA
   ORA   #$80
   TAX
   LDA   __local_find_entity_at_pos__link
   RTS
.if_end@0:
   LDY   #$02
   LDA   (__local_find_entity_at_pos__ev),Y
.loop@0_start:
   BNE   .find_entity_at_pos@rot@0
   LDY   #$06
   LDA   (CHAR_PTR),Y
   STA   __local_find_entity_at_pos__ev
   INY
   LDA   (CHAR_PTR),Y
   STA   __local_find_entity_at_pos__0
   JMP   .loop@1_start
.find_entity_at_pos@rot@1:
   CMP   __zpabi_find_entity_at_pos__pos
   BNE   .if_end@1
   LDX   __local_find_entity_at_pos__0
   LDA   __local_find_entity_at_pos__ev
   RTS
.if_end@1:
   LDA   __local_find_entity_at_pos__ev
   CLC
   ADC   #$03
   STA   __local_find_entity_at_pos__ev
   BCC   .loop@1_start
   INC   __local_find_entity_at_pos__0
.loop@1_start:
   LDY   #$00
   LDA   (__local_find_entity_at_pos__ev),Y
   CMP   #$FF
   BNE   .find_entity_at_pos@rot@1
   LDA   __zpabi_find_entity_at_pos__pos
   STA   __zpabi_check_at_boundary__pos
   JMP   check_at_boundary

get_entity_fontchar:
   SUBROUTINE

   LDA   #$01
   STA   FONT_CHARSET
   LDA   __zpabi_get_entity_fontchar__e_1
   BNE   .if_else@3
   LDA   __zpabi_get_entity_fontchar__e_0
   ORA   __zpabi_get_entity_fontchar__e_1
   BNE   .cond_else@4
   LDA   BLINK_ALT_CHAR
   JMP   .if_end@2
.cond_else@4:
   LDA   DEFAULT_CHAR
   JMP   .if_end@2
.if_else@3:
   BPL   .if_else@7
   LDA   __zpabi_get_entity_fontchar__e_0
   STA   __local_get_entity_fontchar__sprite
   LDA   __zpabi_get_entity_fontchar__e_1
   AND   #$7F
   STA   __local_get_entity_fontchar__0
   LDY   #$04
   LDA   (__local_get_entity_fontchar__sprite),Y
   STA   __zpabi_appearance_to_fontchar__appearance
   JSR   appearance_to_fontchar
   JMP   .if_end@6
.if_else@7:
   LDY   #$01
   LDA   (__zpabi_get_entity_fontchar__e_0),Y
   STA   __local_get_entity_fontchar__0
   AND   #$C0
   BNE   .if_else@9
   LDA   #$1B
   BNE   .if_end@8
.if_else@9:
   CMP   #$40
   BNE   .if_else@11
   LDA   #$1C
   BNE   .if_end@10
.if_else@11:
   CMP   #$80
   BNE   .if_else@13
   LDA   BLINK_ALT_CHAR
   JMP   .if_end@12
.if_else@13:
   LDA   __local_get_entity_fontchar__0
   CMP   #$FE
   BNE   .if_else@15
   LDY   #$02
   LDA   (__zpabi_get_entity_fontchar__e_0),Y
   AND   #$1F
   BNE   .if_end@14
   LDA   BLINK_ALT_CHAR
   JMP   .if_end@14
.if_else@15:
   CMP   #$D5
   BCC   .if_else@18
   LDA   #$1D
   BNE   .if_end@17
.if_else@18:
   CMP   #$D2
   BCC   .if_else@20
   LDA   #$18
   BNE   .if_end@19
.if_else@20:
   LDA   #$1A
.if_end@19:
.if_end@17:
.if_end@14:
.if_end@12:
.if_end@10:
.if_end@8:
.if_end@6:
.if_end@2:
   STA   FONT_CHARNUM
   RTS

appearance_to_fontchar:
   SUBROUTINE

   LDX   #$00
   LDA   __zpabi_appearance_to_fontchar__appearance
   JMP   .loop@2_continue
.appearance_to_fontchar@rot@2:
   SBC   #$15
   STA   __zpabi_appearance_to_fontchar__appearance
   INX
.loop@2_continue:
   CMP   #$15
   BCS   .appearance_to_fontchar@rot@2
   INC   __zpabi_appearance_to_fontchar__appearance
   LDA   __zpabi_appearance_to_fontchar__appearance
   CMP   #$05
   BCS   .vthread@0
   TXA
   BEQ   .if_end@23
   LDA   __zpabi_appearance_to_fontchar__appearance
   ADC   #$20
   STA   __zpabi_appearance_to_fontchar__appearance
.if_end@23:
   LDA   __zpabi_appearance_to_fontchar__appearance
.vthread@0:
   ORA   #$00
   RTS

print_mob_name:
   SUBROUTINE

   LDY   #$00
   LDA   (ENTITY_PTR),Y
   STA   __zpabi_print_bottom_centered__s_0
   INY
   LDA   (ENTITY_PTR),Y
   STA   __zpabi_print_bottom_centered__s_1
   JMP   print_bottom_centered

adjust_pc_count:
   SUBROUTINE

   LDA   __zpabi_adjust_pc_count__loc_0
   BNE   .if_end@24
   RTS
.if_end@24:
   LDY   #$04
   LDA   (__zpabi_adjust_pc_count__m_0),Y
   CMP   #$15
   BCC   .if_end@25
   RTS
.if_end@25:
   LDY   #$08
   LDA   (__zpabi_adjust_pc_count__loc_0),Y
   ADC   __zpabi_adjust_pc_count__delta
   STA   (__zpabi_adjust_pc_count__loc_0),Y
   RTS

reorder_char:
   SUBROUTINE

   LDA   SOURCE_CHAR
   JSR   reorder_char@run3
   JSR   reorder_char@run1
   LDA   #$FF
   STA   __zpabi_adjust_pc_count__delta
   JSR   adjust_pc_count
   LDA   __zpabi_adjust_pc_count__loc_0
   CLC
   ADC   #$02
   STA   __zpabi_adjust_pc_count__loc_0
   BCC   .carry_inc@1
   INC   __zpabi_adjust_pc_count__loc_1
.carry_inc@1:
.loop@3_start:
   LDY   #$00
   LDA   (__zpabi_adjust_pc_count__loc_0),Y
   STA   __local_reorder_char__5
   STA   __zpabi_get_mob_data__index
   JSR   get_mob_data
   CPX   __zpabi_reorder_char__m_1
   BNE   .if_end@26
   CMP   __zpabi_reorder_char__m_0
   BEQ   .loop@3_break
.if_end@26:
   CLC
   ADC   #$02
   STA   __zpabi_adjust_pc_count__loc_0
   TXA
   ADC   #$00
   STA   __zpabi_adjust_pc_count__loc_1
   JMP   .loop@3_start
.loop@3_break:
   LDA   __local_reorder_char__5
   CMP   CURRENT_ROOM
   BNE   .if_end@27
   LDY   #$02
   LDA   (__zpabi_reorder_char__m_0),Y
   STA   CURRENT_ROOM
.if_end@27:
   LDY   #$02
   LDA   (__zpabi_reorder_char__m_0),Y
   LDY   #$00
   STA   (__zpabi_adjust_pc_count__loc_0),Y
   LDY   #$0B
   LDA   (__zpabi_reorder_char__m_0),Y
   AND   #$1F
   STA   __local_reorder_char__4
   LDA   ACTIVE_CHAR
   JSR   reorder_char@run3
   STA   __local_reorder_char__0
   STX   __local_reorder_char__3
   JSR   reorder_char@run1
   LDA   #$01
   STA   __zpabi_adjust_pc_count__delta
   JSR   adjust_pc_count
   LDA   __local_reorder_char__0
   CLC
   ADC   #$02
   STA   __local_reorder_char__link
   BCC   .loop@4_continue
   INC   __local_reorder_char__3
   JMP   .loop@4_continue
.reorder_char@rot@3:
   STA   __zpabi_get_mob_data__index
   JSR   get_mob_data
   STA   __local_reorder_char__1
   STX   __local_reorder_char__2
   LDA   ACTIVE_CHAR
   BNE   .if_else@29
   JSR   step_prng
   ORA   #$00
   BEQ   .loop@4_break
   CMP   #$81
   BCC   .if_end@28
   JMP   .loop@4_break
.if_else@29:
   LDY   #$0B
   LDA   (__local_reorder_char__1),Y
   AND   #$1F
   CMP   __local_reorder_char__4
   BCC   .loop@4_break
.if_end@28:
   LDA   __local_reorder_char__1
   CLC
   ADC   #$02
   STA   __local_reorder_char__link
   LDA   __local_reorder_char__2
   ADC   #$00
   STA   __local_reorder_char__3
.loop@4_continue:
   LDY   #$00
   LDA   (__local_reorder_char__link),Y
   BNE   .reorder_char@rot@3
.loop@4_break:
   LDY   #$00
   LDA   (__local_reorder_char__link),Y
   LDY   #$02
   STA   (__zpabi_reorder_char__m_0),Y
   LDA   __local_reorder_char__5
   LDY   #$00
   STA   (__local_reorder_char__link),Y
   LDY   #$04
   LDA   (__zpabi_reorder_char__m_0),Y
   CMP   #$15
   BCS   .if_end@34
   LDA   ACTIVE_CHAR
   BNE   .if_end@35
   DEC   TOTAL_MOB_COUNT
.if_end@35:
   LDA   SOURCE_CHAR
   BNE   .if_end@36
   INC   TOTAL_MOB_COUNT
.if_end@36:
.if_end@34:
   RTS

reorder_char@run3:
   SUBROUTINE

   STA   __zpabi_get_char_record__index
   JMP   get_char_record

reorder_char@run1:
   SUBROUTINE

   STA   __zpabi_adjust_pc_count__loc_0
   STX   __zpabi_adjust_pc_count__loc_1
   LDA   __zpabi_reorder_char__m_0
   STA   __zpabi_adjust_pc_count__m_0
   LDA   __zpabi_reorder_char__m_1
   STA   __zpabi_adjust_pc_count__m_1
   RTS

print_entity_name:
   SUBROUTINE

   JSR   set_cursor_row21
   LDY   #$00
   LDA   (__zpabi_print_entity_name__m_0),Y
   STA   __zpabi_print_bottom_centered__s_0
   INY
   LDA   (__zpabi_print_entity_name__m_0),Y
   STA   __zpabi_print_bottom_centered__s_1
   JSR   print_bottom_centered
   JMP   scroll_status_line

draw_char_at_pos:
   SUBROUTINE

   LDA   __zpabi_draw_char_at_pos__pos
   JSR   setup_char_sprite@run2
   STA   FONT_COL
   STX   FONT_ROW
   LDA   __zpabi_draw_char_at_pos__pos
   STA   __zpabi_find_entity_at_pos__pos
   JSR   find_entity_at_pos
   STA   __zpabi_get_entity_fontchar__e_0
   STX   __zpabi_get_entity_fontchar__e_1
   JSR   get_entity_fontchar
   JMP   render_font_char

classify_entity_type:
   SUBROUTINE

   LDA   __zpabi_classify_entity_type__e_1
   BNE   .if_end@37
   LDA   __zpabi_classify_entity_type__e_0
   ORA   __zpabi_classify_entity_type__e_1
   BNE   .cond_else@38
   LDA   #<MAP_OBJECT_NAMES
   CLC
   ADC   #$21
   STA   __local_classify_entity_type__0
   JSR   classify_entity_type@run0
   JMP   .tail@0
.cond_else@38:
   CLC
   JSR   classify_entity_type@run0
   LDA   #<MAP_OBJECT_NAMES
   RTS
.if_end@37:
   LDY   #$01
   LDA   (__zpabi_classify_entity_type__e_0),Y
   CMP   #$80
   BCS   .if_end@40
   LDA   #<MAP_OBJECT_NAMES
   ADC   #$39
   STA   __local_classify_entity_type__0
   JSR   classify_entity_type@run0
   LDA   __local_classify_entity_type__0
   RTS
.if_end@40:
   CMP   #$C0
   BCS   .if_end@41
   LDA   #<MAP_OBJECT_NAMES
   ADC   #$21
   STA   __local_classify_entity_type__0
   JSR   classify_entity_type@run0
   LDA   __local_classify_entity_type__0
   RTS
.if_end@41:
   CMP   #$D2
   BCS   .if_end@42
   LDA   #<MAP_OBJECT_NAMES
   ADC   #$10
   STA   __local_classify_entity_type__0
   JSR   classify_entity_type@run0
   LDA   __local_classify_entity_type__0
   RTS
.if_end@42:
   CMP   #$D5
   BCS   .if_end@43
   LDA   #<MAP_OBJECT_NAMES
   ADC   #$53
   STA   __local_classify_entity_type__0
   JSR   classify_entity_type@run0
   LDA   __local_classify_entity_type__0
   RTS
.if_end@43:
   CMP   #$FE
   BEQ   .if_end@44
   LDA   #<MAP_OBJECT_NAMES
   CLC
   ADC   #$65
   STA   __local_classify_entity_type__0
   JSR   classify_entity_type@run0
   LDA   __local_classify_entity_type__0
   RTS
.if_end@44:
   LDY   #$02
   LDA   (__zpabi_classify_entity_type__e_0),Y
   STA   __local_classify_entity_type__0
   AND   #$1F
   STA   __local_classify_entity_type__1
   BNE   .if_end@45
   LDA   #<MAP_OBJECT_NAMES
   CLC
   ADC   #$40
   STA   __local_classify_entity_type__0
   JSR   classify_entity_type@run0
   LDA   __local_classify_entity_type__0
   RTS
.if_end@45:
   LDA   __local_classify_entity_type__0
   AND   #$C0
   CMP   #$C0
   BEQ   .if_end@46
   LDA   #<MAP_OBJECT_NAMES
   CLC
   ADC   #$2D
   STA   __local_classify_entity_type__0
   JSR   classify_entity_type@run0
   LDA   __local_classify_entity_type__0
   RTS
.if_end@46:
   LDA   __local_classify_entity_type__1
   CMP   #$1B
   BCS   .if_end@47
   LDA   #<MAP_OBJECT_NAMES
   ADC   #$07
   STA   __local_classify_entity_type__0
   JSR   classify_entity_type@run0
   LDA   __local_classify_entity_type__0
   RTS
.if_end@47:
   CMP   #$1D
   BCS   .if_end@48
   LDA   #<MAP_OBJECT_NAMES
   ADC   #$39
   STA   __local_classify_entity_type__0
   JSR   classify_entity_type@run0
   LDA   __local_classify_entity_type__0
   RTS
.if_end@48:
   BNE   .if_end@49
   LDA   #<MAP_OBJECT_NAMES
   CLC
   ADC   #$2D
   STA   __local_classify_entity_type__0
   JSR   classify_entity_type@run0
   LDA   __local_classify_entity_type__0
   RTS
.if_end@49:
   LDA   #<MAP_OBJECT_NAMES
   CLC
   STA   __local_classify_entity_type__0
   JSR   classify_entity_type@run0
.tail@0:
   LDA   __local_classify_entity_type__0
   RTS

classify_entity_type@run0:
   SUBROUTINE

   LDA   #>MAP_OBJECT_NAMES
   ADC   #$00
   TAX
   RTS

follow_link:
   SUBROUTINE

   LDA   __zpabi_follow_link__link
   BNE   .if_else@51
   LDA   #$00
   STA   MOB_PTR
   STA   MOB_PTR+1
   RTS
.if_else@51:
   STA   __zpabi_get_mob_data__index
   JSR   get_mob_data
   STA   MOB_PTR
   STX   MOB_PTR+1
   RTS

first_group_member:
   SUBROUTINE

   LDY   #$02
   LDA   (CHAR_PTR),Y
   BNE   .if_end@52
   LDA   (MOB_PTR),Y
.if_end@52:
   STA   __zpabi_follow_link__link
   JMP   follow_link

next_group_member:
   SUBROUTINE

   LDY   #$02
   LDA   (MOB_PTR),Y
   STA   __zpabi_follow_link__link
   JMP   follow_link

print_member_name:
   SUBROUTINE

   LDY   #$00
   LDA   (MOB_PTR),Y
   STA   __zpabi_print_bottom_centered__s_0
   INY
   LDA   (MOB_PTR),Y
   STA   __zpabi_print_bottom_centered__s_1
   JMP   print_bottom_centered

reorder_current_char:
   SUBROUTINE

   LDA   CURRENT_PLAYER
   STA   SOURCE_CHAR
   LDA   ENTITY_PTR
   STA   __zpabi_reorder_char__m_0
   LDA   ENTITY_PTR+1
   STA   __zpabi_reorder_char__m_1
   JMP   reorder_char

set_blink_target:
   SUBROUTINE

   LDA   IS_PLAYER_TURN
   CMP   #$01
   BNE   .if_else@54
   LDA   CURRENT_COL
   STA   BLINK_COL
   LDA   CURRENT_ROW
   STA   BLINK_ROW
   LDA   MOB_PTR
   STA   __zpabi_get_entity_fontchar__e_0
   LDA   MOB_PTR+1
   STA   __zpabi_get_entity_fontchar__e_1
   JSR   get_entity_fontchar
   STA   BLINK_CHAR
   RTS
.if_else@54:
   LDY   #$03
   LDA   (MOB_PTR),Y
   JSR   setup_char_sprite@run2
   STA   BLINK_COL
   STX   BLINK_ROW
   LDY   #$04
   LDA   (MOB_PTR),Y
   STA   __zpabi_appearance_to_fontchar__appearance
   JSR   appearance_to_fontchar
   STA   BLINK_CHAR
   RTS

load_char_to_fa:
   SUBROUTINE

   LDA   __zpabi_load_char_to_fa__index
   JSR   reorder_char@run3
   STA   CHAR_PTR
   STX   CHAR_PTR+1
   RTS

render_font_char:
   SUBROUTINE

   JSR   font_pos_to_text_pos
   JMP   print_fontchar_at_text_pos

print_fontchar_at_text_pos:
   SUBROUTINE

   LDA   FONT_CHARSET
   CMP   #$02
   BNE   .print_fontchar_at_text_pos@asm_ssa_split@0
   LDA   #$B3
.if_end@55:
   STA   s_PRINT_FONT_CHAR+2
   STA   __zpabi_render_font_char_direct__charset_sel
   JMP   render_font_char_direct
.print_fontchar_at_text_pos@asm_ssa_split@0:
   LDA   #$B1
   BNE   .if_end@55

render_font_char_direct:
   SUBROUTINE

   LDA   FONT_CHARNUM
   STA   __local_render_font_char_direct__sprite
   JMP   .loop@5_continue
.render_font_char_direct@rot@4:
   INC   __zpabi_render_font_char_direct__charset_sel
   LDA   __zpabi_render_font_char_direct__charset_sel
   STA   s_PRINT_FONT_CHAR+2
   LDA   __local_render_font_char_direct__sprite
   SBC   #$18
   STA   __local_render_font_char_direct__sprite
.loop@5_continue:
   CMP   #$18
   BCS   .render_font_char_direct@rot@4
   ASL
   ASL
   CLC
   ADC   #$20
   STA   __zpabi_render_font_char_direct__charset_sel
   STA   s_PRINT_FONT_CHAR+4
   CLC
   ADC   #$01
   STA   s_PRINT_FONT_CHAR+5
   LDA   __zpabi_render_font_char_direct__charset_sel
   CLC
   ADC   #$02
   STA   s_PRINT_FONT_CHAR+7
   LDA   __zpabi_render_font_char_direct__charset_sel
   CLC
   ADC   #$03
   STA   s_PRINT_FONT_CHAR+8
   LDA   #<s_PRINT_FONT_CHAR
   STA   __zpabi_print_string__s_0
   LDA   #>s_PRINT_FONT_CHAR
   STA   __zpabi_print_string__s_1
   JMP   print_string

draw_blink_char:
   SUBROUTINE

   LDA   BLINK_COL
   CMP   #$14
   BCC   .if_end@56
   RTS
.if_end@56:
   STA   FONT_COL
   LDA   BLINK_ROW
   STA   FONT_ROW
   LDA   BLINK_CHAR
   STA   FONT_CHARNUM
   JMP   setup_char_sprite@tail0

draw_blink_alt:
   SUBROUTINE

   LDA   BLINK_CHAR
   STA   __local_draw_blink_alt__sprite
   LDA   BLINK_ALT_CHAR
   STA   BLINK_CHAR
   JSR   draw_blink_char
   LDA   __local_draw_blink_alt__sprite
   STA   BLINK_CHAR
   RTS

draw_blink_normal:
   SUBROUTINE

   JSR   print_ctrl_n
   JMP   draw_blink_char

font_pos_to_text_pos:
   SUBROUTINE

   LDA   FONT_COL
   ASL
   STA   CH
   LDA   FONT_ROW
   ASL
   STA   CV
   RTS
