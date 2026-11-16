HARGS	EQU	$00
SSP	EQU	$01
FP	EQU	$03
DPTR	EQU	$05
disk_buffer	EQU	$07
disk_sector	EQU	$09
input_ptr	EQU	$0B
char_count	EQU	$0C
input_count	EQU	$0D
token_idx	EQU	$0E
parse	EQU	$0F
text	EQU	$11
props	EQU	$13
remainder	EQU	$15
attr_ptr	EQU	$17
sign_bit	EQU	$19
__local_print_status_line__1	EQU	$1A
__local_return_value__0	EQU	$1A
__local_print_status_line__2	EQU	$1B
__local_return_value__1	EQU	$1B
__local_divu16__partial_1	EQU	$1C
__local_print_obj__obj	EQU	$1C
__local_return_value__x	EQU	$1C
__local_divu16__partial_0	EQU	$1D
__local_print_obj__0	EQU	$1D
__local_return_value__count	EQU	$1D
__local_divu16__0	EQU	$1E
__local_print_obj__1	EQU	$1E
__local_store_result__0	EQU	$1E
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
__local_ascii_to_zchar__z	EQU	$50
__local_do_2op__equal	EQU	$50
__local_instr_call__0	EQU	$50
__local_match_dictionary_word__0	EQU	$50
__local_print_zstring__ch	EQU	$50
__local_ascii_to_zchar__dest	EQU	$51
__local_do_2op__0	EQU	$51
__local_instr_call__1	EQU	$51
__local_match_dictionary_word__count_0	EQU	$51
__local_print_zstring__c_0	EQU	$51
__local_ascii_to_zchar__left	EQU	$52
__local_do_2op__left	EQU	$52
__local_instr_call__2	EQU	$52
__local_match_dictionary_word__5	EQU	$52
__local_print_zstring__0	EQU	$52
__local_ascii_to_zchar__0	EQU	$53
__local_do_2op__2	EQU	$53
__local_instr_call__i	EQU	$53
__local_match_dictionary_word__6	EQU	$53
__local_ascii_to_zchar__1	EQU	$54
__local_do_2op__3	EQU	$54
__local_instr_call__args	EQU	$54
__local_match_dictionary_word__count_1	EQU	$54
__local_ascii_to_zchar__2	EQU	$55
__local_do_2op__4	EQU	$55
__local_instr_call__3	EQU	$55
__local_match_dictionary_word__8	EQU	$55
__local_ascii_to_zchar__index	EQU	$56
__local_do_2op__5	EQU	$56
__local_match_dictionary_word__entry_0	EQU	$56
__local_get_object_addr__0	EQU	$57
__local_match_dictionary_word__entry_1	EQU	$57
__local_print_ascii_string__count	EQU	$57
__local_get_object_addr__1	EQU	$58
__local_print_ascii_string__i	EQU	$58
__local_buffer_char__0	EQU	$59
__local_get_object_addr__2	EQU	$59
__local_dump_buffer_with_more__0	EQU	$5A
__local_get_object_addr__3	EQU	$5A
__local_dump_buffer_with_more__end	EQU	$5B
__local_get_object_addr__obj	EQU	$5B
__local_dump_buffer_to_printer__x	EQU	$5C
__local_dump_buffer_to_printer__1	EQU	$5D
__local_dump_buffer_to_printer__2	EQU	$5E
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
__local_get_const_word__1	EQU	$E0
__local_get_next_zchar__compound_literal_1	EQU	$E0
__local_instr_get_next_prop__y	EQU	$E0
__local_instr_get_parent__0	EQU	$E0
__local_is_dict_separator__1	EQU	$E0
__local_mulu16__product	EQU	$E0
__local_please_insert_save_diskette__0	EQU	$E0
__local_var_get__var	EQU	$E0
__local_write_to_sector__1	EQU	$E0
__zpabi_branch_to_offset__offset_0	EQU	$E0
__zpabi_check_sign__a_0	EQU	$E0
__zpabi_copy_data_from_buff__dest_0	EQU	$E0
__zpabi_copy_data_to_buff__src_0	EQU	$E0
__zpabi_get_alphabet_for_char__c	EQU	$E0
__zpabi_load_address__addr_0	EQU	$E0
__zpabi_load_packed_address__addr_0	EQU	$E0
__zpabi_print_number__n_0	EQU	$E0
__zpabi_return_value__value_0	EQU	$E0
__zpabi_search_nonalpha_table__c	EQU	$E0
__zpabi_set_attr_word__word_0	EQU	$E0
__zpabi_store_with_sign__n_0	EQU	$E0
__zpabi_var_put__value_0	EQU	$E0
__local_get_next_code_word__1	EQU	$E1
__local_instr_get_next_prop__1	EQU	$E1
__local_instr_get_parent__1	EQU	$E1
__local_is_dict_separator__2	EQU	$E1
__local_please_insert_save_diskette__1	EQU	$E1
__local_search_nonalpha_table__0	EQU	$E1
__local_var_get__0	EQU	$E1
__local_write_to_sector__2	EQU	$E1
__zpabi_branch_to_offset__offset_1	EQU	$E1
__zpabi_check_sign__a_1	EQU	$E1
__zpabi_copy_data_from_buff__dest_1	EQU	$E1
__zpabi_copy_data_to_buff__src_1	EQU	$E1
__zpabi_load_address__addr_1	EQU	$E1
__zpabi_load_packed_address__addr_1	EQU	$E1
__zpabi_print_number__n_1	EQU	$E1
__zpabi_return_value__value_1	EQU	$E1
__zpabi_set_attr_word__word_1	EQU	$E1
__zpabi_store_with_sign__n_1	EQU	$E1
__zpabi_var_put__value_1	EQU	$E1
Z_ABBREV_TABLE	EQU	$E2
__local_dec_var__0	EQU	$E4
__local_inc_var__0	EQU	$E4
__local_instr_clear_attr__0	EQU	$E4
__local_instr_get_child__0	EQU	$E4
__local_instr_get_sibling__0	EQU	$E4
__local_instr_insert_obj__0	EQU	$E4
__local_instr_jin__2	EQU	$E4
__local_instr_put_prop__0	EQU	$E4
__local_instr_restore__pages	EQU	$E4
__local_instr_save__pages	EQU	$E4
__local_instr_test_attr__1	EQU	$E4
__local_locate_last_ram_page__2	EQU	$E4
__zpabi_check_sign__b_0	EQU	$E4
__zpabi_is_dict_separator__c	EQU	$E4
__zpabi_is_separator__c	EQU	$E4
__zpabi_is_std_separator__c	EQU	$E4
__local_dec_var__1	EQU	$E5
__local_inc_var__1	EQU	$E5
__local_instr_clear_attr__1	EQU	$E5
__local_instr_get_child__1	EQU	$E5
__local_instr_get_sibling__1	EQU	$E5
__local_instr_insert_obj__1	EQU	$E5
__local_instr_jin__3	EQU	$E5
__local_instr_put_prop__y	EQU	$E5
__local_instr_restore__1	EQU	$E5
__local_instr_sread__1	EQU	$E5
__local_locate_last_ram_page__3	EQU	$E5
__zpabi_check_sign__b_1	EQU	$E5
__local_branch_to_offset__0	EQU	$E6
__local_dec_var__2	EQU	$E6
__local_get_dictionary_addr__0	EQU	$E6
__local_get_next_code_byte2__page	EQU	$E6
__local_get_prompted_number_from_user__key	EQU	$E6
__local_inc_var__2	EQU	$E6
__local_instr_clear_attr__3	EQU	$E6
__local_instr_insert_obj__2	EQU	$E6
__local_instr_put_prop__1	EQU	$E6
__local_locate_last_ram_page__page	EQU	$E6
__local_read_line__c	EQU	$E6
__local_z_compress__0	EQU	$E6
__zpabi_divu16__dividend_0	EQU	$E6
__zpabi_mulu16__a_0	EQU	$E6
__zpabi_store_result__value_0	EQU	$E6
__local_attr_ptr_and_mask__attr	EQU	$E7
__local_branch_to_offset__1	EQU	$E7
__local_get_dictionary_addr__1	EQU	$E7
__local_get_nonstack_var__var	EQU	$E7
__local_get_prompted_number_from_user__1	EQU	$E7
__local_instr_insert_obj__3	EQU	$E7
__local_instr_put_prop__2	EQU	$E7
__local_read_line__left	EQU	$E7
__local_var_put__var	EQU	$E7
__local_z_compress__1	EQU	$E7
__zpabi_divu16__dividend_1	EQU	$E7
__zpabi_mulu16__a_1	EQU	$E7
__zpabi_store_result__value_1	EQU	$E7
__local_attr_ptr_and_mask__mask_1	EQU	$E8
__local_branch_to_offset__pages_1	EQU	$E8
__local_get_nonstack_var__0	EQU	$E8
__local_get_property_ptr__0	EQU	$E8
__local_instr_insert_obj__4	EQU	$E8
__local_next_property__y	EQU	$E8
__local_pop__1	EQU	$E8
__local_read_line__n	EQU	$E8
__local_z_compress__2	EQU	$E8
__zpabi_divu16__divisor_0	EQU	$E8
__zpabi_find_index_of_page_table__page_0	EQU	$E8
__zpabi_mulu16__b_0	EQU	$E8
__zpabi_push__value_0	EQU	$E8
__zpabi_set_nonstack_var__value_0	EQU	$E8
__local_attr_ptr_and_mask__n	EQU	$E9
__local_branch_to_offset__2	EQU	$E9
__local_get_nonstack_var__1	EQU	$E9
__local_get_property_len__0	EQU	$E9
__local_get_property_ptr__1	EQU	$E9
__local_instr_insert_obj__5	EQU	$E9
__local_pop__2	EQU	$E9
__local_z_compress__3	EQU	$E9
__zpabi_divu16__divisor_1	EQU	$E9
__zpabi_find_index_of_page_table__page_1	EQU	$E9
__zpabi_mulu16__b_1	EQU	$E9
__zpabi_push__value_1	EQU	$E9
__zpabi_set_nonstack_var__value_1	EQU	$E9
__local_get_nonstack_var__2	EQU	$EA
__local_get_property_ptr__2	EQU	$EA
__local_store_and_branch_on__related	EQU	$EA
BUFF_END	EQU	$EB
BUFF_LINE_LEN	EQU	$EC
CURR_LINE	EQU	$ED
PRINTER_CSW	EQU	$EE
__local_get_next_code_byte__page	EQU	$F0
__local_print_number__count	EQU	$F0
__local_set_nonstack_var__var	EQU	$F0
__zpabi_print_ascii_string__s_0	EQU	$F0
__local_print_number__digits	EQU	$F1
__local_set_nonstack_var__0	EQU	$F1
__local_set_page_first__0	EQU	$F1
__zpabi_print_ascii_string__s_1	EQU	$F1
__local_set_nonstack_var__1	EQU	$F2
__local_set_page_first__prev_first	EQU	$F2
__zpabi_cout_string__s_0	EQU	$F2
__zpabi_cout_string__s_1	EQU	$F3
__local_main__0	EQU	$F4
__local_main__sector	EQU	$F5
__local_do_instruction__v_1	EQU	$F6
__local_do_instruction__0	EQU	$F7
__local_do_instruction__v_0	EQU	$F8
__local_do_instruction__forms	EQU	$F9
__local_do_instruction__1	EQU	$FA
__local_branch__offset_1	EQU	$FB
__local_do_var__n	EQU	$FB
__local_instr_get_prop__0	EQU	$FB
__local_instr_get_prop_addr__0	EQU	$FB
__local_instr_get_prop_len__0	EQU	$FB
__local_remove_obj__0	EQU	$FB
__local_branch__offset_0	EQU	$FC
__local_do_var__0	EQU	$FC
__local_instr_get_prop__1	EQU	$FC
__local_instr_get_prop_addr__y	EQU	$FC
__local_instr_get_prop_len__1	EQU	$FC
__local_remove_obj__1	EQU	$FC
__local_branch__condition	EQU	$FD
__local_do_var__1	EQU	$FD
__local_instr_get_prop__y	EQU	$FD
__local_instr_get_prop_addr__1	EQU	$FD
__local_instr_get_prop_len__2	EQU	$FD
__local_remove_obj__2	EQU	$FD
__local_do_var__2	EQU	$FE
__local_instr_get_prop__2	EQU	$FE
__local_remove_obj__3	EQU	$FE
__local_do_var__3	EQU	$FF
__local_instr_get_prop__3	EQU	$FF
__local_remove_obj__4	EQU	$FF
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
; def A_mod_3 params=__zpabi_A_mod_3__a locals= indirect=false in_cycle=false param_regs=A
; def ascii_to_zchar params= locals=__local_ascii_to_zchar__z,__local_ascii_to_zchar__dest,__local_ascii_to_zchar__left,__local_ascii_to_zchar__0,__local_ascii_to_zchar__1,__local_ascii_to_zchar__2,__local_ascii_to_zchar__index indirect=false in_cycle=false
; def attr_ptr_and_mask params= locals=__local_attr_ptr_and_mask__attr,__local_attr_ptr_and_mask__mask_1,__local_attr_ptr_and_mask__n indirect=false in_cycle=false
; def branch params=__zpabi_branch__condition locals=__local_branch__offset_1,__local_branch__offset_0,__local_branch__condition indirect=false in_cycle=false param_regs=A
; def branch_to_offset params=__zpabi_branch_to_offset__offset_0,__zpabi_branch_to_offset__offset_1 locals=__local_branch_to_offset__0,__local_branch_to_offset__1,__local_branch_to_offset__pages_1,__local_branch_to_offset__2 indirect=false in_cycle=false changes=__zpabi_branch_to_offset__offset_1
; def buffer_char params=__zpabi_buffer_char__c locals=__local_buffer_char__0 indirect=false in_cycle=false param_regs=A
; def check_sign params=__zpabi_check_sign__a_0,__zpabi_check_sign__a_1,__zpabi_check_sign__b_0,__zpabi_check_sign__b_1 locals= indirect=false in_cycle=false changes=__zpabi_check_sign__a_0
; def copy_data_from_buff params=__zpabi_copy_data_from_buff__dest_0,__zpabi_copy_data_from_buff__dest_1,__zpabi_copy_data_from_buff__x,__zpabi_copy_data_from_buff__count locals= indirect=false in_cycle=false param_regs=,,X,Y
; def copy_data_to_buff params=__zpabi_copy_data_to_buff__src_0,__zpabi_copy_data_to_buff__src_1,__zpabi_copy_data_to_buff__x,__zpabi_copy_data_to_buff__count locals= indirect=false in_cycle=false param_regs=,,X,Y
; def cout_string params=__zpabi_cout_string__s_0,__zpabi_cout_string__s_1,__zpabi_cout_string__count locals= indirect=false in_cycle=false param_regs=,,X
; def dec_var params= locals=__local_dec_var__0,__local_dec_var__1,__local_dec_var__2 indirect=false in_cycle=false
; def divu16 params=__zpabi_divu16__dividend_0,__zpabi_divu16__dividend_1,__zpabi_divu16__divisor_0,__zpabi_divu16__divisor_1 locals=__local_divu16__partial_1,__local_divu16__partial_0,__local_divu16__0 indirect=false in_cycle=false changes=__zpabi_divu16__dividend_0
; def do_0op params=__zpabi_do_0op__n locals= indirect=false in_cycle=false param_regs=A
; def do_1op params=__zpabi_do_1op__n locals= indirect=false in_cycle=false param_regs=A
; def do_2op params=__zpabi_do_2op__n locals=__local_do_2op__equal,__local_do_2op__0,__local_do_2op__left,__local_do_2op__2,__local_do_2op__3,__local_do_2op__4,__local_do_2op__5 indirect=false in_cycle=false param_regs=A
; def do_instruction params= locals=__local_do_instruction__v_1,__local_do_instruction__0,__local_do_instruction__v_0,__local_do_instruction__forms,__local_do_instruction__1 indirect=false in_cycle=false
; def do_rwts_on_sector params=__zpabi_do_rwts_on_sector__command locals= indirect=false in_cycle=false param_regs=A carry=true
; def do_var params=__zpabi_do_var__n locals=__local_do_var__n,__local_do_var__0,__local_do_var__1,__local_do_var__2,__local_do_var__3 indirect=false in_cycle=false param_regs=A
; def dump_buffer_line params= locals= indirect=false in_cycle=false
; def dump_buffer_to_printer params= locals=__local_dump_buffer_to_printer__x,__local_dump_buffer_to_printer__1,__local_dump_buffer_to_printer__2 indirect=false in_cycle=false
; def dump_buffer_to_screen params= locals= indirect=false in_cycle=false
; def dump_buffer_with_more params= locals=__local_dump_buffer_with_more__0,__local_dump_buffer_with_more__end indirect=false in_cycle=false
; def find_index_of_page_table params=__zpabi_find_index_of_page_table__page_0,__zpabi_find_index_of_page_table__page_1 locals= indirect=false in_cycle=false carry=true
; def get_alphabet params= locals= indirect=false in_cycle=false
; def get_alphabet_for_char params=__zpabi_get_alphabet_for_char__c locals= indirect=false in_cycle=false
; def get_const_word params= locals=__local_get_const_word__1 indirect=false in_cycle=false
; def get_dictionary_addr params= locals=__local_get_dictionary_addr__0,__local_get_dictionary_addr__1 indirect=false in_cycle=false
; def get_next_code_byte params= locals=__local_get_next_code_byte__page indirect=false in_cycle=false
; def get_next_code_byte2 params= locals=__local_get_next_code_byte2__page indirect=false in_cycle=false
; def get_next_code_word params= locals=__local_get_next_code_word__1 indirect=false in_cycle=false
; def get_next_zchar params= locals=__local_get_next_zchar__compound_literal_1 indirect=false in_cycle=false carry=true
; def get_nonstack_var params=__zpabi_get_nonstack_var__var locals=__local_get_nonstack_var__var,__local_get_nonstack_var__0,__local_get_nonstack_var__1,__local_get_nonstack_var__2 indirect=false in_cycle=false param_regs=A
; def get_object_addr params=__zpabi_get_object_addr__obj locals=__local_get_object_addr__0,__local_get_object_addr__1,__local_get_object_addr__2,__local_get_object_addr__3,__local_get_object_addr__obj indirect=false in_cycle=false param_regs=A
; def get_prompted_number_from_user params=__zpabi_get_prompted_number_from_user__which locals=__local_get_prompted_number_from_user__key,__local_get_prompted_number_from_user__1 indirect=false in_cycle=false param_regs=A
; def get_property_len params=__zpabi_get_property_len__y locals=__local_get_property_len__0 indirect=false in_cycle=false param_regs=A
; def get_property_num params=__zpabi_get_property_num__y locals= indirect=false in_cycle=false param_regs=A
; def get_property_ptr params= locals=__local_get_property_ptr__0,__local_get_property_ptr__1,__local_get_property_ptr__2 indirect=false in_cycle=false
; def get_var_content params= locals= indirect=false in_cycle=false
; def home params= locals= indirect=false in_cycle=false
; def inc_sector_and_read params= locals= indirect=false in_cycle=false carry=true
; def inc_sector_and_write params= locals= indirect=false in_cycle=false carry=true
; def inc_var params= locals=__local_inc_var__0,__local_inc_var__1,__local_inc_var__2 indirect=false in_cycle=false
; def instr_call params= locals=__local_instr_call__0,__local_instr_call__1,__local_instr_call__2,__local_instr_call__i,__local_instr_call__args,__local_instr_call__3 indirect=false in_cycle=false
; def instr_clear_attr params= locals=__local_instr_clear_attr__0,__local_instr_clear_attr__1,__local_instr_clear_attr__3 indirect=false in_cycle=false
; def instr_div params= locals= indirect=false in_cycle=false
; def instr_get_child params= locals=__local_instr_get_child__0,__local_instr_get_child__1 indirect=false in_cycle=false
; def instr_get_next_prop params= locals=__local_instr_get_next_prop__y,__local_instr_get_next_prop__1 indirect=false in_cycle=false
; def instr_get_parent params= locals=__local_instr_get_parent__0,__local_instr_get_parent__1 indirect=false in_cycle=false
; def instr_get_prop params= locals=__local_instr_get_prop__0,__local_instr_get_prop__1,__local_instr_get_prop__y,__local_instr_get_prop__2,__local_instr_get_prop__3 indirect=false in_cycle=false
; def instr_get_prop_addr params= locals=__local_instr_get_prop_addr__0,__local_instr_get_prop_addr__y,__local_instr_get_prop_addr__1 indirect=false in_cycle=false
; def instr_get_prop_len params= locals=__local_instr_get_prop_len__0,__local_instr_get_prop_len__1,__local_instr_get_prop_len__2 indirect=false in_cycle=false
; def instr_get_sibling params= locals=__local_instr_get_sibling__0,__local_instr_get_sibling__1 indirect=false in_cycle=false
; def instr_insert_obj params= locals=__local_instr_insert_obj__0,__local_instr_insert_obj__1,__local_instr_insert_obj__2,__local_instr_insert_obj__3,__local_instr_insert_obj__4,__local_instr_insert_obj__5 indirect=false in_cycle=false
; def instr_jin params= locals=__local_instr_jin__2,__local_instr_jin__3 indirect=false in_cycle=false
; def instr_mod params= locals= indirect=false in_cycle=false
; def instr_mul params= locals= indirect=false in_cycle=false
; def instr_put_prop params= locals=__local_instr_put_prop__0,__local_instr_put_prop__y,__local_instr_put_prop__1,__local_instr_put_prop__2 indirect=false in_cycle=false
; def instr_quit params= locals= indirect=false in_cycle=false
; def instr_random params= locals= indirect=false in_cycle=false
; def instr_remove_obj params= locals= indirect=false in_cycle=false
; def instr_restart params= locals= indirect=false in_cycle=false
; def instr_restore params= locals=__local_instr_restore__pages,__local_instr_restore__1 indirect=false in_cycle=false
; def instr_save params= locals=__local_instr_save__pages indirect=false in_cycle=false
; def instr_set_attr params= locals= indirect=false in_cycle=false
; def instr_sread params= locals=__local_instr_sread__1 indirect=false in_cycle=false
; def instr_test_attr params= locals=__local_instr_test_attr__1 indirect=false in_cycle=false
; def internal_error params= locals= indirect=false in_cycle=false
; def is_dict_separator params=__zpabi_is_dict_separator__c locals=__local_is_dict_separator__1,__local_is_dict_separator__2 indirect=false in_cycle=false carry=true
; def is_separator params=__zpabi_is_separator__c locals= indirect=false in_cycle=false carry=true
; def is_std_separator params=__zpabi_is_std_separator__c locals= indirect=false in_cycle=false carry=true
; def load_address params=__zpabi_load_address__addr_0,__zpabi_load_address__addr_1 locals= indirect=false in_cycle=false
; def load_packed_address params=__zpabi_load_packed_address__addr_0,__zpabi_load_packed_address__addr_1 locals= indirect=false in_cycle=false
; def locate_last_ram_page params= locals=__local_locate_last_ram_page__2,__local_locate_last_ram_page__3,__local_locate_last_ram_page__page indirect=false in_cycle=false
; def main params= locals=__local_main__0,__local_main__sector indirect=false in_cycle=false
; def match_dictionary_word params= locals=__local_match_dictionary_word__0,__local_match_dictionary_word__count_0,__local_match_dictionary_word__5,__local_match_dictionary_word__6,__local_match_dictionary_word__count_1,__local_match_dictionary_word__8,__local_match_dictionary_word__entry_0,__local_match_dictionary_word__entry_1 indirect=false in_cycle=false
; def mulu16 params=__zpabi_mulu16__a_0,__zpabi_mulu16__a_1,__zpabi_mulu16__b_0,__zpabi_mulu16__b_1 locals=__local_mulu16__product indirect=false in_cycle=false changes=__zpabi_mulu16__a_0,__zpabi_mulu16__a_1,__zpabi_mulu16__b_0,__zpabi_mulu16__b_1
; def new_line params= locals= indirect=false in_cycle=false
; def next_property params=__zpabi_next_property__y locals=__local_next_property__y indirect=false in_cycle=false param_regs=A
; def please_insert_save_diskette params= locals=__local_please_insert_save_diskette__0,__local_please_insert_save_diskette__1 indirect=false in_cycle=false
; def please_reinsert_game_diskette params= locals= indirect=false in_cycle=false
; def pop params= locals=__local_pop__1,__local_pop__2 indirect=false in_cycle=false
; def print_ascii_string params=__zpabi_print_ascii_string__s_0,__zpabi_print_ascii_string__s_1,__zpabi_print_ascii_string__count locals=__local_print_ascii_string__count,__local_print_ascii_string__i indirect=false in_cycle=false param_regs=,,X
; def print_number params=__zpabi_print_number__n_0,__zpabi_print_number__n_1 locals=__local_print_number__count,__local_print_number__digits indirect=false in_cycle=false changes=__zpabi_print_number__n_0,__zpabi_print_number__n_1
; def print_obj params=__zpabi_print_obj__obj locals=__local_print_obj__obj,__local_print_obj__0,__local_print_obj__1 indirect=false in_cycle=false param_regs=A
; def print_status_line params= locals=__local_print_status_line__1,__local_print_status_line__2 indirect=false in_cycle=false
; def print_string_literal params= locals= indirect=false in_cycle=false
; def print_zstring params= locals=__local_print_zstring__ch,__local_print_zstring__c_0,__local_print_zstring__0 indirect=false in_cycle=true
; def push params=__zpabi_push__value_0,__zpabi_push__value_1 locals= indirect=false in_cycle=false
; def read_from_sector params= locals= indirect=false in_cycle=false carry=true
; def read_line params= locals=__local_read_line__c,__local_read_line__left,__local_read_line__n indirect=false in_cycle=false
; def read_next_sector params= locals= indirect=false in_cycle=false carry=true
; def remove_obj params= locals=__local_remove_obj__0,__local_remove_obj__1,__local_remove_obj__2,__local_remove_obj__3,__local_remove_obj__4 indirect=false in_cycle=false
; def reset_window params= locals= indirect=false in_cycle=false
; def return_value params=__zpabi_return_value__value_0,__zpabi_return_value__value_1 locals=__local_return_value__0,__local_return_value__1,__local_return_value__x,__local_return_value__count indirect=false in_cycle=false
; def search_nonalpha_table params=__zpabi_search_nonalpha_table__c locals=__local_search_nonalpha_table__0 indirect=false in_cycle=false
; def set_attr_word params=__zpabi_set_attr_word__word_0,__zpabi_set_attr_word__word_1 locals= indirect=false in_cycle=false
; def set_nonstack_var params=__zpabi_set_nonstack_var__var,__zpabi_set_nonstack_var__value_0,__zpabi_set_nonstack_var__value_1 locals=__local_set_nonstack_var__var,__local_set_nonstack_var__0,__local_set_nonstack_var__1 indirect=false in_cycle=false param_regs=A,,
; def set_page_first params=__zpabi_set_page_first__slot locals=__local_set_page_first__0,__local_set_page_first__prev_first indirect=false in_cycle=false param_regs=A
; def skip_separators params= locals= indirect=false in_cycle=false
; def store_and_branch_on params=__zpabi_store_and_branch_on__related locals=__local_store_and_branch_on__related indirect=false in_cycle=false param_regs=A
; def store_result params=__zpabi_store_result__value_0,__zpabi_store_result__value_1 locals=__local_store_result__0 indirect=false in_cycle=false
; def store_with_sign params=__zpabi_store_with_sign__n_0,__zpabi_store_with_sign__n_1 locals= indirect=false in_cycle=false changes=__zpabi_store_with_sign__n_0
; def var_get params=__zpabi_var_get__var locals=__local_var_get__var,__local_var_get__0 indirect=false in_cycle=false param_regs=A
; def var_put params=__zpabi_var_put__var,__zpabi_var_put__value_0,__zpabi_var_put__value_1 locals=__local_var_put__var indirect=false in_cycle=false param_regs=A,,
; def write_next_sector params= locals= indirect=false in_cycle=false carry=true
; def write_to_sector params= locals=__local_write_to_sector__1,__local_write_to_sector__2 indirect=false in_cycle=false carry=true
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
; ext get_object_addr params=__zpabi_get_object_addr__obj param_regs=A
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
; ext instr_sread params=
; ext instr_test_attr params=
; ext internal_error params=
; ext load_address params=__zpabi_load_address__addr_0,__zpabi_load_address__addr_1
; ext load_packed_address params=__zpabi_load_packed_address__addr_0,__zpabi_load_packed_address__addr_1
; ext new_line params=
; ext please_insert_save_diskette params=
; ext please_reinsert_game_diskette params=
; ext pop params=
; ext print_ascii_string params=__zpabi_print_ascii_string__s_0,__zpabi_print_ascii_string__s_1,__zpabi_print_ascii_string__count param_regs=,,X
; ext print_number params=__zpabi_print_number__n_0,__zpabi_print_number__n_1
; ext print_obj params=__zpabi_print_obj__obj param_regs=A
; ext print_status_line params=
; ext print_string_literal params=
; ext print_zstring params=
; ext push params=__zpabi_push__value_0,__zpabi_push__value_1
; ext read_from_sector params= carry=true
; ext read_next_sector params= carry=true
; ext reset_window params=
; ext restart params=
; ext return_value params=__zpabi_return_value__value_0,__zpabi_return_value__value_1
; ext store_result params=__zpabi_store_result__value_0,__zpabi_store_result__value_1
; ext var_get params=__zpabi_var_get__var param_regs=A
; ext var_put params=__zpabi_var_put__var,__zpabi_var_put__value_0,__zpabi_var_put__value_1 param_regs=A,,
; ext write_next_sector params= carry=true
; ext write_to_sector params= carry=true
; call ascii_to_zchar -> A_mod_3
; call ascii_to_zchar -> get_alphabet_for_char
; call ascii_to_zchar -> search_nonalpha_table
; call ascii_to_zchar -> z_compress
; call attr_ptr_and_mask -> get_object_addr
; call branch -> branch_to_offset
; call branch -> get_next_code_byte
; call branch -> return_value
; call buffer_char -> dump_buffer_with_more
; call cout_string -> COUT1
; call dec_var -> var_get
; call dec_var -> var_put
; call do_0op -> instr_quit
; call do_0op -> instr_restart
; call do_0op -> instr_restore
; call do_0op -> instr_save
; call do_0op -> internal_error
; call do_0op -> new_line
; call do_0op -> pop
; call do_0op -> print_string_literal
; call do_0op -> return_value
; call do_1op -> branch
; call do_1op -> branch_to_offset
; call do_1op -> dec_var
; call do_1op -> inc_var
; call do_1op -> instr_get_child
; call do_1op -> instr_get_parent
; call do_1op -> instr_get_prop_len
; call do_1op -> instr_get_sibling
; call do_1op -> instr_remove_obj
; call do_1op -> internal_error
; call do_1op -> load_address
; call do_1op -> load_packed_address
; call do_1op -> print_obj
; call do_1op -> print_zstring
; call do_1op -> return_value
; call do_1op -> store_result
; call do_1op -> var_get
; call do_2op -> branch
; call do_2op -> dec_var
; call do_2op -> get_next_code_byte2
; call do_2op -> get_next_code_word
; call do_2op -> inc_var
; call do_2op -> instr_clear_attr
; call do_2op -> instr_div
; call do_2op -> instr_get_next_prop
; call do_2op -> instr_get_prop
; call do_2op -> instr_get_prop_addr
; call do_2op -> instr_insert_obj
; call do_2op -> instr_jin
; call do_2op -> instr_mod
; call do_2op -> instr_mul
; call do_2op -> instr_set_attr
; call do_2op -> instr_test_attr
; call do_2op -> internal_error
; call do_2op -> load_address
; call do_2op -> store_result
; call do_2op -> var_put
; call do_instruction -> do_0op
; call do_instruction -> do_1op
; call do_instruction -> do_2op
; call do_instruction -> do_var
; call do_instruction -> get_const_word
; call do_instruction -> get_next_code_byte
; call do_instruction -> get_var_content
; call do_rwts_on_sector -> RWTS
; call do_var -> buffer_char
; call do_var -> instr_call
; call do_var -> instr_put_prop
; call do_var -> instr_random
; call do_var -> instr_sread
; call do_var -> internal_error
; call do_var -> pop
; call do_var -> print_number
; call do_var -> push
; call do_var -> var_put
; call dump_buffer_line -> dump_buffer_to_printer
; call dump_buffer_line -> dump_buffer_to_screen
; call dump_buffer_to_printer -> COUT
; call dump_buffer_to_screen -> COUT1
; call dump_buffer_with_more -> CLREOL
; call dump_buffer_with_more -> COUT
; call dump_buffer_with_more -> COUT1
; call dump_buffer_with_more -> RDKEY
; call dump_buffer_with_more -> cout_string
; call dump_buffer_with_more -> dump_buffer_line
; call get_const_word -> get_next_code_byte
; call get_next_code_byte -> find_index_of_page_table
; call get_next_code_byte -> read_from_sector
; call get_next_code_byte -> restart
; call get_next_code_byte -> set_page_first
; call get_next_code_byte2 -> find_index_of_page_table
; call get_next_code_byte2 -> read_from_sector
; call get_next_code_byte2 -> restart
; call get_next_code_byte2 -> set_page_first
; call get_next_code_word -> get_next_code_byte2
; call get_next_zchar -> get_next_code_word
; call get_prompted_number_from_user -> CLREOL
; call get_prompted_number_from_user -> RDKEY
; call get_prompted_number_from_user -> cout_string
; call get_prompted_number_from_user -> dump_buffer_line
; call get_prompted_number_from_user -> dump_buffer_with_more
; call get_prompted_number_from_user -> print_ascii_string
; call get_property_ptr -> get_object_addr
; call get_var_content -> get_next_code_byte
; call get_var_content -> get_nonstack_var
; call get_var_content -> pop
; call home -> HOME
; call inc_sector_and_read -> read_from_sector
; call inc_sector_and_write -> write_to_sector
; call inc_var -> var_get
; call inc_var -> var_put
; call instr_call -> get_next_code_byte
; call instr_call -> push
; call instr_call -> store_result
; call instr_clear_attr -> attr_ptr_and_mask
; call instr_clear_attr -> set_attr_word
; call instr_div -> check_sign
; call instr_div -> divu16
; call instr_div -> store_with_sign
; call instr_get_child -> get_object_addr
; call instr_get_child -> store_and_branch_on
; call instr_get_next_prop -> get_property_num
; call instr_get_next_prop -> get_property_ptr
; call instr_get_next_prop -> next_property
; call instr_get_next_prop -> store_result
; call instr_get_parent -> get_object_addr
; call instr_get_parent -> store_result
; call instr_get_prop -> get_property_len
; call instr_get_prop -> get_property_num
; call instr_get_prop -> get_property_ptr
; call instr_get_prop -> internal_error
; call instr_get_prop -> next_property
; call instr_get_prop -> store_result
; call instr_get_prop_addr -> get_property_num
; call instr_get_prop_addr -> get_property_ptr
; call instr_get_prop_addr -> next_property
; call instr_get_prop_addr -> store_result
; call instr_get_prop_len -> store_result
; call instr_get_sibling -> get_object_addr
; call instr_get_sibling -> store_and_branch_on
; call instr_insert_obj -> get_object_addr
; call instr_insert_obj -> remove_obj
; call instr_jin -> branch
; call instr_jin -> get_object_addr
; call instr_mod -> check_sign
; call instr_mod -> divu16
; call instr_mod -> store_result
; call instr_mul -> check_sign
; call instr_mul -> mulu16
; call instr_mul -> store_with_sign
; call instr_put_prop -> get_property_len
; call instr_put_prop -> get_property_num
; call instr_put_prop -> get_property_ptr
; call instr_put_prop -> internal_error
; call instr_put_prop -> next_property
; call instr_quit -> dump_buffer_with_more
; call instr_quit -> print_ascii_string
; call instr_random -> divu16
; call instr_random -> store_result
; call instr_remove_obj -> remove_obj
; call instr_restart -> dump_buffer_with_more
; call instr_restart -> restart
; call instr_restore -> branch
; call instr_restore -> copy_data_from_buff
; call instr_restore -> inc_sector_and_read
; call instr_restore -> please_insert_save_diskette
; call instr_restore -> please_reinsert_game_diskette
; call instr_restore -> read_next_sector
; call instr_save -> branch
; call instr_save -> copy_data_to_buff
; call instr_save -> inc_sector_and_write
; call instr_save -> please_insert_save_diskette
; call instr_save -> please_reinsert_game_diskette
; call instr_save -> write_next_sector
; call instr_set_attr -> attr_ptr_and_mask
; call instr_set_attr -> set_attr_word
; call instr_sread -> ascii_to_zchar
; call instr_sread -> is_dict_separator
; call instr_sread -> is_separator
; call instr_sread -> is_std_separator
; call instr_sread -> match_dictionary_word
; call instr_sread -> print_status_line
; call instr_sread -> read_line
; call instr_sread -> skip_separators
; call instr_test_attr -> attr_ptr_and_mask
; call instr_test_attr -> branch
; call internal_error -> dump_buffer_with_more
; call internal_error -> print_ascii_string
; call is_dict_separator -> get_dictionary_addr
; call is_separator -> is_dict_separator
; call is_separator -> is_std_separator
; call main -> do_instruction
; call main -> internal_error
; call main -> locate_last_ram_page
; call main -> read_from_sector
; call main -> reset_window
; call main -> restart
; call match_dictionary_word -> get_dictionary_addr
; call new_line -> buffer_char
; call next_property -> get_property_len
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
; call pop -> internal_error
; call print_ascii_string -> buffer_char
; call print_number -> buffer_char
; call print_number -> divu16
; call print_obj -> get_object_addr
; call print_obj -> load_address
; call print_obj -> print_zstring
; call print_status_line -> CLREOL
; call print_status_line -> VTAB
; call print_status_line -> buffer_char
; call print_status_line -> cout_string
; call print_status_line -> dump_buffer_line
; call print_status_line -> dump_buffer_to_screen
; call print_status_line -> print_number
; call print_status_line -> print_obj
; call print_status_line -> var_get
; call print_string_literal -> print_zstring
; call print_zstring -> A_mod_3
; call print_zstring -> buffer_char
; call print_zstring -> get_alphabet
; call print_zstring -> get_next_zchar
; call print_zstring -> load_packed_address
; call print_zstring -> print_zstring saved
; call push -> internal_error
; call read_from_sector -> do_rwts_on_sector
; call read_line -> GETLN1
; call read_line -> dump_buffer_line
; call read_line -> dump_buffer_to_printer
; call read_next_sector -> inc_sector_and_read
; call remove_obj -> get_object_addr
; call reset_window -> home
; call return_value -> pop
; call return_value -> store_result
; call skip_separators -> is_separator
; call store_and_branch_on -> branch
; call store_and_branch_on -> store_result
; call store_result -> get_next_code_byte
; call store_result -> push
; call store_result -> set_nonstack_var
; call store_with_sign -> store_result
; call var_get -> get_nonstack_var
; call var_get -> pop
; call var_get -> push
; call var_put -> pop
; call var_put -> push
; call var_put -> set_nonstack_var
; call write_next_sector -> inc_sector_and_write
; call write_to_sector -> do_rwts_on_sector
; share member=check_sign:__zpabi_check_sign__b_0,__zpabi_check_sign__b_1 holder=instr_div holder=instr_mod holder=instr_mul live=instr_div:check_sign live=instr_mod:check_sign live=instr_mul:check_sign forward=instr_div>check_sign forward=instr_mod>check_sign forward=instr_mul>check_sign writer=instr_div writer=instr_mod writer=instr_mul
; share member=is_dict_separator:__zpabi_is_dict_separator__c member=is_separator:__zpabi_is_separator__c member=is_std_separator:__zpabi_is_std_separator__c holder=instr_sread live=instr_sread:is_dict_separator live=is_separator:is_dict_separator forward=instr_sread>is_dict_separator forward=is_separator>is_dict_separator forward=is_separator>is_std_separator writer=instr_sread
; share member=find_index_of_page_table:__zpabi_find_index_of_page_table__page_0,__zpabi_find_index_of_page_table__page_1 holder=get_next_code_byte holder=get_next_code_byte2 live=get_next_code_byte:find_index_of_page_table live=get_next_code_byte2:find_index_of_page_table forward=get_next_code_byte>find_index_of_page_table forward=get_next_code_byte2>find_index_of_page_table writer=get_next_code_byte writer=get_next_code_byte2
; zp disk_buffer size=2 at=$07 def=true init=$00,$00
; zp disk_sector size=2 at=$09 def=true init=$00,$00
; zp input_ptr size=1 at=$0B def=true init=$00
; zp char_count size=1 at=$0C def=true init=$00
; zp input_count size=1 at=$0D def=true init=$00
; zp token_idx size=1 at=$0E def=true init=$00
; zp parse size=2 at=$0F def=true init=$00,$00
; zp text size=2 at=$11 def=true init=$00,$00
; zp props size=2 at=$13 def=true init=$00,$00
; zp remainder size=2 at=$15 def=true init=$00,$00
; zp attr_ptr size=2 at=$17 def=true init=$00,$00
; zp sign_bit size=1 at=$19 def=true init=$00
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
; runtime hargs_size=1 hargs_need=0
; @zp-link-meta-end


   SEG   main
   ORG   $0800
__main_start:

main:
   SUBROUTINE

   LDA   #$00
   STA   PRINTER_CSW
   LDA   #$C1
   STA   PRINTER_CSW+1
   LDA   #$00
   STA   ZCODE_PAGE_VALID
   STA   ZCODE_PAGE_VALID2
   STA   PAGE_TABLE_INDEX
   STA   PAGE_TABLE_INDEX2
   STA   BUFF_END
   LDX   #$00
.main@rot@0:
   STA   LOCAL_ZVARS,X
   INX
   CPX   #$1E
   BNE   .main@rot@0
   STA   FRAME_Z_SP
   STA   FRAME_Z_SP+1
   STA   FRAME_STACK_COUNT
   LDA   #$01
   STA   STACK_COUNT
   LDA   #$E8
   STA   Z_SP
   LDA   #$03
   STA   Z_SP+1
   LDA   #$FF
   STA   ZCHAR_SCRATCH1+6
   LDA   #<page_tables
   STA   PAGE_L_TABLE
   LDA   #>page_tables
   STA   PAGE_L_TABLE+1
   LDA   #<(page_tables+128)
   STA   PAGE_H_TABLE
   LDA   #>(page_tables+128)
   STA   PAGE_H_TABLE+1
   LDA   #<page_tables
   STA   NEXT_PAGE_TABLE
   LDA   #>page_tables
   CLC
   ADC   #$01
   STA   NEXT_PAGE_TABLE+1
   LDA   #<(page_tables+384)
   STA   PREV_PAGE_TABLE
   LDA   #>(page_tables+384)
   STA   PREV_PAGE_TABLE+1
   LDY   #$00
.loop@2_start:
   LDA   #$00
   STA   (PAGE_L_TABLE),Y
   STA   (PAGE_H_TABLE),Y
   TYA
   CLC
   ADC   #$01
   STA   __local_main__sector
   STA   (NEXT_PAGE_TABLE),Y
   TYA
   SEC
   SBC   #$01
   STA   (PREV_PAGE_TABLE),Y
   LDA   __local_main__sector
   CMP   #$80
   BEQ   .lb_skip@0
   JMP   .main@asm_ssa_split@0
.lb_skip@0:
   LDA   #$FF
   LDY   #$7F
   STA   (NEXT_PAGE_TABLE),Y
   LDA   #$00
   STA   FIRST_Z_PAGE
   LDA   #$7F
   STA   LAST_Z_PAGE
   LDA   #<__story
   STA   Z_HEADER_ADDR
   LDA   #>__story
   STA   Z_HEADER_ADDR+1
   JSR   reset_window
   LDA   #$00
   STA   disk_sector
   STA   disk_sector+1
   LDA   Z_HEADER_ADDR
   STA   disk_buffer
   LDA   Z_HEADER_ADDR+1
   STA   disk_buffer+1
   JSR   read_from_sector
   BCC   .if_end@2
   JSR   restart
.if_end@2:
   LDA   #$FF
   LDY   #$05
   STA   (Z_HEADER_ADDR),Y
   DEY
   LDA   (Z_HEADER_ADDR),Y
   CLC
   ADC   #$01
   STA   NUM_IMAGE_PAGES
   LDA   #$01
   STA   __local_main__sector
   JMP   .loop@3_start
.main@rot@1:
   STA   disk_sector
   LDA   #$00
   STA   disk_sector+1
   LDA   Z_HEADER_ADDR
   CLC
   ADC   #$00
   STA   disk_buffer
   LDA   Z_HEADER_ADDR+1
   ADC   __local_main__sector
   STA   disk_buffer+1
   JSR   read_from_sector
   BCC   .if_end@3
   JSR   restart
.if_end@3:
   INC   __local_main__sector
.loop@3_start:
   LDA   __local_main__sector
   CMP   NUM_IMAGE_PAGES
   BNE   .main@rot@1
   LDY   #$01
   LDA   (Z_HEADER_ADDR),Y
   LSR
   BCC   .if_end@4
   JSR   internal_error
.if_end@4:
   LDY   #$06
   LDA   (Z_HEADER_ADDR),Y
   STA   __local_main__0
   INY
   LDA   (Z_HEADER_ADDR),Y
   STA   Z_PC
   LDA   __local_main__0
   STA   Z_PC+1
   LDA   #$00
   STA   Z_PC+2
   LDY   #$0C
   LDA   (Z_HEADER_ADDR),Y
   STA   __local_main__0
   INY
   LDA   (Z_HEADER_ADDR),Y
   CLC
   ADC   Z_HEADER_ADDR
   STA   GLOBAL_ZVARS_ADDR
   LDA   Z_HEADER_ADDR+1
   ADC   __local_main__0
   STA   GLOBAL_ZVARS_ADDR+1
   LDY   #$18
   LDA   (Z_HEADER_ADDR),Y
   STA   __local_main__0
   INY
   LDA   (Z_HEADER_ADDR),Y
   CLC
   ADC   Z_HEADER_ADDR
   STA   Z_ABBREV_TABLE
   LDA   Z_HEADER_ADDR+1
   ADC   __local_main__0
   STA   Z_ABBREV_TABLE+1
   LDA   Z_HEADER_ADDR
   CLC
   ADC   #$00
   STA   HIGH_MEM_ADDR
   LDA   Z_HEADER_ADDR+1
   ADC   NUM_IMAGE_PAGES
   STA   HIGH_MEM_ADDR+1
   STA   __local_main__sector
   JSR   locate_last_ram_page
   STA   __local_main__0
   CMP   __local_main__sector
   BCS   .if_end@5
   JSR   internal_error
.if_end@5:
   LDA   __local_main__0
   SEC
   SBC   __local_main__sector
   TAY
   STY   LAST_Z_PAGE
   TYA
   CLC
   ADC   #$01
   STA   NUM_PAGE_TABLE_ENTRIES
   LDA   #$FF
   STA   (NEXT_PAGE_TABLE),Y
.loop@4_start:
   JSR   do_instruction
   JMP   .loop@4_start
.main@asm_ssa_split@0:
   LDY   __local_main__sector
   JMP   .loop@2_start
__main_end:
__main_load	EQU	__main_start
   IF    __main_end > $2200
   ECHO  "section .main runs past the end of region low by", [__main_end - $2200]d, "bytes"
   ERR
   ENDIF

   SEG   text
   ORG   __main_end
__text_start:

push:
   SUBROUTINE

   LDA   Z_SP
   SEC
   SBC   #$01
   STA   Z_SP
   BCS   .carry_inc@0
   DEC   Z_SP+1
.carry_inc@0:
   LDA   __zpabi_push__value_0
   LDY   #$00
   STA   (Z_SP),Y
   LDA   Z_SP
   SEC
   SBC   #$01
   STA   Z_SP
   BCS   .carry_inc@1
   DEC   Z_SP+1
.carry_inc@1:
   LDA   __zpabi_push__value_1
   LDY   #$00
   STA   (Z_SP),Y
   INC   STACK_COUNT
   LDA   STACK_COUNT
   CMP   #$B4
   BCC   .if_end@0
   JSR   internal_error
.if_end@0:
   RTS

pop:
   SUBROUTINE

   LDY   #$00
   LDA   (Z_SP),Y
   STA   __local_pop__2
   INY
   LDA   (Z_SP),Y
   STA   __local_pop__1
   LDA   Z_SP
   CLC
   ADC   #$02
   STA   Z_SP
   BCC   .carry_inc@2
   INC   Z_SP+1
.carry_inc@2:
   DEC   STACK_COUNT
   BNE   .if_end@1
   JSR   internal_error
.if_end@1:
   LDA   __local_pop__1
   LDX   __local_pop__2
   RTS

var_get:
   SUBROUTINE

   ORA   #$00
   BNE   .if_end@4
   JSR   pop
   STA   __local_var_get__0
   STX   __local_var_get__var
   STA   __zpabi_push__value_0
   STX   __zpabi_push__value_1
   JSR   push
   LDX   __local_var_get__var
   LDA   __local_var_get__0
   RTS
.if_end@4:

get_nonstack_var:
   SUBROUTINE

   STA   __local_get_nonstack_var__var
   CMP   #$10
   BCS   .if_end@2
   DEC   __local_get_nonstack_var__var
   LDA   __local_get_nonstack_var__var
   ASL
   TAX
   LDA   LOCAL_ZVARS,X
   STA   __local_get_nonstack_var__0
   INX
   LDA   LOCAL_ZVARS,X
   LDX   __local_get_nonstack_var__0
   RTS
.if_end@2:
   SBC   #$10
   ASL
   STA   __local_get_nonstack_var__0
   LDA   #$00
   ROL
   STA   __local_get_nonstack_var__var
   LDA   GLOBAL_ZVARS_ADDR
   CLC
   ADC   __local_get_nonstack_var__0
   STA   __local_get_nonstack_var__1
   LDA   GLOBAL_ZVARS_ADDR+1
   ADC   __local_get_nonstack_var__var
   STA   __local_get_nonstack_var__2
   LDY   #$00
   LDA   (__local_get_nonstack_var__1),Y
   STA   __local_get_nonstack_var__0
   INY
   LDA   (__local_get_nonstack_var__1),Y
   LDX   __local_get_nonstack_var__0
   RTS

var_put:
   SUBROUTINE

   STA   __local_var_put__var
   ORA   #$00
   BNE   .if_end@5
   JSR   pop
   LDA   __zpabi_var_put__value_0
   STA   __zpabi_push__value_0
   LDA   __zpabi_var_put__value_1
   STA   __zpabi_push__value_1
   JMP   push
.if_end@5:
   LDA   __zpabi_var_put__value_0
   STA   __zpabi_set_nonstack_var__value_0
   LDA   __zpabi_var_put__value_1
   STA   __zpabi_set_nonstack_var__value_1
   LDA   __local_var_put__var

set_nonstack_var:
   SUBROUTINE

   STA   __local_set_nonstack_var__var
   CMP   #$10
   BCS   .if_end@3
   DEC   __local_set_nonstack_var__var
   LDA   __local_set_nonstack_var__var
   ASL
   TAX
   LDA   __zpabi_set_nonstack_var__value_1
   STA   LOCAL_ZVARS,X
   INX
   LDA   __zpabi_set_nonstack_var__value_0
   STA   LOCAL_ZVARS,X
   RTS
.if_end@3:
   SBC   #$10
   ASL
   STA   __local_set_nonstack_var__var
   LDA   #$00
   ROL
   STA   __local_set_nonstack_var__1
   LDA   GLOBAL_ZVARS_ADDR
   CLC
   ADC   __local_set_nonstack_var__var
   STA   __local_set_nonstack_var__var
   LDA   GLOBAL_ZVARS_ADDR+1
   ADC   __local_set_nonstack_var__1
   STA   __local_set_nonstack_var__0
   STA   DPTR+1
   LDA   __local_set_nonstack_var__var
   STA   DPTR
   LDA   __zpabi_set_nonstack_var__value_1
   LDY   #$00
   STA   (DPTR),Y
   LDA   __local_set_nonstack_var__var
   STA   DPTR
   LDA   __local_set_nonstack_var__0
   STA   DPTR+1
   LDA   __zpabi_set_nonstack_var__value_0
   INY
   STA   (DPTR),Y
   RTS

get_const_word:
   SUBROUTINE

   JSR   get_next_code_byte
   STA   __local_get_const_word__1
   JSR   get_next_code_byte
   LDX   __local_get_const_word__1
   RTS

get_var_content:
   SUBROUTINE

   JSR   get_next_code_byte
   ORA   #$00
   BNE   .if_end@8
   JMP   pop
.if_end@8:
   JMP   get_nonstack_var

return_value:
   SUBROUTINE

   LDA   FRAME_Z_SP
   STA   Z_SP
   LDA   FRAME_Z_SP+1
   STA   Z_SP+1
   LDA   FRAME_STACK_COUNT
   STA   STACK_COUNT
   JSR   pop
   STA   __local_return_value__count
   ORA   #$00
   BEQ   .if_end@9
   ASL
   STA   __local_return_value__x
.loop@0_start:
   JSR   pop
   STA   __local_return_value__1
   STX   __local_return_value__0
   LDA   __local_return_value__x
   SEC
   SBC   #$02
   STA   __local_return_value__x
   LDX   __local_return_value__x
   LDA   __local_return_value__0
   STA   LOCAL_ZVARS,X
   INX
   LDA   __local_return_value__1
   STA   LOCAL_ZVARS,X
   DEC   __local_return_value__count
   BNE   .loop@0_start
.if_end@9:
   JSR   pop
   STA   __local_return_value__count
   STX   __local_return_value__x
   JSR   pop
   STA   FRAME_Z_SP
   STX   FRAME_Z_SP+1
   JSR   pop
   STA   __local_return_value__1
   STX   Z_PC
   LDA   __local_return_value__count
   STA   Z_PC+1
   LDA   __local_return_value__x
   STA   Z_PC+2
   LDA   __local_return_value__1
   STA   FRAME_STACK_COUNT
   LDA   #$00
   STA   ZCODE_PAGE_VALID
   LDA   __zpabi_return_value__value_0
   STA   __zpabi_store_result__value_0
   LDA   __zpabi_return_value__value_1
   STA   __zpabi_store_result__value_1

store_result:
   SUBROUTINE

   JSR   get_next_code_byte
   STA   __local_store_result__0
   ORA   #$00
   BNE   .if_else@7
   LDA   __zpabi_store_result__value_0
   STA   __zpabi_push__value_0
   LDA   __zpabi_store_result__value_1
   STA   __zpabi_push__value_1
   JSR   push
   JMP   .if_end@6
.if_else@7:
   LDA   __zpabi_store_result__value_0
   STA   __zpabi_set_nonstack_var__value_0
   LDA   __zpabi_store_result__value_1
   STA   __zpabi_set_nonstack_var__value_1
   LDA   __local_store_result__0
   JSR   set_nonstack_var
.if_end@6:
   RTS

instr_call:
   SUBROUTINE

   LDA   OPERANDS
   ORA   OPERANDS+1
   BNE   .if_end@20
   LDA   #$00
   STA   __zpabi_store_result__value_0
   STA   __zpabi_store_result__value_1
   JMP   store_result
.if_end@20:
   LDA   FRAME_STACK_COUNT
   STA   __zpabi_push__value_0
   LDA   Z_PC
   STA   __zpabi_push__value_1
   JSR   push
   LDA   FRAME_Z_SP
   STA   __zpabi_push__value_0
   LDA   FRAME_Z_SP+1
   STA   __zpabi_push__value_1
   JSR   push
   LDA   Z_PC+1
   STA   __zpabi_push__value_0
   LDA   Z_PC+2
   STA   __zpabi_push__value_1
   JSR   push
   LDA   #$00
   STA   ZCODE_PAGE_VALID
   LDA   OPERANDS
   ASL
   STA   Z_PC
   LDA   OPERANDS+1
   ROL
   STA   Z_PC+1
   LDA   #$00
   ROL
   STA   Z_PC+2
   JSR   get_next_code_byte
   STA   __local_instr_call__3
   ORA   #$00
   BEQ   .if_end@21
   LDY   #$00
   STY   __local_instr_call__args
   STA   __local_instr_call__i
.loop@1_start:
   LDX   __local_instr_call__args
   LDA   LOCAL_ZVARS,X
   INX
   STX   __local_instr_call__2
   STA   __zpabi_push__value_1
   LDA   LOCAL_ZVARS,X
   STA   __zpabi_push__value_0
   JSR   push
   JSR   get_next_code_byte
   STA   __local_instr_call__1
   JSR   get_next_code_byte
   STA   __local_instr_call__0
   LDX   __local_instr_call__args
   LDA   __local_instr_call__1
   STA   LOCAL_ZVARS,X
   LDX   __local_instr_call__2
   LDA   __local_instr_call__0
   STA   LOCAL_ZVARS,X
   LDA   __local_instr_call__args
   CLC
   ADC   #$02
   STA   __local_instr_call__args
   DEC   __local_instr_call__i
   BNE   .loop@1_start
.if_end@21:
   LDA   OPERAND_COUNT
   SEC
   SBC   #$01
   STA   __local_instr_call__args
   LDY   #$00
   STY   __local_instr_call__i
   JMP   .loop@2_continue
.instr_call@rot@0:
   LDA   __local_instr_call__i
   CLC
   ADC   #$01
   STA   __local_instr_call__2
   ASL
   TAX
   LDA   OPERANDS,X
   STA   __local_instr_call__1
   LDA   OPERANDS+1,X
   STA   __local_instr_call__0
   LDA   __local_instr_call__i
   ASL
   TAX
   LDA   __local_instr_call__0
   STA   LOCAL_ZVARS,X
   INX
   LDA   __local_instr_call__1
   STA   LOCAL_ZVARS,X
   DEC   __local_instr_call__args
   LDA   __local_instr_call__2
   STA   __local_instr_call__i
.loop@2_continue:
   LDA   __local_instr_call__args
   BNE   .instr_call@rot@0
   LDA   __local_instr_call__3
   STA   __zpabi_push__value_0
   LDA   #$00
   STA   __zpabi_push__value_1
   JSR   push
   LDA   STACK_COUNT
   STA   FRAME_STACK_COUNT
   LDA   Z_SP
   STA   FRAME_Z_SP
   LDA   Z_SP+1
   STA   FRAME_Z_SP+1
   RTS

inc_var:
   SUBROUTINE

   LDA   OPERANDS
   STA   __local_inc_var__2
   JSR   var_get
   STA   __local_inc_var__0
   STX   __local_inc_var__1
   INC   __local_inc_var__0
   BNE   .inc_done@1
   INC   __local_inc_var__1
.inc_done@1:
   LDA   __local_inc_var__0
   STA   __zpabi_var_put__value_0
   LDA   __local_inc_var__1
   STA   __zpabi_var_put__value_1
   LDA   __local_inc_var__2
   JSR   var_put
   LDX   __local_inc_var__1
   LDA   __local_inc_var__0
   RTS

dec_var:
   SUBROUTINE

   LDA   OPERANDS
   STA   __local_dec_var__2
   JSR   var_get
   STX   __local_dec_var__1
   SEC
   SBC   #$01
   STA   __local_dec_var__0
   BCS   .carry_inc@3
   DEC   __local_dec_var__1
.carry_inc@3:
   STA   __zpabi_var_put__value_0
   LDA   __local_dec_var__1
   STA   __zpabi_var_put__value_1
   LDA   __local_dec_var__2
   JSR   var_put
   LDX   __local_dec_var__1
   LDA   __local_dec_var__0
   RTS

do_0op:
   SUBROUTINE

   ORA   #$00
   BEQ   .case@4
   CMP   #$01
   BEQ   .case@5
   CMP   #$02
   BEQ   .case@6
   CMP   #$03
   BEQ   .case@7
   CMP   #$04
   BEQ   .switch@3_break
   CMP   #$05
   BEQ   .case@9
   CMP   #$06
   BEQ   .case@10
   CMP   #$07
   BEQ   .case@11
   CMP   #$08
   BEQ   .case@12
   CMP   #$09
   BEQ   .case@13
   CMP   #$0A
   BEQ   .case@14
   CMP   #$0B
   BEQ   .case@15
   JMP   .default@16
.case@4:
   LDA   #$01
   STA   __zpabi_return_value__value_0
   LDA   #$00
   STA   __zpabi_return_value__value_1
   JSR   return_value
   JMP   .switch@3_break
.case@5:
   LDA   #$00
   STA   __zpabi_return_value__value_0
.xjump@0:
   STA   __zpabi_return_value__value_1
   JSR   return_value
   JMP   .switch@3_break
.case@6:
   JSR   print_string_literal
   JMP   .switch@3_break
.case@7:
   JSR   print_string_literal
   JSR   new_line
   JMP   .case@4
.case@9:
   JSR   instr_save
   JMP   .switch@3_break
.case@10:
   JSR   instr_restore
   JMP   .switch@3_break
.case@11:
   JSR   instr_restart
   JMP   .switch@3_break
.case@12:
   JSR   pop
   STA   __zpabi_return_value__value_0
   TXA
   JMP   .xjump@0
.case@13:
   JSR   pop
   JMP   .switch@3_break
.case@14:
   JSR   instr_quit
   JMP   .switch@3_break
.case@15:
   JSR   new_line
   JMP   .switch@3_break
.default@16:
   JSR   internal_error
.switch@3_break:
   RTS

do_1op:
   SUBROUTINE

   ORA   #$00
   BEQ   .case@18
   CMP   #$01
   BEQ   .case@19
   CMP   #$02
   BEQ   .case@20
   CMP   #$03
   BEQ   .case@21
   CMP   #$04
   BEQ   .case@22
   CMP   #$05
   BEQ   .case@23
   CMP   #$06
   BEQ   .case@24
   CMP   #$07
   BEQ   .case@25
   CMP   #$09
   BEQ   .case@26
   CMP   #$0A
   BEQ   .case@27
   CMP   #$0B
   BEQ   .case@28
   CMP   #$0C
   BNE   .lb_skip@4
   JMP   .case@29
.lb_skip@4:
   CMP   #$0D
   BNE   .lb_skip@3
   JMP   .case@30
.lb_skip@3:
   CMP   #$0E
   BNE   .lb_skip@2
   JMP   .case@31
.lb_skip@2:
   CMP   #$0F
   BNE   .lb_skip@1
   JMP   .case@32
.lb_skip@1:
   JMP   .default@33
.case@18:
   LDA   OPERANDS+1
   ORA   OPERANDS
   BEQ   .cmp_true@2
   LDA   #$00
   JMP   .cmp_end@1
.cmp_true@2:
   LDA   #$01
.cmp_end@1:
   JSR   branch
   JMP   .switch@17_break
.case@19:
   JSR   instr_get_sibling
   JMP   .switch@17_break
.case@20:
   JSR   instr_get_child
   JMP   .switch@17_break
.case@21:
   JSR   instr_get_parent
   JMP   .switch@17_break
.case@22:
   JSR   instr_get_prop_len
   JMP   .switch@17_break
.case@23:
   JSR   inc_var
   JMP   .switch@17_break
.case@24:
   JSR   dec_var
   JMP   .switch@17_break
.case@25:
   LDA   OPERANDS
   STA   __zpabi_load_address__addr_0
   LDA   OPERANDS+1
   STA   __zpabi_load_address__addr_1
   JSR   load_address
   JSR   print_zstring
   JMP   .switch@17_break
.case@26:
   JSR   instr_remove_obj
   JMP   .switch@17_break
.case@27:
   LDA   OPERANDS
   JSR   print_obj
   JMP   .switch@17_break
.case@28:
   LDA   OPERANDS
   STA   __zpabi_return_value__value_0
   LDA   OPERANDS+1
   STA   __zpabi_return_value__value_1
   JSR   return_value
   JMP   .switch@17_break
.case@29:
   LDA   OPERANDS
   SEC
   SBC   #$02
   STA   __zpabi_branch_to_offset__offset_0
   LDA   OPERANDS+1
   SBC   #$00
   STA   __zpabi_branch_to_offset__offset_1
   JSR   branch_to_offset
   JMP   .switch@17_break
.case@30:
   LDA   OPERANDS
   STA   __zpabi_load_packed_address__addr_0
   LDA   OPERANDS+1
   STA   __zpabi_load_packed_address__addr_1
   JSR   load_packed_address
   JSR   print_zstring
   JMP   .switch@17_break
.case@31:
   LDA   OPERANDS
   JSR   var_get
   STA   __zpabi_store_result__value_0
   TXA
.xjump@1:
   STA   __zpabi_store_result__value_1
   JSR   store_result
   JMP   .switch@17_break
.case@32:
   LDA   OPERANDS
   EOR   #$FF
   STA   __zpabi_store_result__value_0
   LDA   OPERANDS+1
   EOR   #$FF
   JMP   .xjump@1
.default@33:
   JSR   internal_error
.switch@17_break:
   RTS

do_2op:
   SUBROUTINE

   STA   __local_do_2op__equal
   LDA   OPERANDS
   STA   __local_do_2op__5
   LDA   OPERANDS+1
   STA   __local_do_2op__4
   LDA   OPERANDS+2
   STA   __local_do_2op__2
   LDA   OPERANDS+3
   STA   __local_do_2op__3
   LDA   __local_do_2op__equal
   CMP   #$01
   BNE   .lb_skip@28
   JMP   .case@35
.lb_skip@28:
   CMP   #$02
   BNE   .lb_skip@27
   JMP   .case@36
.lb_skip@27:
   CMP   #$03
   BNE   .lb_skip@26
   JMP   .case@37
.lb_skip@26:
   CMP   #$04
   BNE   .lb_skip@25
   JMP   .case@38
.lb_skip@25:
   CMP   #$05
   BNE   .lb_skip@24
   JMP   .case@39
.lb_skip@24:
   CMP   #$06
   BNE   .lb_skip@23
   JMP   .case@40
.lb_skip@23:
   CMP   #$07
   BNE   .lb_skip@22
   JMP   .case@41
.lb_skip@22:
   CMP   #$08
   BNE   .lb_skip@21
   JMP   .case@42
.lb_skip@21:
   CMP   #$09
   BNE   .lb_skip@20
   JMP   .case@43
.lb_skip@20:
   CMP   #$0A
   BNE   .lb_skip@19
   JMP   .case@44
.lb_skip@19:
   CMP   #$0B
   BNE   .lb_skip@18
   JMP   .case@45
.lb_skip@18:
   CMP   #$0C
   BNE   .lb_skip@17
   JMP   .case@46
.lb_skip@17:
   CMP   #$0D
   BNE   .lb_skip@16
   JMP   .case@47
.lb_skip@16:
   CMP   #$0E
   BNE   .lb_skip@15
   JMP   .case@48
.lb_skip@15:
   CMP   #$0F
   BNE   .lb_skip@14
   JMP   .case@49
.lb_skip@14:
   CMP   #$10
   BNE   .lb_skip@13
   JMP   .case@50
.lb_skip@13:
   CMP   #$11
   BNE   .lb_skip@12
   JMP   .case@51
.lb_skip@12:
   CMP   #$12
   BNE   .lb_skip@11
   JMP   .case@52
.lb_skip@11:
   CMP   #$13
   BNE   .lb_skip@10
   JMP   .case@53
.lb_skip@10:
   CMP   #$14
   BNE   .lb_skip@9
   JMP   .case@54
.lb_skip@9:
   CMP   #$15
   BNE   .lb_skip@8
   JMP   .case@55
.lb_skip@8:
   CMP   #$16
   BNE   .lb_skip@7
   JMP   .case@56
.lb_skip@7:
   CMP   #$17
   BNE   .lb_skip@6
   JMP   .case@57
.lb_skip@6:
   CMP   #$18
   BNE   .lb_skip@5
   JMP   .case@58
.lb_skip@5:
   JMP   .default@59
.case@35:
   LDA   OPERAND_COUNT
   SEC
   SBC   #$01
   STA   __local_do_2op__left
   BNE   .if_end@22
   JSR   internal_error
.if_end@22:
   LDA   __local_do_2op__4
   CMP   __local_do_2op__3
   BNE   .thread@0
   LDA   __local_do_2op__5
   CMP   __local_do_2op__2
   BEQ   .cmp_true@3
.thread@0:
   LDA   #$00
   JMP   .cmp_end@4
.cmp_true@3:
   LDA   #$01
.cmp_end@4:
   STA   __local_do_2op__equal
   ORA   #$00
   BNE   .and_false@23
   DEC   __local_do_2op__left
   BEQ   .and_false@23
   LDA   #$01
   JMP   .and_end@24
.and_false@23:
   LDA   #$00
.and_end@24:
   ORA   #$00
   BEQ   .if_end@25
   LDA   __local_do_2op__4
   CMP   OPERANDS+5
   BNE   .thread@1
   LDA   __local_do_2op__5
   CMP   OPERANDS+4
   BEQ   .cmp_true@6
.thread@1:
   LDA   #$00
   JMP   .cmp_end@7
.cmp_true@6:
   LDA   #$01
.cmp_end@7:
   STA   __local_do_2op__equal
   ORA   #$00
   BNE   .if_end@28
   LDA   __local_do_2op__left
   SEC
   SBC   #$01
   BEQ   .if_end@28
   LDA   __local_do_2op__4
   CMP   OPERANDS+7
   BNE   .thread@2
   LDA   __local_do_2op__5
   CMP   OPERANDS+6
   BEQ   .cmp_true@9
.thread@2:
   LDA   #$00
   JMP   .cmp_end@10
.cmp_true@9:
   LDA   #$01
.cmp_end@10:
   STA   __local_do_2op__equal
.if_end@28:
.if_end@25:
   LDA   __local_do_2op__equal
   JSR   branch
   JMP   .switch@34_break
.case@36:
   LDA   __local_do_2op__5
   SEC
   SBC   __local_do_2op__2
   LDA   __local_do_2op__4
   SBC   __local_do_2op__3
   BVC   .cmp_novf@12
   EOR   #$80
.cmp_novf@12:
   BMI   .cmp_true@13
   LDA   #$00
   JMP   .cmp_end@14
.cmp_true@13:
   LDA   #$01
.cmp_end@14:
   JSR   branch
   JMP   .switch@34_break
.case@37:
   LDA   __local_do_2op__2
   SEC
   SBC   __local_do_2op__5
   LDA   __local_do_2op__3
   SBC   __local_do_2op__4
   BVC   .cmp_novf@15
   EOR   #$80
.cmp_novf@15:
   BMI   .cmp_true@16
   LDA   #$00
   JMP   .cmp_end@17
.cmp_true@16:
   LDA   #$01
.cmp_end@17:
   JSR   branch
   JMP   .switch@34_break
.case@38:
   JSR   dec_var
   SEC
   SBC   __local_do_2op__2
   TXA
   SBC   __local_do_2op__3
   BVC   .cmp_novf@18
   EOR   #$80
.cmp_novf@18:
   BMI   .cmp_true@19
   LDA   #$00
   JMP   .cmp_end@20
.cmp_true@19:
   LDA   #$01
.cmp_end@20:
   JSR   branch
   JMP   .switch@34_break
.case@39:
   JSR   inc_var
   STA   __local_do_2op__0
   STX   __local_do_2op__equal
   LDA   __local_do_2op__2
   SEC
   SBC   __local_do_2op__0
   LDA   __local_do_2op__3
   SBC   __local_do_2op__equal
   BVC   .cmp_novf@21
   EOR   #$80
.cmp_novf@21:
   BMI   .cmp_true@22
   LDA   #$00
   JMP   .cmp_end@23
.cmp_true@22:
   LDA   #$01
.cmp_end@23:
   JSR   branch
   JMP   .switch@34_break
.case@40:
   JSR   instr_jin
   JMP   .switch@34_break
.case@41:
   LDA   __local_do_2op__5
   AND   __local_do_2op__2
   STA   __local_do_2op__0
   LDA   __local_do_2op__4
   AND   __local_do_2op__3
   CMP   __local_do_2op__3
   BNE   .thread@3
   LDA   __local_do_2op__0
   CMP   __local_do_2op__2
   BEQ   .cmp_true@24
.thread@3:
   LDA   #$00
   JMP   .cmp_end@25
.cmp_true@24:
   LDA   #$01
.cmp_end@25:
   JSR   branch
   JMP   .switch@34_break
.case@42:
   LDA   __local_do_2op__5
   ORA   __local_do_2op__2
   STA   __zpabi_store_result__value_0
   LDA   __local_do_2op__4
   ORA   __local_do_2op__3
.xjump@2:
   STA   __zpabi_store_result__value_1
   JSR   store_result
   JMP   .switch@34_break
.case@43:
   LDA   __local_do_2op__5
   AND   __local_do_2op__2
   STA   __zpabi_store_result__value_0
   LDA   __local_do_2op__4
   AND   __local_do_2op__3
   JMP   .xjump@2
.case@44:
   JSR   instr_test_attr
   JMP   .switch@34_break
.case@45:
   JSR   instr_set_attr
   JMP   .switch@34_break
.case@46:
   JSR   instr_clear_attr
   JMP   .switch@34_break
.case@47:
   LDA   __local_do_2op__2
   STA   __zpabi_var_put__value_0
   LDA   __local_do_2op__3
   STA   __zpabi_var_put__value_1
   LDA   __local_do_2op__5
   JSR   var_put
   JMP   .switch@34_break
.case@48:
   JSR   instr_insert_obj
   JMP   .switch@34_break
.case@49:
   ASL   __local_do_2op__2
   ROL   __local_do_2op__3
   LDA   __local_do_2op__5
   CLC
   ADC   __local_do_2op__2
   STA   __zpabi_load_address__addr_0
   LDA   __local_do_2op__4
   ADC   __local_do_2op__3
   STA   __zpabi_load_address__addr_1
   JSR   load_address
   JSR   get_next_code_word
   STA   __zpabi_store_result__value_0
   TXA
   JMP   .xjump@2
.case@50:
   LDA   __local_do_2op__5
   CLC
   ADC   __local_do_2op__2
   STA   __zpabi_load_address__addr_0
   LDA   __local_do_2op__4
   ADC   __local_do_2op__3
   STA   __zpabi_load_address__addr_1
   JSR   load_address
   JSR   get_next_code_byte2
   STA   __zpabi_store_result__value_0
   LDA   #$00
   STA   __zpabi_store_result__value_1
   JSR   store_result
   JMP   .switch@34_break
.case@51:
   JSR   instr_get_prop
   JMP   .switch@34_break
.case@52:
   JSR   instr_get_prop_addr
   JMP   .switch@34_break
.case@53:
   JSR   instr_get_next_prop
   JMP   .switch@34_break
.case@54:
   LDA   __local_do_2op__5
   CLC
   ADC   __local_do_2op__2
   STA   __zpabi_store_result__value_0
   LDA   __local_do_2op__4
   ADC   __local_do_2op__3
   JMP   .xjump@2
.case@55:
   LDA   __local_do_2op__5
   SEC
   SBC   __local_do_2op__2
   STA   __zpabi_store_result__value_0
   LDA   __local_do_2op__4
   SBC   __local_do_2op__3
   JMP   .xjump@2
.case@56:
   JSR   instr_mul
   JMP   .switch@34_break
.case@57:
   JSR   instr_div
   JMP   .switch@34_break
.case@58:
   JSR   instr_mod
   JMP   .switch@34_break
.default@59:
   JSR   internal_error
.switch@34_break:
   RTS

do_var:
   SUBROUTINE

   STA   __local_do_var__n
   LDA   OPERANDS
   STA   __local_do_var__3
   LDA   __local_do_var__n
   BEQ   .case@61
   CMP   #$01
   BEQ   .case@62
   CMP   #$02
   BNE   .lb_skip@36
   JMP   .case@63
.lb_skip@36:
   CMP   #$03
   BNE   .lb_skip@35
   JMP   .case@64
.lb_skip@35:
   CMP   #$04
   BNE   .lb_skip@34
   JMP   .case@65
.lb_skip@34:
   CMP   #$05
   BNE   .lb_skip@33
   JMP   .case@66
.lb_skip@33:
   CMP   #$06
   BNE   .lb_skip@32
   JMP   .case@67
.lb_skip@32:
   CMP   #$07
   BNE   .lb_skip@31
   JMP   .case@68
.lb_skip@31:
   CMP   #$08
   BNE   .lb_skip@30
   JMP   .case@69
.lb_skip@30:
   CMP   #$09
   BNE   .lb_skip@29
   JMP   .case@70
.lb_skip@29:
   JMP   .default@71
.case@61:
   JSR   instr_call
   JMP   .switch@60_break
.case@62:
   LDA   Z_HEADER_ADDR
   CLC
   ADC   __local_do_var__3
   STA   __local_do_var__0
   LDA   Z_HEADER_ADDR+1
   ADC   OPERANDS+1
   STA   __local_do_var__n
   LDA   OPERANDS+2
   STA   __local_do_var__2
   LDA   OPERANDS+3
   ASL   __local_do_var__2
   ROL
   STA   __local_do_var__3
   LDA   __local_do_var__0
   CLC
   ADC   __local_do_var__2
   STA   __local_do_var__0
   LDA   __local_do_var__n
   ADC   __local_do_var__3
   STA   __local_do_var__1
   LDA   OPERANDS+5
   LDY   #$00
   STA   (__local_do_var__0),Y
   LDA   OPERANDS+4
   INY
   STA   (__local_do_var__0),Y
   JMP   .switch@60_break
.case@63:
   LDA   __local_do_var__3
   CLC
   ADC   OPERANDS+2
   STA   __local_do_var__n
   LDA   OPERANDS+1
   ADC   OPERANDS+3
   STA   __local_do_var__1
   LDA   Z_HEADER_ADDR
   CLC
   ADC   __local_do_var__n
   STA   __local_do_var__n
   LDA   Z_HEADER_ADDR+1
   ADC   __local_do_var__1
   STA   __local_do_var__0
   LDA   OPERANDS+4
   LDY   #$00
   STA   (__local_do_var__n),Y
   JMP   .switch@60_break
.case@64:
   JSR   instr_put_prop
   JMP   .switch@60_break
.case@65:
   JSR   instr_sread
   JMP   .switch@60_break
.case@66:
   LDA   __local_do_var__3
   JSR   buffer_char
   JMP   .switch@60_break
.case@67:
   LDA   __local_do_var__3
   STA   __zpabi_print_number__n_0
   LDA   OPERANDS+1
   STA   __zpabi_print_number__n_1
   JSR   print_number
   JMP   .switch@60_break
.case@68:
   JSR   instr_random
   JMP   .switch@60_break
.case@69:
   LDA   __local_do_var__3
   STA   __zpabi_push__value_0
   LDA   OPERANDS+1
   STA   __zpabi_push__value_1
   JSR   push
   JMP   .switch@60_break
.case@70:
   JSR   pop
   STA   __zpabi_var_put__value_0
   STX   __zpabi_var_put__value_1
   LDA   __local_do_var__3
   JSR   var_put
   JMP   .switch@60_break
.default@71:
   JSR   internal_error
.switch@60_break:
   RTS

do_instruction:
   SUBROUTINE

   LDA   #$00
   STA   OPERAND_COUNT
   JSR   get_next_code_byte
   STA   __local_do_instruction__1
   STA   CURR_OPCODE
   CMP   #$80
   BCS   .if_end@29
   AND   #$40
   BEQ   .cond_else@30
   JSR   get_var_content
   STX   __local_do_instruction__v_1
   JMP   .cond_end@31
.cond_else@30:
   JSR   get_next_code_byte
   LDY   #$00
   STY   __local_do_instruction__v_1
.cond_end@31:
   STA   OPERANDS
   LDA   __local_do_instruction__v_1
   STA   OPERANDS+1
   LDA   __local_do_instruction__1
   AND   #$20
   BEQ   .cond_else@32
   JSR   get_var_content
   STX   __local_do_instruction__v_1
   JMP   .cond_end@33
.cond_else@32:
   JSR   get_next_code_byte
   LDY   #$00
   STY   __local_do_instruction__v_1
.cond_end@33:
   STA   OPERANDS+2
   LDA   __local_do_instruction__v_1
   STA   OPERANDS+3
   LDA   #$02
   STA   OPERAND_COUNT
   LDA   __local_do_instruction__1
   AND   #$1F
   JMP   do_2op
.if_end@29:
   CMP   #$B0
   BCS   .if_end@34
   AND   #$30
   BNE   .if_else@36
   JSR   get_const_word
   STA   __local_do_instruction__0
   STX   __local_do_instruction__v_1
   JMP   .if_end@35
.if_else@36:
   CMP   #$10
   BNE   .if_else@38
   JSR   get_next_code_byte
   STA   __local_do_instruction__0
   LDA   #$00
   STA   __local_do_instruction__v_1
   JMP   .if_end@37
.if_else@38:
   JSR   get_var_content
   STA   __local_do_instruction__0
   STX   __local_do_instruction__v_1
.if_end@37:
.if_end@35:
   LDA   #$01
   STA   OPERAND_COUNT
   LDA   __local_do_instruction__0
   STA   OPERANDS
   LDA   __local_do_instruction__v_1
   STA   OPERANDS+1
   LDA   __local_do_instruction__1
   AND   #$0F
   JMP   do_1op
.if_end@34:
   CMP   #$C0
   BCS   .if_end@39
   SEC
   SBC   #$B0
   JMP   do_0op
.if_end@39:
   JSR   get_next_code_byte
   STA   __local_do_instruction__forms
.loop@72_start:
   AND   #$C0
   BNE   .if_else@41
   JSR   get_const_word
.xjump@3:
   STA   __local_do_instruction__v_0
   STX   __local_do_instruction__0
   JMP   .if_end@40
.if_else@41:
   CMP   #$80
   BNE   .if_else@43
   JSR   get_var_content
   JMP   .xjump@3
.if_else@43:
   CMP   #$40
   BNE   .loop@72_break
   JSR   get_next_code_byte
   STA   __local_do_instruction__v_0
   LDA   #$00
   STA   __local_do_instruction__0
.if_end@40:
   LDA   OPERAND_COUNT
   ASL
   TAX
   LDA   __local_do_instruction__v_0
   STA   OPERANDS,X
   LDA   __local_do_instruction__0
   STA   OPERANDS+1,X
   INC   OPERAND_COUNT
   LDA   __local_do_instruction__forms
   ASL
   ASL
   ORA   #$03
   STA   __local_do_instruction__forms
   JMP   .loop@72_start
.loop@72_break:
   LDA   __local_do_instruction__1
   CMP   #$E0
   BCC   .if_else@47
   SBC   #$E0
   JSR   do_var
   JMP   .if_end@46
.if_else@47:
   AND   #$1F
   JSR   do_2op
.if_end@46:
   RTS

set_page_first:
   SUBROUTINE

   TAY
   CPY   FIRST_Z_PAGE
   BNE   .if_end@0
   RTS
.if_end@0:
   LDA   FIRST_Z_PAGE
   STA   __local_set_page_first__prev_first
   STY   FIRST_Z_PAGE
   LDA   (NEXT_PAGE_TABLE),Y
   STA   __local_set_page_first__0
   LDA   __local_set_page_first__prev_first
   STA   (NEXT_PAGE_TABLE),Y
   LDA   (PREV_PAGE_TABLE),Y
   TAX
   LDA   #$FF
   STA   (PREV_PAGE_TABLE),Y
   TXA
   TAY
   LDA   __local_set_page_first__0
   STA   (NEXT_PAGE_TABLE),Y
   LDA   FIRST_Z_PAGE
   LDY   __local_set_page_first__prev_first
   STA   (PREV_PAGE_TABLE),Y
   LDA   __local_set_page_first__0
   CMP   #$FF
   BNE   .if_else@2
   STX   LAST_Z_PAGE
   JMP   .if_end@1
.if_else@2:
   TXA
   LDY   __local_set_page_first__0
   STA   (PREV_PAGE_TABLE),Y
.if_end@1:
   RTS

find_index_of_page_table:
   SUBROUTINE

   LDX   NUM_PAGE_TABLE_ENTRIES
   LDY   #$00
   LDA   __zpabi_find_index_of_page_table__page_0
.loop@0_start:
   CMP   (PAGE_L_TABLE),Y
   BNE   .if_end@5
   LDA   (PAGE_H_TABLE),Y
   CMP   __zpabi_find_index_of_page_table__page_1
   BNE   .find_index_of_page_table@lia@1
   CLC
   TYA
   RTS
.find_index_of_page_table@lia@1:
   LDA   __zpabi_find_index_of_page_table__page_0
.if_end@5:
   INY
   DEX
   BNE   .loop@0_start
   SEC
   LDA   LAST_Z_PAGE
   RTS

get_next_code_byte:
   SUBROUTINE

.loop@1_start:
   LDA   ZCODE_PAGE_VALID
   BEQ   .if_end@6
   LDY   Z_PC
   INC   Z_PC
   BNE   .if_end@7
   INC   Z_PC+1
   LDA   Z_PC
   LDX   #$00
   STX   ZCODE_PAGE_VALID
   ORA   Z_PC+1
   BNE   .if_end@8
   INC   Z_PC+2
.if_end@8:
.if_end@7:
   LDA   (ZCODE_PAGE_ADDR),Y
   RTS
.if_end@6:
   LDA   Z_PC+1
   STA   __zpabi_find_index_of_page_table__page_0
   STA   __local_get_next_code_byte__page
   LDA   Z_PC+2
   STA   __zpabi_find_index_of_page_table__page_1
   BNE   .if_else@12
   LDA   __local_get_next_code_byte__page
   CMP   NUM_IMAGE_PAGES
   BCC   .vthread@0
.if_else@12:
   JSR   find_index_of_page_table
   STA   PAGE_TABLE_INDEX
   BCC   .if_end@13
   CMP   PAGE_TABLE_INDEX2
   BNE   .if_end@14
   LDA   #$00
   STA   ZCODE_PAGE_VALID2
.if_end@14:
   LDA   __zpabi_find_index_of_page_table__page_0
   STA   disk_sector
   LDA   __zpabi_find_index_of_page_table__page_1
   STA   disk_sector+1
   LDA   HIGH_MEM_ADDR
   CLC
   ADC   #$00
   STA   disk_buffer
   LDA   HIGH_MEM_ADDR+1
   ADC   PAGE_TABLE_INDEX
   STA   disk_buffer+1
   JSR   read_from_sector
   BCC   .if_end@15
   JSR   restart
.if_end@15:
   LDA   __local_get_next_code_byte__page
   LDY   PAGE_TABLE_INDEX
   STA   (PAGE_L_TABLE),Y
   LDA   Z_PC+2
   LDY   PAGE_TABLE_INDEX
   STA   (PAGE_H_TABLE),Y
.if_end@13:
   LDA   PAGE_TABLE_INDEX
   JSR   set_page_first
   LDA   PAGE_TABLE_INDEX
   CLC
   ADC   NUM_IMAGE_PAGES
   CLC
.vthread@0:
   ADC   Z_HEADER_ADDR+1
   STA   ZCODE_PAGE_ADDR+1
   LDA   #$00
   STA   ZCODE_PAGE_ADDR
   LDA   #$FF
   STA   ZCODE_PAGE_VALID
   JMP   .loop@1_start

load_address:
   SUBROUTINE

   LDA   __zpabi_load_address__addr_0
   STA   Z_PC2
   LDA   __zpabi_load_address__addr_1
   STA   Z_PC2+1
   LDA   #$00
   STA   Z_PC2+2
   STA   ZCODE_PAGE_VALID2
   RTS

load_packed_address:
   SUBROUTINE

   LDA   __zpabi_load_packed_address__addr_0
   ASL
   STA   Z_PC2
   LDA   __zpabi_load_packed_address__addr_1
   ROL
   STA   Z_PC2+1
   LDA   #$00
   ROL
   STA   Z_PC2+2
   LDA   #$00
   STA   ZCODE_PAGE_VALID2
   RTS

get_next_code_word:
   SUBROUTINE

   JSR   get_next_code_byte2
   STA   __local_get_next_code_word__1
   JSR   get_next_code_byte2
   LDX   __local_get_next_code_word__1
   RTS

get_next_code_byte2:
   SUBROUTINE

.loop@2_start:
   LDA   ZCODE_PAGE_VALID2
   BEQ   .if_end@16
   LDY   Z_PC2
   INC   Z_PC2
   BNE   .if_end@17
   INC   Z_PC2+1
   LDA   Z_PC2
   LDX   #$00
   STX   ZCODE_PAGE_VALID2
   ORA   Z_PC2+1
   BNE   .if_end@18
   INC   Z_PC2+2
.if_end@18:
.if_end@17:
   LDA   (ZCODE_PAGE_ADDR2),Y
   RTS
.if_end@16:
   LDA   Z_PC2+1
   STA   __zpabi_find_index_of_page_table__page_0
   STA   __local_get_next_code_byte2__page
   LDA   Z_PC2+2
   STA   __zpabi_find_index_of_page_table__page_1
   BNE   .if_else@22
   LDA   __local_get_next_code_byte2__page
   CMP   NUM_IMAGE_PAGES
   BCC   .vthread@1
.if_else@22:
   JSR   find_index_of_page_table
   STA   PAGE_TABLE_INDEX2
   BCC   .if_end@23
   CMP   PAGE_TABLE_INDEX
   BNE   .if_end@24
   LDA   #$00
   STA   ZCODE_PAGE_VALID
.if_end@24:
   LDA   __zpabi_find_index_of_page_table__page_0
   STA   disk_sector
   LDA   __zpabi_find_index_of_page_table__page_1
   STA   disk_sector+1
   LDA   HIGH_MEM_ADDR
   CLC
   ADC   #$00
   STA   disk_buffer
   LDA   HIGH_MEM_ADDR+1
   ADC   PAGE_TABLE_INDEX2
   STA   disk_buffer+1
   JSR   read_from_sector
   BCC   .if_end@25
   JSR   restart
.if_end@25:
   LDA   __local_get_next_code_byte2__page
   LDY   PAGE_TABLE_INDEX2
   STA   (PAGE_L_TABLE),Y
   LDA   Z_PC2+2
   LDY   PAGE_TABLE_INDEX2
   STA   (PAGE_H_TABLE),Y
.if_end@23:
   LDA   PAGE_TABLE_INDEX2
   JSR   set_page_first
   LDA   PAGE_TABLE_INDEX2
   CLC
   ADC   NUM_IMAGE_PAGES
   CLC
.vthread@1:
   ADC   Z_HEADER_ADDR+1
   STA   ZCODE_PAGE_ADDR2+1
   LDA   #$00
   STA   ZCODE_PAGE_ADDR2
   LDA   #$FF
   STA   ZCODE_PAGE_VALID2
   JMP   .loop@2_start

A_mod_3:
   SUBROUTINE

   JMP   .loop@1_continue
.A_mod_3@rot@0:
   SBC   #$03
.loop@1_continue:
   CMP   #$03
   BCS   .A_mod_3@rot@0
   RTS

get_alphabet:
   SUBROUTINE

   LDA   SHIFT_ALPHABET
   BPL   .if_end@17
   LDA   LOCKED_ALPHABET
   RTS
.if_end@17:
   LDY   #$FF
   STY   SHIFT_ALPHABET
   RTS

get_next_zchar:
   SUBROUTINE

   LDA   ZDECOMPRESS_STATE
   BPL   .if_end@18
   SEC
   RTS
.if_end@18:
   BNE   .if_else@20
   LDA   #$01
   STA   ZDECOMPRESS_STATE
   JSR   get_next_code_word
   STA   ZCHARS
   STX   ZCHARS+1
   TXA
   LSR
   LSR
   AND   #$1F
   JMP   .if_end@19
.if_else@20:
   CMP   #$01
   BNE   .if_else@22
   LDA   #$02
   STA   ZDECOMPRESS_STATE
   LDA   ZCHARS
   LSR
   LSR
   LSR
   LSR
   LSR
   STA   __local_get_next_zchar__compound_literal_1
   LDA   ZCHARS+1
   ASL
   ASL
   ASL
   ORA   __local_get_next_zchar__compound_literal_1
   AND   #$1F
   JMP   .if_end@21
.if_else@22:
   LDA   ZCHARS+1
   BPL   .cond_else@23
   LDA   #$FF
   JMP   .cond_end@24
.cond_else@23:
   LDA   #$00
.cond_end@24:
   STA   ZDECOMPRESS_STATE
   LDA   ZCHARS
   AND   #$1F
.if_end@21:
.if_end@19:
   CLC
   RTS

print_string_literal:
   SUBROUTINE

   LDA   Z_PC
   STA   Z_PC2
   LDA   Z_PC+1
   STA   Z_PC2+1
   LDA   Z_PC+2
   STA   Z_PC2+2
   LDA   #$00
   STA   ZCODE_PAGE_VALID2
   JSR   print_zstring
   LDA   Z_PC2
   STA   Z_PC
   LDA   Z_PC2+1
   STA   Z_PC+1
   LDA   Z_PC2+2
   STA   Z_PC+2
   LDA   ZCODE_PAGE_VALID2
   STA   ZCODE_PAGE_VALID
   LDA   ZCODE_PAGE_ADDR2
   STA   ZCODE_PAGE_ADDR
   LDA   ZCODE_PAGE_ADDR2+1
   STA   ZCODE_PAGE_ADDR+1
   RTS

dump_buffer_line:
   SUBROUTINE

   LDY   #$11
   LDA   (Z_HEADER_ADDR),Y
   LSR
   BCC   .if_end@9
   JSR   dump_buffer_to_printer
.if_end@9:

dump_buffer_to_screen:
   SUBROUTINE

   LDX   #$00
   JMP   .loop@3_start
.dump_buffer_to_screen@rot@1:
   LDA   BUFF_AREA,X
   JSR   COUT1
   INX
.loop@3_start:
   CPX   BUFF_END
   BNE   .dump_buffer_to_screen@rot@1
   LDA   #$00
   STA   BUFF_END
   RTS

dump_buffer_to_printer:
   SUBROUTINE

   LDA   CSW
   STA   __local_dump_buffer_to_printer__2
   LDA   CSW+1
   STA   __local_dump_buffer_to_printer__1
   LDA   PRINTER_CSW
   STA   CSW
   LDA   PRINTER_CSW+1
   STA   CSW+1
   LDA   printer_card_initialized_flag
   BNE   .if_end@10
   LDA   #$01
   STA   printer_card_initialized_flag
   LDA   #$09
   JSR   COUT
   LDA   #$91
   STA   SLOT1_SCRATCH
   LDA   #$B8
   JSR   COUT
   LDA   #$B0
   JSR   COUT
   LDA   #$CE
   JSR   COUT
.if_end@10:
   LDX   #$00
   JMP   .loop@2_start
.dump_buffer_to_printer@rot@0:
   LDA   BUFF_AREA,X
   STX   __local_dump_buffer_to_printer__x
   JSR   COUT
   LDX   __local_dump_buffer_to_printer__x
   INX
.loop@2_start:
   CPX   BUFF_END
   BNE   .dump_buffer_to_printer@rot@0
   LDA   CSW
   STA   PRINTER_CSW
   LDA   CSW+1
   STA   PRINTER_CSW+1
   LDA   __local_dump_buffer_to_printer__2
   STA   CSW
   LDA   __local_dump_buffer_to_printer__1
   STA   CSW+1
   RTS

dump_buffer_with_more:
   SUBROUTINE

   INC   CURR_LINE
   LDA   CURR_LINE
   CMP   WNDBTM
   BCC   .if_end@11
   LDA   #$3F
   STA   INVFLG
   LDA   #<string_more
   STA   __zpabi_cout_string__s_0
   LDA   #>string_more
   STA   __zpabi_cout_string__s_1
   LDX   #$06
   JSR   cout_string
   LDA   #$FF
   STA   INVFLG
   JSR   RDKEY
   LDA   CH
   SEC
   SBC   #$06
   STA   CH
   JSR   CLREOL
   LDA   WNDTOP
   CLC
   ADC   #$01
   STA   CURR_LINE
.if_end@11:
   LDA   BUFF_END
   STA   __local_dump_buffer_with_more__end
   JSR   dump_buffer_line
   LDA   __local_dump_buffer_with_more__end
   CMP   WNDWDTH
   BEQ   .if_end@12
   LDA   #$8D
   JSR   COUT1
.if_end@12:
   LDY   #$11
   LDA   (Z_HEADER_ADDR),Y
   LSR
   BCC   .if_end@13
   LDA   CSW
   STA   __local_dump_buffer_with_more__end
   LDA   CSW+1
   STA   __local_dump_buffer_with_more__0
   LDA   PRINTER_CSW
   STA   CSW
   LDA   PRINTER_CSW+1
   STA   CSW+1
   LDA   #$8D
   JSR   COUT
   LDA   CSW
   STA   PRINTER_CSW
   LDA   CSW+1
   STA   PRINTER_CSW+1
   LDA   __local_dump_buffer_with_more__end
   STA   CSW
   LDA   __local_dump_buffer_with_more__0
   STA   CSW+1
.if_end@13:
   LDA   #$00
   STA   BUFF_END
   RTS

new_line:
   SUBROUTINE

   LDA   #$0D
   JSR   buffer_char
   LDA   #$0A

buffer_char:
   SUBROUTINE

   LDX   BUFF_END
   CMP   #$0D
   BNE   .if_end@0
   JMP   dump_buffer_with_more
.if_end@0:
   CMP   #$20
   BCS   .if_end@1
   RTS
.if_end@1:
   CMP   #$60
   BCC   .if_end@4
   CMP   #$80
   BCS   .if_end@4
   SEC
   SBC   #$20
.if_end@4:
   ORA   #$80
   STA   BUFF_AREA,X
   CPX   WNDWDTH
   BCS   .if_end@5
   INX
   STX   BUFF_END
   RTS
.if_end@5:
.loop@0_start:
   LDA   BUFF_AREA,X
   CMP   #$A0
   BEQ   .loop@0_break
   DEX
   BNE   .loop@0_start
   LDX   WNDWDTH
.loop@0_break:
   STX   BUFF_LINE_LEN
   STX   BUFF_END
   JSR   dump_buffer_with_more
.loop@1_start:
   LDA   BUFF_LINE_LEN
   CLC
   ADC   #$01
   STA   __local_buffer_char__0
   STA   BUFF_LINE_LEN
   LDA   WNDWDTH
   CMP   __local_buffer_char__0
   BCS   .if_end@8
   RTS
.if_end@8:
   LDX   __local_buffer_char__0
   LDA   BUFF_AREA,X
   LDX   BUFF_END
   INC   BUFF_END
   STA   BUFF_AREA,X
   JMP   .loop@1_start

cout_string:
   SUBROUTINE

   LDY   #$00
.loop@4_start:
   LDA   (__zpabi_cout_string__s_0),Y
   ORA   #$80
   JSR   COUT1
   INY
   DEX
   BNE   .loop@4_start
   RTS

print_ascii_string:
   SUBROUTINE

   STX   __local_print_ascii_string__count
   LDA   #$00
   STA   __local_print_ascii_string__i
.loop@5_start:
   LDY   __local_print_ascii_string__i
   LDA   (__zpabi_print_ascii_string__s_0),Y
   JSR   buffer_char
   INC   __local_print_ascii_string__i
   DEC   __local_print_ascii_string__count
   BNE   .loop@5_start
   RTS

reset_window:
   SUBROUTINE

   LDA   #$01
   STA   WNDTOP
   LDA   #$00
   STA   WNDLFT
   LDA   #$28
   STA   WNDWDTH
   LDA   #$18
   STA   WNDBTM
   LDA   #$3E
   STA   PROMPT
   LDA   #$FF
   STA   INVFLG

home:
   SUBROUTINE

   JSR   HOME
   LDA   WNDTOP
   STA   CURR_LINE
   RTS

get_object_addr:
   SUBROUTINE

   STA   __local_get_object_addr__obj
   LDY   #$0A
   LDA   (Z_HEADER_ADDR),Y
   STA   __local_get_object_addr__0
   INY
   LDA   (Z_HEADER_ADDR),Y
   CLC
   ADC   Z_HEADER_ADDR
   STA   __local_get_object_addr__3
   LDA   Z_HEADER_ADDR+1
   ADC   __local_get_object_addr__0
   STA   __local_get_object_addr__2
   LDA   __local_get_object_addr__obj
   LSR
   LSR
   LSR
   LSR
   LSR
   STA   __local_get_object_addr__1
   LDA   __local_get_object_addr__obj
   ASL
   ASL
   ASL
   CLC
   ADC   __local_get_object_addr__3
   STA   __local_get_object_addr__0
   LDA   __local_get_object_addr__2
   ADC   __local_get_object_addr__1
   TAX
   LDA   __local_get_object_addr__0
   CLC
   ADC   __local_get_object_addr__obj
   BCC   .carry_inc@0
   INX
   CLC
.carry_inc@0:
   ADC   #$35
   BCC   .carry_inc@1
   INX
.carry_inc@1:
   RTS

instr_remove_obj:
   SUBROUTINE


remove_obj:
   SUBROUTINE

   LDA   OPERANDS
   STA   __local_remove_obj__4
   JSR   get_object_addr
   STA   __local_remove_obj__2
   STX   __local_remove_obj__3
   CLC
   ADC   #$04
   TXA
   ADC   #$00
   LDY   #$04
   LDA   (__local_remove_obj__2),Y
   BNE   .if_end@0
   RTS
.if_end@0:
   JSR   get_object_addr
   STA   __local_remove_obj__0
   STX   __local_remove_obj__1
   LDY   #$06
   LDA   (__local_remove_obj__0),Y
   CMP   __local_remove_obj__4
   BNE   .if_else@2
   DEY
   LDA   (__local_remove_obj__2),Y
   INY
   STA   (__local_remove_obj__0),Y
   JMP   .if_end@1
.if_else@2:
.loop@0_start:
   JSR   get_object_addr
   STA   __local_remove_obj__0
   STX   __local_remove_obj__1
   LDY   #$05
   LDA   (__local_remove_obj__0),Y
   CMP   __local_remove_obj__4
   BNE   .loop@0_start
   LDA   (__local_remove_obj__2),Y
   STA   (__local_remove_obj__0),Y
.if_end@1:
   LDA   #$00
   LDY   #$04
   STA   (__local_remove_obj__2),Y
   INY
   STA   (__local_remove_obj__2),Y
   RTS

instr_insert_obj:
   SUBROUTINE

   JSR   remove_obj
   LDA   OPERANDS
   STA   __local_instr_insert_obj__5
   LDA   OPERANDS+2
   STA   __local_instr_insert_obj__0
   LDA   OPERANDS
   JSR   get_object_addr
   STA   __local_instr_insert_obj__3
   STX   __local_instr_insert_obj__4
   LDA   __local_instr_insert_obj__0
   LDY   #$04
   STA   (__local_instr_insert_obj__3),Y
   JSR   get_object_addr
   STA   __local_instr_insert_obj__1
   STX   __local_instr_insert_obj__2
   CLC
   ADC   #$06
   TXA
   ADC   #$00
   LDY   #$06
   LDA   (__local_instr_insert_obj__1),Y
   STA   __local_instr_insert_obj__0
   LDA   __local_instr_insert_obj__5
   STA   (__local_instr_insert_obj__1),Y
   LDA   __local_instr_insert_obj__0
   BEQ   .if_end@3
   DEY
   STA   (__local_instr_insert_obj__3),Y
.if_end@3:
   RTS

print_obj:
   SUBROUTINE

   JSR   get_object_addr
   STA   __local_print_obj__0
   STX   __local_print_obj__1
   LDY   #$07
   LDA   (__local_print_obj__0),Y
   STA   __local_print_obj__obj
   INY
   LDA   (__local_print_obj__0),Y
   CLC
   ADC   #$01
   STA   __zpabi_load_address__addr_0
   LDA   __local_print_obj__obj
   ADC   #$00
   STA   __zpabi_load_address__addr_1
   JSR   load_address

print_zstring:
   SUBROUTINE

   LDA   #$00
   STA   LOCKED_ALPHABET
   STA   ZDECOMPRESS_STATE
   LDA   #$FF
   STA   SHIFT_ALPHABET
.loop@0_start:
   JSR   get_next_zchar
   STA   __local_print_zstring__c_0
   BCC   .if_end@0
   RTS
.if_end@0:
   ORA   #$00
   BNE   .if_else@2
   LDA   #$20
   JMP   .if_end@1
.if_else@2:
   CMP   #$01
   BNE   .if_else@4
   JSR   get_next_zchar
   ASL
   STA   __local_print_zstring__ch
   LDY   __local_print_zstring__ch
   LDA   (Z_ABBREV_TABLE),Y
   STA   __local_print_zstring__0
   LDA   Z_ABBREV_TABLE
   CLC
   ADC   __local_print_zstring__ch
   STA   __local_print_zstring__ch
   LDA   Z_ABBREV_TABLE+1
   ADC   #$00
   STA   __local_print_zstring__c_0
   LDY   #$01
   LDA   (__local_print_zstring__ch),Y
   STA   __zpabi_load_packed_address__addr_0
   LDA   __local_print_zstring__0
   STA   __zpabi_load_packed_address__addr_1
   LDA   LOCKED_ALPHABET
   PHA
   LDA   ZDECOMPRESS_STATE
   PHA
   LDA   ZCHARS
   PHA
   LDA   ZCHARS+1
   PHA
   LDA   Z_PC2
   PHA
   LDA   Z_PC2+1
   PHA
   LDA   Z_PC2+2
   PHA
   JSR   load_packed_address
   JSR   print_zstring
   PLA
   STA   Z_PC2+2
   PLA
   STA   Z_PC2+1
   PLA
   STA   Z_PC2
   PLA
   STA   ZCHARS+1
   PLA
   STA   ZCHARS
   PLA
   STA   ZDECOMPRESS_STATE
   PLA
   STA   LOCKED_ALPHABET
   LDA   #$00
   STA   ZCODE_PAGE_VALID2
   LDA   #$FF
   STA   SHIFT_ALPHABET
   JMP   .loop@0_start
.if_else@4:
   CMP   #$04
   BCS   .if_else@6
   JSR   get_alphabet
   CLC
   ADC   __local_print_zstring__c_0
   CLC
   ADC   #$02
   JSR   A_mod_3
   STA   SHIFT_ALPHABET
   JMP   .loop@0_start
.if_else@6:
   CMP   #$06
   BCS   .if_else@8
   JSR   get_alphabet
   CLC
   ADC   __local_print_zstring__c_0
   JSR   A_mod_3
   STA   LOCKED_ALPHABET
   JMP   .loop@0_start
.if_else@8:
   JSR   get_alphabet
   ORA   #$00
   BNE   .if_else@10
   LDA   #$5B
.xjump@0:
   CLC
   ADC   __local_print_zstring__c_0
   JMP   .if_end@9
.if_else@10:
   CMP   #$01
   BNE   .if_else@12
   LDA   #$3B
   JMP   .xjump@0
.if_else@12:
   LDA   __local_print_zstring__c_0
   CMP   #$06
   BNE   .if_else@14
   JSR   get_next_zchar
   ASL
   ASL
   ASL
   ASL
   ASL
   STA   __local_print_zstring__c_0
   JSR   get_next_zchar
   ORA   __local_print_zstring__c_0
   JMP   .if_end@13
.if_else@14:
   CMP   #$07
   BNE   .if_else@16
   LDA   #$0D
   JSR   buffer_char
   LDA   #$0A
   JMP   .if_end@15
.if_else@16:
   SEC
   SBC   #$08
   TAX
   LDA   a2_table,X
.if_end@15:
.if_end@13:
.if_end@9:
.if_end@1:
   JSR   buffer_char
   JMP   .loop@0_start

instr_get_sibling:
   SUBROUTINE

   LDA   OPERANDS
   JSR   get_object_addr
   STA   __local_instr_get_sibling__0
   STX   __local_instr_get_sibling__1
   LDY   #$05
   LDA   (__local_instr_get_sibling__0),Y

store_and_branch_on:
   SUBROUTINE

   STA   __local_store_and_branch_on__related
   STA   __zpabi_store_result__value_0
   LDA   #$00
   STA   __zpabi_store_result__value_1
   JSR   store_result
   LDA   __local_store_and_branch_on__related
   BEQ   .cmp_end@0
   LDA   #$01
.cmp_end@0:

branch:
   SUBROUTINE

   STA   __local_branch__condition
   JSR   get_next_code_byte
   STA   __local_branch__offset_0
   AND   #$80
   BEQ   .cmp_end@0
   LDA   #$01
.cmp_end@0:
   CMP   __local_branch__condition
   BEQ   .if_end@13
   LDA   __local_branch__offset_0
   AND   #$40
   BNE   .if_end@14
   JSR   get_next_code_byte
.if_end@14:
   RTS
.if_end@13:
   LDA   __local_branch__offset_0
   AND   #$40
   BEQ   .if_else@16
   LDA   __local_branch__offset_0
   AND   #$3F
   STA   __local_branch__offset_0
   LDA   #$00
   STA   __local_branch__offset_1
   JMP   .if_end@15
.if_else@16:
   LDA   __local_branch__offset_0
   AND   #$3F
   STA   __local_branch__offset_1
   JSR   get_next_code_byte
   STA   __local_branch__offset_0
   LDA   __local_branch__offset_1
   AND   #$20
   BEQ   .if_end@17
   LDA   __local_branch__offset_1
   ORA   #$C0
   STA   __local_branch__offset_1
.if_end@17:
.if_end@15:
   LDA   __local_branch__offset_0
   ORA   __local_branch__offset_1
   BNE   .if_end@18
   LDA   #$00
   STA   __zpabi_return_value__value_0
   STA   __zpabi_return_value__value_1
   JMP   return_value
.if_end@18:
   LDA   __local_branch__offset_1
   BNE   .if_end@19
   LDA   __local_branch__offset_0
   CMP   #$01
   BNE   .vthread@0
   LDA   #$01
   STA   __zpabi_return_value__value_0
   LDA   #$00
   STA   __zpabi_return_value__value_1
   JMP   return_value
.if_end@19:
   LDA   __local_branch__offset_0
.vthread@0:
   SEC
   SBC   #$02
   STA   __zpabi_branch_to_offset__offset_0
   LDA   __local_branch__offset_1
   SBC   #$00
   STA   __zpabi_branch_to_offset__offset_1

branch_to_offset:
   SUBROUTINE

   LDA   __zpabi_branch_to_offset__offset_1
   BMI   .lb_skip@0
   JMP   .branch_to_offset@asm_ssa_split@0
.lb_skip@0:
   LDA   #$00
   CLC
   ADC   #$01
   STA   __local_branch_to_offset__pages_1
.if_end@10:
   LDA   Z_PC
   STA   __local_branch_to_offset__0
   CLC
   ADC   __zpabi_branch_to_offset__offset_0
   STA   __local_branch_to_offset__2
   CMP   __local_branch_to_offset__0
   BCS   .if_end@11
   INC   __zpabi_branch_to_offset__offset_1
   BNE   .inc_done@0
   INC   __local_branch_to_offset__pages_1
.inc_done@0:
.if_end@11:
   LDA   __zpabi_branch_to_offset__offset_1
   ORA   __local_branch_to_offset__pages_1
   BNE   .if_end@12
   LDA   Z_PC
   STA   __zpabi_branch_to_offset__offset_1
   LDA   Z_PC+1
   STA   __local_branch_to_offset__0
   LDA   __zpabi_branch_to_offset__offset_1
   AND   #$00
   ORA   __local_branch_to_offset__2
   STA   Z_PC
   LDA   __local_branch_to_offset__0
   STA   Z_PC+1
   RTS
.if_end@12:
   LDA   Z_PC+1
   STA   __local_branch_to_offset__0
   LDA   Z_PC+2
   STA   __local_branch_to_offset__1
   LDA   __local_branch_to_offset__0
   CLC
   ADC   __zpabi_branch_to_offset__offset_1
   STA   __zpabi_branch_to_offset__offset_1
   LDA   __local_branch_to_offset__1
   ADC   __local_branch_to_offset__pages_1
   STA   __local_branch_to_offset__0
   LDA   __local_branch_to_offset__2
   STA   Z_PC
   LDA   __zpabi_branch_to_offset__offset_1
   STA   Z_PC+1
   LDA   __local_branch_to_offset__0
   AND   #$01
   STA   Z_PC+2
   LDA   #$00
   STA   ZCODE_PAGE_VALID
   RTS
.branch_to_offset@asm_ssa_split@0:
   LDA   #$00
   STA   __local_branch_to_offset__pages_1
   JMP   .if_end@10

instr_get_child:
   SUBROUTINE

   LDA   OPERANDS
   JSR   get_object_addr
   STA   __local_instr_get_child__0
   STX   __local_instr_get_child__1
   LDY   #$06
   LDA   (__local_instr_get_child__0),Y
   JMP   store_and_branch_on

instr_get_parent:
   SUBROUTINE

   LDA   OPERANDS
   JSR   get_object_addr
   STA   __local_instr_get_parent__0
   STX   __local_instr_get_parent__1
   LDY   #$04
   LDA   (__local_instr_get_parent__0),Y
   STA   __zpabi_store_result__value_0
   LDA   #$00
   STA   __zpabi_store_result__value_1
   JMP   store_result

instr_jin:
   SUBROUTINE

   LDA   OPERANDS
   JSR   get_object_addr
   STA   __local_instr_jin__2
   STX   __local_instr_jin__3
   LDY   #$04
   LDA   OPERANDS+2
   CMP   (__local_instr_jin__2),Y
   BEQ   .cmp_true@1
   LDA   #$00
   JMP   .cmp_end@2
.cmp_true@1:
   LDA   #$01
.cmp_end@2:
   JMP   branch

attr_ptr_and_mask:
   SUBROUTINE

   LDA   OPERANDS
   JSR   get_object_addr
   STA   attr_ptr
   STX   attr_ptr+1
   LDA   OPERANDS+2
   STA   __local_attr_ptr_and_mask__attr
   CMP   #$10
   BCC   .if_end@4
   SBC   #$10
   STA   __local_attr_ptr_and_mask__attr
   LDA   attr_ptr
   CLC
   ADC   #$02
   STA   attr_ptr
   BCC   .carry_inc@2
   INC   attr_ptr+1
.carry_inc@2:
.if_end@4:
   LDA   #$0F
   SEC
   SBC   __local_attr_ptr_and_mask__attr
   STA   __local_attr_ptr_and_mask__n
   LDA   #$01
   STA   __local_attr_ptr_and_mask__attr
   LDA   #$00
   STA   __local_attr_ptr_and_mask__mask_1
   JMP   .loop@1_start
.attr_ptr_and_mask@rot@0:
   ASL   __local_attr_ptr_and_mask__attr
   ROL   __local_attr_ptr_and_mask__mask_1
   DEC   __local_attr_ptr_and_mask__n
.loop@1_start:
   LDA   __local_attr_ptr_and_mask__n
   BNE   .attr_ptr_and_mask@rot@0
   LDA   __local_attr_ptr_and_mask__attr
   STA   attr_mask
   LDA   __local_attr_ptr_and_mask__mask_1
   STA   attr_mask+1
   LDY   #$00
   LDA   (attr_ptr),Y
   STA   __local_attr_ptr_and_mask__mask_1
   INY
   LDA   (attr_ptr),Y
   LDX   __local_attr_ptr_and_mask__mask_1
   RTS

instr_test_attr:
   SUBROUTINE

   JSR   attr_ptr_and_mask
   AND   attr_mask
   STA   __local_instr_test_attr__1
   TXA
   AND   attr_mask+1
   ORA   __local_instr_test_attr__1
   BEQ   .cmp_end@3
   LDA   #$01
.cmp_end@3:
   JMP   branch

instr_set_attr:
   SUBROUTINE

   JSR   attr_ptr_and_mask
   ORA   attr_mask
   STA   __zpabi_set_attr_word__word_0
   TXA
   ORA   attr_mask+1
   STA   __zpabi_set_attr_word__word_1

set_attr_word:
   SUBROUTINE

   LDA   __zpabi_set_attr_word__word_0
   LDY   #$01
   STA   (attr_ptr),Y
   LDA   __zpabi_set_attr_word__word_1
   DEY
   STA   (attr_ptr),Y
   RTS

instr_clear_attr:
   SUBROUTINE

   JSR   attr_ptr_and_mask
   STA   __local_instr_clear_attr__3
   LDA   attr_mask
   EOR   #$FF
   STA   __local_instr_clear_attr__1
   LDA   attr_mask+1
   EOR   #$FF
   STA   __local_instr_clear_attr__0
   LDA   __local_instr_clear_attr__3
   AND   __local_instr_clear_attr__1
   STA   __zpabi_set_attr_word__word_0
   TXA
   AND   __local_instr_clear_attr__0
   STA   __zpabi_set_attr_word__word_1
   JMP   set_attr_word

get_property_ptr:
   SUBROUTINE

   LDA   OPERANDS
   JSR   get_object_addr
   STA   __local_get_property_ptr__1
   STX   __local_get_property_ptr__2
   LDY   #$07
   LDA   (__local_get_property_ptr__1),Y
   STA   __local_get_property_ptr__0
   INY
   LDA   (__local_get_property_ptr__1),Y
   CLC
   ADC   Z_HEADER_ADDR
   STA   props
   LDA   Z_HEADER_ADDR+1
   ADC   __local_get_property_ptr__0
   STA   props+1
   LDY   #$00
   LDA   (props),Y
   ASL
   CLC
   ADC   #$01
   RTS

get_property_num:
   SUBROUTINE

   TAY
   LDA   (props),Y
   AND   #$1F
   RTS

get_property_len:
   SUBROUTINE

   TAY
   LDA   (props),Y
   LSR
   LSR
   LSR
   LSR
   LSR
   STA   __local_get_property_len__0
   LDA   #$00
   ASL
   ASL
   ASL
   ORA   __local_get_property_len__0
   RTS

next_property:
   SUBROUTINE

   STA   __local_next_property__y
   JSR   get_property_len
   CLC
   ADC   __local_next_property__y
   CLC
   ADC   #$02
   RTS

instr_get_prop:
   SUBROUTINE

   LDA   OPERANDS+2
   STA   __local_instr_get_prop__3
   JSR   get_property_ptr
   STA   __local_instr_get_prop__y
   JMP   .loop@2_start
.instr_get_prop@rot@1:
   BCS   .if_end@6
   LDY   #$0A
   LDA   (Z_HEADER_ADDR),Y
   STA   __local_instr_get_prop__0
   INY
   LDA   (Z_HEADER_ADDR),Y
   ADC   Z_HEADER_ADDR
   STA   __local_instr_get_prop__y
   LDA   Z_HEADER_ADDR+1
   ADC   __local_instr_get_prop__0
   STA   __local_instr_get_prop__2
   LDA   __local_instr_get_prop__3
   ASL
   STA   __local_instr_get_prop__1
   TAY
   DEY
   DEY
   LDA   (__local_instr_get_prop__y),Y
   STA   __local_instr_get_prop__0
   LDA   __local_instr_get_prop__1
   SEC
   SBC   #$01
   TAY
   LDA   (__local_instr_get_prop__y),Y
   STA   __zpabi_store_result__value_0
   LDA   __local_instr_get_prop__0
   STA   __zpabi_store_result__value_1
   JMP   store_result
.if_end@6:
   LDA   __local_instr_get_prop__y
   JSR   next_property
   STA   __local_instr_get_prop__y
.loop@2_start:
   JSR   get_property_num
   CMP   __local_instr_get_prop__3
   BNE   .instr_get_prop@rot@1
   LDA   __local_instr_get_prop__y
   JSR   get_property_len
   STA   __local_instr_get_prop__1
   LDA   __local_instr_get_prop__y
   CLC
   ADC   #$01
   STA   __local_instr_get_prop__0
   LDA   __local_instr_get_prop__1
   BNE   .if_else@8
   LDY   __local_instr_get_prop__0
   LDA   (props),Y
   STA   __zpabi_store_result__value_0
   LDA   #$00
   STA   __zpabi_store_result__value_1
   JSR   store_result
   JMP   .if_end@7
.if_else@8:
   CMP   #$01
   BNE   .if_else@10
   LDY   __local_instr_get_prop__0
   LDA   (props),Y
   STA   __local_instr_get_prop__y
   LDA   props
   CLC
   ADC   __local_instr_get_prop__0
   STA   __local_instr_get_prop__0
   LDA   props+1
   ADC   #$00
   STA   __local_instr_get_prop__1
   LDY   #$01
   LDA   (__local_instr_get_prop__0),Y
   STA   __zpabi_store_result__value_0
   LDA   __local_instr_get_prop__y
   STA   __zpabi_store_result__value_1
   JSR   store_result
   JMP   .if_end@9
.if_else@10:
   JSR   internal_error
.if_end@9:
.if_end@7:
   RTS

instr_get_prop_addr:
   SUBROUTINE

   LDA   OPERANDS+2
   STA   __local_instr_get_prop_addr__1
   JSR   get_property_ptr
   STA   __local_instr_get_prop_addr__y
.loop@3_start:
   JSR   get_property_num
   CMP   __local_instr_get_prop_addr__1
   BNE   .if_end@11
   LDA   props
   CLC
   ADC   __local_instr_get_prop_addr__y
   STA   __local_instr_get_prop_addr__0
   LDA   props+1
   ADC   #$00
   STA   __local_instr_get_prop_addr__y
   INC   __local_instr_get_prop_addr__0
   BNE   .inc_done@0
   INC   __local_instr_get_prop_addr__y
.inc_done@0:
   LDA   __local_instr_get_prop_addr__0
   SEC
   SBC   Z_HEADER_ADDR
   STA   __zpabi_store_result__value_0
   LDA   __local_instr_get_prop_addr__y
   SBC   Z_HEADER_ADDR+1
   STA   __zpabi_store_result__value_1
   JMP   store_result
.if_end@11:
   BCS   .if_end@12
   LDA   #$00
   STA   __zpabi_store_result__value_0
   STA   __zpabi_store_result__value_1
   JMP   store_result
.if_end@12:
   LDA   __local_instr_get_prop_addr__y
   JSR   next_property
   STA   __local_instr_get_prop_addr__y
   JMP   .loop@3_start

instr_get_next_prop:
   SUBROUTINE

   LDA   OPERANDS+2
   STA   __local_instr_get_next_prop__1
   JSR   get_property_ptr
   STA   __local_instr_get_next_prop__y
   LDA   __local_instr_get_next_prop__1
   BEQ   .if_end@13
.loop@4_start:
   LDA   __local_instr_get_next_prop__y
   JSR   get_property_num
   CMP   __local_instr_get_next_prop__1
   BNE   .if_end@14
   LDA   __local_instr_get_next_prop__y
   JSR   next_property
   STA   __local_instr_get_next_prop__y
   JMP   .loop@4_break
.if_end@14:
   BCS   .if_end@15
   LDA   #$00
   STA   __zpabi_store_result__value_0
   STA   __zpabi_store_result__value_1
   JMP   store_result
.if_end@15:
   LDA   __local_instr_get_next_prop__y
   JSR   next_property
   STA   __local_instr_get_next_prop__y
   JMP   .loop@4_start
.loop@4_break:
.if_end@13:
   LDA   __local_instr_get_next_prop__y
   JSR   get_property_num
   STA   __zpabi_store_result__value_0
   LDA   #$00
   STA   __zpabi_store_result__value_1
   JMP   store_result

instr_get_prop_len:
   SUBROUTINE

   LDA   Z_HEADER_ADDR
   CLC
   ADC   OPERANDS
   STA   __local_instr_get_prop_len__2
   LDA   Z_HEADER_ADDR+1
   ADC   OPERANDS+1
   STA   __local_instr_get_prop_len__1
   LDA   __local_instr_get_prop_len__2
   SEC
   SBC   #$01
   STA   __local_instr_get_prop_len__0
   BCS   .carry_inc@3
   DEC   __local_instr_get_prop_len__1
.carry_inc@3:
   LDY   #$00
   LDA   (__local_instr_get_prop_len__0),Y
   LSR
   LSR
   LSR
   LSR
   LSR
   STA   __local_instr_get_prop_len__0
   TYA
   ASL
   ASL
   ASL
   ORA   __local_instr_get_prop_len__0
   CLC
   ADC   #$01
   STA   __zpabi_store_result__value_0
   LDA   #$00
   STA   __zpabi_store_result__value_1
   JMP   store_result

instr_put_prop:
   SUBROUTINE

   LDA   OPERANDS+2
   STA   __local_instr_put_prop__1
   JSR   get_property_ptr
   STA   __local_instr_put_prop__y
   JMP   .loop@5_start
.instr_put_prop@rot@2:
   BCS   .if_end@17
   JSR   internal_error
.if_end@17:
   LDA   __local_instr_put_prop__y
   JSR   next_property
   STA   __local_instr_put_prop__y
.loop@5_start:
   JSR   get_property_num
   CMP   __local_instr_put_prop__1
   BNE   .instr_put_prop@rot@2
   LDA   __local_instr_put_prop__y
   JSR   get_property_len
   STA   __local_instr_put_prop__2
   INC   __local_instr_put_prop__y
   LDA   OPERANDS+4
   STA   __local_instr_put_prop__1
   LDA   __local_instr_put_prop__2
   BNE   .if_else@19
   LDA   __local_instr_put_prop__1
   LDY   __local_instr_put_prop__y
   STA   (props),Y
   JMP   .if_end@18
.if_else@19:
   CMP   #$01
   BNE   .if_else@21
   LDA   OPERANDS+5
   LDY   __local_instr_put_prop__y
   STA   (props),Y
   LDA   props
   CLC
   ADC   __local_instr_put_prop__y
   STA   __local_instr_put_prop__0
   LDA   props+1
   ADC   #$00
   STA   __local_instr_put_prop__y
   LDA   __local_instr_put_prop__1
   LDY   #$01
   STA   (__local_instr_put_prop__0),Y
   JMP   .if_end@20
.if_else@21:
   JSR   internal_error
.if_end@20:
.if_end@18:
   RTS

mulu16:
   SUBROUTINE

   LDA   #$00
   STA   __local_mulu16__product
   LDX   #$00
   JMP   .loop@0_continue
.mulu16@rot@0:
   LDA   __zpabi_mulu16__b_0
   LSR
   BCC   .if_end@0
   LDA   __local_mulu16__product
   CLC
   ADC   __zpabi_mulu16__a_0
   STA   __local_mulu16__product
   TXA
   ADC   __zpabi_mulu16__a_1
   TAX
.if_end@0:
   ASL   __zpabi_mulu16__a_0
   ROL   __zpabi_mulu16__a_1
   LSR   __zpabi_mulu16__b_1
   ROR   __zpabi_mulu16__b_0
.loop@0_continue:
   LDA   __zpabi_mulu16__b_0
   ORA   __zpabi_mulu16__b_1
   BNE   .mulu16@rot@0
   LDA   __local_mulu16__product
   RTS

divu16:
   SUBROUTINE

   LDA   __zpabi_divu16__dividend_0
   LDX   __zpabi_divu16__dividend_1
   LDA   #$00
   STA   __local_divu16__partial_0
   STA   __local_divu16__partial_1
   LDY   #$11
.divu16@rot@1:
   LDA   __local_divu16__partial_0
   SEC
   SBC   __zpabi_divu16__divisor_0
   STA   __local_divu16__0
   LDA   __local_divu16__partial_1
   SBC   __zpabi_divu16__divisor_1
   BCC   .divu16@asm_ssa_split@0
   STA   __local_divu16__partial_1
   LDA   __local_divu16__0
   STA   __local_divu16__partial_0
   LDA   #$01
.if_end@1:
   LSR
   ROL   __zpabi_divu16__dividend_0
   TXA
   ROL
   TAX
   ROL   __local_divu16__partial_0
   ROL   __local_divu16__partial_1
   DEY
   BNE   .divu16@rot@1
   LDA   __local_divu16__partial_1
   LSR
   STA   remainder+1
   LDA   __local_divu16__partial_0
   ROR
   STA   remainder
   LDA   __zpabi_divu16__dividend_0
   RTS
.divu16@asm_ssa_split@0:
   LDA   #$00
   JMP   .if_end@1

check_sign:
   SUBROUTINE

   LDA   __zpabi_check_sign__a_1
   EOR   __zpabi_check_sign__b_1
   STA   sign_bit
   BPL   .check_sign@asm_ssa_split@0
   SEC
   LDA   #$00
   SBC   __zpabi_check_sign__a_0
   STA   __zpabi_check_sign__a_0
   LDA   #$00
   SBC   __zpabi_check_sign__a_1
   TAX
.if_end@2:
   LDA   __zpabi_check_sign__a_0
   RTS
.check_sign@asm_ssa_split@0:
   LDX   __zpabi_check_sign__a_1
   JMP   .if_end@2

instr_mul:
   SUBROUTINE

   LDA   OPERANDS+2
   STA   __zpabi_check_sign__b_0
   LDA   OPERANDS+3
   STA   __zpabi_check_sign__b_1
   LDA   OPERANDS
   STA   __zpabi_check_sign__a_0
   LDA   OPERANDS+1
   STA   __zpabi_check_sign__a_1
   JSR   check_sign
   STA   __zpabi_store_with_sign__n_0
   STX   __zpabi_store_with_sign__n_1
   LDA   __zpabi_check_sign__b_1
   BNE   .if_else@7
   LDA   __zpabi_check_sign__b_0
   CMP   #$02
   BNE   .vthread@0
.xjump@0:
   ASL   __zpabi_store_with_sign__n_0
   ROL   __zpabi_store_with_sign__n_1
   JMP   .if_end@4
.vthread@0:
   CMP   #$04
   BNE   .if_else@7
   ASL   __zpabi_store_with_sign__n_0
   ROL   __zpabi_store_with_sign__n_1
   JMP   .xjump@0
.if_else@7:
   LDA   __zpabi_store_with_sign__n_0
   STA   __zpabi_mulu16__a_0
   STX   __zpabi_mulu16__a_1
   LDA   __zpabi_check_sign__b_0
   STA   __zpabi_mulu16__b_0
   LDA   __zpabi_check_sign__b_1
   STA   __zpabi_mulu16__b_1
   JSR   mulu16
   STA   __zpabi_store_with_sign__n_0
   STX   __zpabi_store_with_sign__n_1
.if_end@4:

store_with_sign:
   SUBROUTINE

   LDA   sign_bit
   BPL   .store_with_sign@asm_ssa_split@0
   SEC
   LDA   #$00
   SBC   __zpabi_store_with_sign__n_0
   STA   __zpabi_store_with_sign__n_0
   LDA   #$00
   SBC   __zpabi_store_with_sign__n_1
.if_end@3:
   STA   __zpabi_store_result__value_1
   LDA   __zpabi_store_with_sign__n_0
   STA   __zpabi_store_result__value_0
   JMP   store_result
.store_with_sign@asm_ssa_split@0:
   LDA   __zpabi_store_with_sign__n_1
   JMP   .if_end@3

instr_div:
   SUBROUTINE

   LDA   OPERANDS+2
   STA   __zpabi_check_sign__b_0
   LDA   OPERANDS+3
   STA   __zpabi_check_sign__b_1
   LDA   OPERANDS
   STA   __zpabi_check_sign__a_0
   LDA   OPERANDS+1
   STA   __zpabi_check_sign__a_1
   JSR   check_sign
   STA   __zpabi_store_with_sign__n_0
   STX   __zpabi_store_with_sign__n_1
   LDA   __zpabi_check_sign__b_1
   BNE   .if_else@11
   LDA   __zpabi_check_sign__b_0
   CMP   #$02
   BNE   .vthread@1
.xjump@1:
   LSR   __zpabi_store_with_sign__n_1
   ROR   __zpabi_store_with_sign__n_0
   JMP   .if_end@8
.vthread@1:
   CMP   #$04
   BNE   .if_else@11
   LSR   __zpabi_store_with_sign__n_1
   ROR   __zpabi_store_with_sign__n_0
   JMP   .xjump@1
.if_else@11:
   LDA   __zpabi_store_with_sign__n_0
   STA   __zpabi_divu16__dividend_0
   STX   __zpabi_divu16__dividend_1
   LDA   __zpabi_check_sign__b_0
   STA   __zpabi_divu16__divisor_0
   LDA   __zpabi_check_sign__b_1
   STA   __zpabi_divu16__divisor_1
   JSR   divu16
   STA   __zpabi_store_with_sign__n_0
   STX   __zpabi_store_with_sign__n_1
.if_end@8:
   JMP   store_with_sign

instr_mod:
   SUBROUTINE

   LDA   OPERANDS+2
   STA   __zpabi_check_sign__b_0
   LDA   OPERANDS+3
   STA   __zpabi_check_sign__b_1
   LDA   OPERANDS
   STA   __zpabi_check_sign__a_0
   LDA   OPERANDS+1
   STA   __zpabi_check_sign__a_1
   JSR   check_sign
   STA   __zpabi_divu16__dividend_0
   STX   __zpabi_divu16__dividend_1
   LDA   __zpabi_check_sign__b_0
   STA   __zpabi_divu16__divisor_0
   LDA   __zpabi_check_sign__b_1
   STA   __zpabi_divu16__divisor_1
   JSR   divu16
   LDA   remainder
   STA   __zpabi_store_result__value_0
   LDA   remainder+1
   STA   __zpabi_store_result__value_1
   JMP   store_result

instr_random:
   SUBROUTINE

   LDA   RND+1
   ASL
   STA   RND+1
   LDA   RND
   STA   __zpabi_divu16__dividend_0
   LDA   RND+1
   STA   __zpabi_divu16__dividend_1
   LDA   OPERANDS
   STA   __zpabi_divu16__divisor_0
   LDA   OPERANDS+1
   STA   __zpabi_divu16__divisor_1
   JSR   divu16
   LDA   remainder
   CLC
   ADC   #$01
   STA   __zpabi_store_result__value_0
   LDA   remainder+1
   ADC   #$00
   STA   __zpabi_store_result__value_1
   JMP   store_result

print_number:
   SUBROUTINE

   LDA   __zpabi_print_number__n_1
   BPL   .if_end@12
   LDA   #$2D
   JSR   buffer_char
   SEC
   LDA   #$00
   SBC   __zpabi_print_number__n_0
   STA   __zpabi_print_number__n_0
   LDA   #$00
   SBC   __zpabi_print_number__n_1
   STA   __zpabi_print_number__n_1
.if_end@12:
   LDA   #$00
   STA   __local_print_number__count
   JMP   .loop@2_continue
.print_number@rot@2:
   LDA   __zpabi_print_number__n_0
   STA   __zpabi_divu16__dividend_0
   LDA   __zpabi_print_number__n_1
   STA   __zpabi_divu16__dividend_1
   LDA   #$0A
   STA   __zpabi_divu16__divisor_0
   LDA   #$00
   STA   __zpabi_divu16__divisor_1
   JSR   divu16
   STA   __zpabi_print_number__n_0
   STX   __zpabi_print_number__n_1
   LDA   remainder
   LDY   __local_print_number__count
   STA   __local_print_number__digits,Y
   INC   __local_print_number__count
.loop@2_continue:
   LDA   __zpabi_print_number__n_0
   ORA   __zpabi_print_number__n_1
   BNE   .print_number@rot@2
   LDA   __local_print_number__count
   BNE   .if_end@13
   LDA   #$30
   JMP   buffer_char
.if_end@13:
.loop@3_start:
   DEC   __local_print_number__count
   LDY   __local_print_number__count
   LDA   __local_print_number__digits,Y
   CLC
   ADC   #$30
   JSR   buffer_char
   LDA   __local_print_number__count
   BNE   .loop@3_start
   RTS
__text_end:
__text_load	EQU	__text_start
   IF    __text_end > $2200
   ECHO  "section .text runs past the end of region low by", [__text_end - $2200]d, "bytes"
   ERR
   ENDIF

   SEG   text_rest
   ORG   $2C00
__text_rest_start:

locate_last_ram_page:
   SUBROUTINE

   LDA   #$C0
   STA   __local_locate_last_ram_page__page
.loop@0_start:
   DEC   __local_locate_last_ram_page__page
   LDA   #$FF
   STA   __local_locate_last_ram_page__2
   LDA   __local_locate_last_ram_page__page
   STA   __local_locate_last_ram_page__3
   LDY   #$00
   LDA   (__local_locate_last_ram_page__2),Y
   CMP   (__local_locate_last_ram_page__2),Y
   BNE   .loop@0_start
   EOR   #$FF
   STA   (__local_locate_last_ram_page__2),Y
   CMP   (__local_locate_last_ram_page__2),Y
   BNE   .loop@0_start
   EOR   #$FF
   STA   (__local_locate_last_ram_page__2),Y
   LDA   __local_locate_last_ram_page__page
   RTS

instr_restart:
   SUBROUTINE

   JSR   dump_buffer_with_more
   JMP   restart

instr_quit:
   SUBROUTINE

   JSR   dump_buffer_with_more
   LDA   #<sEndOfSession
   STA   __zpabi_print_ascii_string__s_0
   LDA   #>sEndOfSession
   STA   __zpabi_print_ascii_string__s_1
   LDX   #$14
   JSR   print_ascii_string
   JSR   dump_buffer_with_more
.loop@5_start:
   JMP   .loop@5_start

internal_error:
   SUBROUTINE

   JSR   dump_buffer_with_more
   LDA   #<sInternalError
   STA   __zpabi_print_ascii_string__s_0
   LDA   #>sInternalError
   STA   __zpabi_print_ascii_string__s_1
   LDX   #$14
   JSR   print_ascii_string
   JSR   dump_buffer_with_more
.loop@6_start:
   JMP   .loop@6_start

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
   STA   __zpabi_print_number__n_0
   STX   __zpabi_print_number__n_1
   JSR   print_number
   LDA   #$2F
   JSR   buffer_char
   LDA   #$12
   JSR   var_get
   STA   __zpabi_print_number__n_0
   STX   __zpabi_print_number__n_1
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
   JMP   .loop@0_continue
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
   LDA   #$5A
   CMP   __local_read_line__c
   BCC   .if_end@5
   LDA   __local_read_line__c
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
   STA   __local_get_dictionary_addr__1
   INY
   LDA   (Z_HEADER_ADDR),Y
   CLC
   ADC   Z_HEADER_ADDR
   STA   __local_get_dictionary_addr__0
   LDA   Z_HEADER_ADDR+1
   ADC   __local_get_dictionary_addr__1
   TAX
   LDA   __local_get_dictionary_addr__0
   RTS

is_dict_separator:
   SUBROUTINE

   JSR   get_dictionary_addr
   STA   __local_is_dict_separator__1
   STX   __local_is_dict_separator__2
   LDY   #$00
   LDA   (__local_is_dict_separator__1),Y
   TAX
   LDA   __zpabi_is_dict_separator__c
   BEQ   .tail@1
.loop@2_start:
   INY
   CMP   (__local_is_dict_separator__1),Y
   BNE   .if_end@8
   SEC
   RTS
.if_end@8:
   DEX
   BNE   .loop@2_start
.tail@1:
   CLC
   RTS

is_separator:
   SUBROUTINE

   JSR   is_dict_separator
   BCC   .if_end@11
   RTS
.if_end@11:

is_std_separator:
   SUBROUTINE

   LDX   #$00
.is_std_separator@rot@1:
   LDA   __zpabi_is_std_separator__c
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

skip_separators:
   SUBROUTINE

   JMP   .loop@4_continue
.skip_separators@rot@2:
   LDY   input_ptr
   LDA   (text),Y
   STA   __zpabi_is_separator__c
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

   LDA   __zpabi_get_alphabet_for_char__c
   CMP   #$61
   BCC   .vthread@0
   LDA   #$7A
   CMP   __zpabi_get_alphabet_for_char__c
   BCC   .if_end@15
   LDA   #$00
   RTS
.if_end@15:
   LDA   __zpabi_get_alphabet_for_char__c
.vthread@0:
   CMP   #$41
   BCC   .if_end@18
   LDA   #$5A
   CMP   __zpabi_get_alphabet_for_char__c
   BCC   .if_end@18
   LDA   #$01
   RTS
.if_end@18:
   LDA   __zpabi_get_alphabet_for_char__c
   BEQ   .vthread@1
   AND   #$80
   BEQ   .if_end@21
   LDA   __zpabi_get_alphabet_for_char__c
.vthread@1:
   RTS
.if_end@21:
   LDA   #$02
   RTS

search_nonalpha_table:
   SUBROUTINE

   LDX   #$17
.loop@5_start:
   LDA   __zpabi_search_nonalpha_table__c
   CMP   a2_table,X
   BNE   .if_end@22
   TXA
   CLC
   ADC   #$08
   RTS
.if_end@22:
   TXA
   SEC
   SBC   #$01
   STA   __local_search_nonalpha_table__0
   TXA
   BNE   .search_nonalpha_table@ssa_split@1
   LDA   #$00
   RTS
.search_nonalpha_table@ssa_split@1:
   LDX   __local_search_nonalpha_table__0
   JMP   .loop@5_start

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
   STA   __zpabi_get_alphabet_for_char__c
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
   STA   __zpabi_get_alphabet_for_char__c
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
   INC   __local_ascii_to_zchar__dest
   DEC   __local_ascii_to_zchar__left
   BEQ   .loop@7_break
.if_end@25:
   LDA   __local_ascii_to_zchar__2
   STA   __zpabi_get_alphabet_for_char__c
   JSR   get_alphabet_for_char
   SEC
   SBC   #$01
   STA   __local_ascii_to_zchar__z
   AND   #$80
   BEQ   .if_else@30
   LDA   __local_ascii_to_zchar__2
   SEC
   SBC   #$5B
   JMP   .if_end@29
.if_else@30:
   LDA   __local_ascii_to_zchar__z
   BNE   .if_else@32
   LDA   __local_ascii_to_zchar__2
   SEC
   SBC   #$3B
   JMP   .if_end@31
.if_else@32:
   LDA   __local_ascii_to_zchar__2
   STA   __zpabi_search_nonalpha_table__c
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
   STA   __local_ascii_to_zchar__z
   LDA   #$00
   ASL
   ASL
   ASL
   ORA   __local_ascii_to_zchar__z
   AND   #$03
   STA   ZCHAR_SCRATCH2,X
   TXA
   CLC
   ADC   #$01
   STA   __local_ascii_to_zchar__dest
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

match_dictionary_word:
   SUBROUTINE

   JSR   get_dictionary_addr
   STA   __local_match_dictionary_word__entry_0
   STX   __local_match_dictionary_word__entry_1
   LDY   #$00
   LDA   (__local_match_dictionary_word__entry_0),Y
   TAY
   INY
   LDA   (__local_match_dictionary_word__entry_0),Y
   STA   __local_match_dictionary_word__8
   ASL
   ASL
   ASL
   ASL
   STA   __local_match_dictionary_word__6
   INY
   LDA   (__local_match_dictionary_word__entry_0),Y
   INY
   STA   __local_match_dictionary_word__count_1
   LDA   (__local_match_dictionary_word__entry_0),Y
   STA   __local_match_dictionary_word__5
   INY
   TYA
   CLC
   ADC   __local_match_dictionary_word__entry_0
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
   BNE   .vthread@2
   RTS
.if_end@48:
   LDA   char_count
.vthread@2:
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
   LDY   input_ptr
   LDA   (text),Y
   STA   __zpabi_is_dict_separator__c
   JSR   is_dict_separator
   BCC   .if_end@51
   LDA   __zpabi_is_dict_separator__c
   STA   ZCHAR_SCRATCH1
   INC   char_count
   DEC   input_count
   INC   input_ptr
   JMP   .instr_sread@search
.if_end@51:
   LDY   input_ptr
   LDA   (text),Y
   STA   __zpabi_is_std_separator__c
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
   STA   __zpabi_is_separator__c
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

read_next_sector:
   SUBROUTINE

   LDA   #<BUFF_AREA
   STA   disk_buffer
   LDA   #>BUFF_AREA
   STA   disk_buffer+1

inc_sector_and_read:
   SUBROUTINE

   INC   disk_sector
   BNE   .inc_done@0
   INC   disk_sector+1
.inc_done@0:

read_from_sector:
   SUBROUTINE

   LDA   #$01

do_rwts_on_sector:
   SUBROUTINE

   STA   iob+12
   LDA   disk_buffer
   STA   iob+8
   LDA   disk_buffer+1
   STA   iob+9
   LDA   #$03
   STA   iob+4
   LDA   disk_sector
   LDX   disk_sector+1
   SEC
.loop@0_continue:
   SBC   SECTORS_PER_TRACK
   BCS   .thread@0
   CPX   #$01
   DEX
   BCC   .loop@0_break
.thread@0:
   INC   iob+4
   JMP   .loop@0_continue
.loop@0_break:
   ADC   SECTORS_PER_TRACK
   STA   iob+5
   LDA   #>iob
   LDY   #<iob
   JMP   RWTS

write_next_sector:
   SUBROUTINE

   LDA   #<BUFF_AREA
   STA   disk_buffer
   LDA   #>BUFF_AREA
   STA   disk_buffer+1

inc_sector_and_write:
   SUBROUTINE

   INC   disk_sector
   BNE   .inc_done@1
   INC   disk_sector+1
.inc_done@1:

write_to_sector:
   SUBROUTINE

   LDA   dct+2
   STA   __local_write_to_sector__2
   LDA   dct+3
   STA   __local_write_to_sector__1
   LDA   #$EF
   STA   dct+2
   LDA   #$D8
   STA   dct+3
   LDA   #$02
   JSR   do_rwts_on_sector
   LDA   __local_write_to_sector__2
   STA   dct+2
   LDA   __local_write_to_sector__1
   STA   dct+3
   RTS

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

copy_data_from_buff:
   SUBROUTINE

.loop@1_start:
   DEY
   LDA   BUFF_AREA,X
   STA   (__zpabi_copy_data_from_buff__dest_0),Y
   INX
   TYA
   BNE   .loop@1_start
   TXA
   RTS

instr_save:
   SUBROUTINE

   JSR   please_insert_save_diskette
   STA   disk_sector
   STX   disk_sector+1
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
   LDA   #$C8
   STA   __zpabi_copy_data_to_buff__src_0
   LDA   #$00
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
   STA   disk_buffer
   LDA   Z_HEADER_ADDR+1
   STA   disk_buffer+1
   LDY   #$0E
   LDA   (Z_HEADER_ADDR),Y
   ADC   #$01
   STA   __local_instr_save__pages
.loop@2_start:
   JSR   inc_sector_and_write
   BCS   .instr_save@fail
   INC   disk_buffer+1
   DEC   __local_instr_save__pages
   BNE   .loop@2_start
   JSR   inc_sector_and_write
   BCS   .instr_save@fail
   JSR   please_reinsert_game_diskette
   LDA   #$01
   JMP   branch
.instr_save@fail:
   JSR   please_reinsert_game_diskette
   LDA   #$00
   JMP   branch

instr_restore:
   SUBROUTINE

   JSR   please_insert_save_diskette
   STA   disk_sector
   STX   disk_sector+1
   JSR   read_next_sector
   BCC   .lb_skip@1
   JMP   .instr_restore@fail
.lb_skip@1:
   LDY   #$00
   LDA   (Z_HEADER_ADDR),Y
   CMP   BUFF_AREA
   BEQ   .lb_skip@0
   JMP   .instr_restore@fail
.lb_skip@0:
   LDY   #$11
   LDA   (Z_HEADER_ADDR),Y
   STA   __local_instr_restore__1
   LDA   #<Z_PC
   STA   __zpabi_copy_data_from_buff__dest_0
   LDA   #>Z_PC
   STA   __zpabi_copy_data_from_buff__dest_1
   LDX   #$01
   LDY   #$03
   JSR   copy_data_from_buff
   TAX
   LDA   #$00
   STA   ZCODE_PAGE_VALID
   LDA   #<LOCAL_ZVARS
   STA   __zpabi_copy_data_from_buff__dest_0
   LDA   #>LOCAL_ZVARS
   STA   __zpabi_copy_data_from_buff__dest_1
   LDY   #$1E
   JSR   copy_data_from_buff
   TAX
   LDA   #$C8
   STA   __zpabi_copy_data_from_buff__dest_0
   LDA   #$00
   STA   __zpabi_copy_data_from_buff__dest_1
   LDY   #$06
   JSR   copy_data_from_buff
   JSR   read_next_sector
   BCS   .instr_restore@fail
   LDA   #$80
   STA   __zpabi_copy_data_from_buff__dest_0
   LDA   #$02
   STA   __zpabi_copy_data_from_buff__dest_1
   LDX   #$00
   LDY   #$00
   JSR   copy_data_from_buff
   JSR   read_next_sector
   BCS   .instr_restore@fail
   LDA   #$80
   STA   __zpabi_copy_data_from_buff__dest_0
   LDA   #$03
   STA   __zpabi_copy_data_from_buff__dest_1
   LDX   #$00
   LDY   #$68
   JSR   copy_data_from_buff
   LDA   Z_HEADER_ADDR
   STA   disk_buffer
   LDA   Z_HEADER_ADDR+1
   STA   disk_buffer+1
   LDY   #$0E
   LDA   (Z_HEADER_ADDR),Y
   CLC
   ADC   #$01
   STA   __local_instr_restore__pages
.loop@3_start:
   JSR   inc_sector_and_read
   BCS   .instr_restore@fail
   INC   disk_buffer+1
   DEC   __local_instr_restore__pages
   BNE   .loop@3_start
   LDA   __local_instr_restore__1
   LDY   #$11
   STA   (Z_HEADER_ADDR),Y
   JSR   please_reinsert_game_diskette
   LDA   #$01
   JMP   branch
.instr_restore@fail:
   JSR   please_reinsert_game_diskette
   LDA   #$00
   JMP   branch

__zp_init:
   SUBROUTINE

   LDA   #$00
   STA   disk_buffer
   STA   disk_buffer+1
   STA   disk_sector
   STA   disk_sector+1
   STA   input_ptr
   STA   char_count
   STA   input_count
   STA   token_idx
   STA   parse
   STA   parse+1
   STA   text
   STA   text+1
   STA   props
   STA   props+1
   STA   remainder
   STA   remainder+1
   STA   attr_ptr
   STA   attr_ptr+1
   STA   sign_bit
   RTS
__text_rest_end:
__text_rest_load	EQU	__text_rest_start
   IF    __text_rest_end > $C000
   ECHO  "section .text.rest runs past the end of region high by", [__text_rest_end - $C000]d, "bytes"
   ERR
   ENDIF

   SEG   rodata
   ORG   __text_rest_end
__rodata_start:

sEndOfSession:
   DC.B  $2D, $2D, $20, $45, $4E, $44, $20, $4F, $46, $20, $53, $45, $53, $53, $49, $4F
   DC.B  $4E, $20, $2D, $2D

sInternalError:
   DC.B  $5A, $4F, $52, $4B, $20, $49, $4E, $54, $45, $52, $4E, $41, $4C, $20, $45, $52
   DC.B  $52, $4F, $52, $21

sScore:
   DC.B  $53, $43, $4F, $52, $45, $3A

SEPARATORS_TABLE:
   DC.B  $20, $2E, $2C, $3F, $0D, $0A, $09, $0C

a2_table:
   DC.B  $30, $31, $32, $33, $34, $35, $36, $37, $38, $39, $2E, $2C, $21, $3F, $5F, $23
   DC.B  $27, $22, $2F, $5C, $2D, $3A, $28, $29

string_more:
   DC.B  $5B, $4D, $4F, $52, $45, $5D

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
   IF    __rodata_end > $C000
   ECHO  "section .rodata runs past the end of region high by", [__rodata_end - $C000]d, "bytes"
   ERR
   ENDIF

   SEG   data
   ORG   __rodata_end
__data_start:

iob:
   DC.B  $01
   DC.B  $60
   DC.B  $01
   DS.B  3
   DC.W  dct
   DS.B  7
   DC.B  $60
   DC.B  $01

dct:
   DS.B  1
   DC.B  $01
   DC.W  $D8EF

prompts:
   DC.B  $53, $4C, $4F, $54, $20, $20, $20, $20, $20, $28, $31, $2D, $37, $29, $3A
   DC.B  $36
   DC.B  $44, $52, $49, $56, $45, $20, $20, $20, $20, $28, $31, $2D, $32, $29, $3A
   DC.B  $32
   DC.B  $50, $4F, $53, $49, $54, $49, $4F, $4E, $20, $28, $30, $2D, $37, $29, $3A
   DC.B  $30
__data_end:
__data_load	EQU	__data_start
   IF    __data_end > $C000
   ECHO  "section .data runs past the end of region high by", [__data_end - $C000]d, "bytes"
   ERR
   ENDIF

   SEG   bss
   ORG   __data_end
__bss_start:

attr_mask:
   DS.B  2

printer_card_initialized_flag:
   DS.B  1
__bss_end:
__bss_load	EQU	__bss_start
   IF    __bss_end > $C000
   ECHO  "section .bss runs past the end of region high by", [__bss_end - $C000]d, "bytes"
   ERR
   ENDIF

   SEG   tables
   ORG   [[__bss_end + 255] & $FF00]
__tables_start:

page_tables:
   DS.B  512
__tables_end:
__tables_load	EQU	__tables_start
   IF    __tables_end > $C000
   ECHO  "section .tables runs past the end of region high by", [__tables_end - $C000]d, "bytes"
   ERR
   ENDIF
__story	EQU	[[__tables_end + 255] & $FF00]
