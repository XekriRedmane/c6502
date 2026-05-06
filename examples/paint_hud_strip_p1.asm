paint_hud_strip_p1:
   SUBROUTINE

.paint_hud_strip_p1@asm_ssa_preheader@0:
.paint_hud_strip_p1@ssa_block@0:
   LDY   #$00
   LDX   #$0F
.loop@0_start:
   LDA   $A30D,Y
   INY
   STA   $240C,X
   STA   $280C,X
   STA   $2C0C,X
   STA   $300C,X
   STA   $340C,X
   STA   $380C,X
   STA   $3C0C,X
   LDA   $A30D,Y
   INY
   STA   $208C,X
   LDA   $A30D,Y
   INY
   STA   $248C,X
   STA   $288C,X
   STA   $2C8C,X
   STA   $308C,X
   STA   $348C,X
   STA   $388C,X
   STA   $3C8C,X
   LDA   $A30D,Y
   INY
   STA   $210C,X
   LDA   $A30D,Y
   INY
   STA   $250C,X
   STA   $290C,X
   STA   $2D0C,X
   STA   $310C,X
   STA   $350C,X
   STA   $390C,X
   STA   $3D0C,X
   LDA   $A30D,Y
   INY
   STA   $218C,X
   LDA   $A30D,Y
   STA   $80
   INY
   STA   $258C,X
   STA   $298C,X
   STA   $2D8C,X
   STA   $318C,X
   STA   $358C,X
   STA   $398C,X
   STA   $3D8C,X
.loop@0_continue:
   DEX
   BPL   .paint_hud_strip_p1@asm_ssa_split@0
.paint_hud_strip_p1@asm_ssa_block@0:
   RTS
.paint_hud_strip_p1@asm_ssa_split@0:
   JMP   .loop@0_start
