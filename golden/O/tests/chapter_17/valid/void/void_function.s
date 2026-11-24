SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__local_set_foo_to_positive_num__a_0	EQU	$81

; @zp-link-meta-begin
; def do_nothing params= locals= indirect=false in_cycle=false
; def main params= locals= indirect=false in_cycle=false
; def set_foo_to_positive_num params=__zpabi_set_foo_to_positive_num__a_0,__zpabi_set_foo_to_positive_num__a_1 locals=__local_set_foo_to_positive_num__a_1,__local_set_foo_to_positive_num__a_0 indirect=false in_cycle=false param_regs=A,X
; call main -> do_nothing
; call main -> set_foo_to_positive_num
; item set_foo_to_positive_num section=.text local=false zero=false
; item do_nothing section=.text local=false zero=false
; item main section=.text local=false zero=false
; item foo section=.bss local=false zero=true
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

set_foo_to_positive_num:
   SUBROUTINE

   STA   __local_set_foo_to_positive_num__a_0
   TXA
   BPL   .if_end@0
   RTS
.if_end@0:
   STA   foo+1
   LDA   __local_set_foo_to_positive_num__a_0
   STA   foo
   RTS

do_nothing:
   SUBROUTINE

   RTS

main:
   SUBROUTINE

   LDX   #$FF
   LDA   #$FE
   JSR   set_foo_to_positive_num
   LDA   foo
   ORA   foo+1
   BEQ   .if_end@1
   LDX   #$00
   LDA   #$01
   RTS
.if_end@1:
   LDX   #$00
   LDA   #$0C
   JSR   set_foo_to_positive_num
   LDA   foo+1
   BNE   .jcmp_skip@0
   LDA   foo
   CMP   #$0C
   BEQ   .if_end@2
.jcmp_skip@0:
   LDX   #$00
   LDA   #$02
   RTS
.if_end@2:
   JSR   do_nothing
   LDX   #$00
   TXA
   RTS

foo:
   DS.B  2
