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
__monitor_rnd	EQU	$4E
__zork_zp_7c	EQU	$7C
SECTORS_PER_TRACK	EQU	$7F
__zork_zp_80	EQU	$80
Z_PC	EQU	$8A
ZCODE_PAGE_ADDR	EQU	$8D
ZCODE_PAGE_VALID	EQU	$8F
PAGE_TABLE_INDEX	EQU	$90
Z_PC2	EQU	$91
ZCODE_PAGE_ADDR2	EQU	$94
ZCODE_PAGE_VALID2	EQU	$96
PAGE_TABLE_INDEX2	EQU	$97
__zork_zp_98	EQU	$98
__zork_zp_9a	EQU	$9A
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
__zork_zp_c8	EQU	$C8
SHIFT_ALPHABET	EQU	$CE
LOCKED_ALPHABET	EQU	$CF
ZDECOMPRESS_STATE	EQU	$D0
ZCHARS	EQU	$D1
__zork_zp_d3	EQU	$D3
Z_ABBREV_TABLE	EQU	$E2
SCRATCH1	EQU	$E4
SCRATCH2	EQU	$E6
__zpabi_cout_string__s_0	EQU	$E6
__zpabi_cout_string__s_1	EQU	$E7
__zork_zp_e8	EQU	$E8
BUFF_END	EQU	$EB
BUFF_LINE_LEN	EQU	$EC
CURR_LINE	EQU	$ED
PRINTER_CSW	EQU	$EE
__zork_zp_f0	EQU	$F0
__local_get_next_code_word__1	EQU	$F4
__local_get_next_code_byte2__found_1	EQU	$F5
__local_get_next_code_byte2__page	EQU	$F6
__local_get_next_code_byte2__0	EQU	$F8
__local_set_page_first__0	EQU	$F9
__local_set_page_first__prev_first	EQU	$FA
__local_find_index_of_page_table__0	EQU	$FB
__local_find_index_of_page_table__1	EQU	$FC
BUFF_AREA	EQU	$0200
SLOT1_SCRATCH	EQU	$0779
restart	EQU	$0800
RWTS	EQU	$2900
HOME	EQU	$FC58
CLREOL	EQU	$FC9C
WAIT	EQU	$FCA8
RDKEY	EQU	$FD0C
COUT	EQU	$FDED
COUT1	EQU	$FDF0

; @zp-link-meta-begin
; def find_index_of_page_table params= locals=__local_find_index_of_page_table__compound_literal_1,__local_find_index_of_page_table__compound_literal_0,__local_find_index_of_page_table__0,__local_find_index_of_page_table__1 indirect=false in_cycle=false
; def get_next_code_byte2 params= locals=__local_get_next_code_byte2__found_1,__local_get_next_code_byte2__page,__local_get_next_code_byte2__found_0,__local_get_next_code_byte2__0 indirect=false in_cycle=false
; def get_next_code_word params= locals=__local_get_next_code_word__0,__local_get_next_code_word__1 indirect=false in_cycle=false
; def invalidate_zcode_page2 params= locals= indirect=false in_cycle=false
; def load_packed_address params= locals= indirect=false in_cycle=false
; def set_page_first params=__zpabi_set_page_first__slot locals=__local_set_page_first__0,__local_set_page_first__prev_first indirect=false in_cycle=false param_regs=A
; ext A_mod_3 params=__zpabi_A_mod_3__a param_regs=A
; ext CLREOL params=
; ext COUT params=__zpabi_COUT__c param_regs=A
; ext COUT1 params=__zpabi_COUT1__c param_regs=A
; ext HOME params=
; ext RDKEY params=
; ext RWTS params=__zpabi_RWTS__iob_0,__zpabi_RWTS__iob_1 param_regs=Y,A
; ext WAIT params=__zpabi_WAIT__a param_regs=A
; ext buffer_char params=__zpabi_buffer_char__c param_regs=A
; ext cout_string params=__zpabi_cout_string__s_0,__zpabi_cout_string__s_1,__zpabi_cout_string__count param_regs=,,X pinned=$E6,$E7,
; ext do_rwts_on_sector params=__zpabi_do_rwts_on_sector__command param_regs=A
; ext dump_buffer_line params=
; ext dump_buffer_to_printer params=
; ext dump_buffer_to_screen params=
; ext dump_buffer_with_more params=
; ext get_alphabet params=
; ext get_next_zchar params=
; ext print_string_literal params=
; ext print_zstring params=
; ext read_from_sector params=
; ext restart params=
; call get_next_code_byte2 -> find_index_of_page_table
; call get_next_code_byte2 -> read_from_sector
; call get_next_code_byte2 -> restart
; call get_next_code_byte2 -> set_page_first
; call get_next_code_word -> get_next_code_byte2
; call load_packed_address -> invalidate_zcode_page2
; item invalidate_zcode_page2 section=.text local=false zero=false
; item load_packed_address section=.text local=false zero=false
; item get_next_code_word section=.text local=false zero=false
; item get_next_code_byte2 section=.text local=false zero=false
; item set_page_first section=.text local=false zero=false
; item find_index_of_page_table section=.text local=false zero=false
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
; zp __monitor_rnd size=2 at=$4E def=false init=
; zp __zork_zp_7c size=3 at=$7C def=false init=
; zp SECTORS_PER_TRACK size=1 at=$7F def=false init=
; zp __zork_zp_80 size=10 at=$80 def=false init=
; zp Z_PC size=3 at=$8A def=false init=
; zp ZCODE_PAGE_ADDR size=2 at=$8D def=false init=
; zp ZCODE_PAGE_VALID size=1 at=$8F def=false init=
; zp PAGE_TABLE_INDEX size=1 at=$90 def=false init=
; zp Z_PC2 size=3 at=$91 def=false init=
; zp ZCODE_PAGE_ADDR2 size=2 at=$94 def=false init=
; zp ZCODE_PAGE_VALID2 size=1 at=$96 def=false init=
; zp PAGE_TABLE_INDEX2 size=1 at=$97 def=false init=
; zp __zork_zp_98 size=2 at=$98 def=false init=
; zp __zork_zp_9a size=30 at=$9A def=false init=
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
; zp __zork_zp_c8 size=6 at=$C8 def=false init=
; zp SHIFT_ALPHABET size=1 at=$CE def=false init=
; zp LOCKED_ALPHABET size=1 at=$CF def=false init=
; zp ZDECOMPRESS_STATE size=1 at=$D0 def=false init=
; zp ZCHARS size=2 at=$D1 def=false init=
; zp __zork_zp_d3 size=15 at=$D3 def=false init=
; zp Z_ABBREV_TABLE size=2 at=$E2 def=false init=
; zp SCRATCH1 size=2 at=$E4 def=false init=
; zp SCRATCH2 size=2 at=$E6 def=false init=
; zp __zork_zp_e8 size=3 at=$E8 def=false init=
; zp BUFF_END size=1 at=$EB def=false init=
; zp BUFF_LINE_LEN size=1 at=$EC def=false init=
; zp CURR_LINE size=1 at=$ED def=false init=
; zp PRINTER_CSW size=2 at=$EE def=false init=
; zp __zork_zp_f0 size=3 at=$F0 def=false init=
; runtime hargs_size=1 hargs_need=0
; @zp-link-meta-end

invalidate_zcode_page2:
   SUBROUTINE

   LDA   #$00
   STA   ZCODE_PAGE_VALID2
   RTS

load_packed_address:
   SUBROUTINE

   LDA   SCRATCH2
   ASL
   STA   Z_PC2
   LDA   SCRATCH2+1
   ROL
   STA   Z_PC2+1
   LDA   #$00
   ROL
   STA   Z_PC2+2
   JMP   invalidate_zcode_page2

get_next_code_word:
   SUBROUTINE

   JSR   get_next_code_byte2
   STA   __local_get_next_code_word__1
   JSR   get_next_code_byte2
   STA   SCRATCH2
   LDA   __local_get_next_code_word__1
   STA   SCRATCH2+1
   RTS

get_next_code_byte2:
   SUBROUTINE

.loop@0_start:
   LDA   ZCODE_PAGE_VALID2
   BEQ   .if_end@0
   LDY   Z_PC2
   INC   Z_PC2
   BNE   .if_end@1
   INC   Z_PC2+1
   LDA   Z_PC2
   LDX   #$00
   STX   ZCODE_PAGE_VALID2
   ORA   Z_PC2+1
   BNE   .if_end@2
   INC   Z_PC2+2
.if_end@2:
.if_end@1:
   LDA   (ZCODE_PAGE_ADDR2),Y
   RTS
.if_end@0:
   LDA   Z_PC2+1
   STA   __local_get_next_code_byte2__page
   LDA   Z_PC2+2
   STA   __local_get_next_code_byte2__0
   BNE   .if_else@6
   LDA   __local_get_next_code_byte2__page
   CMP   NUM_IMAGE_PAGES
   BCC   .if_end@5
.if_else@6:
   LDA   __local_get_next_code_byte2__page
   STA   SCRATCH2
   LDA   __local_get_next_code_byte2__0
   STA   SCRATCH2+1
   JSR   find_index_of_page_table
   STA   PAGE_TABLE_INDEX2
   BCC   .if_end@7
   CMP   PAGE_TABLE_INDEX
   BNE   .if_end@8
   LDY   #$00
   STY   ZCODE_PAGE_VALID
.if_end@8:
   STA   __local_get_next_code_byte2__found_1
   LDA   HIGH_MEM_ADDR
   CLC
   ADC   #$00
   STA   SCRATCH2
   LDA   HIGH_MEM_ADDR+1
   ADC   __local_get_next_code_byte2__found_1
   STA   SCRATCH2+1
   LDA   __local_get_next_code_byte2__page
   STA   SCRATCH1
   LDA   __local_get_next_code_byte2__0
   STA   SCRATCH1+1
   JSR   read_from_sector
   BCC   .if_end@9
   JSR   restart
.if_end@9:
   LDA   __local_get_next_code_byte2__page
   LDY   PAGE_TABLE_INDEX2
   STA   (PAGE_L_TABLE),Y
   LDA   Z_PC2+2
   LDY   PAGE_TABLE_INDEX2
   STA   (PAGE_H_TABLE),Y
.if_end@7:
   LDA   PAGE_TABLE_INDEX2
   JSR   set_page_first
   LDA   PAGE_TABLE_INDEX2
   CLC
   ADC   NUM_IMAGE_PAGES
   CLC
.if_end@5:
   ADC   Z_HEADER_ADDR+1
   STA   ZCODE_PAGE_ADDR2+1
   LDA   #$00
   STA   ZCODE_PAGE_ADDR2
   LDA   #$FF
   STA   ZCODE_PAGE_VALID2
   JMP   .loop@0_start

set_page_first:
   SUBROUTINE

   TAY
   CPY   FIRST_Z_PAGE
   BNE   .if_end@10
   RTS
.if_end@10:
   LDA   FIRST_Z_PAGE
   STA   __local_set_page_first__prev_first
   STY   FIRST_Z_PAGE
   LDA   (NEXT_PAGE_TABLE),Y
   TAX
   LDA   __local_set_page_first__prev_first
   STA   (NEXT_PAGE_TABLE),Y
   LDA   (PREV_PAGE_TABLE),Y
   STA   __local_set_page_first__0
   LDA   #$FF
   STA   (PREV_PAGE_TABLE),Y
   TXA
   LDY   __local_set_page_first__0
   STA   (NEXT_PAGE_TABLE),Y
   LDA   FIRST_Z_PAGE
   LDY   __local_set_page_first__prev_first
   STA   (PREV_PAGE_TABLE),Y
   CPX   #$FF
   BNE   .if_else@12
   LDA   __local_set_page_first__0
   STA   LAST_Z_PAGE
   JMP   .if_end@11
.if_else@12:
   LDA   __local_set_page_first__0
   PHA
   TXA
   TAY
   PLA
   STA   (PREV_PAGE_TABLE),Y
.if_end@11:
   RTS

find_index_of_page_table:
   SUBROUTINE

   LDA   SCRATCH2
   STA   __local_find_index_of_page_table__1
   LDA   SCRATCH2+1
   STA   __local_find_index_of_page_table__0
   LDX   NUM_PAGE_TABLE_ENTRIES
   LDY   #$00
   LDA   __local_find_index_of_page_table__1
.loop@1_start:
   CMP   (PAGE_L_TABLE),Y
   BNE   .if_end@15
   LDA   (PAGE_H_TABLE),Y
   CMP   __local_find_index_of_page_table__0
   BNE   .find_index_of_page_table@lia@1
   CLC
   TYA
   RTS
.find_index_of_page_table@lia@1:
   LDA   __local_find_index_of_page_table__1
.if_end@15:
   INY
   DEX
   BNE   .loop@1_start
   SEC
   LDA   LAST_Z_PAGE
   RTS
