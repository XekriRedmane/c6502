SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__local_main__1	EQU	$81
__local_multi_path__x	EQU	$82

; @zp-link-meta-begin
; def main params= locals=__local_main__0,__local_main__1 indirect=false in_cycle=false
; def multi_path params=__zpabi_multi_path__flag_0,__zpabi_multi_path__flag_1 locals=__local_multi_path__x,__local_multi_path__flag_0 indirect=false in_cycle=false param_regs=A,X
; call main -> multi_path
; item multi_path section=.text local=false zero=false
; item main section=.text local=false zero=false
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

multi_path:
   SUBROUTINE

   STX   __local_multi_path__x
   ORA   __local_multi_path__x
   BEQ   .multi_path@asm_ssa_split@0
   LDA   #$04
   LDX   #$00
   RTS
.multi_path@asm_ssa_split@0:
   LDA   #$03
   LDX   #$00
   RTS

main:
   SUBROUTINE

   LDX   #$00
   LDA   #$01
   JSR   multi_path
   STA   __local_main__1
   TXA
   BNE   .jcmp_skip@0
   LDA   __local_main__1
   CMP   #$04
   BEQ   .if_end@1
.jcmp_skip@0:
   LDX   #$00
   LDA   #$01
   RTS
.if_end@1:
   LDX   #$00
   TXA
   JSR   multi_path
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
   LDX   #$00
   TXA
   RTS
