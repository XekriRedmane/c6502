HARGS	EQU	$00
SSP	EQU	$01
FP	EQU	$03
DPTR	EQU	$05
disk_sector	EQU	$07
disk_buffer	EQU	$09
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
__zpabi_print_ascii_string__s_0	EQU	$E0
__zpabi_cout_string__s_1	EQU	$E1
__zpabi_print_ascii_string__s_1	EQU	$E1
Z_ABBREV_TABLE	EQU	$E2
__local_write_to_sector__1	EQU	$E5
__local_write_to_sector__2	EQU	$E6
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
; def do_rwts_on_sector params=__zpabi_do_rwts_on_sector__command locals=__local_do_rwts_on_sector__command,__local_do_rwts_on_sector__s indirect=false in_cycle=false param_regs=A carry=true
; def inc_sector_and_read params= locals=__local_inc_sector_and_read__0 indirect=false in_cycle=false carry=true
; def inc_sector_and_write params= locals=__local_inc_sector_and_write__0 indirect=false in_cycle=false carry=true
; def read_from_sector params= locals=__local_read_from_sector__0 indirect=false in_cycle=false carry=true
; def read_next_sector params= locals=__local_read_next_sector__0 indirect=false in_cycle=false carry=true
; def write_next_sector params= locals=__local_write_next_sector__0 indirect=false in_cycle=false carry=true
; def write_to_sector params= locals=__local_write_to_sector__0,__local_write_to_sector__1,__local_write_to_sector__2 indirect=false in_cycle=false carry=true
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
; ext load_address params=__zpabi_load_address__addr_0,__zpabi_load_address__addr_1 param_regs=A,X
; ext load_packed_address params=__zpabi_load_packed_address__addr_0,__zpabi_load_packed_address__addr_1 param_regs=A,X
; ext new_line params=
; ext please_insert_save_diskette params=
; ext please_reinsert_game_diskette params=
; ext print_ascii_string params=__zpabi_print_ascii_string__s_0,__zpabi_print_ascii_string__s_1,__zpabi_print_ascii_string__count param_regs=,,X
; ext print_number params=__zpabi_print_number__n_0,__zpabi_print_number__n_1 param_regs=A,X
; ext print_obj params=__zpabi_print_obj__obj param_regs=A
; ext print_status_line params=
; ext print_string_literal params=
; ext print_zstring params=
; ext reset_window params=
; ext restart params=
; ext return_value params=__zpabi_return_value__value_0,__zpabi_return_value__value_1 param_regs=A,X
; ext store_result params=__zpabi_store_result__value_0,__zpabi_store_result__value_1 param_regs=A,X
; ext var_get params=__zpabi_var_get__var param_regs=A
; call do_rwts_on_sector -> RWTS
; call inc_sector_and_read -> read_from_sector
; call inc_sector_and_write -> write_to_sector
; call read_from_sector -> do_rwts_on_sector
; call read_next_sector -> inc_sector_and_read
; call write_next_sector -> inc_sector_and_write
; call write_to_sector -> do_rwts_on_sector
; item do_rwts_on_sector section=.text local=true zero=false
; item read_from_sector section=.text local=false zero=false
; item inc_sector_and_read section=.text local=false zero=false
; item read_next_sector section=.text local=false zero=false
; item read_next_sector@run0 section=.text local=true zero=false
; item write_to_sector section=.text local=false zero=false
; item inc_sector_and_write section=.text local=false zero=false
; item write_next_sector section=.text local=false zero=false
; item iob section=.data local=false zero=false
; item dct section=.data local=false zero=false
; item __zp_init section=.text local=false zero=false
; zp disk_sector size=2 at=auto def=true init=$00,$00
; zp disk_buffer size=2 at=auto def=true init=$00,$00
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

read_from_sector:
   SUBROUTINE

   LDA   #$01
   JMP   do_rwts_on_sector

inc_sector_and_read:
   SUBROUTINE

   INC   disk_sector
   BNE   .inc_done@0
   INC   disk_sector+1
.inc_done@0:
   JMP   read_from_sector

read_next_sector:
   SUBROUTINE

   JSR   read_next_sector@run0
   JMP   inc_sector_and_read

read_next_sector@run0:
   SUBROUTINE

   LDA   #<BUFF_AREA
   STA   disk_buffer
   LDA   #>BUFF_AREA
   STA   disk_buffer+1
   RTS

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

inc_sector_and_write:
   SUBROUTINE

   INC   disk_sector
   BNE   .inc_done@1
   INC   disk_sector+1
.inc_done@1:
   JMP   write_to_sector

write_next_sector:
   SUBROUTINE

   JSR   read_next_sector@run0
   JMP   inc_sector_and_write

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

__zp_init:
   SUBROUTINE

   LDA   #$00
   STA   disk_sector
   STA   disk_sector+1
   STA   disk_buffer
   STA   disk_buffer+1
   RTS
