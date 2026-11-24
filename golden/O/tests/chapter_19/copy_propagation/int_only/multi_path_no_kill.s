SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__local_main__1	EQU	$81
__local_target__flag_1	EQU	$82

; @zp-link-meta-begin
; def callee params= locals= indirect=false in_cycle=false
; def main params= locals=__local_main__0,__local_main__1 indirect=false in_cycle=false
; def target params=__zpabi_target__flag_0,__zpabi_target__flag_1 locals=__local_target__flag_1,__local_target__flag_0 indirect=false in_cycle=false param_regs=A,X
; call main -> target
; call target -> callee
; item callee section=.text local=false zero=false
; item target section=.text local=false zero=false
; item main section=.text local=false zero=false
; item var section=.bss local=false zero=true
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

callee:
   SUBROUTINE

   INC   var
   BNE   .inc_done@0
   INC   var+1
.inc_done@0:
   LDX   #$00
   TXA
   RTS

target:
   SUBROUTINE

   STX   __local_target__flag_1
   ORA   __local_target__flag_1
   BEQ   .if_end@0
   JSR   callee
.if_end@0:
   LDX   #$00
   LDA   #$03
   RTS

main:
   SUBROUTINE

   LDX   #$00
   TXA
   JSR   target
   STA   __local_main__1
   TXA
   BNE   .jcmp_skip@0
   LDA   __local_main__1
   CMP   #$03
   BEQ   .if_end@1
.jcmp_skip@0:
   LDX   #$00
   LDA   #$01
   RTS
.if_end@1:
   LDX   #$00
   LDA   #$01
   JSR   target
   STA   __local_main__1
   TXA
   BNE   .jcmp_skip@1
   LDA   __local_main__1
   CMP   #$03
   BEQ   .if_end@2
.jcmp_skip@1:
   LDX   #$00
   LDA   #$02
   RTS
.if_end@2:
   LDA   var+1
   BNE   .jcmp_skip@2
   LDA   var
   CMP   #$01
   BEQ   .if_end@3
.jcmp_skip@2:
   LDX   #$00
   LDA   #$03
   RTS
.if_end@3:
   LDX   #$00
   TXA
   RTS

var:
   DS.B  2
