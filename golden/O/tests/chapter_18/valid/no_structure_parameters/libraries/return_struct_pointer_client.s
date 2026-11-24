SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__local_main__0	EQU	$80
__zpabi_malloc__size_0	EQU	$80
__zpabi_malloc__size_1	EQU	$81
__local_test_get_struct_pointer_member__0	EQU	$82
__local_test_update_member_thru_retval__0	EQU	$82
__local_test_update_nested_struct_thru_retval__0	EQU	$82
__zpabi_malloc__size_2	EQU	$82
__local_test_get_struct_pointer_member__1	EQU	$83
__local_test_get_struct_ptr__1	EQU	$83
__local_test_update_member_thru_retval__1	EQU	$83
__local_test_update_nested_struct_thru_retval__1	EQU	$83
__zpabi_malloc__size_3	EQU	$83
__local_test_get_struct_pointer_member__2	EQU	$84
__local_test_get_struct_ptr__2	EQU	$84
__local_test_update_member_thru_retval__2	EQU	$84
__local_test_update_nested_struct_thru_retval__2	EQU	$84
__local_test_get_struct_pointer_member__3	EQU	$85
__local_test_get_struct_ptr__3	EQU	$85
__local_test_update_member_thru_retval__3	EQU	$85
__local_test_update_nested_struct_thru_retval__small_9	EQU	$85
__local_test_get_struct_pointer_member__4	EQU	$86
__local_test_get_struct_ptr__4	EQU	$86
__local_test_update_member_thru_retval__4	EQU	$86
__local_test_update_nested_struct_thru_retval__small_8	EQU	$86
__local_test_get_struct_pointer_member__5	EQU	$87
__local_test_get_struct_ptr__5	EQU	$87
__local_test_update_member_thru_retval__5	EQU	$87
__local_test_update_nested_struct_thru_retval__small_7	EQU	$87
__local_test_get_struct_pointer_member__6	EQU	$88
__local_test_get_struct_ptr__6	EQU	$88
__local_test_update_member_thru_retval__6	EQU	$88
__local_test_update_nested_struct_thru_retval__small_6	EQU	$88
__local_test_get_struct_pointer_member__7	EQU	$89
__local_test_get_struct_ptr__7	EQU	$89
__local_test_update_member_thru_retval__7	EQU	$89
__local_test_update_nested_struct_thru_retval__small_5	EQU	$89
__local_test_get_struct_pointer_member__8	EQU	$8A
__local_test_get_struct_ptr__8	EQU	$8A
__local_test_update_member_thru_retval__8	EQU	$8A
__local_test_update_nested_struct_thru_retval__small_4	EQU	$8A
__local_test_get_struct_pointer_member__9	EQU	$8B
__local_test_get_struct_ptr__9	EQU	$8B
__local_test_update_member_thru_retval__9	EQU	$8B
__local_test_update_nested_struct_thru_retval__small_3	EQU	$8B
__local_test_get_struct_ptr__o	EQU	$8C
__local_test_update_nested_struct_thru_retval__small_2	EQU	$8C
__local_test_update_nested_struct_thru_retval__small_1	EQU	$8D

; @zp-link-meta-begin
; def get_static_struct_ptr params= locals= indirect=false in_cycle=false
; def main params= locals=__local_main__0,__local_main__1 indirect=false in_cycle=false
; def test_get_struct_pointer_member params= locals=__local_test_get_struct_pointer_member__0,__local_test_get_struct_pointer_member__1,__local_test_get_struct_pointer_member__2,__local_test_get_struct_pointer_member__3,__local_test_get_struct_pointer_member__4,__local_test_get_struct_pointer_member__5,__local_test_get_struct_pointer_member__6,__local_test_get_struct_pointer_member__7,__local_test_get_struct_pointer_member__8,__local_test_get_struct_pointer_member__9 indirect=false in_cycle=false
; def test_get_struct_ptr params= locals=__local_test_get_struct_ptr__0,__local_test_get_struct_ptr__1,__local_test_get_struct_ptr__2,__local_test_get_struct_ptr__3,__local_test_get_struct_ptr__4,__local_test_get_struct_ptr__5,__local_test_get_struct_ptr__6,__local_test_get_struct_ptr__7,__local_test_get_struct_ptr__8,__local_test_get_struct_ptr__9,__local_test_get_struct_ptr__o,__local_test_get_struct_ptr__11,__local_test_get_struct_ptr__12,__local_test_get_struct_ptr__13,__local_test_get_struct_ptr__14,__local_test_get_struct_ptr__15,__local_test_get_struct_ptr__16,__local_test_get_struct_ptr__17,__local_test_get_struct_ptr__18,__local_test_get_struct_ptr__19,__local_test_get_struct_ptr__20,__local_test_get_struct_ptr__21,__local_test_get_struct_ptr__22,__local_test_get_struct_ptr__23,__local_test_get_struct_ptr__24,__local_test_get_struct_ptr__25 indirect=false in_cycle=false
; def test_update_member_thru_retval params= locals=__local_test_update_member_thru_retval__0,__local_test_update_member_thru_retval__1,__local_test_update_member_thru_retval__2,__local_test_update_member_thru_retval__3,__local_test_update_member_thru_retval__4,__local_test_update_member_thru_retval__5,__local_test_update_member_thru_retval__6,__local_test_update_member_thru_retval__7,__local_test_update_member_thru_retval__8,__local_test_update_member_thru_retval__9 indirect=false in_cycle=false
; def test_update_nested_struct_thru_retval params= locals=__local_test_update_nested_struct_thru_retval__0,__local_test_update_nested_struct_thru_retval__1,__local_test_update_nested_struct_thru_retval__2,__local_test_update_nested_struct_thru_retval__small_9,__local_test_update_nested_struct_thru_retval__small_8,__local_test_update_nested_struct_thru_retval__small_7,__local_test_update_nested_struct_thru_retval__small_6,__local_test_update_nested_struct_thru_retval__small_5,__local_test_update_nested_struct_thru_retval__small_4,__local_test_update_nested_struct_thru_retval__small_3,__local_test_update_nested_struct_thru_retval__small_2,__local_test_update_nested_struct_thru_retval__small_1,__local_test_update_nested_struct_thru_retval__small_0,__local_test_update_nested_struct_thru_retval__3,__local_test_update_nested_struct_thru_retval__4,__local_test_update_nested_struct_thru_retval__5,__local_test_update_nested_struct_thru_retval__6,__local_test_update_nested_struct_thru_retval__7,__local_test_update_nested_struct_thru_retval__8,__local_test_update_nested_struct_thru_retval__9 indirect=false in_cycle=false
; ext make_struct_inner params=__zpabi_make_struct_inner__seed_0,__zpabi_make_struct_inner__seed_1 param_regs=A,X
; ext make_struct_outer params=__zpabi_make_struct_outer__seed_0,__zpabi_make_struct_outer__seed_1 param_regs=A,X
; ext make_struct_outermost params=__zpabi_make_struct_outermost__seed_0,__zpabi_make_struct_outermost__seed_1 param_regs=A,X
; ext malloc params=__zpabi_malloc__size_0,__zpabi_malloc__size_1,__zpabi_malloc__size_2,__zpabi_malloc__size_3
; call main -> test_get_struct_pointer_member
; call main -> test_get_struct_ptr
; call main -> test_update_member_thru_retval
; call main -> test_update_nested_struct_thru_retval
; call test_get_struct_pointer_member -> make_struct_inner
; call test_get_struct_pointer_member -> make_struct_outer
; call test_get_struct_pointer_member -> make_struct_outermost
; call test_get_struct_ptr -> make_struct_inner
; call test_get_struct_ptr -> make_struct_outer
; call test_update_member_thru_retval -> get_static_struct_ptr
; call test_update_nested_struct_thru_retval -> get_static_struct_ptr
; item test_get_struct_ptr section=.text local=false zero=false
; item test_get_struct_ptr@tail1 section=.text local=true zero=false
; item test_get_struct_ptr@tail0 section=.text local=true zero=false
; item test_get_struct_pointer_member section=.text local=false zero=false
; item get_static_struct_ptr section=.text local=false zero=false
; item test_update_member_thru_retval section=.text local=false zero=false
; item test_update_nested_struct_thru_retval section=.text local=false zero=false
; item main section=.text local=false zero=false
; item @6.s section=.bss local=true zero=true
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

test_get_struct_ptr:
   SUBROUTINE

   LDX   #$00
   LDA   #$0B
   JSR   make_struct_inner
   STA   __local_test_get_struct_ptr__8
   STX   __local_test_get_struct_ptr__9
   LDY   #$00
   LDA   (__local_test_get_struct_ptr__8),Y
   STA   __local_test_get_struct_ptr__7
   INY
   LDA   (__local_test_get_struct_ptr__8),Y
   STA   __local_test_get_struct_ptr__6
   INY
   LDA   (__local_test_get_struct_ptr__8),Y
   STA   __local_test_get_struct_ptr__5
   INY
   LDA   (__local_test_get_struct_ptr__8),Y
   STA   __local_test_get_struct_ptr__4
   INY
   LDA   (__local_test_get_struct_ptr__8),Y
   STA   __local_test_get_struct_ptr__3
   INY
   LDA   (__local_test_get_struct_ptr__8),Y
   STA   __local_test_get_struct_ptr__2
   INY
   LDA   (__local_test_get_struct_ptr__8),Y
   STA   __local_test_get_struct_ptr__1
   INY
   LDA   (__local_test_get_struct_ptr__8),Y
   CMP   #$40
   BNE   .test_get_struct_ptr@jthread@0
   LDA   __local_test_get_struct_ptr__1
   CMP   #$26
   BNE   .test_get_struct_ptr@jthread@0
   LDA   __local_test_get_struct_ptr__2
   BNE   .test_get_struct_ptr@jthread@0
   LDA   __local_test_get_struct_ptr__3
   BNE   .test_get_struct_ptr@jthread@0
   LDA   __local_test_get_struct_ptr__4
   BNE   .test_get_struct_ptr@jthread@0
   LDA   __local_test_get_struct_ptr__5
   BNE   .test_get_struct_ptr@jthread@0
   LDA   __local_test_get_struct_ptr__6
   BNE   .test_get_struct_ptr@jthread@0
   LDA   __local_test_get_struct_ptr__7
   BNE   .test_get_struct_ptr@jthread@0
   INY
   LDA   (__local_test_get_struct_ptr__8),Y
   STA   __local_test_get_struct_ptr__1
   INY
   LDA   (__local_test_get_struct_ptr__8),Y
   BNE   .test_get_struct_ptr@jthread@0
   LDA   __local_test_get_struct_ptr__1
   CMP   #$0B
   BEQ   .if_end@2
.test_get_struct_ptr@jthread@0:
   JMP   test_get_struct_ptr@tail1
.if_end@2:
   LDY   #$10
   LDA   #$00
.clear@3:
   DEY
   STA   __local_test_get_struct_ptr__o,Y
   BNE   .clear@3
   LDX   #$00
   LDA   #$14
   JSR   make_struct_outer
   STA   __local_test_get_struct_ptr__8
   STX   __local_test_get_struct_ptr__9
   LDY   #$00
   LDA   (__local_test_get_struct_ptr__8),Y
   CMP   #$14
   BNE   .test_get_struct_ptr@jthread@3
   INY
   LDA   (__local_test_get_struct_ptr__8),Y
   CMP   #$15
   BNE   .test_get_struct_ptr@jthread@3
   INY
   LDA   (__local_test_get_struct_ptr__8),Y
   STA   __local_test_get_struct_ptr__7
   INY
   LDA   (__local_test_get_struct_ptr__8),Y
   STA   __local_test_get_struct_ptr__6
   INY
   LDA   (__local_test_get_struct_ptr__8),Y
   STA   __local_test_get_struct_ptr__5
   INY
   LDA   (__local_test_get_struct_ptr__8),Y
   STA   __local_test_get_struct_ptr__4
   INY
   LDA   (__local_test_get_struct_ptr__8),Y
   STA   __local_test_get_struct_ptr__3
   INY
   LDA   (__local_test_get_struct_ptr__8),Y
   STA   __local_test_get_struct_ptr__2
   INY
   LDA   (__local_test_get_struct_ptr__8),Y
   STA   __local_test_get_struct_ptr__1
   INY
   LDA   (__local_test_get_struct_ptr__8),Y
   CMP   #$40
   BNE   .test_get_struct_ptr@jthread@3
   LDA   __local_test_get_struct_ptr__1
   CMP   #$36
   BNE   .test_get_struct_ptr@jthread@3
   LDA   __local_test_get_struct_ptr__2
   BNE   .test_get_struct_ptr@jthread@3
   LDA   __local_test_get_struct_ptr__3
   BNE   .test_get_struct_ptr@jthread@3
   LDA   __local_test_get_struct_ptr__4
   BNE   .test_get_struct_ptr@jthread@3
   LDA   __local_test_get_struct_ptr__5
   BNE   .test_get_struct_ptr@jthread@3
   LDA   __local_test_get_struct_ptr__6
   BNE   .test_get_struct_ptr@jthread@3
   LDA   __local_test_get_struct_ptr__7
   BNE   .test_get_struct_ptr@jthread@3
   INY
   LDA   (__local_test_get_struct_ptr__8),Y
   STA   __local_test_get_struct_ptr__1
   INY
   LDA   (__local_test_get_struct_ptr__8),Y
   BNE   .test_get_struct_ptr@jthread@3
   LDA   __local_test_get_struct_ptr__1
   CMP   #$17
   BEQ   .if_end@10
.test_get_struct_ptr@jthread@3:
   LDX   #$00
   TXA
   RTS
.if_end@10:
   JMP   test_get_struct_ptr@tail0

test_get_struct_ptr@tail1:
   SUBROUTINE

   LDX   #$00
   TXA
   RTS

test_get_struct_ptr@tail0:
   SUBROUTINE

   LDX   #$00
   LDA   #$01
   RTS

test_get_struct_pointer_member:
   SUBROUTINE

   LDX   #$00
   LDA   #$02
   JSR   make_struct_inner
   STA   __local_test_get_struct_pointer_member__8
   STX   __local_test_get_struct_pointer_member__9
   LDY   #$00
   LDA   (__local_test_get_struct_pointer_member__8),Y
   STA   __local_test_get_struct_pointer_member__7
   INY
   LDA   (__local_test_get_struct_pointer_member__8),Y
   STA   __local_test_get_struct_pointer_member__6
   INY
   LDA   (__local_test_get_struct_pointer_member__8),Y
   STA   __local_test_get_struct_pointer_member__5
   INY
   LDA   (__local_test_get_struct_pointer_member__8),Y
   STA   __local_test_get_struct_pointer_member__4
   INY
   LDA   (__local_test_get_struct_pointer_member__8),Y
   STA   __local_test_get_struct_pointer_member__3
   INY
   LDA   (__local_test_get_struct_pointer_member__8),Y
   STA   __local_test_get_struct_pointer_member__2
   INY
   LDA   (__local_test_get_struct_pointer_member__8),Y
   STA   __local_test_get_struct_pointer_member__1
   INY
   LDA   (__local_test_get_struct_pointer_member__8),Y
   CMP   #$40
   BNE   .jcmp_skip@0
   LDA   __local_test_get_struct_pointer_member__1
   BNE   .jcmp_skip@0
   LDA   __local_test_get_struct_pointer_member__2
   BNE   .jcmp_skip@0
   LDA   __local_test_get_struct_pointer_member__3
   BNE   .jcmp_skip@0
   LDA   __local_test_get_struct_pointer_member__4
   BNE   .jcmp_skip@0
   LDA   __local_test_get_struct_pointer_member__5
   BNE   .jcmp_skip@0
   LDA   __local_test_get_struct_pointer_member__6
   BNE   .jcmp_skip@0
   LDA   __local_test_get_struct_pointer_member__7
   BEQ   .if_end@11
.jcmp_skip@0:
   JMP   test_get_struct_ptr@tail1
.if_end@11:
   LDX   #$00
   LDA   #$02
   JSR   make_struct_outer
   STA   __local_test_get_struct_pointer_member__8
   STX   __local_test_get_struct_pointer_member__9
   LDY   #$02
   LDA   (__local_test_get_struct_pointer_member__8),Y
   STA   __local_test_get_struct_pointer_member__7
   INY
   LDA   (__local_test_get_struct_pointer_member__8),Y
   STA   __local_test_get_struct_pointer_member__6
   INY
   LDA   (__local_test_get_struct_pointer_member__8),Y
   STA   __local_test_get_struct_pointer_member__5
   INY
   LDA   (__local_test_get_struct_pointer_member__8),Y
   STA   __local_test_get_struct_pointer_member__4
   INY
   LDA   (__local_test_get_struct_pointer_member__8),Y
   STA   __local_test_get_struct_pointer_member__3
   INY
   LDA   (__local_test_get_struct_pointer_member__8),Y
   STA   __local_test_get_struct_pointer_member__2
   INY
   LDA   (__local_test_get_struct_pointer_member__8),Y
   STA   __local_test_get_struct_pointer_member__1
   INY
   LDA   (__local_test_get_struct_pointer_member__8),Y
   CMP   #$40
   BNE   .tail@0
   LDA   __local_test_get_struct_pointer_member__1
   CMP   #$10
   BNE   .tail@0
   LDA   __local_test_get_struct_pointer_member__2
   BNE   .tail@0
   LDA   __local_test_get_struct_pointer_member__3
   BNE   .tail@0
   LDA   __local_test_get_struct_pointer_member__4
   BNE   .tail@0
   LDA   __local_test_get_struct_pointer_member__5
   BNE   .tail@0
   LDA   __local_test_get_struct_pointer_member__6
   BNE   .tail@0
   LDA   __local_test_get_struct_pointer_member__7
   BNE   .tail@0
   LDX   #$00
   TXA
   JSR   make_struct_outermost
   STA   __local_test_get_struct_pointer_member__2
   STX   __local_test_get_struct_pointer_member__3
   LDY   #$02
   LDA   (__local_test_get_struct_pointer_member__2),Y
   STA   __local_test_get_struct_pointer_member__0
   INY
   LDA   (__local_test_get_struct_pointer_member__2),Y
   STA   __local_test_get_struct_pointer_member__1
   LDY   #$00
   LDA   (__local_test_get_struct_pointer_member__0),Y
   CMP   #$01
   BEQ   .if_end@13
.tail@0:
   LDX   #$00
   TXA
   RTS
.if_end@13:
   JMP   test_get_struct_ptr@tail0

get_static_struct_ptr:
   SUBROUTINE

   LDX   #>@6.s
   LDA   #<@6.s
   RTS

test_update_member_thru_retval:
   SUBROUTINE

   JSR   get_static_struct_ptr
   STA   __local_test_update_member_thru_retval__0
   STX   __local_test_update_member_thru_retval__1
   LDA   #$0A
   LDY   #$00
   STA   (__local_test_update_member_thru_retval__0),Y
   JSR   get_static_struct_ptr
   STA   __local_test_update_member_thru_retval__0
   STX   __local_test_update_member_thru_retval__1
   LDA   #$00
   LDY   #$02
   STA   (__local_test_update_member_thru_retval__0),Y
   INY
   STA   (__local_test_update_member_thru_retval__0),Y
   INY
   STA   (__local_test_update_member_thru_retval__0),Y
   INY
   STA   (__local_test_update_member_thru_retval__0),Y
   INY
   STA   (__local_test_update_member_thru_retval__0),Y
   INY
   STA   (__local_test_update_member_thru_retval__0),Y
   LDA   #$34
   INY
   STA   (__local_test_update_member_thru_retval__0),Y
   LDA   #$40
   INY
   STA   (__local_test_update_member_thru_retval__0),Y
   JSR   get_static_struct_ptr
   STA   __local_test_update_member_thru_retval__8
   STX   __local_test_update_member_thru_retval__9
   LDY   #$00
   LDA   (__local_test_update_member_thru_retval__8),Y
   CMP   #$0A
   BNE   .test_update_member_thru_retval@jthread@0
   LDY   #$02
   LDA   (__local_test_update_member_thru_retval__8),Y
   STA   __local_test_update_member_thru_retval__7
   INY
   LDA   (__local_test_update_member_thru_retval__8),Y
   STA   __local_test_update_member_thru_retval__6
   INY
   LDA   (__local_test_update_member_thru_retval__8),Y
   STA   __local_test_update_member_thru_retval__5
   INY
   LDA   (__local_test_update_member_thru_retval__8),Y
   STA   __local_test_update_member_thru_retval__4
   INY
   LDA   (__local_test_update_member_thru_retval__8),Y
   STA   __local_test_update_member_thru_retval__3
   INY
   LDA   (__local_test_update_member_thru_retval__8),Y
   STA   __local_test_update_member_thru_retval__2
   INY
   LDA   (__local_test_update_member_thru_retval__8),Y
   STA   __local_test_update_member_thru_retval__1
   INY
   LDA   (__local_test_update_member_thru_retval__8),Y
   CMP   #$40
   BNE   .test_update_member_thru_retval@jthread@0
   LDA   __local_test_update_member_thru_retval__1
   CMP   #$34
   BNE   .test_update_member_thru_retval@jthread@0
   LDA   __local_test_update_member_thru_retval__2
   BNE   .test_update_member_thru_retval@jthread@0
   LDA   __local_test_update_member_thru_retval__3
   BNE   .test_update_member_thru_retval@jthread@0
   LDA   __local_test_update_member_thru_retval__4
   BNE   .test_update_member_thru_retval@jthread@0
   LDA   __local_test_update_member_thru_retval__5
   BNE   .test_update_member_thru_retval@jthread@0
   LDA   __local_test_update_member_thru_retval__6
   BNE   .test_update_member_thru_retval@jthread@0
   LDA   __local_test_update_member_thru_retval__7
   BEQ   .if_end@16
.test_update_member_thru_retval@jthread@0:
   JMP   test_get_struct_ptr@tail1
.if_end@16:
   JMP   test_get_struct_ptr@tail0

test_update_nested_struct_thru_retval:
   SUBROUTINE

   LDA   #$00
   LDY   #$00
   STY   __local_test_update_nested_struct_thru_retval__small_1
   STY   __local_test_update_nested_struct_thru_retval__small_2
   STY   __local_test_update_nested_struct_thru_retval__small_3
   STY   __local_test_update_nested_struct_thru_retval__small_4
   STA   __local_test_update_nested_struct_thru_retval__small_5
   LDA   #$28
   STA   __local_test_update_nested_struct_thru_retval__small_6
   LDA   #$40
   STA   __local_test_update_nested_struct_thru_retval__small_7
   LDA   #$0D
   STA   __local_test_update_nested_struct_thru_retval__small_8
   STY   __local_test_update_nested_struct_thru_retval__small_9
   JSR   get_static_struct_ptr
   STX   __local_test_update_nested_struct_thru_retval__1
   CLC
   ADC   #$02
   STA   __local_test_update_nested_struct_thru_retval__0
   BCC   .carry_inc@0
   INC   __local_test_update_nested_struct_thru_retval__1
.carry_inc@0:
   STA   DPTR
   LDA   __local_test_update_nested_struct_thru_retval__1
   STA   DPTR+1
   LDA   #$00
   LDY   #$00
   STA   (__local_test_update_nested_struct_thru_retval__0),Y
   LDA   __local_test_update_nested_struct_thru_retval__small_1
   INY
   STA   (DPTR),Y
   LDA   __local_test_update_nested_struct_thru_retval__small_2
   INY
   STA   (DPTR),Y
   LDA   __local_test_update_nested_struct_thru_retval__small_3
   INY
   STA   (DPTR),Y
   LDA   __local_test_update_nested_struct_thru_retval__small_4
   INY
   STA   (DPTR),Y
   LDA   __local_test_update_nested_struct_thru_retval__small_5
   INY
   STA   (DPTR),Y
   LDA   __local_test_update_nested_struct_thru_retval__small_6
   INY
   STA   (DPTR),Y
   LDA   __local_test_update_nested_struct_thru_retval__small_7
   INY
   STA   (DPTR),Y
   LDA   __local_test_update_nested_struct_thru_retval__small_8
   INY
   STA   (DPTR),Y
   LDA   __local_test_update_nested_struct_thru_retval__small_9
   INY
   STA   (DPTR),Y
   JSR   get_static_struct_ptr
   STA   __local_test_update_nested_struct_thru_retval__small_4
   STX   __local_test_update_nested_struct_thru_retval__small_3
   LDY   #$02
   LDA   (__local_test_update_nested_struct_thru_retval__small_4),Y
   STA   __local_test_update_nested_struct_thru_retval__small_5
   INY
   LDA   (__local_test_update_nested_struct_thru_retval__small_4),Y
   STA   __local_test_update_nested_struct_thru_retval__small_6
   INY
   LDA   (__local_test_update_nested_struct_thru_retval__small_4),Y
   STA   __local_test_update_nested_struct_thru_retval__small_7
   INY
   LDA   (__local_test_update_nested_struct_thru_retval__small_4),Y
   STA   __local_test_update_nested_struct_thru_retval__small_8
   INY
   LDA   (__local_test_update_nested_struct_thru_retval__small_4),Y
   STA   __local_test_update_nested_struct_thru_retval__small_9
   INY
   LDA   (__local_test_update_nested_struct_thru_retval__small_4),Y
   STA   __local_test_update_nested_struct_thru_retval__2
   INY
   LDA   (__local_test_update_nested_struct_thru_retval__small_4),Y
   STA   __local_test_update_nested_struct_thru_retval__1
   INY
   LDA   (__local_test_update_nested_struct_thru_retval__small_4),Y
   CMP   #$40
   BNE   .jcmp_skip@3
   LDA   __local_test_update_nested_struct_thru_retval__1
   CMP   #$28
   BNE   .jcmp_skip@3
   LDA   __local_test_update_nested_struct_thru_retval__2
   BNE   .jcmp_skip@3
   LDA   __local_test_update_nested_struct_thru_retval__small_9
   BNE   .jcmp_skip@3
   LDA   __local_test_update_nested_struct_thru_retval__small_8
   BNE   .jcmp_skip@3
   LDA   __local_test_update_nested_struct_thru_retval__small_7
   BNE   .jcmp_skip@3
   LDA   __local_test_update_nested_struct_thru_retval__small_6
   BNE   .jcmp_skip@3
   LDA   __local_test_update_nested_struct_thru_retval__small_5
   BNE   .jcmp_skip@3
   JSR   get_static_struct_ptr
   STA   __local_test_update_nested_struct_thru_retval__2
   STX   __local_test_update_nested_struct_thru_retval__small_9
   LDY   #$0A
   LDA   (__local_test_update_nested_struct_thru_retval__2),Y
   STA   __local_test_update_nested_struct_thru_retval__1
   INY
   LDA   (__local_test_update_nested_struct_thru_retval__2),Y
   BNE   .jcmp_skip@3
   LDA   __local_test_update_nested_struct_thru_retval__1
   CMP   #$0D
   BEQ   .if_end@18
.jcmp_skip@3:
   JMP   test_get_struct_ptr@tail1
.if_end@18:
   JMP   test_get_struct_ptr@tail0

main:
   SUBROUTINE

   JSR   test_get_struct_ptr
   STX   __local_main__0
   ORA   __local_main__0
   BNE   .if_end@19
   JMP   test_get_struct_ptr@tail0
.if_end@19:
   JSR   test_get_struct_pointer_member
   STX   __local_main__0
   ORA   __local_main__0
   BNE   .if_end@20
   LDX   #$00
   LDA   #$02
   RTS
.if_end@20:
   JSR   test_update_member_thru_retval
   STX   __local_main__0
   ORA   __local_main__0
   BNE   .if_end@21
   LDX   #$00
   LDA   #$03
   RTS
.if_end@21:
   JSR   test_update_nested_struct_thru_retval
   STX   __local_main__0
   ORA   __local_main__0
   BNE   .if_end@22
   LDX   #$00
   LDA   #$04
   RTS
.if_end@22:
   JMP   test_get_struct_ptr@tail1

@6.s:
   DS.B  12
