SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__zpabi_fma__x_0	EQU	$80
__zpabi_ldexp__x_0	EQU	$80
__zpabi_fma__x_1	EQU	$81
__zpabi_ldexp__x_1	EQU	$81
__zpabi_fma__x_2	EQU	$82
__zpabi_ldexp__x_2	EQU	$82
__zpabi_fma__x_3	EQU	$83
__zpabi_ldexp__x_3	EQU	$83
__zpabi_fma__x_4	EQU	$84
__zpabi_ldexp__x_4	EQU	$84
__zpabi_fma__x_5	EQU	$85
__zpabi_ldexp__x_5	EQU	$85
__zpabi_fma__x_6	EQU	$86
__zpabi_ldexp__x_6	EQU	$86
__zpabi_fma__x_7	EQU	$87
__zpabi_ldexp__x_7	EQU	$87
__zpabi_fma__y_0	EQU	$88
__zpabi_ldexp__exp_0	EQU	$88
__zpabi_fma__y_1	EQU	$89
__zpabi_ldexp__exp_1	EQU	$89
__zpabi_fma__y_2	EQU	$8A
__zpabi_fma__y_3	EQU	$8B
__zpabi_fma__y_4	EQU	$8C
__zpabi_fma__y_5	EQU	$8D
__zpabi_fma__y_6	EQU	$8E
__zpabi_fma__y_7	EQU	$8F
__zpabi_fma__z_0	EQU	$90
__zpabi_fma__z_1	EQU	$91
__zpabi_fma__z_2	EQU	$92
__zpabi_fma__z_3	EQU	$93
__zpabi_fma__z_4	EQU	$94
__zpabi_fma__z_5	EQU	$95
__zpabi_fma__z_6	EQU	$96
__zpabi_fma__z_7	EQU	$97
__local_main__8	EQU	$A0
__local_main__9	EQU	$A1
__local_main__10	EQU	$A2
__local_main__11	EQU	$A3
__local_main__12	EQU	$A4
__local_main__13	EQU	$A5
__local_main__14	EQU	$A6
__local_main__15	EQU	$A7

; @zp-link-meta-begin
; def main params= locals=__local_main__0,__local_main__1,__local_main__2,__local_main__3,__local_main__4,__local_main__5,__local_main__6,__local_main__7,__local_main__8,__local_main__9,__local_main__10,__local_main__11,__local_main__12,__local_main__13,__local_main__14,__local_main__15 indirect=false in_cycle=false
; ext fma params=__zpabi_fma__x_0,__zpabi_fma__x_1,__zpabi_fma__x_2,__zpabi_fma__x_3,__zpabi_fma__x_4,__zpabi_fma__x_5,__zpabi_fma__x_6,__zpabi_fma__x_7,__zpabi_fma__y_0,__zpabi_fma__y_1,__zpabi_fma__y_2,__zpabi_fma__y_3,__zpabi_fma__y_4,__zpabi_fma__y_5,__zpabi_fma__y_6,__zpabi_fma__y_7,__zpabi_fma__z_0,__zpabi_fma__z_1,__zpabi_fma__z_2,__zpabi_fma__z_3,__zpabi_fma__z_4,__zpabi_fma__z_5,__zpabi_fma__z_6,__zpabi_fma__z_7
; ext ldexp params=__zpabi_ldexp__x_0,__zpabi_ldexp__x_1,__zpabi_ldexp__x_2,__zpabi_ldexp__x_3,__zpabi_ldexp__x_4,__zpabi_ldexp__x_5,__zpabi_ldexp__x_6,__zpabi_ldexp__x_7,__zpabi_ldexp__exp_0,__zpabi_ldexp__exp_1
; call main -> fma
; call main -> ldexp
; item main section=.text local=false zero=false
; runtime hargs_size=32 hargs_need=24
; @zp-link-meta-end

main:
   SUBROUTINE

   LDA   #$00
   STA   __zpabi_fma__x_0
   STA   __zpabi_fma__x_1
   STA   __zpabi_fma__x_2
   STA   __zpabi_fma__x_3
   STA   __zpabi_fma__x_4
   STA   __zpabi_fma__x_5
   STA   __zpabi_fma__z_0
   STA   __zpabi_fma__z_1
   STA   __zpabi_fma__z_2
   STA   __zpabi_fma__z_3
   LDA   #$14
   STA   __zpabi_fma__x_6
   LDA   #$40
   STA   __zpabi_fma__x_7
   LDA   #$92
   STA   __zpabi_fma__y_0
   LDA   #$D5
   STA   __zpabi_fma__y_1
   LDA   #$4D
   STA   __zpabi_fma__y_2
   LDA   #$06
   STA   __zpabi_fma__y_3
   LDA   #$CF
   STA   __zpabi_fma__y_4
   LDA   #$F0
   STA   __zpabi_fma__y_5
   LDA   #$80
   STA   __zpabi_fma__y_6
   STA   __zpabi_fma__z_4
   LDA   #$44
   STA   __zpabi_fma__y_7
   LDA   #$84
   STA   __zpabi_fma__z_5
   LDA   #$4E
   STA   __zpabi_fma__z_6
   LDA   #$41
   STA   __zpabi_fma__z_7
   JSR   fma
   LDA   HARGS+16
   STA   __local_main__15
   LDA   HARGS+17
   STA   __local_main__14
   LDA   HARGS+18
   STA   __local_main__13
   LDA   HARGS+19
   STA   __local_main__12
   LDA   HARGS+20
   STA   __local_main__11
   LDA   HARGS+21
   STA   __local_main__10
   LDA   HARGS+22
   STA   __local_main__9
   LDA   HARGS+23
   STA   __local_main__8
   LDA   #$C8
   STA   __zpabi_ldexp__x_0
   LDA   #$CA
   STA   __zpabi_ldexp__x_1
   LDA   #$B0
   STA   __zpabi_ldexp__x_2
   LDA   #$C4
   STA   __zpabi_ldexp__x_3
   LDA   #$9C
   STA   __zpabi_ldexp__x_4
   LDA   #$45
   STA   __zpabi_ldexp__x_5
   LDA   #$80
   STA   __zpabi_ldexp__x_6
   LDA   #$4F
   STA   __zpabi_ldexp__x_7
   LDA   #$05
   STA   __zpabi_ldexp__exp_0
   LDA   #$00
   STA   __zpabi_ldexp__exp_1
   JSR   ldexp
   LDA   __local_main__8
   CMP   #$44
   BNE   .jcmp_skip@0
   LDA   __local_main__9
   CMP   #$A5
   BNE   .jcmp_skip@0
   LDA   __local_main__10
   CMP   #$2D
   BNE   .jcmp_skip@0
   LDA   __local_main__11
   CMP   #$02
   BNE   .jcmp_skip@0
   LDA   __local_main__12
   CMP   #$C7
   BNE   .jcmp_skip@0
   LDA   __local_main__13
   CMP   #$E1
   BNE   .jcmp_skip@0
   LDA   __local_main__14
   CMP   #$4A
   BNE   .jcmp_skip@0
   LDA   __local_main__15
   CMP   #$F7
   BEQ   .if_end@0
.jcmp_skip@0:
   LDX   #$00
   LDA   #$01
   RTS
.if_end@0:
   LDA   HARGS+23
   CMP   #$4F
   BNE   .jcmp_skip@1
   LDA   HARGS+22
   CMP   #$D0
   BNE   .jcmp_skip@1
   LDA   HARGS+21
   CMP   #$45
   BNE   .jcmp_skip@1
   LDA   HARGS+20
   CMP   #$9C
   BNE   .jcmp_skip@1
   LDA   HARGS+19
   CMP   #$C4
   BNE   .jcmp_skip@1
   LDA   HARGS+18
   CMP   #$B0
   BNE   .jcmp_skip@1
   LDA   HARGS+17
   CMP   #$CA
   BNE   .jcmp_skip@1
   LDA   HARGS+16
   CMP   #$C8
   BEQ   .if_end@1
.jcmp_skip@1:
   LDX   #$00
   LDA   #$02
   RTS
.if_end@1:
   LDX   #$00
   TXA
   RTS
