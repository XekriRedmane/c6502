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
cursor_sprite	EQU	$1E
wait_result	EQU	$1F
__dos_zp	EQU	$26
GUARD_RESURRECT_COL	EQU	$53
NOTE_INDEX	EQU	$54
GUARD_GOLD_TIMER_START_VALUE	EQU	$5F
GUARD_PATTERNS	EQU	$60
GUARD_PHASE	EQU	$64
PADDLE_VALUES	EQU	$65
__zpabi_draw_sprite_at_pixel_coords__row	EQU	$80
__zpabi_erase_sprite_at_pixel_coords__row	EQU	$80
__zpabi_play_note__y	EQU	$80
__local_check_for_input__1	EQU	$81
__local_detect_lack_of_joystick__n_1	EQU	$81
__local_detect_lack_of_joystick__n_0	EQU	$82
__local_poll_for_key__n	EQU	$82
__local_poll_for_key__joystick_ends	EQU	$83
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
__local_past_threshold__threshold	EQU	$AE
__local_return_handler__sprite	EQU	$AE
__local_check_joystick_or_delay__x	EQU	$AF
__local_past_threshold__reading	EQU	$AF
__local_return_handler__g	EQU	$AF
__local_return_handler__z	EQU	$B0
__local_return_handler__x	EQU	$B1
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
; def button_pressed params= locals=__local_button_pressed__0 indirect=false in_cycle=false carry=true
; def check_for_input params= locals=__local_check_for_input__0,__local_check_for_input__1 indirect=false in_cycle=false
; def check_for_mode_1_input params= locals=__local_check_for_mode_1_input__0 indirect=false in_cycle=false
; def check_joystick_or_delay params= locals=__local_check_joystick_or_delay__button,__local_check_joystick_or_delay__x indirect=false in_cycle=false carry=true
; def detect_lack_of_joystick params= locals=__local_detect_lack_of_joystick__0,__local_detect_lack_of_joystick__n_1,__local_detect_lack_of_joystick__n_0 indirect=false in_cycle=false
; def paddle_timer_running params= locals=__local_paddle_timer_running__0,__local_paddle_timer_running__1 indirect=false in_cycle=false carry=true
; def past_threshold params=__zpabi_past_threshold__reading,__zpabi_past_threshold__threshold locals=__local_past_threshold__threshold,__local_past_threshold__reading indirect=false in_cycle=false param_regs=A,X carry=true
; def poll_for_key params=__zpabi_poll_for_key__joystick_ends locals=__local_poll_for_key__0,__local_poll_for_key__n,__local_poll_for_key__joystick_ends indirect=false in_cycle=false param_regs=A carry=true
; def read_joystick_for_command params= locals=__local_read_joystick_for_command__dig,__local_read_joystick_for_command__0 indirect=false in_cycle=false
; def read_paddles params= locals=__local_read_paddles__0,__local_read_paddles__1 indirect=false in_cycle=false
; def return_handler params= locals=__local_return_handler__sprite,__local_return_handler__g,__local_return_handler__z,__local_return_handler__x,__local_return_handler__0 indirect=false in_cycle=false
; def wait_for_key params=__zpabi_wait_for_key__cursor locals=__local_wait_for_key__cursor indirect=false in_cycle=false param_regs=A
; def wait_for_key_with_cursor_page_1 params=__zpabi_wait_for_key_with_cursor_page_1__cursor locals=__local_wait_for_key_with_cursor_page_1__cursor indirect=false in_cycle=false param_regs=A
; def wait_key params= locals=__local_wait_key__0 indirect=false in_cycle=false
; def wait_key_queued params= locals=__local_wait_key_queued__0,__local_wait_key_queued__1 indirect=false in_cycle=false
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
; ext clear_hgr1 params=
; ext clear_hgr2 params=
; ext cold_start params=
; ext compress_and_save_level_data params=
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
; ext iris_wipe params=
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
; ext spinning_game_over params= carry=true
; ext start_level_editor params=
; ext to_decimal3 params=__zpabi_to_decimal3__n param_regs=A
; call check_for_input -> check_for_mode_1_input
; call check_for_input -> put_status_lives
; call check_for_input -> read_joystick_for_command
; call check_for_input -> return_handler
; call check_for_input -> wait_key_queued
; call check_for_mode_1_input -> button_pressed
; call check_joystick_or_delay -> read_paddles
; call detect_lack_of_joystick -> paddle_timer_running
; call poll_for_key -> check_joystick_or_delay
; call read_joystick_for_command -> past_threshold
; call read_joystick_for_command -> read_paddles
; call read_paddles -> paddle_timer_running
; call return_handler -> button_pressed
; call return_handler -> clear_hgr2
; call return_handler -> draw_sprite_page2
; call return_handler -> hi_score_screen
; call wait_for_key -> draw_sprite_page2
; call wait_for_key -> poll_for_key
; call wait_for_key_with_cursor_page_1 -> draw_sprite_page1
; call wait_for_key_with_cursor_page_1 -> poll_for_key
; item paddle_timer_running section=.text local=true zero=false
; item read_paddles section=.text local=true zero=false
; item past_threshold section=.text local=true zero=false
; item button_pressed section=.text local=true zero=false
; item read_joystick_for_command section=.text local=true zero=false
; item check_for_mode_1_input section=.text local=true zero=false
; item wait_key_queued section=.text local=true zero=false
; item check_for_input section=.text local=false zero=false
; item return_handler section=.text local=true zero=false
; item check_joystick_or_delay section=.text local=true zero=false
; item poll_for_key section=.text local=true zero=false
; item wait_for_key section=.text local=false zero=false
; item wait_for_key_with_cursor_page_1 section=.text local=false zero=false
; item wait_key section=.text local=false zero=false
; item detect_lack_of_joystick section=.text local=false zero=false
; item VALID_KEY_COMMANDS section=.rodata local=true zero=false
; item paddle_thresholds section=.data local=true zero=false
; item LEVEL_BG@lo section=.rodata local=true zero=false merge=true
; item LEVEL_BG@hi section=.rodata local=true zero=false merge=true
; item __zp_init section=.text local=false zero=false
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
; zp cursor_sprite size=1 at=auto def=true init=$06 weight=36 local=true
; zp wait_result size=1 at=auto def=true init=$00 weight=19 local=true
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

paddle_timer_running:
   SUBROUTINE

   LDA   PADDL
   ORA   PADDL+1
   AND   #$80
   BEQ   .cmp_end@0
   SEC
   RTS
.cmp_end@0:
   CMP   #$01
   RTS

read_paddles:
   SUBROUTINE

   LDA   #$00
   STA   PADDLE_VALUES
   STA   PADDLE_VALUES+1
   LDA   PTRIG
.loop@0_start:
   LDA   PADDL+1
   BPL   .if_end@0
   INC   PADDLE_VALUES+1
.if_end@0:
   LDA   PADDL
   BPL   .if_end@1
   INC   PADDLE_VALUES
.if_end@1:
   JSR   paddle_timer_running
   BCS   .if_end@2
   RTS
.if_end@2:
   LDA   PADDLE_VALUES
   ORA   PADDLE_VALUES+1
   BPL   .loop@0_start
   RTS

past_threshold:
   SUBROUTINE

   STA   __local_past_threshold__reading
   STX   __local_past_threshold__threshold
   TXA
   CMP   #$2E
   BNE   .if_end@3
   LDA   __local_past_threshold__reading
   SEC
   SBC   __local_past_threshold__threshold
   RTS
.if_end@3:
   CLC
   SBC   __local_past_threshold__reading
   RTS

button_pressed:
   SUBROUTINE

   LDA   INPUT_MODE
   CMP   #$CB
   BEQ   .and_false@4
   LDA   BUTN1
   BMI   .button_pressed@scfold@0
   LDA   BUTN0
   BPL   .and_false@4
.button_pressed@scfold@0:
   SEC
   RTS
.and_false@4:
   CLC
   RTS

read_joystick_for_command:
   SUBROUTINE

   LDA   BUTN1
   BPL   .if_else@9
   LDA   #$D5
   BNE   .if_end@8
.if_else@9:
   LDA   BUTN0
   BMI   .if_end@10
   JSR   read_paddles
   LDA   PADDLE_VALUES
   LDX   paddle_thresholds+1
   JSR   past_threshold
   BCC   .if_else@13
   LDA   #$CC
   STA   KEY_COMMAND_LR
   JMP   .if_end@12
.if_else@13:
   LDX   paddle_thresholds
   LDA   PADDLE_VALUES
   JSR   past_threshold
   BCC   .if_else@15
   LDA   #$CA
   STA   KEY_COMMAND_LR
   JMP   .if_end@14
.if_else@15:
   LDA   #$C0
   STA   KEY_COMMAND_LR
.if_end@14:
.if_end@12:
   LDA   PADDLE_VALUES+1
   LDX   paddle_thresholds+2
   JSR   past_threshold
   BCC   .if_else@17
   LDA   #$C9
   STA   KEY_COMMAND
   RTS
.if_else@17:
   LDX   paddle_thresholds+3
   LDA   PADDLE_VALUES+1
   JSR   past_threshold
   BCC   .if_else@19
   LDA   #$CB
   STA   KEY_COMMAND
   RTS
.if_else@19:
   LDA   #$C0
   STA   KEY_COMMAND
   RTS
.if_end@10:
   LDA   #$CF
.if_end@8:
   STA   KEY_COMMAND
   STA   KEY_COMMAND_LR
   RTS

check_for_mode_1_input:
   SUBROUTINE

   LDA   KBD
   BMI   .check_for_mode_1_input@scfold@0
   JSR   button_pressed
   BCC   .if_end@22
.check_for_mode_1_input@scfold@0:
   LSR   DEMO_RUNNING
   LSR   ALIVE
   LDA   #$01
   STA   LIVES
   RTS
.if_end@22:
   LDA   DEMO_MOVE_TURNS
   BNE   .if_end@23
   LDY   #$00
   LDA   (DEMO_PTR),Y
   STA   DEMO_MOVE
   INY
   LDA   (DEMO_PTR),Y
   STA   DEMO_MOVE_TURNS
   LDA   DEMO_PTR
   ADC   #$02
   STA   DEMO_PTR
   BCC   .carry_inc@0
   INC   DEMO_PTR+1
.carry_inc@0:
.if_end@23:
   LDA   DEMO_MOVE
   AND   #$0F
   TAX
   LDA   VALID_KEY_COMMANDS,X
   STA   KEY_COMMAND
   LDA   DEMO_MOVE
   LSR
   LSR
   LSR
   LSR
   TAX
   LDA   VALID_KEY_COMMANDS,X
   STA   KEY_COMMAND_LR
   DEC   DEMO_MOVE_TURNS
   RTS

wait_key_queued:
   SUBROUTINE

.loop@1_start:
   LDA   KBD
   BPL   .loop@1_start
   STA   KBDSTRB
   RTS

check_for_input:
   SUBROUTINE

.loop@2_start:
   LDA   GAME_MODE
   CMP   #$01
   BNE   .if_end@24
   JMP   check_for_mode_1_input
.if_end@24:
   LDA   KBD
   STA   __local_check_for_input__1
   STA   KBDSTRB
   BMI   .if_end@25
   LDA   INPUT_MODE
   CMP   #$CB
   BEQ   .if_end@26
   JSR   read_joystick_for_command
.if_end@26:
   RTS
.if_end@25:
   CMP   #$9E
   BEQ   .case@4
   CMP   #$80
   BEQ   .case@5
   CMP   #$9B
   BEQ   .case@6
   CMP   #$92
   BEQ   .case@8
   CMP   #$81
   BEQ   .case@9
   CMP   #$93
   BEQ   .case@10
   CMP   #$8A
   BEQ   .case@11
   CMP   #$8B
   BEQ   .case@12
   CMP   #$88
   BEQ   .case@13
   CMP   #$95
   BEQ   .case@14
   CMP   #$98
   BNE   .lb_skip@4
   JMP   .case@15
.lb_skip@4:
   CMP   #$99
   BNE   .lb_skip@3
   JMP   .case@16
.lb_skip@3:
   CMP   #$8D
   BNE   .lb_skip@2
   JMP   .case@17
.lb_skip@2:
   JMP   .switch@3_break
.case@4:
   INC   LIVES
   INC   LEVELNUM
   INC   DISK_LEVEL_LOC
   LSR   ALIVE
   LSR   HI_SCORE_ELIGIBLE
   RTS
.case@5:
   LDA   LIVES
   CMP   #$FF
   BEQ   .if_end@27
   INC   LIVES
.if_end@27:
   JSR   put_status_lives
   LSR   HI_SCORE_ELIGIBLE
   JMP   .loop@2_start
.case@6:
.loop@7_continue:
   JSR   wait_key_queued
   CMP   #$9B
   BNE   .loop@7_continue
   JMP   .loop@2_start
.case@8:
   LDA   #$01
   STA   LIVES
   LSR   ALIVE
   RTS
.case@9:
   LSR   ALIVE
   RTS
.case@10:
   LDA   ENABLE_SOUND
   EOR   #$FF
   STA   ENABLE_SOUND
   JMP   .loop@2_start
.case@11:
   LDA   #$CA
   STA   INPUT_MODE
   JMP   .loop@2_start
.case@12:
   LDA   #$CB
   STA   INPUT_MODE
   JMP   .loop@2_start
.case@13:
   LDA   FRAME_PERIOD
   CMP   #$0F
   BNE   .lb_skip@1
   JMP   .loop@2_start
.lb_skip@1:
   INC   FRAME_PERIOD
   JMP   .loop@2_start
.case@14:
   LDA   FRAME_PERIOD
   BNE   .lb_skip@0
   JMP   .loop@2_start
.lb_skip@0:
   DEC   FRAME_PERIOD
   JMP   .loop@2_start
.case@15:
   LDA   paddle_thresholds
   LDX   paddle_thresholds+1
   STX   paddle_thresholds
   STA   paddle_thresholds+1
   JMP   .loop@2_start
.case@16:
   LDA   paddle_thresholds+2
   LDX   paddle_thresholds+3
   STX   paddle_thresholds+2
   STA   paddle_thresholds+3
   JMP   .loop@2_start
.case@17:
   JSR   return_handler
   JMP   .loop@2_start
.switch@3_break:
   LDA   INPUT_MODE
   CMP   #$CA
   BNE   .if_else@31
   JMP   read_joystick_for_command
.if_else@31:
   LDA   __local_check_for_input__1
   STA   KEY_COMMAND
   STA   KEY_COMMAND_LR
   RTS

return_handler:
   SUBROUTINE

   JSR   hi_score_screen
   LDA   #$FF
   STA   __local_return_handler__x
   STA   __local_return_handler__sprite
   LDA   #$04
   STA   __local_return_handler__z
.loop@18_start:
   JSR   button_pressed
   BCS   .loop@18_break
   LDA   KBD
   BMI   .loop@18_break
   DEC   __local_return_handler__x
   BNE   .loop@18_start
   DEC   __local_return_handler__sprite
   BNE   .loop@18_start
   DEC   __local_return_handler__z
   BNE   .loop@18_start
.loop@18_break:
   LDA   #$00
   STA   KBDSTRB
   STA   TXTPAGE1
   JSR   clear_hgr2
   LDA   #$0F
   STA   GAME_ROWNUM
.return_handler@rot@1:
   LDA   #$1B
   STA   GAME_COLNUM
.return_handler@rot@0:
   LDX   GAME_ROWNUM
   LDA   LEVEL_BG@lo,X
   STA   __local_return_handler__sprite
   LDA   LEVEL_BG@hi,X
   STA   __local_return_handler__g
   LDY   GAME_COLNUM
   LDA   (__local_return_handler__sprite),Y
   CMP   #$05
   BNE   .if_end@39
   LDA   #$01
.if_end@39:
   JSR   draw_sprite_page2
   DEC   GAME_COLNUM
   BPL   .return_handler@rot@0
   DEC   GAME_ROWNUM
   BPL   .return_handler@rot@1
   LDA   #$1E
   STA   __local_return_handler__g
.return_handler@rot@2:
   LDX   __local_return_handler__g
   LDA   BRICK_FILL_TIMERS,X
   BEQ   .loop@21_continue
   LDA   BRICK_DIG_ROWS,X
   STA   GAME_ROWNUM
   LDA   BRICK_DIG_COLS,X
   STA   GAME_COLNUM
   LDA   BRICK_FILL_TIMERS,X
   CMP   #$15
   BCC   .if_else@42
   LDA   #$00
   JSR   draw_sprite_page2
   JMP   .if_end@41
.if_else@42:
   CMP   #$0B
   BCC   .if_else@44
   LDA   #$37
   JSR   draw_sprite_page2
   JMP   .if_end@43
.if_else@44:
   LDA   #$38
   JSR   draw_sprite_page2
.if_end@43:
.if_end@41:
.loop@21_continue:
   DEC   __local_return_handler__g
   BPL   .return_handler@rot@2
   LDA   GUARD_COUNT
   STA   __local_return_handler__g
   JMP   .loop@22_start
.return_handler@rot@3:
   LDX   __local_return_handler__g
   LDA   GUARD_RESURRECTION_TIMERS,X
   BEQ   .loop@22_continue
   LDA   GUARD_LOCS_COL,X
   STA   GAME_COLNUM
   LDA   GUARD_LOCS_ROW,X
   STA   GAME_ROWNUM
   LDA   GUARD_RESURRECTION_TIMERS,X
   CMP   #$14
   BCS   .loop@22_continue
   CMP   #$0B
   BCC   .cond_else@47
   LDA   #$39
   BNE   .cond_end@48
.cond_else@47:
   LDA   #$3A
.cond_end@48:
   JSR   draw_sprite_page2
.loop@22_continue:
   DEC   __local_return_handler__g
.loop@22_start:
   LDA   __local_return_handler__g
   BNE   .return_handler@rot@3
   RTS

check_joystick_or_delay:
   SUBROUTINE

   LDA   INPUT_MODE
   CMP   #$CB
   BNE   .if_end@49
   LDA   #$02
   STA   __local_check_joystick_or_delay__x
.loop@23_start:
   LDA   __local_check_joystick_or_delay__x
   BEQ   .if_end@54
   LDX   #$00
.loop@24_keep_start:
   DEX
   BNE   .loop@24_keep_start
   DEC   __local_check_joystick_or_delay__x
   JMP   .loop@23_start
.if_end@49:
   JSR   read_paddles
   LDX   #$00
.check_joystick_or_delay@rot@4:
   LDA   PADDLE_VALUES,X
   CMP   #$12
   BCC   .tail@0
   CMP   #$3B
   BCS   .tail@0
   INX
   CPX   #$02
   BCC   .check_joystick_or_delay@rot@4
   LDA   BUTN1
   BMI   .tail@0
   LDA   BUTN0
   BPL   .if_end@54
.tail@0:
   STA   wait_result
   SEC
   RTS
.if_end@54:
   CLC
   RTS

poll_for_key:
   SUBROUTINE

   STA   __local_poll_for_key__joystick_ends
   LDA   #$68
   STA   __local_poll_for_key__n
.poll_for_key@rot@5:
   LDA   KBD
   BPL   .if_end@55
   STA   wait_result
   SEC
   RTS
.if_end@55:
   JSR   check_joystick_or_delay
   BCC   .if_end@58
   LDA   __local_poll_for_key__joystick_ends
   BEQ   .if_end@58
   RTS
.if_end@58:
   DEC   __local_poll_for_key__n
   BNE   .poll_for_key@rot@5
   CLC
   RTS

wait_for_key:
   SUBROUTINE

   STA   cursor_sprite
.loop@27_start:
   LDA   cursor_sprite
   BNE   .cond_end@60
   LDA   #$0A
.cond_end@60:
   JSR   draw_sprite_page2
   LDA   #$00
   JSR   poll_for_key
   BCS   .loop@27_break
   LDA   #$00
   JSR   draw_sprite_page2
   LDA   #$00
   JSR   poll_for_key
   BCC   .loop@27_start
.loop@27_break:
   LDA   cursor_sprite
   JSR   draw_sprite_page2
   LDA   wait_result
   RTS

wait_for_key_with_cursor_page_1:
   SUBROUTINE

   STA   cursor_sprite
.loop@28_start:
   LDA   cursor_sprite
   BEQ   .cond_else@62
   LDA   #$00
   BEQ   .cond_end@63
.cond_else@62:
   LDA   #$0A
.cond_end@63:
   JSR   draw_sprite_page1
   LDA   #$01
   JSR   poll_for_key
   BCS   .loop@28_break
   LDA   cursor_sprite
   JSR   draw_sprite_page1
   LDA   #$01
   JSR   poll_for_key
   BCC   .loop@28_start
.loop@28_break:
   LDA   cursor_sprite
   JSR   draw_sprite_page1
   LDA   wait_result
   RTS

wait_key:
   SUBROUTINE

.loop@29_start:
   LDA   #$00
   STA   KBDSTRB
   LDA   KBD
   BMI   .loop@29_start
   RTS

detect_lack_of_joystick:
   SUBROUTINE

   LDA   PTRIG
   LDA   #$00
   STA   __local_detect_lack_of_joystick__n_0
   LDY   #$10
   STY   __local_detect_lack_of_joystick__n_1
   JMP   .loop@30_start
.detect_lack_of_joystick@rot@6:
   JSR   paddle_timer_running
   BCS   .if_end@65
   RTS
.if_end@65:
   LDA   __local_detect_lack_of_joystick__n_0
   SBC   #$01
   STA   __local_detect_lack_of_joystick__n_0
   BCS   .loop@30_start
   DEC   __local_detect_lack_of_joystick__n_1
.loop@30_start:
   ORA   __local_detect_lack_of_joystick__n_1
   BNE   .detect_lack_of_joystick@rot@6
   LDA   #$CB
   STA   INPUT_MODE
   RTS

VALID_KEY_COMMANDS:
   DC.B  $C9, $CA, $CB, $CC, $CF, $D5, $A0

paddle_thresholds:
   DC.B  $12, $2E
   DC.B  $12, $2E

LEVEL_BG@lo:
   DC.B  $00, $1C, $38, $54, $70, $8C, $A8, $C4, $E0, $FC, $18, $34, $50, $6C, $88, $A4
   DC.B  $C0

LEVEL_BG@hi:
   DC.B  $0A, $0A, $0A, $0A, $0A, $0A, $0A, $0A, $0A, $0A, $0B, $0B, $0B, $0B, $0B, $0B
   DC.B  $0B

__zp_init:
   SUBROUTINE

   LDA   #$00
   STA   wait_result
   LDA   #$06
   STA   cursor_sprite
   RTS
