SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__local_main__1	EQU	$81

; @zp-link-meta-begin
; def main params= locals=__local_main__0,__local_main__1 indirect=false in_cycle=false
; item main section=.text local=false zero=false
; item ui section=.data local=false zero=false
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

main:
   SUBROUTINE

   LDA   #$00
   STA   ui+1
   CMP   #$00
   BNE   .jcmp_skip@0
   LDA   ui
   CMP   #$A0
   BEQ   .if_end@0
.jcmp_skip@0:
   LDX   #$00
   LDA   #$01
   RTS
.if_end@0:
   STA   __local_main__1
   ORA   #$00
   BPL   .jcmp_skip@3
   LDA   __local_main__1
   CMP   #$A0
   BEQ   .if_end@1
.jcmp_skip@3:
   LDX   #$00
   LDA   #$02
   RTS
.if_end@1:
   LDX   #$00
   TXA
   RTS

ui:
   DC.W  $FFA0
