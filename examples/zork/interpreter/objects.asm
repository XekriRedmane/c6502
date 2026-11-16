HARGS	EQU	$00
SSP	EQU	$05
FP	EQU	$07
DPTR	EQU	$09
props	EQU	$0B
attr_ptr	EQU	$0D
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
__zpabi_cout_string__s_0	EQU	$E0
__zpabi_load_address__addr_0	EQU	$E0
__zpabi_load_packed_address__addr_0	EQU	$E0
__zpabi_print_ascii_string__s_0	EQU	$E0
__zpabi_print_number__n_0	EQU	$E0
__zpabi_push__value_0	EQU	$E0
__zpabi_return_value__value_0	EQU	$E0
__zpabi_set_attr_word__word_0	EQU	$E0
__zpabi_store_result__value_0	EQU	$E0
__zpabi_var_put__value_0	EQU	$E0
__local_instr_test_attr__1	EQU	$E1
__local_next_property__y	EQU	$E1
__zpabi_cout_string__s_1	EQU	$E1
__zpabi_load_address__addr_1	EQU	$E1
__zpabi_load_packed_address__addr_1	EQU	$E1
__zpabi_print_ascii_string__s_1	EQU	$E1
__zpabi_print_number__n_1	EQU	$E1
__zpabi_push__value_1	EQU	$E1
__zpabi_return_value__value_1	EQU	$E1
__zpabi_set_attr_word__word_1	EQU	$E1
__zpabi_store_result__value_1	EQU	$E1
__zpabi_var_put__value_1	EQU	$E1
Z_ABBREV_TABLE	EQU	$E2
__local_instr_clear_attr__0	EQU	$E4
__local_instr_get_child__0	EQU	$E4
__local_instr_get_next_prop__y	EQU	$E4
__local_instr_get_parent__0	EQU	$E4
__local_instr_get_prop__0	EQU	$E4
__local_instr_get_prop_addr__0	EQU	$E4
__local_instr_get_prop_len__0	EQU	$E4
__local_instr_get_sibling__0	EQU	$E4
__local_instr_insert_obj__0	EQU	$E4
__local_instr_put_prop__0	EQU	$E4
__local_print_obj__obj	EQU	$E4
__local_instr_clear_attr__1	EQU	$E5
__local_instr_get_child__1	EQU	$E5
__local_instr_get_parent__1	EQU	$E5
__local_instr_get_prop__1	EQU	$E5
__local_instr_get_prop_addr__y	EQU	$E5
__local_instr_get_prop_len__1	EQU	$E5
__local_instr_get_sibling__1	EQU	$E5
__local_instr_insert_obj__1	EQU	$E5
__local_instr_put_prop__y	EQU	$E5
__local_print_obj__0	EQU	$E5
__local_instr_get_next_prop__1	EQU	$E6
__local_instr_get_prop__y	EQU	$E6
__local_instr_get_prop_addr__1	EQU	$E6
__local_instr_get_prop_len__2	EQU	$E6
__local_instr_insert_obj__2	EQU	$E6
__local_instr_jin__2	EQU	$E6
__local_instr_put_prop__1	EQU	$E6
__local_print_obj__1	EQU	$E6
__local_store_and_branch_on__related	EQU	$E6
__local_instr_clear_attr__3	EQU	$E7
__local_instr_get_prop__2	EQU	$E7
__local_instr_insert_obj__3	EQU	$E7
__local_instr_jin__3	EQU	$E7
__local_instr_put_prop__2	EQU	$E7
__local_attr_ptr_and_mask__attr	EQU	$E8
__local_instr_get_prop__3	EQU	$E8
__local_instr_insert_obj__4	EQU	$E8
__local_attr_ptr_and_mask__mask_1	EQU	$E9
__local_get_property_len__0	EQU	$E9
__local_instr_insert_obj__5	EQU	$E9
__local_attr_ptr_and_mask__n	EQU	$EA
BUFF_END	EQU	$EB
BUFF_LINE_LEN	EQU	$EC
CURR_LINE	EQU	$ED
PRINTER_CSW	EQU	$EE
__local_get_property_ptr__0	EQU	$F0
__local_get_property_ptr__1	EQU	$F1
__local_remove_obj__0	EQU	$F1
__local_get_property_ptr__2	EQU	$F2
__local_remove_obj__1	EQU	$F2
__local_remove_obj__2	EQU	$F3
__local_remove_obj__3	EQU	$F4
__local_remove_obj__4	EQU	$F5
__local_get_object_addr__0	EQU	$F6
__local_get_object_addr__1	EQU	$F7
__local_get_object_addr__2	EQU	$F8
__local_get_object_addr__3	EQU	$F9
__local_get_object_addr__obj	EQU	$FA
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
; def attr_ptr_and_mask params= locals=__local_attr_ptr_and_mask__attr,__local_attr_ptr_and_mask__mask_1,__local_attr_ptr_and_mask__n indirect=false in_cycle=false
; def get_object_addr params=__zpabi_get_object_addr__obj locals=__local_get_object_addr__0,__local_get_object_addr__1,__local_get_object_addr__2,__local_get_object_addr__3,__local_get_object_addr__obj indirect=false in_cycle=false param_regs=A
; def get_property_len params=__zpabi_get_property_len__y locals=__local_get_property_len__0 indirect=false in_cycle=false param_regs=A
; def get_property_num params=__zpabi_get_property_num__y locals=__local_get_property_num__0 indirect=false in_cycle=false param_regs=A
; def get_property_ptr params= locals=__local_get_property_ptr__0,__local_get_property_ptr__1,__local_get_property_ptr__2,__local_get_property_ptr__3 indirect=false in_cycle=false
; def instr_clear_attr params= locals=__local_instr_clear_attr__0,__local_instr_clear_attr__1,__local_instr_clear_attr__2,__local_instr_clear_attr__3 indirect=false in_cycle=false
; def instr_get_child params= locals=__local_instr_get_child__0,__local_instr_get_child__1 indirect=false in_cycle=false
; def instr_get_next_prop params= locals=__local_instr_get_next_prop__y,__local_instr_get_next_prop__0,__local_instr_get_next_prop__1 indirect=false in_cycle=false
; def instr_get_parent params= locals=__local_instr_get_parent__0,__local_instr_get_parent__1 indirect=false in_cycle=false
; def instr_get_prop params= locals=__local_instr_get_prop__0,__local_instr_get_prop__1,__local_instr_get_prop__y,__local_instr_get_prop__2,__local_instr_get_prop__3 indirect=false in_cycle=false
; def instr_get_prop_addr params= locals=__local_instr_get_prop_addr__0,__local_instr_get_prop_addr__y,__local_instr_get_prop_addr__1 indirect=false in_cycle=false
; def instr_get_prop_len params= locals=__local_instr_get_prop_len__0,__local_instr_get_prop_len__1,__local_instr_get_prop_len__2 indirect=false in_cycle=false
; def instr_get_sibling params= locals=__local_instr_get_sibling__0,__local_instr_get_sibling__1 indirect=false in_cycle=false
; def instr_insert_obj params= locals=__local_instr_insert_obj__0,__local_instr_insert_obj__1,__local_instr_insert_obj__2,__local_instr_insert_obj__3,__local_instr_insert_obj__4,__local_instr_insert_obj__5 indirect=false in_cycle=false
; def instr_jin params= locals=__local_instr_jin__0,__local_instr_jin__1,__local_instr_jin__2,__local_instr_jin__3 indirect=false in_cycle=false
; def instr_put_prop params= locals=__local_instr_put_prop__0,__local_instr_put_prop__y,__local_instr_put_prop__1,__local_instr_put_prop__2 indirect=false in_cycle=false
; def instr_remove_obj params= locals= indirect=false in_cycle=false
; def instr_set_attr params= locals=__local_instr_set_attr__0,__local_instr_set_attr__1 indirect=false in_cycle=false
; def instr_test_attr params= locals=__local_instr_test_attr__0,__local_instr_test_attr__1 indirect=false in_cycle=false
; def next_property params=__zpabi_next_property__y locals=__local_next_property__0,__local_next_property__y indirect=false in_cycle=false param_regs=A
; def print_obj params=__zpabi_print_obj__obj locals=__local_print_obj__obj,__local_print_obj__0,__local_print_obj__1 indirect=false in_cycle=false param_regs=A
; def remove_obj params= locals=__local_remove_obj__child,__local_remove_obj__0,__local_remove_obj__1,__local_remove_obj__2,__local_remove_obj__3,__local_remove_obj__4 indirect=false in_cycle=false
; def set_attr_word params=__zpabi_set_attr_word__word_0,__zpabi_set_attr_word__word_1 locals= indirect=false in_cycle=false
; def store_and_branch_on params=__zpabi_store_and_branch_on__related locals=__local_store_and_branch_on__related indirect=false in_cycle=false param_regs=A
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
; ext instr_div params=
; ext instr_mod params=
; ext instr_mul params=
; ext instr_quit params=
; ext instr_random params=
; ext instr_restart params=
; ext instr_restore params=
; ext instr_save params=
; ext instr_sread params=
; ext internal_error params=
; ext load_address params=__zpabi_load_address__addr_0,__zpabi_load_address__addr_1
; ext load_packed_address params=__zpabi_load_packed_address__addr_0,__zpabi_load_packed_address__addr_1
; ext new_line params=
; ext please_insert_save_diskette params=
; ext please_reinsert_game_diskette params=
; ext pop params=
; ext print_ascii_string params=__zpabi_print_ascii_string__s_0,__zpabi_print_ascii_string__s_1,__zpabi_print_ascii_string__count param_regs=,,X
; ext print_number params=__zpabi_print_number__n_0,__zpabi_print_number__n_1
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
; call attr_ptr_and_mask -> get_object_addr
; call get_property_ptr -> get_object_addr
; call instr_clear_attr -> attr_ptr_and_mask
; call instr_clear_attr -> set_attr_word
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
; call instr_put_prop -> get_property_len
; call instr_put_prop -> get_property_num
; call instr_put_prop -> get_property_ptr
; call instr_put_prop -> internal_error
; call instr_put_prop -> next_property
; call instr_remove_obj -> remove_obj
; call instr_set_attr -> attr_ptr_and_mask
; call instr_set_attr -> set_attr_word
; call instr_test_attr -> attr_ptr_and_mask
; call instr_test_attr -> branch
; call next_property -> get_property_len
; call print_obj -> get_object_addr
; call print_obj -> load_address
; call print_obj -> print_zstring
; call remove_obj -> get_object_addr
; call store_and_branch_on -> branch
; call store_and_branch_on -> store_result
; item get_object_addr section=.text local=false zero=false
; item remove_obj section=.text local=true zero=false
; item instr_remove_obj section=.text local=false zero=false
; item instr_insert_obj section=.text local=false zero=false
; item print_obj section=.text local=false zero=false
; item store_and_branch_on section=.text local=true zero=false
; item instr_get_sibling section=.text local=false zero=false
; item instr_get_child section=.text local=false zero=false
; item instr_get_parent section=.text local=false zero=false
; item instr_jin section=.text local=false zero=false
; item attr_ptr_and_mask section=.text local=true zero=false
; item set_attr_word section=.text local=true zero=false
; item instr_test_attr section=.text local=false zero=false
; item instr_set_attr section=.text local=false zero=false
; item instr_clear_attr section=.text local=false zero=false
; item get_property_ptr section=.text local=true zero=false
; item get_property_num section=.text local=true zero=false
; item get_property_len section=.text local=true zero=false
; item next_property section=.text local=true zero=false
; item instr_get_prop section=.text local=false zero=false
; item instr_get_prop_addr section=.text local=false zero=false
; item instr_get_next_prop section=.text local=false zero=false
; item instr_get_prop_len section=.text local=false zero=false
; item instr_put_prop section=.text local=false zero=false
; item attr_mask section=.bss local=true zero=true
; item __zp_init section=.text local=false zero=false
; zp props size=2 at=auto def=true init=$00,$00 weight=18 local=true
; zp attr_ptr size=2 at=auto def=true init=$00,$00 weight=7 local=true
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
; runtime hargs_size=5 hargs_need=0
; @zp-link-meta-end

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

instr_remove_obj:
   SUBROUTINE

   JMP   remove_obj

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
   JMP   print_zstring

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
   JMP   branch

instr_get_sibling:
   SUBROUTINE

   LDA   OPERANDS
   JSR   get_object_addr
   STA   __local_instr_get_sibling__0
   STX   __local_instr_get_sibling__1
   LDY   #$05
   LDA   (__local_instr_get_sibling__0),Y
   JMP   store_and_branch_on

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

set_attr_word:
   SUBROUTINE

   LDA   __zpabi_set_attr_word__word_0
   LDY   #$01
   STA   (attr_ptr),Y
   LDA   __zpabi_set_attr_word__word_1
   DEY
   STA   (attr_ptr),Y
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
   JMP   set_attr_word

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

attr_mask:
   DS.B  2

__zp_init:
   SUBROUTINE

   LDA   #$00
   STA   attr_ptr
   STA   attr_ptr+1
   STA   props
   STA   props+1
   RTS
