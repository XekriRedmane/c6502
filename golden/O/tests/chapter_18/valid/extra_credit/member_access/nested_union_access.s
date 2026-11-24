SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
@16.outer_ptr	EQU	$26
__local_test_static_arrow__0	EQU	$80
__local_test_static_dot__0	EQU	$80
__zpabi_calloc__nmemb_0	EQU	$80
__zpabi_malloc__size_0	EQU	$80
__local_test_static_arrow__1	EQU	$81
__local_test_static_dot__1	EQU	$81
__zpabi_calloc__nmemb_1	EQU	$81
__zpabi_malloc__size_1	EQU	$81
__local_test_static_arrow__2	EQU	$82
__local_test_static_dot__2	EQU	$82
__zpabi_calloc__nmemb_2	EQU	$82
__zpabi_malloc__size_2	EQU	$82
__local_test_static_arrow__3	EQU	$83
__local_test_static_dot__3	EQU	$83
__zpabi_calloc__nmemb_3	EQU	$83
__zpabi_malloc__size_3	EQU	$83
__local_test_static_arrow__4	EQU	$84
__local_test_static_dot__4	EQU	$84
__zpabi_calloc__size_0	EQU	$84
__local_test_static_arrow__5	EQU	$85
__local_test_static_dot__5	EQU	$85
__zpabi_calloc__size_1	EQU	$85
__local_test_static_dot__6	EQU	$86
__zpabi_calloc__size_2	EQU	$86
__zpabi_calloc__size_3	EQU	$87
__local_main__0	EQU	$88
__local_test_array_of_union_pointers__0	EQU	$8A
__local_test_auto_arrow__0	EQU	$8A
__local_test_array_of_union_pointers__1	EQU	$8B
__local_test_auto_arrow__1	EQU	$8B
__local_test_array_of_union_pointers__2	EQU	$8C
__local_test_auto_arrow__2	EQU	$8C
__local_test_array_of_union_pointers__3	EQU	$8D
__local_test_auto_arrow__3	EQU	$8D
__local_test_array_of_union_pointers__4	EQU	$8E
__local_test_auto_arrow__outer_0	EQU	$8E
__local_test_array_of_union_pointers__5	EQU	$8F
__local_test_auto_arrow__outer_1	EQU	$8F
__local_test_array_of_union_pointers__6	EQU	$90
__local_test_auto_arrow__inner	EQU	$90
__local_test_array_of_union_pointers__i_1	EQU	$91
__local_test_array_of_union_pointers__i_0	EQU	$92
__local_test_auto_dot__7	EQU	$92
__local_test_array_of_union_pointers__ptr_arr	EQU	$93
__local_test_auto_dot__8	EQU	$93
__local_test_auto_dot__9	EQU	$94
__local_test_auto_dot__10	EQU	$95
__local_test_auto_dot__11	EQU	$96
__local_test_auto_dot__12	EQU	$97
__local_test_auto_dot__13	EQU	$98
__local_test_auto_dot__14	EQU	$99

; @zp-link-meta-begin
; def main params= locals=__local_main__0,__local_main__1 indirect=false in_cycle=false
; def test_array_of_union_pointers params= locals=__local_test_array_of_union_pointers__0,__local_test_array_of_union_pointers__1,__local_test_array_of_union_pointers__2,__local_test_array_of_union_pointers__3,__local_test_array_of_union_pointers__4,__local_test_array_of_union_pointers__5,__local_test_array_of_union_pointers__6,__local_test_array_of_union_pointers__i_1,__local_test_array_of_union_pointers__i_0,__local_test_array_of_union_pointers__ptr_arr,__local_test_array_of_union_pointers__8,__local_test_array_of_union_pointers__9,__local_test_array_of_union_pointers__10,__local_test_array_of_union_pointers__11,__local_test_array_of_union_pointers__12 indirect=false in_cycle=false
; def test_array_of_unions params= locals=__local_test_array_of_unions__arr_16,__local_test_array_of_unions__arr_8,__local_test_array_of_unions__arr_3,__local_test_array_of_unions__arr_2,__local_test_array_of_unions__arr_1,__local_test_array_of_unions__arr_0 indirect=false in_cycle=false
; def test_auto_arrow params= locals=__local_test_auto_arrow__0,__local_test_auto_arrow__1,__local_test_auto_arrow__2,__local_test_auto_arrow__3,__local_test_auto_arrow__outer_0,__local_test_auto_arrow__outer_1,__local_test_auto_arrow__inner,__local_test_auto_arrow__5,__local_test_auto_arrow__6,__local_test_auto_arrow__7 indirect=false in_cycle=false
; def test_auto_dot params= locals=__local_test_auto_dot__z_0,__local_test_auto_dot__0,__local_test_auto_dot__1,__local_test_auto_dot__2,__local_test_auto_dot__3,__local_test_auto_dot__4,__local_test_auto_dot__5,__local_test_auto_dot__6,__local_test_auto_dot__7,__local_test_auto_dot__8,__local_test_auto_dot__9,__local_test_auto_dot__10,__local_test_auto_dot__11,__local_test_auto_dot__12,__local_test_auto_dot__13,__local_test_auto_dot__14,__local_test_auto_dot__z_1,__local_test_auto_dot__15,__local_test_auto_dot__y_13,__local_test_auto_dot__y_12,__local_test_auto_dot__y_1,__local_test_auto_dot__y_0,__local_test_auto_dot__16,__local_test_auto_dot__17,__local_test_auto_dot__18,__local_test_auto_dot__19,__local_test_auto_dot__20,__local_test_auto_dot__21 indirect=false in_cycle=false
; def test_static_arrow params= locals=__local_test_static_arrow__0,__local_test_static_arrow__1,__local_test_static_arrow__2,__local_test_static_arrow__3,__local_test_static_arrow__4,__local_test_static_arrow__5 indirect=false in_cycle=false
; def test_static_dot params= locals=__local_test_static_dot__0,__local_test_static_dot__1,__local_test_static_dot__2,__local_test_static_dot__3,__local_test_static_dot__4,__local_test_static_dot__5,__local_test_static_dot__6,__local_test_static_dot__7 indirect=false in_cycle=false
; ext calloc params=__zpabi_calloc__nmemb_0,__zpabi_calloc__nmemb_1,__zpabi_calloc__nmemb_2,__zpabi_calloc__nmemb_3,__zpabi_calloc__size_0,__zpabi_calloc__size_1,__zpabi_calloc__size_2,__zpabi_calloc__size_3
; ext malloc params=__zpabi_malloc__size_0,__zpabi_malloc__size_1,__zpabi_malloc__size_2,__zpabi_malloc__size_3
; call main -> test_array_of_union_pointers
; call main -> test_array_of_unions
; call main -> test_auto_arrow
; call main -> test_auto_dot
; call main -> test_static_arrow
; call main -> test_static_dot
; call test_array_of_union_pointers -> calloc
; item test_auto_dot section=.text local=false zero=false
; item test_auto_dot@tail2 section=.text local=true zero=false
; item test_auto_dot@tail1 section=.text local=true zero=false
; item test_auto_dot@tail0 section=.text local=true zero=false
; item test_static_dot section=.text local=false zero=false
; item test_auto_arrow section=.text local=false zero=false
; item test_auto_arrow@run1 section=.text local=true zero=false
; item test_auto_arrow@run1@run2 section=.text local=true zero=false
; item test_static_arrow section=.text local=false zero=false
; item test_array_of_unions section=.text local=false zero=false
; item test_array_of_union_pointers section=.text local=false zero=false
; item test_array_of_union_pointers@run0 section=.text local=true zero=false
; item main section=.text local=false zero=false
; item @8.x section=.bss local=true zero=true
; item @9.y section=.bss local=true zero=true
; item @10.z section=.bss local=true zero=true
; item @14.inner section=.data local=true zero=false
; item @15.outer section=.bss local=true zero=true
; item __zp_init section=.text local=false zero=false
; zp @16.outer_ptr size=2 at=auto def=true init=$00,$00 weight=10 local=true
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

test_auto_dot:
   SUBROUTINE

   LDA   #$0D
   BPL   .test_auto_dot@jthread@0
   LDA   #$FF
   BNE   .test_auto_dot@jthread@0
   CMP   #$03
   BNE   .test_auto_dot@jthread@0
   LDA   #$0D
   CMP   #$0D
   BNE   .test_auto_dot@jthread@0
   LDA   #$40
   CMP   #$40
   BNE   .test_auto_dot@jthread@0
   LDA   __local_test_auto_dot__7
   ORA   __local_test_auto_dot__8
   ORA   __local_test_auto_dot__9
   ORA   __local_test_auto_dot__10
   ORA   __local_test_auto_dot__11
   ORA   __local_test_auto_dot__12
   ORA   __local_test_auto_dot__13
   ORA   __local_test_auto_dot__14
   BEQ   .if_end@6
   JMP   test_auto_dot@tail1
.test_auto_dot@jthread@0:
   JMP   test_auto_dot@tail2
.if_end@6:
   JMP   test_auto_dot@tail0

test_auto_dot@tail2:
   SUBROUTINE

   LDX   #$00
   TXA
   RTS

test_auto_dot@tail1:
   SUBROUTINE

   JMP   test_auto_dot@tail2

test_auto_dot@tail0:
   SUBROUTINE

   LDX   #$00
   LDA   #$01
   RTS

test_static_dot:
   SUBROUTINE

   LDA   #$40
   STA   @8.x
   LDA   #$03
   STA   @8.x+2
   LDA   #$00
   STA   @8.x+3
   LDA   #$0D
   STA   @8.x+1
   BMI   .lb_skip@0
   JMP   .tail@0
.lb_skip@0:
   LDA   #$FF
   BNE   .jcmp_skip@6
   CMP   #$03
   BNE   .jcmp_skip@6
   LDA   #$0D
   CMP   #$0D
   BNE   .jcmp_skip@6
   LDA   #$40
   CMP   #$40
   BEQ   .if_end@7
.jcmp_skip@6:
   JMP   test_auto_dot@tail1
.if_end@7:
   LDA   #$4D
   STA   @9.y
   LDA   #$EA
   STA   @9.y+1
   LDA   #$FF
   STA   @9.y+2
   STA   @9.y+3
   LDA   #$00
   STA   @9.y+4
   LDY   #$00
   STY   @9.y+5
   STY   @9.y+6
   STY   @9.y+7
   STY   @9.y+8
   STA   @9.y+9
   LDA   #$24
   STA   @9.y+10
   LDA   #$40
   STA   @9.y+11
   LDA   #$64
   STA   @9.y+12
   STY   @9.y+13
   LDA   #$FF
   CMP   #$FF
   BNE   .tail@0
   LDA   #$EA
   CMP   #$EA
   BNE   .tail@0
   LDA   #$4D
   CMP   #$4D
   BNE   .tail@0
   LDA   #$39
   STA   @10.z
   LDA   #$30
   STA   @10.z+1
   TYA
   STA   @10.z+4
   STY   @10.z+5
   STY   @10.z+6
   STA   @10.z+7
   LDA   #$39
   CMP   #$39
   BNE   .tail@0
   LDA   @10.z+8
   LDX   @10.z+9
   STX   __local_test_static_dot__6
   LDX   @10.z+10
   STX   __local_test_static_dot__5
   LDX   @10.z+11
   STX   __local_test_static_dot__4
   LDX   @10.z+12
   STX   __local_test_static_dot__3
   LDX   @10.z+13
   STX   __local_test_static_dot__2
   LDX   @10.z+14
   STX   __local_test_static_dot__1
   LDX   @10.z+15
   STX   __local_test_static_dot__0
   ORA   __local_test_static_dot__6
   ORA   __local_test_static_dot__5
   ORA   __local_test_static_dot__4
   ORA   __local_test_static_dot__3
   ORA   __local_test_static_dot__2
   ORA   __local_test_static_dot__1
   ORA   __local_test_static_dot__0
   BEQ   .if_end@10
.tail@0:
   JMP   test_auto_dot@tail2
.if_end@10:
   JMP   test_auto_dot@tail0

test_auto_arrow:
   SUBROUTINE

   LDA   #$64
   STA   __local_test_auto_arrow__inner
   LDA   #$00
   STA   __local_test_auto_arrow__inner+1
   LDA   #<__local_test_auto_arrow__inner
   STA   __local_test_auto_arrow__outer_0
   LDY   #>__local_test_auto_arrow__inner
   STY   __local_test_auto_arrow__outer_1
   STA   DPTR
   STY   DPTR+1
   LDY   #$00
   LDA   (__local_test_auto_arrow__outer_0),Y
   STA   __local_test_auto_arrow__1
   INY
   LDA   (__local_test_auto_arrow__outer_0),Y
   BNE   .jcmp_skip@8
   LDA   __local_test_auto_arrow__1
   CMP   #$64
   BEQ   .if_end@11
.jcmp_skip@8:
   JMP   test_auto_dot@tail1
.if_end@11:
   LDA   #$F6
   LDY   #$00
   STA   (__local_test_auto_arrow__outer_0),Y
   JSR   test_auto_arrow@run1
   LDY   #$00
   LDA   (__local_test_auto_arrow__outer_0),Y
   STA   __local_test_auto_arrow__0
   LDA   #$00
   CMP   #$FF
   BNE   .test_auto_arrow@jthread@3
   LDA   __local_test_auto_arrow__0
   CMP   #$F6
   BNE   .test_auto_arrow@jthread@3
   LDA   (__local_test_auto_arrow__outer_0),Y
   STA   __local_test_auto_arrow__1
   INY
   LDA   (__local_test_auto_arrow__outer_0),Y
   CMP   #$FF
   BNE   .test_auto_arrow@jthread@3
   LDA   __local_test_auto_arrow__1
   CMP   #$F6
   BNE   .test_auto_arrow@jthread@3
   DEY
   LDA   (__local_test_auto_arrow__outer_0),Y
   STA   __local_test_auto_arrow__3
   INY
   LDA   (__local_test_auto_arrow__outer_0),Y
   STA   __local_test_auto_arrow__2
   INY
   LDA   (__local_test_auto_arrow__outer_0),Y
   STA   __local_test_auto_arrow__1
   INY
   LDA   (__local_test_auto_arrow__outer_0),Y
   CMP   #$FF
   BNE   .test_auto_arrow@jthread@3
   LDA   __local_test_auto_arrow__1
   CMP   #$FF
   BNE   .test_auto_arrow@jthread@3
   LDA   __local_test_auto_arrow__2
   CMP   #$FF
   BNE   .test_auto_arrow@jthread@3
   LDA   __local_test_auto_arrow__3
   CMP   #$F6
   BNE   .test_auto_arrow@jthread@3
   LDY   #$00
   LDA   (__local_test_auto_arrow__outer_0),Y
   CMP   #$F6
   BNE   .test_auto_arrow@jthread@3
   INY
   LDA   (__local_test_auto_arrow__outer_0),Y
   CMP   #$FF
   BNE   .test_auto_arrow@jthread@3
   INY
   LDA   (__local_test_auto_arrow__outer_0),Y
   CMP   #$FF
   BEQ   .if_end@21
.test_auto_arrow@jthread@3:
   JMP   test_auto_dot@tail2
.if_end@21:
   JMP   test_auto_dot@tail0

test_auto_arrow@run1:
   SUBROUTINE

   LDA   #$FF
   LDY   #$01
   STA   (DPTR),Y
   JSR   test_auto_arrow@run1@run2
   RTS

test_auto_arrow@run1@run2:
   SUBROUTINE

   LDY   #$02
   STA   (DPTR),Y
   INY
   STA   (DPTR),Y
   RTS

test_static_arrow:
   SUBROUTINE

   LDA   #<@15.outer
   STA   @16.outer_ptr
   LDA   #>@15.outer
   STA   @16.outer_ptr+1
   LDA   #<@14.inner
   STA   @15.outer
   LDY   #>@14.inner
   STY   @15.outer+1
   STA   __local_test_static_arrow__2
   STY   __local_test_static_arrow__3
   LDY   #$00
   LDA   (__local_test_static_arrow__2),Y
   STA   __local_test_static_arrow__1
   INY
   LDA   (__local_test_static_arrow__2),Y
   BNE   .jcmp_skip@9
   LDA   __local_test_static_arrow__1
   CMP   #$64
   BEQ   .if_end@22
.jcmp_skip@9:
   JMP   test_auto_dot@tail1
.if_end@22:
   LDA   @15.outer
   STA   __local_test_static_arrow__0
   STA   DPTR
   LDA   @15.outer+1
   STA   __local_test_static_arrow__1
   STA   DPTR+1
   LDA   #$F6
   LDY   #$00
   STA   (__local_test_static_arrow__0),Y
   JSR   test_auto_arrow@run1
   LDY   #$00
   LDA   (@16.outer_ptr),Y
   STA   __local_test_static_arrow__0
   INY
   LDA   (@16.outer_ptr),Y
   STA   __local_test_static_arrow__1
   DEY
   LDA   (__local_test_static_arrow__0),Y
   STA   __local_test_static_arrow__0
   LDA   #$00
   CMP   #$FF
   BNE   .test_static_arrow@jthread@1
   LDA   __local_test_static_arrow__0
   CMP   #$F6
   BNE   .test_static_arrow@jthread@1
   LDA   (@16.outer_ptr),Y
   STA   __local_test_static_arrow__2
   INY
   LDA   (@16.outer_ptr),Y
   STA   __local_test_static_arrow__3
   DEY
   LDA   (__local_test_static_arrow__2),Y
   STA   __local_test_static_arrow__1
   INY
   LDA   (__local_test_static_arrow__2),Y
   CMP   #$FF
   BNE   .test_static_arrow@jthread@1
   LDA   __local_test_static_arrow__1
   CMP   #$F6
   BNE   .test_static_arrow@jthread@1
   DEY
   LDA   (@16.outer_ptr),Y
   STA   __local_test_static_arrow__4
   INY
   LDA   (@16.outer_ptr),Y
   STA   __local_test_static_arrow__5
   DEY
   LDA   (__local_test_static_arrow__4),Y
   STA   __local_test_static_arrow__3
   INY
   LDA   (__local_test_static_arrow__4),Y
   STA   __local_test_static_arrow__2
   INY
   LDA   (__local_test_static_arrow__4),Y
   STA   __local_test_static_arrow__1
   INY
   LDA   (__local_test_static_arrow__4),Y
   CMP   #$FF
   BNE   .test_static_arrow@jthread@3
   LDA   __local_test_static_arrow__1
   CMP   #$FF
   BNE   .test_static_arrow@jthread@3
   LDA   __local_test_static_arrow__2
   CMP   #$FF
   BNE   .test_static_arrow@jthread@3
   LDA   __local_test_static_arrow__3
   CMP   #$F6
   BEQ   .if_end@27
.test_static_arrow@jthread@1:
   JMP   test_auto_dot@tail2
.if_end@27:
   LDY   #$00
   LDA   (@16.outer_ptr),Y
   STA   __local_test_static_arrow__0
   INY
   LDA   (@16.outer_ptr),Y
   STA   __local_test_static_arrow__1
   DEY
   LDA   (__local_test_static_arrow__0),Y
   CMP   #$F6
   BNE   .test_static_arrow@jthread@3
   LDA   (@16.outer_ptr),Y
   STA   __local_test_static_arrow__0
   INY
   LDA   (@16.outer_ptr),Y
   STA   __local_test_static_arrow__1
   LDA   (__local_test_static_arrow__0),Y
   CMP   #$FF
   BNE   .test_static_arrow@jthread@3
   DEY
   LDA   (@16.outer_ptr),Y
   STA   __local_test_static_arrow__0
   INY
   LDA   (@16.outer_ptr),Y
   STA   __local_test_static_arrow__1
   INY
   LDA   (__local_test_static_arrow__0),Y
   CMP   #$FF
   BEQ   .if_end@32
.test_static_arrow@jthread@3:
   LDX   #$00
   TXA
   RTS
.if_end@32:
   JMP   test_auto_dot@tail0

test_array_of_unions:
   SUBROUTINE

   LDA   #$FF
   CMP   #$FF
   BNE   .test_array_of_unions@jthread@1
   LDA   #$D8
   CMP   #$D8
   BNE   .test_array_of_unions@jthread@1
   LDA   #$F0
   CMP   #$F0
   BNE   .test_array_of_unions@jthread@1
   LDA   #$00
   CMP   #$FF
   BNE   .test_array_of_unions@jthread@1
   LDA   #$C8
   CMP   #$C8
   BNE   .test_array_of_unions@jthread@1
   LDA   #$88
   CMP   #$88
   BEQ   .if_end@37
.test_array_of_unions@jthread@1:
   JMP   test_auto_dot@tail1
.if_end@37:
   JMP   test_auto_dot@tail0

test_array_of_union_pointers:
   SUBROUTINE

   LDA   #$00
   STA   __local_test_array_of_union_pointers__i_0
   LDY   #$00
   STY   __local_test_array_of_union_pointers__i_1
.loop@0_start:
   LDA   __local_test_array_of_union_pointers__i_0
   SEC
   SBC   #$03
   LDA   __local_test_array_of_union_pointers__i_1
   SBC   #$00
   BVC   .jcmp_novf@10
   EOR   #$80
.jcmp_novf@10:
   BMI   .lb_skip@3
   JMP   .loop@0_break
.lb_skip@3:
   LDA   #$01
   STA   __zpabi_calloc__nmemb_0
   LDA   #$08
   JSR   test_array_of_union_pointers@run0
   STA   __local_test_array_of_union_pointers__5
   LDA   __local_test_array_of_union_pointers__i_0
   ASL
   STA   __local_test_array_of_union_pointers__6
   LDA   #<__local_test_array_of_union_pointers__ptr_arr
   CLC
   ADC   __local_test_array_of_union_pointers__6
   STA   __local_test_array_of_union_pointers__0
   LDA   #>__local_test_array_of_union_pointers__ptr_arr
   ADC   #$00
   STA   __local_test_array_of_union_pointers__1
   LDA   __local_test_array_of_union_pointers__5
   LDY   #$00
   STA   (__local_test_array_of_union_pointers__0),Y
   TXA
   INY
   STA   (__local_test_array_of_union_pointers__0),Y
   LDA   #$01
   STA   __zpabi_calloc__nmemb_0
   LDA   #$04
   JSR   test_array_of_union_pointers@run0
   STA   __local_test_array_of_union_pointers__5
   LDA   #<__local_test_array_of_union_pointers__ptr_arr
   CLC
   ADC   __local_test_array_of_union_pointers__6
   STA   __local_test_array_of_union_pointers__2
   LDA   #>__local_test_array_of_union_pointers__ptr_arr
   ADC   #$00
   STA   __local_test_array_of_union_pointers__3
   LDY   #$00
   LDA   (__local_test_array_of_union_pointers__2),Y
   STA   __local_test_array_of_union_pointers__0
   STA   DPTR
   INY
   LDA   (__local_test_array_of_union_pointers__2),Y
   STA   __local_test_array_of_union_pointers__1
   STA   DPTR+1
   LDA   __local_test_array_of_union_pointers__5
   DEY
   STA   (__local_test_array_of_union_pointers__0),Y
   TXA
   INY
   STA   (DPTR),Y
   LDA   __local_test_array_of_union_pointers__i_1
   BMI   .sx_neg@11
   LDA   #$00
   BEQ   .sx_done@12
.sx_neg@11:
   LDA   #$FF
.sx_done@12:
   STA   __local_test_array_of_union_pointers__5
   LDA   #<__local_test_array_of_union_pointers__ptr_arr
   CLC
   ADC   __local_test_array_of_union_pointers__6
   STA   __local_test_array_of_union_pointers__0
   LDA   #>__local_test_array_of_union_pointers__ptr_arr
   ADC   #$00
   STA   __local_test_array_of_union_pointers__1
   LDY   #$00
   LDA   (__local_test_array_of_union_pointers__0),Y
   STA   __local_test_array_of_union_pointers__2
   INY
   LDA   (__local_test_array_of_union_pointers__0),Y
   STA   __local_test_array_of_union_pointers__3
   DEY
   LDA   (__local_test_array_of_union_pointers__2),Y
   STA   __local_test_array_of_union_pointers__0
   STA   DPTR
   INY
   LDA   (__local_test_array_of_union_pointers__2),Y
   STA   __local_test_array_of_union_pointers__1
   STA   DPTR+1
   LDA   __local_test_array_of_union_pointers__i_0
   DEY
   STA   (__local_test_array_of_union_pointers__0),Y
   LDA   __local_test_array_of_union_pointers__i_1
   INY
   STA   (DPTR),Y
   LDA   __local_test_array_of_union_pointers__5
   JSR   test_auto_arrow@run1@run2
   INC   __local_test_array_of_union_pointers__i_0
   BEQ   .lb_skip@2
   JMP   .loop@0_start
.lb_skip@2:
   INC   __local_test_array_of_union_pointers__i_1
   JMP   .loop@0_start
.loop@0_break:
   LDA   __local_test_array_of_union_pointers__ptr_arr
   STA   __local_test_array_of_union_pointers__0
   LDA   __local_test_array_of_union_pointers__ptr_arr+1
   STA   __local_test_array_of_union_pointers__1
   LDY   #$00
   LDA   (__local_test_array_of_union_pointers__0),Y
   STA   __local_test_array_of_union_pointers__4
   INY
   LDA   (__local_test_array_of_union_pointers__0),Y
   STA   __local_test_array_of_union_pointers__5
   LDA   (__local_test_array_of_union_pointers__4),Y
   STA   __local_test_array_of_union_pointers__2
   INY
   LDA   (__local_test_array_of_union_pointers__4),Y
   STA   __local_test_array_of_union_pointers__1
   INY
   LDA   (__local_test_array_of_union_pointers__4),Y
   STA   __local_test_array_of_union_pointers__0
   LDY   #$00
   LDA   (__local_test_array_of_union_pointers__4),Y
   ORA   __local_test_array_of_union_pointers__2
   ORA   __local_test_array_of_union_pointers__1
   ORA   __local_test_array_of_union_pointers__0
   BEQ   .lb_skip@1
   JMP   .test_array_of_union_pointers@jthread@1
.lb_skip@1:
   LDA   __local_test_array_of_union_pointers__ptr_arr+2
   STA   __local_test_array_of_union_pointers__0
   LDA   __local_test_array_of_union_pointers__ptr_arr+3
   STA   __local_test_array_of_union_pointers__1
   LDY   #$00
   LDA   (__local_test_array_of_union_pointers__0),Y
   STA   __local_test_array_of_union_pointers__4
   INY
   LDA   (__local_test_array_of_union_pointers__0),Y
   STA   __local_test_array_of_union_pointers__5
   DEY
   LDA   (__local_test_array_of_union_pointers__4),Y
   STA   __local_test_array_of_union_pointers__3
   INY
   LDA   (__local_test_array_of_union_pointers__4),Y
   STA   __local_test_array_of_union_pointers__2
   INY
   LDA   (__local_test_array_of_union_pointers__4),Y
   STA   __local_test_array_of_union_pointers__1
   INY
   LDA   (__local_test_array_of_union_pointers__4),Y
   BNE   .test_array_of_union_pointers@jthread@1
   LDA   __local_test_array_of_union_pointers__1
   BNE   .test_array_of_union_pointers@jthread@1
   LDA   __local_test_array_of_union_pointers__2
   BNE   .test_array_of_union_pointers@jthread@1
   LDA   __local_test_array_of_union_pointers__3
   CMP   #$01
   BNE   .test_array_of_union_pointers@jthread@1
   LDA   __local_test_array_of_union_pointers__ptr_arr+4
   STA   __local_test_array_of_union_pointers__0
   LDA   __local_test_array_of_union_pointers__ptr_arr+5
   STA   __local_test_array_of_union_pointers__1
   LDY   #$00
   LDA   (__local_test_array_of_union_pointers__0),Y
   STA   __local_test_array_of_union_pointers__4
   INY
   LDA   (__local_test_array_of_union_pointers__0),Y
   STA   __local_test_array_of_union_pointers__5
   DEY
   LDA   (__local_test_array_of_union_pointers__4),Y
   STA   __local_test_array_of_union_pointers__3
   INY
   LDA   (__local_test_array_of_union_pointers__4),Y
   STA   __local_test_array_of_union_pointers__2
   INY
   LDA   (__local_test_array_of_union_pointers__4),Y
   STA   __local_test_array_of_union_pointers__1
   INY
   LDA   (__local_test_array_of_union_pointers__4),Y
   BNE   .test_array_of_union_pointers@jthread@1
   LDA   __local_test_array_of_union_pointers__1
   BNE   .test_array_of_union_pointers@jthread@1
   LDA   __local_test_array_of_union_pointers__2
   BNE   .test_array_of_union_pointers@jthread@1
   LDA   __local_test_array_of_union_pointers__3
   CMP   #$02
   BEQ   .if_end@42
.test_array_of_union_pointers@jthread@1:
   JMP   test_auto_dot@tail1
.if_end@42:
   JMP   test_auto_dot@tail0

test_array_of_union_pointers@run0:
   SUBROUTINE

   STA   __zpabi_calloc__size_0
   LDA   #$00
   STA   __zpabi_calloc__nmemb_1
   STA   __zpabi_calloc__nmemb_2
   STA   __zpabi_calloc__nmemb_3
   STA   __zpabi_calloc__size_1
   STA   __zpabi_calloc__size_2
   STA   __zpabi_calloc__size_3
   JMP   calloc

main:
   SUBROUTINE

   JSR   test_auto_dot
   STX   __local_main__0
   ORA   __local_main__0
   BNE   .if_end@43
   JMP   test_auto_dot@tail0
.if_end@43:
   JSR   test_static_dot
   STX   __local_main__0
   ORA   __local_main__0
   BNE   .if_end@44
   LDX   #$00
   LDA   #$02
   RTS
.if_end@44:
   JSR   test_auto_arrow
   STX   __local_main__0
   ORA   __local_main__0
   BNE   .if_end@45
   LDX   #$00
   LDA   #$03
   RTS
.if_end@45:
   JSR   test_static_arrow
   STX   __local_main__0
   ORA   __local_main__0
   BNE   .if_end@46
   LDX   #$00
   LDA   #$04
   RTS
.if_end@46:
   JSR   test_array_of_unions
   STX   __local_main__0
   ORA   __local_main__0
   BNE   .if_end@47
   LDX   #$00
   LDA   #$05
   RTS
.if_end@47:
   JSR   test_array_of_union_pointers
   STX   __local_main__0
   ORA   __local_main__0
   BNE   .if_end@48
   LDX   #$00
   LDA   #$06
   RTS
.if_end@48:
   JMP   test_auto_dot@tail1

@8.x:
   DS.B  8

@9.y:
   DS.B  14

@10.z:
   DS.B  16

@14.inner:
   DC.W  $0064
   DS.B  2

@15.outer:
   DS.B  8

__zp_init:
   SUBROUTINE

   LDA   #$00
   STA   @16.outer_ptr
   STA   @16.outer_ptr+1
   RTS
