SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__local_f__i_1	EQU	$80
__local_f__i_0	EQU	$81

; @zp-link-meta-begin
; def callee params= locals= indirect=false in_cycle=false
; def f params= locals=__local_f__i_1,__local_f__i_0 indirect=false in_cycle=false
; def main params= locals= indirect=false in_cycle=false
; call f -> callee
; call main -> f
; item callee section=.text local=false zero=false
; item f section=.text local=false zero=false
; item main section=.text local=false zero=false
; item x section=.bss local=false zero=true
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

callee:
   SUBROUTINE

   INC   x
   BNE   .inc_done@0
   INC   x+1
.inc_done@0:
   LDX   #$00
   TXA
   RTS

f:
   SUBROUTINE

   LDA   #$00
   STA   __local_f__i_0
   LDY   #$00
   STY   __local_f__i_1
.loop@0_start:
   LDA   __local_f__i_0
   SEC
   SBC   #$0A
   LDA   __local_f__i_1
   SBC   #$00
   BVC   .jcmp_novf@0
   EOR   #$80
.jcmp_novf@0:
   BPL   .loop@0_break
   JSR   callee
   INC   __local_f__i_0
   BNE   .loop@0_start
   INC   __local_f__i_1
   JMP   .loop@0_start
.loop@0_break:
   LDX   #$00
   TXA
   RTS

main:
   SUBROUTINE

   JSR   f
   LDX   x+1
   LDA   x
   RTS

x:
   DS.B  2
