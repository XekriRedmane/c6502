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
__local_enter_combat_state__pos	EQU	$80
__local_handle_mob_encounter__courage	EQU	$80
__local_process_entity_script__0	EQU	$80
__local_select_combat_target__0	EQU	$80
__zpabi_apply_damage__damage	EQU	$80
__zpabi_num_to_decimal__out_0	EQU	$80
__local_get_entity_threat__0	EQU	$81
__local_handle_mob_encounter__0	EQU	$81
__local_map_value_to_string__index	EQU	$81
__local_pick_random_member__n	EQU	$81
__zpabi_modify_char_stats__amount_0	EQU	$81
__zpabi_num_to_decimal__out_1	EQU	$81
__local_map_value_to_string__verb	EQU	$82
__zpabi_modify_char_stats__amount_1	EQU	$82
__local_init_attack_display__i	EQU	$83
__local_map_value_to_string__1	EQU	$83
__zpabi_modify_char_stats__add	EQU	$83
__zpabi_modify_current_stats__add	EQU	$83
__zpabi_update_event_list__value_0	EQU	$83
__local_check_encounter__ref_1	EQU	$84
__local_init_attack_display__left	EQU	$84
__zpabi_update_event_list__value_1	EQU	$84
__local_check_encounter__ref_0	EQU	$85
__local_handle_encounter__dest	EQU	$85
__local_handle_event_slot__keep	EQU	$85
__local_init_attack_display__2	EQU	$85
__local_handle_encounter__ev	EQU	$86
__local_handle_event_slot__0	EQU	$86
__local_init_attack_display__3	EQU	$86
__local_handle_encounter__0	EQU	$87
__local_handle_event_slot__ev	EQU	$87
__local_handle_event_slot__1	EQU	$88
__local_check_adjacent_threats__pos	EQU	$8A
__local_find_attack_target__1	EQU	$8A
__local_resolve_attack__0	EQU	$8B
__local_resolve_attack__1	EQU	$8C
__local_sum_hostile_at_pos__0	EQU	$8C
__local_check_hostile__other	EQU	$8D
__local_player_attack__0	EQU	$8D
__local_check_hostile__0	EQU	$8E
__local_find_mob_at_same_pos__1	EQU	$8E
__local_mob_attack__1	EQU	$8E
__local_player_attack__1	EQU	$8E
__local_find_mob_at_same_pos__2	EQU	$8F
__local_player_attack__2	EQU	$8F
__local_find_mob_at_same_pos__link	EQU	$90
__local_player_attack__target	EQU	$90
__local_player_attack__3	EQU	$91
__local_calc_combat_strength__strength	EQU	$92
__local_apply_damage__msg	EQU	$93
__local_calc_combat_strength__attacker	EQU	$93
__local_apply_damage__tune	EQU	$94
__local_calc_combat_strength__0	EQU	$94
__local_apply_damage__0	EQU	$95
__local_calc_combat_strength__weapon	EQU	$95
__local_apply_damage__rec	EQU	$96
__local_apply_damage__1	EQU	$97
__local_modify_char_stats__levels	EQU	$98
__local_modify_char_stats__0	EQU	$99
__local_modify_char_stats__threshold_1	EQU	$9A
__local_modify_char_stats__threshold_0	EQU	$9B
__local_modify_char_stats__gold_1	EQU	$9C
__local_modify_char_stats__gold_0	EQU	$9D
__local_modify_char_stats__rec	EQU	$9E
__local_modify_char_stats__1	EQU	$9F
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
; def apply_damage params=__zpabi_apply_damage__rec_0,__zpabi_apply_damage__rec_1,__zpabi_apply_damage__damage locals=__local_apply_damage__msg,__local_apply_damage__tune,__local_apply_damage__0,__local_apply_damage__rec,__local_apply_damage__1 indirect=false in_cycle=false param_regs=A,X,
; def apply_damage_to_current params=__zpabi_apply_damage_to_current__damage locals= indirect=false in_cycle=false param_regs=A
; def apply_damage_to_mob params=__zpabi_apply_damage_to_mob__damage locals= indirect=false in_cycle=false param_regs=A
; def attack_down params= locals=__local_attack_down__0 indirect=false in_cycle=false
; def attack_left params= locals=__local_attack_left__0 indirect=false in_cycle=false
; def attack_right params= locals=__local_attack_right__0 indirect=false in_cycle=false
; def attack_up params= locals=__local_attack_up__0 indirect=false in_cycle=false
; def auto_heal params= locals=__local_auto_heal__0 indirect=false in_cycle=false
; def calc_combat_strength params= locals=__local_calc_combat_strength__strength,__local_calc_combat_strength__attacker,__local_calc_combat_strength__0,__local_calc_combat_strength__weapon,__local_calc_combat_strength__1,__local_calc_combat_strength__2 indirect=false in_cycle=false
; def check_adjacent_threats params=__zpabi_check_adjacent_threats__pos locals=__local_check_adjacent_threats__0,__local_check_adjacent_threats__pos indirect=false in_cycle=false param_regs=A
; def check_attack_eligible params= locals=__local_check_attack_eligible__0 indirect=false in_cycle=false carry=true
; def check_encounter params=__zpabi_check_encounter__pos locals=__local_check_encounter__result,__local_check_encounter__ref_1,__local_check_encounter__ref_0,__local_check_encounter__0 indirect=false in_cycle=false param_regs=A
; def check_hostile params=__zpabi_check_hostile__other_0,__zpabi_check_hostile__other_1 locals=__local_check_hostile__other,__local_check_hostile__0,__local_check_hostile__1 indirect=false in_cycle=false param_regs=A,X carry=true
; def check_threats_here params= locals=__local_check_threats_here__0 indirect=false in_cycle=false
; def enter_combat_state params=__zpabi_enter_combat_state__pos locals=__local_enter_combat_state__pos,__local_enter_combat_state__0 indirect=false in_cycle=false param_regs=A
; def event_prng_check params= locals=__local_event_prng_check__0 indirect=false in_cycle=false
; def find_attack_target params= locals=__local_find_attack_target__0,__local_find_attack_target__1 indirect=false in_cycle=false
; def find_mob_at_same_pos params= locals=__local_find_mob_at_same_pos__0,__local_find_mob_at_same_pos__1,__local_find_mob_at_same_pos__2,__local_find_mob_at_same_pos__link indirect=false in_cycle=false
; def get_entity_threat params= locals=__local_get_entity_threat__0 indirect=false in_cycle=false
; def get_hostility params=__zpabi_get_hostility__group locals=__local_get_hostility__group indirect=false in_cycle=false param_regs=A
; def handle_encounter params=__zpabi_handle_encounter__ev_0,__zpabi_handle_encounter__ev_1 locals=__local_handle_encounter__dest,__local_handle_encounter__ev,__local_handle_encounter__0,__local_handle_encounter__1 indirect=false in_cycle=false param_regs=A,X
; def handle_event_slot params=__zpabi_handle_event_slot__ev_0,__zpabi_handle_event_slot__ev_1 locals=__local_handle_event_slot__keep,__local_handle_event_slot__0,__local_handle_event_slot__ev,__local_handle_event_slot__1 indirect=false in_cycle=false param_regs=A,X
; def handle_mob_encounter params= locals=__local_handle_mob_encounter__courage,__local_handle_mob_encounter__0,__local_handle_mob_encounter__1 indirect=false in_cycle=false
; def increment_hp params= locals=__local_increment_hp__0 indirect=false in_cycle=false
; def init_attack_display params= locals=__local_init_attack_display__0,__local_init_attack_display__1,__local_init_attack_display__c,__local_init_attack_display__i,__local_init_attack_display__left,__local_init_attack_display__2,__local_init_attack_display__3 indirect=false in_cycle=false
; def load_target_ptr params= locals= indirect=false in_cycle=false
; def map_value_to_string params=__zpabi_map_value_to_string__value locals=__local_map_value_to_string__0,__local_map_value_to_string__index,__local_map_value_to_string__verb,__local_map_value_to_string__1 indirect=false in_cycle=false param_regs=A
; def mob_attack params= locals=__local_mob_attack__0,__local_mob_attack__1 indirect=false in_cycle=false
; def modify_char_stats params=__zpabi_modify_char_stats__rec_0,__zpabi_modify_char_stats__rec_1,__zpabi_modify_char_stats__amount_0,__zpabi_modify_char_stats__amount_1,__zpabi_modify_char_stats__add locals=__local_modify_char_stats__levels,__local_modify_char_stats__0,__local_modify_char_stats__threshold_1,__local_modify_char_stats__threshold_0,__local_modify_char_stats__gold_1,__local_modify_char_stats__gold_0,__local_modify_char_stats__rec,__local_modify_char_stats__1 indirect=false in_cycle=false param_regs=A,X,,,
; def modify_current_stats params=__zpabi_modify_current_stats__amount_0,__zpabi_modify_current_stats__amount_1,__zpabi_modify_current_stats__add locals=__local_modify_current_stats__amount_1 indirect=false in_cycle=false param_regs=A,X,
; def next_colocated_mob params= locals=__local_next_colocated_mob__0 indirect=false in_cycle=false
; def pick_random_member params= locals=__local_pick_random_member__0,__local_pick_random_member__n indirect=false in_cycle=false
; def player_attack params= locals=__local_player_attack__0,__local_player_attack__1,__local_player_attack__2,__local_player_attack__target,__local_player_attack__3 indirect=false in_cycle=false
; def process_entity_script params= locals=__local_process_entity_script__0,__local_process_entity_script__entry,__local_process_entity_script__1 indirect=false in_cycle=false
; def resolve_attack params= locals=__local_resolve_attack__0,__local_resolve_attack__1 indirect=false in_cycle=false
; def select_combat_target params= locals=__local_select_combat_target__0 indirect=false in_cycle=false
; def select_someone_else params= locals= indirect=false in_cycle=false
; def set_target_restricted params= locals=__local_set_target_restricted__0 indirect=false in_cycle=false
; def show_locked_msg params= locals= indirect=false in_cycle=false
; def start_combat params= locals= indirect=false in_cycle=false
; def sum_hostile_at_pos params=__zpabi_sum_hostile_at_pos__pos locals=__local_sum_hostile_at_pos__threat,__local_sum_hostile_at_pos__0 indirect=false in_cycle=false param_regs=A
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
; ext border_anim_step params=
; ext browse_handler params=
; ext cancel_scenario params=
; ext char_select_here params=
; ext char_select_home params=
; ext char_select_init params=
; ext char_select_next params=
; ext char_select_prev params=
; ext check_at_boundary params=__zpabi_check_at_boundary__pos param_regs=A
; ext check_can_leave params= carry=true
; ext check_scene_events params=
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
; ext enter_location params=
; ext enter_save_game_slot params=
; ext enter_scenario_disk params=
; ext fill_map params=
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
; ext hrcg_cout params=__zpabi_hrcg_cout__c param_regs=A
; ext hrcg_init params=
; ext idle_anim_handler params=
; ext inc_threshold_wrap params=
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
; ext next_group_member params=
; ext npc_move_ai params=
; ext num_to_decimal params=__zpabi_num_to_decimal__value_0,__zpabi_num_to_decimal__value_1,__zpabi_num_to_decimal__out_0,__zpabi_num_to_decimal__out_1 param_regs=A,X,,
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
; ext retire_character params=
; ext run_chain params=__zpabi_run_chain__state param_regs=A
; ext rwts_dispatch params=__zpabi_rwts_dispatch__command param_regs=A carry=true
; ext save_game params=
; ext scene_loop params=__zpabi_scene_loop__scene param_regs=A
; ext scene_setup params=
; ext scroll_status_line params=
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
; ext slot_dec_enter params=
; ext slot_dec_reenter params=
; ext slot_inc_enter params=
; ext slot_inc_reenter params=
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
; call apply_damage -> display_message
; call apply_damage -> draw_char_at_pos
; call apply_damage -> modify_char_stats
; call apply_damage -> print_entity_name
; call apply_damage -> random_in_range
; call apply_damage -> reorder_char
; call apply_damage -> set_cursor_row21
; call apply_damage -> tune_player
; call apply_damage -> update_event_list
; call apply_damage_to_current -> apply_damage
; call apply_damage_to_mob -> apply_damage
; call attack_down -> find_attack_target
; call attack_left -> attack_up
; call attack_left -> find_attack_target
; call attack_right -> attack_left
; call attack_right -> find_attack_target
; call attack_up -> attack_down
; call attack_up -> find_attack_target
; call auto_heal -> display_message
; call auto_heal -> increment_hp
; call auto_heal -> print_mob_name
; call auto_heal -> random_in_range
; call auto_heal -> scroll_status_line
; call auto_heal -> set_cursor_row21
; call auto_heal -> step_prng
; call calc_combat_strength -> random_in_range
; call calc_combat_strength -> scene_loop
; call calc_combat_strength -> set_cursor_row21
; call check_adjacent_threats -> sum_hostile_at_pos
; call check_attack_eligible -> check_hostile
; call check_attack_eligible -> find_mob_at_same_pos
; call check_encounter -> check_hostile
; call check_encounter -> find_entity_at_pos
; call check_encounter -> find_mob_at_same_pos
; call check_hostile -> appearance_to_fontchar
; call check_hostile -> get_hostility
; call check_threats_here -> check_adjacent_threats
; call check_threats_here -> colrow_to_pos
; call enter_combat_state -> check_encounter
; call enter_combat_state -> clear_restricted
; call enter_combat_state -> set_char_state
; call enter_combat_state -> set_restricted
; call event_prng_check -> apply_damage_to_current
; call event_prng_check -> delay_with_animation
; call event_prng_check -> display_message
; call event_prng_check -> print_mob_name
; call event_prng_check -> set_cursor_row21
; call event_prng_check -> set_restricted
; call event_prng_check -> step_prng
; call find_attack_target -> colrow_to_pos
; call find_attack_target -> find_entity_at_pos
; call find_attack_target -> find_mob_at_same_pos
; call find_attack_target -> next_colocated_mob
; call find_attack_target -> resolve_attack
; call find_mob_at_same_pos -> get_mob_data
; call get_entity_threat -> check_hostile
; call get_entity_threat -> manhattan_distance
; call handle_encounter -> apply_damage_to_current
; call handle_encounter -> commit_move
; call handle_encounter -> display_message
; call handle_encounter -> pos_to_colrow
; call handle_encounter -> random_in_range
; call handle_encounter -> reorder_current_char
; call handle_encounter -> reset_turn_pos
; call handle_encounter -> scene_loop
; call handle_encounter -> set_char_state
; call handle_encounter -> set_cursor_row21
; call handle_encounter -> step_prng
; call handle_encounter -> timed_wait
; call handle_encounter -> tune_player
; call handle_event_slot -> anim_tick_and_wait
; call handle_event_slot -> apply_damage_to_current
; call handle_event_slot -> check_threats_here
; call handle_event_slot -> commit_move
; call handle_event_slot -> display_message
; call handle_event_slot -> event_prng_check
; call handle_event_slot -> print_mob_name
; call handle_event_slot -> random_in_range
; call handle_event_slot -> reset_turn_pos
; call handle_event_slot -> scene_loop
; call handle_event_slot -> scroll_status_line
; call handle_event_slot -> set_char_state
; call handle_event_slot -> set_cursor_row21
; call handle_event_slot -> show_locked_msg
; call handle_event_slot -> tune_player
; call handle_mob_encounter -> check_encounter
; call handle_mob_encounter -> check_threats_here
; call handle_mob_encounter -> colrow_to_pos
; call handle_mob_encounter -> commit_move
; call handle_mob_encounter -> display_message
; call handle_mob_encounter -> first_group_member
; call handle_mob_encounter -> next_group_member
; call handle_mob_encounter -> print_mob_name
; call handle_mob_encounter -> scroll_status_line
; call handle_mob_encounter -> set_cursor_row21
; call handle_mob_encounter -> set_target_restricted
; call handle_mob_encounter -> step_prng
; call handle_mob_encounter -> sum_hostile_at_pos
; call increment_hp -> display_message
; call increment_hp -> print_mob_name
; call increment_hp -> scroll_status_line
; call increment_hp -> set_cursor_row21
; call map_value_to_string -> print_bottom_centered
; call mob_attack -> apply_damage_to_mob
; call mob_attack -> calc_combat_strength
; call mob_attack -> dispatch_on_mode
; call mob_attack -> display_message
; call mob_attack -> map_value_to_string
; call mob_attack -> print_member_name
; call mob_attack -> print_mob_name
; call mob_attack -> scroll_status_line
; call mob_attack -> set_cursor_row21
; call mob_attack -> tune_player
; call modify_current_stats -> modify_char_stats
; call next_colocated_mob -> find_mob_at_same_pos
; call pick_random_member -> first_group_member
; call pick_random_member -> next_group_member
; call pick_random_member -> random_in_range
; call player_attack -> calc_combat_strength
; call player_attack -> classify_entity_type
; call player_attack -> dispatch_on_mode
; call player_attack -> display_message
; call player_attack -> draw_char_at_pos
; call player_attack -> event_prng_check
; call player_attack -> get_entity_fontchar
; call player_attack -> map_value_to_string
; call player_attack -> pos_to_colrow
; call player_attack -> print_bottom_centered
; call player_attack -> print_mob_name
; call player_attack -> process_entity_script
; call player_attack -> random_in_range
; call player_attack -> render_font_char
; call player_attack -> scene_loop
; call player_attack -> scroll_status_line
; call player_attack -> set_cursor_row21
; call player_attack -> tune_player
; call process_entity_script -> dispatch_on_mode
; call process_entity_script -> draw_char_at_pos
; call process_entity_script -> tune_player
; call resolve_attack -> delay_with_animation
; call resolve_attack -> mob_attack
; call resolve_attack -> player_attack
; call resolve_attack -> print_bottom_centered
; call resolve_attack -> print_member_name
; call resolve_attack -> print_mob_name
; call resolve_attack -> random_in_range
; call resolve_attack -> scroll_status_line
; call resolve_attack -> set_blink_target
; call resolve_attack -> set_cursor_row21
; call select_combat_target -> check_encounter
; call select_combat_target -> check_threats_here
; call select_combat_target -> first_group_member
; call select_combat_target -> get_entity_threat
; call select_combat_target -> next_group_member
; call select_someone_else -> next_colocated_mob
; call show_locked_msg -> display_message
; call show_locked_msg -> print_mob_name
; call show_locked_msg -> scroll_status_line
; call show_locked_msg -> set_char_state
; call show_locked_msg -> set_cursor_row21
; call start_combat -> load_target_ptr
; call start_combat -> resolve_attack
; call start_combat -> set_char_state
; call sum_hostile_at_pos -> check_hostile
; call sum_hostile_at_pos -> find_entity_at_pos
; call sum_hostile_at_pos -> find_mob_at_same_pos
; share member=modify_char_stats:__zpabi_modify_char_stats__amount_0,__zpabi_modify_char_stats__amount_1 holder=apply_damage live=apply_damage:modify_char_stats forward=apply_damage>modify_char_stats writer=apply_damage
; share member=modify_char_stats:__zpabi_modify_char_stats__add member=modify_current_stats:__zpabi_modify_current_stats__add forward=modify_current_stats>modify_char_stats
; item check_encounter section=.text local=false zero=false
; item check_encounter@run3 section=.text local=true zero=false
; item find_mob_at_same_pos section=.text local=true zero=false
; item get_hostility section=.text local=true zero=false
; item check_hostile section=.text local=false zero=false
; item check_adjacent_threats section=.text local=false zero=false
; item sum_hostile_at_pos section=.text local=true zero=false
; item check_threats_here section=.text local=false zero=false
; item check_threats_here@run4 section=.text local=true zero=false
; item next_colocated_mob section=.text local=true zero=false
; item pick_random_member section=.text local=false zero=false
; item check_attack_eligible section=.text local=false zero=false
; item apply_damage section=.text local=true zero=false
; item apply_damage_to_current section=.text local=false zero=false
; item apply_damage_to_mob section=.text local=true zero=false
; item increment_hp section=.text local=true zero=false
; item increment_hp@run0 section=.text local=true zero=false
; item auto_heal section=.text local=false zero=false
; item enter_combat_state section=.text local=false zero=false
; item load_target_ptr section=.text local=true zero=false
; item start_combat section=.text local=false zero=false
; item attack_right section=.text local=false zero=false
; item attack_left section=.text local=false zero=false
; item attack_up section=.text local=false zero=false
; item attack_down section=.text local=false zero=false
; item find_attack_target section=.text local=false zero=false
; item init_attack_display section=.text local=false zero=false
; item select_someone_else section=.text local=false zero=false
; item resolve_attack section=.text local=false zero=false
; item resolve_attack@run6 section=.text local=true zero=false
; item calc_combat_strength section=.text local=true zero=false
; item map_value_to_string section=.text local=true zero=false
; item player_attack section=.text local=true zero=false
; item player_attack@run5 section=.text local=true zero=false
; item player_attack@tail0 section=.text local=true zero=false
; item process_entity_script section=.text local=true zero=false
; item mob_attack section=.text local=true zero=false
; item get_entity_threat section=.text local=true zero=false
; item select_combat_target section=.text local=false zero=false
; item handle_encounter section=.text local=false zero=false
; item handle_encounter@run2 section=.text local=true zero=false
; item handle_encounter@run1 section=.text local=true zero=false
; item show_locked_msg section=.text local=false zero=false
; item handle_event_slot section=.text local=false zero=false
; item event_prng_check section=.text local=true zero=false
; item set_target_restricted section=.text local=true zero=false
; item handle_mob_encounter section=.text local=false zero=false
; item modify_char_stats section=.text local=true zero=false
; item modify_current_stats section=.text local=false zero=false
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

check_encounter:
   SUBROUTINE

   JSR   find_entity_at_pos
   STA   __local_check_encounter__ref_1
   TXA
   BPL   .check_encounter@asm_ssa_split@0
   LDA   __local_check_encounter__ref_1
   JSR   check_encounter@run3
.loop@0_start:
   LDA   MOB_PTR
   STA   __local_check_encounter__ref_0
   LDA   MOB_PTR+1
   STA   __local_check_encounter__ref_1
   JSR   find_mob_at_same_pos
   ORA   #$00
   BNE   .if_end@1
   LDA   #$00
   BEQ   .loop@0_break
.if_end@1:
   CMP   #$01
   BNE   .if_end@2
   LDA   #$01
   BNE   .loop@0_break
.if_end@2:
   LDY   #$06
   LDA   (MOB_PTR),Y
   AND   #$3F
   CMP   #$03
   BCC   .loop@0_start
   LDX   __local_check_encounter__ref_1
   LDA   __local_check_encounter__ref_0
   JSR   check_hostile
   BCC   .loop@0_start
   LDA   #$02
.loop@0_break:
.if_end@0:
   STA   ENCOUNTER_RESULT
   RTS
.check_encounter@asm_ssa_split@0:
   LDA   #$00
   BEQ   .if_end@0

check_encounter@run3:
   SUBROUTINE

   STA   MOB_PTR
   TXA
   AND   #$7F
   STA   MOB_PTR+1
   RTS

find_mob_at_same_pos:
   SUBROUTINE

   LDY   #$03
   LDA   (MOB_PTR),Y
   STA   COLOCATE_POS
   DEY
   LDA   (MOB_PTR),Y
   STA   __local_find_mob_at_same_pos__link
   STA   COLOCATE_LINK
   STY   COLOCATE_WRAPPED
.loop@1_start:
   LDA   __local_find_mob_at_same_pos__link
   BNE   .if_end@6
   LDA   #$01
   STA   COLOCATE_WRAPPED
   LDY   #$02
   LDA   (CHAR_PTR),Y
.if_end@6:
   JSR   get_mob_data
   STA   __local_find_mob_at_same_pos__1
   STX   __local_find_mob_at_same_pos__2
   LDY   #$02
   LDA   (__local_find_mob_at_same_pos__1),Y
   STA   __local_find_mob_at_same_pos__link
   CMP   COLOCATE_LINK
   BNE   .if_end@7
   LDA   #$00
   RTS
.if_end@7:
   LDY   #$03
   LDA   (__local_find_mob_at_same_pos__1),Y
   CMP   COLOCATE_POS
   BNE   .loop@1_start
   LDA   __local_find_mob_at_same_pos__1
   STA   MOB_PTR
   STX   MOB_PTR+1
   LDA   COLOCATE_WRAPPED
   RTS

get_hostility:
   SUBROUTINE

   CMP   #$03
   BCC   .and_false@9
   CMP   #$07
   BEQ   .and_false@9
   LDA   #$01
   RTS
.and_false@9:
   LDA   #$00
   RTS

check_hostile:
   SUBROUTINE

   STA   __local_check_hostile__other
   STX   __local_check_hostile__0
   LDY   #$04
   LDA   (MOB_PTR),Y
   STA   HOSTILE_APP
   LDA   (__local_check_hostile__other),Y
   JSR   appearance_to_fontchar
   STA   HOSTILE_A
   TXA
   JSR   get_hostility
   STA   HOSTILE_Y
   LDA   HOSTILE_APP
   JSR   appearance_to_fontchar
   STA   __local_check_hostile__0
   TXA
   JSR   get_hostility
   CMP   HOSTILE_Y
   BNE   .tail@0
   ORA   #$00
   BEQ   .and_false@12
   LDA   __local_check_hostile__0
   CMP   HOSTILE_A
   BEQ   .and_false@12
.tail@0:
   SEC
   RTS
.and_false@12:
   CLC
   RTS

check_adjacent_threats:
   SUBROUTINE

   STA   __local_check_adjacent_threats__pos
   STA   ADJACENT_POS
   LDY   #$00
   STY   ADJACENT_THREAT
   ORA   #$00
   BEQ   .if_end@14
   SEC
   SBC   #$01
   JSR   sum_hostile_at_pos
.if_end@14:
   LDA   __local_check_adjacent_threats__pos
   CMP   #$C7
   BEQ   .vthread@0
   CLC
   ADC   #$01
   JSR   sum_hostile_at_pos
   LDA   __local_check_adjacent_threats__pos
   CMP   #$14
   BCC   .vthread@1
.vthread@0:
   SEC
   SBC   #$14
   JSR   sum_hostile_at_pos
   LDA   __local_check_adjacent_threats__pos
.vthread@1:
   CMP   #$B4
   BCS   .if_end@17
   ADC   #$14
   JSR   sum_hostile_at_pos
.if_end@17:
   LDY   #$05
   LDA   (ENTITY_PTR),Y
   LSR
   AND   #$0F
   SEC
   SBC   ADJACENT_THREAT
   LDA   #$00
   ROL
   RTS

sum_hostile_at_pos:
   SUBROUTINE

   JSR   find_entity_at_pos
   STA   __local_sum_hostile_at_pos__0
   TXA
   BMI   .if_end@18
   RTS
.if_end@18:
   LDA   __local_sum_hostile_at_pos__0
   JSR   check_encounter@run3
.loop@2_start:
   LDX   ENTITY_PTR+1
   LDA   ENTITY_PTR
   JSR   check_hostile
   BCC   .if_end@21
   LDY   #$0F
   LDA   (MOB_PTR),Y
   AND   #$04
   BNE   .if_end@21
   LDY   #$05
   LDA   (MOB_PTR),Y
   AND   #$1F
   CLC
   ADC   ADJACENT_THREAT
   CMP   #$80
   BCC   .if_end@22
   LDA   #$7F
.if_end@22:
   STA   ADJACENT_THREAT
.if_end@21:
   JSR   find_mob_at_same_pos
   CMP   #$02
   BEQ   .loop@2_start
   RTS

check_threats_here:
   SUBROUTINE

   JSR   check_threats_here@run4
   JSR   colrow_to_pos
   JMP   check_adjacent_threats

check_threats_here@run4:
   SUBROUTINE

   LDX   CURRENT_ROW
   LDA   CURRENT_COL
   RTS

next_colocated_mob:
   SUBROUTINE

   LDY   #$02
   LDA   (MOB_PTR),Y
   STA   SAVED_MOB_LINK
.loop@3_start:
   JSR   find_mob_at_same_pos
   LDY   #$02
   LDA   (MOB_PTR),Y
   CMP   SAVED_MOB_LINK
   BNE   .if_end@23
   LDA   #$00
   RTS
.if_end@23:
   LDY   #$04
   LDA   (MOB_PTR),Y
   CMP   #$15
   BCC   .loop@3_start
   LDA   #$01
   RTS

pick_random_member:
   SUBROUTINE

.loop@4_start:
   LDY   #$08
   LDA   (CHAR_PTR),Y
   BNE   .if_end@24
   STA   MOB_PTR
   LDA   #$00
   STA   MOB_PTR+1
   RTS
.if_end@24:
   JSR   random_in_range
   STA   __local_pick_random_member__n
   ORA   #$00
   BEQ   .loop@4_start
   JSR   first_group_member
.loop@5_start:
   LDY   #$04
   LDA   (MOB_PTR),Y
   CMP   #$15
   BCS   .if_end@27
   DEC   __local_pick_random_member__n
   BNE   .if_end@27
   RTS
.if_end@27:
   JSR   next_group_member
   JMP   .loop@5_start

check_attack_eligible:
   SUBROUTINE

   LDA   ENTITY_PTR
   STA   MOB_PTR
   LDA   ENTITY_PTR+1
   STA   MOB_PTR+1
.loop@6_start:
   JSR   find_mob_at_same_pos
   LDA   MOB_PTR+1
   CMP   ENTITY_PTR+1
   BNE   .if_end@28
   LDA   MOB_PTR
   CMP   ENTITY_PTR
   BNE   .if_end@28
   CLC
   RTS
.if_end@28:
   LDX   ENTITY_PTR+1
   LDA   ENTITY_PTR
   JSR   check_hostile
   BCC   .loop@6_start
   RTS

apply_damage:
   SUBROUTINE

   STA   __local_apply_damage__rec
   STX   __local_apply_damage__1
   LDA   __zpabi_apply_damage__damage
   AND   #$1F
   STA   __local_apply_damage__tune
   STA   DAMAGE_AMOUNT
   JSR   set_cursor_row21
   LDA   #$03
   STA   __local_apply_damage__msg
.loop@7_start:
   INC   __local_apply_damage__msg
   LSR   __local_apply_damage__tune
   BNE   .loop@7_start
   LDA   __local_apply_damage__msg
   JSR   display_message
   LDX   DAMAGE_AMOUNT
   LDA   #$02
   JSR   tune_player
   LDY   #$06
   LDA   (__local_apply_damage__rec),Y
   STA   __local_apply_damage__0
   AND   #$3F
   STA   __local_apply_damage__msg
   LDA   DAMAGE_AMOUNT
   CMP   __local_apply_damage__msg
   BCS   .if_end@30
   LDA   __local_apply_damage__msg
   SEC
   SBC   DAMAGE_AMOUNT
   STA   __local_apply_damage__tune
   STA   DAMAGE_AMOUNT
   LDA   __local_apply_damage__0
   AND   #$C0
   ORA   __local_apply_damage__tune
   STA   (__local_apply_damage__rec),Y
   JSR   set_cursor_row21
   LDA   __local_apply_damage__tune
   CMP   #$03
   BCS   .if_else@32
   LDY   #$0F
   LDA   (__local_apply_damage__rec),Y
   AND   #$F8
   ORA   #$04
   STA   (__local_apply_damage__rec),Y
   LDX   __local_apply_damage__1
   LDA   __local_apply_damage__rec
   JSR   print_entity_name
   LDA   #$09
   JMP   display_message
.if_else@32:
   CMP   #$08
   BCS   .if_else@34
   LDX   __local_apply_damage__1
   LDA   __local_apply_damage__rec
   JSR   print_entity_name
   LDA   #$0A
   JMP   display_message
.if_else@34:
   LDX   __local_apply_damage__1
   LDA   __local_apply_damage__rec
   JSR   print_entity_name
   LDA   #$0B
   JMP   display_message
.if_end@30:
   LDY   #$04
   LDA   (__local_apply_damage__rec),Y
   CMP   #$15
   BCS   .lb_skip@0
   JMP   .apply_damage@asm_ssa_split@1
.lb_skip@0:
   LDY   #$0F
   LDA   (__local_apply_damage__rec),Y
   ORA   #$10
   STA   (__local_apply_damage__rec),Y
   LDA   #$05
   STA   __local_apply_damage__tune
.if_end@35:
   LDX   __local_apply_damage__1
   LDA   __local_apply_damage__rec
   JSR   print_entity_name
   LDA   #$03
   JSR   random_in_range
   ORA   #$00
   BEQ   .if_else@37
   LDA   #$0E
   BNE   .if_end@36
.if_else@37:
   LDA   #$02
   JSR   random_in_range
   STA   __local_apply_damage__msg
   LDA   #$27
   CLC
   ADC   __local_apply_damage__msg
.if_end@36:
   JSR   display_message
   LDX   #$00
   LDA   __local_apply_damage__tune
   JSR   tune_player
   LDA   #$00
   STA   ACTIVE_CHAR
   LDA   CURRENT_PLAYER
   STA   SOURCE_CHAR
   LDX   __local_apply_damage__1
   LDA   __local_apply_damage__rec
   JSR   reorder_char
   LDY   #$0D
   LDA   (__local_apply_damage__rec),Y
   STA   __zpabi_modify_char_stats__amount_0
   INY
   LDA   (__local_apply_damage__rec),Y
   STA   __zpabi_modify_char_stats__amount_1
   LDA   #$00
   STA   __zpabi_modify_char_stats__add
   LDX   __local_apply_damage__1
   LDA   __local_apply_damage__rec
   JSR   modify_char_stats
   LDY   #$03
   LDA   (__local_apply_damage__rec),Y
   STA   __local_apply_damage__msg
   LDA   __zpabi_modify_char_stats__amount_0
   STA   __zpabi_update_event_list__value_0
   LDA   __zpabi_modify_char_stats__amount_1
   STA   __zpabi_update_event_list__value_1
   LDA   __local_apply_damage__msg
   JSR   update_event_list
   LDY   #$03
   LDA   (__local_apply_damage__rec),Y
   JMP   draw_char_at_pos
.apply_damage@asm_ssa_split@1:
   LDA   #$04
   STA   __local_apply_damage__tune
   JMP   .if_end@35

apply_damage_to_current:
   SUBROUTINE

   STA   __zpabi_apply_damage__damage
   LDX   ENTITY_PTR+1
   LDA   ENTITY_PTR
   JMP   apply_damage

apply_damage_to_mob:
   SUBROUTINE

   STA   __zpabi_apply_damage__damage
   LDX   MOB_PTR+1
   LDA   MOB_PTR
   JMP   apply_damage

increment_hp:
   SUBROUTINE

   CLC
   LDY   #$06
   LDA   (ENTITY_PTR),Y
   ADC   #$01
   STA   (ENTITY_PTR),Y
   AND   #$3F
   CMP   #$03
   BNE   .if_end@38
   JSR   increment_hp@run0
   LDA   #$22
   STA   HEAL_NOTIFIED
   JSR   display_message
.if_end@38:
   RTS

increment_hp@run0:
   SUBROUTINE

   JSR   set_cursor_row21
   JSR   print_mob_name
   JMP   scroll_status_line

auto_heal:
   SUBROUTINE

   LDY   #$06
   LDA   (ENTITY_PTR),Y
   AND   #$3F
   CMP   #$28
   BCC   .if_end@39
   RTS
.if_end@39:
   LDA   #$01
   JSR   random_in_range
   ORA   #$00
   BNE   .if_end@40
   RTS
.if_end@40:
   LDA   #$00
   STA   HEAL_NOTIFIED
.loop@8_start:
   JSR   increment_hp
   JSR   step_prng
   SEC
   SBC   #$50
   BPL   .loop@8_start
   LDY   #$04
   LDA   (ENTITY_PTR),Y
   CMP   #$15
   BCS   .if_end@43
   LDA   HEAL_NOTIFIED
   BNE   .if_end@43
   JSR   increment_hp@run0
   LDA   #$21
   JSR   display_message
.if_end@43:
   RTS

enter_combat_state:
   SUBROUTINE

   JSR   check_encounter
   STA   __local_enter_combat_state__pos
   JSR   clear_restricted
   LDA   __local_enter_combat_state__pos
   CMP   #$02
   BNE   .if_end@44
   JSR   set_restricted
.if_end@44:
   LDA   #$02
   JMP   set_char_state

load_target_ptr:
   SUBROUTINE

   LDA   TARGET_PTR
   STA   MOB_PTR
   LDA   TARGET_PTR+1
   STA   MOB_PTR+1
   LDA   #$00
   STA   IS_PLAYER_TURN
   RTS

start_combat:
   SUBROUTINE

   LDA   #$03
   JSR   set_char_state
   JSR   load_target_ptr
   JMP   resolve_attack

attack_right:
   SUBROUTINE

   INC   CURRENT_COL
   BMI   .if_end@45
   JMP   find_attack_target
.if_end@45:
   JMP   attack_left

attack_left:
   SUBROUTINE

   DEC   CURRENT_COL
   BMI   .if_end@46
   JMP   find_attack_target
.if_end@46:
   JMP   attack_up

attack_up:
   SUBROUTINE

   DEC   CURRENT_ROW
   BMI   .if_end@47
   JMP   find_attack_target
.if_end@47:
   JMP   attack_down

attack_down:
   SUBROUTINE

   INC   CURRENT_ROW
   JMP   find_attack_target

find_attack_target:
   SUBROUTINE

   JSR   check_threats_here@run4
   JSR   colrow_to_pos
   JSR   find_entity_at_pos
   STA   __local_find_attack_target__1
   TXA
   BMI   .if_else@49
   LDA   #$01
   STA   IS_PLAYER_TURN
   LDA   __local_find_attack_target__1
   STA   MOB_PTR
   STX   MOB_PTR+1
   JMP   .if_end@48
.if_else@49:
   LDA   #$00
   STA   IS_PLAYER_TURN
   LDA   __local_find_attack_target__1
   JSR   check_encounter@run3
   JSR   find_mob_at_same_pos
   ORA   #$00
   BEQ   .if_end@50
   JSR   next_colocated_mob
   JSR   next_colocated_mob
   ORA   #$00
   BEQ   .if_end@51
   LDA   #$1C
   RTS
.if_end@51:
.if_end@50:
.if_end@48:
   JSR   resolve_attack
   LDA   #$00
   RTS

init_attack_display:
   SUBROUTINE

   LDY   #$00
   LDA   (MOB_PTR),Y
   STA   __local_init_attack_display__2
   INY
   LDA   (MOB_PTR),Y
   STA   __local_init_attack_display__3
   DEY
   LDA   (__local_init_attack_display__2),Y
   STA   __local_init_attack_display__left
   LDA   #$01
   STA   __local_init_attack_display__i
.init_attack_display@rot@0:
   LDY   __local_init_attack_display__i
   LDA   (__local_init_attack_display__2),Y
   DEC   __local_init_attack_display__left
   BPL   .if_end@52
   LDA   #$20
.if_end@52:
   LDX   __local_init_attack_display__i
   STA   S_ATTACK_TARGET+7,X
   INC   __local_init_attack_display__i
   LDA   __local_init_attack_display__i
   CMP   #$15
   BCC   .init_attack_display@rot@0
   RTS

select_someone_else:
   SUBROUTINE

   JSR   next_colocated_mob
   LDA   #$1C
   RTS

resolve_attack:
   SUBROUTINE

   JSR   set_blink_target
   LDA   IS_PLAYER_TURN
   CMP   #$01
   BNE   .if_else@54
   LDA   #$1C
   STA   COMBAT_WILLINGNESS
   JMP   .if_end@53
.if_else@54:
   LDY   #$0B
   LDA   (ENTITY_PTR),Y
   STA   __local_resolve_attack__0
   AND   #$1F
   STA   COMBAT_WILLINGNESS
   LDA   __local_resolve_attack__0
   AND   #$20
   BEQ   .if_end@55
   LDA   __local_resolve_attack__0
   AND   #$EF
   STA   (ENTITY_PTR),Y
   LDA   COMBAT_WILLINGNESS
   CLC
   ADC   #$18
   STA   COMBAT_WILLINGNESS
.if_end@55:
   LDA   (MOB_PTR),Y
   STA   __local_resolve_attack__0
   AND   #$40
   BEQ   .if_end@56
   LDA   COMBAT_WILLINGNESS
   SEC
   SBC   #$04
   STA   COMBAT_WILLINGNESS
.if_end@56:
   LDA   __local_resolve_attack__0
   BPL   .if_end@57
   LDA   COMBAT_WILLINGNESS
   CLC
   ADC   #$08
   STA   COMBAT_WILLINGNESS
.if_end@57:
   LDY   #$0F
   LDA   (MOB_PTR),Y
   STA   __local_resolve_attack__1
   AND   #$04
   BEQ   .if_end@58
   LDA   COMBAT_WILLINGNESS
   CLC
   ADC   #$06
   STA   COMBAT_WILLINGNESS
.if_end@58:
   LDA   __local_resolve_attack__1
   AND   #$03
   BEQ   .case@11
   CMP   #$01
   BEQ   .case@12
   CMP   #$03
   BEQ   .case@13
   JMP   .switch@10_break
.case@11:
   LDA   COMBAT_WILLINGNESS
   CLC
   ADC   #$03
   STA   COMBAT_WILLINGNESS
   JMP   .switch@10_break
.case@12:
   DEC   COMBAT_WILLINGNESS
   JMP   .switch@10_break
.case@13:
   LDA   COMBAT_WILLINGNESS
   SEC
   SBC   #$03
   STA   COMBAT_WILLINGNESS
.switch@10_break:
   JSR   resolve_attack@run6
   BCS   .if_end@59
   LDA   COMBAT_WILLINGNESS
   ADC   #$18
   STA   COMBAT_WILLINGNESS
.if_end@59:
   LDY   #$03
   LDA   (ENTITY_PTR),Y
   CMP   (MOB_PTR),Y
   BNE   .if_end@60
   LDY   #$05
   LDA   (MOB_PTR),Y
   LSR
   AND   #$0F
   STA   TARGET_DEFENSE
   LDA   (ENTITY_PTR),Y
   LSR
   AND   #$0F
   SEC
   SBC   TARGET_DEFENSE
   CLC
   ADC   COMBAT_WILLINGNESS
   STA   COMBAT_WILLINGNESS
.if_end@60:
   LDA   COMBAT_WILLINGNESS
   BPL   .if_else@62
   LDA   #$00
   STA   COMBAT_WILLINGNESS
   JMP   .if_end@61
.if_else@62:
   CMP   #$1F
   BCC   .if_end@63
   LDA   #$1E
   STA   COMBAT_WILLINGNESS
.if_end@63:
.if_end@61:
.if_end@53:
   LDA   #$1F
   JSR   random_in_range
   STA   __local_resolve_attack__0
   LDA   COMBAT_WILLINGNESS
   CMP   __local_resolve_attack__0
   BCC   .if_end@64
   LDA   IS_PLAYER_TURN
   BEQ   .if_else@66
   JMP   player_attack
.if_else@66:
   JMP   mob_attack
.if_end@64:
   JSR   increment_hp@run0
   LDX   #>S_MISSES
   LDA   #<S_MISSES
   JSR   print_bottom_centered
   JSR   scroll_status_line
   LDA   IS_PLAYER_TURN
   CMP   #$01
   BEQ   .if_end@67
   JSR   print_member_name
.if_end@67:
   JMP   delay_with_animation

resolve_attack@run6:
   SUBROUTINE

   LDY   #$06
   LDA   (MOB_PTR),Y
   AND   #$3F
   CMP   #$03
   RTS

calc_combat_strength:
   SUBROUTINE

   LDA   IS_PLAYER_TURN
   CMP   #$01
   BEQ   .calc_combat_strength@scfold@0
   LDY   #$03
   LDA   (MOB_PTR),Y
   CMP   (ENTITY_PTR),Y
   BNE   .calc_combat_strength@scfold@0
   LDA   #$09
   STA   __local_calc_combat_strength__weapon
.if_end@70:
   LDA   ENTITY_PTR
   STA   __local_calc_combat_strength__attacker
   LDA   ENTITY_PTR+1
   STA   __local_calc_combat_strength__0
   LDY   __local_calc_combat_strength__weapon
   LDA   (ENTITY_PTR),Y
   AND   #$0F
   STA   __local_calc_combat_strength__strength
   JSR   random_in_range
   STA   COMBAT_STRENGTH
   LDA   __local_calc_combat_strength__strength
   JSR   random_in_range
   CLC
   ADC   COMBAT_STRENGTH
   CLC
   ADC   #$01
   STA   COMBAT_STRENGTH
   LDA   CHAR_AI_MODE
   BNE   .calc_combat_strength@scfold@1
   LDA   IS_PLAYER_TURN
   BEQ   .if_end@73
.calc_combat_strength@scfold@1:
   RTS
.if_end@73:
   LDY   __local_calc_combat_strength__weapon
   LDA   (__local_calc_combat_strength__attacker),Y
   BPL   .cond_else@74
   LDA   #$50
   BNE   .cond_end@75
.cond_else@74:
   LDA   #$0D
.cond_end@75:
   JSR   random_in_range
   ORA   #$00
   BEQ   .if_end@76
   RTS
.if_end@76:
   LDA   #$81
   LDY   __local_calc_combat_strength__weapon
   STA   (__local_calc_combat_strength__attacker),Y
   JSR   set_cursor_row21
   LDA   #$01
   JSR   scene_loop
   LDA   #$0A
   JSR   random_in_range
   CLC
   ADC   COMBAT_STRENGTH
   CLC
   ADC   #$01
   CMP   #$1F
   BCC   .if_end@77
   LDA   #$1F
.if_end@77:
   STA   COMBAT_STRENGTH
   RTS
.calc_combat_strength@scfold@0:
   LDA   #$07
   STA   __local_calc_combat_strength__weapon
   JMP   .if_end@70

map_value_to_string:
   SUBROUTINE

   STA   __local_map_value_to_string__index
   CMP   #$10
   BCC   .if_else@79
   LSR
   LSR
   AND   #$03
   ORA   #$0C
   STA   __local_map_value_to_string__index
   JMP   .if_end@78
.if_else@79:
   CMP   #$08
   BCC   .if_end@80
   LSR
   AND   #$03
   ORA   #$08
   STA   __local_map_value_to_string__index
.if_end@80:
.if_end@78:
   LDA   #<ATTACK_VERB_STRINGS
   STA   __local_map_value_to_string__verb
   LDY   #>ATTACK_VERB_STRINGS
   STY   __local_map_value_to_string__1
   JMP   .loop@14_continue
.map_value_to_string@rot@1:
   LDY   #$00
   LDA   (__local_map_value_to_string__verb),Y
   CLC
   ADC   __local_map_value_to_string__verb
   BCC   .carry_inc@0
   INC   __local_map_value_to_string__1
   CLC
.carry_inc@0:
   ADC   #$01
   STA   __local_map_value_to_string__verb
   BCC   .loop@14_continue
   INC   __local_map_value_to_string__1
.loop@14_continue:
   DEC   __local_map_value_to_string__index
   BNE   .map_value_to_string@rot@1
   LDX   __local_map_value_to_string__1
   JMP   print_bottom_centered

player_attack:
   SUBROUTINE

   JSR   player_attack@run5
   JSR   map_value_to_string
   JSR   scroll_status_line
   LDA   MOB_PTR
   STA   __local_player_attack__target
   LDA   MOB_PTR+1
   STA   __local_player_attack__3
   LDX   MOB_PTR+1
   LDA   MOB_PTR
   JSR   classify_entity_type
   JSR   print_bottom_centered
   LDX   COMBAT_STRENGTH
   LDA   #$01
   JSR   tune_player
   LDY   #$01
   LDA   (__local_player_attack__target),Y
   CMP   #$FE
   BEQ   .lb_skip@3
   JMP   .if_else@82
.lb_skip@3:
   LDY   #$02
   LDA   (__local_player_attack__target),Y
   STA   __local_player_attack__2
   AND   #$C0
   STA   __local_player_attack__0
   LDA   __local_player_attack__2
   AND   #$1F
   STA   __local_player_attack__1
   LDA   __local_player_attack__0
   CMP   #$C0
   BNE   .if_else@84
   LDA   __local_player_attack__1
   CMP   #$1D
   BCC   .lb_skip@2
   JMP   .if_end@81
.lb_skip@2:
   JSR   process_entity_script
   JSR   set_cursor_row21
   LDA   #$02
   JMP   scene_loop
.if_else@84:
   ORA   #$00
   BNE   .lb_skip@1
   JMP   .if_end@81
.lb_skip@1:
   LDA   COMBAT_STRENGTH
   CMP   #$0A
   BCS   .player_attack@scfold@1
   LDA   __local_player_attack__2
   BMI   .if_end@91
   LDA   COMBAT_STRENGTH
   CMP   #$04
   BCC   .if_end@91
.player_attack@scfold@1:
   LDA   __local_player_attack__1
   CMP   #$1F
   BCS   .if_else@93
   JSR   process_entity_script
   JSR   set_cursor_row21
   LDY   #$02
   LDA   (__local_player_attack__target),Y
   AND   #$1F
   CMP   #$1B
   BCS   .if_end@92
   LDA   #$03
   JSR   scene_loop
   JMP   event_prng_check
.if_else@93:
   LDA   __local_player_attack__2
   SBC   #$01
   STA   (__local_player_attack__target),Y
   LDX   #$00
   LDA   #$07
   JSR   tune_player
   JSR   dispatch_on_mode
   LDY   #$00
   LDA   (__local_player_attack__target),Y
   JSR   draw_char_at_pos
   JSR   set_cursor_row21
.if_end@92:
   LDA   #$12
   JSR   display_message
   JMP   event_prng_check
.if_end@91:
   JSR   dispatch_on_mode
   JSR   set_cursor_row21
   LDA   #$10
   JMP   display_message
.if_else@82:
   CMP   #$40
   BCS   .if_end@97
   LDA   #$0D
   JSR   random_in_range
   CMP   COMBAT_STRENGTH
   BCS   .if_end@97
   JSR   dispatch_on_mode
   LDY   #$01
   LDA   (__local_player_attack__target),Y
   ORA   #$40
   STA   (__local_player_attack__target),Y
   DEY
   LDA   (__local_player_attack__target),Y
   JSR   pos_to_colrow
   STA   FONT_COL
   STX   FONT_ROW
   LDX   __local_player_attack__3
   LDA   __local_player_attack__target
   JSR   get_entity_fontchar
   JSR   render_font_char
   JMP   player_attack@tail0
.if_end@97:
.if_end@81:
   JSR   dispatch_on_mode
   JSR   scroll_status_line
   LDA   #$14
   JMP   display_message

player_attack@run5:
   SUBROUTINE

   JSR   calc_combat_strength
   JSR   increment_hp@run0
   LDA   COMBAT_STRENGTH
   RTS

player_attack@tail0:
   SUBROUTINE

   LDX   #$00
   LDA   #$06
   JSR   tune_player
   JSR   set_cursor_row21
   LDA   #$02
   JMP   display_message

process_entity_script:
   SUBROUTINE

   LDY   #$00
   LDA   (MOB_PTR),Y
   STA   __local_process_entity_script__0
   LDA   #$FE
   STA   (MOB_PTR),Y
   LDX   #$00
   LDA   #$07
   JSR   tune_player
   JSR   dispatch_on_mode
   LDA   __local_process_entity_script__0
   JMP   draw_char_at_pos

mob_attack:
   SUBROUTINE

   JSR   player_attack@run5
   JSR   map_value_to_string
   JSR   scroll_status_line
   JSR   print_member_name
   LDX   COMBAT_STRENGTH
   LDA   #$01
   JSR   tune_player
   LDY   #$04
   LDA   (ENTITY_PTR),Y
   CMP   #$15
   BCS   .if_end@98
   LDA   #$00
   STA   SCENE_COMBAT_FLAG
.if_end@98:
   JSR   dispatch_on_mode
   LDY   #$08
   LDA   (MOB_PTR),Y
   STA   MOB_DEFENSE
   AND   #$0F
   BNE   .if_end@99
   LDA   COMBAT_STRENGTH
   JMP   apply_damage_to_mob
.if_end@99:
   JSR   scroll_status_line
   LDA   MOB_DEFENSE
   BPL   .cond_else@100
   LDA   #$17
   STA   __local_mob_attack__1
   JMP   .cond_end@101
.cond_else@100:
   LDY   #$18
   STY   __local_mob_attack__1
.cond_end@101:
   LDA   MOB_DEFENSE
   AND   #$0F
   STA   MOB_DEFENSE
   LDA   COMBAT_STRENGTH
   SEC
   SBC   MOB_DEFENSE
   BEQ   .if_else@105
   BMI   .if_else@105
   STA   COMBAT_STRENGTH
   LDA   __local_mob_attack__1
   JSR   display_message
   LDA   COMBAT_STRENGTH
   JMP   apply_damage_to_mob
.if_else@105:
   JSR   resolve_attack@run6
   BCS   .if_else@107
   LDA   #$34
   JMP   display_message
.if_else@107:
   LDA   __local_mob_attack__1
   CLC
   ADC   #$02
   JMP   display_message

get_entity_threat:
   SUBROUTINE

   LDX   ENTITY_PTR+1
   LDA   ENTITY_PTR
   JSR   check_hostile
   BCC   .cond_else@110
   LDY   #$03
   LDA   (MOB_PTR),Y
   STA   __local_get_entity_threat__0
   LDA   (ENTITY_PTR),Y
   TAX
   LDA   __local_get_entity_threat__0
   JSR   manhattan_distance
   CMP   #$01
   BNE   .if_end@109
   LDA   #$01
   RTS
.if_end@109:
   ORA   #$00
   BNE   .cond_else@110
   LDA   #$02
   RTS
.cond_else@110:
   LDA   #$00
   RTS

select_combat_target:
   SUBROUTINE

   LDA   CHAR_AI_MODE
   CMP   #$03
   BCC   .if_end@112
   CMP   #$05
   BCC   .if_else@114
   JSR   first_group_member
   LDA   #$00
   STA   BEST_THREAT
.loop@15_start:
   JSR   get_entity_threat
   ORA   #$00
   BEQ   .if_end@115
   LDY   #$05
   LDA   (MOB_PTR),Y
   AND   #$1F
   CMP   BEST_THREAT
   BCC   .if_end@116
   STA   BEST_THREAT
   LDA   MOB_PTR
   STA   TARGET_PTR
   LDA   MOB_PTR+1
   STA   TARGET_PTR+1
.if_end@116:
.if_end@115:
   JSR   next_group_member
   LDA   MOB_PTR+1
   BNE   .loop@15_start
   JMP   .if_end@113
.if_else@114:
   LDA   #$00
   STA   BEST_THREAT
   JSR   first_group_member
.loop@16_start:
   JSR   get_entity_threat
   STA   __local_select_combat_target__0
   LDA   BEST_THREAT
   CMP   __local_select_combat_target__0
   BCS   .if_end@117
   LDA   __local_select_combat_target__0
   STA   BEST_THREAT
   LDA   MOB_PTR
   STA   TARGET_PTR
   LDA   MOB_PTR+1
   STA   TARGET_PTR+1
.if_end@117:
   JSR   next_group_member
   LDA   MOB_PTR+1
   BNE   .loop@16_start
.if_end@113:
   LDA   BEST_THREAT
   BEQ   .if_end@118
   LDA   #$01
   RTS
.if_end@118:
.if_end@112:
   LDA   STEPS_TAKEN
   BNE   .tail@1
   LDY   #$03
   LDA   (ENTITY_PTR),Y
   JSR   check_encounter
   CMP   #$02
   BEQ   .tail@1
   JSR   check_threats_here
   CMP   #$01
   BEQ   .if_end@121
.tail@1:
   LDA   #$02
   RTS
.if_end@121:
   LDA   #$04
   RTS

handle_encounter:
   SUBROUTINE

   STA   __local_handle_encounter__ev
   STX   __local_handle_encounter__0
   LDA   #$00
   JSR   set_char_state
   LDY   #$01
   LDA   (__local_handle_encounter__ev),Y
   CMP   #$40
   BCS   .if_end@122
   LDA   #$14
   JSR   random_in_range
   JSR   handle_encounter@run2
   BCC   .if_end@123
   LDY   #$01
   LDA   (__local_handle_encounter__ev),Y
   ORA   #$40
   STA   (__local_handle_encounter__ev),Y
   JSR   handle_encounter@run1
   LDA   TURN_START_COL
   STA   BLINK_COL
   LDA   TURN_START_ROW
   STA   BLINK_ROW
   LDA   FONT_CHARNUM
   STA   BLINK_CHAR
   JMP   player_attack@tail0
.if_end@123:
   JSR   step_prng
   STA   __local_handle_encounter__dest
   LDA   #$74
   SEC
   SBC   __local_handle_encounter__dest
   BPL   .if_end@124
   JSR   set_cursor_row21
   LDA   #$09
   JSR   scene_loop
   LDA   #$01
   JMP   apply_damage_to_current
.if_end@124:
   JSR   set_cursor_row21
   LDA   #$01
   JSR   display_message
   LDX   #$00
   LDA   #$0A
   JMP   tune_player
.if_end@122:
   LDY   #$00
   LDA   (__local_handle_encounter__ev),Y
   STA   EVENT_POS
   LDA   #$01
   JSR   set_char_state
   LDY   #$01
   LDA   (__local_handle_encounter__ev),Y
   AND   #$3F
   BNE   .if_end@125
   JSR   step_prng
   LSR
   LSR
   AND   #$1C
   ORA   #$01
.if_end@125:
   STA   ACTIVE_CHAR
   LSR
   BCC   .if_end@126
   LDY   #$04
   LDA   (ENTITY_PTR),Y
   CMP   #$2A
   BCC   .if_end@129
   CMP   #$3F
   BCS   .if_end@129
   LDA   #$00
   STA   ACTIVE_CHAR
.if_end@129:
.if_end@126:
   LDY   #$02
   LDA   (__local_handle_encounter__ev),Y
   STA   EVENT_DEST_POS
   JSR   commit_move
   LDA   #$20
   STA   WAIT_LOOP_COUNT
   JSR   reorder_current_char
   JSR   timed_wait
   LDA   EVENT_DEST_POS
   LDY   #$03
   STA   (ENTITY_PTR),Y
   LDA   ACTIVE_CHAR
   CMP   CURRENT_PLAYER
   BEQ   .if_end@130
   JMP   reset_turn_pos
.if_end@130:
   LDA   EVENT_DEST_POS
   JSR   pos_to_colrow
   STA   CURRENT_COL
   STX   CURRENT_ROW
   JMP   commit_move

handle_encounter@run2:
   SUBROUTINE

   STA   PRNG_OUTPUT
   LDY   #$05
   LDA   (ENTITY_PTR),Y
   AND   #$1F
   CMP   PRNG_OUTPUT
   RTS

handle_encounter@run1:
   SUBROUTINE

   LDA   CURRENT_COL
   STA   TURN_START_COL
   LDA   CURRENT_ROW
   STA   TURN_START_ROW
   JMP   reset_turn_pos

show_locked_msg:
   SUBROUTINE

   JSR   increment_hp@run0
   LDA   #$0C
   JSR   display_message
   LDA   #$00
   JMP   set_char_state

handle_event_slot:
   SUBROUTINE

   STA   __local_handle_event_slot__ev
   STX   __local_handle_event_slot__1
   LDA   #$00
   JSR   set_char_state
   LDY   #$02
   LDA   (__local_handle_event_slot__ev),Y
   STA   __local_handle_event_slot__0
   STA   EVENT_SLOT_BYTE
   AND   #$C0
   STA   __local_handle_event_slot__keep
   CMP   #$C0
   BNE   .if_end@131
   LDA   __local_handle_event_slot__0
   AND   #$1F
   STA   __local_handle_event_slot__keep
   CMP   #$1D
   BCS   .if_end@132
   LDA   #$FE
   LDY   #$00
   STA   (__local_handle_event_slot__ev),Y
   LDA   __local_handle_event_slot__keep
   BEQ   .if_end@133
   LDA   #$0A
   JSR   scene_loop
.if_end@133:
.if_end@132:
   JSR   commit_move
   JSR   check_threats_here
   ORA   #$00
   BNE   .if_end@134
   LDA   #$00
   STA   MOVE_POINTS
.if_end@134:
   LDA   #$02
   RTS
.if_end@131:
   LDX   #$00
   LDA   #$07
   JSR   tune_player
   LDA   __local_handle_event_slot__keep
   BNE   .if_end@135
   LDA   EVENT_SLOT_BYTE
   AND   #$1F
   CMP   #$1B
   BCC   .if_end@136
   JSR   show_locked_msg
   LDA   #$00
   RTS
.if_end@136:
   JSR   increment_hp@run0
   LDA   #$0D
   JSR   display_message
   LDA   #$01
   JSR   apply_damage_to_current
   LDA   #$00
   RTS
.if_end@135:
   CMP   #$40
   BEQ   .if_end@137
   LDA   #$19
   JSR   random_in_range
   JSR   handle_encounter@run2
   BCS   .if_end@138
   JSR   set_cursor_row21
   LDA   #$10
   JSR   display_message
   LDA   EVENT_SLOT_BYTE
   AND   #$3F
   ORA   #$40
   LDY   #$02
   STA   (__local_handle_event_slot__ev),Y
   JSR   event_prng_check
   LDA   #$00
   RTS
.if_end@138:
.if_end@137:
   LDA   EVENT_SLOT_BYTE
   AND   #$1F
   STA   EVENT_SUB_VALUE
   JSR   set_cursor_row21
   LDA   EVENT_SUB_VALUE
   CMP   #$1B
   BCS   .if_else@140
   LDA   #$13
   JSR   scene_loop
   JMP   .vthread@3
.if_else@140:
   LDA   #$12
   JSR   display_message
   LDA   EVENT_SUB_VALUE
   CMP   #$1F
   BCC   .vthread@3
   DEC   EVENT_SUB_VALUE
   LDA   EVENT_SLOT_BYTE
   AND   #$E0
   ORA   EVENT_SUB_VALUE
   LDY   #$02
   STA   (__local_handle_event_slot__ev),Y
   LDA   #$01
   BNE   .if_end@142
.vthread@3:
   LDA   #$FE
   LDY   #$00
   STA   (__local_handle_event_slot__ev),Y
.if_end@142:
   JSR   handle_encounter@run1
   JSR   anim_tick_and_wait
   JSR   event_prng_check
   LDA   #$00
   RTS

event_prng_check:
   SUBROUTINE

   JSR   step_prng
   SEC
   SBC   #$14
   BMI   .if_end@143
   RTS
.if_end@143:
   JSR   set_cursor_row21
   LDA   #$11
   JSR   display_message
   JSR   set_restricted
   JSR   print_mob_name
   JSR   delay_with_animation
   JSR   step_prng
   AND   #$40
   BEQ   .cond_else@144
   LDA   #$02
   BNE   .cond_end@145
.cond_else@144:
   LDA   #$01
.cond_end@145:
   JMP   apply_damage_to_current

set_target_restricted:
   SUBROUTINE

   LDY   #$0F
   LDA   (MOB_PTR),Y
   ORA   #$04
   STA   (MOB_PTR),Y
   RTS

handle_mob_encounter:
   SUBROUTINE

   LDA   #$00
   STA   ADJACENT_THREAT
   JSR   check_threats_here@run4
   JSR   colrow_to_pos
   JSR   sum_hostile_at_pos
   LDA   ADJACENT_THREAT
   BEQ   .if_end@146
   LDY   #$05
   LDA   (ENTITY_PTR),Y
   AND   #$1F
   CLC
   ADC   #$10
   SEC
   SBC   ADJACENT_THREAT
   STA   __local_handle_mob_encounter__courage
   BPL   .if_else@148
   LDA   #$04
   STA   __local_handle_mob_encounter__courage
   JMP   .if_end@147
.if_else@148:
   CMP   #$1D
   BCC   .if_end@149
   LDA   #$1C
   STA   __local_handle_mob_encounter__courage
.if_end@149:
.if_end@147:
   ASL   __local_handle_mob_encounter__courage
   ASL   __local_handle_mob_encounter__courage
   LDA   __local_handle_mob_encounter__courage
   ORA   #$03
   STA   __local_handle_mob_encounter__0
   JSR   step_prng
   STA   __local_handle_mob_encounter__courage
   LDA   __local_handle_mob_encounter__0
   SEC
   SBC   __local_handle_mob_encounter__courage
   BPL   .if_end@150
   JSR   increment_hp@run0
   LDA   #$1E
   JSR   display_message
   LDA   TURN_START_COL
   STA   CURRENT_COL
   LDA   TURN_START_ROW
   STA   CURRENT_ROW
   LDA   #$03
   RTS
.if_end@150:
.if_end@146:
   JSR   commit_move
   LDY   #$03
   LDA   (ENTITY_PTR),Y
   JSR   check_encounter
   CMP   #$02
   BCS   .if_end@151
   JSR   check_threats_here
   CMP   #$01
   BNE   .tail@2
   LDA   #$02
   RTS
.if_end@151:
   JSR   first_group_member
   JMP   .loop@17_continue
.handle_mob_encounter@rot@2:
   LDY   #$03
   LDA   (ENTITY_PTR),Y
   CMP   (MOB_PTR),Y
   BNE   .if_end@154
   JSR   set_target_restricted
.if_end@154:
   JSR   next_group_member
.loop@17_continue:
   LDA   MOB_PTR+1
   BNE   .handle_mob_encounter@rot@2
   JSR   increment_hp@run0
   LDA   #$1F
   JSR   display_message
.tail@2:
   LDA   #$03
   RTS

modify_char_stats:
   SUBROUTINE

   STA   __local_modify_char_stats__rec
   STX   __local_modify_char_stats__1
   CLC
   STA   DPTR
   LDY   #$0D
   LDA   (__local_modify_char_stats__rec),Y
   STA   __local_modify_char_stats__0
   INY
   LDA   (__local_modify_char_stats__rec),Y
   STA   __local_modify_char_stats__levels
   LDA   __zpabi_modify_char_stats__add
   BEQ   .if_else@156
   LDA   __local_modify_char_stats__0
   ADC   __zpabi_modify_char_stats__amount_0
   STA   __local_modify_char_stats__gold_0
   LDA   __local_modify_char_stats__levels
   ADC   __zpabi_modify_char_stats__amount_1
   STA   __local_modify_char_stats__gold_1
   JMP   .if_end@155
.if_else@156:
   LDA   __local_modify_char_stats__0
   SEC
   SBC   __zpabi_modify_char_stats__amount_0
   STA   __local_modify_char_stats__gold_0
   LDA   __local_modify_char_stats__levels
   SBC   __zpabi_modify_char_stats__amount_1
   STA   __local_modify_char_stats__gold_1
.if_end@155:
   STX   DPTR+1
   LDA   __local_modify_char_stats__gold_0
   LDY   #$0D
   STA   (__local_modify_char_stats__rec),Y
   LDA   __local_modify_char_stats__gold_1
   INY
   STA   (DPTR),Y
   LDY   #$05
   LDA   (__local_modify_char_stats__rec),Y
   ASL
   ASL
   ASL
   STA   STAT_SCALE
   LDY   #$0C
   LDA   (__local_modify_char_stats__rec),Y
   STA   __local_modify_char_stats__levels
   AND   #$0F
   STA   __local_modify_char_stats__0
   LDA   __local_modify_char_stats__levels
   LSR
   LSR
   LSR
   LSR
   CLC
   ADC   __local_modify_char_stats__0
   STA   __local_modify_char_stats__levels
   LDA   #$00
   STA   STAT_LEVEL_BITS
   STA   __local_modify_char_stats__threshold_0
   STA   __local_modify_char_stats__threshold_1
.loop@18_continue:
   LDA   __local_modify_char_stats__levels
   BEQ   .vthread@2
   LDA   __local_modify_char_stats__threshold_0
   CLC
   ADC   STAT_SCALE
   STA   __local_modify_char_stats__threshold_0
   BCC   .carry_inc@2
   INC   __local_modify_char_stats__threshold_1
.carry_inc@2:
   LDA   __local_modify_char_stats__gold_0
   SEC
   SBC   __local_modify_char_stats__threshold_0
   LDA   __local_modify_char_stats__gold_1
   SBC   __local_modify_char_stats__threshold_1
   BCC   .loop@18_break
   DEC   __local_modify_char_stats__levels
   LDA   STAT_LEVEL_BITS
   CLC
   ADC   #$10
   STA   STAT_LEVEL_BITS
   BNE   .loop@18_continue
.loop@18_break:
   LDA   __local_modify_char_stats__levels
.vthread@2:
   ORA   STAT_LEVEL_BITS
   STA   (__local_modify_char_stats__rec),Y
   RTS

modify_current_stats:
   SUBROUTINE

   STA   __zpabi_modify_char_stats__amount_0
   STX   __zpabi_modify_char_stats__amount_1
   LDX   ENTITY_PTR+1
   LDA   ENTITY_PTR
   JMP   modify_char_stats
