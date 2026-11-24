PLAYER_COL	EQU	$00
PLAYER_ROW	EQU	$01
PLAYER_X_ADJ	EQU	$02
PLAYER_Y_ADJ	EQU	$03
PLAYER_ANIM_STATE	EQU	$04
PLAYER_FACING_DIRECTION	EQU	$05
HARGS	EQU	$06
SSP	EQU	$07
FP	EQU	$09
DPTR	EQU	$0B
cursor_cell@ret	EQU	$0D
kbd_entry_index	EQU	$0F
saved_game_colnum	EQU	$10
GUARD_LOC_COL	EQU	$12
GUARD_LOC_ROW	EQU	$13
GUARD_ANIM_STATE	EQU	$14
GUARD_FACING_DIRECTION	EQU	$15
GUARD_GOLD_TIMER	EQU	$16
GUARD_X_ADJ	EQU	$17
GUARD_Y_ADJ	EQU	$18
GUARD_NUM	EQU	$19
__dos_zp	EQU	$26
GUARD_RESURRECT_COL	EQU	$53
NOTE_INDEX	EQU	$54
GUARD_GOLD_TIMER_START_VALUE	EQU	$5F
GUARD_PATTERNS	EQU	$60
GUARD_PHASE	EQU	$64
PADDLE_VALUES	EQU	$65
__local_edit_level_key_loop__0	EQU	$80
__local_format_patch__0	EQU	$80
__zpabi_draw_sprite_at_pixel_coords__row	EQU	$80
__zpabi_erase_sprite_at_pixel_coords__row	EQU	$80
__zpabi_play_note__y	EQU	$80
__local_format_patch__b	EQU	$81
__local_editor_initialize_disk__saved_level	EQU	$82
__local_format_patch__tries_1	EQU	$82
__local_get_level_from_keyboard__n	EQU	$82
__local_edit_level_key_loop__3	EQU	$83
__local_format_patch__tries_0	EQU	$83
__local_get_level_from_keyboard__level	EQU	$83
__local_format_patch__slot16	EQU	$84
GAME_COLNUM	EQU	$85
GAME_ROWNUM	EQU	$86
DRAW_PAGE	EQU	$87
DIGITS	EQU	$89
FRAME_PERIOD	EQU	$8C
GUARD_COUNT	EQU	$8D
SCORE	EQU	$8E
GOLD_COUNT	EQU	$93
DIDNT_PICK_UP_GOLD	EQU	$94
INPUT_MODE	EQU	$95
DISK_LEVEL_LOC	EQU	$96
GUARD_PATTERN_OFFSET	EQU	$97
LIVES	EQU	$98
ENABLE_SOUND	EQU	$99
ALIVE	EQU	$9A
NOT_FALLING	EQU	$9B
DIG_DIRECTION	EQU	$9C
HI_SCORE_ELIGIBLE	EQU	$9D
KEY_COMMAND	EQU	$9E
KEY_COMMAND_LR	EQU	$9F
DIG_ANIM_STATE	EQU	$A0
VERBATIM	EQU	$A2
LADDER_COUNT	EQU	$A3
FALL_PITCH	EQU	$A4
WIPE_MODE	EQU	$A5
LEVELNUM	EQU	$A6
GAME_MODE	EQU	$A7
DEMO_PTR	EQU	$A8
DEMO_MOVE	EQU	$AA
DEMO_MOVE_TURNS	EQU	$AB
DEMO_RUNNING	EQU	$AC
LEVEL_SAVED	EQU	$AD
__local_level_edit_save1__col	EQU	$AF
__local_level_edit_save1__row	EQU	$B0
BLOCK_DATA	EQU	$DF
DOS_CURRENT_TRACK	EQU	$0478
LEVEL_MAP	EQU	$0800
LEVEL_BG	EQU	$0A00
LADDER_LOCS_COL	EQU	$0C00
LADDER_LOCS_ROW	EQU	$0C30
GUARD_LOCS_COL	EQU	$0C60
GUARD_LOCS_ROW	EQU	$0C68
GUARD_GOLD_TIMERS	EQU	$0C70
GUARD_X_ADJS	EQU	$0C78
GUARD_Y_ADJS	EQU	$0C80
GUARD_ANIM_STATES	EQU	$0C88
GUARD_FACING_DIRECTIONS	EQU	$0C90
GUARD_RESURRECTION_TIMERS	EQU	$0C98
BRICK_DIG_COLS	EQU	$0CA0
BRICK_DIG_ROWS	EQU	$0CC0
BRICK_FILL_TIMERS	EQU	$0CE0
DISK_BUFFER	EQU	$0D00
SOUND_DURATION	EQU	$0E00
SOUND_PITCH	EQU	$0E80
SPLASH_SCREEN	EQU	$0F00
ROW_TO_OFFSET_LO	EQU	$1A85
ROW_TO_OFFSET_HI	EQU	$1B45
HALF_SCREEN_COL_TABLE	EQU	$1C35
SCREEN_ROW_TABLE	EQU	$1C51
COL_BYTE_TABLE	EQU	$1C62
COL_SHIFT_TABLE	EQU	$1C7E
HALF_SCREEN_COL_BYTE_TABLE	EQU	$1C9A
HALF_SCREEN_COL_SHIFT_TABLE	EQU	$1D26
DISK_BOOT_SECTOR_DATA	EQU	$1DB2
HI_SCORES	EQU	$1F00
HI_SCORE_MARKER	EQU	$1FF4
HI_SCORE_MASTER_FLAG	EQU	$1FFF
HGR1	EQU	$2000
HGR2	EQU	$4000
DEMO_MOVES	EQU	$9B00
DEMO_LEVELS	EQU	$9E00
PIXEL_SHIFT_TABLE	EQU	$A200
SPRITE_DATA	EQU	$AD00
RWTS	EQU	$B7B5
DOS_IOB	EQU	$B7E8
SEEKABS	EQU	$B9A0
DSKFORM	EQU	$BEAF
KBD	EQU	$C000
KBDSTRB	EQU	$C010
SPKR	EQU	$C030
TXTCLR	EQU	$C050
MIXCLR	EQU	$C052
TXTPAGE1	EQU	$C054
TXTPAGE2	EQU	$C055
HIRES	EQU	$C057
BUTN0	EQU	$C061
BUTN1	EQU	$C062
PADDL	EQU	$C064
PTRIG	EQU	$C070
ROMIN_RDROM_WRRAM2	EQU	$C081
DISK_IO	EQU	$C088

; @zp-link-meta-begin
; def bad_data_disk params= locals= indirect=false in_cycle=false
; def check_for_valid_data_disk params= locals=__local_check_for_valid_data_disk__0 indirect=false in_cycle=false
; def cursor_cell params= locals=__local_cursor_cell__0,__local_cursor_cell__1,__local_cursor_cell__2 indirect=false in_cycle=false
; def dont_manipulate_master_disk params= locals= indirect=false in_cycle=false
; def edit_level params= locals= indirect=false in_cycle=false
; def edit_level_key_loop params= locals=__local_edit_level_key_loop__0,__local_edit_level_key_loop__1,__local_edit_level_key_loop__2,__local_edit_level_key_loop__3 indirect=false in_cycle=false
; def editor_clear_high_scores params= locals=__local_editor_clear_high_scores__i indirect=false in_cycle=false
; def editor_clear_level params= locals=__local_editor_clear_level__0 indirect=false in_cycle=false
; def editor_command_loop params= locals=__local_editor_command_loop__0 indirect=false in_cycle=false
; def editor_edit_level params= locals=__local_editor_edit_level__0 indirect=false in_cycle=false
; def editor_initialize_disk params= locals=__local_editor_initialize_disk__0,__local_editor_initialize_disk__saved_level indirect=false in_cycle=false
; def editor_move_level params= locals=__local_editor_move_level__0 indirect=false in_cycle=false
; def editor_play_level params= locals=__local_editor_play_level__0 indirect=false in_cycle=false
; def editor_wait_for_key params= locals=__local_editor_wait_for_key__0 indirect=false in_cycle=false
; def format_patch params=__zpabi_format_patch__slot16 locals=__local_format_patch__0,__local_format_patch__b,__local_format_patch__tries_1,__local_format_patch__tries_0,__local_format_patch__slot16 indirect=false in_cycle=false param_regs=X
; def get_level_from_keyboard params= locals=__local_get_level_from_keyboard__n,__local_get_level_from_keyboard__level indirect=false in_cycle=false carry=true
; def hit_key_to_continue params= locals= indirect=false in_cycle=false
; def level_edit_change_level params= locals=__local_level_edit_change_level__0 indirect=false in_cycle=false
; def level_edit_save1 params= locals=__local_level_edit_save1__0,__local_level_edit_save1__col,__local_level_edit_save1__row indirect=false in_cycle=false
; def level_editor params= locals= indirect=false in_cycle=false
; def start_level_editor params= locals= indirect=false in_cycle=false
; def start_message params= locals= indirect=false in_cycle=false
; ext DSKFORM params=
; ext RWTS params=__zpabi_RWTS__iob_0,__zpabi_RWTS__iob_1 param_regs=Y,A carry=true
; ext SEEKABS params=__zpabi_SEEKABS__track param_regs=A
; ext access_compressed_level_data params=__zpabi_access_compressed_level_data__command param_regs=A
; ext access_disk_or_reset_game params=
; ext access_hi_score_data_from_disk params=__zpabi_access_hi_score_data_from_disk__command param_regs=A
; ext add_and_update_score params=__zpabi_add_and_update_score__bcd_points_0,__zpabi_add_and_update_score__bcd_points_1 param_regs=A,X
; ext append_note params=__zpabi_append_note__pitch,__zpabi_append_note__duration param_regs=A,X
; ext bcd_to_decimal2 params=__zpabi_bcd_to_decimal2__bcd param_regs=A
; ext beep params=
; ext char_to_sprite_num params=__zpabi_char_to_sprite_num__c param_regs=A
; ext check_for_input params=
; ext clear_hgr1 params=
; ext clear_hgr2 params=
; ext cold_start params=
; ext compress_and_save_level_data params=
; ext detect_lack_of_joystick params=
; ext draw_guard_sprite params=__zpabi_draw_guard_sprite__sprite param_regs=A
; ext draw_sprite_at_pixel_coords params=__zpabi_draw_sprite_at_pixel_coords__sprite,__zpabi_draw_sprite_at_pixel_coords__half_col,__zpabi_draw_sprite_at_pixel_coords__row param_regs=A,X,
; ext draw_sprite_page1 params=__zpabi_draw_sprite_page1__sprite param_regs=A
; ext draw_sprite_page2 params=__zpabi_draw_sprite_page2__sprite param_regs=A
; ext enable_next_level_ladders params=
; ext erase_guard params=
; ext erase_sprite_at_pixel_coords params=__zpabi_erase_sprite_at_pixel_coords__sprite,__zpabi_erase_sprite_at_pixel_coords__half_col,__zpabi_erase_sprite_at_pixel_coords__row param_regs=A,X,
; ext get_key_for_edit_level params=
; ext half_screen_col_for params=__zpabi_half_screen_col_for__col,__zpabi_half_screen_col_for__x_adj param_regs=A,X
; ext handle_timers params=
; ext hi_score_screen params=
; ext init_game_data params=
; ext iris_wipe params=
; ext load_guard_data params=
; ext load_level params=__zpabi_load_level__verbatim param_regs=A
; ext load_sound_data params=__zpabi_load_sound_data__notes_0,__zpabi_load_sound_data__notes_1 param_regs=A,X
; ext long_delay params=
; ext move_guards params=
; ext move_player params=
; ext newline params=
; ext play_note params=__zpabi_play_note__pitch,__zpabi_play_note__duration,__zpabi_play_note__y param_regs=A,X,
; ext play_sound params= carry=true
; ext put_char params=__zpabi_put_char__c param_regs=A
; ext put_digit params=__zpabi_put_digit__digit param_regs=A
; ext put_sprite_at_cursor params=__zpabi_put_sprite_at_cursor__sprite param_regs=A
; ext put_status params=
; ext put_status_draw params=
; ext put_status_level params=
; ext put_status_lives params=
; ext put_string params=__zpabi_put_string__s_0,__zpabi_put_string__s_1 param_regs=A,X
; ext record_hi_score_data_to_disk params=
; ext reset_game params=
; ext row_to_addr params=__zpabi_row_to_addr__row,__zpabi_row_to_addr__page param_regs=A,X
; ext screen_row_for params=__zpabi_screen_row_for__row,__zpabi_screen_row_for__y_adj param_regs=A,X
; ext sound_delay params=__zpabi_sound_delay__index param_regs=A
; ext sound_delay1 params=__zpabi_sound_delay1__count param_regs=A
; ext spinning_game_over params= carry=true
; ext to_decimal3 params=__zpabi_to_decimal3__n param_regs=A
; ext wait_for_key params=__zpabi_wait_for_key__cursor param_regs=A
; ext wait_for_key_with_cursor_page_1 params=__zpabi_wait_for_key_with_cursor_page_1__cursor param_regs=A
; ext wait_key params=
; call bad_data_disk -> hit_key_to_continue
; call bad_data_disk -> put_string
; call bad_data_disk -> start_message
; call check_for_valid_data_disk -> access_hi_score_data_from_disk
; call check_for_valid_data_disk -> bad_data_disk
; call check_for_valid_data_disk -> dont_manipulate_master_disk
; call dont_manipulate_master_disk -> hit_key_to_continue
; call dont_manipulate_master_disk -> put_string
; call dont_manipulate_master_disk -> start_message
; call edit_level -> check_for_valid_data_disk
; call edit_level -> clear_hgr2
; call edit_level -> load_level
; call edit_level -> put_status_draw
; call edit_level_key_loop -> beep
; call edit_level_key_loop -> cursor_cell
; call edit_level_key_loop -> draw_sprite_page1
; call edit_level_key_loop -> level_edit_change_level
; call edit_level_key_loop -> level_edit_save1
; call edit_level_key_loop -> wait_for_key_with_cursor_page_1
; call editor_clear_high_scores -> access_hi_score_data_from_disk
; call editor_clear_high_scores -> bad_data_disk
; call editor_clear_high_scores -> editor_wait_for_key
; call editor_clear_high_scores -> put_string
; call editor_clear_level -> access_compressed_level_data
; call editor_clear_level -> beep
; call editor_clear_level -> check_for_valid_data_disk
; call editor_clear_level -> get_level_from_keyboard
; call editor_clear_level -> put_string
; call editor_command_loop -> beep
; call editor_command_loop -> editor_clear_high_scores
; call editor_command_loop -> editor_clear_level
; call editor_command_loop -> editor_edit_level
; call editor_command_loop -> editor_initialize_disk
; call editor_command_loop -> editor_move_level
; call editor_command_loop -> editor_play_level
; call editor_command_loop -> editor_wait_for_key
; call editor_command_loop -> put_string
; call editor_edit_level -> beep
; call editor_edit_level -> get_level_from_keyboard
; call editor_edit_level -> put_string
; call editor_initialize_disk -> access_compressed_level_data
; call editor_initialize_disk -> access_disk_or_reset_game
; call editor_initialize_disk -> access_hi_score_data_from_disk
; call editor_initialize_disk -> editor_wait_for_key
; call editor_initialize_disk -> put_string
; call editor_move_level -> access_compressed_level_data
; call editor_move_level -> beep
; call editor_move_level -> check_for_valid_data_disk
; call editor_move_level -> editor_wait_for_key
; call editor_move_level -> get_level_from_keyboard
; call editor_move_level -> put_string
; call editor_play_level -> beep
; call editor_play_level -> get_level_from_keyboard
; call editor_play_level -> put_string
; call editor_wait_for_key -> wait_for_key_with_cursor_page_1
; call format_patch -> SEEKABS
; call format_patch -> dont_manipulate_master_disk
; call get_level_from_keyboard -> beep
; call get_level_from_keyboard -> put_digit
; call get_level_from_keyboard -> to_decimal3
; call get_level_from_keyboard -> wait_for_key_with_cursor_page_1
; call hit_key_to_continue -> beep
; call hit_key_to_continue -> put_string
; call hit_key_to_continue -> wait_for_key
; call level_edit_change_level -> beep
; call level_edit_change_level -> level_edit_save1
; call level_edit_change_level -> put_string
; call level_edit_change_level -> start_message
; call level_edit_change_level -> wait_for_key
; call level_edit_save1 -> access_hi_score_data_from_disk
; call level_edit_save1 -> bad_data_disk
; call level_edit_save1 -> compress_and_save_level_data
; call level_edit_save1 -> dont_manipulate_master_disk
; call start_level_editor -> clear_hgr1
; call start_level_editor -> put_string
; call start_message -> clear_hgr2
; item editor_wait_for_key section=.text local=true zero=false
; item start_message section=.text local=true zero=false
; item start_message@run0 section=.text local=true zero=false
; item hit_key_to_continue section=.text local=true zero=false
; item hit_key_to_continue@run1 section=.text local=true zero=false
; item dont_manipulate_master_disk section=.text local=true zero=false
; item bad_data_disk section=.text local=false zero=false
; item check_for_valid_data_disk section=.text local=true zero=false
; item get_level_from_keyboard section=.text local=true zero=false
; item level_editor section=.text local=false zero=false
; item start_level_editor section=.text local=false zero=false
; item editor_play_level section=.text local=true zero=false
; item editor_clear_level section=.text local=true zero=false
; item editor_edit_level section=.text local=true zero=false
; item editor_move_level section=.text local=true zero=false
; item editor_initialize_disk section=.text local=true zero=false
; item editor_clear_high_scores section=.text local=true zero=false
; item editor_command_loop section=.text local=false zero=false
; item edit_level section=.text local=true zero=false
; item edit_level@tail0 section=.text local=true zero=false
; item level_edit_save1 section=.text local=true zero=false
; item level_edit_change_level section=.text local=true zero=false
; item cursor_cell section=.text local=true zero=false
; item edit_level_key_loop section=.text local=true zero=false
; item format_patch section=format_patch local=false zero=false
; item str@0 section=.rodata local=true zero=false
; item str@1 section=.rodata local=true zero=false
; item str@2 section=.rodata local=true zero=false
; item str@3 section=.rodata local=true zero=false
; item str@4 section=.rodata local=true zero=false
; item str@5 section=.rodata local=true zero=false
; item str@6 section=.rodata local=true zero=false
; item str@7 section=.rodata local=true zero=false
; item str@8 section=.rodata local=true zero=false
; item str@9 section=.rodata local=true zero=false
; item str@10 section=.rodata local=true zero=false
; item str@11 section=.rodata local=true zero=false
; item str@12 section=.rodata local=true zero=false
; item str@13 section=.rodata local=true zero=false
; item str@14 section=.rodata local=true zero=false
; item saved_input_mode section=.bss local=true zero=true
; item editor_level_entry section=.data local=true zero=false
; item saved_vtoc_data section=.data local=true zero=false
; item SAVED_FILE_DESCRIPTIVE_ENTRY_DATA section=.rodata local=true zero=false
; item LEVEL_MAP@lo section=.rodata local=true zero=false merge=true
; item LEVEL_MAP@hi section=.rodata local=true zero=false merge=true
; item __zp_init section=.text local=false zero=false
; zp PLAYER_COL size=1 at=$00 def=false init=
; zp PLAYER_ROW size=1 at=$01 def=false init=
; zp PLAYER_X_ADJ size=1 at=$02 def=false init=
; zp PLAYER_Y_ADJ size=1 at=$03 def=false init=
; zp PLAYER_ANIM_STATE size=1 at=$04 def=false init=
; zp PLAYER_FACING_DIRECTION size=1 at=$05 def=false init=
; zp cursor_cell@ret size=2 at=auto def=true init= local=true
; zp kbd_entry_index size=1 at=auto def=true init=$60 weight=162 local=true
; zp saved_game_colnum size=1 at=auto def=true init=$85 weight=3 local=true
; zp GUARD_LOC_COL size=1 at=$12 def=false init=
; zp GUARD_LOC_ROW size=1 at=$13 def=false init=
; zp GUARD_ANIM_STATE size=1 at=$14 def=false init=
; zp GUARD_FACING_DIRECTION size=1 at=$15 def=false init=
; zp GUARD_GOLD_TIMER size=1 at=$16 def=false init=
; zp GUARD_X_ADJ size=1 at=$17 def=false init=
; zp GUARD_Y_ADJ size=1 at=$18 def=false init=
; zp GUARD_NUM size=1 at=$19 def=false init=
; zp __dos_zp size=42 at=$26 def=false init=
; zp GUARD_RESURRECT_COL size=1 at=$53 def=false init=
; zp NOTE_INDEX size=1 at=$54 def=false init=
; zp GUARD_GOLD_TIMER_START_VALUE size=1 at=$5F def=false init=
; zp GUARD_PATTERNS size=3 at=$60 def=false init=
; zp GUARD_PHASE size=1 at=$64 def=false init=
; zp PADDLE_VALUES size=2 at=$65 def=false init=
; zp GAME_COLNUM size=1 at=$85 def=false init=
; zp GAME_ROWNUM size=1 at=$86 def=false init=
; zp DRAW_PAGE size=1 at=$87 def=false init=
; zp DIGITS size=3 at=$89 def=false init=
; zp FRAME_PERIOD size=1 at=$8C def=false init=
; zp GUARD_COUNT size=1 at=$8D def=false init=
; zp SCORE size=4 at=$8E def=false init=
; zp GOLD_COUNT size=1 at=$93 def=false init=
; zp DIDNT_PICK_UP_GOLD size=1 at=$94 def=false init=
; zp INPUT_MODE size=1 at=$95 def=false init=
; zp DISK_LEVEL_LOC size=1 at=$96 def=false init=
; zp GUARD_PATTERN_OFFSET size=1 at=$97 def=false init=
; zp LIVES size=1 at=$98 def=false init=
; zp ENABLE_SOUND size=1 at=$99 def=false init=
; zp ALIVE size=1 at=$9A def=false init=
; zp NOT_FALLING size=1 at=$9B def=false init=
; zp DIG_DIRECTION size=1 at=$9C def=false init=
; zp HI_SCORE_ELIGIBLE size=1 at=$9D def=false init=
; zp KEY_COMMAND size=1 at=$9E def=false init=
; zp KEY_COMMAND_LR size=1 at=$9F def=false init=
; zp DIG_ANIM_STATE size=1 at=$A0 def=false init=
; zp VERBATIM size=1 at=$A2 def=false init=
; zp LADDER_COUNT size=1 at=$A3 def=false init=
; zp FALL_PITCH size=1 at=$A4 def=false init=
; zp WIPE_MODE size=1 at=$A5 def=false init=
; zp LEVELNUM size=1 at=$A6 def=false init=
; zp GAME_MODE size=1 at=$A7 def=false init=
; zp DEMO_PTR size=2 at=$A8 def=false init=
; zp DEMO_MOVE size=1 at=$AA def=false init=
; zp DEMO_MOVE_TURNS size=1 at=$AB def=false init=
; zp DEMO_RUNNING size=1 at=$AC def=false init=
; zp LEVEL_SAVED size=1 at=$AD def=false init=
; zp BLOCK_DATA size=33 at=$DF def=false init=
; runtime hargs_size=1 hargs_need=0
; @zp-link-meta-end

editor_wait_for_key:
   SUBROUTINE

   LDA   #$00
   JSR   wait_for_key_with_cursor_page_1
   LDY   #$00
   STY   KBDSTRB
   CMP   #$9B
   BNE   .if_end@0
   JMP   editor_command_loop
.if_end@0:
   RTS

start_message:
   SUBROUTINE

   JSR   clear_hgr2
   LDA   #$40
   STA   DRAW_PAGE
   JSR   start_message@run0
   RTS

start_message@run0:
   SUBROUTINE

   LDA   #$00
   STA   GAME_COLNUM
   STA   GAME_ROWNUM
   RTS

hit_key_to_continue:
   SUBROUTINE

   LDX   #>str@0
   LDA   #<str@0
   JSR   put_string
   JSR   hit_key_to_continue@run1
   JSR   wait_for_key
   LDA   #$00
   STA   KBDSTRB
   STA   TXTPAGE1
   RTS

hit_key_to_continue@run1:
   SUBROUTINE

   JSR   beep
   LDA   #$00
   STA   TXTPAGE2
   RTS

dont_manipulate_master_disk:
   SUBROUTINE

   JSR   start_message
   LDX   #>str@1
   LDA   #<str@1
   JSR   put_string
   JMP   hit_key_to_continue

bad_data_disk:
   SUBROUTINE

   JSR   start_message
   LDX   #>str@2
   LDA   #<str@2
   JSR   put_string
   JMP   hit_key_to_continue

check_for_valid_data_disk:
   SUBROUTINE

   LDA   #$01
   JSR   access_hi_score_data_from_disk
   ORA   #$00
   BNE   .if_end@1
   JSR   bad_data_disk
   JMP   start_level_editor
.if_end@1:
   CMP   #$01
   BNE   .if_end@2
   JSR   dont_manipulate_master_disk
   JMP   start_level_editor
.if_end@2:
   RTS

get_level_from_keyboard:
   SUBROUTINE

   LDA   DISK_LEVEL_LOC
   CLC
   ADC   #$01
   JSR   to_decimal3
   LDA   GAME_COLNUM
   STA   saved_game_colnum
   LDA   #$00
   STA   kbd_entry_index
   JMP   .loop@0_start
.get_level_from_keyboard@rot@0:
   TAX
   LDA   DIGITS,X
   JSR   put_digit
   INC   kbd_entry_index
.loop@0_start:
   LDA   kbd_entry_index
   CMP   #$03
   BCC   .get_level_from_keyboard@rot@0
   LDA   saved_game_colnum
   STA   GAME_COLNUM
   LDA   #$00
   STA   kbd_entry_index
   JMP   .loop@1_start
.get_level_from_keyboard@rot@1:
   CMP   #$88
   BEQ   .case@3
   CMP   #$95
   BEQ   .case@4
   CMP   #$9B
   BEQ   .case@5
   BNE   .default@6
.case@3:
   LDA   kbd_entry_index
   BEQ   .switch@2_break
.xjump@0:
   DEC   kbd_entry_index
   DEC   GAME_COLNUM
   JMP   .loop@1_start
.case@4:
   LDA   kbd_entry_index
   CMP   #$02
   BEQ   .switch@2_break
   INC   GAME_COLNUM
   INC   kbd_entry_index
   JMP   .loop@1_start
.case@5:
   JMP   editor_command_loop
.default@6:
   SEC
   SBC   #$B0
   CMP   #$0A
   BCS   .switch@2_break
   LDX   kbd_entry_index
   STA   DIGITS,X
   JSR   put_digit
   INC   kbd_entry_index
   LDA   kbd_entry_index
   CMP   #$03
   BCC   .loop@1_start
   BCS   .xjump@0
.switch@2_break:
   JSR   beep
.loop@1_start:
   LDX   kbd_entry_index
   LDA   #$3B
   CLC
   ADC   DIGITS,X
   JSR   wait_for_key_with_cursor_page_1
   LDY   #$00
   STY   KBDSTRB
   CMP   #$8D
   BNE   .get_level_from_keyboard@rot@1
   LDA   saved_game_colnum
   CLC
   ADC   #$03
   STA   GAME_COLNUM
   LDA   DIGITS
   STA   __local_get_level_from_keyboard__n
   LDA   #$00
   STA   __local_get_level_from_keyboard__level
   LDX   #$00
   BEQ   .loop@7_start
.get_level_from_keyboard@rot@2:
   LDA   __local_get_level_from_keyboard__level
   CLC
   ADC   #$64
   STA   __local_get_level_from_keyboard__level
   BCC   .carry_inc@0
   INX
.carry_inc@0:
   DEC   __local_get_level_from_keyboard__n
.loop@7_start:
   LDA   __local_get_level_from_keyboard__n
   BNE   .get_level_from_keyboard@rot@2
   LDA   DIGITS+1
   STA   __local_get_level_from_keyboard__n
   JMP   .loop@8_start
.get_level_from_keyboard@rot@3:
   LDA   __local_get_level_from_keyboard__level
   CLC
   ADC   #$0A
   STA   __local_get_level_from_keyboard__level
   BCC   .carry_inc@1
   INX
.carry_inc@1:
   DEC   __local_get_level_from_keyboard__n
.loop@8_start:
   LDA   __local_get_level_from_keyboard__n
   BNE   .get_level_from_keyboard@rot@3
   LDA   __local_get_level_from_keyboard__level
   CLC
   ADC   DIGITS+2
   STA   __local_get_level_from_keyboard__n
   BCC   .carry_inc@2
   INX
.carry_inc@2:
   TXA
   BEQ   .if_end@8
   SEC
   RTS
.if_end@8:
   LDA   __local_get_level_from_keyboard__n
   STA   LEVELNUM
   SEC
   SBC   #$01
   STA   DISK_LEVEL_LOC
   SEC
   SBC   #$96
   TYA
   ROL
   CMP   #$01
   RTS

level_editor:
   SUBROUTINE

   LDA   #$00
   STA   SCORE
   STA   SCORE+1
   STA   SCORE+2
   STA   SCORE+3
   LDA   #$05
   STA   LIVES
   STA   GAME_MODE
   LDA   INPUT_MODE
   STA   saved_input_mode
   LDA   #$CB
   STA   INPUT_MODE
   LDA   #$00
   STA   TXTPAGE1
   LDA   DISK_LEVEL_LOC
   CMP   #$96
   BCC   .if_end@9
   LDA   #$00
   STA   DISK_LEVEL_LOC
.if_end@9:
   JMP   start_level_editor

start_level_editor:
   SUBROUTINE

   JSR   clear_hgr1
   LDA   #$20
   STA   DRAW_PAGE
   JSR   start_message@run0
   LDX   #>str@3
   LDA   #<str@3
   JSR   put_string
   JMP   editor_command_loop

editor_play_level:
   SUBROUTINE

   LDX   #>str@4
   LDA   #<str@4
   JSR   put_string
   JSR   get_level_from_keyboard
   BCC   .if_end@10
   JMP   beep
.if_end@10:
   LDA   saved_input_mode
   STA   INPUT_MODE
   LDA   #$03
   STA   GAME_MODE
   LDA   DISK_LEVEL_LOC
   BEQ   .cmp_true@1
   LDA   #$00
   BEQ   .cmp_end@0
.cmp_true@1:
   LDA   #$01
.cmp_end@0:
   STA   HI_SCORE_ELIGIBLE
   JMP   init_game_data

editor_clear_level:
   SUBROUTINE

   LDX   #>str@5
   LDA   #<str@5
   JSR   put_string
   JSR   get_level_from_keyboard
   BCC   .if_end@11
   JMP   beep
.if_end@11:
   JSR   check_for_valid_data_disk
   LDX   #$00
   TXA
.loop@9_start:
   STA   DISK_BUFFER,X
   INX
   BNE   .loop@9_start
   LDA   #$02
   JMP   access_compressed_level_data

editor_edit_level:
   SUBROUTINE

   LDX   #>str@6
   LDA   #<str@6
   JSR   put_string
   JSR   get_level_from_keyboard
   BCC   .if_end@12
   JMP   beep
.if_end@12:
   JMP   edit_level

editor_move_level:
   SUBROUTINE

   LDX   #>str@7
   LDA   #<str@7
   JSR   put_string
   JSR   get_level_from_keyboard
   BCC   .if_end@13
   JMP   beep
.if_end@13:
   LDA   DISK_LEVEL_LOC
   STA   editor_level_entry
   LDX   #>str@8
   LDA   #<str@8
   JSR   put_string
   JSR   get_level_from_keyboard
   BCC   .if_end@14
   JMP   beep
.if_end@14:
   LDA   DISK_LEVEL_LOC
   STA   saved_vtoc_data
   LDX   #>str@9
   LDA   #<str@9
   JSR   put_string
   JSR   editor_wait_for_key
   JSR   check_for_valid_data_disk
   LDA   editor_level_entry
   STA   DISK_LEVEL_LOC
   LDA   #$01
   JSR   access_compressed_level_data
   LDX   #>str@10
   LDA   #<str@10
   JSR   put_string
   JSR   editor_wait_for_key
   JSR   check_for_valid_data_disk
   LDA   saved_vtoc_data
   STA   DISK_LEVEL_LOC
   LDA   #$02
   JMP   access_compressed_level_data

editor_initialize_disk:
   SUBROUTINE

   LDX   #>str@11
   LDA   #<str@11
   JSR   put_string
   JSR   editor_wait_for_key
   CMP   #$D9
   BEQ   .if_end@15
   RTS
.if_end@15:
   LDA   DISK_LEVEL_LOC
   STA   __local_editor_initialize_disk__saved_level
   LDA   #$04
   JSR   access_compressed_level_data
   LDA   #<DISK_BOOT_SECTOR_DATA
   STA   DOS_IOB+8
   LDA   #>DISK_BOOT_SECTOR_DATA
   STA   DOS_IOB+9
   LDA   #$00
   STA   DOS_IOB+5
   STA   DOS_IOB+4
   LDA   #$02
   STA   DOS_IOB+12
   JSR   access_disk_or_reset_game
   LDA   #$E0
   STA   DISK_LEVEL_LOC
   LDA   #$01
   JSR   access_compressed_level_data
   LDX   #$00
.editor_initialize_disk@rot@4:
   LDA   saved_vtoc_data+1,X
   STA   DISK_BUFFER,X
   INX
   CPX   #$38
   BCC   .editor_initialize_disk@rot@4
   LDA   #$02
   JSR   access_compressed_level_data
   LDA   #$EF
   STA   DISK_LEVEL_LOC
   LDA   #$01
   JSR   access_compressed_level_data
   LDX   #$00
.editor_initialize_disk@rot@5:
   LDA   SAVED_FILE_DESCRIPTIVE_ENTRY_DATA,X
   STA   DISK_BUFFER+11,X
   INX
   CPX   #$21
   BCC   .editor_initialize_disk@rot@5
   LDA   #$02
   JSR   access_compressed_level_data
   LDA   #$01
   JSR   access_hi_score_data_from_disk
   LDX   #$00
.editor_initialize_disk@rot@6:
   LDA   HI_SCORE_DATA_MARKER,X
   STA   HI_SCORE_MARKER,X
   INX
   CPX   #$0B
   BCC   .editor_initialize_disk@rot@6
   LDA   #$02
   JSR   access_hi_score_data_from_disk
   LDA   __local_editor_initialize_disk__saved_level
   STA   DISK_LEVEL_LOC
   RTS

editor_clear_high_scores:
   SUBROUTINE

   LDX   #>str@12
   LDA   #<str@12
   JSR   put_string
   JSR   editor_wait_for_key
   CMP   #$D9
   BEQ   .if_end@16
   RTS
.if_end@16:
   LDA   #$01
   JSR   access_hi_score_data_from_disk
   ORA   #$00
   BNE   .if_end@17
   JSR   bad_data_disk
   JMP   start_level_editor
.if_end@17:
   LDA   #$00
   TAX
.editor_clear_high_scores@rot@7:
   STA   HI_SCORES,X
   INX
   CPX   #$50
   BCC   .editor_clear_high_scores@rot@7
   LDA   #$02
   JMP   access_hi_score_data_from_disk

editor_command_loop:
   SUBROUTINE

.loop@14_start:
   LDA   GAME_ROWNUM
   CMP   #$09
   BCC   .if_end@18
   JMP   start_level_editor
.if_end@18:
   LDX   #>str@13
   LDA   #<str@13
   JSR   put_string
   JSR   editor_wait_for_key
   CMP   #$D0
   BEQ   .case@16
   CMP   #$C3
   BEQ   .case@17
   CMP   #$C5
   BEQ   .case@18
   CMP   #$CD
   BEQ   .case@19
   CMP   #$C9
   BEQ   .case@20
   CMP   #$D3
   BEQ   .case@21
   BNE   .default@22
.case@16:
   JSR   editor_play_level
   JMP   .loop@14_start
.case@17:
   JSR   editor_clear_level
   JMP   .loop@14_start
.case@18:
   JSR   editor_edit_level
   JMP   .loop@14_start
.case@19:
   JSR   editor_move_level
   JMP   .loop@14_start
.case@20:
   JSR   editor_initialize_disk
   JMP   .loop@14_start
.case@21:
   JSR   editor_clear_high_scores
   JMP   .loop@14_start
.default@22:
   JSR   beep
   JMP   .loop@14_start

edit_level:
   SUBROUTINE

   JSR   clear_hgr2
   LDA   #$40
   STA   DRAW_PAGE
   JSR   put_status_draw
   LDA   #$20
   STA   DRAW_PAGE
   JSR   check_for_valid_data_disk
   LDA   #$01
   STA   LEVEL_SAVED
   LDA   #$00
   JSR   load_level
   JMP   edit_level@tail0

edit_level@tail0:
   SUBROUTINE

   JSR   start_message@run0
   JMP   edit_level_key_loop

level_edit_save1:
   SUBROUTINE

   LDA   GAME_ROWNUM
   STA   __local_level_edit_save1__row
   LDA   GAME_COLNUM
   STA   __local_level_edit_save1__col
   LDA   #$01
   JSR   access_hi_score_data_from_disk
   ORA   #$00
   BNE   .if_else@20
   JSR   bad_data_disk
   JMP   .if_end@19
.if_else@20:
   CMP   #$01
   BNE   .if_else@22
   JSR   dont_manipulate_master_disk
   JMP   .if_end@21
.if_else@22:
   JSR   compress_and_save_level_data
   LDA   __local_level_edit_save1__col
   STA   GAME_COLNUM
   LDA   __local_level_edit_save1__row
   STA   GAME_ROWNUM
   LDA   #$01
   STA   LEVEL_SAVED
   RTS
.if_end@21:
.if_end@19:
   JMP   edit_level@tail0

level_edit_change_level:
   SUBROUTINE

   LDA   LEVEL_SAVED
   BNE   .if_end@23
   JSR   start_message
   LDX   #>str@14
   LDA   #<str@14
   JSR   put_string
   JSR   hit_key_to_continue@run1
.loop@23_start:
   LDA   #$00
   JSR   wait_for_key
   LDY   #$00
   STY   KBDSTRB
   CMP   #$CE
   BEQ   .loop@23_break
   CMP   #$D9
   BNE   .loop@23_start
   JSR   level_edit_save1
.loop@23_break:
.if_end@23:
   LDA   #$00
   STA   TXTPAGE1
   STA   GAME_COLNUM
   STA   GAME_ROWNUM
   RTS

cursor_cell:
   SUBROUTINE

   LDX   GAME_ROWNUM
   LDA   LEVEL_MAP@lo,X
   CLC
   ADC   GAME_COLNUM
   STA   cursor_cell@ret
   LDA   LEVEL_MAP@hi,X
   ADC   #$00
   STA   cursor_cell@ret+1
   RTS

edit_level_key_loop:
   SUBROUTINE

   JMP   .loop@24_start
.edit_level_key_loop@rot@8:
   JSR   cursor_cell
   LDY   #$00
   LDA   (cursor_cell@ret),Y
   CMP   __local_edit_level_key_loop__3
   BEQ   .i1_if_end@26
   LDA   #$00
   STA   LEVEL_SAVED
.i1_if_end@26:
   LDA   __local_edit_level_key_loop__3
   LDY   #$00
   STA   (cursor_cell@ret),Y
   JSR   draw_sprite_page1
.loop@24_start:
   JSR   cursor_cell
   LDY   #$00
   LDA   (cursor_cell@ret),Y
   JSR   wait_for_key_with_cursor_page_1
   STA   __local_edit_level_key_loop__0
   LDY   #$00
   STY   KBDSTRB
   SEC
   SBC   #$B0
   STA   __local_edit_level_key_loop__3
   CMP   #$0A
   BCC   .edit_level_key_loop@rot@8
   LDA   __local_edit_level_key_loop__0
   CMP   #$CA
   BEQ   .case@26
   CMP   #$C9
   BEQ   .case@27
   CMP   #$CB
   BEQ   .case@28
   CMP   #$CD
   BEQ   .case@29
   CMP   #$93
   BEQ   .case@30
   CMP   #$95
   BEQ   .case@31
   CMP   #$88
   BEQ   .case@32
   CMP   #$91
   BEQ   .case@33
   BNE   .switch@25_break
.case@26:
   LDA   GAME_COLNUM
   BEQ   .switch@25_break
   DEC   GAME_COLNUM
   JMP   .loop@24_start
.case@27:
   LDA   GAME_ROWNUM
   BEQ   .switch@25_break
   DEC   GAME_ROWNUM
   JMP   .loop@24_start
.case@28:
   LDA   GAME_COLNUM
   CMP   #$1B
   BCS   .switch@25_break
   INC   GAME_COLNUM
   JMP   .loop@24_start
.case@29:
   LDA   GAME_ROWNUM
   CMP   #$0F
   BCS   .switch@25_break
   INC   GAME_ROWNUM
   JMP   .loop@24_start
.case@30:
   JSR   level_edit_save1
   JMP   .loop@24_start
.case@31:
   LDA   DISK_LEVEL_LOC
   CMP   #$95
   BEQ   .switch@25_break
   JSR   level_edit_change_level
   INC   DISK_LEVEL_LOC
   INC   LEVELNUM
   JMP   edit_level
.case@32:
   LDA   DISK_LEVEL_LOC
   BEQ   .switch@25_break
   JSR   level_edit_change_level
   DEC   LEVELNUM
   DEC   DISK_LEVEL_LOC
   JMP   edit_level
.case@33:
   JSR   level_edit_change_level
   JMP   start_level_editor
.switch@25_break:
   JSR   beep
   JMP   .loop@24_start

format_patch:
   SUBROUTINE

   STX   __local_format_patch__slot16
   LDA   #$44
   STA   DOS_CURRENT_TRACK
   LDA   #$00
   JSR   SEEKABS
   LDA   #$00
   STA   __local_format_patch__tries_0
   LDY   #$20
   STY   __local_format_patch__tries_1
.format_patch@await_mark:
   LDA   __local_format_patch__tries_0
   SEC
   SBC   #$01
   STA   __local_format_patch__tries_0
   BCS   .carry_inc@3
   DEC   __local_format_patch__tries_1
.carry_inc@3:
   ORA   __local_format_patch__tries_1
   BNE   .if_end@34
   JMP   DSKFORM
.if_end@34:
.loop@34_start:
   LDX   __local_format_patch__slot16
   LDA   DISK_IO+4,X
   STA   __local_format_patch__b
   BPL   .loop@34_start
.vthread@0:
   CMP   #$D4
   BNE   .format_patch@await_mark
.loop@35_start:
   LDA   DISK_IO+4,X
   STA   __local_format_patch__b
   BPL   .loop@35_start
.vthread@1:
   CMP   #$D5
   BNE   .vthread@0
.loop@36_start:
   LDA   DISK_IO+4,X
   STA   __local_format_patch__b
   BPL   .loop@36_start
   CMP   #$D6
   BNE   .vthread@1
   LDA   DISK_IO,X
   STA   __local_format_patch__0
   JSR   dont_manipulate_master_disk
   JMP   start_level_editor

str@0:
   DC.B  $0D, $0D, $48, $49, $54, $20, $41, $20, $4B, $45, $59, $20, $54, $4F, $20, $43
   DC.B  $4F, $4E, $54, $49, $4E, $55, $45, $20, $00

str@1:
   DC.B  $55, $53, $45, $52, $20, $4E, $4F, $54, $20, $41, $4C, $4C, $4F, $57, $45, $44
   DC.B  $20, $54, $4F, $0D, $4D, $41, $4E, $49, $50, $55, $4C, $41, $54, $45, $20, $4D
   DC.B  $41, $53, $54, $45, $52, $20, $44, $49, $53, $4B, $45, $54, $54, $45, $2E, $00

str@2:
   DC.B  $44, $49, $53, $4B, $45, $54, $54, $45, $20, $49, $4E, $20, $44, $52, $49, $56
   DC.B  $45, $20, $49, $53, $20, $4E, $4F, $54, $20, $41, $0D, $4C, $4F, $44, $45, $20
   DC.B  $52, $55, $4E, $4E, $45, $52, $20, $44, $41, $54, $41, $20, $44, $49, $53, $4B
   DC.B  $2E, $00

str@3:
   DC.B  $20, $20, $4C, $4F, $44, $45, $20, $52, $55, $4E, $4E, $45, $52, $20, $42, $4F
   DC.B  $41, $52, $44, $20, $45, $44, $49, $54, $4F, $52, $0D, $2D, $2D, $2D, $2D, $2D
   DC.B  $2D, $2D, $2D, $2D, $2D, $2D, $2D, $2D, $2D, $2D, $2D, $2D, $2D, $2D, $2D, $2D
   DC.B  $2D, $2D, $2D, $2D, $2D, $2D, $2D, $0D, $20, $20, $3C, $45, $53, $43, $3E, $20
   DC.B  $41, $42, $4F, $52, $54, $53, $20, $41, $4E, $59, $20, $43, $4F, $4D, $4D, $41
   DC.B  $4E, $44, $0D, $00

str@4:
   DC.B  $0D, $3E, $3E, $50, $4C, $41, $59, $20, $4C, $45, $56, $45, $4C, $00

str@5:
   DC.B  $0D, $3E, $3E, $43, $4C, $45, $41, $52, $20, $4C, $45, $56, $45, $4C, $00

str@6:
   DC.B  $0D, $3E, $3E, $45, $44, $49, $54, $20, $4C, $45, $56, $45, $4C, $00

str@7:
   DC.B  $0D, $3E, $3E, $4D, $4F, $56, $45, $20, $4C, $45, $56, $45, $4C, $00

str@8:
   DC.B  $20, $54, $4F, $20, $4C, $45, $56, $45, $4C, $00

str@9:
   DC.B  $0D, $20, $20, $53, $4F, $55, $52, $43, $45, $20, $44, $49, $53, $4B, $45, $54
   DC.B  $54, $45, $00

str@10:
   DC.B  $0D, $20, $20, $44, $45, $53, $54, $49, $4E, $41, $54, $49, $4F, $4E, $20, $44
   DC.B  $49, $53, $4B, $45, $54, $54, $45, $00

str@11:
   DC.B  $0D, $3E, $3E, $49, $4E, $49, $54, $49, $41, $4C, $49, $5A, $45, $0D, $20, $20
   DC.B  $54, $48, $49, $53, $20, $46, $4F, $52, $4D, $41, $54, $53, $20, $54, $48, $45
   DC.B  $20, $44, $49, $53, $4B, $45, $54, $54, $45, $0D, $20, $20, $46, $4F, $52, $20
   DC.B  $55, $53, $45, $52, $20, $43, $52, $45, $41, $54, $45, $44, $20, $4C, $45, $56
   DC.B  $45, $4C, $53, $2E, $0D, $20, $20, $28, $43, $41, $55, $54, $49, $4F, $4E, $2E
   DC.B  $20, $49, $54, $20, $45, $52, $41, $53, $45, $53, $20, $54, $48, $45, $0D, $20
   DC.B  $20, $20, $45, $4E, $54, $49, $52, $45, $20, $44, $49, $53, $4B, $45, $54, $54
   DC.B  $45, $20, $46, $49, $52, $53, $54, $29, $0D, $0D, $20, $20, $41, $52, $45, $20
   DC.B  $59, $4F, $55, $20, $53, $55, $52, $45, $20, $28, $59, $2F, $4E, $29, $20, $00

str@12:
   DC.B  $0D, $3E, $3E, $43, $4C, $45, $41, $52, $20, $53, $43, $4F, $52, $45, $20, $46
   DC.B  $49, $4C, $45, $0D, $20, $20, $54, $48, $49, $53, $20, $43, $4C, $45, $41, $52
   DC.B  $53, $20, $54, $48, $45, $20, $48, $49, $47, $48, $0D, $20, $20, $53, $43, $4F
   DC.B  $52, $45, $20, $46, $49, $4C, $45, $20, $4F, $46, $20, $41, $4C, $4C, $0D, $20
   DC.B  $20, $45, $4E, $54, $52, $49, $45, $53, $2E, $0D, $0D, $20, $20, $41, $52, $45
   DC.B  $20, $59, $4F, $55, $20, $53, $55, $52, $45, $20, $28, $59, $2F, $4E, $29, $20
   DC.B  $00

str@13:
   DC.B  $0D, $43, $4F, $4D, $4D, $41, $4E, $44, $3E, $00

str@14:
   DC.B  $4C, $45, $56, $45, $4C, $20, $48, $41, $53, $20, $42, $45, $45, $4E, $20, $43
   DC.B  $48, $41, $4E, $47, $45, $44, $20, $42, $55, $54, $0D, $4E, $4F, $54, $20, $53
   DC.B  $41, $56, $45, $44, $2E, $20, $44, $4F, $20, $59, $4F, $55, $20, $57, $49, $53
   DC.B  $48, $20, $54, $4F, $0D, $53, $41, $56, $45, $20, $4D, $4F, $44, $49, $46, $49
   DC.B  $45, $44, $20, $4C, $45, $56, $45, $4C, $20, $28, $59, $2F, $4E, $29, $20, $00

saved_input_mode:
   DS.B  1

editor_level_entry:
   DC.B  $0F

saved_vtoc_data:
   DC.B  $60, $02, $11, $0F, $04, $00, $00, $FE, $00, $00, $00, $00, $00, $00, $00, $00
   DC.B  $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00
   DC.B  $00, $00, $00, $00, $00, $00, $00, $00, $7A, $00, $00, $00, $00, $00, $00, $00
   DC.B  $00, $FF, $FF, $00, $00, $23, $0F, $00, $01

SAVED_FILE_DESCRIPTIVE_ENTRY_DATA:
   DC.B  $22, $0F, $88, $88, $88, $88, $88, $88, $88, $88, $CC, $CF, $C4, $C5, $A0, $D2
   DC.B  $D5, $CE, $CE, $C5, $D2, $A0, $C4, $C1, $D4, $C1, $A0, $C4, $C9, $D3, $CB, $A0
   DC.B  $A0

LEVEL_MAP@lo:
   DC.B  $00, $1C, $38, $54, $70, $8C, $A8, $C4, $E0, $FC, $18, $34, $50, $6C, $88, $A4
   DC.B  $C0

LEVEL_MAP@hi:
   DC.B  $08, $08, $08, $08, $08, $08, $08, $08, $08, $08, $09, $09, $09, $09, $09, $09
   DC.B  $09

__zp_init:
   SUBROUTINE

   LDA   #$60
   STA   kbd_entry_index
   LDA   #$85
   STA   saved_game_colnum
   RTS
