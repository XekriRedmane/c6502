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

push:
   SUBROUTINE

   ; prologue: 2 arg bytes, 17 local bytes
   SEC
   LDA   SSP
   SBC   #$13
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$12
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   LDA   Z_SP
   LDY   #$01
   STA   (FP),Y
   LDA   Z_SP+1
   INY
   STA   (FP),Y
   LDA   Z_SP
   SEC
   SBC   #$01
   INY
   STA   (FP),Y
   LDA   Z_SP+1
   SBC   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   Z_SP
   INY
   LDA   (FP),Y
   STA   Z_SP+1
   LDY   #$14
   LDA   (FP),Y
   LDY   #$05
   STA   (FP),Y
   LDA   Z_SP
   STA   DPTR
   LDA   Z_SP+1
   STA   DPTR+1
   LDA   (FP),Y
   LDY   #$00
   STA   (Z_SP),Y
   LDA   Z_SP
   LDY   #$06
   STA   (FP),Y
   LDA   Z_SP+1
   INY
   STA   (FP),Y
   LDA   Z_SP
   SEC
   SBC   #$01
   INY
   STA   (FP),Y
   LDA   Z_SP+1
   SBC   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   Z_SP
   INY
   LDA   (FP),Y
   STA   Z_SP+1
   LDY   #$15
   LDA   (FP),Y
   LDY   #$0A
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$0C
   STA   (FP),Y
   LDA   Z_SP
   STA   DPTR
   LDA   Z_SP+1
   STA   DPTR+1
   LDA   (FP),Y
   LDY   #$00
   STA   (Z_SP),Y
   LDA   STACK_COUNT
   CLC
   ADC   #$01
   LDY   #$0D
   STA   (FP),Y
   STA   STACK_COUNT
   LDA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   SEC
   SBC   #$B4
   INY
   LDA   (FP),Y
   SBC   #$00
   BVC   .cmp_novf@0
   EOR   #$80
.cmp_novf@0:
   ASL
   LDA   #$00
   ROL
   EOR   #$01
   LDY   #$10
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@0
   JSR   internal_error
.if_end@0:

   ; epilogue
   CLC
   LDA   FP
   ADC   #$15
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$12
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   RTS

pop:
   SUBROUTINE

   ; prologue: 0 arg bytes, 25 local bytes
   SEC
   LDA   SSP
   SBC   #$1B
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$1A
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   LDA   Z_SP
   CLC
   LDY   #$01
   STA   (FP),Y
   LDA   Z_SP+1
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
   LDY   #$03
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   Z_SP
   CLC
   ADC   #$01
   INY
   STA   (FP),Y
   LDA   Z_SP+1
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
   INY
   STA   (FP),Y
   LDA   Z_SP
   CLC
   ADC   #$02
   INY
   STA   (FP),Y
   LDA   Z_SP+1
   ADC   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   Z_SP
   INY
   LDA   (FP),Y
   STA   Z_SP+1
   LDA   STACK_COUNT
   SEC
   SBC   #$01
   INY
   STA   (FP),Y
   STA   STACK_COUNT
   LDA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   BEQ   .cmp_true@1
   LDA   #$00
   JMP   .cmp_end@2
.cmp_true@1:
   LDA   #$01
.cmp_end@2:
   LDY   #$0E
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@1
   JSR   internal_error
.if_end@1:
   LDY   #$04
   LDA   (FP),Y
   LDY   #$10
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$13
   STA   (FP),Y
   LDA   #$00
   DEY
   STA   (FP),Y
   LDY   #$08
   LDA   (FP),Y
   LDY   #$14
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$16
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$17
   STA   (FP),Y
   LDY   #$12
   LDA   (FP),Y
   LDY   #$16
   ORA   (FP),Y
   LDY   #$18
   STA   (FP),Y
   LDY   #$13
   LDA   (FP),Y
   LDY   #$17
   ORA   (FP),Y
   LDY   #$19
   STA   (FP),Y
   TAX
   DEY
   LDA   (FP),Y

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$1B
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$1A
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

get_nonstack_var:
   SUBROUTINE

   ; prologue: 1 arg bytes, 61 local bytes
   SEC
   LDA   SSP
   SBC   #$3F
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$3E
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   INY
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   SEC
   SBC   #$10
   INY
   LDA   (FP),Y
   SBC   #$00
   BVC   .cmp_novf@3
   EOR   #$80
.cmp_novf@3:
   ASL
   LDA   #$00
   ROL
   LDY   #$03
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@0
   JMP   .if_end@2
.lb_skip@0:
   LDY   #$40
   LDA   (FP),Y
   LDY   #$05
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   SEC
   SBC   #$01
   LDY   #$07
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   SBC   #$00
   LDY   #$08
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$09
   STA   (FP),Y
   ASL
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$0A
   STA   (FP),Y
   ROL
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$0B
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
   LDY   #$0F
   STA   (FP),Y
   TAX
   LDA   LOCAL_ZVARS,X
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$14
   STA   (FP),Y
   LDA   #$00
   DEY
   STA   (FP),Y
   LDY   #$0C
   LDA   (FP),Y
   LDY   #$15
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$17
   STA   (FP),Y
   TAX
   LDA   LOCAL_ZVARS+1,X
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$1B
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$1C
   STA   (FP),Y
   LDY   #$13
   LDA   (FP),Y
   LDY   #$1B
   ORA   (FP),Y
   LDY   #$1D
   STA   (FP),Y
   LDY   #$14
   LDA   (FP),Y
   LDY   #$1C
   ORA   (FP),Y
   LDY   #$1E
   STA   (FP),Y
   TAX
   DEY
   LDA   (FP),Y

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$40
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$3E
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
.if_end@2:
   LDY   #$40
   LDA   (FP),Y
   LDY   #$1F
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   SEC
   SBC   #$10
   LDY   #$21
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   SBC   #$00
   LDY   #$22
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$23
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ASL
   LDY   #$26
   STA   (FP),Y
   LDA   #$00
   ROL
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$28
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$29
   STA   (FP),Y
   LDA   GLOBAL_ZVARS_ADDR
   CLC
   DEY
   ADC   (FP),Y
   LDY   #$2A
   STA   (FP),Y
   LDA   GLOBAL_ZVARS_ADDR+1
   DEY
   ADC   (FP),Y
   LDY   #$2B
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$2C
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$2D
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
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
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$30
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$34
   STA   (FP),Y
   LDA   #$00
   DEY
   STA   (FP),Y
   LDY   #$2C
   LDA   (FP),Y
   CLC
   ADC   #$01
   LDY   #$35
   STA   (FP),Y
   LDY   #$2D
   LDA   (FP),Y
   ADC   #$00
   LDY   #$36
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$37
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$3A
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$3B
   STA   (FP),Y
   LDY   #$33
   LDA   (FP),Y
   LDY   #$3A
   ORA   (FP),Y
   LDY   #$3C
   STA   (FP),Y
   LDY   #$34
   LDA   (FP),Y
   LDY   #$3B
   ORA   (FP),Y
   LDY   #$3D
   STA   (FP),Y
   TAX
   DEY
   LDA   (FP),Y

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$40
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$3E
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

set_nonstack_var:
   SUBROUTINE

   ; prologue: 3 arg bytes, 45 local bytes
   SEC
   LDA   SSP
   SBC   #$2F
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$2E
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   INY
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   SEC
   SBC   #$10
   INY
   LDA   (FP),Y
   SBC   #$00
   BVC   .cmp_novf@4
   EOR   #$80
.cmp_novf@4:
   ASL
   LDA   #$00
   ROL
   LDY   #$03
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@1
   JMP   .if_end@3
.lb_skip@1:
   LDY   #$30
   LDA   (FP),Y
   LDY   #$05
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   SEC
   SBC   #$01
   LDY   #$07
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   SBC   #$00
   LDY   #$08
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$09
   STA   (FP),Y
   ASL
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$0A
   STA   (FP),Y
   ROL
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$0B
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$32
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
   LDY   #$0C
   LDA   (FP),Y
   LDY   #$10
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$12
   STA   (FP),Y
   TAX
   LDY   #$0F
   LDA   (FP),Y
   STA   LOCAL_ZVARS,X
   LDY   #$31
   LDA   (FP),Y
   LDY   #$13
   STA   (FP),Y
   LDY   #$0C
   LDA   (FP),Y
   LDY   #$14
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$16
   STA   (FP),Y
   TAX
   LDY   #$13
   LDA   (FP),Y
   STA   LOCAL_ZVARS+1,X

   ; epilogue
   CLC
   LDA   FP
   ADC   #$32
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$2E
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   RTS
.if_end@3:
   LDY   #$30
   LDA   (FP),Y
   LDY   #$17
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   SEC
   SBC   #$10
   LDY   #$19
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   SBC   #$00
   LDY   #$1A
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$1B
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ASL
   LDY   #$1E
   STA   (FP),Y
   LDA   #$00
   ROL
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$20
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$21
   STA   (FP),Y
   LDA   GLOBAL_ZVARS_ADDR
   CLC
   DEY
   ADC   (FP),Y
   LDY   #$22
   STA   (FP),Y
   LDA   GLOBAL_ZVARS_ADDR+1
   DEY
   ADC   (FP),Y
   LDY   #$23
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$24
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$25
   STA   (FP),Y
   LDY   #$32
   LDA   (FP),Y
   LDY   #$26
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$28
   STA   (FP),Y
   LDY   #$24
   LDA   (FP),Y
   CLC
   LDY   #$29
   STA   (FP),Y
   LDY   #$25
   LDA   (FP),Y
   ADC   #$00
   LDY   #$2A
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$28
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$31
   LDA   (FP),Y
   LDY   #$2B
   STA   (FP),Y
   LDY   #$24
   LDA   (FP),Y
   CLC
   ADC   #$01
   LDY   #$2C
   STA   (FP),Y
   LDY   #$25
   LDA   (FP),Y
   ADC   #$00
   LDY   #$2D
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

   ; epilogue
   CLC
   LDA   FP
   ADC   #$32
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$2E
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   RTS

var_get:
   SUBROUTINE

   ; prologue: 1 arg bytes, 10 local bytes
   SEC
   LDA   SSP
   SBC   #$0C
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$0B
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   INY
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CMP   #$00
   BEQ   .cmp_true@5
   LDA   #$00
   JMP   .cmp_end@6
.cmp_true@5:
   LDA   #$01
.cmp_end@6:
   LDY   #$03
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@4
   JSR   pop
   LDY   #$05
   STA   (FP),Y
   TXA
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$07
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$08
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$02
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   DEY
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$08
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   JSR   push
   LDY   #$08
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
   ADC   #$0D
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$0B
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
.if_end@4:
   SEC
   LDA   SSP
   SBC   #$01
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDY   #$0D
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   JSR   get_nonstack_var
   LDY   #$09
   STA   (FP),Y
   TXA
   INY
   STA   (FP),Y
   TAX
   DEY
   LDA   (FP),Y

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$0D
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$0B
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

var_put:
   SUBROUTINE

   ; prologue: 3 arg bytes, 6 local bytes
   SEC
   LDA   SSP
   SBC   #$08
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$07
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   INY
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CMP   #$00
   BEQ   .cmp_true@7
   LDA   #$00
   JMP   .cmp_end@8
.cmp_true@7:
   LDA   #$01
.cmp_end@8:
   LDY   #$03
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@5
   JSR   pop
   LDY   #$05
   STA   (FP),Y
   TXA
   INY
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$02
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDY   #$0A
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$0B
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   JSR   push

   ; epilogue
   CLC
   LDA   FP
   ADC   #$0B
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$07
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   RTS
.if_end@5:
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
   JSR   set_nonstack_var

   ; epilogue
   CLC
   LDA   FP
   ADC   #$0B
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$07
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   RTS

store_result:
   SUBROUTINE

   ; prologue: 2 arg bytes, 6 local bytes
   SEC
   LDA   SSP
   SBC   #$08
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$07
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   JSR   get_next_code_byte
   LDY   #$01
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
   CMP   #$00
   BEQ   .cmp_true@9
   LDA   #$00
   JMP   .cmp_end@10
.cmp_true@9:
   LDA   #$01
.cmp_end@10:
   LDY   #$05
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_else@7
   SEC
   LDA   SSP
   SBC   #$02
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
   JSR   push
   JMP   .if_end@6
.if_else@7:
   SEC
   LDA   SSP
   SBC   #$03
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDY   #$02
   LDA   (FP),Y
   DEY
   STA   (SSP),Y
   LDY   #$09
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$0A
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   JSR   set_nonstack_var
.if_end@6:

   ; epilogue
   CLC
   LDA   FP
   ADC   #$0A
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$07
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   RTS

get_const_word:
   SUBROUTINE

   ; prologue: 0 arg bytes, 14 local bytes
   SEC
   LDA   SSP
   SBC   #$10
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$0F
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   JSR   get_next_code_byte
   LDY   #$01
   STA   (FP),Y
   INY
   STA   (FP),Y
   JSR   get_next_code_byte
   LDY   #$03
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
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
   LDY   #$04
   LDA   (FP),Y
   LDY   #$09
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$0B
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$0C
   STA   (FP),Y
   LDY   #$07
   LDA   (FP),Y
   LDY   #$0B
   ORA   (FP),Y
   LDY   #$0D
   STA   (FP),Y
   LDY   #$08
   LDA   (FP),Y
   LDY   #$0C
   ORA   (FP),Y
   LDY   #$0E
   STA   (FP),Y
   TAX
   DEY
   LDA   (FP),Y

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$10
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$0F
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

get_var_content:
   SUBROUTINE

   ; prologue: 0 arg bytes, 10 local bytes
   SEC
   LDA   SSP
   SBC   #$0C
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$0B
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   JSR   get_next_code_byte
   LDY   #$01
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
   CMP   #$00
   BEQ   .cmp_true@11
   LDA   #$00
   JMP   .cmp_end@12
.cmp_true@11:
   LDA   #$01
.cmp_end@12:
   LDY   #$05
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@8
   JSR   pop
   LDY   #$07
   STA   (FP),Y
   TXA
   INY
   STA   (FP),Y
   TAX
   DEY
   LDA   (FP),Y

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$0C
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$0B
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
.if_end@8:
   SEC
   LDA   SSP
   SBC   #$01
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDY   #$02
   LDA   (FP),Y
   DEY
   STA   (SSP),Y
   JSR   get_nonstack_var
   LDY   #$09
   STA   (FP),Y
   TXA
   INY
   STA   (FP),Y
   TAX
   DEY
   LDA   (FP),Y

   ; epilogue
   PHA
   TXA
   PHA
   CLC
   LDA   FP
   ADC   #$0C
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$0B
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

return_value:
   SUBROUTINE

   ; prologue: 2 arg bytes, 65 local bytes
   SEC
   LDA   SSP
   SBC   #$43
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$42
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   LDA   FRAME_Z_SP
   STA   Z_SP
   LDA   FRAME_Z_SP+1
   STA   Z_SP+1
   LDA   FRAME_STACK_COUNT
   STA   STACK_COUNT
   JSR   pop
   LDY   #$01
   STA   (FP),Y
   TXA
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$03
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
   BNE   .cmp_true@13
   LDA   #$00
   JMP   .cmp_end@14
.cmp_true@13:
   LDA   #$01
.cmp_end@14:
   LDY   #$07
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@3
   JMP   .if_end@9
.lb_skip@3:
   LDY   #$04
   LDA   (FP),Y
   LDY   #$09
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ASL
   LDY   #$0B
   STA   (FP),Y
   LDA   #$00
   ROL
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$0D
   STA   (FP),Y
   INY
   STA   (FP),Y
.loop@0_start:
   JSR   pop
   LDY   #$0F
   STA   (FP),Y
   TXA
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$11
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$12
   STA   (FP),Y
   LDY   #$0E
   LDA   (FP),Y
   LDY   #$13
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   SEC
   SBC   #$02
   LDY   #$15
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   SBC   #$00
   LDY   #$16
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$17
   STA   (FP),Y
   LDY   #$0E
   STA   (FP),Y
   LDY   #$12
   LDA   (FP),Y
   LDY   #$18
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$1A
   STA   (FP),Y
   LDY   #$0E
   LDA   (FP),Y
   LDY   #$1B
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$1D
   STA   (FP),Y
   TAX
   LDY   #$1A
   LDA   (FP),Y
   STA   LOCAL_ZVARS,X
   LDY   #$11
   LDA   (FP),Y
   LDY   #$1E
   STA   (FP),Y
   LDY   #$0E
   LDA   (FP),Y
   LDY   #$1F
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$21
   STA   (FP),Y
   TAX
   LDY   #$1E
   LDA   (FP),Y
   STA   LOCAL_ZVARS+1,X
   LDY   #$04
   LDA   (FP),Y
   SEC
   SBC   #$01
   LDY   #$22
   STA   (FP),Y
   LDY   #$04
   STA   (FP),Y
   LDY   #$23
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@15
   LDA   #$00
   JMP   .cmp_end@16
.cmp_true@15:
   LDA   #$01
.cmp_end@16:
   LDY   #$25
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@2
   JMP   .loop@0_start
.lb_skip@2:
.if_end@9:
   JSR   pop
   LDY   #$27
   STA   (FP),Y
   TXA
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$29
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$2A
   STA   (FP),Y
   JSR   pop
   LDY   #$2B
   STA   (FP),Y
   TXA
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   FRAME_Z_SP
   INY
   LDA   (FP),Y
   STA   FRAME_Z_SP+1
   JSR   pop
   LDY   #$2D
   STA   (FP),Y
   TXA
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$2F
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$30
   STA   (FP),Y
   LDY   #$29
   LDA   (FP),Y
   LDY   #$32
   STA   (FP),Y
   LDA   #$00
   DEY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$33
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$31
   LDA   (FP),Y
   LDY   #$33
   ORA   (FP),Y
   LDY   #$35
   STA   (FP),Y
   LDY   #$32
   LDA   (FP),Y
   LDY   #$34
   ORA   (FP),Y
   LDY   #$36
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
   LDY   #$35
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$36
   LDA   (FP),Y
   LDY   #$01
   STA   (DPTR),Y
   LDY   #$2A
   LDA   (FP),Y
   LDY   #$39
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$3B
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
   LDY   #$3E
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
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
   LDY   #$2F
   LDA   (FP),Y
   LDY   #$40
   STA   (FP),Y
   STA   FRAME_STACK_COUNT
   LDA   #$00
   INY
   STA   (FP),Y
   STA   ZCODE_PAGE_VALID
   SEC
   LDA   SSP
   SBC   #$02
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDY   #$44
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$45
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   JSR   store_result

   ; epilogue
   CLC
   LDA   FP
   ADC   #$45
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$42
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   RTS

branch_to_offset:
   SUBROUTINE

   ; prologue: 2 arg bytes, 96 local bytes
   SEC
   LDA   SSP
   SBC   #$62
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$61
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   LDY   #$64
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$03
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$06
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$07
   STA   (FP),Y
   LDY   #$63
   LDA   (FP),Y
   LDA   #$00
   LDY   #$08
   STA   (FP),Y
   LDY   #$64
   LDA   (FP),Y
   AND   #$80
   LDY   #$09
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@10
   LDY   #$06
   LDA   (FP),Y
   LDY   #$0A
   STA   (FP),Y
   LDY   #$07
   LDA   (FP),Y
   CLC
   ADC   #$01
   LDY   #$0B
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$06
   STA   (FP),Y
   LDY   #$0B
   LDA   (FP),Y
   LDY   #$07
   STA   (FP),Y
.if_end@10:
   LDA   #<Z_PC
   LDY   #$0C
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
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$0E
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$0F
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$10
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$63
   LDA   (FP),Y
   LDY   #$14
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$12
   LDA   (FP),Y
   CLC
   LDY   #$15
   ADC   (FP),Y
   LDY   #$17
   STA   (FP),Y
   LDY   #$13
   LDA   (FP),Y
   LDY   #$16
   ADC   (FP),Y
   LDY   #$18
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$19
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$11
   LDA   (FP),Y
   LDY   #$1D
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$1B
   LDA   (FP),Y
   SEC
   LDY   #$1D
   SBC   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$1E
   SBC   (FP),Y
   BVC   .cmp_novf@17
   EOR   #$80
.cmp_novf@17:
   ASL
   LDA   #$00
   ROL
   LDY   #$1F
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@11
   LDY   #$06
   LDA   (FP),Y
   LDY   #$21
   STA   (FP),Y
   LDY   #$07
   LDA   (FP),Y
   LDY   #$22
   STA   (FP),Y
   LDY   #$06
   LDA   (FP),Y
   CLC
   ADC   #$01
   LDY   #$23
   STA   (FP),Y
   LDY   #$07
   LDA   (FP),Y
   ADC   #$00
   LDY   #$24
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$06
   STA   (FP),Y
   LDY   #$24
   LDA   (FP),Y
   LDY   #$07
   STA   (FP),Y
.if_end@11:
   LDY   #$07
   LDA   (FP),Y
   BNE   .thread@0
   DEY
   LDA   (FP),Y
   BEQ   .cmp_true@18
.thread@0:
   LDA   #$00
   JMP   .cmp_end@19
.cmp_true@18:
   LDA   #$01
.cmp_end@19:
   LDY   #$25
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@4
   JMP   .if_end@12
.lb_skip@4:
   LDA   #<Z_PC
   LDY   #$27
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
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$29
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$2A
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDA   #$00
   LDY   #$2B
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$2C
   STA   (FP),Y
   LDY   #$1A
   LDA   (FP),Y
   LDY   #$2D
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$2F
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$30
   STA   (FP),Y
   LDY   #$2B
   LDA   (FP),Y
   LDY   #$2F
   ORA   (FP),Y
   LDY   #$31
   STA   (FP),Y
   LDY   #$2C
   LDA   (FP),Y
   LDY   #$30
   ORA   (FP),Y
   LDY   #$32
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
   LDY   #$31
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$32
   LDA   (FP),Y
   LDY   #$01
   STA   (DPTR),Y

   ; epilogue
   CLC
   LDA   FP
   ADC   #$64
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$61
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   RTS
.if_end@12:
   LDA   #<Z_PC
   LDY   #$35
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
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$37
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$38
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
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
   LDY   #$3D
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$3E
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$3F
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$43
   STA   (FP),Y
   LDA   #$00
   DEY
   STA   (FP),Y
   LDY   #$39
   LDA   (FP),Y
   LDY   #$42
   ORA   (FP),Y
   LDY   #$44
   STA   (FP),Y
   LDY   #$3A
   LDA   (FP),Y
   LDY   #$43
   ORA   (FP),Y
   LDY   #$45
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$46
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$47
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   LDY   #$06
   ADC   (FP),Y
   LDY   #$48
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$07
   ADC   (FP),Y
   LDY   #$49
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$46
   STA   (FP),Y
   LDY   #$49
   LDA   (FP),Y
   LDY   #$47
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$4B
   STA   (FP),Y
   LDA   #$00
   DEY
   STA   (FP),Y
   LDY   #$1A
   LDA   (FP),Y
   LDY   #$4C
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$4E
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$4F
   STA   (FP),Y
   LDY   #$4A
   LDA   (FP),Y
   LDY   #$4E
   ORA   (FP),Y
   LDY   #$50
   STA   (FP),Y
   LDY   #$4B
   LDA   (FP),Y
   LDY   #$4F
   ORA   (FP),Y
   LDY   #$51
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
   LDY   #$50
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$51
   LDA   (FP),Y
   LDY   #$01
   STA   (DPTR),Y
   LDY   #$47
   LDA   (FP),Y
   LDY   #$54
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$56
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   AND   #$01
   LDY   #$59
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDA   #$00
   LDY   #$5A
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$5B
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
   LDY   #$5E
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$5F
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$5B
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDA   #$00
   LDY   #$60
   STA   (FP),Y
   STA   ZCODE_PAGE_VALID

   ; epilogue
   CLC
   LDA   FP
   ADC   #$64
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
   RTS

branch:
   SUBROUTINE

   ; prologue: 1 arg bytes, 68 local bytes
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

   JSR   get_next_code_byte
   LDY   #$01
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
   AND   #$80
   LDY   #$05
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDA   #$00
   LDY   #$06
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@21
   LDA   #$00
   JMP   .cmp_end@22
.cmp_true@21:
   LDA   #$01
.cmp_end@22:
   LDY   #$07
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$09
   STA   (FP),Y
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$47
   LDA   (FP),Y
   LDY   #$0D
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   LDY   #$0B
   LDA   (FP),Y
   LDY   #$0D
   CMP   (FP),Y
   BNE   .cmp_true@23
   LDA   #$00
   JMP   .cmp_end@24
.cmp_true@23:
   LDA   #$01
.cmp_end@24:
   LDY   #$0F
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@13
   LDY   #$02
   LDA   (FP),Y
   LDY   #$11
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   AND   #$40
   LDY   #$13
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDA   #$00
   LDY   #$14
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   BEQ   .cmp_true@25
   LDA   #$00
   JMP   .cmp_end@26
.cmp_true@25:
   LDA   #$01
.cmp_end@26:
   LDY   #$15
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@14
   JSR   get_next_code_byte
   LDY   #$17
   STA   (FP),Y
.if_end@14:

   ; epilogue
   CLC
   LDA   FP
   ADC   #$47
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
.if_end@13:
   LDY   #$02
   LDA   (FP),Y
   LDY   #$18
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   AND   #$40
   LDY   #$1A
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDA   #$00
   LDY   #$1B
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_else@16
   LDY   #$02
   LDA   (FP),Y
   LDY   #$1C
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   AND   #$3F
   LDY   #$1E
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDA   #$00
   LDY   #$1F
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$20
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$21
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$22
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$23
   STA   (FP),Y
   JMP   .if_end@15
.if_else@16:
   LDY   #$02
   LDA   (FP),Y
   LDY   #$24
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   AND   #$3F
   LDY   #$26
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDA   #$00
   LDY   #$27
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$28
   STA   (FP),Y
   INY
   STA   (FP),Y
   JSR   get_next_code_byte
   LDY   #$2A
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$29
   LDA   (FP),Y
   LDY   #$2C
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   AND   #$20
   LDY   #$2E
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDA   #$00
   LDY   #$2F
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@17
   LDY   #$29
   LDA   (FP),Y
   LDY   #$30
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ORA   #$C0
   LDY   #$32
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$33
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$34
   STA   (FP),Y
   LDY   #$29
   STA   (FP),Y
.if_end@17:
   LDY   #$29
   LDA   (FP),Y
   LDY   #$35
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$38
   STA   (FP),Y
   LDA   #$00
   DEY
   STA   (FP),Y
   LDY   #$2B
   LDA   (FP),Y
   LDY   #$39
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$3B
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$3C
   STA   (FP),Y
   LDY   #$37
   LDA   (FP),Y
   LDY   #$3B
   ORA   (FP),Y
   LDY   #$3D
   STA   (FP),Y
   LDY   #$38
   LDA   (FP),Y
   LDY   #$3C
   ORA   (FP),Y
   LDY   #$3E
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$22
   STA   (FP),Y
   LDY   #$3E
   LDA   (FP),Y
   LDY   #$23
   STA   (FP),Y
.if_end@15:
   CMP   #$00
   BNE   .thread@1
   LDY   #$22
   LDA   (FP),Y
   CMP   #$00
   BEQ   .cmp_true@27
.thread@1:
   LDA   #$00
   JMP   .cmp_end@28
.cmp_true@27:
   LDA   #$01
.cmp_end@28:
   LDY   #$3F
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@18
   SEC
   LDA   SSP
   SBC   #$02
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   #$00
   LDY   #$01
   STA   (SSP),Y
   INY
   STA   (SSP),Y
   JSR   return_value

   ; epilogue
   CLC
   LDA   FP
   ADC   #$47
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
.if_end@18:
   LDY   #$23
   LDA   (FP),Y
   CMP   #$00
   BNE   .thread@2
   DEY
   LDA   (FP),Y
   CMP   #$01
   BEQ   .cmp_true@30
.thread@2:
   LDA   #$00
   JMP   .cmp_end@31
.cmp_true@30:
   LDA   #$01
.cmp_end@31:
   LDY   #$41
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@19
   SEC
   LDA   SSP
   SBC   #$02
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   #$01
   LDY   #$01
   STA   (SSP),Y
   LDA   #$00
   INY
   STA   (SSP),Y
   JSR   return_value

   ; epilogue
   CLC
   LDA   FP
   ADC   #$47
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
.if_end@19:
   LDY   #$22
   LDA   (FP),Y
   SEC
   SBC   #$02
   LDY   #$43
   STA   (FP),Y
   LDY   #$23
   LDA   (FP),Y
   SBC   #$00
   LDY   #$44
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$02
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   DEY
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$44
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   JSR   branch_to_offset

   ; epilogue
   CLC
   LDA   FP
   ADC   #$47
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

instr_call:
   SUBROUTINE

   ; prologue: 0 arg bytes, 141 local bytes
   SEC
   LDA   SSP
   SBC   #$8F
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$8E
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   LDA   OPERANDS
   LDY   #$01
   STA   (FP),Y
   LDA   OPERANDS+1
   INY
   STA   (FP),Y
   CMP   #$00
   BNE   .thread@3
   DEY
   LDA   (FP),Y
   CMP   #$00
   BEQ   .cmp_true@33
.thread@3:
   LDA   #$00
   JMP   .cmp_end@34
.cmp_true@33:
   LDA   #$01
.cmp_end@34:
   LDY   #$03
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@20
   SEC
   LDA   SSP
   SBC   #$02
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   #$00
   LDY   #$01
   STA   (SSP),Y
   INY
   STA   (SSP),Y
   JSR   store_result

   ; epilogue
   CLC
   LDA   FP
   ADC   #$8F
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$8E
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   RTS
.if_end@20:
   LDA   #<Z_PC
   LDY   #$05
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
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$07
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$08
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$09
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$0D
   STA   (FP),Y
   LDA   #$00
   DEY
   STA   (FP),Y
   LDA   FRAME_STACK_COUNT
   LDY   #$0E
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$10
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$11
   STA   (FP),Y
   LDY   #$0C
   LDA   (FP),Y
   LDY   #$10
   ORA   (FP),Y
   LDY   #$12
   STA   (FP),Y
   LDY   #$0D
   LDA   (FP),Y
   LDY   #$11
   ORA   (FP),Y
   LDY   #$13
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$02
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   DEY
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$13
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   JSR   push
   SEC
   LDA   SSP
   SBC   #$02
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FRAME_Z_SP
   LDY   #$01
   STA   (SSP),Y
   LDA   FRAME_Z_SP+1
   INY
   STA   (SSP),Y
   JSR   push
   LDA   #<Z_PC
   LDY   #$14
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
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$16
   STA   (FP),Y
   LDY   #$01
   LDA   (DPTR),Y
   LDY   #$17
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
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
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$22
   STA   (FP),Y
   LDA   #$00
   DEY
   STA   (FP),Y
   LDY   #$18
   LDA   (FP),Y
   LDY   #$21
   ORA   (FP),Y
   LDY   #$23
   STA   (FP),Y
   LDY   #$19
   LDA   (FP),Y
   LDY   #$22
   ORA   (FP),Y
   LDY   #$24
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$02
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   DEY
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$24
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   JSR   push
   LDA   #$00
   LDY   #$25
   STA   (FP),Y
   STA   ZCODE_PAGE_VALID
   LDA   OPERANDS
   INY
   STA   (FP),Y
   LDA   OPERANDS+1
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$28
   STA   (FP),Y
   ASL
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$29
   STA   (FP),Y
   ROL
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
   LDY   #$28
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$29
   LDA   (FP),Y
   LDY   #$01
   STA   (DPTR),Y
   LDA   OPERANDS
   LDY   #$2C
   STA   (FP),Y
   LDA   OPERANDS+1
   INY
   STA   (FP),Y
   ASL
   LDA   #$00
   ROL
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$30
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
   LDY   #$33
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$34
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$30
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   JSR   get_next_code_byte
   LDY   #$35
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
   CMP   #$00
   BNE   .cmp_true@36
   LDA   #$00
   JMP   .cmp_end@37
.cmp_true@36:
   LDA   #$01
.cmp_end@37:
   LDY   #$39
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@7
   JMP   .if_end@21
.lb_skip@7:
   LDA   #$00
   LDY   #$3B
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$36
   LDA   (FP),Y
   LDY   #$3D
   STA   (FP),Y
.loop@1_start:
   LDY   #$3C
   LDA   (FP),Y
   LDY   #$3E
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$40
   STA   (FP),Y
   TAX
   LDA   LOCAL_ZVARS,X
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$45
   STA   (FP),Y
   LDA   #$00
   DEY
   STA   (FP),Y
   LDY   #$3C
   LDA   (FP),Y
   LDY   #$46
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$48
   STA   (FP),Y
   TAX
   LDA   LOCAL_ZVARS+1,X
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$4C
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$4D
   STA   (FP),Y
   LDY   #$44
   LDA   (FP),Y
   LDY   #$4C
   ORA   (FP),Y
   LDY   #$4E
   STA   (FP),Y
   LDY   #$45
   LDA   (FP),Y
   LDY   #$4D
   ORA   (FP),Y
   LDY   #$4F
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$02
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   DEY
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$4F
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   JSR   push
   JSR   get_next_code_byte
   LDY   #$50
   STA   (FP),Y
   INY
   STA   (FP),Y
   JSR   get_next_code_byte
   LDY   #$52
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$3C
   LDA   (FP),Y
   LDY   #$54
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$56
   STA   (FP),Y
   TAX
   LDY   #$51
   LDA   (FP),Y
   STA   LOCAL_ZVARS,X
   LDY   #$3C
   LDA   (FP),Y
   LDY   #$57
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$59
   STA   (FP),Y
   TAX
   LDY   #$53
   LDA   (FP),Y
   STA   LOCAL_ZVARS+1,X
   LDY   #$3C
   LDA   (FP),Y
   LDY   #$5A
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$02
   LDY   #$5C
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$5D
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$5E
   STA   (FP),Y
   LDY   #$3C
   STA   (FP),Y
   INY
   LDA   (FP),Y
   SEC
   SBC   #$01
   LDY   #$5F
   STA   (FP),Y
   LDY   #$3D
   STA   (FP),Y
   LDY   #$60
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@38
   LDA   #$00
   JMP   .cmp_end@39
.cmp_true@38:
   LDA   #$01
.cmp_end@39:
   LDY   #$62
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@6
   JMP   .loop@1_start
.lb_skip@6:
.if_end@21:
   LDA   OPERAND_COUNT
   LDY   #$64
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   SEC
   SBC   #$01
   LDY   #$66
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   SBC   #$00
   LDY   #$67
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$68
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
.loop@2_continue:
   LDY   #$69
   LDA   (FP),Y
   LDY   #$6C
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@40
   LDA   #$00
   JMP   .cmp_end@41
.cmp_true@40:
   LDA   #$01
.cmp_end@41:
   LDY   #$6E
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@5
   JMP   .loop@2_break
.lb_skip@5:
   LDY   #$6B
   LDA   (FP),Y
   LDY   #$70
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$72
   STA   (FP),Y
   STA   HARGS
   LDA   #$02
   STA   HARGS+1
   JSR   mul8
   LDA   HARGS+2
   LDY   #$73
   STA   (FP),Y
   TAX
   LDA   OPERANDS+2,X
   INY
   STA   (FP),Y
   LDA   OPERANDS+3,X
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$76
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$77
   STA   (FP),Y
   LDY   #$6B
   LDA   (FP),Y
   LDY   #$78
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ASL
   LDY   #$7A
   STA   (FP),Y
   LDA   #$00
   ROL
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$7C
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$77
   LDA   (FP),Y
   LDY   #$7E
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$80
   STA   (FP),Y
   LDY   #$7D
   LDA   (FP),Y
   LDY   #$81
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$83
   STA   (FP),Y
   TAX
   LDY   #$80
   LDA   (FP),Y
   STA   LOCAL_ZVARS,X
   LDY   #$76
   LDA   (FP),Y
   LDY   #$84
   STA   (FP),Y
   LDY   #$7D
   LDA   (FP),Y
   LDY   #$85
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$87
   STA   (FP),Y
   TAX
   LDY   #$84
   LDA   (FP),Y
   STA   LOCAL_ZVARS+1,X
   LDY   #$6B
   LDA   (FP),Y
   LDY   #$88
   STA   (FP),Y
   CLC
   ADC   #$01
   INY
   STA   (FP),Y
   LDY   #$6B
   STA   (FP),Y
   LDY   #$69
   LDA   (FP),Y
   LDY   #$8A
   STA   (FP),Y
   SEC
   SBC   #$01
   INY
   STA   (FP),Y
   LDY   #$69
   STA   (FP),Y
   JMP   .loop@2_continue
.loop@2_break:
   LDY   #$36
   LDA   (FP),Y
   LDY   #$8C
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$02
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   DEY
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$8D
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   JSR   push
   LDA   STACK_COUNT
   STA   FRAME_STACK_COUNT
   LDA   Z_SP
   STA   FRAME_Z_SP
   LDA   Z_SP+1
   STA   FRAME_Z_SP+1

   ; epilogue
   CLC
   LDA   FP
   ADC   #$8F
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$8E
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   RTS

inc_var:
   SUBROUTINE

   ; prologue: 0 arg bytes, 10 local bytes
   SEC
   LDA   SSP
   SBC   #$0C
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$0B
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   LDA   OPERANDS
   LDY   #$01
   STA   (FP),Y
   LDA   OPERANDS+1
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$03
   STA   (FP),Y
   INY
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
   JSR   var_get
   LDY   #$05
   STA   (FP),Y
   TXA
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$01
   LDY   #$07
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$08
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$09
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$0A
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$03
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDY   #$04
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$09
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$0A
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   JSR   var_put
   LDY   #$0A
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
   ADC   #$0C
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$0B
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

dec_var:
   SUBROUTINE

   ; prologue: 0 arg bytes, 10 local bytes
   SEC
   LDA   SSP
   SBC   #$0C
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$0B
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   LDA   OPERANDS
   LDY   #$01
   STA   (FP),Y
   LDA   OPERANDS+1
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$03
   STA   (FP),Y
   INY
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
   JSR   var_get
   LDY   #$05
   STA   (FP),Y
   TXA
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   SEC
   SBC   #$01
   LDY   #$07
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   SBC   #$00
   LDY   #$08
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$09
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$0A
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$03
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDY   #$04
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$09
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$0A
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   JSR   var_put
   LDY   #$0A
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
   ADC   #$0C
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$0B
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

do_0op:
   SUBROUTINE

   ; prologue: 1 arg bytes, 30 local bytes
   SEC
   LDA   SSP
   SBC   #$20
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$1F
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   INY
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CMP   #$00
   BEQ   .cmp_true@42
   LDA   #$00
   JMP   .cmp_end@43
.cmp_true@42:
   LDA   #$01
.cmp_end@43:
   LDY   #$03
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@19
   JMP   .case@4
.lb_skip@19:
   LDY   #$01
   LDA   (FP),Y
   CMP   #$01
   BEQ   .cmp_true@44
   LDA   #$00
   JMP   .cmp_end@45
.cmp_true@44:
   LDA   #$01
.cmp_end@45:
   LDY   #$05
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@18
   JMP   .case@5
.lb_skip@18:
   LDY   #$01
   LDA   (FP),Y
   CMP   #$02
   BEQ   .cmp_true@46
   LDA   #$00
   JMP   .cmp_end@47
.cmp_true@46:
   LDA   #$01
.cmp_end@47:
   LDY   #$07
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@17
   JMP   .case@6
.lb_skip@17:
   LDY   #$01
   LDA   (FP),Y
   CMP   #$03
   BEQ   .cmp_true@48
   LDA   #$00
   JMP   .cmp_end@49
.cmp_true@48:
   LDA   #$01
.cmp_end@49:
   LDY   #$09
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@16
   JMP   .case@7
.lb_skip@16:
   LDY   #$01
   LDA   (FP),Y
   CMP   #$04
   BEQ   .cmp_true@50
   LDA   #$00
   JMP   .cmp_end@51
.cmp_true@50:
   LDA   #$01
.cmp_end@51:
   LDY   #$0B
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@15
   JMP   .switch@3_break
.lb_skip@15:
   LDY   #$01
   LDA   (FP),Y
   CMP   #$05
   BEQ   .cmp_true@52
   LDA   #$00
   JMP   .cmp_end@53
.cmp_true@52:
   LDA   #$01
.cmp_end@53:
   LDY   #$0D
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@14
   JMP   .case@9
.lb_skip@14:
   LDY   #$01
   LDA   (FP),Y
   CMP   #$06
   BEQ   .cmp_true@54
   LDA   #$00
   JMP   .cmp_end@55
.cmp_true@54:
   LDA   #$01
.cmp_end@55:
   LDY   #$0F
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@13
   JMP   .case@10
.lb_skip@13:
   LDY   #$01
   LDA   (FP),Y
   CMP   #$07
   BEQ   .cmp_true@56
   LDA   #$00
   JMP   .cmp_end@57
.cmp_true@56:
   LDA   #$01
.cmp_end@57:
   LDY   #$11
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@12
   JMP   .case@11
.lb_skip@12:
   LDY   #$01
   LDA   (FP),Y
   CMP   #$08
   BEQ   .cmp_true@58
   LDA   #$00
   JMP   .cmp_end@59
.cmp_true@58:
   LDA   #$01
.cmp_end@59:
   LDY   #$13
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@11
   JMP   .case@12
.lb_skip@11:
   LDY   #$01
   LDA   (FP),Y
   CMP   #$09
   BEQ   .cmp_true@60
   LDA   #$00
   JMP   .cmp_end@61
.cmp_true@60:
   LDA   #$01
.cmp_end@61:
   LDY   #$15
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@10
   JMP   .case@13
.lb_skip@10:
   LDY   #$01
   LDA   (FP),Y
   CMP   #$0A
   BEQ   .cmp_true@62
   LDA   #$00
   JMP   .cmp_end@63
.cmp_true@62:
   LDA   #$01
.cmp_end@63:
   LDY   #$17
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@9
   JMP   .case@14
.lb_skip@9:
   LDY   #$01
   LDA   (FP),Y
   CMP   #$0B
   BEQ   .cmp_true@64
   LDA   #$00
   JMP   .cmp_end@65
.cmp_true@64:
   LDA   #$01
.cmp_end@65:
   LDY   #$19
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@8
   JMP   .case@15
.lb_skip@8:
   JMP   .default@16
.case@4:
   SEC
   LDA   SSP
   SBC   #$02
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   #$01
   LDY   #$01
   STA   (SSP),Y
   LDA   #$00
   INY
   STA   (SSP),Y
   JSR   return_value
   JMP   .switch@3_break
.case@5:
   SEC
   LDA   SSP
   SBC   #$02
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   #$00
   LDY   #$01
   STA   (SSP),Y
.xjump@0:
   LDY   #$02
   STA   (SSP),Y
   JSR   return_value
   JMP   .switch@3_break
.case@6:
   JSR   print_string_literal
   JMP   .switch@3_break
.case@7:
   JSR   print_string_literal
   JSR   new_line
   JMP   .case@4
.case@9:
   JSR   instr_save
   JMP   .switch@3_break
.case@10:
   JSR   instr_restore
   JMP   .switch@3_break
.case@11:
   JSR   instr_restart
   JMP   .switch@3_break
.case@12:
   JSR   pop
   LDY   #$1B
   STA   (FP),Y
   TXA
   INY
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$02
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   DEY
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$1C
   LDA   (FP),Y
   JMP   .xjump@0
.case@13:
   JSR   pop
   LDY   #$1D
   STA   (FP),Y
   TXA
   INY
   STA   (FP),Y
   JMP   .switch@3_break
.case@14:
   JSR   instr_quit
   JMP   .switch@3_break
.case@15:
   JSR   new_line
   JMP   .switch@3_break
.default@16:
   JSR   internal_error
.switch@3_break:

   ; epilogue
   CLC
   LDA   FP
   ADC   #$21
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$1F
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   RTS

do_1op:
   SUBROUTINE

   ; prologue: 1 arg bytes, 51 local bytes
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

   LDA   OPERANDS
   LDY   #$01
   STA   (FP),Y
   LDA   OPERANDS+1
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$03
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$04
   STA   (FP),Y
   LDY   #$36
   LDA   (FP),Y
   LDY   #$05
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   BEQ   .cmp_true@66
   LDA   #$00
   JMP   .cmp_end@67
.cmp_true@66:
   LDA   #$01
.cmp_end@67:
   LDY   #$07
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@34
   JMP   .case@18
.lb_skip@34:
   LDY   #$05
   LDA   (FP),Y
   CMP   #$01
   BEQ   .cmp_true@68
   LDA   #$00
   JMP   .cmp_end@69
.cmp_true@68:
   LDA   #$01
.cmp_end@69:
   LDY   #$09
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@33
   JMP   .case@19
.lb_skip@33:
   LDY   #$05
   LDA   (FP),Y
   CMP   #$02
   BEQ   .cmp_true@70
   LDA   #$00
   JMP   .cmp_end@71
.cmp_true@70:
   LDA   #$01
.cmp_end@71:
   LDY   #$0B
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@32
   JMP   .case@20
.lb_skip@32:
   LDY   #$05
   LDA   (FP),Y
   CMP   #$03
   BEQ   .cmp_true@72
   LDA   #$00
   JMP   .cmp_end@73
.cmp_true@72:
   LDA   #$01
.cmp_end@73:
   LDY   #$0D
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@31
   JMP   .case@21
.lb_skip@31:
   LDY   #$05
   LDA   (FP),Y
   CMP   #$04
   BEQ   .cmp_true@74
   LDA   #$00
   JMP   .cmp_end@75
.cmp_true@74:
   LDA   #$01
.cmp_end@75:
   LDY   #$0F
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@30
   JMP   .case@22
.lb_skip@30:
   LDY   #$05
   LDA   (FP),Y
   CMP   #$05
   BEQ   .cmp_true@76
   LDA   #$00
   JMP   .cmp_end@77
.cmp_true@76:
   LDA   #$01
.cmp_end@77:
   LDY   #$11
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@29
   JMP   .case@23
.lb_skip@29:
   LDY   #$05
   LDA   (FP),Y
   CMP   #$06
   BEQ   .cmp_true@78
   LDA   #$00
   JMP   .cmp_end@79
.cmp_true@78:
   LDA   #$01
.cmp_end@79:
   LDY   #$13
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@28
   JMP   .case@24
.lb_skip@28:
   LDY   #$05
   LDA   (FP),Y
   CMP   #$07
   BEQ   .cmp_true@80
   LDA   #$00
   JMP   .cmp_end@81
.cmp_true@80:
   LDA   #$01
.cmp_end@81:
   LDY   #$15
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@27
   JMP   .case@25
.lb_skip@27:
   LDY   #$05
   LDA   (FP),Y
   CMP   #$09
   BEQ   .cmp_true@82
   LDA   #$00
   JMP   .cmp_end@83
.cmp_true@82:
   LDA   #$01
.cmp_end@83:
   LDY   #$17
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@26
   JMP   .case@26
.lb_skip@26:
   LDY   #$05
   LDA   (FP),Y
   CMP   #$0A
   BEQ   .cmp_true@84
   LDA   #$00
   JMP   .cmp_end@85
.cmp_true@84:
   LDA   #$01
.cmp_end@85:
   LDY   #$19
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@25
   JMP   .case@27
.lb_skip@25:
   LDY   #$05
   LDA   (FP),Y
   CMP   #$0B
   BEQ   .cmp_true@86
   LDA   #$00
   JMP   .cmp_end@87
.cmp_true@86:
   LDA   #$01
.cmp_end@87:
   LDY   #$1B
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@24
   JMP   .case@28
.lb_skip@24:
   LDY   #$05
   LDA   (FP),Y
   CMP   #$0C
   BEQ   .cmp_true@88
   LDA   #$00
   JMP   .cmp_end@89
.cmp_true@88:
   LDA   #$01
.cmp_end@89:
   LDY   #$1D
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@23
   JMP   .case@29
.lb_skip@23:
   LDY   #$05
   LDA   (FP),Y
   CMP   #$0D
   BEQ   .cmp_true@90
   LDA   #$00
   JMP   .cmp_end@91
.cmp_true@90:
   LDA   #$01
.cmp_end@91:
   LDY   #$1F
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@22
   JMP   .case@30
.lb_skip@22:
   LDY   #$05
   LDA   (FP),Y
   CMP   #$0E
   BEQ   .cmp_true@92
   LDA   #$00
   JMP   .cmp_end@93
.cmp_true@92:
   LDA   #$01
.cmp_end@93:
   LDY   #$21
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@21
   JMP   .case@31
.lb_skip@21:
   LDY   #$05
   LDA   (FP),Y
   CMP   #$0F
   BEQ   .cmp_true@94
   LDA   #$00
   JMP   .cmp_end@95
.cmp_true@94:
   LDA   #$01
.cmp_end@95:
   LDY   #$23
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@20
   JMP   .case@32
.lb_skip@20:
   JMP   .default@33
.case@18:
   LDY   #$04
   LDA   (FP),Y
   CMP   #$00
   BNE   .thread@4
   DEY
   LDA   (FP),Y
   CMP   #$00
   BEQ   .cmp_true@96
.thread@4:
   LDA   #$00
   JMP   .cmp_end@97
.cmp_true@96:
   LDA   #$01
.cmp_end@97:
   LDY   #$25
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$27
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
   JSR   branch
   JMP   .switch@17_break
.case@19:
   JSR   instr_get_sibling
   JMP   .switch@17_break
.case@20:
   JSR   instr_get_child
   JMP   .switch@17_break
.case@21:
   JSR   instr_get_parent
   JMP   .switch@17_break
.case@22:
   JSR   instr_get_prop_len
   JMP   .switch@17_break
.case@23:
   JSR   inc_var
   LDY   #$28
   STA   (FP),Y
   TXA
   INY
   STA   (FP),Y
   JMP   .switch@17_break
.case@24:
   JSR   dec_var
   LDY   #$2A
   STA   (FP),Y
   TXA
   INY
   STA   (FP),Y
   JMP   .switch@17_break
.case@25:
   SEC
   LDA   SSP
   SBC   #$02
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDY   #$03
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$04
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   JSR   load_address
   JSR   print_zstring
   JMP   .switch@17_break
.case@26:
   JSR   instr_remove_obj
   JMP   .switch@17_break
.case@27:
   LDY   #$03
   LDA   (FP),Y
   LDY   #$2C
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
   JSR   print_obj
   JMP   .switch@17_break
.case@28:
   SEC
   LDA   SSP
   SBC   #$02
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDY   #$03
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$04
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   JSR   return_value
   JMP   .switch@17_break
.case@29:
   LDY   #$03
   LDA   (FP),Y
   SEC
   SBC   #$02
   LDY   #$2D
   STA   (FP),Y
   LDY   #$04
   LDA   (FP),Y
   SBC   #$00
   LDY   #$2E
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$02
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   DEY
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$2E
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   JSR   branch_to_offset
   JMP   .switch@17_break
.case@30:
   SEC
   LDA   SSP
   SBC   #$02
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDY   #$03
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$04
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   JSR   load_packed_address
   JSR   print_zstring
   JMP   .switch@17_break
.case@31:
   LDY   #$03
   LDA   (FP),Y
   LDY   #$2F
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
   JSR   var_get
   LDY   #$30
   STA   (FP),Y
   TXA
   INY
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$02
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   DEY
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$31
   LDA   (FP),Y
.xjump@1:
   LDY   #$02
   STA   (SSP),Y
   JSR   store_result
   JMP   .switch@17_break
.case@32:
   LDY   #$03
   LDA   (FP),Y
   EOR   #$FF
   LDY   #$32
   STA   (FP),Y
   LDY   #$04
   LDA   (FP),Y
   EOR   #$FF
   LDY   #$33
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$02
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   DEY
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$33
   LDA   (FP),Y
   JMP   .xjump@1
.default@33:
   JSR   internal_error
.switch@17_break:

   ; epilogue
   CLC
   LDA   FP
   ADC   #$36
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

do_2op:
   SUBROUTINE

   ; prologue: 1 arg bytes, 157 local bytes
   SEC
   LDA   SSP
   SBC   #$9F
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$9E
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   LDA   OPERANDS
   LDY   #$01
   STA   (FP),Y
   LDA   OPERANDS+1
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$03
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$04
   STA   (FP),Y
   LDA   OPERANDS+2
   INY
   STA   (FP),Y
   LDA   OPERANDS+3
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$07
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$08
   STA   (FP),Y
   LDY   #$A0
   LDA   (FP),Y
   LDY   #$09
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CMP   #$01
   BEQ   .cmp_true@99
   LDA   #$00
   JMP   .cmp_end@100
.cmp_true@99:
   LDA   #$01
.cmp_end@100:
   LDY   #$0B
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@59
   JMP   .case@35
.lb_skip@59:
   LDY   #$09
   LDA   (FP),Y
   CMP   #$02
   BEQ   .cmp_true@101
   LDA   #$00
   JMP   .cmp_end@102
.cmp_true@101:
   LDA   #$01
.cmp_end@102:
   LDY   #$0D
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@58
   JMP   .case@36
.lb_skip@58:
   LDY   #$09
   LDA   (FP),Y
   CMP   #$03
   BEQ   .cmp_true@103
   LDA   #$00
   JMP   .cmp_end@104
.cmp_true@103:
   LDA   #$01
.cmp_end@104:
   LDY   #$0F
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@57
   JMP   .case@37
.lb_skip@57:
   LDY   #$09
   LDA   (FP),Y
   CMP   #$04
   BEQ   .cmp_true@105
   LDA   #$00
   JMP   .cmp_end@106
.cmp_true@105:
   LDA   #$01
.cmp_end@106:
   LDY   #$11
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@56
   JMP   .case@38
.lb_skip@56:
   LDY   #$09
   LDA   (FP),Y
   CMP   #$05
   BEQ   .cmp_true@107
   LDA   #$00
   JMP   .cmp_end@108
.cmp_true@107:
   LDA   #$01
.cmp_end@108:
   LDY   #$13
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@55
   JMP   .case@39
.lb_skip@55:
   LDY   #$09
   LDA   (FP),Y
   CMP   #$06
   BEQ   .cmp_true@109
   LDA   #$00
   JMP   .cmp_end@110
.cmp_true@109:
   LDA   #$01
.cmp_end@110:
   LDY   #$15
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@54
   JMP   .case@40
.lb_skip@54:
   LDY   #$09
   LDA   (FP),Y
   CMP   #$07
   BEQ   .cmp_true@111
   LDA   #$00
   JMP   .cmp_end@112
.cmp_true@111:
   LDA   #$01
.cmp_end@112:
   LDY   #$17
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@53
   JMP   .case@41
.lb_skip@53:
   LDY   #$09
   LDA   (FP),Y
   CMP   #$08
   BEQ   .cmp_true@113
   LDA   #$00
   JMP   .cmp_end@114
.cmp_true@113:
   LDA   #$01
.cmp_end@114:
   LDY   #$19
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@52
   JMP   .case@42
.lb_skip@52:
   LDY   #$09
   LDA   (FP),Y
   CMP   #$09
   BEQ   .cmp_true@115
   LDA   #$00
   JMP   .cmp_end@116
.cmp_true@115:
   LDA   #$01
.cmp_end@116:
   LDY   #$1B
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@51
   JMP   .case@43
.lb_skip@51:
   LDY   #$09
   LDA   (FP),Y
   CMP   #$0A
   BEQ   .cmp_true@117
   LDA   #$00
   JMP   .cmp_end@118
.cmp_true@117:
   LDA   #$01
.cmp_end@118:
   LDY   #$1D
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@50
   JMP   .case@44
.lb_skip@50:
   LDY   #$09
   LDA   (FP),Y
   CMP   #$0B
   BEQ   .cmp_true@119
   LDA   #$00
   JMP   .cmp_end@120
.cmp_true@119:
   LDA   #$01
.cmp_end@120:
   LDY   #$1F
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@49
   JMP   .case@45
.lb_skip@49:
   LDY   #$09
   LDA   (FP),Y
   CMP   #$0C
   BEQ   .cmp_true@121
   LDA   #$00
   JMP   .cmp_end@122
.cmp_true@121:
   LDA   #$01
.cmp_end@122:
   LDY   #$21
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@48
   JMP   .case@46
.lb_skip@48:
   LDY   #$09
   LDA   (FP),Y
   CMP   #$0D
   BEQ   .cmp_true@123
   LDA   #$00
   JMP   .cmp_end@124
.cmp_true@123:
   LDA   #$01
.cmp_end@124:
   LDY   #$23
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@47
   JMP   .case@47
.lb_skip@47:
   LDY   #$09
   LDA   (FP),Y
   CMP   #$0E
   BEQ   .cmp_true@125
   LDA   #$00
   JMP   .cmp_end@126
.cmp_true@125:
   LDA   #$01
.cmp_end@126:
   LDY   #$25
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@46
   JMP   .case@48
.lb_skip@46:
   LDY   #$09
   LDA   (FP),Y
   CMP   #$0F
   BEQ   .cmp_true@127
   LDA   #$00
   JMP   .cmp_end@128
.cmp_true@127:
   LDA   #$01
.cmp_end@128:
   LDY   #$27
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@45
   JMP   .case@49
.lb_skip@45:
   LDY   #$09
   LDA   (FP),Y
   CMP   #$10
   BEQ   .cmp_true@129
   LDA   #$00
   JMP   .cmp_end@130
.cmp_true@129:
   LDA   #$01
.cmp_end@130:
   LDY   #$29
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@44
   JMP   .case@50
.lb_skip@44:
   LDY   #$09
   LDA   (FP),Y
   CMP   #$11
   BEQ   .cmp_true@131
   LDA   #$00
   JMP   .cmp_end@132
.cmp_true@131:
   LDA   #$01
.cmp_end@132:
   LDY   #$2B
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@43
   JMP   .case@51
.lb_skip@43:
   LDY   #$09
   LDA   (FP),Y
   CMP   #$12
   BEQ   .cmp_true@133
   LDA   #$00
   JMP   .cmp_end@134
.cmp_true@133:
   LDA   #$01
.cmp_end@134:
   LDY   #$2D
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@42
   JMP   .case@52
.lb_skip@42:
   LDY   #$09
   LDA   (FP),Y
   CMP   #$13
   BEQ   .cmp_true@135
   LDA   #$00
   JMP   .cmp_end@136
.cmp_true@135:
   LDA   #$01
.cmp_end@136:
   LDY   #$2F
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@41
   JMP   .case@53
.lb_skip@41:
   LDY   #$09
   LDA   (FP),Y
   CMP   #$14
   BEQ   .cmp_true@137
   LDA   #$00
   JMP   .cmp_end@138
.cmp_true@137:
   LDA   #$01
.cmp_end@138:
   LDY   #$31
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@40
   JMP   .case@54
.lb_skip@40:
   LDY   #$09
   LDA   (FP),Y
   CMP   #$15
   BEQ   .cmp_true@139
   LDA   #$00
   JMP   .cmp_end@140
.cmp_true@139:
   LDA   #$01
.cmp_end@140:
   LDY   #$33
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@39
   JMP   .case@55
.lb_skip@39:
   LDY   #$09
   LDA   (FP),Y
   CMP   #$16
   BEQ   .cmp_true@141
   LDA   #$00
   JMP   .cmp_end@142
.cmp_true@141:
   LDA   #$01
.cmp_end@142:
   LDY   #$35
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@38
   JMP   .case@56
.lb_skip@38:
   LDY   #$09
   LDA   (FP),Y
   CMP   #$17
   BEQ   .cmp_true@143
   LDA   #$00
   JMP   .cmp_end@144
.cmp_true@143:
   LDA   #$01
.cmp_end@144:
   LDY   #$37
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@37
   JMP   .case@57
.lb_skip@37:
   LDY   #$09
   LDA   (FP),Y
   CMP   #$18
   BEQ   .cmp_true@145
   LDA   #$00
   JMP   .cmp_end@146
.cmp_true@145:
   LDA   #$01
.cmp_end@146:
   LDY   #$39
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@36
   JMP   .case@58
.lb_skip@36:
   JMP   .default@59
.case@35:
   LDA   OPERAND_COUNT
   LDY   #$3B
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   SEC
   SBC   #$01
   LDY   #$3D
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   SBC   #$00
   LDY   #$3E
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$3F
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
   BEQ   .cmp_true@147
   LDA   #$00
   JMP   .cmp_end@148
.cmp_true@147:
   LDA   #$01
.cmp_end@148:
   LDY   #$43
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@22
   JSR   internal_error
.if_end@22:
   LDY   #$04
   LDA   (FP),Y
   LDY   #$08
   CMP   (FP),Y
   BNE   .thread@5
   LDY   #$03
   LDA   (FP),Y
   LDY   #$07
   CMP   (FP),Y
   BEQ   .cmp_true@149
.thread@5:
   LDA   #$00
   JMP   .cmp_end@150
.cmp_true@149:
   LDA   #$01
.cmp_end@150:
   LDY   #$45
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$47
   STA   (FP),Y
   INY
   STA   (FP),Y
   ORA   #$00
   BEQ   .lnot_true@152
   LDA   #$00
   JMP   .lnot_end@153
.lnot_true@152:
   LDA   #$01
.lnot_end@153:
   LDY   #$49
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@23
   LDY   #$40
   LDA   (FP),Y
   SEC
   SBC   #$01
   LDY   #$4B
   STA   (FP),Y
   LDY   #$40
   STA   (FP),Y
   LDY   #$4C
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@154
   LDA   #$00
   JMP   .cmp_end@155
.cmp_true@154:
   LDA   #$01
.cmp_end@155:
   LDY   #$4E
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@23
   LDA   #$01
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   JMP   .and_end@24
.and_false@23:
   LDA   #$00
   LDY   #$50
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@24:
   LDY   #$50
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@35
   JMP   .if_end@25
.lb_skip@35:
   LDA   OPERANDS+4
   LDY   #$52
   STA   (FP),Y
   LDA   OPERANDS+5
   INY
   STA   (FP),Y
   LDY   #$04
   LDA   (FP),Y
   LDY   #$53
   CMP   (FP),Y
   BNE   .thread@6
   LDY   #$03
   LDA   (FP),Y
   LDY   #$52
   CMP   (FP),Y
   BEQ   .cmp_true@156
.thread@6:
   LDA   #$00
   JMP   .cmp_end@157
.cmp_true@156:
   LDA   #$01
.cmp_end@157:
   LDY   #$54
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$56
   STA   (FP),Y
   LDY   #$48
   STA   (FP),Y
   ORA   #$00
   BEQ   .lnot_true@159
   LDA   #$00
   JMP   .lnot_end@160
.lnot_true@159:
   LDA   #$01
.lnot_end@160:
   LDY   #$57
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@26
   LDY   #$40
   LDA   (FP),Y
   SEC
   SBC   #$01
   LDY   #$59
   STA   (FP),Y
   LDY   #$40
   STA   (FP),Y
   LDY   #$5A
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@161
   LDA   #$00
   JMP   .cmp_end@162
.cmp_true@161:
   LDA   #$01
.cmp_end@162:
   LDY   #$5C
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .and_false@26
   LDA   #$01
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   JMP   .and_end@27
.and_false@26:
   LDA   #$00
   LDY   #$5E
   STA   (FP),Y
   INY
   STA   (FP),Y
.and_end@27:
   LDY   #$5E
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@28
   LDA   OPERANDS+6
   INY
   STA   (FP),Y
   LDA   OPERANDS+7
   INY
   STA   (FP),Y
   LDY   #$04
   LDA   (FP),Y
   LDY   #$61
   CMP   (FP),Y
   BNE   .thread@7
   LDY   #$03
   LDA   (FP),Y
   LDY   #$60
   CMP   (FP),Y
   BEQ   .cmp_true@163
.thread@7:
   LDA   #$00
   JMP   .cmp_end@164
.cmp_true@163:
   LDA   #$01
.cmp_end@164:
   LDY   #$62
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$64
   STA   (FP),Y
   LDY   #$48
   STA   (FP),Y
.if_end@28:
.if_end@25:
   SEC
   LDA   SSP
   SBC   #$01
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDY   #$48
   LDA   (FP),Y
.xjump@2:
   LDY   #$01
   STA   (SSP),Y
   JSR   branch
   JMP   .switch@34_break
.case@36:
   LDY   #$03
   LDA   (FP),Y
   LDY   #$65
   STA   (FP),Y
   LDY   #$04
   LDA   (FP),Y
   LDY   #$66
   STA   (FP),Y
   LDY   #$07
   LDA   (FP),Y
   LDY   #$67
   STA   (FP),Y
   LDY   #$08
   LDA   (FP),Y
   LDY   #$68
   STA   (FP),Y
   LDY   #$65
   LDA   (FP),Y
   SEC
   LDY   #$67
   SBC   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$68
   SBC   (FP),Y
   BVC   .cmp_novf@166
   EOR   #$80
.cmp_novf@166:
   ASL
   LDA   #$00
   ROL
   LDY   #$69
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$6B
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$01
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   (FP),Y
   JMP   .xjump@2
.case@37:
   LDY   #$03
   LDA   (FP),Y
   LDY   #$6C
   STA   (FP),Y
   LDY   #$04
   LDA   (FP),Y
   LDY   #$6D
   STA   (FP),Y
   LDY   #$07
   LDA   (FP),Y
   LDY   #$6E
   STA   (FP),Y
   LDY   #$08
   LDA   (FP),Y
   LDY   #$6F
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   SEC
   LDY   #$6C
   SBC   (FP),Y
   LDY   #$6F
   LDA   (FP),Y
   LDY   #$6D
   SBC   (FP),Y
   BVC   .cmp_novf@167
   EOR   #$80
.cmp_novf@167:
   ASL
   LDA   #$00
   ROL
   LDY   #$70
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$72
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$01
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   (FP),Y
   JMP   .xjump@2
.case@38:
   JSR   dec_var
   LDY   #$73
   STA   (FP),Y
   TXA
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$75
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$76
   STA   (FP),Y
   LDY   #$07
   LDA   (FP),Y
   LDY   #$77
   STA   (FP),Y
   LDY   #$08
   LDA   (FP),Y
   LDY   #$78
   STA   (FP),Y
   LDY   #$75
   LDA   (FP),Y
   SEC
   LDY   #$77
   SBC   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$78
   SBC   (FP),Y
   BVC   .cmp_novf@168
   EOR   #$80
.cmp_novf@168:
   ASL
   LDA   #$00
   ROL
   LDY   #$79
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$7B
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$01
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   (FP),Y
   JMP   .xjump@2
.case@39:
   JSR   inc_var
   LDY   #$7C
   STA   (FP),Y
   TXA
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$7E
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$7F
   STA   (FP),Y
   LDY   #$07
   LDA   (FP),Y
   LDY   #$80
   STA   (FP),Y
   LDY   #$08
   LDA   (FP),Y
   LDY   #$81
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   SEC
   LDY   #$7E
   SBC   (FP),Y
   LDY   #$81
   LDA   (FP),Y
   LDY   #$7F
   SBC   (FP),Y
   BVC   .cmp_novf@169
   EOR   #$80
.cmp_novf@169:
   ASL
   LDA   #$00
   ROL
   LDY   #$82
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$84
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$01
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   (FP),Y
   JMP   .xjump@2
.case@40:
   JSR   instr_jin
   JMP   .switch@34_break
.case@41:
   LDY   #$03
   LDA   (FP),Y
   LDY   #$07
   AND   (FP),Y
   LDY   #$85
   STA   (FP),Y
   LDY   #$04
   LDA   (FP),Y
   LDY   #$08
   AND   (FP),Y
   LDY   #$86
   STA   (FP),Y
   LDY   #$08
   CMP   (FP),Y
   BNE   .thread@8
   LDY   #$85
   LDA   (FP),Y
   LDY   #$07
   CMP   (FP),Y
   BEQ   .cmp_true@170
.thread@8:
   LDA   #$00
   JMP   .cmp_end@171
.cmp_true@170:
   LDA   #$01
.cmp_end@171:
   LDY   #$87
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$89
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$01
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   (FP),Y
   JMP   .xjump@2
.case@42:
   LDY   #$03
   LDA   (FP),Y
   LDY   #$07
   ORA   (FP),Y
   LDY   #$8A
   STA   (FP),Y
   LDY   #$04
   LDA   (FP),Y
   LDY   #$08
   ORA   (FP),Y
   LDY   #$8B
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$02
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   DEY
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$8B
   LDA   (FP),Y
.xjump@3:
   LDY   #$02
   STA   (SSP),Y
   JSR   store_result
   JMP   .switch@34_break
.case@43:
   LDY   #$03
   LDA   (FP),Y
   LDY   #$07
   AND   (FP),Y
   LDY   #$8C
   STA   (FP),Y
   LDY   #$04
   LDA   (FP),Y
   LDY   #$08
   AND   (FP),Y
   LDY   #$8D
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$02
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   DEY
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$8D
   LDA   (FP),Y
   JMP   .xjump@3
.case@44:
   JSR   instr_test_attr
   JMP   .switch@34_break
.case@45:
   JSR   instr_set_attr
   JMP   .switch@34_break
.case@46:
   JSR   instr_clear_attr
   JMP   .switch@34_break
.case@47:
   LDY   #$03
   LDA   (FP),Y
   LDY   #$8E
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$03
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$07
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$08
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   JSR   var_put
   JMP   .switch@34_break
.case@48:
   JSR   instr_insert_obj
   JMP   .switch@34_break
.case@49:
   LDY   #$07
   LDA   (FP),Y
   LDY   #$8F
   STA   (FP),Y
   ASL
   STA   (FP),Y
   LDY   #$08
   LDA   (FP),Y
   LDY   #$90
   STA   (FP),Y
   ROL
   STA   (FP),Y
   LDY   #$03
   LDA   (FP),Y
   CLC
   LDY   #$8F
   ADC   (FP),Y
   LDY   #$91
   STA   (FP),Y
   LDY   #$04
   LDA   (FP),Y
   LDY   #$90
   ADC   (FP),Y
   LDY   #$92
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$02
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   DEY
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$92
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   JSR   load_address
   JSR   get_next_code_word
   LDY   #$93
   STA   (FP),Y
   TXA
   INY
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$02
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   DEY
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$94
   LDA   (FP),Y
   JMP   .xjump@3
.case@50:
   LDY   #$03
   LDA   (FP),Y
   CLC
   LDY   #$07
   ADC   (FP),Y
   LDY   #$95
   STA   (FP),Y
   LDY   #$04
   LDA   (FP),Y
   LDY   #$08
   ADC   (FP),Y
   LDY   #$96
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$02
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   DEY
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$96
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   JSR   load_address
   JSR   get_next_code_byte2
   LDY   #$97
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$02
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   DEY
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$99
   LDA   (FP),Y
   JMP   .xjump@3
.case@51:
   JSR   instr_get_prop
   JMP   .switch@34_break
.case@52:
   JSR   instr_get_prop_addr
   JMP   .switch@34_break
.case@53:
   JSR   instr_get_next_prop
   JMP   .switch@34_break
.case@54:
   LDY   #$03
   LDA   (FP),Y
   CLC
   LDY   #$07
   ADC   (FP),Y
   LDY   #$9A
   STA   (FP),Y
   LDY   #$04
   LDA   (FP),Y
   LDY   #$08
   ADC   (FP),Y
   LDY   #$9B
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$02
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   DEY
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$9B
   LDA   (FP),Y
   JMP   .xjump@3
.case@55:
   LDY   #$03
   LDA   (FP),Y
   SEC
   LDY   #$07
   SBC   (FP),Y
   LDY   #$9C
   STA   (FP),Y
   LDY   #$04
   LDA   (FP),Y
   LDY   #$08
   SBC   (FP),Y
   LDY   #$9D
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$02
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   DEY
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$9D
   LDA   (FP),Y
   JMP   .xjump@3
.case@56:
   JSR   instr_mul
   JMP   .switch@34_break
.case@57:
   JSR   instr_div
   JMP   .switch@34_break
.case@58:
   JSR   instr_mod
   JMP   .switch@34_break
.default@59:
   JSR   internal_error
.switch@34_break:

   ; epilogue
   CLC
   LDA   FP
   ADC   #$A0
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$9E
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   RTS

do_var:
   SUBROUTINE

   ; prologue: 1 arg bytes, 67 local bytes
   SEC
   LDA   SSP
   SBC   #$45
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$44
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   LDA   OPERANDS
   LDY   #$01
   STA   (FP),Y
   LDA   OPERANDS+1
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$03
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$04
   STA   (FP),Y
   LDY   #$46
   LDA   (FP),Y
   LDY   #$05
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   BEQ   .cmp_true@173
   LDA   #$00
   JMP   .cmp_end@174
.cmp_true@173:
   LDA   #$01
.cmp_end@174:
   LDY   #$07
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@69
   JMP   .case@61
.lb_skip@69:
   LDY   #$05
   LDA   (FP),Y
   CMP   #$01
   BEQ   .cmp_true@175
   LDA   #$00
   JMP   .cmp_end@176
.cmp_true@175:
   LDA   #$01
.cmp_end@176:
   LDY   #$09
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@68
   JMP   .case@62
.lb_skip@68:
   LDY   #$05
   LDA   (FP),Y
   CMP   #$02
   BEQ   .cmp_true@177
   LDA   #$00
   JMP   .cmp_end@178
.cmp_true@177:
   LDA   #$01
.cmp_end@178:
   LDY   #$0B
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@67
   JMP   .case@63
.lb_skip@67:
   LDY   #$05
   LDA   (FP),Y
   CMP   #$03
   BEQ   .cmp_true@179
   LDA   #$00
   JMP   .cmp_end@180
.cmp_true@179:
   LDA   #$01
.cmp_end@180:
   LDY   #$0D
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@66
   JMP   .case@64
.lb_skip@66:
   LDY   #$05
   LDA   (FP),Y
   CMP   #$04
   BEQ   .cmp_true@181
   LDA   #$00
   JMP   .cmp_end@182
.cmp_true@181:
   LDA   #$01
.cmp_end@182:
   LDY   #$0F
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@65
   JMP   .case@65
.lb_skip@65:
   LDY   #$05
   LDA   (FP),Y
   CMP   #$05
   BEQ   .cmp_true@183
   LDA   #$00
   JMP   .cmp_end@184
.cmp_true@183:
   LDA   #$01
.cmp_end@184:
   LDY   #$11
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@64
   JMP   .case@66
.lb_skip@64:
   LDY   #$05
   LDA   (FP),Y
   CMP   #$06
   BEQ   .cmp_true@185
   LDA   #$00
   JMP   .cmp_end@186
.cmp_true@185:
   LDA   #$01
.cmp_end@186:
   LDY   #$13
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@63
   JMP   .case@67
.lb_skip@63:
   LDY   #$05
   LDA   (FP),Y
   CMP   #$07
   BEQ   .cmp_true@187
   LDA   #$00
   JMP   .cmp_end@188
.cmp_true@187:
   LDA   #$01
.cmp_end@188:
   LDY   #$15
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@62
   JMP   .case@68
.lb_skip@62:
   LDY   #$05
   LDA   (FP),Y
   CMP   #$08
   BEQ   .cmp_true@189
   LDA   #$00
   JMP   .cmp_end@190
.cmp_true@189:
   LDA   #$01
.cmp_end@190:
   LDY   #$17
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@61
   JMP   .case@69
.lb_skip@61:
   LDY   #$05
   LDA   (FP),Y
   CMP   #$09
   BEQ   .cmp_true@191
   LDA   #$00
   JMP   .cmp_end@192
.cmp_true@191:
   LDA   #$01
.cmp_end@192:
   LDY   #$19
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@60
   JMP   .case@70
.lb_skip@60:
   JMP   .default@71
.case@61:
   JSR   instr_call
   JMP   .switch@60_break
.case@62:
   LDY   #$03
   LDA   (FP),Y
   LDY   #$1B
   STA   (FP),Y
   LDY   #$04
   LDA   (FP),Y
   LDY   #$1C
   STA   (FP),Y
   LDA   Z_HEADER_ADDR
   CLC
   DEY
   ADC   (FP),Y
   LDY   #$1D
   STA   (FP),Y
   LDA   Z_HEADER_ADDR+1
   DEY
   ADC   (FP),Y
   LDY   #$1E
   STA   (FP),Y
   LDA   OPERANDS+2
   INY
   STA   (FP),Y
   LDA   OPERANDS+3
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$21
   STA   (FP),Y
   ASL
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$22
   STA   (FP),Y
   ROL
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$23
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$24
   STA   (FP),Y
   LDY   #$1D
   LDA   (FP),Y
   CLC
   LDY   #$23
   ADC   (FP),Y
   LDY   #$25
   STA   (FP),Y
   LDY   #$1E
   LDA   (FP),Y
   LDY   #$24
   ADC   (FP),Y
   LDY   #$26
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$27
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$28
   STA   (FP),Y
   LDA   OPERANDS+4
   INY
   STA   (FP),Y
   LDA   OPERANDS+5
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$2D
   STA   (FP),Y
   LDY   #$27
   LDA   (FP),Y
   CLC
   LDY   #$2E
   STA   (FP),Y
   LDY   #$28
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
   LDY   #$2D
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDA   OPERANDS+4
   LDY   #$30
   STA   (FP),Y
   LDA   OPERANDS+5
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$32
   STA   (FP),Y
   LDY   #$27
   LDA   (FP),Y
   CLC
   ADC   #$01
   LDY   #$33
   STA   (FP),Y
   LDY   #$28
   LDA   (FP),Y
   ADC   #$00
   LDY   #$34
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
   JMP   .switch@60_break
.case@63:
   LDA   OPERANDS+4
   LDY   #$35
   STA   (FP),Y
   LDA   OPERANDS+5
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$37
   STA   (FP),Y
   LDA   OPERANDS+2
   INY
   STA   (FP),Y
   LDA   OPERANDS+3
   INY
   STA   (FP),Y
   LDY   #$03
   LDA   (FP),Y
   CLC
   LDY   #$38
   ADC   (FP),Y
   LDY   #$3A
   STA   (FP),Y
   LDY   #$04
   LDA   (FP),Y
   LDY   #$39
   ADC   (FP),Y
   LDY   #$3B
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$3C
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$3D
   STA   (FP),Y
   LDA   Z_HEADER_ADDR
   CLC
   DEY
   ADC   (FP),Y
   LDY   #$3E
   STA   (FP),Y
   LDA   Z_HEADER_ADDR+1
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
   LDY   #$37
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   JMP   .switch@60_break
.case@64:
   JSR   instr_put_prop
   JMP   .switch@60_break
.case@65:
   JSR   instr_sread
   JMP   .switch@60_break
.case@66:
   LDY   #$03
   LDA   (FP),Y
   LDY   #$40
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
   JSR   buffer_char
   JMP   .switch@60_break
.case@67:
   SEC
   LDA   SSP
   SBC   #$02
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDY   #$03
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$04
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   JSR   print_number
   JMP   .switch@60_break
.case@68:
   JSR   instr_random
   JMP   .switch@60_break
.case@69:
   SEC
   LDA   SSP
   SBC   #$02
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDY   #$03
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$04
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   JSR   push
   JMP   .switch@60_break
.case@70:
   LDY   #$03
   LDA   (FP),Y
   LDY   #$41
   STA   (FP),Y
   JSR   pop
   LDY   #$42
   STA   (FP),Y
   TXA
   INY
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$03
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDY   #$41
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$42
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$43
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   JSR   var_put
   JMP   .switch@60_break
.default@71:
   JSR   internal_error
.switch@60_break:

   ; epilogue
   CLC
   LDA   FP
   ADC   #$46
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$44
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   RTS

do_instruction:
   SUBROUTINE

   ; prologue: 0 arg bytes, 140 local bytes
   SEC
   LDA   SSP
   SBC   #$8E
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$8D
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   LDA   #$00
   LDY   #$01
   STA   (FP),Y
   STA   OPERAND_COUNT
   JSR   get_next_code_byte
   LDY   #$02
   STA   (FP),Y
   INY
   STA   (FP),Y
   STA   CURR_OPCODE
   LDA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   SEC
   SBC   #$80
   INY
   LDA   (FP),Y
   SBC   #$00
   BVC   .cmp_novf@193
   EOR   #$80
.cmp_novf@193:
   ASL
   LDA   #$00
   ROL
   LDY   #$06
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@72
   JMP   .if_end@29
.lb_skip@72:
   LDY   #$03
   LDA   (FP),Y
   LDY   #$08
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   AND   #$40
   LDY   #$0A
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDA   #$00
   LDY   #$0B
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .cond_else@30
   JSR   get_var_content
   LDY   #$0C
   STA   (FP),Y
   TXA
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$0E
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$0F
   STA   (FP),Y
   JMP   .cond_end@31
.cond_else@30:
   JSR   get_next_code_byte
   LDY   #$10
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$13
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$14
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$0E
   STA   (FP),Y
   LDY   #$14
   LDA   (FP),Y
   LDY   #$0F
   STA   (FP),Y
.cond_end@31:
   LDY   #$0E
   LDA   (FP),Y
   STA   OPERANDS
   INY
   LDA   (FP),Y
   STA   OPERANDS+1
   LDY   #$03
   LDA   (FP),Y
   LDY   #$15
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   AND   #$20
   LDY   #$17
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDA   #$00
   LDY   #$18
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .cond_else@32
   JSR   get_var_content
   LDY   #$19
   STA   (FP),Y
   TXA
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$1B
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$1C
   STA   (FP),Y
   JMP   .cond_end@33
.cond_else@32:
   JSR   get_next_code_byte
   LDY   #$1D
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$20
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$21
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$1B
   STA   (FP),Y
   LDY   #$21
   LDA   (FP),Y
   LDY   #$1C
   STA   (FP),Y
.cond_end@33:
   LDY   #$1B
   LDA   (FP),Y
   STA   OPERANDS+2
   INY
   LDA   (FP),Y
   STA   OPERANDS+3
   LDA   #$02
   LDY   #$22
   STA   (FP),Y
   STA   OPERAND_COUNT
   LDY   #$03
   LDA   (FP),Y
   LDY   #$23
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   AND   #$1F
   LDY   #$25
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDA   #$00
   LDY   #$26
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$27
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
   JSR   do_2op

   ; epilogue
   CLC
   LDA   FP
   ADC   #$8E
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$8D
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   RTS
.if_end@29:
   LDY   #$03
   LDA   (FP),Y
   LDY   #$28
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   SEC
   SBC   #$B0
   INY
   LDA   (FP),Y
   SBC   #$00
   BVC   .cmp_novf@194
   EOR   #$80
.cmp_novf@194:
   ASL
   LDA   #$00
   ROL
   LDY   #$2A
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@71
   JMP   .if_end@34
.lb_skip@71:
   LDY   #$03
   LDA   (FP),Y
   LDY   #$2C
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   AND   #$30
   LDY   #$2E
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDA   #$00
   LDY   #$2F
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$30
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
   BEQ   .cmp_true@195
   LDA   #$00
   JMP   .cmp_end@196
.cmp_true@195:
   LDA   #$01
.cmp_end@196:
   LDY   #$34
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_else@36
   JSR   get_const_word
   LDY   #$36
   STA   (FP),Y
   TXA
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$38
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$39
   STA   (FP),Y
   JMP   .if_end@35
.if_else@36:
   LDY   #$31
   LDA   (FP),Y
   LDY   #$3A
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CMP   #$10
   BEQ   .cmp_true@197
   LDA   #$00
   JMP   .cmp_end@198
.cmp_true@197:
   LDA   #$01
.cmp_end@198:
   LDY   #$3C
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_else@38
   JSR   get_next_code_byte
   LDY   #$3E
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$38
   STA   (FP),Y
   LDY   #$40
   LDA   (FP),Y
   LDY   #$39
   STA   (FP),Y
   JMP   .if_end@37
.if_else@38:
   JSR   get_var_content
   LDY   #$41
   STA   (FP),Y
   TXA
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$38
   STA   (FP),Y
   LDY   #$42
   LDA   (FP),Y
   LDY   #$39
   STA   (FP),Y
.if_end@37:
.if_end@35:
   LDA   #$01
   LDY   #$43
   STA   (FP),Y
   STA   OPERAND_COUNT
   LDY   #$38
   LDA   (FP),Y
   STA   OPERANDS
   INY
   LDA   (FP),Y
   STA   OPERANDS+1
   LDY   #$03
   LDA   (FP),Y
   LDY   #$44
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   AND   #$0F
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
   JSR   do_1op

   ; epilogue
   CLC
   LDA   FP
   ADC   #$8E
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$8D
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   RTS
.if_end@34:
   LDY   #$03
   LDA   (FP),Y
   LDY   #$49
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   SEC
   SBC   #$C0
   INY
   LDA   (FP),Y
   SBC   #$00
   BVC   .cmp_novf@199
   EOR   #$80
.cmp_novf@199:
   ASL
   LDA   #$00
   ROL
   LDY   #$4B
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@39
   LDY   #$03
   LDA   (FP),Y
   LDY   #$4D
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   SEC
   SBC   #$B0
   LDY   #$4F
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   SBC   #$00
   LDY   #$50
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$51
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
   JSR   do_0op

   ; epilogue
   CLC
   LDA   FP
   ADC   #$8E
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$8D
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   RTS
.if_end@39:
   JSR   get_next_code_byte
   LDY   #$52
   STA   (FP),Y
   INY
   STA   (FP),Y
.loop@72_start:
   LDY   #$54
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   AND   #$C0
   LDY   #$56
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDA   #$00
   LDY   #$57
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$58
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
   BEQ   .cmp_true@200
   LDA   #$00
   JMP   .cmp_end@201
.cmp_true@200:
   LDA   #$01
.cmp_end@201:
   LDY   #$5C
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_else@41
   JSR   get_const_word
   LDY   #$5E
   STA   (FP),Y
   TXA
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$60
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$61
   STA   (FP),Y
   JMP   .if_end@40
.if_else@41:
   LDY   #$59
   LDA   (FP),Y
   LDY   #$62
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CMP   #$80
   BEQ   .cmp_true@202
   LDA   #$00
   JMP   .cmp_end@203
.cmp_true@202:
   LDA   #$01
.cmp_end@203:
   LDY   #$64
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_else@43
   JSR   get_var_content
   LDY   #$66
   STA   (FP),Y
   TXA
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$60
   STA   (FP),Y
   LDY   #$67
   LDA   (FP),Y
   LDY   #$61
   STA   (FP),Y
   JMP   .if_end@42
.if_else@43:
   LDY   #$59
   LDA   (FP),Y
   LDY   #$68
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CMP   #$40
   BEQ   .cmp_true@204
   LDA   #$00
   JMP   .cmp_end@205
.cmp_true@204:
   LDA   #$01
.cmp_end@205:
   LDY   #$6A
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@70
   JMP   .loop@72_break
.lb_skip@70:
   JSR   get_next_code_byte
   LDY   #$6C
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$60
   STA   (FP),Y
   LDY   #$6E
   LDA   (FP),Y
   LDY   #$61
   STA   (FP),Y
.if_end@42:
.if_end@40:
   LDA   OPERAND_COUNT
   LDY   #$6F
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$71
   STA   (FP),Y
   STA   HARGS
   LDA   #$02
   STA   HARGS+1
   JSR   mul8
   LDA   HARGS+2
   LDY   #$72
   STA   (FP),Y
   TAX
   LDY   #$60
   LDA   (FP),Y
   STA   OPERANDS,X
   INY
   LDA   (FP),Y
   STA   OPERANDS+1,X
   LDA   OPERAND_COUNT
   LDY   #$73
   STA   (FP),Y
   LDA   OPERAND_COUNT
   CLC
   ADC   #$01
   INY
   STA   (FP),Y
   STA   OPERAND_COUNT
   LDY   #$53
   LDA   (FP),Y
   LDY   #$75
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ASL
   LDY   #$77
   STA   (FP),Y
   LDA   #$00
   ROL
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ASL
   STA   (FP),Y
   INY
   LDA   (FP),Y
   ROL
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$79
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ORA   #$03
   LDY   #$7C
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$7D
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$7E
   STA   (FP),Y
   LDY   #$53
   STA   (FP),Y
   JMP   .loop@72_start
.loop@72_break:
   LDY   #$03
   LDA   (FP),Y
   LDY   #$7F
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   SEC
   SBC   #$E0
   INY
   LDA   (FP),Y
   SBC   #$00
   BVC   .cmp_novf@206
   EOR   #$80
.cmp_novf@206:
   ASL
   LDA   #$00
   ROL
   EOR   #$01
   LDY   #$81
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_else@47
   LDY   #$03
   LDA   (FP),Y
   LDY   #$83
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   SEC
   SBC   #$E0
   LDY   #$85
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   SBC   #$00
   LDY   #$86
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$87
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
   JSR   do_var
   JMP   .if_end@46
.if_else@47:
   LDY   #$03
   LDA   (FP),Y
   LDY   #$88
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   AND   #$1F
   LDY   #$8A
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDA   #$00
   LDY   #$8B
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$8C
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
   JSR   do_2op
.if_end@46:

   ; epilogue
   CLC
   LDA   FP
   ADC   #$8E
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$8D
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   RTS
