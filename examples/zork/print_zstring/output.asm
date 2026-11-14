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
__local_buffer_char__0	EQU	$F4
__local_dump_buffer_with_more__0	EQU	$F5
__local_dump_buffer_with_more__end	EQU	$F6
__local_dump_buffer_to_printer__x	EQU	$F9
__local_dump_buffer_to_printer__1	EQU	$FA
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
; def buffer_char params=__zpabi_buffer_char__c locals=__local_buffer_char__c,__local_buffer_char__0 indirect=false in_cycle=false param_regs=A
; def cout_string params=__zpabi_cout_string__s_0,__zpabi_cout_string__s_1,__zpabi_cout_string__count locals=__local_cout_string__0 indirect=false in_cycle=false param_regs=,,X pinned=$E6,$E7,
; def dump_buffer_line params= locals=__local_dump_buffer_line__0 indirect=false in_cycle=false
; def dump_buffer_to_printer params= locals=__local_dump_buffer_to_printer__0,__local_dump_buffer_to_printer__x,__local_dump_buffer_to_printer__1,__local_dump_buffer_to_printer__2 indirect=false in_cycle=false
; def dump_buffer_to_screen params= locals=__local_dump_buffer_to_screen__0 indirect=false in_cycle=false
; def dump_buffer_with_more params= locals=__local_dump_buffer_with_more__0,__local_dump_buffer_with_more__end indirect=false in_cycle=false
; ext A_mod_3 params=__zpabi_A_mod_3__a param_regs=A
; ext CLREOL params=
; ext COUT params=__zpabi_COUT__c param_regs=A
; ext COUT1 params=__zpabi_COUT1__c param_regs=A
; ext HOME params=
; ext RDKEY params=
; ext RWTS params=__zpabi_RWTS__iob_0,__zpabi_RWTS__iob_1 param_regs=Y,A
; ext WAIT params=__zpabi_WAIT__a param_regs=A
; ext do_rwts_on_sector params=__zpabi_do_rwts_on_sector__command param_regs=A
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
; item buffer_char section=.text local=false zero=false
; item dump_buffer_line section=.text local=false zero=false
; item dump_buffer_to_printer section=.text local=false zero=false
; item dump_buffer_to_screen section=.text local=false zero=false
; item dump_buffer_with_more section=.text local=false zero=false
; item cout_string section=.text local=false zero=false
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
