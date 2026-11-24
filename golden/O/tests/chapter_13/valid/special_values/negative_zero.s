SSP	EQU	$00
FP	EQU	$02
HARGS	EQU	$04
DPTR	EQU	$24
__zpabi_copysign__x_0	EQU	$80
__zpabi_copysign__x_1	EQU	$81
__zpabi_copysign__x_2	EQU	$82
__zpabi_copysign__x_3	EQU	$83
__zpabi_copysign__x_4	EQU	$84
__zpabi_copysign__x_5	EQU	$85
__zpabi_copysign__x_6	EQU	$86
__zpabi_copysign__x_7	EQU	$87
__zpabi_copysign__y_0	EQU	$88
__zpabi_copysign__y_1	EQU	$89
__zpabi_copysign__y_2	EQU	$8A
__zpabi_copysign__y_3	EQU	$8B
__zpabi_copysign__y_4	EQU	$8C
__zpabi_copysign__y_5	EQU	$8D
__zpabi_copysign__y_6	EQU	$8E
__zpabi_copysign__y_7	EQU	$8F
__local_main__8	EQU	$98
__local_main__9	EQU	$99
__local_main__10	EQU	$9A
__local_main__11	EQU	$9B
__local_main__12	EQU	$9C
__local_main__13	EQU	$9D
__local_main__14	EQU	$9E
__local_main__15	EQU	$9F

; @zp-link-meta-begin
; def main params= locals=__local_main__0,__local_main__1,__local_main__2,__local_main__3,__local_main__4,__local_main__5,__local_main__6,__local_main__7,__local_main__8,__local_main__9,__local_main__10,__local_main__11,__local_main__12,__local_main__13,__local_main__14,__local_main__15 indirect=false in_cycle=false
; ext copysign params=__zpabi_copysign__x_0,__zpabi_copysign__x_1,__zpabi_copysign__x_2,__zpabi_copysign__x_3,__zpabi_copysign__x_4,__zpabi_copysign__x_5,__zpabi_copysign__x_6,__zpabi_copysign__x_7,__zpabi_copysign__y_0,__zpabi_copysign__y_1,__zpabi_copysign__y_2,__zpabi_copysign__y_3,__zpabi_copysign__y_4,__zpabi_copysign__y_5,__zpabi_copysign__y_6,__zpabi_copysign__y_7
; call main -> copysign
; item main section=.text local=false zero=false
; item main@run1 section=.text local=true zero=false
; item main@run0 section=.text local=true zero=false
; item zero section=.bss local=false zero=true
; runtime hargs_size=32 hargs_need=24
; @zp-link-meta-end

main:
   SUBROUTINE

   LDA   zero
   STA   __local_main__15
   LDA   zero+1
   STA   __local_main__14
   LDA   zero+2
   STA   __local_main__13
   LDA   zero+3
   STA   __local_main__12
   LDA   zero+4
   STA   __local_main__11
   LDA   zero+5
   STA   __local_main__10
   LDA   zero+6
   STA   __local_main__9
   LDA   zero+7
   EOR   #$80
   STA   __local_main__8
   LDA   __local_main__15
   ORA   __local_main__14
   ORA   __local_main__13
   ORA   __local_main__12
   ORA   __local_main__11
   ORA   __local_main__10
   ORA   __local_main__9
   ORA   __local_main__8
   BEQ   .if_end@0
   LDX   #$00
   LDA   #$01
   RTS
.if_end@0:
   JSR   main@run1
   LDA   #$F0
   STA   HARGS+6
   LDA   #$3F
   STA   HARGS+7
   LDA   __local_main__15
   STA   HARGS+8
   LDA   __local_main__14
   STA   HARGS+9
   LDA   __local_main__13
   STA   HARGS+10
   LDA   __local_main__12
   STA   HARGS+11
   LDA   __local_main__11
   STA   HARGS+12
   LDA   __local_main__10
   STA   HARGS+13
   LDA   __local_main__9
   STA   HARGS+14
   LDA   __local_main__8
   STA   HARGS+15
   JSR   ddiv
   LDA   HARGS+23
   CMP   #$FF
   BNE   .jcmp_skip@0
   LDA   HARGS+22
   CMP   #$F0
   BNE   .jcmp_skip@0
   LDA   HARGS+21
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
   BEQ   .if_end@1
.jcmp_skip@0:
   LDX   #$00
   LDA   #$02
   RTS
.if_end@1:
   JSR   main@run1
   LDA   #$24
   STA   HARGS+6
   LDA   #$C0
   STA   HARGS+7
   LDA   __local_main__15
   STA   HARGS+8
   LDA   __local_main__14
   STA   HARGS+9
   LDA   __local_main__13
   STA   HARGS+10
   LDA   __local_main__12
   STA   HARGS+11
   LDA   __local_main__11
   STA   HARGS+12
   LDA   __local_main__10
   STA   HARGS+13
   LDA   __local_main__9
   STA   HARGS+14
   LDA   __local_main__8
   STA   HARGS+15
   JSR   ddiv
   LDA   HARGS+23
   CMP   #$7F
   BNE   .jcmp_skip@1
   LDA   HARGS+22
   CMP   #$F0
   BNE   .jcmp_skip@1
   LDA   HARGS+21
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
   BEQ   .if_end@2
.jcmp_skip@1:
   LDX   #$00
   LDA   #$03
   RTS
.if_end@2:
   LDA   __local_main__15
   ORA   __local_main__14
   ORA   __local_main__13
   ORA   __local_main__12
   ORA   __local_main__11
   ORA   __local_main__10
   ORA   __local_main__9
   ORA   __local_main__8
   BEQ   .if_end@5
   LDX   #$00
   LDA   #$04
   RTS
.if_end@5:
   LDA   __local_main__15
   ORA   __local_main__14
   ORA   __local_main__13
   ORA   __local_main__12
   ORA   __local_main__11
   ORA   __local_main__10
   ORA   __local_main__9
   ORA   __local_main__8
   BEQ   .if_end@6
   LDX   #$00
   LDA   #$05
   RTS
.if_end@6:
   LDA   zero+7
   CMP   #$80
   BNE   .jcmp_skip@2
   LDA   zero+6
   BNE   .jcmp_skip@2
   LDA   zero+5
   BNE   .jcmp_skip@2
   LDA   zero+4
   BNE   .jcmp_skip@2
   LDA   zero+3
   BNE   .jcmp_skip@2
   LDA   zero+2
   BNE   .jcmp_skip@2
   LDA   zero+1
   BNE   .jcmp_skip@2
   LDA   zero
   BEQ   .if_end@7
.jcmp_skip@2:
   LDX   #$00
   LDA   #$06
   RTS
.if_end@7:
   JSR   main@run0
   LDA   #$10
   STA   __zpabi_copysign__x_6
   LDA   #$40
   STA   __zpabi_copysign__x_7
   LDA   #$80
   STA   __zpabi_copysign__y_7
   JSR   copysign
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
   LDA   #$14
   STA   __zpabi_copysign__x_6
   LDA   #$C0
   STA   __zpabi_copysign__x_7
   JSR   main@run0
   STA   __zpabi_copysign__y_7
   JSR   copysign
   LDA   __local_main__8
   CMP   #$C0
   BNE   .jcmp_skip@3
   LDA   __local_main__9
   CMP   #$10
   BNE   .jcmp_skip@3
   LDA   __local_main__10
   BNE   .jcmp_skip@3
   LDA   __local_main__11
   BNE   .jcmp_skip@3
   LDA   __local_main__12
   BNE   .jcmp_skip@3
   LDA   __local_main__13
   BNE   .jcmp_skip@3
   LDA   __local_main__14
   BNE   .jcmp_skip@3
   LDA   __local_main__15
   BEQ   .if_end@8
.jcmp_skip@3:
   LDX   #$00
   LDA   #$07
   RTS
.if_end@8:
   LDA   HARGS+23
   CMP   #$40
   BNE   .jcmp_skip@4
   LDA   HARGS+22
   CMP   #$14
   BNE   .jcmp_skip@4
   LDA   HARGS+21
   BNE   .jcmp_skip@4
   LDA   HARGS+20
   BNE   .jcmp_skip@4
   LDA   HARGS+19
   BNE   .jcmp_skip@4
   LDA   HARGS+18
   BNE   .jcmp_skip@4
   LDA   HARGS+17
   BNE   .jcmp_skip@4
   LDA   HARGS+16
   BEQ   .if_end@9
.jcmp_skip@4:
   LDX   #$00
   LDA   #$08
   RTS
.if_end@9:
   LDX   #$00
   TXA
   RTS

main@run1:
   SUBROUTINE

   LDA   #$00
   STA   HARGS
   LDY   #$00
   STY   HARGS+1
   STY   HARGS+2
   STY   HARGS+3
   STY   HARGS+4
   STA   HARGS+5
   RTS

main@run0:
   SUBROUTINE

   LDA   #$00
   STA   __zpabi_copysign__x_0
   STA   __zpabi_copysign__x_1
   STA   __zpabi_copysign__x_2
   STA   __zpabi_copysign__x_3
   STA   __zpabi_copysign__x_4
   STA   __zpabi_copysign__x_5
   STA   __zpabi_copysign__y_0
   STA   __zpabi_copysign__y_1
   STA   __zpabi_copysign__y_2
   STA   __zpabi_copysign__y_3
   STA   __zpabi_copysign__y_4
   STA   __zpabi_copysign__y_5
   STA   __zpabi_copysign__y_6
   RTS

zero:
   DS.B  8
