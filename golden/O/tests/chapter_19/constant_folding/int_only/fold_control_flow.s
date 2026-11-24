SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__local_main__0	EQU	$80
__local_main__1	EQU	$81

; @zp-link-meta-begin
; def main params= locals=__local_main__0,__local_main__1 indirect=false in_cycle=false
; def target_conditional_false params= locals= indirect=false in_cycle=false
; def target_conditional_true params= locals= indirect=false in_cycle=false
; def target_do_loop params= locals= indirect=false in_cycle=false
; def target_for_loop_false params= locals= indirect=false in_cycle=false
; def target_for_loop_true params= locals= indirect=false in_cycle=false
; def target_if params= locals= indirect=false in_cycle=false
; def target_if_else_false params= locals= indirect=false in_cycle=false
; def target_if_else_true params= locals= indirect=false in_cycle=false
; def target_while_loop_false params= locals= indirect=false in_cycle=false
; def target_while_loop_true params= locals= indirect=false in_cycle=false
; call main -> target_conditional_false
; call main -> target_conditional_true
; call main -> target_do_loop
; call main -> target_for_loop_false
; call main -> target_for_loop_true
; call main -> target_if
; call main -> target_if_else_false
; call main -> target_if_else_true
; call main -> target_while_loop_false
; call main -> target_while_loop_true
; item target_if section=.text local=false zero=false
; item target_if_else_true section=.text local=false zero=false
; item target_if_else_true@tail0 section=.text local=true zero=false
; item target_if_else_false section=.text local=false zero=false
; item target_conditional_true section=.text local=false zero=false
; item target_conditional_false section=.text local=false zero=false
; item target_do_loop section=.text local=false zero=false
; item target_do_loop@tail1 section=.text local=true zero=false
; item target_while_loop_false section=.text local=false zero=false
; item target_while_loop_true section=.text local=false zero=false
; item target_for_loop_true section=.text local=false zero=false
; item target_for_loop_false section=.text local=false zero=false
; item main section=.text local=false zero=false
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

target_if:
   SUBROUTINE

   LDX   #$00
   TXA
   RTS

target_if_else_true:
   SUBROUTINE

   JMP   target_if_else_true@tail0

target_if_else_true@tail0:
   SUBROUTINE

   LDX   #$00
   LDA   #$02
   RTS

target_if_else_false:
   SUBROUTINE

   LDX   #$00
   LDA   #$03
   RTS

target_conditional_true:
   SUBROUTINE

   JMP   target_if_else_true@tail0

target_conditional_false:
   SUBROUTINE

   LDX   #$00
   LDA   #$05
   RTS

target_do_loop:
   SUBROUTINE

   JMP   target_do_loop@tail1

target_do_loop@tail1:
   SUBROUTINE

   LDX   #$00
   LDA   #$0A
   RTS

target_while_loop_false:
   SUBROUTINE

   LDX   #$00
   TXA
   RTS

target_while_loop_true:
   SUBROUTINE

   JMP   target_do_loop@tail1

target_for_loop_true:
   SUBROUTINE

   LDX   #$00
   LDA   #$64
   RTS

target_for_loop_false:
   SUBROUTINE

   LDX   #$00
   TXA
   RTS

main:
   SUBROUTINE

   JSR   target_if
   STX   __local_main__0
   ORA   __local_main__0
   BEQ   .if_end@9
   LDX   #$00
   LDA   #$01
   RTS
.if_end@9:
   JSR   target_if_else_true
   STA   __local_main__1
   TXA
   BNE   .jcmp_skip@0
   LDA   __local_main__1
   CMP   #$02
   BEQ   .if_end@10
.jcmp_skip@0:
   JMP   target_if_else_true@tail0
.if_end@10:
   JSR   target_if_else_false
   STA   __local_main__1
   TXA
   BNE   .jcmp_skip@1
   LDA   __local_main__1
   CMP   #$03
   BEQ   .if_end@11
.jcmp_skip@1:
   LDX   #$00
   LDA   #$03
   RTS
.if_end@11:
   JSR   target_conditional_true
   STA   __local_main__1
   TXA
   BNE   .jcmp_skip@2
   LDA   __local_main__1
   CMP   #$02
   BEQ   .if_end@12
.jcmp_skip@2:
   LDX   #$00
   LDA   #$04
   RTS
.if_end@12:
   JSR   target_conditional_false
   STA   __local_main__1
   TXA
   BNE   .jcmp_skip@3
   LDA   __local_main__1
   CMP   #$05
   BEQ   .if_end@13
.jcmp_skip@3:
   LDX   #$00
   LDA   #$05
   RTS
.if_end@13:
   JSR   target_do_loop
   STA   __local_main__1
   TXA
   BNE   .jcmp_skip@4
   LDA   __local_main__1
   CMP   #$0A
   BEQ   .if_end@14
.jcmp_skip@4:
   LDX   #$00
   LDA   #$06
   RTS
.if_end@14:
   JSR   target_while_loop_false
   STX   __local_main__0
   ORA   __local_main__0
   BEQ   .if_end@15
   LDX   #$00
   LDA   #$07
   RTS
.if_end@15:
   JSR   target_while_loop_true
   STA   __local_main__1
   TXA
   BNE   .jcmp_skip@5
   LDA   __local_main__1
   CMP   #$0A
   BEQ   .if_end@16
.jcmp_skip@5:
   LDX   #$00
   LDA   #$08
   RTS
.if_end@16:
   JSR   target_for_loop_true
   STA   __local_main__1
   TXA
   BNE   .jcmp_skip@6
   LDA   __local_main__1
   CMP   #$64
   BEQ   .if_end@17
.jcmp_skip@6:
   LDX   #$00
   LDA   #$09
   RTS
.if_end@17:
   JSR   target_for_loop_false
   STX   __local_main__0
   ORA   __local_main__0
   BEQ   .if_end@18
   JMP   target_do_loop@tail1
.if_end@18:
   LDX   #$00
   TXA
   RTS
