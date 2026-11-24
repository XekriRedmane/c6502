SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24

; @zp-link-meta-begin
; def main params= locals=__local_main__special_5,__local_main__special_4,__local_main__special_3,__local_main__special_2,__local_main__special_1,__local_main__special_0 indirect=false in_cycle=false
; item main section=.text local=false zero=false
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

main:
   SUBROUTINE

   LDA   #$07
   CMP   #$07
   BEQ   .if_end@5
   LDX   #$00
   LDA   #$01
   RTS
.if_end@5:
   LDX   #$00
   TXA
   RTS
