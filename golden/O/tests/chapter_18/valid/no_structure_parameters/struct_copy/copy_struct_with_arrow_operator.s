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
__local_test_copy_from_member_pointer__0	EQU	$8A
__local_test_copy_from_nested_member_pointer__small_9	EQU	$8A
__local_test_copy_to_member_pointer__small_9	EQU	$8A
__local_test_copy_to_nested_member_pointer__0	EQU	$8A
__local_test_mixed_nested_access__0	EQU	$8A
__local_test_copy_from_member_pointer__1	EQU	$8B
__local_test_copy_from_nested_member_pointer__small_8	EQU	$8B
__local_test_copy_to_member_pointer__small_8	EQU	$8B
__local_test_copy_to_nested_member_pointer__1	EQU	$8B
__local_test_member_from_cast__small_9	EQU	$8B
__local_test_mixed_nested_access__1	EQU	$8B
__local_test_copy_from_member_pointer__2	EQU	$8C
__local_test_copy_to_member_pointer__small_7	EQU	$8C
__local_test_copy_to_nested_member_pointer__2	EQU	$8C
__local_test_member_from_cast__small_8	EQU	$8C
__local_test_mixed_nested_access__2	EQU	$8C
__local_test_copy_from_nested_member_pointer__small_6	EQU	$8D
__local_test_copy_to_member_pointer__small_6	EQU	$8D
__local_test_copy_to_nested_member_pointer__3	EQU	$8D
__local_test_member_from_cast__small_7	EQU	$8D
__local_test_mixed_nested_access__3	EQU	$8D
__local_test_copy_from_nested_member_pointer__small_5	EQU	$8E
__local_test_copy_to_member_pointer__small_5	EQU	$8E
__local_test_copy_to_nested_member_pointer__4	EQU	$8E
__local_test_member_from_cast__small_6	EQU	$8E
__local_test_mixed_nested_access__4	EQU	$8E
__local_test_copy_from_nested_member_pointer__small_4	EQU	$8F
__local_test_copy_to_member_pointer__small_4	EQU	$8F
__local_test_copy_to_nested_member_pointer__small_9	EQU	$8F
__local_test_member_from_cast__small_5	EQU	$8F
__local_test_mixed_nested_access__5	EQU	$8F
__local_test_copy_from_nested_member_pointer__small_3	EQU	$90
__local_test_copy_to_member_pointer__small_3	EQU	$90
__local_test_copy_to_nested_member_pointer__small_8	EQU	$90
__local_test_member_from_cast__small_4	EQU	$90
__local_test_mixed_nested_access__6	EQU	$90
__local_test_copy_from_nested_member_pointer__small_2	EQU	$91
__local_test_copy_to_member_pointer__small_2	EQU	$91
__local_test_copy_to_nested_member_pointer__small_7	EQU	$91
__local_test_member_from_cast__small_3	EQU	$91
__local_test_mixed_nested_access__7	EQU	$91
__local_test_copy_from_member_pointer__small	EQU	$92
__local_test_copy_from_nested_member_pointer__small_1	EQU	$92
__local_test_copy_to_member_pointer__0	EQU	$92
__local_test_copy_to_nested_member_pointer__small_6	EQU	$92
__local_test_member_from_cast__1	EQU	$92
__local_test_mixed_nested_access__8	EQU	$92
__local_test_copy_from_nested_member_pointer__small_0	EQU	$93
__local_test_copy_to_member_pointer__1	EQU	$93
__local_test_copy_to_nested_member_pointer__small_5	EQU	$93
__local_test_member_from_cast__2	EQU	$93
__local_test_mixed_nested_access__9	EQU	$93
__local_test_copy_from_nested_member_pointer__0	EQU	$94
__local_test_copy_to_nested_member_pointer__small_4	EQU	$94
__local_test_member_from_cast__small_2	EQU	$94
__local_test_mixed_nested_access__10	EQU	$94
__local_test_copy_from_nested_member_pointer__1	EQU	$95
__local_test_copy_to_nested_member_pointer__small_3	EQU	$95
__local_test_member_from_cast__small_1	EQU	$95
__local_test_mixed_nested_access__11	EQU	$95
__local_test_copy_from_nested_member_pointer__2	EQU	$96
__local_test_copy_to_member_pointer__small_1	EQU	$96
__local_test_copy_to_nested_member_pointer__small_2	EQU	$96
__local_test_member_from_cast__small_0	EQU	$96
__local_test_mixed_nested_access__12	EQU	$96
__local_test_copy_from_nested_member_pointer__3	EQU	$97
__local_test_copy_to_member_pointer__small_0	EQU	$97
__local_test_copy_to_nested_member_pointer__small_1	EQU	$97
__local_test_mixed_nested_access__s1	EQU	$97
__local_test_copy_to_nested_member_pointer__small_0	EQU	$98

; @zp-link-meta-begin
; def main params= locals=__local_main__0,__local_main__1 indirect=false in_cycle=false
; def test_copy_from_member_pointer params= locals=__local_test_copy_from_member_pointer__0,__local_test_copy_from_member_pointer__1,__local_test_copy_from_member_pointer__2,__local_test_copy_from_member_pointer__3,__local_test_copy_from_member_pointer__4,__local_test_copy_from_member_pointer__5,__local_test_copy_from_member_pointer__6,__local_test_copy_from_member_pointer__7,__local_test_copy_from_member_pointer__small,__local_test_copy_from_member_pointer__9,__local_test_copy_from_member_pointer__10,__local_test_copy_from_member_pointer__11,__local_test_copy_from_member_pointer__12,__local_test_copy_from_member_pointer__13,__local_test_copy_from_member_pointer__14,__local_test_copy_from_member_pointer__15,__local_test_copy_from_member_pointer__16,__local_test_copy_from_member_pointer__17 indirect=false in_cycle=false
; def test_copy_from_nested_member_pointer params= locals=__local_test_copy_from_nested_member_pointer__small_9,__local_test_copy_from_nested_member_pointer__small_8,__local_test_copy_from_nested_member_pointer__small_7,__local_test_copy_from_nested_member_pointer__small_6,__local_test_copy_from_nested_member_pointer__small_5,__local_test_copy_from_nested_member_pointer__small_4,__local_test_copy_from_nested_member_pointer__small_3,__local_test_copy_from_nested_member_pointer__small_2,__local_test_copy_from_nested_member_pointer__small_1,__local_test_copy_from_nested_member_pointer__small_0,__local_test_copy_from_nested_member_pointer__0,__local_test_copy_from_nested_member_pointer__1,__local_test_copy_from_nested_member_pointer__2,__local_test_copy_from_nested_member_pointer__3 indirect=false in_cycle=false
; def test_copy_to_member_pointer params= locals=__local_test_copy_to_member_pointer__small_9,__local_test_copy_to_member_pointer__small_8,__local_test_copy_to_member_pointer__small_7,__local_test_copy_to_member_pointer__small_6,__local_test_copy_to_member_pointer__small_5,__local_test_copy_to_member_pointer__small_4,__local_test_copy_to_member_pointer__small_3,__local_test_copy_to_member_pointer__small_2,__local_test_copy_to_member_pointer__0,__local_test_copy_to_member_pointer__1,__local_test_copy_to_member_pointer__2,__local_test_copy_to_member_pointer__3,__local_test_copy_to_member_pointer__small_1,__local_test_copy_to_member_pointer__small_0,__local_test_copy_to_member_pointer__4,__local_test_copy_to_member_pointer__5,__local_test_copy_to_member_pointer__6,__local_test_copy_to_member_pointer__7,__local_test_copy_to_member_pointer__8,__local_test_copy_to_member_pointer__9,__local_test_copy_to_member_pointer__10,__local_test_copy_to_member_pointer__11 indirect=false in_cycle=false
; def test_copy_to_nested_member_pointer params= locals=__local_test_copy_to_nested_member_pointer__0,__local_test_copy_to_nested_member_pointer__1,__local_test_copy_to_nested_member_pointer__2,__local_test_copy_to_nested_member_pointer__3,__local_test_copy_to_nested_member_pointer__4,__local_test_copy_to_nested_member_pointer__small_9,__local_test_copy_to_nested_member_pointer__small_8,__local_test_copy_to_nested_member_pointer__small_7,__local_test_copy_to_nested_member_pointer__small_6,__local_test_copy_to_nested_member_pointer__small_5,__local_test_copy_to_nested_member_pointer__small_4,__local_test_copy_to_nested_member_pointer__small_3,__local_test_copy_to_nested_member_pointer__small_2,__local_test_copy_to_nested_member_pointer__small_1,__local_test_copy_to_nested_member_pointer__small_0,__local_test_copy_to_nested_member_pointer__5,__local_test_copy_to_nested_member_pointer__6,__local_test_copy_to_nested_member_pointer__7,__local_test_copy_to_nested_member_pointer__8,__local_test_copy_to_nested_member_pointer__9,__local_test_copy_to_nested_member_pointer__10,__local_test_copy_to_nested_member_pointer__11 indirect=false in_cycle=false
; def test_member_from_cast params= locals=__local_test_member_from_cast__0,__local_test_member_from_cast__small_9,__local_test_member_from_cast__small_8,__local_test_member_from_cast__small_7,__local_test_member_from_cast__small_6,__local_test_member_from_cast__small_5,__local_test_member_from_cast__small_4,__local_test_member_from_cast__small_3,__local_test_member_from_cast__1,__local_test_member_from_cast__2,__local_test_member_from_cast__small_2,__local_test_member_from_cast__small_1,__local_test_member_from_cast__small_0,__local_test_member_from_cast__3,__local_test_member_from_cast__4,__local_test_member_from_cast__5,__local_test_member_from_cast__6,__local_test_member_from_cast__7,__local_test_member_from_cast__8,__local_test_member_from_cast__9 indirect=false in_cycle=false
; def test_mixed_nested_access params= locals=__local_test_mixed_nested_access__0,__local_test_mixed_nested_access__1,__local_test_mixed_nested_access__2,__local_test_mixed_nested_access__3,__local_test_mixed_nested_access__4,__local_test_mixed_nested_access__5,__local_test_mixed_nested_access__6,__local_test_mixed_nested_access__7,__local_test_mixed_nested_access__8,__local_test_mixed_nested_access__9,__local_test_mixed_nested_access__10,__local_test_mixed_nested_access__11,__local_test_mixed_nested_access__12,__local_test_mixed_nested_access__s1,__local_test_mixed_nested_access__14,__local_test_mixed_nested_access__15,__local_test_mixed_nested_access__16,__local_test_mixed_nested_access__17,__local_test_mixed_nested_access__18,__local_test_mixed_nested_access__19,__local_test_mixed_nested_access__20,__local_test_mixed_nested_access__21,__local_test_mixed_nested_access__22,__local_test_mixed_nested_access__23,__local_test_mixed_nested_access__24,__local_test_mixed_nested_access__25,__local_test_mixed_nested_access__26,__local_test_mixed_nested_access__27,__local_test_mixed_nested_access__28,__local_test_mixed_nested_access__29,__local_test_mixed_nested_access__30 indirect=false in_cycle=false
; ext calloc params=__zpabi_calloc__nmemb_0,__zpabi_calloc__nmemb_1,__zpabi_calloc__nmemb_2,__zpabi_calloc__nmemb_3,__zpabi_calloc__size_0,__zpabi_calloc__size_1,__zpabi_calloc__size_2,__zpabi_calloc__size_3
; ext malloc params=__zpabi_malloc__size_0,__zpabi_malloc__size_1,__zpabi_malloc__size_2,__zpabi_malloc__size_3
; call main -> test_copy_from_member_pointer
; call main -> test_copy_from_nested_member_pointer
; call main -> test_copy_to_member_pointer
; call main -> test_copy_to_nested_member_pointer
; call main -> test_member_from_cast
; call main -> test_mixed_nested_access
; call test_copy_from_member_pointer -> malloc
; call test_copy_from_nested_member_pointer -> calloc
; call test_copy_to_member_pointer -> calloc
; call test_copy_to_nested_member_pointer -> calloc
; call test_member_from_cast -> calloc
; call test_mixed_nested_access -> calloc
; item test_copy_from_member_pointer section=.text local=false zero=false
; item test_copy_from_member_pointer@tail0 section=.text local=true zero=false
; item test_copy_to_member_pointer section=.text local=false zero=false
; item test_copy_to_member_pointer@run4 section=.text local=true zero=false
; item test_copy_to_member_pointer@run2 section=.text local=true zero=false
; item test_copy_to_member_pointer@run0 section=.text local=true zero=false
; item test_copy_from_nested_member_pointer section=.text local=false zero=false
; item test_copy_from_nested_member_pointer@run3 section=.text local=true zero=false
; item test_copy_from_nested_member_pointer@run1 section=.text local=true zero=false
; item test_copy_from_nested_member_pointer@run1@run5 section=.text local=true zero=false
; item test_copy_from_nested_member_pointer@tail1 section=.text local=true zero=false
; item test_copy_to_nested_member_pointer section=.text local=false zero=false
; item test_mixed_nested_access section=.text local=false zero=false
; item test_member_from_cast section=.text local=false zero=false
; item main section=.text local=false zero=false
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

test_copy_from_member_pointer:
   SUBROUTINE

   LDY   #$0A
   LDA   #$00
.clear@0:
   DEY
   STA   __local_test_copy_from_member_pointer__small,Y
   BNE   .clear@0
   LDA   #$0C
   STA   __zpabi_malloc__size_0
   LDA   #$00
   STA   __zpabi_malloc__size_1
   STA   __zpabi_malloc__size_2
   STA   __zpabi_malloc__size_3
   JSR   malloc
   STA   __local_test_copy_from_member_pointer__0
   STX   __local_test_copy_from_member_pointer__1
   LDA   #$64
   LDY   #$00
   STA   (__local_test_copy_from_member_pointer__0),Y
   LDA   __local_test_copy_from_member_pointer__0
   CLC
   ADC   #$02
   STA   __local_test_copy_from_member_pointer__0
   BCC   .carry_inc@0
   INC   __local_test_copy_from_member_pointer__1
.carry_inc@0:
   LDA   #$00
   LDY   #$00
   STA   (__local_test_copy_from_member_pointer__0),Y
   INY
   STA   (__local_test_copy_from_member_pointer__0),Y
   INY
   STA   (__local_test_copy_from_member_pointer__0),Y
   INY
   STA   (__local_test_copy_from_member_pointer__0),Y
   INY
   STA   (__local_test_copy_from_member_pointer__0),Y
   LDA   #$80
   INY
   STA   (__local_test_copy_from_member_pointer__0),Y
   LDA   #$35
   INY
   STA   (__local_test_copy_from_member_pointer__0),Y
   LDA   #$40
   INY
   STA   (__local_test_copy_from_member_pointer__0),Y
   LDA   #$A1
   INY
   STA   (__local_test_copy_from_member_pointer__0),Y
   LDA   #$86
   INY
   STA   (__local_test_copy_from_member_pointer__0),Y
   LDY   #$07
   LDA   (__local_test_copy_from_member_pointer__0),Y
   STA   __local_test_copy_from_member_pointer__small+7
   LDY   #$05
   LDA   (__local_test_copy_from_member_pointer__0),Y
   STA   __local_test_copy_from_member_pointer__2
   INY
   LDA   (__local_test_copy_from_member_pointer__0),Y
   STA   __local_test_copy_from_member_pointer__1
   LDA   __local_test_copy_from_member_pointer__small+7
   CMP   #$40
   BNE   .test_copy_from_member_pointer@jthread@0
   LDA   __local_test_copy_from_member_pointer__1
   CMP   #$35
   BNE   .test_copy_from_member_pointer@jthread@0
   LDA   __local_test_copy_from_member_pointer__2
   CMP   #$80
.test_copy_from_member_pointer@jthread@0:
   JMP   test_copy_from_member_pointer@tail0

test_copy_from_member_pointer@tail0:
   SUBROUTINE

   LDX   #$00
   TXA
   RTS

test_copy_to_member_pointer:
   SUBROUTINE

   LDA   #$00
   STA   __local_test_copy_to_member_pointer__small_0
   LDY   #$00
   STY   __local_test_copy_to_member_pointer__small_1
   STY   __local_test_copy_to_member_pointer__small_2
   STY   __local_test_copy_to_member_pointer__small_3
   STA   __local_test_copy_to_member_pointer__small_4
   LDA   #$D0
   STA   __local_test_copy_to_member_pointer__small_5
   LDA   #$58
   STA   __local_test_copy_to_member_pointer__small_6
   LDA   #$40
   STA   __local_test_copy_to_member_pointer__small_7
   LDA   #$06
   STA   __local_test_copy_to_member_pointer__small_8
   LDA   #$12
   STA   __local_test_copy_to_member_pointer__small_9
   JSR   test_copy_to_member_pointer@run4
   CLC
   ADC   #$02
   STA   __local_test_copy_to_member_pointer__0
   TXA
   ADC   #$00
   STA   __local_test_copy_to_member_pointer__1
   STA   DPTR+1
   LDA   __local_test_copy_to_member_pointer__0
   STA   DPTR
   LDA   __local_test_copy_to_member_pointer__small_0
   LDY   #$00
   STA   (__local_test_copy_to_member_pointer__0),Y
   LDA   __local_test_copy_to_member_pointer__small_1
   INY
   STA   (DPTR),Y
   LDA   __local_test_copy_to_member_pointer__small_2
   INY
   STA   (DPTR),Y
   LDA   __local_test_copy_to_member_pointer__small_3
   INY
   STA   (DPTR),Y
   LDA   __local_test_copy_to_member_pointer__small_4
   INY
   STA   (DPTR),Y
   LDA   __local_test_copy_to_member_pointer__small_5
   INY
   STA   (DPTR),Y
   LDA   __local_test_copy_to_member_pointer__small_6
   INY
   STA   (DPTR),Y
   LDA   __local_test_copy_to_member_pointer__small_7
   INY
   STA   (DPTR),Y
   LDA   __local_test_copy_to_member_pointer__small_8
   INY
   STA   (DPTR),Y
   LDA   __local_test_copy_to_member_pointer__small_9
   INY
   STA   (DPTR),Y
   LDY   #$05
   LDA   (__local_test_copy_to_member_pointer__0),Y
   STA   __local_test_copy_to_member_pointer__small_7
   INY
   LDA   (__local_test_copy_to_member_pointer__0),Y
   STA   __local_test_copy_to_member_pointer__small_8
   INY
   LDA   (__local_test_copy_to_member_pointer__0),Y
   CMP   #$40
   BNE   .test_copy_to_member_pointer@jthread@1
   LDA   __local_test_copy_to_member_pointer__small_8
   CMP   #$58
   BNE   .test_copy_to_member_pointer@jthread@1
   LDA   __local_test_copy_to_member_pointer__small_7
   CMP   #$D0
.test_copy_to_member_pointer@jthread@1:
   JMP   test_copy_from_member_pointer@tail0

test_copy_to_member_pointer@run4:
   SUBROUTINE

   JSR   test_copy_to_member_pointer@run2
   TYA
   JMP   test_copy_to_member_pointer@run0

test_copy_to_member_pointer@run2:
   SUBROUTINE

   LDA   #$01
   STA   __zpabi_calloc__nmemb_0
   LDA   #$0C
   STA   __zpabi_calloc__size_0
   RTS

test_copy_to_member_pointer@run0:
   SUBROUTINE

   STA   __zpabi_calloc__nmemb_1
   STA   __zpabi_calloc__nmemb_2
   STA   __zpabi_calloc__nmemb_3
   STA   __zpabi_calloc__size_1
   STA   __zpabi_calloc__size_2
   STA   __zpabi_calloc__size_3
   JMP   calloc

test_copy_from_nested_member_pointer:
   SUBROUTINE

   JSR   test_copy_from_nested_member_pointer@run1
   STA   __local_test_copy_from_nested_member_pointer__2
   STX   __local_test_copy_from_nested_member_pointer__3
   JSR   test_copy_from_nested_member_pointer@run3
   STA   __local_test_copy_from_nested_member_pointer__small_8
   CLC
   LDA   __local_test_copy_from_nested_member_pointer__2
   STA   DPTR
   LDA   __local_test_copy_from_nested_member_pointer__3
   STA   DPTR+1
   LDA   __local_test_copy_from_nested_member_pointer__small_8
   LDY   #$02
   STA   (__local_test_copy_from_nested_member_pointer__2),Y
   TXA
   INY
   STA   (DPTR),Y
   LDA   __local_test_copy_from_nested_member_pointer__2
   STA   DPTR
   LDA   __local_test_copy_from_nested_member_pointer__3
   STA   DPTR+1
   LDA   #$FB
   LDY   #$00
   STA   (__local_test_copy_from_nested_member_pointer__2),Y
   LDA   #$FF
   INY
   STA   (DPTR),Y
   INY
   LDA   (__local_test_copy_from_nested_member_pointer__2),Y
   STA   __local_test_copy_from_nested_member_pointer__small_9
   INY
   LDA   (__local_test_copy_from_nested_member_pointer__2),Y
   STA   __local_test_copy_from_nested_member_pointer__small_8
   LDA   #$65
   LDY   #$00
   STA   (__local_test_copy_from_nested_member_pointer__small_9),Y
   LDY   #$02
   LDA   (__local_test_copy_from_nested_member_pointer__2),Y
   STA   __local_test_copy_from_nested_member_pointer__small_9
   INY
   LDA   (__local_test_copy_from_nested_member_pointer__2),Y
   STA   __local_test_copy_from_nested_member_pointer__small_8
   LDA   #$66
   LDY   #$01
   STA   (__local_test_copy_from_nested_member_pointer__small_9),Y
   INY
   LDA   (__local_test_copy_from_nested_member_pointer__2),Y
   STA   __local_test_copy_from_nested_member_pointer__small_9
   INY
   LDA   (__local_test_copy_from_nested_member_pointer__2),Y
   STA   __local_test_copy_from_nested_member_pointer__small_8
   LDA   #$00
   DEY
   STA   (__local_test_copy_from_nested_member_pointer__small_9),Y
   INY
   STA   (__local_test_copy_from_nested_member_pointer__small_9),Y
   INY
   STA   (__local_test_copy_from_nested_member_pointer__small_9),Y
   INY
   STA   (__local_test_copy_from_nested_member_pointer__small_9),Y
   INY
   STA   (__local_test_copy_from_nested_member_pointer__small_9),Y
   LDA   #$60
   INY
   STA   (__local_test_copy_from_nested_member_pointer__small_9),Y
   LDA   #$53
   INY
   STA   (__local_test_copy_from_nested_member_pointer__small_9),Y
   LDA   #$40
   INY
   STA   (__local_test_copy_from_nested_member_pointer__small_9),Y
   LDY   #$02
   LDA   (__local_test_copy_from_nested_member_pointer__2),Y
   STA   __local_test_copy_from_nested_member_pointer__small_9
   INY
   LDA   (__local_test_copy_from_nested_member_pointer__2),Y
   STA   __local_test_copy_from_nested_member_pointer__small_8
   LDA   #$58
   LDY   #$0A
   STA   (__local_test_copy_from_nested_member_pointer__small_9),Y
   LDA   #$00
   INY
   STA   (__local_test_copy_from_nested_member_pointer__small_9),Y
   LDY   #$03
   LDA   (__local_test_copy_from_nested_member_pointer__2),Y
   STA   __local_test_copy_from_nested_member_pointer__1
   DEY
   LDA   (__local_test_copy_from_nested_member_pointer__2),Y
   ADC   #$02
   STA   __local_test_copy_from_nested_member_pointer__0
   BCC   .carry_inc@1
   INC   __local_test_copy_from_nested_member_pointer__1
.carry_inc@1:
   LDY   #$00
   LDA   (__local_test_copy_from_nested_member_pointer__0),Y
   STA   __local_test_copy_from_nested_member_pointer__small_0
   INY
   LDA   (__local_test_copy_from_nested_member_pointer__0),Y
   STA   __local_test_copy_from_nested_member_pointer__small_1
   INY
   LDA   (__local_test_copy_from_nested_member_pointer__0),Y
   STA   __local_test_copy_from_nested_member_pointer__small_2
   INY
   LDA   (__local_test_copy_from_nested_member_pointer__0),Y
   STA   __local_test_copy_from_nested_member_pointer__small_3
   INY
   LDA   (__local_test_copy_from_nested_member_pointer__0),Y
   STA   __local_test_copy_from_nested_member_pointer__small_4
   INY
   LDA   (__local_test_copy_from_nested_member_pointer__0),Y
   STA   __local_test_copy_from_nested_member_pointer__small_5
   INY
   LDA   (__local_test_copy_from_nested_member_pointer__0),Y
   STA   __local_test_copy_from_nested_member_pointer__small_6
   LDY   #$08
   LDA   (__local_test_copy_from_nested_member_pointer__0),Y
   STA   __local_test_copy_from_nested_member_pointer__small_8
   INY
   LDA   (__local_test_copy_from_nested_member_pointer__0),Y
   STA   __local_test_copy_from_nested_member_pointer__small_9
   LDY   #$07
   LDA   (__local_test_copy_from_nested_member_pointer__0),Y
   CMP   #$40
   BNE   .test_copy_from_nested_member_pointer@jthread@1
   LDA   __local_test_copy_from_nested_member_pointer__small_6
   CMP   #$53
   BNE   .test_copy_from_nested_member_pointer@jthread@1
   LDA   __local_test_copy_from_nested_member_pointer__small_5
   CMP   #$60
   BNE   .test_copy_from_nested_member_pointer@jthread@1
   LDA   __local_test_copy_from_nested_member_pointer__small_4
   BNE   .test_copy_from_nested_member_pointer@jthread@1
   LDA   __local_test_copy_from_nested_member_pointer__small_3
   BNE   .test_copy_from_nested_member_pointer@jthread@1
   LDA   __local_test_copy_from_nested_member_pointer__small_2
   BNE   .test_copy_from_nested_member_pointer@jthread@1
   LDA   __local_test_copy_from_nested_member_pointer__small_1
   BNE   .test_copy_from_nested_member_pointer@jthread@1
   LDA   __local_test_copy_from_nested_member_pointer__small_0
   BNE   .test_copy_from_nested_member_pointer@jthread@1
   LDA   __local_test_copy_from_nested_member_pointer__small_9
   BNE   .test_copy_from_nested_member_pointer@jthread@1
   LDA   __local_test_copy_from_nested_member_pointer__small_8
   CMP   #$58
   BNE   .test_copy_from_nested_member_pointer@jthread@1
   LDY   #$00
   LDA   (__local_test_copy_from_nested_member_pointer__2),Y
   STA   __local_test_copy_from_nested_member_pointer__small_8
   INY
   LDA   (__local_test_copy_from_nested_member_pointer__2),Y
   CMP   #$FF
   BNE   .test_copy_from_nested_member_pointer@jthread@1
   LDA   __local_test_copy_from_nested_member_pointer__small_8
   CMP   #$FB
   BNE   .test_copy_from_nested_member_pointer@jthread@1
   LDY   #$04
   LDA   (__local_test_copy_from_nested_member_pointer__2),Y
   BEQ   .if_end@15
.test_copy_from_nested_member_pointer@jthread@1:
   JMP   test_copy_from_member_pointer@tail0
.if_end@15:
   JMP   test_copy_from_nested_member_pointer@tail1

test_copy_from_nested_member_pointer@run3:
   SUBROUTINE

   JSR   test_copy_to_member_pointer@run2
   LDA   #$00
   JMP   test_copy_to_member_pointer@run0

test_copy_from_nested_member_pointer@run1:
   SUBROUTINE

   JSR   test_copy_from_nested_member_pointer@run1@run5
   LDA   #$00
   JMP   test_copy_to_member_pointer@run0

test_copy_from_nested_member_pointer@run1@run5:
   SUBROUTINE

   LDA   #$01
   STA   __zpabi_calloc__nmemb_0
   LDA   #$10
   STA   __zpabi_calloc__size_0
   RTS

test_copy_from_nested_member_pointer@tail1:
   SUBROUTINE

   LDX   #$00
   LDA   #$01
   RTS

test_copy_to_nested_member_pointer:
   SUBROUTINE

   LDA   #$00
   STA   __local_test_copy_to_nested_member_pointer__small_0
   LDY   #$00
   STY   __local_test_copy_to_nested_member_pointer__small_1
   STY   __local_test_copy_to_nested_member_pointer__small_2
   STY   __local_test_copy_to_nested_member_pointer__small_3
   STA   __local_test_copy_to_nested_member_pointer__small_4
   LDA   #$D0
   STA   __local_test_copy_to_nested_member_pointer__small_5
   LDA   #$58
   STA   __local_test_copy_to_nested_member_pointer__small_6
   LDA   #$40
   STA   __local_test_copy_to_nested_member_pointer__small_7
   LDA   #$06
   STA   __local_test_copy_to_nested_member_pointer__small_8
   LDA   #$12
   STA   __local_test_copy_to_nested_member_pointer__small_9
   JSR   test_copy_from_nested_member_pointer@run1@run5
   TYA
   JSR   test_copy_to_member_pointer@run0
   STA   __local_test_copy_to_nested_member_pointer__3
   STX   __local_test_copy_to_nested_member_pointer__4
   JSR   test_copy_from_nested_member_pointer@run3
   STA   __local_test_copy_to_nested_member_pointer__1
   CLC
   LDA   __local_test_copy_to_nested_member_pointer__3
   STA   DPTR
   LDA   __local_test_copy_to_nested_member_pointer__4
   STA   DPTR+1
   LDA   __local_test_copy_to_nested_member_pointer__1
   LDY   #$02
   STA   (__local_test_copy_to_nested_member_pointer__3),Y
   TXA
   INY
   STA   (DPTR),Y
   LDA   (__local_test_copy_to_nested_member_pointer__3),Y
   STA   __local_test_copy_to_nested_member_pointer__1
   DEY
   LDA   (__local_test_copy_to_nested_member_pointer__3),Y
   ADC   #$02
   STA   __local_test_copy_to_nested_member_pointer__0
   BCC   .carry_inc@2
   INC   __local_test_copy_to_nested_member_pointer__1
.carry_inc@2:
   STA   DPTR
   LDA   __local_test_copy_to_nested_member_pointer__1
   STA   DPTR+1
   LDA   __local_test_copy_to_nested_member_pointer__small_0
   LDY   #$00
   STA   (__local_test_copy_to_nested_member_pointer__0),Y
   LDA   __local_test_copy_to_nested_member_pointer__small_1
   INY
   STA   (DPTR),Y
   LDA   __local_test_copy_to_nested_member_pointer__small_2
   INY
   STA   (DPTR),Y
   LDA   __local_test_copy_to_nested_member_pointer__small_3
   INY
   STA   (DPTR),Y
   LDA   __local_test_copy_to_nested_member_pointer__small_4
   INY
   STA   (DPTR),Y
   LDA   __local_test_copy_to_nested_member_pointer__small_5
   INY
   STA   (DPTR),Y
   LDA   __local_test_copy_to_nested_member_pointer__small_6
   INY
   STA   (DPTR),Y
   LDA   __local_test_copy_to_nested_member_pointer__small_7
   INY
   STA   (DPTR),Y
   LDA   __local_test_copy_to_nested_member_pointer__small_8
   INY
   STA   (DPTR),Y
   LDA   __local_test_copy_to_nested_member_pointer__small_9
   INY
   STA   (DPTR),Y
   LDY   #$02
   LDA   (__local_test_copy_to_nested_member_pointer__3),Y
   STA   __local_test_copy_to_nested_member_pointer__small_6
   INY
   LDA   (__local_test_copy_to_nested_member_pointer__3),Y
   STA   __local_test_copy_to_nested_member_pointer__small_5
   LDY   #$07
   LDA   (__local_test_copy_to_nested_member_pointer__small_6),Y
   STA   __local_test_copy_to_nested_member_pointer__2
   INY
   LDA   (__local_test_copy_to_nested_member_pointer__small_6),Y
   STA   __local_test_copy_to_nested_member_pointer__1
   INY
   LDA   (__local_test_copy_to_nested_member_pointer__small_6),Y
   CMP   #$40
   BNE   .test_copy_to_nested_member_pointer@jthread@1
   LDA   __local_test_copy_to_nested_member_pointer__1
   CMP   #$58
   BNE   .test_copy_to_nested_member_pointer@jthread@1
   LDA   __local_test_copy_to_nested_member_pointer__2
   CMP   #$D0
.test_copy_to_nested_member_pointer@jthread@1:
   JMP   test_copy_from_member_pointer@tail0

test_mixed_nested_access:
   SUBROUTINE

   LDA   #$64
   STA   __local_test_mixed_nested_access__s1
   LDA   #$00
   STA   __local_test_mixed_nested_access__s1+1
   LDA   #$0E
   STA   __local_test_mixed_nested_access__4
.clear@22:
   DEC   __local_test_mixed_nested_access__4
   LDA   __local_test_mixed_nested_access__4
   CLC
   ADC   #<__local_test_mixed_nested_access__s1
   STA   __local_test_mixed_nested_access__0
   LDA   #$00
   ADC   #>__local_test_mixed_nested_access__s1
   STA   __local_test_mixed_nested_access__1
   LDA   #$00
   LDY   #$02
   STA   (__local_test_mixed_nested_access__0),Y
   LDA   __local_test_mixed_nested_access__4
   BNE   .clear@22
   JSR   test_copy_from_nested_member_pointer@run1
   STA   __local_test_mixed_nested_access__2
   STX   __local_test_mixed_nested_access__3
   LDA   #$FF
   STA   __local_test_mixed_nested_access__s1
   STA   __local_test_mixed_nested_access__s1+1
   JSR   test_copy_from_nested_member_pointer@run1
   STA   __local_test_mixed_nested_access__s1+2
   TXA
   STA   __local_test_mixed_nested_access__s1+3
   STA   __local_test_mixed_nested_access__1
   LDA   __local_test_mixed_nested_access__s1+2
   STA   __local_test_mixed_nested_access__0
   LDA   #$7D
   LDY   #$00
   STA   (__local_test_mixed_nested_access__0),Y
   LDA   __local_test_mixed_nested_access__s1+2
   STA   __local_test_mixed_nested_access__0
   LDA   __local_test_mixed_nested_access__s1+3
   STA   __local_test_mixed_nested_access__1
   LDA   #$7E
   INY
   STA   (__local_test_mixed_nested_access__0),Y
   LDA   __local_test_mixed_nested_access__s1+2
   STA   __local_test_mixed_nested_access__0
   LDA   __local_test_mixed_nested_access__s1+3
   STA   __local_test_mixed_nested_access__1
   LDA   #$00
   INY
   STA   (__local_test_mixed_nested_access__0),Y
   INY
   STA   (__local_test_mixed_nested_access__0),Y
   INY
   STA   (__local_test_mixed_nested_access__0),Y
   INY
   STA   (__local_test_mixed_nested_access__0),Y
   INY
   STA   (__local_test_mixed_nested_access__0),Y
   INY
   STA   (__local_test_mixed_nested_access__0),Y
   LDA   #$49
   INY
   STA   (__local_test_mixed_nested_access__0),Y
   LDA   #$C0
   INY
   STA   (__local_test_mixed_nested_access__0),Y
   LDA   __local_test_mixed_nested_access__s1+2
   STA   __local_test_mixed_nested_access__0
   LDA   __local_test_mixed_nested_access__s1+3
   STA   __local_test_mixed_nested_access__1
   LDA   #$BA
   INY
   STA   (__local_test_mixed_nested_access__0),Y
   LDA   #$FF
   INY
   STA   (__local_test_mixed_nested_access__0),Y
   LDA   #$65
   STA   __local_test_mixed_nested_access__s1+4
   LDA   #$66
   STA   __local_test_mixed_nested_access__s1+5
   LDA   __local_test_mixed_nested_access__2
   STA   DPTR
   LDA   __local_test_mixed_nested_access__3
   STA   DPTR+1
   LDA   #$01
   LDY   #$00
   STA   (__local_test_mixed_nested_access__2),Y
   LDA   #$00
   INY
   STA   (DPTR),Y
   JSR   test_copy_from_nested_member_pointer@run1
   STA   __local_test_mixed_nested_access__1
   CLC
   LDA   __local_test_mixed_nested_access__2
   STA   DPTR
   LDA   __local_test_mixed_nested_access__3
   STA   DPTR+1
   LDA   __local_test_mixed_nested_access__1
   LDY   #$02
   STA   (__local_test_mixed_nested_access__2),Y
   TXA
   INY
   STA   (DPTR),Y
   DEY
   LDA   (__local_test_mixed_nested_access__2),Y
   STA   __local_test_mixed_nested_access__0
   INY
   LDA   (__local_test_mixed_nested_access__2),Y
   STA   __local_test_mixed_nested_access__1
   LDA   #$05
   LDY   #$00
   STA   (__local_test_mixed_nested_access__0),Y
   LDY   #$02
   LDA   (__local_test_mixed_nested_access__2),Y
   STA   __local_test_mixed_nested_access__0
   INY
   LDA   (__local_test_mixed_nested_access__2),Y
   STA   __local_test_mixed_nested_access__1
   LDA   #$06
   LDY   #$01
   STA   (__local_test_mixed_nested_access__0),Y
   LDA   __local_test_mixed_nested_access__2
   ADC   #$04
   STA   __local_test_mixed_nested_access__2
   BCC   .carry_inc@3
   INC   __local_test_mixed_nested_access__3
   CLC
.carry_inc@3:
   ADC   #$02
   STA   __local_test_mixed_nested_access__0
   LDA   __local_test_mixed_nested_access__3
   ADC   #$00
   STA   __local_test_mixed_nested_access__1
   LDA   #$00
   LDY   #$00
   STA   (__local_test_mixed_nested_access__0),Y
   INY
   STA   (__local_test_mixed_nested_access__0),Y
   INY
   STA   (__local_test_mixed_nested_access__0),Y
   INY
   STA   (__local_test_mixed_nested_access__0),Y
   LDA   #$84
   INY
   STA   (__local_test_mixed_nested_access__0),Y
   LDA   #$D7
   INY
   STA   (__local_test_mixed_nested_access__0),Y
   LDA   #$C7
   INY
   STA   (__local_test_mixed_nested_access__0),Y
   LDA   #$41
   INY
   STA   (__local_test_mixed_nested_access__0),Y
   LDA   #$FB
   LDY   #$0A
   STA   (__local_test_mixed_nested_access__2),Y
   LDA   #$FF
   INY
   STA   (__local_test_mixed_nested_access__2),Y
   LDY   #$00
   LDA   (__local_test_mixed_nested_access__0),Y
   STA   __local_test_mixed_nested_access__12
   INY
   LDA   (__local_test_mixed_nested_access__0),Y
   STA   __local_test_mixed_nested_access__11
   INY
   LDA   (__local_test_mixed_nested_access__0),Y
   STA   __local_test_mixed_nested_access__10
   INY
   LDA   (__local_test_mixed_nested_access__0),Y
   STA   __local_test_mixed_nested_access__9
   INY
   LDA   (__local_test_mixed_nested_access__0),Y
   STA   __local_test_mixed_nested_access__8
   INY
   LDA   (__local_test_mixed_nested_access__0),Y
   STA   __local_test_mixed_nested_access__7
   INY
   LDA   (__local_test_mixed_nested_access__0),Y
   STA   __local_test_mixed_nested_access__6
   INY
   LDA   (__local_test_mixed_nested_access__0),Y
   STA   __local_test_mixed_nested_access__5
   INY
   LDA   (__local_test_mixed_nested_access__0),Y
   STA   __local_test_mixed_nested_access__4
   INY
   LDA   (__local_test_mixed_nested_access__0),Y
   STA   __local_test_mixed_nested_access__3
   LDA   __local_test_mixed_nested_access__s1+2
   LDX   __local_test_mixed_nested_access__s1+3
   STX   __local_test_mixed_nested_access__1
   CLC
   ADC   #$02
   STA   __local_test_mixed_nested_access__0
   BCC   .carry_inc@4
   INC   __local_test_mixed_nested_access__1
.carry_inc@4:
   STA   DPTR
   LDA   __local_test_mixed_nested_access__1
   STA   DPTR+1
   LDA   __local_test_mixed_nested_access__12
   LDY   #$00
   STA   (__local_test_mixed_nested_access__0),Y
   LDA   __local_test_mixed_nested_access__11
   INY
   STA   (DPTR),Y
   LDA   __local_test_mixed_nested_access__10
   INY
   STA   (DPTR),Y
   LDA   __local_test_mixed_nested_access__9
   INY
   STA   (DPTR),Y
   LDA   __local_test_mixed_nested_access__8
   INY
   STA   (DPTR),Y
   LDA   __local_test_mixed_nested_access__7
   INY
   STA   (DPTR),Y
   LDA   __local_test_mixed_nested_access__6
   INY
   STA   (DPTR),Y
   LDA   __local_test_mixed_nested_access__5
   INY
   STA   (DPTR),Y
   LDA   __local_test_mixed_nested_access__4
   INY
   STA   (DPTR),Y
   LDA   __local_test_mixed_nested_access__3
   INY
   STA   (DPTR),Y
   LDA   __local_test_mixed_nested_access__s1+2
   STA   __local_test_mixed_nested_access__8
   LDA   __local_test_mixed_nested_access__s1+3
   STA   __local_test_mixed_nested_access__9
   LDY   #$02
   LDA   (__local_test_mixed_nested_access__8),Y
   STA   __local_test_mixed_nested_access__7
   INY
   LDA   (__local_test_mixed_nested_access__8),Y
   STA   __local_test_mixed_nested_access__6
   INY
   LDA   (__local_test_mixed_nested_access__8),Y
   STA   __local_test_mixed_nested_access__5
   INY
   LDA   (__local_test_mixed_nested_access__8),Y
   STA   __local_test_mixed_nested_access__4
   INY
   LDA   (__local_test_mixed_nested_access__8),Y
   STA   __local_test_mixed_nested_access__3
   INY
   LDA   (__local_test_mixed_nested_access__8),Y
   STA   __local_test_mixed_nested_access__2
   INY
   LDA   (__local_test_mixed_nested_access__8),Y
   STA   __local_test_mixed_nested_access__1
   INY
   LDA   (__local_test_mixed_nested_access__8),Y
   CMP   #$41
   BNE   .test_mixed_nested_access@jthread@1
   LDA   __local_test_mixed_nested_access__1
   CMP   #$C7
   BNE   .test_mixed_nested_access@jthread@1
   LDA   __local_test_mixed_nested_access__2
   CMP   #$D7
   BNE   .test_mixed_nested_access@jthread@1
   LDA   __local_test_mixed_nested_access__3
   CMP   #$84
   BNE   .test_mixed_nested_access@jthread@1
   LDA   __local_test_mixed_nested_access__4
   BNE   .test_mixed_nested_access@jthread@1
   LDA   __local_test_mixed_nested_access__5
   BNE   .test_mixed_nested_access@jthread@1
   LDA   __local_test_mixed_nested_access__6
   BNE   .test_mixed_nested_access@jthread@1
   LDA   __local_test_mixed_nested_access__7
   BNE   .test_mixed_nested_access@jthread@1
   INY
   LDA   (__local_test_mixed_nested_access__8),Y
   STA   __local_test_mixed_nested_access__1
   INY
   LDA   (__local_test_mixed_nested_access__8),Y
   CMP   #$FF
   BNE   .test_mixed_nested_access@jthread@1
   LDA   __local_test_mixed_nested_access__1
   CMP   #$FB
   BNE   .test_mixed_nested_access@jthread@1
   LDY   #$00
   LDA   (__local_test_mixed_nested_access__8),Y
   CMP   #$7D
   BNE   .test_mixed_nested_access@jthread@1
   INY
   LDA   (__local_test_mixed_nested_access__8),Y
   CMP   #$7E
   BEQ   .if_end@28
.test_mixed_nested_access@jthread@1:
   JMP   test_copy_from_member_pointer@tail0
.if_end@28:
   LDX   #$00
   TYA
   RTS

test_member_from_cast:
   SUBROUTINE

   LDA   #$00
   STA   __local_test_member_from_cast__small_0
   LDY   #$00
   STY   __local_test_member_from_cast__small_1
   STY   __local_test_member_from_cast__small_2
   STY   __local_test_member_from_cast__small_3
   STY   __local_test_member_from_cast__small_4
   STA   __local_test_member_from_cast__small_5
   LDA   #$34
   STA   __local_test_member_from_cast__small_6
   LDA   #$40
   STA   __local_test_member_from_cast__small_7
   LDA   #$0A
   STA   __local_test_member_from_cast__small_8
   STY   __local_test_member_from_cast__small_9
   JSR   test_copy_to_member_pointer@run4
   STX   __local_test_member_from_cast__2
   CLC
   ADC   #$02
   STA   __local_test_member_from_cast__1
   BCC   .carry_inc@5
   INC   __local_test_member_from_cast__2
.carry_inc@5:
   STA   DPTR
   LDA   __local_test_member_from_cast__2
   STA   DPTR+1
   LDA   __local_test_member_from_cast__small_0
   LDY   #$00
   STA   (__local_test_member_from_cast__1),Y
   LDA   __local_test_member_from_cast__small_1
   INY
   STA   (DPTR),Y
   LDA   __local_test_member_from_cast__small_2
   INY
   STA   (DPTR),Y
   LDA   __local_test_member_from_cast__small_3
   INY
   STA   (DPTR),Y
   LDA   __local_test_member_from_cast__small_4
   INY
   STA   (DPTR),Y
   LDA   __local_test_member_from_cast__small_5
   INY
   STA   (DPTR),Y
   LDA   __local_test_member_from_cast__small_6
   INY
   STA   (DPTR),Y
   LDA   __local_test_member_from_cast__small_7
   INY
   STA   (DPTR),Y
   LDA   __local_test_member_from_cast__small_8
   INY
   STA   (DPTR),Y
   LDA   __local_test_member_from_cast__small_9
   INY
   STA   (DPTR),Y
   LDY   #$00
   LDA   (__local_test_member_from_cast__1),Y
   STA   __local_test_member_from_cast__small_3
   INY
   LDA   (__local_test_member_from_cast__1),Y
   STA   __local_test_member_from_cast__small_4
   INY
   LDA   (__local_test_member_from_cast__1),Y
   STA   __local_test_member_from_cast__small_5
   INY
   LDA   (__local_test_member_from_cast__1),Y
   STA   __local_test_member_from_cast__small_6
   INY
   LDA   (__local_test_member_from_cast__1),Y
   STA   __local_test_member_from_cast__small_7
   INY
   LDA   (__local_test_member_from_cast__1),Y
   STA   __local_test_member_from_cast__small_8
   INY
   LDA   (__local_test_member_from_cast__1),Y
   STA   __local_test_member_from_cast__small_9
   INY
   LDA   (__local_test_member_from_cast__1),Y
   CMP   #$40
   BNE   .test_member_from_cast@jthread@0
   LDA   __local_test_member_from_cast__small_9
   CMP   #$34
   BNE   .test_member_from_cast@jthread@0
   LDA   __local_test_member_from_cast__small_8
   BNE   .test_member_from_cast@jthread@0
   LDA   __local_test_member_from_cast__small_7
   BNE   .test_member_from_cast@jthread@0
   LDA   __local_test_member_from_cast__small_6
   BNE   .test_member_from_cast@jthread@0
   LDA   __local_test_member_from_cast__small_5
   BNE   .test_member_from_cast@jthread@0
   LDA   __local_test_member_from_cast__small_4
   BNE   .test_member_from_cast@jthread@0
   LDA   __local_test_member_from_cast__small_3
   BNE   .test_member_from_cast@jthread@0
   INY
   LDA   (__local_test_member_from_cast__1),Y
   STA   __local_test_member_from_cast__small_9
   INY
   LDA   (__local_test_member_from_cast__1),Y
   BNE   .test_member_from_cast@jthread@0
   LDA   __local_test_member_from_cast__small_9
   CMP   #$0A
   BEQ   .if_end@31
.test_member_from_cast@jthread@0:
   JMP   test_copy_from_member_pointer@tail0
.if_end@31:
   JMP   test_copy_from_nested_member_pointer@tail1

main:
   SUBROUTINE

   JSR   test_copy_from_member_pointer
   STX   __local_main__0
   ORA   __local_main__0
   BNE   .if_end@32
   JMP   test_copy_from_nested_member_pointer@tail1
.if_end@32:
   JSR   test_copy_to_member_pointer
   STX   __local_main__0
   ORA   __local_main__0
   BNE   .if_end@33
   LDX   #$00
   LDA   #$02
   RTS
.if_end@33:
   JSR   test_copy_from_nested_member_pointer
   STX   __local_main__0
   ORA   __local_main__0
   BNE   .if_end@34
   LDX   #$00
   LDA   #$03
   RTS
.if_end@34:
   JSR   test_copy_to_nested_member_pointer
   STX   __local_main__0
   ORA   __local_main__0
   BNE   .if_end@35
   LDX   #$00
   LDA   #$04
   RTS
.if_end@35:
   JSR   test_mixed_nested_access
   STX   __local_main__0
   ORA   __local_main__0
   BNE   .if_end@36
   LDX   #$00
   LDA   #$05
   RTS
.if_end@36:
   JSR   test_member_from_cast
   STX   __local_main__0
   ORA   __local_main__0
   BNE   .if_end@37
   LDX   #$00
   LDA   #$06
   RTS
.if_end@37:
   JMP   test_copy_from_member_pointer@tail0
