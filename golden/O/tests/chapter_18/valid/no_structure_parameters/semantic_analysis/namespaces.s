SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__local_main__0	EQU	$80
__local_test_same_name_fun_member_and_tag__my_struct_0	EQU	$83

; @zp-link-meta-begin
; def f params= locals= indirect=false in_cycle=false
; def main params= locals=__local_main__0,__local_main__1 indirect=false in_cycle=false
; def test_same_name_fun_member_and_tag params= locals=__local_test_same_name_fun_member_and_tag__my_struct_1,__local_test_same_name_fun_member_and_tag__my_struct_0 indirect=false in_cycle=false
; def test_same_name_var_member_and_tag params= locals=__local_test_same_name_var_member_and_tag__x_1,__local_test_same_name_var_member_and_tag__x_0 indirect=false in_cycle=false
; def test_shared_member_names params= locals=__local_test_shared_member_names__p2_7,__local_test_shared_member_names__p2_6,__local_test_shared_member_names__p2_5,__local_test_shared_member_names__p2_4,__local_test_shared_member_names__p2_3,__local_test_shared_member_names__p2_2,__local_test_shared_member_names__p2_1,__local_test_shared_member_names__p2_0,__local_test_shared_member_names__p1_1,__local_test_shared_member_names__p1_0 indirect=false in_cycle=false
; def test_shared_nested_member_names params= locals=__local_test_shared_nested_member_names__0,__local_test_shared_nested_member_names__1,__local_test_shared_nested_member_names__p2_0,__local_test_shared_nested_member_names__p2_1,__local_test_shared_nested_member_names__p1,__local_test_shared_nested_member_names__3,__local_test_shared_nested_member_names__4,__local_test_shared_nested_member_names__5 indirect=false in_cycle=false
; call main -> test_same_name_fun_member_and_tag
; call main -> test_same_name_var_member_and_tag
; call main -> test_shared_member_names
; call main -> test_shared_nested_member_names
; call test_same_name_fun_member_and_tag -> f
; item test_shared_member_names section=.text local=false zero=false
; item test_shared_member_names@tail1 section=.text local=true zero=false
; item test_shared_member_names@tail0 section=.text local=true zero=false
; item test_shared_nested_member_names section=.text local=false zero=false
; item test_same_name_var_member_and_tag section=.text local=false zero=false
; item test_same_name_fun_member_and_tag section=.text local=false zero=false
; item f section=.text local=false zero=false
; item main section=.text local=false zero=false
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

test_shared_member_names:
   SUBROUTINE

   LDY   #$00
   BNE   .test_shared_member_names@jthread@0
   LDA   #$01
   CMP   #$01
   BNE   .test_shared_member_names@jthread@0
   LDA   #$40
   CMP   #$40
   BNE   .test_shared_member_names@jthread@0
   LDA   #$08
   CMP   #$08
   BNE   .test_shared_member_names@jthread@0
   TYA
   BNE   .test_shared_member_names@jthread@0
   BEQ   .if_end@2
.test_shared_member_names@jthread@0:
   JMP   test_shared_member_names@tail1
.if_end@2:
   JMP   test_shared_member_names@tail0

test_shared_member_names@tail1:
   SUBROUTINE

   LDX   #$00
   TXA
   RTS

test_shared_member_names@tail0:
   SUBROUTINE

   LDX   #$00
   LDA   #$01
   RTS

test_shared_nested_member_names:
   SUBROUTINE

   LDA   #$00
   BNE   .jcmp_skip@0
   LDA   #$03
   CMP   #$03
   BEQ   .if_end@3
.jcmp_skip@0:
   JMP   test_shared_member_names@tail1
.if_end@3:
   JMP   test_shared_member_names@tail0

test_same_name_var_member_and_tag:
   SUBROUTINE

   LDA   #$00
   BNE   .jcmp_skip@1
   LDA   #$0A
   CMP   #$0A
   BEQ   .if_end@4
.jcmp_skip@1:
   JMP   test_shared_member_names@tail1
.if_end@4:
   JMP   test_shared_member_names@tail0

test_same_name_fun_member_and_tag:
   SUBROUTINE

   JSR   f
   STA   __local_test_same_name_fun_member_and_tag__my_struct_0
   TXA
   BNE   .jcmp_skip@2
   LDA   __local_test_same_name_fun_member_and_tag__my_struct_0
   CMP   #$0A
   BEQ   .if_end@5
.jcmp_skip@2:
   JMP   test_shared_member_names@tail1
.if_end@5:
   JMP   test_shared_member_names@tail0

f:
   SUBROUTINE

   LDX   #$00
   LDA   #$0A
   RTS

main:
   SUBROUTINE

   JSR   test_shared_member_names
   STX   __local_main__0
   ORA   __local_main__0
   BNE   .if_end@6
   JMP   test_shared_member_names@tail0
.if_end@6:
   JSR   test_shared_nested_member_names
   STX   __local_main__0
   ORA   __local_main__0
   BNE   .if_end@7
   LDX   #$00
   LDA   #$02
   RTS
.if_end@7:
   JSR   test_same_name_var_member_and_tag
   STX   __local_main__0
   ORA   __local_main__0
   BNE   .if_end@8
   LDX   #$00
   LDA   #$03
   RTS
.if_end@8:
   JSR   test_same_name_fun_member_and_tag
   STX   __local_main__0
   ORA   __local_main__0
   BNE   .if_end@9
   LDX   #$00
   LDA   #$04
   RTS
.if_end@9:
   JMP   test_shared_member_names@tail1
