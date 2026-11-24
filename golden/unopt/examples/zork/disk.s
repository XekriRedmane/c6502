HARGS	EQU	$00
SSP	EQU	$01
FP	EQU	$03
DPTR	EQU	$05
disk_sector	EQU	$07
disk_buffer	EQU	$09
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

do_rwts_on_sector:
   SUBROUTINE

   ; prologue: 1 arg bytes, 39 local bytes
   SEC
   LDA   SSP
   SBC   #$29
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$28
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   LDA   #<iob
   LDY   #$01
   STA   (FP),Y
   LDA   #>iob
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$0C
   LDY   #$03
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$04
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$2A
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDA   #<iob
   LDY   #$05
   STA   (FP),Y
   LDA   #>iob
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$08
   LDY   #$07
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$08
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDA   disk_buffer
   LDY   #$00
   STA   (DPTR),Y
   LDA   disk_buffer+1
   INY
   STA   (DPTR),Y
   LDA   #$03
   LDY   #$09
   STA   (FP),Y
   LDA   #<iob
   INY
   STA   (FP),Y
   LDA   #>iob
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$04
   LDY   #$0C
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$0D
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$09
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDA   disk_sector
   LDY   #$0E
   STA   (FP),Y
   LDA   disk_sector+1
   INY
   STA   (FP),Y
.loop@0_continue:
   LDA   SECTORS_PER_TRACK
   LDY   #$10
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$12
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$13
   STA   (FP),Y
   LDY   #$0E
   LDA   (FP),Y
   SEC
   LDY   #$12
   SBC   (FP),Y
   LDY   #$0F
   LDA   (FP),Y
   LDY   #$13
   SBC   (FP),Y
   LDA   #$00
   ROL
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@0
   JMP   .loop@0_break
.lb_skip@0:
   LDA   SECTORS_PER_TRACK
   LDY   #$16
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$0E
   LDA   (FP),Y
   SEC
   LDY   #$16
   SBC   (FP),Y
   LDY   #$18
   STA   (FP),Y
   LDY   #$0F
   LDA   (FP),Y
   LDY   #$17
   SBC   (FP),Y
   LDY   #$19
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$0E
   STA   (FP),Y
   LDY   #$19
   LDA   (FP),Y
   LDY   #$0F
   STA   (FP),Y
   LDA   #<iob
   LDY   #$1A
   STA   (FP),Y
   LDA   #>iob
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$04
   LDY   #$1C
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$1D
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$1E
   STA   (FP),Y
   CLC
   ADC   #$01
   INY
   STA   (FP),Y
   LDY   #$1C
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$1F
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   JMP   .loop@0_continue
.loop@0_break:
   LDY   #$0E
   LDA   (FP),Y
   LDY   #$20
   STA   (FP),Y
   LDA   #<iob
   INY
   STA   (FP),Y
   LDA   #>iob
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$05
   LDY   #$23
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$24
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$20
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDA   #<iob
   LDY   #$25
   STA   (FP),Y
   LDA   #>iob
   INY
   STA   (FP),Y
   PHA
   DEY
   LDA   (FP),Y
   TAY
   PLA
   JSR   RWTS
   LDA   #$00
   ROL
   LDY   #$27
   STA   (FP),Y
   CMP   #$01

   ; epilogue
   PHP
   CLC
   LDA   FP
   ADC   #$2A
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   INY
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   PLP
   RTS

read_from_sector:
   SUBROUTINE

   ; prologue: 0 arg bytes, 2 local bytes
   SEC
   LDA   SSP
   SBC   #$04
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$03
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   LDA   #$01
   LDY   #$01
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$01
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   (FP),Y
   STA   (SSP),Y
   JSR   do_rwts_on_sector
   LDA   #$00
   ROL
   LDY   #$02
   STA   (FP),Y

   ; epilogue
   PHA
   CLC
   LDA   FP
   ADC   #$04
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   INY
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   PLA
   RTS

inc_sector_and_read:
   SUBROUTINE

   ; prologue: 0 arg bytes, 5 local bytes
   SEC
   LDA   SSP
   SBC   #$07
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$06
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   LDA   disk_sector
   LDY   #$01
   STA   (FP),Y
   LDA   disk_sector+1
   INY
   STA   (FP),Y
   LDA   disk_sector
   CLC
   ADC   #$01
   INY
   STA   (FP),Y
   LDA   disk_sector+1
   ADC   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   disk_sector
   INY
   LDA   (FP),Y
   STA   disk_sector+1
   JSR   read_from_sector
   LDY   #$05
   STA   (FP),Y

   ; epilogue
   PHA
   CLC
   LDA   FP
   ADC   #$07
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   INY
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   PLA
   RTS

read_next_sector:
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

   LDA   #<BUFF_AREA
   LDY   #$01
   STA   (FP),Y
   LDA   #>BUFF_AREA
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   disk_buffer
   INY
   LDA   (FP),Y
   STA   disk_buffer+1
   JSR   inc_sector_and_read
   LDY   #$03
   STA   (FP),Y

   ; epilogue
   PHA
   CLC
   LDA   FP
   ADC   #$05
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   INY
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   PLA
   RTS

write_to_sector:
   SUBROUTINE

   ; prologue: 0 arg bytes, 19 local bytes
   SEC
   LDA   SSP
   SBC   #$15
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$14
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   LDA   #<dct
   LDY   #$01
   STA   (FP),Y
   LDA   #>dct
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$02
   LDY   #$03
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$04
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$05
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$06
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$07
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$08
   STA   (FP),Y
   LDA   #<dct
   INY
   STA   (FP),Y
   LDA   #>dct
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$02
   LDY   #$0B
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$0C
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDA   #$EF
   LDY   #$00
   STA   (DPTR),Y
   LDA   #$D8
   INY
   STA   (DPTR),Y
   LDA   #$02
   LDY   #$0D
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$01
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   JSR   do_rwts_on_sector
   LDA   #$00
   ROL
   LDY   #$0E
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #<dct
   INY
   STA   (FP),Y
   LDA   #>dct
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$02
   LDY   #$12
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$13
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$07
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$08
   LDA   (FP),Y
   LDY   #$01
   STA   (DPTR),Y
   LDY   #$0F
   LDA   (FP),Y

   ; epilogue
   PHA
   CLC
   LDA   FP
   ADC   #$15
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$14
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   PLA
   RTS

inc_sector_and_write:
   SUBROUTINE

   ; prologue: 0 arg bytes, 5 local bytes
   SEC
   LDA   SSP
   SBC   #$07
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$06
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   LDA   disk_sector
   LDY   #$01
   STA   (FP),Y
   LDA   disk_sector+1
   INY
   STA   (FP),Y
   LDA   disk_sector
   CLC
   ADC   #$01
   INY
   STA   (FP),Y
   LDA   disk_sector+1
   ADC   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   disk_sector
   INY
   LDA   (FP),Y
   STA   disk_sector+1
   JSR   write_to_sector
   LDY   #$05
   STA   (FP),Y

   ; epilogue
   PHA
   CLC
   LDA   FP
   ADC   #$07
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   INY
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   PLA
   RTS

write_next_sector:
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

   LDA   #<BUFF_AREA
   LDY   #$01
   STA   (FP),Y
   LDA   #>BUFF_AREA
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   disk_buffer
   INY
   LDA   (FP),Y
   STA   disk_buffer+1
   JSR   inc_sector_and_write
   LDY   #$03
   STA   (FP),Y

   ; epilogue
   PHA
   CLC
   LDA   FP
   ADC   #$05
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   INY
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   PLA
   RTS

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

__zp_init:
   SUBROUTINE

   LDA   #$00
   STA   disk_sector
   STA   disk_sector+1
   STA   disk_buffer
   STA   disk_buffer+1
   RTS
