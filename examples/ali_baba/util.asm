NULL_PAGE_LOW	EQU	$00
TEXT_STREAM_PTR	EQU	$06
RWTS_IOB_PTR	EQU	$08
NULL_PAGE_HIGH	EQU	$0A
HARGS	EQU	$10
SSP	EQU	$1A
FP	EQU	$1C
ANIM_FRAME_CTR	EQU	$1E
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
__local_cold_restart__0	EQU	$80
__local_colrow_to_pos__row	EQU	$80
__local_delay_with_animation__n	EQU	$80
__local_find_prev_pc__prev	EQU	$80
__local_get_char_record__rec	EQU	$80
__local_idle_anim_handler__n	EQU	$80
__local_random_in_range__sample	EQU	$80
__zpabi_compute_checksum__end_0	EQU	$80
__zpabi_modify_current_stats__add	EQU	$80
__zpabi_num_to_decimal__out_0	EQU	$80
__zpabi_update_event_list__value_0	EQU	$80
__local_clamp_char_field__1	EQU	$81
__local_cold_restart__1	EQU	$81
__local_colrow_to_pos__col	EQU	$81
__local_find_prev_pc__start	EQU	$81
__local_get_char_record__index	EQU	$81
__local_random_in_range__shift	EQU	$81
__zpabi_compute_checksum__end_1	EQU	$81
__zpabi_num_to_decimal__out_1	EQU	$81
__zpabi_update_event_list__value_1	EQU	$81
__local_cold_restart__page	EQU	$82
__local_random_in_range__bound	EQU	$82
__local_random_in_range__scaled	EQU	$83
__local_compute_checksum__lo	EQU	$84
__local_random_in_range__mask	EQU	$84
__local_compute_checksum__hi	EQU	$85
__local_find_first_pc__0	EQU	$85
__local_compute_checksum__0	EQU	$86
__local_find_first_pc__1	EQU	$86
__local_step_prng__1	EQU	$86
__local_compute_checksum__p_0	EQU	$87
__local_copy_pc_name_to_buf__m	EQU	$87
__local_get_mob_data__index	EQU	$87
__local_step_prng__s	EQU	$87
__local_compute_checksum__p_1	EQU	$88
__local_copy_pc_name_to_buf__0	EQU	$88
__local_get_mob_data__0	EQU	$88
__local_copy_pc_name_to_buf__1	EQU	$89
__local_copy_pc_name_to_buf__i	EQU	$8A
__local_copy_pc_name_to_buf__left	EQU	$8B
__local_copy_pc_name_to_buf__2	EQU	$8C
__local_copy_pc_name_to_buf__3	EQU	$8D
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
; def anim_check_frame params= locals= indirect=false in_cycle=false
; def anim_tick_and_wait params= locals=__local_anim_tick_and_wait__0 indirect=false in_cycle=false
; def clamp_char_field params= locals=__local_clamp_char_field__0,__local_clamp_char_field__1 indirect=false in_cycle=false
; def cold_restart params= locals=__local_cold_restart__0,__local_cold_restart__1,__local_cold_restart__page indirect=false in_cycle=false
; def colrow_to_pos params=__zpabi_colrow_to_pos__col,__zpabi_colrow_to_pos__row locals=__local_colrow_to_pos__row,__local_colrow_to_pos__col indirect=false in_cycle=false param_regs=A,X
; def compute_checksum params=__zpabi_compute_checksum__p_0,__zpabi_compute_checksum__p_1,__zpabi_compute_checksum__end_0,__zpabi_compute_checksum__end_1 locals=__local_compute_checksum__lo,__local_compute_checksum__hi,__local_compute_checksum__0,__local_compute_checksum__p_0,__local_compute_checksum__p_1 indirect=false in_cycle=false param_regs=A,X,,
; def copy_pc_name_to_buf params=__zpabi_copy_pc_name_to_buf__m_0,__zpabi_copy_pc_name_to_buf__m_1 locals=__local_copy_pc_name_to_buf__m,__local_copy_pc_name_to_buf__0,__local_copy_pc_name_to_buf__1,__local_copy_pc_name_to_buf__i,__local_copy_pc_name_to_buf__left,__local_copy_pc_name_to_buf__2,__local_copy_pc_name_to_buf__3 indirect=false in_cycle=false param_regs=A,X
; def dec_wait_if_nonzero params= locals= indirect=false in_cycle=false
; def delay_with_animation params= locals=__local_delay_with_animation__n indirect=false in_cycle=false
; def dispatch_on_mode params= locals= indirect=false in_cycle=false
; def find_first_pc params= locals=__local_find_first_pc__index,__local_find_first_pc__0,__local_find_first_pc__1 indirect=false in_cycle=false carry=true
; def find_next_pc params= locals=__local_find_next_pc__0 indirect=false in_cycle=false
; def find_next_pc_wrap params= locals= indirect=false in_cycle=false
; def find_prev_pc params= locals=__local_find_prev_pc__prev,__local_find_prev_pc__start,__local_find_prev_pc__0 indirect=false in_cycle=false
; def get_char_record params=__zpabi_get_char_record__index locals=__local_get_char_record__rec,__local_get_char_record__index indirect=false in_cycle=false param_regs=A
; def get_mob_data params=__zpabi_get_mob_data__index locals=__local_get_mob_data__index,__local_get_mob_data__0,__local_get_mob_data__1,__local_get_mob_data__2 indirect=false in_cycle=false param_regs=A
; def idle_anim_handler params= locals=__local_idle_anim_handler__n indirect=false in_cycle=false
; def init_char_select params= locals=__local_init_char_select__0 indirect=false in_cycle=false
; def pos_to_colrow params=__zpabi_pos_to_colrow__pos locals=__local_pos_to_colrow__pos indirect=false in_cycle=false param_regs=A
; def random_in_range params=__zpabi_random_in_range__limit locals=__local_random_in_range__sample,__local_random_in_range__shift,__local_random_in_range__bound,__local_random_in_range__scaled,__local_random_in_range__mask indirect=false in_cycle=false param_regs=A
; def step_prng params= locals=__local_step_prng__0,__local_step_prng__1,__local_step_prng__s indirect=false in_cycle=false
; def tick_anim_counter params= locals=__local_tick_anim_counter__0 indirect=false in_cycle=false
; def timed_wait params= locals= indirect=false in_cycle=false
; def verify_disk_io params= locals= indirect=false in_cycle=false
; def verify_integrity params= locals=__local_verify_integrity__0,__local_verify_integrity__1 indirect=false in_cycle=false
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
; ext classify_entity_type params=__zpabi_classify_entity_type__e_0,__zpabi_classify_entity_type__e_1 param_regs=A,X
; ext clear_map params=
; ext clear_restricted params=
; ext commit_move params=
; ext configure_input params=
; ext continue_game params=
; ext dec_threshold_wrap params=
; ext disk_read params= carry=true
; ext disk_write params= carry=true
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
; ext find_nearest_event params=
; ext first_group_member params=
; ext flush_pending_xp params=
; ext font_pos_to_text_pos params=
; ext format_scenario_disk params=
; ext game_init params=
; ext get_entity_fontchar params=__zpabi_get_entity_fontchar__e_0,__zpabi_get_entity_fontchar__e_1 param_regs=A,X
; ext get_input_table_ptr params=
; ext handle_encounter params=__zpabi_handle_encounter__ev_0,__zpabi_handle_encounter__ev_1 param_regs=A,X
; ext handle_event_slot params=__zpabi_handle_event_slot__ev_0,__zpabi_handle_event_slot__ev_1 param_regs=A,X
; ext handle_mob_encounter params=
; ext hrcg_cout params=__zpabi_hrcg_cout__c param_regs=A
; ext hrcg_init params=
; ext inc_threshold_wrap params=
; ext init_attack_display params=
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
; ext teardown_input params=
; ext text_renderer params=
; ext text_renderer_no_pause params=
; ext toggle_sound params=
; ext trigger_scene_event params=__zpabi_trigger_scene_event__ev_0,__zpabi_trigger_scene_event__ev_1 param_regs=A,X
; ext tune_player params=__zpabi_tune_player__tune,__zpabi_tune_player__strength param_regs=A,X
; ext update_event_list params=__zpabi_update_event_list__pos,__zpabi_update_event_list__value_0,__zpabi_update_event_list__value_1 param_regs=A,,
; ext use_scenario_disk params=
; call anim_check_frame -> draw_blink_normal
; call anim_tick_and_wait -> anim_check_frame
; call anim_tick_and_wait -> tick_anim_counter
; call anim_tick_and_wait -> timed_wait
; call delay_with_animation -> anim_tick_and_wait
; call dispatch_on_mode -> anim_tick_and_wait
; call dispatch_on_mode -> delay_with_animation
; call find_first_pc -> copy_pc_name_to_buf
; call find_first_pc -> get_mob_data
; call find_next_pc -> copy_pc_name_to_buf
; call find_next_pc -> find_first_pc
; call find_next_pc -> get_mob_data
; call find_next_pc_wrap -> find_next_pc
; call find_prev_pc -> copy_pc_name_to_buf
; call find_prev_pc -> find_first_pc
; call find_prev_pc -> find_next_pc
; call find_prev_pc -> get_mob_data
; call idle_anim_handler -> WAIT
; call idle_anim_handler -> anim_check_frame
; call idle_anim_handler -> dec_wait_if_nonzero
; call idle_anim_handler -> tick_anim_counter
; call init_char_select -> find_first_pc
; call init_char_select -> scene_setup
; call random_in_range -> step_prng
; call tick_anim_counter -> draw_blink_alt
; call timed_wait -> WAIT
; call verify_integrity -> compute_checksum
; call verify_integrity -> integrity_fail
; item tick_anim_counter section=.text local=false zero=false
; item timed_wait section=.text local=false zero=false
; item anim_tick_and_wait section=.text local=false zero=false
; item anim_check_frame section=.text local=false zero=false
; item delay_with_animation section=.text local=false zero=false
; item dispatch_on_mode section=.text local=false zero=false
; item dec_wait_if_nonzero section=.text local=true zero=false
; item idle_anim_handler section=.text local=false zero=false
; item pos_to_colrow section=.text local=false zero=false
; item colrow_to_pos section=.text local=false zero=false
; item get_mob_data section=.text local=false zero=false
; item get_char_record section=.text local=false zero=false
; item clamp_char_field section=.text local=false zero=false
; item copy_pc_name_to_buf section=.text local=true zero=false
; item find_first_pc section=.text local=false zero=false
; item find_next_pc section=.text local=true zero=false
; item find_next_pc_wrap section=.text local=false zero=false
; item find_prev_pc section=.text local=false zero=false
; item init_char_select section=.text local=false zero=false
; item step_prng section=.text local=false zero=false
; item random_in_range section=.text local=false zero=false
; item compute_checksum section=.text.integrity local=true zero=false
; item verify_integrity section=.text.integrity local=false zero=false
; item verify_disk_io section=.text.integrity local=false zero=false
; item cold_restart section=.cold_restart local=false zero=false
; item ANIM_INDEX section=.data local=false zero=false
; item ANIM_WAIT_TABLE section=.rodata local=true zero=false
; item ANIM_FRAME_TABLE section=.rodata local=true zero=false
; item __zp_init section=.text local=false zero=false
; zp NULL_PAGE_LOW size=6 at=$00 def=false init=
; zp TEXT_STREAM_PTR size=2 at=$06 def=false init=
; zp RWTS_IOB_PTR size=2 at=$08 def=false init=
; zp NULL_PAGE_HIGH size=6 at=$0A def=false init=
; zp ANIM_FRAME_CTR size=1 at=auto def=true init=$03 weight=5 local=true
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

tick_anim_counter:
   SUBROUTINE

   DEC   ANIM_FRAME_CTR
   BNE   .if_end@0
   JSR   draw_blink_alt
.if_end@0:
   LDX   ANIM_INDEX
   LDA   ANIM_WAIT_TABLE,X
   STA   WAIT_LOOP_COUNT
   RTS

timed_wait:
   SUBROUTINE

   JMP   .loop@0_continue
.timed_wait@rot@0:
   LDA   #$50
   JSR   WAIT
   DEC   WAIT_LOOP_COUNT
.loop@0_continue:
   LDA   WAIT_LOOP_COUNT
   BNE   .timed_wait@rot@0
   RTS

anim_tick_and_wait:
   SUBROUTINE

   JSR   tick_anim_counter
   JSR   timed_wait
   JMP   anim_check_frame

anim_check_frame:
   SUBROUTINE

   LDA   ANIM_FRAME_CTR
   BNE   .if_end@1
   JSR   draw_blink_normal
   LDX   ANIM_INDEX
   LDA   ANIM_FRAME_TABLE,X
   STA   ANIM_FRAME_CTR
.if_end@1:
   RTS

delay_with_animation:
   SUBROUTINE

   LDA   #$04
   STA   __local_delay_with_animation__n
.loop@1_start:
   JSR   anim_tick_and_wait
   DEC   __local_delay_with_animation__n
   BNE   .loop@1_start
   RTS

dispatch_on_mode:
   SUBROUTINE

   LDA   SOUND_ON
   BEQ   .if_else@3
   JMP   anim_tick_and_wait
.if_else@3:
   JMP   delay_with_animation

dec_wait_if_nonzero:
   SUBROUTINE

   LDA   WAIT_LOOP_COUNT
   BEQ   .if_end@4
   DEC   WAIT_LOOP_COUNT
.if_end@4:
   RTS

idle_anim_handler:
   SUBROUTINE

   JMP   .loop@2_continue
.idle_anim_handler@rot@1:
   JSR   anim_check_frame
   JSR   tick_anim_counter
.loop@2_continue:
   LDA   WAIT_LOOP_COUNT
   BEQ   .idle_anim_handler@rot@1
   LDA   INPUT_MODE
   BNE   .if_else@6
   LDA   #$50
   JSR   WAIT
   DEC   WAIT_LOOP_COUNT
   RTS
.if_else@6:
   LDA   #$0C
   STA   __local_idle_anim_handler__n
.idle_anim_handler@rot@2:
   JSR   dec_wait_if_nonzero
   DEC   __local_idle_anim_handler__n
   BNE   .idle_anim_handler@rot@2
   RTS

pos_to_colrow:
   SUBROUTINE

   LDX   #$00
   BEQ   .loop@4_continue
.pos_to_colrow@rot@3:
   SBC   #$14
   INX
.loop@4_continue:
   CMP   #$14
   BCS   .pos_to_colrow@rot@3
   ORA   #$00
   RTS

colrow_to_pos:
   SUBROUTINE

   STA   __local_colrow_to_pos__col
   STX   __local_colrow_to_pos__row
   JMP   .loop@5_start
.colrow_to_pos@rot@4:
   LDA   __local_colrow_to_pos__col
   CLC
   ADC   #$14
   STA   __local_colrow_to_pos__col
   DEC   __local_colrow_to_pos__row
.loop@5_start:
   LDA   __local_colrow_to_pos__row
   BNE   .colrow_to_pos@rot@4
   LDA   __local_colrow_to_pos__col
   RTS

get_mob_data:
   SUBROUTINE

   SEC
   SBC   #$01
   STA   __local_get_mob_data__index
   LSR
   LSR
   LSR
   LSR
   STA   __local_get_mob_data__0
   LDA   __local_get_mob_data__index
   ASL
   ASL
   ASL
   ASL
   CLC
   ADC   LOCATIONS
   STA   __local_get_mob_data__index
   LDA   LOCATIONS+1
   ADC   __local_get_mob_data__0
   TAX
   LDA   __local_get_mob_data__index
   RTS

get_char_record:
   SUBROUTINE

   STA   __local_get_char_record__index
   CMP   #$81
   BCC   .if_end@7
   LDA   #$00
   STA   __local_get_char_record__index
.if_end@7:
   LDA   #<LOCATIONS
   STA   __local_get_char_record__rec
   LDX   #>LOCATIONS
   JMP   .loop@6_start
.get_char_record@rot@5:
   LDA   __local_get_char_record__rec
   CLC
   ADC   #$09
   STA   __local_get_char_record__rec
   BCC   .carry_inc@0
   INX
.carry_inc@0:
   DEC   __local_get_char_record__index
.loop@6_start:
   LDA   __local_get_char_record__index
   BNE   .get_char_record@rot@5
   LDA   __local_get_char_record__rec
   RTS

clamp_char_field:
   SUBROUTINE

   LDY   #$05
   LDA   (MOB_PTR),Y
   AND   #$1F
   STA   __local_clamp_char_field__1
   INY
   LDA   (MOB_PTR),Y
   AND   #$3F
   CMP   __local_clamp_char_field__1
   BCS   .if_end@8
   LDA   (MOB_PTR),Y
   AND   #$C0
   ORA   __local_clamp_char_field__1
   STA   (MOB_PTR),Y
.if_end@8:
   RTS

copy_pc_name_to_buf:
   SUBROUTINE

   STA   __local_copy_pc_name_to_buf__m
   STX   __local_copy_pc_name_to_buf__0
   STA   MOB_PTR
   STX   MOB_PTR+1
   LDY   #$00
   LDA   (__local_copy_pc_name_to_buf__m),Y
   STA   __local_copy_pc_name_to_buf__2
   INY
   LDA   (__local_copy_pc_name_to_buf__m),Y
   STA   __local_copy_pc_name_to_buf__3
   DEY
   LDA   (__local_copy_pc_name_to_buf__2),Y
   STA   __local_copy_pc_name_to_buf__left
   LDA   #$01
   STA   __local_copy_pc_name_to_buf__i
.copy_pc_name_to_buf@rot@6:
   DEC   __local_copy_pc_name_to_buf__left
   BMI   .cond_else@9
   LDY   __local_copy_pc_name_to_buf__i
   LDA   (__local_copy_pc_name_to_buf__2),Y
   STA   __local_copy_pc_name_to_buf__1
   JMP   .cond_end@10
.cond_else@9:
   LDY   #$20
   STY   __local_copy_pc_name_to_buf__1
.cond_end@10:
   LDX   __local_copy_pc_name_to_buf__i
   LDA   __local_copy_pc_name_to_buf__1
   STA   S_ADD_NAME+4,X
   INC   __local_copy_pc_name_to_buf__i
   LDA   __local_copy_pc_name_to_buf__i
   CMP   #$15
   BCC   .copy_pc_name_to_buf@rot@6
   RTS

find_first_pc:
   SUBROUTINE

   LDA   LOCATIONS+2
   JMP   .loop@8_continue
.find_first_pc@rot@7:
   STA   BROWSE_CHAR_INDEX
   JSR   get_mob_data
   STA   __local_find_first_pc__0
   STX   __local_find_first_pc__1
   LDY   #$04
   LDA   (__local_find_first_pc__0),Y
   CMP   #$15
   BCS   .if_end@11
   LDA   __local_find_first_pc__0
   JSR   copy_pc_name_to_buf
   SEC
   RTS
.if_end@11:
   LDY   #$02
   LDA   (__local_find_first_pc__0),Y
.loop@8_continue:
   BNE   .find_first_pc@rot@7
   CLC
   RTS

find_next_pc:
   SUBROUTINE

.loop@9_start:
   LDY   #$02
   LDA   (MOB_PTR),Y
   BNE   .if_end@12
   JMP   find_first_pc
.if_end@12:
   STA   BROWSE_CHAR_INDEX
   JSR   get_mob_data
   STA   MOB_PTR
   STX   MOB_PTR+1
   LDY   #$04
   LDA   (MOB_PTR),Y
   CMP   #$15
   BCS   .loop@9_start
   LDA   MOB_PTR
   JMP   copy_pc_name_to_buf

find_next_pc_wrap:
   SUBROUTINE

   JMP   find_next_pc

find_prev_pc:
   SUBROUTINE

   LDA   BROWSE_CHAR_INDEX
   STA   __local_find_prev_pc__start
   JSR   find_first_pc
   LDA   BROWSE_CHAR_INDEX
.vthread@0:
   STA   __local_find_prev_pc__prev
   JSR   find_next_pc
   LDA   BROWSE_CHAR_INDEX
   CMP   __local_find_prev_pc__start
   BNE   .vthread@0
   LDA   __local_find_prev_pc__prev
   STA   BROWSE_CHAR_INDEX
   JSR   get_mob_data
   JMP   copy_pc_name_to_buf

init_char_select:
   SUBROUTINE

   LDA   #$01
   STA   SCENE_EVENT_RESULT
   JSR   scene_setup
   JSR   find_first_pc
   BCC   .if_end@14
   LDA   #$27
   RTS
.if_end@14:
   LDA   #$02
   RTS

step_prng:
   SUBROUTINE

   LDA   PRNG_STATE
   STA   __local_step_prng__s
   ASL
   ASL
   STA   __local_step_prng__1
   ASL
   ASL
   ASL
   ASL
   CLC
   ADC   __local_step_prng__1
   CLC
   ADC   __local_step_prng__s
   CLC
   ADC   #$53
   STA   PRNG_STATE
   LSR
   STA   PRNG_OUTPUT
   RTS

random_in_range:
   SUBROUTINE

   STA   __local_random_in_range__scaled
   ORA   #$00
   BNE   .if_end@15
   LDA   #$00
   RTS
.if_end@15:
   LDA   #$06
   STA   __local_random_in_range__shift
   LDA   #$40
   STA   __local_random_in_range__mask
   LDA   #$02
   STA   __local_random_in_range__bound
.loop@11_continue:
   LDA   __local_random_in_range__shift
   BEQ   .vthread@1
   LDA   __local_random_in_range__scaled
   SEC
   SBC   __local_random_in_range__bound
   BMI   .loop@11_break
   DEC   __local_random_in_range__shift
   LDA   __local_random_in_range__mask
   LSR
   ORA   #$40
   STA   __local_random_in_range__mask
   ASL   __local_random_in_range__bound
   JMP   .loop@11_continue
.loop@11_break:
   LDA   __local_random_in_range__shift
.vthread@1:
   STA   __local_random_in_range__sample
   JMP   .loop@12_start
.random_in_range@rot@8:
   ASL   __local_random_in_range__scaled
   DEC   __local_random_in_range__sample
.loop@12_start:
   LDA   __local_random_in_range__sample
   BNE   .random_in_range@rot@8
.loop@13_start:
   JSR   step_prng
   AND   __local_random_in_range__mask
   STA   __local_random_in_range__sample
   SEC
   SBC   __local_random_in_range__scaled
   BEQ   .loop@14_start
   BPL   .loop@13_start
   JMP   .loop@14_start
.random_in_range@rot@9:
   LSR   __local_random_in_range__sample
   DEC   __local_random_in_range__shift
.loop@14_start:
   LDA   __local_random_in_range__shift
   BNE   .random_in_range@rot@9
   LDA   __local_random_in_range__sample
   RTS

compute_checksum:
   SUBROUTINE

   STA   __local_compute_checksum__p_0
   STX   __local_compute_checksum__p_1
   LDA   #$00
   STA   __local_compute_checksum__lo
   STA   __local_compute_checksum__hi
.loop@15_start:
   LDY   #$00
   LDA   (__local_compute_checksum__p_0),Y
   STA   __local_compute_checksum__0
   INC   __local_compute_checksum__p_0
   BNE   .inc_done@0
   INC   __local_compute_checksum__p_1
.inc_done@0:
   LDA   __local_compute_checksum__lo
   CLC
   ADC   __local_compute_checksum__0
   STA   __local_compute_checksum__lo
   LDA   __local_compute_checksum__p_1
   CMP   __zpabi_compute_checksum__end_1
   BNE   .if_end@20
   LDA   __local_compute_checksum__p_0
   CMP   __zpabi_compute_checksum__end_0
   BEQ   .loop@15_break
.if_end@20:
   LDY   #$00
   LDA   (__local_compute_checksum__p_0),Y
   STA   __local_compute_checksum__0
   INC   __local_compute_checksum__p_0
   BNE   .inc_done@1
   INC   __local_compute_checksum__p_1
.inc_done@1:
   LDA   __local_compute_checksum__hi
   CLC
   ADC   __local_compute_checksum__0
   STA   __local_compute_checksum__hi
   LDA   __local_compute_checksum__lo
   EOR   __local_compute_checksum__hi
   STA   __local_compute_checksum__lo
   LDA   __local_compute_checksum__p_1
   CMP   __zpabi_compute_checksum__end_1
   BNE   .loop@15_start
   LDA   __local_compute_checksum__p_0
   CMP   __zpabi_compute_checksum__end_0
   BNE   .loop@15_start
.loop@15_break:
   LDA   __local_compute_checksum__lo
   LDX   __local_compute_checksum__hi
   RTS

verify_integrity:
   SUBROUTINE

   LDA   #<(DISK_IO_CODE+323)
   STA   __zpabi_compute_checksum__end_0
   LDA   #>(DISK_IO_CODE+323)
   STA   __zpabi_compute_checksum__end_1
   LDX   #>DISK_IO_CODE
   LDA   #<DISK_IO_CODE
   JSR   compute_checksum
   CPX   #$7A
   BNE   .jcmp_skip@0
   CMP   #$AE
   BEQ   .if_end@22
.jcmp_skip@0:
   JSR   integrity_fail
.if_end@22:
   RTS

verify_disk_io:
   SUBROUTINE

   RTS

cold_restart:
   SUBROUTINE

   LDA   #$08
   STA   __local_cold_restart__page
.loop@16_start:
   STA   __local_cold_restart__1
   LDA   #$00
   STA   __local_cold_restart__0
   LDY   #$00
.loop@17_start:
   LDA   __local_cold_restart__page
   STA   (__local_cold_restart__0),Y
   DEY
   BNE   .loop@17_start
   INC   __local_cold_restart__page
   LDA   __local_cold_restart__page
   CMP   #$A5
   BNE   .loop@16_start
   LDX   #$6F
   LDA   #$A5
.loop@18_start:
   STA   $A500,X
   DEX
   BNE   .loop@18_start
   JMP   DISK_BOOT_ROM

ANIM_INDEX:
   DC.B  $02

ANIM_WAIT_TABLE:
   DC.B  $00, $32, $23, $17, $0C, $05

ANIM_FRAME_TABLE:
   DC.B  $00, $02, $02, $04, $09, $13

__zp_init:
   SUBROUTINE

   LDA   #$03
   STA   ANIM_FRAME_CTR
   RTS
