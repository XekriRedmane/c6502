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
__local_setup_scene_display__wait	EQU	$80
__zpabi_modify_current_stats__add	EQU	$80
__zpabi_num_to_decimal__out_0	EQU	$80
__zpabi_update_event_list__value_0	EQU	$80
__local_load_scene_from_disk__scene	EQU	$81
__local_scene_setup__1	EQU	$81
__zpabi_num_to_decimal__out_1	EQU	$81
__zpabi_update_event_list__value_1	EQU	$81
__local_load_scene_from_disk__tries	EQU	$82
__local_scene_setup__2	EQU	$82
__local_load_scene_from_disk__iob	EQU	$83
__local_scene_setup__3	EQU	$83
__local_draw_map_entities__1	EQU	$84
__local_load_scene_from_disk__0	EQU	$84
__local_scene_setup__4	EQU	$84
__local_draw_map_entities__2	EQU	$85
__local_integrity_fail__offset	EQU	$85
__local_draw_map_entities__3	EQU	$86
__local_draw_map_entities__4	EQU	$87
__local_rwts_dispatch__iob	EQU	$87
__local_rwts_dispatch__0	EQU	$88
__local_rwts_dispatch__command	EQU	$89
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
; def check_at_boundary params=__zpabi_check_at_boundary__pos locals=__local_check_at_boundary__pos,__local_check_at_boundary__0 indirect=false in_cycle=false param_regs=A
; def clear_map params= locals=__local_clear_map__0 indirect=false in_cycle=false
; def disk_read params= locals=__local_disk_read__0 indirect=false in_cycle=false carry=true
; def disk_write params= locals=__local_disk_write__0 indirect=false in_cycle=false carry=true
; def draw_map_col params= locals=__local_draw_map_col__0 indirect=false in_cycle=false
; def draw_map_entities params= locals=__local_draw_map_entities__0,__local_draw_map_entities__1,__local_draw_map_entities__2,__local_draw_map_entities__3,__local_draw_map_entities__4 indirect=false in_cycle=false
; def draw_map_icon_b params= locals= indirect=false in_cycle=false
; def enter_location params= locals=__local_enter_location__0,__local_enter_location__1 indirect=false in_cycle=false
; def fill_map params= locals=__local_fill_map__0 indirect=false in_cycle=false
; def integrity_fail params= locals=__local_integrity_fail__0,__local_integrity_fail__1,__local_integrity_fail__2,__local_integrity_fail__offset indirect=false in_cycle=false
; def load_map_tiles params= locals=__local_load_map_tiles__0 indirect=false in_cycle=false
; def load_scene_data params= locals=__local_load_scene_data__0 indirect=false in_cycle=false
; def load_scene_from_disk params=__zpabi_load_scene_from_disk__scene locals=__local_load_scene_from_disk__scene,__local_load_scene_from_disk__tries,__local_load_scene_from_disk__iob,__local_load_scene_from_disk__0,__local_load_scene_from_disk__1 indirect=false in_cycle=false param_regs=A carry=true
; def rwts_dispatch params=__zpabi_rwts_dispatch__command locals=__local_rwts_dispatch__iob,__local_rwts_dispatch__0,__local_rwts_dispatch__command indirect=false in_cycle=false param_regs=A carry=true
; def scene_loop params=__zpabi_scene_loop__scene locals=__local_scene_loop__scene indirect=false in_cycle=false param_regs=A
; def scene_setup params= locals=__local_scene_setup__0,__local_scene_setup__1,__local_scene_setup__2,__local_scene_setup__3,__local_scene_setup__4 indirect=false in_cycle=false
; def setup_scene_display params= locals=__local_setup_scene_display__wait indirect=false in_cycle=false
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
; ext check_attack_eligible params= carry=true
; ext check_can_leave params= carry=true
; ext check_encounter params=__zpabi_check_encounter__pos param_regs=A
; ext check_hostile params=__zpabi_check_hostile__other_0,__zpabi_check_hostile__other_1 param_regs=A,X carry=true
; ext check_scene_events params=
; ext check_threats_here params=
; ext clamp_char_field params=
; ext classify_entity_type params=__zpabi_classify_entity_type__e_0,__zpabi_classify_entity_type__e_1 param_regs=A,X
; ext clear_restricted params=
; ext cold_restart params=
; ext colrow_to_pos params=__zpabi_colrow_to_pos__col,__zpabi_colrow_to_pos__row param_regs=A,X
; ext commit_move params=
; ext configure_input params=
; ext continue_game params=
; ext dec_threshold_wrap params=
; ext delay_with_animation params=
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
; ext enter_save_game_slot params=
; ext enter_scenario_disk params=
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
; ext save_game params=
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
; call check_at_boundary -> pos_to_colrow
; call clear_map -> setup_text_pos
; call disk_read -> rwts_dispatch
; call disk_write -> rwts_dispatch
; call draw_map_col -> colrow_to_pos
; call draw_map_col -> find_entity_at_pos
; call draw_map_col -> get_entity_fontchar
; call draw_map_col -> render_font_char
; call draw_map_entities -> appearance_to_fontchar
; call draw_map_entities -> classify_entity_type
; call draw_map_entities -> colrow_to_pos
; call draw_map_entities -> display_message
; call draw_map_entities -> draw_map_icon_b
; call draw_map_entities -> find_entity_at_pos
; call draw_map_entities -> first_group_member
; call draw_map_entities -> get_entity_fontchar
; call draw_map_entities -> next_group_member
; call draw_map_entities -> pos_to_colrow
; call draw_map_entities -> print_bottom_centered
; call draw_map_entities -> print_member_name
; call draw_map_entities -> render_font_char
; call draw_map_entities -> set_cursor_row21
; call draw_map_icon_b -> anim_tick_and_wait
; call draw_map_icon_b -> draw_blink_alt
; call draw_map_icon_b -> draw_blink_normal
; call draw_map_icon_b -> render_font_char
; call draw_map_icon_b -> scroll_status_line
; call draw_map_icon_b -> timed_wait
; call enter_location -> anim_tick_and_wait
; call enter_location -> clear_map
; call enter_location -> draw_map_col
; call enter_location -> draw_map_entities
; call enter_location -> fill_map
; call enter_location -> find_nearest_event
; call enter_location -> load_map_tiles
; call enter_location -> pos_to_colrow
; call enter_location -> set_text_window_bottom
; call fill_map -> font_pos_to_text_pos
; call fill_map -> print_string
; call fill_map -> render_font_char
; call integrity_fail -> step_prng
; call load_scene_data -> DISK_READ_FIELD
; call load_scene_data -> integrity_fail
; call load_scene_data -> rwts_dispatch
; call load_scene_from_disk -> disk_read
; call rwts_dispatch -> RWTS_ENTRY
; call scene_loop -> display_message
; call scene_loop -> display_status_bar
; call scene_loop -> enter_location
; call scene_loop -> load_scene_data
; call scene_loop -> load_scene_from_disk
; call scene_loop -> set_text_window_bottom
; call scene_loop -> step_prng
; call scene_loop -> text_renderer
; call scene_loop -> tune_player
; call scene_setup -> clamp_char_field
; call scene_setup -> colrow_to_pos
; call scene_setup -> draw_char_at_pos
; call scene_setup -> get_char_record
; call scene_setup -> pos_to_colrow
; call scene_setup -> random_in_range
; call scene_setup -> reorder_char
; call setup_scene_display -> load_char_name_ptr
; call setup_scene_display -> setup_char_sprite
; call setup_scene_display -> timed_wait
; call setup_scene_display -> tune_player
; item load_map_tiles section=.text local=true zero=false
; item fill_map section=.text local=false zero=false
; item clear_map section=.text local=false zero=false
; item draw_map_col section=.text local=true zero=false
; item draw_map_icon_b section=.text local=true zero=false
; item draw_map_entities section=.text local=true zero=false
; item enter_location section=.text local=false zero=false
; item check_at_boundary section=.text local=false zero=false
; item setup_scene_display section=.text local=false zero=false
; item scene_setup section=.text local=false zero=false
; item rwts_dispatch section=.text local=false zero=false
; item disk_read section=.text local=false zero=false
; item disk_write section=.text local=false zero=false
; item integrity_fail section=.text local=false zero=false
; item load_scene_data section=.text local=true zero=false
; item load_scene_from_disk section=.text.scene_disk local=true zero=false
; item scene_loop section=.text local=false zero=false
; item DEFAULT_CHAR section=.data local=false zero=false
; item BLINK_ALT_CHAR section=.bss local=false zero=true
; item MAP_FILL_CHAR section=.data local=true zero=false
; item TILE_FILL_TAB section=.rodata local=true zero=false
; item MAP_REDRAW_FLAG section=.bss local=true zero=true
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
; runtime hargs_size=11 hargs_need=0
; @zp-link-meta-end

load_map_tiles:
   SUBROUTINE

   LDA   LOCATION_STYLE
   LSR
   LSR
   LSR
   LSR
   LSR
   AND   #$03
   TAX
   LDA   #$00
   STA   BLINK_ALT_CHAR
   LDA   TILE_FILL_TAB,X
   STA   MAP_FILL_CHAR
   LDA   #$2C
   STA   DEFAULT_CHAR
   RTS

fill_map:
   SUBROUTINE

   LDA   #$13
   STA   FONT_COL
   LDA   #$09
   STA   FONT_ROW
   LDA   MAP_FILL_CHAR
   STA   FONT_CHARNUM
   LDA   #$01
   STA   FONT_CHARSET
   JSR   render_font_char
.loop@0_start:
   LDA   FONT_COL
   DEC   FONT_COL
   ORA   #$00
   BNE   .if_end@0
   LDA   #$13
   STA   FONT_COL
   LDA   FONT_ROW
   DEC   FONT_ROW
   ORA   #$00
   BNE   .if_end@1
   RTS
.if_end@1:
.if_end@0:
   JSR   font_pos_to_text_pos
   LDX   #>s_PRINT_FONT_CHAR
   LDA   #<s_PRINT_FONT_CHAR
   JSR   print_string
   JMP   .loop@0_start

clear_map:
   SUBROUTINE

   LDY   #$01
   LDA   (CHAR_PTR),Y
   TAX
   DEY
   LDA   (CHAR_PTR),Y
   JMP   setup_text_pos

draw_map_col:
   SUBROUTINE

   LDX   FONT_ROW
   LDA   FONT_COL
   JSR   colrow_to_pos
   JSR   find_entity_at_pos
   JSR   get_entity_fontchar
   JMP   render_font_char

draw_map_icon_b:
   SUBROUTINE

   JSR   scroll_status_line
   JSR   render_font_char
   LDA   FONT_COL
   STA   BLINK_COL
   LDA   FONT_ROW
   STA   BLINK_ROW
   LDA   FONT_CHARNUM
   STA   BLINK_CHAR
   JSR   draw_blink_alt
   LDA   #$2D
   STA   WAIT_LOOP_COUNT
   JSR   timed_wait
   JSR   draw_blink_normal
   JSR   anim_tick_and_wait
   LDA   #$14
   STA   BLINK_COL
   RTS

draw_map_entities:
   SUBROUTINE

   LDY   #$05
   LDA   (CHAR_PTR),Y
   ORA   #$80
   STA   (CHAR_PTR),Y
   LDA   MAP_REDRAW_FLAG
   BEQ   .lb_skip@0
   JMP   .if_end@2
.lb_skip@0:
   LDA   LOCATION_STYLE
   AND   #$60
   BEQ   .if_else@4
   JSR   set_cursor_row21
   LDA   #$0F
   JSR   display_message
   LDA   #$FF
   STA   MAP_REDRAW_FLAG
   JMP   .if_end@3
.if_else@4:
   LDY   #$02
   LDA   (CHAR_PTR),Y
   STA   __local_draw_map_entities__4
   LDA   #$00
   STA   (CHAR_PTR),Y
   LDA   VIEW_MIN_ROW
   STA   FONT_ROW
   JMP   .loop@1_start
.draw_map_entities@rot@1:
   LDA   VIEW_MIN_COL
   STA   FONT_COL
   JMP   .loop@2_start
.draw_map_entities@rot@0:
   LDX   FONT_ROW
   LDA   FONT_COL
   JSR   colrow_to_pos
   JSR   find_entity_at_pos
   STA   __local_draw_map_entities__2
   STX   __local_draw_map_entities__3
   AND   #$FE
   STA   __local_draw_map_entities__1
   TXA
   ORA   __local_draw_map_entities__1
   BEQ   .loop@2_continue
   LDA   __local_draw_map_entities__2
   JSR   get_entity_fontchar
   LDY   #$01
   LDA   (__local_draw_map_entities__2),Y
   CMP   #$FE
   BEQ   .if_end@12
   CMP   #$80
   BCC   .draw_map_entities@scfold@1
   CMP   #$C0
   BCC   .if_end@12
.draw_map_entities@scfold@1:
   JSR   set_cursor_row21
   LDX   __local_draw_map_entities__3
   LDA   __local_draw_map_entities__2
   JSR   classify_entity_type
   JSR   print_bottom_centered
   JSR   draw_map_icon_b
.if_end@12:
.loop@2_continue:
   INC   FONT_COL
.loop@2_start:
   LDA   VIEW_MAX_COL
   CMP   FONT_COL
   BCS   .draw_map_entities@rot@0
   INC   FONT_ROW
.loop@1_start:
   LDA   VIEW_MAX_ROW
   CMP   FONT_ROW
   BCS   .draw_map_entities@rot@1
   LDA   __local_draw_map_entities__4
   LDY   #$02
   STA   (CHAR_PTR),Y
   ORA   #$00
   BNE   .if_end@13
   RTS
.if_end@13:
.if_end@3:
.if_end@2:
   JSR   first_group_member
.loop@3_start:
   LDY   #$03
   LDA   (MOB_PTR),Y
   JSR   pos_to_colrow
   STA   FONT_COL
   STX   FONT_ROW
   LDA   #$01
   STA   FONT_CHARSET
   LDY   #$04
   LDA   (MOB_PTR),Y
   JSR   appearance_to_fontchar
   STA   FONT_CHARNUM
   TXA
   BEQ   .draw_map_entities@scfold@2
   LDY   #$0C
   LDA   (MOB_PTR),Y
   AND   #$0F
   BEQ   .if_else@17
.draw_map_entities@scfold@2:
   JSR   set_cursor_row21
   JSR   print_member_name
   JSR   draw_map_icon_b
   JMP   .if_end@16
.if_else@17:
   JSR   render_font_char
.if_end@16:
   JSR   next_group_member
   LDA   MOB_PTR
   ORA   MOB_PTR+1
   BNE   .loop@3_start
   RTS

enter_location:
   SUBROUTINE

   LDA   CHAR_PTR
   CMP   LOCATION_POS
   BNE   .if_end@18
   RTS
.if_end@18:
   STA   LOCATION_POS
   JSR   anim_tick_and_wait
   JSR   set_text_window_bottom
   JSR   clear_map
   LDA   #<CANDIDATE_POS
   STA   ENTITY_PTR
   LDA   #>CANDIDATE_POS
   STA   ENTITY_PTR+1
   JSR   find_nearest_event
   STA   EVENT_POS
   LDY   #$05
   LDA   (CHAR_PTR),Y
   AND   #$7F
   CMP   LOCATION_STYLE
   BEQ   .if_end@19
   STA   LOCATION_STYLE
   JSR   load_map_tiles
   JSR   fill_map
.if_end@19:
   LDY   #$03
   LDA   (CHAR_PTR),Y
   JSR   pos_to_colrow
   STA   VIEW_MIN_COL
   STX   VIEW_MIN_ROW
   LDY   #$04
   LDA   (CHAR_PTR),Y
   JSR   pos_to_colrow
   STA   VIEW_MAX_COL
   STX   VIEW_MAX_ROW
   LDA   VIEW_MIN_ROW
   STA   FONT_ROW
   JMP   .loop@4_start
.enter_location@rot@3:
   LDA   VIEW_MIN_COL
   STA   FONT_COL
   JMP   .loop@5_start
.enter_location@rot@2:
   JSR   draw_map_col
   INC   FONT_COL
.loop@5_start:
   LDA   VIEW_MAX_COL
   CMP   FONT_COL
   BCS   .enter_location@rot@2
   INC   FONT_ROW
.loop@4_start:
   LDA   VIEW_MAX_ROW
   CMP   FONT_ROW
   BCS   .enter_location@rot@3
   LDY   #$05
   LDA   (CHAR_PTR),Y
   BMI   .if_end@20
   JSR   draw_map_entities
.if_end@20:
   RTS

check_at_boundary:
   SUBROUTINE

   JSR   pos_to_colrow
   CMP   VIEW_MIN_COL
   BEQ   .check_at_boundary@scfold@0
   CMP   VIEW_MAX_COL
   BEQ   .check_at_boundary@scfold@0
   TXA
   CMP   VIEW_MIN_ROW
   BEQ   .check_at_boundary@scfold@0
   CMP   VIEW_MAX_ROW
   BNE   .if_end@27
.check_at_boundary@scfold@0:
   LDX   #$00
   LDA   #$01
   RTS
.if_end@27:
   LDX   #$00
   TXA
   RTS

setup_scene_display:
   SUBROUTINE

   LDA   CHAR_AI_MODE
   BNE   .setup_scene_display@asm_ssa_split@0
   LDX   #$00
   LDA   #$08
   JSR   tune_player
   LDA   #$14
   STA   __local_setup_scene_display__wait
.if_end@28:
   JSR   setup_char_sprite
   JSR   load_char_name_ptr
   LDA   __local_setup_scene_display__wait
   STA   WAIT_LOOP_COUNT
   JMP   timed_wait
.setup_scene_display@asm_ssa_split@0:
   LDA   #$00
   STA   __local_setup_scene_display__wait
   JMP   .if_end@28

scene_setup:
   SUBROUTINE

   LDA   ACTIVE_CHAR
   JSR   get_char_record
   STA   __local_scene_setup__3
   STX   __local_scene_setup__4
   LDY   #$03
   LDA   (__local_scene_setup__3),Y
   JSR   pos_to_colrow
   STA   __local_scene_setup__1
   STX   __local_scene_setup__2
   LDY   #$04
   LDA   (__local_scene_setup__3),Y
   JSR   pos_to_colrow
   STX   __local_scene_setup__3
   INC   __local_scene_setup__1
   INC   __local_scene_setup__2
   SEC
   SBC   __local_scene_setup__1
   SEC
   SBC   #$01
   JSR   random_in_range
   CLC
   ADC   __local_scene_setup__1
   STA   __local_scene_setup__1
   LDA   __local_scene_setup__3
   SEC
   SBC   __local_scene_setup__2
   SEC
   SBC   #$01
   JSR   random_in_range
   CLC
   ADC   __local_scene_setup__2
   TAX
   LDA   __local_scene_setup__1
   JSR   colrow_to_pos
   LDY   #$03
   STA   (MOB_PTR),Y
   LDA   #$00
   STA   SOURCE_CHAR
   LDX   MOB_PTR+1
   LDA   MOB_PTR
   JSR   reorder_char
   LDA   #$00
   LDY   #$0D
   STA   (MOB_PTR),Y
   INY
   STA   (MOB_PTR),Y
   JSR   clamp_char_field
   LDA   CURRENT_PLAYER
   CMP   ACTIVE_CHAR
   BNE   .if_end@29
   LDY   #$03
   LDA   (MOB_PTR),Y
   JSR   draw_char_at_pos
.if_end@29:
   RTS

rwts_dispatch:
   SUBROUTINE

   STA   __local_rwts_dispatch__command
   LDA   RWTS_IOB_PTR
   STA   __local_rwts_dispatch__iob
   LDA   RWTS_IOB_PTR+1
   STA   __local_rwts_dispatch__0
   LDA   __local_rwts_dispatch__command
   LDY   #$0C
   STA   (RWTS_IOB_PTR),Y
   LDA   #$00
   LDY   #$03
   STA   (__local_rwts_dispatch__iob),Y
   LDA   __local_rwts_dispatch__0
   LDY   __local_rwts_dispatch__iob
   JSR   RWTS_ENTRY
   LDA   #$00
   ROL
   LDY   #$00
   STY   DOS_ZP+30
   CMP   #$01
   RTS

disk_read:
   SUBROUTINE

   LDA   #$01
   JMP   rwts_dispatch

disk_write:
   SUBROUTINE

   LDA   #$02
   JMP   rwts_dispatch

integrity_fail:
   SUBROUTINE

   LDA   PRNG_OUTPUT
   STA   __local_integrity_fail__offset
   JSR   step_prng
   LDX   __local_integrity_fail__offset
   STA   GAME_MESSAGE_STRINGS+1,X
   RTS

load_scene_data:
   SUBROUTINE

   LDA   DISK_MOTOR_ON
   LDA   #$05
   LDY   #$04
   STA   (RWTS_IOB_PTR),Y
   LDA   #$00
   JSR   rwts_dispatch
   JSR   DISK_READ_FIELD
   BCC   .if_end@30
   JSR   integrity_fail
.if_end@30:
   RTS

load_scene_from_disk:
   SUBROUTINE

   STA   __local_load_scene_from_disk__scene
   LDA   RWTS_IOB_PTR
   STA   __local_load_scene_from_disk__iob
   LDA   RWTS_IOB_PTR+1
   STA   __local_load_scene_from_disk__0
   LDA   #<SCENE_TEXT_DATA
   LDY   #$08
   STA   (RWTS_IOB_PTR),Y
   LDA   #>SCENE_TEXT_DATA
   INY
   STA   (__local_load_scene_from_disk__iob),Y
   LDA   GAME_SIG_SECTOR
   CLC
   ADC   __local_load_scene_from_disk__scene
   LDX   #$FF
   SEC
.div_loop@0:
   INX
   SBC   #$0D
   BCS   .div_loop@0
   ADC   #$0D
   LDY   #$05
   STA   (__local_load_scene_from_disk__iob),Y
   TXA
   CLC
   ADC   GAME_SIG_TRACK
   DEY
   STA   (__local_load_scene_from_disk__iob),Y
   LDA   #$02
   STA   __local_load_scene_from_disk__tries
.loop@6_continue:
   JSR   disk_read
   BCC   .loop@6_break
   DEC   __local_load_scene_from_disk__tries
   BNE   .loop@6_continue
   RTS
.loop@6_break:
   RTS

scene_loop:
   SUBROUTINE

   STA   SCENE_NUMBER
   LDA   #$14
   STA   BLINK_COL
   LDA   ENTITY_PTR
   STA   SAVED_ENTITY_PTR
   LDA   ENTITY_PTR+1
   STA   SAVED_ENTITY_PTR+1
   LDA   MOB_PTR
   STA   SAVED_MOB_PTR
   LDA   MOB_PTR+1
   STA   SAVED_MOB_PTR+1
   JSR   step_prng
   AND   #$0A
   BNE   .if_end@32
   JSR   load_scene_data
.if_end@32:
.loop@7_start:
   LDA   SCENE_NUMBER
   JSR   load_scene_from_disk
   BCC   .if_end@33
   JSR   set_text_window_bottom
   LDA   #$1B
   JSR   display_message
   JMP   .loop@7_break
.if_end@33:
   JSR   display_status_bar
   LDA   #<SCENE_TEXT_DATA
   STA   TEXT_STREAM_PTR
   LDA   #>SCENE_TEXT_DATA
   STA   TEXT_STREAM_PTR+1
   JSR   text_renderer
   LDX   #$00
   LDA   #$03
   JSR   tune_player
   LDA   SCENE_TEXT_DATA+255
   BEQ   .loop@7_break
   STA   SCENE_NUMBER
   JMP   .loop@7_start
.loop@7_break:
   JSR   set_text_window_bottom
   LDA   #$00
   STA   LOCATION_POS
   STA   LOCATION_STYLE
   JSR   enter_location
   LDA   SAVED_ENTITY_PTR
   STA   ENTITY_PTR
   LDA   SAVED_ENTITY_PTR+1
   STA   ENTITY_PTR+1
   LDA   SAVED_MOB_PTR
   STA   MOB_PTR
   LDA   SAVED_MOB_PTR+1
   STA   MOB_PTR+1
   RTS

DEFAULT_CHAR:
   DC.B  $2C

BLINK_ALT_CHAR:
   DS.B  1

MAP_FILL_CHAR:
   DC.B  $25

TILE_FILL_TAB:
   DC.B  $25, $2A, $20, $29

MAP_REDRAW_FLAG:
   DS.B  1
