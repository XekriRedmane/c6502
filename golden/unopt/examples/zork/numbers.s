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

mulu16:
   SUBROUTINE

   ; prologue: 4 arg bytes, 12 local bytes
   SEC
   LDA   SSP
   SBC   #$0E
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$0D
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
   INY
   STA   (FP),Y
.loop@0_continue:
   LDY   #$12
   LDA   (FP),Y
   BNE   .cmp_true@0
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@0
   LDA   #$00
   JMP   .cmp_end@1
.cmp_true@0:
   LDA   #$01
.cmp_end@1:
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
   JMP   .loop@0_break
.lb_skip@0:
   LDY   #$11
   LDA   (FP),Y
   AND   #$01
   LDY   #$05
   STA   (FP),Y
   LDY   #$12
   LDA   (FP),Y
   LDA   #$00
   LDY   #$06
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@0
   LDY   #$01
   LDA   (FP),Y
   CLC
   LDY   #$0F
   ADC   (FP),Y
   LDY   #$07
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   LDY   #$10
   ADC   (FP),Y
   LDY   #$08
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   LDY   #$08
   LDA   (FP),Y
   LDY   #$02
   STA   (FP),Y
.if_end@0:
   LDY   #$0F
   LDA   (FP),Y
   LDY   #$09
   STA   (FP),Y
   ASL
   STA   (FP),Y
   LDY   #$10
   LDA   (FP),Y
   LDY   #$0A
   STA   (FP),Y
   ROL
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$0F
   STA   (FP),Y
   LDY   #$0A
   LDA   (FP),Y
   LDY   #$10
   STA   (FP),Y
   LDY   #$12
   LDA   (FP),Y
   LDY   #$0C
   STA   (FP),Y
   LSR
   STA   (FP),Y
   LDY   #$11
   LDA   (FP),Y
   LDY   #$0B
   STA   (FP),Y
   ROR
   STA   (FP),Y
   LDY   #$11
   STA   (FP),Y
   LDY   #$0C
   LDA   (FP),Y
   LDY   #$12
   STA   (FP),Y
   JMP   .loop@0_continue
.loop@0_break:
   LDY   #$02
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
   ADC   #$12
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$0D
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

divu16:
   SUBROUTINE

   ; prologue: 4 arg bytes, 41 local bytes
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

   INY
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   LDY   #$2D
   LDA   (FP),Y
   LDY   #$02
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$11
   INY
   STA   (FP),Y
   INY
   STA   (FP),Y
.loop@1_start:
   LDY   #$07
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@3
   LDA   #$00
   JMP   .cmp_end@4
.cmp_true@3:
   LDA   #$01
.cmp_end@4:
   LDY   #$09
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BNE   .lb_skip@1
   JMP   .loop@1_break
.lb_skip@1:
   LDA   #$00
   LDY   #$0B
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$03
   LDA   (FP),Y
   SEC
   LDY   #$2E
   SBC   (FP),Y
   LDY   #$04
   LDA   (FP),Y
   LDY   #$2F
   SBC   (FP),Y
   LDA   #$00
   ROL
   LDY   #$0D
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_end@1
   LDY   #$03
   LDA   (FP),Y
   SEC
   LDY   #$2E
   SBC   (FP),Y
   LDY   #$0F
   STA   (FP),Y
   LDY   #$04
   LDA   (FP),Y
   LDY   #$2F
   SBC   (FP),Y
   LDY   #$10
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$03
   STA   (FP),Y
   LDY   #$10
   LDA   (FP),Y
   LDY   #$04
   STA   (FP),Y
   LDA   #$01
   LDY   #$11
   STA   (FP),Y
   LDY   #$0C
   STA   (FP),Y
.if_end@1:
   LDY   #$02
   LDA   (FP),Y
   ASL
   LDA   #$00
   ROL
   LDY   #$12
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$14
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDY   #$01
   LDA   (FP),Y
   LDY   #$16
   STA   (FP),Y
   ASL
   STA   (FP),Y
   LDY   #$02
   LDA   (FP),Y
   LDY   #$17
   STA   (FP),Y
   ROL
   STA   (FP),Y
   LDY   #$0C
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
   DEY
   LDA   (FP),Y
   LDY   #$1B
   STA   (FP),Y
   LDY   #$16
   LDA   (FP),Y
   LDY   #$1A
   ORA   (FP),Y
   LDY   #$1C
   STA   (FP),Y
   LDY   #$17
   LDA   (FP),Y
   LDY   #$1B
   ORA   (FP),Y
   LDY   #$1D
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   LDY   #$1D
   LDA   (FP),Y
   LDY   #$02
   STA   (FP),Y
   INY
   LDA   (FP),Y
   LDY   #$1E
   STA   (FP),Y
   ASL
   STA   (FP),Y
   LDY   #$04
   LDA   (FP),Y
   LDY   #$1F
   STA   (FP),Y
   ROL
   STA   (FP),Y
   LDY   #$15
   LDA   (FP),Y
   LDY   #$20
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$22
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$23
   STA   (FP),Y
   LDY   #$1E
   LDA   (FP),Y
   LDY   #$22
   ORA   (FP),Y
   LDY   #$24
   STA   (FP),Y
   LDY   #$1F
   LDA   (FP),Y
   LDY   #$23
   ORA   (FP),Y
   LDY   #$25
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$03
   STA   (FP),Y
   LDY   #$25
   LDA   (FP),Y
   LDY   #$04
   STA   (FP),Y
   LDY   #$06
   LDA   (FP),Y
   LDY   #$26
   STA   (FP),Y
   SEC
   SBC   #$01
   INY
   STA   (FP),Y
   LDY   #$06
   STA   (FP),Y
   JMP   .loop@1_start
.loop@1_break:
   LDY   #$04
   LDA   (FP),Y
   LDY   #$29
   STA   (FP),Y
   LSR
   STA   (FP),Y
   LDY   #$03
   LDA   (FP),Y
   LDY   #$28
   STA   (FP),Y
   ROR
   STA   (FP),Y
   STA   remainder
   INY
   LDA   (FP),Y
   STA   remainder+1
   LDY   #$02
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
   ADC   #$2F
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
   PLA
   TAX
   PLA
   RTS

check_sign:
   SUBROUTINE

   ; prologue: 4 arg bytes, 11 local bytes
   SEC
   LDA   SSP
   SBC   #$0D
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$0C
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
   LDY   #$10
   EOR   (FP),Y
   LDY   #$01
   STA   (FP),Y
   LDY   #$0F
   LDA   (FP),Y
   LDY   #$11
   EOR   (FP),Y
   LDY   #$02
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$05
   STA   (FP),Y
   STA   sign_bit
   INY
   STA   (FP),Y
   LDA   sign_bit
   BMI   .sx_neg@5
   LDA   #$00
   JMP   .sx_done@6
.sx_neg@5:
   LDA   #$FF
.sx_done@6:
   LDY   #$07
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
   INY
   ORA   (FP),Y
   BEQ   .if_end@2
   SEC
   LDA   #$00
   LDY   #$0E
   SBC   (FP),Y
   LDY   #$0A
   STA   (FP),Y
   LDA   #$00
   LDY   #$0F
   SBC   (FP),Y
   LDY   #$0B
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$0E
   STA   (FP),Y
   LDY   #$0B
   LDA   (FP),Y
   LDY   #$0F
   STA   (FP),Y
.if_end@2:
   LDY   #$0F
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
   ADC   #$11
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$0C
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

store_with_sign:
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

   LDA   sign_bit
   LDY   #$01
   STA   (FP),Y
   LDA   sign_bit
   BMI   .sx_neg@7
   LDA   #$00
   JMP   .sx_done@8
.sx_neg@7:
   LDA   #$FF
.sx_done@8:
   LDY   #$02
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
   INY
   ORA   (FP),Y
   BEQ   .if_end@3
   SEC
   LDA   #$00
   LDY   #$09
   SBC   (FP),Y
   LDY   #$05
   STA   (FP),Y
   LDA   #$00
   LDY   #$0A
   SBC   (FP),Y
   LDY   #$06
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$09
   STA   (FP),Y
   LDY   #$06
   LDA   (FP),Y
   LDY   #$0A
   STA   (FP),Y
.if_end@3:
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
   JSR   store_result

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

instr_mul:
   SUBROUTINE

   ; prologue: 0 arg bytes, 22 local bytes
   SEC
   LDA   SSP
   SBC   #$18
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$17
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   LDA   OPERANDS+2
   LDY   #$01
   STA   (FP),Y
   LDA   OPERANDS+3
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
   LDA   OPERANDS
   INY
   STA   (FP),Y
   LDA   OPERANDS+1
   INY
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$04
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   DEY
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$06
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   INY
   LDA   (FP),Y
   STA   (SSP),Y
   INY
   LDA   (FP),Y
   STA   (SSP),Y
   JSR   check_sign
   LDY   #$07
   STA   (FP),Y
   TXA
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$09
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$0A
   STA   (FP),Y
   LDY   #$04
   LDA   (FP),Y
   BNE   .thread@0
   DEY
   LDA   (FP),Y
   CMP   #$02
   BEQ   .cmp_true@9
.thread@0:
   LDA   #$00
   JMP   .cmp_end@10
.cmp_true@9:
   LDA   #$01
.cmp_end@10:
   LDY   #$0B
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_else@5
   LDY   #$09
   LDA   (FP),Y
   LDY   #$0D
   STA   (FP),Y
   ASL
   STA   (FP),Y
   LDY   #$0A
   LDA   (FP),Y
   LDY   #$0E
   STA   (FP),Y
   ROL
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$0F
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$10
   STA   (FP),Y
   JMP   .if_end@4
.if_else@5:
   LDY   #$04
   LDA   (FP),Y
   CMP   #$00
   BNE   .thread@1
   DEY
   LDA   (FP),Y
   CMP   #$04
   BEQ   .cmp_true@12
.thread@1:
   LDA   #$00
   JMP   .cmp_end@13
.cmp_true@12:
   LDA   #$01
.cmp_end@13:
   LDY   #$11
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_else@7
   LDY   #$09
   LDA   (FP),Y
   LDY   #$13
   STA   (FP),Y
   ASL
   STA   (FP),Y
   LDY   #$0A
   LDA   (FP),Y
   LDY   #$14
   STA   (FP),Y
   ROL
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
   LDY   #$0F
   STA   (FP),Y
   LDY   #$14
   LDA   (FP),Y
   LDY   #$10
   STA   (FP),Y
   JMP   .if_end@6
.if_else@7:
   SEC
   LDA   SSP
   SBC   #$04
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
   INY
   LDA   (FP),Y
   STA   (SSP),Y
   INY
   LDA   (FP),Y
   STA   (SSP),Y
   JSR   mulu16
   LDY   #$15
   STA   (FP),Y
   TXA
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$0F
   STA   (FP),Y
   LDY   #$16
   LDA   (FP),Y
   LDY   #$10
   STA   (FP),Y
.if_end@6:
.if_end@4:
   SEC
   LDA   SSP
   SBC   #$02
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDY   #$0F
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$10
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   JSR   store_with_sign

   ; epilogue
   CLC
   LDA   FP
   ADC   #$18
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$17
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   RTS

instr_div:
   SUBROUTINE

   ; prologue: 0 arg bytes, 22 local bytes
   SEC
   LDA   SSP
   SBC   #$18
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$17
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   LDA   OPERANDS+2
   LDY   #$01
   STA   (FP),Y
   LDA   OPERANDS+3
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
   LDA   OPERANDS
   INY
   STA   (FP),Y
   LDA   OPERANDS+1
   INY
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$04
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   DEY
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$06
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   INY
   LDA   (FP),Y
   STA   (SSP),Y
   INY
   LDA   (FP),Y
   STA   (SSP),Y
   JSR   check_sign
   LDY   #$07
   STA   (FP),Y
   TXA
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$09
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$0A
   STA   (FP),Y
   LDY   #$04
   LDA   (FP),Y
   BNE   .thread@2
   DEY
   LDA   (FP),Y
   CMP   #$02
   BEQ   .cmp_true@15
.thread@2:
   LDA   #$00
   JMP   .cmp_end@16
.cmp_true@15:
   LDA   #$01
.cmp_end@16:
   LDY   #$0B
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_else@9
   LDY   #$0A
   LDA   (FP),Y
   LDY   #$0E
   STA   (FP),Y
   LSR
   STA   (FP),Y
   LDY   #$09
   LDA   (FP),Y
   LDY   #$0D
   STA   (FP),Y
   ROR
   STA   (FP),Y
   LDY   #$0F
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$10
   STA   (FP),Y
   JMP   .if_end@8
.if_else@9:
   LDY   #$04
   LDA   (FP),Y
   CMP   #$00
   BNE   .thread@3
   DEY
   LDA   (FP),Y
   CMP   #$04
   BEQ   .cmp_true@18
.thread@3:
   LDA   #$00
   JMP   .cmp_end@19
.cmp_true@18:
   LDA   #$01
.cmp_end@19:
   LDY   #$11
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .if_else@11
   LDY   #$0A
   LDA   (FP),Y
   LDY   #$14
   STA   (FP),Y
   LSR
   STA   (FP),Y
   LDY   #$09
   LDA   (FP),Y
   LDY   #$13
   STA   (FP),Y
   ROR
   STA   (FP),Y
   INY
   LDA   (FP),Y
   LSR
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ROR
   STA   (FP),Y
   LDY   #$0F
   STA   (FP),Y
   LDY   #$14
   LDA   (FP),Y
   LDY   #$10
   STA   (FP),Y
   JMP   .if_end@10
.if_else@11:
   SEC
   LDA   SSP
   SBC   #$04
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
   INY
   LDA   (FP),Y
   STA   (SSP),Y
   INY
   LDA   (FP),Y
   STA   (SSP),Y
   JSR   divu16
   LDY   #$15
   STA   (FP),Y
   TXA
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$0F
   STA   (FP),Y
   LDY   #$16
   LDA   (FP),Y
   LDY   #$10
   STA   (FP),Y
.if_end@10:
.if_end@8:
   SEC
   LDA   SSP
   SBC   #$02
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDY   #$0F
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$10
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   JSR   store_with_sign

   ; epilogue
   CLC
   LDA   FP
   ADC   #$18
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$17
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   RTS

instr_mod:
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

   LDA   OPERANDS+2
   LDY   #$01
   STA   (FP),Y
   LDA   OPERANDS+3
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
   LDA   OPERANDS
   INY
   STA   (FP),Y
   LDA   OPERANDS+1
   INY
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$04
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   DEY
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$06
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   INY
   LDA   (FP),Y
   STA   (SSP),Y
   INY
   LDA   (FP),Y
   STA   (SSP),Y
   JSR   check_sign
   LDY   #$07
   STA   (FP),Y
   TXA
   INY
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$04
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
   INY
   LDA   (FP),Y
   STA   (SSP),Y
   INY
   LDA   (FP),Y
   STA   (SSP),Y
   JSR   divu16
   LDY   #$09
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
   LDA   remainder
   LDY   #$01
   STA   (SSP),Y
   LDA   remainder+1
   INY
   STA   (SSP),Y
   JSR   store_result

   ; epilogue
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
   RTS

instr_random:
   SUBROUTINE

   ; prologue: 0 arg bytes, 18 local bytes
   SEC
   LDA   SSP
   SBC   #$14
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDA   FP
   LDY   #$13
   STA   (SSP),Y
   LDA   FP+1
   INY
   STA   (SSP),Y
   LDA   SSP
   STA   FP
   LDA   SSP+1
   STA   FP+1

   LDA   #<RND
   LDY   #$01
   STA   (FP),Y
   LDA   #>RND
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
   ASL
   LDY   #$08
   STA   (FP),Y
   LDA   #$00
   ROL
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$0A
   STA   (FP),Y
   LDY   #$03
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$0A
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDA   RND
   LDY   #$0B
   STA   (FP),Y
   LDA   RND+1
   INY
   STA   (FP),Y
   LDA   OPERANDS
   INY
   STA   (FP),Y
   LDA   OPERANDS+1
   INY
   STA   (FP),Y
   SEC
   LDA   SSP
   SBC   #$04
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDY   #$0B
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$0C
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDY   #$0D
   LDA   (FP),Y
   LDY   #$03
   STA   (SSP),Y
   LDY   #$0E
   LDA   (FP),Y
   LDY   #$04
   STA   (SSP),Y
   JSR   divu16
   LDY   #$0F
   STA   (FP),Y
   TXA
   INY
   STA   (FP),Y
   LDA   remainder
   CLC
   ADC   #$01
   INY
   STA   (FP),Y
   LDA   remainder+1
   ADC   #$00
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
   LDY   #$12
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   JSR   store_result

   ; epilogue
   CLC
   LDA   FP
   ADC   #$14
   STA   SSP
   LDA   FP+1
   ADC   #$00
   STA   SSP+1
   LDY   #$13
   LDA   (FP),Y
   TAX
   INY
   LDA   (FP),Y
   STA   FP+1
   TXA
   STA   FP
   RTS

print_number:
   SUBROUTINE

   ; prologue: 2 arg bytes, 51 local bytes
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

   INY
   LDA   (FP),Y
   LDY   #$01
   STA   (FP),Y
   LDY   #$37
   LDA   (FP),Y
   LDY   #$02
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
   INY
   ORA   (FP),Y
   BEQ   .if_end@12
   LDA   #$2D
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
   JSR   buffer_char
   SEC
   LDA   #$00
   LDY   #$36
   SBC   (FP),Y
   LDY   #$06
   STA   (FP),Y
   LDA   #$00
   LDY   #$37
   SBC   (FP),Y
   LDY   #$07
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$36
   STA   (FP),Y
   LDY   #$07
   LDA   (FP),Y
   LDY   #$37
   STA   (FP),Y
.if_end@12:
   LDA   #$00
   LDY   #$08
   STA   (FP),Y
   INY
   STA   (FP),Y
.loop@2_start:
   SEC
   LDA   SSP
   SBC   #$04
   STA   SSP
   LDA   SSP+1
   SBC   #$00
   STA   SSP+1
   LDY   #$36
   LDA   (FP),Y
   LDY   #$01
   STA   (SSP),Y
   LDY   #$37
   LDA   (FP),Y
   LDY   #$02
   STA   (SSP),Y
   LDA   #$0A
   INY
   STA   (SSP),Y
   LDA   #$00
   INY
   STA   (SSP),Y
   JSR   divu16
   LDY   #$0A
   STA   (FP),Y
   TXA
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$36
   STA   (FP),Y
   LDY   #$0B
   LDA   (FP),Y
   LDY   #$37
   STA   (FP),Y
   LDA   remainder
   LDY   #$0C
   STA   (FP),Y
   CLC
   LDA   FP
   ADC   #$0D
   LDY   #$17
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   LDY   #$09
   LDA   (FP),Y
   LDY   #$12
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$14
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   INY
   LDA   (FP),Y
   CLC
   LDY   #$15
   ADC   (FP),Y
   LDY   #$19
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$16
   ADC   (FP),Y
   LDY   #$1A
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$0C
   LDA   (FP),Y
   LDY   #$00
   STA   (DPTR),Y
   LDY   #$09
   LDA   (FP),Y
   LDY   #$1B
   STA   (FP),Y
   CLC
   ADC   #$01
   INY
   STA   (FP),Y
   LDY   #$09
   STA   (FP),Y
   LDY   #$37
   LDA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@21
   DEY
   LDA   (FP),Y
   CMP   #$00
   BNE   .cmp_true@21
   LDA   #$00
   JMP   .cmp_end@22
.cmp_true@21:
   LDA   #$01
.cmp_end@22:
   LDY   #$1D
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
.loop@3_start:
   LDY   #$09
   LDA   (FP),Y
   LDY   #$1F
   STA   (FP),Y
   SEC
   SBC   #$01
   INY
   STA   (FP),Y
   LDY   #$09
   STA   (FP),Y
   CLC
   LDA   FP
   ADC   #$0D
   LDY   #$26
   STA   (FP),Y
   LDA   FP+1
   ADC   #$00
   INY
   STA   (FP),Y
   LDY   #$09
   LDA   (FP),Y
   LDY   #$21
   STA   (FP),Y
   LDA   #$00
   INY
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
   INY
   LDA   (FP),Y
   CLC
   LDY   #$24
   ADC   (FP),Y
   LDY   #$28
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   LDY   #$25
   ADC   (FP),Y
   LDY   #$29
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   STA   DPTR
   INY
   LDA   (FP),Y
   STA   DPTR+1
   LDY   #$00
   LDA   (DPTR),Y
   LDY   #$2A
   STA   (FP),Y
   INY
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   CLC
   ADC   #$30
   LDY   #$2D
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   ADC   #$00
   LDY   #$2E
   STA   (FP),Y
   DEY
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
   JSR   buffer_char
   LDY   #$09
   LDA   (FP),Y
   LDY   #$30
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   BNE   .cmp_true@24
   LDA   #$00
   JMP   .cmp_end@25
.cmp_true@24:
   LDA   #$01
.cmp_end@25:
   LDY   #$32
   STA   (FP),Y
   LDA   #$00
   INY
   STA   (FP),Y
   DEY
   LDA   (FP),Y
   INY
   ORA   (FP),Y
   BEQ   .lb_skip@2
   JMP   .loop@3_start
.lb_skip@2:

   ; epilogue
   CLC
   LDA   FP
   ADC   #$37
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

sign_bit:
   DS.B  1

remainder:
   DS.B  2
