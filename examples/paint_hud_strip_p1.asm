paint_hud_strip_p1:
   SUBROUTINE

.paint_hud_strip_p1@asm_ssa_preheader@0:
.paint_hud_strip_p1@ssa_block@0:
   LDY   #$00
   LDA   #$10
   STA   $80
.loop@0_start:
   LDA   $80
   SEC
   SBC   #$01
   TAX
   BCS   .lb_skip@0
   JMP   .loop@0_break
.lb_skip@0:
.paint_hud_strip_p1@asm_ssa_block@0:
   LDA   $A30D,Y
   STA   $80
   INY
   STA   $240C,X
   STA   $280C,X
   STA   $2C0C,X
   STA   $300C,X
   STA   $340C,X
   STA   $380C,X
   STA   $3C0C,X
   LDA   $A30D,Y
   STA   $80
   INY
   STA   $208C,X
   LDA   $A30D,Y
   STA   $80
   INY
   STA   $248C,X
   STA   $288C,X
   STA   $2C8C,X
   STA   $308C,X
   STA   $348C,X
   STA   $388C,X
   STA   $3C8C,X
   LDA   $A30D,Y
   STA   $80
   INY
   STA   $210C,X
   LDA   $A30D,Y
   STA   $80
   INY
   STA   $250C,X
   STA   $290C,X
   STA   $2D0C,X
   STA   $310C,X
   STA   $350C,X
   STA   $390C,X
   STA   $3D0C,X
   LDA   $A30D,Y
   STA   $80
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
   STX   $80
   JMP   .loop@0_start
.loop@0_break:
   RTS
