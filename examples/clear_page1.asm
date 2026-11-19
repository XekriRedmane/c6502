SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__local_clear_page1__x	EQU	$80
__local_clear_page1__zp_clear_col_end	EQU	$81

; @zp-link-meta-begin
; def clear_page1 params=__zpabi_clear_page1__zp_clear_col,__zpabi_clear_page1__zp_clear_col_end locals=__local_clear_page1__x,__local_clear_page1__zp_clear_col_end,__local_clear_page1__0 indirect=false in_cycle=false param_regs=A,X
; ext interlace_fill_p1 params=__zpabi_interlace_fill_p1__col,__zpabi_interlace_fill_p1__paint param_regs=A,X
; call clear_page1 -> interlace_fill_p1
; item clear_page1 section=.text local=false zero=false
; item TEXT_STRIP_SRC section=.rodata local=false zero=false
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

clear_page1:
   SUBROUTINE

   STA   __local_clear_page1__x
   STX   __local_clear_page1__zp_clear_col_end
.loop@0_start:
   LDX   __local_clear_page1__x
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
   TXA
   LDX   #$00
   JSR   interlace_fill_p1
   LDA   __local_clear_page1__x
   CMP   __local_clear_page1__zp_clear_col_end
   BEQ   .loop@0_break
   DEC   __local_clear_page1__x
   JMP   .loop@0_start
.loop@0_break:
   LDX   #$27
   BNE   .loop@1_start
.clear_page1@rot@0:
   DEX
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
   BNE   .clear_page1@rot@0
   RTS

TEXT_STRIP_SRC:
   DC.W  $0300
