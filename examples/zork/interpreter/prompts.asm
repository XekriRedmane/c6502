HARGS	EQU	$00
SSP	EQU	$0A
FP	EQU	$0C
DPTR	EQU	$0E
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
RND	EQU	$4E
SECTORS_PER_TRACK	EQU	$7F
CURR_OPCODE	EQU	$80
OPERAND_COUNT	EQU	$81
OPERANDS	EQU	$82
Z_PC	EQU	$8A
ZCODE_PAGE_ADDR	EQU	$8D
ZCODE_PAGE_VALID	EQU	$8F
PAGE_TABLE_INDEX	EQU	$90
Z_PC2	EQU	$91
ZCODE_PAGE_ADDR2	EQU	$94
ZCODE_PAGE_VALID2	EQU	$96
PAGE_TABLE_INDEX2	EQU	$97
GLOBAL_ZVARS_ADDR	EQU	$98
LOCAL_ZVARS	EQU	$9A
HIGH_MEM_ADDR	EQU	$B8
Z_HEADER_ADDR	EQU	$BA
NUM_IMAGE_PAGES	EQU	$BC
NUM_PAGE_TABLE_ENTRIES	EQU	$BD
FIRST_Z_PAGE	EQU	$BE
LAST_Z_PAGE	EQU	$BF
PAGE_L_TABLE	EQU	$C0
PAGE_H_TABLE	EQU	$C2
NEXT_PAGE_TABLE	EQU	$C4
PREV_PAGE_TABLE	EQU	$C6
STACK_COUNT	EQU	$C8
Z_SP	EQU	$C9
FRAME_Z_SP	EQU	$CB
FRAME_STACK_COUNT	EQU	$CD
SHIFT_ALPHABET	EQU	$CE
LOCKED_ALPHABET	EQU	$CF
ZDECOMPRESS_STATE	EQU	$D0
ZCHARS	EQU	$D1
ZCHAR_SCRATCH1	EQU	$D3
ZCHAR_SCRATCH2	EQU	$DA
__zpabi_branch__condition	EQU	$E0
__zpabi_cout_string__s_0	EQU	$E0
__zpabi_load_address__addr_0	EQU	$E0
__zpabi_load_packed_address__addr_0	EQU	$E0
__zpabi_print_ascii_string__s_0	EQU	$E0
__zpabi_print_number__n_0	EQU	$E0
__zpabi_push__value_0	EQU	$E0
__zpabi_return_value__value_0	EQU	$E0
__zpabi_store_result__value_0	EQU	$E0
__zpabi_var_put__value_0	EQU	$E0
__zpabi_cout_string__s_1	EQU	$E1
__zpabi_load_address__addr_1	EQU	$E1
__zpabi_load_packed_address__addr_1	EQU	$E1
__zpabi_print_ascii_string__s_1	EQU	$E1
__zpabi_print_number__n_1	EQU	$E1
__zpabi_push__value_1	EQU	$E1
__zpabi_return_value__value_1	EQU	$E1
__zpabi_store_result__value_1	EQU	$E1
__zpabi_var_put__value_1	EQU	$E1
Z_ABBREV_TABLE	EQU	$E2
__local_please_insert_save_diskette__0	EQU	$E4
__zpabi_read_from_sector__sector_0	EQU	$E4
__zpabi_write_to_sector__sector_0	EQU	$E4
__local_please_insert_save_diskette__1	EQU	$E5
__zpabi_read_from_sector__sector_1	EQU	$E5
__zpabi_write_to_sector__sector_1	EQU	$E5
__local_get_prompted_number_from_user__key	EQU	$E6
__zpabi_read_from_sector__buffer_0	EQU	$E6
__zpabi_write_to_sector__buffer_0	EQU	$E6
__zpabi_read_from_sector__buffer_1	EQU	$E7
__zpabi_write_to_sector__buffer_1	EQU	$E7
__local_get_prompted_number_from_user__1	EQU	$E8
BUFF_END	EQU	$EB
BUFF_LINE_LEN	EQU	$EC
CURR_LINE	EQU	$ED
PRINTER_CSW	EQU	$EE
BUFF_AREA	EQU	$0200
SLOT1_SCRATCH	EQU	$0779
restart	EQU	$0800
RWTS	EQU	$2900
VTAB	EQU	$FC22
HOME	EQU	$FC58
CLREOL	EQU	$FC9C
WAIT	EQU	$FCA8
RDKEY	EQU	$FD0C
GETLN1	EQU	$FD6F
COUT	EQU	$FDED
COUT1	EQU	$FDF0

; @zp-link-meta-begin
; def get_prompted_number_from_user params=__zpabi_get_prompted_number_from_user__which locals=__local_get_prompted_number_from_user__key,__local_get_prompted_number_from_user__0,__local_get_prompted_number_from_user__1 indirect=false in_cycle=false param_regs=A
; def please_insert_save_diskette params= locals=__local_please_insert_save_diskette__0,__local_please_insert_save_diskette__1 indirect=false in_cycle=false
; def please_reinsert_game_diskette params= locals=__local_please_reinsert_game_diskette__0 indirect=false in_cycle=false
; ext A_mod_3 params=__zpabi_A_mod_3__a param_regs=A
; ext CLREOL params=
; ext COUT params=__zpabi_COUT__c param_regs=A
; ext COUT1 params=__zpabi_COUT1__c param_regs=A
; ext GETLN1 params=
; ext HOME params=
; ext RDKEY params=
; ext RWTS params=__zpabi_RWTS__iob_0,__zpabi_RWTS__iob_1 param_regs=Y,A
; ext VTAB params=
; ext WAIT params=__zpabi_WAIT__a param_regs=A
; ext branch params=__zpabi_branch__condition
; ext buffer_char params=__zpabi_buffer_char__c param_regs=A
; ext cout_string params=__zpabi_cout_string__s_0,__zpabi_cout_string__s_1,__zpabi_cout_string__count param_regs=,,X
; ext do_instruction params=
; ext dump_buffer_line params=
; ext dump_buffer_to_printer params=
; ext dump_buffer_to_screen params=
; ext dump_buffer_with_more params=
; ext get_alphabet params=
; ext get_next_code_byte params=
; ext get_next_code_byte2 params=
; ext get_next_code_word params=
; ext get_next_zchar params=
; ext get_object_addr params=__zpabi_get_object_addr__obj param_regs=A
; ext home params=
; ext instr_clear_attr params=
; ext instr_div params=
; ext instr_get_child params=
; ext instr_get_next_prop params=
; ext instr_get_parent params=
; ext instr_get_prop params=
; ext instr_get_prop_addr params=
; ext instr_get_prop_len params=
; ext instr_get_sibling params=
; ext instr_insert_obj params=
; ext instr_jin params=
; ext instr_mod params=
; ext instr_mul params=
; ext instr_put_prop params=
; ext instr_quit params=
; ext instr_random params=
; ext instr_remove_obj params=
; ext instr_restart params=
; ext instr_restore params=
; ext instr_save params=
; ext instr_set_attr params=
; ext instr_sread params=
; ext instr_test_attr params=
; ext internal_error params=
; ext load_address params=__zpabi_load_address__addr_0,__zpabi_load_address__addr_1
; ext load_packed_address params=__zpabi_load_packed_address__addr_0,__zpabi_load_packed_address__addr_1
; ext new_line params=
; ext pop params=
; ext print_ascii_string params=__zpabi_print_ascii_string__s_0,__zpabi_print_ascii_string__s_1,__zpabi_print_ascii_string__count param_regs=,,X
; ext print_number params=__zpabi_print_number__n_0,__zpabi_print_number__n_1
; ext print_obj params=__zpabi_print_obj__obj param_regs=A
; ext print_status_line params=
; ext print_string_literal params=
; ext print_zstring params=
; ext push params=__zpabi_push__value_0,__zpabi_push__value_1
; ext read_from_sector params=__zpabi_read_from_sector__sector_0,__zpabi_read_from_sector__sector_1,__zpabi_read_from_sector__buffer_0,__zpabi_read_from_sector__buffer_1
; ext reset_window params=
; ext restart params=
; ext return_value params=__zpabi_return_value__value_0,__zpabi_return_value__value_1
; ext store_result params=__zpabi_store_result__value_0,__zpabi_store_result__value_1
; ext var_get params=__zpabi_var_get__var param_regs=A
; ext var_put params=__zpabi_var_put__var,__zpabi_var_put__value_0,__zpabi_var_put__value_1 param_regs=A,,
; ext write_to_sector params=__zpabi_write_to_sector__sector_0,__zpabi_write_to_sector__sector_1,__zpabi_write_to_sector__buffer_0,__zpabi_write_to_sector__buffer_1
; call get_prompted_number_from_user -> CLREOL
; call get_prompted_number_from_user -> RDKEY
; call get_prompted_number_from_user -> cout_string
; call get_prompted_number_from_user -> dump_buffer_line
; call get_prompted_number_from_user -> dump_buffer_with_more
; call get_prompted_number_from_user -> print_ascii_string
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
; item get_prompted_number_from_user section=.text local=true zero=false
; item please_insert_save_diskette section=.text local=false zero=false
; item please_reinsert_game_diskette section=.text local=false zero=false
; item sPleaseInsert section=.rodata local=true zero=false
; item sDefault section=.rodata local=true zero=false
; item sReturnToBegin section=.rodata local=true zero=false
; item sReinsertGameDiskette section=.rodata local=true zero=false
; item sPressReturnToContinue section=.rodata local=true zero=false
; item prompts section=.data local=true zero=false
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
; zp RND size=2 at=$4E def=false init=
; zp SECTORS_PER_TRACK size=1 at=$7F def=false init=
; zp CURR_OPCODE size=1 at=$80 def=false init=
; zp OPERAND_COUNT size=1 at=$81 def=false init=
; zp OPERANDS size=8 at=$82 def=false init=
; zp Z_PC size=3 at=$8A def=false init=
; zp ZCODE_PAGE_ADDR size=2 at=$8D def=false init=
; zp ZCODE_PAGE_VALID size=1 at=$8F def=false init=
; zp PAGE_TABLE_INDEX size=1 at=$90 def=false init=
; zp Z_PC2 size=3 at=$91 def=false init=
; zp ZCODE_PAGE_ADDR2 size=2 at=$94 def=false init=
; zp ZCODE_PAGE_VALID2 size=1 at=$96 def=false init=
; zp PAGE_TABLE_INDEX2 size=1 at=$97 def=false init=
; zp GLOBAL_ZVARS_ADDR size=2 at=$98 def=false init=
; zp LOCAL_ZVARS size=30 at=$9A def=false init=
; zp HIGH_MEM_ADDR size=2 at=$B8 def=false init=
; zp Z_HEADER_ADDR size=2 at=$BA def=false init=
; zp NUM_IMAGE_PAGES size=1 at=$BC def=false init=
; zp NUM_PAGE_TABLE_ENTRIES size=1 at=$BD def=false init=
; zp FIRST_Z_PAGE size=1 at=$BE def=false init=
; zp LAST_Z_PAGE size=1 at=$BF def=false init=
; zp PAGE_L_TABLE size=2 at=$C0 def=false init=
; zp PAGE_H_TABLE size=2 at=$C2 def=false init=
; zp NEXT_PAGE_TABLE size=2 at=$C4 def=false init=
; zp PREV_PAGE_TABLE size=2 at=$C6 def=false init=
; zp STACK_COUNT size=1 at=$C8 def=false init=
; zp Z_SP size=2 at=$C9 def=false init=
; zp FRAME_Z_SP size=2 at=$CB def=false init=
; zp FRAME_STACK_COUNT size=1 at=$CD def=false init=
; zp SHIFT_ALPHABET size=1 at=$CE def=false init=
; zp LOCKED_ALPHABET size=1 at=$CF def=false init=
; zp ZDECOMPRESS_STATE size=1 at=$D0 def=false init=
; zp ZCHARS size=2 at=$D1 def=false init=
; zp ZCHAR_SCRATCH1 size=7 at=$D3 def=false init=
; zp ZCHAR_SCRATCH2 size=6 at=$DA def=false init=
; zp Z_ABBREV_TABLE size=2 at=$E2 def=false init=
; zp BUFF_END size=1 at=$EB def=false init=
; zp BUFF_LINE_LEN size=1 at=$EC def=false init=
; zp CURR_LINE size=1 at=$ED def=false init=
; zp PRINTER_CSW size=2 at=$EE def=false init=
; runtime hargs_size=10 hargs_need=0
; @zp-link-meta-end

get_prompted_number_from_user:
   SUBROUTINE

   STA   __local_get_prompted_number_from_user__key
   JSR   dump_buffer_with_more
   LDA   __local_get_prompted_number_from_user__key
   ASL
   ASL
   ASL
   ASL
   STA   __local_get_prompted_number_from_user__1
   LDA   #<prompts
   CLC
   ADC   __local_get_prompted_number_from_user__1
   STA   __zpabi_print_ascii_string__s_0
   LDA   #>prompts
   ADC   #$00
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
   CLC
   LDA   #<(prompts+15)
   ADC   __local_get_prompted_number_from_user__1
   STA   __zpabi_cout_string__s_0
   LDA   #>(prompts+15)
   ADC   #$00
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
   LDX   __local_get_prompted_number_from_user__1
   LDA   prompts+15,X
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
.loop@0_start:
   LDA   #$02
   JSR   get_prompted_number_from_user
   CMP   #$30
   BCC   .loop@0_start
   CMP   #$38
   BCS   .loop@0_start
   STA   prompts+47
   JSR   buffer_char
.loop@1_start:
   LDA   #$00
   JSR   get_prompted_number_from_user
   STA   __local_please_insert_save_diskette__1
   CMP   #$31
   BCC   .loop@1_start
   CMP   #$38
   BCS   .loop@1_start
   AND   #$07
   ASL
   ASL
   ASL
   ASL
   STA   iob+1
   LDA   __local_please_insert_save_diskette__1
   STA   prompts+15
   JSR   buffer_char
.loop@2_start:
   LDA   #$01
   JSR   get_prompted_number_from_user
   STA   __local_please_insert_save_diskette__0
   CMP   #$31
   BCC   .loop@2_start
   CMP   #$33
   BCS   .loop@2_start
   AND   #$03
   STA   iob+2
   LDA   __local_please_insert_save_diskette__0
   STA   prompts+31
   JSR   buffer_char
.loop@3_start:
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
   BNE   .loop@3_start
   LDA   prompts+47
   AND   #$07
   STA   __local_please_insert_save_diskette__0
   LSR
   LSR
   STA   __local_please_insert_save_diskette__1
   LDA   __local_please_insert_save_diskette__0
   ROR
   ROR
   ROR
   AND   #$C0
   SEC
   SBC   #$01
   STA   __local_please_insert_save_diskette__0
   BCS   .carry_inc@0
   DEC   __local_please_insert_save_diskette__1
.carry_inc@0:
   JSR   dump_buffer_with_more
   LDX   __local_please_insert_save_diskette__1
   LDA   __local_please_insert_save_diskette__0
   RTS

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
.loop@4_start:
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
   BNE   .loop@4_start
   JSR   dump_buffer_with_more
.if_end@9:
   LDA   #$60
   STA   iob+1
   LDA   #$01
   STA   iob+2
   RTS

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

prompts:
   DC.B  $53, $4C, $4F, $54, $20, $20, $20, $20, $20, $28, $31, $2D, $37, $29, $3A
   DC.B  $36
   DC.B  $44, $52, $49, $56, $45, $20, $20, $20, $20, $28, $31, $2D, $32, $29, $3A
   DC.B  $32
   DC.B  $50, $4F, $53, $49, $54, $49, $4F, $4E, $20, $28, $30, $2D, $37, $29, $3A
   DC.B  $30
