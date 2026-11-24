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
TEXT_COL	EQU	$24
TEXT_ROW	EQU	$25
MONITOR_GBAS	EQU	$26
BASL	EQU	$28
DOS_ZP	EQU	$2A
DISK_VM_ZP	EQU	$50
DPTR	EQU	$60
__local_random_events__i	EQU	$80
__zpabi_modify_current_stats__add	EQU	$80
__zpabi_num_to_decimal__out_0	EQU	$80
__zpabi_update_event_list__value_0	EQU	$80
__local_adj_drop__0	EQU	$81
__local_check_can_leave__1	EQU	$81
__local_flush_pending_xp__1	EQU	$81
__local_scan_for_hostile__0	EQU	$81
__local_trigger_scene_event__ev	EQU	$81
__zpabi_num_to_decimal__out_1	EQU	$81
__zpabi_update_event_list__value_1	EQU	$81
__local_adj_drop__1	EQU	$82
__local_drop_gold_exec__0	EQU	$82
__local_flush_pending_xp__2	EQU	$82
__local_pick_random_mob__index	EQU	$82
__local_pickup_treasure__gold_hi	EQU	$82
__local_scan_for_hostile__1	EQU	$82
__local_trigger_scene_event__1	EQU	$82
__local_adj_drop__down	EQU	$83
__local_drop_gold_exec__1	EQU	$83
__local_pickup_treasure__0	EQU	$83
__local_process_char_events__e	EQU	$83
__local_event_faction_scene_cont__index	EQU	$84
__local_pick_random_mob__1	EQU	$84
__local_pickup_treasure__i	EQU	$84
__local_process_char_events__1	EQU	$84
__local_update_event_list__0	EQU	$84
__local_init_drop_gold__1	EQU	$85
__local_pick_random_mob__2	EQU	$85
__local_pickup_treasure__1	EQU	$85
__local_process_char_events__index	EQU	$85
__local_update_event_list__pos	EQU	$85
__local_event_spawn_mob__index	EQU	$86
__local_init_drop_gold__mask	EQU	$86
__local_pick_random_mob__remaining	EQU	$86
__local_pickup_treasure__2	EQU	$86
__local_update_event_list__e	EQU	$86
__local_init_drop_gold__2	EQU	$87
__local_pick_random_char__0	EQU	$87
__local_pickup_treasure__ev	EQU	$87
__local_update_event_list__1	EQU	$87
__local_find_mob_by_faction__faction	EQU	$88
__local_init_drop_gold__3	EQU	$88
__local_pickup_treasure__3	EQU	$88
__local_update_event_list__2	EQU	$88
__local_init_drop_gold__4	EQU	$89
__local_inc_stat__1	EQU	$8A
__local_inc_stat__field	EQU	$8B
__local_inc_stat__2	EQU	$8C
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
; def adj_drop params=__zpabi_adj_drop__down locals=__local_adj_drop__0,__local_adj_drop__1,__local_adj_drop__down indirect=false in_cycle=false param_regs=A
; def adj_drop_dec params= locals=__local_adj_drop_dec__0 indirect=false in_cycle=false
; def adj_drop_inc params= locals=__local_adj_drop_inc__0 indirect=false in_cycle=false
; def check_can_leave params= locals=__local_check_can_leave__0,__local_check_can_leave__1 indirect=false in_cycle=false carry=true
; def check_location_match params= locals=__local_check_location_match__0 indirect=false in_cycle=false
; def check_scene_events params= locals= indirect=false in_cycle=false
; def drop_gold_exec params= locals=__local_drop_gold_exec__0,__local_drop_gold_exec__1 indirect=false in_cycle=false
; def event_faction_scene_cont params=__zpabi_event_faction_scene_cont__index locals=__local_event_faction_scene_cont__0,__local_event_faction_scene_cont__1,__local_event_faction_scene_cont__index indirect=false in_cycle=false param_regs=A
; def event_spawn_mob params= locals=__local_event_spawn_mob__0,__local_event_spawn_mob__index indirect=false in_cycle=false
; def find_mob_by_faction params=__zpabi_find_mob_by_faction__faction locals=__local_find_mob_by_faction__link,__local_find_mob_by_faction__faction indirect=false in_cycle=false param_regs=A carry=true
; def flush_pending_xp params= locals=__local_flush_pending_xp__0,__local_flush_pending_xp__1,__local_flush_pending_xp__2 indirect=false in_cycle=false
; def inc_stat params=__zpabi_inc_stat__field_0,__zpabi_inc_stat__field_1 locals=__local_inc_stat__0,__local_inc_stat__1,__local_inc_stat__field,__local_inc_stat__2 indirect=false in_cycle=false param_regs=A,X
; def init_drop_display params= locals=__local_init_drop_display__0,__local_init_drop_display__1,__local_init_drop_display__i indirect=false in_cycle=false
; def init_drop_gold params= locals=__local_init_drop_gold__0,__local_init_drop_gold__1,__local_init_drop_gold__mask,__local_init_drop_gold__2,__local_init_drop_gold__3,__local_init_drop_gold__4 indirect=false in_cycle=false
; def pick_random_char params= locals=__local_pick_random_char__0,__local_pick_random_char__1 indirect=false in_cycle=false
; def pick_random_mob params= locals=__local_pick_random_mob__index,__local_pick_random_mob__0,__local_pick_random_mob__1,__local_pick_random_mob__2,__local_pick_random_mob__remaining indirect=false in_cycle=false
; def pickup_treasure params=__zpabi_pickup_treasure__ev_0,__zpabi_pickup_treasure__ev_1 locals=__local_pickup_treasure__gold_hi,__local_pickup_treasure__0,__local_pickup_treasure__i,__local_pickup_treasure__1,__local_pickup_treasure__2,__local_pickup_treasure__ev,__local_pickup_treasure__3 indirect=false in_cycle=false param_regs=A,X
; def process_char_events params=__zpabi_process_char_events__index locals=__local_process_char_events__0,__local_process_char_events__e,__local_process_char_events__1,__local_process_char_events__index indirect=false in_cycle=false param_regs=A
; def random_events params= locals=__local_random_events__i,__local_random_events__0 indirect=false in_cycle=false
; def scan_and_reorder params= locals= indirect=false in_cycle=false
; def scan_for_hostile params= locals=__local_scan_for_hostile__link,__local_scan_for_hostile__0,__local_scan_for_hostile__1 indirect=false in_cycle=false
; def trigger_scene_event params=__zpabi_trigger_scene_event__ev_0,__zpabi_trigger_scene_event__ev_1 locals=__local_trigger_scene_event__0,__local_trigger_scene_event__ev,__local_trigger_scene_event__1 indirect=false in_cycle=false param_regs=A,X
; def update_event_list params=__zpabi_update_event_list__pos,__zpabi_update_event_list__value_0,__zpabi_update_event_list__value_1 locals=__local_update_event_list__0,__local_update_event_list__pos,__local_update_event_list__e,__local_update_event_list__1,__local_update_event_list__2,__local_update_event_list__3 indirect=false in_cycle=false param_regs=A,, changes=__zpabi_update_event_list__value_0
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
; ext check_encounter params=__zpabi_check_encounter__pos param_regs=A
; ext check_hostile params=__zpabi_check_hostile__other_0,__zpabi_check_hostile__other_1 param_regs=A,X carry=true
; ext check_threats_here params=
; ext clamp_char_field params=
; ext classify_entity_type params=__zpabi_classify_entity_type__e_0,__zpabi_classify_entity_type__e_1 param_regs=A,X
; ext clear_and_display params=__zpabi_clear_and_display__strings_0,__zpabi_clear_and_display__strings_1 param_regs=A,X
; ext clear_map params=
; ext clear_restricted params=
; ext clear_treasure_digits params=
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
; ext enter_char_select params=
; ext enter_combat_state params=__zpabi_enter_combat_state__pos param_regs=A
; ext enter_location params=
; ext enter_save_game_slot params=
; ext enter_scenario_disk params=
; ext fill_map params=
; ext find_attack_target params=
; ext find_entity_at_pos params=__zpabi_find_entity_at_pos__pos param_regs=A
; ext find_first_pc params= carry=true
; ext find_msb params=__zpabi_find_msb__v_0,__zpabi_find_msb__v_1 param_regs=A,X
; ext find_nearest_event params=
; ext find_next_pc_wrap params=
; ext find_prev_pc params=
; ext first_group_member params=
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
; ext process_scene_groups params=
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
; ext tune_player params=__zpabi_tune_player__tune,__zpabi_tune_player__strength param_regs=A,X
; ext use_scenario_disk params=
; ext verify_disk_io params=
; ext verify_integrity params=
; call adj_drop -> init_drop_gold
; call adj_drop_dec -> adj_drop
; call adj_drop_inc -> adj_drop
; call check_can_leave -> display_message
; call check_can_leave -> print_mob_name
; call check_can_leave -> scroll_status_line
; call check_can_leave -> set_cursor_row21
; call check_can_leave -> step_prng
; call check_location_match -> get_char_record
; call check_scene_events -> load_char_to_fa
; call check_scene_events -> scan_and_reorder
; call check_scene_events -> scan_for_hostile
; call check_scene_events -> scene_loop
; call check_scene_events -> scroll_status_line
; call check_scene_events -> set_cursor_row21
; call drop_gold_exec -> display_message
; call drop_gold_exec -> modify_current_stats
; call drop_gold_exec -> random_in_range
; call drop_gold_exec -> set_cursor_row21
; call drop_gold_exec -> update_event_list
; call event_faction_scene_cont -> check_location_match
; call event_faction_scene_cont -> event_spawn_mob
; call event_faction_scene_cont -> find_mob_by_faction
; call event_faction_scene_cont -> scene_setup
; call event_spawn_mob -> clamp_char_field
; call event_spawn_mob -> find_mob_by_faction
; call event_spawn_mob -> pick_random_char
; call event_spawn_mob -> reorder_char
; call find_mob_by_faction -> get_mob_data
; call flush_pending_xp -> display_message
; call flush_pending_xp -> set_cursor_row21
; call init_drop_display -> num_to_decimal
; call init_drop_gold -> display_message
; call init_drop_gold -> set_cursor_row21
; call pick_random_char -> get_char_record
; call pick_random_char -> random_in_range
; call pick_random_mob -> get_char_record
; call pick_random_mob -> pick_random_char
; call pick_random_mob -> random_in_range
; call pickup_treasure -> apply_damage_to_current
; call pickup_treasure -> commit_move
; call pickup_treasure -> display_message_loop
; call pickup_treasure -> inc_stat
; call pickup_treasure -> modify_current_stats
; call pickup_treasure -> num_to_decimal
; call pickup_treasure -> random_in_range
; call pickup_treasure -> scene_loop
; call pickup_treasure -> set_char_state
; call pickup_treasure -> set_cursor_row21
; call process_char_events -> check_location_match
; call random_events -> event_faction_scene_cont
; call random_events -> event_spawn_mob
; call random_events -> pick_random_char
; call random_events -> pick_random_mob
; call random_events -> process_char_events
; call random_events -> step_prng
; call scan_and_reorder -> reorder_char
; call scan_and_reorder -> scan_for_hostile
; call scan_for_hostile -> check_hostile
; call scan_for_hostile -> get_mob_data
; call trigger_scene_event -> apply_damage_to_current
; call trigger_scene_event -> commit_move
; call trigger_scene_event -> random_in_range
; call trigger_scene_event -> scene_loop
; call trigger_scene_event -> set_char_state
; call update_event_list -> display_message
; call update_event_list -> set_cursor_row21
; item check_can_leave section=.text local=false zero=false
; item update_event_list section=.text local=false zero=false
; item trigger_scene_event section=.text local=false zero=false
; item inc_stat section=.text local=true zero=false
; item pickup_treasure section=.text local=false zero=false
; item check_location_match section=.text local=true zero=false
; item pick_random_char section=.text local=true zero=false
; item pick_random_mob section=.text local=true zero=false
; item process_char_events section=.text local=true zero=false
; item find_mob_by_faction section=.text local=true zero=false
; item event_spawn_mob section=.text local=true zero=false
; item event_faction_scene_cont section=.text local=true zero=false
; item random_events section=.text local=false zero=false
; item random_events@run1 section=.text local=true zero=false
; item scan_for_hostile section=.text local=true zero=false
; item scan_and_reorder section=.text local=true zero=false
; item check_scene_events section=.text local=false zero=false
; item flush_pending_xp section=.text local=false zero=false
; item init_drop_gold section=.text local=false zero=false
; item init_drop_display section=.text local=false zero=false
; item adj_drop section=.text local=true zero=false
; item adj_drop@run2 section=.text local=true zero=false
; item adj_drop@run0 section=.text local=true zero=false
; item adj_drop_inc section=.text local=false zero=false
; item adj_drop_dec section=.text local=false zero=false
; item drop_gold_exec section=.text local=false zero=false
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
; zp MENU_DESC_PTR size=2 at=$C0 def=false init=
; zp MENU_OPTION_PTR size=2 at=$C2 def=false init=
; zp TUNE_PC size=2 at=$E4 def=false init=
; zp MOB_PTR size=2 at=$F4 def=false init=
; zp ENTITY_PTR size=2 at=$F8 def=false init=
; zp CHAR_PTR size=2 at=$FA def=false init=
; runtime hargs_size=10 hargs_need=0
; @zp-link-meta-end

check_can_leave:
   SUBROUTINE

   JSR   step_prng
   SEC
   SBC   #$40
   LDA   #$00
   ROL
   STA   __local_check_can_leave__1
   JSR   set_cursor_row21
   JSR   print_mob_name
   JSR   scroll_status_line
   LDA   __local_check_can_leave__1
   BEQ   .cond_else@0
   LDA   #$1C
   BNE   .cond_end@1
.cond_else@0:
   LDA   #$1D
.cond_end@1:
   JSR   display_message
   LDA   __local_check_can_leave__1
   CMP   #$01
   RTS

update_event_list:
   SUBROUTINE

   STA   __local_update_event_list__pos
   LDA   __zpabi_update_event_list__value_0
   ORA   __zpabi_update_event_list__value_1
   BNE   .if_end@2
   RTS
.if_end@2:
   LDA   __zpabi_update_event_list__value_1
   AND   #$07
   STA   __local_update_event_list__2
   LDY   #$06
   LDA   (CHAR_PTR),Y
   STA   __local_update_event_list__e
   INY
   LDA   (CHAR_PTR),Y
   STA   __local_update_event_list__1
.loop@0_start:
   LDY   #$00
   LDA   (__local_update_event_list__e),Y
   CMP   #$FF
   BNE   .if_end@3
   JSR   set_cursor_row21
   LDA   #$2A
   JMP   display_message
.if_end@3:
   CMP   #$FE
   BNE   .if_end@4
   LDA   __local_update_event_list__pos
   LDY   #$00
   STA   (__local_update_event_list__e),Y
   LDA   #$C0
   ORA   __local_update_event_list__2
   INY
   STA   (__local_update_event_list__e),Y
   LDA   __zpabi_update_event_list__value_0
   INY
   STA   (__local_update_event_list__e),Y
   RTS
.if_end@4:
   CMP   __local_update_event_list__pos
   BNE   .if_end@9
   LDY   #$01
   LDA   (__local_update_event_list__e),Y
   STA   __local_update_event_list__0
   CMP   #$C0
   BCC   .if_end@9
   CMP   #$D1
   BCS   .if_end@9
   INY
   LDA   (__local_update_event_list__e),Y
   ADC   __zpabi_update_event_list__value_0
   STA   __local_update_event_list__pos
   LDA   __local_update_event_list__0
   ADC   __local_update_event_list__2
   DEY
   STA   (__local_update_event_list__e),Y
   LDA   __local_update_event_list__pos
   INY
   STA   (__local_update_event_list__e),Y
   RTS
.if_end@9:
   LDA   __local_update_event_list__e
   CLC
   ADC   #$03
   STA   __local_update_event_list__e
   BCC   .loop@0_start
   INC   __local_update_event_list__1
   JMP   .loop@0_start

trigger_scene_event:
   SUBROUTINE

   STA   __local_trigger_scene_event__ev
   STX   __local_trigger_scene_event__1
   LDA   #$00
   JSR   set_char_state
   LDA   __local_trigger_scene_event__ev
   STA   SAVED_EVENT_PTR
   LDA   __local_trigger_scene_event__1
   STA   SAVED_EVENT_PTR+1
   JSR   commit_move
   LDY   #$02
   LDA   (__local_trigger_scene_event__ev),Y
   JSR   scene_loop
   LDY   #$01
   LDA   (__local_trigger_scene_event__ev),Y
   CMP   #$D3
   BCS   .if_end@10
   RTS
.if_end@10:
   LDA   #$FE
   LDY   #$00
   STA   (__local_trigger_scene_event__ev),Y
   INY
   LDA   (__local_trigger_scene_event__ev),Y
   CMP   #$D3
   BNE   .if_end@11
   RTS
.if_end@11:
   LDA   #$0F
   JSR   scene_loop
   LDA   #$03
   JSR   random_in_range
   CLC
   ADC   #$01
   JMP   apply_damage_to_current

inc_stat:
   SUBROUTINE

   STA   __local_inc_stat__field
   STX   __local_inc_stat__2
   LDY   #$00
   LDA   (__local_inc_stat__field),Y
   STA   __local_inc_stat__1
   AND   #$1F
   CMP   #$1F
   BEQ   .if_end@12
   LDA   __local_inc_stat__1
   CLC
   ADC   #$01
   STA   (__local_inc_stat__field),Y
.if_end@12:
   RTS

pickup_treasure:
   SUBROUTINE

   STA   __local_pickup_treasure__ev
   STX   __local_pickup_treasure__3
   LDA   #$00
   JSR   set_char_state
   LDA   __local_pickup_treasure__ev
   STA   MOB_PTR
   LDA   __local_pickup_treasure__3
   STA   MOB_PTR+1
   JSR   commit_move
   LDY   #$01
   LDA   (__local_pickup_treasure__ev),Y
   CMP   #$D1
   BNE   .if_end@13
   LDA   #$0B
   JSR   scene_loop
   LDA   ENTITY_PTR
   CLC
   ADC   #$05
   STA   __local_pickup_treasure__gold_hi
   LDA   ENTITY_PTR+1
   ADC   #$00
   TAX
   LDA   __local_pickup_treasure__gold_hi
   JSR   inc_stat
   LDA   ENTITY_PTR
   CLC
   ADC   #$0B
   STA   __local_pickup_treasure__gold_hi
   LDA   ENTITY_PTR+1
   ADC   #$00
   TAX
   LDA   __local_pickup_treasure__gold_hi
   JSR   inc_stat
   LDA   #$03
   JSR   random_in_range
   ORA   #$00
   BEQ   .if_end@14
   RTS
.if_end@14:
   LDA   #$0C
   JSR   scene_loop
   LDA   #$FE
   LDY   #$00
   STA   (__local_pickup_treasure__ev),Y
   RTS
.if_end@13:
   CMP   #$C9
   BCS   .if_else@16
   SEC
   SBC   #$C0
   JMP   .if_end@15
.if_else@16:
   LDA   #$06
   JSR   random_in_range
   AND   #$03
   STA   __local_pickup_treasure__0
   LDY   #$04
   LDA   (ENTITY_PTR),Y
   CMP   #$15
   BCS   .if_end@17
   JSR   set_cursor_row21
   LDA   #$36
   CLC
   ADC   __local_pickup_treasure__0
   JSR   scene_loop
.if_end@17:
   LDA   __local_pickup_treasure__0
   BEQ   .case@3
   CMP   #$01
   BEQ   .case@4
   CMP   #$02
   BEQ   .case@5
   BNE   .default@6
.case@3:
   LDA   #$05
   JSR   random_in_range
   CLC
   ADC   #$01
   JSR   apply_damage_to_current
   JMP   .switch@2_break
.case@4:
   LDA   #$08
   JSR   random_in_range
   CLC
   ADC   #$02
   JSR   apply_damage_to_current
   JMP   .switch@2_break
.case@5:
   LDY   #$0B
   LDA   (ENTITY_PTR),Y
   AND   #$1F
   BEQ   .switch@2_break
   LDA   (ENTITY_PTR),Y
   SEC
   SBC   #$01
   STA   (ENTITY_PTR),Y
   JMP   .switch@2_break
.default@6:
   LDA   #$00
   LDY   #$0D
   STA   (ENTITY_PTR),Y
   INY
   STA   (ENTITY_PTR),Y
.switch@2_break:
   LDY   #$02
   LDA   (ENTITY_PTR),Y
   CMP   CURRENT_ROOM
   BEQ   .if_end@19
   DEY
   LDA   (__local_pickup_treasure__ev),Y
   SEC
   SBC   #$09
   STA   (__local_pickup_treasure__ev),Y
   RTS
.if_end@19:
   LDY   #$01
   LDA   (__local_pickup_treasure__ev),Y
   SEC
   SBC   #$C9
.if_end@15:
   LDY   #$02
   STA   __local_pickup_treasure__1
   LDA   (__local_pickup_treasure__ev),Y
   STA   __local_pickup_treasure__2
   LDA   #$01
   STA   __zpabi_modify_current_stats__add
   LDX   __local_pickup_treasure__1
   LDA   __local_pickup_treasure__2
   JSR   modify_current_stats
   LDY   #$04
   LDA   (ENTITY_PTR),Y
   CMP   #$15
   BCS   .if_end@20
   LDA   #$00
   STA   __local_pickup_treasure__i
.pickup_treasure@rot@0:
   LDX   __local_pickup_treasure__i
   LDA   #$20
   STA   S_CHEST_MESSAGE+39,X
   STA   S_CHEST_MESSAGE+72,X
   INC   __local_pickup_treasure__i
   LDA   __local_pickup_treasure__i
   CMP   #$05
   BCC   .pickup_treasure@rot@0
   LDA   #<(S_CHEST_MESSAGE+39)
   STA   __zpabi_num_to_decimal__out_0
   LDA   #>(S_CHEST_MESSAGE+39)
   STA   __zpabi_num_to_decimal__out_1
   LDX   __local_pickup_treasure__1
   LDA   __local_pickup_treasure__2
   JSR   num_to_decimal
   LDY   #$0D
   LDA   (ENTITY_PTR),Y
   STA   __local_pickup_treasure__gold_hi
   INY
   LDA   (ENTITY_PTR),Y
   TAX
   LDA   #<(S_CHEST_MESSAGE+72)
   STA   __zpabi_num_to_decimal__out_0
   LDA   #>(S_CHEST_MESSAGE+72)
   STA   __zpabi_num_to_decimal__out_1
   LDA   __local_pickup_treasure__gold_hi
   JSR   num_to_decimal
   JSR   set_cursor_row21
   LDA   #$00
   STA   MSG_LINE_COUNT
   LDX   #>S_CHEST_MESSAGE
   LDA   #<S_CHEST_MESSAGE
   JSR   display_message_loop
.if_end@20:
   LDA   #$FE
   LDY   #$00
   STA   (__local_pickup_treasure__ev),Y
   RTS

check_location_match:
   SUBROUTINE

   LDA   ACTIVE_CHAR
   JSR   get_char_record
   CMP   LOCATION_POS
   BNE   .if_end@21
   LDA   #$00
   STA   LOCATION_POS
.if_end@21:
   RTS

pick_random_char:
   SUBROUTINE

   LDA   CURRENT_PLAYER
   SEC
   SBC   #$03
   JSR   random_in_range
   CLC
   ADC   #$02
   STA   __local_pick_random_char__0
   JSR   get_char_record
   STA   CHAR_PTR
   STX   CHAR_PTR+1
   LDA   __local_pick_random_char__0
   RTS

pick_random_mob:
   SUBROUTINE

   LDA   TOTAL_MOB_COUNT
   BNE   .vthread@0
   JMP   pick_random_char
.loop@7_start:
   LDA   TOTAL_MOB_COUNT
.vthread@0:
   JSR   random_in_range
   STA   __local_pick_random_mob__remaining
   ORA   #$00
   BEQ   .loop@7_start
   LDA   #$01
   STA   __local_pick_random_mob__index
.loop@8_start:
   LDA   __local_pick_random_mob__index
   JSR   get_char_record
   STA   __local_pick_random_mob__1
   STX   __local_pick_random_mob__2
   LDY   #$08
   LDA   __local_pick_random_mob__remaining
   SEC
   SBC   (__local_pick_random_mob__1),Y
   STA   __local_pick_random_mob__remaining
   BMI   .pick_random_mob@scfold@0
   BEQ   .pick_random_mob@scfold@0
   INC   __local_pick_random_mob__index
   BNE   .loop@8_start
.pick_random_mob@scfold@0:
   STA   MOB_PTR
   LDA   __local_pick_random_mob__1
   STA   CHAR_PTR
   STX   CHAR_PTR+1
   LDA   __local_pick_random_mob__index
   RTS

process_char_events:
   SUBROUTINE

   STA   __local_process_char_events__index
   LDY   #$06
   LDA   (CHAR_PTR),Y
   STA   __local_process_char_events__e
   INY
   LDA   (CHAR_PTR),Y
   STA   __local_process_char_events__1
   JMP   .loop@9_start
.process_char_events@rot@1:
   LDY   #$01
   LDA   (__local_process_char_events__e),Y
   CMP   #$40
   BCC   .if_end@27
   CMP   #$80
   BCS   .if_end@27
   SEC
   SBC   #$40
   STA   (__local_process_char_events__e),Y
   LDA   __local_process_char_events__index
   STA   ACTIVE_CHAR
   JMP   check_location_match
.if_end@27:
   LDA   __local_process_char_events__e
   CLC
   ADC   #$03
   STA   __local_process_char_events__e
   BCC   .loop@9_start
   INC   __local_process_char_events__1
.loop@9_start:
   LDY   #$00
   LDA   (__local_process_char_events__e),Y
   CMP   #$FF
   BNE   .process_char_events@rot@1
   RTS

find_mob_by_faction:
   SUBROUTINE

   STA   __local_find_mob_by_faction__faction
   LDY   #$02
   LDA   (CHAR_PTR),Y
   JMP   .loop@10_start
.find_mob_by_faction@rot@2:
   JSR   get_mob_data
   STA   MOB_PTR
   STX   MOB_PTR+1
   LDY   #$0F
   LDA   (MOB_PTR),Y
   BPL   .if_end@30
   LDA   (MOB_PTR),Y
   AND   #$70
   CMP   __local_find_mob_by_faction__faction
   BNE   .if_end@30
   SEC
   RTS
.if_end@30:
   LDY   #$02
   LDA   (MOB_PTR),Y
.loop@10_start:
   BNE   .find_mob_by_faction@rot@2
   CLC
   RTS

event_spawn_mob:
   SUBROUTINE

   JSR   pick_random_char
   STA   __local_event_spawn_mob__index
   LDY   #$08
   LDA   (CHAR_PTR),Y
   BEQ   .if_end@31
   JSR   pick_random_char
   STA   __local_event_spawn_mob__index
   LDY   #$08
   LDA   (CHAR_PTR),Y
   BEQ   .if_end@32
   RTS
.if_end@32:
.if_end@31:
   LDY   #$05
   LDA   (CHAR_PTR),Y
   AND   #$60
   JSR   find_mob_by_faction
   BCS   .if_end@33
   RTS
.if_end@33:
   LDA   __local_event_spawn_mob__index
   STA   SOURCE_CHAR
   LDA   #$00
   STA   ACTIVE_CHAR
   LDX   MOB_PTR+1
   LDA   MOB_PTR
   JSR   reorder_char
   JMP   clamp_char_field

event_faction_scene_cont:
   SUBROUTINE

   STA   __local_event_faction_scene_cont__index
   LDA   CHAR_PTR+1
   CMP   #>(LOCATIONS+9)
   BNE   .if_end@34
   LDA   CHAR_PTR
   CMP   #<(LOCATIONS+9)
   BNE   .if_end@34
   RTS
.if_end@34:
   LDY   #$05
   LDA   (CHAR_PTR),Y
   AND   #$60
   LDX   #<LOCATIONS
   STX   CHAR_PTR
   LDX   #>LOCATIONS
   STX   CHAR_PTR+1
   JSR   find_mob_by_faction
   BCS   .if_end@35
   JMP   event_spawn_mob
.if_end@35:
   LDA   #$00
   STA   SOURCE_CHAR
   LDA   __local_event_faction_scene_cont__index
   STA   ACTIVE_CHAR
   JSR   scene_setup
   JMP   check_location_match

random_events:
   SUBROUTINE

   JSR   random_events@run1
   BCS   .if_end@36
   JSR   pick_random_mob
   JSR   process_char_events
.if_end@36:
   JSR   random_events@run1
   BCS   .if_end@37
   JSR   pick_random_char
   JSR   process_char_events
.if_end@37:
   JSR   random_events@run1
   BCS   .if_end@38
   JSR   pick_random_mob
   JSR   event_faction_scene_cont
.if_end@38:
   JSR   random_events@run1
   BCS   .if_end@39
   JSR   event_spawn_mob
.if_end@39:
   JSR   random_events@run1
   BCS   .if_end@40
   JSR   event_spawn_mob
.if_end@40:
   JSR   random_events@run1
   BCS   .if_end@41
   LDA   #$04
   STA   __local_random_events__i
.random_events@rot@3:
   JSR   event_spawn_mob
   DEC   __local_random_events__i
   BNE   .random_events@rot@3
   JSR   pick_random_char
   JSR   event_faction_scene_cont
.if_end@41:
   RTS

random_events@run1:
   SUBROUTINE

   JSR   step_prng
   CMP   EVENT_THRESHOLD
   RTS

scan_for_hostile:
   SUBROUTINE

   LDA   #$00
   STA   SCENE_FOUND_FLAG
   STA   SCENE_HOSTILE_FLAG
   LDA   SCENE_GROUP_IDX
   JSR   get_mob_data
   STA   MOB_PTR
   STX   MOB_PTR+1
   LDY   #$02
   LDA   (CHAR_PTR),Y
   JMP   .loop@12_start
.scan_for_hostile@rot@4:
   CMP   SCENE_GROUP_IDX
   BNE   .if_end@42
   LDY   #$01
   STY   SCENE_FOUND_FLAG
.if_end@42:
   JSR   get_mob_data
   STA   __local_scan_for_hostile__0
   STX   __local_scan_for_hostile__1
   JSR   check_hostile
   BCC   .if_end@43
   LDA   #$01
   STA   SCENE_HOSTILE_FLAG
.if_end@43:
   LDY   #$02
   LDA   (__local_scan_for_hostile__0),Y
.loop@12_start:
   BNE   .scan_for_hostile@rot@4
   RTS

scan_and_reorder:
   SUBROUTINE

   JSR   scan_for_hostile
   LDA   SCENE_FOUND_FLAG
   BEQ   .if_end@44
   LDX   MOB_PTR+1
   LDA   MOB_PTR
   JSR   reorder_char
.if_end@44:
   RTS

check_scene_events:
   SUBROUTINE

   LDA   SCENE_HOSTILE_DONE
   BEQ   .if_end@45
   RTS
.if_end@45:
   LDA   #$40
   STA   SCENE_GROUP_IDX
   JSR   scan_for_hostile
   LDA   SCENE_FOUND_FLAG
   BNE   .if_end@46
   RTS
.if_end@46:
   LDA   CURRENT_PLAYER
   CMP   #$01
   BEQ   .if_end@47
   LDA   SCENE_P3_DONE
   CMP   #$01
   BNE   .if_end@48
   RTS
.if_end@48:
   LDA   #$01
   STA   SCENE_P3_DONE
   JSR   set_cursor_row21
   LDA   #$05
   JMP   scene_loop
.if_end@47:
   LDA   SCENE_HOSTILE_FLAG
   BEQ   .if_end@49
   LDA   SCENE_P1_DONE
   CMP   #$01
   BNE   .if_end@50
   RTS
.if_end@50:
   LDA   #$01
   STA   SCENE_P1_DONE
   LDA   #$04
   JMP   scene_loop
.if_end@49:
   LDA   #$01
   STA   SCENE_HOSTILE_DONE
   LDA   #$06
   JSR   scene_loop
   LDA   #$03
   STA   SOURCE_CHAR
   JSR   load_char_to_fa
   LDA   #$00
   STA   ACTIVE_CHAR
   LDA   #$3F
   STA   SCENE_GROUP_IDX
   JSR   scan_and_reorder
   LDA   #$41
   STA   SCENE_GROUP_IDX
   JSR   scan_and_reorder
   LDA   #$42
   STA   SCENE_GROUP_IDX
   JSR   scan_and_reorder
   LDA   CURRENT_PLAYER
   JSR   load_char_to_fa
   LDA   SCENE_EVENT_RESULT
   BEQ   .if_end@51
   RTS
.if_end@51:
   LDA   #$07
   JSR   scene_loop
   LDA   SCENE_COMBAT_FLAG
   BNE   .if_end@52
   RTS
.if_end@52:
   JSR   scroll_status_line
   LDA   #$08
   JMP   scene_loop

flush_pending_xp:
   SUBROUTINE

   LDY   #$08
   LDA   (ENTITY_PTR),Y
   STA   __local_flush_pending_xp__2
   AND   #$80
   STA   (ENTITY_PTR),Y
   LDA   __local_flush_pending_xp__2
   AND   #$7F
   BNE   .if_end@53
   JSR   set_cursor_row21
   LDA   #$37
   JSR   display_message
   LDA   #$02
   RTS
.if_end@53:
   LDY   #$0B
   LDA   (ENTITY_PTR),Y
   STA   __local_flush_pending_xp__1
   LDA   __local_flush_pending_xp__2
   LSR
   LSR
   LSR
   LSR
   AND   #$07
   CLC
   ADC   __local_flush_pending_xp__1
   STA   (ENTITY_PTR),Y
   JSR   set_cursor_row21
   LDA   #$2B
   JSR   display_message
   LDA   #$00
   RTS

init_drop_gold:
   SUBROUTINE

   LDY   #$0D
   LDA   (ENTITY_PTR),Y
   STA   __local_init_drop_gold__4
   INY
   LDA   (ENTITY_PTR),Y
   STA   __local_init_drop_gold__3
   LDA   #$00
   STA   __local_init_drop_gold__mask
   LDY   #$80
   STY   __local_init_drop_gold__2
   JMP   .i2_loop@13_continue
.init_drop_gold@rot@5:
   LDA   __local_init_drop_gold__4
   AND   __local_init_drop_gold__mask
   STA   __local_init_drop_gold__1
   LDA   __local_init_drop_gold__3
   AND   __local_init_drop_gold__2
   ORA   __local_init_drop_gold__1
   BNE   .i2_loop@13_break
   LSR   __local_init_drop_gold__2
   ROR   __local_init_drop_gold__mask
.i2_loop@13_continue:
   LDA   __local_init_drop_gold__mask
   ORA   __local_init_drop_gold__2
   BNE   .init_drop_gold@rot@5
.i2_loop@13_break:
   LDA   __local_init_drop_gold__mask
   STA   DROP_AMOUNT
   LDA   __local_init_drop_gold__2
   STA   DROP_AMOUNT+1
   ORA   DROP_AMOUNT
   BNE   .if_end@56
   JSR   set_cursor_row21
   LDA   #$2E
   JSR   display_message
   LDA   #$15
   RTS
.if_end@56:
   LDA   DROP_AMOUNT+1
   LSR
   STA   DROP_STEP+1
   LDA   DROP_AMOUNT
   ROR
   STA   DROP_STEP
   LDA   #$1D
   RTS

init_drop_display:
   SUBROUTINE

   LDX   #$00
.init_drop_display@rot@6:
   LDA   #$20
   STA   S_DUCATS_TO_DROP+18,X
   INX
   CPX   #$05
   BCC   .init_drop_display@rot@6
   LDA   #<(S_DUCATS_TO_DROP+18)
   STA   __zpabi_num_to_decimal__out_0
   LDA   #>(S_DUCATS_TO_DROP+18)
   STA   __zpabi_num_to_decimal__out_1
   LDX   DROP_AMOUNT+1
   LDA   DROP_AMOUNT
   JMP   num_to_decimal

adj_drop:
   SUBROUTINE

   STA   __local_adj_drop__down
.loop@15_start:
   LDA   DROP_STEP
   ORA   DROP_STEP+1
   BNE   .if_end@57
   JMP   init_drop_gold
.if_end@57:
   LDA   __local_adj_drop__down
   BEQ   .if_end@58
   JSR   adj_drop@run0
   LSR   DROP_STEP+1
   ROR   DROP_STEP
   LDA   #$1D
   RTS
.if_end@58:
   LDA   DROP_AMOUNT
   CLC
   ADC   DROP_STEP
   JSR   adj_drop@run2
   ADC   DROP_STEP+1
   STA   DROP_AMOUNT+1
   LSR   DROP_STEP+1
   ROR   DROP_STEP
   LDY   #$0D
   LDA   (ENTITY_PTR),Y
   SEC
   SBC   DROP_AMOUNT
   INY
   LDA   (ENTITY_PTR),Y
   SBC   DROP_AMOUNT+1
   BCC   .if_end@59
   LDA   #$1D
   RTS
.if_end@59:
   LDA   DROP_STEP
   ASL
   STA   __local_adj_drop__0
   LDA   DROP_STEP+1
   ROL
   STA   __local_adj_drop__1
   LDA   DROP_AMOUNT
   SEC
   SBC   __local_adj_drop__0
   JSR   adj_drop@run2
   SBC   __local_adj_drop__1
   STA   DROP_AMOUNT+1
   JMP   .loop@15_start

adj_drop@run2:
   SUBROUTINE

   STA   DROP_AMOUNT
   LDA   DROP_AMOUNT+1
   RTS

adj_drop@run0:
   SUBROUTINE

   LDA   DROP_AMOUNT
   SEC
   SBC   DROP_STEP
   JSR   adj_drop@run2
   SBC   DROP_STEP+1
   STA   DROP_AMOUNT+1
   RTS

adj_drop_inc:
   SUBROUTINE

   LDA   #$01
   JMP   adj_drop

adj_drop_dec:
   SUBROUTINE

   LDA   #$00
   JMP   adj_drop

drop_gold_exec:
   SUBROUTINE

   LDA   #$00
   STA   __zpabi_modify_current_stats__add
   LDX   DROP_AMOUNT+1
   LDA   DROP_AMOUNT
   JSR   modify_current_stats
   LDA   #$19
   JSR   random_in_range
   CLC
   ADC   #$01
   STA   DROP_STEP
   LDA   #$00
   ADC   #$00
   STA   DROP_STEP+1
   JSR   set_cursor_row21
   JSR   adj_drop@run0
   STA   __local_drop_gold_exec__0
   LDA   DROP_AMOUNT
   STA   __local_drop_gold_exec__1
   LDA   __local_drop_gold_exec__0
   BMI   .jcmp_neg@0
   ORA   __local_drop_gold_exec__1
   BNE   .if_else@61
.jcmp_neg@0:
   LDA   #$2C
   JSR   display_message
   JMP   .if_end@60
.if_else@61:
   LDA   #$2D
   JSR   display_message
   LDY   #$03
   LDA   (ENTITY_PTR),Y
   LDX   DROP_AMOUNT
   STX   __zpabi_update_event_list__value_0
   LDX   DROP_AMOUNT+1
   STX   __zpabi_update_event_list__value_1
   JSR   update_event_list
.if_end@60:
   LDA   #$02
   RTS
