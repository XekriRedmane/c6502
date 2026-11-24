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

get_prompted_number_from_user:
   SUBROUTINE

   ; prologue: 1 arg bytes, 55 local bytes
   SEC
   LDA   SSP
   SBC   #$39
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$38
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   JSR   dump_buffer_with_more
   LDA   #<prompts
   LDY   #$01
   STA   (FP),Y
   LDA   #>prompts
   INY
   STA   (FP),Y
   LDY   #$3A
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
   STA   HARGS
   LDA   #$10
   STA   HARGS+1
   JSR   mul8
   LDA   HARGS+2
   LDY   #$06
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$01
   LDA   (FP),Y
   CLC
   LDY   #$07
   ADC   (FP),Y
   LDY   #$09
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   LDY   #$08
   ADC   (FP),Y
   LDY   #$0A
   STA   (FP),Y
   LDA   #$0F
   INY
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$03
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDY   #$09
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$0A
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$0B
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   JSR   print_ascii_string
   JSR   dump_buffer_line
   LDA   #$19
   LDY   #$0C
   STA   (FP),Y
   STA   CH
   LDA   #$3F
   INY
   STA   (FP),Y
   STA   INVFLG
   LDA   #<sDefault
   INY
   STA   (FP),Y
   LDA   #>sDefault
   INY
   STA   (FP),Y
   LDA   #$0A
   INY
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$03
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDY   #$0E
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$0F
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$10
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   JSR   cout_string
   LDA   #<prompts
   LDY   #$11
   STA   (FP),Y
   LDA   #>prompts
   INY
   STA   (FP),Y
   LDY   #$3A
   LDA   (FP),Y
   LDY   #$13
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$15
   STA   (FP),Y
   STA   HARGS
   LDA   #$10
   STA   HARGS+1
   JSR   mul8
   LDA   HARGS+2
   LDY   #$16
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$11
   LDA   (FP),Y
   CLC
   LDY   #$17
   ADC   (FP),Y
   LDY   #$19
   STA   (FP),Y
   LDY   #$12
   LDA   (FP),Y
   LDY   #$18
   ADC   (FP),Y
   LDY   #$1A
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$0F
   LDY   #$1B
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$1C
   STA   (FP),Y
   LDA   #$01
   INY
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$03
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
   LDY   #$1D
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   JSR   cout_string
   LDA   #$FF
   LDY   #$1E
   STA   (FP),Y
   STA   INVFLG
   JSR   RDKEY
   LDY   #$1F
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$19
   INY
   STA   (FP),Y
   STA   CH
   JSR   CLREOL
   LDY   #$20
   LDA   (FP),Y
   LDY   #$22
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CMP   #$8D
   BEQ   .cmp_true@0
   LDA   #$00
   JMP   .cmp_end@1
.cmp_true@0:
   LDA   #$01
.cmp_end@1:
   LDY   #$24
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@0
   JMP   .if_end@0
.lb_skip@0:
   LDA   #<prompts
   LDY   #$26
   STA   (FP),Y
   LDA   #>prompts
   INY
   STA   (FP),Y
   LDY   #$3A
   LDA   (FP),Y
   LDY   #$28
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$2A
   STA   (FP),Y
   STA   HARGS
   LDA   #$10
   STA   HARGS+1
   JSR   mul8
   LDA   HARGS+2
   LDY   #$2B
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$26
   LDA   (FP),Y
   CLC
   LDY   #$2C
   ADC   (FP),Y
   LDY   #$2E
   STA   (FP),Y
   LDY   #$27
   LDA   (FP),Y
   LDY   #$2D
   ADC   (FP),Y
   LDY   #$2F
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$0F
   LDY   #$30
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$31
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
   LDY   #$20
   STA   (FP),Y
.if_end@0:
   LDY   #$20
   LDA   (FP),Y
   LDY   #$33
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   AND   #$7F
   LDY   #$35
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDA   #$00
   LDY   #$36
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$37
   STA   (FP),Y

   ; epilogue
   PHA
   CLC
   LDA   FP
   ADC   #$3A
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

please_insert_save_diskette:
   SUBROUTINE

   ; prologue: 0 arg bytes, 113 local bytes
   SEC
   LDA   SSP
   SBC   #$73
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$72
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   JSR   home
   JSR   dump_buffer_with_more
   JSR   dump_buffer_with_more
   LDA   #<sPleaseInsert
   LDY   #$01
   STA   (FP),Y
   LDA   #>sPleaseInsert
   INY
   STA   (FP),Y
   LDA   #$1C
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
.loop@0_start:
   LDA   #$02
   LDY   #$04
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
   JSR   get_prompted_number_from_user
   LDY   #$05
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
   SEC
   SBC   #$30
   INY
   LDA   (FP),Y
   SBC   #$00
   BVC   .cmp_novf@2
   EOR   #$80
.cmp_novf@2:
   ASL
   LDA   #$00
   ROL
   LDY   #$09
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@1
   LDY   #$06
   LDA   (FP),Y
   LDY   #$0B
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   SEC
   SBC   #$38
   INY
   LDA   (FP),Y
   SBC   #$00
   BVC   .cmp_novf@3
   EOR   #$80
.cmp_novf@3:
   ASL
   LDA   #$00
   ROL
   EOR   #$01
   LDY   #$0D
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@1
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   JMP   .or_end@2
.or_true@1:
   LDA   #$01
   LDY   #$0F
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@2:
   LDY   #$0F
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@3
   JMP   .loop@0_start
.lb_skip@3:
   LDA   #<prompts
   LDY   #$11
   STA   (FP),Y
   LDA   #>prompts
   INY
   STA   (FP),Y
   LDA   #$02
   STA   HARGS
   LDA   #$00
   LDX   #$08
   LSR   HARGS
.mul_loop@4:
   BCC   .mul_skip@5
   CLC
   ADC   #$10
.mul_skip@5:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@4
   LDY   #$14
   STA   (FP),Y
   LDA   HARGS
   DEY
   STA   (FP),Y
   LDY   #$11
   LDA   (FP),Y
   CLC
   LDY   #$13
   ADC   (FP),Y
   LDY   #$15
   STA   (FP),Y
   LDY   #$12
   LDA   (FP),Y
   LDY   #$14
   ADC   (FP),Y
   LDY   #$16
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$0F
   LDY   #$17
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$18
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
   SEC
   LDA   SSP
   SBC   #$01
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDY   #$06
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   JSR   buffer_char
.loop@1_start:
   LDA   #$00
   LDY   #$19
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
   JSR   get_prompted_number_from_user
   LDY   #$1A
   STA   (FP),Y
   LDY   #$06
   STA   (FP),Y
   LDY   #$1B
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   SEC
   SBC   #$31
   INY
   LDA   (FP),Y
   SBC   #$00
   BVC   .cmp_novf@6
   EOR   #$80
.cmp_novf@6:
   ASL
   LDA   #$00
   ROL
   LDY   #$1D
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@3
   LDY   #$06
   LDA   (FP),Y
   LDY   #$1F
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   SEC
   SBC   #$38
   INY
   LDA   (FP),Y
   SBC   #$00
   BVC   .cmp_novf@7
   EOR   #$80
.cmp_novf@7:
   ASL
   LDA   #$00
   ROL
   EOR   #$01
   LDY   #$21
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@3
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   JMP   .or_end@4
.or_true@3:
   LDA   #$01
   LDY   #$23
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@4:
   LDY   #$23
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@2
   JMP   .loop@1_start
.lb_skip@2:
   LDY   #$06
   LDA   (FP),Y
   LDY   #$25
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   AND   #$07
   LDY   #$27
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDA   #$00
   LDY   #$28
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LSR
   LSR
   LSR
   LSR
   LDY   #$2A
   STA   (FP),Y
   LDY   #$27
   LDA   (FP),Y
   ASL
   ASL
   ASL
   ASL
   LDY   #$29
   STA   (FP),Y
   LDY   #$2B
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
   ADC   #$01
   LDY   #$2E
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$2F
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$2B
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDA   #<prompts
   LDY   #$30
   STA   (FP),Y
   LDA   #>prompts
   INY
   STA   (FP),Y
   LDA   #$00
   STA   HARGS
   LDX   #$08
   LSR   HARGS
.mul_loop@8:
   BCC   .mul_skip@9
   CLC
   ADC   #$10
.mul_skip@9:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@8
   LDY   #$33
   STA   (FP),Y
   LDA   HARGS
   DEY
   STA   (FP),Y
   LDY   #$30
   LDA   (FP),Y
   CLC
   LDY   #$32
   ADC   (FP),Y
   LDY   #$34
   STA   (FP),Y
   LDY   #$31
   LDA   (FP),Y
   LDY   #$33
   ADC   (FP),Y
   LDY   #$35
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$0F
   LDY   #$36
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$37
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
   SEC
   LDA   SSP
   SBC   #$01
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDY   #$06
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   JSR   buffer_char
.loop@2_start:
   LDA   #$01
   LDY   #$38
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
   JSR   get_prompted_number_from_user
   LDY   #$39
   STA   (FP),Y
   LDY   #$06
   STA   (FP),Y
   LDY   #$3A
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   SEC
   SBC   #$31
   INY
   LDA   (FP),Y
   SBC   #$00
   BVC   .cmp_novf@10
   EOR   #$80
.cmp_novf@10:
   ASL
   LDA   #$00
   ROL
   LDY   #$3C
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@5
   LDY   #$06
   LDA   (FP),Y
   LDY   #$3E
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   SEC
   SBC   #$33
   INY
   LDA   (FP),Y
   SBC   #$00
   BVC   .cmp_novf@11
   EOR   #$80
.cmp_novf@11:
   ASL
   LDA   #$00
   ROL
   EOR   #$01
   LDY   #$40
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .or_true@5
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   JMP   .or_end@6
.or_true@5:
   LDA   #$01
   LDY   #$42
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
.or_end@6:
   LDY   #$42
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@1
   JMP   .loop@2_start
.lb_skip@1:
   LDY   #$06
   LDA   (FP),Y
   LDY   #$44
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   AND   #$03
   LDY   #$46
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDA   #$00
   LDY   #$47
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$48
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
   ADC   #$02
   LDY   #$4B
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$4C
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$48
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDA   #<prompts
   LDY   #$4D
   STA   (FP),Y
   LDA   #>prompts
   INY
   STA   (FP),Y
   LDA   #$01
   STA   HARGS
   LDA   #$00
   LDX   #$08
   LSR   HARGS
.mul_loop@12:
   BCC   .mul_skip@13
   CLC
   ADC   #$10
.mul_skip@13:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@12
   LDY   #$50
   STA   (FP),Y
   LDA   HARGS
   DEY
   STA   (FP),Y
   LDY   #$4D
   LDA   (FP),Y
   CLC
   LDY   #$4F
   ADC   (FP),Y
   LDY   #$51
   STA   (FP),Y
   LDY   #$4E
   LDA   (FP),Y
   LDY   #$50
   ADC   (FP),Y
   LDY   #$52
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$0F
   LDY   #$53
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$54
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
   SEC
   LDA   SSP
   SBC   #$01
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDY   #$06
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   JSR   buffer_char
.loop@3_start:
   JSR   dump_buffer_with_more
   LDA   #<sReturnToBegin
   LDY   #$55
   STA   (FP),Y
   LDA   #>sReturnToBegin
   INY
   STA   (FP),Y
   LDA   #$23
   INY
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$03
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDY   #$55
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$56
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$57
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   JSR   print_ascii_string
   JSR   dump_buffer_line
   JSR   RDKEY
   LDY   #$58
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CMP   #$8D
   BNE   .cmp_true@14
   LDA   #$00
   JMP   .cmp_end@15
.cmp_true@14:
   LDA   #$01
.cmp_end@15:
   LDY   #$5B
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .loop@3_start
   LDA   #<prompts
   INY
   STA   (FP),Y
   LDA   #>prompts
   INY
   STA   (FP),Y
   LDA   #$02
   STA   HARGS
   LDA   #$00
   LDX   #$08
   LSR   HARGS
.mul_loop@16:
   BCC   .mul_skip@17
   CLC
   ADC   #$10
.mul_skip@17:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@16
   LDY   #$60
   STA   (FP),Y
   LDA   HARGS
   DEY
   STA   (FP),Y
   LDY   #$5D
   LDA   (FP),Y
   CLC
   LDY   #$5F
   ADC   (FP),Y
   LDY   #$61
   STA   (FP),Y
   LDY   #$5E
   LDA   (FP),Y
   LDY   #$60
   ADC   (FP),Y
   LDY   #$62
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$0F
   LDY   #$63
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$64
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$65
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   AND   #$07
   LDY   #$68
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDA   #$00
   LDY   #$69
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   HARGS
   LDA   #$00
   LDX   #$08
   LSR   HARGS
.mul_loop@18:
   BCC   .mul_skip@19
   CLC
   ADC   #$40
.mul_skip@19:
   ROR
   ROR   HARGS
   DEX
   BNE   .mul_loop@18
   LDY   #$6B
   STA   (FP),Y
   LDA   HARGS
   DEY
   STA   (FP),Y
   SEC
   SBC   #$01
   LDY   #$6C
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   SBC   #$00
   LDY   #$6D
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$6E
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$6F
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$70
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$71
   STA   (FP),Y
   JSR   dump_buffer_with_more
   LDY   #$71
   LDA   (FP),Y
   TAX
   DEY
   LDA   (FP),Y

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$73
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$72
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   PLA
   TAX
   PLA
   RTS

please_reinsert_game_diskette:
   SUBROUTINE

   ; prologue: 0 arg bytes, 41 local bytes
   SEC
   LDA   SSP
   SBC   #$2B
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$2A
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
   ADC   #$01
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
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CMP   #$60
   BEQ   .cmp_true@20
   LDA   #$00
   JMP   .cmp_end@21
.cmp_true@20:
   LDA   #$01
.cmp_end@21:
   LDY   #$08
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@7
   LDA   #<iob
   INY
   STA   (FP),Y
   LDA   #>iob
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$02
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
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$0E
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CMP   #$01
   BEQ   .cmp_true@22
   LDA   #$00
   JMP   .cmp_end@23
.cmp_true@22:
   LDA   #$01
.cmp_end@23:
   LDY   #$11
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@7
   LDA   #$01
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   JMP   .and_end@8
.and_false@7:
   LDA   #$00
   LDY   #$13
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@8:
   LDY   #$13
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@4
   JMP   .if_end@9
.lb_skip@4:
   JSR   dump_buffer_with_more
   LDA   #<sReinsertGameDiskette
   LDY   #$15
   STA   (FP),Y
   LDA   #>sReinsertGameDiskette
   INY
   STA   (FP),Y
   LDA   #$1F
   INY
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$03
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDY   #$15
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$16
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$17
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   JSR   print_ascii_string
.loop@4_start:
   JSR   dump_buffer_with_more
   LDA   #<sPressReturnToContinue
   LDY   #$18
   STA   (FP),Y
   LDA   #>sPressReturnToContinue
   INY
   STA   (FP),Y
   LDA   #$26
   INY
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$03
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDY   #$18
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$19
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$1A
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   JSR   print_ascii_string
   JSR   dump_buffer_line
   JSR   RDKEY
   LDY   #$1B
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CMP   #$8D
   BNE   .cmp_true@24
   LDA   #$00
   JMP   .cmp_end@25
.cmp_true@24:
   LDA   #$01
.cmp_end@25:
   LDY   #$1E
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .loop@4_start
   JSR   dump_buffer_with_more
.if_end@9:
   LDA   #$60
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
   ADC   #$01
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
   LDA   #$01
   LDY   #$25
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
   ADC   #$02
   LDY   #$28
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$29
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$25
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y

   ; epilogue
   CLC
   LDA   FP
   ADC   #$2B
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$2A
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   RTS

sPleaseInsert:
   DC.B  $50, $4C, $45, $41, $53, $45, $20, $49, $4E, $53, $45, $52, $54, $20, $53, $41
   DC.B  $56, $45, $20, $44, $49, $53, $4B, $45, $54, $54, $45, $2C

sDefault:
   DC.B  $44, $45, $46, $41, $55, $4C, $54, $20, $3D, $20

sReturnToBegin:
   DC.B  $2D, $2D, $2D, $20, $50, $52, $45, $53, $53, $20, $27, $52, $45, $54, $55, $52
   DC.B  $4E, $27, $20, $4B, $45, $59, $20, $54, $4F, $20, $42, $45, $47, $49, $4E, $20
   DC.B  $2D, $2D, $2D

sReinsertGameDiskette:
   DC.B  $50, $4C, $45, $41, $53, $45, $20, $52, $45, $2D, $49, $4E, $53, $45, $52, $54
   DC.B  $20, $47, $41, $4D, $45, $20, $44, $49, $53, $4B, $45, $54, $54, $45, $2C

sPressReturnToContinue:
   DC.B  $2D, $2D, $2D, $20, $50, $52, $45, $53, $53, $20, $27, $52, $45, $54, $55, $52
   DC.B  $4E, $27, $20, $4B, $45, $59, $20, $54, $4F, $20, $43, $4F, $4E, $54, $49, $4E
   DC.B  $55, $45, $20, $2D, $2D, $2D

prompts:
   DC.B  $53, $4C, $4F, $54, $20, $20, $20, $20, $20, $28, $31, $2D, $37, $29, $3A
   DC.B  $36
   DC.B  $44, $52, $49, $56, $45, $20, $20, $20, $20, $28, $31, $2D, $32, $29, $3A
   DC.B  $32
   DC.B  $50, $4F, $53, $49, $54, $49, $4F, $4E, $20, $28, $30, $2D, $37, $29, $3A
   DC.B  $30
