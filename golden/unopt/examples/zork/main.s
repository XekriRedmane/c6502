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

locate_last_ram_page:
   SUBROUTINE

   ; prologue: 0 arg bytes, 38 local bytes
   SEC
   LDA   SSP
   SBC   #$28
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$27
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   LDA   #$C0
   LDY   #$01
   STA   (FP),Y
   INY
   STA   (FP),Y
.loop@0_start:
   LDY   #$02
   LDA   (FP),Y
   INY
   STA   (FP),Y
   SEC
   SBC   #$01
   INY
   STA   (FP),Y
   LDY   #$02
   STA   (FP),Y
   LDY   #$05
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$08
   STA   (FP),Y
   LDA   #$00
   DEY
   STA   (FP),Y
   LDA   #$FF
   LDY   #$09
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$0A
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$0B
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$0C
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$0D
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$0B
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$11
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$0F
   LDA   (FP),Y
   LDY   #$12
   CMP   (FP),Y
   BNE   .cmp_true@0
   LDA   #$00
   JMP   .cmp_end@1
.cmp_true@0:
   LDA   #$01
.cmp_end@1:
   LDY   #$14
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@1
   JMP   .loop@0_start
.lb_skip@1:
   LDY   #$0E
   LDA   (FP),Y
   LDY   #$16
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   EOR   #$FF
   LDY   #$18
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$19
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$1A
   STA   (FP),Y
   LDY   #$0E
   STA   (FP),Y
   LDY   #$0B
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$0E
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$0E
   LDA   (FP),Y
   LDY   #$1B
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$0B
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$1D
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$1B
   LDA   (FP),Y
   LDY   #$1E
   CMP   (FP),Y
   BNE   .cmp_true@2
   LDA   #$00
   JMP   .cmp_end@3
.cmp_true@2:
   LDA   #$01
.cmp_end@3:
   LDY   #$20
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
   LDY   #$0E
   LDA   (FP),Y
   LDY   #$22
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   EOR   #$FF
   LDY   #$24
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$25
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$26
   STA   (FP),Y
   LDY   #$0B
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$26
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$02
   LDA   (FP),Y

   ; epilogue
   PHA
   CLC
   LDA   FP
   ADC   #$28
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$27
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   PLA
   RTS

main:
   SUBROUTINE

   ; prologue: 0 arg bytes, 225 local bytes
   SEC
   LDA   SSP
   SBC   #$E3
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$E2
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   LDA   #$00
   STA   PRINTER_CSW
   LDA   #$C1
   STA   PRINTER_CSW+1
   LDA   #$00
   LDY   #$01
   STA   (FP),Y
   STA   ZCODE_PAGE_VALID
   INY
   STA   (FP),Y
   STA   ZCODE_PAGE_VALID2
   INY
   STA   (FP),Y
   STA   PAGE_TABLE_INDEX
   INY
   STA   (FP),Y
   STA   PAGE_TABLE_INDEX2
   INY
   STA   (FP),Y
   STA   BUFF_END
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
.loop@1_start:
   LDY   #$08
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CMP   #$1E
   BNE   .cmp_true@4
   LDA   #$00
   JMP   .cmp_end@5
.cmp_true@4:
   LDA   #$01
.cmp_end@5:
   LDY   #$0A
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .loop@1_break
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$07
   LDA   (FP),Y
   LDY   #$0D
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$0F
   STA   (FP),Y
   TAX
   LDY   #$0C
   LDA   (FP),Y
   STA   LOCAL_ZVARS,X
   LDY   #$07
   LDA   (FP),Y
   LDY   #$10
   STA   (FP),Y
   CLC
   ADC   #$01
   INY
   STA   (FP),Y
   LDY   #$07
   STA   (FP),Y
   JMP   .loop@1_start
.loop@1_break:
   LDA   #$00
   STA   FRAME_Z_SP
   STA   FRAME_Z_SP+1
   LDY   #$12
   STA   (FP),Y
   STA   FRAME_STACK_COUNT
   LDA   #$01
   INY
   STA   (FP),Y
   STA   STACK_COUNT
   LDA   #$E8
   STA   Z_SP
   LDA   #$03
   STA   Z_SP+1
   LDA   #$FF
   STA   ZCHAR_SCRATCH1+6
   LDA   #<page_tables
   INY
   STA   (FP),Y
   LDA   #>page_tables
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   PAGE_L_TABLE
   INY
   LDA   (FP),Y
   STA   PAGE_L_TABLE+1
   LDA   #<page_tables
   INY
   STA   (FP),Y
   LDA   #>page_tables
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$80
   LDY   #$18
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$19
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   PAGE_H_TABLE
   INY
   LDA   (FP),Y
   STA   PAGE_H_TABLE+1
   LDA   #<page_tables
   INY
   STA   (FP),Y
   LDA   #>page_tables
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$1C
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$01
   LDY   #$1D
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   NEXT_PAGE_TABLE
   INY
   LDA   (FP),Y
   STA   NEXT_PAGE_TABLE+1
   LDA   #<page_tables
   INY
   STA   (FP),Y
   LDA   #>page_tables
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$80
   LDY   #$20
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$01
   LDY   #$21
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   PREV_PAGE_TABLE
   INY
   LDA   (FP),Y
   STA   PREV_PAGE_TABLE+1
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
.loop@2_start:
   LDA   #$00
   LDY   #$24
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$25
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDA   PAGE_L_TABLE
   CLC
   DEY
   ADC   (FP),Y
   LDY   #$27
   STA   (FP),Y
   LDA   PAGE_L_TABLE+1
   DEY
   ADC   (FP),Y
   LDY   #$28
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$24
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDA   #$00
   LDY   #$29
   STA   (FP),Y
   LDY   #$23
   LDA   (FP),Y
   LDY   #$2A
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDA   PAGE_H_TABLE
   CLC
   DEY
   ADC   (FP),Y
   LDY   #$2C
   STA   (FP),Y
   LDA   PAGE_H_TABLE+1
   DEY
   ADC   (FP),Y
   LDY   #$2D
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$29
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$23
   LDA   (FP),Y
   LDY   #$2E
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$01
   LDY   #$30
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$31
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$32
   STA   (FP),Y
   LDY   #$23
   LDA   (FP),Y
   LDY   #$33
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDA   NEXT_PAGE_TABLE
   CLC
   DEY
   ADC   (FP),Y
   LDY   #$35
   STA   (FP),Y
   LDA   NEXT_PAGE_TABLE+1
   DEY
   ADC   (FP),Y
   LDY   #$36
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$32
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$23
   LDA   (FP),Y
   LDY   #$37
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   SEC
   SBC   #$01
   LDY   #$39
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   SBC   #$00
   LDY   #$3A
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$3B
   STA   (FP),Y
   LDY   #$23
   LDA   (FP),Y
   LDY   #$3C
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDA   PREV_PAGE_TABLE
   CLC
   DEY
   ADC   (FP),Y
   LDY   #$3E
   STA   (FP),Y
   LDA   PREV_PAGE_TABLE+1
   DEY
   ADC   (FP),Y
   LDY   #$3F
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$3B
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$23
   LDA   (FP),Y
   LDY   #$40
   STA   (FP),Y
   CLC
   ADC   #$01
   INY
   STA   (FP),Y
   LDY   #$23
   STA   (FP),Y
   LDY   #$42
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CMP   #$80
   BNE   .cmp_true@6
   LDA   #$00
   JMP   .cmp_end@7
.cmp_true@6:
   LDA   #$01
.cmp_end@7:
   LDY   #$44
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@3
   JMP   .loop@2_start
.lb_skip@3:
   LDA   #$FF
   LDY   #$46
   STA   (FP),Y
   LDA   NEXT_PAGE_TABLE
   CLC
   ADC   #$7F
   INY
   STA   (FP),Y
   LDA   NEXT_PAGE_TABLE+1
   ADC   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$46
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDA   #$00
   LDY   #$49
   STA   (FP),Y
   STA   FIRST_Z_PAGE
   LDA   #$7F
   INY
   STA   (FP),Y
   STA   LAST_Z_PAGE
   LDA   #<__story
   INY
   STA   (FP),Y
   LDA   #>__story
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   Z_HEADER_ADDR
   INY
   LDA   (FP),Y
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
   LDY   #$4D
   STA   (FP),Y
   ORA   #$00
   BEQ   .if_end@2
   JSR   restart
.if_end@2:
   LDA   #$FF
   LDY   #$4E
   STA   (FP),Y
   LDA   #$04
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
   LDY   #$51
   STA   (FP),Y
   LDA   Z_HEADER_ADDR+1
   DEY
   ADC   (FP),Y
   LDY   #$52
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$4E
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDA   Z_HEADER_ADDR
   CLC
   ADC   #$04
   LDY   #$53
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
   LDY   #$55
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
   LDY   #$58
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$59
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$5A
   STA   (FP),Y
   STA   NUM_IMAGE_PAGES
   LDA   #$01
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
.loop@3_start:
   LDY   #$5D
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDA   NUM_IMAGE_PAGES
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$5D
   LDA   (FP),Y
   LDY   #$5F
   CMP   (FP),Y
   BNE   .cmp_true@8
   LDA   #$00
   JMP   .cmp_end@9
.cmp_true@8:
   LDA   #$01
.cmp_end@9:
   LDY   #$61
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@2
   JMP   .loop@3_break
.lb_skip@2:
   LDY   #$5C
   LDA   (FP),Y
   LDY   #$63
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   disk_sector
   INY
   LDA   (FP),Y
   STA   disk_sector+1
   LDY   #$5C
   LDA   (FP),Y
   LDY   #$65
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$68
   STA   (FP),Y
   LDA   #$00
   DEY
   STA   (FP),Y
   LDY   #$69
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$6A
   STA   (FP),Y
   LDA   Z_HEADER_ADDR
   CLC
   DEY
   ADC   (FP),Y
   LDY   #$6B
   STA   (FP),Y
   LDA   Z_HEADER_ADDR+1
   DEY
   ADC   (FP),Y
   LDY   #$6C
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   disk_buffer
   INY
   LDA   (FP),Y
   STA   disk_buffer+1
   JSR   read_from_sector
   LDY   #$6D
   STA   (FP),Y
   ORA   #$00
   BEQ   .if_end@3
   JSR   restart
.if_end@3:
   LDY   #$5C
   LDA   (FP),Y
   LDY   #$6E
   STA   (FP),Y
   CLC
   ADC   #$01
   INY
   STA   (FP),Y
   LDY   #$5C
   STA   (FP),Y
   JMP   .loop@3_start
.loop@3_break:
   LDA   Z_HEADER_ADDR
   CLC
   ADC   #$01
   LDY   #$70
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
   LDY   #$72
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   AND   #$01
   LDY   #$75
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDA   #$00
   LDY   #$76
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@4
   JSR   internal_error
.if_end@4:
   LDA   Z_HEADER_ADDR
   CLC
   ADC   #$06
   LDY   #$77
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
   LDY   #$79
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$7D
   STA   (FP),Y
   LDA   #$00
   DEY
   STA   (FP),Y
   LDA   #$06
   CLC
   ADC   #$01
   LDY   #$7E
   STA   (FP),Y
   LDA   #$00
   ADC   #$00
   INY
   STA   (FP),Y
   LDA   Z_HEADER_ADDR
   CLC
   DEY
   ADC   (FP),Y
   LDY   #$80
   STA   (FP),Y
   LDA   Z_HEADER_ADDR+1
   DEY
   ADC   (FP),Y
   LDY   #$81
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$82
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$85
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$86
   STA   (FP),Y
   LDY   #$7C
   LDA   (FP),Y
   LDY   #$85
   ORA   (FP),Y
   LDY   #$87
   STA   (FP),Y
   LDY   #$7D
   LDA   (FP),Y
   LDY   #$86
   ORA   (FP),Y
   LDY   #$88
   STA   (FP),Y
   LDA   #<Z_PC
   INY
   STA   (FP),Y
   LDA   #>Z_PC
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$87
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$88
   LDA   (FP),Y
   LDY   #$01
   STA   (DPTR),Y
   LDA   #$00
   LDY   #$8B
   STA   (FP),Y
   LDA   #<Z_PC
   INY
   STA   (FP),Y
   LDA   #>Z_PC
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$02
   LDY   #$8E
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$8F
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$8B
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDA   Z_HEADER_ADDR
   CLC
   ADC   #$0C
   LDY   #$90
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
   LDY   #$92
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$96
   STA   (FP),Y
   LDA   #$00
   DEY
   STA   (FP),Y
   LDA   #$0C
   CLC
   ADC   #$01
   LDY   #$97
   STA   (FP),Y
   LDA   #$00
   ADC   #$00
   INY
   STA   (FP),Y
   LDA   Z_HEADER_ADDR
   CLC
   DEY
   ADC   (FP),Y
   LDY   #$99
   STA   (FP),Y
   LDA   Z_HEADER_ADDR+1
   DEY
   ADC   (FP),Y
   LDY   #$9A
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$9B
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$9E
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$9F
   STA   (FP),Y
   LDY   #$95
   LDA   (FP),Y
   LDY   #$9E
   ORA   (FP),Y
   LDY   #$A0
   STA   (FP),Y
   LDY   #$96
   LDA   (FP),Y
   LDY   #$9F
   ORA   (FP),Y
   LDY   #$A1
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$A2
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$A3
   STA   (FP),Y
   LDA   Z_HEADER_ADDR
   CLC
   DEY
   ADC   (FP),Y
   LDY   #$A4
   STA   (FP),Y
   LDA   Z_HEADER_ADDR+1
   DEY
   ADC   (FP),Y
   LDY   #$A5
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   GLOBAL_ZVARS_ADDR
   INY
   LDA   (FP),Y
   STA   GLOBAL_ZVARS_ADDR+1
   LDA   Z_HEADER_ADDR
   CLC
   ADC   #$18
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
   LDY   #$A8
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$AC
   STA   (FP),Y
   LDA   #$00
   DEY
   STA   (FP),Y
   LDA   #$18
   CLC
   ADC   #$01
   LDY   #$AD
   STA   (FP),Y
   LDA   #$00
   ADC   #$00
   INY
   STA   (FP),Y
   LDA   Z_HEADER_ADDR
   CLC
   DEY
   ADC   (FP),Y
   LDY   #$AF
   STA   (FP),Y
   LDA   Z_HEADER_ADDR+1
   DEY
   ADC   (FP),Y
   LDY   #$B0
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$B1
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$B4
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$B5
   STA   (FP),Y
   LDY   #$AB
   LDA   (FP),Y
   LDY   #$B4
   ORA   (FP),Y
   LDY   #$B6
   STA   (FP),Y
   LDY   #$AC
   LDA   (FP),Y
   LDY   #$B5
   ORA   (FP),Y
   LDY   #$B7
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$B8
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$B9
   STA   (FP),Y
   LDA   Z_HEADER_ADDR
   CLC
   DEY
   ADC   (FP),Y
   LDY   #$BA
   STA   (FP),Y
   LDA   Z_HEADER_ADDR+1
   DEY
   ADC   (FP),Y
   LDY   #$BB
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   Z_ABBREV_TABLE
   INY
   LDA   (FP),Y
   STA   Z_ABBREV_TABLE+1
   LDA   NUM_IMAGE_PAGES
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$BF
   STA   (FP),Y
   LDA   #$00
   DEY
   STA   (FP),Y
   LDY   #$C0
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$C1
   STA   (FP),Y
   LDA   Z_HEADER_ADDR
   CLC
   DEY
   ADC   (FP),Y
   LDY   #$C2
   STA   (FP),Y
   LDA   Z_HEADER_ADDR+1
   DEY
   ADC   (FP),Y
   LDY   #$C3
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   HIGH_MEM_ADDR
   INY
   LDA   (FP),Y
   STA   HIGH_MEM_ADDR+1
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$C6
   STA   (FP),Y
   INY
   STA   (FP),Y
   JSR   locate_last_ram_page
   LDY   #$C8
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$C7
   LDA   (FP),Y
   LDY   #$CC
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$CA
   LDA   (FP),Y
   SEC
   LDY   #$CC
   SBC   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$CD
   SBC   (FP),Y
   BVC   .cmp_novf@10
   EOR   #$80
.cmp_novf@10:
   ASL
   LDA   #$00
   ROL
   LDY   #$CE
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@5
   JSR   internal_error
.if_end@5:
   LDY   #$C9
   LDA   (FP),Y
   LDY   #$D0
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$C7
   LDA   (FP),Y
   LDY   #$D2
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$D0
   LDA   (FP),Y
   SEC
   LDY   #$D2
   SBC   (FP),Y
   LDY   #$D4
   STA   (FP),Y
   LDY   #$D1
   LDA   (FP),Y
   LDY   #$D3
   SBC   (FP),Y
   LDY   #$D5
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$D6
   STA   (FP),Y
   INY
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
   LDY   #$DA
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$DB
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$DC
   STA   (FP),Y
   STA   NUM_PAGE_TABLE_ENTRIES
   LDY   #$D7
   LDA   (FP),Y
   STA   LAST_Z_PAGE
   LDA   #$FF
   LDY   #$DD
   STA   (FP),Y
   LDY   #$D7
   LDA   (FP),Y
   LDY   #$DE
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDA   NEXT_PAGE_TABLE
   CLC
   DEY
   ADC   (FP),Y
   LDY   #$E0
   STA   (FP),Y
   LDA   NEXT_PAGE_TABLE+1
   DEY
   ADC   (FP),Y
   LDY   #$E1
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$DD
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
.loop@4_start:
   JSR   do_instruction
   JMP   .loop@4_start

instr_restart:
   SUBROUTINE

   JSR   dump_buffer_with_more
   JSR   restart
   RTS

instr_quit:
   SUBROUTINE

   ; prologue: 0 arg bytes, 3 local bytes
   SEC
   LDA   SSP
   SBC   #$05
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$04
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   JSR   dump_buffer_with_more
   LDA   #<sEndOfSession
   LDY   #$01
   STA   (FP),Y
   LDA   #>sEndOfSession
   INY
   STA   (FP),Y
   LDA   #$14
   INY
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$03
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDY   #$01
   LDA   (FP),Y
   STA   (SSP),Y
   INY
   LDA   (FP),Y
   STA   (SSP),Y
   INY
   LDA   (FP),Y
   STA   (SSP),Y
   JSR   print_ascii_string
   JSR   dump_buffer_with_more
.loop@5_start:
   JMP   .loop@5_start

internal_error:
   SUBROUTINE

   ; prologue: 0 arg bytes, 3 local bytes
   SEC
   LDA   SSP
   SBC   #$05
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$04
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   JSR   dump_buffer_with_more
   LDA   #<sInternalError
   LDY   #$01
   STA   (FP),Y
   LDA   #>sInternalError
   INY
   STA   (FP),Y
   LDA   #$14
   INY
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$03
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDY   #$01
   LDA   (FP),Y
   STA   (SSP),Y
   INY
   LDA   (FP),Y
   STA   (SSP),Y
   INY
   LDA   (FP),Y
   STA   (SSP),Y
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
