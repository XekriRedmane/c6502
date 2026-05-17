__local_paint_hud_strip_p1__y	EQU	$81

; @zp-link-meta-begin
; def paint_hud_strip_p1 params= locals=__local_paint_hud_strip_p1__0,__local_paint_hud_strip_p1__y indirect=false in_cycle=false
; @zp-link-meta-end

paint_hud_strip_p1:
   SUBROUTINE

.paint_hud_strip_p1@asm_ssa_preheader@0:
.paint_hud_strip_p1@ssa_block@0:
   LDA   #$00
   STA   __local_paint_hud_strip_p1__y
   LDY   #$0F
.loop@0_start:
   LDX   __local_paint_hud_strip_p1__y
   LDA   $A30D,X
   INC   __local_paint_hud_strip_p1__y
   STA   $240C,Y
   STA   $280C,Y
   STA   $2C0C,Y
   STA   $300C,Y
   STA   $340C,Y
   STA   $380C,Y
   STA   $3C0C,Y
   LDX   __local_paint_hud_strip_p1__y
   LDA   $A30D,X
   INC   __local_paint_hud_strip_p1__y
   STA   $208C,Y
   LDX   __local_paint_hud_strip_p1__y
   LDA   $A30D,X
   INC   __local_paint_hud_strip_p1__y
   STA   $248C,Y
   STA   $288C,Y
   STA   $2C8C,Y
   STA   $308C,Y
   STA   $348C,Y
   STA   $388C,Y
   STA   $3C8C,Y
   LDX   __local_paint_hud_strip_p1__y
   LDA   $A30D,X
   INC   __local_paint_hud_strip_p1__y
   STA   $210C,Y
   LDX   __local_paint_hud_strip_p1__y
   LDA   $A30D,X
   INC   __local_paint_hud_strip_p1__y
   STA   $250C,Y
   STA   $290C,Y
   STA   $2D0C,Y
   STA   $310C,Y
   STA   $350C,Y
   STA   $390C,Y
   STA   $3D0C,Y
   LDX   __local_paint_hud_strip_p1__y
   LDA   $A30D,X
   INC   __local_paint_hud_strip_p1__y
   STA   $218C,Y
   LDX   __local_paint_hud_strip_p1__y
   LDA   $A30D,X
   INC   __local_paint_hud_strip_p1__y
   STA   $258C,Y
   STA   $298C,Y
   STA   $2D8C,Y
   STA   $318C,Y
   STA   $358C,Y
   STA   $398C,Y
   STA   $3D8C,Y
.loop@0_continue:
   DEY
   BPL   .paint_hud_strip_p1@asm_ssa_split@0
.paint_hud_strip_p1@asm_ssa_block@0:
   RTS
.paint_hud_strip_p1@asm_ssa_split@0:
   JMP   .loop@0_start
