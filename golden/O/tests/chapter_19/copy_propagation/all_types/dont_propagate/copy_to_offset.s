SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24

; @zp-link-meta-begin
; def main params= locals=__local_main__s2_3,__local_main__s2_2,__local_main__s2_1,__local_main__s2_0 indirect=false in_cycle=false
; item main section=.text local=false zero=false
; item @0.s1 section=.data local=true zero=false
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

main:
   SUBROUTINE

   LDA   #$00
   LDY   #$03
   STY   @0.s1
   STA   @0.s1+1
   INY
   STY   @0.s1+2
   STA   @0.s1+3
   LDA   @0.s1
   LDX   @0.s1+1
   RTS

@0.s1:
   DC.W  $0001
   DC.W  $0002
