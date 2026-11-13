HARGS	EQU	$00
SSP	EQU	$0A
FP	EQU	$0C
DPTR	EQU	$0E
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
__zpabi_get_prompted_number_from_user__which	EQU	$0200
__zpabi_print_ascii_string__count	EQU	$0201
__local_please_insert_save_diskette__n	EQU	$0203
__local_please_insert_save_diskette__0	EQU	$0204
__local_get_prompted_number_from_user__key	EQU	$0205
__local_get_prompted_number_from_user__0	EQU	$0206
__local_get_prompted_number_from_user__1	EQU	$0207
__local_get_prompted_number_from_user__2	EQU	$0208
__local_print_ascii_string__count	EQU	$020A
__local_print_ascii_string__i	EQU	$020B
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
; def get_prompted_number_from_user params=__zpabi_get_prompted_number_from_user__which locals=__local_get_prompted_number_from_user__key,__local_get_prompted_number_from_user__0,__local_get_prompted_number_from_user__1,__local_get_prompted_number_from_user__2 indirect=false in_cycle=false param_regs=A
; def please_insert_save_diskette params= locals=__local_please_insert_save_diskette__n,__local_please_insert_save_diskette__0 indirect=false in_cycle=false
; def please_reinsert_game_diskette params= locals=__local_please_reinsert_game_diskette__0 indirect=false in_cycle=false
; def print_ascii_string params=__zpabi_print_ascii_string__count locals=__local_print_ascii_string__0,__local_print_ascii_string__count,__local_print_ascii_string__i indirect=false in_cycle=false param_regs=X
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
; ext inc_sector_and_write params=
; ext instr_save params=
; ext negated_branch params=
; ext write_next_sector params=
; call get_prompted_number_from_user -> CLREOL
; call get_prompted_number_from_user -> RDKEY
; call get_prompted_number_from_user -> cout_string
; call get_prompted_number_from_user -> dump_buffer_line
; call get_prompted_number_from_user -> dump_buffer_with_more
; call get_prompted_number_from_user -> print_ascii_string
; call please_insert_save_diskette -> RDKEY
; call please_insert_save_diskette -> buffer_char
; call please_insert_save_diskette -> dump_buffer_line
; call please_insert_save_diskette -> dump_buffer_with_more
; call please_insert_save_diskette -> get_prompted_number_from_user
; call please_insert_save_diskette -> home
; call please_insert_save_diskette -> print_ascii_string
; call please_reinsert_game_diskette -> RDKEY
; call please_reinsert_game_diskette -> dump_buffer_line
; call please_reinsert_game_diskette -> dump_buffer_with_more
; call please_reinsert_game_diskette -> print_ascii_string
; call print_ascii_string -> buffer_char
; item print_ascii_string section=.text local=false zero=false
; item get_prompted_number_from_user section=.text local=false zero=false
; item please_insert_save_diskette section=.text local=false zero=false
; item please_reinsert_game_diskette section=.text local=false zero=false
; item sPleaseInsert section=.rodata local=true zero=false
; item sDefault section=.rodata local=true zero=false
; item sReturnToBegin section=.rodata local=true zero=false
; item sReinsertGameDiskette section=.rodata local=true zero=false
; item sPressReturnToContinue section=.rodata local=true zero=false
; item prompts section=.data local=false zero=false
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
; runtime hargs_size=10 hargs_need=0
; @zp-link-meta-end

print_ascii_string:
   SUBROUTINE

   LDA   #$00
   STA   __local_print_ascii_string__i
   STX   __local_print_ascii_string__count
.loop@0_start:
   LDY   __local_print_ascii_string__i
   LDA   (SCRATCH2),Y
   JSR   buffer_char
   INC   __local_print_ascii_string__i
   DEC   __local_print_ascii_string__count
   BNE   .loop@0_start
   RTS

get_prompted_number_from_user:
   SUBROUTINE

   STA   __zpabi_get_prompted_number_from_user__which
   JSR   dump_buffer_with_more
   LDA   __zpabi_get_prompted_number_from_user__which
   ASL
   STA   __local_get_prompted_number_from_user__1
   LDA   #$00
   STA   __local_get_prompted_number_from_user__2
   ROL   __local_get_prompted_number_from_user__2
   ASL   __local_get_prompted_number_from_user__1
   ROL   __local_get_prompted_number_from_user__2
   ASL   __local_get_prompted_number_from_user__1
   ROL   __local_get_prompted_number_from_user__2
   ASL   __local_get_prompted_number_from_user__1
   ROL   __local_get_prompted_number_from_user__2
   LDA   #<prompts
   CLC
   ADC   __local_get_prompted_number_from_user__1
   STA   SCRATCH2
   LDA   #>prompts
   ADC   __local_get_prompted_number_from_user__2
   STA   SCRATCH2+1
   LDX   #$0F
   JSR   print_ascii_string
   JSR   dump_buffer_line
   LDA   #$19
   STA   CH
   LDA   #$3F
   STA   INVFLG
   LDA   #<sDefault
   STA   SCRATCH2
   LDA   #>sDefault
   STA   SCRATCH2+1
   LDX   #$0A
   JSR   cout_string
   LDA   #<prompts
   CLC
   ADC   __local_get_prompted_number_from_user__1
   STA   __local_get_prompted_number_from_user__0
   LDA   #>prompts
   ADC   __local_get_prompted_number_from_user__2
   STA   __local_get_prompted_number_from_user__key
   LDA   __local_get_prompted_number_from_user__0
   CLC
   ADC   #$0F
   STA   SCRATCH2
   LDA   __local_get_prompted_number_from_user__key
   ADC   #$00
   STA   SCRATCH2+1
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
   LDA   #<prompts
   CLC
   ADC   __local_get_prompted_number_from_user__1
   STA   __local_get_prompted_number_from_user__key
   LDA   #>prompts
   ADC   __local_get_prompted_number_from_user__2
   STA   __local_get_prompted_number_from_user__0
   LDA   __local_get_prompted_number_from_user__key
   STA   DPTR
   LDA   __local_get_prompted_number_from_user__0
   STA   DPTR+1
   LDY   #$0F
   LDA   (DPTR),Y
.if_end@0:
   AND   #$7F
   RTS

please_insert_save_diskette:
   SUBROUTINE

   JSR   home
   JSR   dump_buffer_with_more
   JSR   dump_buffer_with_more
   LDA   #<sPleaseInsert
   STA   SCRATCH2
   LDA   #>sPleaseInsert
   STA   SCRATCH2+1
   LDX   #$1C
   JSR   print_ascii_string
   JSR   dump_buffer_with_more
.loop@1_start:
   LDA   #$02
   JSR   get_prompted_number_from_user
   CMP   #$30
   BCC   .loop@1_start
   CMP   #$38
   BCS   .loop@1_start
   STA   prompts+47
   JSR   buffer_char
.loop@2_start:
   LDA   #$00
   JSR   get_prompted_number_from_user
   STA   __local_please_insert_save_diskette__0
   CMP   #$31
   BCC   .loop@2_start
   CMP   #$38
   BCS   .loop@2_start
   AND   #$07
   ASL
   ASL
   ASL
   ASL
   STA   iob+1
   LDA   __local_please_insert_save_diskette__0
   STA   prompts+15
   JSR   buffer_char
.loop@3_start:
   LDA   #$01
   JSR   get_prompted_number_from_user
   STA   __local_please_insert_save_diskette__n
   CMP   #$31
   BCC   .loop@3_start
   CMP   #$33
   BCS   .loop@3_start
   AND   #$03
   STA   iob+2
   LDA   __local_please_insert_save_diskette__n
   STA   prompts+31
   JSR   buffer_char
.loop@4_start:
   JSR   dump_buffer_with_more
   LDA   #<sReturnToBegin
   STA   SCRATCH2
   LDA   #>sReturnToBegin
   STA   SCRATCH2+1
   LDX   #$23
   JSR   print_ascii_string
   JSR   dump_buffer_line
   JSR   RDKEY
   CMP   #$8D
   BNE   .loop@4_start
   LDA   #$FF
   STA   SCRATCH1
   LDA   #$FF
   STA   SCRATCH1+1
   LDA   prompts+47
   AND   #$07
   STA   __local_please_insert_save_diskette__n
.loop@5_start:
   LDA   __local_please_insert_save_diskette__n
   BEQ   .loop@5_break
   LDA   SCRATCH1
   CLC
   ADC   #$40
   STA   SCRATCH1
   BCC   .carry_inc@0
   INC   SCRATCH1+1
.carry_inc@0:
   DEC   __local_please_insert_save_diskette__n
   JMP   .loop@5_start
.loop@5_break:
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
   LDA   #<sReinsertGameDiskette
   STA   SCRATCH2
   LDA   #>sReinsertGameDiskette
   STA   SCRATCH2+1
   LDX   #$1F
   JSR   print_ascii_string
.loop@6_start:
   JSR   dump_buffer_with_more
   LDA   #<sPressReturnToContinue
   STA   SCRATCH2
   LDA   #>sPressReturnToContinue
   STA   SCRATCH2+1
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
