SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__zpabi_callee__a_0	EQU	$80
__zpabi_callee__a_1	EQU	$81
__zpabi_callee__a_2	EQU	$82
__zpabi_callee__a_3	EQU	$83
__zpabi_callee__b_0	EQU	$84
__zpabi_callee__b_1	EQU	$85
__zpabi_callee__b_2	EQU	$86
__zpabi_callee__b_3	EQU	$87
__local_callee__1	EQU	$8E
__local_callee__2	EQU	$8F
__local_callee__3	EQU	$90

; @zp-link-meta-begin
; def callee params=__zpabi_callee__a_0,__zpabi_callee__a_1,__zpabi_callee__a_2,__zpabi_callee__a_3,__zpabi_callee__b_0,__zpabi_callee__b_1,__zpabi_callee__b_2,__zpabi_callee__b_3 locals=__local_callee__0,__local_callee__1,__local_callee__2,__local_callee__3,__local_callee__4,__local_callee__5 indirect=false in_cycle=false changes=__zpabi_callee__a_0,__zpabi_callee__a_1,__zpabi_callee__a_2,__zpabi_callee__a_3,__zpabi_callee__b_0,__zpabi_callee__b_1,__zpabi_callee__b_2,__zpabi_callee__b_3
; def main params= locals=__local_main__0 indirect=false in_cycle=false
; def target params= locals=__local_target__u2_3,__local_target__u2_2,__local_target__u2_1,__local_target__u2_0 indirect=false in_cycle=false
; call main -> target
; call target -> callee
; item callee section=.text local=false zero=false
; item target section=.text local=false zero=false
; item main section=.text local=false zero=false
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

callee:
   SUBROUTINE

   LDA   __zpabi_callee__a_0
   STA   __local_callee__3
   LDA   __zpabi_callee__a_0+1
   STA   __local_callee__2
   LDA   __zpabi_callee__a_0+2
   STA   __local_callee__1
   LDA   __zpabi_callee__a_0+3
   CMP   #$FF
   BNE   .jcmp_skip@0
   LDA   __local_callee__1
   CMP   #$FF
   BNE   .jcmp_skip@0
   LDA   __local_callee__2
   CMP   #$FF
   BNE   .jcmp_skip@0
   LDA   __local_callee__3
   CMP   #$9C
   BEQ   .if_end@0
.jcmp_skip@0:
   LDX   #$00
   LDA   #$01
   RTS
.if_end@0:
   LDA   __zpabi_callee__b_0
   STA   __local_callee__3
   LDA   __zpabi_callee__b_0+1
   STA   __local_callee__2
   LDA   __zpabi_callee__b_0+2
   STA   __local_callee__1
   LDA   __zpabi_callee__b_0+3
   CMP   #$FF
   BNE   .jcmp_skip@1
   LDA   __local_callee__1
   CMP   #$FF
   BNE   .jcmp_skip@1
   LDA   __local_callee__2
   CMP   #$FF
   BNE   .jcmp_skip@1
   LDA   __local_callee__3
   CMP   #$9C
   BEQ   .if_end@1
.jcmp_skip@1:
   LDX   #$00
   LDA   #$02
   RTS
.if_end@1:
   LDX   #$00
   TXA
   RTS

target:
   SUBROUTINE

   LDA   #$FF
   LDY   #$9C
   STY   __zpabi_callee__a_0
   LDY   #$FF
   STY   __zpabi_callee__a_1
   STY   __zpabi_callee__a_2
   STA   __zpabi_callee__a_3
   LDY   #$9C
   STY   __zpabi_callee__b_0
   LDY   #$FF
   STY   __zpabi_callee__b_1
   STY   __zpabi_callee__b_2
   STA   __zpabi_callee__b_3
   JMP   callee

main:
   SUBROUTINE

   JMP   target
