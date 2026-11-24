SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24

; @zp-link-meta-begin
; def main params= locals=__local_main__retval indirect=false in_cycle=false
; item main section=.text local=false zero=false
; item flse section=.bss local=true zero=true
; runtime hargs_size=32 hargs_need=10
; @zp-link-meta-end

main:
   SUBROUTINE

   LDA   flse
   ORA   flse+1
   BEQ   .main@asm_ssa_split@0
   LDA   #$00
   STA   HARGS
   LDY   #$00
   STY   HARGS+1
   STY   HARGS+2
   STY   HARGS+3
   STY   HARGS+4
   STA   HARGS+5
   LDA   #$F8
   STA   HARGS+6
   LDA   #$FF
   STA   HARGS+7
   JSR   d2i
   LDA   HARGS+8
   LDX   HARGS+9
   RTS
.main@asm_ssa_split@0:
   LDA   #$00
   LDX   #$00
   RTS

flse:
   DS.B  2
