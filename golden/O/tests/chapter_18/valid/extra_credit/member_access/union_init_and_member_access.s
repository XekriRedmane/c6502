SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24

; @zp-link-meta-begin
; def main params= locals=__local_main__x_3,__local_main__x_2,__local_main__x_1,__local_main__x_0,__local_main__0,__local_main__1,__local_main__2,__local_main__3 indirect=false in_cycle=false
; item main section=.text local=false zero=false
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

main:
   SUBROUTINE

   LDY   #$00
   LDA   #$40
   CMP   #$40
   BNE   .jcmp_skip@0
   LDA   #$34
   CMP   #$34
   BNE   .jcmp_skip@0
   TYA
   BNE   .jcmp_skip@0
   BEQ   .jcmp_skip@2
.jcmp_skip@0:
   LDX   #$00
   LDA   #$01
   RTS
.jcmp_skip@2:
   LDX   #$00
   LDA   #$03
   RTS
