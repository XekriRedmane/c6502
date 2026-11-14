HARGS	EQU	$00
SSP	EQU	$01
FP	EQU	$03
DPTR	EQU	$05
save_sector	EQU	$07
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
__zpabi_copy_data_from_buff__dest_0	EQU	$E0
__zpabi_copy_data_to_buff__src_0	EQU	$E0
__zpabi_cout_string__s_0	EQU	$E0
__zpabi_load_address__addr_0	EQU	$E0
__zpabi_load_packed_address__addr_0	EQU	$E0
__zpabi_print_ascii_string__s_0	EQU	$E0
__zpabi_print_number__n_0	EQU	$E0
__zpabi_push__value_0	EQU	$E0
__zpabi_return_value__value_0	EQU	$E0
__zpabi_store_result__value_0	EQU	$E0
__zpabi_var_put__value_0	EQU	$E0
__zpabi_copy_data_from_buff__dest_1	EQU	$E1
__zpabi_copy_data_to_buff__src_1	EQU	$E1
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
__zpabi_inc_sector_and_read__buffer_0	EQU	$E4
__zpabi_inc_sector_and_write__buffer_0	EQU	$E4
__zpabi_inc_sector_and_read__buffer_1	EQU	$E5
__zpabi_inc_sector_and_write__buffer_1	EQU	$E5
__zpabi_read_from_sector__sector_0	EQU	$E6
__zpabi_write_to_sector__sector_0	EQU	$E6
__zpabi_read_from_sector__sector_1	EQU	$E7
__zpabi_write_to_sector__sector_1	EQU	$E7
__zpabi_read_from_sector__buffer_0	EQU	$E8
__zpabi_write_to_sector__buffer_0	EQU	$E8
__zpabi_read_from_sector__buffer_1	EQU	$E9
__zpabi_write_to_sector__buffer_1	EQU	$E9
BUFF_END	EQU	$EB
BUFF_LINE_LEN	EQU	$EC
CURR_LINE	EQU	$ED
PRINTER_CSW	EQU	$EE
__local_instr_restore__pages	EQU	$F1
__local_instr_save__pages	EQU	$F1
__local_instr_restore__1	EQU	$F2
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
; def copy_data_from_buff params=__zpabi_copy_data_from_buff__dest_0,__zpabi_copy_data_from_buff__dest_1,__zpabi_copy_data_from_buff__x,__zpabi_copy_data_from_buff__count locals=__local_copy_data_from_buff__0 indirect=false in_cycle=false param_regs=,,X,Y
; def copy_data_to_buff params=__zpabi_copy_data_to_buff__src_0,__zpabi_copy_data_to_buff__src_1,__zpabi_copy_data_to_buff__x,__zpabi_copy_data_to_buff__count locals=__local_copy_data_to_buff__0 indirect=false in_cycle=false param_regs=,,X,Y
; def inc_sector_and_read params=__zpabi_inc_sector_and_read__buffer_0,__zpabi_inc_sector_and_read__buffer_1 locals=__local_inc_sector_and_read__0 indirect=false in_cycle=false
; def inc_sector_and_write params=__zpabi_inc_sector_and_write__buffer_0,__zpabi_inc_sector_and_write__buffer_1 locals=__local_inc_sector_and_write__0 indirect=false in_cycle=false
; def instr_restore params= locals=__local_instr_restore__0,__local_instr_restore__pages,__local_instr_restore__1 indirect=false in_cycle=false
; def instr_save params= locals=__local_instr_save__0,__local_instr_save__pages indirect=false in_cycle=false
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
; ext read_from_sector params=__zpabi_read_from_sector__sector_0,__zpabi_read_from_sector__sector_1,__zpabi_read_from_sector__buffer_0,__zpabi_read_from_sector__buffer_1
; ext reset_window params=
; ext restart params=
; ext return_value params=__zpabi_return_value__value_0,__zpabi_return_value__value_1
; ext store_result params=__zpabi_store_result__value_0,__zpabi_store_result__value_1
; ext var_get params=__zpabi_var_get__var param_regs=A
; ext var_put params=__zpabi_var_put__var,__zpabi_var_put__value_0,__zpabi_var_put__value_1 param_regs=A,,
; ext write_to_sector params=__zpabi_write_to_sector__sector_0,__zpabi_write_to_sector__sector_1,__zpabi_write_to_sector__buffer_0,__zpabi_write_to_sector__buffer_1
; call inc_sector_and_read -> read_from_sector
; call inc_sector_and_write -> write_to_sector
; call instr_restore -> branch
; call instr_restore -> copy_data_from_buff
; call instr_restore -> inc_sector_and_read
; call instr_restore -> please_insert_save_diskette
; call instr_restore -> please_reinsert_game_diskette
; call instr_save -> branch
; call instr_save -> copy_data_to_buff
; call instr_save -> inc_sector_and_write
; call instr_save -> please_insert_save_diskette
; call instr_save -> please_reinsert_game_diskette
; share member=inc_sector_and_read:__zpabi_inc_sector_and_read__buffer_0,__zpabi_inc_sector_and_read__buffer_1 holder=instr_restore live=instr_restore:inc_sector_and_read writer=instr_restore
; share member=inc_sector_and_write:__zpabi_inc_sector_and_write__buffer_0,__zpabi_inc_sector_and_write__buffer_1 holder=instr_save live=instr_save:inc_sector_and_write writer=instr_save
; item inc_sector_and_write section=.text local=true zero=false
; item inc_sector_and_read section=.text local=true zero=false
; item copy_data_to_buff section=.text local=true zero=false
; item copy_data_from_buff section=.text local=true zero=false
; item instr_save section=.text local=false zero=false
; item instr_restore section=.text local=false zero=false
; item __zp_init section=.text local=false zero=false
; zp save_sector size=2 at=auto def=true init=$00,$00 weight=10 local=true
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

inc_sector_and_write:
   SUBROUTINE

   INC   save_sector
   BNE   .inc_done@0
   INC   save_sector+1
.inc_done@0:
   LDA   save_sector
   STA   __zpabi_write_to_sector__sector_0
   LDA   save_sector+1
   STA   __zpabi_write_to_sector__sector_1
   LDA   __zpabi_inc_sector_and_write__buffer_0
   STA   __zpabi_write_to_sector__buffer_0
   LDA   __zpabi_inc_sector_and_write__buffer_1
   STA   __zpabi_write_to_sector__buffer_1
   JMP   write_to_sector

inc_sector_and_read:
   SUBROUTINE

   INC   save_sector
   BNE   .inc_done@1
   INC   save_sector+1
.inc_done@1:
   LDA   save_sector
   STA   __zpabi_read_from_sector__sector_0
   LDA   save_sector+1
   STA   __zpabi_read_from_sector__sector_1
   LDA   __zpabi_inc_sector_and_read__buffer_0
   STA   __zpabi_read_from_sector__buffer_0
   LDA   __zpabi_inc_sector_and_read__buffer_1
   STA   __zpabi_read_from_sector__buffer_1
   JMP   read_from_sector

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
   STA   save_sector
   STX   save_sector+1
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
   LDA   #<BUFF_AREA
   STA   __zpabi_inc_sector_and_write__buffer_0
   LDA   #>BUFF_AREA
   STA   __zpabi_inc_sector_and_write__buffer_1
   JSR   inc_sector_and_write
   BCS   .instr_save@fail
   LDA   #$80
   STA   __zpabi_copy_data_to_buff__src_0
   LDA   #$02
   STA   __zpabi_copy_data_to_buff__src_1
   LDX   #$00
   LDY   #$00
   JSR   copy_data_to_buff
   LDA   #<BUFF_AREA
   STA   __zpabi_inc_sector_and_write__buffer_0
   LDA   #>BUFF_AREA
   STA   __zpabi_inc_sector_and_write__buffer_1
   JSR   inc_sector_and_write
   BCS   .instr_save@fail
   LDA   #$80
   STA   __zpabi_copy_data_to_buff__src_0
   LDA   #$03
   STA   __zpabi_copy_data_to_buff__src_1
   LDX   #$00
   LDY   #$68
   JSR   copy_data_to_buff
   LDA   #<BUFF_AREA
   STA   __zpabi_inc_sector_and_write__buffer_0
   LDA   #>BUFF_AREA
   STA   __zpabi_inc_sector_and_write__buffer_1
   JSR   inc_sector_and_write
   BCS   .instr_save@fail
   LDA   Z_HEADER_ADDR
   STA   __zpabi_inc_sector_and_write__buffer_0
   LDA   Z_HEADER_ADDR+1
   STA   __zpabi_inc_sector_and_write__buffer_1
   LDY   #$0E
   LDA   (Z_HEADER_ADDR),Y
   ADC   #$01
   STA   __local_instr_save__pages
.loop@2_start:
   JSR   inc_sector_and_write
   BCS   .instr_save@fail
   INC   __zpabi_inc_sector_and_write__buffer_1
   DEC   __local_instr_save__pages
   BNE   .loop@2_start
   JSR   inc_sector_and_write
   BCS   .instr_save@fail
   JSR   please_reinsert_game_diskette
   LDA   #$01
   STA   __zpabi_branch__condition
   JMP   branch
.instr_save@fail:
   JSR   please_reinsert_game_diskette
   LDA   #$00
   STA   __zpabi_branch__condition
   JMP   branch

instr_restore:
   SUBROUTINE

   JSR   please_insert_save_diskette
   STA   save_sector
   STX   save_sector+1
   LDA   #<BUFF_AREA
   STA   __zpabi_inc_sector_and_read__buffer_0
   LDA   #>BUFF_AREA
   STA   __zpabi_inc_sector_and_read__buffer_1
   JSR   inc_sector_and_read
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
   LDA   #<BUFF_AREA
   STA   __zpabi_inc_sector_and_read__buffer_0
   LDA   #>BUFF_AREA
   STA   __zpabi_inc_sector_and_read__buffer_1
   JSR   inc_sector_and_read
   BCS   .instr_restore@fail
   LDA   #$80
   STA   __zpabi_copy_data_from_buff__dest_0
   LDA   #$02
   STA   __zpabi_copy_data_from_buff__dest_1
   LDX   #$00
   LDY   #$00
   JSR   copy_data_from_buff
   LDA   #<BUFF_AREA
   STA   __zpabi_inc_sector_and_read__buffer_0
   LDA   #>BUFF_AREA
   STA   __zpabi_inc_sector_and_read__buffer_1
   JSR   inc_sector_and_read
   BCS   .instr_restore@fail
   LDA   #$80
   STA   __zpabi_copy_data_from_buff__dest_0
   LDA   #$03
   STA   __zpabi_copy_data_from_buff__dest_1
   LDX   #$00
   LDY   #$68
   JSR   copy_data_from_buff
   LDA   Z_HEADER_ADDR
   STA   __zpabi_inc_sector_and_read__buffer_0
   LDA   Z_HEADER_ADDR+1
   STA   __zpabi_inc_sector_and_read__buffer_1
   LDY   #$0E
   LDA   (Z_HEADER_ADDR),Y
   CLC
   ADC   #$01
   STA   __local_instr_restore__pages
.loop@3_start:
   JSR   inc_sector_and_read
   BCS   .instr_restore@fail
   INC   __zpabi_inc_sector_and_read__buffer_1
   DEC   __local_instr_restore__pages
   BNE   .loop@3_start
   LDA   __local_instr_restore__1
   LDY   #$11
   STA   (Z_HEADER_ADDR),Y
   JSR   please_reinsert_game_diskette
   LDA   #$01
   STA   __zpabi_branch__condition
   JMP   branch
.instr_restore@fail:
   JSR   please_reinsert_game_diskette
   LDA   #$00
   STA   __zpabi_branch__condition
   JMP   branch

__zp_init:
   SUBROUTINE

   LDA   #$00
   STA   save_sector
   STA   save_sector+1
   RTS
