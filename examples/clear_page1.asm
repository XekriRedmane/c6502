__zpabi_clear_page1_p0	EQU	$80
__zpabi_clear_page1_p1	EQU	$81
__zpabi_interlace_fill_p1_p0	EQU	$82
__zpabi_interlace_fill_p1_p1	EQU	$83

clear_page1:
   SUBROUTINE

.clear_page1@asm_ssa_preheader@0:
.clear_page1@ssa_block@0:
   LDA   __zpabi_clear_page1_p0
   STA   $84
.loop@0_start:
   LDX   $84
   LDA   #$00
   STA   $2600,X
   STA   $2A00,X
   STA   $2E00,X
   STA   $3200,X
   STA   $3600,X
   STA   $3A00,X
   STA   $3E00,X
   STA   $2280,X
   STA   $2680,X
   STA   $2A80,X
   STA   $2E80,X
   STA   $3280,X
   STA   $3680,X
   STA   $3A80,X
   STA   $3E80,X
   STA   $2300,X
   STA   $2700,X
   STA   $2B00,X
   STA   $2F00,X
   STA   $3300,X
   STA   $3700,X
   STA   $3B00,X
   STA   $3F00,X
   STA   $2380,X
   STA   $2780,X
   STA   $2B80,X
   STA   $2F80,X
   STA   $3380,X
   STA   $3780,X
   STA   $3B80,X
   STA   $3F80,X
   STA   $2028,X
   STA   $2428,X
   STA   $2828,X
   LDA   $84
   STA   __zpabi_interlace_fill_p1_p0
   LDA   #$00
   STA   __zpabi_interlace_fill_p1_p1
   JSR   interlace_fill_p1
   LDA   $84
   CMP   __zpabi_clear_page1_p1
   BNE   .if_end@0
.clear_page1@asm_ssa_block@0:
   JMP   .loop@0_break
.if_end@0:
   DEC   $84
.loop@0_continue:
   JMP   .loop@0_start
.loop@0_break:
   LDX   #$27
.loop@1_start:
   LDA   $0300,X
   STA   $3028,X
   STA   $3428,X
   STA   $3828,X
   STA   $3C28,X
   STA   $32A8,X
   STA   $36A8,X
   STA   $3AA8,X
   STA   $3EA8,X
   STA   $3150,X
   STA   $3550,X
   STA   $3950,X
   STA   $3D50,X
   TXA
   BNE   .if_end@1
.clear_page1@asm_ssa_block@1:
   JMP   .loop@1_break
.if_end@1:
   DEX
.loop@1_continue:
   JMP   .loop@1_start
.loop@1_break:
   RTS

TEXT_STRIP_SRC:
   DC.W  $0300
