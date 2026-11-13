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
COLOR	EQU	$30
INVFLG	EQU	$32
PROMPT	EQU	$33
__monitor_ysav1	EQU	$35
CSW	EQU	$36
KSW	EQU	$38
__monitor_acc	EQU	$45
__monitor_status	EQU	$48
__monitor_rnd	EQU	$4E
__local_dump_buffer_to_printer__x	EQU	$82
__local_dump_buffer_to_printer__1	EQU	$83
__local_dump_buffer_to_printer__2	EQU	$84
BUFF_END	EQU	$EB
PRINTER_CSW	EQU	$EE
BUFF_AREA	EQU	$0200
SLOT1_SCRATCH	EQU	$0779
HOME	EQU	$FC58
CLREOL	EQU	$FC9C
WAIT	EQU	$FCA8
RDKEY	EQU	$FD0C
COUT	EQU	$FDED
COUT1	EQU	$FDF0

; @zp-link-meta-begin
; def dump_buffer_to_printer params= locals=__local_dump_buffer_to_printer__0,__local_dump_buffer_to_printer__x,__local_dump_buffer_to_printer__1,__local_dump_buffer_to_printer__2 indirect=false in_cycle=false
; def dump_buffer_to_screen params= locals=__local_dump_buffer_to_screen__0 indirect=false in_cycle=false
; ext CLREOL params=
; ext COUT params=__zpabi_COUT__c param_regs=A
; ext COUT1 params=__zpabi_COUT1__c param_regs=A
; ext HOME params=
; ext RDKEY params=
; ext WAIT params=__zpabi_WAIT__a param_regs=A
; call dump_buffer_to_printer -> COUT
; call dump_buffer_to_screen -> COUT1
; item dump_buffer_to_screen section=.text local=false zero=false
; item dump_buffer_to_printer section=.text local=false zero=false
; item printer_card_initialized_flag section=.bss local=true zero=true
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
; zp COLOR size=1 at=$30 def=false init=
; zp INVFLG size=1 at=$32 def=false init=
; zp PROMPT size=1 at=$33 def=false init=
; zp __monitor_ysav1 size=1 at=$35 def=false init=
; zp CSW size=2 at=$36 def=false init=
; zp KSW size=2 at=$38 def=false init=
; zp __monitor_acc size=1 at=$45 def=false init=
; zp __monitor_status size=1 at=$48 def=false init=
; zp __monitor_rnd size=2 at=$4E def=false init=
; zp BUFF_END size=1 at=$EB def=false init=
; zp PRINTER_CSW size=2 at=$EE def=false init=
; runtime hargs_size=1 hargs_need=0
; @zp-link-meta-end

dump_buffer_to_screen:
   SUBROUTINE

   LDX   #$00
.loop@0_start:
   CPX   BUFF_END
   BEQ   .loop@0_break
   LDA   BUFF_AREA,X
   JSR   COUT1
   INX
   JMP   .loop@0_start
.loop@0_break:
   LDA   #$00
   STA   BUFF_END
   RTS

dump_buffer_to_printer:
   SUBROUTINE

   LDA   CSW
   STA   __local_dump_buffer_to_printer__2
   LDA   CSW+1
   STA   __local_dump_buffer_to_printer__1
   LDA   PRINTER_CSW
   STA   CSW
   LDA   PRINTER_CSW+1
   STA   CSW+1
   LDA   printer_card_initialized_flag
   BNE   .if_end@0
   LDA   #$01
   STA   printer_card_initialized_flag
   LDA   #$09
   JSR   COUT
   LDA   #$91
   STA   SLOT1_SCRATCH
   LDA   #$B8
   JSR   COUT
   LDA   #$B0
   JSR   COUT
   LDA   #$CE
   JSR   COUT
.if_end@0:
   LDX   #$00
.loop@1_start:
   CPX   BUFF_END
   BEQ   .loop@1_break
   LDA   BUFF_AREA,X
   STX   __local_dump_buffer_to_printer__x
   JSR   COUT
   LDX   __local_dump_buffer_to_printer__x
   INX
   JMP   .loop@1_start
.loop@1_break:
   LDA   CSW
   STA   PRINTER_CSW
   LDA   CSW+1
   STA   PRINTER_CSW+1
   LDA   __local_dump_buffer_to_printer__2
   STA   CSW
   LDA   __local_dump_buffer_to_printer__1
   STA   CSW+1
   RTS

printer_card_initialized_flag:
   DS.B  1
