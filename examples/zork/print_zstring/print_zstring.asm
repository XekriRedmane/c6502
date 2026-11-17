; elf32-mos executable
   PROCESSOR 6502

HARGS	EQU	$00
__bss_size	EQU	$00
__data_size	EQU	$00
__rodata_size	EQU	$00
__text_size	EQU	$00
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
__literal_size	EQU	$2C
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
__disk_size	EQU	$49
__dos_zp_49	EQU	$49
RND	EQU	$4E
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
__local_print_zstring__ch	EQU	$F3
__local_print_zstring__c_0	EQU	$F4
__local_print_zstring__0	EQU	$F5
__local_buffer_char__0	EQU	$F6
__local_get_next_zchar__compound_literal_1	EQU	$F6
__local_dump_buffer_with_more__0	EQU	$F7
__local_get_next_code_word__1	EQU	$F7
__local_dump_buffer_with_more__end	EQU	$F8
__local_get_next_code_byte2__page	EQU	$F8
__local_dump_buffer_to_printer__x	EQU	$F9
__local_get_next_code_byte2__0	EQU	$F9
__local_dump_buffer_to_printer__1	EQU	$FA
__local_set_page_first__0	EQU	$FA
__local_dump_buffer_to_printer__2	EQU	$FB
__local_set_page_first__prev_first	EQU	$FB
__pages_size	EQU	$FB
__output_size	EQU	$0156
__zstring_size	EQU	$0168
BUFF_AREA	EQU	$0200
SLOT1_SCRATCH	EQU	$0779
restart	EQU	$0800
__literal_load	EQU	$0C5B
__literal_start	EQU	$0C5B
__literal_end	EQU	$0C87
__literal_load_end	EQU	$0C87
__pages_load	EQU	$17DD
__pages_start	EQU	$17DD
__pages_end	EQU	$18D8
__pages_load_end	EQU	$18D8
__zstring_load	EQU	$18D8
__zstring_start	EQU	$18D8
__zstring_end	EQU	$1A40
__zstring_load_end	EQU	$1A40
__output_load	EQU	$1B90
__output_start	EQU	$1B90
__output_end	EQU	$1CE6
__output_load_end	EQU	$1CE6
__disk_load	EQU	$1DAC
__disk_start	EQU	$1DAC
__bss_end	EQU	$1DF5
__bss_load	EQU	$1DF5
__bss_load_end	EQU	$1DF5
__bss_start	EQU	$1DF5
__data_end	EQU	$1DF5
__data_load	EQU	$1DF5
__data_load_end	EQU	$1DF5
__data_start	EQU	$1DF5
__disk_end	EQU	$1DF5
__disk_load_end	EQU	$1DF5
__rodata_end	EQU	$1DF5
__rodata_load	EQU	$1DF5
__rodata_load_end	EQU	$1DF5
__rodata_start	EQU	$1DF5
__text_end	EQU	$1DF5
__text_load	EQU	$1DF5
__text_load_end	EQU	$1DF5
__text_start	EQU	$1DF5
RWTS	EQU	$2900
VTAB	EQU	$FC22
HOME	EQU	$FC58
CLREOL	EQU	$FC9C
WAIT	EQU	$FCA8
RDKEY	EQU	$FD0C
GETLN1	EQU	$FD6F
COUT	EQU	$FDED
COUT1	EQU	$FDF0

; Disassembly of section .literal at $0C5B
   SEG   literal
   ORG   $0C5B
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

; Disassembly of section .pages at $17DD
   SEG   pages
   ORG   $17DD
load_packed_address:
   SUBROUTINE
   LDA   SCRATCH2
   ASL
   STA   Z_PC2
   LDA   __zpabi_cout_string__s_1
   ROL
   STA   Z_PC2+1
   LDA   #$00
   ROL
   STA   Z_PC2+2
invalidate_zcode_page2:
   SUBROUTINE
   LDA   #$00
   STA   ZCODE_PAGE_VALID2
   RTS
get_next_code_word:
   SUBROUTINE
   JSR   get_next_code_byte2
   STA   __local_dump_buffer_with_more__0
   JSR   get_next_code_byte2
   STA   SCRATCH2
   LDA   __local_dump_buffer_with_more__0
   STA   __zpabi_cout_string__s_1
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
   BNE   .if_end@1
   INC   Z_PC2+2
.if_end@1:
.if_end@2:
   LDA   (ZCODE_PAGE_ADDR2),Y
   RTS
.if_end@0:
   LDA   Z_PC2+1
   STA   __local_dump_buffer_with_more__end
   LDA   Z_PC2+2
   STA   __local_dump_buffer_to_printer__x
   BNE   .if_else@6
   LDA   __local_dump_buffer_with_more__end
   CMP   NUM_IMAGE_PAGES
   BCC   .vthread@0
.if_else@6:
   LDA   __local_dump_buffer_with_more__end
   STA   SCRATCH2
   LDA   __local_dump_buffer_to_printer__x
   STA   __zpabi_cout_string__s_1
   JSR   find_index_of_page_table
   STA   PAGE_TABLE_INDEX2
   BCC   .if_end@7
   CMP   PAGE_TABLE_INDEX
   BNE   .if_end@8
   LDA   #$00
   STA   ZCODE_PAGE_VALID
.if_end@8:
   LDA   HIGH_MEM_ADDR
   CLC
   ADC   #$00
   STA   SCRATCH2
   LDA   HIGH_MEM_ADDR+1
   ADC   PAGE_TABLE_INDEX2
   STA   __zpabi_cout_string__s_1
   LDA   __local_dump_buffer_with_more__end
   STA   SCRATCH1
   LDA   __local_dump_buffer_to_printer__x
   STA   SCRATCH1+1
   JSR   read_from_sector
   BCC   .if_end@9
   JSR   restart
.if_end@9:
   LDA   __local_dump_buffer_with_more__end
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
.vthread@0:
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
   STA   __local_dump_buffer_to_printer__2
   STY   FIRST_Z_PAGE
   LDA   (NEXT_PAGE_TABLE),Y
   TAX
   LDA   __local_dump_buffer_to_printer__2
   STA   (NEXT_PAGE_TABLE),Y
   LDA   (PREV_PAGE_TABLE),Y
   STA   __local_dump_buffer_to_printer__1
   LDA   #$FF
   STA   (PREV_PAGE_TABLE),Y
   TXA
   LDY   __local_dump_buffer_to_printer__1
   STA   (NEXT_PAGE_TABLE),Y
   LDA   FIRST_Z_PAGE
   LDY   __local_dump_buffer_to_printer__2
   STA   (PREV_PAGE_TABLE),Y
   CPX   #$FF
   BNE   .if_else@12
   LDA   __local_dump_buffer_to_printer__1
   STA   LAST_Z_PAGE
   RTS
.if_else@12:
   TXA
   TAY
   LDA   __local_dump_buffer_to_printer__1
   STA   (PREV_PAGE_TABLE),Y
   RTS
find_index_of_page_table:
   SUBROUTINE
   LDX   NUM_PAGE_TABLE_ENTRIES
   LDY   #$00
   LDA   SCRATCH2
.loop@1_start:
   CMP   (PAGE_L_TABLE),Y
   BNE   .if_end@15
   LDA   (PAGE_H_TABLE),Y
   CMP   __zpabi_cout_string__s_1
   BNE   .find_index_of_page_table@lia@1
   CLC
   TYA
   RTS
.find_index_of_page_table@lia@1:
   LDA   SCRATCH2
.if_end@15:
   INY
   DEX
   BNE   .loop@1_start
   SEC
   LDA   LAST_Z_PAGE
   RTS

; Disassembly of section .zstring at $18D8
   SEG   zstring
   ORG   $18D8
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
   JMP   .if_end@15
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
   STA   __zpabi_cout_string__s_1
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
   JMP   .if_end@15
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
   JMP   .if_end@15
.if_else@14:
   CMP   #$07
   BNE   .if_else@16
   LDA   #$0D
   JSR   buffer_char
   LDA   #$0A
   BNE   .if_end@1
.if_else@16:
   SEC
   SBC   #$08
   TAX
   LDA   a2_table,X
.if_end@1:
.if_end@13:
.if_end@15:
.if_end@9:
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
   LDA   SCRATCH2
   STA   ZCHARS
   LDA   __zpabi_cout_string__s_1
   STA   ZCHARS+1
   LSR
   LSR
   AND   #$1F
   JMP   .if_end@21
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
   STA   __local_buffer_char__0
   LDA   ZCHARS+1
   ASL
   ASL
   ASL
   ORA   __local_buffer_char__0
   AND   #$1F
   JMP   .if_end@21
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
.if_end@21:
   CLC
   RTS
a2_table:
   DC.B  $30, $31, $32, $33, $34, $35, $36, $37
   DC.B  $38, $39, $2E, $2C, $21, $3F, $5F, $23
   DC.B  $27, $22, $2F, $5C, $2D, $3A, $28, $29

; Disassembly of section .output at $1B90
   SEG   output
   ORG   $1B90
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
   BNE   .if_end@5
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
   BCC   dump_buffer_to_screen
   JSR   dump_buffer_to_printer
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
   LDA   CSW
   STA   PRINTER_CSW
   LDA   CSW+1
   STA   PRINTER_CSW+1
   LDA   __local_dump_buffer_to_printer__2
   STA   CSW
   LDA   __local_dump_buffer_to_printer__1
   STA   CSW+1
   RTS
dump_buffer_with_more:
   SUBROUTINE
   INC   CURR_LINE
   LDA   CURR_LINE
   CMP   WNDBTM
   BCC   .if_end@11
   LDA   #$3F
   STA   INVFLG
   LDA   #$E0
   STA   SCRATCH2
   LDA   #$1C
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
   LDA   (SCRATCH2),Y
   ORA   #$80
   JSR   COUT1
   INY
   DEX
   BNE   .loop@4_start
   RTS
printer_card_initialized_flag:
   DC.B  $00
string_more:
   DC.B  $5B, $4D, $4F, $52, $45, $5D

; Disassembly of section .disk at $1DAC
   SEG   disk
   ORG   $1DAC
iob:
   DC.B  $01, $60, $01, $00, $00, $00, $BD, $1D
   DC.B  $00, $00, $00, $00, $00, $00, $00, $60
   DC.B  $01
dct:
   DC.B  $00, $01, $EF, $D8
read_from_sector:
   SUBROUTINE
   LDA   #$01
do_rwts_on_sector:
   SUBROUTINE
   STA   iob+12
   LDA   SCRATCH2
   STA   iob+8
   LDA   __zpabi_cout_string__s_1
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
   LDA   #$1D
   LDY   #$AC
   JMP   RWTS
