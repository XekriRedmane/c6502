HARGS	EQU	$00
SSP	EQU	$05
FP	EQU	$07
DPTR	EQU	$09
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
COLOR	EQU	$30
INVFLG	EQU	$32
PROMPT	EQU	$33
__monitor_ysav1	EQU	$35
CSW	EQU	$36
KSW	EQU	$38
__monitor_acc	EQU	$45
__monitor_status	EQU	$48
__monitor_rnd	EQU	$4E
SECTORS_PER_TRACK	EQU	$7F
__local_get_alphabet__shift	EQU	$80
__zpabi_A_mod_3__a	EQU	$80
__zpabi_buffer_char__c	EQU	$80
__zpabi_do_rwts_on_sector__command	EQU	$80
__zpabi_set_page_first__slot	EQU	$80
__local_set_page_first__0	EQU	$81
__zpabi_cout_string__count	EQU	$81
__local_set_page_first__prev_first	EQU	$82
__local_print_zstring__ch	EQU	$83
__local_print_zstring__c_0	EQU	$84
__local_print_zstring__0	EQU	$85
__local_buffer_char__c	EQU	$87
__local_get_next_zchar__compound_literal_1	EQU	$87
__local_buffer_char__0	EQU	$88
__local_get_next_zchar__code	EQU	$88
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
__local_dump_buffer_with_more__0	EQU	$98
__local_dump_buffer_with_more__end	EQU	$99
__local_get_next_code_word__1	EQU	$99
__local_get_next_code_byte2__found_1	EQU	$9A
__local_get_next_code_byte2__page	EQU	$9B
__local_do_rwts_on_sector__0	EQU	$9C
__local_cout_string__s	EQU	$9D
__local_do_rwts_on_sector__sector_1	EQU	$9D
__local_dump_buffer_to_printer__x	EQU	$9D
__local_cout_string__1	EQU	$9E
__local_do_rwts_on_sector__sector_0	EQU	$9E
__local_dump_buffer_to_printer__1	EQU	$9E
__local_find_index_of_page_table__0	EQU	$9E
__local_dump_buffer_to_printer__2	EQU	$9F
__local_find_index_of_page_table__1	EQU	$9F
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
SHIFT_ALPHABET	EQU	$CE
LOCKED_ALPHABET	EQU	$CF
ZDECOMPRESS_STATE	EQU	$D0
ZCHARS	EQU	$D1
Z_ABBREV_TABLE	EQU	$E2
SCRATCH1	EQU	$E4
SCRATCH2	EQU	$E6
BUFF_END	EQU	$EB
BUFF_LINE_LEN	EQU	$EC
CURR_LINE	EQU	$ED
PRINTER_CSW	EQU	$EE
BUFF_AREA	EQU	$0200
SLOT1_SCRATCH	EQU	$0779
RWTS	EQU	$2900
HOME	EQU	$FC58
CLREOL	EQU	$FC9C
WAIT	EQU	$FCA8
RDKEY	EQU	$FD0C
COUT	EQU	$FDED
COUT1	EQU	$FDF0

; @zp-link-meta-begin
; def A_mod_3 params=__zpabi_A_mod_3__a locals=__local_A_mod_3__a indirect=false in_cycle=false param_regs=A
; def buffer_char params=__zpabi_buffer_char__c locals=__local_buffer_char__c,__local_buffer_char__0 indirect=false in_cycle=false param_regs=A
; def cout_string params=__zpabi_cout_string__count locals=__local_cout_string__0,__local_cout_string__s,__local_cout_string__1 indirect=false in_cycle=false param_regs=X
; def do_rwts_on_sector params=__zpabi_do_rwts_on_sector__command locals=__local_do_rwts_on_sector__0,__local_do_rwts_on_sector__sector_1,__local_do_rwts_on_sector__sector_0 indirect=false in_cycle=false param_regs=A
; def dump_buffer_line params= locals=__local_dump_buffer_line__0 indirect=false in_cycle=false
; def dump_buffer_to_printer params= locals=__local_dump_buffer_to_printer__0,__local_dump_buffer_to_printer__x,__local_dump_buffer_to_printer__1,__local_dump_buffer_to_printer__2 indirect=false in_cycle=false
; def dump_buffer_to_screen params= locals=__local_dump_buffer_to_screen__0 indirect=false in_cycle=false
; def dump_buffer_with_more params= locals=__local_dump_buffer_with_more__0,__local_dump_buffer_with_more__end,__local_dump_buffer_with_more__1,__local_dump_buffer_with_more__2 indirect=false in_cycle=false
; def find_index_of_page_table params= locals=__local_find_index_of_page_table__compound_literal_1,__local_find_index_of_page_table__compound_literal_0,__local_find_index_of_page_table__0,__local_find_index_of_page_table__1 indirect=false in_cycle=false
; def get_alphabet params= locals=__local_get_alphabet__shift indirect=false in_cycle=false
; def get_next_code_byte2 params= locals=__local_get_next_code_byte2__found_1,__local_get_next_code_byte2__page indirect=false in_cycle=false
; def get_next_code_word params= locals=__local_get_next_code_word__0,__local_get_next_code_word__1 indirect=false in_cycle=false
; def get_next_zchar params= locals=__local_get_next_zchar__compound_literal_1,__local_get_next_zchar__code indirect=false in_cycle=false
; def invalidate_zcode_page2 params= locals= indirect=false in_cycle=false
; def load_packed_address params= locals=__local_load_packed_address__0 indirect=false in_cycle=false
; def print_string_literal params= locals= indirect=false in_cycle=false
; def print_zstring params= locals=__local_print_zstring__ch,__local_print_zstring__c_0,__local_print_zstring__0,__local_print_zstring__1 indirect=false in_cycle=true
; def read_from_sector params= locals=__local_read_from_sector__0 indirect=false in_cycle=false
; def set_page_first params=__zpabi_set_page_first__slot locals=__local_set_page_first__0,__local_set_page_first__prev_first indirect=false in_cycle=false param_regs=A
; ext CLREOL params=
; ext COUT params=__zpabi_COUT__c param_regs=A
; ext COUT1 params=__zpabi_COUT1__c param_regs=A
; ext HOME params=
; ext RDKEY params=
; ext RWTS params=__zpabi_RWTS__iob_hi,__zpabi_RWTS__iob_lo param_regs=A,Y
; ext WAIT params=__zpabi_WAIT__a param_regs=A
; ext restart params=
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
; call print_zstring -> print_zstring
; call read_from_sector -> do_rwts_on_sector
; item do_rwts_on_sector section=.text local=false zero=false
; item read_from_sector section=.text local=false zero=false
; item find_index_of_page_table section=.text local=false zero=false
; item set_page_first section=.text local=false zero=false
; item invalidate_zcode_page2 section=.text local=false zero=false
; item get_next_code_byte2 section=.text local=false zero=false
; item get_next_code_word section=.text local=false zero=false
; item load_packed_address section=.text local=false zero=false
; item dump_buffer_to_screen section=.text local=false zero=false
; item dump_buffer_to_printer section=.text local=false zero=false
; item dump_buffer_line section=.text local=false zero=false
; item cout_string section=.text local=false zero=false
; item dump_buffer_with_more section=.text local=false zero=false
; item buffer_char section=.text local=false zero=false
; item get_next_zchar section=.text local=false zero=false
; item A_mod_3 section=.text local=false zero=false
; item get_alphabet section=.text local=false zero=false
; item print_zstring section=.text local=false zero=false
; item print_string_literal section=.text local=false zero=false
; item dct section=.data local=false zero=false
; item iob section=.data local=false zero=false
; item printer_card_initialized_flag section=.bss local=true zero=true
; item string_more section=.data local=true zero=false
; item a2_table section=.rodata local=true zero=false
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
; zp COLOR size=1 at=$30 def=false init=
; zp INVFLG size=1 at=$32 def=false init=
; zp PROMPT size=1 at=$33 def=false init=
; zp __monitor_ysav1 size=1 at=$35 def=false init=
; zp CSW size=2 at=$36 def=false init=
; zp KSW size=2 at=$38 def=false init=
; zp __monitor_acc size=1 at=$45 def=false init=
; zp __monitor_status size=1 at=$48 def=false init=
; zp __monitor_rnd size=2 at=$4E def=false init=
; zp SECTORS_PER_TRACK size=1 at=$7F def=false init=
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
; zp SHIFT_ALPHABET size=1 at=$CE def=false init=
; zp LOCKED_ALPHABET size=1 at=$CF def=false init=
; zp ZDECOMPRESS_STATE size=1 at=$D0 def=false init=
; zp ZCHARS size=2 at=$D1 def=false init=
; zp Z_ABBREV_TABLE size=2 at=$E2 def=false init=
; zp SCRATCH1 size=2 at=$E4 def=false init=
; zp SCRATCH2 size=2 at=$E6 def=false init=
; zp BUFF_END size=1 at=$EB def=false init=
; zp BUFF_LINE_LEN size=1 at=$EC def=false init=
; zp CURR_LINE size=1 at=$ED def=false init=
; zp PRINTER_CSW size=2 at=$EE def=false init=
; runtime hargs_size=5
; @zp-link-meta-end

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
   STA   __local_do_rwts_on_sector__sector_0
   LDA   SCRATCH1+1
   STA   __local_do_rwts_on_sector__sector_1
.loop@0_continue:
   LDA   SECTORS_PER_TRACK
   STA   __local_do_rwts_on_sector__0
   LDA   __local_do_rwts_on_sector__sector_0
   SEC
   SBC   __local_do_rwts_on_sector__0
   LDA   __local_do_rwts_on_sector__sector_1
   SBC   #$00
   BCC   .loop@0_break
   LDA   SECTORS_PER_TRACK
   STA   __local_do_rwts_on_sector__0
   LDA   __local_do_rwts_on_sector__sector_0
   SEC
   SBC   __local_do_rwts_on_sector__0
   STA   __local_do_rwts_on_sector__sector_0
   BCS   .carry_inc@0
   DEC   __local_do_rwts_on_sector__sector_1
.carry_inc@0:
   INC   iob+4
   JMP   .loop@0_continue
.loop@0_break:
   LDA   __local_do_rwts_on_sector__sector_0
   STA   iob+5
   LDA   #>iob
   LDY   #<iob
   JMP   RWTS

read_from_sector:
   SUBROUTINE

   LDA   #$01
   JMP   do_rwts_on_sector

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
   BNE   .if_end@2
   LDA   (PAGE_H_TABLE),Y
   CMP   __local_find_index_of_page_table__0
   BNE   .find_index_of_page_table@lia@1
   CLC
   TYA
   RTS
.find_index_of_page_table@lia@1:
   LDA   __local_find_index_of_page_table__1
.if_end@2:
   INY
   DEX
   BNE   .loop@1_start
   SEC
   LDA   LAST_Z_PAGE
   RTS

set_page_first:
   SUBROUTINE

   STA   __zpabi_set_page_first__slot
   CMP   FIRST_Z_PAGE
   BNE   .if_end@3
   RTS
.if_end@3:
   LDA   FIRST_Z_PAGE
   STA   __local_set_page_first__prev_first
   LDA   __zpabi_set_page_first__slot
   STA   FIRST_Z_PAGE
   LDY   __zpabi_set_page_first__slot
   LDA   (NEXT_PAGE_TABLE),Y
   TAX
   LDA   __local_set_page_first__prev_first
   STA   (NEXT_PAGE_TABLE),Y
   LDY   __zpabi_set_page_first__slot
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
   BNE   .if_else@5
   LDA   __local_set_page_first__0
   STA   LAST_Z_PAGE
   JMP   .if_end@4
.if_else@5:
   LDA   __local_set_page_first__0
   PHA
   TXA
   TAY
   PLA
   STA   (PREV_PAGE_TABLE),Y
.if_end@4:
   RTS

invalidate_zcode_page2:
   SUBROUTINE

   LDA   #$00
   STA   ZCODE_PAGE_VALID2
   RTS

get_next_code_byte2:
   SUBROUTINE

.loop@2_start:
   LDA   ZCODE_PAGE_VALID2
   BEQ   .if_end@6
   LDY   Z_PC2_L
   LDA   (ZCODE_PAGE_ADDR2),Y
   STA   __local_get_next_code_byte2__page
   TYA
   CLC
   ADC   #$01
   STA   Z_PC2_L
   BNE   .if_end@7
   LDA   #$00
   STA   ZCODE_PAGE_VALID2
   INC   Z_PC2_H
   BNE   .if_end@8
   INC   Z_PC2_HH
.if_end@8:
.if_end@7:
   LDA   __local_get_next_code_byte2__page
   RTS
.if_end@6:
   LDA   Z_PC2_HH
   BNE   .if_else@12
   LDA   Z_PC2_H
   CMP   NUM_IMAGE_PAGES
   BCS   .if_else@12
   STA   __local_get_next_code_byte2__page
   JMP   .if_end@11
.if_else@12:
   LDA   Z_PC2_H
   STA   __local_get_next_code_byte2__page
   LDA   Z_PC2_HH
   STA   __local_get_next_code_byte2__found_1
   LDA   __local_get_next_code_byte2__page
   STA   SCRATCH2
   LDA   __local_get_next_code_byte2__found_1
   STA   SCRATCH2+1
   JSR   find_index_of_page_table
   STA   PAGE_TABLE_INDEX2
   BCC   .if_end@13
   CMP   PAGE_TABLE_INDEX
   BNE   .if_end@14
   LDA   #$00
   STA   ZCODE_PAGE_VALID
.if_end@14:
   LDA   PAGE_TABLE_INDEX2
   STA   __local_get_next_code_byte2__found_1
   LDA   HIGH_MEM_ADDR
   CLC
   ADC   #$00
   STA   SCRATCH2
   LDA   HIGH_MEM_ADDR+1
   ADC   __local_get_next_code_byte2__found_1
   STA   SCRATCH2+1
   LDA   Z_PC2_H
   STA   __local_get_next_code_byte2__page
   LDA   Z_PC2_HH
   STA   __local_get_next_code_byte2__found_1
   LDA   __local_get_next_code_byte2__page
   STA   SCRATCH1
   LDA   __local_get_next_code_byte2__found_1
   STA   SCRATCH1+1
   JSR   read_from_sector
   BCC   .if_end@15
   JSR   restart
.if_end@15:
   LDA   Z_PC2_H
   LDY   PAGE_TABLE_INDEX2
   STA   (PAGE_L_TABLE),Y
   LDA   PAGE_H_TABLE
   STA   DPTR
   LDA   PAGE_H_TABLE+1
   STA   DPTR+1
   LDA   Z_PC2_HH
   LDY   PAGE_TABLE_INDEX2
   STA   (PAGE_H_TABLE),Y
.if_end@13:
   LDA   PAGE_TABLE_INDEX2
   JSR   set_page_first
   LDA   PAGE_TABLE_INDEX2
   CLC
   ADC   NUM_IMAGE_PAGES
   STA   __local_get_next_code_byte2__page
.if_end@11:
   LDA   Z_HEADER_ADDR+1
   STA   __local_get_next_code_byte2__found_1
   LDA   __local_get_next_code_byte2__page
   CLC
   ADC   __local_get_next_code_byte2__found_1
   STA   ZCODE_PAGE_ADDR2+1
   LDA   #$00
   STA   ZCODE_PAGE_ADDR2
   LDA   #$FF
   STA   ZCODE_PAGE_VALID2
   JMP   .loop@2_start

get_next_code_word:
   SUBROUTINE

   JSR   get_next_code_byte2
   STA   __local_get_next_code_word__1
   JSR   get_next_code_byte2
   STA   SCRATCH2
   LDA   __local_get_next_code_word__1
   STA   SCRATCH2+1
   RTS

load_packed_address:
   SUBROUTINE

   LDA   SCRATCH2
   ASL
   STA   Z_PC2_L
   LDA   SCRATCH2
   ASL
   LDA   SCRATCH2+1
   ROL
   STA   Z_PC2_H
   LDA   SCRATCH2+1
   ASL
   LDA   #$00
   ROL
   STA   Z_PC2_HH
   JMP   invalidate_zcode_page2

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
   BNE   .if_end@16
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
.if_end@16:
   LDX   #$00
.loop@4_start:
   CPX   BUFF_END
   BEQ   .loop@4_break
   LDA   BUFF_AREA,X
   STX   __local_dump_buffer_to_printer__x
   JSR   COUT
   LDX   __local_dump_buffer_to_printer__x
   INX
   JMP   .loop@4_start
.loop@4_break:
   LDA   CSW
   STA   PRINTER_CSW
   LDA   CSW+1
   STA   PRINTER_CSW+1
   LDA   __local_dump_buffer_to_printer__2
   STA   CSW
   LDA   __local_dump_buffer_to_printer__1
   STA   CSW+1
   RTS

dump_buffer_line:
   SUBROUTINE

   LDA   Z_HEADER_ADDR
   STA   DPTR
   LDA   Z_HEADER_ADDR+1
   STA   DPTR+1
   LDY   #$11
   LDA   (Z_HEADER_ADDR),Y
   AND   #$01
   BEQ   .if_end@17
   JSR   dump_buffer_to_printer
.if_end@17:
   JMP   dump_buffer_to_screen

cout_string:
   SUBROUTINE

   LDA   SCRATCH2
   STA   __local_cout_string__s
   LDA   SCRATCH2+1
   STA   __local_cout_string__1
   LDY   #$00
.loop@5_start:
   LDA   __local_cout_string__s
   STA   DPTR
   LDA   __local_cout_string__1
   STA   DPTR+1
   LDA   (__local_cout_string__s),Y
   ORA   #$80
   JSR   COUT1
   INY
   DEX
   BNE   .loop@5_start
   RTS

dump_buffer_with_more:
   SUBROUTINE

   LDA   CURR_LINE
   CLC
   ADC   #$01
   STA   __local_dump_buffer_with_more__end
   STA   CURR_LINE
   LDA   WNDBTM
   STA   __local_dump_buffer_with_more__0
   LDA   __local_dump_buffer_with_more__end
   CMP   __local_dump_buffer_with_more__0
   BCC   .if_end@18
   LDA   #<string_more
   STA   SCRATCH2
   LDA   #>string_more
   STA   SCRATCH2+1
   LDA   #$3F
   STA   INVFLG
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
.if_end@18:
   LDA   BUFF_END
   STA   __local_dump_buffer_with_more__end
   JSR   dump_buffer_line
   LDA   WNDWDTH
   STA   __local_dump_buffer_with_more__0
   LDA   __local_dump_buffer_with_more__end
   CMP   __local_dump_buffer_with_more__0
   BEQ   .if_end@19
   LDA   #$8D
   JSR   COUT1
.if_end@19:
   LDA   Z_HEADER_ADDR
   STA   DPTR
   LDA   Z_HEADER_ADDR+1
   STA   DPTR+1
   LDY   #$11
   LDA   (Z_HEADER_ADDR),Y
   AND   #$01
   BEQ   .if_end@20
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
.if_end@20:
   LDA   #$00
   STA   BUFF_END
   RTS

buffer_char:
   SUBROUTINE

   STA   __zpabi_buffer_char__c
   LDX   BUFF_END
   CMP   #$0D
   BNE   .if_end@21
   JMP   dump_buffer_with_more
.if_end@21:
   CMP   #$20
   BCS   .if_end@22
   RTS
.if_end@22:
   CMP   #$60
   BCC   .and_false@23
   CMP   #$80
   BCS   .and_false@23
   LDA   #$01
   STA   __local_buffer_char__0
   JMP   .and_end@24
.and_false@23:
   LDA   #$00
   STA   __local_buffer_char__0
.and_end@24:
   LDA   __local_buffer_char__0
   BEQ   .buffer_char@asm_ssa_split@0
   LDA   __zpabi_buffer_char__c
   SEC
   SBC   #$20
.if_end@25:
   ORA   #$80
   STA   BUFF_AREA,X
   LDA   WNDWDTH
   STA   __local_buffer_char__c
   CPX   __local_buffer_char__c
   BCS   .if_end@26
   INX
   STX   BUFF_END
   RTS
.if_end@26:
.loop@6_start:
   LDA   BUFF_AREA,X
   CMP   #$A0
   BNE   .if_end@27
   JMP   .loop@6_break
.if_end@27:
   DEX
   BNE   .loop@6_start
   LDX   WNDWDTH
.loop@6_break:
   STX   BUFF_LINE_LEN
   STX   BUFF_END
   JSR   dump_buffer_with_more
.loop@7_start:
   LDA   BUFF_LINE_LEN
   CLC
   ADC   #$01
   STA   __local_buffer_char__0
   STA   BUFF_LINE_LEN
   LDA   WNDWDTH
   CMP   __local_buffer_char__0
   BCS   .if_end@29
   RTS
.if_end@29:
   LDX   __local_buffer_char__0
   LDA   BUFF_AREA,X
   LDX   BUFF_END
   INC   BUFF_END
   STA   BUFF_AREA,X
   JMP   .loop@7_start
.buffer_char@asm_ssa_split@0:
   LDA   __zpabi_buffer_char__c
   JMP   .if_end@25

get_next_zchar:
   SUBROUTINE

   LDA   ZDECOMPRESS_STATE
   BPL   .if_end@30
   SEC
   RTS
.if_end@30:
   BNE   .if_else@32
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
   JMP   .if_end@31
.if_else@32:
   CMP   #$01
   BNE   .if_else@34
   LDA   #$02
   STA   ZDECOMPRESS_STATE
   LDA   ZCHARS
   STA   HARGS
   LDA   ZCHARS+1
   STA   HARGS+1
   LDA   #$05
   STA   HARGS+2
   JSR   lsr16
   LDA   HARGS+3
   AND   #$1F
   JMP   .if_end@33
.if_else@34:
   LDA   ZCHARS
   AND   #$00
   STA   __local_get_next_zchar__code
   LDA   ZCHARS+1
   AND   #$80
   STA   __local_get_next_zchar__compound_literal_1
   LDA   __local_get_next_zchar__code
   ORA   __local_get_next_zchar__compound_literal_1
   BEQ   .cond_else@35
   LDA   #$FF
   JMP   .cond_end@36
.cond_else@35:
   LDA   #$00
.cond_end@36:
   STA   ZDECOMPRESS_STATE
   LDA   ZCHARS
   AND   #$1F
.if_end@33:
.if_end@31:
   CLC
   RTS

A_mod_3:
   SUBROUTINE

.loop@8_continue:
   CMP   #$03
   BCC   .loop@8_break
   SEC
   SBC   #$03
   JMP   .loop@8_continue
.loop@8_break:
   RTS

get_alphabet:
   SUBROUTINE

   LDA   SHIFT_ALPHABET
   STA   __local_get_alphabet__shift
   BPL   .if_end@37
   LDA   LOCKED_ALPHABET
   RTS
.if_end@37:
   LDA   #$FF
   STA   SHIFT_ALPHABET
   LDA   __local_get_alphabet__shift
   RTS

print_zstring:
   SUBROUTINE

   LDA   #$00
   STA   LOCKED_ALPHABET
   LDA   #$00
   STA   ZDECOMPRESS_STATE
   LDA   #$FF
   STA   SHIFT_ALPHABET
.loop@9_start:
   JSR   get_next_zchar
   STA   __local_print_zstring__c_0
   BCC   .if_end@38
   RTS
.if_end@38:
   LDA   __local_print_zstring__c_0
   BNE   .if_else@40
   LDA   #$20
   JMP   .if_end@39
.if_else@40:
   CMP   #$01
   BEQ   .lb_skip@0
   JMP   .if_else@42
.lb_skip@0:
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
   LDA   __local_print_zstring__ch
   STA   DPTR
   LDA   __local_print_zstring__c_0
   STA   DPTR+1
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
   JMP   .loop@9_start
.if_else@42:
   CMP   #$04
   BCS   .if_else@44
   JSR   get_alphabet
   STA   __local_print_zstring__ch
   LDA   __local_print_zstring__c_0
   CLC
   ADC   __local_print_zstring__ch
   CLC
   ADC   #$02
   JSR   A_mod_3
   STA   SHIFT_ALPHABET
   JMP   .loop@9_start
.if_else@44:
   CMP   #$06
   BCS   .if_else@46
   JSR   get_alphabet
   CLC
   ADC   __local_print_zstring__c_0
   JSR   A_mod_3
   STA   LOCKED_ALPHABET
   JMP   .loop@9_start
.if_else@46:
   JSR   get_alphabet
   STA   __local_print_zstring__ch
   LDA   __local_print_zstring__ch
   BNE   .if_else@48
   LDA   #$5B
   CLC
   ADC   __local_print_zstring__c_0
   JMP   .if_end@47
.if_else@48:
   CMP   #$01
   BNE   .if_else@50
   LDA   #$3B
   CLC
   ADC   __local_print_zstring__c_0
   JMP   .if_end@49
.if_else@50:
   LDA   __local_print_zstring__c_0
   CMP   #$06
   BNE   .if_else@52
   JSR   get_next_zchar
   ASL
   ASL
   ASL
   ASL
   ASL
   STA   __local_print_zstring__c_0
   JSR   get_next_zchar
   STA   __local_print_zstring__ch
   LDA   __local_print_zstring__c_0
   ORA   __local_print_zstring__ch
   JMP   .if_end@51
.if_else@52:
   CMP   #$07
   BNE   .if_else@54
   LDA   #$0D
   JSR   buffer_char
   LDA   #$0A
   JMP   .if_end@53
.if_else@54:
   SEC
   SBC   #$08
   TAX
   LDA   a2_table,X
.if_end@53:
.if_end@51:
.if_end@49:
.if_end@47:
.if_end@39:
   JSR   buffer_char
   JMP   .loop@9_start

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

dct:
   DS.B  1
   DC.B  $01
   DC.W  $D8EF

iob:
   DC.B  $01
   DC.B  $60
   DC.B  $01
   DS.B  3
   DC.W  dct
   DS.B  7
   DC.B  $60
   DC.B  $01

printer_card_initialized_flag:
   DS.B  1

string_more:
   DC.B  $5B, $4D, $4F, $52, $45, $5D

a2_table:
   DC.B  $30, $31, $32, $33, $34, $35, $36, $37, $38, $39, $2E, $2C, $21, $3F, $5F, $23
   DC.B  $27, $22, $2F, $5C, $2D, $3A, $28, $29
