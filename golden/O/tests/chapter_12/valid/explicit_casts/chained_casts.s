SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24

; @zp-link-meta-begin
; def main params= locals=__local_main__0,__local_main__1,__local_main__2,__local_main__3,__local_main__4,__local_main__5,__local_main__6,__local_main__7 indirect=false in_cycle=false
; item main section=.text local=false zero=false
; item ui section=.data local=false zero=false
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

main:
   SUBROUTINE

   LDA   ui+1
   BPL   .jcmp_skip@2
   CMP   #$FF
   BNE   .jcmp_skip@2
   LDA   ui
   CMP   #$A0
   BEQ   .jcmp_skip@5
.jcmp_skip@2:
   LDX   #$00
   LDA   #$01
   RTS
.jcmp_skip@5:
   LDX   #$00
   LDA   #$02
   RTS

ui:
   DC.L  $0000FFA0
