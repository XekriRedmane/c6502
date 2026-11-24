SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__local_check_increment_static_element__0	EQU	$80
__zpabi_integer_types__expected_0	EQU	$80
__zpabi_reverse_subscript__expected_0	EQU	$80
__zpabi_update_element__expected_0	EQU	$80
__local_check_increment_static_element__1	EQU	$81
__zpabi_integer_types__expected_1	EQU	$81
__zpabi_reverse_subscript__expected_1	EQU	$81
__zpabi_update_element__expected_1	EQU	$81
__local_check_increment_static_element__2	EQU	$82
__zpabi_reverse_subscript__expected_2	EQU	$82
__local_check_increment_static_element__3	EQU	$83
__zpabi_reverse_subscript__expected_3	EQU	$83
__local_main__0	EQU	$84
__local_main__1	EQU	$85
__local_main__unsigned_arr	EQU	$86
__local_main__long_arr	EQU	$92
__local_main__int_arr	EQU	$A2
__local_integer_types__0	EQU	$B8
__local_reverse_subscript__arr_0	EQU	$B8
__local_update_element__0	EQU	$B8
__local_integer_types__1	EQU	$B9
__local_reverse_subscript__0	EQU	$B9
__local_update_element__1	EQU	$B9
__local_integer_types__arr	EQU	$BA
__local_reverse_subscript__1	EQU	$BA
__local_update_element__arr	EQU	$BA
__local_integer_types__2	EQU	$BB
__local_reverse_subscript__2	EQU	$BB
__local_update_element__2	EQU	$BB
__local_reverse_subscript__3	EQU	$BC
__local_reverse_subscript__arr_1	EQU	$BD

; @zp-link-meta-begin
; def check_increment_static_element params= locals=__local_check_increment_static_element__0,__local_check_increment_static_element__1,__local_check_increment_static_element__2,__local_check_increment_static_element__3 indirect=false in_cycle=false
; def increment_static_element params= locals= indirect=false in_cycle=false
; def integer_types params=__zpabi_integer_types__arr_0,__zpabi_integer_types__arr_1,__zpabi_integer_types__expected_0,__zpabi_integer_types__expected_1 locals=__local_integer_types__0,__local_integer_types__1,__local_integer_types__arr,__local_integer_types__2 indirect=false in_cycle=false param_regs=A,X,,
; def main params= locals=__local_main__0,__local_main__1,__local_main__unsigned_arr,__local_main__3,__local_main__4,__local_main__5,__local_main__6,__local_main__7,__local_main__8,__local_main__9,__local_main__10,__local_main__11,__local_main__12,__local_main__13,__local_main__long_arr,__local_main__15,__local_main__16,__local_main__17,__local_main__18,__local_main__19,__local_main__20,__local_main__21,__local_main__22,__local_main__23,__local_main__24,__local_main__25,__local_main__26,__local_main__27,__local_main__28,__local_main__29,__local_main__int_arr,__local_main__31,__local_main__32,__local_main__33,__local_main__34,__local_main__35,__local_main__36,__local_main__37,__local_main__38,__local_main__39,__local_main__40,__local_main__41,__local_main__42,__local_main__43,__local_main__44,__local_main__45,__local_main__46,__local_main__47,__local_main__48,__local_main__49,__local_main__50,__local_main__51 indirect=false in_cycle=false
; def reverse_subscript params=__zpabi_reverse_subscript__arr_0,__zpabi_reverse_subscript__arr_1,__zpabi_reverse_subscript__expected_0,__zpabi_reverse_subscript__expected_1,__zpabi_reverse_subscript__expected_2,__zpabi_reverse_subscript__expected_3 locals=__local_reverse_subscript__arr_0,__local_reverse_subscript__0,__local_reverse_subscript__1,__local_reverse_subscript__2,__local_reverse_subscript__3,__local_reverse_subscript__arr_1 indirect=false in_cycle=false param_regs=A,X,,,,
; def subscript_static params= locals=__local_subscript_static__0,__local_subscript_static__1,__local_subscript_static__2,__local_subscript_static__3,__local_subscript_static__4,__local_subscript_static__5,__local_subscript_static__6,__local_subscript_static__7 indirect=false in_cycle=false
; def update_element params=__zpabi_update_element__arr_0,__zpabi_update_element__arr_1,__zpabi_update_element__expected_0,__zpabi_update_element__expected_1 locals=__local_update_element__0,__local_update_element__1,__local_update_element__arr,__local_update_element__2 indirect=false in_cycle=false param_regs=A,X,,
; call check_increment_static_element -> increment_static_element
; call main -> check_increment_static_element
; call main -> integer_types
; call main -> reverse_subscript
; call main -> subscript_static
; call main -> update_element
; item integer_types section=.text local=false zero=false
; item integer_types@tail0 section=.text local=true zero=false
; item reverse_subscript section=.text local=false zero=false
; item subscript_static section=.text local=false zero=false
; item update_element section=.text local=false zero=false
; item increment_static_element section=.text local=false zero=false
; item check_increment_static_element section=.text local=false zero=false
; item main section=.text local=false zero=false
; item static_array section=.data local=true zero=false
; item @10.arr section=.bss local=true zero=true
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

integer_types:
   SUBROUTINE

   STA   __local_integer_types__arr
   STX   __local_integer_types__2
   LDY   #$0A
   LDA   (__local_integer_types__arr),Y
   STA   __local_integer_types__1
   INY
   LDA   (__local_integer_types__arr),Y
   STA   __local_integer_types__0
   CMP   __zpabi_integer_types__expected_1
   BNE   .jcmp_skip@0
   LDA   __local_integer_types__1
   CMP   __zpabi_integer_types__expected_0
   BEQ   .if_end@0
.jcmp_skip@0:
   LDX   #$00
   LDA   #$01
   RTS
.if_end@0:
   LDA   __local_integer_types__0
   CMP   __zpabi_integer_types__expected_1
   BNE   .jcmp_skip@1
   LDA   __local_integer_types__1
   CMP   __zpabi_integer_types__expected_0
   BEQ   .if_end@1
.jcmp_skip@1:
   LDX   #$00
   LDA   #$02
   RTS
.if_end@1:
   LDA   __local_integer_types__0
   CMP   __zpabi_integer_types__expected_1
   BNE   .jcmp_skip@2
   LDA   __local_integer_types__1
   CMP   __zpabi_integer_types__expected_0
   BEQ   .if_end@2
.jcmp_skip@2:
   LDX   #$00
   LDA   #$03
   RTS
.if_end@2:
   LDA   __local_integer_types__0
   CMP   __zpabi_integer_types__expected_1
   BNE   .jcmp_skip@3
   LDA   __local_integer_types__1
   CMP   __zpabi_integer_types__expected_0
   BEQ   .if_end@3
.jcmp_skip@3:
   LDX   #$00
   LDA   #$04
   RTS
.if_end@3:
   JMP   integer_types@tail0

integer_types@tail0:
   SUBROUTINE

   LDX   #$00
   TXA
   RTS

reverse_subscript:
   SUBROUTINE

   STX   __local_reverse_subscript__arr_1
   CLC
   ADC   #$0C
   STA   __local_reverse_subscript__3
   BCC   .carry_inc@0
   INC   __local_reverse_subscript__arr_1
.carry_inc@0:
   LDY   #$00
   LDA   (__local_reverse_subscript__3),Y
   STA   __local_reverse_subscript__2
   INY
   LDA   (__local_reverse_subscript__3),Y
   STA   __local_reverse_subscript__1
   INY
   LDA   (__local_reverse_subscript__3),Y
   STA   __local_reverse_subscript__0
   INY
   LDA   (__local_reverse_subscript__3),Y
   STA   __local_reverse_subscript__arr_0
   CMP   __zpabi_reverse_subscript__expected_3
   BNE   .jcmp_skip@4
   LDA   __local_reverse_subscript__0
   CMP   __zpabi_reverse_subscript__expected_2
   BNE   .jcmp_skip@4
   LDA   __local_reverse_subscript__1
   CMP   __zpabi_reverse_subscript__expected_1
   BNE   .jcmp_skip@4
   LDA   __local_reverse_subscript__2
   CMP   __zpabi_reverse_subscript__expected_0
   BEQ   .if_end@4
.jcmp_skip@4:
   LDX   #$00
   LDA   #$05
   RTS
.if_end@4:
   LDA   __local_reverse_subscript__arr_0
   CMP   __zpabi_reverse_subscript__expected_3
   BNE   .jcmp_skip@5
   LDA   __local_reverse_subscript__0
   CMP   __zpabi_reverse_subscript__expected_2
   BNE   .jcmp_skip@5
   LDA   __local_reverse_subscript__1
   CMP   __zpabi_reverse_subscript__expected_1
   BNE   .jcmp_skip@5
   LDA   __local_reverse_subscript__2
   CMP   __zpabi_reverse_subscript__expected_0
   BEQ   .if_end@5
.jcmp_skip@5:
   LDX   #$00
   LDA   #$06
   RTS
.if_end@5:
   LDA   __local_reverse_subscript__arr_1
   CMP   __local_reverse_subscript__arr_1
   BNE   .jcmp_skip@6
   LDA   __local_reverse_subscript__3
   CMP   __local_reverse_subscript__3
   BEQ   .if_end@6
.jcmp_skip@6:
   LDX   #$00
   LDA   #$07
   RTS
.if_end@6:
   JMP   integer_types@tail0

subscript_static:
   SUBROUTINE

   LDA   static_array+7
   CMP   #$3F
   BNE   .jcmp_skip@7
   LDA   static_array+6
   CMP   #$B9
   BNE   .jcmp_skip@7
   LDA   static_array+5
   CMP   #$99
   BNE   .jcmp_skip@7
   LDA   static_array+4
   CMP   #$99
   BNE   .jcmp_skip@7
   LDA   static_array+3
   CMP   #$99
   BNE   .jcmp_skip@7
   LDA   static_array+2
   CMP   #$99
   BNE   .jcmp_skip@7
   LDA   static_array+1
   CMP   #$99
   BNE   .jcmp_skip@7
   LDA   static_array
   CMP   #$9A
   BEQ   .if_end@7
.jcmp_skip@7:
   LDX   #$00
   LDA   #$08
   RTS
.if_end@7:
   LDA   static_array+15
   CMP   #$3F
   BNE   .jcmp_skip@8
   LDA   static_array+14
   CMP   #$C9
   BNE   .jcmp_skip@8
   LDA   static_array+13
   CMP   #$99
   BNE   .jcmp_skip@8
   LDA   static_array+12
   CMP   #$99
   BNE   .jcmp_skip@8
   LDA   static_array+11
   CMP   #$99
   BNE   .jcmp_skip@8
   LDA   static_array+10
   CMP   #$99
   BNE   .jcmp_skip@8
   LDA   static_array+9
   CMP   #$99
   BNE   .jcmp_skip@8
   LDA   static_array+8
   CMP   #$9A
   BEQ   .if_end@8
.jcmp_skip@8:
   LDX   #$00
   LDA   #$09
   RTS
.if_end@8:
   LDA   static_array+23
   CMP   #$3F
   BNE   .jcmp_skip@9
   LDA   static_array+22
   CMP   #$D3
   BNE   .jcmp_skip@9
   LDA   static_array+21
   CMP   #$33
   BNE   .jcmp_skip@9
   LDA   static_array+20
   CMP   #$33
   BNE   .jcmp_skip@9
   LDA   static_array+19
   CMP   #$33
   BNE   .jcmp_skip@9
   LDA   static_array+18
   CMP   #$33
   BNE   .jcmp_skip@9
   LDA   static_array+17
   CMP   #$33
   BNE   .jcmp_skip@9
   LDA   static_array+16
   CMP   #$33
   BEQ   .if_end@9
.jcmp_skip@9:
   LDX   #$00
   LDA   #$0A
   RTS
.if_end@9:
   JMP   integer_types@tail0

update_element:
   SUBROUTINE

   STA   __local_update_element__arr
   STX   __local_update_element__2
   STA   DPTR
   LDY   #$14
   LDA   (__local_update_element__arr),Y
   STA   __local_update_element__0
   INY
   LDA   (__local_update_element__arr),Y
   ASL   __local_update_element__0
   ROL
   STA   __local_update_element__1
   STX   DPTR+1
   LDA   __local_update_element__0
   DEY
   STA   (__local_update_element__arr),Y
   LDA   __local_update_element__1
   INY
   STA   (DPTR),Y
   DEY
   LDA   (__local_update_element__arr),Y
   STA   __local_update_element__1
   INY
   LDA   (__local_update_element__arr),Y
   CMP   __zpabi_update_element__expected_1
   BNE   .jcmp_skip@10
   LDA   __local_update_element__1
   CMP   __zpabi_update_element__expected_0
   BEQ   .if_end@10
.jcmp_skip@10:
   LDX   #$00
   LDA   #$0B
   RTS
.if_end@10:
   JMP   integer_types@tail0

increment_static_element:
   SUBROUTINE

   INC   @10.arr+6
   BNE   .inc_done@0
   INC   @10.arr+7
.inc_done@0:
   LDX   #>@10.arr
   LDA   #<@10.arr
   RTS

check_increment_static_element:
   SUBROUTINE

   JSR   increment_static_element
   STA   __local_check_increment_static_element__2
   STX   __local_check_increment_static_element__3
   LDY   #$06
   LDA   (__local_check_increment_static_element__2),Y
   STA   __local_check_increment_static_element__1
   INY
   LDA   (__local_check_increment_static_element__2),Y
   BNE   .jcmp_skip@11
   LDA   __local_check_increment_static_element__1
   CMP   #$01
   BEQ   .if_end@11
.jcmp_skip@11:
   LDX   #$00
   LDA   #$0C
   RTS
.if_end@11:
   LDY   #$01
   LDA   (__local_check_increment_static_element__2),Y
   DEY
   ORA   (__local_check_increment_static_element__2),Y
   BNE   .check_increment_static_element@jthread@1
   LDY   #$03
   LDA   (__local_check_increment_static_element__2),Y
   DEY
   ORA   (__local_check_increment_static_element__2),Y
   BNE   .check_increment_static_element@jthread@1
   LDY   #$05
   LDA   (__local_check_increment_static_element__2),Y
   DEY
   ORA   (__local_check_increment_static_element__2),Y
   BEQ   .if_end@16
.check_increment_static_element@jthread@1:
   LDX   #$00
   LDA   #$0D
   RTS
.if_end@16:
   JSR   increment_static_element
   STA   __local_check_increment_static_element__1
   STX   __local_check_increment_static_element__0
   LDA   __local_check_increment_static_element__3
   CMP   __local_check_increment_static_element__0
   BNE   .jcmp_skip@12
   LDA   __local_check_increment_static_element__2
   CMP   __local_check_increment_static_element__1
   BEQ   .if_end@17
.jcmp_skip@12:
   LDX   #$00
   LDA   #$0E
   RTS
.if_end@17:
   LDY   #$06
   LDA   (__local_check_increment_static_element__2),Y
   STA   __local_check_increment_static_element__1
   INY
   LDA   (__local_check_increment_static_element__2),Y
   BNE   .jcmp_skip@13
   LDA   __local_check_increment_static_element__1
   CMP   #$02
   BEQ   .if_end@18
.jcmp_skip@13:
   LDX   #$00
   LDA   #$0F
   RTS
.if_end@18:
   JMP   integer_types@tail0

main:
   SUBROUTINE

   LDY   #$0A
   LDA   #$00
.clear@19:
   DEY
   STA   __local_main__unsigned_arr,Y
   BNE   .clear@19
   STA   __local_main__unsigned_arr+11
   LDA   #$07
   STA   __local_main__unsigned_arr+10
   STA   __zpabi_integer_types__expected_0
   LDA   #$00
   STA   __zpabi_integer_types__expected_1
   LDX   #>__local_main__unsigned_arr
   LDA   #<__local_main__unsigned_arr
   JSR   integer_types
   STA   __local_main__1
   STX   __local_main__0
   ORA   __local_main__0
   BEQ   .if_end@20
   LDA   __local_main__1
   RTS
.if_end@20:
   LDA   #$64
   STA   __local_main__long_arr
   LDA   #$00
   STA   __local_main__long_arr+1
   LDY   #$00
   STY   __local_main__long_arr+2
   STA   __local_main__long_arr+3
   LDY   #$66
   STY   __local_main__long_arr+4
   LDY   #$00
   STY   __local_main__long_arr+5
   STY   __local_main__long_arr+6
   STA   __local_main__long_arr+7
   LDY   #$68
   STY   __local_main__long_arr+8
   LDY   #$00
   STY   __local_main__long_arr+9
   STY   __local_main__long_arr+10
   STA   __local_main__long_arr+11
   LDY   #$6A
   STY   __local_main__long_arr+12
   LDY   #$00
   STY   __local_main__long_arr+13
   STY   __local_main__long_arr+14
   STA   __local_main__long_arr+15
   LDA   #$6A
   STA   __zpabi_reverse_subscript__expected_0
   LDA   #$00
   STA   __zpabi_reverse_subscript__expected_1
   STA   __zpabi_reverse_subscript__expected_2
   STA   __zpabi_reverse_subscript__expected_3
   LDX   #>__local_main__long_arr
   LDA   #<__local_main__long_arr
   JSR   reverse_subscript
   STA   __local_main__1
   STX   __local_main__0
   ORA   __local_main__0
   BNE   .tail@0
   JSR   subscript_static
   STA   __local_main__1
   STX   __local_main__0
   ORA   __local_main__0
   BNE   .tail@0
   LDY   #$14
   LDA   #$00
.clear@23:
   DEY
   STA   __local_main__int_arr,Y
   BNE   .clear@23
   STA   __local_main__int_arr+21
   LDA   #$0F
   STA   __local_main__int_arr+20
   LDA   #$1E
   STA   __zpabi_update_element__expected_0
   LDA   #$00
   STA   __zpabi_update_element__expected_1
   LDX   #>__local_main__int_arr
   LDA   #<__local_main__int_arr
   JSR   update_element
   STA   __local_main__1
   STX   __local_main__0
   ORA   __local_main__0
   BNE   .tail@0
   JSR   check_increment_static_element
   STA   __local_main__1
   STX   __local_main__0
   ORA   __local_main__0
   BEQ   .if_end@25
.tail@0:
   LDA   __local_main__1
   RTS
.if_end@25:
   JMP   integer_types@tail0

static_array:
   DC.L  $9999999A
   DC.L  $3FB99999
   DC.L  $9999999A
   DC.L  $3FC99999
   DC.L  $33333333
   DC.L  $3FD33333

@10.arr:
   DS.B  8
