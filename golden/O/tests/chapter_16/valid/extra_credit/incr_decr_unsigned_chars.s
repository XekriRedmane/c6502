SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__local_main__chars_4	EQU	$80
__local_main__0	EQU	$81
__local_main__chars_3	EQU	$82

; @zp-link-meta-begin
; def main params= locals=__local_main__chars_4,__local_main__0,__local_main__chars_3,__local_main__chars_0,__local_main__1,__local_main__chars_2 indirect=false in_cycle=false
; item main section=.text local=false zero=false
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

main:
   SUBROUTINE

   LDA   #$FD
   STA   __local_main__chars_4
   LDA   #$FF
   STA   __local_main__0
   LDA   #$00
   BEQ   .if_end@1
   LDX   #$00
   LDA   #$01
   RTS
.if_end@1:
   DEC   __local_main__chars_4
   LDA   __local_main__chars_4
   STA   __local_main__chars_3
   CMP   #$FC
   BEQ   .if_end@2
   LDX   #$00
   LDA   #$03
   RTS
.if_end@2:
   INC   __local_main__0
   LDA   __local_main__0
   STA   __local_main__chars_4
   BEQ   .if_end@6
   LDX   #$00
   LDA   #$04
   RTS
.if_end@6:
   LDA   __local_main__chars_3
   CMP   #$FC
   BEQ   .if_end@7
   LDX   #$00
   LDA   #$08
   RTS
.if_end@7:
   LDA   __local_main__chars_4
   BEQ   .if_end@8
   LDX   #$00
   LDA   #$09
   RTS
.if_end@8:
   LDX   #$00
   TXA
   RTS
