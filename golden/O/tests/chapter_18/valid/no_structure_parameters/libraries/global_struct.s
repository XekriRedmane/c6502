SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24

; @zp-link-meta-begin
; def update_outer_struct params= locals=__local_update_outer_struct__0,__local_update_outer_struct__1,__local_update_outer_struct__inner_11,__local_update_outer_struct__inner_10,__local_update_outer_struct__inner_9,__local_update_outer_struct__inner_8,__local_update_outer_struct__inner_7,__local_update_outer_struct__inner_6,__local_update_outer_struct__inner_5,__local_update_outer_struct__inner_4,__local_update_outer_struct__inner_3,__local_update_outer_struct__inner_2,__local_update_outer_struct__inner_1,__local_update_outer_struct__inner_0 indirect=false in_cycle=false
; def update_struct params= locals=__local_update_struct__0 indirect=false in_cycle=false
; item update_struct section=.text local=false zero=false
; item update_outer_struct section=.text local=false zero=false
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

update_struct:
   SUBROUTINE

   LDA   global+2
   ASL
   STA   global+3
   LDA   #$00
   STA   global+4
   LDY   #$00
   STY   global+5
   STY   global+6
   STY   global+7
   STY   global+8
   STA   global+9
   LDA   #$14
   STA   global+10
   LDA   #$40
   STA   global+11
   RTS

update_outer_struct:
   SUBROUTINE

   LDY   #$00
   TYA
   STA   global_outer+1
   STA   global_outer+2
   STA   global_outer+5
   LDA   #$FF
   STA   global_outer+3
   STA   global_outer+4
   TYA
   STA   global_outer+6
   STA   global_outer+7
   STA   global_outer+8
   STA   global_outer+9
   STA   global_outer+10
   STA   global_outer+11
   STA   global_outer+12
   RTS
