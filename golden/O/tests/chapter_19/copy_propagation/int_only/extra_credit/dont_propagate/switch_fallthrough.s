SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__local_target__flag_0	EQU	$82

; @zp-link-meta-begin
; def main params= locals=__local_main__0 indirect=false in_cycle=false
; def target params=__zpabi_target__flag_0,__zpabi_target__flag_1 locals=__local_target__retval,__local_target__flag_0 indirect=false in_cycle=false param_regs=A,X
; call main -> target
; item target section=.text local=false zero=false
; item main section=.text local=false zero=false
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

target:
   SUBROUTINE

   STA   __local_target__flag_0
   TXA
   BNE   .jcmp_skip@0
   LDA   __local_target__flag_0
   CMP   #$01
   BEQ   .case@1
.jcmp_skip@0:
   TXA
   BNE   .default@3
   LDA   __local_target__flag_0
   CMP   #$02
   BEQ   .target@asm_ssa_split@0
   BNE   .default@3
.case@1:
   LDA   #$00
   LDX   #$00
   RTS
.default@3:
   LDX   #$FF
   TXA
   RTS
.target@asm_ssa_split@0:
   LDA   #$0A
   LDX   #$00
   RTS

main:
   SUBROUTINE

   LDX   #$00
   LDA   #$01
   JMP   target
