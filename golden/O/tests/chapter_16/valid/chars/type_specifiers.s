SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__local_main__loop_counter	EQU	$81

; @zp-link-meta-begin
; def main params= locals=__local_main__d,__local_main__loop_counter indirect=false in_cycle=false
; item main section=.text local=false zero=false
; item a section=.data local=true zero=false
; item b section=.data local=true zero=false
; item c section=.data local=false zero=false
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

main:
   SUBROUTINE

   LDA   a
   CMP   #$0A
   BEQ   .if_end@0
   LDX   #$00
   LDA   #$01
   RTS
.if_end@0:
   LDA   b
   CMP   #$14
   BEQ   .if_end@1
   LDX   #$00
   LDA   #$02
   RTS
.if_end@1:
   LDA   c
   CMP   #$1E
   BEQ   .if_end@2
   LDX   #$00
   LDA   #$03
   RTS
.if_end@2:
   LDA   #$00
   STA   __local_main__loop_counter
   LDX   #$00
   LDY   #$64
.main@rot@0:
   INC   __local_main__loop_counter
   BNE   .inc_done@0
   INX
.inc_done@0:
   DEY
   BNE   .main@rot@0
   CPX   #$00
   BNE   .jcmp_skip@0
   LDA   __local_main__loop_counter
   CMP   #$64
   BEQ   .if_end@3
.jcmp_skip@0:
   LDX   #$00
   LDA   #$04
   RTS
.if_end@3:
   LDX   #$00
   TXA
   RTS

a:
   DC.B  $0A

b:
   DC.B  $14

c:
   DC.B  $1E
