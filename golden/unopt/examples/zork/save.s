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
Z_ABBREV_TABLE	EQU	$E2
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

copy_data_to_buff:
   SUBROUTINE

   ; prologue: 4 arg bytes, 16 local bytes
   SEC
   LDA   SSP
   SBC   #$12
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$11
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

.loop@0_start:
   LDY   #$16
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   SEC
   SBC   #$01
   INY
   STA   (FP),Y
   LDY   #$16
   STA   (FP),Y
   LDY   #$03
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$13
   LDA   (FP),Y
   CLC
   LDY   #$03
   ADC   (FP),Y
   LDY   #$05
   STA   (FP),Y
   LDY   #$14
   LDA   (FP),Y
   LDY   #$04
   ADC   (FP),Y
   LDY   #$06
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$07
   STA   (FP),Y
   LDY   #$15
   LDA   (FP),Y
   LDY   #$08
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$0A
   STA   (FP),Y
   TAX
   LDY   #$07
   LDA   (FP),Y
   STA   BUFF_AREA,X
   LDY   #$15
   LDA   (FP),Y
   LDY   #$0B
   STA   (FP),Y
   CLC
   ADC   #$01
   INY
   STA   (FP),Y
   LDY   #$15
   STA   (FP),Y
   INY
   LDA   (FP),Y
   LDY   #$0D
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@0
   LDA   #$00
   JMP   .cmp_end@1
.cmp_true@0:
   LDA   #$01
.cmp_end@1:
   LDY   #$0F
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@0
   JMP   .loop@0_start
.lb_skip@0:
   LDY   #$15
   LDA   (FP),Y

   ; epilogue
   PHA
   CLC
   LDA   FP
   ADC   #$16
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$11
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   PLA
   RTS

copy_data_from_buff:
   SUBROUTINE

   ; prologue: 4 arg bytes, 16 local bytes
   SEC
   LDA   SSP
   SBC   #$12
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$11
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

.loop@1_start:
   LDY   #$16
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   SEC
   SBC   #$01
   INY
   STA   (FP),Y
   LDY   #$16
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$03
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$05
   STA   (FP),Y
   TAX
   LDA   BUFF_AREA,X
   INY
   STA   (FP),Y
   LDY   #$16
   LDA   (FP),Y
   LDY   #$07
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$13
   LDA   (FP),Y
   CLC
   LDY   #$07
   ADC   (FP),Y
   LDY   #$09
   STA   (FP),Y
   LDY   #$14
   LDA   (FP),Y
   LDY   #$08
   ADC   (FP),Y
   LDY   #$0A
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$06
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$15
   LDA   (FP),Y
   LDY   #$0B
   STA   (FP),Y
   CLC
   ADC   #$01
   INY
   STA   (FP),Y
   LDY   #$15
   STA   (FP),Y
   INY
   LDA   (FP),Y
   LDY   #$0D
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@2
   LDA   #$00
   JMP   .cmp_end@3
.cmp_true@2:
   LDA   #$01
.cmp_end@3:
   LDY   #$0F
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@1
   JMP   .loop@1_start
.lb_skip@1:
   LDY   #$15
   LDA   (FP),Y

   ; epilogue
   PHA
   CLC
   LDA   FP
   ADC   #$16
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$11
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   PLA
   RTS

instr_save:
   SUBROUTINE

   ; prologue: 0 arg bytes, 51 local bytes
   SEC
   LDA   SSP
   SBC   #$35
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$34
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   JSR   please_insert_save_diskette
   LDY   #$01
   STA   (FP),Y
   TXA
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   disk_sector
   INY
   LDA   (FP),Y
   STA   disk_sector+1
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   Z_HEADER_ADDR
   CLC
   INY
   STA   (FP),Y
   LDA   Z_HEADER_ADDR+1
   ADC   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$07
   STA   (FP),Y
   LDY   #$04
   LDA   (FP),Y
   LDY   #$08
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$0A
   STA   (FP),Y
   TAX
   LDY   #$07
   LDA   (FP),Y
   STA   BUFF_AREA,X
   LDY   #$04
   LDA   (FP),Y
   LDY   #$0B
   STA   (FP),Y
   CLC
   ADC   #$01
   INY
   STA   (FP),Y
   LDY   #$04
   STA   (FP),Y
   LDA   #<Z_PC
   LDY   #$0D
   STA   (FP),Y
   LDA   #>Z_PC
   INY
   STA   (FP),Y
   LDA   #$03
   INY
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$04
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDY   #$0D
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$0E
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$04
   LDA   (FP),Y
   DEY
   STA   (SSP),Y
   LDY   #$0F
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   JSR   copy_data_to_buff
   LDY   #$10
   STA   (FP),Y
   LDY   #$04
   STA   (FP),Y
   LDA   #<LOCAL_ZVARS
   LDY   #$11
   STA   (FP),Y
   LDA   #>LOCAL_ZVARS
   INY
   STA   (FP),Y
   LDA   #$1E
   INY
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$04
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDY   #$11
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$12
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$04
   LDA   (FP),Y
   DEY
   STA   (SSP),Y
   LDY   #$13
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   JSR   copy_data_to_buff
   LDY   #$14
   STA   (FP),Y
   LDY   #$04
   STA   (FP),Y
   LDA   #$06
   LDY   #$15
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$04
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   #$C8
   LDY   #$01
   STA   (SSP),Y
   LDA   #$00
   INY
   STA   (SSP),Y
   LDY   #$04
   LDA   (FP),Y
   DEY
   STA   (SSP),Y
   LDY   #$15
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   JSR   copy_data_to_buff
   LDY   #$16
   STA   (FP),Y
   JSR   write_next_sector
   LDY   #$17
   STA   (FP),Y
   ORA   #$00
   BEQ   .lb_skip@5
   JMP   .instr_save@fail
.lb_skip@5:
   LDA   #$00
   LDY   #$18
   STA   (FP),Y
   INY
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$04
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   #$80
   LDY   #$01
   STA   (SSP),Y
   LDA   #$02
   INY
   STA   (SSP),Y
   LDY   #$18
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$19
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   JSR   copy_data_to_buff
   LDY   #$1A
   STA   (FP),Y
   JSR   write_next_sector
   LDY   #$1B
   STA   (FP),Y
   ORA   #$00
   BEQ   .lb_skip@4
   JMP   .instr_save@fail
.lb_skip@4:
   LDA   #$80
   LDY   #$1C
   STA   (FP),Y
   LDA   #$02
   CLC
   ADC   #$01
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDA   #$68
   INY
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$04
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDY   #$1C
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$1D
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$1E
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$1F
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   JSR   copy_data_to_buff
   LDY   #$20
   STA   (FP),Y
   JSR   write_next_sector
   LDY   #$21
   STA   (FP),Y
   ORA   #$00
   BEQ   .lb_skip@3
   JMP   .instr_save@fail
.lb_skip@3:
   LDA   Z_HEADER_ADDR
   STA   disk_buffer
   LDA   Z_HEADER_ADDR+1
   STA   disk_buffer+1
   LDA   Z_HEADER_ADDR
   CLC
   ADC   #$0E
   LDY   #$22
   STA   (FP),Y
   LDA   Z_HEADER_ADDR+1
   ADC   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$24
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$01
   LDY   #$27
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$28
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$29
   STA   (FP),Y
   INY
   STA   (FP),Y
.loop@2_start:
   JSR   inc_sector_and_write
   LDY   #$2B
   STA   (FP),Y
   ORA   #$00
   BEQ   .lb_skip@2
   JMP   .instr_save@fail
.lb_skip@2:
   LDA   disk_buffer
   LDY   #$2C
   STA   (FP),Y
   LDA   disk_buffer+1
   CLC
   ADC   #$01
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   disk_buffer
   INY
   LDA   (FP),Y
   STA   disk_buffer+1
   LDY   #$2A
   LDA   (FP),Y
   SEC
   SBC   #$01
   LDY   #$2E
   STA   (FP),Y
   LDY   #$2A
   STA   (FP),Y
   LDY   #$2F
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@4
   LDA   #$00
   JMP   .cmp_end@5
.cmp_true@4:
   LDA   #$01
.cmp_end@5:
   LDY   #$31
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .loop@2_start
   JSR   inc_sector_and_write
   LDY   #$33
   STA   (FP),Y
   ORA   #$00
   BNE   .instr_save@fail
   JSR   please_reinsert_game_diskette
   SEC
   LDA   SSP
   SBC   #$01
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   #$01
   LDY   #$01
   STA   (SSP),Y
   JSR   branch

   ; epilogue
   CLC
   LDA   FP
   ADC   #$35
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$34
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   RTS
.instr_save@fail:
   JSR   please_reinsert_game_diskette
   SEC
   LDA   SSP
   SBC   #$01
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   #$00
   LDY   #$01
   STA   (SSP),Y
   JSR   branch

   ; epilogue
   CLC
   LDA   FP
   ADC   #$35
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$34
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   RTS

instr_restore:
   SUBROUTINE

   ; prologue: 0 arg bytes, 68 local bytes
   SEC
   LDA   SSP
   SBC   #$46
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$45
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   JSR   please_insert_save_diskette
   LDY   #$01
   STA   (FP),Y
   TXA
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   disk_sector
   INY
   LDA   (FP),Y
   STA   disk_sector+1
   JSR   read_next_sector
   LDY   #$03
   STA   (FP),Y
   ORA   #$00
   BEQ   .lb_skip@10
   JMP   .instr_restore@fail
.lb_skip@10:
   LDA   #$00
   LDY   #$04
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   Z_HEADER_ADDR
   CLC
   INY
   STA   (FP),Y
   LDA   Z_HEADER_ADDR+1
   ADC   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$08
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$05
   LDA   (FP),Y
   LDY   #$0B
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$0D
   STA   (FP),Y
   TAX
   LDA   BUFF_AREA,X
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$09
   LDA   (FP),Y
   LDY   #$0F
   CMP   (FP),Y
   BNE   .cmp_true@6
   LDA   #$00
   JMP   .cmp_end@7
.cmp_true@6:
   LDA   #$01
.cmp_end@7:
   LDY   #$11
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@9
   JMP   .instr_restore@fail
.lb_skip@9:
   LDA   #$10
   CLC
   ADC   #$01
   LDY   #$13
   STA   (FP),Y
   LDA   #$00
   ADC   #$00
   INY
   STA   (FP),Y
   LDA   Z_HEADER_ADDR
   CLC
   DEY
   ADC   (FP),Y
   LDY   #$15
   STA   (FP),Y
   LDA   Z_HEADER_ADDR+1
   DEY
   ADC   (FP),Y
   LDY   #$16
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$17
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$05
   LDA   (FP),Y
   LDY   #$19
   STA   (FP),Y
   CLC
   ADC   #$01
   INY
   STA   (FP),Y
   LDY   #$05
   STA   (FP),Y
   LDA   #<Z_PC
   LDY   #$1B
   STA   (FP),Y
   LDA   #>Z_PC
   INY
   STA   (FP),Y
   LDA   #$03
   INY
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$04
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDY   #$1B
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$1C
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$05
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$1D
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   JSR   copy_data_from_buff
   LDY   #$1E
   STA   (FP),Y
   LDY   #$05
   STA   (FP),Y
   LDA   #$00
   LDY   #$1F
   STA   (FP),Y
   STA   ZCODE_PAGE_VALID
   LDA   #<LOCAL_ZVARS
   INY
   STA   (FP),Y
   LDA   #>LOCAL_ZVARS
   INY
   STA   (FP),Y
   LDA   #$1E
   INY
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$04
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDY   #$20
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$21
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$05
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$22
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   JSR   copy_data_from_buff
   LDY   #$23
   STA   (FP),Y
   LDY   #$05
   STA   (FP),Y
   LDA   #$06
   LDY   #$24
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$04
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   #$C8
   LDY   #$01
   STA   (SSP),Y
   LDA   #$00
   INY
   STA   (SSP),Y
   LDY   #$05
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$24
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   JSR   copy_data_from_buff
   LDY   #$25
   STA   (FP),Y
   JSR   read_next_sector
   LDY   #$26
   STA   (FP),Y
   ORA   #$00
   BEQ   .lb_skip@8
   JMP   .instr_restore@fail
.lb_skip@8:
   LDA   #$00
   LDY   #$27
   STA   (FP),Y
   INY
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$04
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   #$80
   LDY   #$01
   STA   (SSP),Y
   LDA   #$02
   INY
   STA   (SSP),Y
   LDY   #$27
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$28
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   JSR   copy_data_from_buff
   LDY   #$29
   STA   (FP),Y
   JSR   read_next_sector
   LDY   #$2A
   STA   (FP),Y
   ORA   #$00
   BEQ   .lb_skip@7
   JMP   .instr_restore@fail
.lb_skip@7:
   LDA   #$80
   LDY   #$2B
   STA   (FP),Y
   LDA   #$02
   CLC
   ADC   #$01
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDA   #$68
   INY
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$04
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDY   #$2B
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$2C
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$2D
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$2E
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   JSR   copy_data_from_buff
   LDY   #$2F
   STA   (FP),Y
   LDA   Z_HEADER_ADDR
   STA   disk_buffer
   LDA   Z_HEADER_ADDR+1
   STA   disk_buffer+1
   LDA   Z_HEADER_ADDR
   CLC
   ADC   #$0E
   INY
   STA   (FP),Y
   LDA   Z_HEADER_ADDR+1
   ADC   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$32
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$01
   LDY   #$35
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$36
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$37
   STA   (FP),Y
   INY
   STA   (FP),Y
.loop@3_start:
   JSR   inc_sector_and_read
   LDY   #$39
   STA   (FP),Y
   ORA   #$00
   BEQ   .lb_skip@6
   JMP   .instr_restore@fail
.lb_skip@6:
   LDA   disk_buffer
   LDY   #$3A
   STA   (FP),Y
   LDA   disk_buffer+1
   CLC
   ADC   #$01
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   disk_buffer
   INY
   LDA   (FP),Y
   STA   disk_buffer+1
   LDY   #$38
   LDA   (FP),Y
   SEC
   SBC   #$01
   LDY   #$3C
   STA   (FP),Y
   LDY   #$38
   STA   (FP),Y
   LDY   #$3D
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@8
   LDA   #$00
   JMP   .cmp_end@9
.cmp_true@8:
   LDA   #$01
.cmp_end@9:
   LDY   #$3F
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .loop@3_start
   LDA   #$10
   CLC
   ADC   #$01
   INY
   STA   (FP),Y
   LDA   #$00
   ADC   #$00
   INY
   STA   (FP),Y
   LDA   Z_HEADER_ADDR
   CLC
   DEY
   ADC   (FP),Y
   LDY   #$43
   STA   (FP),Y
   LDA   Z_HEADER_ADDR+1
   DEY
   ADC   (FP),Y
   LDY   #$44
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$18
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   JSR   please_reinsert_game_diskette
   SEC
   LDA   SSP
   SBC   #$01
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   #$01
   LDY   #$01
   STA   (SSP),Y
   JSR   branch

   ; epilogue
   CLC
   LDA   FP
   ADC   #$46
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$45
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   RTS
.instr_restore@fail:
   JSR   please_reinsert_game_diskette
   SEC
   LDA   SSP
   SBC   #$01
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   #$00
   LDY   #$01
   STA   (SSP),Y
   JSR   branch

   ; epilogue
   CLC
   LDA   FP
   ADC   #$46
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$45
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   RTS
