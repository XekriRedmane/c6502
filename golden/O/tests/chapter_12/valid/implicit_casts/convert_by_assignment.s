SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__zpabi_check_int__converted_0	EQU	$80
__zpabi_check_long__converted_0	EQU	$80
__zpabi_check_ulong__converted_0	EQU	$80
__zpabi_extend_on_assignment__ui_0	EQU	$80
__zpabi_return_extended_uint__u_0	EQU	$80
__zpabi_return_truncated_ulong__ul_0	EQU	$80
__local_return_extended_int__i_0	EQU	$81
__zpabi_check_int__converted_1	EQU	$81
__zpabi_check_long__converted_1	EQU	$81
__zpabi_check_ulong__converted_1	EQU	$81
__zpabi_extend_on_assignment__ui_1	EQU	$81
__zpabi_return_extended_uint__u_1	EQU	$81
__zpabi_return_truncated_ulong__ul_1	EQU	$81
__zpabi_check_int__converted_2	EQU	$82
__zpabi_check_long__converted_2	EQU	$82
__zpabi_check_ulong__converted_2	EQU	$82
__zpabi_extend_on_assignment__ui_2	EQU	$82
__zpabi_return_extended_uint__u_2	EQU	$82
__zpabi_return_truncated_ulong__ul_2	EQU	$82
__zpabi_check_int__converted_3	EQU	$83
__zpabi_check_long__converted_3	EQU	$83
__zpabi_check_ulong__converted_3	EQU	$83
__zpabi_extend_on_assignment__ui_3	EQU	$83
__zpabi_return_extended_uint__u_3	EQU	$83
__zpabi_return_truncated_ulong__ul_3	EQU	$83
__zpabi_check_int__expected_0	EQU	$84
__zpabi_check_long__converted_4	EQU	$84
__zpabi_check_ulong__converted_4	EQU	$84
__zpabi_extend_on_assignment__expected_0	EQU	$84
__zpabi_return_truncated_ulong__ul_4	EQU	$84
__zpabi_check_int__expected_1	EQU	$85
__zpabi_check_long__converted_5	EQU	$85
__zpabi_check_ulong__converted_5	EQU	$85
__zpabi_extend_on_assignment__expected_1	EQU	$85
__zpabi_return_truncated_ulong__ul_5	EQU	$85
__zpabi_check_int__expected_2	EQU	$86
__zpabi_check_long__converted_6	EQU	$86
__zpabi_check_ulong__converted_6	EQU	$86
__zpabi_extend_on_assignment__expected_2	EQU	$86
__zpabi_return_truncated_ulong__ul_6	EQU	$86
__zpabi_check_int__expected_3	EQU	$87
__zpabi_check_long__converted_7	EQU	$87
__zpabi_check_ulong__converted_7	EQU	$87
__zpabi_extend_on_assignment__expected_3	EQU	$87
__zpabi_return_truncated_ulong__ul_7	EQU	$87
__zpabi_check_long__expected_0	EQU	$88
__zpabi_check_ulong__expected_0	EQU	$88
__zpabi_extend_on_assignment__expected_4	EQU	$88
__zpabi_check_long__expected_1	EQU	$89
__zpabi_check_ulong__expected_1	EQU	$89
__zpabi_extend_on_assignment__expected_5	EQU	$89
__zpabi_check_long__expected_2	EQU	$8A
__zpabi_check_ulong__expected_2	EQU	$8A
__zpabi_extend_on_assignment__expected_6	EQU	$8A
__zpabi_check_long__expected_3	EQU	$8B
__zpabi_check_ulong__expected_3	EQU	$8B
__zpabi_extend_on_assignment__expected_7	EQU	$8B
__zpabi_check_long__expected_4	EQU	$8C
__zpabi_check_ulong__expected_4	EQU	$8C
__zpabi_check_long__expected_5	EQU	$8D
__zpabi_check_ulong__expected_5	EQU	$8D
__zpabi_check_long__expected_6	EQU	$8E
__zpabi_check_ulong__expected_6	EQU	$8E
__zpabi_check_long__expected_7	EQU	$8F
__zpabi_check_ulong__expected_7	EQU	$8F
__local_main__0	EQU	$90

; @zp-link-meta-begin
; def check_int params=__zpabi_check_int__converted_0,__zpabi_check_int__converted_1,__zpabi_check_int__converted_2,__zpabi_check_int__converted_3,__zpabi_check_int__expected_0,__zpabi_check_int__expected_1,__zpabi_check_int__expected_2,__zpabi_check_int__expected_3 locals=__local_check_int__0 indirect=false in_cycle=false
; def check_long params=__zpabi_check_long__converted_0,__zpabi_check_long__converted_1,__zpabi_check_long__converted_2,__zpabi_check_long__converted_3,__zpabi_check_long__converted_4,__zpabi_check_long__converted_5,__zpabi_check_long__converted_6,__zpabi_check_long__converted_7,__zpabi_check_long__expected_0,__zpabi_check_long__expected_1,__zpabi_check_long__expected_2,__zpabi_check_long__expected_3,__zpabi_check_long__expected_4,__zpabi_check_long__expected_5,__zpabi_check_long__expected_6,__zpabi_check_long__expected_7 locals=__local_check_long__0 indirect=false in_cycle=false
; def check_ulong params=__zpabi_check_ulong__converted_0,__zpabi_check_ulong__converted_1,__zpabi_check_ulong__converted_2,__zpabi_check_ulong__converted_3,__zpabi_check_ulong__converted_4,__zpabi_check_ulong__converted_5,__zpabi_check_ulong__converted_6,__zpabi_check_ulong__converted_7,__zpabi_check_ulong__expected_0,__zpabi_check_ulong__expected_1,__zpabi_check_ulong__expected_2,__zpabi_check_ulong__expected_3,__zpabi_check_ulong__expected_4,__zpabi_check_ulong__expected_5,__zpabi_check_ulong__expected_6,__zpabi_check_ulong__expected_7 locals=__local_check_ulong__0 indirect=false in_cycle=false
; def extend_on_assignment params=__zpabi_extend_on_assignment__ui_0,__zpabi_extend_on_assignment__ui_1,__zpabi_extend_on_assignment__ui_2,__zpabi_extend_on_assignment__ui_3,__zpabi_extend_on_assignment__expected_0,__zpabi_extend_on_assignment__expected_1,__zpabi_extend_on_assignment__expected_2,__zpabi_extend_on_assignment__expected_3,__zpabi_extend_on_assignment__expected_4,__zpabi_extend_on_assignment__expected_5,__zpabi_extend_on_assignment__expected_6,__zpabi_extend_on_assignment__expected_7 locals=__local_extend_on_assignment__0 indirect=false in_cycle=false
; def main params= locals=__local_main__0,__local_main__1,__local_main__2,__local_main__3,__local_main__4,__local_main__5,__local_main__6,__local_main__7 indirect=false in_cycle=false
; def return_extended_int params=__zpabi_return_extended_int__i_0,__zpabi_return_extended_int__i_1 locals=__local_return_extended_int__i_1,__local_return_extended_int__i_0 indirect=false in_cycle=false param_regs=A,X
; def return_extended_uint params=__zpabi_return_extended_uint__u_0,__zpabi_return_extended_uint__u_1,__zpabi_return_extended_uint__u_2,__zpabi_return_extended_uint__u_3 locals= indirect=false in_cycle=false
; def return_truncated_ulong params=__zpabi_return_truncated_ulong__ul_0,__zpabi_return_truncated_ulong__ul_1,__zpabi_return_truncated_ulong__ul_2,__zpabi_return_truncated_ulong__ul_3,__zpabi_return_truncated_ulong__ul_4,__zpabi_return_truncated_ulong__ul_5,__zpabi_return_truncated_ulong__ul_6,__zpabi_return_truncated_ulong__ul_7 locals= indirect=false in_cycle=false
; call main -> check_int
; call main -> check_long
; call main -> check_ulong
; call main -> extend_on_assignment
; call main -> return_extended_int
; call main -> return_extended_uint
; call main -> return_truncated_ulong
; item check_int section=.text local=false zero=false
; item check_long section=.text local=false zero=false
; item check_ulong section=.text local=false zero=false
; item return_extended_uint section=.text local=false zero=false
; item return_extended_int section=.text local=false zero=false
; item return_truncated_ulong section=.text local=false zero=false
; item extend_on_assignment section=.text local=false zero=false
; item main section=.text local=false zero=false
; runtime hargs_size=32 hargs_need=24
; @zp-link-meta-end

check_int:
   SUBROUTINE

   LDA   __zpabi_check_int__converted_3
   CMP   __zpabi_check_int__expected_3
   BNE   .thread@0
   LDA   __zpabi_check_int__converted_2
   CMP   __zpabi_check_int__expected_2
   BNE   .thread@0
   LDA   __zpabi_check_int__converted_1
   CMP   __zpabi_check_int__expected_1
   BNE   .thread@0
   LDA   __zpabi_check_int__converted_0
   CMP   __zpabi_check_int__expected_0
   BEQ   .cmp_true@0
.thread@0:
   LDA   #$00
   BEQ   .cmp_end@1
.cmp_true@0:
   LDA   #$01
.cmp_end@1:
   LDX   #$00
   RTS

check_long:
   SUBROUTINE

   LDA   __zpabi_check_long__converted_7
   CMP   __zpabi_check_long__expected_7
   BNE   .thread@1
   LDA   __zpabi_check_long__converted_6
   CMP   __zpabi_check_long__expected_6
   BNE   .thread@1
   LDA   __zpabi_check_long__converted_5
   CMP   __zpabi_check_long__expected_5
   BNE   .thread@1
   LDA   __zpabi_check_long__converted_4
   CMP   __zpabi_check_long__expected_4
   BNE   .thread@1
   LDA   __zpabi_check_long__converted_3
   CMP   __zpabi_check_long__expected_3
   BNE   .thread@1
   LDA   __zpabi_check_long__converted_2
   CMP   __zpabi_check_long__expected_2
   BNE   .thread@1
   LDA   __zpabi_check_long__converted_1
   CMP   __zpabi_check_long__expected_1
   BNE   .thread@1
   LDA   __zpabi_check_long__converted_0
   CMP   __zpabi_check_long__expected_0
   BEQ   .cmp_true@3
.thread@1:
   LDA   #$00
   BEQ   .cmp_end@4
.cmp_true@3:
   LDA   #$01
.cmp_end@4:
   LDX   #$00
   RTS

check_ulong:
   SUBROUTINE

   LDA   __zpabi_check_ulong__converted_7
   CMP   __zpabi_check_ulong__expected_7
   BNE   .thread@2
   LDA   __zpabi_check_ulong__converted_6
   CMP   __zpabi_check_ulong__expected_6
   BNE   .thread@2
   LDA   __zpabi_check_ulong__converted_5
   CMP   __zpabi_check_ulong__expected_5
   BNE   .thread@2
   LDA   __zpabi_check_ulong__converted_4
   CMP   __zpabi_check_ulong__expected_4
   BNE   .thread@2
   LDA   __zpabi_check_ulong__converted_3
   CMP   __zpabi_check_ulong__expected_3
   BNE   .thread@2
   LDA   __zpabi_check_ulong__converted_2
   CMP   __zpabi_check_ulong__expected_2
   BNE   .thread@2
   LDA   __zpabi_check_ulong__converted_1
   CMP   __zpabi_check_ulong__expected_1
   BNE   .thread@2
   LDA   __zpabi_check_ulong__converted_0
   CMP   __zpabi_check_ulong__expected_0
   BEQ   .cmp_true@6
.thread@2:
   LDA   #$00
   BEQ   .cmp_end@7
.cmp_true@6:
   LDA   #$01
.cmp_end@7:
   LDX   #$00
   RTS

return_extended_uint:
   SUBROUTINE

   LDA   __zpabi_return_extended_uint__u_0
   STA   HARGS+16
   LDA   __zpabi_return_extended_uint__u_1
   STA   HARGS+17
   LDA   __zpabi_return_extended_uint__u_2
   STA   HARGS+18
   LDA   __zpabi_return_extended_uint__u_3
   STA   HARGS+19
   LDA   #$00
   STA   HARGS+20
   LDY   #$00
   STY   HARGS+21
   STY   HARGS+22
   STA   HARGS+23
   RTS

return_extended_int:
   SUBROUTINE

   STA   __local_return_extended_int__i_0
   TXA
   BMI   .sx_neg@9
   LDA   #$00
   BEQ   .sx_done@10
.sx_neg@9:
   LDA   #$FF
.sx_done@10:
   STA   HARGS+18
   STA   HARGS+19
   STA   HARGS+20
   STA   HARGS+21
   STA   HARGS+22
   STA   HARGS+23
   LDA   __local_return_extended_int__i_0
   STA   HARGS+16
   STX   HARGS+17
   RTS

return_truncated_ulong:
   SUBROUTINE

   LDA   __zpabi_return_truncated_ulong__ul_0
   STA   HARGS+8
   LDA   __zpabi_return_truncated_ulong__ul_1
   STA   HARGS+9
   LDA   __zpabi_return_truncated_ulong__ul_2
   STA   HARGS+10
   LDA   __zpabi_return_truncated_ulong__ul_3
   STA   HARGS+11
   RTS

extend_on_assignment:
   SUBROUTINE

   LDA   #$00
   CMP   __zpabi_extend_on_assignment__expected_7
   BNE   .cmp_end@12
   CMP   __zpabi_extend_on_assignment__expected_6
   BNE   .cmp_end@12
   CMP   __zpabi_extend_on_assignment__expected_5
   BNE   .cmp_end@12
   CMP   __zpabi_extend_on_assignment__expected_4
   BNE   .cmp_end@12
   LDA   __zpabi_extend_on_assignment__ui_3
   CMP   __zpabi_extend_on_assignment__expected_3
   BNE   .thread@3
   LDA   __zpabi_extend_on_assignment__ui_2
   CMP   __zpabi_extend_on_assignment__expected_2
   BNE   .thread@3
   LDA   __zpabi_extend_on_assignment__ui_1
   CMP   __zpabi_extend_on_assignment__expected_1
   BNE   .thread@3
   LDA   __zpabi_extend_on_assignment__ui_0
   CMP   __zpabi_extend_on_assignment__expected_0
   BEQ   .cmp_true@11
.thread@3:
   LDA   #$00
   BEQ   .cmp_end@12
.cmp_true@11:
   LDA   #$01
.cmp_end@12:
   LDX   #$00
   RTS

main:
   SUBROUTINE

   LDA   #$05
   STA   __zpabi_check_int__converted_0
   STA   __zpabi_check_int__expected_0
   LDA   #$80
   STA   __zpabi_check_int__converted_3
   LDA   #$00
   STA   __zpabi_check_int__converted_1
   STA   __zpabi_check_int__converted_2
   STA   __zpabi_check_int__expected_1
   STA   __zpabi_check_int__expected_2
   STA   __zpabi_check_int__expected_3
   JSR   check_int
   STX   __local_main__0
   ORA   __local_main__0
   BNE   .if_end@0
   LDX   #$00
   LDA   #$01
   RTS
.if_end@0:
   LDA   #$0A
   STA   __zpabi_check_long__converted_0
   STA   __zpabi_check_long__expected_0
   LDA   #$80
   STA   __zpabi_check_long__converted_1
   STA   __zpabi_check_long__expected_1
   LDA   #$00
   STA   __zpabi_check_long__converted_2
   STA   __zpabi_check_long__converted_3
   STA   __zpabi_check_long__converted_4
   STA   __zpabi_check_long__converted_5
   STA   __zpabi_check_long__converted_6
   STA   __zpabi_check_long__converted_7
   STA   __zpabi_check_long__expected_2
   STA   __zpabi_check_long__expected_3
   STA   __zpabi_check_long__expected_4
   STA   __zpabi_check_long__expected_5
   STA   __zpabi_check_long__expected_6
   STA   __zpabi_check_long__expected_7
   JSR   check_long
   STX   __local_main__0
   ORA   __local_main__0
   BNE   .if_end@1
   LDX   #$00
   LDA   #$02
   RTS
.if_end@1:
   LDA   #$FF
   STA   __zpabi_check_ulong__converted_0
   STA   __zpabi_check_ulong__converted_1
   STA   __zpabi_check_ulong__converted_2
   STA   __zpabi_check_ulong__converted_3
   STA   __zpabi_check_ulong__converted_4
   STA   __zpabi_check_ulong__converted_5
   STA   __zpabi_check_ulong__converted_6
   STA   __zpabi_check_ulong__converted_7
   STA   __zpabi_check_ulong__expected_0
   STA   __zpabi_check_ulong__expected_1
   STA   __zpabi_check_ulong__expected_2
   STA   __zpabi_check_ulong__expected_3
   LDA   #$00
   STA   __zpabi_check_ulong__expected_4
   STA   __zpabi_check_ulong__expected_5
   STA   __zpabi_check_ulong__expected_6
   STA   __zpabi_check_ulong__expected_7
   JSR   check_ulong
   STX   __local_main__0
   ORA   __local_main__0
   BNE   .if_end@2
   LDX   #$00
   LDA   #$03
   RTS
.if_end@2:
   LDA   #$0A
   STA   __zpabi_return_extended_uint__u_0
   LDA   #$80
   STA   __zpabi_return_extended_uint__u_1
   LDA   #$00
   STA   __zpabi_return_extended_uint__u_2
   STA   __zpabi_return_extended_uint__u_3
   JSR   return_extended_uint
   LDA   HARGS+23
   BNE   .jcmp_skip@14
   LDA   HARGS+22
   BNE   .jcmp_skip@14
   LDA   HARGS+21
   BNE   .jcmp_skip@14
   LDA   HARGS+20
   BNE   .jcmp_skip@14
   LDA   HARGS+19
   BNE   .jcmp_skip@14
   LDA   HARGS+18
   BNE   .jcmp_skip@14
   LDA   HARGS+17
   CMP   #$80
   BNE   .jcmp_skip@14
   LDA   HARGS+16
   CMP   #$0A
   BEQ   .if_end@3
.jcmp_skip@14:
   LDX   #$00
   LDA   #$04
   RTS
.if_end@3:
   LDX   #$FF
   TXA
   JSR   return_extended_int
   LDA   HARGS+23
   BNE   .jcmp_skip@15
   LDA   HARGS+22
   BNE   .jcmp_skip@15
   LDA   HARGS+21
   BNE   .jcmp_skip@15
   LDA   HARGS+20
   BNE   .jcmp_skip@15
   LDA   HARGS+19
   CMP   #$FF
   BNE   .jcmp_skip@15
   LDA   HARGS+18
   CMP   #$FF
   BNE   .jcmp_skip@15
   LDA   HARGS+17
   CMP   #$FF
   BNE   .jcmp_skip@15
   LDA   HARGS+16
   CMP   #$FF
   BEQ   .if_end@4
.jcmp_skip@15:
   LDX   #$00
   LDA   #$05
   RTS
.if_end@4:
   LDA   #$64
   STA   __zpabi_return_truncated_ulong__ul_0
   LDA   #$80
   STA   __zpabi_return_truncated_ulong__ul_1
   LDA   #$02
   STA   __zpabi_return_truncated_ulong__ul_3
   LDA   #$00
   STA   __zpabi_return_truncated_ulong__ul_2
   STA   __zpabi_return_truncated_ulong__ul_4
   STA   __zpabi_return_truncated_ulong__ul_5
   STA   __zpabi_return_truncated_ulong__ul_6
   STA   __zpabi_return_truncated_ulong__ul_7
   JSR   return_truncated_ulong
   LDA   HARGS+11
   BPL   .jcmp_skip@18
   CMP   #$FF
   BNE   .jcmp_skip@18
   LDA   HARGS+10
   CMP   #$FF
   BNE   .jcmp_skip@18
   LDA   HARGS+9
   CMP   #$80
   BNE   .jcmp_skip@18
   LDA   HARGS+8
   CMP   #$64
   BEQ   .if_end@5
.jcmp_skip@18:
   LDX   #$00
   LDA   #$06
   RTS
.if_end@5:
   LDA   #$0A
   STA   __zpabi_extend_on_assignment__ui_0
   STA   __zpabi_extend_on_assignment__expected_0
   LDA   #$80
   STA   __zpabi_extend_on_assignment__ui_1
   STA   __zpabi_extend_on_assignment__expected_1
   LDA   #$00
   STA   __zpabi_extend_on_assignment__ui_2
   STA   __zpabi_extend_on_assignment__ui_3
   STA   __zpabi_extend_on_assignment__expected_2
   STA   __zpabi_extend_on_assignment__expected_3
   STA   __zpabi_extend_on_assignment__expected_4
   STA   __zpabi_extend_on_assignment__expected_5
   STA   __zpabi_extend_on_assignment__expected_6
   STA   __zpabi_extend_on_assignment__expected_7
   JSR   extend_on_assignment
   STX   __local_main__0
   ORA   __local_main__0
   BNE   .if_end@6
   LDX   #$00
   LDA   #$07
   RTS
.if_end@6:
   LDX   #$00
   LDA   #$08
   RTS
