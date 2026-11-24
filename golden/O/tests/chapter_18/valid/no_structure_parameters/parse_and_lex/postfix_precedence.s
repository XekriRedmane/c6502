SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__local_main__array_20	EQU	$81

; @zp-link-meta-begin
; def main params= locals=__local_main__array_21,__local_main__array_20 indirect=false in_cycle=false
; item main section=.text local=false zero=false
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

main:
   SUBROUTINE

   LDA   #$00
   SEC
   SBC   #$0B
   STA   __local_main__array_20
   LDA   #$00
   SBC   #$00
   CMP   #$FF
   BNE   .thread@0
   LDA   __local_main__array_20
   CMP   #$F5
   BEQ   .cmp_true@0
.thread@0:
   LDA   #$00
   BEQ   .cmp_end@1
.cmp_true@0:
   LDA   #$01
.cmp_end@1:
   LDX   #$00
   RTS
