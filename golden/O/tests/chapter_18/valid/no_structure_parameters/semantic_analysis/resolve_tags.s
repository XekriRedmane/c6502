SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__zpabi_calloc__nmemb_0	EQU	$80
__zpabi_malloc__size_0	EQU	$80
__local_test_member_declaration__1	EQU	$81
__zpabi_calloc__nmemb_1	EQU	$81
__zpabi_malloc__size_1	EQU	$81
__local_test_member_declaration__2	EQU	$82
__zpabi_calloc__nmemb_2	EQU	$82
__zpabi_malloc__size_2	EQU	$82
__local_test_member_declaration__3	EQU	$83
__zpabi_calloc__nmemb_3	EQU	$83
__zpabi_malloc__size_3	EQU	$83
__local_test_cast__0	EQU	$84
__local_test_member_declaration__my_struct	EQU	$84
__zpabi_calloc__size_0	EQU	$84
__local_test_cast__1	EQU	$85
__zpabi_calloc__size_1	EQU	$85
__zpabi_calloc__size_2	EQU	$86
__zpabi_calloc__size_3	EQU	$87
__local_main__0	EQU	$88
__local_test_for_loop__loop_struct_1	EQU	$8A
__local_test_function_declaration__0	EQU	$8A
__local_test_derived_types__1	EQU	$8B
__local_test_for_loop__loop_struct_0	EQU	$8B
__local_test_function_declaration__1	EQU	$8B
__local_test_derived_types__2	EQU	$8C
__local_test_derived_types__3	EQU	$8D
__local_test_for_loop__1	EQU	$8D
__local_test_function_declaration__3	EQU	$8D
__local_test_derived_types__4	EQU	$8E
__local_test_for_loop__2	EQU	$8E
__local_test_function_declaration__outer_struct	EQU	$8E
__local_test_derived_types__5	EQU	$8F
__local_test_for_loop__3	EQU	$8F
__local_copy_struct__0	EQU	$90
__local_test_derived_types__6	EQU	$90
__local_test_for_loop__4	EQU	$90
__local_test_derived_types__7	EQU	$91
__local_test_for_loop__5	EQU	$91
__local_copy_struct__2	EQU	$92
__local_test_derived_types__outer_struct	EQU	$92
__local_test_for_loop__6	EQU	$92
__local_copy_struct__3	EQU	$93
__local_test_for_loop__7	EQU	$93
__local_copy_struct__arg	EQU	$94
__local_test_derived_types__inner_struct	EQU	$94
__local_copy_struct__4	EQU	$95

; @zp-link-meta-begin
; def copy_struct params=__zpabi_copy_struct__arg_0,__zpabi_copy_struct__arg_1 locals=__local_copy_struct__0,__local_copy_struct__1,__local_copy_struct__2,__local_copy_struct__3,__local_copy_struct__arg,__local_copy_struct__4 indirect=false in_cycle=false param_regs=A,X
; def main params= locals=__local_main__0,__local_main__1 indirect=false in_cycle=false
; def test_cast params= locals=__local_test_cast__0,__local_test_cast__1 indirect=false in_cycle=false
; def test_contentless_tag_noop params= locals=__local_test_contentless_tag_noop__var_3,__local_test_contentless_tag_noop__var_2,__local_test_contentless_tag_noop__var_1,__local_test_contentless_tag_noop__var_0 indirect=false in_cycle=false
; def test_derived_types params= locals=__local_test_derived_types__0,__local_test_derived_types__1,__local_test_derived_types__2,__local_test_derived_types__3,__local_test_derived_types__4,__local_test_derived_types__5,__local_test_derived_types__6,__local_test_derived_types__7,__local_test_derived_types__outer_struct,__local_test_derived_types__9,__local_test_derived_types__inner_struct,__local_test_derived_types__11 indirect=false in_cycle=false
; def test_for_loop params= locals=__local_test_for_loop__loop_struct_1,__local_test_for_loop__loop_struct_0,__local_test_for_loop__0,__local_test_for_loop__1,__local_test_for_loop__2,__local_test_for_loop__3,__local_test_for_loop__4,__local_test_for_loop__5,__local_test_for_loop__6,__local_test_for_loop__7 indirect=false in_cycle=false
; def test_function_declaration params= locals=__local_test_function_declaration__0,__local_test_function_declaration__1,__local_test_function_declaration__2,__local_test_function_declaration__3,__local_test_function_declaration__outer_struct,__local_test_function_declaration__5 indirect=false in_cycle=false
; def test_member_declaration params= locals=__local_test_member_declaration__0,__local_test_member_declaration__1,__local_test_member_declaration__2,__local_test_member_declaration__3,__local_test_member_declaration__my_struct,__local_test_member_declaration__5,__local_test_member_declaration__6,__local_test_member_declaration__7 indirect=false in_cycle=false
; def test_sizeof params= locals= indirect=false in_cycle=false
; def test_var_declaration params= locals=__local_test_var_declaration__inner_1,__local_test_var_declaration__inner_0,__local_test_var_declaration__outer_1,__local_test_var_declaration__outer_0 indirect=false in_cycle=false
; ext calloc params=__zpabi_calloc__nmemb_0,__zpabi_calloc__nmemb_1,__zpabi_calloc__nmemb_2,__zpabi_calloc__nmemb_3,__zpabi_calloc__size_0,__zpabi_calloc__size_1,__zpabi_calloc__size_2,__zpabi_calloc__size_3
; ext malloc params=__zpabi_malloc__size_0,__zpabi_malloc__size_1,__zpabi_malloc__size_2,__zpabi_malloc__size_3
; call copy_struct -> malloc
; call main -> test_cast
; call main -> test_contentless_tag_noop
; call main -> test_derived_types
; call main -> test_for_loop
; call main -> test_function_declaration
; call main -> test_member_declaration
; call main -> test_sizeof
; call main -> test_var_declaration
; call test_cast -> malloc
; call test_derived_types -> calloc
; call test_function_declaration -> copy_struct
; item test_var_declaration section=.text local=false zero=false
; item test_var_declaration@tail1 section=.text local=true zero=false
; item test_var_declaration@tail0 section=.text local=true zero=false
; item test_member_declaration section=.text local=false zero=false
; item test_function_declaration section=.text local=false zero=false
; item copy_struct section=.text local=false zero=false
; item copy_struct@run2 section=.text local=true zero=false
; item copy_struct@run1 section=.text local=true zero=false
; item test_for_loop section=.text local=false zero=false
; item test_cast section=.text local=false zero=false
; item test_sizeof section=.text local=false zero=false
; item test_derived_types section=.text local=false zero=false
; item test_derived_types@run0 section=.text local=true zero=false
; item test_contentless_tag_noop section=.text local=false zero=false
; item main section=.text local=false zero=false
; item @17.loop_body_struct section=.bss local=true zero=true
; runtime hargs_size=32 hargs_need=24
; @zp-link-meta-end

test_var_declaration:
   SUBROUTINE

   LDA   #$00
   LDY   #$00
   CMP   #$00
   BNE   .jcmp_skip@1
   LDA   #$02
   CMP   #$02
   BNE   .jcmp_skip@1
   TYA
   BNE   .jcmp_skip@1
   LDA   #$03
   CMP   #$03
   BEQ   .if_end@1
.jcmp_skip@1:
   JMP   test_var_declaration@tail1
.if_end@1:
   JMP   test_var_declaration@tail0

test_var_declaration@tail1:
   SUBROUTINE

   LDX   #$00
   TXA
   RTS

test_var_declaration@tail0:
   SUBROUTINE

   LDX   #$00
   LDA   #$01
   RTS

test_member_declaration:
   SUBROUTINE

   LDA   #$7B
   STA   __local_test_member_declaration__my_struct
   LDA   #$00
   STA   __local_test_member_declaration__my_struct+1
   LDA   #<__local_test_member_declaration__my_struct
   STA   __local_test_member_declaration__my_struct+2
   LDY   #>__local_test_member_declaration__my_struct
   STY   __local_test_member_declaration__my_struct+3
   STA   __local_test_member_declaration__2
   STY   __local_test_member_declaration__3
   LDY   #$00
   LDA   (__local_test_member_declaration__2),Y
   STA   __local_test_member_declaration__1
   INY
   LDA   (__local_test_member_declaration__2),Y
   BNE   .jcmp_skip@2
   LDA   __local_test_member_declaration__1
   CMP   #$7B
   BEQ   .if_end@2
.jcmp_skip@2:
   JMP   test_var_declaration@tail1
.if_end@2:
   JMP   test_var_declaration@tail0

test_function_declaration:
   SUBROUTINE

   LDA   #$01
   STA   __local_test_function_declaration__outer_struct
   LDA   #$00
   STA   __local_test_function_declaration__outer_struct+1
   LDX   #>__local_test_function_declaration__outer_struct
   LDA   #<__local_test_function_declaration__outer_struct
   JSR   copy_struct
   STA   __local_test_function_declaration__0
   STX   __local_test_function_declaration__1
   LDY   #$00
   LDA   (__local_test_function_declaration__0),Y
   STA   __local_test_function_declaration__3
   INY
   LDA   (__local_test_function_declaration__0),Y
   LDX   __local_test_function_declaration__outer_struct
   STX   __local_test_function_declaration__1
   CMP   __local_test_function_declaration__outer_struct+1
   BNE   .jcmp_skip@3
   LDA   __local_test_function_declaration__3
   CMP   __local_test_function_declaration__1
   BEQ   .if_end@3
.jcmp_skip@3:
   JMP   test_var_declaration@tail1
.if_end@3:
   JMP   test_var_declaration@tail0

copy_struct:
   SUBROUTINE

   STA   __local_copy_struct__arg
   STX   __local_copy_struct__4
   JSR   copy_struct@run2
   STA   __local_copy_struct__2
   STX   __local_copy_struct__3
   STA   DPTR
   LDY   #$01
   LDA   (__local_copy_struct__arg),Y
   STA   __local_copy_struct__0
   STX   DPTR+1
   DEY
   LDA   (__local_copy_struct__arg),Y
   STA   (__local_copy_struct__2),Y
   LDA   __local_copy_struct__0
   INY
   STA   (DPTR),Y
   LDA   __local_copy_struct__2
   RTS

copy_struct@run2:
   SUBROUTINE

   LDA   #$04
   STA   __zpabi_malloc__size_0
   JMP   copy_struct@run1

copy_struct@run1:
   SUBROUTINE

   LDA   #$00
   STA   __zpabi_malloc__size_1
   STA   __zpabi_malloc__size_2
   STA   __zpabi_malloc__size_3
   JMP   malloc

test_for_loop:
   SUBROUTINE

   LDA   #$0A
   STA   __local_test_for_loop__loop_struct_0
   LDA   #$00
   STA   __local_test_for_loop__loop_struct_1
.loop@0_start:
   BPL   .lb_skip@1
   JMP   .loop@0_break
.lb_skip@1:
   ORA   __local_test_for_loop__loop_struct_0
   BNE   .lb_skip@0
   JMP   .loop@0_break
.lb_skip@0:
   LDA   @17.loop_body_struct
   STA   HARGS
   LDA   @17.loop_body_struct+1
   STA   HARGS+1
   LDA   @17.loop_body_struct+2
   STA   HARGS+2
   LDA   @17.loop_body_struct+3
   STA   HARGS+3
   LDA   @17.loop_body_struct+4
   STA   HARGS+4
   LDA   @17.loop_body_struct+5
   STA   HARGS+5
   LDA   @17.loop_body_struct+6
   STA   HARGS+6
   LDA   @17.loop_body_struct+7
   STA   HARGS+7
   LDA   #$00
   STA   HARGS+8
   LDY   #$00
   STY   HARGS+9
   STY   HARGS+10
   STY   HARGS+11
   STY   HARGS+12
   STA   HARGS+13
   LDA   #$F0
   STA   HARGS+14
   LDA   #$3F
   STA   HARGS+15
   JSR   dadd
   LDA   HARGS+16
   STA   @17.loop_body_struct
   LDA   HARGS+17
   STA   @17.loop_body_struct+1
   LDA   HARGS+18
   STA   @17.loop_body_struct+2
   LDA   HARGS+19
   STA   @17.loop_body_struct+3
   LDA   HARGS+20
   STA   @17.loop_body_struct+4
   LDA   HARGS+21
   STA   @17.loop_body_struct+5
   LDA   HARGS+22
   STA   @17.loop_body_struct+6
   LDA   HARGS+23
   STA   @17.loop_body_struct+7
   LDA   __local_test_for_loop__loop_struct_1
   BNE   .if_end@4
   LDA   __local_test_for_loop__loop_struct_0
   CMP   #$01
   BNE   .vthread@0
   LDA   @17.loop_body_struct
   STA   __local_test_for_loop__7
   LDA   @17.loop_body_struct+1
   STA   __local_test_for_loop__6
   LDA   @17.loop_body_struct+2
   STA   __local_test_for_loop__5
   LDA   @17.loop_body_struct+3
   STA   __local_test_for_loop__4
   LDA   @17.loop_body_struct+4
   STA   __local_test_for_loop__3
   LDA   @17.loop_body_struct+5
   STA   __local_test_for_loop__2
   LDA   @17.loop_body_struct+6
   STA   __local_test_for_loop__1
   LDA   @17.loop_body_struct+7
   CMP   #$40
   BNE   .jcmp_skip@4
   LDA   __local_test_for_loop__1
   CMP   #$24
   BNE   .jcmp_skip@4
   LDA   __local_test_for_loop__2
   BNE   .jcmp_skip@4
   LDA   __local_test_for_loop__3
   BNE   .jcmp_skip@4
   LDA   __local_test_for_loop__4
   BNE   .jcmp_skip@4
   LDA   __local_test_for_loop__5
   BNE   .jcmp_skip@4
   LDA   __local_test_for_loop__6
   BNE   .jcmp_skip@4
   LDA   __local_test_for_loop__7
   BEQ   .if_end@5
.jcmp_skip@4:
   JMP   test_var_declaration@tail1
.if_end@5:
.if_end@4:
   LDA   __local_test_for_loop__loop_struct_0
.vthread@0:
   SEC
   SBC   #$01
   STA   __local_test_for_loop__loop_struct_0
   BCS   .carry_inc@0
   DEC   __local_test_for_loop__loop_struct_1
.carry_inc@0:
   LDA   __local_test_for_loop__loop_struct_1
   JMP   .loop@0_start
.loop@0_break:
   JMP   test_var_declaration@tail0

test_cast:
   SUBROUTINE

   LDA   #$0A
   STA   __zpabi_malloc__size_0
   JSR   copy_struct@run1
   STA   __local_test_cast__0
   STX   __local_test_cast__1
   ORA   __local_test_cast__1
   BEQ   .if_end@6
   LDA   #$0A
   LDY   #$02
   STA   (__local_test_cast__0),Y
   LDA   (__local_test_cast__0),Y
   CMP   #$0A
   BNE   .tail@0
.if_end@6:
   JSR   copy_struct@run2
   STA   __local_test_cast__0
   STX   __local_test_cast__1
   STA   DPTR
   STX   DPTR+1
   LDA   #$0A
   LDY   #$00
   STA   (__local_test_cast__0),Y
   LDA   #$00
   INY
   STA   (DPTR),Y
   DEY
   LDA   (__local_test_cast__0),Y
   CMP   #$0A
   BEQ   .if_end@8
.tail@0:
   JMP   test_var_declaration@tail1
.if_end@8:
   JMP   test_var_declaration@tail0

test_sizeof:
   SUBROUTINE

   JMP   test_var_declaration@tail1

test_derived_types:
   SUBROUTINE

   LDA   #$01
   STA   __local_test_derived_types__outer_struct
   LDA   #$00
   STA   __local_test_derived_types__outer_struct+1
   JSR   test_derived_types@run0
   STA   __local_test_derived_types__6
   STX   __local_test_derived_types__7
   LDA   #$02
   STA   __local_test_derived_types__inner_struct
   LDA   #$00
   STA   __local_test_derived_types__inner_struct+1
   JSR   test_derived_types@run0
   STA   __local_test_derived_types__4
   STX   __local_test_derived_types__5
   LDA   __local_test_derived_types__6
   STA   DPTR
   LDA   __local_test_derived_types__7
   STA   DPTR+1
   LDA   #<__local_test_derived_types__outer_struct
   LDY   #$00
   STA   (__local_test_derived_types__6),Y
   LDA   #>__local_test_derived_types__outer_struct
   INY
   STA   (DPTR),Y
   LDA   __local_test_derived_types__6
   STA   DPTR
   LDA   __local_test_derived_types__7
   STA   DPTR+1
   LDA   #<__local_test_derived_types__outer_struct
   INY
   STA   (__local_test_derived_types__6),Y
   LDA   #>__local_test_derived_types__outer_struct
   INY
   STA   (DPTR),Y
   LDA   __local_test_derived_types__4
   STA   DPTR
   STX   DPTR+1
   LDA   #<__local_test_derived_types__inner_struct
   LDY   #$00
   STA   (__local_test_derived_types__4),Y
   LDA   #>__local_test_derived_types__inner_struct
   INY
   STA   (DPTR),Y
   LDA   __local_test_derived_types__4
   STA   DPTR
   STX   DPTR+1
   LDA   #<__local_test_derived_types__inner_struct
   LDY   #$04
   STA   (__local_test_derived_types__4),Y
   LDA   #>__local_test_derived_types__inner_struct
   INY
   STA   (DPTR),Y
   LDY   #$00
   LDA   (__local_test_derived_types__6),Y
   STA   __local_test_derived_types__2
   INY
   LDA   (__local_test_derived_types__6),Y
   STA   __local_test_derived_types__3
   DEY
   LDA   (__local_test_derived_types__2),Y
   STA   __local_test_derived_types__1
   INY
   LDA   (__local_test_derived_types__2),Y
   BNE   .jcmp_skip@6
   LDA   __local_test_derived_types__1
   CMP   #$01
   BNE   .jcmp_skip@6
   DEY
   LDA   (__local_test_derived_types__4),Y
   STA   __local_test_derived_types__2
   INY
   LDA   (__local_test_derived_types__4),Y
   STA   __local_test_derived_types__3
   DEY
   LDA   (__local_test_derived_types__2),Y
   STA   __local_test_derived_types__1
   INY
   LDA   (__local_test_derived_types__2),Y
   BNE   .jcmp_skip@6
   LDA   __local_test_derived_types__1
   CMP   #$02
   BEQ   .if_end@13
.jcmp_skip@6:
   JMP   test_var_declaration@tail1
.if_end@13:
   JMP   test_var_declaration@tail0

test_derived_types@run0:
   SUBROUTINE

   LDA   #$03
   STA   __zpabi_calloc__nmemb_0
   LDA   #$02
   STA   __zpabi_calloc__size_0
   LDA   #$00
   STA   __zpabi_calloc__nmemb_1
   STA   __zpabi_calloc__nmemb_2
   STA   __zpabi_calloc__nmemb_3
   STA   __zpabi_calloc__size_1
   STA   __zpabi_calloc__size_2
   STA   __zpabi_calloc__size_3
   JMP   calloc

test_contentless_tag_noop:
   SUBROUTINE

   LDA   #$00
   LDY   #$00
   CMP   #$00
   BNE   .test_contentless_tag_noop@jthread@0
   LDA   #$0A
   CMP   #$0A
   BNE   .test_contentless_tag_noop@jthread@0
   TYA
   BNE   .test_contentless_tag_noop@jthread@0
   LDA   #$0B
   CMP   #$0B
   BEQ   .if_end@16
.test_contentless_tag_noop@jthread@0:
   JMP   test_var_declaration@tail1
.if_end@16:
   JMP   test_var_declaration@tail0

main:
   SUBROUTINE

   JSR   test_var_declaration
   STX   __local_main__0
   ORA   __local_main__0
   BNE   .if_end@17
   JMP   test_var_declaration@tail0
.if_end@17:
   JSR   test_member_declaration
   STX   __local_main__0
   ORA   __local_main__0
   BNE   .if_end@18
   LDX   #$00
   LDA   #$02
   RTS
.if_end@18:
   JSR   test_function_declaration
   STX   __local_main__0
   ORA   __local_main__0
   BNE   .if_end@19
   LDX   #$00
   LDA   #$03
   RTS
.if_end@19:
   JSR   test_for_loop
   STX   __local_main__0
   ORA   __local_main__0
   BNE   .if_end@20
   LDX   #$00
   LDA   #$04
   RTS
.if_end@20:
   JSR   test_cast
   STX   __local_main__0
   ORA   __local_main__0
   BNE   .if_end@21
   LDX   #$00
   LDA   #$05
   RTS
.if_end@21:
   JSR   test_sizeof
   STX   __local_main__0
   ORA   __local_main__0
   BNE   .if_end@22
   LDX   #$00
   LDA   #$06
   RTS
.if_end@22:
   JSR   test_derived_types
   STX   __local_main__0
   ORA   __local_main__0
   BNE   .if_end@23
   LDX   #$00
   LDA   #$07
   RTS
.if_end@23:
   JSR   test_contentless_tag_noop
   STX   __local_main__0
   ORA   __local_main__0
   BNE   .if_end@24
   LDX   #$00
   LDA   #$08
   RTS
.if_end@24:
   JMP   test_var_declaration@tail1

@17.loop_body_struct:
   DS.B  8
