SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__zpabi_uint_to_double__ui_0	EQU	$80
__zpabi_ulong_to_double__ul_0	EQU	$80
__zpabi_uint_to_double__ui_1	EQU	$81
__zpabi_ulong_to_double__ul_1	EQU	$81
__zpabi_uint_to_double__ui_2	EQU	$82
__zpabi_ulong_to_double__ul_2	EQU	$82
__zpabi_uint_to_double__ui_3	EQU	$83
__zpabi_ulong_to_double__ul_3	EQU	$83
__zpabi_ulong_to_double__ul_4	EQU	$84
__zpabi_ulong_to_double__ul_5	EQU	$85
__zpabi_ulong_to_double__ul_6	EQU	$86
__zpabi_ulong_to_double__ul_7	EQU	$87

; @zp-link-meta-begin
; def main params= locals=__local_main__0,__local_main__1,__local_main__2,__local_main__3,__local_main__4,__local_main__5,__local_main__6,__local_main__7 indirect=false in_cycle=false
; def uint_to_double params=__zpabi_uint_to_double__ui_0,__zpabi_uint_to_double__ui_1,__zpabi_uint_to_double__ui_2,__zpabi_uint_to_double__ui_3 locals= indirect=false in_cycle=false
; def ulong_to_double params=__zpabi_ulong_to_double__ul_0,__zpabi_ulong_to_double__ul_1,__zpabi_ulong_to_double__ul_2,__zpabi_ulong_to_double__ul_3,__zpabi_ulong_to_double__ul_4,__zpabi_ulong_to_double__ul_5,__zpabi_ulong_to_double__ul_6,__zpabi_ulong_to_double__ul_7 locals= indirect=false in_cycle=false
; call main -> uint_to_double
; call main -> ulong_to_double
; item uint_to_double section=.text local=false zero=false
; item ulong_to_double section=.text local=false zero=false
; item main section=.text local=false zero=false
; item main@run1 section=.text local=true zero=false
; item main@run0 section=.text local=true zero=false
; runtime hargs_size=32 hargs_need=24
; @zp-link-meta-end

uint_to_double:
   SUBROUTINE

   LDA   __zpabi_uint_to_double__ui_0
   STA   HARGS
   LDA   __zpabi_uint_to_double__ui_1
   STA   HARGS+1
   LDA   __zpabi_uint_to_double__ui_2
   STA   HARGS+2
   LDA   __zpabi_uint_to_double__ui_3
   STA   HARGS+3
   JSR   ul2d
   LDA   HARGS+4
   STA   HARGS+16
   LDA   HARGS+5
   STA   HARGS+17
   LDA   HARGS+6
   STA   HARGS+18
   LDA   HARGS+7
   STA   HARGS+19
   LDA   HARGS+8
   STA   HARGS+20
   LDA   HARGS+9
   STA   HARGS+21
   LDA   HARGS+10
   STA   HARGS+22
   LDA   HARGS+11
   STA   HARGS+23
   RTS

ulong_to_double:
   SUBROUTINE

   LDA   __zpabi_ulong_to_double__ul_0
   STA   HARGS
   LDA   __zpabi_ulong_to_double__ul_1
   STA   HARGS+1
   LDA   __zpabi_ulong_to_double__ul_2
   STA   HARGS+2
   LDA   __zpabi_ulong_to_double__ul_3
   STA   HARGS+3
   LDA   __zpabi_ulong_to_double__ul_4
   STA   HARGS+4
   LDA   __zpabi_ulong_to_double__ul_5
   STA   HARGS+5
   LDA   __zpabi_ulong_to_double__ul_6
   STA   HARGS+6
   LDA   __zpabi_ulong_to_double__ul_7
   STA   HARGS+7
   JSR   ull2d
   LDA   HARGS+8
   STA   HARGS+16
   LDA   HARGS+9
   STA   HARGS+17
   LDA   HARGS+10
   STA   HARGS+18
   LDA   HARGS+11
   STA   HARGS+19
   LDA   HARGS+12
   STA   HARGS+20
   LDA   HARGS+13
   STA   HARGS+21
   LDA   HARGS+14
   STA   HARGS+22
   LDA   HARGS+15
   STA   HARGS+23
   RTS

main:
   SUBROUTINE

   LDA   #$E8
   STA   __zpabi_uint_to_double__ui_0
   LDA   #$03
   STA   __zpabi_uint_to_double__ui_1
   JSR   main@run1
   BNE   .jcmp_skip@0
   LDA   HARGS+22
   CMP   #$8F
   BNE   .jcmp_skip@0
   LDA   HARGS+21
   CMP   #$40
   BNE   .jcmp_skip@0
   LDA   HARGS+20
   BNE   .jcmp_skip@0
   LDA   HARGS+19
   BNE   .jcmp_skip@0
   LDA   HARGS+18
   BNE   .jcmp_skip@0
   LDA   HARGS+17
   BNE   .jcmp_skip@0
   LDA   HARGS+16
   BEQ   .if_end@0
.jcmp_skip@0:
   LDX   #$00
   LDA   #$01
   RTS
.if_end@0:
   LDA   #$60
   STA   __zpabi_uint_to_double__ui_0
   LDA   #$EA
   STA   __zpabi_uint_to_double__ui_1
   JSR   main@run1
   BNE   .jcmp_skip@1
   LDA   HARGS+22
   CMP   #$ED
   BNE   .jcmp_skip@1
   LDA   HARGS+21
   CMP   #$4C
   BNE   .jcmp_skip@1
   LDA   HARGS+20
   BNE   .jcmp_skip@1
   LDA   HARGS+19
   BNE   .jcmp_skip@1
   LDA   HARGS+18
   BNE   .jcmp_skip@1
   LDA   HARGS+17
   BNE   .jcmp_skip@1
   LDA   HARGS+16
   BEQ   .if_end@1
.jcmp_skip@1:
   LDX   #$00
   LDA   #$02
   RTS
.if_end@1:
   LDA   #$CA
   STA   __zpabi_ulong_to_double__ul_1
   LDA   #$9A
   STA   __zpabi_ulong_to_double__ul_2
   LDA   #$3B
   JSR   main@run0
   BNE   .jcmp_skip@2
   LDA   HARGS+22
   CMP   #$CD
   BNE   .jcmp_skip@2
   LDA   HARGS+21
   CMP   #$CD
   BNE   .jcmp_skip@2
   LDA   HARGS+20
   CMP   #$65
   BNE   .jcmp_skip@2
   LDA   HARGS+19
   BNE   .jcmp_skip@2
   LDA   HARGS+18
   BNE   .jcmp_skip@2
   LDA   HARGS+17
   BNE   .jcmp_skip@2
   LDA   HARGS+16
   BEQ   .if_end@2
.jcmp_skip@2:
   LDX   #$00
   LDA   #$03
   RTS
.if_end@2:
   LDA   #$5E
   STA   __zpabi_ulong_to_double__ul_1
   LDA   #$D0
   STA   __zpabi_ulong_to_double__ul_2
   LDA   #$B2
   JSR   main@run0
   BNE   .jcmp_skip@3
   LDA   HARGS+22
   CMP   #$E6
   BNE   .jcmp_skip@3
   LDA   HARGS+21
   CMP   #$5A
   BNE   .jcmp_skip@3
   LDA   HARGS+20
   CMP   #$0B
   BNE   .jcmp_skip@3
   LDA   HARGS+19
   CMP   #$C0
   BNE   .jcmp_skip@3
   LDA   HARGS+18
   BNE   .jcmp_skip@3
   LDA   HARGS+17
   BNE   .jcmp_skip@3
   LDA   HARGS+16
   BEQ   .if_end@3
.jcmp_skip@3:
   LDX   #$00
   LDA   #$04
   RTS
.if_end@3:
   LDX   #$00
   TXA
   RTS

main@run1:
   SUBROUTINE

   LDA   #$00
   STA   __zpabi_uint_to_double__ui_2
   STA   __zpabi_uint_to_double__ui_3
   JSR   uint_to_double
   LDA   HARGS+23
   CMP   #$40
   RTS

main@run0:
   SUBROUTINE

   STA   __zpabi_ulong_to_double__ul_3
   LDA   #$00
   STA   __zpabi_ulong_to_double__ul_0
   STA   __zpabi_ulong_to_double__ul_4
   STA   __zpabi_ulong_to_double__ul_5
   STA   __zpabi_ulong_to_double__ul_6
   STA   __zpabi_ulong_to_double__ul_7
   JSR   ulong_to_double
   LDA   HARGS+23
   CMP   #$41
   RTS
