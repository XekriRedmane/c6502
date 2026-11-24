SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24

; @zp-link-meta-begin
; def f params= locals= indirect=false in_cycle=false
; def main params= locals= indirect=false in_cycle=false
; call main -> f
; item f section=.text local=false zero=false
; item main section=.text local=false zero=false
; item global section=.bss local=false zero=true
; item flag section=.data local=false zero=false
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

f:
   SUBROUTINE

   LDA   #$64
   STA   global
   LDA   #$00
   STA   global+1
   LDX   #$00
   RTS

main:
   SUBROUTINE

   LDA   #$00
   STA   global
   STA   global+1
   LDA   flag
   ORA   flag+1
   BEQ   .if_end@0
   JSR   f
.if_end@0:
   LDX   global+1
   LDA   global
   RTS

global:
   DS.B  2

flag:
   DC.W  $0001
