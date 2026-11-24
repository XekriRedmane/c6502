SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__local_main__1	EQU	$81
__local_target__x	EQU	$84
__local_callee__ptr	EQU	$86
__local_callee__0	EQU	$87

; @zp-link-meta-begin
; def callee params=__zpabi_callee__ptr_0,__zpabi_callee__ptr_1 locals=__local_callee__ptr,__local_callee__0 indirect=false in_cycle=false param_regs=A,X
; def main params= locals=__local_main__0,__local_main__1 indirect=false in_cycle=false
; def target params= locals=__local_target__y_1,__local_target__y_0,__local_target__x,__local_target__1 indirect=false in_cycle=false
; call main -> target
; call target -> callee
; item callee section=.text local=false zero=false
; item target section=.text local=false zero=false
; item main section=.text local=false zero=false
; item b section=.bss local=false zero=true
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

callee:
   SUBROUTINE

   STA   __local_callee__ptr
   STX   __local_callee__0
   LDY   #$00
   LDA   (__local_callee__ptr),Y
   STA   b
   INY
   LDA   (__local_callee__ptr),Y
   STA   b+1
   LDA   #$64
   DEY
   STA   (__local_callee__ptr),Y
   LDA   #$00
   INY
   STA   (__local_callee__ptr),Y
   RTS

target:
   SUBROUTINE

   LDA   #$0A
   STA   __local_target__x
   LDA   #$00
   STA   __local_target__x+1
   LDX   #>__local_target__x
   LDA   #<__local_target__x
   JSR   callee
   LDA   __local_target__x
   LDX   __local_target__x+1
   RTS

main:
   SUBROUTINE

   JSR   target
   STA   __local_main__1
   TXA
   BNE   .jcmp_skip@0
   LDA   __local_main__1
   CMP   #$64
   BEQ   .if_end@0
.jcmp_skip@0:
   LDX   #$00
   LDA   #$01
   RTS
.if_end@0:
   LDA   b+1
   BNE   .jcmp_skip@1
   LDA   b
   CMP   #$0A
   BEQ   .if_end@1
.jcmp_skip@1:
   LDX   #$00
   LDA   #$02
   RTS
.if_end@1:
   LDX   #$00
   TXA
   RTS

b:
   DS.B  2
