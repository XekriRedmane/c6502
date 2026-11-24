SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__local_main__0	EQU	$80

; @zp-link-meta-begin
; def main params= locals=__local_main__0,__local_main__1 indirect=false in_cycle=false
; def three params= locals= indirect=false in_cycle=false
; call main -> three
; item three section=.text local=false zero=false
; item main section=.text local=false zero=false
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

three:
   SUBROUTINE

   LDX   #$00
   LDA   #$03
   RTS

main:
   SUBROUTINE

   JSR   three
   STX   __local_main__0
   ORA   __local_main__0
   BEQ   .lnot_true@0
   LDA   #$00
   BEQ   .lnot_end@1
.lnot_true@0:
   LDA   #$01
.lnot_end@1:
   LDX   #$00
   RTS
