SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__zpabi_long_to_uint__l_0	EQU	$80
__zpabi_ulong_to_int__ul_0	EQU	$80
__zpabi_ulong_to_uint__ul_0	EQU	$80
__zpabi_long_to_uint__l_1	EQU	$81
__zpabi_ulong_to_int__ul_1	EQU	$81
__zpabi_ulong_to_uint__ul_1	EQU	$81
__zpabi_long_to_uint__l_2	EQU	$82
__zpabi_ulong_to_int__ul_2	EQU	$82
__zpabi_ulong_to_uint__ul_2	EQU	$82
__zpabi_long_to_uint__l_3	EQU	$83
__zpabi_ulong_to_int__ul_3	EQU	$83
__zpabi_ulong_to_uint__ul_3	EQU	$83
__zpabi_long_to_uint__l_4	EQU	$84
__zpabi_ulong_to_int__ul_4	EQU	$84
__zpabi_ulong_to_uint__ul_4	EQU	$84
__zpabi_long_to_uint__l_5	EQU	$85
__zpabi_ulong_to_int__ul_5	EQU	$85
__zpabi_ulong_to_uint__ul_5	EQU	$85
__zpabi_long_to_uint__l_6	EQU	$86
__zpabi_ulong_to_int__ul_6	EQU	$86
__zpabi_ulong_to_uint__ul_6	EQU	$86
__zpabi_long_to_uint__l_7	EQU	$87
__zpabi_ulong_to_int__ul_7	EQU	$87
__zpabi_ulong_to_uint__ul_7	EQU	$87
__zpabi_long_to_uint__expected_0	EQU	$88
__zpabi_ulong_to_int__expected_0	EQU	$88
__zpabi_ulong_to_uint__expected_0	EQU	$88
__zpabi_long_to_uint__expected_1	EQU	$89
__zpabi_ulong_to_int__expected_1	EQU	$89
__zpabi_ulong_to_uint__expected_1	EQU	$89
__zpabi_long_to_uint__expected_2	EQU	$8A
__zpabi_ulong_to_int__expected_2	EQU	$8A
__zpabi_ulong_to_uint__expected_2	EQU	$8A
__zpabi_long_to_uint__expected_3	EQU	$8B
__zpabi_ulong_to_int__expected_3	EQU	$8B
__zpabi_ulong_to_uint__expected_3	EQU	$8B
__local_main__0	EQU	$8C

; @zp-link-meta-begin
; def long_to_uint params=__zpabi_long_to_uint__l_0,__zpabi_long_to_uint__l_1,__zpabi_long_to_uint__l_2,__zpabi_long_to_uint__l_3,__zpabi_long_to_uint__l_4,__zpabi_long_to_uint__l_5,__zpabi_long_to_uint__l_6,__zpabi_long_to_uint__l_7,__zpabi_long_to_uint__expected_0,__zpabi_long_to_uint__expected_1,__zpabi_long_to_uint__expected_2,__zpabi_long_to_uint__expected_3 locals=__local_long_to_uint__0 indirect=false in_cycle=false
; def main params= locals=__local_main__0,__local_main__1 indirect=false in_cycle=false
; def ulong_to_int params=__zpabi_ulong_to_int__ul_0,__zpabi_ulong_to_int__ul_1,__zpabi_ulong_to_int__ul_2,__zpabi_ulong_to_int__ul_3,__zpabi_ulong_to_int__ul_4,__zpabi_ulong_to_int__ul_5,__zpabi_ulong_to_int__ul_6,__zpabi_ulong_to_int__ul_7,__zpabi_ulong_to_int__expected_0,__zpabi_ulong_to_int__expected_1,__zpabi_ulong_to_int__expected_2,__zpabi_ulong_to_int__expected_3 locals=__local_ulong_to_int__0 indirect=false in_cycle=false
; def ulong_to_uint params=__zpabi_ulong_to_uint__ul_0,__zpabi_ulong_to_uint__ul_1,__zpabi_ulong_to_uint__ul_2,__zpabi_ulong_to_uint__ul_3,__zpabi_ulong_to_uint__ul_4,__zpabi_ulong_to_uint__ul_5,__zpabi_ulong_to_uint__ul_6,__zpabi_ulong_to_uint__ul_7,__zpabi_ulong_to_uint__expected_0,__zpabi_ulong_to_uint__expected_1,__zpabi_ulong_to_uint__expected_2,__zpabi_ulong_to_uint__expected_3 locals=__local_ulong_to_uint__0 indirect=false in_cycle=false
; call main -> long_to_uint
; call main -> ulong_to_int
; call main -> ulong_to_uint
; item ulong_to_int section=.text local=false zero=false
; item ulong_to_uint section=.text local=false zero=false
; item long_to_uint section=.text local=false zero=false
; item main section=.text local=false zero=false
; item main@run5 section=.text local=true zero=false
; item main@run4 section=.text local=true zero=false
; item main@run3 section=.text local=true zero=false
; item main@run2 section=.text local=true zero=false
; item main@run1 section=.text local=true zero=false
; item main@run0 section=.text local=true zero=false
; runtime hargs_size=32 hargs_need=0
; @zp-link-meta-end

ulong_to_int:
   SUBROUTINE

   LDA   __zpabi_ulong_to_int__ul_3
   CMP   __zpabi_ulong_to_int__expected_3
   BNE   .thread@0
   LDA   __zpabi_ulong_to_int__ul_2
   CMP   __zpabi_ulong_to_int__expected_2
   BNE   .thread@0
   LDA   __zpabi_ulong_to_int__ul_1
   CMP   __zpabi_ulong_to_int__expected_1
   BNE   .thread@0
   LDA   __zpabi_ulong_to_int__ul_0
   CMP   __zpabi_ulong_to_int__expected_0
   BEQ   .cmp_true@0
.thread@0:
   LDA   #$00
   BEQ   .cmp_end@1
.cmp_true@0:
   LDA   #$01
.cmp_end@1:
   LDX   #$00
   RTS

ulong_to_uint:
   SUBROUTINE

   LDA   __zpabi_ulong_to_uint__ul_3
   CMP   __zpabi_ulong_to_uint__expected_3
   BNE   .thread@1
   LDA   __zpabi_ulong_to_uint__ul_2
   CMP   __zpabi_ulong_to_uint__expected_2
   BNE   .thread@1
   LDA   __zpabi_ulong_to_uint__ul_1
   CMP   __zpabi_ulong_to_uint__expected_1
   BNE   .thread@1
   LDA   __zpabi_ulong_to_uint__ul_0
   CMP   __zpabi_ulong_to_uint__expected_0
   BEQ   .cmp_true@3
.thread@1:
   LDA   #$00
   BEQ   .cmp_end@4
.cmp_true@3:
   LDA   #$01
.cmp_end@4:
   LDX   #$00
   RTS

long_to_uint:
   SUBROUTINE

   LDA   __zpabi_long_to_uint__l_3
   CMP   __zpabi_long_to_uint__expected_3
   BNE   .thread@2
   LDA   __zpabi_long_to_uint__l_2
   CMP   __zpabi_long_to_uint__expected_2
   BNE   .thread@2
   LDA   __zpabi_long_to_uint__l_1
   CMP   __zpabi_long_to_uint__expected_1
   BNE   .thread@2
   LDA   __zpabi_long_to_uint__l_0
   CMP   __zpabi_long_to_uint__expected_0
   BEQ   .cmp_true@6
.thread@2:
   LDA   #$00
   BEQ   .cmp_end@7
.cmp_true@6:
   LDA   #$01
.cmp_end@7:
   LDX   #$00
   RTS

main:
   SUBROUTINE

   LDA   #$64
   STA   __zpabi_long_to_uint__l_0
   STA   __zpabi_long_to_uint__expected_0
   LDA   #$00
   STA   __zpabi_long_to_uint__l_1
   STA   __zpabi_long_to_uint__l_2
   STA   __zpabi_long_to_uint__l_3
   JSR   main@run5
   STA   __zpabi_long_to_uint__expected_1
   JSR   main@run4
   STX   __local_main__0
   ORA   __local_main__0
   BNE   .if_end@0
   LDX   #$00
   LDA   #$01
   RTS
.if_end@0:
   LDA   #$D2
   STA   __zpabi_long_to_uint__l_0
   STA   __zpabi_long_to_uint__expected_0
   LDA   #$04
   STA   __zpabi_long_to_uint__l_1
   STA   __zpabi_long_to_uint__expected_1
   LDA   #$80
   STA   __zpabi_long_to_uint__l_3
   LDA   #$FF
   JSR   main@run5
   LDA   #$00
   STA   __zpabi_long_to_uint__l_2
   JSR   main@run4
   STX   __local_main__0
   ORA   __local_main__0
   BNE   .if_end@1
   LDX   #$00
   LDA   #$02
   RTS
.if_end@1:
   LDA   #$64
   STA   __zpabi_ulong_to_int__ul_0
   STA   __zpabi_ulong_to_int__expected_0
   LDA   #$00
   STA   __zpabi_ulong_to_int__ul_1
   STA   __zpabi_ulong_to_int__ul_2
   JSR   main@run0
   STA   __zpabi_ulong_to_int__expected_1
   JSR   main@run2
   STX   __local_main__0
   ORA   __local_main__0
   BNE   .if_end@2
   LDX   #$00
   LDA   #$03
   RTS
.if_end@2:
   LDA   #$64
   STA   __zpabi_ulong_to_uint__ul_0
   STA   __zpabi_ulong_to_uint__expected_0
   LDA   #$00
   STA   __zpabi_ulong_to_uint__ul_1
   STA   __zpabi_ulong_to_uint__ul_2
   JSR   main@run1
   STA   __zpabi_ulong_to_uint__expected_1
   JSR   main@run3
   STX   __local_main__0
   ORA   __local_main__0
   BNE   .if_end@3
   LDX   #$00
   LDA   #$04
   RTS
.if_end@3:
   LDA   #$A0
   STA   __zpabi_ulong_to_uint__ul_0
   STA   __zpabi_ulong_to_uint__expected_0
   LDA   #$FF
   STA   __zpabi_ulong_to_uint__ul_1
   STA   __zpabi_ulong_to_uint__expected_1
   LDA   #$00
   STA   __zpabi_ulong_to_uint__ul_2
   JSR   main@run1
   JSR   main@run3
   STX   __local_main__0
   ORA   __local_main__0
   BNE   .if_end@4
   LDX   #$00
   LDA   #$05
   RTS
.if_end@4:
   LDA   #$A0
   STA   __zpabi_ulong_to_int__ul_0
   STA   __zpabi_ulong_to_int__expected_0
   LDA   #$00
   STA   __zpabi_ulong_to_int__ul_2
   JSR   main@run0
   LDA   #$FF
   STA   __zpabi_ulong_to_int__ul_1
   STA   __zpabi_ulong_to_int__expected_1
   JSR   main@run2
   STX   __local_main__0
   ORA   __local_main__0
   BNE   .if_end@5
   LDX   #$00
   LDA   #$06
   RTS
.if_end@5:
   LDA   #$80
   STA   __zpabi_ulong_to_uint__ul_1
   STA   __zpabi_ulong_to_uint__expected_1
   LDA   #$01
   STA   __zpabi_ulong_to_uint__ul_2
   LDA   #$00
   STA   __zpabi_ulong_to_uint__ul_0
   JSR   main@run1
   STA   __zpabi_ulong_to_uint__expected_0
   JSR   main@run3
   STX   __local_main__0
   ORA   __local_main__0
   BNE   .if_end@6
   LDX   #$00
   LDA   #$07
   RTS
.if_end@6:
   LDA   #$00
   STA   __zpabi_ulong_to_int__ul_0
   JSR   main@run0
   STA   __zpabi_ulong_to_int__expected_0
   LDA   #$80
   STA   __zpabi_ulong_to_int__ul_1
   STA   __zpabi_ulong_to_int__expected_1
   LDA   #$01
   STA   __zpabi_ulong_to_int__ul_2
   LDA   #$FF
   JSR   main@run2
   STX   __local_main__0
   ORA   __local_main__0
   BNE   .if_end@7
   LDX   #$00
   LDA   #$08
   RTS
.if_end@7:
   LDX   #$00
   LDA   #$09
   RTS

main@run5:
   SUBROUTINE

   STA   __zpabi_long_to_uint__l_4
   STA   __zpabi_long_to_uint__l_5
   STA   __zpabi_long_to_uint__l_6
   STA   __zpabi_long_to_uint__l_7
   RTS

main@run4:
   SUBROUTINE

   STA   __zpabi_long_to_uint__expected_2
   STA   __zpabi_long_to_uint__expected_3
   JMP   long_to_uint

main@run3:
   SUBROUTINE

   STA   __zpabi_ulong_to_uint__expected_2
   STA   __zpabi_ulong_to_uint__expected_3
   JMP   ulong_to_uint

main@run2:
   SUBROUTINE

   STA   __zpabi_ulong_to_int__expected_2
   STA   __zpabi_ulong_to_int__expected_3
   JMP   ulong_to_int

main@run1:
   SUBROUTINE

   STA   __zpabi_ulong_to_uint__ul_3
   STA   __zpabi_ulong_to_uint__ul_4
   STA   __zpabi_ulong_to_uint__ul_5
   STA   __zpabi_ulong_to_uint__ul_6
   STA   __zpabi_ulong_to_uint__ul_7
   RTS

main@run0:
   SUBROUTINE

   STA   __zpabi_ulong_to_int__ul_3
   STA   __zpabi_ulong_to_int__ul_4
   STA   __zpabi_ulong_to_int__ul_5
   STA   __zpabi_ulong_to_int__ul_6
   STA   __zpabi_ulong_to_int__ul_7
   RTS
