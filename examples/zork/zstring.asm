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
__local_print_zstring__ch	EQU	$E0
__zpabi_cout_string__s_0	EQU	$E0
__zpabi_print_ascii_string__s_0	EQU	$E0
__local_print_zstring__c_0	EQU	$E1
__zpabi_cout_string__s_1	EQU	$E1
__zpabi_print_ascii_string__s_1	EQU	$E1
Z_ABBREV_TABLE	EQU	$E2
__local_get_next_zchar__compound_literal_1	EQU	$E4
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
; def A_mod_3 params=__zpabi_A_mod_3__a locals=__local_A_mod_3__a indirect=false in_cycle=false param_regs=A
; def get_alphabet params= locals=__local_get_alphabet__shift indirect=false in_cycle=false
; def get_next_zchar params= locals=__local_get_next_zchar__compound_literal_1,__local_get_next_zchar__code indirect=false in_cycle=false carry=true
; def print_string_literal params= locals= indirect=false in_cycle=false
; def print_zstring params= locals=__local_print_zstring__ch,__local_print_zstring__c_0 indirect=false in_cycle=true
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
; ext get_next_code_byte params=
; ext get_next_code_byte2 params=
; ext get_next_code_word params=
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
; ext load_address params=__zpabi_load_address__addr_0,__zpabi_load_address__addr_1 param_regs=A,X
; ext load_packed_address params=__zpabi_load_packed_address__addr_0,__zpabi_load_packed_address__addr_1 param_regs=A,X
; ext new_line params=
; ext please_insert_save_diskette params=
; ext please_reinsert_game_diskette params=
; ext print_ascii_string params=__zpabi_print_ascii_string__s_0,__zpabi_print_ascii_string__s_1,__zpabi_print_ascii_string__count param_regs=,,X
; ext print_number params=__zpabi_print_number__n_0,__zpabi_print_number__n_1 param_regs=A,X
; ext print_obj params=__zpabi_print_obj__obj param_regs=A
; ext print_status_line params=
; ext read_from_sector params= carry=true
; ext read_next_sector params= carry=true
; ext reset_window params=
; ext restart params=
; ext return_value params=__zpabi_return_value__value_0,__zpabi_return_value__value_1 param_regs=A,X
; ext store_result params=__zpabi_store_result__value_0,__zpabi_store_result__value_1 param_regs=A,X
; ext var_get params=__zpabi_var_get__var param_regs=A
; ext write_next_sector params= carry=true
; ext write_to_sector params= carry=true
; call get_next_zchar -> get_next_code_word
; call print_string_literal -> print_zstring
; call print_zstring -> A_mod_3
; call print_zstring -> buffer_char
; call print_zstring -> get_alphabet
; call print_zstring -> get_next_zchar
; call print_zstring -> load_packed_address
; call print_zstring -> print_zstring saved
; item print_zstring section=.text local=false zero=false
; item A_mod_3 section=.text local=false zero=false
; item get_alphabet section=.text local=false zero=false
; item get_next_zchar section=.text local=false zero=false
; item print_string_literal section=.text local=false zero=false
; item a2_table section=.rodata local=false zero=false
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
   STA   __local_print_zstring__c_0
   INY
   LDA   (Z_ABBREV_TABLE),Y
   STA   __local_print_zstring__ch
   LDX   __local_print_zstring__c_0
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
   LDA   __local_print_zstring__ch
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
   BNE   .xjump@0
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
   BNE   .if_end@15
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
.xjump@1:
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
   JMP   .xjump@1
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

a2_table:
   DC.B  $30, $31, $32, $33, $34, $35, $36, $37, $38, $39, $2E, $2C, $21, $3F, $5F, $23
   DC.B  $27, $22, $2F, $5C, $2D, $3A, $28, $29
