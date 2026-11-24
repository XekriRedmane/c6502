SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__local_main__x	EQU	$82
__local_f__0	EQU	$84
__local_f__1	EQU	$85
__local_f__ptr	EQU	$86
__local_f__2	EQU	$87

; @zp-link-meta-begin
; def f params=__zpabi_f__ptr_0,__zpabi_f__ptr_1 locals=__local_f__0,__local_f__1,__local_f__ptr,__local_f__2 indirect=false in_cycle=false param_regs=A,X
; def main params= locals=__local_main__0,__local_main__1,__local_main__x,__local_main__3 indirect=false in_cycle=false
; call main -> f
; item f section=.text local=false zero=false
; item main section=.text local=false zero=false
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

f:
   SUBROUTINE

   STA   __local_f__ptr
   STX   __local_f__2
   LDY   #$00
   LDA   (__local_f__ptr),Y
   STA   __local_f__0
   INY
   LDA   (__local_f__ptr),Y
   STA   __local_f__1
   INC   __local_f__0
   BNE   .inc_done@0
   INC   __local_f__1
.inc_done@0:
   LDA   __local_f__0
   LDY   #$00
   STA   (__local_f__ptr),Y
   LDA   __local_f__1
   INY
   STA   (__local_f__ptr),Y
   RTS

main:
   SUBROUTINE

   LDA   #$00
   STA   __local_main__x
   STA   __local_main__x+1
   LDX   #>__local_main__x
   LDA   #<__local_main__x
   JSR   f
   LDX   __local_main__x+1
   LDA   __local_main__x
   RTS
