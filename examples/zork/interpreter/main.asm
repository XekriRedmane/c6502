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
__zpabi_cout_string__s_0	EQU	$E0
__zpabi_load_address__addr_0	EQU	$E0
__zpabi_load_packed_address__addr_0	EQU	$E0
__zpabi_print_ascii_string__s_0	EQU	$E0
__zpabi_print_number__n_0	EQU	$E0
__zpabi_push__value_0	EQU	$E0
__zpabi_return_value__value_0	EQU	$E0
__zpabi_store_result__value_0	EQU	$E0
__zpabi_var_put__value_0	EQU	$E0
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
__local_main__0	EQU	$E4
__local_main__sector	EQU	$E5
__local_locate_last_ram_page__2	EQU	$E8
__local_locate_last_ram_page__3	EQU	$E9
__local_locate_last_ram_page__page	EQU	$EA
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
; def instr_quit params= locals= indirect=false in_cycle=false
; def instr_restart params= locals= indirect=false in_cycle=false
; def internal_error params= locals= indirect=false in_cycle=false
; def locate_last_ram_page params= locals=__local_locate_last_ram_page__0,__local_locate_last_ram_page__1,__local_locate_last_ram_page__2,__local_locate_last_ram_page__3,__local_locate_last_ram_page__page indirect=false in_cycle=false
; def main params= locals=__local_main__0,__local_main__sector indirect=false in_cycle=false
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
; ext get_next_zchar params=
; ext get_object_addr params=__zpabi_get_object_addr__obj param_regs=A
; ext home params=
; ext inc_sector_and_read params=
; ext inc_sector_and_write params=
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
; ext instr_random params=
; ext instr_remove_obj params=
; ext instr_restore params=
; ext instr_save params=
; ext instr_set_attr params=
; ext instr_sread params=
; ext instr_test_attr params=
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
; ext read_from_sector params=
; ext read_next_sector params=
; ext reset_window params=
; ext restart params=
; ext return_value params=__zpabi_return_value__value_0,__zpabi_return_value__value_1
; ext store_result params=__zpabi_store_result__value_0,__zpabi_store_result__value_1
; ext var_get params=__zpabi_var_get__var param_regs=A
; ext var_put params=__zpabi_var_put__var,__zpabi_var_put__value_0,__zpabi_var_put__value_1 param_regs=A,,
; ext write_next_sector params=
; ext write_to_sector params=
; call instr_quit -> dump_buffer_with_more
; call instr_quit -> print_ascii_string
; call instr_restart -> dump_buffer_with_more
; call instr_restart -> restart
; call internal_error -> dump_buffer_with_more
; call internal_error -> print_ascii_string
; call main -> do_instruction
; call main -> internal_error
; call main -> locate_last_ram_page
; call main -> read_from_sector
; call main -> reset_window
; call main -> restart
; item locate_last_ram_page section=.text local=true zero=false
; item main section=.text.main local=false zero=false
; item instr_restart section=.text local=false zero=false
; item instr_quit section=.text local=false zero=false
; item internal_error section=.text local=false zero=false
; item page_tables section=.tables local=false zero=true
; item sEndOfSession section=.rodata local=true zero=false
; item sInternalError section=.rodata local=true zero=false
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
.loop@1_break:
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
.loop@3_break:
   LDY   #$01
   LDA   (Z_HEADER_ADDR),Y
   AND   #$01
   BEQ   .if_end@4
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

page_tables:
   DS.B  512

sEndOfSession:
   DC.B  $2D, $2D, $20, $45, $4E, $44, $20, $4F, $46, $20, $53, $45, $53, $53, $49, $4F
   DC.B  $4E, $20, $2D, $2D

sInternalError:
   DC.B  $5A, $4F, $52, $4B, $20, $49, $4E, $54, $45, $52, $4E, $41, $4C, $20, $45, $52
   DC.B  $52, $4F, $52, $21
