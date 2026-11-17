HARGS	EQU	$00
SSP	EQU	$05
FP	EQU	$07
DPTR	EQU	$09
remainder	EQU	$0B
sign_bit	EQU	$0D
__local_divu16__partial_1	EQU	$0F
__local_divu16__partial_0	EQU	$10
__local_divu16__0	EQU	$11
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
__zpabi_check_sign__a_0	EQU	$E0
__zpabi_cout_string__s_0	EQU	$E0
__zpabi_load_address__addr_0	EQU	$E0
__zpabi_load_packed_address__addr_0	EQU	$E0
__zpabi_print_ascii_string__s_0	EQU	$E0
__zpabi_print_number__n_0	EQU	$E0
__zpabi_return_value__value_0	EQU	$E0
__zpabi_store_with_sign__n_0	EQU	$E0
__zpabi_check_sign__a_1	EQU	$E1
__zpabi_cout_string__s_1	EQU	$E1
__zpabi_load_address__addr_1	EQU	$E1
__zpabi_load_packed_address__addr_1	EQU	$E1
__zpabi_print_ascii_string__s_1	EQU	$E1
__zpabi_print_number__n_1	EQU	$E1
__zpabi_return_value__value_1	EQU	$E1
__zpabi_store_with_sign__n_1	EQU	$E1
Z_ABBREV_TABLE	EQU	$E2
__zpabi_check_sign__b_0	EQU	$E4
__zpabi_check_sign__b_1	EQU	$E5
__zpabi_divu16__dividend_0	EQU	$E6
__zpabi_mulu16__a_0	EQU	$E6
__zpabi_store_result__value_0	EQU	$E6
__zpabi_divu16__dividend_1	EQU	$E7
__zpabi_mulu16__a_1	EQU	$E7
__zpabi_store_result__value_1	EQU	$E7
__zpabi_divu16__divisor_0	EQU	$E8
__zpabi_mulu16__b_0	EQU	$E8
__zpabi_divu16__divisor_1	EQU	$E9
__zpabi_mulu16__b_1	EQU	$E9
BUFF_END	EQU	$EB
BUFF_LINE_LEN	EQU	$EC
CURR_LINE	EQU	$ED
PRINTER_CSW	EQU	$EE
__local_print_number__count	EQU	$F2
__local_print_number__digits	EQU	$F6
__local_mulu16__product	EQU	$F7
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
; def check_sign params=__zpabi_check_sign__a_0,__zpabi_check_sign__a_1,__zpabi_check_sign__b_0,__zpabi_check_sign__b_1 locals=__local_check_sign__a indirect=false in_cycle=false changes=__zpabi_check_sign__a_0
; def divu16 params=__zpabi_divu16__dividend_0,__zpabi_divu16__dividend_1,__zpabi_divu16__divisor_0,__zpabi_divu16__divisor_1 locals=__local_divu16__fits,__local_divu16__partial_1,__local_divu16__partial_0,__local_divu16__0,__local_divu16__i,__local_divu16__quotient indirect=false in_cycle=false changes=__zpabi_divu16__dividend_0
; def instr_div params= locals=__local_instr_div__quotient,__local_instr_div__0,__local_instr_div__1 indirect=false in_cycle=false
; def instr_mod params= locals=__local_instr_mod__0,__local_instr_mod__1 indirect=false in_cycle=false
; def instr_mul params= locals=__local_instr_mul__product,__local_instr_mul__0,__local_instr_mul__1 indirect=false in_cycle=false
; def instr_random params= locals=__local_instr_random__0,__local_instr_random__1 indirect=false in_cycle=false
; def mulu16 params=__zpabi_mulu16__a_0,__zpabi_mulu16__a_1,__zpabi_mulu16__b_0,__zpabi_mulu16__b_1 locals=__local_mulu16__b_0,__local_mulu16__b_1,__local_mulu16__a_0,__local_mulu16__a_1,__local_mulu16__product indirect=false in_cycle=false changes=__zpabi_mulu16__a_0,__zpabi_mulu16__a_1,__zpabi_mulu16__b_0,__zpabi_mulu16__b_1
; def print_number params=__zpabi_print_number__n_0,__zpabi_print_number__n_1 locals=__local_print_number__0,__local_print_number__1,__local_print_number__count,__local_print_number__2,__local_print_number__n_1,__local_print_number__n_0,__local_print_number__digits,__local_print_number__4,__local_print_number__5,__local_print_number__6,__local_print_number__7 indirect=false in_cycle=false changes=__zpabi_print_number__n_0,__zpabi_print_number__n_1
; def store_with_sign params=__zpabi_store_with_sign__n_0,__zpabi_store_with_sign__n_1 locals=__local_store_with_sign__n_1,__local_store_with_sign__n_0 indirect=false in_cycle=false
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
; ext instr_get_child params=
; ext instr_get_next_prop params=
; ext instr_get_parent params=
; ext instr_get_prop params=
; ext instr_get_prop_addr params=
; ext instr_get_prop_len params=
; ext instr_get_sibling params=
; ext instr_insert_obj params=
; ext instr_jin params=
; ext instr_put_prop params=
; ext instr_quit params=
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
; ext print_obj params=__zpabi_print_obj__obj param_regs=A
; ext print_status_line params=
; ext print_string_literal params=
; ext print_zstring params=
; ext read_from_sector params= carry=true
; ext read_next_sector params= carry=true
; ext reset_window params=
; ext restart params=
; ext return_value params=__zpabi_return_value__value_0,__zpabi_return_value__value_1
; ext store_result params=__zpabi_store_result__value_0,__zpabi_store_result__value_1
; ext var_get params=__zpabi_var_get__var param_regs=A
; ext write_next_sector params= carry=true
; ext write_to_sector params= carry=true
; call instr_div -> check_sign
; call instr_div -> divu16
; call instr_div -> store_with_sign
; call instr_mod -> check_sign
; call instr_mod -> divu16
; call instr_mod -> store_result
; call instr_mul -> check_sign
; call instr_mul -> mulu16
; call instr_mul -> store_with_sign
; call instr_random -> divu16
; call instr_random -> store_result
; call print_number -> buffer_char
; call print_number -> divu16
; call store_with_sign -> store_result
; share member=check_sign:__zpabi_check_sign__b_0,__zpabi_check_sign__b_1 holder=instr_div holder=instr_mod holder=instr_mul live=instr_div:check_sign live=instr_mod:check_sign live=instr_mul:check_sign forward=instr_div>check_sign forward=instr_mod>check_sign forward=instr_mul>check_sign writer=instr_div writer=instr_mod writer=instr_mul
; item mulu16 section=.text local=true zero=false
; item divu16 section=.text local=true zero=false
; item check_sign section=.text local=true zero=false
; item store_with_sign section=.text local=true zero=false
; item instr_mul section=.text local=false zero=false
; item instr_div section=.text local=false zero=false
; item instr_mod section=.text local=false zero=false
; item instr_random section=.text local=false zero=false
; item print_number section=.text local=false zero=false
; item __zp_init section=.text local=false zero=false
; zp remainder size=2 at=auto def=true init=$00,$00 weight=11 local=true
; zp sign_bit size=1 at=auto def=true init=$00 weight=3 local=true
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

mulu16:
   SUBROUTINE

   LDA   #$00
   STA   __local_mulu16__product
   LDX   #$00
   BEQ   .loop@0_continue
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
   BEQ   .if_end@1

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

store_with_sign:
   SUBROUTINE

   LDA   sign_bit
   BPL   .store_with_sign@asm_ssa_split@0
   SEC
   LDA   #$00
   SBC   __zpabi_store_with_sign__n_0
   STA   __zpabi_store_result__value_0
   LDA   #$00
   SBC   __zpabi_store_with_sign__n_1
.if_end@3:
   STA   __zpabi_store_result__value_1
   JMP   store_result
.store_with_sign@asm_ssa_split@0:
   LDA   __zpabi_store_with_sign__n_0
   STA   __zpabi_store_result__value_0
   LDA   __zpabi_store_with_sign__n_1
   JMP   .if_end@3

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
   JMP   store_with_sign
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
   JMP   store_with_sign

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
   JMP   store_with_sign
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
.loop@2_start:
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
   LDA   __zpabi_print_number__n_0
   ORA   __zpabi_print_number__n_1
   BNE   .loop@2_start
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

__zp_init:
   SUBROUTINE

   LDA   #$00
   STA   sign_bit
   STA   remainder
   STA   remainder+1
   RTS
