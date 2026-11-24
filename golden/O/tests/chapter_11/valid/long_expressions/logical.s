SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__zpabi_and__l1_0	EQU	$80
__zpabi_if_cond__l_0	EQU	$80
__zpabi_not__l_0	EQU	$80
__zpabi_or__l2_0	EQU	$80
__zpabi_and__l1_1	EQU	$81
__zpabi_if_cond__l_1	EQU	$81
__zpabi_not__l_1	EQU	$81
__zpabi_or__l2_1	EQU	$81
__zpabi_and__l1_2	EQU	$82
__zpabi_if_cond__l_2	EQU	$82
__zpabi_not__l_2	EQU	$82
__zpabi_or__l2_2	EQU	$82
__zpabi_and__l1_3	EQU	$83
__zpabi_if_cond__l_3	EQU	$83
__zpabi_not__l_3	EQU	$83
__zpabi_or__l2_3	EQU	$83
__zpabi_and__l1_4	EQU	$84
__zpabi_if_cond__l_4	EQU	$84
__zpabi_not__l_4	EQU	$84
__zpabi_or__l2_4	EQU	$84
__zpabi_and__l1_5	EQU	$85
__zpabi_if_cond__l_5	EQU	$85
__zpabi_not__l_5	EQU	$85
__zpabi_or__l2_5	EQU	$85
__zpabi_and__l1_6	EQU	$86
__zpabi_if_cond__l_6	EQU	$86
__zpabi_not__l_6	EQU	$86
__zpabi_or__l2_6	EQU	$86
__zpabi_and__l1_7	EQU	$87
__zpabi_if_cond__l_7	EQU	$87
__zpabi_not__l_7	EQU	$87
__zpabi_or__l2_7	EQU	$87
__local_or__l1_1	EQU	$88
__zpabi_and__l2_0	EQU	$88
__zpabi_and__l2_1	EQU	$89
__local_main__0	EQU	$8A

; @zp-link-meta-begin
; def and params=__zpabi_and__l1_0,__zpabi_and__l1_1,__zpabi_and__l1_2,__zpabi_and__l1_3,__zpabi_and__l1_4,__zpabi_and__l1_5,__zpabi_and__l1_6,__zpabi_and__l1_7,__zpabi_and__l2_0,__zpabi_and__l2_1 locals=__local_and__0 indirect=false in_cycle=false
; def if_cond params=__zpabi_if_cond__l_0,__zpabi_if_cond__l_1,__zpabi_if_cond__l_2,__zpabi_if_cond__l_3,__zpabi_if_cond__l_4,__zpabi_if_cond__l_5,__zpabi_if_cond__l_6,__zpabi_if_cond__l_7 locals= indirect=false in_cycle=false
; def main params= locals=__local_main__0,__local_main__1 indirect=false in_cycle=false
; def not params=__zpabi_not__l_0,__zpabi_not__l_1,__zpabi_not__l_2,__zpabi_not__l_3,__zpabi_not__l_4,__zpabi_not__l_5,__zpabi_not__l_6,__zpabi_not__l_7 locals=__local_not__0 indirect=false in_cycle=false
; def or params=__zpabi_or__l1_0,__zpabi_or__l1_1,__zpabi_or__l2_0,__zpabi_or__l2_1,__zpabi_or__l2_2,__zpabi_or__l2_3,__zpabi_or__l2_4,__zpabi_or__l2_5,__zpabi_or__l2_6,__zpabi_or__l2_7 locals=__local_or__l1_1,__local_or__l1_0 indirect=false in_cycle=false param_regs=A,X,,,,,,,,
; call main -> and
; call main -> if_cond
; call main -> not
; call main -> or
; item not section=.text local=false zero=false
; item if_cond section=.text local=false zero=false
; item and section=.text local=false zero=false
; item or section=.text local=false zero=false
; item main section=.text local=false zero=false
; item main@run1 section=.text local=true zero=false
; item main@run0 section=.text local=true zero=false
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

not:
   SUBROUTINE

   LDA   __zpabi_not__l_0
   ORA   __zpabi_not__l_1
   ORA   __zpabi_not__l_2
   ORA   __zpabi_not__l_3
   ORA   __zpabi_not__l_4
   ORA   __zpabi_not__l_5
   ORA   __zpabi_not__l_6
   ORA   __zpabi_not__l_7
   BEQ   .lnot_true@0
   LDA   #$00
   BEQ   .lnot_end@1
.lnot_true@0:
   LDA   #$01
.lnot_end@1:
   LDX   #$00
   RTS

if_cond:
   SUBROUTINE

   LDA   __zpabi_if_cond__l_0
   ORA   __zpabi_if_cond__l_1
   ORA   __zpabi_if_cond__l_2
   ORA   __zpabi_if_cond__l_3
   ORA   __zpabi_if_cond__l_4
   ORA   __zpabi_if_cond__l_5
   ORA   __zpabi_if_cond__l_6
   ORA   __zpabi_if_cond__l_7
   BEQ   .if_end@0
   LDX   #$00
   LDA   #$01
   RTS
.if_end@0:
   LDX   #$00
   TXA
   RTS

and:
   SUBROUTINE

   LDA   __zpabi_and__l1_0
   ORA   __zpabi_and__l1_1
   ORA   __zpabi_and__l1_2
   ORA   __zpabi_and__l1_3
   ORA   __zpabi_and__l1_4
   ORA   __zpabi_and__l1_5
   ORA   __zpabi_and__l1_6
   ORA   __zpabi_and__l1_7
   BEQ   .and_false@1
   LDA   __zpabi_and__l2_0
   ORA   __zpabi_and__l2_1
   BEQ   .and_false@1
   LDA   #$01
   LDX   #$00
   RTS
.and_false@1:
   LDA   #$00
   LDX   #$00
   RTS

or:
   SUBROUTINE

   STX   __local_or__l1_1
   ORA   __local_or__l1_1
   BNE   .or_true@3
   LDA   __zpabi_or__l2_0
   ORA   __zpabi_or__l2_1
   ORA   __zpabi_or__l2_2
   ORA   __zpabi_or__l2_3
   ORA   __zpabi_or__l2_4
   ORA   __zpabi_or__l2_5
   ORA   __zpabi_or__l2_6
   ORA   __zpabi_or__l2_7
   BNE   .or_true@3
   LDA   #$00
   LDX   #$00
   RTS
.or_true@3:
   LDA   #$01
   LDX   #$00
   RTS

main:
   SUBROUTINE

   LDA   #$40
   STA   __zpabi_not__l_3
   LDA   #$00
   STA   __zpabi_not__l_0
   STA   __zpabi_not__l_1
   STA   __zpabi_not__l_2
   JSR   main@run0
   STX   __local_main__0
   ORA   __local_main__0
   BEQ   .if_end@5
   LDX   #$00
   LDA   #$01
   RTS
.if_end@5:
   LDA   #$00
   STA   __zpabi_not__l_0
   STA   __zpabi_not__l_1
   STA   __zpabi_not__l_2
   STA   __zpabi_not__l_3
   JSR   main@run0
   STX   __local_main__0
   ORA   __local_main__0
   BNE   .if_end@6
   LDX   #$00
   LDA   #$02
   RTS
.if_end@6:
   LDA   #$40
   STA   __zpabi_if_cond__l_3
   LDA   #$00
   STA   __zpabi_if_cond__l_0
   STA   __zpabi_if_cond__l_1
   STA   __zpabi_if_cond__l_2
   JSR   main@run1
   STX   __local_main__0
   ORA   __local_main__0
   BNE   .if_end@7
   LDX   #$00
   LDA   #$03
   RTS
.if_end@7:
   LDA   #$00
   STA   __zpabi_if_cond__l_0
   STA   __zpabi_if_cond__l_1
   STA   __zpabi_if_cond__l_2
   STA   __zpabi_if_cond__l_3
   JSR   main@run1
   STX   __local_main__0
   ORA   __local_main__0
   BEQ   .if_end@8
   LDX   #$00
   LDA   #$04
   RTS
.if_end@8:
   LDA   #$01
   STA   __zpabi_and__l2_0
   LDA   #$00
   STA   __zpabi_and__l1_0
   STA   __zpabi_and__l1_1
   STA   __zpabi_and__l1_2
   STA   __zpabi_and__l1_3
   STA   __zpabi_and__l1_4
   STA   __zpabi_and__l1_5
   STA   __zpabi_and__l1_6
   STA   __zpabi_and__l1_7
   STA   __zpabi_and__l2_1
   JSR   and
   STX   __local_main__0
   ORA   __local_main__0
   BEQ   .if_end@9
   LDX   #$00
   LDA   #$05
   RTS
.if_end@9:
   LDA   #$40
   STA   __zpabi_or__l2_3
   LDA   #$00
   STA   __zpabi_or__l2_0
   STA   __zpabi_or__l2_1
   STA   __zpabi_or__l2_2
   STA   __zpabi_or__l2_4
   STA   __zpabi_or__l2_5
   STA   __zpabi_or__l2_6
   STA   __zpabi_or__l2_7
   LDX   #$00
   LDA   #$01
   JSR   or
   STX   __local_main__0
   ORA   __local_main__0
   BNE   .if_end@10
   LDX   #$00
   LDA   #$06
   RTS
.if_end@10:
   LDX   #$00
   TXA
   RTS

main@run1:
   SUBROUTINE

   STA   __zpabi_if_cond__l_4
   STA   __zpabi_if_cond__l_5
   STA   __zpabi_if_cond__l_6
   STA   __zpabi_if_cond__l_7
   JMP   if_cond

main@run0:
   SUBROUTINE

   STA   __zpabi_not__l_4
   STA   __zpabi_not__l_5
   STA   __zpabi_not__l_6
   STA   __zpabi_not__l_7
   JMP   not
