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
__zork_zp_8d	EQU	$8D
LOCAL_ZVARS	EQU	$9A
__zork_zp_b8	EQU	$B8
Z_HEADER_ADDR	EQU	$BA
__zork_zp_bc	EQU	$BC
STACK_STATE	EQU	$C8
__zork_zp_ce	EQU	$CE
SCRATCH1	EQU	$E4
__zork_zp_e6	EQU	$E6
__zork_zp_e8	EQU	$E8
__zpabi_copy_data_to_buff__src_0	EQU	$F3
__zpabi_copy_data_to_buff__src_1	EQU	$F4
__zpabi_inc_sector_and_write__buffer_0	EQU	$F5
__zpabi_inc_sector_and_write__buffer_1	EQU	$F6
__local_instr_save__pages	EQU	$F8
BUFF_AREA	EQU	$0200
branch	EQU	$0BAA
negated_branch	EQU	$0BB3
buffer_char	EQU	$1B90
dump_buffer_line	EQU	$1BE2
dump_buffer_with_more	EQU	$1C5B
home	EQU	$1CC9
RWTS	EQU	$2900
HOME	EQU	$FC58
CLREOL	EQU	$FC9C
WAIT	EQU	$FCA8
RDKEY	EQU	$FD0C
COUT	EQU	$FDED
COUT1	EQU	$FDF0

; @zp-link-meta-begin
; def copy_data_to_buff params=__zpabi_copy_data_to_buff__src_0,__zpabi_copy_data_to_buff__src_1,__zpabi_copy_data_to_buff__x,__zpabi_copy_data_to_buff__count locals=__local_copy_data_to_buff__0 indirect=false in_cycle=false param_regs=,,X,Y
; def instr_save params= locals=__local_instr_save__0,__local_instr_save__pages indirect=false in_cycle=false
; ext CLREOL params=
; ext COUT params=__zpabi_COUT__c param_regs=A
; ext COUT1 params=__zpabi_COUT1__c param_regs=A
; ext HOME params=
; ext RDKEY params=
; ext RWTS params=__zpabi_RWTS__iob_hi,__zpabi_RWTS__iob_lo param_regs=A,Y
; ext WAIT params=__zpabi_WAIT__a param_regs=A
; ext branch params=
; ext buffer_char params=__zpabi_buffer_char__c param_regs=A
; ext dump_buffer_line params=
; ext dump_buffer_with_more params=
; ext home params=
; ext inc_sector_and_write params=__zpabi_inc_sector_and_write__buffer_0,__zpabi_inc_sector_and_write__buffer_1
; ext negated_branch params=
; ext please_insert_save_diskette params=
; ext please_reinsert_game_diskette params=
; ext write_next_sector params=
; call instr_save -> branch
; call instr_save -> copy_data_to_buff
; call instr_save -> inc_sector_and_write
; call instr_save -> negated_branch
; call instr_save -> please_insert_save_diskette
; call instr_save -> please_reinsert_game_diskette
; call instr_save -> write_next_sector
; share member=inc_sector_and_write:__zpabi_inc_sector_and_write__buffer_0,__zpabi_inc_sector_and_write__buffer_1 holder=instr_save live=instr_save:inc_sector_and_write forward=instr_save>inc_sector_and_write writer=instr_save
; item copy_data_to_buff section=.text local=true zero=false
; item instr_save section=.text local=false zero=false
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
; zp __zork_zp_8d size=13 at=$8D def=false init=
; zp LOCAL_ZVARS size=30 at=$9A def=false init=
; zp __zork_zp_b8 size=2 at=$B8 def=false init=
; zp Z_HEADER_ADDR size=2 at=$BA def=false init=
; zp __zork_zp_bc size=12 at=$BC def=false init=
; zp STACK_STATE size=6 at=$C8 def=false init=
; zp __zork_zp_ce size=22 at=$CE def=false init=
; zp SCRATCH1 size=2 at=$E4 def=false init=
; zp __zork_zp_e6 size=2 at=$E6 def=false init=
; zp __zork_zp_e8 size=11 at=$E8 def=false init=
; runtime hargs_size=1 hargs_need=0
; @zp-link-meta-end

copy_data_to_buff:
   SUBROUTINE

.loop@0_start:
   DEY
   LDA   (__zpabi_copy_data_to_buff__src_0),Y
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
   LDA   #<Z_PC
   STA   __zpabi_copy_data_to_buff__src_0
   LDA   #>Z_PC
   STA   __zpabi_copy_data_to_buff__src_1
   LDX   #$01
   LDY   #$03
   JSR   copy_data_to_buff
   TAX
   LDA   #<LOCAL_ZVARS
   STA   __zpabi_copy_data_to_buff__src_0
   LDA   #>LOCAL_ZVARS
   STA   __zpabi_copy_data_to_buff__src_1
   LDY   #$1E
   JSR   copy_data_to_buff
   TAX
   LDA   #<STACK_STATE
   STA   __zpabi_copy_data_to_buff__src_0
   LDA   #>STACK_STATE
   STA   __zpabi_copy_data_to_buff__src_1
   LDY   #$06
   JSR   copy_data_to_buff
   JSR   write_next_sector
   BCS   .instr_save@fail
   LDA   #$80
   STA   __zpabi_copy_data_to_buff__src_0
   LDA   #$02
   STA   __zpabi_copy_data_to_buff__src_1
   LDX   #$00
   LDY   #$00
   JSR   copy_data_to_buff
   JSR   write_next_sector
   BCS   .instr_save@fail
   LDA   #$80
   STA   __zpabi_copy_data_to_buff__src_0
   LDA   #$03
   STA   __zpabi_copy_data_to_buff__src_1
   LDX   #$00
   LDY   #$68
   JSR   copy_data_to_buff
   JSR   write_next_sector
   BCS   .instr_save@fail
   LDA   Z_HEADER_ADDR
   STA   __zpabi_inc_sector_and_write__buffer_0
   LDA   Z_HEADER_ADDR+1
   STA   __zpabi_inc_sector_and_write__buffer_1
   LDY   #$0E
   LDA   (Z_HEADER_ADDR),Y
   CLC
   ADC   #$01
   STA   __local_instr_save__pages
.loop@1_start:
   JSR   inc_sector_and_write
   BCS   .instr_save@fail
   LDA   __zpabi_inc_sector_and_write__buffer_0
   CLC
   ADC   #$00
   STA   __zpabi_inc_sector_and_write__buffer_0
   LDA   __zpabi_inc_sector_and_write__buffer_1
   ADC   #$01
   STA   __zpabi_inc_sector_and_write__buffer_1
   DEC   __local_instr_save__pages
   BNE   .loop@1_start
   JSR   inc_sector_and_write
   BCS   .instr_save@fail
   JSR   please_reinsert_game_diskette
   JMP   branch
.instr_save@fail:
   JSR   please_reinsert_game_diskette
   JMP   negated_branch
