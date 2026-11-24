SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__local_main__x	EQU	$84
__local_main__y	EQU	$88

; @zp-link-meta-begin
; def main params= locals=__local_main__0,__local_main__1,__local_main__2,__local_main__3,__local_main__x,__local_main__5,__local_main__6,__local_main__7,__local_main__y,__local_main__9,__local_main__10,__local_main__11 indirect=false in_cycle=false
; item main section=.text local=false zero=false
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

main:
   SUBROUTINE

   LDY   #$00
   LDA   #>__local_main__x
   CMP   #>__local_main__y
   BNE   .thread@0
   LDA   #<__local_main__x
   CMP   #<__local_main__y
   BEQ   .cmp_true@0
.thread@0:
   TYA
   JMP   .cmp_end@1
.cmp_true@0:
   LDA   #$01
.cmp_end@1:
   LDX   #$00
   RTS
