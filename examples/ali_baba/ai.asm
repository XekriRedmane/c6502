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
current_slot@ret	EQU	$62
shop	EQU	$64
__zpabi_appearance_to_fontchar__appearance	EQU	$80
__zpabi_apply_damage_to_current__damage	EQU	$80
__zpabi_apply_shop_effect__n	EQU	$80
__zpabi_check_adjacent_threats__pos	EQU	$80
__zpabi_check_at_boundary__pos	EQU	$80
__zpabi_check_encounter__pos	EQU	$80
__zpabi_check_hostile__other_0	EQU	$80
__zpabi_classify_entity_type__e_0	EQU	$80
__zpabi_colrow_to_pos__col	EQU	$80
__zpabi_display_message_loop__msg_0	EQU	$80
__zpabi_display_shop__ev_0	EQU	$80
__zpabi_draw_char_at_pos__pos	EQU	$80
__zpabi_enter_combat_state__pos	EQU	$80
__zpabi_evaluate_direction__dir	EQU	$80
__zpabi_get_char_record__index	EQU	$80
__zpabi_get_entity_fontchar__e_0	EQU	$80
__zpabi_get_mob_data__index	EQU	$80
__zpabi_handle_encounter__ev_0	EQU	$80
__zpabi_handle_event_slot__ev_0	EQU	$80
__zpabi_hrcg_cout__c	EQU	$80
__zpabi_input_loop__m_0	EQU	$80
__zpabi_load_char_to_fa__index	EQU	$80
__zpabi_pickup_treasure__ev_0	EQU	$80
__zpabi_print_bottom_centered__s_0	EQU	$80
__zpabi_print_centered_plus_col__s_0	EQU	$80
__zpabi_print_entity_name__m_0	EQU	$80
__zpabi_print_string__s_0	EQU	$80
__zpabi_random_in_range__limit	EQU	$80
__zpabi_render_font_char_direct__charset_sel	EQU	$80
__zpabi_reorder_char__m_0	EQU	$80
__zpabi_run_chain__state	EQU	$80
__zpabi_rwts_dispatch__command	EQU	$80
__zpabi_set_char_state__state	EQU	$80
__zpabi_setup_input__m_0	EQU	$80
__zpabi_setup_text_pos__name_0	EQU	$80
__zpabi_setup_text_pos_col20__name_0	EQU	$80
__zpabi_trigger_scene_event__ev_0	EQU	$80
__zpabi_tune_player__tune	EQU	$80
__zpabi_update_event_list__pos	EQU	$80
__zpabi_award_xp__n	EQU	$81
__zpabi_check_hostile__other_1	EQU	$81
__zpabi_classify_entity_type__e_1	EQU	$81
__zpabi_colrow_to_pos__row	EQU	$81
__zpabi_display_message__index	EQU	$81
__zpabi_display_message_loop__msg_1	EQU	$81
__zpabi_display_shop__ev_1	EQU	$81
__zpabi_evaluate_direction__test_pos	EQU	$81
__zpabi_get_entity_fontchar__e_1	EQU	$81
__zpabi_handle_encounter__ev_1	EQU	$81
__zpabi_handle_event_slot__ev_1	EQU	$81
__zpabi_input_loop__m_1	EQU	$81
__zpabi_modify_current_stats__amount_0	EQU	$81
__zpabi_pickup_treasure__ev_1	EQU	$81
__zpabi_print_bottom_centered__s_1	EQU	$81
__zpabi_print_centered_plus_col__s_1	EQU	$81
__zpabi_print_entity_name__m_1	EQU	$81
__zpabi_print_string__s_1	EQU	$81
__zpabi_reorder_char__m_1	EQU	$81
__zpabi_setup_input__m_1	EQU	$81
__zpabi_setup_text_pos__name_1	EQU	$81
__zpabi_setup_text_pos_col20__name_1	EQU	$81
__zpabi_trigger_scene_event__ev_1	EQU	$81
__zpabi_tune_player__strength	EQU	$81
__zpabi_update_event_list__value_0	EQU	$81
__zpabi_event_penalty__ev_0	EQU	$82
__zpabi_find_entity_at_pos__pos	EQU	$82
__zpabi_manhattan_distance__pos	EQU	$82
__zpabi_modify_current_stats__amount_1	EQU	$82
__zpabi_num_to_decimal__value_0	EQU	$82
__zpabi_pending_xp__armor	EQU	$82
__zpabi_scene_loop__scene	EQU	$82
__zpabi_update_event_list__value_1	EQU	$82
__local_join_party__1	EQU	$83
__zpabi_event_penalty__ev_1	EQU	$83
__zpabi_manhattan_distance__from	EQU	$83
__zpabi_modify_current_stats__add	EQU	$83
__zpabi_num_to_decimal__value_1	EQU	$83
__local_shop_buy_handler__0	EQU	$84
__zpabi_num_to_decimal__out_0	EQU	$84
__zpabi_pos_to_colrow__pos	EQU	$84
__local_ai_choose_target__best_pos	EQU	$85
__local_npc_move_ai__0	EQU	$85
__local_shop_buy_handler__1	EQU	$85
__zpabi_num_to_decimal__out_1	EQU	$85
__local_ai_choose_target__best_strength	EQU	$86
__local_display_shop__n	EQU	$86
__local_npc_move_ai__best	EQU	$86
__local_shop_buy_handler__2	EQU	$86
__local_npc_move_ai__best_score	EQU	$87
__local_shop_buy_handler__3	EQU	$87
__local_award_xp__0	EQU	$88
__local_npc_move_ai__1	EQU	$88
__local_show_item__tier	EQU	$88
__local_apply_shop_effect__1	EQU	$89
__local_award_xp__1	EQU	$89
__local_find_nearest_event__nearest_dist	EQU	$89
__local_show_item__i	EQU	$89
__local_apply_shop_effect__2	EQU	$8A
__local_award_xp__2	EQU	$8A
__local_show_item__to	EQU	$8A
__local_apply_shop_effect__rec	EQU	$8B
__local_evaluate_direction__0	EQU	$8B
__local_find_nearest_event__1	EQU	$8B
__local_show_item__1	EQU	$8B
__local_apply_shop_effect__3	EQU	$8C
__local_evaluate_direction__1	EQU	$8C
__local_find_nearest_event__ev	EQU	$8C
__local_show_item__2	EQU	$8C
__local_find_nearest_event__2	EQU	$8D
__local_manhattan_distance__0	EQU	$90
__local_manhattan_distance__1	EQU	$91
__local_manhattan_distance__2	EQU	$92
__local_manhattan_distance__3	EQU	$93
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
; def ai_choose_target params= locals=__local_ai_choose_target__best_pos,__local_ai_choose_target__best_strength,__local_ai_choose_target__0,__local_ai_choose_target__1 indirect=false in_cycle=false
; def apply_shop_effect params=__zpabi_apply_shop_effect__n locals=__local_apply_shop_effect__0,__local_apply_shop_effect__1,__local_apply_shop_effect__2,__local_apply_shop_effect__rec,__local_apply_shop_effect__3 indirect=false in_cycle=false
; def award_xp params=__zpabi_award_xp__n locals=__local_award_xp__0,__local_award_xp__1,__local_award_xp__2 indirect=false in_cycle=false
; def browse_handler params= locals=__local_browse_handler__0 indirect=false in_cycle=false
; def current_slot params= locals=__local_current_slot__0 indirect=false in_cycle=false
; def display_shop params=__zpabi_display_shop__ev_0,__zpabi_display_shop__ev_1 locals=__local_display_shop__n indirect=false in_cycle=false
; def evaluate_direction params=__zpabi_evaluate_direction__dir,__zpabi_evaluate_direction__test_pos locals=__local_evaluate_direction__0,__local_evaluate_direction__1,__local_evaluate_direction__2,__local_evaluate_direction__3,__local_evaluate_direction__4 indirect=false in_cycle=false
; def event_penalty params=__zpabi_event_penalty__ev_0,__zpabi_event_penalty__ev_1 locals=__local_event_penalty__0 indirect=false in_cycle=false
; def find_nearest_event params= locals=__local_find_nearest_event__nearest_dist,__local_find_nearest_event__0,__local_find_nearest_event__1,__local_find_nearest_event__ev,__local_find_nearest_event__2 indirect=false in_cycle=false
; def join_party params= locals=__local_join_party__0,__local_join_party__1 indirect=false in_cycle=false
; def manhattan_distance params=__zpabi_manhattan_distance__pos,__zpabi_manhattan_distance__from locals=__local_manhattan_distance__0,__local_manhattan_distance__1,__local_manhattan_distance__2,__local_manhattan_distance__3 indirect=false in_cycle=false
; def next_slot params= locals= indirect=false in_cycle=false
; def npc_move_ai params= locals=__local_npc_move_ai__0,__local_npc_move_ai__best,__local_npc_move_ai__best_score,__local_npc_move_ai__1,__local_npc_move_ai__2,__local_npc_move_ai__3 indirect=false in_cycle=false
; def pending_xp params=__zpabi_pending_xp__armor locals=__local_pending_xp__0 indirect=false in_cycle=false
; def shop_buy_handler params= locals=__local_shop_buy_handler__0,__local_shop_buy_handler__1,__local_shop_buy_handler__2,__local_shop_buy_handler__3 indirect=false in_cycle=false
; def show_item params= locals=__local_show_item__0,__local_show_item__tier,__local_show_item__i,__local_show_item__to,__local_show_item__1,__local_show_item__2,__local_show_item__3,__local_show_item__4 indirect=false in_cycle=false
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
; ext display_message params=__zpabi_display_message__index
; ext display_message_loop params=__zpabi_display_message_loop__msg_0,__zpabi_display_message_loop__msg_1
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
; ext num_to_decimal params=__zpabi_num_to_decimal__value_0,__zpabi_num_to_decimal__value_1,__zpabi_num_to_decimal__out_0,__zpabi_num_to_decimal__out_1
; ext pick_random_member params=
; ext pickup_treasure params=__zpabi_pickup_treasure__ev_0,__zpabi_pickup_treasure__ev_1
; ext pos_to_colrow params=__zpabi_pos_to_colrow__pos
; ext print_bottom_centered params=__zpabi_print_bottom_centered__s_0,__zpabi_print_bottom_centered__s_1
; ext print_centered_plus_col params=__zpabi_print_centered_plus_col__s_0,__zpabi_print_centered_plus_col__s_1
; ext print_ctrl_ab params=
; ext print_ctrl_n params=
; ext print_entity_name params=__zpabi_print_entity_name__m_0,__zpabi_print_entity_name__m_1
; ext print_fontchar_at_text_pos params=
; ext print_member_name params=
; ext print_mob_name params=
; ext print_string params=__zpabi_print_string__s_0,__zpabi_print_string__s_1
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
; ext scroll_status_line params=
; ext select_combat_target params=
; ext select_someone_else params=
; ext set_blink_target params=
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
; ext setup_char_sprite params=
; ext setup_input params=__zpabi_setup_input__m_0,__zpabi_setup_input__m_1
; ext setup_scene_display params=
; ext setup_text_pos params=__zpabi_setup_text_pos__name_0,__zpabi_setup_text_pos__name_1
; ext setup_text_pos_col20 params=__zpabi_setup_text_pos_col20__name_0,__zpabi_setup_text_pos_col20__name_1
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
; call ai_choose_target -> check_hostile
; call ai_choose_target -> find_nearest_event
; call ai_choose_target -> first_group_member
; call ai_choose_target -> join_party
; call ai_choose_target -> manhattan_distance
; call ai_choose_target -> next_group_member
; call ai_choose_target -> pick_random_member
; call ai_choose_target -> pos_to_colrow
; call award_xp -> pending_xp
; call browse_handler -> next_slot
; call browse_handler -> show_item
; call display_shop -> load_char_name_ptr
; call display_shop -> scene_loop
; call display_shop -> set_cursor_row21
; call display_shop -> setup_char_sprite
; call display_shop -> show_item
; call evaluate_direction -> event_penalty
; call evaluate_direction -> find_entity_at_pos
; call evaluate_direction -> manhattan_distance
; call find_nearest_event -> manhattan_distance
; call join_party -> delay_with_animation
; call join_party -> display_message
; call join_party -> print_member_name
; call join_party -> scroll_status_line
; call join_party -> set_cursor_row21
; call manhattan_distance -> pos_to_colrow
; call npc_move_ai -> anim_tick_and_wait
; call npc_move_ai -> evaluate_direction
; call npc_move_ai -> manhattan_distance
; call npc_move_ai -> random_in_range
; call shop_buy_handler -> apply_shop_effect
; call shop_buy_handler -> award_xp
; call shop_buy_handler -> current_slot
; call shop_buy_handler -> display_message
; call shop_buy_handler -> modify_current_stats
; call shop_buy_handler -> set_cursor_row21
; call show_item -> current_slot
; call show_item -> next_slot
; call show_item -> num_to_decimal
; share member=apply_shop_effect:__zpabi_apply_shop_effect__n holder=shop_buy_handler live=shop_buy_handler:apply_shop_effect,award_xp forward=shop_buy_handler>apply_shop_effect writer=shop_buy_handler
; share member=pending_xp:__zpabi_pending_xp__armor holder=award_xp live=award_xp:pending_xp writer=award_xp
; item manhattan_distance section=.text local=false zero=false
; item manhattan_distance@run2 section=.text local=true zero=false
; item find_nearest_event section=.text local=false zero=false
; item event_penalty section=.text local=true zero=false
; item evaluate_direction section=.text local=true zero=false
; item evaluate_direction@run1 section=.text local=true zero=false
; item npc_move_ai section=.text local=false zero=false
; item join_party section=.text local=true zero=false
; item ai_choose_target section=.text local=false zero=false
; item current_slot section=.text local=true zero=false
; item next_slot section=.text local=true zero=false
; item next_slot@run0 section=.text local=true zero=false
; item show_item section=.text local=true zero=false
; item display_shop section=.text local=false zero=false
; item browse_handler section=.text local=false zero=false
; item apply_shop_effect section=.text local=true zero=false
; item pending_xp section=.text local=true zero=false
; item award_xp section=.text local=true zero=false
; item shop_buy_handler section=.text local=false zero=false
; item WEAPON_EFFECT_ADDRS section=.rodata local=true zero=false
; item ARMOR_EFFECT_ADDRS section=.rodata local=true zero=false
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
; zp CH size=1 at=$24 def=false init=
; zp CV size=1 at=$25 def=false init=
; zp MONITOR_GBAS size=2 at=$26 def=false init=
; zp BASL size=2 at=$28 def=false init=
; zp DOS_ZP size=38 at=$2A def=false init=
; zp DISK_VM_ZP size=16 at=$50 def=false init=
; zp current_slot@ret size=2 at=auto def=true init= local=true
; zp shop size=2 at=auto def=true init=$00,$00 weight=27 local=true
; zp MENU_DESC_PTR size=2 at=$C0 def=false init=
; zp MENU_OPTION_PTR size=2 at=$C2 def=false init=
; zp TUNE_PC size=2 at=$E4 def=false init=
; zp MOB_PTR size=2 at=$F4 def=false init=
; zp ENTITY_PTR size=2 at=$F8 def=false init=
; zp CHAR_PTR size=2 at=$FA def=false init=
; runtime hargs_size=10 hargs_need=0
; @zp-link-meta-end

manhattan_distance:
   SUBROUTINE

   LDA   __zpabi_manhattan_distance__pos
   JSR   manhattan_distance@run2
   STA   __local_manhattan_distance__3
   STX   __local_manhattan_distance__2
   STA   FONT_COL
   STX   FONT_ROW
   LDA   __zpabi_manhattan_distance__from
   JSR   manhattan_distance@run2
   STA   __local_manhattan_distance__1
   STX   __local_manhattan_distance__0
   CMP   __local_manhattan_distance__3
   BCC   .cond_else@0
   SBC   __local_manhattan_distance__3
   STA   __local_manhattan_distance__1
   JMP   .cond_end@1
.cond_else@0:
   LDA   __local_manhattan_distance__3
   SEC
   SBC   __local_manhattan_distance__1
   STA   __local_manhattan_distance__1
.cond_end@1:
   TXA
   SEC
   SBC   __local_manhattan_distance__2
   BCC   .cond_else@2
   STA   __local_manhattan_distance__0
   JMP   .cond_end@3
.cond_else@2:
   LDA   __local_manhattan_distance__2
   SEC
   SBC   __local_manhattan_distance__0
   STA   __local_manhattan_distance__0
.cond_end@3:
   LDA   __local_manhattan_distance__1
   CLC
   ADC   __local_manhattan_distance__0
   RTS

manhattan_distance@run2:
   SUBROUTINE

   STA   __zpabi_pos_to_colrow__pos
   JMP   pos_to_colrow

find_nearest_event:
   SUBROUTINE

   LDY   #$06
   LDA   (CHAR_PTR),Y
   STA   __local_find_nearest_event__ev
   INY
   LDA   (CHAR_PTR),Y
   STA   __local_find_nearest_event__2
   LDA   #$7F
   STA   __local_find_nearest_event__nearest_dist
   JMP   .loop@0_start
.find_nearest_event@rot@0:
   CMP   #$FE
   BEQ   .loop@0_continue
   LDY   #$01
   LDA   (__local_find_nearest_event__ev),Y
   CMP   #$C0
   BCS   .loop@0_continue
   DEY
   LDA   (__local_find_nearest_event__ev),Y
   STA   __local_find_nearest_event__1
   STA   CANDIDATE_POS
   LDY   #$03
   STA   __zpabi_manhattan_distance__pos
   LDA   (ENTITY_PTR),Y
   STA   __zpabi_manhattan_distance__from
   JSR   manhattan_distance
   CMP   __local_find_nearest_event__nearest_dist
   BCS   .if_end@7
   STA   __local_find_nearest_event__nearest_dist
   LDA   __local_find_nearest_event__1
   STA   NEAREST_POS
.if_end@7:
.loop@0_continue:
   LDA   __local_find_nearest_event__ev
   CLC
   ADC   #$03
   STA   __local_find_nearest_event__ev
   BCC   .loop@0_start
   INC   __local_find_nearest_event__2
.loop@0_start:
   LDY   #$00
   LDA   (__local_find_nearest_event__ev),Y
   CMP   #$FF
   BNE   .find_nearest_event@rot@0
   LDA   NEAREST_POS
   RTS

event_penalty:
   SUBROUTINE

   LDY   #$01
   LDA   (__zpabi_event_penalty__ev_0),Y
   CMP   #$80
   BCC   .tail@0
   CMP   #$C0
   BCS   .if_end@9
   LDA   #$06
   RTS
.if_end@9:
   CMP   #$D2
   BCS   .if_end@10
   TYA
   RTS
.if_end@10:
   CMP   #$D5
   BCS   .if_end@11
   LDA   #$02
   RTS
.if_end@11:
   CMP   #$D8
   BCS   .if_end@12
.tail@0:
   LDA   #$03
   RTS
.if_end@12:
   LDY   #$02
   LDA   (__zpabi_event_penalty__ev_0),Y
   CMP   #$40
   BCS   .cond_else@13
   LDA   #$07
   RTS
.cond_else@13:
   LDA   #$01
   RTS

evaluate_direction:
   SUBROUTINE

   LDA   AI_TARGET_POS
   CMP   __zpabi_evaluate_direction__test_pos
   BEQ   .if_end@15
   CLC
   JSR   evaluate_direction@run1
   ADC   #$09
   STA   DIR_SCORES,X
.if_end@15:
   LDA   AI_TARGET_POS
   BEQ   .if_end@18
   STA   __zpabi_manhattan_distance__pos
   LDA   __zpabi_evaluate_direction__test_pos
   STA   __zpabi_manhattan_distance__from
   JSR   manhattan_distance
   CMP   AI_BEST_DIST
   BCC   .if_end@18
   CLC
   JSR   evaluate_direction@run1
   ADC   #$02
   STA   DIR_SCORES,X
.if_end@18:
   LDA   __zpabi_evaluate_direction__test_pos
   STA   __zpabi_find_entity_at_pos__pos
   JSR   find_entity_at_pos
   STA   __zpabi_event_penalty__ev_0
   STX   __zpabi_event_penalty__ev_1
   TXA
   AND   #$80
   STA   __local_evaluate_direction__0
   BNE   .evaluate_direction@scfold@0
   LDA   __zpabi_event_penalty__ev_0
   ORA   __zpabi_event_penalty__ev_1
   BNE   .if_end@21
.evaluate_direction@scfold@0:
   RTS
.if_end@21:
   LDA   #$FF
   SEC
   SBC   __zpabi_event_penalty__ev_0
   LDA   #$00
   SBC   __zpabi_event_penalty__ev_1
   BCS   .if_else@25
   LDA   __local_evaluate_direction__0
   BNE   .if_else@25
   JSR   evaluate_direction@run1
   STA   __local_evaluate_direction__1
   JSR   event_penalty
   CLC
   ADC   __local_evaluate_direction__1
   LDX   __zpabi_evaluate_direction__dir
   STA   DIR_SCORES,X
   RTS
.if_else@25:
   CLC
   JSR   evaluate_direction@run1
   ADC   #$07
   STA   DIR_SCORES,X
   RTS

evaluate_direction@run1:
   SUBROUTINE

   LDX   __zpabi_evaluate_direction__dir
   LDA   DIR_SCORES,X
   RTS

npc_move_ai:
   SUBROUTINE

   JSR   anim_tick_and_wait
   LDA   CHAR_AI_MODE
   CMP   #$04
   BEQ   .if_end@28
   LDA   AI_TARGET_POS
   BNE   .if_end@28
   LDA   #$00
   RTS
.if_end@28:
   LDA   #$00
   STA   DIR_SCORES+1
   LDY   #$00
   STY   DIR_SCORES+2
   STY   DIR_SCORES+3
   STA   DIR_SCORES+4
   LDA   #$0A
   STA   DIR_SCORES
   LDY   #$03
   LDA   (ENTITY_PTR),Y
   STA   __zpabi_manhattan_distance__from
   LDA   AI_TARGET_POS
   STA   __zpabi_manhattan_distance__pos
   JSR   manhattan_distance
   STA   AI_BEST_DIST
   ORA   #$00
   BEQ   .if_end@29
   LDA   #$13
   STA   DIR_SCORES
.if_end@29:
   LDA   AI_PREV_DIR
   BEQ   .if_end@30
   STA   __local_npc_move_ai__best
   CLC
   LDX   __local_npc_move_ai__best
   LDA   DIR_SCORES,X
   ADC   #$03
   STA   DIR_SCORES,X
.if_end@30:
   LDY   #$03
   LDA   (ENTITY_PTR),Y
   STA   __local_npc_move_ai__0
   SEC
   SBC   #$14
   STA   __zpabi_evaluate_direction__test_pos
   LDA   #$01
   STA   __zpabi_evaluate_direction__dir
   JSR   evaluate_direction
   LDA   __local_npc_move_ai__0
   CLC
   ADC   #$14
   STA   __zpabi_evaluate_direction__test_pos
   LDA   #$02
   STA   __zpabi_evaluate_direction__dir
   JSR   evaluate_direction
   LDA   __local_npc_move_ai__0
   SEC
   SBC   #$01
   STA   __zpabi_evaluate_direction__test_pos
   LDA   #$03
   STA   __zpabi_evaluate_direction__dir
   JSR   evaluate_direction
   LDA   __local_npc_move_ai__0
   CLC
   ADC   #$01
   STA   __zpabi_evaluate_direction__test_pos
   LDA   #$04
   STA   __zpabi_evaluate_direction__dir
   JSR   evaluate_direction
   LDA   CHAR_AI_MODE
   CMP   #$04
   BNE   .cond_else@31
   LDA   #$0F
   STA   __local_npc_move_ai__1
   JMP   .cond_end@32
.cond_else@31:
   LDA   #$06
   STA   __local_npc_move_ai__1
.cond_end@32:
   LDA   #$05
   STA   __local_npc_move_ai__best_score
   JMP   .loop@1_start
.npc_move_ai@rot@1:
   LDX   __local_npc_move_ai__best_score
   LDA   DIR_SCORES,X
   ASL
   ASL
   STA   DIR_SCORES,X
   LDA   DIR_SCORES,X
   STA   __local_npc_move_ai__best
   LDA   __local_npc_move_ai__1
   STA   __zpabi_random_in_range__limit
   JSR   random_in_range
   CLC
   ADC   __local_npc_move_ai__best
   LDX   __local_npc_move_ai__best_score
   STA   DIR_SCORES,X
.loop@1_start:
   DEC   __local_npc_move_ai__best_score
   LDA   __local_npc_move_ai__best_score
   CMP   #$FF
   BNE   .npc_move_ai@rot@1
   LDA   #$7F
   STA   __local_npc_move_ai__best_score
   LDA   #$00
   STA   __local_npc_move_ai__best
   LDX   #$05
   BNE   .loop@2_start
.npc_move_ai@rot@2:
   LDA   DIR_SCORES,X
   CMP   __local_npc_move_ai__best_score
   BCS   .loop@2_start
   STA   __local_npc_move_ai__best_score
   STX   __local_npc_move_ai__best
.loop@2_start:
   DEX
   CPX   #$FF
   BNE   .npc_move_ai@rot@2
   LDA   __local_npc_move_ai__best
   CMP   #$03
   BCS   .cond_else@34
   EOR   #$03
   JMP   .cond_end@35
.cond_else@34:
   EOR   #$07
.cond_end@35:
   STA   AI_PREV_DIR
   LDA   __local_npc_move_ai__best
   RTS

join_party:
   SUBROUTINE

   JSR   set_cursor_row21
   LDA   #$24
   STA   __zpabi_display_message__index
   JSR   display_message
   JSR   print_member_name
   JSR   scroll_status_line
   JSR   delay_with_animation
   LDA   #$25
   STA   __zpabi_display_message__index
   JSR   display_message
   LDA   #$00
   STA   CHAR_AI_MODE
   LDY   #$04
   LDA   (ENTITY_PTR),Y
   SEC
   SBC   #$15
   STA   (ENTITY_PTR),Y
   LDY   #$0F
   LDA   (ENTITY_PTR),Y
   AND   #$3F
   STA   __local_join_party__1
   LDA   (MOB_PTR),Y
   AND   #$C0
   ORA   __local_join_party__1
   STA   (ENTITY_PTR),Y
   CLC
   LDY   #$08
   LDA   (CHAR_PTR),Y
   ADC   #$01
   STA   (CHAR_PTR),Y
   INC   TOTAL_MOB_COUNT
   RTS

ai_choose_target:
   SUBROUTINE

   LDA   CHAR_AI_MODE
   LDY   #$00
   STY   AI_PREV_DIR
   CMP   #$02
   BCS   .vthread@0
   JSR   pick_random_member
   SEC
   LDA   MOB_PTR+1
   SBC   #$01
   BCC   .vthread@1
   JMP   join_party
.vthread@0:
   BNE   .if_else@39
.vthread@1:
   LDY   #$06
   LDA   (ENTITY_PTR),Y
   AND   #$3F
   STA   __local_ai_choose_target__best_strength
   DEY
   LDA   (ENTITY_PTR),Y
   AND   #$1F
   STA   __local_ai_choose_target__best_pos
   LDA   __local_ai_choose_target__best_strength
   CMP   __local_ai_choose_target__best_pos
   BCC   .if_else@41
   JSR   find_nearest_event
   STA   AI_TARGET_POS
   JMP   .if_end@38
.if_else@41:
   LDA   #$00
   STA   AI_TARGET_POS
   JMP   .if_end@38
.if_else@39:
   CMP   #$03
   BNE   .if_else@43
   LDA   #$3C
   STA   AI_TARGET_COL
   JSR   first_group_member
.loop@3_start:
   LDA   ENTITY_PTR
   STA   __zpabi_check_hostile__other_0
   LDA   ENTITY_PTR+1
   STA   __zpabi_check_hostile__other_1
   JSR   check_hostile
   BCC   .if_end@44
   LDY   #$03
   LDA   (MOB_PTR),Y
   STA   __zpabi_manhattan_distance__pos
   LDA   (ENTITY_PTR),Y
   STA   __zpabi_manhattan_distance__from
   JSR   manhattan_distance
   CMP   AI_TARGET_COL
   BCS   .if_end@45
   STA   AI_TARGET_COL
   LDY   #$03
   LDA   (MOB_PTR),Y
   STA   AI_TARGET_POS
.if_end@45:
.if_end@44:
   JSR   next_group_member
   SEC
   LDA   MOB_PTR+1
   SBC   #$01
   BCS   .loop@3_start
   LDA   AI_TARGET_COL
   CMP   #$3C
   BNE   .if_end@42
   LDA   EVENT_POS
   STA   AI_TARGET_POS
   JMP   .if_end@42
.if_else@43:
   CMP   #$04
   BNE   .if_else@48
   LDA   #$00
   STA   AI_TARGET_POS
   JMP   .if_end@47
.if_else@48:
   JSR   first_group_member
   LDA   #$00
   STA   __local_ai_choose_target__best_strength
.loop@4_start:
   LDA   ENTITY_PTR
   STA   __zpabi_check_hostile__other_0
   LDA   ENTITY_PTR+1
   STA   __zpabi_check_hostile__other_1
   JSR   check_hostile
   BCC   .if_end@49
   LDY   #$05
   LDA   (MOB_PTR),Y
   AND   #$1F
   CMP   __local_ai_choose_target__best_strength
   BCC   .if_end@50
   LDY   #$03
   STA   __local_ai_choose_target__best_strength
   LDA   (MOB_PTR),Y
   STA   __local_ai_choose_target__best_pos
.if_end@50:
.if_end@49:
   JSR   next_group_member
   SEC
   LDA   MOB_PTR+1
   SBC   #$01
   BCS   .loop@4_start
   LDA   __local_ai_choose_target__best_strength
   BNE   .cond_end@52
   LDA   EVENT_POS
   STA   __local_ai_choose_target__best_pos
.cond_end@52:
   LDA   __local_ai_choose_target__best_pos
   STA   AI_TARGET_POS
.if_end@47:
.if_end@42:
.if_end@38:
   LDA   AI_TARGET_POS
   JSR   manhattan_distance@run2
   STA   AI_TARGET_COL
   STX   AI_TARGET_ROW
   RTS

current_slot:
   SUBROUTINE

   LDA   shop
   CLC
   ADC   SHOP_ITEM_OFFSET
   STA   current_slot@ret
   LDA   shop+1
   ADC   #$00
   STA   current_slot@ret+1
   RTS

next_slot:
   SUBROUTINE

   LDA   SHOP_ITEM_MASK
   LSR
   BCC   .if_else@54
   JSR   next_slot@run0
   RTS
.if_else@54:
   LSR   SHOP_ITEM_MASK
   LDA   SHOP_ITEM_OFFSET
   CLC
   ADC   #$05
   STA   SHOP_ITEM_OFFSET
   RTS

next_slot@run0:
   SUBROUTINE

   LDA   #$80
   STA   SHOP_ITEM_MASK
   LDA   #$00
   STA   SHOP_ITEM_OFFSET
   RTS

show_item:
   SUBROUTINE

   JMP   .loop@5_continue
.show_item@rot@3:
   JSR   next_slot
.loop@5_continue:
   LDA   SHOP_VALID_MASK
   AND   SHOP_ITEM_MASK
   BEQ   .show_item@rot@3
   JSR   current_slot
   LDA   current_slot@ret
   STA   __local_show_item__to
   LDA   current_slot@ret+1
   STA   __local_show_item__1
   LDA   LOCATION_STYLE
   LSR
   LSR
   LSR
   LSR
   LSR
   AND   #$03
   STA   __local_show_item__tier
   STA   LOCATION_STYLE_BITS
   LDA   #$00
   STA   SHOP_PRICE
   STA   SHOP_PRICE+1
.loop@6_start:
   LDY   #$04
   LDA   (current_slot@ret),Y
   CLC
   ADC   SHOP_PRICE
   STA   SHOP_PRICE
   BCC   .carry_inc@1
   INC   SHOP_PRICE+1
.carry_inc@1:
   DEC   __local_show_item__tier
   LDA   __local_show_item__tier
   CMP   #$FF
   BNE   .loop@6_start
   LDA   #$00
   STA   __local_show_item__i
.show_item@rot@4:
   LDX   __local_show_item__i
   LDA   #$20
   STA   S_PAY_FOR+5,X
   INC   __local_show_item__i
   LDA   __local_show_item__i
   CMP   #$05
   BCC   .show_item@rot@4
   LDA   #$00
   STA   __local_show_item__i
.show_item@rot@5:
   LDX   __local_show_item__i
   LDA   #$20
   STA   S_PAY_FOR+16,X
   INC   __local_show_item__i
   LDA   __local_show_item__i
   CMP   #$14
   BCC   .show_item@rot@5
   LDA   #<(S_PAY_FOR+5)
   STA   __zpabi_num_to_decimal__out_0
   LDA   #>(S_PAY_FOR+5)
   STA   __zpabi_num_to_decimal__out_1
   LDA   SHOP_PRICE
   STA   __zpabi_num_to_decimal__value_0
   LDA   SHOP_PRICE+1
   STA   __zpabi_num_to_decimal__value_1
   JSR   num_to_decimal
   LDA   __local_show_item__to
   STA   DPTR
   LDA   __local_show_item__1
   STA   DPTR+1
   LDY   #$02
   LDA   (__local_show_item__to),Y
   STA   __local_show_item__1
   INY
   LDA   (DPTR),Y
   STA   __local_show_item__2
   LDY   #$00
   LDA   (__local_show_item__1),Y
   TAY
   TYA
   SEC
   SBC   #$01
   STA   __local_show_item__to
.loop@9_start:
   LDA   (__local_show_item__1),Y
   LDX   __local_show_item__to
   STA   S_PAY_FOR+16,X
   DEY
   DEC   __local_show_item__to
   BPL   .loop@9_start
   LDA   #$2D
   RTS

display_shop:
   SUBROUTINE

   LDA   CHAR_AI_MODE
   BEQ   .if_end@55
   LDA   #$00
   RTS
.if_end@55:
   LDA   #<SHOP_TABLE
   STA   shop
   LDA   #>SHOP_TABLE
   STA   shop+1
   LDY   #$01
   LDA   (__zpabi_display_shop__ev_0),Y
   SEC
   SBC   #$D5
   STA   __local_display_shop__n
   JMP   .loop@10_start
.display_shop@rot@6:
   LDA   shop
   CLC
   ADC   #$29
   STA   shop
   BCC   .carry_inc@2
   INC   shop+1
.carry_inc@2:
   DEC   __local_display_shop__n
.loop@10_start:
   LDA   __local_display_shop__n
   BNE   .display_shop@rot@6
   LDY   #$02
   LDA   (__zpabi_display_shop__ev_0),Y
   STA   SHOP_VALID_MASK
   JSR   set_cursor_row21
   LDY   #$28
   LDA   (shop),Y
   STA   __zpabi_scene_loop__scene
   JSR   scene_loop
   JSR   load_char_name_ptr
   JSR   setup_char_sprite
   JSR   next_slot@run0
   JMP   show_item

browse_handler:
   SUBROUTINE

   JSR   next_slot
   JMP   show_item

apply_shop_effect:
   SUBROUTINE

   LDA   ENTITY_PTR
   STA   __local_apply_shop_effect__rec
   LDA   ENTITY_PTR+1
   STA   __local_apply_shop_effect__3
   LDA   __zpabi_apply_shop_effect__n
   CMP   #$03
   BCS   .cond_else@56
   LDA   #$07
   STA   __local_apply_shop_effect__2
   JMP   .cond_end@57
.cond_else@56:
   LDA   #$09
   STA   __local_apply_shop_effect__2
.cond_end@57:
   LDX   __zpabi_apply_shop_effect__n
   LDA   SHOP_EFFECT_TABLE,X
   CLC
   ADC   LOCATION_STYLE_BITS
   LDY   __local_apply_shop_effect__2
   STA   (ENTITY_PTR),Y
   LDA   (__local_apply_shop_effect__rec),Y
   STA   __local_apply_shop_effect__1
   LDY   #$05
   LDA   (ENTITY_PTR),Y
   LSR
   LSR
   LSR
   AND   #$03
   CLC
   ADC   __local_apply_shop_effect__1
   LDY   __local_apply_shop_effect__2
   STA   (__local_apply_shop_effect__rec),Y
   RTS

pending_xp:
   SUBROUTINE

   LDA   __zpabi_pending_xp__armor
   LSR
   LSR
   LSR
   LSR
   AND   #$07
   RTS

award_xp:
   SUBROUTINE

   LDY   #$0B
   LDA   (ENTITY_PTR),Y
   STA   __local_award_xp__1
   AND   #$E0
   STA   (ENTITY_PTR),Y
   LDA   __local_award_xp__1
   AND   #$1F
   STA   __local_award_xp__1
   LDY   #$08
   LDA   (ENTITY_PTR),Y
   STA   __zpabi_pending_xp__armor
   JSR   pending_xp
   CLC
   ADC   __local_award_xp__1
   STA   __local_award_xp__2
   LDX   __zpabi_award_xp__n
   LDA   XP_AWARD_TABLE,X
   CLC
   ADC   LOCATION_STYLE_BITS
   STA   __zpabi_pending_xp__armor
   JMP   .loop@11_start
.award_xp@rot@7:
   LDA   __zpabi_pending_xp__armor
   SBC   #$10
   STA   __zpabi_pending_xp__armor
.loop@11_start:
   LDY   #$08
   STA   (ENTITY_PTR),Y
   JSR   pending_xp
   STA   __local_award_xp__0
   CMP   __local_award_xp__2
   BCS   .award_xp@rot@7
   LDY   #$0B
   LDA   (ENTITY_PTR),Y
   STA   __local_award_xp__1
   LDA   __local_award_xp__2
   SEC
   SBC   __local_award_xp__0
   ORA   __local_award_xp__1
   STA   (ENTITY_PTR),Y
   RTS

shop_buy_handler:
   SUBROUTINE

   LDY   #$0D
   LDA   (ENTITY_PTR),Y
   STA   __local_shop_buy_handler__1
   INY
   LDA   (ENTITY_PTR),Y
   STA   __local_shop_buy_handler__0
   LDA   __local_shop_buy_handler__1
   SEC
   SBC   SHOP_PRICE
   LDA   __local_shop_buy_handler__0
   SBC   SHOP_PRICE+1
   BCS   .if_end@59
   JSR   set_cursor_row21
   LDA   #$31
   STA   __zpabi_display_message__index
   JSR   display_message
   LDA   #$3A
   RTS
.if_end@59:
   LDA   SHOP_PRICE
   STA   __zpabi_modify_current_stats__amount_0
   LDA   SHOP_PRICE+1
   STA   __zpabi_modify_current_stats__amount_1
   LDA   #$00
   STA   __zpabi_modify_current_stats__add
   JSR   modify_current_stats
   JSR   set_cursor_row21
   LDA   #$26
   STA   __zpabi_display_message__index
   JSR   display_message
   JSR   current_slot
   LDY   #$00
   LDA   (current_slot@ret),Y
   STA   __local_shop_buy_handler__3
   INY
   LDA   (current_slot@ret),Y
   STA   __local_shop_buy_handler__2
   LDX   #$00
   BEQ   .loop@12_start
.shop_buy_handler@rot@8:
   ASL
   TAY
   LDA   WEAPON_EFFECT_ADDRS,X
   STA   __local_shop_buy_handler__1
   LDA   __local_shop_buy_handler__2
   CMP   WEAPON_EFFECT_ADDRS+5,X
   BNE   .if_else@61
   LDA   __local_shop_buy_handler__3
   CMP   __local_shop_buy_handler__1
   BNE   .if_else@61
   STX   __zpabi_apply_shop_effect__n
   JSR   apply_shop_effect
   LDX   __zpabi_apply_shop_effect__n
   JMP   .if_end@60
.if_else@61:
   LDA   ARMOR_EFFECT_ADDRS,Y
   STA   __local_shop_buy_handler__1
   LDA   __local_shop_buy_handler__2
   CMP   ARMOR_EFFECT_ADDRS+1,Y
   BNE   .if_end@62
   LDA   __local_shop_buy_handler__3
   CMP   __local_shop_buy_handler__1
   BNE   .if_end@62
   STX   __zpabi_award_xp__n
   STX   __zpabi_apply_shop_effect__n
   JSR   award_xp
   LDX   __zpabi_apply_shop_effect__n
.if_end@62:
.if_end@60:
   INX
.loop@12_start:
   TXA
   CMP   #$05
   BCC   .shop_buy_handler@rot@8
   LDA   #$3A
   RTS

WEAPON_EFFECT_ADDRS:
   DC.B  $0B, $11, $17, $1D, $23, $72, $72, $72, $72, $72

ARMOR_EFFECT_ADDRS:
   DC.W  $7249
   DC.W  $724D
   DC.W  $7251
   DC.W  $7255
   DC.W  $7259

__zp_init:
   SUBROUTINE

   LDA   #$00
   STA   shop
   STA   shop+1
   RTS
