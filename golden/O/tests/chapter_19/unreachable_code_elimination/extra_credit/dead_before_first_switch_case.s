SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__local_target__x_0	EQU	$82

; @zp-link-meta-begin
; def callee params= locals= indirect=false in_cycle=false
; def main params= locals=__local_main__0 indirect=false in_cycle=false
; def target params=__zpabi_target__x_0,__zpabi_target__x_1 locals=__local_target__x_1,__local_target__x_0 indirect=false in_cycle=false param_regs=A,X
; call main -> target
; item callee section=.text local=false zero=false
; item target section=.text local=false zero=false
; item main section=.text local=false zero=false
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

callee:
   SUBROUTINE

   LDX   #$00
   TXA
   RTS

target:
   SUBROUTINE

   STA   __local_target__x_0
   TXA
   BNE   .default@2
   LDA   __local_target__x_0
   CMP   #$01
   BNE   .default@2
   LDX   #$00
   LDA   #$01
   RTS
.default@2:
   LDX   #$00
   LDA   #$02
   RTS

main:
   SUBROUTINE

   LDX   #$00
   LDA   #$01
   JMP   target
