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
Z_PC2_H	EQU	$91
Z_PC2_HH	EQU	$92
Z_PC2_L	EQU	$93
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
__local_print_zstring__ch	EQU	$F3
__local_print_zstring__c_0	EQU	$F4
__local_print_zstring__0	EQU	$F5
__local_buffer_char__0	EQU	$F6
__local_get_next_zchar__compound_literal_1	EQU	$F6
__local_dump_buffer_with_more__0	EQU	$F7
__local_get_next_code_word__1	EQU	$F7
__local_dump_buffer_with_more__end	EQU	$F8
__local_get_next_code_byte2__found_1	EQU	$F8
__local_dump_buffer_to_printer__x	EQU	$F9
__local_find_index_of_page_table__0	EQU	$F9
__local_set_page_first__0	EQU	$F9
__local_dump_buffer_to_printer__1	EQU	$FA
__local_find_index_of_page_table__1	EQU	$FA
__local_set_page_first__prev_first	EQU	$FA
__local_dump_buffer_to_printer__2	EQU	$FB
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
; def A_mod_3 params=__zpabi_A_mod_3__a locals= indirect=false in_cycle=false param_regs=A
; def buffer_char params=__zpabi_buffer_char__c locals=__local_buffer_char__0 indirect=false in_cycle=false param_regs=A
; def cout_string params=__zpabi_cout_string__s_0,__zpabi_cout_string__s_1,__zpabi_cout_string__count locals= indirect=false in_cycle=false param_regs=,,X pinned=$E6,$E7,
; def do_rwts_on_sector params=__zpabi_do_rwts_on_sector__command locals= indirect=false in_cycle=false param_regs=A
; def dump_buffer_line params= locals= indirect=false in_cycle=false
; def dump_buffer_to_printer params= locals=__local_dump_buffer_to_printer__x,__local_dump_buffer_to_printer__1,__local_dump_buffer_to_printer__2 indirect=false in_cycle=false
; def dump_buffer_to_screen params= locals= indirect=false in_cycle=false
; def dump_buffer_with_more params= locals=__local_dump_buffer_with_more__0,__local_dump_buffer_with_more__end indirect=false in_cycle=false
; def find_index_of_page_table params= locals=__local_find_index_of_page_table__0,__local_find_index_of_page_table__1 indirect=false in_cycle=false
; def get_alphabet params= locals= indirect=false in_cycle=false
; def get_next_code_byte2 params= locals=__local_get_next_code_byte2__found_1 indirect=false in_cycle=false
; def get_next_code_word params= locals=__local_get_next_code_word__1 indirect=false in_cycle=false
; def get_next_zchar params= locals=__local_get_next_zchar__compound_literal_1 indirect=false in_cycle=false
; def invalidate_zcode_page2 params= locals= indirect=false in_cycle=false
; def load_packed_address params= locals= indirect=false in_cycle=false
; def print_string_literal params= locals= indirect=false in_cycle=false
; def print_zstring params= locals=__local_print_zstring__ch,__local_print_zstring__c_0,__local_print_zstring__0 indirect=false in_cycle=true
; def read_from_sector params= locals= indirect=false in_cycle=false
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
; ext find_index_of_page_table params=
; ext get_alphabet params=
; ext get_next_code_byte2 params=
; ext get_next_code_word params=
; ext get_next_zchar params=
; ext invalidate_zcode_page2 params=
; ext load_packed_address params=
; ext print_string_literal params=
; ext print_zstring params=
; ext read_from_sector params=
; ext restart params=
; ext set_page_first params=__zpabi_set_page_first__slot param_regs=A
; call buffer_char -> dump_buffer_with_more
; call cout_string -> COUT1
; call do_rwts_on_sector -> RWTS
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
; call get_next_code_byte2 -> find_index_of_page_table
; call get_next_code_byte2 -> read_from_sector
; call get_next_code_byte2 -> restart
; call get_next_code_byte2 -> set_page_first
; call get_next_code_word -> get_next_code_byte2
; call get_next_zchar -> get_next_code_word
; call load_packed_address -> invalidate_zcode_page2
; call print_string_literal -> print_zstring
; call print_zstring -> A_mod_3
; call print_zstring -> buffer_char
; call print_zstring -> get_alphabet
; call print_zstring -> get_next_zchar
; call print_zstring -> load_packed_address
; call print_zstring -> print_zstring saved
; call read_from_sector -> do_rwts_on_sector
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
; zp Z_PC2_H size=1 at=$91 def=false init=
; zp Z_PC2_HH size=1 at=$92 def=false init=
; zp Z_PC2_L size=1 at=$93 def=false init=
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


   SEG   literal
   ORG   $0C5B
__literal_start:

print_string_literal:
   SUBROUTINE

   LDA   Z_PC
   STA   Z_PC2_L
   LDA   Z_PC+1
   STA   Z_PC2_H
   LDA   Z_PC+2
   STA   Z_PC2_HH
   LDA   #$00
   STA   ZCODE_PAGE_VALID2
   JSR   print_zstring
   LDA   Z_PC2_L
   STA   Z_PC
   LDA   Z_PC2_H
   STA   Z_PC+1
   LDA   Z_PC2_HH
   STA   Z_PC+2
   LDA   ZCODE_PAGE_VALID2
   STA   ZCODE_PAGE_VALID
   LDA   ZCODE_PAGE_ADDR2
   STA   ZCODE_PAGE_ADDR
   LDA   ZCODE_PAGE_ADDR2+1
   STA   ZCODE_PAGE_ADDR+1
   RTS
__literal_end:
__literal_load	EQU	__literal_start
   IF    __literal_end > $17DD
   ECHO  "section .literal runs past the end of region literal by", [__literal_end - $17DD]d, "bytes"
   ERR
   ENDIF

   SEG   pages
   ORG   $17DD
__pages_start:

invalidate_zcode_page2:
   SUBROUTINE

   LDA   #$00
   STA   ZCODE_PAGE_VALID2
   RTS

load_packed_address:
   SUBROUTINE

   LDA   SCRATCH2
   ASL
   STA   Z_PC2_L
   LDA   SCRATCH2+1
   ROL
   STA   Z_PC2_H
   LDA   #$00
   ROL
   STA   Z_PC2_HH
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
   LDY   Z_PC2_L
   LDA   (ZCODE_PAGE_ADDR2),Y
   INY
   STY   Z_PC2_L
   BNE   .if_end@1
   LDY   #$00
   STY   ZCODE_PAGE_VALID2
   INC   Z_PC2_H
   BNE   .if_end@2
   INC   Z_PC2_HH
.if_end@2:
.if_end@1:
   RTS
.if_end@0:
   LDA   Z_PC2_HH
   BNE   .if_else@6
   LDA   Z_PC2_H
   CMP   NUM_IMAGE_PAGES
   BCC   .if_end@5
.if_else@6:
   LDA   Z_PC2_H
   STA   SCRATCH2
   LDA   Z_PC2_HH
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
   LDA   Z_PC2_H
   STA   SCRATCH1
   LDA   Z_PC2_HH
   STA   SCRATCH1+1
   JSR   read_from_sector
   BCC   .if_end@9
   JSR   restart
.if_end@9:
   LDA   Z_PC2_H
   LDY   PAGE_TABLE_INDEX2
   STA   (PAGE_L_TABLE),Y
   LDA   Z_PC2_HH
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
__pages_end:
__pages_load	EQU	__pages_start
   IF    __pages_end > $1B90
   ECHO  "section .pages runs past the end of region zstring by", [__pages_end - $1B90]d, "bytes"
   ERR
   ENDIF

   SEG   zstring
   ORG   __pages_end
__zstring_start:

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
   STA   __local_print_zstring__0
   LDA   Z_ABBREV_TABLE
   CLC
   ADC   __local_print_zstring__ch
   STA   __local_print_zstring__ch
   LDA   Z_ABBREV_TABLE+1
   ADC   #$00
   STA   __local_print_zstring__c_0
   LDY   #$01
   LDA   (__local_print_zstring__ch),Y
   STA   SCRATCH2
   LDA   __local_print_zstring__0
   STA   SCRATCH2+1
   LDA   LOCKED_ALPHABET
   PHA
   LDA   ZDECOMPRESS_STATE
   PHA
   LDA   ZCHARS
   PHA
   LDA   ZCHARS+1
   PHA
   LDA   Z_PC2_L
   PHA
   LDA   Z_PC2_H
   PHA
   LDA   Z_PC2_HH
   PHA
   JSR   load_packed_address
   JSR   print_zstring
   PLA
   STA   Z_PC2_HH
   PLA
   STA   Z_PC2_H
   PLA
   STA   Z_PC2_L
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
   CLC
   ADC   __local_print_zstring__c_0
   JMP   .if_end@9
.if_else@10:
   CMP   #$01
   BNE   .if_else@12
   LDA   #$3B
   CLC
   ADC   __local_print_zstring__c_0
   JMP   .if_end@11
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
   JMP   .if_end@15
.if_else@16:
   SEC
   SBC   #$08
   TAX
   LDA   a2_table,X
.if_end@15:
.if_end@13:
.if_end@11:
.if_end@9:
.if_end@1:
   JSR   buffer_char
   JMP   .loop@0_start

A_mod_3:
   SUBROUTINE

.loop@1_continue:
   CMP   #$03
   BCC   .loop@1_break
   SBC   #$03
   JMP   .loop@1_continue
.loop@1_break:
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
   LDA   SCRATCH2
   STA   ZCHARS
   LDA   SCRATCH2+1
   STA   ZCHARS+1
   LSR
   LSR
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
   AND   #$1F
   JMP   .if_end@21
.if_else@22:
   LDA   ZCHARS+1
   BPL   .cond_else@23
   LDA   #$FF
   JMP   .cond_end@24
.cond_else@23:
   LDA   #$00
.cond_end@24:
   STA   ZDECOMPRESS_STATE
   LDA   ZCHARS
   AND   #$1F
.if_end@21:
.if_end@19:
   CLC
   RTS

a2_table:
   DC.B  $30, $31, $32, $33, $34, $35, $36, $37, $38, $39, $2E, $2C, $21, $3F, $5F, $23
   DC.B  $27, $22, $2F, $5C, $2D, $3A, $28, $29
__zstring_end:
__zstring_load	EQU	__zstring_start
   IF    __zstring_end > $1B90
   ECHO  "section .zstring runs past the end of region zstring by", [__zstring_end - $1B90]d, "bytes"
   ERR
   ENDIF

   SEG   output
   ORG   $1B90
__output_start:

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
   AND   #$01
   BEQ   .if_end@9
   JSR   dump_buffer_to_printer
.if_end@9:
   JMP   dump_buffer_to_screen

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
.loop@2_start:
   CPX   BUFF_END
   BEQ   .loop@2_break
   LDA   BUFF_AREA,X
   STX   __local_dump_buffer_to_printer__x
   JSR   COUT
   LDX   __local_dump_buffer_to_printer__x
   INX
   JMP   .loop@2_start
.loop@2_break:
   LDA   CSW
   STA   PRINTER_CSW
   LDA   CSW+1
   STA   PRINTER_CSW+1
   LDA   __local_dump_buffer_to_printer__2
   STA   CSW
   LDA   __local_dump_buffer_to_printer__1
   STA   CSW+1
   RTS

dump_buffer_to_screen:
   SUBROUTINE

   LDX   #$00
.loop@3_start:
   CPX   BUFF_END
   BEQ   .loop@3_break
   LDA   BUFF_AREA,X
   JSR   COUT1
   INX
   JMP   .loop@3_start
.loop@3_break:
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
   AND   #$01
   BEQ   .if_end@13
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

printer_card_initialized_flag:
   DS.B  1

string_more:
   DC.B  $5B, $4D, $4F, $52, $45, $5D
__output_end:
__output_load	EQU	__output_start
   IF    __output_end > $1DAC
   ECHO  "section .output runs past the end of region output by", [__output_end - $1DAC]d, "bytes"
   ERR
   ENDIF

   SEG   disk
   ORG   $1DAC
__disk_start:

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

do_rwts_on_sector:
   SUBROUTINE

   STA   iob+12
   LDA   SCRATCH2
   STA   iob+8
   LDA   SCRATCH2+1
   STA   iob+9
   LDA   #$03
   STA   iob+4
   LDA   SCRATCH1
   LDX   SCRATCH1+1
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
__disk_end:
__disk_load	EQU	__disk_start
   IF    __disk_end > $2200
   ECHO  "section .disk runs past the end of region disk by", [__disk_end - $2200]d, "bytes"
   ERR
   ENDIF

   SEG   text
   ORG   __disk_end
__text_start:
__text_end:
__text_load	EQU	__text_start
   IF    __text_end > $2200
   ECHO  "section .text runs past the end of region disk by", [__text_end - $2200]d, "bytes"
   ERR
   ENDIF

   SEG   rodata
   ORG   __text_end
__rodata_start:
__rodata_end:
__rodata_load	EQU	__rodata_start
   IF    __rodata_end > $2200
   ECHO  "section .rodata runs past the end of region disk by", [__rodata_end - $2200]d, "bytes"
   ERR
   ENDIF

   SEG   data
   ORG   __rodata_end
__data_start:
__data_end:
__data_load	EQU	__data_start
   IF    __data_end > $2200
   ECHO  "section .data runs past the end of region disk by", [__data_end - $2200]d, "bytes"
   ERR
   ENDIF

   SEG   bss
   ORG   __data_end
__bss_start:
__bss_end:
__bss_load	EQU	__bss_start
   IF    __bss_end > $2200
   ECHO  "section .bss runs past the end of region disk by", [__bss_end - $2200]d, "bytes"
   ERR
   ENDIF
