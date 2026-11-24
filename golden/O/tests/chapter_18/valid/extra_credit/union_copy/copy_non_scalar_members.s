SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__zpabi_calloc__nmemb_0	EQU	$80
__zpabi_malloc__size_0	EQU	$80
__zpabi_calloc__nmemb_1	EQU	$81
__zpabi_malloc__size_1	EQU	$81
__zpabi_calloc__nmemb_2	EQU	$82
__zpabi_malloc__size_2	EQU	$82
__zpabi_calloc__nmemb_3	EQU	$83
__zpabi_malloc__size_3	EQU	$83
__zpabi_calloc__size_0	EQU	$84
__zpabi_calloc__size_1	EQU	$85
__zpabi_calloc__size_2	EQU	$86
__zpabi_calloc__size_3	EQU	$87
__local_main__0	EQU	$88
__local_test_arrow__0	EQU	$8B
__local_test_dot__my_simple_union_0	EQU	$8B
__local_test_arrow__1	EQU	$8C
__local_test_dot__my_simple_union_3	EQU	$8C
__local_test_arrow__2	EQU	$8D
__local_test_dot__my_simple_union_2	EQU	$8D
__local_test_arrow__3	EQU	$8E
__local_test_arrow__4	EQU	$8F
__local_test_arrow__small_union_1	EQU	$90
__local_test_dot__my_struct	EQU	$90
__local_test_arrow__5	EQU	$91
__local_test_arrow__another_union_0	EQU	$92
__local_test_arrow__another_union_1	EQU	$93
__local_test_arrow__6	EQU	$94
__local_test_arrow__7	EQU	$95

; @zp-link-meta-begin
; def main params= locals=__local_main__0,__local_main__1 indirect=false in_cycle=false
; def test_arrow params= locals=__local_test_arrow__small_union_0,__local_test_arrow__0,__local_test_arrow__1,__local_test_arrow__2,__local_test_arrow__3,__local_test_arrow__4,__local_test_arrow__small_union_1,__local_test_arrow__5,__local_test_arrow__another_union_0,__local_test_arrow__another_union_1,__local_test_arrow__6,__local_test_arrow__7 indirect=false in_cycle=false
; def test_dot params= locals=__local_test_dot__my_simple_union_1,__local_test_dot__my_simple_union_0,__local_test_dot__my_simple_union_3,__local_test_dot__my_simple_union_2,__local_test_dot__0,__local_test_dot__1,__local_test_dot__my_struct,__local_test_dot__3,__local_test_dot__4,__local_test_dot__5,__local_test_dot__6,__local_test_dot__7,__local_test_dot__8,__local_test_dot__9 indirect=false in_cycle=false
; ext calloc params=__zpabi_calloc__nmemb_0,__zpabi_calloc__nmemb_1,__zpabi_calloc__nmemb_2,__zpabi_calloc__nmemb_3,__zpabi_calloc__size_0,__zpabi_calloc__size_1,__zpabi_calloc__size_2,__zpabi_calloc__size_3
; ext malloc params=__zpabi_malloc__size_0,__zpabi_malloc__size_1,__zpabi_malloc__size_2,__zpabi_malloc__size_3
; call main -> test_arrow
; call main -> test_dot
; call test_arrow -> calloc
; item test_dot section=.text local=false zero=false
; item test_arrow section=.text local=false zero=false
; item test_arrow@run0 section=.text local=true zero=false
; item main section=.text local=false zero=false
; item @7.my_union section=.bss local=true zero=true
; item @8.another_struct section=.bss local=true zero=true
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

test_dot:
   SUBROUTINE

   LDY   #$FF
   TYA
   STA   __local_test_dot__my_struct
   STA   @7.my_union
   STA   @7.my_union+1
   STA   @7.my_union+2
   STA   @7.my_union+3
   LDA   #$A0
   STA   @7.my_union+4
   LDA   #$86
   STA   @7.my_union+5
   LDA   #$01
   STA   @7.my_union+6
   LDX   #$00
   STX   @7.my_union+7
   BNE   .test_dot@jthread@0
   CMP   #$01
   BNE   .test_dot@jthread@0
   LDA   #$86
   CMP   #$86
   BNE   .test_dot@jthread@0
   LDA   #$A0
   CMP   #$A0
   BNE   .test_dot@jthread@0
   LDA   __local_test_dot__my_struct
   CMP   #$FF
   BNE   .test_dot@jthread@0
   CPY   #$FF
   BNE   .test_dot@jthread@0
   CMP   #$FF
   BEQ   .if_end@2
.test_dot@jthread@0:
   TXA
   RTS
.if_end@2:
   LDA   @7.my_union+4
   STA   __local_test_dot__my_simple_union_2
   LDA   @7.my_union+5
   STA   __local_test_dot__my_simple_union_3
   LDA   @7.my_union+6
   STA   __local_test_dot__my_simple_union_0
   TXA
   BNE   .jcmp_skip@0
   LDA   __local_test_dot__my_simple_union_0
   CMP   #$01
   BNE   .test_dot@jthread@1
   LDA   __local_test_dot__my_simple_union_3
   CMP   #$86
   BNE   .test_dot@jthread@1
   LDA   __local_test_dot__my_simple_union_2
   CMP   #$A0
   BEQ   .if_end@3
.jcmp_skip@0:
   TXA
   RTS
.if_end@3:
   LDA   #$2D
   STA   @7.my_union
   LDY   #$00
   STY   @7.my_union+1
   TXA
   BNE   .test_dot@jthread@1
   LDA   #$2D
   CMP   #$2D
   BNE   .test_dot@jthread@1
   LDA   @7.my_union
   STA   @8.another_struct
   STY   @8.another_struct+1
   LDA   @7.my_union+2
   STA   @8.another_struct+2
   STA   @8.another_struct+3
   LDA   @7.my_union+4
   STA   @8.another_struct+4
   LDA   @7.my_union+5
   STA   @8.another_struct+5
   LDA   @7.my_union+6
   STA   @8.another_struct+6
   TXA
   STA   @8.another_struct+7
   BNE   .test_dot@jthread@1
   LDA   __local_test_dot__my_simple_union_0
   CMP   #$01
   BNE   .test_dot@jthread@1
   LDA   __local_test_dot__my_simple_union_3
   CMP   #$86
   BNE   .test_dot@jthread@1
   LDA   __local_test_dot__my_simple_union_2
   CMP   #$A0
   BNE   .test_dot@jthread@1
   LDA   @8.another_struct
   STA   __local_test_dot__my_simple_union_0
   TYA
   BNE   .test_dot@jthread@1
   LDA   __local_test_dot__my_simple_union_0
   CMP   #$2D
   BEQ   .if_end@7
.test_dot@jthread@1:
   TXA
   RTS
.if_end@7:
   LDA   #$01
   RTS

test_arrow:
   SUBROUTINE

   LDA   #$01
   STA   __zpabi_calloc__nmemb_0
   LDA   #$10
   JSR   test_arrow@run0
   STA   __local_test_arrow__6
   STX   __local_test_arrow__7
   LDA   #$01
   STA   __zpabi_calloc__nmemb_0
   LDA   #$08
   JSR   test_arrow@run0
   LDY   __local_test_arrow__6
   STY   DPTR
   LDY   __local_test_arrow__7
   STY   DPTR+1
   LDY   #$00
   STA   (__local_test_arrow__6),Y
   TXA
   INY
   STA   (DPTR),Y
   LDA   #$01
   STA   __zpabi_calloc__nmemb_0
   LDA   #$04
   JSR   test_arrow@run0
   STA   __local_test_arrow__5
   LDY   #$00
   LDA   (__local_test_arrow__6),Y
   STA   __local_test_arrow__3
   STA   DPTR
   INY
   LDA   (__local_test_arrow__6),Y
   STA   __local_test_arrow__4
   STA   DPTR+1
   LDA   __local_test_arrow__5
   DEY
   STA   (__local_test_arrow__3),Y
   TXA
   INY
   STA   (DPTR),Y
   DEY
   LDA   (__local_test_arrow__6),Y
   STA   __local_test_arrow__small_union_1
   INY
   LDA   (__local_test_arrow__6),Y
   STA   __local_test_arrow__5
   DEY
   LDA   (__local_test_arrow__small_union_1),Y
   STA   __local_test_arrow__3
   STA   DPTR
   INY
   LDA   (__local_test_arrow__small_union_1),Y
   STA   __local_test_arrow__4
   STA   DPTR+1
   LDA   #$B1
   DEY
   STA   (__local_test_arrow__3),Y
   LDA   #$68
   INY
   STA   (DPTR),Y
   DEY
   LDA   (__local_test_arrow__6),Y
   STA   __local_test_arrow__3
   INY
   LDA   (__local_test_arrow__6),Y
   STA   __local_test_arrow__4
   DEY
   LDA   (__local_test_arrow__3),Y
   STA   __local_test_arrow__another_union_0
   INY
   LDA   (__local_test_arrow__3),Y
   STA   __local_test_arrow__another_union_1
   DEY
   LDA   (__local_test_arrow__6),Y
   STA   __local_test_arrow__3
   INY
   LDA   (__local_test_arrow__6),Y
   STA   __local_test_arrow__4
   DEY
   LDA   (__local_test_arrow__3),Y
   STA   __local_test_arrow__small_union_1
   INY
   LDA   (__local_test_arrow__3),Y
   STA   __local_test_arrow__5
   LDA   __local_test_arrow__another_union_1
   CMP   __local_test_arrow__5
   BNE   .jcmp_skip@2
   LDA   __local_test_arrow__another_union_0
   CMP   __local_test_arrow__small_union_1
   BNE   .jcmp_skip@2
   DEY
   LDA   (__local_test_arrow__small_union_1),Y
   STA   __local_test_arrow__3
   LDA   (__local_test_arrow__another_union_0),Y
   CMP   __local_test_arrow__3
   BNE   .jcmp_skip@2
   LDA   (__local_test_arrow__6),Y
   STA   __local_test_arrow__3
   STA   DPTR
   INY
   LDA   (__local_test_arrow__6),Y
   STA   __local_test_arrow__4
   STA   DPTR+1
   LDA   #$F1
   DEY
   STA   (__local_test_arrow__3),Y
   LDA   #$D8
   INY
   STA   (DPTR),Y
   LDA   __local_test_arrow__1
   INY
   STA   (DPTR),Y
   LDA   __local_test_arrow__2
   INY
   STA   (DPTR),Y
   LDY   #$00
   LDA   (__local_test_arrow__6),Y
   STA   __local_test_arrow__1
   INY
   LDA   (__local_test_arrow__6),Y
   STA   __local_test_arrow__2
   DEY
   LDA   (__local_test_arrow__1),Y
   STA   __local_test_arrow__0
   INY
   LDA   (__local_test_arrow__1),Y
   CMP   #$D8
   BNE   .jcmp_skip@2
   LDA   __local_test_arrow__0
   CMP   #$F1
   BEQ   .if_end@11
.jcmp_skip@2:
   LDX   #$00
   TXA
   RTS
.if_end@11:
   LDX   #$00
   LDA   #$01
   RTS

test_arrow@run0:
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

   JSR   test_dot
   STX   __local_main__0
   ORA   __local_main__0
   BNE   .if_end@12
   LDX   #$00
   LDA   #$01
   RTS
.if_end@12:
   JSR   test_arrow
   STX   __local_main__0
   ORA   __local_main__0
   BNE   .if_end@13
   LDX   #$00
   LDA   #$02
   RTS
.if_end@13:
   LDX   #$00
   TXA
   RTS

@7.my_union:
   DS.B  16

@8.another_struct:
   DS.B  8
