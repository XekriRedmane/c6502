SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__zpabi_check_char_ptr_argument__expected_val	EQU	$80
__zpabi_get_dbl_array__n_0	EQU	$80
__zpabi_memcmp__s2_0	EQU	$80
__zpabi_set_doubles__n_0	EQU	$80
__local_check_char_ptr_argument__pointer	EQU	$81
__zpabi_get_dbl_array__n_1	EQU	$81
__zpabi_memcmp__s2_1	EQU	$81
__zpabi_set_doubles__n_1	EQU	$81
__local_check_char_ptr_argument__0	EQU	$82
__zpabi_get_dbl_array__n_2	EQU	$82
__zpabi_memcmp__n_0	EQU	$82
__zpabi_set_doubles__n_2	EQU	$82
__zpabi_get_dbl_array__n_3	EQU	$83
__zpabi_memcmp__n_1	EQU	$83
__zpabi_set_doubles__n_3	EQU	$83
__zpabi_malloc__size_0	EQU	$84
__zpabi_memcmp__n_2	EQU	$84
__zpabi_set_doubles__d_0	EQU	$84
__zpabi_malloc__size_1	EQU	$85
__zpabi_memcmp__n_3	EQU	$85
__zpabi_set_doubles__d_1	EQU	$85
__zpabi_malloc__size_2	EQU	$86
__zpabi_set_doubles__d_2	EQU	$86
__zpabi_malloc__size_3	EQU	$87
__zpabi_set_doubles__d_3	EQU	$87
__zpabi_set_doubles__d_4	EQU	$88
__zpabi_set_doubles__d_5	EQU	$89
__zpabi_set_doubles__d_6	EQU	$8A
__zpabi_set_doubles__d_7	EQU	$8B
__local_main__0	EQU	$8C
__local_main__1	EQU	$8D
__local_main__2	EQU	$8E
__local_main__3	EQU	$8F
__local_main__4	EQU	$90
__local_main__5	EQU	$91
__local_main__6	EQU	$92
__local_main__7	EQU	$93
__local_main__long_ptr_array_4	EQU	$94
__local_main__long_ptr_array_5	EQU	$95
__local_main__long_ptr_array_2	EQU	$96
__local_main__long_ptr_array_3	EQU	$97
__local_main__long_ptr_array_0	EQU	$98
__local_main__long_ptr_array_1	EQU	$99
__local_main__some_other_ptr	EQU	$9A
__local_main__arr1	EQU	$9C
__local_main__arr2	EQU	$A8
__local_main__arr3	EQU	$B4
__local_set_doubles__0	EQU	$C0
__local_set_doubles__1	EQU	$C1
__local_set_doubles__2	EQU	$C2
__local_set_doubles__3	EQU	$C3
__local_set_doubles__i_3	EQU	$C4
__local_set_doubles__i_2	EQU	$C5
__local_set_doubles__i_0	EQU	$C6
__local_set_doubles__array_1	EQU	$C7
__local_set_doubles__array_0	EQU	$C8

; @zp-link-meta-begin
; def check_char_ptr_argument params=__zpabi_check_char_ptr_argument__pointer_0,__zpabi_check_char_ptr_argument__pointer_1,__zpabi_check_char_ptr_argument__expected_val locals=__local_check_char_ptr_argument__pointer,__local_check_char_ptr_argument__0 indirect=false in_cycle=false param_regs=A,X,
; def get_dbl_array params=__zpabi_get_dbl_array__n_0,__zpabi_get_dbl_array__n_1,__zpabi_get_dbl_array__n_2,__zpabi_get_dbl_array__n_3 locals=__local_get_dbl_array__0 indirect=false in_cycle=false
; def main params= locals=__local_main__0,__local_main__1,__local_main__2,__local_main__3,__local_main__4,__local_main__5,__local_main__6,__local_main__7,__local_main__long_ptr_array_4,__local_main__long_ptr_array_5,__local_main__long_ptr_array_2,__local_main__long_ptr_array_3,__local_main__long_ptr_array_0,__local_main__long_ptr_array_1,__local_main__some_other_ptr,__local_main__9,__local_main__arr1,__local_main__11,__local_main__12,__local_main__13,__local_main__14,__local_main__15,__local_main__16,__local_main__17,__local_main__18,__local_main__19,__local_main__20,__local_main__21,__local_main__arr2,__local_main__23,__local_main__24,__local_main__25,__local_main__26,__local_main__27,__local_main__28,__local_main__29,__local_main__30,__local_main__31,__local_main__32,__local_main__33,__local_main__arr3,__local_main__35,__local_main__36,__local_main__37,__local_main__38,__local_main__39,__local_main__40,__local_main__41,__local_main__42,__local_main__43,__local_main__44,__local_main__45 indirect=false in_cycle=false
; def return_dbl_ptr_as_void_ptr params=__zpabi_return_dbl_ptr_as_void_ptr__ptr_0,__zpabi_return_dbl_ptr_as_void_ptr__ptr_1 locals=__local_return_dbl_ptr_as_void_ptr__ptr indirect=false in_cycle=false param_regs=A,X
; def return_ptr params=__zpabi_return_ptr__i_0,__zpabi_return_ptr__i_1 locals=__local_return_ptr__i indirect=false in_cycle=false param_regs=A,X
; def return_void_ptr_as_int_ptr params=__zpabi_return_void_ptr_as_int_ptr__pointer_0,__zpabi_return_void_ptr_as_int_ptr__pointer_1 locals=__local_return_void_ptr_as_int_ptr__pointer indirect=false in_cycle=false param_regs=A,X
; def set_doubles params=__zpabi_set_doubles__array_0,__zpabi_set_doubles__array_1,__zpabi_set_doubles__n_0,__zpabi_set_doubles__n_1,__zpabi_set_doubles__n_2,__zpabi_set_doubles__n_3,__zpabi_set_doubles__d_0,__zpabi_set_doubles__d_1,__zpabi_set_doubles__d_2,__zpabi_set_doubles__d_3,__zpabi_set_doubles__d_4,__zpabi_set_doubles__d_5,__zpabi_set_doubles__d_6,__zpabi_set_doubles__d_7 locals=__local_set_doubles__0,__local_set_doubles__1,__local_set_doubles__2,__local_set_doubles__3,__local_set_doubles__i_3,__local_set_doubles__i_2,__local_set_doubles__i_0,__local_set_doubles__array_1,__local_set_doubles__array_0 indirect=false in_cycle=false param_regs=A,X,,,,,,,,,,,,
; ext free params=__zpabi_free__ptr_0,__zpabi_free__ptr_1 param_regs=A,X
; ext malloc params=__zpabi_malloc__size_0,__zpabi_malloc__size_1,__zpabi_malloc__size_2,__zpabi_malloc__size_3
; ext memcmp params=__zpabi_memcmp__s1_0,__zpabi_memcmp__s1_1,__zpabi_memcmp__s2_0,__zpabi_memcmp__s2_1,__zpabi_memcmp__n_0,__zpabi_memcmp__n_1,__zpabi_memcmp__n_2,__zpabi_memcmp__n_3 param_regs=A,X,,,,,,
; call get_dbl_array -> malloc
; call main -> check_char_ptr_argument
; call main -> free
; call main -> get_dbl_array
; call main -> malloc
; call main -> memcmp
; call main -> return_dbl_ptr_as_void_ptr
; call main -> return_void_ptr_as_int_ptr
; call main -> set_doubles
; item return_ptr section=.text local=false zero=false
; item check_char_ptr_argument section=.text local=false zero=false
; item return_void_ptr_as_int_ptr section=.text local=false zero=false
; item get_dbl_array section=.text local=false zero=false
; item set_doubles section=.text local=false zero=false
; item return_dbl_ptr_as_void_ptr section=.text local=false zero=false
; item main section=.text local=false zero=false
; item main@run1 section=.text local=true zero=false
; item main@run0 section=.text local=true zero=false
; runtime hargs_size=32 hargs_need=9
; @zp-link-meta-end

return_ptr:
   SUBROUTINE

   CLC
   ADC   #$03
   BCC   .carry_inc@0
   INX
.carry_inc@0:
   RTS

check_char_ptr_argument:
   SUBROUTINE

   STA   __local_check_char_ptr_argument__pointer
   STX   __local_check_char_ptr_argument__0
   LDY   #$00
   LDA   (__local_check_char_ptr_argument__pointer),Y
   CMP   __zpabi_check_char_ptr_argument__expected_val
   BEQ   .cmp_true@0
   LDA   #$00
   BEQ   .cmp_end@1
.cmp_true@0:
   LDA   #$01
.cmp_end@1:
   LDX   #$00
   RTS

return_void_ptr_as_int_ptr:
   SUBROUTINE

   RTS

get_dbl_array:
   SUBROUTINE

   LDA   __zpabi_get_dbl_array__n_0
   STA   HARGS
   LDA   __zpabi_get_dbl_array__n_1
   STA   HARGS+1
   LDA   __zpabi_get_dbl_array__n_2
   STA   HARGS+2
   LDA   __zpabi_get_dbl_array__n_3
   STA   HARGS+3
   LDA   #$03
   STA   HARGS+4
   JSR   asl32
   LDA   HARGS+5
   STA   __zpabi_malloc__size_0
   LDA   HARGS+6
   STA   __zpabi_malloc__size_1
   LDA   HARGS+7
   STA   __zpabi_malloc__size_2
   LDA   HARGS+8
   STA   __zpabi_malloc__size_3
   JMP   malloc

set_doubles:
   SUBROUTINE

   STA   __local_set_doubles__array_0
   STX   __local_set_doubles__array_1
   LDA   #$00
   STA   __local_set_doubles__i_0
   LDX   #$00
   STA   __local_set_doubles__i_2
   LDY   #$00
   STY   __local_set_doubles__i_3
.loop@0_start:
   LDA   __local_set_doubles__i_0
   SEC
   SBC   __zpabi_set_doubles__n_0
   TXA
   SBC   __zpabi_set_doubles__n_1
   LDA   __local_set_doubles__i_2
   SBC   __zpabi_set_doubles__n_2
   LDA   __local_set_doubles__i_3
   SBC   __zpabi_set_doubles__n_3
   BCC   .lb_skip@3
   JMP   .loop@0_break
.lb_skip@3:
   LDA   __local_set_doubles__i_0
   ASL
   STA   __local_set_doubles__2
   STX   __local_set_doubles__3
   ROL   __local_set_doubles__3
   ASL   __local_set_doubles__2
   ROL   __local_set_doubles__3
   ASL   __local_set_doubles__2
   ROL   __local_set_doubles__3
   LDA   __local_set_doubles__array_0
   CLC
   ADC   __local_set_doubles__2
   STA   __local_set_doubles__0
   LDA   __local_set_doubles__array_1
   ADC   __local_set_doubles__3
   STA   __local_set_doubles__1
   LDA   __zpabi_set_doubles__d_0
   LDY   #$00
   STA   (__local_set_doubles__0),Y
   LDA   __zpabi_set_doubles__d_1
   INY
   STA   (__local_set_doubles__0),Y
   LDA   __zpabi_set_doubles__d_2
   INY
   STA   (__local_set_doubles__0),Y
   LDA   __zpabi_set_doubles__d_3
   INY
   STA   (__local_set_doubles__0),Y
   LDA   __zpabi_set_doubles__d_4
   INY
   STA   (__local_set_doubles__0),Y
   LDA   __zpabi_set_doubles__d_5
   INY
   STA   (__local_set_doubles__0),Y
   LDA   __zpabi_set_doubles__d_6
   INY
   STA   (__local_set_doubles__0),Y
   LDA   __zpabi_set_doubles__d_7
   INY
   STA   (__local_set_doubles__0),Y
   INC   __local_set_doubles__i_0
   BEQ   .lb_skip@2
   JMP   .loop@0_start
.lb_skip@2:
   INX
   BEQ   .lb_skip@1
   JMP   .loop@0_start
.lb_skip@1:
   INC   __local_set_doubles__i_2
   BEQ   .lb_skip@0
   JMP   .loop@0_start
.lb_skip@0:
   INC   __local_set_doubles__i_3
   JMP   .loop@0_start
.loop@0_break:
   RTS

return_dbl_ptr_as_void_ptr:
   SUBROUTINE

   RTS

main:
   SUBROUTINE

   JSR   main@run0
   STA   __local_main__2
   STX   __local_main__3
   STA   DPTR
   STX   DPTR+1
   LDA   #$FF
   LDY   #$00
   STA   (__local_main__2),Y
   INY
   STA   (DPTR),Y
   STA   __zpabi_check_char_ptr_argument__expected_val
   LDA   __local_main__2
   JSR   check_char_ptr_argument
   STX   __local_main__0
   ORA   __local_main__0
   BNE   .if_end@0
   LDX   #$00
   LDA   #$01
   RTS
.if_end@0:
   LDX   __local_main__3
   LDA   __local_main__2
   JSR   return_void_ptr_as_int_ptr
   CPX   __local_main__3
   BNE   .jcmp_skip@2
   CMP   __local_main__2
   BEQ   .if_end@1
.jcmp_skip@2:
   LDX   #$00
   LDA   #$02
   RTS
.if_end@1:
   LDA   __local_main__3
   CMP   __local_main__3
   BNE   .main@jthread@1
   LDA   __local_main__2
   CMP   __local_main__2
   BNE   .main@jthread@1
   LDA   __local_main__3
   CMP   __local_main__3
   BNE   .main@jthread@1
   LDA   __local_main__2
   CMP   __local_main__2
   BNE   .main@jthread@1
   LDA   __local_main__3
   CMP   __local_main__3
   BNE   .main@jthread@1
   LDA   __local_main__2
   CMP   __local_main__2
   BEQ   .if_end@6
.main@jthread@1:
   LDX   #$00
   LDA   #$03
   RTS
.if_end@6:
   LDX   __local_main__3
   JSR   free
   LDA   #$05
   STA   __zpabi_get_dbl_array__n_0
   LDA   #$00
   STA   __zpabi_get_dbl_array__n_1
   STA   __zpabi_get_dbl_array__n_2
   STA   __zpabi_get_dbl_array__n_3
   JSR   get_dbl_array
   STA   __local_main__long_ptr_array_4
   STX   __local_main__long_ptr_array_5
   LDA   #$05
   STA   __zpabi_set_doubles__n_0
   LDA   #$00
   STA   __zpabi_set_doubles__n_1
   STA   __zpabi_set_doubles__n_2
   STA   __zpabi_set_doubles__n_3
   STA   __zpabi_set_doubles__d_0
   STA   __zpabi_set_doubles__d_1
   STA   __zpabi_set_doubles__d_2
   STA   __zpabi_set_doubles__d_3
   STA   __zpabi_set_doubles__d_4
   STA   __zpabi_set_doubles__d_5
   LDA   #$10
   STA   __zpabi_set_doubles__d_6
   LDA   #$40
   STA   __zpabi_set_doubles__d_7
   LDA   __local_main__long_ptr_array_4
   JSR   set_doubles
   LDY   #$18
   LDA   (__local_main__long_ptr_array_4),Y
   STA   __local_main__7
   INY
   LDA   (__local_main__long_ptr_array_4),Y
   STA   __local_main__6
   INY
   LDA   (__local_main__long_ptr_array_4),Y
   STA   __local_main__5
   INY
   LDA   (__local_main__long_ptr_array_4),Y
   STA   __local_main__4
   INY
   LDA   (__local_main__long_ptr_array_4),Y
   STA   __local_main__3
   INY
   LDA   (__local_main__long_ptr_array_4),Y
   STA   __local_main__2
   INY
   LDA   (__local_main__long_ptr_array_4),Y
   STA   __local_main__1
   INY
   LDA   (__local_main__long_ptr_array_4),Y
   CMP   #$40
   BNE   .jcmp_skip@3
   LDA   __local_main__1
   CMP   #$10
   BNE   .jcmp_skip@3
   LDA   __local_main__2
   BNE   .jcmp_skip@3
   LDA   __local_main__3
   BNE   .jcmp_skip@3
   LDA   __local_main__4
   BNE   .jcmp_skip@3
   LDA   __local_main__5
   BNE   .jcmp_skip@3
   LDA   __local_main__6
   BNE   .jcmp_skip@3
   LDA   __local_main__7
   BEQ   .if_end@7
.jcmp_skip@3:
   LDX   #$00
   LDA   #$04
   RTS
.if_end@7:
   LDX   __local_main__long_ptr_array_5
   LDA   __local_main__long_ptr_array_4
   JSR   return_dbl_ptr_as_void_ptr
   CPX   __local_main__long_ptr_array_5
   BNE   .jcmp_skip@4
   CMP   __local_main__long_ptr_array_4
   BEQ   .if_end@8
.jcmp_skip@4:
   LDX   #$00
   LDA   #$05
   RTS
.if_end@8:
   LDA   __local_main__long_ptr_array_4
   STA   __local_main__some_other_ptr
   LDA   __local_main__long_ptr_array_5
   CMP   __local_main__long_ptr_array_5
   BNE   .jcmp_skip@5
   LDA   __local_main__some_other_ptr
   CMP   __local_main__long_ptr_array_4
   BEQ   .if_end@9
.jcmp_skip@5:
   LDX   #$00
   LDA   #$06
   RTS
.if_end@9:
   LDA   #>__local_main__some_other_ptr
   CMP   __local_main__long_ptr_array_5
   BNE   .if_end@10
   LDA   #<__local_main__some_other_ptr
   CMP   __local_main__long_ptr_array_4
   BNE   .if_end@10
   LDX   #$00
   LDA   #$07
   RTS
.if_end@10:
   LDA   #$00
   STA   __local_main__some_other_ptr
   LDY   #$00
   STY   __local_main__some_other_ptr+1
   ORA   #$00
   BEQ   .if_end@11
   LDX   #$00
   LDA   #$08
   RTS
.if_end@11:
   LDX   __local_main__long_ptr_array_5
   LDA   __local_main__long_ptr_array_4
   JSR   free
   JSR   main@run0
   STA   __local_main__long_ptr_array_0
   STX   __local_main__long_ptr_array_1
   JSR   main@run0
   STA   __local_main__long_ptr_array_2
   STX   __local_main__long_ptr_array_3
   JSR   main@run0
   STA   __local_main__long_ptr_array_4
   STX   __local_main__long_ptr_array_5
   LDA   #$64
   LDY   #$00
   STA   (__local_main__long_ptr_array_0),Y
   LDA   #$00
   INY
   STA   (__local_main__long_ptr_array_0),Y
   INY
   STA   (__local_main__long_ptr_array_0),Y
   INY
   STA   (__local_main__long_ptr_array_0),Y
   LDY   #$01
   STA   (__local_main__long_ptr_array_2),Y
   INY
   STA   (__local_main__long_ptr_array_2),Y
   INY
   STA   (__local_main__long_ptr_array_2),Y
   LDA   #$C8
   LDY   #$00
   STA   (__local_main__long_ptr_array_2),Y
   LDA   #$2C
   STA   (__local_main__long_ptr_array_4),Y
   LDA   #$01
   INY
   STA   (__local_main__long_ptr_array_4),Y
   LDA   #$00
   INY
   STA   (__local_main__long_ptr_array_4),Y
   INY
   STA   (__local_main__long_ptr_array_4),Y
   LDY   #$00
   LDA   (__local_main__long_ptr_array_2),Y
   STA   __local_main__3
   INY
   LDA   (__local_main__long_ptr_array_2),Y
   STA   __local_main__2
   INY
   LDA   (__local_main__long_ptr_array_2),Y
   STA   __local_main__1
   INY
   LDA   (__local_main__long_ptr_array_2),Y
   STA   __local_main__0
   LDY   #$00
   LDA   (__local_main__long_ptr_array_0),Y
   CLC
   ADC   __local_main__3
   STA   __local_main__7
   INY
   LDA   (__local_main__long_ptr_array_0),Y
   ADC   __local_main__2
   STA   __local_main__6
   INY
   LDA   (__local_main__long_ptr_array_0),Y
   ADC   __local_main__1
   STA   __local_main__5
   INY
   LDA   (__local_main__long_ptr_array_0),Y
   ADC   __local_main__0
   STA   __local_main__4
   LDY   #$00
   LDA   (__local_main__long_ptr_array_4),Y
   STA   __local_main__3
   INY
   LDA   (__local_main__long_ptr_array_4),Y
   STA   __local_main__2
   INY
   LDA   (__local_main__long_ptr_array_4),Y
   STA   __local_main__1
   INY
   LDA   (__local_main__long_ptr_array_4),Y
   STA   __local_main__0
   LDA   __local_main__7
   CLC
   ADC   __local_main__3
   STA   __local_main__3
   LDA   __local_main__6
   ADC   __local_main__2
   STA   __local_main__2
   LDA   __local_main__5
   ADC   __local_main__1
   STA   __local_main__1
   LDA   __local_main__4
   ADC   __local_main__0
   BNE   .jcmp_skip@6
   LDA   __local_main__1
   BNE   .jcmp_skip@6
   LDA   __local_main__2
   CMP   #$02
   BNE   .jcmp_skip@6
   LDA   __local_main__3
   CMP   #$58
   BEQ   .if_end@12
.jcmp_skip@6:
   LDX   #$00
   LDA   #$09
   RTS
.if_end@12:
   LDX   __local_main__long_ptr_array_1
   LDA   __local_main__long_ptr_array_0
   JSR   free
   LDX   __local_main__long_ptr_array_3
   LDA   __local_main__long_ptr_array_2
   JSR   free
   LDX   __local_main__long_ptr_array_5
   LDA   __local_main__long_ptr_array_4
   JSR   free
   LDA   #$01
   STA   __local_main__arr1
   LDA   #$00
   STA   __local_main__arr1+1
   LDY   #$00
   STY   __local_main__arr1+2
   STA   __local_main__arr1+3
   LDY   #$02
   STY   __local_main__arr1+4
   LDY   #$00
   STY   __local_main__arr1+5
   STY   __local_main__arr1+6
   STA   __local_main__arr1+7
   LDY   #$03
   STY   __local_main__arr1+8
   LDY   #$00
   STY   __local_main__arr1+9
   STY   __local_main__arr1+10
   STA   __local_main__arr1+11
   INY
   STY   __local_main__arr2
   DEY
   STY   __local_main__arr2+1
   STY   __local_main__arr2+2
   STA   __local_main__arr2+3
   LDY   #$02
   STY   __local_main__arr2+4
   LDY   #$00
   STY   __local_main__arr2+5
   STY   __local_main__arr2+6
   STA   __local_main__arr2+7
   LDY   #$03
   STY   __local_main__arr2+8
   LDY   #$00
   STY   __local_main__arr2+9
   STY   __local_main__arr2+10
   STA   __local_main__arr2+11
   INY
   STY   __local_main__arr3
   DEY
   STY   __local_main__arr3+1
   STY   __local_main__arr3+2
   STA   __local_main__arr3+3
   LDY   #$02
   STY   __local_main__arr3+4
   LDY   #$00
   STY   __local_main__arr3+5
   STY   __local_main__arr3+6
   STA   __local_main__arr3+7
   LDY   #$04
   STY   __local_main__arr3+8
   LDY   #$00
   STY   __local_main__arr3+9
   STY   __local_main__arr3+10
   STA   __local_main__arr3+11
   LDA   #<__local_main__arr2
   STA   __zpabi_memcmp__s2_0
   LDA   #>__local_main__arr2
   JSR   main@run1
   JSR   memcmp
   STX   __local_main__0
   ORA   __local_main__0
   BEQ   .if_end@13
   LDX   #$00
   LDA   #$0A
   RTS
.if_end@13:
   LDA   #<__local_main__arr3
   STA   __zpabi_memcmp__s2_0
   LDA   #>__local_main__arr3
   JSR   main@run1
   JSR   memcmp
   CPX   #$FF
   BNE   .jcmp_skip@7
   CMP   #$FF
   BEQ   .if_end@14
.jcmp_skip@7:
   LDX   #$00
   LDA   #$0B
   RTS
.if_end@14:
   LDX   #$00
   TXA
   RTS

main@run1:
   SUBROUTINE

   STA   __zpabi_memcmp__s2_1
   LDA   #$0C
   STA   __zpabi_memcmp__n_0
   LDA   #$00
   STA   __zpabi_memcmp__n_1
   STA   __zpabi_memcmp__n_2
   STA   __zpabi_memcmp__n_3
   LDX   #>__local_main__arr1
   LDA   #<__local_main__arr1
   RTS

main@run0:
   SUBROUTINE

   LDA   #$04
   STA   __zpabi_malloc__size_0
   LDA   #$00
   STA   __zpabi_malloc__size_1
   STA   __zpabi_malloc__size_2
   STA   __zpabi_malloc__size_3
   JMP   malloc
