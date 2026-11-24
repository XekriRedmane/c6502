SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__local_target__flag_0	EQU	$81

; @zp-link-meta-begin
; def callee params=__zpabi_callee__arg_0,__zpabi_callee__arg_1 locals=__local_callee__arg_1,__local_callee__arg_0 indirect=false in_cycle=false param_regs=A,X
; def main params= locals= indirect=false in_cycle=false
; def target params=__zpabi_target__flag_0,__zpabi_target__flag_1 locals=__local_target__flag_1,__local_target__flag_0 indirect=false in_cycle=false param_regs=A,X
; call main -> target
; call target -> callee
; item callee section=.text local=false zero=false
; item target section=.text local=false zero=false
; item main section=.text local=false zero=false
; item globvar section=.bss local=false zero=true
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

callee:
   SUBROUTINE

   STA   globvar
   STX   globvar+1
   LDX   #$00
   TXA
   RTS

target:
   SUBROUTINE

   STA   __local_target__flag_0
   TXA
   BNE   .jcmp_skip@0
   LDA   __local_target__flag_0
   CMP   #$01
   BEQ   .switch@0_break
.jcmp_skip@0:
   TXA
   BNE   .default@3
   LDA   __local_target__flag_0
   CMP   #$02
   BNE   .default@3
   LDX   #$00
   LDA   #$0A
   JSR   callee
   JMP   .switch@0_break
.default@3:
   LDA   #$FF
   STA   globvar
   STA   globvar+1
.switch@0_break:
   LDX   #$00
   TXA
   RTS

main:
   SUBROUTINE

   LDX   #$00
   LDA   #$02
   JSR   target
   LDA   globvar+1
   BNE   .if_end@0
   LDA   globvar
   CMP   #$0A
   BNE   .if_end@0
   LDX   #$00
   TXA
   RTS
.if_end@0:
   LDX   #$00
   LDA   #$01
   RTS

globvar:
   DS.B  2
