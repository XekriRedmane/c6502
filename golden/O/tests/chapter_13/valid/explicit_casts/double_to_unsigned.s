SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__zpabi_double_to_uint__d_0	EQU	$80
__zpabi_double_to_ulong__d_0	EQU	$80
__zpabi_double_to_uint__d_1	EQU	$81
__zpabi_double_to_ulong__d_1	EQU	$81
__zpabi_double_to_uint__d_2	EQU	$82
__zpabi_double_to_ulong__d_2	EQU	$82
__zpabi_double_to_uint__d_3	EQU	$83
__zpabi_double_to_ulong__d_3	EQU	$83
__zpabi_double_to_uint__d_4	EQU	$84
__zpabi_double_to_ulong__d_4	EQU	$84
__zpabi_double_to_uint__d_5	EQU	$85
__zpabi_double_to_ulong__d_5	EQU	$85
__zpabi_double_to_uint__d_6	EQU	$86
__zpabi_double_to_ulong__d_6	EQU	$86
__zpabi_double_to_uint__d_7	EQU	$87
__zpabi_double_to_ulong__d_7	EQU	$87

; @zp-link-meta-begin
; def double_to_uint params=__zpabi_double_to_uint__d_0,__zpabi_double_to_uint__d_1,__zpabi_double_to_uint__d_2,__zpabi_double_to_uint__d_3,__zpabi_double_to_uint__d_4,__zpabi_double_to_uint__d_5,__zpabi_double_to_uint__d_6,__zpabi_double_to_uint__d_7 locals= indirect=false in_cycle=false
; def double_to_ulong params=__zpabi_double_to_ulong__d_0,__zpabi_double_to_ulong__d_1,__zpabi_double_to_ulong__d_2,__zpabi_double_to_ulong__d_3,__zpabi_double_to_ulong__d_4,__zpabi_double_to_ulong__d_5,__zpabi_double_to_ulong__d_6,__zpabi_double_to_ulong__d_7 locals= indirect=false in_cycle=false
; def main params= locals=__local_main__0,__local_main__1,__local_main__2,__local_main__3,__local_main__4,__local_main__5,__local_main__6,__local_main__7 indirect=false in_cycle=false
; call main -> double_to_uint
; call main -> double_to_ulong
; item double_to_uint section=.text local=false zero=false
; item double_to_ulong section=.text local=false zero=false
; item main section=.text local=false zero=false
; item main@run0 section=.text local=true zero=false
; runtime hargs_size=32 hargs_need=24
; @zp-link-meta-end

double_to_uint:
   SUBROUTINE

   LDA   __zpabi_double_to_uint__d_0
   STA   HARGS
   LDA   __zpabi_double_to_uint__d_1
   STA   HARGS+1
   LDA   __zpabi_double_to_uint__d_2
   STA   HARGS+2
   LDA   __zpabi_double_to_uint__d_3
   STA   HARGS+3
   LDA   __zpabi_double_to_uint__d_4
   STA   HARGS+4
   LDA   __zpabi_double_to_uint__d_5
   STA   HARGS+5
   LDA   __zpabi_double_to_uint__d_6
   STA   HARGS+6
   LDA   __zpabi_double_to_uint__d_7
   STA   HARGS+7
   JMP   d2ul

double_to_ulong:
   SUBROUTINE

   LDA   __zpabi_double_to_ulong__d_0
   STA   HARGS
   LDA   __zpabi_double_to_ulong__d_1
   STA   HARGS+1
   LDA   __zpabi_double_to_ulong__d_2
   STA   HARGS+2
   LDA   __zpabi_double_to_ulong__d_3
   STA   HARGS+3
   LDA   __zpabi_double_to_ulong__d_4
   STA   HARGS+4
   LDA   __zpabi_double_to_ulong__d_5
   STA   HARGS+5
   LDA   __zpabi_double_to_ulong__d_6
   STA   HARGS+6
   LDA   __zpabi_double_to_ulong__d_7
   STA   HARGS+7
   JSR   d2ull
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

   LDA   #$CD
   STA   __zpabi_double_to_uint__d_0
   LDA   #$CC
   STA   __zpabi_double_to_uint__d_1
   STA   __zpabi_double_to_uint__d_2
   STA   __zpabi_double_to_uint__d_3
   STA   __zpabi_double_to_uint__d_4
   STA   __zpabi_double_to_uint__d_5
   LDA   #$25
   STA   __zpabi_double_to_uint__d_6
   JSR   main@run0
   BNE   .jcmp_skip@0
   LDA   HARGS+10
   BNE   .jcmp_skip@0
   LDA   HARGS+9
   BNE   .jcmp_skip@0
   LDA   HARGS+8
   CMP   #$0A
   BEQ   .if_end@0
.jcmp_skip@0:
   LDX   #$00
   LDA   #$01
   RTS
.if_end@0:
   LDA   #$00
   STA   __zpabi_double_to_uint__d_0
   STA   __zpabi_double_to_uint__d_1
   STA   __zpabi_double_to_uint__d_2
   STA   __zpabi_double_to_uint__d_3
   LDA   #$10
   STA   __zpabi_double_to_uint__d_4
   LDA   #$6A
   STA   __zpabi_double_to_uint__d_5
   LDA   #$E8
   STA   __zpabi_double_to_uint__d_6
   JSR   main@run0
   BNE   .jcmp_skip@1
   LDA   HARGS+10
   BNE   .jcmp_skip@1
   LDA   HARGS+9
   CMP   #$C3
   BNE   .jcmp_skip@1
   LDA   HARGS+8
   CMP   #$50
   BEQ   .if_end@1
.jcmp_skip@1:
   LDX   #$00
   LDA   #$02
   RTS
.if_end@1:
   LDA   #$00
   STA   __zpabi_double_to_ulong__d_0
   STA   __zpabi_double_to_ulong__d_1
   STA   __zpabi_double_to_ulong__d_3
   LDA   #$40
   STA   __zpabi_double_to_ulong__d_2
   LDA   #$65
   STA   __zpabi_double_to_ulong__d_4
   LDA   #$CD
   STA   __zpabi_double_to_ulong__d_5
   STA   __zpabi_double_to_ulong__d_6
   LDA   #$41
   STA   __zpabi_double_to_ulong__d_7
   JSR   double_to_ulong
   LDA   HARGS+23
   BNE   .jcmp_skip@2
   LDA   HARGS+22
   BNE   .jcmp_skip@2
   LDA   HARGS+21
   BNE   .jcmp_skip@2
   LDA   HARGS+20
   BNE   .jcmp_skip@2
   LDA   HARGS+19
   CMP   #$3B
   BNE   .jcmp_skip@2
   LDA   HARGS+18
   CMP   #$9A
   BNE   .jcmp_skip@2
   LDA   HARGS+17
   CMP   #$CA
   BNE   .jcmp_skip@2
   LDA   HARGS+16
   BEQ   .if_end@2
.jcmp_skip@2:
   LDX   #$00
   LDA   #$03
   RTS
.if_end@2:
   LDA   #$00
   STA   __zpabi_double_to_ulong__d_0
   STA   __zpabi_double_to_ulong__d_1
   STA   __zpabi_double_to_ulong__d_2
   LDA   #$C0
   STA   __zpabi_double_to_ulong__d_3
   LDA   #$0B
   STA   __zpabi_double_to_ulong__d_4
   LDA   #$5A
   STA   __zpabi_double_to_ulong__d_5
   LDA   #$E6
   STA   __zpabi_double_to_ulong__d_6
   LDA   #$41
   STA   __zpabi_double_to_ulong__d_7
   JSR   double_to_ulong
   LDA   HARGS+23
   BNE   .jcmp_skip@3
   LDA   HARGS+22
   BNE   .jcmp_skip@3
   LDA   HARGS+21
   BNE   .jcmp_skip@3
   LDA   HARGS+20
   BNE   .jcmp_skip@3
   LDA   HARGS+19
   CMP   #$B2
   BNE   .jcmp_skip@3
   LDA   HARGS+18
   CMP   #$D0
   BNE   .jcmp_skip@3
   LDA   HARGS+17
   CMP   #$5E
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

main@run0:
   SUBROUTINE

   LDA   #$40
   STA   __zpabi_double_to_uint__d_7
   JSR   double_to_uint
   LDA   HARGS+11
   RTS
