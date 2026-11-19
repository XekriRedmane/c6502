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
__zpabi_modify_current_stats__add	EQU	$80
__zpabi_num_to_decimal__out_0	EQU	$80
__zpabi_update_event_list__value_0	EQU	$80
__zpabi_num_to_decimal__out_1	EQU	$81
__zpabi_update_event_list__value_1	EQU	$81
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
; def attract_screen params= locals= indirect=false in_cycle=false
; def check_mob_needs_turn params= locals=__local_check_mob_needs_turn__0 indirect=false in_cycle=false carry=true
; def dec_event_threshold params= locals= indirect=false in_cycle=false
; def dec_threshold_wrap params= locals= indirect=false in_cycle=false
; def inc_event_threshold params= locals= indirect=false in_cycle=false
; def inc_threshold_wrap params= locals= indirect=false in_cycle=false
; def main params= locals=__local_main__0 indirect=false in_cycle=false
; def process_mob_groups params= locals=__local_process_mob_groups__0 indirect=false in_cycle=false
; def settings_display_init params= locals=__local_settings_display_init__0 indirect=false in_cycle=false
; def show_idle_status params= locals= indirect=false in_cycle=false
; def start_mob_turn params= locals=__local_start_mob_turn__0,__local_start_mob_turn__1 indirect=false in_cycle=false
; def toggle_sound params= locals= indirect=false in_cycle=false
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
; ext clear_restricted params=
; ext cold_restart params=
; ext colrow_to_pos params=__zpabi_colrow_to_pos__col,__zpabi_colrow_to_pos__row param_regs=A,X
; ext commit_move params=
; ext configure_input params=
; ext continue_game params=
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
; ext init_attack_display params=
; ext init_char_select params=
; ext init_drop_display params=
; ext init_drop_gold params=
; ext init_world params=
; ext input_loop params=__zpabi_input_loop__m_0,__zpabi_input_loop__m_1 param_regs=A,X
; ext integrity_fail params=
; ext load_char_name_ptr params=
; ext load_char_to_fa params=__zpabi_load_char_to_fa__index param_regs=A
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
; ext trigger_scene_event params=__zpabi_trigger_scene_event__ev_0,__zpabi_trigger_scene_event__ev_1 param_regs=A,X
; ext tune_player params=__zpabi_tune_player__tune,__zpabi_tune_player__strength param_regs=A,X
; ext update_event_list params=__zpabi_update_event_list__pos,__zpabi_update_event_list__value_0,__zpabi_update_event_list__value_1 param_regs=A,,
; ext use_scenario_disk params=
; ext verify_disk_io params=
; ext verify_integrity params=
; call attract_screen -> get_mob_data
; call attract_screen -> scene_loop
; call attract_screen -> scene_setup
; call attract_screen -> set_cursor_row21
; call check_mob_needs_turn -> check_adjacent_threats
; call check_mob_needs_turn -> check_encounter
; call check_mob_needs_turn -> clear_restricted
; call dec_threshold_wrap -> dec_event_threshold
; call inc_threshold_wrap -> inc_event_threshold
; call main -> attract_screen
; call main -> check_scene_events
; call main -> enter_location
; call main -> game_init
; call main -> process_mob_groups
; call main -> random_events
; call process_mob_groups -> auto_heal
; call process_mob_groups -> clear_map
; call process_mob_groups -> get_mob_data
; call process_mob_groups -> show_idle_status
; call process_mob_groups -> start_mob_turn
; call process_mob_groups -> step_prng
; call process_mob_groups -> verify_disk_io
; call settings_display_init -> num_to_decimal
; call show_idle_status -> display_message
; call show_idle_status -> print_mob_name
; call show_idle_status -> scroll_status_line
; call start_mob_turn -> appearance_to_fontchar
; call start_mob_turn -> check_mob_needs_turn
; call start_mob_turn -> run_chain
; call start_mob_turn -> set_cursor_row21
; call start_mob_turn -> setup_scene_display
; item main section=.main local=false zero=false
; item show_idle_status section=.text local=true zero=false
; item check_mob_needs_turn section=.text local=true zero=false
; item start_mob_turn section=.text local=true zero=false
; item process_mob_groups section=.text local=true zero=false
; item toggle_sound section=.text local=false zero=false
; item settings_display_init section=.text local=false zero=false
; item dec_event_threshold section=.text local=true zero=false
; item inc_event_threshold section=.text local=true zero=false
; item dec_threshold_wrap section=.text local=false zero=false
; item inc_threshold_wrap section=.text local=false zero=false
; item attract_screen section=.text local=true zero=false
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

main:
   SUBROUTINE

   JSR   game_init
.loop@0_start:
   LDA   #<(LOCATIONS+9)
   STA   CHAR_PTR
   LDA   #>(LOCATIONS+9)
   STA   CHAR_PTR+1
   LDA   TOTAL_MOB_COUNT
   BNE   .if_end@0
   JSR   attract_screen
.if_end@0:
   LDA   #$01
   STA   CURRENT_PLAYER
.loop@1_start:
   LDY   #$08
   LDA   (CHAR_PTR),Y
   BEQ   .if_end@1
   JSR   enter_location
   LDA   CURRENT_PLAYER
   CMP   #$01
   BEQ   .main@scfold@0
   CMP   #$03
   BNE   .if_end@4
.main@scfold@0:
   JSR   check_scene_events
.if_end@4:
   JSR   process_mob_groups
.if_end@1:
   INC   CURRENT_PLAYER
   LDA   CHAR_PTR
   CLC
   ADC   #$09
   STA   CHAR_PTR
   BCC   .carry_inc@0
   INC   CHAR_PTR+1
.carry_inc@0:
   LDY   #$01
   LDA   (CHAR_PTR),Y
   CMP   #$FF
   BNE   .loop@1_start
   JSR   random_events
   JMP   .loop@0_start

show_idle_status:
   SUBROUTINE

   JSR   scroll_status_line
   JSR   print_mob_name
   JSR   scroll_status_line
   LDA   #$20
   JMP   display_message

check_mob_needs_turn:
   SUBROUTINE

   LDA   CHAR_AI_MODE
   BEQ   .tail@0
   LDY   #$0C
   LDA   (ENTITY_PTR),Y
   AND   #$0F
   BNE   .tail@0
   LDY   #$03
   LDA   (ENTITY_PTR),Y
   JSR   check_encounter
   ORA   #$00
   BNE   .tail@0
   LDY   #$03
   LDA   (ENTITY_PTR),Y
   JSR   check_adjacent_threats
   LDA   ADJACENT_THREAT
   BEQ   .if_end@8
.tail@0:
   SEC
   RTS
.if_end@8:
   JSR   clear_restricted
   CLC
   RTS

start_mob_turn:
   SUBROUTINE

   LDY   #$04
   LDA   (ENTITY_PTR),Y
   JSR   appearance_to_fontchar
   STA   CHAR_SPRITE
   STX   CHAR_AI_MODE
   JSR   check_mob_needs_turn
   BCS   .if_end@9
   RTS
.if_end@9:
   JSR   set_cursor_row21
   JSR   setup_scene_display
   LDA   CHAR_AI_MODE
   STA   SAVED_AI_MODE
   LDA   #$01
   JMP   run_chain

process_mob_groups:
   SUBROUTINE

   LDY   #$02
   LDA   (CHAR_PTR),Y
   STA   CURRENT_ROOM
   JSR   clear_map
   JSR   step_prng
   AND   #$17
   BNE   .loop@2_continue
   JSR   verify_disk_io
   JMP   .loop@2_continue
.process_mob_groups@rot@0:
   JSR   get_mob_data
   STA   ENTITY_PTR
   STX   ENTITY_PTR+1
   LDY   #$02
   LDA   (ENTITY_PTR),Y
   STA   CURRENT_ROOM
   LDY   #$06
   LDA   (ENTITY_PTR),Y
   AND   #$3F
   CMP   #$03
   BCS   .if_else@12
   JSR   show_idle_status
   JSR   auto_heal
   JMP   .loop@2_continue
.if_else@12:
   JSR   start_mob_turn
   LDA   #$14
   STA   BLINK_COL
.loop@2_continue:
   LDA   CURRENT_ROOM
   BNE   .process_mob_groups@rot@0
   RTS

toggle_sound:
   SUBROUTINE

   LDA   SOUND_ON
   BNE   .if_else@14
   LDA   #$CE
   STA   S_SOUND_IS_ON+11
   LDA   #$A0
   STA   S_SOUND_IS_ON+12
   LDA   #$01
   STA   SOUND_ON
   JMP   .if_end@13
.if_else@14:
   LDA   #$C6
   STA   S_SOUND_IS_ON+11
   STA   S_SOUND_IS_ON+12
   LDA   #$00
   STA   SOUND_ON
.if_end@13:
   LDA   #$16
   RTS

settings_display_init:
   SUBROUTINE

   LDA   #$20
   STA   S_DANGER+9
   LDY   #$20
   STY   S_DANGER+10
   STA   S_DANGER+11
   LDA   #<(S_DANGER+7)
   STA   __zpabi_num_to_decimal__out_0
   LDA   #>(S_DANGER+7)
   STA   __zpabi_num_to_decimal__out_1
   LDX   #$00
   LDA   EVENT_THRESHOLD
   JMP   num_to_decimal

dec_event_threshold:
   SUBROUTINE

   DEC   EVENT_THRESHOLD
   BPL   .if_end@15
   INC   EVENT_THRESHOLD
.if_end@15:
   RTS

inc_event_threshold:
   SUBROUTINE

   LDA   EVENT_THRESHOLD
   CMP   #$63
   BCS   .if_end@16
   INC   EVENT_THRESHOLD
.if_end@16:
   RTS

dec_threshold_wrap:
   SUBROUTINE

   JMP   dec_event_threshold

inc_threshold_wrap:
   SUBROUTINE

   JMP   inc_event_threshold

attract_screen:
   SUBROUTINE

   LDA   #$01
   STA   ACTIVE_CHAR
   LDY   #$01
   STY   SCENE_EVENT_RESULT
   JSR   get_mob_data
   STA   MOB_PTR
   STX   MOB_PTR+1
   JSR   scene_setup
   LDA   #$00
   STA   LOCATION_POS
   JSR   set_cursor_row21
   LDA   #$26
   JMP   scene_loop
