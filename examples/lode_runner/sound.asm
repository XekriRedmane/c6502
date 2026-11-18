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
__zpabi_access_compressed_level_data__command	EQU	$80
__zpabi_access_hi_score_data_from_disk__command	EQU	$80
__zpabi_add_and_update_score__bcd_points_0	EQU	$80
__zpabi_bcd_to_decimal2__bcd	EQU	$80
__zpabi_char_to_sprite_num__c	EQU	$80
__zpabi_draw_guard_sprite__sprite	EQU	$80
__zpabi_draw_sprite_at_pixel_coords__sprite	EQU	$80
__zpabi_draw_sprite_page1__sprite	EQU	$80
__zpabi_draw_sprite_page2__sprite	EQU	$80
__zpabi_erase_sprite_at_pixel_coords__sprite	EQU	$80
__zpabi_half_screen_col_for__col	EQU	$80
__zpabi_load_level__verbatim	EQU	$80
__zpabi_load_sound_data__notes_0	EQU	$80
__zpabi_play_note__pitch	EQU	$80
__zpabi_put_char__c	EQU	$80
__zpabi_put_digit__digit	EQU	$80
__zpabi_put_string__s_0	EQU	$80
__zpabi_row_to_addr__row	EQU	$80
__zpabi_screen_row_for__row	EQU	$80
__zpabi_sound_delay__index	EQU	$80
__zpabi_to_decimal3__n	EQU	$80
__zpabi_wait_for_key__cursor	EQU	$80
__zpabi_wait_for_key_with_cursor_page_1__cursor	EQU	$80
__zpabi_add_and_update_score__bcd_points_1	EQU	$81
__zpabi_draw_sprite_at_pixel_coords__half_col	EQU	$81
__zpabi_erase_sprite_at_pixel_coords__half_col	EQU	$81
__zpabi_half_screen_col_for__x_adj	EQU	$81
__zpabi_load_sound_data__notes_1	EQU	$81
__zpabi_play_note__duration	EQU	$81
__zpabi_put_string__s_1	EQU	$81
__zpabi_row_to_addr__page	EQU	$81
__zpabi_screen_row_for__y_adj	EQU	$81
__zpabi_sound_delay1__count	EQU	$81
__zpabi_append_note__pitch	EQU	$82
__zpabi_draw_sprite_at_pixel_coords__row	EQU	$82
__zpabi_erase_sprite_at_pixel_coords__row	EQU	$82
__zpabi_play_note__y	EQU	$82
__local_play_sound__0	EQU	$83
__zpabi_append_note__duration	EQU	$83
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
__local_play_note__duration	EQU	$AE
__local_load_sound_data__notes	EQU	$AF
__local_play_note__y	EQU	$AF
__local_load_sound_data__1	EQU	$B0
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
; def append_note params=__zpabi_append_note__pitch,__zpabi_append_note__duration locals= indirect=false in_cycle=false
; def beep params= locals=__local_beep__x,__local_beep__n indirect=false in_cycle=false
; def load_sound_data params=__zpabi_load_sound_data__notes_0,__zpabi_load_sound_data__notes_1 locals=__local_load_sound_data__0,__local_load_sound_data__notes,__local_load_sound_data__1 indirect=false in_cycle=false
; def play_note params=__zpabi_play_note__pitch,__zpabi_play_note__duration,__zpabi_play_note__y locals=__local_play_note__duration,__local_play_note__y,__local_play_note__x indirect=false in_cycle=false changes=__zpabi_play_note__duration
; def play_sound params= locals=__local_play_sound__0,__local_play_sound__1 indirect=false in_cycle=false carry=true
; def sound_delay params=__zpabi_sound_delay__index locals=__local_sound_delay__0 indirect=false in_cycle=false
; def sound_delay1 params=__zpabi_sound_delay1__count locals=__local_sound_delay1__y,__local_sound_delay1__count indirect=false in_cycle=false
; ext DSKFORM params=
; ext RWTS params=__zpabi_RWTS__iob_0,__zpabi_RWTS__iob_1 param_regs=Y,A carry=true
; ext SEEKABS params=__zpabi_SEEKABS__track param_regs=A
; ext access_compressed_level_data params=__zpabi_access_compressed_level_data__command
; ext access_disk_or_reset_game params=
; ext access_hi_score_data_from_disk params=__zpabi_access_hi_score_data_from_disk__command
; ext add_and_update_score params=__zpabi_add_and_update_score__bcd_points_0,__zpabi_add_and_update_score__bcd_points_1
; ext bad_data_disk params=
; ext bcd_to_decimal2 params=__zpabi_bcd_to_decimal2__bcd
; ext char_to_sprite_num params=__zpabi_char_to_sprite_num__c
; ext check_for_input params=
; ext clear_hgr1 params=
; ext clear_hgr2 params=
; ext cold_start params=
; ext compress_and_save_level_data params=
; ext detect_lack_of_joystick params=
; ext draw_guard_sprite params=__zpabi_draw_guard_sprite__sprite
; ext draw_sprite_at_pixel_coords params=__zpabi_draw_sprite_at_pixel_coords__sprite,__zpabi_draw_sprite_at_pixel_coords__half_col,__zpabi_draw_sprite_at_pixel_coords__row
; ext draw_sprite_page1 params=__zpabi_draw_sprite_page1__sprite
; ext draw_sprite_page2 params=__zpabi_draw_sprite_page2__sprite
; ext editor_command_loop params=
; ext enable_next_level_ladders params=
; ext erase_guard params=
; ext erase_sprite_at_pixel_coords params=__zpabi_erase_sprite_at_pixel_coords__sprite,__zpabi_erase_sprite_at_pixel_coords__half_col,__zpabi_erase_sprite_at_pixel_coords__row
; ext half_screen_col_for params=__zpabi_half_screen_col_for__col,__zpabi_half_screen_col_for__x_adj
; ext handle_timers params=
; ext hi_score_screen params=
; ext init_game_data params=
; ext iris_wipe params=
; ext level_editor params=
; ext load_guard_data params=
; ext load_level params=__zpabi_load_level__verbatim
; ext long_delay params=
; ext move_guards params=
; ext move_player params=
; ext newline params=
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
; ext screen_row_for params=__zpabi_screen_row_for__row,__zpabi_screen_row_for__y_adj
; ext spinning_game_over params= carry=true
; ext start_level_editor params=
; ext to_decimal3 params=__zpabi_to_decimal3__n
; ext wait_for_key params=__zpabi_wait_for_key__cursor
; ext wait_for_key_with_cursor_page_1 params=__zpabi_wait_for_key_with_cursor_page_1__cursor
; ext wait_key params=
; call load_sound_data -> append_note
; call play_sound -> play_note
; call play_sound -> sound_delay
; call play_sound -> sound_delay1
; call sound_delay -> sound_delay1
; share member=play_note:__zpabi_play_note__y holder=play_sound live=play_sound:play_note writer=play_sound
; item sound_delay section=.text local=false zero=false
; item sound_delay1 section=.text local=false zero=false
; item beep section=.text local=false zero=false
; item play_note section=.text local=false zero=false
; item append_note section=.text local=false zero=false
; item load_sound_data section=.text local=false zero=false
; item play_sound section=.text local=false zero=false
; item SOUND_DELAY_AMOUNTS section=.rodata local=true zero=false
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

sound_delay:
   SUBROUTINE

   LDX   __zpabi_sound_delay__index
   LDA   SOUND_DELAY_AMOUNTS,X
   STA   __zpabi_sound_delay1__count
   JMP   sound_delay1

sound_delay1:
   SUBROUTINE

   LDX   __zpabi_sound_delay1__count
.loop@0_start:
   LDY   #$B4
.loop@1_keep_start:
   DEY
   BNE   .loop@1_keep_start
   DEX
   BNE   .loop@0_start
   RTS

beep:
   SUBROUTINE

   LDY   #$C0
.beep@rot@0:
   LDX   #$80
.loop@3_keep_start:
   DEX
   BNE   .loop@3_keep_start
   LDA   ENABLE_SOUND
   BEQ   .if_end@0
   LDA   SPKR
.if_end@0:
   DEY
   BNE   .beep@rot@0
   RTS

play_note:
   SUBROUTINE

   LDA   __zpabi_play_note__duration
   STA   __local_play_note__duration
   LDA   __zpabi_play_note__y
   STA   __local_play_note__y
.loop@4_start:
   LDA   ENABLE_SOUND
   BEQ   .if_end@1
   LDA   SPKR
.if_end@1:
.loop@5_start:
   DEC   __local_play_note__y
   BNE   .if_end@4
   DEC   __local_play_note__duration
   BNE   .if_end@4
   RTS
.if_end@4:
   DEC   __zpabi_play_note__duration
   BNE   .loop@5_start
   LDA   __zpabi_play_note__pitch
   STA   __zpabi_play_note__duration
   JMP   .loop@4_start

append_note:
   SUBROUTINE

   INC   NOTE_INDEX
   LDX   NOTE_INDEX
   LDA   __zpabi_append_note__pitch
   STA   SOUND_PITCH,X
   LDX   NOTE_INDEX
   LDA   __zpabi_append_note__duration
   STA   SOUND_DURATION,X
   RTS

load_sound_data:
   SUBROUTINE

   LDA   __zpabi_load_sound_data__notes_0
   STA   __local_load_sound_data__notes
   LDA   __zpabi_load_sound_data__notes_1
   STA   __local_load_sound_data__1
   JMP   .loop@6_start
.load_sound_data@rot@1:
   LDY   #$01
   STA   __zpabi_append_note__duration
   LDA   (__local_load_sound_data__notes),Y
   STA   __zpabi_append_note__pitch
   JSR   append_note
   LDA   __local_load_sound_data__notes
   CLC
   ADC   #$02
   STA   __local_load_sound_data__notes
   BCC   .loop@6_start
   INC   __local_load_sound_data__1
.loop@6_start:
   LDY   #$00
   LDA   (__local_load_sound_data__notes),Y
   BNE   .load_sound_data@rot@1
   RTS

play_sound:
   SUBROUTINE

   LDA   NOTE_INDEX
   STA   __zpabi_play_note__y
   BEQ   .if_end@5
   LDX   __zpabi_play_note__y
   LDA   SOUND_PITCH,X
   STA   __zpabi_play_note__pitch
   LDA   SOUND_DURATION,X
   STA   __zpabi_play_note__duration
   JSR   play_note
   DEC   NOTE_INDEX
   LDX   __zpabi_play_note__y
   LDA   SOUND_DURATION,X
   STA   __local_play_sound__0
   CMP   FRAME_PERIOD
   BCS   .if_end@6
   LDA   FRAME_PERIOD
   SEC
   SBC   __local_play_sound__0
   STA   __zpabi_sound_delay1__count
   JSR   sound_delay1
   SEC
.if_end@6:
   RTS
.if_end@5:
   LDA   NOT_FALLING
   BEQ   .if_end@7
   LDA   FRAME_PERIOD
   STA   __zpabi_sound_delay__index
   JSR   sound_delay
   CLC
   RTS
.if_end@7:
   LDA   FALL_PITCH
   LSR
   STA   __zpabi_play_note__pitch
   LDA   FRAME_PERIOD
   CLC
   ADC   #$02
   STA   __zpabi_play_note__duration
   LDA   #$00
   STA   __zpabi_play_note__y
   JSR   play_note
   INC   FALL_PITCH
   CLC
   RTS

SOUND_DELAY_AMOUNTS:
   DC.B  $02, $04, $06, $08, $0A, $0C, $0E, $10, $12, $14, $16, $18, $1A, $1C, $1E, $20
