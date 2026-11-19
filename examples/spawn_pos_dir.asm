SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24

; @zp-link-meta-begin
; def spawn_pos_dir params=__zpabi_spawn_pos_dir__slot locals=__local_spawn_pos_dir__0 indirect=false in_cycle=false param_regs=A
; item spawn_pos_dir section=.text local=true zero=false
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

spawn_pos_dir:
   SUBROUTINE

   TAY
   LDA   #$01
   STA   entity_active,Y
   STA   rescue_dir,Y
   LDA   #$3E
   STA   entity_floor_col,Y
   LDA   #$00
   STA   entity_xoff_idx,Y
   STA   rescue_anim,Y
   LDX   rescue_floor,Y
   LDA   floor_thresh,X
   SEC
   SBC   #$07
   STA   entity_floor_pos,Y
   RTS
