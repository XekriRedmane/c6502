SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__local_subscript_function_result__0	EQU	$80
__zpabi_negate_subscript__idx_0	EQU	$80
__zpabi_subscript_inception__a_0	EQU	$80
__local_subscript_function_result__1	EQU	$81
__zpabi_negate_subscript__idx_1	EQU	$81
__zpabi_subscript_inception__a_1	EQU	$81
__local_subscript_function_result__2	EQU	$82
__zpabi_negate_subscript__expected_0	EQU	$82
__zpabi_subscript_inception__b_0	EQU	$82
__local_subscript_function_result__3	EQU	$83
__zpabi_negate_subscript__expected_1	EQU	$83
__zpabi_subscript_inception__b_1	EQU	$83
__local_main__0	EQU	$84
__local_main__1	EQU	$85
__local_main__2	EQU	$86
__local_main__arr	EQU	$87
__local_assign_in_index__idx_0	EQU	$8D
__local_funcall_in_index__0	EQU	$8D
__local_negate_subscript__0	EQU	$8D
__local_assign_in_index__0	EQU	$8E
__local_check_subscript_inception__1	EQU	$8E
__local_funcall_in_index__1	EQU	$8E
__local_negate_subscript__1	EQU	$8E
__local_assign_in_index__1	EQU	$8F
__local_funcall_in_index__2	EQU	$8F
__local_negate_subscript__arr_0	EQU	$8F
__local_assign_in_index__2	EQU	$90
__local_funcall_in_index__3	EQU	$90
__local_negate_subscript__2	EQU	$90
__local_assign_in_index__3	EQU	$91
__local_check_subscript_inception__arr	EQU	$91
__local_funcall_in_index__4	EQU	$91
__local_assign_in_index__idx_1	EQU	$92
__local_funcall_in_index__5	EQU	$92
__local_assign_in_index__arr	EQU	$93
__local_funcall_in_index__arr	EQU	$93
__local_check_subscript_inception__indices	EQU	$A1
__local_subscript_inception__0	EQU	$A5
__local_subscript_inception__1	EQU	$A6
__local_subscript_inception__2	EQU	$A7
__local_subscript_inception__3	EQU	$A8
__local_subscript_inception__4	EQU	$A9
__local_subscript_inception__arr_0	EQU	$AB

; @zp-link-meta-begin
; def assign_in_index params=__zpabi_assign_in_index__idx_0,__zpabi_assign_in_index__idx_1 locals=__local_assign_in_index__idx_0,__local_assign_in_index__0,__local_assign_in_index__1,__local_assign_in_index__2,__local_assign_in_index__3,__local_assign_in_index__idx_1,__local_assign_in_index__arr,__local_assign_in_index__5,__local_assign_in_index__6,__local_assign_in_index__7,__local_assign_in_index__8,__local_assign_in_index__9 indirect=false in_cycle=false param_regs=A,X
; def check_subscript_inception params= locals=__local_check_subscript_inception__0,__local_check_subscript_inception__1,__local_check_subscript_inception__2,__local_check_subscript_inception__3,__local_check_subscript_inception__arr,__local_check_subscript_inception__5,__local_check_subscript_inception__6,__local_check_subscript_inception__7,__local_check_subscript_inception__8,__local_check_subscript_inception__9,__local_check_subscript_inception__10,__local_check_subscript_inception__11,__local_check_subscript_inception__12,__local_check_subscript_inception__13,__local_check_subscript_inception__14,__local_check_subscript_inception__15,__local_check_subscript_inception__16,__local_check_subscript_inception__17,__local_check_subscript_inception__18,__local_check_subscript_inception__19,__local_check_subscript_inception__indices,__local_check_subscript_inception__21,__local_check_subscript_inception__22,__local_check_subscript_inception__23 indirect=false in_cycle=false
; def funcall_in_index params= locals=__local_funcall_in_index__0,__local_funcall_in_index__1,__local_funcall_in_index__2,__local_funcall_in_index__3,__local_funcall_in_index__4,__local_funcall_in_index__5,__local_funcall_in_index__arr,__local_funcall_in_index__7,__local_funcall_in_index__8,__local_funcall_in_index__9,__local_funcall_in_index__10,__local_funcall_in_index__11 indirect=false in_cycle=false
; def get_array params= locals= indirect=false in_cycle=false
; def main params= locals=__local_main__0,__local_main__1,__local_main__2,__local_main__arr,__local_main__4,__local_main__5,__local_main__6,__local_main__7,__local_main__8 indirect=false in_cycle=false
; def negate_subscript params=__zpabi_negate_subscript__arr_0,__zpabi_negate_subscript__arr_1,__zpabi_negate_subscript__idx_0,__zpabi_negate_subscript__idx_1,__zpabi_negate_subscript__expected_0,__zpabi_negate_subscript__expected_1 locals=__local_negate_subscript__0,__local_negate_subscript__1,__local_negate_subscript__arr_0,__local_negate_subscript__2,__local_negate_subscript__arr_1 indirect=false in_cycle=false param_regs=A,X,,,,
; def static_index params= locals=__local_static_index__retval indirect=false in_cycle=false
; def subscript_function_result params= locals=__local_subscript_function_result__0,__local_subscript_function_result__1,__local_subscript_function_result__2,__local_subscript_function_result__3 indirect=false in_cycle=false
; def subscript_inception params=__zpabi_subscript_inception__arr_0,__zpabi_subscript_inception__arr_1,__zpabi_subscript_inception__a_0,__zpabi_subscript_inception__a_1,__zpabi_subscript_inception__b_0,__zpabi_subscript_inception__b_1 locals=__local_subscript_inception__0,__local_subscript_inception__1,__local_subscript_inception__2,__local_subscript_inception__3,__local_subscript_inception__4,__local_subscript_inception__arr_1,__local_subscript_inception__arr_0 indirect=false in_cycle=false param_regs=A,X,,,,
; call check_subscript_inception -> subscript_inception
; call funcall_in_index -> static_index
; call main -> assign_in_index
; call main -> check_subscript_inception
; call main -> funcall_in_index
; call main -> negate_subscript
; call main -> subscript_function_result
; call subscript_function_result -> get_array
; item assign_in_index section=.text local=false zero=false
; item assign_in_index@tail0 section=.text local=true zero=false
; item static_index section=.text local=false zero=false
; item funcall_in_index section=.text local=false zero=false
; item subscript_inception section=.text local=false zero=false
; item check_subscript_inception section=.text local=false zero=false
; item check_subscript_inception@run0 section=.text local=true zero=false
; item get_array section=.text local=false zero=false
; item subscript_function_result section=.text local=false zero=false
; item negate_subscript section=.text local=false zero=false
; item main section=.text local=false zero=false
; item @3.index section=.bss local=true zero=true
; item @13.arr section=.bss local=true zero=true
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

assign_in_index:
   SUBROUTINE

   STA   __local_assign_in_index__idx_0
   STX   __local_assign_in_index__idx_1
   LDA   #$01
   STA   __local_assign_in_index__arr
   LDA   #$00
   STA   __local_assign_in_index__arr+1
   LDY   #$02
   STY   __local_assign_in_index__arr+2
   STA   __local_assign_in_index__arr+3
   INY
   STY   __local_assign_in_index__arr+4
   STA   __local_assign_in_index__arr+5
   LDA   #>__local_assign_in_index__arr
   STA   __local_assign_in_index__3
   LDA   __local_assign_in_index__idx_0
   CLC
   ADC   #$02
   STA   __local_assign_in_index__1
   BCC   .carry_inc@0
   INC   __local_assign_in_index__idx_1
.carry_inc@0:
   ASL
   STA   __local_assign_in_index__idx_0
   LDA   #<__local_assign_in_index__arr
   CLC
   ADC   __local_assign_in_index__idx_0
   STA   __local_assign_in_index__2
   BCC   .carry_inc@1
   INC   __local_assign_in_index__3
.carry_inc@1:
   LDY   #$00
   LDA   (__local_assign_in_index__2),Y
   STA   __local_assign_in_index__0
   INY
   LDA   (__local_assign_in_index__2),Y
   STA   __local_assign_in_index__idx_0
   LDA   __local_assign_in_index__idx_1
   BNE   .jcmp_skip@0
   LDA   __local_assign_in_index__1
   CMP   #$01
   BEQ   .if_end@0
.jcmp_skip@0:
   LDX   #$00
   LDA   #$01
   RTS
.if_end@0:
   LDA   __local_assign_in_index__idx_0
   BNE   .jcmp_skip@1
   LDA   __local_assign_in_index__0
   CMP   #$02
   BEQ   .if_end@1
.jcmp_skip@1:
   LDX   #$00
   LDA   #$02
   RTS
.if_end@1:
   JMP   assign_in_index@tail0

assign_in_index@tail0:
   SUBROUTINE

   LDX   #$00
   TXA
   RTS

static_index:
   SUBROUTINE

   LDA   @3.index
   LDX   @3.index+1
   INC   @3.index
   BNE   .inc_done@0
   INC   @3.index+1
.inc_done@0:
   RTS

funcall_in_index:
   SUBROUTINE

   LDA   #$01
   STA   __local_funcall_in_index__arr
   LDA   #$00
   STA   __local_funcall_in_index__arr+1
   LDY   #$02
   STY   __local_funcall_in_index__arr+2
   STA   __local_funcall_in_index__arr+3
   INY
   STY   __local_funcall_in_index__arr+4
   STA   __local_funcall_in_index__arr+5
   LDA   #>__local_funcall_in_index__arr
   STA   __local_funcall_in_index__2
   JSR   static_index
   ASL
   STA   __local_funcall_in_index__0
   LDA   #<__local_funcall_in_index__arr
   CLC
   ADC   __local_funcall_in_index__0
   STA   __local_funcall_in_index__1
   BCC   .carry_inc@2
   INC   __local_funcall_in_index__2
.carry_inc@2:
   LDY   #$00
   LDA   (__local_funcall_in_index__1),Y
   STA   __local_funcall_in_index__5
   INY
   LDA   (__local_funcall_in_index__1),Y
   STA   __local_funcall_in_index__4
   LDA   #>__local_funcall_in_index__arr
   STA   __local_funcall_in_index__3
   JSR   static_index
   ASL
   STA   __local_funcall_in_index__0
   LDA   #<__local_funcall_in_index__arr
   CLC
   ADC   __local_funcall_in_index__0
   STA   __local_funcall_in_index__2
   BCC   .carry_inc@3
   INC   __local_funcall_in_index__3
.carry_inc@3:
   LDY   #$00
   LDA   (__local_funcall_in_index__2),Y
   STA   __local_funcall_in_index__1
   INY
   LDA   (__local_funcall_in_index__2),Y
   STA   __local_funcall_in_index__0
   LDA   __local_funcall_in_index__4
   BNE   .jcmp_skip@2
   LDA   __local_funcall_in_index__5
   CMP   #$01
   BEQ   .if_end@2
.jcmp_skip@2:
   LDX   #$00
   LDA   #$03
   RTS
.if_end@2:
   LDA   __local_funcall_in_index__0
   BNE   .jcmp_skip@3
   LDA   __local_funcall_in_index__1
   CMP   #$02
   BEQ   .if_end@3
.jcmp_skip@3:
   LDX   #$00
   LDA   #$04
   RTS
.if_end@3:
   JMP   assign_in_index@tail0

subscript_inception:
   SUBROUTINE

   STA   __local_subscript_inception__arr_0
   LDA   __zpabi_subscript_inception__b_0
   ASL
   STA   __local_subscript_inception__2
   LDA   __zpabi_subscript_inception__b_1
   ROL
   STA   __local_subscript_inception__3
   LDA   __zpabi_subscript_inception__a_0
   CLC
   ADC   __local_subscript_inception__2
   STA   __local_subscript_inception__0
   LDA   __zpabi_subscript_inception__a_1
   ADC   __local_subscript_inception__3
   STA   __local_subscript_inception__1
   LDY   #$00
   LDA   (__local_subscript_inception__0),Y
   STA   __local_subscript_inception__3
   INY
   LDA   (__local_subscript_inception__0),Y
   ASL   __local_subscript_inception__3
   ROL
   ASL   __local_subscript_inception__3
   ROL
   STA   __local_subscript_inception__4
   LDA   __local_subscript_inception__arr_0
   CLC
   ADC   __local_subscript_inception__3
   STA   __local_subscript_inception__1
   TXA
   ADC   __local_subscript_inception__4
   STA   __local_subscript_inception__2
   LDA   (__local_subscript_inception__1),Y
   TAX
   DEY
   LDA   (__local_subscript_inception__1),Y
   RTS

check_subscript_inception:
   SUBROUTINE

   LDA   #$04
   STA   __local_check_subscript_inception__arr
   LDA   #$00
   STA   __local_check_subscript_inception__arr+1
   LDY   #$00
   STY   __local_check_subscript_inception__arr+2
   STA   __local_check_subscript_inception__arr+3
   LDY   #$03
   STY   __local_check_subscript_inception__arr+4
   LDY   #$00
   STY   __local_check_subscript_inception__arr+5
   STY   __local_check_subscript_inception__arr+6
   STA   __local_check_subscript_inception__arr+7
   LDY   #$02
   STY   __local_check_subscript_inception__arr+8
   LDY   #$00
   STY   __local_check_subscript_inception__arr+9
   STY   __local_check_subscript_inception__arr+10
   STA   __local_check_subscript_inception__arr+11
   INY
   STY   __local_check_subscript_inception__arr+12
   DEY
   STY   __local_check_subscript_inception__arr+13
   STY   __local_check_subscript_inception__arr+14
   STA   __local_check_subscript_inception__arr+15
   INY
   STY   __local_check_subscript_inception__indices
   STA   __local_check_subscript_inception__indices+1
   INY
   STY   __local_check_subscript_inception__indices+2
   STA   __local_check_subscript_inception__indices+3
   JSR   check_subscript_inception@run0
   LDA   #$01
   STA   __zpabi_subscript_inception__b_0
   LDA   #$00
   STA   __zpabi_subscript_inception__b_1
   LDX   #>__local_check_subscript_inception__arr
   LDA   #<__local_check_subscript_inception__arr
   JSR   subscript_inception
   STA   __local_check_subscript_inception__1
   TXA
   BNE   .jcmp_skip@4
   LDA   __local_check_subscript_inception__1
   CMP   #$02
   BEQ   .if_end@4
.jcmp_skip@4:
   LDX   #$00
   LDA   #$05
   RTS
.if_end@4:
   JSR   check_subscript_inception@run0
   LDA   #$00
   STA   __zpabi_subscript_inception__b_0
   STA   __zpabi_subscript_inception__b_1
   LDX   #>__local_check_subscript_inception__arr
   LDA   #<__local_check_subscript_inception__arr
   JSR   subscript_inception
   STA   __local_check_subscript_inception__1
   TXA
   BNE   .jcmp_skip@5
   LDA   __local_check_subscript_inception__1
   CMP   #$03
   BEQ   .if_end@5
.jcmp_skip@5:
   LDX   #$00
   LDA   #$06
   RTS
.if_end@5:
   JMP   assign_in_index@tail0

check_subscript_inception@run0:
   SUBROUTINE

   LDA   #<__local_check_subscript_inception__indices
   STA   __zpabi_subscript_inception__a_0
   LDA   #>__local_check_subscript_inception__indices
   STA   __zpabi_subscript_inception__a_1
   RTS

get_array:
   SUBROUTINE

   LDX   #>@13.arr
   LDA   #<@13.arr
   RTS

subscript_function_result:
   SUBROUTINE

   JSR   get_array
   STA   __local_subscript_function_result__0
   STX   __local_subscript_function_result__1
   STA   DPTR
   STX   DPTR+1
   LDA   #$01
   LDY   #$04
   STA   (__local_subscript_function_result__0),Y
   LDA   #$00
   INY
   STA   (DPTR),Y
   JSR   get_array
   STA   __local_subscript_function_result__2
   STX   __local_subscript_function_result__3
   LDY   #$04
   LDA   (__local_subscript_function_result__2),Y
   STA   __local_subscript_function_result__1
   INY
   LDA   (__local_subscript_function_result__2),Y
   BNE   .jcmp_skip@6
   LDA   __local_subscript_function_result__1
   CMP   #$01
   BEQ   .if_end@6
.jcmp_skip@6:
   LDX   #$00
   LDA   #$07
   RTS
.if_end@6:
   JMP   assign_in_index@tail0

negate_subscript:
   SUBROUTINE

   STA   __local_negate_subscript__arr_0
   SEC
   LDA   #$00
   SBC   __zpabi_negate_subscript__idx_0
   STA   __local_negate_subscript__0
   LDA   #$00
   SBC   __zpabi_negate_subscript__idx_1
   ASL   __local_negate_subscript__0
   ROL
   STA   __local_negate_subscript__1
   LDA   __local_negate_subscript__arr_0
   CLC
   ADC   __local_negate_subscript__0
   STA   __local_negate_subscript__arr_0
   TXA
   ADC   __local_negate_subscript__1
   STA   __local_negate_subscript__2
   LDY   #$00
   LDA   (__local_negate_subscript__arr_0),Y
   STA   __local_negate_subscript__1
   INY
   LDA   (__local_negate_subscript__arr_0),Y
   CMP   __zpabi_negate_subscript__expected_1
   BNE   .jcmp_skip@7
   LDA   __local_negate_subscript__1
   CMP   __zpabi_negate_subscript__expected_0
   BEQ   .if_end@7
.jcmp_skip@7:
   LDX   #$00
   LDA   #$08
   RTS
.if_end@7:
   JMP   assign_in_index@tail0

main:
   SUBROUTINE

   LDX   #$FF
   TXA
   JSR   assign_in_index
   STA   __local_main__1
   STX   __local_main__0
   ORA   __local_main__0
   BEQ   .if_end@8
   LDA   __local_main__1
   RTS
.if_end@8:
   JSR   funcall_in_index
   STA   __local_main__1
   STX   __local_main__0
   ORA   __local_main__0
   BNE   .tail@0
   JSR   check_subscript_inception
   STA   __local_main__1
   STX   __local_main__0
   ORA   __local_main__0
   BNE   .tail@0
   JSR   subscript_function_result
   STA   __local_main__1
   STX   __local_main__0
   ORA   __local_main__0
   BNE   .tail@0
   LDA   #$00
   STA   __local_main__arr
   STA   __local_main__arr+1
   LDY   #$01
   STY   __local_main__arr+2
   STA   __local_main__arr+3
   INY
   STY   __local_main__arr+4
   STA   __local_main__arr+5
   LDA   #>__local_main__arr
   STA   __local_main__2
   LDA   #<__local_main__arr
   CLC
   ADC   #$04
   STA   __local_main__0
   BCC   .carry_inc@4
   INC   __local_main__2
.carry_inc@4:
   LDA   #$02
   STA   __zpabi_negate_subscript__idx_0
   LDA   #$00
   STA   __zpabi_negate_subscript__idx_1
   STA   __zpabi_negate_subscript__expected_0
   STA   __zpabi_negate_subscript__expected_1
   LDX   __local_main__2
   LDA   __local_main__0
   JSR   negate_subscript
   STA   __local_main__1
   STX   __local_main__0
   ORA   __local_main__0
   BEQ   .if_end@12
.tail@0:
   LDA   __local_main__1
   RTS
.if_end@12:
   JMP   assign_in_index@tail0

@3.index:
   DS.B  2

@13.arr:
   DS.B  6
