SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__local_main__0	EQU	$80
__local_test_same_name_fun_and_tag__x_0	EQU	$83
__local_test_shared_member_names__3	EQU	$86
__local_test_shared_member_names__4	EQU	$87
__local_test_shared_member_names__5	EQU	$88
__local_test_shared_member_names__6	EQU	$89

; @zp-link-meta-begin
; def f params= locals= indirect=false in_cycle=false
; def main params= locals=__local_main__0,__local_main__1 indirect=false in_cycle=false
; def test_same_name_fun_and_tag params= locals=__local_test_same_name_fun_and_tag__x_1,__local_test_same_name_fun_and_tag__x_0 indirect=false in_cycle=false
; def test_same_name_var_member_and_tag params= locals=__local_test_same_name_var_member_and_tag__u_1,__local_test_same_name_var_member_and_tag__u_0 indirect=false in_cycle=false
; def test_shared_member_names params= locals=__local_test_shared_member_names__var2_0,__local_test_shared_member_names__0,__local_test_shared_member_names__1,__local_test_shared_member_names__2,__local_test_shared_member_names__3,__local_test_shared_member_names__4,__local_test_shared_member_names__5,__local_test_shared_member_names__6,__local_test_shared_member_names__var3,__local_test_shared_member_names__var2_3,__local_test_shared_member_names__var2_2,__local_test_shared_member_names__var2_1,__local_test_shared_member_names__7,__local_test_shared_member_names__var1_1,__local_test_shared_member_names__var1_0 indirect=false in_cycle=false
; call main -> test_same_name_fun_and_tag
; call main -> test_same_name_var_member_and_tag
; call main -> test_shared_member_names
; call test_same_name_fun_and_tag -> f
; item test_shared_member_names section=.text local=false zero=false
; item test_shared_member_names@tail0 section=.text local=true zero=false
; item test_same_name_var_member_and_tag section=.text local=false zero=false
; item f section=.text local=false zero=false
; item test_same_name_fun_and_tag section=.text local=false zero=false
; item main section=.text local=false zero=false
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

test_shared_member_names:
   SUBROUTINE

   LDY   #$00
   BNE   .test_shared_member_names@jthread@1
   LDA   #$0A
   CMP   #$0A
   BNE   .test_shared_member_names@jthread@1
   LDA   __local_test_shared_member_names__6
   CMP   #$80
   BNE   .test_shared_member_names@jthread@1
   LDA   __local_test_shared_member_names__5
   BNE   .test_shared_member_names@jthread@1
   LDA   __local_test_shared_member_names__4
   BNE   .test_shared_member_names@jthread@1
   LDA   __local_test_shared_member_names__3
   BNE   .test_shared_member_names@jthread@1
   LDA   #$80
   BNE   .test_shared_member_names@jthread@1
   TYA
   BNE   .test_shared_member_names@jthread@1
   CMP   #$FF
   BNE   .test_shared_member_names@jthread@1
   LDA   #$FF
   CMP   #$FF
   BEQ   .if_end@4
.test_shared_member_names@jthread@1:
   LDX   #$00
   TXA
   RTS
.if_end@4:
   JMP   test_shared_member_names@tail0

test_shared_member_names@tail0:
   SUBROUTINE

   LDX   #$00
   LDA   #$01
   RTS

test_same_name_var_member_and_tag:
   SUBROUTINE

   LDA   #$00
   BNE   .jcmp_skip@0
   LDA   #$64
   CMP   #$64
   BEQ   .if_end@5
.jcmp_skip@0:
   LDX   #$00
   TXA
   RTS
.if_end@5:
   JMP   test_shared_member_names@tail0

f:
   SUBROUTINE

   LDX   #$00
   LDA   #$0A
   RTS

test_same_name_fun_and_tag:
   SUBROUTINE

   JSR   f
   STA   __local_test_same_name_fun_and_tag__x_0
   TXA
   BNE   .jcmp_skip@1
   LDA   __local_test_same_name_fun_and_tag__x_0
   CMP   #$0A
   BEQ   .if_end@6
.jcmp_skip@1:
   LDX   #$00
   TXA
   RTS
.if_end@6:
   JMP   test_shared_member_names@tail0

main:
   SUBROUTINE

   JSR   test_shared_member_names
   STX   __local_main__0
   ORA   __local_main__0
   BNE   .if_end@7
   JMP   test_shared_member_names@tail0
.if_end@7:
   JSR   test_same_name_var_member_and_tag
   STX   __local_main__0
   ORA   __local_main__0
   BNE   .if_end@8
   LDX   #$00
   LDA   #$02
   RTS
.if_end@8:
   JSR   test_same_name_fun_and_tag
   STX   __local_main__0
   ORA   __local_main__0
   BNE   .if_end@9
   LDX   #$00
   LDA   #$03
   RTS
.if_end@9:
   LDX   #$00
   TXA
   RTS
