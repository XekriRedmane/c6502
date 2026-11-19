NULL_PAGE_LOW	EQU	$00
TEXT_STREAM_PTR	EQU	$06
RWTS_IOB_PTR	EQU	$08
NULL_PAGE_HIGH	EQU	$0A
HARGS	EQU	$10
SSP	EQU	$11
FP	EQU	$13
DPTR	EQU	$15
TURN_ACTIVE	EQU	$17
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
__zpabi_modify_current_stats__add	EQU	$80
__zpabi_num_to_decimal__out_0	EQU	$80
__zpabi_update_event_list__value_0	EQU	$80
__local_move_loop__0	EQU	$81
__zpabi_num_to_decimal__out_1	EQU	$81
__zpabi_update_event_list__value_1	EQU	$81
__local_move_loop__dir	EQU	$82
__local_move_loop__1	EQU	$83
__local_move_loop__2	EQU	$84
__local_check_and_restrict__0	EQU	$85
__local_set_char_state__state	EQU	$86
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
; def check_and_restrict params= locals=__local_check_and_restrict__0,__local_check_and_restrict__1 indirect=false in_cycle=false
; def clear_restricted params= locals=__local_clear_restricted__0 indirect=false in_cycle=false
; def commit_move params= locals=__local_commit_move__0 indirect=false in_cycle=false
; def end_turn params= locals=__local_end_turn__0 indirect=false in_cycle=false
; def game_turn_loop params= locals=__local_game_turn_loop__0,__local_game_turn_loop__1 indirect=false in_cycle=false
; def move_loop params= locals=__local_move_loop__0,__local_move_loop__dir,__local_move_loop__1,__local_move_loop__2 indirect=false in_cycle=false
; def reset_turn_pos params= locals=__local_reset_turn_pos__0 indirect=false in_cycle=false
; def run_chain params=__zpabi_run_chain__state locals=__local_run_chain__state indirect=false in_cycle=false param_regs=A
; def set_char_state params=__zpabi_set_char_state__state locals=__local_set_char_state__0,__local_set_char_state__state indirect=false in_cycle=false param_regs=A
; def set_restricted params= locals=__local_set_restricted__0 indirect=false in_cycle=false
; def set_state_fight params= locals= indirect=false in_cycle=false
; def turn_dispatch params= locals=__local_turn_dispatch__0 indirect=false in_cycle=false
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
; ext cold_restart params=
; ext colrow_to_pos params=__zpabi_colrow_to_pos__col,__zpabi_colrow_to_pos__row param_regs=A,X
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
; ext resolve_attack params=
; ext retire_character params=
; ext rwts_dispatch params=__zpabi_rwts_dispatch__command param_regs=A carry=true
; ext save_game params=
; ext scene_loop params=__zpabi_scene_loop__scene param_regs=A
; ext scene_setup params=
; ext scroll_status_line params=
; ext select_combat_target params=
; ext select_someone_else params=
; ext set_blink_target params=
; ext set_cursor_row21 params=
; ext set_inverse_video params=
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
; call check_and_restrict -> check_encounter
; call check_and_restrict -> clear_restricted
; call check_and_restrict -> set_restricted
; call commit_move -> colrow_to_pos
; call commit_move -> render_font_char
; call commit_move -> reset_turn_pos
; call end_turn -> check_and_restrict
; call end_turn -> set_state_fight
; call game_turn_loop -> ai_choose_target
; call game_turn_loop -> check_encounter
; call game_turn_loop -> check_threats_here
; call game_turn_loop -> clear_restricted
; call game_turn_loop -> pos_to_colrow
; call game_turn_loop -> random_in_range
; call game_turn_loop -> set_restricted
; call move_loop -> auto_heal
; call move_loop -> check_and_restrict
; call move_loop -> check_can_leave
; call move_loop -> check_threats_here
; call move_loop -> colrow_to_pos
; call move_loop -> commit_move
; call move_loop -> configure_input
; call move_loop -> display_shop
; call move_loop -> enter_combat_state
; call move_loop -> find_entity_at_pos
; call move_loop -> get_input_table_ptr
; call move_loop -> handle_encounter
; call move_loop -> handle_event_slot
; call move_loop -> npc_move_ai
; call move_loop -> pickup_treasure
; call move_loop -> read_keyboard
; call move_loop -> set_char_state
; call move_loop -> set_restricted
; call move_loop -> set_state_fight
; call move_loop -> setup_input
; call move_loop -> show_locked_msg
; call move_loop -> start_combat
; call move_loop -> trigger_scene_event
; call move_loop -> tune_player
; call reset_turn_pos -> colrow_to_pos
; call reset_turn_pos -> draw_char_at_pos
; call reset_turn_pos -> tune_player
; call run_chain -> adj_drop_dec
; call run_chain -> adj_drop_inc
; call run_chain -> attack_down
; call run_chain -> attack_left
; call run_chain -> attack_right
; call run_chain -> attack_up
; call run_chain -> auto_heal
; call run_chain -> browse_handler
; call run_chain -> cancel_scenario
; call run_chain -> char_select_here
; call run_chain -> char_select_home
; call run_chain -> char_select_init
; call run_chain -> char_select_next
; call run_chain -> char_select_prev
; call run_chain -> continue_game
; call run_chain -> drop_gold_exec
; call run_chain -> end_turn
; call run_chain -> enter_char_select
; call run_chain -> enter_combat_state
; call run_chain -> enter_save_game_slot
; call run_chain -> enter_scenario_disk
; call run_chain -> find_attack_target
; call run_chain -> flush_pending_xp
; call run_chain -> format_scenario_disk
; call run_chain -> game_turn_loop
; call run_chain -> handle_mob_encounter
; call run_chain -> init_char_select
; call run_chain -> init_drop_gold
; call run_chain -> init_world
; call run_chain -> menu_attack_target
; call run_chain -> menu_check_attack
; call run_chain -> menu_drop_dec
; call run_chain -> menu_drop_gold
; call run_chain -> menu_drop_inc
; call run_chain -> menu_game_settings
; call run_chain -> menu_game_settings_enter
; call run_chain -> menu_other_options
; call run_chain -> menu_see_your_lot
; call run_chain -> move_loop
; call run_chain -> resolve_attack
; call run_chain -> retire_character
; call run_chain -> save_game
; call run_chain -> select_someone_else
; call run_chain -> set_state_fight
; call run_chain -> settings_danger_down
; call run_chain -> settings_danger_up
; call run_chain -> settings_speed_down
; call run_chain -> settings_speed_up
; call run_chain -> shop_buy_handler
; call run_chain -> slot_dec_enter
; call run_chain -> slot_dec_reenter
; call run_chain -> slot_inc_enter
; call run_chain -> slot_inc_reenter
; call run_chain -> start_game_input
; call run_chain -> teardown_input
; call run_chain -> toggle_sound
; call run_chain -> turn_dispatch
; call run_chain -> use_scenario_disk
; call set_state_fight -> set_char_state
; call turn_dispatch -> auto_heal
; call turn_dispatch -> enter_combat_state
; call turn_dispatch -> select_combat_target
; call turn_dispatch -> start_combat
; item run_chain section=.text local=false zero=false
; item turn_dispatch section=.text local=true zero=false
; item game_turn_loop section=.text local=true zero=false
; item move_loop section=.text local=true zero=false
; item commit_move section=.text local=false zero=false
; item reset_turn_pos section=.text local=false zero=false
; item end_turn section=.text local=true zero=false
; item set_state_fight section=.text local=true zero=false
; item check_and_restrict section=.text local=true zero=false
; item set_char_state section=.text local=false zero=false
; item clear_restricted section=.text local=false zero=false
; item set_restricted section=.text local=false zero=false
; item __zp_init section=.text local=false zero=false
; zp NULL_PAGE_LOW size=6 at=$00 def=false init=
; zp TEXT_STREAM_PTR size=2 at=$06 def=false init=
; zp RWTS_IOB_PTR size=2 at=$08 def=false init=
; zp NULL_PAGE_HIGH size=6 at=$0A def=false init=
; zp TURN_ACTIVE size=1 at=auto def=true init=$4B weight=10
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
; runtime hargs_size=1 hargs_need=0
; @zp-link-meta-end

run_chain:
   SUBROUTINE

.loop@0_continue:
   CMP   #$3B
   BCC   .lb_skip@0
   JMP   .jtdef@0
.lb_skip@0:
   ASL
   TAY
   LDA   .jtab@1+1,Y
   PHA
   LDA   .jtab@1,Y
   PHA
   RTS
.jtab@1:
   DC.W  .loop@0_break-1
   DC.W  .case@2-1
   DC.W  .case@3-1
   DC.W  .case@4-1
   DC.W  .case@5-1
   DC.W  set_state_fight-1
   DC.W  .case@7-1
   DC.W  .case@8-1
   DC.W  auto_heal-1
   DC.W  .case@10-1
   DC.W  .case@11-1
   DC.W  .case@12-1
   DC.W  .case@13-1
   DC.W  .case@14-1
   DC.W  .case@15-1
   DC.W  .case@16-1
   DC.W  resolve_attack-1
   DC.W  .case@18-1
   DC.W  .case@19-1
   DC.W  .case@20-1
   DC.W  .case@21-1
   DC.W  .case@22-1
   DC.W  .case@23-1
   DC.W  .case@24-1
   DC.W  .case@25-1
   DC.W  .case@26-1
   DC.W  .case@27-1
   DC.W  .case@28-1
   DC.W  .case@29-1
   DC.W  .case@30-1
   DC.W  .case@31-1
   DC.W  .case@32-1
   DC.W  .case@33-1
   DC.W  .case@34-1
   DC.W  .case@35-1
   DC.W  .case@36-1
   DC.W  .case@37-1
   DC.W  .case@38-1
   DC.W  .case@39-1
   DC.W  .case@40-1
   DC.W  .case@41-1
   DC.W  .case@42-1
   DC.W  .case@43-1
   DC.W  .case@44-1
   DC.W  .case@45-1
   DC.W  .case@46-1
   DC.W  .case@47-1
   DC.W  .case@48-1
   DC.W  .case@49-1
   DC.W  .case@50-1
   DC.W  .case@51-1
   DC.W  retire_character-1
   DC.W  .case@53-1
   DC.W  .case@54-1
   DC.W  .case@55-1
   DC.W  .case@56-1
   DC.W  .case@57-1
   DC.W  .case@58-1
   DC.W  .case@59-1
.jtdef@0:
   RTS
.case@2:
   JSR   game_turn_loop
   JMP   .loop@0_continue
.case@3:
   JSR   move_loop
   JMP   .loop@0_continue
.case@4:
   JSR   end_turn
   JMP   .loop@0_continue
.case@5:
   JSR   turn_dispatch
   JMP   .loop@0_continue
.case@7:
   JSR   toggle_sound
   JMP   .loop@0_continue
.case@8:
   JSR   init_char_select
   JMP   .loop@0_continue
.case@10:
   LDA   #$14
   JMP   enter_combat_state
.case@11:
   JSR   attack_right
   JMP   .loop@0_continue
.case@12:
   JSR   attack_left
   JMP   .loop@0_continue
.case@13:
   JSR   attack_up
   JMP   .loop@0_continue
.case@14:
   JSR   attack_down
   JMP   .loop@0_continue
.case@15:
   JSR   find_attack_target
   JMP   .loop@0_continue
.case@16:
   JSR   select_someone_else
   JMP   .loop@0_continue
.case@18:
   JSR   handle_mob_encounter
   JMP   .loop@0_continue
.case@19:
   JSR   teardown_input
   JMP   .loop@0_continue
.case@20:
   JSR   menu_see_your_lot
   JMP   .loop@0_continue
.case@21:
   JSR   menu_check_attack
   JMP   .loop@0_continue
.case@22:
   JSR   menu_other_options
   JMP   .loop@0_continue
.case@23:
   JSR   menu_game_settings
   JMP   .loop@0_continue
.case@24:
   JSR   menu_game_settings_enter
   JMP   .loop@0_continue
.case@25:
   JSR   settings_speed_up
   JMP   .loop@0_continue
.case@26:
   JSR   settings_speed_down
   JMP   .loop@0_continue
.case@27:
   JSR   settings_danger_up
   JMP   .loop@0_continue
.case@28:
   JSR   settings_danger_down
   JMP   .loop@0_continue
.case@29:
   JSR   menu_attack_target
   JMP   .loop@0_continue
.case@30:
   JSR   menu_drop_gold
   JMP   .loop@0_continue
.case@31:
   JSR   menu_drop_inc
   JMP   .loop@0_continue
.case@32:
   JSR   menu_drop_dec
   JMP   .loop@0_continue
.case@33:
   JSR   init_world
   JMP   .loop@0_continue
.case@34:
   JSR   slot_inc_enter
   JMP   .loop@0_continue
.case@35:
   JSR   slot_dec_enter
   JMP   .loop@0_continue
.case@36:
   JSR   enter_save_game_slot
   JMP   .loop@0_continue
.case@37:
   JSR   slot_inc_reenter
   JMP   .loop@0_continue
.case@38:
   JSR   slot_dec_reenter
   JMP   .loop@0_continue
.case@39:
   JSR   enter_scenario_disk
   JMP   .loop@0_continue
.case@40:
   JSR   enter_char_select
   JMP   .loop@0_continue
.case@41:
   JSR   char_select_init
   JMP   .loop@0_continue
.case@42:
   JSR   char_select_next
   JMP   .loop@0_continue
.case@43:
   JSR   char_select_prev
   JMP   .loop@0_continue
.case@44:
   JSR   char_select_here
   JMP   .loop@0_continue
.case@45:
   JSR   char_select_home
   JMP   .loop@0_continue
.case@46:
   JSR   start_game_input
   JMP   .loop@0_continue
.case@47:
   JSR   continue_game
   JMP   .loop@0_continue
.case@48:
   JSR   save_game
   JMP   .loop@0_continue
.case@49:
   JSR   cancel_scenario
   JMP   .loop@0_continue
.case@50:
   JSR   format_scenario_disk
   JMP   .loop@0_continue
.case@51:
   JSR   use_scenario_disk
   JMP   .loop@0_continue
.case@53:
   JSR   flush_pending_xp
   JMP   .loop@0_continue
.case@54:
   JSR   init_drop_gold
   JMP   .loop@0_continue
.case@55:
   JSR   adj_drop_inc
   JMP   .loop@0_continue
.case@56:
   JSR   adj_drop_dec
   JMP   .loop@0_continue
.case@57:
   JSR   drop_gold_exec
   JMP   .loop@0_continue
.case@58:
   JSR   shop_buy_handler
   JMP   .loop@0_continue
.case@59:
   JSR   browse_handler
   JMP   .loop@0_continue
.loop@0_break:
   RTS

turn_dispatch:
   SUBROUTINE

   LDA   CHAR_AI_MODE
   BNE   .if_end@0
   LDA   #$12
   RTS
.if_end@0:
   JSR   select_combat_target
   CMP   #$02
   BNE   .if_else@2
   LDA   #$02
   JSR   enter_combat_state
   JMP   .if_end@1
.if_else@2:
   BCS   .if_else@4
   JSR   start_combat
   JMP   .if_end@3
.if_else@4:
   JSR   auto_heal
.if_end@3:
.if_end@1:
   LDA   #$00
   RTS

game_turn_loop:
   SUBROUTINE

   LDA   #$01
   STA   TURN_ACTIVE
   LDY   #$03
   LDA   (ENTITY_PTR),Y
   JSR   pos_to_colrow
   STA   TURN_START_COL
   STA   CURRENT_COL
   TXA
   STA   TURN_START_ROW
   STA   CURRENT_ROW
   LDA   #$00
   STA   STEPS_TAKEN
   JSR   clear_restricted
   LDY   #$06
   LDA   (ENTITY_PTR),Y
   AND   #$3F
   CMP   #$03
   BCS   .if_end@5
   JSR   set_restricted
.if_end@5:
   LDY   #$03
   LDA   (ENTITY_PTR),Y
   JSR   check_encounter
   CMP   #$02
   BNE   .if_end@6
   JSR   set_restricted
.if_end@6:
   LDA   CHAR_AI_MODE
   BEQ   .if_end@7
   JSR   ai_choose_target
.if_end@7:
   LDY   #$0C
   LDA   (ENTITY_PTR),Y
   AND   #$0F
   STA   MOVE_POINTS
   LDA   ENCOUNTER_RESULT
   CMP   #$02
   BNE   .if_else@9
   LDA   MOVE_POINTS
   CMP   #$02
   BCC   .if_end@8
   LDA   #$01
   JSR   random_in_range
   CLC
   ADC   #$01
   STA   MOVE_POINTS
   JMP   .if_end@8
.if_else@9:
   JSR   check_threats_here
   CMP   #$01
   BEQ   .if_end@13
   LDA   MOVE_POINTS
   CMP   #$02
   BCC   .if_end@13
   JSR   random_in_range
   ORA   #$00
   BEQ   .if_else@15
   STA   MOVE_POINTS
   JMP   .if_end@14
.if_else@15:
   LDA   #$01
   JSR   random_in_range
   CLC
   ADC   #$01
   STA   MOVE_POINTS
.if_end@14:
.if_end@13:
.if_end@8:
   LDA   #$02
   RTS

move_loop:
   SUBROUTINE

.loop@61_start:
   LDA   MOVE_POINTS
   BNE   .if_end@20
   LDA   STEPS_TAKEN
   BNE   .move_loop@scfold@0
   LDA   CHAR_AI_MODE
   BEQ   .if_else@22
.move_loop@scfold@0:
   LDA   #$03
   RTS
.if_end@20:
   LDA   CHAR_AI_MODE
   BEQ   .if_else@22
   JSR   npc_move_ai
   STA   __local_move_loop__dir
   STA   INPUT_DIR
   CMP   #$05
   BEQ   .loop@61_start
   CMP   #$09
   BNE   .if_end@24
   JSR   set_restricted
   LDA   #$00
   JSR   set_char_state
   JSR   auto_heal
   LDA   #$00
   RTS
.if_end@24:
   CMP   #$06
   BCC   .if_end@21
   JSR   check_and_restrict
   LDA   __local_move_loop__dir
   CMP   #$07
   BNE   .if_else@27
   LDA   #$07
   JSR   enter_combat_state
   JMP   .tail@0
.if_else@27:
   CMP   #$06
   BNE   .if_else@29
   JSR   start_combat
   JMP   .tail@0
.if_else@29:
   JSR   set_state_fight
   LDA   #$00
   RTS
.if_else@22:
   JSR   configure_input
   ORA   #$00
   BNE   .if_end@30
   LDA   #$12
   RTS
.if_end@30:
   JSR   get_input_table_ptr
   JSR   setup_input
   JSR   read_keyboard
   STA   __local_move_loop__dir
   STA   INPUT_DIR
   ORA   #$00
   BNE   .if_end@31
   LDA   #$12
   RTS
.if_end@31:
.if_end@21:
   LDA   ENCOUNTER_RESULT
   CMP   #$02
   BCC   .if_end@34
   JSR   check_can_leave
   BCS   .if_end@34
   LDA   #$03
   RTS
.if_end@34:
   LDA   #$00
   STA   ENCOUNTER_RESULT
   DEC   MOVE_POINTS
   INC   STEPS_TAKEN
   LDA   __local_move_loop__dir
   CMP   #$02
   BCS   .vthread@0
   DEC   CURRENT_ROW
   JMP   .if_end@35
.vthread@0:
   BNE   .if_else@38
   INC   CURRENT_ROW
   JMP   .if_end@37
.if_else@38:
   CMP   #$03
   BNE   .if_else@40
   DEC   CURRENT_COL
   JMP   .if_end@39
.if_else@40:
   INC   CURRENT_COL
.if_end@39:
.if_end@37:
.if_end@35:
   LDX   CURRENT_ROW
   LDA   CURRENT_COL
   JSR   colrow_to_pos
   JSR   find_entity_at_pos
   STA   __local_move_loop__1
   STX   __local_move_loop__2
   ORA   __local_move_loop__2
   BNE   .if_end@41
   JSR   commit_move
   JSR   check_threats_here
   ORA   #$00
   BEQ   .lb_skip@1
   JMP   .loop@61_start
.lb_skip@1:
   LDA   #$00
   STA   MOVE_POINTS
   STA   TURN_ACTIVE
   JMP   .loop@61_start
.if_end@41:
   TXA
   AND   #$80
   STA   __local_move_loop__0
   BEQ   .if_end@43
   LDA   #$11
   RTS
.if_end@43:
   LDA   #$FF
   SEC
   SBC   __local_move_loop__1
   LDA   #$00
   SBC   __local_move_loop__2
   BCS   .move_loop@scfold@1
   LDA   __local_move_loop__0
   BEQ   .if_end@46
.move_loop@scfold@1:
   LDX   #$00
   LDA   #$07
   JSR   tune_player
   JSR   show_locked_msg
   LDA   #$00
   RTS
.if_end@46:
   LDY   #$01
   LDA   (__local_move_loop__1),Y
   CMP   #$C0
   BCS   .if_end@47
   LDA   __local_move_loop__1
   JSR   handle_encounter
   LDA   #$00
   RTS
.if_end@47:
   CMP   #$D2
   BCS   .if_end@48
   LDA   __local_move_loop__1
   JSR   pickup_treasure
   LDA   #$00
   RTS
.if_end@48:
   CMP   #$D5
   BCS   .if_end@49
   LDA   __local_move_loop__1
   JSR   trigger_scene_event
.tail@0:
   LDA   #$00
   RTS
.if_end@49:
   CMP   #$DB
   BCS   .if_end@50
   JSR   commit_move
   LDX   __local_move_loop__2
   LDA   __local_move_loop__1
   JMP   display_shop
.if_end@50:
   CMP   #$FE
   BNE   .if_end@51
   LDA   __local_move_loop__1
   JMP   handle_event_slot
.if_end@51:
   LDA   #$12
   RTS

commit_move:
   SUBROUTINE

   LDA   CURRENT_COL
   STA   BLINK_COL
   STA   FONT_COL
   LDA   CURRENT_ROW
   STA   BLINK_ROW
   STA   FONT_ROW
   LDX   CURRENT_ROW
   LDA   CURRENT_COL
   JSR   colrow_to_pos
   LDY   #$03
   STA   (ENTITY_PTR),Y
   LDA   CHAR_SPRITE
   STA   BLINK_CHAR
   STA   FONT_CHARNUM
   LDA   #$01
   STA   FONT_CHARSET
   JSR   render_font_char
   LDA   #$16
   STA   TURN_TUNE_GUARD
   JMP   reset_turn_pos

reset_turn_pos:
   SUBROUTINE

   LDX   TURN_START_ROW
   LDA   TURN_START_COL
   JSR   colrow_to_pos
   JSR   draw_char_at_pos
   LDA   CURRENT_COL
   STA   TURN_START_COL
   LDA   CURRENT_ROW
   STA   TURN_START_ROW
   LDA   TURN_TUNE_GUARD
   CMP   #$16
   BNE   .if_end@52
   DEC   TURN_TUNE_GUARD
   LDX   #$00
   LDA   #$0B
   JSR   tune_player
.if_end@52:
   RTS

end_turn:
   SUBROUTINE

   LDA   #$00
   STA   TURN_ACTIVE
   JSR   check_and_restrict
   LDA   STEPS_TAKEN
   BEQ   .tail@1
   LDY   #$0C
   LDA   (ENTITY_PTR),Y
   LSR
   AND   #$07
   SEC
   SBC   STEPS_TAKEN
   BMI   .if_end@54
.tail@1:
   LDA   #$04
   RTS
.if_end@54:
   JSR   set_state_fight
   LDA   #$00
   RTS

set_state_fight:
   SUBROUTINE

   LDA   #$01
   JMP   set_char_state

check_and_restrict:
   SUBROUTINE

   LDY   #$03
   LDA   (ENTITY_PTR),Y
   JSR   check_encounter
   STA   __local_check_and_restrict__0
   JSR   clear_restricted
   LDA   __local_check_and_restrict__0
   CMP   #$02
   BNE   .if_end@55
   JSR   set_restricted
.if_end@55:
   RTS

set_char_state:
   SUBROUTINE

   STA   __local_set_char_state__state
   LDY   #$0F
   LDA   (ENTITY_PTR),Y
   AND   #$FC
   ORA   __local_set_char_state__state
   STA   (ENTITY_PTR),Y
   RTS

clear_restricted:
   SUBROUTINE

   LDY   #$0F
   LDA   (ENTITY_PTR),Y
   AND   #$FB
   STA   (ENTITY_PTR),Y
   RTS

set_restricted:
   SUBROUTINE

   LDY   #$0F
   LDA   (ENTITY_PTR),Y
   ORA   #$04
   STA   (ENTITY_PTR),Y
   RTS

__zp_init:
   SUBROUTINE

   LDA   #$4B
   STA   TURN_ACTIVE
   RTS
