; elf32-mos executable
   PROCESSOR 6502

HARGS	EQU	$00
__bss_size	EQU	$00
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
__data_size	EQU	$45
__monitor_acc	EQU	$45
__dos_zp_46	EQU	$46
__monitor_status	EQU	$48
__dos_zp_49	EQU	$49
RND	EQU	$4E
__zork_zp_7c	EQU	$7C
SECTORS_PER_TRACK	EQU	$7F
__zork_zp_80	EQU	$80
Z_PC	EQU	$8A
__zork_zp_8d	EQU	$8D
__rodata_size	EQU	$8E
LOCAL_ZVARS	EQU	$9A
__zork_zp_b8	EQU	$B8
Z_HEADER_ADDR	EQU	$BA
__zork_zp_bc	EQU	$BC
STACK_STATE	EQU	$C8
__zork_zp_ce	EQU	$CE
SCRATCH1	EQU	$E4
__zork_zp_e6	EQU	$E6
__zork_zp_e8	EQU	$E8
__local_inc_sector_and_write__1	EQU	$F3
__zpabi_copy_data_to_buff__src_0	EQU	$F3
__zpabi_cout_string__s_0	EQU	$F3
__zpabi_print_ascii_string__s_0	EQU	$F3
__local_inc_sector_and_write__2	EQU	$F4
__zpabi_copy_data_to_buff__src_1	EQU	$F4
__zpabi_cout_string__s_1	EQU	$F4
__zpabi_print_ascii_string__s_1	EQU	$F4
__zpabi_do_rwts_on_sector__buffer_0	EQU	$F5
__zpabi_inc_sector_and_write__buffer_0	EQU	$F5
__zpabi_do_rwts_on_sector__buffer_1	EQU	$F6
__zpabi_inc_sector_and_write__buffer_1	EQU	$F6
__local_instr_save__pages	EQU	$F7
__local_please_insert_save_diskette__0	EQU	$F8
__local_please_insert_save_diskette__1	EQU	$F9
__local_get_prompted_number_from_user__key	EQU	$FA
__local_get_prompted_number_from_user__1	EQU	$FB
__local_print_ascii_string__count	EQU	$FC
__local_print_ascii_string__i	EQU	$FD
BUFF_AREA	EQU	$0200
__text_size	EQU	$026F
branch	EQU	$0BAA
negated_branch	EQU	$0BB3
buffer_char	EQU	$1B90
dump_buffer_line	EQU	$1BE2
dump_buffer_with_more	EQU	$1C5B
home	EQU	$1CC9
__text_load	EQU	$1DAC
__text_start	EQU	$1DAC
__rodata_load	EQU	$201B
__rodata_start	EQU	$201B
__text_end	EQU	$201B
__text_load_end	EQU	$201B
__data_load	EQU	$20A9
__data_start	EQU	$20A9
__rodata_end	EQU	$20A9
__rodata_load_end	EQU	$20A9
__bss_end	EQU	$20EE
__bss_load	EQU	$20EE
__bss_load_end	EQU	$20EE
__bss_start	EQU	$20EE
__data_end	EQU	$20EE
__data_load_end	EQU	$20EE
RWTS	EQU	$2900
VTAB	EQU	$FC22
HOME	EQU	$FC58
CLREOL	EQU	$FC9C
WAIT	EQU	$FCA8
RDKEY	EQU	$FD0C
GETLN1	EQU	$FD6F
COUT	EQU	$FDED
COUT1	EQU	$FDF0

; Disassembly of section .text at $1DAC
   SEG   text
   ORG   $1DAC
do_rwts_on_sector:
   SUBROUTINE
   STA   iob+12
   LDA   __zpabi_do_rwts_on_sector__buffer_0
   STA   iob+8
   LDA   __zpabi_do_rwts_on_sector__buffer_1
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
   LDA   #$20
   LDY   #$AD
   JMP   RWTS
write_next_sector:
   SUBROUTINE
   LDA   #$00
   STA   __zpabi_do_rwts_on_sector__buffer_0
   LDA   #$02
   STA   __zpabi_do_rwts_on_sector__buffer_1
inc_sector_and_write:
   SUBROUTINE
   INC   SCRATCH1
   BNE   .inc_done@0
   INC   SCRATCH1+1
.inc_done@0:
   LDA   dct+2
   STA   __local_inc_sector_and_write__2
   LDA   dct+3
   STA   __local_inc_sector_and_write__1
   LDA   #$EF
   STA   dct+2
   LDA   #$D8
   STA   dct+3
   LDA   #$02
   JSR   __text_load
   LDA   __local_inc_sector_and_write__2
   STA   dct+2
   LDA   __local_inc_sector_and_write__1
   STA   dct+3
   RTS
print_ascii_string:
   SUBROUTINE
   STX   __local_print_ascii_string__count
   LDA   #$00
   STA   __local_print_ascii_string__i
.loop@0_start:
   LDY   __local_print_ascii_string__i
   LDA   (__local_inc_sector_and_write__1),Y
   JSR   buffer_char
   INC   __local_print_ascii_string__i
   DEC   __local_print_ascii_string__count
   BNE   .loop@0_start
   RTS
cout_string:
   SUBROUTINE
   LDY   #$00
.loop@1_start:
   LDA   (__local_inc_sector_and_write__1),Y
   ORA   #$80
   JSR   COUT1
   INY
   DEX
   BNE   .loop@1_start
   RTS
get_prompted_number_from_user:
   SUBROUTINE
   STA   __local_get_prompted_number_from_user__key
   JSR   dump_buffer_with_more
   LDA   __local_get_prompted_number_from_user__key
   ASL
   ASL
   ASL
   ASL
   STA   __local_get_prompted_number_from_user__1
   LDA   #$BE
   CLC
   ADC   __local_get_prompted_number_from_user__1
   STA   __local_inc_sector_and_write__1
   LDA   #$20
   ADC   #$00
   STA   __local_inc_sector_and_write__2
   LDX   #$0F
   JSR   print_ascii_string
   JSR   dump_buffer_line
   LDA   #$19
   STA   CH
   LDA   #$3F
   STA   INVFLG
   LDA   #$37
   STA   __local_inc_sector_and_write__1
   LDA   #$20
   STA   __local_inc_sector_and_write__2
   LDX   #$0A
   JSR   cout_string
   CLC
   LDA   #$CD
   ADC   __local_get_prompted_number_from_user__1
   STA   __local_inc_sector_and_write__1
   LDA   #$20
   ADC   #$00
   STA   __local_inc_sector_and_write__2
   LDX   #$01
   JSR   cout_string
   LDA   #$FF
   STA   INVFLG
   JSR   RDKEY
   STA   __local_get_prompted_number_from_user__key
   LDA   #$19
   STA   CH
   JSR   CLREOL
   LDA   __local_get_prompted_number_from_user__key
   CMP   #$8D
   BNE   .if_end@0
   LDX   __local_get_prompted_number_from_user__1
   LDA   prompts+15,X
.if_end@0:
   AND   #$7F
   RTS
please_insert_save_diskette:
   SUBROUTINE
   JSR   home
   JSR   dump_buffer_with_more
   JSR   dump_buffer_with_more
   LDA   #$1B
   STA   __local_inc_sector_and_write__1
   LDA   #$20
   STA   __local_inc_sector_and_write__2
   LDX   #$1C
   JSR   print_ascii_string
   JSR   dump_buffer_with_more
.loop@2_start:
   LDA   #$02
   JSR   get_prompted_number_from_user
   CMP   #$30
   BCC   .loop@2_start
   CMP   #$38
   BCS   .loop@2_start
   STA   prompts+47
   JSR   buffer_char
.loop@3_start:
   LDA   #$00
   JSR   get_prompted_number_from_user
   STA   __local_please_insert_save_diskette__1
   CMP   #$31
   BCC   .loop@3_start
   CMP   #$38
   BCS   .loop@3_start
   AND   #$07
   ASL
   ASL
   ASL
   ASL
   STA   iob+1
   LDA   __local_please_insert_save_diskette__1
   STA   prompts+15
   JSR   buffer_char
.loop@4_start:
   LDA   #$01
   JSR   get_prompted_number_from_user
   STA   __local_please_insert_save_diskette__0
   CMP   #$31
   BCC   .loop@4_start
   CMP   #$33
   BCS   .loop@4_start
   AND   #$03
   STA   iob+2
   LDA   __local_please_insert_save_diskette__0
   STA   prompts+31
   JSR   buffer_char
.loop@5_start:
   JSR   dump_buffer_with_more
   LDA   #$41
   STA   __local_inc_sector_and_write__1
   LDA   #$20
   STA   __local_inc_sector_and_write__2
   LDX   #$23
   JSR   print_ascii_string
   JSR   dump_buffer_line
   JSR   RDKEY
   CMP   #$8D
   BNE   .loop@5_start
   LDA   prompts+47
   AND   #$07
   STA   __local_please_insert_save_diskette__0
   LSR
   LSR
   STA   __local_please_insert_save_diskette__1
   LDA   __local_please_insert_save_diskette__0
   ROR
   ROR
   ROR
   AND   #$C0
   SEC
   SBC   #$01
   STA   SCRATCH1
   LDA   __local_please_insert_save_diskette__1
   SBC   #$00
   STA   SCRATCH1+1
   JMP   dump_buffer_with_more
please_reinsert_game_diskette:
   SUBROUTINE
   LDA   iob+1
   CMP   #$60
   BNE   .if_end@9
   LDA   iob+2
   CMP   #$01
   BNE   .if_end@9
   JSR   dump_buffer_with_more
   LDA   #$64
   STA   __local_inc_sector_and_write__1
   LDA   #$20
   STA   __local_inc_sector_and_write__2
   LDX   #$1F
   JSR   print_ascii_string
.loop@6_start:
   JSR   dump_buffer_with_more
   LDA   #$83
   STA   __local_inc_sector_and_write__1
   LDA   #$20
   STA   __local_inc_sector_and_write__2
   LDX   #$26
   JSR   print_ascii_string
   JSR   dump_buffer_line
   JSR   RDKEY
   CMP   #$8D
   BNE   .loop@6_start
   JSR   dump_buffer_with_more
.if_end@9:
   LDA   #$60
   STA   iob+1
   LDA   #$01
   STA   iob+2
   RTS
copy_data_to_buff:
   SUBROUTINE
.loop@0_start:
   DEY
   LDA   (__local_inc_sector_and_write__1),Y
   STA   BUFF_AREA,X
   INX
   TYA
   BNE   .loop@0_start
   TXA
   RTS
instr_save:
   SUBROUTINE
   JSR   please_insert_save_diskette
   LDY   #$00
   LDA   (Z_HEADER_ADDR),Y
   STA   BUFF_AREA
   LDA   #$8A
   STA   __local_inc_sector_and_write__1
   LDA   #$00
   STA   __local_inc_sector_and_write__2
   LDX   #$01
   LDY   #$03
   JSR   copy_data_to_buff
   TAX
   LDA   #$9A
   STA   __local_inc_sector_and_write__1
   LDA   #$00
   STA   __local_inc_sector_and_write__2
   LDY   #$1E
   JSR   copy_data_to_buff
   TAX
   LDA   #$C8
   STA   __local_inc_sector_and_write__1
   LDA   #$00
   STA   __local_inc_sector_and_write__2
   LDY   #$06
   JSR   copy_data_to_buff
   JSR   write_next_sector
   BCS   .instr_save@fail
   LDA   #$80
   STA   __local_inc_sector_and_write__1
   LDA   #$02
   STA   __local_inc_sector_and_write__2
   LDX   #$00
   LDY   #$00
   JSR   copy_data_to_buff
   JSR   write_next_sector
   BCS   .instr_save@fail
   LDA   #$80
   STA   __local_inc_sector_and_write__1
   LDA   #$03
   STA   __local_inc_sector_and_write__2
   LDX   #$00
   LDY   #$68
   JSR   copy_data_to_buff
   JSR   write_next_sector
   BCS   .instr_save@fail
   LDA   Z_HEADER_ADDR
   STA   __zpabi_do_rwts_on_sector__buffer_0
   LDA   Z_HEADER_ADDR+1
   STA   __zpabi_do_rwts_on_sector__buffer_1
   LDY   #$0E
   LDA   (Z_HEADER_ADDR),Y
   ADC   #$01
   STA   __local_instr_save__pages
.loop@1_start:
   JSR   inc_sector_and_write
   BCS   .instr_save@fail
   INC   __zpabi_do_rwts_on_sector__buffer_1
   DEC   __local_instr_save__pages
   BNE   .loop@1_start
   JSR   inc_sector_and_write
   BCS   .instr_save@fail
   JSR   please_reinsert_game_diskette
   JMP   branch
.instr_save@fail:
   JSR   please_reinsert_game_diskette
   JMP   negated_branch

; Disassembly of section .rodata at $201B
   SEG   rodata
   ORG   $201B
sPleaseInsert:
   DC.B  $50, $4C, $45, $41, $53, $45, $20, $49
   DC.B  $4E, $53, $45, $52, $54, $20, $53, $41
   DC.B  $56, $45, $20, $44, $49, $53, $4B, $45
   DC.B  $54, $54, $45, $2C
sDefault:
   DC.B  $44, $45, $46, $41, $55, $4C, $54, $20
   DC.B  $3D, $20
sReturnToBegin:
   DC.B  $2D, $2D, $2D, $20, $50, $52, $45, $53
   DC.B  $53, $20, $27, $52, $45, $54, $55, $52
   DC.B  $4E, $27, $20, $4B, $45, $59, $20, $54
   DC.B  $4F, $20, $42, $45, $47, $49, $4E, $20
   DC.B  $2D, $2D, $2D
sReinsertGameDiskette:
   DC.B  $50, $4C, $45, $41, $53, $45, $20, $52
   DC.B  $45, $2D, $49, $4E, $53, $45, $52, $54
   DC.B  $20, $47, $41, $4D, $45, $20, $44, $49
   DC.B  $53, $4B, $45, $54, $54, $45, $2C
sPressReturnToContinue:
   DC.B  $2D, $2D, $2D, $20, $50, $52, $45, $53
   DC.B  $53, $20, $27, $52, $45, $54, $55, $52
   DC.B  $4E, $27, $20, $4B, $45, $59, $20, $54
   DC.B  $4F, $20, $43, $4F, $4E, $54, $49, $4E
   DC.B  $55, $45, $20, $2D, $2D, $2D

; Disassembly of section .data at $20A9
   SEG   data
   ORG   $20A9
dct:
   DC.B  $00, $01, $EF, $D8
iob:
   DC.B  $01, $60, $01, $00, $00, $00, $A9, $20
   DC.B  $00, $00, $00, $00, $00, $00, $00, $60
   DC.B  $01
prompts:
   DC.B  $53, $4C, $4F, $54, $20, $20, $20, $20
   DC.B  $20, $28, $31, $2D, $37, $29, $3A, $36
   DC.B  $44, $52, $49, $56, $45, $20, $20, $20
   DC.B  $20, $28, $31, $2D, $32, $29, $3A, $32
   DC.B  $50, $4F, $53, $49, $54, $49, $4F, $4E
   DC.B  $20, $28, $30, $2D, $37, $29, $3A, $30
