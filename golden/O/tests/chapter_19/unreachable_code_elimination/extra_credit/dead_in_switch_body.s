SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__local_target__x_0	EQU	$82

; @zp-link-meta-begin
; def callee params= locals= indirect=false in_cycle=false
; def main params= locals=__local_main__0 indirect=false in_cycle=false
; def target params=__zpabi_target__x_0,__zpabi_target__x_1 locals=__local_target__retval,__local_target__x_0 indirect=false in_cycle=false param_regs=A,X
; call main -> target
; item callee section=.text local=false zero=false
; item target section=.text local=false zero=false
; item main section=.text local=false zero=false
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

callee:
   SUBROUTINE

   LDX   #$FF
   TXA
   RTS

target:
   SUBROUTINE

   STA   __local_target__x_0
   TXA
   BNE   .jcmp_skip@0
   LDA   __local_target__x_0
   CMP   #$01
   BEQ   .case@1
.jcmp_skip@0:
   TXA
   BNE   .jcmp_skip@1
   LDA   __local_target__x_0
   CMP   #$02
   BEQ   .case@2
.jcmp_skip@1:
   TXA
   BNE   .default@4
   LDA   __local_target__x_0
   CMP   #$03
   BEQ   .case@3
   BNE   .default@4
.case@1:
   LDA   #$01
   LDX   #$00
   RTS
.case@2:
   LDA   #$02
   LDX   #$00
   RTS
.case@3:
   LDA   #$0A
   LDX   #$00
   RTS
.default@4:
   LDX   #$FF
   TXA
   RTS

main:
   SUBROUTINE

   LDX   #$00
   LDA   #$03
   JMP   target
