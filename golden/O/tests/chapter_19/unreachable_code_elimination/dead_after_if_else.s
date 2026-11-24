SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__local_main__1	EQU	$81
__local_target__a_1	EQU	$82

; @zp-link-meta-begin
; def callee params= locals= indirect=false in_cycle=false
; def main params= locals=__local_main__0,__local_main__1 indirect=false in_cycle=false
; def target params=__zpabi_target__a_0,__zpabi_target__a_1 locals=__local_target__a_1,__local_target__a_0 indirect=false in_cycle=false param_regs=A,X
; call main -> target
; item callee section=.text local=false zero=false
; item target section=.text local=false zero=false
; item main section=.text local=false zero=false
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

callee:
   SUBROUTINE

   LDX   #$00
   LDA   #$64
   RTS

target:
   SUBROUTINE

   STX   __local_target__a_1
   ORA   __local_target__a_1
   BEQ   .if_else@1
   LDX   #$00
   LDA   #$01
   RTS
.if_else@1:
   LDX   #$00
   LDA   #$02
   RTS

main:
   SUBROUTINE

   LDX   #$00
   LDA   #$01
   JSR   target
   STA   __local_main__1
   TXA
   BNE   .jcmp_skip@0
   LDA   __local_main__1
   CMP   #$01
   BEQ   .if_end@2
.jcmp_skip@0:
   LDX   #$00
   LDA   #$01
   RTS
.if_end@2:
   LDX   #$00
   TXA
   JSR   target
   STA   __local_main__1
   TXA
   BNE   .jcmp_skip@1
   LDA   __local_main__1
   CMP   #$02
   BEQ   .if_end@3
.jcmp_skip@1:
   LDX   #$00
   LDA   #$02
   RTS
.if_end@3:
   LDX   #$00
   TXA
   RTS
