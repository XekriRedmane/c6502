SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__local_main__1	EQU	$81
__local_main__a	EQU	$86
__local_main__ptr	EQU	$88
__local_subscript_pointer_to_pointer__1	EQU	$8B
__local_subscript_pointer_to_pointer__2	EQU	$8C
__local_subscript_pointer_to_pointer__x	EQU	$8D
__local_subscript_pointer_to_pointer__3	EQU	$8E

; @zp-link-meta-begin
; def main params= locals=__local_main__0,__local_main__1,__local_main__2,__local_main__3,__local_main__4,__local_main__5,__local_main__a,__local_main__7,__local_main__ptr,__local_main__9 indirect=false in_cycle=false
; def subscript_pointer_to_pointer params=__zpabi_subscript_pointer_to_pointer__x_0,__zpabi_subscript_pointer_to_pointer__x_1 locals=__local_subscript_pointer_to_pointer__0,__local_subscript_pointer_to_pointer__1,__local_subscript_pointer_to_pointer__2,__local_subscript_pointer_to_pointer__x,__local_subscript_pointer_to_pointer__3 indirect=false in_cycle=false param_regs=A,X
; call main -> subscript_pointer_to_pointer
; item subscript_pointer_to_pointer section=.text local=false zero=false
; item main section=.text local=false zero=false
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

subscript_pointer_to_pointer:
   SUBROUTINE

   STA   __local_subscript_pointer_to_pointer__x
   STX   __local_subscript_pointer_to_pointer__3
   LDY   #$00
   LDA   (__local_subscript_pointer_to_pointer__x),Y
   STA   __local_subscript_pointer_to_pointer__1
   INY
   LDA   (__local_subscript_pointer_to_pointer__x),Y
   STA   __local_subscript_pointer_to_pointer__2
   LDA   (__local_subscript_pointer_to_pointer__1),Y
   TAX
   DEY
   LDA   (__local_subscript_pointer_to_pointer__1),Y
   RTS

main:
   SUBROUTINE

   LDA   #$00
   STA   __local_main__a+1
   LDA   #<__local_main__a
   STA   __local_main__ptr
   LDA   #$03
   STA   __local_main__a
   LDA   #$00
   BNE   .jcmp_skip@0
   LDA   #$03
   CMP   #$03
   BEQ   .if_end@1
.jcmp_skip@0:
   LDX   #$00
   LDA   #$01
   RTS
.if_end@1:
   LDX   #>__local_main__ptr
   LDA   #<__local_main__ptr
   JSR   subscript_pointer_to_pointer
   STA   __local_main__1
   TXA
   BNE   .jcmp_skip@2
   LDA   __local_main__1
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
