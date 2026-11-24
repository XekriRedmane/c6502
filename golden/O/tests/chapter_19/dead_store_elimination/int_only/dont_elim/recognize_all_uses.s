SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__zpabi_test_binary__arg1_0	EQU	$80
__zpabi_test_funcall__arg_0	EQU	$80
__zpabi_test_jnz__arg_0	EQU	$80
__zpabi_test_jz__arg_0	EQU	$80
__zpabi_test_unary__arg_0	EQU	$80
__zpabi_test_binary__arg1_1	EQU	$81
__zpabi_test_funcall__arg_1	EQU	$81
__zpabi_test_jnz__arg_1	EQU	$81
__zpabi_test_jz__arg_1	EQU	$81
__zpabi_test_unary__arg_1	EQU	$81
__local_test_funcall__arg	EQU	$82
__local_test_jnz__flag_1	EQU	$82
__local_test_jz__flag_1	EQU	$82
__local_test_unary__arg_0	EQU	$82
__zpabi_test_binary__arg2_0	EQU	$82
__zpabi_test_binary__arg2_1	EQU	$83
__local_main__0	EQU	$84
__local_main__1	EQU	$85
__local_f__arg	EQU	$86
__local_test_binary__arg2_1	EQU	$86
__local_test_binary__arg2_0	EQU	$87

; @zp-link-meta-begin
; def f params=__zpabi_f__arg_0,__zpabi_f__arg_1 locals=__local_f__arg indirect=false in_cycle=false param_regs=A,X
; def main params= locals=__local_main__0,__local_main__1 indirect=false in_cycle=false
; def test_binary params=__zpabi_test_binary__flag_0,__zpabi_test_binary__flag_1,__zpabi_test_binary__arg1_0,__zpabi_test_binary__arg1_1,__zpabi_test_binary__arg2_0,__zpabi_test_binary__arg2_1 locals=__local_test_binary__arg2_1,__local_test_binary__arg2_0,__local_test_binary__arg1_1,__local_test_binary__arg1_0 indirect=false in_cycle=false param_regs=A,X,,,,
; def test_funcall params=__zpabi_test_funcall__flag_0,__zpabi_test_funcall__flag_1,__zpabi_test_funcall__arg_0,__zpabi_test_funcall__arg_1 locals=__local_test_funcall__arg,__local_test_funcall__flag_0 indirect=false in_cycle=false param_regs=A,X,,
; def test_jnz params=__zpabi_test_jnz__flag_0,__zpabi_test_jnz__flag_1,__zpabi_test_jnz__arg_0,__zpabi_test_jnz__arg_1 locals=__local_test_jnz__flag_1,__local_test_jnz__flag_0 indirect=false in_cycle=false param_regs=A,X,,
; def test_jz params=__zpabi_test_jz__flag_0,__zpabi_test_jz__flag_1,__zpabi_test_jz__arg_0,__zpabi_test_jz__arg_1 locals=__local_test_jz__flag_1,__local_test_jz__flag_0 indirect=false in_cycle=false param_regs=A,X,,
; def test_unary params=__zpabi_test_unary__flag_0,__zpabi_test_unary__flag_1,__zpabi_test_unary__arg_0,__zpabi_test_unary__arg_1 locals=__local_test_unary__arg_0,__local_test_unary__arg_1 indirect=false in_cycle=false param_regs=A,X,, changes=__zpabi_test_unary__arg_1
; call main -> test_binary
; call main -> test_funcall
; call main -> test_jnz
; call main -> test_jz
; call main -> test_unary
; call test_funcall -> f
; item test_jz section=.text local=false zero=false
; item test_jnz section=.text local=false zero=false
; item test_binary section=.text local=false zero=false
; item test_unary section=.text local=false zero=false
; item f section=.text local=false zero=false
; item test_funcall section=.text local=false zero=false
; item main section=.text local=false zero=false
; item main@run3 section=.text local=true zero=false
; item main@run2 section=.text local=true zero=false
; item main@run1 section=.text local=true zero=false
; item main@run0 section=.text local=true zero=false
; runtime hargs_size=32 hargs_need=10
; @zp-link-meta-end

test_jz:
   SUBROUTINE

   STX   __local_test_jz__flag_1
   ORA   __local_test_jz__flag_1
   BNE   .cond_else@1
   LDA   __zpabi_test_jz__arg_0
   ORA   __zpabi_test_jz__arg_1
   BEQ   .cond_else@1
   LDA   #$01
   LDX   #$00
   RTS
.cond_else@1:
   LDA   #$02
   LDX   #$00
   RTS

test_jnz:
   SUBROUTINE

   STX   __local_test_jnz__flag_1
   ORA   __local_test_jnz__flag_1
   BNE   .test_jnz@ssa_block@3
   LDA   __zpabi_test_jnz__arg_0
   ORA   __zpabi_test_jnz__arg_1
   BNE   .or_true@4
.test_jnz@ssa_block@3:
   LDA   #$00
   LDX   #$00
   RTS
.or_true@4:
   LDA   #$01
   LDX   #$00
   RTS

test_binary:
   SUBROUTINE

   STA   __local_test_binary__arg2_0
   STX   __local_test_binary__arg2_1
   ORA   __local_test_binary__arg2_1
   BNE   .if_else@7
   LDA   #$04
   STA   HARGS
   LDA   #$00
   STA   HARGS+1
   LDA   __zpabi_test_binary__arg2_0
   STA   __local_test_binary__arg2_0
   LDA   __zpabi_test_binary__arg2_1
   STA   __local_test_binary__arg2_1
   JMP   .if_end@6
.if_else@7:
   TXA
   BNE   .test_binary@asm_ssa_split@0
   LDA   __local_test_binary__arg2_0
   CMP   #$01
   BNE   .test_binary@asm_ssa_split@0
   LDA   #$03
   STA   __local_test_binary__arg2_0
   LDA   #$00
   STA   __local_test_binary__arg2_1
.if_end@8:
   LDA   __zpabi_test_binary__arg1_0
   STA   HARGS
   LDA   __zpabi_test_binary__arg1_1
   STA   HARGS+1
.if_end@6:
   LDA   __local_test_binary__arg2_0
   STA   HARGS+2
   LDA   __local_test_binary__arg2_1
   STA   HARGS+3
   JSR   mul16
   LDA   HARGS+4
   LDX   HARGS+5
   RTS
.test_binary@asm_ssa_split@0:
   LDA   __zpabi_test_binary__arg2_0
   STA   __local_test_binary__arg2_0
   LDA   __zpabi_test_binary__arg2_1
   STA   __local_test_binary__arg2_1
   JMP   .if_end@8

test_unary:
   SUBROUTINE

   STX   __local_test_unary__arg_0
   ORA   __local_test_unary__arg_0
   BEQ   .test_unary@asm_ssa_split@0
   LDA   #$05
   STA   __local_test_unary__arg_0
   LDA   #$00
   STA   __zpabi_test_unary__arg_1
.if_end@9:
   SEC
   LDA   #$00
   SBC   __local_test_unary__arg_0
   STA   __local_test_unary__arg_0
   LDA   #$00
   SBC   __zpabi_test_unary__arg_1
   TAX
   LDA   __local_test_unary__arg_0
   RTS
.test_unary@asm_ssa_split@0:
   LDA   __zpabi_test_unary__arg_0
   STA   __local_test_unary__arg_0
   JMP   .if_end@9

f:
   SUBROUTINE

   STA   __local_f__arg
   INC   __local_f__arg
   BNE   .inc_done@0
   INX
.inc_done@0:
   LDA   __local_f__arg
   RTS

test_funcall:
   SUBROUTINE

   STX   __local_test_funcall__arg
   ORA   __local_test_funcall__arg
   BEQ   .test_funcall@asm_ssa_split@0
   LDA   #$07
   LDX   #$00
.if_end@10:
   JMP   f
.test_funcall@asm_ssa_split@0:
   LDA   __zpabi_test_funcall__arg_0
   LDX   __zpabi_test_funcall__arg_1
   JMP   .if_end@10

main:
   SUBROUTINE

   JSR   main@run1
   LDA   #$01
   JSR   test_jz
   STA   __local_main__1
   TXA
   BNE   .jcmp_skip@0
   LDA   __local_main__1
   CMP   #$02
   BEQ   .if_end@11
.jcmp_skip@0:
   LDX   #$00
   LDA   #$01
   RTS
.if_end@11:
   JSR   main@run1
   JSR   test_jz
   STA   __local_main__1
   TXA
   BNE   .jcmp_skip@1
   LDA   __local_main__1
   CMP   #$01
   BEQ   .if_end@12
.jcmp_skip@1:
   LDX   #$00
   LDA   #$02
   RTS
.if_end@12:
   JSR   main@run2
   LDA   #$01
   JSR   test_jnz
   STX   __local_main__0
   ORA   __local_main__0
   BEQ   .if_end@13
   LDX   #$00
   LDA   #$03
   RTS
.if_end@13:
   JSR   main@run2
   JSR   test_jnz
   STA   __local_main__1
   TXA
   BNE   .jcmp_skip@2
   LDA   __local_main__1
   CMP   #$01
   BEQ   .if_end@14
.jcmp_skip@2:
   LDX   #$00
   LDA   #$04
   RTS
.if_end@14:
   JSR   main@run0
   JSR   test_binary
   STA   __local_main__1
   TXA
   BNE   .jcmp_skip@3
   LDA   __local_main__1
   CMP   #$24
   BEQ   .if_end@15
.jcmp_skip@3:
   LDX   #$00
   LDA   #$05
   RTS
.if_end@15:
   JSR   main@run0
   LDA   #$01
   JSR   test_binary
   STA   __local_main__1
   TXA
   BNE   .jcmp_skip@4
   LDA   __local_main__1
   CMP   #$18
   BEQ   .if_end@16
.jcmp_skip@4:
   LDX   #$00
   LDA   #$06
   RTS
.if_end@16:
   JSR   main@run0
   LDA   #$02
   JSR   test_binary
   STA   __local_main__1
   TXA
   BNE   .jcmp_skip@5
   LDA   __local_main__1
   CMP   #$48
   BEQ   .if_end@17
.jcmp_skip@5:
   LDX   #$00
   LDA   #$07
   RTS
.if_end@17:
   JSR   main@run3
   JSR   test_unary
   CPX   #$FF
   BNE   .jcmp_skip@6
   CMP   #$F8
   BEQ   .if_end@18
.jcmp_skip@6:
   LDX   #$00
   LDA   #$08
   RTS
.if_end@18:
   JSR   main@run3
   LDA   #$01
   JSR   test_unary
   CPX   #$FF
   BNE   .jcmp_skip@7
   CMP   #$FB
   BEQ   .if_end@19
.jcmp_skip@7:
   LDX   #$00
   LDA   #$09
   RTS
.if_end@19:
   LDA   #$05
   STA   __zpabi_test_funcall__arg_0
   LDA   #$00
   STA   __zpabi_test_funcall__arg_1
   LDX   #$00
   LDA   #$01
   JSR   test_funcall
   STA   __local_main__1
   TXA
   BNE   .jcmp_skip@8
   LDA   __local_main__1
   CMP   #$08
   BEQ   .if_end@20
.jcmp_skip@8:
   LDX   #$00
   LDA   #$0A
   RTS
.if_end@20:
   LDA   #$09
   STA   __zpabi_test_funcall__arg_0
   LDA   #$00
   STA   __zpabi_test_funcall__arg_1
   LDX   #$00
   JSR   test_funcall
   STA   __local_main__1
   TXA
   BNE   .jcmp_skip@9
   LDA   __local_main__1
   CMP   #$0A
   BEQ   .if_end@21
.jcmp_skip@9:
   LDX   #$00
   LDA   #$0B
   RTS
.if_end@21:
   LDX   #$00
   TXA
   RTS

main@run3:
   SUBROUTINE

   LDA   #$08
   STA   __zpabi_test_unary__arg_0
   LDA   #$00
   STA   __zpabi_test_unary__arg_1
   LDX   #$00
   RTS

main@run2:
   SUBROUTINE

   LDA   #$01
   STA   __zpabi_test_jnz__arg_0
   LDA   #$00
   STA   __zpabi_test_jnz__arg_1
   LDX   #$00
   RTS

main@run1:
   SUBROUTINE

   LDA   #$01
   STA   __zpabi_test_jz__arg_0
   LDA   #$00
   STA   __zpabi_test_jz__arg_1
   LDX   #$00
   RTS

main@run0:
   SUBROUTINE

   LDA   #$08
   STA   __zpabi_test_binary__arg1_0
   LDA   #$09
   STA   __zpabi_test_binary__arg2_0
   LDA   #$00
   STA   __zpabi_test_binary__arg1_1
   STA   __zpabi_test_binary__arg2_1
   LDX   #$00
   RTS
