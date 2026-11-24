SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__zpabi_int_to_uint__i_0	EQU	$80
__zpabi_long_to_ulong__l_0	EQU	$80
__zpabi_uint_to_int__ui_0	EQU	$80
__zpabi_ulong_to_long__ul_0	EQU	$80
__zpabi_int_to_uint__i_1	EQU	$81
__zpabi_long_to_ulong__l_1	EQU	$81
__zpabi_uint_to_int__ui_1	EQU	$81
__zpabi_ulong_to_long__ul_1	EQU	$81
__zpabi_int_to_uint__i_2	EQU	$82
__zpabi_long_to_ulong__l_2	EQU	$82
__zpabi_uint_to_int__ui_2	EQU	$82
__zpabi_ulong_to_long__ul_2	EQU	$82
__zpabi_int_to_uint__i_3	EQU	$83
__zpabi_long_to_ulong__l_3	EQU	$83
__zpabi_uint_to_int__ui_3	EQU	$83
__zpabi_ulong_to_long__ul_3	EQU	$83
__zpabi_int_to_uint__expected_0	EQU	$84
__zpabi_long_to_ulong__l_4	EQU	$84
__zpabi_uint_to_int__expected_0	EQU	$84
__zpabi_ulong_to_long__ul_4	EQU	$84
__zpabi_int_to_uint__expected_1	EQU	$85
__zpabi_long_to_ulong__l_5	EQU	$85
__zpabi_uint_to_int__expected_1	EQU	$85
__zpabi_ulong_to_long__ul_5	EQU	$85
__zpabi_int_to_uint__expected_2	EQU	$86
__zpabi_long_to_ulong__l_6	EQU	$86
__zpabi_uint_to_int__expected_2	EQU	$86
__zpabi_ulong_to_long__ul_6	EQU	$86
__zpabi_int_to_uint__expected_3	EQU	$87
__zpabi_long_to_ulong__l_7	EQU	$87
__zpabi_uint_to_int__expected_3	EQU	$87
__zpabi_ulong_to_long__ul_7	EQU	$87
__zpabi_long_to_ulong__expected_0	EQU	$88
__zpabi_ulong_to_long__expected_0	EQU	$88
__zpabi_long_to_ulong__expected_1	EQU	$89
__zpabi_ulong_to_long__expected_1	EQU	$89
__zpabi_long_to_ulong__expected_2	EQU	$8A
__zpabi_ulong_to_long__expected_2	EQU	$8A
__zpabi_long_to_ulong__expected_3	EQU	$8B
__zpabi_ulong_to_long__expected_3	EQU	$8B
__zpabi_long_to_ulong__expected_4	EQU	$8C
__zpabi_ulong_to_long__expected_4	EQU	$8C
__zpabi_long_to_ulong__expected_5	EQU	$8D
__zpabi_ulong_to_long__expected_5	EQU	$8D
__zpabi_long_to_ulong__expected_6	EQU	$8E
__zpabi_ulong_to_long__expected_6	EQU	$8E
__zpabi_long_to_ulong__expected_7	EQU	$8F
__zpabi_ulong_to_long__expected_7	EQU	$8F
__local_main__0	EQU	$90

; @zp-link-meta-begin
; def int_to_uint params=__zpabi_int_to_uint__i_0,__zpabi_int_to_uint__i_1,__zpabi_int_to_uint__i_2,__zpabi_int_to_uint__i_3,__zpabi_int_to_uint__expected_0,__zpabi_int_to_uint__expected_1,__zpabi_int_to_uint__expected_2,__zpabi_int_to_uint__expected_3 locals=__local_int_to_uint__0 indirect=false in_cycle=false
; def long_to_ulong params=__zpabi_long_to_ulong__l_0,__zpabi_long_to_ulong__l_1,__zpabi_long_to_ulong__l_2,__zpabi_long_to_ulong__l_3,__zpabi_long_to_ulong__l_4,__zpabi_long_to_ulong__l_5,__zpabi_long_to_ulong__l_6,__zpabi_long_to_ulong__l_7,__zpabi_long_to_ulong__expected_0,__zpabi_long_to_ulong__expected_1,__zpabi_long_to_ulong__expected_2,__zpabi_long_to_ulong__expected_3,__zpabi_long_to_ulong__expected_4,__zpabi_long_to_ulong__expected_5,__zpabi_long_to_ulong__expected_6,__zpabi_long_to_ulong__expected_7 locals=__local_long_to_ulong__0 indirect=false in_cycle=false
; def main params= locals=__local_main__0,__local_main__1 indirect=false in_cycle=false
; def uint_to_int params=__zpabi_uint_to_int__ui_0,__zpabi_uint_to_int__ui_1,__zpabi_uint_to_int__ui_2,__zpabi_uint_to_int__ui_3,__zpabi_uint_to_int__expected_0,__zpabi_uint_to_int__expected_1,__zpabi_uint_to_int__expected_2,__zpabi_uint_to_int__expected_3 locals=__local_uint_to_int__0 indirect=false in_cycle=false
; def ulong_to_long params=__zpabi_ulong_to_long__ul_0,__zpabi_ulong_to_long__ul_1,__zpabi_ulong_to_long__ul_2,__zpabi_ulong_to_long__ul_3,__zpabi_ulong_to_long__ul_4,__zpabi_ulong_to_long__ul_5,__zpabi_ulong_to_long__ul_6,__zpabi_ulong_to_long__ul_7,__zpabi_ulong_to_long__expected_0,__zpabi_ulong_to_long__expected_1,__zpabi_ulong_to_long__expected_2,__zpabi_ulong_to_long__expected_3,__zpabi_ulong_to_long__expected_4,__zpabi_ulong_to_long__expected_5,__zpabi_ulong_to_long__expected_6,__zpabi_ulong_to_long__expected_7 locals=__local_ulong_to_long__0 indirect=false in_cycle=false
; call main -> int_to_uint
; call main -> long_to_ulong
; call main -> uint_to_int
; call main -> ulong_to_long
; item uint_to_int section=.text local=false zero=false
; item int_to_uint section=.text local=false zero=false
; item ulong_to_long section=.text local=false zero=false
; item long_to_ulong section=.text local=false zero=false
; item main section=.text local=false zero=false
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

uint_to_int:
   SUBROUTINE

   LDA   __zpabi_uint_to_int__ui_3
   CMP   __zpabi_uint_to_int__expected_3
   BNE   .thread@0
   LDA   __zpabi_uint_to_int__ui_2
   CMP   __zpabi_uint_to_int__expected_2
   BNE   .thread@0
   LDA   __zpabi_uint_to_int__ui_1
   CMP   __zpabi_uint_to_int__expected_1
   BNE   .thread@0
   LDA   __zpabi_uint_to_int__ui_0
   CMP   __zpabi_uint_to_int__expected_0
   BEQ   .cmp_true@0
.thread@0:
   LDA   #$00
   BEQ   .cmp_end@1
.cmp_true@0:
   LDA   #$01
.cmp_end@1:
   LDX   #$00
   RTS

int_to_uint:
   SUBROUTINE

   LDA   __zpabi_int_to_uint__i_3
   CMP   __zpabi_int_to_uint__expected_3
   BNE   .thread@1
   LDA   __zpabi_int_to_uint__i_2
   CMP   __zpabi_int_to_uint__expected_2
   BNE   .thread@1
   LDA   __zpabi_int_to_uint__i_1
   CMP   __zpabi_int_to_uint__expected_1
   BNE   .thread@1
   LDA   __zpabi_int_to_uint__i_0
   CMP   __zpabi_int_to_uint__expected_0
   BEQ   .cmp_true@3
.thread@1:
   LDA   #$00
   BEQ   .cmp_end@4
.cmp_true@3:
   LDA   #$01
.cmp_end@4:
   LDX   #$00
   RTS

ulong_to_long:
   SUBROUTINE

   LDA   __zpabi_ulong_to_long__ul_7
   CMP   __zpabi_ulong_to_long__expected_7
   BNE   .thread@2
   LDA   __zpabi_ulong_to_long__ul_6
   CMP   __zpabi_ulong_to_long__expected_6
   BNE   .thread@2
   LDA   __zpabi_ulong_to_long__ul_5
   CMP   __zpabi_ulong_to_long__expected_5
   BNE   .thread@2
   LDA   __zpabi_ulong_to_long__ul_4
   CMP   __zpabi_ulong_to_long__expected_4
   BNE   .thread@2
   LDA   __zpabi_ulong_to_long__ul_3
   CMP   __zpabi_ulong_to_long__expected_3
   BNE   .thread@2
   LDA   __zpabi_ulong_to_long__ul_2
   CMP   __zpabi_ulong_to_long__expected_2
   BNE   .thread@2
   LDA   __zpabi_ulong_to_long__ul_1
   CMP   __zpabi_ulong_to_long__expected_1
   BNE   .thread@2
   LDA   __zpabi_ulong_to_long__ul_0
   CMP   __zpabi_ulong_to_long__expected_0
   BEQ   .cmp_true@6
.thread@2:
   LDA   #$00
   BEQ   .cmp_end@7
.cmp_true@6:
   LDA   #$01
.cmp_end@7:
   LDX   #$00
   RTS

long_to_ulong:
   SUBROUTINE

   LDA   __zpabi_long_to_ulong__l_7
   CMP   __zpabi_long_to_ulong__expected_7
   BNE   .thread@3
   LDA   __zpabi_long_to_ulong__l_6
   CMP   __zpabi_long_to_ulong__expected_6
   BNE   .thread@3
   LDA   __zpabi_long_to_ulong__l_5
   CMP   __zpabi_long_to_ulong__expected_5
   BNE   .thread@3
   LDA   __zpabi_long_to_ulong__l_4
   CMP   __zpabi_long_to_ulong__expected_4
   BNE   .thread@3
   LDA   __zpabi_long_to_ulong__l_3
   CMP   __zpabi_long_to_ulong__expected_3
   BNE   .thread@3
   LDA   __zpabi_long_to_ulong__l_2
   CMP   __zpabi_long_to_ulong__expected_2
   BNE   .thread@3
   LDA   __zpabi_long_to_ulong__l_1
   CMP   __zpabi_long_to_ulong__expected_1
   BNE   .thread@3
   LDA   __zpabi_long_to_ulong__l_0
   CMP   __zpabi_long_to_ulong__expected_0
   BEQ   .cmp_true@9
.thread@3:
   LDA   #$00
   BEQ   .cmp_end@10
.cmp_true@9:
   LDA   #$01
.cmp_end@10:
   LDX   #$00
   RTS

main:
   SUBROUTINE

   LDA   #$0A
   STA   __zpabi_int_to_uint__i_0
   STA   __zpabi_int_to_uint__expected_0
   LDA   #$00
   STA   __zpabi_int_to_uint__i_1
   STA   __zpabi_int_to_uint__i_2
   STA   __zpabi_int_to_uint__i_3
   STA   __zpabi_int_to_uint__expected_1
   STA   __zpabi_int_to_uint__expected_2
   STA   __zpabi_int_to_uint__expected_3
   JSR   int_to_uint
   STX   __local_main__0
   ORA   __local_main__0
   BNE   .if_end@0
   LDX   #$00
   LDA   #$01
   RTS
.if_end@0:
   LDA   #$0A
   STA   __zpabi_uint_to_int__ui_0
   STA   __zpabi_uint_to_int__expected_0
   LDA   #$00
   STA   __zpabi_uint_to_int__ui_1
   STA   __zpabi_uint_to_int__ui_2
   STA   __zpabi_uint_to_int__ui_3
   STA   __zpabi_uint_to_int__expected_1
   STA   __zpabi_uint_to_int__expected_2
   STA   __zpabi_uint_to_int__expected_3
   JSR   uint_to_int
   STX   __local_main__0
   ORA   __local_main__0
   BNE   .if_end@1
   LDX   #$00
   LDA   #$02
   RTS
.if_end@1:
   LDA   #$18
   STA   __zpabi_long_to_ulong__l_0
   STA   __zpabi_long_to_ulong__expected_0
   LDA   #$FC
   STA   __zpabi_long_to_ulong__l_1
   STA   __zpabi_long_to_ulong__expected_1
   LDA   #$FF
   STA   __zpabi_long_to_ulong__l_2
   STA   __zpabi_long_to_ulong__l_3
   STA   __zpabi_long_to_ulong__l_4
   STA   __zpabi_long_to_ulong__l_5
   STA   __zpabi_long_to_ulong__l_6
   STA   __zpabi_long_to_ulong__l_7
   STA   __zpabi_long_to_ulong__expected_2
   STA   __zpabi_long_to_ulong__expected_3
   LDA   #$00
   STA   __zpabi_long_to_ulong__expected_4
   STA   __zpabi_long_to_ulong__expected_5
   STA   __zpabi_long_to_ulong__expected_6
   STA   __zpabi_long_to_ulong__expected_7
   JSR   long_to_ulong
   STX   __local_main__0
   ORA   __local_main__0
   BNE   .if_end@2
   LDX   #$00
   LDA   #$03
   RTS
.if_end@2:
   LDA   #$18
   STA   __zpabi_ulong_to_long__ul_0
   STA   __zpabi_ulong_to_long__expected_0
   LDA   #$FC
   STA   __zpabi_ulong_to_long__ul_1
   STA   __zpabi_ulong_to_long__expected_1
   LDA   #$00
   STA   __zpabi_ulong_to_long__ul_4
   STA   __zpabi_ulong_to_long__ul_5
   STA   __zpabi_ulong_to_long__ul_6
   STA   __zpabi_ulong_to_long__ul_7
   LDA   #$FF
   STA   __zpabi_ulong_to_long__ul_2
   STA   __zpabi_ulong_to_long__ul_3
   STA   __zpabi_ulong_to_long__expected_2
   STA   __zpabi_ulong_to_long__expected_3
   STA   __zpabi_ulong_to_long__expected_4
   STA   __zpabi_ulong_to_long__expected_5
   STA   __zpabi_ulong_to_long__expected_6
   STA   __zpabi_ulong_to_long__expected_7
   JSR   ulong_to_long
   STX   __local_main__0
   ORA   __local_main__0
   BNE   .if_end@3
   LDX   #$00
   LDA   #$04
   RTS
.if_end@3:
   LDX   #$00
   TXA
   RTS
