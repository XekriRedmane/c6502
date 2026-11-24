SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__zpabi_foo__y_0	EQU	$80
__zpabi_foo__y_1	EQU	$81
__local_main__1	EQU	$83
__local_main__2	EQU	$84
__local_main__3	EQU	$85

; @zp-link-meta-begin
; def foo params=__zpabi_foo__x_0,__zpabi_foo__x_1,__zpabi_foo__y_0,__zpabi_foo__y_1 locals=__local_foo__x_1 indirect=false in_cycle=false param_regs=A,X,,
; def main params= locals=__local_main__0,__local_main__1,__local_main__2,__local_main__3 indirect=false in_cycle=false
; call main -> foo
; item foo section=.text local=false zero=false
; item main section=.text local=false zero=false
; item arr section=.data local=false zero=false
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

foo:
   SUBROUTINE

   STA   arr+2
   STX   arr+3
   LDA   __zpabi_foo__y_0
   STA   arr+4
   LDA   __zpabi_foo__y_1
   STA   arr+5
   LDX   #>arr
   LDA   #<arr
   RTS

main:
   SUBROUTINE

   LDA   #$03
   STA   __zpabi_foo__y_0
   LDA   #$00
   STA   __zpabi_foo__y_1
   LDX   #$00
   LDA   #$02
   JSR   foo
   STA   __local_main__2
   STX   __local_main__3
   LDY   #$00
   LDA   (__local_main__2),Y
   STA   __local_main__1
   INY
   LDA   (__local_main__2),Y
   BNE   .jcmp_skip@0
   LDA   __local_main__1
   CMP   #$01
   BEQ   .if_end@0
.jcmp_skip@0:
   LDX   #$00
   LDA   #$01
   RTS
.if_end@0:
   LDY   #$02
   LDA   (__local_main__2),Y
   STA   __local_main__1
   INY
   LDA   (__local_main__2),Y
   BNE   .jcmp_skip@1
   LDA   __local_main__1
   CMP   #$02
   BEQ   .if_end@1
.jcmp_skip@1:
   LDX   #$00
   LDA   #$02
   RTS
.if_end@1:
   LDY   #$04
   LDA   (__local_main__2),Y
   STA   __local_main__1
   INY
   LDA   (__local_main__2),Y
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

arr:
   DC.W  $0001
   DC.W  $0001
   DC.W  $0001
