NULL_PAGE_LOW	EQU	$00
TEXT_STREAM_PTR	EQU	$06
RWTS_IOB_PTR	EQU	$08
NULL_PAGE_HIGH	EQU	$0A
HARGS	EQU	$10
SSP	EQU	$1A
FP	EQU	$1C
slot_number	EQU	$1E
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
option@ret	EQU	$62
under_level	EQU	$64
joy_zone_x	EQU	$65
joy_zone_y	EQU	$66
safe_to_rest	EQU	$67
__zpabi_modify_current_stats__add	EQU	$80
__zpabi_num_to_decimal__out_0	EQU	$80
__zpabi_update_event_list__value_0	EQU	$80
__zpabi_validate_input__o_0	EQU	$80
__local_write_disk_signature__tries	EQU	$81
__zpabi_num_to_decimal__out_1	EQU	$81
__zpabi_update_event_list__value_1	EQU	$81
__zpabi_validate_input__o_1	EQU	$81
__local_verify_disk_signature__0	EQU	$83
__local_transfer_game_data__left	EQU	$84
__local_verify_disk_signature__tries	EQU	$84
__local_verify_game_disk__tries	EQU	$84
__local_transfer_game_data__tries	EQU	$85
__local_transfer_game_data__command	EQU	$86
__local_input_loop__m	EQU	$87
__local_setup_slot_iob__slot	EQU	$87
__local_input_loop__0	EQU	$88
__local_draw_option__0	EQU	$8A
__local_setup_input__m	EQU	$8A
__local_draw_option__o	EQU	$8B
__local_read_keyboard__1	EQU	$8B
__local_setup_input__0	EQU	$8B
__local_validate_input__o_0	EQU	$8B
__local_draw_option__1	EQU	$8C
__local_validate_input__o_1	EQU	$8C
__local_format_slot_number__n	EQU	$8D
__local_render_input_buffer__i	EQU	$8D
__local_set_cursor_pos__pos	EQU	$8D
__local_validate_input__1	EQU	$8D
__local_check_input__dir	EQU	$8E
__local_validate_input__2	EQU	$8E
__local_check_combat_eligibility__1	EQU	$8F
__local_check_input__2	EQU	$8F
__local_validate_input__dir	EQU	$8F
__local_copy_record_data__i	EQU	$90
__local_copy_record_data__1	EQU	$91
__local_option__n	EQU	$91
__local_read_paddle__0	EQU	$91
__local_copy_record_data__2	EQU	$92
__local_option__0	EQU	$92
__local_copy_record_data__o	EQU	$93
__local_joy_to_zone__old_zone	EQU	$93
__local_option__1	EQU	$93
__local_copy_record_data__3	EQU	$94
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
; def advance_sector params= locals=__local_advance_sector__0 indirect=false in_cycle=false
; def call_input_init params=__zpabi_call_input_init__init locals=__local_call_input_init__init indirect=false in_cycle=false param_regs=A
; def cancel_scenario params= locals= indirect=false in_cycle=false
; def char_select_here params= locals=__local_char_select_here__0 indirect=false in_cycle=false
; def char_select_home params= locals=__local_char_select_home__0 indirect=false in_cycle=false
; def char_select_init params= locals=__local_char_select_init__0 indirect=false in_cycle=false
; def char_select_next params= locals= indirect=false in_cycle=false
; def char_select_patch params=__zpabi_char_select_patch__home_enabled,__zpabi_char_select_patch__here_enabled locals=__local_char_select_patch__here_enabled indirect=false in_cycle=false param_regs=A,X
; def char_select_prev params= locals= indirect=false in_cycle=false
; def check_combat_eligibility params= locals=__local_check_combat_eligibility__0,__local_check_combat_eligibility__1 indirect=false in_cycle=false
; def check_input params= locals=__local_check_input__0,__local_check_input__1,__local_check_input__dir,__local_check_input__2 indirect=false in_cycle=false
; def check_joystick params= locals=__local_check_joystick__0 indirect=false in_cycle=false
; def clear_input_buffer params= locals= indirect=false in_cycle=false
; def configure_input params= locals=__local_configure_input__0 indirect=false in_cycle=false
; def continue_game params= locals=__local_continue_game__0 indirect=false in_cycle=false
; def copy_record_data params=__zpabi_copy_record_data__o_0,__zpabi_copy_record_data__o_1 locals=__local_copy_record_data__0,__local_copy_record_data__i,__local_copy_record_data__1,__local_copy_record_data__2,__local_copy_record_data__o,__local_copy_record_data__3 indirect=false in_cycle=false param_regs=A,X
; def display_insert_msg params= locals= indirect=false in_cycle=false
; def draw_input_options params= locals=__local_draw_input_options__0,__local_draw_input_options__1,__local_draw_input_options__2 indirect=false in_cycle=false
; def draw_option params=__zpabi_draw_option__o_0,__zpabi_draw_option__o_1 locals=__local_draw_option__0,__local_draw_option__o,__local_draw_option__1 indirect=false in_cycle=false param_regs=A,X
; def ensure_game_disk params= locals=__local_ensure_game_disk__0 indirect=false in_cycle=false
; def enter_char_select params= locals=__local_enter_char_select__0 indirect=false in_cycle=false
; def enter_save_game_slot params= locals=__local_enter_save_game_slot__0 indirect=false in_cycle=false
; def enter_scenario_disk params= locals=__local_enter_scenario_disk__0 indirect=false in_cycle=false
; def format_disk_track params= locals=__local_format_disk_track__0 indirect=false in_cycle=false carry=true
; def format_scenario_disk params= locals=__local_format_scenario_disk__0 indirect=false in_cycle=false
; def format_slot_number params= locals=__local_format_slot_number__n indirect=false in_cycle=false
; def get_input_table_ptr params= locals= indirect=false in_cycle=false
; def init_world params= locals=__local_init_world__0 indirect=false in_cycle=false
; def input_loop params=__zpabi_input_loop__m_0,__zpabi_input_loop__m_1 locals=__local_input_loop__m,__local_input_loop__0,__local_input_loop__1 indirect=false in_cycle=false param_regs=A,X
; def joy_to_zone params=__zpabi_joy_to_zone__old_zone,__zpabi_joy_to_zone__reading locals=__local_joy_to_zone__reading,__local_joy_to_zone__0,__local_joy_to_zone__old_zone indirect=false in_cycle=false param_regs=A,X
; def menu_attack_target params= locals=__local_menu_attack_target__0 indirect=false in_cycle=false
; def menu_check_attack params= locals=__local_menu_check_attack__0 indirect=false in_cycle=false
; def menu_drop_dec params= locals= indirect=false in_cycle=false
; def menu_drop_gold params= locals=__local_menu_drop_gold__0 indirect=false in_cycle=false
; def menu_drop_inc params= locals= indirect=false in_cycle=false
; def menu_game_settings params= locals=__local_menu_game_settings__0 indirect=false in_cycle=false
; def menu_game_settings_enter params= locals=__local_menu_game_settings_enter__0 indirect=false in_cycle=false
; def menu_other_options params= locals=__local_menu_other_options__0 indirect=false in_cycle=false
; def menu_see_your_lot params= locals= indirect=false in_cycle=false
; def menu_settings_init params= locals= indirect=false in_cycle=false
; def option params=__zpabi_option__n locals=__local_option__n,__local_option__0,__local_option__1 indirect=false in_cycle=false param_regs=A
; def prompt_insert_disk params= locals= indirect=false in_cycle=false
; def read_game_data params= locals=__local_read_game_data__0 indirect=false in_cycle=false
; def read_keyboard params= locals=__local_read_keyboard__0,__local_read_keyboard__1 indirect=false in_cycle=false
; def read_paddle params=__zpabi_read_paddle__n locals=__local_read_paddle__0 indirect=false in_cycle=false param_regs=X
; def render_input_buffer params= locals=__local_render_input_buffer__0,__local_render_input_buffer__i indirect=false in_cycle=false
; def retire_character params= locals= indirect=false in_cycle=false
; def save_game params= locals=__local_save_game__0 indirect=false in_cycle=false
; def scenario_done params= locals= indirect=false in_cycle=false
; def set_cursor_pos params=__zpabi_set_cursor_pos__pos locals=__local_set_cursor_pos__pos,__local_set_cursor_pos__row indirect=false in_cycle=false param_regs=A
; def settings_danger_down params= locals= indirect=false in_cycle=false
; def settings_danger_up params= locals= indirect=false in_cycle=false
; def settings_speed_down params= locals=__local_settings_speed_down__0 indirect=false in_cycle=false
; def settings_speed_up params= locals= indirect=false in_cycle=false
; def setup_data_iob params= locals= indirect=false in_cycle=false
; def setup_input params=__zpabi_setup_input__m_0,__zpabi_setup_input__m_1 locals=__local_setup_input__m,__local_setup_input__0 indirect=false in_cycle=false param_regs=A,X
; def setup_slot_iob params= locals=__local_setup_slot_iob__slot,__local_setup_slot_iob__0 indirect=false in_cycle=false
; def slot_dec params= locals=__local_slot_dec__0 indirect=false in_cycle=false
; def slot_dec_enter params= locals= indirect=false in_cycle=false
; def slot_dec_reenter params= locals= indirect=false in_cycle=false
; def slot_inc params= locals= indirect=false in_cycle=false
; def slot_inc_enter params= locals= indirect=false in_cycle=false
; def slot_inc_reenter params= locals= indirect=false in_cycle=false
; def slot_update params= locals= indirect=false in_cycle=false
; def start_game_input params= locals=__local_start_game_input__0 indirect=false in_cycle=false
; def store_selection_to_record params= locals= indirect=false in_cycle=false
; def teardown_input params= locals=__local_teardown_input__0 indirect=false in_cycle=false
; def transfer_game_data params=__zpabi_transfer_game_data__command locals=__local_transfer_game_data__0,__local_transfer_game_data__left,__local_transfer_game_data__tries,__local_transfer_game_data__command indirect=false in_cycle=false param_regs=A carry=true
; def use_scenario_disk params= locals=__local_use_scenario_disk__0 indirect=false in_cycle=false
; def validate_input params=__zpabi_validate_input__dir,__zpabi_validate_input__o_0,__zpabi_validate_input__o_1 locals=__local_validate_input__0,__local_validate_input__o_0,__local_validate_input__o_1,__local_validate_input__1,__local_validate_input__2,__local_validate_input__dir,__local_validate_input__3 indirect=false in_cycle=false param_regs=A,,
; def verify_disk_signature params= locals=__local_verify_disk_signature__0,__local_verify_disk_signature__tries,__local_verify_disk_signature__1 indirect=false in_cycle=false carry=true
; def verify_game_disk params= locals=__local_verify_game_disk__0,__local_verify_game_disk__tries indirect=false in_cycle=false carry=true
; def wait_keypress params= locals=__local_wait_keypress__0 indirect=false in_cycle=false
; def write_disk_signature params= locals=__local_write_disk_signature__0,__local_write_disk_signature__tries indirect=false in_cycle=false carry=true
; def write_game_data params= locals= indirect=false in_cycle=false
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
; ext enter_combat_state params=__zpabi_enter_combat_state__pos param_regs=A
; ext enter_location params=
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
; ext game_init params=
; ext get_char_record params=__zpabi_get_char_record__index param_regs=A
; ext get_entity_fontchar params=__zpabi_get_entity_fontchar__e_0,__zpabi_get_entity_fontchar__e_1 param_regs=A,X
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
; ext integrity_fail params=
; ext load_char_name_ptr params=
; ext load_char_to_fa params=__zpabi_load_char_to_fa__index param_regs=A
; ext main params=
; ext manhattan_distance params=__zpabi_manhattan_distance__pos,__zpabi_manhattan_distance__from param_regs=A,X
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
; ext render_font_char params=
; ext render_font_char_direct params=__zpabi_render_font_char_direct__charset_sel param_regs=A
; ext reorder_char params=__zpabi_reorder_char__m_0,__zpabi_reorder_char__m_1 param_regs=A,X
; ext reorder_current_char params=
; ext reset_turn_pos params=
; ext resolve_attack params=
; ext run_chain params=__zpabi_run_chain__state param_regs=A
; ext rwts_dispatch params=__zpabi_rwts_dispatch__command param_regs=A carry=true
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
; ext settings_display_init params=
; ext setup_char_sprite params=
; ext setup_scene_display params=
; ext setup_text_pos params=__zpabi_setup_text_pos__name_0,__zpabi_setup_text_pos__name_1 param_regs=A,X
; ext setup_text_pos_col20 params=__zpabi_setup_text_pos_col20__name_0,__zpabi_setup_text_pos_col20__name_1 param_regs=A,X
; ext shop_buy_handler params=
; ext show_locked_msg params=
; ext start_combat params=
; ext step_prng params=
; ext text_renderer params=
; ext text_renderer_no_pause params=
; ext tick_anim_counter params=
; ext timed_wait params=
; ext toggle_sound params=
; ext trigger_scene_event params=__zpabi_trigger_scene_event__ev_0,__zpabi_trigger_scene_event__ev_1 param_regs=A,X
; ext tune_player params=__zpabi_tune_player__tune,__zpabi_tune_player__strength param_regs=A,X
; ext update_event_list params=__zpabi_update_event_list__pos,__zpabi_update_event_list__value_0,__zpabi_update_event_list__value_1 param_regs=A,,
; ext verify_disk_io params=
; ext verify_integrity params=
; call call_input_init -> configure_input
; call call_input_init -> format_slot_number
; call call_input_init -> init_attack_display
; call call_input_init -> init_drop_display
; call call_input_init -> menu_settings_init
; call cancel_scenario -> ensure_game_disk
; call char_select_here -> char_select_patch
; call char_select_home -> char_select_here
; call char_select_home -> char_select_patch
; call char_select_init -> char_select_home
; call char_select_init -> display_message
; call char_select_init -> find_first_pc
; call char_select_init -> set_cursor_row21
; call char_select_next -> find_next_pc_wrap
; call char_select_next -> store_selection_to_record
; call char_select_patch -> enter_char_select
; call char_select_prev -> find_prev_pc
; call char_select_prev -> store_selection_to_record
; call check_combat_eligibility -> check_adjacent_threats
; call check_combat_eligibility -> check_encounter
; call check_input -> check_joystick
; call check_joystick -> WAIT
; call check_joystick -> joy_to_zone
; call check_joystick -> read_paddle
; call configure_input -> check_combat_eligibility
; call continue_game -> display_message
; call continue_game -> ensure_game_disk
; call continue_game -> prompt_insert_disk
; call continue_game -> read_game_data
; call continue_game -> set_cursor_row21
; call continue_game -> verify_disk_signature
; call continue_game -> wait_keypress
; call display_insert_msg -> display_message
; call display_insert_msg -> set_cursor_row21
; call draw_input_options -> copy_record_data
; call draw_input_options -> option
; call draw_option -> print_ctrl_ab
; call draw_option -> print_ctrl_n
; call draw_option -> print_string
; call draw_option -> set_cursor_pos
; call ensure_game_disk -> prompt_insert_disk
; call ensure_game_disk -> verify_game_disk
; call enter_char_select -> input_loop
; call enter_save_game_slot -> input_loop
; call enter_scenario_disk -> input_loop
; call format_disk_track -> rwts_dispatch
; call format_scenario_disk -> display_insert_msg
; call format_scenario_disk -> display_message
; call format_scenario_disk -> enter_scenario_disk
; call format_scenario_disk -> format_disk_track
; call format_scenario_disk -> scenario_done
; call format_scenario_disk -> set_cursor_row21
; call format_scenario_disk -> verify_disk_signature
; call format_scenario_disk -> verify_game_disk
; call format_scenario_disk -> write_disk_signature
; call format_scenario_disk -> write_game_data
; call init_world -> input_loop
; call input_loop -> draw_option
; call input_loop -> option
; call input_loop -> read_keyboard
; call input_loop -> set_inverse_video
; call input_loop -> setup_input
; call input_loop -> tune_player
; call input_loop -> validate_input
; call menu_attack_target -> input_loop
; call menu_check_attack -> check_attack_eligible
; call menu_check_attack -> input_loop
; call menu_drop_dec -> store_selection_to_record
; call menu_drop_gold -> input_loop
; call menu_drop_inc -> store_selection_to_record
; call menu_game_settings -> menu_game_settings_enter
; call menu_game_settings_enter -> input_loop
; call menu_other_options -> input_loop
; call menu_see_your_lot -> display_char_stats
; call menu_settings_init -> settings_display_init
; call prompt_insert_disk -> display_message
; call prompt_insert_disk -> set_cursor_row21
; call prompt_insert_disk -> wait_keypress
; call read_game_data -> display_message
; call read_game_data -> set_cursor_row21
; call read_game_data -> transfer_game_data
; call read_keyboard -> anim_check_frame
; call read_keyboard -> anim_tick_and_wait
; call read_keyboard -> check_input
; call read_keyboard -> idle_anim_handler
; call read_keyboard -> step_prng
; call read_keyboard -> tick_anim_counter
; call read_keyboard -> verify_integrity
; call render_input_buffer -> hrcg_cout
; call render_input_buffer -> print_ctrl_ab
; call retire_character -> reorder_current_char
; call retire_character -> scene_loop
; call save_game -> ensure_game_disk
; call save_game -> enter_scenario_disk
; call save_game -> scene_loop
; call save_game -> write_game_data
; call scenario_done -> ensure_game_disk
; call scenario_done -> prompt_insert_disk
; call settings_danger_down -> dec_threshold_wrap
; call settings_danger_down -> store_selection_to_record
; call settings_danger_up -> inc_threshold_wrap
; call settings_danger_up -> store_selection_to_record
; call settings_speed_down -> store_selection_to_record
; call settings_speed_up -> store_selection_to_record
; call setup_input -> call_input_init
; call setup_input -> clear_input_buffer
; call setup_input -> draw_input_options
; call setup_input -> render_input_buffer
; call slot_dec -> slot_update
; call slot_dec_enter -> slot_dec
; call slot_dec_reenter -> slot_dec
; call slot_inc -> slot_update
; call slot_inc_enter -> slot_inc
; call slot_inc_reenter -> slot_inc
; call slot_update -> format_slot_number
; call slot_update -> store_selection_to_record
; call start_game_input -> input_loop
; call teardown_input -> get_input_table_ptr
; call teardown_input -> input_loop
; call transfer_game_data -> advance_sector
; call transfer_game_data -> rwts_dispatch
; call transfer_game_data -> setup_slot_iob
; call use_scenario_disk -> display_message
; call use_scenario_disk -> scenario_done
; call use_scenario_disk -> set_cursor_row21
; call use_scenario_disk -> verify_disk_signature
; call use_scenario_disk -> write_game_data
; call validate_input -> option
; call verify_disk_signature -> disk_read
; call verify_disk_signature -> setup_data_iob
; call verify_game_disk -> disk_read
; call verify_game_disk -> setup_data_iob
; call wait_keypress -> display_message
; call write_disk_signature -> disk_write
; call write_disk_signature -> setup_data_iob
; call write_game_data -> transfer_game_data
; share member=validate_input:__zpabi_validate_input__o_0,__zpabi_validate_input__o_1 holder=input_loop live=input_loop:draw_option,read_keyboard,set_inverse_video,validate_input forward=input_loop>validate_input writer=input_loop
; item option section=.text local=true zero=false
; item set_cursor_pos section=.text local=true zero=false
; item draw_option section=.text local=true zero=false
; item clear_input_buffer section=.text local=true zero=false
; item copy_record_data section=.text local=true zero=false
; item draw_input_options section=.text local=true zero=false
; item render_input_buffer section=.text local=true zero=false
; item setup_input section=.text local=false zero=false
; item validate_input section=.text local=true zero=false
; item input_loop section=.text local=false zero=false
; item store_selection_to_record section=.text local=true zero=false
; item read_paddle section=.text local=true zero=false
; item joy_to_zone section=.text local=true zero=false
; item check_joystick section=.text local=true zero=false
; item check_input section=.text local=true zero=false
; item read_keyboard section=.text local=false zero=false
; item check_combat_eligibility section=.text local=true zero=false
; item configure_input section=.text local=false zero=false
; item get_input_table_ptr section=.text local=false zero=false
; item teardown_input section=.text local=false zero=false
; item menu_see_your_lot section=.text local=false zero=false
; item menu_check_attack section=.text local=false zero=false
; item menu_other_options section=.text local=false zero=false
; item retire_character section=.text local=false zero=false
; item menu_game_settings section=.text local=false zero=false
; item menu_game_settings_enter section=.text local=false zero=false
; item menu_settings_init section=.text local=true zero=false
; item settings_speed_up section=.text local=false zero=false
; item settings_speed_down section=.text local=false zero=false
; item settings_danger_up section=.text local=false zero=false
; item settings_danger_down section=.text local=false zero=false
; item menu_attack_target section=.text local=false zero=false
; item menu_drop_gold section=.text local=false zero=false
; item menu_drop_inc section=.text local=false zero=false
; item menu_drop_dec section=.text local=false zero=false
; item format_slot_number section=.text local=true zero=false
; item slot_update section=.text local=true zero=false
; item slot_inc section=.text local=true zero=false
; item slot_dec section=.text local=true zero=false
; item init_world section=.text local=false zero=false
; item slot_inc_enter section=.text local=false zero=false
; item slot_dec_enter section=.text local=false zero=false
; item enter_save_game_slot section=.text local=false zero=false
; item slot_dec_reenter section=.text local=false zero=false
; item slot_inc_reenter section=.text local=false zero=false
; item call_input_init section=.text local=true zero=false
; item enter_char_select section=.text local=false zero=false
; item char_select_init section=.text local=false zero=false
; item char_select_next section=.text local=false zero=false
; item char_select_prev section=.text local=false zero=false
; item char_select_patch section=.text local=true zero=false
; item char_select_here section=.text local=false zero=false
; item char_select_home section=.text local=false zero=false
; item start_game_input section=.text local=false zero=false
; item setup_data_iob section=.text local=true zero=false
; item verify_game_disk section=.text local=true zero=false
; item wait_keypress section=.text local=true zero=false
; item prompt_insert_disk section=.text local=true zero=false
; item ensure_game_disk section=.text local=true zero=false
; item format_disk_track section=.text local=true zero=false
; item write_disk_signature section=.text local=true zero=false
; item verify_disk_signature section=.text local=true zero=false
; item setup_slot_iob section=.text local=true zero=false
; item advance_sector section=.text local=true zero=false
; item transfer_game_data section=.text local=true zero=false
; item write_game_data section=.text local=true zero=false
; item read_game_data section=.text local=true zero=false
; item continue_game section=.text local=false zero=false
; item save_game section=.text local=false zero=false
; item enter_scenario_disk section=.text local=false zero=false
; item scenario_done section=.text local=true zero=false
; item cancel_scenario section=.text local=false zero=false
; item display_insert_msg section=.text local=true zero=false
; item format_scenario_disk section=.text local=false zero=false
; item use_scenario_disk section=.text local=false zero=false
; item GAME_INPUT_TABLE section=.data local=false zero=false
; item ATTACK_DIR_TABLE section=.data local=false zero=false
; item OTHER_OPTIONS_TABLE section=.data local=false zero=false
; item GAME_SETTINGS_TABLE section=.data local=false zero=false
; item ATTACK_TARGET_TABLE section=.data local=false zero=false
; item DROP_GOLD_TABLE section=.data local=false zero=false
; item SAVE_LOAD_TABLE section=.data local=false zero=false
; item SAVE_GAME_SLOT_TABLE section=.data local=false zero=false
; item SCENARIO_DISK_TABLE section=.data local=false zero=false
; item CHAR_SELECT_TABLE section=.data local=false zero=false
; item ALT_INPUT_TABLE section=.data local=false zero=false
; item S_SOUND_IS_ON section=.data local=false zero=false
; item S_DANGER section=.data local=false zero=false
; item S_ADD_NAME section=.data local=false zero=false
; item S_CHEST_MESSAGE section=.data local=false zero=false
; item S_ATTACK_TARGET section=.data local=false zero=false
; item S_MISSES section=.data local=false zero=false
; item S_DUCATS_TO_DROP section=.data local=false zero=false
; item S_PAY_FOR section=.data local=false zero=false
; item GAME_SIG_TRACK section=.rodata local=false zero=false
; item GAME_SIG_SECTOR section=.rodata local=false zero=true
; item S_DROP_ARMOR section=.data local=true zero=false
; item S_RETIRE section=.data local=true zero=false
; item S_ADD_SOMEONE section=.data local=true zero=false
; item S_DROP_GOLD section=.data local=true zero=false
; item S_SAVE_GAME section=.data local=true zero=false
; item S_TO_THIS_ROOM section=.data local=true zero=false
; item S_TO_ALI_BABAS_HOME section=.data local=true zero=false
; item S_INCREASE section=.data local=true zero=false
; item S_DECREASE section=.data local=true zero=false
; item S_SEE_YOUR_LOT section=.data local=true zero=false
; item S_REST section=.data local=true zero=false
; item S_DEFEND section=.data local=true zero=false
; item S_STOP section=.data local=true zero=false
; item S_CONTINUE_GAME_NUMBER section=.data local=true zero=false
; item S_BEGIN_NEW_GAME section=.data local=true zero=false
; item S_USE_PREVIOUS_DISK section=.data local=true zero=false
; item S_FORMAT_NEW_DISK section=.data local=true zero=false
; item S_CANCEL_REQUEST section=.data local=true zero=false
; item S_ATTACK_ABOVE section=.data local=true zero=false
; item S_ATTACK_LEFT section=.data local=true zero=false
; item S_ATTACK_RIGHT section=.data local=true zero=false
; item S_ATTACK_BELOW section=.data local=true zero=false
; item S_ATTACK_SOMEONE_ELSE section=.data local=true zero=false
; item S_CANCEL_ATTACK section=.data local=true zero=false
; item S_KEEP_ALL_GOLD section=.data local=true zero=false
; item S_BUY_SOMETHING_ELSE section=.data local=true zero=false
; item S_NO_SALE section=.data local=true zero=false
; item S_ATTACK section=.data local=true zero=false
; item S_OTHER_OPTIONS section=.data local=true zero=false
; item S_MOVE section=.data local=true zero=false
; item S_GAME_SPEED section=.data local=true zero=false
; item S_SAVE_IN_SLOT section=.data local=true zero=false
; item GAME_INPUT_OPTIONS section=.data local=true zero=false
; item ATTACK_DIR_OPTIONS section=.data local=true zero=false
; item OTHER_OPTIONS_OPTIONS section=.data local=true zero=false
; item GAME_SETTINGS_OPTIONS section=.data local=true zero=false
; item ATTACK_TARGET_OPTIONS section=.data local=true zero=false
; item DROP_GOLD_OPTIONS section=.data local=true zero=false
; item SAVE_LOAD_OPTIONS section=.data local=true zero=false
; item SAVE_GAME_SLOT_OPTIONS section=.data local=true zero=false
; item SCENARIO_DISK_OPTIONS section=.data local=true zero=false
; item CHAR_SELECT_OPTIONS section=.data local=true zero=false
; item ALT_INPUT_OPTIONS section=.data local=true zero=false
; item DIR_KEYS section=.rodata local=true zero=false
; item ZONE_THRESH_PLUS section=.rodata local=true zero=false
; item ZONE_THRESH_MINUS section=.rodata local=true zero=false
; item SAVE_TRACK_BASE section=.rodata local=true zero=false
; item DISK_SIGNATURE section=.rodata local=true zero=false
; item scenario_init_flag section=.bss local=true zero=true
; item __zp_init section=.text local=false zero=false
; zp NULL_PAGE_LOW size=6 at=$00 def=false init=
; zp TEXT_STREAM_PTR size=2 at=$06 def=false init=
; zp RWTS_IOB_PTR size=2 at=$08 def=false init=
; zp NULL_PAGE_HIGH size=6 at=$0A def=false init=
; zp slot_number size=1 at=auto def=true init=$01 weight=16 local=true
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
; zp option@ret size=2 at=auto def=true init= local=true
; zp under_level size=1 at=auto def=true init=$00 weight=6 local=true
; zp joy_zone_x size=1 at=auto def=true init=$01 weight=5 local=true
; zp joy_zone_y size=1 at=auto def=true init=$01 weight=4 local=true
; zp safe_to_rest size=1 at=auto def=true init=$00 weight=3 local=true
; zp MENU_DESC_PTR size=2 at=$C0 def=false init=
; zp MENU_OPTION_PTR size=2 at=$C2 def=false init=
; zp TUNE_PC size=2 at=$E4 def=false init=
; zp MOB_PTR size=2 at=$F4 def=false init=
; zp ENTITY_PTR size=2 at=$F8 def=false init=
; zp CHAR_PTR size=2 at=$FA def=false init=
; runtime hargs_size=10 hargs_need=0
; @zp-link-meta-end

option:
   SUBROUTINE

   SEC
   SBC   #$01
   STA   __local_option__0
   ASL
   ASL
   ASL
   CLC
   ADC   __local_option__0
   STA   __local_option__1
   LDY   #$03
   LDA   (MENU_DESC_PTR),Y
   STA   __local_option__0
   INY
   LDA   (MENU_DESC_PTR),Y
   STA   __local_option__n
   LDA   __local_option__0
   CLC
   ADC   __local_option__1
   STA   option@ret
   LDA   __local_option__n
   ADC   #$00
   STA   option@ret+1
   RTS

set_cursor_pos:
   SUBROUTINE

   STA   __local_set_cursor_pos__pos
   CMP   #$28
   BCC   .set_cursor_pos@asm_ssa_split@0
   SBC   #$28
   STA   __local_set_cursor_pos__pos
   CMP   #$28
   BCC   .set_cursor_pos@asm_ssa_split@1
   SBC   #$28
   STA   __local_set_cursor_pos__pos
   LDA   #$17
.if_end@1:
.if_end@0:
   STA   TEXT_ROW
   LDA   __local_set_cursor_pos__pos
   STA   TEXT_COL
   RTS
.set_cursor_pos@asm_ssa_split@0:
   LDA   #$15
   BNE   .if_end@0
.set_cursor_pos@asm_ssa_split@1:
   LDA   #$16
   BNE   .if_end@1

draw_option:
   SUBROUTINE

   STA   __local_draw_option__o
   STX   __local_draw_option__1
   LDY   #$01
   LDA   (__local_draw_option__o),Y
   JSR   set_cursor_pos
   JSR   print_ctrl_ab
   LDY   #$06
   LDA   (__local_draw_option__o),Y
   STA   __local_draw_option__0
   INY
   LDA   (__local_draw_option__o),Y
   TAX
   LDA   __local_draw_option__0
   JSR   print_string
   JMP   print_ctrl_n

clear_input_buffer:
   SUBROUTINE

   LDX   #$78
   BNE   .loop@0_start
.clear_input_buffer@rot@0:
   LDA   #$A0
   STA   INPUT_BUFFER,X
.loop@0_start:
   DEX
   CPX   #$FF
   BNE   .clear_input_buffer@rot@0
   RTS

copy_record_data:
   SUBROUTINE

   STA   __local_copy_record_data__o
   STX   __local_copy_record_data__3
   LDY   #$06
   LDA   (__local_copy_record_data__o),Y
   STA   __local_copy_record_data__1
   INY
   LDA   (__local_copy_record_data__o),Y
   STA   __local_copy_record_data__2
   LDY   #$00
   LDA   (__local_copy_record_data__1),Y
   STA   __local_copy_record_data__i
   INY
   LDA   (__local_copy_record_data__o),Y
   CLC
   ADC   __local_copy_record_data__i
   TAX
   DEX
.loop@1_start:
   LDY   __local_copy_record_data__i
   LDA   (__local_copy_record_data__1),Y
   STA   INPUT_BUFFER,X
   DEX
   DEC   __local_copy_record_data__i
   BNE   .loop@1_start
   RTS

draw_input_options:
   SUBROUTINE

   LDY   #$00
   LDA   (MENU_DESC_PTR),Y
   STA   INPUT_OPTION_COUNT
.loop@2_start:
   JSR   option
   LDY   #$00
   LDA   (option@ret),Y
   BEQ   .if_end@2
   LDX   option@ret+1
   LDA   option@ret
   JSR   copy_record_data
.if_end@2:
   DEC   INPUT_OPTION_COUNT
   LDA   INPUT_OPTION_COUNT
   BNE   .loop@2_start
   RTS

render_input_buffer:
   SUBROUTINE

   LDA   #$15
   STA   TEXT_ROW
   LDA   #$00
   STA   TEXT_COL
   JSR   print_ctrl_ab
   LDX   #$00
.render_input_buffer@rot@1:
   LDA   INPUT_BUFFER,X
   STX   __local_render_input_buffer__i
   JSR   hrcg_cout
   LDX   __local_render_input_buffer__i
   INX
   CPX   #$78
   BCC   .render_input_buffer@rot@1
   RTS

setup_input:
   SUBROUTINE

   STA   __local_setup_input__m
   STX   __local_setup_input__0
   STA   MENU_DESC_PTR
   STX   MENU_DESC_PTR+1
   LDY   #$02
   LDA   (__local_setup_input__m),Y
   JSR   call_input_init
   JSR   clear_input_buffer
   JSR   draw_input_options
   JMP   render_input_buffer

validate_input:
   SUBROUTINE

   STA   __local_validate_input__dir
   LDA   __zpabi_validate_input__o_0
   STA   __local_validate_input__o_0
   LDA   __zpabi_validate_input__o_1
   STA   __local_validate_input__o_1
.loop@4_start:
   LDA   __local_validate_input__dir
   SEC
   SBC   #$01
   CLC
   ADC   __local_validate_input__o_0
   STA   __local_validate_input__1
   LDA   #$00
   ADC   __local_validate_input__o_1
   STA   __local_validate_input__2
   LDY   #$02
   LDA   (__local_validate_input__1),Y
   STA   __local_validate_input__1
   BEQ   .validate_input@scfold@0
   CMP   CURRENT_SELECTION
   BNE   .if_end@5
.validate_input@scfold@0:
   RTS
.if_end@5:
   JSR   option
   LDA   option@ret
   STA   __local_validate_input__o_0
   LDA   option@ret+1
   STA   __local_validate_input__o_1
   LDY   #$00
   LDA   (option@ret),Y
   BEQ   .loop@4_start
   LDA   __local_validate_input__1
   RTS

input_loop:
   SUBROUTINE

   STA   __local_input_loop__m
   STX   __local_input_loop__0
   JSR   setup_input
   LDY   #$01
   LDA   (__local_input_loop__m),Y
   STA   CURRENT_SELECTION
.loop@5_start:
   LDA   CURRENT_SELECTION
   JSR   option
   LDA   option@ret
   STA   __zpabi_validate_input__o_0
   LDA   option@ret+1
   STA   __zpabi_validate_input__o_1
   JSR   set_inverse_video
   LDX   __zpabi_validate_input__o_1
   LDA   __zpabi_validate_input__o_0
   JSR   draw_option
   JSR   read_keyboard
   ORA   #$00
   BNE   .if_end@7
   LDY   #$08
   LDA   (__zpabi_validate_input__o_0),Y
   RTS
.if_end@7:
   JSR   validate_input
   CMP   CURRENT_SELECTION
   BEQ   .input_loop@scfold@0
   ORA   #$00
   BNE   .if_else@11
.input_loop@scfold@0:
   LDX   #$00
   LDA   #$09
   JSR   tune_player
   JMP   .loop@5_start
.if_else@11:
   STA   CURRENT_SELECTION
   LDX   __zpabi_validate_input__o_1
   LDA   __zpabi_validate_input__o_0
   JSR   draw_option
   JMP   .loop@5_start

store_selection_to_record:
   SUBROUTINE

   LDA   CURRENT_SELECTION
   LDY   #$01
   STA   (MENU_DESC_PTR),Y
   RTS

read_paddle:
   SUBROUTINE

   LDA   PTRIG
   LDY   #$00
.loop@6_continue:
   LDA   PADDL,X
   STA   __local_read_paddle__0
   BPL   .loop@6_break
   INY
   BNE   .loop@6_continue
   LDA   #$FF
   RTS
.loop@6_break:
   TYA
   RTS

joy_to_zone:
   SUBROUTINE

   STA   __local_joy_to_zone__old_zone
   TXA
   LSR
   LSR
   LSR
   LDX   __local_joy_to_zone__old_zone
   CMP   ZONE_THRESH_PLUS,X
   BCC   .if_end@13
   LDA   #$00
   RTS
.if_end@13:
   CMP   ZONE_THRESH_MINUS,X
   BCC   .if_end@14
   LDA   #$01
   RTS
.if_end@14:
   LDA   #$02
   RTS

check_joystick:
   SUBROUTINE

   LDA   #$C8
   JSR   WAIT
   LDX   #$00
   JSR   read_paddle
   TAX
   LDA   joy_zone_x
   JSR   joy_to_zone
   STA   joy_zone_x
   LDA   #$C8
   JSR   WAIT
   LDX   #$01
   JSR   read_paddle
   TAX
   LDA   joy_zone_y
   JSR   joy_to_zone
   STA   joy_zone_y
   CMP   #$01
   BNE   .if_else@16
   LDA   joy_zone_x
   BNE   .if_end@17
   LDA   #$04
   RTS
.if_end@17:
   CMP   #$02
   BNE   .if_end@15
   LDA   #$03
   RTS
.if_else@16:
   LDA   joy_zone_x
   CMP   #$01
   BNE   .if_end@19
   LDA   joy_zone_y
   CMP   #$02
   BNE   .cond_else@20
   LDA   #$01
   RTS
.cond_else@20:
   LDA   #$02
   RTS
.if_end@19:
.if_end@15:
   LDA   BUTN
   BPL   .if_end@22
   LDA   #$00
   RTS
.if_end@22:
   LDA   #$FF
   STA   KBDSTRB
   RTS

check_input:
   SUBROUTINE

   LDA   KBD
   STA   __local_check_input__2
   LDA   INPUT_MODE
   BEQ   .if_end@23
   JMP   check_joystick
.if_end@23:
   LDA   __local_check_input__2
   BPL   .if_end@24
   LDA   #$05
   STA   __local_check_input__dir
.loop@7_start:
   DEC   __local_check_input__dir
   LDA   __local_check_input__dir
   CMP   #$FF
   BEQ   .loop@7_break
   LDX   __local_check_input__dir
   LDA   __local_check_input__2
   CMP   DIR_KEYS,X
   BEQ   .check_input@scfold@0
   CMP   DIR_KEYS+5,X
   BEQ   .check_input@scfold@0
   CMP   DIR_KEYS+10,X
   BNE   .loop@7_start
.check_input@scfold@0:
   TXA
   RTS
.loop@7_break:
.if_end@24:
   LDA   #$FF
   STA   KBDSTRB
   RTS

read_keyboard:
   SUBROUTINE

   LDA   #$FF
   STA   KBDSTRB
   JSR   anim_tick_and_wait
   LDA   PRNG_OUTPUT
   AND   #$7E
   BNE   .if_end@30
   JSR   verify_integrity
.if_end@30:
   JSR   tick_anim_counter
   JMP   .loop@8_start
.read_keyboard@rot@2:
   JSR   idle_anim_handler
.loop@8_start:
   JSR   step_prng
   JSR   check_input
   STA   __local_read_keyboard__1
   AND   #$80
   BNE   .read_keyboard@rot@2
   LDA   __local_read_keyboard__1
   STA   KBDSTRB
   JSR   anim_check_frame
   LDA   __local_read_keyboard__1
   RTS

check_combat_eligibility:
   SUBROUTINE

   LDA   #$00
   STA   safe_to_rest
   STA   under_level
   LDY   #$0C
   LDA   (ENTITY_PTR),Y
   LSR
   AND   #$07
   SEC
   SBC   STEPS_TAKEN
   BPL   .if_end@32
   LDA   #$01
   STA   under_level
   RTS
.if_end@32:
   LDA   STEPS_TAKEN
   BEQ   .if_end@33
   RTS
.if_end@33:
   LDY   #$05
   LDA   (ENTITY_PTR),Y
   AND   #$1F
   STA   __local_check_combat_eligibility__1
   INY
   LDA   (ENTITY_PTR),Y
   AND   #$3F
   CMP   __local_check_combat_eligibility__1
   BCC   .if_end@34
   RTS
.if_end@34:
   LDY   #$03
   LDA   (ENTITY_PTR),Y
   JSR   check_adjacent_threats
   LDA   ADJACENT_THREAT
   BEQ   .if_end@35
   RTS
.if_end@35:
   LDY   #$03
   LDA   (ENTITY_PTR),Y
   JSR   check_encounter
   CMP   #$02
   BNE   .if_end@36
   RTS
.if_end@36:
   LDA   #$01
   STA   safe_to_rest
   RTS

configure_input:
   SUBROUTINE

   JSR   check_combat_eligibility
   LDA   safe_to_rest
   STA   GAME_INPUT_OPTIONS+45
   LDA   under_level
   STA   GAME_INPUT_OPTIONS+27
   EOR   #$01
   STA   GAME_INPUT_OPTIONS+9
   LDA   under_level
   EOR   #$01
   STA   GAME_INPUT_OPTIONS+18
   LDA   under_level
   CLC
   ADC   #$03
   STA   GAME_INPUT_TABLE+1
   LDA   MOVE_POINTS
   STA   GAME_INPUT_OPTIONS
   LDA   TURN_ACTIVE
   BNE   .if_end@37
   LDA   #$00
   STA   GAME_INPUT_OPTIONS
.if_end@37:
   LDA   GAME_INPUT_OPTIONS
   RTS

get_input_table_ptr:
   SUBROUTINE

   LDX   #>GAME_INPUT_TABLE
   LDA   #<GAME_INPUT_TABLE
   RTS

teardown_input:
   SUBROUTINE

   JSR   get_input_table_ptr
   JMP   input_loop

menu_see_your_lot:
   SUBROUTINE

   JSR   display_char_stats
   LDA   #$12
   RTS

menu_check_attack:
   SUBROUTINE

   JSR   check_attack_eligible
   BCC   .if_end@38
   LDA   #$0E
   RTS
.if_end@38:
   LDX   #>ATTACK_DIR_TABLE
   LDA   #<ATTACK_DIR_TABLE
   JMP   input_loop

menu_other_options:
   SUBROUTINE

   LDX   #>OTHER_OPTIONS_TABLE
   LDA   #<OTHER_OPTIONS_TABLE
   JMP   input_loop

retire_character:
   SUBROUTINE

   LDA   #$00
   STA   ACTIVE_CHAR
   JSR   reorder_current_char
   LDA   #$23
   JMP   scene_loop

menu_game_settings:
   SUBROUTINE

   LDA   #$01
   STA   GAME_SETTINGS_TABLE+1
   JMP   menu_game_settings_enter

menu_game_settings_enter:
   SUBROUTINE

   LDX   #>GAME_SETTINGS_TABLE
   LDA   #<GAME_SETTINGS_TABLE
   JMP   input_loop

menu_settings_init:
   SUBROUTINE

   JSR   settings_display_init
   LDA   ANIM_INDEX
   ORA   #$B0
   STA   S_GAME_SPEED+14
   RTS

settings_speed_up:
   SUBROUTINE

   JSR   store_selection_to_record
   INC   ANIM_INDEX
   LDA   ANIM_INDEX
   CMP   #$05
   BCC   .if_end@39
   LDA   #$05
   STA   ANIM_INDEX
.if_end@39:
   LDA   #$17
   RTS

settings_speed_down:
   SUBROUTINE

   JSR   store_selection_to_record
   DEC   ANIM_INDEX
   BNE   .if_end@40
   INC   ANIM_INDEX
.if_end@40:
   LDA   #$17
   RTS

settings_danger_up:
   SUBROUTINE

   JSR   store_selection_to_record
   JSR   inc_threshold_wrap
   LDA   #$17
   RTS

settings_danger_down:
   SUBROUTINE

   JSR   store_selection_to_record
   JSR   dec_threshold_wrap
   LDA   #$17
   RTS

menu_attack_target:
   SUBROUTINE

   LDX   #>ATTACK_TARGET_TABLE
   LDA   #<ATTACK_TARGET_TABLE
   JMP   input_loop

menu_drop_gold:
   SUBROUTINE

   LDX   #>DROP_GOLD_TABLE
   LDA   #<DROP_GOLD_TABLE
   JMP   input_loop

menu_drop_inc:
   SUBROUTINE

   JSR   store_selection_to_record
   LDA   #$36
   RTS

menu_drop_dec:
   SUBROUTINE

   JSR   store_selection_to_record
   LDA   #$37
   RTS

format_slot_number:
   SUBROUTINE

   LDA   #$A0
   STA   S_CONTINUE_GAME_NUMBER+22
   STA   S_SAVE_IN_SLOT+26
   LDA   slot_number
   STA   __local_format_slot_number__n
   CMP   #$0A
   BCC   .if_end@41
   SBC   #$0A
   STA   __local_format_slot_number__n
   LDA   #$B1
   STA   S_CONTINUE_GAME_NUMBER+22
   STA   S_SAVE_IN_SLOT+26
.if_end@41:
   LDA   __local_format_slot_number__n
   ORA   #$B0
   STA   S_CONTINUE_GAME_NUMBER+23
   STA   S_SAVE_IN_SLOT+27
   RTS

slot_update:
   SUBROUTINE

   JSR   store_selection_to_record
   JMP   format_slot_number

slot_inc:
   SUBROUTINE

   INC   slot_number
   LDA   slot_number
   CMP   #$11
   BCC   .if_end@42
   DEC   slot_number
.if_end@42:
   JMP   slot_update

slot_dec:
   SUBROUTINE

   DEC   slot_number
   BNE   .if_end@43
   INC   slot_number
.if_end@43:
   JMP   slot_update

init_world:
   SUBROUTINE

   LDX   #>SAVE_LOAD_TABLE
   LDA   #<SAVE_LOAD_TABLE
   JMP   input_loop

slot_inc_enter:
   SUBROUTINE

   JSR   slot_inc
   LDA   #$20
   RTS

slot_dec_enter:
   SUBROUTINE

   JSR   slot_dec
   LDA   #$20
   RTS

enter_save_game_slot:
   SUBROUTINE

   LDX   #>SAVE_GAME_SLOT_TABLE
   LDA   #<SAVE_GAME_SLOT_TABLE
   JMP   input_loop

slot_dec_reenter:
   SUBROUTINE

   JSR   slot_dec
   LDA   #$23
   RTS

slot_inc_reenter:
   SUBROUTINE

   JSR   slot_inc
   LDA   #$23
   RTS

call_input_init:
   SUBROUTINE

   CMP   #$01
   BEQ   .case@10
   CMP   #$02
   BEQ   .case@11
   CMP   #$03
   BEQ   .case@12
   CMP   #$04
   BEQ   .case@13
   CMP   #$05
   BEQ   .case@14
   CMP   #$06
   BEQ   .case@15
   RTS
.case@10:
   JMP   configure_input
.case@11:
   JMP   menu_settings_init
.case@12:
   JMP   init_attack_display
.case@13:
   JMP   init_drop_display
.case@14:
.case@15:
   JMP   format_slot_number

enter_char_select:
   SUBROUTINE

   LDX   #>CHAR_SELECT_TABLE
   LDA   #<CHAR_SELECT_TABLE
   JMP   input_loop

char_select_init:
   SUBROUTINE

   JSR   find_first_pc
   BCC   .if_end@44
   JMP   char_select_home
.if_end@44:
   JSR   set_cursor_row21
   LDA   #$35
   JSR   display_message
   LDA   #$02
   RTS

char_select_next:
   SUBROUTINE

   JSR   store_selection_to_record
   JSR   find_next_pc_wrap
   LDA   #$27
   RTS

char_select_prev:
   SUBROUTINE

   JSR   store_selection_to_record
   JSR   find_prev_pc
   LDA   #$27
   RTS

char_select_patch:
   SUBROUTINE

   STA   CHAR_SELECT_OPTIONS+45
   STX   CHAR_SELECT_OPTIONS+36
   LDA   #$02
   STA   CHAR_SELECT_TABLE+1
   JMP   enter_char_select

char_select_here:
   SUBROUTINE

   LDA   #$01
   STA   ACTIVE_CHAR
   LDX   #$00
   JMP   char_select_patch

char_select_home:
   SUBROUTINE

   LDA   CURRENT_PLAYER
   CMP   #$09
   BNE   .if_end@45
   JMP   char_select_here
.if_end@45:
   STA   ACTIVE_CHAR
   LDX   #$01
   LDA   #$00
   JMP   char_select_patch

start_game_input:
   SUBROUTINE

   LDX   #>ALT_INPUT_TABLE
   LDA   #<ALT_INPUT_TABLE
   JMP   input_loop

setup_data_iob:
   SUBROUTINE

   LDA   #$00
   LDY   #$04
   STA   (RWTS_IOB_PTR),Y
   INY
   STA   (RWTS_IOB_PTR),Y
   LDA   RWTS_IOB_PTR
   STA   DPTR
   LDA   RWTS_IOB_PTR+1
   STA   DPTR+1
   LDA   #<SCENE_TEXT_DATA
   LDY   #$08
   STA   (RWTS_IOB_PTR),Y
   LDA   #>SCENE_TEXT_DATA
   INY
   STA   (DPTR),Y
   RTS

verify_game_disk:
   SUBROUTINE

   JSR   setup_data_iob
   LDA   #$11
   LDY   #$04
   STA   (RWTS_IOB_PTR),Y
   LDA   #$00
   INY
   STA   (RWTS_IOB_PTR),Y
   LDA   #$02
   STA   __local_verify_game_disk__tries
.loop@16_continue:
   JSR   disk_read
   BCC   .loop@16_break
   DEC   __local_verify_game_disk__tries
   BNE   .loop@16_continue
   RTS
.loop@16_break:
   LDA   SCENE_TEXT_DATA
   CMP   #$03
   BNE   .or_true@47
   LDA   SCENE_TEXT_DATA+1
   CMP   #$0F
   BNE   .or_true@47
   LDA   SCENE_TEXT_DATA+2
   CMP   #$10
   BNE   .or_true@47
   CLC
   RTS
.or_true@47:
   SEC
   RTS

wait_keypress:
   SUBROUTINE

   LDA   #$30
   JSR   display_message
   LDA   #$00
   STA   KBDSTRB
.loop@17_continue:
   LDA   KBD
   BPL   .loop@17_continue
   RTS

prompt_insert_disk:
   SUBROUTINE

   JSR   set_cursor_row21
   LDA   #$03
   JSR   display_message
   JMP   wait_keypress

ensure_game_disk:
   SUBROUTINE

   JMP   .loop@18_continue
.ensure_game_disk@rot@3:
   JSR   prompt_insert_disk
.loop@18_continue:
   JSR   verify_game_disk
   BCS   .ensure_game_disk@rot@3
   RTS

format_disk_track:
   SUBROUTINE

   LDA   #$04
   JMP   rwts_dispatch

write_disk_signature:
   SUBROUTINE

   LDX   #$03
   BNE   .loop@19_start
.write_disk_signature@rot@4:
   LDA   DISK_SIGNATURE,X
   STA   SCENE_TEXT_DATA,X
.loop@19_start:
   DEX
   CPX   #$FF
   BNE   .write_disk_signature@rot@4
   JSR   setup_data_iob
   LDA   #$03
   STA   __local_write_disk_signature__tries
.write_disk_signature@rot@5:
   JSR   disk_write
   BCS   .if_end@51
   RTS
.if_end@51:
   DEC   __local_write_disk_signature__tries
   BNE   .write_disk_signature@rot@5
   RTS

verify_disk_signature:
   SUBROUTINE

   JSR   setup_data_iob
   LDA   #$03
   STA   __local_verify_disk_signature__tries
.loop@21_continue:
   JSR   disk_read
   BCC   .loop@21_break
   DEC   __local_verify_disk_signature__tries
   BNE   .loop@21_continue
   RTS
.loop@21_break:
   LDX   #$03
.loop@22_start:
   DEX
   CPX   #$FF
   BEQ   .loop@22_break
   LDA   SCENE_TEXT_DATA,X
   STA   __local_verify_disk_signature__0
   LDA   DISK_SIGNATURE,X
   CMP   __local_verify_disk_signature__0
   BEQ   .loop@22_start
   SEC
   RTS
.loop@22_break:
   CLC
   RTS

setup_slot_iob:
   SUBROUTINE

   LDA   slot_number
   STA   __local_setup_slot_iob__slot
   CMP   #$01
   BCC   .setup_slot_iob@asm_ssa_split@0
   DEC   __local_setup_slot_iob__slot
   LDX   #$01
.if_end@54:
   LDA   __local_setup_slot_iob__slot
   ASL
   CLC
   ADC   SAVE_TRACK_BASE,X
   LDY   #$04
   STA   (RWTS_IOB_PTR),Y
   LDA   #$00
   INY
   STA   (RWTS_IOB_PTR),Y
   LDA   RWTS_IOB_PTR
   STA   DPTR
   LDA   RWTS_IOB_PTR+1
   STA   DPTR+1
   LDA   #<LOCATIONS
   LDY   #$08
   STA   (RWTS_IOB_PTR),Y
   LDA   #>LOCATIONS
   INY
   STA   (DPTR),Y
   LDA   #$00
   LDY   #$0B
   STA   (RWTS_IOB_PTR),Y
   RTS
.setup_slot_iob@asm_ssa_split@0:
   LDX   #$00
   BEQ   .if_end@54

advance_sector:
   SUBROUTINE

   CLC
   LDY   #$09
   LDA   (RWTS_IOB_PTR),Y
   ADC   #$01
   STA   (RWTS_IOB_PTR),Y
   LDY   #$05
   LDA   (RWTS_IOB_PTR),Y
   CLC
   ADC   #$01
   CMP   #$0D
   BCC   .if_else@56
   LDA   #$00
   STA   (RWTS_IOB_PTR),Y
   CLC
   DEY
   LDA   (RWTS_IOB_PTR),Y
   ADC   #$01
   STA   (RWTS_IOB_PTR),Y
   RTS
.if_else@56:
   STA   (RWTS_IOB_PTR),Y
   RTS

transfer_game_data:
   SUBROUTINE

   STA   __local_transfer_game_data__command
   JSR   setup_slot_iob
   LDA   #$02
   STA   __local_transfer_game_data__tries
   LDA   #$1A
   STA   __local_transfer_game_data__left
.loop@23_start:
   LDA   __local_transfer_game_data__command
   JSR   rwts_dispatch
   BCC   .if_else@58
   DEC   __local_transfer_game_data__tries
   BNE   .loop@23_start
   RTS
.if_else@58:
   JSR   advance_sector
   DEC   __local_transfer_game_data__left
   BNE   .loop@23_start
   CLC
   RTS

write_game_data:
   SUBROUTINE

   LDA   #$02
   JMP   transfer_game_data

read_game_data:
   SUBROUTINE

   LDA   #$01
   JSR   transfer_game_data
   BCC   .if_end@61
   JSR   set_cursor_row21
   LDA   #$3B
   JSR   display_message
   JMP   cold_restart
.if_end@61:
   RTS

continue_game:
   SUBROUTINE

   LDA   slot_number
   CMP   #$01
   BCS   .if_end@62
   JSR   ensure_game_disk
   JSR   read_game_data
   LDA   #$00
   RTS
.if_end@62:
   JSR   set_cursor_row21
   LDA   #$33
   JSR   display_message
   JSR   wait_keypress
   JSR   verify_disk_signature
   BCC   .if_end@63
   JSR   set_cursor_row21
   LDA   #$16
   JSR   display_message
   JSR   ensure_game_disk
   LDA   #$20
   RTS
.if_end@63:
   JSR   read_game_data
   JSR   prompt_insert_disk
   JSR   ensure_game_disk
   LDA   #$00
   RTS

save_game:
   SUBROUTINE

   LDA   slot_number
   CMP   #$01
   BCS   .if_end@64
   JSR   ensure_game_disk
   JSR   write_game_data
   LDA   #$02
   RTS
.if_end@64:
   LDA   scenario_init_flag
   BNE   .if_end@65
   JSR   ensure_game_disk
   LDA   #$42
   JSR   scene_loop
   LDA   #$01
   STA   scenario_init_flag
.if_end@65:
   JMP   enter_scenario_disk

enter_scenario_disk:
   SUBROUTINE

   LDX   #>SCENARIO_DISK_TABLE
   LDA   #<SCENARIO_DISK_TABLE
   JMP   input_loop

scenario_done:
   SUBROUTINE

   JSR   prompt_insert_disk
   JSR   ensure_game_disk
   LDA   #$02
   RTS

cancel_scenario:
   SUBROUTINE

   JSR   ensure_game_disk
   LDA   #$02
   RTS

display_insert_msg:
   SUBROUTINE

   JSR   set_cursor_row21
   LDA   #$1B
   JMP   display_message

format_scenario_disk:
   SUBROUTINE

   JSR   verify_game_disk
   BCS   .if_end@66
   JSR   set_cursor_row21
   LDA   #$33
   JSR   display_message
   JMP   enter_scenario_disk
.if_end@66:
   JSR   verify_disk_signature
   BCC   .if_end@67
   JSR   format_disk_track
   BCS   .format_scenario_disk@scfold@0
   JSR   write_disk_signature
   BCC   .if_end@70
.format_scenario_disk@scfold@0:
   JSR   display_insert_msg
   JMP   scenario_done
.if_end@70:
.if_end@67:
   JSR   write_game_data
   JMP   scenario_done

use_scenario_disk:
   SUBROUTINE

   JSR   verify_disk_signature
   BCC   .if_else@72
   JSR   set_cursor_row21
   LDA   #$16
   JSR   display_message
   JMP   scenario_done
.if_else@72:
   JSR   write_game_data
.if_end@71:
   JMP   scenario_done

GAME_INPUT_TABLE:
   DC.B  $07
   DC.B  $03
   DC.B  $01
   DC.W  GAME_INPUT_OPTIONS

ATTACK_DIR_TABLE:
   DC.B  $05
   DC.B  $01
   DS.B  1
   DC.W  ATTACK_DIR_OPTIONS

OTHER_OPTIONS_TABLE:
   DC.B  $06
   DC.B  $01
   DS.B  1
   DC.W  OTHER_OPTIONS_OPTIONS

GAME_SETTINGS_TABLE:
   DC.B  $09
   DC.B  $01
   DC.B  $02
   DC.W  GAME_SETTINGS_OPTIONS

ATTACK_TARGET_TABLE:
   DC.B  $03
   DC.B  $02
   DC.B  $03
   DC.W  ATTACK_TARGET_OPTIONS

DROP_GOLD_TABLE:
   DC.B  $05
   DC.B  $02
   DC.B  $04
   DC.W  DROP_GOLD_OPTIONS

SAVE_LOAD_TABLE:
   DC.B  $04
   DC.B  $01
   DC.B  $05
   DC.W  SAVE_LOAD_OPTIONS

SAVE_GAME_SLOT_TABLE:
   DC.B  $04
   DC.B  $02
   DC.B  $06
   DC.W  SAVE_GAME_SLOT_OPTIONS

SCENARIO_DISK_TABLE:
   DC.B  $03
   DC.B  $01
   DS.B  1
   DC.W  SCENARIO_DISK_OPTIONS

CHAR_SELECT_TABLE:
   DC.B  $06
   DC.B  $02
   DS.B  1
   DC.W  CHAR_SELECT_OPTIONS

ALT_INPUT_TABLE:
   DC.B  $03
   DC.B  $02
   DS.B  1
   DC.W  ALT_INPUT_OPTIONS

S_SOUND_IS_ON:
   DC.B  $0C, $D3, $CF, $D5, $CE, $C4, $A0, $C9, $D3, $A0, $CF
   DC.B  $CE, $A0

S_DANGER:
   DC.B  $0B, $C4, $C1, $CE, $C7, $C5, $D2
   DC.B  $A0, $BA, $A0, $A0, $A0

S_ADD_NAME:
   DC.B  $18, $C1, $C4, $C4
   DC.B  $A0, $B1, $A0, $A0, $AE, $A0, $A0, $AE, $B5, $A0, $C6, $CC, $FE, $A0, $D8, $C1
   DC.B  $A0, $B5, $A0, $F2, $C2

S_CHEST_MESSAGE:
   DC.B  $1B, $D4, $C8, $C5, $A0, $D4, $D2, $C5, $C1, $D3, $D5, $D2, $C5, $A0, $C3, $C8
   DC.B  $C5, $D3, $D4, $A0, $CF, $D0, $C5, $CE, $D3, $A0, $D5, $D0
   DC.B  $1D, $D4, $CF, $A0, $D2, $C5, $D6, $C5, $C1, $CC, $A0
   DC.B  $A0, $A0, $CC, $A0, $EB
   DC.B  $B0, $A0, $C7, $CF, $CC, $C4, $A0, $C4, $D5, $C3, $C1, $D4, $D3, $AE
   DC.B  $1B, $D9, $CF, $D5, $A0, $CE, $CF, $D7, $A0, $C8, $C1, $D6, $C5, $A0
   DC.B  $A0, $E4, $CF, $D2, $A0
   DC.B  $B0, $A0, $C4, $D5, $C3, $C1, $D4, $D3, $AE
   DS.B  1

S_ATTACK_TARGET:
   DC.B  $1B, $C1, $D4, $D4, $C1, $C3, $CB
   DC.B  $A0, $B5, $E9, $BC, $A5, $A0, $D3, $C9, $C2, $C3, $CD, $A0, $A0, $D4, $B1, $A0
   DC.B  $A0, $A0, $A0, $A0, $C9

S_MISSES:
   DC.B  $06, $CD, $C9, $D3, $D3, $C5, $D3

S_DUCATS_TO_DROP:
   DC.B  $17, $C4, $D5, $C3, $C1, $D4, $D3, $A0, $D4, $CF, $A0, $C4, $D2, $CF, $D0, $A0
   DC.B  $BA, $A0
   DC.B  $A0, $B4, $CC, $A0, $BA
   DC.B  $B0

S_PAY_FOR:
   DC.B  $23, $D0, $C1, $D9, $A0
   DC.B  $B0, $B0, $B0, $B0, $B0
   DC.B  $B0, $A0, $C6, $CF, $D2, $A0
   DC.B  $A0, $DF, $A0, $A0, $A0, $A0, $D9, $A0, $E0, $A0, $A0, $A0, $C2, $B7, $A0, $B1
   DC.B  $C5, $A0, $C1, $F5

GAME_SIG_TRACK:
   DC.B  $11

GAME_SIG_SECTOR:
   DS.B  1

S_DROP_ARMOR:
   DC.B  $0A, $C4, $D2, $CF, $D0, $A0, $C1, $D2, $CD, $CF, $D2

S_RETIRE:
   DC.B  $06, $D2, $C5, $D4, $C9, $D2, $C5

S_ADD_SOMEONE:
   DC.B  $0B, $C1, $C4, $C4, $A0, $D3, $CF, $CD, $C5, $CF, $CE, $C5

S_DROP_GOLD:
   DC.B  $09, $C4, $D2, $CF, $D0, $A0, $C7, $CF, $CC, $C4

S_SAVE_GAME:
   DC.B  $09, $D3, $C1, $D6, $C5, $A0, $C7, $C1, $CD, $C5

S_TO_THIS_ROOM:
   DC.B  $0C, $D4, $CF, $A0, $D4, $C8, $C9, $D3, $A0, $D2, $CF, $CF, $CD

S_TO_ALI_BABAS_HOME:
   DC.B  $12, $D4, $CF, $A0, $C1, $CC, $C9, $A0, $C2, $C1, $C2, $C1, $A7, $D3, $A0, $C8
   DC.B  $CF, $CD, $C5

S_INCREASE:
   DC.B  $01, $A3

S_DECREASE:
   DC.B  $01, $C0

S_SEE_YOUR_LOT:
   DC.B  $0C, $D3, $C5, $C5, $A0, $D9, $CF, $D5, $D2, $A0, $CC, $CF, $D4

S_REST:
   DC.B  $04, $D2, $C5, $D3, $D4

S_DEFEND:
   DC.B  $06, $C4, $C5, $C6, $C5, $CE, $C4

S_STOP:
   DC.B  $04, $D3, $D4, $CF, $D0

S_CONTINUE_GAME_NUMBER:
   DC.B  $17, $C3, $CF, $CE, $D4, $C9, $CE, $D5, $C5, $A0, $C7, $C1, $CD, $C5, $A0, $CE
   DC.B  $D5, $CD, $C2, $C5, $D2, $A0
   DC.B  $CE, $CE

S_BEGIN_NEW_GAME:
   DC.B  $0E, $C2, $C5, $C7, $C9, $CE, $A0, $CE, $C5, $D7, $A0, $C7, $C1, $CD, $C5

S_USE_PREVIOUS_DISK:
   DC.B  $26, $D5, $D3, $C5, $A0, $D0, $D2, $C5, $D6, $C9, $CF, $D5, $D3, $CC, $D9, $A0
   DC.B  $C6, $CF, $D2, $CD, $C1, $D4, $D4, $C5, $C4, $A0, $D3, $C3, $C5, $CE, $C1, $D2
   DC.B  $C9, $CF, $A0, $C4, $C9, $D3, $CB

S_FORMAT_NEW_DISK:
   DC.B  $20, $C6, $CF, $D2, $CD, $C1, $D4, $A0, $C1, $CE, $C4, $A0, $D5, $D3, $C5, $A0
   DC.B  $CE, $C5, $D7, $A0, $D3, $C3, $C5, $CE, $C1, $D2, $C9, $CF, $A0, $C4, $C9, $D3
   DC.B  $CB

S_CANCEL_REQUEST:
   DC.B  $0E, $C3, $C1, $CE, $C3, $C5, $CC, $A0, $D2, $C5, $D1, $D5, $C5, $D3, $D4

S_ATTACK_ABOVE:
   DC.B  $10, $C1, $D4, $D4, $C1, $C3, $CB, $A0, $C1, $C2, $CF, $D6, $C5, $A0, $D9, $CF
   DC.B  $D5

S_ATTACK_LEFT:
   DC.B  $0B, $C1, $D4, $D4, $C1, $C3, $CB, $A0, $CC, $C5, $C6, $D4

S_ATTACK_RIGHT:
   DC.B  $0C, $C1, $D4, $D4, $C1, $C3, $CB, $A0, $D2, $C9, $C7, $C8, $D4

S_ATTACK_BELOW:
   DC.B  $10, $C1, $D4, $D4, $C1, $C3, $CB, $A0, $C2, $C5, $CC, $CF, $D7, $A0, $D9, $CF
   DC.B  $D5

S_ATTACK_SOMEONE_ELSE:
   DC.B  $13, $C1, $D4, $D4, $C1, $C3, $CB, $A0, $D3, $CF, $CD, $C5, $CF, $CE, $C5, $A0
   DC.B  $C5, $CC, $D3, $C5

S_CANCEL_ATTACK:
   DC.B  $0D, $C3, $C1, $CE, $C3, $C5, $CC, $A0, $C1, $D4, $D4, $C1, $C3, $CB

S_KEEP_ALL_GOLD:
   DC.B  $0D, $CB, $C5, $C5, $D0, $A0, $C1, $CC, $CC, $A0, $C7, $CF, $CC, $C4

S_BUY_SOMETHING_ELSE:
   DC.B  $12, $C2, $D5, $D9, $A0, $D3, $CF, $CD, $C5, $D4, $C8, $C9, $CE, $C7, $A0, $C5
   DC.B  $CC, $D3, $C5

S_NO_SALE:
   DC.B  $07, $CE, $CF, $A0, $D3, $C1, $CC, $C5

S_ATTACK:
   DC.B  $06, $C1, $D4, $D4, $C1, $C3, $CB

S_OTHER_OPTIONS:
   DC.B  $0D, $CF, $D4, $C8, $C5, $D2, $A0, $CF, $D0, $D4, $C9, $CF, $CE, $D3

S_MOVE:
   DC.B  $04, $CD, $CF, $D6, $C5

S_GAME_SPEED:
   DC.B  $0E, $C7, $C1, $CD, $C5, $A0, $D3, $D0, $C5, $C5, $C4, $A0, $BA, $A0
   DC.B  $B0

S_SAVE_IN_SLOT:
   DC.B  $1B, $D3, $C1, $D6, $C5, $A0, $C7, $C1, $CD, $C5, $A0, $C9, $CE, $A0, $D3, $CC
   DC.B  $CF, $D4, $A0, $CE, $D5, $CD, $C2, $C5, $D2, $A0
   DC.B  $B0, $B0

GAME_INPUT_OPTIONS:
   DC.B  $01
   DC.B  $07
   DC.B  $04, $02, $07, $05
   DC.W  S_MOVE
   DC.B  $02
   DC.B  $01
   DC.B  $2F
   DC.B  $01, $03, $05, $06
   DC.W  S_ATTACK
   DC.B  $14
   DC.B  $01
   DC.B  $57
   DC.B  $02, $04, $06, $04
   DC.W  S_DEFEND
   DC.B  $09
   DC.B  $01
   DC.B  $57
   DC.B  $03, $01, $03, $07
   DC.W  S_STOP
   DC.B  $05
   DC.B  $01
   DC.B  $14
   DC.B  $07, $06, $01, $02
   DC.W  S_OTHER_OPTIONS
   DC.B  $15
   DC.B  $01
   DC.B  $3C
   DC.B  $05, $07, $02, $03
   DC.W  S_REST
   DC.B  $08
   DC.B  $01
   DC.B  $64
   DC.B  $06, $05, $04, $01
   DC.W  S_SEE_YOUR_LOT
   DC.B  $13

ATTACK_DIR_OPTIONS:
   DC.B  $01
   DC.B  $39
   DC.B  $03, $05, $02, $04
   DC.W  S_DEFEND
   DC.B  $09
   DC.B  $01
   DC.B  $29
   DC.B  $03, $05, $04, $01
   DC.W  S_ATTACK_LEFT
   DC.B  $0B
   DC.B  $01
   DC.B  $0C
   DC.B  $05, $01, $02, $04
   DC.W  S_ATTACK_ABOVE
   DC.B  $0C
   DC.B  $01
   DC.B  $44
   DC.B  $03, $05, $01, $02
   DC.W  S_ATTACK_RIGHT
   DC.B  $0A
   DC.B  $01
   DC.B  $5C
   DC.B  $01, $03, $02, $04
   DC.W  S_ATTACK_BELOW
   DC.B  $0D

OTHER_OPTIONS_OPTIONS:
   DC.B  $01
   DC.B  $05
   DC.B  $03, $02, $06, $04
   DC.W  S_MOVE
   DC.B  $02
   DC.B  $01
   DC.B  $2D
   DC.B  $01, $03, $04, $05
   DC.W  S_DROP_GOLD
   DC.B  $35
   DC.B  $01
   DC.B  $55
   DC.B  $02, $01, $05, $06
   DC.W  S_DROP_ARMOR
   DC.B  $34
   DC.B  $01
   DC.B  $15
   DC.B  $06, $05, $01, $02
   DC.W  S_OTHER_OPTIONS
   DC.B  $16
   DC.B  $01
   DC.B  $3D
   DC.B  $04, $06, $02, $03
   DC.W  S_RETIRE
   DC.B  $33
   DC.B  $01
   DC.B  $65
   DC.B  $05, $04, $03, $01
   DC.W  S_ADD_SOMEONE
   DC.B  $28

GAME_SETTINGS_OPTIONS:
   DC.B  $01
   DC.B  $03
   DC.B  $05, $09, $08, $03
   DC.W  S_MOVE
   DC.B  $02
   DC.B  $01
   DC.B  $13
   DS.B  4
   DC.W  S_GAME_SPEED
   DS.B  1
   DC.B  $01
   DC.B  $23
   DC.B  $07, $07, $01, $04
   DC.W  S_INCREASE
   DC.B  $18
   DC.B  $01
   DC.B  $25
   DC.B  $08, $08, $03, $09
   DC.W  S_DECREASE
   DC.B  $19
   DC.B  $01
   DC.B  $53
   DC.B  $09, $01, $09, $07
   DC.W  S_SOUND_IS_ON
   DC.B  $06
   DC.B  $01
   DC.B  $66
   DS.B  4
   DC.W  S_DANGER
   DS.B  1
   DC.B  $01
   DC.B  $73
   DC.B  $03, $03, $05, $08
   DC.W  S_INCREASE
   DC.B  $1A
   DC.B  $01
   DC.B  $75
   DC.B  $04, $04, $07, $01
   DC.W  S_DECREASE
   DC.B  $1B
   DC.B  $01
   DC.B  $2B
   DC.B  $01, $05, $04, $05
   DC.W  S_SAVE_GAME
   DC.B  $23

ATTACK_TARGET_OPTIONS:
   DC.B  $01
   DC.B  $06
   DC.B  $03, $02, $00, $00
   DC.W  S_ATTACK_TARGET
   DC.B  $10
   DC.B  $01
   DC.B  $2E
   DC.B  $01, $03, $00, $00
   DC.W  S_ATTACK_SOMEONE_ELSE
   DC.B  $0F
   DC.B  $01
   DC.B  $56
   DC.B  $02, $01, $00, $00
   DC.W  S_CANCEL_ATTACK
   DS.B  1

DROP_GOLD_OPTIONS:
   DC.B  $01
   DC.B  $2A
   DS.B  4
   DC.W  S_DUCATS_TO_DROP
   DC.B  $1D
   DC.B  $01
   DC.B  $43
   DC.B  $04, $04, $04, $03
   DC.W  S_INCREASE
   DC.B  $1F
   DC.B  $01
   DC.B  $45
   DC.B  $04, $04, $02, $04
   DC.W  S_DECREASE
   DC.B  $1E
   DC.B  $01
   DC.B  $52
   DC.B  $02, $03, $03, $05
   DC.W  S_DROP_GOLD
   DC.B  $38
   DC.B  $01
   DC.B  $69
   DC.B  $02, $03, $04, $04
   DC.W  S_KEEP_ALL_GOLD
   DC.B  $02

SAVE_LOAD_OPTIONS:
   DC.B  $01
   DC.B  $05
   DC.B  $02, $02, $00, $00
   DC.W  S_BEGIN_NEW_GAME
   DS.B  1
   DC.B  $01
   DC.B  $55
   DC.B  $01, $01, $04, $03
   DC.W  S_CONTINUE_GAME_NUMBER
   DC.B  $2E
   DC.B  $01
   DC.B  $6F
   DC.B  $01, $01, $02, $04
   DC.W  S_INCREASE
   DC.B  $21
   DC.B  $01
   DC.B  $71
   DC.B  $01, $01, $03, $02
   DC.W  S_DECREASE
   DC.B  $22

SAVE_GAME_SLOT_OPTIONS:
   DC.B  $01
   DC.B  $04
   DC.B  $02, $02, $02, $02
   DC.W  S_MOVE
   DC.B  $02
   DC.B  $01
   DC.B  $2C
   DC.B  $01, $01, $04, $03
   DC.W  S_SAVE_IN_SLOT
   DC.B  $2F
   DC.B  $01
   DC.B  $49
   DC.B  $01, $01, $02, $04
   DC.W  S_INCREASE
   DC.B  $24
   DC.B  $01
   DC.B  $4B
   DC.B  $01, $01, $03, $02
   DC.W  S_DECREASE
   DC.B  $25

SCENARIO_DISK_OPTIONS:
   DC.B  $01
   DS.B  1
   DC.B  $03, $02, $00, $00
   DC.W  S_USE_PREVIOUS_DISK
   DC.B  $32
   DC.B  $01
   DC.B  $28
   DC.B  $01, $03, $00, $00
   DC.W  S_FORMAT_NEW_DISK
   DC.B  $31
   DC.B  $01
   DC.B  $50
   DC.B  $02, $01, $00, $00
   DC.W  S_CANCEL_REQUEST
   DC.B  $30

CHAR_SELECT_OPTIONS:
   DC.B  $01
   DC.B  $05
   DC.B  $06, $02, $06, $02
   DC.W  S_MOVE
   DC.B  $02
   DC.B  $01
   DC.B  $2D
   DC.B  $01, $05, $01, $03
   DC.W  S_ADD_NAME
   DC.B  $07
   DC.B  $01
   DC.B  $49
   DC.B  $01, $05, $02, $04
   DC.W  S_INCREASE
   DC.B  $29
   DC.B  $01
   DC.B  $4B
   DC.B  $01, $05, $03, $05
   DC.W  S_DECREASE
   DC.B  $2A
   DC.B  $01
   DC.B  $55
   DC.B  $02, $06, $04, $06
   DC.W  S_TO_THIS_ROOM
   DC.B  $2B
   DS.B  1
   DC.B  $55
   DC.B  $05, $01, $05, $01
   DC.W  S_TO_ALI_BABAS_HOME
   DC.B  $2C

ALT_INPUT_OPTIONS:
   DC.B  $01
   DC.B  $05
   DC.B  $03, $02, $00, $00
   DC.W  S_PAY_FOR
   DC.B  $39
   DC.B  $01
   DC.B  $2D
   DC.B  $01, $03, $00, $00
   DC.W  S_BUY_SOMETHING_ELSE
   DC.B  $3A
   DC.B  $01
   DC.B  $55
   DC.B  $02, $01, $00, $00
   DC.W  S_NO_SALE
   DS.B  1

DIR_KEYS:
   DC.B  $A0, $C9, $CD, $CA, $CB
   DC.B  $8D, $E9, $ED, $EA, $EB
   DC.B  $9B, $8B, $8A, $88, $95

ZONE_THRESH_PLUS:
   DC.B  $1A, $1D, $1D

ZONE_THRESH_MINUS:
   DC.B  $03, $03, $06

SAVE_TRACK_BASE:
   DC.B  $13, $01

DISK_SIGNATURE:
   DC.B  $C1, $CC, $C9

scenario_init_flag:
   DS.B  1

__zp_init:
   SUBROUTINE

   LDA   #$00
   STA   under_level
   STA   safe_to_rest
   LDA   #$01
   STA   joy_zone_x
   STA   joy_zone_y
   STA   slot_number
   RTS
