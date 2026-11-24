SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__local_main__0	EQU	$80

; @zp-link-meta-begin
; def main params= locals=__local_main__0,__local_main__1 indirect=false in_cycle=false
; item main section=.text local=false zero=false
; item x section=.data local=false zero=false
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

main:
   SUBROUTINE

   LDA   x
   ASL
   STA   __local_main__0
   LDA   x+1
   ROL
   BNE   .if_end@0
   LDA   __local_main__0
   CMP   #$1E
   BNE   .if_end@0
   LDX   #$00
   TXA
   RTS
.if_end@0:
   LDX   #$00
   LDA   #$05
   RTS

x:
   DC.W  $000F
