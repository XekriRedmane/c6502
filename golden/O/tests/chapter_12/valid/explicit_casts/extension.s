SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__zpabi_int_to_ulong__expected_0	EQU	$80
__zpabi_uint_to_long__ui_0	EQU	$80
__zpabi_uint_to_ulong__ui_0	EQU	$80
__zpabi_int_to_ulong__expected_1	EQU	$81
__zpabi_uint_to_long__ui_1	EQU	$81
__zpabi_uint_to_ulong__ui_1	EQU	$81
__zpabi_int_to_ulong__expected_2	EQU	$82
__zpabi_uint_to_long__ui_2	EQU	$82
__zpabi_uint_to_ulong__ui_2	EQU	$82
__zpabi_int_to_ulong__expected_3	EQU	$83
__zpabi_uint_to_long__ui_3	EQU	$83
__zpabi_uint_to_ulong__ui_3	EQU	$83
__zpabi_int_to_ulong__expected_4	EQU	$84
__zpabi_uint_to_long__expected_0	EQU	$84
__zpabi_uint_to_ulong__expected_0	EQU	$84
__zpabi_int_to_ulong__expected_5	EQU	$85
__zpabi_uint_to_long__expected_1	EQU	$85
__zpabi_uint_to_ulong__expected_1	EQU	$85
__zpabi_int_to_ulong__expected_6	EQU	$86
__zpabi_uint_to_long__expected_2	EQU	$86
__zpabi_uint_to_ulong__expected_2	EQU	$86
__zpabi_int_to_ulong__expected_7	EQU	$87
__zpabi_uint_to_long__expected_3	EQU	$87
__zpabi_uint_to_ulong__expected_3	EQU	$87
__zpabi_uint_to_long__expected_4	EQU	$88
__zpabi_uint_to_ulong__expected_4	EQU	$88
__zpabi_uint_to_long__expected_5	EQU	$89
__zpabi_uint_to_ulong__expected_5	EQU	$89
__zpabi_uint_to_long__expected_6	EQU	$8A
__zpabi_uint_to_ulong__expected_6	EQU	$8A
__zpabi_uint_to_long__expected_7	EQU	$8B
__zpabi_uint_to_ulong__expected_7	EQU	$8B
__local_main__0	EQU	$8C
__local_int_to_ulong__i_0	EQU	$95

; @zp-link-meta-begin
; def int_to_ulong params=__zpabi_int_to_ulong__i_0,__zpabi_int_to_ulong__i_1,__zpabi_int_to_ulong__expected_0,__zpabi_int_to_ulong__expected_1,__zpabi_int_to_ulong__expected_2,__zpabi_int_to_ulong__expected_3,__zpabi_int_to_ulong__expected_4,__zpabi_int_to_ulong__expected_5,__zpabi_int_to_ulong__expected_6,__zpabi_int_to_ulong__expected_7 locals=__local_int_to_ulong__0,__local_int_to_ulong__1,__local_int_to_ulong__2,__local_int_to_ulong__3,__local_int_to_ulong__4,__local_int_to_ulong__5,__local_int_to_ulong__i_1,__local_int_to_ulong__i_0 indirect=false in_cycle=false param_regs=A,X,,,,,,,,
; def main params= locals=__local_main__0,__local_main__1 indirect=false in_cycle=false
; def uint_to_long params=__zpabi_uint_to_long__ui_0,__zpabi_uint_to_long__ui_1,__zpabi_uint_to_long__ui_2,__zpabi_uint_to_long__ui_3,__zpabi_uint_to_long__expected_0,__zpabi_uint_to_long__expected_1,__zpabi_uint_to_long__expected_2,__zpabi_uint_to_long__expected_3,__zpabi_uint_to_long__expected_4,__zpabi_uint_to_long__expected_5,__zpabi_uint_to_long__expected_6,__zpabi_uint_to_long__expected_7 locals=__local_uint_to_long__0 indirect=false in_cycle=false
; def uint_to_ulong params=__zpabi_uint_to_ulong__ui_0,__zpabi_uint_to_ulong__ui_1,__zpabi_uint_to_ulong__ui_2,__zpabi_uint_to_ulong__ui_3,__zpabi_uint_to_ulong__expected_0,__zpabi_uint_to_ulong__expected_1,__zpabi_uint_to_ulong__expected_2,__zpabi_uint_to_ulong__expected_3,__zpabi_uint_to_ulong__expected_4,__zpabi_uint_to_ulong__expected_5,__zpabi_uint_to_ulong__expected_6,__zpabi_uint_to_ulong__expected_7 locals=__local_uint_to_ulong__0 indirect=false in_cycle=false
; call main -> int_to_ulong
; call main -> uint_to_long
; call main -> uint_to_ulong
; item int_to_ulong section=.text local=false zero=false
; item uint_to_long section=.text local=false zero=false
; item uint_to_ulong section=.text local=false zero=false
; item main section=.text local=false zero=false
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

int_to_ulong:
   SUBROUTINE

   STA   __local_int_to_ulong__i_0
   TXA
   BMI   .sx_neg@0
   LDA   #$00
   BEQ   .sx_done@1
.sx_neg@0:
   LDA   #$FF
.sx_done@1:
   CMP   __zpabi_int_to_ulong__expected_7
   BNE   .thread@0
   CMP   __zpabi_int_to_ulong__expected_6
   BNE   .thread@0
   CMP   __zpabi_int_to_ulong__expected_5
   BNE   .thread@0
   CMP   __zpabi_int_to_ulong__expected_4
   BNE   .thread@0
   CMP   __zpabi_int_to_ulong__expected_3
   BNE   .thread@0
   CMP   __zpabi_int_to_ulong__expected_2
   BNE   .thread@0
   CPX   __zpabi_int_to_ulong__expected_1
   BNE   .thread@0
   LDA   __local_int_to_ulong__i_0
   CMP   __zpabi_int_to_ulong__expected_0
   BEQ   .cmp_true@2
.thread@0:
   LDA   #$00
   BEQ   .cmp_end@3
.cmp_true@2:
   LDA   #$01
.cmp_end@3:
   LDX   #$00
   RTS

uint_to_long:
   SUBROUTINE

   LDA   #$00
   CMP   __zpabi_uint_to_long__expected_7
   BNE   .cmp_end@6
   CMP   __zpabi_uint_to_long__expected_6
   BNE   .cmp_end@6
   CMP   __zpabi_uint_to_long__expected_5
   BNE   .cmp_end@6
   CMP   __zpabi_uint_to_long__expected_4
   BNE   .cmp_end@6
   LDA   __zpabi_uint_to_long__ui_3
   CMP   __zpabi_uint_to_long__expected_3
   BNE   .thread@1
   LDA   __zpabi_uint_to_long__ui_2
   CMP   __zpabi_uint_to_long__expected_2
   BNE   .thread@1
   LDA   __zpabi_uint_to_long__ui_1
   CMP   __zpabi_uint_to_long__expected_1
   BNE   .thread@1
   LDA   __zpabi_uint_to_long__ui_0
   CMP   __zpabi_uint_to_long__expected_0
   BEQ   .cmp_true@5
.thread@1:
   LDA   #$00
   BEQ   .cmp_end@6
.cmp_true@5:
   LDA   #$01
.cmp_end@6:
   LDX   #$00
   RTS

uint_to_ulong:
   SUBROUTINE

   LDA   #$00
   CMP   __zpabi_uint_to_ulong__expected_7
   BNE   .cmp_end@9
   CMP   __zpabi_uint_to_ulong__expected_6
   BNE   .cmp_end@9
   CMP   __zpabi_uint_to_ulong__expected_5
   BNE   .cmp_end@9
   CMP   __zpabi_uint_to_ulong__expected_4
   BNE   .cmp_end@9
   LDA   __zpabi_uint_to_ulong__ui_3
   CMP   __zpabi_uint_to_ulong__expected_3
   BNE   .thread@2
   LDA   __zpabi_uint_to_ulong__ui_2
   CMP   __zpabi_uint_to_ulong__expected_2
   BNE   .thread@2
   LDA   __zpabi_uint_to_ulong__ui_1
   CMP   __zpabi_uint_to_ulong__expected_1
   BNE   .thread@2
   LDA   __zpabi_uint_to_ulong__ui_0
   CMP   __zpabi_uint_to_ulong__expected_0
   BEQ   .cmp_true@8
.thread@2:
   LDA   #$00
   BEQ   .cmp_end@9
.cmp_true@8:
   LDA   #$01
.cmp_end@9:
   LDX   #$00
   RTS

main:
   SUBROUTINE

   LDA   #$0A
   STA   __zpabi_int_to_ulong__expected_0
   LDA   #$00
   STA   __zpabi_int_to_ulong__expected_1
   STA   __zpabi_int_to_ulong__expected_2
   STA   __zpabi_int_to_ulong__expected_3
   STA   __zpabi_int_to_ulong__expected_4
   STA   __zpabi_int_to_ulong__expected_5
   STA   __zpabi_int_to_ulong__expected_6
   STA   __zpabi_int_to_ulong__expected_7
   LDX   #$00
   LDA   #$0A
   JSR   int_to_ulong
   STX   __local_main__0
   ORA   __local_main__0
   BNE   .if_end@0
   LDX   #$00
   LDA   #$01
   RTS
.if_end@0:
   LDA   #$F6
   STA   __zpabi_int_to_ulong__expected_0
   LDA   #$FF
   STA   __zpabi_int_to_ulong__expected_1
   STA   __zpabi_int_to_ulong__expected_2
   STA   __zpabi_int_to_ulong__expected_3
   LDA   #$00
   STA   __zpabi_int_to_ulong__expected_4
   STA   __zpabi_int_to_ulong__expected_5
   STA   __zpabi_int_to_ulong__expected_6
   STA   __zpabi_int_to_ulong__expected_7
   LDX   #$FF
   LDA   #$F6
   JSR   int_to_ulong
   STX   __local_main__0
   ORA   __local_main__0
   BNE   .if_end@1
   LDX   #$00
   LDA   #$02
   RTS
.if_end@1:
   LDA   #$A0
   STA   __zpabi_uint_to_long__ui_0
   STA   __zpabi_uint_to_long__expected_0
   LDA   #$FF
   STA   __zpabi_uint_to_long__ui_1
   STA   __zpabi_uint_to_long__expected_1
   LDA   #$00
   STA   __zpabi_uint_to_long__ui_2
   STA   __zpabi_uint_to_long__ui_3
   STA   __zpabi_uint_to_long__expected_2
   STA   __zpabi_uint_to_long__expected_3
   STA   __zpabi_uint_to_long__expected_4
   STA   __zpabi_uint_to_long__expected_5
   STA   __zpabi_uint_to_long__expected_6
   STA   __zpabi_uint_to_long__expected_7
   JSR   uint_to_long
   STX   __local_main__0
   ORA   __local_main__0
   BNE   .if_end@2
   LDX   #$00
   LDA   #$03
   RTS
.if_end@2:
   LDA   #$A0
   STA   __zpabi_uint_to_ulong__ui_0
   STA   __zpabi_uint_to_ulong__expected_0
   LDA   #$FF
   STA   __zpabi_uint_to_ulong__ui_1
   STA   __zpabi_uint_to_ulong__expected_1
   LDA   #$00
   STA   __zpabi_uint_to_ulong__ui_2
   STA   __zpabi_uint_to_ulong__ui_3
   STA   __zpabi_uint_to_ulong__expected_2
   STA   __zpabi_uint_to_ulong__expected_3
   STA   __zpabi_uint_to_ulong__expected_4
   STA   __zpabi_uint_to_ulong__expected_5
   STA   __zpabi_uint_to_ulong__expected_6
   STA   __zpabi_uint_to_ulong__expected_7
   JSR   uint_to_ulong
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
