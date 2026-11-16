HARGS	EQU	$00
SSP	EQU	$05
FP	EQU	$07
DPTR	EQU	$09
__local_branch_to_offset__0	EQU	$0B
__local_return_value__0	EQU	$0B
__local_return_value__1	EQU	$0C
__local_return_value__x	EQU	$0D
__local_branch_to_offset__pages_1	EQU	$0E
__local_return_value__count	EQU	$0E
__local_branch_to_offset__2	EQU	$0F
__local_store_result__0	EQU	$0F
__local_set_nonstack_var__var	EQU	$10
__local_set_nonstack_var__0	EQU	$11
__local_set_nonstack_var__1	EQU	$12
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
__local_var_get__var	EQU	$E0
__zpabi_branch_to_offset__offset_0	EQU	$E0
__zpabi_cout_string__s_0	EQU	$E0
__zpabi_load_address__addr_0	EQU	$E0
__zpabi_load_packed_address__addr_0	EQU	$E0
__zpabi_print_ascii_string__s_0	EQU	$E0
__zpabi_print_number__n_0	EQU	$E0
__zpabi_return_value__value_0	EQU	$E0
__zpabi_var_put__value_0	EQU	$E0
__local_get_const_word__1	EQU	$E1
__local_var_get__0	EQU	$E1
__zpabi_branch_to_offset__offset_1	EQU	$E1
__zpabi_cout_string__s_1	EQU	$E1
__zpabi_load_address__addr_1	EQU	$E1
__zpabi_load_packed_address__addr_1	EQU	$E1
__zpabi_print_ascii_string__s_1	EQU	$E1
__zpabi_print_number__n_1	EQU	$E1
__zpabi_return_value__value_1	EQU	$E1
__zpabi_var_put__value_1	EQU	$E1
Z_ABBREV_TABLE	EQU	$E2
__local_get_nonstack_var__var	EQU	$E4
__local_var_put__var	EQU	$E4
__zpabi_store_result__value_0	EQU	$E4
__local_get_nonstack_var__0	EQU	$E5
__zpabi_store_result__value_1	EQU	$E5
__local_get_nonstack_var__1	EQU	$E6
__local_pop__1	EQU	$E6
__zpabi_push__value_0	EQU	$E6
__zpabi_set_nonstack_var__value_0	EQU	$E6
__local_get_nonstack_var__2	EQU	$E7
__local_pop__2	EQU	$E7
__zpabi_push__value_1	EQU	$E7
__zpabi_set_nonstack_var__value_1	EQU	$E7
BUFF_END	EQU	$EB
BUFF_LINE_LEN	EQU	$EC
CURR_LINE	EQU	$ED
PRINTER_CSW	EQU	$EE
__local_do_instruction__0	EQU	$F1
__local_do_instruction__v_0	EQU	$F2
__local_do_instruction__forms	EQU	$F3
__local_do_instruction__1	EQU	$F4
__local_do_2op__equal	EQU	$F5
__local_do_var__n	EQU	$F5
__local_do_2op__0	EQU	$F6
__local_do_var__0	EQU	$F6
__local_do_var__1	EQU	$F7
__local_do_2op__left	EQU	$F8
__local_do_var__2	EQU	$F8
__local_do_2op__2	EQU	$F9
__local_do_var__3	EQU	$F9
__local_do_2op__3	EQU	$FA
__local_instr_call__1	EQU	$FB
__local_instr_call__i	EQU	$FC
__local_branch__offset_1	EQU	$FD
__local_dec_var__0	EQU	$FD
__local_inc_var__0	EQU	$FD
__local_instr_call__args	EQU	$FD
__local_branch__offset_0	EQU	$FE
__local_dec_var__1	EQU	$FE
__local_inc_var__1	EQU	$FE
__local_instr_call__2	EQU	$FE
__local_branch__condition	EQU	$FF
__local_dec_var__2	EQU	$FF
__local_inc_var__2	EQU	$FF
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
; def branch params=__zpabi_branch__condition locals=__local_branch__offset_1,__local_branch__offset_0,__local_branch__condition indirect=false in_cycle=false param_regs=A
; def branch_to_offset params=__zpabi_branch_to_offset__offset_0,__zpabi_branch_to_offset__offset_1 locals=__local_branch_to_offset__0,__local_branch_to_offset__pages_0,__local_branch_to_offset__1,__local_branch_to_offset__pages_1,__local_branch_to_offset__2 indirect=false in_cycle=false changes=__zpabi_branch_to_offset__offset_1
; def dec_var params= locals=__local_dec_var__0,__local_dec_var__1,__local_dec_var__2 indirect=false in_cycle=false
; def do_0op params=__zpabi_do_0op__n locals=__local_do_0op__n indirect=false in_cycle=false param_regs=A
; def do_1op params=__zpabi_do_1op__n locals=__local_do_1op__0,__local_do_1op__1,__local_do_1op__n indirect=false in_cycle=false param_regs=A
; def do_2op params=__zpabi_do_2op__n locals=__local_do_2op__equal,__local_do_2op__0,__local_do_2op__1,__local_do_2op__left,__local_do_2op__2,__local_do_2op__3,__local_do_2op__4,__local_do_2op__5 indirect=false in_cycle=false param_regs=A
; def do_instruction params= locals=__local_do_instruction__v_1,__local_do_instruction__0,__local_do_instruction__v_0,__local_do_instruction__forms,__local_do_instruction__1 indirect=false in_cycle=false
; def do_var params=__zpabi_do_var__n locals=__local_do_var__n,__local_do_var__0,__local_do_var__1,__local_do_var__2,__local_do_var__3 indirect=false in_cycle=false param_regs=A
; def get_const_word params= locals=__local_get_const_word__0,__local_get_const_word__1 indirect=false in_cycle=false
; def get_nonstack_var params=__zpabi_get_nonstack_var__var locals=__local_get_nonstack_var__var,__local_get_nonstack_var__0,__local_get_nonstack_var__1,__local_get_nonstack_var__2 indirect=false in_cycle=false param_regs=A
; def get_var_content params= locals=__local_get_var_content__0 indirect=false in_cycle=false
; def inc_var params= locals=__local_inc_var__0,__local_inc_var__1,__local_inc_var__2 indirect=false in_cycle=false
; def instr_call params= locals=__local_instr_call__0,__local_instr_call__1,__local_instr_call__i,__local_instr_call__args,__local_instr_call__2 indirect=false in_cycle=false
; def pop params= locals=__local_pop__0,__local_pop__1,__local_pop__2 indirect=false in_cycle=false
; def push params=__zpabi_push__value_0,__zpabi_push__value_1 locals=__local_push__0 indirect=false in_cycle=false
; def return_value params=__zpabi_return_value__value_0,__zpabi_return_value__value_1 locals=__local_return_value__0,__local_return_value__1,__local_return_value__x,__local_return_value__count indirect=false in_cycle=false
; def set_nonstack_var params=__zpabi_set_nonstack_var__var,__zpabi_set_nonstack_var__value_0,__zpabi_set_nonstack_var__value_1 locals=__local_set_nonstack_var__var,__local_set_nonstack_var__0,__local_set_nonstack_var__1 indirect=false in_cycle=false param_regs=A,,
; def store_result params=__zpabi_store_result__value_0,__zpabi_store_result__value_1 locals=__local_store_result__0 indirect=false in_cycle=false
; def var_get params=__zpabi_var_get__var locals=__local_var_get__var,__local_var_get__0 indirect=false in_cycle=false param_regs=A
; def var_put params=__zpabi_var_put__var,__zpabi_var_put__value_0,__zpabi_var_put__value_1 locals=__local_var_put__var indirect=false in_cycle=false param_regs=A,,
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
; ext buffer_char params=__zpabi_buffer_char__c param_regs=A
; ext cout_string params=__zpabi_cout_string__s_0,__zpabi_cout_string__s_1,__zpabi_cout_string__count param_regs=,,X
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
; ext print_ascii_string params=__zpabi_print_ascii_string__s_0,__zpabi_print_ascii_string__s_1,__zpabi_print_ascii_string__count param_regs=,,X
; ext print_number params=__zpabi_print_number__n_0,__zpabi_print_number__n_1
; ext print_obj params=__zpabi_print_obj__obj param_regs=A
; ext print_status_line params=
; ext print_string_literal params=
; ext print_zstring params=
; ext read_from_sector params= carry=true
; ext read_next_sector params= carry=true
; ext reset_window params=
; ext restart params=
; ext write_next_sector params= carry=true
; ext write_to_sector params= carry=true
; call branch -> branch_to_offset
; call branch -> get_next_code_byte
; call branch -> return_value
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
; call get_const_word -> get_next_code_byte
; call get_var_content -> get_next_code_byte
; call get_var_content -> get_nonstack_var
; call get_var_content -> pop
; call inc_var -> var_get
; call inc_var -> var_put
; call instr_call -> get_next_code_byte
; call instr_call -> push
; call instr_call -> store_result
; call pop -> internal_error
; call push -> internal_error
; call return_value -> pop
; call return_value -> store_result
; call store_result -> get_next_code_byte
; call store_result -> push
; call store_result -> set_nonstack_var
; call var_get -> get_nonstack_var
; call var_get -> pop
; call var_get -> push
; call var_put -> pop
; call var_put -> push
; call var_put -> set_nonstack_var
; item push section=.text local=false zero=false
; item pop section=.text local=false zero=false
; item get_nonstack_var section=.text local=true zero=false
; item set_nonstack_var section=.text local=true zero=false
; item var_get section=.text local=false zero=false
; item var_put section=.text local=false zero=false
; item store_result section=.text local=false zero=false
; item get_const_word section=.text local=true zero=false
; item get_var_content section=.text local=true zero=false
; item return_value section=.text local=false zero=false
; item branch_to_offset section=.text local=true zero=false
; item branch section=.text local=false zero=false
; item instr_call section=.text local=true zero=false
; item inc_var section=.text local=true zero=false
; item dec_var section=.text local=true zero=false
; item do_0op section=.text local=true zero=false
; item do_1op section=.text local=true zero=false
; item do_2op section=.text local=true zero=false
; item do_var section=.text local=true zero=false
; item do_instruction section=.text local=false zero=false
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
   JMP   internal_error
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
   JMP   internal_error
.if_end@1:
   LDA   __local_pop__1
   LDX   __local_pop__2
   RTS

get_nonstack_var:
   SUBROUTINE

   STA   __local_get_nonstack_var__var
   CMP   #$10
   BCS   .if_end@2
   DEC   __local_get_nonstack_var__var
   LDA   __local_get_nonstack_var__var
   ASL
   TAX
   LDA   LOCAL_ZVARS+1,X
   STA   __local_get_nonstack_var__var
   LDA   LOCAL_ZVARS,X
   TAX
   LDA   __local_get_nonstack_var__var
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
   LDA   __zpabi_set_nonstack_var__value_0
   STA   LOCAL_ZVARS+1,X
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
   LDA   __zpabi_set_nonstack_var__value_1
   LDY   #$00
   STA   (__local_set_nonstack_var__var),Y
   LDA   __zpabi_set_nonstack_var__value_0
   INY
   STA   (__local_set_nonstack_var__var),Y
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
   JMP   get_nonstack_var

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
   JMP   set_nonstack_var

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
   JMP   push
.if_else@7:
   LDA   __zpabi_store_result__value_0
   STA   __zpabi_set_nonstack_var__value_0
   LDA   __zpabi_store_result__value_1
   STA   __zpabi_set_nonstack_var__value_1
   LDA   __local_store_result__0
   JMP   set_nonstack_var

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
   LDA   __local_return_value__1
   STA   LOCAL_ZVARS+1,X
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
   JMP   store_result

branch_to_offset:
   SUBROUTINE

   LDA   __zpabi_branch_to_offset__offset_1
   BPL   .branch_to_offset@asm_ssa_split@0
   LDY   #$01
   STY   __local_branch_to_offset__pages_1
.if_end@10:
   LDA   Z_PC
   CLC
   ADC   __zpabi_branch_to_offset__offset_0
   STA   __local_branch_to_offset__2
   CMP   Z_PC
   BCS   .if_end@11
   INC   __zpabi_branch_to_offset__offset_1
   BNE   .inc_done@0
   INC   __local_branch_to_offset__pages_1
.inc_done@0:
.if_end@11:
   LDA   __zpabi_branch_to_offset__offset_1
   ORA   __local_branch_to_offset__pages_1
   BNE   .if_end@12
   LDA   __local_branch_to_offset__2
   STA   Z_PC
   RTS
.if_end@12:
   LDA   Z_PC+1
   CLC
   ADC   __zpabi_branch_to_offset__offset_1
   STA   __zpabi_branch_to_offset__offset_1
   LDA   Z_PC+2
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
   JMP   branch_to_offset

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
   STA   __local_instr_call__2
   ORA   #$00
   BEQ   .if_end@21
   LDY   #$00
   STY   __local_instr_call__args
   STA   __local_instr_call__i
.loop@1_start:
   LDX   __local_instr_call__args
   LDA   LOCAL_ZVARS,X
   STA   __zpabi_push__value_1
   LDA   LOCAL_ZVARS+1,X
   STA   __zpabi_push__value_0
   JSR   push
   JSR   get_next_code_byte
   STA   __local_instr_call__1
   JSR   get_next_code_byte
   LDX   __local_instr_call__args
   STA   LOCAL_ZVARS+1,X
   LDA   __local_instr_call__1
   STA   LOCAL_ZVARS,X
   INX
   INX
   STX   __local_instr_call__args
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
   ASL
   TAX
   LDA   OPERANDS+3,X
   STA   LOCAL_ZVARS,X
   LDA   OPERANDS+2,X
   STA   LOCAL_ZVARS+1,X
   INC   __local_instr_call__i
   DEC   __local_instr_call__args
.loop@2_continue:
   LDA   __local_instr_call__args
   BNE   .instr_call@rot@0
   LDA   __local_instr_call__2
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
   CMP   #$0C
   BCS   .default@16
   ASL
   TAY
   LDA   .jtab@0+1,Y
   PHA
   LDA   .jtab@0,Y
   PHA
   RTS
.jtab@0:
   DC.W  .default@16-1
   DC.W  .case@5-1
   DC.W  .case@6-1
   DC.W  .case@7-1
   DC.W  .switch@3_break-1
   DC.W  .case@9-1
   DC.W  .case@10-1
   DC.W  .case@11-1
   DC.W  .case@12-1
   DC.W  .case@13-1
   DC.W  .case@14-1
   DC.W  .case@15-1
.case@4:
   LDA   #$01
   STA   __zpabi_return_value__value_0
   LDA   #$00
   STA   __zpabi_return_value__value_1
   JMP   return_value
.case@5:
   LDA   #$00
   STA   __zpabi_return_value__value_0
   STA   __zpabi_return_value__value_1
   JMP   return_value
.case@6:
   JMP   print_string_literal
.case@7:
   JSR   print_string_literal
   JSR   new_line
   JMP   .case@4
.case@9:
   JMP   instr_save
.case@10:
   JMP   instr_restore
.case@11:
   JMP   instr_restart
.case@12:
   JSR   pop
   STA   __zpabi_return_value__value_0
   STX   __zpabi_return_value__value_1
   JMP   return_value
.case@13:
   JMP   pop
.case@14:
   JMP   instr_quit
.case@15:
   JMP   new_line
.default@16:
   JMP   internal_error
.switch@3_break:
   RTS

do_1op:
   SUBROUTINE

   ORA   #$00
   BEQ   .case@18
   CMP   #$10
   BCC   .lb_skip@0
   JMP   .default@33
.lb_skip@0:
   ASL
   TAY
   LDA   .jtab@1+1,Y
   PHA
   LDA   .jtab@1,Y
   PHA
   RTS
.jtab@1:
   DC.W  .default@33-1
   DC.W  .case@19-1
   DC.W  .case@20-1
   DC.W  .case@21-1
   DC.W  .case@22-1
   DC.W  .case@23-1
   DC.W  .case@24-1
   DC.W  .case@25-1
   DC.W  .default@33-1
   DC.W  .case@26-1
   DC.W  .case@27-1
   DC.W  .case@28-1
   DC.W  .case@29-1
   DC.W  .case@30-1
   DC.W  .case@31-1
   DC.W  .case@32-1
.case@18:
   LDA   OPERANDS+1
   ORA   OPERANDS
   BEQ   .cmp_true@2
   LDA   #$00
   JMP   .cmp_end@1
.cmp_true@2:
   LDA   #$01
.cmp_end@1:
   JMP   branch
.case@19:
   JMP   instr_get_sibling
.case@20:
   JMP   instr_get_child
.case@21:
   JMP   instr_get_parent
.case@22:
   JMP   instr_get_prop_len
.case@23:
   JMP   inc_var
.case@24:
   JMP   dec_var
.case@25:
   LDA   OPERANDS
   STA   __zpabi_load_address__addr_0
   LDA   OPERANDS+1
   STA   __zpabi_load_address__addr_1
   JSR   load_address
   JMP   print_zstring
.case@26:
   JMP   instr_remove_obj
.case@27:
   LDA   OPERANDS
   JMP   print_obj
.case@28:
   LDA   OPERANDS
   STA   __zpabi_return_value__value_0
   LDA   OPERANDS+1
   STA   __zpabi_return_value__value_1
   JMP   return_value
.case@29:
   LDA   OPERANDS
   SEC
   SBC   #$02
   STA   __zpabi_branch_to_offset__offset_0
   LDA   OPERANDS+1
   SBC   #$00
   STA   __zpabi_branch_to_offset__offset_1
   JMP   branch_to_offset
.case@30:
   LDA   OPERANDS
   STA   __zpabi_load_packed_address__addr_0
   LDA   OPERANDS+1
   STA   __zpabi_load_packed_address__addr_1
   JSR   load_packed_address
   JMP   print_zstring
.case@31:
   LDA   OPERANDS
   JSR   var_get
   STA   __zpabi_store_result__value_0
   STX   __zpabi_store_result__value_1
   JMP   store_result
.case@32:
   LDA   OPERANDS
   EOR   #$FF
   STA   __zpabi_store_result__value_0
   LDA   OPERANDS+1
   EOR   #$FF
   STA   __zpabi_store_result__value_1
   JMP   store_result
.default@33:
   JMP   internal_error

do_2op:
   SUBROUTINE

   STA   __local_do_2op__equal
   LDA   OPERANDS+2
   STA   __local_do_2op__2
   LDA   OPERANDS+3
   STA   __local_do_2op__3
   LDA   __local_do_2op__equal
   CMP   #$19
   BCC   .lb_skip@1
   JMP   .default@59
.lb_skip@1:
   ASL
   TAY
   LDA   .jtab@2+1,Y
   PHA
   LDA   .jtab@2,Y
   PHA
   RTS
.jtab@2:
   DC.W  .default@59-1
   DC.W  .case@35-1
   DC.W  .case@36-1
   DC.W  .case@37-1
   DC.W  .case@38-1
   DC.W  .case@39-1
   DC.W  .case@40-1
   DC.W  .case@41-1
   DC.W  .case@42-1
   DC.W  .case@43-1
   DC.W  .case@44-1
   DC.W  .case@45-1
   DC.W  .case@46-1
   DC.W  .case@47-1
   DC.W  .case@48-1
   DC.W  .case@49-1
   DC.W  .case@50-1
   DC.W  .case@51-1
   DC.W  .case@52-1
   DC.W  .case@53-1
   DC.W  .case@54-1
   DC.W  .case@55-1
   DC.W  .case@56-1
   DC.W  .case@57-1
   DC.W  .case@58-1
.case@35:
   LDA   OPERAND_COUNT
   SEC
   SBC   #$01
   STA   __local_do_2op__left
   BNE   .if_end@22
   JMP   internal_error
.if_end@22:
   LDA   OPERANDS+1
   CMP   __local_do_2op__3
   BNE   .thread@0
   LDA   OPERANDS
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
   LDA   OPERANDS+1
   CMP   OPERANDS+5
   BNE   .thread@1
   LDA   OPERANDS
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
   LDA   OPERANDS+1
   CMP   OPERANDS+7
   BNE   .thread@2
   LDA   OPERANDS
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
   JMP   branch
.case@36:
   LDA   OPERANDS
   SEC
   SBC   __local_do_2op__2
   LDA   OPERANDS+1
   SBC   __local_do_2op__3
   BVC   .cmp_novf@12
   EOR   #$80
.cmp_novf@12:
   ASL
   LDA   #$00
   ROL
   JMP   branch
.case@37:
   LDA   __local_do_2op__2
   SEC
   SBC   OPERANDS
   LDA   __local_do_2op__3
   SBC   OPERANDS+1
   BVC   .cmp_novf@12
   EOR   #$80
   JMP   .cmp_novf@12
.case@38:
   JSR   dec_var
   SEC
   SBC   __local_do_2op__2
   TXA
   SBC   __local_do_2op__3
   BVC   .cmp_novf@12
   EOR   #$80
   JMP   .cmp_novf@12
.case@39:
   JSR   inc_var
   STA   __local_do_2op__0
   STX   __local_do_2op__equal
   LDA   __local_do_2op__2
   SEC
   SBC   __local_do_2op__0
   LDA   __local_do_2op__3
   SBC   __local_do_2op__equal
   BVC   .cmp_novf@12
   EOR   #$80
   JMP   .cmp_novf@12
.case@40:
   JMP   instr_jin
.case@41:
   LDA   OPERANDS
   AND   __local_do_2op__2
   STA   __local_do_2op__0
   LDA   OPERANDS+1
   AND   __local_do_2op__3
   CMP   __local_do_2op__3
   BNE   .thread@3
   LDA   __local_do_2op__0
   CMP   __local_do_2op__2
   BEQ   .cmp_true@16
.thread@3:
   LDA   #$00
   JMP   .cmp_end@17
.cmp_true@16:
   LDA   #$01
.cmp_end@17:
   JMP   branch
.case@42:
   LDA   OPERANDS
   ORA   __local_do_2op__2
   STA   __zpabi_store_result__value_0
   LDA   OPERANDS+1
   ORA   __local_do_2op__3
   STA   __zpabi_store_result__value_1
   JMP   store_result
.case@43:
   LDA   OPERANDS
   AND   __local_do_2op__2
   STA   __zpabi_store_result__value_0
   LDA   OPERANDS+1
   AND   __local_do_2op__3
   STA   __zpabi_store_result__value_1
   JMP   store_result
.case@44:
   JMP   instr_test_attr
.case@45:
   JMP   instr_set_attr
.case@46:
   JMP   instr_clear_attr
.case@47:
   LDA   __local_do_2op__2
   STA   __zpabi_var_put__value_0
   LDA   __local_do_2op__3
   STA   __zpabi_var_put__value_1
   LDA   OPERANDS
   JMP   var_put
.case@48:
   JMP   instr_insert_obj
.case@49:
   ASL   __local_do_2op__2
   ROL   __local_do_2op__3
   LDA   OPERANDS
   CLC
   ADC   __local_do_2op__2
   STA   __zpabi_load_address__addr_0
   LDA   OPERANDS+1
   ADC   __local_do_2op__3
   STA   __zpabi_load_address__addr_1
   JSR   load_address
   JSR   get_next_code_word
   STA   __zpabi_store_result__value_0
   STX   __zpabi_store_result__value_1
   JMP   store_result
.case@50:
   LDA   OPERANDS
   CLC
   ADC   __local_do_2op__2
   STA   __zpabi_load_address__addr_0
   LDA   OPERANDS+1
   ADC   __local_do_2op__3
   STA   __zpabi_load_address__addr_1
   JSR   load_address
   JSR   get_next_code_byte2
   STA   __zpabi_store_result__value_0
   LDA   #$00
   STA   __zpabi_store_result__value_1
   JMP   store_result
.case@51:
   JMP   instr_get_prop
.case@52:
   JMP   instr_get_prop_addr
.case@53:
   JMP   instr_get_next_prop
.case@54:
   LDA   OPERANDS
   CLC
   ADC   __local_do_2op__2
   STA   __zpabi_store_result__value_0
   LDA   OPERANDS+1
   ADC   __local_do_2op__3
   STA   __zpabi_store_result__value_1
   JMP   store_result
.case@55:
   LDA   OPERANDS
   SEC
   SBC   __local_do_2op__2
   STA   __zpabi_store_result__value_0
   LDA   OPERANDS+1
   SBC   __local_do_2op__3
   STA   __zpabi_store_result__value_1
   JMP   store_result
.case@56:
   JMP   instr_mul
.case@57:
   JMP   instr_div
.case@58:
   JMP   instr_mod
.default@59:
   JMP   internal_error

do_var:
   SUBROUTINE

   STA   __local_do_var__n
   LDA   OPERANDS
   STA   __local_do_var__3
   LDA   __local_do_var__n
   CMP   #$0A
   BCC   .lb_skip@2
   JMP   .default@71
.lb_skip@2:
   ASL
   TAY
   LDA   .jtab@3+1,Y
   PHA
   LDA   .jtab@3,Y
   PHA
   RTS
.jtab@3:
   DC.W  .case@61-1
   DC.W  .case@62-1
   DC.W  .case@63-1
   DC.W  .case@64-1
   DC.W  .case@65-1
   DC.W  .case@66-1
   DC.W  .case@67-1
   DC.W  .case@68-1
   DC.W  .case@69-1
   DC.W  .case@70-1
.case@61:
   JMP   instr_call
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
   RTS
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
   RTS
.case@64:
   JMP   instr_put_prop
.case@65:
   JMP   instr_sread
.case@66:
   LDA   __local_do_var__3
   JMP   buffer_char
.case@67:
   LDA   __local_do_var__3
   STA   __zpabi_print_number__n_0
   LDA   OPERANDS+1
   STA   __zpabi_print_number__n_1
   JMP   print_number
.case@68:
   JMP   instr_random
.case@69:
   LDA   __local_do_var__3
   STA   __zpabi_push__value_0
   LDA   OPERANDS+1
   STA   __zpabi_push__value_1
   JMP   push
.case@70:
   JSR   pop
   STA   __zpabi_var_put__value_0
   STX   __zpabi_var_put__value_1
   LDA   __local_do_var__3
   JMP   var_put
.default@71:
   JMP   internal_error

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
   JMP   .if_end@35
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
.if_end@37:
.if_end@35:
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
   JMP   do_2op
