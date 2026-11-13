HARGS	EQU	$00
SSP	EQU	$01
FP	EQU	$03
DPTR	EQU	$05
__monitor_zp_1f	EQU	$1F
WNDLFT	EQU	$20
WNDWDTH	EQU	$21
WNDTOP	EQU	$22
WNDBTM	EQU	$23
CH	EQU	$24
CV	EQU	$25
GBAS	EQU	$26
BAS	EQU	$28
__monitor_bas2	EQU	$2A
__monitor_zp_2c	EQU	$2C
__dos_zp_2f	EQU	$2F
COLOR	EQU	$30
__dos_zp_31	EQU	$31
INVFLG	EQU	$32
PROMPT	EQU	$33
__dos_zp_34	EQU	$34
__monitor_ysav1	EQU	$35
CSW	EQU	$36
KSW	EQU	$38
__dos_zp_3a	EQU	$3A
__monitor_acc	EQU	$45
__dos_zp_46	EQU	$46
__monitor_status	EQU	$48
__dos_zp_49	EQU	$49
__monitor_rnd	EQU	$4E
__zork_zp_7c	EQU	$7C
SECTORS_PER_TRACK	EQU	$7F
__zork_zp_80	EQU	$80
Z_PC	EQU	$8A
__zork_zp_8d	EQU	$8D
LOCAL_ZVARS	EQU	$9A
__zork_zp_b8	EQU	$B8
Z_HEADER_ADDR	EQU	$BA
__zork_zp_bc	EQU	$BC
STACK_STATE	EQU	$C8
__zork_zp_ce	EQU	$CE
SCRATCH1	EQU	$E4
__zork_zp_e6	EQU	$E6
__zork_zp_e8	EQU	$E8
__local_inc_sector_and_write__1	EQU	$F3
__zpabi_copy_data_to_buff__src_0	EQU	$F3
__zpabi_cout_string__s_0	EQU	$F3
__zpabi_print_ascii_string__s_0	EQU	$F3
__local_inc_sector_and_write__2	EQU	$F4
__zpabi_copy_data_to_buff__src_1	EQU	$F4
__zpabi_cout_string__s_1	EQU	$F4
__zpabi_print_ascii_string__s_1	EQU	$F4
__zpabi_do_rwts_on_sector__buffer_0	EQU	$F5
__zpabi_inc_sector_and_write__buffer_0	EQU	$F5
__zpabi_do_rwts_on_sector__buffer_1	EQU	$F6
__zpabi_inc_sector_and_write__buffer_1	EQU	$F6
__local_instr_save__pages	EQU	$F7
__local_do_rwts_on_sector__command	EQU	$F8
__local_please_insert_save_diskette__n	EQU	$F8
__local_do_rwts_on_sector__sector_1	EQU	$F9
__local_please_insert_save_diskette__0	EQU	$F9
__local_do_rwts_on_sector__sector_0	EQU	$FA
__local_get_prompted_number_from_user__key	EQU	$FA
__local_get_prompted_number_from_user__0	EQU	$FB
__local_get_prompted_number_from_user__which	EQU	$FC
__local_get_prompted_number_from_user__1	EQU	$FD
__local_print_ascii_string__count	EQU	$FE
__local_print_ascii_string__i	EQU	$FF
BUFF_AREA	EQU	$0200
branch	EQU	$0BAA
negated_branch	EQU	$0BB3
buffer_char	EQU	$1B90
dump_buffer_line	EQU	$1BE2
dump_buffer_with_more	EQU	$1C5B
home	EQU	$1CC9
RWTS	EQU	$2900
HOME	EQU	$FC58
CLREOL	EQU	$FC9C
WAIT	EQU	$FCA8
RDKEY	EQU	$FD0C
COUT	EQU	$FDED
COUT1	EQU	$FDF0

; @zp-link-meta-begin
; def copy_data_to_buff params=__zpabi_copy_data_to_buff__src_0,__zpabi_copy_data_to_buff__src_1,__zpabi_copy_data_to_buff__x,__zpabi_copy_data_to_buff__count locals= indirect=false in_cycle=false param_regs=,,X,Y
; def cout_string params=__zpabi_cout_string__s_0,__zpabi_cout_string__s_1,__zpabi_cout_string__count locals= indirect=false in_cycle=false param_regs=,,X
; def do_rwts_on_sector params=__zpabi_do_rwts_on_sector__command,__zpabi_do_rwts_on_sector__buffer_0,__zpabi_do_rwts_on_sector__buffer_1 locals=__local_do_rwts_on_sector__command,__local_do_rwts_on_sector__sector_1,__local_do_rwts_on_sector__sector_0 indirect=false in_cycle=false param_regs=A,,
; def get_prompted_number_from_user params=__zpabi_get_prompted_number_from_user__which locals=__local_get_prompted_number_from_user__key,__local_get_prompted_number_from_user__0,__local_get_prompted_number_from_user__which,__local_get_prompted_number_from_user__1 indirect=false in_cycle=false param_regs=A
; def inc_sector_and_write params=__zpabi_inc_sector_and_write__buffer_0,__zpabi_inc_sector_and_write__buffer_1 locals=__local_inc_sector_and_write__1,__local_inc_sector_and_write__2 indirect=false in_cycle=false
; def instr_save params= locals=__local_instr_save__pages indirect=false in_cycle=false
; def please_insert_save_diskette params= locals=__local_please_insert_save_diskette__n,__local_please_insert_save_diskette__0 indirect=false in_cycle=false
; def please_reinsert_game_diskette params= locals= indirect=false in_cycle=false
; def print_ascii_string params=__zpabi_print_ascii_string__s_0,__zpabi_print_ascii_string__s_1,__zpabi_print_ascii_string__count locals=__local_print_ascii_string__count,__local_print_ascii_string__i indirect=false in_cycle=false param_regs=,,X
; def write_next_sector params= locals= indirect=false in_cycle=false
; ext CLREOL params=
; ext COUT params=__zpabi_COUT__c param_regs=A
; ext COUT1 params=__zpabi_COUT1__c param_regs=A
; ext HOME params=
; ext RDKEY params=
; ext RWTS params=__zpabi_RWTS__iob_hi,__zpabi_RWTS__iob_lo param_regs=A,Y
; ext WAIT params=__zpabi_WAIT__a param_regs=A
; ext branch params=
; ext buffer_char params=__zpabi_buffer_char__c param_regs=A
; ext dump_buffer_line params=
; ext dump_buffer_with_more params=
; ext home params=
; ext inc_sector_and_write params=__zpabi_inc_sector_and_write__buffer_0,__zpabi_inc_sector_and_write__buffer_1
; ext instr_save params=
; ext negated_branch params=
; ext please_insert_save_diskette params=
; ext please_reinsert_game_diskette params=
; ext write_next_sector params=
; call cout_string -> COUT1
; call do_rwts_on_sector -> RWTS
; call get_prompted_number_from_user -> CLREOL
; call get_prompted_number_from_user -> RDKEY
; call get_prompted_number_from_user -> cout_string
; call get_prompted_number_from_user -> dump_buffer_line
; call get_prompted_number_from_user -> dump_buffer_with_more
; call get_prompted_number_from_user -> print_ascii_string
; call inc_sector_and_write -> do_rwts_on_sector
; call instr_save -> branch
; call instr_save -> copy_data_to_buff
; call instr_save -> inc_sector_and_write
; call instr_save -> negated_branch
; call instr_save -> please_insert_save_diskette
; call instr_save -> please_reinsert_game_diskette
; call instr_save -> write_next_sector
; call please_insert_save_diskette -> RDKEY
; call please_insert_save_diskette -> buffer_char
; call please_insert_save_diskette -> dump_buffer_line
; call please_insert_save_diskette -> dump_buffer_with_more
; call please_insert_save_diskette -> get_prompted_number_from_user
; call please_insert_save_diskette -> home
; call please_insert_save_diskette -> print_ascii_string
; call please_reinsert_game_diskette -> RDKEY
; call please_reinsert_game_diskette -> dump_buffer_line
; call please_reinsert_game_diskette -> dump_buffer_with_more
; call please_reinsert_game_diskette -> print_ascii_string
; call print_ascii_string -> buffer_char
; call write_next_sector -> inc_sector_and_write
; share member=do_rwts_on_sector:__zpabi_do_rwts_on_sector__buffer_0,__zpabi_do_rwts_on_sector__buffer_1 member=inc_sector_and_write:__zpabi_inc_sector_and_write__buffer_0,__zpabi_inc_sector_and_write__buffer_1 holder=instr_save live=instr_save:inc_sector_and_write forward=inc_sector_and_write>do_rwts_on_sector forward=instr_save>inc_sector_and_write writer=instr_save
; zp __monitor_zp_1f size=1 at=$1F def=false init=
; zp WNDLFT size=1 at=$20 def=false init=
; zp WNDWDTH size=1 at=$21 def=false init=
; zp WNDTOP size=1 at=$22 def=false init=
; zp WNDBTM size=1 at=$23 def=false init=
; zp CH size=1 at=$24 def=false init=
; zp CV size=1 at=$25 def=false init=
; zp GBAS size=2 at=$26 def=false init=
; zp BAS size=2 at=$28 def=false init=
; zp __monitor_bas2 size=2 at=$2A def=false init=
; zp __monitor_zp_2c size=3 at=$2C def=false init=
; zp __dos_zp_2f size=1 at=$2F def=false init=
; zp COLOR size=1 at=$30 def=false init=
; zp __dos_zp_31 size=1 at=$31 def=false init=
; zp INVFLG size=1 at=$32 def=false init=
; zp PROMPT size=1 at=$33 def=false init=
; zp __dos_zp_34 size=1 at=$34 def=false init=
; zp __monitor_ysav1 size=1 at=$35 def=false init=
; zp CSW size=2 at=$36 def=false init=
; zp KSW size=2 at=$38 def=false init=
; zp __dos_zp_3a size=11 at=$3A def=false init=
; zp __monitor_acc size=1 at=$45 def=false init=
; zp __dos_zp_46 size=2 at=$46 def=false init=
; zp __monitor_status size=1 at=$48 def=false init=
; zp __dos_zp_49 size=5 at=$49 def=false init=
; zp __monitor_rnd size=2 at=$4E def=false init=
; zp __zork_zp_7c size=3 at=$7C def=false init=
; zp SECTORS_PER_TRACK size=1 at=$7F def=false init=
; zp __zork_zp_80 size=10 at=$80 def=false init=
; zp Z_PC size=3 at=$8A def=false init=
; zp __zork_zp_8d size=13 at=$8D def=false init=
; zp LOCAL_ZVARS size=30 at=$9A def=false init=
; zp __zork_zp_b8 size=2 at=$B8 def=false init=
; zp Z_HEADER_ADDR size=2 at=$BA def=false init=
; zp __zork_zp_bc size=12 at=$BC def=false init=
; zp STACK_STATE size=6 at=$C8 def=false init=
; zp __zork_zp_ce size=22 at=$CE def=false init=
; zp SCRATCH1 size=2 at=$E4 def=false init=
; zp __zork_zp_e6 size=2 at=$E6 def=false init=
; zp __zork_zp_e8 size=11 at=$E8 def=false init=
; runtime hargs_size=1 hargs_need=0
; @zp-link-meta-end


   SEG   text
   ORG   $1DAC
__text_start:

do_rwts_on_sector:
   SUBROUTINE

   STA   iob+12
   LDA   __zpabi_do_rwts_on_sector__buffer_0
   STA   iob+8
   LDA   __zpabi_do_rwts_on_sector__buffer_1
   STA   iob+9
   LDA   #$03
   STA   iob+4
   LDA   SCRATCH1
   STA   __local_do_rwts_on_sector__sector_0
   LDA   SCRATCH1+1
   STA   __local_do_rwts_on_sector__sector_1
.loop@0_continue:
   LDA   SECTORS_PER_TRACK
   STA   __local_do_rwts_on_sector__command
   LDA   __local_do_rwts_on_sector__sector_0
   SEC
   SBC   __local_do_rwts_on_sector__command
   LDA   __local_do_rwts_on_sector__sector_1
   SBC   #$00
   BCC   .loop@0_break
   LDA   SECTORS_PER_TRACK
   STA   __local_do_rwts_on_sector__command
   LDA   __local_do_rwts_on_sector__sector_0
   SEC
   SBC   __local_do_rwts_on_sector__command
   STA   __local_do_rwts_on_sector__sector_0
   BCS   .carry_inc@0
   DEC   __local_do_rwts_on_sector__sector_1
.carry_inc@0:
   INC   iob+4
   JMP   .loop@0_continue
.loop@0_break:
   LDA   __local_do_rwts_on_sector__sector_0
   STA   iob+5
   LDA   #>iob
   LDY   #<iob
   JMP   RWTS

inc_sector_and_write:
   SUBROUTINE

   INC   SCRATCH1
   BNE   .inc_done@0
   INC   SCRATCH1+1
.inc_done@0:
   LDA   dct+2
   STA   __local_inc_sector_and_write__2
   LDA   dct+3
   STA   __local_inc_sector_and_write__1
   LDA   #$EF
   STA   dct+2
   LDA   #$D8
   STA   dct+3
   LDA   #$02
   JSR   do_rwts_on_sector
   LDA   __local_inc_sector_and_write__2
   STA   dct+2
   LDA   __local_inc_sector_and_write__1
   STA   dct+3
   RTS

write_next_sector:
   SUBROUTINE

   LDA   #<BUFF_AREA
   STA   __zpabi_inc_sector_and_write__buffer_0
   LDA   #>BUFF_AREA
   STA   __zpabi_inc_sector_and_write__buffer_1
   JMP   inc_sector_and_write

print_ascii_string:
   SUBROUTINE

   STX   __local_print_ascii_string__count
   LDA   #$00
   STA   __local_print_ascii_string__i
.loop@0_start:
   LDY   __local_print_ascii_string__i
   LDA   (__zpabi_print_ascii_string__s_0),Y
   JSR   buffer_char
   INC   __local_print_ascii_string__i
   DEC   __local_print_ascii_string__count
   BNE   .loop@0_start
   RTS

cout_string:
   SUBROUTINE

   LDY   #$00
.loop@1_start:
   LDA   (__zpabi_cout_string__s_0),Y
   ORA   #$80
   JSR   COUT1
   INY
   DEX
   BNE   .loop@1_start
   RTS

get_prompted_number_from_user:
   SUBROUTINE

   STA   __local_get_prompted_number_from_user__which
   JSR   dump_buffer_with_more
   ASL   __local_get_prompted_number_from_user__which
   LDA   #$00
   STA   __local_get_prompted_number_from_user__1
   ROL   __local_get_prompted_number_from_user__1
   ASL   __local_get_prompted_number_from_user__which
   ROL   __local_get_prompted_number_from_user__1
   ASL   __local_get_prompted_number_from_user__which
   ROL   __local_get_prompted_number_from_user__1
   ASL   __local_get_prompted_number_from_user__which
   ROL   __local_get_prompted_number_from_user__1
   LDA   #<prompts
   CLC
   ADC   __local_get_prompted_number_from_user__which
   STA   __local_get_prompted_number_from_user__0
   LDA   #>prompts
   ADC   __local_get_prompted_number_from_user__1
   STA   __local_get_prompted_number_from_user__key
   LDA   __local_get_prompted_number_from_user__0
   STA   __zpabi_print_ascii_string__s_0
   LDA   __local_get_prompted_number_from_user__key
   STA   __zpabi_print_ascii_string__s_1
   LDX   #$0F
   JSR   print_ascii_string
   JSR   dump_buffer_line
   LDA   #$19
   STA   CH
   LDA   #$3F
   STA   INVFLG
   LDA   #<sDefault
   STA   __zpabi_cout_string__s_0
   LDA   #>sDefault
   STA   __zpabi_cout_string__s_1
   LDX   #$0A
   JSR   cout_string
   LDA   #<prompts
   CLC
   ADC   __local_get_prompted_number_from_user__which
   STA   __local_get_prompted_number_from_user__key
   LDA   #>prompts
   ADC   __local_get_prompted_number_from_user__1
   STA   __local_get_prompted_number_from_user__0
   LDA   __local_get_prompted_number_from_user__key
   CLC
   ADC   #$0F
   BCC   .carry_inc@0
   INC   __local_get_prompted_number_from_user__0
.carry_inc@0:
   STA   __zpabi_cout_string__s_0
   LDA   __local_get_prompted_number_from_user__0
   STA   __zpabi_cout_string__s_1
   LDX   #$01
   JSR   cout_string
   LDA   #$FF
   STA   INVFLG
   JSR   RDKEY
   STA   __local_get_prompted_number_from_user__key
   LDA   #$19
   STA   CH
   JSR   CLREOL
   LDA   __local_get_prompted_number_from_user__key
   CMP   #$8D
   BNE   .if_end@0
   LDA   #<prompts
   CLC
   ADC   __local_get_prompted_number_from_user__which
   STA   __local_get_prompted_number_from_user__key
   LDA   #>prompts
   ADC   __local_get_prompted_number_from_user__1
   STA   __local_get_prompted_number_from_user__0
   LDY   #$0F
   LDA   (__local_get_prompted_number_from_user__key),Y
.if_end@0:
   AND   #$7F
   RTS

please_insert_save_diskette:
   SUBROUTINE

   JSR   home
   JSR   dump_buffer_with_more
   JSR   dump_buffer_with_more
   LDA   #<sPleaseInsert
   STA   __zpabi_print_ascii_string__s_0
   LDA   #>sPleaseInsert
   STA   __zpabi_print_ascii_string__s_1
   LDX   #$1C
   JSR   print_ascii_string
   JSR   dump_buffer_with_more
.loop@2_start:
   LDA   #$02
   JSR   get_prompted_number_from_user
   CMP   #$30
   BCC   .loop@2_start
   CMP   #$38
   BCS   .loop@2_start
   STA   prompts+47
   JSR   buffer_char
.loop@3_start:
   LDA   #$00
   JSR   get_prompted_number_from_user
   STA   __local_please_insert_save_diskette__0
   CMP   #$31
   BCC   .loop@3_start
   CMP   #$38
   BCS   .loop@3_start
   AND   #$07
   ASL
   ASL
   ASL
   ASL
   STA   iob+1
   LDA   __local_please_insert_save_diskette__0
   STA   prompts+15
   JSR   buffer_char
.loop@4_start:
   LDA   #$01
   JSR   get_prompted_number_from_user
   STA   __local_please_insert_save_diskette__n
   CMP   #$31
   BCC   .loop@4_start
   CMP   #$33
   BCS   .loop@4_start
   AND   #$03
   STA   iob+2
   LDA   __local_please_insert_save_diskette__n
   STA   prompts+31
   JSR   buffer_char
.loop@5_start:
   JSR   dump_buffer_with_more
   LDA   #<sReturnToBegin
   STA   __zpabi_print_ascii_string__s_0
   LDA   #>sReturnToBegin
   STA   __zpabi_print_ascii_string__s_1
   LDX   #$23
   JSR   print_ascii_string
   JSR   dump_buffer_line
   JSR   RDKEY
   CMP   #$8D
   BNE   .loop@5_start
   LDA   #$FF
   STA   SCRATCH1
   LDA   #$FF
   STA   SCRATCH1+1
   LDA   prompts+47
   AND   #$07
   STA   __local_please_insert_save_diskette__n
.loop@6_start:
   LDA   __local_please_insert_save_diskette__n
   BEQ   .loop@6_break
   LDA   SCRATCH1
   CLC
   ADC   #$40
   STA   SCRATCH1
   BCC   .carry_inc@1
   INC   SCRATCH1+1
.carry_inc@1:
   DEC   __local_please_insert_save_diskette__n
   JMP   .loop@6_start
.loop@6_break:
   JMP   dump_buffer_with_more

please_reinsert_game_diskette:
   SUBROUTINE

   LDA   iob+1
   CMP   #$60
   BNE   .if_end@9
   LDA   iob+2
   CMP   #$01
   BNE   .if_end@9
   JSR   dump_buffer_with_more
   LDA   #<sReinsertGameDiskette
   STA   __zpabi_print_ascii_string__s_0
   LDA   #>sReinsertGameDiskette
   STA   __zpabi_print_ascii_string__s_1
   LDX   #$1F
   JSR   print_ascii_string
.loop@7_start:
   JSR   dump_buffer_with_more
   LDA   #<sPressReturnToContinue
   STA   __zpabi_print_ascii_string__s_0
   LDA   #>sPressReturnToContinue
   STA   __zpabi_print_ascii_string__s_1
   LDX   #$26
   JSR   print_ascii_string
   JSR   dump_buffer_line
   JSR   RDKEY
   CMP   #$8D
   BNE   .loop@7_start
   JSR   dump_buffer_with_more
.if_end@9:
   LDA   #$60
   STA   iob+1
   LDA   #$01
   STA   iob+2
   RTS

copy_data_to_buff:
   SUBROUTINE

.loop@0_start:
   DEY
   LDA   (__zpabi_copy_data_to_buff__src_0),Y
   STA   BUFF_AREA,X
   INX
   TYA
   BNE   .loop@0_start
   TXA
   RTS

instr_save:
   SUBROUTINE

   JSR   please_insert_save_diskette
   LDY   #$00
   LDA   (Z_HEADER_ADDR),Y
   STA   BUFF_AREA
   LDA   #<Z_PC
   STA   __zpabi_copy_data_to_buff__src_0
   LDA   #>Z_PC
   STA   __zpabi_copy_data_to_buff__src_1
   LDX   #$01
   LDY   #$03
   JSR   copy_data_to_buff
   TAX
   LDA   #<LOCAL_ZVARS
   STA   __zpabi_copy_data_to_buff__src_0
   LDA   #>LOCAL_ZVARS
   STA   __zpabi_copy_data_to_buff__src_1
   LDY   #$1E
   JSR   copy_data_to_buff
   TAX
   LDA   #<STACK_STATE
   STA   __zpabi_copy_data_to_buff__src_0
   LDA   #>STACK_STATE
   STA   __zpabi_copy_data_to_buff__src_1
   LDY   #$06
   JSR   copy_data_to_buff
   JSR   write_next_sector
   BCS   .instr_save@fail
   LDA   #$80
   STA   __zpabi_copy_data_to_buff__src_0
   LDA   #$02
   STA   __zpabi_copy_data_to_buff__src_1
   LDX   #$00
   LDY   #$00
   JSR   copy_data_to_buff
   JSR   write_next_sector
   BCS   .instr_save@fail
   LDA   #$80
   STA   __zpabi_copy_data_to_buff__src_0
   LDA   #$03
   STA   __zpabi_copy_data_to_buff__src_1
   LDX   #$00
   LDY   #$68
   JSR   copy_data_to_buff
   JSR   write_next_sector
   BCS   .instr_save@fail
   LDA   Z_HEADER_ADDR
   STA   __zpabi_inc_sector_and_write__buffer_0
   LDA   Z_HEADER_ADDR+1
   STA   __zpabi_inc_sector_and_write__buffer_1
   LDY   #$0E
   LDA   (Z_HEADER_ADDR),Y
   CLC
   ADC   #$01
   STA   __local_instr_save__pages
.loop@1_start:
   JSR   inc_sector_and_write
   BCS   .instr_save@fail
   LDA   __zpabi_inc_sector_and_write__buffer_0
   CLC
   ADC   #$00
   STA   __zpabi_inc_sector_and_write__buffer_0
   LDA   __zpabi_inc_sector_and_write__buffer_1
   ADC   #$01
   STA   __zpabi_inc_sector_and_write__buffer_1
   DEC   __local_instr_save__pages
   BNE   .loop@1_start
   JSR   inc_sector_and_write
   BCS   .instr_save@fail
   JSR   please_reinsert_game_diskette
   JMP   branch
.instr_save@fail:
   JSR   please_reinsert_game_diskette
   JMP   negated_branch
__text_end:
__text_load	EQU	__text_start
   IF    __text_end > $2200
   ECHO  "section .text runs past the end of region save by", [__text_end - $2200]d, "bytes"
   ERR
   ENDIF

   SEG   rodata
   ORG   __text_end
__rodata_start:

sPleaseInsert:
   DC.B  $50, $4C, $45, $41, $53, $45, $20, $49, $4E, $53, $45, $52, $54, $20, $53, $41
   DC.B  $56, $45, $20, $44, $49, $53, $4B, $45, $54, $54, $45, $2C

sDefault:
   DC.B  $44, $45, $46, $41, $55, $4C, $54, $20, $3D, $20

sReturnToBegin:
   DC.B  $2D, $2D, $2D, $20, $50, $52, $45, $53, $53, $20, $27, $52, $45, $54, $55, $52
   DC.B  $4E, $27, $20, $4B, $45, $59, $20, $54, $4F, $20, $42, $45, $47, $49, $4E, $20
   DC.B  $2D, $2D, $2D

sReinsertGameDiskette:
   DC.B  $50, $4C, $45, $41, $53, $45, $20, $52, $45, $2D, $49, $4E, $53, $45, $52, $54
   DC.B  $20, $47, $41, $4D, $45, $20, $44, $49, $53, $4B, $45, $54, $54, $45, $2C

sPressReturnToContinue:
   DC.B  $2D, $2D, $2D, $20, $50, $52, $45, $53, $53, $20, $27, $52, $45, $54, $55, $52
   DC.B  $4E, $27, $20, $4B, $45, $59, $20, $54, $4F, $20, $43, $4F, $4E, $54, $49, $4E
   DC.B  $55, $45, $20, $2D, $2D, $2D
__rodata_end:
__rodata_load	EQU	__rodata_start
   IF    __rodata_end > $2200
   ECHO  "section .rodata runs past the end of region save by", [__rodata_end - $2200]d, "bytes"
   ERR
   ENDIF

   SEG   data
   ORG   __rodata_end
__data_start:

dct:
   DS.B  1
   DC.B  $01
   DC.W  $D8EF

iob:
   DC.B  $01
   DC.B  $60
   DC.B  $01
   DS.B  3
   DC.W  dct
   DS.B  7
   DC.B  $60
   DC.B  $01

prompts:
   DC.B  $53, $4C, $4F, $54, $20, $20, $20, $20, $20, $28, $31, $2D, $37, $29, $3A
   DC.B  $36
   DC.B  $44, $52, $49, $56, $45, $20, $20, $20, $20, $28, $31, $2D, $32, $29, $3A
   DC.B  $32
   DC.B  $50, $4F, $53, $49, $54, $49, $4F, $4E, $20, $28, $30, $2D, $37, $29, $3A
   DC.B  $30
__data_end:
__data_load	EQU	__data_start
   IF    __data_end > $2200
   ECHO  "section .data runs past the end of region save by", [__data_end - $2200]d, "bytes"
   ERR
   ENDIF

   SEG   bss
   ORG   __data_end
__bss_start:
__bss_end:
__bss_load	EQU	__bss_start
   IF    __bss_end > $2200
   ECHO  "section .bss runs past the end of region save by", [__bss_end - $2200]d, "bytes"
   ERR
   ENDIF
