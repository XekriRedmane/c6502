SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__local_main__0	EQU	$80
__local_main__1	EQU	$81

; @zp-link-meta-begin
; def main params= locals=__local_main__0,__local_main__1 indirect=false in_cycle=false
; item main section=.text local=false zero=false
; item x section=.data local=false zero=false
; item y section=.bss local=false zero=true
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

main:
   SUBROUTINE

   LDA   #$FF
   STA   y
   STA   y+1
   LDA   x
   ASL
   STA   __local_main__0
   LDA   x+1
   ROL
   STA   __local_main__1
   LDA   __local_main__0
   ORA   #$01
   STA   x
   LDA   __local_main__1
   STA   x+1
   BNE   .jcmp_skip@0
   LDA   x
   CMP   #$03
   BEQ   .if_end@0
.jcmp_skip@0:
   LDX   #$00
   LDA   #$01
   RTS
.if_end@0:
   LDA   y
   AND   #$FB
   LDX   y+1
   STX   __local_main__0
   EOR   #$0C
   STA   __local_main__1
   LDA   __local_main__0
   ASL
   ROR   y+1
   LDA   __local_main__1
   ROR
   STA   y
   LDA   y+1
   ASL
   ROR   y+1
   ROR   y
   LDA   y+1
   CMP   #$FF
   BNE   .jcmp_skip@1
   LDA   y
   CMP   #$FD
   BEQ   .if_end@1
.jcmp_skip@1:
   LDX   #$00
   LDA   #$02
   RTS
.if_end@1:
   LDX   #$00
   TXA
   RTS

x:
   DC.W  $0001

y:
   DS.B  2
