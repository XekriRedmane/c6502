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
SCRATCH2	EQU	$E6
__zork_zp_e8	EQU	$E8
BUFF_AREA	EQU	$0200
__zpabi_do_rwts_on_sector__command	EQU	$0200
__zpabi_print_ascii_string__count	EQU	$0200
__local_inc_sector_and_write__1	EQU	$0205
__local_inc_sector_and_write__2	EQU	$0206
__local_do_rwts_on_sector__0	EQU	$0207
__local_do_rwts_on_sector__sector_1	EQU	$0208
__local_do_rwts_on_sector__sector_0	EQU	$0209
branch	EQU	$0BAA
negated_branch	EQU	$0BB3
buffer_char	EQU	$1B90
dump_buffer_line	EQU	$1BE2
dump_buffer_with_more	EQU	$1C5B
home	EQU	$1CC9
cout_string	EQU	$1D34
RWTS	EQU	$2900
HOME	EQU	$FC58
CLREOL	EQU	$FC9C
WAIT	EQU	$FCA8
RDKEY	EQU	$FD0C
COUT	EQU	$FDED
COUT1	EQU	$FDF0

; @zp-link-meta-begin
; def do_rwts_on_sector params=__zpabi_do_rwts_on_sector__command locals=__local_do_rwts_on_sector__0,__local_do_rwts_on_sector__sector_1,__local_do_rwts_on_sector__sector_0 indirect=false in_cycle=false param_regs=A
; def inc_sector_and_write params= locals=__local_inc_sector_and_write__0,__local_inc_sector_and_write__1,__local_inc_sector_and_write__2 indirect=false in_cycle=false
; def write_next_sector params= locals=__local_write_next_sector__0 indirect=false in_cycle=false
; ext CLREOL params=
; ext COUT params=__zpabi_COUT__c param_regs=A
; ext COUT1 params=__zpabi_COUT1__c param_regs=A
; ext HOME params=
; ext RDKEY params=
; ext RWTS params=__zpabi_RWTS__iob_hi,__zpabi_RWTS__iob_lo param_regs=A,Y
; ext WAIT params=__zpabi_WAIT__a param_regs=A
; ext branch params=
; ext buffer_char params=__zpabi_buffer_char__c param_regs=A
; ext cout_string params=__zpabi_cout_string__count param_regs=X
; ext dump_buffer_line params=
; ext dump_buffer_with_more params=
; ext home params=
; ext instr_save params=
; ext negated_branch params=
; ext please_insert_save_diskette params=
; ext please_reinsert_game_diskette params=
; ext print_ascii_string params=__zpabi_print_ascii_string__count param_regs=X
; call do_rwts_on_sector -> RWTS
; call inc_sector_and_write -> do_rwts_on_sector
; call write_next_sector -> inc_sector_and_write
; item do_rwts_on_sector section=.text local=false zero=false
; item inc_sector_and_write section=.text local=false zero=false
; item write_next_sector section=.text local=false zero=false
; item dct section=.data local=false zero=false
; item iob section=.data local=false zero=false
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
; zp SCRATCH2 size=2 at=$E6 def=false init=
; zp __zork_zp_e8 size=11 at=$E8 def=false init=
; runtime hargs_size=1 hargs_need=0
; @zp-link-meta-end

do_rwts_on_sector:
   SUBROUTINE

   STA   iob+12
   LDA   SCRATCH2
   STA   iob+8
   LDA   SCRATCH2+1
   STA   iob+9
   LDA   #$03
   STA   iob+4
   LDA   SCRATCH1
   STA   __local_do_rwts_on_sector__sector_0
   LDA   SCRATCH1+1
   STA   __local_do_rwts_on_sector__sector_1
.loop@0_continue:
   LDA   SECTORS_PER_TRACK
   STA   __local_do_rwts_on_sector__0
   LDA   __local_do_rwts_on_sector__sector_0
   SEC
   SBC   __local_do_rwts_on_sector__0
   LDA   __local_do_rwts_on_sector__sector_1
   SBC   #$00
   BCC   .loop@0_break
   LDA   SECTORS_PER_TRACK
   STA   __local_do_rwts_on_sector__0
   LDA   __local_do_rwts_on_sector__sector_0
   SEC
   SBC   __local_do_rwts_on_sector__0
   STA   __local_do_rwts_on_sector__sector_0
   BCS   .carry_inc@0
   DEC   __local_do_rwts_on_sector__sector_1
.carry_inc@0:
   INC   iob+4
   JMP   .loop@0_continue
.loop@0_break:
   LDA   __local_do_rwts_on_sector__sector_0
   STA   iob+5
   LDA   #>iob
   LDY   #<iob
   JMP   RWTS

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
   JSR   do_rwts_on_sector
   LDA   __local_inc_sector_and_write__2
   STA   dct+2
   LDA   __local_inc_sector_and_write__1
   STA   dct+3
   RTS

write_next_sector:
   SUBROUTINE

   LDA   #<BUFF_AREA
   STA   SCRATCH2
   LDA   #>BUFF_AREA
   STA   SCRATCH2+1
   JMP   inc_sector_and_write

dct:
   DS.B  1
   DC.B  $01
   DC.W  $D8EF

iob:
   DC.B  $01
   DC.B  $60
   DC.B  $01
   DS.B  3
   DC.W  dct
   DS.B  7
   DC.B  $60
   DC.B  $01
