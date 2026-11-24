SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__local_main__1	EQU	$81
__local_main__2	EQU	$82
__local_main__3	EQU	$83
__local_main__x	EQU	$84

; @zp-link-meta-begin
; def main params= locals=__local_main__0,__local_main__1,__local_main__2,__local_main__3,__local_main__x,__local_main__5 indirect=false in_cycle=false
; def return_pointer params=__zpabi_return_pointer__in_0,__zpabi_return_pointer__in_1 locals=__local_return_pointer__in indirect=false in_cycle=false param_regs=A,X
; call main -> return_pointer
; item return_pointer section=.text local=false zero=false
; item main section=.text local=false zero=false
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

return_pointer:
   SUBROUTINE

   RTS

main:
   SUBROUTINE

   LDA   #$0A
   STA   __local_main__x
   LDA   #$00
   STA   __local_main__x+1
   LDX   #>__local_main__x
   LDA   #<__local_main__x
   JSR   return_pointer
   STA   __local_main__2
   STX   __local_main__3
   LDY   #$00
   LDA   (__local_main__2),Y
   STA   __local_main__1
   INY
   LDA   (__local_main__2),Y
   BNE   .jcmp_skip@0
   LDA   __local_main__1
   CMP   #$0A
   BEQ   .if_end@0
.jcmp_skip@0:
   LDX   #$00
   LDA   #$01
   RTS
.if_end@0:
   LDA   #$64
   STA   __local_main__x
   LDA   #$00
   STA   __local_main__x+1
   LDY   #$00
   LDA   (__local_main__2),Y
   STA   __local_main__1
   INY
   LDA   (__local_main__2),Y
   BNE   .jcmp_skip@1
   LDA   __local_main__1
   CMP   #$64
   BEQ   .if_end@1
.jcmp_skip@1:
   LDX   #$00
   LDA   #$02
   RTS
.if_end@1:
   TXA
   CMP   #>__local_main__x
   BNE   .jcmp_skip@2
   LDA   __local_main__2
   CMP   #<__local_main__x
   BEQ   .if_end@2
.jcmp_skip@2:
   LDX   #$00
   LDA   #$03
   RTS
.if_end@2:
   LDX   #$00
   TXA
   RTS
