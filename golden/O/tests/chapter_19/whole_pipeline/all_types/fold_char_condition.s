SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__local_main__0	EQU	$80
__local_main__1	EQU	$81

; @zp-link-meta-begin
; def main params= locals=__local_main__0,__local_main__1 indirect=false in_cycle=false
; def target_and_schar params= locals= indirect=false in_cycle=false
; def target_and_true_char params= locals= indirect=false in_cycle=false
; def target_branch_char params= locals= indirect=false in_cycle=false
; def target_not_char params= locals= indirect=false in_cycle=false
; def target_not_true_char params= locals= indirect=false in_cycle=false
; def target_not_uchar params= locals= indirect=false in_cycle=false
; def target_or_char params= locals= indirect=false in_cycle=false
; def target_or_uchar params= locals= indirect=false in_cycle=false
; ext putchar params=__zpabi_putchar__c_0,__zpabi_putchar__c_1 param_regs=A,X
; call main -> target_and_schar
; call main -> target_and_true_char
; call main -> target_branch_char
; call main -> target_not_char
; call main -> target_not_true_char
; call main -> target_not_uchar
; call main -> target_or_char
; call main -> target_or_uchar
; item target_not_char section=.text local=false zero=false
; item target_not_char@tail0 section=.text local=true zero=false
; item target_not_uchar section=.text local=false zero=false
; item target_not_true_char section=.text local=false zero=false
; item target_and_schar section=.text local=false zero=false
; item target_and_true_char section=.text local=false zero=false
; item target_or_uchar section=.text local=false zero=false
; item target_or_char section=.text local=false zero=false
; item target_branch_char section=.text local=false zero=false
; item main section=.text local=false zero=false
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

target_not_char:
   SUBROUTINE

   JMP   target_not_char@tail0

target_not_char@tail0:
   SUBROUTINE

   LDX   #$00
   LDA   #$01
   RTS

target_not_uchar:
   SUBROUTINE

   JMP   target_not_char@tail0

target_not_true_char:
   SUBROUTINE

   LDX   #$00
   TXA
   RTS

target_and_schar:
   SUBROUTINE

   LDX   #$00
   TXA
   RTS

target_and_true_char:
   SUBROUTINE

   JMP   target_not_char@tail0

target_or_uchar:
   SUBROUTINE

   JMP   target_not_char@tail0

target_or_char:
   SUBROUTINE

   JMP   target_not_char@tail0

target_branch_char:
   SUBROUTINE

   LDA   #$0A
   RTS

main:
   SUBROUTINE

   JSR   target_not_char
   STA   __local_main__1
   TXA
   BNE   .jcmp_skip@0
   LDA   __local_main__1
   CMP   #$01
   BEQ   .if_end@9
.jcmp_skip@0:
   JMP   target_not_char@tail0
.if_end@9:
   JSR   target_not_uchar
   STA   __local_main__1
   TXA
   BNE   .jcmp_skip@1
   LDA   __local_main__1
   CMP   #$01
   BEQ   .if_end@10
.jcmp_skip@1:
   LDX   #$00
   LDA   #$02
   RTS
.if_end@10:
   JSR   target_not_true_char
   STX   __local_main__0
   ORA   __local_main__0
   BEQ   .if_end@11
   LDX   #$00
   LDA   #$03
   RTS
.if_end@11:
   JSR   target_and_schar
   STX   __local_main__0
   ORA   __local_main__0
   BEQ   .if_end@12
   LDX   #$00
   LDA   #$04
   RTS
.if_end@12:
   JSR   target_and_true_char
   STA   __local_main__1
   TXA
   BNE   .jcmp_skip@2
   LDA   __local_main__1
   CMP   #$01
   BEQ   .if_end@13
.jcmp_skip@2:
   LDX   #$00
   LDA   #$05
   RTS
.if_end@13:
   JSR   target_or_uchar
   STA   __local_main__1
   TXA
   BNE   .jcmp_skip@3
   LDA   __local_main__1
   CMP   #$01
   BEQ   .if_end@14
.jcmp_skip@3:
   LDX   #$00
   LDA   #$06
   RTS
.if_end@14:
   JSR   target_or_char
   STA   __local_main__1
   TXA
   BNE   .jcmp_skip@4
   LDA   __local_main__1
   CMP   #$01
   BEQ   .if_end@15
.jcmp_skip@4:
   LDX   #$00
   LDA   #$07
   RTS
.if_end@15:
   JSR   target_branch_char
   CMP   #$0A
   BEQ   .if_end@16
   LDX   #$00
   LDA   #$08
   RTS
.if_end@16:
   LDX   #$00
   TXA
   RTS
