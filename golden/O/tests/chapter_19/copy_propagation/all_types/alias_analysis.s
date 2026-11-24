SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__zpabi_target__ptr2_0	EQU	$80
__zpabi_target__ptr2_1	EQU	$81
__local_main__1	EQU	$83
__local_main__callee_check1	EQU	$86
__local_main__callee_check2	EQU	$88
__local_target__0	EQU	$8A
__local_target__ptr1	EQU	$8C
__local_target__2	EQU	$8D
__local_target__i	EQU	$8E
__local_callee__1	EQU	$91
__local_callee__ptr	EQU	$92
__local_callee__2	EQU	$93

; @zp-link-meta-begin
; def callee params=__zpabi_callee__ptr_0,__zpabi_callee__ptr_1 locals=__local_callee__0,__local_callee__1,__local_callee__ptr,__local_callee__2 indirect=false in_cycle=false param_regs=A,X
; def main params= locals=__local_main__0,__local_main__1,__local_main__2,__local_main__3,__local_main__callee_check1,__local_main__5,__local_main__callee_check2,__local_main__7 indirect=false in_cycle=false
; def target params=__zpabi_target__ptr1_0,__zpabi_target__ptr1_1,__zpabi_target__ptr2_0,__zpabi_target__ptr2_1 locals=__local_target__0,__local_target__1,__local_target__ptr1,__local_target__2,__local_target__i,__local_target__4 indirect=false in_cycle=false param_regs=A,X,,
; call main -> target
; call target -> callee
; item callee section=.text local=false zero=false
; item target section=.text local=false zero=false
; item main section=.text local=false zero=false
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

callee:
   SUBROUTINE

   STA   __local_callee__ptr
   STX   __local_callee__2
   LDY   #$00
   LDA   (__local_callee__ptr),Y
   STA   __local_callee__1
   INY
   LDA   (__local_callee__ptr),Y
   BNE   .jcmp_skip@0
   LDA   __local_callee__1
   CMP   #$0A
   BEQ   .if_end@0
.jcmp_skip@0:
   LDX   #$00
   TXA
   RTS
.if_end@0:
   LDA   #$FF
   LDY   #$00
   STA   (__local_callee__ptr),Y
   INY
   STA   (__local_callee__ptr),Y
   LDX   #$00
   LDA   #$01
   RTS

target:
   SUBROUTINE

   STA   __local_target__ptr1
   STX   __local_target__2
   LDA   #$0A
   STA   __local_target__i
   LDA   #$00
   STA   __local_target__i+1
   LDX   #>__local_target__i
   LDA   #<__local_target__i
   JSR   callee
   LDY   #$00
   STA   (__local_target__ptr1),Y
   TXA
   INY
   STA   (__local_target__ptr1),Y
   LDA   __local_target__i
   DEY
   STA   (__zpabi_target__ptr2_0),Y
   LDA   __local_target__i+1
   INY
   STA   (__zpabi_target__ptr2_0),Y
   LDA   #$04
   DEY
   CLC
   ADC   #$14
   STA   __local_target__0
   TYA
   ADC   #$00
   TAX
   LDA   __local_target__0
   RTS

main:
   SUBROUTINE

   LDA   #<__local_main__callee_check2
   STA   __zpabi_target__ptr2_0
   LDA   #>__local_main__callee_check2
   STA   __zpabi_target__ptr2_1
   LDX   #>__local_main__callee_check1
   LDA   #<__local_main__callee_check1
   JSR   target
   STA   __local_main__1
   LDA   __local_main__callee_check1+1
   BNE   .jcmp_skip@1
   LDA   __local_main__callee_check1
   CMP   #$01
   BEQ   .if_end@1
.jcmp_skip@1:
   LDX   #$00
   LDA   #$01
   RTS
.if_end@1:
   LDA   __local_main__callee_check2+1
   CMP   #$FF
   BNE   .jcmp_skip@2
   LDA   __local_main__callee_check2
   CMP   #$FF
   BEQ   .if_end@2
.jcmp_skip@2:
   LDX   #$00
   LDA   #$02
   RTS
.if_end@2:
   TXA
   BNE   .jcmp_skip@3
   LDA   __local_main__1
   CMP   #$18
   BEQ   .if_end@3
.jcmp_skip@3:
   LDX   #$00
   LDA   #$03
   RTS
.if_end@3:
   LDX   #$00
   TXA
   RTS
