NULL_PAGE_LOW	EQU	$00
TEXT_STREAM_PTR	EQU	$06
RWTS_IOB_PTR	EQU	$08
NULL_PAGE_HIGH	EQU	$0A
HARGS	EQU	$10
SSP	EQU	$1A
FP	EQU	$1C
border_handler	EQU	$1E
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
border_pass	EQU	$62
border_anim_index	EQU	$63
border_next	EQU	$64
__local_clear_hires_page1__page	EQU	$80
__local_init_mob_count__0	EQU	$80
__local_move_border_icon_list__first	EQU	$80
__zpabi_anim_border_icons__to	EQU	$80
__zpabi_draw_border_strings__first	EQU	$80
__zpabi_modify_current_stats__add	EQU	$80
__zpabi_num_to_decimal__out_0	EQU	$80
__zpabi_plot_title_glyphs__charset	EQU	$80
__zpabi_print_at__col	EQU	$80
__zpabi_update_event_list__value_0	EQU	$80
__local_clear_hires_page1__0	EQU	$81
__local_init_mob_count__1	EQU	$81
__local_plot_title_glyphs__i	EQU	$81
__zpabi_draw_border_strings__last	EQU	$81
__zpabi_num_to_decimal__out_1	EQU	$81
__zpabi_print_at__row	EQU	$81
__zpabi_update_event_list__value_1	EQU	$81
__local_anim_border_icons__limit	EQU	$82
__local_move_border_icons__carried	EQU	$82
__local_plot_title_glyphs__end	EQU	$82
__local_select_input_mode__2	EQU	$82
__local_anim_border_icons__from	EQU	$83
__local_draw_border_strings__s_1	EQU	$83
__local_move_border_icons__cell	EQU	$83
__local_print_at__s_0	EQU	$83
__local_draw_border_strings__s_0	EQU	$84
__local_move_border_icons__first	EQU	$84
__local_draw_border_cell__cell	EQU	$86
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
; def anim_border_icons params=__zpabi_anim_border_icons__limit,__zpabi_anim_border_icons__from,__zpabi_anim_border_icons__to locals=__local_anim_border_icons__limit,__local_anim_border_icons__from indirect=false in_cycle=false param_regs=A,X,
; def border_anim_step params= locals=__local_border_anim_step__0 indirect=false in_cycle=false
; def clear_hires_page1 params= locals=__local_clear_hires_page1__page,__local_clear_hires_page1__0 indirect=false in_cycle=false
; def draw_border_cell params=__zpabi_draw_border_cell__cell,__zpabi_draw_border_cell__glyph locals=__local_draw_border_cell__glyph,__local_draw_border_cell__cell indirect=false in_cycle=false param_regs=A,X
; def draw_border_labels params= locals= indirect=false in_cycle=false
; def draw_border_strings params=__zpabi_draw_border_strings__s_0,__zpabi_draw_border_strings__s_1,__zpabi_draw_border_strings__first,__zpabi_draw_border_strings__last locals=__local_draw_border_strings__last,__local_draw_border_strings__s_1,__local_draw_border_strings__s_0 indirect=false in_cycle=false param_regs=A,X,, changes=__zpabi_draw_border_strings__last
; def game_init params= locals= indirect=false in_cycle=false
; def init_disk_iob params= locals= indirect=false in_cycle=false
; def init_game_display params= locals= indirect=false in_cycle=false
; def init_game_state params= locals= indirect=false in_cycle=false
; def init_mob_count params= locals=__local_init_mob_count__0,__local_init_mob_count__1 indirect=false in_cycle=false
; def move_border_icon_list params=__zpabi_move_border_icon_list__first locals=__local_move_border_icon_list__first indirect=false in_cycle=false param_regs=A
; def move_border_icons params=__zpabi_move_border_icons__first,__zpabi_move_border_icons__last locals=__local_move_border_icons__carried,__local_move_border_icons__cell,__local_move_border_icons__first indirect=false in_cycle=false param_regs=A,X
; def plot_title_glyphs params=__zpabi_plot_title_glyphs__i,__zpabi_plot_title_glyphs__end,__zpabi_plot_title_glyphs__charset locals=__local_plot_title_glyphs__i,__local_plot_title_glyphs__end indirect=false in_cycle=false param_regs=A,X,
; def print_at params=__zpabi_print_at__s_0,__zpabi_print_at__s_1,__zpabi_print_at__col,__zpabi_print_at__row locals=__local_print_at__s_1,__local_print_at__s_0 indirect=false in_cycle=false param_regs=A,X,,
; def select_input_mode params= locals=__local_select_input_mode__0,__local_select_input_mode__1,__local_select_input_mode__2 indirect=false in_cycle=false
; def start_border_passes params= locals=__local_start_border_passes__0 indirect=false in_cycle=false
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
; ext trigger_scene_event params=__zpabi_trigger_scene_event__ev_0,__zpabi_trigger_scene_event__ev_1 param_regs=A,X
; ext tune_player params=__zpabi_tune_player__tune,__zpabi_tune_player__strength param_regs=A,X
; ext update_event_list params=__zpabi_update_event_list__pos,__zpabi_update_event_list__value_0,__zpabi_update_event_list__value_1 param_regs=A,,
; ext use_scenario_disk params=
; ext verify_disk_io params=
; ext verify_integrity params=
; call anim_border_icons -> draw_border_cell
; call anim_border_icons -> random_in_range
; call border_anim_step -> anim_border_icons
; call border_anim_step -> move_border_icon_list
; call border_anim_step -> move_border_icons
; call border_anim_step -> start_border_passes
; call draw_border_cell -> render_font_char_direct
; call draw_border_labels -> draw_border_strings
; call draw_border_labels -> hrcg_cout
; call draw_border_strings -> print_string
; call game_init -> SETPWRC
; call game_init -> clear_hires_page1
; call game_init -> fill_map
; call game_init -> hrcg_init
; call game_init -> init_disk_iob
; call game_init -> init_game_state
; call game_init -> init_mob_count
; call game_init -> run_chain
; call game_init -> select_input_mode
; call game_init -> set_text_window_bottom
; call game_init -> set_text_window_wrap
; call init_game_display -> draw_border_labels
; call init_game_display -> hrcg_cout
; call init_game_display -> plot_title_glyphs
; call init_game_display -> print_at
; call init_game_display -> print_ctrl_ab
; call init_game_display -> text_renderer
; call init_game_state -> init_game_display
; call init_game_state -> tune_player
; call move_border_icon_list -> move_border_icons
; call move_border_icons -> draw_border_cell
; call plot_title_glyphs -> print_fontchar_at_text_pos
; call print_at -> print_string
; call select_input_mode -> display_message
; call select_input_mode -> set_cursor_row21
; call start_border_passes -> draw_border_cell
; item draw_border_cell section=.text local=true zero=false
; item move_border_icons section=.text local=true zero=false
; item move_border_icon_list section=.text local=true zero=false
; item start_border_passes section=.text local=true zero=false
; item anim_border_icons section=.text local=true zero=false
; item border_anim_step section=.text local=false zero=false
; item plot_title_glyphs section=.text local=true zero=false
; item draw_border_strings section=.text local=true zero=false
; item draw_border_labels section=.text local=true zero=false
; item print_at section=.text local=true zero=false
; item init_game_display section=.text local=true zero=false
; item init_game_display@run0 section=.text local=true zero=false
; item init_game_state section=.text local=true zero=false
; item select_input_mode section=.text local=true zero=false
; item init_mob_count section=.text local=true zero=false
; item init_disk_iob section=.text local=true zero=false
; item clear_hires_page1 section=.text local=true zero=false
; item game_init section=.text local=false zero=false
; item BORDER_ANIM_ON section=.data local=false zero=false
; item border_glyph section=.data local=true zero=false
; item border_row section=.rodata local=true zero=false
; item border_col section=.rodata local=true zero=false
; item UI_TEXT_STREAM section=.rodata local=true zero=false
; item BORDER_LABEL_COL section=.rodata local=true zero=false
; item BORDER_LABEL_ROW section=.rodata local=true zero=false
; item BORDER_STR_HORIZ section=.rodata local=true zero=false
; item BORDER_STR_VERT_L section=.rodata local=true zero=false
; item BORDER_STR_VERT_R section=.rodata local=true zero=false
; item S_COPYRIGHT section=.rodata local=true zero=false
; item S_PRESS_SPACE_BOOT section=.rodata local=true zero=false
; item S_DECOR_ROW1 section=.rodata local=true zero=false
; item S_DECOR_ROW2 section=.rodata local=true zero=false
; item S_DECOR_ROW3 section=.rodata local=true zero=false
; item TITLE_GLYPH section=.rodata local=true zero=false
; item TITLE_GLYPH_COL section=.rodata local=true zero=false
; item TITLE_GLYPH_ROW section=.rodata local=true zero=false
; item __zp_init section=.text local=false zero=false
; zp NULL_PAGE_LOW size=6 at=$00 def=false init=
; zp TEXT_STREAM_PTR size=2 at=$06 def=false init=
; zp RWTS_IOB_PTR size=2 at=$08 def=false init=
; zp NULL_PAGE_HIGH size=6 at=$0A def=false init=
; zp border_handler size=1 at=auto def=true init=$00 weight=7 local=true
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
; zp border_pass size=1 at=auto def=true init=$00 weight=7 local=true
; zp border_anim_index size=1 at=auto def=true init=$06 weight=4 local=true
; zp border_next size=1 at=auto def=true init=$00 weight=3 local=true
; zp MENU_DESC_PTR size=2 at=$C0 def=false init=
; zp MENU_OPTION_PTR size=2 at=$C2 def=false init=
; zp TUNE_PC size=2 at=$E4 def=false init=
; zp MOB_PTR size=2 at=$F4 def=false init=
; zp ENTITY_PTR size=2 at=$F8 def=false init=
; zp CHAR_PTR size=2 at=$FA def=false init=
; runtime hargs_size=10 hargs_need=0
; @zp-link-meta-end

draw_border_cell:
   SUBROUTINE

   STA   __local_draw_border_cell__cell
   STX   FONT_CHARNUM
   LDX   __local_draw_border_cell__cell
   LDA   border_row,X
   STA   TEXT_ROW
   LDA   border_col,X
   STA   TEXT_COL
   LDA   #$00
   JMP   render_font_char_direct

move_border_icons:
   SUBROUTINE

   STA   __local_move_border_icons__first
   STX   __local_move_border_icons__cell
   LDA   #$00
   STA   __local_move_border_icons__carried
.loop@0_start:
   LDA   __local_move_border_icons__carried
   BEQ   .if_else@1
   LDX   __local_move_border_icons__cell
   STA   border_glyph,X
   LDX   __local_move_border_icons__carried
   LDA   __local_move_border_icons__cell
   JSR   draw_border_cell
   LDA   #$00
   STA   __local_move_border_icons__carried
   JMP   .if_end@0
.if_else@1:
   LDX   __local_move_border_icons__cell
   LDA   border_glyph,X
   STA   __local_move_border_icons__carried
   BEQ   .if_end@2
   LDA   #$00
   STA   border_glyph,X
   LDX   #$00
   LDA   __local_move_border_icons__cell
   JSR   draw_border_cell
.if_end@2:
.if_end@0:
   LDA   __local_move_border_icons__cell
   CMP   __local_move_border_icons__first
   BEQ   .loop@0_break
   DEC   __local_move_border_icons__cell
   JMP   .loop@0_start
.loop@0_break:
   RTS

move_border_icon_list:
   SUBROUTINE

   STA   __local_move_border_icon_list__first
   CLC
   ADC   #$08
   TAX
   LDA   __local_move_border_icon_list__first
   JMP   move_border_icons

start_border_passes:
   SUBROUTINE

   LDX   border_anim_index
   LDA   #$03
   STA   border_pass
   STX   border_glyph+9
   LDA   #$09
   JSR   draw_border_cell
   INC   border_anim_index
   LDA   border_anim_index
   CMP   #$16
   BCC   .if_end@4
   LDA   #$05
   STA   border_anim_index
.if_end@4:
   LDA   #$02
   STA   border_handler
   RTS

anim_border_icons:
   SUBROUTINE

   STX   __local_anim_border_icons__from
   JSR   random_in_range
   CMP   #$02
   BCC   .if_end@5
   RTS
.if_end@5:
   ORA   #$00
   BNE   .anim_border_icons@asm_ssa_split@0
   LDA   __zpabi_anim_border_icons__to
   ADC   #$09
   TAY
.if_end@6:
   LDX   __local_anim_border_icons__from
   LDA   border_glyph,X
   STA   __local_anim_border_icons__limit
   STA   border_glyph,Y
   LDA   #$00
   STA   border_glyph,X
   LDX   __local_anim_border_icons__limit
   TYA
   JSR   draw_border_cell
   LDX   #$00
   LDA   __local_anim_border_icons__from
   JMP   draw_border_cell
.anim_border_icons@asm_ssa_split@0:
   LDY   __zpabi_anim_border_icons__to
   JMP   .if_end@6

border_anim_step:
   SUBROUTINE

   LDA   border_next
   BEQ   .case@2
   CMP   #$01
   BEQ   .case@3
   CMP   #$02
   BEQ   .case@4
   JMP   .default@5
.case@2:
   LDA   #$0A
   JSR   move_border_icon_list
   LDA   #$13
   JSR   move_border_icon_list
   JMP   .switch@1_break
.case@3:
   LDX   #$09
   LDA   #$00
   JSR   move_border_icons
   JMP   .switch@1_break
.case@4:
   LDA   #$1C
   JSR   move_border_icon_list
   LDA   #$25
   JSR   move_border_icon_list
   JMP   .switch@1_break
.default@5:
   JSR   start_border_passes
.switch@1_break:
   LDA   border_handler
   BPL   .vthread@0
   LDA   border_pass
   BNE   .if_else@9
   JSR   start_border_passes
   JMP   .if_end@8
.if_else@9:
   DEC   border_pass
   LDA   border_pass
   CMP   #$01
   BNE   .if_else@11
   LDA   #$12
   STA   __zpabi_anim_border_icons__to
   LDX   #$07
   LDA   #$04
   JSR   anim_border_icons
   JMP   .if_end@10
.if_else@11:
   ORA   #$00
   BNE   .if_end@12
   LDA   #$24
   STA   __zpabi_anim_border_icons__to
   LDX   #$02
   TXA
   JSR   anim_border_icons
.if_end@12:
.if_end@10:
   LDA   #$02
   STA   border_handler
.if_end@8:
   LDA   border_handler
.vthread@0:
   STA   border_next
   DEC   border_handler
   RTS

plot_title_glyphs:
   SUBROUTINE

   STA   __local_plot_title_glyphs__i
   STX   __local_plot_title_glyphs__end
.loop@6_start:
   LDX   __local_plot_title_glyphs__i
   LDA   TITLE_GLYPH,X
   STA   FONT_CHARNUM
   LDA   TITLE_GLYPH_COL,X
   STA   TEXT_COL
   LDA   TITLE_GLYPH_ROW,X
   STA   TEXT_ROW
   LDA   __zpabi_plot_title_glyphs__charset
   STA   FONT_CHARSET
   JSR   print_fontchar_at_text_pos
   INC   __local_plot_title_glyphs__i
   LDA   __local_plot_title_glyphs__i
   CMP   __local_plot_title_glyphs__end
   BNE   .loop@6_start
   RTS

draw_border_strings:
   SUBROUTINE

   STA   __local_draw_border_strings__s_0
   STX   __local_draw_border_strings__s_1
   JMP   .loop@7_start
.draw_border_strings@rot@0:
   DEC   __zpabi_draw_border_strings__last
.loop@7_start:
   LDX   __zpabi_draw_border_strings__last
   LDA   BORDER_LABEL_COL,X
   STA   TEXT_COL
   LDA   BORDER_LABEL_ROW,X
   STA   TEXT_ROW
   LDX   __local_draw_border_strings__s_1
   LDA   __local_draw_border_strings__s_0
   JSR   print_string
   LDA   __zpabi_draw_border_strings__last
   CMP   __zpabi_draw_border_strings__first
   BNE   .draw_border_strings@rot@0
   RTS

draw_border_labels:
   SUBROUTINE

   LDA   #$81
   JSR   hrcg_cout
   LDA   #$B2
   JSR   hrcg_cout
   LDA   #$00
   STA   __zpabi_draw_border_strings__first
   LDA   #$05
   STA   __zpabi_draw_border_strings__last
   LDX   #>BORDER_STR_HORIZ
   LDA   #<BORDER_STR_HORIZ
   JSR   draw_border_strings
   LDA   #$06
   STA   __zpabi_draw_border_strings__first
   LDA   #$0F
   STA   __zpabi_draw_border_strings__last
   LDX   #>BORDER_STR_VERT_L
   LDA   #<BORDER_STR_VERT_L
   JSR   draw_border_strings
   LDA   #$10
   STA   __zpabi_draw_border_strings__first
   LDA   #$19
   STA   __zpabi_draw_border_strings__last
   LDX   #>BORDER_STR_VERT_R
   LDA   #<BORDER_STR_VERT_R
   JMP   draw_border_strings

print_at:
   SUBROUTINE

   STA   __local_print_at__s_0
   LDA   __zpabi_print_at__row
   STA   TEXT_ROW
   LDA   __zpabi_print_at__col
   STA   TEXT_COL
   LDA   __local_print_at__s_0
   JMP   print_string

init_game_display:
   SUBROUTINE

   LDA   #$90
   JSR   hrcg_cout
   LDA   #<UI_TEXT_STREAM
   STA   TEXT_STREAM_PTR
   LDA   #>UI_TEXT_STREAM
   STA   TEXT_STREAM_PTR+1
   JSR   text_renderer
   LDA   #$02
   STA   __zpabi_plot_title_glyphs__charset
   LDX   #$03
   LDA   #$00
   JSR   plot_title_glyphs
   JSR   draw_border_labels
   LDA   #$01
   STA   __zpabi_plot_title_glyphs__charset
   LDX   #$09
   LDA   #$03
   JSR   plot_title_glyphs
   JSR   print_ctrl_ab
   LDA   #$00
   STA   __zpabi_print_at__col
   LDA   #$15
   STA   __zpabi_print_at__row
   LDX   #>S_COPYRIGHT
   LDA   #<S_COPYRIGHT
   JSR   print_at
   LDA   #$00
   STA   __zpabi_print_at__col
   LDA   #$17
   STA   __zpabi_print_at__row
   LDX   #>S_PRESS_SPACE_BOOT
   LDA   #<S_PRESS_SPACE_BOOT
   JSR   print_at
   LDA   #$81
   JSR   hrcg_cout
   LDA   #$B5
   JSR   hrcg_cout
   LDA   #$1E
   STA   __zpabi_print_at__col
   LDA   #$15
   STA   __zpabi_print_at__row
   LDX   #>S_DECOR_ROW1
   LDA   #<S_DECOR_ROW1
   JSR   print_at
   JSR   init_game_display@run0
   LDX   #>S_DECOR_ROW2
   LDA   #<S_DECOR_ROW2
   JSR   print_at
   JSR   init_game_display@run0
   LDX   #>S_DECOR_ROW3
   LDA   #<S_DECOR_ROW3
   JMP   print_at

init_game_display@run0:
   SUBROUTINE

   LDA   TEXT_ROW
   CLC
   ADC   #$01
   STA   __zpabi_print_at__row
   LDA   #$1E
   STA   __zpabi_print_at__col
   RTS

init_game_state:
   SUBROUTINE

   JSR   init_game_display
   LDA   #$03
   STA   border_next
   LDA   #$00
   STA   INPUT_MODE
   LDA   #$B1
   STA   s_PRINT_FONT_CHAR+2
   LDX   #$00
   LDA   #$03
   JSR   tune_player
   LDA   #$00
   STA   BORDER_ANIM_ON
   RTS

select_input_mode:
   SUBROUTINE

   LDA   #$38
   JSR   display_message
   LDA   BUTN
   AND   #$80
   STA   __local_select_input_mode__2
.loop@8_start:
   LDA   #$00
   STA   KBDSTRB
.loop@9_continue:
   LDA   KBD
   BMI   .loop@9_break
   LDA   BUTN
   EOR   __local_select_input_mode__2
   BPL   .loop@9_continue
   LDA   #$01
   STA   INPUT_MODE
   JSR   set_cursor_row21
   LDA   #$2F
   JMP   display_message
.loop@9_break:
   CMP   #$A0
   BNE   .loop@8_start
   LDA   #$00
   STA   INPUT_MODE
   RTS

init_mob_count:
   SUBROUTINE

   LDA   #$00
   STA   TOTAL_MOB_COUNT
   LDA   #<(LOCATIONS+9)
   STA   CHAR_PTR
   LDA   #>(LOCATIONS+9)
   STA   CHAR_PTR+1
   JMP   .loop@10_continue
.init_mob_count@rot@1:
   LDY   #$08
   LDA   (CHAR_PTR),Y
   STA   __local_init_mob_count__1
   CLC
   ADC   TOTAL_MOB_COUNT
   STA   __local_init_mob_count__0
   STA   TOTAL_MOB_COUNT
   LDA   CHAR_PTR
   CLC
   ADC   #$09
   STA   CHAR_PTR
   BCC   .carry_inc@0
   INC   CHAR_PTR+1
.carry_inc@0:
   LDA   __local_init_mob_count__0
   CMP   __local_init_mob_count__1
   BCS   .loop@10_continue
   INC   CHAR_PTR
   BNE   .loop@10_continue
   INC   CHAR_PTR+1
.loop@10_continue:
   LDY   #$01
   LDA   (CHAR_PTR),Y
   CMP   #$FF
   BNE   .init_mob_count@rot@1
   RTS

init_disk_iob:
   SUBROUTINE

   LDA   #<RWTS_IOB
   STA   RWTS_IOB_PTR
   LDA   #>RWTS_IOB
   STA   RWTS_IOB_PTR+1
   RTS

clear_hires_page1:
   SUBROUTINE

   LDA   #$00
   STA   __local_clear_hires_page1__page
   LDA   #$20
   STA   __local_clear_hires_page1__0
.loop@11_start:
   LDA   __local_clear_hires_page1__0
   CMP   #$40
   BNE   .jcmp_skip@0
   LDA   __local_clear_hires_page1__page
   BEQ   .loop@11_break
.jcmp_skip@0:
   LDY   #$00
   TYA
.loop@12_start:
   STA   (__local_clear_hires_page1__page),Y
   INY
   BNE   .loop@12_start
   INC   __local_clear_hires_page1__0
   JMP   .loop@11_start
.loop@11_break:
   RTS

game_init:
   SUBROUTINE

   LDA   #$6F
   STA   SOFTEV
   LDA   #$A5
   STA   SOFTEV+1
   JSR   SETPWRC
   LDA   #$14
   STA   BLINK_COL
   JSR   clear_hires_page1
   LDA   TXTCLR
   LDA   MIXCLR
   LDA   TXTPAGE1
   LDA   HIRES
   JSR   init_disk_iob
   JSR   hrcg_init
   JSR   set_text_window_wrap
   JSR   init_game_state
   JSR   fill_map
   JSR   set_text_window_bottom
   LDA   #$00
   STA   LOCATION_POS
   LDY   #$00
   STY   LOCATION_STYLE
   STA   ENTITY_PTR+1
   JSR   select_input_mode
   LDA   #$20
   JSR   run_chain
   JSR   init_mob_count
   LDA   #$01
   STA   CURRENT_PLAYER
   RTS

BORDER_ANIM_ON:
   DC.B  $01

border_glyph:
   DC.B  $00, $00, $00, $00, $00, $0B, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00
   DC.B  $0F, $00, $00, $00, $00, $00, $00, $00, $05, $00, $00, $00, $00, $00, $00, $00
   DC.B  $14, $00, $00, $00, $00, $00, $00, $00, $00, $0E, $00, $00, $00, $00

border_row:
   DC.B  $09, $0A, $0B, $0C, $0D, $0E, $0F, $10, $11, $12, $10, $10, $10, $10, $10, $10
   DC.B  $10, $10, $10, $10, $10, $10, $10, $10, $10, $10, $10, $10, $0B, $0B, $0B, $0B
   DC.B  $0B, $0B, $0B, $0B, $0B, $0B, $0B, $0B, $0B, $0B, $0B, $0B, $0B, $0B

border_col:
   DC.B  $13, $13, $13, $13, $13, $13, $13, $13, $13, $13, $06, $07, $08, $09, $0A, $0B
   DC.B  $0C, $0D, $0E, $20, $1F, $1E, $1D, $1C, $1B, $1A, $19, $18, $06, $07, $08, $09
   DC.B  $0A, $0B, $0C, $0D, $0E, $20, $1F, $1E, $1D, $1C, $1B, $1A, $19, $18

UI_TEXT_STREAM:
   DC.B  $7D, $80, $02, $2F, $00, $00, $01, $0C, $09, $00, $00, $02, $01, $02, $01, $00
   DC.B  $00, $2F, $83, $01, $14, $08, $05, $00, $06, $0F, $12, $14, $19, $00, $14, $08
   DC.B  $09, $05, $16, $05, $13, $7F

BORDER_LABEL_COL:
   DC.B  $01, $01, $01, $16, $16, $16, $01, $01, $01, $01, $01, $01, $16, $16, $16, $16
   DC.B  $10, $10, $10, $10, $25, $25, $25, $25, $25, $25

BORDER_LABEL_ROW:
   DC.B  $09, $0E, $13, $09, $0E, $13, $0A, $0B, $0C, $0D, $0F, $12, $0A, $0D, $0F, $12
   DC.B  $0A, $0D, $0F, $12, $0A, $0D, $0F, $10, $11, $12

BORDER_STR_HORIZ:
   DC.B  $11, $F0, $F1, $F0, $F1, $F0, $F1, $F0, $F1, $F0, $F1, $F0, $F1, $F0, $F1, $F0
   DC.B  $F1, $F0

BORDER_STR_VERT_L:
   DC.B  $02, $F0, $F1

BORDER_STR_VERT_R:
   DC.B  $02, $F1, $F0

S_COPYRIGHT:
   DC.B  $1B, $C3, $CF, $D0, $D9, $D2, $C9, $C7, $C8, $D4, $A0, $B1, $B9, $B8, $B2, $A0
   DC.B  $D3, $D4, $D5, $C1, $D2, $D4, $A0, $D3, $CD, $C9, $D4, $C8

S_PRESS_SPACE_BOOT:
   DC.B  $1B, $D0, $D2, $C5, $D3, $D3, $A0, $D3, $D0, $C1, $C3, $C5, $A0, $C2, $C1, $D2
   DC.B  $A0, $D4, $CF, $A0, $C3, $CF, $CE, $D4, $C9, $CE, $D5, $C5

S_DECOR_ROW1:
   DC.B  $07, $E8, $E9, $EA, $EB, $EC, $ED, $EE

S_DECOR_ROW2:
   DC.B  $08, $EF, $F0, $F1, $F2, $F3, $F4, $F5, $F6

S_DECOR_ROW3:
   DC.B  $08, $F7, $F8, $F9, $FA, $FB, $FC, $FD, $FE

TITLE_GLYPH:
   DC.B  $01, $0E, $04, $02, $01, $03, $04, $1C, $1C

TITLE_GLYPH_COL:
   DC.B  $10, $12, $14, $04, $04, $22, $22, $10, $16

TITLE_GLYPH_ROW:
   DC.B  $03, $03, $03, $0B, $10, $10, $0B, $10, $0B

__zp_init:
   SUBROUTINE

   LDA   #$00
   STA   border_next
   STA   border_handler
   STA   border_pass
   LDA   #$06
   STA   border_anim_index
   RTS
