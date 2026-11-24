SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__zpabi_access_members_through_pointer__expected_a_0	EQU	$80
__zpabi_malloc__size_0	EQU	$80
__zpabi_update_members_through_pointer__a_0	EQU	$80
__zpabi_access_members_through_pointer__expected_a_1	EQU	$81
__zpabi_malloc__size_1	EQU	$81
__zpabi_update_members_through_pointer__a_1	EQU	$81
__zpabi_access_members_through_pointer__expected_b_0	EQU	$82
__zpabi_malloc__size_2	EQU	$82
__zpabi_update_members_through_pointer__b_0	EQU	$82
__zpabi_access_members_through_pointer__expected_b_1	EQU	$83
__zpabi_malloc__size_3	EQU	$83
__zpabi_update_members_through_pointer__b_1	EQU	$83
__zpabi_access_members_through_pointer__expected_d_0	EQU	$84
__zpabi_update_members_through_pointer__inner_ptr_0	EQU	$84
__zpabi_access_members_through_pointer__expected_d_1	EQU	$85
__zpabi_update_members_through_pointer__inner_ptr_1	EQU	$85
__local_update_members_through_pointer__0	EQU	$86
__zpabi_access_members_through_pointer__expected_d_2	EQU	$86
__local_update_members_through_pointer__1	EQU	$87
__zpabi_access_members_through_pointer__expected_d_3	EQU	$87
__local_update_members_through_pointer__2	EQU	$88
__zpabi_access_members_through_pointer__expected_d_4	EQU	$88
__local_update_members_through_pointer__3	EQU	$89
__zpabi_access_members_through_pointer__expected_d_5	EQU	$89
__local_update_members_through_pointer__4	EQU	$8A
__zpabi_access_members_through_pointer__expected_d_6	EQU	$8A
__local_update_members_through_pointer__5	EQU	$8B
__zpabi_access_members_through_pointer__expected_d_7	EQU	$8B
__local_update_members_through_pointer__6	EQU	$8C
__zpabi_access_members_through_pointer__expected_i_0	EQU	$8C
__local_update_members_through_pointer__7	EQU	$8D
__zpabi_access_members_through_pointer__expected_i_1	EQU	$8D
__local_access_members_through_pointer__0	EQU	$8E
__local_update_members_through_pointer__8	EQU	$8E
__local_access_members_through_pointer__1	EQU	$8F
__local_update_members_through_pointer__9	EQU	$8F
__local_access_members_through_pointer__2	EQU	$90
__local_update_members_through_pointer__ptr	EQU	$90
__local_access_members_through_pointer__3	EQU	$91
__local_update_members_through_pointer__10	EQU	$91
__local_access_members_through_pointer__4	EQU	$92
__local_access_members_through_pointer__5	EQU	$93
__local_access_members_through_pointer__6	EQU	$94
__local_access_members_through_pointer__7	EQU	$95
__local_access_members_through_pointer__ptr	EQU	$96
__local_access_members_through_pointer__8	EQU	$97

; @zp-link-meta-begin
; def access_members_through_pointer params=__zpabi_access_members_through_pointer__ptr_0,__zpabi_access_members_through_pointer__ptr_1,__zpabi_access_members_through_pointer__expected_a_0,__zpabi_access_members_through_pointer__expected_a_1,__zpabi_access_members_through_pointer__expected_b_0,__zpabi_access_members_through_pointer__expected_b_1,__zpabi_access_members_through_pointer__expected_d_0,__zpabi_access_members_through_pointer__expected_d_1,__zpabi_access_members_through_pointer__expected_d_2,__zpabi_access_members_through_pointer__expected_d_3,__zpabi_access_members_through_pointer__expected_d_4,__zpabi_access_members_through_pointer__expected_d_5,__zpabi_access_members_through_pointer__expected_d_6,__zpabi_access_members_through_pointer__expected_d_7,__zpabi_access_members_through_pointer__expected_i_0,__zpabi_access_members_through_pointer__expected_i_1 locals=__local_access_members_through_pointer__0,__local_access_members_through_pointer__1,__local_access_members_through_pointer__2,__local_access_members_through_pointer__3,__local_access_members_through_pointer__4,__local_access_members_through_pointer__5,__local_access_members_through_pointer__6,__local_access_members_through_pointer__7,__local_access_members_through_pointer__ptr,__local_access_members_through_pointer__8 indirect=false in_cycle=false param_regs=A,X,,,,,,,,,,,,,,
; def update_members_through_pointer params=__zpabi_update_members_through_pointer__ptr_0,__zpabi_update_members_through_pointer__ptr_1,__zpabi_update_members_through_pointer__a_0,__zpabi_update_members_through_pointer__a_1,__zpabi_update_members_through_pointer__b_0,__zpabi_update_members_through_pointer__b_1,__zpabi_update_members_through_pointer__inner_ptr_0,__zpabi_update_members_through_pointer__inner_ptr_1 locals=__local_update_members_through_pointer__0,__local_update_members_through_pointer__1,__local_update_members_through_pointer__2,__local_update_members_through_pointer__3,__local_update_members_through_pointer__4,__local_update_members_through_pointer__5,__local_update_members_through_pointer__6,__local_update_members_through_pointer__7,__local_update_members_through_pointer__8,__local_update_members_through_pointer__9,__local_update_members_through_pointer__ptr,__local_update_members_through_pointer__10 indirect=false in_cycle=false param_regs=A,X,,,,,,
; ext malloc params=__zpabi_malloc__size_0,__zpabi_malloc__size_1,__zpabi_malloc__size_2,__zpabi_malloc__size_3
; item access_members_through_pointer section=.text local=false zero=false
; item update_members_through_pointer section=.text local=false zero=false
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

access_members_through_pointer:
   SUBROUTINE

   STA   __local_access_members_through_pointer__ptr
   STX   __local_access_members_through_pointer__8
   LDY   #$00
   LDA   (__local_access_members_through_pointer__ptr),Y
   STA   __local_access_members_through_pointer__0
   LDA   #$00
   CMP   __zpabi_access_members_through_pointer__expected_a_1
   BNE   .jcmp_skip@0
   LDA   __local_access_members_through_pointer__0
   CMP   __zpabi_access_members_through_pointer__expected_a_0
   BEQ   .if_end@0
.jcmp_skip@0:
   LDX   #$00
   TXA
   RTS
.if_end@0:
   LDY   #$01
   LDA   (__local_access_members_through_pointer__ptr),Y
   STA   __local_access_members_through_pointer__0
   LDA   #$00
   CMP   __zpabi_access_members_through_pointer__expected_b_1
   BNE   .jcmp_skip@1
   LDA   __local_access_members_through_pointer__0
   CMP   __zpabi_access_members_through_pointer__expected_b_0
   BEQ   .if_end@1
.jcmp_skip@1:
   LDX   #$00
   TXA
   RTS
.if_end@1:
   LDA   __local_access_members_through_pointer__ptr
   CLC
   ADC   #$02
   STA   __local_access_members_through_pointer__ptr
   BCC   .carry_inc@0
   INC   __local_access_members_through_pointer__8
.carry_inc@0:
   LDY   #$00
   LDA   (__local_access_members_through_pointer__ptr),Y
   STA   __local_access_members_through_pointer__7
   INY
   LDA   (__local_access_members_through_pointer__ptr),Y
   STA   __local_access_members_through_pointer__6
   INY
   LDA   (__local_access_members_through_pointer__ptr),Y
   STA   __local_access_members_through_pointer__5
   INY
   LDA   (__local_access_members_through_pointer__ptr),Y
   STA   __local_access_members_through_pointer__4
   INY
   LDA   (__local_access_members_through_pointer__ptr),Y
   STA   __local_access_members_through_pointer__3
   INY
   LDA   (__local_access_members_through_pointer__ptr),Y
   STA   __local_access_members_through_pointer__2
   INY
   LDA   (__local_access_members_through_pointer__ptr),Y
   STA   __local_access_members_through_pointer__1
   INY
   LDA   (__local_access_members_through_pointer__ptr),Y
   CMP   __zpabi_access_members_through_pointer__expected_d_7
   BNE   .jcmp_skip@3
   LDA   __local_access_members_through_pointer__1
   CMP   __zpabi_access_members_through_pointer__expected_d_6
   BNE   .jcmp_skip@3
   LDA   __local_access_members_through_pointer__2
   CMP   __zpabi_access_members_through_pointer__expected_d_5
   BNE   .jcmp_skip@3
   LDA   __local_access_members_through_pointer__3
   CMP   __zpabi_access_members_through_pointer__expected_d_4
   BNE   .jcmp_skip@3
   LDA   __local_access_members_through_pointer__4
   CMP   __zpabi_access_members_through_pointer__expected_d_3
   BNE   .jcmp_skip@3
   LDA   __local_access_members_through_pointer__5
   CMP   __zpabi_access_members_through_pointer__expected_d_2
   BNE   .jcmp_skip@3
   LDA   __local_access_members_through_pointer__6
   CMP   __zpabi_access_members_through_pointer__expected_d_1
   BNE   .jcmp_skip@3
   LDA   __local_access_members_through_pointer__7
   CMP   __zpabi_access_members_through_pointer__expected_d_0
   BNE   .jcmp_skip@3
   INY
   LDA   (__local_access_members_through_pointer__ptr),Y
   STA   __local_access_members_through_pointer__1
   INY
   LDA   (__local_access_members_through_pointer__ptr),Y
   CMP   __zpabi_access_members_through_pointer__expected_i_1
   BNE   .jcmp_skip@3
   LDA   __local_access_members_through_pointer__1
   CMP   __zpabi_access_members_through_pointer__expected_i_0
   BEQ   .if_end@3
.jcmp_skip@3:
   LDX   #$00
   TXA
   RTS
.if_end@3:
   LDX   #$00
   LDA   #$01
   RTS

update_members_through_pointer:
   SUBROUTINE

   STA   __local_update_members_through_pointer__ptr
   STX   __local_update_members_through_pointer__10
   LDA   __zpabi_update_members_through_pointer__a_0
   LDY   #$00
   STA   (__local_update_members_through_pointer__ptr),Y
   LDA   __zpabi_update_members_through_pointer__b_0
   INY
   STA   (__local_update_members_through_pointer__ptr),Y
   DEY
   LDA   (__zpabi_update_members_through_pointer__inner_ptr_0),Y
   STA   __local_update_members_through_pointer__9
   INY
   LDA   (__zpabi_update_members_through_pointer__inner_ptr_0),Y
   STA   __local_update_members_through_pointer__8
   INY
   LDA   (__zpabi_update_members_through_pointer__inner_ptr_0),Y
   STA   __local_update_members_through_pointer__7
   INY
   LDA   (__zpabi_update_members_through_pointer__inner_ptr_0),Y
   STA   __local_update_members_through_pointer__6
   INY
   LDA   (__zpabi_update_members_through_pointer__inner_ptr_0),Y
   STA   __local_update_members_through_pointer__5
   INY
   LDA   (__zpabi_update_members_through_pointer__inner_ptr_0),Y
   STA   __local_update_members_through_pointer__4
   INY
   LDA   (__zpabi_update_members_through_pointer__inner_ptr_0),Y
   STA   __local_update_members_through_pointer__3
   INY
   LDA   (__zpabi_update_members_through_pointer__inner_ptr_0),Y
   STA   __local_update_members_through_pointer__2
   INY
   LDA   (__zpabi_update_members_through_pointer__inner_ptr_0),Y
   STA   __local_update_members_through_pointer__1
   INY
   LDA   (__zpabi_update_members_through_pointer__inner_ptr_0),Y
   STA   __local_update_members_through_pointer__0
   LDA   __local_update_members_through_pointer__ptr
   CLC
   ADC   #$02
   STA   __local_update_members_through_pointer__ptr
   BCC   .carry_inc@1
   INC   __local_update_members_through_pointer__10
.carry_inc@1:
   STA   DPTR
   LDA   __local_update_members_through_pointer__10
   STA   DPTR+1
   LDA   __local_update_members_through_pointer__9
   LDY   #$00
   STA   (__local_update_members_through_pointer__ptr),Y
   LDA   __local_update_members_through_pointer__8
   INY
   STA   (DPTR),Y
   LDA   __local_update_members_through_pointer__7
   INY
   STA   (DPTR),Y
   LDA   __local_update_members_through_pointer__6
   INY
   STA   (DPTR),Y
   LDA   __local_update_members_through_pointer__5
   INY
   STA   (DPTR),Y
   LDA   __local_update_members_through_pointer__4
   INY
   STA   (DPTR),Y
   LDA   __local_update_members_through_pointer__3
   INY
   STA   (DPTR),Y
   LDA   __local_update_members_through_pointer__2
   INY
   STA   (DPTR),Y
   LDA   __local_update_members_through_pointer__1
   INY
   STA   (DPTR),Y
   LDA   __local_update_members_through_pointer__0
   INY
   STA   (DPTR),Y
   RTS
