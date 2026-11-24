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
__zpabi_print_ascii_string__s_0	EQU	$E0
__zpabi_print_ascii_string__s_1	EQU	$E1
Z_ABBREV_TABLE	EQU	$E2
__zpabi_cout_string__s_0	EQU	$E4
__zpabi_cout_string__s_1	EQU	$E5
__local_print_ascii_string__count	EQU	$E7
__local_print_ascii_string__i	EQU	$E8
__local_buffer_char__0	EQU	$EA
BUFF_END	EQU	$EB
BUFF_LINE_LEN	EQU	$EC
CURR_LINE	EQU	$ED
PRINTER_CSW	EQU	$EE
__local_dump_buffer_with_more__0	EQU	$F0
__local_dump_buffer_with_more__end	EQU	$F1
__local_dump_buffer_to_printer__x	EQU	$F3
__local_dump_buffer_to_printer__1	EQU	$F4
__local_dump_buffer_to_printer__2	EQU	$F5
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
; def buffer_char params=__zpabi_buffer_char__c locals=__local_buffer_char__c,__local_buffer_char__0 indirect=false in_cycle=false param_regs=A
; def cout_string params=__zpabi_cout_string__s_0,__zpabi_cout_string__s_1,__zpabi_cout_string__count locals=__local_cout_string__0 indirect=false in_cycle=false param_regs=,,X
; def dump_buffer_line params= locals=__local_dump_buffer_line__0 indirect=false in_cycle=false
; def dump_buffer_to_printer params= locals=__local_dump_buffer_to_printer__0,__local_dump_buffer_to_printer__x,__local_dump_buffer_to_printer__1,__local_dump_buffer_to_printer__2 indirect=false in_cycle=false
; def dump_buffer_to_screen params= locals=__local_dump_buffer_to_screen__0 indirect=false in_cycle=false
; def dump_buffer_with_more params= locals=__local_dump_buffer_with_more__0,__local_dump_buffer_with_more__end indirect=false in_cycle=false
; def home params= locals= indirect=false in_cycle=false
; def new_line params= locals= indirect=false in_cycle=false
; def print_ascii_string params=__zpabi_print_ascii_string__s_0,__zpabi_print_ascii_string__s_1,__zpabi_print_ascii_string__count locals=__local_print_ascii_string__0,__local_print_ascii_string__count,__local_print_ascii_string__i indirect=false in_cycle=false param_regs=,,X
; def reset_window params= locals= indirect=false in_cycle=false
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
; ext do_instruction params=
; ext get_alphabet params=
; ext get_next_code_byte params=
; ext get_next_code_byte2 params=
; ext get_next_code_word params=
; ext get_next_zchar params= carry=true
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
; ext load_address params=__zpabi_load_address__addr_0,__zpabi_load_address__addr_1 param_regs=A,X
; ext load_packed_address params=__zpabi_load_packed_address__addr_0,__zpabi_load_packed_address__addr_1 param_regs=A,X
; ext please_insert_save_diskette params=
; ext please_reinsert_game_diskette params=
; ext print_number params=__zpabi_print_number__n_0,__zpabi_print_number__n_1 param_regs=A,X
; ext print_obj params=__zpabi_print_obj__obj param_regs=A
; ext print_status_line params=
; ext print_string_literal params=
; ext print_zstring params=
; ext read_from_sector params= carry=true
; ext read_next_sector params= carry=true
; ext restart params=
; ext return_value params=__zpabi_return_value__value_0,__zpabi_return_value__value_1 param_regs=A,X
; ext store_result params=__zpabi_store_result__value_0,__zpabi_store_result__value_1 param_regs=A,X
; ext var_get params=__zpabi_var_get__var param_regs=A
; ext write_next_sector params= carry=true
; ext write_to_sector params= carry=true
; call buffer_char -> dump_buffer_with_more
; call cout_string -> COUT1
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
; call home -> HOME
; call new_line -> buffer_char
; call print_ascii_string -> buffer_char
; call reset_window -> home
; item buffer_char section=.text local=false zero=false
; item dump_buffer_line section=.text local=false zero=false
; item dump_buffer_to_printer section=.text local=false zero=false
; item dump_buffer_to_printer@run1 section=.text local=true zero=false
; item dump_buffer_to_printer@run0 section=.text local=true zero=false
; item dump_buffer_to_screen section=.text local=false zero=false
; item dump_buffer_with_more section=.text local=false zero=false
; item new_line section=.text local=false zero=false
; item cout_string section=.text local=false zero=false
; item print_ascii_string section=.text local=false zero=false
; item home section=.text local=false zero=false
; item reset_window section=.text local=false zero=false
; item printer_card_initialized_flag section=.bss local=true zero=true
; item string_more section=.rodata local=true zero=false
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

dump_buffer_line:
   SUBROUTINE

   LDY   #$11
   LDA   (Z_HEADER_ADDR),Y
   LSR
   BCC   .if_end@9
   JSR   dump_buffer_to_printer
.if_end@9:
   JMP   dump_buffer_to_screen

dump_buffer_to_printer:
   SUBROUTINE

   LDA   CSW
   STA   __local_dump_buffer_to_printer__2
   LDA   CSW+1
   STA   __local_dump_buffer_to_printer__1
   JSR   dump_buffer_to_printer@run0
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
   JSR   dump_buffer_to_printer@run1
   LDA   __local_dump_buffer_to_printer__2
   STA   CSW
   LDA   __local_dump_buffer_to_printer__1
   STA   CSW+1
   RTS

dump_buffer_to_printer@run1:
   SUBROUTINE

   LDA   CSW
   STA   PRINTER_CSW
   LDA   CSW+1
   STA   PRINTER_CSW+1
   RTS

dump_buffer_to_printer@run0:
   SUBROUTINE

   LDA   PRINTER_CSW
   STA   CSW
   LDA   PRINTER_CSW+1
   STA   CSW+1
   RTS

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
   JSR   dump_buffer_to_printer@run0
   LDA   #$8D
   JSR   COUT
   JSR   dump_buffer_to_printer@run1
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
   JMP   buffer_char

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

home:
   SUBROUTINE

   JSR   HOME
   LDA   WNDTOP
   STA   CURR_LINE
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
   JMP   home

printer_card_initialized_flag:
   DS.B  1

string_more:
   DC.B  $5B, $4D, $4F, $52, $45, $5D
