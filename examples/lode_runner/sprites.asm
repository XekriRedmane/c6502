PLAYER_COL	EQU	$00
PLAYER_ROW	EQU	$01
PLAYER_X_ADJ	EQU	$02
PLAYER_Y_ADJ	EQU	$03
PLAYER_ANIM_STATE	EQU	$04
PLAYER_FACING_DIRECTION	EQU	$05
HARGS	EQU	$06
SSP	EQU	$10
GUARD_LOC_COL	EQU	$12
GUARD_LOC_ROW	EQU	$13
GUARD_ANIM_STATE	EQU	$14
GUARD_FACING_DIRECTION	EQU	$15
GUARD_GOLD_TIMER	EQU	$16
GUARD_X_ADJ	EQU	$17
GUARD_Y_ADJ	EQU	$18
GUARD_NUM	EQU	$19
FP	EQU	$1A
DPTR	EQU	$1C
shifted_pixels@ret	EQU	$1E
__dos_zp	EQU	$26
GUARD_RESURRECT_COL	EQU	$53
NOTE_INDEX	EQU	$54
GUARD_GOLD_TIMER_START_VALUE	EQU	$5F
GUARD_PATTERNS	EQU	$60
GUARD_PHASE	EQU	$64
PADDLE_VALUES	EQU	$65
__zpabi_access_compressed_level_data__command	EQU	$80
__zpabi_access_hi_score_data_from_disk__command	EQU	$80
__zpabi_add_and_update_score__bcd_points_0	EQU	$80
__zpabi_append_note__pitch	EQU	$80
__zpabi_bcd_to_decimal2__bcd	EQU	$80
__zpabi_char_to_sprite_num__c	EQU	$80
__zpabi_draw_guard_sprite__sprite	EQU	$80
__zpabi_draw_sprite__page	EQU	$80
__zpabi_draw_sprite_at_pixel_coords__half_col	EQU	$80
__zpabi_erase_sprite_at_pixel_coords__half_col	EQU	$80
__zpabi_half_screen_col_for__col	EQU	$80
__zpabi_load_level__verbatim	EQU	$80
__zpabi_load_sound_data__notes_0	EQU	$80
__zpabi_play_note__pitch	EQU	$80
__zpabi_put_char__c	EQU	$80
__zpabi_put_digit__digit	EQU	$80
__zpabi_put_string__s_0	EQU	$80
__zpabi_screen_row_for__row	EQU	$80
__zpabi_sound_delay1__count	EQU	$80
__zpabi_sound_delay__index	EQU	$80
__zpabi_to_decimal3__n	EQU	$80
__zpabi_wait_for_key__cursor	EQU	$80
__zpabi_wait_for_key_with_cursor_page_1__cursor	EQU	$80
__zpabi_add_and_update_score__bcd_points_1	EQU	$81
__zpabi_append_note__duration	EQU	$81
__zpabi_draw_sprite_at_pixel_coords__row	EQU	$81
__zpabi_erase_sprite_at_pixel_coords__row	EQU	$81
__zpabi_half_screen_col_for__x_adj	EQU	$81
__zpabi_load_sound_data__notes_1	EQU	$81
__zpabi_play_note__duration	EQU	$81
__zpabi_put_string__s_1	EQU	$81
__zpabi_screen_row_for__y_adj	EQU	$81
__zpabi_compute_shifted_sprite__sprite	EQU	$82
__zpabi_draw_sprite__sprite	EQU	$82
__zpabi_draw_sprite_at_pixel_coords__sprite	EQU	$82
__zpabi_draw_sprite_page1__sprite	EQU	$82
__zpabi_draw_sprite_page2__sprite	EQU	$82
__zpabi_erase_sprite_at_pixel_coords__sprite	EQU	$82
__zpabi_play_note__y	EQU	$82
__zpabi_compute_shifted_sprite__shift	EQU	$83
__zpabi_shifted_pixels__b	EQU	$84
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
__zpabi_row_to_addr__row	EQU	$AE
__zpabi_shifted_pixels__table_0	EQU	$AE
__zpabi_row_to_addr__page	EQU	$AF
__zpabi_shifted_pixels__table_1	EQU	$AF
__local_draw_sprite__0	EQU	$B0
__local_draw_sprite_at_pixel_coords__0	EQU	$B0
__local_erase_sprite_at_pixel_coords__0	EQU	$B0
__local_draw_sprite__1	EQU	$B1
__local_draw_sprite_at_pixel_coords__1	EQU	$B1
__local_erase_sprite_at_pixel_coords__1	EQU	$B1
__local_draw_sprite__2	EQU	$B2
__local_draw_sprite_at_pixel_coords__2	EQU	$B2
__local_draw_sprite__3	EQU	$B3
__local_draw_sprite_at_pixel_coords__3	EQU	$B3
__local_erase_sprite_at_pixel_coords__3	EQU	$B3
__local_draw_sprite__i	EQU	$B4
__local_draw_sprite_at_pixel_coords__4	EQU	$B4
__local_erase_sprite_at_pixel_coords__4	EQU	$B4
__local_draw_sprite__row	EQU	$B5
__local_draw_sprite_at_pixel_coords__b	EQU	$B5
__local_erase_sprite_at_pixel_coords__5	EQU	$B5
__local_draw_sprite__4	EQU	$B6
__local_draw_sprite_at_pixel_coords__5	EQU	$B6
__local_erase_sprite_at_pixel_coords__b	EQU	$B6
__local_draw_sprite__5	EQU	$B7
__local_draw_sprite_at_pixel_coords__6	EQU	$B7
__local_erase_sprite_at_pixel_coords__6	EQU	$B7
__local_draw_sprite__6	EQU	$B8
__local_erase_sprite_at_pixel_coords__7	EQU	$B8
__local_draw_sprite_at_pixel_coords__i	EQU	$B9
__local_draw_sprite_at_pixel_coords__screens_differ	EQU	$BA
__local_erase_sprite_at_pixel_coords__i	EQU	$BA
__local_draw_sprite_at_pixel_coords__7	EQU	$BB
__local_erase_sprite_at_pixel_coords__8	EQU	$BB
__local_draw_sprite_at_pixel_coords__8	EQU	$BC
__local_erase_sprite_at_pixel_coords__9	EQU	$BC
__local_compute_shifted_sprite__0	EQU	$BF
__local_compute_shifted_sprite__1	EQU	$C0
__local_compute_shifted_sprite__2	EQU	$C1
__local_compute_shifted_sprite__3	EQU	$C2
__local_compute_shifted_sprite__4	EQU	$C3
__local_compute_shifted_sprite__i	EQU	$C4
__local_compute_shifted_sprite__data	EQU	$C5
__local_compute_shifted_sprite__5	EQU	$C6
__local_shifted_pixels__high	EQU	$C9
__local_shifted_pixels__2	EQU	$CA
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
; def compute_shifted_sprite params=__zpabi_compute_shifted_sprite__sprite,__zpabi_compute_shifted_sprite__shift locals=__local_compute_shifted_sprite__0,__local_compute_shifted_sprite__1,__local_compute_shifted_sprite__2,__local_compute_shifted_sprite__3,__local_compute_shifted_sprite__4,__local_compute_shifted_sprite__i,__local_compute_shifted_sprite__data,__local_compute_shifted_sprite__5 indirect=false in_cycle=false
; def draw_sprite params=__zpabi_draw_sprite__sprite,__zpabi_draw_sprite__page locals=__local_draw_sprite__0,__local_draw_sprite__1,__local_draw_sprite__2,__local_draw_sprite__3,__local_draw_sprite__i,__local_draw_sprite__row,__local_draw_sprite__4,__local_draw_sprite__5,__local_draw_sprite__6 indirect=false in_cycle=false
; def draw_sprite_at_pixel_coords params=__zpabi_draw_sprite_at_pixel_coords__sprite,__zpabi_draw_sprite_at_pixel_coords__half_col,__zpabi_draw_sprite_at_pixel_coords__row locals=__local_draw_sprite_at_pixel_coords__0,__local_draw_sprite_at_pixel_coords__1,__local_draw_sprite_at_pixel_coords__2,__local_draw_sprite_at_pixel_coords__3,__local_draw_sprite_at_pixel_coords__4,__local_draw_sprite_at_pixel_coords__b,__local_draw_sprite_at_pixel_coords__5,__local_draw_sprite_at_pixel_coords__6,__local_draw_sprite_at_pixel_coords__row,__local_draw_sprite_at_pixel_coords__i,__local_draw_sprite_at_pixel_coords__screens_differ,__local_draw_sprite_at_pixel_coords__7,__local_draw_sprite_at_pixel_coords__8,__local_draw_sprite_at_pixel_coords__9,__local_draw_sprite_at_pixel_coords__10 indirect=false in_cycle=false changes=__zpabi_draw_sprite_at_pixel_coords__row
; def draw_sprite_page1 params=__zpabi_draw_sprite_page1__sprite locals= indirect=false in_cycle=false
; def draw_sprite_page2 params=__zpabi_draw_sprite_page2__sprite locals= indirect=false in_cycle=false
; def erase_sprite_at_pixel_coords params=__zpabi_erase_sprite_at_pixel_coords__sprite,__zpabi_erase_sprite_at_pixel_coords__half_col,__zpabi_erase_sprite_at_pixel_coords__row locals=__local_erase_sprite_at_pixel_coords__0,__local_erase_sprite_at_pixel_coords__1,__local_erase_sprite_at_pixel_coords__2,__local_erase_sprite_at_pixel_coords__3,__local_erase_sprite_at_pixel_coords__4,__local_erase_sprite_at_pixel_coords__5,__local_erase_sprite_at_pixel_coords__b,__local_erase_sprite_at_pixel_coords__6,__local_erase_sprite_at_pixel_coords__7,__local_erase_sprite_at_pixel_coords__row,__local_erase_sprite_at_pixel_coords__i,__local_erase_sprite_at_pixel_coords__8,__local_erase_sprite_at_pixel_coords__9,__local_erase_sprite_at_pixel_coords__10 indirect=false in_cycle=false changes=__zpabi_erase_sprite_at_pixel_coords__row
; def half_screen_col_for params=__zpabi_half_screen_col_for__col,__zpabi_half_screen_col_for__x_adj locals=__local_half_screen_col_for__0,__local_half_screen_col_for__1 indirect=false in_cycle=false
; def screen_row_for params=__zpabi_screen_row_for__row,__zpabi_screen_row_for__y_adj locals=__local_screen_row_for__0,__local_screen_row_for__1 indirect=false in_cycle=false
; def shifted_pixels params=__zpabi_shifted_pixels__table_0,__zpabi_shifted_pixels__table_1,__zpabi_shifted_pixels__b locals=__local_shifted_pixels__0,__local_shifted_pixels__1,__local_shifted_pixels__high,__local_shifted_pixels__2 indirect=false in_cycle=false
; ext DSKFORM params=
; ext RWTS params=__zpabi_RWTS__iob_0,__zpabi_RWTS__iob_1 param_regs=Y,A carry=true
; ext SEEKABS params=__zpabi_SEEKABS__track param_regs=A
; ext access_compressed_level_data params=__zpabi_access_compressed_level_data__command
; ext access_disk_or_reset_game params=
; ext access_hi_score_data_from_disk params=__zpabi_access_hi_score_data_from_disk__command
; ext add_and_update_score params=__zpabi_add_and_update_score__bcd_points_0,__zpabi_add_and_update_score__bcd_points_1
; ext append_note params=__zpabi_append_note__pitch,__zpabi_append_note__duration
; ext bad_data_disk params=
; ext bcd_to_decimal2 params=__zpabi_bcd_to_decimal2__bcd
; ext beep params=
; ext char_to_sprite_num params=__zpabi_char_to_sprite_num__c
; ext check_for_input params=
; ext clear_hgr1 params=
; ext clear_hgr2 params=
; ext cold_start params=
; ext compress_and_save_level_data params=
; ext detect_lack_of_joystick params=
; ext draw_guard_sprite params=__zpabi_draw_guard_sprite__sprite
; ext editor_command_loop params=
; ext enable_next_level_ladders params=
; ext erase_guard params=
; ext handle_timers params=
; ext hi_score_screen params=
; ext init_game_data params=
; ext iris_wipe params=
; ext level_editor params=
; ext load_guard_data params=
; ext load_level params=__zpabi_load_level__verbatim
; ext load_sound_data params=__zpabi_load_sound_data__notes_0,__zpabi_load_sound_data__notes_1
; ext long_delay params=
; ext move_guards params=
; ext move_player params=
; ext newline params=
; ext play_note params=__zpabi_play_note__pitch,__zpabi_play_note__duration,__zpabi_play_note__y
; ext play_sound params= carry=true
; ext put_char params=__zpabi_put_char__c
; ext put_digit params=__zpabi_put_digit__digit
; ext put_status params=
; ext put_status_draw params=
; ext put_status_level params=
; ext put_status_lives params=
; ext put_string params=__zpabi_put_string__s_0,__zpabi_put_string__s_1
; ext record_hi_score_data_to_disk params=
; ext reset_game params=
; ext row_to_addr params=__zpabi_row_to_addr__row,__zpabi_row_to_addr__page
; ext sound_delay params=__zpabi_sound_delay__index
; ext sound_delay1 params=__zpabi_sound_delay1__count
; ext spinning_game_over params= carry=true
; ext start_level_editor params=
; ext to_decimal3 params=__zpabi_to_decimal3__n
; ext wait_for_key params=__zpabi_wait_for_key__cursor
; ext wait_for_key_with_cursor_page_1 params=__zpabi_wait_for_key_with_cursor_page_1__cursor
; ext wait_key params=
; call compute_shifted_sprite -> shifted_pixels
; call draw_sprite -> compute_shifted_sprite
; call draw_sprite -> row_to_addr
; call draw_sprite_at_pixel_coords -> compute_shifted_sprite
; call draw_sprite_at_pixel_coords -> row_to_addr
; call draw_sprite_page1 -> draw_sprite
; call draw_sprite_page2 -> draw_sprite
; call erase_sprite_at_pixel_coords -> compute_shifted_sprite
; call erase_sprite_at_pixel_coords -> row_to_addr
; share member=compute_shifted_sprite:__zpabi_compute_shifted_sprite__sprite member=draw_sprite:__zpabi_draw_sprite__sprite member=draw_sprite_at_pixel_coords:__zpabi_draw_sprite_at_pixel_coords__sprite member=draw_sprite_page1:__zpabi_draw_sprite_page1__sprite member=draw_sprite_page2:__zpabi_draw_sprite_page2__sprite member=erase_sprite_at_pixel_coords:__zpabi_erase_sprite_at_pixel_coords__sprite forward=draw_sprite>compute_shifted_sprite forward=draw_sprite_at_pixel_coords>compute_shifted_sprite forward=draw_sprite_page1>draw_sprite forward=draw_sprite_page2>draw_sprite forward=erase_sprite_at_pixel_coords>compute_shifted_sprite
; share member=compute_shifted_sprite:__zpabi_compute_shifted_sprite__shift holder=draw_sprite holder=draw_sprite_at_pixel_coords holder=erase_sprite_at_pixel_coords live=draw_sprite:compute_shifted_sprite,row_to_addr live=draw_sprite_at_pixel_coords:compute_shifted_sprite live=erase_sprite_at_pixel_coords:compute_shifted_sprite forward=draw_sprite>compute_shifted_sprite forward=draw_sprite_at_pixel_coords>compute_shifted_sprite forward=erase_sprite_at_pixel_coords>compute_shifted_sprite writer=draw_sprite writer=draw_sprite_at_pixel_coords writer=erase_sprite_at_pixel_coords
; share member=shifted_pixels:__zpabi_shifted_pixels__table_0,__zpabi_shifted_pixels__table_1 holder=compute_shifted_sprite live=compute_shifted_sprite:shifted_pixels forward=compute_shifted_sprite>shifted_pixels writer=compute_shifted_sprite
; item shifted_pixels section=.text local=true zero=false
; item compute_shifted_sprite section=.text local=true zero=false
; item draw_sprite section=.text local=true zero=false
; item draw_sprite_page1 section=.text local=false zero=false
; item draw_sprite_page2 section=.text local=false zero=false
; item erase_sprite_at_pixel_coords section=.text local=false zero=false
; item erase_sprite_at_pixel_coords@run1 section=.text local=true zero=false
; item erase_sprite_at_pixel_coords@run0 section=.text local=true zero=false
; item draw_sprite_at_pixel_coords section=.text local=false zero=false
; item screen_row_for section=.text local=false zero=false
; item half_screen_col_for section=.text local=false zero=false
; item PIXEL_MASK0 section=.rodata local=true zero=false
; item PIXEL_MASK1 section=.rodata local=true zero=false
; item ROW_OFFSET_TABLE section=.rodata local=true zero=false
; item COL_OFFSET_TABLE section=.rodata local=true zero=false
; zp PLAYER_COL size=1 at=$00 def=false init=
; zp PLAYER_ROW size=1 at=$01 def=false init=
; zp PLAYER_X_ADJ size=1 at=$02 def=false init=
; zp PLAYER_Y_ADJ size=1 at=$03 def=false init=
; zp PLAYER_ANIM_STATE size=1 at=$04 def=false init=
; zp PLAYER_FACING_DIRECTION size=1 at=$05 def=false init=
; zp GUARD_LOC_COL size=1 at=$12 def=false init=
; zp GUARD_LOC_ROW size=1 at=$13 def=false init=
; zp GUARD_ANIM_STATE size=1 at=$14 def=false init=
; zp GUARD_FACING_DIRECTION size=1 at=$15 def=false init=
; zp GUARD_GOLD_TIMER size=1 at=$16 def=false init=
; zp GUARD_X_ADJ size=1 at=$17 def=false init=
; zp GUARD_Y_ADJ size=1 at=$18 def=false init=
; zp GUARD_NUM size=1 at=$19 def=false init=
; zp shifted_pixels@ret size=2 at=auto def=true init= local=true
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
; runtime hargs_size=10 hargs_need=0
; @zp-link-meta-end

shifted_pixels:
   SUBROUTINE

   LDA   __zpabi_shifted_pixels__table_0
   CLC
   ADC   #$80
   STA   __local_shifted_pixels__high
   LDA   __zpabi_shifted_pixels__table_1
   ADC   #$00
   STA   __local_shifted_pixels__2
   LDY   __zpabi_shifted_pixels__b
   LDA   (__zpabi_shifted_pixels__table_0),Y
   STA   shifted_pixels@ret
   LDA   (__local_shifted_pixels__high),Y
   STA   shifted_pixels@ret+1
   RTS

compute_shifted_sprite:
   SUBROUTINE

   LDA   #<PIXEL_SHIFT_TABLE
   CLC
   STA   __zpabi_shifted_pixels__table_0
   LDA   #>PIXEL_SHIFT_TABLE
   ADC   __zpabi_compute_shifted_sprite__shift
   STA   __zpabi_shifted_pixels__table_1
   LDA   __zpabi_compute_shifted_sprite__sprite
   CLC
   ADC   #<SPRITE_DATA
   STA   __local_compute_shifted_sprite__0
   LDA   #$00
   ADC   #>SPRITE_DATA
   STA   __local_compute_shifted_sprite__5
   LDA   __local_compute_shifted_sprite__0
   CLC
   STA   __local_compute_shifted_sprite__data
   BCC   .carry_inc@0
   INC   __local_compute_shifted_sprite__5
.carry_inc@0:
   LDA   #$00
   STA   __local_compute_shifted_sprite__i
.loop@0_start:
   CMP   #$21
   BCS   .loop@0_break
   LDY   #$00
   LDA   (__local_compute_shifted_sprite__data),Y
   STA   __zpabi_shifted_pixels__b
   JSR   shifted_pixels
   LDA   shifted_pixels@ret
   STA   __local_compute_shifted_sprite__1
   LDA   shifted_pixels@ret+1
   STA   __local_compute_shifted_sprite__2
   LDY   #$00
   LDA   (shifted_pixels@ret),Y
   LDX   __local_compute_shifted_sprite__i
   STA   BLOCK_DATA,X
   INY
   LDA   (__local_compute_shifted_sprite__1),Y
   STA   BLOCK_DATA+1,X
   LDY   #$68
   LDA   (__local_compute_shifted_sprite__data),Y
   STA   __zpabi_shifted_pixels__b
   JSR   shifted_pixels
   LDA   shifted_pixels@ret
   STA   __local_compute_shifted_sprite__3
   LDA   shifted_pixels@ret+1
   STA   __local_compute_shifted_sprite__4
   LDA   __local_compute_shifted_sprite__i
   CLC
   ADC   #$01
   STA   __local_compute_shifted_sprite__2
   CLC
   LDX   __local_compute_shifted_sprite__2
   LDA   BLOCK_DATA,X
   STA   __local_compute_shifted_sprite__1
   LDY   #$00
   LDA   (shifted_pixels@ret),Y
   ORA   __local_compute_shifted_sprite__1
   STA   BLOCK_DATA,X
   INY
   LDA   (__local_compute_shifted_sprite__3),Y
   LDX   __local_compute_shifted_sprite__i
   STA   BLOCK_DATA+2,X
   LDA   __local_compute_shifted_sprite__data
   ADC   #$D0
   STA   __local_compute_shifted_sprite__data
   BCC   .carry_inc@1
   INC   __local_compute_shifted_sprite__5
.carry_inc@1:
   TXA
   CLC
   ADC   #$03
   STA   __local_compute_shifted_sprite__i
   JMP   .loop@0_start
.loop@0_break:
   RTS

draw_sprite:
   SUBROUTINE

   LDX   GAME_ROWNUM
   LDA   SCREEN_ROW_TABLE,X
   STA   __local_draw_sprite__row
   LDX   GAME_COLNUM
   LDA   COL_BYTE_TABLE,X
   STA   __local_draw_sprite__6
   LDA   COL_SHIFT_TABLE,X
   STA   __zpabi_compute_shifted_sprite__shift
   LDX   __zpabi_compute_shifted_sprite__shift
   LDA   PIXEL_MASK0,X
   STA   __local_draw_sprite__5
   LDA   PIXEL_MASK1,X
   STA   __local_draw_sprite__4
   JSR   compute_shifted_sprite
   LDA   #$00
   STA   __local_draw_sprite__i
.loop@1_start:
   CMP   #$21
   BCC   .lb_skip@0
   JMP   .loop@1_break
.lb_skip@0:
   LDA   __local_draw_sprite__row
   STA   __zpabi_row_to_addr__row
   LDA   __zpabi_draw_sprite__page
   STA   __zpabi_row_to_addr__page
   JSR   row_to_addr
   STX   __local_draw_sprite__3
   CLC
   ADC   __local_draw_sprite__6
   STA   __local_draw_sprite__2
   BCC   .carry_inc@2
   INC   __local_draw_sprite__3
.carry_inc@2:
   LDY   #$00
   LDA   (__local_draw_sprite__2),Y
   STA   __local_draw_sprite__0
   LDX   __local_draw_sprite__i
   LDA   BLOCK_DATA,X
   STA   __local_draw_sprite__1
   LDA   __local_draw_sprite__0
   AND   __local_draw_sprite__5
   ORA   __local_draw_sprite__1
   STA   (__local_draw_sprite__2),Y
   LDA   __zpabi_compute_shifted_sprite__shift
   CMP   #$05
   BCS   .if_else@1
   INY
   LDA   (__local_draw_sprite__2),Y
   STA   __local_draw_sprite__0
   LDA   BLOCK_DATA+1,X
   STA   __local_draw_sprite__1
   LDA   __local_draw_sprite__0
   AND   __local_draw_sprite__4
   ORA   __local_draw_sprite__1
   STA   (__local_draw_sprite__2),Y
   JMP   .if_end@0
.if_else@1:
   LDA   BLOCK_DATA+1,X
   LDY   #$01
   STA   (__local_draw_sprite__2),Y
   CLC
   INY
   LDA   (__local_draw_sprite__2),Y
   STA   __local_draw_sprite__0
   LDA   BLOCK_DATA+2,X
   STA   __local_draw_sprite__1
   LDA   __local_draw_sprite__0
   AND   __local_draw_sprite__4
   ORA   __local_draw_sprite__1
   STA   (__local_draw_sprite__2),Y
.if_end@0:
   INC   __local_draw_sprite__row
   TXA
   ADC   #$03
   STA   __local_draw_sprite__i
   JMP   .loop@1_start
.loop@1_break:
   RTS

draw_sprite_page1:
   SUBROUTINE

   LDA   #$20
   STA   __zpabi_draw_sprite__page
   JMP   draw_sprite

draw_sprite_page2:
   SUBROUTINE

   LDA   #$40
   STA   __zpabi_draw_sprite__page
   JMP   draw_sprite

erase_sprite_at_pixel_coords:
   SUBROUTINE

   LDX   __zpabi_erase_sprite_at_pixel_coords__half_col
   LDA   HALF_SCREEN_COL_BYTE_TABLE,X
   STA   __local_erase_sprite_at_pixel_coords__9
   JSR   erase_sprite_at_pixel_coords@run0
   BCS   .cond_else@2
   LDA   #$02
   STA   __local_erase_sprite_at_pixel_coords__8
   JMP   .cond_end@3
.cond_else@2:
   LDA   #$03
   STA   __local_erase_sprite_at_pixel_coords__8
.cond_end@3:
   LDA   #$00
   STA   __local_erase_sprite_at_pixel_coords__i
.loop@2_start:
   CMP   #$21
   BCS   .loop@2_break
   LDA   __zpabi_erase_sprite_at_pixel_coords__row
   JSR   erase_sprite_at_pixel_coords@run1
   STA   __local_erase_sprite_at_pixel_coords__7
   STX   __local_erase_sprite_at_pixel_coords__6
   LDA   #$00
   STA   __local_erase_sprite_at_pixel_coords__b
   JMP   .loop@3_start
.erase_sprite_at_pixel_coords@rot@0:
   LDA   __local_erase_sprite_at_pixel_coords__9
   ADC   __local_erase_sprite_at_pixel_coords__b
   CLC
   ADC   __local_erase_sprite_at_pixel_coords__7
   STA   __local_erase_sprite_at_pixel_coords__4
   LDA   __local_erase_sprite_at_pixel_coords__6
   ADC   #$00
   STA   __local_erase_sprite_at_pixel_coords__5
   LDY   #$00
   LDA   (__local_erase_sprite_at_pixel_coords__4),Y
   STA   __local_erase_sprite_at_pixel_coords__3
   LDA   __local_erase_sprite_at_pixel_coords__i
   CLC
   ADC   __local_erase_sprite_at_pixel_coords__b
   TAX
   LDA   __local_erase_sprite_at_pixel_coords__4
   STA   __local_erase_sprite_at_pixel_coords__0
   LDA   __local_erase_sprite_at_pixel_coords__5
   CLC
   ADC   #$20
   STA   __local_erase_sprite_at_pixel_coords__1
   LDA   (__local_erase_sprite_at_pixel_coords__0),Y
   STA   __local_erase_sprite_at_pixel_coords__1
   LDA   BLOCK_DATA,X
   EOR   #$7F
   AND   __local_erase_sprite_at_pixel_coords__3
   ORA   __local_erase_sprite_at_pixel_coords__1
   STA   (__local_erase_sprite_at_pixel_coords__4),Y
   INC   __local_erase_sprite_at_pixel_coords__b
.loop@3_start:
   LDA   __local_erase_sprite_at_pixel_coords__b
   CMP   __local_erase_sprite_at_pixel_coords__8
   BCC   .erase_sprite_at_pixel_coords@rot@0
   INC   __zpabi_erase_sprite_at_pixel_coords__row
   LDA   __local_erase_sprite_at_pixel_coords__i
   CLC
   ADC   #$03
   STA   __local_erase_sprite_at_pixel_coords__i
   JMP   .loop@2_start
.loop@2_break:
   RTS

erase_sprite_at_pixel_coords@run1:
   SUBROUTINE

   STA   __zpabi_row_to_addr__row
   LDA   #$20
   STA   __zpabi_row_to_addr__page
   JMP   row_to_addr

erase_sprite_at_pixel_coords@run0:
   SUBROUTINE

   LDA   HALF_SCREEN_COL_SHIFT_TABLE,X
   STA   __zpabi_compute_shifted_sprite__shift
   JSR   compute_shifted_sprite
   LDA   __zpabi_compute_shifted_sprite__shift
   CMP   #$05
   RTS

draw_sprite_at_pixel_coords:
   SUBROUTINE

   LDX   __zpabi_draw_sprite_at_pixel_coords__half_col
   LDA   HALF_SCREEN_COL_BYTE_TABLE,X
   STA   __local_draw_sprite_at_pixel_coords__8
   JSR   erase_sprite_at_pixel_coords@run0
   BCS   .cond_else@4
   LDA   #$02
   STA   __local_draw_sprite_at_pixel_coords__7
   JMP   .cond_end@5
.cond_else@4:
   LDA   #$03
   STA   __local_draw_sprite_at_pixel_coords__7
.cond_end@5:
   LDA   #$00
   STA   __local_draw_sprite_at_pixel_coords__screens_differ
   STA   __local_draw_sprite_at_pixel_coords__i
.loop@4_start:
   CMP   #$21
   BCC   .lb_skip@1
   JMP   .loop@4_break
.lb_skip@1:
   LDA   __zpabi_draw_sprite_at_pixel_coords__row
   JSR   erase_sprite_at_pixel_coords@run1
   STA   __local_draw_sprite_at_pixel_coords__6
   STX   __local_draw_sprite_at_pixel_coords__5
   LDA   #$00
   STA   __local_draw_sprite_at_pixel_coords__b
   JMP   .loop@5_start
.draw_sprite_at_pixel_coords@rot@1:
   LDA   __local_draw_sprite_at_pixel_coords__8
   ADC   __local_draw_sprite_at_pixel_coords__b
   CLC
   ADC   __local_draw_sprite_at_pixel_coords__6
   STA   __local_draw_sprite_at_pixel_coords__3
   LDA   __local_draw_sprite_at_pixel_coords__5
   ADC   #$00
   STA   __local_draw_sprite_at_pixel_coords__4
   LDA   __local_draw_sprite_at_pixel_coords__3
   STA   __local_draw_sprite_at_pixel_coords__0
   LDY   #$00
   LDA   (__local_draw_sprite_at_pixel_coords__3),Y
   STA   __local_draw_sprite_at_pixel_coords__2
   LDA   __local_draw_sprite_at_pixel_coords__4
   CLC
   ADC   #$20
   STA   __local_draw_sprite_at_pixel_coords__1
   LDA   (__local_draw_sprite_at_pixel_coords__0),Y
   STA   __local_draw_sprite_at_pixel_coords__0
   LDA   __local_draw_sprite_at_pixel_coords__i
   CLC
   ADC   __local_draw_sprite_at_pixel_coords__b
   TAX
   LDA   BLOCK_DATA,X
   STA   __local_draw_sprite_at_pixel_coords__1
   LDA   __local_draw_sprite_at_pixel_coords__2
   EOR   __local_draw_sprite_at_pixel_coords__0
   AND   __local_draw_sprite_at_pixel_coords__1
   ORA   __local_draw_sprite_at_pixel_coords__screens_differ
   STA   __local_draw_sprite_at_pixel_coords__screens_differ
   LDA   __local_draw_sprite_at_pixel_coords__2
   ORA   __local_draw_sprite_at_pixel_coords__1
   STA   (__local_draw_sprite_at_pixel_coords__3),Y
   INC   __local_draw_sprite_at_pixel_coords__b
.loop@5_start:
   LDA   __local_draw_sprite_at_pixel_coords__b
   CMP   __local_draw_sprite_at_pixel_coords__7
   BCC   .draw_sprite_at_pixel_coords@rot@1
   INC   __zpabi_draw_sprite_at_pixel_coords__row
   LDA   __local_draw_sprite_at_pixel_coords__i
   CLC
   ADC   #$03
   STA   __local_draw_sprite_at_pixel_coords__i
   JMP   .loop@4_start
.loop@4_break:
   LDA   __local_draw_sprite_at_pixel_coords__screens_differ
   RTS

screen_row_for:
   SUBROUTINE

   LDX   __zpabi_screen_row_for__row
   LDA   SCREEN_ROW_TABLE,X
   LDX   __zpabi_screen_row_for__y_adj
   CLC
   ADC   ROW_OFFSET_TABLE,X
   RTS

half_screen_col_for:
   SUBROUTINE

   LDX   __zpabi_half_screen_col_for__col
   LDA   HALF_SCREEN_COL_TABLE,X
   LDX   __zpabi_half_screen_col_for__x_adj
   CLC
   ADC   COL_OFFSET_TABLE,X
   RTS

PIXEL_MASK0:
   DC.B  $00, $01, $03, $07, $0F, $1F, $3F

PIXEL_MASK1:
   DC.B  $F8, $F0, $E0, $C0, $80, $FE, $FC

ROW_OFFSET_TABLE:
   DC.B  $FB, $FD, $00, $02, $04

COL_OFFSET_TABLE:
   DC.B  $FE, $FF, $00, $01, $02
