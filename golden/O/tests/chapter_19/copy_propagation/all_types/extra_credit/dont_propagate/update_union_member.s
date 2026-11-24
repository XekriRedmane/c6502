SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24

; @zp-link-meta-begin
; def main params= locals=__local_main__u2_3,__local_main__u2_2,__local_main__u2_1,__local_main__u2_0 indirect=false in_cycle=false
; item main section=.text local=false zero=false
; item @0.u1 section=.data local=true zero=false
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

main:
   SUBROUTINE

   LDY   #$03
   STY   @0.u1
   LDY   #$00
   STY   @0.u1+1
   STY   @0.u1+2
   STY   @0.u1+3
   LDA   @0.u1
   LDX   @0.u1+1
   RTS

@0.u1:
   DC.L  $00000014
