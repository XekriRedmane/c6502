SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
foo	EQU	$26
__local_main__i	EQU	$80

; @zp-link-meta-begin
; def main params= locals=__local_main__i indirect=false in_cycle=false
; item main section=.text local=false zero=false
; item __zp_init section=.text local=false zero=false
; zp foo size=2 at=auto def=true init=$00,$00 weight=17
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

main:
   SUBROUTINE

   LDA   #$00
   STA   __local_main__i
   LDX   #$00
.loop@0_start:
   LDA   __local_main__i
   SEC
   SBC   #$05
   TXA
   SBC   #$00
   BVC   .jcmp_novf@0
   EOR   #$80
.jcmp_novf@0:
   BPL   .loop@0_break
   INC   foo
   BNE   .inc_done@0
   INC   foo+1
.inc_done@0:
   INC   __local_main__i
   BNE   .loop@0_start
   INX
   JMP   .loop@0_start
.loop@0_break:
   LDX   foo+1
   LDA   foo
   RTS

__zp_init:
   SUBROUTINE

   LDA   #$00
   STA   foo
   STA   foo+1
   RTS
