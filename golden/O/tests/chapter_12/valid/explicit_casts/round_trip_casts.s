SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24

; @zp-link-meta-begin
; def main params= locals=__local_main__0,__local_main__1,__local_main__2,__local_main__3,__local_main__4,__local_main__5,__local_main__6,__local_main__7 indirect=false in_cycle=false
; item main section=.text local=false zero=false
; item a section=.data local=false zero=false
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

main:
   SUBROUTINE

   LDA   a+3
   BNE   .jcmp_skip@0
   LDA   a+2
   BNE   .jcmp_skip@0
   LDA   a+1
   CMP   #$86
   BNE   .jcmp_skip@0
   LDA   a
   CMP   #$A0
   BEQ   .jcmp_skip@3
.jcmp_skip@0:
   LDX   #$00
   LDA   #$01
   RTS
.jcmp_skip@3:
   LDX   #$00
   LDA   #$02
   RTS

a:
   DC.L  $000186A0
   DC.L  $00000000
