; elf32-mos executable
   PROCESSOR 6502

HARGS	EQU	$00
SSP	EQU	$01
FP	EQU	$03
__bss_size	EQU	$03
DPTR	EQU	$05
disk_buffer	EQU	$07
disk_sector	EQU	$09
get_object_addr@ret	EQU	$0B
get_dictionary_addr@ret	EQU	$0D
input_ptr	EQU	$0F
char_count	EQU	$10
input_count	EQU	$11
token_idx	EQU	$12
parse	EQU	$13
text	EQU	$15
props	EQU	$17
remainder	EQU	$19
attr_ptr	EQU	$1B
sign_bit	EQU	$1D
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
__data_size	EQU	$45
__monitor_acc	EQU	$45
__dos_zp_46	EQU	$46
__monitor_status	EQU	$48
__dos_zp_49	EQU	$49
RND	EQU	$4E
__local_ascii_to_zchar__z	EQU	$50
__local_branch__offset_1	EQU	$50
__local_instr_call__1	EQU	$50
__local_instr_get_prop__0	EQU	$50
__local_instr_get_prop_addr__0	EQU	$50
__local_match_dictionary_word__0	EQU	$50
__local_print_status_line__1	EQU	$50
__local_ascii_to_zchar__dest	EQU	$51
__local_branch__offset_0	EQU	$51
__local_instr_call__i	EQU	$51
__local_instr_get_prop__1	EQU	$51
__local_instr_get_prop_addr__y	EQU	$51
__local_match_dictionary_word__count_0	EQU	$51
__local_print_status_line__2	EQU	$51
__local_ascii_to_zchar__left	EQU	$52
__local_branch__condition	EQU	$52
__local_divu16__partial_1	EQU	$52
__local_instr_call__args	EQU	$52
__local_instr_get_prop__y	EQU	$52
__local_instr_get_prop_addr__1	EQU	$52
__local_match_dictionary_word__5	EQU	$52
__local_print_zstring__ch	EQU	$52
__local_ascii_to_zchar__0	EQU	$53
__local_divu16__partial_0	EQU	$53
__local_instr_call__2	EQU	$53
__local_instr_get_prop__2	EQU	$53
__local_match_dictionary_word__6	EQU	$53
__local_print_zstring__c_0	EQU	$53
__local_return_value__0	EQU	$53
__local_ascii_to_zchar__1	EQU	$54
__local_divu16__0	EQU	$54
__local_instr_get_prop__3	EQU	$54
__local_match_dictionary_word__count_1	EQU	$54
__local_print_zstring__0	EQU	$54
__local_return_value__1	EQU	$54
__local_ascii_to_zchar__2	EQU	$55
__local_get_object_addr__0	EQU	$55
__local_match_dictionary_word__8	EQU	$55
__local_return_value__x	EQU	$55
__local_ascii_to_zchar__index	EQU	$56
__local_get_object_addr__1	EQU	$56
__local_match_dictionary_word__entry_0	EQU	$56
__local_return_value__count	EQU	$56
__local_get_object_addr__2	EQU	$57
__local_match_dictionary_word__entry_1	EQU	$57
__local_print_ascii_string__count	EQU	$57
__local_set_nonstack_var__var	EQU	$57
__local_get_object_addr__3	EQU	$58
__local_print_ascii_string__i	EQU	$58
__local_set_nonstack_var__0	EQU	$58
__local_dump_buffer_with_more__0	EQU	$59
__local_get_object_addr__obj	EQU	$59
__local_set_nonstack_var__1	EQU	$59
__local_dump_buffer_with_more__end	EQU	$5A
__local_dump_buffer_to_printer__x	EQU	$5B
__local_dump_buffer_to_printer__1	EQU	$5C
__local_dump_buffer_to_printer__2	EQU	$5D
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
__local_dec_var__0	EQU	$E0
__local_get_const_word__1	EQU	$E0
__local_get_dictionary_addr__0	EQU	$E0
__local_get_next_zchar__compound_literal_1	EQU	$E0
__local_inc_var__0	EQU	$E0
__local_instr_get_next_prop__y	EQU	$E0
__local_mulu16__product	EQU	$E0
__local_please_insert_save_diskette__0	EQU	$E0
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
__local_get_next_code_word__1	EQU	$E1
__local_instr_get_next_prop__1	EQU	$E1
__local_please_insert_save_diskette__1	EQU	$E1
__local_search_nonalpha_table__0	EQU	$E1
__local_var_put__var	EQU	$E1
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
Z_ABBREV_TABLE	EQU	$E2
__rodata_size	EQU	$E2
__local_instr_clear_attr__0	EQU	$E4
__local_instr_insert_obj__0	EQU	$E4
__local_instr_put_prop__0	EQU	$E4
__local_instr_restore__pages	EQU	$E4
__local_instr_save__pages	EQU	$E4
__local_instr_test_attr__1	EQU	$E4
__local_locate_last_ram_page__2	EQU	$E4
__local_store_and_branch_on__related	EQU	$E4
__zpabi_check_sign__b_0	EQU	$E4
__zpabi_is_dict_separator__c	EQU	$E4
__zpabi_is_separator__c	EQU	$E4
__zpabi_is_std_separator__c	EQU	$E4
__local_instr_clear_attr__1	EQU	$E5
__local_instr_insert_obj__3	EQU	$E5
__local_instr_put_prop__y	EQU	$E5
__local_instr_restore__1	EQU	$E5
__local_instr_sread__1	EQU	$E5
__local_locate_last_ram_page__3	EQU	$E5
__zpabi_check_sign__b_1	EQU	$E5
__local_branch_to_offset__0	EQU	$E6
__local_get_prompted_number_from_user__key	EQU	$E6
__local_instr_clear_attr__3	EQU	$E6
__local_instr_insert_obj__4	EQU	$E6
__local_instr_put_prop__1	EQU	$E6
__local_locate_last_ram_page__page	EQU	$E6
__local_pop__1	EQU	$E6
__local_print_obj__obj	EQU	$E6
__local_read_line__c	EQU	$E6
__local_z_compress__0	EQU	$E6
__zpabi_divu16__dividend_0	EQU	$E6
__zpabi_mulu16__a_0	EQU	$E6
__zpabi_store_result__value_0	EQU	$E6
__local_attr_ptr_and_mask__attr	EQU	$E7
__local_branch_to_offset__pages_1	EQU	$E7
__local_get_next_code_byte2__page	EQU	$E7
__local_get_prompted_number_from_user__1	EQU	$E7
__local_instr_insert_obj__5	EQU	$E7
__local_instr_put_prop__2	EQU	$E7
__local_pop__2	EQU	$E7
__local_read_line__left	EQU	$E7
__local_z_compress__1	EQU	$E7
__zpabi_divu16__dividend_1	EQU	$E7
__zpabi_mulu16__a_1	EQU	$E7
__zpabi_store_result__value_1	EQU	$E7
__local_attr_ptr_and_mask__mask_1	EQU	$E8
__local_branch_to_offset__2	EQU	$E8
__local_get_property_ptr__0	EQU	$E8
__local_next_property__y	EQU	$E8
__local_read_line__n	EQU	$E8
__local_remove_obj__2	EQU	$E8
__local_z_compress__2	EQU	$E8
__zpabi_divu16__divisor_0	EQU	$E8
__zpabi_find_index_of_page_table__page_0	EQU	$E8
__zpabi_mulu16__b_0	EQU	$E8
__zpabi_push__value_0	EQU	$E8
__zpabi_var_put__value_0	EQU	$E8
__local_attr_ptr_and_mask__n	EQU	$E9
__local_remove_obj__3	EQU	$E9
__local_z_compress__3	EQU	$E9
__zpabi_divu16__divisor_1	EQU	$E9
__zpabi_find_index_of_page_table__page_1	EQU	$E9
__zpabi_mulu16__b_1	EQU	$E9
__zpabi_push__value_1	EQU	$E9
__zpabi_var_put__value_1	EQU	$E9
__local_remove_obj__4	EQU	$EA
__local_store_result__0	EQU	$EA
BUFF_END	EQU	$EB
BUFF_LINE_LEN	EQU	$EC
CURR_LINE	EQU	$ED
PRINTER_CSW	EQU	$EE
__local_get_next_code_byte__page	EQU	$F0
__local_get_nonstack_var__var	EQU	$F0
__local_print_number__count	EQU	$F0
__zpabi_print_ascii_string__s_0	EQU	$F0
__zpabi_set_nonstack_var__value_0	EQU	$F0
__local_get_nonstack_var__0	EQU	$F1
__local_print_number__digits	EQU	$F1
__local_set_page_first__0	EQU	$F1
__zpabi_print_ascii_string__s_1	EQU	$F1
__zpabi_set_nonstack_var__value_1	EQU	$F1
__local_get_nonstack_var__1	EQU	$F2
__local_set_page_first__prev_first	EQU	$F2
__zpabi_cout_string__s_0	EQU	$F2
__local_get_nonstack_var__2	EQU	$F3
__zpabi_cout_string__s_1	EQU	$F3
__local_main__0	EQU	$F4
__local_main__sector	EQU	$F5
__local_do_instruction__0	EQU	$F6
__local_do_instruction__v_0	EQU	$F7
__local_do_instruction__forms	EQU	$F8
__local_do_instruction__1	EQU	$F9
__local_do_2op__equal	EQU	$FA
__local_do_var__n	EQU	$FA
__local_instr_get_prop_len__0	EQU	$FA
__local_do_2op__0	EQU	$FB
__local_do_var__0	EQU	$FB
__local_instr_get_prop_len__1	EQU	$FB
__local_do_2op__1	EQU	$FC
__local_do_var__1	EQU	$FC
__local_instr_get_prop_len__2	EQU	$FC
__local_do_2op__2	EQU	$FD
__local_do_var__2	EQU	$FD
__local_do_var__3	EQU	$FE
__local_buffer_char__0	EQU	$FF
__main_size	EQU	$0147
BUFF_AREA	EQU	$0200
__tables_size	EQU	$0200
SLOT1_SCRATCH	EQU	$0779
__text_rest_size	EQU	$07C9
__main_load	EQU	$0800
__main_start	EQU	$0800
restart	EQU	$0800
__main_end	EQU	$0947
__main_load_end	EQU	$0947
__text_load	EQU	$0947
__text_start	EQU	$0947
__text_size	EQU	$1119
__text_end	EQU	$1A60
__text_load_end	EQU	$1A60
RWTS	EQU	$2900
__text_rest_load	EQU	$2C00
__text_rest_start	EQU	$2C00
__rodata_load	EQU	$33C9
__rodata_start	EQU	$33C9
__text_rest_end	EQU	$33C9
__text_rest_load_end	EQU	$33C9
__data_load	EQU	$34AB
__data_start	EQU	$34AB
__rodata_end	EQU	$34AB
__rodata_load_end	EQU	$34AB
__bss_load	EQU	$34F0
__bss_start	EQU	$34F0
__data_end	EQU	$34F0
__data_load_end	EQU	$34F0
__bss_end	EQU	$34F3
__bss_load_end	EQU	$34F3
__tables_load	EQU	$3500
__tables_start	EQU	$3500
__story	EQU	$3700
__tables_end	EQU	$3700
__tables_load_end	EQU	$3700
VTAB	EQU	$FC22
HOME	EQU	$FC58
CLREOL	EQU	$FC9C
WAIT	EQU	$FCA8
RDKEY	EQU	$FD0C
GETLN1	EQU	$FD6F
COUT	EQU	$FDED
COUT1	EQU	$FDF0

; Disassembly of section .main at $0800
   SEG   main
   ORG   $0800
main:
   SUBROUTINE
   LDA   #$C1
   STA   PRINTER_CSW+1
   LDA   #$00
   STA   PRINTER_CSW
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
   LDA   #$00
   STA   PAGE_L_TABLE
   STA   NEXT_PAGE_TABLE
   LDA   #$35
   STA   PAGE_L_TABLE+1
   LDA   #$80
   STA   PAGE_H_TABLE
   LDA   #$35
   STA   PAGE_H_TABLE+1
   LDA   #$36
   STA   NEXT_PAGE_TABLE+1
   LDA   #$80
   STA   PREV_PAGE_TABLE
   LDA   #$36
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
   LDA   #$00
   STA   Z_HEADER_ADDR
   LDA   #$37
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
   JMP   main
.if_end@2:
   LDA   #$FF
   LDY   #$05
   STA   (Z_HEADER_ADDR),Y
   DEY
   LDA   (Z_HEADER_ADDR),Y
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
   STA   disk_buffer
   LDA   Z_HEADER_ADDR+1
   ADC   __local_main__sector
   STA   disk_buffer+1
   JSR   read_from_sector
   BCC   .if_end@3
   JMP   main
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
   JMP   internal_error
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
   STA   HIGH_MEM_ADDR
   LDA   Z_HEADER_ADDR+1
   ADC   NUM_IMAGE_PAGES
   STA   HIGH_MEM_ADDR+1
   STA   __local_main__sector
   JSR   __text_rest_load
   CMP   __local_main__sector
   BCS   .if_end@5
   JMP   internal_error
.if_end@5:
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

; Disassembly of section .text at $0947
   SEG   text
   ORG   $0947
push:
   SUBROUTINE
   LDA   Z_SP
   SEC
   SBC   #$01
   STA   Z_SP
   BCS   .carry_inc@0
   DEC   Z_SP+1
.carry_inc@0:
   LDA   __local_attr_ptr_and_mask__mask_1
   LDY   #$00
   STA   (Z_SP),Y
   LDA   Z_SP
   SEC
   SBC   #$01
   STA   Z_SP
   BCS   .carry_inc@1
   DEC   Z_SP+1
.carry_inc@1:
   LDA   __local_attr_ptr_and_mask__n
   LDY   #$00
   STA   (Z_SP),Y
   INC   STACK_COUNT
   LDA   STACK_COUNT
   CMP   #$B4
   BCC   .if_end@0
   JMP   internal_error
.if_end@0:
   RTS
pop:
   SUBROUTINE
   LDY   #$00
   LDA   (Z_SP),Y
   STA   __local_attr_ptr_and_mask__attr
   INY
   LDA   (Z_SP),Y
   STA   __local_branch_to_offset__0
   LDA   Z_SP
   CLC
   ADC   #$02
   STA   Z_SP
   BCC   .carry_inc@2
   INC   Z_SP+1
.carry_inc@2:
   DEC   STACK_COUNT
   BNE   .if_end@1
   JMP   internal_error
.if_end@1:
   LDA   __local_branch_to_offset__0
   LDX   __local_attr_ptr_and_mask__attr
   RTS
var_get:
   SUBROUTINE
   ORA   #$00
   BNE   get_nonstack_var
   JSR   pop
   STA   __local_attr_ptr_and_mask__mask_1
   STX   __local_attr_ptr_and_mask__n
   JSR   __main_end
   LDX   __local_attr_ptr_and_mask__n
   LDA   __local_attr_ptr_and_mask__mask_1
   RTS
get_nonstack_var:
   SUBROUTINE
   STA   __local_get_next_code_byte__page
   CMP   #$10
   BCS   .if_end@2
   DEC   __local_get_next_code_byte__page
   LDA   __local_get_next_code_byte__page
   ASL
   TAX
   LDA   LOCAL_ZVARS+1,X
   STA   __local_get_next_code_byte__page
   LDA   LOCAL_ZVARS,X
   TAX
   LDA   __local_get_next_code_byte__page
   RTS
.if_end@2:
   SBC   #$10
   ASL
   STA   __local_get_nonstack_var__0
   LDA   #$00
   ROL
   STA   __local_get_next_code_byte__page
   LDA   GLOBAL_ZVARS_ADDR
   CLC
   ADC   __local_get_nonstack_var__0
   STA   __local_get_nonstack_var__1
   LDA   GLOBAL_ZVARS_ADDR+1
   ADC   __local_get_next_code_byte__page
   STA   __local_get_nonstack_var__2
   LDY   #$00
   LDA   (__local_get_nonstack_var__1),Y
   STA   __local_get_nonstack_var__0
   INY
   LDA   (__local_get_nonstack_var__1),Y
   LDX   __local_get_nonstack_var__0
   RTS
get_const_word:
   SUBROUTINE
   JSR   get_next_code_byte
   STA   __local_dec_var__0
   JSR   get_next_code_byte
   LDX   __local_dec_var__0
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
   STA   __local_ascii_to_zchar__index
   ORA   #$00
   BEQ   .if_end@9
   ASL
   STA   __local_ascii_to_zchar__2
.loop@0_start:
   JSR   pop
   STX   __local_ascii_to_zchar__0
   LDX   __local_ascii_to_zchar__2
   DEX
   DEX
   STX   __local_ascii_to_zchar__2
   STA   LOCAL_ZVARS+1,X
   LDA   __local_ascii_to_zchar__0
   STA   LOCAL_ZVARS,X
   DEC   __local_ascii_to_zchar__index
   BNE   .loop@0_start
.if_end@9:
   JSR   pop
   STA   __local_ascii_to_zchar__index
   STX   __local_ascii_to_zchar__2
   JSR   pop
   STA   FRAME_Z_SP
   STX   FRAME_Z_SP+1
   JSR   pop
   STA   __local_ascii_to_zchar__1
   STX   Z_PC
   LDA   __local_ascii_to_zchar__index
   STA   Z_PC+1
   LDA   __local_ascii_to_zchar__2
   STA   Z_PC+2
   LDA   __local_ascii_to_zchar__1
   STA   FRAME_STACK_COUNT
   LDA   #$00
   STA   ZCODE_PAGE_VALID
   LDA   __local_dec_var__0
   STA   __local_branch_to_offset__0
   LDA   __local_get_next_code_word__1
   STA   __local_attr_ptr_and_mask__attr
store_result:
   SUBROUTINE
   JSR   get_next_code_byte
   STA   __local_remove_obj__4
   ORA   #$00
   BNE   .if_else@7
   LDA   __local_branch_to_offset__0
   STA   __local_attr_ptr_and_mask__mask_1
   LDA   __local_attr_ptr_and_mask__attr
   STA   __local_attr_ptr_and_mask__n
   JMP   __main_end
.if_else@7:
   LDA   __local_branch_to_offset__0
   STA   __local_get_next_code_byte__page
   LDA   __local_attr_ptr_and_mask__attr
   STA   __local_get_nonstack_var__0
   LDA   __local_remove_obj__4
   JMP   set_nonstack_var
instr_call:
   SUBROUTINE
   LDA   OPERANDS
   ORA   OPERANDS+1
   BNE   .if_end@20
   LDA   #$00
   STA   __local_branch_to_offset__0
   STA   __local_attr_ptr_and_mask__attr
   JMP   store_result
.if_end@20:
   LDA   FRAME_STACK_COUNT
   STA   __local_attr_ptr_and_mask__mask_1
   LDA   Z_PC
   STA   __local_attr_ptr_and_mask__n
   JSR   __main_end
   LDA   FRAME_Z_SP
   STA   __local_attr_ptr_and_mask__mask_1
   LDA   FRAME_Z_SP+1
   STA   __local_attr_ptr_and_mask__n
   JSR   __main_end
   LDA   Z_PC+1
   STA   __local_attr_ptr_and_mask__mask_1
   LDA   Z_PC+2
   STA   __local_attr_ptr_and_mask__n
   JSR   __main_end
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
   STA   __local_ascii_to_zchar__0
   ORA   #$00
   BEQ   .if_end@21
   LDY   #$00
   STY   __local_ascii_to_zchar__left
   STA   __local_ascii_to_zchar__dest
.loop@1_start:
   LDX   __local_ascii_to_zchar__left
   LDA   LOCAL_ZVARS,X
   STA   __local_attr_ptr_and_mask__n
   LDA   LOCAL_ZVARS+1,X
   STA   __local_attr_ptr_and_mask__mask_1
   JSR   __main_end
   JSR   get_next_code_byte
   STA   __local_ascii_to_zchar__z
   JSR   get_next_code_byte
   LDX   __local_ascii_to_zchar__left
   STA   LOCAL_ZVARS+1,X
   LDA   __local_ascii_to_zchar__z
   STA   LOCAL_ZVARS,X
   INX
   INX
   STX   __local_ascii_to_zchar__left
   DEC   __local_ascii_to_zchar__dest
   BNE   .loop@1_start
.if_end@21:
   LDA   OPERAND_COUNT
   SEC
   SBC   #$01
   STA   __local_ascii_to_zchar__left
   LDY   #$00
   STY   __local_ascii_to_zchar__dest
   JMP   .loop@2_continue
.instr_call@rot@0:
   LDA   __local_ascii_to_zchar__dest
   ASL
   TAX
   LDA   OPERANDS+3,X
   STA   LOCAL_ZVARS,X
   LDA   OPERANDS+2,X
   STA   LOCAL_ZVARS+1,X
   INC   __local_ascii_to_zchar__dest
   DEC   __local_ascii_to_zchar__left
.loop@2_continue:
   LDA   __local_ascii_to_zchar__left
   BNE   .instr_call@rot@0
   LDA   __local_ascii_to_zchar__0
   STA   __local_attr_ptr_and_mask__mask_1
   LDA   #$00
   STA   __local_attr_ptr_and_mask__n
   JSR   __main_end
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
   STA   __local_dec_var__0
   JSR   var_get
   STA   __local_attr_ptr_and_mask__mask_1
   STX   __local_attr_ptr_and_mask__n
   INC   __local_attr_ptr_and_mask__mask_1
   BNE   .inc_done@1
   INC   __local_attr_ptr_and_mask__n
.inc_done@1:
   LDA   __local_dec_var__0
   JSR   var_put
   LDX   __local_attr_ptr_and_mask__n
   LDA   __local_attr_ptr_and_mask__mask_1
   RTS
dec_var:
   SUBROUTINE
   LDA   OPERANDS
   STA   __local_dec_var__0
   JSR   var_get
   STX   __local_attr_ptr_and_mask__n
   SEC
   SBC   #$01
   STA   __local_attr_ptr_and_mask__mask_1
   BCS   .carry_inc@3
   DEC   __local_attr_ptr_and_mask__n
.carry_inc@3:
   LDA   __local_dec_var__0
   JSR   var_put
   LDX   __local_attr_ptr_and_mask__n
   LDA   __local_attr_ptr_and_mask__mask_1
   RTS
do_0op:
   SUBROUTINE
   CMP   #$0C
   BCS   .default@16
   ASL
   TAY
   LDA   $0B6E,Y
   PHA
   LDA   .jtab@0,Y
   PHA
   RTS
.jtab@0:
   DC.B  $84, $0B, $8F, $0B, $86, $11, $98, $0B
   DC.B  $AE, $0B, $5F, $32, $F5, $32, $24, $2C
   DC.B  $A1, $0B, $74, $09, $2A, $2C, $9B, $12
.case@4:
   LDA   #$01
   STA   __local_dec_var__0
   LDA   #$00
   STA   __local_get_next_code_word__1
   JMP   return_value
.case@5:
   LDA   #$00
   STA   __local_dec_var__0
   STA   __local_get_next_code_word__1
   JMP   return_value
.case@7:
   JSR   print_string_literal
   JSR   new_line
   JMP   .case@4
.case@12:
   JSR   pop
   STA   __local_dec_var__0
   STX   __local_get_next_code_word__1
   JMP   return_value
.default@16:
   JMP   internal_error
.switch@3_break:
   RTS
do_1op:
   SUBROUTINE
   CMP   #$10
   BCC   .lb_skip@0
   JMP   internal_error
.lb_skip@0:
   ASL
   TAY
   LDA   $0BC3,Y
   PHA
   LDA   .jtab@1,Y
   PHA
   RTS
.jtab@1:
   DC.B  $E1, $0B, $FD, $14, $D2, $15, $DE, $15
   DC.B  $01, $18, $26, $0B, $41, $0B, $F0, $0B
   DC.B  $40, $2C, $80, $13, $FE, $0B, $03, $0C
   DC.B  $0E, $0C, $1E, $0C, $2C, $0C, $38, $0C
.case@18:
   LDA   OPERANDS+1
   ORA   OPERANDS
   BEQ   .cmp_true@2
   LDA   #$00
   BEQ   .cmp_end@1
.cmp_true@2:
   LDA   #$01
.cmp_end@1:
   JMP   branch
.case@25:
   LDA   OPERANDS
   STA   __local_dec_var__0
   LDA   OPERANDS+1
   STA   __local_get_next_code_word__1
   JSR   load_address
   JMP   print_zstring
.case@27:
   LDA   OPERANDS
   JMP   print_obj
.case@28:
   LDA   OPERANDS
   STA   __local_dec_var__0
   LDA   OPERANDS+1
   STA   __local_get_next_code_word__1
   JMP   return_value
.case@29:
   LDA   OPERANDS
   SEC
   SBC   #$02
   STA   __local_dec_var__0
   LDA   OPERANDS+1
   SBC   #$00
   STA   __local_get_next_code_word__1
   JMP   branch_to_offset
.case@30:
   LDA   OPERANDS
   STA   __local_dec_var__0
   LDA   OPERANDS+1
   STA   __local_get_next_code_word__1
   JSR   load_packed_address
   JMP   print_zstring
.case@31:
   LDA   OPERANDS
   JSR   var_get
   STA   __local_branch_to_offset__0
   STX   __local_attr_ptr_and_mask__attr
   JMP   store_result
.case@32:
   LDA   OPERANDS
   EOR   #$FF
   STA   __local_branch_to_offset__0
   LDA   OPERANDS+1
   EOR   #$FF
   STA   __local_attr_ptr_and_mask__attr
   JMP   store_result
do_var:
   SUBROUTINE
   LDX   OPERANDS
   STX   __local_do_var__3
   CMP   #$0A
   BCC   .lb_skip@2
   JMP   internal_error
.lb_skip@2:
   ASL
   TAY
   LDA   $0C5F,Y
   PHA
   LDA   .jtab@3,Y
   PHA
   RTS
.jtab@3:
   DC.B  $75, $0A, $71, $0C, $A2, $0C, $2E, $18
   DC.B  $9A, $2F, $C3, $0C, $C8, $0C, $E5, $19
   DC.B  $D3, $0C, $DE, $0C
.case@62:
   LDA   Z_HEADER_ADDR
   CLC
   ADC   __local_do_var__3
   STA   __local_do_2op__0
   LDA   Z_HEADER_ADDR+1
   ADC   OPERANDS+1
   STA   __local_do_2op__equal
   LDA   OPERANDS+2
   STA   __local_do_2op__2
   LDA   OPERANDS+3
   ASL   __local_do_2op__2
   ROL
   STA   __local_do_var__3
   LDA   __local_do_2op__0
   CLC
   ADC   __local_do_2op__2
   STA   __local_do_2op__0
   LDA   __local_do_2op__equal
   ADC   __local_do_var__3
   STA   __local_do_2op__1
   LDA   OPERANDS+5
   LDY   #$00
   STA   (__local_do_2op__0),Y
   LDA   OPERANDS+4
   INY
   STA   (__local_do_2op__0),Y
   RTS
.case@63:
   LDA   __local_do_var__3
   CLC
   ADC   OPERANDS+2
   STA   __local_do_2op__equal
   LDA   OPERANDS+1
   ADC   OPERANDS+3
   STA   __local_do_2op__1
   LDA   Z_HEADER_ADDR
   CLC
   ADC   __local_do_2op__equal
   STA   __local_do_2op__equal
   LDA   Z_HEADER_ADDR+1
   ADC   __local_do_2op__1
   STA   __local_do_2op__0
   LDA   OPERANDS+4
   LDY   #$00
   STA   (__local_do_2op__equal),Y
   RTS
.case@66:
   LDA   __local_do_var__3
   JMP   buffer_char
.case@67:
   LDA   __local_do_var__3
   STA   __local_dec_var__0
   LDA   OPERANDS+1
   STA   __local_get_next_code_word__1
   JMP   print_number
.case@69:
   LDA   __local_do_var__3
   STA   __local_attr_ptr_and_mask__mask_1
   LDA   OPERANDS+1
   STA   __local_attr_ptr_and_mask__n
   JMP   __main_end
.case@70:
   JSR   pop
   STA   __local_attr_ptr_and_mask__mask_1
   STX   __local_attr_ptr_and_mask__n
   LDA   __local_do_var__3
var_put:
   SUBROUTINE
   STA   __local_get_next_code_word__1
   ORA   #$00
   BNE   .if_end@5
   JSR   pop
   JMP   __main_end
.if_end@5:
   LDA   __local_attr_ptr_and_mask__mask_1
   STA   __local_get_next_code_byte__page
   LDA   __local_attr_ptr_and_mask__n
   STA   __local_get_nonstack_var__0
   LDA   __local_get_next_code_word__1
set_nonstack_var:
   SUBROUTINE
   STA   __local_get_object_addr__2
   CMP   #$10
   BCS   .if_end@3
   DEC   __local_get_object_addr__2
   LDA   __local_get_object_addr__2
   ASL
   TAX
   LDA   __local_get_nonstack_var__0
   STA   LOCAL_ZVARS,X
   LDA   __local_get_next_code_byte__page
   STA   LOCAL_ZVARS+1,X
   RTS
.if_end@3:
   SBC   #$10
   ASL
   STA   __local_get_object_addr__2
   LDA   #$00
   ROL
   STA   __local_dump_buffer_with_more__0
   LDA   GLOBAL_ZVARS_ADDR
   CLC
   ADC   __local_get_object_addr__2
   STA   __local_get_object_addr__2
   LDA   GLOBAL_ZVARS_ADDR+1
   ADC   __local_dump_buffer_with_more__0
   STA   __local_get_object_addr__3
   LDA   __local_get_nonstack_var__0
   LDY   #$00
   STA   (__local_get_object_addr__2),Y
   LDA   __local_get_next_code_byte__page
   INY
   STA   (__local_get_object_addr__2),Y
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
   STX   OPERANDS+1
   JMP   .cond_end@31
.cond_else@30:
   JSR   get_next_code_byte
   LDY   #$00
   STY   OPERANDS+1
.cond_end@31:
   STA   OPERANDS
   LDA   __local_do_instruction__1
   AND   #$20
   BEQ   .cond_else@32
   JSR   get_var_content
   STX   OPERANDS+3
   JMP   .cond_end@33
.cond_else@32:
   JSR   get_next_code_byte
   LDY   #$00
   STY   OPERANDS+3
.cond_end@33:
   STA   OPERANDS+2
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
   STA   OPERANDS
   STX   OPERANDS+1
   JMP   .if_end@37
.if_else@36:
   CMP   #$10
   BNE   .if_else@38
   JSR   get_next_code_byte
   STA   OPERANDS
   LDA   #$00
   STA   OPERANDS+1
   JMP   .if_end@37
.if_else@38:
   JSR   get_var_content
   STA   OPERANDS
   STX   OPERANDS+1
.if_end@35:
.if_end@37:
   LDA   #$01
   STA   OPERAND_COUNT
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
.xjump@0:
   STA   __local_do_instruction__v_0
   STX   __local_do_instruction__0
   JMP   .if_end@40
.if_else@41:
   CMP   #$80
   BNE   .if_else@43
   JSR   get_var_content
   JMP   .xjump@0
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
   JMP   do_var
.if_else@47:
   AND   #$1F
do_2op:
   SUBROUTINE
   STA   __local_do_2op__equal
   LDA   OPERANDS+2
   STA   __local_do_2op__1
   LDA   OPERANDS+3
   STA   __local_do_2op__2
   LDA   __local_do_2op__equal
   CMP   #$19
   BCC   .lb_skip@1
   JMP   internal_error
.lb_skip@1:
   ASL
   TAY
   LDA   $0E29,Y
   PHA
   LDA   .jtab@2,Y
   PHA
   RTS
.jtab@2:
   DC.B  $40, $2C, $59, $0E, $BE, $0E, $D2, $0E
   DC.B  $E2, $0E, $F2, $0E, $F0, $15, $09, $0F
   DC.B  $26, $0F, $35, $0F, $5C, $16, $71, $16
   DC.B  $8B, $16, $44, $0F, $C3, $13, $51, $0F
   DC.B  $6F, $0F, $E7, $16, $76, $17, $BE, $17
   DC.B  $8B, $0F, $9B, $0F, $08, $19, $6D, $19
   DC.B  $B8, $19
.case@35:
   LDA   OPERAND_COUNT
   SEC
   SBC   #$01
   STA   __local_do_2op__0
   BNE   .if_end@22
   JMP   internal_error
.if_end@22:
   LDA   OPERANDS+1
   CMP   __local_do_2op__2
   BNE   .thread@0
   LDA   OPERANDS
   CMP   __local_do_2op__1
   BEQ   .cmp_true@3
.thread@0:
   LDA   #$00
   BEQ   .cmp_end@4
.cmp_true@3:
   LDA   #$01
.cmp_end@4:
   STA   __local_do_2op__equal
   ORA   #$00
   BNE   .if_end@25
   LDA   __local_do_2op__0
   SEC
   SBC   #$01
   STA   __local_do_2op__1
   BEQ   .if_end@25
   LDA   OPERANDS+1
   CMP   OPERANDS+5
   BNE   .thread@1
   LDA   OPERANDS
   CMP   OPERANDS+4
   BEQ   .cmp_true@6
.thread@1:
   LDA   #$00
   BEQ   .cmp_end@7
.cmp_true@6:
   LDA   #$01
.cmp_end@7:
   STA   __local_do_2op__equal
   ORA   #$00
   BNE   .if_end@25
   LDA   __local_do_2op__1
   SEC
   SBC   #$01
   BEQ   .if_end@25
   LDA   OPERANDS+1
   CMP   OPERANDS+7
   BNE   .thread@2
   LDA   OPERANDS
   CMP   OPERANDS+6
   BEQ   .cmp_true@9
.thread@2:
   LDA   #$00
   BEQ   .cmp_end@10
.cmp_true@9:
   LDA   #$01
.cmp_end@10:
   STA   __local_do_2op__equal
.if_end@25:
.if_end@28:
   LDA   __local_do_2op__equal
   JMP   branch
.case@36:
   LDA   OPERANDS
   SEC
   SBC   __local_do_2op__1
   LDA   OPERANDS+1
   SBC   __local_do_2op__2
   BVC   .cmp_novf@12
   EOR   #$80
.cmp_novf@12:
   ASL
   LDA   #$00
   ROL
   JMP   branch
.case@37:
   LDA   __local_do_2op__1
   SEC
   SBC   OPERANDS
   LDA   __local_do_2op__2
   SBC   OPERANDS+1
   BVC   .cmp_novf@12
   EOR   #$80
   JMP   .cmp_novf@12
.case@38:
   JSR   dec_var
   SEC
   SBC   __local_do_2op__1
   TXA
   SBC   __local_do_2op__2
   BVC   .cmp_novf@12
   EOR   #$80
   JMP   .cmp_novf@12
.case@39:
   JSR   inc_var
   STA   __local_do_2op__0
   STX   __local_do_2op__equal
   LDA   __local_do_2op__1
   SEC
   SBC   __local_do_2op__0
   LDA   __local_do_2op__2
   SBC   __local_do_2op__equal
   BVC   .cmp_novf@12
   EOR   #$80
   JMP   .cmp_novf@12
.case@41:
   LDA   OPERANDS
   AND   __local_do_2op__1
   STA   __local_do_2op__0
   LDA   OPERANDS+1
   AND   __local_do_2op__2
   CMP   __local_do_2op__2
   BNE   .thread@3
   LDA   __local_do_2op__0
   CMP   __local_do_2op__1
   BEQ   .cmp_true@16
.thread@3:
   LDA   #$00
   BEQ   .cmp_end@17
.cmp_true@16:
   LDA   #$01
.cmp_end@17:
   JMP   branch
.case@42:
   LDA   OPERANDS
   ORA   __local_do_2op__1
   STA   __local_branch_to_offset__0
   LDA   OPERANDS+1
   ORA   __local_do_2op__2
   STA   __local_attr_ptr_and_mask__attr
   JMP   store_result
.case@43:
   LDA   OPERANDS
   AND   __local_do_2op__1
   STA   __local_branch_to_offset__0
   LDA   OPERANDS+1
   AND   __local_do_2op__2
   STA   __local_attr_ptr_and_mask__attr
   JMP   store_result
.case@47:
   LDA   __local_do_2op__1
   STA   __local_attr_ptr_and_mask__mask_1
   LDA   __local_do_2op__2
   STA   __local_attr_ptr_and_mask__n
   LDA   OPERANDS
   JMP   var_put
.case@49:
   ASL   __local_do_2op__1
   ROL   __local_do_2op__2
   LDA   OPERANDS
   CLC
   ADC   __local_do_2op__1
   STA   __local_dec_var__0
   LDA   OPERANDS+1
   ADC   __local_do_2op__2
   STA   __local_get_next_code_word__1
   JSR   load_address
   JSR   get_next_code_word
   STA   __local_branch_to_offset__0
   STX   __local_attr_ptr_and_mask__attr
   JMP   store_result
.case@50:
   LDA   OPERANDS
   CLC
   ADC   __local_do_2op__1
   STA   __local_dec_var__0
   LDA   OPERANDS+1
   ADC   __local_do_2op__2
   STA   __local_get_next_code_word__1
   JSR   load_address
   JSR   get_next_code_byte2
   STA   __local_branch_to_offset__0
   LDA   #$00
   STA   __local_attr_ptr_and_mask__attr
   JMP   store_result
.case@54:
   LDA   OPERANDS
   CLC
   ADC   __local_do_2op__1
   STA   __local_branch_to_offset__0
   LDA   OPERANDS+1
   ADC   __local_do_2op__2
   STA   __local_attr_ptr_and_mask__attr
   JMP   store_result
.case@55:
   LDA   OPERANDS
   SEC
   SBC   __local_do_2op__1
   STA   __local_branch_to_offset__0
   LDA   OPERANDS+1
   SBC   __local_do_2op__2
   STA   __local_attr_ptr_and_mask__attr
   JMP   store_result
set_page_first:
   SUBROUTINE
   TAY
   CPY   FIRST_Z_PAGE
   BNE   .if_end@0
   RTS
.if_end@0:
   LDA   FIRST_Z_PAGE
   STA   __local_get_nonstack_var__1
   STY   FIRST_Z_PAGE
   LDA   (NEXT_PAGE_TABLE),Y
   STA   __local_get_nonstack_var__0
   LDA   __local_get_nonstack_var__1
   STA   (NEXT_PAGE_TABLE),Y
   LDA   (PREV_PAGE_TABLE),Y
   TAX
   LDA   #$FF
   STA   (PREV_PAGE_TABLE),Y
   TXA
   TAY
   LDA   __local_get_nonstack_var__0
   STA   (NEXT_PAGE_TABLE),Y
   LDA   FIRST_Z_PAGE
   LDY   __local_get_nonstack_var__1
   STA   (PREV_PAGE_TABLE),Y
   LDA   __local_get_nonstack_var__0
   CMP   #$FF
   BNE   .if_else@2
   STX   LAST_Z_PAGE
   RTS
.if_else@2:
   TXA
   LDY   __local_get_nonstack_var__0
   STA   (PREV_PAGE_TABLE),Y
   RTS
find_index_of_page_table:
   SUBROUTINE
   LDX   NUM_PAGE_TABLE_ENTRIES
   LDY   #$00
   LDA   __local_attr_ptr_and_mask__mask_1
.loop@0_start:
   CMP   (PAGE_L_TABLE),Y
   BNE   .if_end@5
   LDA   (PAGE_H_TABLE),Y
   CMP   __local_attr_ptr_and_mask__n
   BNE   .find_index_of_page_table@lia@1
   CLC
   TYA
   RTS
.find_index_of_page_table@lia@1:
   LDA   __local_attr_ptr_and_mask__mask_1
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
   BNE   .if_end@7
   INC   Z_PC+2
.if_end@7:
.if_end@8:
   LDA   (ZCODE_PAGE_ADDR),Y
   RTS
.if_end@6:
   LDA   Z_PC+1
   STA   __local_attr_ptr_and_mask__mask_1
   STA   __local_get_next_code_byte__page
   LDA   Z_PC+2
   STA   __local_attr_ptr_and_mask__n
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
   LDA   __local_attr_ptr_and_mask__mask_1
   STA   disk_sector
   LDA   __local_attr_ptr_and_mask__n
   STA   disk_sector+1
   LDA   HIGH_MEM_ADDR
   CLC
   STA   disk_buffer
   LDA   HIGH_MEM_ADDR+1
   ADC   PAGE_TABLE_INDEX
   STA   disk_buffer+1
   JSR   read_from_sector
   BCC   .if_end@15
   JMP   main
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
   LDA   __local_dec_var__0
   STA   Z_PC2
   LDA   __local_get_next_code_word__1
   STA   Z_PC2+1
   LDA   #$00
   STA   Z_PC2+2
   STA   ZCODE_PAGE_VALID2
   RTS
load_packed_address:
   SUBROUTINE
   LDA   __local_dec_var__0
   ASL
   STA   Z_PC2
   LDA   __local_get_next_code_word__1
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
   BNE   .if_end@17
   INC   Z_PC2+2
.if_end@17:
.if_end@18:
   LDA   (ZCODE_PAGE_ADDR2),Y
   RTS
.if_end@16:
   LDA   Z_PC2+1
   STA   __local_attr_ptr_and_mask__mask_1
   STA   __local_attr_ptr_and_mask__attr
   LDA   Z_PC2+2
   STA   __local_attr_ptr_and_mask__n
   BNE   .if_else@22
   LDA   __local_attr_ptr_and_mask__attr
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
   LDA   __local_attr_ptr_and_mask__mask_1
   STA   disk_sector
   LDA   __local_attr_ptr_and_mask__n
   STA   disk_sector+1
   LDA   HIGH_MEM_ADDR
   CLC
   STA   disk_buffer
   LDA   HIGH_MEM_ADDR+1
   ADC   PAGE_TABLE_INDEX2
   STA   disk_buffer+1
   JSR   read_from_sector
   BCC   .if_end@25
   JMP   main
.if_end@25:
   LDA   __local_attr_ptr_and_mask__attr
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
   JMP   .if_end@21
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
   STA   __local_dec_var__0
   LDA   ZCHARS+1
   ASL
   ASL
   ASL
   ORA   __local_dec_var__0
   AND   #$1F
   JMP   .if_end@21
.if_else@22:
   LDA   ZCHARS+1
   BPL   .cond_else@23
   LDA   #$FF
   BNE   .cond_end@24
.cond_else@23:
   LDA   #$00
.cond_end@24:
   STA   ZDECOMPRESS_STATE
   LDA   ZCHARS
   AND   #$1F
.if_end@19:
.if_end@21:
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
   BCC   dump_buffer_to_screen
   JSR   dump_buffer_to_printer
dump_buffer_to_screen:
   SUBROUTINE
   LDX   #$00
   BEQ   .loop@3_start
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
   BEQ   .loop@2_start
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
   LDA   #$17
   STA   __local_get_nonstack_var__1
   LDA   #$34
   STA   __local_get_nonstack_var__2
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
   BNE   .if_end@5
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
   LDA   (__local_get_nonstack_var__1),Y
   ORA   #$80
   JSR   COUT1
   INY
   DEX
   BNE   .loop@4_start
   RTS
print_ascii_string:
   SUBROUTINE
   STX   __local_get_object_addr__2
   LDA   #$00
   STA   __local_get_object_addr__3
.loop@5_start:
   LDY   __local_get_object_addr__3
   LDA   (__local_get_next_code_byte__page),Y
   JSR   buffer_char
   INC   __local_get_object_addr__3
   DEC   __local_get_object_addr__2
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
   STA   __local_dump_buffer_with_more__0
   LDY   #$0A
   LDA   (Z_HEADER_ADDR),Y
   STA   __local_ascii_to_zchar__2
   INY
   LDA   (Z_HEADER_ADDR),Y
   CLC
   ADC   Z_HEADER_ADDR
   STA   __local_get_object_addr__3
   LDA   Z_HEADER_ADDR+1
   ADC   __local_ascii_to_zchar__2
   STA   __local_get_object_addr__2
   LDA   __local_dump_buffer_with_more__0
   LSR
   LSR
   LSR
   LSR
   LSR
   STA   __local_ascii_to_zchar__index
   LDA   __local_dump_buffer_with_more__0
   ASL
   ASL
   ASL
   CLC
   ADC   __local_get_object_addr__3
   STA   __local_ascii_to_zchar__2
   LDA   __local_get_object_addr__2
   ADC   __local_ascii_to_zchar__index
   STA   __local_ascii_to_zchar__index
   LDA   __local_ascii_to_zchar__2
   CLC
   ADC   __local_dump_buffer_with_more__0
   BCC   .carry_inc@0
   INC   __local_ascii_to_zchar__index
   CLC
.carry_inc@0:
   ADC   #$35
   STA   get_object_addr@ret
   LDA   __local_ascii_to_zchar__index
   ADC   #$00
   STA   get_object_addr@ret+1
   RTS
instr_remove_obj:
   SUBROUTINE
remove_obj:
   SUBROUTINE
   LDA   OPERANDS
   STA   __local_remove_obj__4
   JSR   get_object_addr
   LDA   get_object_addr@ret
   STA   __local_attr_ptr_and_mask__mask_1
   LDA   get_object_addr@ret+1
   STA   __local_attr_ptr_and_mask__n
   LDY   #$04
   LDA   (get_object_addr@ret),Y
   BNE   .if_end@0
   RTS
.if_end@0:
   JSR   get_object_addr
   LDY   #$06
   LDA   (get_object_addr@ret),Y
   CMP   __local_remove_obj__4
   BNE   .if_else@2
   DEY
   LDA   (__local_attr_ptr_and_mask__mask_1),Y
   INY
   STA   (get_object_addr@ret),Y
   JMP   .if_end@1
.if_else@2:
.loop@0_start:
   JSR   get_object_addr
   LDY   #$05
   LDA   (get_object_addr@ret),Y
   CMP   __local_remove_obj__4
   BNE   .if_else@2
   LDA   (__local_attr_ptr_and_mask__mask_1),Y
   STA   (get_object_addr@ret),Y
.if_end@1:
   LDA   #$00
   LDY   #$04
   STA   (__local_attr_ptr_and_mask__mask_1),Y
   INY
   STA   (__local_attr_ptr_and_mask__mask_1),Y
   RTS
instr_insert_obj:
   SUBROUTINE
   JSR   instr_remove_obj
   LDA   OPERANDS
   STA   __local_attr_ptr_and_mask__attr
   LDA   OPERANDS+2
   STA   __local_instr_clear_attr__0
   LDA   OPERANDS
   JSR   get_object_addr
   LDA   get_object_addr@ret
   STA   __local_instr_clear_attr__1
   LDA   get_object_addr@ret+1
   STA   __local_branch_to_offset__0
   LDA   __local_instr_clear_attr__0
   LDY   #$04
   STA   (get_object_addr@ret),Y
   JSR   get_object_addr
   LDY   #$06
   LDA   (get_object_addr@ret),Y
   STA   __local_instr_clear_attr__0
   LDA   __local_attr_ptr_and_mask__attr
   STA   (get_object_addr@ret),Y
   LDA   __local_instr_clear_attr__0
   BEQ   .if_end@3
   DEY
   STA   (__local_instr_clear_attr__1),Y
.if_end@3:
   RTS
print_obj:
   SUBROUTINE
   JSR   get_object_addr
   LDY   #$07
   LDA   (get_object_addr@ret),Y
   STA   __local_branch_to_offset__0
   INY
   LDA   (get_object_addr@ret),Y
   CLC
   ADC   #$01
   STA   __local_dec_var__0
   LDA   __local_branch_to_offset__0
   ADC   #$00
   STA   __local_get_next_code_word__1
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
   STA   __local_ascii_to_zchar__0
   BCC   .if_end@0
   RTS
.if_end@0:
   ORA   #$00
   BNE   .if_else@2
   LDA   #$20
   JMP   .if_end@15
.if_else@2:
   CMP   #$01
   BNE   .if_else@4
   JSR   get_next_zchar
   ASL
   STA   __local_ascii_to_zchar__left
   LDY   __local_ascii_to_zchar__left
   LDA   (Z_ABBREV_TABLE),Y
   STA   __local_ascii_to_zchar__1
   LDA   Z_ABBREV_TABLE
   CLC
   ADC   __local_ascii_to_zchar__left
   STA   __local_ascii_to_zchar__left
   LDA   Z_ABBREV_TABLE+1
   ADC   #$00
   STA   __local_ascii_to_zchar__0
   LDY   #$01
   LDA   (__local_ascii_to_zchar__left),Y
   STA   __local_dec_var__0
   LDA   __local_ascii_to_zchar__1
   STA   __local_get_next_code_word__1
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
   ADC   __local_ascii_to_zchar__0
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
   ADC   __local_ascii_to_zchar__0
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
   ADC   __local_ascii_to_zchar__0
   JMP   .if_end@15
.if_else@10:
   CMP   #$01
   BNE   .if_else@12
   LDA   #$3B
   BNE   .xjump@0
.if_else@12:
   LDA   __local_ascii_to_zchar__0
   CMP   #$06
   BNE   .if_else@14
   JSR   get_next_zchar
   ASL
   ASL
   ASL
   ASL
   ASL
   STA   __local_ascii_to_zchar__0
   JSR   get_next_zchar
   ORA   __local_ascii_to_zchar__0
   JMP   .if_end@15
.if_else@14:
   CMP   #$07
   BNE   .if_else@16
   LDA   #$0D
   JSR   buffer_char
   LDA   #$0A
   BNE   .if_end@1
.if_else@16:
   SEC
   SBC   #$08
   TAX
   LDA   a2_table,X
.if_end@1:
.if_end@13:
.if_end@15:
.if_end@9:
   JSR   buffer_char
   JMP   .loop@0_start
instr_get_sibling:
   SUBROUTINE
   LDA   OPERANDS
   JSR   get_object_addr
   LDY   #$05
   LDA   (get_object_addr@ret),Y
store_and_branch_on:
   SUBROUTINE
   STA   __local_instr_clear_attr__0
   STA   __local_branch_to_offset__0
   LDA   #$00
   STA   __local_attr_ptr_and_mask__attr
   JSR   store_result
   LDA   __local_instr_clear_attr__0
   BEQ   branch
   LDA   #$01
branch:
   SUBROUTINE
   STA   __local_ascii_to_zchar__left
   JSR   get_next_code_byte
   STA   __local_ascii_to_zchar__dest
   AND   #$80
   BEQ   .cmp_end@0
   LDA   #$01
.cmp_end@0:
   CMP   __local_ascii_to_zchar__left
   BEQ   .if_end@13
   BIT   __local_ascii_to_zchar__dest
   BVS   .if_end@14
   JSR   get_next_code_byte
.if_end@14:
   RTS
.if_end@13:
   BIT   __local_ascii_to_zchar__dest
   BVC   .if_else@16
   LDA   __local_ascii_to_zchar__dest
   AND   #$3F
   STA   __local_ascii_to_zchar__dest
   LDA   #$00
   STA   __local_ascii_to_zchar__z
   JMP   .if_end@17
.if_else@16:
   LDA   __local_ascii_to_zchar__dest
   AND   #$3F
   STA   __local_ascii_to_zchar__z
   JSR   get_next_code_byte
   STA   __local_ascii_to_zchar__dest
   LDA   __local_ascii_to_zchar__z
   AND   #$20
   BEQ   .if_end@15
   LDA   __local_ascii_to_zchar__z
   ORA   #$C0
   STA   __local_ascii_to_zchar__z
.if_end@15:
.if_end@17:
   LDA   __local_ascii_to_zchar__dest
   ORA   __local_ascii_to_zchar__z
   BNE   .if_end@18
   LDA   #$00
   STA   __local_dec_var__0
   STA   __local_get_next_code_word__1
   JMP   return_value
.if_end@18:
   LDA   __local_ascii_to_zchar__z
   BNE   .if_end@19
   LDA   __local_ascii_to_zchar__dest
   CMP   #$01
   BNE   .vthread@0
   LDA   #$01
   STA   __local_dec_var__0
   LDA   #$00
   STA   __local_get_next_code_word__1
   JMP   return_value
.if_end@19:
   LDA   __local_ascii_to_zchar__dest
.vthread@0:
   SEC
   SBC   #$02
   STA   __local_dec_var__0
   LDA   __local_ascii_to_zchar__z
   SBC   #$00
   STA   __local_get_next_code_word__1
branch_to_offset:
   SUBROUTINE
   LDA   __local_get_next_code_word__1
   BPL   .branch_to_offset@asm_ssa_split@0
   LDY   #$01
   STY   __local_attr_ptr_and_mask__attr
.if_end@10:
   LDA   Z_PC
   CLC
   ADC   __local_dec_var__0
   STA   __local_attr_ptr_and_mask__mask_1
   BCC   .if_end@11
   INC   __local_get_next_code_word__1
   BNE   .if_end@11
   INC   __local_attr_ptr_and_mask__attr
.if_end@11:
.inc_done@0:
   LDA   __local_get_next_code_word__1
   ORA   __local_attr_ptr_and_mask__attr
   BNE   .if_end@12
   LDA   __local_attr_ptr_and_mask__mask_1
   STA   Z_PC
   RTS
.if_end@12:
   LDA   Z_PC+1
   CLC
   ADC   __local_get_next_code_word__1
   STA   __local_get_next_code_word__1
   LDA   Z_PC+2
   ADC   __local_attr_ptr_and_mask__attr
   STA   __local_branch_to_offset__0
   LDA   __local_attr_ptr_and_mask__mask_1
   STA   Z_PC
   LDA   __local_get_next_code_word__1
   STA   Z_PC+1
   LDA   __local_branch_to_offset__0
   AND   #$01
   STA   Z_PC+2
   LDA   #$00
   STA   ZCODE_PAGE_VALID
   RTS
.branch_to_offset@asm_ssa_split@0:
   LDA   #$00
   STA   __local_attr_ptr_and_mask__attr
   JMP   .if_end@10
instr_get_child:
   SUBROUTINE
   LDA   OPERANDS
   JSR   get_object_addr
   LDY   #$06
   LDA   (get_object_addr@ret),Y
   JMP   store_and_branch_on
instr_get_parent:
   SUBROUTINE
   LDA   OPERANDS
   JSR   get_object_addr
   LDY   #$04
   LDA   (get_object_addr@ret),Y
   STA   __local_branch_to_offset__0
   LDA   #$00
   STA   __local_attr_ptr_and_mask__attr
   JMP   store_result
instr_jin:
   SUBROUTINE
   LDA   OPERANDS
   JSR   get_object_addr
   LDY   #$04
   LDA   OPERANDS+2
   CMP   (get_object_addr@ret),Y
   BEQ   .cmp_true@1
   LDA   #$00
   BEQ   .cmp_end@2
.cmp_true@1:
   LDA   #$01
.cmp_end@2:
   JMP   branch
attr_ptr_and_mask:
   SUBROUTINE
   LDA   OPERANDS
   JSR   get_object_addr
   LDA   get_object_addr@ret
   STA   attr_ptr
   LDA   get_object_addr@ret+1
   STA   attr_ptr+1
   LDA   OPERANDS+2
   STA   __local_attr_ptr_and_mask__attr
   CMP   #$10
   BCC   .carry_inc@1
   SBC   #$10
   STA   __local_attr_ptr_and_mask__attr
   LDA   attr_ptr
   CLC
   ADC   #$02
   STA   attr_ptr
   BCC   .carry_inc@1
   INC   attr_ptr+1
.carry_inc@1:
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
   STA   __bss_load
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
   AND   __bss_load
   STA   __local_instr_clear_attr__0
   TXA
   AND   attr_mask+1
   ORA   __local_instr_clear_attr__0
   BEQ   .cmp_end@3
   LDA   #$01
.cmp_end@3:
   JMP   branch
instr_set_attr:
   SUBROUTINE
   JSR   attr_ptr_and_mask
   ORA   __bss_load
   STA   __local_dec_var__0
   TXA
   ORA   attr_mask+1
   STA   __local_get_next_code_word__1
set_attr_word:
   SUBROUTINE
   LDA   __local_dec_var__0
   LDY   #$01
   STA   (attr_ptr),Y
   LDA   __local_get_next_code_word__1
   DEY
   STA   (attr_ptr),Y
   RTS
instr_clear_attr:
   SUBROUTINE
   JSR   attr_ptr_and_mask
   STA   __local_branch_to_offset__0
   LDA   __bss_load
   EOR   #$FF
   STA   __local_instr_clear_attr__1
   LDA   attr_mask+1
   EOR   #$FF
   STA   __local_instr_clear_attr__0
   LDA   __local_branch_to_offset__0
   AND   __local_instr_clear_attr__1
   STA   __local_dec_var__0
   TXA
   AND   __local_instr_clear_attr__0
   STA   __local_get_next_code_word__1
   JMP   set_attr_word
get_property_ptr:
   SUBROUTINE
   LDA   OPERANDS
   JSR   get_object_addr
   LDY   #$07
   LDA   (get_object_addr@ret),Y
   STA   __local_attr_ptr_and_mask__mask_1
   INY
   LDA   (get_object_addr@ret),Y
   CLC
   ADC   Z_HEADER_ADDR
   STA   props
   LDA   Z_HEADER_ADDR+1
   ADC   __local_attr_ptr_and_mask__mask_1
   STA   props+1
   LDY   #$00
   LDA   (props),Y
   SEC
   ROL
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
   RTS
next_property:
   SUBROUTINE
   STA   __local_attr_ptr_and_mask__mask_1
   JSR   get_property_len
   CLC
   ADC   __local_attr_ptr_and_mask__mask_1
   CLC
   ADC   #$02
   RTS
instr_get_prop:
   SUBROUTINE
   LDA   OPERANDS+2
   STA   __local_ascii_to_zchar__1
   JSR   get_property_ptr
   STA   __local_ascii_to_zchar__left
   JMP   .loop@2_start
.instr_get_prop@rot@1:
   BCS   .if_end@6
   LDY   #$0A
   LDA   (Z_HEADER_ADDR),Y
   STA   __local_ascii_to_zchar__z
   INY
   LDA   (Z_HEADER_ADDR),Y
   ADC   Z_HEADER_ADDR
   STA   __local_ascii_to_zchar__left
   LDA   Z_HEADER_ADDR+1
   ADC   __local_ascii_to_zchar__z
   STA   __local_ascii_to_zchar__0
   LDA   __local_ascii_to_zchar__1
   ASL
   STA   __local_ascii_to_zchar__dest
   TAY
   DEY
   DEY
   LDA   (__local_ascii_to_zchar__left),Y
   STA   __local_ascii_to_zchar__z
   LDY   __local_ascii_to_zchar__dest
   DEY
   LDA   (__local_ascii_to_zchar__left),Y
   STA   __local_branch_to_offset__0
   LDA   __local_ascii_to_zchar__z
   STA   __local_attr_ptr_and_mask__attr
   JMP   store_result
.if_end@6:
   LDA   __local_ascii_to_zchar__left
   JSR   next_property
   STA   __local_ascii_to_zchar__left
.loop@2_start:
   JSR   get_property_num
   CMP   __local_ascii_to_zchar__1
   BNE   .instr_get_prop@rot@1
   LDA   __local_ascii_to_zchar__left
   JSR   get_property_len
   STA   __local_ascii_to_zchar__dest
   LDA   __local_ascii_to_zchar__left
   CLC
   ADC   #$01
   STA   __local_ascii_to_zchar__z
   LDA   __local_ascii_to_zchar__dest
   BNE   .if_else@8
   LDY   __local_ascii_to_zchar__z
   LDA   (props),Y
   STA   __local_branch_to_offset__0
   LDA   #$00
   STA   __local_attr_ptr_and_mask__attr
   JMP   store_result
.if_else@8:
   CMP   #$01
   BNE   .if_else@10
   LDY   __local_ascii_to_zchar__z
   LDA   (props),Y
   STA   __local_ascii_to_zchar__left
   LDA   props
   CLC
   ADC   __local_ascii_to_zchar__z
   STA   __local_ascii_to_zchar__z
   LDA   props+1
   ADC   #$00
   STA   __local_ascii_to_zchar__dest
   LDY   #$01
   LDA   (__local_ascii_to_zchar__z),Y
   STA   __local_branch_to_offset__0
   LDA   __local_ascii_to_zchar__left
   STA   __local_attr_ptr_and_mask__attr
   JMP   store_result
.if_else@10:
   JMP   internal_error
instr_get_prop_addr:
   SUBROUTINE
   LDA   OPERANDS+2
   STA   __local_ascii_to_zchar__left
   JSR   get_property_ptr
   STA   __local_ascii_to_zchar__dest
.loop@3_start:
   JSR   get_property_num
   CMP   __local_ascii_to_zchar__left
   BNE   .if_end@11
   LDA   props
   CLC
   ADC   __local_ascii_to_zchar__dest
   STA   __local_ascii_to_zchar__z
   LDA   props+1
   ADC   #$00
   STA   __local_ascii_to_zchar__dest
   INC   __local_ascii_to_zchar__z
   BNE   .inc_done@0
   INC   __local_ascii_to_zchar__dest
.inc_done@0:
   LDA   __local_ascii_to_zchar__z
   SEC
   SBC   Z_HEADER_ADDR
   STA   __local_branch_to_offset__0
   LDA   __local_ascii_to_zchar__dest
   SBC   Z_HEADER_ADDR+1
   STA   __local_attr_ptr_and_mask__attr
   JMP   store_result
.if_end@11:
   BCS   .if_end@12
   LDA   #$00
   STA   __local_branch_to_offset__0
   STA   __local_attr_ptr_and_mask__attr
   JMP   store_result
.if_end@12:
   LDA   __local_ascii_to_zchar__dest
   JSR   next_property
   STA   __local_ascii_to_zchar__dest
   JMP   .loop@3_start
instr_get_next_prop:
   SUBROUTINE
   LDA   OPERANDS+2
   STA   __local_get_next_code_word__1
   JSR   get_property_ptr
   STA   __local_dec_var__0
   LDA   __local_get_next_code_word__1
   BEQ   .if_end@13
.loop@4_start:
   LDA   __local_dec_var__0
   JSR   get_property_num
   CMP   __local_get_next_code_word__1
   BNE   .if_end@14
   LDA   __local_dec_var__0
   JSR   next_property
   STA   __local_dec_var__0
   JMP   .loop@4_break
.if_end@14:
   BCS   .if_end@15
   LDA   #$00
   STA   __local_branch_to_offset__0
   STA   __local_attr_ptr_and_mask__attr
   JMP   store_result
.if_end@15:
   LDA   __local_dec_var__0
   JSR   next_property
   STA   __local_dec_var__0
   JMP   .loop@4_start
.if_end@13:
.loop@4_break:
   LDA   __local_dec_var__0
   JSR   get_property_num
   STA   __local_branch_to_offset__0
   LDA   #$00
   STA   __local_attr_ptr_and_mask__attr
   JMP   store_result
instr_get_prop_len:
   SUBROUTINE
   LDA   Z_HEADER_ADDR
   CLC
   ADC   OPERANDS
   STA   __local_do_2op__1
   LDA   Z_HEADER_ADDR+1
   ADC   OPERANDS+1
   STA   __local_do_2op__0
   LDA   __local_do_2op__1
   SEC
   SBC   #$01
   STA   __local_do_2op__equal
   BCS   .carry_inc@2
   DEC   __local_do_2op__0
.carry_inc@2:
   LDY   #$00
   LDA   (__local_do_2op__equal),Y
   LSR
   LSR
   LSR
   LSR
   LSR
   CLC
   ADC   #$01
   STA   __local_branch_to_offset__0
   LDA   #$00
   STA   __local_attr_ptr_and_mask__attr
   JMP   store_result
instr_put_prop:
   SUBROUTINE
   LDA   OPERANDS+2
   STA   __local_branch_to_offset__0
   JSR   get_property_ptr
   STA   __local_instr_clear_attr__1
   JMP   .loop@5_start
.instr_put_prop@rot@2:
   BCS   .if_end@17
   JMP   internal_error
.if_end@17:
   LDA   __local_instr_clear_attr__1
   JSR   next_property
   STA   __local_instr_clear_attr__1
.loop@5_start:
   JSR   get_property_num
   CMP   __local_branch_to_offset__0
   BNE   .instr_put_prop@rot@2
   LDA   __local_instr_clear_attr__1
   JSR   get_property_len
   STA   __local_attr_ptr_and_mask__attr
   INC   __local_instr_clear_attr__1
   LDA   OPERANDS+4
   STA   __local_branch_to_offset__0
   LDA   __local_attr_ptr_and_mask__attr
   BNE   .if_else@19
   LDA   __local_branch_to_offset__0
   LDY   __local_instr_clear_attr__1
   STA   (props),Y
   RTS
.if_else@19:
   CMP   #$01
   BNE   .if_else@21
   LDA   OPERANDS+5
   LDY   __local_instr_clear_attr__1
   STA   (props),Y
   LDA   props
   CLC
   ADC   __local_instr_clear_attr__1
   STA   __local_instr_clear_attr__0
   LDA   props+1
   ADC   #$00
   STA   __local_instr_clear_attr__1
   LDA   __local_branch_to_offset__0
   LDY   #$01
   STA   (__local_instr_clear_attr__0),Y
   RTS
.if_else@21:
   JMP   internal_error
mulu16:
   SUBROUTINE
   LDA   #$00
   STA   __local_dec_var__0
   LDX   #$00
   BEQ   .loop@0_continue
.mulu16@rot@0:
   LDA   __local_attr_ptr_and_mask__mask_1
   LSR
   BCC   .if_end@0
   LDA   __local_dec_var__0
   CLC
   ADC   __local_branch_to_offset__0
   STA   __local_dec_var__0
   TXA
   ADC   __local_attr_ptr_and_mask__attr
   TAX
.if_end@0:
   ASL   __local_branch_to_offset__0
   ROL   __local_attr_ptr_and_mask__attr
   LSR   __local_attr_ptr_and_mask__n
   ROR   __local_attr_ptr_and_mask__mask_1
.loop@0_continue:
   LDA   __local_attr_ptr_and_mask__mask_1
   ORA   __local_attr_ptr_and_mask__n
   BNE   .mulu16@rot@0
   LDA   __local_dec_var__0
   RTS
divu16:
   SUBROUTINE
   LDX   __local_attr_ptr_and_mask__attr
   LDA   #$00
   STA   __local_ascii_to_zchar__0
   STA   __local_ascii_to_zchar__left
   LDY   #$11
.divu16@rot@1:
   LDA   __local_ascii_to_zchar__0
   SEC
   SBC   __local_attr_ptr_and_mask__mask_1
   STA   __local_ascii_to_zchar__1
   LDA   __local_ascii_to_zchar__left
   SBC   __local_attr_ptr_and_mask__n
   BCC   .divu16@asm_ssa_split@0
   STA   __local_ascii_to_zchar__left
   LDA   __local_ascii_to_zchar__1
   STA   __local_ascii_to_zchar__0
   LDA   #$01
.if_end@1:
   LSR
   ROL   __local_branch_to_offset__0
   TXA
   ROL
   TAX
   ROL   __local_ascii_to_zchar__0
   ROL   __local_ascii_to_zchar__left
   DEY
   BNE   .divu16@rot@1
   LDA   __local_ascii_to_zchar__left
   LSR
   STA   remainder+1
   LDA   __local_ascii_to_zchar__0
   ROR
   STA   remainder
   LDA   __local_branch_to_offset__0
   RTS
.divu16@asm_ssa_split@0:
   LDA   #$00
   BEQ   .if_end@1
check_sign:
   SUBROUTINE
   LDA   __local_get_next_code_word__1
   EOR   __local_instr_clear_attr__1
   STA   sign_bit
   BPL   .check_sign@asm_ssa_split@0
   SEC
   LDA   #$00
   SBC   __local_dec_var__0
   STA   __local_dec_var__0
   LDA   #$00
   SBC   __local_get_next_code_word__1
   TAX
.if_end@2:
   LDA   __local_dec_var__0
   RTS
.check_sign@asm_ssa_split@0:
   LDX   __local_get_next_code_word__1
   JMP   .if_end@2
instr_mul:
   SUBROUTINE
   LDA   OPERANDS+2
   STA   __local_instr_clear_attr__0
   LDA   OPERANDS+3
   STA   __local_instr_clear_attr__1
   LDA   OPERANDS
   STA   __local_dec_var__0
   LDA   OPERANDS+1
   STA   __local_get_next_code_word__1
   JSR   check_sign
   STA   __local_dec_var__0
   STX   __local_get_next_code_word__1
   LDA   __local_instr_clear_attr__1
   BNE   .if_else@7
   LDA   __local_instr_clear_attr__0
   CMP   #$02
   BNE   .vthread@0
.xjump@0:
   ASL   __local_dec_var__0
   ROL   __local_get_next_code_word__1
   JMP   store_with_sign
.vthread@0:
   CMP   #$04
   BNE   .if_else@7
   ASL   __local_dec_var__0
   ROL   __local_get_next_code_word__1
   JMP   .xjump@0
.if_else@7:
   LDA   __local_dec_var__0
   STA   __local_branch_to_offset__0
   STX   __local_attr_ptr_and_mask__attr
   LDA   __local_instr_clear_attr__0
   STA   __local_attr_ptr_and_mask__mask_1
   LDA   __local_instr_clear_attr__1
   STA   __local_attr_ptr_and_mask__n
   JSR   mulu16
   STA   __local_dec_var__0
   STX   __local_get_next_code_word__1
store_with_sign:
   SUBROUTINE
   LDA   sign_bit
   BPL   .store_with_sign@asm_ssa_split@0
   SEC
   LDA   #$00
   SBC   __local_dec_var__0
   STA   __local_branch_to_offset__0
   LDA   #$00
   SBC   __local_get_next_code_word__1
.if_end@3:
   STA   __local_attr_ptr_and_mask__attr
   JMP   store_result
.store_with_sign@asm_ssa_split@0:
   LDA   __local_dec_var__0
   STA   __local_branch_to_offset__0
   LDA   __local_get_next_code_word__1
   JMP   .if_end@3
instr_div:
   SUBROUTINE
   LDA   OPERANDS+2
   STA   __local_instr_clear_attr__0
   LDA   OPERANDS+3
   STA   __local_instr_clear_attr__1
   LDA   OPERANDS
   STA   __local_dec_var__0
   LDA   OPERANDS+1
   STA   __local_get_next_code_word__1
   JSR   check_sign
   STA   __local_dec_var__0
   STX   __local_get_next_code_word__1
   LDA   __local_instr_clear_attr__1
   BNE   .if_else@11
   LDA   __local_instr_clear_attr__0
   CMP   #$02
   BNE   .vthread@1
.xjump@1:
   LSR   __local_get_next_code_word__1
   ROR   __local_dec_var__0
   JMP   store_with_sign
.vthread@1:
   CMP   #$04
   BNE   .if_else@11
   LSR   __local_get_next_code_word__1
   ROR   __local_dec_var__0
   JMP   .xjump@1
.if_else@11:
   LDA   __local_dec_var__0
   STA   __local_branch_to_offset__0
   STX   __local_attr_ptr_and_mask__attr
   LDA   __local_instr_clear_attr__0
   STA   __local_attr_ptr_and_mask__mask_1
   LDA   __local_instr_clear_attr__1
   STA   __local_attr_ptr_and_mask__n
   JSR   divu16
   STA   __local_dec_var__0
   STX   __local_get_next_code_word__1
.if_end@8:
   JMP   store_with_sign
instr_mod:
   SUBROUTINE
   LDA   OPERANDS+2
   STA   __local_instr_clear_attr__0
   LDA   OPERANDS+3
   STA   __local_instr_clear_attr__1
   LDA   OPERANDS
   STA   __local_dec_var__0
   LDA   OPERANDS+1
   STA   __local_get_next_code_word__1
   JSR   check_sign
   STA   __local_branch_to_offset__0
   STX   __local_attr_ptr_and_mask__attr
   LDA   __local_instr_clear_attr__0
   STA   __local_attr_ptr_and_mask__mask_1
   LDA   __local_instr_clear_attr__1
   STA   __local_attr_ptr_and_mask__n
   JSR   divu16
   LDA   remainder
   STA   __local_branch_to_offset__0
   LDA   remainder+1
   STA   __local_attr_ptr_and_mask__attr
   JMP   store_result
instr_random:
   SUBROUTINE
   LDA   RND+1
   ASL
   STA   RND+1
   LDA   RND
   STA   __local_branch_to_offset__0
   LDA   RND+1
   STA   __local_attr_ptr_and_mask__attr
   LDA   OPERANDS
   STA   __local_attr_ptr_and_mask__mask_1
   LDA   OPERANDS+1
   STA   __local_attr_ptr_and_mask__n
   JSR   divu16
   LDA   remainder
   CLC
   ADC   #$01
   STA   __local_branch_to_offset__0
   LDA   remainder+1
   ADC   #$00
   STA   __local_attr_ptr_and_mask__attr
   JMP   store_result
print_number:
   SUBROUTINE
   LDA   __local_get_next_code_word__1
   BPL   .if_end@12
   LDA   #$2D
   JSR   buffer_char
   SEC
   LDA   #$00
   SBC   __local_dec_var__0
   STA   __local_dec_var__0
   LDA   #$00
   SBC   __local_get_next_code_word__1
   STA   __local_get_next_code_word__1
.if_end@12:
   LDA   #$00
   STA   __local_get_next_code_byte__page
.loop@2_start:
   LDA   __local_dec_var__0
   STA   __local_branch_to_offset__0
   LDA   __local_get_next_code_word__1
   STA   __local_attr_ptr_and_mask__attr
   LDA   #$0A
   STA   __local_attr_ptr_and_mask__mask_1
   LDA   #$00
   STA   __local_attr_ptr_and_mask__n
   JSR   divu16
   STA   __local_dec_var__0
   STX   __local_get_next_code_word__1
   LDA   remainder
   LDY   __local_get_next_code_byte__page
   STA   __local_get_nonstack_var__0,Y
   INC   __local_get_next_code_byte__page
   LDA   __local_dec_var__0
   ORA   __local_get_next_code_word__1
   BNE   .loop@2_start
.loop@3_start:
   DEC   __local_get_next_code_byte__page
   LDY   __local_get_next_code_byte__page
   LDA   __local_get_nonstack_var__0,Y
   CLC
   ADC   #$30
   JSR   buffer_char
   LDA   __local_get_next_code_byte__page
   BNE   .loop@3_start
   RTS

; Disassembly of section .text.rest at $2C00
   SEG   text_rest
   ORG   $2C00
locate_last_ram_page:
   SUBROUTINE
   LDA   #$C0
   STA   __local_branch_to_offset__0
.loop@0_start:
   DEC   __local_branch_to_offset__0
   LDA   #$FF
   STA   __local_instr_clear_attr__0
   LDA   __local_branch_to_offset__0
   STA   __local_instr_clear_attr__1
   LDY   #$00
   LDA   (__local_instr_clear_attr__0),Y
   CMP   (__local_instr_clear_attr__0),Y
   BNE   .loop@0_start
   EOR   #$FF
   STA   (__local_instr_clear_attr__0),Y
   CMP   (__local_instr_clear_attr__0),Y
   BNE   .loop@0_start
   EOR   #$FF
   STA   (__local_instr_clear_attr__0),Y
   LDA   __local_branch_to_offset__0
   RTS
instr_restart:
   SUBROUTINE
   JSR   dump_buffer_with_more
   JMP   main
instr_quit:
   SUBROUTINE
   JSR   dump_buffer_with_more
   LDA   #$C9
   STA   __local_get_next_code_byte__page
   LDA   #$33
   STA   __local_get_nonstack_var__0
   LDX   #$14
   JSR   print_ascii_string
   JSR   dump_buffer_with_more
.loop@5_start:
   JMP   .loop@5_start
internal_error:
   SUBROUTINE
   JSR   dump_buffer_with_more
   LDA   #$DD
   STA   __local_get_next_code_byte__page
   LDA   #$33
   STA   __local_get_nonstack_var__0
   LDX   #$14
   JSR   print_ascii_string
   JSR   dump_buffer_with_more
.loop@6_start:
   JMP   .loop@6_start
print_status_line:
   SUBROUTINE
   JSR   dump_buffer_line
   LDA   CH
   STA   __local_ascii_to_zchar__dest
   LDA   CV
   STA   __local_ascii_to_zchar__z
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
   LDA   #$F1
   STA   __local_get_nonstack_var__1
   LDA   #$33
   STA   __local_get_nonstack_var__2
   LDX   #$06
   JSR   cout_string
   LDA   CH
   CLC
   ADC   #$01
   STA   CH
   LDA   #$11
   JSR   var_get
   STA   __local_dec_var__0
   STX   __local_get_next_code_word__1
   JSR   print_number
   LDA   #$2F
   JSR   buffer_char
   LDA   #$12
   JSR   var_get
   STA   __local_dec_var__0
   STX   __local_get_next_code_word__1
   JSR   print_number
   JSR   dump_buffer_to_screen
   LDA   #$FF
   STA   INVFLG
   LDA   __local_ascii_to_zchar__z
   STA   CV
   LDA   __local_ascii_to_zchar__dest
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
   STX   __local_attr_ptr_and_mask__mask_1
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
   LDA   __local_attr_ptr_and_mask__mask_1
   CMP   (text),Y
   BCC   .if_end@1
   LDA   (text),Y
   STA   __local_attr_ptr_and_mask__mask_1
.if_end@1:
   ORA   #$00
   BEQ   .if_end@2
   LDX   #$00
   STA   __local_attr_ptr_and_mask__attr
.loop@1_start:
   LDA   BUFF_AREA,X
   AND   #$7F
   STA   __local_branch_to_offset__0
   CMP   #$41
   BCC   .if_end@5
   CMP   #$5B
   BCS   .if_end@5
   ORA   #$20
   STA   __local_branch_to_offset__0
.if_end@5:
   INX
   TXA
   TAY
   LDA   __local_branch_to_offset__0
   STA   (text),Y
   CMP   #$0D
   BEQ   .if_end@2
   DEC   __local_attr_ptr_and_mask__attr
   BNE   .loop@1_start
.if_end@2:
.loop@1_break:
   LDA   __local_attr_ptr_and_mask__mask_1
   RTS
get_dictionary_addr:
   SUBROUTINE
   LDY   #$08
   LDA   (Z_HEADER_ADDR),Y
   STA   __local_dec_var__0
   INY
   LDA   (Z_HEADER_ADDR),Y
   CLC
   ADC   Z_HEADER_ADDR
   STA   get_dictionary_addr@ret
   LDA   Z_HEADER_ADDR+1
   ADC   __local_dec_var__0
   STA   get_dictionary_addr@ret+1
   RTS
is_dict_separator:
   SUBROUTINE
   JSR   get_dictionary_addr
   LDY   #$00
   LDA   (get_dictionary_addr@ret),Y
   TAX
   LDA   __local_instr_clear_attr__0
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
is_separator:
   SUBROUTINE
   JSR   is_dict_separator
   BCC   is_std_separator
   RTS
is_std_separator:
   SUBROUTINE
   LDX   #$00
.is_std_separator@rot@1:
   LDA   __local_instr_clear_attr__0
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
   STA   __local_instr_clear_attr__0
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
   LDA   __local_dec_var__0
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
   AND   #$80
   BEQ   .if_end@21
.get_alphabet_for_char@scfold@0:
   LDA   __local_dec_var__0
   RTS
.if_end@21:
   LDA   #$02
   RTS
search_nonalpha_table:
   SUBROUTINE
   LDX   #$17
.loop@5_start:
   LDA   __local_dec_var__0
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
   STA   __local_get_next_code_word__1
   TXA
   BNE   .search_nonalpha_table@ssa_split@1
   LDA   #$00
   RTS
.search_nonalpha_table@ssa_split@1:
   LDX   __local_get_next_code_word__1
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
   JMP   .if_end@33
.if_else@24:
   STA   __local_dec_var__0
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
   STA   __local_dec_var__0
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
   BEQ   z_compress
.if_end@25:
   LDA   __local_ascii_to_zchar__2
   STA   __local_dec_var__0
   JSR   get_alphabet_for_char
   SEC
   SBC   #$01
   STA   __local_ascii_to_zchar__z
   AND   #$80
   BEQ   .if_else@30
   LDA   __local_ascii_to_zchar__2
   SEC
   SBC   #$5B
   JMP   .if_end@33
.if_else@30:
   LDA   __local_ascii_to_zchar__z
   BNE   .if_else@32
   LDA   __local_ascii_to_zchar__2
   SEC
   SBC   #$3B
   JMP   .if_end@33
.if_else@32:
   LDA   __local_ascii_to_zchar__2
   STA   __local_dec_var__0
   JSR   search_nonalpha_table
   ORA   #$00
   BNE   .if_end@23
   LDX   __local_ascii_to_zchar__dest
   LDA   #$06
   STA   ZCHAR_SCRATCH2,X
   INX
   DEC   __local_ascii_to_zchar__left
   BEQ   z_compress
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
   BEQ   z_compress
   LDA   __local_ascii_to_zchar__2
   AND   #$1F
.if_end@23:
.if_end@29:
.if_end@31:
.if_end@33:
   LDX   __local_ascii_to_zchar__dest
   STA   ZCHAR_SCRATCH2,X
   INC   __local_ascii_to_zchar__dest
   DEC   __local_ascii_to_zchar__left
   BEQ   z_compress
   JMP   .loop@7_start
z_compress:
   SUBROUTINE
   LDA   ZCHAR_SCRATCH2
   ASL
   ASL
   STA   __local_attr_ptr_and_mask__n
   LDA   ZCHAR_SCRATCH2+1
   LSR
   LSR
   LSR
   STA   __local_branch_to_offset__0
   LDA   ZCHAR_SCRATCH2+1
   ASL
   ASL
   ASL
   ASL
   ASL
   STA   __local_attr_ptr_and_mask__mask_1
   LDA   __local_attr_ptr_and_mask__n
   ORA   __local_branch_to_offset__0
   STA   __local_attr_ptr_and_mask__attr
   LDA   __local_attr_ptr_and_mask__mask_1
   ORA   ZCHAR_SCRATCH2+2
   STA   ZCHAR_SCRATCH2
   LDA   __local_attr_ptr_and_mask__attr
   STA   ZCHAR_SCRATCH2+1
   LDA   ZCHAR_SCRATCH2+3
   ASL
   ASL
   STA   __local_attr_ptr_and_mask__mask_1
   LDA   ZCHAR_SCRATCH2+4
   LSR
   LSR
   LSR
   STA   __local_branch_to_offset__0
   LDA   ZCHAR_SCRATCH2+4
   ASL
   ASL
   ASL
   ASL
   ASL
   STA   __local_attr_ptr_and_mask__attr
   LDA   __local_attr_ptr_and_mask__mask_1
   ORA   __local_branch_to_offset__0
   STA   __local_attr_ptr_and_mask__mask_1
   LDA   __local_attr_ptr_and_mask__attr
   ORA   ZCHAR_SCRATCH2+5
   STA   ZCHAR_SCRATCH2+2
   LDA   __local_attr_ptr_and_mask__mask_1
   ORA   #$80
   STA   ZCHAR_SCRATCH2+3
   RTS
match_dictionary_word:
   SUBROUTINE
   JSR   get_dictionary_addr
   LDA   get_dictionary_addr@ret+1
   STA   __local_get_object_addr__2
   LDY   #$00
   LDA   (get_dictionary_addr@ret),Y
   TAY
   INY
   LDA   (get_dictionary_addr@ret),Y
   STA   __local_ascii_to_zchar__2
   ASL
   ASL
   ASL
   ASL
   STA   __local_ascii_to_zchar__0
   INY
   LDA   (get_dictionary_addr@ret),Y
   INY
   STA   __local_ascii_to_zchar__1
   LDA   (get_dictionary_addr@ret),Y
   STA   __local_ascii_to_zchar__left
   INY
   TYA
   CLC
   ADC   get_dictionary_addr@ret
   STA   __local_ascii_to_zchar__index
   BCC   .carry_inc@0
   INC   __local_get_object_addr__2
.carry_inc@0:
.loop@8_start:
   LDA   __local_ascii_to_zchar__index
   CLC
   ADC   __local_ascii_to_zchar__0
   STA   __local_ascii_to_zchar__index
   BCC   .carry_inc@1
   INC   __local_get_object_addr__2
.carry_inc@1:
   LDA   __local_ascii_to_zchar__left
   SEC
   SBC   #$10
   STA   __local_ascii_to_zchar__left
   BCS   .carry_inc@2
   DEC   __local_ascii_to_zchar__1
.carry_inc@2:
   LDA   __local_ascii_to_zchar__1
   BMI   .loop@8_break
   LDY   #$00
   LDA   (__local_ascii_to_zchar__index),Y
   CMP   ZCHAR_SCRATCH2+1
   BCC   .carry_inc@0
.loop@8_break:
   LDA   __local_ascii_to_zchar__index
   SEC
   SBC   __local_ascii_to_zchar__0
   STA   __local_ascii_to_zchar__index
   BCS   .carry_inc@3
   DEC   __local_get_object_addr__2
.carry_inc@3:
   LDA   __local_ascii_to_zchar__left
   CLC
   ADC   #$10
   STA   __local_ascii_to_zchar__dest
   BCC   .carry_inc@4
   INC   __local_ascii_to_zchar__1
.carry_inc@4:
.loop@9_start:
   LDY   #$00
   LDA   ZCHAR_SCRATCH2+1
   CMP   (__local_ascii_to_zchar__index),Y
   BCC   .tail@0
   BNE   .if_end@41
   INY
   LDA   ZCHAR_SCRATCH2
   CMP   (__local_ascii_to_zchar__index),Y
   BCC   .tail@0
   BNE   .if_end@41
   INY
   LDA   ZCHAR_SCRATCH2+3
   CMP   (__local_ascii_to_zchar__index),Y
   BCC   .tail@0
   BNE   .if_end@41
   INY
   LDA   ZCHAR_SCRATCH2+2
   CMP   (__local_ascii_to_zchar__index),Y
   BCC   .tail@0
   BNE   .if_end@41
   LDA   __local_ascii_to_zchar__index
   SBC   Z_HEADER_ADDR
   STA   __local_ascii_to_zchar__z
   LDA   __local_get_object_addr__2
   SBC   Z_HEADER_ADDR+1
   TAX
   LDA   __local_ascii_to_zchar__z
   RTS
.if_end@41:
.if_end@43:
   LDA   __local_ascii_to_zchar__index
   CLC
   ADC   __local_ascii_to_zchar__2
   STA   __local_ascii_to_zchar__index
   BCC   .carry_inc@5
   INC   __local_get_object_addr__2
.carry_inc@5:
   LDA   __local_ascii_to_zchar__dest
   SEC
   SBC   #$01
   STA   __local_ascii_to_zchar__dest
   BCS   .carry_inc@6
   DEC   __local_ascii_to_zchar__1
.carry_inc@6:
   ORA   __local_ascii_to_zchar__1
   BNE   .carry_inc@4
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
   LDY   input_ptr
   LDA   (text),Y
   STA   __local_instr_clear_attr__0
   JSR   is_dict_separator
   BCC   .if_end@51
   LDA   __local_instr_clear_attr__0
   STA   ZCHAR_SCRATCH1
   INC   char_count
   DEC   input_count
   INC   input_ptr
   JMP   .if_end@55
.if_end@51:
   LDY   input_ptr
   LDA   (text),Y
   STA   __local_instr_clear_attr__0
   JSR   is_std_separator
   BCC   .if_end@50
   INC   input_ptr
   DEC   input_count
   JMP   .loop@10_start
.if_end@50:
.if_end@52:
   LDA   input_count
   BEQ   .if_end@55
   LDY   input_ptr
   LDA   (text),Y
   STA   __local_instr_clear_attr__0
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
   STA   __local_instr_clear_attr__1
   TXA
   LDY   token_idx
   STA   (parse),Y
   INY
   LDA   __local_instr_clear_attr__1
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
   LDA   #$00
   STA   disk_buffer
   LDA   #$02
   STA   disk_buffer+1
inc_sector_and_read:
   SUBROUTINE
   INC   disk_sector
   BNE   read_from_sector
   INC   disk_sector+1
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
   LDA   #$34
   LDY   #$AB
   JMP   RWTS
write_next_sector:
   SUBROUTINE
   LDA   #$00
   STA   disk_buffer
   LDA   #$02
   STA   disk_buffer+1
inc_sector_and_write:
   SUBROUTINE
   INC   disk_sector
   BNE   write_to_sector
   INC   disk_sector+1
write_to_sector:
   SUBROUTINE
   LDA   dct+2
   STA   __local_get_next_code_word__1
   LDA   dct+3
   STA   __local_dec_var__0
   LDA   #$EF
   STA   dct+2
   LDA   #$D8
   STA   dct+3
   LDA   #$02
   JSR   do_rwts_on_sector
   LDA   __local_get_next_code_word__1
   STA   dct+2
   LDA   __local_dec_var__0
   STA   dct+3
   RTS
get_prompted_number_from_user:
   SUBROUTINE
   STA   __local_branch_to_offset__0
   JSR   dump_buffer_with_more
   LDA   __local_branch_to_offset__0
   ASL
   ASL
   ASL
   ASL
   STA   __local_attr_ptr_and_mask__attr
   LDA   #$C0
   CLC
   ADC   __local_attr_ptr_and_mask__attr
   STA   __local_get_next_code_byte__page
   LDA   #$34
   ADC   #$00
   STA   __local_get_nonstack_var__0
   LDX   #$0F
   JSR   print_ascii_string
   JSR   dump_buffer_line
   LDA   #$19
   STA   CH
   LDA   #$3F
   STA   INVFLG
   LDA   #$39
   STA   __local_get_nonstack_var__1
   LDA   #$34
   STA   __local_get_nonstack_var__2
   LDX   #$0A
   JSR   cout_string
   CLC
   LDA   #$CF
   ADC   __local_attr_ptr_and_mask__attr
   STA   __local_get_nonstack_var__1
   LDA   #$34
   ADC   #$00
   STA   __local_get_nonstack_var__2
   LDX   #$01
   JSR   cout_string
   LDA   #$FF
   STA   INVFLG
   JSR   RDKEY
   STA   __local_branch_to_offset__0
   LDA   #$19
   STA   CH
   JSR   CLREOL
   LDA   __local_branch_to_offset__0
   CMP   #$8D
   BNE   .if_end@0
   LDX   __local_attr_ptr_and_mask__attr
   LDA   prompts+15,X
.if_end@0:
   AND   #$7F
   RTS
please_insert_save_diskette:
   SUBROUTINE
   JSR   home
   JSR   dump_buffer_with_more
   JSR   dump_buffer_with_more
   LDA   #$1D
   STA   __local_get_next_code_byte__page
   LDA   #$34
   STA   __local_get_nonstack_var__0
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
   STA   __local_get_next_code_word__1
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
   LDA   __local_get_next_code_word__1
   STA   prompts+15
   JSR   buffer_char
.loop@2_start:
   LDA   #$01
   JSR   get_prompted_number_from_user
   STA   __local_dec_var__0
   CMP   #$31
   BCC   .loop@2_start
   CMP   #$33
   BCS   .loop@2_start
   AND   #$03
   STA   iob+2
   LDA   __local_dec_var__0
   STA   prompts+31
   JSR   buffer_char
.loop@3_start:
   JSR   dump_buffer_with_more
   LDA   #$43
   STA   __local_get_next_code_byte__page
   LDA   #$34
   STA   __local_get_nonstack_var__0
   LDX   #$23
   JSR   print_ascii_string
   JSR   dump_buffer_line
   JSR   RDKEY
   CMP   #$8D
   BNE   .loop@3_start
   LDA   prompts+47
   AND   #$07
   STA   __local_dec_var__0
   LSR
   LSR
   STA   __local_get_next_code_word__1
   LDA   __local_dec_var__0
   ROR
   ROR
   ROR
   AND   #$C0
   SEC
   SBC   #$01
   STA   __local_dec_var__0
   BCS   .carry_inc@0
   DEC   __local_get_next_code_word__1
.carry_inc@0:
   JSR   dump_buffer_with_more
   LDX   __local_get_next_code_word__1
   LDA   __local_dec_var__0
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
   LDA   #$66
   STA   __local_get_next_code_byte__page
   LDA   #$34
   STA   __local_get_nonstack_var__0
   LDX   #$1F
   JSR   print_ascii_string
.loop@4_start:
   JSR   dump_buffer_with_more
   LDA   #$85
   STA   __local_get_next_code_byte__page
   LDA   #$34
   STA   __local_get_nonstack_var__0
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
   LDA   (__local_dec_var__0),Y
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
   STA   (__local_dec_var__0),Y
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
   LDA   #$8A
   STA   __local_dec_var__0
   LDA   #$00
   STA   __local_get_next_code_word__1
   LDX   #$01
   LDY   #$03
   JSR   copy_data_to_buff
   TAX
   LDA   #$9A
   STA   __local_dec_var__0
   LDA   #$00
   STA   __local_get_next_code_word__1
   LDY   #$1E
   JSR   copy_data_to_buff
   TAX
   LDA   #$C8
   STA   __local_dec_var__0
   LDA   #$00
   STA   __local_get_next_code_word__1
   LDY   #$06
   JSR   copy_data_to_buff
   JSR   write_next_sector
   BCS   .instr_save@fail
   LDA   #$80
   STA   __local_dec_var__0
   LDA   #$02
   STA   __local_get_next_code_word__1
   LDX   #$00
   LDY   #$00
   JSR   copy_data_to_buff
   JSR   write_next_sector
   BCS   .instr_save@fail
   LDA   #$80
   STA   __local_dec_var__0
   LDA   #$03
   STA   __local_get_next_code_word__1
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
   STA   __local_instr_clear_attr__0
.loop@2_start:
   JSR   inc_sector_and_write
   BCS   .instr_save@fail
   INC   disk_buffer+1
   DEC   __local_instr_clear_attr__0
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
   STA   __local_instr_clear_attr__1
   LDA   #$8A
   STA   __local_dec_var__0
   LDA   #$00
   STA   __local_get_next_code_word__1
   LDX   #$01
   LDY   #$03
   JSR   copy_data_from_buff
   TAX
   LDA   #$00
   STA   ZCODE_PAGE_VALID
   LDA   #$9A
   STA   __local_dec_var__0
   LDA   #$00
   STA   __local_get_next_code_word__1
   LDY   #$1E
   JSR   copy_data_from_buff
   TAX
   LDA   #$C8
   STA   __local_dec_var__0
   LDA   #$00
   STA   __local_get_next_code_word__1
   LDY   #$06
   JSR   copy_data_from_buff
   JSR   read_next_sector
   BCS   .instr_restore@fail
   LDA   #$80
   STA   __local_dec_var__0
   LDA   #$02
   STA   __local_get_next_code_word__1
   LDX   #$00
   LDY   #$00
   JSR   copy_data_from_buff
   JSR   read_next_sector
   BCS   .instr_restore@fail
   LDA   #$80
   STA   __local_dec_var__0
   LDA   #$03
   STA   __local_get_next_code_word__1
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
   STA   __local_instr_clear_attr__0
.loop@3_start:
   JSR   inc_sector_and_read
   BCS   .instr_restore@fail
   INC   disk_buffer+1
   DEC   __local_instr_clear_attr__0
   BNE   .loop@3_start
   LDA   __local_instr_clear_attr__1
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

; Disassembly of section .rodata at $33C9
   SEG   rodata
   ORG   $33C9
sEndOfSession:
   DC.B  $2D, $2D, $20, $45, $4E, $44, $20, $4F
   DC.B  $46, $20, $53, $45, $53, $53, $49, $4F
   DC.B  $4E, $20, $2D, $2D
sInternalError:
   DC.B  $5A, $4F, $52, $4B, $20, $49, $4E, $54
   DC.B  $45, $52, $4E, $41, $4C, $20, $45, $52
   DC.B  $52, $4F, $52, $21
sScore:
   DC.B  $53, $43, $4F, $52, $45, $3A
SEPARATORS_TABLE:
   DC.B  $20, $2E, $2C, $3F, $0D, $0A, $09, $0C
a2_table:
   DC.B  $30, $31, $32, $33, $34, $35, $36, $37
   DC.B  $38, $39, $2E, $2C, $21, $3F, $5F, $23
   DC.B  $27, $22, $2F, $5C, $2D, $3A, $28, $29
string_more:
   DC.B  $5B, $4D, $4F, $52, $45, $5D
sPleaseInsert:
   DC.B  $50, $4C, $45, $41, $53, $45, $20, $49
   DC.B  $4E, $53, $45, $52, $54, $20, $53, $41
   DC.B  $56, $45, $20, $44, $49, $53, $4B, $45
   DC.B  $54, $54, $45, $2C
sDefault:
   DC.B  $44, $45, $46, $41, $55, $4C, $54, $20
   DC.B  $3D, $20
sReturnToBegin:
   DC.B  $2D, $2D, $2D, $20, $50, $52, $45, $53
   DC.B  $53, $20, $27, $52, $45, $54, $55, $52
   DC.B  $4E, $27, $20, $4B, $45, $59, $20, $54
   DC.B  $4F, $20, $42, $45, $47, $49, $4E, $20
   DC.B  $2D, $2D, $2D
sReinsertGameDiskette:
   DC.B  $50, $4C, $45, $41, $53, $45, $20, $52
   DC.B  $45, $2D, $49, $4E, $53, $45, $52, $54
   DC.B  $20, $47, $41, $4D, $45, $20, $44, $49
   DC.B  $53, $4B, $45, $54, $54, $45, $2C
sPressReturnToContinue:
   DC.B  $2D, $2D, $2D, $20, $50, $52, $45, $53
   DC.B  $53, $20, $27, $52, $45, $54, $55, $52
   DC.B  $4E, $27, $20, $4B, $45, $59, $20, $54
   DC.B  $4F, $20, $43, $4F, $4E, $54, $49, $4E
   DC.B  $55, $45, $20, $2D, $2D, $2D

; Disassembly of section .data at $34AB
   SEG   data
   ORG   $34AB
iob:
   DC.B  $01, $60, $01, $00, $00, $00, $BC, $34
   DC.B  $00, $00, $00, $00, $00, $00, $00, $60
   DC.B  $01
dct:
   DC.B  $00, $01, $EF, $D8
prompts:
   DC.B  $53, $4C, $4F, $54, $20, $20, $20, $20
   DC.B  $20, $28, $31, $2D, $37, $29, $3A, $36
   DC.B  $44, $52, $49, $56, $45, $20, $20, $20
   DC.B  $20, $28, $31, $2D, $32, $29, $3A, $32
   DC.B  $50, $4F, $53, $49, $54, $49, $4F, $4E
   DC.B  $20, $28, $30, $2D, $37, $29, $3A, $30

; Disassembly of section .bss at $34F0
   SEG   bss
   ORG   $34F0
attr_mask:
   DC.B  $00, $00
printer_card_initialized_flag:
   DC.B  $00

; Disassembly of section .tables at $3500
   SEG   tables
   ORG   $3500
page_tables:
   DC.B  $00, $00, $00, $00, $00, $00, $00, $00
   DC.B  $00, $00, $00, $00, $00, $00, $00, $00
   DC.B  $00, $00, $00, $00, $00, $00, $00, $00
   DC.B  $00, $00, $00, $00, $00, $00, $00, $00
   DC.B  $00, $00, $00, $00, $00, $00, $00, $00
   DC.B  $00, $00, $00, $00, $00, $00, $00, $00
   DC.B  $00, $00, $00, $00, $00, $00, $00, $00
   DC.B  $00, $00, $00, $00, $00, $00, $00, $00
   DC.B  $00, $00, $00, $00, $00, $00, $00, $00
   DC.B  $00, $00, $00, $00, $00, $00, $00, $00
   DC.B  $00, $00, $00, $00, $00, $00, $00, $00
   DC.B  $00, $00, $00, $00, $00, $00, $00, $00
   DC.B  $00, $00, $00, $00, $00, $00, $00, $00
   DC.B  $00, $00, $00, $00, $00, $00, $00, $00
   DC.B  $00, $00, $00, $00, $00, $00, $00, $00
   DC.B  $00, $00, $00, $00, $00, $00, $00, $00
   DC.B  $00, $00, $00, $00, $00, $00, $00, $00
   DC.B  $00, $00, $00, $00, $00, $00, $00, $00
   DC.B  $00, $00, $00, $00, $00, $00, $00, $00
   DC.B  $00, $00, $00, $00, $00, $00, $00, $00
   DC.B  $00, $00, $00, $00, $00, $00, $00, $00
   DC.B  $00, $00, $00, $00, $00, $00, $00, $00
   DC.B  $00, $00, $00, $00, $00, $00, $00, $00
   DC.B  $00, $00, $00, $00, $00, $00, $00, $00
   DC.B  $00, $00, $00, $00, $00, $00, $00, $00
   DC.B  $00, $00, $00, $00, $00, $00, $00, $00
   DC.B  $00, $00, $00, $00, $00, $00, $00, $00
   DC.B  $00, $00, $00, $00, $00, $00, $00, $00
   DC.B  $00, $00, $00, $00, $00, $00, $00, $00
   DC.B  $00, $00, $00, $00, $00, $00, $00, $00
   DC.B  $00, $00, $00, $00, $00, $00, $00, $00
   DC.B  $00, $00, $00, $00, $00, $00, $00, $00
   DC.B  $00, $00, $00, $00, $00, $00, $00, $00
   DC.B  $00, $00, $00, $00, $00, $00, $00, $00
   DC.B  $00, $00, $00, $00, $00, $00, $00, $00
   DC.B  $00, $00, $00, $00, $00, $00, $00, $00
   DC.B  $00, $00, $00, $00, $00, $00, $00, $00
   DC.B  $00, $00, $00, $00, $00, $00, $00, $00
   DC.B  $00, $00, $00, $00, $00, $00, $00, $00
   DC.B  $00, $00, $00, $00, $00, $00, $00, $00
   DC.B  $00, $00, $00, $00, $00, $00, $00, $00
   DC.B  $00, $00, $00, $00, $00, $00, $00, $00
   DC.B  $00, $00, $00, $00, $00, $00, $00, $00
   DC.B  $00, $00, $00, $00, $00, $00, $00, $00
   DC.B  $00, $00, $00, $00, $00, $00, $00, $00
   DC.B  $00, $00, $00, $00, $00, $00, $00, $00
   DC.B  $00, $00, $00, $00, $00, $00, $00, $00
   DC.B  $00, $00, $00, $00, $00, $00, $00, $00
   DC.B  $00, $00, $00, $00, $00, $00, $00, $00
   DC.B  $00, $00, $00, $00, $00, $00, $00, $00
   DC.B  $00, $00, $00, $00, $00, $00, $00, $00
   DC.B  $00, $00, $00, $00, $00, $00, $00, $00
   DC.B  $00, $00, $00, $00, $00, $00, $00, $00
   DC.B  $00, $00, $00, $00, $00, $00, $00, $00
   DC.B  $00, $00, $00, $00, $00, $00, $00, $00
   DC.B  $00, $00, $00, $00, $00, $00, $00, $00
   DC.B  $00, $00, $00, $00, $00, $00, $00, $00
   DC.B  $00, $00, $00, $00, $00, $00, $00, $00
   DC.B  $00, $00, $00, $00, $00, $00, $00, $00
   DC.B  $00, $00, $00, $00, $00, $00, $00, $00
   DC.B  $00, $00, $00, $00, $00, $00, $00, $00
   DC.B  $00, $00, $00, $00, $00, $00, $00, $00
   DC.B  $00, $00, $00, $00, $00, $00, $00, $00
   DC.B  $00, $00, $00, $00, $00, $00, $00, $00
