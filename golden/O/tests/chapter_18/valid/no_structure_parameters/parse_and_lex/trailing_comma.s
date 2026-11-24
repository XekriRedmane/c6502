SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24

; @zp-link-meta-begin
; def main params= locals=__local_main__x_3,__local_main__x_2,__local_main__x_1,__local_main__x_0 indirect=false in_cycle=false
; item main section=.text local=false zero=false
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

main:
   SUBROUTINE

   LDA   #$00
   LDY   #$00
   CMP   #$00
   BNE   .main@jthread@0
   LDA   #$01
   CMP   #$01
   BNE   .main@jthread@0
   TYA
   BNE   .main@jthread@0
   LDA   #$02
   CMP   #$02
   BEQ   .if_end@2
.main@jthread@0:
   LDX   #$00
   LDA   #$01
   RTS
.if_end@2:
   LDX   #$00
   TXA
   RTS
