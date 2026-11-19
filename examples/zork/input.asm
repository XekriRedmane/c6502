HARGS	EQU	$00
SSP	EQU	$01
FP	EQU	$03
DPTR	EQU	$05
get_dictionary_addr@ret	EQU	$07
input_ptr	EQU	$09
char_count	EQU	$0A
input_count	EQU	$0B
token_idx	EQU	$0C
parse	EQU	$0D
text	EQU	$0F
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
__local_get_alphabet_for_char__c	EQU	$E0
__zpabi_cout_string__s_0	EQU	$E0
__zpabi_print_ascii_string__s_0	EQU	$E0
__zpabi_cout_string__s_1	EQU	$E1
__zpabi_print_ascii_string__s_1	EQU	$E1
Z_ABBREV_TABLE	EQU	$E2
__local_instr_sread__1	EQU	$E5
__local_read_line__c	EQU	$E6
__local_z_compress__0	EQU	$E6
__local_is_separator__c	EQU	$E7
__local_print_status_line__1	EQU	$E7
__local_read_line__left	EQU	$E7
__local_z_compress__1	EQU	$E7
__local_get_dictionary_addr__0	EQU	$E8
__local_print_status_line__2	EQU	$E8
__local_read_line__n	EQU	$E8
__local_z_compress__2	EQU	$E8
__local_z_compress__3	EQU	$E9
BUFF_END	EQU	$EB
BUFF_LINE_LEN	EQU	$EC
CURR_LINE	EQU	$ED
PRINTER_CSW	EQU	$EE
__local_ascii_to_zchar__z	EQU	$F0
__local_match_dictionary_word__0	EQU	$F0
__local_ascii_to_zchar__dest	EQU	$F1
__local_ascii_to_zchar__left	EQU	$F2
__local_ascii_to_zchar__0	EQU	$F3
__local_ascii_to_zchar__1	EQU	$F4
__local_is_dict_separator__c	EQU	$F4
__local_match_dictionary_word__count_0	EQU	$F4
__local_ascii_to_zchar__2	EQU	$F5
__local_ascii_to_zchar__index	EQU	$F6
__local_match_dictionary_word__5	EQU	$F6
__local_match_dictionary_word__6	EQU	$F7
__local_match_dictionary_word__count_1	EQU	$F9
__local_match_dictionary_word__8	EQU	$FA
__local_match_dictionary_word__entry_0	EQU	$FB
__local_match_dictionary_word__entry_1	EQU	$FC
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
; def ascii_to_zchar params= locals=__local_ascii_to_zchar__z,__local_ascii_to_zchar__dest,__local_ascii_to_zchar__left,__local_ascii_to_zchar__0,__local_ascii_to_zchar__1,__local_ascii_to_zchar__2,__local_ascii_to_zchar__index indirect=false in_cycle=false
; def get_alphabet_for_char params=__zpabi_get_alphabet_for_char__c locals=__local_get_alphabet_for_char__c indirect=false in_cycle=false param_regs=A
; def get_dictionary_addr params= locals=__local_get_dictionary_addr__0,__local_get_dictionary_addr__1 indirect=false in_cycle=false
; def instr_sread params= locals=__local_instr_sread__0,__local_instr_sread__1 indirect=false in_cycle=false
; def is_dict_separator params=__zpabi_is_dict_separator__c locals=__local_is_dict_separator__0,__local_is_dict_separator__n,__local_is_dict_separator__1,__local_is_dict_separator__2,__local_is_dict_separator__c indirect=false in_cycle=false param_regs=A carry=true
; def is_separator params=__zpabi_is_separator__c locals=__local_is_separator__0,__local_is_separator__c indirect=false in_cycle=false param_regs=A carry=true
; def is_std_separator params=__zpabi_is_std_separator__c locals=__local_is_std_separator__0,__local_is_std_separator__c indirect=false in_cycle=false param_regs=A carry=true
; def match_dictionary_word params= locals=__local_match_dictionary_word__0,__local_match_dictionary_word__1,__local_match_dictionary_word__2,__local_match_dictionary_word__3,__local_match_dictionary_word__count_0,__local_match_dictionary_word__4,__local_match_dictionary_word__5,__local_match_dictionary_word__6,__local_match_dictionary_word__7,__local_match_dictionary_word__count_1,__local_match_dictionary_word__8,__local_match_dictionary_word__entry_0,__local_match_dictionary_word__entry_1 indirect=false in_cycle=false
; def print_status_line params= locals=__local_print_status_line__0,__local_print_status_line__1,__local_print_status_line__2 indirect=false in_cycle=false
; def read_line params= locals=__local_read_line__c,__local_read_line__left,__local_read_line__n indirect=false in_cycle=false
; def search_nonalpha_table params=__zpabi_search_nonalpha_table__c locals=__local_search_nonalpha_table__0,__local_search_nonalpha_table__c indirect=false in_cycle=false param_regs=A
; def skip_separators params= locals=__local_skip_separators__0 indirect=false in_cycle=false
; def z_compress params= locals=__local_z_compress__0,__local_z_compress__1,__local_z_compress__2,__local_z_compress__3 indirect=false in_cycle=false
; ext A_mod_3 params=__zpabi_A_mod_3__a param_regs=A
; ext CLREOL params=
; ext COUT params=__zpabi_COUT__c param_regs=A
; ext COUT1 params=__zpabi_COUT1__c param_regs=A
; ext GETLN1 params=
; ext HOME params=
; ext RDKEY params=
; ext RWTS params=__zpabi_RWTS__iob_0,__zpabi_RWTS__iob_1 param_regs=Y,A carry=true
; ext VTAB params=
; ext WAIT params=__zpabi_WAIT__a param_regs=A
; ext branch params=__zpabi_branch__condition param_regs=A
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
; ext get_next_zchar params= carry=true
; ext home params=
; ext inc_sector_and_read params= carry=true
; ext inc_sector_and_write params= carry=true
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
; ext instr_test_attr params=
; ext internal_error params=
; ext load_address params=__zpabi_load_address__addr_0,__zpabi_load_address__addr_1 param_regs=A,X
; ext load_packed_address params=__zpabi_load_packed_address__addr_0,__zpabi_load_packed_address__addr_1 param_regs=A,X
; ext new_line params=
; ext please_insert_save_diskette params=
; ext please_reinsert_game_diskette params=
; ext print_ascii_string params=__zpabi_print_ascii_string__s_0,__zpabi_print_ascii_string__s_1,__zpabi_print_ascii_string__count param_regs=,,X
; ext print_number params=__zpabi_print_number__n_0,__zpabi_print_number__n_1 param_regs=A,X
; ext print_obj params=__zpabi_print_obj__obj param_regs=A
; ext print_string_literal params=
; ext print_zstring params=
; ext read_from_sector params= carry=true
; ext read_next_sector params= carry=true
; ext reset_window params=
; ext restart params=
; ext return_value params=__zpabi_return_value__value_0,__zpabi_return_value__value_1 param_regs=A,X
; ext store_result params=__zpabi_store_result__value_0,__zpabi_store_result__value_1 param_regs=A,X
; ext var_get params=__zpabi_var_get__var param_regs=A
; ext write_next_sector params= carry=true
; ext write_to_sector params= carry=true
; call ascii_to_zchar -> A_mod_3
; call ascii_to_zchar -> get_alphabet_for_char
; call ascii_to_zchar -> search_nonalpha_table
; call ascii_to_zchar -> z_compress
; call instr_sread -> ascii_to_zchar
; call instr_sread -> is_dict_separator
; call instr_sread -> is_separator
; call instr_sread -> is_std_separator
; call instr_sread -> match_dictionary_word
; call instr_sread -> print_status_line
; call instr_sread -> read_line
; call instr_sread -> skip_separators
; call is_dict_separator -> get_dictionary_addr
; call is_separator -> is_dict_separator
; call is_separator -> is_std_separator
; call match_dictionary_word -> get_dictionary_addr
; call print_status_line -> CLREOL
; call print_status_line -> VTAB
; call print_status_line -> buffer_char
; call print_status_line -> cout_string
; call print_status_line -> dump_buffer_line
; call print_status_line -> dump_buffer_to_screen
; call print_status_line -> print_number
; call print_status_line -> print_obj
; call print_status_line -> var_get
; call read_line -> GETLN1
; call read_line -> dump_buffer_line
; call read_line -> dump_buffer_to_printer
; call skip_separators -> is_separator
; item print_status_line section=.text local=false zero=false
; item read_line section=.text local=true zero=false
; item get_dictionary_addr section=.text local=true zero=false
; item is_dict_separator section=.text local=true zero=false
; item is_std_separator section=.text local=true zero=false
; item is_separator section=.text local=true zero=false
; item skip_separators section=.text local=true zero=false
; item get_alphabet_for_char section=.text local=true zero=false
; item search_nonalpha_table section=.text local=true zero=false
; item z_compress section=.text local=true zero=false
; item ascii_to_zchar section=.text local=true zero=false
; item match_dictionary_word section=.text local=true zero=false
; item instr_sread section=.text local=false zero=false
; item sScore section=.rodata local=true zero=false
; item SEPARATORS_TABLE section=.rodata local=true zero=false
; item __zp_init section=.text local=false zero=false
; zp get_dictionary_addr@ret size=2 at=auto def=true init= local=true
; zp input_ptr size=1 at=auto def=true init=$00 weight=145 local=true
; zp char_count size=1 at=auto def=true init=$00 weight=129 local=true
; zp input_count size=1 at=auto def=true init=$00 weight=121 local=true
; zp token_idx size=1 at=auto def=true init=$00 weight=49 local=true
; zp parse size=2 at=auto def=true init=$00,$00 weight=58 local=true
; zp text size=2 at=auto def=true init=$00,$00 weight=51 local=true
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
; zp disk_buffer size=2 at=auto def=false init=
; zp disk_sector size=2 at=auto def=false init=
; runtime hargs_size=1 hargs_need=0
; @zp-link-meta-end

print_status_line:
   SUBROUTINE

   JSR   dump_buffer_line
   LDA   CH
   STA   __local_print_status_line__2
   LDA   CV
   STA   __local_print_status_line__1
   LDA   #$00
   STA   CH
   STA   CV
   JSR   VTAB
   LDA   #$3F
   STA   INVFLG
   JSR   CLREOL
   LDA   #$10
   JSR   var_get
   JSR   print_obj
   JSR   dump_buffer_to_screen
   LDA   #$19
   STA   CH
   LDA   #<sScore
   STA   __zpabi_cout_string__s_0
   LDA   #>sScore
   STA   __zpabi_cout_string__s_1
   LDX   #$06
   JSR   cout_string
   LDA   CH
   CLC
   ADC   #$01
   STA   CH
   LDA   #$11
   JSR   var_get
   JSR   print_number
   LDA   #$2F
   JSR   buffer_char
   LDA   #$12
   JSR   var_get
   JSR   print_number
   JSR   dump_buffer_to_screen
   LDA   #$FF
   STA   INVFLG
   LDA   __local_print_status_line__1
   STA   CV
   LDA   __local_print_status_line__2
   STA   CH
   JMP   VTAB

read_line:
   SUBROUTINE

   JSR   dump_buffer_line
   LDA   WNDTOP
   STA   CURR_LINE
   JSR   GETLN1
   INC   CURR_LINE
   LDX   #$00
   BEQ   .loop@0_continue
.read_line@rot@0:
   INX
.loop@0_continue:
   LDA   BUFF_AREA,X
   CMP   #$8D
   BNE   .read_line@rot@0
   INX
   STX   __local_read_line__n
   LDY   #$11
   LDA   (Z_HEADER_ADDR),Y
   LSR
   BCC   .if_end@0
   STX   BUFF_END
   JSR   dump_buffer_to_printer
   LDA   #$00
   STA   BUFF_END
.if_end@0:
   LDY   #$00
   LDA   __local_read_line__n
   CMP   (text),Y
   BCC   .if_end@1
   LDA   (text),Y
   STA   __local_read_line__n
.if_end@1:
   ORA   #$00
   BEQ   .if_end@2
   LDX   #$00
   STA   __local_read_line__left
.loop@1_start:
   LDA   BUFF_AREA,X
   AND   #$7F
   STA   __local_read_line__c
   CMP   #$41
   BCC   .if_end@5
   CMP   #$5B
   BCS   .if_end@5
   ORA   #$20
   STA   __local_read_line__c
.if_end@5:
   INX
   TXA
   TAY
   LDA   __local_read_line__c
   STA   (text),Y
   CMP   #$0D
   BEQ   .loop@1_break
   DEC   __local_read_line__left
   BNE   .loop@1_start
.loop@1_break:
.if_end@2:
   LDA   __local_read_line__n
   RTS

get_dictionary_addr:
   SUBROUTINE

   LDY   #$08
   LDA   (Z_HEADER_ADDR),Y
   STA   __local_get_dictionary_addr__0
   INY
   LDA   (Z_HEADER_ADDR),Y
   CLC
   ADC   Z_HEADER_ADDR
   STA   get_dictionary_addr@ret
   LDA   Z_HEADER_ADDR+1
   ADC   __local_get_dictionary_addr__0
   STA   get_dictionary_addr@ret+1
   RTS

is_dict_separator:
   SUBROUTINE

   STA   __local_is_dict_separator__c
   JSR   get_dictionary_addr
   LDY   #$00
   LDA   (get_dictionary_addr@ret),Y
   TAX
   LDA   __local_is_dict_separator__c
   BEQ   .tail@1
.loop@2_start:
   INY
   CMP   (get_dictionary_addr@ret),Y
   BNE   .if_end@8
   SEC
   RTS
.if_end@8:
   DEX
   BNE   .loop@2_start
.tail@1:
   CLC
   RTS

is_std_separator:
   SUBROUTINE

   LDX   #$00
.is_std_separator@rot@1:
   CMP   SEPARATORS_TABLE,X
   BNE   .if_end@10
   SEC
   RTS
.if_end@10:
   INX
   CPX   #$08
   BNE   .is_std_separator@rot@1
   CLC
   RTS

is_separator:
   SUBROUTINE

   STA   __local_is_separator__c
   JSR   is_dict_separator
   BCC   .if_end@11
   RTS
.if_end@11:
   LDA   __local_is_separator__c
   JMP   is_std_separator

skip_separators:
   SUBROUTINE

   JMP   .loop@4_continue
.skip_separators@rot@2:
   LDY   input_ptr
   LDA   (text),Y
   JSR   is_separator
   BCC   .if_end@12
   RTS
.if_end@12:
   INC   input_ptr
   DEC   input_count
   INC   char_count
.loop@4_continue:
   LDA   input_count
   BNE   .skip_separators@rot@2
   RTS

get_alphabet_for_char:
   SUBROUTINE

   STA   __local_get_alphabet_for_char__c
   CMP   #$61
   BCC   .if_end@15
   CMP   #$7B
   BCS   .if_end@15
   LDA   #$00
   RTS
.if_end@15:
   CMP   #$41
   BCC   .if_end@18
   CMP   #$5B
   BCS   .if_end@18
   LDA   #$01
   RTS
.if_end@18:
   ORA   #$00
   BEQ   .get_alphabet_for_char@scfold@0
   BPL   .if_end@21
.get_alphabet_for_char@scfold@0:
   LDA   __local_get_alphabet_for_char__c
   RTS
.if_end@21:
   LDA   #$02
   RTS

search_nonalpha_table:
   SUBROUTINE

   LDX   #$17
.loop@5_start:
   CMP   a2_table,X
   BNE   .if_end@22
   TXA
   CLC
   ADC   #$08
   RTS
.if_end@22:
   DEX
   CPX   #$FF
   BNE   .loop@5_start
   LDA   #$00
   RTS

z_compress:
   SUBROUTINE

   LDA   ZCHAR_SCRATCH2
   ASL
   ASL
   STA   __local_z_compress__3
   LDA   ZCHAR_SCRATCH2+1
   LSR
   LSR
   LSR
   STA   __local_z_compress__0
   LDA   ZCHAR_SCRATCH2+1
   ASL
   ASL
   ASL
   ASL
   ASL
   STA   __local_z_compress__2
   LDA   __local_z_compress__3
   ORA   __local_z_compress__0
   STA   __local_z_compress__1
   LDA   __local_z_compress__2
   ORA   ZCHAR_SCRATCH2+2
   STA   ZCHAR_SCRATCH2
   LDA   __local_z_compress__1
   STA   ZCHAR_SCRATCH2+1
   LDA   ZCHAR_SCRATCH2+3
   ASL
   ASL
   STA   __local_z_compress__2
   LDA   ZCHAR_SCRATCH2+4
   LSR
   LSR
   LSR
   STA   __local_z_compress__0
   LDA   ZCHAR_SCRATCH2+4
   ASL
   ASL
   ASL
   ASL
   ASL
   STA   __local_z_compress__1
   LDA   __local_z_compress__2
   ORA   __local_z_compress__0
   STA   __local_z_compress__2
   LDA   __local_z_compress__1
   ORA   ZCHAR_SCRATCH2+5
   STA   ZCHAR_SCRATCH2+2
   LDA   __local_z_compress__2
   ORA   #$80
   STA   ZCHAR_SCRATCH2+3
   RTS

ascii_to_zchar:
   SUBROUTINE

   LDA   #$00
   STA   LOCKED_ALPHABET
   LDX   #$00
.ascii_to_zchar@rot@3:
   LDA   #$05
   STA   ZCHAR_SCRATCH2,X
   INX
   CPX   #$06
   BNE   .ascii_to_zchar@rot@3
   LDA   #$06
   STA   __local_ascii_to_zchar__left
   LDA   #$00
   STA   __local_ascii_to_zchar__dest
   STA   __local_ascii_to_zchar__index
.loop@7_start:
   LDX   __local_ascii_to_zchar__index
   LDA   ZCHAR_SCRATCH1,X
   STA   __local_ascii_to_zchar__2
   INC   __local_ascii_to_zchar__index
   ORA   #$00
   BNE   .if_else@24
   LDA   #$05
   JMP   .if_end@23
.if_else@24:
   JSR   get_alphabet_for_char
   STA   __local_ascii_to_zchar__1
   CMP   LOCKED_ALPHABET
   BEQ   .if_end@25
   SEC
   SBC   LOCKED_ALPHABET
   CLC
   ADC   #$03
   JSR   A_mod_3
   STA   __local_ascii_to_zchar__0
   LDX   __local_ascii_to_zchar__index
   LDA   ZCHAR_SCRATCH1,X
   JSR   get_alphabet_for_char
   CMP   __local_ascii_to_zchar__1
   BNE   .if_else@27
   LDA   __local_ascii_to_zchar__0
   CLC
   ADC   #$03
   STA   __local_ascii_to_zchar__z
   LDA   __local_ascii_to_zchar__1
   STA   LOCKED_ALPHABET
   JMP   .if_end@26
.if_else@27:
   LDA   __local_ascii_to_zchar__0
   CLC
   ADC   #$01
   STA   __local_ascii_to_zchar__z
.if_end@26:
   LDX   __local_ascii_to_zchar__dest
   LDA   __local_ascii_to_zchar__z
   STA   ZCHAR_SCRATCH2,X
   DEC   __local_ascii_to_zchar__left
   BEQ   .loop@7_break
   INC   __local_ascii_to_zchar__dest
.if_end@25:
   LDA   __local_ascii_to_zchar__2
   JSR   get_alphabet_for_char
   SEC
   SBC   #$01
   BPL   .if_else@30
   LDA   __local_ascii_to_zchar__2
   SEC
   SBC   #$5B
   JMP   .if_end@29
.if_else@30:
   BNE   .if_else@32
   LDA   __local_ascii_to_zchar__2
   SEC
   SBC   #$3B
   JMP   .if_end@31
.if_else@32:
   LDA   __local_ascii_to_zchar__2
   JSR   search_nonalpha_table
   ORA   #$00
   BNE   .if_end@33
   LDX   __local_ascii_to_zchar__dest
   LDA   #$06
   STA   ZCHAR_SCRATCH2,X
   INX
   DEC   __local_ascii_to_zchar__left
   BEQ   .loop@7_break
   LDA   __local_ascii_to_zchar__2
   LSR
   LSR
   LSR
   LSR
   LSR
   AND   #$03
   STA   ZCHAR_SCRATCH2,X
   INX
   STX   __local_ascii_to_zchar__dest
   DEC   __local_ascii_to_zchar__left
   BEQ   .loop@7_break
   LDA   __local_ascii_to_zchar__2
   AND   #$1F
.if_end@33:
.if_end@31:
.if_end@29:
.if_end@23:
   LDX   __local_ascii_to_zchar__dest
   STA   ZCHAR_SCRATCH2,X
   INC   __local_ascii_to_zchar__dest
   DEC   __local_ascii_to_zchar__left
   BEQ   .lb_skip@0
   JMP   .loop@7_start
.lb_skip@0:
.loop@7_break:
   JMP   z_compress

match_dictionary_word:
   SUBROUTINE

   JSR   get_dictionary_addr
   LDA   get_dictionary_addr@ret+1
   STA   __local_match_dictionary_word__entry_1
   LDY   #$00
   LDA   (get_dictionary_addr@ret),Y
   TAY
   INY
   LDA   (get_dictionary_addr@ret),Y
   STA   __local_match_dictionary_word__8
   ASL
   ASL
   ASL
   ASL
   STA   __local_match_dictionary_word__6
   INY
   LDA   (get_dictionary_addr@ret),Y
   INY
   STA   __local_match_dictionary_word__count_1
   LDA   (get_dictionary_addr@ret),Y
   STA   __local_match_dictionary_word__5
   INY
   TYA
   CLC
   ADC   get_dictionary_addr@ret
   STA   __local_match_dictionary_word__entry_0
   BCC   .carry_inc@0
   INC   __local_match_dictionary_word__entry_1
.carry_inc@0:
.loop@8_start:
   LDA   __local_match_dictionary_word__entry_0
   CLC
   ADC   __local_match_dictionary_word__6
   STA   __local_match_dictionary_word__entry_0
   BCC   .carry_inc@1
   INC   __local_match_dictionary_word__entry_1
.carry_inc@1:
   LDA   __local_match_dictionary_word__5
   SEC
   SBC   #$10
   STA   __local_match_dictionary_word__5
   BCS   .carry_inc@2
   DEC   __local_match_dictionary_word__count_1
.carry_inc@2:
   LDA   __local_match_dictionary_word__count_1
   BMI   .loop@8_break
   LDY   #$00
   LDA   (__local_match_dictionary_word__entry_0),Y
   CMP   ZCHAR_SCRATCH2+1
   BCC   .loop@8_start
.loop@8_break:
   LDA   __local_match_dictionary_word__entry_0
   SEC
   SBC   __local_match_dictionary_word__6
   STA   __local_match_dictionary_word__entry_0
   BCS   .carry_inc@3
   DEC   __local_match_dictionary_word__entry_1
.carry_inc@3:
   LDA   __local_match_dictionary_word__5
   CLC
   ADC   #$10
   STA   __local_match_dictionary_word__count_0
   BCC   .carry_inc@4
   INC   __local_match_dictionary_word__count_1
.carry_inc@4:
.loop@9_start:
   LDY   #$00
   LDA   ZCHAR_SCRATCH2+1
   CMP   (__local_match_dictionary_word__entry_0),Y
   BCC   .tail@0
   BNE   .if_end@41
   INY
   LDA   ZCHAR_SCRATCH2
   CMP   (__local_match_dictionary_word__entry_0),Y
   BCC   .tail@0
   BNE   .if_end@41
   INY
   LDA   ZCHAR_SCRATCH2+3
   CMP   (__local_match_dictionary_word__entry_0),Y
   BCC   .tail@0
   BNE   .if_end@43
   INY
   LDA   ZCHAR_SCRATCH2+2
   CMP   (__local_match_dictionary_word__entry_0),Y
   BCC   .tail@0
   BNE   .if_end@43
   LDA   __local_match_dictionary_word__entry_0
   SBC   Z_HEADER_ADDR
   STA   __local_match_dictionary_word__0
   LDA   __local_match_dictionary_word__entry_1
   SBC   Z_HEADER_ADDR+1
   TAX
   LDA   __local_match_dictionary_word__0
   RTS
.if_end@43:
.if_end@41:
   LDA   __local_match_dictionary_word__entry_0
   CLC
   ADC   __local_match_dictionary_word__8
   STA   __local_match_dictionary_word__entry_0
   BCC   .carry_inc@5
   INC   __local_match_dictionary_word__entry_1
.carry_inc@5:
   LDA   __local_match_dictionary_word__count_0
   SEC
   SBC   #$01
   STA   __local_match_dictionary_word__count_0
   BCS   .carry_inc@6
   DEC   __local_match_dictionary_word__count_1
.carry_inc@6:
   ORA   __local_match_dictionary_word__count_1
   BNE   .loop@9_start
.tail@0:
   LDX   #$00
   TXA
   RTS

instr_sread:
   SUBROUTINE

   JSR   print_status_line
   LDA   Z_HEADER_ADDR
   CLC
   ADC   OPERANDS
   STA   text
   LDA   Z_HEADER_ADDR+1
   ADC   OPERANDS+1
   STA   text+1
   LDA   Z_HEADER_ADDR
   CLC
   ADC   OPERANDS+2
   STA   parse
   LDA   Z_HEADER_ADDR+1
   ADC   OPERANDS+3
   STA   parse+1
   JSR   read_line
   STA   input_count
   LDA   #$00
   STA   char_count
   LDY   #$01
   STA   (parse),Y
   LDA   #$02
   STA   token_idx
   LDA   #$01
   STA   input_ptr
.loop@10_start:
   LDY   #$00
   LDA   (parse),Y
   INY
   CMP   (parse),Y
   BNE   .if_end@45
   RTS
.if_end@45:
   LDA   input_count
   BNE   .if_end@48
   LDA   char_count
   BNE   .vthread@0
   RTS
.if_end@48:
   LDA   char_count
.vthread@0:
   CMP   #$06
   BNE   .if_end@49
   JSR   skip_separators
.if_end@49:
   LDA   char_count
   BNE   .if_end@50
   LDX   #$00
.instr_sread@rot@4:
   LDA   #$00
   STA   ZCHAR_SCRATCH1,X
   INX
   CPX   #$06
   BNE   .instr_sread@rot@4
   LDA   token_idx
   CLC
   ADC   #$03
   TAY
   LDA   input_ptr
   STA   (parse),Y
   TAY
   LDA   (text),Y
   STA   __local_instr_sread__1
   JSR   is_dict_separator
   BCC   .if_end@51
   LDA   __local_instr_sread__1
   STA   ZCHAR_SCRATCH1
   INC   char_count
   DEC   input_count
   INC   input_ptr
   JMP   .instr_sread@search
.if_end@51:
   LDY   input_ptr
   LDA   (text),Y
   JSR   is_std_separator
   BCC   .if_end@52
   INC   input_ptr
   DEC   input_count
   JMP   .loop@10_start
.if_end@52:
.if_end@50:
   LDA   input_count
   BEQ   .if_end@55
   LDY   input_ptr
   LDA   (text),Y
   JSR   is_separator
   BCS   .if_end@55
   LDY   input_ptr
   LDA   (text),Y
   LDX   char_count
   STA   ZCHAR_SCRATCH1,X
   DEC   input_count
   INC   char_count
   INC   input_ptr
   JMP   .loop@10_start
.if_end@55:
.instr_sread@search:
   LDA   char_count
   BNE   .lb_skip@1
   JMP   .loop@10_start
.lb_skip@1:
   LDY   token_idx
   INY
   INY
   STA   (parse),Y
   JSR   ascii_to_zchar
   JSR   match_dictionary_word
   STA   __local_instr_sread__1
   TXA
   LDY   token_idx
   STA   (parse),Y
   INY
   LDA   __local_instr_sread__1
   STA   (parse),Y
   LDA   token_idx
   CLC
   ADC   #$04
   STA   token_idx
   CLC
   LDY   #$01
   LDA   (parse),Y
   ADC   #$01
   STA   (parse),Y
   LDA   #$00
   STA   char_count
   JMP   .loop@10_start

sScore:
   DC.B  $53, $43, $4F, $52, $45, $3A

SEPARATORS_TABLE:
   DC.B  $20, $2E, $2C, $3F, $0D, $0A, $09, $0C

__zp_init:
   SUBROUTINE

   LDA   #$00
   STA   text
   STA   text+1
   STA   parse
   STA   parse+1
   STA   input_count
   STA   char_count
   STA   input_ptr
   STA   token_idx
   RTS
