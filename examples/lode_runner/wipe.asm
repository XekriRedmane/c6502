PLAYER_COL	EQU	$00
PLAYER_ROW	EQU	$01
PLAYER_X_ADJ	EQU	$02
PLAYER_Y_ADJ	EQU	$03
PLAYER_ANIM_STATE	EQU	$04
PLAYER_FACING_DIRECTION	EQU	$05
HARGS	EQU	$06
anim_count	EQU	$11
GUARD_LOC_COL	EQU	$12
GUARD_LOC_ROW	EQU	$13
GUARD_ANIM_STATE	EQU	$14
GUARD_FACING_DIRECTION	EQU	$15
GUARD_GOLD_TIMER	EQU	$16
GUARD_X_ADJ	EQU	$17
GUARD_Y_ADJ	EQU	$18
GUARD_NUM	EQU	$19
SSP	EQU	$1A
FP	EQU	$1C
DPTR	EQU	$1E
row_minus_x	EQU	$20
row_minus_y	EQU	$22
row_plus_x	EQU	$24
__dos_zp	EQU	$26
row_plus_y	EQU	$50
wipe_dir	EQU	$52
GUARD_RESURRECT_COL	EQU	$53
NOTE_INDEX	EQU	$54
GUARD_GOLD_TIMER_START_VALUE	EQU	$5F
GUARD_PATTERNS	EQU	$60
GUARD_PHASE	EQU	$64
PADDLE_VALUES	EQU	$65
__local_spinning_game_over__0	EQU	$80
__local_step_left__0	EQU	$80
__zpabi_div_by_7__c_0	EQU	$80
__zpabi_draw_sprite_at_pixel_coords__row	EQU	$80
__zpabi_draw_wipe_row__west_0	EQU	$80
__zpabi_erase_sprite_at_pixel_coords__row	EQU	$80
__zpabi_play_note__y	EQU	$80
__local_spinning_game_over__i	EQU	$81
__local_step_left__c	EQU	$81
__local_step_right__c	EQU	$81
__zpabi_div_by_7__c_1	EQU	$81
__zpabi_draw_wipe_row__west_1	EQU	$81
__local_step_left__1	EQU	$82
__local_step_right__1	EQU	$82
__zpabi_draw_wipe_block__c_0	EQU	$82
__zpabi_draw_wipe_row__east_0	EQU	$82
__zpabi_draw_wipe_block__c_1	EQU	$83
__zpabi_draw_wipe_row__east_1	EQU	$83
__local_iris_wipe__r	EQU	$84
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
__local_iris_wipe_step__0	EQU	$AE
__local_show_anim_line__y	EQU	$AE
__local_iris_wipe_step__1	EQU	$AF
__local_show_anim_line__x	EQU	$AF
__local_iris_wipe_step__2	EQU	$B0
__local_show_anim_line__0	EQU	$B0
__local_iris_wipe_step__3	EQU	$B1
__local_show_anim_line__1	EQU	$B1
__local_iris_wipe_step__d_1	EQU	$B2
__local_show_anim_line__2	EQU	$B2
__local_iris_wipe_step__d_0	EQU	$B3
__local_show_anim_line__3	EQU	$B3
__local_iris_wipe_step__y	EQU	$B4
__local_show_anim_line__4	EQU	$B4
__local_iris_wipe_step__x	EQU	$B5
__local_show_anim_line__rows_0	EQU	$B5
__local_show_anim_line__rows_1	EQU	$B6
__local_div_by_7__x_0	EQU	$B7
__local_draw_wipe_row__row_1	EQU	$B7
__local_div_by_7__d_1	EQU	$B8
__local_draw_wipe_row__row_0	EQU	$B8
__local_div_by_7__1	EQU	$BA
__local_div_by_7__q	EQU	$BB
__local_draw_wipe_block__0	EQU	$BB
__local_div_by_7__n_0	EQU	$BC
__local_draw_wipe_block__1	EQU	$BC
__local_div_by_7__x_1	EQU	$BD
__local_draw_wipe_block__2	EQU	$BD
__local_draw_wipe_block__3	EQU	$BE
__local_draw_wipe_block__line	EQU	$BF
__local_draw_wipe_block__4	EQU	$C0
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
; def div_by_7 params=__zpabi_div_by_7__x_0,__zpabi_div_by_7__x_1,__zpabi_div_by_7__c_0,__zpabi_div_by_7__c_1 locals=__local_div_by_7__x_0,__local_div_by_7__d_1,__local_div_by_7__0,__local_div_by_7__1,__local_div_by_7__q,__local_div_by_7__n_0,__local_div_by_7__x_1 indirect=false in_cycle=false param_regs=A,X,,
; def draw_wipe_block params=__zpabi_draw_wipe_block__line_0,__zpabi_draw_wipe_block__line_1,__zpabi_draw_wipe_block__c_0,__zpabi_draw_wipe_block__c_1 locals=__local_draw_wipe_block__0,__local_draw_wipe_block__1,__local_draw_wipe_block__2,__local_draw_wipe_block__3,__local_draw_wipe_block__line,__local_draw_wipe_block__4 indirect=false in_cycle=false param_regs=A,X,,
; def draw_wipe_row params=__zpabi_draw_wipe_row__row_0,__zpabi_draw_wipe_row__row_1,__zpabi_draw_wipe_row__east_0,__zpabi_draw_wipe_row__east_1,__zpabi_draw_wipe_row__west_0,__zpabi_draw_wipe_row__west_1 locals=__local_draw_wipe_row__row_1,__local_draw_wipe_row__row_0,__local_draw_wipe_row__0,__local_draw_wipe_row__1 indirect=false in_cycle=false param_regs=A,X,,,,
; def draw_wipe_step params= locals= indirect=false in_cycle=false
; def iris_wipe params= locals=__local_iris_wipe__r indirect=false in_cycle=false
; def iris_wipe_step params=__zpabi_iris_wipe_step__r locals=__local_iris_wipe_step__0,__local_iris_wipe_step__1,__local_iris_wipe_step__2,__local_iris_wipe_step__3,__local_iris_wipe_step__d_1,__local_iris_wipe_step__d_0,__local_iris_wipe_step__y,__local_iris_wipe_step__x,__local_iris_wipe_step__4 indirect=false in_cycle=false param_regs=A
; def show_anim_line params=__zpabi_show_anim_line__rows_0,__zpabi_show_anim_line__rows_1 locals=__local_show_anim_line__y,__local_show_anim_line__x,__local_show_anim_line__0,__local_show_anim_line__1,__local_show_anim_line__2,__local_show_anim_line__3,__local_show_anim_line__4,__local_show_anim_line__rows_0,__local_show_anim_line__rows_1 indirect=false in_cycle=false param_regs=A,X carry=true
; def spinning_game_over params= locals=__local_spinning_game_over__0,__local_spinning_game_over__i indirect=false in_cycle=false carry=true
; def step_left params=__zpabi_step_left__c_0,__zpabi_step_left__c_1 locals=__local_step_left__0,__local_step_left__c,__local_step_left__1 indirect=false in_cycle=false param_regs=A,X
; def step_right params=__zpabi_step_right__c_0,__zpabi_step_right__c_1 locals=__local_step_right__0,__local_step_right__c,__local_step_right__1 indirect=false in_cycle=false param_regs=A,X
; ext DSKFORM params=
; ext RWTS params=__zpabi_RWTS__iob_0,__zpabi_RWTS__iob_1 param_regs=Y,A carry=true
; ext SEEKABS params=__zpabi_SEEKABS__track param_regs=A
; ext access_compressed_level_data params=__zpabi_access_compressed_level_data__command param_regs=A
; ext access_disk_or_reset_game params=
; ext access_hi_score_data_from_disk params=__zpabi_access_hi_score_data_from_disk__command param_regs=A
; ext add_and_update_score params=__zpabi_add_and_update_score__bcd_points_0,__zpabi_add_and_update_score__bcd_points_1 param_regs=A,X
; ext append_note params=__zpabi_append_note__pitch,__zpabi_append_note__duration param_regs=A,X
; ext bad_data_disk params=
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
; ext editor_command_loop params=
; ext enable_next_level_ladders params=
; ext erase_guard params=
; ext erase_sprite_at_pixel_coords params=__zpabi_erase_sprite_at_pixel_coords__sprite,__zpabi_erase_sprite_at_pixel_coords__half_col,__zpabi_erase_sprite_at_pixel_coords__row param_regs=A,X,
; ext half_screen_col_for params=__zpabi_half_screen_col_for__col,__zpabi_half_screen_col_for__x_adj param_regs=A,X
; ext handle_timers params=
; ext hi_score_screen params=
; ext init_game_data params=
; ext level_editor params=
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
; ext start_level_editor params=
; ext to_decimal3 params=__zpabi_to_decimal3__n param_regs=A
; ext wait_for_key params=__zpabi_wait_for_key__cursor param_regs=A
; ext wait_for_key_with_cursor_page_1 params=__zpabi_wait_for_key_with_cursor_page_1__cursor param_regs=A
; ext wait_key params=
; call draw_wipe_row -> draw_wipe_block
; call draw_wipe_row -> row_to_addr
; call draw_wipe_step -> draw_wipe_row
; call iris_wipe -> iris_wipe_step
; call iris_wipe -> put_status_level
; call iris_wipe -> put_status_lives
; call iris_wipe_step -> div_by_7
; call iris_wipe_step -> draw_wipe_step
; call iris_wipe_step -> step_left
; call iris_wipe_step -> step_right
; call show_anim_line -> row_to_addr
; call spinning_game_over -> show_anim_line
; share member=draw_wipe_block:__zpabi_draw_wipe_block__c_0,__zpabi_draw_wipe_block__c_1 member=draw_wipe_row:__zpabi_draw_wipe_row__east_0,__zpabi_draw_wipe_row__east_1 live=draw_wipe_row:row_to_addr
; item div_by_7 section=.text local=true zero=false
; item step_right section=.text local=true zero=false
; item step_left section=.text local=true zero=false
; item draw_wipe_block section=.text local=true zero=false
; item draw_wipe_row section=.text local=true zero=false
; item draw_wipe_step section=.text local=true zero=false
; item draw_wipe_step@run1 section=.text local=true zero=false
; item draw_wipe_step@run0 section=.text local=true zero=false
; item iris_wipe_step section=.text local=true zero=false
; item iris_wipe section=.text local=false zero=false
; item show_anim_line section=.text local=true zero=false
; item spinning_game_over section=.text local=false zero=false
; item game_over_key section=.bss local=false zero=true
; item col_minus_x section=.bss local=true zero=true
; item col_minus_y section=.bss local=true zero=true
; item col_plus_y section=.bss local=true zero=true
; item col_plus_x section=.bss local=true zero=true
; item WIPE_BLOCK_CLOSE_MASK section=.rodata local=true zero=false
; item WIPE_BLOCK_OPEN_MASK section=.rodata local=true zero=false
; item ANIM0 section=.rodata local=true zero=false
; item ANIM1 section=.rodata local=true zero=false
; item ANIM2 section=.rodata local=true zero=false
; item ANIM3 section=.rodata local=true zero=false
; item ANIM4 section=.rodata local=true zero=false
; item ANIM5 section=.rodata local=true zero=false
; item ANIM6 section=.rodata local=true zero=false
; item ANIM7 section=.rodata local=true zero=false
; item ANIM8 section=.rodata local=true zero=false
; item ANIM9 section=.rodata local=true zero=false
; item ANIM10 section=.rodata local=true zero=false
; item ANIM_PIXELS section=.rodata local=true zero=false
; item SPIN_ORDER section=.rodata local=true zero=false
; item __zp_init section=.text local=false zero=false
; zp PLAYER_COL size=1 at=$00 def=false init=
; zp PLAYER_ROW size=1 at=$01 def=false init=
; zp PLAYER_X_ADJ size=1 at=$02 def=false init=
; zp PLAYER_Y_ADJ size=1 at=$03 def=false init=
; zp PLAYER_ANIM_STATE size=1 at=$04 def=false init=
; zp PLAYER_FACING_DIRECTION size=1 at=$05 def=false init=
; zp anim_count size=1 at=auto def=true init=$9D weight=13 local=true
; zp GUARD_LOC_COL size=1 at=$12 def=false init=
; zp GUARD_LOC_ROW size=1 at=$13 def=false init=
; zp GUARD_ANIM_STATE size=1 at=$14 def=false init=
; zp GUARD_FACING_DIRECTION size=1 at=$15 def=false init=
; zp GUARD_GOLD_TIMER size=1 at=$16 def=false init=
; zp GUARD_X_ADJ size=1 at=$17 def=false init=
; zp GUARD_Y_ADJ size=1 at=$18 def=false init=
; zp GUARD_NUM size=1 at=$19 def=false init=
; zp row_minus_x size=2 at=auto def=true init=$00,$00 weight=26 local=true
; zp row_minus_y size=2 at=auto def=true init=$00,$00 weight=26 local=true
; zp row_plus_x size=2 at=auto def=true init=$00,$00 weight=26 local=true
; zp __dos_zp size=42 at=$26 def=false init=
; zp row_plus_y size=2 at=auto def=true init=$00,$00 weight=26 local=true
; zp wipe_dir size=1 at=auto def=true init=$00 weight=3 local=true
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
; runtime hargs_size=11 hargs_need=0
; @zp-link-meta-end

div_by_7:
   SUBROUTINE

   STX   __local_div_by_7__x_1
   CLC
   ADC   #$23
   STA   __local_div_by_7__n_0
   BCC   .carry_inc@0
   INC   __local_div_by_7__x_1
.carry_inc@0:
   LDA   #$00
   STA   __local_div_by_7__q
   LDA   #$C0
   STA   __local_div_by_7__x_0
   LDY   #$01
   STY   __local_div_by_7__d_1
   JMP   .loop@0_start
.div_by_7@rot@0:
   ASL   __local_div_by_7__q
   LDA   __local_div_by_7__n_0
   SEC
   SBC   __local_div_by_7__x_0
   STA   __local_div_by_7__1
   LDA   __local_div_by_7__x_1
   SBC   __local_div_by_7__d_1
   BCC   .if_end@0
   INC   __local_div_by_7__q
   STA   __local_div_by_7__x_1
   LDA   __local_div_by_7__1
   STA   __local_div_by_7__n_0
.if_end@0:
   LSR   __local_div_by_7__d_1
   ROR   __local_div_by_7__x_0
.loop@0_start:
   LDA   __local_div_by_7__x_0
   SEC
   SBC   #$07
   LDA   __local_div_by_7__d_1
   SBC   #$00
   BCS   .div_by_7@rot@0
   LDA   __local_div_by_7__q
   SEC
   SBC   #$05
   LDY   #$00
   STA   (__zpabi_div_by_7__c_0),Y
   LDA   __local_div_by_7__n_0
   INY
   STA   (__zpabi_div_by_7__c_0),Y
   RTS

step_right:
   SUBROUTINE

   STA   __local_step_right__c
   STX   __local_step_right__1
   CLC
   LDY   #$01
   LDA   (__local_step_right__c),Y
   ADC   #$01
   STA   (__local_step_right__c),Y
   CMP   #$07
   BNE   .if_end@1
   LDA   #$00
   STA   (__local_step_right__c),Y
   DEY
   LDA   (__local_step_right__c),Y
   CLC
   ADC   #$01
   STA   (__local_step_right__c),Y
.if_end@1:
   RTS

step_left:
   SUBROUTINE

   STA   __local_step_left__c
   STX   __local_step_left__1
   LDY   #$01
   LDA   (__local_step_left__c),Y
   STA   __local_step_left__0
   BNE   .if_else@3
   LDA   #$06
   STA   (__local_step_left__c),Y
   DEY
   LDA   (__local_step_left__c),Y
   SEC
   SBC   #$01
   STA   (__local_step_left__c),Y
   RTS
.if_else@3:
   DEC   __local_step_left__0
   LDA   __local_step_left__0
   STA   (__local_step_left__c),Y
   RTS

draw_wipe_block:
   SUBROUTINE

   STA   __local_draw_wipe_block__line
   STX   __local_draw_wipe_block__4
   LDY   #$00
   LDA   (__zpabi_draw_wipe_block__c_0),Y
   STA   __local_draw_wipe_block__3
   CMP   #$28
   BCC   .if_end@4
   RTS
.if_end@4:
   LDA   wipe_dir
   BNE   .if_else@6
   LDY   __local_draw_wipe_block__3
   LDA   (__local_draw_wipe_block__line),Y
   STA   __local_draw_wipe_block__1
   LDY   #$01
   LDA   (__zpabi_draw_wipe_block__c_0),Y
   TAX
   LDA   __local_draw_wipe_block__1
   AND   WIPE_BLOCK_CLOSE_MASK,X
   LDY   __local_draw_wipe_block__3
   STA   (__local_draw_wipe_block__line),Y
   RTS
.if_else@6:
   LDY   __local_draw_wipe_block__3
   LDA   (__local_draw_wipe_block__line),Y
   STA   __local_draw_wipe_block__2
   LDA   __local_draw_wipe_block__line
   STA   __local_draw_wipe_block__0
   TXA
   ADC   #$20
   STA   __local_draw_wipe_block__1
   LDA   (__local_draw_wipe_block__0),Y
   STA   __local_draw_wipe_block__1
   LDY   #$01
   LDA   (__zpabi_draw_wipe_block__c_0),Y
   TAX
   LDA   __local_draw_wipe_block__1
   AND   WIPE_BLOCK_OPEN_MASK,X
   ORA   __local_draw_wipe_block__2
   LDY   __local_draw_wipe_block__3
   STA   (__local_draw_wipe_block__line),Y
   RTS

draw_wipe_row:
   SUBROUTINE

   STA   __local_draw_wipe_row__row_0
   SEC
   SBC   #$B0
   TXA
   SBC   #$00
   BCC   .if_end@7
   RTS
.if_end@7:
   LDX   #$20
   LDA   __local_draw_wipe_row__row_0
   JSR   row_to_addr
   STA   __local_draw_wipe_row__row_0
   STX   __local_draw_wipe_row__row_1
   JSR   draw_wipe_block
   LDA   __zpabi_draw_wipe_row__west_0
   STA   __zpabi_draw_wipe_block__c_0
   LDA   __zpabi_draw_wipe_row__west_1
   STA   __zpabi_draw_wipe_block__c_1
   LDX   __local_draw_wipe_row__row_1
   LDA   __local_draw_wipe_row__row_0
   JMP   draw_wipe_block

draw_wipe_step:
   SUBROUTINE

   JSR   draw_wipe_step@run0
   LDX   row_plus_x+1
   LDA   row_plus_x
   JSR   draw_wipe_row
   JSR   draw_wipe_step@run0
   LDX   row_minus_x+1
   LDA   row_minus_x
   JSR   draw_wipe_row
   JSR   draw_wipe_step@run1
   LDX   row_plus_y+1
   LDA   row_plus_y
   JSR   draw_wipe_row
   JSR   draw_wipe_step@run1
   LDX   row_minus_y+1
   LDA   row_minus_y
   JMP   draw_wipe_row

draw_wipe_step@run1:
   SUBROUTINE

   LDA   #<col_plus_x
   STA   __zpabi_draw_wipe_row__east_0
   LDA   #>col_plus_x
   STA   __zpabi_draw_wipe_row__east_1
   LDA   #<col_minus_x
   STA   __zpabi_draw_wipe_row__west_0
   LDA   #>col_minus_x
   STA   __zpabi_draw_wipe_row__west_1
   RTS

draw_wipe_step@run0:
   SUBROUTINE

   LDA   #<col_plus_y
   STA   __zpabi_draw_wipe_row__east_0
   LDA   #>col_plus_y
   STA   __zpabi_draw_wipe_row__east_1
   LDA   #<col_minus_y
   STA   __zpabi_draw_wipe_row__west_0
   LDA   #>col_minus_y
   STA   __zpabi_draw_wipe_row__west_1
   RTS

iris_wipe_step:
   SUBROUTINE

   STA   __local_iris_wipe_step__x
   ASL
   STA   __local_iris_wipe_step__1
   LDA   #$00
   ROL
   STA   __local_iris_wipe_step__0
   LDA   #$03
   SEC
   SBC   __local_iris_wipe_step__1
   STA   __local_iris_wipe_step__d_0
   LDA   #$00
   SBC   __local_iris_wipe_step__0
   STA   __local_iris_wipe_step__d_1
   LDA   #$58
   SEC
   SBC   __local_iris_wipe_step__x
   STA   row_minus_x
   LDA   #$00
   SBC   #$00
   STA   row_minus_x+1
   LDA   #$00
   STA   row_minus_y+1
   LDA   #$58
   STA   row_minus_y
   STA   row_plus_y
   LDY   #$00
   STY   row_plus_y+1
   CLC
   ADC   __local_iris_wipe_step__x
   STA   row_plus_x
   TYA
   ADC   #$00
   STA   row_plus_x+1
   LDA   #$8C
   SEC
   SBC   __local_iris_wipe_step__x
   STA   __local_iris_wipe_step__0
   TYA
   SBC   #$00
   TAX
   LDA   #<col_minus_x
   STA   __zpabi_div_by_7__c_0
   LDA   #>col_minus_x
   STA   __zpabi_div_by_7__c_1
   LDA   __local_iris_wipe_step__0
   JSR   div_by_7
   LDA   #$14
   STA   col_plus_y
   STA   col_minus_y
   LDA   #$00
   STA   col_plus_y+1
   STA   col_minus_y+1
   LDA   #$8C
   CLC
   ADC   __local_iris_wipe_step__x
   STA   __local_iris_wipe_step__0
   LDA   #$00
   ADC   #$00
   TAX
   LDA   #<col_plus_x
   STA   __zpabi_div_by_7__c_0
   LDA   #>col_plus_x
   STA   __zpabi_div_by_7__c_1
   LDA   __local_iris_wipe_step__0
   JSR   div_by_7
   LDA   #$00
   STA   __local_iris_wipe_step__y
.loop@1_start:
   LDA   __local_iris_wipe_step__x
   CMP   __local_iris_wipe_step__y
   BCS   .if_end@8
   RTS
.if_end@8:
   JSR   draw_wipe_step
   LDA   __local_iris_wipe_step__y
   CMP   __local_iris_wipe_step__x
   BNE   .if_end@9
   RTS
.if_end@9:
   LDA   __local_iris_wipe_step__d_1
   BPL   .if_else@11
   LDA   __local_iris_wipe_step__y
   ASL
   STA   __local_iris_wipe_step__1
   LDA   #$00
   ROL
   ASL   __local_iris_wipe_step__1
   ROL
   STA   __local_iris_wipe_step__2
   LDA   __local_iris_wipe_step__d_0
   CLC
   ADC   __local_iris_wipe_step__1
   STA   __local_iris_wipe_step__0
   LDA   __local_iris_wipe_step__d_1
   ADC   __local_iris_wipe_step__2
   STA   __local_iris_wipe_step__d_1
   LDA   __local_iris_wipe_step__0
   CLC
   ADC   #$06
   STA   __local_iris_wipe_step__d_0
   BCC   .if_end@10
   INC   __local_iris_wipe_step__d_1
   JMP   .if_end@10
.if_else@11:
   LDA   __local_iris_wipe_step__x
   SEC
   SBC   __local_iris_wipe_step__y
   STA   __local_iris_wipe_step__2
   LDA   #$00
   SBC   #$00
   ASL   __local_iris_wipe_step__2
   ROL
   ASL   __local_iris_wipe_step__2
   ROL
   STA   __local_iris_wipe_step__3
   LDA   #$10
   SEC
   SBC   __local_iris_wipe_step__2
   STA   __local_iris_wipe_step__1
   LDA   #$00
   SBC   __local_iris_wipe_step__3
   STA   __local_iris_wipe_step__0
   LDA   __local_iris_wipe_step__d_0
   CLC
   ADC   __local_iris_wipe_step__1
   STA   __local_iris_wipe_step__d_0
   LDA   __local_iris_wipe_step__d_1
   ADC   __local_iris_wipe_step__0
   STA   __local_iris_wipe_step__d_1
   DEC   __local_iris_wipe_step__x
   INC   row_minus_x
   BNE   .inc_done@0
   INC   row_minus_x+1
.inc_done@0:
   LDX   #>col_plus_x
   LDA   #<col_plus_x
   JSR   step_left
   LDX   #>col_minus_x
   LDA   #<col_minus_x
   JSR   step_right
   LDA   row_plus_x
   BNE   .carry_inc@2
   DEC   row_plus_x+1
.carry_inc@2:
   DEC   row_plus_x
.if_end@10:
   INC   __local_iris_wipe_step__y
   LDX   #>col_plus_y
   LDA   #<col_plus_y
   JSR   step_right
   LDA   row_minus_y
   SEC
   SBC   #$01
   STA   row_minus_y
   BCS   .carry_inc@3
   DEC   row_minus_y+1
.carry_inc@3:
   INC   row_plus_y
   BNE   .inc_done@1
   INC   row_plus_y+1
.inc_done@1:
   LDX   #>col_minus_y
   LDA   #<col_minus_y
   JSR   step_left
   JMP   .loop@1_start

iris_wipe:
   SUBROUTINE

   LDA   WIPE_MODE
   BEQ   .if_end@12
   LDA   #$00
   STA   wipe_dir
   LDA   #$AA
   STA   __local_iris_wipe__r
.loop@2_start:
   LDA   __local_iris_wipe__r
   JSR   iris_wipe_step
   DEC   __local_iris_wipe__r
   BNE   .loop@2_start
.if_end@12:
   LDA   #$01
   STA   WIPE_MODE
   STA   wipe_dir
   JSR   put_status_lives
   JSR   put_status_level
   LDA   #$01
   STA   __local_iris_wipe__r
.loop@3_start:
   JSR   iris_wipe_step
   INC   __local_iris_wipe__r
   LDA   __local_iris_wipe__r
   CMP   #$AA
   BNE   .loop@3_start
   RTS

show_anim_line:
   SUBROUTINE

   STA   __local_show_anim_line__rows_0
   STX   __local_show_anim_line__rows_1
   LDA   #$51
   STA   GAME_ROWNUM
   JMP   .loop@4_start
.show_anim_line@rot@2:
   LDX   #$20
   JSR   row_to_addr
   STA   __local_show_anim_line__4
   STX   __local_show_anim_line__3
   LDY   #$00
   LDA   (__local_show_anim_line__rows_0),Y
   INC   __local_show_anim_line__rows_0
   BNE   .inc_done@2
   INC   __local_show_anim_line__rows_1
.inc_done@2:
   ASL
   STA   __local_show_anim_line__x
   ASL
   ASL
   ASL
   SEC
   SBC   __local_show_anim_line__x
   STA   __local_show_anim_line__y
   LDA   #<ANIM_PIXELS
   CLC
   ADC   __local_show_anim_line__y
   STA   __local_show_anim_line__1
   LDA   #>ANIM_PIXELS
   ADC   #$00
   STA   __local_show_anim_line__2
   LDX   #$00
   BEQ   .loop@5_start
.show_anim_line@rot@1:
   TXA
   TAY
   LDA   (__local_show_anim_line__1),Y
   STA   __local_show_anim_line__0
   TXA
   ADC   __local_show_anim_line__4
   STA   __local_show_anim_line__y
   LDA   #$00
   ADC   __local_show_anim_line__3
   STA   __local_show_anim_line__x
   LDA   __local_show_anim_line__0
   LDY   #$0D
   STA   (__local_show_anim_line__y),Y
   INX
.loop@5_start:
   CPX   #$0E
   BCC   .show_anim_line@rot@1
   INC   GAME_ROWNUM
.loop@4_start:
   LDA   GAME_ROWNUM
   CMP   #$5F
   BCC   .show_anim_line@rot@2
   LDA   anim_count
   STA   __local_show_anim_line__x
   LDY   #$FF
   STY   __local_show_anim_line__y
   JMP   .loop@6_keep_start
.loop@7_keep_start:
   DEC   __local_show_anim_line__y
   BNE   .loop@7_keep_start
   DEC   __local_show_anim_line__x
.loop@6_keep_start:
   LDA   __local_show_anim_line__x
   BNE   .loop@7_keep_start
   INC   anim_count
   LDA   INPUT_MODE
   CMP   #$CB
   BEQ   .show_anim_line@scfold@1
   LDA   BUTN1
   BMI   .show_anim_line@scfold@2
   LDA   BUTN0
   BMI   .show_anim_line@scfold@2
.show_anim_line@scfold@1:
   LDA   KBD
   BPL   .if_end@19
.show_anim_line@scfold@2:
   LDA   KBD
   STA   game_over_key
   STA   KBDSTRB
   SEC
   RTS
.if_end@19:
   CLC
   RTS

spinning_game_over:
   SUBROUTINE

   LDA   #$01
   STA   anim_count
.loop@8_start:
   LDA   #$00
   STA   __local_spinning_game_over__i
.spinning_game_over@rot@3:
   LDX   __local_spinning_game_over__i
   LDA   SPIN_ORDER,X
   STA   __local_spinning_game_over__0
   LDY   SPIN_ORDER+20,X
   TYA
   TAX
   LDA   __local_spinning_game_over__0
   JSR   show_anim_line
   BCS   .tail@0
   INC   __local_spinning_game_over__i
   LDA   __local_spinning_game_over__i
   CMP   #$14
   BCC   .spinning_game_over@rot@3
   LDA   anim_count
   CMP   #$64
   BCC   .loop@8_start
   LDA   #$00
   STA   __local_spinning_game_over__i
.spinning_game_over@rot@4:
   LDX   __local_spinning_game_over__i
   LDA   SPIN_ORDER,X
   STA   __local_spinning_game_over__0
   LDY   SPIN_ORDER+20,X
   TYA
   TAX
   LDA   __local_spinning_game_over__0
   JSR   show_anim_line
   BCC   .if_end@21
.tail@0:
   RTS
.if_end@21:
   INC   __local_spinning_game_over__i
   LDA   __local_spinning_game_over__i
   CMP   #$06
   BCC   .spinning_game_over@rot@4
   CLC
   RTS

game_over_key:
   DS.B  1

col_minus_x:
   DS.B  2

col_minus_y:
   DS.B  2

col_plus_y:
   DS.B  2

col_plus_x:
   DS.B  2

WIPE_BLOCK_CLOSE_MASK:
   DC.B  $F0, $F0, $F0, $F0, $8F, $8F, $8F

WIPE_BLOCK_OPEN_MASK:
   DC.B  $8F, $8F, $8F, $8F, $F0, $F0, $F0

ANIM0:
   DC.B  $00, $01, $02, $03, $04, $05, $06, $07, $08, $09, $0A, $02, $01, $00

ANIM1:
   DC.B  $00, $00, $01, $02, $03, $04, $05, $07, $09, $0A, $02, $01, $00, $00

ANIM2:
   DC.B  $00, $00, $00, $01, $02, $03, $04, $09, $0A, $02, $01, $00, $00, $00

ANIM3:
   DC.B  $00, $00, $00, $00, $01, $02, $03, $0A, $02, $01, $00, $00, $00, $00

ANIM4:
   DC.B  $00, $00, $00, $00, $00, $01, $03, $0A, $01, $00, $00, $00, $00, $00

ANIM5:
   DC.B  $00, $00, $00, $00, $00, $00, $01, $01, $00, $00, $00, $00, $00, $00

ANIM6:
   DC.B  $00, $01, $02, $0A, $09, $08, $07, $06, $05, $04, $03, $02, $01, $00

ANIM7:
   DC.B  $00, $00, $01, $02, $0A, $09, $07, $05, $04, $03, $02, $01, $00, $00

ANIM8:
   DC.B  $00, $00, $00, $01, $02, $0A, $09, $04, $03, $02, $01, $00, $00, $00

ANIM9:
   DC.B  $00, $00, $00, $00, $01, $02, $0A, $03, $02, $01, $00, $00, $00, $00

ANIM10:
   DC.B  $00, $00, $00, $00, $00, $01, $0A, $03, $01, $00, $00, $00, $00, $00

ANIM_PIXELS:
   DC.B  $80, $80, $80, $80, $80, $80, $80, $80, $80, $80, $80, $80, $80, $80
   DC.B  $C0, $AA, $D5, $AA, $D5, $AA, $D5, $AA, $D5, $AA, $D5, $AA, $D5, $80
   DC.B  $90, $80, $80, $80, $80, $80, $80, $80, $80, $80, $80, $80, $80, $82
   DC.B  $90, $AA, $D1, $A2, $D5, $A8, $85, $A8, $C5, $A2, $D4, $A2, $95, $82
   DC.B  $90, $82, $91, $A2, $C5, $A8, $80, $88, $C5, $A2, $94, $A0, $90, $82
   DC.B  $90, $82, $90, $A2, $C4, $A8, $80, $88, $C5, $A2, $94, $A0, $90, $82
   DC.B  $90, $82, $90, $A2, $C4, $A8, $81, $88, $C4, $A2, $D4, $A0, $95, $82
   DC.B  $90, $A2, $D1, $A2, $C4, $88, $80, $88, $C4, $A2, $84, $A0, $85, $82
   DC.B  $90, $82, $91, $A2, $C4, $88, $80, $88, $C4, $AA, $84, $A0, $85, $82
   DC.B  $90, $82, $91, $A2, $C4, $88, $80, $88, $C4, $8A, $84, $A0, $91, $82
   DC.B  $90, $AA, $91, $A2, $C4, $A8, $85, $A8, $85, $82, $D4, $A2, $91, $82

SPIN_ORDER:
   DC.B  <ANIM5
   DC.B  <ANIM4
   DC.B  <ANIM3
   DC.B  <ANIM2
   DC.B  <ANIM1
   DC.B  <ANIM0
   DC.B  <ANIM1
   DC.B  <ANIM2
   DC.B  <ANIM3
   DC.B  <ANIM4
   DC.B  <ANIM5
   DC.B  <ANIM10
   DC.B  <ANIM9
   DC.B  <ANIM8
   DC.B  <ANIM7
   DC.B  <ANIM6
   DC.B  <ANIM7
   DC.B  <ANIM8
   DC.B  <ANIM9
   DC.B  <ANIM10
   DC.B  >ANIM5
   DC.B  >ANIM4
   DC.B  >ANIM3
   DC.B  >ANIM2
   DC.B  >ANIM1
   DC.B  >ANIM0
   DC.B  >ANIM1
   DC.B  >ANIM2
   DC.B  >ANIM3
   DC.B  >ANIM4
   DC.B  >ANIM5
   DC.B  >ANIM10
   DC.B  >ANIM9
   DC.B  >ANIM8
   DC.B  >ANIM7
   DC.B  >ANIM6
   DC.B  >ANIM7
   DC.B  >ANIM8
   DC.B  >ANIM9
   DC.B  >ANIM10

__zp_init:
   SUBROUTINE

   LDA   #$00
   STA   wipe_dir
   STA   row_minus_x
   STA   row_minus_x+1
   STA   row_minus_y
   STA   row_minus_y+1
   STA   row_plus_y
   STA   row_plus_y+1
   STA   row_plus_x
   STA   row_plus_x+1
   LDA   #$9D
   STA   anim_count
   RTS
