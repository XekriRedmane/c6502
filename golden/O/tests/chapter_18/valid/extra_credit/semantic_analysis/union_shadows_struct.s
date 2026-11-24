SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24

; @zp-link-meta-begin
; def main params= locals=__local_main__0,__local_main__1,__local_main__local_union_1,__local_main__local_union_0 indirect=false in_cycle=false
; item main section=.text local=false zero=false
; item global_struct section=.data local=false zero=false
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

main:
   SUBROUTINE

   LDA   global_struct+1
   BNE   .jcmp_skip@0
   LDA   global_struct
   CMP   #$01
   BEQ   .if_end@1
.jcmp_skip@0:
   LDX   #$00
   LDA   #$01
   RTS
.if_end@1:
   LDX   #$00
   TXA
   RTS

global_struct:
   DC.W  $0001
   DC.W  $0002
