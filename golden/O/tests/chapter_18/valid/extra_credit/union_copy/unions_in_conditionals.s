SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__local_main__1	EQU	$81
__local_choose_union__one	EQU	$82
__local_choose_union__flag_0	EQU	$85

; @zp-link-meta-begin
; def choose_union params=__zpabi_choose_union__flag_0,__zpabi_choose_union__flag_1 locals=__local_choose_union__one,__local_choose_union__two,__local_choose_union__flag_1,__local_choose_union__flag_0 indirect=false in_cycle=false param_regs=A,X
; def main params= locals=__local_main__0,__local_main__1 indirect=false in_cycle=false
; call main -> choose_union
; item choose_union section=.text local=false zero=false
; item main section=.text local=false zero=false
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

choose_union:
   SUBROUTINE

   STA   __local_choose_union__flag_0
   LDA   #$FF
   STA   __local_choose_union__one
   TXA
   ORA   __local_choose_union__flag_0
   BNE   .cond_end@1
   LDA   #$64
   STA   __local_choose_union__one
.cond_end@1:
   LDX   #$00
   LDA   __local_choose_union__one
   RTS

main:
   SUBROUTINE

   LDX   #$00
   LDA   #$01
   JSR   choose_union
   CPX   #$FF
   BNE   .jcmp_skip@0
   CMP   #$FF
   BEQ   .if_end@2
.jcmp_skip@0:
   LDX   #$00
   LDA   #$01
   RTS
.if_end@2:
   LDX   #$00
   TXA
   JSR   choose_union
   STA   __local_main__1
   TXA
   BNE   .jcmp_skip@1
   LDA   __local_main__1
   CMP   #$64
   BEQ   .if_end@3
.jcmp_skip@1:
   LDX   #$00
   LDA   #$02
   RTS
.if_end@3:
   LDX   #$00
   TXA
   RTS
