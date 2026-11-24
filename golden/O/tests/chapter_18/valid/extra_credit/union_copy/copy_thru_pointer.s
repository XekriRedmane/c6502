SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__zpabi_calloc__nmemb_0	EQU	$80
__zpabi_malloc__size_0	EQU	$80
__zpabi_strcmp__s2_0	EQU	$80
__zpabi_calloc__nmemb_1	EQU	$81
__zpabi_malloc__size_1	EQU	$81
__zpabi_strcmp__s2_1	EQU	$81
__zpabi_calloc__nmemb_2	EQU	$82
__zpabi_malloc__size_2	EQU	$82
__zpabi_calloc__nmemb_3	EQU	$83
__zpabi_malloc__size_3	EQU	$83
__local_main__0	EQU	$84
__zpabi_calloc__size_0	EQU	$84
__zpabi_calloc__size_1	EQU	$85
__local_test_copy_array_members__0	EQU	$86
__local_test_copy_to_pointer__y_3	EQU	$86
__zpabi_calloc__size_2	EQU	$86
__local_test_copy_array_members__1	EQU	$87
__local_test_copy_to_pointer__y_2	EQU	$87
__zpabi_calloc__size_3	EQU	$87
__local_test_copy_array_members__2	EQU	$88
__local_test_copy_to_pointer__y_1	EQU	$88
__local_test_copy_array_members__union_array	EQU	$89
__local_test_copy_to_pointer__y_0	EQU	$89
__local_test_copy_to_pointer__0	EQU	$8A
__local_test_copy_from_pointer__another_union_8	EQU	$8B
__local_test_copy_to_pointer__1	EQU	$8B
__local_test_copy_array_members__another_union	EQU	$A7
__local_test_copy_array_members__yet_another_union	EQU	$B1

; @zp-link-meta-begin
; def main params= locals=__local_main__0,__local_main__1 indirect=false in_cycle=false
; def test_copy_array_members params= locals=__local_test_copy_array_members__0,__local_test_copy_array_members__1,__local_test_copy_array_members__2,__local_test_copy_array_members__union_array,__local_test_copy_array_members__4,__local_test_copy_array_members__5,__local_test_copy_array_members__6,__local_test_copy_array_members__7,__local_test_copy_array_members__8,__local_test_copy_array_members__9,__local_test_copy_array_members__10,__local_test_copy_array_members__11,__local_test_copy_array_members__12,__local_test_copy_array_members__13,__local_test_copy_array_members__14,__local_test_copy_array_members__15,__local_test_copy_array_members__16,__local_test_copy_array_members__17,__local_test_copy_array_members__18,__local_test_copy_array_members__19,__local_test_copy_array_members__20,__local_test_copy_array_members__21,__local_test_copy_array_members__22,__local_test_copy_array_members__23,__local_test_copy_array_members__24,__local_test_copy_array_members__25,__local_test_copy_array_members__26,__local_test_copy_array_members__27,__local_test_copy_array_members__28,__local_test_copy_array_members__29,__local_test_copy_array_members__30,__local_test_copy_array_members__31,__local_test_copy_array_members__32,__local_test_copy_array_members__another_union,__local_test_copy_array_members__34,__local_test_copy_array_members__35,__local_test_copy_array_members__36,__local_test_copy_array_members__37,__local_test_copy_array_members__38,__local_test_copy_array_members__39,__local_test_copy_array_members__40,__local_test_copy_array_members__41,__local_test_copy_array_members__42,__local_test_copy_array_members__yet_another_union,__local_test_copy_array_members__44,__local_test_copy_array_members__45,__local_test_copy_array_members__46,__local_test_copy_array_members__47,__local_test_copy_array_members__48,__local_test_copy_array_members__49,__local_test_copy_array_members__50,__local_test_copy_array_members__51,__local_test_copy_array_members__52 indirect=false in_cycle=false
; def test_copy_from_pointer params= locals=__local_test_copy_from_pointer__another_union_13,__local_test_copy_from_pointer__another_union_12,__local_test_copy_from_pointer__another_union_11,__local_test_copy_from_pointer__another_union_10,__local_test_copy_from_pointer__another_union_9,__local_test_copy_from_pointer__another_union_8,__local_test_copy_from_pointer__another_union_7,__local_test_copy_from_pointer__another_union_6,__local_test_copy_from_pointer__another_union_5,__local_test_copy_from_pointer__another_union_4,__local_test_copy_from_pointer__another_union_3,__local_test_copy_from_pointer__another_union_2,__local_test_copy_from_pointer__another_union_1,__local_test_copy_from_pointer__another_union_0,__local_test_copy_from_pointer__my_struct_1,__local_test_copy_from_pointer__my_struct_0 indirect=false in_cycle=false
; def test_copy_to_pointer params= locals=__local_test_copy_to_pointer__y_3,__local_test_copy_to_pointer__y_2,__local_test_copy_to_pointer__y_1,__local_test_copy_to_pointer__y_0,__local_test_copy_to_pointer__0,__local_test_copy_to_pointer__1,__local_test_copy_to_pointer__2,__local_test_copy_to_pointer__3,__local_test_copy_to_pointer__4,__local_test_copy_to_pointer__5 indirect=false in_cycle=false
; ext calloc params=__zpabi_calloc__nmemb_0,__zpabi_calloc__nmemb_1,__zpabi_calloc__nmemb_2,__zpabi_calloc__nmemb_3,__zpabi_calloc__size_0,__zpabi_calloc__size_1,__zpabi_calloc__size_2,__zpabi_calloc__size_3
; ext malloc params=__zpabi_malloc__size_0,__zpabi_malloc__size_1,__zpabi_malloc__size_2,__zpabi_malloc__size_3
; ext strcmp params=__zpabi_strcmp__s1_0,__zpabi_strcmp__s1_1,__zpabi_strcmp__s2_0,__zpabi_strcmp__s2_1 param_regs=A,X,,
; call main -> test_copy_array_members
; call main -> test_copy_from_pointer
; call main -> test_copy_to_pointer
; call test_copy_array_members -> strcmp
; call test_copy_to_pointer -> malloc
; item test_copy_to_pointer section=.text local=false zero=false
; item test_copy_to_pointer@tail0 section=.text local=true zero=false
; item test_copy_from_pointer section=.text local=false zero=false
; item test_copy_array_members section=.text local=false zero=false
; item main section=.text local=false zero=false
; item str@0 section=.rodata local=true zero=false
; item str@1 section=.rodata local=true zero=false
; item str@2 section=.rodata local=true zero=false
; item str@3 section=.rodata local=true zero=false
; item str@4 section=.rodata local=true zero=false
; item @8.my_union section=.bss local=true zero=true
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

test_copy_to_pointer:
   SUBROUTINE

   LDA   #$EC
   STA   __local_test_copy_to_pointer__y_0
   LDA   #$FF
   STA   __local_test_copy_to_pointer__y_1
   LDY   #$FF
   STY   __local_test_copy_to_pointer__y_2
   STA   __local_test_copy_to_pointer__y_3
   LDA   #$04
   STA   __zpabi_malloc__size_0
   LDA   #$00
   STA   __zpabi_malloc__size_1
   STA   __zpabi_malloc__size_2
   STA   __zpabi_malloc__size_3
   JSR   malloc
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
   LDY   #$00
   LDA   (__local_test_copy_to_pointer__0),Y
   STA   __local_test_copy_to_pointer__y_0
   INY
   LDA   (__local_test_copy_to_pointer__0),Y
   STA   __local_test_copy_to_pointer__y_1
   INY
   LDA   (__local_test_copy_to_pointer__0),Y
   STA   __local_test_copy_to_pointer__y_2
   INY
   LDA   (__local_test_copy_to_pointer__0),Y
   CMP   #$FF
   BNE   .test_copy_to_pointer@jthread@3
   LDA   __local_test_copy_to_pointer__y_2
   CMP   #$FF
   BNE   .test_copy_to_pointer@jthread@3
   LDA   __local_test_copy_to_pointer__y_1
   CMP   #$FF
   BNE   .test_copy_to_pointer@jthread@3
   LDA   __local_test_copy_to_pointer__y_0
   CMP   #$EC
   BNE   .test_copy_to_pointer@jthread@3
   LDY   #$00
   LDA   (__local_test_copy_to_pointer__0),Y
   STA   __local_test_copy_to_pointer__y_2
   INY
   LDA   (__local_test_copy_to_pointer__0),Y
   CMP   #$FF
   BNE   .test_copy_to_pointer@jthread@3
   LDA   __local_test_copy_to_pointer__y_2
   CMP   #$EC
   BNE   .test_copy_to_pointer@jthread@3
   DEY
   LDA   (__local_test_copy_to_pointer__0),Y
   CMP   #$EC
   BNE   .test_copy_to_pointer@jthread@3
   INY
   LDA   (__local_test_copy_to_pointer__0),Y
   CMP   #$FF
   BNE   .test_copy_to_pointer@jthread@3
   INY
   LDA   (__local_test_copy_to_pointer__0),Y
   CMP   #$FF
   BEQ   .if_end@8
.test_copy_to_pointer@jthread@3:
   LDX   #$00
   TXA
   RTS
.if_end@8:
   JMP   test_copy_to_pointer@tail0

test_copy_to_pointer@tail0:
   SUBROUTINE

   LDX   #$00
   LDA   #$01
   RTS

test_copy_from_pointer:
   SUBROUTINE

   LDY   #$00
   STY   @8.my_union
   LDA   #$94
   STA   @8.my_union+1
   LDA   #$35
   STA   @8.my_union+2
   LDA   #$77
   STA   @8.my_union+3
   TYA
   STA   @8.my_union+4
   STY   @8.my_union+5
   STY   @8.my_union+6
   STY   @8.my_union+7
   STA   @8.my_union+8
   LDA   #$88
   STA   @8.my_union+9
   LDA   #$D3
   STA   @8.my_union+10
   LDA   #$40
   STA   @8.my_union+11
   LDA   #$02
   STA   @8.my_union+12
   STY   @8.my_union+13
   LDA   @8.my_union+8
   STA   __local_test_copy_from_pointer__another_union_8
   LDA   #$77
   CMP   #$77
   BNE   .test_copy_from_pointer@jthread@1
   LDA   #$35
   CMP   #$35
   BNE   .test_copy_from_pointer@jthread@1
   LDA   #$94
   CMP   #$94
   BNE   .test_copy_from_pointer@jthread@1
   TYA
   BNE   .test_copy_from_pointer@jthread@1
   LDA   #$40
   CMP   #$40
   BNE   .test_copy_from_pointer@jthread@1
   LDA   #$D3
   CMP   #$D3
   BNE   .test_copy_from_pointer@jthread@1
   LDA   #$88
   CMP   #$88
   BNE   .test_copy_from_pointer@jthread@1
   LDA   __local_test_copy_from_pointer__another_union_8
   BNE   .test_copy_from_pointer@jthread@1
   TYA
   BNE   .test_copy_from_pointer@jthread@1
   CMP   #$80
   BNE   .test_copy_from_pointer@jthread@1
   CMP   #$00
   BNE   .test_copy_from_pointer@jthread@1
   ORA   #$00
   BNE   .test_copy_from_pointer@jthread@1
   LDA   #$02
   CMP   #$02
   BEQ   .if_end@13
.test_copy_from_pointer@jthread@1:
   LDX   #$00
   TXA
   RTS
.if_end@13:
   JMP   test_copy_to_pointer@tail0

test_copy_array_members:
   SUBROUTINE

   LDA   #$66
   STA   __local_test_copy_array_members__union_array
   LDA   #$6F
   STA   __local_test_copy_array_members__union_array+1
   STA   __local_test_copy_array_members__union_array+2
   LDA   #$62
   STA   __local_test_copy_array_members__union_array+3
   LDA   #$61
   STA   __local_test_copy_array_members__union_array+4
   LDA   #$72
   STA   __local_test_copy_array_members__union_array+5
   LDA   #$00
   STA   __local_test_copy_array_members__union_array+6
   LDY   #$00
   STY   __local_test_copy_array_members__union_array+7
   STY   __local_test_copy_array_members__union_array+8
   STA   __local_test_copy_array_members__union_array+9
   LDA   #$68
   STA   __local_test_copy_array_members__union_array+10
   LDA   #$65
   STA   __local_test_copy_array_members__union_array+11
   LDA   #$6C
   STA   __local_test_copy_array_members__union_array+12
   STA   __local_test_copy_array_members__union_array+13
   LDA   #$6F
   STA   __local_test_copy_array_members__union_array+14
   TYA
   STA   __local_test_copy_array_members__union_array+15
   STY   __local_test_copy_array_members__union_array+16
   STY   __local_test_copy_array_members__union_array+17
   STY   __local_test_copy_array_members__union_array+18
   STA   __local_test_copy_array_members__union_array+19
   LDA   #$69
   STA   __local_test_copy_array_members__union_array+20
   STA   __local_test_copy_array_members__union_array+26
   LDA   #$74
   STA   __local_test_copy_array_members__union_array+21
   LDA   #$73
   STA   __local_test_copy_array_members__union_array+22
   LDA   #$61
   STA   __local_test_copy_array_members__union_array+23
   LDA   #$75
   STA   __local_test_copy_array_members__union_array+24
   LDA   #$6F
   STA   __local_test_copy_array_members__union_array+27
   LDA   #$6E
   STA   __local_test_copy_array_members__union_array+25
   STA   __local_test_copy_array_members__union_array+28
   STY   __local_test_copy_array_members__union_array+29
   LDA   #>__local_test_copy_array_members__union_array
   STA   __local_test_copy_array_members__1
   LDA   #<__local_test_copy_array_members__union_array
   CLC
   STA   __local_test_copy_array_members__0
   BCC   .carry_inc@0
   INC   __local_test_copy_array_members__1
.carry_inc@0:
   LDY   #$00
   LDA   (__local_test_copy_array_members__0),Y
   STA   __local_test_copy_array_members__another_union
   INY
   LDA   (__local_test_copy_array_members__0),Y
   STA   __local_test_copy_array_members__another_union+1
   INY
   LDA   (__local_test_copy_array_members__0),Y
   STA   __local_test_copy_array_members__another_union+2
   INY
   LDA   (__local_test_copy_array_members__0),Y
   STA   __local_test_copy_array_members__another_union+3
   INY
   LDA   (__local_test_copy_array_members__0),Y
   STA   __local_test_copy_array_members__another_union+4
   INY
   LDA   (__local_test_copy_array_members__0),Y
   STA   __local_test_copy_array_members__another_union+5
   INY
   LDA   (__local_test_copy_array_members__0),Y
   STA   __local_test_copy_array_members__another_union+6
   INY
   LDA   (__local_test_copy_array_members__0),Y
   STA   __local_test_copy_array_members__another_union+7
   INY
   LDA   (__local_test_copy_array_members__0),Y
   STA   __local_test_copy_array_members__another_union+8
   INY
   LDA   (__local_test_copy_array_members__0),Y
   STA   __local_test_copy_array_members__another_union+9
   LDA   #$62
   STA   __local_test_copy_array_members__yet_another_union
   STA   __local_test_copy_array_members__yet_another_union+4
   LDA   #$6C
   STA   __local_test_copy_array_members__yet_another_union+1
   STA   __local_test_copy_array_members__yet_another_union+5
   LDA   #$61
   STA   __local_test_copy_array_members__yet_another_union+2
   STA   __local_test_copy_array_members__yet_another_union+6
   LDA   #$68
   STA   __local_test_copy_array_members__yet_another_union+3
   STA   __local_test_copy_array_members__yet_another_union+7
   LDA   #$00
   STA   __local_test_copy_array_members__yet_another_union+8
   STA   __local_test_copy_array_members__yet_another_union+9
   LDA   #>__local_test_copy_array_members__union_array
   STA   __local_test_copy_array_members__1
   LDA   #<__local_test_copy_array_members__union_array
   CLC
   ADC   #$14
   STA   __local_test_copy_array_members__0
   BCC   .carry_inc@1
   INC   __local_test_copy_array_members__1
.carry_inc@1:
   STA   DPTR
   LDA   __local_test_copy_array_members__1
   STA   DPTR+1
   LDA   #$62
   LDY   #$00
   STA   (__local_test_copy_array_members__0),Y
   LDA   __local_test_copy_array_members__yet_another_union+1
   INY
   STA   (DPTR),Y
   LDA   __local_test_copy_array_members__yet_another_union+2
   INY
   STA   (DPTR),Y
   LDA   __local_test_copy_array_members__yet_another_union+3
   INY
   STA   (DPTR),Y
   LDA   __local_test_copy_array_members__yet_another_union+4
   INY
   STA   (DPTR),Y
   LDA   __local_test_copy_array_members__yet_another_union+5
   INY
   STA   (DPTR),Y
   LDA   __local_test_copy_array_members__yet_another_union+6
   INY
   STA   (DPTR),Y
   LDA   __local_test_copy_array_members__yet_another_union+7
   INY
   STA   (DPTR),Y
   LDA   __local_test_copy_array_members__yet_another_union+8
   INY
   STA   (DPTR),Y
   LDA   __local_test_copy_array_members__yet_another_union+9
   INY
   STA   (DPTR),Y
   LDA   #>__local_test_copy_array_members__union_array
   STA   __local_test_copy_array_members__2
   CLC
   BCC   .carry_inc@2
   INC   __local_test_copy_array_members__2
.carry_inc@2:
   LDA   #<str@0
   STA   __zpabi_strcmp__s2_0
   LDA   #>str@0
   STA   __zpabi_strcmp__s2_1
   LDX   __local_test_copy_array_members__2
   LDA   #<__local_test_copy_array_members__union_array
   JSR   strcmp
   STX   __local_test_copy_array_members__0
   ORA   __local_test_copy_array_members__0
   BNE   .test_copy_array_members@jthread@1
   LDA   #>__local_test_copy_array_members__union_array
   STA   __local_test_copy_array_members__2
   LDA   #<__local_test_copy_array_members__union_array
   CLC
   ADC   #$0A
   STA   __local_test_copy_array_members__0
   BCC   .carry_inc@3
   INC   __local_test_copy_array_members__2
.carry_inc@3:
   LDA   #<str@1
   STA   __zpabi_strcmp__s2_0
   LDA   #>str@1
   STA   __zpabi_strcmp__s2_1
   LDX   __local_test_copy_array_members__2
   LDA   __local_test_copy_array_members__0
   JSR   strcmp
   STX   __local_test_copy_array_members__0
   ORA   __local_test_copy_array_members__0
   BNE   .tail@0
   LDA   #>__local_test_copy_array_members__union_array
   STA   __local_test_copy_array_members__2
   LDA   #<__local_test_copy_array_members__union_array
   CLC
   ADC   #$14
   STA   __local_test_copy_array_members__0
   BCC   .carry_inc@4
   INC   __local_test_copy_array_members__2
.carry_inc@4:
   LDA   #<str@2
   STA   __zpabi_strcmp__s2_0
   LDA   #>str@2
   STA   __zpabi_strcmp__s2_1
   LDX   __local_test_copy_array_members__2
   LDA   __local_test_copy_array_members__0
   JSR   strcmp
   STX   __local_test_copy_array_members__0
   ORA   __local_test_copy_array_members__0
   BEQ   .if_end@18
.test_copy_array_members@jthread@1:
   LDX   #$00
   TXA
   RTS
.if_end@18:
   LDA   #<str@3
   STA   __zpabi_strcmp__s2_0
   LDA   #>str@3
   STA   __zpabi_strcmp__s2_1
   LDX   #>__local_test_copy_array_members__another_union
   LDA   #<__local_test_copy_array_members__another_union
   JSR   strcmp
   STX   __local_test_copy_array_members__0
   ORA   __local_test_copy_array_members__0
   BNE   .tail@0
   LDA   #<str@4
   STA   __zpabi_strcmp__s2_0
   LDA   #>str@4
   STA   __zpabi_strcmp__s2_1
   LDX   #>__local_test_copy_array_members__yet_another_union
   LDA   #<__local_test_copy_array_members__yet_another_union
   JSR   strcmp
   STX   __local_test_copy_array_members__0
   ORA   __local_test_copy_array_members__0
   BEQ   .if_end@20
.tail@0:
   LDX   #$00
   TXA
   RTS
.if_end@20:
   JMP   test_copy_to_pointer@tail0

main:
   SUBROUTINE

   JSR   test_copy_to_pointer
   STX   __local_main__0
   ORA   __local_main__0
   BNE   .if_end@21
   JMP   test_copy_to_pointer@tail0
.if_end@21:
   JSR   test_copy_from_pointer
   STX   __local_main__0
   ORA   __local_main__0
   BNE   .if_end@22
   LDX   #$00
   LDA   #$02
   RTS
.if_end@22:
   JSR   test_copy_array_members
   STX   __local_main__0
   ORA   __local_main__0
   BNE   .if_end@23
   LDX   #$00
   LDA   #$03
   RTS
.if_end@23:
   LDX   #$00
   TXA
   RTS

str@0:
   DC.B  $66, $6F, $6F, $62, $61, $72, $00

str@1:
   DC.B  $68, $65, $6C, $6C, $6F, $00

str@2:
   DC.B  $62, $6C, $61, $68, $62, $6C, $61, $68, $00

str@3:
   DC.B  $66, $6F, $6F, $62, $61, $72, $00

str@4:
   DC.B  $62, $6C, $61, $68, $62, $6C, $61, $68, $00

@8.my_union:
   DS.B  14
