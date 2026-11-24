SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__local_main__1	EQU	$81

; @zp-link-meta-begin
; def main params= locals=__local_main__0,__local_main__1 indirect=false in_cycle=false
; def target params=__zpabi_target__flag_0,__zpabi_target__flag_1 locals=__local_target__flag_1,__local_target__flag_0 indirect=false in_cycle=false param_regs=A,X
; call main -> target
; item target section=.text local=false zero=false
; item main section=.text local=false zero=false
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

target:
   SUBROUTINE

   LDX   #$00
   LDA   #$03
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
   CMP   #$03
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
   CMP   #$03
   BEQ   .if_end@3
.jcmp_skip@1:
   LDX   #$00
   LDA   #$02
   RTS
.if_end@3:
   LDX   #$00
   TXA
   RTS
