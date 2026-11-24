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
__zpabi_access_members_through_pointer__expected_d_2	EQU	$86
__zpabi_access_members_through_pointer__expected_d_3	EQU	$87
__zpabi_access_members_through_pointer__expected_d_4	EQU	$88
__zpabi_access_members_through_pointer__expected_d_5	EQU	$89
__zpabi_access_members_through_pointer__expected_d_6	EQU	$8A
__zpabi_access_members_through_pointer__expected_d_7	EQU	$8B
__zpabi_access_members_through_pointer__expected_i_0	EQU	$8C
__zpabi_access_members_through_pointer__expected_i_1	EQU	$8D
__local_main__0	EQU	$8E
__local_main__1	EQU	$8F
__local_main__2	EQU	$90
__local_main__3	EQU	$91
__local_main__4	EQU	$92
__local_main__5	EQU	$93
__local_main__6	EQU	$94
__local_main__7	EQU	$95
__local_main__s	EQU	$96
__local_main__inner_struct	EQU	$A2

; @zp-link-meta-begin
; def main params= locals=__local_main__0,__local_main__1,__local_main__2,__local_main__3,__local_main__4,__local_main__5,__local_main__6,__local_main__7,__local_main__s,__local_main__9,__local_main__10,__local_main__11,__local_main__12,__local_main__13,__local_main__14,__local_main__15,__local_main__16,__local_main__17,__local_main__18,__local_main__19,__local_main__inner_struct,__local_main__21,__local_main__22,__local_main__23,__local_main__24,__local_main__25,__local_main__26,__local_main__27,__local_main__28,__local_main__29 indirect=false in_cycle=false
; ext access_members_through_pointer params=__zpabi_access_members_through_pointer__ptr_0,__zpabi_access_members_through_pointer__ptr_1,__zpabi_access_members_through_pointer__expected_a_0,__zpabi_access_members_through_pointer__expected_a_1,__zpabi_access_members_through_pointer__expected_b_0,__zpabi_access_members_through_pointer__expected_b_1,__zpabi_access_members_through_pointer__expected_d_0,__zpabi_access_members_through_pointer__expected_d_1,__zpabi_access_members_through_pointer__expected_d_2,__zpabi_access_members_through_pointer__expected_d_3,__zpabi_access_members_through_pointer__expected_d_4,__zpabi_access_members_through_pointer__expected_d_5,__zpabi_access_members_through_pointer__expected_d_6,__zpabi_access_members_through_pointer__expected_d_7,__zpabi_access_members_through_pointer__expected_i_0,__zpabi_access_members_through_pointer__expected_i_1 param_regs=A,X,,,,,,,,,,,,,,
; ext malloc params=__zpabi_malloc__size_0,__zpabi_malloc__size_1,__zpabi_malloc__size_2,__zpabi_malloc__size_3
; ext update_members_through_pointer params=__zpabi_update_members_through_pointer__ptr_0,__zpabi_update_members_through_pointer__ptr_1,__zpabi_update_members_through_pointer__a_0,__zpabi_update_members_through_pointer__a_1,__zpabi_update_members_through_pointer__b_0,__zpabi_update_members_through_pointer__b_1,__zpabi_update_members_through_pointer__inner_ptr_0,__zpabi_update_members_through_pointer__inner_ptr_1 param_regs=A,X,,,,,,
; call main -> access_members_through_pointer
; call main -> update_members_through_pointer
; item main section=.text local=false zero=false
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

main:
   SUBROUTINE

   LDA   #$01
   STA   __local_main__s
   LDA   #$02
   STA   __local_main__s+1
   LDA   #$00
   STA   __local_main__s+2
   LDY   #$00
   STY   __local_main__s+3
   STY   __local_main__s+4
   STY   __local_main__s+5
   STY   __local_main__s+6
   STA   __local_main__s+7
   LDA   #$08
   STA   __local_main__s+8
   LDA   #$40
   STA   __local_main__s+9
   LDA   #$04
   STA   __local_main__s+10
   STY   __local_main__s+11
   LDA   #$01
   STA   __zpabi_access_members_through_pointer__expected_a_0
   LDA   #$02
   STA   __zpabi_access_members_through_pointer__expected_b_0
   LDA   #$08
   STA   __zpabi_access_members_through_pointer__expected_d_6
   LDA   #$40
   STA   __zpabi_access_members_through_pointer__expected_d_7
   LDA   #$04
   STA   __zpabi_access_members_through_pointer__expected_i_0
   TYA
   STA   __zpabi_access_members_through_pointer__expected_a_1
   STA   __zpabi_access_members_through_pointer__expected_b_1
   STA   __zpabi_access_members_through_pointer__expected_d_0
   STA   __zpabi_access_members_through_pointer__expected_d_1
   STA   __zpabi_access_members_through_pointer__expected_d_2
   STA   __zpabi_access_members_through_pointer__expected_d_3
   STA   __zpabi_access_members_through_pointer__expected_d_4
   STA   __zpabi_access_members_through_pointer__expected_d_5
   STA   __zpabi_access_members_through_pointer__expected_i_1
   LDX   #>__local_main__s
   LDA   #<__local_main__s
   JSR   access_members_through_pointer
   STX   __local_main__0
   ORA   __local_main__0
   BNE   .if_end@0
   LDX   #$00
   LDA   #$01
   RTS
.if_end@0:
   LDA   #$00
   STA   __local_main__inner_struct
   LDY   #$00
   STY   __local_main__inner_struct+1
   STY   __local_main__inner_struct+2
   STY   __local_main__inner_struct+3
   STY   __local_main__inner_struct+4
   STA   __local_main__inner_struct+5
   LDA   #$1C
   STA   __local_main__inner_struct+6
   LDA   #$40
   STA   __local_main__inner_struct+7
   LDA   #$08
   STA   __local_main__inner_struct+8
   STY   __local_main__inner_struct+9
   LDA   #$05
   STA   __zpabi_update_members_through_pointer__a_0
   TYA
   STA   __zpabi_update_members_through_pointer__a_1
   STA   __zpabi_update_members_through_pointer__b_1
   LDA   #$06
   STA   __zpabi_update_members_through_pointer__b_0
   LDA   #<__local_main__inner_struct
   STA   __zpabi_update_members_through_pointer__inner_ptr_0
   LDA   #>__local_main__inner_struct
   STA   __zpabi_update_members_through_pointer__inner_ptr_1
   LDX   #>__local_main__s
   LDA   #<__local_main__s
   JSR   update_members_through_pointer
   LDA   __local_main__s
   CMP   #$05
   BNE   .main@jthread@2
   LDA   __local_main__s+1
   CMP   #$06
   BNE   .main@jthread@2
   LDA   __local_main__s+2
   STA   __local_main__7
   LDA   __local_main__s+3
   STA   __local_main__6
   LDA   __local_main__s+4
   STA   __local_main__5
   LDA   __local_main__s+5
   STA   __local_main__4
   LDA   __local_main__s+6
   STA   __local_main__3
   LDA   __local_main__s+7
   STA   __local_main__2
   LDA   __local_main__s+8
   STA   __local_main__1
   LDA   __local_main__s+9
   CMP   #$40
   BNE   .main@jthread@2
   LDA   __local_main__1
   CMP   #$1C
   BNE   .main@jthread@2
   LDA   __local_main__2
   BNE   .main@jthread@2
   LDA   __local_main__3
   BNE   .main@jthread@2
   LDA   __local_main__4
   BNE   .main@jthread@2
   LDA   __local_main__5
   BNE   .main@jthread@2
   LDA   __local_main__6
   BNE   .main@jthread@2
   LDA   __local_main__7
   BNE   .main@jthread@2
   LDA   __local_main__s+10
   STA   __local_main__1
   LDA   __local_main__s+11
   BNE   .main@jthread@2
   LDA   __local_main__1
   CMP   #$08
   BEQ   .if_end@7
.main@jthread@2:
   LDX   #$00
   LDA   #$02
   RTS
.if_end@7:
   LDX   #$00
   TXA
   RTS
