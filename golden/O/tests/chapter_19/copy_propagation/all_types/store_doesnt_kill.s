SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__zpabi_callee__p2_0	EQU	$80
__zpabi_check_pointers__b_0	EQU	$80
__zpabi_callee__p2_1	EQU	$81
__zpabi_check_pointers__b_1	EQU	$81
__zpabi_check_pointers__ptr1_0	EQU	$82
__local_callee__p1_0	EQU	$83
__zpabi_check_pointers__ptr1_1	EQU	$83
__zpabi_check_pointers__ptr2_0	EQU	$84
__zpabi_target__ptr2_0	EQU	$84
__zpabi_check_pointers__ptr2_1	EQU	$85
__zpabi_target__ptr2_1	EQU	$85
__local_main__x	EQU	$8A
__local_main__y	EQU	$8C
__local_target__ptr	EQU	$8E
__local_target__0	EQU	$8F
__local_check_pointers__a_0	EQU	$92

; @zp-link-meta-begin
; def callee params=__zpabi_callee__p1_0,__zpabi_callee__p1_1,__zpabi_callee__p2_0,__zpabi_callee__p2_1 locals=__local_callee__p1_1,__local_callee__p1_0 indirect=false in_cycle=false param_regs=A,X,,
; def check_pointers params=__zpabi_check_pointers__a_0,__zpabi_check_pointers__a_1,__zpabi_check_pointers__b_0,__zpabi_check_pointers__b_1,__zpabi_check_pointers__ptr1_0,__zpabi_check_pointers__ptr1_1,__zpabi_check_pointers__ptr2_0,__zpabi_check_pointers__ptr2_1 locals=__local_check_pointers__a_1,__local_check_pointers__a_0 indirect=false in_cycle=false param_regs=A,X,,,,,,
; def main params= locals=__local_main__0,__local_main__1,__local_main__2,__local_main__3,__local_main__x,__local_main__5,__local_main__y,__local_main__7 indirect=false in_cycle=false
; def target params=__zpabi_target__ptr_0,__zpabi_target__ptr_1,__zpabi_target__ptr2_0,__zpabi_target__ptr2_1 locals=__local_target__ptr,__local_target__0,__local_target__1 indirect=false in_cycle=false param_regs=A,X,,
; ext exit params=__zpabi_exit__status_0,__zpabi_exit__status_1 param_regs=A,X
; call callee -> exit
; call check_pointers -> exit
; call main -> target
; call target -> callee
; call target -> check_pointers
; share member=check_pointers:__zpabi_check_pointers__ptr2_0,__zpabi_check_pointers__ptr2_1 member=target:__zpabi_target__ptr2_0,__zpabi_target__ptr2_1 forward=target>check_pointers
; item check_pointers section=.text local=false zero=false
; item callee section=.text local=false zero=false
; item target section=.text local=false zero=false
; item main section=.text local=false zero=false
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

check_pointers:
   SUBROUTINE

   STA   __local_check_pointers__a_0
   TXA
   BNE   .check_pointers@jthread@0
   LDA   __local_check_pointers__a_0
   CMP   #$64
   BNE   .check_pointers@jthread@0
   LDA   __zpabi_check_pointers__b_1
   BNE   .check_pointers@jthread@0
   LDA   __zpabi_check_pointers__b_0
   CMP   #$65
   BEQ   .if_end@2
.check_pointers@jthread@0:
   LDX   #$00
   LDA   #$01
   JSR   exit
.if_end@2:
   LDY   #$00
   LDA   (__zpabi_check_pointers__ptr1_0),Y
   STA   __local_check_pointers__a_0
   INY
   LDA   (__zpabi_check_pointers__ptr1_0),Y
   BNE   .check_pointers@jthread@1
   LDA   __local_check_pointers__a_0
   CMP   #$3C
   BNE   .check_pointers@jthread@1
   DEY
   LDA   (__zpabi_check_pointers__ptr2_0),Y
   STA   __local_check_pointers__a_0
   INY
   LDA   (__zpabi_check_pointers__ptr2_0),Y
   BNE   .check_pointers@jthread@1
   LDA   __local_check_pointers__a_0
   CMP   #$3D
   BEQ   .if_end@5
.check_pointers@jthread@1:
   LDX   #$00
   LDA   #$02
   JSR   exit
.if_end@5:
   RTS

callee:
   SUBROUTINE

   CPX   __zpabi_callee__p2_1
   BNE   .jcmp_skip@0
   CMP   __zpabi_callee__p2_0
   BEQ   .if_end@6
.jcmp_skip@0:
   LDX   #$00
   LDA   #$03
   JSR   exit
.if_end@6:
   LDY   #$00
   LDA   (__zpabi_callee__p2_0),Y
   STA   __local_callee__p1_0
   INY
   LDA   (__zpabi_callee__p2_0),Y
   BNE   .jcmp_skip@1
   LDA   __local_callee__p1_0
   CMP   #$0A
   BEQ   .if_end@7
.jcmp_skip@1:
   LDX   #$00
   LDA   #$04
   JSR   exit
.if_end@7:
   LDX   #$00
   TXA
   RTS

target:
   SUBROUTINE

   STA   __local_target__ptr
   STX   __local_target__0
   LDA   #$65
   STA   __zpabi_check_pointers__b_0
   LDA   #$00
   STA   __zpabi_check_pointers__b_1
   LDA   __local_target__ptr
   STA   __zpabi_check_pointers__ptr1_0
   STX   __zpabi_check_pointers__ptr1_1
   LDX   #$00
   LDA   #$64
   JSR   check_pointers
   LDA   #$0A
   LDY   #$00
   STA   (__local_target__ptr),Y
   LDA   #$00
   INY
   STA   (__local_target__ptr),Y
   LDA   __local_target__ptr
   STA   __zpabi_callee__p2_0
   LDA   __local_target__0
   STA   __zpabi_callee__p2_1
   TAX
   LDA   __local_target__ptr
   JMP   callee

main:
   SUBROUTINE

   LDA   #$3C
   STA   __local_main__x
   LDA   #$00
   STA   __local_main__x+1
   LDA   #$3D
   STA   __local_main__y
   LDA   #$00
   STA   __local_main__y+1
   LDA   #<__local_main__y
   STA   __zpabi_target__ptr2_0
   LDA   #>__local_main__y
   STA   __zpabi_target__ptr2_1
   LDX   #>__local_main__x
   LDA   #<__local_main__x
   JMP   target
