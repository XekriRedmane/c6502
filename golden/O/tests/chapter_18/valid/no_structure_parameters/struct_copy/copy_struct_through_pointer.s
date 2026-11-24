SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__zpabi_malloc__size_0	EQU	$80
__zpabi_strcmp__s2_0	EQU	$80
__zpabi_malloc__size_1	EQU	$81
__zpabi_strcmp__s2_1	EQU	$81
__zpabi_malloc__size_2	EQU	$82
__zpabi_malloc__size_3	EQU	$83
__local_main__0	EQU	$84
__local_test_copy_array_element_with_padding__0	EQU	$86
__local_test_copy_from_array_elem__0	EQU	$86
__local_test_copy_from_pointer__0	EQU	$86
__local_test_copy_to_and_from_array_elem__0	EQU	$86
__local_test_copy_to_and_from_pointer__0	EQU	$86
__local_test_copy_to_array_elem__0	EQU	$86
__local_test_copy_to_pointer__y_8	EQU	$86
__local_test_copy_array_element_with_padding__1	EQU	$87
__local_test_copy_from_array_elem__1	EQU	$87
__local_test_copy_from_pointer__1	EQU	$87
__local_test_copy_to_and_from_array_elem__1	EQU	$87
__local_test_copy_to_and_from_pointer__1	EQU	$87
__local_test_copy_to_array_elem__1	EQU	$87
__local_test_copy_to_pointer__y_7	EQU	$87
__local_test_copy_array_element_with_padding__elem_4	EQU	$88
__local_test_copy_from_array_elem__2	EQU	$88
__local_test_copy_from_pointer__2	EQU	$88
__local_test_copy_to_and_from_array_elem__2	EQU	$88
__local_test_copy_to_and_from_pointer__2	EQU	$88
__local_test_copy_to_array_elem__2	EQU	$88
__local_test_copy_to_pointer__y_6	EQU	$88
__local_test_copy_array_element_with_padding__elem_3	EQU	$89
__local_test_copy_from_array_elem__3	EQU	$89
__local_test_copy_from_pointer__3	EQU	$89
__local_test_copy_to_and_from_array_elem__3	EQU	$89
__local_test_copy_to_and_from_pointer__3	EQU	$89
__local_test_copy_to_array_elem__3	EQU	$89
__local_test_copy_to_pointer__y_5	EQU	$89
__local_test_copy_array_element_with_padding__elem_2	EQU	$8A
__local_test_copy_from_array_elem__x	EQU	$8A
__local_test_copy_from_pointer__x	EQU	$8A
__local_test_copy_to_and_from_array_elem__4	EQU	$8A
__local_test_copy_to_and_from_pointer__4	EQU	$8A
__local_test_copy_to_pointer__0	EQU	$8A
__local_test_copy_array_element_with_padding__elem_1	EQU	$8B
__local_test_copy_to_and_from_array_elem__5	EQU	$8B
__local_test_copy_to_and_from_pointer__5	EQU	$8B
__local_test_copy_to_pointer__1	EQU	$8B
__local_test_copy_to_and_from_array_elem__6	EQU	$8C
__local_test_copy_to_and_from_pointer__6	EQU	$8C
__local_test_copy_to_pointer__y_4	EQU	$8C
__local_test_copy_array_element_with_padding__arr	EQU	$8D
__local_test_copy_to_and_from_array_elem__7	EQU	$8D
__local_test_copy_to_and_from_pointer__7	EQU	$8D
__local_test_copy_to_pointer__y_3	EQU	$8D
__local_test_copy_to_and_from_array_elem__8	EQU	$8E
__local_test_copy_to_pointer__y_2	EQU	$8E
__local_test_copy_to_and_from_array_elem__9	EQU	$8F
__local_test_copy_to_and_from_pointer__9	EQU	$8F
__local_test_copy_to_pointer__y_1	EQU	$8F
__local_test_copy_to_and_from_array_elem__10	EQU	$90
__local_test_copy_to_and_from_pointer__10	EQU	$90
__local_test_copy_to_pointer__y_0	EQU	$90
__local_test_copy_to_and_from_array_elem__arr	EQU	$91
__local_test_copy_to_and_from_pointer__11	EQU	$91
__local_test_copy_to_and_from_pointer__12	EQU	$92
__local_test_copy_from_array_elem__arr	EQU	$93
__local_test_copy_to_and_from_pointer__my_struct	EQU	$93

; @zp-link-meta-begin
; def main params= locals=__local_main__0,__local_main__1 indirect=false in_cycle=false
; def test_copy_array_element_with_padding params= locals=__local_test_copy_array_element_with_padding__0,__local_test_copy_array_element_with_padding__1,__local_test_copy_array_element_with_padding__elem_4,__local_test_copy_array_element_with_padding__elem_3,__local_test_copy_array_element_with_padding__elem_2,__local_test_copy_array_element_with_padding__elem_1,__local_test_copy_array_element_with_padding__elem_0,__local_test_copy_array_element_with_padding__arr,__local_test_copy_array_element_with_padding__3,__local_test_copy_array_element_with_padding__4,__local_test_copy_array_element_with_padding__5,__local_test_copy_array_element_with_padding__6,__local_test_copy_array_element_with_padding__7,__local_test_copy_array_element_with_padding__8,__local_test_copy_array_element_with_padding__9,__local_test_copy_array_element_with_padding__10,__local_test_copy_array_element_with_padding__11,__local_test_copy_array_element_with_padding__12,__local_test_copy_array_element_with_padding__13,__local_test_copy_array_element_with_padding__14,__local_test_copy_array_element_with_padding__15,__local_test_copy_array_element_with_padding__16 indirect=false in_cycle=false
; def test_copy_from_array_elem params= locals=__local_test_copy_from_array_elem__0,__local_test_copy_from_array_elem__1,__local_test_copy_from_array_elem__2,__local_test_copy_from_array_elem__3,__local_test_copy_from_array_elem__x,__local_test_copy_from_array_elem__5,__local_test_copy_from_array_elem__6,__local_test_copy_from_array_elem__7,__local_test_copy_from_array_elem__8,__local_test_copy_from_array_elem__9,__local_test_copy_from_array_elem__10,__local_test_copy_from_array_elem__11,__local_test_copy_from_array_elem__12,__local_test_copy_from_array_elem__arr,__local_test_copy_from_array_elem__14,__local_test_copy_from_array_elem__15,__local_test_copy_from_array_elem__16,__local_test_copy_from_array_elem__17,__local_test_copy_from_array_elem__18,__local_test_copy_from_array_elem__19,__local_test_copy_from_array_elem__20,__local_test_copy_from_array_elem__21,__local_test_copy_from_array_elem__22,__local_test_copy_from_array_elem__23,__local_test_copy_from_array_elem__24,__local_test_copy_from_array_elem__25,__local_test_copy_from_array_elem__26,__local_test_copy_from_array_elem__27,__local_test_copy_from_array_elem__28,__local_test_copy_from_array_elem__29,__local_test_copy_from_array_elem__30,__local_test_copy_from_array_elem__31,__local_test_copy_from_array_elem__32,__local_test_copy_from_array_elem__33,__local_test_copy_from_array_elem__34,__local_test_copy_from_array_elem__35,__local_test_copy_from_array_elem__36,__local_test_copy_from_array_elem__37,__local_test_copy_from_array_elem__38,__local_test_copy_from_array_elem__39 indirect=false in_cycle=false
; def test_copy_from_pointer params= locals=__local_test_copy_from_pointer__0,__local_test_copy_from_pointer__1,__local_test_copy_from_pointer__2,__local_test_copy_from_pointer__3,__local_test_copy_from_pointer__x,__local_test_copy_from_pointer__5,__local_test_copy_from_pointer__6,__local_test_copy_from_pointer__7,__local_test_copy_from_pointer__8,__local_test_copy_from_pointer__9,__local_test_copy_from_pointer__10,__local_test_copy_from_pointer__11,__local_test_copy_from_pointer__12 indirect=false in_cycle=false
; def test_copy_to_and_from_array_elem params= locals=__local_test_copy_to_and_from_array_elem__0,__local_test_copy_to_and_from_array_elem__1,__local_test_copy_to_and_from_array_elem__2,__local_test_copy_to_and_from_array_elem__3,__local_test_copy_to_and_from_array_elem__4,__local_test_copy_to_and_from_array_elem__5,__local_test_copy_to_and_from_array_elem__6,__local_test_copy_to_and_from_array_elem__7,__local_test_copy_to_and_from_array_elem__8,__local_test_copy_to_and_from_array_elem__9,__local_test_copy_to_and_from_array_elem__10,__local_test_copy_to_and_from_array_elem__arr,__local_test_copy_to_and_from_array_elem__12,__local_test_copy_to_and_from_array_elem__13,__local_test_copy_to_and_from_array_elem__14,__local_test_copy_to_and_from_array_elem__15,__local_test_copy_to_and_from_array_elem__16,__local_test_copy_to_and_from_array_elem__17,__local_test_copy_to_and_from_array_elem__18,__local_test_copy_to_and_from_array_elem__19,__local_test_copy_to_and_from_array_elem__20,__local_test_copy_to_and_from_array_elem__21,__local_test_copy_to_and_from_array_elem__22,__local_test_copy_to_and_from_array_elem__23,__local_test_copy_to_and_from_array_elem__24,__local_test_copy_to_and_from_array_elem__25,__local_test_copy_to_and_from_array_elem__26,__local_test_copy_to_and_from_array_elem__27,__local_test_copy_to_and_from_array_elem__28,__local_test_copy_to_and_from_array_elem__29,__local_test_copy_to_and_from_array_elem__30,__local_test_copy_to_and_from_array_elem__31,__local_test_copy_to_and_from_array_elem__32,__local_test_copy_to_and_from_array_elem__33,__local_test_copy_to_and_from_array_elem__34,__local_test_copy_to_and_from_array_elem__35,__local_test_copy_to_and_from_array_elem__36,__local_test_copy_to_and_from_array_elem__37 indirect=false in_cycle=false
; def test_copy_to_and_from_pointer params= locals=__local_test_copy_to_and_from_pointer__0,__local_test_copy_to_and_from_pointer__1,__local_test_copy_to_and_from_pointer__2,__local_test_copy_to_and_from_pointer__3,__local_test_copy_to_and_from_pointer__4,__local_test_copy_to_and_from_pointer__5,__local_test_copy_to_and_from_pointer__6,__local_test_copy_to_and_from_pointer__7,__local_test_copy_to_and_from_pointer__8,__local_test_copy_to_and_from_pointer__9,__local_test_copy_to_and_from_pointer__10,__local_test_copy_to_and_from_pointer__11,__local_test_copy_to_and_from_pointer__12,__local_test_copy_to_and_from_pointer__my_struct,__local_test_copy_to_and_from_pointer__14,__local_test_copy_to_and_from_pointer__15,__local_test_copy_to_and_from_pointer__16,__local_test_copy_to_and_from_pointer__17,__local_test_copy_to_and_from_pointer__18,__local_test_copy_to_and_from_pointer__19,__local_test_copy_to_and_from_pointer__20,__local_test_copy_to_and_from_pointer__21 indirect=false in_cycle=false
; def test_copy_to_array_elem params= locals=__local_test_copy_to_array_elem__0,__local_test_copy_to_array_elem__1,__local_test_copy_to_array_elem__2,__local_test_copy_to_array_elem__3 indirect=false in_cycle=false
; def test_copy_to_pointer params= locals=__local_test_copy_to_pointer__y_8,__local_test_copy_to_pointer__y_7,__local_test_copy_to_pointer__y_6,__local_test_copy_to_pointer__y_5,__local_test_copy_to_pointer__0,__local_test_copy_to_pointer__1,__local_test_copy_to_pointer__y_4,__local_test_copy_to_pointer__y_3,__local_test_copy_to_pointer__y_2,__local_test_copy_to_pointer__y_1,__local_test_copy_to_pointer__y_0,__local_test_copy_to_pointer__2,__local_test_copy_to_pointer__3,__local_test_copy_to_pointer__4,__local_test_copy_to_pointer__5 indirect=false in_cycle=false
; ext malloc params=__zpabi_malloc__size_0,__zpabi_malloc__size_1,__zpabi_malloc__size_2,__zpabi_malloc__size_3
; ext strcmp params=__zpabi_strcmp__s1_0,__zpabi_strcmp__s1_1,__zpabi_strcmp__s2_0,__zpabi_strcmp__s2_1 param_regs=A,X,,
; call main -> test_copy_array_element_with_padding
; call main -> test_copy_from_array_elem
; call main -> test_copy_from_pointer
; call main -> test_copy_to_and_from_array_elem
; call main -> test_copy_to_and_from_pointer
; call main -> test_copy_to_array_elem
; call main -> test_copy_to_pointer
; call test_copy_from_array_elem -> strcmp
; call test_copy_from_pointer -> strcmp
; call test_copy_to_and_from_array_elem -> strcmp
; call test_copy_to_and_from_pointer -> malloc
; call test_copy_to_and_from_pointer -> strcmp
; call test_copy_to_array_elem -> strcmp
; call test_copy_to_pointer -> malloc
; call test_copy_to_pointer -> strcmp
; item test_copy_to_pointer section=.text local=false zero=false
; item test_copy_to_pointer@run0 section=.text local=true zero=false
; item test_copy_to_pointer@tail1 section=.text local=true zero=false
; item test_copy_to_pointer@tail0 section=.text local=true zero=false
; item test_copy_from_pointer section=.text local=false zero=false
; item test_copy_to_and_from_pointer section=.text local=false zero=false
; item test_copy_to_array_elem section=.text local=false zero=false
; item test_copy_from_array_elem section=.text local=false zero=false
; item test_copy_to_and_from_array_elem section=.text local=false zero=false
; item test_copy_array_element_with_padding section=.text local=false zero=false
; item main section=.text local=false zero=false
; item str@0 section=.rodata local=true zero=false
; item str@1 section=.rodata local=true zero=false
; item str@2 section=.rodata local=true zero=false
; item str@3 section=.rodata local=true zero=false
; item str@4 section=.rodata local=true zero=false
; item str@5 section=.rodata local=true zero=false
; item str@6 section=.rodata local=true zero=false
; item str@7 section=.rodata local=true zero=false
; item @5.my_struct section=.data local=true zero=false
; item @12.arr section=.bss local=true zero=true
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

test_copy_to_pointer:
   SUBROUTINE

   LDA   #$21
   STA   __local_test_copy_to_pointer__y_0
   LDA   #$3F
   STA   __local_test_copy_to_pointer__y_1
   LDA   #$00
   STA   __local_test_copy_to_pointer__y_2
   LDA   #$EC
   STA   __local_test_copy_to_pointer__y_3
   LDA   #$FF
   STA   __local_test_copy_to_pointer__y_4
   LDY   #$E2
   STY   __local_test_copy_to_pointer__y_5
   LDY   #$FF
   STY   __local_test_copy_to_pointer__y_6
   STY   __local_test_copy_to_pointer__y_7
   STA   __local_test_copy_to_pointer__y_8
   JSR   test_copy_to_pointer@run0
   STA   __local_test_copy_to_pointer__0
   STX   __local_test_copy_to_pointer__1
   STA   DPTR
   STX   DPTR+1
   LDA   __local_test_copy_to_pointer__y_0
   LDY   #$00
   STA   (__local_test_copy_to_pointer__0),Y
   LDA   __local_test_copy_to_pointer__y_1
   INY
   STA   (DPTR),Y
   LDA   __local_test_copy_to_pointer__y_2
   INY
   STA   (DPTR),Y
   LDA   __local_test_copy_to_pointer__y_3
   INY
   STA   (DPTR),Y
   LDA   __local_test_copy_to_pointer__y_4
   INY
   STA   (DPTR),Y
   LDA   __local_test_copy_to_pointer__y_5
   INY
   STA   (DPTR),Y
   LDA   __local_test_copy_to_pointer__y_6
   INY
   STA   (DPTR),Y
   LDA   __local_test_copy_to_pointer__y_7
   INY
   STA   (DPTR),Y
   LDA   __local_test_copy_to_pointer__y_8
   INY
   STA   (DPTR),Y
   LDA   #<str@0
   STA   __zpabi_strcmp__s2_0
   LDA   #>str@0
   STA   __zpabi_strcmp__s2_1
   LDA   __local_test_copy_to_pointer__0
   JSR   strcmp
   STX   __local_test_copy_to_pointer__y_8
   ORA   __local_test_copy_to_pointer__y_8
   BNE   .test_copy_to_pointer@jthread@1
   LDY   #$03
   LDA   (__local_test_copy_to_pointer__0),Y
   STA   __local_test_copy_to_pointer__y_7
   INY
   LDA   (__local_test_copy_to_pointer__0),Y
   CMP   #$FF
   BNE   .test_copy_to_pointer@jthread@1
   LDA   __local_test_copy_to_pointer__y_7
   CMP   #$EC
   BNE   .test_copy_to_pointer@jthread@1
   INY
   LDA   (__local_test_copy_to_pointer__0),Y
   STA   __local_test_copy_to_pointer__y_5
   INY
   LDA   (__local_test_copy_to_pointer__0),Y
   STA   __local_test_copy_to_pointer__y_6
   INY
   LDA   (__local_test_copy_to_pointer__0),Y
   STA   __local_test_copy_to_pointer__y_7
   INY
   LDA   (__local_test_copy_to_pointer__0),Y
   CMP   #$FF
   BNE   .test_copy_to_pointer@jthread@1
   LDA   __local_test_copy_to_pointer__y_7
   CMP   #$FF
   BNE   .test_copy_to_pointer@jthread@1
   LDA   __local_test_copy_to_pointer__y_6
   CMP   #$FF
   BNE   .test_copy_to_pointer@jthread@1
   LDA   __local_test_copy_to_pointer__y_5
   CMP   #$E2
   BEQ   .if_end@4
.test_copy_to_pointer@jthread@1:
   JMP   test_copy_to_pointer@tail1
.if_end@4:
   JMP   test_copy_to_pointer@tail0

test_copy_to_pointer@run0:
   SUBROUTINE

   LDA   #$09
   STA   __zpabi_malloc__size_0
   LDA   #$00
   STA   __zpabi_malloc__size_1
   STA   __zpabi_malloc__size_2
   STA   __zpabi_malloc__size_3
   JMP   malloc

test_copy_to_pointer@tail1:
   SUBROUTINE

   LDX   #$00
   TXA
   RTS

test_copy_to_pointer@tail0:
   SUBROUTINE

   LDX   #$00
   LDA   #$01
   RTS

test_copy_from_pointer:
   SUBROUTINE

   LDA   #<@5.my_struct
   STA   __local_test_copy_from_pointer__2
   LDA   #>@5.my_struct
   STA   __local_test_copy_from_pointer__3
   LDY   #$09
   LDA   #$00
.clear@5:
   DEY
   STA   __local_test_copy_from_pointer__x,Y
   BNE   .clear@5
   LDY   #$00
   LDA   (__local_test_copy_from_pointer__2),Y
   STA   __local_test_copy_from_pointer__x
   INY
   LDA   (__local_test_copy_from_pointer__2),Y
   STA   __local_test_copy_from_pointer__x+1
   INY
   LDA   (__local_test_copy_from_pointer__2),Y
   STA   __local_test_copy_from_pointer__x+2
   INY
   LDA   (__local_test_copy_from_pointer__2),Y
   STA   __local_test_copy_from_pointer__x+3
   INY
   LDA   (__local_test_copy_from_pointer__2),Y
   STA   __local_test_copy_from_pointer__x+4
   INY
   LDA   (__local_test_copy_from_pointer__2),Y
   STA   __local_test_copy_from_pointer__x+5
   INY
   LDA   (__local_test_copy_from_pointer__2),Y
   STA   __local_test_copy_from_pointer__x+6
   INY
   LDA   (__local_test_copy_from_pointer__2),Y
   STA   __local_test_copy_from_pointer__x+7
   INY
   LDA   (__local_test_copy_from_pointer__2),Y
   STA   __local_test_copy_from_pointer__x+8
   LDA   #<str@1
   STA   __zpabi_strcmp__s2_0
   LDA   #>str@1
   STA   __zpabi_strcmp__s2_1
   LDX   #>__local_test_copy_from_pointer__x
   LDA   #<__local_test_copy_from_pointer__x
   JSR   strcmp
   STX   __local_test_copy_from_pointer__0
   ORA   __local_test_copy_from_pointer__0
   BNE   .test_copy_from_pointer@jthread@1
   LDA   __local_test_copy_from_pointer__x+3
   STA   __local_test_copy_from_pointer__1
   LDA   __local_test_copy_from_pointer__x+4
   BNE   .test_copy_from_pointer@jthread@1
   LDA   __local_test_copy_from_pointer__1
   CMP   #$4D
   BNE   .test_copy_from_pointer@jthread@1
   LDA   __local_test_copy_from_pointer__x+5
   STA   __local_test_copy_from_pointer__3
   LDA   __local_test_copy_from_pointer__x+6
   STA   __local_test_copy_from_pointer__2
   LDA   __local_test_copy_from_pointer__x+7
   STA   __local_test_copy_from_pointer__1
   LDA   __local_test_copy_from_pointer__x+8
   BNE   .test_copy_from_pointer@jthread@1
   LDA   __local_test_copy_from_pointer__1
   BNE   .test_copy_from_pointer@jthread@1
   LDA   __local_test_copy_from_pointer__2
   BNE   .test_copy_from_pointer@jthread@1
   LDA   __local_test_copy_from_pointer__3
   CMP   #$4E
   BEQ   .if_end@10
.test_copy_from_pointer@jthread@1:
   JMP   test_copy_to_pointer@tail1
.if_end@10:
   JMP   test_copy_to_pointer@tail0

test_copy_to_and_from_pointer:
   SUBROUTINE

   LDA   #$2B
   STA   __local_test_copy_to_and_from_pointer__my_struct
   LDA   #$2D
   STA   __local_test_copy_to_and_from_pointer__my_struct+1
   LDA   #$00
   STA   __local_test_copy_to_and_from_pointer__my_struct+2
   LDA   #$E8
   STA   __local_test_copy_to_and_from_pointer__my_struct+3
   LDA   #$03
   STA   __local_test_copy_to_and_from_pointer__my_struct+4
   LDY   #$E9
   STY   __local_test_copy_to_and_from_pointer__my_struct+5
   STA   __local_test_copy_to_and_from_pointer__my_struct+6
   LDA   #$00
   STA   __local_test_copy_to_and_from_pointer__my_struct+7
   STA   __local_test_copy_to_and_from_pointer__my_struct+8
   LDA   #<__local_test_copy_to_and_from_pointer__my_struct
   STA   __local_test_copy_to_and_from_pointer__11
   LDA   #>__local_test_copy_to_and_from_pointer__my_struct
   STA   __local_test_copy_to_and_from_pointer__12
   JSR   test_copy_to_pointer@run0
   STA   __local_test_copy_to_and_from_pointer__9
   STX   __local_test_copy_to_and_from_pointer__10
   LDY   #$01
   LDA   (__local_test_copy_to_and_from_pointer__11),Y
   STA   __local_test_copy_to_and_from_pointer__7
   INY
   LDA   (__local_test_copy_to_and_from_pointer__11),Y
   STA   __local_test_copy_to_and_from_pointer__6
   INY
   LDA   (__local_test_copy_to_and_from_pointer__11),Y
   STA   __local_test_copy_to_and_from_pointer__5
   INY
   LDA   (__local_test_copy_to_and_from_pointer__11),Y
   STA   __local_test_copy_to_and_from_pointer__4
   INY
   LDA   (__local_test_copy_to_and_from_pointer__11),Y
   STA   __local_test_copy_to_and_from_pointer__3
   INY
   LDA   (__local_test_copy_to_and_from_pointer__11),Y
   STA   __local_test_copy_to_and_from_pointer__2
   INY
   LDA   (__local_test_copy_to_and_from_pointer__11),Y
   STA   __local_test_copy_to_and_from_pointer__1
   INY
   LDA   (__local_test_copy_to_and_from_pointer__11),Y
   STA   __local_test_copy_to_and_from_pointer__0
   LDA   __local_test_copy_to_and_from_pointer__9
   STA   DPTR
   STX   DPTR+1
   LDY   #$00
   LDA   (__local_test_copy_to_and_from_pointer__11),Y
   STA   (__local_test_copy_to_and_from_pointer__9),Y
   LDA   __local_test_copy_to_and_from_pointer__7
   INY
   STA   (DPTR),Y
   LDA   __local_test_copy_to_and_from_pointer__6
   INY
   STA   (DPTR),Y
   LDA   __local_test_copy_to_and_from_pointer__5
   INY
   STA   (DPTR),Y
   LDA   __local_test_copy_to_and_from_pointer__4
   INY
   STA   (DPTR),Y
   LDA   __local_test_copy_to_and_from_pointer__3
   INY
   STA   (DPTR),Y
   LDA   __local_test_copy_to_and_from_pointer__2
   INY
   STA   (DPTR),Y
   LDA   __local_test_copy_to_and_from_pointer__1
   INY
   STA   (DPTR),Y
   LDA   __local_test_copy_to_and_from_pointer__0
   INY
   STA   (DPTR),Y
   LDA   #<str@2
   STA   __zpabi_strcmp__s2_0
   LDA   #>str@2
   STA   __zpabi_strcmp__s2_1
   LDA   __local_test_copy_to_and_from_pointer__9
   JSR   strcmp
   STX   __local_test_copy_to_and_from_pointer__0
   ORA   __local_test_copy_to_and_from_pointer__0
   BNE   .test_copy_to_and_from_pointer@jthread@1
   LDY   #$03
   LDA   (__local_test_copy_to_and_from_pointer__9),Y
   STA   __local_test_copy_to_and_from_pointer__1
   INY
   LDA   (__local_test_copy_to_and_from_pointer__9),Y
   CMP   #$03
   BNE   .test_copy_to_and_from_pointer@jthread@1
   LDA   __local_test_copy_to_and_from_pointer__1
   CMP   #$E8
   BNE   .test_copy_to_and_from_pointer@jthread@1
   INY
   LDA   (__local_test_copy_to_and_from_pointer__9),Y
   STA   __local_test_copy_to_and_from_pointer__3
   INY
   LDA   (__local_test_copy_to_and_from_pointer__9),Y
   STA   __local_test_copy_to_and_from_pointer__2
   INY
   LDA   (__local_test_copy_to_and_from_pointer__9),Y
   STA   __local_test_copy_to_and_from_pointer__1
   INY
   LDA   (__local_test_copy_to_and_from_pointer__9),Y
   BNE   .test_copy_to_and_from_pointer@jthread@1
   LDA   __local_test_copy_to_and_from_pointer__1
   BNE   .test_copy_to_and_from_pointer@jthread@1
   LDA   __local_test_copy_to_and_from_pointer__2
   CMP   #$03
   BNE   .test_copy_to_and_from_pointer@jthread@1
   LDA   __local_test_copy_to_and_from_pointer__3
   CMP   #$E9
   BEQ   .if_end@15
.test_copy_to_and_from_pointer@jthread@1:
   JMP   test_copy_to_pointer@tail1
.if_end@15:
   JMP   test_copy_to_pointer@tail0

test_copy_to_array_elem:
   SUBROUTINE

   LDA   #$0A
   STA   @12.arr+9
   LDA   #$09
   STA   @12.arr+10
   LDA   #$10
   STA   @12.arr+12
   LDA   #$27
   STA   @12.arr+13
   LDA   #$20
   STA   @12.arr+14
   LDA   #$4E
   STA   @12.arr+15
   LDA   #$00
   STA   @12.arr+11
   STA   @12.arr+16
   STA   @12.arr+17
   LDX   #>(@12.arr+9)
   LDA   #<str@3
   STA   __zpabi_strcmp__s2_0
   LDA   #>str@3
   STA   __zpabi_strcmp__s2_1
   LDA   #<(@12.arr+9)
   JSR   strcmp
   STX   __local_test_copy_to_array_elem__0
   ORA   __local_test_copy_to_array_elem__0
   BNE   .test_copy_to_array_elem@jthread@1
   LDA   @12.arr+12
   STA   __local_test_copy_to_array_elem__1
   LDA   @12.arr+13
   CMP   #$27
   BNE   .test_copy_to_array_elem@jthread@4
   LDA   __local_test_copy_to_array_elem__1
   CMP   #$10
   BNE   .test_copy_to_array_elem@jthread@4
   LDA   @12.arr+14
   STA   __local_test_copy_to_array_elem__3
   LDA   @12.arr+15
   STA   __local_test_copy_to_array_elem__2
   LDA   @12.arr+16
   STA   __local_test_copy_to_array_elem__1
   LDA   @12.arr+17
   BNE   .test_copy_to_array_elem@jthread@4
   LDA   __local_test_copy_to_array_elem__1
   BNE   .test_copy_to_array_elem@jthread@4
   LDA   __local_test_copy_to_array_elem__2
   CMP   #$4E
   BNE   .test_copy_to_array_elem@jthread@4
   LDA   __local_test_copy_to_array_elem__3
   CMP   #$20
   BEQ   .if_end@20
.test_copy_to_array_elem@jthread@1:
   JMP   test_copy_to_pointer@tail1
.if_end@20:
   LDA   @12.arr+3
   ORA   @12.arr+4
   BNE   .test_copy_to_array_elem@jthread@4
   LDA   @12.arr+5
   LDX   @12.arr+6
   STX   __local_test_copy_to_array_elem__2
   LDX   @12.arr+7
   STX   __local_test_copy_to_array_elem__1
   LDX   @12.arr+8
   STX   __local_test_copy_to_array_elem__0
   ORA   __local_test_copy_to_array_elem__2
   ORA   __local_test_copy_to_array_elem__1
   ORA   __local_test_copy_to_array_elem__0
   BNE   .test_copy_to_array_elem@jthread@4
   LDA   @12.arr+18
   BNE   .test_copy_to_array_elem@jthread@4
   LDA   @12.arr+19
   BEQ   .if_end@27
.test_copy_to_array_elem@jthread@4:
   LDX   #$00
   TXA
   RTS
.if_end@27:
   JMP   test_copy_to_pointer@tail0

test_copy_from_array_elem:
   SUBROUTINE

   LDA   #$61
   STA   __local_test_copy_from_array_elem__arr
   LDA   #$62
   STA   __local_test_copy_from_array_elem__arr+1
   LDA   #$00
   STA   __local_test_copy_from_array_elem__arr+2
   STA   __local_test_copy_from_array_elem__arr+11
   STA   __local_test_copy_from_array_elem__arr+20
   LDA   #$48
   STA   __local_test_copy_from_array_elem__arr+3
   LDA   #$F4
   STA   __local_test_copy_from_array_elem__arr+4
   LDA   #$60
   STA   __local_test_copy_from_array_elem__arr+5
   LDA   #$F0
   STA   __local_test_copy_from_array_elem__arr+6
   LDA   #$63
   STA   __local_test_copy_from_array_elem__arr+9
   LDA   #$64
   STA   __local_test_copy_from_array_elem__arr+10
   LDA   #$78
   STA   __local_test_copy_from_array_elem__arr+12
   LDA   #$EC
   STA   __local_test_copy_from_array_elem__arr+13
   LDA   #$90
   STA   __local_test_copy_from_array_elem__arr+14
   LDA   #$E8
   STA   __local_test_copy_from_array_elem__arr+15
   LDA   #$65
   STA   __local_test_copy_from_array_elem__arr+18
   LDA   #$66
   STA   __local_test_copy_from_array_elem__arr+19
   LDA   #$A8
   STA   __local_test_copy_from_array_elem__arr+21
   LDA   #$E4
   STA   __local_test_copy_from_array_elem__arr+22
   LDA   #$C0
   STA   __local_test_copy_from_array_elem__arr+23
   LDA   #$E0
   STA   __local_test_copy_from_array_elem__arr+24
   LDA   #$FF
   STA   __local_test_copy_from_array_elem__arr+7
   STA   __local_test_copy_from_array_elem__arr+8
   STA   __local_test_copy_from_array_elem__arr+16
   STA   __local_test_copy_from_array_elem__arr+17
   STA   __local_test_copy_from_array_elem__arr+25
   STA   __local_test_copy_from_array_elem__arr+26
   LDY   #$09
   LDA   #$00
.clear@28:
   DEY
   STA   __local_test_copy_from_array_elem__x,Y
   BNE   .clear@28
   LDA   #>__local_test_copy_from_array_elem__arr
   STA   __local_test_copy_from_array_elem__1
   LDA   #<__local_test_copy_from_array_elem__arr
   CLC
   ADC   #$09
   STA   __local_test_copy_from_array_elem__0
   BCC   .carry_inc@0
   INC   __local_test_copy_from_array_elem__1
.carry_inc@0:
   LDY   #$00
   LDA   (__local_test_copy_from_array_elem__0),Y
   STA   __local_test_copy_from_array_elem__x
   INY
   LDA   (__local_test_copy_from_array_elem__0),Y
   STA   __local_test_copy_from_array_elem__x+1
   INY
   LDA   (__local_test_copy_from_array_elem__0),Y
   STA   __local_test_copy_from_array_elem__x+2
   INY
   LDA   (__local_test_copy_from_array_elem__0),Y
   STA   __local_test_copy_from_array_elem__x+3
   INY
   LDA   (__local_test_copy_from_array_elem__0),Y
   STA   __local_test_copy_from_array_elem__x+4
   INY
   LDA   (__local_test_copy_from_array_elem__0),Y
   STA   __local_test_copy_from_array_elem__x+5
   INY
   LDA   (__local_test_copy_from_array_elem__0),Y
   STA   __local_test_copy_from_array_elem__x+6
   INY
   LDA   (__local_test_copy_from_array_elem__0),Y
   STA   __local_test_copy_from_array_elem__x+7
   INY
   LDA   (__local_test_copy_from_array_elem__0),Y
   STA   __local_test_copy_from_array_elem__x+8
   LDA   #<str@4
   STA   __zpabi_strcmp__s2_0
   LDA   #>str@4
   STA   __zpabi_strcmp__s2_1
   LDX   #>__local_test_copy_from_array_elem__x
   LDA   #<__local_test_copy_from_array_elem__x
   JSR   strcmp
   STX   __local_test_copy_from_array_elem__0
   ORA   __local_test_copy_from_array_elem__0
   BNE   .test_copy_from_array_elem@jthread@1
   LDA   __local_test_copy_from_array_elem__x+3
   STA   __local_test_copy_from_array_elem__1
   LDA   __local_test_copy_from_array_elem__x+4
   CMP   #$EC
   BNE   .test_copy_from_array_elem@jthread@1
   LDA   __local_test_copy_from_array_elem__1
   CMP   #$78
   BNE   .test_copy_from_array_elem@jthread@1
   LDA   __local_test_copy_from_array_elem__x+5
   STA   __local_test_copy_from_array_elem__3
   LDA   __local_test_copy_from_array_elem__x+6
   STA   __local_test_copy_from_array_elem__2
   LDA   __local_test_copy_from_array_elem__x+7
   STA   __local_test_copy_from_array_elem__1
   LDA   __local_test_copy_from_array_elem__x+8
   CMP   #$FF
   BNE   .test_copy_from_array_elem@jthread@1
   LDA   __local_test_copy_from_array_elem__1
   CMP   #$FF
   BNE   .test_copy_from_array_elem@jthread@1
   LDA   __local_test_copy_from_array_elem__2
   CMP   #$E8
   BNE   .test_copy_from_array_elem@jthread@1
   LDA   __local_test_copy_from_array_elem__3
   CMP   #$90
   BEQ   .if_end@33
.test_copy_from_array_elem@jthread@1:
   JMP   test_copy_to_pointer@tail1
.if_end@33:
   JMP   test_copy_to_pointer@tail0

test_copy_to_and_from_array_elem:
   SUBROUTINE

   LDA   #$61
   STA   __local_test_copy_to_and_from_array_elem__arr
   LDA   #$62
   STA   __local_test_copy_to_and_from_array_elem__arr+1
   LDA   #$00
   STA   __local_test_copy_to_and_from_array_elem__arr+2
   STA   __local_test_copy_to_and_from_array_elem__arr+11
   STA   __local_test_copy_to_and_from_array_elem__arr+20
   LDA   #$48
   STA   __local_test_copy_to_and_from_array_elem__arr+3
   LDA   #$F4
   STA   __local_test_copy_to_and_from_array_elem__arr+4
   LDA   #$60
   STA   __local_test_copy_to_and_from_array_elem__arr+5
   LDA   #$F0
   STA   __local_test_copy_to_and_from_array_elem__arr+6
   LDA   #$FF
   STA   __local_test_copy_to_and_from_array_elem__arr+7
   STA   __local_test_copy_to_and_from_array_elem__arr+8
   STA   __local_test_copy_to_and_from_array_elem__arr+16
   STA   __local_test_copy_to_and_from_array_elem__arr+17
   STA   __local_test_copy_to_and_from_array_elem__arr+25
   STA   __local_test_copy_to_and_from_array_elem__arr+26
   LDA   #$63
   STA   __local_test_copy_to_and_from_array_elem__arr+9
   LDA   #$64
   STA   __local_test_copy_to_and_from_array_elem__arr+10
   LDA   #$78
   STA   __local_test_copy_to_and_from_array_elem__arr+12
   LDA   #$EC
   STA   __local_test_copy_to_and_from_array_elem__arr+13
   LDA   #$90
   STA   __local_test_copy_to_and_from_array_elem__arr+14
   LDA   #$E8
   STA   __local_test_copy_to_and_from_array_elem__arr+15
   LDA   #$65
   STA   __local_test_copy_to_and_from_array_elem__arr+18
   LDA   #$66
   STA   __local_test_copy_to_and_from_array_elem__arr+19
   LDA   #$A8
   STA   __local_test_copy_to_and_from_array_elem__arr+21
   LDA   #$E4
   STA   __local_test_copy_to_and_from_array_elem__arr+22
   LDA   #$C0
   STA   __local_test_copy_to_and_from_array_elem__arr+23
   LDA   #$E0
   STA   __local_test_copy_to_and_from_array_elem__arr+24
   LDA   #>__local_test_copy_to_and_from_array_elem__arr
   STA   __local_test_copy_to_and_from_array_elem__1
   LDA   #<__local_test_copy_to_and_from_array_elem__arr
   CLC
   ADC   #$12
   STA   __local_test_copy_to_and_from_array_elem__0
   BCC   .carry_inc@1
   INC   __local_test_copy_to_and_from_array_elem__1
.carry_inc@1:
   LDY   #$00
   LDA   (__local_test_copy_to_and_from_array_elem__0),Y
   STA   __local_test_copy_to_and_from_array_elem__10
   INY
   LDA   (__local_test_copy_to_and_from_array_elem__0),Y
   STA   __local_test_copy_to_and_from_array_elem__9
   INY
   LDA   (__local_test_copy_to_and_from_array_elem__0),Y
   STA   __local_test_copy_to_and_from_array_elem__8
   INY
   LDA   (__local_test_copy_to_and_from_array_elem__0),Y
   STA   __local_test_copy_to_and_from_array_elem__7
   INY
   LDA   (__local_test_copy_to_and_from_array_elem__0),Y
   STA   __local_test_copy_to_and_from_array_elem__6
   INY
   LDA   (__local_test_copy_to_and_from_array_elem__0),Y
   STA   __local_test_copy_to_and_from_array_elem__5
   INY
   LDA   (__local_test_copy_to_and_from_array_elem__0),Y
   STA   __local_test_copy_to_and_from_array_elem__4
   INY
   LDA   (__local_test_copy_to_and_from_array_elem__0),Y
   STA   __local_test_copy_to_and_from_array_elem__3
   INY
   LDA   (__local_test_copy_to_and_from_array_elem__0),Y
   STA   __local_test_copy_to_and_from_array_elem__2
   LDA   #>__local_test_copy_to_and_from_array_elem__arr
   STA   __local_test_copy_to_and_from_array_elem__1
   LDA   #<__local_test_copy_to_and_from_array_elem__arr
   CLC
   STA   __local_test_copy_to_and_from_array_elem__0
   BCC   .carry_inc@2
   INC   __local_test_copy_to_and_from_array_elem__1
.carry_inc@2:
   STA   DPTR
   LDA   __local_test_copy_to_and_from_array_elem__1
   STA   DPTR+1
   LDA   __local_test_copy_to_and_from_array_elem__10
   LDY   #$00
   STA   (__local_test_copy_to_and_from_array_elem__0),Y
   LDA   __local_test_copy_to_and_from_array_elem__9
   INY
   STA   (DPTR),Y
   LDA   __local_test_copy_to_and_from_array_elem__8
   INY
   STA   (DPTR),Y
   LDA   __local_test_copy_to_and_from_array_elem__7
   INY
   STA   (DPTR),Y
   LDA   __local_test_copy_to_and_from_array_elem__6
   INY
   STA   (DPTR),Y
   LDA   __local_test_copy_to_and_from_array_elem__5
   INY
   STA   (DPTR),Y
   LDA   __local_test_copy_to_and_from_array_elem__4
   INY
   STA   (DPTR),Y
   LDA   __local_test_copy_to_and_from_array_elem__3
   INY
   STA   (DPTR),Y
   LDA   __local_test_copy_to_and_from_array_elem__2
   INY
   STA   (DPTR),Y
   LDA   #>__local_test_copy_to_and_from_array_elem__arr
   STA   __local_test_copy_to_and_from_array_elem__2
   CLC
   BCC   .carry_inc@3
   INC   __local_test_copy_to_and_from_array_elem__2
.carry_inc@3:
   LDA   #<str@5
   STA   __zpabi_strcmp__s2_0
   LDA   #>str@5
   STA   __zpabi_strcmp__s2_1
   LDX   __local_test_copy_to_and_from_array_elem__2
   LDA   #<__local_test_copy_to_and_from_array_elem__arr
   JSR   strcmp
   STX   __local_test_copy_to_and_from_array_elem__0
   ORA   __local_test_copy_to_and_from_array_elem__0
   BNE   .test_copy_to_and_from_array_elem@jthread@1
   LDA   __local_test_copy_to_and_from_array_elem__arr+3
   STA   __local_test_copy_to_and_from_array_elem__1
   LDA   __local_test_copy_to_and_from_array_elem__arr+4
   CMP   #$E4
   BNE   .test_copy_to_and_from_array_elem@jthread@1
   LDA   __local_test_copy_to_and_from_array_elem__1
   CMP   #$A8
   BNE   .test_copy_to_and_from_array_elem@jthread@1
   LDA   __local_test_copy_to_and_from_array_elem__arr+5
   STA   __local_test_copy_to_and_from_array_elem__3
   LDA   __local_test_copy_to_and_from_array_elem__arr+6
   STA   __local_test_copy_to_and_from_array_elem__2
   LDA   __local_test_copy_to_and_from_array_elem__arr+7
   STA   __local_test_copy_to_and_from_array_elem__1
   LDA   __local_test_copy_to_and_from_array_elem__arr+8
   CMP   #$FF
   BNE   .test_copy_to_and_from_array_elem@jthread@1
   LDA   __local_test_copy_to_and_from_array_elem__1
   CMP   #$FF
   BNE   .test_copy_to_and_from_array_elem@jthread@1
   LDA   __local_test_copy_to_and_from_array_elem__2
   CMP   #$E0
   BNE   .test_copy_to_and_from_array_elem@jthread@1
   LDA   __local_test_copy_to_and_from_array_elem__3
   CMP   #$C0
   BEQ   .if_end@38
.test_copy_to_and_from_array_elem@jthread@1:
   JMP   test_copy_to_pointer@tail1
.if_end@38:
   LDA   #>__local_test_copy_to_and_from_array_elem__arr
   STA   __local_test_copy_to_and_from_array_elem__2
   LDA   #<__local_test_copy_to_and_from_array_elem__arr
   CLC
   ADC   #$09
   STA   __local_test_copy_to_and_from_array_elem__0
   BCC   .carry_inc@4
   INC   __local_test_copy_to_and_from_array_elem__2
.carry_inc@4:
   LDA   #<str@6
   STA   __zpabi_strcmp__s2_0
   LDA   #>str@6
   STA   __zpabi_strcmp__s2_1
   LDX   __local_test_copy_to_and_from_array_elem__2
   LDA   __local_test_copy_to_and_from_array_elem__0
   JSR   strcmp
   STX   __local_test_copy_to_and_from_array_elem__0
   ORA   __local_test_copy_to_and_from_array_elem__0
   BNE   .test_copy_to_and_from_array_elem@jthread@3
   LDA   __local_test_copy_to_and_from_array_elem__arr+12
   STA   __local_test_copy_to_and_from_array_elem__1
   LDA   __local_test_copy_to_and_from_array_elem__arr+13
   CMP   #$EC
   BNE   .test_copy_to_and_from_array_elem@jthread@3
   LDA   __local_test_copy_to_and_from_array_elem__1
   CMP   #$78
   BNE   .test_copy_to_and_from_array_elem@jthread@3
   LDA   __local_test_copy_to_and_from_array_elem__arr+14
   STA   __local_test_copy_to_and_from_array_elem__3
   LDA   __local_test_copy_to_and_from_array_elem__arr+15
   STA   __local_test_copy_to_and_from_array_elem__2
   LDA   __local_test_copy_to_and_from_array_elem__arr+16
   STA   __local_test_copy_to_and_from_array_elem__1
   LDA   __local_test_copy_to_and_from_array_elem__arr+17
   CMP   #$FF
   BNE   .test_copy_to_and_from_array_elem@jthread@3
   LDA   __local_test_copy_to_and_from_array_elem__1
   CMP   #$FF
   BNE   .test_copy_to_and_from_array_elem@jthread@3
   LDA   __local_test_copy_to_and_from_array_elem__2
   CMP   #$E8
   BNE   .test_copy_to_and_from_array_elem@jthread@3
   LDA   __local_test_copy_to_and_from_array_elem__3
   CMP   #$90
   BEQ   .if_end@43
.test_copy_to_and_from_array_elem@jthread@3:
   LDX   #$00
   TXA
   RTS
.if_end@43:
   LDA   #>__local_test_copy_to_and_from_array_elem__arr
   STA   __local_test_copy_to_and_from_array_elem__2
   LDA   #<__local_test_copy_to_and_from_array_elem__arr
   CLC
   ADC   #$12
   STA   __local_test_copy_to_and_from_array_elem__0
   BCC   .carry_inc@5
   INC   __local_test_copy_to_and_from_array_elem__2
.carry_inc@5:
   LDA   #<str@7
   STA   __zpabi_strcmp__s2_0
   LDA   #>str@7
   STA   __zpabi_strcmp__s2_1
   LDX   __local_test_copy_to_and_from_array_elem__2
   LDA   __local_test_copy_to_and_from_array_elem__0
   JSR   strcmp
   STX   __local_test_copy_to_and_from_array_elem__0
   ORA   __local_test_copy_to_and_from_array_elem__0
   BNE   .test_copy_to_and_from_array_elem@jthread@5
   LDA   __local_test_copy_to_and_from_array_elem__arr+21
   STA   __local_test_copy_to_and_from_array_elem__1
   LDA   __local_test_copy_to_and_from_array_elem__arr+22
   CMP   #$E4
   BNE   .test_copy_to_and_from_array_elem@jthread@5
   LDA   __local_test_copy_to_and_from_array_elem__1
   CMP   #$A8
   BNE   .test_copy_to_and_from_array_elem@jthread@5
   LDA   __local_test_copy_to_and_from_array_elem__arr+23
   STA   __local_test_copy_to_and_from_array_elem__3
   LDA   __local_test_copy_to_and_from_array_elem__arr+24
   STA   __local_test_copy_to_and_from_array_elem__2
   LDA   __local_test_copy_to_and_from_array_elem__arr+25
   STA   __local_test_copy_to_and_from_array_elem__1
   LDA   __local_test_copy_to_and_from_array_elem__arr+26
   CMP   #$FF
   BNE   .test_copy_to_and_from_array_elem@jthread@5
   LDA   __local_test_copy_to_and_from_array_elem__1
   CMP   #$FF
   BNE   .test_copy_to_and_from_array_elem@jthread@5
   LDA   __local_test_copy_to_and_from_array_elem__2
   CMP   #$E0
   BNE   .test_copy_to_and_from_array_elem@jthread@5
   LDA   __local_test_copy_to_and_from_array_elem__3
   CMP   #$C0
   BEQ   .if_end@48
.test_copy_to_and_from_array_elem@jthread@5:
   LDX   #$00
   TXA
   RTS
.if_end@48:
   JMP   test_copy_to_pointer@tail0

test_copy_array_element_with_padding:
   SUBROUTINE

   LDA   #$00
   STA   __local_test_copy_array_element_with_padding__arr
   STA   __local_test_copy_array_element_with_padding__arr+1
   LDY   #$01
   STY   __local_test_copy_array_element_with_padding__arr+2
   STA   __local_test_copy_array_element_with_padding__arr+3
   LDA   #$02
   STA   __local_test_copy_array_element_with_padding__arr+4
   LDA   #$03
   STA   __local_test_copy_array_element_with_padding__arr+5
   LDA   #$00
   STA   __local_test_copy_array_element_with_padding__arr+6
   LDY   #$04
   STY   __local_test_copy_array_element_with_padding__arr+7
   STA   __local_test_copy_array_element_with_padding__arr+8
   LDA   #$05
   STA   __local_test_copy_array_element_with_padding__arr+9
   LDA   #$06
   STA   __local_test_copy_array_element_with_padding__arr+10
   LDA   #$00
   STA   __local_test_copy_array_element_with_padding__arr+11
   LDY   #$07
   STY   __local_test_copy_array_element_with_padding__arr+12
   STA   __local_test_copy_array_element_with_padding__arr+13
   LDA   #$08
   STA   __local_test_copy_array_element_with_padding__arr+14
   LDA   #$09
   LDY   #$00
   STY   __local_test_copy_array_element_with_padding__elem_1
   STA   __local_test_copy_array_element_with_padding__elem_2
   STY   __local_test_copy_array_element_with_padding__elem_3
   STA   __local_test_copy_array_element_with_padding__elem_4
   LDA   #>__local_test_copy_array_element_with_padding__arr
   STA   __local_test_copy_array_element_with_padding__1
   LDA   #<__local_test_copy_array_element_with_padding__arr
   CLC
   ADC   #$05
   STA   __local_test_copy_array_element_with_padding__0
   BCC   .carry_inc@6
   INC   __local_test_copy_array_element_with_padding__1
.carry_inc@6:
   STA   DPTR
   LDA   __local_test_copy_array_element_with_padding__1
   STA   DPTR+1
   LDA   #$09
   LDY   #$00
   STA   (__local_test_copy_array_element_with_padding__0),Y
   LDA   __local_test_copy_array_element_with_padding__elem_1
   INY
   STA   (DPTR),Y
   LDA   __local_test_copy_array_element_with_padding__elem_2
   INY
   STA   (DPTR),Y
   LDA   __local_test_copy_array_element_with_padding__elem_3
   INY
   STA   (DPTR),Y
   LDA   __local_test_copy_array_element_with_padding__elem_4
   INY
   STA   (DPTR),Y
   LDA   __local_test_copy_array_element_with_padding__arr
   ORA   __local_test_copy_array_element_with_padding__arr+1
   BNE   .test_copy_array_element_with_padding@jthread@7
   LDA   __local_test_copy_array_element_with_padding__arr+2
   STA   __local_test_copy_array_element_with_padding__1
   LDA   __local_test_copy_array_element_with_padding__arr+3
   BNE   .test_copy_array_element_with_padding@jthread@7
   LDA   __local_test_copy_array_element_with_padding__1
   CMP   #$01
   BNE   .test_copy_array_element_with_padding@jthread@7
   LDA   __local_test_copy_array_element_with_padding__arr+4
   CMP   #$02
   BNE   .test_copy_array_element_with_padding@jthread@7
   LDA   __local_test_copy_array_element_with_padding__arr+5
   STA   __local_test_copy_array_element_with_padding__1
   LDA   __local_test_copy_array_element_with_padding__arr+6
   BNE   .test_copy_array_element_with_padding@jthread@7
   LDA   __local_test_copy_array_element_with_padding__1
   CMP   #$09
   BNE   .test_copy_array_element_with_padding@jthread@7
   LDA   __local_test_copy_array_element_with_padding__arr+7
   STA   __local_test_copy_array_element_with_padding__1
   LDA   __local_test_copy_array_element_with_padding__arr+8
   BNE   .test_copy_array_element_with_padding@jthread@7
   LDA   __local_test_copy_array_element_with_padding__1
   CMP   #$09
   BNE   .test_copy_array_element_with_padding@jthread@7
   LDA   __local_test_copy_array_element_with_padding__arr+9
   CMP   #$09
   BNE   .test_copy_array_element_with_padding@jthread@7
   LDA   __local_test_copy_array_element_with_padding__arr+10
   STA   __local_test_copy_array_element_with_padding__1
   LDA   __local_test_copy_array_element_with_padding__arr+11
   BNE   .test_copy_array_element_with_padding@jthread@7
   LDA   __local_test_copy_array_element_with_padding__1
   CMP   #$06
   BNE   .test_copy_array_element_with_padding@jthread@7
   LDA   __local_test_copy_array_element_with_padding__arr+12
   STA   __local_test_copy_array_element_with_padding__1
   LDA   __local_test_copy_array_element_with_padding__arr+13
   BNE   .test_copy_array_element_with_padding@jthread@7
   LDA   __local_test_copy_array_element_with_padding__1
   CMP   #$07
   BNE   .test_copy_array_element_with_padding@jthread@7
   LDA   __local_test_copy_array_element_with_padding__arr+14
   CMP   #$08
   BEQ   .if_end@65
.test_copy_array_element_with_padding@jthread@7:
   JMP   test_copy_to_pointer@tail1
.if_end@65:
   JMP   test_copy_to_pointer@tail0

main:
   SUBROUTINE

   JSR   test_copy_to_pointer
   STX   __local_main__0
   ORA   __local_main__0
   BNE   .if_end@66
   JMP   test_copy_to_pointer@tail0
.if_end@66:
   JSR   test_copy_from_pointer
   STX   __local_main__0
   ORA   __local_main__0
   BNE   .if_end@67
   LDX   #$00
   LDA   #$02
   RTS
.if_end@67:
   JSR   test_copy_to_and_from_pointer
   STX   __local_main__0
   ORA   __local_main__0
   BNE   .if_end@68
   LDX   #$00
   LDA   #$03
   RTS
.if_end@68:
   JSR   test_copy_to_array_elem
   STX   __local_main__0
   ORA   __local_main__0
   BNE   .if_end@69
   LDX   #$00
   LDA   #$04
   RTS
.if_end@69:
   JSR   test_copy_from_array_elem
   STX   __local_main__0
   ORA   __local_main__0
   BNE   .if_end@70
   LDX   #$00
   LDA   #$05
   RTS
.if_end@70:
   JSR   test_copy_to_and_from_array_elem
   STX   __local_main__0
   ORA   __local_main__0
   BNE   .if_end@71
   LDX   #$00
   LDA   #$06
   RTS
.if_end@71:
   JSR   test_copy_array_element_with_padding
   STX   __local_main__0
   ORA   __local_main__0
   BNE   .if_end@72
   LDX   #$00
   LDA   #$07
   RTS
.if_end@72:
   JMP   test_copy_to_pointer@tail1

str@0:
   DC.B  $21, $3F, $00

str@1:
   DC.B  $28, $29, $00

str@2:
   DC.B  $2B, $2D, $00

str@3:
   DC.B  $0A, $09, $00

str@4:
   DC.B  $63, $64, $00

str@5:
   DC.B  $65, $66, $00

str@6:
   DC.B  $63, $64, $00

str@7:
   DC.B  $65, $66, $00

@5.my_struct:
   DC.B  $28, $29, $00
   DC.W  $004D
   DC.L  $0000004E

@12.arr:
   DS.B  27
