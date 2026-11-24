SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__local_main__2	EQU	$82
__local_main__4	EQU	$84
__local_main__arr	EQU	$85

; @zp-link-meta-begin
; def main params= locals=__local_main__0,__local_main__1,__local_main__2,__local_main__3,__local_main__4,__local_main__arr,__local_main__6,__local_main__7,__local_main__8,__local_main__9,__local_main__10 indirect=false in_cycle=false
; item main section=.text local=false zero=false
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

main:
   SUBROUTINE

   LDA   #>__local_main__arr
   STA   __local_main__4
   LDA   #<__local_main__arr
   CLC
   ADC   #$04
   STA   __local_main__2
   BCC   .carry_inc@0
   INC   __local_main__4
.carry_inc@0:
   LDA   #<__local_main__arr
   SEC
   SBC   __local_main__2
   LDA   #>__local_main__arr
   SBC   __local_main__4
   BCS   .if_end@0
   LDX   #$00
   TXA
   RTS
.if_end@0:
   LDX   #$00
   LDA   #$01
   RTS
