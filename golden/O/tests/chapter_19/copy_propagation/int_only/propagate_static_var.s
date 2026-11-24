SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__zpabi_callee__b_0	EQU	$80
__zpabi_callee__b_1	EQU	$81
__local_main__1	EQU	$83
__local_callee__a_0	EQU	$85

; @zp-link-meta-begin
; def callee params=__zpabi_callee__a_0,__zpabi_callee__a_1,__zpabi_callee__b_0,__zpabi_callee__b_1 locals=__local_callee__a_0,__local_callee__a_1 indirect=false in_cycle=false param_regs=A,X,,
; def main params= locals=__local_main__0,__local_main__1 indirect=false in_cycle=false
; def target params= locals=__local_target__0 indirect=false in_cycle=false
; call main -> target
; call target -> callee
; item callee section=.text local=false zero=false
; item target section=.text local=false zero=false
; item main section=.text local=false zero=false
; item @2.x section=.data local=true zero=false
; item @3.y section=.bss local=true zero=true
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

callee:
   SUBROUTINE

   CLC
   ADC   __zpabi_callee__b_0
   STA   __local_callee__a_0
   TXA
   ADC   __zpabi_callee__b_1
   TAX
   LDA   __local_callee__a_0
   RTS

target:
   SUBROUTINE

   LDA   @2.x
   STA   @3.y
   STA   __zpabi_callee__b_0
   LDA   @2.x+1
   STA   @3.y+1
   STA   __zpabi_callee__b_1
   TAX
   LDA   @2.x
   JSR   callee
   INC   @2.x
   BNE   .inc_done@0
   INC   @2.x+1
.inc_done@0:
   RTS

main:
   SUBROUTINE

   JSR   target
   STA   __local_main__1
   TXA
   BNE   .jcmp_skip@0
   LDA   __local_main__1
   CMP   #$06
   BEQ   .if_end@0
.jcmp_skip@0:
   LDX   #$00
   LDA   #$01
   RTS
.if_end@0:
   JSR   target
   STA   __local_main__1
   TXA
   BNE   .jcmp_skip@1
   LDA   __local_main__1
   CMP   #$08
   BEQ   .if_end@1
.jcmp_skip@1:
   LDX   #$00
   LDA   #$02
   RTS
.if_end@1:
   LDX   #$00
   TXA
   RTS

@2.x:
   DC.W  $0003

@3.y:
   DS.B  2
