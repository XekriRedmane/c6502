SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24

; @zp-link-meta-begin
; def main params= locals=__local_main__my_struct_5,__local_main__my_struct_4,__local_main__my_struct_1,__local_main__my_struct_0 indirect=false in_cycle=false
; item main section=.text local=false zero=false
; item glob section=.data local=false zero=false
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

main:
   SUBROUTINE

   LDA   #$00
   BNE   .jcmp_skip@0
   LDA   #$64
   CMP   #$64
   BEQ   .if_end@0
.jcmp_skip@0:
   LDX   #$00
   LDA   #$01
   RTS
.if_end@0:
   LDA   glob+1
   BNE   .jcmp_skip@1
   LDA   glob
   CMP   #$01
   BEQ   .if_end@1
.jcmp_skip@1:
   LDX   #$00
   LDA   #$02
   RTS
.if_end@1:
   LDA   glob+5
   BNE   .jcmp_skip@2
   LDA   glob+4
   CMP   #$03
   BEQ   .if_end@2
.jcmp_skip@2:
   LDX   #$00
   LDA   #$03
   RTS
.if_end@2:
   LDX   #$00
   TXA
   RTS

glob:
   DC.W  $0001
   DC.W  $0002
   DC.W  $0003
